
/*******************************************************************************
       malfConfig.c
       (Generated from interface file malfConfig.i)
*******************************************************************************/

#include <stdio.h>

#ifdef MOTIF
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#endif /* MOTIF */

#include "UxXt.h"
#include <Xm/Separator.h>
#include <Xm/Label.h>
#include <Xm/Text.h>
#include <Xm/CascadeB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

#include "bistrutt.h"
#include "malfunzioni.h"
#include "tabelle_malf.h"
#include "file_selection.h"
#include "messaggi.h"

extern M_COMPONENT* m_component;
extern M_COMP_TYPE *m_comp_type;
extern MALF_SET *malf_set;

extern M_COMP_TYPE *fr_comp_type;
extern MALF_SET *frem_set;
extern M_COMPONENT *fr_component;


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
	Widget	UxmalfConfig;
	Widget	Uxframe30;
	Widget	Uxform56;
	Widget	UxapplyMalfConfigpb;
	Widget	Uxmenu10;
	Widget	UxmalfConfigFile;
	Widget	UxmalfConfigFileQuitpb;
	Widget	Uxmenu10_top_b3;
	Widget	Uxform58;
	Widget	UxsecVar;
	Widget	UxsecVarRc;
	Widget	Uxmenu11;
	Widget	UxmalfConfigPopupQuit;
	Widget	UxmalfParam;
	Widget	UxdurationTf;
	Widget	UxrampTf;
	Widget	Uxlabel26;
	Widget	Uxlabel27;
	Widget	Uxlabel28;
	Widget	Uxseparator4;
	Widget	UxdelayTf;
	float	Uxdelay;
	float	Uxduration;
	float	Uxramp;
	float	UxsecVal1;
	float	UxsecVal2;
	float	UxsecVal3;
	float	UxsecVal4;
	XtIntervalId	Uxtimer_malfConfig;
	int	UxdelayModified;
	int	UxrampModified;
	int	UxdurataModified;
	int	UxvalSec1Modified;
	int	UxvalSec2Modified;
	int	UxvalSec3Modified;
	int	UxvalSec4Modified;
	unsigned int	Uxsfondo;
	unsigned int	UxcolorTesto;
	Widget	UxSecVar1;
	Widget	UxSecVar2;
	Widget	UxSecVar3;
	Widget	UxSecVar4;
	Widget	UxsecVar1Label;
	Widget	UxsecVar2Label;
	Widget	UxsecVar3Label;
	Widget	UxsecVar4Label;
	Widget	UxsecVar1Descr;
	Widget	UxsecVar2Descr;
	Widget	UxsecVar3Descr;
	Widget	UxsecVar4Descr;
	Widget	UxsecVar1ValTf;
	Widget	UxsecVar2ValTf;
	Widget	UxsecVar3ValTf;
	Widget	UxsecVar4ValTf;
	Widget	UxsecVar1Tb;
	Widget	UxsecVar2Tb;
	Widget	UxsecVar3Tb;
	Widget	UxsecVar4Tb;
	Widget	UxsecVar1Mod;
	Widget	UxsecVar2Mod;
	Widget	UxsecVar3Mod;
	Widget	UxsecVar4Mod;
	Widget	UxsecVar1ValActLab;
	Widget	UxsecVar2ValActLab;
	Widget	UxsecVar3ValActLab;
	Widget	UxsecVar4ValActLab;
	Widget	UxsecVar1ValAct;
	Widget	UxsecVar2ValAct;
	Widget	UxsecVar3ValAct;
	Widget	UxsecVar4ValAct;
	int	UxpertToggle1;
	int	UxpertToggle2;
	int	UxpertToggle3;
	int	UxpertToggle4;
	int	UxvarSecExist1;
	int	UxvarSecExist2;
	int	UxvarSecExist3;
	int	UxvarSecExist4;
	float	UxvalPrec1;
	float	UxvalPrec2;
	float	UxvalPrec3;
	float	UxvalPrec4;
	swidget	UxUxParent;
	int	Uxindx;
	int	Uxtipo;
} _UxCmalfConfig;

