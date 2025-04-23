/**********************************************************************
*
*       C Source:               sked_banco.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Nov  8 11:30:58 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: sked_banco.c-21 %  (%full_filespec: sked_banco.c-21:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)sked_banco.c	5.5\t3/14/96";
/*
   modulo sked_banco.c
   tipo 
   release 5.5
   data 3/14/96
   reserved @(#)sked_banco.c	5.5
*/
# include <stdio.h>
# include <errno.h>
# include <math.h>
# include <time.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <sys/stat.h>
#endif
#if defined VMS
# include <stdlib.h>
# include"vmsipc.h"
# include <stat.h>
#endif
# include <sim_param.h>
# include <sim_types.h>
# include <sim_ipc.h>
# include "comandi.h"
# include "sked.h"
# include "dispatcher.h"
# include "libnet.h"
# include "mod_data.h"
# include <Rt/RtDbPunti.h>
# include <Rt/RtMemory.h>

#include "libipc.h"

#include <curses.h>

#if defined SCO_UNIX
#define vfork() fork()
#endif

extern int      id_msg_pert;	/* id coda messaggi pert */
extern int      tipo_sked;

extern FILE    *fp_pert;


extern int      stato_sked;	/* definisce lo stato dello sked */

extern S02      s02_;

extern
SNTAB *snapshot_hea;			/* area shm per tabella snapshot */

#if defined BACKTRACK
extern
BKTAB *backtrack_hea;			/* area shm per tabella backtrack */
#endif

extern int      id_msg_banco;	/* id coda messaggi banco    */

extern int      id_msg_monit;	/* id coda messaggi monit    */

extern float    velocita;	/* velocita della simulazione */

extern float    fattore_step;	/* fattore moltiplicativo del passo di tempo                           */

extern int      fp_ordini[MAX_MODEL];	/* puntatori dei socket a cui spedire
					 * i comandi, solo per sked master */

extern RtDbPuntiOggetto dbpunti;
extern RtDbPuntiOggetto dbpunti_ext;

extern int      nrip;

extern double   tempo_iniziale;

extern double   tempo_reale;

extern double   tempo_freeze;

extern double   tempo_sim;

extern double   ritardo_sim;

extern int      iterazione_successiva;

extern float    passo_grafica;

extern int      nmod;

extern int      shr_usr_key;

extern STATISTICA_SKED statistica_sked;
int             update_sicre_status=0;

extern int      exit_sim;     /* flag che indica se e' arrivato un comando
                                 di STOP dal banco; consente di disattivare
                                 la ripartenza delle task.                  */

int             caricata_ci = 0;


int             id_msg_to_sked, id_msg_from_sked;

int             id_msg_to_mandb, id_msg_from_mandb;

int             id_sem_disp;

int             mandb_agganciato;

extern int      pid_bm_slave;

int             slot_snap[MAX_VAL_SNAP];	/* indirizzi delle variabili
						 * caratteristiche degli
						 * snapshot */

#if defined BACKTRACK
int             slot_bktk[MAX_VAL_SNAP];	/* indirizzi delle variabili
						 * caratteristiche dei
						 * backtrack */
#endif

int             id_sh;		/* identificatore sh_var */
VARIABILI      *variabili;	/* database variabili di tutti i modelli */
char           *ind_sh_top;	/* puntatore inizio shared memory sh_var */

#if defined MFFR
extern SNAP_SKED sommari_snapshot;
extern SNAP_SKED sommari_backtrack;
char            app_sommari_snapshot[sizeof(sommari_snapshot)];
char            app_sommari_backtrack[sizeof(sommari_backtrack)];
#endif

/*
 * memorizza la prima modifica dei dati durante la sessione per ciascuna task 
 */
int             modifica_dati[MAX_MODEL];

int             sked_banco_ack();
#if defined REPLAY
int             check_termine_replay();
double          calcola_tempo_max_replay();
#endif

int pid_disp;
int pid_moni;

int abilita_bktk=0;   /* Impedisce REPLAY se non preceduto da un LOADBT */

extern int    _MAX_SNAP_SHOT;
extern int    _MAX_BACK_TRACK;
extern int    _MAX_CAMPIONI;
extern int    _NUM_VAR;
extern int    _MAX_PERTUR;
extern int    _SPARE_SNAP;
extern int    _PERT_CLEAR;

int  attiva_listaci=0; /* Attiva la lettura degli slot */

int  rest_banco_err=0; 

float 	   dt, max_dt, max_sim_time=-1.0;


extern  int converti_tempo(float,long  *,long  *,long  *,long  *,long  *,long  *);

