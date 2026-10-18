/**********************************************************************
*
*       C Source:               other_mmi.c
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Fri Mar 10 11:06:41 2006 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: other_mmi.c-21 %  (%full_filespec: other_mmi.c-21:csrc:3 %)";
#endif

/*
        Inizio sezione per SCCS
*/
/*
   modulo other_mmi.c
   tipo 
   release 5.3
   data 1/22/96
   reserved @(#)other_mmi.c	5.3
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)other_mmi.c	5.3\t1/22/96";
/*
        Fine sezione per SCCS
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#endif
#if defined VMS
# include "vmsipc.h"
#include <unixlib.h>
#endif
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <Xm/TextF.h>
#include <Xm/Text.h>
#include <Xl/XlP.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlManagerP.h>
#include <X11/CompositeP.h>
#include <Xl/XlChangePage.h>
#include <Xl/XlCai.h>
#include <Xl/XlIndicP.h>
#include <Xl/XlDispReg.h>
#include <Xl/XlPort.h>
#include <Xl/XlIconReg.h>
#include <Ol/OlDatabasePunti.h>
#include <Ol/OlDatabaseTopologia.h>
#include <Ol/OlPertP.h>

#include <Ol/OlTree.h>		/* CAPPE oggetto OlTree */

#include <Xl/XlUtilBkg.h>
#include "other.h"
#include "ric_aing.h"
#include <Rt/RtMemory.h>
#include <UxXt.h>
#include <time.h>
#include "fontfallback.h"



#define IND_TEST 3


/*
	elenco variabili gobali
*/
/* dichiarazioni funzioni interne */
void ripulisci_files();
Boolean PosMouse( );
void show_warning( );
Boolean RaisePlotAing();
Boolean XlSetConfigOff(WidgetList ,Cardinal );
Boolean OlSetDatabasePunti(WidgetList ,Cardinal , OlDatabasePuntiObject );
Boolean OlSetDatabaseTopologia(WidgetList ,Cardinal ,OlDatabaseTopologiaObject );
Boolean OlSetPert(WidgetList ,Cardinal ,OlPertObject );
Boolean XlCreatePixmapFromGIF(Widget, char*, Pixmap *);
Boolean XlCvtStrToPixel (Display *,char *,Pixel *,unsigned short *,unsigned short *, unsigned short *);
char *XlGetFirstVarGraf (Widget );

Widget	create_VariableEditor( swidget, Widget, Widget, int*, OlDatabaseTopologiaObject );


char *XlGetFirstVcc (Widget );
Boolean XlIsIndicTelep (Widget );
Widget	create_Aing( unsigned char	*, Widget );
void DestroyAing(Widget );
void telep_chiudi_pag( Widget );
int getTimeLicense(double *);
void	quit_proc();
Boolean RtCheckPointer(void *);
void *RtRecord(void *punt);
Boolean XlIsAnimateIcon (Widget );
Boolean XlSetVarGrafico(Widget, char *,int ,int, int);




extern Widget listDrawing[MAX_PAGE];
extern OlDatabasePuntiObject database_simulatore;
static int errori_database_simulatore=0;
extern OlDatabaseTopologiaObject database_topologia;

extern OlTreeObject root_oltree;	/* CAPPE oggetto OlTree */

extern OlPertObject pert;
extern AN_PAG *pagine;
extern int totale_pagine;
extern char *nome_file_context;
extern char app_pathdb[LUN_PATH];
extern char app_pathproject[LUN_PATH];

int pid_aing = -1;
PAGINE_APERTE *el_pagine=NULL;
int num_el_pagine=0;
AN_DISPLAY *elenco_display;
int numero_display;
char startup_pages[MAX_STARTUP_PAGES][L_NOME_PAGINE];
int num_startup_pages;
char background_pages[MAX_BACKGROUND_PAGES][L_NOME_PAGINE];
int num_background_pages;
char noclose_pages[MAX_NOCLOSE_PAGES][L_NOME_PAGINE];
int num_noclose_pages;
Boolean topologia_on=False;
int port_daemon=-1;
extern Boolean alldisp; /* default startup pages su tutti i display configurati */
extern int mmiMaxConErr;

/*
	elenco funzioni private
*/
static Widget * CreoOggetti(Widget , AN_OBJ *);
static int SelectRunTime();
/*
elenco funzioni importate
*/
extern void IcoSelect();
extern void IcoSelectStaz();
extern void PageSelect();
extern void PageStazSelect();
extern int teleperm_IcoSelect();
extern int teleperm_apriStaz();
extern void AingStartEnable(Widget, int);

extern double tempo_license;/*
Funzioni esportate
*/
Widget ActivateVarAing(Widget *);
void ActivateVarPlot(Widget *);
extern Boolean XlSetSomething(WidgetList ,Cardinal ,char * , char * , char * );
extern void XlSetSelectRunTime();
//void XlOperableKeysSendPert( Widget  );
int teleperm_ApplSelect( Widget , Environment *, Widget );
Boolean XlIsOperableKeys( Widget);
Boolean XlIsOperableKeysDirect( Widget  );
Boolean XlIsCaiAll (Widget );
extern void teleperm_naviga();
void XlOperableKeysSendPert( Widget  );
Boolean XlIsOperableKeysImp( Widget  );
Boolean XlIsOperableKeysExec( Widget  );
Boolean XlIsCai (Widget );
int OlJumpPage(OlDatabasePuntiObject ,char *,char );
Boolean XlIsChangePage( Widget );
Boolean XlIsInterfaceIconReg(Widget );
char * XlGetInterfaceIconRegPagConnect(Widget);
size_t RtMemoryAllocated(void *);
Widget	popup_Plot( unsigned char	*, Widget );


/*
	Verifico l'esistenza della pagina
*/
Boolean EsistePagina(char *Pagina)
{
FILE *fp;
char nome_file[100];

if(Pagina[0]==0)
	return(False);

sprintf(nome_file,"%s/%s.rtf",XlGetenv("LEGOMMI_PAGINE"),Pagina);

if(!(fp=fopen(nome_file,"r")))
	{
	printf("La Pagina non esiste\n");
	return(False);
	}
fclose(fp);
return(True);
}

/*
        Restituisce il numero della pagina a partire dal nome
*/
int NumeroPagina(char *Pagina)
{
int i;
for(i=0;i<totale_pagine;i++)
    if(strcasecmp(pagine[i].nome,Pagina)==0)
		return(i);
return(-1);
}

/*
	Carico il database delle risorse dal file selezionato
*/
XrmDatabase CaricaDB(Display *display,char *Pagina)
{
XrmDatabase RisDb;  /* data base delle risorse  contenuto in Pagina */
char nome_file[FILENAME_MAX];


sprintf(nome_file,"%s/%s.rtf",XlGetenv("LEGOMMI_PAGINE"),Pagina);
/*
	Carico il database delle risorse contenuto in Pagina
*/

RisDb=XrmGetFileDatabase(XlConvPathVms(nome_file));

if(RisDb == NULL)
	return(RisDb);
/*
 Associo il database letto al display
*/
XrmSetDatabase(display,RisDb);

return(RisDb);
}

/*
	Carico la pagina sulla DrawingArea [w]
*/
Boolean CreaPagina(Widget w, WidgetList *widgetlist, int *num_widget)
{
Display *display;
AN_OBJ *plista_an_obj;
int num_wid=0;



*num_widget=0;


/*
	leggo l'elenco degli oggetti
*/
plista_an_obj=XlGetAnObjList(w);

while(plista_an_obj[num_wid].nome[0] !='\0')
        {
        if(strcmp(plista_an_obj[num_wid].nome,"Allarmi")==0)
		{
        	if(!OlCanOpenAllarmPage(database_simulatore))
			{
        		show_warning("Allarm page already open\n");
                	return(False);
			}
		}
        num_wid++;
        }

/*
	Creo gli oggetti
*/
*widgetlist=CreoOggetti(w,plista_an_obj);


/*
	setto la Routine di select run-time
*/
while(plista_an_obj[*num_widget].nome[0] !='\0')
        (*num_widget)++;
/*
	impongo la risorsa config = False
*/
XlSetConfigOff(*widgetlist, *num_widget);

/*
	impongo la risorsa XlNdispRegMode a DISP_VALUE_MODE per gli
	eventuali oggetti DispReg presenti nella pagina
*/
XlSetSomething(*widgetlist,*num_widget,"DispReg",
		XlNdispRegMode,(char*)DISP_VALUE_MODE);

/*
	impongo la risorsa XlNvisibleMode a VISIBLE_PORT_ON per gli
	eventuali oggetti Port presenti nella pagina
*/
XlSetSomething(*widgetlist,*num_widget,"Port",
		XlNvisibleMode,(char*)VISIBLE_PORT_ON);

/*
	libero la lista
*/
XlFreeAnObjList(plista_an_obj);

return(True);
}


/*
	Elimina tutti gli oggetti di una pagina
*/
Boolean DistruggiPagina(WidgetList widgetlist, int num_widget)
{
int i;
	for(i=0;i< num_widget; i++)
		XtDestroyWidget(widgetlist[i]);
}


/* set della callback su evento di select 
   ( la XlSetSelectRunTime sceglie su quali classi settare la CB)
*/
Boolean SettaSelect()
{
   XlSetSelectRunTime((caddr_t)SelectRunTime);
}


