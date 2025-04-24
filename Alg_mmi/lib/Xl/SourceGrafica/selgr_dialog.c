/**********************************************************************
*
*       C Source:               selgr_dialog.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 16 17:12:15 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: selgr_dialog.c,2 %  (%full_filespec: 1,csrc,selgr_dialog.c,2 %)";
#endif
/*
   modulo selgr_dialog.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)selgr_dialog.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)selgr_dialog.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*******************************************************************************
	selgr_dialog.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Separator.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/BulletinB.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <Xl/XlGrafico.h>
#include "selgr_dialog.h"

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
	Widget	Uxselgr_dialog;
	Widget	Uxselgr_row;
	Widget	Uxlabel_grmis1;
	Widget	Uxlabel_grmis2;
	Widget	Uxlabel_grmis3;
	Widget	Uxlabel_grmis4;
	Widget	Uxgrafici9;
	Widget	Uxlist_grUxWin;
	Widget	Uxlist_gr;
	Widget	Uxselgr_row1;
	Widget	Uxselgr_ok;
	Widget	Uxselgr_cancel;
} _UxCselgr_dialog;

#define selgr_Graf		UxSelgr_dialogContext->UxPXlGrafico
#define selgr_dialog            UxSelgr_dialogContext->Uxselgr_dialog
#define selgr_row               UxSelgr_dialogContext->Uxselgr_row
#define label_grmis1            UxSelgr_dialogContext->Uxlabel_grmis1
#define label_grmis2            UxSelgr_dialogContext->Uxlabel_grmis2
#define label_grmis3            UxSelgr_dialogContext->Uxlabel_grmis3
#define label_grmis4            UxSelgr_dialogContext->Uxlabel_grmis4
#define grafici9                UxSelgr_dialogContext->Uxgrafici9
#define list_grUxWin            UxSelgr_dialogContext->Uxlist_grUxWin
#define list_gr                 UxSelgr_dialogContext->Uxlist_gr
#define selgr_row1              UxSelgr_dialogContext->Uxselgr_row1
#define selgr_ok                UxSelgr_dialogContext->Uxselgr_ok
#define selgr_cancel            UxSelgr_dialogContext->Uxselgr_cancel

static _UxCselgr_dialog	*UxSelgr_dialogContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_selgr_dialog();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_selgr_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget	selgr_dialog_shell;

/*  Setto il puntatore alla struttura delle variabili del grafics  */
selgr_Graf = PuntGraf;

	selgr_dialog_shell = XtVaCreatePopupShell( "selgr_dialog_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNtitle, "selgr_dialog",
			NULL );

	selgr_dialog = XtVaCreateWidget( "selgr_dialog",
			xmBulletinBoardWidgetClass, selgr_dialog_shell,
			RES_CONVERT( XmNdialogTitle, "Selezione gruppi" ),
			XmNallowOverlap, FALSE,
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			NULL );

	UxPutContext( selgr_dialog, (char *) UxSelgr_dialogContext );

	create_proc( selgr_dialog,
			50, (XtPointer) NULL );

	selgr_row = XtVaCreateManagedWidget( "selgr_row",
			xmRowColumnWidgetClass, selgr_dialog,
			XmNorientation, XmVERTICAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( selgr_row, (char *) UxSelgr_dialogContext );

	label_grmis1 = XtVaCreateManagedWidget( "label_grmis1",
			xmLabelWidgetClass, selgr_row,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNwidth, 800,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( label_grmis1, (char *) UxSelgr_dialogContext );

	create_proc( label_grmis1,
			53, (XtPointer) NULL );

	label_grmis2 = XtVaCreateManagedWidget( "label_grmis2",
			xmLabelWidgetClass, selgr_row,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( label_grmis2, (char *) UxSelgr_dialogContext );

	create_proc( label_grmis2,
			54, (XtPointer) NULL );

	label_grmis3 = XtVaCreateManagedWidget( "label_grmis3",
			xmLabelWidgetClass, selgr_row,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( label_grmis3, (char *) UxSelgr_dialogContext );

	create_proc( label_grmis3,
			55, (XtPointer) NULL );

	label_grmis4 = XtVaCreateManagedWidget( "label_grmis4",
			xmLabelWidgetClass, selgr_row,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( label_grmis4, (char *) UxSelgr_dialogContext );

	create_proc( label_grmis4,
			56, (XtPointer) NULL );

	grafici9 = XtVaCreateManagedWidget( "grafici9",
			xmSeparatorWidgetClass, selgr_row,
			XmNorientation, XmHORIZONTAL,
			XmNwidth, 400,
			XmNheight, 3,
			NULL );

	UxPutContext( grafici9, (char *) UxSelgr_dialogContext );

	list_grUxWin = XtVaCreateManagedWidget( "list_grUxWin",
			xmScrolledWindowWidgetClass, selgr_row,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			NULL );

	UxPutContext( list_grUxWin, (char *) UxSelgr_dialogContext );

	list_gr = XtVaCreateManagedWidget( "list_gr",
			xmListWidgetClass, list_grUxWin,
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNvisibleItemCount, 8,
			NULL );

	UxPutContext( list_gr, (char *) UxSelgr_dialogContext );

	create_proc( list_gr,
			52, (XtPointer) NULL );

	selgr_row1 = XtVaCreateManagedWidget( "selgr_row1",
			xmRowColumnWidgetClass, selgr_row,
			XmNorientation, XmHORIZONTAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( selgr_row1, (char *) UxSelgr_dialogContext );

	selgr_ok = XtVaCreateManagedWidget( "selgr_ok",
			xmPushButtonWidgetClass, selgr_row1,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNshowAsDefault, 1,
			NULL );

	UxPutContext( selgr_ok, (char *) UxSelgr_dialogContext );

	selgr_cancel = XtVaCreateManagedWidget( "selgr_cancel",
			xmPushButtonWidgetClass, selgr_row1,
			RES_CONVERT( XmNlabelString, "Abbandona" ),
			NULL );

	UxPutContext( selgr_cancel, (char *) UxSelgr_dialogContext );

	XtAddCallback( selgr_dialog, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxSelgr_dialogContext );

	XtVaSetValues(selgr_dialog,
			XmNdefaultButton, selgr_ok,
			NULL );

	XtAddCallback( selgr_dialog, XmNmapCallback,
			map_proc,
			(XtPointer)50 );

	XtAddCallback( list_gr, XmNsingleSelectionCallback,
			apply_proc,
			(XtPointer)52 );

	XtAddCallback( selgr_ok, XmNactivateCallback,
			ok_proc,
			(XtPointer)50 );

	XtAddCallback( selgr_cancel, XmNactivateCallback,
			cancel_proc,
			(XtPointer)50 );



	return ( selgr_dialog );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_selgr_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget                  rtrn;
	_UxCselgr_dialog        *UxContext;

	UxSelgr_dialogContext = UxContext =
		(_UxCselgr_dialog *) XtMalloc( sizeof(_UxCselgr_dialog) );

	rtrn = _Uxbuild_selgr_dialog(PuntGraf);

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_selgr_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_selgr_dialog(PuntGraf);

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

