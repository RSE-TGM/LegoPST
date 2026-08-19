/**********************************************************************
 *
 *      stazpag - elenca le pagine di faceplate e ne chiede la
 *                visualizzazione a xstaz.
 *
 *      Serve perche' l'unico modo previsto per aprire una pagina era il
 *      dialogo di net_monit (Alg_rt/net_simula/net_monit/monit_staz.c):
 *      chi avvia la simulazione con net_startup ha il banco (new_monit),
 *      che non ha quel dialogo, e xstaz da solo si limita a una finestra
 *      iconificata con il tasto Quit, in attesa di un messaggio.
 *
 *      Uso:
 *          stazpag              elenca le pagine definite in ./r02.dat
 *          stazpag <NOME>       chiede a xstaz di visualizzare la pagina
 *
 *      Va lanciato nella directory che contiene r02.dat, come xstaz.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

/*  xstaz.h dichiara campi di tipo Widget: serve l'header X, solo per le
    dichiarazioni (questo programma non chiama nulla di X e non ci si linka).  */
#include <X11/Intrinsic.h>

#include "sim_param.h"
#include "sim_types.h"
#include "xstaz.h"
#include "sim_ipc.h"

static void uso(const char *prog)
{
    fprintf(stderr, "\nuso:  %s [nome_pagina]\n\n", prog);
    fprintf(stderr, "  senza argomenti   elenca le pagine definite in ./r02.dat\n");
    fprintf(stderr, "  <nome_pagina>     chiede a xstaz di visualizzare quella pagina\n\n");
    fprintf(stderr, "Da eseguire nella directory che contiene r02.dat.\n");
    fprintf(stderr, "xstaz deve essere gia' in esecuzione:  xstaz 1 &\n\n");
    exit(1);
}

/*  Legge l'intestazione e l'elenco pagine di r02.dat. Ritorna il numero di
    pagine lette, -1 se il file non c'e'.  */
static int leggi_pagine(S_PAGINA **pagine)
{
    FILE *fp;
    HEAD_R02 header;
    int i;

    if (!(fp = fopen("r02.dat", "r")))
    {
        fprintf(stderr, "stazpag: r02.dat non trovato nella directory corrente.\n"
                        "         Spostarsi nella directory della regolazione, oppure\n"
                        "         compilare r01.dat con 'compstaz'.\n");
        return (-1);
    }
    if (fread(header.data, sizeof(HEAD_R02), 1, fp) != 1)
    {
        fprintf(stderr, "stazpag: r02.dat troncato (intestazione illeggibile).\n");
        fclose(fp);
        return (-1);
    }
    if (header.tot_pagine <= 0 || header.tot_pagine > MAX_PAG)
    {
        fprintf(stderr, "stazpag: r02.dat incoerente (tot_pagine=%d).\n", header.tot_pagine);
        fclose(fp);
        return (-1);
    }

    *pagine = (S_PAGINA *) calloc(header.tot_pagine, sizeof(S_PAGINA));
    if (*pagine == NULL)
    {
        fprintf(stderr, "stazpag: memoria insufficiente.\n");
        fclose(fp);
        return (-1);
    }
    for (i = 0; i < header.tot_pagine; i++)
    {
        if (fread(&(*pagine)[i], sizeof(S_PAGINA), 1, fp) != 1)
        {
            fprintf(stderr, "stazpag: r02.dat troncato alla pagina %d di %d.\n",
                    i + 1, header.tot_pagine);
            fclose(fp);
            return (-1);
        }
    }
    fclose(fp);
    return (header.tot_pagine);
}

int main(int argc, char **argv)
{
    S_PAGINA *pagine = NULL;
    RICHIESTA_STAZ richiesta;
    char *env;
    int shr_usr_key, id_coda, npag, i, trovata;

    if (argc > 2 || (argc == 2 && argv[1][0] == '-'))
        uso(argv[0]);

    if ((npag = leggi_pagine(&pagine)) < 0)
        exit(2);

    /* ---- senza argomenti: elenco ---- */
    if (argc == 1)
    {
        printf("\nPagine definite in r02.dat: %d\n\n", npag);
        printf("  %-10s %-52s %s\n", "NOME", "DESCRIZIONE", "STAZIONI");
        printf("  --------------------------------------------------------------------------\n");
        for (i = 0; i < npag; i++)
            printf("  %-10.*s %-52.*s %d\n",
                   LUN_NOM_PAG, pagine[i].nome,
                   LUN_DES_PAG, pagine[i].descrizione,
                   pagine[i].num_staz);
        printf("\nPer visualizzarne una:  xstaz 1 &   poi   %s <NOME>\n\n", argv[0]);
        free(pagine);
        exit(0);
    }

    /* ---- con un nome: verifica che esista, poi manda la richiesta ---- */
    trovata = 0;
    for (i = 0; i < npag; i++)
        if (!strncmp(argv[1], pagine[i].nome, LUN_NOM_PAG))
        {
            trovata = 1;
            break;
        }
    if (!trovata)
    {
        fprintf(stderr, "stazpag: la pagina '%s' non e' definita in r02.dat.\n", argv[1]);
        fprintf(stderr, "         Lanciare '%s' senza argomenti per l'elenco.\n", argv[0]);
        free(pagine);
        exit(3);
    }
    free(pagine);

    if ((env = getenv("SHR_USR_KEY")) == NULL)
    {
        fprintf(stderr, "stazpag: SHR_USR_KEY non definita: sorgiare il profilo LegoPST.\n");
        exit(4);
    }
    shr_usr_key = atoi(env);

    /*  Coda creata dallo schedulatore all'avvio della simulazione: qui la si
        aggancia soltanto (niente IPC_CREAT), cosi' l'assenza si vede subito
        invece di creare una coda che nessuno legge.  */
    if ((id_coda = msgget(shr_usr_key + ID_MSG_STAZ, 0)) == -1)
    {
        fprintf(stderr, "stazpag: coda %d (SHR_USR_KEY %d + ID_MSG_STAZ %d) non presente: %s\n",
                shr_usr_key + ID_MSG_STAZ, shr_usr_key, ID_MSG_STAZ, strerror(errno));
        fprintf(stderr, "         La simulazione non e' avviata (net_startup / net_simula).\n");
        exit(5);
    }

    memset(&richiesta, 0, sizeof(richiesta));
    richiesta.mtype = RIC_STAZ;
    strncpy(richiesta.nome_pagina, argv[1], LUN_NOM_PAG);

    if (msgsnd(id_coda, &richiesta, sizeof(richiesta.nome_pagina), IPC_NOWAIT) == -1)
    {
        fprintf(stderr, "stazpag: invio fallito sulla coda %d: %s\n",
                shr_usr_key + ID_MSG_STAZ, strerror(errno));
        exit(6);
    }

    printf("Richiesta inviata: pagina '%s'.\n", argv[1]);
    printf("Se non compare nulla, xstaz non e' in esecuzione: 'xstaz 1 &' in questa directory.\n");
    exit(0);
}
