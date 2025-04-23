/**********************************************************************
*
*       C Source:               from_dispatcher.c
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Fri Aug 29 14:24:05 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: from_dispatcher.c-10 %  (%full_filespec: from_dispatcher.c-10:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)from_dispatcher.c	5.1\t11/7/95";
/*
   modulo from_dispatcher.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)from_dispatcher.c	5.1
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined UNIX
#include <sys/types.h>
#include <sys/ipc.h>
#endif
#if defined VMS
#include "vmsipc.h"
#endif
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "libdispatcher.h"

#include "sim_ipc.h"
#include "libipc.h"

int from_dispatcher (processo, comando, tipo, dati, size, modo)
int     processo;
int    *comando;
int    *tipo;
char   *dati;
int    *size;
int     modo;
{
static int  id = -1;
static int  id_ret = -1;

static int  id_sem = -1;

int     shr_usr_key;
    MSG_DISPATCHER messaggio_dispatcher;
    MSG_ACK msg_ack;

/* aggancio semaforo */
    if (id_sem == -1) {
       shr_usr_key = atoi (getenv ("SHR_USR_KEY"));
       id_sem = sem_create(shr_usr_key + ID_SEM_DISP, 0);
// printf("from_dispatcher: id_sem = %d\n",id_sem);
    }

/* aggancio coda messaggi */
    if (id == -1)
    {
	shr_usr_key = atoi (getenv ("SHR_USR_KEY"));
	if (processo == MONIT)
	{
	    id_ret = msg_create (shr_usr_key + ID_MSG_FROM_MONIT, 0);
	    id = msg_create (shr_usr_key + ID_MSG_TO_MONIT, 0);
	}
	if (processo == BI)
	{
	    id_ret = msg_create (shr_usr_key + ID_MSG_FROM_BI, 0);
	    id = msg_create (shr_usr_key + ID_MSG_TO_BI, 0);
	}
	if (processo == SUPERVISIONE)
	{
	    id_ret = msg_create (shr_usr_key + ID_MSG_FROM_SUPER, 0);
	    id = msg_create (shr_usr_key + ID_MSG_TO_SUPER, 0);
	}
	if (processo == BM)
	{
	    id_ret = msg_create (shr_usr_key + ID_MSG_FROM_BM, 0);
	    id = msg_create (shr_usr_key + ID_MSG_TO_BM, 0);
	}
	if (processo == TASTIERA)
	{
	    id_ret = msg_create (shr_usr_key + ID_MSG_FROM_TAST, 0);
	    id = msg_create (shr_usr_key + ID_MSG_TO_TAST, 0);
	}
	if (processo == SKED)
	{
	    id_ret = msg_create (shr_usr_key + ID_MSG_FROM_SKED, 0);
	    id = msg_create (shr_usr_key + ID_MSG_TO_SKED, 0);
	}
    }
/* preleva il messaggio */
    if (msg_rcv (id, &messaggio_dispatcher, sizeof (MSG_DISPATCHER),
		(long)*comando, modo,0) < 0)
    {
	*comando = -1;
	*size = -1;
    }
    else
    {
	*size = messaggio_dispatcher.comando_dispatcher.size;
	*comando = messaggio_dispatcher.mtype;
	*tipo = messaggio_dispatcher.comando_dispatcher.tipo;
	memcpy (dati, messaggio_dispatcher.comando_dispatcher.dati,
		*size);
	if (*comando != DATI_ASINCRONI)
	{
	    /* invia ack ack */
	    msg_ack.mtype = 1000;
	    msg_ack.ret = 1;
            if(msg_snd(id_ret, &msg_ack, sizeof(MSG_ACK)-sizeof(long),
                        !IPC_NOWAIT)<0) {
               printf("FROM_DISPATCHER: msgsnd failed (ACKNOLEDGE)\n");
               return(-1);
            }
            sem_signal_disp(id_sem);
	}
    }
}
