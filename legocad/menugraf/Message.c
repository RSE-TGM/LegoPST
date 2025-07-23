
/*******************************************************************************
	Message.c
	(Generated from interface file Message.i)
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

#ifndef DESIGN_TIME
/*
   modulo Message.i
   tipo 
   release 1.10
   data 3/28/95
   reserved @(#)Message.i	1.10
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif


extern swidget Menu_graf;
extern swidget fileSelectionBox1,create_fileSelectionBox1();
extern int Input_Message;
extern int Salvato;

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
	int	mumble;
} _UxCMessage;


static _UxCMessage	*UxMessageContext;

Widget	Message;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_Message();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	okCallback_Message( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMessage             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMessageContext;
	UxMessageContext = UxContext =
			(_UxCMessage *) UxGetContext( UxWidget );
	{
	Position PosX,PosY;
	
	if (Input_Message == 1)
	{
		Salvato = 1;
		Input_Message == 0;
		fileSelectionBox1 = create_fileSelectionBox1();
		get_something (Menu_graf, XmNx, (void*) &PosX);
		get_something (Menu_graf, XmNy, (void*) &PosY);
		set_something_val (fileSelectionBox1, XmNx, (XtArgVal) PosX+10);
		set_something_val (fileSelectionBox1, XmNy, (XtArgVal) PosY+10);
		UxPopupInterface(fileSelectionBox1,no_grab);
	}
	}
	UxMessageContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_Message()
{
	Widget	Message_shell;

	Message_shell = XtVaCreatePopupShell( "Message_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 235,
			XmNy, 240,
			XmNwidth, 330,
			XmNheight, 175,
			XmNallowShellResize, FALSE,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "Message",
			NULL );

	Message = XtVaCreateWidget( "Message",
			xmMessageBoxWidgetClass, Message_shell,
			XmNnoResize, FALSE,
			XmNdialogType, XmDIALOG_ERROR,
			RES_CONVERT( XmNmessageString, "" ),
			RES_CONVERT( XmNdialogTitle, "Message" ),
			XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
			XmNmappedWhenManaged, TRUE,
			XmNallowOverlap, TRUE,
			XmNautoUnmanage, TRUE,
			XmNmessageAlignment, XmALIGNMENT_CENTER,
			XmNheight, 175,
			XmNwidth, 330,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( Message, (char *) UxMessageContext );

	XtAddCallback( Message, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxMessageContext );

	XtAddCallback( Message, XmNokCallback,
			okCallback_Message,
			(XtPointer) UxMessageContext );



	return ( Message );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_Message()
{
	Widget                  rtrn;
	_UxCMessage             *UxContext;

	UxMessageContext = UxContext =
		(_UxCMessage *) XtMalloc( sizeof(_UxCMessage) );

	rtrn = _Uxbuild_Message();

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_Message()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_Message();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

