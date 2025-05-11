/**********************************************************************
*
*       C Source:               co_main.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Sep 27 10:00:27 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: co_main.c-11 %  (%full_filespec: co_main.c-11:csrc:1 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)co_main.c	5.2\t12/15/95";
/*
   modulo co_main.c
   tipo 
   release 5.2
   data 12/15/95
   reserved @(#)co_main.c	5.2
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sim_param.h"
#include "sim_types.h"
#include "libnet.h"
#include <sqlite3.h>


typedef struct strin_st {
  char *stringa;
  int lun_stringa;
  } STRIN_ST;

typedef struct collega {
  char coll [MAX_COLL] [8];
  } COLLEG_ST;

int id_sh;                     /* identificatore sh_var */
VARIABILI *variabili;        /* database variabili di tutti i modelli */
char *ind_sh_top;            /* puntatore inizio shared memory sh_var */
int shr_usr_key;                /* chiave utente per shared  */

VARLEGO *varlego;
S02 s02_;
FILE *fp_s01, *fp_s04, *fp_s05, *fp_n04, *fp;
int riga_s01;
int riga_n04;
COLLEGAMENTO collegamento[MAX_MODEL][MAX_MODEL];
SCAMBIO scambio[MAX_MODEL][MAX_DT_DEC];
char macro_modello [11];
int lun_macro_modello;
char descr_macro_modello [65];
int nmod,nrip;
char path_svil[MAX_MODEL][80];
int swap_disk=0;
int speed_version=0;
sqlite3 *db;
char sigla_tipo_task[16];

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

int legge_riga( char *riga, int *lun);
void separa_str( char *riga, int lun, int nstr, STRIN_ST strin[]);
void check_dt( MODEL model[], int nmod, int *nrip);
void tabella( MODEL model[], TABEL tabel[], int nmod, int nrip);
void tabelle_net(int nmod, int nrip);
void read_f04_write_s04( VARLEGO *varlego, int i, MODEL model[]);
void read_n04_write_s04( VARLEGO *varlego, int i, MODEL model[]);
void check_input ( char *stringa, int imi, int *ii,VARLEGO *varlego, MODEL *model);
void check_model ( char *stringa, int imi, int *imu, MODEL *model, int nmod);
void check_output( char *stringa, int imu, int *iu,VARLEGO *varlego, MODEL *model);

COLLEG_ST colleg [MAX_MODEL];


char **cdim2(int,int);

