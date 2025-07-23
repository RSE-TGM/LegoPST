/**********************************************************************
*
*       C Source:               f22io.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 30 12:23:48 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: f22io.c,2 %  (%full_filespec: 1,csrc,f22io.c,2 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)f22io.c	1.10\t3/28/95";
/*
        Fine sezione per SCCS
*/
/*
 * f22io.c
 *    contiene tutte le routine di lettura-scrittura su file f22
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#if defined AIX || defined ULTRIX
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include <Xm/Xm.h>
#include <X11/Xlib.h>
#include <Xm/CutPaste.h>
#include <Xm/List.h>
#if defined VMS
#include <types.h>
#include <stat.h>
#include <unixio.h>
#include <file.h>
#endif
#include <grsf22.h>
#include <math.h>
#include "menu_graf.h"
                 
/*
 variabili di uso comune alle routines di I/O
 */
extern float delta_nuovo,delta_vecchio,old_campione;
extern int campioni;
extern int numero_campioni;
extern float t_iniziale;
extern float t_finale;
extern S_DATI dato;
extern S_SEL_DATI bufdati[];

extern S_HEAD1 header1;
extern S_HEAD2 header2;
extern char nomi_var[NUM_VAR+1][LUN_NOME+1]; /* tabella dei nomi delle misure
                                          con terminatore 0 aggiunto */
                 
extern char **simboli;

extern int n_last;     /* posizione dell'ultimo campione nel buffer circolare */


int no_grab,inizio_dati,numero_var;
fpos_t posizione_iniziale;
float DeltaMinimo,DeltaMassimo;
int fine_dati; /* posizione ultimo dato */
#if defined (VMS_FORTRAN)
int fpDAT;
#else
FILE *fpDAT,*fpAPPO;
#endif
extern char *path_22dat;
char path_name[101];
/*
 open_22dat
   apre il file f22.dat
*/

static void rew_dati(void);
static void read_nomi(FILE*,int *);
static int read_multi(char*);

open_22dat()
{
int i;

i=strlen(path_22dat)-1;
strcpy(path_name,path_22dat);
#if defined VMS || VMS_FORTRAN
while(path_22dat[i]!=']' && path_22dat[i]!=':' ) i--;
#else
while(path_22dat[i]!='/' ) i--;
#endif
i++;
if(i>0) path_name[i]=0;
#if defined (VMS_FORTRAN)
fpDAT=open(path_22dat,O_RDWR,0,"ctx=rec","mrs=0","tmo=100",
            "rop=tmo","rfm=var","shr=upd");
if(fpDAT==-1)
        {
        return(1);
        }
#elif defined VMS 
i=open(path_22dat,O_RDONLY,0004|0002,
                "ctx=rec","rfm=var","shr=put");
printf("Apertura VMS file %s fp=%d\n",path_22dat,i);
fpDAT=fdopen(i,"r");
fseek(fpDAT,0,0);
if(fpDAT==NULL)
        {
        return(ERRORE);
        }
#endif
#if defined AIX || ULTRIX
fpDAT=fopen(path_22dat,"r");
if(fpDAT==NULL)
        {
        return(ERRORE);
        }
#endif
return(GIUSTO);
}

/*
 close_22dat
   chiude il file f22.dat
*/
close_22dat()
{
#if defined VMS_FORTRAN
close(fpDAT);
#else
fclose(fpDAT);
#endif
}


/*
 read_22dat1
    legge i dati relativi a tutte le misure;
	- flag : AGGIORNA    aggiorna l'area dati con gli ultimi dati acquisiti
	         TUTTI       trasferisce nell'area dati tutti i campioni 
			     presenti nel file.
*/

float tsec;   /* tempo in secondi da memorizzare nel buffer dati; 
				 viene incrementato di INC_SEC secondi per ogni
				 dato che viene inserito nel buffer */
int off_f22; 
int lun_rec_dati;  /* lunghezza record dati */
int lun_parziale;  /* lunghezza lettura parziale dati */
int num_complete;  /* numero di letture complete */

int read_22dat1(char flag)
//char flag;
{
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
if(flag==1)
	{
	tsec=0.0;
	n_last=0;
	ind=0;
	pieno=0;
	off_f22=0;
	if(read_nomi(fpDAT,&off_f22)==1) /* legge le tabelle dei nomi e simboli */
		{
		printf("\n fpDAT = %d",fpDAT);
		printf("\n errore in read nomi");
		return(ERRORE);
		}
/* lettura da file: si posiziona sul primo dato */
#if defined VMS_FORTRAN
	inizio_dati=lseek(fpDAT,0,1);
#else
	inizio_dati=off_f22;
#endif
/* lunghezza totale dati da leggere */
	lun_rec_dati=(header1.nvar+1)*sizeof(float);
#if defined VMS_FORTRAN
/* numero di letture complete da effettuare */
        num_complete=lun_rec_dati/2042;
        if(lun_rec_dati%2042) parziali=1;
        else parziali=0;
/* lunghezza reale dei dati (compresi i separatori) */
        lun_rec_dati+=(2*(num_complete+parziali));
        lun_parziale=(lun_rec_dati)%2044-2;
#endif
/*
  lettura del primo campione: ad esso dovra' corrispondere un tempo
  pari a 0.0 secondi.
*/	
	if((iret=read_multi(&buf.t))==0)
              {
              printf("\n dati non presenti in F22.DAT (read_multi)");
              return (ERRORE);
              }
	off_f22+=iret;
/*
  aggiorna se necessario i valori di minimo e di massimo
*/
	DeltaMassimo = 0;
	campioni = CAMP_UGUALI;
	t_iniziale = buf.t;
	old_campione = buf.t;
	if (read_multi(&buf.t)==lun_rec_dati)
	{
		bufdati[ind].t=buf.t;
		ind++;
	}
	delta_nuovo = buf.t - old_campione;
	DeltaMinimo = delta_nuovo;
	old_campione = buf.t;
	while(read_multi(&buf.t)==lun_rec_dati)
		{
		delta_vecchio = delta_nuovo;
		delta_nuovo = buf.t - old_campione;

/* Trasformazione dei delta letti per poter
	eseguirne un controllo congruente */

		delta_nuovo = delta_nuovo * 100;
		delta_vecchio = delta_vecchio * 100;
		delta_nuovo = rint (delta_nuovo);
		delta_vecchio = rint (delta_vecchio);
		delta_nuovo = delta_nuovo / 100;
		delta_vecchio = delta_vecchio / 100;

		if (delta_nuovo != delta_vecchio)
			{ campioni = CAMP_DISUGUALI; }
		if (delta_nuovo > DeltaMassimo) DeltaMassimo = delta_nuovo;
		if (delta_nuovo < DeltaMinimo) DeltaMinimo = delta_nuovo;
		old_campione = buf.t;
		bufdati[ind].t=buf.t;
		ind++;
		}
	}
numero_campioni = ind;
n_last=header1.nvar;
fine_dati=n_last;
return(GIUSTO);
}

read_multi(buf)
char *buf;
{
char sep[2];
char *pb;
int tot_letti=0; /* lunghezza totale bytes letti */
int lun,i;
int ret;
float tempo;

pb=buf;
#if defined VMS_FORTRAN
for(i=0;i<num_complete;i++)
        {
        lun=read(fpDAT,sep,2);
        if(lun!=2) return(0);
        tot_letti+=2;
        if((lun=read(fpDAT,pb,2042))!=2042)
                {
                printf("\n lun_2042 = %d",lun);
                return(GIUSTO);
                }
        pb+=2042;
        tot_letti+=lun;
        }
if(lun_parziale)
        {
        lun=read(fpDAT,sep,2);  /* legge il separatore */
        if(lun!=2) return(GIUSTO);
        tot_letti+=2;
        if((lun=read(fpDAT,pb,lun_parziale))!=lun_parziale)
                {
                printf("\n lun_parziale %d = %d",lun_parziale,lun);
                return(GIUSTO);
                }
        tot_letti+=lun;
        }
#else
tot_letti=fread(pb,1,lun_rec_dati,fpDAT);
if ((tot_letti!=lun_rec_dati)&&(tot_letti!=0))
	{
	while(tot_letti<lun_rec_dati)
		tot_letti+=fread(&pb[tot_letti],1,
			(lun_rec_dati-tot_letti),fpDAT);
	}
if(tot_letti==lun_rec_dati)
	{
	memcpy(&tempo,&pb[0],sizeof(float));
	}
#endif
return(tot_letti);
}

void rew_dati(void)
{
int ret;

n_last=0;
#if defined VMS_FORTRAN
lseek(fpDAT,inizio_dati,0);
#else
/*
fseek(fpDAT,inizio_dati,0);
*/
ret=fseek(fpDAT,0,0);
ret=fseek(fpDAT,inizio_dati,0);
fsetpos(fpDAT,&posizione_iniziale);
if(ret!=0)
	{
	printf("errore rew_dati\n");
	}
#endif
}


