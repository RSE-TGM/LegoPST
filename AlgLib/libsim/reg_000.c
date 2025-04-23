/**********************************************************************
*
*       C Source:               reg_000.c
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Fri Aug 29 11:53:05 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: reg_000.c-13 %  (%full_filespec: reg_000.c-13:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)reg_000.c	5.2\t5/16/96";
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
#include <unistd.h>
# include <errno.h>
# include <time.h>
# include <math.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#endif
#if defined VMS 
# include "vmsipc.h"
#endif
# include "sim_param.h"          /* parametri per lego           */
# include "sim_ipc.h"          /* parametri per semafori           */
# include "sim_types.h"       
# include <Rt/RtDbPunti.h>
#if defined BANCO_MANOVRA
# include "agg_manovra.h"    
#endif
#include <Rt/RtMemory.h>

#if defined VMS
#define CLOCKS_PER_SEC CLK_TCK
#endif


/* **************** Definizione variabili globali ****************** */

extern int kmod;                       /* puntatori interi             */
int id_msg_task[MAX_MODEL];   /* identificatore coda messaggi  */
int id_msg_rettask;	      /* identificatore coda messaggi verso sked */
#if defined BANCO_MANOVRA || defined FIFO
int id_msg_taskbm;           /* identificatore coda messaggi verso BM */
int bm_agganciato=0;
char host[MAX_LUN_HOST];
MESSAGGI_TASK messaggi_bm;
#endif
extern MESSAGGI_TASK messaggi_task;
extern RtDbPuntiOggetto dbpunti;

extern STATISTICS statistics;

extern void f22_open(char *,char *,char *,int );
extern void f22_write(float,float*,int);

/*
Variabile globale per passaggio di parametri a sync_task
*/
SYNC_TASK_PARAM var;

