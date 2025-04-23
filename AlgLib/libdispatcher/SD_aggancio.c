/**********************************************************************
*
*       C Source:               SD_aggancio.c
*       Subsystem:              3
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed Jan 22 11:35:42 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: SD_aggancio.c-4 %  (%full_filespec: SD_aggancio.c-4:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)SD_aggancio.c	5.1\t11/7/95";
/*
   modulo SD_aggancio.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)SD_aggancio.c	5.1
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "sim_param.h"
#include "dispatcher.h"
#include "libdispatcher.h"

int     SD_aggancio (int processo, char *err)
{
int      ret = -1;
int      comando, size, tipo, k;
char    *app;
int      errsked,errdisp,errshm;
int      sizemax=10*sizeof(int);

 printf("SD_aggancio: GUAG prima di to_dispatcher processo=%d\n",processo);
    ret = to_dispatcher (processo, COMANDO_AGGANCIO, NULL, 0);
 printf("SD_aggancio: GUAG dopo di to_dispatcher\n");
    if(ret<0)
       return(ret);
    comando = DATI_DISPATCHER;
    app = (char*)malloc(sizemax);
    from_dispatcher (processo, &comando, &tipo, app,
                     &size, !IPC_NOWAIT);
    if( (size>0)&&(size<sizemax) )
      memcpy(err,app,size);
    free(app);
    return (0);
}
