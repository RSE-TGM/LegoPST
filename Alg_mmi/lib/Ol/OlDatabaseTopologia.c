/**********************************************************************
*
*       C Source:               OlDatabaseTopologia.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Fri Jan 17 15:49:44 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: OlDatabaseTopologia.c-14 %  (%full_filespec: OlDatabaseTopologia.c-14:csrc:1 %)";
#endif
/*
   modulo OlDatabaseTopologia.c
   tipo 
   release 5.3
   data 3/6/96
   reserved @(#)OlDatabaseTopologia.c	5.3
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)OlDatabaseTopologia.c	5.3\t3/6/96";
/*
        Fine sezione per SCCS
*/
/*
 *  OlDatabaseTopologia.c 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <Xl/XlResources.h>
#include <Xl/XlGraficoP.h>

#include <Ol/OlDatabaseTopologiaP.h>

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

char   *nome_modello (char *, int);
char *sim_shvar(int,int);
extern int   ind_modello (char *, int, VARIABILI *);
extern int   numero_modello (char*, int, char*);
extern 	char   *nome_blocco (char*, int, int);
extern int  numero_blocco(char *ind, int nmod, char *nome);




/* lista delle risorse  */
static XlResources resources[]= {
        {
        XlNnome,
        XlCNome,
        XmRString,
        sizeof(char *),
        XtOffsetOf(OlDatabaseTopologiaRec,oldatabaseTopologia.nome_database),
        XmRString,
        "olrtf"
        },
        {
        XlNnomeFile,
        XlCNomeFile,
        XmRString,
        sizeof(char *),
        XtOffsetOf(OlDatabaseTopologiaRec,oldatabaseTopologia.nome_file),
        XmRString,
        "None"
        },
        {
        XlNshrKey,
        XlCShrKey,
        XmRInt,
        sizeof(int),
        XtOffsetOf(OlDatabaseTopologiaRec,oldatabaseTopologia.shr_key),
        XmRInt,
        0
        },
	{
	XlNfileSubSystem,
	XlCFileSubSystem,
	XmRString,
	sizeof(char *),
	XtOffsetOf(OlDatabaseTopologiaRec,oldatabaseTopologia.file_subs),
	XmRString,
	"None"
	},
	{
	XlNfileComponent,
	XlCFileComponent,
	XmRString,
	sizeof(char *),
	XtOffsetOf(OlDatabaseTopologiaRec,oldatabaseTopologia.file_comp),
	XmRString,
	"None"
	},
	{
	XlNfileVarCC,
	XlCFileVarCC,
	XmRString,
	sizeof(char *),
	XtOffsetOf(OlDatabaseTopologiaRec,oldatabaseTopologia.file_varcc),
	XmRString,
	"None"
	}
	};

/* dichiarazioni funzioni varie */
static Boolean init_db();
static Boolean init_db_rtf();
static Boolean init_db_oracle();
static int confVar(char *, char *);
static Boolean ricerca( char *, short *, short *, long *, short *, FILE *);
int rbyte(int ,void * ,size_t ,size_t );
         
/* dichiarazione dei metodi (methods) */
static Boolean Initialize();
static Boolean TagToInd();
static Boolean TagToIndS();
static int NumModelli();
static int NumBlocchi();
static int NumVariabili();
static VARIABILI * ProgToVar();
static int IndToMod();

extern int sgancia_shrmem(char *);
extern void elimina_shrmem(int,char*,int);

/* Inizializzazione del class record */
OlDatabaseTopologiaClassRec olDatabaseTopologiaClassRec = {
  { /* database fields */
    /* initialize               */      Initialize,
    /* tagtoind                 */      TagToInd,
    /* tagtoinds                */      TagToIndS,
    /* nummodelli               */      NumModelli,
    /* numblocchi               */      NumBlocchi,
    /* numvariabili             */      NumVariabili,
    /* progtovar                */      ProgToVar,
    /* indtomod                 */      IndToMod,
  }
};

ObjectClass olDatabaseTopologiaObjectClass = (ObjectClass) &olDatabaseTopologiaClassRec;


/*
	Metodo di inizializzazioene dell'oggetto database
*/
static Boolean Initialize(OlDatabaseTopologiaObject database) 
{

if(!strcmp(database->oldatabaseTopologia.nome_database,"olscada"))
                {
/*
database topologia "leggero" cioe' senza caricamento variabili.rtf in shm
utilizzato solo per leggere fnomi e ricavarsi puntatore alle variabili
dello scada
*/

                database->oldatabaseTopologia.tipo_db=DB_XLSCADA;
		return(True);
		}
	if(!strcmp(database->oldatabaseTopologia.nome_database,"olrtf"))
                {
                database->oldatabaseTopologia.tipo_db=DB_XLRTF;
                if(!init_db(database))
                        return(False);
                return(True);
                }
	if(!strcmp(database->oldatabaseTopologia.nome_database,"olrtf"))
		{
		database->oldatabaseTopologia.tipo_db=DB_XLRTF;
		if(!init_db(database))
			return(False);
		return(True);
		}
	if(!strcmp(database->oldatabaseTopologia.nome_database,"oloracle"))
		{
		database->oldatabaseTopologia.tipo_db=DB_XLORACLE;
		if(!init_db(database))
			return(False);
		return(True);
		}
	XlWarning("OlDatabaseTopologia","Initialize","database inesistente");
	return(False);
}


/*
	Creazione oggetto database
*/
OlDatabaseTopologiaObject OlCreateDatabaseTopologia(char *nome_database, 
				char *nome_file, int shr_key, char *nome_subs,
				char *nome_comp,char *nome_varcc)
{
OlDatabaseTopologiaObject database=NULL;
char fnomirtf[FILENAME_MAX];
int nres=((Cardinal) (sizeof(resources) / sizeof(resources[0]))); /* numero di 
								risorse */
int k;

	database=(OlDatabaseTopologiaRec*) XtMalloc (sizeof (OlDatabaseTopologiaRec));
	if(database==NULL)
		{
		XlWarning("OlDatabaseTopologia","OlCreateDatabaseTopologia",
			"Impossibile allocare il database\n");
		return(NULL);
		}
/*
Inizializzazione per file fnomi per gestione calcolate SCADA
*/
/*
Apro il file fnomi.rtf per ricavarmi la posizione del punto
*/
database->oldatabaseTopologia.fnomip=NULL;
if(!XlGetenv("LEGOMMI_RTF"))
        {
        fprintf(stderr,"Imposssibile trovare file fnomi.rtf !!!\n");
        }
else
        {
        strcpy(fnomirtf,XlGetenv("LEGOMMI_RTF"));
        strcat(fnomirtf,"/fnomi.rtf");
        if((database->oldatabaseTopologia.fnomip=fopen(fnomirtf,"r"))==NULL)
            {
            XlWarning("OlDatabaseTopologia","OLCREATEDATAB_TOPOLOGIA",
            "Impossibile aprire il file fnomi.rtf per variabili di tipo SCADA");
            printf("OlCreateDatabaseTopologia:pathname fnomi.rtf=%s\n",fnomirtf);
            }
        }

	/*
		allocazione e riempimento risorse 
	*/
	for(k=0;k<nres;k++)
	  {
	  if(!strcmp(resources[k].resource_name,"nome"))
	    {
	    if(nome_database==NULL)
		{
	        database->oldatabaseTopologia.nome_database=(char*)
			XtMalloc (strlen(resources[k].default_addr)+1);
		strcpy(database->oldatabaseTopologia.nome_database,
			resources[k].default_addr);
		}
	    else
		{
	        database->oldatabaseTopologia.nome_database=(char*)
			XtMalloc (strlen(nome_database)+1);
		strcpy(database->oldatabaseTopologia.nome_database,
			nome_database);
		}
	    }
	  if(!strcmp(resources[k].resource_name,"nomeFile"))
	    {
	    if(nome_file==NULL)
		{
	        database->oldatabaseTopologia.nome_file=(char*)
			XtMalloc (strlen(resources[k].default_addr)+1);
		strcpy(database->oldatabaseTopologia.nome_file,
			resources[k].default_addr);
		}
	    else
		{
	        database->oldatabaseTopologia.nome_file=(char*)
			XtMalloc (strlen(nome_file)+1);
		strcpy(database->oldatabaseTopologia.nome_file,
			nome_file);
		}
	    }
	  if(!strcmp(resources[k].resource_name,"shrKey"))
		 database->oldatabaseTopologia.shr_key=shr_key;
          if(!strcmp(resources[k].resource_name,"fileSubS"))
            {
            if(nome_subs==NULL)
                {
                database->oldatabaseTopologia.file_subs=(char*)
                        XtMalloc (strlen(resources[k].default_addr)+1);
                strcpy(database->oldatabaseTopologia.file_subs,
                        resources[k].default_addr);
                }
            else
                {
                database->oldatabaseTopologia.file_subs=(char*)
                        XtMalloc (strlen(nome_subs)+1);
                strcpy(database->oldatabaseTopologia.file_subs,
                        nome_subs);
                }
            }
          if(!strcmp(resources[k].resource_name,"fileComp"))
            {
            if(nome_comp==NULL)
                {
                database->oldatabaseTopologia.file_comp=(char*)
                        XtMalloc (strlen(resources[k].default_addr)+1);
                strcpy(database->oldatabaseTopologia.file_comp,
                        resources[k].default_addr);
                }
            else
                {
                database->oldatabaseTopologia.file_comp=(char*)
                        XtMalloc (strlen(nome_comp)+1);
                strcpy(database->oldatabaseTopologia.file_comp,
                        nome_comp);
                }
            }
          if(!strcmp(resources[k].resource_name,"fileVarCC"))
            {
            if(nome_varcc==NULL)
                {
                database->oldatabaseTopologia.file_varcc=(char*)
                        XtMalloc (strlen(resources[k].default_addr)+1);
                strcpy(database->oldatabaseTopologia.file_varcc,
                        resources[k].default_addr);
                }
            else
                {
                database->oldatabaseTopologia.file_varcc=(char*)
                        XtMalloc (strlen(nome_varcc)+1);
                strcpy(database->oldatabaseTopologia.file_varcc,
                        nome_varcc);
                }
            }
	  }
	/*
		inizializzazione del database
	*/
	if((olDatabaseTopologiaClassRec.oldatabaseTopologia_class.initialize)(database))
		return(database);
	else
		{
		XlWarning("OlDatabaseTopologia","OlDestroyDatabase",
			"database non allocato");
		XtFree((char *)database);
		return(NULL);
		}
}

/*
	Metodo di inizializzazione del database.
	Attualmente e' implementato il solo database olrtf
*/
static Boolean init_db(OlDatabaseTopologiaObject database)
{

	switch(database->oldatabaseTopologia.tipo_db){
	  case(DB_XLRTF):
		return(init_db_rtf(database));
	  }
	XlWarning("OlDatabaseTopologia","init_db","database inesistente");
	return (False);
}