void sync_task(SYNC_TASK_PARAM *var)
{
  static double tempo=0;                         /* tempo task        */
  float app_tempo;
  static int ordine,prologo=0;
  static double step;
  float f_step;
  float app_step,app_dt;
  int ret;                           /* intero spare                 */
  FILE *fp, *fopen();                /* manipolazione file           */
  char *reinit = NULL;
#if defined BANCO_MANOVRA || defined FIFO
  int fd_fifo;
  char path_fifo[70];
  char *getenv();
#endif
#if defined UNIX
  /*
	per f21.dat
  */
  char nome_var[2][MAX_LUN_NOME_VAR];
  char descr_var[2][MAX_LUN_DESCR_VAR];
  char buffer_nome[MAX_LUN_NOME_VAR*2];
  char buffer_descr[MAX_LUN_DESCR_VAR*2];
  float buffer_valori[2];
  int num_valori_f22;
#endif
  STAT_PROC *stat_proc;


// printf("reg000: prima di switch ipr=%d\n", *(var->ipr));

switch (*(var->ipr)) 
{
case 3:
  {
  reg_prolog(); /* aggancio shared memory e coda messaggi */
  RtDbPGetDt(dbpunti,kmod-1,&app_dt);
  RtDbPGetStepScaling(dbpunti,&app_step);
  *(var->tstep) = app_dt * app_step;
  step = (double) app_dt;
  step = (double)floor(step*10); 
  step = step/10;
  prologo=1;
  break;
  }
case 1:
  {
  /* prima chiamata */

// printf("reg000: prima di reg_prolog\n");


  if (prologo == 0)
     reg_prolog(); /* aggancio shared memory e coda messaggi */

// printf("reg000: dopo    reg_prolog\n");

  RtDbPGetDt(dbpunti,kmod-1,&app_dt);

// printf("reg000: dopo RtDbPGetDt    \n");

  RtDbPGetStepScaling(dbpunti,&app_step);

// printf("reg000: dopo  RtDbPGetStepScaling \n");

  *(var->tstep) = app_dt * app_step;
					/* lettura passo di tempo effettuata ad 
			                ogni iterazione; modifica del 16/9/93 */
  step = (double) app_dt * app_step;
  step = (double)floor(step*10); 
  step = step/10;

  reinit = getenv("REINITTASK");
  if(strcmp(reinit,"NO")==0) {
     (var->xy,var->uu,var->px,var->dati,var->neqsis,var->nu,var->neqdif,var->ndati,var->cnxy,var->cnuu);
//     printf("reg000: write in shrmem!!! NO REINIT\n");
  }

//  printf("reg000: REINITTASK = %s\n", reinit);

#if defined UNIX
  /*
	scrittura file f21.dat
  */
  if(*(var->t_proc)!=0)
	{
  	memset(buffer_nome,' ',MAX_LUN_NOME_VAR*2);
  	memset(buffer_descr,' ',MAX_LUN_DESCR_VAR*2);

  	sprintf(&nome_var[0][0],"NJAC    ");
  	sprintf(&nome_var[1][0],"NITE    ");
  	sprintf(&descr_var[0][0],"NUMERO DI JACOBIANI  PER OGNI PASSO DI INTEGRAZIONE");
	sprintf(&descr_var[1][0],"NUMERO DI ITERAZIONI PER OGNI PASSO DI INTEGRAZIONE");

  	memcpy(&buffer_nome[0],&nome_var[0][0],MAX_LUN_NOME_VAR-1);
  	memcpy(&buffer_nome[MAX_LUN_NOME_VAR-1],
		&nome_var[1][0],MAX_LUN_NOME_VAR-1);

  	memcpy(&buffer_descr[0],&descr_var[0][0],MAX_LUN_DESCR_VAR-1);
  	memcpy(&buffer_descr[MAX_LUN_DESCR_VAR-1],
		&descr_var[1][0],MAX_LUN_DESCR_VAR-1);

  	num_valori_f22=2;
#if defined UNIX
  	f22_open("f21.dat",
		&buffer_nome[0],&buffer_descr[0],num_valori_f22);
#endif

  	buffer_valori[0]= (float)(*(var->jac));
  	buffer_valori[1]= (float)(*(var->itert));
  	num_valori_f22=2;
  	f_step= 0;
  	f22_write(f_step,buffer_valori,num_valori_f22);
	}
#endif
  /* segnale fine passo */
  messaggi_task.mtype=kmod;
  msg_snd(id_msg_rettask,&messaggi_task,sizeof(messaggi_task.mtext),!IPC_NOWAIT);
  /* attesa nuovo passo */
  msg_rcv(id_msg_task[kmod-1],&messaggi_task,sizeof(messaggi_task.mtext),
               (long)(kmod+MAX_MODEL),!IPC_NOWAIT,0);
  /*
          TEST TASK
  */
  while(messaggi_task.mtext==TASK_TEST)
  {
          /*printf("reg_000: Test task %d OK\n",kmod);*/
          messaggi_task.mtype=kmod;
          msg_snd(id_msg_rettask,&messaggi_task,
                          sizeof(messaggi_task.mtext),!IPC_NOWAIT);
          msg_rcv(id_msg_task[kmod-1],&messaggi_task,
             sizeof(messaggi_task.mtext), (long)(kmod+MAX_MODEL),!IPC_NOWAIT,0);
  }
  if(messaggi_task.mtext==TASK_SNAP)
		{
		reg_rdshm(var->xy,var->uu,var->px,var->dati,var->neqsis,var->nu,var->neqdif,var->ndati,var->cnxy,var->cnuu);
                *(var->ipr) = SIGNAL_LOAD_SNAP;
		*(var->ifine)=1;
		}
  if(messaggi_task.mtext==TASK_SAVE_SNAP)
                {
                if( (*(var->tasksicre)) )
                   {
		   /*printf("reg000: initialize TASK_SAVE_SNAP \n");*/
                   *(var->ipr) = SIGNAL_SAVE_SNAP;
		   *(var->ifine)=1;
                   }
		}
#if defined BANCO_MANOVRA
  /*
         AGGANCIO BM 
  */
  while(messaggi_task.mtext==TASK_BM)
  {
#if defined FIFO
	  if(bm_agganciato==0)
		{
		strcpy(host,io_sono());
		sprintf(path_fifo,"%s/agg_manovra.%s",getenv("HOME"),host);
		fd_fifo=fifo_create(path_fifo,1);
		}
#endif
          /*printf("reg_000: Test task %d ricevuto messaggio agg_manovra\n",kmod);*/
          msg_rcv(id_msg_task[kmod-1],&messaggi_task,
             sizeof(messaggi_task.mtext), (long)(kmod+MAX_MODEL),!IPC_NOWAIT,0);
	  bm_agganciato=1;
  }
#endif
  ordine=messaggi_task.mtext;
/*
  Vengono riletti gli ingresi allo scopo di non perdere una perturbazione
  inviata prima di eseguire il primo passo della simulazione.
*/
  if( (ordine==TASK_NEXT) || (ordine==TASK_STEP) )
    reg_ing(var->uu,var->neqsis,var->nu,var->cnuu);
  /*
  Inserisco lettura tstep
  */
  RtDbPGetDt(dbpunti,kmod-1,&app_dt);
  RtDbPGetStepScaling(dbpunti,&app_step);
  *(var->tstep) = app_dt *app_step;
  
  break;
  }
case 2:
  {
  /* successive chiamate */
  /*
	  aggiorna le statistiche
  */
  get_statistics(&statistics);
  stat_proc = RtDbPGetPuntTaskStat(dbpunti,kmod-1);
  stat_proc->cpu_use = statistics.user_time + statistics.system_time;

  RtDbPGetDt(dbpunti,kmod-1,&app_dt);
  RtDbPGetStepScaling(dbpunti,&app_step);
  *(var->tstep) = app_dt *app_step;
					/* lettura passo di tempo effettuata ad 
			                ogni iterazione; modifica del 16/9/93 */
  step = (double) app_dt *app_step;
  step = (double)floor(step*10); 
  step = step/10;
#if defined UNIX
  /*
	scrittura file f21.dat
  */
  if(*(var->t_proc)!=0)
	{
  	buffer_valori[0]= (float)(*(var->jac));
  	buffer_valori[1]= (float)(*(var->itert));
  	num_valori_f22=2;
	RtDbPGetTimeTask(dbpunti,kmod-1,&app_tempo);
  	f_step= app_tempo+step;
  	f22_write(f_step,buffer_valori,num_valori_f22);
	}
#endif
  switch (ordine){
        case TASK_SAVE_SNAP:
                {
                if( (*(var->tasksicre)) )
                   {
	           /*printf(" reg000:inizio TASK_SAVE_SNAP \n");*/

                   /*
		   Scrivo in shm i dati appena letti da sicre
		   */
		   reg_wrshm(var->xy,var->uu,var->px,var->dati,var->neqsis,
                             var->nu,var->neqdif,var->ndati,var->cnxy,var->cnuu);
		   *(var->ifine)=1;
                   }
		break;
                }
	case TASK_NEXT:
	case TASK_SNAP:
		{
                /**************************************
                if(ordine==TASK_NEXT)
			printf("reg000:TASK_NEXT prima msg \n");
		else
			printf("reg000:TASK_SNAP prima msg \n");
                ****************************************/
		reg_wrshm(var->xy,var->uu,var->px,var->dati,var->neqsis,var->nu,var->neqdif,var->ndati,var->cnxy,var->cnuu);
		*(var->ifine)=1;
		break;
		}
	case TASK_STEP:
		{
		/*printf("reg000:TASK_STEP prima msg \n");*/
RtDbPGetTime(dbpunti,var->tsim);
    /*printf("reg_000:  task No %d\n",kmod);*/
/*printf("reg000 T_STEP: var->tsim =%f\n",*(var->tsim));*/
		reg_wrshm(var->xy,var->uu,var->px,var->dati,var->neqsis,
                          var->nu,var->neqdif,var->ndati,var->cnxy,var->cnuu);
		*(var->ifine)=1;
                break;
		}
	case TASK_FINE:
		{
		/*printf("reg000:TASK_FINE prima msg \n");*/
                *(var->ifine)=2;
		break;
		}
	}

  /* aggiornamento tempo singola task */
  RtDbPGetTimeTask(dbpunti,kmod-1,&app_tempo);
  tempo=step+app_tempo;
  app_tempo=(float)tempo;
  RtDbPPutTimeTask(dbpunti,kmod-1,app_tempo);
  messaggi_task.mtype=kmod;
#if defined BANCO_MANOVRA
  if(bm_agganciato)
	{
#if defined FIFO
	write_fifo(fd_fifo,&messaggi_task,sizeof(MESSAGGI_TASK));
#else
  	msg_snd(id_msg_taskbm,&messaggi_task,
		sizeof(messaggi_task.mtext),!IPC_NOWAIT);
#endif
	}
#endif
  /* segnale fine passo */
  msg_snd(id_msg_rettask,&messaggi_task,sizeof(messaggi_task.mtext),!IPC_NOWAIT);
  /* attesa nuovo passo */
  if(ordine!=TASK_FINE)
   {
	msg_rcv(id_msg_task[kmod-1],&messaggi_task,sizeof(messaggi_task.mtext),
               (long)(kmod+MAX_MODEL),!IPC_NOWAIT,0);

/******************************************
Ricevuto msg riaggiorno tstep nel caso fosse cambiato dopo un comando FREEZE
Modifica by Fabio 2/12/97
*******************************************/

  RtDbPGetDt(dbpunti,kmod-1,&app_dt);
  RtDbPGetStepScaling(dbpunti,&app_step);
  *(var->tstep) = app_dt *app_step;
/*
Acquisisco tempo di simulazione
*/
RtDbPGetTime(dbpunti,var->tsim);
/*printf("reg000: var->tsim =%f\n",*(var->tsim));*/
  /*
          TEST TASK
  */
  while(messaggi_task.mtext==TASK_TEST)
  {
          /*printf("Test task %d OK\n",kmod);*/
          messaggi_task.mtype=kmod;
          msg_snd(id_msg_rettask,&messaggi_task,
                          sizeof(messaggi_task.mtext),!IPC_NOWAIT);
          msg_rcv(id_msg_task[kmod-1],&messaggi_task,
             sizeof(messaggi_task.mtext), (long)(kmod+MAX_MODEL),!IPC_NOWAIT,0);
  }
  if(messaggi_task.mtext==TASK_SNAP)
		{
		reg_rdshm(var->xy,var->uu,var->px,var->dati,var->neqsis,var->nu,var->neqdif,var->ndati,var->cnxy,var->cnuu);
                *(var->ipr) = SIGNAL_LOAD_SNAP;
		/*printf("reg_000: La task %d carica lo snapshot\n",kmod);*/
		*(var->ifine)=1;
		}
#if defined BANCO_MANOVRA
  /*
         AGGANCIO BM 
  */
  while(messaggi_task.mtext==TASK_BM)
  {
          /*printf("reg_000: Test task %d ricevuto messaggio agg_manovra\n",kmod);*/
#if defined FIFO
	  if(bm_agganciato==0)
		{
		strcpy(host,io_sono());
		sprintf(path_fifo,"%s/agg_manovra.%s",getenv("HOME"),host);
		fd_fifo=fifo_create(path_fifo,1);
		}
#endif
          msg_rcv(id_msg_task[kmod-1],&messaggi_task,
             sizeof(messaggi_task.mtext), (long)(kmod+MAX_MODEL),!IPC_NOWAIT,0);
	  bm_agganciato=1;
  }
#endif
    }
  if(ordine==TASK_FINE)
    {
#if defined BANCO_MANOVRA
  messaggi_task.mtext=TASK_FINE;
  if(bm_agganciato)
#if defined FIFO
	write_fifo(fd_fifo,&messaggi_task,sizeof(MESSAGGI_TASK));
#else
  	msg_snd(id_msg_taskbm,&messaggi_task,
		sizeof(messaggi_task.mtext),!IPC_NOWAIT);
#endif
#endif
    /* chiusura o sgancio code messaggi */
    /*printf("reg_000: termine task %d\n",kmod);*/
    msg_close_fam();
    *(var->ifine)=2;
    }
  ordine=messaggi_task.mtext;
  if(ordine==TASK_NEXT)
		{
		reg_ing(var->uu,var->neqsis,var->nu,var->cnuu);
/*printf("DEBUG:reg000: TASK_NEXT finale No=%d\n",kmod);*/
		}
  if(ordine==TASK_STEP)
		{
		reg_ing(var->uu,var->neqsis,var->nu,var->cnuu);
		reg_rdshm(var->xy,var->uu,var->px,var->dati,var->neqsis,var->nu,var->neqdif,var->ndati,var->cnxy,var->cnuu);
                *(var->ipr) = -1;
/*printf("DEBUG:reg000: TASK_STEP finale con ipr=-1\n");*/
		}
  if(ordine==TASK_SAVE_SNAP)
                {
                if( (*(var->tasksicre)) )
                   {
		   /*printf("reg000: finale TASK_SAVE_SNAP \n");*/
                   *(var->ipr) = SIGNAL_SAVE_SNAP;
                   }
		}
/*printf("\n\t DEBUG: fine REG000 per task No=%d\n",kmod);*/
  break;
  }
}
}

