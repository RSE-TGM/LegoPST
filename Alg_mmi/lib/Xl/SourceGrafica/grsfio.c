/**********************************************************************
*
*       C Source:               grsfio.c
*       Subsystem:              1
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed Jan 22 12:55:21 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: grsfio.c-2.1.2 %  (%full_filespec: grsfio.c-2.1.2:csrc:1 %)";
#endif
/*
   modulo grsfio.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)grsfio.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)grsfio.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*
 * grsfio.c
 *    contiene tutte le routine di lettura-scrittura su file relative
 *    all'applicazione graf.
 */

#include <stdio.h>
#include <stdlib.h>

#if defined AIX || defined ULTRIX
#include <sys/types.h>
#include <sys/stat.h>
#endif

#if defined VMS
#include <types.h>
#include <stat.h>
#endif

#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Mrm/MrmPublic.h> 

#if defined VMS
#include <unixio.h>
#include <file.h>
#endif

#include <libutilx.h>
#include <sim_param.h>
#include <f22_circ.h>
#include <Xl/XlP.h>
#include <Xl/XlGrafico.h>

/* variabili di uso comune alle routines di I/O */
extern S_XLGRAFICO *pXlGraf;

/**************************************************
 open_22dat
   apre il file f22.dat
**************************************************/
int open_22datGR(pXlGraf)
S_XLGRAFICO *pXlGraf;
{
int i,lun;
char *punt;

i=strlen(path_22dat)-1;
strcpy(path_name,path_22dat);

#if defined VMS
  while(path_22dat[i]!=']' && path_22dat[i]!=':' ) i--;
#else
  while(path_22dat[i]!='/' ) i--;
#endif

i++;
if(i>0) path_name[i]=0;

printf ("Apertura file: %s\n",path_22dat);

#if defined UNIX || defined SCADA_MMI
  fpDAT = fopen(path_22dat,"r");
  if(fpDAT == NULL)
    return(1);
#else
  printf("Apertura VMS file %s fp=%d\n",path_22dat,i);
  i=open(path_22dat,O_RDONLY,0004|0002,"ctx=rec","rfm=var","shr=put");
  fpDAT=fdopen(i,"r");
  fseek(fpDAT,0,0);
  if(fpDAT==NULL)
    return(1);
#endif

/*
        nella lettura del file circolare utilizzando le routine di
        libreria non e' necessario tenere registrato il puntatore
        al file; quindi verifico solo l'esistenza del file di header
        e poi lo richiudo
*/
if(tipo_graf == ARCH_CIRC_GRAF)
	{
#if defined UNIX
/*
        Viene eliminata la terminazione con zero al primo punto incontrato
        per compatibilita' con i pathname alla continuus.
*/
        lun = strlen(path_22dat);
        i=1;
        while(i<lun)
           {
           if ( path_22dat[lun-i] == '.' )
               {
               path_22dat[lun-i] = '\0';
               break;
               }
           i++;
           }
#endif
        fclose(fpDAT);
	
	}

return(0);
}

/**************************************************
 close_22dat
   chiude il file f22.dat
**************************************************/
close_22dat()
{
/*
	vedi commento funzione open_22dat
*/
if(tipo_graf != ARCH_CIRC_GRAF)
	fclose(fpDAT);
}


