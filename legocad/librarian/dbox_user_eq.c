
/*******************************************************************************
	dbox_user_eq.c
	(Generated from interface file dbox_user_eq.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/ScrolledW.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/DialogS.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo dbox_user_eq.i
   tipo 
   release 2.22
   data 5/9/95
   reserved @(#)dbox_user_eq.i	2.22
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

#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/Text.h>

#include "definizioni.h"

#ifndef LIBUTILX
#include <libutilx.h>
#define LIBUTILX
#endif

/****************************************************************/
/* VARIABILI GLOBALI ESTERNE					*/
/****************************************************************/

extern Boolean non_salvato;

extern Arg args[20];
extern Cardinal argcount;

extern Widget UxTopLevel;
extern Dialog_geometry geom_attention;
extern Widget attention_wdg;

extern int num_var_stato;
extern int num_var_algebriche;
extern int num_var_ingresso;
extern char *str_signeq[], *str_uniteq[], *str_cosnor[];

extern XmFontList fontList;

/****************************************************************/
/* VARIABILI GLOBALI      					*/
/****************************************************************/

Boolean dbox_usereq_managed = False;

Widget *text_signeq, *text_uniteq, *text_cosnor;

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
	Widget	Uxdbox_user_eq;
	Widget	Uxform_user_eq;
	Widget	Uxform_labels_eq;
	Widget	Uxdescr_eq_label;
	Widget	Uxunit_eq_label;
	Widget	Uxnorm_eq_label;
	Widget	Uxok_eq_pb;
	Widget	Uxapply_eq_pb;
	Widget	Uxcancel_eq_pb;
	Widget	UxscrolledWindow11;
	Widget	Uxform_eq_scroll;
} _UxCdbox_user_eq;

#define dbox_user_eq            UxDbox_user_eqContext->Uxdbox_user_eq
#define form_user_eq            UxDbox_user_eqContext->Uxform_user_eq
#define form_labels_eq          UxDbox_user_eqContext->Uxform_labels_eq
#define descr_eq_label          UxDbox_user_eqContext->Uxdescr_eq_label
#define unit_eq_label           UxDbox_user_eqContext->Uxunit_eq_label
#define norm_eq_label           UxDbox_user_eqContext->Uxnorm_eq_label
#define ok_eq_pb                UxDbox_user_eqContext->Uxok_eq_pb
#define apply_eq_pb             UxDbox_user_eqContext->Uxapply_eq_pb
#define cancel_eq_pb            UxDbox_user_eqContext->Uxcancel_eq_pb
#define scrolledWindow11        UxDbox_user_eqContext->UxscrolledWindow11
#define form_eq_scroll          UxDbox_user_eqContext->Uxform_eq_scroll

static _UxCdbox_user_eq	*UxDbox_user_eqContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_dbox_usereq();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*** text_user_eq()
 ***
 ***   Descrizione:
 ***     Compone le righe di label e text per la caratterizzazione
 ***     delle equazioni del nuovo modulo.
 ***     Abilitata dopo le definizioni per il nuovo modulo.
 ***/
text_user_eq()
{
   int i;
   XmString cstring;
   Position posy = 10;

/* Allocazione della memoria per i text-widget relativi a: */
/* significato, unita di misura e costante di normalizzazione delle equazioni */
   text_signeq = (Widget *) XtCalloc(num_var_stato+num_var_algebriche,
                                     sizeof(Widget));
 
   text_uniteq = (Widget *) XtCalloc(num_var_stato+num_var_algebriche,
                                     sizeof(Widget));

   text_cosnor = (Widget *) XtCalloc(num_var_stato+num_var_algebriche,
                                     sizeof(Widget));

/****************************************/
/* Riempimento righe equazioni          */
/****************************************/
   for (i=0; i<num_var_stato+num_var_algebriche; i++)
   {
   /* Label EQUATION... */
      if ( i<num_var_stato )
         crea_label_eq ("STATE EQUATION # ",i+1,1,29,posy);
      else
         crea_label_eq ("ALGEBRAIC EQUATION # ",i+1,1,29,posy);

   /* Text DESCRIPTION */
      crea_texts_eq (&text_signeq[i],30,69,0,posy,50);
      if (str_signeq[i] != NULL)
         XmTextSetString(text_signeq[i], str_signeq[i]);

   /* Text UNIT */
      crea_texts_eq (&text_uniteq[i],70,79,0,posy,6);
      if (str_uniteq[i] != NULL)
         XmTextSetString(text_uniteq[i], str_uniteq[i]);

   /* Text NORMALIZATION... */
      crea_texts_eq (&text_cosnor[i],80,100,10,posy,8);
      if (str_cosnor[i] != NULL)
         XmTextSetString(text_cosnor[i], str_cosnor[i]);

      posy += 35;
   }

   posy += SPAZIO_FRA_SEZIONI;
}