#if defined VMS_FORTRAN
read_nomi(fp,offset)
int *offset;
int fp;
{
int row,col,dimens;
char *pdata;
int max_dim;  /* massima dimensione del blocco fortran */
int sizebuf,sizeprec;  /* dimensione del blocco allocato */
short sep[2]; /* separatore di blocchi (scrittura fortran) */
char *pbuf;   /* puntatore buffer di appoggio  */
char *papp;
int pos,newpos;
int lun;
int ultimo;
register int i,k;
/*
  lettura del primo header
*/
max_dim=2042;
sizebuf=2044;
pbuf=XtCalloc(sizebuf,sizeof(char));
if(pbuf==NULL) printf("\n manca memoria per calloc");

pos=(*offset);
if((lun=read(fp,pbuf,sizeof(S_HEAD1)-6))==0)
        {
        printf("\n dati non presenti in F22.DAT");
        return (ERRORE);
        }
if(lun==-1)
        {
        printf("\n errore in lettura");
        return (ERRORE);
        }
pos+=lun;
memcpy(&header1,pbuf,sizeof(S_HEAD1)-6);
ultimo=0;
pos=lseek(fp,0,1);
newpos=lseek(fp,pos,0);
if(newpos==-1) {perror("\ngraf:");XtFree(pbuf); return(1);}
lun=read(fp,pbuf,sizebuf); /* legge il primo blocco */
if(lun==-1)
        {
        printf("\n errore in lettura");
        XtFree(pbuf);
        perror("\n graf:");
        return(ERRORE);
        }
if(lun<max_dim) ultimo=1;
pos+=lun;
memcpy(&header1.void2[0],pbuf,6);  /* copia spare e numero variabili */
sizeprec=sizebuf;
sizebuf=max_dim;
/*
   legge tutta la parte variabili del file
*/
while(ultimo==0)
        {
        if(read(fp,sep,2)!=2) break;  /* legge il separatore */
        pos+=2;
        sizebuf+=max_dim;
        pbuf=XtRealloc(pbuf,sizebuf);
        if(pbuf==NULL) printf("\n manca memoria per realloc");
        if((lun=read(fp,&pbuf[sizeprec],max_dim))!=max_dim)
                {pos+=lun; ultimo=1; break;}
        (*offset)+=(max_dim);
        sizeprec+=max_dim;
        pos+=lun;
        }
papp=pbuf+6;
for(i=0;i<header1.nvar;i++)
        {
        memcpy(nomi_var[i],papp,LUN_NOME);
        papp+=LUN_NOME;
        nomi_var[i][LUN_NOME]=0;
        }
nomi_var[i][0]=0;  /* tappo */
memcpy(&header2,papp,sizeof(S_HEAD2));
papp+=sizeof(S_HEAD2);
/*
        alloca l'array bidimensionale che conterra' la lista delle
        descrizioni.
*/
row=header2.ncasi+1;
col=LUN_SIMB+1;
pdata = (char *) XtCalloc(row * col,  sizeof(char));
if(pdata==(char *)NULL) {
                printf("\n mancanza di spazio per allocazione");
                return (ERRORE);
                }
dimens=row * (sizeof(char *));
simboli = (char **) XtCalloc(row,sizeof(char *));
if(simboli==(char **)NULL) {
                printf("\n mancanza di spazio per allocazione");
                return (ERRORE);
                }
for(i=0;i<row;i++)
                {
                simboli[i]=pdata;
                pdata+=col;
                }
for(i=0;i<header2.ncasi;i++)
        {
        memcpy(simboli[i],papp,LUN_SIMB);
        simboli[i][LUN_SIMB]=(char)0;
        papp+=LUN_SIMB;
        (*offset)+=LUN_SIMB;
        }
simboli[i][0]=(char)0;
XtFree(pbuf);
(*offset)=pos;
return(GIUSTO);
}
#else
read_nomi(fp,offset)
int *offset;
FILE *fp;
{
extern int non_f22;
int row,col,dimens;
char *pdata;
int i,k;
/*
  lettura del primo header
*/
if((fread(&header1,sizeof(S_HEAD1),1,fp))==NULL)
        {
        printf("\n dati non presenti in F22.DAT (lettura S_HEAD1)");
        return (ERRORE);
        }

/* Controllo del caso in cui venga aperto un file 
    che non sia un file f22.dat   */

non_f22 = 0;
if ((header1.nvar < 0) || (header1.nvar > 6000))
	{ 
	non_f22 = 1;
	return (ERRORE); 
	}

(*offset)+=sizeof(S_HEAD1);

for(i=0;i<header1.nvar;i++)
        {
        fread(nomi_var[i],LUN_NOME,1,fp);
        for(k=0;k<LUN_NOME;k++)
                if(nomi_var[i][k]==' ') nomi_var[i][k]=0;
        nomi_var[i][LUN_NOME]=0;
        (*offset)+=LUN_NOME;
        }
nomi_var[i][0]=0;  /* tappo */

if((fread(&header2,sizeof(S_HEAD2),1,fp))==NULL)
        {
        printf("\n dati non presenti in F22.DAT (lettura S_HEAD2)");
        return(ERRORE);
        }
(*offset)+=sizeof(S_HEAD2);
/*
        alloca l'array bidimensionale che conterra' la lista delle
        descrizioni.
*/
row=header2.ncasi+1;
col=LUN_SIMB+1;
pdata = (char *) XtCalloc(row * col,  sizeof(char));
if(pdata==(char *)NULL) {
                printf("\n mancanza di spazio per allocazione");
                return (ERRORE);
                }
dimens=row * (sizeof(char *));
simboli = (char **) XtCalloc(row,sizeof(char *));
if(simboli==(char **)NULL) {
                printf("\n mancanza di spazio per allocazione");
                return (ERRORE);
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
return(GIUSTO);
}
#endif


void d2free(prow) 
char **prow;
{
   if( (*prow) != NULL )
      XtFree( (*prow) );
   if( prow != NULL)
      XtFree( prow );
}

chiudi_prog_legocad ()
{
	return (GIUSTO);
}

/***********************
    Routine che esegue il controllo sui parametri sele e camp
    per accettare le condizioni necessarie alla scrittura della
    Clipboard
***********************/

int ControllaValiditaPerClip (sele,camp)
int sele,camp;
{
	extern Widget Message;

	char stringa[100];
	XmString Messaggio;

	if (sele == 0)
		{
		strcpy (stringa,"Must be selected at least one item\0");
		Messaggio = XmStringCreate (stringa,XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_INFORMATION);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message,no_grab);
		XmStringFree (Messaggio);
		return (ERRORE);
		}
	else
		{
		if (camp == CAMP_DISUGUALI)
			{
			strcpy (stringa,"Delta must be the same\0");
                	Messaggio = XmStringCreate (stringa,
					XmSTRING_DEFAULT_CHARSET);
                	set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_INFORMATION);
                	set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
                	UxPopupInterface (Message,no_grab);
			XmStringFree (Messaggio);
			return (ERRORE);
			}
		}
	return (GIUSTO);
}

/***********************
	Funzione che cancella i file utilizzati per la applicazione
***********************/

CancellazioneF22()
{
extern char f22source[],f22target[],f22undo[];
extern FILE *fpELENCO;

char cdo[100];
	
if (Controllo_gia_presente (f22source))
	{
#ifdef OLDVERS
	strcpy (cdo,"rm ");
	strcat (cdo,f22source);
	system (cdo);
#endif
        fprintf(fpELENCO,"rm  %s\n",f22source);
	}
if (Controllo_gia_presente (f22target))
	{
#ifdef OLDVERS
	strcpy (cdo,"rm ");
	strcat (cdo,f22target);
	system (cdo);
#endif
        fprintf(fpELENCO,"rm  %s\n",f22target);
	}
if (Controllo_gia_presente (f22undo))
	{
#ifdef OLDVERS
	strcpy (cdo,"rm ");
	strcat (cdo,f22undo);
	system (cdo);
#endif
        fprintf(fpELENCO,"rm  %s\n",f22undo);
	}
return (GIUSTO);
}

/***********************
	Controlla che il nomefile passato come parametro 
	sia gia' presente oppure si stia tentando di 
	aprire un file nuovo
***********************/

Controllo_gia_presente (nomefile)
char *nomefile;
{
FILE *fp;
	
fp = fopen (nomefile,"r");
if (fp == NULL)
	{
	fclose (fp);
	return (GIUSTO);
	}
if (fp != NULL) 
	{
	fclose (fp);
	return (ERRORE);
	}
}

Scrivi_header1 ()
{
int i;

fwrite (&header1,sizeof(S_HEAD1),1,fpAPPO);
for (i=0;i<header1.nvar;i++)
	fwrite (nomi_var[i],LUN_NOME,1,fpAPPO);
}

Scrivi_header2 ()
{
int i;

fwrite (&header2,sizeof(S_HEAD2),1,fpAPPO);
for (i=0;i<header2.ncasi;i++)
	fwrite (simboli[i],LUN_SIMB,1,fpAPPO);
}

/***********************
	Funzione che esegue l'elevamento a potenza:
	-- base: deve essere un float positivo o negativo
	-- esponente: deve essere un intero positivo,negativo o = a 0
	Ritorna il valore dell'elevamento a potenza.
***********************/

float toExp (float base,short esponente)
{
int i;
float valore;

if (esponente == 0) return (1);
if (esponente > 0)
	{
	for (i=1;i<esponente+1;i++)
		valore = valore * base;
	}
if (esponente < 0)
	{
	for (i=(-1);i>esponente-1;i--)
		valore = valore * base;
	valore = 1/valore;
	}
return (valore);
}

/***********************
	Funzione che azzera il testo dell'oggetto passato
	come parametro;l'oggetto deve essere di tipo
	textField          
***********************/

Azzera_text (Oggetto)
Widget Oggetto;
{
	char stringa[5];

	strcpy (stringa,"\0");
	set_something_val (Oggetto, XmNvalue, (XtArgVal) stringa);
	return (GIUSTO);
}

/***********************
	Aggiorna la stringa che visualizza il tempo
	iniziale della simulazione
***********************/

