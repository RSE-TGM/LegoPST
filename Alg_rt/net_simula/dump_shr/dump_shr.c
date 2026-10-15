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
   modulo dump_shr.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)dump_shr.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)dump_shr.c	5.1\t11/7/95";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
# include "sim_param.h"
# include "sim_types.h"
#include <Rt/RtDbPunti.h>
#include <Rt/RtErrore.h>
# include <Rt/RtMemory.h>

VARIABILI *variabili;          /* database variabili di tutti i modelli */
char *ind_sh_top;          /* puntatore inizio shared memory sh_var */
int id_sh;                 /* identificatore sh_var */
int nmod;
int tot_variabili;

char   *nome_modello (char *, int);



#define errore(stringa)         exit(fprintf(stderr,"Attenzione !!!\n      Errore processo dump_shr: %s\n",stringa))


RtDbPuntiOggetto dbpunti;
RtErroreOggetto errore;

/*  libsim (letture_dati.c, var_sh.c...) usa il database SQLite della
    topologia tramite la globale db, che si aspetta definita dal programma.
    Qui non serve: NULL, e i controlli if(db!=NULL) la saltano. Come negli
    strumenti di Alg_rt/lg_fmu/tools.  */
#include <sqlite3.h>
sqlite3 *db = NULL;

void costruisci_var (char **, VARIABILI **, int *);	/* libsim/var_sh.c */

int main(argc,argv)
int argc;
char **argv;
{
int shr_usr_key;
int modello;
char nome_task[10];
char nome_file[50];
FILE *fp;
int i,t;
int count_zeri = 0;
int c_d =0;
int fine;


/* decodifica SHR_USR_KEY */
shr_usr_key = atoi((char *)getenv("SHR_USR_KEY"));

/* creazione o aggancio shared-memory   */
errore = RtCreateErrore(RT_ERRORE_TERMINALE,"dump_shr");
/* il quarto argomento (SIMULATOR *) e' arrivato dopo: NULL, come probe_step */
dbpunti = RtCreateDbPunti(errore,NULL,DB_PUNTI_INT,NULL);
if(dbpunti == NULL)
	errore("simulazione non attiva");


/* aggancio shared memory sh_var */
costruisci_var(&ind_sh_top,&variabili,&id_sh);

/* legge numero dei modelli */
nmod=numero_modelli(ind_sh_top);
tot_variabili=numero_variabili(ind_sh_top);



if(argc!=3)
	{
	printf("\n\n\nElenco delle task\n\n");
	for(i=1;i<=nmod;i++)
		printf("%2d) %s\n",i,nome_modello(ind_sh_top,i));
	printf("\nScegli il modello ");
	scanf("%d",&modello);
	if(modello>nmod)
		errore("il modello non esiste");
	printf("\n\nScegli il nome del file ");
	scanf("%s",nome_file);
	}
else
	{
	modello=atoi(argv[1]);
	strcpy(nome_file,argv[2]);
	}

if(modello>nmod)
	errore("il modello non esiste");

strcpy(nome_task,nome_modello(ind_sh_top,modello));

printf("*****************************************************\n");
printf("*                                                   *\n");
printf("* dump task <%10s> sul file <%15s> *\n",nome_task,nome_file);
printf("*                                                   *\n");
printf("*****************************************************\n");

if(strcmp(nome_file,"term")==0)
	fp = stdout;
else
	{
	if((fp=fopen(nome_file,"w"))==NULL)
		errore("apertura file di dump");
	}

fprintf(fp,"dump task <%s>\n\n\n",nome_task);
fprintf(fp,"Variabili ingresso e uscita\n\n");
for(i=0;i<tot_variabili;i++)
	{
	if(variabili[i].mod==modello)
		{
		fprintf(fp,"%s [%5d] = %f\n",variabili[i].nome,variabili[i].addr,
			RtDbPGetValueD(dbpunti,variabili[i].addr));
		t=variabili[i].addr;
		}
	}
++t;


		

fprintf(fp,"\n\nDerivate e Dati\n\n");

fine = RtDbPSizeModelli(dbpunti,modello-1) / sizeof(float);



fine = fine + RtDbPInizioModelli(dbpunti,modello-1);

for(i=t;i<fine;i++)
	{
	++c_d;
	if(RtDbPGetValueD(dbpunti,i)==0)
		count_zeri ++;
	else
		count_zeri= 0;
	if(count_zeri==10)
		{
		fprintf(fp,"   .\n");
		fprintf(fp,"   .\n");
		fprintf(fp,"   .\n");
		}
	if(count_zeri<10)
		fprintf(fp,"der/dato[%4d] addr [%5d] = %f\n",c_d,i,
			RtDbPGetValueD(dbpunti,i));
	}

if(strcmp(nome_file,"term")!=0)
	fclose(fp);
}
