
/*******************************************************************************
       ContextSelection.c
       (Generated from interface file ContextSelection.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include <Xm/FileSB.h>
#include <Xm/SelectioB.h>

#include "UxXt.h"
#include "libutilx.h"

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo ContextSelection.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)ContextSelection.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <stdlib.h>


#include <Xm/Xm.h>
#include <Xm/List.h>

#include "config.h"
#include "message.h"

extern swidget topLevelShell;
int Oper; 
Boolean chiudi_simul;


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
	int	Uxidop;
} _UxCContextSelection;

static _UxCContextSelection    *UxContextSelectionContext;
#define idop                    UxContextSelectionContext->Uxidop


Widget	ContextSelection;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	creat_ContextSelection();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	okCallback_ContextSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCContextSelection    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxContextSelectionContext;
	UxContextSelectionContext = UxContext =
			(_UxCContextSelection *) UxGetContext( UxWidget );
	{
	extern int pagedit_context_init();
	extern int display_resources();
	extern int pagedit_context_getres();
	extern PAGEDIT_CONTEXT *pagedit; 
	extern swidget ListPagine;
	extern swidget LibraryList;
	extern void removefromlist();
	extern swidget creat_DefineContext();
	extern OlDatabaseTopologiaObject dbtopologia;
	extern Boolean CompilerOk;
	extern int gest_conf_op();
	extern FILE *file_exists();
	extern void TopMenuSetSensitive();
	extern void change_database();
	char *nomeCtxSel;
	Widget text;
	swidget swid;
	int i,nitem;
	FILE *fp;
	char filertf[MAXLENFILENAME];
	char varcc[MAXLENFILENAME];
	char varsub[MAXLENFILENAME];
	char varcom[MAXLENFILENAME];
	char varrtf[MAXLENFILENAME];
	
	
	switch(Oper)
	{
	   case LOAD_CONTEXT:
	      TopMenuSetSensitive();
	      text = XmSelectionBoxGetChild(UxWidget,XmDIALOG_TEXT);
	      get_something(text,XmNvalue, (void*) &nomeCtxSel);
	
	      printf("Load file Context %s\n",nomeCtxSel);
	
	/* se il file context non esiste prima della load lo creo */
	
	      if(pagedit_context_init(nomeCtxSel,&pagedit) != False)
	      {
	         if( pagedit_context_getres(pagedit) != False )
		 {
	/* annullo la lista delle pagina e librerie nella topLevelShell (se ce ne sono) */
	            get_something(ListPagine, XmNitemCount, (void*) &nitem);
	            if(nitem > 0)
	               XmListDeleteAllItems(ListPagine);    
	            get_something(LibraryList, XmNitemCount, (void*) &nitem);
	            if(nitem > 0)
	               XmListDeleteAllItems(LibraryList);
	    
	/* visualizzo nella topLevelShell i dati del nuovo Context */
	            display_resources(pagedit);
	         } 
	      } 
	      else
	      {
	        UxDestroyInterface(ContextSelection);
	        printf("Errore nella inizializzazione del context\n");
	        return;
	      }
	
	/* annullo la lista delle pagina e librerie nella topLevelShell (se ce ne sono) */
	 /* spostate sopra
	     get_something(ListPagine, XmNitemCount, (void*) &nitem);
	      if(nitem > 0)
	         XmListDeleteAllItems(ListPagine);    
	      get_something(LibraryList, XmNitemCount, (void*) &nitem);
	      if(nitem > 0)
	         XmListDeleteAllItems(LibraryList);
	*/    
	/* visualizzo nella topLevelShell i dati del nuovo Context */
	/*
	      display_resources(pagedit);
	*/
	
	/* chiedo conferma per effettuare la killsim 
	   se la risposta e' negativa non la faccio
	   e non distruggo il database ma non permetto
	   di effettuare le compilazioni della pagina
	   settando il reltivo flag
	*/
	      if( getenv(ENVRTF) != NULL)
	      {
	         if( (strcmp(pagedit->simulator,getenv(ENVRTF))) )
	            gest_conf_op(KILL_SIMULATOR,"Do you want kill previous Simulator?",NULL);
	      }
	      else
	         change_database();
	 
	/* sia che il simulatore sia cambiato oppure no setto le 
	   variabili del context relative al Context attuale */
	      setPageditEnv(pagedit);
	      UxDestroyInterface(ContextSelection);
	
	   break;
	   case SAVE_CONTEXT:
	      text = XmSelectionBoxGetChild(UxWidget,XmDIALOG_TEXT);
	      get_something(text,XmNvalue, (void*) &nomeCtxSel);
	      printf("Save on  file Context %s\n",nomeCtxSel);
	      strcpy(pagedit->fileContext,nomeCtxSel);
	      new_save_context(pagedit);
	      UxDestroyInterface(ContextSelection);
	   break;
	   case DEF_CONTEXT:
	      text = XmSelectionBoxGetChild(UxWidget,XmDIALOG_TEXT);
	      get_something(text,XmNvalue, (void*) &nomeCtxSel);
	      if( (fp = fopen(nomeCtxSel,"r")) == NULL)
	      {
	         printf("Define New Context\n");
	         swid = creat_DefineContext(nomeCtxSel,DEF_CONTEXT);
	         UxPopupInterface(swid,no_grab);
	      } 
	      else
	      {
	         printf("Modify Context\n");
	         swid = creat_DefineContext(nomeCtxSel,MOD_CONTEXT);
	         UxPopupInterface(swid,no_grab);
	      }
	   break;
	}
	
	}
	UxContextSelectionContext = UxSaveCtx;
}

static	void	cancelCB_ContextSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCContextSelection    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxContextSelectionContext;
	UxContextSelectionContext = UxContext =
			(_UxCContextSelection *) UxGetContext( UxWidget );
	{
	UxDestroyInterface(ContextSelection);
	}
	UxContextSelectionContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_ContextSelection()
{
	Widget		_UxParent;


	/* Creation of ContextSelection */
	_UxParent = topLevelShell;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "ContextSelection_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 210,
			XmNy, 180,
			XmNwidth, 410,
			XmNheight, 380,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "ContextSelection",
			NULL );

	ContextSelection = XtVaCreateWidget( "ContextSelection",
			xmFileSelectionBoxWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 410,
			XmNheight, 380,
			RES_CONVERT( XmNpattern, "*.ctx" ),
			NULL );
	XtAddCallback( ContextSelection, XmNokCallback,
		(XtCallbackProc) okCallback_ContextSelection,
		(XtPointer) UxContextSelectionContext );
	XtAddCallback( ContextSelection, XmNcancelCallback,
		(XtCallbackProc) cancelCB_ContextSelection,
		(XtPointer) UxContextSelectionContext );

	UxPutContext( ContextSelection, (char *) UxContextSelectionContext );


	XtAddCallback( ContextSelection, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxContextSelectionContext);


	return ( ContextSelection );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	creat_ContextSelection( _Uxidop )
	int	_Uxidop;
{
	Widget                  rtrn;
	_UxCContextSelection    *UxContext;

	UxContextSelectionContext = UxContext =
		(_UxCContextSelection *) UxNewContext( sizeof(_UxCContextSelection), False );

	idop = _Uxidop;

	{
		Oper = idop;
		rtrn = _Uxbuild_ContextSelection();

		if( getenv(ENVCTX) != NULL)
		   set_something(ContextSelection,XmNdirSpec,(void*) getenv(ENVCTX));
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

