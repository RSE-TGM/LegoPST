/**********************************************************************
*
*       C Source:               dispatcher.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Mon Dec  5 16:37:29 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: dispatcher.c-14 %  (%full_filespec: dispatcher.c-14:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)dispatcher.c	5.6\t3/14/96";
/*
   modulo dispatcher.c
   tipo 
   release 5.6
   data 3/14/96
   reserved @(#)dispatcher.c	5.6
*/
#include <stdio.h>
#include <string.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#endif
#if defined VMS 
# include <stdlib.h>
# include"vmsipc.h"
#endif
#include "sim_param.h"
#include "sim_ipc.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "sked.h"
#include "agg_scada.h"

#include "libipc.h"

#if defined BANCO_MANOVRA
#include "agg_manovra.h"
#endif
# include "mod_data.h"
# include <Rt/RtErrore.h>
# include <Rt/RtDbPunti.h>

#define ATTESA_SKED        20

RtDbPuntiOggetto dbpunti;
RtErroreOggetto errore;
int size_area_dati;


TIPO_PERT *perturbazioni;	/* area shm per tabella pert */
TIPO_PERT *perturbazioni_att;   /* area shm per tabella pert */

SNTAB *snapshot;                 /* area shm per tabella snapshot */

#if defined BACKTRACK
BKTAB *backtrack;		/* area shm per tabella
				 * backtrack */
#endif


int             id_msg_to_monit;
int             id_msg_from_monit;
int             id_msg_to_banco;
int             id_msg_from_banco;
int             id_msg_to_super;
int             id_msg_from_super;
int             id_msg_to_bm;
int             id_msg_from_bm;
int             id_msg_to_tast;
int             id_msg_from_tast;
int             id_msg_to_sked;
int             id_msg_from_sked;

int             id_sem_disp;

int             shr_usr_key;

int             monit_agganciato = 0;
int             banco_agganciato = 0;
int             super_agganciato = 0;
int             bm_agganciato = 0;
int             tast_agganciato = 0;

int             restart_banco = 0;  /* = 0  prima attivazione del banco 
                                      != 0  ripartenze dopo crash       */

MSG_DISPATCHER  messaggio_dispatcher;

STATISTICA_SKED statistica_sked;


/*
 * Stato dello schedulatore 
 */

static int      stato_sked = STATO_STOP;
static int      stato_sked_old = STATO_STOP;
static float    tempo_sim = 0;
static float    tempo_sim_old = -1;

static int ini_bk = 0;

STATO_CR        stato_cr;



#if defined BACKTRACK
SNAP_SKED       sommari_snapshot;
SNAP_SKED       sommari_backtrack;

int             read_sommari_snapshot();
int             read_sommari_backtrack();
#endif

int  size_ack = (sizeof(MSG_ACK) - sizeof(long));

char pid_processi[10];
int  pid_disp;
int  pid_moni;

int _MAX_SNAP_SHOT;
int _MAX_BACK_TRACK;
int _MAX_CAMPIONI;
int _NUM_VAR;
int _MAX_PERTUR;
int _SPARE_SNAP;
int _PERT_CLEAR;
SIMULATOR *simpar;

int errdisp,errsked,errshm;

extern void testata(char *, char *);
void net_disp_prolog();
int ControlParam(int processo);
int ControlSHM();
int from_someone(int , int , int , int , char *, int );
int net_disp_scoda_sked();
int net_disp_scoda_monit_e_banco();
int net_disp_scoda_super();
int net_disp_scoda_BM();
int net_disp_scoda_tast();
void net_disp_fine();
void net_disp_prolog();
int load_stato_cr(STATO_CR *);
void net_disp_fine();
int net_disp_scoda_monit_e_banco();
static int to_someone(int, int, int, int, char*, int);
static int to_sked(int, char*);
static void snap_tab_read();
static void bktk_tab_read();
void bktk_tab_read();
int read_sommari_snapshot(int);
int AnalisiPar(HEADER_REGISTRAZIONI , int );


int main(int argc, char **argv)
{
int i;
int sem_val;

#if defined NOFUNCTION
/* QUESTE DUE POSSONO DIVENTARE GLOBALI */
MSG_ACK         msg_ack;
#endif

printf("DISPATCHER : parto\n");
   for(i=0;i<argc-1;i++)
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
      }
/*
   printf("dispatcher con = [%d %d %d %d %d %d %d]\n",
           _MAX_SNAP_SHOT , _MAX_BACK_TRACK, 
           _MAX_CAMPIONI  , _NUM_VAR       ,
           _MAX_PERTUR    , _SPARE_SNAP    , _PERT_CLEAR);
*/

   /* redirezione output  e stampa versione */
printf("DISPATCHER : parto2\n");
   testata("dispatcher", SccsID);
printf("DISPATCHER : parto3\n");
   net_disp_prolog();
printf("DISPATCHER : parto4\n");
   errdisp = ControlParam(DISPATCHER);
printf("DISPATCHER : parto5\n");
   errshm  = ControlSHM();
printf("DISPATCHER : parto6\n");
   /*snap_tab_read();*/ /* **** 
                           Si potrebbe SPOSTARE IN FASE DI INITIALIZE     
                           (se fosse necessario)
                           **** */
/* 
   Ricezione da net_sked del numero di errori fra i parametri negli header
   dei file delle registrazioni con i parametri letti dal file Simulator.
   Deve essere fatta qui altrimenti viene scodata da net_disp_scoda_sked.
*/
printf("DISPATCHER : parto7 id_msg_from_sked=%d id_msg_to_sked=%d\n",id_msg_from_sked, id_msg_to_sked);
fflush(stdout);

   from_someone(id_msg_from_sked, id_msg_to_sked, 1, 1, 
                (char*)&errsked, sizeof(int));
printf("DISPATCHER : parto8\n");
fflush(stdout);


   sem_wait_disp(id_sem_disp);
printf("DISPATCHER : parto9\n");
fflush(stdout);

   while (1)
   {
#ifdef MAURIC
int count_sem_wait;
#endif
      if(net_disp_scoda_sked()==1) {
         sem_wait_disp(id_sem_disp);
#ifdef MAURIC
         count_sem_wait++;
         sem_val=semctl(id_sem_disp,0,GETVAL,0);
//         printf("main SCODA_SKED AFTER sem_val=%d\tcount_sem_wait=%d\n",sem_val,count_sem_wait);
#endif
      }
      else {
         if(net_disp_scoda_monit_e_banco()==1) {
            sem_wait_disp(id_sem_disp);
#ifdef MAURIC
            count_sem_wait++;
            sem_val=semctl(id_sem_disp,0,GETVAL,0);
            printf("SCODA_MONIT_E_BANCO AFTER sem_val=%d\tcount_sem_wait=%d\n",sem_val,count_sem_wait);
#endif
         }
#ifdef SCADA
         if(net_disp_scoda_super()==1) {
            sem_wait_disp(id_sem_disp);
#ifdef MAURIC
            count_sem_wait++;
            sem_val=semctl(id_sem_disp,0,GETVAL,0);
            printf("SCODA_SUPERV AFTER sem_val = %d\tcount_sem_wait=%d\n",sem_val,count_sem_wait);
#endif
         }
#endif
#ifdef BANCO_MANOVRA
         if(net_disp_scoda_BM()==1) {
            sem_wait_disp(id_sem_disp);
         }
#endif
         if(net_disp_scoda_tast()==1) {
            sem_wait_disp(id_sem_disp);
         }            
      }
   }  
   net_disp_fine();
}


void net_disp_prolog()
{
int i;

//   printf("Aperura code messaggi processo dispatcher\n");
   shr_usr_key = atoi(getenv("SHR_USR_KEY"));
   id_msg_to_monit = msg_create(shr_usr_key + ID_MSG_TO_MONIT, 1);
   id_msg_from_monit = msg_create(shr_usr_key + ID_MSG_FROM_MONIT, 1);
   id_msg_to_banco = msg_create(shr_usr_key + ID_MSG_TO_BI, 1);
   id_msg_from_banco = msg_create(shr_usr_key + ID_MSG_FROM_BI, 1);
   id_msg_to_super = msg_create(shr_usr_key + ID_MSG_TO_SUPER, 1);
   id_msg_from_super = msg_create(shr_usr_key + ID_MSG_FROM_SUPER, 1);
   id_msg_to_bm = msg_create(shr_usr_key + ID_MSG_TO_BM, 1);
   id_msg_from_bm = msg_create(shr_usr_key + ID_MSG_FROM_BM, 1);
   id_msg_to_tast = msg_create(shr_usr_key + ID_MSG_TO_TAST, 1);
   id_msg_from_tast = msg_create(shr_usr_key + ID_MSG_FROM_TAST, 1);
   id_msg_to_sked = msg_create(shr_usr_key + ID_MSG_TO_SKED, 1);
   id_msg_from_sked = msg_create(shr_usr_key + ID_MSG_FROM_SKED, 1);

//   printf("Apertura semaforo nel processo dispatcher\n");
   id_sem_disp = sem_create(shr_usr_key + ID_SEM_DISP, 0);

   simpar = (SIMULATOR*)calloc(1,sizeof(SIMULATOR));
   simpar->max_snap_shot  = _MAX_SNAP_SHOT;
   simpar->max_back_track = _MAX_BACK_TRACK;
   simpar->max_campioni   = _MAX_CAMPIONI;
   simpar->num_var        = _NUM_VAR;
   simpar->max_pertur     = _MAX_PERTUR;
   simpar->spare_snap     = _SPARE_SNAP;
   simpar->pert_clear     = _PERT_CLEAR;


   /* aggancio al database dei punti */
   errore = RtCreateErrore(RT_ERRORE_TERMINALE,"dispatcher");
#if defined SCO_UNIX
   dbpunti = RtCreateDbPunti(errore,"S02_SCO",DB_PUNTI_INT,simpar);
#endif
#if defined OSF1
   dbpunti = RtCreateDbPunti(errore,"S02_OSF1",DB_PUNTI_INT,simpar);
#endif
#if defined LINUX
   dbpunti = RtCreateDbPunti(errore,"S02_LINUX",DB_PUNTI_INT,simpar);
#endif
#if defined AIX
   dbpunti = RtCreateDbPunti(errore,"S02_AIX",DB_PUNTI_INT,simpar);
#endif
#if defined VMS
   dbpunti = RtCreateDbPunti(errore,"S02_VMS",DB_PUNTI_INT,simpar);
#endif

   /* recupero il puntatore all'area degli snapshot */
   snapshot = RtDbPGetPuntSnapshot(dbpunti);

#if defined BACKTRACK
   /* recupero il puntatore all'area dei backtrack */
   backtrack = RtDbPGetPuntBacktrack(dbpunti);
#endif

   /* recupero la dimensione dell'area dati del simulatore */
   size_area_dati = RtDbPSizeData(dbpunti);

   /*
        recupero i puntatori alle aree delle perturbazioni
   */
   perturbazioni = RtDbPGetPuntPert(dbpunti);
   perturbazioni_att = RtDbPGetPuntPertAtt(dbpunti);

   load_stato_cr(&stato_cr);

}



