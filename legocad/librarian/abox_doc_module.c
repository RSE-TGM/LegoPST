
/*******************************************************************************
	abox_doc_module.c
	(Generated from interface file abox_doc_module.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/ScrolledW.h>
#include <Xm/SeparatoG.h>
#include <Xm/LabelG.h>
#include <Xm/PushB.h>
#include <Xm/ArrowB.h>
#include <Xm/Label.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo abox_doc_module.i
   tipo 
   release 2.22
   data 5/9/95
   reserved @(#)abox_doc_module.i	2.22
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif
/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/

#include <ctype.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#ifndef LIBUTILX
#include <libutilx.h>
#define LIBUTILX
#endif

#ifndef LIBICOFILE
#include <file_icone.h>
#define LIBICOFILE
#endif

#include "definizioni.h"

/****************************************************************/
/* VARIABILI GLOBALI ESTERNE					*/
/****************************************************************/

extern Arg args[];
extern Cardinal argcount;

extern Widget UxTopLevel, attention_wdg;
extern Dialog_geometry geom_attention;

/* Array di compound strings */
extern XmString cstrings[], cstring, cstring_null;

extern char message[];

/* VARIABILE GLOBALE DEL NOME DEL MODULO SELEZIONATO */
extern char nome_modulo[];
extern char descr_modulo[];

extern byte tipo_modulo;

/****************************************************************/
/* VARIABILI GLOBALI						*/
/****************************************************************/

/* dialog-box lista dei moduli */
Boolean dbox_doclist_managed = False;
Boolean dbox_doc_managed = False;

/* scroll-list dei moduli */
swidget dbox_doclist;

char doc_modulo[256], doc_of_module[5];

/* VARIABILE GLOBALE DELLA SEZIONE DI DOCUMENTAZIONE CORRENTE */
short int sezione_attiva = 0;

Sezioni sezione[] =  {
                        {"CARD A", "FUNCTION"},
                        {"CARD B", "TOPOLOGICAL DATA"},
                        {"CARD C", "PHYSICAL AND GEOMETRICAL DATA"},
                        {"CARD D",
                          "MODULE VARIABLES INITIALIZATION"},
                        {"CARD E", "USER SUBPROGRAMS"},
                        {"CARD F", "RESIDUALS"},
                        {"CARD G", "MODULE SUBPROGRAMS"},
                        {"CARD H", "TEST CASES"}
                     };

Text_find_struct parametri_doc;

/*******************************************************************************
	The definition of the context structure:
	If you create multiple instances of your interface, the context
	structure ensures that your callbacks use the variables for the
	correct instance.

	For each swidget in the interface, each argument to the Interface
	function, and each variable in the Instance Specific section of the
	Declarations Editor, there is an entry in the context structure.
	and a #define.  The #define makes the variable name refer to the
	corresponding entry in the context structure.
*******************************************************************************/

typedef	struct
{
	Widget	Uxabox_doc_module;
	Widget	Uxform2;
	Widget	Uxframe2;
	Widget	Uxlabel3;
	Widget	Uxarrow_prec;
	Widget	Uxarrow_next;
	Widget	Uxdoc_cancelButton;
	Widget	UxlabelGadget1;
	Widget	UxseparatorGadget4;
	Widget	Uxform6;
	Widget	UxscrolledWindow3;
	Widget	Uxdoc_scroll_text;
	Widget	Uxdoc_loadButton;
	Widget	Uxdoc_listButton;
} _UxCabox_doc_module;

#define abox_doc_module         UxAbox_doc_moduleContext->Uxabox_doc_module
#define form2                   UxAbox_doc_moduleContext->Uxform2
#define frame2                  UxAbox_doc_moduleContext->Uxframe2
#define label3                  UxAbox_doc_moduleContext->Uxlabel3
#define arrow_prec              UxAbox_doc_moduleContext->Uxarrow_prec
#define arrow_next              UxAbox_doc_moduleContext->Uxarrow_next
#define doc_cancelButton        UxAbox_doc_moduleContext->Uxdoc_cancelButton
#define labelGadget1            UxAbox_doc_moduleContext->UxlabelGadget1
#define separatorGadget4        UxAbox_doc_moduleContext->UxseparatorGadget4
#define form6                   UxAbox_doc_moduleContext->Uxform6
#define scrolledWindow3         UxAbox_doc_moduleContext->UxscrolledWindow3
#define doc_scroll_text         UxAbox_doc_moduleContext->Uxdoc_scroll_text
#define doc_loadButton          UxAbox_doc_moduleContext->Uxdoc_loadButton
#define doc_listButton          UxAbox_doc_moduleContext->Uxdoc_listButton

