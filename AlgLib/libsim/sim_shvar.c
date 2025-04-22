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
static char SccsID[] = "@(#)sim_shvar.c	5.4\t11/10/95";
/*
   modulo sim_shvar.c
   tipo 
   release 5.4
   data 11/10/95
   reserved @(#)sim_shvar.c	5.4
*/
# include <math.h>
# include <errno.h>
# include <stdio.h>
# include "sim_param.h"      /* paramteri generali LEGO              */
# include "sim_types.h"      /* tipi di variabili LEGO               */ 
# include "sim_ipc.h"      /* parametri per semafori               */
# include "comandi.h"
#include <Rt/RtMemory.h>

int     shmvar;                  /* identificativo shm               */

char *crea_shrmem(int,int,int *);


char *sim_shvar(shr_usr_key,size)
 int shr_usr_key;                     /* chiave utente per shared  */
 int size;

 {
  char *ind;                            /* variabile spare           */


printf("Creazione/aggancio shared memory database topologia simulatore\n          (proc_id=%d sh_id=%d size=%d)\n",
	getpid(),shr_usr_key+ID_SHM_VAR,size);

  ind = (char*) crea_shrmem(shr_usr_key+ID_SHM_VAR,size,&shmvar);

return(ind);

 }

void sim_shvar_free()   
{
distruggi_shrmem(shmvar);
}
