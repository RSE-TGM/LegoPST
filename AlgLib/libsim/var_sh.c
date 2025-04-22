/**********************************************************************
*
*       C Source:               var_sh.c
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue May 30 11:25:46 2006 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: var_sh.c-13 %  (%full_filespec: var_sh.c-13:csrc:3 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)var_sh.c	5.3\t3/27/96";
/*
   modulo var_sh.c
   tipo 
   release 5.3
   data 3/27/96
   reserved @(#)var_sh.c	5.3
*/
# include <stdio.h>
# include <math.h>
# include <errno.h>
# include <string.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <sys/shm.h>
# include <sys/stat.h>
#endif
#if defined VMS 
# include <stat.h>
# include "vmsipc.h"
#endif
#if defined HELIOS
# include <sys/types.h>
# include <sys/stat.h>
# include "vmsipc.h"
#endif
# include "sim_param.h"
# include "sim_types.h"
# include "sim_ipc.h"
#include <Rt/RtMemory.h>
#include <sqlite3.h>

struct variabili_rid_st {
                char nome[MAX_LUN_NOME_VAR];
                char  tipo;
               } ;
typedef struct variabili_rid_st VARIABILI_RID ;



/* restituisce il numero di modelli con in ingressi il puntatore
   all'inizio della shared memory				*/
int     numero_modelli (char *);

/* restituisce il numero dei blocchi di un determinato modello
   con in ingressi il puntatore all'inizio della shared memory
   e il numero del modello (1..MAX_MODEL)			*/
int     numero_blocchi (char *, int);

/* restituisce il numero delle variabili con in ingressi il puntatore
   all'inizio della shared memory                               */
int     numero_variabili (char *);

/* restituisce il nome di un modello con in ingressi il puntatore
   all'inizio della shared memory e il numero del modello	*/
char   *nome_modello (char *, int);

/* restituisce il numero del modello con in ingressi il puntatore
   all'inizio della shared memory e l'indirizzo della variabile */
int   ind_modello (char *, int, VARIABILI *);

/* restituisce il nome di un blocco con in ingressi il puntatore
   all'inizio della shared memory e il numero del modello e del
   blocco selezionato						*/
char   *nome_blocco (char *, int, int);

/* costruisce o si aggancia alla shared memory contenete i nomi dei
   modelli, dei blocchi e l'elenco delle variabili con i relativi 
   indirizzi; restituisce il puntatore all'inizio della shared memory,
   il puntatore all'inizio della zona delle variabili e l'identificatore
   della shared memory						*/
void costruisci_var (char **, VARIABILI **, int *);

/* costruisce il file variabili.rtf */
void crea_variabili (char **, VARIABILI **, int *, int);

/* elimina o si "sconnette" dalla shared memory			*/
void distruggi_var (int);

/* legge i file S02, S04, S05 costruendo un elenco di dati di appoggio
   utilizzato dalla procedura costruisci_var			*/
void legge_dati (VARIABILI **, VARIABILI_RID **, NOMI *, int *, int *);

void fill_space ( char *str, int lun);

extern sqlite3 *db;

extern S02 s02_;

char *sim_shvar(int,int);

char **cdim2(int,int);
int **idim2(int,int);

static void output_ascii (char*, VARIABILI*);


/************************ INIZIO PROCEDURE ***********************/

/* elimina se e' possibile la shared memory */
void distruggi_var (ind)
int     ind;
{
    sim_shvar_free ();
}


/* ritorna il numero delle variabili */
int     numero_variabili (ind)
char   *ind;
{
int     mod;
int     tot, p;
int     i;
int     ret;
char *car;

/*
Inizio modifica di Fabio del 27/5/96 :
Non viene piu' usato un intenero per contenere l' indirizzo, ma
un puntatore a carattere 
*/

car=ind;
memcpy (&mod, ind, sizeof (int));
car= ind + sizeof (int) + mod * sizeof (NOMI_MODELLI);
   /*
	Vecchia versione con intero
	 p = (int) ind + sizeof (int) + mod * sizeof (NOMI_MODELLI);
    */
    for (i = 0; i < mod; i++)
	{
	car+=sizeof (NOMI_BLOCCHI) * numero_blocchi (ind, i + 1);
/*
Vecchia vers.
*/
/*
	p += sizeof (NOMI_BLOCCHI) * numero_blocchi (ind, i + 1);
*/
	}

memcpy (&ret, car, sizeof (int));
 /*   memcpy (&ret, p, sizeof (int));*/
return (ret);
}

/* ritorna il numero di modelli */

int     numero_modelli (ind)
char   *ind;
{
int     mod;
    memcpy (&mod, ind, sizeof (int));
    return (mod);
}

int   ind_modello (ind, num,var)
char   *ind;
int     num;
VARIABILI *var;
{
int     tot_var,i;

	tot_var = numero_variabili(ind);
	for(i=0;i<tot_var;i++)
		if(var[i].addr == num)
			return(var[i].mod -1);
		
	return(-1);
}

/* ritorna il puntatore al nome del modello richiesto */

char   *nome_modello (ind, num)
char   *ind;
int     num;
{
int     i;
char   *ret;

    ret = ind;
    ret += sizeof (int);
    for (i = 1; i < num; i++)
	ret += sizeof (NOMI_MODELLI);
    return (ret);
}

/* ritorna il numero dei blocchi del modello richiesto */

int     numero_blocchi (ind, num)
char   *ind;
int     num;
{
int     i;
short   ret;
char   *p;
    p = ind;
    p += sizeof (int);
    for (i = 1; i < num; i++)
	p += sizeof (NOMI_MODELLI);
    p += MAX_LUN_NOME_MODELLO;
    memcpy (&ret, p, sizeof (short));
    return ((int) ret);
}
/* ritorna il numero delle variabili di un blocco di un modello */

