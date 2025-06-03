
/*******************************************************************************
       fileSelection.c
       (Generated from interface file fileSelection.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/FileSB.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "cursore.h"
#include "cont_rec.h"
#include "file_selection.h"
#endif
#include "messaggi.h"
#ifndef VAR_FOR_CR_PATTERN
#define VAR_FOR_CR_PATTERN    "*.edf"
#endif
#ifndef PREVIEW_FILE_PATTERN
#define PREVIEW_FILE_PATTERN  "*.view"
#endif
extern char *fileSelectionCb();

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
	unsigned char	Uxfile_target[100];
	int	Uxr;
	Widget	Uxw;
	int	Uxmodo;
	int	Uxapp;
} _UxCfileSelection;

static _UxCfileSelection       *UxFileSelectionContext;
#define file_target             UxFileSelectionContext->Uxfile_target
#define r                       UxFileSelectionContext->Uxr
#define w                       UxFileSelectionContext->Uxw
#define modo                    UxFileSelectionContext->Uxmodo
#define app                     UxFileSelectionContext->Uxapp


Widget	fileSelection;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

unsigned char	*create_fileSelection();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	unmapCB_fileSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCfileSelection       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFileSelectionContext;
	UxFileSelectionContext = UxContext =
			(_UxCfileSelection *) UxGetContext( UxWidget );
	{
	
	}
	UxFileSelectionContext = UxSaveCtx;
}

static	void	okCallback_fileSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCfileSelection       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFileSelectionContext;
	UxFileSelectionContext = UxContext =
			(_UxCfileSelection *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	XmFileSelectionBoxCallbackStruct *cbs;
	cbs = (XmFileSelectionBoxCallbackStruct *)UxCallbackArg;
	strcpy (file_target,fileSelectionCb ((Widget)UxClientData,
	                    cbs,
	                    modo,
	                    app));
	r = 1;
	DistruggiInterfaccia(XtParent(fileSelection));
#endif
	}
	UxFileSelectionContext = UxSaveCtx;
}

static	void	cancelCB_fileSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCfileSelection       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFileSelectionContext;
	UxFileSelectionContext = UxContext =
			(_UxCfileSelection *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	XmFileSelectionBoxCallbackStruct *cbs;
	cbs = (XmFileSelectionBoxCallbackStruct *)UxCallbackArg;
	fileSelectionCb ((Widget)UxClientData,
	                    cbs,
	                    modo,
	                    app);
	strcpy (file_target,"");
	r = 1;
	DistruggiInterfaccia (XtParent(fileSelection));
#endif
	}
	UxFileSelectionContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_fileSelection()
{
	Widget		_UxParent;


	/* Creation of fileSelection */
	_UxParent = w;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "fileSelection_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 430,
			XmNy, 380,
			XmNwidth, 290,
			XmNheight, 380,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "fileSelection",
			NULL );

	fileSelection = XtVaCreateWidget( "fileSelection",
			xmFileSelectionBoxWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 290,
			XmNheight, 380,
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			RES_CONVERT( XmNpattern, "" ),
			XmNtextColumns, 60,
			NULL );
	XtAddCallback( fileSelection, XmNunmapCallback,
		(XtCallbackProc) unmapCB_fileSelection,
		(XtPointer) UxFileSelectionContext );
	XtAddCallback( fileSelection, XmNokCallback,
		(XtCallbackProc) okCallback_fileSelection,
		UxGetWidget(fileSelection) );
	XtAddCallback( fileSelection, XmNcancelCallback,
		(XtCallbackProc) cancelCB_fileSelection,
		UxGetWidget(fileSelection) );

	UxPutContext( fileSelection, (char *) UxFileSelectionContext );


	XtAddCallback( fileSelection, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxFileSelectionContext);


	return ( fileSelection );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

unsigned char	*create_fileSelection( _Uxw, _Uxmodo, _Uxapp )
	Widget	_Uxw;
	int	_Uxmodo;
	int	_Uxapp;
{
	Widget                  rtrn;
	_UxCfileSelection       *UxContext;

	UxFileSelectionContext = UxContext =
		(_UxCfileSelection *) UxNewContext( sizeof(_UxCfileSelection), False );

	w = _Uxw;
	modo = _Uxmodo;
	app = _Uxapp;

	rtrn = _Uxbuild_fileSelection();

	XtUnmanageChild (
	    XmFileSelectionBoxGetChild(UxGetWidget(rtrn),XmDIALOG_HELP_BUTTON));
	XtUnmanageChild (
	    XmFileSelectionBoxGetChild(UxGetWidget(rtrn),XmDIALOG_DIR_LIST));
	XtUnmanageChild (
	    XmFileSelectionBoxGetChild(UxGetWidget(rtrn),XmDIALOG_DIR_LIST_LABEL));
#ifndef DESIGN_TIME
	switch (app)
	   {
	   case VAR_FOR_CR:
	      switch (modo)
	         {
	         case CARICA:
	            update_title (UxGetWidget(rtrn),LOAD_FILESEL);
	            break;
	         case SALVA:
	            update_title (UxGetWidget(rtrn),SAVE_FILESEL);
	            break;  
	         }
	      UxPutStrRes (rtrn, XmNpattern,VAR_FOR_CR_PATTERN);
	      break;
	   case PREVIEW_FILE:
	      switch (modo)
	         {
	         case CARICA:   
	            break;
	         case SALVA:
	            break;  
	         }
	      UxPutStrRes (rtrn, XmNpattern,PREVIEW_FILE_PATTERN);
	      break;   
	   }
#endif
	UxPopupInterface (rtrn, no_grab);
	r = -1;
	while (r != 1)
	   {
	   XtAppProcessEvent (UxAppContext,XtIMAll);
	   XSync (UxDisplay,0);
	   }
	return(file_target);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

