/**********************************************************************
*
*       C Source:               conv_r.c
*       Subsystem:              2
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Mar 31 17:07:23 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: conv_r.c-10 %  (%full_filespec: conv_r.c-10:csrc:2 %)";
#endif
/*
   modulo conv_r.c
   tipo 
   release 1.19
   data 11/10/95
   reserved @(#)conv_r.c	1.19
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)conv_r.c	1.19\t11/10/95";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/BulletinB.h>
#include <Xm/DrawingA.h>
#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xl/XlP.h>
#include <Xl/XlCore.h>
#include <Xl/XlComposite.h>
#include <Xl/XlUtilBkg.h>
#include <Xl/XlPort.h>
#include <Xl/XlIconReg.h>
#include <Xl/XlDispReg.h>

#include <Ol/OlForm.h>
#include <Ol/OlForm_regol.h>

#include "util.h"
#include "UxXt.h"
#include "libutilx.h"
#include <sqlite3.h>

XtAppContext    UxAppContext;

char nome_file_input[100];
char nome_file_lib[255];
char file_lib[255];
char path_ico[255];
char path_pag[255];
char file_porte[255];
char file_icone[255];
int modo_icona;
int modo_lib=0;

FILE *fp_input;
Widget topLevel;
XtAppContext app_context;
Widget main_window_widget;
Widget drawing;
Widget drawing_lib;
Display * display;
int     screen_num;
BCK_OBJ *sfondo;
Region region;
WidgetList wlist,wlist_lib;
Widget UxTopLevel;
int num_widget,num_widget_lib;
XrmDatabase RisDb;
XrmDatabase RisDbLib;
int i;
Arg args[50];
Cardinal lnargs;
char file_sfondo[255];
char path_lib[255];
char file_sfondo_bkg[255];
char file_pagina[255];
char file_14[255];
int dim[4];

static void   expose_drawing( );
void sistema_figli();
Boolean CreaPagina();
static Widget * CreoOggetti();
static void get_punto_porta(Widget,int*,int*);
Boolean su_porta();
OlFormObject ResGetForm (Widget );
void get_child();
char *copia_n ();
char *getpul ();
extern void sistema_file(char *);
void legge_sfondo(FILE *fp, char *nome);
int rd_f14 (int , int *, int *);
void leggi_porte(char *);
void get_size_icone(char *);
void skip_header(FILE *);
int legge_riga(FILE *, char *);
void legge_icona(FILE *, char *);
void leggi_posizioni_porte();
void SistemaInputValue(WidgetList ,int );
static void scrivi_connessioni(char *);
int to_snap(int );
static void SettaTags(char *, OlFormObject , Widget ,int ,char **);

extern CONNESSIONI conn[3000];
extern int num_conn;

TAB_PORTE tab_porte[4000];
int num_porte=0;


#define NVAR_IN_ICON 50  /* Max numero di variabili in un icona */
#define LUNSIGL   5      /* Lunghezza delle sigle */
#define LUNNOME   9      /* Lunghezza dei nomi */
#define LUNDESC  80      /* Lunghezza stringhe descrittive dell'icona */

typedef struct
   {
   char nome[LUNNOME];
   char blocco[LUNNOME];
   char descr[LUNDESC];
   float fval;
   } VARSDATI;

typedef struct
   {
   char nome[LUNNOME];
   char descr[LUNDESC];
   float fval;
   } VARS;

typedef struct
   {
   char blocco[LUNNOME];
   char descr[LUNDESC];
   } ELENCO_BLOCCHI;

typedef struct
   {
   char nome[100];
   char nome_lib[100];
   char nome_figlio[100]
   } ELENCO_FIGLI;

ELENCO_BLOCCHI elenco_blocchi[1000];
int num_blocchi=0;

VARS *varsch;
int num_varsch;

VARSDATI *vardati;
int num_vardati;

sqlite3 *db; 