/* callback per la apertura di una pagina
   puo' essere attivata da SINOTTICO per click su change page
   o per click su un tipo di widget diverso con il bottone STAZ 
   della pagina selezionato
   oppure su pagina STAZIONI per click su change page
*/
static int SelectRunTime(Widget cw)
{
   extern void OperatingWindow_OpenPage();
   extern void apriExt2();
   extern void store_pert();
   //extern XlOperableKeysSendPert();
   Widget drawing;
   char *pagina;
   char app_pag[300];
   int tipo;
   int num_pagina;
   Environment UxEnv;
/* Parte per Direct Jump per Cai */
   int indice;
   char tipo_all;
   int stato;
   char *punt;
   OlTreeObject curr_oltree;
   int progress;

/* 
Recupera l'id dello widget della drawing area 
*/
   if(XlIsXlComposite(XtParent(cw)))
	drawing=XtParent(XtParent(cw));
   else
	drawing=XtParent(cw);

/*
Se deriva da un tipo di pagina teleperm, verifica che non sia richiesto
Info, Plot o Aing
*/

    if(strcmp(XtName(drawing),DRAWING_TELE)==0)  
       {
       /*
       Se deve essere visualizzata un' applicazione
       ritorna e non fa null' altro
       */
       if(teleperm_ApplSelect(XtParent(XtParent(drawing)),&UxEnv,cw))
          return 1;
       }



/* discrimina subito il tipo di classe dello widget
   il quale puo' essere di tipo ChangePage o InterfaceIconReg
   oppure un oggetto Cai oppure un CaiAll
*/
   if( XlIsOperableKeys( cw ) )
   {
      
      /* in base al tipo invio la perturbazione
        se OPKEY_DIRECT subito
        se OPKEY_IMPULSE solo se e' stato abilitato dalla pressione tasto IMP
        se OPKEY_EXEC    memorizzo solo la perturbazione che partira con il tasto EXECUTE
        Questa gestione viene fatta solo per le Operating Window
        Nelle pagine non OperatingWindow permetto l'invio delle
        perturbazioni solo dagli OperableKeys tipo direct
      */ 
      if( strcmp(XtName(drawing),DRAWING_OW ) == 0 )
      {
         if( XlIsOperableKeysDirect(cw) )
            XlOperableKeysSendPert( cw ); 
         else if( XlIsOperableKeysImp(cw) )
            XlOperableKeysSendPert( cw ); 
         else if( XlIsOperableKeysExec(cw)  )
            store_pert( cw ); 
      }
      else
      {
         if( XlIsOperableKeysDirect(cw) )
            XlOperableKeysSendPert( cw ); 
         else
            printf("Only OperableKeysDirect can send pertubation on non OperatingWindow page\n");
      }
   }
   if( XlIsCaiAll(cw) )
	{
        /*
        Per prima cosa acquisisco la risorsa gerarchia dall' oggetto CaiAll
        */
        get_something(cw,XlNvarGerarchia,(void*) &punt);

	if(punt==NULL)
		{
		printf("\n other_mmi:VALORE di gerarchia del Cai non corretto!!!\n");
		return(-1);
		}
	else        /* Conversione formato gerarchia */
		{
		format_hierarchy_new(app_pag,punt);
		if(!strcmp(app_pag,"errore"))
			return(-1);
		printf("\nVALORE di gerarchia.str_input=%s\n",app_pag);
		}
	/*
	Vado ad acquisire l' indice della pagina corrispondente alla gerarchia
	associata al CaiAll. Se si tratta di una operating window vado a restituire
	l' indice della pagina padre della O.W. 
	*/
	if(OlTreeGetPage(root_oltree,&curr_oltree,&progress,&indice,app_pag)==True)
		{
		/*
		Controllo (DA AGGIUNGERE) per verificare se il nodo e' di tipo Operating Window
		*/

/****************** PER OP> WINDOW*******************************/
		while(strcmp(pagine[indice].tipo,"Teleperm"))
			{
			OlTreeGetTop(curr_oltree,&indice);
			}

		teleperm_naviga(drawing,GOTOPAG,indice);
		}
	else
		{
		printf("WARNING: Impossible find a page with hierarchy=%s\n",app_pag);
		return(-1);
		}

	} /* end if( XlIsCaiAll) */
   else if( XlIsCai(cw) )
	{
	punt=NULL;
	/*
	(Solo per pagine non teleperm)
	If Info push-button is on -> popup Info interface (formInfo )
	*/
	/*
	Per prima cosa acquisisco la risorsa gerarchia dall' oggetto Cai
	*/

	get_something(cw,XlNvarGerarchia,(void*) &punt);
printf("Valore di punt=%s\n",punt);
	if(punt==NULL)
		{
		 printf("\n other_mmi:VALORE di gerarchia del Cai non corretto!!!\n");
		return(-1);
		}
	else
		{
		format_hierarchy_new(app_pag,punt);
		if(!strcmp(app_pag,"errore"))
			return(-1);
		printf("\nVALORE di gerarchia.str_input=%s\n",app_pag);
		}
/*
	Devo sapere in che stato e' il Cai
*/

	if(XlGetCaiState(cw,&tipo_all,&stato))
		{
		if(stato!=BLINK_ON_FQZ1) /* Cai deve essere in allarme A */
			return(-1);
/*
	Chiamo metodo di Ol, che mi restituisce l' indice della pagina a cui
dovro' saltare
*/
		indice=OlJumpPage(database_simulatore,app_pag,tipo_all);
	/*
	Una volta acquisito l' indice di pagina da aprire scorro
	la lista delle pagine.
	Se la pagina e' di tipo teleperm basta invocare teleperm_navigation,
	altrimenti prendo il nome della pagina e utilizzo la normale
	ChangePage.
	Se pero' il parent e' una form -> siamo nel teleperm (Cai su pullDown
	Menu).
	*/
		if(indice>=0)
              		if( strcmp(XtName(drawing),DRAWING_TELE ) == 0 )
					{
					while(strcmp(pagine[indice].tipo,"Teleperm"))
						{
						if(OlTreeGetPage(root_oltree,&curr_oltree,&progress,&indice,pagine[indice].gerarchia)==True)
                        				OlTreeGetTop(curr_oltree,&indice);
						else
							{
							printf("ERROR impossible retrieving pag=%s\n",pagine[indice].nome);
							return(-1);
							}
						} /* end while */
						
					teleperm_naviga(drawing,GOTOPAG,indice);
					}

			else
				{
/*
Caso per Cai sui bordi della pagina teleperm
(in tal caso i Cai sono tutti figli di una Form)
METODO provvisorio
*/
				if(!strcmp(XlGetClassName(XtClass(drawing)),
						"XmForm"))
					{
					while(strcmp(pagine[indice].tipo,"Teleperm"))
                        			{
						if(OlTreeGetPage(root_oltree,&curr_oltree,&progress,&indice,pagine[indice].gerarchia)==True)
                        				OlTreeGetTop(curr_oltree,&indice);
						else
							{
							printf("ERROR impossible retrieving pag=%s\n",pagine[indice].nome);
							return(-1);
							}
							
                        			}
					teleperm_naviga(drawing,GOTOPAG,indice);
					}
				else /* pagina non teleperm */
			        	PageSelect(drawing,pagine[indice].nome,NEW_PAGE); 
				}

		}/* end if XlGetCaiState */
	}
   else if( (XlIsChangePage(cw)) || (XlIsInterfaceIconReg(cw)) )
   {
      /* change page
         10/7/97 carlo aggiunta gestione tipo OPEN_EXT2
      */
      if( XlIsChangePage(cw) ) 
      {
           get_something(cw,XlNpageName,(void*) &pagina);
           get_something(cw,XlNchangeType,(void*) &tipo);
           /* se il change page e' di OPEN_EXT2
              chiamo la funzione di apertura della extension2
              passandogli come parametro il parent del changePage
              La pagina deve essere di tipo DRAWING_OW.
           */
           if( tipo == OPEN_EXT2 )
           {
              if( strcmp(XtName(drawing),DRAWING_OW ) == 0 )
                   apriExt2( XtParent(cw) );
           }
           else
           {
	    if(tipo==CLOSE_PAGE || EsistePagina(pagina))
              {		
              if( strcmp(XtName(drawing),DRAWING_TELE ) == 0 )
                {
                if((num_pagina=NumeroPagina(pagina))!=-1)
                   {
                   if(strcmp(pagine[num_pagina].tipo,"Stazioni"))
	              teleperm_naviga(drawing,GOTOPAG,num_pagina);
                   else
                      {
                      printf("DEBUG Sto per aprire una STAZ !!!\n");
                      teleperm_apriStaz(pagina,
                                         XtParent(cw),
                                         "nome_display",
                         pagine[num_pagina].sfondo);
                      }
                   }
                else
                   fprintf(stderr,"WARNING: Page =%s not in list\n",pagina);
                }
	      else if( strcmp(XtName(drawing),DRAWING_OW) == 0 )
                 OperatingWindow_OpenPage(drawing,pagina,tipo);
              else if( strcmp(XtName(drawing),DRAWING_SIN ) == 0)
                {
                PageSelect(drawing,pagina,tipo);
                }
	      else if( strcmp(XtName(drawing),DRAWING_STAZ) == 0 )
                 {
                 PageStazSelect(drawing,pagina,tipo);
                 }
              }
           }
      }

      /* lo widget e' una icona di interfaccia (regolazione) 
      */
      if(XlIsInterfaceIconReg(cw))
      {
          /* apro la pagina associata alla icona di interfaccia
          */
	  strcpy(app_pag,XlGetInterfaceIconRegPagConnect(cw));

	  if( strlen(app_pag) )
	  {
	     if(strcmp(XtName(drawing),DRAWING_STAZ))
                if( strcmp(XtName(drawing),DRAWING_TELE ) == 0 )
/*
                   teleperm_changePage(drawing,app_pag,0);
*/
	        teleperm_naviga(drawing,GOTOPAG,NumeroPagina(app_pag));

                else
	           PageSelect(drawing, app_pag,0);
	     else 
	        PageStazSelect(drawing,app_pag,0);
	  }
       }
   }
   else
   {
         if( strcmp(XtName(drawing),DRAWING_TELE ) == 0 )
            teleperm_IcoSelect(XtParent(XtParent(drawing)),&UxEnv,cw);
         else if( (strcmp(XtName(drawing),DRAWING_STAZ)==0)  ||
                  (strcmp(XtName(drawing),DRAWING_OW)==0)   )
	    IcoSelectStaz(cw);
         /* Se click su Aing o Plot */
         else if ( (strcmp(XtName(drawing),DRAWING_PLOT) == 0)  ||
             (strcmp(XtName(drawing),DRAWING_AING)==0)        )
             {
             return 1;
             }
         else
	    IcoSelect(cw);
   }

   RaisePlotAing();

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
if( (lista_wid=(Widget *)calloc(num_wid+1,sizeof(Widget *))) == NULL )
   return(NULL);


/*
 Crea i widget in lista 
*/
XtSetArg(args[0],XlNconfig,False);
for(i=0;i<num_wid;i++)
        {
        lista_wid[i]=XtCreateManagedWidget(plista_an_obj[i].nome,
                                        plista_an_obj[i].classe,
                                        wpadre,args,1);
      
        }
OlSetDatabasePunti(lista_wid,num_wid,database_simulatore);
if(topologia_on )
	OlSetDatabaseTopologia(lista_wid,num_wid,database_topologia);
OlSetPert(lista_wid,num_wid,pert);
return(lista_wid);
}

Boolean GetResDrawing(Widget w, int *width, int *height, Pixel *background,
	Pixmap *pixmap)
{
unsigned short dummy;
char *background_str; /* stringa contenente o il colore di background
                         o il nome del file .gif da caricare sullo
                         sfondo */
static XtResource res_drawing_width[]= { { XtNdrawing_width, XtCDrawing_width, 
			XmRInt, sizeof(int), 0, XmRImmediate, 0 } };
static XtResource res_drawing_height[]={{XtNdrawing_height, XtCDrawing_height, 
			XmRInt, sizeof(int), 0, XmRImmediate, 0 } };
static XtResource res_drawing_background[]={{XtNdrawing_background, 
			XtCDrawing_background, 
			XmRString, sizeof(char *), 0, XmRImmediate, "black" } };

XtVaGetApplicationResources(w,width,res_drawing_width,
		XtNumber(res_drawing_width),NULL);
XtVaGetApplicationResources(w,height,res_drawing_height,
		XtNumber(res_drawing_height),NULL);
XtVaGetApplicationResources(w,&background_str,res_drawing_background,
		XtNumber(res_drawing_background),NULL);

if(strstr(background_str,".gif"))
	{
	XlCreatePixmapFromGIF(w,background_str,pixmap);
	}
else
	{
	XlCvtStrToPixel(XtDisplay(w),background_str,background,
				&dummy,&dummy,&dummy);
	pixmap=0;
	}

return(True);
}

