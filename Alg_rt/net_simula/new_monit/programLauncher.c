
/*******************************************************************************
       programLauncher.c
       (Generated from interface file programLauncher.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "sim_param.h"
#include "option.h"

char selectedCommand[MAX_USERPROG_LUN];


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
	Widget	UxprogramLauncher;
	Widget	Uxmenu8;
	Widget	Uxmenu8_p1;
	Widget	UxprogLaunchFileQuitpb;
	Widget	Uxmenu8_top_b1;
	Widget	Uxframe22;
	Widget	Uxform29;
	Widget	UxprogLaunchExecpb;
	Widget	UxprogLaunchQuitpb;
	Widget	UxrowColumn4;
	Widget	UxprogLaunchText[8];
	unsigned char	Uxcomandi[8][100];
	swidget	UxUxParent;
} _UxCprogramLauncher;

static _UxCprogramLauncher     *UxProgramLauncherContext;
#define programLauncher         UxProgramLauncherContext->UxprogramLauncher
#define menu8                   UxProgramLauncherContext->Uxmenu8
#define menu8_p1                UxProgramLauncherContext->Uxmenu8_p1
#define progLaunchFileQuitpb    UxProgramLauncherContext->UxprogLaunchFileQuitpb
#define menu8_top_b1            UxProgramLauncherContext->Uxmenu8_top_b1
#define frame22                 UxProgramLauncherContext->Uxframe22
#define form29                  UxProgramLauncherContext->Uxform29
#define progLaunchExecpb        UxProgramLauncherContext->UxprogLaunchExecpb
#define progLaunchQuitpb        UxProgramLauncherContext->UxprogLaunchQuitpb
#define rowColumn4              UxProgramLauncherContext->UxrowColumn4
#define progLaunchText          UxProgramLauncherContext->UxprogLaunchText
#define comandi                 UxProgramLauncherContext->Uxcomandi
#define UxParent                UxProgramLauncherContext->UxUxParent



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_programLauncher();
int DistruggiInterfaccia (Widget );
int creaBottoniLauncher (Widget);
int loadPrograms(Widget);




/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_progLaunchFileQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCprogramLauncher     *UxSaveCtx, *UxContext;

	UxSaveCtx = UxProgramLauncherContext;
	UxProgramLauncherContext = UxContext =
			(_UxCprogramLauncher *) UxGetContext( UxWidget );
	{
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
	}
	UxProgramLauncherContext = UxSaveCtx;
}

static	void	activateCB_progLaunchExecpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCprogramLauncher     *UxSaveCtx, *UxContext;

	UxSaveCtx = UxProgramLauncherContext;
	UxProgramLauncherContext = UxContext =
			(_UxCprogramLauncher *) UxGetContext( UxWidget );
	{
	system (selectedCommand);
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
	}
	UxProgramLauncherContext = UxSaveCtx;
}

static	void	activateCB_progLaunchQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCprogramLauncher     *UxSaveCtx, *UxContext;

	UxSaveCtx = UxProgramLauncherContext;
	UxProgramLauncherContext = UxContext =
			(_UxCprogramLauncher *) UxGetContext( UxWidget );
	{
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
	}
	UxProgramLauncherContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_programLauncher()
{
	Widget		_UxParent;
	Widget		menu8_p1_shell;


	/* Creation of programLauncher */
	_UxParent = XtVaCreatePopupShell( "programLauncher_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 270,
			XmNy, 390,
			XmNwidth, 330,
			XmNheight, 310,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "programLauncher",
			NULL );

	programLauncher = XtVaCreateWidget( "programLauncher",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 330,
			XmNheight, 310,
			NULL );
	UxPutContext( programLauncher, (char *) UxProgramLauncherContext );


	/* Creation of menu8 */
	menu8 = XtVaCreateManagedWidget( "menu8",
			xmRowColumnWidgetClass,
			programLauncher,
			XmNrowColumnType, XmMENU_BAR,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( menu8, (char *) UxProgramLauncherContext );


	/* Creation of menu8_p1 */
	menu8_p1_shell = XtVaCreatePopupShell ("menu8_p1_shell",
			xmMenuShellWidgetClass, menu8,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu8_p1 = XtVaCreateWidget( "menu8_p1",
			xmRowColumnWidgetClass,
			menu8_p1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu8_p1, (char *) UxProgramLauncherContext );


	/* Creation of progLaunchFileQuitpb */
	progLaunchFileQuitpb = XtVaCreateManagedWidget( "progLaunchFileQuitpb",
			xmPushButtonWidgetClass,
			menu8_p1,
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );
	XtAddCallback( progLaunchFileQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_progLaunchFileQuitpb,
		programLauncher );

	UxPutContext( progLaunchFileQuitpb, (char *) UxProgramLauncherContext );


	/* Creation of menu8_top_b1 */
	menu8_top_b1 = XtVaCreateManagedWidget( "menu8_top_b1",
			xmCascadeButtonWidgetClass,
			menu8,
			RES_CONVERT( XmNlabelString, "File" ),
			XmNsubMenuId, menu8_p1,
			NULL );
	UxPutContext( menu8_top_b1, (char *) UxProgramLauncherContext );


	/* Creation of frame22 */
	frame22 = XtVaCreateManagedWidget( "frame22",
			xmFrameWidgetClass,
			programLauncher,
			XmNx, 0,
			XmNy, 150,
			XmNwidth, 360,
			XmNheight, 60,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame22, (char *) UxProgramLauncherContext );


	/* Creation of form29 */
	form29 = XtVaCreateManagedWidget( "form29",
			xmFormWidgetClass,
			frame22,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 90,
			XmNy, 10,
			XmNwidth, 200,
			XmNheight, 40,
			NULL );
	UxPutContext( form29, (char *) UxProgramLauncherContext );


	/* Creation of progLaunchExecpb */
	progLaunchExecpb = XtVaCreateManagedWidget( "progLaunchExecpb",
			xmPushButtonWidgetClass,
			form29,
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
			RES_CONVERT( XmNlabelString, "Execute" ),
			NULL );
	XtAddCallback( progLaunchExecpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_progLaunchExecpb,
		programLauncher );

	UxPutContext( progLaunchExecpb, (char *) UxProgramLauncherContext );


	/* Creation of progLaunchQuitpb */
	progLaunchQuitpb = XtVaCreateManagedWidget( "progLaunchQuitpb",
			xmPushButtonWidgetClass,
			form29,
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
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );
	XtAddCallback( progLaunchQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_progLaunchQuitpb,
		programLauncher );

	UxPutContext( progLaunchQuitpb, (char *) UxProgramLauncherContext );


	/* Creation of rowColumn4 */
	rowColumn4 = XtVaCreateManagedWidget( "rowColumn4",
			xmRowColumnWidgetClass,
			programLauncher,
			XmNx, 10,
			XmNy, 40,
			XmNwidth, 320,
			XmNheight, 300,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, frame22,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, menu8,
			XmNnumColumns, 1,
			XmNradioBehavior, TRUE,
			NULL );
	UxPutContext( rowColumn4, (char *) UxProgramLauncherContext );


	XtAddCallback( programLauncher, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxProgramLauncherContext);


	return ( programLauncher );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_programLauncher( _UxUxParent )
	swidget	_UxUxParent;
{
	Widget                  rtrn;
	_UxCprogramLauncher     *UxContext;

	UxProgramLauncherContext = UxContext =
		(_UxCprogramLauncher *) UxNewContext( sizeof(_UxCprogramLauncher), False );

	UxParent = _UxUxParent;

	{
		strcpy (selectedCommand,"");
		rtrn = _Uxbuild_programLauncher();

		creaBottoniLauncher (programLauncher);
		loadPrograms (programLauncher);
		UxPopupInterface (rtrn,no_grab);
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