/*
	Inizializzazione database tipo olrtf
*/
static Boolean init_db_rtf(OlDatabaseTopologiaObject database)
{
FILE *fp;
int esiste_sh;
struct stat *buf_var;
int size,mod,i;
long offset;

	/*
		verifico che il file *.rtf esista
	*/
printf ("Apertura file: [%s]\n",database->oldatabaseTopologia.nome_file);
	fp=fopen(database->oldatabaseTopologia.nome_file,"r");
	if( fp==NULL )
	/*if((int)(fp=fopen(database->oldatabaseTopologia.nome_file,"r"))<=0)*/
		{
	        XlWarning("OlDatabaseTopologia","init_db_rtf",
			"Impossibile aprire il file associato al database");
		return(False);
		}

	/* testa l'esistenza della shared memory */
	esiste_sh = shresist(database->oldatabaseTopologia.shr_key+ID_SHM_VAR);
	
	/* legge le dimensione del file variabili.rtf */
	buf_var = (struct stat *) XtMalloc  (sizeof (struct stat));
        stat (XlConvPathVms(database->oldatabaseTopologia.nome_file), buf_var);
        size = (int) (buf_var -> st_size);
        XtFree ((char *)buf_var);

	/* aggancio o creazione shared memory  */
	database->oldatabaseTopologia.id = 
	  (char *) sim_shvar (database->oldatabaseTopologia.shr_key, 
			size);
/*
Introduzione di un controllo per vedere se la creazione o l' aggancio
alla shared maemory e' avvenuto correttamente
*/
	if(database->oldatabaseTopologia.id == NULL)
		{
	        XlWarning("OlDatabaseTopologia","init_db_rtf",
		  "Errore durante la creazione o aggancio alla shared_mem");
		return(False);
		}
	/*  registra la size del database */
	database->oldatabaseTopologia.size = size;

	/* registra il della shared memory */
		{
		/* questa variabile deve essere globale nel
		   fil contenete la chiamata sim_shvar  */
		extern int shmvar;
		database->oldatabaseTopologia.shmvar = shmvar;
		}
	
	/* se il database non esiste carica il file; lo stesso se esiste ma
	   non lo usa nessun altro (residuo di una sessione finita) */
	if (esiste_sh == 0 || sim_shvar_da_caricare ())
        {
            if (fread (database->oldatabaseTopologia.id, 
			size , 1, fp) < 0)
	        XlWarning("OlDatabaseTopologia","init_db_rtf",
		  "Errore durante la lettura del file associato al database");
            fclose (fp);
        }
	/*
		Calcolo del puntatore variabili
	*/
        memcpy (&mod, database->oldatabaseTopologia.id, sizeof (int));
/*
        offset = (int) database->oldatabaseTopologia.id + sizeof (int) 
		+ mod * sizeof (NOMI_MODELLI);
	printf("Numero di modelli letti da file .rtf=%d\n",mod);
        for (i = 0; i < mod; i++)
            offset += sizeof (NOMI_BLOCCHI) * 
		NumBlocchi (database, i + 1);
        offset += sizeof (int);
        database->oldatabaseTopologia.variabili = (VARIABILI *) offset;
*/
        offset = (long) database->oldatabaseTopologia.id + (long)sizeof (int) 
		+ (long)mod * (long)sizeof (NOMI_MODELLI);
	printf("Numero di modelli letti da file .rtf=%d\n",mod);
        for (i = 0; i < mod; i++)
            offset += (long)sizeof (NOMI_BLOCCHI) * 
		(long)NumBlocchi (database, i + 1);
        offset += (long)sizeof (int);
        database->oldatabaseTopologia.variabili = (VARIABILI *) offset;


	return(True);
}

/*
	Inizializzazione database tipo oloracle
	(attualmente non implementato)
*/
static Boolean init_db_oracle(OlDatabaseTopologiaObject database)
{
	XlWarning("OlDatabaseTopologia","init_db_oracle",
		"database ORACLE non abilitato");
	return(False);
}

/*
	Ritorna il numero dei modelli contenuti nel database
*/
static int NumModelli(OlDatabaseTopologiaObject database)
{
int nmod = -1;

if(database==NULL)
	{
	XlWarning("OlDatabaseTopologia","NumModelli","database non allocato");
	return(nmod);
	}

switch(database->oldatabaseTopologia.tipo_db){
	case(DB_XLRTF):
		nmod=numero_modelli (database->oldatabaseTopologia.id);
		break;
	case(DB_XLORACLE):
		break;
	}
return(nmod);
}

/*
	Ritorna il numero dei blocchi di un modello 
	specificato (1)->(numero modelli)
*/
static int NumBlocchi(OlDatabaseTopologiaObject database, int nmod)
{
int nbl = -1;

if(database==NULL)
	{
	XlWarning("OlDatabaseTopologia","NumBlocchi","database non allocato");
	return(nbl);
	}

switch(database->oldatabaseTopologia.tipo_db){
	case(DB_XLRTF):
		nbl=numero_blocchi (database->oldatabaseTopologia.id,nmod);
		break;
	case(DB_XLORACLE):
		break;
	}
return(nbl);
}

/*
	Ritorna il numero delle variabili contenute 
	nel database
*/
static int NumVariabili(OlDatabaseTopologiaObject database)
{
int nvar = -1;

if(database==NULL)
	{
	XlWarning("OlDatabaseTopologia","NumVariabili","database non allocato");
	return(nvar);
	}

switch(database->oldatabaseTopologia.tipo_db){
	case(DB_XLRTF):
		nvar=numero_variabili (database->oldatabaseTopologia.id);
		break;
	case(DB_XLORACLE):
		break;
	}
return(nvar);
}

/*
	ritorna il massimo fra i due valori
*/
static int max(int a,int b)
{
if(a>b)
	return(a);
else
	return(b);
}
/* 
Funzione che legge nbyte dal file il cui descrittore e' file_handle,
a partire dall' offset specificato.
Restituisce il numero di bytes effettivamente letti.
*/
int rbyte(int file_handle,void * buffer,size_t offset,size_t nbyte)
{
int ii;
ssize_t rrbyte;


if(lseek(file_handle,offset,SEEK_SET) == -1)        {
        printf("rbyte: errore lseek\n");
        return(-1);
        }

if((rrbyte=read(file_handle, buffer,nbyte)) == -1)
        printf("rbyte: errore read\n ");
return(rrbyte);
}

/*
 Funzione che ricerca, nel file fnomi.rtf la variabile il cui nome e' puntato da
pbuf e restituisce nome ed indirizzo (point ed ext). 
Questa funzione utilizza un algoritmo di hashing basato sul nome della 
variabile da cercare.
*/
static Boolean ricerca(pbuf,point,ext,blocco,posiz,fp)

char *pbuf ; /* puntatore al nome del punto */
short *point ; /* indice  in data base */
short *ext  ; /* tipo di estensione */
long  *blocco; /* blocco in cui andra' inserito o e' inserito */
short *posiz ; /* posizione all'interno del blocco */
FILE *fp   ; /* canale open fnomi */
{

short somma,i,ii;
long toto ;
short j;
long tot ;
short newposiz,newblocco ;
short flow ;
short r_tamp[SCD_FATBLK/2];
short r_tampo[SCD_FATBLK/2];   /* buffer per overflow */
char *p1, *p2 ;
/*        calcolo il blocco in cui e' inserito il nome
*/
p1= (char *) & r_tamp[0];
p2 = (char *) & r_tampo[0] ;
flow=0 ;  /* non sono ancora nei blocchi di overflow */
newblocco=-1;  /* iniz. nuovo posto non trovato */

for(i=0,somma=0;i<SCD_SIGLA;i++)
        {
        char *kk = (char*)pbuf;
        /*printf("Valore di kk+i=%c\n",*(kk+i)); */
        somma = (somma << ONE_EIGHT) + *(kk+i);
        if((j=somma&HIGH_BITS)!=0)
                somma = (somma^(j>>THREE_QUARTERS))&~HIGH_BITS;
        }
somma = somma % BLK_NORM;

/*
        lettura da file fnomi
*/
      /*printf(" somma %d tot*SCD_FATBLK = %d\n",somma,tot*SCD_FATBLK) ;*/
tot=somma;rbyte(fileno(fp),r_tamp,tot*SCD_FATBLK,SCD_FATBLK) ;
/*
        ciclo di scansione del buffer per verificare
        se il nome esiste
*/
for(i=SCD_BLK_OVER_FLOW;i<(SCD_FATBLK)/2;i=i+SCD_RECORD)
{
if(!memcmp(r_tamp+i,pbuf,SCD_SIGLA))  /* variable found */
        {
                *point=r_tamp[i+SCD_SIGLA/2];
                *ext =r_tamp[i+SCD_SIGLA/2+1];
                *blocco=somma ;
                *posiz=i;
                return(True);
        }
}
return(False); /* tag not found */
}
/*
Metodo OlTagToIndS
*/
Boolean OlTagToIndS(OlDatabaseTopologiaObject database, char * tag,
                int *indirizzo, int *tipo)
{
return(TagToIndS(database,tag,indirizzo,tipo));
}
/*
        A partire dalla label della variabile di tipo SCADA, fornisce
        l'indirizzo della stessa
*/
static Boolean TagToIndS(OlDatabaseTopologiaObject database, char * tag,
                int *indirizzo, int *tipo)
{
char app_tag[MAX_LUN_NOME_VARS];
short punto,est,pos,i;
long block;

if(!database)
   return(False);
/*
per come e' concepita la funzione ricerca devo ripulire il buffer 
contenente il nome della variabile
*/
for(i=0;i<MAX_LUN_NOME_VARS;i++)
        app_tag[i]=0;
strcpy(app_tag,tag);
/*
Apro il file fnomi.rtf per ricavarmi la posizione del punto
*/
if(database->oldatabaseTopologia.fnomip==NULL)
    return(False);

/*
Chiamo la funzione ricerca :
Se variabile SCADA non trovata, ritorna False
*/
if(!ricerca(app_tag,&punto,&est,&block,&pos,database->oldatabaseTopologia.fnomip))
	return(False);
/*
A questo punto se tutto ha funzionato, ho la posizione ed
il tipo della variabile
*/
*tipo=est;
*indirizzo=punto;
return(True);
}
/*
Metodo OlTagToInd
*/
Boolean OlTagToInd(OlDatabaseTopologiaObject database, char * tag,
                int *indirizzo, int *tipo)
{
return(TagToInd(database,tag,indirizzo,tipo));
}
/*
	A partire dalla label della variabile fornisce
	l'indirizzo della stessa
*/
static Boolean TagToInd(OlDatabaseTopologiaObject database, char * tag,
		int *indirizzo, int *tipo)
{
int app_tipo;
Boolean ret=False;
int k,nvar;
char descr_err[100];
char app_tag[MAX_LUN_NOME_VAR];

sprintf(app_tag,"%-*s",MAX_LUN_NOME_VAR-1,tag);

sprintf(descr_err,"TagToInd (%s)",tag);

if(database==NULL)
	{
	XlWarning("OlDatabaseTopologia",descr_err,"database non allocato");
	return(ret);
	}

if(tag==NULL)
	{
	XlWarning("OlDatabaseTopologia",descr_err,"label non specificata");
	return(ret);
	}

switch(database->oldatabaseTopologia.tipo_db){
	case(DB_XLRTF):
		{
		nvar=NumVariabili(database);
		for(k=0;k<nvar;k++)
		   if(app_tag[0]==database->oldatabaseTopologia.variabili[k].nome[0])
			if (!strcmp (app_tag,
			    database->oldatabaseTopologia.variabili[k].nome))
					break;
		if(k<nvar)
			{
			memcpy(indirizzo,
			   &(database->oldatabaseTopologia.variabili[k].addr),
				sizeof(int));
#if defined SCADA_MMI
			if(database->oldatabaseTopologia.variabili[k].blocco<6)
                                app_tipo=(int)database->oldatabaseTopologia.
                                        variabili[k].blocco;
                        else
                                app_tipo=(int)database->oldatabaseTopologia.
                                        variabili[k].blocco+1;
#else
                        app_tipo=(int)database->oldatabaseTopologia.
					variabili[k].tipo_var;
#endif
			memcpy(tipo,&app_tipo,sizeof(int));
			ret=True;
			}
		break;
		}
	case(DB_XLORACLE):
		break;
	}
return(ret);
}

