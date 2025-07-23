
/*******************************************************************************
	dbox_user_resid.c
	(Generated from interface file dbox_user_resid.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/Text.h>
#include <Xm/ScrolledW.h>
#include <Xm/LabelG.h>
#include <Xm/Form.h>
#include <Xm/PanedW.h>
#include <Xm/DialogS.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo dbox_user_resid.i
   tipo 
   release 2.22
   data 5/9/95
   reserved @(#)dbox_user_resid.i	2.22
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
extern XmFontList fontList;

extern num_var_stato, num_var_algebriche;
extern char *str_dichMOD, *str_codMOD, *str_resMOD[];

/****************************************************************/
/* VARIABILI GLOBALI 						*/
/****************************************************************/

Boolean dbox_userresid_managed = False;

Widget *text_residui;

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
	Widget	Uxdbox_user_resid;
	Widget	Uxresid_pane;
	Widget	Uxresid_form_1;
	Widget	UxlabelGadget14;
	Widget	UxscrolledWindow8;
	Widget	Uxresid_form_2;
	Widget	UxlabelGadget15;
	Widget	UxscrolledWindow10;
	Widget	Uxresid_form_3;
	Widget	UxlabelGadget16;
	Widget	Uxok_resid_pb;
	Widget	Uxapply_resid_pb;
	Widget	Uxcancel_resid_pb;
	Widget	UxscrolledWindow12;
	Widget	Uxresid_resid_form;
} _UxCdbox_user_resid;

#define dbox_user_resid         UxDbox_user_residContext->Uxdbox_user_resid
#define resid_pane              UxDbox_user_residContext->Uxresid_pane
#define resid_form_1            UxDbox_user_residContext->Uxresid_form_1
#define labelGadget14           UxDbox_user_residContext->UxlabelGadget14
#define scrolledWindow8         UxDbox_user_residContext->UxscrolledWindow8
#define resid_form_2            UxDbox_user_residContext->Uxresid_form_2
#define labelGadget15           UxDbox_user_residContext->UxlabelGadget15
#define scrolledWindow10        UxDbox_user_residContext->UxscrolledWindow10
#define resid_form_3            UxDbox_user_residContext->Uxresid_form_3
#define labelGadget16           UxDbox_user_residContext->UxlabelGadget16
#define ok_resid_pb             UxDbox_user_residContext->Uxok_resid_pb
#define apply_resid_pb          UxDbox_user_residContext->Uxapply_resid_pb
#define cancel_resid_pb         UxDbox_user_residContext->Uxcancel_resid_pb
#define scrolledWindow12        UxDbox_user_residContext->UxscrolledWindow12
#define resid_resid_form        UxDbox_user_residContext->Uxresid_resid_form

static _UxCdbox_user_resid	*UxDbox_user_residContext;

Widget	resid_decl_scrolledText;
Widget	resid_code_scrolledText;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_dbox_userresid();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*** text_user_resid (form_height)
 ***   Parametri:
 ***     Dimension *form_height: dimensione della form in altezza (uscita)
 ***
 ***   Descrizione:
 ***     Compone le righe di label e text per la caratterizzazione
 ***     dei residui delle equazioni del nuovo modulo.
 ***     Abilitata dopo le definizioni per il nuovo modulo.
 ***/
text_user_resid (form_height)
Dimension *form_height;
{
   int i;
   Position posy = 10;

/* Allocazione della memoria per i text-widget dei residui */
   text_residui = (Widget *) XtCalloc(num_var_stato+num_var_algebriche,
				      sizeof(Widget));

/**********************************************************************/
/* Riempimento righe residui equazioni di uscita (stato e algebriche) */
/**********************************************************************/
   for (i=0; i<num_var_stato+num_var_algebriche; i++)
   {
   /* Label RN ( ) */
      crea_label_resid (i+1,1,posy);

   /* Text Residui */
      crea_texts_resid (&text_residui[i],15,posy,65);
      if (str_resMOD[i] != NULL)
         XmTextSetString(text_residui[i],str_resMOD[i]);

      posy += ALTEZZA_LABEL * TEXT_FORTRAN_LINES + 10;
   }

   *form_height = posy;
}

/*************************************************************/
/***
 *** crea_label_resid (num_resid,leftPos,posy)
 ***   Parametri:
 ***      int num_resid : indice del residuo
 ***      int leftPos : left position
 ***      Position posy : posizione y della label
 ***
 ***   Descrizione:
 ***      Crea la label di intestazione dei residui.
 ***
 ***/
