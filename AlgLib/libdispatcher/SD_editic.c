/**********************************************************************
*
*       C Source:               SD_editic.c
*       Subsystem:              3
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed Jan 22 11:35:47 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: SD_editic.c-4 %  (%full_filespec: SD_editic.c-4:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)SD_editic.c	5.1\t11/7/95";
/*
   modulo SD_editic.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)SD_editic.c	5.1
*/
#include <stdio.h>
#include <string.h>
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

int SD_editic (int processo, SNTAB *tabella_snap, int modo, int num_snap)
{
int     i, k, comando, size, tipo;
SNTAB app_snap[5];
int     ret = -1;
int app_int;

if (modo != 0)
   {
/* richiesta header singolo snapshot */
    app_int=modo;
    ret = to_dispatcher (processo, COMANDO_EDITSINGLEIC, (char *)&app_int, sizeof(int));
    comando = DATI_DISPATCHER;
    from_dispatcher (processo, &comando, &tipo, (char*)&app_snap[0], 
		&size, !IPC_NOWAIT);
    memcpy(tabella_snap,&app_snap[0],sizeof(SNTAB));
   }
else
   {
/* richiesta header di tutti gli snapshot */
    ret = to_dispatcher (processo, COMANDO_EDITIC, NULL, 0);
    for (i = 0; i < num_snap; i++ )
       {
       comando = DATI_DISPATCHER;
       from_dispatcher (processo, &comando, &tipo, (char*)&tabella_snap[i], 
                         &size, !IPC_NOWAIT);
#if defined STAMPE_DEBUG
       printf("SD k=%d)   mod=%d [%s]\n",
                    i, tabella_snap[i].mod,
                    tabella_snap[i].descr);
       printf("\t st=%d pos=%d prog=%d fz=%d \n",
                    tabella_snap[i].stat,
                    tabella_snap[i].pos,
                    tabella_snap[i].prog,
                    tabella_snap[i].forzato);
       printf("\t t=%f\n", tabella_snap[i].tempo);
       printf("\t val=%f\n", tabella_snap[i].val);
       printf("\t dsn=%s\n", tabella_snap[i].datasn);
       printf("\t tsn=%s\n\n", tabella_snap[i].temposn
                    );
#endif
       }
   }
   return (ret);
}