void net_disp_fine()
{
//   printf("Chiusura code messaggi processo dispatcher\n");
   msg_close(id_msg_to_monit);
   msg_close(id_msg_from_monit);
   msg_close(id_msg_to_banco);
   msg_close(id_msg_from_banco);
   msg_close(id_msg_to_super);
   msg_close(id_msg_from_super);
   msg_close(id_msg_to_bm);
   msg_close(id_msg_from_bm);
   msg_close(id_msg_to_tast);
   msg_close(id_msg_from_tast);
   msg_close(id_msg_to_sked);
   msg_close(id_msg_from_sked);

//   printf("Chiusura semaforo processo dispatcher\n");
   sem_close(id_sem_disp);

   RtDestroyDbPunti(dbpunti);
   free(simpar);
/* printf("\t\t PRIMA DI NET_DISP_FINE\n"); */
   exit(0);
}

int net_disp_scoda_monit_e_banco()
{
   int             comando_ricevuto_monit = 0;
   int             comando_ricevuto_banco = 0;
   int             id_msg_to, id_msg_from;
   int             comando;
   int             i;
   int             n, totale, indirizzo;
   float           valore;
   MSG_ACK         msg_ack;
   float           fattore_step;
   float           velocita;
   float           maxsimtime;
   int             last_snap_reg = 2;
   int             k, righe, messaggi, size_dati;
   char           *app_char;
   char           *app_varupd;
   int             resto;
   int             ret;
   int app_stato;

STATISTICA_SKED nuova_statistica_sked;

   int valsem;

#if defined BANCO_MANOVRA
   MSG_ALLINEAMENTO msg_allineamento;
#endif
   MSG_DISPATCHER msg_disp;
   int size;
   char *err;
   int num_sped;
   int pert_spedite=0;
   TIPO_PERT *pert_in_spedizione;
   comando_ricevuto_monit = msg_rcv(id_msg_from_monit, &messaggio_dispatcher,
                                    sizeof(COMANDO_DISPATCHER), (long)-999, 
                                    IPC_NOWAIT, 0);

   if (comando_ricevuto_monit <= 0)
   {
      comando_ricevuto_banco = msg_rcv(id_msg_from_banco,
				    &messaggio_dispatcher,
	 sizeof(COMANDO_DISPATCHER), (long)-999, IPC_NOWAIT, 0);
      if (comando_ricevuto_banco <= 0)
         {
	 return(-1);
         }
   }

   comando = messaggio_dispatcher.comando_dispatcher.tipo;

   if (comando_ricevuto_monit > 0)
   {
      id_msg_to = id_msg_to_monit;
      id_msg_from = id_msg_from_monit;
   }
   if (comando_ricevuto_banco > 0)
   {
      id_msg_to = id_msg_to_banco;
      id_msg_from = id_msg_from_banco;
   }
printf("in net_disp_scoda_monit_e_banco comando = %d\n", comando);
   switch (comando)
   {
   case COMANDO_AGGANCIO:
      {
/*
      Si ritorna alla to_dispatcher l'ack e successivamente,
      come dato asincrono, i valori degli errori dei parametri
      dei processi net_sked e dispatcher.
*/
      printf("DISPATCHER : ricevuto comando AGGANCIO\n");
      if (comando_ricevuto_monit > 0)
          monit_agganciato = 1;
      else
          banco_agganciato = 1;
/*    Invio ACK */
      msg_ack.ret = 1;
      msg_ack.mtype = 1000;
      msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
/*    
      Invio i valori degli errori alla SD_aggancio. Utilizzo la
      to_someone anche se nella msg_snd usa una size incrementata
      di 5*sizeof(int).
*/
      err = (char*)malloc(3*sizeof(int));
/*       printf("errsked=%d errdisp=%d errshm=%d\n",errsked,errdisp,errshm);*/
      memcpy(&err[0], &errsked,sizeof(int));
      memcpy(&err[sizeof(int)], &errdisp, sizeof(int));
      memcpy(&err[2*sizeof(int)], &errshm, sizeof(int));
      to_someone(id_msg_to, id_msg_from, DATI_DISPATCHER,
                 DATI_DISPATCHER, err, 3*sizeof(int));
      free(err);
      break;
      }
   case COMANDO_SGANCIO:
      {
	 printf("DISPATCHER : ricevuto comando SGANCIO\n");
	 if (comando_ricevuto_monit)
	    monit_agganciato = 0;
	 else
	    banco_agganciato = 0;
	 msg_ack.ret = 1;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_RUN:
      {
	 printf("DISPATCHER : ricevuto comando RUN\n");
	 msg_ack.ret = to_sked(SKDIS_RUN, NULL);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_NEWVARCR:
      {
	 printf("DISPATCHER : ricevuto comando NEWVARCR\n");
	 msg_ack.ret = to_sked(SKDIS_NEWVARCR, NULL);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_VALIDATEBT:
      {
	 printf("DISPATCHER : ricevuto comando VALIDATEBT\n");
	 ini_bk = 1;
	 msg_ack.ret = to_sked(SKDIS_VALIDATEBT, NULL);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_CLEAR:
      {
	 printf("DISPATCHER : ricevuto comando CLEAR\n");
	 msg_ack.ret = to_sked(SKDIS_CLEAR, NULL);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_STOP:
      {
	 printf("DISPATCHER : ricevuto comando STOP\n");
	 msg_ack.ret = to_sked(SKDIS_STOP, NULL);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_FREEZE:
      {
	 printf("DISPATCHER : ricevuto comando FREEZE\n");
	 msg_ack.ret = to_sked(SKDIS_FREEZE, NULL);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_BACKTRACK:
      {
	 printf("DISPATCHER : ricevuto comando BACKTRACK\n");
	 msg_ack.ret = to_sked(SKDIS_BACKTRACK, NULL);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_REPLAY:
      {
	 printf("DISPATCHER : ricevuto comando REPLAY\n");
	 msg_ack.ret = to_sked(SKDIS_REPLAY, NULL);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_STEP:
      {
	 printf("DISPATCHER : ricevuto comando STEP\n");
	 msg_ack.ret = to_sked(SKDIS_STEP, NULL);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_GO_UP:
      {
	 printf("DISPATCHER : ricevuto comando GO UP\n");
	 msg_ack.ret = to_sked(SKDIS_GO_UP, NULL);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_INITIALIZE:
      {
	 printf("DISPATCHER : ricevuto comando INITIALIZE\n");
         if(restart_banco==0) {
/*
   Prima attivazione del 'banco': partenza del simulatore.
*/
            restart_banco++;
            memcpy(&pid_moni,&messaggio_dispatcher.comando_dispatcher.dati[0],
                   sizeof(int));
            pid_disp = getpid();
            memcpy(&pid_processi[0],&pid_disp,sizeof(int));
            memcpy(&pid_processi[4],&pid_moni,sizeof(int));
	    msg_ack.ret = to_sked(SKDIS_INITIALIZE, pid_processi);
	    /* attende la commutazione dello stato */
	    for (i = 0; i < 100; i++)
	    {
	       sleep(1);
	       RtDbPGetStato(dbpunti,&app_stato);
	       if (stato_sked != app_stato);
	          break;
	    }
	    if (i == 500)
	       msg_ack.ret = -1;
	    msg_ack.mtype = 1000;
	    msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
         }
         else {
/*
   Ripartenza del 'banco' dopo crash.
*/
	    msg_ack.ret = 1;
	    msg_ack.mtype = 1000;
	    msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
            if(stato_sked==STATO_FREEZE) {
	       msg_ack.ret = to_sked(SKDIS_FREEZE_REST, NULL);
            }
         }   
	 break;
      }
   case COMANDO_EDITSINGLEIC:
      {
	 /* legge la tabella degli snapshot da file */
	 msg_ack.ret = 1;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 snap_tab_read();
	 memcpy(&k,
		messaggio_dispatcher.comando_dispatcher.dati, sizeof(int));
	 messaggio_dispatcher.comando_dispatcher.tipo =
	    DATI_DISPATCHER;
	 to_someone(id_msg_to, id_msg_from,
		    DATI_DISPATCHER,
	     messaggio_dispatcher.comando_dispatcher.tipo,
		    (char*)&snapshot[k - 1], sizeof(SNTAB));
	 break;
      }
   case COMANDO_EDITIC:
      {
	 /* legge la tabella degli snapshot da file */
	 printf("DISPATCHER : ricevuto comando EDITIC\n");
	 msg_ack.ret = 1;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 snap_tab_read();
	 for (i = 0; i < _MAX_SNAP_SHOT; i ++)
	 {
	    messaggio_dispatcher.comando_dispatcher.tipo =
	       DATI_DISPATCHER;
	    to_someone(id_msg_to, id_msg_from,
		       DATI_DISPATCHER,
                       messaggio_dispatcher.comando_dispatcher.tipo,
		       (char*)&snapshot[i], sizeof(SNTAB) );
	 }
	 break;
      }
   case COMANDO_EDITPERT:
      {
	 /*
	  * chiede allo schedulatore di scodare le
	  * perturbazioni 
	  */
	 if (to_sked(SKDIS_EDITPERT, NULL) < 0)
	    break;
	 /*
	  * legge la tabella delle perturbazioni dalla
	  * shared-memory 
	  */
	 printf("DISPATCHER : ricevuto comando EDITPERT\n");
	 msg_ack.ret = 1;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
/*       
         Spedisce le perturbazioni attive e quelle in attesa a pacchetti di 
         NELEM_PACCHETTO perturbazioni
*/
         if ( _MAX_PERTUR > 0 )
            {
            pert_in_spedizione = (TIPO_PERT*)
                                   calloc(2*_MAX_PERTUR, sizeof(TIPO_PERT));
            memcpy(&pert_in_spedizione[0],perturbazioni,
                   sizeof(TIPO_PERT)*_MAX_PERTUR);
            memcpy(&pert_in_spedizione[_MAX_PERTUR],perturbazioni_att,
                   sizeof(TIPO_PERT)*_MAX_PERTUR);

            messaggio_dispatcher.comando_dispatcher.tipo=DATI_DISPATCHER;
            pert_spedite = 0;
            for (i = 0; i < 2*_MAX_PERTUR; i += NELEM_PACCHETTO)
               {
               if( (pert_spedite+NELEM_PACCHETTO) > 2*_MAX_PERTUR )
                   {
                   num_sped     =  2*_MAX_PERTUR - pert_spedite;
                   pert_spedite =  2*_MAX_PERTUR;
                   }
               else
                   {
                   num_sped     =  NELEM_PACCHETTO;
                   pert_spedite += NELEM_PACCHETTO;
                   }
               to_someone(id_msg_to, id_msg_from, DATI_DISPATCHER,
                          messaggio_dispatcher.comando_dispatcher.tipo,
                          (char*)&pert_in_spedizione[i], 
                          sizeof(TIPO_PERT) * num_sped);
               }
            free(pert_in_spedizione);
            }
	 break;
      }
   case COMANDO_REMOVEPERT:
      {
	 printf("DISPATCHER : ricevuto comando REMOVEPERT\n");
	 msg_ack.ret = to_sked(SKDIS_REMOVEPERT,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
#if defined BANCO_MANOVRA
   case COMANDO_ALLINEA:
      {
	 printf("DISPATCHER : ricevuto comando ALLINEA\n");
	 msg_ack.ret = to_sked(SKDIS_ALLINEA,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
#endif
#if defined BACKTRACK
   case COMANDO_EDITSINGLEBT:
      {
	 /* legge la tabella degli backtrack da file */
	 printf("DISPATCHER : ricevuto comando EDITSINGLEBT\n");
	 msg_ack.ret = 1;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 bktk_tab_read();
	 memcpy(&k,
		messaggio_dispatcher.comando_dispatcher.dati, sizeof(int));
	 messaggio_dispatcher.comando_dispatcher.tipo =
	    DATI_DISPATCHER;
	 to_someone(id_msg_to, id_msg_from,
		    DATI_DISPATCHER,
                    messaggio_dispatcher.comando_dispatcher.tipo,
		    (char*)&backtrack[k - 1], sizeof(BKTAB));
	 break;
      }
   case COMANDO_EDITBT:
      {
	 /* legge la tabella degli snapshot da file */
	 printf("DISPATCHER : ricevuto comando EDITBT \n");
	 msg_ack.ret = 1;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 bktk_tab_read();
	 for (i = 0; i < _MAX_BACK_TRACK; i++)
	 {
	    messaggio_dispatcher.comando_dispatcher.tipo =
	       DATI_DISPATCHER;
	    to_someone(id_msg_to, id_msg_from,
		       DATI_DISPATCHER,
                       messaggio_dispatcher.comando_dispatcher.tipo,
		       (char*)&backtrack[i], sizeof(BKTAB));
	 }
	 break;
      }
   case COMANDO_LOADBT:
      {
	 printf("DISPATCHER : ricevuto comando LOADBT\n");
	 msg_ack.ret = to_sked(SKDIS_LOADBT,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 if (msg_ack.ret < 0)
	    break;
	 /* lettura sommari_backtrack */
	 bktk_tab_read();
	 memcpy(&k, messaggio_dispatcher.comando_dispatcher.dati,
		sizeof(float));
	 if (read_sommari_backtrack(k) < 0)
	 {
	    printf("Errore lettura sommari di backtrack\n");
	    break;
	 }
	 /* invio i sommari_backtrack e malfunzioni remote */
	 app_char = (char *) (&sommari_backtrack);
	 i = 0;
	 for (k = 0; k < (sizeof(SNAP_SKED) / MAX_LUN_COMANDI_DISPATCHER); k++)
	 {
	    messaggio_dispatcher.comando_dispatcher.tipo =
	       DATI_DISPATCHER;
	    to_someone(id_msg_to, id_msg_from,
		       DATI_DISPATCHER,
	     messaggio_dispatcher.comando_dispatcher.tipo,
		&app_char[i], MAX_LUN_COMANDI_DISPATCHER);
	    i += MAX_LUN_COMANDI_DISPATCHER;
	 }
	 k = sizeof(SNAP_SKED) - k * MAX_LUN_COMANDI_DISPATCHER;
	 messaggio_dispatcher.comando_dispatcher.tipo =
	    DATI_DISPATCHER;
	 to_someone(id_msg_to, id_msg_from,
		    DATI_DISPATCHER,
	     messaggio_dispatcher.comando_dispatcher.tipo,
		    &app_char[i], k);
	 app_char[29] = 0;
	 break;
      }
   case COMANDO_SAVEBT:
      {
	 printf("DISPATCHER : ricevuto comando SAVEBT\n");
	 msg_ack.ret = to_sked(SKDIS_SAVEBT,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 /* ricevo i sommari e malfunzioni remote */
	 for (k = 0; k <= (sizeof(SNAP_SKED) / MAX_LUN_COMANDI_DISPATCHER); k++)
	 {
	    msg_ack.mtype = 1000;
	    msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);

            sem_wait_disp(id_sem_disp);

	    msg_rcv(id_msg_from, &messaggio_dispatcher,
		    sizeof(COMANDO_DISPATCHER), (long)-999, !IPC_NOWAIT, 0);
	    msg_ack.ret = to_sked(SKDIS_SAVEBT,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 }
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
#endif
   case COMANDO_TIMESCALING:
      {
	 printf("DISPATCHER : ricevuto comando TIMESCALING\n");
	 memcpy(&velocita,
	     messaggio_dispatcher.comando_dispatcher.dati,
		4);
	 msg_ack.ret = to_sked(SKDIS_TIMESCALING,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_MAXSIMTIME:
      {
	 printf("DISPATCHER : ricevuto comando SKDIS_MAXSIMTIME\n");
	 memcpy(&maxsimtime,
	     messaggio_dispatcher.comando_dispatcher.dati,
		4);
	 msg_ack.ret = to_sked(SKDIS_MAXSIMTIME,
            messaggio_dispatcher.comando_dispatcher.dati);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_STEPSCALING:
      {
	 printf("DISPATCHER : ricevuto comando STEPSCALING\n");
	 memcpy(&fattore_step,
	     messaggio_dispatcher.comando_dispatcher.dati,
		4);
	 msg_ack.ret = to_sked(SKDIS_STEPSCALING,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_STEPCR:
      {
	 printf("DISPATCHER : ricevuto comando STEPCR\n");
	 msg_ack.ret = to_sked(SKDIS_STEPCR,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_LOADIC:
      {
	 printf("DISPATCHER : ricevuto comando LOADIC\n");
	 msg_ack.ret = to_sked(SKDIS_LOADIC,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 if (msg_ack.ret < 0)
	    break;
#if defined MFFR
	 /* lettura sommari_snapshot */
	 snap_tab_read();
	 memcpy(&k, messaggio_dispatcher.comando_dispatcher.dati,
		sizeof(float));
	 if (read_sommari_snapshot(k) < 0)
	 {
	    printf("Errore lettura sommari snapshot\n");
	    break;
	 }
	 /* invio i sommari_snapshot e malfunzioni remote */
	 app_char = (char *) (&sommari_snapshot);
	 i = 0;
	 for (k = 0; k < (sizeof(SNAP_SKED) / MAX_LUN_COMANDI_DISPATCHER); k++)
	 {
	    messaggio_dispatcher.comando_dispatcher.tipo =
	                                DATI_DISPATCHER;
	    to_someone(id_msg_to, id_msg_from,
		       DATI_DISPATCHER,
	               messaggio_dispatcher.comando_dispatcher.tipo,
	               &app_char[i], MAX_LUN_COMANDI_DISPATCHER);
	    i += MAX_LUN_COMANDI_DISPATCHER;
	 }
	 k = sizeof(SNAP_SKED) - k * MAX_LUN_COMANDI_DISPATCHER;
	 messaggio_dispatcher.comando_dispatcher.tipo =
	    DATI_DISPATCHER;
	 to_someone(id_msg_to, id_msg_from,
		    DATI_DISPATCHER,
	     messaggio_dispatcher.comando_dispatcher.tipo,
		    &app_char[i], k);
	 app_char[29] = 0;
#endif
	 break;
      }
   case COMANDO_MODIC:
      {
	 printf("DISPATCHER : ricevuto comando MODIC\n");
	 msg_ack.ret = to_sked(SKDIS_MODIC,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 if (msg_ack.ret < 0)
	 {
	    msg_ack.mtype = 1000;
	    msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	    break;
	 }
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_VARUPD:
      {
	 /*
	  * ricezione e inoltro a skeduler della richiesta 
	  */
	 memcpy(&n,		/* numero delle variabili
				 * richieste */
	     messaggio_dispatcher.comando_dispatcher.dati,
		sizeof(int));
	 /*
	  * printf ("DISPATCHER : ricevuto comando VARUPD
	  * per %d variabili \n",n); 
	  */
	 app_varupd = (char *) malloc
	    (sizeof(int) + n * (sizeof(int) + sizeof(float)));
	 /*********************
	  * la lettura in database viene effettuata da dispatcher invece 
	  * che da sked implementare SKDIS_VARUPD in sked_banco in caso 
	  * che dispatcher debba essere collocato su macchina diversa 
	  * da master
	  ******************************/
	 totale = sizeof(int) + n * (sizeof(int) + sizeof(float));
	 for (k = 0; k < totale / MAX_LUN_COMANDI_DISPATCHER; k++)
	 {
	    memcpy(app_varupd + (k * MAX_LUN_COMANDI_DISPATCHER),
	     messaggio_dispatcher.comando_dispatcher.dati,
		   MAX_LUN_COMANDI_DISPATCHER);
	    msg_ack.ret = 1;
	    msg_ack.mtype = 1000;
	    msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);

            sem_wait_disp(id_sem_disp);

	    msg_rcv(id_msg_from, &messaggio_dispatcher,
		    sizeof(COMANDO_DISPATCHER), (long)-999, !IPC_NOWAIT, 0);
	 }
	 /*
	  * printf ("dispatcher VARUPD: ricevuto %d
	  * messaggi\n",k+1); 
	  */
	 resto = totale - k * MAX_LUN_COMANDI_DISPATCHER;
	 /*
	  * printf ("DISPATCHER: copio %d bytes ad offset
	  * %d\n", resto,k*MAX_LUN_COMANDI_DISPATCHER);  
	  */
	 memcpy(app_varupd + (k * MAX_LUN_COMANDI_DISPATCHER),
		messaggio_dispatcher.comando_dispatcher.dati, resto);

	 msg_ack.ret = 1;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);

	 /*
	  * ricezione della risposta ed inoltro a monit 
	  */

	 for (k = 0; k < n; k++)
	 {
	    memcpy(&indirizzo,
	        app_varupd + sizeof(int) + (k * (sizeof(int) + sizeof(float))),
		   sizeof(int));

            valore = RtDbPGetValueD(dbpunti,indirizzo);

	    memcpy(app_varupd + sizeof(int) +
		   (k * (sizeof(int) + sizeof(float))) +sizeof(int),
		   &valore, sizeof(float));
	 }


	 for (k = 0; k < totale / MAX_LUN_COMANDI_DISPATCHER; k++)
	 {
	    messaggio_dispatcher.comando_dispatcher.tipo =
	       DATI_DISPATCHER;
	    to_someone(id_msg_to, id_msg_from,
		       DATI_DISPATCHER,
	     messaggio_dispatcher.comando_dispatcher.tipo,
	    app_varupd + (k * MAX_LUN_COMANDI_DISPATCHER),
		       MAX_LUN_COMANDI_DISPATCHER);
	 }
	 resto = totale - k * MAX_LUN_COMANDI_DISPATCHER;
	 messaggio_dispatcher.comando_dispatcher.tipo =
	    DATI_DISPATCHER;
	 to_someone(id_msg_to, id_msg_from,
		    DATI_DISPATCHER,
	     messaggio_dispatcher.comando_dispatcher.tipo,
		    app_varupd + (k * MAX_LUN_COMANDI_DISPATCHER), resto);

	 free(app_varupd);
	 /* printf ("VARUPD comando terminato\n");   */
	 break;
      }

   case COMANDO_SAVEIC:
      {
	 printf("DISPATCHER : ricevuto comando SAVEIC\n");
	 msg_ack.ret = to_sked(SKDIS_SAVEIC,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 if (msg_ack.ret < 0)
	 {
	    msg_ack.mtype = 1000;
	    msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	    break;
	 }
#if defined MFFR
	 /* ricevo i sommari e malfunzioni remote */
	 for (k = 0; k <= (sizeof(SNAP_SKED) / MAX_LUN_COMANDI_DISPATCHER); k++)
	 {
	    msg_ack.mtype = 1000;
	    msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);

            sem_wait_disp(id_sem_disp);

	    msg_rcv(id_msg_from, &messaggio_dispatcher,
		    sizeof(COMANDO_DISPATCHER), (long)-999, !IPC_NOWAIT, 0);
	    msg_ack.ret = to_sked(SKDIS_SAVEIC,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 }
#endif
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_REMOVE:
      {
	 printf("DISPATCHER : ricevuto comando REMOVEIC\n");
	 msg_ack.ret = to_sked(SKDIS_REMOVE,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_CHIUSURA:
      {
	 printf("DISPATCHER : ricevuto comando CHIUSURA\n");
	 msg_ack.ret = 1;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 /* system("killsim 2> /dev/null"); */
	 exit(0);
	 break;
      }
   case COMANDO_LSNAPREG:
      {
	 printf("DISPATCHER : ricevuto comando LSNAPREG\n");
	 msg_ack.ret = load_stato_cr(&stato_cr);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 to_someone(id_msg_to, id_msg_from,
		    DATI_DISPATCHER,
		    DATI_DISPATCHER,
		 (char*)&(stato_cr.last_snap_save), sizeof(int));
	 break;
      }
   case COMANDO_LSNAPLOAD:
      {
	 printf("DISPATCHER : ricevuto comando LSNAPLOAD\n");
	 msg_ack.ret = load_stato_cr(&stato_cr);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 to_someone(id_msg_to, id_msg_from,
		    DATI_DISPATCHER,
		    DATI_DISPATCHER,
		 (char*)&(stato_cr.last_snap_load), sizeof(int));
	 break;
      }
#if defined BACKTRACK
   case COMANDO_LBTREG:
      {
	 printf("DISPATCHER : ricevuto comando LBTREG\n");
	 msg_ack.ret = load_stato_cr(&stato_cr);
	 msg_ack.mtype = 1000;
         /*
          * printf("DISPATCHER spedisce:  %d %d   %d %d (ret=%d)\n\n",
          *        stato_cr.last_snap_save,stato_cr.last_snap_load,
          *        stato_cr.last_bktk_save,stato_cr.last_bktk_load,
          *        msg_ack.ret );
          */
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 to_someone(id_msg_to, id_msg_from,
		    DATI_DISPATCHER,
		    DATI_DISPATCHER,
		 (char*)&(stato_cr.last_bktk_save), sizeof(int));
	 break;
      }
   case COMANDO_LBTLOAD:
      {
	 fprintf(stderr,"DISPATCHER : ricevuto comando LBTLOAD\n");
	 msg_ack.ret = load_stato_cr(&stato_cr);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 to_someone(id_msg_to, id_msg_from,
		    DATI_DISPATCHER,
		    DATI_DISPATCHER,
		 (char*)&(stato_cr.last_bktk_load), sizeof(int));
	 break;
      }
#endif
   case COMANDO_STATO:
      {
	 nuova_statistica_sked.stato_sked = stato_sked;
  	 nuova_statistica_sked.tempo_sim = tempo_sim;
	 msg_ack.ret = 1;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 to_someone(id_msg_to, id_msg_from,
		    DATI_ASINCRONI,
		    DATI_DISPATCHER,
	       (char*)&nuova_statistica_sked, sizeof(STATISTICA_SKED));
	 break;
      }
   case COMANDO_LISTACI:
      {
	 printf("DISPATCHER : ricevuto comando LISTACI\n");
	 msg_ack.ret = to_sked(SKDIS_LISTACI,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_EDITDATA:
      {
	 printf("DISPATCHER : ricevuto comando EDITDATA\n");
	 msg_ack.ret = to_sked(SKDIS_EDITDATA,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 /* riceve i dati */

         sem_wait_disp(id_sem_disp);

	 msg_rcv(id_msg_from_sked, &messaggio_dispatcher,
	    sizeof(MSG_DISPATCHER), (long)-999, !IPC_NOWAIT, 0);
	 memcpy(&righe, messaggio_dispatcher.comando_dispatcher.dati,
		sizeof(int));
	 printf("dispatcher: ricevuto messaggio dati r=%d\n", righe);
	 messaggi = (righe * LUN_RIGA_F14) / MAX_LUN_COMANDI_DISPATCHER;
	 app_char = (char *) malloc
	    ((messaggi + 1) * MAX_LUN_COMANDI_DISPATCHER);
	 i = 0;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to_sked, &msg_ack, size_ack, !IPC_NOWAIT);
	 printf("dispatcher: inviato ack a sked coda %d\n", id_msg_to_sked);
	 for (k = 0; k <= messaggi; k++)
	 {
	    printf("dispatcher: messaggio dati %d\n", k);

            sem_wait_disp(id_sem_disp);

	    msg_rcv(id_msg_from_sked, &messaggio_dispatcher,
	    sizeof(MSG_DISPATCHER), (long)-999, !IPC_NOWAIT, 0);
	    memcpy(&app_char[i],
		   &messaggio_dispatcher.comando_dispatcher.dati[0],
		   MAX_LUN_COMANDI_DISPATCHER);
	    i += MAX_LUN_COMANDI_DISPATCHER;
	    printf("dispatcher: ricevuto messaggio dati\n");
	    msg_ack.mtype = 1000;
	    msg_snd(id_msg_to_sked, &msg_ack, size_ack, !IPC_NOWAIT);
	    printf(
		   "dispatcher: inviato ack a sked coda %d\n", id_msg_to_sked);
	 }
	 printf("dispatcher: %s\n", app_char);
	 /* invio i dati a monit/banco  */
	 messaggio_dispatcher.comando_dispatcher.tipo =
	    DATI_DISPATCHER;
	 to_someone(id_msg_to, id_msg_from,
		    DATI_DISPATCHER,
	     messaggio_dispatcher.comando_dispatcher.tipo,
		    (char*)&righe, sizeof(int));
	 i = 0;
	 size_dati = righe * LUN_RIGA_F14;
	 for (k = 0; k < (size_dati / MAX_LUN_COMANDI_DISPATCHER); k++)
	 {
	    messaggio_dispatcher.comando_dispatcher.tipo =
	       DATI_DISPATCHER;
	    to_someone(id_msg_to, id_msg_from,
		       DATI_DISPATCHER,
	     messaggio_dispatcher.comando_dispatcher.tipo,
		(char*)&app_char[i], MAX_LUN_COMANDI_DISPATCHER);
	    i += MAX_LUN_COMANDI_DISPATCHER;
	 }
	 k = size_dati - k * MAX_LUN_COMANDI_DISPATCHER;
	 messaggio_dispatcher.comando_dispatcher.tipo =
	    DATI_DISPATCHER;
	 to_someone(id_msg_to, id_msg_from,
		    DATI_DISPATCHER,
	     messaggio_dispatcher.comando_dispatcher.tipo,
		    (char*)&app_char[i], k);
	 free(app_char);
	 break;
      }
   case COMANDO_MODIDATA:
      {
	 printf("DISPATCHER : ricevuto comando MODIDATA\n");
	 msg_ack.ret = to_sked(SKDIS_MODIDATA,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 memcpy(&righe, messaggio_dispatcher.comando_dispatcher.dati,
		sizeof(int));
	 size_dati = righe * LUN_RIGA_F14;
	 /* ricevo i sommari e malfunzioni remote */
	 for (k = 0; k <= (size_dati / MAX_LUN_COMANDI_DISPATCHER); k++)
	 {
	    msg_ack.mtype = 1000;
	    msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);

            sem_wait_disp(id_sem_disp);

	    msg_rcv(id_msg_from, &messaggio_dispatcher,
		    sizeof(COMANDO_DISPATCHER), (long)-999, !IPC_NOWAIT, 0);
	    msg_ack.ret = to_sked(SKDIS_MODIDATA,
	    messaggio_dispatcher.comando_dispatcher.dati);
	 }
	 printf("dispatcher fine ricevimento dati\n");
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 printf("dispatcher inviato ack\n");
	 break;
      }
   default:
      {
	 msg_ack.ret = -1;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to, &msg_ack, size_ack, !IPC_NOWAIT);
	 printf("Warning : comando non implementato\n");
	 break;
      }
   } /* Fine switch */

   /* fine case */
return(1);
}

int net_disp_scoda_super()
{
   int             comando_ricevuto = 0;
   int             comando;
   int             i;
   MSG_ACK         msg_ack;
   int             cond_ind = 0;

   comando_ricevuto = msg_rcv(id_msg_from_super, &messaggio_dispatcher,
	 sizeof(COMANDO_DISPATCHER), (long)-999, IPC_NOWAIT, 0);

   if (comando_ricevuto <= 0)
      return(-1);

   msg_ack.mtype = 1000;
   msg_snd(id_msg_to_super, &msg_ack, size_ack, !IPC_NOWAIT);
   comando = messaggio_dispatcher.comando_dispatcher.tipo;


   switch (comando)
   {
   case COMANDO_AGGANCIO:
      {
	 super_agganciato = 1;
/*
	 printf("Supervisione agganciata\n");
*/
	 if (stato_sked != STATO_STOP)
	 {
	    printf("Prima statistica \n");
	    to_someone(id_msg_to_super, id_msg_from_super,
		       DATI_ASINCRONI, START_SCADA, 0, 0);
	    to_someone(id_msg_to_super, id_msg_from_super,
		       DATI_ASINCRONI, LOADCI,
		       (char*)&cond_ind, sizeof(int));
	    to_someone(id_msg_to_super, id_msg_from_super,
		       DATI_ASINCRONI, SIM_TIME_EXTRA,
		       (char*)&tempo_sim, sizeof(float));
	    to_someone(id_msg_to_super, id_msg_from_super,
		       DATI_ASINCRONI, DB_ALL, 0, 0);
	    to_someone(id_msg_to_super, id_msg_from_super,
		       DATI_ASINCRONI, INITDB, 0, 0);

	 }
	 switch (stato_sked)
	 {
	 case STATO_RUN:
	    {
	       to_someone(id_msg_to_super, id_msg_from_super,
			  DATI_ASINCRONI, RUN_SCADA, 0, 0);
	       break;
	    }
	 case STATO_FREEZE:
	    {
	       to_someone(id_msg_to_super, id_msg_from_super,
			  DATI_ASINCRONI, DB_ANA, 0, 0);
	       to_someone(id_msg_to_super, id_msg_from_super,
		      DATI_ASINCRONI, FREEZE_SCADA, 0, 0);
	       break;
	    }
	 }
	 break;
      }
   case COMANDO_SGANCIO:
      {
	 super_agganciato = 0;
	 printf("Supervisione scollegata\n");
	 break;
      }
   }
return(1);
}

int net_disp_scoda_BM()
{
#if defined BANCO_MANOVRA
   int             comando;
   int             comando_ricevuto;
   int             i;
   MSG_ACK         msg_ack;
   int             k;
   char           *app_char;

   comando_ricevuto = msg_rcv(id_msg_from_bm, &messaggio_dispatcher,
	 sizeof(COMANDO_DISPATCHER), (long)-999, IPC_NOWAIT, 0);

   if (comando_ricevuto < 0)
      return(-1);

   comando = messaggio_dispatcher.comando_dispatcher.tipo;


   printf("DISPATCHER : ricevuto da BM comando %d\n", comando);
   switch (comando)
   {
   case COMANDO_AGGANCIO:
      {
	 printf("DISPATCHER : ricevuto comando AGGANCIO\n");
	 bm_agganciato = 1;
	 msg_ack.ret = 1;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to_bm, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case COMANDO_SGANCIO:
      {
	 printf("DISPATCHER : ricevuto comando SGANCIO\n");
	 bm_agganciato = 1;
	 msg_ack.ret = 1;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to_bm, &msg_ack, size_ack, !IPC_NOWAIT);
	 break;
      }
   case DATI_DISPATCHER:
      {
	 printf("dispatcher: ricevuti dati allineamento\n");
	 msg_ack.ret = 1;
	 msg_ack.mtype = 1000;
	 msg_snd(id_msg_to_bm, &msg_ack, size_ack, !IPC_NOWAIT);
	 messaggio_dispatcher.comando_dispatcher.tipo = DATI_DISPATCHER;
	 /* to_someone (id_msg_to_banco, id_msg_from_banco,  */
	 to_someone(id_msg_to_monit, id_msg_from_monit,
		    DATI_DISPATCHER,
	     messaggio_dispatcher.comando_dispatcher.tipo,
	     messaggio_dispatcher.comando_dispatcher.dati,
	    messaggio_dispatcher.comando_dispatcher.size);
	 break;
      }
   }

#endif

return(1);
}


int net_disp_scoda_sked()
{
   int             comando_ricevuto = 0;
   int             comando;
   int             operazione;
   int             num_operazione = 0;
   FILE           *fp;

   int             num_msg_to_monit;
#if defined UNIX
   struct msqid_ds buf;
#endif 


   comando_ricevuto = msg_rcv(id_msg_from_sked, &messaggio_dispatcher,
	     sizeof(MSG_DISPATCHER), (long)-999, IPC_NOWAIT, 0);
   if (comando_ricevuto <= 0)
      return (-1);
   comando = messaggio_dispatcher.comando_dispatcher.tipo;

   if (messaggio_dispatcher.comando_dispatcher.size == sizeof(STATISTICA_SKED))
   {
      memcpy(&statistica_sked, messaggio_dispatcher.comando_dispatcher.dati,
	     messaggio_dispatcher.comando_dispatcher.size);
      if((comando == SKDIS_STATO_INT)&&(stato_sked!=STATO_RUN))
		return (1);
      tempo_sim = (float) statistica_sked.tempo_sim;
      stato_sked = statistica_sked.stato_sked;
      operazione = statistica_sked.operazione_eseguita;
      num_operazione = statistica_sked.num_operazione;
      /*
       * printf ("Stato_sked = %d \n tempo_sim= %f eseguito
       * = %d/%d\n", stato_sked, tempo_sim,
       * statistica_sked.operazione_eseguita,
       * statistica_sked.num_operazione); 
       */
   }
   switch (comando)
   {
   case SKDIS_STATO_INT:
      {
	 if (super_agganciato == 1)
	 {
	    to_someone(id_msg_to_super, id_msg_from_super, DATI_ASINCRONI,
		 SIM_TIME_INT, (char*)&tempo_sim, sizeof(float));
	 }
	 break;
      }
   case SKDIS_STATO:
      {
/*
printf("net_disp_scoda_sked: SKDIS_STATO!!!\n");
*/
	 if (tast_agganciato == 1)
	 {
	    to_someone(id_msg_to_tast, id_msg_from_tast, DATI_ASINCRONI,
	     messaggio_dispatcher.comando_dispatcher.tipo,
	     messaggio_dispatcher.comando_dispatcher.dati,
	    messaggio_dispatcher.comando_dispatcher.size);
	 }
	 if (monit_agganciato == 1)
	 {
/*
   Recupero del numero di messaggi presenti sulla coda id_msg_to_monit.
*/
   msgctl(id_msg_to_monit, IPC_STAT, &buf);
   num_msg_to_monit = buf.msg_qnum;
/*
   printf("net_disp_scoda_sked: id_msg_to_monit    num_msg = %d\t tipo_msg = %d\n", num_msg_to_monit,comando);
*/

/*
   Si controlla il numero di messaggi presenti sulla coda id_msg_to_monit.
   Se il numero di messaggi presenti e' 5 non ne vengono accodati altri
   di tipo SKDIS_STATO. Questo limite e' necessario nel caso di caduta
   e ripartenza del banco: non si deve riempire la coda.
*/
            if(num_msg_to_monit<=5) {
	       to_someone(id_msg_to_monit, id_msg_from_monit, DATI_ASINCRONI,
	                  messaggio_dispatcher.comando_dispatcher.tipo,
	                  messaggio_dispatcher.comando_dispatcher.dati,
	                  messaggio_dispatcher.comando_dispatcher.size);
            }
	 }
	 if (bm_agganciato == 1)
	 {
	    to_someone(id_msg_to_bm, id_msg_from_bm,
		       DATI_ASINCRONI, stato_sked,
		       (char*)&operazione, sizeof(int));
	 }
	 if (super_agganciato == 1)
	 {
/*
printf("SKDIS_STATO: super_agganciato\n");
*/
	    if (stato_sked != stato_sked_old)
	    {
/*
printf("net_disp_scoda_sked: stato_sked != stato_sked_old\n");
*/
	       switch (stato_sked)
	       {
	       case STATO_STOP:
		  {
		     to_someone(id_msg_to_super, id_msg_from_super,
				DATI_ASINCRONI, ALT, 0, 0);
		     to_someone(id_msg_to_super, id_msg_from_super,
			DATI_ASINCRONI, STOP_SCADA, 0, 0);
		     break;
		  }
	       case STATO_RUN:
		  {
		     if (stato_sked_old != STATO_FREEZE)
		     {
			to_someone(id_msg_to_super, id_msg_from_super,
			    DATI_ASINCRONI, DB_ANA, 0, 0);
			to_someone(id_msg_to_super, id_msg_from_super,
				   DATI_ASINCRONI, FREEZE_SCADA, 0, 0);
		     }
		     to_someone(id_msg_to_super, id_msg_from_super,
			 DATI_ASINCRONI, RUN_SCADA, 0, 0);
		     break;
		  }
	       case STATO_FREEZE:
		  {
/*
printf("net_disp_scoda_sked: da sked STATO_FREEZE!!!\n");
*/
		     /* prima statistica */
		     if (stato_sked_old == STATO_STOP)
		     {
			printf("Prima statistica \n");
/**
			to_someone(id_msg_to_super, id_msg_from_super,
			DATI_ASINCRONI, START_SCADA, 0, 0);
**/
			to_someone(id_msg_to_super, id_msg_from_super,
				   DATI_ASINCRONI, LOADCI,
			    (char*)&num_operazione, sizeof(int));
			to_someone(id_msg_to_super, id_msg_from_super,
			   DATI_ASINCRONI, SIM_TIME_EXTRA,
			       (char*)&tempo_sim, sizeof(float));
			to_someone(id_msg_to_super, id_msg_from_super,
			    DATI_ASINCRONI, DB_ALL, 0, 0);
			to_someone(id_msg_to_super, id_msg_from_super,
			    DATI_ASINCRONI, INITDB, 0, 0);
		     }
		     if (stato_sked_old == STATO_BACKTRACK)
		     {
			if (ini_bk == 1)
			{
			   to_someone(id_msg_to_super, id_msg_from_super,
			    DATI_ASINCRONI, INITDB, 0, 0);
/***
			   to_someone(id_msg_to_super, id_msg_from_super,
				      DATI_ASINCRONI, RUN_SCADA, 0, 0);
**/
			   to_someone(id_msg_to_super, id_msg_from_super,
			    DATI_ASINCRONI, DB_ANA, 0, 0);
			   to_someone(id_msg_to_super, id_msg_from_super,
				      DATI_ASINCRONI, FREEZE_SCADA, 0, 0);
			} else
			{
			   to_someone(id_msg_to_super, id_msg_from_super,
			   DATI_ASINCRONI, BKTKOFF, 0, 0);
			   to_someone(id_msg_to_super, id_msg_from_super,
			   DATI_ASINCRONI, DB_ANA, 0, 0);
			}
			ini_bk = 0;
		     } else
		     {
			if (stato_sked_old != STATO_BACKTRACK)
			   to_someone(id_msg_to_super, id_msg_from_super,
			    DATI_ASINCRONI, DB_ANA, 0, 0);
			to_someone(id_msg_to_super, id_msg_from_super,
				   DATI_ASINCRONI, FREEZE_SCADA, 0, 0);
		     }
		     break;
		  }
	       case STATO_BACKTRACK:
		  {
		     if (stato_sked_old == STATO_RUN)
		     {
			to_someone(id_msg_to_super, id_msg_from_super,
			    DATI_ASINCRONI, DB_ANA, 0, 0);
			to_someone(id_msg_to_super, id_msg_from_super,
				   DATI_ASINCRONI, FREEZE_SCADA, 0, 0);
		     }
		     if (stato_sked_old == STATO_REPLAY)
			to_someone(id_msg_to_super, id_msg_from_super,
			    DATI_ASINCRONI, REPOFF, 0, 0);
		     else
		        to_someone(id_msg_to_super, id_msg_from_super,
			    DATI_ASINCRONI, BKTKON, 0, 0);
		     break;
		  }
	       case STATO_REPLAY:
		  {
		     to_someone(id_msg_to_super, id_msg_from_super,
			     DATI_ASINCRONI, REPON, 0, 0);
		     break;
		  }
	       case STATO_ERRORE:
		  {
		     to_someone(id_msg_to_super, id_msg_from_super,
				DATI_ASINCRONI, ALT, 0, 0);
		     to_someone(id_msg_to_super, id_msg_from_super,
			DATI_ASINCRONI, STOP_SCADA, 0, 0);
		     break;
		  }
	       }
	    } else
	       if ((operazione == ESEGUITO_STEP) ||
		   (operazione == ESEGUITO_LOADIC))
	    {
	       switch (operazione)
	       {
	       case ESEGUITO_STEP:
		  {
		     to_someone(id_msg_to_super, id_msg_from_super,
			    DATI_ASINCRONI, DB_ANA, 0, 0);
		     to_someone(id_msg_to_super, id_msg_from_super,
			   DATI_ASINCRONI, SIM_TIME_EXTRA,
				(char*)&tempo_sim, sizeof(float));
		     to_someone(id_msg_to_super, id_msg_from_super,
			      DATI_ASINCRONI, STEP, 0, 0);
		     break;
		  }
	       case ESEGUITO_LOADIC:
		  {  // inizio
                     if (stato_sked == STATO_RUN)
                     {
                        to_someone(id_msg_to_super, id_msg_from_super,
                            DATI_ASINCRONI, DB_ANA, 0, 0);
                        to_someone(id_msg_to_super, id_msg_from_super,
                                   DATI_ASINCRONI, FREEZE_SCADA, 0, 0);
                     }
                     if (stato_sked == STATO_BACKTRACK)
                        to_someone(id_msg_to_super, id_msg_from_super,
                                   DATI_ASINCRONI, LOADBT,
                            (char*)&num_operazione, sizeof(int));
                     else
                        {
                        if (stato_sked != STATO_REPLAY)
                          to_someone(id_msg_to_super, id_msg_from_super,
                                   DATI_ASINCRONI, LOADCI,
                            (char*)&num_operazione, sizeof(int));
printf("Inviata LOADCI\n");
                        }
                     to_someone(id_msg_to_super, id_msg_from_super,
                           DATI_ASINCRONI, SIM_TIME_EXTRA,
                                (char*)&tempo_sim, sizeof(float));
                     to_someone(id_msg_to_super, id_msg_from_super,
                            DATI_ASINCRONI, DB_ALL, 0, 0);
                     if (stato_sked == STATO_BACKTRACK)
                     {
                        break;
                     }
                     if (stato_sked == STATO_REPLAY)
                        break;
                     to_someone(id_msg_to_super, id_msg_from_super,
                            DATI_ASINCRONI, INITDB, 0, 0);
/*
                     to_someone(id_msg_to_super, id_msg_from_super,
                         DATI_ASINCRONI, RUN_SCADA, 0, 0);
*/
                     if (stato_sked == STATO_FREEZE)
                     {
/*
                        to_someone(id_msg_to_super, id_msg_from_super,
                            DATI_ASINCRONI, DB_ANA, 0, 0);
*/
                        to_someone(id_msg_to_super, id_msg_from_super,
                                   DATI_ASINCRONI, FREEZE_SCADA, 0, 0);
                     }
                     break;
		  }  // FINE LOADCI
	       }
	    } else
	    if (tempo_sim != tempo_sim_old)
	    {
	       to_someone(id_msg_to_super, id_msg_from_super, DATI_ASINCRONI,
		     SIM_TIME, (char*)&tempo_sim, sizeof(float));
	    }
	 }
	 break;
      }
   }
   tempo_sim_old = tempo_sim;
   stato_sked_old = stato_sked;
   if(stato_sked == STATO_STOP)
      {
      sleep(1);
      net_disp_fine();
      }
   return (1);
}

int net_disp_scoda_tast()
{
   int             comando_ricevuto = 0;
   int             comando;
   int             i;
   MSG_ACK         msg_ack;

   if ((comando_ricevuto = msg_rcv(id_msg_from_tast, &messaggio_dispatcher,
    sizeof(COMANDO_DISPATCHER), (long)-999, IPC_NOWAIT, 0)) > 0)
      printf("\n\n\nRicevuto messaggio da tastiera comando = %d\n",
	     messaggio_dispatcher.comando_dispatcher.tipo);

   if (comando_ricevuto <= 0)
      return(-1);

   comando = messaggio_dispatcher.comando_dispatcher.tipo;

   msg_ack.ret = 1;
   msg_ack.mtype = 1000;
   msg_snd(id_msg_to_tast, &msg_ack, size_ack, !IPC_NOWAIT);


   switch (comando)
   {
   case COMANDO_AGGANCIO:
      {
	 tast_agganciato = 1;
	 break;
      }
   case COMANDO_SGANCIO:
      {
	 tast_agganciato = 0;
	 break;
      }
   case COMANDO_RUN:
      {
	 to_sked(SKDIS_RUN, NULL);
	 break;
      }
   case COMANDO_STOP:
      {
	 to_sked(SKDIS_STOP, NULL);
	 break;
      }
   case COMANDO_FREEZE:
      {
	 printf("DISPATCHER : ricevuto comando FREEZE1\n");
	 to_sked(SKDIS_FREEZE, NULL);
	 break;
      }
   case COMANDO_MAXSIMTIME:
      {
printf("in dispatcher: max sim time = |%f| \n", messaggio_dispatcher.comando_dispatcher.dati[0]);
	 to_sked(SKDIS_MAXSIMTIME, NULL);
	 break;
      }
   case COMANDO_INITIALIZE:
      {
	 to_sked(SKDIS_INITIALIZE, NULL);
	 break;
      }
   case COMANDO_EDITIC:
      {
	 to_sked(SKDIS_EDITIC, NULL);
	 for (i = 0; i < _MAX_SNAP_SHOT; i++)
	 {
	    messaggio_dispatcher.comando_dispatcher.tipo =
	       DATI_DISPATCHER;
	    to_someone(id_msg_to_tast, id_msg_from_tast,
		       DATI_DISPATCHER,
	     messaggio_dispatcher.comando_dispatcher.tipo,
		       (char*)&snapshot[i], sizeof(SNTAB));
	 }
	 break;
      }
   case COMANDO_CHIUSURA:
      {
	 break;
      }
   default:
      {
	 printf("Warning : comando non implementato\n");
	 break;
      }
   }
   /* fine case */
return(1);
}


int to_sked(comando, dati)
   int             comando;
   char           *dati;
{
   MSG_DISPATCHER  messaggio;
   MSG_ACK         msg_ack;

   int             valsem;

   if (dati != NULL)
      memcpy(&(messaggio.comando_dispatcher.dati[0]),
	     dati, MAX_LUN_COMANDI_DISPATCHER);
   messaggio.mtype = comando;

#if defined MAURIC
   valsem = semctl(id_sem_disp,0,GETVAL,0);
   printf("dispatcher_to_sked: valsem = %d\n",valsem);
#endif

    /* printf("            to_sked spedisce con size=%d\n",
              sizeof(COMANDO_DISPATCHER)); */
   if (msg_snd(id_msg_to_sked, &messaggio, sizeof(COMANDO_DISPATCHER),
	       !IPC_NOWAIT) < 0)
   {
      perror("messaggio");
      return (-1);
   }

   sem_wait_disp(id_sem_disp);
   if (msg_rcv(id_msg_from_sked, &msg_ack, size_ack, (long)1000,
	       !IPC_NOWAIT, TIME_ACK) < 0)
      return (-1);

#if defined MAURIC
   valsem = semctl(id_sem_disp,0,GETVAL,0);
   printf("dispatcher_to_sked: uscita!! valsem = %d\n",valsem);
#endif

   if (msg_ack.ret == NO_ALLINEA )
      return(NO_ALLINEA);

   if (msg_ack.ret == NUOVA_CI )
      {
      return(NUOVA_CI);
      }

   if (msg_ack.ret < 0)
      return (-1);
   else
      return (1);
}


/*
   Questa funzione inserisce nella parte dati la size passata come parametro
   ma nel campo size del messaggio aggiunge anche 5 interi.
*/
int to_someone(id, id_ret, tipo_dati, comando, dati, size)
   int             id;
   int             id_ret;
   int             tipo_dati;
   int             comando;
   char           *dati;
   int             size;
{
   MSG_DISPATCHER  messaggio;
   MSG_ACK         msg_ack;

   messaggio.comando_dispatcher.size = size;
   messaggio.comando_dispatcher.tipo = comando;
   if (size > 0)
      {
      /*printf("to_someone fa memcpy con size=%d\n",size);*/
      memcpy(&(messaggio.comando_dispatcher.dati[0]), dati, size);
      }
   messaggio.mtype = tipo_dati;
   /*printf("dispatcher-to_someone spedisce size=%d e 5 int\n",size);*/
   if (msg_snd(id, &messaggio, size + 5 * sizeof(int)
	       /* sizeof (COMANDO_DISPATCHER) */ , !IPC_NOWAIT) == (-1))
      perror("messaggio");
   if ((id_ret != (-1)) && (tipo_dati != DATI_ASINCRONI)) {
      sem_wait_disp(id_sem_disp);
      msg_rcv(id_ret, &msg_ack, sizeof(MSG_ACK) - sizeof(long),
	      (long)1000, !IPC_NOWAIT, TIME_ACK);
   }
}

/*
 * legge da file la tabella degli snapshot e la carica in
 * shared memory 
 */

void snap_tab_read()
{
static int      prima = 1;
int             i, ret, offset;
FILE           *fp_snap;
HEADER_REGISTRAZIONI hreg_snap;

   if (prima)
   {
      printf("Dispatcher legge tabella snapshot\n");
      /* caricamento shared memory sezione snapshot */
      if ((fp_snap = fopen("snapshot.dat", "r")))
      {
/*       Legge l'header */
         fread(&hreg_snap,sizeof(HEADER_REGISTRAZIONI),1,fp_snap);
/*       Vengono lette le tabella degli snap  */
	 for (i = 1; i <= _MAX_SNAP_SHOT; i++)
	 {
	    fread(&snapshot[i - 1], sizeof(SNTAB), 1, fp_snap);
            /*
             * printf ("snap_shared prog=%d stat=%d pos=%d mod=%d %s\n", 
             *          snapshot[i - 1].prog, snapshot[i - 1].stat, 
             *          snapshot[i - 1].pos,  snapshot[i - 1].mod, 
             *          snapshot[i - 1].descr); 
             */
	 }
	 fclose(fp_snap);
      }
/*
 * patch per copy/paste banco 8jan96 fm, la lista viene aggiornata
 * ogni volta !!!
      prima = 0;
**********/
   }
}


#if defined BACKTRACK

/*
 * legge da file la tabella dei backtrack e la carica in
 * shared memory 
 * COSI' COME E' NON FA NULLA !!!!
 */
void bktk_tab_read()
{
   static int      prima = 1;
   int             i, ret, offset;
   FILE           *fp_bktk;

#if defined QQQQQQQQQ
   if (prima)
   {
      printf("\t Dispatcher legge tabella backtrack\n");
      /* caricamento shared memory sezione backtrack */
      if ((fp_bktk = fopen("backtrack.dat", "r")))
      {
	 /* il file esiste */

         offset = sizeof(HEADER_REGISTRAZIONI);
         fseek(fp_bktk, offset, 0);

	 for (i = 1; i <= _MAX_BACK_TRACK; i++)
	 {
	    fread(&(*bkt)[i - 1], sizeof(BKTAB), 1, fp_bktk);
	    /*
	     * printf ("snap_shared prog=%d stat=%d pos=%d
	     * mod=%d %s\n", backtrack[i - 1].prog, backtrack[i -
	     * 1].stat, backtrack[i - 1].pos, backtrack[i -
	     * 1].mod, backtrack[i- 1].descr); 
	     */
	 }
	 fclose(fp_bktk);
      }
      prima = 0;
   }
#endif
}


int read_sommari_snapshot(posizione)
   int             posizione;
{
   int             offset;
   FILE           *fp_snap;
   char            app_char[30];

   if ((fp_snap = fopen("snapshot.dat", "r")))
   {
      /* calcolo posizione */
#if defined SNAP_PIAC
      offset = sizeof(HEADER_REGISTRAZIONI) + _MAX_SNAP_SHOT * sizeof(SNTAB) +
               (posizione - 1) * (size_area_dati      +  sizeof(float) +
                                  sizeof(SNAP_SKED)   + 
                                  DIM_SNAP_AUS        +
                                  _SPARE_SNAP*AREA_SPARE*sizeof(char)  +
                                  2*_MAX_PERTUR * sizeof(TIPO_PERT)        ) +
               size_area_dati + sizeof(float) + DIM_SNAP_AUS;
#else
      offset = _MAX_SNAP_SHOT * sizeof(SNTAB)                                +
               (snapshot[posizione - 1].pos - 1) * 
               (size_area_dati + sizeof(SNAP_SKED) + DIM_SNAP_AUS)           +
               size_area_dati + DIM_SNAP_AUS;
#endif
      fseek(fp_snap, offset, 0);
      fread(&sommari_snapshot, sizeof(sommari_snapshot), 1, fp_snap);
      fclose(fp_snap);
      printf("DISPATCHER:read_sommari_snapshot  val=%d  fr=%d  mf=%d\n",
             sommari_snapshot.somm_snappati,
             sommari_snapshot.readfr.ul_fr, 
             sommari_snapshot.readmf.ul_mf);
      memcpy(&app_char[0], &sommari_snapshot, 30);
      app_char[29] = 0;
      return (1);
   } else
      return (-1);
}


int read_sommari_backtrack(posizione)
   int             posizione;
{
   int             offset;
   FILE           *fp_bktk;
   char            app_char[30];

   if ((fp_bktk = fopen("backtrack.dat", "r")))
   {
      /* calcolo posizione */
#if defined SNAP_PIAC
      offset = sizeof(HEADER_REGISTRAZIONI) + _MAX_BACK_TRACK * sizeof(BKTAB)  +
               (posizione - 1) * (size_area_dati    + sizeof(float) +
                                  sizeof(SNAP_SKED) + DIM_SNAP_AUS  +
                                  _SPARE_SNAP*AREA_SPARE*sizeof(char) +
                                  2*_MAX_PERTUR * sizeof(TIPO_PERT) )          +
               size_area_dati + sizeof(float) + DIM_SNAP_AUS;
#else
      offset = _MAX_BACK_TRACK * sizeof(BKTAB)             +
               (backtrack[posizione - 1].pos - 1) *
                 ( size_area_dati + sizeof(float)    +
                   sizeof(SNAP_SKED) + DIM_SNAP_AUS  +
                   MAX_UPDOWN * (sizeof(float)       + 
                   sizeof(int)) )                         +
               size_area_dati + sizeof(float) + DIM_SNAP_AUS;
#endif
      fseek(fp_bktk, offset, 0);
      fread(&sommari_backtrack, sizeof(sommari_backtrack), 1, fp_bktk);
      fclose(fp_bktk);
      printf("DISPATCHER:read_sommari_backtrack  val=%d  fr=%d  mf=%d\n",
              sommari_backtrack.somm_snappati,
              sommari_backtrack.readfr.ul_fr, 
              sommari_backtrack.readmf.ul_mf);
      memcpy(&app_char[0], &sommari_backtrack, 30);
      app_char[29] = 0;
      return (1);
   } else
      return (-1);
}
#endif



int from_someone(int id, int id_ret, int tipo_dati, int comando, 
                 char *dati, int size)
{
MSG_DISPATCHER  messaggio;
MSG_ACK         msg_ack;
int ret;

/* Attesa messaggi */
printf("from_someone: 0 GUAG id=%d id=%d_ret size=%d\n",id,id_ret,size);
printf("from_someone: 1 GUAG id_sem_disp=%d\n",id_sem_disp);
fflush(stdout);
   sem_wait_disp(id_sem_disp);
printf("from_someone: 2 GUAG  to sked id_ret=%d id=%d sizeof(MSG_DISPATCHER)=%d\n",id_ret,id,sizeof(MSG_DISPATCHER));
fflush(stdout);

   msg_rcv(id, &messaggio, sizeof(MSG_DISPATCHER), (long)-999, !IPC_NOWAIT, 0);
//   msg_rcv(id, &messaggio, sizeof(MSG_DISPATCHER) , -999, MSG_NOERROR, 0);
//   msg_rcv(id, &messaggio, sizeof(MSG_DISPATCHER) , -999, 0, 0);
printf("\t from_someone: 3 GUAG riceve size=%d su coda=%d\n",size,id);
   if( size>0 )
      memcpy(dati, messaggio.comando_dispatcher.dati, size);
   if( id_ret == (-1) )
       return(0);
/* Spedisco acknowledge */
   msg_ack.ret   = 1;
   msg_ack.mtype = 1000;
   ret = msg_snd(id_ret, &msg_ack, size_ack, !IPC_NOWAIT);
   if ( ret== (-1) )
      {
      perror("from_someone");
      }
   /*printf("\t from_someoneD: inviato ack a sked coda %d\n", id_ret);*/
   return(0);
}


/*
    Controllo delle shared memory.
    Allo startup la DB_PUNTI_INT e' creata leggendo il file S02_$SO
    per cui non contiene i parametri del file Simulatori mentre  la
    DB_PUNTI_SHARED che non e' ancora stata creata, se esiste gia', 
    cioe' sh ha successo l'aggancio, li contiene e quindi vengono 
    confrontati. 
                                       ERRORE:
                        DB_PUNTI_INT           DB_PUNTI_SHARED
    Parametri Nulli          0                       1    
    Parametri diversi        -(1)                    1

*/
int ControlSHM()
{
HEADER_REGISTRAZIONI hdreg;
RtDbPuntiOggetto dbpunti_ext;
int err=0;

/*    Aggancio */
      dbpunti_ext = RtCreateDbPunti(errore,"TEST",DB_PUNTI_SHARED,simpar);
      if( (dbpunti==NULL)&&(dbpunti_ext==NULL) )
         {
         printf(" dbpunti e dbpunti_ext nulle\n");
         err=2;
         }
      else
         {
         if( (dbpunti_ext!=NULL) )
             {
             RtDbPGetHeaderReg(dbpunti_ext,&hdreg);
             err += AnalisiPar(hdreg,DB_PUNTI_SHARED);
/*           printf("\t\t AnalisiPar con dbpunti_ext!=NULL ritorna err=%d\n",
                     err); */
             RtDestroyDbPunti(dbpunti_ext);
             }
         if( (dbpunti!=NULL) )
             {
             RtDbPGetHeaderReg(dbpunti,&hdreg);
             err += AnalisiPar(hdreg,DB_PUNTI_INT);
             }
         }
      if( err>0 )
         printf("Confrontate le shared memory ritorno err [%d]\n",err);
      return(err);
}


int AnalisiPar(HEADER_REGISTRAZIONI hdreg, int tipo)
{
int i,ret;
int zero=0;
int scrivi_file=0;
FILE *fpedf;
char messaggio[FILENAME_MAX]="";
char tipolog[2][20]={"DB_PUNTI_INT",
                   "DB_PUNTI_SHARED"};

    ret=0;
    scrivi_file=0;
    if ( 
        ( hdreg.simulator.max_snap_shot  == zero ) &&
        ( hdreg.simulator.max_back_track == zero ) &&
        ( hdreg.simulator.max_campioni   == zero ) &&
        ( hdreg.simulator.num_var        == zero ) &&
        ( hdreg.simulator.max_pertur     == zero ) &&
        ( hdreg.simulator.spare_snap     == zero ) &&
        ( hdreg.simulator.pert_clear     == zero )
       )
       {
       if( tipo==DB_PUNTI_SHARED )
          {
          scrivi_file = 1;
          sprintf(messaggio,"Errore in shm %s : Parametri nulli.",
                  tipolog[tipo]);
          ret = 1;
          }
       else
          {
          ret = 0;
          }
       }
    else
       if ( 
           ( hdreg.simulator.max_snap_shot  != _MAX_SNAP_SHOT  ) ||
           ( hdreg.simulator.max_back_track != _MAX_BACK_TRACK ) ||
           ( hdreg.simulator.max_campioni   != _MAX_CAMPIONI   ) ||
           ( hdreg.simulator.num_var        != _NUM_VAR        ) ||
           ( hdreg.simulator.max_pertur     != _MAX_PERTUR     ) ||
           ( hdreg.simulator.spare_snap     != _SPARE_SNAP     ) ||
           ( hdreg.simulator.pert_clear     != _PERT_CLEAR     )
          )
          {
          scrivi_file = 1;
          sprintf(messaggio,"Errore in shm %s : parametri non congruenti.",
                 tipolog[tipo]);
          ret = 1;
          }
       else
          {
          scrivi_file = 0;
          ret = 0;
          }

      if( scrivi_file == 1)
           {
           fpedf = fopen(PAREDF,"a");
           fprintf(fpedf,"\t %s\n",messaggio);
           fprintf(fpedf,"%-20s:  SHM=[%5d]    file=[%d]\n",
                 ARGV_N_SNAP,hdreg.simulator.max_snap_shot,_MAX_SNAP_SHOT);
           fprintf(fpedf,"%-20s:  SHM=[%5d]    file=[%d]\n",
                 ARGV_N_BKTK, hdreg.simulator.max_back_track,_MAX_BACK_TRACK);
           fprintf(fpedf,"%-20s:  SHM=[%5d]    file=[%d]\n",
                 ARGV_N_CAMP_CR, hdreg.simulator.max_campioni,_MAX_CAMPIONI);
           fprintf(fpedf,"%-20s:  SHM=[%5d]    file=[%d]\n",
                 ARGV_N_VAR_CR, hdreg.simulator.num_var,_NUM_VAR);
           fprintf(fpedf,"%-20s:  SHM=[%5d]    file=[%d]\n",
                 ARGV_N_PERT_ACTIVE, hdreg.simulator.max_pertur,_MAX_PERTUR);
           fprintf(fpedf,"%-20s:  SHM=[%5d]    file=[%d]\n",
                 ARGV_N_SPARE, hdreg.simulator.spare_snap,_SPARE_SNAP);
           fprintf(fpedf,"%-20s:  SHM=[%5d]    file=[%d]\n",
                 ARGV_N_PERTCL, hdreg.simulator.pert_clear,_PERT_CLEAR);
           fflush(fpedf);
           fclose(fpedf);
           }
      return(ret);
}
/* Fine file dispatcher.c */
