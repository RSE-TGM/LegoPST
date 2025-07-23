
/*******************************************************************************
	dbox_user_jac.c
	(Generated from interface file dbox_user_jac.i)
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
   modulo dbox_user_jac.i
   tipo 
   release 2.22
   data 5/9/95
   reserved @(#)dbox_user_jac.i	2.22
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

extern StructVars *variabili;
extern StructDati *dati_geom;

extern JacToggStruct jactoggstruct[MAX_EQUAZIONI][MAX_VARIABILI_IN_OUT];
extern int jac_rows, jac_cols;
extern int num_var_stato, num_var_algebriche;

extern int num_yes_toggle;
extern char *str_dichJC, *str_codJC, *str_coefJC[MAX_EQUAZIONI];

extern XmFontList fontList;

/****************************************************************/
/* VARIABILI GLOBALI 						*/
/****************************************************************/

Boolean dbox_userjac_managed = False;

Widget *text_jac_dipend;

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
	Widget	Uxdbox_user_jac;
	Widget	Uxjac_pane;
	Widget	Uxjac_form_1;
	Widget	UxlabelGadget11;
	Widget	UxscrolledWindow7;
	Widget	Uxjac_form_2;
	Widget	UxlabelGadget13;
	Widget	UxscrolledWindow9;
	Widget	Uxjac_form_3;
	Widget	UxlabelGadget12;
	Widget	Uxok_jac_pb;
	Widget	Uxapply_jac_pb;
	Widget	Uxcancel_jac_pb;
	Widget	Uxjac_dipend_scrolledW;
	Widget	Uxjac_dipend_form;
} _UxCdbox_user_jac;

#define dbox_user_jac           UxDbox_user_jacContext->Uxdbox_user_jac
#define jac_pane                UxDbox_user_jacContext->Uxjac_pane
#define jac_form_1              UxDbox_user_jacContext->Uxjac_form_1
#define labelGadget11           UxDbox_user_jacContext->UxlabelGadget11
#define scrolledWindow7         UxDbox_user_jacContext->UxscrolledWindow7
#define jac_form_2              UxDbox_user_jacContext->Uxjac_form_2
#define labelGadget13           UxDbox_user_jacContext->UxlabelGadget13
#define scrolledWindow9         UxDbox_user_jacContext->UxscrolledWindow9
#define jac_form_3              UxDbox_user_jacContext->Uxjac_form_3
#define labelGadget12           UxDbox_user_jacContext->UxlabelGadget12
#define ok_jac_pb               UxDbox_user_jacContext->Uxok_jac_pb
#define apply_jac_pb            UxDbox_user_jacContext->Uxapply_jac_pb
#define cancel_jac_pb           UxDbox_user_jacContext->Uxcancel_jac_pb
#define jac_dipend_scrolledW    UxDbox_user_jacContext->Uxjac_dipend_scrolledW
#define jac_dipend_form         UxDbox_user_jacContext->Uxjac_dipend_form

static _UxCdbox_user_jac	*UxDbox_user_jacContext;

Widget	jac_decl_scrolledText;
Widget	jac_text_scrolledText;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_dbox_userjac();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/***
 *** text_user_jac(form_height)
 ***   Parametri:
 ***     Dimension *form_height : altezza della form (uscita) 
 ***
 ***   Descrizione:
 ***     Compone le righe di label e text per la caratterizzazione
 ***     delle dipendenze dello jacobiano del nuovo modulo (jac_yes=True=BLACK).
 ***     Abilitata dopo le definizioni per il nuovo modulo.
 ***/
