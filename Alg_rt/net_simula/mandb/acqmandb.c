/**********************************************************************
*
*       C Source:               acqmandb.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Mar 31 10:05:47 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: acqmandb.c-4 %  (%full_filespec: acqmandb.c-4:csrc:1 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)acqmandb.c	5.1\t11/7/95";
/*
   modulo acqmandb.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)acqmandb.c	5.1
*/
/*
        utility mandb
*/

#include <stdio.h>                              /* For printf and so on. */
#include <string.h>                              /* For printf and so on. */
#include <errno.h>

#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>

#if defined UNIX
                                   
#include <sys/types.h>   /* include per la lettura della coda di messaggi */
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <signal.h>

#elif defined VMS
# include "vmsipc.h"

#else
 exit(puts("ATTENZIONE: compilare con -DAIX (o -DVMS / -DULTRIX)"));
#endif

#include "sim_param.h"
#include "sim_types.h"
#include "sim_ipc.h"
#include "dispatcher.h"
#include "mandb.h"
#include <Rt/RtDbPunti.h>
#include <Rt/RtErrore.h>
# include <Rt/RtMemory.h>
#include <sqlite3.h>


float estr_sh();
void testata(char *, char *);
void costruisci_var (char**, VARIABILI **, int*);
extern int aggancia_sem_shr();
int write_sh(int,float);

int id_msg_to_mandb, id_msg_from_mandb;

RtDbPuntiOggetto dbpunti;
RtErroreOggetto errore;

/*
        Variabili per l'area shared delle variabili */

char *ind_sh_top;
int id_sh;
VARIABILI *variabili;
int tot_variabili;
int mod,nbloc;
int id_sem_mdb_3;


/*
        dati per l' area shared privata di mandb
*/

DBASE_MANDB *p_db;
int     shmvid_mandb;                  /* identificativo shm               */

int msgqid;                      /* identificativo coda di messaggi  */

MSG msg_rx;              /* buffer di scodamento messaggi */


int idsem_locale;

sqlite3 *db;

#if defined UNIX
void fine_acqmandb();
#endif

