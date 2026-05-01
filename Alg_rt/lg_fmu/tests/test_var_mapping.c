/*
 * test_var_mapping.c — exercise lg_var_mapping against a live simulation.
 *
 * Validates that:
 *   - lg_var_open attaches successfully
 *   - lg_var_count > 0, model count > 0
 *   - inputs/outputs partition correctly (n_input + n_output <= n_vars,
 *     remainder are INGRESSO_C)
 *   - lookup-by-name round-trips for a sample of variables (the first
 *     output and the first input)
 *   - the addr returned matches what RtDbPGetValue accepts
 *
 * Build:    make -f Makefile.mk
 * Run from a task cwd (e.g. /home/antonio/legocad/MDC_GV) with the env
 * set up and a simulation active:
 *   ./test_var_mapping
 *   ./test_var_mapping <NAME>      # also dump info for that variable
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>
#include <sim_param.h>
#include <sim_types.h>
#include <Rt/RtErrore.h>
#include <Rt/RtDbPunti.h>

#include "lg_var_mapping.h"

extern sqlite3 *db;   /* defined in lg_var_mapping.c */

static const char *kind_str(lg_var_kind k)
{
    switch (k) {
        case LG_VAR_OUTPUT:          return "OUTPUT";
        case LG_VAR_INPUT_FREE:      return "INPUT_FREE";
        case LG_VAR_INPUT_CONNECTED: return "INPUT_CONN";
        default:                     return "?";
    }
}

static int dump_one(const lg_var_mapping *m, RtDbPuntiOggetto dbpunti, int idx,
                    const char *header)
{
    lg_var_info info;
    float val = 0.0f;

    if (lg_var_get_info(m, idx, &info) != 0) {
        printf("%s: idx %d FUORI RANGE\n", header, idx);
        return -1;
    }
    printf("%s [idx=%d]\n", header, idx);
    printf("  name   = '%s'\n", info.name);
    printf("  kind   = %s\n", kind_str(info.kind));
    printf("  model  = %d (%s)\n", info.model,
           lg_var_model_name(m, info.model));
    printf("  block  = %d\n", info.block);
    printf("  addr   = %d\n", info.addr);
    printf("  descr  = %s\n", info.descr ? info.descr : "(null)");
    if (RtDbPGetValue(dbpunti, info.addr, &val))
        printf("  value  = %g\n", val);
    else
        printf("  value  = (RtDbPGetValue ha fallito)\n");

    /* round-trip: name -> idx -> name */
    {
        int back = lg_var_index_by_name(m, info.name);
        if (back == idx)
            printf("  round-trip lookup OK\n");
        else
            printf("  round-trip lookup FAIL: cercato '%s' -> idx=%d\n",
                   info.name, back);
    }
    return 0;
}