int main(argc,argv)
int argc;
char **argv;
{
char riga[MAXRIGA];

topLevel = XtAppInitialize(&app_context, "xaing",
                                     NULL, 0, &argc, argv, NULL, NULL, 0);

        display = XtDisplay(topLevel);
        screen_num = XDefaultScreen(display);

        XtVaSetValues(topLevel,
                        XmNx, 0,
                        XmNy, 0,
                        XmNwidth, DisplayWidth(display, screen_num),
                        XmNheight, DisplayHeight(display, screen_num),
                        NULL);
                                     


if(argc!=8)
  exit(fprintf(stderr,
  "usa: conv_l <nome_file> <path_pag> <path_ico> <path_lib> <file_porte> <file_icone> <file_14>\n"));

sprintf(nome_file_input,"%s.dxf",argv[1]);
strcpy (path_pag,argv[2]);
strcpy (path_ico,argv[3]);
strcpy (path_lib,argv[4]);
strcpy (file_porte,argv[5]);
strcpy (file_icone,argv[6]);
strcpy (file_14,argv[7]);
sprintf(file_sfondo,"%s/%s",path_pag,argv[1]);
printf("Converto il file <%s>\n",nome_file_input);
XlPutenv("LEGOMMI_ICO",path_ico);

if((fp_input=fopen(nome_file_input,"r"))<=0)
	exit(fprintf(stderr,"il file %s.dxf non esiste\n",nome_file_input));
/*
 Legge file dati (Es: CSP1_14.DAT)
*/
rd_f14(5000,&num_varsch,&num_vardati);
/*
 Legge file P-CSRPR.OUT contenente le informazioni sulle
 porte interne alle icone di regolazione.
*/
leggi_porte(file_porte);
/*
 Legge file I-SCHM.OUT contenente nomi e posizioni
 icone nello schema e topologia e posizioni connessioni
*/
get_size_icone(file_icone);

/*
 Lettura file DXF (file A-SCHM.dxf)
*/
/*
 salta parte inutilizzata del formato DXF
*/
skip_header(fp_input);

/*
	skip 4 righe
*/
legge_riga(fp_input,riga);
legge_riga(fp_input,riga);
legge_riga(fp_input,riga);
legge_riga(fp_input,riga);

/*
	lettura icone
*/
/*
 Setta il flag modo_icona che se settato a 1 fa si' che le poliline
 siano tradotte nel formato .ico come POLY se settato a 0 le
 poliline vengono tradotte come CONN (uso per connessioni)
*/
modo_icona=1;
while (legge_riga(fp_input,riga)==1)
      {
      if(strcmp(riga,"ICON")==0)
	{
	legge_riga(fp_input,riga);
	legge_riga(fp_input,riga);
/*
 Legge il disegno dell'icona e se non esiste il corrispondente file
 .ico lo crea (col nome dell'icona) e lo scrive traducendo il DXF in
 formato .ico
*/
	legge_icona(fp_input,riga);
	}
      if(strcmp(riga,"ENDSEC")==0)
	break;
      }

modo_icona=0;
/***
modo_icona=0;
legge_sfondo(fp_input,argv[1]);

fclose(fp_input);
***/

    i = 0;
    XtSetArg (args[i], XmNwidth, 150);
    i++;
    XtSetArg (args[i], XmNheight, 40);
    i++;
    main_window_widget = XmCreateMainWindow (topLevel,
                "pagina1", args, i);
    XtManageChild(main_window_widget);
    drawing = XtVaCreateManagedWidget( "drawing",
                        xmDrawingAreaWidgetClass, main_window_widget,
                        XmNmarginWidth, 0,
                        XmNmarginHeight, 0,
                        XmNheight, 1190,
                        XmNwidth, 1200,
                        XmNy, 14,
                        XmNx, 4,
                        XmNresizePolicy, XmRESIZE_NONE,
                        NULL );
    drawing_lib = XtVaCreateManagedWidget( "drawing_lib",
                        xmDrawingAreaWidgetClass, main_window_widget,
                        XmNmarginWidth, 0,
                        XmNmarginHeight, 0,
                        XmNheight, 1190,
                        XmNwidth, 1200,
                        XmNy, 14,
                        XmNx, 4,
                        XmNresizePolicy, XmRESIZE_NONE,
                        NULL );
    sprintf(file_pagina,"%s.pag",file_sfondo);
    sprintf(file_lib,"%s/REGOLIB.lib",path_lib);
/*
 Crea la pagina della libreria (REGOLIB.lib)
*/
    RisDbLib=XrmGetFileDatabase(file_lib);
    display=XtDisplay(drawing_lib);
    display->db=RisDbLib;
    CreaPagina(drawing_lib,&wlist_lib,&num_widget_lib);
    leggi_posizioni_porte();
/*
 Legge lo sfondo della pagina che contiene le connessioni
 Scrive di conseguenza il file .bkg e il file .pag inserendo
 le icone nella corretta posizione e trasformando i campi testo
 in labels.
*/
    legge_sfondo(fp_input,argv[1]);
    fclose(fp_input);
/*
 Crea la pagina che sta convertendo
*/
    RisDb=XrmGetFileDatabase(file_pagina);
    display=XtDisplay(drawing);
    display->db=RisDb;
    CreaPagina(drawing,&wlist,&num_widget);
    SistemaInputValue(wlist,num_widget);

    XtAddCallback( drawing, XmNexposeCallback,
                        expose_drawing,NULL);
    XtRealizeWidget (topLevel);

    XtAppMainLoop (app_context);
}


