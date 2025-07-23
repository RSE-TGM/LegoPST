
/*******************************************************************************
	FilterWindow.c
	(Generated from interface file FilterWindow.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Separator.h>
#include <Xm/ToggleB.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo FilterWindow.i
   tipo 
   release 1.4
   data 3/28/95
   reserved @(#)FilterWindow.i	1.4
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif

#include "menu_graf.h"
#ifndef DESIGN_TIME
#include <math.h>
#include <grsf22.h>
extern char nomi_var[NUM_VAR+1][LUN_NOME+1];
#endif
extern char **simboli;
extern swidget Message;

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
	Widget	UxFilterWindow;
	Widget	Uxform3;
	Widget	Uxlabel15;
	Widget	UxtoggleButton1;
	Widget	UxtoggleButton2;
	Widget	Uxseparator9;
	Widget	Uxlabel16;
	Widget	UxtextFCut;
	Widget	Uxseparator10;
	Widget	UxpushButton17;
	Widget	UxpushButton18;
	Widget	Uxseparator11;
	Widget	Uxlabel17;
	Widget	UxlabSourceVarF;
	Widget	Uxseparator12;
	Widget	Uxlabel19;
	Widget	Uxlabel20;
	Widget	Uxlabel18;
	Widget	UxtextFieldVarF;
	Widget	UxtextFieldDescrF;
	int	UxIndex;
	Position	Uxx;
	Position	Uxy;
	int	UxInd;
} _UxCFilterWindow;

#define FilterWindow            UxFilterWindowContext->UxFilterWindow
#define form3                   UxFilterWindowContext->Uxform3
#define label15                 UxFilterWindowContext->Uxlabel15
#define toggleButton1           UxFilterWindowContext->UxtoggleButton1
#define toggleButton2           UxFilterWindowContext->UxtoggleButton2
#define separator9              UxFilterWindowContext->Uxseparator9
#define label16                 UxFilterWindowContext->Uxlabel16
#define textFCut                UxFilterWindowContext->UxtextFCut
#define separator10             UxFilterWindowContext->Uxseparator10
#define pushButton17            UxFilterWindowContext->UxpushButton17
#define pushButton18            UxFilterWindowContext->UxpushButton18
#define separator11             UxFilterWindowContext->Uxseparator11
#define label17                 UxFilterWindowContext->Uxlabel17
#define labSourceVarF           UxFilterWindowContext->UxlabSourceVarF
#define separator12             UxFilterWindowContext->Uxseparator12
#define label19                 UxFilterWindowContext->Uxlabel19
#define label20                 UxFilterWindowContext->Uxlabel20
#define label18                 UxFilterWindowContext->Uxlabel18
#define textFieldVarF           UxFilterWindowContext->UxtextFieldVarF
#define textFieldDescrF         UxFilterWindowContext->UxtextFieldDescrF
#define Index                   UxFilterWindowContext->UxIndex
#define x                       UxFilterWindowContext->Uxx
#define y                       UxFilterWindowContext->Uxy
#define Ind                     UxFilterWindowContext->UxInd

static _UxCFilterWindow	*UxFilterWindowContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_FilterWindow();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/************************************************
          Controlla se la stringa in input e' un float
************************************************/

Boolean NumFloVerifica(char *Stringa)
{
int i,lunghezza;

lunghezza = strlen (Stringa);
for (i=0;i<lunghezza;i++)
        if ( (!isdigit(Stringa[i])) && (Stringa[i] != '.') ) return (False);
return (True);
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	disarmCB_toggleButton1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCFilterWindow        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFilterWindowContext;
	UxFilterWindowContext = UxContext =
			(_UxCFilterWindow *) UxGetContext( UxWidget );
	{
	XmToggleButtonSetState (toggleButton1, True, False);
	}
	UxFilterWindowContext = UxSaveCtx;
}

static void	armCB_toggleButton1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCFilterWindow        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFilterWindowContext;
	UxFilterWindowContext = UxContext =
			(_UxCFilterWindow *) UxGetContext( UxWidget );
	{
	XmToggleButtonSetState (toggleButton2, False, False);
	}
	UxFilterWindowContext = UxSaveCtx;
}

static void	disarmCB_toggleButton2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCFilterWindow        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFilterWindowContext;
	UxFilterWindowContext = UxContext =
			(_UxCFilterWindow *) UxGetContext( UxWidget );
	{
	XmToggleButtonSetState (toggleButton2, True, False);
	}
	UxFilterWindowContext = UxSaveCtx;
}

