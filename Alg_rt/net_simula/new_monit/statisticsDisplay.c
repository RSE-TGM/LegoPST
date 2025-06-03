
/*******************************************************************************
       statisticsDisplay.c
       (Generated from interface file statisticsDisplay.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/


#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

#include "sked.h"
#include "messaggi.h"
#include "bistrutt.h"
#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"
#include "refresh.h"
#ifndef DESIGN_TIME
#include "masterMenu.h"
#endif

extern VAL val;   /* dati consolidati       */
extern int nmod;  /* numero modelli attivi  */
XtIntervalId timer_statisticsDisplay;


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
	Widget	UxstatisticsDisplay;
	Widget	UxdrawingArea1;
	Widget	Uxmenu7;
	Widget	Uxmenu7_p1;
	Widget	UxstatDisplayMenuQuitpb;
	Widget	Uxmenu7_top_b1;
	Widget	Uxframe20;
	Widget	Uxform47;
	Widget	UxpreviewQuitpb1;
	Window	Uxdwg;
	Widget	UxUxParent;
} _UxCstatisticsDisplay;

static _UxCstatisticsDisplay   *UxStatisticsDisplayContext;
#define statisticsDisplay       UxStatisticsDisplayContext->UxstatisticsDisplay
#define drawingArea1            UxStatisticsDisplayContext->UxdrawingArea1
#define menu7                   UxStatisticsDisplayContext->Uxmenu7
#define menu7_p1                UxStatisticsDisplayContext->Uxmenu7_p1
#define statDisplayMenuQuitpb   UxStatisticsDisplayContext->UxstatDisplayMenuQuitpb
#define menu7_top_b1            UxStatisticsDisplayContext->Uxmenu7_top_b1
#define frame20                 UxStatisticsDisplayContext->Uxframe20
#define form47                  UxStatisticsDisplayContext->Uxform47
#define previewQuitpb1          UxStatisticsDisplayContext->UxpreviewQuitpb1
#define dwg                     UxStatisticsDisplayContext->Uxdwg
#define UxParent                UxStatisticsDisplayContext->UxUxParent



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_statisticsDisplay();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	exposeCB_drawingArea1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCstatisticsDisplay   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxStatisticsDisplayContext;
	UxStatisticsDisplayContext = UxContext =
			(_UxCstatisticsDisplay *) UxGetContext( UxWidget );
	{
	dwg = ((XmDrawingAreaCallbackStruct *)UxCallbackArg)->window;
	istogramma_tasks (UxWidget);
	
	}
	UxStatisticsDisplayContext = UxSaveCtx;
}

static	void	helpCB_drawingArea1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCstatisticsDisplay   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxStatisticsDisplayContext;
	UxStatisticsDisplayContext = UxContext =
			(_UxCstatisticsDisplay *) UxGetContext( UxWidget );
	{
	
	}
	UxStatisticsDisplayContext = UxSaveCtx;
}

static	void	createCB_drawingArea1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCstatisticsDisplay   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxStatisticsDisplayContext;
	UxContext = UxStatisticsDisplayContext;
	{
	
	}
	UxStatisticsDisplayContext = UxSaveCtx;
}

static	void	activateCB_statDisplayMenuQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCstatisticsDisplay   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxStatisticsDisplayContext;
	UxStatisticsDisplayContext = UxContext =
			(_UxCstatisticsDisplay *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	_UxCmasterMenu *CtxMaster;
	CtxMaster = (_UxCmasterMenu *)UxGetContext(UxParent);
	CtxMaster->UxstatisticheDisplayTask = (Widget)NULL;
#endif
	stop_timer (timer_statisticsDisplay);
	DistruggiInterfaccia(XtParent((Widget)UxClientData));
	}
	UxStatisticsDisplayContext = UxSaveCtx;
}