Boolean GetResTopLevel(Widget w, int *x, int *y, int *width, int *height, 
			int *time_ref, char **tipo, char **descrizione)
{
static XtResource res_top_x[]= { { XtNtop_x, XtCTop_x, XmRInt, 
			sizeof(int), 0, XmRImmediate, 0 } };
static XtResource res_top_y[]= { { XtNtop_y, XtCTop_y, XmRInt, 
			sizeof(int), 0, XmRImmediate, 0 } };
static XtResource res_top_width[]= { { XtNtop_width, XtCTop_width, XmRInt, 
			sizeof(int), 0, XmRImmediate, 0 } };
static XtResource res_top_height[]= { { XtNtop_height, XtCTop_height, XmRInt, 
			sizeof(int), 0, XmRImmediate, 0 } };
static XtResource res_top_tipo[]= { { XtNtop_tipo, XtCTop_tipo, XmRString, 
			sizeof(char *), 0, XmRString, "" } };
static XtResource res_top_descrizione[]= { { XtNtop_descrizione, 
			XtCTop_descrizione, XmRString, 
			sizeof(char *), 0, XmRString, "" } };
static XtResource res_refresh_freq[]={{XlNrefreshFreq, XlCRefreshFreq, 
			XmRInt, sizeof(int), 0, XmRImmediate, (char *)10 } };

/*        risorsa x       */
XtVaGetApplicationResources(w,x,res_top_x,XtNumber(res_top_x),NULL);
XtVaGetApplicationResources(w,y,res_top_y,XtNumber(res_top_y),NULL);
XtVaGetApplicationResources(w,width,res_top_width,
		XtNumber(res_top_width),NULL);
XtVaGetApplicationResources(w,height,res_top_height,
		XtNumber(res_top_height),NULL);
XtVaGetApplicationResources(w,tipo,res_top_tipo,
		XtNumber(res_top_tipo),NULL);
XtVaGetApplicationResources(w,descrizione,res_top_descrizione,
		XtNumber(res_top_descrizione),NULL);
XtVaGetApplicationResources(w,time_ref,res_refresh_freq,
		XtNumber(res_refresh_freq),NULL);
return(True);
}


Boolean LoadBitmap(Widget w, Pixmap *stip, char * bits, int width, int height)
{
long cnero,cbianco;
Pixel background;

get_something(w,XmNbackground,(void*) &background);

if((*stip=XCreatePixmapFromBitmapData(XtDisplay(w),
            RootWindow(XtDisplay(w),DefaultScreen(XtDisplay(w))),bits,
            width,height,
	    BlackPixel(XtDisplay(w),DefaultScreen(XtDisplay(w))), background,
            DefaultDepth(XtDisplay(w),DefaultScreen(XtDisplay(w)))))== False)
        {
        fprintf(stderr,"\n Errore creazione pixmap");
	ripulisci_files();
        exit(1);
        }
}
/*
Funzione che inserisce la drawing area di una pagina nell' elenco delle
pagine aperte
*/
int InsElencoPagine(char *nome_pagina, WidgetList w, int num, int t_ref)
{
int ret;
int i;

for(i=0;i<num_el_pagine;i++)
	if(el_pagine[i].w==NULL)
		break;

if(i==num_el_pagine)
	{
	++num_el_pagine;
	el_pagine=(PAGINE_APERTE*)XtRealloc((char *)el_pagine,sizeof(PAGINE_APERTE)*num_el_pagine);
	i= num_el_pagine-1;
	}


if( num )
   el_pagine[i].w=XtParent(w[0]);
else
   el_pagine[i].w=NULL;



strcpy(el_pagine[i].nome,nome_pagina);

if(!el_pagine[i].w)  /* Se pagina senza widget evita creazione shm */
	return(1);

ret=OlAddPage(database_simulatore,nome_pagina,w,num,t_ref);
if(ret<0)
	{
	-- num_el_pagine;
	el_pagine[i].w=NULL;
	}

return(ret);
}

Boolean DelElencoPagine(int num, Widget w)
{
int i;
Cardinal num_children;
Boolean ret;
if(!w)
	{
	printf("\n Chiamata DelElencoPagine con w=0\n");
	return(True);
	}
/*
 Esamina se la pagina era vuota: in tal caso non 
 elimina la pagina dall'elenco perche' non presente
*/
XtVaGetValues(w,XmNnumChildren,&num_children,NULL);
if(!num_children)
	{
	printf("\n CANCELLAZIONE PAGINA VUOTA \n");
	return(True);
	}
for(i=0;i<num_el_pagine;i++)
	if(w==el_pagine[i].w)
		{
		el_pagine[i].w=NULL;
                strcpy(el_pagine[i].nome,"");
		break;
		}
/* Solo se esiste almeno un widget chiama la distruz. della pag. in shm */
if(w)
	ret=OlDelPage(database_simulatore,num);

return(ret);
}

Boolean DelElencoPagineTeleperm(int num, Widget w,char *nome)
{
int i;
Boolean ret;

for(i=0;i<num_el_pagine;i++)
{
   if( !strcmp(nome,el_pagine[i].nome) )
   {
      strcpy(el_pagine[i].nome,"");
      break;
   }
}


/* Solo se esiste almeno un widget chiama la distruz. della pag. in shm */
if(w)
	ret=OlDelPage(database_simulatore,num);

return(ret);
}



Boolean PreparaTestoInfo(Widget w, char *testo)
{
char riga[100];
char opzione[250];
XlConfInfo *confinfo;
char *riga_app, *token;
int num_confinfo,i,k,j;
float app_float;
int app_int,tipo;
XlCoreClassRec * wxlcoreclass;
XlManagerClassRec * wxlmanagerclass;


testo[0]=0;

sprintf(riga,"Object name: %s\nObject class: %s\n\n",
		XtName(w),XlGetClassName(XtClass(w)));
strcat(testo,riga);
if(XlIsXlCore(w))
	{
	wxlcoreclass=(XlCoreClassRec*)XtClass(w);
	confinfo=wxlcoreclass->xlcore_class.confinfo;
	num_confinfo=wxlcoreclass->xlcore_class.num_confinfo;
	}
else
	{
	wxlmanagerclass=(XlManagerClassRec*)XtClass(w);
	confinfo=wxlmanagerclass->xlmanager_class.confinfo;
	num_confinfo=wxlmanagerclass->xlmanager_class.num_confinfo;
	}
for(i=0;i<num_confinfo;i++)
	{
	if((confinfo[i].confinfo_group==XlRGrIo)||
		(confinfo[i].confinfo_group==XlRGrVCC1)||
		(confinfo[i].confinfo_group==XlRGrVCC2)||
		(confinfo[i].confinfo_group==XlRGrVCC3)||
		(confinfo[i].confinfo_group==XlRGrVCC4))
	  {
	  tipo=confinfo[i].confinfo_type;
	  if(tipo>LOCKED_PAG && tipo < LOCKED_LIB) tipo -= LOCKED_PAG;
	  if(tipo>LOCKED_LIB && tipo < LOCKED_PAG_LIB) tipo -= LOCKED_LIB;
	  if(tipo>LOCKED_PAG_LIB) tipo -= LOCKED_PAG_LIB;
          switch(tipo){
		case XlRTipoVarInp:
			{
			get_something(w,confinfo[i].resource_name, (void*) &riga_app);
	  	   	sprintf(riga,"%s = %s\n",
			   confinfo[i].confinfo_name,riga_app);
	  	   	strcat(testo,riga);
			break;
			}
		case XlRTipoVarOut:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &riga_app);
	  	   	sprintf(riga,"%s = %s\n",
			   confinfo[i].confinfo_name,riga_app);
	  	   	strcat(testo,riga);
			break;
			}
		case XlRGerarchia:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &riga_app);
	  	   	sprintf(riga,"%s = %s\n",
			   confinfo[i].confinfo_name,riga_app);
			printf("\n XLRGERARCHIA:%s\n",testo);
	  	   	strcat(testo,riga);
			break;
			}
		case XlRTipoVarReg:
		case XlRTipoVarPort:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &riga_app);
	  	   	sprintf(riga,"%s = %s\n",
			   confinfo[i].confinfo_name,riga_app);
	  	   	strcat(testo,riga);
			break;
			}
		case XlRText:
		case XlRFile:
		case XlRComponent:
		case XlRSubSystem:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &riga_app);
	  	   	sprintf(riga,"%s = %s\n",
			   confinfo[i].confinfo_name,riga_app);
	  	   	strcat(testo,riga);
			break;
			}
		case XlRFloat:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &app_float);
	  	   	sprintf(riga,"%s = %f\n",
			   confinfo[i].confinfo_name,app_float);
	  	   	strcat(testo,riga);
			break;
			}
		case XlRInt:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &app_int);
	  	   	sprintf(riga,"%s = %d\n",
			   confinfo[i].confinfo_name,app_int);
	  	   	strcat(testo,riga);
			break;
			}
		case XlROption:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &app_int);
			strcpy(opzione,confinfo[i].enum_options);
			token=strtok(opzione,",");
			for(k=0;k<app_int;k++)
				token=strtok(NULL,",");
	  	   	sprintf(riga,"%s = [%d] %s\n",
			   confinfo[i].confinfo_name,app_int,token);
	  	   	strcat(testo,riga);
			break;
			}
			
		}
          }
	}
}
/*
Funzione che scrive su file testo info
*/
Boolean ScriviTestoInfo(Widget w, FILE *fp )
{
char opzione[250];
XlConfInfo *confinfo;
char *riga_app, *token;
int num_confinfo,i,k,j;
float app_float;
int app_int,tipo;
XlCoreClassRec * wxlcoreclass;
XlManagerClassRec * wxlmanagerclass;



fprintf(fp,"Object name: %s\nObject class: %s\n\n",
		XtName(w),XlGetClassName(XtClass(w)));
if(XlIsXlCore(w))
	{
	wxlcoreclass=(XlCoreClassRec*)XtClass(w);
	confinfo=wxlcoreclass->xlcore_class.confinfo;
	num_confinfo=wxlcoreclass->xlcore_class.num_confinfo;
	}
else
	{
	wxlmanagerclass=(XlManagerClassRec*)XtClass(w);
	confinfo=wxlmanagerclass->xlmanager_class.confinfo;
	num_confinfo=wxlmanagerclass->xlmanager_class.num_confinfo;
	}
for(i=0;i<num_confinfo;i++)
	{
	if((confinfo[i].confinfo_group==XlRGrIo)||
		(confinfo[i].confinfo_group==XlRGrVCC1)||
		(confinfo[i].confinfo_group==XlRGrVCC2)||
		(confinfo[i].confinfo_group==XlRGrVCC3)||
		(confinfo[i].confinfo_group==XlRGrVCC4))
	  {
	  tipo=confinfo[i].confinfo_type;
	  if(tipo>LOCKED_PAG && tipo < LOCKED_LIB) tipo -= LOCKED_PAG;
	  if(tipo>LOCKED_LIB && tipo < LOCKED_PAG_LIB) tipo -= LOCKED_LIB;
	  if(tipo>LOCKED_PAG_LIB) tipo -= LOCKED_PAG_LIB;
          switch(tipo){
		case XlRTipoVarInp:
			{
			get_something(w,confinfo[i].resource_name, (void*) &riga_app);
	  	   	fprintf(fp,"%s = %s\n",
			   confinfo[i].confinfo_name,riga_app);
			break;
			}
		case XlRTipoVarOut:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &riga_app);
	  	   	fprintf(fp,"%s = %s\n",
			   confinfo[i].confinfo_name,riga_app);
			break;
			}
		case XlRGerarchia:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &riga_app);
	  	   	fprintf(fp,"%s = %s\n",
			   confinfo[i].confinfo_name,riga_app);
			break;
			}
		case XlRTipoVarReg:
		case XlRTipoVarPort:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &riga_app);
	  	   	fprintf(fp,"%s = %s\n",
			   confinfo[i].confinfo_name,riga_app);
			break;
			}
		case XlRText:
		case XlRFile:
		case XlRComponent:
		case XlRSubSystem:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &riga_app);
	  	   	fprintf(fp,"%s = %s\n",
			   confinfo[i].confinfo_name,riga_app);
			break;
			}
		case XlRFloat:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &app_float);
	  	   	fprintf(fp,"%s = %f\n",
			   confinfo[i].confinfo_name,app_float);
			break;
			}
		case XlRInt:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &app_int);
	  	   	fprintf(fp,"%s = %d\n",
			   confinfo[i].confinfo_name,app_int);
			break;
			}
		case XlROption:
			{
			get_something(w,confinfo[i].resource_name,
				(void*) &app_int);
			strcpy(opzione,confinfo[i].enum_options);
			token=strtok(opzione,",");
			for(k=0;k<app_int;k++)
				token=strtok(NULL,",");
	  	   	fprintf(fp,"%s = [%d] %s\n",
			   confinfo[i].confinfo_name,app_int,token);
			break;
			}
			
		}  /* end switch(tipo) */
          } /* end if su gruppo */
	}  /* end for num_confinfo */

}
Boolean LoadFont(char *fontname, XmFontList *font_list, Display *display)
{
char   *font_name_default = "fixed";
XFontStruct *font_info;

/* Carica il font ottenendo la descrizione del font stesso */
    if ((font_info = FontFallbackLoad (display, fontname)) == NULL)
    {
        fprintf (stderr,"Cannot open font [%s]\n",fontname);
	if ((font_info = XLoadQueryFont (display, font_name_default)) == NULL)
		return(False);
    }
*font_list=XmFontListCreate(font_info, XmSTRING_DEFAULT_CHARSET);
return(True);
}