main(argc,argv) 
int argc;
char **argv;
{

typedef struct collega_buffer {
  int input;
  int output;
  } COLLEGA_BUFFER_ST;
COLLEGA_BUFFER_ST collega_buffer [MAX_COLL];

char riga [100];
STRIN_ST strin [10];
char sistema_operativo[10];
int lun, nstr, i, iapp, ncoll, ntotcoll, imi, imu, ii, iu, k;
double tempo,gettim();
FILE *fp;
char *zErrMsg = 0;
char *SqlStatement;
int rc;
int task_offset;
int sked_distribuito=0;
char macchina_target[FILENAME_MAX];

/* redirezione output  e stampa versione */
testata("net_compi",SccsID);

tempo=gettim();

/*
	prelega dall'environment i parametri caratteristici del lego
*/
if(!GetParLego())
 exit( fprintf(stderr, "ATTENZIONE: i parametri del LEGO non sono definiti"));

if(argc>1)
	{
	if(strcmp(argv[1],"-s")==0)
		{
		swap_disk=1;
		printf("Versione low-memory\n");
		}
	if(strcmp(argv[1],"-speed")==0)
		{
		speed_version=1;
		printf("Versione veloce (non ricrea variabili.edf/rtf)\n");
		}
	}


/*
if(speed_version)
	read_S02_net();
*/

/* ripulisce la directory dai file pre esistenti */
#if defined UNIX
if (!(fp = fopen( "S02_AIX", "r")))
        close(fp);
else
        system("rm S02_AIX");
if (!(fp = fopen( "S02_VMS", "r")))
        close(fp);
else
        system("rm S02_VMS");
if (!(fp = fopen( "S02_ULTRIX", "r")))
        close(fp);
else
        system("rm S02_ULTRIX");
if(!speed_version)
{
	if (!(fp = fopen( "variabili.rtf", "r")))
        	close(fp);
	else
#if defined UNIX
        	system("rm variabili.rtf");
#endif
#if defined VMS
		system("delete/noconf variabili.rtf;*");
#endif
	if (!(fp = fopen( "variabili.edf", "r")))
        	close(fp);
	else
#if defined UNIX
        	system("rm variabili.edf");
#endif
#if defined VMS
		system("delete/noconf variabili.edf;*");
#endif
}
if (!(fp = fopen( "recorder.rtf", "r")))
        close(fp);
else
        system("rm recorder.rtf");
#endif
#if defined VMS
if (!(fp = fopen( "S02_AIX", "r")))
	close(fp);
else
	system("delete/noconf S02_AIX;*");
if (!(fp = fopen( "S02_VMS", "r")))
	close(fp);
else
	system("delete/noconf S02_VMS;*");
if (!(fp = fopen( "S02_ULTRIX", "r")))
	close(fp);
else
	system("delete/noconf S02_ULTRIX;*");
if(!speed_version)
{
	if (!(fp = fopen( "variabili.rtf", "r")))
		close(fp);
	else
		system("delete/noconf variabili.rtf;*");
	if (!(fp = fopen( "variabili.edf", "r")))
		close(fp);
	else
		system("delete/noconf variabili.edf;*");
}
if (!(fp = fopen( "recorder.rtf", "r")))
	close(fp);
else
	system("delete/noconf recorder.rtf;*");
#endif

/* Apertura database algrt.db3 sqlite se esite la variabile di ambiente
   ALGRTDB=YES */ 

db=NULL;
if ( getenv("ALGRTDB") != NULL )
{
   if ( !strcmp( getenv("ALGRTDB"),"YES" ) )
   {
      if (system("if [ -f algrt_new.db3 ]; then rm algrt_new.db3; fi") == -1)
	 {
	   perror("Rimozione  algrt_new.db3");
	   exit(7);
         }
      if (system("cat ${LEGORT_BIN}/algrt_db.sql | sqlite3 algrt_new.db3") == -1)
	 {
	   perror("Creazione algrt_new.db3");
	   exit(7);
         }
      rc = sqlite3_open("algrt_new.db3", &db);
      if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
      }
   }
}

/* Se db != NULL gestisco il caricamento del database */
if ( db != NULL )
{
   /* Inizio di una transazione per velocizzare */ 
   rc = sqlite3_exec(db, "BEGIN;", 0, 0, &zErrMsg);
   if( rc!=SQLITE_OK ){
     fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
   }

   /* Test di funzionamento del database algrt.db3 */ 

   SqlStatement = sqlite3_mprintf("delete from PARAMETRI where sigla='%q';", "merda");
   rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
   if( rc!=SQLITE_OK ){
     fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
   }
   sqlite3_free(SqlStatement);

   SqlStatement = sqlite3_mprintf("insert into PARAMETRI values ('%q', '%q');", "merda", "cacca");
   rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
   if( rc!=SQLITE_OK ){
     fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
   }
   sqlite3_free(SqlStatement);

   /* Pulizia delle tabelle principali di algrt.db3 */ 

   SqlStatement = sqlite3_mprintf("delete from VARIABILI ;");
   rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
   if( rc!=SQLITE_OK ){
     fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
   }
   sqlite3_free(SqlStatement);

   SqlStatement = sqlite3_mprintf("delete from BLOC_VAR ;");
   rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
   if( rc!=SQLITE_OK ){
     fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
   }
   sqlite3_free(SqlStatement);

   SqlStatement = sqlite3_mprintf("delete from BLOCCHI ;");
   rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
   if( rc!=SQLITE_OK ){
     fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
   }
   sqlite3_free(SqlStatement);

   SqlStatement = sqlite3_mprintf("delete from MODELLI ;");
   rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
   if( rc!=SQLITE_OK ){
     fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
   }
   sqlite3_free(SqlStatement);

   SqlStatement = sqlite3_mprintf("delete from PARAMETRI ;");
   rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
   if( rc!=SQLITE_OK ){
     fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
   }
   sqlite3_free(SqlStatement);
}
/* FINE Apertura database algrt.db3 sqlite se esite la variabile di ambiente
   ALGRTDB=YES */ 



