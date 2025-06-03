
/*******************************************************************************
       initSimulator.c
       (Generated from interface file initSimulator.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/CascadeB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <malloc.h>
#include <X11/Intrinsic.h>

#ifndef DESIGN_TIME
extern int _MAX_SNAP_SHOT;
extern int _MAX_BACK_TRACK;
#include "init_sim.h"
#include "backtrack.h"
#include "bistrutt.h"
#include "preview.h"
#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"

/* #include "interf_attive.h"  */
extern int init_ci_interface_active;
extern int init_bt_interface_active;
extern int bt_interface_active;
#include "cursore.h"
extern int inizializzazione;
extern int backtrack_caricato;
SNTAB *snap_list;
BKTAB *bt_list;
XtIntervalId timer_initci;
XtIntervalId timer_initbt;
int tasto_loadci_attivabile = False;
int tasto_loadbt_attivabile = False;
#endif

int attiva_timer_initSimci (Widget);
int attiva_timer_initSimbt (Widget);
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
	Widget	Uxform4;
	Widget	Uxframe3;
	Widget	Uxform3;
	Widget	UxinitQuitpb;
	Widget	UxinitViewpb;
	Widget	Uxmenu1;
	Widget	UxinitMenu;
	Widget	UxinitMenuExitpb;
	Widget	Uxmenu1_top_b2;
	Widget	UxscrolledWindow3;
	Widget	UxscrollListaSnap;
	Widget	UxlistaSnapPopupMenu;
	Widget	UxlistaSnapPopupView;
	int	Uxitem_select;
	Widget	Uxpadre_init;
	int	Uxtipo;
} _UxCinitSimulator;

static _UxCinitSimulator       *UxInitSimulatorContext;
#define form4                   UxInitSimulatorContext->Uxform4
#define frame3                  UxInitSimulatorContext->Uxframe3
#define form3                   UxInitSimulatorContext->Uxform3
#define initQuitpb              UxInitSimulatorContext->UxinitQuitpb
#define initViewpb              UxInitSimulatorContext->UxinitViewpb
#define menu1                   UxInitSimulatorContext->Uxmenu1
#define initMenu                UxInitSimulatorContext->UxinitMenu
#define initMenuExitpb          UxInitSimulatorContext->UxinitMenuExitpb
#define menu1_top_b2            UxInitSimulatorContext->Uxmenu1_top_b2
#define scrolledWindow3         UxInitSimulatorContext->UxscrolledWindow3
#define scrollListaSnap         UxInitSimulatorContext->UxscrollListaSnap
#define listaSnapPopupMenu      UxInitSimulatorContext->UxlistaSnapPopupMenu
#define listaSnapPopupView      UxInitSimulatorContext->UxlistaSnapPopupView
#define item_select             UxInitSimulatorContext->Uxitem_select
#define padre_init              UxInitSimulatorContext->Uxpadre_init
#define tipo                    UxInitSimulatorContext->Uxtipo



int carica_ci (Widget,int);
int init_from_bt (Widget,int);


