/**********************************************************************
*
*       C Source:               net_operator.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Mon Sep 26 17:47:15 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: net_operator.c-1 %  (%full_filespec: net_operator.c-1:csrc:1 %)";
#endif
/*
   modulo net_operator.c
   tipo 
   release 2.4
   data 6/7/94
   reserved @(#)net_operator.c	2.4
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)net_operator.c	2.4\t6/7/94";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <Rt/RtErrore.h>
#include <Rt/RtDbPunti.h>
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "sked.h"
#include "../new_monit/bistrutt.h"
#include <sqlite3.h>

RtDbPuntiOggetto dbpunti;
RtErroreOggetto errore;

#define MITTENTE BI
#define COMANDO_START 200
#define FILE_PARAMETRI "/Simulator"
#define FILE_S02 "/S02_OSF1"

char riga[40],pathname[40];
char file_parametri[60];
char file_s02[60];

int _MAX_SNAP_SHOT;
int _MAX_BACK_TRACK;
int _MAX_CAMPIONI;
int _NUM_VAR;
int _MAX_PERTUR;
int _SPARE_SNAP;
int _PERT_CLEAR;

long ore     = 0;
long minuti  = 0;
long secondi = 0;
long giorno  = 1;
long mese    = 1;
long anno    = 2000;
int t_cent,t_int;
float app_float;

SIMULATOR *simpar;

SNTAB *snapshot;

STATISTICA_SKED statistica_sked;

#if defined MFFR
SNAP_SKED sommari;
#endif

#if defined BACKTRACK
BKTAB *backtrack;
#endif

/* definizioni per la coda di messaggio delle perturbazioni */
int     id_msg_pert;
MSG_PERT messaggio;

/* definizioni per il database sqlite */
sqlite3 *db;

int pert_step_by_name(char *TagVariabile, float ValoreFinale, float TempoFinale);
int pert_step_by_addr(int TagAddress, float ValoreFinale, float TempoFinale);
int sub_maxsimtime();
extern  int converti_tempo(float,long  *,long  *,long  *,long  *,long  *,long  *);

/***************************************************************/
main(argc,argv)

int argc;
char **argv;

