
/*******************************************************************************
       SaveSelection.c
       (Generated from interface file SaveSelection.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/SelectioB.h>
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
   modulo SaveSelection.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)SaveSelection.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#ifndef ON_PAGEDIT
extern char *nome_file_draw;
#endif


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
	Widget	UxSaveSelection;
	Widget	UxParent;
	Widget	UxPadre;
} _UxCSaveSelection;

static _UxCSaveSelection       *UxSaveSelectionContext;
#define SaveSelection           UxSaveSelectionContext->UxSaveSelection
#define Parent                  UxSaveSelectionContext->UxParent
#define Padre                   UxSaveSelectionContext->UxPadre



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_SaveSelection();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	okCallback_SaveSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCSaveSelection       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSaveSelectionContext;
	UxSaveSelectionContext = UxContext =
			(_UxCSaveSelection *) UxGetContext( UxWidget );
	{
	extern swidget create_MessageBox();
#ifndef ON_PAGEDIT
	extern Widget FinestraDraw;
	extern char *extract_string();
	 
	Arg args[3];
	XmString XmStr;
	char *NomeFile;
	FILE *PuntFile;
	
	XtSetArg(args[0],XmNtextString,&XmStr);
	XtGetValues(UxWidget,args,1);
	
	NomeFile = extract_string (XmStr);
	
	PuntFile = fopen (NomeFile,"r");
	if (PuntFile == NULL)
	  {
	  /*  Il file non esiste e quindi puo' essere salvato  */
	  WriteBackground (FinestraDraw,NomeFile);
	  XtDestroyWidget (XtParent(SaveSelection));
	  }
	else
	  {
	  /* Il file esiste e quindi viene chiesta una conferma  */ 
	  UxPopupInterface ( create_MessageBox(XtParent(SaveSelection),NomeFile),no_grab);
	  }
#endif
	}
	UxSaveSelectionContext = UxSaveCtx;
}

static	void	cancelCB_SaveSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCSaveSelection       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSaveSelectionContext;
	UxSaveSelectionContext = UxContext =
			(_UxCSaveSelection *) UxGetContext( UxWidget );
	{
	XtDestroyWidget (XtParent(SaveSelection));
	}
	UxSaveSelectionContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_SaveSelection()
{
	Widget		_UxParent;


	/* Creation of SaveSelection */
	_UxParent = Parent;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "SaveSelection_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 600,
			XmNy, 270,
			XmNwidth, 200,
			XmNheight, 190,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "SaveSelection",
			NULL );

	SaveSelection = XtVaCreateWidget( "SaveSelection",
			xmSelectionBoxWidgetClass,
			_UxParent,
			XmNdialogType, XmDIALOG_PROMPT,
			XmNunitType, XmPIXELS,
			XmNwidth, 200,
			XmNheight, 190,
			RES_CONVERT( XmNselectionLabelString, "New File Name:" ),
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			XmNautoUnmanage, FALSE,
			NULL );
	XtAddCallback( SaveSelection, XmNokCallback,
		(XtCallbackProc) okCallback_SaveSelection,
		(XtPointer) UxSaveSelectionContext );
	XtAddCallback( SaveSelection, XmNcancelCallback,
		(XtCallbackProc) cancelCB_SaveSelection,
		(XtPointer) UxSaveSelectionContext );

	UxPutContext( SaveSelection, (char *) UxSaveSelectionContext );


	XtAddCallback( SaveSelection, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxSaveSelectionContext);


	return ( SaveSelection );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_SaveSelection( _UxPadre )
	Widget	_UxPadre;
{
	Widget                  rtrn;
	_UxCSaveSelection       *UxContext;

	UxSaveSelectionContext = UxContext =
		(_UxCSaveSelection *) UxNewContext( sizeof(_UxCSaveSelection), False );

	Padre = _UxPadre;

	{
		char *NomeFile;
		XmString XmTesto;
		
		Parent=Padre;
		rtrn = _Uxbuild_SaveSelection();

#ifndef ON_PAGEDIT
		NomeFile = XtNewString (nome_file_draw);
		XmTesto = XmStringCreateSimple (NomeFile);
		set_something_val (SaveSelection, XmNtextString, (XtArgVal)XmTesto);
		XtFree (NomeFile);
#endif
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

