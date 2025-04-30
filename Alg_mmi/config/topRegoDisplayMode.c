
/*******************************************************************************
       topRegoDisplayMode.c
       (Generated from interface file topRegoDisplayMode.i)
       Associated Resource file: topRegoDisplayMode.rf
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo topRegoDisplayMode.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)topRegoDisplayMode.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <Xl/XlDispReg.h>
#include <Xl/XlPort.h>

#include "config.h"

/*******************************************************************************
       The definition of the context structure:
       If you create multiple copies of your interface, the context
       structure ensures that your callbacks use the variables for the
       correct copy.

       For each swidget in the interface, each argument to the Interface
       function, and each variable in the Interface Specific section of the
       Declarations Editor, there is an entry in the context structure.
       and a #define.  The #define makes the variable name refer to the
       corresponding entry in the context structure.
*******************************************************************************/

typedef	struct
{
	Widget	UxtopRegoDisplayMode;
	Widget	UxformRegoDisplayMode;
	Widget	UxlabelRegoDisplayMode;
	Widget	UxlabelRegoPortMode;
	Widget	UxlabelRegoPortMode1;
	Widget	UxrowColRegoDisplayMode;
	Widget	UxtoggleDispRegUnmanaged;
	Widget	UxtoggleDispRegTag;
	Widget	UxtoggleDispRegModul;
	Widget	UxtoggleDispRegValue;
	Widget	UxrowColRegoPortMode;
	Widget	UxtogglePortUnmanaged;
	Widget	UxtogglePortManaged;
	Widget	UxtogglePortTag;
	Widget	UxrowColRegoInterPortMode;
	Widget	UxtogglePortInterUnmanaged;
	Widget	UxtogglePortInterManaged;
	Widget	UxtogglePortInterTag;
	Widget	UxbuttonOkRegoDisplayMode;
	Widget	UxbuttonCancelRegoDisplayMode;
	int	UxstatoDispReg;
	int	UxstatoPort;
	int	UxstatoInterfacePort;
	Widget	Uxdrawing;
} _UxCtopRegoDisplayMode;

