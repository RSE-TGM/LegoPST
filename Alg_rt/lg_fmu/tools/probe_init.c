/*
 * probe_init.c — Diagnostic: send SD_inizializza(BI) e attende STATO_FREEZE.
 *
 * Uso: ./probe_init [--no-wait]
 *
 * Contesto: net_sked partito ma fermo in attesa SKDIS_INITIALIZE
 * (perche' lanciato senza banco). Mandiamo COMANDO_INITIALIZE come BI=banco
 * istruttore, per replicare quello che farebbe il banco operatore.
 *
 * Default: dopo SD_inizializza, polla RtDbPGetStato fino a STATO != STATO_STOP
 * (timeout 30s, poll a 200ms). Senza il polling, il chiamante non sa quando
 * la sim e' davvero pronta: lg5sk impiega secondi a leggere F04, fare LGTOPS
 * e arrivare al primo passo di reg_000 dove reg_wrshm scrive UU in SHM. Senza
 * questo, gen_modeldescription leggerebbe input free a 0 (default da
 * reg_prolog) invece dei valori di stazionario letti da F04.
 *
 * Con --no-wait ritorna subito dopo aver mandato il comando (vecchio
 * comportamento fire-and-forget).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sim_param.h>
#include <sim_types.h>
#include <dispatcher.h>
#include <libdispatcher.h>
#include <Rt/RtDbPunti.h>
#include <Rt/RtErrore.h>
#include <sked.h>

#define POLL_MS       200
#define MAX_POLLS     150  /* 150 * 200ms = 30s, come init_sim_if_stopped */

static void msleep_ms(long ms)
{
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };
    nanosleep(&ts, NULL);
}

int main(int argc, char **argv)
{
    int wait_freeze = 1;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--no-wait")) wait_freeze = 0;
        else { fprintf(stderr, "ERR: opzione sconosciuta: %s\n", argv[i]); return 2; }
    }

    if (!getenv("SHR_USR_KEY")) {
        fprintf(stderr, "ERR: SHR_USR_KEY non definito\n");
        return 2;
    }

    int rc = SD_inizializza(BI);
    printf("SD_inizializza(BI) -> %d\n", rc);
    if (rc <= 0) return 1;
    if (!wait_freeze) return 0;

    /* Attach al DB punti per leggere lo stato. file_top="TEST" → in caso di
     * SHM non valida si sgancia silenzioso, niente "[error shared-memory not
     * attached]" cosmetico (vedi lg_fmi2.c::try_attach_db). */
    RtErroreOggetto err = RtCreateErrore(RT_ERRORE_TERMINALE, "probe_init");
    RtDbPuntiOggetto db = RtCreateDbPunti(err, "TEST", DB_PUNTI_INT, NULL);
    if (!db) {
        fprintf(stderr, "ERR: RtCreateDbPunti fallito (sim viva e in stato consistente?)\n");
        return 3;
    }

    int last_s = -1;
    for (int j = 0; j < MAX_POLLS; ++j) {
        int s = -1;
        RtDbPGetStato(db, &s);
        if (s != last_s) {
            printf("probe_init: poll j=%d stato=%d\n", j, s);
            last_s = s;
        }
        if (s != STATO_STOP) {
            printf("probe_init: sim out of STOP dopo %d ms (stato=%d)\n",
                   (j + 1) * POLL_MS, s);
            return 0;
        }
        msleep_ms(POLL_MS);
    }
    fprintf(stderr, "ERR: timeout %ds attendendo STATO != STOP\n",
            MAX_POLLS * POLL_MS / 1000);
    return 4;
}
