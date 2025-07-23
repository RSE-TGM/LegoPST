
/*******************************************************************************
	dialog_print_config.c
	(Generated from interface file dialog_print_config.i)
*******************************************************************************/

#include <stdio.h>
#include <unistd.h>

#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/SeparatoG.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/Form.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
   modulo dialog_print_config.i
   tipo 
   release 1.12
   data 3/31/95
   reserved @(#)dialog_print_config.i	1.12
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#include "libutilx.h"
#include "utile.h"
#include "autodoc.h"

extern void lcDestroySwidget(Widget);

/************************************************************************/
/* VARIABILI GLOBALI ESTERNE						*/
/************************************************************************/

extern int num_lines_vert, num_lines_horiz;
extern byte tipo_stampante;

extern char page_prefix_dati[], page_prefix_var[];

extern Boolean open_print_config;

/************************************************************************/
/* VARIABILI GLOBALI 							*/
/************************************************************************/

static byte temp_tipo_stampante;

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
	Widget	Uxdialog_print_config;
	Widget	Uxlabel10;
	Widget	Uxlabel11;
	Widget	Uxpb_ok_print_config;
	Widget	Uxpb_cancel_print_config;
	Widget	Uxtext_num_righe_vr;
	Widget	UxrowColumn1;
	Widget	Uxopt_printer;
	Widget	Uxopt_printer_ascii;
	Widget	Uxopt_printer_altro;
	Widget	Uxmenu1;
	Widget	Uxlabel7;
	Widget	Uxtext_num_righe_or;
	Widget	Uxlabel4;
	Widget	Uxtext_prefix_dati;
	Widget	Uxlabel9;
	Widget	Uxtext_prefix_var;
	Widget	UxseparatorGadget6;
} _UxCdialog_print_config;

#define dialog_print_config     UxDialog_print_configContext->Uxdialog_print_config
#define label10                 UxDialog_print_configContext->Uxlabel10
#define label11                 UxDialog_print_configContext->Uxlabel11
#define pb_ok_print_config      UxDialog_print_configContext->Uxpb_ok_print_config
#define pb_cancel_print_config  UxDialog_print_configContext->Uxpb_cancel_print_config
#define text_num_righe_vr       UxDialog_print_configContext->Uxtext_num_righe_vr
#define rowColumn1              UxDialog_print_configContext->UxrowColumn1
#define opt_printer             UxDialog_print_configContext->Uxopt_printer
#define opt_printer_ascii       UxDialog_print_configContext->Uxopt_printer_ascii
#define opt_printer_altro       UxDialog_print_configContext->Uxopt_printer_altro
#define menu1                   UxDialog_print_configContext->Uxmenu1
#define label7                  UxDialog_print_configContext->Uxlabel7
#define text_num_righe_or       UxDialog_print_configContext->Uxtext_num_righe_or
#define label4                  UxDialog_print_configContext->Uxlabel4
#define text_prefix_dati        UxDialog_print_configContext->Uxtext_prefix_dati
#define label9                  UxDialog_print_configContext->Uxlabel9
#define text_prefix_var         UxDialog_print_configContext->Uxtext_prefix_var
#define separatorGadget6        UxDialog_print_configContext->UxseparatorGadget6

