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
static char SccsID[] = "@(#)var_bin.c	5.1\t11/7/95";
/*
   modulo var_bin.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)var_bin.c	5.1
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sim_param.h"
#include "sim_types.h"
#include <Rt/RtMemory.h>

#define MAXRIGA 200

void legge_riga_bin(riga,lun,fp)
 char riga [MAXRIGA];
 int *lun;
 FILE *fp;
{
 int c, k;

 for (k=0; k<MAXRIGA; k++)
   riga[k] = (char)NULL;

 for (k=0; (c = fgetc( fp)) != '\n'; k++) 
  riga[k]=c;

 riga[k]=(char)NULL;
} 


void var_bin()
{
int nmod,mod;
int nbloc,bloc;
int nvar,ndati_blocco;
int tot_variabili=0;
int j,i,z,y,t;
FILE *fp;
FILE *fp_var;
char app[200];
char app_1[200];
char app_2[200];
char nome_modello[200];
char nome_blocco[200];
char app_nome[MAX_LUN_NOME_VAR];
int *k;
NOMI *nomi;
VARIABILI *variabili;
VARIABILI *app_var;
char *ind;
char *punt;
int offset;
int size_variabili,size_nomi,size;
NOMI_MODELLI nomi_modelli;
int app_int;

nomi=(NOMI *) malloc( sizeof(NOMI));
variabili=(VARIABILI *)malloc(sizeof(VARIABILI));
t=0;


/* apertura file variabili.edf */
fp=fopen("variabili.edf","r");


/* legge le prime due righe */
legge_riga_bin(app,k,fp);
legge_riga_bin(app,k,fp);

/* legge la terza riga (numero dei modelli)  */
legge_riga_bin(app,k,fp);
sscanf(app,"%s %s %d",app_1,app_1,&nmod);
nomi->nmod=nmod;


for(j=0;j<nmod;j++)
{
/* salta due righe */
legge_riga_bin(app,k,fp);
legge_riga_bin(app,k,fp);
/* legge il nome del modello e il numero dei blocchi */
legge_riga_bin(app,k,fp);
sscanf(app,"%s %s %s %s %s %s %d",app_1,app_1,app_1,nome_modello,app_1,
		app_1,&nbloc);
strcpy(nomi->name[j],nome_modello);
nomi->num_blocchi[j]=nbloc;


for(i=0;i<nbloc;i++)
{
/* salta una righa */
legge_riga_bin(app,k,fp);
/* legge il nome del blocco, il numero delle variabili e il numero di 
   dati realtivo al blocco */
legge_riga_bin(app,k,fp);
for (y=0; y<200; y++)
   app_1[y] = (char)NULL;
memcpy(nome_blocco,&app[8],MAX_LUN_NOME_BLOCCO);
memcpy(app_1,&app[8+MAX_LUN_NOME_BLOCCO],strlen(app)-(8+MAX_LUN_NOME_BLOCCO));
/*    sscanf(app_1,"%s %s %d",app_2,app_2,&nvar);     */
sscanf(app_1,"%s %s %d %s %s %s %d",app_2,app_2,&nvar,app_2,app_2,app_2,
                                    &ndati_blocco);
strcpy(nomi->nom_bloc[j][i],nome_blocco);
nomi->num_dati_bloc[j][i] = ndati_blocco;
for(z=0;z<nvar;z++)
	{
	if(z==0)
		{
		app_var=(VARIABILI*)malloc(tot_variabili*sizeof(VARIABILI));
		memcpy(app_var,variabili,tot_variabili*sizeof(VARIABILI));
		free(variabili);
		variabili=(VARIABILI*)malloc
			((tot_variabili+nvar)*sizeof(VARIABILI));
		memcpy(variabili,app_var,tot_variabili*sizeof(VARIABILI));
		tot_variabili+=nvar;
		free(app_var);
		}
	legge_riga_bin(app,k,fp);
	sscanf(app,"%d %s %d",
		&app_int,app_nome,
			&(variabili[t].addr));
	variabili[t].tipo=app_int;
	memset(variabili[t].nome,' ',(MAX_LUN_NOME_VAR-1));
	memcpy(variabili[t].nome,app_nome,strlen(app_nome));
	variabili[t].nome[MAX_LUN_NOME_VAR-1]='\00';
	variabili[t].mod=(short)j+1;
	variabili[t].blocco=(short)i+1;
	variabili[t].typ=NO_GRAF_SEL;
	variabili[t].typlg=NO_GRAF_SEL;
	if(variabili[t].addr<10)
	  strcpy(variabili[t].descr,&app[13]);
	else
	  if(variabili[t].addr<100)
	    strcpy(variabili[t].descr,&app[14]);
	  else
	    if(variabili[t].addr<1000)
	      strcpy(variabili[t].descr,&app[15]);
	    else
	      strcpy(variabili[t].descr,&app[16]);
	t++;
	}
}
}
/* costruzione file variabili.rtf */
size_variabili=sizeof(VARIABILI)*tot_variabili+sizeof(int);
size_nomi=sizeof(int)+sizeof(NOMI_MODELLI)*nomi->nmod;
for(mod=0;mod<nomi->nmod;mod++)
        size_nomi=size_nomi+sizeof(NOMI_BLOCCHI)*nomi->num_blocchi[mod];
size=size_variabili+size_nomi;
ind=(char*)malloc(sizeof(char)*size);
punt=ind;
memcpy(punt,&(nomi->nmod),sizeof(int));
punt = punt + sizeof(int);
offset=sizeof(NOMI_MODELLI)*nomi->nmod;

/* scrive le struttura NOMI_MODELLI */
for(mod=0;mod<nomi->nmod;mod++)
        {
        memcpy(punt,nomi->name[mod],MAX_LUN_NOME_MODELLO);
	fill_space(punt,MAX_LUN_NOME_MODELLO-1);
        punt = punt + MAX_LUN_NOME_MODELLO;
        nomi_modelli.num_blocchi=(unsigned short)nomi->num_blocchi[mod];
        memcpy(punt,&nomi_modelli.num_blocchi,sizeof(unsigned short));
        punt+=sizeof(unsigned short);
        memcpy(punt,&offset,sizeof(int));
        punt+=sizeof(int);
#if defined UNIX
        punt+=1;
#endif
        offset+=nomi_modelli.num_blocchi*sizeof(NOMI_BLOCCHI);
        }

/* scrive i nomi dei blocchi  */
for(mod=0;mod<nomi->nmod;mod++)
        {
        for(bloc=0;bloc<nomi->num_blocchi[mod];bloc++)
                {
                memcpy(punt,nomi->nom_bloc[mod][bloc],MAX_LUN_NOME_BLOCCO);
		fill_space(punt,MAX_LUN_NOME_BLOCCO-1);
                punt+=MAX_LUN_NOME_BLOCCO;
                memcpy(punt,&nomi ->num_dati_bloc[mod][bloc],sizeof(int));
                punt = punt + sizeof(int);
                }
        }

/* scrive il numero delle variabili */
memcpy(punt,&tot_variabili,sizeof(int));
punt+=sizeof(int);

/* scrive le variabili */
/*   *p_var=(VARIABILI*)punt;*/
memcpy(punt,variabili,sizeof(VARIABILI)*tot_variabili);
punt+=sizeof(VARIABILI)*tot_variabili;

   /* registra il file variabili.rtf  */
   if (! (fp_var = fopen( "variabili.rtf", "w")))
     exit(fprintf(stderr,"Impossibile creare variabili.rtf\n"));
   fwrite(ind,size,1,fp_var);
   fclose(fp_var);

free(nomi);
free(variabili);
}