unsigned int main(argc, argv)
    unsigned int argc;   /* Command line argument count */
    char *argv[];        /* Pointers to command line args. */
{
char *ind;
int shr_usr_key;      /* chiave utente per shared  */
int i,size,j,old_pointin;
float valore,tempo;
MOD_VALORI *p_mod;
RESET_HISTORY *p_reset;
MSG_DISPATCHER messaggio;
SIMULATOR *simpar;


/* redirezione output  e stampa versione */
testata("acqmandb",SccsID);

/*
	si alloca l'area shared del simulatore
*/

printf("\n ATTIVATO ");
   shr_usr_key = atoi((char *)getenv("SHR_USR_KEY"));
   p_db = (DBASE_MANDB *) crea_shrmem(shr_usr_key+ID_SHM_MDB,sizeof(DBASE_MANDB),&shmvid_mandb);
   memset ((char *) p_db,0,sizeof (DBASE_MANDB));

/* si alloca  l'area shared delle variabili */

   costruisci_var(&ind_sh_top,&variabili,&id_sh);
   mod=numero_modelli(ind_sh_top);
   tot_variabili=numero_variabili(ind_sh_top);

/* si alloca  l'area shared del simulatore */

   simpar  = (SIMULATOR*)calloc(1,sizeof(SIMULATOR));
   errore  = RtCreateErrore(RT_ERRORE_TERMINALE,"acqmandb");
   dbpunti = RtCreateDbPunti(errore,NULL,DB_PUNTI_INT,simpar);
   if(dbpunti == NULL)
	exit(printf("acqmandb: simulazione non attiva\n"));
   free(simpar);

/*
	accede al semaforo locale tra i processi mandb e acqmandb 
*/

      idsem_locale = sem_create(shr_usr_key+ID_SEM_MDB_4,1); 
      sem_set(idsem_locale,1);

	if (idsem_locale == -1 ) exit(puts("errore creazione semaforo"));

/* attiva la routine di risposta all' interupt di kill */

#if defined UNIX
    signal(SIGUSR1,fine_acqmandb);
#endif

/*
	accede alla coda dei messaggi 
*/
    msgqid = msg_create(shr_usr_key+ID_MSG_MANDB,1);

/* si aggancia ai semafori e si sospende sul semaforo per il MANDB */

    aggancia_sem_shr();

/*
	aspetta il messaggio di attivazione 
*/
    if (msg_rcv( msgqid, &msg_rx, sizeof(msg_rx.mtext), (long)MSG_START, !IPC_NOWAIT,0 ) == -1)
	perror("errore scodamento");
    printf("start ricevuto %d \n",msg_rx.mtype);

/* avvisa lo schedulatore */
id_msg_to_mandb=msg_create(shr_usr_key+ID_MSG_TO_MANDB,0);
id_msg_from_mandb=msg_create(shr_usr_key+ID_MSG_FROM_MANDB,0);
messaggio.mtype=SKDIS_AGGANCIO_MANDB;
printf("Invio messaggio allo schedulatore\n");
msg_snd(id_msg_from_mandb,&messaggio,sizeof(MSG_DISPATCHER) - sizeof (long),
	!IPC_NOWAIT);
printf("Attendo ack dallo schedulatore\n");
msg_rcv(id_msg_to_mandb,&messaggio,sizeof(MSG_DISPATCHER) - sizeof (long),(long)1000,
	!IPC_NOWAIT,TIME_ACK);
printf("Ricevuto ack dallo sked\n");

for (;;)
    {
    msg_rcv(id_msg_to_mandb,&messaggio,sizeof(MSG_DISPATCHER) - sizeof (long),
		(long)SKDIS_MANDB_SIGNAL,!IPC_NOWAIT,TIMEOUT_INF);
    sem_wait(idsem_locale);

/*  raccoglie i dati dal data base del simulatore e li mette
    nell' area shared del mandb */

    for (i=0; i< MAX_TOTVAR; i++)
    {
        if (p_db->indice_var[i] == -1) continue;
        valore  = estr_sh(variabili[p_db->indice_var[i]].addr,0);
        p_db->valori[i].valore[p_db->pointin[i]] =valore;
        RtDbPGetTimeTask(dbpunti,variabili[p_db->indice_var[i]].mod-1,&tempo);
        p_db->valori[i].tempo[p_db->pointin[i]]  =tempo;

        if ( p_db->pointin[i] == MAX_CAMPIONI - 1) p_db->pointin[i]=0;
        else               p_db->pointin[i]++;
    }

/* scoda gli eventuali messaggi */

    while  (msg_rcv( msgqid, &msg_rx, sizeof(msg_rx.mtext), (long)0,IPC_NOWAIT,0) != (-1))
    {
printf("\n arrivato messaggio %d",msg_rx.mtype);
	switch (msg_rx.mtype)
	{

		case MSG_MOD_VALORI:
		p_mod = (MOD_VALORI *) &msg_rx.mtext[0];
		printf("Ind=%d valore %f\n",p_mod->indice_var,p_mod->valore);
		write_sh(p_mod->indice_var,p_mod->valore);
		break;

/*************************  non viene piu' inviato 
		case MSG_RESET_HISTORY:

		p_reset = (RESET_HISTORY *) &msg_rx.mtext[0];
		p_db->pointin[p_reset->indice_var]=0;
		for (j=0; j< MAX_CAMPIONI; j++)
		{
                  p_db->valori[p_reset->indice_var].valore[j]=0.;
	       	   p_db->valori[p_reset->indice_var].tempo[j]=0.;
		}
		break;
*************************/

	}
    }

    sem_signal(idsem_locale);
    messaggio.mtype=SKDIS_MANDB_SIGNAL_ACK;
    msg_snd(id_msg_from_mandb,&messaggio,sizeof(MSG_DISPATCHER)-sizeof(long),
		!IPC_NOWAIT);
    }
}

float estr_sh(indice,funct)
int indice;
int funct;  /* se >0 converte in logical  */
{

float val; /* puntatore di appoggio */

val= RtDbPGetValueD(dbpunti,indice);

return(val);
}

/* Stile prototipo, NON K&R: il prototipo (riga 67) dichiara (int,float), il K&R
   promuoveva val a double -> il valore scritto in SHM era corrotto.
   Vedi docs/KR_PROTOTYPE_AUDIT.md. */
int write_sh(int indice, float val)
{
RtDbPPutValue(dbpunti,indice,val);
printf ("\n variabile indice %d valore : %f \n",indice,val);  
return(0);
}

void fine_acqmandb()
{
MSG_DISPATCHER messaggio;

	printf("\n  fine acq_mandb ");
	messaggio.mtype=SKDIS_SGANCIO_MANDB;
	msg_snd(id_msg_from_mandb,&messaggio,
		sizeof(MSG_DISPATCHER)- sizeof (long),!IPC_NOWAIT);
	msg_rcv(id_msg_to_mandb,&messaggio,
			sizeof(MSG_DISPATCHER) - sizeof (long),(long)1000,
		!IPC_NOWAIT,TIME_ACK);
	sem_close(idsem_locale);
        distruggi_var(id_sh);
        distruggi_shrmem(shmvid_mandb);
        exit(0);
}