crea_label_resid (num_resid,leftPos,posy)
int num_resid;
int leftPos;
Position posy;
{
   Widget wdg;
   char temp[20];
   XmString cstring;

   sprintf (temp,"RN (%d) = ",num_resid);
   cstring = CREATE_CSTRING (temp);

   argcount = 0;
   XtSetArg (args[argcount],XmNy,posy); argcount++;
   XtSetArg (args[argcount],XmNheight,ALTEZZA_TEXT); argcount++;
   XtSetArg (args[argcount],XmNleftAttachment,XmATTACH_POSITION); argcount++;
   XtSetArg (args[argcount],XmNleftPosition,leftPos); argcount++;
   XtSetArg (args[argcount],XmNlabelString,cstring); argcount++;
   XtSetArg (args[argcount],XmNalignment,XmALIGNMENT_BEGINNING); argcount++;
   wdg = XmCreateLabel (UxGetWidget(resid_resid_form),"DboxUserResid",
                         args,argcount);
   XtManageChild (wdg);

   XmStringFree (cstring);
}

/*************************************************************/
/***
 *** crea_texts_resid (wdg,leftPos,posy,num_col)
 ***   Parametri:
 ***     Widget *wdg : text-widget creato (uscita)
 ***     int leftPos : left position
 ***     Position posy : posizione y del text-widget.
 ***     int num_col : numero colonne del text-widget.
 ***
 ***   Descrizione:
 ***     Crea i text per l'inserimento del Fortran relativo ai residui
 ***     delle equazioni.
 ***     Si tratta di scrolledTextWidget MULTILINE (5 righe) da 65 caratteri
 ***     che devono contenere le istruzioni Fortran da colonna 7 a 72.
 ***     Per le eventuali istruzioni comprese tra la riga 2 e la 5, il
 ***     generatore di Fortran si preoccupa di porre un simbolo di
 ***     continuazione ('$') a colonna 6.
 ***/
crea_texts_resid (wdg,leftPos,posy,num_col)
Widget *wdg;
int leftPos;
Position posy;
int num_col;
{
   argcount = 0;
   XtSetArg (args[argcount],XmNheight,ALTEZZA_LABEL*TEXT_FORTRAN_LINES);argcount++;
   XtSetArg (args[argcount],XmNresizeHeight,False);argcount++;
   XtSetArg (args[argcount],XmNresizeWidth, False);argcount++;
   XtSetArg (args[argcount],XmNy,posy); argcount++;
   XtSetArg (args[argcount],XmNcolumns,num_col);argcount++;
   XtSetArg (args[argcount],XmNrows,TEXT_FORTRAN_LINES);argcount++;
   XtSetArg (args[argcount],XmNeditMode,XmMULTI_LINE_EDIT);argcount++;
   XtSetArg (args[argcount],XmNeditable,True);argcount++;
   XtSetArg (args[argcount],XmNleftAttachment,XmATTACH_POSITION); argcount++;
   XtSetArg (args[argcount],XmNleftPosition,leftPos); argcount++;
   XtSetArg (args[argcount],XmNrightAttachment,XmATTACH_FORM); argcount++;
   XtSetArg (args[argcount],XmNrightOffset,10); argcount++;
   XtSetArg (args[argcount],XmNfontList,fontList); argcount++;
   *wdg = XmCreateScrolledText (UxGetWidget(resid_resid_form),"DboxUserEq",
                                args,argcount);
   XtAddCallback(*wdg, XmNmodifyVerifyCallback, text_maius_callback, NULL);
   XtManageChild (*wdg);
}

/*** memo_textMOD(flag)
 ***   Parametri:
 ***     Boolean flag : indica se la window deve sparire o meno.
 ***
 ***   Descrizione:
 ***     memorizza il contenuto dei text-widget della window dei dati nelle
 ***     variabili globali str_dichMOD, str_codMOD e str_resMOD
 ***/
memo_textMOD(flag)
Boolean flag;
{
   int i;

   non_salvato = True;

/* Dealloca la memoria precedentemente allocata da XmTextGetString() */
   XtFree( str_dichMOD );
   XtFree( str_codMOD );

   str_dichMOD = XmTextGetString(UxGetWidget(resid_decl_scrolledText));
   str_codMOD = XmTextGetString(UxGetWidget(resid_code_scrolledText));

   for ( i=0 ; i<num_var_stato+num_var_algebriche ; i++ )
   {
       XtFree(str_resMOD[i]);
       str_resMOD[i]=XmTextGetString(text_residui[i]);
   }

   if (flag)
   {
      UxDestroySwidget(dbox_user_resid);
      dbox_userresid_managed = False;    
      XtFree(text_residui);
   }
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_ok_resid_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_user_resid     *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_user_residContext;
	UxDbox_user_residContext = UxContext =
			(_UxCdbox_user_resid *) UxGetContext( UxWidget );
	memo_textMOD(True);
	UxDbox_user_residContext = UxSaveCtx;
}

