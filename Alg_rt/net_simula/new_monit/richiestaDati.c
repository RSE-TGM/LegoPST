
/*******************************************************************************
	richiestaDati.c

       Associated Header file: richiestaDati.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/SelectioB.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

#include "bistrutt.h"
#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"

#endif
/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "richiestaDati.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	okCallback_richiestaDati(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCrichiestaDati       *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxRichiestaDatiContext;
	UxRichiestaDatiContext = UxContext =
			(_UxCrichiestaDati *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	Widget campo_testo; 
	Widget padre; 
	char t[MAX_LUN_COMMENTO];
	
	   padre = (Widget)UxClientData;
	   campo_testo = XmSelectionBoxGetChild(padre,XmDIALOG_TEXT);
	
	   if (read_txt(campo_testo,t))
	      {
	      printf ("entry non corretta \n");
	      strcpy (t,act);
	      write_txt (campo_testo,t);
	      r = False;
	      return; 
	      }
	
	   strcpy (new,t);
	   DistruggiInterfaccia (XtParent(padre));
	   r = True;
	   uscita = 1;
#endif
	}
	UxRichiestaDatiContext = UxSaveCtx;
}

static	void	cancelCB_richiestaDati(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCrichiestaDati       *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxRichiestaDatiContext;
	UxRichiestaDatiContext = UxContext =
			(_UxCrichiestaDati *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	Widget campo_testo; 
	Widget padre; 
	char t[MAX_LUN_COMMENTO];
	
	   padre = (Widget)UxClientData;
	   campo_testo = XmSelectionBoxGetChild(padre,XmDIALOG_TEXT);
	
	   strcpy (t,act);
	   write_txt (campo_testo,t);
	   strcpy (new,t);
	   DistruggiInterfaccia (XtParent(padre));
	   r = True;
	   uscita = -1;
	   return;
#endif
	}
	UxRichiestaDatiContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_richiestaDati()
{
	Widget		_UxParent;


	/* Creation of richiestaDati */
	_UxParent = w;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "richiestaDati_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 880,
			XmNy, 170,
			XmNwidth, 360,
			XmNheight, 160,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "richiestaDati",
			NULL );

	richiestaDati = XtVaCreateWidget( "richiestaDati",
			xmSelectionBoxWidgetClass,
			_UxParent,
			XmNdialogType, XmDIALOG_PROMPT,
			XmNwidth, 360,
			XmNheight, 160,
			XmNunitType, XmPIXELS,
			RES_CONVERT( XmNcancelLabelString, "Quit" ),
			RES_CONVERT( XmNselectionLabelString, label_string ),
			RES_CONVERT( XmNtextString, act ),
			XmNautoUnmanage, FALSE,
			RES_CONVERT( XmNchildPlacement, "place_above_selection" ),
			NULL );
	XtAddCallback( richiestaDati, XmNokCallback,
		(XtCallbackProc) okCallback_richiestaDati,
		richiestaDati );
	XtAddCallback( richiestaDati, XmNcancelCallback,
		(XtCallbackProc) cancelCB_richiestaDati,
		richiestaDati );

	UxPutContext( richiestaDati, (char *) UxRichiestaDatiContext );


	XtAddCallback( richiestaDati, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxRichiestaDatiContext);


	return ( richiestaDati );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

int	create_richiestaDati( Widget _Uxw, unsigned char *_Uxlabel_string, unsigned char *_Uxact, unsigned char *_Uxnew )
{
	Widget                  rtrn;
	_UxCrichiestaDati       *UxContext;

	UxRichiestaDatiContext = UxContext =
		(_UxCrichiestaDati *) UxNewContext( sizeof(_UxCrichiestaDati), False );

	w = _Uxw;
	label_string = _Uxlabel_string;
	act = _Uxact;
	new = _Uxnew;

	{
		r = -1;
		uscita = -1;
		rtrn = _Uxbuild_richiestaDati();

		XtUnmanageChild (
		    XmSelectionBoxGetChild(UxGetWidget(rtrn),XmDIALOG_HELP_BUTTON));
		UxPopupInterface (rtrn,no_grab);
		r = -1;
		while (r != 1)
		   {
		   XtAppProcessEvent (UxAppContext,XtIMAll);
		   XSync (UxDisplay,0);
		   }
		printf ("chiusura richiesta\n");
		
		return(uscita);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

