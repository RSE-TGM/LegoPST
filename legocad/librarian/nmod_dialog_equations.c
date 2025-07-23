
/*******************************************************************************
	nmod_dialog_equations.c
	(Generated from interface file nmod_dialog_equations.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Label.h>
#include <Xm/BulletinB.h>
#include <Xm/ScrolledW.h>
#include <Xm/LabelG.h>
#include <Xm/SeparatoG.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/DialogS.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo nmod_dialog_equations.i
   tipo 
   release 2.23
   data 5/9/95
   reserved @(#)nmod_dialog_equations.i	2.23
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

#include <string.h>
#include <fcntl.h>
#include <Xm/Xm.h>

#include "definizioni.h"

#ifndef LIBUTILX
#include <libutilx.h>
#define LIBUTILX
#endif

/****************************************************************/
/* VARIABILI GLOBALI ESTERNE					*/
/****************************************************************/

extern Boolean non_salvato;

extern Arg args[];
extern Cardinal argcount;

extern Boolean bool_dialog_equation;

extern int num_var_stato,
           num_var_algebriche,
           num_var_ingresso,
           num_dati_geometrici,
           jac_rows,
           jac_cols;

extern StructVars *variabili;

extern JacToggStruct jactoggstruct[100][300];

extern swidget pb_nmod_varsetup, pb_nmod_JC;
extern Boolean nmod_def_initialized;
extern byte jacobian_type;

extern char *names[];
extern int num_colors;
extern Pixel apix[];
extern short color_values[];
extern Colormap colormap;
extern Display *display;
extern Widget _nmod_varsetup;
extern XmString cstring;

/****************************************************************/
/* VARIABILI GLOBALI						*/
/****************************************************************/

/* indica il numero di toggle settati a 'yes' e serve per dimensionare */
/* alcuni array */
int num_yes_toggle;

char file_jac_temp[30];
Boolean prima_volta_jac = True;

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
	Widget	Uxnmod_dialog_equations;
	Widget	Uxnmod_dialog2_form1;
	Widget	Uxnmod_dialog2_ok;
	Widget	Uxnmod_dialog2_sep1;
	Widget	UxlabelGadget6;
	Widget	Uxscrollw_jac_var;
	Widget	Uxbboard_jac_var;
	Widget	Uxscrollw_jac_equ;
	Widget	Uxbboard_jac_equ;
	Widget	UxlabelGadget8;
	Widget	Uxnmod_dialog2_scrollw3;
	Widget	Uxbboard_jac_toggles;
	Widget	Uxnmod_dialog2_cancel;
	Widget	Uxlabel1;
	Widget	Uxlabel2;
} _UxCnmod_dialog_equations;

#define nmod_dialog_equations   UxNmod_dialog_equationsContext->Uxnmod_dialog_equations
#define nmod_dialog2_form1      UxNmod_dialog_equationsContext->Uxnmod_dialog2_form1
#define nmod_dialog2_ok         UxNmod_dialog_equationsContext->Uxnmod_dialog2_ok
#define nmod_dialog2_sep1       UxNmod_dialog_equationsContext->Uxnmod_dialog2_sep1
#define labelGadget6            UxNmod_dialog_equationsContext->UxlabelGadget6
#define scrollw_jac_var         UxNmod_dialog_equationsContext->Uxscrollw_jac_var
#define bboard_jac_var          UxNmod_dialog_equationsContext->Uxbboard_jac_var
#define scrollw_jac_equ         UxNmod_dialog_equationsContext->Uxscrollw_jac_equ
#define bboard_jac_equ          UxNmod_dialog_equationsContext->Uxbboard_jac_equ
#define labelGadget8            UxNmod_dialog_equationsContext->UxlabelGadget8
#define nmod_dialog2_scrollw3   UxNmod_dialog_equationsContext->Uxnmod_dialog2_scrollw3
#define bboard_jac_toggles      UxNmod_dialog_equationsContext->Uxbboard_jac_toggles
#define nmod_dialog2_cancel     UxNmod_dialog_equationsContext->Uxnmod_dialog2_cancel
#define label1                  UxNmod_dialog_equationsContext->Uxlabel1
#define label2                  UxNmod_dialog_equationsContext->Uxlabel2

