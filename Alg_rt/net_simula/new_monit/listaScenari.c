
/*******************************************************************************
       listaScenari.c
       (Generated from interface file listaScenari.i)
*******************************************************************************/

#include <stdio.h>

#ifdef MOTIF
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#endif /* MOTIF */

#include "UxXt.h"
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <Xm/CascadeB.h>
#include <Xm/Separator.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <X11/Intrinsic.h>
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "bistrutt.h"

#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"


#include "messaggi.h"
#include "tabelle_malf.h"
#include "malfunzioni.h"
SCENARI scenari_malf;
SCENARI scenari_frem;
#include "cursore.h"
extern swidget create_scenarioMalf();
#include "malfunzioni.h"
extern int lista_scenari_malf_active;
extern int lista_scenari_frem_active;
Widget listaScenariMalf;
Widget listaScenariFrem;
int scenarioValido = 0;
int leggiPertMalfAttivo = 0;		/* se > 0 il timer di lettura pert e' attivo */
extern swidget masterMenu;
extern int _MAX_PERTUR;
TIPO_PERT *pertMalf;
TIPO_PERT *pertMalfOld;
XtIntervalId timer_leggiPertMalf;
int nuovoScenarioMalf = 0;
int nuovoScenarioFrem = 0;


static	int _UxIfClassId;
#ifndef XKLOADDS
#define XKLOADDS
#endif /* XKLOADDS */

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
	Widget	UxlistaScenari;
	Widget	Uxmenu2;
	Widget	UxmalfScenMenu;
	Widget	UxmalfScenMenuOpenpb;
	Widget	UxmalfScenMenuNewpb;
	Widget	UxmalfScenMenu_b2;
	Widget	UxmalfScenMenuExitpb;
	Widget	Uxmenu1_top_b1;
	Widget	Uxmenu2_p2;
	Widget	UxmalfScenEditCutpb;
	Widget	Uxmenu2_top_b1;
	Widget	Uxmenu2_p3;
	Widget	Uxmenu2_p3_b1;
	Widget	Uxmenu2_top_b2;
	Widget	Uxmenu2_p4;
	Widget	UxmalfScenMenuSummarypb;
	Widget	Uxmenu2_top_b3;
	Widget	Uxform30;
	Widget	Uxframe14;
	Widget	Uxform31;
	Widget	UxscenariMalfQuitpb1;
	Widget	UxscrolledWindow5;
	Widget	UxscrollListaScen;
	Widget	Uxmenu12;
	Widget	UxmfScenPopupOpen;
	Widget	UxmfScenPopupNew;
	Widget	Uxmenu12_p1_b4;
	Widget	UxmfScenPopupCut;
	Widget	Uxmenu12_p1_b5;
	Widget	UxmfScenPopupSummary;
	Widget	Uxmenu12_p1_b8;
	Widget	UxmfScenPopupQuit;
	int	UxscenarioSelect;
	XtIntervalId	Uxtimer_listaScenari;
	Widget	UxlistaScenariParent;
	int	Uxtipo;
} _UxClistaScenari;