static _UxCmalfConfig          *UxMalfConfigContext;
#define malfConfig              UxMalfConfigContext->UxmalfConfig
#define frame30                 UxMalfConfigContext->Uxframe30
#define form56                  UxMalfConfigContext->Uxform56
#define applyMalfConfigpb       UxMalfConfigContext->UxapplyMalfConfigpb
#define menu10                  UxMalfConfigContext->Uxmenu10
#define malfConfigFile          UxMalfConfigContext->UxmalfConfigFile
#define malfConfigFileQuitpb    UxMalfConfigContext->UxmalfConfigFileQuitpb
#define menu10_top_b3           UxMalfConfigContext->Uxmenu10_top_b3
#define form58                  UxMalfConfigContext->Uxform58
#define secVar                  UxMalfConfigContext->UxsecVar
#define secVarRc                UxMalfConfigContext->UxsecVarRc
#define menu11                  UxMalfConfigContext->Uxmenu11
#define malfConfigPopupQuit     UxMalfConfigContext->UxmalfConfigPopupQuit
#define malfParam               UxMalfConfigContext->UxmalfParam
#define durationTf              UxMalfConfigContext->UxdurationTf
#define rampTf                  UxMalfConfigContext->UxrampTf
#define label26                 UxMalfConfigContext->Uxlabel26
#define label27                 UxMalfConfigContext->Uxlabel27
#define label28                 UxMalfConfigContext->Uxlabel28
#define separator4              UxMalfConfigContext->Uxseparator4
#define delayTf                 UxMalfConfigContext->UxdelayTf
#define delay                   UxMalfConfigContext->Uxdelay
#define duration                UxMalfConfigContext->Uxduration
#define ramp                    UxMalfConfigContext->Uxramp
#define secVal1                 UxMalfConfigContext->UxsecVal1
#define secVal2                 UxMalfConfigContext->UxsecVal2
#define secVal3                 UxMalfConfigContext->UxsecVal3
#define secVal4                 UxMalfConfigContext->UxsecVal4
#define timer_malfConfig        UxMalfConfigContext->Uxtimer_malfConfig
#define delayModified           UxMalfConfigContext->UxdelayModified
#define rampModified            UxMalfConfigContext->UxrampModified
#define durataModified          UxMalfConfigContext->UxdurataModified
#define valSec1Modified         UxMalfConfigContext->UxvalSec1Modified
#define valSec2Modified         UxMalfConfigContext->UxvalSec2Modified
#define valSec3Modified         UxMalfConfigContext->UxvalSec3Modified
#define valSec4Modified         UxMalfConfigContext->UxvalSec4Modified
#define sfondo                  UxMalfConfigContext->Uxsfondo
#define colorTesto              UxMalfConfigContext->UxcolorTesto
#define SecVar1                 UxMalfConfigContext->UxSecVar1
#define SecVar2                 UxMalfConfigContext->UxSecVar2
#define SecVar3                 UxMalfConfigContext->UxSecVar3
#define SecVar4                 UxMalfConfigContext->UxSecVar4
#define secVar1Label            UxMalfConfigContext->UxsecVar1Label
#define secVar2Label            UxMalfConfigContext->UxsecVar2Label
#define secVar3Label            UxMalfConfigContext->UxsecVar3Label
#define secVar4Label            UxMalfConfigContext->UxsecVar4Label
#define secVar1Descr            UxMalfConfigContext->UxsecVar1Descr
#define secVar2Descr            UxMalfConfigContext->UxsecVar2Descr
#define secVar3Descr            UxMalfConfigContext->UxsecVar3Descr
#define secVar4Descr            UxMalfConfigContext->UxsecVar4Descr
#define secVar1ValTf            UxMalfConfigContext->UxsecVar1ValTf
#define secVar2ValTf            UxMalfConfigContext->UxsecVar2ValTf
#define secVar3ValTf            UxMalfConfigContext->UxsecVar3ValTf
#define secVar4ValTf            UxMalfConfigContext->UxsecVar4ValTf
#define secVar1Tb               UxMalfConfigContext->UxsecVar1Tb
#define secVar2Tb               UxMalfConfigContext->UxsecVar2Tb
#define secVar3Tb               UxMalfConfigContext->UxsecVar3Tb
#define secVar4Tb               UxMalfConfigContext->UxsecVar4Tb
#define secVar1Mod              UxMalfConfigContext->UxsecVar1Mod
#define secVar2Mod              UxMalfConfigContext->UxsecVar2Mod
#define secVar3Mod              UxMalfConfigContext->UxsecVar3Mod
#define secVar4Mod              UxMalfConfigContext->UxsecVar4Mod
#define secVar1ValActLab        UxMalfConfigContext->UxsecVar1ValActLab
#define secVar2ValActLab        UxMalfConfigContext->UxsecVar2ValActLab
#define secVar3ValActLab        UxMalfConfigContext->UxsecVar3ValActLab
#define secVar4ValActLab        UxMalfConfigContext->UxsecVar4ValActLab
#define secVar1ValAct           UxMalfConfigContext->UxsecVar1ValAct
#define secVar2ValAct           UxMalfConfigContext->UxsecVar2ValAct
#define secVar3ValAct           UxMalfConfigContext->UxsecVar3ValAct
#define secVar4ValAct           UxMalfConfigContext->UxsecVar4ValAct
#define pertToggle1             UxMalfConfigContext->UxpertToggle1
#define pertToggle2             UxMalfConfigContext->UxpertToggle2
#define pertToggle3             UxMalfConfigContext->UxpertToggle3
#define pertToggle4             UxMalfConfigContext->UxpertToggle4
#define varSecExist1            UxMalfConfigContext->UxvarSecExist1
#define varSecExist2            UxMalfConfigContext->UxvarSecExist2
#define varSecExist3            UxMalfConfigContext->UxvarSecExist3
#define varSecExist4            UxMalfConfigContext->UxvarSecExist4
#define valPrec1                UxMalfConfigContext->UxvalPrec1
#define valPrec2                UxMalfConfigContext->UxvalPrec2
#define valPrec3                UxMalfConfigContext->UxvalPrec3
#define valPrec4                UxMalfConfigContext->UxvalPrec4
#define UxParent                UxMalfConfigContext->UxUxParent
#define indx                    UxMalfConfigContext->Uxindx
#define tipo                    UxMalfConfigContext->Uxtipo

