
/*******************************************************************************
	nmod_dialog_config.c
	(Generated from interface file nmod_dialog_config.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/PushB.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/DialogS.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo nmod_dialog_config.i
   tipo 
   release 2.22
   data 5/9/95
   reserved @(#)nmod_dialog_config.i	2.22
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif

/****************************************************************/
/* INCLUDE FILES                                                */
/****************************************************************/

#include <Xm/Xm.h>


#ifndef LIBUTILX
#include <libutilx.h>
#define LIBUTILX
#endif

#include "definizioni.h"

/****************************************************************/
/* VARIABILI GLOBALI ESTERNE                                    */
/****************************************************************/

/* variabili esterne */
extern int num_var_stato, num_var_algebriche;
extern int num_var_ingresso, num_dati_geometrici;

extern Boolean bool_dialog_config;
extern Boolean nmod_def_initialized;

extern int option_scelto;

extern int jac_rows, jac_cols;

extern swidget pb_nmod_save,pb_nmod_jactopology,nmod_bboard,pb_nmod_JC,
               pb_nmod_residual, pb_nmod_D1;

extern Dialog_geometry geom_attention;
extern Widget attention_wdg;

extern char message[];

/********************************************************************/
/* VARIABILI GLOBALI 		                                    */
/********************************************************************/

byte jacobian_type;

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
	Widget	Uxnmod_dialog_config;
	Widget	Uxnmod_dialog_form1;
	Widget	Uxnmod_dialog_label1;
	Widget	Uxnmod_dialog_label2;
	Widget	Uxnmod_dialog_label3;
	Widget	Uxnmod_dialog_label4;
	Widget	Uxnmod_dialog_text1;
	Widget	Uxnmod_dialog_text2;
	Widget	Uxnmod_dialog_text3;
	Widget	Uxnmod_dialog_text4;
	Widget	Uxnmod_dialog_ok;
	Widget	Uxnmod_dialog_cancel;
	Widget	Uxseparator1;
	Widget	Uxnmod_dialog_label5;
	Widget	Uxjac_optMenu_pane;
	Widget	Uxanal_jacMenu_pb;
	Widget	Uxnum_jacMenu_pb;
	Widget	Uxjac_optMenu;
} _UxCnmod_dialog_config;

#define nmod_dialog_config      UxNmod_dialog_configContext->Uxnmod_dialog_config
#define nmod_dialog_form1       UxNmod_dialog_configContext->Uxnmod_dialog_form1
#define nmod_dialog_label1      UxNmod_dialog_configContext->Uxnmod_dialog_label1
#define nmod_dialog_label2      UxNmod_dialog_configContext->Uxnmod_dialog_label2
#define nmod_dialog_label3      UxNmod_dialog_configContext->Uxnmod_dialog_label3
#define nmod_dialog_label4      UxNmod_dialog_configContext->Uxnmod_dialog_label4
#define nmod_dialog_text1       UxNmod_dialog_configContext->Uxnmod_dialog_text1
#define nmod_dialog_text2       UxNmod_dialog_configContext->Uxnmod_dialog_text2
#define nmod_dialog_text3       UxNmod_dialog_configContext->Uxnmod_dialog_text3
#define nmod_dialog_text4       UxNmod_dialog_configContext->Uxnmod_dialog_text4
#define nmod_dialog_ok          UxNmod_dialog_configContext->Uxnmod_dialog_ok
#define nmod_dialog_cancel      UxNmod_dialog_configContext->Uxnmod_dialog_cancel
#define separator1              UxNmod_dialog_configContext->Uxseparator1
#define nmod_dialog_label5      UxNmod_dialog_configContext->Uxnmod_dialog_label5
#define jac_optMenu_pane        UxNmod_dialog_configContext->Uxjac_optMenu_pane
#define anal_jacMenu_pb         UxNmod_dialog_configContext->Uxanal_jacMenu_pb
#define num_jacMenu_pb          UxNmod_dialog_configContext->Uxnum_jacMenu_pb
#define jac_optMenu             UxNmod_dialog_configContext->Uxjac_optMenu

static _UxCnmod_dialog_config	*UxNmod_dialog_configContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_nmod_dialog_config();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*** test_input_setup(text_swdg)
 ***   Parametri:
 ***     swidget text_swdg : text input