int     numero_var_bl(p_var,tot_var,nmod,nbloc)
VARIABILI *p_var;
int        tot_var,nmod,nbloc;
{
int     i=0,j,nvar_bl=0;

/* Cerco la zona delle variabili del modello */
    for (i=0;i < tot_var;i++)
       if ( p_var[i].mod==(short)nmod)
          break;
    for (j=i;j < tot_var;j++)
       {
       if ( p_var[j].blocco==(short)nbloc)
          nvar_bl++;
       if ( p_var[j].blocco==(short)(nbloc+1))
          break;
       }
    return (nvar_bl);
}
/* Dato il numero totale di modelli ed un loro nome ne ritorna il numero */
int   numero_modello (ind, ntot, nome)

char *ind;
int  ntot;
char *nome;
{
int     i;
char   *punt;

    punt = ind;
    punt += sizeof (int);
    i = 1;
    while ( strncmp(punt,nome,strlen(nome)) && (i<=ntot)  )
       {
        punt += sizeof (NOMI_MODELLI);
        i++;
       }
    if ( i<= ntot )
       return (i);
    else
       return(0);
}
/* ********************************************** */
/* ritorna il numero del blocco di un modello */

int  numero_blocco(char *ind, int nmod, char *nome)
{
int     i;
char   *ret;
char   *punt;
short   ntbl;
int     init;
int     inizio;
int     nmax;

    nmax = numero_modelli(ind);
    if (nmod>nmax)
       {
       fprintf(stderr,"   ATTENZIONE modello n. %d  sono contenuti soltanto %d modelli\n",nmod,nmax);
       return(0);
       }
    if (nmod<=0)
       {
       fprintf(stderr,"   ATTENZIONE numero del modello <=0 \n");
       return(0);
       }
    ret = ind;
    ret += sizeof (int);
    init = (int) ret;
    for (i = 1; i < nmod; i++)
        ret += sizeof (NOMI_MODELLI);
    punt = ret + MAX_LUN_NOME_MODELLO + sizeof(short);
    memcpy((void *)&inizio,(void *)punt,sizeof(int));
    punt = (char*) (inizio + init);
    ret += MAX_LUN_NOME_MODELLO;
    memcpy (&ntbl, ret, sizeof (short));
    i=1;
    while( (i<=ntbl) && strncmp(punt,nome,strlen(nome)) )
       {
       punt+= sizeof(NOMI_BLOCCHI);
       i++;
       }
    if (i<=ntbl)
      return (i);
    else
      return(0);
}
 
/* ********************************************** */
/* ritorna il puntatore al nome del blocco richiesto  */

char   *nome_blocco (ind, mod, bloc)
char   *ind;
int     mod;
int     bloc;
{
int     i;
char   *ret;
int     offset;
int     init;

    ret = ind;
    ret += sizeof (int);
/*
Vecchia versione
*/
    /*
	init = (int) ret;
    */
    for (i = 1; i < mod; i++)
	ret += sizeof (NOMI_MODELLI);
    ret += (MAX_LUN_NOME_MODELLO + sizeof (short));
    memcpy (&offset, ret, sizeof (int));
/*
Modifica introdotta da Fabio 28/5/96
 */
/* ret = (char *) (offset + sizeof (NOMI_BLOCCHI) * (bloc - 1) + init);*/
ret =ind + sizeof (int) + offset + sizeof (NOMI_BLOCCHI) * (bloc - 1);
return (ret);
}

/* ritorna il numero di dati del blocco richiesto  */

int     numero_dati(ind, mod, bloc)
char   *ind;
int     mod;
int     bloc;
{
int     i;
char   *p;
int     offset;
int     ndati;
/*
Modifica di Fabio del 28/5/96
*/
    p = ind;
    p+= sizeof(int);
    for(i=1;i<mod;i++)
       p+=sizeof(NOMI_MODELLI);
    p+= (MAX_LUN_NOME_MODELLO + sizeof(short));
    memcpy(&offset,p,sizeof(int)); 
    p = ( offset + sizeof(NOMI_BLOCCHI)*(bloc-1) + ind+
                  MAX_LUN_NOME_BLOCCO + sizeof(int) );
/*vecchia versione*/
   /* p = (char*) ( offset + sizeof(NOMI_BLOCCHI)*(bloc-1) + (int)ind+
                  MAX_LUN_NOME_BLOCCO + sizeof(int) );*/
    memcpy(&ndati,p,sizeof(int));
    return(ndati);
}