{
int comando;
int app_int;
int ch;

if(argc<3)
	elenca();

strcat(pathname,argv[1]);
strcpy(file_parametri,pathname);
strcat(file_parametri,FILE_PARAMETRI);
strcpy(file_s02,pathname);
strcat(file_s02,FILE_S02);

if(!atoi(argv[2]))
	{
	comando=-1;
	if((!strncasecmp("START",argv[2],5)) || (!strncasecmp("start",argv[2],5)))
		comando=COMANDO_START;
	if((!strncasecmp("FINE",argv[2],4)) || (!strncasecmp("fine",argv[2],4)))
		comando=COMANDO_CHIUSURA;
	if((!strncasecmp("RUN",argv[2],3)) || (!strncasecmp("run",argv[2],3)))
		comando=COMANDO_RUN;
	if((!strncasecmp("FREEZE",argv[2],6)) || (!strncasecmp("freeze",argv[2],6)))
		comando=COMANDO_FREEZE;
	if((!strncasecmp("STEP",argv[2],4)) || (!strncasecmp("step",argv[2],4)))
		comando=COMANDO_STEP;
	if((!strncasecmp("CLEAR",argv[2],5)) || (!strncasecmp("clear",argv[2],5)))
		comando=COMANDO_CLEAR;
	if((!strncasecmp("SPEED",argv[2],5)) || (!strncasecmp("speed",argv[2],5)))
		comando=COMANDO_TIMESCALING;
	if((!strncasecmp("LOADIC",argv[2],6)) || (!strncasecmp("loadic",argv[2],6)))
		comando=COMANDO_LOADIC;
	if((!strncasecmp("SAVEIC",argv[2],6)) || (!strncasecmp("saveic",argv[2],6)))
		comando=COMANDO_SAVEIC;
	if((!strncasecmp("EDITIC",argv[2],6)) || (!strncasecmp("editic",argv[2],6)))
		comando=COMANDO_EDITIC;
	if((!strncasecmp("LOADBT",argv[2],6)) || (!strncasecmp("loadbt",argv[2],6)))
		comando=COMANDO_LOADBT;
	if((!strncasecmp("VALBT",argv[2],6)) || (!strncasecmp("valbt",argv[2],6)))
		comando=COMANDO_VALIDATEBT;
	if((!strncasecmp("SAVEBT",argv[2],6)) || (!strncasecmp("savebt",argv[2],6)))
		comando=COMANDO_SAVEBT;
	if((!strncasecmp("EDITBT",argv[2],6)) || (!strncasecmp("editbt",argv[2],6)))
		comando=COMANDO_EDITBT;
	if((!strncasecmp("REPLAY",argv[2],6)) || (!strncasecmp("replay",argv[2],6)))
		comando=COMANDO_REPLAY;
	if((!strncasecmp("STATO",argv[2],5)) || (!strncasecmp("stato",argv[2],5)))
		comando=COMANDO_STATO;
	if((!strncasecmp("INIT",argv[2],4)) || (!strncasecmp("init",argv[2],4)))
		comando=COMANDO_INITIALIZE;
	if((!strncasecmp("PERT_STEP_BY_NAME",argv[2],17)) || (!strncasecmp("pert_step_by_name",argv[2],17)))
		if (argc == 6)
			comando=COMANDO_PERT_STEP_BY_NAME;
		else
			elenca();
	if((!strncasecmp("PERT_STEP_BY_ADDR",argv[2],17)) || (!strncasecmp("pert_step_by_addr",argv[2],17)))
		if (argc == 6)
			comando=COMANDO_PERT_STEP_BY_ADDR;
		else
			elenca();
	if((!strncasecmp("MAXSIMTIME",argv[2],10)) || (!strncasecmp("maxsimtime",argv[2],10)))
		comando=COMANDO_MAXSIMTIME;
	}

if (comando==-1)
	printf("\nComando NON implementato !!!\n\a");
else if (comando==COMANDO_START)
	{
	chdir(pathname);
/*	system("web_startup"); */
	lettura_parametri();
        SD_inizializza(MITTENTE);
        printf("NET_OPERATOR:Comando START ok!\n");
	}
else
	{
	lettura_parametri();
	SD_stato(MITTENTE,&statistica_sked);
	t_int=statistica_sked.tempo_sim;
	t_cent=(int)((float)statistica_sked.tempo_sim*100.0)-t_int*100;
	converti_tempo((float)(statistica_sked.tempo_sim),
                        &ore,&minuti,&secondi,&giorno,&mese,&anno);
	switch(statistica_sked.stato_sked)
		{
                case STATO_STOP:
                        {
			strcpy(riga,"STOP");
			if (comando==COMANDO_STATO)
				stampa_stato();
                        else if (comando==COMANDO_INITIALIZE)
                                {
                                SD_inizializza(MITTENTE);
                                }
			else
				printf("NET_OPERATOR:Comando NON ammesso !!!\a\n");
			break;
                        }
		case STATO_RUN:
			{
			strcpy(riga,"RUN");
			if (comando==COMANDO_FREEZE)
				sub_freeze();
			else if (comando==COMANDO_TIMESCALING)
				{
				app_float=atof(argv[3]);
				sub_timescaling();
				}
			else if (comando==COMANDO_STATO)
				stampa_stato();
			else
				printf("NET_OPERATOR:Comando NON ammesso !!!\a\n");
			break;
			}
		case STATO_FREEZE:
			{
			strcpy(riga,"FREEZE");
			if (comando==COMANDO_RUN)
				sub_run();
			else if (comando==COMANDO_CHIUSURA)
				{
				SD_stop(MITTENTE);
				system ("killsim");
				printf("NET_OPERATOR:Comando FINE ok!!!\a\n");
				}
			else if (comando==COMANDO_STEP)
				sub_step();
			else if (comando==COMANDO_CLEAR)
				sub_clear();
			else if (comando==COMANDO_TIMESCALING)
				{
				app_float=atof(argv[3]);
				sub_timescaling();
				}
			else if (comando==COMANDO_MAXSIMTIME)
                                {
				app_float=atof(argv[3]);
				sub_maxsimtime();
                                }
			else if (comando==COMANDO_EDITIC)
				sub_editic();
			else if (comando==COMANDO_LOADIC)
				{
				app_int=atoi(argv[3]);
				sub_loadic(app_int);
				}
			else if (comando==COMANDO_SAVEIC)
				{
				app_int=atoi(argv[3]);
				sub_saveic(app_int,argv[4]);
				}
			else if (comando==COMANDO_EDITBT)
				sub_editbt();
			else if (comando==COMANDO_LOADBT)
				{
				if(sub_saveic(_MAX_SNAP_SHOT,"Default snapshot")<0)
					exit(0);
				app_int=atoi(argv[3]);
				if(sub_loadbt(app_int)<0) exit(0);
				sub_backtrack();
               			}
			else if (comando==COMANDO_STATO)
                                {
				stampa_stato();
                                }
			else if (comando==COMANDO_PERT_STEP_BY_NAME)
                                {
				pert_step_by_name(argv[3], atof(argv[4]), atof(argv[5]));
                                }
			else if (comando==COMANDO_PERT_STEP_BY_ADDR)
                                {
				pert_step_by_addr(atoi(argv[3]), atof(argv[4]), atof(argv[5]));
                                }
			else
				printf("NET_OPERATOR:Comando NON ammesso !!!\a\n");
			break;
			}
		case STATO_BACKTRACK:
			{
			strcpy(riga,"BACKTRACK");
			if (comando==COMANDO_EDITBT)
				sub_editbt();
			else if (comando==COMANDO_LOADBT)
				{
				app_int=atoi(argv[3]);
				sub_loadbt(app_int);
				}
			else if (comando==COMANDO_VALIDATEBT)
                		{
				sub_valbt();
				sub_freeze();
               			}
			else if (comando==COMANDO_FREEZE)
                		{
				printf("\nBACK TO THE PREVIOUS SESSION !!!\n");
				if(sub_loadic(_MAX_SNAP_SHOT)<0) exit(0);
				sub_freeze();
               			}
			else if (comando==COMANDO_REPLAY)
                		{
				if(SD_replay(MITTENTE)>0)
					printf("OK REPLAY\n");
				else
					printf("NOT OK REPLAY\n");
               			}
			else if (comando==COMANDO_STATO)
				stampa_stato();
			else
				printf("NET_OPERATOR:Comando NON ammesso !!!\a\n");
			break;
			}
		case STATO_REPLAY:
			{
			strcpy(riga,"REPLAY");
			if (comando==COMANDO_STATO)
				stampa_stato();
			else if (comando==COMANDO_REPLAY)
				sub_backtrack();
			else
				printf("NET_OPERATOR:Comando NON ammesso !!!\a\n");
			break;
			}
		case STATO_ERRORE:
			{
			strcpy(riga,"ERRORE");
			if (comando==COMANDO_STATO)
				stampa_stato();
			else if (comando==COMANDO_CHIUSURA)
				{
				SD_stop(MITTENTE);
				system ("killsim");
				printf("NET_OPERATOR:Comando FINE ok!!!\a\n");
				}
			else
				printf("NET_OPERATOR:Comando NON ammesso !!!\a\n");
			break;
			}
		default:
			{
			strcpy(riga,"NON DEFINITO");
			break;
			}
		}
	}
}

