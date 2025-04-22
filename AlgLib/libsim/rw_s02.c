/**********************************************************************
*
*       C Source:               rw_s02.c
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed Aug 23 16:25:35 2006 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: rw_s02.c-4 %  (%full_filespec: rw_s02.c-4:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)rw_s02.c	5.1\t11/7/95";
/*
   modulo rw_s02.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)rw_s02.c	5.1
*/
#include <stdio.h>
#include <string.h>
#include "sim_param.h"
#include "sim_types.h"
#include "libnet.h"
#include <Rt/RtMemory.h>

COLLEGAMENTO collegamento[MAX_MODEL][MAX_MODEL];
SCAMBIO scambio[MAX_MODEL][MAX_DT_DEC];
S02 s02_;
int nmod,nrip;
char macro_modello [11];
int lun_macro_modello;
char descr_macro_modello [65];

void read_S02_net()
{
FILE *fp_s02;
int r_mod,w_mod;
int i,i1,j,nrip,nmod;

#if defined AIX
if (!(fp_s02=fopen("S02_AIX","r")))
   exit(fprintf(stderr,"rw_s02: errore apertura file S02_AIX\n"));
#endif
#if defined VMS
if (!(fp_s02=fopen("S02_VMS","r")))
   exit(fprintf(stderr,"rw_s02: errore apertura file S02_VMS\n"));
#endif
#if defined ULTRIX 
if (!(fp_s02=fopen("S02_ULTRIX","r")))
   exit(fprintf(stderr,"rw_s02: errore apertura file S02_ULTRIX\n"));
#endif
#if defined OSF1
if (!(fp_s02=fopen("S02_OSF1","r")))
   exit(fprintf(stderr,"rw_s02: errore apertura file S02_OSF1\n"));
#endif
#if defined LINUX
if (!(fp_s02=fopen("S02_LINUX","r")))
   exit(fprintf(stderr,"rw_s02: errore apertura file S02_LINUX\n"));
#endif
#if defined SCO_UNIX
if (!(fp_s02=fopen("S02_SCO","r")))
   exit(fprintf(stderr,"rw_s02: errore apertura file S02_SCO\n"));
#endif

/* scrittura del file S02_new in C */

fread( &s02_.nmod, sizeof( int), 1, fp_s02);
fread( &s02_.model[0], sizeof( MODEL), s02_.nmod, fp_s02);
fread( &s02_.nrip, sizeof( int), 1, fp_s02);
fread( &s02_.tabel[0], sizeof( TABEL), s02_.nmod, fp_s02);
fread( s02_.macro_modello, sizeof( s02_.macro_modello), 1, fp_s02);
fread(&s02_.lun_macro_modello, sizeof( int), 1, fp_s02);
fread( s02_.descr_macro_modello, sizeof( s02_.descr_macro_modello), 1, fp_s02);
#if defined BANCO_MANOVRA
fread( s02_.host_bm, sizeof(s02_.host_bm), 1, fp_s02);
#endif
#if defined SCADA
fread( s02_.host_scada, sizeof(s02_.host_scada), 1, fp_s02);
#endif
#if defined BANCO_ISTRUTTORE
fread( s02_.host_bi, sizeof(s02_.host_bi), 1, fp_s02);
#endif

for(r_mod=0;r_mod<MAX_MODEL;r_mod++)
        for(w_mod=0;w_mod<MAX_MODEL;w_mod++)
          {
          fread(&collegamento[r_mod][w_mod].numero_collegamenti,
                sizeof(int),1,fp_s02);
          if(collegamento[r_mod][w_mod].numero_collegamenti)
            {
            collegamento[r_mod][w_mod].ingresso=(int*)malloc(
             collegamento[r_mod][w_mod].numero_collegamenti*sizeof(int));
            collegamento[r_mod][w_mod].uscita=(int*)malloc(
             collegamento[r_mod][w_mod].numero_collegamenti*sizeof(int));
            fread(collegamento[r_mod][w_mod].ingresso,sizeof(int),
               collegamento[r_mod][w_mod].numero_collegamenti,fp_s02);
            fread(collegamento[r_mod][w_mod].uscita,sizeof(int),
               collegamento[r_mod][w_mod].numero_collegamenti,fp_s02);
            }
          }

for(r_mod=0;r_mod<MAX_MODEL;r_mod++)
   for(w_mod=0;w_mod<MAX_DT_DEC;w_mod++)
    {
	fread(&scambio[r_mod][w_mod],sizeof(SCAMBIO),1,fp_s02);
    }

fclose(fp_s02);


nmod=s02_.nmod;
nrip=s02_.nrip;
/* riassunto write  */
/*
for (i=0; i<nmod; i++)
{
   for (j=0; j<nrip; j++)
   {
   for (i1=0; i1<nmod; i1++)
      if(scambio[i][j].write[i1])
       printf("dt=%d  %d->%d\n",j,i,i1);
   }
}
*/

/* riassunto read */
/*
printf("processi di read\n");
for (i=0; i<nmod; i++)
{
  printf("task %d\n",i);
   for (j=0; j<nrip; j++)
   {
   for (i1=0; i1<nmod; i1++)
      if(scambio[i][j].read[i1])
       printf("dt=%d  %d<-%d\n",j,i,i1);
   }
}
*/
}