static void	activateCB_apply_resid_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_user_resid     *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_user_residContext;
	UxDbox_user_residContext = UxContext =
			(_UxCdbox_user_resid *) UxGetContext( UxWidget );
	memo_textMOD(False);
	UxDbox_user_residContext = UxSaveCtx;
}

static void	activateCB_cancel_resid_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_user_resid     *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_user_residContext;
	UxDbox_user_residContext = UxContext =
			(_UxCdbox_user_resid *) UxGetContext( UxWidget );
	{
	UxDestroySwidget(dbox_user_resid);
	dbox_userresid_managed = False;    
	XtFree(text_residui);
	}
	UxDbox_user_residContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_dbox_user_resid()
{

	dbox_user_resid = XtVaCreatePopupShell( "dbox_user_resid",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNtitle, "RESIDUAL SECTION - NEWMOD ",
			XmNgeometry, "+230+280",
			XmNheight, 750,
			XmNwidth, 700,
			XmNy, 560,
			XmNx, 150,
			NULL );

	UxPutContext( dbox_user_resid, (char *) UxDbox_user_residContext );

	resid_pane = XtVaCreateWidget( "resid_pane",
			xmPanedWindowWidgetClass, dbox_user_resid,
			XmNheight, 450,
			XmNwidth, 694,
			XmNy, 0,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( resid_pane, (char *) UxDbox_user_residContext );

	resid_form_1 = XtVaCreateManagedWidget( "resid_form_1",
			xmFormWidgetClass, resid_pane,
			XmNheight, 200,
			XmNwidth, 690,
			XmNy, 0,
			XmNx, 3,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( resid_form_1, (char *) UxDbox_user_residContext );

	labelGadget14 = XtVaCreateManagedWidget( "labelGadget14",
			xmLabelGadgetClass, resid_form_1,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNlabelString, "Declarations" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 150,
			XmNy, 0,
			XmNx, 20,
			NULL );

	UxPutContext( labelGadget14, (char *) UxDbox_user_residContext );

	scrolledWindow8 = XtVaCreateManagedWidget( "scrolledWindow8",
			xmScrolledWindowWidgetClass, resid_form_1,
			XmNbottomOffset, 10,
			XmNtopWidget, labelGadget14,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 30,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow8, (char *) UxDbox_user_residContext );

	resid_decl_scrolledText = XtVaCreateManagedWidget( "resid_decl_scrolledText",
			xmTextWidgetClass, scrolledWindow8,
			XmNfontList, UxConvertFontList( "-Adobe-Courier-Bold-R-Normal--14-140-75-75-M-90-ISO8859-1" ),
			XmNresizeWidth, TRUE,
			XmNresizeHeight, TRUE,
			XmNeditMode, XmMULTI_LINE_EDIT ,
			XmNcolumns, 80,
			XmNy, 0,
			XmNx, 0,
			XmNheight, 160,
			XmNwidth, 680,
			NULL );

	UxPutContext( resid_decl_scrolledText, (char *) UxDbox_user_residContext );

	resid_form_2 = XtVaCreateManagedWidget( "resid_form_2",
			xmFormWidgetClass, resid_pane,
			XmNheight, 200,
			XmNwidth, 690,
			XmNy, -10,
			XmNx, -3,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( resid_form_2, (char *) UxDbox_user_residContext );

	labelGadget15 = XtVaCreateManagedWidget( "labelGadget15",
			xmLabelGadgetClass, resid_form_2,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "User code" ),
			XmNbottomAttachment, XmATTACH_NONE,
			XmNheight, 20,
			XmNwidth, 120,
			XmNy, 10,
			XmNx, 30,
			NULL );

	UxPutContext( labelGadget15, (char *) UxDbox_user_residContext );

	scrolledWindow10 = XtVaCreateManagedWidget( "scrolledWindow10",
			xmScrolledWindowWidgetClass, resid_form_2,
			XmNtopWidget, labelGadget15,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 30,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow10, (char *) UxDbox_user_residContext );

	resid_code_scrolledText = XtVaCreateManagedWidget( "resid_code_scrolledText",
			xmTextWidgetClass, scrolledWindow10,
			XmNfontList, UxConvertFontList( "-Adobe-Courier-Bold-R-Normal--14-140-75-75-M-90-ISO8859-1" ),
			XmNresizeWidth, TRUE,
			XmNresizeHeight, TRUE,
			XmNeditMode, XmMULTI_LINE_EDIT ,
			XmNcolumns, 80,
			XmNy, 0,
			XmNx, 0,
			XmNheight, 160,
			XmNwidth, 680,
			NULL );

	UxPutContext( resid_code_scrolledText, (char *) UxDbox_user_residContext );

	resid_form_3 = XtVaCreateManagedWidget( "resid_form_3",
			xmFormWidgetClass, resid_pane,
			XmNheight, 250,
			XmNwidth, 690,
			XmNy, 211,
			XmNx, 3,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( resid_form_3, (char *) UxDbox_user_residContext );

	labelGadget16 = XtVaCreateManagedWidget( "labelGadget16",
			xmLabelGadgetClass, resid_form_3,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Residuals" ),
			XmNbottomAttachment, XmATTACH_NONE,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 300,
			XmNy, 10,
			XmNx, 20,
			NULL );

	UxPutContext( labelGadget16, (char *) UxDbox_user_residContext );

	ok_resid_pb = XtVaCreateManagedWidget( "ok_resid_pb",
			xmPushButtonWidgetClass, resid_form_3,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 100,
			XmNy, 300,
			XmNx, 20,
			NULL );

	UxPutContext( ok_resid_pb, (char *) UxDbox_user_residContext );

	apply_resid_pb = XtVaCreateManagedWidget( "apply_resid_pb",
			xmPushButtonWidgetClass, resid_form_3,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftWidget, ok_resid_pb,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Apply" ),
			XmNheight, 28,
			XmNwidth, 90,
			XmNy, 300,
			XmNx, 140,
			NULL );

	UxPutContext( apply_resid_pb, (char *) UxDbox_user_residContext );

	cancel_resid_pb = XtVaCreateManagedWidget( "cancel_resid_pb",
			xmPushButtonWidgetClass, resid_form_3,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftWidget, apply_resid_pb,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNheight, 28,
			XmNwidth, 90,
			XmNy, 300,
			XmNx, 250,
			NULL );

	UxPutContext( cancel_resid_pb, (char *) UxDbox_user_residContext );

	scrolledWindow12 = XtVaCreateManagedWidget( "scrolledWindow12",
			xmScrolledWindowWidgetClass, resid_form_3,
			XmNtopWidget, labelGadget16,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, ok_resid_pb,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 420,
			XmNwidth, 660,
			XmNy, 50,
			XmNx, 20,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( scrolledWindow12, (char *) UxDbox_user_residContext );

	resid_resid_form = XtVaCreateManagedWidget( "resid_resid_form",
			xmFormWidgetClass, scrolledWindow12,
			XmNheight, 200,
			XmNwidth, 630,
			XmNy, -2,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( resid_resid_form, (char *) UxDbox_user_residContext );

	XtAddCallback( dbox_user_resid, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxDbox_user_residContext );

	XtAddCallback( resid_decl_scrolledText, XmNmodifyVerifyCallback,
			text_maius_callback,
			(XtPointer) UxDbox_user_residContext );

	XtAddCallback( resid_code_scrolledText, XmNmodifyVerifyCallback,
			text_maius_callback,
			(XtPointer) UxDbox_user_residContext );

	XtAddCallback( ok_resid_pb, XmNactivateCallback,
			activateCB_ok_resid_pb,
			(XtPointer) UxDbox_user_residContext );

	XtAddCallback( apply_resid_pb, XmNactivateCallback,
			activateCB_apply_resid_pb,
			(XtPointer) UxDbox_user_residContext );

	XtAddCallback( cancel_resid_pb, XmNactivateCallback,
			activateCB_cancel_resid_pb,
			(XtPointer) UxDbox_user_residContext );



	return ( dbox_user_resid );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_dbox_userresid()
{
	Widget                  rtrn;
	_UxCdbox_user_resid     *UxContext;

	UxDbox_user_residContext = UxContext =
		(_UxCdbox_user_resid *) XtMalloc( sizeof(_UxCdbox_user_resid) );

	{
		Dimension height;
		rtrn = _Uxbuild_dbox_user_resid();

		/* Visualizzazione text per l'input */
		text_user_resid (&height);
		
		UxPopupInterface (rtrn, no_grab);
		dbox_userresid_managed = True;
		
		/* Dimensionamento dell'altezza della form dei text */
		set_something_val (UxGetWidget(resid_resid_form),XmNheight, (XtArgVal) height);
		
		/* Aggiorna i text dichiarazione e codice */
		if ( str_dichMOD != NULL )
		   XmTextSetString(UxGetWidget(resid_decl_scrolledText),str_dichMOD);
		
		if ( str_codMOD != NULL )
		   XmTextSetString(UxGetWidget(resid_code_scrolledText),str_codMOD);
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_dbox_userresid()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_dbox_userresid();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

