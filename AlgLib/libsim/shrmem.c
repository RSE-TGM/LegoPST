/**********************************************************************
*
*       C Source:               shrmem.c
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Mon Feb 28 18:07:58 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: shrmem.c-9 %  (%full_filespec: shrmem.c-9:csrc:3 %)";
#endif
/*
   modulo shrmem.c
   tipo 
   release 5.2
   data 3/13/96
   reserved @(#)shrmem.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)shrmem.c	5.2\t3/13/96";
/*
        Fine sezione per SCCS
*/
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <math.h>
# include <errno.h>
# include <stdio.h>
#include <unistd.h>
#include <Rt/RtMemory.h>
#include <string.h>

/* ****** Funzione modificata da Fabio 6/2/97  ***** *
Funzione che crea una shrm (se non esiste) di dimensioni size + sizeof(int).
Nella word iniziale della shm, viene inserito il valore di size.
Se la shm con chiave key esiste gia', controlla che le dimensioni (scritte nella prima word della shm) siano = al valore size. 

Se si verifica un errore viene restituito NULL

 ***************************************************************** */

char *crea_shrmem();
void distruggi_shrmem(); /* per comaptibilita' col vecchio codice */
void elimina_shrmem();   /* nuova chiamata per l'eliminazione della shared
				memory */
int sgancia_shrmem();



/* ------------------------------------------------------------------------
   Diagnostica dei fallimenti sulla shared memory.

   Storicamente qui si stampava solo "ERRORE:shmget-EINVAL" seguito da
   "impossibile agganciarsi a shm gia' es. che ha dim inf": nessuna
   indicazione di quale chiave, quale dimensione, chi occupa il segmento.
   Peggio, il NULL restituito non veniva controllato da alcuni chiamanti
   (p.es. costruisci_var), che lo dereferenziavano subito: il sintomo che
   arrivava all'utente era un SIGSEGV senza spiegazione.

   Qui si stampa quel che serve a capire, incluso lo stato del segmento
   gia' presente alla stessa chiave, che e' la causa tipica: la topologia
   in memoria appartiene a un altro modello.
   ------------------------------------------------------------------------ */
static void diagnostica_shm(const char *fase, int key, int size, int errsv)
{
    int idesist;
    struct shmid_ds info;

    fflush(stdout);
    fprintf(stderr, "\n=========== ERRORE SHARED MEMORY (%s) ===========\n", fase);
    fprintf(stderr, "  chiave richiesta ...: %d (0x%x)\n", key, (unsigned)key);
    fprintf(stderr, "  dimensione richiesta: %d byte\n", size);
    if (errsv)
        fprintf(stderr, "  errore di sistema ..: %s (errno=%d)\n", strerror(errsv), errsv);

    /* shmget con size 0 e senza IPC_CREAT: interroga senza creare nulla */
    idesist = shmget(key, 0, 0);
    if (idesist >= 0 && shmctl(idesist, IPC_STAT, &info) == 0)
    {
        fprintf(stderr, "  ALLA STESSA CHIAVE ESISTE GIA' UN SEGMENTO:\n");
        fprintf(stderr, "    shmid %d - dimensione %d byte - agganciati %d processi - creato dal pid %d\n",
                idesist, (int) info.shm_segsz, (int) info.shm_nattch, (int) info.shm_cpid);
        if ((int) info.shm_segsz < size)
            fprintf(stderr, "    e' PIU' PICCOLO di quanto serve: quasi certamente contiene la\n"
                            "    topologia di un ALTRO modello o di una sessione precedente.\n");
        else if ((int) info.shm_segsz > size)
            fprintf(stderr, "    e' PIU' GRANDE di quanto serve: contiene un altro modello.\n");
    }
    else
    {
        fprintf(stderr, "  nessun segmento presente a quella chiave.\n");
    }

    fprintf(stderr, "  COSA FARE: verificare con 'ipcs -m' chi occupa la chiave e chiudere la\n");
    fprintf(stderr, "             sessione che la tiene; in alternativa 'killsim', che pero' su\n");
    fprintf(stderr, "             Linux cancella TUTTE le SHM dell'utente (nessun filtro per\n");
    fprintf(stderr, "             chiave): non usarlo con altre sessioni o GUI aperte.\n");
    fprintf(stderr, "================================================================\n\n");
    fflush(stderr);
}

