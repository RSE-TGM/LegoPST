
/*******************************************************************************
       messageBoxDialog2.c
       (Generated from interface file messageBoxDialog2.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/MessageB.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo messageBoxDialog2.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)messageBoxDialog2.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <Xm/Xm.h>
#include "message.h"
#include "libutilx.h"

extern  void *		UxNewContext();

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
	Widget	UxmessageBoxDialog2;
	Widget	UxParent;
} _UxCmessageBoxDialog2;

static _UxCmessageBoxDialog2   *UxMessageBoxDialog2Context;
#define messageBoxDialog2       UxMessageBoxDialog2Context->UxmessageBoxDialog2
#define Parent                  UxMessageBoxDialog2Context->UxParent



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_messageBoxDialog2();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

Boolean ConfiguraDialogo(MESSAGGI mes)
{
   Widget WCButton,WHButton;
   XmString Titolo,Messaggio,LabOk,LabCancel,LabHelp;

/* set layout interfaccia */
   switch(mes.idType)
   {
      case INFMSG:
         set_something(messageBoxDialog2,XmNdialogType,(void*) XmDIALOG_MESSAGE);
         set_something(messageBoxDialog2,XmNdialogStyle,(void*) XmDIALOG_PRIMARY_APPLICATION_MODAL);
      break;
      case WRNMSG:
         set_something(messageBoxDialog2,XmNdialogType,(void*) XmDIALOG_WARNING);
         set_something(messageBoxDialog2,XmNdialogStyle,(void*) XmDIALOG_PRIMARY_APPLICATION_MODAL);
      break;
      case ERRMSG:
         set_something(messageBoxDialog2,XmNdialogType,(void*) XmDIALOG_ERROR);
         set_something(messageBoxDialog2,XmNdialogStyle,(void*) XmDIALOG_FULL_APPLICATION_MODAL);
      break;      
   }

   WCButton = XmMessageBoxGetChild(messageBoxDialog2,XmDIALOG_CANCEL_BUTTON);     
   WHButton = XmMessageBoxGetChild(messageBoxDialog2,XmDIALOG_HELP_BUTTON);     

/* set Lebl dei bottoni e destroy dei bottoni non desiderati */


   LabOk = XmStringCreateSimple(mes.LabelOk);
   set_something(messageBoxDialog2,XmNokLabelString,(void*) LabOk);

   if(!mes.CancelButton && WCButton)
      XtDestroyWidget(WCButton);
   else
   {
      LabCancel = XmStringCreateSimple(mes.LabelCancel);
      set_something(messageBoxDialog2,XmNcancelLabelString,(void*) LabCancel);
   }

   if(!mes.HelpButton && WHButton)
      XtDestroyWidget(WHButton);
   else
   {
      LabHelp   = XmStringCreateSimple(mes.LabelHelp);
      set_something(messageBoxDialog2,XmNhelpLabelString,(void*) LabHelp);
   }


/* setto il titolo e il messaggio */
   Messaggio = XmStringCreateSimple(mes.msg);
   Titolo   = XmStringCreateSimple(mes.titolo);
   set_something(messageBoxDialog2,XmNdialogTitle,(void*) Titolo);
   set_something(messageBoxDialog2,XmNmessageString,(void*) Messaggio);

   return(True);
}

void SetMsg(swidget WParent,int id,int err_level,char *shtitle,char *msg,char *lOk,Boolean Cancel,char *lCancel,Boolean Help,char *lHelp)
{
   swidget ShMsg;
   MESSAGGI mesg;

   mesg.idOp = id;
   mesg.idType = err_level;

   if(shtitle != NULL)
      strcpy(mesg.titolo,shtitle);
  

   strcpy(mesg.msg,msg);

   if(lOk != NULL)
      strcpy(mesg.LabelOk,lOk);
   else
      strcpy(mesg.LabelOk,"Ok");

   mesg.CancelButton = Cancel;

   if(lCancel != NULL)
      strcpy(mesg.LabelCancel,lCancel);
   else
      strcpy(mesg.LabelCancel,"Cancel");

   mesg.HelpButton = Help;

   if(lHelp != NULL)
      strcpy(mesg.LabelHelp,lHelp); 
   else
      strcpy(mesg.LabelHelp,"Help"); 

   if(WParent == NULL)
      WParent = UxTopLevel;
 
#ifndef DESIGN_TIME
   ShMsg = create_messageBoxDialog2(WParent);
   if( ConfiguraDialogo(mesg) == True)
      UxPopupInterface(ShMsg,no_grab);
#endif
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_messageBoxDialog2()
{
	Widget		_UxParent;


	/* Creation of messageBoxDialog2 */
	_UxParent = Parent;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "messageBoxDialog2_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNwidth, 440,
			XmNheight, 180,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "messageBoxDialog2",
			NULL );

	messageBoxDialog2 = XtVaCreateWidget( "messageBoxDialog2",
			xmMessageBoxWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 440,
			XmNheight, 180,
			XmNallowOverlap, FALSE,
			XmNmessageAlignment, XmALIGNMENT_CENTER,
			NULL );
	UxPutContext( messageBoxDialog2, (char *) UxMessageBoxDialog2Context );


	XtAddCallback( messageBoxDialog2, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxMessageBoxDialog2Context);


	return ( messageBoxDialog2 );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_messageBoxDialog2( _UxParent )
	Widget	_UxParent;
{
	Widget                  rtrn;
	_UxCmessageBoxDialog2   *UxContext;

	UxMessageBoxDialog2Context = UxContext =
		(_UxCmessageBoxDialog2 *) UxNewContext( sizeof(_UxCmessageBoxDialog2), False );

	Parent = _UxParent;

	{
		if(Parent == NULL)
			Parent = UxTopLevel;
		rtrn = _Uxbuild_messageBoxDialog2();

		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