Controlla l'input nella window di setup. Ritorna il numero positivo se
tutto e' corretto, altrimenti ritorna -1 */
test_input_setup(text_swdg)
swidget text_swdg;
{
   char *str_val;
   int valore, last_pos;

   str_val = XmTextGetString(UxGetWidget(text_swdg));
   if (!IsNumeric(str_val))  
   {
      scrivi_messaggio("Invalid setup value. Modify to continue.");
      last_pos = XmTextGetLastPosition (UxGetWidget(text_swdg));
      XmTextSetSelection (UxGetWidget(text_swdg),0,last_pos,CurrentTime);
      return(-1);
   }
   sscanf(str_val,"%3d",&valore);
   XtFree(str_val);

   if (valore < 0)
   {
      scrivi_messaggio("Negative input value. Modify to continue.");
      last_pos = XmTextGetLastPosition (UxGetWidget(text_swdg));
      XmTextSetSelection (UxGetWidget(text_swdg),0,last_pos,CurrentTime);
      return(-1);
   }
   return(valore);
}

/*** chiudi_dlg_config()
 *** chiude la window di configurazione delle variabili (OK e CANCEL) ***/
chiudi_dlg_config()
{
/* cancella la window */
   UxDestroySwidget(nmod_dialog_config);

/* riabilita le voci di menu se e' il caso */
   if (nmod_def_initialized)
   {
      set_something_val(UxGetWidget(pb_nmod_jactopology),XmNsensitive, (XtArgVal) True);
      set_something_val(UxGetWidget(pb_nmod_JC),XmNsensitive, (XtArgVal) True);
      set_something_val(UxGetWidget(pb_nmod_residual),XmNsensitive, (XtArgVal) True);
      set_something_val(UxGetWidget(pb_nmod_D1),XmNsensitive, (XtArgVal) True);
   }

   bool_dialog_config = True;
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_nmod_dialog_ok( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_dialog_config  *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_dialog_configContext;
	UxNmod_dialog_configContext = UxContext =
			(_UxCnmod_dialog_config *) UxGetContext( UxWidget );
	{
	char *value;
	int num_st, num_alg, num_in, num_dati;
	int last_pos;
	
	if ( (num_st = test_input_setup(nmod_dialog_text1)) == -1)
	   return;
	
	if ( (num_alg = test_input_setup(nmod_dialog_text2)) == -1)
	   return;
	
	if ( (num_in = test_input_setup(nmod_dialog_text3)) == -1)
	   return;
	
	if ( (num_dati = test_input_setup(nmod_dialog_text4)) == -1)
	   return;
	
	if ( num_st + num_alg > MAX_VARIABILI_USCITA )
	{
	   sprintf(message,
	           "The number of output variable must be less or equal than %d. Modify to continue.",
	           MAX_VARIABILI_USCITA);
	   scrivi_messaggio(message);
	   attention_wdg = (Widget) attention(UxTopLevel,message,MAPPA,geom_attention);
	   last_pos = XmTextGetLastPosition (UxGetWidget(nmod_dialog_text1));
	   XmTextSetSelection (UxGetWidget(nmod_dialog_text1),0,last_pos,CurrentTime);
	   return;
	}
	
	if ( num_in > MAX_VARIABILI_INGRESSO )
	{
	   sprintf(message,
	           "The number of input variable must be less or equal than %d. Modify to continue.",
	            MAX_VARIABILI_INGRESSO);
	   scrivi_messaggio(message);
	   attention_wdg = (Widget) attention(UxTopLevel,message,MAPPA,geom_attention);
	   last_pos = XmTextGetLastPosition (UxGetWidget(nmod_dialog_text3));
	   XmTextSetSelection (UxGetWidget(nmod_dialog_text3),0,last_pos,CurrentTime);
	   return;
	}
	
	if ( num_dati > MAX_DATI_GEOMETRICI )
	{
	   sprintf(message,
	           "The number of geometrical data must be less or equal than %d. Modify to continue.",
	           MAX_DATI_GEOMETRICI);
	   scrivi_messaggio(message);
	   attention_wdg = (Widget) attention(UxTopLevel,message,MAPPA,geom_attention);
	   last_pos = XmTextGetLastPosition (UxGetWidget(nmod_dialog_text4));
	   XmTextSetSelection (UxGetWidget(nmod_dialog_text4),0,last_pos,CurrentTime);
	   return;
	}
	chiudi_dlg_config();
	
	/* Adesso l'utente deve confermare le nuove informazioni. Quindi le voci di */
	/* menu (jacobiano, residui, ecc..) sono disabilitate */
	menu_newmod_init();
	
	alloca_text_widget(UxGetWidget(nmod_bboard),num_st, num_alg, num_in, num_dati);
	}
	UxNmod_dialog_configContext = UxSaveCtx;
}

static void	activateCB_nmod_dialog_cancel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_dialog_config  *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_dialog_configContext;
	UxNmod_dialog_configContext = UxContext =
			(_UxCnmod_dialog_config *) UxGetContext( UxWidget );
	{
	chiudi_dlg_config();
	}
	UxNmod_dialog_configContext = UxSaveCtx;
}