/**************************************************
 read_22datGR
    legge i dati relativi a tutte le misure;
	- flag : AGGIORNA    aggiorna l'area dati con gli ultimi dati acquisiti
	         TUTTI       trasferisce nell'area dati tutti i campioni 
			     presenti nel file.
**************************************************/
int read_22datGR(flag,pXlGraf)
char flag;
S_XLGRAFICO *pXlGraf;
{
F22CIRC_HD header;
static S_DATI buf;   /* buffer di appoggio per la lettura del record dati */
char *pb;
int iret;
float step,correz;
int parziali;
int i,lun;
char sep[2];
int ind;      /* indice che scorre il buffer di memorizzazione dati
				 bufval   */
int pieno;   /* indica se il buffer e' stato completamente riempito 
		(variabile utilizzata in fase di prima lettura) */

if(flag==TUTTI)
  {
  b_wait=1;

  tsec=0.0;
  n_last=0;
  ind=0;
  pieno=0;
  off_f22=0;

if(tipo_graf == ARCH_GRAF)
  {
  if(read_nomi(fpDAT,&off_f22)==1) /* legge le tabelle dei nomi e simboli */
    {
    XlWarning ("XlGrafico","read_22datGR","Errore in read nomi");
    return(1);
    }
  }
else
  {
printf("read_22datGR prima read_nomi_circ\n");
  if(read_nomi_circ(&header,path_22dat)==1)/* legge le tabelle dei nomi e simboli */
    {
    XlWarning ("XlGrafico","read_22datGR","Errore in read nomi");
    return(1);
    }
printf("read_22datGR read_nomi_circ ritorno corretto\n");
  }

  /* lettura da file: si posiziona sul primo dato */
  inizio_dati=off_f22;

  /* lunghezza totale dati da leggere */
  lun_rec_dati=(header1.nvar+1)*sizeof(float);
  printf("Lunghezza del record dati: %d\n",lun_rec_dati);

  /* inizializza il buffer dei minimi e massimi a valori estremi */
  set_min_max(NULL,pXlGraf);

  /* lettura del primo campione:dovra' corrispondere un tempo a 0.0 secondi */	
  if((iret=read_multi(&buf.t,pXlGraf,0.0))==0)
    {
    XlWarning ("XlGrafico","read_22datGR",
		"Dati non presenti in F22 (read_multi)");
    return (1);
    }
  off_f22+=iret;

  /* aggiorna se necessario i valori di minimo e di massimo */
  t_iniziale=buf.t;
  while(read_multi(&buf.t,pXlGraf,buf.t)==lun_rec_dati)
    {
    if (ind < n_elementi_bufdati)
      {
      bufdati[ind].t=buf.t;
      set_min_max(&buf,pXlGraf);
      ind++;
      }
    else
      if (!notify_overflow)
        {
        notify_overflow = True;
        XlWarning ("XlGrafico","read_22datGR",
		"Raggiunta dimensione massima di bufdati");
        }
    }
  printf("Numero dei dati letti: %d\n",ind);
  }
else  /* caso di lettura per aggiornamento  */
  {
  float ultimo_tempo;

  ind=n_last;
  if(n_last==0) 
    b_wait=1;

  if(n_last == 0)
	ultimo_tempo = 0.0;
  else
	ultimo_tempo = bufdati[n_last -1].t;

  while(read_multi(&buf.t,pXlGraf,ultimo_tempo)==lun_rec_dati)
    {
    off_f22+=lun_rec_dati;
    set_min_max(&buf,pXlGraf);
    ultimo_tempo = buf.t;
    if (ind < n_elementi_bufdati)
      {
      for(i=0;i<4;i++)
        {
        if(sg.ind_mis[i]!=-1)
          bufdati[ind].mis[i]=buf.mis[sg.ind_mis[i]];
        }
      bufdati[ind].t=buf.t;
      ind++;
      }
    else
      if (!notify_overflow)
        {
	notify_overflow = True;
        XlWarning ("XlGrafico","read_22datGR",
		"Raggiunta dimensione massima di bufdati");
	}
    }
  }

/*
 e' terminata la scansione del file f22.dat; ind punta alla posizione suc-
 cessiva all'ultima riempita.
*/
FINE:
  if(b_wait)
    b_wait=0;

n_last=ind;
fine_dati=n_last;

return(0);
}