Boolean FreeFont(XmFontList font_list, Display *display)
{
XmFontContext context;
XFontStruct *font;

return(True);
XmFontListFree(font_list);

/*
if(!XmFontListInitFontContext(&context,font_list))
	return(False);
*/

if(!XmFontListGetNextFont(context,(char**)XmSTRING_DEFAULT_CHARSET,&font))
	return(False);

XFreeFont(display,font);

XmFontListFreeFontContext(context);

XmFontListFree(font_list);

return(True);
}

Boolean PrintAllarmi(Widget cw)
{

Boolean apertura_ok;
Position x,y;
char comando[255];
char opzioni[355];

x=XlPrintOffsetX(cw);
y=XlPrintOffsetY(cw);


#if defined XPRINTER_USED && !defined LINUX
        apertura_ok=XlPrintOpen(cw);

strcpy(opzioni,"");
if( getenv("PRINTALLARM_OPTION") != NULL )
   strcpy(opzioni,getenv("PRINTALLARM_OPTION"));




sprintf(comando,"xwd -out PrintAll.img -root");
system(comando);
sprintf(comando,"xpr %s -output PrintAll.ps PrintAll.img",opzioni);
system(comando);
printf("Comando di xpr = %s\n\n",comando);
sprintf(comando,"lpr PrintAll.ps");
system(comando);
sprintf(comando,"rm PrintAll.img PrintAll.ps");
system(comando);


        XlPrintClose();
        XBell(XtDisplay(cw),40);
        return(True);
#else
return(False);
#endif

}



Boolean PrintScreen(Widget cw, char* nomeDisp, char * file, Boolean color,
	BCK_OBJ *sfondo)
{
Window win;
char comando[100];
Pixel old,white;
XmDrawingAreaCallbackStruct str;
int width,height;
GC gc;
XGCValues values;
unsigned long valuemask=0;


#if defined XPRINTER_USED && !defined LINUX
	if( !XlPrintOpen(cw) )
        {
           fprintf(stderr,"Printer not available\n");
           return(False);
        }
        disegna_bck(cw,sfondo,NULL,NULL,True);
        XlPrint(cw);
        XlPrintClose();
	XBell(XtDisplay(cw),40);
	return(True);
#else
return(False);
#endif

/***********************
if(!color)
	{
	if(file==NULL)
		sprintf(comando,"xwd  -display %s:0.0 | xpr -device ps | lpr",				nomeDisp);
	else
		sprintf(comando,"xwd -display %s:0.0 | xpr -device ps > %s.ps",
			nomeDisp,file);
	system(comando);
	}
*********************/
}

Widget PrintSetup(Widget cw)
{
#if defined XPRINTER_USED && !defined LINUX
return(XlPrintSetup(cw));
#else
return(NULL);
#endif
}


void show_warning(mess)
char *mess;
{
  char   *str, *new_str;
  extern Widget MessageText;
  extern FILE *fpLog;

  if(fpLog!=NULL)
	fprintf(fpLog,"%s",mess);

  str = XmTextGetString( MessageText );
  RtRecord((void*)str);
  new_str = (char*) malloc ( sizeof(char*) * ( strlen(mess)+strlen(str)+1 ) );
  strcpy( new_str, str );

  strcat( new_str, mess );
  XmTextSetString( MessageText, new_str );
  XmTextShowPosition(MessageText,strlen(new_str)-1);
  free( str );
  free( new_str );
}

void SalvaContextBin()
{
int i,j;
FILE *fp;
int num_b;
int size;
char nome_file[150];



sprintf(nome_file,"%s_rtf",nome_file_context);


if((fp=fopen(nome_file,"w"))==0)
	return;

/*
	scrivo il numero delle pagine
*/
fwrite(&totale_pagine,sizeof(int),1,fp);

for(i=0;i<totale_pagine;i++)
	{
	/*
		calcolo il numero degli oggetti grafici dello sfondo
	*/
	num_b=0;
	if(pagine[i].sfondo!=NULL)
		{
		num_b=1;
		while(pagine[i].sfondo[num_b-1].tipo>0)
			{
			++num_b;
			}
		}
	size=L_NOME_PAGINE+L_DESCR_PAGINE+L_DESCR_TIPO+LUN_GERAR+
		sizeof(int)+num_b*sizeof(BCK_OBJ);
	fwrite(&size,sizeof(int),1,fp);
	fwrite(pagine[i].nome,1,L_NOME_PAGINE,fp);
	fwrite(pagine[i].descrizione,1,L_DESCR_PAGINE,fp);
	fwrite(pagine[i].tipo,1,L_DESCR_TIPO,fp);
	fwrite(pagine[i].gerarchia,1,LUN_GERAR,fp);	/* CAPPE OlTree */
	fwrite(&pagine[i].t_refresh,1,sizeof(int),fp);
	if(num_b!=0)
	   {
	   for(j=0;j<num_b;j++)
		{
		fwrite(&(pagine[i].sfondo[j]),sizeof(BCK_OBJ),1,fp);
		if((pagine[i].sfondo[j].tipo==TIPO_POLIGONO)||
		    (pagine[i].sfondo[j].tipo==TIPO_CONNESSIONE))
		  fwrite(pagine[i].sfondo[j].points,sizeof(XPoint),pagine[i].sfondo[j].dati[0],fp);
		}
	   }
	}
fclose(fp);
}


Boolean CaricaContextBin()
{
FILE *fp;
int i,j,k;
int num_b;
int size;
char nome_file[FILENAME_MAX];
char nome_pagina[FILENAME_MAX];
Boolean context_ok;            /* Variabile per verificare se il context
                                  non va rifatto per DEBUG */
time_t time_start,time_end;

context_ok=0;
time(&time_start);
sprintf(nome_file,"%s_rtf",nome_file_context);

if(XlGetenv("NOCONTEXTRTF"))
   {
   printf("NOCONTEXTRTF=%s\n",XlGetenv("NOCONTEXTRTF"));
   if(!strcmp(XlGetenv("NOCONTEXTRTF"),"YES"))
      context_ok=1;
   }
if((fp=fopen(nome_file,"r"))==0)
	return(False);
/*
	verifica le date dei file ascii e del file binario
*/
if(tempo_file(nome_file,nome_file_context)==0)
	{
        printf("File Context.ctx_rtf da rifare !!!!\n");
        if(!context_ok)
	   return(False);
	}


/*
        leggo il numero delle pagine
*/
fread(&totale_pagine,sizeof(int),1,fp);
/*
	alloco lo spazio delle pagine
*/
pagine=(AN_PAG *)XtCalloc((totale_pagine+1),sizeof(AN_PAG));
for(i=0;i<totale_pagine;i++)
        {
	fread(&size,sizeof(int),1,fp);
	fread(pagine[i].nome,1,L_NOME_PAGINE,fp);
        fread(pagine[i].descrizione,1,L_DESCR_PAGINE,fp);
        fread(pagine[i].tipo,1,L_DESCR_TIPO,fp);
	fread(pagine[i].gerarchia,1,LUN_GERAR,fp);
        fread(&pagine[i].t_refresh,1,sizeof(int),fp);
        /*
		verifica che il background delle pagine 
		non siano stati modificati
	*/
	sprintf(nome_pagina,"%s/%s.bkg",XlGetenv("LEGOMMI_PAGINE"),pagine[i].nome);
        if(!context_ok)
	   if(tempo_file(nome_file,nome_pagina)==0)
		{
		XtFree((char*)pagine);
		fclose(fp);
		totale_pagine=0;
                printf("WARNING pag=%s aggiornata !!! \n",nome_pagina);
		return(False);
		}
	num_b=(size-L_NOME_PAGINE-L_DESCR_PAGINE-L_DESCR_TIPO-LUN_GERAR)/sizeof(BCK_OBJ);
        if(num_b!=0)
	   {
	   pagine[i].sfondo=(BCK_OBJ*)XtCalloc(num_b,sizeof(BCK_OBJ));
	   for(j=0;j<num_b;j++)
	      {
	      fread(&(pagine[i].sfondo[j]),sizeof(BCK_OBJ),1,fp);
	      if((pagine[i].sfondo[j].tipo==TIPO_POLIGONO)||
			(pagine[i].sfondo[j].tipo==TIPO_CONNESSIONE))
		{
		(pagine[i].sfondo[j].points)=
                                (XPoint *)XtCalloc(pagine[i].sfondo[j].dati[0],sizeof(XPoint));
		fread(pagine[i].sfondo[j].points,sizeof(XPoint),pagine[i].sfondo[j].dati[0],fp);
		}
	      }
           }
	}
time(&time_end);
printf("\n\nDEBUG: tempo per caricare il Context=%ld\n",(time_end - time_start));
return(True);
}


Widget ToTopLevel(Widget parent)
{
Widget wid;

while(!XtIsVendorShell(parent)&&(wid=XtParent(parent)))
                     parent=wid;
return(parent);
}


void AllocaColoriSfondo(w,sfondo)
Widget w;
BCK_OBJ *sfondo;
{
int num_b;
Colormap colmap;
XColor xcol;


/*
	colmap = DefaultColormap ( XtDisplay(w), DefaultScreen (XtDisplay(w)) );
*/
	colmap = DefaultColormapOfScreen ( XtScreen(w) );
	num_b=0;
	while(sfondo[num_b].tipo>0)
		{
		xcol.pixel=0;
		xcol.flags= DoRed | DoGreen | DoBlue;
		xcol.pad=0;
		xcol.red=sfondo[num_b].fg_c_red;
		xcol.green=sfondo[num_b].fg_c_green;
		xcol.blue=sfondo[num_b].fg_c_blue;
		if(!XAllocColor(XtDisplay(w),colmap,&xcol))
                {
			fprintf(stderr,"Error in XAllocColor\n");
		        sfondo[num_b].fg_c=BlackPixel(XtDisplay(w),DefaultScreen(XtDisplay(w)));
                }
                else
		   sfondo[num_b].fg_c=xcol.pixel;
		xcol.pixel=0;
		xcol.flags= DoRed | DoGreen | DoBlue;
		xcol.pad=0;
		xcol.red=sfondo[num_b].bg_c_red;
		xcol.green=sfondo[num_b].bg_c_green;
		xcol.blue=sfondo[num_b].bg_c_blue;
		if(!XAllocColor(XtDisplay(w),colmap,&xcol))
                {
			fprintf(stderr,"Error in XAllocColor\n");
                        sfondo[num_b].bg_c=BlackPixel(XtDisplay(w),DefaultScreen(XtDisplay(w)));
                }
                else
   		   sfondo[num_b].bg_c=xcol.pixel;
		xcol.pixel=0;
		xcol.flags= DoRed | DoGreen | DoBlue;
		xcol.pad=0;
		xcol.red=sfondo[num_b].fg_f_red;
		xcol.green=sfondo[num_b].fg_f_green;
		xcol.blue=sfondo[num_b].fg_f_blue;
		if(!XAllocColor(XtDisplay(w),colmap,&xcol))
                {
			fprintf(stderr,"Error in XAllocColor\n");
                        sfondo[num_b].fg_f=BlackPixel(XtDisplay(w),DefaultScreen(XtDisplay(w)));
                }
                else
		   sfondo[num_b].fg_f=xcol.pixel;
		++num_b;
		}
}


