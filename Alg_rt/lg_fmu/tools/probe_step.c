/*
 * probe_step.c — Diagnostic tool: drive simulation cadence from outside
 *
 * Validates whether an external client can dictate the simulation pace.
 *
 * Two modes:
 *
 *  (1) Wallclock RUN/FREEZE:    ./probe_step <dt_wallclock_s>
 *      Sends SD_freeze, then SD_run, sleeps dt seconds, sends SD_freeze.
 *      The sim cadence is governed by net_sked (timescaling * dt_sim);
 *      FREEZE is honored at the next iteration boundary, so the actual
 *      sim advance is "dt * timescaling" rounded up to the next step.
 *
 *  (2) Exact step count (FMU-friendly):  ./probe_step --goup <N>
 *      Sends SD_goup N times (SKDIS_GO_UP). Each call advances exactly
 *      one sked iteration (one dt_sim of simulated time). This is the
 *      primitive that maps cleanly to FMI doStep — pick N so that
 *      N * dt_sim equals the FMI step.
 *
 * The dispatcher path (SD_run/SD_freeze/SD_goup with sender BI) is the
 * same one net_operator uses, so net_sked accepts these commands.
 *
 * Build:    make -f Makefile.mk probe_step
 * Run:      ./probe_step              (1 second wallclock RUN, default)
 *           ./probe_step 0.5          (500 ms wallclock RUN)
 *           ./probe_step --goup 1     (1 sked iteration, FMU style)
 *           ./probe_step --goup 10    (10 sked iterations)
 *
 * Requires: a simulation must be running and currently in FREEZE or RUN
 * (SHR_USR_KEY in env, dispatcher up).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sqlite3.h>
#include <sim_param.h>
#include <sim_types.h>
#include <sked.h>
#include <dispatcher.h>
#include <libdispatcher.h>
#include <Rt/RtErrore.h>
#include <Rt/RtDbPunti.h>

#define MITTENTE BI

sqlite3 *db = NULL;

static const char *stato_str(int s)
{
    switch (s) {
        case STATO_STOP:    return "STOP";
        case STATO_RUN:     return "RUN";
        case STATO_FREEZE:  return "FREEZE";
        default:            return "?";
    }
}

static void msleep(long ms)
{
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

static float read_time(RtDbPuntiOggetto db_)
{
    float t = -1.0f;
    RtDbPGetTime(db_, &t);
    return t;
}

static int read_stato(RtDbPuntiOggetto db_)
{
    int s = -1;
    RtDbPGetStato(db_, &s);
    return s;
}

static void print_cadence(RtDbPuntiOggetto dbpunti)
{
    float ts = -1.0f, dt0 = -1.0f;
    RtDbPGetTimeScaling(dbpunti, &ts);
    RtDbPGetDt(dbpunti, 0, &dt0);
    printf("  timescaling      = %.3f\n", ts);
    printf("  dt sked (mod 0)  = %.3f s\n", dt0);
}


static int run_wallclock(RtDbPuntiOggetto dbpunti, double dt, int initial_stato)
{
    long settle_ms = 250;
    float t0, t_freeze, t_after_run, t_final;
    int rc;

    t0 = read_time(dbpunti);
    printf("\n--- iniziale ---\n");
    printf("  stato = %s\n", stato_str(initial_stato));
    printf("  tempo = %.3f s\n", t0);
    print_cadence(dbpunti);

    rc = SD_freeze(MITTENTE);
    if (rc <= 0) fprintf(stderr, "  warning: SD_freeze=%d\n", rc);
    msleep(settle_ms);
    t_freeze = read_time(dbpunti);
    printf("\n--- dopo FREEZE (+%ld ms) ---\n", settle_ms);
    printf("  stato = %s   tempo = %.3f s   (delta = %+.3f)\n",
           stato_str(read_stato(dbpunti)), t_freeze, t_freeze - t0);

    rc = SD_run(MITTENTE);
    if (rc <= 0) fprintf(stderr, "  warning: SD_run=%d\n", rc);
    msleep((long)(dt * 1000.0));
    t_after_run = read_time(dbpunti);
    printf("\n--- dopo RUN per %.3f s wallclock ---\n", dt);
    printf("  stato = %s   tempo = %.3f s   (avanzamento = %+.3f s sim)\n",
           stato_str(read_stato(dbpunti)), t_after_run, t_after_run - t_freeze);

    rc = SD_freeze(MITTENTE);
    if (rc <= 0) fprintf(stderr, "  warning: SD_freeze=%d\n", rc);
    msleep(settle_ms);
    t_final = read_time(dbpunti);
    printf("\n--- dopo FREEZE finale (+%ld ms) ---\n", settle_ms);
    printf("  stato = %s   tempo = %.3f s   (delta da pre-run = %+.3f)\n",
           stato_str(read_stato(dbpunti)), t_final, t_final - t_freeze);

    if (initial_stato == STATO_RUN) {
        printf("\n[restore] stato originale era RUN, riporto in RUN.\n");
        SD_run(MITTENTE);
    }

    printf("\n=== SOMMARIO (modo wallclock) ===\n");
    printf("  tempo iniziale  : %.3f\n", t0);
    printf("  tempo finale    : %.3f\n", t_final);
    printf("  avanzamento tot : %+.3f s sim   (%.3f s wallclock richiesti)\n",
           t_final - t0, dt);
    printf("  rapporto sim/real = %.2fx\n",
           (dt > 0.001) ? (t_final - t0) / dt : 0.0);
    printf("\nNota: l'overshoot deriva dalla latenza di FREEZE (onorato a fine\n");
    printf("      iterazione): l'avanzamento e' (dt*timescaling) arrotondato\n");
    printf("      al multiplo superiore di dt_sked. Per step esatti usa --goup.\n");
    return 0;
}


static int run_goup(RtDbPuntiOggetto dbpunti, int n, int initial_stato)
{
    float t0, t_step, t_final;
    int   i, rc;
    long  poll_ms = 50;
    int   max_polls = 200;          /* 10 s di attesa massima per step */

    if (initial_stato != STATO_FREEZE) {
        rc = SD_freeze(MITTENTE);
        if (rc <= 0) fprintf(stderr, "  warning: SD_freeze=%d\n", rc);
        msleep(250);
    }

    t0 = read_time(dbpunti);
    printf("\n--- iniziale (modo GO_UP, N=%d) ---\n", n);
    printf("  stato = %s   tempo = %.3f s\n",
           stato_str(read_stato(dbpunti)), t0);
    print_cadence(dbpunti);

    for (i = 1; i <= n; i++) {
        float t_pre = read_time(dbpunti);
        int   k;

        rc = SD_goup(MITTENTE);
        if (rc <= 0) {
            fprintf(stderr, "  warning: SD_goup #%d ha ritornato %d\n", i, rc);
            break;
        }

        /* attendi che il tempo cambi */
        for (k = 0; k < max_polls; k++) {
            msleep(poll_ms);
            t_step = read_time(dbpunti);
            if (t_step > t_pre + 1e-4f) break;
        }
        printf("  step %3d: t=%.3f  (delta=%+.3f)%s\n",
               i, t_step, t_step - t_pre,
               (k >= max_polls) ? "  [TIMEOUT - sked non risponde]" : "");
        if (k >= max_polls) break;
    }

    t_final = read_time(dbpunti);

    if (initial_stato == STATO_RUN) {
        printf("\n[restore] stato originale era RUN, riporto in RUN.\n");
        SD_run(MITTENTE);
    }

    printf("\n=== SOMMARIO (modo --goup) ===\n");
    printf("  step richiesti  : %d\n", n);
    printf("  tempo iniziale  : %.3f\n", t0);
    printf("  tempo finale    : %.3f\n", t_final);
    printf("  avanzamento tot : %+.3f s sim\n", t_final - t0);
    return 0;
}