/*************************************************
*************************************************/
read_multi(buf,pXlGraf,ultimo_tempo)
char *buf;
S_XLGRAFICO *pXlGraf;
float ultimo_tempo;
{
char sep[2];
char *pb;
int tot_letti=0; /* lunghezza totale bytes letti */
int lun,i;
int ret;
float tempo;
int zero=0;

if(tipo_graf == ARCH_GRAF)
	{
	pb=buf;
	tot_letti=fread(pb,1,lun_rec_dati,fpDAT);
	
	if ((tot_letti!=lun_rec_dati)&&(tot_letti!=0))
  	{
  	while(tot_letti<lun_rec_dati)
    	tot_letti += fread(&pb[tot_letti],1,(lun_rec_dati-tot_letti),fpDAT);
  	}
	
	if(tot_letti==lun_rec_dati)
  	memcpy(&tempo,&pb[0],sizeof(float));
	
	return(tot_letti);
	}
else
	{
	PUNT_FILE_F22 punt_file;
	F22CIRC_HD header_circ;
	int ret;
	float app_t;

	pb=buf;

	f22_open_file(path_22dat,&punt_file);
	f22_leggo_header(&header_circ, punt_file, zero, NULL);
	app_t=ultimo_tempo;
	ret = f22_leggi_ultimo(punt_file,header_circ,&app_t,
		(float*)(&pb[sizeof(float)]));
	if(ret)
		memcpy(&pb[0],&app_t,sizeof(float));
	f22_close_file(punt_file);
	ret = ret * lun_rec_dati;
	return(ret);
	}
}

/*************************************************
*************************************************/
int rew_dati(pXlGraf)
S_XLGRAFICO *pXlGraf;
{
int ret;

n_last=0;
if(tipo_graf == ARCH_CIRC_GRAF)
	return;
ret=fseek(fpDAT,0,0);
ret=fseek(fpDAT,inizio_dati,0);
fsetpos(fpDAT,&posizione_iniziale);
if(ret!=0)
  XlWarning ("XlGrafico","rew_dati","Errore rew_dati");
}


/**********************************************************
 *  set_min_max
 *      aggiorna i valori di minimo e massimo per ogni variabile 
 *      appartenente al record.
 *		se il parametro passato come argomento e' =NULL inizializza
 *      a valori estremi i valori di minimo e massimo.
 **********************************************************/
set_min_max(rec,pXlGraf)
S_DATI *rec;   /* record dati */
S_XLGRAFICO *pXlGraf;
{
register int i;
float delta;
int variato;
for(i=0;i<4;i++)
  {
  if(rec==NULL)
    {
    sg.var_min_max[i].max=(-1.0E-37);
    sg.var_min_max[i].min=1.0E+38;
    }
  else
    {
    variato=0;
    if(sg.var_min_max[i].min>rec->mis[sg.ind_mis[i]])
      {
      sg.var_min_max[i].min=rec->mis[sg.ind_mis[i]];
      variato=1;
      }

    if(sg.var_min_max[i].max<rec->mis[sg.ind_mis[i]])
      {
      sg.var_min_max[i].max=rec->mis[sg.ind_mis[i]];
      variato=1;
      }

    if(variato && (sg.var_min_max[i].max>=sg.var_min_max[i].min)
          && (sg.var_min_max[i].max-sg.var_min_max[i].min)<=
			(0.001*sg.var_min_max[i].max))
      {
      delta=sg.var_min_max[i].max * 0.001; 
      if(delta <0.001)
        delta=0.001;
      sg.var_min_max[i].max+=delta;
      sg.var_min_max[i].min-=delta;
      }
    }
  }

}

