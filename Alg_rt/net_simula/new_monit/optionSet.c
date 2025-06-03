
/*******************************************************************************
       optionSet.c
       (Generated from interface file optionSet.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/CascadeB.h>
#include <Xm/Separator.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

extern swidget masterMenu;
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "messaggi.h"


#ifndef DESIGN_TIME
#include "option.h"
extern int opt_interface_active;
extern OPTIONS_FLAGS options;
extern Boolean *IcProt;
#endif
extern int isSuperuser;

#include "bistrutt.h"
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
	Widget	Uxform7;
	Widget	Uxform12;
	Widget	Uxframe12;
	Widget	Uxform21;
	Widget	UxpushButton6;
	Widget	Uxmenu6;
	Widget	UxoptionSetMenu;
	Widget	UxoptionSetMenuSavepb;
	Widget	UxoptionSetMenu_b2;
	Widget	UxoptionSetMenuLoadpb;
	Widget	UxoptionSetMenu_b4;
	Widget	UxoptionSetMenuQuitpb;
	Widget	Uxmenu6_top_b2;
	Widget	Uxframe11;
	Widget	Uxform11;
	Widget	UxoptSelectionMenu;
	Widget	UxoptSelMenuInitpb;
	Widget	UxoptSelMenuSnappb;
	Widget	UxoptSelMenuIcProtpb;
	Widget	UxoptSelMenuRecpb;
	Widget	UxoptSelMenuMffrpb;
	Widget	UxoptSelMenuArchivepb;
	Widget	UxoptSelMenuShowpb;
	Widget	UxoptSelectionMenu_b7;
	Widget	UxoptSelMenuDisplaypb;
	Widget	UxoptSelnMenuProgLaunchpb;
	Widget	UxoptionSelection;
	Widget	Uxframe10;
	Widget	UxoptionSetForm;
} _UxCoptionSet;

static _UxCoptionSet           *UxOptionSetContext;
#define form7                   UxOptionSetContext->Uxform7
#define form12                  UxOptionSetContext->Uxform12
#define frame12                 UxOptionSetContext->Uxframe12
#define form21                  UxOptionSetContext->Uxform21
#define pushButton6             UxOptionSetContext->UxpushButton6
#define menu6                   UxOptionSetContext->Uxmenu6
#define optionSetMenu           UxOptionSetContext->UxoptionSetMenu
#define optionSetMenuSavepb     UxOptionSetContext->UxoptionSetMenuSavepb
#define optionSetMenu_b2        UxOptionSetContext->UxoptionSetMenu_b2
#define optionSetMenuLoadpb     UxOptionSetContext->UxoptionSetMenuLoadpb
#define optionSetMenu_b4        UxOptionSetContext->UxoptionSetMenu_b4
#define optionSetMenuQuitpb     UxOptionSetContext->UxoptionSetMenuQuitpb
#define menu6_top_b2            UxOptionSetContext->Uxmenu6_top_b2
#define frame11                 UxOptionSetContext->Uxframe11
#define form11                  UxOptionSetContext->Uxform11
#define optSelectionMenu        UxOptionSetContext->UxoptSelectionMenu
#define optSelMenuInitpb        UxOptionSetContext->UxoptSelMenuInitpb
#define optSelMenuSnappb        UxOptionSetContext->UxoptSelMenuSnappb
#define optSelMenuIcProtpb      UxOptionSetContext->UxoptSelMenuIcProtpb
#define optSelMenuRecpb         UxOptionSetContext->UxoptSelMenuRecpb
#define optSelMenuMffrpb        UxOptionSetContext->UxoptSelMenuMffrpb
#define optSelMenuArchivepb     UxOptionSetContext->UxoptSelMenuArchivepb
#define optSelMenuShowpb        UxOptionSetContext->UxoptSelMenuShowpb
#define optSelectionMenu_b7     UxOptionSetContext->UxoptSelectionMenu_b7
#define optSelMenuDisplaypb     UxOptionSetContext->UxoptSelMenuDisplaypb
#define optSelnMenuProgLaunchpb UxOptionSetContext->UxoptSelnMenuProgLaunchpb
#define optionSelection         UxOptionSetContext->UxoptionSelection
#define frame10                 UxOptionSetContext->Uxframe10
#define optionSetForm           UxOptionSetContext->UxoptionSetForm


Widget	optionSet;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_optionSet();
int aggiorna_opzioni_show (Widget, OPTIONS_FLAGS *p);
int aggiorna_opzioni(Widget, OPTIONS_FLAGS *p);
//int SD_optsave (int, char*);
int carica_options (Widget, int);



/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

char *check_if_superuser (flag)
int flag;
{
   if (flag)
      return ("true");
   return ("false");
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_pushButton6( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCoptionSet           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOptionSetContext;
	UxOptionSetContext = UxContext =
			(_UxCoptionSet *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	aggiorna_opzioni (UxWidget, &options);
#endif
	}
	UxOptionSetContext = UxSaveCtx;
}

static	void	activateCB_optionSetMenuSavepb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCoptionSet           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOptionSetContext;
	UxOptionSetContext = UxContext =
			(_UxCoptionSet *) UxGetContext( UxWidget );
	{
#ifndef  DESIGN_TIME
	if (SD_optsave3 (BANCO, (char*)&options,IcProt) > 0)
	   fprintf (stderr,"OPZIONI salvate\n");
	else
	   fprintf (stderr,"*** errore save opzioni\n");
#endif
	}
	UxOptionSetContext = UxSaveCtx;
}

static	void	activateCB_optionSetMenuLoadpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCoptionSet           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOptionSetContext;
	UxOptionSetContext = UxContext =
			(_UxCoptionSet *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME 
	printf ("load Options\n");
	read_options();
	opt_interface_active = 0;
	DistruggiInterfaccia (optionSet);
	aggiorna_opzioni (UxWidget, &options);
#endif
	}
	UxOptionSetContext = UxSaveCtx;
}

static	void	activateCB_optionSetMenuQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCoptionSet           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOptionSetContext;
	UxOptionSetContext = UxContext =
			(_UxCoptionSet *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	opt_interface_active = 0;  /* reset memoria interfaccia attiva */
	DistruggiInterfaccia ((Widget)UxClientData);
