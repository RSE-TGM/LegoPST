/**********************************************************************
*
*       C Source:               graphics_io.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Feb 22 17:03:14 2007 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: graphics_io.c-14 %  (%full_filespec: graphics_io.c-14:csrc:1 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)grsfio.c	1.2\t6/16/93";
/*
   modulo grsfio.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)grsfio.c	1.2
*/
/*
 * grsfio.c
 *    contiene tutte le routine di lettura-scrittura su file relative
 *    all'applicazione graf.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#if defined UNIX
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#endif
#if defined VMS
#include <types.h>
#include <stat.h>
#endif
#include <X11/Xlib.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Mrm/MrmPublic.h> 
#if defined VMS
#include <unixio.h>
#include <file.h>
#endif

#include "sim_param.h"
#include "sim_types.h"
#include "f22_circ.h"

#include "libutilx.h"
#include "uni_mis.h"
#include "grsf22.h"
#include "graphics.h"
#if defined UNIX
#include <unistd.h>
#endif
#include <Rt/RtMemory.h>
 
                 
/*
 variabili di uso comune alle routines di I/O
 */
extern float t_iniziale;
extern float t_old;
extern float t_finale;
/********** extern S_DATI_SIM dato; **********/     /* Non serve */
extern S_SEL_DATI *bufdati;
extern S_GRAFICO sg;
extern S_MIN_MAX *min_max;   /* valori minimi e massimi per ogni
                              * variabile. Vecchia allocazione statica di
                              * NUM_VAR  */
extern S_HEAD1 header1;
extern S_HEAD2 header2;
extern char **nomi_misure;   /* tabella dei nomi delle misure con terminatore 
                              * 0 aggiunto. Vecchia allocazione statica di 
                              * NUM_VAR+1, LUN_NOME+1 */
extern char **simboli;
extern int n_last;           /* posizione dell'ultimo campione nel buffer 
                              * circolare */

extern XmString x_gruppi[NUM_GRUPPI+1]; /* elenco dei gruppi */
extern REC_GRUPPO gruppi[NUM_GRUPPI];
extern char lista_gruppi[NUM_GRUPPI][LUN_SIMB+1];

int inizio_dati;
fpos_t posizione_iniziale;
int fine_dati; /* posizione ultimo dato */
                   /* File dei dati f22circ.dat e f22 selezionato  */
/* 
   Apertura fopen e open
*/
FILE *fpDAT;
FILE *fpda;       
int opDAT;
int opda;
int opvar;

FILE *fpGR;   /* file per memorizzazione gruppi */
extern char *path_22dat;
char path_name[FILENAME_MAX];
char nomefiledati[FILENAME_MAX];

int iniz_file;
int fine_file;
int file_pi;
int file_pf;

F22CIRC_HD load_header;  /* primo header letto */

/*
	variabili per la nuova gestione del file circolare
*/
extern PUNT_FILE_F22 file_f22;
extern char *nome_file_f22;

float tsec;   /* tempo in secondi da memorizzare nel buffer dati;
                                 viene incrementato di INC_SEC secondi per ogni
                                 dato che viene inserito nel buffer */
int b_wait;        /* segnala che e' stato settato il cursore di wait */


int alloca_bufdati();
int dim_bufdati;
int check_header();
static int read_nomi_circ(F22CIRC_HD *);
extern int read_gruppi(int);


extern int    _MAX_SNAP_SHOT;
extern int    _MAX_BACK_TRACK;
extern int    _MAX_CAMPIONI;
extern int    _NUM_VAR;
extern int    _MAX_PERTUR;
extern int    _SPARE_SNAP;
extern int    _PERT_CLEAR;
extern int    RileggiF22Par;

/*
   Apre i file f22circ.dat e il file selezionato
   Usa la open con gli identificatori interi !!!!!!!
*/
open_22dat_circ()
{
int i,fi,fj;
char *punt;

/*
	nuova apertura del file
*/
   nome_file_f22=malloc(strlen(path_22dat)+10);
   strcpy(nome_file_f22,path_22dat);
/*
   printf("NUOVA APERTURA: [%s]\n",nome_file_f22);
*/

/* Si carica path_name usato per l'apertura dei gruppi (f22_fgraf.edf) */
   strcpy(path_name,path_22dat);
   punt = strstr(path_name,"f22");
   if(punt!=NULL)
        *punt = 0;

   return(0);
}

