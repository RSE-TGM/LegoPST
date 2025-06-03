
/*******************************************************************************
       backtrackManagement.c
       (Generated from interface file backtrackManagement.i)
*******************************************************************************/

#include <stdio.h>

#ifdef MOTIF
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#endif /* MOTIF */

#include "UxXt.h"
#include <Xm/ScrolledW.h>
#include <Xm/Label.h>
#include <Xm/BulletinB.h>
#include <Xm/CascadeB.h>
#include <Xm/Separator.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "bistrutt.h"
#ifndef DESIGN_TIME
#include "parametri.h" 
#include "preview.h"
#endif

extern int _MAX_BACK_TRACK;
extern int _MAX_SNAP_SHOT;
extern int freeze_rest;

#include "backtrack.h"
extern int bt_interface_active;
#include "cursore.h"
#include "sked.h"
#include "messaggi.h"
#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"
BKTAB *bt_header;
SNTAB *snap_header;
extern int inizializzazione;
int backtrack_caricato;
extern swidget masterMenu;
XtIntervalId timer_replayOn;
extern int stato_sim;
int prec_sel;
/* flag interblocco tasti INIT/LOAD dal nome del testatore */
int DragonFlag;


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
	Widget	Uxframe15;
	Widget	Uxform22;
	Widget	UxbtQuitpb;
	Widget	UxbtLoadpb;
	Widget	UxbtInitpb;
	Widget	UxbtViewpb;
	Widget	UxbtMenu;
	Widget	UxbtMenuFile;
	Widget	UxbtMenuFilepb;
	Widget	Uxbt_menu_file;
	Widget	UxbtMenuReplay;
	Widget	UxbtMenuReplayOntb;
	Widget	UxbtMenuReplay_b3;
	Widget	UxbtMenuReplayOfftb;
	Widget	Uxbt_menu_replay;
	Widget	UxbtMenuModi;
	Widget	UxbtMenuModiAvind;
	Widget	UxbtMenuModiAvindForwtb;
	Widget	UxbtMenuModiAvindBacktb;
	Widget	UxbtMenuModiFbcb;
	Widget	UxbtMenuModiAutoman;
	Widget	UxbtMenuModiAutomanAutotb;
	Widget	UxbtMenuModiAutomanMan;
	Widget	UxbtMenuModiAmcb;
	Widget	Uxbt_menu_mode;
	Widget	UxbtMenuEdit;
	Widget	UxbtMenuEditRecpb;
	Widget	UxbtMenuEditPrespb;
	Widget	Uxbt_menu_edit;
	Widget	UxbtMenuHelp;
	Widget	UxbtMenuHelppb;
	Widget	Uxbt_menu_help;
	Widget	UxbulletinBoard2;
	Widget	UxreplaySelectionbt;
	Widget	UxreplaySelectionLabel;
	Widget	UxbtLoadLabel;
	Widget	UxbtLoadNumber;
	Widget	UxbacktrackScrollW;
	Widget	UxBtRc;
	Widget	Uxmenu5;
	Widget	UxBtRcPopupView;
	Widget	Uxmenu5_p1_b5;
	Widget	UxBtRcPopupLoad;
	Widget	UxBtRcPopupInit;
	Widget	Uxmenu5_p1_b6;
	Widget	UxBtRcPopupQuit;
	Widget	Uxpadre_bt;
} _UxCbacktrackManagement;

