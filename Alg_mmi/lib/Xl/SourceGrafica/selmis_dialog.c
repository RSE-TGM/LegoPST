/**********************************************************************
*
*       C Source:               selmis_dialog.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 16 17:12:21 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: selmis_dialog.c,2 %  (%full_filespec: 1,csrc,selmis_dialog.c,2 %)";
#endif
/*
   modulo selmis_dialog.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)selmis_dialog.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)selmis_dialog.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*******************************************************************************
	selmis_dialog.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/PushB.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/BulletinB.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "selmis_dialog.h"
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
	Widget	Uxselmis_dialog;
	Widget	Uxselmis_row;
	Widget	Uxselmis_radio;
	Widget	Uxtoggle_mis1;
	Widget	Uxtoggle_mis2;
	Widget	Uxtoggle_mis3;
	Widget	Uxtoggle_mis4;
	Widget	Uxselmis_boxUxWin;
	Widget	Uxselmis_box;
	Widget	Uxselmis_button;
	Widget	Uxselmis_ok;
	Widget	Uxselmis_cancel;
	Widget	Uxfind_next;
	Widget	Uxfind_previous;
	Widget	Uxselmis_elimina;
	Widget	Uxfind_text;
} _UxCselmis_dialog;

#define selmis_Graf		UxSelmis_dialogContext->UxPXlGrafico
#define selmis_dialog           UxSelmis_dialogContext->Uxselmis_dialog
#define selmis_row              UxSelmis_dialogContext->Uxselmis_row
#define selmis_radio            UxSelmis_dialogContext->Uxselmis_radio
#define toggle_mis1             UxSelmis_dialogContext->Uxtoggle_mis1
#define toggle_mis2             UxSelmis_dialogContext->Uxtoggle_mis2
#define toggle_mis3             UxSelmis_dialogContext->Uxtoggle_mis3
#define toggle_mis4             UxSelmis_dialogContext->Uxtoggle_mis4
#define selmis_boxUxWin         UxSelmis_dialogContext->Uxselmis_boxUxWin
#define selmis_box              UxSelmis_dialogContext->Uxselmis_box
#define selmis_button           UxSelmis_dialogContext->Uxselmis_button
#define selmis_ok               UxSelmis_dialogContext->Uxselmis_ok
#define selmis_cancel           UxSelmis_dialogContext->Uxselmis_cancel
#define find_next               UxSelmis_dialogContext->Uxfind_next
#define find_previous           UxSelmis_dialogContext->Uxfind_previous
#define selmis_elimina          UxSelmis_dialogContext->Uxselmis_elimina
#define find_text               UxSelmis_dialogContext->Uxfind_text

static _UxCselmis_dialog	*UxSelmis_dialogContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_selmis_dialog();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_selmis_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget	selmis_dialog_shell;

/*  Setto il puntatore alla struttura delle variabili del grafics  */
selmis_Graf = PuntGraf;

	selmis_dialog_shell = XtVaCreatePopupShell( "selmis_dialog_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 200,
			XmNtitle, "selmis_dialog",
			NULL );

	selmis_dialog = XtVaCreateWidget( "selmis_dialog",
			xmBulletinBoardWidgetClass, selmis_dialog_shell,
			RES_CONVERT( XmNdialogTitle, "Scelta singola variabile" ),
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			XmNmarginWidth, 20,
			NULL );

	UxPutContext( selmis_dialog, (char *) UxSelmis_dialogContext );

	create_proc( selmis_dialog,
			20, (XtPointer) NULL );

	selmis_row = XtVaCreateManagedWidget( "selmis_row",
			xmRowColumnWidgetClass, selmis_dialog,
			XmNorientation, XmVERTICAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( selmis_row, (char *) UxSelmis_dialogContext );

	selmis_radio = XtVaCreateManagedWidget( "selmis_radio",
			xmRowColumnWidgetClass, selmis_row,
			XmNspacing, 10,
			XmNpacking, XmPACK_COLUMN,
			XmNradioBehavior, TRUE,
			XmNisHomogeneous, TRUE,
			XmNentryClass, xmToggleButtonGadgetClass,
			XmNrowColumnType, XmWORK_AREA,
			NULL );

	UxPutContext( selmis_radio, (char *) UxSelmis_dialogContext );

	toggle_mis1 = XtVaCreateManagedWidget( "toggle_mis1",
			xmToggleButtonWidgetClass, selmis_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle_mis1, (char *) UxSelmis_dialogContext );

	create_proc( toggle_mis1,
			21, (XtPointer) NULL );

	toggle_mis2 = XtVaCreateManagedWidget( "toggle_mis2",
			xmToggleButtonWidgetClass, selmis_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle_mis2, (char *) UxSelmis_dialogContext );

	create_proc( toggle_mis2,
			22, (XtPointer) NULL );

	toggle_mis3 = XtVaCreateManagedWidget( "toggle_mis3",
			xmToggleButtonWidgetClass, selmis_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle_mis3, (char *) UxSelmis_dialogContext );

	create_proc( toggle_mis3,
			23, (XtPointer) NULL );

	toggle_mis4 = XtVaCreateManagedWidget( "toggle_mis4",
			xmToggleButtonWidgetClass, selmis_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle_mis4, (char *) UxSelmis_dialogContext );

	create_proc( toggle_mis4,
			24, (XtPointer) NULL );

	selmis_boxUxWin = XtVaCreateManagedWidget( "selmis_boxUxWin",
			xmScrolledWindowWidgetClass, selmis_row,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			NULL );

	UxPutContext( selmis_boxUxWin, (char *) UxSelmis_dialogContext );

	selmis_box = XtVaCreateManagedWidget( "selmis_box",
			xmListWidgetClass, selmis_boxUxWin,
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNvisibleItemCount, 8,
			NULL );

	UxPutContext( selmis_box, (char *) UxSelmis_dialogContext );

	create_proc( selmis_box,
			25, (XtPointer) NULL );

	selmis_button = XtVaCreateManagedWidget( "selmis_button",
			xmRowColumnWidgetClass, selmis_row,
			XmNorientation, XmHORIZONTAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( selmis_button, (char *) UxSelmis_dialogContext );

	selmis_ok = XtVaCreateManagedWidget( "selmis_ok",
			xmPushButtonWidgetClass, selmis_button,
			RES_CONVERT( XmNlabelString, "Ok" ),
			NULL );

	UxPutContext( selmis_ok, (char *) UxSelmis_dialogContext );

	selmis_cancel = XtVaCreateManagedWidget( "selmis_cancel",
			xmPushButtonWidgetClass, selmis_button,
			RES_CONVERT( XmNlabelString, "Abbandona" ),
			NULL );

	UxPutContext( selmis_cancel, (char *) UxSelmis_dialogContext );

	find_next = XtVaCreateManagedWidget( "find_next",
			xmPushButtonWidgetClass, selmis_button,
			RES_CONVERT( XmNlabelString, "Prossima" ),
			XmNshowAsDefault, 1,
			NULL );

	UxPutContext( find_next, (char *) UxSelmis_dialogContext );

	find_previous = XtVaCreateManagedWidget( "find_previous",
			xmPushButtonWidgetClass, selmis_button,
			RES_CONVERT( XmNlabelString, "Precedente" ),
			NULL );

	UxPutContext( find_previous, (char *) UxSelmis_dialogContext );

	selmis_elimina = XtVaCreateManagedWidget( "selmis_elimina",
			xmPushButtonWidgetClass, selmis_button,
			RES_CONVERT( XmNlabelString, "Cancella" ),
			NULL );

	UxPutContext( selmis_elimina, (char *) UxSelmis_dialogContext );

	find_text = XtVaCreateManagedWidget( "find_text",
			xmTextWidgetClass, selmis_row,
			XmNcolumns, 24,
			XmNmaxLength, 24,
			NULL );

	UxPutContext( find_text, (char *) UxSelmis_dialogContext );

	create_proc( find_text,
			65, (XtPointer) NULL );

	XtAddCallback( selmis_dialog, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxSelmis_dialogContext );

	XtVaSetValues(selmis_dialog,
			XmNcancelButton, selmis_cancel,
			XmNdefaultButton, find_next,
			NULL );

	XtAddCallback( selmis_dialog, XmNmapCallback,
			map_proc,
			(XtPointer)20 );

	XtAddCallback( toggle_mis1, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)21 );

	XtAddCallback( toggle_mis2, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)22 );

	XtAddCallback( toggle_mis3, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)23 );

	XtAddCallback( toggle_mis4, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)24 );

	XtAddCallback( selmis_box, XmNsingleSelectionCallback,
			apply_proc,
			(XtPointer)25 );

	XtAddCallback( selmis_ok, XmNactivateCallback,
			ok_proc,
			(XtPointer)20 );

	XtAddCallback( selmis_cancel, XmNactivateCallback,
			cancel_proc,
			(XtPointer)20 );

	XtAddCallback( find_next, XmNactivateCallback,
			find_proc,
			(XtPointer)66 );

	XtAddCallback( find_previous, XmNactivateCallback,
			find_proc,
			(XtPointer)67 );

	XtAddCallback( selmis_elimina, XmNactivateCallback,
			apply_proc,
			(XtPointer)68 );

	return ( selmis_dialog );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_selmis_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget                  rtrn;
	_UxCselmis_dialog       *UxContext;

	UxSelmis_dialogContext = UxContext =
		(_UxCselmis_dialog *) XtMalloc( sizeof(_UxCselmis_dialog) );

	rtrn = _Uxbuild_selmis_dialog(PuntGraf);

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_selmis_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_selmis_dialog(PuntGraf);

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