sked_banco(int modo, char *dati)
{
   int             ret;
   int             check;
   static int      tipo_attesa;
   int             i, k, j;
   char           *app;
   MSG_DISPATCHER  messaggio_dispatcher;
   MSG_DISPATCHER  messaggio_mandb;
   PACCHETTO_NET   messaggio_master;	/* messaggio da master */
   SNTAB           snapshot_sked;
   MSG_ACK         msg_ack;
   int             size_ack = sizeof(MSG_ACK) - sizeof(long);
   int             comando_ricevuto = 0;
   double          gettim();
   static int      prima_volta = 1;
   int             snapshot_selezionato;
#if defined BACKTRACK
   int             backtrack_selezionato;
   BKTAB           backtrack_sked;
#endif
   int             app_int;
   long            ore, minuti, secondi, giorno, mese, anno;
   int             posizione_slot;
   int             tot_variabili;
   char            app_var[MAX_LUN_NOME_VAR];
   static double   tempo_salvato = 0;
   static double   ritardo_salvato = 0;
   static double   tempo_massimo_replay = 0;
   static int      primo_ingresso_replay = 0;
   static int      entrato_stato_replay = 0;
   int             num_modello, num_blocco;
   char           *p_iniz;
   int             nrighe;
   int             legge_dati;
   int             legge_edf;
   char            pathf14[FILENAME_MAX];
   int             scrivi;
   char            eseguibile[50];
   char            appm[2], appb[2];
   int             size_comando;
   int             stato_ref;
   struct stat    *ref;
   int             sizefile = 0;
   int             tipo_modulo;
   char           *lista_m;
   char           *lista_s;
   char            n4lst[5];
   char            n8f14[9];
   int             no_message;


   if (modo == 0)
   {
      /* questa apertura andra' spostata in sked_prolog */
      if (prima_volta)
      {
	 prima_volta = 0;
//	 printf("sked_banco: Creazione code con dispatcher\n");
	 /* decodifica SHR_USR_KEY */
	 shr_usr_key = atoi((char *) getenv("SHR_USR_KEY"));
	 /* creazione code messaggio da dispatcher */
	 id_msg_to_sked = msg_create(shr_usr_key + ID_MSG_TO_SKED, 0);
	 id_msg_from_sked = msg_create(shr_usr_key + ID_MSG_FROM_SKED, 0);
	 /* creazione code messaggio da mandb */
	 id_msg_to_mandb = msg_create(shr_usr_key + ID_MSG_TO_MANDB, 1);
	 id_msg_from_mandb = msg_create(shr_usr_key + ID_MSG_FROM_MANDB, 1);

//	 printf("sked_banco: Creazione semaforo con dispatcher\n");
         id_sem_disp = sem_create(shr_usr_key + ID_SEM_DISP, 0);

         messaggio_dispatcher.mtype = 0;
	 /* attende il comando di initialize */
	 while (messaggio_dispatcher.mtype != SKDIS_INITIALIZE)
	 {
	    msg_rcv(id_msg_to_sked, &messaggio_dispatcher, 
                    sizeof(MSG_DISPATCHER),
		    (long)-999, !IPC_NOWAIT, TIMEOUT_INF);
            memcpy(&pid_disp,
                 &messaggio_dispatcher.comando_dispatcher.dati[0],sizeof(int));
            memcpy(&pid_moni,
                 &messaggio_dispatcher.comando_dispatcher.dati[4],sizeof(int));
	    sked_banco_ack(1);
//	    printf("Sked : ricevuto messaggio %d\n",messaggio_dispatcher.mtype);
	    if (messaggio_dispatcher.mtype == SKDIS_STOP)
	       exit(0);
	 }
	 tipo_attesa = !IPC_NOWAIT;
	 return;
      }
   }
/*
         ************************************
         * QUESTO IF CHE ESCLUDE GLI SLAVE  *
         * SI RITROVA ANCHE ALL'INTERNO DEI *
         * SEGUENTI COMANDI SKDIS_***:      *
         * -STEPSCALING   -STEPCR   -FREEZE-*
         * -CLEAR         -SAVEBT           *
         *  e' inutile                      *
         ************************************
*/
   if (tipo_sked == MASTER)
   {
      if (modo == 0)
      {
	 switch (stato_sked)
	 {
	 case STATO_STOP:
	    {
	       tipo_attesa = !IPC_NOWAIT;
	       break;
	    }
	 case STATO_FREEZE:
	    {
	       tipo_attesa = !IPC_NOWAIT;
	       break;
	    }
	 case STATO_BACKTRACK:
	    {
	       tipo_attesa = !IPC_NOWAIT;
	       break;
	    }
	 case STATO_RUN:
	    {
	       tipo_attesa = IPC_NOWAIT;
	       break;
	    }
#if defined REPLAY
	 case STATO_REPLAY:
	    {
	       tipo_attesa = IPC_NOWAIT;
	       check = check_termine_replay(tempo_massimo_replay);
               if( check==0 )
                   iterazione_successiva = 0;
               /*printf(" comando_ricevuto=%d iter=%d (dopo CHECK=%d)\n",
                       comando_ricevuto,iterazione_successiva,check);*/
	       break;
	    }
#endif
	 default:
	    {
	       tipo_attesa = IPC_NOWAIT;
	       break;
	    }
	 }


	 /* verifica che non ci siano messaggi da MANDB */
	 if (msg_rcv(id_msg_from_mandb, &messaggio_mandb, sizeof(MSG_DISPATCHER) -
		     sizeof(long), (long)-999, IPC_NOWAIT, TIMEOUT_INF) > 0)
	 {
//	    printf("\n\n\n RICEVUTO MESSAGGIO MANDB\n\n\n");
	    if (messaggio_mandb.mtype == SKDIS_AGGANCIO_MANDB)
	       mandb_agganciato = 1;
	    if (messaggio_mandb.mtype == SKDIS_SGANCIO_MANDB)
	       mandb_agganciato = 0;
	    /* comunica al mandb il ricevimento del messaggio */
	    msg_ack.mtype = 1000;
	    if(msg_snd(id_msg_to_mandb, &msg_ack, 0, !IPC_NOWAIT) == (-1)) {
               perror("sked_banco");
            }
	 }

//printf(" sked_banco: dt  = |%f| \n", max_dt);
	if (max_sim_time > -0.1)
	{
		if ( tempo_sim/1000 > (max_sim_time - max_dt) )
   		{
     			/* aggiorna lo stato dello sked */
     			sked_stato(STATO_FREEZE);
     			iterazione_successiva = 0;
  	 	}
	}

	 /* scoda il primo messaggio  dal dispatcher */
//printf("in sked_banco: prima di msg_rcv\n");
	 if (msg_rcv(id_msg_to_sked, &messaggio_dispatcher, 
                     sizeof(MSG_DISPATCHER), (long)-999, tipo_attesa, 
                     TIMEOUT_INF) > 0)
         {
	    comando_ricevuto = 1;
//printf("in sked_banco: dentro msg_rcv\n");
         }
      } else
      {
	 comando_ricevuto = 1;
	 messaggio_dispatcher.mtype = modo;
	 memcpy(messaggio_dispatcher.comando_dispatcher.dati, 
                dati, sizeof(int));
      }

      if (comando_ricevuto == 1)
      {
	 if (stato_sked == STATO_FREEZE)
	    iterazione_successiva = 0;
//printf("in sked_banco: messaggio_dispatcher.mtype=|%d|\n", messaggio_dispatcher.mtype);
	 switch (messaggio_dispatcher.mtype)
	 {
	 case SKDIS_MAXSIMTIME:
           {
	       printf("Comando SKDIS_MAXSIMTIME\n");
             // Inizializzazione del dt per anticipare il freeze in caso di max simulation time per net_operator
             RtDbPGetDt(dbpunti,0,  &dt);
             printf("sked_banco: inizializzazione dt = |%f| \n", dt);
             max_dt = dt; // dovrebbe poi diventare un ciclo per trovare il dt massimo tra le task
             memcpy(&max_sim_time,
                    messaggio_dispatcher.comando_dispatcher.dati,
                    sizeof(float));
             printf("Nuovo max_sim_time %f\n", max_sim_time);

	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(1);
	     break;
            }
	  case SKDIS_TIMESCALING:
	    {
	       printf("Comando SKDIS_TIMESCALING\n");
	       memcpy(&velocita,
		      messaggio_dispatcher.comando_dispatcher.dati,
		      sizeof(float));
	       printf("Nuova velocita %f\n", velocita);
	       RtDbPPutTimeScaling(dbpunti,velocita);

	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(1);
	       break;
	    }
	 case SKDIS_STEPSCALING:
	    {
	       printf("Comando SKDIS_STEPSCALING\n");
	       memcpy(&fattore_step,
		      messaggio_dispatcher.comando_dispatcher.dati,
		      sizeof(float));
	       printf("Nuovo fattore step %f\n", fattore_step);
	       RtDbPPutStepScaling(dbpunti,fattore_step);
	       /* invia il comando agli slave */
	       /* siamo gia' in un if analogo!!!! */
	       if (tipo_sked == MASTER)
	       {
		  messaggio_master.header_net.tipo = COMSIM;
		  messaggio_master.header_net.codice = STEPSCALING;
		  messaggio_master.header_net.flag =
		     (int) (fattore_step * 10);
		  messaggio_master.header_net.lun = 0;
		  messaggio_master.dato[0] = fattore_step;
		  for (i = 0; i < nmod; i++)
		     if (fp_ordini[i] > 0)
		     {
			if (writen(fp_ordini[i],
				   &messaggio_master,
				   sizeof(HEADER_NET) +
				   messaggio_master.header_net.lun) < 0)
			{
			   sked_stato(STATO_ERRORE);
			   printf("Impossibile com %d\n", i);
			   sked_errore();
			}
		     }
	       }
	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(1);
	       break;
	    }
	 case SKDIS_STEPCR:
	    {
	       printf("Comando SKDIS_STEPCR\n");
	       memcpy(&app_int,
		      messaggio_dispatcher.comando_dispatcher.dati,
		      sizeof(int));
	       RtDbPPutStepCr(dbpunti,app_int);
	       passo_grafica = (float) app_int;

	       printf("Nuovo passo %f\n", passo_grafica);
	       /* invia il comando agli slave */
	       /* siamo gia' in un if analogo!!!! */
	       if (tipo_sked == MASTER)
	       {
		  messaggio_master.header_net.tipo = COMSIM;
		  messaggio_master.header_net.codice = STEPCR;
		  messaggio_master.header_net.flag = (int) passo_grafica;
		  messaggio_master.header_net.lun = 0;
		  messaggio_master.dato[0] = passo_grafica;
		  for (i = 0; i < nmod; i++)
		     if (fp_ordini[i] > 0)
		     {
			if (writen(fp_ordini[i],
				   &messaggio_master,
				   sizeof(HEADER_NET) +
				   messaggio_master.header_net.lun) < 0)
			{
			   sked_stato(STATO_ERRORE);
			   printf("Impossibile com %d\n", i);
			   sked_errore();
			}
		     }
	       }
	       sked_stato(stato_sked);
	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(1);
	       break;
	    }
	 case SKDIS_STOP:
	    {
	       printf("Comando SKDIS_STOP\n");
	       /* aggiorna lo stato dello sked */
               exit_sim = 1;
	       sked_stato(STATO_STOP);
	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(1);
	       sked_fine();
	       break;
	    }
         case SKDIS_FREEZE_REST:
            {
               printf("Comando SKDIS_FREEZE_REST\n");
               for(i=0; i<nmod; i++) {
                  if(statistica_sked.stato_task[i] == 2) {
                     rest_banco_err = 1;
                     break;
                  }
               }
            }
	 case SKDIS_FREEZE:
	    {
               printf("Comando SKDIS_FREEZE\n");
               primo_ingresso_replay = 1;
               if (stato_sked != STATO_FREEZE)
                  {
		  /* se proviene dal backtrack ricarica il tempo */
		  if (stato_sked != STATO_RUN)
		     {
		     if (velocita == -1)
			velocita = 1;
		     tempo_sim = tempo_salvato;
		     RtDbPPutTime(dbpunti,(float)(tempo_salvato / 1000.0));
		     if (ritardo_salvato >= 0)
		     	RtDbPPutDelay(dbpunti,
				(float)(ritardo_salvato / 1000.0));
		     else
		     	RtDbPPutDelay(dbpunti,0.0);

		     RtDbPPutTime(dbpunti_ext,(float)(tempo_salvato / 1000.0));
		     }
#if defined SAVEPERT
		  /* invia il comando agli slave */
	          /* siamo gia' in un if analogo!!!! */
		  if (tipo_sked == MASTER)
                     {
		     messaggio_master.header_net.tipo = COMSIM;
/* 
Ora non invio piu' CLEAR_PERT ma CLEAR_PERT_FILE:
Tale modifica e' resa nessaria dall'eliminazione di init_area_pert per uscita
OK da BACKTRACK dopo una fase di REPLAY. Infatti le perturbazioni che rimangono
allo SLAVE esistono gia' in perturbazioni.dat ed al primo FREEZE dopo un RUN
verrebbero riscritte. Percio' lo SLAVE le cancellera' dal suo file ma non dalla
memoria shared.
*/
                     no_message = 0;
		     if (stato_sked == STATO_RUN)
                        {
/*                      Caso generale si proviene da un RUN */
			messaggio_master.header_net.codice = INVIO_PERT;
                        }
	             else
                         if ( entrato_stato_replay==1 )
                            {
/*                          Provenienza da BACKTRACK dopo un REPLAY */
                            messaggio_master.header_net.codice=CLEAR_PERT_FILE;
                            }
                         else
                            {
/*                          Provenienza da stato !RUN ma non da un REPLAY */
                            no_message = 1;
                            }
                     if( no_message==0 ) 
                       {
                       messaggio_master.header_net.lun = 0;
                       for (i = 0; i < nmod; i++)
		       if (fp_ordini[i] > 0)
                           {
                           if (writen(fp_ordini[i], &messaggio_master,
                                      sizeof(HEADER_NET) +
                                      messaggio_master.header_net.lun) < 0)
                              {
                              sked_stato(STATO_ERRORE);
                              printf("Impossibile com %d\n", i);
                              sked_errore();
                              }
                           }
                       }
		     if (stato_sked == STATO_RUN)
			sked_raccogli_pert();
         /*
          *    Ora non elimino piu' l'area pert
          *    perche' ho cariacato quella nuova
	  *          else
	  *             init_area_pert();
          */
                     } /* Fine if su tipo_sked==MASTER */
#endif
                  }    /* Fine if su stato_sked!=FREEZE */
               /* aggiorna lo stato dello sked */
               sked_stato(STATO_FREEZE);
               iterazione_successiva = 0;
               /* comunica al dispatcher il ricevimento del messaggio */
               sked_banco_ack(1);
               break;
	    }
	 case SKDIS_REMOVEPERT:
	    {
	       printf("Comando SKDIS_REMOVEPERT\n");
	       memcpy(&app_int,
		      messaggio_dispatcher.comando_dispatcher.dati,
		      sizeof(int));
	       remove_pert(app_int);
	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(ret);
	       break;
	    }
	 case SKDIS_NEWVARCR:
	    {
	       printf("Comando SKDIS_NEWVARCR\n");
	       if (stato_sked == STATO_FREEZE)
		  sked_pf22(2);
	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(ret);

	       sked_stato(stato_sked);
	       break;
	    }
	 case SKDIS_EDITPERT:
	    {
	       printf("Comando SKDIS_EDITPERT\n");
	       /* aggiorna lo stato dello sked */
	       if (iterazione_successiva == 0)
	       {
		  pert_rcv(id_msg_pert);
	       }
	       /*
	        * comunica al dispatcher il ricevimento del messaggio 
	        */
	       sked_banco_ack(1);
	       break;
	    }
#if defined BACKTRACK
	 case SKDIS_BACKTRACK:
	    {
	       printf("Comando SKDIS_BACKTRACK\n");
	       if (stato_sked == STATO_FREEZE)
	       {
/*                Aggiunto perche' senza averne caricato alcuno e scegliendo
                  OK questa variabile ha il valore precedente */
	          entrato_stato_replay = 0;
                  abilita_bktk = 0;
		  tempo_salvato = tempo_sim;
		  ritardo_salvato = ritardo_sim;
/*
//printf("sked_banco DEBUG: stato_sked = %d tempo_sim = %f ritardo_sim = %f\n",
        stato_sked, tempo_sim, ritardo_sim);
*/
	       }
	       if (stato_sked == STATO_REPLAY)
	       {
		  /*
		   * cambia la modalita' di accesso al file delle
		   * perturbazioni 
		   */
		  if (sked_replay(2) < 0)
		     fprintf(stderr,
			     "errore apertura file perturbazioni.dat\n");;
	       }
	       /* aggiorna lo stato dello sked */
	       sked_stato(STATO_BACKTRACK);
	       iterazione_successiva = 0;
	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(1);
	       break;
	    }
#endif
#if defined REPLAY
	 case SKDIS_REPLAY:
	    {
	       printf("Comando SKDIS_REPLAY\n");
               if( abilita_bktk == 0 )
                   {
//                   printf("abilita_bkck=%d \n",abilita_bktk);
	           sked_banco_ack(1);
                   break;
                   }
//	       printf("Primo_ingresso_replay=%d\n",primo_ingresso_replay);
	       /* annulla il ritardo */
	       ritardo_sim = 0;
	       tempo_massimo_replay = calcola_tempo_max_replay(tempo_salvato);
	       /*
	        * cambia la modalita' di accesso al file delle perturbazioni 
	        */
	       if (sked_replay(0) < 0)
		  printf("errore apertura file perturbazioni.dat\n");
	       /* aggiorna il timer */
	       tempo_iniziale = gettim();
	       /* tempo_reale = tempo_iniziale; */

	       /* aggiorna lo stato dello sked */
	       sked_stato(STATO_REPLAY);
	       iterazione_successiva = 1;
	       entrato_stato_replay = 1;
	       /* comunica al dispatcher il ricev. del messaggio */
	       sked_banco_ack(1);
	       break;
	    }
#endif
#if defined BANCO_MANOVRA
	 case SKDIS_ALLINEA:
	    {
	       printf("Comando SKDIS_ALLINEA\n");
	       if (pid_bm_slave > 0)
	       {
		  ret = 1;
		  statistica_sked.operazione_eseguita =
		     ESEGUITO_ALLINEAMENTO;
		  /*
		   * comunica al dispatcher il ricevimento del messaggio 
		   */
	       } else
	       {
		  statistica_sked.operazione_eseguita =
		     ESEGUITO_DEFAULT;
		  ret = NO_ALLINEA;	/* il banco manovra non e' configurato */
	       }
	       sked_banco_ack(ret);
	       sked_stato(stato_sked);
	       break;
	    }
#endif
	 case SKDIS_GO_UP:
	    {
	       printf("Comando SKDIS_GO_UP\n");
	       /* aggiorna lo stato dello sked */
	       iterazione_successiva = 1;
	       statistica_sked.operazione_eseguita = ESEGUITO_STEP;
	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(1);
	       break;
	    }
#if defined BACKTRACK
	 case SKDIS_VALIDATEBT:
	    {
	       printf("Comando SKDIS_VALIDATEBT  ");
/*             La sola differenza ora e' nell'export_db */
	       if (entrato_stato_replay == 1)
	       {
/*
                  Proveniendo da un replay nello stato backtyrack ed uscendo
                  con un OK si prosegue con le perturbazioni presenti tranne
                  che per gli updown che vengono eliminati.
*/
		  printf("con entrato_stato_replay=1 (iter=%d)\n",
                          iterazione_successiva);
		  messaggio_master.header_net.tipo = COMSIM;
		  messaggio_master.header_net.codice = CLEAR_UPDOWN;
		  messaggio_master.header_net.lun = 0;
		  for (i = 0; i < nmod; i++)
		     if (fp_ordini[i] > 0)
		     {
			if (writen(fp_ordini[i],
				   &messaggio_master,
				   sizeof(HEADER_NET) +
				   messaggio_master.header_net.lun) < 0)
			{
			   sked_stato(STATO_ERRORE);
			   printf("Impossibile com %d\n", i);
			   sked_errore();
			}
		     }
/*                Elimino le perturbazioni delle altre macchine */
                  printf("\t Elimino le perturbazioni delle altre macchine\n");
                  rm_pert_remote();
/*                Elimino gli updown */
		  cut_updown();

                  /**********
		  init_area_pert();
                  **********/
	       } else
	       {
		  printf(" con entrato_stato_replay=0 (iter success.=%d)\n",
                           iterazione_successiva);
/*                Tra le perturbazioni (di tutte le macchine) elimino gli 
                  updown e nego la variabile relativa.  */
		  rm_updown_from_pert();

/*                Invia il DB alle macchine slave */
		  export_db();	

/*                Elimino le perturbazioni delle altre macchine */
                  printf("\t Elimino le perturbazioni delle altre macchine\n");
                  rm_pert_remote();
	       }
/*             In modalita' _PERT_CLEAR si azzerano le perturbazioni */
               if ( _PERT_CLEAR==1 )
                  {
                  printf("\t\t VALIDATEBT con azzeramento perturbazioni\n");
                  init_area_pert();
                  }
/*             Riapro in append il file perturbazioni.dat */
               sked_replay(2);
/*             Comunica a dispatcher caricamento nuova condizione iniziale */
	       sked_banco_ack(NUOVA_CI);
	       break;
	    }
#endif
	 case SKDIS_RUN:
	    {
	       printf("Comando SKDIS_RUN\n");
	       /* aggiorna il timer */
	       tempo_iniziale = gettim();
	       /* aggiorna lo stato dello sked */
	       sked_stato(STATO_RUN);
	       iterazione_successiva = 1;
	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(1);
	       break;
	    }
	 case SKDIS_CLEAR:
	    {
	       printf("Comando SKDIS_CLEAR\n");
	       /* aggiorna lo stato dello sked */
	       tempo_sim = 0;
	       ritardo_sim = 0;
               max_sim_time = -1;
	       RtDbPPutTime(dbpunti,tempo_sim);
	       RtDbPPutTime(dbpunti_ext,tempo_sim);
	       RtDbPPutDelay(dbpunti,ritardo_sim);
	       RtDbPPutDelay(dbpunti_ext,ritardo_sim);
	       sked_stato(STATO_FREEZE);
	       iterazione_successiva = 0;
#if defined BACKTRACK
#if defined UNIX
	       system("rm -f perturbazioni.dat");
#if defined SNAP_PIAC
	       bktk_clear_piac();
#else
	       system("rm -f backtrack.dat");
#endif
#endif
#if defined VMS
	       system("delete/nolog/noconf perturbazioni.dat;*");
#if defined SNAP_PIAC
	       bktk_clear_piac();
#else
	       system("delete/nolog/noconf backtrack.dat;*");
#endif
#endif
#if defined SNAP_PIAC
	       bktk_tab_read_piac();
#else
	       bktk_tab_read();
#endif
#endif
#if defined SAVEPERT
	       /* chiude il file delle perturbazioni */
	       fclose(fp_pert);
	       /* siamo gia' in un if analogo!!!! */
	       if (tipo_sked == MASTER)
		  if (sked_replay(2) < 0)
		     printf("errore apertura append  perturbazioni.dat\n");
	       if (sked_replay(4) < 0)
		  printf("errore apertura file perturbazioni.dat\n");
#endif
/********************************************************************/
/*             Invia il messaggio analogo allo slave */
               messaggio_master.header_net.tipo = COMSIM;
               messaggio_master.header_net.codice = CLEAR;
               messaggio_master.header_net.lun = 0;
               for (i = 0; i < nmod; i++)
                   if (fp_ordini[i] > 0)
                      {
                      if (writen(fp_ordini[i], &messaggio_master,
                                 sizeof(HEADER_NET) +
                                 messaggio_master.header_net.lun) < 0)
                         {
                         sked_stato(STATO_ERRORE);
                         printf("Impossibile com %d\n", i);
                         sked_errore();
                         }
                      }
/********************************************************************/
	       /* azzero f22 */
	       sked_pf22(0);
/*             Dopo un clear viene imposto il BACKTRACK IMMEDIATO */
	       sked_banco_ack(NUOVA_CI);
	       break;
	    }
	 case SKDIS_LISTACI:
	    {
	       printf("SKED: listaci\n");
               attiva_listaci = 1;
	       memcpy(&posizione_slot,
		      messaggio_dispatcher.comando_dispatcher.dati,
		      sizeof(int));
	       memcpy(&app_var[0],
		      &messaggio_dispatcher.comando_dispatcher.dati[4],
		      MAX_LUN_NOME_VAR);
	       tot_variabili = numero_variabili(ind_sh_top);
	       for (i = 0; i < tot_variabili; i++)
	       {
		  if (!strncmp(variabili[i].nome, app_var, strlen(app_var)))
		     break;
	       }
	       if (i == tot_variabili)
	       {
		  printf("errore variabile non trovata\n");
		  sked_banco_ack(-1);
	       } else
	       {
		  slot_snap[posizione_slot - 1] = i;
#if defined SNAP_PIAC
		  snap_slot_piac(posizione_slot, i, 0);
#else
		  snap_slot(posizione_slot, i, 0);
#endif
#if defined BACKTRACK
		  slot_bktk[posizione_slot - 1] = i;
#if defined SNAP_PIAC
		  bktk_slot_piac(posizione_slot, i, 0);
#else
		  bktk_slot(posizione_slot, i, 0);
#endif
#endif
		  printf("Caricato lo slot %d con la variabile %s(%d)\n",
			 posizione_slot, app_var, variabili[i].addr);
		  sked_banco_ack(1);
	       }
	       break;
	    }
	 case SKDIS_SAVEIC:
	    {
	       printf("Comando SKDIS_SAVEIC\n");
               update_sicre_status=1;
               iterazione_successiva=0;
//               printf("t_sim prima di sked_task =%f\n",tempo_sim);
	       /*
               Chiamo la routine per svegliare le task
               senza compiere alcun passo di integrazione
               */
               sked_task_info(&update_sicre_status);
//               printf("t_sim dopo di sked_task =%f\n",tempo_sim);
	       memcpy(&snapshot_selezionato,
		      &messaggio_dispatcher.comando_dispatcher.dati[0],
		      sizeof(int));

	       memcpy(snapshot_sked.descr,
		      &messaggio_dispatcher.comando_dispatcher.dati[4],
		      SNAP_DESC);
	       snapshot_sked.prog = snapshot_selezionato;
	       i = snapshot_sked.prog;
	       if (snapshot_sked.prog == 0)
	       {
		  for (i = 0; i < _MAX_SNAP_SHOT; i++)
		     if (snapshot_hea[i].stat == 0)
			break;
		  if (i < _MAX_SNAP_SHOT)
		     snapshot_sked.prog = i;
		  else
		  {
		     fprintf(stderr, "sked:e' gia' stato scritto il massimo numero (%d) di snapshot\n", _MAX_SNAP_SHOT);
		     sked_banco_ack(-1);
		     break;
		  }
		  snapshot_hea[snapshot_sked.prog].prog = i + 1;
		  snapshot_hea[snapshot_sked.prog].stat = 1;
		  snapshot_hea[snapshot_sked.prog].mod = 0;
		  /* Calcola la posizione nella sezione dati */
#if defined COMPRESS_SNAPSHOT
		  snapshot_hea[snapshot_sked.prog].pos = 0;

#ifndef SNAP_PIAC
		  for (i = 0; i < _MAX_SNAP_SHOT; i++)
		     if (snapshot_hea[i].stat == 1)
			snapshot_hea[snapshot_sked.prog].pos++;
#endif
#else
		  if (snapshot_hea[snapshot_sked.prog].pos == -1)
		  {
		     snapshot_hea[snapshot_sked.prog].pos = 0;
		     for (i = 0; i < _MAX_SNAP_SHOT; i++)
			if (snapshot_hea[i].stat == 1)
			   snapshot_hea[snapshot_sked.prog].pos++;
		  }
#endif
	       } else
	       {
		  if (i > _MAX_SNAP_SHOT)
		  {
		     fprintf(stderr, "sked: gia' salvato il massimo numero (%d) di snapshot; snapshot_sked.prog =%d\n",
			     _MAX_SNAP_SHOT, snapshot_sked.prog);
		     sked_banco_ack(-1);
		     break;
		  }
		  --i;
		  snapshot_sked.prog = i;
		  snapshot_hea[snapshot_sked.prog].prog = i + 1;
		  snapshot_hea[snapshot_sked.prog].mod = 0;
		  if (snapshot_hea[snapshot_sked.prog].stat != 1)
		  {
		     /* calcola la posizione nella sezione dati */
#ifndef SNAP_PIAC
		     snapshot_hea[snapshot_sked.prog].pos = 1;
		     for (i = 0; i < _MAX_SNAP_SHOT; i++)
			if (snapshot_hea[i].stat == 1)
			   snapshot_hea[snapshot_sked.prog].pos++;
#endif
		     snapshot_hea[snapshot_sked.prog].stat = 1;
		  }
	       }
	       sprintf(snapshot_hea[snapshot_sked.prog].descr, "%s",
		       snapshot_sked.descr);

/*             Ora e data dello snapshot */
	       secondi = tempo_sim / 1000;
	       ore     = 0;
	       minuti  = 0;
	       secondi = 0;
	       giorno  = 1;
	       mese    = 1;
	       anno    = 2000;
	       converti_tempo((float) (tempo_sim / 1000),
			      &ore,    &minuti, &secondi, 
                              &giorno, &mese,   &anno);
	       sprintf(snapshot_hea[snapshot_sked.prog].temposn,
		       "%2d:%2d:%2d", ore, minuti, secondi);
	       data((int *)(&giorno), (int *)(&mese), (int *)(&anno));
	       sprintf(snapshot_hea[snapshot_sked.prog].datasn,
		       "%2d/%2d/%2d", giorno, mese, anno);
	       printf("Salvo Snapshot <%d> - %s - date = %s - time = %s\n",
		      snapshot_hea[snapshot_sked.prog].prog,
		      snapshot_hea[snapshot_sked.prog].descr,
		      snapshot_hea[snapshot_sked.prog].datasn,
		      snapshot_hea[snapshot_sked.prog].temposn);

#if defined MFFR
	       /* riceve i sommari_snapshot e malfunzioni remote */
	       i = 0;
	       for (k = 0; k <= (sizeof(SNAP_SKED) /
				 MAX_LUN_COMANDI_DISPATCHER); k++)
	       {
		  sked_banco_ack(1);
		  msg_rcv(id_msg_to_sked, &messaggio_dispatcher,
			  sizeof(MSG_DISPATCHER),
			  (long)-999, !IPC_NOWAIT, TIMEOUT_INF);
		  j = MAX_LUN_COMANDI_DISPATCHER;
		  if ((j + i) > sizeof(sommari_snapshot))
		     j = sizeof(sommari_snapshot) - i;
		  memcpy(&app_sommari_snapshot[i],
			 messaggio_dispatcher.comando_dispatcher.dati, j);
		  i += MAX_LUN_COMANDI_DISPATCHER;
		  /*printf("Ricevuto il pacchetto %d SMFR\n", k);*/
	       }
	       memcpy(&sommari_snapshot,
		      &app_sommari_snapshot[0], sizeof(sommari_snapshot));
	       /*printf("SKED : FINE SMFR\n");*/
#endif
#if defined SNAP_PIAC
	       sked_snapshot_piac(SAVESNAP, snapshot_sked.prog);
#else
	       sked_snapshot(SAVESNAP, snapshot_sked.prog);
#endif
	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(1);
	       break;
	    }
	 case SKDIS_MODIC:
	    {
	       memcpy(&snapshot_selezionato,
		      &messaggio_dispatcher.comando_dispatcher.dati[0],
		      sizeof(int));

	       memcpy(snapshot_sked.descr,
		      &messaggio_dispatcher.comando_dispatcher.dati[4],
		      SNAP_DESC);
	       printf("Comando SKDIS_MODIC %d %s\n", snapshot_selezionato,
		      snapshot_sked.descr);
	       sked_snap_mod_descr(snapshot_selezionato,
				   snapshot_sked.descr);
	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(1);
	       break;
	    }
	 case SKDIS_LOADIC:
	    {
	       memcpy(&snapshot_selezionato,
		      messaggio_dispatcher.comando_dispatcher.dati,
		      sizeof(int));
	       if (snapshot_selezionato <= _MAX_SNAP_SHOT)
		  snapshot_sked.prog = snapshot_selezionato;
	       else
	       {
		  fprintf(stderr, "sked:snapshot selezionato non corretto (%d, _MAX_SNAP_SHOT=%d)\n", snapshot_selezionato, _MAX_SNAP_SHOT);
		  sked_banco_ack(-1);
		  break;
	       }
	       if (snapshot_hea[snapshot_selezionato - 1].stat == 0)
	       {
		  sked_banco_ack(-1);
		  break;
	       }
	       printf("Carico Snapshot %d\n", snapshot_selezionato);
	       /* azzeramento shared memory */
               RtDbPClearData(dbpunti);

#if defined SNAP_PIAC
	       sked_snapshot_piac(LOADSNAP, snapshot_selezionato);
#else
	       sked_snapshot(LOADSNAP, snapshot_selezionato);
#endif
/*             Ora e' stata copiata l'area pert che ha appena letta */
               if ( _PERT_CLEAR==1 )
                  {
                  printf("SKDISLOADIC: eliminate le perturbazioni lette\n");
	          init_area_pert();
                  }
	       statistica_sked.operazione_eseguita = ESEGUITO_LOADIC;
	       statistica_sked.num_operazione = snapshot_sked.prog;
	       caricata_ci = 1;
/*             Introdotto per poter scrivere le pert anche a seguito dell'
               uscita dallo stato backtrack con un exit */
	       if (stato_sked == STATO_FREEZE)
                  sked_replay(2);
/*             Comunica a dispatcher caricamento nuova condizione iniziale */
	       sked_banco_ack(NUOVA_CI);
	       break;
	    }
	 case SKDIS_REMOVE:
	    {
	       memcpy(&snapshot_selezionato,
		      messaggio_dispatcher.comando_dispatcher.dati,
		      sizeof(int));
	       snapshot_sked.prog = snapshot_selezionato;
	       printf("Elimino Snapshot %d\n",
		      snapshot_sked.prog);
#if defined SNAP_PIAC
	       sked_snapshot_piac(DELSNAP, snapshot_sked.prog);
#else
	       sked_snapshot(DELSNAP, snapshot_sked.prog);
#endif
	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(1);
	       break;
	    }
#if defined BACKTRACK
	 case SKDIS_SAVEBT:
	    {
	       printf("Comando SKDIS_SAVEBT\n");
               update_sicre_status=1;
//               printf("t_sim prima di sked_task =%f\n",tempo_sim);
               /*
               Chiamo la routine per svegliare le task
               senza compiere alcun passo di integrazione
               */
               sked_task_info(&update_sicre_status);
//               printf("t_sim dopo di sked_task =%f\n",tempo_sim);

	       memcpy(&backtrack_selezionato,
		      &messaggio_dispatcher.comando_dispatcher.dati[0],
		      sizeof(int));
	       memcpy(backtrack_sked.descr,
		      &messaggio_dispatcher.comando_dispatcher.dati[4],
		      SNAP_DESC);
	       backtrack_sked.prog = backtrack_selezionato;
	       i = backtrack_sked.prog;
	       if (backtrack_sked.prog == 0)
	       {
		  for (i = 0; i < _MAX_BACK_TRACK; i++)
		     if (backtrack_hea[i].stat == 0)
			break;
		  if (i < _MAX_BACK_TRACK)
		     backtrack_sked.prog = i;
		  else
		  {
		     fprintf(stderr, "sked:e' gia' stato scritto il massimo numero (%d) di backtrack.\n", _MAX_BACK_TRACK);
		     sked_banco_ack(-1);
		     break;
		  }
		  backtrack_sked.prog = i;
		  backtrack_hea[backtrack_sked.prog].prog = i + 1;
		  backtrack_hea[backtrack_sked.prog].stat = 1;
		  backtrack_hea[backtrack_sked.prog].mod = 0;
		  backtrack_hea[backtrack_sked.prog].forzato = 0;
		  backtrack_hea[backtrack_sked.prog].tempo = (float) tempo_sim;
		  /* calcola la posizione nella sezione dati */
#ifndef SNAP_PIAC
		  backtrack_hea[backtrack_sked.prog].pos = 0;
		  for (i = 0; i < _MAX_BACK_TRACK; i++)
		     if (backtrack_hea[i].stat == 1)
			backtrack_hea[backtrack_sked.prog].pos++;
#endif
	       } else
	       {
		  if (i > _MAX_BACK_TRACK)
		  {
		     fprintf(stderr, "sked: gia' salvato il massimo numero (%d) di back_track; backtrack_sked.prog=%d\n",
			     _MAX_BACK_TRACK, backtrack_sked.prog);
		     sked_banco_ack(-1);
		     break;
		  }
		  --i;
		  backtrack_sked.prog = i;
		  backtrack_hea[backtrack_sked.prog].prog = i + 1;
		  backtrack_hea[backtrack_sked.prog].mod = 0;
		  backtrack_hea[backtrack_sked.prog].forzato = 0;
		  backtrack_hea[backtrack_sked.prog].tempo = tempo_sim;
		  if (backtrack_hea[backtrack_sked.prog].stat != 1)
		  {
		     /* calcola la posizione nella sezione dati */
#ifndef SNAP_PIAC
		     backtrack_hea[backtrack_sked.prog].pos = 1;
		     for (i = 0; i < _MAX_BACK_TRACK; i++)
			if (backtrack_hea[i].stat == 1)
			   backtrack_hea[backtrack_sked.prog].pos++;
#endif
		     backtrack_hea[backtrack_sked.prog].stat = 1;
		  }
	       }
	       sprintf(backtrack_hea[backtrack_sked.prog].descr, "%s",
		       backtrack_sked.descr);

/*             Ora e data del backtrack */
	       secondi = tempo_sim / 1000;
	       ore = 0;
	       minuti = 0;
	       secondi = 0;
	       giorno = 1;
	       mese = 1;
	       anno = 2000;
	       converti_tempo((float) (tempo_sim / 1000),
			      &ore,    &minuti, &secondi, 
                              &giorno, &mese,   &anno);
	       sprintf(backtrack_hea[backtrack_sked.prog].temposn,
		       "%2d:%2d:%2d", ore, minuti, secondi);
	       data((int*)(&giorno), (int*)(&mese), (int*)(&anno));
	       sprintf(backtrack_hea[backtrack_sked.prog].datasn,
		       "%2d/%2d/%2d",
		       giorno, mese, anno);
	       printf("Salvo bktk <%d>-[%s]-date=%s-time=%s tsim=%f\n",
		      backtrack_hea[backtrack_sked.prog].prog,
		      backtrack_hea[backtrack_sked.prog].descr,
		      backtrack_hea[backtrack_sked.prog].datasn,
		      backtrack_hea[backtrack_sked.prog].temposn,
                      tempo_sim);

	       /* riceve i sommari_backtrack e malfunzioni remote */
	       i = 0;
	       for (k = 0; k <= (sizeof(SNAP_SKED) / MAX_LUN_COMANDI_DISPATCHER); k++)
	       {
		  sked_banco_ack(1);
		  msg_rcv(id_msg_to_sked, &messaggio_dispatcher,
			  sizeof(MSG_DISPATCHER),
			  (long)-999, !IPC_NOWAIT, TIMEOUT_INF);
		  j = MAX_LUN_COMANDI_DISPATCHER;
		  if ((j + i) > sizeof(sommari_backtrack))
		     j = sizeof(sommari_backtrack) - i;
		  memcpy(&app_sommari_backtrack[i],
			 messaggio_dispatcher.comando_dispatcher.dati, j);
		  i += MAX_LUN_COMANDI_DISPATCHER;
	       }
	       memcpy(&sommari_backtrack, &app_sommari_backtrack[0], 
                      sizeof(sommari_backtrack));
#if defined SNAP_PIAC
	       sked_backtrack_piac(SAVEBKTK, backtrack_sked.prog);
#else
	       sked_backtrack(SAVEBKTK, backtrack_sked.prog);
#endif
	       /* comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(1);
#if defined SAVEPERT
	       /* invia il comando agli slave */
	       /* siamo gia' in un if analogo!!!! */
	       if (tipo_sked == MASTER)
	       {
		  messaggio_master.header_net.tipo = COMSIM;
		  messaggio_master.header_net.codice = INVIO_PERT;
		  messaggio_master.header_net.lun = 0;
		  for (i = 0; i < nmod; i++)
		     if (fp_ordini[i] > 0)
		     {
			if (writen(fp_ordini[i],
				   &messaggio_master,
				   sizeof(HEADER_NET) +
				   messaggio_master.header_net.lun) < 0)
			{
			   sked_stato(STATO_ERRORE);
			   printf("Impossibile com %d\n", i);
			   sked_errore();
			}
		     }
		  sked_raccogli_pert();
	       }
#endif
	       break;
	    }
	 case SKDIS_LOADBT:
	    {
	    float tempo_float;

	       printf("case SKDIS_LOADBT stato = %d\n", stato_sked);
	       /*
	        * se lo stato e' BACKTRACK pulisce l'area delle 
                * perturbazioni 
	        */
	       if (stato_sked == STATO_BACKTRACK)
	       {
		  entrato_stato_replay = 0;
		  init_area_pert();

		  messaggio_master.header_net.tipo = COMSIM;
		  messaggio_master.header_net.codice = CLEAR_AREA_PERT;
		  messaggio_master.header_net.lun = 0;
		  for (i = 0; i < nmod; i++)
		     if (fp_ordini[i] > 0)
		     {
			if (writen(fp_ordini[i],
				   &messaggio_master,
				   sizeof(HEADER_NET) +
				   messaggio_master.header_net.lun) < 0)
			{
			   sked_stato(STATO_ERRORE);
			   printf("Impossibile com %d\n", i);
			   sked_errore();
			}
		     }
	       }
	       memcpy(&backtrack_selezionato,
		      messaggio_dispatcher.comando_dispatcher.dati,
		      sizeof(int));
	       if (backtrack_selezionato <= _MAX_BACK_TRACK)
		  backtrack_sked.prog = backtrack_selezionato;
	       else
	       {
		  fprintf(stderr, "sked:backtrack selezionato non corretto (%d, _MAX_BACK_TRACK=%d)\n", backtrack_selezionato, _MAX_BACK_TRACK);
		  sked_banco_ack(-1);
		  break;
	       }
	       if (backtrack_hea[backtrack_selezionato - 1].stat == 0)
	       {
		  sked_banco_ack(-1);
		  break;
	       }
	       printf("Carico il backtrack %d \n",
		      backtrack_selezionato);
	       /* azzeramento shared memory */
		RtDbPClearData(dbpunti);
#if defined SNAP_PIAC
	       sked_backtrack_piac(LOADBKTK, backtrack_selezionato);
#else
	       sked_backtrack(LOADBKTK, backtrack_selezionato);
#endif
	       statistica_sked.operazione_eseguita = ESEGUITO_LOADIC;
	       statistica_sked.num_operazione = backtrack_sked.prog;
	       caricata_ci = 1;

               abilita_bktk = 1;

	       /*
	        * inserisce la tacca nell'ultimo backtrack registrato 
	        */
#if defined SNAP_PIAC
	       ins_tacca_bktk_piac(1);
#else
	       ins_tacca_bktk(1);
#endif
	       /* azzera il ritardo */
	       RtDbPPutDelay(dbpunti,0.0);
	       /* legge il tempo e lo memorizza */
	       RtDbPGetTime(dbpunti,&tempo_float);
	       tempo_sim = (double)tempo_float;
	       tempo_sim = tempo_sim * 1000;

/* 17-10       Per evitare una iterazione ad ogni load in stato backtrack */
               if( stato_sked == STATO_BACKTRACK )
	          iterazione_successiva = 0;

	       /* comunica al dispatcher il ricevimento del messaggio */
	       if (modo == 0)
	       {
		  primo_ingresso_replay = 1;
		  sked_banco_ack(1);
	       }
	       break;
	    }
#endif
	 case SKDIS_EDITDATA:
	    {
	       printf("sked_banco: ricevuto SKDIS_EDITDATA\n");
	       memcpy(&num_modello,
		      messaggio_dispatcher.comando_dispatcher.dati,
		      sizeof(int));
	       memcpy(&num_blocco,
		      &messaggio_dispatcher.comando_dispatcher.dati[4],
		      sizeof(int));
	       printf("modello %d blocco %d\n", num_modello, num_blocco);
	       strcpy(pathf14, s02_.model[num_modello - 1].path);
	       if (modifica_dati[num_modello - 1] == 0)
	       {
		  strcat(pathf14, "f14.dat");
		  modifica_dati[num_modello - 1] = 1;
	       } else
		  strcat(pathf14, "f14.mom");
	       legge_dati = leggi_dati_f14(ind_sh_top, pathf14,
					   num_modello, num_blocco, &p_iniz,
					   &nrighe);
	       if (legge_dati == -1)
	       {
		  fprintf(stderr, "sked:lettura blocco su f14 non riuscita\n");
		  free(p_iniz);
		  modifica_dati[num_modello - 1] = 0;
		  sked_banco_ack(-1);
		  break;
	       }
	       sked_banco_ack(1);
	       to_someone(id_msg_from_sked, id_msg_to_sked, 1, 1,
			  &nrighe, MAX_LUN_COMANDI_DISPATCHER);
	       i = 0;
	       for (k = 0;
		(k <= (nrighe * LUN_RIGA_F14) / MAX_LUN_COMANDI_DISPATCHER);
		    k++)
	       {
		  to_someone(id_msg_from_sked, id_msg_to_sked, 1, 1,
			     &p_iniz[i], MAX_LUN_COMANDI_DISPATCHER);
		  i += MAX_LUN_COMANDI_DISPATCHER;
	       }
	       free(p_iniz);
	       break;
	    }
	 case SKDIS_MODIDATA:
	    {
	       printf("sked:Comando SKDIS_MODIDATA\n");
	       printf("sked:numero modello=%d   numero blocco=%d\n",
		      num_modello, num_blocco);
	       memcpy(&nrighe,
		      &messaggio_dispatcher.comando_dispatcher.dati[0],
		      sizeof(int));
	       p_iniz = (char *) malloc(nrighe * LUN_RIGA_F14);
	       i = 0;
	       for (k = 0; k <= ((nrighe * LUN_RIGA_F14) /
				 MAX_LUN_COMANDI_DISPATCHER); k++)
	       {
		  sked_banco_ack(1);
		  msg_rcv(id_msg_to_sked, &messaggio_dispatcher,
			  sizeof(MSG_DISPATCHER),
			  (long)-999, !IPC_NOWAIT, TIMEOUT_INF);
		  j = MAX_LUN_COMANDI_DISPATCHER;
		  if ((j + i) > (nrighe * LUN_RIGA_F14))
		     j = (nrighe * LUN_RIGA_F14) - i;
		  memcpy(&p_iniz[i],
			 messaggio_dispatcher.comando_dispatcher.dati, j);
		  i += MAX_LUN_COMANDI_DISPATCHER;
		  printf("sked:Ricevuto il pacchetto %d DATI\n", k);
	       }
	       /*
	        * for(k=0;k<nrighe;k++) printf("sked:
	        * %s\n",&p_iniz[k*LUN_RIGA_F14]); 
	        */
	       /* Scrittura di f14.mom  */
	       strcpy(pathf14, s02_.model[num_modello - 1].path);
	       strcat(pathf14, "f14.dat");
	       scrivi = scrivi_dati_f14(ind_sh_top, pathf14, num_modello,
					num_blocco, p_iniz, nrighe);
	       free(p_iniz);
	       if (scrivi == -1)
	       {
		  fprintf(stderr, "sked:ERRORE IN SCRITTURA f14.mom\n");
		  sked_banco_ack(-1);
		  break;
	       }
	       /* Si controlla se e' um modulo di regolazione o di processo */
	       printf("Sked : Prima di scelta_lista\n");
	       tipo_modulo = scelta_lista(ind_sh_top, num_modello,
					  num_blocco, pathf14,
					  &lista_m, &lista_s, n4lst, n8f14);
	       printf("Sked : Dopo di scelta_lista\n");
	       free(lista_m);
	       free(lista_s);
	       if (tipo_modulo == -1)
	       {
		  fprintf(stderr, "sked:ERRORE RICERCA MODULO NELLE LISTE.\n");
		  sked_banco_ack(-1);
		  break;
	       }
	       /* Si controlla se esiste l'eseguibile refresh_dati libut */
	       if (tipo_modulo == 2)
		  strcpy(eseguibile, "../../legocad/libut_reg/refresh_dati");
	       else
	       if (tipo_modulo == 1)
		  strcpy(eseguibile, "../../legocad/libut/refresh_dati");
	       else
		  fprintf(stderr, "sked:ERRORE tipo TASK=%d\n",
			  tipo_modulo);
	       ref = (struct stat *) malloc(sizeof(struct stat));
	       if (stat(eseguibile, ref) != -1)
		  sizefile = (int) (ref->st_size);
	       free(ref);
	       if (sizefile > 0)
	       {
		  /* Lancio refresh(I2)     */
		  if (vfork())
		  {
		     wait(&stato_ref);
		     printf("Processo PADRE - pid=%d   ppid=%d\n",
			    getpid(), getppid());
		     printf("   il processo FIGLIO restituisce stato_ref=%d\n",
			    stato_ref);
		  } else
		  {
		     printf("Processo FIGLIO - pid=%d   ppid=%d\n",
			    getpid(), getppid());
		     sprintf(appm, "%-d", num_modello);
		     sprintf(appb, "%-d", num_blocco);
		     execl(eseguibile, eseguibile, appm, appb, pathf14, 0);
		  }
		  /* mauri  */
		  printf("sked:s02_.model[nmodello-1].punt_ider=%d\n",
			 s02_.model[num_modello - 1].punt_ider);
		  printf("sked:s02_.model[nmodello-1].punt_idati=%d\n",
			 s02_.model[num_modello - 1].punt_idati);
		  if (stato_ref != 0)
		  {
		     fprintf(stderr, "sked:dati non inseriti in memoria\n");
		     sked_banco_ack(-1);
		     break;
		  } else
		     legge_edf = legge_dati_var(ind_sh_top, num_modello,
						num_blocco);
		  if (legge_edf == -1)
		  {
		     fprintf(stderr, "sked:ERRORE LETTURA dati_var.edf.\n");
		     sked_banco_ack(-1);
		     break;
		  }
	       } else
	       {
		  fprintf(stderr,
			  "sked:impossibile accedere eseguibile %s\n",
			  eseguibile);
		  sked_banco_ack(-1);
		  break;
	       }
	       /* Comunica al dispatcher il ricevimento del messaggio */
	       sked_banco_ack(1);
	       break;
	    }
	 default:
	    {
	       printf("Comando [%d] non implementato\n",
		      messaggio_dispatcher.mtype);
	       /* comunica a dispatcher il ricevimento del messaggio */
	       sked_banco_ack(-1);
	       break;
	    }
	 }
//	 printf("Fine sked_banco\n");
      }
   }
}

