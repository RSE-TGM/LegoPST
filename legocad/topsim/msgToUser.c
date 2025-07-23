
/*******************************************************************************
	msgToUser.c
	(Generated from interface file msgToUser.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/MessageB.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
   modulo msgToUser.i
   tipo 
   release 1.17
   data 8/29/95
   reserved @(#)msgToUser.i	1.17
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/


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
	int	Uxlevel;
	unsigned char	*Uxmsg;
} _UxCmsgToUser;

#define level                   UxMsgToUserContext->Uxlevel
#define msg                     UxMsgToUserContext->Uxmsg

static _UxCmsgToUser	*UxMsgToUserContext;

Widget	msgToUser;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_msgToUser();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	okCallback_msgToUser( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmsgToUser           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMsgToUserContext;
	UxMsgToUserContext = UxContext =
			(_UxCmsgToUser *) UxGetContext( UxWidget );
	{
	UxDestroyInterface(UxWidget);
	}
	UxMsgToUserContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_msgToUser()
{
	Widget	msgToUser_shell;

	msgToUser_shell = XtVaCreatePopupShell( "msgToUser_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 448,
			XmNy, 353,
			XmNwidth, 494,
			XmNheight, 225,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "msgToUser",
			NULL );

	msgToUser = XtVaCreateWidget( "msgToUser",
			xmMessageBoxWidgetClass, msgToUser_shell,
			RES_CONVERT( XmNmessageString, "Message To User" ),
			XmNmessageAlignment, XmALIGNMENT_CENTER,
			XmNdialogType, XmDIALOG_MESSAGE,
			XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
			XmNheight, 225,
			XmNwidth, 494,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( msgToUser, (char *) UxMsgToUserContext );

	XtAddCallback( msgToUser, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxMsgToUserContext );

	XtVaSetValues(msgToUser,
			RES_CONVERT( XmNsymbolPixmap, "/usr/include/X11/bitmaps/xdt_m_large/warning.px" ),
			NULL );

	XtAddCallback( msgToUser, XmNokCallback,
			okCallback_msgToUser,
			(XtPointer) UxMsgToUserContext );



	return ( msgToUser );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_msgToUser( _Uxlevel, _Uxmsg )
	int	_Uxlevel;
	unsigned char	*_Uxmsg;
{
	Widget                  rtrn;
	_UxCmsgToUser           *UxContext;

	UxMsgToUserContext = UxContext =
		(_UxCmsgToUser *) XtMalloc( sizeof(_UxCmsgToUser) );

	level = _Uxlevel;
	msg = _Uxmsg;
	{
		Arg arg[2];
		Cardinal narg;
		XmString xmsg;
		rtrn = _Uxbuild_msgToUser();

		xmsg=XmStringCreateSimple(msg);
		
		set_something_val(rtrn,XmNmessageString, (XtArgVal) xmsg);
		
		XtDestroyWidget(XmMessageBoxGetChild(rtrn,XmDIALOG_CANCEL_BUTTON));
		XtDestroyWidget(XmMessageBoxGetChild(rtrn,XmDIALOG_HELP_BUTTON));
		UxPopupInterface(rtrn,no_grab);
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_msgToUser( _Uxlevel, _Uxmsg )
	int	_Uxlevel;
	unsigned char	*_Uxmsg;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_msgToUser( _Uxlevel, _Uxmsg );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