static _UxCbacktrackManagement *UxBacktrackManagementContext;
#define frame15                 UxBacktrackManagementContext->Uxframe15
#define form22                  UxBacktrackManagementContext->Uxform22
#define btQuitpb                UxBacktrackManagementContext->UxbtQuitpb
#define btLoadpb                UxBacktrackManagementContext->UxbtLoadpb
#define btInitpb                UxBacktrackManagementContext->UxbtInitpb
#define btViewpb                UxBacktrackManagementContext->UxbtViewpb
#define btMenu                  UxBacktrackManagementContext->UxbtMenu
#define btMenuFile              UxBacktrackManagementContext->UxbtMenuFile
#define btMenuFilepb            UxBacktrackManagementContext->UxbtMenuFilepb
#define bt_menu_file            UxBacktrackManagementContext->Uxbt_menu_file
#define btMenuReplay            UxBacktrackManagementContext->UxbtMenuReplay
#define btMenuReplayOntb        UxBacktrackManagementContext->UxbtMenuReplayOntb
#define btMenuReplay_b3         UxBacktrackManagementContext->UxbtMenuReplay_b3
#define btMenuReplayOfftb       UxBacktrackManagementContext->UxbtMenuReplayOfftb
#define bt_menu_replay          UxBacktrackManagementContext->Uxbt_menu_replay
#define btMenuModi              UxBacktrackManagementContext->UxbtMenuModi
#define btMenuModiAvind         UxBacktrackManagementContext->UxbtMenuModiAvind
#define btMenuModiAvindForwtb   UxBacktrackManagementContext->UxbtMenuModiAvindForwtb
#define btMenuModiAvindBacktb   UxBacktrackManagementContext->UxbtMenuModiAvindBacktb
#define btMenuModiFbcb          UxBacktrackManagementContext->UxbtMenuModiFbcb
#define btMenuModiAutoman       UxBacktrackManagementContext->UxbtMenuModiAutoman
#define btMenuModiAutomanAutotb UxBacktrackManagementContext->UxbtMenuModiAutomanAutotb
#define btMenuModiAutomanMan    UxBacktrackManagementContext->UxbtMenuModiAutomanMan
#define btMenuModiAmcb          UxBacktrackManagementContext->UxbtMenuModiAmcb
#define bt_menu_mode            UxBacktrackManagementContext->Uxbt_menu_mode
#define btMenuEdit              UxBacktrackManagementContext->UxbtMenuEdit
#define btMenuEditRecpb         UxBacktrackManagementContext->UxbtMenuEditRecpb
#define btMenuEditPrespb        UxBacktrackManagementContext->UxbtMenuEditPrespb
#define bt_menu_edit            UxBacktrackManagementContext->Uxbt_menu_edit
#define btMenuHelp              UxBacktrackManagementContext->UxbtMenuHelp
#define btMenuHelppb            UxBacktrackManagementContext->UxbtMenuHelppb
#define bt_menu_help            UxBacktrackManagementContext->Uxbt_menu_help
#define bulletinBoard2          UxBacktrackManagementContext->UxbulletinBoard2
#define replaySelectionbt       UxBacktrackManagementContext->UxreplaySelectionbt
#define replaySelectionLabel    UxBacktrackManagementContext->UxreplaySelectionLabel
#define btLoadLabel             UxBacktrackManagementContext->UxbtLoadLabel
#define btLoadNumber            UxBacktrackManagementContext->UxbtLoadNumber
#define backtrackScrollW        UxBacktrackManagementContext->UxbacktrackScrollW
#define BtRc                    UxBacktrackManagementContext->UxBtRc
#define menu5                   UxBacktrackManagementContext->Uxmenu5
#define BtRcPopupView           UxBacktrackManagementContext->UxBtRcPopupView
#define menu5_p1_b5             UxBacktrackManagementContext->Uxmenu5_p1_b5
#define BtRcPopupLoad           UxBacktrackManagementContext->UxBtRcPopupLoad
#define BtRcPopupInit           UxBacktrackManagementContext->UxBtRcPopupInit
#define menu5_p1_b6             UxBacktrackManagementContext->Uxmenu5_p1_b6
#define BtRcPopupQuit           UxBacktrackManagementContext->UxBtRcPopupQuit
#define padre_bt                UxBacktrackManagementContext->Uxpadre_bt


/*******************************************************************************
       The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxbacktrackManagementMenuPost( wgt, client_data, event, ctd )
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

Widget	backtrackManagement;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_backtrackManagement();
int set_colore_riga_bt (int, int);
int read_lista_bt (BKTAB *);
int display_header_bt (Widget,BKTAB *);
void uscita_backtrack (Widget);
int abilita_replay_mode(Widget);
int update_btload (Widget,int);
int btload_ok (Widget);
int richiesta_timer_bt (Widget);
int AllocaAreeBacktrack();



/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/******************************************************************/
void BtSelActivate (parent, dati, reason)
Widget parent;
char *dati;
XmAnyCallbackStruct *reason;
{
int tipo_entry;
/* messo come var. globale per azzeramento da clear del tempo
static int prec_sel = 0;
**/
   
#ifndef DESIGN_TIME   
/*   bt_sel = atoi(dati); */  /* da 1 a n  */

   bt_punt = atoi(dati); /* da 1 a n  (numero di item (riga))*/	
   tipo_entry = reason->reason;

   switch (tipo_entry)
      {
      case XmCR_FOCUS:
	if ((prec_sel>0)&&(prec_sel!=bt_punt)&&(prec_sel!=bt_sel))  
	   set_colore_riga_bt(prec_sel-1, background_backtrack); 
        if (bt_punt != bt_sel)
           set_colore_riga_bt(bt_punt-1, colore_app[5]);  
	break;       	   
      case XmCR_LOSING_FOCUS:
         break;
      case XmCR_ACTIVATE:
	if ((prec_sel>0)&&(prec_sel!=bt_punt)&&(prec_sel!=bt_sel))  
	   set_colore_riga_bt(prec_sel-1, background_backtrack); 
        if (bt_punt != bt_sel)
           set_colore_riga_bt(bt_punt-1, colore_app[5]); 
         break;
      default:
         break; 
      }
/***   
   printf ("selezionato bt %d record n. %d\n",
              bt_sel, punt_rec_bt[bt_sel]);  
   printf ("puntato bt %d record %d\n",
              bt_punt, punt_rec_bt[bt_punt]);
***/
   if ((bt_header+punt_rec_bt[bt_punt])->stat == 1)  /* snap occupato  */
      {
      XtVaSetValues (UxGetWidget(btLoadpb), XmNsensitive, True, NULL);
      XtVaSetValues (UxGetWidget(btViewpb), XmNsensitive, True, NULL);
      XtVaSetValues (UxGetWidget(BtRcPopupLoad), XmNsensitive, True, NULL);
      XtVaSetValues (UxGetWidget(BtRcPopupView), XmNsensitive, True, NULL);
      }
   else
      {
      /* printf ("record %d non occupato\n",n); */
      XtVaSetValues (UxGetWidget(btLoadpb), XmNsensitive, False, NULL);
      XtVaSetValues (UxGetWidget(btViewpb), XmNsensitive, False, NULL);
      XtVaSetValues (UxGetWidget(BtRcPopupLoad), XmNsensitive, False, NULL);
      XtVaSetValues (UxGetWidget(BtRcPopupView), XmNsensitive, False, NULL);
      }

   prec_sel = bt_punt;

#endif 

   return;
}
/******************************************************************/
int carica_header_bt ()
{
#ifndef DESIGN_TIME
   read_lista_bt (bt_header);
   display_header_bt (UxGetWidget(BtRc),bt_header);
#endif
return (0);
}