int sked_banco_ack(int ret)
{
   MSG_ACK         msg_ack;
   int             size_ack = sizeof(MSG_ACK) - sizeof(long);

#if defined MAURIC
/* Ripristino stampe id_sem_disp */
   int valsem;
   int messaggi_code;
   struct msqid_ds buf;
#endif
   if ( (ret < 0)&&(ret!=NO_ALLINEA) )
      fprintf(stderr, "Errore [net_sked]: ritorno -1 a dispatcher\n");
   else if(ret==NO_ALLINEA)
          fprintf(stderr,"sked_banco_ack comunica NO_ALLINEA a dispatcher\n");
   msg_ack.ret = ret;
   msg_ack.mtype = 1000;
// printf("sked_banco_ack: 0 GUAG id_msg_from_sked=%d size_ack%d getpid=%d\n",id_msg_from_sked,size_ack,getpid());
// fflush(stdout);
   if(msg_snd(id_msg_from_sked, &msg_ack, size_ack, !IPC_NOWAIT) == -1) {
      perror("sked_banco_ack");
   }
   else {
   sem_signal_disp(id_sem_disp);
   }

#if defined MAURIC
/* Ripristino stampe id_sem_disp */
   msgctl(id_msg_from_sked, IPC_STAT, &buf);
   messaggi_code = buf.msg_qnum;
printf("sked_banco_ack: id_msg_from_sked    num_msg=%d\t tipo_msg=%d\n",
        messaggi_code,msg_ack.mtype);
   valsem = semctl(id_sem_disp,0,GETVAL,0);
printf("sked_banco_ack: valsem = %d\n",valsem);
   if(valsem>=5)
      beep();
#endif

}

