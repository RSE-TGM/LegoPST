
/*******************************************************************************
	Resampling.c
	(Generated from interface file Resampling.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/Form.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo Resampling.i
   tipo 
   release 1.10
   data 3/28/95
   reserved @(#)Resampling.i	1.10
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif


#ifndef DESIGN_TIME
#include <stdlib.h>
#endif

extern float t_iniziale;
extern float t_finale;
extern float New_delta;
extern swidget Message;
extern int numero_campioni;

char *New_Delta_stringa;

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
	Widget	Uxlabel7;
	Widget	Uxtext_New_Delta;
	Widget	UxpushButton9;
	Widget	UxpushButton10;
} _UxCResampling;

#define label7                  UxResamplingContext->Uxlabel7
#define text_New_Delta          UxResamplingContext->Uxtext_New_Delta
#define pushButton9             UxResamplingContext->UxpushButton9
#define pushButton10            UxResamplingContext->UxpushButton10

static _UxCResampling	*UxResamplingContext;

Widget	Resampling;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_Resampling();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_pushButton9( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCResampling          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxResamplingContext;
	UxResamplingContext = UxContext =
			(_UxCResampling *) UxGetContext( UxWidget );
	{
	char stringa[100];
	XmString Messaggio;
	
	New_Delta_stringa = XmTextFieldGetString (text_New_Delta);
	New_delta = atof (New_Delta_stringa);
	 
	if ((New_delta <= 0) || (New_delta >= numero_campioni/2))
		{
		strcpy (stringa, "Input value is not valid\0");
		Messaggio = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message, no_grab);
		XmStringFree (Messaggio);
		}
	else
		{
		UxPopdownInterface (Resampling);
		F22_resampling ();
		}
	}
	UxResamplingContext = UxSaveCtx;
}

static void	activateCB_pushButton10( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCResampling          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxResamplingContext;
	UxResamplingContext = UxContext =
			(_UxCResampling *) UxGetContext( UxWidget );
	{
	UxPopdownInterface (Resampling);
	}
	UxResamplingContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_Resampling()
{
	Widget	Resampling_shell;

	Resampling_shell = XtVaCreatePopupShell( "Resampling_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 410,
			XmNy, 374,
			XmNwidth, 223,
			XmNheight, 136,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "Resampling",
			NULL );

	Resampling = XtVaCreateWidget( "Resampling",
			xmFormWidgetClass, Resampling_shell,
			XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
			XmNheight, 136,
			XmNwidth, 223,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( Resampling, (char *) UxResamplingContext );

	label7 = XtVaCreateManagedWidget( "label7",
			xmLabelWidgetClass, Resampling,
			XmNfontList, UxConvertFontList( "-adobe-new century schoolbook-bold-r-normal--14-140-75-75-p-87-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "SAMPLING INTERVAL" ),
			XmNheight, 45,
			XmNwidth, 215,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( label7, (char *) UxResamplingContext );

	text_New_Delta = XtVaCreateManagedWidget( "text_New_Delta",
			xmTextFieldWidgetClass, Resampling,
			XmNfontList, UxConvertFontList( "-adobe-new century schoolbook-medium-r-normal--12-120-75-75-p-70-iso8859-1" ),
			XmNheight, 30,
			XmNwidth, 120,
			XmNy, 55,
			XmNx, 50,
			NULL );

	UxPutContext( text_New_Delta, (char *) UxResamplingContext );

	pushButton9 = XtVaCreateManagedWidget( "pushButton9",
			xmPushButtonWidgetClass, Resampling,
			RES_CONVERT( XmNlabelString, "OK" ),
			XmNheight, 35,
			XmNwidth, 80,
			XmNy, 95,
			XmNx, 10,
			NULL );

	UxPutContext( pushButton9, (char *) UxResamplingContext );

	pushButton10 = XtVaCreateManagedWidget( "pushButton10",
			xmPushButtonWidgetClass, Resampling,
			RES_CONVERT( XmNlabelString, "CANCEL" ),
			XmNheight, 35,
			XmNwidth, 80,
			XmNy, 95,
			XmNx, 130,
			NULL );

	UxPutContext( pushButton10, (char *) UxResamplingContext );

	XtAddCallback( Resampling, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxResamplingContext );

	XtAddCallback( pushButton9, XmNactivateCallback,
			activateCB_pushButton9,
			(XtPointer) UxResamplingContext );

	XtAddCallback( pushButton10, XmNactivateCallback,
			activateCB_pushButton10,
			(XtPointer) UxResamplingContext );



	return ( Resampling );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_Resampling()
{
	Widget                  rtrn;
	_UxCResampling          *UxContext;

	UxResamplingContext = UxContext =
		(_UxCResampling *) XtMalloc( sizeof(_UxCResampling) );

	rtrn = _Uxbuild_Resampling();

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_Resampling()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_Resampling();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