static _UxCdialog_print_config	*UxDialog_print_configContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_dialog_print_config();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_pb_ok_print_config( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdialog_print_config *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDialog_print_configContext;
	UxDialog_print_configContext = UxContext =
			(_UxCdialog_print_config *) UxGetContext( UxWidget );
	{
	char *str;
	int num;
	FILE *fp;
	
	str = XmTextFieldGetString(UxGetWidget(text_num_righe_vr));
	if (IsNumeric(str))
	   sscanf(str,"%d",&num);
	XmStringFree((XmString)str);
	
	/* cancella i file di documentazione cosi' il programma e' costretto a */
	/* ricrearli con i nuovi parametri */
	if (num != num_lines_vert)
	{
	   unlink(DOC_DATI_MODELLO);
	   unlink(DOC_VAR_MODELLO);
	   unlink(DOC_SSTATE_MODELLO);
	}
	num_lines_vert = num;
	
	str = XmTextFieldGetString(UxGetWidget(text_num_righe_or));
	if (IsNumeric(str))
	   sscanf(str,"%d",&num);
	XmStringFree((XmString)str);
	
	/* cancella i file di documentazione cosi' il programma e' costretto a */
	/* ricrearli con i nuovi parametri */
	if (num != num_lines_horiz)
	{
	   unlink(DOC_VAR_MODELLO);
	   unlink(DOC_SSTATE_MODELLO);
	}
	num_lines_horiz = num;
	 
	str = XmTextFieldGetString(UxGetWidget(text_prefix_dati));
	strcpy(page_prefix_dati, str);
	XmStringFree((XmString)str);
	 
	str = XmTextFieldGetString(UxGetWidget(text_prefix_var));
	strcpy(page_prefix_var, str);
	XmStringFree((XmString)str);
	
	tipo_stampante = temp_tipo_stampante;
	
	/* salva la configurazione */
	if ((fp = fopen(FILE_PRINT_CONFIG,"w")) != NULL)
	{
	   fprintf(fp, "%d %d %s %s", num_lines_vert, num_lines_horiz, page_prefix_dati,
				      page_prefix_var);
	   fclose(fp);
	}
	
	open_print_config = False;
	UxDestroySwidget(dialog_print_config);
	}
	UxDialog_print_configContext = UxSaveCtx;
}

static void	activateCB_pb_cancel_print_config( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdialog_print_config *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDialog_print_configContext;
	UxDialog_print_configContext = UxContext =
			(_UxCdialog_print_config *) UxGetContext( UxWidget );
	{
	open_print_config = False;
	UxDestroySwidget(dialog_print_config);
	}
	UxDialog_print_configContext = UxSaveCtx;
}

static void	activateCB_opt_printer_ascii( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdialog_print_config *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDialog_print_configContext;
	UxDialog_print_configContext = UxContext =
			(_UxCdialog_print_config *) UxGetContext( UxWidget );
	{
	temp_tipo_stampante = 0;  /* ASCII */
	}
	UxDialog_print_configContext = UxSaveCtx;
}