int uscitaMalfConfig (Widget );
int DistruggiInterfaccia (Widget );
int reset_MoreDataFlag (Widget , int );
void stop_timer (XtIntervalId);
int applyMalfConfig (Widget , Widget );
int read_background_color (Widget);

Widget	create_editPert();
int SD_editpert (int , TIPO_PERT *, int , int );
int richiesta_editPertRate (Widget);
int updateEditPertLabel (Widget);
int displayPert (Widget );
int attiva_timer_editPert (Widget );
int read_background_color (Widget);
int read_foreground_color (Widget);
int creaCampiVarSec (Widget , Widget ,M_COMP_TYPE *, MALF_SET *, M_COMPONENT *);
int displayDatiMalfConfig (Widget , Widget , M_COMP_TYPE *, MALF_SET *, M_COMPONENT *);
int attiva_timer_malfConfig (Widget );

/*******************************************************************************
       The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxmalfConfigMenuPost( wgt, client_data, event, ctd )
	Widget		wgt;
	XtPointer	client_data;
	XEvent		*event;
	int ctd;

{
	Widget	menu = (Widget) client_data;
	int 	which_button;

	XtVaGetValues( menu, XmNwhichButton, &which_button, NULL );

	if ( event->xbutton.button == which_button )
	{
		XmMenuPosition( menu, (XButtonPressedEvent *) event );
		XtManageChild( menu );
	}
}


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_malfConfig();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

int uscitaMalfConfig (Widget w)
{
/*** in destroy callback
   reset_MoreDataFlag (UxParent,indx);
   stop_timer (timer_malfConfig);
**************/
   DistruggiInterfaccia (XtParent(malfConfig));
}
/*****************************************************/
int inModifica (int *flag, Boolean val)
{
   if (val == True)   /* setta la flag */
      *flag = 1;
   else
      *flag = 0;
   return (0);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void  destroyCB_malfConfig( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmalfConfig          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMalfConfigContext;
	UxMalfConfigContext = UxContext =
			(_UxCmalfConfig *) UxGetContext( UxWidget );
	{
	reset_MoreDataFlag (UxParent,indx);
	stop_timer (timer_malfConfig);
	
	}
	UxMalfConfigContext = UxSaveCtx;
}

static void  activateCB_applyMalfConfigpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmalfConfig          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMalfConfigContext;
	UxMalfConfigContext = UxContext =
			(_UxCmalfConfig *) UxGetContext( UxWidget );
	{
	applyMalfConfig (malfConfig,UxParent);
	}
	UxMalfConfigContext = UxSaveCtx;
}

static void  activateCB_malfConfigFileQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmalfConfig          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMalfConfigContext;
	UxMalfConfigContext = UxContext =
			(_UxCmalfConfig *) UxGetContext( UxWidget );
	uscitaMalfConfig (malfConfig);
	UxMalfConfigContext = UxSaveCtx;
}

