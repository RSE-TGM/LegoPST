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
static char SccsID[] = "@(#)SD_inizializza.c	5.2\t3/14/96";
/*
   modulo SD_inizializza.c
   tipo 
   release 5.2
   data 3/14/96
   reserved @(#)SD_inizializza.c	5.2
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "sim_param.h"
#include "dispatcher.h"
#include "libdispatcher.h"


int     SD_inizializza (processo)
int     processo;
{
int     ret = -1;
char dati[10];

    ret = getpid();
    memcpy(&dati[0],&ret,sizeof(int));
    ret = to_dispatcher (processo, COMANDO_INITIALIZE, dati, sizeof(int));
    return (ret);
}