static _UxClistaScenari        *UxListaScenariContext;
#define listaScenari            UxListaScenariContext->UxlistaScenari
#define menu2                   UxListaScenariContext->Uxmenu2
#define malfScenMenu            UxListaScenariContext->UxmalfScenMenu
#define malfScenMenuOpenpb      UxListaScenariContext->UxmalfScenMenuOpenpb
#define malfScenMenuNewpb       UxListaScenariContext->UxmalfScenMenuNewpb
#define malfScenMenu_b2         UxListaScenariContext->UxmalfScenMenu_b2
#define malfScenMenuExitpb      UxListaScenariContext->UxmalfScenMenuExitpb
#define menu1_top_b1            UxListaScenariContext->Uxmenu1_top_b1
#define menu2_p2                UxListaScenariContext->Uxmenu2_p2
#define malfScenEditCutpb       UxListaScenariContext->UxmalfScenEditCutpb
#define menu2_top_b1            UxListaScenariContext->Uxmenu2_top_b1
#define menu2_p3                UxListaScenariContext->Uxmenu2_p3
#define menu2_p3_b1             UxListaScenariContext->Uxmenu2_p3_b1
#define menu2_top_b2            UxListaScenariContext->Uxmenu2_top_b2
#define menu2_p4                UxListaScenariContext->Uxmenu2_p4
#define malfScenMenuSummarypb   UxListaScenariContext->UxmalfScenMenuSummarypb
#define menu2_top_b3            UxListaScenariContext->Uxmenu2_top_b3
#define form30                  UxListaScenariContext->Uxform30
#define frame14                 UxListaScenariContext->Uxframe14
#define form31                  UxListaScenariContext->Uxform31
#define scenariMalfQuitpb1      UxListaScenariContext->UxscenariMalfQuitpb1
#define scrolledWindow5         UxListaScenariContext->UxscrolledWindow5
#define scrollListaScen         UxListaScenariContext->UxscrollListaScen
#define menu12                  UxListaScenariContext->Uxmenu12
#define mfScenPopupOpen         UxListaScenariContext->UxmfScenPopupOpen
#define mfScenPopupNew          UxListaScenariContext->UxmfScenPopupNew
#define menu12_p1_b4            UxListaScenariContext->Uxmenu12_p1_b4
#define mfScenPopupCut          UxListaScenariContext->UxmfScenPopupCut
#define menu12_p1_b5            UxListaScenariContext->Uxmenu12_p1_b5
#define mfScenPopupSummary      UxListaScenariContext->UxmfScenPopupSummary
#define menu12_p1_b8            UxListaScenariContext->Uxmenu12_p1_b8
#define mfScenPopupQuit         UxListaScenariContext->UxmfScenPopupQuit
#define scenarioSelect          UxListaScenariContext->UxscenarioSelect
#define timer_listaScenari      UxListaScenariContext->Uxtimer_listaScenari
#define listaScenariParent      UxListaScenariContext->UxlistaScenariParent
#define tipo                    UxListaScenariContext->Uxtipo

int deleteScenario (int , int );
int salvaScenariMalf (Widget , int );
int gestioneTastiListaScenari (Widget);
int readScenari (Widget, int, SCENARI *);
int displayScenari (Widget, SCENARI *);
int countScenariValidi (SCENARI *);
int remove_timer_editPertMalf ();
int uscita_listaScenari (Widget , int );
int readConfMalf (Widget , int );
int attiva_timer_editPertMalf (Widget);
int attiva_timer_listaScenari (Widget);



