
/*******************************************************************************
	SimulationTitle.c
	(Generated from interface file SimulationTitle.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/TextF.h>
#include <Xm/Separator.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo SimulationTitle.i
   tipo 
   release 1.7
   data 3/28/95
   reserved @(#)SimulationTitle.i	1.7
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif

#ifndef DESIGN_TIME
#include <grsf22.h>
extern S_HEAD1 header1;
#endif

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
	Widget	UxSimulationTitle;
	Widget	Uxform2;
	Widget	Uxseparator8;
	Widget	UxtextTitolo;
	Widget	Uxlabel14;
	Widget	UxpushButton15;
	Widget	UxpushButton16;
} _UxCSimulationTitle;

#define SimulationTitle         UxSimulationTitleContext->UxSimulationTitle
#define form2                   UxSimulationTitleContext->Uxform2
#define separator8              UxSimulationTitleContext->Uxseparator8
#define textTitolo              UxSimulationTitleContext->UxtextTitolo
#define label14                 UxSimulationTitleContext->Uxlabel14
#define pushButton15            UxSimulationTitleContext->UxpushButton15
#define pushButton16            UxSimulationTitleContext->UxpushButton16

static _UxCSimulationTitle	*UxSimulationTitleContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_SimulationTitle();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_pushButton15( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSimulationTitle     *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSimulationTitleContext;
	UxSimulationTitleContext = UxContext =
			(_UxCSimulationTitle *) UxGetContext( UxWidget );
	{
	char *Valore;
	
	Valore = XmTextFieldGetString (textTitolo);
#ifndef DESIGN_TIME
	strcpy (header1.titolo,Valore);
#endif
	CambiaTitolo();
	XtFree (Valore);
	XtDestroyWidget (SimulationTitle);
	}
	UxSimulationTitleContext = UxSaveCtx;
}

static void	activateCB_pushButton16( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSimulationTitle     *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSimulationTitleContext;
	UxSimulationTitleContext = UxContext =
			(_UxCSimulationTitle *) UxGetContext( UxWidget );
	{
	XtDestroyWidget (SimulationTitle);
	}
	UxSimulationTitleContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_SimulationTitle()
{

	SimulationTitle = XtVaCreatePopupShell( "SimulationTitle",
			topLevelShellWidgetClass, UxTopLevel,
			XmNtitle, "Simulation Title",
			XmNiconName, "Simulation title",
			XmNallowShellResize, TRUE,
			XmNheight, 180,
			XmNwidth, 480,
			XmNy, 250,
			XmNx, 400,
			NULL );

	UxPutContext( SimulationTitle, (char *) UxSimulationTitleContext );

	form2 = XtVaCreateManagedWidget( "form2",
			xmFormWidgetClass, SimulationTitle,
			XmNheight, 180,
			XmNwidth, 480,
			XmNy, 20,
			XmNx, 70,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form2, (char *) UxSimulationTitleContext );

	separator8 = XtVaCreateManagedWidget( "separator8",
			xmSeparatorWidgetClass, form2,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 20,
			XmNwidth, 460,
			XmNy, 100,
			XmNx, 10,
			NULL );

	UxPutContext( separator8, (char *) UxSimulationTitleContext );

	textTitolo = XtVaCreateManagedWidget( "textTitolo",
			xmTextFieldWidgetClass, form2,
			XmNmaxLength, 64,
			XmNbottomWidget, separator8,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 40,
			XmNwidth, 460,
			XmNy, 60,
			XmNx, 10,
			NULL );

	UxPutContext( textTitolo, (char *) UxSimulationTitleContext );

	label14 = XtVaCreateManagedWidget( "label14",
			xmLabelWidgetClass, form2,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNbottomWidget, textTitolo,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "" ),
			XmNheight, 50,
			XmNwidth, 440,
			XmNy, 10,
			XmNx, 20,
			NULL );

	UxPutContext( label14, (char *) UxSimulationTitleContext );

	pushButton15 = XtVaCreateManagedWidget( "pushButton15",
			xmPushButtonWidgetClass, form2,
			XmNtopWidget, separator8,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "OK" ),
			XmNheight, 30,
			XmNwidth, 70,
			XmNy, 120,
			XmNx, 20,
			NULL );

	UxPutContext( pushButton15, (char *) UxSimulationTitleContext );

	pushButton16 = XtVaCreateManagedWidget( "pushButton16",
			xmPushButtonWidgetClass, form2,
			XmNtopWidget, separator8,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, pushButton15,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "CANCEL" ),
			XmNheight, 30,
			XmNwidth, 70,
			XmNy, 130,
			XmNx, 100,
			NULL );

	UxPutContext( pushButton16, (char *) UxSimulationTitleContext );

	XtAddCallback( SimulationTitle, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxSimulationTitleContext );

	XtAddCallback( pushButton15, XmNactivateCallback,
			activateCB_pushButton15,
			(XtPointer) UxSimulationTitleContext );

	XtAddCallback( pushButton16, XmNactivateCallback,
			activateCB_pushButton16,
			(XtPointer) UxSimulationTitleContext );



	return ( SimulationTitle );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_SimulationTitle()
{
	Widget                  rtrn;
	_UxCSimulationTitle     *UxContext;

	UxSimulationTitleContext = UxContext =
		(_UxCSimulationTitle *) XtMalloc( sizeof(_UxCSimulationTitle) );

	{
		XmString Stringa;
		char *Valore;
		rtrn = _Uxbuild_SimulationTitle();

#ifndef DESIGN_TIME
		Valore = XtNewString (header1.titolo);
#endif
		XmTextFieldSetString (textTitolo, Valore);
		Stringa = XmStringCreate (Valore, XmSTRING_DEFAULT_CHARSET);
		set_something_val (label14, XmNlabelString, (XtArgVal) Stringa);
		XtFree (Valore);
		XmStringFree (Stringa); 
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_SimulationTitle()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_SimulationTitle();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