static void  activateCB_malfConfigPopupQuit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmalfConfig          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMalfConfigContext;
	UxMalfConfigContext = UxContext =
			(_UxCmalfConfig *) UxGetContext( UxWidget );
	{
	uscitaMalfConfig (malfConfig);
	}
	UxMalfConfigContext = UxSaveCtx;
}

static void  gainPrimaryCB_durationTf( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmalfConfig          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMalfConfigContext;
	UxMalfConfigContext = UxContext =
			(_UxCmalfConfig *) UxGetContext( UxWidget );
	{
	durataModified = 1;
	}
	UxMalfConfigContext = UxSaveCtx;
}

static void  gainPrimaryCB_rampTf( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmalfConfig          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMalfConfigContext;
	UxMalfConfigContext = UxContext =
			(_UxCmalfConfig *) UxGetContext( UxWidget );
	{
	rampModified  = 1;
	}
	UxMalfConfigContext = UxSaveCtx;
}

static void  gainPrimaryCB_delayTf( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmalfConfig          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMalfConfigContext;
	UxMalfConfigContext = UxContext =
			(_UxCmalfConfig *) UxGetContext( UxWidget );
	{
	delayModified = 1;
	
	}
	UxMalfConfigContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_malfConfig()
{
	Widget		_UxParent;
	Widget		malfConfigFile_shell;
	Widget		menu11_shell;
	char		*UxTmp0;


	/* Creation of malfConfig */
	_UxParent = UxParent;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "malfConfig_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 500,
			XmNy, 170,
			XmNwidth, 260,
			XmNheight, 480,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "malfConfig",
			NULL );

	malfConfig = XtVaCreateWidget( "malfConfig",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 260,
			XmNheight, 480,
			XmNnoResize, TRUE,
			NULL );
	XtAddCallback( malfConfig, XmNdestroyCallback,
		(XtCallbackProc) destroyCB_malfConfig,
		(XtPointer) UxMalfConfigContext );

	UxPutContext( malfConfig, (char *) UxMalfConfigContext );


	/* Creation of frame30 */
	frame30 = XtVaCreateManagedWidget( "frame30",
			xmFrameWidgetClass,
			malfConfig,
			XmNx, -267,
			XmNy, 140,
			XmNwidth, 795,
			XmNheight, 50,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame30, (char *) UxMalfConfigContext );


	/* Creation of form56 */
	form56 = XtVaCreateManagedWidget( "form56",
			xmFormWidgetClass,
			frame30,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 745,
			XmNheight, 72,
			NULL );
	UxPutContext( form56, (char *) UxMalfConfigContext );

	UxTmp0 = APPLYLABEL ? APPLYLABEL : "OK";

	/* Creation of applyMalfConfigpb */
	applyMalfConfigpb = XtVaCreateManagedWidget( "applyMalfConfigpb",
			xmPushButtonWidgetClass,
			form56,
			XmNx, 100,
			XmNy, 20,
			XmNwidth, 130,
			XmNheight, 40,
			RES_CONVERT( XmNlabelString, UxTmp0 ),
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
	XtAddCallback( applyMalfConfigpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_applyMalfConfigpb,
		(XtPointer) UxMalfConfigContext );

	UxPutContext( applyMalfConfigpb, (char *) UxMalfConfigContext );


	/* Creation of menu10 */
	menu10 = XtVaCreateManagedWidget( "menu10",
			xmRowColumnWidgetClass,
			malfConfig,
			XmNrowColumnType, XmMENU_BAR,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNmenuAccelerator, "<KeyUp>F10",
			NULL );
	UxPutContext( menu10, (char *) UxMalfConfigContext );


	/* Creation of malfConfigFile */
	malfConfigFile_shell = XtVaCreatePopupShell ("malfConfigFile_shell",
			xmMenuShellWidgetClass, menu10,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	malfConfigFile = XtVaCreateWidget( "malfConfigFile",
			xmRowColumnWidgetClass,
			malfConfigFile_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( malfConfigFile, (char *) UxMalfConfigContext );


	/* Creation of malfConfigFileQuitpb */
	malfConfigFileQuitpb = XtVaCreateManagedWidget( "malfConfigFileQuitpb",
			xmPushButtonWidgetClass,
			malfConfigFile,
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );
	XtAddCallback( malfConfigFileQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_malfConfigFileQuitpb,
		(XtPointer) UxMalfConfigContext );

	UxPutContext( malfConfigFileQuitpb, (char *) UxMalfConfigContext );


	/* Creation of menu10_top_b3 */
	menu10_top_b3 = XtVaCreateManagedWidget( "menu10_top_b3",
			xmCascadeButtonWidgetClass,
			menu10,
			RES_CONVERT( XmNlabelString, "File" ),
			XmNsubMenuId, malfConfigFile,
			NULL );
	UxPutContext( menu10_top_b3, (char *) UxMalfConfigContext );


	/* Creation of form58 */
	form58 = XtVaCreateManagedWidget( "form58",
			xmFormWidgetClass,
			malfConfig,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 10,
			XmNwidth, 250,
			XmNheight, 370,
			XmNtopOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, frame30,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, menu10,
			NULL );
	UxPutContext( form58, (char *) UxMalfConfigContext );


	/* Creation of secVar */
	secVar = XtVaCreateManagedWidget( "secVar",
			xmFormWidgetClass,
			form58,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 260,
			XmNheight, 320,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_NONE,
			XmNtopOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( secVar, (char *) UxMalfConfigContext );


	/* Creation of secVarRc */
	secVarRc = XtVaCreateManagedWidget( "secVarRc",
			xmRowColumnWidgetClass,
			secVar,
			XmNx, 10,
			XmNy, 20,
			XmNwidth, 200,
			XmNheight, 200,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( secVarRc, (char *) UxMalfConfigContext );


	/* Creation of menu11 */
	menu11_shell = XtVaCreatePopupShell ("menu11_shell",
			xmMenuShellWidgetClass, form58,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu11 = XtVaCreateWidget( "menu11",
			xmRowColumnWidgetClass,
			menu11_shell,
			XmNrowColumnType, XmMENU_POPUP,
			NULL );
	UxPutContext( menu11, (char *) UxMalfConfigContext );


	/* Creation of malfConfigPopupQuit */
	malfConfigPopupQuit = XtVaCreateManagedWidget( "malfConfigPopupQuit",
			xmPushButtonWidgetClass,
			menu11,
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );
	XtAddCallback( malfConfigPopupQuit, XmNactivateCallback,
		(XtCallbackProc) activateCB_malfConfigPopupQuit,
		(XtPointer) UxMalfConfigContext );

	UxPutContext( malfConfigPopupQuit, (char *) UxMalfConfigContext );


	/* Creation of malfParam */
	malfParam = XtVaCreateManagedWidget( "malfParam",
			xmFormWidgetClass,
			form58,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 10,
			XmNwidth, 290,
			XmNheight, 120,
			XmNnoResize, TRUE,
			XmNtopOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, secVar,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( malfParam, (char *) UxMalfConfigContext );


	/* Creation of durationTf */
	durationTf = XtVaCreateManagedWidget( "durationTf",
			xmTextWidgetClass,
			malfParam,
			XmNx, 160,
			XmNy, 40,
			XmNwidth, 90,
			XmNheight, 25,
			XmNmarginHeight, 2,
			XmNfontList, UxConvertFontList("-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-8" ),
			NULL );
	XtAddCallback( durationTf, XmNgainPrimaryCallback,
		(XtCallbackProc) gainPrimaryCB_durationTf,
		(XtPointer) UxMalfConfigContext );

	UxPutContext( durationTf, (char *) UxMalfConfigContext );


	/* Creation of rampTf */
	rampTf = XtVaCreateWidget( "rampTf",
			xmTextWidgetClass,
			malfParam,
			XmNx, 160,
			XmNy, 70,
			XmNwidth, 90,
			XmNheight, 25,
			XmNmarginHeight, 2,
			XmNfontList, UxConvertFontList("-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-8" ),
			NULL );
	XtAddCallback( rampTf, XmNgainPrimaryCallback,
		(XtCallbackProc) gainPrimaryCB_rampTf,
		(XtPointer) UxMalfConfigContext );

	UxPutContext( rampTf, (char *) UxMalfConfigContext );


	/* Creation of label26 */
	label26 = XtVaCreateManagedWidget( "label26",
			xmLabelWidgetClass,
			malfParam,
			XmNx, 10,
			XmNy, 10,
			XmNwidth, 130,
			XmNheight, 25,
			XmNborderWidth, 0,
			RES_CONVERT( XmNlabelString, DELAY_LABEL ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList("-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-8" ),
			NULL );
	UxPutContext( label26, (char *) UxMalfConfigContext );


	/* Creation of label27 */
	label27 = XtVaCreateManagedWidget( "label27",
			xmLabelWidgetClass,
			malfParam,
			XmNx, 10,
			XmNy, 40,
			XmNwidth, 130,
			XmNheight, 25,
			XmNborderWidth, 0,
			RES_CONVERT( XmNlabelString, DURATION_LABEL ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList("-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-8" ),
			NULL );
	UxPutContext( label27, (char *) UxMalfConfigContext );


	/* Creation of label28 */
	label28 = XtVaCreateWidget( "label28",
			xmLabelWidgetClass,
			malfParam,
			XmNx, 10,
			XmNy, 70,
			XmNwidth, 130,
			XmNheight, 25,
			XmNborderWidth, 0,
			RES_CONVERT( XmNlabelString, RAMP_LABEL ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList("-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-8" ),
			NULL );
	UxPutContext( label28, (char *) UxMalfConfigContext );


	/* Creation of separator4 */
	separator4 = XtVaCreateManagedWidget( "separator4",
			xmSeparatorWidgetClass,
			malfParam,
			XmNx, 10,
			XmNy, 120,
			XmNwidth, 240,
			XmNheight, 10,
			XmNtopOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_NONE,
			NULL );
	UxPutContext( separator4, (char *) UxMalfConfigContext );


	/* Creation of delayTf */
	delayTf = XtVaCreateManagedWidget( "delayTf",
			xmTextWidgetClass,
			malfParam,
			XmNx, 160,
			XmNy, 10,
			XmNwidth, 90,
			XmNheight, 25,
			XmNmarginHeight, 2,
			XmNfontList, UxConvertFontList("-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-8" ),
			NULL );
	XtAddCallback( delayTf, XmNgainPrimaryCallback,
		(XtCallbackProc) gainPrimaryCB_delayTf,
		(XtPointer) UxMalfConfigContext );

	UxPutContext( delayTf, (char *) UxMalfConfigContext );

	XtVaSetValues(form56,
			XmNdefaultButton, NULL,
			NULL );

	XtVaSetValues(form58,
			XmNdefaultButton, applyMalfConfigpb,
			NULL );


	XtAddCallback( malfConfig, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxMalfConfigContext);

	XtAddEventHandler(form58, ButtonPressMask,
			False, (XtEventHandler) _UxmalfConfigMenuPost, (XtPointer) menu11 );

	return ( malfConfig );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_malfConfig( _UxUxParent, _Uxindx, _Uxtipo )
	swidget	_UxUxParent;
	int	_Uxindx;
	int	_Uxtipo;
{
	Widget                  rtrn;
	_UxCmalfConfig          *UxContext;
	static int		_Uxinit = 0;

	UxMalfConfigContext = UxContext =
		(_UxCmalfConfig *) UxNewContext( sizeof(_UxCmalfConfig), False );

	UxParent = _UxUxParent;
	indx = _Uxindx;
	tipo = _Uxtipo;

	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewInterfaceClassId();
		_Uxinit = 1;
	}

	{
		printf ("Richiesto  su indice %d\n",indx);
		delayModified  = 0; 
		rampModified = 0;
		durataModified = 0;
		
		valSec1Modified = 0;
		valSec2Modified = 0;
		valSec3Modified = 0;
		valSec4Modified = 0;
		
		pertToggle1 = 0;
		pertToggle2 = 0;
		pertToggle3 = 0;
		pertToggle4 = 0;
		
		varSecExist1 = -1;
		varSecExist2 = -1;
		varSecExist3 = -1;
		varSecExist4 = -1;
		
		valPrec1 = -999.999;
		valPrec2 = -999.999;
		valPrec3 = -999.999;
		valPrec4 = -999.999;
		rtrn = _Uxbuild_malfConfig();
		UxPutClassCode( malfConfig, _UxIfClassId );

		/* colore di reset per le deselezioni */
		sfondo = read_background_color (rtrn);
		/* colore di set per tag di modifica */
		colorTesto = read_foreground_color (delayTf);
		
		switch (tipo)
		   {
		   case MALFUNZIONI:
		   case SOMMARIO_MALF:
		      creaCampiVarSec (rtrn, UxParent, m_comp_type, malf_set, m_component);
		      displayDatiMalfConfig (rtrn, UxParent, m_comp_type, malf_set, m_component);
		      break;
		   case FUNZIONI_REMOTE:
		   case SOMMARIO_FREM:
		      creaCampiVarSec (rtrn, UxParent, fr_comp_type, frem_set, fr_component);
		      displayDatiMalfConfig (rtrn, UxParent, fr_comp_type, frem_set, fr_component);
		      break;
		   }
		
		attiva_timer_malfConfig (rtrn);
		UxPopupInterface (rtrn, no_grab);
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