/*******************************************************************************
       The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxlistaScenariMenuPost( wgt, client_data, event, ctd )
	Widget		wgt;
	XtPointer	client_data;
	XEvent		*event;
	int  ctd;

{
	Widget	menu = (Widget) client_data;
	int 	which_button;

	XtVaGetValues( menu, XmNwhichButton, &which_button, NULL );

	if ( event->xbutton.button == which_button )
	{
		XmMenuPosition( menu, (XButtonPressedEvent *) event );
		XtManageChild( menu );
	}
}


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_listaScenari();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

int rimuoviScenario (int n,int t)
{
#ifndef DESIGN_TIME
   deleteScenario (t, n-1);
   salvaScenariMalf (UxGetWidget(listaScenari),t);
   gestioneTastiListaScenari (listaScenari);
   switch (t)
      {
      case MALFUNZIONI:
      case SOMMARIO_MALF:
         nuovoScenarioMalf = 1;
         break;
      case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         nuovoScenarioFrem = 1;
         break;
      }

#endif
}
/**************************************/
int aggListaScenari (w, t)
Widget w;
int t;
{
SCENARI *s;

   switch (t)
      {
      case MALFUNZIONI:
      case SOMMARIO_MALF:
         s = &scenari_malf;
         if (nuovoScenarioMalf)
            {
            nuovoScenarioMalf = 0;
            readScenari (listaScenariParent, t, s);
            displayScenari (w, s);      
            gestioneTastiListaScenari (w);
            }
            break;
      case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         s = &scenari_frem;
         if (nuovoScenarioFrem)
            {
            nuovoScenarioFrem = 0;
            readScenari (listaScenariParent, t, s);
            displayScenari (w, s);      
            gestioneTastiListaScenari (w);
            }
         break;
      }
}
/**************************************/
int deleteScenario (int t, int n)
{
SCENARI *s;
   switch (t)
      {
      case MALFUNZIONI:
      case SOMMARIO_MALF:
         s = &scenari_malf;
         break;
      case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         s = &scenari_frem;
         break;
      }
   s->valido[n] = -1;
   s->n --;
   return (0);
}
/**************************************/
int gestioneTastiListaScenari (Widget w)
{
   if (countScenariValidi (&scenari_malf) > MAX_SCENARI)
      {
      XtVaSetValues (malfScenMenuNewpb, XmNsensitive, False, NULL);
      XtVaSetValues (mfScenPopupNew, XmNsensitive, False, NULL); 
      }
   else
      {
      XtVaSetValues (malfScenMenuNewpb, XmNsensitive, True, NULL);
      XtVaSetValues (mfScenPopupNew, XmNsensitive, True, NULL);  
      }
   return (0);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void  destroyCB_listaScenari( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxClistaScenari        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxListaScenariContext;
	UxListaScenariContext = UxContext =
			(_UxClistaScenari *) UxGetContext( UxWidget );
	{
	switch (tipo)
	   {
	   case MALFUNZIONI:
	   case SOMMARIO_MALF:
	      lista_scenari_malf_active = 0;
	      stop_timer (timer_listaScenari);
	      remove_timer_editPertMalf ();
	      break;
	   case FUNZIONI_REMOTE:
	   case SOMMARIO_FREM:
	      lista_scenari_frem_active = 0;
	      stop_timer (timer_listaScenari);
	      remove_timer_editPertMalf ();
	      break;
	   }
	}
	UxListaScenariContext = UxSaveCtx;
}

static void  activateCB_malfScenMenuOpenpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxClistaScenari        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxListaScenariContext;
	UxListaScenariContext = UxContext =
			(_UxClistaScenari *) UxGetContext( UxWidget );
	{
	create_scenarioMalf (UxClientData, tipo, scenarioSelect);
	
	}
	UxListaScenariContext = UxSaveCtx;
}

static void  activateCB_malfScenMenuNewpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxClistaScenari        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxListaScenariContext;
	UxListaScenariContext = UxContext =
			(_UxClistaScenari *) UxGetContext( UxWidget );
	{
	create_scenarioMalf (UxClientData, tipo, -1);
	}
	UxListaScenariContext = UxSaveCtx;
}

static void  activateCB_malfScenMenuExitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxClistaScenari        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxListaScenariContext;
	UxListaScenariContext = UxContext =
			(_UxClistaScenari *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	uscita_listaScenari (listaScenari,tipo);
#endif
	}
	UxListaScenariContext = UxSaveCtx;
}

static void  activateCB_malfScenEditCutpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxClistaScenari        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxListaScenariContext;
	UxListaScenariContext = UxContext =
			(_UxClistaScenari *) UxGetContext( UxWidget );
	{
	rimuoviScenario (scenarioSelect,tipo);
	
	}
	UxListaScenariContext = UxSaveCtx;
}

static void  activateCB_malfScenMenuSummarypb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxClistaScenari        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxListaScenariContext;
	UxListaScenariContext = UxContext =
			(_UxClistaScenari *) UxGetContext( UxWidget );
	{
	
	switch (tipo)
	   {
	   case MALFUNZIONI:
	   case SOMMARIO_MALF:
		attiva_sommario (masterMenu,SOMMARIO_MALF);
		break;
	   case FUNZIONI_REMOTE:
	   case SOMMARIO_FREM:
		attiva_sommario (masterMenu,SOMMARIO_FREM);
		break;
	   }
	}
	UxListaScenariContext = UxSaveCtx;
}

static void  activateCB_scenariMalfQuitpb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxClistaScenari        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxListaScenariContext;
	UxListaScenariContext = UxContext =
			(_UxClistaScenari *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	uscita_listaScenari (listaScenari,tipo);
#endif
	}
	UxListaScenariContext = UxSaveCtx;
}