/*************************************************************/
/***
 *** crea_label_eq (stringa,num_eq,leftPos,rightPos,posy)
 ***   Parametri:
 ***      char *stringa : stringa da visualizzare
 ***      int num_eq : numero equazione
 ***      int leftPos : left position
 ***      int rightPos : right position
 ***      Position posy : posizione y della label
 ***
 ***   Descrizione:
 ***      Crea la label per intestare i text relativi al significato delle
 ***      equazioni.
 ***/
crea_label_eq (stringa,num_eq,leftPos,rightPos,posy)
char *stringa;
int num_eq;
int leftPos, rightPos;
Position posy;
{
   Widget wdg;
   char temp[50];
   XmString cstring;

   sprintf (temp,"%s%d",stringa,num_eq);
   cstring = CREATE_CSTRING (temp);

   argcount = 0;
   XtSetArg (args[argcount],XmNy,posy); argcount++;
   XtSetArg (args[argcount],XmNheight,ALTEZZA_TEXT); argcount++;
   XtSetArg (args[argcount],XmNleftAttachment,XmATTACH_POSITION); argcount++;
   XtSetArg (args[argcount],XmNrightAttachment,XmATTACH_POSITION); argcount++;
   XtSetArg (args[argcount],XmNleftPosition,leftPos); argcount++;
   XtSetArg (args[argcount],XmNrightPosition,rightPos); argcount++;
   XtSetArg (args[argcount],XmNlabelString,cstring); argcount++;
   XtSetArg (args[argcount],XmNalignment,XmALIGNMENT_BEGINNING); argcount++;
   wdg = XmCreateLabel (UxGetWidget(form_eq_scroll),"DboxUserEq",
                         args,argcount);
   XtManageChild (wdg);

   XmStringFree (cstring);
}


/*************************************************************/
/***
 *** crea_texts_eq (wdg,leftPos,rightPos,rightOff,posy,num_col)
 ***   Parametri:
 ***      Widget *wdg : text-widget creato
 ***      int leftPos : left position
 ***      int rightPos : right position
 ***      int rightOff : right offset
 ***      Position posy : posizione y della label
 ***      int num_col : numero colonne del text
 ***
 ***   Descrizione:
 ***      Crea i text widget relativi al significato delle equazioni, unita di
 ***      misura e costante di normalizzazione.
 ***/
crea_texts_eq (wdg,leftPos,rightPos,rightOff,posy,num_col)
Widget *wdg;
int leftPos, rightPos, rightOff;
Position posy;
int num_col;
{

   argcount = 0;
   XtSetArg (args[argcount],XmNy,posy); argcount++;
   XtSetArg (args[argcount],XmNheight,ALTEZZA_TEXT); argcount++;
   XtSetArg (args[argcount],XmNleftAttachment,XmATTACH_POSITION); argcount++;
   XtSetArg (args[argcount],XmNrightAttachment,XmATTACH_POSITION); argcount++;
   XtSetArg (args[argcount],XmNleftPosition,leftPos); argcount++;
   XtSetArg (args[argcount],XmNrightPosition,rightPos); argcount++;
   XtSetArg (args[argcount],XmNrightOffset,rightOff); argcount++;
   XtSetArg (args[argcount],XmNmaxLength,num_col);argcount++;
   XtSetArg (args[argcount],XmNfontList,fontList);argcount++;
   *wdg = XmCreateText (UxGetWidget(form_eq_scroll),"DboxUserEq",
                         args,argcount);
   XtAddCallback(*wdg, XmNmodifyVerifyCallback, text_maius_callback, NULL);
   XtManageChild (*wdg);
}

