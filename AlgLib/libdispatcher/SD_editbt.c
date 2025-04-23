/**********************************************************************
*
*       C Source:               SD_editbt.c
*       Subsystem:              3
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed Jan 22 11:35:44 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: SD_editbt.c-6 %  (%full_filespec: SD_editbt.c-6:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)SD_editbt.c	5.1\t11/7/95";
/*
   modulo SD_editbt.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)SD_editbt.c	5.1
*/
#if defined BACKTRACK
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


int SD_editbt (int processo, BKTAB *tabella_bktk, int modo, int num_bktk)
{
int     i, k, comando, size, tipo;
    BKTAB app_bktk[5];
int     ret = -1;
int app_int;

if (modo != 0)
   {
/* richiesta header singolo backtrack */
    app_int=modo;
    ret = to_dispatcher (processo, COMANDO_EDITSINGLEBT, &app_int, sizeof(int));
    comando = DATI_DISPATCHER;
    from_dispatcher (processo, &comando, &tipo, (char*)&app_bktk[0], 
		&size, !IPC_NOWAIT);
    memcpy(tabella_bktk,&app_bktk[0],sizeof(BKTAB));
   }
else
   {
/* richiesta header di tutti i backtrack */
   ret = to_dispatcher (processo, COMANDO_EDITBT, NULL, 0);
   for (i = 0; i < num_bktk; i++ )
       {
       comando = DATI_DISPATCHER;
       from_dispatcher (processo, &comando, &tipo, 
                        (char*)&tabella_bktk[i], &size, !IPC_NOWAIT);
#if defined STAMPE_DEBUG
       printf("SD k=%d)   mod=%d [%s]\n",
              i, tabella_bktk[i].mod,
              tabella_bktk[i].descr);
       printf("\t st=%d pos=%d prog=%d fz=%d \n",
              tabella_bktk[i].stat,
              tabella_bktk[i].pos,
              tabella_bktk[i].prog,
              tabella_bktk[i].forzato);
       printf("\t t=%f\n",    tabella_bktk[i].tempo);
       printf("\t val=%f\n",  tabella_bktk[i].val);
       printf("\t dsn=%s\n",  tabella_bktk[i].datasn);
       printf("\t tsn=%s\n\n",tabella_bktk[i].temposn);
#endif
       }
   }
   return (ret);
}
#endif