/*
	Ritorna il puntatore alla struttura variabili a partire
	dal progressivo
*/
static VARIABILI * ProgToVar(OlDatabaseTopologiaObject database, int prog)
{
VARIABILI * var = NULL;

if(database==NULL)
	{
	XlWarning("OlDatabaseTopologia","ProgToVar","database non allocato");
	return(var);
	}

switch(database->oldatabaseTopologia.tipo_db){
	case(DB_XLRTF):
		var= & database->oldatabaseTopologia.variabili[prog];
		break;
	case(DB_XLORACLE):
		break;
	}
return(var);
}

/*
	Ritorna il puntatore alla struttura variabili a partire
	dall'indirizzo
*/
static int IndToMod(OlDatabaseTopologiaObject database, int ind)
{
int var = -1;

if(database==NULL)
	{
	XlWarning("OlDatabaseTopologia","ProgToVar","database non allocato");
	return(var);
	}

switch(database->oldatabaseTopologia.tipo_db){
	case(DB_XLRTF):
		var=ind_modello (database->oldatabaseTopologia.id,ind,
			database->oldatabaseTopologia.variabili);
		break;
	case(DB_XLORACLE):
		break;
	}
return(var);
}

Boolean OlDestroyDatabaseTopologia(OlDatabaseTopologiaObject database)
{
if(database==NULL)
	{
	XlWarning("OlDatabaseTopologia","OlDestroyDatabase","database inesistente");
	return(False);
	}
/*
Se database e' solo per scada (usato solo per read fnomi.rtf) libera e ret
*/
if(database->oldatabaseTopologia.fnomip!=NULL)
   {
   fclose(database->oldatabaseTopologia.fnomip);
   }
if(!strcmp(database->oldatabaseTopologia.nome_database,"olscada"))
	{
	XtFree((char *)database);
	return(True);
	}
/*
	distrugge la shared memory
*/
if(sgancia_shrmem(database->oldatabaseTopologia.id)<0)
	return(False);
elimina_shrmem(database->oldatabaseTopologia.shmvar,
		database->oldatabaseTopologia.id,
		database->oldatabaseTopologia.size);
if(database->oldatabaseTopologia.nome_database)
	XtFree((char *)database->oldatabaseTopologia.nome_database);
XtFree((char *)database);

return(True);
}

/********************************************************
	Routine che crea una lista contenente la lista dei Componenti
	presenti nel file dei Component del database topologico
	passato come parametro:

char *OlGetComponent
	(
	database		--> Database della topologia del simulatore;
	)

	Valore di ritorno	--> Un elenco contenente in testa un intero
					con il numero dei componenti presenti,
					quindi una lista di nomi di 
					componenti dimensionati a 
					MAX_COMPONENT e terminata da un 
					elemento a NULL;
********************************************************/
char *OlGetComponent (OlDatabaseTopologiaObject database)
{
FILE *fComp;
char *ListaAppoggio,*ListaComp,*ListaDaDistruggere,*ListaRitorno;
int NumeroComp,i;
char *pret,LineaFile[100],Componente[25];
char *Asterischi="****";
int MAX_COMPONENT=25;

/*  Controllo se e' definito il file dei Component  */
if ( (strcmp (database->oldatabaseTopologia.file_comp,"None")) == 0 )
	{
	XlWarning("OlDatabaseTopologia","OlGetComponent","file name not defined");
	return (NULL);
	}

/*  COntrollo se esiste il fle dei Component  */
fComp = fopen (database->oldatabaseTopologia.file_comp,"r");
if ( fComp == NULL )
	{
	XlWarning("OlDatabaseTopologia","OlGetComponent","file not found");
	return (NULL);
	}

/*  Inizializzo il puntatore dalla lista  */
ListaComp = NULL;
NumeroComp = 1;

pret = fgets (LineaFile, 100, fComp);
while ( (pret != NULL) )
  {
  LineaFile[strlen (LineaFile)-1] = '\0';
  if ( strncmp (LineaFile,Asterischi,4) == 0 )
    {
    /*  Trovato Component  */
    pret = fgets (LineaFile, 100, fComp);
    if (pret != NULL)
      {
      LineaFile[strlen(LineaFile)-1]='\0';
      /*  Azzero la stringa del componente  */
      for (i=0;i<MAX_COMPONENT;i++)
	Componente[i]='\0';
      /*Compongo il nome del Component copiandolo in una stringa di appoggio*/
      i=23;
      while ( (LineaFile[i] == ' ') && (i>=0) )
        i--;
      if (i>=0) strncpy (Componente,LineaFile,i+1);
      if (Componente[0] != '\0')
        {
	ListaComp = XtRealloc (ListaComp,(NumeroComp*MAX_COMPONENT)*
						sizeof(char));
	ListaAppoggio = ListaComp;
	ListaComp += (NumeroComp-1)*MAX_COMPONENT;
	strcpy (ListaComp,Componente);
	ListaComp = ListaAppoggio;
	NumeroComp++;
	}
      }
    }
  pret = fgets (LineaFile, 100, fComp);
  }
  
/*  Costruisco la lista definitiva  */
NumeroComp--;
if (NumeroComp != 0)
  {
  /*  Alloco memoria per la lista definitiva con in testa 
	l'intero contenente il numero degli elementi della lista */
  ListaRitorno = XtCalloc ( sizeof(int)+(NumeroComp*MAX_COMPONENT), 
						sizeof(char) );
  ListaAppoggio = ListaRitorno;

  /*  Salvo il puntatore per poterlo liberare in uscita  */
  ListaDaDistruggere = ListaComp;

  /*  Memorizzo il numero degli elementi della lista in testa  */
  memcpy (ListaAppoggio,&NumeroComp,sizeof(int));
  ListaAppoggio += sizeof(int);
  for (i=0;i<NumeroComp;i++)
    {
    strcpy (ListaAppoggio,ListaComp);
    ListaAppoggio+=MAX_COMPONENT;
    ListaComp+=MAX_COMPONENT;
    }
  XtFree ((char *)ListaDaDistruggere);
  return (ListaRitorno);
  }
else return (NULL);
}

/********************************************************
	Routine che crea una lista contenente la lista dei SubSystem
	presenti nel file dei SubSystem del database topologico
	passato come parametro:

char *OlGetSubSystem
	(
	database		--> Database della topologia del simulatore;
	)

	Valore di ritorno	--> Un elenco contenente in testa un intero
					con il numero dei modelli presenti,
					quindi una lista di nomi di modelli
					dimensionate alla lunghezza di ogni
					stringa e terminata da un elemento 
					a NULL;
********************************************************/
char *OlGetSubSystem (OlDatabaseTopologiaObject database)
{
FILE *fSubS;
char *pret,LineaFile[100],SottoSistema[25];
char *ListaSubS,*ListaAppoggio,*ListaDaDistruggere,*ListaRitorno;
int i,NumeroSubS;
char *Asterischi="****";
int MAX_SUBSYSTEM=25;

/*  Controllo se e' definito il file dei SubSystem  */
if ( (strcmp (database->oldatabaseTopologia.file_subs,"None")) == 0 )
	{
	XlWarning("OlDatabaseTopologia","OlGetSubSystem","file name not defined");
	return (NULL);
	}

/*  Controllo se esiste il file dei SubSystem  */
fSubS = fopen (database->oldatabaseTopologia.file_subs,"r");
if ( fSubS == NULL )
	{
	XlWarning("OlDatabaseTopologia","OlGetSubSystem","file not found");
	return (NULL);
	}

/*  Inizializzo il puntatore alla lista  */
ListaSubS = NULL;
NumeroSubS = 1;

pret=fgets (LineaFile, 100, fSubS);
while ( (pret != NULL) )
  {
  LineaFile [strlen (LineaFile)-1] = '\0';
  if ( strncmp (LineaFile,Asterischi,4) == 0 )
    {
    /* Trovato inizio SubSystem  */
    pret = fgets (LineaFile, 100, fSubS);
    if (pret != NULL)
      {
      LineaFile[strlen(LineaFile)-1]='\0';
      /*  Azzero la stringa del subsystem  */
      for (i=0;i<MAX_SUBSYSTEM;i++)
	SottoSistema[i]='\0';
      /*Compongo il nome del SubSystem copiandolo in una stringa di appoggio*/
      i=23;
      while ( (LineaFile[i] == ' ') && (i>=0) )
        i--;
      if (i>=0) strncpy (SottoSistema,LineaFile,i+1);
      if (SottoSistema[0] != '\0')
        {
	ListaSubS = XtRealloc (ListaSubS,(NumeroSubS*MAX_SUBSYSTEM)*
						sizeof(char));
	ListaAppoggio = ListaSubS;
	ListaSubS += (NumeroSubS-1)*MAX_SUBSYSTEM;
	strcpy (ListaSubS,SottoSistema);
	ListaSubS = ListaAppoggio;
	NumeroSubS++;
	}
      }
    }
  pret = fgets (LineaFile, 100, fSubS);
  }

/*  Costruisco la lista definitiva  */
NumeroSubS--;
if (NumeroSubS != 0)
  {
  /*  Alloco memoria per contenere la lista definitiva con in testa
	l'intero contenente il numero degli elementi  */
  ListaRitorno = XtCalloc ( sizeof(int)+(NumeroSubS*MAX_SUBSYSTEM), 
						sizeof(char) );
  ListaAppoggio = ListaRitorno;

  /*  Salvo il puntatore per poterlo liberare in uscita  */
  ListaDaDistruggere = ListaSubS;

  /*  copio il numero degli elementi in testa alla lista  */
  memcpy (ListaAppoggio,&NumeroSubS,sizeof(int));
  ListaAppoggio += sizeof(int);
  for (i=0;i<NumeroSubS;i++)
    {
    strcpy (ListaAppoggio,ListaSubS);
    ListaAppoggio+=MAX_SUBSYSTEM;
    ListaSubS+=MAX_SUBSYSTEM;
    }
  XtFree ((char *)ListaDaDistruggere);
  return (ListaRitorno);
  }
else return (NULL);
}

