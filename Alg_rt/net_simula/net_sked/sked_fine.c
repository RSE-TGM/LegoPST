/**********************************************************************
*
*       C Source:               sked_fine.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Dec  3 18:46:02 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: sked_fine.c-9 %  (%full_filespec: sked_fine.c-9:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)sked_fine.c	5.5\t3/14/96";
/*
   modulo sked_fine.c
   tipo 
   release 5.5
   data 3/14/96
   reserved @(#)sked_fine.c	5.5
*/
#include <stdio.h>
#include <string.h>
#include <signal.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <sys/shm.h>
#endif
#if defined VMS
# include"vmsipc.h"
#endif
#include <errno.h>
#include "sim_param.h"
#include "sim_types.h"
#include "sim_ipc.h"
#include "sked.h"
#include "comandi.h"
#include "demone.h"
#include <Rt/RtDbPunti.h>
# include <Rt/RtMemory.h>

#define MAXRIGA 200
typedef struct strin_st {
  char *stringa;
  int lun_stringa;
  } STRIN_ST;

extern RtDbPuntiOggetto dbpunti;
extern RtDbPuntiOggetto dbpunti_ext;

extern S02      s02_;		/* struttura modelli e var  */
extern int      nrip;		/* numero di ripetizioni della tabella di
				 * ripetizione */
extern int      nmod;		/* numero delle task */

extern int      semid_aux;	/* identificatore semafori ausiliari */
extern int      id_msg_rettask;	/* identificatore messaggi task */
extern int      id_msg_task[MAX_MODEL];	/* identificatore messaggi task */
extern int      id_msg_sked;	/* identificatore messaggi sked */
extern int      id_msg_banco;	/* identificatore messaggi banco */
extern int      id_msg_pert;	/* identificatore messaggi pert */
extern int      id_msg_prep;	/* id coda messaggi  proc. aus.  */
extern int      id_msg_snap;	/* id coda messaggi  proc. aus.  */
extern int      id_msg_leg;	/* id coda messaggi  proc. aus.  */
extern int      id_msg_to_mandb;
extern int      id_msg_from_mandb;
extern int      id_msg_taskbm;
extern int      shmvid;		/* identificativo shm  varabili  */
extern int      fp_read[MAX_MODEL];
extern int      pidgr;		/* identificatore processo net_prepf22  */
extern int      pidsnap;	/* identificatore processo net_snap  */

extern int      demone_attivo;

extern int      piddemone;	/* identificatore processo demone  */

extern int      tipo_sked;	/* definisce il tipo di sked SLAVE=0 MASTER=1          */

extern int      stato_sked;	/* definisce lo stato dello sked */

extern int      fp_ordini[MAX_MODEL];	/* puntatori dei socket a cui spedire
					 * i comandi, solo per sked master */
extern int      fp_com[MAX_MODEL];

extern char     host[MAX_LUN_HOST];
extern char     host_bm_master[MAX_LUN_HOST];


extern int      id_sh;		/* identificatore sh_var */

extern int      fp_legograf[MAX_MODEL];

extern int      fp_legograf_master;

extern int      numero_slave;

extern int      pid_disp;
extern int      pid_moni;

MESSAGGI_TASK   messaggi_task;

extern int TimeoutTask;

#define DEBUG_CHIUSURA 1

