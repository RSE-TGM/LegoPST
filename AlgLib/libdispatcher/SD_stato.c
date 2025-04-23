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
static char SccsID[] = "@(#)SD_stato.c	5.1\t11/7/95";
/*
   modulo SD_stato.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)SD_stato.c	5.1
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

int SD_stato (processo, statistica_sked)
int     processo;
STATISTICA_SKED * statistica_sked;
{
int     i, comando, size, tipo;
int     ret = -1;

    ret = to_dispatcher (processo, COMANDO_STATO, NULL, 0);
    comando = DATI_ASINCRONI;
    from_dispatcher (processo, &comando, &tipo, (char*)statistica_sked, 
			&size, !IPC_NOWAIT);
    if(size!=sizeof(STATISTICA_SKED))
	return(-1);
    return (ret);
}