/********************************************************
	Routine che crea una lista contenente la lista dei modelli
	presenti nel file dei SubSystem del database topologico 
	passato come parametro:

char *OlGetModelFromSubsystem 
	(
	database		--> Database della topologia del simulatore;
	SubS			--> Nome del Sottosistema di cui si vuole
					la lista dei modelli;
	)

	Valore di ritorno	--> Un elenco contenente in testa un intero
					con il numero dei modelli presenti,
					quindi una lista di nomi di modelli
					dimensionate a MAX_LUN_NOME_MODELLO
					terminata da un elemento a NULL;
**********************************************************/
char *OlGetModelFromSubSystem (OlDatabaseTopologiaObject database,char *SubS)
{
FILE *fSubS;
int lSubS;
char LFile[102],*ListaAppo,*NewListaModel,*ListaModel,*Model;
char *pret;
char *Lista;
char *Asterischi="****";
int NumMod,i,FineSubS;
Boolean Trovato;

lSubS = strlen (SubS);

/*  Controllo se e' definito il file dei SubSystem  */
if ( (strcmp (database->oldatabaseTopologia.file_subs,"None")) == 0 )
	{
	XlWarning("OlDatabaseTopologia","OlGetModelFromSubSystem","file name not defined");
	return (NULL);
	}

/*  Controllo se esiste il file dei SubSystem  */
fSubS = fopen (database->oldatabaseTopologia.file_subs,"r");
if ( fSubS == NULL )
	{
	XlWarning("OlDatabaseTopologia","OlGetModelFromSubSystem","file not found");
	return (NULL);
	}

/*  Inizializzo il puntatore alla lista  */
ListaModel = NULL;

Trovato = False;
pret=fgets (LFile, 100, fSubS);
if(pret!=NULL)
	LFile[strlen (LFile)-1] = '\0';
while ( (pret != NULL) && (!Trovato) )
  {
  if (strncmp (LFile,Asterischi,4) == 0)
    {
    /*  Trovato nuovo Sub System  */
    pret = fgets (LFile, 100, fSubS);
    if(pret!=NULL)
        LFile[strlen (LFile)-1] = '\0';
    else
	{
	printf("Errore di sintassi nel file dei SubSystem\n");
    	return(NULL);
	}
    /*  Controllo se il SubSystem trovato e' quello desiderato  */
    if (strncmp (LFile, SubS, lSubS) == 0)
      {
      /*  Trovato Sub System valido  */
      Trovato = True;
      NumMod = 1;
      FineSubS = 1;
      while (FineSubS != 0)
        {
        Model = LFile;
        Model += 24;  /*  Punta al nome del modello  */
        if (Model[0] != ' ')
	  {
          ListaModel = (char *)XtRealloc (ListaModel,
			(NumMod*MAX_LUN_NOME_MODELLO)*sizeof(char));
	  ListaAppo = ListaModel;
	  ListaAppo += (NumMod-1)*MAX_LUN_NOME_MODELLO;
          strncpy (ListaAppo,Model,MAX_LUN_NOME_MODELLO-1);
	  i = 0;
	  while (ListaAppo[i] != ' ')
	    i++;
	  ListaAppo[i] = '\0';
	  NumMod++;
	  }
	pret = fgets (LFile, 100, fSubS);
        if(pret)
        	LFile[strlen (LFile)-1] = '\0';
	else
		{
		printf("Errore di sintassi nel file dei SubSystem\n");
		return(NULL);
		}
	FineSubS = strncmp (LFile,Asterischi,4);
	}
      }
    }
  pret = fgets (LFile, 100, fSubS);
  if (pret != NULL)
    LFile[strlen (LFile)-1] = '\0';
  }

/*  Salvo il puntatore per poterlo liberare in uscita  */
ListaAppo = ListaModel;
NumMod--;

/*  Alloco memoria per la lista definitiva con in testa l'intero
	contenente il numero degli elementi della lista  */
NewListaModel = (char *)XtCalloc (sizeof(int) + (NumMod*MAX_LUN_NOME_MODELLO),
						sizeof(char));

Lista = NewListaModel;

/*  Copio il numero degli elementi in testa alla lista  */
memcpy (Lista, &NumMod, sizeof(int));
Lista += sizeof(int);
for (i=0; i<NumMod; i++)
  {
  strcpy (Lista,ListaAppo);
  Lista += MAX_LUN_NOME_MODELLO;
  ListaAppo += MAX_LUN_NOME_MODELLO;
  }
XtFree((char *)ListaModel);
return (NewListaModel);
}

/********************************************************
	Routine che crea una lista contenente la lista dei moduli
	presenti nel file dei Component del database topologico 
	passato come parametro:

char *OlGetModulFromComponent 
	(
	database		--> Database della topologia del simulatore;
	Comp			--> Nome del Componente di cui si vuole
					la lista dei moduli;
	)

	Valore di ritorno	--> Un elenco contenente in testa un intero
					con il numero dei moduli presenti,
					quindi una lista di nomi di moduli
					dimensionate a 4 caratteri
					terminata da un elemento a NULL;
**********************************************************/
char *OlGetModulFromComponent (OlDatabaseTopologiaObject database,char *Comp)
{
FILE *fComp;
int lComp;
char LFile[102],*ListaAppo,*NewListaModul,*ListaModul,*Modul;
char *pret;
char *Lista;
char *Asterischi="****";
int NumMod,i,FineComp;
Boolean Trovato;

lComp = strlen (Comp);

/*  Controllo se e' definito il file dei Component  */
if ( (strcmp (database->oldatabaseTopologia.file_comp,"None")) == 0 )
	{
	XlWarning("OlDatabaseTopologia","OlGetModulFromComponent","file name not defined");
	return (NULL);
	}

/*  Controllo se esiste il file dei Component  */
fComp = fopen (database->oldatabaseTopologia.file_comp,"r");
if ( fComp == NULL )
	{
	XlWarning("OlDatabaseTopologia","OlGetModulFromComponent","file not found");
	return (NULL);
	}

/*  Inizializzo il puntatore alla lista  */
ListaModul = NULL;

Trovato = False;
pret = fgets (LFile, 100, fComp);
if(pret!=NULL)
	LFile [strlen (LFile)-1] = '\0';
else
	{
	printf("Errore di sintassi nel file dei Component\n");
	return(NULL);
	}
while ( (pret != NULL) && (!Trovato) )
  {
  if (strncmp (LFile,Asterischi,4) == 0)
    {
    /*  Trovato nuovo Component */
    pret = fgets (LFile, 100, fComp);
    if(pret!=NULL)
    	LFile [strlen (LFile)-1] = '\0';
    else
        {
        printf("Errore di sintassi nel file dei Component\n");
        return(NULL);
        }

    if (strncmp (LFile, Comp, lComp) == 0)
      {
      /*  Trovato Component valido  */
      Trovato = True;
      NumMod = 1;
      FineComp = 1;
      while (FineComp != 0)
        {
        Modul = LFile;
        Modul += 24;  /*  Punta al nome del modulo  */
        if (Modul[0] != ' ')
	  {
          ListaModul = (char *)XtRealloc (ListaModul,
			(NumMod*5)*sizeof(char));
	  ListaAppo = ListaModul;
	  ListaAppo += (NumMod-1)*5;
          strncpy (ListaAppo,Modul,4);
	  NumMod++;
	  }
	pret = fgets (LFile, 100, fComp);
        if(pret!=NULL)
        	LFile [strlen (LFile)-1] = '\0';
	else
		{
        	printf("Errore di sintassi nel file dei Component\n");
		printf("Manca riga ****\n");
        	return(NULL);
		}

	FineComp = strncmp (LFile,Asterischi,4);
	}
      }
    }
  pret = fgets (LFile, 100, fComp);
  if(pret!=NULL)
      LFile [strlen (LFile)-1] = '\0';
  }

/*  Salvo il puntatore per poterlo liberare in uscita  */
ListaAppo = ListaModul;
NumMod--;

/*  Alloco memoria per contenere la lista definitiva con in testa
	l'intero conetenente il numero degli elementi  */
NewListaModul = (char *)XtCalloc (sizeof(int) + (NumMod*5),
						sizeof(char));

Lista = NewListaModul;

/*  Copio il numero degli elementi in testa alla lista  */
memcpy (Lista, &NumMod, sizeof(int));
Lista += sizeof(int);
for (i=0; i<NumMod; i++)
  {
  strncpy (Lista,ListaAppo,4);
  Lista += 5;
  ListaAppo += 5;
  }
XtFree((char *)ListaModul);
return (NewListaModul);
}

/*****************************************************
	Routine che controlla se il nome del blocco passato come 
	parametro e' valido nel modello passato come parametro;
	il Nome del blocco, se trovato, viene modificato in quanto
	gli viene assegnato l'intera stringa del nome del blocco:

Boolean OlIsBlockValid
	(
	database		--> Database della topologia del simulatore;
	Mod			--> Valore in input del nome del modello;
	Bloc			--> Valore in input del nome del blocco 
					da controllare;questo valore viene 
					modificato applicando la riga
					completa del nome del blocco;
	)

	Valore di ritorno	--> True se il nome del blocco esiste
					nel database caricato; False se
					il nome del blocco non e' valido;
*******************************************************/
Boolean OlIsBlockValid (OlDatabaseTopologiaObject database,char *Mod,
			char **Bloc)
{
VARIABILI *variabile;
int NumeroModTot,NumMod,NumVarTot;
int NumBloc,i,Lung,risul;
char *NomeBlocco,*ind,*NomeBloccoAppo;
Boolean Valido;

NumMod = 0;

NomeBloccoAppo = XtNewString (*Bloc);
XtFree ((char *)*Bloc);

/*  Ricavo l'indirizzo di inizio della shared memory  */
ind = database->oldatabaseTopologia.id;

/*  Ricavo il numero del modello selezionato  */
NumeroModTot = NumModelli (database);
NumMod = numero_modello (ind,NumeroModTot,Mod);

Lung = strlen (NomeBloccoAppo);
NumVarTot = NumVariabili (database);
i=0;
Valido = False;

while ( (i<NumVarTot) && (Valido == False) )
	{
	variabile = ProgToVar (database,i);
	if (variabile->mod == NumMod)
		{
		NomeBlocco = (char *)nome_blocco (ind,NumMod,variabile->blocco);
		risul = strncmp (NomeBlocco,NomeBloccoAppo,Lung);
		if (risul == 0)
			Valido = True;
		}
	i++;
	}

if (Valido == True) 
	{
	*Bloc = XtNewString (NomeBlocco);
	return (True);
	}
else
	{
	*Bloc = NULL;
	return (False);
	}
}

