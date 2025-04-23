/**********************************************************************
*
*       C Source:               SD_editpert.c
*       Subsystem:              3
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed Jan 22 11:35:50 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: SD_editpert.c-4 %  (%full_filespec: SD_editpert.c-4:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)SD_editpert.c	5.1\t11/7/95";
/*
   modulo SD_editpert.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)SD_editpert.c	5.1
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


int SD_editpert (int processo, TIPO_PERT *perturbazioni, int modo, 
             int num_pert_active)
{
int       i, k, comando, size, tipo;
TIPO_PERT app_pert[NELEM_PACCHETTO];
int       ret = -1;
int       pert_ricevute=0;
int       num_ric;

/* richiesta tabella perturbazioni */
    ret = to_dispatcher (processo, COMANDO_EDITPERT, NULL, 0);
    for (i = 0; i < num_pert_active*2 ; i+=NELEM_PACCHETTO )
        {
        comando = DATI_DISPATCHER;
        if( (pert_ricevute+NELEM_PACCHETTO) > 2*num_pert_active )
            {
            num_ric       = 2*num_pert_active - pert_ricevute;
            pert_ricevute = 2*num_pert_active;
            }
        else
            {
            num_ric        = NELEM_PACCHETTO;
            pert_ricevute += NELEM_PACCHETTO;
            }

        from_dispatcher (processo, &comando, &tipo, (char*)&app_pert[0], 
                         &size, !IPC_NOWAIT);
        for(k=0;k<num_ric;k++)
           {
	   memcpy (&perturbazioni[i+k], &app_pert[k],
		sizeof (TIPO_PERT));
/*
           printf("perturbazioni[i+k=%d].tipo=%d rateo=%d\n",
                   i+k,perturbazioni[i+k].tipo,
                   perturbazioni[i+k].rateo_durata);
*/
           }
        }

return (ret);
}