/*
   Chiude i file f22circ.dat e il file selezionato
   Usa la close !!!!!!!
*/
close_22dat_circ()
{
}






/*
    Aggiorna i valori di minimo e massimo per ogni variabile 
    appartenente al record.  Se il parametro passato come argomento
    e' =NULL inizializza a valori estremi i valori di minimo e massimo.
*/
set_min_max_circ(int nvar, float *valori_var)
{
register int i;
float delta;
int variato;

for(i=0;i<nvar;i++)
        {
        if(valori_var==NULL)
                {
                min_max[i].max=(-1.0E-37);
                min_max[i].min=1.0E+38;
                }
        else
                {
                variato=0;
                if(min_max[i].min>valori_var[i])
                        {
                        min_max[i].min=valori_var[i];
                        variato=1;
                        }
                if(min_max[i].max<valori_var[i])
                        {
                        min_max[i].max=valori_var[i];
                        variato=1;
                        }
                if(variato && (min_max[i].max>=min_max[i].min)
                    && (min_max[i].max-min_max[i].min)<=(0.001*min_max[i].max))
                        {
                        delta=min_max[i].max * 0.001; 
                        if(delta <0.001) delta=0.001;
                        min_max[i].max+=delta;
                        min_max[i].min-=delta;
                        }
                }
	}
}



/*
    Aggiorna i valori di minimo e massimo delle variabili selezionate
    a causa dell'aggiornamento sulla finestra temporale mobile.
    In questo modo si coregge la scala del primo valore rappresentato
    nel buffer circolare.
*/
modif_min_max(int ncamp, int *ind_mis, S_SEL_DATI *buf)
{
int i,k;
float delta;
static float min[4];
static float max[4];
float oldmin,oldmax;


/* Inizializzazioni */
   for (k=0;k<4;k++)
      {
      min[k] = 1.0E+38;
      max[k] = (-1.0E-37);
      }

/* Cerco i minimi e i massimi nel buffer */
for(i=0;i<ncamp;i++)
   {
   for (k=0;k<4;k++)
      {
      if (ind_mis[k]!=-1)
         {
         if( buf[i].mis[k]>max[k] )
             max[k] = buf[i].mis[k];
         if( buf[i].mis[k]<min[k] )
             min[k] = buf[i].mis[k];
         }
      }
   }



/* Correggo i valori di min_max */
for(i=0;i<4;i++)
   {
   if (ind_mis[i]!=-1)
      {
      min_max[ind_mis[i]].min = min[i];
      min_max[ind_mis[i]].max = max[i];

/*    Correzione per scale piccole */
      if( (min_max[ind_mis[i]].max>=min_max[ind_mis[i]].min) &&
          (min_max[ind_mis[i]].max-min_max[ind_mis[i]].min)<=
               (0.001*min_max[ind_mis[i]].max) )
               {
               /* printf("differenza=%f confrontata con=%f\n",
                          min_max[ind_mis[i]].max-min_max[ind_mis[i]].min,
                          (0.001*min_max[ind_mis[i]].max));*/
               delta=min_max[ind_mis[i]].max * 0.001; 
               if(delta <0.001) delta=0.001;
               min_max[ind_mis[i]].max+=delta;
               min_max[ind_mis[i]].min-=delta;
               }
      }     /*  Fine if        */
   }        /*  Fine ciclo for */
}