/********************************************************
	Routine che crea una lista contenente la lista dei blocchi
	appartenenti al modello specificato e facenti parte del 
	Sub System selezionato del database topologico passato
	come parametro:

char *OlGetBlocksFromSubSystem 
	(
	database		--> Database della topologia del simulatore;
	NomeMod			--> Nome del modello al quale i blocchi
					devono appartenere;
	SubS			--> Nome del Sottosistema di cui si vuole
					la lista dei modelli;
	)

	Valore di ritorno	--> Un elenco contenente in testa un intero
					con il numero dei blocchi presenti,
					quindi una lista di nomi di blocchi
					dimensionate a MAX_LUN_NOME_BLOCCO
					terminata da un elemento a NULL;
**********************************************************/
char *OlGetBlocksFromSubSystem (OlDatabaseTopologiaObject database,
					char *NomeMod,char *SubS)
{
FILE *fSubS;
int lNomeMod,lSubS,NumBlk;
char LFile[102],*Model,*Block;
char *ListaBlocchi,*NewListaBlocchi,*ListaAppo,*Lista,*ModelloDaLiberare;
char *Asterischi="****";
char *PerBlockValid;
int LunghezzaAppoggio,i;
Boolean Fatto,TrovatoModel,TrovatoSubS,BloccoValido;

lSubS = strlen (SubS);
lNomeMod = strlen (NomeMod);

/*  Controllo se e' definito il file dei SubSystem  */
if ( (strcmp (database->oldatabaseTopologia.file_subs,"None")) == 0 )
	{
	XlWarning("OlDatabaseTopologia","OlGetBlocksFromSubSystem","file name not defined");
	return (NULL);
	}

/*  Controllo se esiste il file dei SubSystem  */
fSubS = fopen (database->oldatabaseTopologia.file_subs,"r");
if ( fSubS == NULL )
	{
	XlWarning("OlDatabaseTopologia","OlGetBlocksFromSubSystem","file not found");
	return (NULL);
	}

/*  Inizializzo il puntatore alla lista   */
ListaBlocchi = NULL; 

Fatto = False;
NumBlk = 1;
TrovatoSubS = False;
TrovatoModel = False;
BloccoValido = False;

while ( (fgets(LFile,100,fSubS) != NULL) && (!Fatto) )
  {
  /*  Sostituisco il new line della riga letta dal file con uno 0  */
  LFile [strlen (LFile) - 1] = '\0';

  /*  Controllo se sono presenti gli asterischi per l'inizio di un 
	nuovo SubSystem  */
  if ( strncmp (LFile,Asterischi,4) == 0)
    {
    if (TrovatoSubS && TrovatoModel) 
      {
      Fatto=True;
      TrovatoSubS = False;
      TrovatoModel = False;
      }
    else 
      {
      TrovatoSubS = False;
      TrovatoModel = False;
      }
    }

  /*  Controllo se e' stato trovato il SubSystem interessato  */
  if ( strncmp(LFile,SubS,lSubS)==0 ) 
    TrovatoSubS = True;

  /*  Controllo se e' stato trovato il modello interessato  */
  if (TrovatoSubS && !TrovatoModel)
    {
    if (LFile[24] != ' ')
      {
      ModelloDaLiberare = XtNewString (LFile);
      Model = ModelloDaLiberare+24;
      if ( strncmp(Model,NomeMod,lNomeMod)==0 )
        {
        TrovatoModel = True;
        Block = XtNewString (LFile);
        PerBlockValid = XtNewString (Block + 39);
        LunghezzaAppoggio = strlen (NomeMod);
        i = LunghezzaAppoggio-1;
        while (NomeMod[i] == ' ') i--;
        NomeMod[i+1] = '\0';
        BloccoValido = True;
        XtFree ((char *)Block);
        }
      XtFree ((char *)ModelloDaLiberare);
      }
    }

  /*  Controllo se c'e' un altro modello  */
  if (TrovatoModel && LFile[24] == ' ')
    {
    Block = XtNewString (LFile);
    PerBlockValid = XtNewString (Block + 39);
    LunghezzaAppoggio = strlen (NomeMod);
    i = LunghezzaAppoggio-1;
    while (NomeMod[i] == ' ') i--;
    NomeMod[i+1] = '\0';
    BloccoValido = True;
    XtFree ((char *)Block);
    }

  /*  Controllo se sono finiti i blocchi del modello  */
  if (TrovatoModel && (LFile[24] != ' ') && !BloccoValido)
    TrovatoModel = False;

  /*  Controllo se puo' essere inserito nella lista il blocco  */
  if (BloccoValido)
    {
    if (OlIsBlockValid (database,NomeMod,&PerBlockValid))
      {
      ListaBlocchi = (char *)XtRealloc (ListaBlocchi,
		(NumBlk*MAX_LUN_NOME_BLOCCO)*sizeof(char));
      ListaAppo = ListaBlocchi;
      ListaAppo += (NumBlk-1)*MAX_LUN_NOME_BLOCCO;
      strcpy (ListaAppo,PerBlockValid);
      NumBlk++;
      XtFree ((char *)PerBlockValid);
      }
    BloccoValido = False;
    }
  }

/*  Salvo il puntatore per poterlo liberare in uscita  */
ListaAppo = ListaBlocchi;
NumBlk--;

/*  Alloco memoria per contenere la lista definitiva con in testa l'intero
	contenente il numero degli elementi della lista  */
NewListaBlocchi = (char *)XtCalloc (sizeof(int) + (NumBlk*MAX_LUN_NOME_BLOCCO),
						sizeof(char));

Lista = NewListaBlocchi;

/*  Copio il numero degli elementi in testa alla lista  */
memcpy (Lista, &NumBlk, sizeof(int));
Lista += sizeof(int);
for (i=0; i<NumBlk; i++)
  {
  strncpy (Lista,ListaAppo,MAX_LUN_NOME_BLOCCO-1);
  Lista += MAX_LUN_NOME_BLOCCO;
  ListaAppo += MAX_LUN_NOME_BLOCCO;
  }
XtFree((char *)ListaBlocchi);
return (NewListaBlocchi);
}

/********************************************************
	Routine che restituisce il nome della variabile di
	cambio colore di default per il modulo passato come parametro:

char *OlGetDefVarCC
	(
	database		--> Database della topologia del simulatore;
	NomeModulo		--> Nome del modulo del quale si vuole la 
					variabile di cambio colore;
	NomeBlocco		--> Blocco selezionato;
	NomeModello		--> Modello selezionato;
	)

	Valore di ritorno	--> Viene ritornata la variabile di
					cambio colore di default relativa al
					modello selezionato se trovata;
					viene ritornato altrimenti il
					valore NULL;

**********************************************************/
char *OlGetDefVarCC (OlDatabaseTopologiaObject database,char *NomeModulo,
			char *NomeBlocco,char *NomeModello)
{
FILE *fVarCC;
int lVarCC;
char *ind,LFile[102],*Ritorno;
char *pret;
int i,numeroVarTot,ProgModello,ProgBlocco;
int NumeroAppoggio;
VARIABILI *variabile;
Boolean Trovato,VarValid;

/*  Ricavo l'indirizzo di inizio della shared memory  */
ind = database->oldatabaseTopologia.id;

/*  Controllo se e' definito il file delle variabili di cambio colore  */
if ( (strcmp (database->oldatabaseTopologia.file_varcc,"None")) == 0 )
        {
        XlWarning("OlDatabaseTopologia","OlGetDefVarCC","file name not defined");
        return (NULL);
        }

/*  Controllo se esiste il file delle variabili di cambio colore  */
fVarCC = fopen (database->oldatabaseTopologia.file_varcc,"r");
if ( fVarCC == NULL )
        {
        XlWarning("OlDatabaseTopologia","OlGetDefVarCC","file not found");
        return (NULL);
        }

/*  Alloco memoria per contenere il nome della variabile di default  */
Ritorno = (char *)XtCalloc ( MAX_LUN_NOME_VAR+MAX_LUN_DESCR_VAR,sizeof(char));

VarValid = False;
Trovato=False;
pret = fgets (LFile, 100, fVarCC);
if(pret!=NULL)
  LFile[strlen(LFile)-1] = '\0';
else
  {
  printf("Errore di sintassi nel file delle VarCC\n");
  return(NULL);
  }
while ( (pret!=NULL) && (!Trovato) )
{
  /*  Controllo se il modulo letto corrisponde con quello desiderato  */
  if (strncmp (LFile,NomeModulo,4) == 0)
    {
    Trovato = True;
    NumeroAppoggio = NumModelli (database);
    numeroVarTot = NumVariabili(database);
    ProgModello = numero_modello (ind, NumeroAppoggio, NomeModello);
    ProgBlocco = numero_blocco (ind, ProgModello, NomeBlocco);
    i=0;
    while ( (i<numeroVarTot) && (VarValid != True) )
      {
      variabile = ProgToVar (database,i);
      if ( (variabile->mod==ProgModello) && 
		(variabile->blocco==ProgBlocco) )
	{
	if (strncmp (variabile->nome,&LFile[9],4)==0)
	  {
	  VarValid = True;
	  strcpy (Ritorno,variabile->nome);
	  strcat (Ritorno,variabile->descr);
	  }
	}
      i++;
      }
    }
  pret = fgets (LFile, 100, fVarCC);
  if(pret!=NULL)
    LFile[strlen(LFile)-1] = '\0';
}

return (Ritorno);
}