text_user_jac (form_height)
Dimension *form_height;
{
   int i=0, j=0, num_resid = 0;
   int i_st, i_alg, i_in;
   Position posy = 10;

   i_st = 0;
   i_alg = num_var_stato; 
   i_in = i_alg + num_var_algebriche;

/* Allocazione di memoria per l'array di text-widget */
   text_jac_dipend = (Widget *) XtCalloc(num_yes_toggle, sizeof(Widget));

/* N.B. Dopo un'apparizione del Divo Silvano mi e' pressoche' chiaro che */
/* accanto alla label "Residual #" va l'INDICE di RIGA della matrice jacob. */
/* mentre accanto a "Variable ABCD ()" va l'INDICE di COLONNA */
   for (i=0; i<jac_rows; i++)
      for (j=0; j<jac_cols; j++)
         if ( jactoggstruct[i][j].jac_yes )
         {
         /* Label AJAC(x,y) */
            crea_labels_jac (i+1,j+1,variabili[j].nome,posy);
            posy += ALTEZZA_LABEL;

         /* Text */
            crea_texts_jac (&text_jac_dipend[num_resid],posy,65,i+1,j+1);

         /* input inserito dall'utente precedentemente */
            if ( jactoggstruct[i][j].stringa != NULL )
               XmTextSetString(text_jac_dipend[num_resid],
                               jactoggstruct[i][j].stringa);

            num_resid++;
            posy += ALTEZZA_LABEL * TEXT_FORTRAN_LINES + 10;
         } /* FINE if JAC_JES */

   *form_height = posy;
}

/*************************************************************/
/***
 *** crea_labels_jac (ind_row,ind_col,nome_var,posy)
 ***   Parametri:
 ***     int ind_row : indice residuo
 ***     int ind_col : indice variabile
 ***     char *nome_var : nome variabile
 ***     Position posy : posizione y della Label
 ***
 ***   Descrizione:
 ***     Crea le label di intestazione degli elementi della matrice jacobiana
 ***     (numero del residuo e nome della variabile)
 ***/
crea_labels_jac (ind_row,ind_col,nome_var,posy)
int ind_row,ind_col;
char *nome_var;
Position posy;
{
   Widget wdg;
   char temp[50];
   XmString cstring;

   sprintf (temp,"Residual # %d  -  Variable %s (%d)",
            ind_row,nome_var,ind_col);
   cstring = CREATE_CSTRING (temp);

/* Creazione label RESIDUAL # x */
   argcount = 0;
   XtSetArg (args[argcount],XmNy,posy); argcount++;
   XtSetArg (args[argcount],XmNheight,ALTEZZA_LABEL); argcount++;
   XtSetArg (args[argcount],XmNlabelString,cstring); argcount++;
   XtSetArg (args[argcount],XmNalignment,XmALIGNMENT_CENTER); argcount++;
   wdg = XmCreateLabel (UxGetWidget(jac_dipend_form),"DboxUserJac",args,
                        argcount);
   XtManageChild (wdg);

   XmStringFree (cstring);
}

/*************************************************************/
/***
 *** crea_texts_jac (wdg,posy,num_col,i,j)
 ***   Parametri:
 ***     Widget *wdg : text-widget creato (uscita)
 ***     Position posy : posizione y del text-widget
 ***     int num_col : numero colonne editabili
 ***     int i, j : indici jacobiano per la stampa 'AJAC(I,J) = '
 ***
 ***   Descrizione:
 ***     Crea i text per l'inserimento del Fortran relativo al valore di 
 ***     alcuni elementi della matrice jacobiana.
 ***     Si tratta di scrolledTextWidget MULTILINE (5 righe) da 65 caratteri
 ***     che devono contenere le istruzioni Fortran da colonna 7 a 72.
 ***     Per le eventuali istruzioni comprese tra la riga 2 e la 5, il
 ***     generatore di Fortran si preoccupa di porre un simbolo di
 ***     continuazione ('$') alla colonna 6.
 ***/