/* Inizializzazione della struttura STRIN_ST */ 
for(i=0; i<10; i++)
   strin[i].stringa = NULL;

/*	apre S01 */
if (!(fp_s01 = fopen( "S01", "r")))
 exit( fprintf(stderr,
 "ATTENZIONE non esiste il file S01.DAT"));
riga_s01 = 0;
/*	lettura prima riga di S01.DAT */
legge_riga( riga, &lun);
if (strncmp( riga, "****", 4))
 exit( fprintf(stderr,
 "ATTENZIONE la prima riga del file S01 deve contenere 4 asterischi"));

/*	lettura seconda riga di S01.DAT (NOME MACRO_MODELLO) */
legge_riga( riga, &lun);
separa_str( riga, lun, nstr=2, strin);
if (strin[0].lun_stringa > 10)
 exit( fprintf(stderr,
 "ATTENZIONE il nome del MACRO-modello non deve essere piu` lungo di 10 caratteri"));
strcpy( macro_modello, strin[0].stringa);
lun_macro_modello = strin[0].lun_stringa;
strcpy( s02_.macro_modello, strin[0].stringa);
s02_.lun_macro_modello = strin[0].lun_stringa;


/* Se db != NULL gestisco Scrittura nome macromodello in sqlite */
if ( db != NULL )
{
   SqlStatement = sqlite3_mprintf("insert into PARAMETRI values ('%q', '%q');", "MACROMODELLO", macro_modello);
   rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
   if( rc!=SQLITE_OK ){
     fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
   }
   sqlite3_free(SqlStatement);
}
/* FINE Se db != NULL gestisco Scrittura nome macromodello in sqlite */




/*
if (strin[1].lun_stringa
 && (i = strin[1].stringa - riga + strin[1].lun_stringa) < lun)
  riga[i] = ' ';*/
strcpy( descr_macro_modello, strin[1].stringa);
printf( "\n");
printf( "********************************************\n\n");
printf( "ACQUISIZIONE DATI DEL FILE S01.DAT\n\n");
printf( "********************************************\n");
printf( "Nome MACRO-modello :   %s\n", macro_modello);
printf( "    Descrizione    :   %s\n", descr_macro_modello);
printf( "********************************************\n");
/*	lettura terza riga di S01.DAT */

legge_riga( riga, &lun);
if (strncmp( riga, "****", 4))
 exit( fprintf(stderr,
 "ATTENZIONE la linea dopo il nome del MACRO-modello deve contenere 4 asterischi"));

/*	lettura dei NOMI dei modelli */

nmod = 0;
do {
  legge_riga( riga, &lun);
  if (! strncmp( riga, "****", 4))
    break;
  separa_str( riga, lun, nstr=2, strin);
  if (strin[0].lun_stringa > 8)
 exit( fprintf(stderr,
 "ATTENZIONE il nome di un modello non deve essere piu` lungo di 8 caratteri"));
  strcpy( s02_.model[nmod].name, strin[0].stringa);
  s02_.model[nmod].lun_name = strin[0].lun_stringa;
/*  
   if (strin[1].lun_stringa
   && (i = strin[1].stringa - riga + strin[1].lun_stringa) < lun)
     riga[i] = ' ';
*/
  strcpy( s02_.model[nmod].descr, strin[1].stringa);
  printf( "Nome modello %2d =  %s\n", nmod+1, s02_.model[nmod].name);
  nmod++;
  } while (1);
printf( "********************************************\n");
if (nmod == 0)
 exit( fprintf(stderr,
 "ATTENZIONE  Numero modelli = 0"));
if (nmod > MAX_MODEL)
 exit( printf(
 "ATTENZIONE  Numero modelli > %2d\n", MAX_MODEL));
printf( "Numero totale modelli :  %d\n", nmod);
s02_.nmod=nmod;
printf( "********************************************\n");