/*------------------------------------------------------------------*/
/*** void cambio_dimensione(w, ev, params, num_params)
 *** Ad ogni ridimensionamento della finestra, legge le nuove
 *** dimensioni (LARGHEZZA) della form delle label di intestazione
 *** e le passa alla form all'interno della scrolled window dei text.
 ***
 ***/
void cambio_dimensione (w, client_data, ev, boh)
Widget w;
char *client_data;
XEvent *ev;
Boolean boh;
{
   Dimension width;

    get_something( UxGetWidget(form_labels_eq), XmNwidth, (void*) &width );
    set_something_val( UxGetWidget(form_eq_scroll), XmNwidth, (XtArgVal) width-20 );
}


/*** memo_textD1(flag)
 ***   Parametri:
 ***     Boolean flag : indica se la window deve sparire o meno.
memorizza il contenuto dei text-widget della window dei dati nelle
variabili globali str_dichMOD, str_codMOD e str_resMOD */
memo_textD1(flag)
Boolean flag;
{
   int i;

   non_salvato = True;

   for ( i=0 ; i<num_var_stato+num_var_algebriche ; i++ )
   {
       XtFree(str_signeq[i]);
       str_signeq[i]=XmTextGetString(text_signeq[i]);

       XtFree(str_uniteq[i]);
       str_uniteq[i]=XmTextGetString(text_uniteq[i]);

       XtFree(str_cosnor[i]);
       str_cosnor[i]=XmTextGetString(text_cosnor[i]);
   }

   if (flag)
   {
      UxDestroySwidget(dbox_user_eq);
      dbox_usereq_managed = False;    
      XtFree(text_signeq);
      XtFree(text_uniteq);
      XtFree(text_cosnor);
   }
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_ok_eq_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_user_eq        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_user_eqContext;
	UxDbox_user_eqContext = UxContext =
			(_UxCdbox_user_eq *) UxGetContext( UxWidget );
	memo_textD1(True);
	UxDbox_user_eqContext = UxSaveCtx;
}

static void	activateCB_apply_eq_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_user_eq        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_user_eqContext;
	UxDbox_user_eqContext = UxContext =
			(_UxCdbox_user_eq *) UxGetContext( UxWidget );
	memo_textD1(False);
	UxDbox_user_eqContext = UxSaveCtx;
}

