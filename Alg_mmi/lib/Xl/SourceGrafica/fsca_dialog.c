/**********************************************************************
*
*       C Source:               fsca_dialog.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 16 17:11:32 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: fsca_dialog.c,2 %  (%full_filespec: 1,csrc,fsca_dialog.c,2 %)";
#endif
/*
   modulo fsca_dialog.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)fsca_dialog.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)fsca_dialog.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*******************************************************************************
	fsca_dialog.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/Separator.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/BulletinB.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "fsca_dialog.h"
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
	Widget	Uxfsca_dialog;
	Widget	Uxfsca_row;
	Widget	Uxselmis3_radio;
	Widget	Uxtoggle3_mis1;
	Widget	Uxtoggle3_mis2;
	Widget	Uxtoggle3_mis3;
	Widget	Uxtoggle3_mis4;
	Widget	Uxgrafici5;
	Widget	Uxfsca_row1;
	Widget	Uxscaling_radio;
	Widget	Uxtoggle_auto;
	Widget	Uxtoggle_man;
	Widget	Uxgrafici6;
	Widget	Uxlabel_row;
	Widget	Uxgrafici7;
	Widget	Uxgrafici8;
	Widget	Uxtext_row;
	Widget	Uxtext_max;
	Widget	Uxtext_min;
	Widget	Uxfsca_row2;
	Widget	Uxfsca_ok;
	Widget	Uxfsca_cancel;
} _UxCfsca_dialog;

#define fsca_Graf		UxFsca_dialogContext->UxPXlGrafico
#define fsca_dialog             UxFsca_dialogContext->Uxfsca_dialog
#define fsca_row                UxFsca_dialogContext->Uxfsca_row
#define selmis3_radio           UxFsca_dialogContext->Uxselmis3_radio
#define toggle3_mis1            UxFsca_dialogContext->Uxtoggle3_mis1
#define toggle3_mis2            UxFsca_dialogContext->Uxtoggle3_mis2
#define toggle3_mis3            UxFsca_dialogContext->Uxtoggle3_mis3
#define toggle3_mis4            UxFsca_dialogContext->Uxtoggle3_mis4
#define grafici5                UxFsca_dialogContext->Uxgrafici5
#define fsca_row1               UxFsca_dialogContext->Uxfsca_row1
#define scaling_radio           UxFsca_dialogContext->Uxscaling_radio
#define toggle_auto             UxFsca_dialogContext->Uxtoggle_auto
#define toggle_man              UxFsca_dialogContext->Uxtoggle_man
#define grafici6                UxFsca_dialogContext->Uxgrafici6
#define label_row               UxFsca_dialogContext->Uxlabel_row
#define grafici7                UxFsca_dialogContext->Uxgrafici7
#define grafici8                UxFsca_dialogContext->Uxgrafici8
#define text_row                UxFsca_dialogContext->Uxtext_row
#define text_max                UxFsca_dialogContext->Uxtext_max
#define text_min                UxFsca_dialogContext->Uxtext_min
#define fsca_row2               UxFsca_dialogContext->Uxfsca_row2
#define fsca_ok                 UxFsca_dialogContext->Uxfsca_ok
#define fsca_cancel             UxFsca_dialogContext->Uxfsca_cancel

static _UxCfsca_dialog	*UxFsca_dialogContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_fsca_dialog();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_fsca_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	S_XLGRAFICO *pXlGraf;
	Widget	fsca_dialog_shell;

/*  Setto il puntatore della struttura variabili del grafics  */
fsca_Graf = PuntGraf;
pXlGraf = PuntGraf;

	fsca_dialog_shell = XtVaCreatePopupShell( "fsca_dialog_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNtitle, "fsca_dialog",
			NULL );

	fsca_dialog = XtVaCreateWidget( "fsca_dialog",
			xmBulletinBoardWidgetClass, fsca_dialog_shell,
			XmNallowOverlap, FALSE,
			XmNautoUnmanage, TRUE,
			RES_CONVERT( XmNdialogTitle, "Modifica valori di fondo scala" ),
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			XmNmarginWidth, 20,
			NULL );

	UxPutContext( fsca_dialog, (char *) UxFsca_dialogContext );

	create_proc( fsca_dialog,
			41, (XtPointer) NULL );

	fsca_row = XtVaCreateManagedWidget( "fsca_row",
			xmRowColumnWidgetClass, fsca_dialog,
			XmNorientation, XmVERTICAL,
			XmNnumColumns, 1,
			XmNspacing, 30,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( fsca_row, (char *) UxFsca_dialogContext );

	selmis3_radio = XtVaCreateManagedWidget( "selmis3_radio",
			xmRowColumnWidgetClass, fsca_row,
			XmNorientation, XmVERTICAL,
			XmNborderWidth, 0,
			XmNpacking, XmPACK_COLUMN,
			XmNradioBehavior, TRUE,
			XmNisHomogeneous, TRUE,
			XmNentryClass, xmToggleButtonGadgetClass,
			XmNrowColumnType, XmWORK_AREA,
			NULL );

	UxPutContext( selmis3_radio, (char *) UxFsca_dialogContext );

	toggle3_mis1 = XtVaCreateManagedWidget( "toggle3_mis1",
			xmToggleButtonWidgetClass, selmis3_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle3_mis1, (char *) UxFsca_dialogContext );

	create_proc( toggle3_mis1,
			42, (XtPointer) NULL );

	toggle3_mis2 = XtVaCreateManagedWidget( "toggle3_mis2",
			xmToggleButtonWidgetClass, selmis3_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle3_mis2, (char *) UxFsca_dialogContext );

	create_proc( toggle3_mis2,
			43, (XtPointer) NULL );

	toggle3_mis3 = XtVaCreateManagedWidget( "toggle3_mis3",
			xmToggleButtonWidgetClass, selmis3_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle3_mis3, (char *) UxFsca_dialogContext );

	create_proc( toggle3_mis3,
			44, (XtPointer) NULL );

	toggle3_mis4 = XtVaCreateManagedWidget( "toggle3_mis4",
			xmToggleButtonWidgetClass, selmis3_radio,
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle3_mis4, (char *) UxFsca_dialogContext );

	create_proc( toggle3_mis4,
			45, (XtPointer) NULL );

	grafici5 = XtVaCreateManagedWidget( "grafici5",
			xmSeparatorWidgetClass, fsca_row,
			XmNorientation, XmHORIZONTAL,
			NULL );

	UxPutContext( grafici5, (char *) UxFsca_dialogContext );

	fsca_row1 = XtVaCreateManagedWidget( "fsca_row1",
			xmRowColumnWidgetClass, fsca_row,
			XmNorientation, XmHORIZONTAL,
			XmNspacing, 50,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( fsca_row1, (char *) UxFsca_dialogContext );

	scaling_radio = XtVaCreateManagedWidget( "scaling_radio",
			xmRowColumnWidgetClass, fsca_row1,
			XmNorientation, XmVERTICAL,
			XmNborderWidth, 0,
			XmNpacking, XmPACK_COLUMN,
			XmNradioBehavior, TRUE,
			XmNisHomogeneous, TRUE,
			XmNentryClass, xmToggleButtonGadgetClass,
			XmNrowColumnType, XmWORK_AREA,
			NULL );

	UxPutContext( scaling_radio, (char *) UxFsca_dialogContext );

	toggle_auto = XtVaCreateManagedWidget( "toggle_auto",
			xmToggleButtonWidgetClass, scaling_radio,
			RES_CONVERT( XmNlabelString, "auto-scaling" ),
			NULL );

	UxPutContext( toggle_auto, (char *) UxFsca_dialogContext );

	create_proc( toggle_auto,
			49, (XtPointer) NULL );

	toggle_man = XtVaCreateManagedWidget( "toggle_man",
			xmToggleButtonWidgetClass, scaling_radio,
			RES_CONVERT( XmNlabelString, "scala fissa" ),
			NULL );

	UxPutContext( toggle_man, (char *) UxFsca_dialogContext );

	create_proc( toggle_man,
			48, (XtPointer) NULL );

	grafici6 = XtVaCreateManagedWidget( "grafici6",
			xmSeparatorWidgetClass, fsca_row1,
			XmNorientation, XmVERTICAL,
			NULL );

	UxPutContext( grafici6, (char *) UxFsca_dialogContext );

	label_row = XtVaCreateManagedWidget( "label_row",
			xmRowColumnWidgetClass, fsca_row1,
			XmNorientation, XmVERTICAL,
			XmNnumColumns, 1,
			XmNspacing, 15,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( label_row, (char *) UxFsca_dialogContext );

	grafici7 = XtVaCreateManagedWidget( "grafici7",
			xmLabelWidgetClass, label_row,
			RES_CONVERT( XmNlabelString, "massimo :" ),
			XmNmarginLeft, 30,
			XmNmarginTop, 5,
			NULL );

	UxPutContext( grafici7, (char *) UxFsca_dialogContext );

	grafici8 = XtVaCreateManagedWidget( "grafici8",
			xmLabelWidgetClass, label_row,
			RES_CONVERT( XmNlabelString, "minimo :" ),
			XmNmarginLeft, 30,
			XmNmarginTop, 5,
			NULL );

	UxPutContext( grafici8, (char *) UxFsca_dialogContext );

	text_row = XtVaCreateManagedWidget( "text_row",
			xmRowColumnWidgetClass, fsca_row1,
			XmNorientation, XmVERTICAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( text_row, (char *) UxFsca_dialogContext );

	text_max = XtVaCreateManagedWidget( "text_max",
			xmTextWidgetClass, text_row,
			XmNcolumns, 12,
			XmNmaxLength, 12,
			NULL );

	UxPutContext( text_max, (char *) UxFsca_dialogContext );

	create_proc( text_max,
			46, (XtPointer) NULL );

	text_min = XtVaCreateManagedWidget( "text_min",
			xmTextWidgetClass, text_row,
			XmNcolumns, 12,
			XmNmaxLength, 12,
			NULL );

	UxPutContext( text_min, (char *) UxFsca_dialogContext );

	create_proc( text_min,
			47, (XtPointer) NULL );

	fsca_row2 = XtVaCreateManagedWidget( "fsca_row2",
			xmRowColumnWidgetClass, fsca_row,
			XmNorientation, XmHORIZONTAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( fsca_row2, (char *) UxFsca_dialogContext );

	fsca_ok = XtVaCreateManagedWidget( "fsca_ok",
			xmPushButtonWidgetClass, fsca_row2,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNshowAsDefault, 1,
			NULL );

	UxPutContext( fsca_ok, (char *) UxFsca_dialogContext );

	fsca_cancel = XtVaCreateManagedWidget( "fsca_cancel",
			xmPushButtonWidgetClass, fsca_row2,
			RES_CONVERT( XmNlabelString, "Abbandona" ),
			NULL );

	UxPutContext( fsca_cancel, (char *) UxFsca_dialogContext );

	XtAddCallback( fsca_dialog, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxFsca_dialogContext );

	XtVaSetValues(fsca_dialog,
			XmNcancelButton, fsca_cancel,
			XmNdefaultButton, fsca_ok,
			NULL );

	XtAddCallback( fsca_dialog, XmNmapCallback,
			map_proc,
			(XtPointer)41 );

	XtAddCallback( toggle3_mis1, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)42 );

	XtAddCallback( toggle3_mis2, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)43 );

	XtAddCallback( toggle3_mis3, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)44 );

	XtAddCallback( toggle3_mis4, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)45 );

	XtAddCallback( toggle_auto, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)49 );

	XtAddCallback( toggle_man, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)48 );

	XtAddCallback( fsca_ok, XmNactivateCallback,
			ok_proc,
			(XtPointer)41 );

	XtAddCallback( fsca_cancel, XmNactivateCallback,
			cancel_proc,
			(XtPointer)41 );

	return ( fsca_dialog );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_fsca_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget                  rtrn;
	_UxCfsca_dialog         *UxContext;

	UxFsca_dialogContext = UxContext =
		(_UxCfsca_dialog *) XtMalloc( sizeof(_UxCfsca_dialog) );

	rtrn = _Uxbuild_fsca_dialog(PuntGraf);

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_fsca_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_fsca_dialog(PuntGraf);

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