Aggiorna_start_time ()
{
	extern Widget start_time;
	extern XmString label_start_time;
	extern float t_iniziale;

	char st_iniziale[10];

	sprintf (st_iniziale,"%.2f",t_iniziale);
	strcat (st_iniziale,"\0");
	label_start_time = XmStringCreate(st_iniziale,XmSTRING_DEFAULT_CHARSET);
	set_something_val (start_time, XmNlabelString, (XtArgVal) label_start_time);
	return (GIUSTO);
}

/***********************
	Aggiorna la stringa che visualizza il tempo
	finale della simulazione
***********************/

Aggiorna_stop_time ()
{
	extern Widget stop_time;
	extern XmString label_stop_time;
	extern float t_finale;

	char st_finale[10];

	sprintf (st_finale,"%.2f",t_finale);
	strcat (st_finale,"\0");
	label_stop_time = XmStringCreate (st_finale,XmSTRING_DEFAULT_CHARSET);
	set_something_val (stop_time, XmNlabelString, (XtArgVal) label_stop_time);
	return (GIUSTO);
}

/***********************
	Aggiorna la stringa che visualizza il numero
	dei campioni presenti sul file f22 della simulazione
***********************/

Aggiorna_numero_camp ()
{
	extern Widget num_campioni;
	extern XmString label_numero_campioni;
	extern int numero_campioni;

	char snumero[10];
	
	sprintf (snumero,"%d",numero_campioni);
	strcat (snumero,"\0");
	label_numero_campioni = XmStringCreate (snumero,
				XmSTRING_DEFAULT_CHARSET);
	set_something_val (num_campioni, XmNlabelString, (XtArgVal) label_numero_campioni);
	return (GIUSTO);
}

/***********************
	Aggiorna la stringa che visualizza il delta
	dei campioni presenti sul file f22 della simulazione
***********************/

Aggiorna_delta_camp ()
{
	extern Widget delta_campioni;
	extern XmString label_delta_campioni;
	extern int campioni;

	char sdelta[12];

	if (campioni == CAMP_DISUGUALI)
	{
	strcpy (sdelta,"Not equal\0");
	label_delta_campioni = XmStringCreate (sdelta,XmSTRING_DEFAULT_CHARSET);
	}else
	{
	sprintf (sdelta,"%.2f",delta_nuovo);
	strcat (sdelta,"\0");
	label_delta_campioni = XmStringCreate (sdelta,XmSTRING_DEFAULT_CHARSET);
	}
	set_something_val (delta_campioni, XmNlabelString, (XtArgVal) label_delta_campioni);
	return (GIUSTO);
}

/***********************
	Aggiorna la stringa che visualizza l'intervallo
	di campionamento massimo dei dati presenti su file
	f22 della simulazione  
***********************/

Aggiorna_MAXInt ()
{
	extern Widget MAX_Int;
	XmString label_MAXInt;

	char sMAXInt[10];
	
	sprintf (sMAXInt,"%.2f",DeltaMassimo);
	label_MAXInt = XmStringCreate (sMAXInt,XmSTRING_DEFAULT_CHARSET);
	set_something_val (MAX_Int, XmNlabelString, (XtArgVal) label_MAXInt);
	return (GIUSTO);
}

/***********************
	Aggiorna la stringa che visualizza l'intervallo
	di campionamento minimo dei dati presenti su file
	f22 della simulazione  
***********************/

Aggiorna_MINInt ()
{
	extern Widget MIN_Int;
	XmString label_MINInt;

	char sMINInt[10];
	
	sprintf (sMINInt,"%.2f",DeltaMinimo);
	label_MINInt = XmStringCreate (sMINInt,XmSTRING_DEFAULT_CHARSET);
	set_something_val (MIN_Int, XmNlabelString, (XtArgVal) label_MINInt);
	return (GIUSTO);
}

/***********************
	Aggiorna la stringa che visualizza il numero delle 
	variabili presenti sul file f22 della simulazione
***********************/

Aggiorna_num_var ()
{
	extern Widget num_var;
	extern XmString label_num_var;

	char snum_var[10];

	numero_var = header1.nvar;
	sprintf (snum_var,"%d",header1.nvar);
	strcat (snum_var,"\0");
	label_num_var = XmStringCreate (snum_var, XmSTRING_DEFAULT_CHARSET);
	set_something_val (num_var, XmNlabelString, (XtArgVal) label_num_var);
	return (GIUSTO);
}

/***********************
	Funzione che legge tutto il file f22 e che 
	visualizza tutti i dati necessari per la main window
***********************/

Carica_f22()
{
/*  Widget esterni alla main window  */
	extern Widget Message;  /*  Dialog box generica */
	extern Widget fileSelectionBox1; /* Finestra per la selezione file */ 
/*  Widget inerenti al find  */
	extern Widget textField_find;
	extern Widget Push_Select;
	extern Widget Push_Deselect;
	extern Widget Find_Pane;
/*  Voci del menu principale  */
	extern Widget SaveItem;
	extern Widget Save_asItem;
	extern Widget AsciiItem;
	extern Widget Var_Pane;
	extern Widget Time_Pane;
	extern Widget Calc_Pane;
	extern Widget Single_Find_Item;
	extern Widget Multiple_Find_Item;
/*  Oggetti della main window   */
	extern Widget lista_var;
	extern Widget path_f22;
	extern XmString label_path_f22;
	extern XmString *x_simboli;
	extern int Find_sel; /* Variabile per il tipo di find */
	extern int non_f22;  /* Variabile per controllo apertura f22 */

	char stringa[100];
	int flag,offset,i;
	XmString Messaggio;

/*	Controllo del caso in cui sia gia' caricato un altro
	file f22;se vero vengono liberate tutte le liste allocate
	in memoria   */
	
	if ((x_simboli != NULL) && (non_f22 == 0))
	{
		close_22dat ();
		d2free (x_simboli);
		d2free (simboli);
		XmListDeleteAllItems (lista_var);
	}

/*	Apertura e lettura del file f22  */

	open_22dat ();
	read_22dat1 (1);

/*	Abilitazione delle voci del menu principale
	in seguito alla apertura ed alla lettura di
	un file f22.dat            */

if (non_f22)
	{
	strcpy (stringa,"File is not an f22 file!\0");
	Messaggio = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
	set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
	set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
	UxPopupInterface (Message,no_grab);
	return (ERRORE);
	XmStringFree (Messaggio);
	}
else
	{
	set_something_val (textField_find, XmNsensitive, (XtArgVal) True);
	set_something_val (SaveItem, XmNsensitive, (XtArgVal) True);
	set_something_val (Save_asItem, XmNsensitive, (XtArgVal) True);
	set_something_val (AsciiItem, XmNsensitive, (XtArgVal) True);
	set_something_val (Var_Pane, XmNsensitive, (XtArgVal) True);
	set_something_val (Time_Pane, XmNsensitive, (XtArgVal) True);
	set_something_val (Find_Pane, XmNsensitive, (XtArgVal) True);
	set_something_val (Calc_Pane, XmNsensitive, (XtArgVal) True);
	set_something_val (Push_Select, XmNsensitive, (XtArgVal) True);
	set_something_val (Push_Deselect, XmNsensitive, (XtArgVal) True);
	if (Find_sel == MULTIPLO)
		{
		set_something_val (Multiple_Find_Item, XmNset, (XtArgVal) True);
		set_something_val (Single_Find_Item, XmNset, (XtArgVal) False);
		}
	else
		{
		set_something_val (Single_Find_Item, XmNset, (XtArgVal) True);
		set_something_val (Multiple_Find_Item, XmNset, (XtArgVal) False);
		}

/*	Passaggio della lista delle variabili
	lette dal file f22.dat alla lista della 
	ScrolledWindow  */

        x_simboli=(XmString *)XtCalloc((header2.ncasi+1),
                          sizeof(XmString));
        for(i=0;i<header2.ncasi;i++)
                {
                x_simboli[i]=XmStringCreateLtoR(simboli[i],
                        XmSTRING_DEFAULT_CHARSET);
                XmListAddItem (lista_var,x_simboli[i],i+1);
		}
        x_simboli[i]=NULL;

/*	Scrittura del tempo iniziale nella
	label "start_time                  */

	Aggiorna_start_time ();

/*	Scrittura del tempo finale nella
	label "stop_time"                   */

	t_finale = bufdati[numero_campioni-1].t;
	Aggiorna_stop_time ();

/*	Scrittura del numero delle variabili lette
	nella label "num_var"               */

	Aggiorna_num_var ();

/*	Scrittura dell'intervallo dei campionamenti 
	nella label "delta_campionamenti"    */

	Aggiorna_delta_camp ();
	Aggiorna_numero_camp ();
	Aggiorna_MINInt ();
	Aggiorna_MAXInt ();

/*	Scrittura del path del file selezionato
	nella label "path_f22"             */

	get_something (fileSelectionBox1, XmNtextString, (void*) &label_path_f22);
	set_something_val (path_f22, XmNlabelString, (XtArgVal) label_path_f22);
        return(GIUSTO);
	}
}

/***********************
	Funzione che cerca a partire da un item selezionato
	verso l'alto, all'interno dello Widget XmList una 
	stringa letta dallo Widget textField_find
***********************/