int main(int argc, char **argv)
{
    char *shr_env;
    int shr_usr_key;
    RtErroreOggetto errore;
    RtDbPuntiOggetto dbpunti;
    int initial_stato;
    int mode_goup = 0;
    int n_steps = 1;
    double dt = 1.0;

    if (argc >= 2 && strcmp(argv[1], "--goup") == 0) {
        mode_goup = 1;
        if (argc >= 3) n_steps = atoi(argv[2]);
        if (n_steps < 1 || n_steps > 1000) {
            fprintf(stderr, "ERRORE: --goup N richiede 1 <= N <= 1000\n");
            return 1;
        }
    } else {
        if (argc >= 2) dt = strtod(argv[1], NULL);
        if (dt <= 0.0 || dt > 60.0) {
            fprintf(stderr, "ERRORE: dt deve essere in (0, 60] secondi\n");
            return 1;
        }
    }

    shr_env = getenv("SHR_USR_KEY");
    if (!shr_env || !*shr_env) {
        fprintf(stderr, "ERRORE: SHR_USR_KEY non e' definita.\n");
        return 1;
    }
    shr_usr_key = atoi(shr_env);
    printf("SHR_USR_KEY = %d\n", shr_usr_key);
    if (mode_goup) printf("modo = --goup, N = %d\n\n", n_steps);
    else           printf("modo = wallclock, dt = %.3f s\n\n", dt);

    errore  = RtCreateErrore(RT_ERRORE_TERMINALE, "probe_step");
    dbpunti = RtCreateDbPunti(errore, NULL, DB_PUNTI_INT, NULL);
    if (dbpunti == NULL) {
        fprintf(stderr, "ERRORE: simulazione non attiva.\n");
        return 2;
    }
    printf("[OK] agganciato a ID_SHM_SIM\n");

    initial_stato = read_stato(dbpunti);
    if (initial_stato != STATO_RUN && initial_stato != STATO_FREEZE) {
        fprintf(stderr, "\nERRORE: simulazione in stato %s.\n",
                stato_str(initial_stato));
        fprintf(stderr, "  Lo step probe richiede RUN o FREEZE iniziale.\n");
        RtDestroyDbPunti(dbpunti);
        return 3;
    }

    if (mode_goup)
        run_goup(dbpunti, n_steps, initial_stato);
    else
        run_wallclock(dbpunti, dt, initial_stato);

    RtDestroyDbPunti(dbpunti);
    return 0;
}
