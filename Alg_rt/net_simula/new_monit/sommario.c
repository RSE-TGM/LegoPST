
/*******************************************************************************
       sommario.c
       (Generated from interface file sommario.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/ScrolledW.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <X11/Intrinsic.h>
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "malfunzioni.h"
#include "messaggi.h"

#include "cursore.h"
#ifndef DESIGN_TIME
/* #include "interf_attive.h" */
extern int sommario_malf_active;
extern int sommario_frem_active;

Widget sommarioMalf;
Widget sommarioFrem;
#endif
#include "bistrutt.h"
#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"

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
	Widget	UxsommMenu;
	Widget	UxsommMenuFile;
	Widget	UxsommMenuFileQuitpb;
	Widget	UxsommMenu_top_b1;
	Widget	UxsommMenuHelp;
	Widget	UxsommMenuHelppb;
	Widget	UxsommMenu_top_b2;
	Widget	Uxframe17;
	Widget	Uxform33;
	Widget	UxquitMalfSommpb;
	Widget	UxsommarioScrollWin;
	Widget	UxsommRc;
	Widget	UxsommParent;
	int	Uxtipo;
} _UxCsommario;

static _UxCsommario            *UxSommarioContext;
#define sommMenu                UxSommarioContext->UxsommMenu
#define sommMenuFile            UxSommarioContext->UxsommMenuFile
#define sommMenuFileQuitpb      UxSommarioContext->UxsommMenuFileQuitpb
#define sommMenu_top_b1         UxSommarioContext->UxsommMenu_top_b1
#define sommMenuHelp            UxSommarioContext->UxsommMenuHelp
#define sommMenuHelppb          UxSommarioContext->UxsommMenuHelppb
#define sommMenu_top_b2         UxSommarioContext->UxsommMenu_top_b2
#define frame17                 UxSommarioContext->Uxframe17
#define form33                  UxSommarioContext->Uxform33
#define quitMalfSommpb          UxSommarioContext->UxquitMalfSommpb
#define sommarioScrollWin       UxSommarioContext->UxsommarioScrollWin
#define sommRc                  UxSommarioContext->UxsommRc
#define sommParent              UxSommarioContext->UxsommParent
#define tipo                    UxSommarioContext->Uxtipo


Widget	sommario;
Widget	actMalfSommpb;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_sommario();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_sommMenuFileQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsommario            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSommarioContext;
	UxSommarioContext = UxContext =
			(_UxCsommario *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	switch (tipo)
	   {
	   case MALFUNZIONI:
	      DistruggiInterfaccia(XtParent(sommarioMalf));
	      sommario_malf_active = 0;
	      break;
	   case FUNZIONI_REMOTE:
	      DistruggiInterfaccia(XtParent(sommarioFrem));
	      sommario_frem_active = 0;
	      break;
	   }
#endif
	}
	UxSommarioContext = UxSaveCtx;
}

static	void	activateCB_sommMenuHelppb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsommario            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSommarioContext;
	UxSommarioContext = UxContext =
			(_UxCsommario *) UxGetContext( UxWidget );
	{printf ("help comandi\n");}
	UxSommarioContext = UxSaveCtx;
}

static	void	unmapCB_form33( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsommario            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSommarioContext;
	UxSommarioContext = UxContext =
			(_UxCsommario *) UxGetContext( UxWidget );
	{
	
	}
	UxSommarioContext = UxSaveCtx;
}

static	void	activateCB_quitMalfSommpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsommario            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSommarioContext;
	UxSommarioContext = UxContext =
			(_UxCsommario *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	switch (tipo)
	   {
	   case MALFUNZIONI:
	      DistruggiInterfaccia(XtParent(sommarioMalf));
	      sommario_malf_active = 0;
	      break;
	   case FUNZIONI_REMOTE:
	      DistruggiInterfaccia(XtParent(sommarioFrem));
	      sommario_frem_active = 0;
	      break;
	   }
#endif
	}
	UxSommarioContext = UxSaveCtx;
}

