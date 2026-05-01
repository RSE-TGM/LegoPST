/*
 * probe_attach.c — Diagnostic tool for the lg_fmu Linux porting
 *
 * Client that attaches to a running LegoPST simulation (started via
 * net_startup.sh on a task) and prints what it can see:
 *   - simulator state (RUN/STOP/FREEZE), simulated time, step counter
 *   - list of models (task)
 *   - if a variable name is given, its address, type, and current value
 *   - if a value is given, writes it to that variable and re-reads
 *
 * The write path is the only thing that touches shared memory. By default
 * it is allowed only on INPUT variables (kind == INGRESSO_NC/INGRESSO_C);
 * writing a STATE/OUTPUT requires --force.
 *
 * Build:    make -f Makefile.mk
 * Run:      ./probe_attach                     (info dump)
 *           ./probe_attach <VAR>               (info + read VAR)
 *           ./probe_attach <VAR> <VALUE>       (read + write VAR=VALUE + reread)
 *           ./probe_attach <VAR> <VALUE> --force   (allow write on STATE/OUTPUT)
 *
 * Requires: a simulation must be running (SHR_USR_KEY in env).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>
#include <sim_param.h>
#include <sim_types.h>
#include <sked.h>
#include <Rt/RtErrore.h>
#include <Rt/RtDbPunti.h>

/* libsim/var_sh.o references `extern sqlite3 *db` and the sqlite optional
 * branches (output_ascii_big, ...) that the probe never exercises.
 * Define db=NULL here so the runtime checks `if(db!=NULL)` short-circuit. */
sqlite3 *db = NULL;

/* Symbols from libsim/var_sh.c */
extern void  costruisci_var(char **p_ind, VARIABILI **p_var, int *id);
extern int   numero_modelli (char *);
extern int   numero_variabili (char *);
extern char *nome_modello (char *, int);


static int find_var_by_name(VARIABILI *vars, int n, const char *name)
{
    int i;
    char padded[16];
    /* Names in the SHM are 8-char space-padded, Fortran-style */
    snprintf(padded, sizeof(padded), "%-8s", name);
    padded[8] = '\0';
    for (i = 0; i < n; i++) {
        if (strncmp(vars[i].nome, padded, 8) == 0)
            return i;
    }
    return -1;
}

static const char *kind_str(int tipo)
{
    switch (tipo) {
        case STATO:       return "STATE/OUTPUT";
        case INGRESSO_NC: return "INPUT (free)";
        case INGRESSO_C:  return "INPUT (connected)";
        default:          return "?";
    }
}

static const char *stato_str(int s)
{
    switch (s) {
        case STATO_STOP:    return "STOP";
        case STATO_RUN:     return "RUN";
        case STATO_FREEZE:  return "FREEZE";
        default:            return "?";
    }
}