void write_S02_net(formato_dati)
int formato_dati;
{
FILE *fp_s02;
int r_mod,w_mod;
int tipo;
int app_int;
float app_float;
MODEL app_model;
TABEL app_tabel;
SCAMBIO app_scambio;
int i,j,k;

#if defined AIX || defined SCO_UNIX
	tipo=formato_dati;
#endif
#if defined HELIOS
	tipo=formato_dati;
#endif
#if defined ULTRIX || defined OSF1 || defined LINUX
	tipo=formato_dati-3;
#endif
#if defined VMS
	tipo=formato_dati-6;
#endif

if(tipo==1)
  if (!(fp_s02=fopen("S02_AIX","w")))
     exit(fprintf(stderr," errore apertura file S02_AIX\n"));
if(tipo==2)
  if (!(fp_s02=fopen("S02_ULTRIX","w")))
     exit(fprintf(stderr," errore apertura file S02_ULTRIX\n"));
if(tipo==3)
  if (!(fp_s02=fopen("S02_VMS","w")))
     exit(fprintf(stderr," errore apertura file S02_VMS\n"));




/* scrittura del file S02 in C */

memcpy(&app_int,converti_int_f(&nmod,TRASMISSIONE,formato_dati),sizeof(int));
fwrite( &app_int, sizeof( int), 1, fp_s02);
for(i=0;i<nmod;i++)
	{
	memcpy(&app_model,&s02_.model[i], sizeof(MODEL));
	memcpy(&app_model.lun_name,
		converti_int_f(&(app_model.lun_name),
		TRASMISSIONE,formato_dati),sizeof(int));
	memcpy(&app_model.lun_path,
		converti_int_f(&(app_model.lun_path),
		TRASMISSIONE,formato_dati),sizeof(int));
	memcpy(&app_model.dt,
		converti_float_f(&(app_model.dt),
		TRASMISSIONE,formato_dati),sizeof(float));
	memcpy(&app_model.tipo_task,
		converti_int_f(&(app_model.tipo_task),
		TRASMISSIONE,formato_dati),sizeof(int));
	memcpy(&app_model.punt_ivus,
		converti_int_f(&(app_model.punt_ivus),
		TRASMISSIONE,formato_dati),sizeof(int));
	memcpy(&app_model.punt_ivin,
		converti_int_f(&(app_model.punt_ivin),
		TRASMISSIONE,formato_dati),sizeof(int));
	memcpy(&app_model.punt_ider,
		converti_int_f(&(app_model.punt_ider),
		TRASMISSIONE,formato_dati),sizeof(int));
	memcpy(&app_model.punt_ifine,
		converti_int_f(&(app_model.punt_ifine),
		TRASMISSIONE,formato_dati),sizeof(int));
	memcpy(&app_model.num_ic,
		converti_int_f(&(app_model.num_ic),
		TRASMISSIONE,formato_dati),sizeof(int));
	for(j=0;j<MAX_COLL;j++)
	  {
	  memcpy(&app_model.punt_ing[j],
	     converti_int_f(&(app_model.punt_ing[j]),
		TRASMISSIONE,formato_dati),sizeof(int));
	  memcpy(&app_model.punt_usc[j],
	     converti_int_f(&(app_model.punt_usc[j]),
		TRASMISSIONE,formato_dati),sizeof(int));
	  }

//	printf("\t\terzo: app_model[i=%d].host=%s\n", i, &app_model.host);

	fwrite( &app_model, sizeof(MODEL), 1, fp_s02);
	}
memcpy(&app_int,converti_int_f(&nrip,TRASMISSIONE,formato_dati),sizeof(int));
fwrite( &app_int, sizeof( int), 1, fp_s02);
for(i=0;i<nmod;i++)
	{
	memcpy(&app_tabel,&s02_.tabel[i], sizeof(TABEL));
	memcpy(&app_tabel.forzatura,
		converti_int_f(&(app_tabel.forzatura),
		TRASMISSIONE,formato_dati),sizeof(int));
	for(j=0;j<MAX_DT_DEC;j++)
	  memcpy(&app_tabel.inst[j],
		converti_int_f(&(app_tabel.inst[j]),
		TRASMISSIONE,formato_dati),sizeof(int));
	fwrite( &app_tabel, sizeof(TABEL), 1, fp_s02);
	}
fwrite( macro_modello, sizeof(s02_.macro_modello), 1, fp_s02);
memcpy(&app_int,converti_int_f(&lun_macro_modello,
	TRASMISSIONE,formato_dati),sizeof(int));
fwrite( &app_int, sizeof(int), 1, fp_s02);
fwrite( descr_macro_modello, sizeof(s02_.descr_macro_modello), 1, fp_s02);
#if defined BANCO_MANOVRA
fwrite( s02_.host_bm, sizeof(s02_.host_bm), 1, fp_s02);
#endif
#if defined SCADA
fwrite( s02_.host_scada, sizeof(s02_.host_scada), 1, fp_s02);
#endif
#if defined BANCO_ISTRUTTORE
fwrite( s02_.host_bi, sizeof(s02_.host_bi), 1, fp_s02);
#endif

fflush(fp_s02);

for(r_mod=0;r_mod<MAX_MODEL;r_mod++)
        for(w_mod=0;w_mod<MAX_MODEL;w_mod++)
          {
	  memcpy(&app_int, converti_int_f
		(&(collegamento[r_mod][w_mod].numero_collegamenti),
			TRASMISSIONE,formato_dati),sizeof(int));
          fwrite (&app_int,sizeof(int),1,fp_s02);
          if(collegamento[r_mod][w_mod].numero_collegamenti)
            {
	    for(i=0;i<collegamento[r_mod][w_mod].numero_collegamenti;i++)
		{
		memcpy(&app_int, converti_int_f(
		   &(collegamento[r_mod][w_mod].ingresso[i]),
			TRASMISSIONE,formato_dati),sizeof(int));
		fwrite(&app_int,sizeof(int),1,fp_s02);
		}
	    for(i=0;i<collegamento[r_mod][w_mod].numero_collegamenti;i++)
		{
		memcpy(&app_int, converti_int_f(
		   &(collegamento[r_mod][w_mod].uscita[i]),
			TRASMISSIONE,formato_dati),sizeof(int));
		fwrite(&app_int,sizeof(int),1,fp_s02);
		}
            }
          }

fflush(fp_s02);

for(r_mod=0;r_mod<MAX_MODEL;r_mod++)
   for(w_mod=0;w_mod<MAX_DT_DEC;w_mod++)
    {
	memcpy(&app_scambio,&scambio[r_mod][w_mod],sizeof(SCAMBIO));
	for(i=0;i<MAX_MODEL;i++)
		{
		memcpy(&app_scambio.read[i],converti_int_f
		   (&(scambio[r_mod][w_mod].read[i]),
			TRASMISSIONE,formato_dati),sizeof(int));
		memcpy(&app_scambio.write[i],converti_int_f
		   (&(scambio[r_mod][w_mod].write[i]),
			TRASMISSIONE,formato_dati),sizeof(int));
		}
	fwrite(&app_scambio,sizeof(SCAMBIO),1,fp_s02);
    }

fclose(fp_s02);
if(tipo==2)
	system("cp S02_ULTRIX S02_OSF1");
	system("cp S02_ULTRIX S02_LINUX");
if(tipo==1)
	system("cp S02_AIX S02_SCO");
}
