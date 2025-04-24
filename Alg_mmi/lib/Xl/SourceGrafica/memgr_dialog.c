/**********************************************************************
*
*       C Source:               memgr_dialog.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 16 17:11:54 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: memgr_dialog.c,2 %  (%full_filespec: 1,csrc,memgr_dialog.c,2 %)";
#endif
/*
   modulo memgr_dialog.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)memgr_dialog.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)memgr_dialog.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*******************************************************************************
	memgr_dialog.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/Text.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/RowColumn.h>
#include <Xm/BulletinB.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <Xl/XlGrafico.h>
#include "memgr_dialog.h"

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
	Widget	Uxmemgr_dialog;
	Widget	Uxmemgr_row;
	Widget	Uxlist_memgrUxWin;
	Widget	Uxlist_memgr;
	Widget	Uxtext_memgr;
	Widget	Uxmemgr_row1;
	Widget	Uxmemgr_ok;
	Widget	Uxmemgr_cancel;
} _UxCmemgr_dialog;

#define memgr_Graf		UxMemgr_dialogContext->UxPXlGrafico
#define memgr_dialog            UxMemgr_dialogContext->Uxmemgr_dialog
#define memgr_row               UxMemgr_dialogContext->Uxmemgr_row
#define list_memgrUxWin         UxMemgr_dialogContext->Uxlist_memgrUxWin
#define list_memgr              UxMemgr_dialogContext->Uxlist_memgr
#define text_memgr              UxMemgr_dialogContext->Uxtext_memgr
#define memgr_row1              UxMemgr_dialogContext->Uxmemgr_row1
#define memgr_ok                UxMemgr_dialogContext->Uxmemgr_ok
#define memgr_cancel            UxMemgr_dialogContext->Uxmemgr_cancel

static _UxCmemgr_dialog	*UxMemgr_dialogContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_memgr_dialog();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_memgr_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget	memgr_dialog_shell;

/*  Setto il puntatore alla struttura delle variabili del grafics  */
memgr_Graf = PuntGraf;

	memgr_dialog_shell = XtVaCreatePopupShell( "memgr_dialog_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNtitle, "memgr_dialog",
			NULL );

	memgr_dialog = XtVaCreateWidget( "memgr_dialog",
			xmBulletinBoardWidgetClass, memgr_dialog_shell,
			RES_CONVERT( XmNdialogTitle, "memorizza gruppi" ),
			XmNallowOverlap, FALSE,
			XmNdialogStyle, XmDIALOG_MODELESS,
			NULL );

	UxPutContext( memgr_dialog, (char *) UxMemgr_dialogContext );

	create_proc( memgr_dialog,
			57, (XtPointer) NULL );

	memgr_row = XtVaCreateManagedWidget( "memgr_row",
			xmRowColumnWidgetClass, memgr_dialog,
			XmNorientation, XmVERTICAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( memgr_row, (char *) UxMemgr_dialogContext );

	list_memgrUxWin = XtVaCreateManagedWidget( "list_memgrUxWin",
			xmScrolledWindowWidgetClass, memgr_row,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			NULL );

	UxPutContext( list_memgrUxWin, (char *) UxMemgr_dialogContext );

	list_memgr = XtVaCreateManagedWidget( "list_memgr",
			xmListWidgetClass, list_memgrUxWin,
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNvisibleItemCount, 8,
			NULL );

	UxPutContext( list_memgr, (char *) UxMemgr_dialogContext );

	create_proc( list_memgr,
			58, (XtPointer) NULL );

	text_memgr = XtVaCreateManagedWidget( "text_memgr",
			xmTextWidgetClass, memgr_row,
			XmNcolumns, 24,
			XmNmaxLength, 24,
			NULL );

	UxPutContext( text_memgr, (char *) UxMemgr_dialogContext );

	create_proc( text_memgr,
			59, (XtPointer) NULL );

	memgr_row1 = XtVaCreateManagedWidget( "memgr_row1",
			xmRowColumnWidgetClass, memgr_row,
			XmNorientation, XmHORIZONTAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( memgr_row1, (char *) UxMemgr_dialogContext );

	memgr_ok = XtVaCreateManagedWidget( "memgr_ok",
			xmPushButtonWidgetClass, memgr_row1,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNshowAsDefault, 1,
			NULL );

	UxPutContext( memgr_ok, (char *) UxMemgr_dialogContext );

	memgr_cancel = XtVaCreateManagedWidget( "memgr_cancel",
			xmPushButtonWidgetClass, memgr_row1,
			RES_CONVERT( XmNlabelString, "Abbandona" ),
			NULL );

	UxPutContext( memgr_cancel, (char *) UxMemgr_dialogContext );

	XtAddCallback( memgr_dialog, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxMemgr_dialogContext );

	XtVaSetValues(memgr_dialog,
			XmNdefaultButton, memgr_ok,
			NULL );

	XtAddCallback( list_memgr, XmNsingleSelectionCallback,
			apply_proc,
			(XtPointer)58 );

	XtAddCallback( memgr_ok, XmNactivateCallback,
			ok_proc,
			(XtPointer)57 );

	XtAddCallback( memgr_cancel, XmNactivateCallback,
			cancel_proc,
			(XtPointer)57 );



	return ( memgr_dialog );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_memgr_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget                  rtrn;
	_UxCmemgr_dialog        *UxContext;

	UxMemgr_dialogContext = UxContext =
		(_UxCmemgr_dialog *) XtMalloc( sizeof(_UxCmemgr_dialog) );

	rtrn = _Uxbuild_memgr_dialog(PuntGraf);

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_memgr_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_memgr_dialog(PuntGraf);

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