static void     expose_drawing( UxWidget, UxClientData, UxCallbackArg
 )
        Widget          UxWidget;
        XtPointer       UxClientData, UxCallbackArg;
{
static int prima=1;

    if(prima==1)
	{
	prima=0;
    	/*
		legge lo sfondo
    	*/
    	sprintf(file_sfondo_bkg,"%s.bkg",file_sfondo);
    	scrivi_connessioni(file_sfondo_bkg);
    	sistema_file(file_sfondo_bkg);
    	leggi_file_bck(drawing,file_sfondo,"bkg",&sfondo,dim);
    	printf("leggi_file_bck [%s]\n",file_sfondo);
	sistema_figli();
	}
        disegna_bck(UxWidget,sfondo,UxCallbackArg,&(region),False);
}

/*
        Carico la pagina sulla DrawingArea [w]
*/
Boolean CreaPagina(Widget w, WidgetList *widgetlist, int *num_widget)
{
Display *display;
AN_OBJ *plista_an_obj;



*num_widget=0;


/*
        leggo l'elenco degli oggetti
*/
plista_an_obj=XlGetAnObjList(w);

/*
        Creo gli oggetti
*/
*widgetlist=CreoOggetti(w,plista_an_obj);

while(plista_an_obj[*num_widget].nome[0] !='\0')
        (*num_widget)++;


return(True);
}


static Widget * CreoOggetti(Widget wpadre, AN_OBJ *plista_an_obj)
{
Widget *lista_wid;
int num_wid,i;
Arg args[1];

/*
 Conta il numero di oggetti da creare (la lista e' terminata
 da un nome a NULL)
*/
num_wid=0;
while(plista_an_obj[num_wid].nome[0] !='\0')
        {
        num_wid++;
        }
/*
 Alloca la lista per memorizzare i widgets
*/
lista_wid=(Widget *)calloc(num_wid+1,sizeof(Widget *));
/*
 Crea i widget in lista
*/
XtSetArg(args[0],XlNconfig,True);
for(i=0;i<num_wid;i++)
        {
        lista_wid[i]=XtCreateWidget(plista_an_obj[i].nome,
                                        plista_an_obj[i].classe,
                                        wpadre,args,1);
        XtManageChild(lista_wid[i]);
        }
return(lista_wid);
}

void scrivi_connessioni(char *nome_file)
{
FILE *fp_sfondo;
int i,j,k;
char riga[500];
int x,y;
Widget p1,p2,padre1,padre2;
char nome[10];
char *app_char;
Position  x1,x2,y1,y2,px1,px2,py1,py2;
int tipo;
int cx,cy;

riga[0]=0;

fp_sfondo=fopen(nome_file,"w");
for(i=0;i<num_conn;i++)
        {
	/*
		ricavo gli indici di widget delle porte
	*/
	strcpy(nome,conn[i].nomep1);
	app_char=strstr(nome,"w");
	app_char++;
	*app_char=0;
	padre1=XtNameToWidget(drawing,nome);
	p1=XtNameToWidget(padre1,conn[i].nomep1);
	strcpy(nome,conn[i].nomep2);
	app_char=strstr(nome,"w");
	app_char++;
	*app_char=0;
	padre2=XtNameToWidget(drawing,nome);
	p2=XtNameToWidget(padre2,conn[i].nomep2);
	/*
		ricava le posizioni delle porte
	*/
	k=0;
    	XtSetArg (args[k], XmNx, &x1);k++;
    	XtSetArg (args[k], XmNy, &y1);k++;
    	XtSetArg (args[k], XlNtipoPort, &tipo);k++;
	XtGetValues(p1,args,k);
	k=0;
    	XtSetArg (args[k], XmNx, &x2);k++;
    	XtSetArg (args[k], XmNy, &y2);k++;
	XtGetValues(p2,args,k);
	k=0;
    	XtSetArg (args[k], XmNx, &px1);k++;
    	XtSetArg (args[k], XmNy, &py1);k++;
	XtGetValues(padre1,args,k);
	k=0;
    	XtSetArg (args[k], XmNx, &px2);k++;
    	XtSetArg (args[k], XmNy, &py2);k++;
	XtGetValues(padre2,args,k);

/*
 a seconda del tipo disegna la connessione rossa o blu
*/
        fprintf(fp_sfondo,"wi_c 0\n");
        fprintf(fp_sfondo,"st_c 0\n");
	if((tipo==0)||(tipo==1))
	{
        fprintf(fp_sfondo,"fg_c red\n");
        fprintf(fp_sfondo,"bg_c red\n");
        fprintf(fp_sfondo,"fg_f red\n");
	}
	else
	{
        fprintf(fp_sfondo,"fg_c blue\n");
        fprintf(fp_sfondo,"bg_c blue\n");
        fprintf(fp_sfondo,"fg_f blue\n");
	}
        fprintf(fp_sfondo,"fill 0\n");
        sprintf(riga,"Conn %d",conn[i].num+2);
	get_punto_porta(p1,&cx,&cy);
	if(!su_porta(p1,cx,cy))
		fprintf(stderr,"errore connessione porta [%s]\n",XtName(p1));
        sprintf(riga,"%s %d %d",riga,cx,cy);
        for(j=0;j<conn[i].num;j++)
                {
		conn[i].x[j]=to_snap(conn[i].x[j]);
		conn[i].y[j]=to_snap(conn[i].y[j]);
                sprintf(riga,"%s %d %d",riga,conn[i].x[j],conn[i].y[j]);
                }
	get_punto_porta(p2,&cx,&cy);
	if(!su_porta(p2,cx,cy))
		fprintf(stderr,"errore connessione porta [%s]\n",XtName(p2));
        sprintf(riga,"%s %d %d",riga,cx,cy);
        fprintf(fp_sfondo,"%s\n",riga);
        }
fclose(fp_sfondo);
}