/********************************************************
	Routine che crea una lista contenente la lista dei modelli
	presenti nel database topologico passato come parametro:

char *OlElencoModelli
	(
	database		--> Database della topologia del simulatore;
	)

	Valore di ritorno	--> Un elenco contenente in testa un intero
					con il numero dei modelli presenti,
					quindi una lista di nomi di modelli 
					dimensionate a MAX_LUN_NOME_MODELLO
					terminata da un elemento a NULL;
**********************************************************/
char *OlElencoModelli (OlDatabaseTopologiaObject database)
{
char *appoggio,*p,*ind,*modelloappoggio;
int numeroModelli;
int i;

/*  Ricavo il numero totale di modelli  */
numeroModelli = NumModelli (database);

/*  Ricavo l'indirizzo di inizio della shared memory  */
ind = database->oldatabaseTopologia.id;

/*  Alloco memoria per contenere la lista dei modelli presenti con in testa
	l'intero contenente il numero di modelli presenti  */
appoggio = (char *)XtCalloc (sizeof(int)+(numeroModelli*MAX_LUN_NOME_MODELLO),
                                sizeof(char));
p = appoggio;

/*  Copio in testa alla lista il numero degli elementi   */
memcpy ( p, &numeroModelli, sizeof(int) );
p += sizeof(int);

for (i=1;i<numeroModelli+1;i++)
  {
  modelloappoggio = XtNewString ( (String) nome_modello (ind,i) );
  strncpy ( p, modelloappoggio, strlen ( modelloappoggio ));
  p += MAX_LUN_NOME_MODELLO;
  XtFree ((char *)modelloappoggio);
  }
return (appoggio);
}

/**********************************************************
	Routine che dato il nome del componente e il nome di un 
	modulo ritorno true se il modulo e compreso nel componente
	altrimenti false:

Boolean OlIsModuleValid
	(
	Componente		--> Nome del componente;
	Modulo			--> Nome del modulo;
	)

	Valore di ritorno	--> True se Modulo e' presente nella 
					lista dei moduli compresi in 
					Componente; False se Modulo
					non e' compreso nella lista dei
					moduli di Componente;

**********************************************************/
Boolean OlIsModuleValid (char *Componente,char *Modulo)
{
return (True);
}

/**********************************************************
	Routine che dato un modello ne ricava tutti i moduli presenti 
	e compone un lista contenente il nome di ogni modulo;

char *OlElencoModuli
	(
	database		--> Database della topologia del simulatore;
	nome_modello		--> nome del modello del quale si vuole
					la lista dei moduli;
	componente		--> nome del componente al quale i moduli
					trovati devono appartenere;
	)

	Valore di ritorno	--> Un elenco contenente in testa un intero
					con il numero dei moduli presenti,
					quindi una lista di nomi di moduli
					terminata da un elemento a NULL;
***********************************************************/ 

char *OlElencoModuli (OlDatabaseTopologiaObject database,char *NomeModello,
			char *componente)
{
char *ind,*NomeBlocco,NomeModulo[5];
int numeroBlocchi,numeroModelli,ProgModello;
int i,j,lung,uguali,Numero;
char *lista_parziale,*lista_second,*lista_appoggio,*ElencoModuli;

/*  Ricavo l'indirizzo di inizio della shared memory  */
ind = database->oldatabaseTopologia.id;

/*  Ricavo il numero totale di modelli  */
numeroModelli = NumModelli (database);

/*  Ricavo il progressivo del modello desiderato  */
ProgModello = numero_modello ( ind, numeroModelli, NomeModello );

/*  Ricavo il numero di blocchi per il modello selezionato  */
numeroBlocchi = numero_blocchi ( ind, ProgModello );

lista_appoggio = NULL;

for (j=1;j<numeroBlocchi+1;j++)
	{
	NomeBlocco = (char *)nome_blocco ( ind, ProgModello, j);
	NomeBlocco += 32;
	strncpy (NomeModulo,NomeBlocco,4);
	NomeModulo[4] = '\0';
	lista_parziale = lista_appoggio;
	uguali = 1;
	if (lista_parziale != NULL)
          {
	  while ( (lista_parziale[0] != '\0') && (uguali != 0) )
		{
		uguali = strcmp (NomeModulo,lista_parziale);
		if ( uguali != 0) 
			lista_parziale += 5;
		}
	  }
	if ( (uguali != 0) && OlIsModuleValid (componente,NomeModulo) )
		{
		Numero = 0;
		lista_second = lista_appoggio;
		if (lista_second != NULL)
		  {
		  while (lista_second[0] != '\0')
			{
			lista_second+= 5;
			Numero++;
			}
		  }
		lista_appoggio = (char *)XtRealloc (lista_appoggio,
			(((Numero+1)*5) + 1)*sizeof(char));
		lista_second = lista_appoggio;
		lista_second += (Numero*5);
		strcpy (lista_second,NomeModulo);
		lista_second[5] = '\0';
		}
	}


/*  Calcolo il numero totale di moduli trovati  */
Numero = 0;
lista_parziale = lista_appoggio;
while (lista_parziale[0] != '\0')
	{
	lista_parziale+= 5;
	Numero++;
	}

/*  Ricreo la lista totale con in testa l'intero contenente il 
	numero dei moduli presenti nella lista  */
lista_parziale = lista_appoggio;
ElencoModuli = (char *)XtCalloc ( sizeof(int)+(Numero*5),sizeof(char) );
lista_second = ElencoModuli;
memcpy (lista_second, &Numero, sizeof(int) );
lista_second += sizeof(int);
for (i=0;i<Numero;i++)
	{
	strncpy (lista_second, lista_appoggio, 4);
	lista_second += 5;
	lista_appoggio += 5;
	}
XtFree ((char *)lista_parziale);
return (ElencoModuli);
}

/**********************************************************
	Routine che dato il nome di un blocco e il nome di un modulo
	controlla se il blocco fa parte del modulo richiesto; se viene
	passato come NomeModulo NULL la routine non esegue alcun
	controllo e ritorna immediatamente un valore True:

Boolean OlFiltraBloccoConModulo
	(
	NomeBlocco		--> Nome del Blocco da esaminare;
	NomeModulo		--> Nome del modulo;
	)

	Valore di ritorno	--> True se NomeModulo = NULL;
					True se il NomeBlocco fa parte
					del modulo richiesto altrimenti
					False;

**********************************************************/
Boolean OlFiltraBloccoConModulo (char *NomeBlocco,char *NomeModulo)
{
int risul;

if (NomeModulo == NULL) return (True);
NomeBlocco += 32;
risul = strncmp (NomeBlocco,NomeModulo,4);
if (risul == 0)
	return (True);
else return (False);
}

/**********************************************************
	Routine che dato un modello e un modulo ricava tutti i blocchi
	che appartengono al modello ed al modulo;

char *OlElencoBlocchi
	(
	database		--> Database della topologia del simulatore;
	NomeModello		--> Nome del modello del quale si vuole la 
					lista di blocchi;
	NomeModulo		--> Nome del modulo al quale il blocco
					deve appartenere;
	)

	Valore di ritorno	--> Un elenco contenente in testa un intero 
					con il numero di blocchi che soddi-
					sfano la condizione e quindi una
					lista di nomi di blocchi terminata
					da un elemento a NULL;
*********************************************************/

char *OlElencoBlocchi (OlDatabaseTopologiaObject database,char *NomeModello,
					char *NomeModulo)
{
char *p,*p2,*ind,*lista_parziale,*lista_totale,*NomeBlocco;
int numeroModelli,numeroBlocchi,ProgModello,NumBlocchiTot;
int j;

/*  Ricavo l'indirizzo di inizio della shared memory  */
ind = database->oldatabaseTopologia.id;

/*  Ricavo il numero totale di modelli  */
numeroModelli = NumModelli (database);

NumBlocchiTot = 0;
numeroBlocchi = 0;

/*  Ricavo il progressivo del modello del quale si vogliono tutti
	i blocchi  */
ProgModello = numero_modello ( ind, numeroModelli, NomeModello );

/*  Ricavo il numero di blocchi per il modello selezionato  */
numeroBlocchi = numero_blocchi ( ind, ProgModello );

/*  Alloco memoria per contenere tuti i nomi dei blocchi del
	modello selezionato  */
lista_parziale = XtCalloc ((numeroBlocchi*MAX_LUN_NOME_BLOCCO),
				sizeof(char) );
p = lista_parziale;

for (j=1;j<numeroBlocchi+1;j++)
	{
	NomeBlocco = (char *)nome_blocco ( ind, ProgModello, j);
	if (OlFiltraBloccoConModulo (NomeBlocco,NomeModulo) )
		{
		strncpy (p, NomeBlocco, strlen (NomeBlocco) );
		p += MAX_LUN_NOME_BLOCCO;
		NumBlocchiTot++;
		}
	}

/*  Alloco memoria per la lista definitiva che contiene in testa 
	l'intero con il numero degli elementi della lista  */
lista_totale=(char *)XtCalloc(sizeof(int)+(NumBlocchiTot*MAX_LUN_NOME_BLOCCO),
					sizeof(char) );
p = lista_totale;

/*  Salvo il puntatore della lista_parziale per poterla liberare alla fine  */
p2 = lista_parziale;

memcpy (p, &NumBlocchiTot, sizeof(int));
p += sizeof(int);

for (j=0;j<NumBlocchiTot;j++)
	{
	strncpy ( p, lista_parziale, strlen (lista_parziale));
	p += MAX_LUN_NOME_BLOCCO;
	lista_parziale += MAX_LUN_NOME_BLOCCO;
	}
XtFree ((char *)p2);
return (lista_totale);
}

/*************************************************************
        Routine che controlla se la variabile passata come parametro
        e' valida per il tipo richiesto dal TipoFilter:

Boolean OlFilterVariable
        (
        var                     --> Variabile che deve essere esaminata;
        Tipofilter              --> Tipo di variabile al quale deve
                                        appartenere la variabile per
                                        essere valida;
        )

        Valore di ritorno       --> True se la variabile e' del tipo
                                        richiesto,altrimenti viene
                                        ritornato il valore False;
**************************************************************/
Boolean OlFilterVariable (VARIABILI *var,int Tipofilter)
{
switch (Tipofilter)
        {
        case XlRVarDO:
                if ( ((var->tipo == INGRESSO_NC) || (var->tipo == INGRESSO_C))
                        && (var->tipo_var == DIGITALE) )
                        return (True);
        break;
        case XlRVarDA:
                if ( (var->tipo == STATO) && (var->tipo_var == DIGITALE) )
                        return (True);
        break;
        case XlRVarAO:
                if ( ((var->tipo == INGRESSO_NC) || (var->tipo == INGRESSO_C))
                        && (var->tipo_var == ANALOGICO) )
                        return (True);
        break;
        case XlRVarAA:
                if ( (var->tipo == STATO) && (var->tipo_var == ANALOGICO) )
                        return (True);
        break;
        case XlRVarXA:
                if ( var->tipo == STATO )
                        return (True);
        break;
        case XlRVarXO:
                if ( (var->tipo == INGRESSO_NC) || (var->tipo == INGRESSO_C) )
                        return (True);
        break;
        case XlRVarAing:
                if ( (var->tipo == INGRESSO_NC) )
                        return (True);
        break;
        case XlRVarDX:
                if ( var->tipo_var == DIGITALE )
                        return (True);
        break;
        case XlRVarAX:
                if ( var->tipo_var == ANALOGICO )
                        return (True);
        break;
        case XlRVarPlot:
                return (True);
        case XlRVarXX:
                return (True);
        break;
        }
return (False);
}