int Cerca_su (char *Trov)
{
	extern Widget lista_var;

	extern int Find_sel;
	extern int trovata;

	int selezione,*lista_sele,num_sele,i,j,gia_sele;
	char *appoggio1,*appoggio2,*punt;

/*	Carica in una lista di appoggio gli item selezionati  */

	selezione = XmListGetSelectedPos (lista_var, &lista_sele, &num_sele);
	appoggio1 = XtNewString (Trov);
	tomaius (appoggio1); 
	trovata = 0;

/*	Controllo del tipo di find selezionato   */

	if (Find_sel == SINGOLO) XmListDeselectAllItems (lista_var);

/*      Imposta gli indici da cui partire con la ricerca  */

	if (selezione)
		{
		if (lista_sele[0] != 1)
			i = (lista_sele[num_sele-1])-2;
		else i = -1;
		}
	else i = header1.nvar - 1;

/*	Ciclo di ricerca   */

	while ((i>=0)&&(trovata==0))
		{
		appoggio2 = XtNewString (simboli[i]);
		tomaius (appoggio2);
		punt = strstr (appoggio2,appoggio1);
		gia_sele = 0;
		if (selezione)
			{
			for (j=0;j<num_sele;j++)
				if (lista_sele[j] == (i+1)) gia_sele = 1;
			}
		if ((punt != NULL) && (gia_sele == 0)) trovata = 1;
		i--;
		XtFree (appoggio2);
		}
	XtFree (appoggio1);
	return (i+2);
}

/***********************
	Funzione che cerca a partire da un item selezionato
	verso il basso, all'interno dello Widget XmList una 
	stringa letta dallo Widget textField_find
***********************/

int Cerca_giu (char *Trov)
{
        extern Widget lista_var;

	extern int Find_sel;
        extern int trovata;

        int selezione,*lista_sele,num_sele,i,j,gia_sele;
	char *appoggio1,*appoggio2,*punt;

/*	Carica in una lista di appoggio gli item selezionati   */

        selezione = XmListGetSelectedPos (lista_var, &lista_sele, &num_sele);
        appoggio1 = XtNewString (Trov);
        tomaius (appoggio1);
	trovata = 0;

/*	Controllo del tipo di find selezionato   */

	if (Find_sel == SINGOLO) XmListDeselectAllItems (lista_var);

/*      Imposta gli indici da cui partire con la ricerca  */

        if (selezione)
        	{
                if (lista_sele[num_sele] != header1.nvar)
       			i = lista_sele[0];
		else i = header1.nvar;
		}
	else i = 0;

/*	Ciclo di ricerca   */

        while ((i<header1.nvar)&&(trovata==0))
        	{
                appoggio2 = XtNewString (simboli[i]);
                tomaius (appoggio2);
                punt = strstr (appoggio2,appoggio1);
		gia_sele = 0;
		if (selezione)
			{
			for (j=0;j<num_sele;j++)
				if (lista_sele[j] == (i+1)) gia_sele = 1;
			}
                if ((punt != NULL) && (gia_sele == 0)) trovata = 1;
                i++;
		XtFree (appoggio2);
        	}
	XtFree (appoggio1);
        return (i);
}

/***********************
	Funzione che scarica il file appena modificato, "target",
	in quello di lavoro "source"
***********************/

Copia_target ()
{
	extern char f22target[];

	char cdo[100];

	strcpy (cdo,"cp ");
	strcat (cdo,f22target);
	strcat (cdo," ");
	strcat (cdo,path_22dat);
	system (cdo);
}

/***********************
	Funzione che chiude il programma grafics solo
	nel caso ce ne sia uno gia aperto
***********************/

Uccidi_grafics ()
{
extern int pid;

char cdo[100];
char strappo[20];

if (pid != 0)
	{
	sprintf (strappo,"%d",pid);
	strcpy (cdo,"kill -9 ");
	strcat (cdo,strappo);
	system (cdo);
	pid = 0;
	}
}

/***********************
	Funzione che copia il file di lavoro,"source", sul
	file di salvataggio della operazione precedente,"undo",
	Apre quindi in scrittura il file da modificare "target"
***********************/

Apri_f22_temp ()
{
extern char Path_appo[];
extern char f22source[],f22undo[],f22target[];
extern Widget UndoItem;

char cdo[200];

printf ("Sto lavorando -- ATTENDERE __\n");
Uccidi_grafics ();
set_something_val (UndoItem, XmNsensitive, (XtArgVal) True);
strcpy (Path_appo,"cp ");
strcat (Path_appo,f22source);
strcat (Path_appo," ");
strcat (Path_appo,f22undo);
system (Path_appo);
strcpy (Path_appo,f22target);
fpAPPO=fopen(Path_appo,"w");
}

/***********************
	Funzione che riceve in ingresso una stringa e controlla
	se e' valida come stringa del tempo di inizio simulazione
***********************/

Controllo_start (lettura)
char *lettura;
{
int i;

for (i=0;i<strlen (lettura);i++)
	{
	if ((isdigit(lettura[i]) == 0) && (lettura[i] != '.') &&
		((isdigit(lettura[0]) != 0) || (lettura[0] != '-')))
		{
		return (ERRORE);
		}
	}
return (GIUSTO);
}

/***********************
	Funzione che riceve in ingresso una stringa e controlla
	se e' valida come stringa del tempo di fine simulazione
***********************/

Controllo_stop (lettura)
char *lettura;
{
int i;

for (i=0;i<strlen (lettura);i++)
	{
	if ((isdigit(lettura[i]) == 0) && (lettura[i] != '.'))
		{
		return (ERRORE);
		}
	}
return (GIUSTO);
}

/***********************
   Funzione che restituisce il valore interpolato 
	tra 'Val1' e 'Val2' al tempo 'CurrentX' che 
	deve trovarsi tra 'T1' e 'T2'
***********************/

float Calcola_XY (Val1,Val2,T1,T2,CurrentX)
float Val1,Val2,T1,T2,CurrentX;
{
float dy,dx,ysegnato,X,Y;

dy = Val2 - Val1;
dx = T2 - T1;
X = CurrentX - T1;
ysegnato = (dy / dx) * X;
Y = Val1 + ysegnato;
return (Y);
}

/***********************
	Funzione che setta le variabili per il salvataggio e per
	l'uscita dalla applicazione e ricarica il nuovo file f22
	modificato
***********************/

Uscita ()
{
extern int Modifica;
extern int Salvato;

Modifica = MODIFICATO;
Salvato = NON_SALVATO;
fclose (fpAPPO);
Copia_target ();
Carica_f22 ();
printf ("Fine operazione \n");
return (GIUSTO);
}

/***********************
	Funzione che rialloca la struttura simboli con il numero di
		elementi passato come parametro
***********************/
char **AllocaStruttura (Righe,Colonne)
int Righe,Colonne;
{
	char *p_varappo,**lista_appoggio;
	int i;

	p_varappo=(char *) XtCalloc((Righe+1) * (Colonne+1),  sizeof(char));
	if(p_varappo==(char *)NULL) 
	{
                printf("\n mancanza di spazio per allocazione memoria");
                return ((char **)NULL);
        }
	lista_appoggio = (char **)XtCalloc((Righe+1),sizeof(char *));
	if(lista_appoggio==(char **)NULL) 
	{
                printf("\n mancanza di spazio per allocazione memoria");
                return ((char **)NULL);
        }
	for(i=0;i<Righe;i++)
        {
                lista_appoggio[i]=p_varappo;
                p_varappo+=Colonne+1;
        }
	lista_appoggio[i] = NULL;
	XtFree (p_varappo);
	return (lista_appoggio);
}

/***********************
	Funzione per la manipolazione del file dei grafici:
	estrae dai tempi della simulazione un intervallo prefissato
***********************/

F22_extract ()
{
	extern Widget Edit_Time_Win;
	extern Widget textField_Start;
	extern Widget textField_Stop;
	extern Widget Message;
	extern float new_iniziale;
	extern float new_finale;
	extern float t_iniziale;
	extern float t_finale;

	int i,j,errore,appo1;
	char *appostart,*appostop,stringa[200];
	float valore,tempo,val1[NUM_VAR+1],val2[NUM_VAR+1],vals[NUM_VAR+1];
	XmString Messaggio;

/*	Lettura del tempo di inizio e del tempo di fine
	dell'intervallo da estrarre           */

	errore = 0;
	appostart = XmTextFieldGetString (textField_Start);
	appostop = XmTextFieldGetString (textField_Stop);
	errore = Controllo_start (appostart);
	errore = Controllo_stop (appostop);
	if (errore == 1)
		{
		strcpy (stringa,"Input time not valid !\0");
		Messaggio = XmStringCreate (stringa,XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message,no_grab);
		XmStringFree (Messaggio);
		return (ERRORE);
		}
	else
		{
		new_iniziale = atof (appostart);
		new_finale = atof (appostop);
/*	Controllo che i tempi inseriti siano congruenti   */
		if ((new_iniziale < t_iniziale)||(new_finale > t_finale)||
			(new_iniziale >= new_finale))
		{
			strcpy (stringa,"Value time not valid!\0");
			Messaggio = XmStringCreate (stringa,
					XmSTRING_DEFAULT_CHARSET);
			set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
			set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
			UxPopupInterface (Message,no_grab);
			XmStringFree (Messaggio);
			return (ERRORE);
		}else
		{
			UxPopdownInterface (Edit_Time_Win);
			Apri_f22_temp ();
			rew_dati ();
/*	Fase di riscrittura del nuovo file f22   */
			Scrivi_header1();
			Scrivi_header2();
			for (j=0;j<header1.nvar+1;j++)
				fread (&val1[j],1,sizeof(float),fpDAT);
			for (i=0;i<numero_campioni+1;i++)
			{
				for (j=0;j<header1.nvar+1;j++)
				{
					fread (&val2[j],1,sizeof(float),fpDAT);
					vals [j] = val1[j];
				}
				if ((val1[0]<new_iniziale)&&
					(val2[0]>new_iniziale))
				   vals[0] = new_iniziale;
				if ((val1[0]<new_finale)&&
					(val2[0]>new_finale))
				   vals[0] = new_finale;
				if ((vals[0] == new_iniziale) ||
				    (vals[0] == new_finale))
				{
				   for (j=1;j<header1.nvar+1;j++)
				     {
					vals[j] = Calcola_XY (val1[j],
					  val2[j],val1[0],val2[0],
					  vals[0]);
				     }
				}
				if ((vals[0] >= new_iniziale) && 
					(vals[0] <= new_finale))
				{
					for (j=0;j<header1.nvar+1;j++)
					fwrite(&vals[j],sizeof(float),1,fpAPPO);
				}
				for (j=0;j<header1.nvar+1;j++)
					val1[j] = val2[j];
			}
			t_iniziale = new_iniziale;
			t_finale = new_finale;
			Uscita ();
		}
		}
	XtFree (appostart);
	XtFree (appostop);
	return (GIUSTO);
}