sked_fine()
{
   char path_link[FILENAME_MAX];
   char *path_iniziale;
   int             i,j,ii,ret;
   MSG_NET         messaggio_net;
   MSG_AUS         messaggio_aus;
   STAT_PROC	   *stat_proc;
   int             pid_processi[10];
   int             *stat_task;
   int             cicla;

#if defined DEBUG_CHIUSURA
   fprintf(stderr,"OPERAZIONI DI CHIUSURA \n");
   fflush(stderr);
#endif
   if ((tipo_sked == MASTER) && (stato_sked < STATO_ERRORE))
      {
      messaggio_net.header_net.tipo = COMSIM;
      messaggio_net.header_net.codice = STOP;
      messaggio_net.header_net.lun = 0;
#if defined DEBUG_CHIUSURA
      fprintf(stderr,"------------ INVIA STOP ------------\n");
      fflush(stderr);
#endif
      for (i = 0; i < nmod; i++)
         if (fp_ordini[i] > 0)
            {
            if (writen(fp_ordini[i], &messaggio_net.header_net,
            sizeof(HEADER_NET)) < 0)
            perror("Operazioni chiusura");
            }
      }

/* Via libera task      */
   for (i = 0; i < nmod; i++)
      {
      stat_proc = RtDbPGetPuntTaskStat(dbpunti,i);
      if ((fp_com[i] == 0) && (stato_processo(stat_proc->pid) != 0))
         {
         messaggi_task.mtype = i + 1 + MAX_MODEL;
         messaggi_task.mtext = TASK_FINE;
         msg_snd(id_msg_task[i], &messaggi_task,
                 sizeof(messaggi_task.mtext), IPC_NOWAIT);
#if defined UNIX
         msg_rcv(id_msg_rettask, &messaggi_task,
                 sizeof(messaggi_task.mtext), (long)(i + 1), !IPC_NOWAIT, TimeoutTask);
#endif
         }
      }


   /*
    * Termina i processi di read 
    */
   for (i = 0; i < MAX_MODEL; i++)
      if (fp_read[i] > 1)
         {
#if defined DEBUG_CHIUSURA
         fprintf(stderr,"\t Termina i processi di net_read pid=%d\n",
                 fp_read[i]);
         fflush(stderr);
#endif
         if (kill(fp_read[i], SIGTERM) != 0)
             perror("invio segnale");
         }


   /*
    * termina i processi di net_updbase
    */
   if (tipo_sked == MASTER)
      {
      for (i = 0; i < MAX_MODEL; i++)
         if (fp_legograf[i] > 1)
           {
#if defined DEBUG_CHIUSURA
           fprintf(stderr,"\t Termina i processi net_updbase pid=%d\n",
                   fp_legograf[i]);
           fflush(stderr);
#endif
           if (kill(fp_legograf[i], SIGTERM) != 0)
              perror("invio segnale");
           }
      }
    else
      {
      if (kill(fp_legograf_master, SIGTERM) != 0)
         perror("invio segnale");
      }


   /* Chiusura delle connessioni */
   for (i = 0; i < nmod; i++)
      if (fp_com[i] > 0)
         {
#if defined DEBUG_CHIUSURA
         fprintf(stderr,"\t Chiusura delle connessioni\n");
         fflush(stderr);
#endif
         if (shutdown(fp_com[i], 2) != 0)
            perror("net_sked");
         }


   /*
    * termina il processo di prepf22 
    */
   if ((tipo_sked == MASTER) && (stato_processo(stat_proc->pid)))
      {
#if defined DEBUG_CHIUSURA
      fprintf(stderr,"\t Termina il processo di prepf22\n");
      fflush(stderr);
#endif
      stat_proc = RtDbPGetPuntAusStat(dbpunti,PREP_F22 - 1);
      messaggio_aus.ausiliario.comando = STOP_AUS;
      messaggio_aus.mtype = PREP_F22;
      msg_snd(id_msg_prep, &messaggio_aus, sizeof(TIPO_AUS), !IPC_NOWAIT);
      msg_rcv(id_msg_prep, &messaggio_aus, sizeof(TIPO_AUS),
              (long)(PREP_F22 + OFFSET_AUS), !IPC_NOWAIT, TIMEOUT_AUS);
      }


   /*
    * invia il messaggio di stop al banco manovra 
    */
#if defined BANCO_MANOVRA
   if ((tipo_sked == MASTER) && (s02_.host_bm[0] != NULL))
      {
      printf("sked_fine: invio messaggio datagram a %s\n", host_bm_master);
      do_demone(SLAVE, host_bm_master, host, DEMONE_STOP_BM, "spare1", "spare2");
      }
#endif


   /*
    * termina il processo di demone con mmi
    */
   if ((tipo_sked == MASTER) && (demone_attivo == 1))
      {
#if defined DEBUG_CHIUSURA
      fprintf(stderr,"\t Termina il processo di demone \n");
      fflush(stderr);
#endif
      if (kill(piddemone, SIGTERM) != 0)
	 perror("invio segnale");
      }


  /*
   * kill al dispatcher in caso di errore
   */
   if (tipo_sked == MASTER)
      if (stato_sked == STATO_ERRORE)
         {
         if (kill(pid_disp, SIGTERM) != 0)
            perror("invio segnale");
         }
      else
         {
/*       Sospensione per attendere termine di net_monit e dispatcher */
#if defined DEBUG_CHIUSURA
         fprintf(stderr,"\t Attesa termine net_monit e dispatcher\n");
         fflush(stderr);
#endif
         for(ii=0;ii<9;ii++)
             pid_processi[ii] = 0;
         ii=0;
         pid_processi[ii] = pid_disp;ii++;
         pid_processi[ii] = pid_moni;ii++;
         i=0;
         while( (ret=processi_terminati(pid_processi,ii)) > 0 )
             {
             i++;
             if(i>300)
               {
#if defined DEBUG_CHIUSURA
               fprintf(stderr,"\t Fine termine net_monit e dispatcher\n");
               fflush(stderr);
#endif
               break;
               }
             }
         system("rm lista.proc");
         }


/* Sospensione per attendere la fine delle task */
   cicla=1;
   stat_task = (int*)calloc(nmod,sizeof(int));
   while(cicla==1)
     {
#if defined DEBUG_CHIUSURA1
     fprintf(stderr,"\n");
     fflush(stderr);
#endif
     cicla=0;
     for (i = 0; i < nmod; i++)
       {
       stat_proc = RtDbPGetPuntTaskStat(dbpunti,i);
       stat_task[i] = stato_processo(stat_proc->pid);
       if( (stat_task[i]==1) && (fp_com[i]==0) )
          cicla=1;
#if defined DEBUG_CHIUSURA1
       fprintf(stderr,"\t stat_proc.pid[%d]=%d  ",i,stat_task[i]);
       fprintf(stderr," fp_ord[%d]=%d fp_rd[%d]=%d fp_cm[%d]=%d",
               i,fp_ordini[i],i,fp_read[i],
               i,fp_com[i]
               );
       fflush(stderr);
#endif
       }
     }
    free(stat_task);


   /*
    * rilascia i semafori
    */
#if defined DEBUG_CHIUSURA
   fprintf(stderr,"\t Rilascia i semafori\n");
   fflush(stderr);
#endif
   sgancia_sem_shr();


   /*
    * rilascia le code di messaggi 
    */
#if defined DEBUG_CHIUSURA
   fprintf(stderr,"\t Rilascia le code di messaggi\n"); 
   fflush(stderr);
#endif
   msg_close_fam();
   msg_close_fam_sim(nmod);

   /*
    *   elimina i link simbolici alle dir. proc
    *   per ogni modello (introdotte per avere
    *   i files lg5.out e f21.dat sotto la
    *   dir. out a valle del simulatore.
    */
/*
 Individua la directory corrente
 (dir. del simulatore)
*/

  path_iniziale= (char *) getcwd((char *) NULL, FILENAME_MAX+1);

  for (i = 0; i < nmod; i++)
	{
	strcpy(path_link,path_iniziale);
	strcat(path_link,"/out/");
	strcat(path_link,s02_.model[i].name);
	strcat(path_link,"/proc");
	unlink(path_link);
	}


   /*
    * code create dal processo net_sked 
    */
   msg_close(id_msg_to_mandb);
   msg_close(id_msg_from_mandb);
#if defined BANCO_MANOVRA
   ret = msg_close(id_msg_taskbm);
#endif


   /*
    * Si eliminano le shared memory create da net_sked
    */
#if defined DEBUG_CHIUSURA
   fprintf(stderr,"\t Si eliminano le shared memory (net_sked)\n"); 
   fflush(stderr);
#endif
   RtDestroyDbPunti(dbpunti);
   RtDestroyDbPunti(dbpunti_ext);
   if( tipo_sked==MASTER )
      distruggi_var (id_sh);


   if (
        (tipo_sked  == MASTER)        &&   /* sked_slave non aggiorna stato */
        (stato_sked != STATO_STOP)    && 
        (stato_sked != STATO_ERRORE)     
      )
      {
#if defined DEBUG_CHIUSURA
      fprintf(stderr,"\t Manda un segnale a tutti i processi figli stato=%d \n",stato_sked);
      fflush(stderr);
#endif
      /* manda un segnale a tutti i processi figli (se esistenti) */
      if (kill(0, SIGTERM) != 0)
         perror("invio segnale al gruppo");
      exit(0);
      }

   fprintf(stderr,"SCHEDULATORE TERMINATO\n");
   fflush(stderr);
   exit(0);
}


