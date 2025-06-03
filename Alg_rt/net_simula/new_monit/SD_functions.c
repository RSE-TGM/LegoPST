/**********************************************************************
*
*       C Source:               SD_functions.c
*       Subsystem:              1
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed May 21 17:32:26 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: SD_functions.c-4 %  (%full_filespec: SD_functions.c-4:csrc:1 %)";
#endif

#include <stdio.h>
#include <X11/Xlib.h>
#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/Xm.h>

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

extern int _MAX_SNAP_SHOT;

#include "option.h"
#include "parametri.h"
#include "archivi.h"
#include "bistrutt.h"
#include "malfunzioni.h"
#include "btsnapfiles.h"
#include "messaggi.h"
#include "refresh.h"

SNTAB		header_snap_copy;
char     *dati_snap_copy;
static int inizio_sequenza=0; /* per allocazione dinamica memoria per copy */
extern   int size_area_dati;
extern   int size_area_spare;
extern   int size_header_snap;
extern   int size_blocco_snap;
extern   VAL val;           /* struttura per display         */

extern char *FILES_PATH;
/****************************************************************/
/*
 legge da file gli scenari registrati e li carica nella struttura puntata
 da dati
 */

int SD_listascenari (processo,tipo, dati)
int processo;
int tipo;
SCENARI *dati;
{
SCENARI *p;
FILE *fp;
char path[400];
int nscen;

	p = (SCENARI*)dati;
/*
 * testa l' esistenza del file
 * se non esiste lo crea
 */
	strcpy (path, FILES_PATH);
   strcat (path,"/");
	switch (tipo)
		{
		case MALFUNZIONI:
			strcat (path,MALF_SCEN_FILE);
			break;
		case FUNZIONI_REMOTE:
			strcat (path,FREM_SCEN_FILE);
         break;
		default:
			fprintf (stderr,"SD_listascenari: Tipo errato %d\n",tipo);
			break;
		}

printf ("SD_listascenari: open [%s] \n",path);
	while ((fp = fopen (path,"r")) == NULL)
		{
		printf ("il file %s non esiste: crea vuoto\n",path);
		if ((fp = fopen (path,"w")) == NULL)
			{
			printf ("impossibile creare file scenari <<<<<<<<\n");
			return (-1);
			}
		clear_scenari(dati);
		fwrite (dati,sizeof(SCENARI), 1,fp);
		rewind (fp);
		printf ("ListaScen: scenari inizializzati\n");
		}
	nscen = read_scenari (fp,dati);
	printf ("ListaScen: scenari Letti\n");
	fclose (fp);
	return (nscen);
}
/****************************************************************/
/*
 * legge gli scenari malfunzioni e funzioni remote
 */
int read_scenari (fp, p)
FILE *fp;
SCENARI *p;
{
int n;
int i;

/*
 * lettura numero scenari registrati
 */
	if (fread(&n, sizeof(int), 1, fp) <= 0)
		{
		printf (" errore lettura header file scenari\n");
		perror ("errore lettura header file scenari");
		return (-1);
		}
	p->n = n;  /* numero scenari registrati */
/*
 * lettura flag di validita' (scenario registrato)
 */
	for (i=0; i<MAX_SCENARI; i++)
		{	
		if (fread(&p->valido[i], sizeof(int), 1, fp) <= 0)
      	{
      	printf (" errore lettura header 2 file scenari\n");
      	perror ("errore lettura header 2 file scenari");
      	return (-1);
      	}
		}
/*
 * lettura dati di configurazione
 */
	for (i=0; i<MAX_SCENARI; i++)
		{
		if (fread ((char *)&p->s[i], sizeof(SCENARIO), 1, fp) <= 0)
			{		
			printf (" errore lettura file scenari\n");
  		   perror ("errore lettura file scenari");
      	return (-1);
			}
		}
	for (i=0; i<MAX_SCENARI; i++)
      {  
		if (p->valido[i] >= 0)
			printf ("scenario %d valido :%s: \n",i,p->s[i].scen_header.commento);
		}
	return (n);
}
/****************************************************************/
/*
 * azzera la struttura dati degli scenari
 */