Boolean ActivateChangeVar(Widget w ,Widget drawing,
		Widget *ListaWid)
{
Arg args[2];
char *rigavar;
Widget ret;
int tipo_var=XlRVarXX;

/*
        ricavo:
                stringa di definizione della variabile
*/
rigavar=(char *)XlGetFirstVarGraf(w);

/*
        creo sette widget
*/
ListaWid[0]=XmCreateTextField(drawing,"_text__0",NULL,0);
ListaWid[1]=XmCreateTextField(drawing,"_text__1",NULL,0);
XtSetArg(args[0],XmNvalue,rigavar);
ListaWid[2]=XmCreateTextField(drawing,"_text__2",args,1);
ListaWid[3]=XmCreateTextField(drawing,"_text__3",NULL,0);
ListaWid[4]=XmCreateTextField(drawing,"_text__4",NULL,0);
ListaWid[5]=XmCreateTextField(drawing,"_text__5",NULL,0);
ListaWid[6]=XmCreateTextField(drawing,"_text__6",NULL,0);

ret=(Widget)create_VariableEditor(w,(Widget)ListaWid,drawing,&tipo_var,
        database_topologia);
XtManageChild(ret);

return(True);
}

Boolean ActivateAing(Widget w ,Widget drawing,char *nome_disp, 
		Widget *ListaWid)
{
Arg args[2];
char *componente, *subsystem, *rigavar;
Widget ret;
int tipo_var=XlRVarAing;
int x,y;
char pos_x[20];
char pos_y[20];
char *app_c1, *app_c2;
char *portName,*tagName,*portNameConnect,*varNameCambioColore;
Display *disp;
char *tipo;
XrmValue value;
char stringa[100];
char regSigla[5];
char nomeVar[10];
int free_input=1;

if(!PosMouse(w, &x, &y))
	{
	x=100;
	y=100;
	}

/*
	ricavo:
		nome_componente
		nome subsystem
		stringa di definizione della variabile
*/
componente=NULL;
subsystem=NULL;
get_something(w,XlNcomponent,(void*) &componente);
get_something(w,XlNsubsystem,(void*) &subsystem);
if(componente==NULL)
	componente=XtNewString("");
if(subsystem==NULL)
	subsystem=XtNewString("");
rigavar=(char *)XlGetFirstVcc(w);

/*
	creo sette widget
*/
XtSetArg(args[0],XmNvalue,componente);
ListaWid[0]=XmCreateTextField(drawing,"_text__0",args,1);
XtSetArg(args[0],XmNvalue,subsystem);
ListaWid[1]=XmCreateTextField(drawing,"_text__1",args,1);

if(OlVarIsFreeInput(database_topologia,rigavar))
	XtSetArg(args[0],XmNvalue,rigavar);
else
	{
        fprintf(stderr,"\n\n");
        fprintf(stderr,"WARNING:-> Input Selected not free !!!\n");
        fprintf(stderr,"\n\n");
        free_input=0;

	/*
		elimina dalla riga il nome della variabile in quanto
		non di ingresso libero
	*/
	app_c1=strstr(rigavar," ");
	if(app_c1!=NULL)
		for(app_c2=rigavar;app_c2!=app_c1;++app_c2)
			*app_c2='-';
	XtSetArg(args[0],XmNvalue,rigavar);
	}
ListaWid[2]=XmCreateTextField(drawing,"_text__2",args,1);
ListaWid[3]=XmCreateTextField(drawing,"_text__3",NULL,0);
sprintf(pos_x,"%d",x);
XtSetArg(args[0],XmNvalue,pos_x);
ListaWid[4]=XmCreateTextField(drawing,"_text__4",args,1);
sprintf(pos_y,"%d",y);
XtSetArg(args[0],XmNvalue,pos_y);
ListaWid[5]=XmCreateTextField(drawing,"_text__5",args,1);
XtSetArg(args[0],XmNvalue,nome_disp);
ListaWid[6]=XmCreateTextField(drawing,"_text__6",args,1);


/*
Verifiche su oggetti regolazione
*/
if(XlIsPort(w))
   {

   XtVaGetValues(w,XlNportName,&portName,NULL);
   if(!strcmp(portName,"--"))
      {
      popup_errorDialog("Port without portName !!!",w);
      return(False);
      }


   /*
   Controllo che la porta non sia connessa
   */
   XtVaGetValues(w,XlNportNameConnect,&portNameConnect,NULL);
   strcpy(stringa,"");
   sscanf(portNameConnect,"%s",stringa);

   if(strlen(stringa))
      {
      fprintf(stderr,"WARNING: port Connected !!!\n");
      return False;
      }

   XtVaGetValues(XtParent(w),XlNtagName,&tagName,NULL);

   /*
   Ricavo sigla della Task
   */
   disp=XtDisplay(w);
   XrmGetResource(disp->db,"regTaskSigla",(char *) NULL,&tipo,&value);
   if( (value.addr == NULL) || (value.size == 0) )
        {
        fprintf(stderr,"Warning: cannnot open regTaskSigla for this page\n");
        return(False);
        }
   strncpy(regSigla,value.addr,(int)value.size);
   sprintf(nomeVar,"%s%s%s",portName,tagName,regSigla);
   /*
   Costruisco la stringa da passare ad Aing
   */
   strcpy(stringa,nomeVar);
   strcat(stringa," DUMMY ");
   strcat(stringa,"DUMMY ");
   strcat(stringa,"DUMMY ");
   strcat(stringa,"1.0 0.0 --");
/*
   printf("DEBUG:var=%s\n",stringa);
*/
   XmTextFieldSetString (ListaWid[3],stringa);
   ret=ActivateVarAing(ListaWid);
   }

else if(XlIsDispReg(w))
   {
   XtVaGetValues(w,XlNvarInputCambioColore1,&portName,NULL);
   if(!strcmp(portName,"--"))
      {
      popup_errorDialog("No varInputCambioColore1 set!!!",w);
      return(False);
      }


   XtVaGetValues(XtParent(w),XlNtagName,&tagName,NULL);

   /*
   Ricavo sigla della Task
   */
   disp=XtDisplay(w);
   XrmGetResource(disp->db,"regTaskSigla",(char *) NULL,&tipo,&value);
   if( (value.addr == NULL) || (value.size == 0) )
        {
        fprintf(stderr,"Warning: cannnot open regTaskSigla for this page\n");
        return(False);
        }
   strncpy(regSigla,value.addr,(int)value.size);
   sprintf(nomeVar,"%s%s%s",portName,tagName,regSigla);
   /*
   Costruisco la stringa da passare ad Aing
   */
   strcpy(stringa,nomeVar);
   strcat(stringa," DUMMY ");
   strcat(stringa,"DUMMY ");
   strcat(stringa,"DUMMY ");
   strcat(stringa,"1.0 0.0 --");
/*
   printf("DEBUG:var=%s\n",stringa);
*/
   XmTextFieldSetString (ListaWid[3],stringa);
   ret=ActivateVarAing(ListaWid);
   }
else if ( XlIsIndic(w) || XlIsIndicTelep(w) )
   {
   XtVaGetValues(w,XlNvarInputCambioColore1,&varNameCambioColore,NULL);
   if ( (varNameCambioColore!=NULL)&&(varNameCambioColore[0]!='\0') )
   sscanf(varNameCambioColore,"%s",nomeVar);


   /*
   Vado a ricavare scalamento e offset
   */
   strcpy(stringa,"");  

   /*
   Costruisco la stringa da passare ad Aing
   */
   strcpy(stringa,"");
   sprintf(stringa,"%s DUMMY DUMMY DUMMY 1.0 0.0 --",nomeVar);
/*
   printf("DEBUG:var=%s\n",stringa);
*/
   XmTextFieldSetString (ListaWid[3],stringa);
   ret=ActivateVarAing(ListaWid);
  

   }
else
   return False;

  /*
   Setto la var di Start ad insensitive se 
   input not free
  */

if(!free_input && ret)
   AingStartEnable(ret,0);

return(True);
}


Widget ActivateVarAing(Widget *wList)
{
static int id_msg_aing= -1;
extern int shr_usr_key;
int stato_aing;
char *proc_argv[5];
char *proc_envp[7];
char proc_name[100];
char *path;
int id_shm;
int i;
char nome_display[100];
Widget wid_ret=NULL;


strcpy(nome_display,XmTextFieldGetString(wList[6]));

for(i=0;i<numero_display;i++)
        {
	if(strcmp(nome_display,elenco_display[i].nome)==0)
			break;
        }
wid_ret=(Widget)create_Aing(XmTextFieldGetString(wList[3]),elenco_display[i].topLevel);

RaisePlotAing();

return wid_ret;
}


/*
	il nome di questa funzione e' falso e tendenzioso; verr� cambiato in
	seguito.
	Lo scopo di questa funzione � ququello di terminare il processo padre
	dell'mmi prima dell'uscita corretta


Pero' non si capisce a cosa serva 
by Fabio
*/
void StopAing()
{
int pid;

ripulisci_files();
}

Boolean PosMouse(Widget w, int *x, int *y)
{
Window root_return, child_return;
int win_x_return, win_y_return;
unsigned int mask_return;


return(XQueryPointer(XtDisplay(w), XtWindow(w), &root_return, &child_return,
	x,y,&win_x_return,&win_y_return,&mask_return));

}

/*
	chiamata nulla per connessione con il VariableEditor
*/
int EseguiLoadResources()
{
}

/*
	Chiude tutte le pagine aperte
*/
Boolean CloseAllPages(AN_DISPLAY *elenco_display, int num_display)
{
int i,j;


for(i=0;i<num_el_pagine;i++)
	{
	if(el_pagine[i].w!=NULL)
	{
	   if(strcmp(XtName(el_pagine[i].w),DRAWING_SIN)==0)
	      {
	      ClosePageSin(el_pagine[i].w);
	      continue;
	      }
	   if(strcmp(XtName(el_pagine[i].w),DRAWING_STAZ)==0)
	      {
	      ClosePageStaz(el_pagine[i].w);
	      continue;
	      }
	   if(strcmp(XtName(el_pagine[i].w),DRAWING_AING)==0)
	      {
	      DestroyAing(el_pagine[i].w);
	      continue;
	      }
	   if(strcmp(XtName(el_pagine[i].w),DRAWING_TELE)==0)
	      {
              telep_chiudi_pag( el_pagine[i].w );
	      continue;
	      }
	}
	}

return(True);
}

