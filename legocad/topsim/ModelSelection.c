
/*******************************************************************************
	ModelSelection.c
	(Generated from interface file ModelSelection.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/FileSB.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
   modulo ModelSelection.i
   tipo 
   release 1.22
   data 8/29/95
   reserved @(#)ModelSelection.i	1.22
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include "topsim.h"

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
	Widget	UxModelSelection;
	swidget	Uxparent;
} _UxCModelSelection;

#define ModelSelection          UxModelSelectionContext->UxModelSelection
#define parent                  UxModelSelectionContext->Uxparent

static _UxCModelSelection	*UxModelSelectionContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_ModelSelection();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	cancelCB_ModelSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelSelection      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelSelectionContext;
	UxModelSelectionContext = UxContext =
			(_UxCModelSelection *) UxGetContext( UxWidget );
	{
	UxDestroyInterface(UxWidget);
	}
	UxModelSelectionContext = UxSaveCtx;
}

static void	okCallback_ModelSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelSelection      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelSelectionContext;
	UxModelSelectionContext = UxContext =
			(_UxCModelSelection *) UxGetContext( UxWidget );
	{
	extern void set_sel_path();
	char lpath[STRLONG];
	
	strcpy(lpath, UxGetTextString(ModelSelection));  
	set_sel_path(lpath);
	
	UxDestroyInterface(UxWidget);
	}
	UxModelSelectionContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_ModelSelection()
{
	Widget	ModelSelection_shell;

	ModelSelection_shell = XtVaCreatePopupShell( "ModelSelection_shell",
			xmDialogShellWidgetClass, parent,
			XmNx, 432,
			XmNy, 255,
			XmNwidth, 255,
			XmNheight, 308,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "ModelSelection",
			NULL );

	ModelSelection = XtVaCreateWidget( "ModelSelection",
			xmFileSelectionBoxWidgetClass, ModelSelection_shell,
			XmNfileTypeMask, XmFILE_DIRECTORY,
			XmNheight, 308,
			XmNwidth, 255,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( ModelSelection, (char *) UxModelSelectionContext );

	XtAddCallback( ModelSelection, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxModelSelectionContext );

	XtAddCallback( ModelSelection, XmNcancelCallback,
			cancelCB_ModelSelection,
			(XtPointer) UxModelSelectionContext );
	XtAddCallback( ModelSelection, XmNokCallback,
			okCallback_ModelSelection,
			(XtPointer) UxModelSelectionContext );



	return ( ModelSelection );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_ModelSelection( _Uxparent )
	swidget	_Uxparent;
{
	Widget                  rtrn;
	_UxCModelSelection      *UxContext;

	UxModelSelectionContext = UxContext =
		(_UxCModelSelection *) XtMalloc( sizeof(_UxCModelSelection) );

	parent = _Uxparent;
	{
		char inidir[STRLONG];
		XmString xstring;
		
		if( getenv("LEGOCAD_USER") != NULL)
		   sprintf(inidir,"%s/legocad",getenv("LEGOCAD_USER"));
		else
		   if( getwd(inidir) == NULL )
		      sprintf(inidir,"%s","./");
		rtrn = _Uxbuild_ModelSelection();

		xstring = XmStringCreateSimple(inidir);
		set_something_val(ModelSelection,XmNdirectory, (XtArgVal) xstring);
		
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_ModelSelection( _Uxparent )
	swidget	_Uxparent;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_ModelSelection( _Uxparent );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

