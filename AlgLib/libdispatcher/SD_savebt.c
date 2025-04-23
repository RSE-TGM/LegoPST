/**********************************************************************
*
*       C Source:               SD_savebt.c
*       Subsystem:              3
*       Description:
*       %created_by:    mauric %
*       %date_created:  Wed Mar 18 17:26:42 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: SD_savebt.c-2 %  (%full_filespec: SD_savebt.c-2:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)SD_savebt.c	5.1\t11/7/95";
/*
   modulo SD_savebt.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)SD_savebt.c	5.1
*/
#if defined BACKTRACK
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "libdispatcher.h"


int     SD_savebt (processo, posizione, descrizione, dati)
int     processo;
int    *posizione;
char   *descrizione;
char   *dati;
/*
SNAP_SKED *dati;
char *dati_char;
*/
{
#if defined MFFR
char    app[SNAP_DESC + 4 + sizeof(SNAP_SKED)];
int k;
char *app_char;
char *app_descr;
#else
char    app[SNAP_DESC + 4];
#endif
int  i;
int     ret = -1;

/*
printf("SNAP_SKED_SIZE = %d\n",sizeof(SNAP_SKED));
    dati_char = (char *)dati;
    for(i=0; i<sizeof(SNAP_SKED); i++)
       dati_char[i] = '\0';
printf("i = %d\n",i);
*/

    for(i=0; i<sizeof(SNAP_SKED); i++)
       dati[i] = '0';

    memcpy (&app[0], posizione, sizeof (int));
    if (descrizione != NULL) {
        app_descr = (char *)calloc(SNAP_DESC, sizeof(char));
        strcpy(app_descr,descrizione);
	memcpy (&app[4], app_descr, SNAP_DESC);
        free(app_descr);
    }
#if defined MFFR
    if (dati != NULL)
	memcpy (&app[4 + SNAP_DESC], dati, sizeof(SNAP_SKED));
    ret = to_dispatcher (processo, COMANDO_SAVEBT, app, 
				SNAP_DESC + sizeof (int));
    if(ret<=0)
         return(ret);
    app_char= &app[0];
    app_char+=SNAP_DESC + sizeof (int);
    
    for(k=0;k<(sizeof(SNAP_SKED)/MAX_LUN_COMANDI_DISPATCHER);k++)
	{
    	ret = to_dispatcher (processo, COMANDO_SAVEBT, app_char,
		MAX_LUN_COMANDI_DISPATCHER);
	app_char+=MAX_LUN_COMANDI_DISPATCHER;
	}
	
    k=sizeof(SNAP_SKED)-k*MAX_LUN_COMANDI_DISPATCHER;
    ret = to_dispatcher (processo, COMANDO_SAVEBT, app_char,k);
#else
    ret = to_dispatcher (processo, COMANDO_SAVEBT, app, 
					SNAP_DESC + sizeof (int));
#endif
    return (ret);
}
#endif