int SettaIndicAing(Widget w, char *tag,float a, float b)
{
XlIndicWidget curcw = (XlIndicWidget)w;
int tipo,ind;


if(!OlTagToInd(database_topologia,tag,&ind,&tipo))
   {
   popup_errorDialog("Variable not found !!!",w);
   fprintf(stderr,"ERROR:variable =%s not found in database\n",tag);
   return -1;
   }
/*
printf("DEBUG:AING: indice=%d tipo=%d\n",ind,tipo);
*/

curcw->indic.cambio_colore[0].input.pdb.tipo=tipo;
curcw->indic.cambio_colore[0].input.pdb.indice=ind;
curcw->indic.cambio_colore[0].input.unimis.a=a;
curcw->indic.cambio_colore[0].input.unimis.b=b;
return(ind);
}

Boolean RaisePlotAing()
{
int i,j;
Widget parent,wid;

for(i=0;i<num_el_pagine;i++)
	{
          if(el_pagine[i].w != NULL)
          {
	  if((strcasecmp(el_pagine[i].nome,"__PLOT__")==0)||
	     (strcasecmp(el_pagine[i].nome,"__AING__")==0))
		  {
		  wid=el_pagine[i].w;
		  parent=el_pagine[i].w;
		  while(!XtIsVendorShell(parent)&&(wid=XtParent(parent)))
				  parent=wid;
		  XRaiseWindow( XtDisplay(parent),XtWindow(parent));
		  }
	  }
	}
return(True);
}

Boolean CanClosePage(char *nome)
{
int j;


for(j=0;j<num_noclose_pages;j++)
	  if(strcasecmp(noclose_pages[j],nome)==0)
		{
		return(False);
		}
return(True);
}

Boolean PaginaAperta(Widget w,char* nome)
{
int i,j;
Widget parent,wid;

for(i=0;i<num_el_pagine;i++)
{
	if(el_pagine[i].w!=NULL)
	{
	if((XtDisplay(el_pagine[i].w)==XtDisplay(w)) &&
		(strcasecmp(el_pagine[i].nome,nome)==0))
			{
			wid=el_pagine[i].w;
			parent=el_pagine[i].w;
			while(!XtIsVendorShell(parent)&&(wid=XtParent(parent)))
					parent=wid;
			for(j=0;j<num_background_pages;j++)
			  if(strcasecmp(background_pages[j],nome)==0)
				return(True);
			XRaiseWindow( XtDisplay(parent),XtWindow(parent));
			return(True);
			}
	}
}
return(False);
}

int NumPagineAperte()
{
int num = 0;
int i;

for(i=0;i<num_el_pagine;i++)
	if(el_pagine[i].w!=NULL)
		++num;
return(num);
}

Boolean CanOpenPage(Widget w)
{
extern OlDatabasePuntiObject database_simulatore;

if(database_simulatore==NULL)
	{
#if defined EXIT_ON_SEVERE_ERROR
	ripulisci_files();
	exit(1);
#endif
        popup_errorDialog("Point database not found",w);
        return(False);
        }
if(NumPagineAperte()<MAX_PAGE)
	return(True);
else
	{
	popup_errorDialog("Too many pages open",w);
	return(False);
	}
}

void CheckMmiStatus(caddr_t client_data,XtIntervalId * id)
{
XtIntervalId timer_id;
int          ret;
double       tempo_lic_corr;

/*
 *   Si controlla se la licenza e' ancora disponibile 
 */
ret = getTimeLicense(&tempo_lic_corr);
if(ret == 0) {
   if(tempo_lic_corr > tempo_license) {
/*
 *   Scaduta disponibilita' licenza.
 */
      printf("###################################################\n");
      printf("###################################################\n");
      printf("#                                                 #\n");
      printf("#            MMI LICENSE IS EXPIRED!!!            #\n");
      printf("#                                                 #\n");
      printf("###################################################\n");
      printf("###################################################\n");
      quit_proc();

   }
}
/*
	verifica lo stato della memoria
*/
if(!RtCheckPointer(NULL))
{
printf("!!! ATTENZIONE SPORCAMENTO IN MEMORIA\n");
printf("Totale memoria allocata = %d\n",RtMemoryAllocated(NULL));
}
	if(database_simulatore!=NULL)
	{
// printf("CheckMmiStatus: database_simulatore=%d \n",database_simulatore);
	   if(!OlCheckDatabasePuntiStatus(database_simulatore))
		{
                errori_database_simulatore++;
                if ( errori_database_simulatore > mmiMaxConErr)
                   {
		   database_simulatore=NULL;
#if defined EXIT_ON_SEVERE_ERROR
		ripulisci_files();
		exit(1);
#endif
        	popup_errorDialog("Errore definitivo in point database (max errori consecutivi)",
			(Widget)client_data);
                   }
                else
                   {
                   printf("CheckMmiStatus:Errore singolo numero %d in point database, proseguo fino a 3 \n", errori_database_simulatore);
                   }
		}
            else
               {
               errori_database_simulatore=0;
               }
	}
	timer_id = XtAppAddTimeOut (
            XtWidgetToApplicationContext ((Widget)client_data),
            (unsigned long) 1000, (XtTimerCallbackProc)CheckMmiStatus, (Widget)client_data);
}

void read_argv(int argc, char **argv)
{
   int i;

   num_startup_pages = 0;

   printf("Paramentri in ingresso");
   for(i=0;i<argc;i++)
   {
      printf("Paramentro [%d]=[%s]\n",i,argv[i]);
      if(strcmp(argv[i],ARGV_CONTEXT)==0)
      {
         i++;
         printf("Paramentro Context [%s]\n",argv[i]);
         XtFree(nome_file_context);
         if( (nome_file_context=XtMalloc(strlen(argv[i])+1)) == NULL)
         {
            printf("read_argv. Error in malloc of nome_file_context. Exit\n");
            exit(1);
         }

         strcpy(nome_file_context,argv[i]);
	 continue;
      }
      if(strcmp(argv[i],ARGV_PAG)==0)
      {
	i++;
	printf("Paramentro pag [%s]\n",argv[i]);
	if(num_startup_pages<MAX_STARTUP_PAGES)
	{
           	     strcpy(startup_pages[num_startup_pages],argv[i]);
		     ++num_startup_pages;
	}
      continue;
      }
	if(strcmp(argv[i],ARGV_TOPOLOGIA)==0)
                        {
                        i++;
                        printf("Paramentro Topologia [%s]\n",argv[i]);
			topologia_on=True;
                        continue;
                        }
	if(strcmp(argv[i],ARGV_DEMONE_PORT)==0)
                        {
                        i++;
			port_daemon=atoi(argv[i]);
                        printf("Paramentro Porta daemon [%d]\n",port_daemon);
                        continue;
                        }

      if(strcmp(argv[i],ARGV_BACKGROUND)==0)
      {
	i++;
	printf("Paramentro pagback [%s]\n",argv[i]);
	if(num_background_pages<MAX_BACKGROUND_PAGES)
	{
           strcpy(background_pages[num_background_pages],argv[i]);
				++num_background_pages;
	}
	continue;
      }
      if(strcmp(argv[i],ARGV_NOCLOSE)==0)
      {
        i++;
        printf("Paramentro noclose [%s]\n",argv[i]);
	if(num_noclose_pages<MAX_NOCLOSE_PAGES)
	{
            strcpy(noclose_pages[num_noclose_pages],argv[i]);
				++num_noclose_pages;
	}
	continue;
      }

      /* invio pagina di default su tutti i display */
      if(strcmp(argv[i],ARGV_ALLDISP)==0)
      {
        alldisp=True;
        i++;
	
        strcpy(startup_pages[num_startup_pages],argv[i]);
        ++num_startup_pages;
        printf("Paramentro alldisp [%s]\n",argv[i]);
        continue;
      }

   }
}


Boolean PutPagBackground()
{
int i,j;
Widget parent,wid;
extern Widget MainInterface;

for(i=0;i<num_el_pagine;i++)
        if(el_pagine[i].w!=NULL)
        {
        for(j=0;j<num_background_pages;j++)
                if(strcasecmp(background_pages[j],el_pagine[i].nome)==0)
                {
                wid=el_pagine[i].w;
                parent=el_pagine[i].w;
                while(!XtIsVendorShell(parent)&&(wid=XtParent(parent)))
                        parent=wid;
                XLowerWindow( XtDisplay(parent),XtWindow(parent));

                }
        XLowerWindow( XtDisplay(MainInterface),XtWindow(MainInterface));
        }

return(False);
}

void ripulisci_files()
{
#if defined VMS
	system("delete/nolog/noconf ARCHIV*.dat;*");
	system("purge/nolog *.log");
#else
	system("rm -f Archiv*.dat");
#endif
}


Boolean AcceptAing(Widget w)
{
/* 
Verifico che la Topologia sia settata
*/
if(!topologia_on)
   return False;
if(XlIsAnimateIcon(w))
	return(True);
if(XlIsIconReg(w) && !XlIsInterfaceIconReg(w))
	return(True);
if(XlIsIndic(w))
	return(True);
if(XlIsPort(w))
	return(True);
if(XlIsDispReg(w))
	return(True);

return(False);
}

Boolean AcceptPlot(Widget w)
{

if(XlIsAnimateIcon(w))
	return(True);
if(XlIsIconReg(w) && !XlIsInterfaceIconReg(w))
	return(True);
if(XlIsIndic(w))
	return(True);
if(XlIsDispReg(w))
	return(True);
if(XlIsPort(w))
	return(True);

return(False);
}