static _UxCabox_doc_module	*UxAbox_doc_moduleContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_doc_modulo();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*** void prepara_file_doc(modulo)
 ***    Parametri:
 ***       char *modulo : nome del modulo
 ***
 ***    Descrizione:
 ***       Visualizza la prima sezione ("A - FUNZIONE") della documentazione
 ***       del modulo.
 ***/ 
void prepara_file_doc(modulo)
char modulo[];
{

/* Composizione percorso file documentazione */
/* 20-1-95 Micheletti
   componi_file_modulo(doc_modulo,modulo, FILE_DOC, tipo_modulo, LIB_STANDARD);
*/
   componi_file_modulo(doc_modulo,modulo, FILE_DOC, tipo_modulo, LIB_UTENTE);


/* recupera la prima sezione di documentazione */
   if (leggi_sezione_doc(0))
      scrivi_messaggio ("Couldn't open documentation.");

/* All'apertura della finestra e' disabilitata la ricerca indietro */
   set_something_val (UxGetWidget(arrow_prec),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(arrow_next),XmNsensitive, (XtArgVal) True);
}

/************************************************************/
/*** leggi_sezione_doc(ind_sez)
 ***    Parametri:
 ***      int ind_sez : indice array sezione[].
 ***
 ***    Descrizione:
 ***      Visualizza la parte di documentazione relativa alla sezione
 ***      indicata.
 ***/
leggi_sezione_doc(ind_sez)
int ind_sez;
{
   char  ptr[81], titolo_finestra[30], modulo_lowercase[5];
   char  *pattern_tmp;
   int char_counter = 0;
   FILE *fp_doc;
   Boolean primo_ciclo;
   char *stringone=NULL;
   Boolean first=True; 

   if ((fp_doc = fopen (doc_modulo,"r")) == NULL)
   {
   /* Se non c'e' nessun modulo selezionato globalmente */
      if (!strcmp(nome_modulo,""))
         strcpy (message,"No module selected.");
      else
         sprintf (message,"File %s does not exist.", doc_modulo);
      scrivi_messaggio (message);
      XmTextSetString(UxGetWidget(doc_scroll_text),"<NO DOCUMENTATION FOUND>");
      return(1);
   }

/* Inizializza il Text-Willer */
   XmTextSetString(UxGetWidget(doc_scroll_text),"");

/* Aggiornamento titolo finestra */
   sprintf (titolo_finestra,"MODULE %.4s DOCUMENTATION",
            doc_modulo+strlen(doc_modulo)-8);
   tomaius (titolo_finestra);
   set_something_val (UxGetWidget(abox_doc_module), XmNtitle, (XtArgVal) titolo_finestra);

/*******************************************************************/ 
/* Leggo il file doc finche' non trovo la sezione da acquisire     */
/* La ricerca e' limitata a 6 lettere ("CARD $")                   */
/*******************************************************************/ 
/* N.B. Il pattern "CARD $" dovrebbe cominciare a colonna 70, ma   */
/* talvolta comincia prima. Per prudenza la ricerca comincia da 68 */
/*******************************************************************/ 
   while( fgets(ptr,80,fp_doc) != NULL )
     if ( strlen(ptr) > 68 )
        if ( confronta_stringhe(ptr+68,sezione[ind_sez].pattern,5) == 0)
           break;

   if (ind_sez < 7)
      pattern_tmp = sezione[ind_sez+1].pattern;
   else
      pattern_tmp = NULL;

/* Acquisisco finche' non trovo l'inizio della sezione successiva */
   primo_ciclo = True;
   while( fgets(ptr,80,fp_doc) != NULL )
     if ( strlen(ptr) <= 68 || 
          (strlen(ptr) > 68 && confronta_stringhe(ptr+68,pattern_tmp,5)) )
     {
        if( stringone )
           stringone = (char *)XtRealloc(stringone,sizeof(char)*strlen(ptr)+sizeof(char)*strlen(stringone)+4);
        else
           stringone = (char *)XtRealloc(stringone,sizeof(char)*strlen(ptr)+4);  
/*
        XmTextInsert(UxGetWidget(doc_scroll_text),char_counter,ptr);
*/

        if( first )
        {
           strcpy(stringone,ptr);
           first=False;
        }
        else
           strcat(stringone,ptr);
     
        char_counter += strlen(ptr);
     }
     else
        break;  /* uscita dal ciclo */

   XmTextInsert(UxGetWidget(doc_scroll_text),char_counter,stringone);
   first = True;
   free(stringone);
   stringone = NULL; 

   fclose (fp_doc);

/* Fisso il valore della variabile globale della sezione visualizzata */
   sezione_attiva = ind_sez;

/* Aggiornamento titolo sezione*/
   set_label(UxGetWidget(label3),sezione[ind_sez].descrizione);

   return (0);
}

