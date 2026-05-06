/**********************************************************************
*
*       C Source:               sked.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Fri Oct 27 14:00:11 2006 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: sked.c-18 %  (%full_filespec: sked.c-18:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)sked.c	5.1\t11/7/95";
/*
   modulo sked.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)sked.c	5.1
*/
# include <stdio.h>
# include <errno.h>
# include <math.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <Rt/RtDbPunti.h>
# include <Rt/RtMemory.h>
# include "sim_param.h"
# include "sim_types.h"
# include "sim_ipc.h"
# include "comandi.h"
# include "sked.h"
# include "dispatcher.h"
# include "CAP.h"		/* Serve per la gestione delle licenze */
#include "skey_manager3.h"	/* Serve per la gestione delle licenze hardware*/
#include <sqlite3.h>

#include "sked_fun.h"

#define     MAXHOSTNAMELEN 256


int         stato_sked;	                    /* indica lo stato dello sked */
int         tipo_sked;	                    /* tipo di sked SLAVE=0 MASTER=1 */
char        nodo_master[MAX_LUN_HOST];	    /* nome del nodo MASTER   */
char        nodo_dispatcher[MAX_LUN_HOST];  /* nome del nodo DISPATCHER   */
char 	    save_path[2500];        /* salvataggio del PATH   */
int         fp_from_dispatcher;
int         fp_to_dispatcher;
int         fp_perturba;
int         nodo_dispatcher_attivo;
extern int  fp_ordini[MAX_MODEL];	/* puntatori dei socket a cui spedire
					 * i comandi, solo per sked master */
extern S02  s02_;     /* struttura modelli e var  */
extern int  nrip;     /* numero di ripetizioni della tabella di ripetizione */
extern int  nmod;     /* numero delle task */

extern RtDbPuntiOggetto dbpunti;

/*
 * tutti i tempi sono indicati in millisecondi       
 */
double      tempo_task;	         /* tempo impiegato dalle task per un ciclo */
double      tempo_rete;	         /* tempo impiegato dalla rete per un ciclo */
double      tempo_sim = 0;	 /* tempo della simulazione   */
double      tempo_reale = 0;     /* tempo reale   */
double      tempo_iniziale = 0;	 /* tempo iniziale della simulazione   */
double      tempo_freeze = 0;	 /* tempo per conteggio freeze  */
double      ritardo_sim = 0;     /* ritardo accumulato        */
double      tempo_license = 0.0; /* tempo limite assoluto di durata della
                                   licenza di tipo 'demo'                  */

#ifdef TEST_SIM_DISTR 
double      ritardo_sim_step = 0;    /* ritardo accumulato nel singolo passo*/
#endif

float       velocita = 1;	/* velocita della simulazione */
float       passo_grafica = 1;	/* passo registrazione grafica */
float       passo_legograf = 1;	/* passo aggiornamento legograf su MASTER   */
int         itab;		/* indice variabile tabella sincronizzazione */
int         indice_macchina;    /* indice della macchina  */
int         demone_attivo = 0;
MSG_NET     messaggio_master;	/* messaggio da master */
int         iterazione_successiva = 0;
/*
   Parametri letti dal file Simulator prima dell'inizializzazione
*/
int    _MAX_SNAP_SHOT;
int    _MAX_BACK_TRACK;
int    _MAX_CAMPIONI;
int    _NUM_VAR;
int    _MAX_PERTUR;
int    _SPARE_SNAP;
int    _PERT_CLEAR;
char   _SCADA_NAME[MAX_LUN_HOST];
char   _NO_PRIOLO[10];

int    id_msg_to_sked, id_msg_from_sked;

int    id_sem_disp;

double          gettim();

int    flag_demo = 0;       /* 0 ---> disponibile licenza tipo 'full'
                               1 ---> disponibile licenza tipo 'demo' (a tempo)
                            */

sqlite3 *db;