/*
 get_punto_porta
 calcola il punto in cui parte la connessione
*/
void get_punto_porta(w,xpunto,ypunto)
Widget w; /* widget della porta */
int *xpunto;
int *ypunto;
{
Arg arg[4];
Dimension width,height;
Position x,y,px,py;

XtSetArg(arg[0],XmNx,&x);
XtSetArg(arg[1],XmNy,&y);
XtSetArg(arg[2],XmNwidth,&width);
XtSetArg(arg[3],XmNheight,&height);
XtGetValues(w,arg,4);
XtSetArg(arg[0],XmNx,&px);
XtSetArg(arg[1],XmNy,&py);
XtGetValues(XtParent(w),arg,2);
*xpunto = (int) (x + px + width/2);
*ypunto = (int) (y + py + height/2);
}

/*
 su_porta
 resituisce True se l'estremo della connessione (filo) identificata dalle
 coordinate xfilo yfilo si trova sulla porta avente come indice di widget
 w
*/
Boolean su_porta(w,xfilo,yfilo)
Widget w; /* widget porta */
int xfilo,yfilo;
{
Arg arg[4];
int xporta,yporta; /* coordinate del punto di arrivo o partenza
                    della connessione */

get_punto_porta(w,&xporta,&yporta);
if(xfilo == xporta && yfilo == yporta)
        return(True);
else
        return(False);
}