/*
    Legge i nomi dal file f22circ.hd, contenente 
    la sezione header.
    Il file e' gia' aperti.
*/
int read_nomi_circ(F22CIRC_HD *head)
{
int row,col;
char *pdata;
int i,k;
int ret;
F22CIRC_VAR *descr;
int num_descr;
FILE *fff;
SIMULATOR simpar;
static int Prima_volta=1;


   if(f22_open_file(nome_file_f22, &file_f22)==0)
	{
	printf("Open fileKO\n");
	printf("Nome file %s\n",nome_file_f22);
	return(-1);
	}
   /*printf("ORA LEGGO_HEADER con RileggiF22Par=%d\n",RileggiF22Par);*/
   if(f22_leggo_header(head,file_f22,RileggiF22Par,&simpar)==0)
	{
	printf("Leggo Header KO");
	return(-1);
	}
   else
        {
/*      ALLOCAZIONI PARAMETRI PER LETTURA HEADER */
        if ( RileggiF22Par==1 )
           {
           RileggiF22Par=0;
           if( min_max!=NULL )
               {
               /*fprintf(stderr,"\t\t free(min_max)\n");*/
               free(min_max);
               /*fprintf(stderr,"\t\t Alloco min_max size=%d\n",
                         simpar.num_var*sizeof(S_MIN_MAX));*/
               min_max     = (S_MIN_MAX*)calloc(simpar.num_var,sizeof(S_MIN_MAX));
               }
           if( nomi_misure!=NULL )
               {
               /*fprintf(stderr,"\t\t cfree2(nomi_misure)\n");*/
               cfree2(nomi_misure);
               /*fprintf(stderr,"\t\t Alloco nomi_misure size=%d\n",
                       (simpar.num_var+1)*(LUN_NOME+1));*/
               nomi_misure = (char **)cdim2(simpar.num_var+1,LUN_NOME+1);
               }
           if( Prima_volta==1 )
               {
               Prima_volta=0;
               /*fprintf(stderr,"\t\t PRIMA Allocazione size=[%d %d]\n",
                        simpar.num_var*sizeof(S_MIN_MAX),
                        (simpar.num_var+1)*(LUN_NOME+1));*/
               min_max    =(S_MIN_MAX*)calloc(simpar.num_var,sizeof(S_MIN_MAX));
               nomi_misure=(char **)cdim2(simpar.num_var+1,LUN_NOME+1);
               }
           fflush(stderr);
           }
        }


/* Allocazioni */
   row = head->num_var_graf + 1;
   col = LUN_SIMB + 1;

/* Anche f22_leggo_nomi_var leggera' l'header */
   if(f22_leggo_nomi_var(file_f22,&descr,&num_descr)==0)
	{
	printf("Leggo nomivar KO\n");
	return(-1);
	}
   if(f22_close_file (file_f22)==0)
	{
	 printf("Closw fileKO\n");
	return(-1);
	}
   pdata   = (char *)XtCalloc(row*col,sizeof(char));
   simboli = (char **)XtCalloc(row,sizeof(char *));


/* Caricamento della variabile nomi_misure */
   for(i=0;i<head->num_var_graf;i++)
      {
      strcpy(nomi_misure[i],descr[i].nomevar);
      for(k=0;k<LUN_NOME;k++)
          if(nomi_misure[i][k]==' ') nomi_misure[i][k]=0;
      nomi_misure[i][LUN_NOME]=0;
      }
   nomi_misure[i][0]=0;  /* tappo */

/* Allocazione dei puntatori del vettore simboli */
   for(i=0;i<row;i++)
      {
      simboli[i]=pdata;
      pdata+=col;
      }

/* Caricamento della variabile nomi_misure */
   for(i=0;i<head->num_var_graf;i++)
      {
      sprintf(&simboli[i][0],"%s \00",descr[i].nomevar);
      sprintf(&simboli[i][LUN_NOME+1],"%s",descr[i].descvar);
      simboli[i][LUN_SIMB]=(char)0;
      /*printf("%s|\n",simboli[i]);*/
      }
   simboli[i][0]=(char)0;

/* Variabili mantenute perche' usate in graphics.c */
   header1.nvar  = head->num_var_graf;
   header2.ncasi = head->num_var_graf;

   free(descr);

   return(0);
}


/*
 *   routine di apertura in lettura-scrittura del file contenente le 
 *   informazioni relative ai gruppi: se l'apertura in lettura non 
 *   riesce viene creato un nuovo file per gruppi inizializzato a 0.
 */