/* lettura dei path della macchina di sviluppo */
for (i=0; i<nmod; i++) {
  legge_riga(riga,&lun);
  separa_str( riga, lun, nstr=2, strin);
  strcpy( path_svil[i], strin[0].stringa);
  s02_.model[i].tipo_task=TASK_PROCESSO;
  if((strin[1].stringa[0]=='p')||(strin[1].stringa[0]=='P'))
        s02_.model[i].tipo_task=TASK_PROCESSO;
  if((strin[1].stringa[0]=='r')||(strin[1].stringa[0]=='R'))
        s02_.model[i].tipo_task=TASK_REGOLAZIONE;
  if((strin[1].stringa[0]=='n')||(strin[1].stringa[0]=='N'))
        s02_.model[i].tipo_task=TASK_NOLEGO;
  }
/* lettura asterischi */
legge_riga(riga,&lun);


/* lettura delle SISTEMA OPERATIVO, MACCHINA E DIRECTORY dei modelli */

for (i=0; i<nmod; i++) {
  legge_riga( riga, &lun);
  if ( ! strncmp( riga, "****", 4))
 exit( fprintf(stderr,
 "ATTENZIONE il numero dei path e` diverso dal numero dei modelli"));

  separa_str( riga, lun, nstr=4, strin);

  if (strcmp(strin[0].stringa,"OS")!=0)
    {
    strcpy( sistema_operativo, strin[0].stringa);
    strcpy( s02_.model[i].sistema_operativo, strin[0].stringa);
    }
  else
    {
#if defined ULTRIX
    strcpy( sistema_operativo, "ULTRIX");
    strcpy( s02_.model[i].sistema_operativo, "ULTRIX");
#endif
#if defined OSF1
    strcpy( sistema_operativo, "OSF1");
    strcpy( s02_.model[i].sistema_operativo, "OSF1");
#endif
#if defined LINUX
    strcpy( sistema_operativo, "LINUX");
    strcpy( s02_.model[i].sistema_operativo, "LINUX");
#endif
#if defined SCO_UNIX
    strcpy( sistema_operativo, "SCO");
    strcpy( s02_.model[i].sistema_operativo, "SCO");
#endif
#if defined AIX 
    strcpy( sistema_operativo, "AIX");
    strcpy( s02_.model[i].sistema_operativo, "AIX");
#endif
#if defined VMS
    strcpy( sistema_operativo, "VMS");
    strcpy( s02_.model[i].sistema_operativo, "VMS");
#endif
    }

  if (strcmp(strin[1].stringa,"host")!=0)
     {
     sked_distribuito=1;
//     printf("\t\t Copio in s02_.model[i=%d].host=%s\n",i,strin[1].stringa);
     strcpy( s02_.model[i].host, strin[1].stringa);
     }
  else
     {
//     printf("\t\t Copio in s02_.model[i=%d].host=loopback\n",i);
     strcpy( s02_.model[i].host,"loopback");
     }

//     printf("\t\tprimo: s02_.model[i=%d].host=%s\n", i, s02_.model[i].host);

  if (strcmp(strin[2].stringa,"utente")!=0)
     strcpy( s02_.model[i].utente, strin[2].stringa);
  else
     strcpy( s02_.model[i].utente, "guest");

  strcpy( s02_.model[i].path, strin[3].stringa);
/*
  printf("Modello %s ",s02_.model[i].name);
*/
  switch(s02_.model[i].tipo_task)
	{
	case TASK_PROCESSO:
		{
		printf("(task di processo)\n");
		break;
		}
	case TASK_REGOLAZIONE:
		{
		printf("(task di regolazione)\n");
		break;
		}
	case TASK_NOLEGO:
		{
		printf("(task non lego)\n");
		break;
		}
	}
  printf("Sistema operativo = %s \nHost = %s \nUtente = %s\n",
	s02_.model[i].sistema_operativo,s02_.model[i].host,
	s02_.model[i].utente);
  printf("Path = %s\n", s02_.model[i].path);
  printf("Path svil = %s\n",path_svil[i]);
  printf( "********************************************\n");
  }

/*  Controllo che sia effettivamente una configurazione distribuita */
    if ( sked_distribuito==1 )
      {
      for (i=0; i<nmod; i++) 
        for (ii=0; ii<=i; ii++) 
          {
          if ( strcmp(s02_.model[i].host,s02_.model[ii].host)!=0 )
               break;
          }
      if ( i==ii )
         {
         sked_distribuito = -1;
         strcpy(macchina_target,s02_.model[i-1].host);
         }
      }