#if defined REPLAY
/*
 * verifica se durante il funzionamento in replay e' stato superato il tempo
 * massimo 
 * Ritorni:
 *      0 Termine replay e cambio stato con ultimo bktk gia' caricato
 *      1 Ultimo bktk gia' caricato ma non termine replay
 *      2 Ultimo bktk non caricato e non termine replay
 */
int check_termine_replay(double tempo_massimo_replay)
{
static char     app[50];
extern STATO_CR stato_cr;
double          t_next_bt;
int             next_bt;
int             ret=-1;
/* 1/2000 *1.2 sec : per accettare di fermarsi nell'intorno di 
   tempo_massimo_replay ma il valore di epsilon scelto non e' accurato*/
float           epsilon=0.6; 

   if (tempo_sim >= (tempo_massimo_replay-epsilon) )
   {
      printf("\n\n\t TempoSim=[%f] supera tmaxreplay - e =[%f]:mi fermo\n",
              tempo_sim,tempo_massimo_replay-epsilon);
      if( tempo_sim < tempo_massimo_replay)
          printf("\n\n ATTENZIONE, nel limite di epsilon\n");
      sked_stato(STATO_BACKTRACK);
      stato_sked = STATO_BACKTRACK;
      ret=0;
      /*
       * RICARICA L'ULTIMO BACKTRACK CARICATO  (non piu')
       */
   }
/*
   else
      printf("\n\n\t TempoSim=[%f] NON SUPERA tmaxreplay=[%f]\n",
              tempo_sim,tempo_massimo_replay);
*/

   next_bt = stato_cr.last_bktk_load;
   if (stato_cr.last_bktk_load == stato_cr.last_bktk_save)
      {
      /*printf("last_bktk_load =last_bktk_save (%d)\n",
                stato_cr.last_bktk_save);*/
      if( ret!=0 )
         ret=1;
      return(ret);
      }
   if (next_bt == _MAX_BACK_TRACK)
      /*next_bt = 0;*/  /**** QUI e' molto dubbio secondo me corretto e' 1 ***/
      next_bt = 1;
   else
      next_bt = next_bt + 1;

   t_next_bt = backtrack_hea[next_bt - 1].tempo;
/* DEBUG
printf("\t next_bt=%d t_next_bt=%f tempo_sim=%f (L=%d S=%d)\n",
        next_bt,backtrack_hea[next_bt - 1].tempo,
        tempo_sim,
        stato_cr.last_bktk_load,
        stato_cr.last_bktk_save);
*/

/* Veniva impedito di ricaricare l'ultimo backtrack salvato ora non piu' */
   if ( (tempo_sim >= t_next_bt) )
   {
      printf("\t Stato Replay: carico il backtrack [%d]\n", next_bt);
      if( next_bt == stato_cr.last_bktk_save)
          printf("\t Nella passata gestione non sarebbe stato caricato\n");
      memcpy(app, &next_bt, sizeof(int));
      sked_banco(SKDIS_LOADBT, app);
   }
   if( ret!=0 )
       ret=2;
   return(ret);
}