/* creazione del file varibili.rtf */
void crea_variabili (p_ind, p_var, id, nmod)
char  **p_ind;
VARIABILI ** p_var;
int    *id;
int    nmod;
{
NOMI          * nomi;
NOMI_MODELLI  nomi_modelli;
VARIABILI     * variabili;
int           tot_variabili;
int           tot_ingressi;
int           mod, bloc, i;
int           size_nomi, size_variabili, size;
char          *ind;		/* puntatore inizio shared memory */
char          *punt;
int           offset;
int           shr_usr_key;	/* chiave utente per shared  */
char          stampa_blocco[8]; 
FILE          * fp_var;
struct stat   *buf_var;
int           esiste_sh = 0;

VARIABILI_RID *var_ridotto;
int           size_head;
char          *ind_head;
char          *punt_head;
char          comando_conc[200];
FILE          * fp_varapp_head;

#if defined UNIX
/*
     parametri lego
*/
extern int _N000;
extern int _N001;
extern int _N002;
extern int _N003;
extern int _N004;
extern int _N005;
extern int _N007;
extern int _M001;
extern int _M002;
extern int _M003;
extern int _M004;
extern int _M005;
#endif


    /* alloca la struttura nomi */
    nomi = (NOMI *) malloc (sizeof (NOMI));

#if defined UNIX
    nomi->num_dati_bloc=(int **)idim2(nmod,_N002+1);
    for(i=0;i<nmod;i++) 
	nomi->nom_bloc[i]=(char **)cdim2(_N002,MAX_LUN_NOME_BLOCCO);
#endif
    if (nomi == NULL)
    {
	fprintf (stderr,"Impossibile allocare la struttura nomi\n");
	exit (0);
    }

    /* legge i dati da S02 S04 S05 */
    tot_variabili = 0;
    legge_dati (&variabili, &var_ridotto, nomi, &tot_variabili, &tot_ingressi);

    /* calcolo size_head */
    size_head = sizeof (int) + sizeof (NOMI_MODELLI) * nomi -> nmod;
    for (mod = 0; mod < nomi -> nmod; mod++)
	size_head = size_head + sizeof (NOMI_BLOCCHI) * nomi -> num_blocchi[mod];
    size_head = size_head + sizeof (int);

    /* costruzione shared memory */
    ind = (char *) calloc (1,sizeof(char));
    *p_ind = ind;

    ind_head = (char *) calloc (size_head,sizeof(char));

    /* caricamento shared memory */
    punt_head = ind_head;
    memcpy (punt_head, &(nomi -> nmod), sizeof (int));
    punt_head += sizeof (int);

    offset = sizeof (NOMI_MODELLI) * nomi -> nmod;
    
    /* scrive le struttura NOMI_MODELLI */
    for (mod = 0; mod < nomi -> nmod; mod++)
    {
        nomi_modelli.num_blocchi = (unsigned short) nomi -> num_blocchi[mod];
/*
	memcpy (punt_head, nomi -> name[mod], MAX_LUN_NOME_MODELLO);
*/
	memcpy (punt_head, nomi -> name[mod], (strlen(nomi -> name[mod])+1));
	punt_head += MAX_LUN_NOME_MODELLO;
	memcpy (punt_head, &nomi_modelli.num_blocchi, sizeof (unsigned short));
	punt_head += sizeof (unsigned short);
	memcpy (punt_head, &offset, sizeof (int));
	punt_head += sizeof (int);
#if defined UNIX
	punt_head += 1;
#endif
	offset += nomi_modelli.num_blocchi * sizeof (NOMI_BLOCCHI);
    }

    /* MODIFICA scrive i nomi dei blocchi e il numero dei dati */
    for (mod = 0; mod < nomi -> nmod; mod++)
    {
	for (bloc = 0; bloc < nomi -> num_blocchi[mod]; bloc++)
	{
	    memcpy (punt_head, nomi -> nom_bloc[mod][bloc], MAX_LUN_NOME_BLOCCO);
	    punt_head += MAX_LUN_NOME_BLOCCO;
            memcpy(punt_head,&nomi ->num_dati_bloc[mod][bloc],sizeof(int));
            punt_head = punt_head + sizeof(int);
	}
    }

    /* scrive le variabili */
    memcpy (punt_head, &tot_variabili, sizeof (int));
    punt_head += sizeof (int);
    /* Scrittura del file temporaneo di header */
    if (!(fp_varapp_head = fopen ("VAR_HEAD_TMP", "w")))
	exit (fprintf (stderr,"Impossibile creare VAR_HEAD_TMP"));
    fwrite (ind_head, size_head, 1, fp_varapp_head);
    fclose (fp_varapp_head);

    strcpy(comando_conc,"cat VAR_HEAD_TMP VAR_TMP > variabili.rtf");
    system(comando_conc);

    /* libera la struttura nomi */
    for(i=0;i<nmod;i++)
       cfree2(nomi->nom_bloc[i]);
    ifree2(nomi->num_dati_bloc);
    free (nomi);
    free (variabili);
    free (var_ridotto);

    /* crea il file ASCII variabili.rtf */
    output_ascii_big (ind_head);

    /* Eliminazione dei files temporanei */
    strcpy(comando_conc,"rm VAR_HEAD_TMP");
    system(comando_conc);
    strcpy(comando_conc,"rm VAR_TMP");
    system(comando_conc);

    free(ind_head);
}

/* aggancio all shared memory ed eventualmente la riempie */