int open_gruppi()
{
int i,k,j;
char appoggio[FILENAME_MAX];
appoggio[0]=0;

   strcpy(appoggio,path_name);
   strcat(appoggio,"f22_fgraf.edf");
   fpGR=fopen(appoggio,"r");
   if(fpGR==NULL)
	{
/*      Se il file non era apribile in lettura lo inizializza a zero */
        printf("   GRUPPI Creazione [%s]\n",appoggio);
	fpGR=fopen(appoggio,"w");
	for(i=0;i<NUM_GRUPPI;i++)
           {
           for(k=0;k<4;k++)
              {
              gruppi[i].gr.ind_mis[k]=(int)-1;
              gruppi[i].gr.ind_umis[k]=(int)-1; /*Aggiunta 27 ott 94*/
              gruppi[i].gr.umis_sel[k]=(int)-1; /*Aggiunta 27 ott 94*/
              gruppi[i].gr.autoscaling[k]=1;
              }
           }
        scrivi_gruppi();
	}
fclose(fpGR);
fpGR=fopen(appoggio,"r+");
if(fpGR==NULL)
	{
	fpGR=fopen(appoggio,"r");
	}
return(0);
}


close_gruppi()
{
int i;
for(i=0;i<NUM_GRUPPI;i++)
	{
	XtFree(x_gruppi[i]);
	}
fclose(fpGR);
}




/*
     Scrive la struttura descrittiva di un gruppo sul file ASCII.
     Effettua la lettura del vecchio file, sostituisce il gruppo
     da eliminare e riscrive il file.
*/
write_gruppo(indice)
int indice;
{
REC_GRUPPO gruppo_tmp;
unsigned long offset;
int ret, flag=0;
   rewind(fpGR);
   memcpy(&gruppo_tmp,&gruppi[indice],sizeof(REC_GRUPPO));
   read_gruppi(flag);
   rewind(fpGR);
   memcpy(&gruppi[indice],&gruppo_tmp,sizeof(REC_GRUPPO));
   ret = scrivi_gruppi();
   return(ret);
}



/*
    Scrive su file il vettore delle strutture di tipo REC_GRUPPO
    gruppo secondo un preciso formato ASCII.
*/
int scrivi_gruppi()
{
int i,j;
int menouno = -1;

   if ( fpGR==NULL)
      {
/*    Ritorno con errore in scrittura */
      printf("Errore scrivi_gruppi, file non aperto\n");
      return(1);
      }
   for(i=0;i<NUM_GRUPPI;i++)
      {
      fprintf(fpGR,"%d\n",gruppi[i].pieno);

      if (gruppi[i].descr[0] == '\n')
         fprintf(fpGR,"%s",gruppi[i].descr);
      else
         fprintf(fpGR,"%s\n",gruppi[i].descr);

      for(j=0;j<4;j++)
         fprintf(fpGR,"%d ",gruppi[i].gr.autoscaling[j]);
      fprintf(fpGR,"\n");

      for(j=0;j<4;j++)
         fprintf(fpGR,"%f %f ",gruppi[i].gr.fix_min_max[j].min,
                               gruppi[i].gr.fix_min_max[j].max);
      fprintf(fpGR,"\n");

      for(j=0;j<4;j++)
         fprintf(fpGR,"%d ",gruppi[i].gr.ind_mis[j]);
      fprintf(fpGR,"\n");

      for(j=0;j<4;j++)
         if( gruppi[i].gr.ind_mis[j]!=-1 )
             fprintf(fpGR,"%d ",gruppi[i].gr.ind_umis[j]);
         else
             fprintf(fpGR,"%d ",menouno);
      fprintf(fpGR,"\n");

      for(j=0;j<4;j++)
         fprintf(fpGR,"%d ",gruppi[i].gr.umis_sel[j]);
      fprintf(fpGR,"\n");

      for(j=0;j<4;j++)
         {
         if( gruppi[i].gr.ind_mis[j]!=-1 )
            {
            if(gruppi[i].gr.descr_mis[j][0] == '\n')
              fprintf(fpGR,"%s",gruppi[i].gr.descr_mis[j]);
            else
              fprintf(fpGR,"%s\n",gruppi[i].gr.descr_mis[j]);
            }
         else
            {
              fprintf(fpGR,"\n");
            }
         }
      }
      fflush(fpGR);
      return(0);
}


