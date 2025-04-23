/**********************************************************************
*
*       C Source:               agg_scada.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Mar 31 10:23:11 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: agg_scada.c-8 %  (%full_filespec: agg_scada.c-8:csrc:1 %)";
#endif

/* 

	Processo di 
	
		AGGIORNAMENTO VERSO SUPERVISIONE

	Ver. 1.1		7/1/'93

*/
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#endif
#if defined VMS
# include "vmsipc.h"
#endif
#include "sim_param.h" 
# include "sim_types.h"
# include "sim_ipc.h"
#include "libnet.h"
#include "dispatcher.h" 
#include "comandi.h"	
#include "agg_scada.h"	/* include lato supervisione */
#include <Rt/RtDbPunti.h>
#include <Rt/RtErrore.h>
# include <Rt/RtMemory.h>
#include <statistics.h>
#include <sqlite3.h>


#define SERVER_SCADA_PORT		1964

#define MAXHOSTNAMELEN                  256

#define OFFSET_KEY_VAR_AGG              197
VARIABILI * variabili_shm, *pvar_shm;	/* database variabili di tutti i modelli */
VARIABILISLIM * variabili, *pvar;	/* database variabili di tutti i modelli */
char   *ind_sh_top;	/* puntatore inizio shared memory sh_var */
int     id_sh;		/* identificatore sh_var */
int     id_qsort;		/* identificatore sh_var */
int     id_qsort_key;		/* identificatore sh_var */
int     nmod;
int     tot_variabili;

RtDbPuntiOggetto dbpunti;
RtDbPuntiOggetto dbpunti_ext;
RtErroreOggetto errore;

pthread_t thread;

int task_residente[MAX_MODEL];
extern S02 s02_;                            /* struttura modelli e var */

void rallenta();
extern int ricevo_pert();

static int compara_var(VARIABILISLIM *, VARIABILISLIM *);
extern  int converti_tempo(float,long  *,long  *,long  *,long  *,long  *,long  *);

int id_msg_pert;

int     shr_usr_key;	/* chiave utente per shared  */
int     key_qsort;	/* chiave utente per shared  */

sqlite3 *db;