static _UxCnmod_dialog_equations	*UxNmod_dialog_equationsContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_dialog_equation();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/***************************************************************/
/*** dimensiona_widgets()
 ***
 ***   Descrizione:
 ***      Dimensiona alcuni widgets per l'allineamento della matrice jacobiana.
 ***/
dimensiona_widgets()
{
/* Dimensionamento tavola dei toggleButtons */
   set_something_val (UxGetWidget(bboard_jac_toggles),
                  XmNwidth, (MARGIN+WIDTH_JAC_TOGGLES)*jac_cols+MARGIN);
   set_something_val (UxGetWidget(bboard_jac_toggles),
                  XmNheight,(MARGIN+HEIGHT_JAC_TOGGLES)*jac_rows+MARGIN);

/* Dimensionamento tavola delle labels  */
   set_something_val (UxGetWidget(bboard_jac_equ),
                  XmNheight,(MARGIN+HEIGHT_JAC_TOGGLES)*jac_rows+MARGIN);
   set_something_val (UxGetWidget(bboard_jac_var),
                  XmNwidth, (MARGIN+WIDTH_JAC_TOGGLES)*jac_cols+MARGIN);
}

/***************************************************************/
/*** salva_jac()
 ***
 ***   Descrizione:
 ***     Scrive in un file temp, in modalita' binaria, gli stati YES-NO
 ***     dello jacobiano prima della visualizzazione della window.
 ***     Gli stati possono essere cosi' recuperati se l'utente abbandona 
 ***     le eventuali modifiche.
 ***/
salva_jac()
{
   int i,j,fd;

   if ( (fd = creat (file_jac_temp,0666)) == -1 )
   {
      printf ("Can't open temporary file for jacobian states storage. Sorry.\n");
      return (-1);
   }

   for (i=0; i<jac_rows; i++)
      for (j=0; j<jac_cols; j++)
         write (fd, (char *)&jactoggstruct[i][j].jac_yes,sizeof(Boolean));

   close (fd);
   return (0);
}

/***************************************************************/
/*** leggi_jac()
 ***
 ***   Descrizione:
 ***      Legge da un file temp, in modalita' binaria, gli stati YES-NO
 ***      dello jacobiano prima della visualizzazione della window.
 ***/
leggi_jac()
{
   int i,j,fd;

   if ( (fd = open (file_jac_temp,O_RDONLY)) == -1 )
   {
      printf ("Can't open temporary file for jacobian states retrieve. Sorry.\n");
      return (-1);
   }

   for (i=0; i<jac_rows; i++)
      for (j=0; j<jac_cols; j++)
         read (fd, (char *)&jactoggstruct[i][j].jac_yes, sizeof(Boolean));

   close (fd);
   return (0);
}

/***************************************************************/
/*** crea_jac_toggles(jactructptr,posx, posy)
 ***
 ***   Descrizione:
 ***     Utilita' per la creazione dei toggleButton dello jacobiano 
 ***     nella scrolled-window.
 ***     N.B. Per volonta' del Divino Sylvan non si tratta piu' di toggleButtons
 ***     bensi' di drawnButtons pieni e vuoti (bianchi e neri).
 ***     Inizialmente sono tutti BLACK, cioe' settati a YES (=dipendenza).
 ***/
crea_jac_toggles(jacstructptr, posx, posy)
JacToggStruct *jacstructptr;
int posx, posy;
{
   Boolean set;
   Pixel bg;

   set = jacstructptr->jac_yes;
   bg  = ((set) ? apix[BLACK] : apix[WHITE]);

   argcount = 0;
   XtSetArg(args[argcount], XmNx, posx); argcount++;
   XtSetArg(args[argcount], XmNy, posy);  argcount++;
   XtSetArg(args[argcount], XmNwidth,  WIDTH_JAC_TOGGLES);  argcount++;
   XtSetArg(args[argcount], XmNheight, HEIGHT_JAC_TOGGLES); argcount++;
   XtSetArg(args[argcount], XmNbackground, bg); argcount++;
   XtSetArg(args[argcount], XmNrecomputeSize, False); argcount++;
   jacstructptr->wdg = (Widget) XmCreateDrawnButton(UxGetWidget(bboard_jac_toggles), 
                                                    "NewmodToggle", args, argcount);

   XtAddCallback (jacstructptr->wdg, XmNactivateCallback, jac_toggle_premuto, 
                  jacstructptr);

   XtManageChild (jacstructptr->wdg);
}