/***********************
   Funzione che esegue lo spostamento del tempo iniziale 
	dei dati sul file f22 a destra o sinistra
***********************/

F22_shift ()
{
	extern Widget Edit_Time_Win;
	extern Widget textField_Start;
	extern Widget Message;
	extern float new_iniziale;
	extern float t_iniziale;

	int i,j,errore,appo1;
	char *appostart,stringa[200];
	float valore,tempo;
	XmString Messaggio;

	errore = 0;
	appostart = XmTextFieldGetString (textField_Start);
	errore = Controllo_start (appostart);
	if (errore == 1)
		{
		strcpy (stringa,"Input time not valid !\0");
		Messaggio = XmStringCreate (stringa,XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message,no_grab);
		XmStringFree (Messaggio);
		return (ERRORE);
		}
	else
		{
		new_iniziale = atof (appostart);
		UxPopdownInterface (Edit_Time_Win);
		Apri_f22_temp ();
		rew_dati ();
		Scrivi_header1();
		Scrivi_header2();
		for (i=0;i<numero_campioni+1;i++)
			{
			fread (&tempo,1,sizeof(float),fpDAT);
			tempo += new_iniziale;
			fwrite (&tempo,sizeof(float),1,fpAPPO);
			for (j=0;j<header1.nvar;j++)
				{
				fread (&valore,1,sizeof(float),fpDAT);
				fwrite (&valore,sizeof(float),1,fpAPPO);
				}
			}
		t_iniziale += new_iniziale;
		Uscita ();
		}
	XtFree (appostart);
	return (GIUSTO);
}

/***********************
   Funzione che esegue lo sfoltimento dei campioni presenti sul 
	file correntemente aperto
***********************/

F22_sampling ()
{
	extern int New_Sampl;

	int i,j,appo1,cont;
	float valore,tempo;

	Apri_f22_temp ();
	rew_dati ();
	Scrivi_header1();
	Scrivi_header2();
	cont = New_Sampl;
	for (i=0;i<numero_campioni+1;i++)
		{
		fread (&tempo,1,sizeof(float),fpDAT);
		if (cont==New_Sampl)
			fwrite (&tempo,sizeof(float),1,fpAPPO);
		for (j=0;j<header1.nvar;j++)
			{
			fread (&valore,1,sizeof(float),fpDAT);
			if (cont==New_Sampl)
				fwrite (&valore,sizeof(float),1,fpAPPO);
			}
		if (cont == New_Sampl) cont = -1;
		cont++;
		}
	Uscita ();
	return (GIUSTO);
}

/***********************
   Funzione che esegue il ricampionamento del file f22 
	corrente
***********************/

F22_resampling ()
{
	extern float New_delta;

	int i,j,ricampionato;
	float val1[NUM_VAR+1],val2[NUM_VAR+1],vals[NUM_VAR+1];
	float tims;

	Apri_f22_temp ();
	rew_dati ();
	Scrivi_header1();
	Scrivi_header2();
	for (j=0;j<header1.nvar+1;j++)
		{
		fread (&val1[j],1,sizeof(float),fpDAT);
                fwrite (&val1[j],sizeof(float),1,fpAPPO);
 		}
	vals[0] = val1[0] + New_delta;
	for (i=0;i<numero_campioni+1;i++)
	{
		for (j=0;j<header1.nvar+1;j++)
			fread (&val2[j],1,sizeof(float),fpDAT);
		while (val2[0]>=vals[0])
		{
			if (val2[0]>vals[0])
			{
			fwrite (&vals[0],sizeof(float),1,fpAPPO);
			for (j=1;j<header1.nvar+1;j++)
			{
				vals [j] = Calcola_XY (val1[j],val2[j],val1[0],
						val2[0],vals[0]);
				fwrite (&vals[j],sizeof(float),1,fpAPPO);
				val1[j] = vals[j];
			}
			val1[0] = vals[0];
			vals[0] += New_delta;
			}else
			{
			if (val2[0]==vals[0])
			{
				fwrite (&val2[0],sizeof(float),1,fpAPPO);
				for (j=1;j<header1.nvar+1;j++)
				fwrite (&val2[j],sizeof(float),1,fpAPPO);
				vals[0] += New_delta;
			}
			}
		}
		for (j=0;j<header1.nvar+1;j++)
			val1[j] = val2[j];
	}
	Uscita ();
	return (GIUSTO);
}

/***********************
   Funzione che aggiorna il nuovo titolo della simulazione
***********************/
CambiaTitolo ()
{
	int i,j;
	float tempo,valore;

	Apri_f22_temp ();
	rew_dati ();
	Scrivi_header1();
	Scrivi_header2();
	for (i=0;i<numero_campioni+1;i++)
	{
		fread (&tempo,1,sizeof(float),fpDAT);
		fwrite (&tempo,sizeof(float),1,fpAPPO);
		for (j=0;j<header1.nvar;j++)
			{
			fread (&valore,1,sizeof(float),fpDAT);
			fwrite (&valore,sizeof(float),1,fpAPPO);
			}
	}
	Uscita ();
	return (GIUSTO);
}

/***********************
   Funzione che restituisce un puntatore ad una stringa contenente
	tutte le informazioni necessarie per essere scritta nella
	Clipboard
***********************/

char *Prepara_stringa (lista,numero,nomefile)
int *lista,numero;
char *nomefile;
{
extern char *path_22dat;
extern float t_iniziale;
extern float t_finale;
extern float delta_nuovo;

int posiz,i,lunghezza;
char str_iniz[15],str_fini[15],str_delt[15],str_path[101],str_nume[15];
char *risul;

strcpy (str_path,nomefile);
sprintf (str_iniz,"%.2f",t_iniziale);
sprintf (str_fini,"%.2f",t_finale);
sprintf (str_delt,"%.2f",delta_nuovo);
sprintf (str_nume,"%d",numero);
strcat (str_path,"|||\0");
strcat (str_iniz,"|||\0");
strcat (str_fini,"|||\0");
strcat (str_delt,"|||\0");
strcat (str_nume,"|||\0");
lunghezza = strlen (str_path);
lunghezza += strlen (str_iniz);
lunghezza += strlen (str_fini);
lunghezza += strlen (str_delt);
lunghezza += strlen (str_nume);
risul = XtCalloc ((lunghezza+(12*numero)+1),sizeof(char));
strcpy (risul,str_path);
strcat (risul,str_iniz);
strcat (risul,str_fini);
strcat (risul,str_delt);
strcat (risul,str_nume);
for (i=0;i<numero;i++)
	{
	posiz = lista[i] - 1;
	strcat (risul,nomi_var[posiz]);
	strcat (risul,"|||\0");
	}
return (risul);
}

/***********************
	Funzione che scompone la stringa letta dalla clipboard
	e memorizzata nel parametro data nelle varie 
	sottostringhe per poter essere utilizzate dal richiedente
	della clipboard
***********************/

Scomponi_stringa (data)
char *data;
{
	extern char *path_22datClip;
	extern float t_inizialeClip;
	extern float t_finaleClip;
	extern float delta_nuovoClip;
	extern int numeroClip;
	extern char **lista_varClip;

	int num_colonne;
	char *appoggio;
	int i,lunghezza;
	char *str_iniz,*str_fini,*str_delt,*str_nume;

/*	libera la lista delle variabili e il path letti da
	un precedente accesso alla clipboard    */

if (lista_varClip != NULL)
	d2free (lista_varClip);

/*	Copia il parametro 'data' in una stringa di appoggio per uso interno */

if (data != NULL)
{
	appoggio = XtNewString (data);

/*	Scandisce tutti i separatori ||| e costruisce tutte le 
	sottostringhe     */

	path_22datClip = strtok (appoggio,"|||");
	str_iniz = strtok (NULL,"|||");
	str_fini = strtok (NULL,"|||");
	str_delt = strtok (NULL,"|||");
	str_nume = strtok (NULL,"|||");


/*	Converte le stringhe lette nei float o negli interi per
	poter essere utilizzati    */

	t_inizialeClip = atof (str_iniz);
	t_finaleClip = atof (str_fini);
	delta_nuovoClip = atof (str_delt);
	numeroClip = atoi (str_nume);

/*	Costruisce la lista dei nomi delle variabili selezionate
	all'interno della clipboard  
	Allocazione di un vettore bidimensionale   */

lista_varClip = AllocaStruttura (numeroClip,LUN_NOME);
if (lista_varClip == NULL) return (ERRORE);
else
	{
	for (i=0;i<numeroClip;i++)
		lista_varClip[i] = strtok (NULL,"|||");
	lista_varClip[numeroClip] = NULL;
	}
}
	return (GIUSTO);
}

