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
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <math.h>
# include <errno.h>
# include <stdio.h>
//#include <sys/types.h>
#include <unistd.h>
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

/*
   Dice se la shared memory della topologia va (ri)caricata dal file.

   Chi chiama sim_shvar() guarda PRIMA, con shresist(), se il segmento c'e'
   gia', e in quel caso non carica variabili.rtf: da' per buono quel che
   trova. Non basta. Il segmento puo' essere il residuo di una sessione
   finita - o quello che crea_shrmem() ha appena rifatto, vuoto, al posto di
   un orfano di dimensione sbagliata - e allora il contenuto e' vecchio o
   nullo: compstaz rispondeva "IL MODELLO ... NON ESISTE" per ogni riga.

   Il criterio e' chi lo sta usando: se siamo gli unici agganciati non c'e'
   una simulazione viva a cui allinearsi, e la verita' e' il file. Ritorna 1
   in quel caso, 0 se qualcun altro lo tiene (e il contenuto e' il suo).
*/
int sim_shvar_da_caricare()
{
struct shmid_ds buf;

if(shmctl(shmvar,IPC_STAT,&buf)<0)
	return(0);
return(buf.shm_nattch<=1);
}

void sim_shvar_free()   
{
distruggi_shrmem(shmvar);
}