/*****************************************************************/
/*** cambia_sezione_doc(verso)
 ***    Parametri:
 ***       byte verso : SEZIONE_NEXT o SEZIONE_PREC
 ***
 ***    Descrizione:
 ***       Cambia la sezione (successiva o precedente alla sezione corrente).
 ***/
cambia_sezione_doc(verso)
byte verso;
{
   Boolean status; /* stato Sensitive True/False per gli arrow button */

   switch (verso)
   {
      case SEZIONE_NEXT:

         if (sezione_attiva < 7)
         {
            set_something_val (UxGetWidget(arrow_prec),XmNsensitive, (XtArgVal) True);
            sezione_attiva++;
            if (sezione_attiva == 7)
                set_something_val (UxGetWidget(arrow_next),XmNsensitive, (XtArgVal) False);
         }
         break;

      case SEZIONE_PREC:

        if (sezione_attiva > 0)
        {

            set_something_val (UxGetWidget(arrow_next),XmNsensitive, (XtArgVal) True);
            sezione_attiva--;
            if (sezione_attiva == 0)
                set_something_val (UxGetWidget(arrow_prec),XmNsensitive, (XtArgVal) False);
        }
        break;
   }  /* Fine switch */

   leggi_sezione_doc (sezione_attiva);
}

/*****************************************************************/
/*** confronta_stringhe(stringa1, stringa2, numchar)
 ***   Parametri:
 ***      char *stringa1, *stringa2;
 ***      int numchar : Numero di caratteri da confrontare.
 ***
 ***   Descrizione:
 ***      Confronta due stringhe a partire dal primo carattere non-blank.
 ***/
