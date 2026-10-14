
/*******************************************************************************
       MessageBox.c
       (Generated from interface file MessageBox.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/MessageB.h>
/* dichiarazioni prima implicite (vedi Alg_mmi/lib/Xl/XlProto.h) */
void *UxNewContext();     /* UxXt.c: ritorna un puntatore, senza dichiarazione veniva troncato */
void  WriteBackground();  /* graphics.c */
void  set_something_val();/* AlgLib/libutilx */

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo MessageBox.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)MessageBox.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/


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
	Widget	UxMessageBox;
	Widget	UxChiamante;
	unsigned char	*UxNomeDelFile;
	Widget	UxChia;
	unsigned char	*UxNFile;
} _UxCMessageBox;

static _UxCMessageBox          *UxMessageBoxContext;
#define MessageBox              UxMessageBoxContext->UxMessageBox
#define Chiamante               UxMessageBoxContext->UxChiamante
#define NomeDelFile             UxMessageBoxContext->UxNomeDelFile
#define Chia                    UxMessageBoxContext->UxChia
#define NFile                   UxMessageBoxContext->UxNFile



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_MessageBox();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	cancelCB_MessageBox( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCMessageBox          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMessageBoxContext;
	UxMessageBoxContext = UxContext =
			(_UxCMessageBox *) UxGetContext( UxWidget );
	{
	XtDestroyWidget (MessageBox);
	}
	UxMessageBoxContext = UxSaveCtx;
}

static	void	okCallback_MessageBox( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCMessageBox          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMessageBoxContext;
	UxMessageBoxContext = UxContext =
			(_UxCMessageBox *) UxGetContext( UxWidget );
	{
#ifndef ON_PAGEDIT
	extern Widget FinestraDraw;
	 
	WriteBackground (FinestraDraw,NomeDelFile);
	XtDestroyWidget (Chiamante);
	XtDestroyWidget (MessageBox);
#endif
	
	}
	UxMessageBoxContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_MessageBox()
{
	Widget		_UxParent;


	/* Creation of MessageBox */
	_UxParent = Chiamante;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "MessageBox_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 650,
			XmNy, 260,
			XmNwidth, 280,
			XmNheight, 180,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "MessageBox",
			NULL );

	MessageBox = XtVaCreateWidget( "MessageBox",
			xmMessageBoxWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 280,
			XmNheight, 180,
			XmNlabelFontList, UxConvertFontList( "-adobe-itc avant garde gothic-demi-r-normal--14-140-75-75-p-82-iso8859-1" ),
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			NULL );
	XtAddCallback( MessageBox, XmNcancelCallback,
		(XtCallbackProc) cancelCB_MessageBox,
		(XtPointer) UxMessageBoxContext );
	XtAddCallback( MessageBox, XmNokCallback,
		(XtCallbackProc) okCallback_MessageBox,
		(XtPointer) UxMessageBoxContext );

	UxPutContext( MessageBox, (char *) UxMessageBoxContext );


	XtAddCallback( MessageBox, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxMessageBoxContext);


	return ( MessageBox );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_MessageBox( _UxChia, _UxNFile )
	Widget	_UxChia;
	unsigned char	*_UxNFile;
{
	Widget                  rtrn;
	_UxCMessageBox          *UxContext;

	UxMessageBoxContext = UxContext =
		(_UxCMessageBox *) UxNewContext( sizeof(_UxCMessageBox), False );

	Chia = _UxChia;
	NFile = _UxNFile;

	{
		XmString XmMess;
		
		Chiamante = Chia;
		NomeDelFile = NFile;
		rtrn = _Uxbuild_MessageBox();

		/*  Setto il messaggio da visualizzare  */
		XmMess = XmStringCreateSimple ("File already exist! Overwrite?");
		set_something_val (MessageBox, XmNmessageString, (XtArgVal) XmMess);
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

