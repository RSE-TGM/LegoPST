
/*******************************************************************************
       icCommentReqform.c
       (Generated from interface file icCommentReqform.i)
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
#include <Xm/TextF.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <Xm/Text.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/


#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "bistrutt.h"
#include "messaggi.h"
#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"
extern SNTAB *snap_header;
extern swidget initialCondition;


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
	Widget	Uxframe4;
	Widget	Uxform9;
	Widget	UxicCommantQuitpb;
	Widget	UxicCommentOkpb;
	Widget	UxicCommentText;
	Widget	UxicCommentTitle;
	int	Uxic_select;
	char	*UxprevComment;
} _UxCicCommentReqform;

static _UxCicCommentReqform    *UxIcCommentReqformContext;
#define frame4                  UxIcCommentReqformContext->Uxframe4
#define form9                   UxIcCommentReqformContext->Uxform9
#define icCommantQuitpb         UxIcCommentReqformContext->UxicCommantQuitpb
#define icCommentOkpb           UxIcCommentReqformContext->UxicCommentOkpb
#define icCommentText           UxIcCommentReqformContext->UxicCommentText
#define icCommentTitle          UxIcCommentReqformContext->UxicCommentTitle
#define ic_select               UxIcCommentReqformContext->Uxic_select
#define prevComment             UxIcCommentReqformContext->UxprevComment


Widget	icCommentReqform;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_icCommentReqform();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void  activateCB_icCommantQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCicCommentReqform    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxIcCommentReqformContext;
	UxIcCommentReqformContext = UxContext =
			(_UxCicCommentReqform *) UxGetContext( UxWidget );
	{
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
	}
	UxIcCommentReqformContext = UxSaveCtx;
}

static void  activateCB_icCommentOkpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCicCommentReqform    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxIcCommentReqformContext;
	UxIcCommentReqformContext = UxContext =
			(_UxCicCommentReqform *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   char *entry;
	   char commento[100];
	
	   entry = XmTextGetString (UxGetWidget(icCommentText));
	   strncpy (commento, entry, MAX_LUN_COMMENTO);
	   commento[MAX_LUN_COMMENTO] = '\0';
	   XtFree (entry);
	   printf ("commento = :%s:\n",commento);
	  
	   esegui_snap(ic_select,commento); 
	  
	   UxDestroyInterface(icCommentReqform);
#endif
	}
	UxIcCommentReqformContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_icCommentReqform()
{
	Widget		_UxParent;


	/* Creation of icCommentReqform */
	_UxParent = initialCondition;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "icCommentReqform_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 685,
			XmNy, 65,
			XmNwidth, 470,
			XmNheight, 205,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "icCommentReqform",
			NULL );

	icCommentReqform = XtVaCreateWidget( "icCommentReqform",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 470,
			XmNheight, 205,
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			NULL );
	UxPutContext( icCommentReqform, (char *) UxIcCommentReqformContext );


	/* Creation of frame4 */
	frame4 = XtVaCreateManagedWidget( "frame4",
			xmFrameWidgetClass,
			icCommentReqform,
			XmNx, 0,
			XmNy, 5,
			XmNwidth, 470,
			XmNheight, 60,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame4, (char *) UxIcCommentReqformContext );


	/* Creation of form9 */
	form9 = XtVaCreateManagedWidget( "form9",
			xmFormWidgetClass,
			frame4,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 15,
			XmNy, 20,
			XmNwidth, 650,
			XmNheight, 60,
			NULL );
	UxPutContext( form9, (char *) UxIcCommentReqformContext );


	/* Creation of icCommantQuitpb */
	icCommantQuitpb = XtVaCreateManagedWidget( "icCommantQuitpb",
			xmPushButtonWidgetClass,
			form9,
			XmNx, 350,
			XmNy, 10,
			XmNwidth, 100,
			XmNheight, 40,
			RES_CONVERT( XmNlabelString, "QUIT" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 10,
			XmNbottomOffset, 10,
			XmNleftPosition, 70,
			NULL );
	XtAddCallback( icCommantQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_icCommantQuitpb,
		UxGetWidget(icCommentReqform) );

	UxPutContext( icCommantQuitpb, (char *) UxIcCommentReqformContext );


	/* Creation of icCommentOkpb */
	icCommentOkpb = XtVaCreateManagedWidget( "icCommentOkpb",
			xmPushButtonWidgetClass,
			form9,
			XmNx, 20,
			XmNy, 15,
			XmNwidth, 100,
			XmNheight, 52,
			RES_CONVERT( XmNlabelString, "SAVE" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 30,
			XmNrightOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 10,
			XmNsensitive, TRUE,
			NULL );
	XtAddCallback( icCommentOkpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_icCommentOkpb,
		(XtPointer) UxIcCommentReqformContext );

	UxPutContext( icCommentOkpb, (char *) UxIcCommentReqformContext );


	/* Creation of icCommentText */
	icCommentText = XtVaCreateManagedWidget( "icCommentText",
			xmTextFieldWidgetClass,
			icCommentReqform,
			XmNx, 20,
			XmNy, 50,
			XmNwidth, 460,
			XmNheight, 40,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNmaxLength, MAX_LUN_COMMENTO,
			NULL );
	UxPutContext( icCommentText, (char *) UxIcCommentReqformContext );


	/* Creation of icCommentTitle */
	icCommentTitle = XtVaCreateManagedWidget( "icCommentTitle",
			xmLabelWidgetClass,
			icCommentReqform,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 473,
			XmNheight, 30,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, IC_COMMENT_REQ ),
			NULL );
	UxPutContext( icCommentTitle, (char *) UxIcCommentReqformContext );

	XtVaSetValues(icCommentReqform,
			XmNdefaultButton, icCommentOkpb,
			NULL );


	XtAddCallback( icCommentReqform, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxIcCommentReqformContext);


	return ( icCommentReqform );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_icCommentReqform( _Uxic_select, _UxprevComment )
	int	_Uxic_select;
	char	*_UxprevComment;
{
	Widget                  rtrn;
	_UxCicCommentReqform    *UxContext;
	static int		_Uxinit = 0;

	UxIcCommentReqformContext = UxContext =
		(_UxCicCommentReqform *) UxNewContext( sizeof(_UxCicCommentReqform), False );

	ic_select = _Uxic_select;
	prevComment = _UxprevComment;

	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewInterfaceClassId();
		_Uxinit = 1;
	}

	rtrn = _Uxbuild_icCommentReqform();
	UxPutClassCode( icCommentReqform, _UxIfClassId );

	fprintf (stderr,"prevComm [%s]\n",prevComment);
	write_txt (icCommentText, prevComment);
	UxPopupInterface (rtrn, no_grab);
	return(rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