confronta_stringhe(stringa1, stringa2, numchar)
char *stringa1, *stringa2;
int numchar;
{
   if (stringa1 == NULL || stringa2 == NULL)
      return(-1);

   /* Confronto tra le due stringhe finche' non si arriva alla fine
      di una delle due  o  all'ultimo carattere da confrontare */
   while (*stringa1 != '\0' && *stringa2 != '\0'  && numchar)
   {

     /* Posizionamento sul primo carattere non blank o tab di stringa1 */
     while ( (*stringa1 == ' ' || *stringa1 == '\t' || *stringa1 == '\n')
            && *stringa1 != '\0')
         stringa1++;

     /* Posizionamento sul primo carattere non blank o tab di stringa 2 */
     while ( (*stringa2 == ' ' || *stringa2 == '\t' || *stringa2 == '\n')
            && *stringa2 != '\0')
         stringa2++;

    /* Confronto tra le due stringhe, carattere per carattere, a partire
       dall'ultimo blank o tab per arrivare al successivo o a fine stringa 
       o all'ultimo carattere da confrontare */
     while (*stringa2 == *stringa1 && *stringa2 != ' ' && *stringa2 != '\t'
            && *stringa2 != '\n' && *stringa2 != '\0' && numchar)
     {
        stringa2++;
        stringa1++;
        numchar--;
     }

    /* Se le stringhe differiscono prima di arrivare all'ultimo carattere */
     if ( !((*stringa1 == ' ' || *stringa1 == '\n' || *stringa1 == '\t' ||
           *stringa1 == '\0' ) &&
          (*stringa2 == ' ' || *stringa2 == '\n' || *stringa2 == '\t' ||
           *stringa2 == '\0' )) && numchar &&
           *stringa2 != *stringa1)
        return (-1);
   } 

   /* Se una stringa e' composta da un numero di caratteri < numchar iniziale */
   /* alla fine del confronto tra le stringhe numchar non vale 0 */
   if (numchar > 0)
      return (-1);

   /* ALTRIMENTI LE STRINGHE SONO IDENTICHE */
   return (0);
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_arrow_prec( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCabox_doc_module     *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAbox_doc_moduleContext;
	UxAbox_doc_moduleContext = UxContext =
			(_UxCabox_doc_module *) UxGetContext( UxWidget );
	{
	cambia_sezione_doc(SEZIONE_PREC);
	}
	UxAbox_doc_moduleContext = UxSaveCtx;
}

static void	activateCB_arrow_next( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCabox_doc_module     *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAbox_doc_moduleContext;
	UxAbox_doc_moduleContext = UxContext =
			(_UxCabox_doc_module *) UxGetContext( UxWidget );
	{
	cambia_sezione_doc(SEZIONE_NEXT);
	}
	UxAbox_doc_moduleContext = UxSaveCtx;
}

static void	activateCB_doc_cancelButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCabox_doc_module     *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAbox_doc_moduleContext;
	UxAbox_doc_moduleContext = UxContext =
			(_UxCabox_doc_module *) UxGetContext( UxWidget );
	{
	/* Cancella la finestra di documentazione */
	   UxDestroySwidget(abox_doc_module);
	   dbox_doc_managed = False;
	
	/* Cancella l'eventuale lista di files *.DOC rimasta aperta */
	   if (dbox_doclist_managed)
	   {
	      UxDestroySwidget(dbox_doclist);
	      dbox_doclist_managed = False;
	   }
	
	}
	UxAbox_doc_moduleContext = UxSaveCtx;
}

static void	activateCB_doc_loadButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCabox_doc_module     *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAbox_doc_moduleContext;
	UxAbox_doc_moduleContext = UxContext =
			(_UxCabox_doc_module *) UxGetContext( UxWidget );
	{
	/* Se c'e' un modulo selezionato lo carica */
	if (strcmp(nome_modulo,""))
	{
	   prepara_file_doc(nome_modulo);
	   return;
	}
	
	}
	UxAbox_doc_moduleContext = UxSaveCtx;
}

