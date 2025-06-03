
/*******************************************************************************
	resourceSetting.c

       Associated Header file: resourceSetting.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/ScrolledW.h>
#include <Xm/CascadeB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "risorse.h"
#include "messaggi.h"
#include "risorse_default.h"

#include "sim_types.h"
#include "bistrutt.h"
#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"

RESOURCE_DEF *Rdef; /* risorse configurabili da utente */


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "resourceSetting.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	destroyCB_resourceSetting(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCresourceSetting     *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxResourceSettingContext;
	UxResourceSettingContext = UxContext =
			(_UxCresourceSetting *) UxGetContext( UxWidget );
	{
	clearResourceList (UxWidget);
	}
	UxResourceSettingContext = UxSaveCtx;
}

static	void	activateCB_applyResourceSetting(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCresourceSetting     *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxResourceSettingContext;
	UxResourceSettingContext = UxContext =
			(_UxCresourceSetting *) UxGetContext( UxWidget );
	{
	applyResource (UxWidget);
	}
	UxResourceSettingContext = UxSaveCtx;
}

static	void	activateCB_resourceSettingFileQuit(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCresourceSetting     *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxResourceSettingContext;
	UxResourceSettingContext = UxContext =
			(_UxCresourceSetting *) UxGetContext( UxWidget );
	{
	DistruggiInterfaccia (resourceSetting);
	}
	UxResourceSettingContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_resourceSetting()
{
	Widget		_UxParent;
	Widget		resourceSettingFile_shell;


	/* Creation of resourceSetting */
	_UxParent = UxParent;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "resourceSetting_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 240,
			XmNy, 430,
			XmNwidth, 420,
			XmNheight, 460,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "resourceSetting",
			NULL );

	resourceSetting = XtVaCreateWidget( "resourceSetting",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 420,
			XmNheight, 460,
			NULL );
	XtAddCallback( resourceSetting, XmNdestroyCallback,
		(XtCallbackProc) destroyCB_resourceSetting,
		(XtPointer) UxResourceSettingContext );

	UxPutContext( resourceSetting, (char *) UxResourceSettingContext );


	/* Creation of applyFrame */
	applyFrame = XtVaCreateManagedWidget( "applyFrame",
			xmFrameWidgetClass,
			resourceSetting,
			XmNx, -267,
			XmNy, 140,
			XmNwidth, 795,
			XmNheight, 50,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( applyFrame, (char *) UxResourceSettingContext );


	/* Creation of form33 */
	form33 = XtVaCreateManagedWidget( "form33",
			xmFormWidgetClass,
			applyFrame,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 745,
			XmNheight, 72,
			NULL );
	UxPutContext( form33, (char *) UxResourceSettingContext );


	/* Creation of applyResourceSetting */
	applyResourceSetting = XtVaCreateManagedWidget( "applyResourceSetting",
			xmPushButtonWidgetClass,
			form33,
			XmNx, 100,
			XmNy, 20,
			XmNwidth, 130,
			XmNheight, 40,
			RES_CONVERT( XmNlabelString, APPLYLABEL ),
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftPosition, 30,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 70,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 12,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 12,
			NULL );
	XtAddCallback( applyResourceSetting, XmNactivateCallback,
		(XtCallbackProc) activateCB_applyResourceSetting,
		(XtPointer) UxResourceSettingContext );

	UxPutContext( applyResourceSetting, (char *) UxResourceSettingContext );


	/* Creation of resourceSettingMenu */
	resourceSettingMenu = XtVaCreateManagedWidget( "resourceSettingMenu",
			xmRowColumnWidgetClass,
			resourceSetting,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( resourceSettingMenu, (char *) UxResourceSettingContext );


	/* Creation of resourceSettingFile */
	resourceSettingFile_shell = XtVaCreatePopupShell ("resourceSettingFile_shell",
			xmMenuShellWidgetClass, resourceSettingMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	resourceSettingFile = XtVaCreateWidget( "resourceSettingFile",
			xmRowColumnWidgetClass,
			resourceSettingFile_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( resourceSettingFile, (char *) UxResourceSettingContext );


	/* Creation of resourceSettingFileQuit */
	resourceSettingFileQuit = XtVaCreateManagedWidget( "resourceSettingFileQuit",
			xmPushButtonWidgetClass,
			resourceSettingFile,
			RES_CONVERT( XmNlabelString, QuitMenuLabel ),
			NULL );
	XtAddCallback( resourceSettingFileQuit, XmNactivateCallback,
		(XtCallbackProc) activateCB_resourceSettingFileQuit,
		(XtPointer) UxResourceSettingContext );

	UxPutContext( resourceSettingFileQuit, (char *) UxResourceSettingContext );


	/* Creation of menu15_top_b1 */
	menu15_top_b1 = XtVaCreateManagedWidget( "menu15_top_b1",
			xmCascadeButtonWidgetClass,
			resourceSettingMenu,
			RES_CONVERT( XmNlabelString, FileMenuLabel ),
			XmNsubMenuId, resourceSettingFile,
			NULL );
	UxPutContext( menu15_top_b1, (char *) UxResourceSettingContext );


	/* Creation of form57 */
	form57 = XtVaCreateManagedWidget( "form57",
			xmFormWidgetClass,
			resourceSetting,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 50,
			XmNy, 80,
			XmNwidth, 330,
			XmNheight, 290,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, applyFrame,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, resourceSettingMenu,
			NULL );
	UxPutContext( form57, (char *) UxResourceSettingContext );


	/* Creation of resourceSettingCategSelectForm */
	resourceSettingCategSelectForm = XtVaCreateManagedWidget( "resourceSettingCategSelectForm",
			xmFormWidgetClass,
			form57,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 20,
			XmNy, 10,
			XmNwidth, 380,
			XmNheight, 50,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			NULL );
	UxPutContext( resourceSettingCategSelectForm, (char *) UxResourceSettingContext );


	/* Creation of ResSetForm */
	ResSetForm = XtVaCreateManagedWidget( "ResSetForm",
			xmFormWidgetClass,
			form57,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 20,
			XmNy, 120,
			XmNwidth, 360,
			XmNheight, 190,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, resourceSettingCategSelectForm,
			NULL );
	UxPutContext( ResSetForm, (char *) UxResourceSettingContext );


	/* Creation of scrollWindResSet */
	scrollWindResSet = XtVaCreateManagedWidget( "scrollWindResSet",
			xmScrolledWindowWidgetClass,
			ResSetForm,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 40,
			XmNy, 30,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 2,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 20,
			XmNleftOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( scrollWindResSet, (char *) UxResourceSettingContext );


	/* Creation of resourceSettingResRc */
	resourceSettingResRc = XtVaCreateManagedWidget( "resourceSettingResRc",
			xmRowColumnWidgetClass,
			scrollWindResSet,
			XmNx, 20,
			XmNy, 20,
			XmNwidth, 350,
			XmNheight, 160,
			NULL );
	UxPutContext( resourceSettingResRc, (char *) UxResourceSettingContext );

	XtVaSetValues(form33,
			XmNdefaultButton, NULL,
			NULL );


	XtAddCallback( resourceSetting, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxResourceSettingContext);


	return ( resourceSetting );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_resourceSetting( swidget _UxUxParent )
{
	Widget                  rtrn;
	_UxCresourceSetting     *UxContext;

	UxResourceSettingContext = UxContext =
		(_UxCresourceSetting *) UxNewContext( sizeof(_UxCresourceSetting), False );

	UxParent = _UxUxParent;

	{
		n_entries = 0;
		rtrn = _Uxbuild_resourceSetting();

		caricaDatiRisorse (rtrn);
		displayRisorse (rtrn);
		UxPopupInterface (rtrn, no_grab);
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

