
/*******************************************************************************
       processStart.c
       (Generated from interface file processStart.i)
*******************************************************************************/

#include <stdio.h>

#ifdef MOTIF
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#endif /* MOTIF */

#include "UxXt.h"
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "sim_param.h"
#include "messaggi.h"
#include "option.h"

extern OPTIONS_FLAGS options;


static	int _UxIfClassId;
#ifndef XKLOADDS
#define XKLOADDS
#endif /* XKLOADDS */

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
	Widget	UxprocessStart;
	Widget	Uxframe21;
	Widget	Uxform48;
	Widget	UxprocessStartOkpb;
	Widget	UxprocessStartQuitpb;
	Widget	UxdisplaySelectForm;
	Widget	Uxlabel1;
	Widget	UxstartProcessLabel;
	Widget	UxMenuShellDisplay;
	Widget	Uxmenu8_p1;
	Widget	UxdisplayMenu;
	Widget	UxdisplaySelectionButton[5];
	int	UxcloseRequest;
	char	UxselectedDisplay[100];
	swidget	UxUxParent;
	char	*Uxtesto;
} _UxCprocessStart;

static _UxCprocessStart        *UxProcessStartContext;
#define processStart            UxProcessStartContext->UxprocessStart
#define frame21                 UxProcessStartContext->Uxframe21
#define form48                  UxProcessStartContext->Uxform48
#define processStartOkpb        UxProcessStartContext->UxprocessStartOkpb
#define processStartQuitpb      UxProcessStartContext->UxprocessStartQuitpb
#define displaySelectForm       UxProcessStartContext->UxdisplaySelectForm
#define label1                  UxProcessStartContext->Uxlabel1
#define startProcessLabel       UxProcessStartContext->UxstartProcessLabel
#define MenuShellDisplay        UxProcessStartContext->UxMenuShellDisplay
#define menu8_p1                UxProcessStartContext->Uxmenu8_p1
#define displayMenu             UxProcessStartContext->UxdisplayMenu
#define displaySelectionButton  UxProcessStartContext->UxdisplaySelectionButton
#define closeRequest            UxProcessStartContext->UxcloseRequest
#define selectedDisplay         UxProcessStartContext->UxselectedDisplay
#define UxParent                UxProcessStartContext->UxUxParent
#define testo                   UxProcessStartContext->Uxtesto



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

char	*create_processStart();
int DistruggiInterfaccia (Widget );
int crea_display_selection (Widget, OPTIONS_FLAGS);


/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void  activateCB_processStartOkpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCprocessStart        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxProcessStartContext;
	UxProcessStartContext = UxContext =
			(_UxCprocessStart *) UxGetContext( UxWidget );
	{
	closeRequest = 1;
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
	}
	UxProcessStartContext = UxSaveCtx;
}

