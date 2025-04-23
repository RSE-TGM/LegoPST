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
static char SccsID[] = "@(#)msg_close_fam_sim.c	5.1\t11/7/95";
/*
   modulo msg_close_fam_sim.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)msg_close_fam_sim.c	5.1
*/
# include "sim_param.h"
# include "sim_types.h" 
# include "libipc.h"




int id_msg_task[MAX_MODEL];       /* id coda messaggi  da sked a task */
int id_msg_rettask;               /* id coda messaggi  da sked a task */

int msg_close_fam_sim();

int msg_close_fam_sim(nmod)
int nmod;
{
int ret;
int k;
#if defined UNIX
struct msqid_ds buf_msg; /* struttura per contenere le informazioni
                         relative alle code di messaggi */

ret=msg_close(id_msg_rettask);
for(k=0;k<nmod;k++)
	ret=msg_close(id_msg_task[k]);

#endif
return(1);
}
