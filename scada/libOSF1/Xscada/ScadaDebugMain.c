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
	ScadaDebugMain.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"
#include "DebugIconBitmap.bmp"

#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/


extern Widget LogWidget;
extern char *XScadaDebug[100];
extern Widget DebugWidget;

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
	Widget	UxScadaDebugMain;
	Widget	UxformDebug;
	Widget	UxQuitDebugButton;
	Widget	UxscrolledWindow1;
	Widget	UxListDebug;
} _UxCScadaDebugMain;

#define ScadaDebugMain          UxScadaDebugMainContext->UxScadaDebugMain
#define formDebug               UxScadaDebugMainContext->UxformDebug
#define QuitDebugButton         UxScadaDebugMainContext->UxQuitDebugButton
#define scrolledWindow1         UxScadaDebugMainContext->UxscrolledWindow1
#define ListDebug               UxScadaDebugMainContext->UxListDebug

static _UxCScadaDebugMain	*UxScadaDebugMainContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	popup_ScadaDebugMain();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

close_debug()
{
XtDestroyWidget(ScadaDebugMain);
DebugWidget = NULL;
}

static SetLines()
{
int i;
XmString c_str;

XtUnmanageChild(ListDebug);
XmListDeleteAllItems(ListDebug);
for(i=0;i<100;i++)
	{
	if(XScadaDebug[i] == NULL)
		XScadaDebug[i]=XtNewString(" ");
	c_str = XmStringCreateLtoR(XScadaDebug[i], 
		XmSTRING_DEFAULT_CHARSET);
	XmListAddItemUnselected(ListDebug, c_str, i+1);
	}
XtManageChild(ListDebug);
}

SetDebug(char *testo, int line)
{
XmString c_str;

if(line>100)
	return;
if(line<1)
	return;
if(testo == NULL)
	return;


XtFree(XScadaDebug[line -1]);


XScadaDebug[line -1]=XtMalloc(strlen(testo)+1);
strcpy(XScadaDebug[line -1],testo);
if(DebugWidget != NULL)
	{
	c_str = XmStringCreateLtoR(XScadaDebug[line -1], 
		XmSTRING_DEFAULT_CHARSET);
	XmListDeletePos(ListDebug,line);
	XmListAddItemUnselected(ListDebug, c_str, line);
	}

}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_QuitDebugButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScadaDebugMain      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaDebugMainContext;
	UxScadaDebugMainContext = UxContext =
			(_UxCScadaDebugMain *) UxGetContext( UxWidget );
	{
	close_debug();
	}
	UxScadaDebugMainContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_ScadaDebugMain()
{
Pixmap pix;

	ScadaDebugMain = XtVaCreatePopupShell( "ScadaDebugMain",
			applicationShellWidgetClass, UxTopLevel,
			XmNheight, 350,
			XmNwidth, 300,
			XmNy, 230,
			XmNx, 730,
			NULL );

	UxPutContext( ScadaDebugMain, (char *) UxScadaDebugMainContext );

	formDebug = XtVaCreateManagedWidget( "formDebug",
			xmFormWidgetClass, ScadaDebugMain,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNheight, 350,
			XmNwidth, 290,
			XmNy, 0,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( formDebug, (char *) UxScadaDebugMainContext );

	QuitDebugButton = XtVaCreateManagedWidget( "QuitDebugButton",
			xmPushButtonWidgetClass, formDebug,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			RES_CONVERT( XmNlabelString, "Quit" ),
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1" ),
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_NONE,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 50,
			XmNwidth, 110,
			XmNy, 290,
			XmNx, 250,
			NULL );

	UxPutContext( QuitDebugButton, (char *) UxScadaDebugMainContext );

	scrolledWindow1 = XtVaCreateManagedWidget( "scrolledWindow1",
			xmScrolledWindowWidgetClass, formDebug,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "#ffffff" ),
			XmNbottomOffset, 65,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 0,
			XmNx, 10,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow1, (char *) UxScadaDebugMainContext );

	ListDebug = XtVaCreateManagedWidget( "ListDebug",
			xmListWidgetClass, scrolledWindow1,
			RES_CONVERT( XmNbackground, "#ffffff" ),
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1" ),
			XmNheight, 280,
			XmNwidth, 290,
			NULL );

	UxPutContext( ListDebug, (char *) UxScadaDebugMainContext );

	XtAddCallback( ScadaDebugMain, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxScadaDebugMainContext );

	XtAddCallback( QuitDebugButton, XmNactivateCallback,
			activateCB_QuitDebugButton,
			(XtPointer) UxScadaDebugMainContext );


        LoadBitmap(ScadaDebugMain,&pix,DebugIconBitmap_bits,DebugIconBitmap_width,
                DebugIconBitmap_height);
        set_something_val(ScadaDebugMain,XmNiconPixmap, (XtArgVal) pix);

	return ( ScadaDebugMain );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_popup_ScadaDebugMain()
{
	Widget                  rtrn;
	_UxCScadaDebugMain      *UxContext;

	UxScadaDebugMainContext = UxContext =
		(_UxCScadaDebugMain *) XtMalloc( sizeof(_UxCScadaDebugMain) );

printf("_Ux_popup_ScadaDebugMain()\n");
	rtrn = _Uxbuild_ScadaDebugMain();

	SetLines();
	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	popup_ScadaDebugMain()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_popup_ScadaDebugMain();

	return ( _Uxrtrn );
}
#endif

/*******************************************************************************
	END OF FILE
*******************************************************************************/