static void	activateCB_doc_listButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCabox_doc_module     *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAbox_doc_moduleContext;
	UxAbox_doc_moduleContext = UxContext =
			(_UxCabox_doc_module *) UxGetContext( UxWidget );
	{
	char file_lista[256];
	FILE *fpd;
	
	/* Se la dialogBox di selezione dei moduli */
	/* e' visualizzata, ritorna */
	if ( dbox_doclist_managed )
	{
	   sprintf(message," Modules list window already in use.");
	   attention_wdg = (Widget) attention (UxTopLevel, message, MAPPA,
	                                       geom_attention);
	   scrivi_messaggio (message);
	   return;
	}
	
	file_dati_lib(file_lista, tipo_modulo, LIB_UTENTE);
	
	/* Se il file lista dei moduli (LIBUT,LIBUTREG,LIBREG) non esiste esci */
	if ((fpd = fopen (file_lista, "r")) == NULL)
	{
	   scrivi_messaggio ("Can't open data file.");
	   return;
	}
	
	/* Visualizza la lista dei moduli della libreria appropriata */
	dbox_doclist = create_lista_moduli(fpd,tipo_modulo,STDLIB_MODULES_DOC,
	                                   &dbox_doclist_managed);
	fclose(fpd);
	
	}
	UxAbox_doc_moduleContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_abox_doc_module()
{

	abox_doc_module = XtVaCreatePopupShell( "abox_doc_module",
			applicationShellWidgetClass, UxTopLevel,
			XmNgeometry, "+80+185",
			XmNtitle, "MODULE DOCUMENTATION",
			XmNiconName, "LEGOdoc",
			XmNheight, 520,
			XmNwidth, 800,
			XmNy, 580,
			XmNx, 70,
			NULL );

	UxPutContext( abox_doc_module, (char *) UxAbox_doc_moduleContext );

	form2 = XtVaCreateManagedWidget( "form2",
			xmFormWidgetClass, abox_doc_module,
			XmNheight, 390,
			XmNwidth, 800,
			XmNy, 10,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form2, (char *) UxAbox_doc_moduleContext );

	frame2 = XtVaCreateManagedWidget( "frame2",
			xmFrameWidgetClass, form2,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 10,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 670,
			XmNy, 10,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( frame2, (char *) UxAbox_doc_moduleContext );

	label3 = XtVaCreateManagedWidget( "label3",
			xmLabelWidgetClass, frame2,
			RES_CONVERT( XmNlabelString, "SECTION TITLE" ),
			XmNheight, 30,
			XmNwidth, 570,
			XmNy, 10,
			XmNx, 50,
			NULL );

	UxPutContext( label3, (char *) UxAbox_doc_moduleContext );

	arrow_prec = XtVaCreateManagedWidget( "arrow_prec",
			xmArrowButtonWidgetClass, form2,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 40,
			XmNy, 350,
			XmNx, 10,
			NULL );

	UxPutContext( arrow_prec, (char *) UxAbox_doc_moduleContext );

	arrow_next = XtVaCreateManagedWidget( "arrow_next",
			xmArrowButtonWidgetClass, form2,
			XmNarrowDirection, XmARROW_DOWN,
			XmNleftWidget, arrow_prec,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 40,
			XmNy, 360,
			XmNx, 70,
			NULL );

	UxPutContext( arrow_next, (char *) UxAbox_doc_moduleContext );

	doc_cancelButton = XtVaCreateManagedWidget( "doc_cancelButton",
			xmPushButtonWidgetClass, form2,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 60,
			XmNy, 410,
			XmNx, 630,
			NULL );

	UxPutContext( doc_cancelButton, (char *) UxAbox_doc_moduleContext );

	labelGadget1 = XtVaCreateManagedWidget( "labelGadget1",
			xmLabelGadgetClass, form2,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNlabelString, "Move to another section" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftWidget, arrow_next,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 200,
			XmNy, 420,
			XmNx, 110,
			NULL );

	UxPutContext( labelGadget1, (char *) UxAbox_doc_moduleContext );

	separatorGadget4 = XtVaCreateManagedWidget( "separatorGadget4",
			xmSeparatorGadgetClass, form2,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, arrow_prec,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 10,
			XmNwidth, 760,
			XmNy, 480,
			XmNx, 20,
			NULL );

	UxPutContext( separatorGadget4, (char *) UxAbox_doc_moduleContext );

	form6 = XtVaCreateManagedWidget( "form6",
			xmFormWidgetClass, form2,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, separatorGadget4,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 40,
			XmNwidth, 770,
			XmNy, 430,
			XmNx, 20,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form6, (char *) UxAbox_doc_moduleContext );

	scrolledWindow3 = XtVaCreateManagedWidget( "scrolledWindow3",
			xmScrolledWindowWidgetClass, form2,
			XmNtopWidget, frame2,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, form6,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 50,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( scrolledWindow3, (char *) UxAbox_doc_moduleContext );

	doc_scroll_text = XtVaCreateManagedWidget( "doc_scroll_text",
			xmTextWidgetClass, scrolledWindow3,
			XmNresizeWidth, TRUE,
			XmNresizeHeight, TRUE,
			XmNfontList, UxConvertFontList( "-Adobe-Courier-Bold-R-Normal--14-140-75-75-M-90-ISO8859-1" ),
			XmNeditable, FALSE,
			XmNeditMode, XmMULTI_LINE_EDIT ,
			XmNcolumns, 81,
			XmNheight, 350,
			XmNwidth, 790,
			NULL );

	UxPutContext( doc_scroll_text, (char *) UxAbox_doc_moduleContext );

	doc_loadButton = XtVaCreateManagedWidget( "doc_loadButton",
			xmPushButtonWidgetClass, form2,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightWidget, doc_cancelButton,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNlabelString, "Load" ),
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 60,
			XmNy, 480,
			XmNx, 660,
			NULL );

	UxPutContext( doc_loadButton, (char *) UxAbox_doc_moduleContext );

	doc_listButton = XtVaCreateManagedWidget( "doc_listButton",
			xmPushButtonWidgetClass, form2,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightWidget, doc_loadButton,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNlabelString, "List" ),
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 60,
			XmNy, 480,
			XmNx, 560,
			NULL );

	UxPutContext( doc_listButton, (char *) UxAbox_doc_moduleContext );

	XtAddCallback( abox_doc_module, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxAbox_doc_moduleContext );

	XtAddCallback( arrow_prec, XmNactivateCallback,
			activateCB_arrow_prec,
			(XtPointer) UxAbox_doc_moduleContext );

	XtAddCallback( arrow_next, XmNactivateCallback,
			activateCB_arrow_next,
			(XtPointer) UxAbox_doc_moduleContext );

	XtAddCallback( doc_cancelButton, XmNactivateCallback,
			activateCB_doc_cancelButton,
			(XtPointer) UxAbox_doc_moduleContext );

	XtAddCallback( doc_loadButton, XmNactivateCallback,
			activateCB_doc_loadButton,
			(XtPointer) UxAbox_doc_moduleContext );

	XtAddCallback( doc_listButton, XmNactivateCallback,
			activateCB_doc_listButton,
			(XtPointer) UxAbox_doc_moduleContext );



	return ( abox_doc_module );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_doc_modulo()
{
	Widget                  rtrn;
	_UxCabox_doc_module     *UxContext;

	UxAbox_doc_moduleContext = UxContext =
		(_UxCabox_doc_module *) XtMalloc( sizeof(_UxCabox_doc_module) );

	{
		Widget doc_find_form;
		rtrn = _Uxbuild_abox_doc_module();

		/* Composizione percorso file documentazione */
		/* Allocazione della memoria per il testo di documentazione */
		/* Recupero della prima sezione di documentazione */
		   prepara_file_doc(nome_modulo);
		
		/* Inserimento find di libreria */
		   parametri_doc.ID_text = UxGetWidget(doc_scroll_text);
		   argcount=0;
		   XtSetArg(args[argcount],XmNleftAttachment,XmATTACH_FORM);argcount++;
		   XtSetArg(args[argcount],XmNleftOffset,0);argcount++;
		   XtSetArg(args[argcount],XmNrightAttachment,XmATTACH_FORM);argcount++;
		   XtSetArg(args[argcount],XmNrightOffset,0);argcount++;
		   XtSetArg(args[argcount],XmNbottomAttachment,XmATTACH_FORM);argcount++;
		   XtSetArg(args[argcount],XmNbottomOffset,0);argcount++;
		   XtSetArg(args[argcount],XmNtopAttachment,XmATTACH_FORM);argcount++;
		   XtSetArg(args[argcount],XmNtopOffset,0);argcount++;
		   doc_find_form = (Widget) find_in_a_text (UxGetWidget(form6),args,argcount,
		                                              &parametri_doc);
		   XtManageChild(doc_find_form);
		
		   UxPopupInterface(rtrn, no_grab);
		   dbox_doc_managed = True;
		
		/* non deve essere piu' utilizzato 11-1-95 
		   da quando la documentazione si trova nella
		   LIBUT UTENTE 
		*/ 
		XtUnmanageChild( doc_listButton);
		
		   return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_doc_modulo()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_doc_modulo();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