/**********************************************************/
elenca()
{
printf("\nnet_operator <path_simulatore> <comando> (<campo_1> <campo_2> <campo_3> <campo_4>)\n\n");
printf("+-------------------+---------+---------+---------+---------+\n");
printf("|     <comando>     |<campo_1>|<campo_2>|<campo_3>|<campo_4>|\n");
printf("+-------------------+---------+---------+---------+---------+\n");
printf("| START             |         |         |         |         |\n");
printf("| FINE              |         |         |         |         |\n");
printf("| RUN               |         |         |         |         |\n");
printf("| FREEZE            |         |         |         |         |\n");
printf("| STEP              |         |         |         |         |\n");
printf("| CLEAR             |         |         |         |         |\n");
printf("| SPEED             |  float  |         |         |         |\n");
printf("| EDITBT            |         |         |         |         |\n");
printf("| LOADBT            |   int   |         |         |         |\n");
printf("| VALBT             |         |         |         |         |\n");
printf("| REPLAY            |         |         |         |         |\n");
printf("| EDITIC            |         |         |         |         |\n");
printf("| LOADIC            |   int   |         |         |         |\n");
printf("| SAVEIC            |   int   |  descr  |         |         |\n");
printf("| STATO             |         |         |         |         |\n");
printf("| PERT_STEP_BY_NAME | tagname |  value  |  time   |         |\n");
printf("| PERT_STEP_BY_ADDR | tagaddr |  value  |  time   |         |\n");
printf("| MAXSIMTIME        |  float  |         |         |         |\n");
printf("+-------------------+---------+---------+---------+---------+\n");
exit(0);
}