/***********************
    Funzione che esegue la cancellazione delle variabili non selezionate 
	nella lista delle variabili della finestra principale
***********************/

F22_CutVarUnsel (posizioni,numero)
int *posizioni,numero;
{
	char **lista_appoggio;
	int num_var_prec,i,ii,j,k;
	float tempo,valore;

	num_var_prec = header1.nvar;
/*******************************************************
	MODIFICA DELLA LISTA "nomi_var"
*******************************************************/
/*	Costruisce la lista dei nomi delle variabili eliminando 
	quelle che devono essere cancellate
	Allocazione di un vettore bidimensionale   */

	lista_appoggio = AllocaStruttura (numero,LUN_NOME);

/*   Scrittura della struttura appoggio saltando i nomi delle
	variabili che devono essere cancellati   */

	k=0;
	for (i=0;i<header1.nvar;i++)
	{
		if ( (i==(posizioni[k]-1)) && (k < numero) )
		{
			strcpy (lista_appoggio[k],nomi_var[i]);
			k++;
		}
	}

/*   Riscrittura della struttura nomi_var con le modifiche  */

	header1.nvar = numero;
	for (i=0;i<header1.nvar;i++)
	{
		for (j=0;j<LUN_NOME;j++)
			nomi_var[i][j] = lista_appoggio[i][j];
        	for(k=0;k<LUN_NOME;k++)
                	if(nomi_var[i][k]==' ') nomi_var[i][k]=0;
	}
	nomi_var[i][0]=0;  /* tappo */

/*  Libera la memoria della lista dei nomi delle variabili di 
	appoggio per poter essere riutilizzata   */

	d2free(lista_appoggio);

/*******************************************************
	MODIFICA DELLA LISTA "simboli"
*******************************************************/
/*
        alloca l'array bidimensionale che conterra' la lista delle
        descrizioni saltando quelle da cencellare.
*/

	lista_appoggio = AllocaStruttura (numero,LUN_SIMB);

/*     Scrittura della struttura appoggio saltando i nomi delle 
	variabili e le descrizioni che devono essere cancellate */

	k=0;
	for (i=0;i<header2.ncasi;i++)
	{
		if ( (i==(posizioni[k]-1)) && (k<numero) )
		{
			strcpy (lista_appoggio[k],simboli[i]);
			k++;
		}
	}

	d2free (simboli);
	simboli = AllocaStruttura (numero,LUN_SIMB);

/*   Riscrittura della struttura simboli con le modifiche  */

	header2.ncasi = numero;
	for (i=0;i<header2.ncasi;i++)
		strcpy (simboli[i],lista_appoggio[i]);
	simboli[header2.ncasi] = NULL;

/*  Libera la memoria della lista dei nomi delle variabili di 
	appoggio per poter essere riutilizzata   */

	d2free(lista_appoggio);

/**************************************************
	SCRITTURA DEL NUOVO FILE F22
**************************************************/

	Apri_f22_temp ();
	rew_dati ();
	Scrivi_header1();
	Scrivi_header2();
	for (i=0;i<numero_campioni+1;i++)
	{
		k=0;
		fread (&tempo,1,sizeof(float),fpDAT);
		fwrite (&tempo,sizeof(float),1,fpAPPO);
		for (j=0;j<num_var_prec;j++)
		{
			fread (&valore,1,sizeof(float),fpDAT);
			if ( (j==(posizioni[k]-1)) && (k<numero) )
				{
				fwrite (&valore,sizeof(float),1,fpAPPO);
				k++;
				}
		}
	}
	Uscita();
	return (GIUSTO);
}

/***********************
    Funzione che esegue la cancellazione delle variabili selezionate 
	nella lista delle variabili della finestra principale
***********************/

F22_CutVar (posizioni,numero)
int *posizioni,numero;
{
	char **lista_appoggio;
	int righe,num_colonne,lung,i,ii,j,k;
	float tempo,valore;

/*******************************************************
	MODIFICA DELLA LISTA "nomi_var"
*******************************************************/
/*	Costruisce la lista dei nomi delle variabili eliminando 
	quelle che devono essere cancellate
	Allocazione di un vettore bidimensionale   */

	righe = header1.nvar - numero;
	lista_appoggio = AllocaStruttura (righe,LUN_NOME);

/*   Scrittura della struttura appoggio saltando i nomi delle
	variabili che devono essere cancellati   */

	k=0;
	j=0;
	for (i=0;i<header1.nvar;i++)
	{
		if (i != (posizioni[j]-1))
		{
			strcpy (lista_appoggio[k],nomi_var[i]);
			k++;
		}else j++;
	}

/*   Riscrittura della struttura nomi_var con le modifiche  */

	header1.nvar -= numero;
	for (i=0;i<header1.nvar;i++)
	{
		lung = strlen (lista_appoggio[i]);
		for (j=0;j<lung;j++)
			nomi_var[i][j] = lista_appoggio[i][j];
        	for(k=0;k<LUN_NOME;k++)
                	if(nomi_var[i][k]==' ') nomi_var[i][k]=0;
	}
	nomi_var[i][0]=0;  /* tappo */
	d2free (lista_appoggio);

/*******************************************************
	MODIFICA DELLA LISTA "simboli"
*******************************************************/
/*
        alloca l'array bidimensionale che conterra' la lista delle
        descrizioni saltando quelle da cencellare.
*/
	lista_appoggio = AllocaStruttura (righe,LUN_SIMB);

/*     Scrittura della struttura appoggio saltando i nomi delle 
	variabili e le descrizioni che devono essere cancellate */

	k=0;
	j=0;
	for (i=0;i<header2.ncasi;i++)
	{
		if (i != (posizioni[j]-1))
		{
			strcpy (lista_appoggio[k],simboli[i]);
			k++;
		}else j++;
	}
	d2free (simboli);
	simboli = AllocaStruttura (righe,LUN_SIMB);

/*   Riscrittura della struttura simboli con le modifiche  */

	header2.ncasi -= numero;
	for (i=0;i<header2.ncasi;i++)
		strcpy (simboli[i],lista_appoggio[i]);
	simboli[header2.ncasi] = NULL;

/**************************************************
	SCRITTURA DEL NUOVO FILE F22
**************************************************/

	Apri_f22_temp ();
	rew_dati ();
	Scrivi_header1();
	Scrivi_header2();
	for (i=0;i<numero_campioni+1;i++)
	{
		k=0;
		fread (&tempo,1,sizeof(float),fpDAT);
		fwrite (&tempo,sizeof(float),1,fpAPPO);
		for (j=0;j<(header1.nvar+numero);j++)
		{
			fread (&valore,1,sizeof(float),fpDAT);
			if ( j != (posizioni[k]-1))
				fwrite (&valore,sizeof(float),1,fpAPPO);
			else k++;
		}
	}
	Uscita ();
	return (GIUSTO);
}

Boolean ControllaPresenza (stringa)
char *stringa;
{
int i,uguali;
Boolean trovata;

i = 0;
uguali = 1;
while ( (nomi_var[i][0] != '\0') && (uguali != 0) )
	{
	uguali = strcmp (stringa,nomi_var[i]);
	i++;
	}
if (uguali == 0) return(False);
return (True);
}

/***********************
    Routine che controlla i nomi della lista letta dalla
	clipboard nella lista delle variabili del file f22
	attualmente aperto
***********************/

char *Controllo_nomivar (confronto)
char *confronto;
{
	int j,k;
	int passate,diverse;
	char *st,*Appoggio;
	Boolean valida;

/*   Trova un nuovo nome di variabile e lo inserisce in coda alla 
	struttura 'nomi_var'  */

	st = XtNewString (confronto);
	Appoggio = XtNewString (confronto);
	valida = False;
	k = 1;
	passate = 0;
	while ( (passate != LUN_NOME) && (valida == False))
		{
		valida = ControllaPresenza (st);
		if (!valida)
			{
			if (k == LUN_NOME)
				{
				k = passate + 1;
				XtFree (st);
				st = XtNewString (Appoggio);
				st[k] = tolower (st[k]);
				passate++;
				XtFree (Appoggio);
				Appoggio = XtNewString (st);
				}
			else
				{
				XtFree (st);
				st = XtNewString (Appoggio);
				st[k] = tolower (st[k]);
				k++;
				}
			}
		}
	if (valida) return (st);
	return (NULL);
}

/***********************
    Routine che esegue il paste chiamato dal menu principale
		 del contenuto della Clipboard
***********************/