/********************************************************
	Routine che ricava la lista delle variabili appartenenti 
	al modello,al blocco e al tipo passati come parametro:

char *OlElencoVariabili
	(
	database		--> Database della topologia del simulatore;
	NomeModello		--> nome del modello del quale si vuole
					l'lenco delle variabili;
	NomeBlocco		--> nome del blocco del quale si vuole
					l'elenco delle variabili;
	db_type			--> tipo di variabile considerata valida;
	)

	Valore di ritorno 	--> Un elenco contenente in testa un intero 
                                        con il numero di variabili che soddi-
                                        sfano la condizione e quindi una
                                        lista di nomi di variabili terminata
                                        da un elemento a NULL;
***********************************************************/

char *OlElencoVariabili (OlDatabaseTopologiaObject database,char *NomeModello,
				char *NomeBlocco,int db_type)
{
VARIABILI *variabile;
char *appoggio,*appo,*ListaVar,*p,*ind,*lista_parziale;
int numeroModelli,numeroBlocchi,ProgModello,ProgBlocco;
int numeroVarTot,numeroVarParz,NumBlocchiAppo;
int i,j,k,lung,NumeroAppoggio,ii,NuovaDimensione;
Boolean VarValida;

/*   Ricavo l'indirizzo di inizio della shared memory  */
ind = database->oldatabaseTopologia.id;

/*  Inizializzo il puntatore della lista  */
lista_parziale = NULL;

numeroVarTot = NumVariabili(database);
numeroVarParz=0;

if (NomeBlocco == NULL)
  {
  NumeroAppoggio = NumModelli (database);
  appoggio = XtNewString (NomeModello);
  ProgModello = numero_modello ( ind, NumeroAppoggio, appoggio );
  numeroBlocchi = numero_blocchi ( ind, ProgModello );

  for (j=1;j<numeroBlocchi+1;j++)
    {
    for (k=0;k<numeroVarTot;k++)
      {
      variabile = ProgToVar (database,k);
      if ( (variabile->mod==ProgModello) && (variabile->blocco==j) &&
                      OlFilterVariable (variabile,db_type) )
        {
        numeroVarParz++;
        NuovaDimensione = numeroVarParz*(MAX_LUN_NOME_VAR+MAX_LUN_DESCR_VAR);
        lista_parziale = XtRealloc (lista_parziale,NuovaDimensione);
        p = lista_parziale;
        p += ((numeroVarParz-1)*(MAX_LUN_NOME_VAR+MAX_LUN_DESCR_VAR));
        lung = strlen (variabile->nome);
        strcpy (p,variabile->nome);
        for (ii=lung;ii<MAX_LUN_NOME_VAR;ii++)
	p[ii] = ' ';
        p += MAX_LUN_NOME_VAR-1;
        lung = strlen (variabile->descr);
        strcpy (p,variabile->descr);
        p[lung] = '\0';
        }
      }
    }
  XtFree ((char *)appoggio);
  }
else
{
  memcpy (&NumBlocchiAppo,NomeBlocco,sizeof(int));
  NomeBlocco += sizeof(int);

  NumeroAppoggio = NumModelli (database);
  appoggio = XtNewString (NomeModello);
  ProgModello = numero_modello ( ind, NumeroAppoggio, appoggio );

  for (i=0;i<NumBlocchiAppo;i++)
    {
    ProgBlocco=numero_blocco(ind,ProgModello,NomeBlocco);
    if (ProgBlocco != 0)
      {
      for (k=0;k<numeroVarTot;k++)
        {
        variabile = ProgToVar (database,k);

        if ( (variabile->mod==ProgModello)&&(variabile->blocco==ProgBlocco)&&
             OlFilterVariable (variabile,db_type) )
          {
          numeroVarParz++;
          NuovaDimensione = numeroVarParz*(MAX_LUN_NOME_VAR+MAX_LUN_DESCR_VAR);
          lista_parziale = XtRealloc (lista_parziale,NuovaDimensione);
          p = lista_parziale;
          p += ((numeroVarParz-1)*(MAX_LUN_NOME_VAR+MAX_LUN_DESCR_VAR));
          lung = strlen (variabile->nome);
          strcpy (p,variabile->nome);
          for (ii=lung;ii<MAX_LUN_NOME_VAR;ii++)
            p[ii] = ' ';
          p += MAX_LUN_NOME_VAR-1;
          lung = strlen (variabile->descr);
          strcpy (p,variabile->descr);
          p[lung] = '\0';
          }
        }
      }
    NomeBlocco+= MAX_LUN_NOME_BLOCCO;
  }
  XtFree ((char *)appoggio);
}

/*  Alloco memoria per contenere la lista totale con in testa l'intero
	contenente il numero degli elementi della lista  */
ListaVar=(char *)XtCalloc(sizeof(int)+(numeroVarParz*(MAX_LUN_NOME_VAR+
		MAX_LUN_DESCR_VAR)),sizeof(char));
memcpy (ListaVar,&numeroVarParz,sizeof(int));
p = ListaVar;

/*  Salvo il puntatore per poterlo liberare alla fine  */
appo = lista_parziale;
p += sizeof(int);

for (i=0;i<numeroVarParz;i++)
	{
	strcpy (p,lista_parziale);
	p += (MAX_LUN_NOME_VAR+MAX_LUN_DESCR_VAR);
	lista_parziale += (MAX_LUN_NOME_VAR+MAX_LUN_DESCR_VAR);
	}
XtFree ((char *)appo);
return (ListaVar);
}

/*****************************************************
	Routine che controlla se il nome del modello passato come
	parametro e' valido oppure se il nome del modello e'
	incongruente:

Boolean OlIsModelValid
	(
	database		--> Database della topologia del simulatore;
	Mod			--> Valore in input del nome del modello
					da controllare;
	)

	Valore di ritorno	--> True se il nome del modello esiste
					nel database caricato; False se
					il nome del modello non e' valido;
********************************************************/
Boolean OlIsModelValid (OlDatabaseTopologiaObject database,char *Mod)
{
int NumeroTot,NumMod;
char *ind;

NumMod = 0;

/*  Ricavo l'indirizzo di inizio della shared memory  */
ind = database->oldatabaseTopologia.id;
NumeroTot = NumModelli (database);
NumMod = numero_modello (ind,NumeroTot,Mod);

if (NumMod == 0) 
	return (False);
else 	return (True);
}
/******************************************************
	Routine che controlla se il nome della variabile passata
	come parametro e' contenuta all'interno del modello e del 
	blocco passati come parametri; se valido il nome della variabile
	viene modificato in quanto viene completato con la descrizione
	della variabile:

Boolean OlIsVariableValid
	(
	database		--> Database della topologia del simulatore; 
	Mod			--> Valore in input del nome del modello;
	Bloc			--> Valore in input del nome del blocco;
	Var			--> Valore in input del nome della variabile
					da controllare;viene modificato se
					trovata nel database;
	)

	Valore di ritorno	--> True se il nome della variabile e'
					valido; False se il nome della 
					variabile non e' contenuto nel 
					database;
*********************************************************/
Boolean OlIsVariableValid (OlDatabaseTopologiaObject database,char *Mod,
			char *Bloc,char **Var)
{
int risul,NumVarTot,i,Lung,j,IndMod,IndBloc,NumModTot;
VARIABILI *variabile;
Boolean Valida;
char *Appoggio,*ind;
char app1[MAX_LUN_NOME_VAR]; /* appoggio per gestire sigle di
                                lunghezza differente (fill
              			con blank dell'appoggio */

ind = database->oldatabaseTopologia.id;

NumModTot = NumModelli(database);
IndMod = numero_modello (ind , NumModTot , Mod);
IndBloc = numero_blocco (ind , IndMod , Bloc);

NumVarTot = NumVariabili (database);
/** Lore
i = 1;
**/
i=0;
Valida = False;

while ( (i<NumVarTot+1) && (Valida == False) )
	{
	variabile = ProgToVar (database, i);
	if ( (variabile->mod == IndMod) && (variabile->blocco == IndBloc))
		{
		Lung = strlen (*Var);
		strcpy(app1,*Var);
		fill_space(app1,MAX_LUN_NOME_VAR-1);
		risul = strcmp (variabile->nome,app1);
		if (risul == 0)
			{
			Valida = True;
			Appoggio = (char *)XtCalloc ( (MAX_LUN_NOME_VAR+
					MAX_LUN_DESCR_VAR),sizeof(char));
			strcpy (Appoggio, variabile->nome);
			Lung = strlen (Appoggio);
			for (j=Lung;j<MAX_LUN_NOME_VAR;j++)
				Appoggio[j] = ' ';
			strcat (Appoggio, variabile->descr);
			}
		}
	i++;
	}

if (Valida)
	{
	*Var = Appoggio;
	return (True);
	}
else
	{
	XtFree ((char *)Appoggio);
	*Var = XtCalloc(1,sizeof(char));
	return (False);
	}
}

/*****************************************************
        Routine che legge la stringa contenente la descrizione
        della variabile con la perturbazione ad essa associata
        e restituisce i pezzetti contenenti il nome della variabile,
        il nome del modello, il nome del blocco, la perturbazione ed
        eventuali altri parametri riguardanti la perturbazione:

Boolean OlReadVariableString
        (
        database                --> Database della topologia del simulatore;
        Stringa                 --> Stringa contenente la descrizione
                                        completa;
        Modello                 --> Valore di ritorno in cui viene caricato
                                        il nome del modello di appartenenza;
        Blocco                  --> Valore di ritorno in cui viene caricato
                                        il nome del blocco di appartenenza;
        Variabile               --> Valore di ritorno in cui viene caricato
                                        il nome della variabile;
        Perturba                --> Valore di ritorno in cui viene caricato
                                        la descrizione completa della
                                        perturbazione eseguita sulla
                                        variabile;
	Unita			--> Valore di ritorno in cui viene caricato
					la descrizione completa della unita
					di misura della variabile;
	TipoVar 		--> Tipo della variabile nel database;
        )

        Valore di ritorno       --> Indica se l'operazione ha avuto successo;
                                        se le informazioni contenute nella
                                        stringa in input non trovano ri-
                                        scontro nel database viene tornato
                                        il valore False e i puntatori delle
                                        stringhe di ritorno vengono ritornate
                                        a NULL;
*******************************************************/