/******************************************************************/
int exit_replay()
{

   replay_mode (UxGetWidget(backtrackManagement), False);
   uscita_backtrack(UxGetWidget(backtrackManagement));

}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void  destroyCB_backtrackManagement( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
	bt_interface_active = 0;
	free (snap_header);
	free (bt_header);
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  activateCB_btQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	/*
	 * uscita per annullamento backtrack
	 */
	
	printf("QUIT bktk_man: freeze_rest = %d\n",freeze_rest);
	
	if (stato_sim == STATO_REPLAY) {
	   replay_mode (UxWidget,False); /* termina replay */
	}
	uscita_backtrack ((Widget)UxClientData);
#endif
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  activateCB_btLoadpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	DragonFlag = 0;   /* interblocco tasto init */
	if (bt_sel > 0)
	   set_colore_riga_bt(bt_sel-1,background_backtrack);
	bt_sel = bt_punt;
	/* disabilito i tasti init ( se attivati prima del tempo danno problemi 
	   (non va in freeze)
	 */
	XtVaSetValues (UxGetWidget(btInitpb),XmNsensitive,False, NULL);
	XtVaSetValues (UxGetWidget(BtRcPopupInit), XmNsensitive,False, NULL);
	
	if (!carica_bt ((Widget)UxClientData, punt_rec_bt[bt_sel]+1))
	   {
	   set_colore_riga_bt(bt_sel-1, colore_app[0]);
	   abilita_replay_mode (UxWidget);
	   update_btload (UxWidget,bt_sel);
	   XtVaSetValues (UxGetWidget(btInitpb),XmNsensitive,True,NULL);
	   XtVaSetValues (UxGetWidget(BtRcPopupInit), XmNsensitive,    True,  NULL);
	   }
	else
	   {
	   XtVaSetValues (UxGetWidget(btInitpb),XmNsensitive,False,NULL);
	   XtVaSetValues (UxGetWidget(BtRcPopupInit), XmNsensitive,False, NULL);
	   }
	DragonFlag = 1;   	
#endif
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  activateCB_btInitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	if (DragonFlag == 1)
	   {
	   btload_ok ((Widget)UxClientData);
	   switch_to_freeze((Widget)UxClientData);
	   bt_interface_active = 0;
	   DistruggiInterfaccia (XtParent((Widget)UxClientData));
	   }
#endif
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  activateCB_btViewpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
	char testo[20];
#ifndef DESIGN_TIME
	/**
	sprintf (testo,"BT %d",bt_sel);
	printf ("preview bt n. %d \n",punt_rec_bt[bt_sel]+1);
	create_previewSnap (BT_VIEW, punt_rec_bt[bt_sel]+1, testo);
	**/
	sprintf (testo,"BT %d",bt_punt);
	printf ("preview bt n. %d \n",punt_rec_bt[bt_punt]+1);
	create_previewSnap (BT_VIEW, punt_rec_bt[bt_punt]+1, testo);
#endif
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  activateCB_btMenuFilepb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
	uscita_backtrack (UxGetWidget(backtrackManagement));
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  valueChangedCB_btMenuReplayOntb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
	printf ("replay on\n");
#ifndef DESIGN_TIME
	SD_replay (BANCO);
#endif
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  valueChangedCB_btMenuReplayOfftb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
	printf ("replay off\n");
#ifndef DESIGN_TIME
	SD_backtrack (BANCO);
#endif
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  valueChangedCB_btMenuModiAvindForwtb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{printf ("direzione avanti\n");}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  valueChangedCB_btMenuModiAvindBacktb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{printf ("direzione indietro\n");}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  valueChangedCB_btMenuModiAutomanAutotb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{printf ("backtrack auto\n");}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  valueChangedCB_btMenuModiAutomanMan( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{printf ("backtrack manuale\n");}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  activateCB_btMenuEditRecpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
	richiesta_timer_bt (backtrackManagement);
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  activateCB_btMenuEditPrespb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{printf ("richiesta timer auto\n");}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  activateCB_btMenuHelppb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{printf ("help comandi\n");}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  valueChangedCB_replaySelectionbt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
	Boolean stato;
	stato = ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set;
	replay_mode (UxWidget, stato);
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  activateCB_BtRcPopupView( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
	char testo[20];
#ifndef DESIGN_TIME
	/**
	printf ("preview bt n. %d (popup)\n",bt_sel);
	sprintf (testo,"BT %d",bt_sel);
	create_previewSnap(BT_VIEW,punt_rec_bt[bt_sel]+1,testo);
	**/
	printf ("preview bt n. %d (popup)\n",bt_punt);
	sprintf (testo,"BT %d",bt_punt);
	create_previewSnap(BT_VIEW,punt_rec_bt[bt_punt]+1,testo);
#endif
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  activateCB_BtRcPopupLoad( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	DragonFlag = 0;   /* interblocco tasto init */
	if (bt_sel > 0)
	   set_colore_riga_bt(bt_sel-1,background_backtrack);
	bt_sel = bt_punt;
	if (!carica_bt (UxGetWidget(backtrackManagement), punt_rec_bt[bt_sel]+1))
	   {
	   set_colore_riga_bt(bt_sel-1, colore_app[0]);
	   abilita_replay_mode (UxWidget);
	   update_btload (UxWidget,bt_sel);
	   XtVaSetValues (UxGetWidget(btInitpb), XmNsensitive,    True,  NULL);
	   XtVaSetValues (UxGetWidget(BtRcPopupInit), XmNsensitive,    True,  NULL);
	   }
	else
	   {
	   XtVaSetValues (UxGetWidget(btInitpb), XmNsensitive,    False, NULL);
	   XtVaSetValues (UxGetWidget(BtRcPopupInit), XmNsensitive,    False, NULL);
	   }
	DragonFlag = 1;   	   	
#endif
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  activateCB_BtRcPopupInit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	if (DragonFlag == 1)
	   {
	   btload_ok ((Widget)UxClientData);
	   switch_to_freeze((Widget)UxClientData);
	   bt_interface_active = 0;
	   DistruggiInterfaccia (XtParent((Widget)UxClientData));
	   }
#endif
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

static void  activateCB_BtRcPopupQuit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCbacktrackManagement *UxSaveCtx, *UxContext;

	UxSaveCtx = UxBacktrackManagementContext;
	UxBacktrackManagementContext = UxContext =
			(_UxCbacktrackManagement *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	/*
	 * uscita per annullamento backtrack
	 */
	if (stato_sim == STATO_REPLAY)
	   replay_mode (UxWidget,False); /* termina replay */
	uscita_backtrack (UxGetWidget(backtrackManagement));
#endif
	}
	UxBacktrackManagementContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_backtrackManagement()
{
	Widget		_UxParent;
	Widget		btMenuFile_shell;
	Widget		btMenuReplay_shell;
	Widget		btMenuModi_shell;
	Widget		btMenuModiAvind_shell;
	Widget		btMenuModiAutoman_shell;
	Widget		btMenuEdit_shell;
	Widget		btMenuHelp_shell;
	Widget		menu5_shell;
	char		*UxTmp0;


	/* Creation of backtrackManagement */
	_UxParent = XtVaCreatePopupShell( "backtrackManagement_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 369,
			XmNy, 175,
			XmNwidth, 400,
			XmNheight, 602,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "backtrackManagement",
			NULL );

	backtrackManagement = XtVaCreateWidget( "backtrackManagement",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 400,
			XmNheight, 602,
			RES_CONVERT( XmNdialogTitle, "BACKTRACK MANAGEMENT" ),
			NULL );
	XtAddCallback( backtrackManagement, XmNdestroyCallback,
		(XtCallbackProc) destroyCB_backtrackManagement,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( backtrackManagement, (char *) UxBacktrackManagementContext );


	/* Creation of frame15 */
	frame15 = XtVaCreateManagedWidget( "frame15",
			xmFrameWidgetClass,
			backtrackManagement,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 400,
			XmNheight, 80,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame15, (char *) UxBacktrackManagementContext );


	/* Creation of form22 */
	form22 = XtVaCreateManagedWidget( "form22",
			xmFormWidgetClass,
			frame15,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 2,
			XmNy, 20,
			XmNwidth, 500,
			XmNheight, 82,
			NULL );
	UxPutContext( form22, (char *) UxBacktrackManagementContext );


	/* Creation of btQuitpb */
	btQuitpb = XtVaCreateManagedWidget( "btQuitpb",
			xmPushButtonWidgetClass,
			form22,
			XmNx, 715,
			XmNy, 15,
			XmNwidth, 100,
			XmNheight, 52,
			RES_CONVERT( XmNlabelString, QUITLABEL ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNbottomOffset, 15,
			XmNleftPosition, 80,
			NULL );
	XtAddCallback( btQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_btQuitpb,
		UxGetWidget(backtrackManagement) );

	UxPutContext( btQuitpb, (char *) UxBacktrackManagementContext );


	/* Creation of btLoadpb */
	btLoadpb = XtVaCreateManagedWidget( "btLoadpb",
			xmPushButtonWidgetClass,
			form22,
			XmNx, 20,
			XmNy, 15,
			XmNwidth, 100,
			XmNheight, 52,
			RES_CONVERT( XmNlabelString, LOADLABEL ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 20,
			XmNrightOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( btLoadpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_btLoadpb,
		backtrackManagement );

	UxPutContext( btLoadpb, (char *) UxBacktrackManagementContext );


	/* Creation of btInitpb */
	btInitpb = XtVaCreateManagedWidget( "btInitpb",
			xmPushButtonWidgetClass,
			form22,
			XmNx, 148,
			XmNy, 15,
			XmNwidth, 100,
			XmNheight, 52,
			RES_CONVERT( XmNlabelString, INITLABEL ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 20,
			XmNleftWidget, btLoadpb,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNsensitive, FALSE,
			XmNrightPosition, 40,
			NULL );
	XtAddCallback( btInitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_btInitpb,
		UxGetWidget(backtrackManagement) );

	UxPutContext( btInitpb, (char *) UxBacktrackManagementContext );


	/* Creation of btViewpb */
	btViewpb = XtVaCreateManagedWidget( "btViewpb",
			xmPushButtonWidgetClass,
			form22,
			XmNx, 275,
			XmNy, 15,
			XmNwidth, 100,
			XmNheight, 52,
			RES_CONVERT( XmNlabelString, VIEWLABEL ),
			XmNsensitive, FALSE,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 20,
			XmNleftWidget, btInitpb,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNrightPosition, 60,
			NULL );
	XtAddCallback( btViewpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_btViewpb,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( btViewpb, (char *) UxBacktrackManagementContext );


	/* Creation of btMenu */
	btMenu = XtVaCreateManagedWidget( "btMenu",
			xmRowColumnWidgetClass,
			backtrackManagement,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( btMenu, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuFile */
	btMenuFile_shell = XtVaCreatePopupShell ("btMenuFile_shell",
			xmMenuShellWidgetClass, btMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	btMenuFile = XtVaCreateWidget( "btMenuFile",
			xmRowColumnWidgetClass,
			btMenuFile_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( btMenuFile, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuFilepb */
	btMenuFilepb = XtVaCreateManagedWidget( "btMenuFilepb",
			xmPushButtonWidgetClass,
			btMenuFile,
			RES_CONVERT( XmNlabelString, QuitMenuLabel ),
			NULL );
	XtAddCallback( btMenuFilepb, XmNactivateCallback,
		(XtCallbackProc) activateCB_btMenuFilepb,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( btMenuFilepb, (char *) UxBacktrackManagementContext );


	/* Creation of bt_menu_file */
	bt_menu_file = XtVaCreateManagedWidget( "bt_menu_file",
			xmCascadeButtonWidgetClass,
			btMenu,
			RES_CONVERT( XmNlabelString, "File" ),
			XmNsubMenuId, btMenuFile,
			NULL );
	UxPutContext( bt_menu_file, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuReplay */
	btMenuReplay_shell = XtVaCreatePopupShell ("btMenuReplay_shell",
			xmMenuShellWidgetClass, btMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	btMenuReplay = XtVaCreateWidget( "btMenuReplay",
			xmRowColumnWidgetClass,
			btMenuReplay_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( btMenuReplay, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuReplayOntb */
	btMenuReplayOntb = XtVaCreateManagedWidget( "btMenuReplayOntb",
			xmToggleButtonWidgetClass,
			btMenuReplay,
			RES_CONVERT( XmNlabelString, "Start Replay" ),
			XmNvisibleWhenOff, TRUE,
			NULL );
	XtAddCallback( btMenuReplayOntb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_btMenuReplayOntb,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( btMenuReplayOntb, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuReplay_b3 */
	btMenuReplay_b3 = XtVaCreateManagedWidget( "btMenuReplay_b3",
			xmSeparatorWidgetClass,
			btMenuReplay,
			NULL );
	UxPutContext( btMenuReplay_b3, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuReplayOfftb */
	btMenuReplayOfftb = XtVaCreateManagedWidget( "btMenuReplayOfftb",
			xmToggleButtonWidgetClass,
			btMenuReplay,
			RES_CONVERT( XmNlabelString, "Stop Replay" ),
			XmNset, TRUE,
			XmNvisibleWhenOff, TRUE,
			NULL );
	XtAddCallback( btMenuReplayOfftb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_btMenuReplayOfftb,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( btMenuReplayOfftb, (char *) UxBacktrackManagementContext );


	/* Creation of bt_menu_replay */
	bt_menu_replay = XtVaCreateWidget( "bt_menu_replay",
			xmCascadeButtonWidgetClass,
			btMenu,
			RES_CONVERT( XmNlabelString, "Replay" ),
			XmNsubMenuId, btMenuReplay,
			NULL );
	UxPutContext( bt_menu_replay, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuModi */
	btMenuModi_shell = XtVaCreatePopupShell ("btMenuModi_shell",
			xmMenuShellWidgetClass, btMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	btMenuModi = XtVaCreateWidget( "btMenuModi",
			xmRowColumnWidgetClass,
			btMenuModi_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( btMenuModi, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuModiAvind */
	btMenuModiAvind_shell = XtVaCreatePopupShell ("btMenuModiAvind_shell",
			xmMenuShellWidgetClass, btMenuModi,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	btMenuModiAvind = XtVaCreateWidget( "btMenuModiAvind",
			xmRowColumnWidgetClass,
			btMenuModiAvind_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			XmNradioBehavior, TRUE,
			NULL );
	UxPutContext( btMenuModiAvind, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuModiAvindForwtb */
	btMenuModiAvindForwtb = XtVaCreateManagedWidget( "btMenuModiAvindForwtb",
			xmToggleButtonWidgetClass,
			btMenuModiAvind,
			RES_CONVERT( XmNlabelString, "Forward" ),
			XmNvisibleWhenOff, TRUE,
			NULL );
	XtAddCallback( btMenuModiAvindForwtb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_btMenuModiAvindForwtb,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( btMenuModiAvindForwtb, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuModiAvindBacktb */
	btMenuModiAvindBacktb = XtVaCreateManagedWidget( "btMenuModiAvindBacktb",
			xmToggleButtonWidgetClass,
			btMenuModiAvind,
			RES_CONVERT( XmNlabelString, "Backward" ),
			XmNset, TRUE,
			XmNvisibleWhenOff, TRUE,
			NULL );
	XtAddCallback( btMenuModiAvindBacktb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_btMenuModiAvindBacktb,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( btMenuModiAvindBacktb, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuModiFbcb */
	btMenuModiFbcb = XtVaCreateManagedWidget( "btMenuModiFbcb",
			xmCascadeButtonWidgetClass,
			btMenuModi,
			RES_CONVERT( XmNlabelString, "Direction" ),
			XmNsubMenuId, btMenuModiAvind,
			NULL );
	UxPutContext( btMenuModiFbcb, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuModiAutoman */
	btMenuModiAutoman_shell = XtVaCreatePopupShell ("btMenuModiAutoman_shell",
			xmMenuShellWidgetClass, btMenuModi,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	btMenuModiAutoman = XtVaCreateWidget( "btMenuModiAutoman",
			xmRowColumnWidgetClass,
			btMenuModiAutoman_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			XmNradioBehavior, TRUE,
			NULL );
	UxPutContext( btMenuModiAutoman, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuModiAutomanAutotb */
	btMenuModiAutomanAutotb = XtVaCreateManagedWidget( "btMenuModiAutomanAutotb",
			xmToggleButtonWidgetClass,
			btMenuModiAutoman,
			RES_CONVERT( XmNlabelString, "Automatic load" ),
			XmNvisibleWhenOff, TRUE,
			NULL );
	XtAddCallback( btMenuModiAutomanAutotb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_btMenuModiAutomanAutotb,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( btMenuModiAutomanAutotb, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuModiAutomanMan */
	btMenuModiAutomanMan = XtVaCreateManagedWidget( "btMenuModiAutomanMan",
			xmToggleButtonWidgetClass,
			btMenuModiAutoman,
			RES_CONVERT( XmNlabelString, "Manual Load" ),
			XmNset, TRUE,
			XmNvisibleWhenOff, TRUE,
			NULL );
	XtAddCallback( btMenuModiAutomanMan, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_btMenuModiAutomanMan,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( btMenuModiAutomanMan, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuModiAmcb */
	btMenuModiAmcb = XtVaCreateManagedWidget( "btMenuModiAmcb",
			xmCascadeButtonWidgetClass,
			btMenuModi,
			RES_CONVERT( XmNlabelString, "Auto/Man" ),
			XmNsubMenuId, btMenuModiAutoman,
			NULL );
	UxPutContext( btMenuModiAmcb, (char *) UxBacktrackManagementContext );


	/* Creation of bt_menu_mode */
	bt_menu_mode = XtVaCreateWidget( "bt_menu_mode",
			xmCascadeButtonWidgetClass,
			btMenu,
			RES_CONVERT( XmNlabelString, "Mode" ),
			XmNsubMenuId, btMenuModi,
			NULL );
	UxPutContext( bt_menu_mode, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuEdit */
	btMenuEdit_shell = XtVaCreatePopupShell ("btMenuEdit_shell",
			xmMenuShellWidgetClass, btMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	btMenuEdit = XtVaCreateWidget( "btMenuEdit",
			xmRowColumnWidgetClass,
			btMenuEdit_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( btMenuEdit, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuEditRecpb */
	btMenuEditRecpb = XtVaCreateManagedWidget( "btMenuEditRecpb",
			xmPushButtonWidgetClass,
			btMenuEdit,
			RES_CONVERT( XmNlabelString, "Set Record. Freq. ..." ),
			NULL );
	XtAddCallback( btMenuEditRecpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_btMenuEditRecpb,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( btMenuEditRecpb, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuEditPrespb */
	btMenuEditPrespb = XtVaCreateWidget( "btMenuEditPrespb",
			xmPushButtonWidgetClass,
			btMenuEdit,
			RES_CONVERT( XmNlabelString, "Timer Auto Bt" ),
			NULL );
	XtAddCallback( btMenuEditPrespb, XmNactivateCallback,
		(XtCallbackProc) activateCB_btMenuEditPrespb,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( btMenuEditPrespb, (char *) UxBacktrackManagementContext );


	/* Creation of bt_menu_edit */
	bt_menu_edit = XtVaCreateManagedWidget( "bt_menu_edit",
			xmCascadeButtonWidgetClass,
			btMenu,
			RES_CONVERT( XmNlabelString, "Edit" ),
			XmNsubMenuId, btMenuEdit,
			NULL );
	UxPutContext( bt_menu_edit, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuHelp */
	btMenuHelp_shell = XtVaCreatePopupShell ("btMenuHelp_shell",
			xmMenuShellWidgetClass, btMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	btMenuHelp = XtVaCreateWidget( "btMenuHelp",
			xmRowColumnWidgetClass,
			btMenuHelp_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( btMenuHelp, (char *) UxBacktrackManagementContext );


	/* Creation of btMenuHelppb */
	btMenuHelppb = XtVaCreateManagedWidget( "btMenuHelppb",
			xmPushButtonWidgetClass,
			btMenuHelp,
			RES_CONVERT( XmNlabelString, "Comandi" ),
			NULL );
	XtAddCallback( btMenuHelppb, XmNactivateCallback,
		(XtCallbackProc) activateCB_btMenuHelppb,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( btMenuHelppb, (char *) UxBacktrackManagementContext );


	/* Creation of bt_menu_help */
	bt_menu_help = XtVaCreateManagedWidget( "bt_menu_help",
			xmCascadeButtonWidgetClass,
			btMenu,
			RES_CONVERT( XmNlabelString, "Help" ),
			XmNsubMenuId, btMenuHelp,
			NULL );
	UxPutContext( bt_menu_help, (char *) UxBacktrackManagementContext );


	/* Creation of bulletinBoard2 */
	bulletinBoard2 = XtVaCreateManagedWidget( "bulletinBoard2",
			xmBulletinBoardWidgetClass,
			backtrackManagement,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 10,
			XmNy, 30,
			XmNwidth, 590,
			XmNheight, 50,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 0,
			XmNtopWidget, btMenu,
			NULL );
	UxPutContext( bulletinBoard2, (char *) UxBacktrackManagementContext );

	UxTmp0 = REPLAY_MODE_LABEL ? REPLAY_MODE_LABEL : "Replay";

	/* Creation of replaySelectionbt */
	replaySelectionbt = XtVaCreateManagedWidget( "replaySelectionbt",
			xmToggleButtonWidgetClass,
			bulletinBoard2,
			XmNx, 200,
			XmNy, 10,
			XmNwidth, 200,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, UxTmp0 ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( replaySelectionbt, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_replaySelectionbt,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( replaySelectionbt, (char *) UxBacktrackManagementContext );


	/* Creation of replaySelectionLabel */
	replaySelectionLabel = XtVaCreateManagedWidget( "replaySelectionLabel",
			xmLabelWidgetClass,
			bulletinBoard2,
			XmNx, 90,
			XmNy, 30,
			XmNwidth, 310,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "" ),
			NULL );
	UxPutContext( replaySelectionLabel, (char *) UxBacktrackManagementContext );


	/* Creation of btLoadLabel */
	btLoadLabel = XtVaCreateManagedWidget( "btLoadLabel",
			xmLabelWidgetClass,
			bulletinBoard2,
			XmNx, 50,
			XmNy, 10,
			XmNwidth, 100,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, BT_ACT_LOAD ),
			XmNalignment, XmALIGNMENT_END,
			NULL );
	UxPutContext( btLoadLabel, (char *) UxBacktrackManagementContext );


	/* Creation of btLoadNumber */
	btLoadNumber = XtVaCreateManagedWidget( "btLoadNumber",
			xmLabelWidgetClass,
			bulletinBoard2,
			XmNx, 150,
			XmNy, 10,
			XmNwidth, 40,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "0" ),
			NULL );
	UxPutContext( btLoadNumber, (char *) UxBacktrackManagementContext );


	/* Creation of backtrackScrollW */
	backtrackScrollW = XtVaCreateManagedWidget( "backtrackScrollW",
			xmScrolledWindowWidgetClass,
			backtrackManagement,
			XmNscrollingPolicy, XmAUTOMATIC,
			XmNx, 15,
			XmNy, 91,
			XmNwidth, 385,
			XmNheight, 416,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 15,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 15,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 10,
			XmNtopWidget, bulletinBoard2,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 15,
			XmNbottomWidget, frame15,
			XmNtopPosition, 0,
			NULL );
	UxPutContext( backtrackScrollW, (char *) UxBacktrackManagementContext );


	/* Creation of BtRc */
	BtRc = XtVaCreateManagedWidget( "BtRc",
			xmRowColumnWidgetClass,
			backtrackScrollW,
			XmNx, 2,
			XmNy, 2,
			XmNwidth, 350,
			XmNheight, 400,
			NULL );
	UxPutContext( BtRc, (char *) UxBacktrackManagementContext );


	/* Creation of menu5 */
	menu5_shell = XtVaCreatePopupShell ("menu5_shell",
			xmMenuShellWidgetClass, BtRc,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu5 = XtVaCreateWidget( "menu5",
			xmRowColumnWidgetClass,
			menu5_shell,
			XmNrowColumnType, XmMENU_POPUP,
			XmNmenuAccelerator, "Shift <KeyUp>F10",
			NULL );
	UxPutContext( menu5, (char *) UxBacktrackManagementContext );


	/* Creation of BtRcPopupView */
	BtRcPopupView = XtVaCreateManagedWidget( "BtRcPopupView",
			xmPushButtonWidgetClass,
			menu5,
			RES_CONVERT( XmNlabelString, ViewMenuLabel ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( BtRcPopupView, XmNactivateCallback,
		(XtCallbackProc) activateCB_BtRcPopupView,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( BtRcPopupView, (char *) UxBacktrackManagementContext );


	/* Creation of menu5_p1_b5 */
	menu5_p1_b5 = XtVaCreateManagedWidget( "menu5_p1_b5",
			xmSeparatorWidgetClass,
			menu5,
			NULL );
	UxPutContext( menu5_p1_b5, (char *) UxBacktrackManagementContext );


	/* Creation of BtRcPopupLoad */
	BtRcPopupLoad = XtVaCreateManagedWidget( "BtRcPopupLoad",
			xmPushButtonWidgetClass,
			menu5,
			RES_CONVERT( XmNlabelString, LoadMenuLabel ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( BtRcPopupLoad, XmNactivateCallback,
		(XtCallbackProc) activateCB_BtRcPopupLoad,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( BtRcPopupLoad, (char *) UxBacktrackManagementContext );


	/* Creation of BtRcPopupInit */
	BtRcPopupInit = XtVaCreateManagedWidget( "BtRcPopupInit",
			xmPushButtonWidgetClass,
			menu5,
			RES_CONVERT( XmNlabelString, InitMenuLabel ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( BtRcPopupInit, XmNactivateCallback,
		(XtCallbackProc) activateCB_BtRcPopupInit,
		UxGetWidget(backtrackManagement) );

	UxPutContext( BtRcPopupInit, (char *) UxBacktrackManagementContext );


	/* Creation of menu5_p1_b6 */
	menu5_p1_b6 = XtVaCreateManagedWidget( "menu5_p1_b6",
			xmSeparatorWidgetClass,
			menu5,
			NULL );
	UxPutContext( menu5_p1_b6, (char *) UxBacktrackManagementContext );


	/* Creation of BtRcPopupQuit */
	BtRcPopupQuit = XtVaCreateManagedWidget( "BtRcPopupQuit",
			xmPushButtonWidgetClass,
			menu5,
			RES_CONVERT( XmNlabelString, QuitMenuLabel ),
			NULL );
	XtAddCallback( BtRcPopupQuit, XmNactivateCallback,
		(XtCallbackProc) activateCB_BtRcPopupQuit,
		(XtPointer) UxBacktrackManagementContext );

	UxPutContext( BtRcPopupQuit, (char *) UxBacktrackManagementContext );

	XtVaSetValues(form22,
			XmNdefaultButton, btViewpb,
			NULL );

	XtVaSetValues(btMenu,
			XmNmenuHelpWidget, bt_menu_help,
			NULL );


	XtAddCallback( backtrackManagement, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxBacktrackManagementContext);

	XtAddEventHandler(BtRc, ButtonPressMask,
			False, (XtEventHandler) _UxbacktrackManagementMenuPost, (XtPointer) menu5 );

	return ( backtrackManagement );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_backtrackManagement( _Uxpadre_bt )
	Widget	_Uxpadre_bt;
{
	Widget                  rtrn;
	_UxCbacktrackManagement *UxContext;
	static int		_Uxinit = 0;

	UxBacktrackManagementContext = UxContext =
		(_UxCbacktrackManagement *) UxNewContext( sizeof(_UxCbacktrackManagement), False );

	padre_bt = _Uxpadre_bt;

	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewInterfaceClassId();
		_Uxinit = 1;
	}

	{
		printf ("inizio colloquio backtrack\n");
		set_cursor (padre_bt,CLOCK);
		bt_interface_active = 1;
		/* preset variabili puntatori ai records */
		prec_sel = 0;
		/* preset sul primo record (indice da 1 a _MAX_BACK_TRACK) */
		bt_sel = 1; 
		DragonFlag = 0;
		rtrn = _Uxbuild_backtrackManagement();
		UxPutClassCode( backtrackManagement, _UxIfClassId );

		bt_header = (BKTAB *)malloc(sizeof(BKTAB)*_MAX_BACK_TRACK);
		AllocaAreeBacktrack ();
		read_lista_bt (bt_header);
		display_header_bt (UxGetWidget(BtRc),bt_header);
		
		background_backtrack = read_background_color (UxGetWidget(BtRc));
		/*
		 * per snap di default
		 */
		snap_header = (SNTAB*)malloc(sizeof(SNTAB)*_MAX_SNAP_SHOT);
		read_lista_snap (snap_header);
		
		UxPopupInterface (rtrn, no_grab);
		set_cursor (padre_bt,NORMALE);
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