/***********************************************
***********************************************/
read_nomi(fp,offset)
int *offset;
FILE *fp;
{
char nomi_misure[LUN_NOME+1];
int row,col,dimens;
char *pdata;
int i,k;

/* lettura del primo header */
if((fread(&header1,sizeof(S_HEAD1),1,fp))==NULL)
  {
  XlWarning ("XlGrafico","read_nomi",
		"Dati non presenti in f22 (lettura S_HEAD1)");
  return (1);
  }

(*offset)+=sizeof(S_HEAD1);

/*  Lettura a vuoto del blocco contenente i nomi delle variabili  */
for(i=0;i<header1.nvar;i++)
  {
  fread(nomi_misure,LUN_NOME,1,fp);
  (*offset)+=LUN_NOME;
  }

/*
if((fread(&header2,sizeof(S_HEAD2),1,fp))==NULL)
  {
  XlWarning ("XlGrafico","read_nomi",
		"Dati non presenti in F22 (lettura S_HEAD2)");
  return (1);
  }
*/
if((fread(header2.nome_mod,LUN_NOME,1,fp))==NULL)
  {
  XlWarning ("XlGrafico","read_nomi",
		"Dati non presenti in F22 (lettura S_HEAD2)");
  return (1);
  }

if((fread(&header2.ncasi,sizeof(int),1,fp))==NULL)
  {
  XlWarning ("XlGrafico","read_nomi",
		"Dati non presenti in F22 (lettura S_HEAD2)");
  return (1);
  }

(*offset)+= (LUN_NOME+sizeof(int));

/* alloca l'array bidimensionale che conterra' la lista delle descrizioni */
row=header2.ncasi+1;
col=LUN_SIMB+1;
pdata = (char *) XtCalloc(row * col,  sizeof(char));
if(pdata==(char *)NULL)
  {
  XlWarning ("XlGrafico","read_nomi","Mancanza di spazio per allocazione");
  return (1);
  }

dimens=row * (sizeof(char *));
simboli = (char **) XtCalloc(row,sizeof(char *));
if(simboli==(char **)NULL)
  {
  XlWarning ("XlGrafico","read_nomi","Mancanza di spazio per allocazione");
  return (1);
  }

for(i=0;i<row;i++)
  {
  simboli[i]=pdata;
  pdata+=col;
  }

for(i=0;i<header2.ncasi;i++)
  {
  fread(simboli[i],LUN_SIMB,1,fp);
  simboli[i][LUN_SIMB]=(char)0;
  (*offset)+=LUN_SIMB;
  }

simboli[i][0]=(char)0;
fgetpos(fp,&posizione_iniziale);
return(0);
}


/*
    Legge i nomi dal file f22circ.hd, contenente
    la sezione header.
    Il file e' gia' aperti.
*/
int read_nomi_circ(F22CIRC_HD *head,char *nome_file_f22)
{
char nomi_misure[NUM_VAR+1][LUN_NOME+1];
int row,col;
char *pdata;
int i,k;
int ret;
F22CIRC_VAR *descr;
int num_descr;
PUNT_FILE_F22 file_f22;
SIMULATOR simpar;
static int Prima_volta=1;
int RileggiF22Par;              /****** P R O V V I S O R I O   ******/

printf("Ingresso read_nomi_circ nome file = [%s]\n",nome_file_f22);


   if(f22_open_file(nome_file_f22, &file_f22)==0)
	{
        XlWarning ("XlGrafico","read_nomi_circ","Errore apertura file");
        return(1);
	}
   RileggiF22Par=0;
   if(f22_leggo_header(head,file_f22,RileggiF22Par,&simpar)==0)
	{
        XlWarning ("XlGrafico","read_nomi_circ","Errore lettura header");
        return(1);
	}
#if defined NUOVO_OGGETTO_GRAPHICS
        Introdotto in previsione di un allineamento a quanto fatto
        per graphics_io.c . Per ora non si prevede la lettura dei
        parametri (NUM_VAR essenzialmente) e nemmeno il loro caricamento.
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
               min_max   = (S_MIN_MAX*)calloc(simpar.num_var,sizeof(S_MIN_MAX));
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
#endif


/* Allocazioni */
   row = head->num_var_graf + 1;
   col = LUN_SIMB + 1;

   if(f22_leggo_nomi_var(file_f22,&descr,&num_descr)==0)
	{
        XlWarning("XlGrafico","read_nomi_circ","Errore lettura nomi variabili");
        return(1);
	}

   if(f22_close_file (file_f22)==0)
	{
        XlWarning("XlGrafico","read_nomi_circ","Errore chiusura file");
        return(1);
	}

printf("read_nomi_circ :  letti i dati dal file\n");

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

   return(0);
}

/***********************************************************
 * open_gruppi()
 *   routine di apertura in lettura-scrittura del file contenente le 
 *   informazioni relative ai gruppi: se l'apertura in lettura non 
 *   riesce viene creato un nuovo file per gruppi inizializzato a 0.
 ***********************************************************/