int main(int argc, char **argv)
{
    char *shr_env;
    int shr_usr_key;
    RtErroreOggetto errore;
    RtDbPuntiOggetto dbpunti;
    char *ind_sh_top = NULL;
    VARIABILI *variabili = NULL;
    int id_sh = 0;
    int nmod, ntot, i;

    /* 1. Check env */
    shr_env = getenv("SHR_USR_KEY");
    if (!shr_env || !*shr_env) {
        fprintf(stderr, "ERRORE: SHR_USR_KEY non e' definita.\n");
        fprintf(stderr, "  Il probe va eseguito in una shell con env LegoPST attiva\n");
        fprintf(stderr, "  e con una simulazione gia' avviata (net_startup.sh sulla task).\n");
        return 1;
    }
    shr_usr_key = atoi(shr_env);
    printf("SHR_USR_KEY = %d\n", shr_usr_key);

    /* 2. Attach to the simulator data SHM (libRt) */
    errore  = RtCreateErrore(RT_ERRORE_TERMINALE, "probe_attach");
    dbpunti = RtCreateDbPunti(errore, NULL, DB_PUNTI_INT, NULL);
    if (dbpunti == NULL) {
        fprintf(stderr, "\nERRORE: simulazione non attiva (RtCreateDbPunti=NULL).\n");
        fprintf(stderr, "  Avvia prima la simulazione con net_startup nella directory\n");
        fprintf(stderr, "  della task (es. /home/antonio/legocad/MDC_GV).\n");
        return 2;
    }
    printf("[OK] agganciato a ID_SHM_SIM (libRt/RtDbPunti)\n");

    /* 3. Attach to the variable-name SHM (libsim) */
    costruisci_var(&ind_sh_top, &variabili, &id_sh);
    if (!ind_sh_top || !variabili) {
        fprintf(stderr, "\nERRORE: costruisci_var fallito.\n");
        fprintf(stderr, "  variabili.rtf assente nella cwd? lancia il probe nella\n");
        fprintf(stderr, "  directory della task, dove ci sono S02/S04/S05/variabili.rtf.\n");
        RtDestroyDbPunti(dbpunti);
        return 3;
    }
    nmod = numero_modelli(ind_sh_top);
    ntot = numero_variabili(ind_sh_top);
    printf("[OK] agganciato a ID_SHM_VAR (modelli=%d, variabili tot=%d)\n",
           nmod, ntot);

    /* 4. Dump some simulator counters */
    {
        float t = -1.0f;
        int   stato = -1;
        int   step_cr = -1;
        if (RtDbPGetTime(dbpunti, &t))
            printf("  tempo simulato  = %.3f s\n", t);
        if (RtDbPGetStato(dbpunti, &stato))
            printf("  stato simulator = %d (%s)\n", stato, stato_str(stato));
        if (RtDbPGetStepCr(dbpunti, &step_cr))
            printf("  step counter    = %d\n", step_cr);
    }

    /* 5. List models */
    printf("\nModelli attivi:\n");
    for (i = 1; i <= nmod; i++)
        printf("  %2d) %s\n", i, nome_modello(ind_sh_top, i));

    /* 6. Optional: read (and optionally write) one variable by name */
    if (argc >= 2) {
        const char *varname = argv[1];
        int idx = find_var_by_name(variabili, ntot, varname);
        if (idx < 0) {
            printf("\nVariabile '%s' NON trovata.\n", varname);
        } else {
            float val = 0.0f;
            int ok = RtDbPGetValue(dbpunti, variabili[idx].addr, &val);
            printf("\nVariabile '%s':\n", varname);
            printf("  modello   = %d (%s)\n",
                   variabili[idx].mod,
                   nome_modello(ind_sh_top, variabili[idx].mod));
            printf("  blocco    = %d\n", variabili[idx].blocco);
            printf("  addr      = %d\n", variabili[idx].addr);
            printf("  tipo      = %s\n", kind_str(variabili[idx].tipo));
            printf("  descr     = %s\n", variabili[idx].descr);
            if (ok)
                printf("  VALORE    = %g\n", val);
            else
                printf("  RtDbPGetValue: lettura fallita\n");

            /* Step A: optional write */
            if (argc >= 3) {
                float new_val = (float) strtod(argv[2], NULL);
                int force = (argc >= 4 && strcmp(argv[3], "--force") == 0);
                int is_input = (variabili[idx].tipo == INGRESSO_NC ||
                                variabili[idx].tipo == INGRESSO_C);

                if (!is_input && !force) {
                    printf("\nSCRITTURA RIFIUTATA: '%s' non e' un INPUT.\n",
                           varname);
                    printf("  Aggiungi --force per scrivere comunque.\n");
                } else {
                    float reread = 0.0f;
                    Boolean wok = RtDbPPutValue(dbpunti,
                                                variabili[idx].addr,
                                                new_val);
                    printf("\nScrittura: %s = %g  (RtDbPPutValue=%s)\n",
                           varname, new_val, wok ? "OK" : "FAIL");
                    if (RtDbPGetValue(dbpunti, variabili[idx].addr, &reread))
                        printf("  rilettura = %g  (delta=%g)\n",
                               reread, reread - new_val);
                }
            }
        }
    } else {
        printf("\nSuggerimento:\n");
        printf("  ./probe_attach <VAR>           leggi una variabile\n");
        printf("  ./probe_attach <VAR> <VALUE>   scrivi una variabile (solo input)\n");
    }

    /* 7. Cleanup */
    RtDestroyDbPunti(dbpunti);
    return 0;
}