static void	activateCB_anal_jacMenu_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_dialog_config  *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_dialog_configContext;
	UxNmod_dialog_configContext = UxContext =
			(_UxCnmod_dialog_config *) UxGetContext( UxWidget );
	{jacobian_type = ANALYTICAL;}
	UxNmod_dialog_configContext = UxSaveCtx;
}

static void	activateCB_num_jacMenu_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_dialog_config  *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_dialog_configContext;
	UxNmod_dialog_configContext = UxContext =
			(_UxCnmod_dialog_config *) UxGetContext( UxWidget );
	{jacobian_type = NUMERICAL;}
	UxNmod_dialog_configContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_nmod_dialog_config()
{
	Widget	jac_optMenu_pane_shell;

	nmod_dialog_config = XtVaCreatePopupShell( "nmod_dialog_config",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNkeyboardFocusPolicy, XmEXPLICIT,
			XmNgeometry, "+785+145",
			XmNtitle, "NEWMOD - INPUT/OUTPUT VARIABLES SETUP",
			XmNheight, 100,
			XmNwidth, 100,
			XmNy, 680,
			XmNx, 180,
			NULL );

	UxPutContext( nmod_dialog_config, (char *) UxNmod_dialog_configContext );

	nmod_dialog_form1 = XtVaCreateWidget( "nmod_dialog_form1",
			xmFormWidgetClass, nmod_dialog_config,
			XmNautoUnmanage, FALSE,
			XmNheight, 310,
			XmNwidth, 370,
			XmNy, 195,
			XmNx, 585,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( nmod_dialog_form1, (char *) UxNmod_dialog_configContext );

	nmod_dialog_label1 = XtVaCreateManagedWidget( "nmod_dialog_label1",
			xmLabelWidgetClass, nmod_dialog_form1,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightPosition, 78,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNtopOffset, 15,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Number of state output variables:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 35,
			XmNwidth, 190,
			XmNy, 30,
			XmNx, 10,
			NULL );

	UxPutContext( nmod_dialog_label1, (char *) UxNmod_dialog_configContext );

	nmod_dialog_label2 = XtVaCreateManagedWidget( "nmod_dialog_label2",
			xmLabelWidgetClass, nmod_dialog_form1,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightPosition, 78,
			XmNrightAttachment, XmATTACH_POSITION,
			RES_CONVERT( XmNlabelString, "Number of algebraic output variables:" ),
			XmNtopWidget, nmod_dialog_label1,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 35,
			XmNwidth, 190,
			XmNy, 75,
			XmNx, 10,
			NULL );

	UxPutContext( nmod_dialog_label2, (char *) UxNmod_dialog_configContext );

	nmod_dialog_label3 = XtVaCreateManagedWidget( "nmod_dialog_label3",
			xmLabelWidgetClass, nmod_dialog_form1,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightPosition, 78,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNtopWidget, nmod_dialog_label2,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "Number of input variables:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 35,
			XmNwidth, 190,
			XmNy, 120,
			XmNx, 10,
			NULL );

	UxPutContext( nmod_dialog_label3, (char *) UxNmod_dialog_configContext );

	nmod_dialog_label4 = XtVaCreateManagedWidget( "nmod_dialog_label4",
			xmLabelWidgetClass, nmod_dialog_form1,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightPosition, 78,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNtopWidget, nmod_dialog_label3,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "Number of geometrical data:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 35,
			XmNwidth, 220,
			XmNy, 160,
			XmNx, 10,
			NULL );

	UxPutContext( nmod_dialog_label4, (char *) UxNmod_dialog_configContext );

	nmod_dialog_text1 = XtVaCreateManagedWidget( "nmod_dialog_text1",
			xmTextWidgetClass, nmod_dialog_form1,
			XmNfontList, UxConvertFontList( "*Courier-Bold-R-Normal--14-140-*" ),
			XmNmaxLength, 3,
			XmNcolumns, 2,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftPosition, 80,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNheight, 35,
			XmNwidth, 60,
			XmNy, 20,
			XmNx, 240,
			NULL );

	UxPutContext( nmod_dialog_text1, (char *) UxNmod_dialog_configContext );

	nmod_dialog_text2 = XtVaCreateManagedWidget( "nmod_dialog_text2",
			xmTextWidgetClass, nmod_dialog_form1,
			XmNmaxLength, 3,
			XmNcolumns, 2,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopWidget, nmod_dialog_text1,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftPosition, 80,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNheight, 35,
			XmNwidth, 60,
			XmNy, 70,
			XmNx, 240,
			NULL );

	UxPutContext( nmod_dialog_text2, (char *) UxNmod_dialog_configContext );

	nmod_dialog_text3 = XtVaCreateManagedWidget( "nmod_dialog_text3",
			xmTextWidgetClass, nmod_dialog_form1,
			XmNmaxLength, 3,
			XmNcolumns, 2,
			XmNtopOffset, 15,
			XmNtopWidget, nmod_dialog_text2,
			XmNtopPosition, 0,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftPosition, 80,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNheight, 35,
			XmNwidth, 60,
			XmNy, 120,
			XmNx, 240,
			NULL );

	UxPutContext( nmod_dialog_text3, (char *) UxNmod_dialog_configContext );

	nmod_dialog_text4 = XtVaCreateManagedWidget( "nmod_dialog_text4",
			xmTextWidgetClass, nmod_dialog_form1,
			XmNmaxLength, 3,
			XmNcolumns, 2,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, nmod_dialog_text3,
			XmNtopOffset, 15,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftPosition, 80,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNheight, 35,
			XmNwidth, 60,
			XmNy, 180,
			XmNx, 240,
			NULL );

	UxPutContext( nmod_dialog_text4, (char *) UxNmod_dialog_configContext );

	nmod_dialog_ok = XtVaCreateManagedWidget( "nmod_dialog_ok",
			xmPushButtonWidgetClass, nmod_dialog_form1,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 90,
			XmNy, 260,
			XmNx, 20,
			NULL );

	UxPutContext( nmod_dialog_ok, (char *) UxNmod_dialog_configContext );

	nmod_dialog_cancel = XtVaCreateManagedWidget( "nmod_dialog_cancel",
			xmPushButtonWidgetClass, nmod_dialog_form1,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftWidget, nmod_dialog_ok,
			XmNleftOffset, 15,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 80,
			XmNy, 260,
			XmNx, 140,
			NULL );

	UxPutContext( nmod_dialog_cancel, (char *) UxNmod_dialog_configContext );

	separator1 = XtVaCreateManagedWidget( "separator1",
			xmSeparatorWidgetClass, nmod_dialog_form1,
			XmNbottomWidget, nmod_dialog_ok,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNtopWidget, NULL,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			XmNheight, 10,
			XmNwidth, 440,
			XmNy, 230,
			XmNx, 20,
			NULL );

	UxPutContext( separator1, (char *) UxNmod_dialog_configContext );

	nmod_dialog_label5 = XtVaCreateManagedWidget( "nmod_dialog_label5",
			xmLabelWidgetClass, nmod_dialog_form1,
			RES_CONVERT( XmNlabelString, "Jacobian computation:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopWidget, nmod_dialog_label4,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightPosition, 60,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 0,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNheight, 35,
			XmNwidth, 260,
			XmNy, 210,
			XmNx, 10,
			NULL );

	UxPutContext( nmod_dialog_label5, (char *) UxNmod_dialog_configContext );

	jac_optMenu_pane_shell = XtVaCreatePopupShell ("jac_optMenu_pane_shell",
			xmMenuShellWidgetClass, nmod_dialog_form1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	jac_optMenu_pane = XtVaCreateWidget( "jac_optMenu_pane",
			xmRowColumnWidgetClass, jac_optMenu_pane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( jac_optMenu_pane, (char *) UxNmod_dialog_configContext );

	anal_jacMenu_pb = XtVaCreateManagedWidget( "anal_jacMenu_pb",
			xmPushButtonWidgetClass, jac_optMenu_pane,
			RES_CONVERT( XmNmnemonic, "A" ),
			RES_CONVERT( XmNlabelString, "Analytical" ),
			NULL );

	UxPutContext( anal_jacMenu_pb, (char *) UxNmod_dialog_configContext );

	num_jacMenu_pb = XtVaCreateManagedWidget( "num_jacMenu_pb",
			xmPushButtonWidgetClass, jac_optMenu_pane,
			RES_CONVERT( XmNmnemonic, "N" ),
			RES_CONVERT( XmNlabelString, "Numerical" ),
			NULL );

	UxPutContext( num_jacMenu_pb, (char *) UxNmod_dialog_configContext );

	jac_optMenu = XtVaCreateManagedWidget( "jac_optMenu",
			xmRowColumnWidgetClass, nmod_dialog_form1,
			XmNtopWidget, nmod_dialog_text4,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNy, 225,
			XmNx, 252,
			XmNsubMenuId, jac_optMenu_pane,
			XmNrowColumnType, XmMENU_OPTION,
			NULL );

	UxPutContext( jac_optMenu, (char *) UxNmod_dialog_configContext );

	XtAddCallback( nmod_dialog_config, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxNmod_dialog_configContext );

	XtAddCallback( nmod_dialog_ok, XmNactivateCallback,
			activateCB_nmod_dialog_ok,
			(XtPointer) UxNmod_dialog_configContext );

	XtAddCallback( nmod_dialog_cancel, XmNactivateCallback,
			activateCB_nmod_dialog_cancel,
			(XtPointer) UxNmod_dialog_configContext );

	XtAddCallback( anal_jacMenu_pb, XmNactivateCallback,
			activateCB_anal_jacMenu_pb,
			(XtPointer) UxNmod_dialog_configContext );

	XtAddCallback( num_jacMenu_pb, XmNactivateCallback,
			activateCB_num_jacMenu_pb,
			(XtPointer) UxNmod_dialog_configContext );



	return ( nmod_dialog_config );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_nmod_dialog_config()
{
	Widget                  rtrn;
	_UxCnmod_dialog_config  *UxContext;

	UxNmod_dialog_configContext = UxContext =
		(_UxCnmod_dialog_config *) XtMalloc( sizeof(_UxCnmod_dialog_config) );

	{
		char value[10];
		
		if (nmod_def_initialized)
		{
		   set_something_val(UxGetWidget(pb_nmod_jactopology),XmNsensitive, (XtArgVal) False);
		   set_something_val(UxGetWidget(pb_nmod_JC),XmNsensitive, (XtArgVal) False);
		   set_something_val(UxGetWidget(pb_nmod_residual),XmNsensitive, (XtArgVal) False);
		   set_something_val(UxGetWidget(pb_nmod_D1),XmNsensitive, (XtArgVal) False);
		}
		rtrn = _Uxbuild_nmod_dialog_config();

		sprintf(value,"%-3d",num_var_stato);
		XmTextSetString(UxGetWidget(nmod_dialog_text1), value);
		
		sprintf(value,"%-3d",num_var_algebriche);
		XmTextSetString(UxGetWidget(nmod_dialog_text2), value);
		
		sprintf(value,"%-3d",num_var_ingresso);
		XmTextSetString(UxGetWidget(nmod_dialog_text3), value);
		
		sprintf(value,"%-3d",num_dati_geometrici);
		XmTextSetString(UxGetWidget(nmod_dialog_text4), value);
		
		bool_dialog_config = False;
		
		printf("jacobian_type=%d\n",jacobian_type);
		if(jacobian_type)
		   set_something_val(jac_optMenu,XmNmenuHistory, (XtArgVal) num_jacMenu_pb);
		
		UxPopupInterface(rtrn, no_grab);
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_nmod_dialog_config()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_nmod_dialog_config();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

