/**********************************************************************
*
*       C Source:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: %  (%full_filespec: %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)monit_aggiorna.c	5.2\t2/20/96";
/*
   modulo monit_aggiorna.c
   tipo 
   release 5.2
   data 2/20/96
   reserved @(#)monit_aggiorna.c	5.2
*/
/*
 monit_aggiorna.c
   legge i valori dei tempi e delle statistiche da visualizzare 
   in aggiornamento periodico
*/
# include <stdio.h>
# include <errno.h>
#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>
#include <Mrm/MrmPublic.h>
#include <Xm/Text.h>
#include <Xm/List.h>
#include <Xm/DrawingA.h>
#include <Xm/RowColumn.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#endif
#if defined VMS
# include "vmsipc.h"
#endif
#include "sim_param.h"      /* paramteri generali LEGO              */
#include "sim_ipc.h"      /* paramteri generali LEGO              */
#include "monit.h"
#include "dispatcher.h"
#include "libutilx.h"
# include <Rt/RtMemory.h>


/* coda di messaggi con sked */
extern int id_msg_monit;

extern char *ind_sh_top;          /* puntatore inizio shared memory sh_var */

STATISTICA_SKED statistica_sked;


int monit_aggiorna(val)
VALORI_AGG *val;
{
static int bfirst=1;
int appoggio;
MSG_SKDIS messaggio;
int dati_prelevati=0;
int i;
char *app;
int size;
int comando;
int tipo;


if (bfirst==1)
	{
	bfirst=0;
	
#if defined UNIX
	bzero(&statistica_sked,sizeof(STATISTICA_SKED));
#endif
#if defined VMS
	app=(char*)(&statistica_sked);
	for(i=0;i<sizeof(STATISTICA_SKED);i++)
		app[i]=0;
#endif
	val->stato_sked_a=1;
	}

/* preleva i dati statistici inviati dallo sked */
comando=DATI_ASINCRONI;
from_dispatcher(MONIT,&comando,&tipo,(char *)&statistica_sked,&size,IPC_NOWAIT);
if(size>0)
{
while(size>0) 
	{
	comando=DATI_ASINCRONI;
	from_dispatcher(MONIT,&comando,&tipo,(char*)&statistica_sked,&size,IPC_NOWAIT);
	}
dati_prelevati=1;
}



if(dati_prelevati==1)
   {
/* Scrive su un file le statistiche */
/*
   ScriviStatistiche(&statistica_sked);
*/

   if(val->tempo_sim!=statistica_sked.tempo_sim)
	{
	val->tempo_sim_a=1;
	val->tempo_sim=statistica_sked.tempo_sim;
	}

   if(val->tempo_ritardo!=statistica_sked.tempo_ritardo*val->timescaling_sim)
	{
	val->tempo_ritardo_a=1;
	val->tempo_ritardo=statistica_sked.tempo_ritardo*val->timescaling_sim;
	}

   if(val->timescaling_sim!=statistica_sked.timescaling_sim)
	{
	val->timescaling_sim_a=1;
	val->timescaling_sim=statistica_sked.timescaling_sim;
	}

   if(val->stepscaling_sim!=statistica_sked.stepscaling_sim)
	{
	val->stepscaling_sim_a=1;
	val->stepscaling_sim=statistica_sked.stepscaling_sim;
	}

   if(val->tempo_reale!=val->tempo_sim+val->tempo_ritardo)
	{
	val->tempo_reale_a=1;
	val->tempo_reale=val->tempo_sim+val->tempo_ritardo;
	}

   appoggio=(int)(spazio_disco("/usr/users")/1024);   /* Kbytes
                                                 consumabili dall'utente */
   if(val->spazio_dsk!=appoggio)
	{
	val->spazio_dsk=appoggio;
	val->spazio_dsk_a=1;
	}

   if(val->grandezze_mem!=statistica_sked.grandezze_mem)
	{
	val->grandezze_mem_a=1;
	val->grandezze_mem=statistica_sked.grandezze_mem;
	}

   if(val->passo_reg!= (int)statistica_sked.passo_reg)
	{
	val->passo_reg_a=1;
	val->passo_reg= (int)statistica_sked.passo_reg;
	}

   if(statistica_sked.grandezze_mem!=0)
      if(val->tempo_simMB= val->passo_reg*1.0e+6/
                    ((statistica_sked.grandezze_mem+1)*sizeof(float)))
	{
	val->tempo_simMB_a=1;
	val->tempo_simMB= val->passo_reg*1.0e+6/
                    ((statistica_sked.grandezze_mem+1)*sizeof(float));
	}


   if(val->stato_sked!=statistica_sked.stato_sked)
	{
	val->stato_sked_a=1;
	val->stato_sked=statistica_sked.stato_sked;
	}

   if(val->stato_snapshot!=statistica_sked.stato_snapshot)
	{
	val->stato_snapshot_a=1;
	val->stato_snapshot=statistica_sked.stato_snapshot;
	}

   if(val->stato_prepf22!=statistica_sked.stato_prepf22)
	{
	val->stato_prepf22_a=1;
	val->stato_prepf22=statistica_sked.stato_prepf22;
	}
   for(i=0;i<numero_modelli(ind_sh_top);i++)
	{
	if(val->stato_task[i]!=statistica_sked.stato_task[i])
	   {
	   val->stato_task_a[i]=1;
	   val->stato_task[i]=statistica_sked.stato_task[i];
	   }
	}
#if defined BANCO_MANOVRA
   for(i=0;i<MAX_SLAVE+2;i++)
	{
	if(val->stato_agg_manovra[i]!=statistica_sked.stato_agg_manovra[i])
		{
		val->stato_agg_manovra_a[i]=1;
	        printf("monit_aggiorna: stato_agg_manovra[%d]=%d\n",
		        i,statistica_sked.stato_agg_manovra[i]);
		val->stato_agg_manovra[i]=statistica_sked.stato_agg_manovra[i];
		}
	}
   for(i=0;i<MAX_SLAVE+1;i++)
	{
	if(val->stato_agg_manovra_pert[i]!=
		statistica_sked.stato_agg_manovra_pert[i])
		{
		val->stato_agg_manovra_pert_a[i]=1;
	        printf("monit_aggiorna: stato_agg_manovra_pert[%d]=%d\n",
		        i,statistica_sked.stato_agg_manovra_pert[i]);
		val->stato_agg_manovra_pert[i]=
			statistica_sked.stato_agg_manovra_pert[i];
		}
	}
   if(val->stato_agg_manovra_stato!=statistica_sked.stato_agg_manovra_stato)
	{
	val->stato_agg_manovra_stato_a=1;
	val->stato_agg_manovra_stato=statistica_sked.stato_agg_manovra_stato;
	}
#endif
#if defined SCADA
   if(val->stato_agg_scada!=statistica_sked.stato_agg_scada)
	{
	val->stato_agg_scada_a=1;
	val->stato_agg_scada=statistica_sked.stato_agg_scada;
	}
#endif
   }
}