static void	activateCB_cancel_eq_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_user_eq        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_user_eqContext;
	UxDbox_user_eqContext = UxContext =
			(_UxCdbox_user_eq *) UxGetContext( UxWidget );
	{
	UxDestroySwidget(dbox_user_eq);
	dbox_usereq_managed = False;
	
	XtFree(text_signeq);
	XtFree(text_uniteq);
	XtFree(text_cosnor);
	}
	UxDbox_user_eqContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_dbox_user_eq()
{

	dbox_user_eq = XtVaCreatePopupShell( "dbox_user_eq",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNtitle, "D1 EQUATIONS SECTION - NEWMOD ",
			XmNheight, 530,
			XmNwidth, 800,
			XmNy, 440,
			XmNx, 160,
			NULL );

	UxPutContext( dbox_user_eq, (char *) UxDbox_user_eqContext );

	form_user_eq = XtVaCreateWidget( "form_user_eq",
			xmFormWidgetClass, dbox_user_eq,
			XmNautoUnmanage, FALSE,
			XmNheight, 530,
			XmNwidth, 800,
			XmNy, 170,
			XmNx, 544,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form_user_eq, (char *) UxDbox_user_eqContext );

	form_labels_eq = XtVaCreateManagedWidget( "form_labels_eq",
			xmFormWidgetClass, form_user_eq,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNheight, 50,
			XmNwidth, 760,
			XmNy, 0,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form_labels_eq, (char *) UxDbox_user_eqContext );

	descr_eq_label = XtVaCreateManagedWidget( "descr_eq_label",
			xmLabelWidgetClass, form_labels_eq,
			XmNleftPosition, 30,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNtopAttachment, XmATTACH_NONE,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			RES_CONVERT( XmNlabelString, "Description" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 130,
			XmNy, 20,
			XmNx, 240,
			NULL );

	UxPutContext( descr_eq_label, (char *) UxDbox_user_eqContext );

	unit_eq_label = XtVaCreateManagedWidget( "unit_eq_label",
			xmLabelWidgetClass, form_labels_eq,
			XmNrightPosition, 79,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNleftPosition, 70,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNtopAttachment, XmATTACH_NONE,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			RES_CONVERT( XmNlabelString, "Unit" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 60,
			XmNy, 20,
			XmNx, 390,
			NULL );

	UxPutContext( unit_eq_label, (char *) UxDbox_user_eqContext );

	norm_eq_label = XtVaCreateManagedWidget( "norm_eq_label",
			xmLabelWidgetClass, form_labels_eq,
			XmNleftPosition, 80,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNtopAttachment, XmATTACH_NONE,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			RES_CONVERT( XmNlabelString, "Normalization \nconstant" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 40,
			XmNwidth, 150,
			XmNy, 20,
			XmNx, 510,
			NULL );

	UxPutContext( norm_eq_label, (char *) UxDbox_user_eqContext );

	ok_eq_pb = XtVaCreateManagedWidget( "ok_eq_pb",
			xmPushButtonWidgetClass, form_user_eq,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 90,
			XmNy, 480,
			XmNx, 20,
			NULL );

	UxPutContext( ok_eq_pb, (char *) UxDbox_user_eqContext );

	apply_eq_pb = XtVaCreateManagedWidget( "apply_eq_pb",
			xmPushButtonWidgetClass, form_user_eq,
			RES_CONVERT( XmNlabelString, "Apply" ),
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftWidget, ok_eq_pb,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 90,
			XmNy, 480,
			XmNx, 130,
			NULL );

	UxPutContext( apply_eq_pb, (char *) UxDbox_user_eqContext );

	cancel_eq_pb = XtVaCreateManagedWidget( "cancel_eq_pb",
			xmPushButtonWidgetClass, form_user_eq,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNleftWidget, apply_eq_pb,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 90,
			XmNy, 480,
			XmNx, 240,
			NULL );

	UxPutContext( cancel_eq_pb, (char *) UxDbox_user_eqContext );

	scrolledWindow11 = XtVaCreateManagedWidget( "scrolledWindow11",
			xmScrolledWindowWidgetClass, form_user_eq,
			XmNtopWidget, form_labels_eq,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, ok_eq_pb,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 420,
			XmNwidth, 660,
			XmNy, 50,
			XmNx, 20,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( scrolledWindow11, (char *) UxDbox_user_eqContext );

	form_eq_scroll = XtVaCreateManagedWidget( "form_eq_scroll",
			xmFormWidgetClass, scrolledWindow11,
			XmNheight, 500,
			XmNwidth, 760,
			XmNy, -2,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form_eq_scroll, (char *) UxDbox_user_eqContext );

	XtAddCallback( dbox_user_eq, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxDbox_user_eqContext );

	XtAddCallback( ok_eq_pb, XmNactivateCallback,
			activateCB_ok_eq_pb,
			(XtPointer) UxDbox_user_eqContext );

	XtAddCallback( apply_eq_pb, XmNactivateCallback,
			activateCB_apply_eq_pb,
			(XtPointer) UxDbox_user_eqContext );

	XtAddCallback( cancel_eq_pb, XmNactivateCallback,
			activateCB_cancel_eq_pb,
			(XtPointer) UxDbox_user_eqContext );



	return ( dbox_user_eq );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_dbox_usereq()
{
	Widget                  rtrn;
	_UxCdbox_user_eq        *UxContext;

	UxDbox_user_eqContext = UxContext =
		(_UxCdbox_user_eq *) XtMalloc( sizeof(_UxCdbox_user_eq) );

	{
		Dimension height;
		rtrn = _Uxbuild_dbox_user_eq();

		/* Visualizzazione text per l'input */
		text_user_eq();
		
		UxPopupInterface (rtrn, no_grab);
		dbox_usereq_managed = True;
		
		/* Controllo per ridimensionamento form delle label */
		/* per l'allineamento con le colonne di text */
		XtAddEventHandler(UxGetWidget(form_labels_eq), StructureNotifyMask, False,
		                  cambio_dimensione, NULL);
		
		/* Dimensionamento dell'altezza della form dei text */
		set_something_val (UxGetWidget(form_eq_scroll),XmNheight,
		               ALTEZZA_TEXT*(num_var_stato+num_var_algebriche)+
		               SPAZIO_FRA_SEZIONI*3);
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_dbox_usereq()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_dbox_usereq();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

