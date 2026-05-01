/*
 * probe_init.c — Diagnostic: send SD_inizializza(BI) to a stuck sim.
 *
 * Uso: ./probe_init
 * Contesto: net_sked partito ma fermo in attesa SKDIS_INITIALIZE
 * (perche' lanciato senza banco). Mandiamo COMANDO_INITIALIZE come BI=banco
 * istruttore, per replicare quello che farebbe il banco operatore.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sim_param.h>
#include <sim_types.h>
#include <dispatcher.h>
#include <libdispatcher.h>

int main(void)
{
    int rc;
    if (!getenv("SHR_USR_KEY")) {
        fprintf(stderr, "ERR: SHR_USR_KEY non definito\n");
        return 2;
    }
    rc = SD_inizializza(BI);
    printf("SD_inizializza(BI) -> %d\n", rc);
    return rc > 0 ? 0 : 1;
}