void costruisci_var (p_ind, p_var, id)
char  **p_ind;
VARIABILI ** p_var;
int    *id;
{
    NOMI * nomi;
    NOMI_MODELLI nomi_modelli;
    VARIABILI * variabili;
int     tot_variabili;
int     tot_ingressi;
int     mod, bloc, i;
int     size_nomi, size_variabili, size;
char   *ind;		/* puntatore inizio shared memory */
char   *punt;
int     offset;
int     shr_usr_key;	/* chiave utente per shared  */
    FILE * fp_var;
struct stat *buf_var;
#if defined UNIX
struct shmid_ds buf;
#endif
int     esiste_sh = 0;

VARIABILI_RID * variabili_rid;
/*
        prelega dall'environment i parametri caratteristici del lego
*/
/*
if(!GetParLego())
 exit( fprintf(stderr, "ATTENZIONE: i parametri del LEGO non sono definiti"));
*/
if(!GetParLego()) {
 fprintf(stderr, "ATTENZIONE: i parametri del LEGO non sono definiti");
}
    if (!(fp_var = fopen ("variabili.rtf", "r")))
    {
	fprintf (stderr,"Errore il file variabili.rtf non esiste\n");
	exit (0);
	/* se non esiste il file variabili.rtf */

	/* alloca la struttura nomi */
	nomi = (NOMI *) malloc (sizeof (NOMI));

	/* legge i dati da S02 S04 S05 */
	tot_variabili = 0;
	legge_dati (&variabili, &variabili_rid, nomi, &tot_variabili, &tot_ingressi);

	/* calcolo size */
	size_variabili = sizeof (VARIABILI) * tot_variabili + sizeof (int);
	size_nomi = sizeof (int) + sizeof (NOMI_MODELLI) * nomi -> nmod;
	for (mod = 0; mod < nomi -> nmod; mod++)
	    size_nomi = size_nomi + sizeof (NOMI_BLOCCHI) * nomi -> num_blocchi[mod];
	size = size_variabili + size_nomi;

	/* costruzione shared memory */
	shr_usr_key = atoi ((char *) getenv ("SHR_USR_KEY"));
/*
   (*id) = shmget(shr_usr_key+ID_SHM_VAR, size, 0777 | IPC_CREAT);
   ind=shmat((*id),0,! ( SHM_RND & SHM_RDONLY ));
*/
	ind = (char *) sim_shvar (shr_usr_key, size);
	*p_ind = ind;

	/* caricamento shared memory */

	punt = ind;
	memcpy (punt, &(nomi -> nmod), sizeof (int));
	punt += sizeof (int);
	offset = sizeof (NOMI_MODELLI) * nomi -> nmod;

	/* scrive le struttura NOMI_MODELLI */
	for (mod = 0; mod < nomi -> nmod; mod++)
	{
/*
	memcpy(nomi_modelli.nome,nomi->name[mod],MAX_LUN_NOME_MODELLO);
	nomi_modelli.num_blocchi=(unsigned short)nomi->num_blocchi[mod];
	nomi_modelli.offset=offset;
	memcpy(punt,&nomi_modelli,sizeof(NOMI_MODELLI));
*/
	    memcpy (punt, nomi -> name[mod], MAX_LUN_NOME_MODELLO);
	    punt += MAX_LUN_NOME_MODELLO;
	    nomi_modelli.num_blocchi = (unsigned short) nomi -> num_blocchi[mod];
	    memcpy (punt, &nomi_modelli.num_blocchi, sizeof (unsigned short));
	    punt += sizeof (unsigned short);
	    memcpy (punt, &offset, sizeof (int));
	    punt += sizeof (int);
#if defined UNIX
	    punt += 1;
#endif
	    offset += nomi_modelli.num_blocchi * sizeof (NOMI_BLOCCHI);
	}

	/* scrive i nomi dei blocchi  */
	for (mod = 0; mod < nomi -> nmod; mod++)
	{
	    for (bloc = 0; bloc < nomi -> num_blocchi[mod]; bloc++)
	    {
		memcpy (punt, nomi -> nom_bloc[mod][bloc], MAX_LUN_NOME_BLOCCO);
		punt  = punt + MAX_LUN_NOME_BLOCCO;
                memcpy (punt, &nomi -> num_dati_bloc[mod][bloc] ,sizeof(int));
                punt = punt + sizeof(int);
	    }
	}

	/* scrive il numero delle variabili */
	memcpy (punt, &tot_variabili, sizeof (int));
	punt += sizeof (int);

	/* scrive le variabili */
	*p_var = (VARIABILI *) punt;
	memcpy (punt, variabili, sizeof (VARIABILI) * tot_variabili);
	punt += sizeof (VARIABILI) * tot_variabili;

	/* registra il file variabili.rtf  */
	if (!(fp_var = fopen ("variabili.rtf", "w")))
	    exit (fprintf (stderr,"Impossibile creare variabili.rtf"));
	fwrite (ind, size, 1, fp_var);
	fclose (fp_var);

	/* libera la struttura nomi */
	free (nomi);
        
        free(variabili);
        free(variabili_rid);

	/* crea il file ASCII variabili.edf */
	output_ascii (ind, *p_var);
    }
    else
    {
	shr_usr_key = atoi ((char *) getenv ("SHR_USR_KEY"));

/* testa l'esistenza della shared memory */

	esiste_sh = shresist (shr_usr_key + ID_SHM_VAR);

	/* legge le dimensione del file variabili.rtf */
buf_var = (struct stat *) malloc (sizeof (struct stat));
	stat ("variabili.rtf", buf_var);
	size = (int) (buf_var -> st_size);

	/* crea la shared memory delle opportune dimensioni */
	shr_usr_key = atoi ((char *) getenv ("SHR_USR_KEY"));

	ind = (char *) sim_shvar (shr_usr_key, size);
	*p_ind = ind;

	/* carica il file nella shared memory */
	if (esiste_sh == 0)
	{
	    /* fp_var = fopen( "variabili.rtf", "r"); */
	    if (fread (ind, (int) (buf_var -> st_size), 1, fp_var) < 0)
		perror ("Lettura variabili.rtf");
	    fclose (fp_var);
	}
	free (buf_var);

	/* costruisce il puntatore alle variabili */
	memcpy (&mod, ind, sizeof (int));
	
// GUAG aprile 2010 - problema con la versione 64 bit: puntatori trattati come interi!
/*	
	offset = (int) ind + sizeof (int) + mod * sizeof (NOMI_MODELLI);
	for (i = 0; i < mod; i++)
	    offset += sizeof (NOMI_BLOCCHI) * numero_blocchi (ind, i + 1);
	offset += sizeof (int);
	*p_var = (VARIABILI *)offset;
*/  
	
	char *poffset;
	poffset = ind + sizeof (int) + mod * sizeof (NOMI_MODELLI);
	for (i = 0; i < mod; i++)
	    poffset += sizeof (NOMI_BLOCCHI) * numero_blocchi (ind, i + 1);
	poffset += sizeof (int);
	*p_var = (VARIABILI *)poffset;
    }


}

/* costruisce i dati da inserire nella shared memory 
   leggendoli eventualmente da file 			*/

