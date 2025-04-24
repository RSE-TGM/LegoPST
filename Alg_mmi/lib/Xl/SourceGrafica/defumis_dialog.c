/**********************************************************************
*
*       C Source:               defumis_dialog.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 16 17:11:18 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: defumis_dialog.c,2 %  (%full_filespec: 1,csrc,defumis_dialog.c,2 %)";
#endif
/*
   modulo defumis_dialog.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)defumis_dialog.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)defumis_dialog.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*******************************************************************************
	defumis_dialog.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/RowColumn.h>
#include <Xm/BulletinB.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "defumis_dialog.h"
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
	Widget	Uxdefumis_dialog;
	Widget	Uxdefumis_row;
	Widget	Uxlist_defumisUxWin;
	Widget	Uxlist_defumis;
	Widget	Uxdefumis_radio;
	Widget	Uxtoggle2_umis1;
	Widget	Uxtoggle2_umis2;
	Widget	Uxtoggle2_umis3;
	Widget	Uxtoggle2_umis4;
	Widget	Uxdefumis_row1;
	Widget	Uxdefumis_ok;
	Widget	Uxdefumis_cancel;
} _UxCdefumis_dialog;

#define defumis_Graf		UxDefumis_dialogContext->UxPXlGrafico
#define defumis_dialog          UxDefumis_dialogContext->Uxdefumis_dialog
#define defumis_row             UxDefumis_dialogContext->Uxdefumis_row
#define list_defumisUxWin       UxDefumis_dialogContext->Uxlist_defumisUxWin
#define list_defumis            UxDefumis_dialogContext->Uxlist_defumis
#define defumis_radio           UxDefumis_dialogContext->Uxdefumis_radio
#define toggle2_umis1           UxDefumis_dialogContext->Uxtoggle2_umis1
#define toggle2_umis2           UxDefumis_dialogContext->Uxtoggle2_umis2
#define toggle2_umis3           UxDefumis_dialogContext->Uxtoggle2_umis3
#define toggle2_umis4           UxDefumis_dialogContext->Uxtoggle2_umis4
#define defumis_row1            UxDefumis_dialogContext->Uxdefumis_row1
#define defumis_ok              UxDefumis_dialogContext->Uxdefumis_ok
#define defumis_cancel          UxDefumis_dialogContext->Uxdefumis_cancel

static _UxCdefumis_dialog	*UxDefumis_dialogContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_defumis_dialog();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_defumis_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget	defumis_dialog_shell;

/*  Setto il puntatore alla struttura delle variabili del grafics  */
defumis_Graf = PuntGraf;

	defumis_dialog_shell = XtVaCreatePopupShell( "defumis_dialog_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNtitle, "defumis_dialog",
			NULL );

	defumis_dialog = XtVaCreateWidget( "defumis_dialog",
			xmBulletinBoardWidgetClass, defumis_dialog_shell,
			XmNautoUnmanage, TRUE,
			RES_CONVERT( XmNdialogTitle, "Defaults unita di misura" ),
			XmNallowOverlap, FALSE,
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			NULL );

	UxPutContext( defumis_dialog, (char *) UxDefumis_dialogContext );

	create_proc( defumis_dialog,
			69, (XtPointer) NULL );

	defumis_row = XtVaCreateManagedWidget( "defumis_row",
			xmRowColumnWidgetClass, defumis_dialog,
			XmNorientation, XmVERTICAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( defumis_row, (char *) UxDefumis_dialogContext );

	list_defumisUxWin = XtVaCreateManagedWidget( "list_defumisUxWin",
			xmScrolledWindowWidgetClass, defumis_row,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			NULL );

	UxPutContext( list_defumisUxWin, (char *) UxDefumis_dialogContext );

	list_defumis = XtVaCreateManagedWidget( "list_defumis",
			xmListWidgetClass, list_defumisUxWin,
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNvisibleItemCount, 8,
			NULL );

	UxPutContext( list_defumis, (char *) UxDefumis_dialogContext );

	create_proc( list_defumis,
			70, (XtPointer) NULL );

	defumis_radio = XtVaCreateManagedWidget( "defumis_radio",
			xmRowColumnWidgetClass, defumis_row,
			XmNorientation, XmHORIZONTAL,
			XmNspacing, 10,
			XmNborderWidth, 0,
			XmNpacking, XmPACK_COLUMN,
			XmNradioBehavior, TRUE,
			XmNisHomogeneous, TRUE,
			XmNentryClass, xmToggleButtonGadgetClass,
			XmNrowColumnType, XmWORK_AREA,
			NULL );

	UxPutContext( defumis_radio, (char *) UxDefumis_dialogContext );

	toggle2_umis1 = XtVaCreateManagedWidget( "toggle2_umis1",
			xmToggleButtonWidgetClass, defumis_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle2_umis1, (char *) UxDefumis_dialogContext );

	create_proc( toggle2_umis1,
			71, (XtPointer) NULL );

	toggle2_umis2 = XtVaCreateManagedWidget( "toggle2_umis2",
			xmToggleButtonWidgetClass, defumis_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle2_umis2, (char *) UxDefumis_dialogContext );

	create_proc( toggle2_umis2,
			72, (XtPointer) NULL );

	toggle2_umis3 = XtVaCreateManagedWidget( "toggle2_umis3",
			xmToggleButtonWidgetClass, defumis_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle2_umis3, (char *) UxDefumis_dialogContext );

	create_proc( toggle2_umis3,
			73, (XtPointer) NULL );

	toggle2_umis4 = XtVaCreateManagedWidget( "toggle2_umis4",
			xmToggleButtonWidgetClass, defumis_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle2_umis4, (char *) UxDefumis_dialogContext );

	create_proc( toggle2_umis4,
			74, (XtPointer) NULL );

	defumis_row1 = XtVaCreateManagedWidget( "defumis_row1",
			xmRowColumnWidgetClass, defumis_row,
			XmNorientation, XmHORIZONTAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( defumis_row1, (char *) UxDefumis_dialogContext );

	defumis_ok = XtVaCreateManagedWidget( "defumis_ok",
			xmPushButtonWidgetClass, defumis_row1,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNshowAsDefault, 1,
			NULL );

	UxPutContext( defumis_ok, (char *) UxDefumis_dialogContext );

	defumis_cancel = XtVaCreateManagedWidget( "defumis_cancel",
			xmPushButtonWidgetClass, defumis_row1,
			RES_CONVERT( XmNlabelString, "Abbandona" ),
			NULL );

	UxPutContext( defumis_cancel, (char *) UxDefumis_dialogContext );

	XtAddCallback( defumis_dialog, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxDefumis_dialogContext );

	XtVaSetValues(defumis_dialog,
			XmNcancelButton, defumis_cancel,
			XmNdefaultButton, defumis_ok,
			NULL );

	XtAddCallback( defumis_dialog, XmNmapCallback,
			map_proc,
			(XtPointer)69 );

	XtAddCallback( list_defumis, XmNsingleSelectionCallback,
			apply_proc,
			(XtPointer)70 );

	XtAddCallback( toggle2_umis1, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)71 );

	XtAddCallback( toggle2_umis2, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)72 );

	XtAddCallback( toggle2_umis3, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)73 );

	XtAddCallback( toggle2_umis4, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)74 );

	XtAddCallback( defumis_ok, XmNactivateCallback,
			ok_proc,
			(XtPointer)69 );

	XtAddCallback( defumis_cancel, XmNactivateCallback,
			cancel_proc,
			(XtPointer)69 );

	return ( defumis_dialog );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_defumis_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget                  rtrn;
	_UxCdefumis_dialog      *UxContext;

	UxDefumis_dialogContext = UxContext =
		(_UxCdefumis_dialog *) XtMalloc( sizeof(_UxCdefumis_dialog) );

	rtrn = _Uxbuild_defumis_dialog(PuntGraf);

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_defumis_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_defumis_dialog(PuntGraf);

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