/*******************************************************************************
       The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxinitSimulatorMenuPost( wgt, client_data, event, ctd )
	Widget		wgt;
	XtPointer	client_data;
	XEvent		*event;
    int ctd;
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

Widget	initSimulator;
Widget	initLoadpb;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_initSimulator();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

int carica_lista_bt ()
{
#ifndef DESIGN_TIME
      read_lista_bt (bt_list);
      display_lista_bt (UxGetWidget(scrollListaSnap), bt_list);
#endif
      return (0);   
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_initLoadpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitSimulator       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitSimulatorContext;
	UxInitSimulatorContext = UxContext =
			(_UxCinitSimulator *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME 
	switch (tipo)
	{
	   case INIT_FROM_CI:
	      printf ("load snap %d\n", item_select);
	      if (carica_ci ((Widget)UxClientData,item_select) == 0)
	         {
	         fprintf (stderr,"snapshot %d caricato\n",item_select);
	         inizializzazione = 1;
	         }
	      else
	         {
	         fprintf (stderr,"***snapshot %d non caricato\n",item_select);
	         }	
	      break;
	   case INIT_FROM_BT:
	      printf ("load bt %d\n", item_select);
	      if (init_from_bt ((Widget)UxClientData,
	                      punt_rec_backtrack[item_select]+1) == 0)
	         {
	         fprintf (stderr,"backtrack %d caricato\n",item_select);
	         inizializzazione = 1;
	         }
	      else
	         {
	         fprintf (stderr,"***backtrack %d non caricato\n",item_select);
	         }	      
	      break;
	   default:
	      printf ("non riconosciuta\n");
	      break;
	}
#endif
	
	
	}
	UxInitSimulatorContext = UxSaveCtx;
}

static	void	activateCB_initQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitSimulator       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitSimulatorContext;
	UxInitSimulatorContext = UxContext =
			(_UxCinitSimulator *) UxGetContext( UxWidget );
	{
	printf ("fine colloquio initialization \n");
#ifndef DESIGN_TIME
	if (tipo == INIT_FROM_CI)
	   {
	   init_ci_interface_active = 0;
	   UxDestroyInterface (initSimCi);
	   free (snap_list); 
	   stop_timer (timer_initci); 
	   }
	else if (tipo == INIT_FROM_BT)
	   {
	   init_bt_interface_active = 0;
	   UxDestroyInterface (initSimBt);
	   free (bt_list);
	   stop_timer (timer_initbt);
	   if (backtrack_caricato)  /* salvata la ci di ritorno */
	      if (!bt_interface_active) /* non esiste bt mngmt */
	         backtrack_caricato = 0;
	   }
#endif
	}
	UxInitSimulatorContext = UxSaveCtx;
}

static	void	activateCB_initViewpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitSimulator       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitSimulatorContext;
	UxInitSimulatorContext = UxContext =
			(_UxCinitSimulator *) UxGetContext( UxWidget );
	{
	char testo[20];
#ifndef DESIGN_TIME
	if (tipo == INIT_FROM_CI)
	   {    
	   printf ("preview ci %d\n",item_select);
	   sprintf (testo,"I.C. %d",item_select);
	   create_previewSnap(CI_VIEW,item_select,testo);
	   }
	else if (tipo == INIT_FROM_BT)
	   {
	   printf ("preview bt %d rec n. %d\n",
	      item_select,punt_rec_backtrack[item_select]+1);
	   sprintf (testo,"BT %d",item_select);
	   create_previewSnap(BT_VIEW,punt_rec_backtrack[item_select]+1,testo);
	   }
#endif
	
	
	}
	UxInitSimulatorContext = UxSaveCtx;
}

static	void	activateCB_initMenuExitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitSimulator       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitSimulatorContext;
	UxInitSimulatorContext = UxContext =
			(_UxCinitSimulator *) UxGetContext( UxWidget );
	{
	printf ("fine colloquio init\n");
#ifndef DESIGN_TIME
	if (tipo == INIT_FROM_CI)
	   {
	   init_ci_interface_active = 0;
	   UxDestroyInterface (initSimCi);
	   stop_timer (timer_initci); 
	   }
	else if (tipo == INIT_FROM_BT)
	   {
	   init_bt_interface_active = 0;
	   UxDestroyInterface (initSimBt);
	   stop_timer (timer_initbt);
	   if (backtrack_caricato)  /* salvata la ci di ritorno */
	      if (!bt_interface_active) /* non esiste bt mngmt */
	         backtrack_caricato = 0;
	   }
#endif
	}
	UxInitSimulatorContext = UxSaveCtx;
}