void legge_dati (p_variabili, p_variabili_rid, nomi, tot_variabili, tot_ingressi)
VARIABILI ** p_variabili;
VARIABILI_RID ** p_variabili_rid;
NOMI * nomi;
int    *tot_variabili;
int    *tot_ingressi;
{
/*                     variabili locali                  */
typedef struct collega
{
    char    coll[MAX_COLL][8];
}                       COLLEG_ST;

VARIABILI *variabili1,*variabili2,*variabili3,*variabili4,*variabili5,*variabili6;
int num_var_parz,count;

    FILE * fp_s04;
    FILE * fp_s05;
    FILE * fp_out;
int       nmod, nrip;
int       i, k, j;
int       i1, i2;
int       indirizzo;
int 	  offset_task;
char      *sptr;
int       spiazzamento;
COLLEG_ST * colleg;
int       ncoll[MAX_MODEL];
char      *stringa;
int       num_dati_bloc;
VARIABILI * variabili;
F04       * f04_;
FILE      * fp_txt;
S02       * s02_;

FILE      * fp_varapp;
VARIABILI *variabili_app;
VARIABILI_RID * var_ridotto;
int       offset_tipo;
   /*
        parametri lego
   */
#if defined UNIX
extern int _N000;
extern int _N001;
extern int _N002;
extern int _N003;
extern int _N004;
extern int _N005;
extern int _N007;
extern int _M001;
extern int _M002;
extern int _M003;
extern int _M004;
extern int _M005;
#endif



    f04_ = (F04 *) malloc (sizeof (F04));


#if defined UNIX
    /*
        allocazione dinamica struttura F04
    */
    f04_->nom_bloc = (char **)cdim2(_N002,MAX_LUN_NOME_BLOCCO);
    f04_->nom_sivar = (char **)cdim2(_N003,100);
    f04_->nom_vari = (char **)cdim2(_N004,100);
    f04_->ip = (int*)malloc(sizeof(int)*_M003);
    f04_->ipvrs = (int*)malloc(sizeof(int)*_N005);
    f04_->iout_sivar = (int*)malloc(sizeof(int)*_N003);
    f04_->iout_vari = (int*)malloc(sizeof(int)*_N004);
    f04_->ipdati = (int*)malloc(sizeof(int)*(_N002+1));
#endif

    s02_ = (S02 *) malloc (sizeof (S02));
    if (!(fp_s04 = fopen ("S04", "r")))
	exit (fprintf (stderr,"File S04 non trovato - fine non corretta del programma"));

    if (!(fp_varapp = fopen ("VAR_TMP", "w")))
	exit (fprintf (stderr,"Errore apertura VAR_TMP - fine non corretta del programma"));

/* legge il numero dei modelli  */
    fread (&nmod, sizeof (int), 1, fp_s04);


/* riempie la variabili nomi  */
    nomi -> nmod = nmod;

/* legge i nomi e le descrizioni dei modelli  */
    for (i = 0; i < nmod; i++)
    {
	fread (s02_ -> model[i].name, sizeof (s02_ -> model[i].name), 1, fp_s04);
	fread (s02_ -> model[i].descr, sizeof (s02_ -> model[i].descr), 1, fp_s04);
	/* riempie la variabili nomi */
	strncpy (nomi -> name[i], s02_ -> model[i].name, strlen(s02_ -> model[i].name));
        (nomi -> name[i])[strlen(s02_ -> model[i].name)] = '\0';
    }

/* legge il file per ottenere il numero delle variabili totali di tutti
   i modelli  */

    for (k = 0; k < nmod; k++)
    {
	fread (&(f04_ -> nbl), sizeof (int), 1, fp_s04);
	fread (&(f04_ -> nbl1), sizeof (int), 1, fp_s04);
	fread (&(f04_ -> nvart), sizeof (int), 1, fp_s04);
	fread (&(f04_ -> neqsis), sizeof (int), 1, fp_s04);
	fread (&(f04_ -> nu), sizeof (int), 1, fp_s04);
	fread (&(f04_ -> neqal), sizeof (int), 1, fp_s04);
	fread (&(f04_ -> ndati), sizeof (int), 1, fp_s04);

	*tot_variabili = (*tot_variabili) + f04_ -> nvart;
/* MODI ( seconda riga di spiazzamento ) */
/*
printf("NOME_MOD = %s NUM_BLOC = %d NUM_VAR_TOT_BLOC = %d\n",
        nomi -> name[k], f04_ -> nbl, f04_ -> nvart); 
*/

	spiazzamento = sizeof (char) * f04_ -> nbl * MAX_LUN_NOME_BLOCCO +
            sizeof (int) * f04_-> nbl1 +
	    sizeof (int) * f04_ -> nbl1 +
	    sizeof (int) * f04_ -> nvart +
	    sizeof (char) * f04_ -> neqsis * 100 +
	    sizeof (int) * f04_ -> neqsis +
	    sizeof (char) * f04_ -> nu * 100 +
	    sizeof (int) * f04_ -> nu;
	fseek (fp_s04, spiazzamento, 1);
    }

printf("tot_variabili = %d\n", *tot_variabili);

/* allocazione del puntatore variabili  */
    variabili = (VARIABILI *) calloc (1, sizeof (VARIABILI));
    variabili_app = (VARIABILI *) calloc (1, sizeof (VARIABILI));
    var_ridotto = (VARIABILI_RID *) calloc ((*tot_variabili), sizeof (VARIABILI_RID));


/* riazzeramento del puntatore al file */
    rewind (fp_s04);

/* scorrimento dell'header */
    spiazzamento = sizeof (int) +
	sizeof (s02_ -> model[0].name) * nmod +
	sizeof (s02_ -> model[0].descr) * nmod;
    fseek (fp_s04, spiazzamento, 1);

/*fseek(fp_s04,sizeof(int),1);
fseek(fp_s04 , sizeof( s02_->model[0].name)*nmod, 1);
fseek(fp_s04 , sizeof( s02_->model[0].descr)*nmod, 1);*/



/* azzero il valore di offset_task  */
offset_task = 0;


/* lettura delle variabili di tutti i modelli */
    *tot_variabili = (-1);
    for (j = 0; j < nmod; j++)
    {
/* inizializzazione dell'indirizzo delle variabili per ogni modello  */
	indirizzo = offset_task;
/*inizializzazione di minimo e massimo */


	fread (&(f04_ -> nbl), sizeof (f04_ -> nbl), 1, fp_s04);
	fread (&(f04_ -> nbl1), sizeof (f04_ -> nbl1), 1, fp_s04);
	fread (&(f04_ -> nvart), sizeof (f04_ -> nvart), 1, fp_s04);
	fread (&(f04_ -> neqsis), sizeof (f04_ -> neqsis), 1, fp_s04);
	fread (&(f04_ -> nu), sizeof (f04_ -> nu), 1, fp_s04);
	fread (&(f04_ -> neqal), sizeof (f04_ -> neqal), 1, fp_s04);
	fread (&(f04_ -> ndati), sizeof (f04_ -> ndati), 1, fp_s04);

printf("nvart = %d neqsis = %d nu = %d neqal=%d ndati = %d\n",
	f04_ -> nvart,f04_ -> neqsis,f04_ -> nu,f04_ -> neqal,f04_ -> ndati);

	offset_task +=  (f04_ -> neqsis + f04_ -> nu + f04_ -> ndati +
			(f04_ -> neqsis - f04_ -> neqal));

/* nomi dei blocchi  */
	fread (*f04_ -> nom_bloc, sizeof (char), f04_ -> nbl*MAX_LUN_NOME_BLOCCO, fp_s04);

/* Puntatori ai dati */
	fread (f04_ -> ipdati, sizeof (int), f04_ -> nbl1,
               fp_s04);

/*
        for (i=0;i<6;i++)
        for (i=0;i<f04_->nbl1;i++)
            printf("%d %d:\n:",i,f04_-> ipdati[i]);
            printf("%d:\n",num_dati_bloc);
            printf("**LETTURA S04 **\n ipdati[%d] :%d\n",i,f04_-> ipdati[i]);
*/

                
/* riempie la variabili nomi  */
	nomi -> num_blocchi[j] = f04_ -> nbl;
	for (i = 0; i < f04_ -> nbl; i++)
{
            num_dati_bloc = f04_ -> ipdati[i+1] - f04_ -> ipdati[i];
	    strcpy (nomi -> nom_bloc[j][i], f04_ -> nom_bloc[i]);
            nomi -> num_dati_bloc[j][i] = num_dati_bloc;
}


/* puntatori alle variabili */
	fread (f04_ -> ip, sizeof (int), f04_ -> nbl1, fp_s04);
	fread (f04_ -> ipvrs, sizeof (int), f04_ -> nvart, fp_s04);

/* variabili della simulazione  */
	for (i = 0; i < f04_ -> neqsis; i++)
	{
	    fread (f04_ -> nom_sivar[i], sizeof (char), 100, fp_s04);
	    fread (&(f04_ -> iout_sivar[i]), sizeof (int), 1,
		    fp_s04);
	}

/* variabili di ingresso    */
	for (i = 0; i < f04_ -> nu; i++)
	{
	    fread (f04_ -> nom_vari[i], sizeof (char), 100, fp_s04);
	    fread (&(f04_ -> iout_vari[i]), sizeof (int), 1,
		    fp_s04);
	}

/* scrittura delle variabili */
	for (i = 0; i < f04_ -> nbl; i++)
	{
	    for (k = (f04_ -> ip[i] - 1); k < (f04_ -> ip[i + 1] - 1); k++)
	    {
		++(*tot_variabili);

                variabili_app->mod = j + 1;
                variabili_app->blocco = i + 1;

		if (f04_ -> ipvrs[k] > 0)
		{
		    strncpy (var_ridotto[*tot_variabili].nome,
			    f04_ -> nom_sivar[f04_ -> ipvrs[k] - 1], 8);
		    var_ridotto[*tot_variabili].nome[8] = '\00';
		    fill_space (var_ridotto[*tot_variabili].nome, 8);

                    strncpy (variabili_app->nome,
                            f04_ -> nom_sivar[f04_ -> ipvrs[k] - 1], 8);
                    variabili_app->nome[8] = '\00';
                    fill_space (variabili_app->nome, 8);
                    sptr = f04_ -> nom_sivar[f04_ -> ipvrs[k] - 1];
                    strncpy (variabili_app->descr, sptr + 9,
                            MAX_LUN_DESCR_VAR);
                    variabili_app->descr[MAX_LUN_DESCR_VAR - 1] =
                        '\00';
                    variabili_app->addr =
                        indirizzo + f04_ -> ipvrs[k] - 1;
                    variabili_app->tipo = STATO;

		    var_ridotto[*tot_variabili].tipo = STATO;

                    if (f04_ -> nom_sivar[f04_ -> ipvrs[k] - 1][9] == '*')
                        variabili_app->typ = GRAF_SEL;
                    else
                        variabili_app->typ = NO_GRAF_SEL;
                    variabili_app->typlg = NO_GRAF_SEL;
/*
printf(" 1 VARIABILI = %s  VAR_APP = %s\n",variabili[*tot_variabili].nome,var_ridotto[*tot_variabili].nome);
*/
		}
		else
		{
		    strncpy (var_ridotto[*tot_variabili].nome,
			    f04_ -> nom_vari[(-f04_ -> ipvrs[k] - 1)], 8);
		    var_ridotto[*tot_variabili].nome[8] = '\00';
		    fill_space (var_ridotto[*tot_variabili].nome, 8);

                    strncpy (variabili_app->nome,
                            f04_ -> nom_vari[(-f04_ -> ipvrs[k] - 1)], 8);
                    variabili_app->nome[8] = '\00';
                    fill_space (variabili_app->nome, 8);
                    sptr = f04_ -> nom_vari[(-f04_ -> ipvrs[k] - 1)];
                    strncpy (variabili_app->descr, sptr + 9,
                            MAX_LUN_DESCR_VAR);
                    variabili_app->descr[MAX_LUN_DESCR_VAR - 1] =
                        '\00';
                    variabili_app->addr = indirizzo +
                        f04_ -> neqsis - f04_ -> ipvrs[k] - 1;
                    variabili_app->tipo = INGRESSO_NC;

		    var_ridotto[*tot_variabili].tipo = INGRESSO_NC;

                    if (f04_ -> nom_vari[(-f04_ -> ipvrs[k] - 1)][9] == '*')
                        variabili_app->typ = GRAF_SEL;
                    else
                        variabili_app->typ = NO_GRAF_SEL;
                    variabili_app->typlg = NO_GRAF_SEL;
/*
printf(" 2 VARIABILI = %s  VAR_APP = %s\n",variabili[*tot_variabili].nome,var_ridotto[*tot_variabili].nome);
*/
		}
		if ((variabili_app->nome[0] == 'J') || 
		    (variabili_app->nome[0] == 'V') )
		    variabili_app->tipo_var = DIGITALE;
		else
		    variabili_app->tipo_var = ANALOGICO;
                
                fwrite (variabili_app, sizeof(VARIABILI), 1, fp_varapp);

	    }
	}
    }
    ++(*tot_variabili);

    fclose (fp_s04);
/*
    fclose (fp_varapp);
*/
    /*
	libero la struttura f04
    */
    cfree2(f04_->nom_bloc);
    cfree2(f04_->nom_sivar);
    cfree2(f04_->nom_vari);
    free(f04_->ip);
    free(f04_->ipvrs);
    free(f04_->iout_sivar);
    free(f04_->iout_vari);
    free(f04_->ipdati);
    free (f04_);
    free (s02_);
    if (!(fp_s05 = fopen ("S05", "r")))
	exit (fprintf (stderr,"File S05 non trovato - fine non corretta del programma"));

    rewind(fp_varapp);
    offset_tipo = MAX_LUN_NOME_VAR + MAX_LUN_DESCR_VAR + (2 * sizeof(unsigned short));

    colleg = (COLLEG_ST *) malloc (sizeof (COLLEG_ST) * nmod);
    stringa = (char *) malloc (sizeof (char) * 9);
/* legge il numero dei modelli  */
    fread (&nmod, sizeof (int), 1, fp_s05);
    for (i = 0; i < nmod; i++)
    {
	fread (&ncoll[i], sizeof (int), 1, fp_s05);
	fread (colleg[i].coll[0], sizeof (colleg[i].coll[0]), ncoll[i], fp_s05);
    }
    for (i = 0; i < nmod; i++)
	for (j = 0; j < ncoll[i]; j++)
	{
	    strncpy (stringa, colleg[i].coll[j], 8);
            stringa[8] = '\0';
	    for (k = 0; k < *tot_variabili; k++) {
		if (strncmp (stringa, var_ridotto[k].nome, 8) == 0) { 
/*
printf("NOME = %s   k = %d\n",var_ridotto[k].nome,k);
*/
		    if (var_ridotto[k].tipo == INGRESSO_NC)
		    {
/*
printf("NOME = %s   k = %d\n",var_ridotto[k].nome,k);
*/
			var_ridotto[k].tipo = INGRESSO_C;
	                fseek (fp_varapp, (offset_tipo + (k*sizeof(VARIABILI))), 0);
                        fwrite (&var_ridotto[k].tipo, sizeof(char), 1, fp_varapp);
			break;
		    }
                }
            }
	}

    free (colleg);
    free (stringa);
    free(variabili_app);
    *tot_ingressi = 0;
    for (k = 0; k < (*tot_variabili); k++) {
/*
	if (strncmp (stringa, variabili[k].nome, 8)) {
*/
	    if (var_ridotto[k].tipo == INGRESSO_NC)
		++(*tot_ingressi);
    }
    fclose (fp_s05);
    fclose(fp_varapp);
printf(" tot_ingressi = %d \n", *tot_ingressi);
    *p_variabili = (variabili);
    *p_variabili_rid = (var_ridotto);
}



