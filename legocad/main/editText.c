
/*******************************************************************************
	editText.c
	(Generated from interface file editText.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/ScrolledW.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/Form.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo editText.i
   tipo 
   release 1.8
   data 7/7/95
   reserved @(#)editText.i	1.8
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif


/* interfaccia per gestione scrolled_text
   descrizione,foraus...etc
*/
#include "legomain.h"

extern Boolean attesa_oknew;

extern int save_descrizione();

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
	Widget	UxeditText;
	Widget	Uxlabel11;
	Widget	UxpushButton12;
	Widget	UxpushButton13;
	Widget	UxscrolledWindow6;
	Widget	UxscrolledText2;
	Widget	Uxwcaller;
	int	Uxid_oper;
	unsigned char	*Uxtext_input;
	unsigned char	*Uxtext_out;
	Widget	Uxcall;
	int	Uxt_ope;
	unsigned char	*Uxtext;
	unsigned char	**Uxouttext;
	int	*(*UxOkCB)();
} _UxCeditText;

#define editText                UxEditTextContext->UxeditText
#define label11                 UxEditTextContext->Uxlabel11
#define pushButton12            UxEditTextContext->UxpushButton12
#define pushButton13            UxEditTextContext->UxpushButton13
#define scrolledWindow6         UxEditTextContext->UxscrolledWindow6
#define scrolledText2           UxEditTextContext->UxscrolledText2
#define wcaller                 UxEditTextContext->Uxwcaller
#define id_oper                 UxEditTextContext->Uxid_oper
#define text_input              UxEditTextContext->Uxtext_input
#define text_out                UxEditTextContext->Uxtext_out
#define call                    UxEditTextContext->Uxcall
#define t_ope                   UxEditTextContext->Uxt_ope
#define text                    UxEditTextContext->Uxtext
#define outtext                 UxEditTextContext->Uxouttext
#define OkCB                    UxEditTextContext->UxOkCB

static _UxCeditText	*UxEditTextContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_editText();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

set_titolo(char *title)
{
  UxPutLabelString(label11,title);
}

set_editabile(Boolean edi)
{
   if( edi == True)
      set_something_val(scrolledText2,XmNeditable, (XtArgVal) True);
   else
      set_something_val(scrolledText2,XmNeditable, (XtArgVal) False);
}

set_text(char *stringa)
{
   XmTextSetString(scrolledText2,stringa);
}

