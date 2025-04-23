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
static char SccsID[] = "@(#)msg_create_fam.c	5.2\t3/8/96";
/*
   modulo msg_create_fam.c
   tipo 
   release 5.2
   data 3/8/96
   reserved @(#)msg_create_fam.c	5.2
*/
# include "libipc.h"
# include "sim_param.h"
# include "sim_types.h"
# include "sim_ipc.h"



int id_msg_banco;                 /* id coda messaggi banco    */
int id_msg_monit;                 /* id coda messaggi monit    */
int id_msg_sked;                  /* id coda messaggi  da processi read */
int id_msg_pert;                  /* id coda messaggi  perturbazioni */
int id_msg_prep;                  /* id coda messaggi  net_prepf22 */
int id_msg_snap;                  /* id coda messaggi  net_sanpshot */
int id_msg_leg;                   /* id coda messaggi   net_legograf */
int id_msg_mandb;                 /* id coda messaggi  proc. mandb. */
int id_msg_aing;                  /* id coda messaggi  proc. aing. */
int id_msg_staz;                  /* id coda messaggi  proc. staz. */
int id_msg_ret_aing;              /* id coda messaggi  proc. aing.(legocad) */
int id_msg_buffer;                /* id coda messaggi tampone dati task
                                     da net_read */

int id_cont;	         	  /* contatore dei processi agganciati */


int msg_create_fam();

int msg_create_fam(shr_usr_key,master)
int shr_usr_key;
int master;
{
int k;

#if defined UNIX
/* crea o aggiorna il semaforo contatore */
id_cont=sem_create(shr_usr_key+ID_SEM_MSG,0);
#endif

id_msg_banco = msg_create(shr_usr_key+ID_MSG_BANCO,master);
id_msg_monit = msg_create(shr_usr_key+ID_MSG_MONIT,master);
id_msg_sked = msg_create(shr_usr_key+ID_MSG_SKED,master);
id_msg_pert = msg_create(shr_usr_key+ID_MSG_PERT,master);
id_msg_prep = msg_create(shr_usr_key+ID_MSG_PREP,master);
id_msg_snap = msg_create(shr_usr_key+ID_MSG_SNAP,master);
id_msg_leg = msg_create(shr_usr_key+ID_MSG_LEG,master);
id_msg_mandb = msg_create(shr_usr_key+ID_MSG_MANDB,master);
id_msg_aing = msg_create(shr_usr_key+ID_MSG_AING,master);
id_msg_staz = msg_create(shr_usr_key+ID_MSG_STAZ,master);
id_msg_ret_aing = msg_create(shr_usr_key+ID_MSG_RET_AING,master);
id_msg_buffer = msg_create(shr_usr_key+ID_MSG_BUFFER,master);
}