/***************************************************************/
/*** jac_toggle_premuto(w,jacstruct_ptr,reason)
 ***
 ***   Descrizione:
 ***     Callback chiamata premendo un qualsiasi toggleButton dello jacobiano.
 ***     Modifica la label e lo state del toggle premuto, aggiornando l'array
 ***     degli stati della matrice.
 ***     N.B. Trattasi di DrawnButton.
 ***/
void jac_toggle_premuto (w,jacstruct_ptr,reason)
Widget w;
JacToggStruct *jacstruct_ptr; 
XmDrawnButtonCallbackStruct *reason;
{
   Pixel bg;

   get_something (w,XmNbackground,(void*) &bg);

   if ( bg == apix[BLACK] )
   {
      num_yes_toggle--;
      set_something_val (w,XmNbackground, (XtArgVal) apix[WHITE]);
      jacstruct_ptr->jac_yes = False;
   }
   else
   {
      num_yes_toggle++;
      set_something_val (w,XmNbackground, (XtArgVal) apix[BLACK]);
      jacstruct_ptr->jac_yes = True;
   }
}

/***************************************************************/
/*** crea_jac_eq_labels(indice, posy)
 ***    Parametri:
 ***      int indice : numero equazione
 ***      int posy : posizione y della label
 ***
 ***    Descrizione:
 ***      Crea la label relativa al numero dell'equazione (intestazione righe
 ***      matrice jacobiana).
 ***/
crea_jac_eq_labels(indice, posy)
int indice;
int posy;
{
   Widget wdg;
   char *num_eq[5];

   sprintf (num_eq,"%4d",indice+1);

   cstring = CREATE_CSTRING(num_eq);
   argcount = 0;
   XtSetArg(args[argcount], XmNx, 0); argcount++;
   XtSetArg(args[argcount], XmNy, posy);  argcount++;
   XtSetArg(args[argcount], XmNheight, HEIGHT_JAC_TOGGLES); argcount++;
   XtSetArg(args[argcount], XmNalignment, XmALIGNMENT_END); argcount++;
   XtSetArg(args[argcount], XmNlabelString, cstring); argcount++;
/* XtSetArg(args[argcount], XmNrecomputeSize, False); argcount++; */
   XtSetArg(args[argcount], XmNmarginWidth, 0); argcount++;
   wdg = XmCreateLabel(UxGetWidget(bboard_jac_equ), 
                               "NewmodJacLabel", args, argcount);
   XmStringFree(cstring);

   XtManageChild (wdg);
}



/***************************************************************/
/*** crea_jac_var_labels(indice, posx)
 ***    Parametri:
 ***      int indice : indice nome variabile
 ***      int posy : posizione y della label
 ***
 ***    Descrizione:
 ***      Crea la label relativa al nome della variabile (intestazione colonne
 ***      matrice jacobiana).
 ***/
crea_jac_var_labels(indice, posx)
int indice;
int posx;
{
   Widget    wdg;
   XmString  cstring_var;

   cstring_var = CREATE_CSTRING(variabili[indice].nome);

   argcount = 0;
   XtSetArg(args[argcount], XmNy, 0); argcount++;
   XtSetArg(args[argcount], XmNx, posx);  argcount++;
   XtSetArg(args[argcount], XmNwidth,  WIDTH_JAC_TOGGLES);  argcount++;
   XtSetArg(args[argcount], XmNheight, HEIGHT_JAC_TOGGLES); argcount++;
   XtSetArg(args[argcount], XmNalignment, XmALIGNMENT_CENTER); argcount++;
   XtSetArg(args[argcount], XmNlabelString, cstring_var); argcount++;
   XtSetArg(args[argcount], XmNrecomputeSize, False); argcount++;
   wdg = XmCreateLabel(UxGetWidget(bboard_jac_var), 
                       "NewmodJacLabel", args, argcount);
   XtManageChild (wdg);

   XmStringFree (cstring_var);
}

/***************************************************************/
/*** aggiorna_jac_var_labels()
 ***
 ***   Descrizione:  
 ***      Aggiorna le labelStrings della matrice jacobiana se e' visualizzata.
 ***/