Boolean ActivatePlot(Widget w ,Widget drawing,char *nome_disp, 
		Widget *ListaWid)
{
Arg args[2];
char *componente, *subsystem, *rigavar;
Widget ret;
int tipo_var=XlRVarPlot;
int x,y;
char pos_x[20];
char pos_y[20];
char *portName,*tagName,*portNameConnect,*varNameCambioColore;
float scalamento, offset;
Display *disp;
char *tipo;
XrmValue value;
char stringa[100];
char offset_new[50],scalamento_new[50];
char regSigla[5];
char nomeVar[10];


if(!PosMouse(w, &x, &y))
	{
	x=100;
	y=100;
	}

/*
	ricavo:
		nome_componente
		nome subsystem
		stringa di definizione della variabile
*/
componente=NULL;
subsystem=NULL;
get_something(w,XlNcomponent,(void*) &componente);
get_something(w,XlNsubsystem,(void*) &subsystem);
if(componente==NULL)
	componente=XtNewString("");
if(subsystem==NULL)
	subsystem=XtNewString("");
rigavar=(char *)XlGetFirstVcc(w);

/*
	creo sette widget
*/
XtSetArg(args[0],XmNvalue,componente);
ListaWid[0]=XmCreateTextField(drawing,"_text__0",args,1);
XtSetArg(args[0],XmNvalue,subsystem);
ListaWid[1]=XmCreateTextField(drawing,"_text__1",args,1);
XtSetArg(args[0],XmNvalue,rigavar);
ListaWid[2]=XmCreateTextField(drawing,"_text__2",args,1);
ListaWid[3]=XmCreateTextField(drawing,"_text__3",NULL,0);
sprintf(pos_x,"%d",x);
XtSetArg(args[0],XmNvalue,pos_x);
ListaWid[4]=XmCreateTextField(drawing,"_text__4",args,1);
sprintf(pos_y,"%d",y);
XtSetArg(args[0],XmNvalue,pos_y);
ListaWid[5]=XmCreateTextField(drawing,"_text__5",args,1);
XtSetArg(args[0],XmNvalue,nome_disp);
ListaWid[6]=XmCreateTextField(drawing,"_text__6",args,1);


/*
Verifiche su oggetti regolazione
*/
if(XlIsPort(w))
   {

   XtVaGetValues(w,XlNportName,&portName,NULL);
   if(!strcmp(portName,"--"))
      {
      popup_errorDialog("Port without portName !!!",w);
      return(False);
      }

   XtVaGetValues(XtParent(w),XlNtagName,&tagName,NULL);

   /*
   Ricavo sigla della Task
   */
   disp=XtDisplay(w);
   XrmGetResource(disp->db,"regTaskSigla",(char *) NULL,&tipo,&value);
   if( (value.addr == NULL) || (value.size == 0) )
        {
        popup_errorDialog("WARNING:No regTaskSigla for this page!!!",w);
        fprintf(stderr,"Warning: cannnot open regTaskSigla for this page\n");
        return(False);
        }
   strncpy(regSigla,value.addr,(int)value.size);
   sprintf(nomeVar,"%s%s%s",portName,tagName,regSigla);
   /*
   Costruisco la stringa da passare ad Aing
   */
   strcpy(stringa,nomeVar);
   strcat(stringa," DUMMY ");
   strcat(stringa,"DUMMY ");
   strcat(stringa,"DUMMY ");
   strcat(stringa,"1.0 0.0 --");
/*
   printf("DEBUG:var=%s\n",stringa);
*/
   XmTextFieldSetString (ListaWid[3],stringa);
   ActivateVarPlot(ListaWid);
   }

else if(XlIsDispReg(w))
   {
   XtVaGetValues(w,XlNvarInputCambioColore1,&portName,NULL);
   if(!strcmp(portName,"--"))
      {
      popup_errorDialog("No varInputCambioColore1 set!!!",w);
      return(False);
      }


   XtVaGetValues(XtParent(w),XlNtagName,&tagName,NULL);

   /*
   Ricavo sigla della Task
   */
   disp=XtDisplay(w);
   XrmGetResource(disp->db,"regTaskSigla",(char *) NULL,&tipo,&value);
   if( (value.addr == NULL) || (value.size == 0) )
        {
        popup_errorDialog("WARNING:No regTaskSigla for this page!!!",w);
        fprintf(stderr,"Warning: cannnot open regTaskSigla for this page\n");
        return(False);
        }
   strncpy(regSigla,value.addr,(int)value.size);
   sprintf(nomeVar,"%s%s%s",portName,tagName,regSigla);



   /*
   Vado a ricavare scalamento e offset
   */
   get_something(w,XlNoffset,(void*) &offset);
   strcpy(stringa,"");  
   sprintf(stringa,"%f",offset);
   if(!strlen(stringa))
      strcpy(offset_new,"0.0");
   else 
      strcpy(offset_new,stringa);



   get_something(w,XlNscalamento,(void*) &scalamento);
   strcpy(stringa,"");  

   sprintf(stringa,"%f",scalamento);
   if(!strlen(stringa))
      strcpy(scalamento_new,"1.0");
   else 
      strcpy(scalamento_new,stringa);

   /*
   Costruisco la stringa da passare ad Aing
   */
   strcpy(stringa,"");
   sprintf(stringa,"%s DUMMY DUMMY DUMMY %s %s --",nomeVar,scalamento_new,offset_new);
/*
   printf("DEBUG:var=%s\n",stringa);
*/
   XmTextFieldSetString (ListaWid[3],stringa);
   ActivateVarPlot(ListaWid);
   }

else if ( XlIsIndic(w) || XlIsIndicTelep(w) )
   {
   XtVaGetValues(w,XlNvarInputCambioColore1,&varNameCambioColore,NULL);
   if ( (varNameCambioColore!=NULL)&&(varNameCambioColore[0]!='\0') )
   sscanf(varNameCambioColore,"%s",nomeVar);


   /*
   Ricavo sigla della Task
   */



   /*
   Vado a ricavare scalamento e offset
   */
   get_something(w,XlNoffset,(void*) &offset);
   strcpy(stringa,"");  
   sprintf(stringa,"%f",offset);
   if(!strlen(stringa))
      strcpy(offset_new,"0.0");
   else 
      strcpy(offset_new,stringa);



   get_something(w,XlNscalamento,(void*) &scalamento);
   strcpy(stringa,"");  

   sprintf(stringa,"%f",scalamento);
   if(!strlen(stringa))
      strcpy(scalamento_new,"1.0");
   else 
      strcpy(scalamento_new,stringa);

   /*
   Costruisco la stringa da passare ad Aing
   */
   strcpy(stringa,"");
   sprintf(stringa,"%s DUMMY DUMMY DUMMY %s %s --",nomeVar,scalamento_new,offset_new);
/*
   printf("DEBUG:var=%s\n",stringa);
*/
   XmTextFieldSetString (ListaWid[3],stringa);
   ActivateVarPlot(ListaWid);
  
   }
else
   return False;

return(True);
}

void ActivateVarPlot(Widget *wList)
{
static int id_msg_aing= -1;
extern int shr_usr_key;
int stato_aing;
char *proc_argv[5];
char *proc_envp[7];
char proc_name[100];
char *path;
int id_shm;
int i;
char nome_display[100];


strcpy(nome_display,XmTextFieldGetString(wList[6]));

for(i=0;i<numero_display;i++)
	if(strcmp(nome_display,elenco_display[i].nome)==0)
			break;
popup_Plot(XmTextFieldGetString(wList[3]),elenco_display[i].topLevel);

RaisePlotAing();

return;
}

int SettaGrafPlot(Widget w, char *tag,float a, float b)
{
int tipo,ind,mod;
char app_tag[MAX_LUN_NOME_VAR];
char *app;
char app_1[300];


strcpy(app_1,tag);
app=strstr(app_1," ");
if(app==NULL)
	return(-1);
*app=0;
strcpy(app_tag,app_1);

if(!OlTagToInd(database_topologia,app_tag,&ind,&tipo))
   {
   popup_errorDialog("Variable not found !!!",w);
   fprintf(stderr,"ERROR:variable =%s not found in database\n",app_tag);
   return -1;
   }

mod=(olDatabaseTopologiaClassRec.oldatabaseTopologia_class.indtomod)
        (database_topologia,ind);

printf("\n PLOT INDICI MISURA ind=%d  tipo=%d nmod=%d",ind,tipo,mod);

if(!XlSetVarGrafico(w,tag,ind,tipo,mod))
	return(-1);
return(ind);
}


void WriteDbAing()
{
FILE *fp;

if((fp=fopen("TemplateIndicAing.rtf","r"))!=NULL)
 {
 fclose(fp);
 return;
 }
fp=fopen("TemplateIndicAing.rtf","w");

fprintf(fp,"*top_descrizione:       TemplateIndic\n");
fprintf(fp,"*1w.trasparent: 0\n");
fprintf(fp,"*1w.normFg:     #000000000000\n");
fprintf(fp,"*1w.compiled:\t# 0 11 0 0 0 1.000000 0.000000 kg/s |\n");
fprintf(fp,"*1w.numeroDec:  2\n");
fprintf(fp,"*1w.scalamento: 1.00\n");
fprintf(fp,"*1w.normalFont: -adobe-itc souvenir-demi-i-normal--0-0-100-100-p-0-iso8859-1\n");
fprintf(fp,"*1w.offset:     0.00\n");
fprintf(fp,"*1w.varInputCambioColore1:   WAUCCC TANKCC TANICA NOP 1.0 0.0 kg/s\n");
fprintf(fp,"*1w.coloreFuoriScansione1:blue\n");
fprintf(fp,"*1w.coloreFuoriAttendib1: magenta\n");
fprintf(fp,"*1w.coloreDigSet1: #000000000000\n");
fprintf(fp,"*1w.coloreBassissimo1: #000000000000\n");
fprintf(fp,"*1w.coloreBassissimoBasso1: #000000000000\n");
fprintf(fp,"*1w.coloreBassoAlto1: #000000000000\n");
fprintf(fp,"*1w.coloreAltoAltissimo1: #000000000000\n");
fprintf(fp,"*1w.coloreAltissimo1: #000000000000\n");
fprintf(fp,"*1w.numeroInt:  6\n");
fprintf(fp,"*1w.borderWidth:        0\n");
fprintf(fp,"*1w.tipoInd:    2\n");
fprintf(fp,"*1w.paginaStazioni:\n");
fprintf(fp,"*1w.rotate:     0\n");
fprintf(fp,"*1w.assRotate:  0\n");
fprintf(fp,"*1w.background: #ffffffffffff\n");
fprintf(fp,"*1w.valoreMassimo:      99999999999.00\n");
fprintf(fp,"*1w.agoFg:      black\n");
fprintf(fp,"*1w.valoreMinimo:       -99999999999.00\n");
fprintf(fp,"*1w.borderColor:        #000000000000\n");
fprintf(fp,"*1w.x:  0\n");
fprintf(fp,"*1w.y:  0\n");
fprintf(fp,"*1w.x0:  0\n");
fprintf(fp,"*1w.y0:  0\n");
fprintf(fp,"*1w.inheritBackground:     0\n");
fprintf(fp,"*1w.width0:     190\n");
fprintf(fp,"*1w.height0:    40 \n");
fprintf(fp,"*num_widget:    1\n");
fprintf(fp,"*top_tipo:      Sinottico\n");
fprintf(fp,"*nomePag:       qq\n");
fprintf(fp,"*elenco_wid0:    1w Indic\n");
fprintf(fp,"*top_width:     900\n");
fprintf(fp,"*top_height:    900\n");
fprintf(fp,"*nextnum:       2\n");
fprintf(fp,"*drawing_height:        900\n");
fprintf(fp,"*top_x: 100\n");
fprintf(fp,"*top_y: 100\n");
fprintf(fp,"*drawing_width: 900\n");
fprintf(fp,"*drawing_background:    lime green\n");
fclose(fp);
}