/*
    Scrive su un file le statistiche
*/
int ScriviStatistiche(STATISTICA_SKED *ss)
{
FILE *ff;
int ii;

ff=fopen("statistiche_sked.edf","w");

fprintf(ff,"DATI STATISTICHE RICEVUTI DA net_monit stati=%d\n",
       ss->stato_sked);

fprintf(ff,"tempo_sim=%f tempo_ritardo=%f timescaling_sim=%f stepscaling=%f\n",
       ss->tempo_sim, ss->tempo_ritardo,
       ss->timescaling_sim, ss->stepscaling_sim);

fprintf(ff,"grandezze_mem=%d passo_reg=%f tempo_cpu=%f\n",
       ss->grandezze_mem, ss->passo_reg, ss->tempo_cpu);

fprintf(ff,"MAX_SLAVE+1=%d MAX_MODEL=%d\n",MAX_SLAVE+1,MAX_MODEL);

for(ii=0;ii<MAX_SLAVE+1;ii++)
    fprintf(ff,"  tempi_sked[%d]=%f\n", ii,ss->tempi_sked[ii]);

for(ii=0;ii<MAX_MODEL;ii++)
    fprintf(ff,"  tempi_task[%d]=%f  stato_task[%d]=%f\n",
             ii,ss->tempi_task[ii], ii,ss->stato_task);

#if defined BANCO_MANOVRA
for(ii=0;ii<MAX_SLAVE+1;ii++)
    fprintf(ff,"  s_agg_manovra[%d]=%d  s_agg_manovra_pert[%d]=%d\n",
            ii,ss->stato_agg_manovra[ii], ii,ss->stato_agg_manovra_pert[ii]);
fprintf(ff,"  s_agg_man[MAX_SLAVE+2]=%d  s_agg_man_stato=%d, s_a_scada=%d\n",
        ss->stato_agg_manovra[MAX_SLAVE+2], ss->stato_agg_manovra_stato,
        ss->stato_agg_scada);
#endif

fprintf(ff,"  s_snapshot=%d  s_prepf22=%d op_eseguita=%d n_op=%d\n",
        ss->stato_snapshot, ss->stato_prepf22,
        ss->operazione_eseguita, ss->num_operazione);

fprintf(ff,"****Fine file****\n");

fclose(ff);
}