/*
 Modifica nel file .pag  in base a quanto letto su file 14
	i valori degli InputValue 
	le descrizioni delle icone di regolazione 
 sistema la tipologia delle porte contenute nelle icone di interfaccia
 trasformando in porte del tipo interfaccia quelle non connesse.
*/
void SistemaInputValue(WidgetList wlist,int num_widget)
{
int i,j,NumeroForm,k,z;
OlFormObject PuntForm;
char *Buffer,*Appoggio,*Lista=NULL;
FILE *fp;
WidgetList figli;
Cardinal num_figli;
char *tagName;
int tipoPort;

fp=fopen(file_pagina,"a");


for(i=0;i<num_widget;i++)
  {
  if ( XlIsIconReg (wlist[i]) && (!XlIsInterfaceIconReg(wlist[i])))
	{
/*
 Ricava la descrizione dell'icona dai dati memorizzati in elenco
 blocchi durante la lettura del file F14
*/
	XtSetArg(args[0],XlNtagName,&tagName);
	XtGetValues(wlist[i],args,1);
	for(z=0;z<num_blocchi;z++)
		{
		if(memcmp(tagName,&elenco_blocchi[z].blocco[4],2)==0)
			{
	      		fprintf(fp,"*%s.iconDescr: %s\n",
				XtName(wlist[i]),elenco_blocchi[z].descr);
			break;
			}
		}
/*
 Inserisce nel campo InputValue i valori di variabili e dati
 letti in precedenza dal file F14
*/
	PuntForm = ResGetForm (wlist[i]);
	if(PuntForm!=NULL)
	{
	NumeroForm = OlGetNumForms (PuntForm);
	Lista=NULL;
	for(j=0;j<NumeroForm;j++)
		{
		Buffer = OlWriteStrInp (PuntForm,&Appoggio,j);
	          if (Buffer != NULL)
       			{
       			if (Lista == NULL)
       			   {
       			   Lista = XtCalloc (strlen(Buffer)+1,sizeof(char));
       			   strcpy (Lista,Buffer);
       			   }
       			else
       			   {
       			   Lista = XtRealloc (Lista, 
		             (strlen(Lista)+strlen(Buffer)+2)*sizeof(char));
              		   strcat (Lista," ");
              		   strcat (Lista,Buffer);
              		   }
            		XtFree (Buffer);
            		}
		}
	if(Lista!=NULL)
	   {
	   if(Lista[0]!=' ')
	{

	if(PuntForm!=NULL)
		{
		NumeroForm = OlGetNumForms (PuntForm);
		for(j=0;j<NumeroForm;j++)
			{
			Buffer = OlGetTags(PuntForm,j);
			SettaTags(Buffer,PuntForm,wlist[i],j,&Lista);
			XtFree(Buffer);
			}
		}
 

	      fprintf(fp,"*%s.inputValue: %s\n",XtName(wlist[i]),Lista);
	}
       	   XtFree (Lista);
       	   }
	} /* fine ciclo PuntForm */
	}
/*
 Se l'icona e' del tipo icona di interfaccia setta come porta di interfaccia
 la porta che non e' collegata (tale porta e' quella che deve essere
 collegabile con gli schemi esterni) setta inoltre correttamente il
 tipo della icona di interfaccia (se di input o se di output)
*/
  if ( XlIsInterfaceIconReg(wlist[i]))
	{
	get_child(wlist[i],&figli,&num_figli);
	for(k=0;k<num_figli;k++)
		if((XlIsPort(figli[k]))&&(!XlPortIsConnected(figli[k])))
		{
	        fprintf(fp,"*%s.interfacePort: 1\n",XtName(figli[k]));
		XtSetArg(args[0],XlNtipoPort,&tipoPort);
		XtGetValues(figli[k],args,1);
		if( XlPortIsInput(figli[k]))
			{
			if(tipoPort == INPUT_PORT_A)
				fprintf(fp,"*%s.nomeFile: INTECA\n",
						XtName(wlist[i]));
			else
				fprintf(fp,"*%s.nomeFile: INTECD\n",
						XtName(wlist[i]));
			fprintf(fp,"*%s.iconRegType: 1\n",XtName(wlist[i]));
			}
		else
			{
			if(tipoPort == OUTPUT_PORT_A)
				fprintf(fp,"*%s.nomeFile: INTEUA\n",
						XtName(wlist[i]));
			else
				fprintf(fp,"*%s.nomeFile: INTEUD\n",
						XtName(wlist[i]));
			fprintf(fp,"*%s.iconRegType: 2\n",XtName(wlist[i]));
			}
		}
	}
    }
fclose(fp);
}

OlFormObject ResGetForm (Widget wid)
{
OlFormObject IndiceForm;
Widget WidgetPadre;

if ( XlIsIconReg (wid) )
  {
  /*  Prendo il puntatore alla form e lo restituisco
        se la form non e' allocata la risorsa XlNobjectForm
        e' settata a NULL  */
  get_something (wid, XlNobjectForm, (void*) &IndiceForm);
  return (IndiceForm);
  }
if ( (XlIsPort (wid)) || (XlIsDispReg (wid)) )
  {
  /*  Prendo il padre per verificare se e' la
        drawing area o se e' una IconReg  */
  WidgetPadre = XtParent (wid);
  /*  Controllo se e' una IconReg e se non lo e'
        viene ritornato NULL  */
  if ( XlIsIconReg (WidgetPadre) )
    {
    /*  Prendo il puntatore alla form e lo restituisco
        se la form non e' allocata la risorsa XlNobjectForm
        e' settata a NULL  */
    get_something (WidgetPadre, XlNobjectForm, (void*) &IndiceForm);
    return (IndiceForm);
    }
  else return (NULL);
  }
return (NULL);
}
                                                          

void get_child(Widget wid,WidgetList *childList,Cardinal *numChild)
{
    lnargs=0;
    XtSetArg(args[lnargs],XmNnumChildren,numChild);lnargs++;
    XtSetArg(args[lnargs],XmNchildren,childList);lnargs++;
    XtGetValues(wid,args,lnargs);
}
   

