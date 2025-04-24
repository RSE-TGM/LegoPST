/**********************************************************************
*
*       C Source:               selzoom_dialog.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 16 17:12:28 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: selzoom_dialog.c,2 %  (%full_filespec: 1,csrc,selzoom_dialog.c,2 %)";
#endif
/*
   modulo selzoom_dialog.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)selzoom_dialog.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)selzoom_dialog.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*******************************************************************************
	selzoom_dialog.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/BulletinB.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "selzoom_dialog.h"
#include <Xl/XlGrafico.h>
#include <Xm/ToggleBG.h>

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
	S_XLGRAFICO *UxPXlGrafico;
	Widget	Uxselzoom_dialog;
	Widget	Uxselzoom_row;
	Widget	Uxselzoom_radio;
	Widget	Uxtoggle_zoom1;
	Widget	Uxtoggle_zoom2;
	Widget	Uxtoggle_zoom3;
	Widget	Uxtoggle_zoom4;
	Widget	Uxtoggle_zoom5;
	Widget	Uxbutton_box;
	Widget	UxPOK_button;
	Widget	UxPCancel_button;
} _UxCselzoom_dialog;

#define selzoom_Graf		UxSelzoom_dialogContext->UxPXlGrafico
#define selzoom_dialog          UxSelzoom_dialogContext->Uxselzoom_dialog
#define selzoom_row             UxSelzoom_dialogContext->Uxselzoom_row
#define selzoom_radio           UxSelzoom_dialogContext->Uxselzoom_radio
#define toggle_zoom1            UxSelzoom_dialogContext->Uxtoggle_zoom1
#define toggle_zoom2            UxSelzoom_dialogContext->Uxtoggle_zoom2
#define toggle_zoom3            UxSelzoom_dialogContext->Uxtoggle_zoom3
#define toggle_zoom4            UxSelzoom_dialogContext->Uxtoggle_zoom4
#define toggle_zoom5            UxSelzoom_dialogContext->Uxtoggle_zoom5
#define button_box              UxSelzoom_dialogContext->Uxbutton_box
#define POK_button              UxSelzoom_dialogContext->UxPOK_button
#define PCancel_button          UxSelzoom_dialogContext->UxPCancel_button

static _UxCselzoom_dialog	*UxSelzoom_dialogContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_selzoom_dialog();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_selzoom_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget	selzoom_dialog_shell;

/*  Setto il puntatore alla struttura delle variabili del grafics  */
selzoom_Graf = PuntGraf;

	selzoom_dialog_shell = XtVaCreatePopupShell( "selzoom_dialog_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNtitle, "selzoom_dialog",
			NULL );

	selzoom_dialog = XtVaCreateWidget( "selzoom_dialog",
			xmBulletinBoardWidgetClass, selzoom_dialog_shell,
			XmNallowOverlap, FALSE,
			RES_CONVERT( XmNdialogTitle, "Scelta intervallo visualizzato" ),
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			XmNmarginWidth, 20,
			NULL );

	UxPutContext( selzoom_dialog, (char *) UxSelzoom_dialogContext );

	create_proc( selzoom_dialog,
			26, (XtPointer) NULL );

	selzoom_row = XtVaCreateManagedWidget( "selzoom_row",
			xmRowColumnWidgetClass, selzoom_dialog,
			XmNorientation, XmVERTICAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			XmNmarginWidth, 80,
			XmNradioBehavior, TRUE,
			NULL );

	UxPutContext( selzoom_row, (char *) UxSelzoom_dialogContext );

	selzoom_radio = XtVaCreateManagedWidget( "selzoom_radio",
			xmRowColumnWidgetClass, selzoom_row,
			XmNpacking, XmPACK_COLUMN,
			XmNradioBehavior, TRUE,
			XmNisHomogeneous, TRUE,
			XmNentryClass, xmToggleButtonGadgetClass,
			XmNrowColumnType, XmWORK_AREA,
			NULL );

	UxPutContext( selzoom_radio, (char *) UxSelzoom_dialogContext );

	toggle_zoom1 = XtVaCreateManagedWidget( "toggle_zoom1",
			xmToggleButtonWidgetClass, selzoom_radio,
			RES_CONVERT( XmNlabelString, "30 minuti" ),
			NULL );

	UxPutContext( toggle_zoom1, (char *) UxSelzoom_dialogContext );

	create_proc( toggle_zoom1,
			27, (XtPointer) NULL );

	toggle_zoom2 = XtVaCreateManagedWidget( "toggle_zoom2",
			xmToggleButtonWidgetClass, selzoom_radio,
			RES_CONVERT( XmNlabelString, "15 minuti" ),
			NULL );

	UxPutContext( toggle_zoom2, (char *) UxSelzoom_dialogContext );

	create_proc( toggle_zoom2,
			28, (XtPointer) NULL );

	toggle_zoom3 = XtVaCreateManagedWidget( "toggle_zoom3",
			xmToggleButtonWidgetClass, selzoom_radio,
			RES_CONVERT( XmNlabelString, "6  minuti" ),
			NULL );

	UxPutContext( toggle_zoom3, (char *) UxSelzoom_dialogContext );

	create_proc( toggle_zoom3,
			29, (XtPointer) NULL );

	toggle_zoom4 = XtVaCreateManagedWidget( "toggle_zoom4",
			xmToggleButtonWidgetClass, selzoom_radio,
			RES_CONVERT( XmNlabelString, "2  minuti" ),
			NULL );

	UxPutContext( toggle_zoom4, (char *) UxSelzoom_dialogContext );

	create_proc( toggle_zoom4,
			30, (XtPointer) NULL );

	toggle_zoom5 = XtVaCreateManagedWidget( "toggle_zoom5",
			xmToggleButtonWidgetClass, selzoom_radio,
			RES_CONVERT( XmNlabelString, "1  minuto" ),
			NULL );

	UxPutContext( toggle_zoom5, (char *) UxSelzoom_dialogContext );

	create_proc( toggle_zoom5,
			31, (XtPointer) NULL );

	button_box = XtVaCreateManagedWidget( "button_box",
			xmRowColumnWidgetClass, selzoom_row,
			XmNorientation, XmHORIZONTAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( button_box, (char *) UxSelzoom_dialogContext );

	POK_button = XtVaCreateManagedWidget( "POK_button",
			xmPushButtonWidgetClass, button_box,
			RES_CONVERT( XmNlabelString, "Ok" ),
			NULL );

	UxPutContext( POK_button, (char *) UxSelzoom_dialogContext );

	PCancel_button = XtVaCreateManagedWidget( "PCancel_button",
			xmPushButtonWidgetClass, button_box,
			RES_CONVERT( XmNlabelString, "Abbandona" ),
			NULL );

	UxPutContext( PCancel_button, (char *) UxSelzoom_dialogContext );

	XtAddCallback( selzoom_dialog, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxSelzoom_dialogContext );

	XtVaSetValues(selzoom_dialog,
			XmNcancelButton, PCancel_button,
			XmNdefaultButton, POK_button,
			NULL );

	XtAddCallback( toggle_zoom1, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)27 );

	XtAddCallback( toggle_zoom2, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)28 );

	XtAddCallback( toggle_zoom3, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)29 );

	XtAddCallback( toggle_zoom4, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)30 );

	XtAddCallback( toggle_zoom5, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)31 );

	XtAddCallback( POK_button, XmNactivateCallback,
			ok_proc,
			(XtPointer)26 );

	XtAddCallback( PCancel_button, XmNactivateCallback,
			cancel_proc,
			(XtPointer)26 );



	return ( selzoom_dialog );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_selzoom_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget                  rtrn;
	_UxCselzoom_dialog      *UxContext;

	UxSelzoom_dialogContext = UxContext =
		(_UxCselzoom_dialog *) XtMalloc( sizeof(_UxCselzoom_dialog) );

	rtrn = _Uxbuild_selzoom_dialog(PuntGraf);

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_selzoom_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_selzoom_dialog(PuntGraf);

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