write_gruppo_noascii(indice)
int indice;
{
unsigned long offset;
offset=(long)indice*(long)sizeof(REC_GRUPPO);
fseek(fpGR,offset,0);
if(fwrite(&gruppi[indice],sizeof(REC_GRUPPO),1,fpGR)!=1)
	return(1); /* ritorno con errore in scrittura */
else
	return(0);
}




/*
 * open_path()
 *	legge i path names memorizzati sul file F22_FILES.EDF 
 */
FILE *fpPATH;	        
extern char path[NUM_PATH_FILES][FILENAME_MAX];
open_path()
{
int i;
fpPATH=fopen("f22_files.edf","r");

if(fpPATH==NULL)
        {
/* 
  se il file non era apribile in lettura lo inizializza a zero
*/
	fpPATH=fopen("f22_files.edf","w");

	if(fpPATH == NULL)
		{
		printf("\n errore apertura file path\n");
		exit(0);
		}
	for(i=0;i<NUM_PATH_FILES;i++)
	        {
                /*fwrite(path[i],FILENAME_MAX,1,fpPATH);*/
                fprintf(fpPATH,"%s\n",path[i]);
	        }
	}
fclose(fpPATH);
fpPATH=fopen("f22_files.edf","r+");
for(i=0;i<NUM_PATH_FILES;i++)
	{
        /*fread(path[i],FILENAME_MAX,1,fpPATH);*/
        fscanf(fpPATH,"%s",path[i]);
        /*printf("Ho letto [%s] (%d)\n",path[i],strlen(path[i]));*/
	}
fclose(fpPATH);
}
             
/*
 * close_path()
 *    salva i valori attuali dei path names e chiude il file 
 *    F22_FILES.EDF
 */
void close_path()
{
int i;

fpPATH=fopen("f22_files.edf","r+");
fseek(fpPATH,0,0);
ControlName(path);
for(i=0;i<NUM_PATH_FILES;i++)
        {
	/*fwrite(path[i],FILENAME_MAX,1,fpPATH);*/
        fprintf(fpPATH,"%s\n",path[i]);
        /*printf("Ho scritto [%s] (%d)\n",path[i],strlen(path[i]));*/
        }
fclose(fpPATH);
}
                                                  

/*
    Se trova una stringa nulla tra i path assegnati dall'utente elimina
    il vecchio contenuto del file scrivendo una riga bianca.
*/
int    ControlName()
{
int i;
char *blank;

     blank = calloc(FILENAME_MAX+1,sizeof(char));
     memset(blank,32,FILENAME_MAX);
     blank[FILENAME_MAX-1] = 0;
     for(i=0;i<NUM_PATH_FILES;i++)
         {
         if( strlen(path[i])==0 )
             {
             memcpy(path[i],blank,FILENAME_MAX*sizeof(char));
             }
         }
     free(blank);
}

d2free(prow) 
char **prow;
{
XtFree(*prow);
XtFree(prow);
}




/*    ********    ROUTINE PER LA GESTIONE DEL FILE F22CIRCOLARE  ****** */