/**********************************************************/
int lettura_parametri()
{
#define MAX_LUN_RIGA 200

struct stat buffer;

char riga[MAX_LUN_RIGA+1];
char *token1,*token2,*token3;
int nocomment,numero;
Boolean retlog;
int *newstato;
FILE *fp;

if (stat(file_parametri,&buffer)<0)
	exit(printf("Il file %s non esiste\n",file_parametri));
if ((fp=fopen(file_parametri,"r"))==NULL)
	exit(printf("Errore apertura file %s\n",file_parametri));

while (fgets(riga,MAX_LUN_RIGA,fp)!=NULL)
        {
        nocomment = strncmp(riga,"#",1);
        if (nocomment != 0)
                {
                token1 = strtok(riga,".");
                token2 = strtok((char *)NULL, ":" );
                token3 = strtok((char *)NULL, ":" );

                if (strcmp(token2,"MAX_SNAP_SHOT")==0)
                        _MAX_SNAP_SHOT = atoi(token3);
                else if (strcmp(token2,"MAX_BACK_TRACK")==0)
                        _MAX_BACK_TRACK = atoi(token3);
                else if (strcmp(token2,"MAX_CAMPIONI")==0)
                        _MAX_CAMPIONI = atoi(token3);
                else if (strcmp(token2,"NUM_VAR")==0)
                        _NUM_VAR = atoi(token3);
                else if (strcmp(token2,"MAX_PERTUR")==0)
                        _MAX_PERTUR = atoi(token3);
                else if (strcmp(token2,"SPARE_SNAP")==0)
                        _SPARE_SNAP = atoi(token3);
                else if (strcmp(token2,"PERT_CLEAR")==0)
                        _PERT_CLEAR = atoi(token3);
                else
                        printf("diverso\n");
                }
        }
fclose(fp);

simpar = (SIMULATOR*)calloc(1,sizeof(SIMULATOR));
simpar->max_snap_shot  = _MAX_SNAP_SHOT;
simpar->max_back_track = _MAX_BACK_TRACK;
simpar->max_campioni   = _MAX_CAMPIONI;
simpar->num_var        = _NUM_VAR;
simpar->max_pertur     = _MAX_PERTUR;
simpar->spare_snap     = _SPARE_SNAP;
simpar->pert_clear     = _PERT_CLEAR;

/* aggancio al database dei punti */

errore = RtCreateErrore(RT_ERRORE_TERMINALE,"net_operator");
dbpunti = RtCreateDbPunti(errore,NULL,DB_PUNTI_INT,simpar);

if(dbpunti == NULL)
	exit(printf("NET_OPERATOR: simulazione non attiva!!\a\n"));

/* recupero il puntatore all'area degli snapshot */

snapshot = RtDbPGetPuntSnapshot(dbpunti);

/* recupero il puntatore all'area dei backtrack */

backtrack = RtDbPGetPuntBacktrack(dbpunti);

return(0);
}

/**********************************************************/
stampa_stato()
{
	printf("NET_OPERATOR:Stato= %s\n",riga);
	printf("NET_OPERATOR:Tempo di simulazione= %2d:%2d:%2d.%d   %2d-%2d-%4d\n",
		ore,minuti,secondi,t_cent,giorno,mese,anno);
}

/**********************************************************/
int sub_freeze()
{
	if(SD_freeze(MITTENTE)>0)
		{
		printf("NET_OPERATOR:%2d:%2d:%2d.%d : Switch to FREEZE\n",
			ore,minuti,secondi,t_cent);
		return(0);
		}
	else
		{
		printf("NET_OPERATOR:Switch to FREEZE failed !!!\a\n");
		return(-1);
		}
}

/**********************************************************/
int sub_run()
{
	if(SD_run(MITTENTE)>0)
		{
		printf("NET_OPERATOR:%2d:%2d:%2d.%d : Switch to RUN\n",
			ore,minuti,secondi,t_cent);
		return(0);
		}
	else
		{
		printf("NET_OPERATOR:Switch to RUN failed !!!\a\n");
		return(-1);
		}
}

/**********************************************************/
int sub_clear()
{
	if(SD_clear(MITTENTE)>0)
		{
		printf("NET_OPERATOR:%2d:%2d:%2d.%d : Reset simulation time & recordings\n",
			ore,minuti,secondi,t_cent);
		return(0);
		}
	else
		{
		printf("NET_OPERATOR:Executing STEP failed !!!\a\n");
		return(-1);
		}
}

