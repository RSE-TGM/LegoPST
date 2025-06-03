
/*******************************************************************************
       sessionName.c
       (Generated from interface file sessionName.i)
*******************************************************************************/

#include <stdio.h>

#ifdef MOTIF
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#endif /* MOTIF */

#include "UxXt.h"
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <Xm/Text.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/


#ifndef DESIGN_TIME
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

#include "parametri.h"
#endif

#include "messaggi.h"

extern swidget topLevelShell1;
extern swidget masterMenu;

extern XtAppContext   UxAppContext;
char cbdata[20];
static int risposta = 0;


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
	Widget	Uxform5;
	Widget	Uxframe7;
	Widget	Uxform6;
	Widget	UxsessionNameSavepb;
	Widget	Uxlabel2;
	Widget	Uxform8;
	Widget	Uxlabel3;
	Widget	UxsessionTitleEntrytext;
	char	*Uxtit;
} _UxCsessionName;

static _UxCsessionName         *UxSessionNameContext;
#define form5                   UxSessionNameContext->Uxform5
#define frame7                  UxSessionNameContext->Uxframe7
#define form6                   UxSessionNameContext->Uxform6
#define sessionNameSavepb       UxSessionNameContext->UxsessionNameSavepb
#define label2                  UxSessionNameContext->Uxlabel2
#define form8                   UxSessionNameContext->Uxform8
#define label3                  UxSessionNameContext->Uxlabel3
#define sessionTitleEntrytext   UxSessionNameContext->UxsessionTitleEntrytext
#define tit                     UxSessionNameContext->Uxtit


Widget	sessionName;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_sessionName();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void  activateCB_sessionNameSavepb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsessionName         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSessionNameContext;
	UxSessionNameContext = UxContext =
			(_UxCsessionName *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   char *entry;
	   char commento[MAX_TITLE_LUN];
	  
	   fprintf (stderr,"save session name\n");
	   entry = XmTextGetString (UxGetWidget(sessionTitleEntrytext));
	   strncpy (commento, entry, MAX_TITLE_LUN);
	   commento[MAX_TITLE_LUN-1] = '\0';
	   XtFree (entry);
	   printf ("commento = :%s:\n",commento);
	   strcpy (cbdata, commento);
	   risposta = 1; /* sblocca il loop */
#endif
	  
	
	}
	UxSessionNameContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_sessionName()
{
	Widget		_UxParent;


	/* Creation of sessionName */
	_UxParent = masterMenu;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "sessionName_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 379,
			XmNy, 267,
			XmNwidth, 460,
			XmNheight, 225,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "sessionName",
			NULL );

	sessionName = XtVaCreateWidget( "sessionName",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 460,
			XmNheight, 225,
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			NULL );
	UxPutContext( sessionName, (char *) UxSessionNameContext );


	/* Creation of form5 */
	form5 = XtVaCreateManagedWidget( "form5",
			xmFormWidgetClass,
			sessionName,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 16,
			XmNy, 108,
			XmNwidth, 750,
			XmNheight, 75,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( form5, (char *) UxSessionNameContext );


	/* Creation of frame7 */
	frame7 = XtVaCreateManagedWidget( "frame7",
			xmFrameWidgetClass,
			form5,
			XmNx, 10,
			XmNy, 405,
			XmNwidth, 795,
			XmNheight, 50,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame7, (char *) UxSessionNameContext );


	/* Creation of form6 */
	form6 = XtVaCreateManagedWidget( "form6",
			xmFormWidgetClass,
			frame7,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 745,
			XmNheight, 72,
			NULL );
	UxPutContext( form6, (char *) UxSessionNameContext );


	/* Creation of sessionNameSavepb */
	sessionNameSavepb = XtVaCreateManagedWidget( "sessionNameSavepb",
			xmPushButtonWidgetClass,
			form6,
			XmNx, 30,
			XmNy, 405,
			XmNwidth, 205,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Continue" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNrightPosition, 60,
			XmNleftPosition, 40,
			NULL );
	XtAddCallback( sessionNameSavepb, XmNactivateCallback,
		(XtCallbackProc) activateCB_sessionNameSavepb,
		(XtPointer) cbdata );

	UxPutContext( sessionNameSavepb, (char *) UxSessionNameContext );


	/* Creation of label2 */
	label2 = XtVaCreateManagedWidget( "label2",
			xmLabelWidgetClass,
			sessionName,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 460,
			XmNheight, 40,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNalignment, XmALIGNMENT_CENTER,
			RES_CONVERT( XmNlabelString, NEW_SESSION_NAME ),
			NULL );
	UxPutContext( label2, (char *) UxSessionNameContext );


	/* Creation of form8 */
	form8 = XtVaCreateManagedWidget( "form8",
			xmFormWidgetClass,
			sessionName,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 20,
			XmNy, 45,
			XmNwidth, 540,
			XmNheight, 80,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 0,
			XmNtopWidget, label2,
			NULL );
	UxPutContext( form8, (char *) UxSessionNameContext );


	/* Creation of label3 */
	label3 = XtVaCreateManagedWidget( "label3",
			xmLabelWidgetClass,
			form8,
			XmNx, 0,
			XmNy, 20,
			XmNwidth, 110,
			XmNheight, 37,
			RES_CONVERT( XmNlabelString, "Title:" ),
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 20,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( label3, (char *) UxSessionNameContext );


	/* Creation of sessionTitleEntrytext */
	sessionTitleEntrytext = XtVaCreateManagedWidget( "sessionTitleEntrytext",
			xmTextFieldWidgetClass,
			form8,
			XmNx, 90,
			XmNy, 25,
			XmNwidth, 445,
			XmNheight, 35,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, label3,
			XmNrightOffset, 20,
			XmNvalue, tit ? tit : "",
			NULL );
	UxPutContext( sessionTitleEntrytext, (char *) UxSessionNameContext );

	XtVaSetValues(sessionName,
			XmNdefaultButton, sessionNameSavepb,
			NULL );

	XtVaSetValues(form6,
			XmNdefaultButton, sessionNameSavepb,
			XmNinitialFocus, sessionNameSavepb,
			NULL );


	XtAddCallback( sessionName, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxSessionNameContext);


	return ( sessionName );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_sessionName( _Uxtit )
	char	*_Uxtit;
{
	Widget                  rtrn;
	_UxCsessionName         *UxContext;
	static int		_Uxinit = 0;

	UxSessionNameContext = UxContext =
		(_UxCsessionName *) UxNewContext( sizeof(_UxCsessionName), False );

	tit = _Uxtit;

	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewInterfaceClassId();
		_Uxinit = 1;
	}

	{
		printf ("creazione session Name\n");
		rtrn = _Uxbuild_sessionName();
		UxPutClassCode( sessionName, _UxIfClassId );

		UxPopupInterface (rtrn, no_grab);
		while (risposta == 0)
		   {
		   XtAppProcessEvent (UxAppContext,XtIMAll);
		   XSync (UxDisplay,0);
		   }
		printf ("chiusura sessionName\n");
		risposta = 0;
		strcpy (tit, cbdata);
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