/*
    Legge dal file ASCII dei gruppi i dati da caricare
    nelle strutture descrittive di ciascun gruppo.
*/
read_gruppi(flag)
int flag;  /*  flag == 1 se si desidera la lista per fase di inserimento
               gruppi  */
{
int i,j;
char tmp[20];
int ngr;
char new_line[10];
int lun;

ngr=0;
for(i=0;i<NUM_GRUPPI;i++)
	{
        fscanf(fpGR,"%d",&gruppi[i].pieno);
        fgets(new_line,10,fpGR);
        fgets(gruppi[i].descr,81,fpGR);
        lun = strlen(gruppi[i].descr);
        gruppi[i].descr[lun-1] = '\0';
        for(j=0;j<4;j++)
           {
           fscanf(fpGR,"%d ",&gruppi[i].gr.autoscaling[j]);
           }
        for(j=0;j<4;j++)
           {
           fscanf(fpGR,"%f %f",&gruppi[i].gr.fix_min_max[j].min,
                               &gruppi[i].gr.fix_min_max[j].max);
           }
        for(j=0;j<4;j++)
           {
           fscanf(fpGR,"%d",&gruppi[i].gr.ind_mis[j]);
           }
        for(j=0;j<4;j++)
           {
           fscanf(fpGR,"%d",&gruppi[i].gr.ind_umis[j]);
           }
        for(j=0;j<4;j++)
           {
           fscanf(fpGR,"%d",&gruppi[i].gr.umis_sel[j]);
           }
        fgets(new_line,10,fpGR);
        for(j=0;j<4;j++)
           {
           fgets(gruppi[i].gr.descr_mis[j],LUN_SIMB+1,fpGR);
           lun = strlen(gruppi[i].gr.descr_mis[j]);
           gruppi[i].gr.descr_mis[j][lun-1] = '\0';
           }
	if(gruppi[i].pieno)
           {
           ngr++;
           sprintf(lista_gruppi[i],"%.2d - %s",i+1,gruppi[i].descr);
           }
	else if (flag==1)
	sprintf(lista_gruppi[i],"%.2d - disponibile -           ", i+1);
        XmStringFree(x_gruppi[i]);
	x_gruppi[i]=XmStringGenerate(lista_gruppi[i],NULL,XmCHARSET_TEXT,NULL);
	}
x_gruppi[i]=NULL;
return(ngr);
}