legge_riga( riga, &lun);
if ( strncmp( riga, "****", 4))
 exit( fprintf(stderr,
 "ATTENZIONE il numero dei path e` diverso dal numero dei modelli"));

/* lettura dei passi di tempo dei singoli modelli */

for (i=0; i<nmod; i++) {
  legge_riga( riga, &lun);
  if ( ! strncmp( riga, "****", 4))
 exit( fprintf(stderr,
 "ATTENZIONE il numero dei DT e` diverso dal numero dei modelli"));
/* verifica la presenza di un asterisco seganlante la forzatura dei
   read */
if(!strncmp(riga,"*",1))
{
  separa_str( riga, lun, nstr=2, strin);
  s02_.tabel[i].forzatura=1;
  if ( sscanf( strin[1].stringa, "%5f", &s02_.model[i].dt) != 1)
 exit( fprintf(stderr,
 "ATTENZIONE errore nel formato dei passi di tempo"));
}
else
{
  separa_str( riga, lun, nstr=1, strin);
  s02_.tabel[i].forzatura=0;
  if ( sscanf( strin[0].stringa, "%5f", &s02_.model[i].dt) != 1)
 exit( fprintf(stderr,
 "ATTENZIONE errore nel formato dei passi di tempo"));
}
if(s02_.tabel[i].forzatura==0)
  printf( "DT del modello %8s :  %5.3f\n",
	s02_.model[i].name, s02_.model[i].dt);
else
  printf( "DT del modello %8s :  %5.3f modello forzato in lettura\n",
        s02_.model[i].name, s02_.model[i].dt);
  }
printf( "********************************************\n");
legge_riga( riga, &lun);
if ( strncmp( riga, "****", 4))
 exit( fprintf(stderr,
 "ATTENZIONE il numero dei DT e` diverso dal numero dei modelli"));

check_dt( &s02_.model[0], nmod, &nrip);

tabella( &s02_.model[0], &s02_.tabel[0], nmod, nrip);


/* scrittura del file S04.DAT, costruzione dei puntatori e di VARLEGO */

/*
if(!speed_version)
{
*/

if (! (fp_s04 = fopen( "S04", "w")))
 exit( fprintf(stderr, "cannot create S04"));
fwrite( &nmod, sizeof( int), 1, fp_s04);
for (i=0; i<nmod; i++) {
  fwrite( s02_.model[i].name, sizeof( s02_.model[i].name), 1, fp_s04);
  fwrite( s02_.model[i].descr, sizeof( s02_.model[i].descr), 1, fp_s04);
  }

/* alloca la struttura varlego */
varlego=(VARLEGO*)malloc(sizeof(VARLEGO)*nmod);
#if defined UNIX
for(i=0;i< nmod;i++)
	{
	varlego[i].nom_sivar=(char **)cdim2(_N003,10);
	varlego[i].nom_vari=(char **)cdim2(_N004,10);
	}
#endif