static void  activateCB_processStartQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCprocessStart        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxProcessStartContext;
	UxProcessStartContext = UxContext =
			(_UxCprocessStart *) UxGetContext( UxWidget );
	{
	strcpy (selectedDisplay,"NO");
	closeRequest = 1;
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
	}
	UxProcessStartContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_processStart()
{
	Widget		_UxParent;
	char		*UxTmp0;


	/* Creation of processStart */
	_UxParent = UxParent;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "processStart_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 560,
			XmNy, 400,
			XmNwidth, 360,
			XmNheight, 210,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "processStart",
			NULL );

	processStart = XtVaCreateWidget( "processStart",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 360,
			XmNheight, 210,
			NULL );
	UxPutContext( processStart, (char *) UxProcessStartContext );


	/* Creation of frame21 */
	frame21 = XtVaCreateManagedWidget( "frame21",
			xmFrameWidgetClass,
			processStart,
			XmNx, 0,
			XmNy, 150,
			XmNwidth, 360,
			XmNheight, 60,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame21, (char *) UxProcessStartContext );


	/* Creation of form48 */
	form48 = XtVaCreateManagedWidget( "form48",
			xmFormWidgetClass,
			frame21,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 90,
			XmNy, 10,
			XmNwidth, 200,
			XmNheight, 40,
			NULL );
	UxPutContext( form48, (char *) UxProcessStartContext );

	UxTmp0 = PROC_START_OK ? PROC_START_OK : "OK";

	/* Creation of processStartOkpb */
	processStartOkpb = XtVaCreateManagedWidget( "processStartOkpb",
			xmPushButtonWidgetClass,
			form48,
			XmNx, 20,
			XmNy, 10,
			XmNwidth, 100,
			XmNheight, 30,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 30,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			RES_CONVERT( XmNlabelString, UxTmp0 ),
			NULL );
	XtAddCallback( processStartOkpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_processStartOkpb,
		processStart );

	UxPutContext( processStartOkpb, (char *) UxProcessStartContext );

	UxTmp0 = PROC_START_Quit ? PROC_START_Quit : "Quit";

	/* Creation of processStartQuitpb */
	processStartQuitpb = XtVaCreateManagedWidget( "processStartQuitpb",
			xmPushButtonWidgetClass,
			form48,
			XmNx, 220,
			XmNy, 10,
			XmNwidth, 100,
			XmNheight, 30,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftPosition, 70,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			RES_CONVERT( XmNlabelString, UxTmp0 ),
			NULL );
	XtAddCallback( processStartQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_processStartQuitpb,
		processStart );

	UxPutContext( processStartQuitpb, (char *) UxProcessStartContext );


	/* Creation of displaySelectForm */
	displaySelectForm = XtVaCreateManagedWidget( "displaySelectForm",
			xmFormWidgetClass,
			processStart,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 10,
			XmNwidth, 360,
			XmNheight, 130,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, frame21,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( displaySelectForm, (char *) UxProcessStartContext );

	UxTmp0 = PROC_START_LABEL ? PROC_START_LABEL : "Process:";

	/* Creation of label1 */
	label1 = XtVaCreateManagedWidget( "label1",
			xmLabelWidgetClass,
			displaySelectForm,
			XmNx, 20,
			XmNy, 10,
			XmNwidth, 330,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, UxTmp0 ),
			NULL );
	UxPutContext( label1, (char *) UxProcessStartContext );

	UxTmp0 = testo ? testo : "NO TASK";

	/* Creation of startProcessLabel */
	startProcessLabel = XtVaCreateManagedWidget( "startProcessLabel",
			xmLabelWidgetClass,
			displaySelectForm,
			XmNx, 20,
			XmNy, 50,
			XmNwidth, 330,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, UxTmp0 ),
			NULL );
	UxPutContext( startProcessLabel, (char *) UxProcessStartContext );


	XtAddCallback( processStart, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxProcessStartContext);


	return ( processStart );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

char	*create_processStart( _UxUxParent, _Uxtesto )
	swidget	_UxUxParent;
	char	*_Uxtesto;
{
	Widget                  rtrn;
	_UxCprocessStart        *UxContext;
	static int		_Uxinit = 0;

	UxProcessStartContext = UxContext =
		(_UxCprocessStart *) UxNewContext( sizeof(_UxCprocessStart), False );

	UxParent = _UxUxParent;
	testo = _Uxtesto;

	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewInterfaceClassId();
		_Uxinit = 1;
	}

	{
		closeRequest = 0;
		strcpy (selectedDisplay,"");
		rtrn = _Uxbuild_processStart();
		UxPutClassCode( processStart, _UxIfClassId );

		crea_display_selection (displaySelectForm,options);
		UxPopupInterface (rtrn,no_grab);
		while (closeRequest == 0)
		   {
		   XtAppProcessEvent (UxAppContext,XtIMAll);
		   XSync (UxDisplay,0);
		   }
		closeRequest = 0;
		printf ("chiusura processStar; display [%s]\n",
		              selectedDisplay);
		return(selectedDisplay);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