read_22dat_circ(flag)
char flag;
{
int i;
int ind;      /* indice che scorre il buffer di memorizzazione dati
				 bufval   */
int pieno;   /* indica se il buffer e' stato completamente riempito 
		(variabile utilizzata in fase di prima lettura) */
int num_camp_read; /* numero di campioni letti nel singolo accesso
                      a f22 */
int num_shift; /* indica quanti campioni shiftare in caso di riempimento
                  di bufdati */

F22CIRC_HD header;
SCIRC_SEL_DATI *dati_f22 = NULL;
float tempo;
int zero=0;

if(flag==TUTTI)
	{
/*      Prima lettura : tutti i valori */
	b_wait=1;
/*      Setta il cursore di wait */
	set_cur_wait(); 
	tsec=0.0;
	n_last=0;
	ind=0;
	t_old= 0;
	pieno=0;
/*      Legge le tabelle dei nomi e simboli */
	if(read_nomi_circ(&header)==-1) 
           {
           printf("\n Errore in read_nomi_circ\n");
           return(1);
           }
	/*
		salva l'header
	*/
	memcpy(&load_header,&header,sizeof(F22CIRC_HD));
        if( (header.p_iniz<1)||(header.p_fine<1) )
           {
           printf("Attenzione, non sono presenti dati: p_i=%d p_f=%d\n",
                   header.p_iniz,header.p_fine);
           exit(1);
           }
	/*
		allocazione bufdati
	*/
	dim_bufdati=alloca_bufdati(header);
	bufdati[n_last].t=0;

/*      Inizializzazioni indici */
        iniz_file = -1;
        fine_file = -1;


/* Inizializza il buffer dei minimi e massimi a valori estremi */
	set_min_max_circ(header.num_var_graf,NULL);
	}
else  /* caso di lettura per aggiornamento  */
	{
/*
 Determina dove inserire il nuovo dato
 Se n_last e' <= 0 si posiziona all'inizio di buf dati
 altrimenti si posiziona al campione successivio
 ind punta al prossimo campione da inserire.
*/
	if(n_last <= 0) /* ?????? Poco chiaro ??? */
		{
		tempo = -1.0;
		ind = 0;
		}
	else
		{
		tempo = bufdati[n_last].t;
		ind = n_last+1;
		}

	f22_open_file(nome_file_f22,&file_f22);
        if( RileggiF22Par==1 )
            {
            printf("\t\t in read_22dat_circ vorrebbe RileggereF22 ma lo inpedisco\n");
            }
	f22_leggo_header(&header,file_f22,zero,NULL);
	if(check_header(header) != 1)
		{
/*
 Elimina il timeout e ricarica il file f22
*/
		reset_graphics();
		return;
		}

/**
printf("data header:  %d-%d-%d   %d:%d:%d\n",
		header.giorno,header.mese,header.anno,
		header.ore,header.minuti,header.secondi);
**/
	num_camp_read=f22_leggi_campioni(file_f22,header, &tempo,&dati_f22,
		sg.ind_mis[0],sg.ind_mis[1],sg.ind_mis[2],sg.ind_mis[3]);
	f22_close_file(file_f22);

	i = 0;
/*
 Procede a verificare se i dati letti possono essere inseriti tutti 
 in bufdati o se e' necessario eseguire uno shift dei dati gia'
 memorizzati, prima di effettuare l'inserimento
 Calcola il numero (eventuale) di posizioni che costituiscono 
 l'entita' dello shift.
*/
	num_shift= ind+num_camp_read-dim_bufdati;
/*
 se num_shift e' > 0 bisogna effettuare lo shift sui dati
 gia' presenti in bufdati
*/
	if (num_shift > 0)
		{
/*
 Se le dimensioni dello shift da effettuare non superano il numero
 di dati gia' memorizzati in bufdati -> sposta i dati gia' presenti
 verso sinistra di una quantita' pari a shift e decrementa l'indice al
 prossimo dato da inserire (ind) di una quantita' pari a shift.
 Altrimenti bufdati viene riempito dall'inizio (ind = 0).
*/
		if((ind-num_shift)>0)
			{
        		memmove(&bufdati[0],&bufdati[num_shift],
				(ind-num_shift)*sizeof(S_SEL_DATI));
			ind=ind-num_shift;
			}
		else
			ind=0;
		}
/*
 Copia in bufdati i dati letti dal file circolare
*/
	for(i=0; i< num_camp_read; i++,ind++)
	   {
	   bufdati[ind].mis[0]=dati_f22[i].mis[0];
	   bufdati[ind].mis[1]=dati_f22[i].mis[1];
	   bufdati[ind].mis[2]=dati_f22[i].mis[2];
	   bufdati[ind].mis[3]=dati_f22[i].mis[3];
	   bufdati[ind].t = dati_f22[i].tempo.tempo;
	   }
/*
 Aggiorna il valore del tempo iniziale al tempo che si trova
 all'inizio di bufdati dopo aver effettuato lo shift (eventuale).
*/
        t_iniziale = bufdati[0].t;
/*
	libero il vettore dati_f22
*/
	if(dati_f22)
		free(dati_f22);

        modif_min_max(ind,&(sg.ind_mis[0]),
                     bufdati);


	}
FINE:
if(b_wait)
	{
	b_wait=0;
	clr_cur_wait();
	}
/*
 Posiziona n_last sull'ultimo dato valido inserito
*/
n_last=ind-1;
fine_dati=n_last;
return(0);
}


/*
 Alloca il buffer che contiene i dati da visualizzare.
 Ritorna il numero di campioni a cui e' dimensionato
 il buffer
*/
int alloca_bufdati(F22CIRC_HD header)
{
int size;
float fatt;

if(bufdati)
	free(bufdati);

/*
 Alloca bufdati della dimensione pari al numero di istanti di tempo
 presenti nel file circolare per 4 variabili selezionate
*/
size = (header.num_campioni + 1)*sizeof(S_SEL_DATI);
bufdati=(S_SEL_DATI*)malloc(size);

if(bufdati==NULL)
	exit(fprintf(stderr,"Impossibile allocare il vettore bufdati\n"));

return(header.num_campioni);
}

int check_header(F22CIRC_HD header)
{
if(load_header.ore != header.ore) return(-1);
if(load_header.minuti != header.minuti) return(-1);
if(load_header.secondi != header.secondi) return(-1);
if(load_header.giorno != header.giorno) return(-1);
if(load_header.mese != header.mese) return(-1);
if(load_header.anno != header.anno) return(-1);
return(1);
}