static void	armCB_toggleButton2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCFilterWindow        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFilterWindowContext;
	UxFilterWindowContext = UxContext =
			(_UxCFilterWindow *) UxGetContext( UxWidget );
	{
	XmToggleButtonSetState (toggleButton1, False, False);
	}
	UxFilterWindowContext = UxSaveCtx;
}

static void	activateCB_pushButton17( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCFilterWindow        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFilterWindowContext;
	UxFilterWindowContext = UxContext =
			(_UxCFilterWindow *) UxGetContext( UxWidget );
	{
	XmString Messaggio;
	char *Valore,*Nome,*Descrizione;
	float FreqCut;
	Boolean Tog1,Tog2;
	char *ritorno;
	
	Tog1 = XmToggleButtonGetState (toggleButton1);
	Tog2 = XmToggleButtonGetState (toggleButton2);
	Valore = XmTextFieldGetString (textFCut);
	Nome = XmTextFieldGetString (textFieldVarF);
	
	ritorno = EseguiControlliInput (Tog1,Tog2,Valore,Nome);
	if (ritorno == NULL)
		{
		FreqCut = atof (Valore);
		Descrizione = XmTextFieldGetString (textFieldDescrF);
	
		if ( XmToggleButtonGetState(toggleButton1) )
			EseguiFiltro (Index,BESSEL,Nome,Descrizione,FreqCut);
		else if ( XmToggleButtonGetState (toggleButton2) )
			EseguiFiltro (Index,BUTTERWORTH,Nome,Descrizione,FreqCut);
		XtDestroyWidget (FilterWindow);
		}
	else
		{
		Messaggio = XmStringCreate (ritorno, XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
		UxPopupInterface (Message, no_grab);
		XmStringFree (Messaggio);
		XtFree (ritorno);
		}
	}
	UxFilterWindowContext = UxSaveCtx;
}

static void	activateCB_pushButton18( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCFilterWindow        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFilterWindowContext;
	UxFilterWindowContext = UxContext =
			(_UxCFilterWindow *) UxGetContext( UxWidget );
	{
	XtDestroyWidget (FilterWindow);
	}
	UxFilterWindowContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_FilterWindow()
{

	FilterWindow = XtVaCreatePopupShell( "FilterWindow",
			topLevelShellWidgetClass, UxTopLevel,
			XmNallowShellResize, TRUE,
			XmNtitle, "Filter Dialog",
			XmNiconName, "FILTER",
			XmNheight, 250,
			XmNwidth, 800,
			XmNy, 603,
			XmNx, 41,
			NULL );

	UxPutContext( FilterWindow, (char *) UxFilterWindowContext );

	form3 = XtVaCreateManagedWidget( "form3",
			xmFormWidgetClass, FilterWindow,
			XmNheight, 250,
			XmNwidth, 770,
			XmNy, 10,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form3, (char *) UxFilterWindowContext );

	label15 = XtVaCreateManagedWidget( "label15",
			xmLabelWidgetClass, form3,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Filter Type:" ),
			XmNheight, 25,
			XmNwidth, 170,
			XmNy, 5,
			XmNx, 10,
			NULL );

	UxPutContext( label15, (char *) UxFilterWindowContext );

	toggleButton1 = XtVaCreateManagedWidget( "toggleButton1",
			xmToggleButtonWidgetClass, form3,
			XmNmarginLeft, 30,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopWidget, label15,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "BESSEL" ),
			XmNindicatorSize, 20,
			XmNindicatorType, XmONE_OF_MANY,
			XmNheight, 30,
			XmNwidth, 160,
			XmNy, 30,
			XmNx, 20,
			NULL );

	UxPutContext( toggleButton1, (char *) UxFilterWindowContext );

	toggleButton2 = XtVaCreateManagedWidget( "toggleButton2",
			xmToggleButtonWidgetClass, form3,
			XmNtopWidget, toggleButton1,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNmarginLeft, 30,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "BUTTERWORTH" ),
			XmNindicatorSize, 20,
			XmNindicatorType, XmONE_OF_MANY,
			XmNheight, 30,
			XmNwidth, 160,
			XmNy, 70,
			XmNx, 10,
			NULL );

	UxPutContext( toggleButton2, (char *) UxFilterWindowContext );

	separator9 = XtVaCreateManagedWidget( "separator9",
			xmSeparatorWidgetClass, form3,
			XmNtopWidget, toggleButton2,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 15,
			XmNwidth, 175,
			XmNy, 105,
			XmNx, 5,
			NULL );

	UxPutContext( separator9, (char *) UxFilterWindowContext );

	label16 = XtVaCreateManagedWidget( "label16",
			xmLabelWidgetClass, form3,
			RES_CONVERT( XmNlabelString, "Cut Frequency:" ),
			XmNtopWidget, separator9,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 170,
			XmNy, 125,
			XmNx, 10,
			NULL );

	UxPutContext( label16, (char *) UxFilterWindowContext );

	textFCut = XtVaCreateManagedWidget( "textFCut",
			xmTextFieldWidgetClass, form3,
			XmNmaxLength, 10,
			XmNtopWidget, label16,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNheight, 30,
			XmNwidth, 120,
			XmNy, 160,
			XmNx, 30,
			NULL );

	UxPutContext( textFCut, (char *) UxFilterWindowContext );

	separator10 = XtVaCreateManagedWidget( "separator10",
			xmSeparatorWidgetClass, form3,
			XmNtopWidget, textFCut,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 15,
			XmNwidth, 230,
			XmNy, 195,
			XmNx, 50,
			NULL );

	UxPutContext( separator10, (char *) UxFilterWindowContext );

	pushButton17 = XtVaCreateManagedWidget( "pushButton17",
			xmPushButtonWidgetClass, form3,
			RES_CONVERT( XmNlabelString, "OK" ),
			XmNtopWidget, separator10,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 70,
			XmNy, 210,
			XmNx, 20,
			NULL );

	UxPutContext( pushButton17, (char *) UxFilterWindowContext );

	pushButton18 = XtVaCreateManagedWidget( "pushButton18",
			xmPushButtonWidgetClass, form3,
			XmNtopWidget, separator10,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, pushButton17,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "CANCEL" ),
			XmNheight, 30,
			XmNwidth, 70,
			XmNy, 210,
			XmNx, 90,
			NULL );

	UxPutContext( pushButton18, (char *) UxFilterWindowContext );

	separator11 = XtVaCreateManagedWidget( "separator11",
			xmSeparatorWidgetClass, form3,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 5,
			XmNleftWidget, separator9,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, separator10,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNorientation, XmVERTICAL,
			XmNheight, 53,
			XmNwidth, 13,
			XmNy, 80,
			XmNx, 186,
			NULL );

	UxPutContext( separator11, (char *) UxFilterWindowContext );

	label17 = XtVaCreateManagedWidget( "label17",
			xmLabelWidgetClass, form3,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftWidget, separator11,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "Source Variables:" ),
			XmNheight, 20,
			XmNwidth, 120,
			XmNy, 10,
			XmNx, 206,
			NULL );

	UxPutContext( label17, (char *) UxFilterWindowContext );

	labSourceVarF = XtVaCreateManagedWidget( "labSourceVarF",
			xmLabelWidgetClass, form3,
			XmNtopWidget, label17,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftWidget, separator11,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList( "fixed" ),
			RES_CONVERT( XmNlabelString, "" ),
			XmNheight, 25,
			XmNwidth, 496,
			XmNy, 40,
			XmNx, 216,
			NULL );

	UxPutContext( labSourceVarF, (char *) UxFilterWindowContext );

	separator12 = XtVaCreateManagedWidget( "separator12",
			xmSeparatorWidgetClass, form3,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftWidget, separator11,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNheight, 10,
			XmNwidth, 505,
			XmNy, 80,
			XmNx, 216,
			NULL );

	UxPutContext( separator12, (char *) UxFilterWindowContext );

	label19 = XtVaCreateManagedWidget( "label19",
			xmLabelWidgetClass, form3,
			XmNleftWidget, separator11,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "description:" ),
			XmNalignment, XmALIGNMENT_END,
			XmNheight, 25,
			XmNwidth, 85,
			XmNy, 160,
			XmNx, 219,
			NULL );

	UxPutContext( label19, (char *) UxFilterWindowContext );

	label20 = XtVaCreateManagedWidget( "label20",
			xmLabelWidgetClass, form3,
			XmNtopWidget, separator12,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, separator11,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "Target Variable:" ),
			XmNheight, 20,
			XmNwidth, 115,
			XmNy, 105,
			XmNx, 214,
			NULL );

	UxPutContext( label20, (char *) UxFilterWindowContext );

	label18 = XtVaCreateManagedWidget( "label18",
			xmLabelWidgetClass, form3,
			XmNtopWidget, label20,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, separator11,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "name:" ),
			XmNalignment, XmALIGNMENT_END,
			XmNheight, 25,
			XmNwidth, 85,
			XmNy, 125,
			XmNx, 219,
			NULL );

	UxPutContext( label18, (char *) UxFilterWindowContext );

	textFieldVarF = XtVaCreateManagedWidget( "textFieldVarF",
			xmTextFieldWidgetClass, form3,
			XmNleftWidget, label18,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginHeight, 2,
			XmNvalue, "",
			XmNfontList, UxConvertFontList( "fixed" ),
			XmNcolumns, 15,
			XmNmaxLength, 8,
			XmNheight, 25,
			XmNwidth, 110,
			XmNy, 125,
			XmNx, 306,
			NULL );

	UxPutContext( textFieldVarF, (char *) UxFilterWindowContext );

	textFieldDescrF = XtVaCreateManagedWidget( "textFieldDescrF",
			xmTextFieldWidgetClass, form3,
			XmNtopWidget, textFieldVarF,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftWidget, label19,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNfontList, UxConvertFontList( "fixed" ),
			XmNmarginHeight, 2,
			XmNcolumns, 72,
			XmNmaxLength, 72,
			XmNheight, 25,
			XmNwidth, 375,
			XmNy, 160,
			XmNx, 306,
			NULL );

	UxPutContext( textFieldDescrF, (char *) UxFilterWindowContext );

	XtAddCallback( FilterWindow, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxFilterWindowContext );

	XtAddCallback( toggleButton1, XmNdisarmCallback,
			disarmCB_toggleButton1,
			(XtPointer) UxFilterWindowContext );
	XtAddCallback( toggleButton1, XmNarmCallback,
			armCB_toggleButton1,
			(XtPointer) UxFilterWindowContext );

	XtAddCallback( toggleButton2, XmNdisarmCallback,
			disarmCB_toggleButton2,
			(XtPointer) UxFilterWindowContext );
	XtAddCallback( toggleButton2, XmNarmCallback,
			armCB_toggleButton2,
			(XtPointer) UxFilterWindowContext );

	XtAddCallback( pushButton17, XmNactivateCallback,
			activateCB_pushButton17,
			(XtPointer) UxFilterWindowContext );

	XtAddCallback( pushButton18, XmNactivateCallback,
			activateCB_pushButton18,
			(XtPointer) UxFilterWindowContext );



	return ( FilterWindow );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_FilterWindow( _Uxx, _Uxy, _UxInd )
	Position	_Uxx;
	Position	_Uxy;
	int	_UxInd;
{
	Widget                  rtrn;
	_UxCFilterWindow        *UxContext;

	UxFilterWindowContext = UxContext =
		(_UxCFilterWindow *) XtMalloc( sizeof(_UxCFilterWindow) );

	x = _Uxx;
	y = _Uxy;
	Ind = _UxInd;
	{
		XmString Stringa;
		char descriz[72];
		int i,j;
		
		Index = Ind;
		rtrn = _Uxbuild_FilterWindow();

		Stringa = XmStringCreate (simboli[Index], XmSTRING_DEFAULT_CHARSET);
		set_something_val (labSourceVarF, XmNlabelString, (XtArgVal) Stringa);
		
#ifndef DESIGN_TIME
		XmTextFieldSetString (textFieldVarF, nomi_var[Index]);
		
		j=0;
		for (i=8;i<LUN_SIMB+1;i++)
			{
			descriz [j] = simboli[Index][i];
			j++;
			}
		XmTextFieldSetString (textFieldDescrF,descriz);
#endif
		
		set_something_val (FilterWindow, XmNx, (XtArgVal) x+10);
		set_something_val (FilterWindow, XmNy, (XtArgVal) y+10);
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_FilterWindow( _Uxx, _Uxy, _UxInd )
	Position	_Uxx;
	Position	_Uxy;
	int	_UxInd;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_FilterWindow( _Uxx, _Uxy, _UxInd );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