static _UxCtopRegoDisplayMode  *UxTopRegoDisplayModeContext;
#define topRegoDisplayMode      UxTopRegoDisplayModeContext->UxtopRegoDisplayMode
#define formRegoDisplayMode     UxTopRegoDisplayModeContext->UxformRegoDisplayMode
#define labelRegoDisplayMode    UxTopRegoDisplayModeContext->UxlabelRegoDisplayMode
#define labelRegoPortMode       UxTopRegoDisplayModeContext->UxlabelRegoPortMode
#define labelRegoPortMode1      UxTopRegoDisplayModeContext->UxlabelRegoPortMode1
#define rowColRegoDisplayMode   UxTopRegoDisplayModeContext->UxrowColRegoDisplayMode
#define toggleDispRegUnmanaged  UxTopRegoDisplayModeContext->UxtoggleDispRegUnmanaged
#define toggleDispRegTag        UxTopRegoDisplayModeContext->UxtoggleDispRegTag
#define toggleDispRegModul      UxTopRegoDisplayModeContext->UxtoggleDispRegModul
#define toggleDispRegValue      UxTopRegoDisplayModeContext->UxtoggleDispRegValue
#define rowColRegoPortMode      UxTopRegoDisplayModeContext->UxrowColRegoPortMode
#define togglePortUnmanaged     UxTopRegoDisplayModeContext->UxtogglePortUnmanaged
#define togglePortManaged       UxTopRegoDisplayModeContext->UxtogglePortManaged
#define togglePortTag           UxTopRegoDisplayModeContext->UxtogglePortTag
#define rowColRegoInterPortMode UxTopRegoDisplayModeContext->UxrowColRegoInterPortMode
#define togglePortInterUnmanaged UxTopRegoDisplayModeContext->UxtogglePortInterUnmanaged
#define togglePortInterManaged  UxTopRegoDisplayModeContext->UxtogglePortInterManaged
#define togglePortInterTag      UxTopRegoDisplayModeContext->UxtogglePortInterTag
#define buttonOkRegoDisplayMode UxTopRegoDisplayModeContext->UxbuttonOkRegoDisplayMode
#define buttonCancelRegoDisplayMode UxTopRegoDisplayModeContext->UxbuttonCancelRegoDisplayMode
#define statoDispReg            UxTopRegoDisplayModeContext->UxstatoDispReg
#define statoPort               UxTopRegoDisplayModeContext->UxstatoPort
#define statoInterfacePort      UxTopRegoDisplayModeContext->UxstatoInterfacePort
#define drawing                 UxTopRegoDisplayModeContext->Uxdrawing



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	popup_topRegoDisplayMode();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void SettaToggle(Widget UxWidget)
{
#ifndef DESIGN_TIME
        _UxCtopRegoDisplayMode  *UxSaveCtx, *UxContext;

        UxSaveCtx = UxTopRegoDisplayModeContext;
        UxTopRegoDisplayModeContext = UxContext =
                        (_UxCtopRegoDisplayMode *) UxGetContext( UxWidget );
#endif
        {
	switch(statoDispReg){
		case DISPREG_UNMANAGED:
			{
			XmToggleButtonSetState(toggleDispRegUnmanaged,True,False);
			break;
			}
		case DISPREG_TAG:
			{
			XmToggleButtonSetState(toggleDispRegTag,True,False);
			break;
			}
		case DISPREG_MODUL:
			{
			XmToggleButtonSetState(toggleDispRegModul,True,False);
			break;
			}
		case DISPREG_VALUE:
			{
			XmToggleButtonSetState(toggleDispRegValue,True,False);
			break;
			}
		}
	switch(statoPort){
		case PORT_UNMANAGED:
			{
			XmToggleButtonSetState(togglePortUnmanaged,True,False);
			break;
			}
		case PORT_MANAGED:
			{
			XmToggleButtonSetState(togglePortManaged,True,False);
			break;
			}
		case PORT_TAG:
			{
			XmToggleButtonSetState(togglePortTag,True,False);
			break;
			}
		}
	switch(statoInterfacePort){
		case INTERFACE_PORT_UNMANAGED:
			{
			XmToggleButtonSetState(togglePortInterUnmanaged,True,False);
			break;
			}
		case INTERFACE_PORT_MANAGED:
			{
			XmToggleButtonSetState(togglePortInterManaged,True,False);
			break;
			}
		case INTERFACE_PORT_TAG:
			{
			XmToggleButtonSetState(togglePortInterTag,True,False);
			break;
			}
		}


        

        }
#ifndef DESIGN_TIME
        UxTopRegoDisplayModeContext = UxSaveCtx;
#endif
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_buttonOkRegoDisplayMode( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopRegoDisplayMode  *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopRegoDisplayModeContext;
	UxTopRegoDisplayModeContext = UxContext =
			(_UxCtopRegoDisplayMode *) UxGetContext( UxWidget );
	{
	if(XmToggleButtonGetState(toggleDispRegUnmanaged))
		statoDispReg=DISPREG_UNMANAGED;
	if(XmToggleButtonGetState(toggleDispRegTag))
		statoDispReg=DISPREG_TAG;
	if(XmToggleButtonGetState(toggleDispRegModul))
		statoDispReg=DISPREG_MODUL;
	if(XmToggleButtonGetState(toggleDispRegValue))
		statoDispReg=DISPREG_VALUE;
	if(XmToggleButtonGetState(togglePortUnmanaged))
		statoPort=PORT_UNMANAGED;
	if(XmToggleButtonGetState(togglePortManaged))
		statoPort=PORT_MANAGED;
	if(XmToggleButtonGetState(togglePortTag))
		statoPort=PORT_TAG;
	if(XmToggleButtonGetState(togglePortInterUnmanaged))
		statoInterfacePort=INTERFACE_PORT_UNMANAGED;
	if(XmToggleButtonGetState(togglePortInterManaged))
		statoInterfacePort=INTERFACE_PORT_MANAGED;
	if(XmToggleButtonGetState(togglePortInterTag))
		statoInterfacePort=INTERFACE_PORT_TAG;
	
	
	
	SetRegoDisplayMode(drawing,statoDispReg, statoPort, statoInterfacePort);
	XtDestroyWidget(topRegoDisplayMode);
	}
	UxTopRegoDisplayModeContext = UxSaveCtx;
}

static	void	activateCB_buttonCancelRegoDisplayMode( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopRegoDisplayMode  *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopRegoDisplayModeContext;
	UxTopRegoDisplayModeContext = UxContext =
			(_UxCtopRegoDisplayMode *) UxGetContext( UxWidget );
	{
	XtDestroyWidget(topRegoDisplayMode);
	}
	UxTopRegoDisplayModeContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_topRegoDisplayMode()
{


	/* Creation of topRegoDisplayMode */
	topRegoDisplayMode = XtVaCreatePopupShell( "topRegoDisplayMode",
			topLevelShellWidgetClass,
			UxTopLevel,
			XmNx, 414,
			XmNy, 167,
			XmNwidth, 620,
			XmNheight, 300,
			NULL );
	UxPutContext( topRegoDisplayMode, (char *) UxTopRegoDisplayModeContext );


	/* Creation of formRegoDisplayMode */
	formRegoDisplayMode = XtVaCreateManagedWidget( "formRegoDisplayMode",
			xmFormWidgetClass,
			topRegoDisplayMode,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNunitType, XmPIXELS,
			XmNx, 5,
			XmNy, 4,
			XmNwidth, 546,
			XmNheight, 427,
			NULL );
	UxPutContext( formRegoDisplayMode, (char *) UxTopRegoDisplayModeContext );


	/* Creation of labelRegoDisplayMode */
	labelRegoDisplayMode = XtVaCreateManagedWidget( "labelRegoDisplayMode",
			xmLabelWidgetClass,
			formRegoDisplayMode,
			XmNx, 10,
			XmNy, 10,
			XmNwidth, 98,
			XmNheight, 29,
			RES_CONVERT( XmNlabelString, "Display Reg" ),
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNresizable, TRUE,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 20,
			XmNrecomputeSize, TRUE,
			XmNmarginBottom, 4,
			XmNmarginTop, 4,
			XmNborderWidth, 0,
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( labelRegoDisplayMode, (char *) UxTopRegoDisplayModeContext );


	/* Creation of labelRegoPortMode */
	labelRegoPortMode = XtVaCreateManagedWidget( "labelRegoPortMode",
			xmLabelWidgetClass,
			formRegoDisplayMode,
			XmNx, 10,
			XmNy, 10,
			XmNwidth, 98,
			XmNheight, 29,
			RES_CONVERT( XmNlabelString, "Port" ),
			XmNrecomputeSize, TRUE,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 25,
			XmNtopWidget, labelRegoDisplayMode,
			XmNmarginBottom, 3,
			XmNmarginTop, 3,
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( labelRegoPortMode, (char *) UxTopRegoDisplayModeContext );


	/* Creation of labelRegoPortMode1 */
	labelRegoPortMode1 = XtVaCreateManagedWidget( "labelRegoPortMode1",
			xmLabelWidgetClass,
			formRegoDisplayMode,
			XmNx, 10,
			XmNy, 10,
			XmNwidth, 98,
			XmNheight, 29,
			RES_CONVERT( XmNlabelString, "Interface port" ),
			XmNrecomputeSize, TRUE,
			XmNmarginBottom, 3,
			XmNmarginTop, 3,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 25,
			XmNtopWidget, labelRegoPortMode,
			XmNleftOffset, 10,
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( labelRegoPortMode1, (char *) UxTopRegoDisplayModeContext );


	/* Creation of rowColRegoDisplayMode */
	rowColRegoDisplayMode = XtVaCreateManagedWidget( "rowColRegoDisplayMode",
			xmRowColumnWidgetClass,
			formRegoDisplayMode,
			XmNx, -109,
			XmNy, 10,
			XmNwidth, 322,
			XmNheight, 46,
			XmNpacking, XmPACK_COLUMN,
			XmNradioBehavior, TRUE,
			XmNresizeHeight, TRUE,
			XmNresizeWidth, TRUE,
			XmNorientation, XmHORIZONTAL,
			RES_CONVERT( XmNlabelString, "" ),
			XmNleftOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNleftWidget, labelRegoDisplayMode,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 20,
			XmNentryAlignment, XmALIGNMENT_BEGINNING,
			XmNborderWidth, 1,
			NULL );
	UxPutContext( rowColRegoDisplayMode, (char *) UxTopRegoDisplayModeContext );


	/* Creation of toggleDispRegUnmanaged */
	toggleDispRegUnmanaged = XtVaCreateManagedWidget( "toggleDispRegUnmanaged",
			xmToggleButtonWidgetClass,
			rowColRegoDisplayMode,
			XmNx, 20,
			XmNy, 3,
			XmNwidth, 285,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "Unmanaged" ),
			RES_CONVERT( XmNselectColor, "#00c600" ),
			NULL );
	UxPutContext( toggleDispRegUnmanaged, (char *) UxTopRegoDisplayModeContext );


	/* Creation of toggleDispRegTag */
	toggleDispRegTag = XtVaCreateManagedWidget( "toggleDispRegTag",
			xmToggleButtonWidgetClass,
			rowColRegoDisplayMode,
			XmNx, 20,
			XmNy, 3,
			XmNwidth, 82,
			XmNheight, 22,
			RES_CONVERT( XmNlabelString, "Tag" ),
			RES_CONVERT( XmNselectColor, "#00c600" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNmarginLeft, 2,
			XmNfillOnSelect, TRUE,
			NULL );
	UxPutContext( toggleDispRegTag, (char *) UxTopRegoDisplayModeContext );


	/* Creation of toggleDispRegModul */
	toggleDispRegModul = XtVaCreateManagedWidget( "toggleDispRegModul",
			xmToggleButtonWidgetClass,
			rowColRegoDisplayMode,
			XmNx, 20,
			XmNy, 3,
			XmNwidth, 76,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "Modul" ),
			RES_CONVERT( XmNselectColor, "#00c600" ),
			NULL );
	UxPutContext( toggleDispRegModul, (char *) UxTopRegoDisplayModeContext );


	/* Creation of toggleDispRegValue */
	toggleDispRegValue = XtVaCreateManagedWidget( "toggleDispRegValue",
			xmToggleButtonWidgetClass,
			rowColRegoDisplayMode,
			XmNx, 20,
			XmNy, 3,
			XmNwidth, 80,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "Value" ),
			RES_CONVERT( XmNselectColor, "#00c600" ),
			NULL );
	UxPutContext( toggleDispRegValue, (char *) UxTopRegoDisplayModeContext );


	/* Creation of rowColRegoPortMode */
	rowColRegoPortMode = XtVaCreateManagedWidget( "rowColRegoPortMode",
			xmRowColumnWidgetClass,
			formRegoDisplayMode,
			XmNx, -109,
			XmNy, 10,
			XmNwidth, 322,
			XmNheight, 46,
			XmNpacking, XmPACK_COLUMN,
			XmNradioBehavior, TRUE,
			XmNresizeHeight, TRUE,
			XmNresizeWidth, TRUE,
			XmNorientation, XmHORIZONTAL,
			RES_CONVERT( XmNlabelString, "" ),
			XmNentryAlignment, XmALIGNMENT_BEGINNING,
			XmNborderWidth, 1,
			XmNleftOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 20,
			XmNtopWidget, rowColRegoDisplayMode,
			NULL );
	UxPutContext( rowColRegoPortMode, (char *) UxTopRegoDisplayModeContext );


	/* Creation of togglePortUnmanaged */
	togglePortUnmanaged = XtVaCreateManagedWidget( "togglePortUnmanaged",
			xmToggleButtonWidgetClass,
			rowColRegoPortMode,
			XmNx, 20,
			XmNy, 3,
			XmNwidth, 285,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "Unmanaged" ),
			RES_CONVERT( XmNselectColor, "#00c600" ),
			NULL );
	UxPutContext( togglePortUnmanaged, (char *) UxTopRegoDisplayModeContext );


	/* Creation of togglePortManaged */
	togglePortManaged = XtVaCreateManagedWidget( "togglePortManaged",
			xmToggleButtonWidgetClass,
			rowColRegoPortMode,
			XmNx, 20,
			XmNy, 3,
			XmNwidth, 82,
			XmNheight, 22,
			RES_CONVERT( XmNlabelString, "Managed" ),
			RES_CONVERT( XmNselectColor, "#00c600" ),
			NULL );
	UxPutContext( togglePortManaged, (char *) UxTopRegoDisplayModeContext );


	/* Creation of togglePortTag */
	togglePortTag = XtVaCreateManagedWidget( "togglePortTag",
			xmToggleButtonWidgetClass,
			rowColRegoPortMode,
			XmNx, 20,
			XmNy, 3,
			XmNwidth, 76,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "Tag" ),
			RES_CONVERT( XmNselectColor, "#00c600" ),
			NULL );
	UxPutContext( togglePortTag, (char *) UxTopRegoDisplayModeContext );


	/* Creation of rowColRegoInterPortMode */
	rowColRegoInterPortMode = XtVaCreateManagedWidget( "rowColRegoInterPortMode",
			xmRowColumnWidgetClass,
			formRegoDisplayMode,
			XmNx, -109,
			XmNy, 10,
			XmNwidth, 322,
			XmNheight, 46,
			XmNpacking, XmPACK_COLUMN,
			XmNradioBehavior, TRUE,
			XmNresizeHeight, TRUE,
			XmNresizeWidth, TRUE,
			XmNorientation, XmHORIZONTAL,
			RES_CONVERT( XmNlabelString, "" ),
			XmNentryAlignment, XmALIGNMENT_BEGINNING,
			XmNborderWidth, 1,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 20,
			XmNtopWidget, rowColRegoPortMode,
			NULL );
	UxPutContext( rowColRegoInterPortMode, (char *) UxTopRegoDisplayModeContext );


	/* Creation of togglePortInterUnmanaged */
	togglePortInterUnmanaged = XtVaCreateManagedWidget( "togglePortInterUnmanaged",
			xmToggleButtonWidgetClass,
			rowColRegoInterPortMode,
			XmNx, 20,
			XmNy, 3,
			XmNwidth, 285,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "Unmanaged" ),
			RES_CONVERT( XmNselectColor, "#00c600" ),
			NULL );
	UxPutContext( togglePortInterUnmanaged, (char *) UxTopRegoDisplayModeContext );


	/* Creation of togglePortInterManaged */
	togglePortInterManaged = XtVaCreateManagedWidget( "togglePortInterManaged",
			xmToggleButtonWidgetClass,
			rowColRegoInterPortMode,
			XmNx, 20,
			XmNy, 3,
			XmNwidth, 82,
			XmNheight, 22,
			RES_CONVERT( XmNlabelString, "Managed" ),
			RES_CONVERT( XmNselectColor, "#00c600" ),
			NULL );
	UxPutContext( togglePortInterManaged, (char *) UxTopRegoDisplayModeContext );


	/* Creation of togglePortInterTag */
	togglePortInterTag = XtVaCreateManagedWidget( "togglePortInterTag",
			xmToggleButtonWidgetClass,
			rowColRegoInterPortMode,
			XmNx, 20,
			XmNy, 3,
			XmNwidth, 76,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "Tag" ),
			RES_CONVERT( XmNselectColor, "#00c600" ),
			NULL );
	UxPutContext( togglePortInterTag, (char *) UxTopRegoDisplayModeContext );


	/* Creation of buttonOkRegoDisplayMode */
	buttonOkRegoDisplayMode = XtVaCreateManagedWidget( "buttonOkRegoDisplayMode",
			xmPushButtonWidgetClass,
			formRegoDisplayMode,
			XmNx, 14,
			XmNy, 380,
			XmNwidth, 95,
			XmNheight, 38,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNrecomputeSize, FALSE,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			NULL );
	XtAddCallback( buttonOkRegoDisplayMode, XmNactivateCallback,
		(XtCallbackProc) activateCB_buttonOkRegoDisplayMode,
		(XtPointer) UxTopRegoDisplayModeContext );

	UxPutContext( buttonOkRegoDisplayMode, (char *) UxTopRegoDisplayModeContext );


	/* Creation of buttonCancelRegoDisplayMode */
	buttonCancelRegoDisplayMode = XtVaCreateManagedWidget( "buttonCancelRegoDisplayMode",
			xmPushButtonWidgetClass,
			formRegoDisplayMode,
			XmNx, 125,
			XmNy, 382,
			XmNwidth, 83,
			XmNheight, 38,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNrecomputeSize, FALSE,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 20,
			XmNleftWidget, buttonOkRegoDisplayMode,
			NULL );
	XtAddCallback( buttonCancelRegoDisplayMode, XmNactivateCallback,
		(XtCallbackProc) activateCB_buttonCancelRegoDisplayMode,
		(XtPointer) UxTopRegoDisplayModeContext );

	UxPutContext( buttonCancelRegoDisplayMode, (char *) UxTopRegoDisplayModeContext );


	XtAddCallback( topRegoDisplayMode, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxTopRegoDisplayModeContext);


	return ( topRegoDisplayMode );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	popup_topRegoDisplayMode( _Uxdrawing )
	Widget	_Uxdrawing;
{
	Widget                  rtrn;
	_UxCtopRegoDisplayMode  *UxContext;
	static int		_Uxinit = 0;

	UxTopRegoDisplayModeContext = UxContext =
		(_UxCtopRegoDisplayMode *) UxNewContext( sizeof(_UxCtopRegoDisplayMode), False );

	drawing = _Uxdrawing;

	if ( ! _Uxinit )
	{
		UxLoadResources( "topRegoDisplayMode.rf" );
		_Uxinit = 1;
	}

	rtrn = _Uxbuild_topRegoDisplayMode();

	UxPopupInterface(rtrn,no_grab);
	GetRegoDisplayMode(drawing,&statoDispReg, &statoPort, &statoInterfacePort);
	SettaToggle(rtrn);
	return(rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

