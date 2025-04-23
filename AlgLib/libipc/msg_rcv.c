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
static char SccsID[] = "@(#)msg_rcv.c	5.1\t11/7/95";
/*
   modulo msg_rcv.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)msg_rcv.c	5.1
*/
#if defined UNIX
# include <errno.h>
# include <stdio.h>
# include <setjmp.h>
# include <sys/signal.h>
# include <unistd.h>
// GUAG aggiunti
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <signal.h>
typedef void (*sighandler_t)(int);

# include "sim_ipc.h"
# include "libipc.h"
#include "sim_param.h"
#include "sim_types.h"



int msg_rcv();

sighandler_t timeout_msg(void);

#define TIMEOUT_SCAD -100

static jmp_buf Jmp;               /* struttura per il timer  */

/* ************************************************************* */

sighandler_t timeout_msg()
{
        longjmp(Jmp,TIMEOUT_SCAD);
}

/* ************************************************************* */


int msg_rcv(msqid, msgp, msgsz, msgtyp, msgflg,time)
int msqid;
void *msgp;
size_t msgsz;
long msgtyp;
int msgflg;
int time;
{
ssize_t ret;
/* inizializzazione timer  */
/* DEBUG
printf("msg_rcv: 0 GUAG msqid=%d msgsz=%d msgtyp=%d msgflg=%d getpid=%d\n",msqid, msgsz,msgtyp, msgflg, getpid());
fflush(stdout);
*/
if(time!=TIMEOUT_INFINITO)
	{
	if(setjmp(Jmp)==TIMEOUT_SCAD)
		{
		printf("\nTIMEOUT SCADUTO pid= %d\n",getpid());
      		return(TIMEOUT_SCAD);
		}
	signal(SIGALRM,(__sighandler_t)timeout_msg);
	alarm(time);
	}
/* DEBUG
printf("msg_rcv: 1 GUAG msqid=%d msgsz=%d msgtyp=%ld\n",msqid, msgsz,msgtyp);
fflush(stdout);
*/
//  molto sbloccato     ret=msgrcv(msqid, msgp, msgsz, 0, MSG_NOERROR);
ret=msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);
/* DEBUG
printf("msg_rcv: 2 GUAG dopo msgrcv -  msqid=%d\n",msqid);
fflush(stdout);
*/
if(time!=TIMEOUT_INFINITO)
	alarm(0);

return(ret);
}
#endif
