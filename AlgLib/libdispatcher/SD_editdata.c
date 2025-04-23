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
   modulo SD_editdata.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)SD_editdata.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)SD_editdata.c	5.1\t11/7/95";
/*
        Fine sezione per SCCS
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
#include "mod_data.h"


int     SD_editdata (processo, modello,blocco,piniz,nrighe)
int     processo;
int modello;
int blocco;
char **piniz;
int *nrighe;
{
int     ret = -1;
int app_int[2];
int comando,tipo,size;
int righe,i,k;
char *app_char;


    app_int[0]=modello;
    app_int[1]=blocco;
    ret = to_dispatcher (processo, COMANDO_EDITDATA, &app_int[0], 2*sizeof (int));
    /*
	riceve il numero delle righe 
    */
    comando=DATI_DISPATCHER;
        from_dispatcher (processo, &comando, &tipo, (char*)&righe,
                &size, !IPC_NOWAIT);
    printf("SD_editdata: righe = %d\n",righe);
    memcpy(nrighe,&righe,sizeof(int));
    printf("SD_editdata: dopo memcpy,righe restituite=%d\n",righe);
    *piniz=(char*)malloc(righe*LUN_RIGA_F14);
    app_char=(char*)malloc(righe*LUN_RIGA_F14);
    printf("SD_editdata: dopo malloc\n");
    /*
	riceve i dati
    */
     i=0;
     for(k=0;k<((righe*LUN_RIGA_F14)/MAX_LUN_COMANDI_DISPATCHER);k++)
        {
        comando=DATI_DISPATCHER;
        from_dispatcher (processo, &comando, &tipo, &app_char[i],
                &size, !IPC_NOWAIT);
	printf("SD_editdata letti %d\n",size);
        i += MAX_LUN_COMANDI_DISPATCHER;
        }
     from_dispatcher (processo, &comando, &tipo, &app_char[i],
                &size, !IPC_NOWAIT);
	printf("SD_editdata letti %d\n",size);
     memcpy(*piniz,&app_char[0],(righe*LUN_RIGA_F14));
     free(app_char);
    return(ret);
}
