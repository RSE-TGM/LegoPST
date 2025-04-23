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
static char SccsID[] = "@(#)msg_close_fam.c	5.2\t3/8/96";
/*
   modulo msg_close_fam.c
   tipo 
   release 5.2
   data 3/8/96
   reserved @(#)msg_close_fam.c	5.2
*/

#include <stdlib.h>

# include "sim_param.h"
# include "sim_types.h"
# include "libipc.h"




int id_msg_banco;                 /* id coda messaggi banco    */
int id_msg_monit;                 /* id coda messaggi monit    */
int id_msg_sked;                  /* id coda messaggi  da processi read */
int id_msg_pert;                  /* id coda messaggi  perturbazioni */
int id_msg_prep;                   /* id coda messaggi  proc. aus. */
int id_msg_snap;                   /* id coda messaggi  proc. aus. */
int id_msg_leg;                   /* id coda messaggi  proc. aus. */
int id_msg_mandb;                 /* id coda messaggi  proc. mandb. */
int id_msg_aing;                  /* id coda messaggi  proc. aing. */
int id_msg_ret_aing;                  /* id coda messaggi  proc. aing. */
int id_msg_buffer;       /* id coda tampone messaggi da net_read */
int id_msg_staz;         /* id coda messaggi  proc. staz. */

int id_cont;                    /* contatore dei processi agganciati */


int msg_close_fam();

int msg_close_fam()
{
int ret;
int k;
#if defined UNIX
struct msqid_ds buf_msg; /* struttura per contenere le informazioni
                         relative alle code di messaggi */
int semval;

if(semop(id_cont,&op_close[0],3)<0)
        perror("semop messaggi impossibile");
if((semval=semctl(id_cont,1,GETVAL,0))<0)
        perror("GETVAL messaggi impossibile");
if(semval>BIGCOUNT)
        {
        printf("Errore BIGCOUNT messaggi\n");
        exit(0);
        }
else if(semval==BIGCOUNT)
	{
	ret=msg_close(id_msg_banco);
	ret=msg_close(id_msg_monit);
	ret=msg_close(id_msg_sked);
	ret=msg_close(id_msg_snap);
	ret=msg_close(id_msg_prep);
	ret=msg_close(id_msg_leg);
	ret=msg_close(id_msg_pert);
	ret=msg_close(id_msg_mandb);
	ret=msg_close(id_msg_aing);
	ret=msg_close(id_msg_staz);
	ret=msg_close(id_msg_ret_aing);
	ret=msg_close(id_msg_buffer);

	sem_rm(id_cont);
	}
else if(semop(id_cont,&op_unlock[0],1)<0)
        perror("unlock messaggi impossibile");

#endif
return(1);
}
