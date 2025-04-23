/**********************************************************************
*
*       C Source:               SD_saveic.c
*       Subsystem:              3
*       Description:
*       %created_by:    mauric %
*       %date_created:  Wed Mar 18 16:11:54 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: SD_saveic.c-2 %  (%full_filespec: SD_saveic.c-2:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)SD_saveic.c	5.1\t11/7/95";
/*
   modulo SD_saveic.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)SD_saveic.c	5.1
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "libdispatcher.h"


int     SD_saveic (processo, posizione, descrizione, dati)
int     processo;
int    *posizione;
char   *descrizione;
char   *dati;
{
#if defined MFFR
char    app[SNAP_DESC + 4 + sizeof(SNAP_SKED)];
int k;
char *app_char;
char *app_descr;
#else
char    app[SNAP_DESC + 4];
#endif
int     ret = -1;

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
    ret = to_dispatcher (processo, COMANDO_SAVEIC, app, 
				SNAP_DESC + sizeof (int));
    if(ret<=0)
	return(ret);
    app_char= &app[0];
    app_char+=SNAP_DESC + sizeof (int);
    
    for(k=0;k<(sizeof(SNAP_SKED)/MAX_LUN_COMANDI_DISPATCHER);k++)
	{
    	ret = to_dispatcher (processo, COMANDO_SAVEIC, app_char,
		MAX_LUN_COMANDI_DISPATCHER);
    	if(ret<=0)
		return(ret);
	app_char+=MAX_LUN_COMANDI_DISPATCHER;
	}
	
    k=sizeof(SNAP_SKED)-k*MAX_LUN_COMANDI_DISPATCHER;
    ret = to_dispatcher (processo, COMANDO_SAVEIC, app_char,k);
#else
    ret = to_dispatcher (processo, COMANDO_SAVEIC, app, 
					SNAP_DESC + sizeof (int));
#endif
    return (ret);
}