get_text(char **text_output)
{
   char *appo;
   appo = XmTextGetString(scrolledText2);

     (*text_output) = malloc(strlen(appo)+2);
     strcpy((*text_output),appo);
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_pushButton12( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCeditText            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxEditTextContext;
	UxEditTextContext = UxContext =
			(_UxCeditText *) UxGetContext( UxWidget );
	{
	char *appo;
	
	switch(id_oper)
	{
	   case VIEW_DESCR:
	   case VIEW_FORAUS:
	   break;
	   case EDIT_FORAUS:
	    get_text(&text_out);
	    *outtext = text_out;
	   break;
	   case EDIT_DESCR:
	     get_text(&text_out);
	     *outtext = text_out;
	     if( (OkCB) != NULL)
	        OkCB();
	   break;
	}
	
	attesa_oknew = False;
	
	UxDestroyInterface(XtParent(UxWidget));
	
	}
	UxEditTextContext = UxSaveCtx;
}

static void	activateCB_pushButton13( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCeditText            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxEditTextContext;
	UxEditTextContext = UxContext =
			(_UxCeditText *) UxGetContext( UxWidget );
	{
	attesa_oknew = False;
	UxDestroyInterface(XtParent(UxWidget));
	}
	UxEditTextContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_editText()
{
	Widget	editText_shell;

	editText_shell = XtVaCreatePopupShell( "editText_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 220,
			XmNy, 350,
			XmNwidth, 680,
			XmNheight, 430,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "editText",
			XmNiconName, "editText",
			NULL );

	editText = XtVaCreateManagedWidget( "editText",
			xmFormWidgetClass, editText_shell,
			XmNheight, 430,
			XmNwidth, 680,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( editText, (char *) UxEditTextContext );

	label11 = XtVaCreateManagedWidget( "label11",
			xmLabelWidgetClass, editText,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 630,
			XmNy, 10,
			XmNx, 20,
			NULL );

	UxPutContext( label11, (char *) UxEditTextContext );

	pushButton12 = XtVaCreateManagedWidget( "pushButton12",
			xmPushButtonWidgetClass, editText,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNheight, 30,
			XmNwidth, 130,
			XmNy, 390,
			XmNx, 30,
			NULL );

	UxPutContext( pushButton12, (char *) UxEditTextContext );

	pushButton13 = XtVaCreateManagedWidget( "pushButton13",
			xmPushButtonWidgetClass, editText,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNheight, 30,
			XmNwidth, 130,
			XmNy, 390,
			XmNx, 530,
			NULL );

	UxPutContext( pushButton13, (char *) UxEditTextContext );

	scrolledWindow6 = XtVaCreateManagedWidget( "scrolledWindow6",
			xmScrolledWindowWidgetClass, editText,
			XmNtopWidget, label11,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, pushButton12,
			XmNbottomOffset, 20,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 60,
			XmNx, 20,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow6, (char *) UxEditTextContext );

	scrolledText2 = XtVaCreateManagedWidget( "scrolledText2",
			xmTextWidgetClass, scrolledWindow6,
			XmNwordWrap, TRUE,
			XmNeditable, FALSE,
			XmNeditMode, XmMULTI_LINE_EDIT ,
			XmNcolumns, 80,
			XmNheight, 300,
			XmNwidth, 640,
			NULL );

	UxPutContext( scrolledText2, (char *) UxEditTextContext );

	XtAddCallback( editText, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxEditTextContext );

	XtAddCallback( pushButton12, XmNactivateCallback,
			activateCB_pushButton12,
			(XtPointer) UxEditTextContext );

	XtAddCallback( pushButton13, XmNactivateCallback,
			activateCB_pushButton13,
			(XtPointer) UxEditTextContext );



	return ( editText );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_editText( _Uxcall, _Uxt_ope, _Uxtext, _Uxouttext, _UxOkCB )
	Widget	_Uxcall;
	int	_Uxt_ope;
	unsigned char	*_Uxtext;
	unsigned char	**_Uxouttext;
	int	*(*_UxOkCB)();
{
	Widget                  rtrn;
	_UxCeditText            *UxContext;

	UxEditTextContext = UxContext =
		(_UxCeditText *) XtMalloc( sizeof(_UxCeditText) );

	call = _Uxcall;
	t_ope = _Uxt_ope;
	text = _Uxtext;
	outtext = _Uxouttext;
	OkCB = _UxOkCB;
	{
		wcaller = call;
		id_oper = t_ope;
		text_input = text;
		rtrn = _Uxbuild_editText();

		switch(id_oper)
		{
		   case VIEW_DESCR:
		      set_titolo("Model Description View");
		      set_editabile(False);
		   break;
		   case EDIT_DESCR:
		      set_titolo("Model Description Edit");
		      set_editabile(True);
		   break;
		   case EDIT_FORAUS:
		      set_titolo("Model Auxiliary Fortran Edit");
		      set_editabile(True);
		   break;
		   case VIEW_FORAUS:
		      set_titolo("Model Auxiliary Fortran View");
		      set_editabile(False);
		   break;
		}
		
		set_text(text_input);
		
		
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

Widget	create_editText( _Uxcall, _Uxt_ope, _Uxtext, _Uxouttext, _UxOkCB )
	Widget	_Uxcall;
	int	_Uxt_ope;
	unsigned char	*_Uxtext;
	unsigned char	**_Uxouttext;
	int	*(*_UxOkCB)();
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_editText( _Uxcall, _Uxt_ope, _Uxtext, _Uxouttext, _UxOkCB );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