void WriteDbPlot()
{
FILE *fp;

if((fp=fopen("TemplateGrafPlot.rtf","r"))!=NULL)
 {
 fclose(fp);
 return;
 }
fp=fopen("TemplateGrafPlot.rtf","w");

fprintf(fp,"*top_descrizione:       TemplateGraf\n");
fprintf(fp,"*1w.trasparent: 0\n");
fprintf(fp,"*1w.normFg:     #000000000000\n");
fprintf(fp,"*1w.numeroDec:  2\n");
fprintf(fp,"*1w.scalamento: 1.00\n");
fprintf(fp,"*1w.normalFont: -adobe-itc souvenir-demi-i-normal--0-0-100-100-p-0-iso8859-1\n");
fprintf(fp,"*1w.offset:     0.00\n");
fprintf(fp,"*1w.varInputCambioColore1:   WAUCCC TANKCC TANICA NOP 1.0 0.0 kg/s\n");
fprintf(fp,"*1w.coloreFuoriScansione1:blue\n");
fprintf(fp,"*1w.coloreFuoriAttendib1: magenta\n");
fprintf(fp,"*1w.coloreDigSet1: #000000000000\n");
fprintf(fp,"*1w.coloreBassissimo1: #000000000000\n");
fprintf(fp,"*1w.coloreBassissimoBasso1: #000000000000\n");
fprintf(fp,"*1w.coloreBassoAlto1: #000000000000\n");
fprintf(fp,"*1w.coloreAltoAltissimo1: #000000000000\n");
fprintf(fp,"*1w.coloreAltissimo1: #000000000000\n");
fprintf(fp,"*1w.numeroInt:  6\n");
fprintf(fp,"*1w.borderWidth:        0\n");
fprintf(fp,"*1w.tipoInd:    2\n");
fprintf(fp,"*1w.compiled:\t# 0 11 0 0 0 1.000000 0.000000 kg/s | -1 -1 -1 0 0 1.000000 0.000000 -- | -1 -1 -1 0 0 1.000000 0.000000 -- | -1 -1 -1 0 0 1.000000 0.000000 -- |\n");
fprintf(fp,"*1w.paginaStazioni:\n");
fprintf(fp,"*1w.rotate:     0\n");
fprintf(fp,"*1w.assRotate:  0\n");
fprintf(fp,"*1w.background: #ffffffffffff\n");
fprintf(fp,"*1w.valoreMassimo:      99999999999.00\n");
fprintf(fp,"*1w.agoFg:      black\n");
fprintf(fp,"*1w.valoreMinimo:       -99999999999.00\n");
fprintf(fp,"*1w.borderColor:        #000000000000\n");
fprintf(fp,"*1w.x:  0\n");
fprintf(fp,"*1w.y:  0\n");
fprintf(fp,"*1w.x0:  0\n");
fprintf(fp,"*1w.y0:  0\n");
fprintf(fp,"*1w.inheritBackground:     0\n");
fprintf(fp,"*1w.width0:     380\n");
fprintf(fp,"*1w.height0:    220\n");
fprintf(fp,"*1w.graficoType:        2\n");
fprintf(fp,"*1w.deltaTime:  %d\n",DELTA_PLOT);
fprintf(fp,"*1w.colorTrend1:  %s\n",COLOR_PLOT);
fprintf(fp,"*num_widget:    1\n");
fprintf(fp,"*top_tipo:      Sinottico\n");
fprintf(fp,"*nomePag:       qq\n");
fprintf(fp,"*elenco_wid0:    1w Grafico\n");
fprintf(fp,"*top_width:     900\n");
fprintf(fp,"*top_height:    900\n");
fprintf(fp,"*nextnum:       2\n");
fprintf(fp,"*drawing_height:        900\n");
fprintf(fp,"*top_x: 100\n");
fprintf(fp,"*top_y: 100\n");
fprintf(fp,"*refresh_freq: 1\n");
fprintf(fp,"*drawing_width: 900\n");
fprintf(fp,"*drawing_background:    lime green\n");
fclose(fp);
}




/*
   Funzione che gestisce la costruzione dell'oggetto OlTree.
*/
Boolean Build_OlTree(OlDatabaseTopologiaObject database_topologia)
{

   int i,j,hier_pb,alarm_state=0;
   int pos_info,cont_root=0,curr_lev;
   OlTreeObject root;
   char msg[500];
   int tipo_pag;
   time_t tempo_start,tempo_fine;
time(&tempo_start);
   /* Allocazione della radice dell'albero (OlTree) */
   for(j=0; j<totale_pagine; j++) {
      curr_lev=OlTreeGetLevel(pagine[j].gerarchia);
      if(curr_lev==0) {
         /* Trovata una pagina con le caratteristiche della radice */
         if(cont_root!=0) {
            /* Esistono piu' radici. Errore!! */
            show_warning("Hierarchy not loaded! More pages have root requirements!\n");
            return(False);
         }
         else {
            pos_info=j;
            cont_root++;
         }
      }
#ifdef PROVA_SPEED
      else {
         if(curr_lev==-1) {
            sprintf(msg,"WARNING!! Hierarchy resource not correct!! Page:%s\tHier:%s\n",pagine[j].nome,
                    pagine[j].gerarchia);
             show_warning(msg);
         }
      }
#endif
   }
   if(cont_root==0) {
      /* Non esistono pagine con le caratteristiche della root */
      show_warning("Hierarchy not loaded! Not present a page with root requirements!\n");
      return(False);
   }
/* Verifica tipo di pagina */
   if(!strcmp(pagine[pos_info].tipo,"Sinottico"))
	tipo_pag=OL_TYPE_SINOTTICO;
   else if (!strcmp(pagine[pos_info].tipo,"Teleperm"))
	tipo_pag=OL_TYPE_TELEPERM;
   else if (!strcmp(pagine[pos_info].tipo,"Stazioni"))
	tipo_pag=OL_TYPE_STAZIONI;
   else 
	tipo_pag=OL_TYPE_UNKNOWN;

   root=OlCreateTree(database_topologia,pos_info,pagine[pos_info].gerarchia,&alarm_state,tipo_pag);
   if(root==NULL) {
      /* Errore nell'allocazione della radice di OlTree */
      show_warning("Hierarchy not loaded! Error in root allocation!\n");
      return(False);
   }
#ifdef PROVA_SPEED
   else {
      sprintf(msg,"Hierarchy. Page root=%s loaded. Hierarchy resource value=%s\n\n",
              pagine[pos_info].nome,pagine[pos_info].gerarchia);
      show_warning(msg);
   } 
#endif

   /* Caricamento dell'albero (OlTree) */
   for(i=1; i<NUM_MAX_LIV; i++) {
#ifdef HIERDEBUG
      printf("Build_OlTree: Caricamento del livello=%d\n", i);
#endif
      for(j=0; j<totale_pagine; j++) {
         curr_lev=OlTreeGetLevel(pagine[j].gerarchia);
         if(curr_lev==i) {
            /* Trovata una pagina di livello i. Inserimento. */
/* Verifica tipo di pagina */
   		if(!strcmp(pagine[j].tipo,"Sinottico"))
			tipo_pag=OL_TYPE_SINOTTICO;
   		else if (!strcmp(pagine[j].tipo,"Teleperm"))
			tipo_pag=OL_TYPE_TELEPERM;
   		else if (!strcmp(pagine[j].tipo,"Stazioni"))
			tipo_pag=OL_TYPE_STAZIONI;
   		else 
			tipo_pag=OL_TYPE_UNKNOWN;
#ifdef HIERDEBUG
            printf("Build_OlTree: Inserimento di: %s\n",pagine[j].gerarchia);
#endif
            hier_pb=OlTreeAddPage(database_topologia,root,curr_lev,j,pagine[j].gerarchia,tipo_pag);
            /* Gestione del valore di ritorno della OlTreeAddPage */
                switch (hier_pb) {
#ifdef PROVA_SPEED
                  case  0:
                    /* Pagina inserita correttamente. */
                    sprintf(msg,"Hierarchy. Page %s loaded. Hierarchy resource value=%s\n",
                            pagine[j].nome,pagine[j].gerarchia);
#ifdef HIERDEBUG
printf("BuildOlTree: Hierarchy. Page %s loaded. Hierarchy resource value=%s\n",
        pagine[j].nome,pagine[j].gerarchia);
                    show_warning(msg);
#endif
                    break; 
#endif
                  case -1:
                     /* Pagina corrente senza padre. Non inserita. */
                     sprintf(msg,"Hierarchy problem. Page %s without father not loaded. Hierarchy resource value=%s\n", pagine[j].nome,pagine[j].gerarchia);
                     show_warning(msg);
                     break;
                  case -2:
                     /* Pagina corrente con valore della risorsa gerarchia gia' esistente. Non inserita. */
                     sprintf(msg,"Hierarchy problem. Page %s not loaded. A page with hierarchy resource value %s already loaded.\n", pagine[j].nome,pagine[j].gerarchia);
                     show_warning(msg);
                     break;
#ifdef PROVA_SPEED
		  case -3:
		     /* Pagina inserita correttamente. Non trovata variabile allarme nel file fnomi.rtf */
                     sprintf(msg,"Hierarchy. Page %s loaded. Hierarchy resource value=%s\n",
                             pagine[j].nome,pagine[j].gerarchia);
                     show_warning(msg);
                     break;
#endif
                }

            }

         }

      }
   root_oltree=root;
time( &tempo_fine);
printf("tempo_start=%ld tempo_fine=%ld\n",tempo_start,tempo_fine);
printf("BUILDOLTREE: tempo impiegato=%ld\n",(tempo_fine-tempo_start));
   return(True);

}





/*
        Funzione per testare l'oggetto OlTree.
*/
int TestOlTree()
{

   OlTreeObject curr_oltree,app_oltree;
   int i,info_pos[20],info_pos_curr,progress;

   /* Chiamata del metodo OlTreeGetPage */
   if(OlTreeGetPage(root_oltree,&curr_oltree,&progress,&info_pos_curr,pagine[IND_TEST].gerarchia)==True) {
   printf("pos_info=%d   nome_pagina=%s   gerarchia=%s\n",info_pos_curr,
          pagine[info_pos_curr].nome,pagine[info_pos_curr].gerarchia);
   app_oltree=OlTreeGetLeft(curr_oltree,&info_pos_curr);
   printf("nome_paginaleft=%s\n",pagine[info_pos_curr].nome);
   curr_oltree=app_oltree;
   app_oltree=OlTreeGetLeft(curr_oltree, &info_pos_curr);
   printf("nome_paginaleft=%s\n",pagine[info_pos_curr].nome);
   curr_oltree=app_oltree;
   app_oltree=OlTreeGetRight(curr_oltree, &info_pos_curr);
   printf("nome_paginaright=%s\n",pagine[info_pos_curr].nome);
   curr_oltree=app_oltree;
   app_oltree=OlTreeGetTop(curr_oltree,&info_pos_curr);
   printf("nome_paginatop=%s\n",pagine[info_pos_curr].nome);
   curr_oltree=app_oltree;
   app_oltree=OlTreeGetTop(curr_oltree,&info_pos_curr);
   printf("nome_paginatop=%s\n",pagine[info_pos_curr].nome);
   curr_oltree=app_oltree;
   /* Test del metodo OlTreeGetSubInfo() */
   if(OlTreeGetSubInfo(curr_oltree,info_pos)==True) {
      i=0;
      while(info_pos[i]!=-1) {
         printf("child: nome_pagina=%s\n",pagine[info_pos[i]].nome);
         i++;
      }
   }
   else {
      printf("OlTreeGetSubInfo: Pagina corrente senza figli!!\n");
   }

  
   }


}

int PagIsTeleperm(AN_PAG pag)
{
   if( strcmp(pag.tipo,"Teleperm") == 0 )
      return(True);
   else
      return(False);
}


/******************************************************
 * 
 * EffettoPushButton
 *
 * funzione che inverte i colori di shadowing
 * per simulare il bottone PushedIn/PushedOut
 * (chiamando due volte consecutive questa funzione
 *  i colori restano inalterati)
 *
 * FUNZIONA SOLO CON BOTTONI CON LABEL INTERNA
 * se e' presente un Pixmap occorre fare una XmGetPixmap
 * ed invertirne il bg e fg
 ******************************************************/

void EffettoPushButton( Widget w )
{
   Pixel bkg_col,arm_col,bot_col,top_col,dummy_col;

   /* recupero i colori correnti dello widget
   */
   XtVaGetValues(w, XmNbackground,&bkg_col,
                    XmNarmColor,&arm_col,
                    XmNbottomShadowColor,&bot_col,
                    XmNtopShadowColor,&top_col,
                    NULL);

   /* inverto i colori
   */
   dummy_col = bkg_col;
   bkg_col = arm_col;
   arm_col = dummy_col; 
   dummy_col = top_col;
   top_col = bot_col;
   bot_col = dummy_col;

   /* e li reimposto allo widget
   */
   XtVaSetValues(w, XmNbackground,bkg_col,
                    XmNarmColor,arm_col,
                    XmNbottomShadowColor,bot_col,
                    XmNtopShadowColor,top_col,
                    NULL);          

}