#endif
	}
	UxOptionSetContext = UxSaveCtx;
}

static	void	activateCB_optSelMenuInitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCoptionSet           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOptionSetContext;
	UxOptionSetContext = UxContext =
			(_UxCoptionSet *) UxGetContext( UxWidget );
	{
	printf ("richiesta init \n");
#ifndef DESIGN_TIME
	carica_options (UxGetWidget(optionSetForm),OPTION_INIT);
#endif
	}
	UxOptionSetContext = UxSaveCtx;
}

static	void	activateCB_optSelMenuSnappb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCoptionSet           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOptionSetContext;
	UxOptionSetContext = UxContext =
			(_UxCoptionSet *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	carica_options (UxGetWidget(optionSetForm),OPTION_SNAPSHOT);
#endif
	}
	UxOptionSetContext = UxSaveCtx;
}

static	void	activateCB_optSelMenuIcProtpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCoptionSet           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOptionSetContext;
	UxOptionSetContext = UxContext =
			(_UxCoptionSet *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   carica_options (UxGetWidget(optionSetForm),OPTION_ICPROTECT);
#endif
	}
	UxOptionSetContext = UxSaveCtx;
}

static	void	activateCB_optSelMenuRecpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCoptionSet           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOptionSetContext;
	UxOptionSetContext = UxContext =
			(_UxCoptionSet *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	carica_options (UxGetWidget(optionSetForm),OPTION_RECORDING);
#endif
	}
	UxOptionSetContext = UxSaveCtx;
}