open_gruppi()
{
int i,k;
char appoggio[150];

appoggio[0] = 0;
strcpy(appoggio,path_name);
strcat(appoggio,"f22_fgraf.dat");

fpGR=fopen(appoggio,"r");
if(fpGR==NULL)
  {
  /* se il file non era apribile in lettura lo inizializza a zero */
  fpGR=fopen(appoggio,"w");

  if(fpGR == NULL)
    {
    XlWarning ("XlGrafico","open_gruppi","Errore apertura file gruppi");
    return(1);
    }
  for(i=0;i<NUM_GRUPPI;i++)
    {
    for(k=0;k<4;k++)
      {
      gruppi[i].gr.ind_mis[k]=(int)-1;
      gruppi[i].gr.autoscaling[k]=1;
      }
    fwrite(&gruppi[i],sizeof(REC_GRUPPO),1,fpGR);
    }
  }

fclose(fpGR);

fpGR=fopen(appoggio,"r+");
if(fpGR==NULL)
  fpGR=fopen(appoggio,"r");

return(0);
}

/*********************************************
**********************************************/
close_gruppi()
{
int i;

for(i=0;i<NUM_GRUPPI;i++)
  XtFree(x_gruppi[i]);

fclose(fpGR);
}

/*************************************************
**************************************************/
read_gruppi(flag)
int flag;  /*  flag == 1 se si desidera la lista per fase di inserimento
               gruppi  */
{
int i;
int ngr;

ngr=0;
for(i=0;i<NUM_GRUPPI;i++)
  {
  fread(&gruppi[i],sizeof(REC_GRUPPO),1,fpGR);
  if(gruppi[i].pieno)
    {
    ngr++;
    sprintf(lista_gruppi[i],"%.2d - %s",i+1,gruppi[i].descr);
    }
  else if (flag==1)
    sprintf(lista_gruppi[i],"%.2d - disponibile -           ",i+1);
  x_gruppi[i]=XmStringCreateLtoR(lista_gruppi[i],XmSTRING_DEFAULT_CHARSET);
  }
x_gruppi[i]=NULL;
return(ngr);
}

/***********************************************
***********************************************/
write_gruppo(indice)
int indice;
{
unsigned long offset;

offset = (long)indice*(long)sizeof(REC_GRUPPO);
fseek(fpGR,offset,0);
if(fwrite(&gruppi[indice],sizeof(REC_GRUPPO),1,fpGR)!=1)
  return(1);       /* ritorno con errore in scrittura */
else
  return(0);
}

/******************************************************
 * open_path()
 *	legge i path names memorizzati sul file F22_FILES.DAT 
 ******************************************************/
void open_path()
{
int i;
fpPATH=fopen("f22_files.dat","r");
if(fpPATH==NULL)
  {
  /* se il file non era apribile in lettura lo inizializza a zero */
  fpPATH=fopen("f22_files.dat","w");

  if(fpPATH == NULL)
    {
    XlWarning ("XlGrafico","open_path","Errore apertura file path");
    }

  for(i=0;i<NUM_PATH_FILES;i++)
    fwrite(path[i],LUN_PATH_FILES,1,fpPATH);
  }
fclose(fpPATH);

fpPATH=fopen("f22_files.dat","r+");
for(i=0;i<NUM_PATH_FILES;i++)
  fread(path[i],LUN_PATH_FILES,1,fpPATH);

fclose(fpPATH);
}
             
/*********************************************************
 * close_path()
 *    salva i valori attuali dei path names e chiude il file 
 *    F22_FILES.DAT
 *********************************************************/
close_path()
{
int i;

fpPATH=fopen("f22_files.dat","r+");
fseek(fpPATH,0,0);
for(i=0;i<NUM_PATH_FILES;i++)
  fwrite(path[i],LUN_PATH_FILES,1,fpPATH);
fclose(fpPATH);
}

/********************************************************
*********************************************************/
d2free(prow) 
char **prow;
{
XtFree(*prow);
XtFree(prow);
}