int rd_f14 (int num, int *valori_letti ,int *valori_letti_dati)
{
FILE *f14;
char riga[256];
char fil14[80];
char messaggio[80];
int cont_ast = 0;
int conta=0;
int nv = 0;
int nd = 0;
int diff;
int i = 0;
int ret;
char file_da[200];
char file_a[200];
char comando[400];
char nome_blocco[20];
char descr_blocco[100];
char valore[30];

/* Allocazione struttura ed apertura file */
   strcat(fil14,file_14);

   printf("Lettura file [%s]\n",file_14);

   varsch = (VARS*)XtCalloc(num,sizeof(VARS));
   vardati = (VARSDATI*)XtCalloc(num,sizeof(VARSDATI));
   f14 = fopen (file_14, "r");
   if (!f14)
      {
      XtFree((char*)varsch);
      strcpy(messaggio,"Errore in apertura del file ");
      strcat(messaggio,fil14);
      return(-1);
      }

/* Lettura schema_14.dat */
   memset(riga,0,255);
   while( getpul (riga, 255, f14) )
      {
      /*printf("(%d)riga=%s\n",strlen(riga),riga);*/
      if (riga[0] == '*')
         {
         if (++cont_ast >= 6)
            break;
         }
      else
         {
         riga[24] = '\0';
	 strcpy(varsch[nv].descr,&riga[45]);
	varsch[nv].descr[LUNDESC-1]=0;
         copia_n (varsch[nv].nome, riga+4, 8);
         for(i=0;(varsch[nv].nome[i]!=' ')&&(i<strlen(varsch[nv].nome));i++);
         varsch[nv].nome[i] = '\0';
         varsch[nv].fval = (float) atof(riga+14);
         nv++;
         }
   	memset(riga,0,255);
      }
   getpul (riga, 255, f14); 
   	memset(riga,0,255);
   while( getpul (riga, 255, f14) )
      {
      if(strncmp(riga,"*LG*EOF",7)==0)
	continue;
      if(strncmp(riga,"*   -",5)==0)
	continue;
      if(strncmp(riga,"*   BLOCCO ",11)==0)
	{
	memcpy(nome_blocco,&riga[11],8);
	nome_blocco[8]=0;
	if(riga[24]!=NULL)
		{
		strcpy(descr_blocco,&riga[24]);
		strcpy(elenco_blocchi[num_blocchi].blocco,nome_blocco);
		strcpy(elenco_blocchi[num_blocchi].descr,descr_blocco);
		num_blocchi++;
		}
	}
      else
	{
	strcpy(vardati[nd].blocco,nome_blocco);
	strcpy(vardati[nd].descr,descr_blocco);
	memcpy(vardati[nd].nome,&riga[4],6);
	vardati[nd].nome[6]=0;
	memcpy(valore,&riga[14],10);
	valore[10]=0;
	vardati[nd].fval=atof(valore);
	nd++;
	if(strlen(riga)>30)
		{
		memcpy(vardati[nd].nome,&riga[29],6);
		strcpy(vardati[nd].blocco,nome_blocco);
		vardati[nd].nome[6]=0;
		memcpy(valore,&riga[39],10);
		valore[10]=0;
		vardati[nd].fval=atof(valore);
		nd++;
		}
	}
      memset(riga,0,255);
      }
   fclose (f14);

/*
   qsort (varsch, nv, sizeof(VARS), strcmp);
*/
/***
   for(i=0;i<nv;i++)
     printf("%s=%f  %s\n",varsch[i].nome,varsch[i].fval,varsch[i].descr);
***/
   (*valori_letti) = nv;
   (*valori_letti_dati) = nd;

   return(1);
}

char *getpul (char *str, int n, FILE *fp)
{
char *ptr;
if (! fgets (str, n, fp))
   return NULL;
ptr = str + strlen(str) - 1;
for ( ; ptr >= str && (unsigned)*ptr <= 32; )
   *(ptr--) = '\0';
return (str);
}

char *copia_n (char *str1, char *str2, int n)
{
int i, j;

for (i = 0, j = 1; i < n; i++)
   if (j)
      j = (int)(str1[i] = str2[i]);
   else
      str1[i] = '\0';
str1[n] = '\0';
return (str1);
}
         