static void  singleSelectionCB_scrollListaScen( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxClistaScenari        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxListaScenariContext;
	UxListaScenariContext = UxContext =
			(_UxClistaScenari *) UxGetContext( UxWidget );
	{
	SCENARI *s;
	XmListCallbackStruct *reason;
	reason = (XmListCallbackStruct *)UxCallbackArg;
	scenarioSelect = reason->item_position;
	
	switch (tipo)
	   {
	   case MALFUNZIONI:
	      s = &scenari_malf;
	      break;
	   case FUNZIONI_REMOTE:
	      s = &scenari_frem;
	      break;
	   }
	if (s->valido[scenarioSelect-1] > 0)
	   {
	   XtVaSetValues (malfScenMenuOpenpb,XmNsensitive,True,NULL);
	   XtVaSetValues (malfScenEditCutpb,XmNsensitive,True,NULL);
	   XtVaSetValues (mfScenPopupOpen,XmNsensitive,True,NULL);
	   XtVaSetValues (mfScenPopupCut,XmNsensitive,True,NULL);
	   scenarioValido = 1;
	   }
	else
	   {
	   XtVaSetValues (malfScenMenuOpenpb,XmNsensitive,False,NULL);
	   XtVaSetValues (malfScenEditCutpb,XmNsensitive,False,NULL);
	   XtVaSetValues (mfScenPopupOpen,XmNsensitive,False,NULL);
	   XtVaSetValues (mfScenPopupCut,XmNsensitive,False,NULL);
	   scenarioValido = 0;
	   }
	}
	UxListaScenariContext = UxSaveCtx;
}

static void  activateCB_mfScenPopupOpen( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxClistaScenari        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxListaScenariContext;
	UxListaScenariContext = UxContext =
			(_UxClistaScenari *) UxGetContext( UxWidget );
	{
	create_scenarioMalf (UxClientData, tipo, scenarioSelect);
	}
	UxListaScenariContext = UxSaveCtx;
}

static void  activateCB_mfScenPopupNew( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxClistaScenari        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxListaScenariContext;
	UxListaScenariContext = UxContext =
			(_UxClistaScenari *) UxGetContext( UxWidget );
	{
	create_scenarioMalf (UxClientData, tipo, -1);
	}
	UxListaScenariContext = UxSaveCtx;
}

static void  activateCB_mfScenPopupCut( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxClistaScenari        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxListaScenariContext;
	UxListaScenariContext = UxContext =
			(_UxClistaScenari *) UxGetContext( UxWidget );
	{
	rimuoviScenario (scenarioSelect,tipo);
	}
	UxListaScenariContext = UxSaveCtx;
}

static void  activateCB_mfScenPopupSummary( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxClistaScenari        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxListaScenariContext;
	UxListaScenariContext = UxContext =
			(_UxClistaScenari *) UxGetContext( UxWidget );
	{
	switch (tipo)
	   {
	   case MALFUNZIONI:
	   case SOMMARIO_MALF:
		attiva_sommario (masterMenu,SOMMARIO_MALF);
		break;
	   case FUNZIONI_REMOTE:
	   case SOMMARIO_FREM:
		attiva_sommario (masterMenu,SOMMARIO_FREM);
		break;
	   }
	
	
	}
	UxListaScenariContext = UxSaveCtx;
}