static	void	activateCB_optSelMenuMffrpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCoptionSet           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOptionSetContext;
	UxOptionSetContext = UxContext =
			(_UxCoptionSet *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	carica_options (UxGetWidget(optionSetForm),OPTION_MFFR);
#endif
	}
	UxOptionSetContext = UxSaveCtx;
}

static	void	activateCB_optSelMenuArchivepb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCoptionSet           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOptionSetContext;
	UxOptionSetContext = UxContext =
			(_UxCoptionSet *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	carica_options (UxGetWidget(optionSetForm),OPTION_ARCHIVE);
#endif
	}
	UxOptionSetContext = UxSaveCtx;
}

static	void	activateCB_optSelMenuShowpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCoptionSet           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOptionSetContext;
	UxOptionSetContext = UxContext =
			(_UxCoptionSet *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	carica_options (UxGetWidget(optionSetForm),OPTION_SHOW);
#endif
	}
	UxOptionSetContext = UxSaveCtx;
}

static	void	activateCB_optSelMenuDisplaypb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCoptionSet           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOptionSetContext;
	UxOptionSetContext = UxContext =
			(_UxCoptionSet *) UxGetContext( UxWidget );
	{
	   printf ("richiesta display \n");
#ifndef DESIGN_TIME
	   carica_options (UxGetWidget(optionSetForm),OPTION_DISPLAY);
#endif
	}
	UxOptionSetContext = UxSaveCtx;
}