int clear_scenari (p)
SCENARI *p;
{
int i;
int k;
	p->n = 0;
	for (i=0; i<MAX_SCENARI; i++)
      {
		p->valido[i] = -1;
		strcpy (p->s[i].scen_header.nome_file, "");
      strcpy (p->s[i].scen_header.data, "");
      strcpy (p->s[i].scen_header.commento, "");
      strcpy (p->s[i].scen_header.sessione, "");
      p->s[i].scen_header.numvar = -1;
      p->s[i].scen_header.numvar = 0.0;
		for (k=0; k<MAX_VAR_SCENARIO; k++)
			{
			strcpy (p->s[i].rec[k].label,"");
			p->s[i].rec[k].valido = -1;
			p->s[i].rec[k].stato = 0;
			p->s[i].rec[k].stato_prec = 0;
			p->s[i].rec[k].punt_comp = -1;
			p->s[i].rec[k].p_main_var = -1;
			p->s[i].rec[k].p_sec1_var = -1;
			p->s[i].rec[k].p_sec2_var = -1;
			p->s[i].rec[k].p_sec3_var = -1;
			p->s[i].rec[k].p_sec4_var = -1;
			p->s[i].rec[k].delay = 0.0;
			p->s[i].rec[k].durata =0.0;
			p->s[i].rec[k].durataInf = 0;
			p->s[i].rec[k].time_to = 0.0;
			p->s[i].rec[k].target = 0.0;
			p->s[i].rec[k].valSec1 = 0.0;
			p->s[i].rec[k].valSec2 = 0.0;
			p->s[i].rec[k].valSec3 = 0.0;
			p->s[i].rec[k].valSec4 = 0.0;
			}
		}
	return (0);
}
/****************************************************************/
int SD_optsave3 (processo, dati, IcDati)
int processo;
char *dati;
Boolean *IcDati;
{
   if (write_options(dati,IcDati) == 0)
      return(1);
   return(0);
}
/****************************************************************/
int SD_optload (processo, dati, IcDati)
int processo;
char *dati;
Boolean *IcDati;
{
   if (lettura_opzioni(dati, IcDati) == 0)
      return (1);
   return (0);
}
/****************************************************************/
int SD_snapview (processo, dati)
int processo;
SNAP_VIEW *dati;
{
int esito = 1; 
int i;
int ret;
FILE *fp;
char *filename;

	switch (dati->which)
		{
		case CI_VIEW:
			printf ("lettura da ci %d\n",dati->record);
			filename = (char *)malloc(strlen(CI_FILE)+10);
			strcpy (filename, CI_FILE);
			if (!(fp = fopen(filename,"r")))
            {
            return (-1);
            }
			for (i=0; i<MAX_VIEW_ENTRY; i++)
				{
				if (dati->var_val[i].puntatore >= 0)
					{
					ret = leggi_da_ci (fp,
										    dati->var_val[i].puntatore,
											 &dati->var_val[i].valore,
											 dati->record);
					if (ret < 0)
						esito = -1;
					}
				}
			break;

		case BT_VIEW:
			printf ("lettura da bt %d\n",dati->record);
			filename = (char *)malloc(strlen(BT_FILE)+10);
		   strcpy (filename, BT_FILE);
   		if (!(fp = fopen(filename,"r")))
      		{
      		return (-1);
      		}
			for (i=0; i<MAX_VIEW_ENTRY; i++)
            {
            if (dati->var_val[i].puntatore >= 0)
               {
               ret = leggi_da_bt (fp,
											 dati->var_val[i].puntatore,
                                  &dati->var_val[i].valore,
                                  dati->record);
               if (ret < 0)
                  esito = -1;
               }
            }
			fclose (fp);
			free (filename);
			break;
		default:
			printf ("which non noto %d\n",dati->which);
			break;
		}
	return (esito);
}
/****************************************************************/
int SD_snappaste (processo,n)
int processo;
int *n;
{
int p;
int esito = 1;
int i;
int ret;
FILE *fp;
char *filename;
int size;
int offset;

   p = *n;
   printf (">>>>>>>>>>>>>>>>>>scrittura su ci %d\n",p);
   size = size_blocco_snap;
   filename = (char *)malloc(strlen(CI_FILE)+10);
   strcpy (filename, CI_FILE);
   if (!(fp = fopen(filename,"r+")))
      {
      return (-1);
      }
/*
 * copia header
 */
   offset = (p-1)*sizeof(SNTAB)+ sizeof(HEADER_REGISTRAZIONI);
   fseek(fp, offset, SEEK_SET);
	header_snap_copy.prog = p;
   header_snap_copy.pos = p; 
   fwrite(&header_snap_copy,sizeof(SNTAB),1,fp);
printf ("paste ci %d descr %s [valido = %d]\n",
			header_snap_copy.prog,
			header_snap_copy.descr,
			header_snap_copy.stat);
/*
 * copia i dati
 */
	offset = size_header_snap + (p-1)*size_blocco_snap;
   fseek(fp, offset, SEEK_SET);
   fwrite(&dati_snap_copy[0],size,1,fp);
	fflush (fp);
/*
 * il buffer resta disponibile per altri paste
	free(dati_snap_copy);
	inizio_sequenza = 0;
 */
   fclose (fp);

   return(1);
}
/****************************************************************/
int SD_snapcopy (processo,n)
int processo;
int *n;   /* da 1 a ... */
{
/*
 * copia una CI in un buffer
 */
int p;
int esito = 1;
int i;
int ret;
FILE *fp;
char *filename;
int size;
int offset;

	size = size_blocco_snap;

	if (!inizio_sequenza)
		dati_snap_copy = (char *)malloc (size);

	printf ("allocati %d bytes per copy\n",size);

   p = *n;

   printf (">>>>>>>>>>>>>>>>>>lettura da ci %d\n",p);
   filename = (char *)malloc(strlen(CI_FILE)+10);
   strcpy (filename, CI_FILE);
   if (!(fp = fopen(filename,"r")))
   	{
   	return (-1);
   	}
/*
 * copia header
 */
	offset = (p-1)*sizeof(SNTAB) + sizeof(HEADER_REGISTRAZIONI);
	fseek(fp, offset, SEEK_SET);
	fread(&header_snap_copy,sizeof(SNTAB),1,fp);
printf ("copiato ci %d descr %s [valido = %d]\n",
			header_snap_copy.prog,
			header_snap_copy.descr,
			header_snap_copy.stat);
/*
 * copia i dati
 */
	offset = size_header_snap + (p-1)*size_blocco_snap;
	fseek(fp, offset, SEEK_SET);
	fread(&dati_snap_copy[0],size,1,fp);

	fclose (fp);

	inizio_sequenza = 1;

   return(1);
}
/****************************************************************/
int SD_cancellaic (processo,n)
int processo;
int *n;   /* da 1 a ... */
{
int p;
int esito = 1;
int i;
int ret;
FILE *fp;
char *filename;
int size;
int offset;
SNTAB header_to_cut;
/*
 * cancella una ci e la rende disponibile in un buffer per operazione 
 * di paste (o undo)
 */
/* prima copia la CI nel buffer, quindi cancella l'header
 */
	if (SD_snapcopy(processo, n) <= 0)
      return(-1);

	p = *n;
   printf (">>>>>>>>>>>>>>>>>>lettura da ci %d\n",*n);
   filename = (char *)malloc(strlen(CI_FILE)+10);
   strcpy (filename, CI_FILE);
   if (!(fp = fopen(filename,"r+")))
      return (-1);
/*
 * copia header
 */
   offset = (p-1)*sizeof(SNTAB) + sizeof(HEADER_REGISTRAZIONI);
   fseek(fp, offset, SEEK_SET);
   fread(&header_to_cut,sizeof(SNTAB),1,fp);
/*
 * azzera e riscrive
 */
	header_to_cut.stat = 0;
	header_to_cut.prog = p;
   header_to_cut.pos = 0;
   header_to_cut.mod = 0;
   strcpy(header_to_cut.descr, " >>>>    SNAPSHOT FREE    <<<<\00");
   strcpy(header_to_cut.datasn, "00/00/00\00");
   strcpy(header_to_cut.temposn, "0\00");

	fseek(fp, offset, SEEK_SET);
	fwrite (&header_to_cut,sizeof(SNTAB),1,fp);

	fclose (fp);

   return(1);
}
/****************************************************************/
int SD_checkci (processo,n)
int processo;
int *n;   /* da 1 a ... */
{
int p;
int esito = 1;
int i;
int ret;
FILE *fp;
char *filename;
int size;
int offset;
SNTAB header;
/*
 * riporta a 0 il campo mod di un header (smarca la modifica di net_compi)
 * 
 */
	p = *n;
   filename = (char *)malloc(strlen(CI_FILE)+10);
   strcpy (filename, CI_FILE);
   if (!(fp = fopen(filename,"r+")))
      return (-1);
/*
 * copia header
 */
   offset = (p-1)*sizeof(SNTAB)+ sizeof(HEADER_REGISTRAZIONI);
   fseek(fp, offset, SEEK_SET);
   fread(&header,sizeof(SNTAB),1,fp);
/*
 * azzera e riscrive
 */
   header.mod = 0;
   fseek(fp, offset, SEEK_SET);
   fwrite (&header,sizeof(SNTAB),1,fp);

   fclose (fp);

   return(1);
}
/****************************************************************/
int SD_btrecstep (processo, dati)
int processo;
float *dati;
{
	val.actual.passo_reg_bt = *dati;

	printf ("SD_btrecstep: settato %f\n",val.actual.passo_reg_bt);
   return (1);
}
/****************************************************************/
int SD_maxtime (processo, dati)
int processo;
float *dati;
{
   if (*dati < val.actual.tempo_sim)
      *dati = val.actual.tempo_sim;

/*
 * flags per display
 */
   val.flags.max_time = 1;
   val.actual.max_time = *dati;

	return (1);
}
/****************************************************************/
int SD_savescenari (processo,tipo,dati)
int processo;
int tipo;
SCENARI *dati;
{
char nome[MAX_PATH_LEN];
FILE *fp;

	strcpy (nome, FILES_PATH);
   strcat (nome,"/");
	if (tipo == MALFUNZIONI)
   	strcat (nome,MALF_SCEN_FILE);
	else if (tipo == FUNZIONI_REMOTE)
		strcat (nome,FREM_SCEN_FILE);

   if ((fp = fopen (nome,"w")) == NULL)
		{
		fprintf (stderr,"File :%s: non accessibile\n",nome);
      return (-1);
		}
	fwrite (dati,sizeof(SCENARI), 1,fp);
	fclose (fp);

	fprintf (stderr,"File :%s: aggiornato\n",nome);
	return (1);
}