void SettaTags(char *buffer, OlFormObject PuntForm, Widget w,int indice,char **Lista)
{
int num;
char *buf;
int i,j;
char riga[MAX_L_TAGS];
char *tag;
char sigla[10];
char valore[40];

if(buffer==NULL)
	return;

buf=buffer;

memcpy(&num,buf,sizeof(int));
buf+=sizeof(int);
for(i=0;i<num;i++)
	{
	memcpy(riga,buf,MAX_L_TAGS);
	riga[MAX_L_TAGS-1]=0;
	buf +=MAX_L_TAGS;
	if(indice==OL_FORM_INPUT_VALUE)
		{
		XtSetArg(args[0],XlNtagName,&tag);
		XtGetValues(w,args,1);
		memcpy(sigla,riga,2);
		sigla[2]=0;
		sprintf(sigla,"%s%s",sigla,tag);
		for(j=0;j<num_varsch;j++)
		  if(strcmp(sigla,varsch[j].nome)==0)
			{
			sprintf(valore,"%f",varsch[j].fval);
			InsTagVal(PuntForm,OL_FORM_INPUT_VALUE,
				sigla,2,Lista,valore);
			InsTagDescr(PuntForm,OL_FORM_INPUT_VALUE,
				sigla,2,Lista,varsch[j].descr);
			}
		}
	if(indice==OL_FORM_INPUT_DATA)
		{
		XtSetArg(args[0],XlNtagName,&tag);
		XtGetValues(w,args,1);
		for(j=0;j<num_vardati;j++)
		  if((memcmp(tag,&vardati[j].blocco[4],2)==0)&&
			(memcmp(riga,vardati[j].nome,6)==0))
			{
			sprintf(valore,"%f",vardati[j].fval);
			InsTagVal(PuntForm,OL_FORM_INPUT_DATA,
				vardati[j].nome,6,Lista,valore);
			}
		}
	}
}


/*
 Ricava dalla libreria le posizioni e i tipi di tutte
 le porte
*/
void leggi_posizioni_porte()
{
int i;
int k,j;
char *nome_icona;
char *tag_porta;
WidgetList wport;
Cardinal npor;
Position x,y;
int rotate;
int ass_rotate;
int tipo;
int interface;
int layout;

for(i=0;i<num_porte;i++)
	{
	/* cerca l'icona */
	for(k=0;k<num_widget_lib;k++)
	   {
    	   XtSetArg (args[0], XlNnomeFile, &nome_icona);
	   XtGetValues(wlist_lib[k],args,1);
	   if(strcmp(nome_icona,tab_porte[i].nome_icona)==0)
		break;
	   }
	if(k==num_widget_lib)
	  exit(printf("icona [%s] non trovata\n",tab_porte[i].nome_icona));
	/* cerca la porta */
	get_child(wlist_lib[k],&wport,&npor);
	for(j=0;j<npor;j++)
	   {
    	   XtSetArg (args[0], XlNportName, &tag_porta);
	   XtGetValues(wport[j],args,1);
	   if(strcmp(tag_porta,tab_porte[i].nome_porta)==0)
		break;
	   }
	if(j==npor)
	  exit(printf("icona [%s][%s] porta [%s] non trovata\n",
		tab_porte[i].nome_icona,
		XtName(wlist_lib[k]),tab_porte[i].nome_porta));
	k=0;
    	XtSetArg (args[0], XlNx0, &x);k++;
    	XtSetArg (args[1], XlNy0, &y);k++;
    	XtSetArg (args[2], XlNrotate, &rotate);k++;
    	XtSetArg (args[3], XlNassRotate, &ass_rotate);k++;
    	XtSetArg (args[4], XlNinterfacePort, &interface);k++;
    	XtSetArg (args[5], XlNlayoutPort, &layout);k++;
    	XtSetArg (args[6], XlNtipoPort, &tipo);k++;
	XtGetValues(wport[j],args,k);
	tab_porte[i].pos_x=x;
	tab_porte[i].pos_y=y;
	tab_porte[i].rotate=rotate;
	tab_porte[i].ass_rotate=ass_rotate;
	tab_porte[i].interface=interface;
	tab_porte[i].layout=layout;
	tab_porte[i].tipo=tipo;
	}
}