crea_texts_jac (wdg,posy,num_col,i,j)
Widget *wdg;
Position posy;
int num_col;
int i,j;
{
   Widget label_wdg;
   char temp[20];
   XmString cstring;

   sprintf (temp,"AJAC (%d,%d) = ",i,j);
   cstring = CREATE_CSTRING (temp);

/* Creazione label AJAC(x,y) */
   argcount = 0;
   XtSetArg (args[argcount],XmNy,posy); argcount++;
   XtSetArg (args[argcount],XmNheight,ALTEZZA_TEXT); argcount++;
   XtSetArg (args[argcount],XmNlabelString,cstring); argcount++;
   XtSetArg (args[argcount],XmNalignment,XmALIGNMENT_CENTER); argcount++;
   XtSetArg (args[argcount],XmNfontList,fontList); argcount++;
   label_wdg = XmCreateLabel (UxGetWidget(jac_dipend_form),"DboxUserJac",
                         args,argcount);

   XtManageChild (label_wdg);
 
/* Creazione Scrolled Text */
   argcount = 0;
   XtSetArg (args[argcount],XmNheight,ALTEZZA_LABEL*TEXT_FORTRAN_LINES);argcount++;
   XtSetArg (args[argcount],XmNresizeHeight,False);argcount++;
   XtSetArg (args[argcount],XmNresizeWidth, False);argcount++;
   XtSetArg (args[argcount],XmNy,posy); argcount++;
   XtSetArg (args[argcount],XmNleftAttachment,XmATTACH_WIDGET); argcount++;
   XtSetArg (args[argcount],XmNleftWidget,label_wdg); argcount++;
   XtSetArg (args[argcount],XmNrightAttachment,XmATTACH_FORM); argcount++;
   XtSetArg (args[argcount],XmNrightOffset,30); argcount++;
   XtSetArg (args[argcount],XmNcolumns,num_col);argcount++;
   XtSetArg (args[argcount],XmNrows,TEXT_FORTRAN_LINES);argcount++;
   XtSetArg (args[argcount],XmNeditMode,XmMULTI_LINE_EDIT);argcount++;
   XtSetArg (args[argcount],XmNeditable,True);argcount++;
   XtSetArg (args[argcount],XmNfontList,fontList); argcount++;
   *wdg = XmCreateScrolledText(UxGetWidget(jac_dipend_form),"DboxUserJac",
                               args,argcount);
   XtAddCallback(*wdg, XmNmodifyVerifyCallback, text_maius_callback, NULL);
   XtManageChild (*wdg);

   XmStringFree (cstring);
}


/*** memo_textJC(flag)
 ***   Parametri:
 ***     Boolean flag : indica se la window deve sparire o meno.
 ***
 ***   Descrizione:
 ***     memorizza il contenuto dei text-widget della window dei dati nelle
 ***     variabili globali str_dichJC, str_codJC e str_coefJC
 ***/
memo_textJC(flag)
Boolean flag;
{
   int i, j, k;

   non_salvato = True;

/* Dealloca la memoria precedentemente allocata da XmTextGetString() */
   if ( str_dichJC != NULL )
      XtFree( str_dichJC );

   if ( str_codJC != NULL )
      XtFree( str_codJC );

   str_dichJC = XmTextGetString(UxGetWidget(jac_decl_scrolledText));
   str_codJC = XmTextGetString(UxGetWidget(jac_text_scrolledText));

   k = 0;
   for ( i=0 ; i<jac_rows ; i++ )
       for ( j=0 ; j<jac_cols ; j++ )
           if (jactoggstruct[i][j].jac_yes)
           {
              XtFree(jactoggstruct[i][j].stringa);
              jactoggstruct[i][j].stringa=XmTextGetString(text_jac_dipend[k++]);
           }

   if (flag)
   {
      XtFree(text_jac_dipend);
      UxDestroySwidget(dbox_user_jac);
      dbox_userjac_managed = False;
   }
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_ok_jac_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_user_jac       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_user_jacContext;
	UxDbox_user_jacContext = UxContext =
			(_UxCdbox_user_jac *) UxGetContext( UxWidget );
	{
	memo_textJC(True);
	}
	UxDbox_user_jacContext = UxSaveCtx;
}