F22_PasteVar ()
{
	extern Widget Message;
	extern char *path_22datClip; /* Path del file della Clipboard */
	extern float t_inizialeClip; /* Tempo iniziale del grafico della Clip */
	extern float t_finaleClip;   /* Tempo finale del grafico della Clip */
	extern float nuovo_deltaClip;/* Intervallo di campionamento della Clip */
	extern int numeroClip;       /* Numero di variabili memorizzate nella Clip */
	extern char **lista_varClip; /* Lista variabili della Clip  */

	FILE *fpPASTE;
	char nomevar [LUN_NOME+1];
	char descr [LUN_SIMB+1];
	S_HEAD1 head1_appo;
	S_HEAD2 head2_appo;
	int ind,iret,k,i,j,righe,colonne,lista_indici[NUM_VAR+1],ris;
	char **simb_appo,stringa[200],*Nuova;
	XmString Messaggio;
	float valore;
	float val1[NUM_VAR+1]; /* Vettore di appoggio per interpolazione */
	float val2[NUM_VAR+1]; /* Vettore di appoggio per interpolazione */
	float vals[NUM_VAR+1]; /* Vettore per i valori interpolati  */

        char appo[200];

/*    Controllo che il numero totale di variabili non superi il
	numero massimo consentito       */

	if ((header1.nvar+numeroClip) > NUM_VAR)
	{
		strcpy (stringa,"Max variables number superated!\0");
		Messaggio = XmStringCreate (stringa,XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message,no_grab);
		XmStringFree (Messaggio);
		return (ERRORE);
	}

/*    Controllo che i nomi delle nuove variabili non siano gia'
	presenti all'interno della lista di destinazione   */
/*    Appende in coda alla struttura nomi_var i nomi delle 
	variabili modificate che devono essere aggiunte  */

	for (i = 0; i < numeroClip; i++)
	{
		Nuova = Controllo_nomivar(lista_varClip[i]);
		if (Nuova == NULL)
			{
			nomi_var[header1.nvar][0]=(char)0;
			strcpy (stringa,"Unable to rename variables!\0");
			Messaggio = XmStringCreate (stringa,
					XmSTRING_DEFAULT_CHARSET);
			set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
			set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
			UxPopupInterface (Message,no_grab);
			XmStringFree (Messaggio);
			return (ERRORE);
			}
		else strcpy (nomi_var[header1.nvar+i],Nuova);
	}

/*
        alloca l'array bidimensionale che conterra' la lista delle
        descrizioni con in aggiunta quelle da inserire.
*/

	righe = header2.ncasi + numeroClip;
	simb_appo = AllocaStruttura (righe,LUN_SIMB);

/*   Lettura del file letto dalla clipboard */
/*   Scrittura della struttura di appoggio 'simb_appo'
	 con le nuove modifiche  */

	for (i=0;i<header2.ncasi;i++)
		strcpy (simb_appo[i],simboli[i]);


	fpPASTE = fopen (path_22datClip,"r");
	fread (&head1_appo,sizeof(S_HEAD1),1,fpPASTE);
	for (i=0;i<head1_appo.nvar;i++)
		fread (nomevar,LUN_NOME,1,fpPASTE);
	fread (&head2_appo,sizeof(S_HEAD2),1,fpPASTE);
	j = 0;
	for (i=0;i<head2_appo.ncasi;i++)
	{
		fread (descr,LUN_SIMB,1,fpPASTE);
		if (j < numeroClip)
		{
			ris = strncmp (lista_varClip[j],descr,
				strlen(lista_varClip[j]));
			if (ris == 0)
			{
				lista_indici[j] = i+1;
				strncpy (descr,nomi_var[header1.nvar+j],
					strlen(nomi_var[header1.nvar+j]));
				strcpy (simb_appo[header2.ncasi+j],descr);
				j++;
			}
		}
	}

/*
        alloca l'array bidimensionale 'simboli' che conterra' la lista delle
        descrizioni con in aggiunta quelle da inserire.
*/

	d2free (simboli);
	righe = header2.ncasi + numeroClip;
	simboli = AllocaStruttura (righe,LUN_SIMB);

	for (i=0;i<righe;i++)
		strcpy (simboli[i],simb_appo[i]);
	d2free (simb_appo);

/*****************************************************
	SCRITTURA DEL NUOVO FILE F22
*****************************************************/

	Apri_f22_temp ();
	rew_dati();
	header1.nvar += numeroClip;
	Scrivi_header1();
	header2.ncasi += numeroClip;
	Scrivi_header2();

/*  Lettura del primo vettore di appoggio per 
	l'interpolazione dei valori  dal file
	della Clipboard    */

	k = 1;
	i = 0;
	fread (&val1[0],1,sizeof(float),fpPASTE);
	for (j=1;j<head1_appo.nvar+1;j++)
	{
		fread (&val1[k],1,sizeof(float),fpPASTE);
		if (j == lista_indici[i]) {i++; k++;}
 	}

/*  Lettura del secondo vettore di appoggio per
	l'interpolazione dei valori dal file 
	della Clipboard */

	k = 1;
	i = 0;
	fread (&val2[0],1,sizeof(float),fpPASTE);
	for (j=1;j<head1_appo.nvar+1;j++)
	{
		fread (&val2[k],1,sizeof(float),fpPASTE);
		if (j == lista_indici[i]) {i++; k++;}
 	}

/*  Ciclo di scansione del file aperto e di scrittura del 
	nuovo file f22 con in aggiunta i nuovi valori letti
	dal file della Clipboard  */

	iret = 1;
	for (i=0;i<numero_campioni+1;i++)
	{

	/*  Legge i valori dal file f22 corrente e scrive i 
		valori letti sul nuovo file f22  */

		fread (&vals[0],1,sizeof(float),fpDAT);
		fwrite (&vals[0],sizeof(float),1,fpAPPO);
		for (k=0;k<header1.nvar-numeroClip;k++)
		{
			fread (&valore,1,sizeof(float),fpDAT);
			fwrite (&valore,sizeof(float),1,fpAPPO);
		}

	/*   Esegue il controllo per la preparazione del vettore 
		vals prima di essere scritto nel nuovo file f22 */

		/*  Sposta i tempi a cui puntano val1 e val2 affinche'
			il tempo di vals si all'interno dell'intervallo
			tra val1 e val2    */

		while ((val2[0] < vals[0]) && (iret != NULL))
		{
			k = 1;
			ind = 0;
			for (j=0;j<numeroClip+1;j++) val1[j] = val2[j];
	 		iret = fread (&val2[0],1,sizeof(float),fpPASTE);
			if (iret != NULL)
			{
				for (j=0;j<head1_appo.nvar;j++)
				{
					fread (&val2[k],1,sizeof(float),fpPASTE);
					if ((ind < numeroClip) && (j == lista_indici[ind]-1))
					 {k++; ind++;}
				}
			}
		}

		/*  Se vals si trova tra val1 e val2 viene eseguita
			l'interpolazione tra val1 e val2  */

		if ((val2[0]>vals[0])&&(val1[0]<vals[0]))
		{
			for (j=1;j<numeroClip+1;j++)
			{
				vals [j] = Calcola_XY (val1[j],val2[j],val1[0],
					val2[0],vals[0]);
			}
		}
	
		/* Se vals si trova sullo stesso tempo di val2 i 
			valori di val2 vengono passati a vals  */

		if (val2[0]==vals[0])
		{
			for (j=1;j<numeroClip+1;j++)
				vals[j] = val2[j];
		}

		/* Se vals si trova sullo stesso tempo di val1
			o si trova su un tempo inferiore i valori
			di val1 vengono passati a vals  */

		if (val1[0] >= vals[0])
		{
			for (j=1;j<numeroClip+1;j++)
				vals[j] = val1[j];
		}

		/*  Ciclo di scrittura dei valori di vals sul 
			nuovo file f22   */

		for (j=1;j<numeroClip+1;j++)
			fwrite (&vals[j],sizeof(float),1,fpAPPO);
	}
	fclose (fpPASTE);
	Uscita ();
	return (GIUSTO);
}

/************************
	Routine che esegue la modifica del nome e della 
	descrizione di una variabile presente nella lista
	delle variabili della finestra principale
************************/
EseguiCambioVar (char *NuovoNome,char *NuovaDescr,int indice)
{
	extern Widget Message,Description;

	int i,j,uguali,lung,k;
	char *Destinazione,Mess[100];
	float tempo,valore;
	XmString Messaggio;

	Destinazione = (char *)XtCalloc (LUN_SIMB+1,sizeof(char));
	i=0;
	uguali =1;
	while ( (i<numero_var) && (uguali!=0) )
 		{
 		if (i != indice)
  			uguali = strcmp (nomi_var[i],NuovoNome);
 		i++;
 		}
	if (uguali == 0)
 		{
		strcpy (Mess,"New variable name already exist!\0");
		Messaggio = XmStringCreate (Mess,XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_INFORMATION);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message,no_grab);
		return (ERRORE);
		XmStringFree (Messaggio);
 		}
	else
 		{
 		strcpy (nomi_var[indice],NuovoNome);
		for (i=(strlen(NuovoNome));i<LUN_NOME+1;i++)
			NuovoNome [i] = ' ';
 		strcpy (Destinazione,NuovoNome);
		lung = strlen (NuovaDescr);
		k = 0;
		for (i=LUN_NOME;i<(LUN_NOME+lung+1);i++)
			{
			Destinazione [i] = NuovaDescr[k];
			k++;
			}

		strcpy (simboli[indice],Destinazione);
 		UxPopdownInterface (Description);
 		Apri_f22_temp ();
 		rew_dati ();
 		Scrivi_header1();
 		Scrivi_header2();
 		for (i=0;i<numero_campioni+1;i++)
  			{
  			fread (&tempo,1,sizeof(float),fpDAT);
  			fwrite (&tempo,sizeof(float),1,fpAPPO);
  			for (j=0;j<numero_var;j++)
   				{
   				fread (&valore,1,sizeof(float),fpDAT);
   				fwrite (&valore,sizeof(float),1,fpAPPO);
   				}
  			}
 		Uscita ();
 		}
	XtFree (Destinazione);
	return (GIUSTO);
}