double calcola_tempo_max_replay(double t_fin)
{
double          ret;
double          t_tacca = -1;
extern STATO_CR stato_cr;
double          t_att, t_mom;
int             i;
int             last_load = stato_cr.last_bktk_load;
int             last_save = stato_cr.last_bktk_save;

   /*
    * calcola il tempo del successivo backtrack taccato 
    */
   printf("Last back:load=%d save=%d t_fin=%f\n", 
           last_load,last_save,t_fin);
   t_att = -1;
   i = last_load;
   t_mom = backtrack_hea[i - 1].tempo;
   if( last_load==0 )
       {
       printf("\t PATCH calcola_tempo_max_replay dopo clear o inizio\n"); 
/*     NON CAPITA MAI */
       i=1;
       last_load = 1;
       }
   while (i != (last_load - 1))
   {
      printf("testo Back n.%d  t=%f forz=%d\n", 
              i, backtrack_hea[i - 1].tempo,
                 backtrack_hea[i - 1].forzato);
      if (i == stato_cr.last_bktk_save)
         {
         printf("\t Esco con break perche' i == stato_cr.last_bktk_save\n");
	 break;
         }
      if (backtrack_hea[i - 1].forzato != 0)
      {
	 if (i + 1 > _MAX_BACK_TRACK)
            {
	    t_att = backtrack_hea[0].tempo - nrip * 100;
            printf("\t Con back n.%d forz(%d) scelgo back 0 t_att=%f\n",
                    i, backtrack_hea[i - 1].forzato,t_att);
            }
	 else
            {
	    t_att = backtrack_hea[i].tempo - nrip * 100;
            printf("\t Con back n.%d forz(%d) scelgo back %d t_att=%f\n",
                    i, backtrack_hea[i - 1].forzato,i+1,t_att);
            }
	 break;
      }
      i++;
      /* test uscita ciclo    */
      if (i == _MAX_BACK_TRACK)
      {
	 if (last_load == 1)
	    break;
	 i = 1;
      }
   }

   if (t_att == -1)
      ret = t_fin;
   else
      ret = t_att;


   printf("Tempo massimo replay calcolato = %f\n", ret);
   return (ret);
}
#endif