static	void	singleSelectionCB_scrollListaSnap( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitSimulator       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitSimulatorContext;
	UxInitSimulatorContext = UxContext =
			(_UxCinitSimulator *) UxGetContext( UxWidget );
	{
	int n; 
	XmListCallbackStruct *reason;
	reason = (XmListCallbackStruct *)UxCallbackArg;
	item_select = reason->item_position;
	
	printf ("lista: item_select = %d\n",item_select);
	
#ifndef DESIGN_TIME
	
	switch (tipo)
	   {
	   case INIT_FROM_CI:
	      n = item_select - 1;
	      if ((snap_list+n)->stat == 1)   
	         {
	         XtVaSetValues (initViewpb, XmNsensitive, True, NULL);
	         XtVaSetValues (listaSnapPopupView,XmNsensitive, True, NULL); 
	         tasto_loadci_attivabile = True;
	         }
	      else
	         {
	         XtVaSetValues (initViewpb, XmNsensitive, False, NULL);
	         XtVaSetValues (listaSnapPopupView,XmNsensitive, False, NULL);
	         tasto_loadci_attivabile = False;
	         }	
	      break;
	
	   case INIT_FROM_BT:
	      n = item_select;
	      if ((bt_list+punt_rec_backtrack[n])->stat == 1)
	         {
	         XtVaSetValues (initViewpb, XmNsensitive, True, NULL);
	         XtVaSetValues (listaSnapPopupView,XmNsensitive, True, NULL); 
	         tasto_loadbt_attivabile = True;
	         }
	     else
	         {
	         XtVaSetValues (initViewpb, XmNsensitive, False, NULL);
	         XtVaSetValues (listaSnapPopupView,XmNsensitive, False, NULL);
	         tasto_loadbt_attivabile = False;
	         } 
	      break;
	   }
#endif
	}
	UxInitSimulatorContext = UxSaveCtx;
}