static	void	activateCB_previewQuitpb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCstatisticsDisplay   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxStatisticsDisplayContext;
	UxStatisticsDisplayContext = UxContext =
			(_UxCstatisticsDisplay *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	_UxCmasterMenu *CtxMaster;
	CtxMaster = (_UxCmasterMenu *)UxGetContext(UxParent);
	CtxMaster->UxstatisticheDisplayTask = (Widget)NULL;
#endif
	stop_timer (timer_statisticsDisplay);
	DistruggiInterfaccia(XtParent((Widget)UxClientData));
	}
	UxStatisticsDisplayContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_statisticsDisplay()
{
	Widget		_UxParent;
	Widget		menu7_p1_shell;


	/* Creation of statisticsDisplay */
	_UxParent = XtVaCreatePopupShell( "statisticsDisplay_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 230,
			XmNy, 380,
			XmNwidth, 290,
			XmNheight, 150,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "statisticsDisplay",
			NULL );

	statisticsDisplay = XtVaCreateWidget( "statisticsDisplay",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 290,
			XmNheight, 150,
			NULL );
	UxPutContext( statisticsDisplay, (char *) UxStatisticsDisplayContext );


	/* Creation of drawingArea1 */
	drawingArea1 = XtVaCreateManagedWidget( "drawingArea1",
			xmDrawingAreaWidgetClass,
			statisticsDisplay,
			XmNresizePolicy, XmRESIZE_ANY,
			XmNx, 10,
			XmNy, 40,
			XmNwidth, 300,
			XmNheight, 270,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNbottomWidget, NULL,
			XmNtopAttachment, XmATTACH_NONE,
			XmNtopWidget, NULL,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	XtAddCallback( drawingArea1, XmNexposeCallback,
		(XtCallbackProc) exposeCB_drawingArea1,
		(XtPointer) UxStatisticsDisplayContext );
	XtAddCallback( drawingArea1, XmNhelpCallback,
		(XtCallbackProc) helpCB_drawingArea1,
		(XtPointer) UxStatisticsDisplayContext );

	UxPutContext( drawingArea1, (char *) UxStatisticsDisplayContext );

	createCB_drawingArea1( drawingArea1,
			(XtPointer) UxStatisticsDisplayContext, (XtPointer) NULL );


	/* Creation of menu7 */
	menu7 = XtVaCreateManagedWidget( "menu7",
			xmRowColumnWidgetClass,
			statisticsDisplay,
			XmNrowColumnType, XmMENU_BAR,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, drawingArea1,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( menu7, (char *) UxStatisticsDisplayContext );


	/* Creation of menu7_p1 */
	menu7_p1_shell = XtVaCreatePopupShell ("menu7_p1_shell",
			xmMenuShellWidgetClass, menu7,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu7_p1 = XtVaCreateWidget( "menu7_p1",
			xmRowColumnWidgetClass,
			menu7_p1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu7_p1, (char *) UxStatisticsDisplayContext );


	/* Creation of statDisplayMenuQuitpb */
	statDisplayMenuQuitpb = XtVaCreateManagedWidget( "statDisplayMenuQuitpb",
			xmPushButtonWidgetClass,
			menu7_p1,
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );
	XtAddCallback( statDisplayMenuQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_statDisplayMenuQuitpb,
		UxGetWidget(statisticsDisplay) );

	UxPutContext( statDisplayMenuQuitpb, (char *) UxStatisticsDisplayContext );


	/* Creation of menu7_top_b1 */
	menu7_top_b1 = XtVaCreateManagedWidget( "menu7_top_b1",
			xmCascadeButtonWidgetClass,
			menu7,
			RES_CONVERT( XmNlabelString, "File" ),
			XmNsubMenuId, menu7_p1,
			NULL );
	UxPutContext( menu7_top_b1, (char *) UxStatisticsDisplayContext );


	/* Creation of frame20 */
	frame20 = XtVaCreateManagedWidget( "frame20",
			xmFrameWidgetClass,
			statisticsDisplay,
			XmNx, 10,
			XmNy, 310,
			XmNwidth, 280,
			XmNheight, 80,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, drawingArea1,
			NULL );
	UxPutContext( frame20, (char *) UxStatisticsDisplayContext );


	/* Creation of form47 */
	form47 = XtVaCreateManagedWidget( "form47",
			xmFormWidgetClass,
			frame20,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 2,
			XmNy, -20,
			XmNwidth, 600,
			XmNheight, 68,
			NULL );
	UxPutContext( form47, (char *) UxStatisticsDisplayContext );


	/* Creation of previewQuitpb1 */
	previewQuitpb1 = XtVaCreateManagedWidget( "previewQuitpb1",
			xmPushButtonWidgetClass,
			form47,
			XmNx, 493,
			XmNy, 35,
			XmNwidth, 100,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "QUIT" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 15,
			XmNrightOffset, 0,
			XmNleftPosition, 40,
			XmNleftOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNrightPosition, 60,
			NULL );
	XtAddCallback( previewQuitpb1, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewQuitpb1,
		UxGetWidget(statisticsDisplay) );

	UxPutContext( previewQuitpb1, (char *) UxStatisticsDisplayContext );

	XtVaSetValues(form47,
			XmNdefaultButton, previewQuitpb1,
			NULL );


	XtAddCallback( statisticsDisplay, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxStatisticsDisplayContext);


	return ( statisticsDisplay );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_statisticsDisplay( _UxUxParent )
	Widget	_UxUxParent;
{
	Widget                  rtrn;
	_UxCstatisticsDisplay   *UxContext;

	UxStatisticsDisplayContext = UxContext =
		(_UxCstatisticsDisplay *) UxNewContext( sizeof(_UxCstatisticsDisplay), False );

	UxParent = _UxUxParent;

	rtrn = _Uxbuild_statisticsDisplay();

	XtVaSetValues (drawingArea1,XmNheight,25*nmod,NULL);
	XtVaSetValues (statisticsDisplay,XmNheight,(25*nmod)+80+40,NULL);
	UxPopupInterface (rtrn, no_grab);
	attiva_timer_statistiche (UxGetWidget(drawingArea1));
	return(UxGetWidget(rtrn));
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

