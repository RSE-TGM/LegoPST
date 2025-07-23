
/*******************************************************************************
	CalcWindow.c
	(Generated from interface file CalcWindow.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/Separator.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/Form.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo CalcWindow.i
   tipo 
   release 1.10
   data 3/28/95
   reserved @(#)CalcWindow.i	1.10
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
#include <Xm/Xm.h>
#include "menu_graf.h"

extern int TypeCalc;
extern char **simboli;

float paraA,paraB;
char *stringaA,*stringaB,*stringaVar,*stringaDescr;

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
	Widget	Uxlabel8;
	Widget	UxlabOpType;
	Widget	Uxlabel9;
	Widget	UxlabSourceVar1;
	Widget	UxlabSourceVar2;
	Widget	Uxlabel10;
	Widget	Uxlabel11;
	Widget	Uxlabel12;
	Widget	UxtextFieldDescr;
	Widget	UxtextFieldVar;
	Widget	Uxseparator4;
	Widget	Uxseparator5;
	Widget	Uxseparator6;
	Widget	UxpushButton13;
	Widget	UxpushButton14;
	Widget	UxlabParameter;
	Widget	UxlabParamA;
	Widget	UxtextFieldParA;
	Widget	UxlabParamB;
	Widget	UxtextFieldParB;
	Widget	Uxseparator7;
	int	Uxind;
	int	Uxind1;
	int	Uxind2;
	int	*UxNumero;
} _UxCCalcWindow;

#define label8                  UxCalcWindowContext->Uxlabel8
#define labOpType               UxCalcWindowContext->UxlabOpType
#define label9                  UxCalcWindowContext->Uxlabel9
#define labSourceVar1           UxCalcWindowContext->UxlabSourceVar1
#define labSourceVar2           UxCalcWindowContext->UxlabSourceVar2
#define label10                 UxCalcWindowContext->Uxlabel10
#define label11                 UxCalcWindowContext->Uxlabel11
#define label12                 UxCalcWindowContext->Uxlabel12
#define textFieldDescr          UxCalcWindowContext->UxtextFieldDescr
#define textFieldVar            UxCalcWindowContext->UxtextFieldVar
#define separator4              UxCalcWindowContext->Uxseparator4
#define separator5              UxCalcWindowContext->Uxseparator5
#define separator6              UxCalcWindowContext->Uxseparator6
#define pushButton13            UxCalcWindowContext->UxpushButton13
#define pushButton14            UxCalcWindowContext->UxpushButton14
#define labParameter            UxCalcWindowContext->UxlabParameter
#define labParamA               UxCalcWindowContext->UxlabParamA
#define textFieldParA           UxCalcWindowContext->UxtextFieldParA
#define labParamB               UxCalcWindowContext->UxlabParamB
#define textFieldParB           UxCalcWindowContext->UxtextFieldParB
#define separator7              UxCalcWindowContext->Uxseparator7
#define ind                     UxCalcWindowContext->Uxind
#define ind1                    UxCalcWindowContext->Uxind1
#define ind2                    UxCalcWindowContext->Uxind2
#define Numero                  UxCalcWindowContext->UxNumero

static _UxCCalcWindow	*UxCalcWindowContext;

Widget	CalcWindow;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_CalcWindow();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_pushButton13( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCCalcWindow          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxCalcWindowContext;
	UxCalcWindowContext = UxContext =
			(_UxCCalcWindow *) UxGetContext( UxWidget );
	{
	extern int EseguiElabLineare();
	extern int EseguiSommaAlg(); 
	
	if (TypeCalc == ELAB_LINEARE)
		{
		stringaA = XmTextFieldGetString (textFieldParA);
		stringaB = XmTextFieldGetString (textFieldParB);
		stringaVar = XmTextFieldGetString (textFieldVar);
		stringaDescr = XmTextFieldGetString (textFieldDescr);
	
		paraA = atof (stringaA);
		paraB = atof (stringaB);
	
		printf ("Prima della chiamata:%d\n",ind);
	
		EseguiElabLineare (ind,stringaVar,stringaDescr,paraA,paraB);
	
		XtFree (stringaA);
		XtFree (stringaB);
		XtFree (stringaVar);
		XtFree (stringaDescr);
		}
	
	if (TypeCalc == SOMMA_ALGEBRICA)
		{
		stringaVar = XmTextFieldGetString (textFieldVar);
		stringaDescr = XmTextFieldGetString (textFieldDescr);
		printf ("Prima della chiamata:%d\n",ind1);
		printf ("Prima della chiamata:%d\n",ind2); 
	
		EseguiSommaAlg (ind1,ind2,stringaVar,stringaDescr);
	
		XtFree (stringaVar);
		XtFree (stringaDescr);
		}
	}
	UxCalcWindowContext = UxSaveCtx;
}

static void	activateCB_pushButton14( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCCalcWindow          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxCalcWindowContext;
	UxCalcWindowContext = UxContext =
			(_UxCCalcWindow *) UxGetContext( UxWidget );
	UxPopdownInterface (CalcWindow);
	UxCalcWindowContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_CalcWindow()
{
	Widget	CalcWindow_shell;

	CalcWindow_shell = XtVaCreatePopupShell( "CalcWindow_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 387,
			XmNy, 13,
			XmNwidth, 516,
			XmNheight, 291,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "CalcWindow",
			XmNiconName, "CalcWindow",
			NULL );

	CalcWindow = XtVaCreateManagedWidget( "CalcWindow",
			xmFormWidgetClass, CalcWindow_shell,
			XmNheight, 291,
			XmNwidth, 516,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( CalcWindow, (char *) UxCalcWindowContext );

	label8 = XtVaCreateManagedWidget( "label8",
			xmLabelWidgetClass, CalcWindow,
			XmNalignment, XmALIGNMENT_END,
			RES_CONVERT( XmNlabelString, "Operation:" ),
			XmNheight, 25,
			XmNwidth, 85,
			XmNy, 5,
			XmNx, 10,
			NULL );

	UxPutContext( label8, (char *) UxCalcWindowContext );

	labOpType = XtVaCreateManagedWidget( "labOpType",
			xmLabelWidgetClass, CalcWindow,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			XmNleftWidget, label8,
			XmNleftOffset, 15,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 30,
			XmNrightAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 25,
			XmNwidth, 414,
			XmNy, 5,
			XmNx, 95,
			NULL );

	UxPutContext( labOpType, (char *) UxCalcWindowContext );

	label9 = XtVaCreateManagedWidget( "label9",
			xmLabelWidgetClass, CalcWindow,
			XmNtopOffset, 55,
			RES_CONVERT( XmNlabelString, "Source Variables:" ),
			XmNheight, 20,
			XmNwidth, 120,
			XmNy, 50,
			XmNx, 15,
			NULL );

	UxPutContext( label9, (char *) UxCalcWindowContext );

	labSourceVar1 = XtVaCreateManagedWidget( "labSourceVar1",
			xmLabelWidgetClass, CalcWindow,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopOffset, 75,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--10-100-75-75-p-60-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "" ),
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNheight, 25,
			XmNwidth, 496,
			XmNy, 75,
			XmNx, 10,
			NULL );

	UxPutContext( labSourceVar1, (char *) UxCalcWindowContext );

	labSourceVar2 = XtVaCreateManagedWidget( "labSourceVar2",
			xmLabelWidgetClass, CalcWindow,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopOffset, 100,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--10-100-75-75-p-60-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "" ),
			XmNheight, 25,
			XmNwidth, 496,
			XmNy, 100,
			XmNx, 10,
			NULL );

	UxPutContext( labSourceVar2, (char *) UxCalcWindowContext );

	label10 = XtVaCreateManagedWidget( "label10",
			xmLabelWidgetClass, CalcWindow,
			XmNtopOffset, 145,
			RES_CONVERT( XmNlabelString, "Target Variable:" ),
			XmNheight, 20,
			XmNwidth, 115,
			XmNy, 140,
			XmNx, 10,
			NULL );

	UxPutContext( label10, (char *) UxCalcWindowContext );

	label11 = XtVaCreateManagedWidget( "label11",
			xmLabelWidgetClass, CalcWindow,
			XmNtopOffset, 165,
			RES_CONVERT( XmNlabelString, "name:" ),
			XmNalignment, XmALIGNMENT_END,
			XmNheight, 25,
			XmNwidth, 85,
			XmNy, 160,
			XmNx, 15,
			NULL );

	UxPutContext( label11, (char *) UxCalcWindowContext );

	label12 = XtVaCreateManagedWidget( "label12",
			xmLabelWidgetClass, CalcWindow,
			XmNtopOffset, 195,
			RES_CONVERT( XmNlabelString, "description:" ),
			XmNalignment, XmALIGNMENT_END,
			XmNheight, 25,
			XmNwidth, 85,
			XmNy, 195,
			XmNx, 15,
			NULL );

	UxPutContext( label12, (char *) UxCalcWindowContext );

	textFieldDescr = XtVaCreateManagedWidget( "textFieldDescr",
			xmTextFieldWidgetClass, CalcWindow,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--10-100-75-75-p-60-iso8859-1" ),
			XmNmarginHeight, 2,
			XmNtopWidget, NULL,
			XmNtopOffset, 195,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftWidget, label12,
			XmNleftOffset, 2,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 30,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomOffset, 35,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNcolumns, 80,
			XmNmaxLength, 72,
			XmNheight, 25,
			XmNwidth, 375,
			XmNy, 180,
			XmNx, 100,
			NULL );

	UxPutContext( textFieldDescr, (char *) UxCalcWindowContext );

	textFieldVar = XtVaCreateManagedWidget( "textFieldVar",
			xmTextFieldWidgetClass, CalcWindow,
			XmNmarginHeight, 2,
			XmNvalue, "",
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--10-100-75-75-p-60-iso8859-1" ),
			XmNtopOffset, 165,
			XmNleftWidget, label11,
			XmNleftOffset, 2,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNcolumns, 15,
			XmNmaxLength, 8,
			XmNheight, 25,
			XmNwidth, 110,
			XmNy, 160,
			XmNx, 100,
			NULL );

	UxPutContext( textFieldVar, (char *) UxCalcWindowContext );

	separator4 = XtVaCreateManagedWidget( "separator4",
			xmSeparatorWidgetClass, CalcWindow,
			XmNtopWidget, labSourceVar2,
			XmNtopOffset, 2,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomWidget, label10,
			XmNbottomOffset, 2,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 10,
			XmNwidth, 505,
			XmNy, 115,
			XmNx, 5,
			NULL );

	UxPutContext( separator4, (char *) UxCalcWindowContext );

	separator5 = XtVaCreateManagedWidget( "separator5",
			xmSeparatorWidgetClass, CalcWindow,
			XmNtopWidget, textFieldDescr,
			XmNtopOffset, 2,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNheight, 10,
			XmNwidth, 505,
			XmNy, 215,
			XmNx, 5,
			NULL );

	UxPutContext( separator5, (char *) UxCalcWindowContext );

	separator6 = XtVaCreateManagedWidget( "separator6",
			xmSeparatorWidgetClass, CalcWindow,
			XmNbottomWidget, label9,
			XmNbottomOffset, 2,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopWidget, labOpType,
			XmNtopOffset, 2,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNheight, 10,
			XmNwidth, 505,
			XmNy, 30,
			XmNx, 0,
			NULL );

	UxPutContext( separator6, (char *) UxCalcWindowContext );

	pushButton13 = XtVaCreateManagedWidget( "pushButton13",
			xmPushButtonWidgetClass, CalcWindow,
			XmNbottomOffset, 15,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopWidget, separator5,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "OK" ),
			XmNheight, 35,
			XmNwidth, 90,
			XmNy, 240,
			XmNx, 15,
			NULL );

	UxPutContext( pushButton13, (char *) UxCalcWindowContext );

	pushButton14 = XtVaCreateManagedWidget( "pushButton14",
			xmPushButtonWidgetClass, CalcWindow,
			XmNtopWidget, separator5,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, pushButton13,
			XmNleftOffset, 15,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 15,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "CANCEL" ),
			XmNheight, 35,
			XmNwidth, 90,
			XmNy, 240,
			XmNx, 120,
			NULL );

	UxPutContext( pushButton14, (char *) UxCalcWindowContext );

	labParameter = XtVaCreateManagedWidget( "labParameter",
			xmLabelWidgetClass, CalcWindow,
			XmNtopWidget, separator4,
			XmNtopOffset, 2,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "" ),
			XmNheight, 20,
			XmNwidth, 115,
			XmNy, 140,
			XmNx, 240,
			NULL );

	UxPutContext( labParameter, (char *) UxCalcWindowContext );

	labParamA = XtVaCreateManagedWidget( "labParamA",
			xmLabelWidgetClass, CalcWindow,
			XmNalignment, XmALIGNMENT_END,
			XmNbottomWidget, textFieldDescr,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNtopWidget, labParameter,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNheight, 20,
			XmNwidth, 40,
			XmNy, 160,
			XmNx, 245,
			NULL );

	UxPutContext( labParamA, (char *) UxCalcWindowContext );

	textFieldParA = XtVaCreateManagedWidget( "textFieldParA",
			xmTextFieldWidgetClass, CalcWindow,
			XmNleftWidget, labParamA,
			XmNleftOffset, 2,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmaxLength, 7,
			XmNmarginHeight, 2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--10-100-75-75-p-60-iso8859-1" ),
			XmNheight, 25,
			XmNwidth, 65,
			XmNy, 160,
			XmNx, 285,
			NULL );

	UxPutContext( textFieldParA, (char *) UxCalcWindowContext );

	labParamB = XtVaCreateManagedWidget( "labParamB",
			xmLabelWidgetClass, CalcWindow,
			XmNalignment, XmALIGNMENT_END,
			XmNleftWidget, NULL,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, labParameter,
			XmNtopOffset, 0,
			RES_CONVERT( XmNlabelString, "" ),
			XmNheight, 25,
			XmNwidth, 40,
			XmNy, 160,
			XmNx, 377,
			NULL );

	UxPutContext( labParamB, (char *) UxCalcWindowContext );

	textFieldParB = XtVaCreateManagedWidget( "textFieldParB",
			xmTextFieldWidgetClass, CalcWindow,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--10-100-75-75-p-60-iso8859-1" ),
			XmNleftWidget, labParamB,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 2,
			XmNrightOffset, 30,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopWidget, labParameter,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNmaxLength, 7,
			XmNmarginHeight, 2,
			XmNheight, 25,
			XmNwidth, 65,
			XmNy, 160,
			XmNx, 420,
			NULL );

	UxPutContext( textFieldParB, (char *) UxCalcWindowContext );

	separator7 = XtVaCreateManagedWidget( "separator7",
			xmSeparatorWidgetClass, CalcWindow,
			XmNtopWidget, separator4,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightWidget, labParamA,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNleftWidget, textFieldVar,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, textFieldDescr,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNorientation, XmVERTICAL,
			XmNheight, 53,
			XmNwidth, 13,
			XmNy, 140,
			XmNx, 240,
			NULL );

	UxPutContext( separator7, (char *) UxCalcWindowContext );

	XtAddCallback( CalcWindow, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxCalcWindowContext );

	XtAddCallback( pushButton13, XmNactivateCallback,
			activateCB_pushButton13,
			(XtPointer) UxCalcWindowContext );

	XtAddCallback( pushButton14, XmNactivateCallback,
			activateCB_pushButton14,
			(XtPointer) UxCalcWindowContext );



	return ( CalcWindow );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_CalcWindow( _UxNumero )
	int	*_UxNumero;
{
	Widget                  rtrn;
	_UxCCalcWindow          *UxContext;

	UxCalcWindowContext = UxContext =
		(_UxCCalcWindow *) XtMalloc( sizeof(_UxCCalcWindow) );

	Numero = _UxNumero;
	{
		XmString parametro;
		int indVar;
		char strPara[100];
		
		printf ("Indice in entrata:%d\n",Numero[0]);
		
		if (TypeCalc == ELAB_LINEARE)
			{
			ind = Numero[0]-1;
			}
		if (TypeCalc == SOMMA_ALGEBRICA)
			{
			ind1 = Numero[0]-1;
			ind2 = Numero[1]-1;
			}
		rtrn = _Uxbuild_CalcWindow();

		if (TypeCalc == ELAB_LINEARE)
			{
			set_something_val (textFieldParA, XmNsensitive, (XtArgVal) True);
			set_something_val (textFieldParB, XmNsensitive, (XtArgVal) True);
			indVar = Numero[0] - 1;
			parametro = XmStringCreate (simboli[indVar], XmSTRING_DEFAULT_CHARSET);
			set_something_val (labSourceVar1, XmNlabelString, (XtArgVal) parametro);
			strcpy (strPara,"Ax + B\0");
			parametro = XmStringCreate (strPara, XmSTRING_DEFAULT_CHARSET);
			set_something_val (labOpType, XmNlabelString, (XtArgVal) parametro);
			strcpy (strPara,"                \0");
			parametro = XmStringCreate (strPara, XmSTRING_DEFAULT_CHARSET);
			set_something_val (labSourceVar2, XmNlabelString, (XtArgVal) parametro);
			strcpy (strPara,"Parameter:\0");
			parametro = XmStringCreate (strPara, XmSTRING_DEFAULT_CHARSET);
			set_something_val (labParameter, XmNlabelString, (XtArgVal) parametro);
			strcpy (strPara,"A:\0");
			parametro = XmStringCreate (strPara, XmSTRING_DEFAULT_CHARSET);
			set_something_val (labParamA, XmNlabelString, (XtArgVal) parametro);
			strcpy (strPara,"B:\0");
			parametro = XmStringCreate (strPara, XmSTRING_DEFAULT_CHARSET);
			set_something_val (labParamB, XmNlabelString, (XtArgVal) parametro);
			}
		if (TypeCalc == SOMMA_ALGEBRICA)
			{
			set_something_val (textFieldParA, XmNsensitive, (XtArgVal) False);
			set_something_val (textFieldParB, XmNsensitive, (XtArgVal) False);
			strcpy (strPara,"x + y\0");
			parametro = XmStringCreate (strPara, XmSTRING_DEFAULT_CHARSET);
			set_something_val (labOpType, XmNlabelString, (XtArgVal) parametro);
			indVar = Numero[0] - 1;
			parametro = XmStringCreate (simboli[indVar], XmSTRING_DEFAULT_CHARSET);
			set_something_val (labSourceVar1, XmNlabelString, (XtArgVal) parametro);
			indVar = Numero[1] - 1;
			parametro = XmStringCreate (simboli[indVar], XmSTRING_DEFAULT_CHARSET);
			set_something_val (labSourceVar2, XmNlabelString, (XtArgVal) parametro);
			strcpy (strPara,"          \0");
			parametro = XmStringCreate (strPara, XmSTRING_DEFAULT_CHARSET);
			set_something_val (labParameter, XmNlabelString, (XtArgVal) parametro);
			set_something_val (labParamA, XmNlabelString, (XtArgVal) parametro);
			set_something_val (labParamB, XmNlabelString, (XtArgVal) parametro);	
			}
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_CalcWindow( _UxNumero )
	int	*_UxNumero;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_CalcWindow( _UxNumero );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

