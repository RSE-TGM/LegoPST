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
static char SccsID[] = "@(#)shr_lock.c	5.2\t3/14/96";
/*
   modulo shr_lock.c
   tipo 
   release 5.2
   data 3/14/96
   reserved @(#)shr_lock.c	5.2
*/
# include <errno.h>
# include <stdio.h>
#if defined UNIX
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#endif
# include "sim_param.h"
# include "sim_types.h"
# include "sim_ipc.h"
# include "comandi.h"
#include <Rt/RtMemory.h>


int id_sem_shr;			/* identificatore semaforo shared
				   memory			*/ 
int id_sem_mdb_1,id_sem_mdb_2,
    id_sem_mdb_3;  		/* identificatori semafori mdb */ 



int aggancia_sem_shr();
int sgancia_sem_shr();
void blocca_shr();
int libera_shr();
int imp_mdb();
int attende_mdb();
int libera_mdb();



int aggancia_sem_shr()
{
int shr_usr_key;                   /* chiave utente per shared  */
int ret;

shr_usr_key = atoi((char *)getenv("SHR_USR_KEY"));

id_sem_shr=sem_create(shr_usr_key+ID_SEM_SHR,1);
id_sem_mdb_1=sem_create(shr_usr_key+ID_SEM_MDB_1,0);
id_sem_mdb_2=sem_create(shr_usr_key+ID_SEM_MDB_2,0);
id_sem_mdb_3=sem_create(shr_usr_key+ID_SEM_MDB_3,0);

}

int sgancia_sem_shr()
{

sem_close(id_sem_shr);
sem_close(id_sem_mdb_1);
sem_close(id_sem_mdb_2);
sem_close(id_sem_mdb_3);

}



void blocca_shr()
{
sem_wait(id_sem_shr);
}



int libera_shr()
{
/* libera la shared memory */
sem_signal(id_sem_shr);
}

int imp_mdb()
{
int semval;
/* verifica se e' agganciato il mandb */
/*
if((num_sem_agg(id_sem_mdb_1)>1)&&(num_sem_agg(id_sem_mdb_2)>1))
*/
#if defined UNIX
if(semctl(id_sem_mdb_3,0,GETVAL,0)==1)
	{
	printf("SKED -> mandb attivo\n");
	sem_signal(id_sem_mdb_1);
	sem_wait(id_sem_mdb_2);
	sem_set(id_sem_mdb_2,0);
	sem_set(id_sem_mdb_1,0);
	}
#endif
}

int attende_mdb()
{
sem_wait(id_sem_mdb_1);
}

int libera_mdb()
{
sem_signal(id_sem_mdb_2);
}