void output_ascii (ind, variabili)
char   *ind;
VARIABILI * variabili;
{
    FILE * fp_txt;
int     k, i, j;
int     nmod;
int     nbl;
int     tot_variabili;
int     var_bl;

/* apertura file variabili.txt */
    if (!(fp_txt = fopen ("variabili.edf", "w")))
	exit (fprintf (stderr,"Impossibile creare variabili.edf"));
/* stampa legenda */
    fprintf (fp_txt, "USCITA = 0  INGRESSO LIBERO = 1  INGRESSO CONNESSO = 2\n\n");
    nmod = numero_modelli (ind);
    tot_variabili = numero_variabili (ind);
    fprintf (fp_txt, "NUMERO MODELLI %d\n", nmod);
    for (k = 0; k < nmod; k++)
    {

	nbl = numero_blocchi (ind, k + 1);
	fprintf (fp_txt, "\n\nNOME MODELLO %d %s NUMERO BLOCCHI %d\n",
		k + 1, nome_modello (ind, k + 1), nbl);
	for (i = 0; i < nbl; i++)
	{
	    var_bl = 0;
	    for (j = 0; j < tot_variabili; j++)
	    {
		if ((variabili[j].mod == k + 1) && (variabili[j].blocco == i + 1))
		    ++var_bl;
	    }
	    fprintf (fp_txt,"\nBLOCCO %80s NUMERO VARIABILI %d NUMERO DATI BLOCCO %d\n",nome_blocco(ind,k+1,i+1),var_bl,numero_dati(ind,k+1,i+1));
	    for (j = 0; j < tot_variabili; j++)
	    {
		if ((variabili[j].mod == k + 1) && (variabili[j].blocco == i + 1))
		    fprintf (fp_txt, "%d %s %d %s\n", variabili[j].tipo,
			    variabili[j].nome, variabili[j].addr,
			    variabili[j].descr);
	    }
	}
    }
    fclose (fp_txt);
}



