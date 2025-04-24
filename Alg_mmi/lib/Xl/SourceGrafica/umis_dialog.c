/**********************************************************************
*
*       C Source:               umis_dialog.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 16 17:12:00 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: umis_dialog.c,2 %  (%full_filespec: 1,csrc,umis_dialog.c,2 %)";
#endif
/*
   modulo umis_dialog.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)umis_dialog.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)umis_dialog.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*******************************************************************************
	umis_dialog.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/Separator.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/BulletinB.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "umis_dialog.h"
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
	Widget	Uxumis_dialog;
	Widget	Uxumis_row;
	Widget	Uxselmis2_radio;
	Widget	Uxtoggle2_mis1;
	Widget	Uxtoggle2_mis2;
	Widget	Uxtoggle2_mis3;
	Widget	Uxtoggle2_mis4;
	Widget	Uxgrafici4;
	Widget	Uxumis_label;
	Widget	Uxselumis_radio;
	Widget	Uxtoggle_umis1;
	Widget	Uxtoggle_umis2;
	Widget	Uxtoggle_umis3;
	Widget	Uxtoggle_umis4;
	Widget	Uxumis_row1;
	Widget	Uxumis_ok;
	Widget	Uxumis_cancel;
} _UxCumis_dialog;

#define umis_Graf		UxUmis_dialogContext->UxPXlGrafico
#define umis_dialog             UxUmis_dialogContext->Uxumis_dialog
#define umis_row                UxUmis_dialogContext->Uxumis_row
#define selmis2_radio           UxUmis_dialogContext->Uxselmis2_radio
#define toggle2_mis1            UxUmis_dialogContext->Uxtoggle2_mis1
#define toggle2_mis2            UxUmis_dialogContext->Uxtoggle2_mis2
#define toggle2_mis3            UxUmis_dialogContext->Uxtoggle2_mis3
#define toggle2_mis4            UxUmis_dialogContext->Uxtoggle2_mis4
#define grafici4                UxUmis_dialogContext->Uxgrafici4
#define umis_label              UxUmis_dialogContext->Uxumis_label
#define selumis_radio           UxUmis_dialogContext->Uxselumis_radio
#define toggle_umis1            UxUmis_dialogContext->Uxtoggle_umis1
#define toggle_umis2            UxUmis_dialogContext->Uxtoggle_umis2
#define toggle_umis3            UxUmis_dialogContext->Uxtoggle_umis3
#define toggle_umis4            UxUmis_dialogContext->Uxtoggle_umis4
#define umis_row1               UxUmis_dialogContext->Uxumis_row1
#define umis_ok                 UxUmis_dialogContext->Uxumis_ok
#define umis_cancel             UxUmis_dialogContext->Uxumis_cancel

static _UxCumis_dialog	*UxUmis_dialogContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_umis_dialog();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_umis_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget	umis_dialog_shell;

/*  Setto il puntatore alla struttura delle variabili del grafics  */
umis_Graf = PuntGraf;

	umis_dialog_shell = XtVaCreatePopupShell( "umis_dialog_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNtitle, "umis_dialog",
			NULL );

	umis_dialog = XtVaCreateWidget( "umis_dialog",
			xmBulletinBoardWidgetClass, umis_dialog_shell,
			XmNallowOverlap, FALSE,
			RES_CONVERT( XmNdialogTitle, "Scelta unita di misura" ),
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			XmNmarginWidth, 20,
			NULL );

	UxPutContext( umis_dialog, (char *) UxUmis_dialogContext );

	create_proc( umis_dialog,
			32, (XtPointer) NULL );

	umis_row = XtVaCreateManagedWidget( "umis_row",
			xmRowColumnWidgetClass, umis_dialog,
			XmNorientation, XmVERTICAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( umis_row, (char *) UxUmis_dialogContext );

	selmis2_radio = XtVaCreateManagedWidget( "selmis2_radio",
			xmRowColumnWidgetClass, umis_row,
			XmNorientation, XmVERTICAL,
			XmNborderWidth, 0,
			XmNpacking, XmPACK_COLUMN,
			XmNradioBehavior, TRUE,
			XmNisHomogeneous, TRUE,
			XmNentryClass, xmToggleButtonGadgetClass,
			XmNrowColumnType, XmWORK_AREA,
			NULL );

	UxPutContext( selmis2_radio, (char *) UxUmis_dialogContext );

	toggle2_mis1 = XtVaCreateManagedWidget( "toggle2_mis1",
			xmToggleButtonWidgetClass, selmis2_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle2_mis1, (char *) UxUmis_dialogContext );

	create_proc( toggle2_mis1,
			37, (XtPointer) NULL );

	toggle2_mis2 = XtVaCreateManagedWidget( "toggle2_mis2",
			xmToggleButtonWidgetClass, selmis2_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle2_mis2, (char *) UxUmis_dialogContext );

	create_proc( toggle2_mis2,
			38, (XtPointer) NULL );

	toggle2_mis3 = XtVaCreateManagedWidget( "toggle2_mis3",
			xmToggleButtonWidgetClass, selmis2_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle2_mis3, (char *) UxUmis_dialogContext );

	create_proc( toggle2_mis3,
			39, (XtPointer) NULL );

	toggle2_mis4 = XtVaCreateManagedWidget( "toggle2_mis4",
			xmToggleButtonWidgetClass, selmis2_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle2_mis4, (char *) UxUmis_dialogContext );

	create_proc( toggle2_mis4,
			40, (XtPointer) NULL );

	grafici4 = XtVaCreateManagedWidget( "grafici4",
			xmSeparatorWidgetClass, umis_row,
			XmNorientation, XmHORIZONTAL,
			XmNheight, 3,
			XmNwidth, 400,
			NULL );

	UxPutContext( grafici4, (char *) UxUmis_dialogContext );

	umis_label = XtVaCreateManagedWidget( "umis_label",
			xmLabelWidgetClass, umis_row,
			RES_CONVERT( XmNlabelString, "Unita di misura ammesse :" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );

	UxPutContext( umis_label, (char *) UxUmis_dialogContext );

	selumis_radio = XtVaCreateManagedWidget( "selumis_radio",
			xmRowColumnWidgetClass, umis_row,
			XmNorientation, XmHORIZONTAL,
			XmNborderWidth, 0,
			XmNpacking, XmPACK_COLUMN,
			XmNradioBehavior, TRUE,
			XmNisHomogeneous, TRUE,
			XmNentryClass, xmToggleButtonGadgetClass,
			XmNrowColumnType, XmWORK_AREA,
			NULL );

	UxPutContext( selumis_radio, (char *) UxUmis_dialogContext );

	toggle_umis1 = XtVaCreateManagedWidget( "toggle_umis1",
			xmToggleButtonWidgetClass, selumis_radio,
			NULL );

	UxPutContext( toggle_umis1, (char *) UxUmis_dialogContext );

	create_proc( toggle_umis1,
			33, (XtPointer) NULL );

	toggle_umis2 = XtVaCreateManagedWidget( "toggle_umis2",
			xmToggleButtonWidgetClass, selumis_radio,
			NULL );

	UxPutContext( toggle_umis2, (char *) UxUmis_dialogContext );

	create_proc( toggle_umis2,
			34, (XtPointer) NULL );

	toggle_umis3 = XtVaCreateManagedWidget( "toggle_umis3",
			xmToggleButtonWidgetClass, selumis_radio,
			NULL );

	UxPutContext( toggle_umis3, (char *) UxUmis_dialogContext );

	create_proc( toggle_umis3,
			35, (XtPointer) NULL );

	toggle_umis4 = XtVaCreateManagedWidget( "toggle_umis4",
			xmToggleButtonWidgetClass, selumis_radio,
			NULL );

	UxPutContext( toggle_umis4, (char *) UxUmis_dialogContext );

	create_proc( toggle_umis4,
			36, (XtPointer) NULL );

	umis_row1 = XtVaCreateManagedWidget( "umis_row1",
			xmRowColumnWidgetClass, umis_row,
			XmNorientation, XmHORIZONTAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( umis_row1, (char *) UxUmis_dialogContext );

	umis_ok = XtVaCreateManagedWidget( "umis_ok",
			xmPushButtonWidgetClass, umis_row1,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNshowAsDefault, 1,
			NULL );

	UxPutContext( umis_ok, (char *) UxUmis_dialogContext );

	umis_cancel = XtVaCreateManagedWidget( "umis_cancel",
			xmPushButtonWidgetClass, umis_row1,
			RES_CONVERT( XmNlabelString, "Abbandona" ),
			NULL );

	UxPutContext( umis_cancel, (char *) UxUmis_dialogContext );

	XtAddCallback( umis_dialog, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxUmis_dialogContext );

	XtVaSetValues(umis_dialog,
			XmNcancelButton, umis_cancel,
			XmNdefaultButton, umis_ok,
			NULL );

	XtAddCallback( umis_dialog, XmNmapCallback,
			map_proc,
			(XtPointer)32 );

	XtAddCallback( toggle2_mis1, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)37 );

	XtAddCallback( toggle2_mis2, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)38 );

	XtAddCallback( toggle2_mis3, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)39 );

	XtAddCallback( toggle2_mis4, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)40 );

	XtAddCallback( toggle_umis1, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)33 );

	XtAddCallback( toggle_umis2, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)34 );

	XtAddCallback( toggle_umis3, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)35 );

	XtAddCallback( toggle_umis4, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)36 );

	XtAddCallback( umis_ok, XmNactivateCallback,
			ok_proc,
			(XtPointer)32 );

	XtAddCallback( umis_cancel, XmNactivateCallback,
			cancel_proc,
			(XtPointer)32 );

	return ( umis_dialog );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_umis_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget                  rtrn;
	_UxCumis_dialog         *UxContext;

	UxUmis_dialogContext = UxContext =
		(_UxCumis_dialog *) XtMalloc( sizeof(_UxCumis_dialog) );

	rtrn = _Uxbuild_umis_dialog(PuntGraf);

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_umis_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_umis_dialog(PuntGraf);

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