static void	activateCB_opt_printer_altro( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdialog_print_config *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDialog_print_configContext;
	UxDialog_print_configContext = UxContext =
			(_UxCdialog_print_config *) UxGetContext( UxWidget );
	{
	temp_tipo_stampante = 1; /* POSTSCRIPT */
	}
	UxDialog_print_configContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_dialog_print_config()
{
	Widget	dialog_print_config_shell;
	Widget	opt_printer_shell;

	dialog_print_config_shell = XtVaCreatePopupShell( "dialog_print_config_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 242,
			XmNy, 175,
			XmNwidth, 362,
			XmNheight, 300,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "dialog_print_config",
			NULL );

	dialog_print_config = XtVaCreateWidget( "dialog_print_config",
			xmFormWidgetClass, dialog_print_config_shell,
			XmNnoResize, TRUE,
			XmNlabelFontList, UxConvertFontList( "-adobe-helvetica-Bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			RES_CONVERT( XmNdialogTitle, "PRINTER CONFIG" ),
			XmNbuttonFontList, UxConvertFontList( "-adobe-helvetica-Bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 300,
			XmNwidth, 362,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( dialog_print_config, (char *) UxDialog_print_configContext );

	label10 = XtVaCreateManagedWidget( "label10",
			xmLabelWidgetClass, dialog_print_config,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Printer type:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopOffset, 10,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 37,
			XmNwidth, 180,
			XmNy, 20,
			XmNx, 10,
			NULL );

	UxPutContext( label10, (char *) UxDialog_print_configContext );

	label11 = XtVaCreateManagedWidget( "label11",
			xmLabelWidgetClass, dialog_print_config,
			RES_CONVERT( XmNlabelString, "No lines per page (VERTICAL):" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopWidget, label10,
			XmNtopOffset, 20,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 30,
			XmNwidth, 240,
			XmNy, 60,
			XmNx, 10,
			NULL );

	UxPutContext( label11, (char *) UxDialog_print_configContext );

	pb_ok_print_config = XtVaCreateManagedWidget( "pb_ok_print_config",
			xmPushButtonWidgetClass, dialog_print_config,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightPosition, 40,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 30,
			XmNwidth, 176,
			XmNy, 150,
			XmNx, 20,
			NULL );

	UxPutContext( pb_ok_print_config, (char *) UxDialog_print_configContext );

	pb_cancel_print_config = XtVaCreateManagedWidget( "pb_cancel_print_config",
			xmPushButtonWidgetClass, dialog_print_config,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftPosition, 60,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 30,
			XmNwidth, 176,
			XmNy, 150,
			XmNx, 294,
			NULL );

	UxPutContext( pb_cancel_print_config, (char *) UxDialog_print_configContext );

	text_num_righe_vr = XtVaCreateManagedWidget( "text_num_righe_vr",
			xmTextFieldWidgetClass, dialog_print_config,
			XmNmaxLength, 3,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopWidget, label10,
			XmNtopOffset, 20,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, label11,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 35,
			XmNwidth, 100,
			XmNy, 60,
			XmNx, 260,
			NULL );

	UxPutContext( text_num_righe_vr, (char *) UxDialog_print_configContext );

	rowColumn1 = XtVaCreateManagedWidget( "rowColumn1",
			xmRowColumnWidgetClass, dialog_print_config,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftWidget, label10,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 37,
			XmNwidth, 170,
			XmNy, 10,
			XmNx, 190,
			NULL );

	UxPutContext( rowColumn1, (char *) UxDialog_print_configContext );

	opt_printer_shell = XtVaCreatePopupShell ("opt_printer_shell",
			xmMenuShellWidgetClass, rowColumn1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	opt_printer = XtVaCreateWidget( "opt_printer",
			xmRowColumnWidgetClass, opt_printer_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( opt_printer, (char *) UxDialog_print_configContext );

	opt_printer_ascii = XtVaCreateManagedWidget( "opt_printer_ascii",
			xmPushButtonWidgetClass, opt_printer,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-Bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNlabelString, "ASCII" ),
			NULL );

	UxPutContext( opt_printer_ascii, (char *) UxDialog_print_configContext );

	opt_printer_altro = XtVaCreateManagedWidget( "opt_printer_altro",
			xmPushButtonWidgetClass, opt_printer,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-Bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNlabelString, "Altre Stmp" ),
			NULL );

	UxPutContext( opt_printer_altro, (char *) UxDialog_print_configContext );

	menu1 = XtVaCreateManagedWidget( "menu1",
			xmRowColumnWidgetClass, rowColumn1,
			XmNheight, 30,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNy, 3,
			XmNx, 196,
			XmNsubMenuId, opt_printer,
			XmNrowColumnType, XmMENU_OPTION,
			NULL );

	UxPutContext( menu1, (char *) UxDialog_print_configContext );

	label7 = XtVaCreateManagedWidget( "label7",
			xmLabelWidgetClass, dialog_print_config,
			XmNtopWidget, label11,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "No lines per page (HORIZONTAL):" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 30,
			XmNwidth, 240,
			XmNy, 90,
			XmNx, 10,
			NULL );

	UxPutContext( label7, (char *) UxDialog_print_configContext );

	text_num_righe_or = XtVaCreateManagedWidget( "text_num_righe_or",
			xmTextFieldWidgetClass, dialog_print_config,
			XmNmaxLength, 3,
			XmNleftWidget, label7,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNtopWidget, label11,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNheight, 35,
			XmNwidth, 80,
			XmNy, 100,
			XmNx, 260,
			NULL );

	UxPutContext( text_num_righe_or, (char *) UxDialog_print_configContext );

	label4 = XtVaCreateManagedWidget( "label4",
			xmLabelWidgetClass, dialog_print_config,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Page prefix geometrical data:" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopWidget, label7,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNheight, 35,
			XmNwidth, 240,
			XmNy, 130,
			XmNx, 10,
			NULL );

	UxPutContext( label4, (char *) UxDialog_print_configContext );

	text_prefix_dati = XtVaCreateManagedWidget( "text_prefix_dati",
			xmTextFieldWidgetClass, dialog_print_config,
			XmNmaxLength, 5,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftWidget, label4,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNtopWidget, label7,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 35,
			XmNwidth, 80,
			XmNy, 140,
			XmNx, 260,
			NULL );

	UxPutContext( text_prefix_dati, (char *) UxDialog_print_configContext );

	label9 = XtVaCreateManagedWidget( "label9",
			xmLabelWidgetClass, dialog_print_config,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNtopWidget, label4,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Page prefix variables:" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 35,
			XmNwidth, 240,
			XmNy, 157,
			XmNx, 20,
			NULL );

	UxPutContext( label9, (char *) UxDialog_print_configContext );

	text_prefix_var = XtVaCreateManagedWidget( "text_prefix_var",
			xmTextFieldWidgetClass, dialog_print_config,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNleftWidget, label9,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopWidget, label4,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmaxLength, 5,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 35,
			XmNwidth, 80,
			XmNy, 157,
			XmNx, 270,
			NULL );

	UxPutContext( text_prefix_var, (char *) UxDialog_print_configContext );

	separatorGadget6 = XtVaCreateManagedWidget( "separatorGadget6",
			xmSeparatorGadgetClass, dialog_print_config,
			XmNtopWidget, label9,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, pb_ok_print_config,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 10,
			XmNwidth, 470,
			XmNy, 110,
			XmNx, 10,
			NULL );

	UxPutContext( separatorGadget6, (char *) UxDialog_print_configContext );

	XtAddCallback( dialog_print_config, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxDialog_print_configContext );

	XtAddCallback( pb_ok_print_config, XmNactivateCallback,
			activateCB_pb_ok_print_config,
			(XtPointer) UxDialog_print_configContext );

	XtAddCallback( pb_cancel_print_config, XmNactivateCallback,
			activateCB_pb_cancel_print_config,
			(XtPointer) UxDialog_print_configContext );

	XtAddCallback( opt_printer_ascii, XmNactivateCallback,
			activateCB_opt_printer_ascii,
			(XtPointer) UxDialog_print_configContext );

	XtAddCallback( opt_printer_altro, XmNactivateCallback,
			activateCB_opt_printer_altro,
			(XtPointer) UxDialog_print_configContext );



	return ( dialog_print_config );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_dialog_print_config()
{
	Widget                  rtrn;
	_UxCdialog_print_config *UxContext;

	UxDialog_print_configContext = UxContext =
		(_UxCdialog_print_config *) XtMalloc( sizeof(_UxCdialog_print_config) );

	{
		char str[10];
		
		open_print_config = True;
		rtrn = _Uxbuild_dialog_print_config();

		sprintf(str, "%3d", num_lines_vert);
		XmTextSetString(UxGetWidget(text_num_righe_vr), str);
		sprintf(str, "%3d", num_lines_horiz);
		XmTextSetString(UxGetWidget(text_num_righe_or), str);
		XmTextSetString(UxGetWidget(text_prefix_dati), page_prefix_dati);
		XmTextSetString(UxGetWidget(text_prefix_var), page_prefix_var);
		
		if (tipo_stampante == PRINTER_ASCII)
		   set_something_val(UxGetWidget(opt_printer), XmNmenuHistory,
		                 UxGetWidget((char*)opt_printer_ascii));
		else
		   set_something_val(UxGetWidget(opt_printer), XmNmenuHistory,
		                 (char*)UxGetWidget(opt_printer_altro));
		
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

Widget	create_dialog_print_config()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_dialog_print_config();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

