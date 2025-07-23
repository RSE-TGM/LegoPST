/**********************************************************************
*
*	C Source:		client_scada.c
*	Subsystem:		4
*	Description:	
*	%created_by:	lomgr %
*	%date_created:	Mon Oct 24 15:38:09 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: client_scada.c-10 %  (%full_filespec: client_scada.c-10:csrc:4 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)client_scada.c	1.11\t3/20/95";
/*
        Fine sezione per SCCS
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <libnet.h>
#include <sim_param.h>
#include <Cs/Cs.h>

#include <client_scada.h>
#include <client_s.h>
#include <sqlite3.h>
#include "pthread.h"



//#include "client_s.h"

ZONE zone[MAX_ZONE];

int numero_mmi;
/**
Variabile per lock miniASD.
Setto tale var a un val !=0 se ho eseguito una richiesta miniASD.
Setto tale var a 0 solo quando ho esaudito la richiesta.
**/
LOCK_MINIASD lock_miniASD;

int fp; /* canale comunicaz. */
pthread_mutex_t fp_mutex;
pthread_mutex_t canale_mutex;

pthread_t threads[3];

/*
        variabili per la gestione degli allarmi
*/
int id_shm_allarmi;
int shmvid_allarmi;
char *punt_shm_allarmi;
int sem_scada,sem_scada_key; /*variabili globali associate a semforo per 
                                  sincronizzazione miniASD */
int sem_mmi,sem_mmi_key; /*variabili globali associate a semforo per 
                                  sincronizzazione miniASD */
sqlite3 *db;

int   socketclient (char *, int);


int main(argc,argv)
int argc;
char **argv;
{
int worker_num;
int status;
int exit_value;


if(argc<3)
        errore("use: client_scada  <hostname> <codice mmi>");

printf("Process client_scada started [%s] [%s]\n\n", argv[1],argv[2]);

numero_mmi=atoi(argv[2]);

if(numero_mmi<=0)
	errore("il codice mmi deve essere maggiore di 0");
if(numero_mmi>4)
	errore("il codice mmi deve essere minore o uguale a 4");

/*
	apre la connessione con lo scada
*/
fp=socketclient(argv[1],SCADA_PORT+ numero_mmi - 1);
if(fp<=0)
	errore("Impossibile aprire la connessione");
printf("Creata connessione fp = %d\n",fp);

/*
        inizializza fp_mutex
*/
//if(pthread_mutex_init(&fp_mutex,pthread_mutexattr_default)!=0)
if(pthread_mutex_init(&fp_mutex,NULL)!=0)
        exit(printf("Errore creazione fp_mutex\n"));
/*
        inizializza canale_mutex
*/
//if(pthread_mutex_init(&canale_mutex,pthread_mutexattr_default)!=0)
if(pthread_mutex_init(&canale_mutex,NULL)!=0)
        exit(printf("Errore creazione canale_mutex\n"));

/*
	crea la shared memory per i dati degli allarmi
*/
if(!getenv("SHR_USR_KEYS"))
	{
	printf("client_scada: SHR_USR_KEYS non settata !!!\n");
	printf("client_scada: Exiting client_scada !!!\n");
	exit(0);
	}
id_shm_allarmi=atoi(getenv("SHR_USR_KEYS"))+OFFSET_SHM_ALLARMI;
punt_shm_allarmi=(char*)crea_shrmem(id_shm_allarmi,sizeof(DATI_ALLARMI_SHM),
                                           &shmvid_allarmi);
if(punt_shm_allarmi==NULL)
	{
	printf("client_scada:impossibile creare SHMS per allarmi\n");
	exit(1);
	}
memset(punt_shm_allarmi,0,sizeof(DATI_ALLARMI_SHM));

/*
Creazione semaforo per sincronizzazione con miniASD
*/
sem_scada_key=atoi(getenv("SHR_USR_KEYS"))+OFFSET_SEM_SCADA;
sem_scada=sem_create(sem_scada_key,1);
sem_mmi_key=atoi(getenv("SHR_USR_KEYS"))+OFFSET_SEM_MMI;
sem_mmi=sem_create(sem_mmi_key,0);
printf("DEBUG CLIENT_SCADA: aperto semaforo mmi id =%d\n",sem_mmi);

libera_tutte_le_zone();

// worker_num = 0;
// status= pthread_create (&threads[worker_num],
//                         pthread_attr_default,
//                         main_client_scada_command,
//                         (void *)(intptr_t) worker_num); // Cast corretto
// //                        (pthread_addr_t) worker_num);
// check(status,"pthread_create main_client_scada_command bad status\n");

// worker_num = 1;
// status= pthread_create (&threads[worker_num],
//                         pthread_attr_default,
//                         main_client_scada_data,
//                         (void *)(intptr_t) worker_num); // Cast corretto
// //                        (pthread_addr_t) worker_num);
// check(status,"pthread_create main_client_scada_data bad status\n");

    // Crea i thread con attributi di default (NULL)
    worker_num = 0;
    status= pthread_create(&threads[worker_num],
                            NULL,
                            (void *(*)(void *))main_client_scada_command,
                            (void *)(intptr_t)worker_num);
    check(status,"pthread_create main_client_scada_command bad status\n");

    worker_num = 1;
    status= pthread_create(&threads[worker_num],
                            NULL,
                            (void *(*)(void *))main_client_scada_data,
                            (void *)(intptr_t)worker_num);
    check(status,"pthread_create main_client_scada_data bad status\n");


/*
	attende il termine dei threads
*/
worker_num=0;
printf("DEBUG CLIENT_SCADA: prima di pthread_join \n");
status= pthread_join( threads[worker_num],(void**)&exit_value);
printf("DEBUG CLIENT_SCADA: dopo  pthread_join \n");
check(status,"pthread join bad status\n");
if (exit_value == worker_num)
        printf("Thread terminato correttamente\n");

}