task_offset = 0;
for (i=0; i<nmod; i++) {

/* Determinazione sigla del tipo modello */ 

  switch(s02_.model[i].tipo_task)
	{
	case TASK_PROCESSO:
		{
		strcpy(sigla_tipo_task, "P");
		break;
		}
	case TASK_REGOLAZIONE:
		{
		strcpy(sigla_tipo_task, "R");
		break;
		}
	case TASK_NOLEGO:
		{
		strcpy(sigla_tipo_task, "N");
		break;
		}
	}

/* Scrittura info modelli in sqlite */ 

/* Se db != NULL gestisco Scrittura info modelli in sqlite  */
if ( db != NULL )
{
   SqlStatement = sqlite3_mprintf("insert into MODELLI(progr_modello, sigla, descrizione, dt, path_locale, path_globale, sistema_operativo, host, utente, sigla_tipimodello)  values ( '%d', '%q', '%q', '%f', '%q', '%q', '%q', '%q', '%q', '%q' );", 
                                i, s02_.model[i].name, s02_.model[i].descr, s02_.model[i].dt, s02_.model[i].path, 
                                path_svil[i], s02_.model[i].sistema_operativo, s02_.model[i].host, s02_.model[i].utente, sigla_tipo_task);
   rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
   if( rc!=SQLITE_OK ){
     fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
   }
   sqlite3_free(SqlStatement);
}
/* FINE Se db != NULL gestisco Scrittura info modelli in sqlite  */


/* modello di tipo LEGO */
  if (s02_.model[i].name[0] != '$')
	{
	printf("Modello LEGO %s\n",s02_.model[i].name);
	read_f04_write_s04( &varlego[i], i, &s02_.model[0]);
	}

/* modello di tipo NON LEGO */
  else  
	{
	printf("Modello NON LEGO %s\n",s02_.model[i].name);
	read_n04_write_s04( &varlego[i], i, &s02_.model[0]);
	}

  s02_.model[i].punt_ivus = iapp = 1 + task_offset /**i*MAX_ALLOC_MODEL**/;
  s02_.model[i].punt_ivin = iapp += varlego[i].neqsis;
  s02_.model[i].punt_ider = iapp += varlego[i].nu;
  s02_.model[i].punt_idati = iapp += varlego[i].nderi;
  s02_.model[i].punt_ifine = iapp += varlego[i].ndati;

  task_offset += (varlego[i].neqsis + varlego[i].nu + 
			varlego[i].nderi + varlego[i].ndati);

printf("nderi=%d \npunt_ivus=%d  punt_ider=%d  punt_idati=%d punt_ifine=%d\n",
       varlego[i].nderi,
       s02_.model[i].punt_ivus,
       s02_.model[i].punt_ider,
       s02_.model[i].punt_idati,s02_.model[i].punt_ifine);
/*
  printf("Modello %d size=%d\n",i,
	(s02_.model[i].punt_ider-s02_.model[i].punt_ivus)*
		sizeof(float));
*/

  }

fclose( fp_s04);
/*
}
*/


/* lettura dei collegamenti tra i modelli */
ntotcoll=0;
rewind( fp_s01);

riga_s01 = 0;
for (i=0; i<4*nmod+7; i++)
  legge_riga( riga, &lun);

/* scrittura del file S05.DAT contenente l'elenco delle variabili di
   ingresso (modello per modello) connesse ad altri modelli */

if (! (fp_s05 = fopen( "S05", "w")))
 exit( fprintf(stderr, "cannot create S05"));
fwrite( &nmod, sizeof( int), 1, fp_s05);
for (imi=0; imi<nmod; imi++) {

  legge_riga( riga, &lun);

  ncoll=0;
  do {
    legge_riga( riga, &lun);
    if ( ! strncmp( riga, "****", 4))
      break;
    separa_str( riga, lun, nstr=3, strin);

    check_input (strin[0].stringa, imi, &ii, &varlego[imi], &s02_.model[0]);
    check_model (strin[1].stringa, imi, &imu, &s02_.model[0], nmod);
    check_output(strin[2].stringa, imu, &iu, &varlego[imu], &s02_.model[0]);

    if (ncoll >= MAX_COLL)
 exit( printf(
 "ATTENZIONE il modello %s ha piu` di %d ingressi connessi",
	s02_.model[imi].name, MAX_COLL));
    /*
	verifica che l'ingresso non sia stato precedentemente connesso
    */
    for(i=0;i<ncoll;i++)
	{
	if(ii==s02_.model[imi].punt_ing[i])
		exit( printf(
 	  "ATTENZIONE la variabile %s e` gia` connessa\n",
		strin[0].stringa));
	}
/* Inserisce nel buffer per il successivo aggiornamento del database */
    collega_buffer[ntotcoll+ncoll].input=ii-1;
    collega_buffer[ntotcoll+ncoll].output=iu-1;
/* FINE Inserisce nel buffer per il successivo aggiornamento del database */

    s02_.model[imi].punt_ing[ncoll]=ii;
    s02_.model[imi].punt_usc[ncoll]=iu;
    strcpy( colleg[imi].coll[ncoll], strin[0].stringa);
    for (i=strlen( strin[0].stringa); i<8; i++)
      colleg[imi].coll[ncoll][i] = ' ';
    /* riempimento tabella COLLEGAMENTO */
    ++collegamento[imi][imu].numero_collegamenti;
    if (collegamento[imi][imu].numero_collegamenti==1)
	{
	collegamento[imi][imu].ingresso=(int*)malloc(sizeof(int));
	collegamento[imi][imu].uscita=(int*)malloc(sizeof(int));
	}
    else
	{
	collegamento[imi][imu].ingresso=(int*)realloc(
		collegamento[imi][imu].ingresso,sizeof(int)*
		collegamento[imi][imu].numero_collegamenti);
	collegamento[imi][imu].uscita=(int*)realloc(
		collegamento[imi][imu].uscita,sizeof(int)*
		collegamento[imi][imu].numero_collegamenti);
	}
    collegamento[imi][imu].
        ingresso[collegamento[imi][imu].numero_collegamenti-1]=ii-1;
    collegamento[imi][imu].
        uscita[collegamento[imi][imu].numero_collegamenti-1]=iu-1;
    } while (++ncoll);
  s02_.model[imi].num_ic=ncoll;
  fwrite( &ncoll, sizeof( int), 1, fp_s05);
  fwrite( colleg[imi].coll[0], sizeof( colleg[imi].coll[0]), ncoll, fp_s05);
  ntotcoll+=ncoll;
  }

