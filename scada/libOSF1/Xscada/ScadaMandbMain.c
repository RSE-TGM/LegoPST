/**********************************************************************
*
*       C Source:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: %  (%full_filespec: %)";
#endif
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#ifdef XSCADA_INTERFACE
/*******************************************************************************
	ScadaMandbMain.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"
#include "MandbIconBitmap.bmp"

#include <Xm/Label.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/ScrolledW.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/



extern Widget MandbWidget;

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
	Widget	UxScadaMandbMain;
	Widget	UxformMandb;
	Widget	UxscrolledWindowMandb;
	Widget	UxformMandbMain;
	Widget	UxformMandbVal1;
	Widget	UxtextField1;
	Widget	UxpushButtonVal1;
	Widget	UxtextFieldSt1;
	Widget	UxrowColumnToggle1;
	Widget	UxtoggleButton1_1;
	Widget	UxtoggleButton2_1;
	Widget	UxpushButtonApply1;
	Widget	UxrowColumn1;
	Widget	UxtoggleButtonf01_1;
	Widget	UxtoggleButtonf02_1;
	Widget	UxtoggleButtonf03_1;
	Widget	UxtoggleButtonf04_1;
	Widget	UxtoggleButtonf05_1;
	Widget	UxtoggleButtonf06_1;
	Widget	UxtoggleButtonf07_1;
	Widget	UxtoggleButtonf08_1;
	Widget	UxtoggleButtonf09_1;
	Widget	UxtoggleButtonf10_1;
	Widget	UxtoggleButtonf11_1;
	Widget	UxtoggleButtonf12_1;
	Widget	UxtoggleButtonf13_1;
	Widget	UxtoggleButtonf14_1;
	Widget	UxtoggleButtonf15_1;
	Widget	UxtoggleButtonf16_1;
	Widget	UxlabelInfo1;
	Widget	UxformMandbVal2;
	Widget	UxtextField2;
	Widget	UxpushButtonVal2;
	Widget	UxtextFieldSt2;
	Widget	UxrowColumnToggle2;
	Widget	UxtoggleButton1_2;
	Widget	UxtoggleButton2_2;
	Widget	UxpushButtonApply2;
	Widget	UxrowColumn2;
	Widget	UxtoggleButtonf01_2;
	Widget	UxtoggleButtonf02_2;
	Widget	UxtoggleButtonf03_2;
	Widget	UxtoggleButtonf04_2;
	Widget	UxtoggleButtonf05_2;
	Widget	UxtoggleButtonf06_2;
	Widget	UxtoggleButtonf07_2;
	Widget	UxtoggleButtonf08_2;
	Widget	UxtoggleButtonf09_2;
	Widget	UxtoggleButtonf10_2;
	Widget	UxtoggleButtonf11_2;
	Widget	UxtoggleButtonf12_2;
	Widget	UxtoggleButtonf13_2;
	Widget	UxtoggleButtonf14_2;
	Widget	UxtoggleButtonf15_2;
	Widget	UxtoggleButtonf16_2;
	Widget	UxlabelInfo2;
	Widget	UxformMandbVal3;
	Widget	UxtextField3;
	Widget	UxpushButtonVal3;
	Widget	UxtextFieldSt3;
	Widget	UxrowColumnToggle3;
	Widget	UxtoggleButton1_3;
	Widget	UxtoggleButton2_3;
	Widget	UxpushButtonApply3;
	Widget	UxrowColumn3;
	Widget	UxtoggleButtonf01_3;
	Widget	UxtoggleButtonf02_3;
	Widget	UxtoggleButtonf03_3;
	Widget	UxtoggleButtonf04_3;
	Widget	UxtoggleButtonf05_3;
	Widget	UxtoggleButtonf06_3;
	Widget	UxtoggleButtonf07_3;
	Widget	UxtoggleButtonf08_3;
	Widget	UxtoggleButtonf09_3;
	Widget	UxtoggleButtonf10_3;
	Widget	UxtoggleButtonf11_3;
	Widget	UxtoggleButtonf12_3;
	Widget	UxtoggleButtonf13_3;
	Widget	UxtoggleButtonf14_3;
	Widget	UxtoggleButtonf15_3;
	Widget	UxtoggleButtonf16_3;
	Widget	UxlabelInfo3;
	Widget	UxformMandbVal4;
	Widget	UxtextField4;
	Widget	UxpushButtonVal4;
	Widget	UxtextFieldSt4;
	Widget	UxrowColumnToggle4;
	Widget	UxtoggleButton1_4;
	Widget	UxtoggleButton2_4;
	Widget	UxpushButtonApply4;
	Widget	UxrowColumn4;
	Widget	UxtoggleButtonf01_4;
	Widget	UxtoggleButtonf02_4;
	Widget	UxtoggleButtonf03_4;
	Widget	UxtoggleButtonf04_4;
	Widget	UxtoggleButtonf05_4;
	Widget	UxtoggleButtonf06_4;
	Widget	UxtoggleButtonf07_4;
	Widget	UxtoggleButtonf08_4;
	Widget	UxtoggleButtonf09_4;
	Widget	UxtoggleButtonf10_4;
	Widget	UxtoggleButtonf11_4;
	Widget	UxtoggleButtonf12_4;
	Widget	UxtoggleButtonf13_4;
	Widget	UxtoggleButtonf14_4;
	Widget	UxtoggleButtonf15_4;
	Widget	UxtoggleButtonf16_4;
	Widget	UxlabelInfo4;
	Widget	UxformMandbVal5;
	Widget	UxtextField5;
	Widget	UxpushButtonVal5;
	Widget	UxtextFieldSt5;
	Widget	UxrowColumnToggle5;
	Widget	UxtoggleButton1_5;
	Widget	UxtoggleButton2_5;
	Widget	UxpushButtonApply5;
	Widget	UxrowColumn5;
	Widget	UxtoggleButtonf01_5;
	Widget	UxtoggleButtonf02_5;
	Widget	UxtoggleButtonf03_5;
	Widget	UxtoggleButtonf04_5;
	Widget	UxtoggleButtonf05_5;
	Widget	UxtoggleButtonf06_5;
	Widget	UxtoggleButtonf07_5;
	Widget	UxtoggleButtonf08_5;
	Widget	UxtoggleButtonf09_5;
	Widget	UxtoggleButtonf10_5;
	Widget	UxtoggleButtonf11_5;
	Widget	UxtoggleButtonf12_5;
	Widget	UxtoggleButtonf13_5;
	Widget	UxtoggleButtonf14_5;
	Widget	UxtoggleButtonf15_5;
	Widget	UxtoggleButtonf16_5;
	Widget	UxlabelInfo5;
	Widget	UxpushButtonQuitMandb;
	int	Uxmode_1;
	int	Uxmode_2;
	int	Uxmode_3;
	int	Uxmode_4;
	int	Uxmode_5;
	XtIntervalId	Uxtimer_refresh;
	int	Uxtime_ref;
} _UxCScadaMandbMain;

#define ScadaMandbMain          UxScadaMandbMainContext->UxScadaMandbMain
#define formMandb               UxScadaMandbMainContext->UxformMandb
#define scrolledWindowMandb     UxScadaMandbMainContext->UxscrolledWindowMandb
#define formMandbMain           UxScadaMandbMainContext->UxformMandbMain
#define formMandbVal1           UxScadaMandbMainContext->UxformMandbVal1
#define textField1              UxScadaMandbMainContext->UxtextField1
#define pushButtonVal1          UxScadaMandbMainContext->UxpushButtonVal1
#define textFieldSt1            UxScadaMandbMainContext->UxtextFieldSt1
#define rowColumnToggle1        UxScadaMandbMainContext->UxrowColumnToggle1
#define toggleButton1_1         UxScadaMandbMainContext->UxtoggleButton1_1
#define toggleButton2_1         UxScadaMandbMainContext->UxtoggleButton2_1
#define pushButtonApply1        UxScadaMandbMainContext->UxpushButtonApply1
#define rowColumn1              UxScadaMandbMainContext->UxrowColumn1
#define toggleButtonf01_1       UxScadaMandbMainContext->UxtoggleButtonf01_1
#define toggleButtonf02_1       UxScadaMandbMainContext->UxtoggleButtonf02_1
#define toggleButtonf03_1       UxScadaMandbMainContext->UxtoggleButtonf03_1
#define toggleButtonf04_1       UxScadaMandbMainContext->UxtoggleButtonf04_1
#define toggleButtonf05_1       UxScadaMandbMainContext->UxtoggleButtonf05_1
#define toggleButtonf06_1       UxScadaMandbMainContext->UxtoggleButtonf06_1
#define toggleButtonf07_1       UxScadaMandbMainContext->UxtoggleButtonf07_1
#define toggleButtonf08_1       UxScadaMandbMainContext->UxtoggleButtonf08_1
#define toggleButtonf09_1       UxScadaMandbMainContext->UxtoggleButtonf09_1
#define toggleButtonf10_1       UxScadaMandbMainContext->UxtoggleButtonf10_1
#define toggleButtonf11_1       UxScadaMandbMainContext->UxtoggleButtonf11_1
#define toggleButtonf12_1       UxScadaMandbMainContext->UxtoggleButtonf12_1
#define toggleButtonf13_1       UxScadaMandbMainContext->UxtoggleButtonf13_1
#define toggleButtonf14_1       UxScadaMandbMainContext->UxtoggleButtonf14_1
#define toggleButtonf15_1       UxScadaMandbMainContext->UxtoggleButtonf15_1
#define toggleButtonf16_1       UxScadaMandbMainContext->UxtoggleButtonf16_1
#define labelInfo1              UxScadaMandbMainContext->UxlabelInfo1
#define formMandbVal2           UxScadaMandbMainContext->UxformMandbVal2
#define textField2              UxScadaMandbMainContext->UxtextField2
#define pushButtonVal2          UxScadaMandbMainContext->UxpushButtonVal2
#define textFieldSt2            UxScadaMandbMainContext->UxtextFieldSt2
#define rowColumnToggle2        UxScadaMandbMainContext->UxrowColumnToggle2
#define toggleButton1_2         UxScadaMandbMainContext->UxtoggleButton1_2
#define toggleButton2_2         UxScadaMandbMainContext->UxtoggleButton2_2
#define pushButtonApply2        UxScadaMandbMainContext->UxpushButtonApply2
#define rowColumn2              UxScadaMandbMainContext->UxrowColumn2
#define toggleButtonf01_2       UxScadaMandbMainContext->UxtoggleButtonf01_2
#define toggleButtonf02_2       UxScadaMandbMainContext->UxtoggleButtonf02_2
#define toggleButtonf03_2       UxScadaMandbMainContext->UxtoggleButtonf03_2
#define toggleButtonf04_2       UxScadaMandbMainContext->UxtoggleButtonf04_2
#define toggleButtonf05_2       UxScadaMandbMainContext->UxtoggleButtonf05_2
#define toggleButtonf06_2       UxScadaMandbMainContext->UxtoggleButtonf06_2
#define toggleButtonf07_2       UxScadaMandbMainContext->UxtoggleButtonf07_2
#define toggleButtonf08_2       UxScadaMandbMainContext->UxtoggleButtonf08_2
#define toggleButtonf09_2       UxScadaMandbMainContext->UxtoggleButtonf09_2
#define toggleButtonf10_2       UxScadaMandbMainContext->UxtoggleButtonf10_2
#define toggleButtonf11_2       UxScadaMandbMainContext->UxtoggleButtonf11_2
#define toggleButtonf12_2       UxScadaMandbMainContext->UxtoggleButtonf12_2
#define toggleButtonf13_2       UxScadaMandbMainContext->UxtoggleButtonf13_2
#define toggleButtonf14_2       UxScadaMandbMainContext->UxtoggleButtonf14_2
#define toggleButtonf15_2       UxScadaMandbMainContext->UxtoggleButtonf15_2
#define toggleButtonf16_2       UxScadaMandbMainContext->UxtoggleButtonf16_2
#define labelInfo2              UxScadaMandbMainContext->UxlabelInfo2
#define formMandbVal3           UxScadaMandbMainContext->UxformMandbVal3
#define textField3              UxScadaMandbMainContext->UxtextField3
#define pushButtonVal3          UxScadaMandbMainContext->UxpushButtonVal3
#define textFieldSt3            UxScadaMandbMainContext->UxtextFieldSt3
#define rowColumnToggle3        UxScadaMandbMainContext->UxrowColumnToggle3
#define toggleButton1_3         UxScadaMandbMainContext->UxtoggleButton1_3
#define toggleButton2_3         UxScadaMandbMainContext->UxtoggleButton2_3
#define pushButtonApply3        UxScadaMandbMainContext->UxpushButtonApply3
#define rowColumn3              UxScadaMandbMainContext->UxrowColumn3
#define toggleButtonf01_3       UxScadaMandbMainContext->UxtoggleButtonf01_3
#define toggleButtonf02_3       UxScadaMandbMainContext->UxtoggleButtonf02_3
#define toggleButtonf03_3       UxScadaMandbMainContext->UxtoggleButtonf03_3
#define toggleButtonf04_3       UxScadaMandbMainContext->UxtoggleButtonf04_3
#define toggleButtonf05_3       UxScadaMandbMainContext->UxtoggleButtonf05_3
#define toggleButtonf06_3       UxScadaMandbMainContext->UxtoggleButtonf06_3
#define toggleButtonf07_3       UxScadaMandbMainContext->UxtoggleButtonf07_3
#define toggleButtonf08_3       UxScadaMandbMainContext->UxtoggleButtonf08_3
#define toggleButtonf09_3       UxScadaMandbMainContext->UxtoggleButtonf09_3
#define toggleButtonf10_3       UxScadaMandbMainContext->UxtoggleButtonf10_3
#define toggleButtonf11_3       UxScadaMandbMainContext->UxtoggleButtonf11_3
#define toggleButtonf12_3       UxScadaMandbMainContext->UxtoggleButtonf12_3
#define toggleButtonf13_3       UxScadaMandbMainContext->UxtoggleButtonf13_3
#define toggleButtonf14_3       UxScadaMandbMainContext->UxtoggleButtonf14_3
#define toggleButtonf15_3       UxScadaMandbMainContext->UxtoggleButtonf15_3
#define toggleButtonf16_3       UxScadaMandbMainContext->UxtoggleButtonf16_3
#define labelInfo3              UxScadaMandbMainContext->UxlabelInfo3
#define formMandbVal4           UxScadaMandbMainContext->UxformMandbVal4
#define textField4              UxScadaMandbMainContext->UxtextField4
#define pushButtonVal4          UxScadaMandbMainContext->UxpushButtonVal4
#define textFieldSt4            UxScadaMandbMainContext->UxtextFieldSt4
#define rowColumnToggle4        UxScadaMandbMainContext->UxrowColumnToggle4
#define toggleButton1_4         UxScadaMandbMainContext->UxtoggleButton1_4
#define toggleButton2_4         UxScadaMandbMainContext->UxtoggleButton2_4
#define pushButtonApply4        UxScadaMandbMainContext->UxpushButtonApply4
#define rowColumn4              UxScadaMandbMainContext->UxrowColumn4
#define toggleButtonf01_4       UxScadaMandbMainContext->UxtoggleButtonf01_4
#define toggleButtonf02_4       UxScadaMandbMainContext->UxtoggleButtonf02_4
#define toggleButtonf03_4       UxScadaMandbMainContext->UxtoggleButtonf03_4
#define toggleButtonf04_4       UxScadaMandbMainContext->UxtoggleButtonf04_4
#define toggleButtonf05_4       UxScadaMandbMainContext->UxtoggleButtonf05_4
#define toggleButtonf06_4       UxScadaMandbMainContext->UxtoggleButtonf06_4
#define toggleButtonf07_4       UxScadaMandbMainContext->UxtoggleButtonf07_4
#define toggleButtonf08_4       UxScadaMandbMainContext->UxtoggleButtonf08_4
#define toggleButtonf09_4       UxScadaMandbMainContext->UxtoggleButtonf09_4
#define toggleButtonf10_4       UxScadaMandbMainContext->UxtoggleButtonf10_4
#define toggleButtonf11_4       UxScadaMandbMainContext->UxtoggleButtonf11_4
#define toggleButtonf12_4       UxScadaMandbMainContext->UxtoggleButtonf12_4
#define toggleButtonf13_4       UxScadaMandbMainContext->UxtoggleButtonf13_4
#define toggleButtonf14_4       UxScadaMandbMainContext->UxtoggleButtonf14_4
#define toggleButtonf15_4       UxScadaMandbMainContext->UxtoggleButtonf15_4
#define toggleButtonf16_4       UxScadaMandbMainContext->UxtoggleButtonf16_4
#define labelInfo4              UxScadaMandbMainContext->UxlabelInfo4
#define formMandbVal5           UxScadaMandbMainContext->UxformMandbVal5
#define textField5              UxScadaMandbMainContext->UxtextField5
#define pushButtonVal5          UxScadaMandbMainContext->UxpushButtonVal5
#define textFieldSt5            UxScadaMandbMainContext->UxtextFieldSt5
#define rowColumnToggle5        UxScadaMandbMainContext->UxrowColumnToggle5
#define toggleButton1_5         UxScadaMandbMainContext->UxtoggleButton1_5
#define toggleButton2_5         UxScadaMandbMainContext->UxtoggleButton2_5
#define pushButtonApply5        UxScadaMandbMainContext->UxpushButtonApply5
#define rowColumn5              UxScadaMandbMainContext->UxrowColumn5
#define toggleButtonf01_5       UxScadaMandbMainContext->UxtoggleButtonf01_5
#define toggleButtonf02_5       UxScadaMandbMainContext->UxtoggleButtonf02_5
#define toggleButtonf03_5       UxScadaMandbMainContext->UxtoggleButtonf03_5
#define toggleButtonf04_5       UxScadaMandbMainContext->UxtoggleButtonf04_5
#define toggleButtonf05_5       UxScadaMandbMainContext->UxtoggleButtonf05_5
#define toggleButtonf06_5       UxScadaMandbMainContext->UxtoggleButtonf06_5
#define toggleButtonf07_5       UxScadaMandbMainContext->UxtoggleButtonf07_5
#define toggleButtonf08_5       UxScadaMandbMainContext->UxtoggleButtonf08_5
#define toggleButtonf09_5       UxScadaMandbMainContext->UxtoggleButtonf09_5
#define toggleButtonf10_5       UxScadaMandbMainContext->UxtoggleButtonf10_5
#define toggleButtonf11_5       UxScadaMandbMainContext->UxtoggleButtonf11_5
#define toggleButtonf12_5       UxScadaMandbMainContext->UxtoggleButtonf12_5
#define toggleButtonf13_5       UxScadaMandbMainContext->UxtoggleButtonf13_5
#define toggleButtonf14_5       UxScadaMandbMainContext->UxtoggleButtonf14_5
#define toggleButtonf15_5       UxScadaMandbMainContext->UxtoggleButtonf15_5
#define toggleButtonf16_5       UxScadaMandbMainContext->UxtoggleButtonf16_5
#define labelInfo5              UxScadaMandbMainContext->UxlabelInfo5
#define pushButtonQuitMandb     UxScadaMandbMainContext->UxpushButtonQuitMandb
#define mode_1                  UxScadaMandbMainContext->Uxmode_1
#define mode_2                  UxScadaMandbMainContext->Uxmode_2
#define mode_3                  UxScadaMandbMainContext->Uxmode_3
#define mode_4                  UxScadaMandbMainContext->Uxmode_4
#define mode_5                  UxScadaMandbMainContext->Uxmode_5
#define timer_refresh           UxScadaMandbMainContext->Uxtimer_refresh
#define time_ref                UxScadaMandbMainContext->Uxtime_ref

static _UxCScadaMandbMain	*UxScadaMandbMainContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_ScadaMandbMain();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

close_mandb()
{
XtDestroyWidget(ScadaMandbMain);
MandbWidget = NULL;
}


refresh_mandb(Widget cw)
{
#ifndef DESIGN_TIME
_UxCScadaMandbMain *Context;



Context =(_UxCScadaMandbMain *) UxGetContext(cw);
Context->Uxtimer_refresh = XtAppAddTimeOut (
            XtWidgetToApplicationContext (cw),
            (unsigned long) (1000) ,refresh_mandb, cw);

if(mode_1 == 0)
	MandbRefresh(Context->UxlabelInfo1);
if(mode_2 == 0)
	MandbRefresh(Context->UxlabelInfo2);
if(mode_3 == 0)
	MandbRefresh(Context->UxlabelInfo3);
if(mode_4 == 0)
	MandbRefresh(Context->UxlabelInfo4);
if(mode_5 == 0)
	MandbRefresh(Context->UxlabelInfo5);
#endif
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_pushButtonVal1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	SetVar(labelInfo1);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButton1_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	Arg args[5];
	
	if(mode_1 == 0)
		{
		mode_1 =1;
		SetMode(labelInfo1);
		XtSetArg(args[0],XmNsensitive,True);
		XtSetValues(pushButtonApply1,args,1);
		XtSetArg(args[0],XmNeditable,True);
		XtSetArg(args[1],XmNcursorPositionVisible, True);
                XtSetValues(textFieldSt1,args,2);
		}
	else
		{
		mode_1 =0;
		VisMode(labelInfo1);
		XtSetArg(args[0],XmNsensitive,False);
		XtSetValues(pushButtonApply1,args,1);
		XtSetArg(args[0],XmNeditable,False);
		XtSetArg(args[1],XmNcursorPositionVisible, False);
                XtSetValues(textFieldSt1,args,2);
		}
	
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButton1_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButton2_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	activateCB_pushButtonApply1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	SetApplay(labelInfo1);
	XmToggleButtonSetState(toggleButton1_1,True,True);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf01_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf01_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf01_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf02_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf02_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf02_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf03_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf03_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf03_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf04_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf04_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf04_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf05_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf05_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf05_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf06_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf06_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf06_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf07_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf07_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf07_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf08_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf08_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf08_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf09_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf09_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf09_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf10_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf10_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf10_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf11_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf11_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf11_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf12_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf12_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf12_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf13_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf13_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf13_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf14_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf14_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf14_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf15_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf15_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf15_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf16_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	if(mode_1 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo1,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	disarmCB_toggleButtonf16_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	armCB_toggleButtonf16_1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	activateCB_pushButtonVal2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	SetVar(labelInfo2);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButton1_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	Arg args[5];
	
	if(mode_2 == 0)
		{
		mode_2 =1;
		SetMode(labelInfo2);
		XtSetArg(args[0],XmNsensitive,True);
		XtSetValues(pushButtonApply2,args,1);
		XtSetArg(args[0],XmNeditable,True);
		XtSetArg(args[1],XmNcursorPositionVisible, True);
                XtSetValues(textFieldSt2,args,2);
		}
	else
		{
		mode_2 =0;
		VisMode(labelInfo2);
		XtSetArg(args[0],XmNsensitive,False);
		XtSetValues(pushButtonApply2,args,1);
		XtSetArg(args[0],XmNeditable,False);
		XtSetArg(args[1],XmNcursorPositionVisible, False);
                XtSetValues(textFieldSt2,args,2);
		}
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	activateCB_pushButtonApply2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	SetApplay(labelInfo2);
	XmToggleButtonSetState(toggleButton1_2,True,True);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf01_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf02_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf03_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf04_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf05_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf06_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf07_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf08_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf09_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf10_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf11_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf12_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf13_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf14_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf15_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf16_2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_2 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo2,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	activateCB_pushButtonVal3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	SetVar(labelInfo3);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButton1_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	Arg args[5];
	
	if(mode_3 == 0)
		{
		mode_3 =1;
		SetMode(labelInfo3);
		XtSetArg(args[0],XmNsensitive,True);
		XtSetValues(pushButtonApply3,args,1);
		XtSetArg(args[0],XmNeditable,True);
		XtSetArg(args[1],XmNcursorPositionVisible, True);
                XtSetValues(textFieldSt3,args,2);
		}
	else
		{
		mode_3 =0;
		VisMode(labelInfo3);
		XtSetArg(args[0],XmNsensitive,False);
		XtSetValues(pushButtonApply3,args,1);
		XtSetArg(args[0],XmNeditable,False);
		XtSetArg(args[1],XmNcursorPositionVisible, False);
                XtSetValues(textFieldSt3,args,2);
		}
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	activateCB_pushButtonApply3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	SetApplay(labelInfo3);
	XmToggleButtonSetState(toggleButton1_3,True,True);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf01_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf02_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf03_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf04_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf05_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf06_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf07_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf08_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf09_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf10_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf11_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf12_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf13_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf14_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf15_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf16_3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_3 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo3,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	activateCB_pushButtonVal4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	SetVar(labelInfo4);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButton1_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	Arg args[5];
	
	if(mode_4 == 0)
		{
		mode_4 =1;
		SetMode(labelInfo4);
		XtSetArg(args[0],XmNsensitive,True);
		XtSetValues(pushButtonApply4,args,1);
		XtSetArg(args[0],XmNeditable,True);
		XtSetArg(args[1],XmNcursorPositionVisible, True);
                XtSetValues(textFieldSt4,args,2);
		}
	else
		{
		mode_4 =0;
		VisMode(labelInfo4);
		XtSetArg(args[0],XmNsensitive,False);
		XtSetValues(pushButtonApply4,args,1);
		XtSetArg(args[0],XmNeditable,False);
		XtSetArg(args[1],XmNcursorPositionVisible, False);
                XtSetValues(textFieldSt4,args,2);
		}
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	activateCB_pushButtonApply4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	SetApplay(labelInfo4);
	XmToggleButtonSetState(toggleButton1_4,True,True);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf01_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf02_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf03_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf04_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf05_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf06_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf07_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf08_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf09_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf10_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf11_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf12_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf13_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf14_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf15_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf16_4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_4 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo4,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	activateCB_pushButtonVal5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	SetVar(labelInfo5);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButton1_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	Arg args[5];
	
	if(mode_5 == 0)
		{
		mode_5 =1;
		SetMode(labelInfo5);
		XtSetArg(args[0],XmNsensitive,True);
		XtSetValues(pushButtonApply5,args,1);
		XtSetArg(args[0],XmNeditable,True);
		XtSetArg(args[1],XmNcursorPositionVisible, True);
                XtSetValues(textFieldSt5,args,2);
		}
	else
		{
		mode_5 =0;
		VisMode(labelInfo5);
		XtSetArg(args[0],XmNsensitive,False);
		XtSetValues(pushButtonApply5,args,1);
		XtSetArg(args[0],XmNeditable,False);
		XtSetArg(args[1],XmNcursorPositionVisible, False);
                XtSetValues(textFieldSt5,args,2);
		}
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	activateCB_pushButtonApply5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	SetApplay(labelInfo5);
	XmToggleButtonSetState(toggleButton1_5,True,True);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf01_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf02_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf03_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf04_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf05_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf06_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf07_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf08_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf09_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf10_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf11_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf12_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf13_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf14_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf15_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	valueChangedCB_toggleButtonf16_5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	if(mode_5 == 0)
		XmToggleButtonSetState(
			UxWidget,!XmToggleButtonGetState(UxWidget),
			False);
	else
		ChangeFlag(labelInfo5,UxWidget);
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

static void	activateCB_pushButtonQuitMandb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaMandbMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaMandbMainContext;
	UxScadaMandbMainContext = UxContext =
			(_UxCScadaMandbMain *) UxGetContext( UxWidget );
	{
	XtRemoveTimeOut(timer_refresh);
	close_mandb();
	}
	UxScadaMandbMainContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_ScadaMandbMain()
{
Pixmap pix;

	ScadaMandbMain = XtVaCreatePopupShell( "ScadaMandbMain",
			applicationShellWidgetClass, UxTopLevel,
			XmNheight, 744,
			XmNwidth, 545,
			XmNy, 230,
			XmNx, 350,
			NULL );

	UxPutContext( ScadaMandbMain, (char *) UxScadaMandbMainContext );

	formMandb = XtVaCreateManagedWidget( "formMandb",
			xmFormWidgetClass, ScadaMandbMain,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNheight, 470,
			XmNwidth, 410,
			XmNy, 10,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( formMandb, (char *) UxScadaMandbMainContext );

	scrolledWindowMandb = XtVaCreateManagedWidget( "scrolledWindowMandb",
			xmScrolledWindowWidgetClass, formMandb,
			XmNvisualPolicy, XmCONSTANT,
			XmNscrollBarDisplayPolicy, XmAS_NEEDED,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "#898989" ),
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomOffset, 60,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 670,
			XmNwidth, 545,
			XmNy, 0,
			XmNx, 0,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( scrolledWindowMandb, (char *) UxScadaMandbMainContext );

	formMandbMain = XtVaCreateManagedWidget( "formMandbMain",
			xmFormWidgetClass, scrolledWindowMandb,
			RES_CONVERT( XmNbackground, "#898989" ),
			XmNheight, 680,
			XmNwidth, 540,
			XmNy, 2,
			XmNx, 2,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( formMandbMain, (char *) UxScadaMandbMainContext );

	formMandbVal1 = XtVaCreateManagedWidget( "formMandbVal1",
			xmFormWidgetClass, formMandbMain,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNheight, 125,
			XmNwidth, 445,
			XmNy, 5,
			XmNx, 5,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( formMandbVal1, (char *) UxScadaMandbMainContext );

	textField1 = XtVaCreateManagedWidget( "textField1",
			xmTextFieldWidgetClass, formMandbVal1,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 15,
			XmNvalue, "Nome Variabile",
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 40,
			XmNwidth, 230,
			XmNy, 10,
			XmNx, 50,
			NULL );

	UxPutContext( textField1, (char *) UxScadaMandbMainContext );

	pushButtonVal1 = XtVaCreateManagedWidget( "pushButtonVal1",
			xmPushButtonWidgetClass, formMandbVal1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNlabelString, " . . . " ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 30,
			XmNwidth, 40,
			XmNy, 20,
			XmNx, 250,
			NULL );

	UxPutContext( pushButtonVal1, (char *) UxScadaMandbMainContext );

	textFieldSt1 = XtVaCreateManagedWidget( "textFieldSt1",
			xmTextFieldWidgetClass, formMandbVal1,
			XmNcursorPositionVisible, FALSE,
			XmNeditable, FALSE,
			XmNvalue, "                          0.0",
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 40,
			XmNwidth, 110,
			XmNy, 10,
			XmNx, 300,
			NULL );

	UxPutContext( textFieldSt1, (char *) UxScadaMandbMainContext );

	rowColumnToggle1 = XtVaCreateManagedWidget( "rowColumnToggle1",
			xmRowColumnWidgetClass, formMandbVal1,
			XmNtopOffset, 5,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNborderColor, "#1e87bc" ),
			XmNborderWidth, 4,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNradioBehavior, TRUE,
			XmNheight, 70,
			XmNwidth, 30,
			XmNy, 10,
			XmNx, 370,
			NULL );

	UxPutContext( rowColumnToggle1, (char *) UxScadaMandbMainContext );

	toggleButton1_1 = XtVaCreateManagedWidget( "toggleButton1_1",
			xmToggleButtonWidgetClass, rowColumnToggle1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorType, XmONE_OF_MANY,
			XmNindicatorOn, TRUE,
			XmNset, TRUE,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNlabelString, "Vis. mode" ),
			XmNheight, 20,
			XmNwidth, 10,
			XmNy, 10,
			XmNx, 10,
			NULL );

	UxPutContext( toggleButton1_1, (char *) UxScadaMandbMainContext );

	toggleButton2_1 = XtVaCreateManagedWidget( "toggleButton2_1",
			xmToggleButtonWidgetClass, rowColumnToggle1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorType, XmONE_OF_MANY,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNlabelString, "Set mode" ),
			XmNheight, 28,
			XmNwidth, 85,
			XmNy, 30,
			XmNx, 7,
			NULL );

	UxPutContext( toggleButton2_1, (char *) UxScadaMandbMainContext );

	pushButtonApply1 = XtVaCreateManagedWidget( "pushButtonApply1",
			xmPushButtonWidgetClass, formMandbVal1,
			XmNsensitive, FALSE,
			XmNmarginRight, 5,
			XmNmarginLeft, 5,
			XmNmarginTop, 5,
			XmNmarginBottom, 5,
			XmNtopWidget, rowColumnToggle1,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			RES_CONVERT( XmNlabelString, "Apply" ),
			XmNheight, 25,
			XmNwidth, 60,
			XmNy, 85,
			XmNx, 425,
			NULL );

	UxPutContext( pushButtonApply1, (char *) UxScadaMandbMainContext );

	rowColumn1 = XtVaCreateManagedWidget( "rowColumn1",
			xmRowColumnWidgetClass, formMandbVal1,
			RES_CONVERT( XmNbackground, "#002bff" ),
			XmNleftOffset, 5,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNpacking, XmPACK_COLUMN,
			XmNnumColumns, 8,
			XmNorientation, XmVERTICAL,
			XmNheight, 50,
			XmNwidth, 350,
			XmNy, 70,
			XmNx, 10,
			NULL );

	UxPutContext( rowColumn1, (char *) UxScadaMandbMainContext );

	toggleButtonf01_1 = XtVaCreateManagedWidget( "toggleButtonf01_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNlabelType, XmSTRING,
			RES_CONVERT( XmNforeground, "black" ),
			XmNsensitive, TRUE,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			XmNset, FALSE,
			XmNalignment, XmALIGNMENT_CENTER,
			XmNspacing, 4,
			XmNvisibleWhenOff, TRUE,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 49,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf01_1, (char *) UxScadaMandbMainContext );

	toggleButtonf02_1 = XtVaCreateManagedWidget( "toggleButtonf02_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf02_1, (char *) UxScadaMandbMainContext );

	toggleButtonf03_1 = XtVaCreateManagedWidget( "toggleButtonf03_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf03_1, (char *) UxScadaMandbMainContext );

	toggleButtonf04_1 = XtVaCreateManagedWidget( "toggleButtonf04_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, -34,
			NULL );

	UxPutContext( toggleButtonf04_1, (char *) UxScadaMandbMainContext );

	toggleButtonf05_1 = XtVaCreateManagedWidget( "toggleButtonf05_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -34,
			NULL );

	UxPutContext( toggleButtonf05_1, (char *) UxScadaMandbMainContext );

	toggleButtonf06_1 = XtVaCreateManagedWidget( "toggleButtonf06_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf06_1, (char *) UxScadaMandbMainContext );

	toggleButtonf07_1 = XtVaCreateManagedWidget( "toggleButtonf07_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf07_1, (char *) UxScadaMandbMainContext );

	toggleButtonf08_1 = XtVaCreateManagedWidget( "toggleButtonf08_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -35,
			NULL );

	UxPutContext( toggleButtonf08_1, (char *) UxScadaMandbMainContext );

	toggleButtonf09_1 = XtVaCreateManagedWidget( "toggleButtonf09_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, -35,
			NULL );

	UxPutContext( toggleButtonf09_1, (char *) UxScadaMandbMainContext );

	toggleButtonf10_1 = XtVaCreateManagedWidget( "toggleButtonf10_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf10_1, (char *) UxScadaMandbMainContext );

	toggleButtonf11_1 = XtVaCreateManagedWidget( "toggleButtonf11_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf11_1, (char *) UxScadaMandbMainContext );

	toggleButtonf12_1 = XtVaCreateManagedWidget( "toggleButtonf12_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 45,
			NULL );

	UxPutContext( toggleButtonf12_1, (char *) UxScadaMandbMainContext );

	toggleButtonf13_1 = XtVaCreateManagedWidget( "toggleButtonf13_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 45,
			NULL );

	UxPutContext( toggleButtonf13_1, (char *) UxScadaMandbMainContext );

	toggleButtonf14_1 = XtVaCreateManagedWidget( "toggleButtonf14_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -56,
			NULL );

	UxPutContext( toggleButtonf14_1, (char *) UxScadaMandbMainContext );

	toggleButtonf15_1 = XtVaCreateManagedWidget( "toggleButtonf15_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -15,
			NULL );

	UxPutContext( toggleButtonf15_1, (char *) UxScadaMandbMainContext );

	toggleButtonf16_1 = XtVaCreateManagedWidget( "toggleButtonf16_1",
			xmToggleButtonWidgetClass, rowColumn1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 24,
			NULL );

	UxPutContext( toggleButtonf16_1, (char *) UxScadaMandbMainContext );

	labelInfo1 = XtVaCreateManagedWidget( "labelInfo1",
			xmLabelWidgetClass, formMandbVal1,
			RES_CONVERT( XmNlabelString, "" ),
			XmNheight, 10,
			XmNwidth, 10,
			XmNy, 30,
			XmNx, 140,
			NULL );

	UxPutContext( labelInfo1, (char *) UxScadaMandbMainContext );

	formMandbVal2 = XtVaCreateManagedWidget( "formMandbVal2",
			xmFormWidgetClass, formMandbMain,
			XmNtopWidget, formMandbVal1,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNheight, 125,
			XmNwidth, 445,
			XmNy, 140,
			XmNx, 10,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( formMandbVal2, (char *) UxScadaMandbMainContext );

	textField2 = XtVaCreateManagedWidget( "textField2",
			xmTextFieldWidgetClass, formMandbVal2,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 15,
			XmNvalue, "Nome Variabile",
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 40,
			XmNwidth, 230,
			XmNy, 10,
			XmNx, 50,
			NULL );

	UxPutContext( textField2, (char *) UxScadaMandbMainContext );

	pushButtonVal2 = XtVaCreateManagedWidget( "pushButtonVal2",
			xmPushButtonWidgetClass, formMandbVal2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNlabelString, " . . . " ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 30,
			XmNwidth, 40,
			XmNy, 20,
			XmNx, 250,
			NULL );

	UxPutContext( pushButtonVal2, (char *) UxScadaMandbMainContext );

	textFieldSt2 = XtVaCreateManagedWidget( "textFieldSt2",
			xmTextFieldWidgetClass, formMandbVal2,
			XmNcursorPositionVisible, FALSE,
			XmNeditable, FALSE,
			XmNvalue, "                          0.0",
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 40,
			XmNwidth, 110,
			XmNy, 10,
			XmNx, 300,
			NULL );

	UxPutContext( textFieldSt2, (char *) UxScadaMandbMainContext );

	rowColumnToggle2 = XtVaCreateManagedWidget( "rowColumnToggle2",
			xmRowColumnWidgetClass, formMandbVal2,
			XmNtopOffset, 5,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNborderColor, "#1e87bc" ),
			XmNborderWidth, 4,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNradioBehavior, TRUE,
			XmNheight, 70,
			XmNwidth, 30,
			XmNy, 10,
			XmNx, 370,
			NULL );

	UxPutContext( rowColumnToggle2, (char *) UxScadaMandbMainContext );

	toggleButton1_2 = XtVaCreateManagedWidget( "toggleButton1_2",
			xmToggleButtonWidgetClass, rowColumnToggle2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorType, XmONE_OF_MANY,
			XmNindicatorOn, TRUE,
			XmNset, TRUE,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNlabelString, "Vis. mode" ),
			XmNheight, 20,
			XmNwidth, 10,
			XmNy, 10,
			XmNx, 10,
			NULL );

	UxPutContext( toggleButton1_2, (char *) UxScadaMandbMainContext );

	toggleButton2_2 = XtVaCreateManagedWidget( "toggleButton2_2",
			xmToggleButtonWidgetClass, rowColumnToggle2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorType, XmONE_OF_MANY,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNlabelString, "Set mode" ),
			XmNheight, 28,
			XmNwidth, 85,
			XmNy, 30,
			XmNx, 7,
			NULL );

	UxPutContext( toggleButton2_2, (char *) UxScadaMandbMainContext );

	pushButtonApply2 = XtVaCreateManagedWidget( "pushButtonApply2",
			xmPushButtonWidgetClass, formMandbVal2,
			XmNsensitive, FALSE,
			XmNmarginRight, 5,
			XmNmarginLeft, 5,
			XmNmarginTop, 5,
			XmNmarginBottom, 5,
			XmNtopWidget, rowColumnToggle2,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			RES_CONVERT( XmNlabelString, "Apply" ),
			XmNheight, 25,
			XmNwidth, 60,
			XmNy, 85,
			XmNx, 425,
			NULL );

	UxPutContext( pushButtonApply2, (char *) UxScadaMandbMainContext );

	rowColumn2 = XtVaCreateManagedWidget( "rowColumn2",
			xmRowColumnWidgetClass, formMandbVal2,
			RES_CONVERT( XmNbackground, "#002bff" ),
			XmNleftOffset, 5,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNpacking, XmPACK_COLUMN,
			XmNnumColumns, 8,
			XmNorientation, XmVERTICAL,
			XmNheight, 50,
			XmNwidth, 350,
			XmNy, 70,
			XmNx, 10,
			NULL );

	UxPutContext( rowColumn2, (char *) UxScadaMandbMainContext );

	toggleButtonf01_2 = XtVaCreateManagedWidget( "toggleButtonf01_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			XmNset, FALSE,
			XmNalignment, XmALIGNMENT_CENTER,
			XmNspacing, 4,
			XmNvisibleWhenOff, TRUE,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 49,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf01_2, (char *) UxScadaMandbMainContext );

	toggleButtonf02_2 = XtVaCreateManagedWidget( "toggleButtonf02_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf02_2, (char *) UxScadaMandbMainContext );

	toggleButtonf03_2 = XtVaCreateManagedWidget( "toggleButtonf03_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf03_2, (char *) UxScadaMandbMainContext );

	toggleButtonf04_2 = XtVaCreateManagedWidget( "toggleButtonf04_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, -34,
			NULL );

	UxPutContext( toggleButtonf04_2, (char *) UxScadaMandbMainContext );

	toggleButtonf05_2 = XtVaCreateManagedWidget( "toggleButtonf05_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -34,
			NULL );

	UxPutContext( toggleButtonf05_2, (char *) UxScadaMandbMainContext );

	toggleButtonf06_2 = XtVaCreateManagedWidget( "toggleButtonf06_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf06_2, (char *) UxScadaMandbMainContext );

	toggleButtonf07_2 = XtVaCreateManagedWidget( "toggleButtonf07_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf07_2, (char *) UxScadaMandbMainContext );

	toggleButtonf08_2 = XtVaCreateManagedWidget( "toggleButtonf08_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -35,
			NULL );

	UxPutContext( toggleButtonf08_2, (char *) UxScadaMandbMainContext );

	toggleButtonf09_2 = XtVaCreateManagedWidget( "toggleButtonf09_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, -35,
			NULL );

	UxPutContext( toggleButtonf09_2, (char *) UxScadaMandbMainContext );

	toggleButtonf10_2 = XtVaCreateManagedWidget( "toggleButtonf10_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf10_2, (char *) UxScadaMandbMainContext );

	toggleButtonf11_2 = XtVaCreateManagedWidget( "toggleButtonf11_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf11_2, (char *) UxScadaMandbMainContext );

	toggleButtonf12_2 = XtVaCreateManagedWidget( "toggleButtonf12_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 45,
			NULL );

	UxPutContext( toggleButtonf12_2, (char *) UxScadaMandbMainContext );

	toggleButtonf13_2 = XtVaCreateManagedWidget( "toggleButtonf13_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 45,
			NULL );

	UxPutContext( toggleButtonf13_2, (char *) UxScadaMandbMainContext );

	toggleButtonf14_2 = XtVaCreateManagedWidget( "toggleButtonf14_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -56,
			NULL );

	UxPutContext( toggleButtonf14_2, (char *) UxScadaMandbMainContext );

	toggleButtonf15_2 = XtVaCreateManagedWidget( "toggleButtonf15_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -15,
			NULL );

	UxPutContext( toggleButtonf15_2, (char *) UxScadaMandbMainContext );

	toggleButtonf16_2 = XtVaCreateManagedWidget( "toggleButtonf16_2",
			xmToggleButtonWidgetClass, rowColumn2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 24,
			NULL );

	UxPutContext( toggleButtonf16_2, (char *) UxScadaMandbMainContext );

	labelInfo2 = XtVaCreateManagedWidget( "labelInfo2",
			xmLabelWidgetClass, formMandbVal2,
			RES_CONVERT( XmNlabelString, "" ),
			XmNheight, 10,
			XmNwidth, 10,
			XmNy, 30,
			XmNx, 140,
			NULL );

	UxPutContext( labelInfo2, (char *) UxScadaMandbMainContext );

	formMandbVal3 = XtVaCreateManagedWidget( "formMandbVal3",
			xmFormWidgetClass, formMandbMain,
			XmNtopWidget, formMandbVal2,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNheight, 125,
			XmNwidth, 445,
			XmNy, 270,
			XmNx, 10,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( formMandbVal3, (char *) UxScadaMandbMainContext );

	textField3 = XtVaCreateManagedWidget( "textField3",
			xmTextFieldWidgetClass, formMandbVal3,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 15,
			XmNvalue, "Nome Variabile",
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 40,
			XmNwidth, 230,
			XmNy, 10,
			XmNx, 50,
			NULL );

	UxPutContext( textField3, (char *) UxScadaMandbMainContext );

	pushButtonVal3 = XtVaCreateManagedWidget( "pushButtonVal3",
			xmPushButtonWidgetClass, formMandbVal3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNlabelString, " . . . " ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 30,
			XmNwidth, 40,
			XmNy, 20,
			XmNx, 250,
			NULL );

	UxPutContext( pushButtonVal3, (char *) UxScadaMandbMainContext );

	textFieldSt3 = XtVaCreateManagedWidget( "textFieldSt3",
			xmTextFieldWidgetClass, formMandbVal3,
			XmNcursorPositionVisible, FALSE,
			XmNeditable, FALSE,
			XmNvalue, "                          0.0",
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 40,
			XmNwidth, 110,
			XmNy, 10,
			XmNx, 300,
			NULL );

	UxPutContext( textFieldSt3, (char *) UxScadaMandbMainContext );

	rowColumnToggle3 = XtVaCreateManagedWidget( "rowColumnToggle3",
			xmRowColumnWidgetClass, formMandbVal3,
			XmNtopOffset, 5,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNborderColor, "#1e87bc" ),
			XmNborderWidth, 4,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNradioBehavior, TRUE,
			XmNheight, 70,
			XmNwidth, 30,
			XmNy, 10,
			XmNx, 370,
			NULL );

	UxPutContext( rowColumnToggle3, (char *) UxScadaMandbMainContext );

	toggleButton1_3 = XtVaCreateManagedWidget( "toggleButton1_3",
			xmToggleButtonWidgetClass, rowColumnToggle3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorType, XmONE_OF_MANY,
			XmNindicatorOn, TRUE,
			XmNset, TRUE,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNlabelString, "Vis. mode" ),
			XmNheight, 20,
			XmNwidth, 10,
			XmNy, 10,
			XmNx, 10,
			NULL );

	UxPutContext( toggleButton1_3, (char *) UxScadaMandbMainContext );

	toggleButton2_3 = XtVaCreateManagedWidget( "toggleButton2_3",
			xmToggleButtonWidgetClass, rowColumnToggle3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorType, XmONE_OF_MANY,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNlabelString, "Set mode" ),
			XmNheight, 28,
			XmNwidth, 85,
			XmNy, 30,
			XmNx, 7,
			NULL );

	UxPutContext( toggleButton2_3, (char *) UxScadaMandbMainContext );

	pushButtonApply3 = XtVaCreateManagedWidget( "pushButtonApply3",
			xmPushButtonWidgetClass, formMandbVal3,
			XmNsensitive, FALSE,
			XmNmarginRight, 5,
			XmNmarginLeft, 5,
			XmNmarginTop, 5,
			XmNmarginBottom, 5,
			XmNtopWidget, rowColumnToggle3,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			RES_CONVERT( XmNlabelString, "Apply" ),
			XmNheight, 25,
			XmNwidth, 60,
			XmNy, 85,
			XmNx, 425,
			NULL );

	UxPutContext( pushButtonApply3, (char *) UxScadaMandbMainContext );

	rowColumn3 = XtVaCreateManagedWidget( "rowColumn3",
			xmRowColumnWidgetClass, formMandbVal3,
			RES_CONVERT( XmNbackground, "#002bff" ),
			XmNleftOffset, 5,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNpacking, XmPACK_COLUMN,
			XmNnumColumns, 8,
			XmNorientation, XmVERTICAL,
			XmNheight, 50,
			XmNwidth, 350,
			XmNy, 70,
			XmNx, 10,
			NULL );

	UxPutContext( rowColumn3, (char *) UxScadaMandbMainContext );

	toggleButtonf01_3 = XtVaCreateManagedWidget( "toggleButtonf01_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			XmNset, FALSE,
			XmNalignment, XmALIGNMENT_CENTER,
			XmNspacing, 4,
			XmNvisibleWhenOff, TRUE,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 49,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf01_3, (char *) UxScadaMandbMainContext );

	toggleButtonf02_3 = XtVaCreateManagedWidget( "toggleButtonf02_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf02_3, (char *) UxScadaMandbMainContext );

	toggleButtonf03_3 = XtVaCreateManagedWidget( "toggleButtonf03_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf03_3, (char *) UxScadaMandbMainContext );

	toggleButtonf04_3 = XtVaCreateManagedWidget( "toggleButtonf04_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, -34,
			NULL );

	UxPutContext( toggleButtonf04_3, (char *) UxScadaMandbMainContext );

	toggleButtonf05_3 = XtVaCreateManagedWidget( "toggleButtonf05_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -34,
			NULL );

	UxPutContext( toggleButtonf05_3, (char *) UxScadaMandbMainContext );

	toggleButtonf06_3 = XtVaCreateManagedWidget( "toggleButtonf06_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf06_3, (char *) UxScadaMandbMainContext );

	toggleButtonf07_3 = XtVaCreateManagedWidget( "toggleButtonf07_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf07_3, (char *) UxScadaMandbMainContext );

	toggleButtonf08_3 = XtVaCreateManagedWidget( "toggleButtonf08_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -35,
			NULL );

	UxPutContext( toggleButtonf08_3, (char *) UxScadaMandbMainContext );

	toggleButtonf09_3 = XtVaCreateManagedWidget( "toggleButtonf09_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, -35,
			NULL );

	UxPutContext( toggleButtonf09_3, (char *) UxScadaMandbMainContext );

	toggleButtonf10_3 = XtVaCreateManagedWidget( "toggleButtonf10_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf10_3, (char *) UxScadaMandbMainContext );

	toggleButtonf11_3 = XtVaCreateManagedWidget( "toggleButtonf11_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf11_3, (char *) UxScadaMandbMainContext );

	toggleButtonf12_3 = XtVaCreateManagedWidget( "toggleButtonf12_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 45,
			NULL );

	UxPutContext( toggleButtonf12_3, (char *) UxScadaMandbMainContext );

	toggleButtonf13_3 = XtVaCreateManagedWidget( "toggleButtonf13_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 45,
			NULL );

	UxPutContext( toggleButtonf13_3, (char *) UxScadaMandbMainContext );

	toggleButtonf14_3 = XtVaCreateManagedWidget( "toggleButtonf14_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -56,
			NULL );

	UxPutContext( toggleButtonf14_3, (char *) UxScadaMandbMainContext );

	toggleButtonf15_3 = XtVaCreateManagedWidget( "toggleButtonf15_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -15,
			NULL );

	UxPutContext( toggleButtonf15_3, (char *) UxScadaMandbMainContext );

	toggleButtonf16_3 = XtVaCreateManagedWidget( "toggleButtonf16_3",
			xmToggleButtonWidgetClass, rowColumn3,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 24,
			NULL );

	UxPutContext( toggleButtonf16_3, (char *) UxScadaMandbMainContext );

	labelInfo3 = XtVaCreateManagedWidget( "labelInfo3",
			xmLabelWidgetClass, formMandbVal3,
			RES_CONVERT( XmNlabelString, "" ),
			XmNheight, 10,
			XmNwidth, 10,
			XmNy, 30,
			XmNx, 140,
			NULL );

	UxPutContext( labelInfo3, (char *) UxScadaMandbMainContext );

	formMandbVal4 = XtVaCreateManagedWidget( "formMandbVal4",
			xmFormWidgetClass, formMandbMain,
			XmNtopWidget, formMandbVal3,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNheight, 125,
			XmNwidth, 445,
			XmNy, 400,
			XmNx, 10,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( formMandbVal4, (char *) UxScadaMandbMainContext );

	textField4 = XtVaCreateManagedWidget( "textField4",
			xmTextFieldWidgetClass, formMandbVal4,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 15,
			XmNvalue, "Nome Variabile",
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 40,
			XmNwidth, 230,
			XmNy, 10,
			XmNx, 50,
			NULL );

	UxPutContext( textField4, (char *) UxScadaMandbMainContext );

	pushButtonVal4 = XtVaCreateManagedWidget( "pushButtonVal4",
			xmPushButtonWidgetClass, formMandbVal4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNlabelString, " . . . " ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 30,
			XmNwidth, 40,
			XmNy, 20,
			XmNx, 250,
			NULL );

	UxPutContext( pushButtonVal4, (char *) UxScadaMandbMainContext );

	textFieldSt4 = XtVaCreateManagedWidget( "textFieldSt4",
			xmTextFieldWidgetClass, formMandbVal4,
			XmNcursorPositionVisible, FALSE,
			XmNeditable, FALSE,
			XmNvalue, "                          0.0",
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 40,
			XmNwidth, 110,
			XmNy, 10,
			XmNx, 300,
			NULL );

	UxPutContext( textFieldSt4, (char *) UxScadaMandbMainContext );

	rowColumnToggle4 = XtVaCreateManagedWidget( "rowColumnToggle4",
			xmRowColumnWidgetClass, formMandbVal4,
			XmNtopOffset, 5,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNborderColor, "#1e87bc" ),
			XmNborderWidth, 4,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNradioBehavior, TRUE,
			XmNheight, 70,
			XmNwidth, 30,
			XmNy, 10,
			XmNx, 370,
			NULL );

	UxPutContext( rowColumnToggle4, (char *) UxScadaMandbMainContext );

	toggleButton1_4 = XtVaCreateManagedWidget( "toggleButton1_4",
			xmToggleButtonWidgetClass, rowColumnToggle4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorType, XmONE_OF_MANY,
			XmNindicatorOn, TRUE,
			XmNset, TRUE,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNlabelString, "Vis. mode" ),
			XmNheight, 20,
			XmNwidth, 10,
			XmNy, 10,
			XmNx, 10,
			NULL );

	UxPutContext( toggleButton1_4, (char *) UxScadaMandbMainContext );

	toggleButton2_4 = XtVaCreateManagedWidget( "toggleButton2_4",
			xmToggleButtonWidgetClass, rowColumnToggle4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorType, XmONE_OF_MANY,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNlabelString, "Set mode" ),
			XmNheight, 28,
			XmNwidth, 85,
			XmNy, 30,
			XmNx, 7,
			NULL );

	UxPutContext( toggleButton2_4, (char *) UxScadaMandbMainContext );

	pushButtonApply4 = XtVaCreateManagedWidget( "pushButtonApply4",
			xmPushButtonWidgetClass, formMandbVal4,
			XmNsensitive, FALSE,
			XmNmarginRight, 5,
			XmNmarginLeft, 5,
			XmNmarginTop, 5,
			XmNmarginBottom, 5,
			XmNtopWidget, rowColumnToggle4,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			RES_CONVERT( XmNlabelString, "Apply" ),
			XmNheight, 25,
			XmNwidth, 60,
			XmNy, 85,
			XmNx, 425,
			NULL );

	UxPutContext( pushButtonApply4, (char *) UxScadaMandbMainContext );

	rowColumn4 = XtVaCreateManagedWidget( "rowColumn4",
			xmRowColumnWidgetClass, formMandbVal4,
			RES_CONVERT( XmNbackground, "#002bff" ),
			XmNleftOffset, 5,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNpacking, XmPACK_COLUMN,
			XmNnumColumns, 8,
			XmNorientation, XmVERTICAL,
			XmNheight, 50,
			XmNwidth, 350,
			XmNy, 70,
			XmNx, 10,
			NULL );

	UxPutContext( rowColumn4, (char *) UxScadaMandbMainContext );

	toggleButtonf01_4 = XtVaCreateManagedWidget( "toggleButtonf01_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			XmNset, FALSE,
			XmNalignment, XmALIGNMENT_CENTER,
			XmNspacing, 4,
			XmNvisibleWhenOff, TRUE,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 49,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf01_4, (char *) UxScadaMandbMainContext );

	toggleButtonf02_4 = XtVaCreateManagedWidget( "toggleButtonf02_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf02_4, (char *) UxScadaMandbMainContext );

	toggleButtonf03_4 = XtVaCreateManagedWidget( "toggleButtonf03_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf03_4, (char *) UxScadaMandbMainContext );

	toggleButtonf04_4 = XtVaCreateManagedWidget( "toggleButtonf04_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, -34,
			NULL );

	UxPutContext( toggleButtonf04_4, (char *) UxScadaMandbMainContext );

	toggleButtonf05_4 = XtVaCreateManagedWidget( "toggleButtonf05_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -34,
			NULL );

	UxPutContext( toggleButtonf05_4, (char *) UxScadaMandbMainContext );

	toggleButtonf06_4 = XtVaCreateManagedWidget( "toggleButtonf06_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf06_4, (char *) UxScadaMandbMainContext );

	toggleButtonf07_4 = XtVaCreateManagedWidget( "toggleButtonf07_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf07_4, (char *) UxScadaMandbMainContext );

	toggleButtonf08_4 = XtVaCreateManagedWidget( "toggleButtonf08_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -35,
			NULL );

	UxPutContext( toggleButtonf08_4, (char *) UxScadaMandbMainContext );

	toggleButtonf09_4 = XtVaCreateManagedWidget( "toggleButtonf09_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, -35,
			NULL );

	UxPutContext( toggleButtonf09_4, (char *) UxScadaMandbMainContext );

	toggleButtonf10_4 = XtVaCreateManagedWidget( "toggleButtonf10_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf10_4, (char *) UxScadaMandbMainContext );

	toggleButtonf11_4 = XtVaCreateManagedWidget( "toggleButtonf11_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf11_4, (char *) UxScadaMandbMainContext );

	toggleButtonf12_4 = XtVaCreateManagedWidget( "toggleButtonf12_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 45,
			NULL );

	UxPutContext( toggleButtonf12_4, (char *) UxScadaMandbMainContext );

	toggleButtonf13_4 = XtVaCreateManagedWidget( "toggleButtonf13_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 45,
			NULL );

	UxPutContext( toggleButtonf13_4, (char *) UxScadaMandbMainContext );

	toggleButtonf14_4 = XtVaCreateManagedWidget( "toggleButtonf14_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -56,
			NULL );

	UxPutContext( toggleButtonf14_4, (char *) UxScadaMandbMainContext );

	toggleButtonf15_4 = XtVaCreateManagedWidget( "toggleButtonf15_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -15,
			NULL );

	UxPutContext( toggleButtonf15_4, (char *) UxScadaMandbMainContext );

	toggleButtonf16_4 = XtVaCreateManagedWidget( "toggleButtonf16_4",
			xmToggleButtonWidgetClass, rowColumn4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 24,
			NULL );

	UxPutContext( toggleButtonf16_4, (char *) UxScadaMandbMainContext );

	labelInfo4 = XtVaCreateManagedWidget( "labelInfo4",
			xmLabelWidgetClass, formMandbVal4,
			RES_CONVERT( XmNlabelString, "" ),
			XmNheight, 10,
			XmNwidth, 10,
			XmNy, 30,
			XmNx, 140,
			NULL );

	UxPutContext( labelInfo4, (char *) UxScadaMandbMainContext );

	formMandbVal5 = XtVaCreateManagedWidget( "formMandbVal5",
			xmFormWidgetClass, formMandbMain,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopWidget, formMandbVal4,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNheight, 125,
			XmNwidth, 445,
			XmNy, 535,
			XmNx, 35,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( formMandbVal5, (char *) UxScadaMandbMainContext );

	textField5 = XtVaCreateManagedWidget( "textField5",
			xmTextFieldWidgetClass, formMandbVal5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 15,
			XmNvalue, "Nome Variabile",
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 40,
			XmNwidth, 230,
			XmNy, 10,
			XmNx, 50,
			NULL );

	UxPutContext( textField5, (char *) UxScadaMandbMainContext );

	pushButtonVal5 = XtVaCreateManagedWidget( "pushButtonVal5",
			xmPushButtonWidgetClass, formMandbVal5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNlabelString, " . . . " ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 30,
			XmNwidth, 40,
			XmNy, 20,
			XmNx, 250,
			NULL );

	UxPutContext( pushButtonVal5, (char *) UxScadaMandbMainContext );

	textFieldSt5 = XtVaCreateManagedWidget( "textFieldSt5",
			xmTextFieldWidgetClass, formMandbVal5,
			XmNcursorPositionVisible, FALSE,
			XmNeditable, FALSE,
			XmNvalue, "                          0.0",
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#fefef8" ),
			XmNheight, 40,
			XmNwidth, 110,
			XmNy, 10,
			XmNx, 300,
			NULL );

	UxPutContext( textFieldSt5, (char *) UxScadaMandbMainContext );

	rowColumnToggle5 = XtVaCreateManagedWidget( "rowColumnToggle5",
			xmRowColumnWidgetClass, formMandbVal5,
			XmNtopOffset, 5,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNborderColor, "#1e87bc" ),
			XmNborderWidth, 4,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNradioBehavior, TRUE,
			XmNheight, 70,
			XmNwidth, 30,
			XmNy, 10,
			XmNx, 370,
			NULL );

	UxPutContext( rowColumnToggle5, (char *) UxScadaMandbMainContext );

	toggleButton1_5 = XtVaCreateManagedWidget( "toggleButton1_5",
			xmToggleButtonWidgetClass, rowColumnToggle5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorType, XmONE_OF_MANY,
			XmNindicatorOn, TRUE,
			XmNset, TRUE,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNlabelString, "Vis. mode" ),
			XmNheight, 20,
			XmNwidth, 10,
			XmNy, 10,
			XmNx, 10,
			NULL );

	UxPutContext( toggleButton1_5, (char *) UxScadaMandbMainContext );

	toggleButton2_5 = XtVaCreateManagedWidget( "toggleButton2_5",
			xmToggleButtonWidgetClass, rowColumnToggle5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorType, XmONE_OF_MANY,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNlabelString, "Set mode" ),
			XmNheight, 28,
			XmNwidth, 85,
			XmNy, 30,
			XmNx, 7,
			NULL );

	UxPutContext( toggleButton2_5, (char *) UxScadaMandbMainContext );

	pushButtonApply5 = XtVaCreateManagedWidget( "pushButtonApply5",
			xmPushButtonWidgetClass, formMandbVal5,
			XmNsensitive, FALSE,
			XmNmarginRight, 5,
			XmNmarginLeft, 5,
			XmNmarginTop, 5,
			XmNmarginBottom, 5,
			XmNtopWidget, rowColumnToggle5,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			RES_CONVERT( XmNlabelString, "Apply" ),
			XmNheight, 25,
			XmNwidth, 60,
			XmNy, 85,
			XmNx, 425,
			NULL );

	UxPutContext( pushButtonApply5, (char *) UxScadaMandbMainContext );

	rowColumn5 = XtVaCreateManagedWidget( "rowColumn5",
			xmRowColumnWidgetClass, formMandbVal5,
			RES_CONVERT( XmNbackground, "#002bff" ),
			XmNleftOffset, 5,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNpacking, XmPACK_COLUMN,
			XmNnumColumns, 8,
			XmNorientation, XmVERTICAL,
			XmNheight, 50,
			XmNwidth, 350,
			XmNy, 70,
			XmNx, 10,
			NULL );

	UxPutContext( rowColumn5, (char *) UxScadaMandbMainContext );

	toggleButtonf01_5 = XtVaCreateManagedWidget( "toggleButtonf01_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			XmNset, FALSE,
			XmNalignment, XmALIGNMENT_CENTER,
			XmNspacing, 4,
			XmNvisibleWhenOff, TRUE,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 49,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf01_5, (char *) UxScadaMandbMainContext );

	toggleButtonf02_5 = XtVaCreateManagedWidget( "toggleButtonf02_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf02_5, (char *) UxScadaMandbMainContext );

	toggleButtonf03_5 = XtVaCreateManagedWidget( "toggleButtonf03_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf03_5, (char *) UxScadaMandbMainContext );

	toggleButtonf04_5 = XtVaCreateManagedWidget( "toggleButtonf04_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, -34,
			NULL );

	UxPutContext( toggleButtonf04_5, (char *) UxScadaMandbMainContext );

	toggleButtonf05_5 = XtVaCreateManagedWidget( "toggleButtonf05_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -34,
			NULL );

	UxPutContext( toggleButtonf05_5, (char *) UxScadaMandbMainContext );

	toggleButtonf06_5 = XtVaCreateManagedWidget( "toggleButtonf06_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf06_5, (char *) UxScadaMandbMainContext );

	toggleButtonf07_5 = XtVaCreateManagedWidget( "toggleButtonf07_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf07_5, (char *) UxScadaMandbMainContext );

	toggleButtonf08_5 = XtVaCreateManagedWidget( "toggleButtonf08_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -35,
			NULL );

	UxPutContext( toggleButtonf08_5, (char *) UxScadaMandbMainContext );

	toggleButtonf09_5 = XtVaCreateManagedWidget( "toggleButtonf09_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, -35,
			NULL );

	UxPutContext( toggleButtonf09_5, (char *) UxScadaMandbMainContext );

	toggleButtonf10_5 = XtVaCreateManagedWidget( "toggleButtonf10_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf10_5, (char *) UxScadaMandbMainContext );

	toggleButtonf11_5 = XtVaCreateManagedWidget( "toggleButtonf11_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 5,
			NULL );

	UxPutContext( toggleButtonf11_5, (char *) UxScadaMandbMainContext );

	toggleButtonf12_5 = XtVaCreateManagedWidget( "toggleButtonf12_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 45,
			NULL );

	UxPutContext( toggleButtonf12_5, (char *) UxScadaMandbMainContext );

	toggleButtonf13_5 = XtVaCreateManagedWidget( "toggleButtonf13_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 35,
			XmNx, 45,
			NULL );

	UxPutContext( toggleButtonf13_5, (char *) UxScadaMandbMainContext );

	toggleButtonf14_5 = XtVaCreateManagedWidget( "toggleButtonf14_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -56,
			NULL );

	UxPutContext( toggleButtonf14_5, (char *) UxScadaMandbMainContext );

	toggleButtonf15_5 = XtVaCreateManagedWidget( "toggleButtonf15_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, -15,
			NULL );

	UxPutContext( toggleButtonf15_5, (char *) UxScadaMandbMainContext );

	toggleButtonf16_5 = XtVaCreateManagedWidget( "toggleButtonf16_5",
			xmToggleButtonWidgetClass, rowColumn5,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNselectColor, "green" ),
			RES_CONVERT( XmNbackground, "#2fa1de" ),
			XmNindicatorSize, 15,
			RES_CONVERT( XmNlabelString, "F01" ),
			XmNindicatorType, XmN_OF_MANY,
			XmNheight, 20,
			XmNwidth, 45,
			XmNy, 15,
			XmNx, 24,
			NULL );

	UxPutContext( toggleButtonf16_5, (char *) UxScadaMandbMainContext );

	labelInfo5 = XtVaCreateManagedWidget( "labelInfo5",
			xmLabelWidgetClass, formMandbVal5,
			RES_CONVERT( XmNlabelString, "" ),
			XmNheight, 10,
			XmNwidth, 10,
			XmNy, 30,
			XmNx, 140,
			NULL );

	UxPutContext( labelInfo5, (char *) UxScadaMandbMainContext );

	pushButtonQuitMandb = XtVaCreateManagedWidget( "pushButtonQuitMandb",
			xmPushButtonWidgetClass, formMandb,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "Quit" ),
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 43,
			XmNwidth, 114,
			XmNy, 710,
			XmNx, 440,
			NULL );

	UxPutContext( pushButtonQuitMandb, (char *) UxScadaMandbMainContext );

	XtAddCallback( ScadaMandbMain, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( pushButtonVal1, XmNactivateCallback,
			activateCB_pushButtonVal1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButton1_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButton1_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButton1_1, XmNarmCallback,
			armCB_toggleButton1_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButton2_1, XmNarmCallback,
			armCB_toggleButton2_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( pushButtonApply1, XmNactivateCallback,
			activateCB_pushButtonApply1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf01_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf01_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf01_1, XmNdisarmCallback,
			disarmCB_toggleButtonf01_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf01_1, XmNarmCallback,
			armCB_toggleButtonf01_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf02_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf02_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf02_1, XmNdisarmCallback,
			disarmCB_toggleButtonf02_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf02_1, XmNarmCallback,
			armCB_toggleButtonf02_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf03_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf03_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf03_1, XmNdisarmCallback,
			disarmCB_toggleButtonf03_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf03_1, XmNarmCallback,
			armCB_toggleButtonf03_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf04_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf04_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf04_1, XmNdisarmCallback,
			disarmCB_toggleButtonf04_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf04_1, XmNarmCallback,
			armCB_toggleButtonf04_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf05_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf05_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf05_1, XmNdisarmCallback,
			disarmCB_toggleButtonf05_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf05_1, XmNarmCallback,
			armCB_toggleButtonf05_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf06_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf06_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf06_1, XmNdisarmCallback,
			disarmCB_toggleButtonf06_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf06_1, XmNarmCallback,
			armCB_toggleButtonf06_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf07_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf07_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf07_1, XmNdisarmCallback,
			disarmCB_toggleButtonf07_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf07_1, XmNarmCallback,
			armCB_toggleButtonf07_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf08_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf08_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf08_1, XmNdisarmCallback,
			disarmCB_toggleButtonf08_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf08_1, XmNarmCallback,
			armCB_toggleButtonf08_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf09_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf09_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf09_1, XmNdisarmCallback,
			disarmCB_toggleButtonf09_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf09_1, XmNarmCallback,
			armCB_toggleButtonf09_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf10_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf10_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf10_1, XmNdisarmCallback,
			disarmCB_toggleButtonf10_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf10_1, XmNarmCallback,
			armCB_toggleButtonf10_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf11_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf11_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf11_1, XmNdisarmCallback,
			disarmCB_toggleButtonf11_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf11_1, XmNarmCallback,
			armCB_toggleButtonf11_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf12_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf12_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf12_1, XmNdisarmCallback,
			disarmCB_toggleButtonf12_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf12_1, XmNarmCallback,
			armCB_toggleButtonf12_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf13_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf13_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf13_1, XmNdisarmCallback,
			disarmCB_toggleButtonf13_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf13_1, XmNarmCallback,
			armCB_toggleButtonf13_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf14_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf14_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf14_1, XmNdisarmCallback,
			disarmCB_toggleButtonf14_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf14_1, XmNarmCallback,
			armCB_toggleButtonf14_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf15_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf15_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf15_1, XmNdisarmCallback,
			disarmCB_toggleButtonf15_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf15_1, XmNarmCallback,
			armCB_toggleButtonf15_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf16_1, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf16_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf16_1, XmNdisarmCallback,
			disarmCB_toggleButtonf16_1,
			(XtPointer) UxScadaMandbMainContext );
	XtAddCallback( toggleButtonf16_1, XmNarmCallback,
			armCB_toggleButtonf16_1,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( pushButtonVal2, XmNactivateCallback,
			activateCB_pushButtonVal2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButton1_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButton1_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( pushButtonApply2, XmNactivateCallback,
			activateCB_pushButtonApply2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf01_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf01_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf02_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf02_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf03_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf03_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf04_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf04_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf05_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf05_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf06_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf06_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf07_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf07_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf08_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf08_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf09_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf09_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf10_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf10_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf11_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf11_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf12_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf12_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf13_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf13_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf14_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf14_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf15_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf15_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf16_2, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf16_2,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( pushButtonVal3, XmNactivateCallback,
			activateCB_pushButtonVal3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButton1_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButton1_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( pushButtonApply3, XmNactivateCallback,
			activateCB_pushButtonApply3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf01_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf01_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf02_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf02_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf03_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf03_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf04_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf04_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf05_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf05_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf06_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf06_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf07_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf07_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf08_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf08_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf09_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf09_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf10_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf10_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf11_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf11_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf12_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf12_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf13_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf13_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf14_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf14_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf15_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf15_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf16_3, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf16_3,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( pushButtonVal4, XmNactivateCallback,
			activateCB_pushButtonVal4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButton1_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButton1_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( pushButtonApply4, XmNactivateCallback,
			activateCB_pushButtonApply4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf01_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf01_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf02_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf02_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf03_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf03_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf04_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf04_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf05_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf05_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf06_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf06_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf07_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf07_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf08_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf08_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf09_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf09_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf10_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf10_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf11_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf11_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf12_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf12_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf13_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf13_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf14_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf14_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf15_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf15_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf16_4, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf16_4,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( pushButtonVal5, XmNactivateCallback,
			activateCB_pushButtonVal5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButton1_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButton1_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( pushButtonApply5, XmNactivateCallback,
			activateCB_pushButtonApply5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf01_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf01_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf02_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf02_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf03_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf03_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf04_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf04_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf05_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf05_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf06_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf06_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf07_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf07_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf08_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf08_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf09_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf09_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf10_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf10_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf11_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf11_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf12_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf12_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf13_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf13_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf14_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf14_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf15_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf15_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( toggleButtonf16_5, XmNvalueChangedCallback,
			valueChangedCB_toggleButtonf16_5,
			(XtPointer) UxScadaMandbMainContext );

	XtAddCallback( pushButtonQuitMandb, XmNactivateCallback,
			activateCB_pushButtonQuitMandb,
			(XtPointer) UxScadaMandbMainContext );


        LoadBitmap(ScadaMandbMain,&pix,MandbIconBitmap_bits,MandbIconBitmap_width,
                MandbIconBitmap_height);
        set_something_val(ScadaMandbMain,XmNiconPixmap, (XtArgVal) pix);

	return ( ScadaMandbMain );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_ScadaMandbMain()
{
	Widget                  rtrn;
	_UxCScadaMandbMain      *UxContext;

	UxScadaMandbMainContext = UxContext =
		(_UxCScadaMandbMain *) XtMalloc( sizeof(_UxCScadaMandbMain) );

	{
		mode_1 = 0;
		mode_2 = 0;
		mode_3 = 0;
		mode_4 = 0;
		mode_5 = 0;
		time_ref =5;
		rtrn = _Uxbuild_ScadaMandbMain();

		/*
			registrazione finestra mandb
		*/
		
		CreateFinMandb(labelInfo1);
		CreateFinMandb(labelInfo2);
		CreateFinMandb(labelInfo3);
		CreateFinMandb(labelInfo4);
		
		/*
			Carica il timer per il refresh
		*/
		timer_refresh = XtAppAddTimeOut (
		            XtWidgetToApplicationContext (rtrn),
		            (unsigned long) (1000*time_ref) ,refresh_mandb, rtrn);
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_ScadaMandbMain()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_ScadaMandbMain();

	return ( _Uxrtrn );
}
#endif

/*******************************************************************************
	END OF FILE
*******************************************************************************/

