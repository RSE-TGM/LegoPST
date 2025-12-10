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

/* **************************************************************** *
 * *** ROUTINES PER L'INIZIALIZZAZIONE DELLE TAVOLE DEL VAPORE  *** *
 * **************************************************************** *
 --- Modificata da L.Castiglioni il 14/1/93 ----
 ism01.c
 provvede all'aggancio e caricamento memoria contenente le tavole del vapore
 Nel caso ULTRIX AIX e VMS:
        - se non esiste viene creata la memoria shared per le tavole e
          vengono caricati i valori numerici specificati nel file
          TAVOLE.DAT
        - se esiste viene soltanto effettuato l'aggancio alla memoria.
*/
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <math.h>
#include <stdlib.h>

# define NVAL 129690                /* dimensione tavole in float   */

void read_tav();

   /* ************  DEFINIZIONE VARIABILI GLOBALI ***************** */

int shmtid;                       /* identificativo shm tavole     */
extern float (*valda)[];                  /* dati numerici TAVOLE         */

   /* ************************************************************ */

 
void ism01_(int *iret)
{
    int shm_size;                  /* ampiezza area memoria shared  */
    int shmflg;                    /* flag per attach alla shm      */
    char *ind;                     /* indirizzo generico per shm    */
    key_t shr_tav_key;             /* key area memoria shared       */
    int shmget(key_t,  size_t,  int);
   /* *********** Creazione memoria condivisa ********************* */

    *iret = 0;
    shm_size = sizeof(float)*NVAL;
    shr_tav_key  = atoi((char *)getenv("SHR_TAV_KEY"));
/*
 esamina se la memoria shared e' gia' esistente
*/
    shmtid   = shmget(shr_tav_key, shm_size, 0);
    if( shmtid != -1 )
	{
          *iret = 97; /* indica che e' gia' esistente */
	}
	else
	{
          shmtid   = shmget(shr_tav_key, shm_size, 0777 | IPC_CREAT);
          if( shmtid == -1 ) 
	  {
             *iret = 98;
             perror("ERRORE IN ISM01.C");
 	  }
          /* *********** Collegamento memoria condivisa ****************** */

           shmflg    = ! ( SHM_RND & SHM_RDONLY );
           ind       = (char*) shmat(shmtid, 0, shmflg);
           valda     = (float (*)[]) ind;
           read_tav();
           *iret = 99;
	   if( shmdt(ind)  == -1 )
          {
            perror("ERRORE IN ISM01.C");
          }  
        }

   /* *********** Collegamento memoria condivisa ****************** */

    shmflg    = ! ( SHM_RND );
    ind       = (char*) shmat(shmtid, 0, shmflg);
    valda     = (float (*)[]) ind;
}
/* FINE DELLA ism01 */