void reg000_(ip,xy,uu,px,dati,neqsis,nu,neqdif,ndati,cnxy,cnuu,tstep,ifine,
	t_proc,jac,itert)
 int *ip,*neqsis,*nu,*neqdif,*ndati,*ifine,*t_proc,*jac,*itert;
 float *tstep;
 float (*xy),(*uu),(*px),(*dati),(*cnxy),(*cnuu); 
{
float t_sim;
int task_sicre=0;

var.ipr=ip;
var.xy=xy;
var.uu=uu;
var.px=px;
var.cnxy=cnxy;
var.cnuu=cnuu;
var.dati=dati;
var.neqsis=neqsis;
var.nu=nu;
var.neqdif=neqdif;
var.ndati=ndati;
var.ifine=ifine;
var.tstep=tstep;

var.t_proc=t_proc;
var.jac = jac;
var.itert=itert;
var.tsim=&t_sim;
var.tasksicre=&task_sicre;

// printf("prima di sync_task\n");

sync_task(&var);


// printf("dopo di sync_task\n");

}

/*
Funzione per initialize task-nolego-sicre
Da modificare : non e' necessario passare l' intera struttura INTERFACE_VAR
*/
void initialize_syncronization( INTERFACE_VAR *vartask)
{

var.ipr=&(vartask->ipr);  /* flag per inizializzazione */
*(var.ipr)=1;
/*
Aggancio le variabili di interesse della task SICRE
*/
var.xy=vartask->xy;
var.uu=vartask->uu;
var.px=vartask->px;
var.cnxy=vartask->cnxy;
var.cnuu=vartask->cnuu;
var.dati=vartask->dati;
var.neqsis=&(vartask->neqsis);
var.nu=&(vartask->nu);
var.neqdif=&(vartask->neqdif);
var.ndati=&(vartask->ndati);
var.ifine=&(vartask->ifine);
var.tstep=&(vartask->tstep);

var.t_proc=&(vartask->t_proc);
var.jac = &(vartask->jac);
var.itert=&(vartask->itert);
var.tsim=&(vartask->tsim);
var.tasksicre=&(vartask->tasksicre);
*(var.tasksicre)=1;
sync_task(&var);
}
/*
Funzione per la sincronizzazione della task SICRE
con le task di regolazione
*/
void syncronize_sicre_run_time()
{
*(var.ipr)=2;


sync_task(&var);
}
/*****************************************************

******************************************************/

#if defined BANCO_MANOVRA
#if defined UNIX
int test_code()
{
struct msqid_ds  buf;
int tentativi=0;

if(msgctl(id_msg_taskbm,IPC_STAT,&buf))
	{
	fprintf(stderr,"Error reg_000(task %d): impossibile testare lo stato della coda\n",kmod-1);
	exit(1);
	}
	
while(buf.msg_qnum>10)
   {
   sleep(1);
   if(msgctl(id_msg_taskbm,IPC_STAT,&buf))
	{
	fprintf(stderr,"Error reg_000(task %d): impossibile testare lo stato della coda\n",kmod-1);
	exit(1);
	}
   if(tentativi==100)
   	fprintf(stderr,"Warning reg_000(task %d): eccesso carico coda con processo agg_manovra\n",kmod-1);
	
   if(++tentativi==10000)
	{
        fprintf(stderr,"Error reg_000(task %d): eccesso carico prolungato coda con processo agg_manovra - exit\n",kmod-1);
	exit(1);
	}
   }
return(1);
}
#endif
#endif