int legge_riga_bin(riga,fp)
 char riga [MAXRIGA];
 FILE *fp;
{
 int c, k;

 for (k=0; k<MAXRIGA; k++)
   riga[k] = (char)NULL;

 for (k=0; (c = fgetc( fp)) != '\n'; k++)
  {
  riga[k]=c;
  if(c==EOF) return(0);
  }

 riga[k]=(char)NULL;
 return(1);
}

int separa_str(
 char riga[],
 int nstr,
 STRIN_ST strin[])
{
char *s;
int i;

for (s=riga, i=0; i<nstr; i++) {
  strin[i].stringa = s = strtok( s, " \t");
  if(strin[i].stringa==(char*)NULL)
     {
     strin[i].stringa=(char*)malloc(3);
     sprintf(strin[i].stringa," ");
     strin[i].lun_stringa = strlen(strin[i].stringa);
     }
  else
     {
     strin[i].lun_stringa = ( s ? strlen( s) : 0 );
     }
  s = (char*)NULL; }
}


int processi_terminati(int *pid_processi, int controlli)
{
FILE *fp;
char riga[200];
char app_riga[200];
int k;
int pid;
STRIN_ST strin[15];
char nome[FILENAME_MAX];
int ret;
int trovati=0;




#if defined AIX 
    system("ps -ef | grep -v PID>lista.proc");
#endif
#if defined OSF1 || defined SCO_UNIX || defined LINUX
    system("ps -a | grep -v PID>lista.proc");
#endif
#if defined ULTRIX
    system("ps -ax>lista.proc");
#endif

/*
   Controllo esistenza processi
*/
fp=fopen("lista.proc","r");
while(1)
	{
	ret = legge_riga_bin(riga,fp);
        if( ret==0 )
           break;
	strcpy(app_riga,riga);
#if defined AIX
	separa_str(riga,8,strin);
	strcpy(nome,strin[7].stringa);
#endif
#if defined SCO_UNIX
	separa_str(riga,4,strin);
	strcpy(nome,strin[3].stringa);
#endif
#if defined ULTRIX  || defined OSF1 || defined LINUX
	separa_str(riga,5,strin);
	strcpy(nome,strin[4].stringa);
/*
	printf("[%s]    [%s]   [%s]   [%s] [%s]|\n",
                strin[0].stringa,strin[1].stringa,
                strin[2].stringa,strin[3].stringa,
                strin[4].stringa);
*/
#endif
#if defined AIX
        pid=atoi(strin[1].stringa);
#else
	pid=atoi(strin[0].stringa);
#endif
	   for(k=0;k<controlli;k++)
                if( pid_processi[k]==pid )
			{
			/*printf("app_riga=%s| pid=%d\n",app_riga,pid);*/
                        trovati++;
			}
	}
fclose(fp);
return(trovati);
}