void output_ascii_big (ind_head)
char   *ind_head;
{
FILE       * fp_edf;
FILE       * fp_var;
int        k, i, j;
int        nmod;
int        nbl;
int        tot_variabili;
int        var_bl;
VARIABILI  var_app;
VARIABILI  *var_blocco;
int        flag, end_file_var_tmp;
char       *zErrMsg = 0;
char       *SqlStatement;
int        rc;
char	   sigla_blocco[256];
char       *descrizione_blocco;
char       *tag_kks;
char	   *null_string="NULL";
char	   descrizione_variabile[256];
char       sigla_blocchetto[2+1];
char       tipo_blocchetto[4+1];

    end_file_var_tmp = 0;

/* apertura file variabili.edf */
    if (!(fp_edf = fopen ("variabili.edf", "w")))
	exit (fprintf (stderr,"Impossibile creare variabili.edf"));
/* apertura file variabili.rtf */
    if (!(fp_var = fopen ("VAR_TMP", "r")))
	exit (fprintf (stderr,"Impossibile aprire VAR_TMP"));

/* stampa legenda */
    fprintf (fp_edf, "USCITA = 0  INGRESSO LIBERO = 1  INGRESSO CONNESSO = 2\n\n");
    nmod = numero_modelli (ind_head);
    tot_variabili = numero_variabili (ind_head);
    fprintf (fp_edf, "NUMERO MODELLI %d\n", nmod);
    fread (&var_app, sizeof(VARIABILI), 1, fp_var);
    for (k = 0; k < nmod; k++)
    {
	nbl = numero_blocchi (ind_head, k + 1);
/*
printf("output_ascii_big DEBUG: nbl = %d del modello %d\n", nbl, k);
*/
	fprintf (fp_edf, "\n\nNOME MODELLO %d %s NUMERO BLOCCHI %d\n",
		k + 1, nome_modello (ind_head, k + 1), nbl);
	for (i = 0; i < nbl; i++)
	{
	    var_bl = 0;
            var_blocco = NULL;
            flag = 0;

/*
printf("output_ascii_big DEBUG: ciclo blocco %d del modello %d\n", i, k);
printf("output_ascii_big DEBUG: var_app.mod = %d var_app.blo = %d\n",
       var_app.mod, var_app.blocco);
*/
	    for (;;)
	    {
                if(end_file_var_tmp == 1)
                   break;
		if ((var_app.mod == k + 1) && (var_app.blocco == i + 1)) {
                    flag = 1;
		    ++var_bl;
                    var_blocco = (VARIABILI *)realloc(var_blocco, sizeof(VARIABILI) * var_bl);
                    memcpy (&var_blocco[var_bl-1], &var_app, sizeof (VARIABILI));
	            if(fread (&var_app, sizeof(VARIABILI), 1, fp_var)==0) {
                       end_file_var_tmp = 1;
                       break;
                    }
                }
                else
                   if (flag == 1)
                     break;
	    }

	    fprintf (fp_edf,"\nBLOCCO %80s NUMERO VARIABILI %d NUMERO DATI BLOCCO %d\n",
                               nome_blocco(ind_head,k+1,i+1),var_bl,numero_dati(ind_head,k+1,i+1));

            /* Se db != NULL gestisco Scrittura  info BLOCCHI in sqlite */
            if ( db != NULL )
            {
	       strncpy(sigla_blocco, nome_blocco(ind_head,k+1,i+1), 8);
	       sigla_blocco[8]='\0'; 
               descrizione_blocco=nome_blocco(ind_head,k+1,i+1) + 10;
               SqlStatement = sqlite3_mprintf("insert into BLOCCHI(progr_blocco, sigla, descrizione, progr_modello) values ( '%d', '%q', '%q', '%d' );",
                                            i, sigla_blocco, descrizione_blocco, k+1);
               rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
               if( rc!=SQLITE_OK ){
                 fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
               }
               sqlite3_free(SqlStatement);
            }
            /* FINE Scrittura info BLOCCHI in sqlite */


	    for (j = 0; j < var_bl; j++)
	    {
                char *puntCarDaElim;
                // elimina il carattere \r (13 ASCII) se presente
                if ( (puntCarDaElim = strchr(var_blocco[j].descr, 13)) != NULL )
                {
                   *puntCarDaElim = '\0';
                }
		fprintf (fp_edf, "%d %s %d %s\n", var_blocco[j].tipo,
		         var_blocco[j].nome, var_blocco[j].addr,
                         var_blocco[j].descr);


            /* Se db != NULL gestisco Scrittura  info VARIABILI in sqlite */
            if ( db != NULL )
            {
               SqlStatement = sqlite3_mprintf("insert into VARIABILI(progr_variabile, sigla, descrizione, nome_unita_misura, progr_tipivar) values ( '%d', '%q', '%q', '%q', '%d' );",
                                                 var_blocco[j].addr, var_blocco[j].nome, var_blocco[j].descr, "NOME_UNITA_MIS", var_blocco[j].tipo);
               rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
               if( rc!=SQLITE_OK &&  rc!=SQLITE_CONSTRAINT){
                        fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
               }
               sqlite3_free(SqlStatement);

/* Se esiste una tag KKS la inserisco con un update in tag_kks */
               tag_kks=strstr(var_blocco[j].descr,"@#K@");
	       if ( tag_kks != NULL)
	       {
                   char * puntaSpazio=NULL;
                   // taglia i primi 4 caratteri @#K@
		   tag_kks+=4;
		   // se contiene un blank rendilo fine stringa
		   puntaSpazio=strchr(tag_kks,' ');
		   if (puntaSpazio != NULL)
		      {
		      *puntaSpazio='\0';
                      }
                   SqlStatement = sqlite3_mprintf("update VARIABILI set  tag_kks = '%q' where progr_variabile = '%d';",
                                                    tag_kks, var_blocco[j].addr);
                   rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
                   if( rc!=SQLITE_OK &&  rc!=SQLITE_CONSTRAINT){
                     fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
                   }
               sqlite3_free(SqlStatement);
               }

/* Se è una task di regolazione faccio l'update dei campi sigla_blocchetto e tipo_blocchetto  */
		
	       if (s02_.model[k].tipo_task == TASK_REGOLAZIONE)
	       {
                   strcpy(descrizione_variabile,var_blocco[j].descr);
                   strncpy(sigla_blocchetto, var_blocco[j].descr + 12, 2);
                   sigla_blocchetto[2] = '\00';
                   strncpy(tipo_blocchetto, var_blocco[j].descr + 8, 4);
                   tipo_blocchetto[4] = '\00';
                   SqlStatement = sqlite3_mprintf("update VARIABILI set  sigla_blocchetto = '%q', tipo_blocchetto = '%q' where progr_variabile = '%d';",
                                                    sigla_blocchetto, tipo_blocchetto,  var_blocco[j].addr);
                   rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
                   if( rc!=SQLITE_OK &&  rc!=SQLITE_CONSTRAINT){
                     fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
                   }
               sqlite3_free(SqlStatement);
               }
             }
            /* FINE Scrittura info VARIABILI in sqlite */

            /* Se db != NULL gestisco Scrittura  info BLOC_VAR in sqlite */
            if ( db != NULL )
            {
                SqlStatement = sqlite3_mprintf("insert into BLOC_VAR(progressivo, progr_blocco, progr_modello, progr_variabile) values ( '%d', '%d', '%d', '%d' );",
                                                   j, i, k+1, var_blocco[j].addr);
                rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
                if( rc!=SQLITE_OK ){
                  fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
                }
                sqlite3_free(SqlStatement);
             }
            /* FINE Scrittura info BLOC_VAR in sqlite */


	    }
            free(var_blocco);
            var_blocco = NULL;
	}
    }
    fclose (fp_edf);
    fclose (fp_var);
}



	void fill_space (str, lun)
char   *str;
int     lun;
{
int     i;
    for (i = strlen (str); i < lun; i++)
    {
	str[i] = ' ';
    }
    str[lun] = 0;
}