/**********************************************************/
int sub_editic()
{
int i;
	if(SD_editic(MITTENTE,snapshot,0,_MAX_SNAP_SHOT)>0)
		{
		for(i=0;i<_MAX_SNAP_SHOT;i++)
			if(snapshot[i].stat!=0)
				printf("NET_OPERATOR:Snapshot n. %d - %s\n",
					snapshot[i].prog,snapshot[i].descr);
		return(0);
		}
	else
		{
		printf("NET_OPERATOR:Errore lettura lista snapshot !!!\a\n");
		return(-1);
		}
}

/**********************************************************/
int sub_loadic(int numsnap)
{
	if(SD_loadic(MITTENTE,&numsnap,&sommari)>0)
		{
		printf("NET_OPERATOR:%2d:%2d:%2d.%d : LOAD C.I. %2d\n",
			ore,minuti,secondi,t_cent,numsnap);
		return(0);
		}
	else
		{
		printf("NET_OPERATOR:LOAD C.I. %2d failed !!!\a\n",numsnap);
		return(-1);
		}
}


/**********************************************************/
int sub_saveic(int numsnap, char **descriz)
{
	if(SD_saveic(MITTENTE,&numsnap,descriz,NULL)>0)
		{
		printf("NET_OPERATOR:%2d:%2d:%2d.%d : Snapshot in C.I. %2d\n",
			ore,minuti,secondi,t_cent,numsnap);
		return(0);
		}
	else
		{
		printf("NET_OPERATOR:Snapshot in C.I. %2d failed !!!\a\n",numsnap);
		return(-1);
		}
}

/**********************************************************/
int sub_editbt()
{
int i;
	if(SD_editbt(BI,backtrack,0,_MAX_BACK_TRACK)>0)
		{
  		for(i=0;i<_MAX_BACK_TRACK;i++)
       		 	if(backtrack[i].stat!=0)
				printf("NET_OPERATOR:Backtrack n. %d - tempo %s - %s\n",
               	               		backtrack[i].prog,backtrack[i].temposn,
					backtrack[i].descr);
		return(0);
       		}	
       	else
		{
		printf("NET_OPERATOR:Errore lettura lista backtrack !!!\a\n");
		return(-1);
		}
}

/**********************************************************/
int sub_loadbt(int numbt)
{
	if(SD_loadbt(MITTENTE,&numbt,&sommari)>0)
		{
		printf("NET_OPERATOR:%2d:%2d:%2d.%d : LOAD BT %2d\n",
			ore,minuti,secondi,t_cent,numbt);
		return(0);
		}
	else
		{
		printf("NET_OPERATOR:LOAD BT %2d failed !!!\a\n",numbt);
		return(-1);
		}
}

/**********************************************************/
int sub_valbt()
{
	if(SD_validatebt(MITTENTE)>0)
		{
		printf("NET_OPERATOR:%2d:%2d:%2d.%d : Validating database loaded from BT\n",
			ore,minuti,secondi,t_cent);
		return(0);
		}
	else
		{
		printf("NET_OPERATOR:Validating database loaded from BT failed !!!\n");
		return(-1);
		}
}

/**********************************************************/
int sub_backtrack()
{
	if(SD_backtrack(MITTENTE)>0)
		{
		printf("NET_OPERATOR:%2d:%2d:%2d.%d : Switch to BACKTRACK\n",
			ore,minuti,secondi,t_cent);
		return(0);
		}
	else
		{
		printf("NET_OPERATOR:Switch to BACKTRACK failed !!!\n");
		return(-1);
		}
}

/**********************************************************/
int sub_step()
{
	if(SD_goup(MITTENTE)>0)
		{
		printf("NET_OPERATOR:%2d:%2d:%2d.%d : Executing STEP\n",
			ore,minuti,secondi,t_cent);
		return(0);
		}
	else
		{
		printf("NET_OPERATOR:Executing STEP failed !!!\a\n");
		return(-1);
		}
}

				
/**********************************************************/

int sub_maxsimtime()
{
printf("NET_OPERATOR:before max simulation time : %f\n", app_float);
	if(SD_maxsimtime(MITTENTE, &app_float) >0)
		{
		printf("NET_OPERATOR:Changing max simulation time : %f\n", app_float);
		return(0);
		}
	else
		{
		printf("NET_OPERATOR:Changing max simulation time failed !!!\a\n");
		return(-1);
		}
}
/**********************************************************/

