/*
   modulo crea_rego_lib.c
   tipo 
   release 1.4
   data 10/19/95
   reserved @(#)crea_rego_lib.c	1.4
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)crea_rego_lib.c	1.4\t10/19/95";
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
#include <Xl/XlUtilBkg.h>
#include <Xl/XlPort.h>
#include <Xl/XlIconReg.h>
#include <Xl/XlDispReg.h>
#include <Ol/OlForm.h>

 

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

char nome_file_input[100];
char path_ico[255];
char path_lib[255];
char nome_file_lib[255];
char path_pag[255];
int modo_icona=1;
int modo_lib=1;

FILE *fp_input;

TAB_PORTE tab_porte[4000];
int num_porte=0;

Widget topLevel;
XtAppContext app_context;
Widget main_window_widget;
Widget drawing;
Widget drawing_lib;
Display * display;
int     screen_num;
WidgetList wlist,wlist_lib;
Widget UxTopLevel;
int num_widget,num_widget_lib;
XrmDatabase RisDb;
int i;
Arg args[50];
Cardinal lnargs;
char file_sfondo[255];
char path_lib[255];
char file_sfondo_bkg[255];
char file_pagina[255];
char file_14[255];
int dim[4];

sqlite3 *db;


static Widget * CreoOggetti();
Boolean CreaPagina();
static void   expose_drawing( );
OlFormObject ResGetForm (Widget );
void leggi_porte(char *);
void skip_header(FILE *);
int legge_riga(FILE *, char *);
void SistemaInputValue(WidgetList ,int );
void insert_icona_lib(char *);
static void crea_header_lib();
void legge_icona(FILE *, char *);

int main(argc,argv)
int argc;
char **argv;
{
char riga[MAXRIGA];


if(argc!=4)
	exit(fprintf(stderr,"usa: crea_rego_lib <nome_file> <path_lib> <path_ico>\n"));
sprintf(nome_file_input,"%s.dxf",argv[1]);
strcpy (path_lib,argv[2]);
strcpy (path_ico,argv[3]);
printf("Converto il file <%s>\n",nome_file_input);
crea_header_lib();
leggi_porte("P-CSRPR.OUT");

if((fp_input=fopen(nome_file_input,"r"))<=0)
	exit(fprintf(stderr,"il file %s.dxf non esiste\n"));

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
while (legge_riga(fp_input,riga)==1)
      {
      if(strcmp(riga,"ICON")==0)
	{
	legge_riga(fp_input,riga);
	legge_riga(fp_input,riga);
	legge_icona(fp_input,riga);
	}
      if(strcmp(riga,"ENDSEC")==0)
	break;
      }
printf("Fine lettura icone\n");
insert_icona_lib(NULL);

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

XlPutenv("LEGOMMI_ICO",path_ico);

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

    sprintf(file_lib,"%s/REGOLIB.lib",path_lib);
/*
 Crea la pagina della libreria (REGOLIB.lib)
*/
    RisDb=XrmGetFileDatabase(file_lib);
    display=XtDisplay(drawing);
    display->db=RisDb;
    CreaPagina(drawing,&wlist,&num_widget);
    display=XtDisplay(drawing);

    XtAddCallback( drawing, XmNexposeCallback,
                        expose_drawing,NULL);
                                              
    XtRealizeWidget (topLevel);


    XtAppMainLoop (app_context);

}



void crea_header_lib()
{
FILE *fp;

nome_file_lib[0]=0;

sprintf(nome_file_lib,"%s/REGOLIB.lib",path_lib);
fp=fopen(nome_file_lib,"w");
fprintf(fp,"*nomePag: REGOLIB\n");
fprintf(fp,"*top_x: 100\n");
fprintf(fp,"*top_y: 100\n");
fprintf(fp,"*top_width: 900\n");
fprintf(fp,"*top_height: 900\n");
fprintf(fp,"*top_tipo: Libreria\n");
fprintf(fp,"*top_descrizione: Icone importate da Regograf\n");
fprintf(fp,"*drawing_width:    900\n");
fprintf(fp,"*drawing_height:   1000\n");
fprintf(fp,"*drawing_background:   lime green\n");
fprintf(fp,"*refresh_freq:   0\n");
fprintf(fp,"*tagPag:\n");
fprintf(fp,"*nextTagReg:\n");
fprintf(fp,"*numTagReg: 0\n");
fclose(fp);
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

void SistemaInputValue(WidgetList wlist,int num_widget)
{
int i,j,NumeroForm,k,z;
OlFormObject PuntForm;
char *Buffer,*Appoggio,*Lista=NULL;
FILE *fp;
WidgetList figli;
Cardinal num_figli;
char *tagName;

fp=fopen(file_lib,"a");


for(i=0;i<num_widget;i++)
  {
  if ( XlIsIconReg (wlist[i]) && (!XlIsInterfaceIconReg(wlist[i])))
        {
	printf("IconReg [%s]\n",XtName(wlist[i]));
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
/***
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
****/
              fprintf(fp,"*%s.inputValue: %s\n",XtName(wlist[i]),Lista);
        }
           XtFree (Lista);
           }
        } /* fine ciclo PuntForm */
	}
  }

fclose(fp);
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
    	SistemaInputValue(wlist,num_widget);
        }
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
                                                    