int main(int argc, char **argv)
{
int    i,err,ret;
int    shr_usr_key;
char   ptr[MAXHOSTNAMELEN];
char   Buffer[ENELINFO_LEN + 1];
int    indiceSocieta; //serve per gestire eventualmente comportamenti differenziati
char   messaggioErrore[1024];
int    errorCode;
int    richiestaScrittura;
double tempo_corr;


/*
   Si controlla se esiste la licenza per l'attivazione del programma.
   Prima la chiave hardware ed eventualmente quella software
*/
        // Controllo chiave hardware
        richiestaScrittura=KEYWRITE;
        errorCode=skey_verifica_e_carica3(richiestaScrittura, KEYLABEL, KEYPASSWD,
                                          LICRUNTIME, &indiceSocieta);
	if(errorCode != 0)
	{
// GUAG2025
//		skey_stampa_errore(errorCode, (char*)&messaggioErrore);
		skey_stampa_errore(errorCode, messaggioErrore);
		printf("NET_SKED:%s \n", messaggioErrore);

		//provo l'eventuale chiave software
		if(CAP(SCHED_LIC, Buffer) == -1)
		{
			exit(1);
		}
		if(manageLicense(Buffer, "NET_SKED", &flag_demo, &tempo_license) == -1)
		{
			exit(1);
		}
	}
	else
	{
		flag_demo=0;
		tempo_license=10000000;
	}


/*   
printf("net_sked main DEBUG: flag_demo = %d\ttempo_license = %lf\n",
       flag_demo, tempo_license);
*/
   ret = setTimeLicense(flag_demo, &tempo_license);
   if (ret == -1)
      exit(1);
/*         
printf("net_sked main DEBUG: flag_demo = %d\ttempo_license = %lf\n",
       flag_demo, tempo_license);
*/      

/* 
   Redirezione output  e stampa versione 
   testata("net_sked", SccsID);
*/

/* Prelega dall'environment i parametri caratteristici del lego */
   if(!GetParLego())
 	exit( fprintf(stderr, 
		"ATTENZIONE: i parametri del LEGO non sono definiti"));

/* Identificazione tipo di schedulatore  */
   if (argc > 1)
      {
      if (atoi(argv[1]) >= 1)
         {
	 tipo_sked = 1;
         gethostname(ptr,MAXHOSTNAMELEN);
         strcpy(nodo_master, ptr);
	 if (atoi(argv[1]) == 2)
            {
	    demone_attivo = 1;
            }
         }
      else
         {
	 tipo_sked = 0;
	 strcpy(nodo_master, argv[1]);
	 printf("MASTER --> %s\n", nodo_master);
	 indice_macchina = atoi(argv[2]);
	 printf("Indice della macchina = %d\n", indice_macchina);
         }
/*
      printf("\t sked: demone_attivo=%d tipo_sked=%d indice_macchina=%d\n",
               demone_attivo,tipo_sked,indice_macchina);
*/
      if( ((argc > 2)&&(tipo_sked==1)) || 
          ((argc > 3)&&(tipo_sked==0))  )
         {
         strcpy(_SCADA_NAME,"");
         strcpy(_NO_PRIOLO,"");
         for(i=1;i<argc-1;i++)
            {
            if(strcmp(argv[i],ARGV_N_SNAP)==0)
                  _MAX_SNAP_SHOT  = atoi(argv[i+1]);
            if(strcmp(argv[i],ARGV_N_BKTK)==0)
                  _MAX_BACK_TRACK = atoi(argv[i+1]);
            if(strcmp(argv[i],ARGV_N_CAMP_CR)==0)
                  _MAX_CAMPIONI   = atoi(argv[i+1]);
            if(strcmp(argv[i],ARGV_N_VAR_CR)==0)
                  _NUM_VAR        = atoi(argv[i+1]);
            if(strcmp(argv[i],ARGV_N_PERT_ACTIVE)==0)
                  _MAX_PERTUR     = atoi(argv[i+1]);
            if(strcmp(argv[i],ARGV_N_SPARE)==0)
                  _SPARE_SNAP     = atoi(argv[i+1]);
            if(strcmp(argv[i],ARGV_N_PERTCL)==0)
                  _PERT_CLEAR     = atoi(argv[i+1]);
            if(strcmp(argv[i],"-host_scada_name")==0)
               {
/*
   Si controlla se il primo carattere della entry successiva e' '-'. Se cio'
   accade nel file Simulator non e' stato assegnato un valore alla risorsa 
   SCADA e sulla linea di comando si ritrovano due entries consecutive.
*/ 
               if(argv[i+1][0] != '-')
                  strcpy(_SCADA_NAME,argv[i+1]);
               else
                  strcpy(_SCADA_NAME,"");
               }
            if(strcmp(argv[i],"-no_priolo")==0)
               {
/*
   Si controlla se il primo carattere della entry successiva e' '-'. Se cio'
   accade nel file Simulator non e' stato assegnato un valore alla risorsa 
   NO_PRIOLO e sulla linea di comando si ritrovano due entries consecutive.
*/ 
               if(argv[i+1][0] != '-') {
                  if(strcmp(argv[i+1],"YES") == 0)
                     strcpy(_NO_PRIOLO,argv[i+1]);
                  else
                     strcpy(_NO_PRIOLO,"");
               }
               else {
                  strcpy(_NO_PRIOLO,"");
               }
               }
            }
         }
      else
         fprintf(stderr,"net_sked senza parametri\n\n");
      }
    else
      exit(fprintf(stderr,"ATTENZIONE net_sked attivato senza parametri\n"));

   printf("Schedulatore con = [%d %d %d %d %d %d %d %s %s]\n", 
           _MAX_SNAP_SHOT ,
           _MAX_BACK_TRACK,
           _MAX_CAMPIONI  ,
           _NUM_VAR       ,
           _MAX_PERTUR    ,
           _SPARE_SNAP    ,
           _PERT_CLEAR    ,
           _SCADA_NAME    ,
           _NO_PRIOLO);

/*
   Si controllano i parametri di attivazione del processo con qelli
   letti sui file delle registrazioni attive : snapshot.dat, backtrack.dat
   f22circ.dat e si invia a dispatcher un messaggio.
*/
   if( tipo_sked == MASTER )
      {
      err = ControlParam(SKED);
      shr_usr_key = atoi((char *) getenv("SHR_USR_KEY"));
      id_msg_to_sked   = msg_create(shr_usr_key + ID_MSG_TO_SKED, 0);
      id_msg_from_sked = msg_create(shr_usr_key + ID_MSG_FROM_SKED, 0);

      id_sem_disp = sem_create(shr_usr_key + ID_SEM_DISP, 0);
      to_someone(id_msg_from_sked, id_msg_to_sked, 1, 1, (char*)&err, sizeof(int));
      }

/* Attende il comando di initialize */
   if (tipo_sked == MASTER)
      {
      sked_banco(0, NULL);
      }
#if defined REPLAY
   else
      {
      sked_replay(6); /* elimina il veccio file delle pertrubazioni */
      }
#endif
// printf("prima di sked_prolog_code \n");
   sked_prolog_code();
// printf("dopo di sked_prolog_code \n");

/* Aggangio-creazione semafori shared-memory code  */
   if (tipo_sked == MASTER)
   	sked_prolog();  /* lo sked slave esegue la sked_prolog in sked_s02 */

// printf("prima di sked_s02 \n");
   sked_s02();
// printf("dopo di sked_s02 \n");
/* Esecuzione lancio task */
   sked_start();

/* Gestione prima statistica */
   sked_statistiche();
   scoda_buffer(0);

   sked_legograf(0.0);
   sked_pf22(1);

/* Aggiorna lo stato dello sked */
   sked_stato(STATO_FREEZE);
   if (tipo_sked == MASTER)
   {
      sked_banco(0, NULL);
   } else
      sked_slave();

/* Loppo di schedulazione */
   while (1)
      {
/*    Giro task */
      if (tipo_sked == MASTER)
         {
         if(RtCheckPointer(NULL) == False)
            printf("!!! ATTENZIONE SPORCAMENTO MEMORIA\n");
	 if (iterazione_successiva == 1)
            {
#ifdef TEST_SIM_DISTR 
           ritardo_sim_step = 0.0;
#endif
           sked_next();
           setenv("PATH", save_path, 1);
           sked_task();
#ifdef TEST_SIM_DISTR 
printf("*******************************\n");
printf("sked.c main(): ritardo_sim_step = %f\n",ritardo_sim_step);
printf("*******************************\n");
#endif
           } 
         else
            sked_stato(stato_sked);
         } 
      else
         {
         if (iterazione_successiva == 1)
            sked_task();
         }
      if (tipo_sked == MASTER)
         {
/*       Blocca l'orologio se la simulazione e' in freeze */
	 tempo_freeze = gettim();
/*       Ricezione eventuali messaggi  da banco */
	 sked_banco(0, NULL);
         } 
      else
         {
/*       Scodamento messaggi sked non master */
#ifdef TEST_SKED_DISTR
printf("sked.c: prima di sked_slave()   tempo_corrente = %lf\n", gettim());
#endif
         sked_slave();
#ifdef TEST_SKED_DISTR
printf("sked.c: dopo di sked_slave()   tempo_corrente = %lf\n", gettim());
#endif
         }
      }/* while(1) */
}/* main */