static	void	activateCB_optSelnMenuProgLaunchpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCoptionSet           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOptionSetContext;
	UxOptionSetContext = UxContext =
			(_UxCoptionSet *) UxGetContext( UxWidget );
	{
	printf ("user programs\n");
#ifndef DESIGN_TIME
	   carica_options (UxGetWidget(optionSetForm),OPTION_USERPROG);
#endif
	}
	UxOptionSetContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_optionSet()
{
	Widget		optionSetMenu_shell;
	Widget		optSelectionMenu_shell;
	char		*UxTmp0;


	/* Creation of optionSet */
	optionSet = XtVaCreatePopupShell( "optionSet",
			applicationShellWidgetClass,
			UxTopLevel,
			XmNx, 335,
			XmNy, 595,
			XmNwidth, 320,
			XmNheight, 400,
			NULL );
	UxPutContext( optionSet, (char *) UxOptionSetContext );


	/* Creation of form7 */
	form7 = XtVaCreateManagedWidget( "form7",
			xmFormWidgetClass,
			optionSet,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNunitType, XmPIXELS,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 320,
			XmNheight, 215,
			NULL );
	UxPutContext( form7, (char *) UxOptionSetContext );


	/* Creation of form12 */
	form12 = XtVaCreateManagedWidget( "form12",
			xmFormWidgetClass,
			form7,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 16,
			XmNy, 108,
			XmNwidth, 750,
			XmNheight, 75,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_NONE,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopWidget, NULL,
			NULL );
	UxPutContext( form12, (char *) UxOptionSetContext );


	/* Creation of frame12 */
	frame12 = XtVaCreateManagedWidget( "frame12",
			xmFrameWidgetClass,
			form12,
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
	UxPutContext( frame12, (char *) UxOptionSetContext );


	/* Creation of form21 */
	form21 = XtVaCreateManagedWidget( "form21",
			xmFormWidgetClass,
			frame12,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 745,
			XmNheight, 72,
			NULL );
	UxPutContext( form21, (char *) UxOptionSetContext );

	UxTmp0 = OPTION_APPLY_LABEL ? OPTION_APPLY_LABEL : "OK";

	/* Creation of pushButton6 */
	pushButton6 = XtVaCreateManagedWidget( "pushButton6",
			xmPushButtonWidgetClass,
			form21,
			XmNx, 100,
			XmNy, 20,
			XmNwidth, 130,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, UxTmp0 ),
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftPosition, 30,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 70,
			NULL );
	XtAddCallback( pushButton6, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton6,
		(XtPointer) UxOptionSetContext );

	UxPutContext( pushButton6, (char *) UxOptionSetContext );


	/* Creation of menu6 */
	menu6 = XtVaCreateManagedWidget( "menu6",
			xmRowColumnWidgetClass,
			form7,
			XmNrowColumnType, XmMENU_BAR,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNmenuAccelerator, "<KeyUp>F10",
			NULL );
	UxPutContext( menu6, (char *) UxOptionSetContext );


	/* Creation of optionSetMenu */
	optionSetMenu_shell = XtVaCreatePopupShell ("optionSetMenu_shell",
			xmMenuShellWidgetClass, menu6,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	optionSetMenu = XtVaCreateWidget( "optionSetMenu",
			xmRowColumnWidgetClass,
			optionSetMenu_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( optionSetMenu, (char *) UxOptionSetContext );


	/* Creation of optionSetMenuSavepb */
	optionSetMenuSavepb = XtVaCreateManagedWidget( "optionSetMenuSavepb",
			xmPushButtonWidgetClass,
			optionSetMenu,
			RES_CONVERT( XmNlabelString, "Save " ),
			NULL );
	XtAddCallback( optionSetMenuSavepb, XmNactivateCallback,
		(XtCallbackProc) activateCB_optionSetMenuSavepb,
		(XtPointer) UxOptionSetContext );

	UxPutContext( optionSetMenuSavepb, (char *) UxOptionSetContext );


	/* Creation of optionSetMenu_b2 */
	optionSetMenu_b2 = XtVaCreateManagedWidget( "optionSetMenu_b2",
			xmSeparatorWidgetClass,
			optionSetMenu,
			NULL );
	UxPutContext( optionSetMenu_b2, (char *) UxOptionSetContext );


	/* Creation of optionSetMenuLoadpb */
	optionSetMenuLoadpb = XtVaCreateManagedWidget( "optionSetMenuLoadpb",
			xmPushButtonWidgetClass,
			optionSetMenu,
			RES_CONVERT( XmNlabelString, "Load " ),
			NULL );
	XtAddCallback( optionSetMenuLoadpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_optionSetMenuLoadpb,
		(XtPointer) UxOptionSetContext );

	UxPutContext( optionSetMenuLoadpb, (char *) UxOptionSetContext );


	/* Creation of optionSetMenu_b4 */
	optionSetMenu_b4 = XtVaCreateManagedWidget( "optionSetMenu_b4",
			xmSeparatorWidgetClass,
			optionSetMenu,
			NULL );
	UxPutContext( optionSetMenu_b4, (char *) UxOptionSetContext );


	/* Creation of optionSetMenuQuitpb */
	optionSetMenuQuitpb = XtVaCreateManagedWidget( "optionSetMenuQuitpb",
			xmPushButtonWidgetClass,
			optionSetMenu,
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );
	XtAddCallback( optionSetMenuQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_optionSetMenuQuitpb,
		UxGetWidget(optionSet) );

	UxPutContext( optionSetMenuQuitpb, (char *) UxOptionSetContext );


	/* Creation of menu6_top_b2 */
	menu6_top_b2 = XtVaCreateManagedWidget( "menu6_top_b2",
			xmCascadeButtonWidgetClass,
			menu6,
			RES_CONVERT( XmNlabelString, "File" ),
			XmNsubMenuId, optionSetMenu,
			NULL );
	UxPutContext( menu6_top_b2, (char *) UxOptionSetContext );


	/* Creation of frame11 */
	frame11 = XtVaCreateManagedWidget( "frame11",
			xmFrameWidgetClass,
			form7,
			XmNx, 0,
			XmNy, 5,
			XmNwidth, 434,
			XmNheight, 55,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, menu6,
			NULL );
	UxPutContext( frame11, (char *) UxOptionSetContext );


	/* Creation of form11 */
	form11 = XtVaCreateManagedWidget( "form11",
			xmFormWidgetClass,
			frame11,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 2,
			XmNy, 2,
			XmNwidth, 430,
			XmNheight, 38,
			NULL );
	UxPutContext( form11, (char *) UxOptionSetContext );


	/* Creation of optSelectionMenu */
	optSelectionMenu_shell = XtVaCreatePopupShell ("optSelectionMenu_shell",
			xmMenuShellWidgetClass, form11,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	optSelectionMenu = XtVaCreateWidget( "optSelectionMenu",
			xmRowColumnWidgetClass,
			optSelectionMenu_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( optSelectionMenu, (char *) UxOptionSetContext );


	/* Creation of optSelMenuInitpb */
	optSelMenuInitpb = XtVaCreateManagedWidget( "optSelMenuInitpb",
			xmPushButtonWidgetClass,
			optSelectionMenu,
			RES_CONVERT( XmNlabelString, "Init      " ),
			NULL );
	XtAddCallback( optSelMenuInitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_optSelMenuInitpb,
		(XtPointer) UxOptionSetContext );

	UxPutContext( optSelMenuInitpb, (char *) UxOptionSetContext );


	/* Creation of optSelMenuSnappb */
	optSelMenuSnappb = XtVaCreateManagedWidget( "optSelMenuSnappb",
			xmPushButtonWidgetClass,
			optSelectionMenu,
			RES_CONVERT( XmNlabelString, "Snapshot  " ),
			NULL );
	XtAddCallback( optSelMenuSnappb, XmNactivateCallback,
		(XtCallbackProc) activateCB_optSelMenuSnappb,
		(XtPointer) UxOptionSetContext );

	UxPutContext( optSelMenuSnappb, (char *) UxOptionSetContext );


	/* Creation of optSelMenuIcProtpb */
	optSelMenuIcProtpb = XtVaCreateWidget( "optSelMenuIcProtpb",
			xmPushButtonWidgetClass,
			optSelectionMenu,
			RES_CONVERT( XmNlabelString, "I.C. prot." ),
			NULL );
	XtAddCallback( optSelMenuIcProtpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_optSelMenuIcProtpb,
		(XtPointer) UxOptionSetContext );

	UxPutContext( optSelMenuIcProtpb, (char *) UxOptionSetContext );

	if ( !strcmp( check_if_superuser (isSuperuser), "true" ) )
		XtManageChild( optSelMenuIcProtpb );


	/* Creation of optSelMenuRecpb */
	optSelMenuRecpb = XtVaCreateManagedWidget( "optSelMenuRecpb",
			xmPushButtonWidgetClass,
			optSelectionMenu,
			RES_CONVERT( XmNlabelString, "Recording " ),
			NULL );
	XtAddCallback( optSelMenuRecpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_optSelMenuRecpb,
		(XtPointer) UxOptionSetContext );

	UxPutContext( optSelMenuRecpb, (char *) UxOptionSetContext );


	/* Creation of optSelMenuMffrpb */
	optSelMenuMffrpb = XtVaCreateManagedWidget( "optSelMenuMffrpb",
			xmPushButtonWidgetClass,
			optSelectionMenu,
			RES_CONVERT( XmNlabelString, "Mf/Fr   " ),
			NULL );
	XtAddCallback( optSelMenuMffrpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_optSelMenuMffrpb,
		(XtPointer) UxOptionSetContext );

	UxPutContext( optSelMenuMffrpb, (char *) UxOptionSetContext );


	/* Creation of optSelMenuArchivepb */
	optSelMenuArchivepb = XtVaCreateManagedWidget( "optSelMenuArchivepb",
			xmPushButtonWidgetClass,
			optSelectionMenu,
			RES_CONVERT( XmNlabelString, "Archive   " ),
			NULL );
	XtAddCallback( optSelMenuArchivepb, XmNactivateCallback,
		(XtCallbackProc) activateCB_optSelMenuArchivepb,
		(XtPointer) UxOptionSetContext );

	UxPutContext( optSelMenuArchivepb, (char *) UxOptionSetContext );


	/* Creation of optSelMenuShowpb */
	optSelMenuShowpb = XtVaCreateManagedWidget( "optSelMenuShowpb",
			xmPushButtonWidgetClass,
			optSelectionMenu,
			RES_CONVERT( XmNlabelString, "Show   " ),
			NULL );
	XtAddCallback( optSelMenuShowpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_optSelMenuShowpb,
		(XtPointer) UxOptionSetContext );

	UxPutContext( optSelMenuShowpb, (char *) UxOptionSetContext );


	/* Creation of optSelectionMenu_b7 */
	optSelectionMenu_b7 = XtVaCreateManagedWidget( "optSelectionMenu_b7",
			xmSeparatorWidgetClass,
			optSelectionMenu,
			NULL );
	UxPutContext( optSelectionMenu_b7, (char *) UxOptionSetContext );


	/* Creation of optSelMenuDisplaypb */
	optSelMenuDisplaypb = XtVaCreateManagedWidget( "optSelMenuDisplaypb",
			xmPushButtonWidgetClass,
			optSelectionMenu,
			RES_CONVERT( XmNlabelString, "Display" ),
			NULL );
	XtAddCallback( optSelMenuDisplaypb, XmNactivateCallback,
		(XtCallbackProc) activateCB_optSelMenuDisplaypb,
		(XtPointer) UxOptionSetContext );

	UxPutContext( optSelMenuDisplaypb, (char *) UxOptionSetContext );


	/* Creation of optSelnMenuProgLaunchpb */
	optSelnMenuProgLaunchpb = XtVaCreateManagedWidget( "optSelnMenuProgLaunchpb",
			xmPushButtonWidgetClass,
			optSelectionMenu,
			RES_CONVERT( XmNlabelString, "User Programs" ),
			NULL );
	XtAddCallback( optSelnMenuProgLaunchpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_optSelnMenuProgLaunchpb,
		(XtPointer) UxOptionSetContext );

	UxPutContext( optSelnMenuProgLaunchpb, (char *) UxOptionSetContext );


	/* Creation of optionSelection */
	optionSelection = XtVaCreateManagedWidget( "optionSelection",
			xmRowColumnWidgetClass,
			form11,
			XmNrowColumnType, XmMENU_OPTION,
			XmNsubMenuId, optSelectionMenu,
			RES_CONVERT( XmNlabelString, "Current Selection :" ),
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNwhichButton, 1,
			NULL );
	UxPutContext( optionSelection, (char *) UxOptionSetContext );


	/* Creation of frame10 */
	frame10 = XtVaCreateManagedWidget( "frame10",
			xmFrameWidgetClass,
			form7,
			XmNx, 5,
			XmNy, 35,
			XmNwidth, 190,
			XmNheight, 90,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, form12,
			XmNleftOffset, 0,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, frame11,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame10, (char *) UxOptionSetContext );


	/* Creation of optionSetForm */
	optionSetForm = XtVaCreateManagedWidget( "optionSetForm",
			xmFormWidgetClass,
			frame10,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 2,
			XmNy, -25,
			XmNwidth, 423,
			XmNheight, 225,
			NULL );
	UxPutContext( optionSetForm, (char *) UxOptionSetContext );


	XtAddCallback( optionSet, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxOptionSetContext);


	return ( optionSet );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_optionSet()
{
	Widget                  rtrn;
	_UxCoptionSet           *UxContext;

	UxOptionSetContext = UxContext =
		(_UxCoptionSet *) UxNewContext( sizeof(_UxCoptionSet), False );


	rtrn = _Uxbuild_optionSet();

#ifndef DESIGN_TIME
	/* 
	 * setta il default 
	 */ 
	carica_options (UxGetWidget(optionSetForm),OPTION_INIT);
	opt_interface_active = 1;
#endif
	UxPopupInterface (rtrn, no_grab);
	return(rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