aggiorna_jac_var_labels()
{
   WidgetList wdg; /* Puntatore ad una lista di Widgets */
   XmString cstring_var;
   int indice;

   get_something (UxGetWidget(bboard_jac_var), XmNchildren, (void*) &wdg);

   for (indice=0; indice < jac_cols; indice++)
   {
      cstring_var = CREATE_CSTRING(variabili[indice].nome);

      set_something_val (wdg[indice],XmNlabelString, (XtArgVal) cstring_var);
      XmStringFree (cstring_var);
   }
}

/***************************************************************/
/*** scrolling_bird (sw_row,sw_col,sw_val)
 ***    Parametri:
 ***       Widget sw_row : scroll-list intestazione righe
 ***       Widget sw_col : scroll-list intestazione colonne
 ***       Widget sw_val : scroll-list matrice jacobiana
 ***
 ***    Descrizione:
 ***       Funzione che gestisce lo scrolling unificato della matrice jacobiana.
 ***/
scrolling_bird (sw_row,sw_col,sw_val)
Widget sw_row,sw_col,sw_val;
{
   Widget sbar_cols_hor,   sbar_cols_vert;    /* VARIABILI */
   Widget sbar_rows_hor,   sbar_rows_vert;    /* EQUAZIONI */
   Widget sbar_values_hor, sbar_values_vert;  /* MATRICE  */

   get_something( sw_val, XmNverticalScrollBar,   (void*) &sbar_values_vert );
   get_something( sw_val, XmNhorizontalScrollBar, (void*) &sbar_values_hor );

   get_something( sw_row, XmNverticalScrollBar,   (void*) &sbar_rows_vert );
   get_something( sw_row, XmNhorizontalScrollBar, (void*) &sbar_rows_hor );

   get_something( sw_col, XmNverticalScrollBar,   (void*) &sbar_cols_vert );
   get_something( sw_col, XmNhorizontalScrollBar, (void*) &sbar_cols_hor );

/* Aggiunta callback scrollBar verticali */
   XtAddCallback(sbar_values_vert, XmNincrementCallback, increment,
                 sbar_rows_vert);
   XtAddCallback(sbar_values_vert, XmNdecrementCallback, increment,
                 sbar_rows_vert);
   XtAddCallback(sbar_values_vert, XmNdragCallback, increment,
                 sbar_rows_vert);
   XtAddCallback(sbar_values_vert, XmNpageIncrementCallback, increment,
                 sbar_rows_vert);
   XtAddCallback(sbar_values_vert, XmNpageDecrementCallback, increment,
                 sbar_rows_vert);

/* Aggiunta callback scrollBar orizzontali */
   XtAddCallback(sbar_values_hor, XmNincrementCallback, increment,
                 sbar_cols_hor);
   XtAddCallback(sbar_values_hor, XmNdecrementCallback, increment,
                 sbar_cols_hor);
   XtAddCallback(sbar_values_hor, XmNdragCallback, increment,
                 sbar_cols_hor);
   XtAddCallback(sbar_values_hor, XmNpageIncrementCallback, increment,
                 sbar_cols_hor);
   XtAddCallback(sbar_values_hor, XmNpageDecrementCallback, increment,
                 sbar_cols_hor);

/* Smappazzamento scrollBar */
   XtUnmapWidget( sbar_cols_hor );
   XtUnmapWidget( sbar_cols_vert );
   XtUnmapWidget( sbar_rows_hor );
   XtUnmapWidget( sbar_rows_vert );
}

/***************************************************************/
/*** void increment(w, dato, list_info)
 ***
 ***   Descrizione:
 ***      Gestisce la callback delle scrollbar della scroll-list della 
 ***      matrice jacobiana (muove anche le scroll-list di intestazione). 
 ***/
void increment(w, dato, list_info)
Widget w;
Widget dato;
XmScrollBarCallbackStruct *list_info;
{
    static int i=0;

    if ( !i )
    {
       i++;
       XtCallCallbacks(dato, XmNincrementCallback, list_info);
       i = 0;
    }
    else
    {
       i = 0;
       return;
    }
}