static	void	activateCB_actMalfSommpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsommario            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSommarioContext;
	UxSommarioContext = UxContext =
			(_UxCsommario *) UxGetContext( UxWidget );
	{
	
	}
	UxSommarioContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_sommario()
{
	Widget		_UxParent;
	Widget		sommMenuFile_shell;
	Widget		sommMenuHelp_shell;


	/* Creation of sommario */
	_UxParent = XtVaCreatePopupShell( "sommario_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 400,
			XmNy, 80,
			XmNwidth, 500,
			XmNheight, 430,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "sommario",
			NULL );

	sommario = XtVaCreateWidget( "sommario",
			xmFormWidgetClass,
			_UxParent,
			XmNwidth, 500,
			XmNheight, 430,
			XmNunitType, XmPIXELS,
			NULL );
	UxPutContext( sommario, (char *) UxSommarioContext );


	/* Creation of sommMenu */
	sommMenu = XtVaCreateManagedWidget( "sommMenu",
			xmRowColumnWidgetClass,
			sommario,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNpacking, XmPACK_TIGHT,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( sommMenu, (char *) UxSommarioContext );


	/* Creation of sommMenuFile */
	sommMenuFile_shell = XtVaCreatePopupShell ("sommMenuFile_shell",
			xmMenuShellWidgetClass, sommMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	sommMenuFile = XtVaCreateWidget( "sommMenuFile",
			xmRowColumnWidgetClass,
			sommMenuFile_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( sommMenuFile, (char *) UxSommarioContext );


	/* Creation of sommMenuFileQuitpb */
	sommMenuFileQuitpb = XtVaCreateManagedWidget( "sommMenuFileQuitpb",
			xmPushButtonWidgetClass,
			sommMenuFile,
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );
	XtAddCallback( sommMenuFileQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_sommMenuFileQuitpb,
		(XtPointer) UxSommarioContext );

	UxPutContext( sommMenuFileQuitpb, (char *) UxSommarioContext );


	/* Creation of sommMenu_top_b1 */
	sommMenu_top_b1 = XtVaCreateManagedWidget( "sommMenu_top_b1",
			xmCascadeButtonWidgetClass,
			sommMenu,
			RES_CONVERT( XmNlabelString, "File    " ),
			XmNsubMenuId, sommMenuFile,
			NULL );
	UxPutContext( sommMenu_top_b1, (char *) UxSommarioContext );


	/* Creation of sommMenuHelp */
	sommMenuHelp_shell = XtVaCreatePopupShell ("sommMenuHelp_shell",
			xmMenuShellWidgetClass, sommMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	sommMenuHelp = XtVaCreateWidget( "sommMenuHelp",
			xmRowColumnWidgetClass,
			sommMenuHelp_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( sommMenuHelp, (char *) UxSommarioContext );


	/* Creation of sommMenuHelppb */
	sommMenuHelppb = XtVaCreateManagedWidget( "sommMenuHelppb",
			xmPushButtonWidgetClass,
			sommMenuHelp,
			RES_CONVERT( XmNlabelString, "Commands" ),
			RES_CONVERT( XmNmnemonic, "C" ),
			NULL );
	XtAddCallback( sommMenuHelppb, XmNactivateCallback,
		(XtCallbackProc) activateCB_sommMenuHelppb,
		(XtPointer) UxSommarioContext );

	UxPutContext( sommMenuHelppb, (char *) UxSommarioContext );


	/* Creation of sommMenu_top_b2 */
	sommMenu_top_b2 = XtVaCreateManagedWidget( "sommMenu_top_b2",
			xmCascadeButtonWidgetClass,
			sommMenu,
			RES_CONVERT( XmNlabelString, "Help" ),
			XmNsubMenuId, sommMenuHelp,
			NULL );
	UxPutContext( sommMenu_top_b2, (char *) UxSommarioContext );


	/* Creation of frame17 */
	frame17 = XtVaCreateManagedWidget( "frame17",
			xmFrameWidgetClass,
			sommario,
			XmNx, 48,
			XmNy, 288,
			XmNwidth, 690,
			XmNheight, 80,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame17, (char *) UxSommarioContext );


	/* Creation of form33 */
	form33 = XtVaCreateManagedWidget( "form33",
			xmFormWidgetClass,
			frame17,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 2,
			XmNy, 20,
			XmNwidth, 851,
			XmNheight, 82,
			NULL );
	XtAddCallback( form33, XmNunmapCallback,
		(XtCallbackProc) unmapCB_form33,
		(XtPointer) UxSommarioContext );

	UxPutContext( form33, (char *) UxSommarioContext );


	/* Creation of quitMalfSommpb */
	quitMalfSommpb = XtVaCreateManagedWidget( "quitMalfSommpb",
			xmPushButtonWidgetClass,
			form33,
			XmNx, 715,
			XmNy, 15,
			XmNwidth, 200,
			XmNheight, 52,
			RES_CONVERT( XmNlabelString, "QUIT" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_SELF,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNbottomOffset, 15,
			XmNleftPosition, 0,
			XmNleftOffset, 120,
			NULL );
	XtAddCallback( quitMalfSommpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_quitMalfSommpb,
		(XtPointer) 0x0 );

	UxPutContext( quitMalfSommpb, (char *) UxSommarioContext );


	/* Creation of actMalfSommpb */
	actMalfSommpb = XtVaCreateManagedWidget( "actMalfSommpb",
			xmPushButtonWidgetClass,
			form33,
			XmNx, 20,
			XmNy, 15,
			XmNwidth, 200,
			XmNheight, 52,
			RES_CONVERT( XmNlabelString, "DEACTIVATE" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNrightAttachment, XmATTACH_SELF,
			XmNrightPosition, 0,
			XmNrightOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( actMalfSommpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_actMalfSommpb,
		(XtPointer) 0x0 );

	UxPutContext( actMalfSommpb, (char *) UxSommarioContext );


	/* Creation of sommarioScrollWin */
	sommarioScrollWin = XtVaCreateManagedWidget( "sommarioScrollWin",
			xmScrolledWindowWidgetClass,
			sommario,
			XmNscrollingPolicy, XmAUTOMATIC,
			XmNx, 15,
			XmNy, 60,
			XmNwidth, 650,
			XmNheight, 255,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 20,
			XmNbottomWidget, frame17,
			XmNleftOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 20,
			XmNtopWidget, sommMenu,
			NULL );
	UxPutContext( sommarioScrollWin, (char *) UxSommarioContext );


	/* Creation of sommRc */
	sommRc = XtVaCreateManagedWidget( "sommRc",
			xmRowColumnWidgetClass,
			sommarioScrollWin,
			XmNx, 2,
			XmNy, 4,
			XmNwidth, 429,
			XmNheight, 275,
			NULL );
	UxPutContext( sommRc, (char *) UxSommarioContext );

	XtVaSetValues(sommMenu,
			XmNmenuHelpWidget, sommMenu_top_b2,
			NULL );

	XtVaSetValues(form33,
			XmNdefaultButton, NULL,
			NULL );


	XtAddCallback( sommario, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxSommarioContext);


	return ( sommario );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_sommario( _UxsommParent, _Uxtipo )
	Widget	_UxsommParent;
	int	_Uxtipo;
{
	Widget                  rtrn;
	_UxCsommario            *UxContext;

	UxSommarioContext = UxContext =
		(_UxCsommario *) UxNewContext( sizeof(_UxCsommario), False );

	sommParent = _UxsommParent;
	tipo = _Uxtipo;

	{
#ifndef DESIGN_TIME
		set_cursor (sommParent,CLOCK);
#endif
		rtrn = _Uxbuild_sommario();

#ifndef DESIGN_TIME
		switch (tipo)
		   {
		   case MALFUNZIONI:
		      sommarioMalf = rtrn;
		      sommario_malf_active = 1;
		      XtVaSetValues (XtParent(UxGetWidget(rtrn)),
		         XmNtitle, MALF_SUMMARY, XmNiconName, MALF_SUMMARY, NULL);
		      break;
		   case FUNZIONI_REMOTE:
		      sommarioFrem = rtrn;
		      sommario_frem_active = 1;
		      XtVaSetValues (XtParent(UxGetWidget(rtrn)),
		         XmNtitle, FREM_SUMMARY, XmNiconName, FREM_SUMMARY, NULL);
		      break; 
		   }
		set_cursor (sommParent,NORMALE);
#endif
		UxPopupInterface(rtrn,no_grab);
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