main (argc, argv)
int     argc;
char   *argv[];
{
int     fp_scada_com = 0;/* descrittore socket da SCADA per comandi */
int     fp_scada_pert = 0;/* descrittore socket da SCADA per perturbaz. */
int     comando= -1;
int     tipo;
int     size;
char    app1[100];
char    app2[100];
    SM_HEAD sm_head;
short   mess_ssc;
int     fine_ciclo = 0;
char    host[MAX_LUN_HOST];
int     fp_mom;
short   app_short;
long    ora,min,sec,giorno,mese,anno;
float   tempo;
int     i,kk;
char    host_locale[MAX_LUN_HOST];
int prima_volta =1;
SIMULATOR *simpar;
char    ptr[MAXHOSTNAMELEN];
VARIABILI *varp;
/*
debug by fabio
*/
printf("SONO IN AGG_ SCADA\n");
    if (argc != 2)
	errore ("Usa agg_scada nome_host");

    
    strcpy (host, argv[1]);


    /* redirezione output  e stampa versione */
/*
    testata("agg_scada",SccsID);
*/

    printf ("Processo di AGGIORNAMENTO VERSO SUPERVISIONE (%s) \n",
	    host);



/* collegamento al database simulatore */
/* decodifica SHR_USR_KEY */
    shr_usr_key = atoi ((char *) getenv ("SHR_USR_KEY"));
/* creazione o aggancio shared-memory   */
   simpar = (SIMULATOR*)calloc(1,sizeof(SIMULATOR));
   errore = RtCreateErrore(RT_ERRORE_TERMINALE,"net_sked");
   dbpunti = RtCreateDbPunti(errore,NULL,DB_PUNTI_INT,simpar);
   dbpunti_ext = RtCreateDbPunti(errore,NULL,DB_PUNTI_SHARED,simpar);
   free(simpar);

/* creazione o aggancio coda messaggi read->sked */
msg_create_fam(shr_usr_key,0);

/* collegamento database variabili */
    costruisci_var (&ind_sh_top, &variabili_shm, &id_sh);
/* legge numero dei modelli */
    nmod = numero_modelli (ind_sh_top);
    tot_variabili = numero_variabili (ind_sh_top);


/*
DEBUG

   varp=variabili;
for(i=0;i<tot_variabili;i++)
   {
   printf("DEBUG:varpname=%s\n",varp->nome);
   varp++;
   }
*/


/*****
DEBUG
   varp=variabili;
for(i=0;i<tot_variabili;i++)
   {
   printf("DEBUG:DOPO qsort varpname=%s\n",varp->nome);
   varp++;
   }
*****/




/*
	verifica le task residenti 
*/
gethostname(ptr,MAXHOSTNAMELEN);
strcpy(host_locale, ptr);
read_S02_net();
for(i=0;i<s02_.nmod;i++)
	if(strcmp(host_locale,s02_.model[i].host)==0)
		{
		printf("La task %d e' residente\n",i);
		task_residente[i]=1;
		}
	else
		{
		printf("La task %d non e' residente\n",i);
		task_residente[i]=0;
		}


/* collegamento database simulatore */

/* collegamento verso dispatcher */
    to_dispatcher (SUPERVISIONE, COMANDO_AGGANCIO, NULL, 0);

/* creazione socket verso supervisione */
    if (fp_scada_com = socketclient (host, 3000))
	printf ("agg_scada: Connessione con Supervisione eseguita (3000)\n");
    else
	errore ("Connessione con Supervisione fallita (3000)\n");

    if (fp_scada_pert = socketclient (host, 2050))
	printf ("agg_scada: Connessione con Supervisione eseguita (2050)\n");
    else
	errore ("Connessione con Supervisione fallita (2050)\n");

/*
	attivazione thread per ricevimento perturbazioni
*/

#if defined  NUTC || defined LINUX
pthread_create(&thread,NULL,ricevo_pert,
	(void *)fp_scada_pert);
#else
pthread_create(&thread,pthread_attr_default,ricevo_pert,
	(pthread_addr_t)fp_scada_pert);
#endif

/* nuovo prototype per la versione 4.0
pthread_create(&thread,NULL,ricevo_pert,
	(void *)fp_scada_pert);
*/

/* Inizio ciclo scodamento dati dispatcher */
    while (!fine_ciclo)
    {
   if(prima_volta == 0)
		{
		comando = -1;
		while(comando==(-1))
			{
			/* 
			scoda e	ventuali perturbazioni
			*/
    			//ricevo_pert(fp_scada_pert);
			comando = DATI_ASINCRONI;
			from_dispatcher (SUPERVISIONE,
				&comando, &tipo, &app1[0], &size, !IPC_NOWAIT);
			}
		if (size < 0)
	    	break;
		}
	else
		{
		prima_volta = 0;
	   tipo = START;
		}
	rallenta();
	switch (tipo)
	{
	    case START: 
		{
#ifdef PROVA
		    app_short = sizeof (SM_HEAD);
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = START;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    printf ("AGG_SCADA : invio START \n");
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
#endif


                    /*
                    Ordina variabili
                    Creo shm di appoggio dove stoccare variabili (se facessi 
                    allocazione, per Puerto non andrebbe bene -> troppe var)
                    In questa shm di apoggio ordino le variabili per 
                    aumentare velocita' comunicaz. tra scada e sim per
                    costruzione tabella di svincolo fra i punti.

                    Algoritmi usati : ordinamento qsort e bisezione.
                    */

                         if((getenv("SHR_USR_KEY"))==NULL)
                            {
                            printf("agg_scada: SHR_USR_KEYS non definita !!!\n")
;
                            return(NULL);
                            }

                    id_qsort_key=(atoi((char *)getenv("SHR_USR_KEY")))+OFFSET_KEY_VAR_AGG;
printf("DEBUG: agg_scada: id_qsort=%d \n",id_qsort_key);
                    variabili=(char*)crea_shrmem(id_qsort_key,tot_variabili * sizeof(VARIABILISLIM),&id_qsort);
                    if(variabili==NULL)
                      {
                      printf("ERROR: Impossible to allocate memory for agg_scada\n");
                      exit(2);
                      }

#ifdef PROVA
                      memcpy(variabili,variabili_shm,tot_variabili * sizeof(VARIABILI));
#endif
                     /*
                     Copio le informazioni necessarie dalla shm del Sim.
                     alla shm 'magra' per costruzione tab svicolo
                     punti SCADA e Sim. 
                     */
                     pvar=(VARIABILISLIM *)variabili;
                     pvar_shm=(VARIABILI *)variabili_shm;

                     for (kk=0;kk<tot_variabili;kk++)
                       {
                       strcpy(pvar->nome,pvar_shm->nome);
                       pvar->tipo=pvar_shm->tipo;
                       pvar->addr=pvar_shm->addr;
                       pvar ++;
                       pvar_shm++;
                       }


                      /* 
                         Ordino elenco variabili slim per applicare poi
                         bisezione sulla ricerca
                      */
                      qsort ( variabili, tot_variabili , sizeof(VARIABILISLIM) , compara_var );

                  /*
                  Spedisco messaggio di start a scada
                  */

		    app_short = sizeof (SM_HEAD);
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = START;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    printf ("AGG_SCADA : invio START \n");
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));



		    /* lettura tabelle di svincolo dati analogici e digitali */
		    ricevi_cfg (fp_scada_com);
			 ack_config_sked();
                    /*
                    Elimino shm di appoggio
                    */
                    if(sgancia_shrmem(variabili)<0)
                        printf("ERROR Impossible to detach shm\n");
                    distruggi_shrmem(id_qsort);
		    break;
		}
	    case STOP: 
		{
		    printf ("AGG_SCADA : invio STOP \n");
		    app_short = sizeof (SM_HEAD);
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = STOP;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI), sizeof (short));
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
		    ack_ssc (fp_scada_com);
		    fine_ciclo = 1;
		    break;
		}
	    case ALT: 
		{
		    printf ("AGG_SCADA : invio ALT \n");
		    app_short = sizeof (SM_HEAD);
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = ALT;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI), sizeof (short));
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
		    ack_ssc (fp_scada_com);
		    break;
		}
	    case RUN_SCADA: 
		{
		    printf ("AGG_SCADA : invio RUN \n");
		    app_short = sizeof (SM_HEAD);
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = RUN_SCADA;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI), sizeof (short));
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
		    ack_ssc (fp_scada_com);
		    break;
		}
	    case STEP: 
		{
		    printf ("AGG_SCADA : invio STEP \n");
		    app_short = sizeof (SM_HEAD);
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = STEP;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI), sizeof (short));
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
		    ack_ssc (fp_scada_com);
		    break;
		}
	    case FREEZE_SCADA: 
		{
		    printf ("AGG_SCADA : invio FREEZE \n");
		    app_short = sizeof (SM_HEAD);
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = FREEZE_SCADA;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI), sizeof (short));
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
		    ack_ssc (fp_scada_com);
		    break;
		}
	    case SIM_TIME: 
	    case SIM_TIME_INT: 
		{
                
		    /* trattamento variabili */
		    costruzione_pacchetti_digitali (fp_scada_com, 0);
			 /* 
					se il comando e' SIM_TIME vengono inviati solo 
					i digitali
			 */
			 if(tipo == SIM_TIME)
		    		costruzione_pacchetti_analogici (fp_scada_com, 0);
		    invia_dbupd (fp_scada_com);
		    app_short = sizeof (SM_HEAD) + sizeof (float)*7;
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = SIMTIME;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI), sizeof (short));
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
		    anno=1996;
		    mese=1;
		    giorno=1;
		    ora=0;
		    min=0;
		    sec=0;
		    memcpy (&tempo,&app1[0],sizeof (float));
		    converti_tempo(tempo,&ora,&min,&sec,&giorno,&mese,&anno);
	            /* converte il tempo foat*/
		    memcpy (&app2[0],
			    converti_float_f (&tempo, 
			    TRASMISSIONE, FORM_DATI), sizeof (float));
	            /* converte l'ora */
		    memcpy (&app2[4],
			    converti_int_f (&ora, 
			    TRASMISSIONE, FORM_DATI), sizeof (int));
	            /* converte i minuti */
		    memcpy (&app2[8],
			    converti_int_f (&min, 
			    TRASMISSIONE, FORM_DATI), sizeof (int));
	            /* converte i secondi */
		    memcpy (&app2[12],
			    converti_int_f (&sec, 
			    TRASMISSIONE, FORM_DATI), sizeof (int));
	            /* converte giorno */
		    memcpy (&app2[16],
			    converti_int_f (&giorno, 
			    TRASMISSIONE, FORM_DATI), sizeof (int));
	            /* converte mese */
		    memcpy (&app2[20],
			    converti_int_f (&mese, 
			    TRASMISSIONE, FORM_DATI), sizeof (int));
	            /* converte l'anno */
		    memcpy (&app2[24],
			    converti_int_f (&anno, 
			    TRASMISSIONE, FORM_DATI), sizeof (int));
		    writen (fp_scada_com, &app2[0], sizeof (int)*7);
		    /* attende l'ack */
		    ack_ssc (fp_scada_com);
		    break;
		}
	    case SIM_TIME_EXTRA: 
		{
printf ("AGG_SCADA : invio SIM_TIME_EXTRA \n");
		    app_short = sizeof (SM_HEAD) + sizeof (float)*7;
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = SIMTIME;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI), sizeof (short));
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
		    anno=1996;
                    mese=1;
                    giorno=1;
                    ora=0;
                    min=0;
                    sec=0;
                    memcpy (&tempo,&app1[0],sizeof (float));
                    converti_tempo(tempo,&ora,&min,&sec,&giorno,&mese,&anno);
                    /* converte il tempo foat*/
                    memcpy (&app2[0],
                            converti_float_f (&tempo,
                            TRASMISSIONE, FORM_DATI), sizeof (float));
                    /* converte l'ora */
                    memcpy (&app2[4],
                            converti_int_f (&ora,
                            TRASMISSIONE, FORM_DATI), sizeof (int));
                    /* converte i minuti */
                    memcpy (&app2[8],
                            converti_int_f (&min,
                            TRASMISSIONE, FORM_DATI), sizeof (int));
                    /* converte i secondi */
                    memcpy (&app2[12],
                            converti_int_f (&sec,
                            TRASMISSIONE, FORM_DATI), sizeof (int));
                    /* converte giorno */
                    memcpy (&app2[16],
                            converti_int_f (&giorno,
                            TRASMISSIONE, FORM_DATI), sizeof (int));
                    /* converte mese */
                    memcpy (&app2[20],
                            converti_int_f (&mese,
                            TRASMISSIONE, FORM_DATI), sizeof (int));
                    /* converte l'anno */
                    memcpy (&app2[24],
                            converti_int_f (&anno,
                            TRASMISSIONE, FORM_DATI), sizeof (int));
                    writen (fp_scada_com, &app2[0], sizeof (int)*7);
                    /* attende l'ack */
		    ack_ssc (fp_scada_com);
		    break;
		}
	    case LOADCI: 
		{
		    printf ("AGG_SCADA : invio LOADCI %d\n",(int)*app1);
		    app_short = sizeof (SM_HEAD) + sizeof (int);
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = LOADCI;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI), sizeof (short));
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
		    memcpy (&app2[0],
			    converti_int_f (&app1[0], TRASMISSIONE, FORM_DATI), sizeof (int));
		    writen (fp_scada_com, &app2[0], sizeof (int));
		    ack_ssc (fp_scada_com);
		    break;
		}
	    case INITDB: 
		{
		    printf ("AGG_SCADA : invio INITDB \n");
		    app_short = sizeof (SM_HEAD);
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = INITDB;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI), sizeof (short));
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
		    ack_ssc (fp_scada_com);
		    break;
		}
	    case DB_ALL: 
		{
printf ("AGG_SCADA : invio DB_ALL \n");
		    /* trattamento variabili */
		    costruzione_pacchetti_digitali (fp_scada_com, 1);
		    costruzione_pacchetti_analogici (fp_scada_com, 1);
		    invia_dbupd (fp_scada_com);
		    break;
		}
	    case DB_ANA: 
		{
printf ("AGG_SCADA : invio DB_ANA \n");
		    /* trattamento variabili */
		    costruzione_pacchetti_digitali (fp_scada_com, 0);
		    costruzione_pacchetti_analogici (fp_scada_com, 1);
		    invia_dbupd (fp_scada_com);
		    break;
		}
	    case LOADBT: 
		{
		    printf ("AGG_SCADA : invio LOADBT \n");
		    app_short = sizeof (SM_HEAD) + sizeof (int);
                    memcpy (&sm_head.nodo,
                            converti_short_f(&app_short,TRASMISSIONE,FORM_DATI),
                            sizeof (short));
                    mess_ssc = LOADBT;      
                    memcpy (&sm_head.mess,
                            converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI
), sizeof (short));
                    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
                    memcpy (&app2[0],
                            converti_int_f (&app1[0], TRASMISSIONE, FORM_DATI),
sizeof (int));
                    writen (fp_scada_com, &app2[0], sizeof (int));
		    ack_ssc (fp_scada_com);
		    break;
		}
	    case BKTKON: 
		{
		    printf ("AGG_SCADA : invio BKTKON \n");
		    app_short = sizeof (SM_HEAD);
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = BKTKON;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI), sizeof (short));
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
		    ack_ssc (fp_scada_com);
		    break;
		}
	    case BKTKOFF: 
		{
		    printf ("AGG_SCADA : invio BKTKOFF \n");
		    app_short = sizeof (SM_HEAD);
                    memcpy (&sm_head.nodo,
                          converti_short_f (&app_short, TRASMISSIONE,FORM_DATI),
                            sizeof (short));
                    mess_ssc = BKTKOFF;
                    memcpy (&sm_head.mess,
                            converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI
), sizeof (short));
                    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
                    ack_ssc (fp_scada_com);
		    break;
		}
	    case REPON: 
		{
		    printf ("AGG_SCADA : invio REPON \n");
		    app_short = sizeof (SM_HEAD);
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = REPON;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI), sizeof (short));
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
		    ack_ssc (fp_scada_com);
		    break;
		}
	    case REPOFF: 
		{
		    printf ("AGG_SCADA : invio REPOFF \n");
		    app_short = sizeof (SM_HEAD);
		    memcpy (&sm_head.nodo,
			    converti_short_f (&app_short, TRASMISSIONE, FORM_DATI),
			    sizeof (short));
		    mess_ssc = REPOFF;
		    memcpy (&sm_head.mess,
			    converti_short_f (&mess_ssc, TRASMISSIONE, FORM_DATI), sizeof (short));
		    writen (fp_scada_com, &sm_head, sizeof (SM_HEAD));
		    ack_ssc (fp_scada_com);
		    break;
		}
	    default: 
		{
		    printf ("\n\nAGG_SCADA:messaggio inesistente da dispatcher\n\n");
		}
	}
    }

printf("Attesa prima sconnessione\n");
sleep(5);
printf("Fine attesa prima sconnessione\n");

/* Fine ciclo scodamento dati dispatcher */
    printf ("Fine ciclo scodamento dati dispatcher\n");

/* scollegamento dal dispatcher */
    to_dispatcher (SUPERVISIONE, COMANDO_SGANCIO, NULL, 0);
}


void rallenta()
{
	sleep(0);
}

ack_config_sked()
{
MSG_AUS messaggio;
int id_msg_taskscada;

printf("SCADA: Invio ACK a sked\n");
/* manda allo sked il messaggio di esecuzione dello start-up */
id_msg_taskscada=msg_create(shr_usr_key+ID_MSG_AGG_SCADA,1);
messaggio.mtype=AGG_SCADA+OFFSET_AUS;
msg_snd(id_msg_taskscada,&messaggio,sizeof(TIPO_AUS),!IPC_NOWAIT);
}
/* 
Funzione per comparazione elementi di VARIABILI.rtf
Utile per ordinamento elenco variabili.rtf
*/
static int compara_var(VARIABILISLIM *var1, VARIABILISLIM *var2)
{
int ret;

ret=strcmp(var1->nome,var2->nome);
return ret ;

}
