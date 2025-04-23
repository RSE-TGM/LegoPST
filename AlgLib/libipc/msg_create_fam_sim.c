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
static char SccsID[] = "@(#)msg_create_fam_sim.c	5.1\t11/7/95";
/*
   modulo msg_create_fam_sim.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)msg_create_fam_sim.c	5.1
*/
# include "libipc.h"
# include "sim_param.h"
#include "sim_types.h"
# include "sim_ipc.h"



int id_msg_rettask;		  /* id coda messaggi  da task a sked */
int id_msg_task[MAX_MODEL];       /* id coda messaggi  da sked a task */

int msg_create_fam_sim();

int msg_create_fam_sim(shr_usr_key,nmod,master)
int shr_usr_key;
int nmod;
int master;
{
int k;

id_msg_rettask = msg_create(shr_usr_key+ID_MSG_RETTASK,master);
for(k=0;k<nmod;k++)
	id_msg_task[k]=msg_create(shr_usr_key+ID_MSG_TASK+k,master);
}