/*
printf("Fine scrittura S05\n");
*/

#if defined BANCO_ISTRUTTORE || defined BANCO_MANOVRA || SCADA
/* 
  legge l'allocazione del BM SCADA e BI
*/
printf("********************************************\n");
#endif

#if defined BANCO_MANOVRA 
legge_riga( riga, &lun);
separa_str( riga, lun, nstr=2, strin);
if (strncmp( strin[0].stringa, "BM", 2)!=0)
        {
        fprintf(stderr,"Definire l'host del banco manovra\n");
	exit(0);
        }
if((strlen(strin[1].stringa)==1)&&(strin[1].stringa[0]=' '))
	{
	printf("banco manovra non configurato\n");
	s02_.host_bm[0]=(char )NULL;
	}
else
	{
	strcpy(s02_.host_bm,strin[1].stringa);
	printf("host banco manovra <%s>\n",s02_.host_bm);
	}
#endif 

#if defined SCADA 
legge_riga( riga, &lun);
separa_str( riga, lun, nstr=2, strin);
if (strncmp( strin[0].stringa, "SCADA", 5)!=0)
        {
        fprintf(stderr,"Definire l'host dello scada\n");
	exit(0);
        }
if((strlen(strin[1].stringa)==1)&&(strin[1].stringa[0]=' '))
	{
	printf("scada non configurato\n");
	s02_.host_scada[0]=(char )NULL;
	}
else
	{
	strcpy(s02_.host_scada,strin[1].stringa);
	printf("host scada <%s>\n",s02_.host_scada);
	}
#endif

#if defined BANCO_ISTRUTTORE 
legge_riga( riga, &lun);
separa_str( riga, lun, nstr=2, strin);
if (strncmp( strin[0].stringa, "BI", 5)!=0)
        {
        fprintf(stderr,"Definire l'host del banco istruttore\n");
	exit(0);
        }
if((strlen(strin[1].stringa)==1)&&(strin[1].stringa[0]=' '))
	{
	printf("banco istruttore non configurato\n");
	s02_.host_bi[0]=NULL;
	}
else
	{
	strcpy(s02_.host_bi,strin[1].stringa);
	printf("host banco istruttore <%s>\n",s02_.host_bi);
	}
#endif
printf("********************************************\n");
/* eliminazione varlego */
#if defined UNIX
for(i=0;i< nmod;i++)
	{
//	printf("\t\tsecondo: s02_.model[i=%d].host=%s\n", i, s02_.model[i].host);
	cfree2(varlego[i].nom_sivar);
	cfree2(varlego[i].nom_vari);
	}
#endif
free(varlego);

fclose( fp_s05);

fclose( fp_s01);

/* costruzione tabelle_net */
printf("Costruzione tabelle\n");
tabelle_net(nmod, nrip);

/* scrittura file S02_new  */
printf("Scrittura file S02_$SO\n");
#if defined ULTRIX
	write_S02_net(ULTRIX_AIX);
	write_S02_net(ULTRIX_ULTRIX);
	write_S02_net(ULTRIX_VMS);
	write_S02_net(ULTRIX_OSF1);
	write_S02_net(ULTRIX_SCO);