/*-----------------------------------------------------------------------*/
/*** void get_pixel(names,apix,count)
 ***   Parametri:
 ***     char *names: stringhe contenenti i nomi dei colori
 ***     Pixel *apix: array per ospitare i valori di pixel (?)
 ***     int count: contatore ENEL
 ***
 ***   Descrizione: 
 ***     Funzione che recupera i valori RGB dei nomi dei colori specificati. 
 ***/
void get_pixel(names,apix,count)
char *names[];
Pixel *apix;
int count;
{
   XColor defc;
   int i;

   for (i=0 ; i < count ; i++ )
   {
      if ( !XParseColor( display, colormap, names[i], &defc  ))
         fprintf(stderr,"il colore %s non esiste nel database.\n",names[i]);
      else
         if ( !XAllocColor( display, colormap, &defc))
            fprintf(stderr,"non e' possibile allocare il colore %s.\n",
                    names[i]);
         else
            apix[i] = defc.pixel;
   }
}


/*** chiudi_dialog_equation()
 ***
 ***   Descrizione:
 ***     chiude la window di settaggio dello jacobiano
 ***     (viene chiamato quando si preme OK o CANCEL) ***/
chiudi_dialog_equation()
{
   UxDestroySwidget(nmod_dialog_equations);
   bool_dialog_equation = True;
   if ( unlink(file_jac_temp) )
      printf ("Can't delete jacobian temporary file. Failure.\n");

   set_something_val (UxGetWidget(pb_nmod_varsetup),XmNsensitive, (XtArgVal) True);
   if (nmod_def_initialized && jacobian_type == ANALYTICAL)
      set_something_val (UxGetWidget(pb_nmod_JC),XmNsensitive, (XtArgVal) True);
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_nmod_dialog2_ok( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_dialog_equations *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_dialog_equationsContext;
	UxNmod_dialog_equationsContext = UxContext =
			(_UxCnmod_dialog_equations *) UxGetContext( UxWidget );
	{
	salva_jac();  /* salva la situazione attuale. */
	
	non_salvato = True; 
	
	chiudi_dialog_equation();
	}
	UxNmod_dialog_equationsContext = UxSaveCtx;
}

static void	activateCB_nmod_dialog2_cancel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_dialog_equations *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_dialog_equationsContext;
	UxNmod_dialog_equationsContext = UxContext =
			(_UxCnmod_dialog_equations *) UxGetContext( UxWidget );
	{
	leggi_jac();
	
	chiudi_dialog_equation();
	}
	UxNmod_dialog_equationsContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_nmod_dialog_equations()
{

	nmod_dialog_equations = XtVaCreatePopupShell( "nmod_dialog_equations",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNgeometry, "+785+450",
			XmNtitle, "NEWMOD - JACOBIAN MATRIX TOPOLOGY",
			XmNheight, 60,
			XmNwidth, 80,
			XmNy, 110,
			XmNx, 820,
			NULL );

	UxPutContext( nmod_dialog_equations, (char *) UxNmod_dialog_equationsContext );

	nmod_dialog2_form1 = XtVaCreateWidget( "nmod_dialog2_form1",
			xmFormWidgetClass, nmod_dialog_equations,
			XmNautoUnmanage, FALSE,
			XmNheight, 430,
			XmNwidth, 600,
			XmNy, 386,
			XmNx, 201,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( nmod_dialog2_form1, (char *) UxNmod_dialog_equationsContext );

	nmod_dialog2_ok = XtVaCreateManagedWidget( "nmod_dialog2_ok",
			xmPushButtonWidgetClass, nmod_dialog2_form1,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 20,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 100,
			XmNy, 510,
			XmNx, 10,
			NULL );

	UxPutContext( nmod_dialog2_ok, (char *) UxNmod_dialog_equationsContext );

	nmod_dialog2_sep1 = XtVaCreateManagedWidget( "nmod_dialog2_sep1",
			xmSeparatorGadgetClass, nmod_dialog2_form1,
			XmNbottomWidget, nmod_dialog2_ok,
			XmNbottomOffset, 20,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNtopWidget, NULL,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 10,
			XmNwidth, 660,
			XmNy, 470,
			XmNx, 10,
			NULL );

	UxPutContext( nmod_dialog2_sep1, (char *) UxNmod_dialog_equationsContext );

	labelGadget6 = XtVaCreateManagedWidget( "labelGadget6",
			xmLabelGadgetClass, nmod_dialog2_form1,
			RES_CONVERT( XmNlabelString, "Variables" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_NONE,
			XmNbottomOffset, 0,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNheight, 20,
			XmNwidth, 200,
			XmNy, 0,
			XmNx, 90,
			NULL );

	UxPutContext( labelGadget6, (char *) UxNmod_dialog_equationsContext );

	scrollw_jac_var = XtVaCreateManagedWidget( "scrollw_jac_var",
			xmScrolledWindowWidgetClass, nmod_dialog2_form1,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmCONSTANT,
			XmNtopWidget, labelGadget6,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftWidget, NULL,
			XmNleftOffset, 80,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 0,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNheight, 60,
			XmNwidth, 290,
			XmNy, 30,
			XmNx, 100,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( scrollw_jac_var, (char *) UxNmod_dialog_equationsContext );

	bboard_jac_var = XtVaCreateManagedWidget( "bboard_jac_var",
			xmBulletinBoardWidgetClass, scrollw_jac_var,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNnoResize, TRUE,
			XmNheight, 30,
			XmNwidth, 280,
			XmNy, 8,
			XmNx, -2,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( bboard_jac_var, (char *) UxNmod_dialog_equationsContext );

	scrollw_jac_equ = XtVaCreateManagedWidget( "scrollw_jac_equ",
			xmScrolledWindowWidgetClass, nmod_dialog2_form1,
			XmNtopWidget, NULL,
			XmNtopOffset, 100,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomWidget, nmod_dialog2_sep1,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNshadowThickness, 2,
			XmNscrolledWindowMarginWidth, 0,
			XmNscrolledWindowMarginHeight, 0,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmAS_NEEDED,
			XmNheight, 230,
			XmNwidth, 60,
			XmNy, 180,
			XmNx, 70,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( scrollw_jac_equ, (char *) UxNmod_dialog_equationsContext );

	bboard_jac_equ = XtVaCreateManagedWidget( "bboard_jac_equ",
			xmBulletinBoardWidgetClass, scrollw_jac_equ,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNnoResize, TRUE,
			XmNheight, 240,
			XmNwidth, 320,
			XmNy, -2,
			XmNx, -2,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( bboard_jac_equ, (char *) UxNmod_dialog_equationsContext );

	labelGadget8 = XtVaCreateManagedWidget( "labelGadget8",
			xmLabelGadgetClass, nmod_dialog2_form1,
			XmNbottomWidget, scrollw_jac_equ,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Equations" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 80,
			XmNy, 40,
			XmNx, 10,
			NULL );

	UxPutContext( labelGadget8, (char *) UxNmod_dialog_equationsContext );

	nmod_dialog2_scrollw3 = XtVaCreateManagedWidget( "nmod_dialog2_scrollw3",
			xmScrolledWindowWidgetClass, nmod_dialog2_form1,
			XmNtopWidget, NULL,
			XmNtopOffset, 100,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftWidget, scrollw_jac_equ,
			XmNleftOffset, 80,
			XmNleftAttachment, XmATTACH_FORM,
			XmNshadowThickness, 2,
			XmNscrolledWindowMarginWidth, 0,
			XmNscrolledWindowMarginHeight, 0,
			XmNvisualPolicy, XmCONSTANT,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomWidget, nmod_dialog2_sep1,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 230,
			XmNwidth, 560,
			XmNy, 180,
			XmNx, 70,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( nmod_dialog2_scrollw3, (char *) UxNmod_dialog_equationsContext );

	bboard_jac_toggles = XtVaCreateManagedWidget( "bboard_jac_toggles",
			xmBulletinBoardWidgetClass, nmod_dialog2_scrollw3,
			XmNnoResize, TRUE,
			XmNheight, 240,
			XmNwidth, 320,
			XmNy, -2,
			XmNx, -2,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( bboard_jac_toggles, (char *) UxNmod_dialog_equationsContext );

	nmod_dialog2_cancel = XtVaCreateManagedWidget( "nmod_dialog2_cancel",
			xmPushButtonWidgetClass, nmod_dialog2_form1,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNbottomOffset, 20,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 100,
			XmNy, 510,
			XmNx, 130,
			NULL );

	UxPutContext( nmod_dialog2_cancel, (char *) UxNmod_dialog_equationsContext );

	label1 = XtVaCreateManagedWidget( "label1",
			xmLabelWidgetClass, nmod_dialog2_form1,
			XmNresizable, FALSE,
			XmNborderWidth, 1,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrecomputeSize, FALSE,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNlabelString, "No" ),
			RES_CONVERT( XmNforeground, "black" ),
			XmNbottomOffset, 20,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "white" ),
			XmNheight, 30,
			XmNwidth, 50,
			XmNy, 380,
			XmNx, 520,
			NULL );

	UxPutContext( label1, (char *) UxNmod_dialog_equationsContext );

	label2 = XtVaCreateManagedWidget( "label2",
			xmLabelWidgetClass, nmod_dialog2_form1,
			XmNborderWidth, 1,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightWidget, label1,
			XmNrightOffset, 15,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNresizable, FALSE,
			XmNrecomputeSize, FALSE,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNlabelString, "Yes" ),
			RES_CONVERT( XmNforeground, "white" ),
			XmNbottomOffset, 20,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "black" ),
			XmNheight, 30,
			XmNwidth, 50,
			XmNy, 380,
			XmNx, 440,
			NULL );

	UxPutContext( label2, (char *) UxNmod_dialog_equationsContext );

	XtAddCallback( nmod_dialog_equations, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxNmod_dialog_equationsContext );

	XtAddCallback( nmod_dialog2_ok, XmNactivateCallback,
			activateCB_nmod_dialog2_ok,
			(XtPointer) UxNmod_dialog_equationsContext );

	XtAddCallback( nmod_dialog2_cancel, XmNactivateCallback,
			activateCB_nmod_dialog2_cancel,
			(XtPointer) UxNmod_dialog_equationsContext );



	return ( nmod_dialog_equations );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_dialog_equation()
{
	Widget                  rtrn;
	_UxCnmod_dialog_equations *UxContext;

	UxNmod_dialog_equationsContext = UxContext =
		(_UxCnmod_dialog_equations *) XtMalloc( sizeof(_UxCnmod_dialog_equations) );

	{
		int i,j;
		
		/* Disabilita alcune voci di menu */
		set_something_val (UxGetWidget(pb_nmod_varsetup),XmNsensitive, (XtArgVal) False);
		if (nmod_def_initialized)
		   set_something_val (UxGetWidget(pb_nmod_JC),XmNsensitive, (XtArgVal) False);
		rtrn = _Uxbuild_nmod_dialog_equations();

		/* Dimensionamenti Widgets */
		dimensiona_widgets();
		
		/* CREAZIONE WIDGETS JACOBIANO */
		for (i=0; i < jac_rows; i++ )
		{
		   for (j=0; j < jac_cols; j++ )
		      crea_jac_toggles (&jactoggstruct[i][j],
		                        (MARGIN+ WIDTH_JAC_TOGGLES)*j+MARGIN,
		                        (MARGIN+HEIGHT_JAC_TOGGLES)*i+MARGIN);
		
		  /* RIEMPIMENTO WINDOW NUMERO EQUAZIONI */
		   crea_jac_eq_labels (i,(MARGIN+HEIGHT_JAC_TOGGLES)*i+MARGIN);
		}
		
		/* RIEMPIMENTO WINDOW NOME VARIABILI */
		for (j=0; j < jac_cols; j++ )
		   crea_jac_var_labels (j,(MARGIN+WIDTH_JAC_TOGGLES)*j+MARGIN);
		
		if (prima_volta_jac)
		   prima_volta_jac = False;
		
		/* Assegnazione nome casuale al file tmp */
		tmpnam (file_jac_temp);
		
		/* Scrittura su file stati jacobiano */
		salva_jac();
		
		
		UxPopupInterface (rtrn, no_grab);
		bool_dialog_equation = False;
		
		/* GESTIONE CASSO-INTEGRATA DELLE SCROLLBAR DELL'INTERA WINDOW */
		scrolling_bird (UxGetWidget(scrollw_jac_equ),
		                UxGetWidget(scrollw_jac_var),
		                UxGetWidget(nmod_dialog2_scrollw3));
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_dialog_equation()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_dialog_equation();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

