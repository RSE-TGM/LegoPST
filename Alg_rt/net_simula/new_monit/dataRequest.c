
/*******************************************************************************
       dataRequest.c
       (Generated from interface file dataRequest.i)
*******************************************************************************/

#include <stdio.h>

#ifdef MOTIF
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#endif /* MOTIF */

#include "UxXt.h"
#include <Xm/SelectioB.h>
#include <Xm/Text.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "bistrutt.h"
#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"
char stringa[MAX_LUN_COMMENTO];
int risp;


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
	Widget	UxdataRequest;
	Widget	Uxw;
	char	*Uxlabel_string;
	char	*Uxact;
} _UxCdataRequest;

static _UxCdataRequest         *UxDataRequestContext;
#define dataRequest             UxDataRequestContext->UxdataRequest
#define w                       UxDataRequestContext->Uxw
#define label_string            UxDataRequestContext->Uxlabel_string
#define act                     UxDataRequestContext->Uxact



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

char	*create_dataRequest();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void  okCallback_dataRequest( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdataRequest         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDataRequestContext;
	UxDataRequestContext = UxContext =
			(_UxCdataRequest *) UxGetContext( UxWidget );
	{
	Widget campo_testo; 
	Widget padre; 
	char *t;
	
	   padre = (Widget)UxClientData;
	   campo_testo = XmSelectionBoxGetChild(padre,XmDIALOG_TEXT);
	   t = XmTextGetString (campo_testo);
	   strcpy (stringa,t);
	   XtFree (t);
	   DistruggiInterfaccia (XtParent(padre));
	   risp = 1;
	   
	
	}
	UxDataRequestContext = UxSaveCtx;
}

static void  cancelCB_dataRequest( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdataRequest         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDataRequestContext;
	UxDataRequestContext = UxContext =
			(_UxCdataRequest *) UxGetContext( UxWidget );
	{
	Widget padre; 
	   padre = (Widget)UxClientData;
	   strcpy (stringa,"");
	   DistruggiInterfaccia (XtParent(padre)); 
	   risp = 1;  
	   return;
	
	}
	UxDataRequestContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_dataRequest()
{
	Widget		_UxParent;
	char		*UxTmp0;
	char		*UxTmp1;

	UxTmp0 = label_string ? label_string : "";
	UxTmp1 = act ? act : "";

	/* Creation of dataRequest */
	_UxParent = w;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "dataRequest_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 844,
			XmNy, 619,
			XmNwidth, 360,
			XmNheight, 160,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "dataRequest",
			NULL );

	dataRequest = XtVaCreateWidget( "dataRequest",
			xmSelectionBoxWidgetClass,
			_UxParent,
			XmNdialogType, XmDIALOG_PROMPT,
			XmNwidth, 360,
			XmNheight, 160,
			XmNunitType, XmPIXELS,
			RES_CONVERT( XmNcancelLabelString, "Quit" ),
			RES_CONVERT( XmNselectionLabelString, UxTmp0 ),
			RES_CONVERT( XmNtextString, UxTmp1 ),
			XmNautoUnmanage, FALSE,
			RES_CONVERT( XmNchildPlacement, "place_above_selection" ),
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			NULL );
	XtAddCallback( dataRequest, XmNokCallback,
		(XtCallbackProc) okCallback_dataRequest,
		dataRequest );
	XtAddCallback( dataRequest, XmNcancelCallback,
		(XtCallbackProc) cancelCB_dataRequest,
		dataRequest );

	UxPutContext( dataRequest, (char *) UxDataRequestContext );


	XtAddCallback( dataRequest, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxDataRequestContext);


	return ( dataRequest );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

char	*create_dataRequest( _Uxw, _Uxlabel_string, _Uxact )
	Widget	_Uxw;
	char	*_Uxlabel_string;
	char	*_Uxact;
{
	Widget                  rtrn;
	_UxCdataRequest         *UxContext;
	static int		_Uxinit = 0;

	UxDataRequestContext = UxContext =
		(_UxCdataRequest *) UxNewContext( sizeof(_UxCdataRequest), False );

	w = _Uxw;
	label_string = _Uxlabel_string;
	act = _Uxact;

	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewInterfaceClassId();
		_Uxinit = 1;
	}

	{
		risp = 0;
		rtrn = _Uxbuild_dataRequest();
		UxPutClassCode( dataRequest, _UxIfClassId );

		XtUnmanageChild (
		    XmSelectionBoxGetChild(UxGetWidget(rtrn),XmDIALOG_HELP_BUTTON));
		UxPopupInterface (rtrn,no_grab);
		
		while (risp == 0)
		   {
		   XtAppProcessEvent (UxAppContext,XtIMAll);
		   XSync (UxDisplay,0);
		   }
		printf ("chiusura sessionName\n");
		risp = 0;
		
		return(&stringa[0]);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

