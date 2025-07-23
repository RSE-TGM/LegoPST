
/*******************************************************************************
	Sampling.c
	(Generated from interface file Sampling.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/ArrowBG.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/Form.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo Sampling.i
   tipo 
   release 1.10
   data 3/28/95
   reserved @(#)Sampling.i	1.10
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif


extern swidget Message;
extern int New_Sampl;
extern int numero_campioni;

char *New_Sampl_stringa;

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
	Widget	Uxlabel6;
	Widget	Uxtext_New_Sampl;
	Widget	UxarrowButtonGadget1;
	Widget	UxarrowButtonGadget2;
	Widget	UxpushButton7;
	Widget	UxpushButton8;
} _UxCSampling;

#define label6                  UxSamplingContext->Uxlabel6
#define text_New_Sampl          UxSamplingContext->Uxtext_New_Sampl
#define arrowButtonGadget1      UxSamplingContext->UxarrowButtonGadget1
#define arrowButtonGadget2      UxSamplingContext->UxarrowButtonGadget2
#define pushButton7             UxSamplingContext->UxpushButton7
#define pushButton8             UxSamplingContext->UxpushButton8

static _UxCSampling	*UxSamplingContext;

Widget	Sampling;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_Sampling();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_arrowButtonGadget1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSampling            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSamplingContext;
	UxSamplingContext = UxContext =
			(_UxCSampling *) UxGetContext( UxWidget );
	{
	get_something (text_New_Sampl, XmNvalue, (void*) &New_Sampl_stringa);
	New_Sampl = atoi (New_Sampl_stringa);
	if (New_Sampl > 1) 
	{
		New_Sampl--;
		sprintf (New_Sampl_stringa,"%d",New_Sampl);
	}
	set_something_val (text_New_Sampl, XmNvalue, (XtArgVal) New_Sampl_stringa);
	}
	UxSamplingContext = UxSaveCtx;
}

static void	activateCB_arrowButtonGadget2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSampling            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSamplingContext;
	UxSamplingContext = UxContext =
			(_UxCSampling *) UxGetContext( UxWidget );
	{
	get_something (text_New_Sampl, XmNvalue, (void*) &New_Sampl_stringa);
	New_Sampl = atoi (New_Sampl_stringa);
	if (New_Sampl < (numero_campioni/4))
	{
		New_Sampl ++;
		sprintf (New_Sampl_stringa,"%d",New_Sampl);
	}
	set_something_val (text_New_Sampl, XmNvalue, (XtArgVal) New_Sampl_stringa);
	}
	UxSamplingContext = UxSaveCtx;
}

static void	activateCB_pushButton7( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSampling            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSamplingContext;
	UxSamplingContext = UxContext =
			(_UxCSampling *) UxGetContext( UxWidget );
	{
	char stringa[100];
	XmString Messaggio;
	
	New_Sampl_stringa = XmTextFieldGetString (text_New_Sampl);
	New_Sampl = atoi (New_Sampl_stringa);
	if ((New_Sampl == 0) || (New_Sampl > (numero_campioni/4)) || (New_Sampl <= 0))
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
		UxPopdownInterface (Sampling);
		F22_sampling ();
		}
	}
	UxSamplingContext = UxSaveCtx;
}

static void	activateCB_pushButton8( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSampling            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSamplingContext;
	UxSamplingContext = UxContext =
			(_UxCSampling *) UxGetContext( UxWidget );
	{
	UxPopdownInterface (Sampling);
	}
	UxSamplingContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_Sampling()
{
	Widget	Sampling_shell;

	Sampling_shell = XtVaCreatePopupShell( "Sampling_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 386,
			XmNy, 333,
			XmNwidth, 223,
			XmNheight, 136,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "Sampling",
			NULL );

	Sampling = XtVaCreateWidget( "Sampling",
			xmFormWidgetClass, Sampling_shell,
			XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
			XmNheight, 136,
			XmNwidth, 223,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( Sampling, (char *) UxSamplingContext );

	label6 = XtVaCreateManagedWidget( "label6",
			xmLabelWidgetClass, Sampling,
			XmNfontList, UxConvertFontList( "-adobe-new century schoolbook-bold-r-normal--14-140-75-75-p-87-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "SAMPLING NUMBER" ),
			XmNheight, 45,
			XmNwidth, 215,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( label6, (char *) UxSamplingContext );

	text_New_Sampl = XtVaCreateManagedWidget( "text_New_Sampl",
			xmTextFieldWidgetClass, Sampling,
			XmNfontList, UxConvertFontList( "-adobe-new century schoolbook-medium-r-normal--12-120-75-75-p-70-iso8859-1" ),
			XmNheight, 30,
			XmNwidth, 90,
			XmNy, 55,
			XmNx, 65,
			NULL );

	UxPutContext( text_New_Sampl, (char *) UxSamplingContext );

	arrowButtonGadget1 = XtVaCreateManagedWidget( "arrowButtonGadget1",
			xmArrowButtonGadgetClass, Sampling,
			XmNarrowDirection, XmARROW_DOWN,
			XmNheight, 30,
			XmNwidth, 30,
			XmNy, 55,
			XmNx, 155,
			NULL );

	UxPutContext( arrowButtonGadget1, (char *) UxSamplingContext );

	arrowButtonGadget2 = XtVaCreateManagedWidget( "arrowButtonGadget2",
			xmArrowButtonGadgetClass, Sampling,
			XmNheight, 30,
			XmNwidth, 30,
			XmNy, 55,
			XmNx, 35,
			NULL );

	UxPutContext( arrowButtonGadget2, (char *) UxSamplingContext );

	pushButton7 = XtVaCreateManagedWidget( "pushButton7",
			xmPushButtonWidgetClass, Sampling,
			RES_CONVERT( XmNlabelString, "OK" ),
			XmNheight, 35,
			XmNwidth, 80,
			XmNy, 95,
			XmNx, 10,
			NULL );

	UxPutContext( pushButton7, (char *) UxSamplingContext );

	pushButton8 = XtVaCreateManagedWidget( "pushButton8",
			xmPushButtonWidgetClass, Sampling,
			RES_CONVERT( XmNlabelString, "CANCEL" ),
			XmNheight, 35,
			XmNwidth, 80,
			XmNy, 95,
			XmNx, 130,
			NULL );

	UxPutContext( pushButton8, (char *) UxSamplingContext );

	XtAddCallback( Sampling, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxSamplingContext );

	XtAddCallback( arrowButtonGadget1, XmNactivateCallback,
			activateCB_arrowButtonGadget1,
			(XtPointer) UxSamplingContext );

	XtAddCallback( arrowButtonGadget2, XmNactivateCallback,
			activateCB_arrowButtonGadget2,
			(XtPointer) UxSamplingContext );

	XtAddCallback( pushButton7, XmNactivateCallback,
			activateCB_pushButton7,
			(XtPointer) UxSamplingContext );

	XtAddCallback( pushButton8, XmNactivateCallback,
			activateCB_pushButton8,
			(XtPointer) UxSamplingContext );



	return ( Sampling );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_Sampling()
{
	Widget                  rtrn;
	_UxCSampling            *UxContext;

	UxSamplingContext = UxContext =
		(_UxCSampling *) XtMalloc( sizeof(_UxCSampling) );

	rtrn = _Uxbuild_Sampling();

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_Sampling()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_Sampling();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