static void  activateCB_mfScenPopupQuit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxClistaScenari        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxListaScenariContext;
	UxListaScenariContext = UxContext =
			(_UxClistaScenari *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   uscita_listaScenari (listaScenari,tipo);
#endif
	}
	UxListaScenariContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_listaScenari()
{
	Widget		_UxParent;
	Widget		malfScenMenu_shell;
	Widget		menu2_p2_shell;
	Widget		menu2_p3_shell;
	Widget		menu2_p4_shell;
	Widget		menu12_shell;


	/* Creation of listaScenari */
	_UxParent = XtVaCreatePopupShell( "listaScenari_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 670,
			XmNy, 490,
			XmNwidth, 400,
			XmNheight, 340,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "listaScenari",
			NULL );

	listaScenari = XtVaCreateWidget( "listaScenari",
			xmFormWidgetClass,
			_UxParent,
			XmNwidth, 400,
			XmNheight, 340,
			XmNunitType, XmPIXELS,
			XmNdefaultPosition, FALSE,
			NULL );
	XtAddCallback( listaScenari, XmNdestroyCallback,
		(XtCallbackProc) destroyCB_listaScenari,
		(XtPointer) UxListaScenariContext );

	UxPutContext( listaScenari, (char *) UxListaScenariContext );


	/* Creation of menu2 */
	menu2 = XtVaCreateManagedWidget( "menu2",
			xmRowColumnWidgetClass,
			listaScenari,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( menu2, (char *) UxListaScenariContext );


	/* Creation of malfScenMenu */
	malfScenMenu_shell = XtVaCreatePopupShell ("malfScenMenu_shell",
			xmMenuShellWidgetClass, menu2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	malfScenMenu = XtVaCreateWidget( "malfScenMenu",
			xmRowColumnWidgetClass,
			malfScenMenu_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( malfScenMenu, (char *) UxListaScenariContext );


	/* Creation of malfScenMenuOpenpb */
	malfScenMenuOpenpb = XtVaCreateManagedWidget( "malfScenMenuOpenpb",
			xmPushButtonWidgetClass,
			malfScenMenu,
			RES_CONVERT( XmNlabelString, "Open" ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( malfScenMenuOpenpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_malfScenMenuOpenpb,
		UxGetWidget (listaScenari) );

	UxPutContext( malfScenMenuOpenpb, (char *) UxListaScenariContext );


	/* Creation of malfScenMenuNewpb */
	malfScenMenuNewpb = XtVaCreateManagedWidget( "malfScenMenuNewpb",
			xmPushButtonWidgetClass,
			malfScenMenu,
			RES_CONVERT( XmNlabelString, "New    " ),
			NULL );
	XtAddCallback( malfScenMenuNewpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_malfScenMenuNewpb,
		UxGetWidget (listaScenari) );

	UxPutContext( malfScenMenuNewpb, (char *) UxListaScenariContext );


	/* Creation of malfScenMenu_b2 */
	malfScenMenu_b2 = XtVaCreateManagedWidget( "malfScenMenu_b2",
			xmSeparatorWidgetClass,
			malfScenMenu,
			NULL );
	UxPutContext( malfScenMenu_b2, (char *) UxListaScenariContext );


	/* Creation of malfScenMenuExitpb */
	malfScenMenuExitpb = XtVaCreateManagedWidget( "malfScenMenuExitpb",
			xmPushButtonWidgetClass,
			malfScenMenu,
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );
	XtAddCallback( malfScenMenuExitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_malfScenMenuExitpb,
		(XtPointer) UxListaScenariContext );

	UxPutContext( malfScenMenuExitpb, (char *) UxListaScenariContext );


	/* Creation of menu1_top_b1 */
	menu1_top_b1 = XtVaCreateManagedWidget( "menu1_top_b1",
			xmCascadeButtonWidgetClass,
			menu2,
			RES_CONVERT( XmNlabelString, "File" ),
			XmNsubMenuId, malfScenMenu,
			NULL );
	UxPutContext( menu1_top_b1, (char *) UxListaScenariContext );


	/* Creation of menu2_p2 */
	menu2_p2_shell = XtVaCreatePopupShell ("menu2_p2_shell",
			xmMenuShellWidgetClass, menu2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu2_p2 = XtVaCreateWidget( "menu2_p2",
			xmRowColumnWidgetClass,
			menu2_p2_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu2_p2, (char *) UxListaScenariContext );


	/* Creation of malfScenEditCutpb */
	malfScenEditCutpb = XtVaCreateManagedWidget( "malfScenEditCutpb",
			xmPushButtonWidgetClass,
			menu2_p2,
			RES_CONVERT( XmNlabelString, "Cut   " ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( malfScenEditCutpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_malfScenEditCutpb,
		(XtPointer) UxListaScenariContext );

	UxPutContext( malfScenEditCutpb, (char *) UxListaScenariContext );


	/* Creation of menu2_top_b1 */
	menu2_top_b1 = XtVaCreateManagedWidget( "menu2_top_b1",
			xmCascadeButtonWidgetClass,
			menu2,
			RES_CONVERT( XmNlabelString, "Edit" ),
			XmNsubMenuId, menu2_p2,
			NULL );
	UxPutContext( menu2_top_b1, (char *) UxListaScenariContext );


	/* Creation of menu2_p3 */
	menu2_p3_shell = XtVaCreatePopupShell ("menu2_p3_shell",
			xmMenuShellWidgetClass, menu2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu2_p3 = XtVaCreateWidget( "menu2_p3",
			xmRowColumnWidgetClass,
			menu2_p3_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu2_p3, (char *) UxListaScenariContext );


	/* Creation of menu2_p3_b1 */
	menu2_p3_b1 = XtVaCreateManagedWidget( "menu2_p3_b1",
			xmPushButtonWidgetClass,
			menu2_p3,
			RES_CONVERT( XmNlabelString, "What" ),
			NULL );
	UxPutContext( menu2_p3_b1, (char *) UxListaScenariContext );


	/* Creation of menu2_top_b2 */
	menu2_top_b2 = XtVaCreateManagedWidget( "menu2_top_b2",
			xmCascadeButtonWidgetClass,
			menu2,
			RES_CONVERT( XmNlabelString, "Help" ),
			XmNsubMenuId, menu2_p3,
			NULL );
	UxPutContext( menu2_top_b2, (char *) UxListaScenariContext );


	/* Creation of menu2_p4 */
	menu2_p4_shell = XtVaCreatePopupShell ("menu2_p4_shell",
			xmMenuShellWidgetClass, menu2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu2_p4 = XtVaCreateWidget( "menu2_p4",
			xmRowColumnWidgetClass,
			menu2_p4_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu2_p4, (char *) UxListaScenariContext );


	/* Creation of malfScenMenuSummarypb */
	malfScenMenuSummarypb = XtVaCreateManagedWidget( "malfScenMenuSummarypb",
			xmPushButtonWidgetClass,
			menu2_p4,
			RES_CONVERT( XmNlabelString, MALF_SUMMARY ),
			NULL );
	XtAddCallback( malfScenMenuSummarypb, XmNactivateCallback,
		(XtCallbackProc) activateCB_malfScenMenuSummarypb,
		(XtPointer) UxListaScenariContext );

	UxPutContext( malfScenMenuSummarypb, (char *) UxListaScenariContext );


	/* Creation of menu2_top_b3 */
	menu2_top_b3 = XtVaCreateManagedWidget( "menu2_top_b3",
			xmCascadeButtonWidgetClass,
			menu2,
			RES_CONVERT( XmNlabelString, "Show" ),
			XmNsubMenuId, menu2_p4,
			NULL );
	UxPutContext( menu2_top_b3, (char *) UxListaScenariContext );


	/* Creation of form30 */
	form30 = XtVaCreateManagedWidget( "form30",
			xmFormWidgetClass,
			listaScenari,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 5,
			XmNy, 260,
			XmNwidth, 855,
			XmNheight, 75,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( form30, (char *) UxListaScenariContext );


	/* Creation of frame14 */
	frame14 = XtVaCreateManagedWidget( "frame14",
			xmFrameWidgetClass,
			form30,
			XmNx, 5,
			XmNy, 340,
			XmNwidth, 765,
			XmNheight, 80,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame14, (char *) UxListaScenariContext );


	/* Creation of form31 */
	form31 = XtVaCreateManagedWidget( "form31",
			xmFormWidgetClass,
			frame14,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 745,
			XmNheight, 75,
			NULL );
	UxPutContext( form31, (char *) UxListaScenariContext );


	/* Creation of scenariMalfQuitpb1 */
	scenariMalfQuitpb1 = XtVaCreateManagedWidget( "scenariMalfQuitpb1",
			xmPushButtonWidgetClass,
			form31,
			XmNx, 240,
			XmNy, 10,
			XmNwidth, 130,
			XmNheight, 45,
			RES_CONVERT( XmNlabelString, "QUIT" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNrightPosition, 60,
			XmNleftPosition, 40,
			NULL );
	XtAddCallback( scenariMalfQuitpb1, XmNactivateCallback,
		(XtCallbackProc) activateCB_scenariMalfQuitpb1,
		(XtPointer) UxListaScenariContext );

	UxPutContext( scenariMalfQuitpb1, (char *) UxListaScenariContext );


	/* Creation of scrolledWindow5 */
	scrolledWindow5 = XtVaCreateManagedWidget( "scrolledWindow5",
			xmScrolledWindowWidgetClass,
			listaScenari,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 20,
			XmNy, 105,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNheight, 250,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopOffset, 20,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, menu2,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 20,
			XmNbottomWidget, form30,
			NULL );
	UxPutContext( scrolledWindow5, (char *) UxListaScenariContext );


	/* Creation of scrollListaScen */
	scrollListaScen = XtVaCreateManagedWidget( "scrollListaScen",
			xmListWidgetClass,
			scrolledWindow5,
			XmNwidth, 660,
			XmNheight, 300,
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNvisibleItemCount, 20,
			NULL );
	XtAddCallback( scrollListaScen, XmNsingleSelectionCallback,
		(XtCallbackProc) singleSelectionCB_scrollListaScen,
		(XtPointer) UxListaScenariContext );

	UxPutContext( scrollListaScen, (char *) UxListaScenariContext );


	/* Creation of menu12 */
	menu12_shell = XtVaCreatePopupShell ("menu12_shell",
			xmMenuShellWidgetClass, scrollListaScen,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu12 = XtVaCreateWidget( "menu12",
			xmRowColumnWidgetClass,
			menu12_shell,
			XmNrowColumnType, XmMENU_POPUP,
			XmNmenuAccelerator, "Shift <KeyUp>F10",
			NULL );
	UxPutContext( menu12, (char *) UxListaScenariContext );


	/* Creation of mfScenPopupOpen */
	mfScenPopupOpen = XtVaCreateManagedWidget( "mfScenPopupOpen",
			xmPushButtonWidgetClass,
			menu12,
			RES_CONVERT( XmNlabelString, "Open" ),
			NULL );
	XtAddCallback( mfScenPopupOpen, XmNactivateCallback,
		(XtCallbackProc) activateCB_mfScenPopupOpen,
		(XtPointer) UxListaScenariContext );

	UxPutContext( mfScenPopupOpen, (char *) UxListaScenariContext );


	/* Creation of mfScenPopupNew */
	mfScenPopupNew = XtVaCreateManagedWidget( "mfScenPopupNew",
			xmPushButtonWidgetClass,
			menu12,
			RES_CONVERT( XmNlabelString, "New ..." ),
			NULL );
	XtAddCallback( mfScenPopupNew, XmNactivateCallback,
		(XtCallbackProc) activateCB_mfScenPopupNew,
		(XtPointer) UxListaScenariContext );

	UxPutContext( mfScenPopupNew, (char *) UxListaScenariContext );


	/* Creation of menu12_p1_b4 */
	menu12_p1_b4 = XtVaCreateManagedWidget( "menu12_p1_b4",
			xmSeparatorWidgetClass,
			menu12,
			NULL );
	UxPutContext( menu12_p1_b4, (char *) UxListaScenariContext );


	/* Creation of mfScenPopupCut */
	mfScenPopupCut = XtVaCreateManagedWidget( "mfScenPopupCut",
			xmPushButtonWidgetClass,
			menu12,
			RES_CONVERT( XmNlabelString, "Cut " ),
			NULL );
	XtAddCallback( mfScenPopupCut, XmNactivateCallback,
		(XtCallbackProc) activateCB_mfScenPopupCut,
		(XtPointer) UxListaScenariContext );

	UxPutContext( mfScenPopupCut, (char *) UxListaScenariContext );


	/* Creation of menu12_p1_b5 */
	menu12_p1_b5 = XtVaCreateManagedWidget( "menu12_p1_b5",
			xmSeparatorWidgetClass,
			menu12,
			NULL );
	UxPutContext( menu12_p1_b5, (char *) UxListaScenariContext );


	/* Creation of mfScenPopupSummary */
	mfScenPopupSummary = XtVaCreateManagedWidget( "mfScenPopupSummary",
			xmPushButtonWidgetClass,
			menu12,
			RES_CONVERT( XmNlabelString, MALF_SUMMARY ),
			NULL );
	XtAddCallback( mfScenPopupSummary, XmNactivateCallback,
		(XtCallbackProc) activateCB_mfScenPopupSummary,
		(XtPointer) UxListaScenariContext );

	UxPutContext( mfScenPopupSummary, (char *) UxListaScenariContext );


	/* Creation of menu12_p1_b8 */
	menu12_p1_b8 = XtVaCreateManagedWidget( "menu12_p1_b8",
			xmSeparatorWidgetClass,
			menu12,
			NULL );
	UxPutContext( menu12_p1_b8, (char *) UxListaScenariContext );


	/* Creation of mfScenPopupQuit */
	mfScenPopupQuit = XtVaCreateManagedWidget( "mfScenPopupQuit",
			xmPushButtonWidgetClass,
			menu12,
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );
	XtAddCallback( mfScenPopupQuit, XmNactivateCallback,
		(XtCallbackProc) activateCB_mfScenPopupQuit,
		(XtPointer) UxListaScenariContext );

	UxPutContext( mfScenPopupQuit, (char *) UxListaScenariContext );

	XtVaSetValues(menu2,
			XmNmenuHelpWidget, menu2_top_b2,
			NULL );


	XtAddCallback( listaScenari, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxListaScenariContext);

	XtAddEventHandler(scrollListaScen, ButtonPressMask,
			False, (XtEventHandler) _UxlistaScenariMenuPost, (XtPointer) menu12 );

	return ( listaScenari );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_listaScenari( _UxlistaScenariParent, _Uxtipo )
	Widget	_UxlistaScenariParent;
	int	_Uxtipo;
{
	Widget                  rtrn;
	_UxClistaScenari        *UxContext;
	static int		_Uxinit = 0;

	UxListaScenariContext = UxContext =
		(_UxClistaScenari *) UxNewContext( sizeof(_UxClistaScenari), False );

	listaScenariParent = _UxlistaScenariParent;
	tipo = _Uxtipo;

	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewInterfaceClassId();
		_Uxinit = 1;
	}

	{
		int numScen;   /* numero scenari validi */
		rtrn = _Uxbuild_listaScenari();
		UxPutClassCode( listaScenari, _UxIfClassId );

		timer_listaScenari = 0;
#ifndef DESIGN_TIME
		if(readConfMalf (rtrn,tipo)>=0)
		   {
		   set_cursor (listaScenariParent,CLOCK);
		   /* attiva il ciclo di lettura delle pert attive (1 in tutto il sistema) */
		   attiva_timer_editPertMalf (masterMenu); 
		   UxPopupInterface(rtrn,no_grab);
		   scenarioSelect = -1;
		   switch (tipo)
		      {
		      case MALFUNZIONI:
		      case SOMMARIO_MALF:       
		         listaScenariMalf = rtrn;
		         lista_scenari_malf_active = 1;
		         XtVaSetValues (XtParent(rtrn),
		                  XmNtitle, MALF_SCEN_LIST, XmNiconName, MALF_SCEN_LIST, NULL);
		         readScenari (listaScenariMalf, tipo, &scenari_malf);
		         displayScenari (rtrn, &scenari_malf);
		         break;
		      case FUNZIONI_REMOTE:
		      case SOMMARIO_FREM:
		         listaScenariFrem = rtrn;
		         lista_scenari_frem_active = 1;
		         XtVaSetValues (XtParent(rtrn),
		                  XmNtitle, FREM_SCEN_LIST, XmNiconName, FREM_SCEN_LIST, NULL);
		         XtVaSetValues (malfScenMenuSummarypb,
		                        RES_CONVERT(XmNlabelString,FREM_SUMMARY),NULL);
		         XtVaSetValues (mfScenPopupSummary,
		                        RES_CONVERT(XmNlabelString,FREM_SUMMARY),NULL);
		         readScenari (listaScenariMalf, tipo, &scenari_frem);
		         displayScenari (rtrn, &scenari_frem);
		         break;
		      }
		   gestioneTastiListaScenari (rtrn);     
		   attiva_timer_listaScenari (UxGetWidget(rtrn));
		   set_cursor (listaScenariParent,NORMALE);
		   }
		   else {
		      UxDestroyInterface(rtrn);
		      return(NULL);
		   }
#endif
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