/*
	aggiunge agli oggetti della pagina le risorse e i figli configurati 
	in librerie
*/
void sistema_figli()
{
ELENCO_FIGLI elenco[2000];
int i,j,k;
WidgetList figli_lib;
Cardinal num_figli_lib;
WidgetList figli;
Cardinal num_figli;
WidgetList figli_ico;
Cardinal num_figli_ico;
WidgetList figli_mom;
Cardinal num_figli_mom;
int numero_figli = 0;
char *nome_icona;
char *nome_icona_lib;
FILE *fp;

/*
	file_lib 	-> 	file della libreria
	file_pagina	-> 	file della pagina
*/

fp=fopen(file_pagina,"a");
printf("Aggiorno il file [%s]\n",file_pagina);

get_child(drawing_lib,&figli_lib,&num_figli_lib);
get_child(drawing,&figli,&num_figli);
for(i=0;i<num_figli;i++)
	{
	int prog = 100;
	char nuovo_nome[100];
	char elenco_figli[5000];
	char *app_ele;
	char *NomeClasseOggetto;
	WidgetClass ClasseOggetto;
	XtResourceList resources;
   	Cardinal num_resources;
   	XrmValue xrm_value;
   	char *str_type;
	int z;


	if(XlIsIconReg(figli[i]))
		{



		get_child(figli[i],&figli_mom,&num_figli_mom);
		prog = num_figli_mom;
    		XtSetArg (args[0], XlNlistChildren, &app_ele);
		XtGetValues(figli[i],args,1);
		elenco_figli[0] = 0;
		strcpy(elenco_figli,app_ele);
		sprintf(elenco_figli,"*%s.listChildren: %s",XtName(figli[i]),
			elenco_figli);

    	   	XtSetArg (args[0], XlNnomeFile, &nome_icona);
	   	XtGetValues(figli[i],args,1);
		for(j=0;j<num_figli_lib;j++)
		   if(XlIsIconReg(figli_lib[j]))
			{
    	   		XtSetArg (args[0], XlNnomeFile, &nome_icona_lib);
	   		XtGetValues(figli_lib[j],args,1);
			if(strcmp(nome_icona,nome_icona_lib)==0)
				break;
			}
		/*
		trasferisce le risorse dell'iconReg
		*/
		ClasseOggetto = XtClass (figli_lib[j]);
		XtGetResourceList(ClasseOggetto,&resources,&num_resources);
   		for( z=0; z<num_resources; z++)
   		{
   		char nome_risorsa[100];

		if((strcmp(resources[z].resource_name,XlNx0)==0)||
		(strcmp(resources[z].resource_name,XlNy0)==0)||
		(strcmp(resources[z].resource_name,XlNrotate)==0)||
		(strcmp(resources[z].resource_name,XlNassRotate)==0)||
		(strcmp(resources[z].resource_name,XlNlistChildren)==0)||
		(strcmp(resources[z].resource_name,XlNnumFigli)==0)||
		(strcmp(resources[z].resource_name,XlNiconDescr)==0)||
		(strcmp(resources[z].resource_name,XlNinputValue)==0)||
		(strcmp(resources[z].resource_name,XlNtagName)==0)||
		(strcmp(resources[z].resource_name,XlNiconRegType)==0)
			)
			continue;
   
   		sprintf(nome_risorsa,"%s.%s",XtName(figli_lib[j]),
				resources[z].resource_name);

        	if(XrmGetResource(RisDbLib,nome_risorsa, 
			resources[z].resource_class,
					&str_type,&xrm_value)==True)
      		{
		if(strcmp(xrm_value.addr,"INTEFA") &&
			strcmp(xrm_value.addr,"INTEFD"))
			fprintf(fp,"*%s.%s : %s\n",XtName(figli[i]),
				resources[z].resource_name,xrm_value.addr);
   		}
		}
		get_child(figli_lib[j],&figli_ico,&num_figli_ico);
		for(k=0;k<num_figli_ico;k++)
		   if(!XlIsPort(figli_ico[k]))
			{
			prog ++;
			sprintf(nuovo_nome,"%s%dc",XtName(figli[i]),prog-1);
			ClasseOggetto = XtClass (figli_ico[k]);
			sprintf(elenco_figli,"%s %s %s",elenco_figli,
				nuovo_nome,XlGetClassName (ClasseOggetto));
			XtGetResourceList(ClasseOggetto,
				&resources,&num_resources);
	/*
		trasferisce le risorse dei figli dell'icona di regolazione
	*/
   			for( z=0; z<num_resources; z++)
   			{
   			char nome_risorsa[100];
   
   			sprintf(nome_risorsa,"%s.%s",XtName(figli_ico[k]),
					resources[z].resource_name);

        		if(XrmGetResource(RisDbLib,nome_risorsa, 
					resources[z].resource_class,
					&str_type,&xrm_value)==True)
      			{
		        fprintf(fp,"*%s.%s : %s\n",nuovo_nome,
				resources[z].resource_name,xrm_value.addr);
   			}
			}
		fprintf(fp,"%s\n",elenco_figli);
		fprintf(fp,"*%s.numFigli: %d\n",XtName(figli[i]),prog);
		}
	}
}
fclose(fp);
}