Boolean OlReadVariableString (OlDatabaseTopologiaObject database,char *Stringa,
		char **Modello,char **Blocco,char **Variabile,char **Perturba,
		char **Unita,int TipoVar)
{
char *AppoggioVariabile,*AppoggioModello,*AppoggioBlocco;
char *AppoggioPerturba,*AppoggioUnita,Appoggio[40];
char *AppoggioStringa;
int indice;

/*  Allocazione stringhe di appoggio e di ritorno valore  */
AppoggioModello = (char *)XtCalloc (MAX_LUN_NOME_MODELLO,sizeof(char));
AppoggioBlocco = (char *)XtCalloc (MAX_LUN_NOME_BLOCCO,sizeof(char));
AppoggioVariabile = (char *)XtCalloc ( (MAX_LUN_NOME_VAR+MAX_LUN_DESCR_VAR),
                                sizeof(char));
AppoggioPerturba = (char *)XtCalloc (40,sizeof(char));
AppoggioUnita = (char *)XtCalloc (40,sizeof(char));

AppoggioStringa = Stringa;

/*  Scansione della stringa in input  */
if ( (Stringa != NULL) && (Stringa[0] != '\0') )
{
indice = 1;
while ( (sscanf(Stringa,"%s",Appoggio)>0) && (indice<6) )
  {
  switch (indice)
    {
    case 1:
      strcpy (AppoggioVariabile,Appoggio);
      Stringa += strlen (Appoggio)+1;
      indice++;
    break;
    case 2:
      strcpy (AppoggioBlocco,Appoggio);
      Stringa += strlen (Appoggio)+1;
      indice++;
    break;
    case 3:
      strcpy (AppoggioModello,Appoggio);
      Stringa += strlen (Appoggio)+1;
      indice++;
    break;
    case 4:
      if ( (TipoVar==XlRVarDA)||(TipoVar==XlRVarAA)||(TipoVar==XlRVarXA) ||
		(TipoVar==XlRVarXX)||(TipoVar==XlRVarAX)||(TipoVar==XlRVarDX)||
		(TipoVar==XlRVarAing) || (TipoVar==XlRVarPlot))
	{
	strcpy (AppoggioPerturba,Appoggio);
	Stringa += strlen (Appoggio)+1;
	}
      if ( (TipoVar==XlRVarDO)||(TipoVar==XlRVarAO)||(TipoVar==XlRVarXO) )
        {
	strcpy (AppoggioPerturba,Appoggio);
	Stringa += strlen (Appoggio)+1;
	strcat (AppoggioPerturba," ");
	if ( sscanf (Stringa,"%s",Appoggio) > 0 )
	  {
	  strcat (AppoggioPerturba,Appoggio);
	  Stringa += strlen (Appoggio)+1;
	  strcat (AppoggioPerturba," ");
	  }
	if ( sscanf (Stringa,"%s",Appoggio) > 0 )
	  {
	  strcat (AppoggioPerturba,Appoggio);
	  Stringa += strlen (Appoggio)+1;
	  }
        }
      indice++;
    break;
    case 5:
      strcpy (AppoggioUnita,Appoggio);
      Stringa += strlen (Appoggio)+1;
      strcat (AppoggioUnita," ");
      if ( sscanf (Stringa,"%s",Appoggio) > 0 )
	{
        strcat (AppoggioUnita,Appoggio);
        Stringa += strlen (Appoggio)+1;
        strcat (AppoggioUnita," ");
	}
      if ( sscanf (Stringa,"%s",Appoggio) > 0 )
	{
	strcat (AppoggioUnita,Appoggio);
        Stringa += strlen (Appoggio)+1;
	}
      indice++;
    break;
    }
  }
/*  Stringa -= (Stringa - AppoggioStringa); */

*Perturba = AppoggioPerturba;
*Unita = AppoggioUnita;
}
else
{
/* La stringa della definizione della variabile e' vuota */
XtFree ((char *)AppoggioModello);
XtFree ((char *)AppoggioBlocco);
XtFree ((char *)AppoggioVariabile);
XtFree ((char *)AppoggioPerturba);
XtFree ((char *)AppoggioUnita);
*Modello = XtCalloc (1,sizeof(char));
*Blocco = XtCalloc (1,sizeof(char));
*Variabile = XtCalloc (1,sizeof(char));
*Perturba = XtCalloc (1,sizeof(char));
*Unita = XtCalloc (1,sizeof(char));
return (False);
}

if ( OlIsModelValid (database,AppoggioModello) == True )
        *Modello = AppoggioModello;
else 
	{
	XtFree ((char *)AppoggioModello);
	XtFree ((char *)AppoggioBlocco);
	XtFree ((char *)AppoggioVariabile);
	*Modello = XtCalloc (1,sizeof(char));
	*Blocco = XtCalloc (1,sizeof(char));
	*Variabile = XtCalloc (1,sizeof(char));
	return (False);
	}

if ( OlIsBlockValid (database,AppoggioModello,&AppoggioBlocco) == True )
        *Blocco = AppoggioBlocco;
else 
	{
	XtFree ((char *)AppoggioBlocco);
	XtFree ((char *)AppoggioVariabile);
	*Blocco = XtCalloc (1,sizeof(char));
	*Variabile = XtCalloc (1,sizeof(char));
	return (False);
	}

if ( OlIsVariableValid (database,AppoggioModello,AppoggioBlocco,
                                        &AppoggioVariabile) == True)
        *Variabile = AppoggioVariabile;
else
	{
	XtFree ((char *)AppoggioVariabile);
	*Variabile = XtCalloc (1,sizeof(char));
	return (False);
	}
return (True);
}

/**************************************************************
    Routine che ricava dal nome della variabile e dal nome del modello
	il nome del blocco associato;

char *OlGetBlockName
	(
	database			--> Database della topologia;
	NomeModello			--> Nome del modello;
	NomeVariabile			--> Nome della variabile;
	)

	Valore di ritorno		--> Viene restituito, se viene
						trovato, il nome del 
						blocco relativo alla
						variabile ed al modello
						passati come parametro;

**************************************************************/
char *OlGetBlockName (OlDatabaseTopologiaObject database,char *NomeModello,
			char *NomeVariabile)
{
char *ind;
int i,uguali,TotaleVariabili,TotaleModelli,IndiceModello;
VARIABILI *Variable;

/*  Ricava l'indice di inizio della shared memory  */
ind = database->oldatabaseTopologia.id;

/*  Ricava l'indice del modello NomeModello  */
TotaleModelli = NumModelli(database);
IndiceModello = numero_modello (ind , TotaleModelli , NomeModello);

/*  Ricava il numero totale di variabili del database  */
TotaleVariabili = NumVariabili(database);

i=0;
while ( i<TotaleVariabili )
  {
  Variable = ProgToVar (database,i);
  uguali = strncmp (Variable->nome,NomeVariabile,strlen(NomeVariabile));
  if ( (uguali==0) && (Variable->mod==IndiceModello) )
    return (XtNewString ((String) nome_blocco(ind,IndiceModello,Variable->blocco)));
  i++;
  }
return (NULL);
}

/**************************
**************************/
char *OlGetNameVar (Widget Wdg,char *NomeVar,int Numero)
{
int k,i,IndiceVariabile,IndiceModello,uguali;
int TotaleVariabili;
XlGraficoWidget w = (XlGraficoWidget)Wdg;
OlDatabaseTopologiaObject Data;
char *Ritorno;
VARIABILI *Variabile;
Boolean Trovata;

Ritorno = (char *)calloc (MAX_LUN_NOME_VAR+MAX_LUN_DESCR_VAR,sizeof(char));

Data = xlGraficoClassRec.xlmanager_class.database_topologia;
IndiceModello = w->grafico.input[Numero].pdb.nmod;

/*  Ricava il numero totale di variabili del database  */
TotaleVariabili = NumVariabili(Data);

Trovata = False;
i=0;
while ( (i<TotaleVariabili) && (!Trovata) )
  {
  Variabile = ProgToVar (Data,i);
/**
  uguali = strncmp (Variabile->nome,NomeVar,strlen(NomeVar));
**/
  uguali = confVar(Variabile->nome,NomeVar);
  if ( (uguali==0) )
    {
    strcpy (Ritorno,Variabile->nome);
    k = strlen (Ritorno);
    while (k<MAX_LUN_NOME_VAR)
      {
      Ritorno[k] = ' ';
      k++;
      }
    strcat (Ritorno,Variabile->descr);
    Trovata = True;
    }
  i++;
  }

if (Trovata)
  return (Ritorno);
else
  return (NULL);
}

Boolean OlGetModelBlockName (OlDatabaseTopologiaObject database,char *var,
			char *Modello, char *Blocco)
{
char *ind;
int i,TotaleVariabili;
VARIABILI *Variable;
char *app_char;




/*  Ricava l'indice di inizio della shared memory  */
ind = database->oldatabaseTopologia.id;


/*  Ricava il numero totale di variabili del database  */
TotaleVariabili = NumVariabili(database);

i=0;
while ( i<TotaleVariabili )
  {
  Variable = ProgToVar (database,i);
  if (strncmp (Variable->nome,var,strlen(var))==0)
	{
    	strcpy(Blocco,(char *)nome_blocco(ind,Variable->mod,Variable->blocco));
    	strcpy(Modello,(char *)nome_modello(ind,Variable->mod));
	app_char=strstr(Blocco," ");
	if(app_char!=NULL)
		*app_char=0;
	return(True);
	}
  i++;
  }
return(False);
}

Boolean OlVarIsFreeInput(OlDatabaseTopologiaObject database,char *var)
{
Boolean ret = False;
char *ind;
int i,TotaleVariabili;
VARIABILI *Variable;
char app_var[200];
char *app_char;


if(database==NULL)
	return(False);
if(var==NULL)
	return(False);

strcpy(app_var,var);
app_char=strstr(app_var," ");
if(app_char!=NULL)
	*app_char=0;


/*  Ricava l'indice di inizio della shared memory  */
ind = database->oldatabaseTopologia.id;


/*  Ricava il numero totale di variabili del database  */
TotaleVariabili = NumVariabili(database);

i=0;
while ( i<TotaleVariabili )
  {
  Variable = ProgToVar (database,i);
  if (strncmp (Variable->nome,app_var,strlen(app_var))==0)
	{
	if (Variable->tipo==INGRESSO_NC)
		return(True);
	}
  i++;
  }
return(ret);
}


int confVar(char *ing1, char *ing2)
{
char v1[MAX_LUN_NOME_VAR];
char v2[MAX_LUN_NOME_VAR];
char *app;

if((ing1 == NULL) || (ing2 == NULL))
        return(-1);

memset(v1,0,MAX_LUN_NOME_VAR);
memset(v2,0,MAX_LUN_NOME_VAR);

strncpy(v1,ing1,MAX_LUN_NOME_VAR-1);
strncpy(v2,ing2,MAX_LUN_NOME_VAR-1);
v1[MAX_LUN_NOME_VAR-1]=0;
v2[MAX_LUN_NOME_VAR-1]=0;
app=strstr(v1," ");
if(app!=NULL) *app=0;
app=strstr(v2," ");
if(app!=NULL) *app=0;

return(strcmp(v1,v2));
}