int main(int argc, char **argv)
{
    char *shr_env;
    int shr_usr_key;
    RtErroreOggetto errore;
    RtDbPuntiOggetto dbpunti;
    lg_var_mapping *m;
    int n_input, n_output, n_total, n_other;

    shr_env = getenv("SHR_USR_KEY");
    if (!shr_env || !*shr_env) {
        fprintf(stderr, "ERRORE: SHR_USR_KEY non e' definita.\n");
        return 1;
    }
    shr_usr_key = atoi(shr_env);
    printf("SHR_USR_KEY = %d\n", shr_usr_key);

    errore  = RtCreateErrore(RT_ERRORE_TERMINALE, "test_var_mapping");
    dbpunti = RtCreateDbPunti(errore, NULL, DB_PUNTI_INT, NULL);
    if (dbpunti == NULL) {
        fprintf(stderr, "ERRORE: RtCreateDbPunti=NULL (sim non attiva?)\n");
        return 2;
    }

    m = lg_var_open();
    if (!m) {
        fprintf(stderr, "ERRORE: lg_var_open fallito.\n");
        RtDestroyDbPunti(dbpunti);
        return 3;
    }

    n_total  = lg_var_count(m);
    n_input  = lg_var_input_count(m);
    n_output = lg_var_output_count(m);
    n_other  = n_total - n_input - n_output;

    printf("\n=== mapping aperto ===\n");
    printf("  modelli           : %d\n", lg_var_model_count(m));
    printf("  variabili totali  : %d\n", n_total);
    printf("  outputs (STATO)   : %d\n", n_output);
    printf("  inputs free (NC)  : %d\n", n_input);
    printf("  altri (INGR_C+?)  : %d\n", n_other);

    if (n_other < 0) {
        fprintf(stderr, "ERRORE: partizione non coerente (n_other=%d).\n", n_other);
        lg_var_close(m);
        RtDestroyDbPunti(dbpunti);
        return 4;
    }

    /* dump primo output e primo input (se presenti) */
    if (n_output > 0)
        dump_one(m, dbpunti, lg_var_output_at(m, 0), "\nprimo OUTPUT");
    if (n_input > 0)
        dump_one(m, dbpunti, lg_var_input_at (m, 0), "\nprimo INPUT_FREE");

    /* lookup esplicito da CLI */
    if (argc >= 2) {
        int idx = lg_var_index_by_name(m, argv[1]);
        if (idx < 0)
            printf("\nlookup '%s' -> NON TROVATO\n", argv[1]);
        else
            dump_one(m, dbpunti, idx, "\nlookup CLI");
    }

    /* Round-trip 1: by-name lookup. Names can be duplicated across
     * blocks, so a back!=i is not necessarily an error — we treat it as
     * a duplicate when the name actually matches. */
    {
        int i, errors = 0, dups = 0, ok = 0, shown = 0;
        for (i = 0; i < n_total; i++) {
            lg_var_info info_i, info_b;
            int back;
            if (lg_var_get_info(m, i, &info_i) != 0) continue;
            back = lg_var_index_by_name(m, info_i.name);
            if (back < 0) {
                if (shown < 10) {
                    printf("  NOT FOUND idx=%d name='%s'\n", i, info_i.name);
                    shown++;
                }
                errors++;
            } else if (back == i) {
                ok++;
            } else if (lg_var_get_info(m, back, &info_b) == 0 &&
                       strcmp(info_i.name, info_b.name) == 0) {
                dups++;       /* same name, different block — fine */
            } else {
                if (shown < 10) {
                    printf("  MISMATCH idx=%d name='%s' -> back=%d\n",
                           i, info_i.name, back);
                    shown++;
                }
                errors++;
            }
        }
        printf("\nRound-trip by-name : %d ok, %d duplicates, %d errors (totale %d)\n",
               ok, dups, errors, n_total);
        if (errors > 0) goto fail;
    }

    /* Round-trip 2: by-addr lookup. addr IS the unique SHM cell key,
     * but VARIABILI[] can have multiple entries pointing to the same
     * cell (block aliases). lg_var_index_by_addr returns the FIRST
     * matching entry, so back!=i is OK if back's addr == i's addr. */
    {
        int i, errors = 0, aliases = 0, ok = 0, shown = 0;
        for (i = 0; i < n_total; i++) {
            lg_var_info info_i, info_b;
            int back;
            if (lg_var_get_info(m, i, &info_i) != 0) continue;
            back = lg_var_index_by_addr(m, info_i.addr);
            if (back == i) ok++;
            else if (back >= 0 &&
                     lg_var_get_info(m, back, &info_b) == 0 &&
                     info_b.addr == info_i.addr) {
                aliases++;
            } else {
                if (shown < 10) {
                    printf("  ADDR ERROR idx=%d addr=%d -> back=%d\n",
                           i, info_i.addr, back);
                    shown++;
                }
                errors++;
            }
        }
        printf("Round-trip by-addr : %d ok, %d aliases, %d errors (totale %d)\n",
               ok, aliases, errors, n_total);
        if (errors > 0) goto fail;
    }

    /* Distinct-addr view: must have count == ok-by-addr (the canonical
     * entries) and every cell is reached exactly once. */
    {
        int n_unique = lg_var_unique_count(m);
        int n_in_unique = 0, n_out_unique = 0, k;
        printf("\n=== distinct-addr view ===\n");
        printf("  celle uniche      : %d\n", n_unique);
        for (k = 0; k < n_unique; k++) {
            lg_var_info info;
            int idx = lg_var_unique_at(m, k);
            if (lg_var_get_info(m, idx, &info) != 0) continue;
            if (info.kind == LG_VAR_OUTPUT)     n_out_unique++;
            else if (info.kind == LG_VAR_INPUT_FREE) n_in_unique++;
        }
        printf("  outputs uniche    : %d\n", n_out_unique);
        printf("  inputs uniche     : %d\n", n_in_unique);
    }

    lg_var_close(m);
    RtDestroyDbPunti(dbpunti);
    return 0;

fail:
    lg_var_close(m);
    RtDestroyDbPunti(dbpunti);
    return 5;
}

