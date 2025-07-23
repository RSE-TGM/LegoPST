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
	ScadaLogMain.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"
#include "LogIconBitmap.bmp"

#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/ScrolledW.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/



extern Widget LogWidget;
extern Widget DebugWidget;
extern char *XScadaLog;

/*******************************************************************************
	The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxScadaLogMainMenuPost( wgt, client_data, event, ctd )
	Widget		wgt;
	XtPointer	client_data;
	XEvent		*event;
	Boolean		*ctd;
{
	Widget	menu = (Widget) client_data;
	int 	which_button;

	XtVaGetValues( menu, XmNwhichButton, &which_button, NULL );

	if ( event->xbutton.button == which_button )
	{
		XmMenuPosition( menu, event );
		XtManageChild( menu );
	}
}

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
	Widget	UxScadaLogMain;
	Widget	UxformLog;
	Widget	UxscrolledWindow;
	Widget	UxscrolledTextScada;
	Widget	Uxmenu1;
	Widget	Uxmenu1_p2_b1;
	Widget	UxQuitLogButton;
	Window	UxMioIcon;
} _UxCScadaLogMain;

#define ScadaLogMain            UxScadaLogMainContext->UxScadaLogMain
#define formLog                 UxScadaLogMainContext->UxformLog
#define scrolledWindow          UxScadaLogMainContext->UxscrolledWindow
#define scrolledTextScada       UxScadaLogMainContext->UxscrolledTextScada
#define menu1                   UxScadaLogMainContext->Uxmenu1
#define menu1_p2_b1             UxScadaLogMainContext->Uxmenu1_p2_b1
#define QuitLogButton           UxScadaLogMainContext->UxQuitLogButton
#define MioIcon                 UxScadaLogMainContext->UxMioIcon

static _UxCScadaLogMain	*UxScadaLogMainContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	popup_ScadaLogMain();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

close_log()
{
XtDestroyWidget(ScadaLogMain);
LogWidget = NULL;
}

AddLog(char *testo)
{
if (testo == NULL)
	return;
if(XScadaLog == NULL)
	{
	XScadaLog = (char *)XtMalloc(strlen(testo)+1);
	strcpy(XScadaLog,testo);
	}
else
	{
	XScadaLog = (char*)XtRealloc(XScadaLog,
		strlen(testo)+strlen(XScadaLog)+2);
	strcat(XScadaLog,testo);
	}
if(LogWidget != NULL)
	XmTextSetString( scrolledTextScada, XScadaLog );
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_menu1_p2_b1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaLogMain        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaLogMainContext;
	UxScadaLogMainContext = UxContext =
			(_UxCScadaLogMain *) UxGetContext( UxWidget );
	{
	XmTextSetString( scrolledTextScada, "" );
	if(XScadaLog != NULL)
		XtFree(XScadaLog);
	
	}
	UxScadaLogMainContext = UxSaveCtx;
}

static void	activateCB_QuitLogButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaLogMain        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaLogMainContext;
	UxScadaLogMainContext = UxContext =
			(_UxCScadaLogMain *) UxGetContext( UxWidget );
	{
	close_log();
	}
	UxScadaLogMainContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_ScadaLogMain()
{
Pixmap pix;

	Widget	menu1_shell;

	ScadaLogMain = XtVaCreatePopupShell( "ScadaLogMain",
			topLevelShellWidgetClass, UxTopLevel,
			XmNiconY, 10,
			XmNiconX, 10,
			XmNiconic, FALSE,
			XmNheight, 580,
			XmNwidth, 500,
			XmNy, 230,
			XmNx, 740,
			NULL );

	UxPutContext( ScadaLogMain, (char *) UxScadaLogMainContext );

	formLog = XtVaCreateManagedWidget( "formLog",
			xmFormWidgetClass, ScadaLogMain,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNheight, 610,
			XmNwidth, 530,
			XmNy, 10,
			XmNx, 30,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( formLog, (char *) UxScadaLogMainContext );

	scrolledWindow = XtVaCreateManagedWidget( "scrolledWindow",
			xmScrolledWindowWidgetClass, formLog,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomOffset, 65,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 300,
			XmNunitType, XmPIXELS,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 10,
			XmNx, 0,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow, (char *) UxScadaLogMainContext );

	scrolledTextScada = XtVaCreateManagedWidget( "scrolledTextScada",
			xmTextWidgetClass, scrolledWindow,
			RES_CONVERT( XmNbackground, "#ffffff" ),
			XmNwordWrap, TRUE,
			XmNvalue, "Log Scada",
			XmNscrollTopSide, FALSE,
			XmNscrollLeftSide, FALSE,
			XmNeditable, FALSE,
			XmNcursorPositionVisible, FALSE,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			XmNeditMode, XmMULTI_LINE_EDIT ,
			XmNheight, 300,
			XmNwidth, 610,
			NULL );

	UxPutContext( scrolledTextScada, (char *) UxScadaLogMainContext );

	menu1_shell = XtVaCreatePopupShell ("menu1_shell",
			xmMenuShellWidgetClass, scrolledTextScada,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu1 = XtVaCreateWidget( "menu1",
			xmRowColumnWidgetClass, menu1_shell,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNrowColumnType, XmMENU_POPUP,
			NULL );

	UxPutContext( menu1, (char *) UxScadaLogMainContext );

	menu1_p2_b1 = XtVaCreateManagedWidget( "menu1_p2_b1",
			xmPushButtonWidgetClass, menu1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			RES_CONVERT( XmNlabelString, "Clear messages" ),
			NULL );

	UxPutContext( menu1_p2_b1, (char *) UxScadaLogMainContext );

	QuitLogButton = XtVaCreateManagedWidget( "QuitLogButton",
			xmPushButtonWidgetClass, formLog,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_NONE,
			XmNleftOffset, 5,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			RES_CONVERT( XmNlabelString, "Quit" ),
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1" ),
			XmNheight, 50,
			XmNwidth, 110,
			XmNy, 290,
			XmNx, 250,
			NULL );

	UxPutContext( QuitLogButton, (char *) UxScadaLogMainContext );

	XtAddCallback( ScadaLogMain, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxScadaLogMainContext );

	XtAddCallback( menu1_p2_b1, XmNactivateCallback,
			activateCB_menu1_p2_b1,
			(XtPointer) UxScadaLogMainContext );

	XtAddCallback( QuitLogButton, XmNactivateCallback,
			activateCB_QuitLogButton,
			(XtPointer) UxScadaLogMainContext );


	XtAddEventHandler( scrolledTextScada, ButtonPressMask,
			False, _UxScadaLogMainMenuPost, menu1 );

       LoadBitmap(ScadaLogMain,&pix,LogIconBitmap_bits,LogIconBitmap_width,
                LogIconBitmap_height);
        set_something_val(ScadaLogMain,XmNiconPixmap, (XtArgVal) pix);

	return ( ScadaLogMain );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_popup_ScadaLogMain()
{
	Widget                  rtrn;
	_UxCScadaLogMain        *UxContext;

	UxScadaLogMainContext = UxContext =
		(_UxCScadaLogMain *) XtMalloc( sizeof(_UxCScadaLogMain) );

	rtrn = _Uxbuild_ScadaLogMain();

	if(XScadaLog != NULL)
		XmTextSetString( scrolledTextScada, XScadaLog );
	else
		XmTextSetString( scrolledTextScada, "" );
	
	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	popup_ScadaLogMain()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_popup_ScadaLogMain();

	return ( _Uxrtrn );
}
#endif

/*******************************************************************************
	END OF FILE
*******************************************************************************/