#endif
#if defined AIX
	write_S02_net(AIX_AIX);
	write_S02_net(AIX_OSF1);
	write_S02_net(AIX_ULTRIX);
	write_S02_net(AIX_VMS);
	write_S02_net(AIX_SCO);
#endif
#if defined OSF1
/*
	write_S02_net(OSF1_AIX);
*/
	write_S02_net(OSF1_OSF1);
/*
	write_S02_net(OSF1_ULTRIX);
	write_S02_net(OSF1_VMS);
	write_S02_net(OSF1_SCO);
*/
#endif
#if defined LINUX
	write_S02_net(LINUX_LINUX);
#endif
#if defined VMS
	write_S02_net(VMS_AIX);
	write_S02_net(VMS_ULTRIX);
	write_S02_net(VMS_VMS);
	write_S02_net(VMS_OSF1);
	write_S02_net(VMS_SCO);
#endif
#if defined SCO_UNIX
	write_S02_net(SCO_AIX);
	write_S02_net(SCO_ULTRIX);
	write_S02_net(SCO_VMS);
	write_S02_net(SCO_OSF1);
	write_S02_net(SCO_SCO);
#endif

/* creazione file variabili.rtf */
if(!speed_version)
	{
	printf("Creazione file variabili.rtf e variabili.edf \n");
	crea_variabili(&ind_sh_top,&variabili,&id_sh,nmod);
	}

if(ind_sh_top != NULL)
	free(ind_sh_top);

/* modifica il file di snapshot se esiste inserendo il flag di modifica */
co_snap();

for(k=0;k<10;k++) {
   if(strin[k].stringa != NULL)
	free(strin[k].stringa);
}
 printf("Prima di gestisco Scrittura nel DB dei collegamenti tra variabili\n");
/* Se db != NULL gestisco Scrittura nel DB dei collegamenti tra variabili */
if ( db != NULL )
{
   for (k=0; k<ntotcoll; k++)
   {
   /* Aggiornamento collegamenti in VARIABILI in sqlite */
                SqlStatement = sqlite3_mprintf("update VARIABILI set progr_varout_connessa ='%d' where progr_variabile='%d' ;",
                               collega_buffer[k].output, collega_buffer[k].input);
                rc = sqlite3_exec(db, SqlStatement, 0, 0, &zErrMsg);
                if( rc!=SQLITE_OK ){
                  fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
                }
                sqlite3_free(SqlStatement); 
   /* FINE Aggiornamento collegamenti in VARIABILI in sqlite */

   }

   /* Chiusura della transazione per velocizzare */ 
   rc = sqlite3_exec(db, "COMMIT;", 0, 0, &zErrMsg);
   if( rc!=SQLITE_OK ){
     fprintf(stderr, "SQL error: %s rc=%d :%s\n", zErrMsg, rc, SqlStatement);
   }

   /* Chiusura database e swap con il vecchio con successivo (eventuale) aggiornamento*/
   sqlite3_close(db);

   if (system("if [ -f algrt.db3 ]; then mv algrt.db3 algrt_old.db3; fi") == -1)
   {
      perror("Swapping algrt_new.db3");
      exit(7);
   }
   if (system("mv -f algrt_new.db3 algrt.db3") == -1)
   {
      perror("Swapping algrt.db3");
      exit(8);
   }
}
/* FINE Se db != NULL gestisco Scrittura nel DB dei collegamenti tra variabili */

printf( "********************************************\n");
if ( sked_distribuito==-1 )
  {
  fprintf(stderr,"WARNING:unico hostname specificato [%s]\n",
          s02_.model[i-1].host);
  fprintf(stderr,"===> configurazione distribuita non consentita !!\n");
  fprintf(stderr,"===> Avviare su macchina [%s]\n",macchina_target);
  /*
  fprintf(stderr,"===> File necessari: S02_$SO,variabili.rtf,variabili.edf\n");
  */
  }
printf( "Fine regolare del programma <%3.1f sec.>\n",(gettim()-tempo)/1000);
printf( "********************************************\n\n");
exit(0);
} 