static	void	activateCB_listaSnapPopupView( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitSimulator       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitSimulatorContext;
	UxInitSimulatorContext = UxContext =
			(_UxCinitSimulator *) UxGetContext( UxWidget );
	{
	char testo[20];
#ifndef DESIGN_TIME
	if (tipo == INIT_FROM_CI)
	   {    
	   sprintf (testo,"I.C. %d",item_select);
	   create_previewSnap(CI_VIEW,item_select,testo);
	   }
	else if (tipo == INIT_FROM_BT)
	   {
	   sprintf (testo,"BT %d",item_select);
	   create_previewSnap(BT_VIEW,punt_rec_backtrack[item_select]+1,testo);
	   }
#endif
	}
	UxInitSimulatorContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_initSimulator()
{
	Widget		_UxParent;
	Widget		initMenu_shell;
	Widget		listaSnapPopupMenu_shell;


	/* Creation of initSimulator */
	_UxParent = XtVaCreatePopupShell( "initSimulator_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 100,
			XmNy, 100,
			XmNwidth, 700,
			XmNheight, 450,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "initSimulator",
			XmNiconName, "initSimulator",
			NULL );

	initSimulator = XtVaCreateManagedWidget( "initSimulator",
			xmFormWidgetClass,
			_UxParent,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNunitType, XmPIXELS,
			XmNwidth, 700,
			XmNheight, 450,
			XmNdefaultPosition, FALSE,
			NULL );
	UxPutContext( initSimulator, (char *) UxInitSimulatorContext );


	/* Creation of form4 */
	form4 = XtVaCreateManagedWidget( "form4",
			xmFormWidgetClass,
			initSimulator,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 5,
			XmNy, 260,
			XmNwidth, 855,
			XmNheight, 75,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( form4, (char *) UxInitSimulatorContext );


	/* Creation of frame3 */
	frame3 = XtVaCreateManagedWidget( "frame3",
			xmFrameWidgetClass,
			form4,
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
	UxPutContext( frame3, (char *) UxInitSimulatorContext );


	/* Creation of form3 */
	form3 = XtVaCreateManagedWidget( "form3",
			xmFormWidgetClass,
			frame3,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 745,
			XmNheight, 75,
			NULL );
	UxPutContext( form3, (char *) UxInitSimulatorContext );


	/* Creation of initLoadpb */
	initLoadpb = XtVaCreateManagedWidget( "initLoadpb",
			xmPushButtonWidgetClass,
			form3,
			XmNx, 20,
			XmNy, 15,
			XmNwidth, 130,
			XmNheight, 45,
			RES_CONVERT( XmNlabelString, "LOAD" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( initLoadpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_initLoadpb,
		initSimulator );

	UxPutContext( initLoadpb, (char *) UxInitSimulatorContext );


	/* Creation of initQuitpb */
	initQuitpb = XtVaCreateManagedWidget( "initQuitpb",
			xmPushButtonWidgetClass,
			form3,
			XmNx, 510,
			XmNy, 15,
			XmNwidth, 130,
			XmNheight, 45,
			RES_CONVERT( XmNlabelString, "QUIT" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNrightPosition, 0,
			XmNleftPosition, 80,
			NULL );
	XtAddCallback( initQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_initQuitpb,
		(XtPointer) UxInitSimulatorContext );

	UxPutContext( initQuitpb, (char *) UxInitSimulatorContext );


	/* Creation of initViewpb */
	initViewpb = XtVaCreateManagedWidget( "initViewpb",
			xmPushButtonWidgetClass,
			form3,
			XmNx, 170,
			XmNy, 15,
			XmNwidth, 130,
			XmNheight, 45,
			RES_CONVERT( XmNlabelString, "VIEW" ),
			XmNsensitive, FALSE,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 20,
			XmNleftWidget, initLoadpb,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 40,
			NULL );
	XtAddCallback( initViewpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_initViewpb,
		(XtPointer) UxInitSimulatorContext );

	UxPutContext( initViewpb, (char *) UxInitSimulatorContext );


	/* Creation of menu1 */
	menu1 = XtVaCreateManagedWidget( "menu1",
			xmRowColumnWidgetClass,
			initSimulator,
			XmNrowColumnType, XmMENU_BAR,
			XmNrightAttachment, XmATTACH_FORM,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( menu1, (char *) UxInitSimulatorContext );


	/* Creation of initMenu */
	initMenu_shell = XtVaCreatePopupShell ("initMenu_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	initMenu = XtVaCreateWidget( "initMenu",
			xmRowColumnWidgetClass,
			initMenu_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( initMenu, (char *) UxInitSimulatorContext );


	/* Creation of initMenuExitpb */
	initMenuExitpb = XtVaCreateManagedWidget( "initMenuExitpb",
			xmPushButtonWidgetClass,
			initMenu,
			RES_CONVERT( XmNlabelString, "Exit" ),
			RES_CONVERT( XmNmnemonic, "E" ),
			NULL );
	XtAddCallback( initMenuExitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_initMenuExitpb,
		(XtPointer) UxInitSimulatorContext );

	UxPutContext( initMenuExitpb, (char *) UxInitSimulatorContext );


	/* Creation of menu1_top_b2 */
	menu1_top_b2 = XtVaCreateManagedWidget( "menu1_top_b2",
			xmCascadeButtonWidgetClass,
			menu1,
			RES_CONVERT( XmNlabelString, "File" ),
			RES_CONVERT( XmNmnemonic, "F" ),
			XmNsubMenuId, initMenu,
			NULL );
	UxPutContext( menu1_top_b2, (char *) UxInitSimulatorContext );


	/* Creation of scrolledWindow3 */
	scrolledWindow3 = XtVaCreateManagedWidget( "scrolledWindow3",
			xmScrolledWindowWidgetClass,
			initSimulator,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 20,
			XmNy, 105,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 20,
			XmNbottomWidget, form4,
			XmNheight, 250,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 20,
			XmNtopWidget, menu1,
			NULL );
	UxPutContext( scrolledWindow3, (char *) UxInitSimulatorContext );


	/* Creation of scrollListaSnap */
	scrollListaSnap = XtVaCreateManagedWidget( "scrollListaSnap",
			xmListWidgetClass,
			scrolledWindow3,
			XmNwidth, 660,
			XmNheight, 300,
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNvisibleItemCount, 15,
			NULL );
	XtAddCallback( scrollListaSnap, XmNsingleSelectionCallback,
		(XtCallbackProc) singleSelectionCB_scrollListaSnap,
		(XtPointer) UxInitSimulatorContext );

	UxPutContext( scrollListaSnap, (char *) UxInitSimulatorContext );


	/* Creation of listaSnapPopupMenu */
	listaSnapPopupMenu_shell = XtVaCreatePopupShell ("listaSnapPopupMenu_shell",
			xmMenuShellWidgetClass, scrollListaSnap,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	listaSnapPopupMenu = XtVaCreateWidget( "listaSnapPopupMenu",
			xmRowColumnWidgetClass,
			listaSnapPopupMenu_shell,
			XmNrowColumnType, XmMENU_POPUP,
			XmNmenuAccelerator, "Shift <KeyUp>F10",
			NULL );
	UxPutContext( listaSnapPopupMenu, (char *) UxInitSimulatorContext );


	/* Creation of listaSnapPopupView */
	listaSnapPopupView = XtVaCreateManagedWidget( "listaSnapPopupView",
			xmPushButtonWidgetClass,
			listaSnapPopupMenu,
			RES_CONVERT( XmNlabelString, "View   " ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( listaSnapPopupView, XmNactivateCallback,
		(XtCallbackProc) activateCB_listaSnapPopupView,
		(XtPointer) UxInitSimulatorContext );

	UxPutContext( listaSnapPopupView, (char *) UxInitSimulatorContext );


	XtAddCallback( initSimulator, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxInitSimulatorContext);

	XtAddEventHandler(scrollListaSnap, ButtonPressMask,
			False, (XtEventHandler) _UxinitSimulatorMenuPost, (XtPointer) listaSnapPopupMenu );

	return ( initSimulator );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_initSimulator( _Uxpadre_init, _Uxtipo )
	Widget	_Uxpadre_init;
	int	_Uxtipo;
{
	Widget                  rtrn;
	_UxCinitSimulator       *UxContext;

	UxInitSimulatorContext = UxContext =
		(_UxCinitSimulator *) UxNewContext( sizeof(_UxCinitSimulator), False );

	padre_init = _Uxpadre_init;
	tipo = _Uxtipo;

	{
#ifndef DESIGN_TIME
		set_cursor (padre_init,CLOCK);
#endif
		rtrn = _Uxbuild_initSimulator();

#ifndef DESIGN_TIME
		XtAddEventHandler (UxGetWidget(scrollListaSnap), ButtonPressMask, False, 
		                     Popup_Menu, listaSnapPopupMenu);
		/*
		 * per commutazione a backtrack e per snapshot
		 */
		snap_list = (SNTAB*)malloc(sizeof(SNTAB)*_MAX_SNAP_SHOT);
		read_lista_snap (snap_list);
		
		switch (tipo)
		   {
		   case INIT_FROM_CI:
		      init_ci_interface_active = 1; 
		      initSimCi = rtrn;
		      XtVaSetValues (XtParent(UxGetWidget(rtrn)), 
		         XmNtitle, "INIT CI", XmNiconName, "INIT CI", NULL);
		      display_lista_snap (UxGetWidget(scrollListaSnap), snap_list);
		      tasto_load_ci = initLoadpb;
		      tasto_loadci_attivabile = False;
		      attiva_timer_initSimci (initSimCi);
		      break;
		   case INIT_FROM_BT:
		      init_bt_interface_active = 1; 
		      initSimBt = rtrn;
		      XtVaSetValues (XtParent(UxGetWidget(rtrn)), 
		         XmNtitle, "INIT BT",XmNiconName, "INIT BT",NULL);
		      bt_list = (BKTAB*)malloc(sizeof(BKTAB)*_MAX_BACK_TRACK);
		      read_lista_bt (bt_list);
		      display_lista_bt (UxGetWidget(scrollListaSnap), bt_list);
		      tasto_load_bt = initLoadpb;
		      tasto_loadbt_attivabile = False;
		      attiva_timer_initSimbt (initSimBt);
		      break;	
		   }
		set_cursor(padre_init,NORMALE);
#endif
		
		UxPopupInterface (rtrn, no_grab);
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