/**********************************************************/
int sub_timescaling()
{
	if(SD_timescaling(MITTENTE,&app_float)>0)
		{
		printf("NET_OPERATOR:%2d:%2d:%2d.%d : Changing simulator speed : %3.1f\n",
			ore,minuti,secondi,t_cent,app_float);
		return(0);
		}
	else
		{
		printf("NET_OPERATOR:Changing simulator speed failed !!!\a\n");
		return(-1);
		}
}
/**********************************************************/
int pert_step_by_name(char *TagVariabile, float ValoreFinale, float TempoFinale)
{

	int iret;
	int     shr_usr_key;    /* chiave utente per shm */
	int indirizzo=-1;

/*
        determina la chiave per gli IPC
*/
    	shr_usr_key = atoi ((char *) getenv ("SHR_USR_KEY"));

/*
 	creazione o aggancio code messaggi 
*/
    	msg_create_fam (shr_usr_key, 0);
        algdb_open(strcat(pathname, "/algrt.db3"));
        indirizzo = algdb_variable_index(TagVariabile);
        algdb_close();
 	if (indirizzo != -1)
        {
             if ( iret = pert_scalino (id_msg_pert, indirizzo, ValoreFinale, TempoFinale) < 0)
                printf("NET_OPERATOR:Executing PERT_STEP failed !!!\a\n");
        }
        else
        {
             printf("NET_OPERATOR:nome variabile PERT non valido!!!\a\n");
        }
}

/**********************************************************/
int pert_step_by_addr(int TagAddress, float ValoreFinale, float TempoFinale)
{

	int iret;
	int     shr_usr_key;    /* chiave utente per shm */
	int indirizzo=-1;

/*
        determina la chiave per gli IPC
*/
    	shr_usr_key = atoi ((char *) getenv ("SHR_USR_KEY"));

/*
 	creazione o aggancio code messaggi 
*/
    	msg_create_fam (shr_usr_key, 0);
        algdb_open(strcat(pathname, "/algrt.db3"));
        indirizzo = AlgdbVariableAddressCheck(TagAddress);
        algdb_close();
 	if (indirizzo != -1)
        {
             if ( iret = pert_scalino (id_msg_pert, indirizzo, ValoreFinale, TempoFinale) < 0)
                printf("NET_OPERATOR:Executing PERT_STEP failed !!!\a\n");
        }
        else
        {
             printf("NET_OPERATOR:indirizzo variabile PERT non valido!!!\a\n");
        }
}

/**********************************************************/

int algdb_open(char *PathDb)
{
   int rc;
   rc = sqlite3_open(PathDb, &db);
   if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(-1);
    }
    return(0);

}

/**********************************************************/

int algdb_close()
{
   int rc;
   rc = sqlite3_close(db);
   return(0);

}

/**********************************************************/

int algdb_variable_index(char *TagVariable)
{
   char *SqlStatement;
   char **result;
   int  nrow;
   int  ncolumn;
   int  rc;
   char *errmsg;

   SqlStatement = sqlite3_mprintf("select progr_variabile from variabili where sigla = '%q';", TagVariable);
   rc = sqlite3_get_table(db, SqlStatement, &result, &nrow, &ncolumn, &errmsg);
 printf("ncolumn=%d \n",ncolumn);
   if( rc!=SQLITE_OK )
   {
        fprintf(stderr, "SQL error: %s rc=%d :%s\n", errmsg, rc, SqlStatement);
	return(-1);
   }
   sqlite3_free(SqlStatement);
   if ( ncolumn > 0 )
      return(atoi(result[1]));
   else
      return(-1);
}

/**********************************************************/

int AlgdbVariableAddressCheck(int TagAddress)
{
   char *SqlStatement;
   char **result;
   int  nrow;
   int  ncolumn;
   int  rc;
   char *errmsg;

printf("TagAddress=%d \n", TagAddress);
   SqlStatement = sqlite3_mprintf("select progr_variabile from variabili where progr_variabile = '%d';", TagAddress);
   rc = sqlite3_get_table(db, SqlStatement, &result, &nrow, &ncolumn, &errmsg);
 printf("ncolumn=%d \n",ncolumn);
   if( rc!=SQLITE_OK )
   {
        fprintf(stderr, "SQL error: %s rc=%d :%s\n", errmsg, rc, SqlStatement);
	return(-1);
   }
   sqlite3_free(SqlStatement);
   if ( ncolumn > 0 )
      return(atoi(result[1]));
   else
      return(-1);
}