to_someone(id, id_ret, tipo_dati, comando, dati, size)
   int             id;
   int             id_ret;
   int             tipo_dati;
   int             comando;
   char           *dati;
   int             size;
{
   MSG_DISPATCHER  messaggio;
   MSG_ACK         msg_ack;
   int             ret;

#if defined MAURIC
   int valsem;
#endif
   messaggio.comando_dispatcher.size = size;
   messaggio.comando_dispatcher.tipo = comando;
   if (size > 0)
      memcpy(&(messaggio.comando_dispatcher.dati[0]), dati, size);
   messaggio.mtype = (long int)tipo_dati;
   /*printf("sked: invio dati a dispatcher su coda %d\n", id);*/
printf("sked - to_someone: 0 GUAG invio a dispatcher - id=%d size=%d tipo=%d getpid=%d\n",id,size + 5 * sizeof(int),tipo_dati, getpid());
fflush(stdout);

   if (msg_snd(id, &messaggio, sizeof (COMANDO_DISPATCHER), !IPC_NOWAIT) == (-1)) {
      perror("to_someone");
   }
 
//  GUAG - era cosi 
//   if (msg_snd(id, &messaggio, size + 5 * sizeof(int)
//	        /* sizeof (COMANDO_DISPATCHER) */ , !IPC_NOWAIT) == (-1)) {
//     perror("to_someone");
//   }


printf("sked - to_someone: 1 GUAG dopo msg_snd id=%d size=%d sizeof (COMANDO_DISPATCHER)=%d getpid=%d\n",id,size + 5 * sizeof(int),sizeof (COMANDO_DISPATCHER), getpid());
fflush(stdout);

   sem_signal_disp(id_sem_disp);
printf("sked - to_someone: 2 GUAG dopo sem_signal_disp \n");
#if defined MAURIC 
   valsem = semctl(id_sem_disp,0,GETVAL,0);
printf("sked_to_someone: valsem = %d dopo sem_signal\n",valsem);
#endif

   if ((id_ret != (-1)) && (tipo_dati != DATI_ASINCRONI))
   {
      /*printf("sked: attendo ack dalla coda %d\n", id_ret);*/
      ret = msg_rcv(id_ret, &msg_ack, sizeof(long),(long)1000,!IPC_NOWAIT,TIME_ACK);
   }
}