static void	activateCB_apply_jac_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_user_jac       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_user_jacContext;
	UxDbox_user_jacContext = UxContext =
			(_UxCdbox_user_jac *) UxGetContext( UxWidget );
	memo_textJC(False);
	UxDbox_user_jacContext = UxSaveCtx;
}

static void	activateCB_cancel_jac_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_user_jac       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_user_jacContext;
	UxDbox_user_jacContext = UxContext =
			(_UxCdbox_user_jac *) UxGetContext( UxWidget );
	{
	XtFree(text_jac_dipend);
	UxDestroySwidget(dbox_user_jac);
	dbox_userjac_managed = False;
	}
	UxDbox_user_jacContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_dbox_user_jac()
{

	dbox_user_jac = XtVaCreatePopupShell( "dbox_user_jac",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNtitle, "JC JACOBIAN MATRIX - NEWMOD ",
			XmNgeometry, "+200+250",
			XmNheight, 500,
			XmNwidth, 730,
			XmNy, 550,
			XmNx, 110,
			NULL );

	UxPutContext( dbox_user_jac, (char *) UxDbox_user_jacContext );

	jac_pane = XtVaCreateWidget( "jac_pane",
			xmPanedWindowWidgetClass, dbox_user_jac,
			XmNheight, 450,
			XmNwidth, 694,
			XmNy, 0,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( jac_pane, (char *) UxDbox_user_jacContext );

	jac_form_1 = XtVaCreateManagedWidget( "jac_form_1",
			xmFormWidgetClass, jac_pane,
			XmNheight, 200,
			XmNwidth, 690,
			XmNy, 0,
			XmNx, 3,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( jac_form_1, (char *) UxDbox_user_jacContext );

	labelGadget11 = XtVaCreateManagedWidget( "labelGadget11",
			xmLabelGadgetClass, jac_form_1,
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

	UxPutContext( labelGadget11, (char *) UxDbox_user_jacContext );

	scrolledWindow7 = XtVaCreateManagedWidget( "scrolledWindow7",
			xmScrolledWindowWidgetClass, jac_form_1,
			XmNbottomOffset, 10,
			XmNtopWidget, labelGadget11,
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

	UxPutContext( scrolledWindow7, (char *) UxDbox_user_jacContext );

	jac_decl_scrolledText = XtVaCreateManagedWidget( "jac_decl_scrolledText",
			xmTextWidgetClass, scrolledWindow7,
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

	UxPutContext( jac_decl_scrolledText, (char *) UxDbox_user_jacContext );

	jac_form_2 = XtVaCreateManagedWidget( "jac_form_2",
			xmFormWidgetClass, jac_pane,
			XmNheight, 200,
			XmNwidth, 690,
			XmNy, -10,
			XmNx, -3,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( jac_form_2, (char *) UxDbox_user_jacContext );

	labelGadget13 = XtVaCreateManagedWidget( "labelGadget13",
			xmLabelGadgetClass, jac_form_2,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Text" ),
			XmNbottomAttachment, XmATTACH_NONE,
			XmNheight, 20,
			XmNwidth, 120,
			XmNy, 10,
			XmNx, 30,
			NULL );

	UxPutContext( labelGadget13, (char *) UxDbox_user_jacContext );

	scrolledWindow9 = XtVaCreateManagedWidget( "scrolledWindow9",
			xmScrolledWindowWidgetClass, jac_form_2,
			XmNtopWidget, labelGadget13,
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

	UxPutContext( scrolledWindow9, (char *) UxDbox_user_jacContext );

	jac_text_scrolledText = XtVaCreateManagedWidget( "jac_text_scrolledText",
			xmTextWidgetClass, scrolledWindow9,
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

	UxPutContext( jac_text_scrolledText, (char *) UxDbox_user_jacContext );

	jac_form_3 = XtVaCreateManagedWidget( "jac_form_3",
			xmFormWidgetClass, jac_pane,
			XmNheight, 250,
			XmNwidth, 690,
			XmNy, 211,
			XmNx, 3,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( jac_form_3, (char *) UxDbox_user_jacContext );

	labelGadget12 = XtVaCreateManagedWidget( "labelGadget12",
			xmLabelGadgetClass, jac_form_3,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Jacobian matrix coefficients" ),
			XmNbottomAttachment, XmATTACH_NONE,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 300,
			XmNy, 10,
			XmNx, 20,
			NULL );

	UxPutContext( labelGadget12, (char *) UxDbox_user_jacContext );

	ok_jac_pb = XtVaCreateManagedWidget( "ok_jac_pb",
			xmPushButtonWidgetClass, jac_form_3,
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

	UxPutContext( ok_jac_pb, (char *) UxDbox_user_jacContext );

	apply_jac_pb = XtVaCreateManagedWidget( "apply_jac_pb",
			xmPushButtonWidgetClass, jac_form_3,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftWidget, ok_jac_pb,
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

	UxPutContext( apply_jac_pb, (char *) UxDbox_user_jacContext );

	cancel_jac_pb = XtVaCreateManagedWidget( "cancel_jac_pb",
			xmPushButtonWidgetClass, jac_form_3,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftWidget, apply_jac_pb,
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

	UxPutContext( cancel_jac_pb, (char *) UxDbox_user_jacContext );

	jac_dipend_scrolledW = XtVaCreateManagedWidget( "jac_dipend_scrolledW",
			xmScrolledWindowWidgetClass, jac_form_3,
			XmNborderWidth, 0,
			XmNtopWidget, labelGadget12,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, ok_jac_pb,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNshadowThickness, 2,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 40,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( jac_dipend_scrolledW, (char *) UxDbox_user_jacContext );

	jac_dipend_form = XtVaCreateManagedWidget( "jac_dipend_form",
			xmFormWidgetClass, jac_dipend_scrolledW,
			XmNheight, 260,
			XmNwidth, 650,
			XmNy, -1,
			XmNx, -1,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( jac_dipend_form, (char *) UxDbox_user_jacContext );

	XtAddCallback( dbox_user_jac, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxDbox_user_jacContext );

	XtAddCallback( jac_decl_scrolledText, XmNmodifyVerifyCallback,
			text_maius_callback,
			(XtPointer) UxDbox_user_jacContext );

	XtAddCallback( jac_text_scrolledText, XmNmodifyVerifyCallback,
			text_maius_callback,
			(XtPointer) UxDbox_user_jacContext );

	XtAddCallback( ok_jac_pb, XmNactivateCallback,
			activateCB_ok_jac_pb,
			(XtPointer) UxDbox_user_jacContext );

	XtAddCallback( apply_jac_pb, XmNactivateCallback,
			activateCB_apply_jac_pb,
			(XtPointer) UxDbox_user_jacContext );

	XtAddCallback( cancel_jac_pb, XmNactivateCallback,
			activateCB_cancel_jac_pb,
			(XtPointer) UxDbox_user_jacContext );



	return ( dbox_user_jac );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_dbox_userjac()
{
	Widget                  rtrn;
	_UxCdbox_user_jac       *UxContext;

	UxDbox_user_jacContext = UxContext =
		(_UxCdbox_user_jac *) XtMalloc( sizeof(_UxCdbox_user_jac) );

	{
		Dimension height;
		rtrn = _Uxbuild_dbox_user_jac();

		/* Visualizzazione text per l'input */
		text_user_jac(&height);
		
		UxPopupInterface (rtrn, no_grab);
		dbox_userjac_managed = True;
		
		/* Dimensionamento dell'altezza della form dei text */
		set_something_val (UxGetWidget(jac_dipend_form),XmNheight, (XtArgVal) height);
		
		if ( str_dichJC != NULL )
		   XmTextSetString(UxGetWidget(jac_decl_scrolledText),str_dichJC);
		
		if ( str_codJC != NULL )
		   XmTextSetString(UxGetWidget(jac_text_scrolledText),str_codJC);
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_dbox_userjac()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_dbox_userjac();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