/***********************
	Routine che esegue l'elaborazione lineare di
	una variabile
***********************/
EseguiElabLineare (int posiz_sel,char *Var,char *Descr,float PA,float PB)
{
	extern Widget Message;
	extern Widget CalcWindow;

	float ValSav,valore,tempo;
	char **simb_appo;
	char Mess[100],*Destinazione;
	int colonne,i,j,uguali,righe,lung,k;
	XmString Messaggio;

	Destinazione = (char *)XtCalloc (LUN_SIMB+1,sizeof (char));
/*    Controllo che il numero totale di variabili non superi il
	numero massimo consentito       */

	if ((header1.nvar+1) > NUM_VAR)
	{
		strcpy (Mess,"Max variables number superated!\0");
		Messaggio = XmStringCreate (Mess,XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message,no_grab);
		return (ERRORE);
	}
i=0;
uguali=1;
while ( (i<numero_var) && (uguali != 0) )
	{
	uguali = strcmp (Var,nomi_var[i]);
	i++;
	}
if (uguali == 0)
	{
	strcpy (Mess,"New variable name already exist!\0");
	Messaggio = XmStringCreate (Mess,XmSTRING_DEFAULT_CHARSET);
	set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_INFORMATION);
	set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
	UxPopupInterface (Message,no_grab);
	return (ERRORE);
 	}
else
	{
	/*  Aggiungo il nuovo nome della variabile */
	strcpy (nomi_var[header1.nvar],Var);

	for (i=(strlen(Var));i<LUN_NOME+1;i++)
		Var [i] = ' ';
 	strcpy (Destinazione,Var);
	lung = strlen (Descr);
	k = 0;
	for (i=LUN_NOME;i<(LUN_NOME+lung+1);i++)
		{
		Destinazione [i] = Descr[k];
		k++;
		}
/*
        alloca l'array bidimensionale che conterra' la lista delle
        descrizioni con in aggiunta quelle da inserire.
*/
	righe = header2.ncasi + 1;
	simb_appo = AllocaStruttura (righe,LUN_SIMB);

	/*  Aggiungo la nuova variabile alla struttura simboli  */
	for (i=0;i<header2.ncasi;i++)
		strcpy (simb_appo[i],simboli[i]);
	strcpy (simb_appo[header2.ncasi],Destinazione);
	d2free (simboli);

	/*  Rialloco la struttura simboli con una variabile in piu' */
	simboli = AllocaStruttura (righe,LUN_SIMB);

	for (i=0;i<righe;i++)
		strcpy (simboli[i],simb_appo[i]);
	d2free (simb_appo);

	header1.nvar++;
	header2.ncasi++;

	UxPopdownInterface (CalcWindow);

	Apri_f22_temp ();
	rew_dati ();
 	Scrivi_header1();
 	Scrivi_header2();
 	for (i=0;i<numero_campioni+1;i++)
  		{
		ValSav = 0;
  		fread (&tempo,1,sizeof(float),fpDAT);
  		fwrite (&tempo,sizeof(float),1,fpAPPO);
  		for (j=0;j<numero_var;j++)
   			{
   			fread (&valore,1,sizeof(float),fpDAT);
   			fwrite (&valore,sizeof(float),1,fpAPPO);
			if (j==posiz_sel) ValSav = valore;
   			}
		ValSav = (PA*ValSav) + PB;
		fwrite (&ValSav,sizeof(float),1,fpAPPO);
  		}
 	Uscita ();
	}
XtFree (Destinazione);
return (GIUSTO);
}

/***********************
	Routine che esegue la somma algebrica di
	due variabili
***********************/
EseguiSommaAlg (int posiz1,int posiz2,char *Var,char *Descr)
{
	extern Widget Message;
	extern Widget CalcWindow;

	float ValSav,valore,tempo;
	char *p_simbappo,**simb_appo;
	char Mess[100],*Destinazione;
	int colonne,i,j,uguali,righe,lung,k;
	XmString Messaggio;

	Destinazione = (char *)XtCalloc (LUN_SIMB+1,sizeof (char));
/*    Controllo che il numero totale di variabili non superi il
	numero massimo consentito       */

	if ((header1.nvar+1) > NUM_VAR)
	{
		strcpy (Mess,"Max variables number superated!\0");
		Messaggio = XmStringCreate (Mess,XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message,no_grab);
		return (ERRORE);
	}
i=0;
uguali=1;
while ( (i<numero_var) && (uguali != 0) )
	{
	uguali = strcmp (Var,nomi_var[i]);
	i++;
	}
if (uguali == 0)
	{
	strcpy (Mess,"New variable name already exist!\0");
	Messaggio = XmStringCreate (Mess,XmSTRING_DEFAULT_CHARSET);
	set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_INFORMATION);
	set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
	UxPopupInterface (Message,no_grab);
	return (ERRORE);
 	}
else
	{
	/*  Aggiungo il nuovo nome della variabile */
	strcpy (nomi_var[header1.nvar],Var);

	for (i=(strlen(Var));i<LUN_NOME+1;i++)
		Var [i] = ' ';
 	strcpy (Destinazione,Var);
	lung = strlen (Descr);
	k = 0;
	for (i=LUN_NOME;i<(LUN_NOME+lung+1);i++)
		{
		Destinazione [i] = Descr[k];
		k++;
		}
/*
        alloca l'array bidimensionale che conterra' la lista delle
        descrizioni con in aggiunta quelle da inserire.
*/
	righe = header2.ncasi + 1;
	simb_appo = AllocaStruttura (righe,LUN_SIMB);

	/*  Aggiungo la nuova variabile alla struttura simboli  */
	for (i=0;i<header2.ncasi;i++)
		strcpy (simb_appo[i],simboli[i]);
	strcpy (simb_appo[header2.ncasi],Destinazione);
	XtFree (p_simbappo);
	d2free (simboli);

	/*  Rialloco la struttura simboli con una variabile in piu' */
	simboli = AllocaStruttura (righe,LUN_SIMB);

	for (i=0;i<righe;i++)
		strcpy (simboli[i],simb_appo[i]);
	d2free (simb_appo);

	header1.nvar++;
	header2.ncasi++;

	UxPopdownInterface (CalcWindow);

	Apri_f22_temp ();
	rew_dati ();
 	Scrivi_header1();
 	Scrivi_header2();
 	for (i=0;i<numero_campioni+1;i++)
  		{
		ValSav = 0;
  		fread (&tempo,1,sizeof(float),fpDAT);
  		fwrite (&tempo,sizeof(float),1,fpAPPO);
  		for (j=0;j<numero_var;j++)
   			{
   			fread (&valore,1,sizeof(float),fpDAT);
   			fwrite (&valore,sizeof(float),1,fpAPPO);
			if ((j==posiz1) || (j==posiz2)) ValSav += valore;
   			}
		fwrite (&ValSav,sizeof(float),1,fpAPPO);
  		}
 	Uscita ();
	}
XtFree (Destinazione);
return (GIUSTO);
}

char *ScriviPezzetto(char *Inp)
{
	char *appo;
	int lung,i;
	
	appo = (char *)XtCalloc (17,sizeof(char));
	strcpy (appo,Inp);
	lung = strlen (Inp);
	for (i=lung;i<15;i++)
		appo[i] = ' ';
	appo[15] = '\t';
	appo[16] = '\0';
	return (appo);
}

char *SalvaInAscii (int *Lista,int Numero)
{
	char *Ritorno,*appogg,*Riga;
	int i,j,k,kk,lung,Lun;
	FILE *fpASCII;
	char *Pezzetto,Scarica[15];
	float valore,tempo;


	appogg = XtNewString (getcwd((char *)NULL,50));
	Ritorno = XtNewString (tempnam(appogg, (char *)NULL ));

	fpASCII=fopen(Ritorno,"w");

	/*  Scrive il nome delle variabili selezionate con a 
		fianco la relativa descrizione   */

	for (i=0;i<Numero;i++)
		{
		kk = Lista[i] - 1;
		Riga = XtNewString (simboli[kk]);
		lung = strlen (Riga);
		fprintf (fpASCII,"%s\n",Riga);
		}
	XtFree (Riga);

	/*  Scrive una riga composta da 4 asterischi e dal
		titolo della simulazione */

	Lun = strlen (header1.titolo) + 7;
	Riga = (char *)XtCalloc (Lun,sizeof(char));
	strcpy (Riga,"****  ");
	strcat (Riga,header1.titolo);
	fprintf (fpASCII,"%s\n",Riga);
	XtFree (Riga);

	/*  Scrive una riga contenente l'intestazione delle variabili  */

	Pezzetto = ScriviPezzetto ("TIME");
	Riga = (char *)XtCalloc(16*(Numero+1),sizeof(char));
	strcpy (Riga,Pezzetto);
	for (i=0;i<Numero;i++)
		{
		kk = Lista[i] - 1;
		Pezzetto = ScriviPezzetto (nomi_var[kk]);
		strcat (Riga,Pezzetto);
		XtFree (Pezzetto);
		}
	fprintf (fpASCII,"%s\n",Riga);

	/*  Scrittura delle righe quante sono i campioni presenti su 
		file f22  */

	rew_dati();
	for (i=0;i<numero_campioni;i++)
		{
		k = 0;
  		fread (&tempo,1,sizeof(float),fpDAT);
		sprintf (Scarica,"%f\0",tempo);
		Pezzetto = ScriviPezzetto (Scarica);
		strcpy (Riga,Pezzetto);
		XtFree (Pezzetto);
  		for (j=0;j<numero_var;j++)
   			{
   			fread (&valore,1,sizeof(float),fpDAT);
			if (j==Lista[k]-1)
				{
				sprintf (Scarica,"%E\0",valore);
				Pezzetto = ScriviPezzetto (Scarica);
				strcat (Riga,Pezzetto);
				k++;
				XtFree (Pezzetto);
				}
   			}
		fprintf (fpASCII,"%s\n",Riga);
		}
	fclose (fpASCII);
	XtFree (appogg);
	XtFree (Riga);
	return (Ritorno);
}

rout_appo ()
{
	return (0);
}