char *crea_shrmem(key,size,shmid)
int key;
int size;
int *shmid;            /* identificativo shm solo per ULTRIX e AIX */
{
  char *ind;                            /* variabile spare           */
  int *appo;
  /* ** Creazione della memoria condivisa ************************** */
/*
Controllo se la shmem esiste gia'
*/

  *shmid   = shmget(key, size+sizeof(int), 0777 | IPC_CREAT | IPC_EXCL);
  if((*shmid) < 0) /* shm  esiste gia'*/
        {
        *shmid   = shmget(key, size+sizeof(int), 0777 | IPC_CREAT );
        if((*shmid) <0)
                {

                diagnostica_shm("aggancio a segmento esistente", key,
                                size + (int) sizeof(int), errno);
                return(NULL); 
		}
        else
                {
                ind = shmat(*shmid, 0, ! ( SHM_RND & SHM_RDONLY ));
                        if(((int)ind) == -1)
                                {
				diagnostica_shm("shmat sul segmento esistente", key,
                                                size + (int) sizeof(int), errno);
                                return(NULL);
                                }
/*
Modifica dovuta alla parte di integrazione Scada 
*/
		appo=(int *)ind;
                if(!(*appo==size))
                        {
                        fflush(stdout);
                        fprintf(stderr,
                                "ERRORE: il segmento alla chiave %d (shmid %d) e' stato creato per\n"
                                "        %d byte, ma ne servono %d: contiene un ALTRO modello.\n",
                                key, *shmid, *appo, size);
                        diagnostica_shm("dimensione registrata incompatibile", key,
                                        size + (int) sizeof(int), 0);
                        return(NULL);
                        }
                else
                        {
                        return(ind+sizeof(int));
                        }
                }

        } /* end shm esiste gia' */
else
        {
        ind = shmat(*shmid, 0, ! ( SHM_RND & SHM_RDONLY ));
        if(((int)ind) == -1)
                {
                diagnostica_shm("shmat sul segmento appena creato", key,
                                size + (int) sizeof(int), errno);
                return (NULL);
                }
/*
Inserisci nella prima parte della shm le dimensioni della shm stessa (cioe' size
)
*/
        memcpy(ind,&size,sizeof(int));
        return(ind+sizeof(int));
        }
}

void distruggi_shrmem(shmid)
int shmid;   
{
struct shmid_ds buf;

    if(shmctl(shmid,IPC_STAT,&buf)<0)
        printf("shmctl: impossibile cancellare %d\n",shmid);
    else if(buf.shm_nattch<=1)
	{
        if(shmctl(shmid,IPC_RMID,&buf)<0)
          printf("shmctl: impossibile cancellare %d\n",shmid);
	}
    else
	printf("shmctl: impossibile cancellare  %d n_attac=%d\n",shmid,buf.shm_nattch);
}

int sgancia_shrmem(char *addr)
{
if(shmdt(addr-(sizeof(int)))!=0)
	{
        perror("shmdt");
	return( -1);
	}
return(0);
}


void elimina_shrmem(shmid,inizio,size)
int shmid;   
char *inizio;
int size;
{
struct shmid_ds buf;

    if(shmctl(shmid,IPC_STAT,&buf)<0)
        printf("shmctl: impossibile cancellare %d\n",shmid);
    else if(buf.shm_nattch<=1)
	{
        if(shmctl(shmid,IPC_RMID,&buf)<0)
          printf("shmctl: impossibile cancellare %d\n",shmid);
	}
   else 
	printf("shmctl: impossibile cancellare  %d n_attac=%d\n",shmid,buf.shm_nattch);
}
