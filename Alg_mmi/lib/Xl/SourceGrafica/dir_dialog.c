/**********************************************************************
*
*       C Source:               dir_dialog.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 16 17:11:24 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: dir_dialog.c,2 %  (%full_filespec: 1,csrc,dir_dialog.c,2 %)";
#endif
/*
   modulo dir_dialog.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)dir_dialog.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)dir_dialog.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*******************************************************************************
	dir_dialog.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/BulletinB.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "dir_dialog.h"
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
	Widget	Uxdir_dialog;
	Widget	Uxdir_row;
	Widget	Uxdir_label;
	Widget	Uxdir_text;
	Widget	Uxdir_radio;
	Widget	Uxtoggle_dir1;
	Widget	Uxtoggle_dir2;
	Widget	Uxtoggle_dir3;
	Widget	Uxtoggle_dir4;
	Widget	Uxdir_text1;
	Widget	Uxtext_dir1;
	Widget	Uxtext_dir2;
	Widget	Uxtext_dir3;
	Widget	Uxtext_dir4;
	Widget	Uxdir_button;
	Widget	Uxdir_ok;
	Widget	Uxdir_cancel;
} _UxCdir_dialog;

#define dir_Graf		UxDir_dialogContext->UxPXlGrafico
#define dir_dialog              UxDir_dialogContext->Uxdir_dialog
#define dir_row                 UxDir_dialogContext->Uxdir_row
#define dir_label               UxDir_dialogContext->Uxdir_label
#define dir_text                UxDir_dialogContext->Uxdir_text
#define dir_radio               UxDir_dialogContext->Uxdir_radio
#define toggle_dir1             UxDir_dialogContext->Uxtoggle_dir1
#define toggle_dir2             UxDir_dialogContext->Uxtoggle_dir2
#define toggle_dir3             UxDir_dialogContext->Uxtoggle_dir3
#define toggle_dir4             UxDir_dialogContext->Uxtoggle_dir4
#define dir_text1               UxDir_dialogContext->Uxdir_text1
#define text_dir1               UxDir_dialogContext->Uxtext_dir1
#define text_dir2               UxDir_dialogContext->Uxtext_dir2
#define text_dir3               UxDir_dialogContext->Uxtext_dir3
#define text_dir4               UxDir_dialogContext->Uxtext_dir4
#define dir_button              UxDir_dialogContext->Uxdir_button
#define dir_ok                  UxDir_dialogContext->Uxdir_ok
#define dir_cancel              UxDir_dialogContext->Uxdir_cancel

static _UxCdir_dialog	*UxDir_dialogContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_dir_dialog();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_dir_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget	dir_dialog_shell;

/*  Setto il puntatore alla struttira delle variabili del grafics  */
dir_Graf = PuntGraf;

	dir_dialog_shell = XtVaCreatePopupShell( "dir_dialog_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 200,
			XmNtitle, "dir_dialog",
			NULL );

	dir_dialog = XtVaCreateWidget( "dir_dialog",
			xmBulletinBoardWidgetClass, dir_dialog_shell,
			RES_CONVERT( XmNdialogTitle, "Selezione files dati" ),
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			XmNmarginWidth, 20,
			NULL );

	UxPutContext( dir_dialog, (char *) UxDir_dialogContext );

	create_proc( dir_dialog,
			75, (XtPointer) NULL );

	dir_row = XtVaCreateManagedWidget( "dir_row",
			xmRowColumnWidgetClass, dir_dialog,
			XmNorientation, XmVERTICAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( dir_row, (char *) UxDir_dialogContext );

	dir_label = XtVaCreateManagedWidget( "dir_label",
			xmLabelWidgetClass, dir_row,
			RES_CONVERT( XmNlabelString, "Apertura file dati" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );

	UxPutContext( dir_label, (char *) UxDir_dialogContext );

	dir_text = XtVaCreateManagedWidget( "dir_text",
			xmRowColumnWidgetClass, dir_row,
			XmNorientation, XmHORIZONTAL,
			XmNnumColumns, 1,
			XmNspacing, 2,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( dir_text, (char *) UxDir_dialogContext );

	dir_radio = XtVaCreateManagedWidget( "dir_radio",
			xmRowColumnWidgetClass, dir_text,
			XmNspacing, 16,
			XmNpacking, XmPACK_COLUMN,
			XmNradioBehavior, TRUE,
			XmNisHomogeneous, TRUE,
			XmNentryClass, xmToggleButtonGadgetClass,
			XmNrowColumnType, XmWORK_AREA,
			NULL );

	UxPutContext( dir_radio, (char *) UxDir_dialogContext );

	toggle_dir1 = XtVaCreateManagedWidget( "toggle_dir1",
			xmToggleButtonWidgetClass, dir_radio,
			RES_CONVERT( XmNlabelString, "  " ),
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle_dir1, (char *) UxDir_dialogContext );

	create_proc( toggle_dir1,
			80, (XtPointer) NULL );

	toggle_dir2 = XtVaCreateManagedWidget( "toggle_dir2",
			xmToggleButtonWidgetClass, dir_radio,
			RES_CONVERT( XmNlabelString, "  " ),
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle_dir2, (char *) UxDir_dialogContext );

	create_proc( toggle_dir2,
			81, (XtPointer) NULL );

	toggle_dir3 = XtVaCreateManagedWidget( "toggle_dir3",
			xmToggleButtonWidgetClass, dir_radio,
			RES_CONVERT( XmNlabelString, "  " ),
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle_dir3, (char *) UxDir_dialogContext );

	create_proc( toggle_dir3,
			82, (XtPointer) NULL );

	toggle_dir4 = XtVaCreateManagedWidget( "toggle_dir4",
			xmToggleButtonWidgetClass, dir_radio,
			RES_CONVERT( XmNlabelString, "  " ),
			XmNrecomputeSize, FALSE,
			NULL );

	UxPutContext( toggle_dir4, (char *) UxDir_dialogContext );

	create_proc( toggle_dir4,
			83, (XtPointer) NULL );

	dir_text1 = XtVaCreateManagedWidget( "dir_text1",
			xmRowColumnWidgetClass, dir_text,
			XmNorientation, XmVERTICAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( dir_text1, (char *) UxDir_dialogContext );

	text_dir1 = XtVaCreateManagedWidget( "text_dir1",
			xmTextWidgetClass, dir_text1,
			XmNcolumns, 50,
			XmNmaxLength, 100,
			NULL );

	UxPutContext( text_dir1, (char *) UxDir_dialogContext );

	create_proc( text_dir1,
			76, (XtPointer) NULL );

	text_dir2 = XtVaCreateManagedWidget( "text_dir2",
			xmTextWidgetClass, dir_text1,
			XmNcolumns, 50,
			XmNmaxLength, 100,
			NULL );

	UxPutContext( text_dir2, (char *) UxDir_dialogContext );

	create_proc( text_dir2,
			77, (XtPointer) NULL );

	text_dir3 = XtVaCreateManagedWidget( "text_dir3",
			xmTextWidgetClass, dir_text1,
			XmNcolumns, 50,
			XmNmaxLength, 100,
			NULL );

	UxPutContext( text_dir3, (char *) UxDir_dialogContext );

	create_proc( text_dir3,
			78, (XtPointer) NULL );

	text_dir4 = XtVaCreateManagedWidget( "text_dir4",
			xmTextWidgetClass, dir_text1,
			XmNcolumns, 50,
			XmNmaxLength, 100,
			NULL );

	UxPutContext( text_dir4, (char *) UxDir_dialogContext );

	create_proc( text_dir4,
			79, (XtPointer) NULL );

	dir_button = XtVaCreateManagedWidget( "dir_button",
			xmRowColumnWidgetClass, dir_row,
			XmNorientation, XmHORIZONTAL,
			XmNnumColumns, 1,
			XmNspacing, 10,
			XmNunitType, XmPIXELS,
			XmNborderWidth, 0,
			NULL );

	UxPutContext( dir_button, (char *) UxDir_dialogContext );

	dir_ok = XtVaCreateManagedWidget( "dir_ok",
			xmPushButtonWidgetClass, dir_button,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNshowAsDefault, 1,
			NULL );

	UxPutContext( dir_ok, (char *) UxDir_dialogContext );

	dir_cancel = XtVaCreateManagedWidget( "dir_cancel",
			xmPushButtonWidgetClass, dir_button,
			RES_CONVERT( XmNlabelString, "Abbandona" ),
			NULL );

	UxPutContext( dir_cancel, (char *) UxDir_dialogContext );

	XtAddCallback( dir_dialog, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxDir_dialogContext );

	XtVaSetValues(dir_dialog,
			XmNdefaultButton, dir_ok,
			NULL );

	XtAddCallback( dir_dialog, XmNmapCallback,
			map_proc,
			(XtPointer)75 );

	XtAddCallback( toggle_dir1, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)80 );

	XtAddCallback( toggle_dir2, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)81 );

	XtAddCallback( toggle_dir3, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)82 );

	XtAddCallback( toggle_dir4, XmNvalueChangedCallback,
			toggle_proc,
			(XtPointer)83 );

	XtAddCallback( dir_ok, XmNactivateCallback,
			ok_proc,
			(XtPointer)75 );

	XtAddCallback( dir_cancel, XmNactivateCallback,
			cancel_proc,
			(XtPointer)75 );



	return ( dir_dialog );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_dir_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget                  rtrn;
	_UxCdir_dialog          *UxContext;

	UxDir_dialogContext = UxContext =
		(_UxCdir_dialog *) XtMalloc( sizeof(_UxCdir_dialog) );

	rtrn = _Uxbuild_dir_dialog(PuntGraf);

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_dir_dialog(PuntGraf)
S_XLGRAFICO *PuntGraf;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_dir_dialog(PuntGraf);

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

