/**********************************************************************
*
*       C Source:               reg_prolog.c
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Dec 12 11:37:21 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: reg_prolog.c-3.1.1 %  (%full_filespec: reg_prolog.c-3.1.1:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)reg_prolog.c	5.3\t3/4/96";
/*
   modulo reg_prolog.c
   tipo 
   release 5.3
   data 3/4/96
   reserved @(#)reg_prolog.c	5.3
*/
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <math.h>
# include <errno.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#endif
#if defined VMS 
# include "vmsipc.h"
#endif
#if defined BANCO_MANOVRA
# include "libmanovra.h"
#endif
# include "sim_param.h"             /* parametri generali LEGO       */
# include "sim_ipc.h"             /* parametri per semafori        */ 
#if defined BANCO_MANOVRA
# include "sim_types.h"  
# include "agg_manovra.h" 
#endif
# include <Rt/RtDbPunti.h>
# include <Rt/RtErrore.h>
#include <Rt/RtMemory.h>

/* ************** Definizione variabili globali ******************** */

int kmod,kini;                      /* puntatori interi         */
MESSAGGI_TASK messaggi_task;
#if defined BANCO_MANOVRA || defined FIFO
int id_msg_taskbm;
#endif

RtDbPuntiOggetto dbpunti;
RtErroreOggetto errore;

STATISTICS statistics;

#if defined UNIX
/*
	Struttura per il controllo dei paramentri caratteristici del lego
*/
/*
La struttura PARSKED viene importata da sim_param.h
modifica del 2/2/96 fatta da Fabio
*/

/*
struct parsked_st
        {
        int kn000;
        int kn001;
        int kn002;
        int kn003;
        int kn004;
        int kn005;
        int kn007;
        int km001;
        int km002;
        int km003;
        int km004;
        int km005;
        };
typedef struct parsked_st PARSKED;
*/
PARSKED parsked_;

/*
     parametri lego
*/
extern int _N000;
extern int _N001;
extern int _N002;
extern int _N003;
extern int _N004;
extern int _N005;
extern int _N007;
extern int _M001;
extern int _M002;
extern int _M003;
extern int _M004;
extern int _M005;

#endif

/* ***************************************************************** */

void reg_prolog()

  {
char *ind;
char task_name[20];
int shr_usr_key;                   /* chiave utente per shared  */
FILE *fp,*fopen();                   /* manipolazione files       */
SIMULATOR *sim;

   /* ** Decodifica le varibili di environment ********************* */

   shr_usr_key = atoi((char *)getenv("SHR_USR_KEY"));

   fp = fopen("lg5c.out","a");
   fprintf(fp,"reg_prolog - shr_usr_key = %d\n",shr_usr_key ); 
   fclose(fp);

   /* ** Traduzione dei nomi logici ******************************** */

                   /*  kmod = numero progressivo che caratterizza
                              questo modello LEGO                    */
                   /*  kini = puntatore all'inizio dell'area var
                              nella shm per questo modello LEGO      */

   kmod = atoi((char *)getenv("SIM_NUM_MOD"));
   kini = atoi((char *)getenv("SIM_VAR_INI"));

   /*
	aggancio al database dei punti
   */
   sprintf(task_name,"task_%d",kmod);
   errore = RtCreateErrore(RT_ERRORE_TERMINALE,task_name);
   sim = (SIMULATOR*)calloc(1,sizeof(SIMULATOR));
   dbpunti = RtCreateDbPunti(errore,NULL,DB_PUNTI_INT,sim);
   free(sim);

   /*
	inizializza le statistiche
   */
   init_statistics(&statistics);

   /* ** Creazione coda messaggi    ******************************** */
   msg_create_fam(shr_usr_key,0);
   msg_create_fam_sim(shr_usr_key,kmod,0);
#if defined BANCO_MANOVRA || defined FIFO
   id_msg_taskbm=msg_create(shr_usr_key+ID_MSG_TASKBM,0);
#endif

#if defined UNIX
   
   /*
        prelega dall'environment i parametri caratteristici del lego
   */
   if(!GetParLego())
	  {
	  fp = fopen("lg5c.out","a");
          fprintf(fp,"reg_prolog - kmod=%d kini=%d \n",kmod,kini);
          fprintf(fp,"reg_prolog - impossibile leggere i parametri LEGO\n");
          fprintf(fp,"reg_prolog - termine della task\n");
          fclose(fp);
          printf("reg_prolog - kmod=%d kini=%d \n",kmod,kini);
          printf("reg_prolog - impossibile leggere i parametri LEGO\n");
          printf("reg_prolog - termine della task\n");
	  exit(0);
	  }
   /*
        controllo la congruenza fra i parametri caratteristici del LEGO e gli
	analoghi valori settati nell'environment:
	I valori settati nell'environment devono essere maggiori dei valori
	con cui e' stata costruita la task
   */
   if((parsked_.kn000>_N000) && (parsked_.kn001>_N001) &&
      (parsked_.kn002>_N002) && (parsked_.kn003>_N003) &&
      (parsked_.kn004>_N004) && (parsked_.kn005>_N005) &&
      (parsked_.kn007>_N007) && (parsked_.km001>_M001) &&
      (parsked_.km002>_M002) && (parsked_.km003>_M003) &&
      (parsked_.km004>_M004) && (parsked_.km005>_M005))
	  {
	  fp = fopen("lg5c.out","a");
          fprintf(fp,"reg_prolog - kmod=%d kini=%d \n",kmod,kini);
          fprintf(fp,"reg_prolog - ERRORE: verificata incongruenza parametri LEGO\n");
          fprintf(fp,"reg_prolog - ERRORE: termine della task\n");
          fclose(fp);
          fprintf(stderr,"reg_prolog - ERRORE: verificata incongruenza parametri LEGO\n");
          fprintf(stderr,"reg_prolog - ERRORE: termine della task\n");
	  exit(0);
	  }
#endif


   fp = fopen("lg5c.out","a");
   fprintf(fp,"reg_prolog - kmod=%d kini=%d \n",kmod,kini);
   fprintf(fp,"reg_prolog - uscita\n");
   fclose(fp);
  }

/* ***************************************************************** */
/* FINE DI reg_prolog */
/* PAGE */
