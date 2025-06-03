
/*******************************************************************************
       scenarioMalf.c
       (Generated from interface file scenarioMalf.i)
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
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/CascadeB.h>
#include <Xm/Separator.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "bistrutt.h"

#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"


#include "malfunzioni.h"
#include "tabelle_malf.h"
#include "file_selection.h"
#include "messaggi.h"
extern swidget create_selezMalf();
extern swidget selezMalf;
extern swidget create_malfConfig();
extern swidget malfConfig;
extern SCENARI scenari_malf;
extern SCENARI scenari_frem;
void openMalfConfig ();
extern swidget masterMenu;
swidget sommarioMalf;
swidget sommarioFrem;
extern int sommario_malf_active;
extern int sommario_frem_active;

extern M_COMP_TYPE *m_comp_type;
extern MALF_SET *malf_set;
extern M_COMPONENT *m_component;

extern M_COMP_TYPE *fr_comp_type;
extern MALF_SET *frem_set;
extern M_COMPONENT *fr_component;


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
	Widget	UxscenarioMalf;
	Widget	UxscenMalfMenu;
	Widget	UxscenMalfMenuFile1;
	Widget	UxscenMalfMenuFileSaveDef;
	Widget	UxscenMalfMenuFileSep1;
	Widget	UxscenMalfMenuFileQuitpb;
	Widget	UxmalfMenu_top_File;
	Widget	UxscenMalfMenuEdit;
	Widget	UxscenMalfEditAdd;
	Widget	UxscenMalfMenuEdit_b3;
	Widget	UxscenMalfEditDel;
	Widget	UxmalfMenu_top_Edit;
	Widget	UxscenMalfMenuHelp;
	Widget	UxscenMalfMenuHelppb;
	Widget	UxmalfMenu_top_b3;
	Widget	UxscenMalfMenuShow;
	Widget	UxscenMalfMenuShowSummaryMalf;
	Widget	UxmalfMenu_top_Show;
	Widget	Uxframe16;
	Widget	Uxform32;
	Widget	UxquitMalfScenpb;
	Widget	UxdeactMalfScenpb;
	Widget	UxscenarioMalfScrollWin;
	Widget	UxscenMalfRc;
	Widget	UxscenMalfRcPopupMenu;
	Widget	UxscenMfPopupSave;
	Widget	UxscenMfPopupSep1;
	Widget	UxscenMalfRcPopupAdd;
	Widget	UxscenMalfPopupDel;
	Widget	UxscenMfPopupSep2;
	Widget	UxscenMfPopupSummary;
	Widget	UxscenMfPopupSep3;
	Widget	UxscenMalfRcPopupQuit;
	Widget	UxscenMalfEntryForm[30];
	Widget	Uxmenu6_p2_shell[30];
	Widget	Uxmenu6_p2[30];
	Widget	UxscenMalfMenuTarget[10][30];
	Widget	Uxmenu6[30];
	Widget	UxscenMalfTb[30];
	Widget	UxscenMalfMainLabel[30];
	Widget	UxscenMalfTipoComp[30];
	Widget	UxscenMalfDescr[30];
	Widget	UxscenMalfValAtt[30];
	Widget	UxscenMalfMoreData[30];
	Widget	UxscenMalfConfig[30];
	Widget	UxscenMalfDelay[30];
	Widget	UxscenMalfDurata[30];
	Widget	UxscenMalfRampa[30];
	int	Uxscen_malf_select;
	int	Uxmalf_select[30];
	int	UxitemExpand[30];
	int	Uxcol_sfondo_scenMalf;
	SCENARIO	Uxscenario;
	XtIntervalId	Uxtimer_scenarioMalf;
	float	Uxtempo_old[30];
	int	Uxprimo_giro[30];
	int	UxmoreDataAperto[30];
	float	UxvalorePrec[30];
	XtIntervalId	Uxtimer_sommarioMalf;
	int	UxnuovoSnapAggiornamento[30];
	Widget	UxParent;
	int	Uxtipo;
	int	Uxnum_scen;
} _UxCscenarioMalf;

static _UxCscenarioMalf        *UxScenarioMalfContext;
#define scenarioMalf            UxScenarioMalfContext->UxscenarioMalf
#define scenMalfMenu            UxScenarioMalfContext->UxscenMalfMenu
#define scenMalfMenuFile1       UxScenarioMalfContext->UxscenMalfMenuFile1
#define scenMalfMenuFileSaveDef UxScenarioMalfContext->UxscenMalfMenuFileSaveDef
#define scenMalfMenuFileSep1    UxScenarioMalfContext->UxscenMalfMenuFileSep1
#define scenMalfMenuFileQuitpb  UxScenarioMalfContext->UxscenMalfMenuFileQuitpb
#define malfMenu_top_File       UxScenarioMalfContext->UxmalfMenu_top_File
#define scenMalfMenuEdit        UxScenarioMalfContext->UxscenMalfMenuEdit
#define scenMalfEditAdd         UxScenarioMalfContext->UxscenMalfEditAdd
#define scenMalfMenuEdit_b3     UxScenarioMalfContext->UxscenMalfMenuEdit_b3
#define scenMalfEditDel         UxScenarioMalfContext->UxscenMalfEditDel
#define malfMenu_top_Edit       UxScenarioMalfContext->UxmalfMenu_top_Edit
#define scenMalfMenuHelp        UxScenarioMalfContext->UxscenMalfMenuHelp
#define scenMalfMenuHelppb      UxScenarioMalfContext->UxscenMalfMenuHelppb
#define malfMenu_top_b3         UxScenarioMalfContext->UxmalfMenu_top_b3
#define scenMalfMenuShow        UxScenarioMalfContext->UxscenMalfMenuShow
#define scenMalfMenuShowSummaryMalf UxScenarioMalfContext->UxscenMalfMenuShowSummaryMalf
#define malfMenu_top_Show       UxScenarioMalfContext->UxmalfMenu_top_Show
#define frame16                 UxScenarioMalfContext->Uxframe16
#define form32                  UxScenarioMalfContext->Uxform32
#define quitMalfScenpb          UxScenarioMalfContext->UxquitMalfScenpb
#define deactMalfScenpb         UxScenarioMalfContext->UxdeactMalfScenpb
#define scenarioMalfScrollWin   UxScenarioMalfContext->UxscenarioMalfScrollWin
#define scenMalfRc              UxScenarioMalfContext->UxscenMalfRc
#define scenMalfRcPopupMenu     UxScenarioMalfContext->UxscenMalfRcPopupMenu
#define scenMfPopupSave         UxScenarioMalfContext->UxscenMfPopupSave
#define scenMfPopupSep1         UxScenarioMalfContext->UxscenMfPopupSep1
#define scenMalfRcPopupAdd      UxScenarioMalfContext->UxscenMalfRcPopupAdd
#define scenMalfPopupDel        UxScenarioMalfContext->UxscenMalfPopupDel
#define scenMfPopupSep2         UxScenarioMalfContext->UxscenMfPopupSep2
#define scenMfPopupSummary      UxScenarioMalfContext->UxscenMfPopupSummary
#define scenMfPopupSep3         UxScenarioMalfContext->UxscenMfPopupSep3
#define scenMalfRcPopupQuit     UxScenarioMalfContext->UxscenMalfRcPopupQuit
#define scenMalfEntryForm       UxScenarioMalfContext->UxscenMalfEntryForm
#define menu6_p2_shell          UxScenarioMalfContext->Uxmenu6_p2_shell
#define menu6_p2                UxScenarioMalfContext->Uxmenu6_p2
#define scenMalfMenuTarget      UxScenarioMalfContext->UxscenMalfMenuTarget
#define menu6                   UxScenarioMalfContext->Uxmenu6
#define scenMalfTb              UxScenarioMalfContext->UxscenMalfTb
#define scenMalfMainLabel       UxScenarioMalfContext->UxscenMalfMainLabel
#define scenMalfTipoComp        UxScenarioMalfContext->UxscenMalfTipoComp
#define scenMalfDescr           UxScenarioMalfContext->UxscenMalfDescr
#define scenMalfValAtt          UxScenarioMalfContext->UxscenMalfValAtt
#define scenMalfMoreData        UxScenarioMalfContext->UxscenMalfMoreData
#define scenMalfConfig          UxScenarioMalfContext->UxscenMalfConfig
#define scenMalfDelay           UxScenarioMalfContext->UxscenMalfDelay
#define scenMalfDurata          UxScenarioMalfContext->UxscenMalfDurata
#define scenMalfRampa           UxScenarioMalfContext->UxscenMalfRampa
#define scen_malf_select        UxScenarioMalfContext->Uxscen_malf_select
#define malf_select             UxScenarioMalfContext->Uxmalf_select
#define itemExpand              UxScenarioMalfContext->UxitemExpand
#define col_sfondo_scenMalf     UxScenarioMalfContext->Uxcol_sfondo_scenMalf
#define scenario                UxScenarioMalfContext->Uxscenario
#define timer_scenarioMalf      UxScenarioMalfContext->Uxtimer_scenarioMalf
#define tempo_old               UxScenarioMalfContext->Uxtempo_old
#define primo_giro              UxScenarioMalfContext->Uxprimo_giro
#define moreDataAperto          UxScenarioMalfContext->UxmoreDataAperto
#define valorePrec              UxScenarioMalfContext->UxvalorePrec
#define timer_sommarioMalf      UxScenarioMalfContext->Uxtimer_sommarioMalf
#define nuovoSnapAggiornamento  UxScenarioMalfContext->UxnuovoSnapAggiornamento
#define Parent                  UxScenarioMalfContext->UxParent
#define tipo                    UxScenarioMalfContext->Uxtipo
#define num_scen                UxScenarioMalfContext->Uxnum_scen

int countScenariValidi (SCENARI *);
int remove_timer_editPertMalf ();
int save_scenmalf (Widget, int, int);
int nuovoScenarioNotify (Widget , Widget );
int delete_item_scen_malf (Widget);
int attivaMalfSelect (Widget , int );
int readConfMalf (Widget , int );
int attiva_timer_editPertMalf (Widget);
int caricaScenario (Widget,SCENARI *,SCENARIO *,int,int);
int displayScenario (Widget,SCENARIO *, M_COMPONENT *, MALF_SET *, M_COMP_TYPE *);
int attiva_timer_sommarioMalf(Widget);
int attiva_timer_scenarioMalf(Widget);





/*******************************************************************************
       The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxscenarioMalfMenuPost( wgt, client_data, event, ctd )
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

Widget	actMalfScenpb;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_scenarioMalf();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

int gestioneTastiScenarioMalf (Widget w)
{
SCENARI *s;
   switch (tipo)
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
   if (countScenariValidi (s) >= MAX_SCENARI)
      {
      XtVaSetValues (scenMalfMenuFileSaveDef, XmNsensitive, False, NULL);
      XtVaSetValues (scenMfPopupSave, XmNsensitive, False, NULL);
      }
   else
      {
      XtVaSetValues (scenMalfMenuFileSaveDef, XmNsensitive, True, NULL);
      XtVaSetValues (scenMfPopupSave, XmNsensitive, True, NULL);
      }
   return (0);
}
/*********************************************/
int clearScenario (SCENARIO *s)
{
int i;
s->scen_header.numvar = 0;
strcpy (s->scen_header.commento,"ENTER COMMENT");
for (i=0; i<MAX_VAR_SCENARIO; i++)
   {
   s->rec[i].valido = -1;
   s->rec[i].non_configurata = 0;
   }
return (0);
}
/*********************************************/
/************************
void openMalfConfig (w, data, reason)
Widget w;
char *data;
XmPushButtonCallbackStruct *reason;
{
int n;
_UxCscenarioMalf *Ctx;
   Ctx = (_UxCscenarioMalf *)UxGetWidget (w);
   memcpy (&n, data, sizeof(int));
#ifndef DESIGN_TIME
   if (Ctx->UxmoreDataAperto[n] == 0)
      {
      create_malfConfig (Ctx->UxscenarioMalf,n,tipo);
      Ctx->UxmoreDataAperto[n] = 1;
      }
#endif
}
*****************************/
/*********************************************/
int updateInterfaceFunctions (int t)
{
   switch (t)
      {
      case MALFUNZIONI:
      case FUNZIONI_REMOTE:
         break;
      case SOMMARIO_MALF:
      case SOMMARIO_FREM:
/* elimina i tasti dal popup */
         XtUnmanageChild (scenMfPopupSave);
         XtUnmanageChild (scenMalfRcPopupAdd);
         XtUnmanageChild (scenMalfPopupDel);
         XtUnmanageChild (scenMfPopupSep1);
         XtUnmanageChild (scenMfPopupSep2);
         XtUnmanageChild (scenMfPopupSummary);
/* elimina i tast1 di attivazione   */
/**         XtUnmanageChild (actMalfScenpb);
         XtUnmanageChild (deactMalfScenpb);
         XtManageChild (deactMalfScenSommpb);**/
/* elimina i tasti del menu edit */
         XtUnmanageChild (malfMenu_top_Edit);
         XtUnmanageChild (scenMalfMenuEdit);
/* elimina i tasti del menu Show */
         XtUnmanageChild (malfMenu_top_Show);
         XtUnmanageChild (scenMalfMenuShow);
/* elimina i tasti dal menu File */
         XtUnmanageChild (scenMalfMenuFileSaveDef);
         XtUnmanageChild (scenMalfMenuFileSep1);
         break;
      }
}
/***************************************************/
int uscita_scenarioMalf ()
{
DistruggiInterfaccia (XtParent(scenarioMalf));
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void  focusCB_scenarioMalf( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
	/*printf ("valutazione tasti nscen Focus = %d\n",num_scen);*/
	if (num_scen < 0) 
	   gestioneTastiScenarioMalf (UxWidget);
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  destroyCB_scenarioMalf( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
	if (tipo == SOMMARIO_MALF)
	   {
	   stop_timer (timer_sommarioMalf);
	   sommario_malf_active = 0;
	   }
	else if (tipo == SOMMARIO_FREM)
	   {
	   stop_timer (timer_sommarioMalf);
	   sommario_frem_active = 0;
	   }
	stop_timer (timer_scenarioMalf);
	remove_timer_editPertMalf();
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_scenMalfMenuFileSaveDef( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	save_scenmalf (scenarioMalf,tipo,num_scen);
	nuovoScenarioNotify (scenarioMalf,Parent);
#endif
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  createCB_scenMalfMenuFileSaveDef( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxContext = UxScenarioMalfContext;
	{
	
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_scenMalfMenuFileQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
	uscita_scenarioMalf ();
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  cascadingCB_malfMenu_top_File( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
	printf ("valutazione tasti nscen cascade= %d\n",num_scen);
	if (num_scen < 0) /* e' stato richiesto un nuovo scenario */
	   gestioneTastiScenarioMalf (UxWidget);
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_scenMalfEditAdd( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	/**
	create_selezMalf (UxGetWidget(scenarioMalf),&scenario,tipo); **/
	create_selVar (UxGetWidget(scenarioMalf),
	                &scenario,tipo,
	                scenari_malf.s[num_scen-1].scen_header.commento); 
#endif
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_scenMalfEditDel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
	delete_item_scen_malf (UxWidget);
	
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_scenMalfMenuHelppb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{printf ("help comandi\n");}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_scenMalfMenuShowSummaryMalf( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
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
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_quitMalfScenpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
	uscita_scenarioMalf ();
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_actMalfScenpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
	attivaMalfSelect (UxWidget,ACTIVATION);
	
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_deactMalfScenpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
	attivaMalfSelect (UxWidget,DEACTIVATION);
	
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_scenMfPopupSave( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	save_scenmalf (scenarioMalf,tipo,num_scen);
	nuovoScenarioNotify (scenarioMalf,Parent);
#endif
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  createCB_scenMfPopupSave( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxContext = UxScenarioMalfContext;
	{
	if (num_scen < 0) /* e' stato richiesto un nuovo scenario */
	   gestioneTastiScenarioMalf (UxWidget);
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_scenMalfRcPopupAdd( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	/***
	create_selezMalf (UxGetWidget(scenarioMalf),&scenario,tipo);
	***/
	create_selVar (UxGetWidget(scenarioMalf),
	                &scenario,tipo,
	                scenari_malf.s[num_scen-1].scen_header.commento);
#endif
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_scenMalfPopupDel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
	delete_item_scen_malf (UxWidget);
	}
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_scenMfPopupSummary( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
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
	UxScenarioMalfContext = UxSaveCtx;
}

static void  activateCB_scenMalfRcPopupQuit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCscenarioMalf        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScenarioMalfContext;
	UxScenarioMalfContext = UxContext =
			(_UxCscenarioMalf *) UxGetContext( UxWidget );
	{
	uscita_scenarioMalf ();
	}
	UxScenarioMalfContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_scenarioMalf()
{
	Widget		_UxParent;
	Widget		scenMalfMenuFile1_shell;
	Widget		scenMalfMenuEdit_shell;
	Widget		scenMalfMenuHelp_shell;
	Widget		scenMalfMenuShow_shell;
	Widget		scenMalfRcPopupMenu_shell;


	/* Creation of scenarioMalf */
	_UxParent = XtVaCreatePopupShell( "scenarioMalf_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 360,
			XmNy, 470,
			XmNwidth, 750,
			XmNheight, 390,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "scenarioMalf",
			NULL );

	scenarioMalf = XtVaCreateWidget( "scenarioMalf",
			xmFormWidgetClass,
			_UxParent,
			XmNwidth, 750,
			XmNheight, 390,
			XmNunitType, XmPIXELS,
			XmNdefaultPosition, FALSE,
			NULL );
	XtAddCallback( scenarioMalf, XmNfocusCallback,
		(XtCallbackProc) focusCB_scenarioMalf,
		(XtPointer) UxScenarioMalfContext );
	XtAddCallback( scenarioMalf, XmNdestroyCallback,
		(XtCallbackProc) destroyCB_scenarioMalf,
		(XtPointer) UxScenarioMalfContext );

	UxPutContext( scenarioMalf, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfMenu */
	scenMalfMenu = XtVaCreateManagedWidget( "scenMalfMenu",
			xmRowColumnWidgetClass,
			scenarioMalf,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNpacking, XmPACK_TIGHT,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( scenMalfMenu, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfMenuFile1 */
	scenMalfMenuFile1_shell = XtVaCreatePopupShell ("scenMalfMenuFile1_shell",
			xmMenuShellWidgetClass, scenMalfMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	scenMalfMenuFile1 = XtVaCreateWidget( "scenMalfMenuFile1",
			xmRowColumnWidgetClass,
			scenMalfMenuFile1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( scenMalfMenuFile1, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfMenuFileSaveDef */
	scenMalfMenuFileSaveDef = XtVaCreateManagedWidget( "scenMalfMenuFileSaveDef",
			xmPushButtonWidgetClass,
			scenMalfMenuFile1,
			RES_CONVERT( XmNlabelString, SaveMenuLabel ),
			NULL );
	XtAddCallback( scenMalfMenuFileSaveDef, XmNactivateCallback,
		(XtCallbackProc) activateCB_scenMalfMenuFileSaveDef,
		(XtPointer) UxScenarioMalfContext );

	UxPutContext( scenMalfMenuFileSaveDef, (char *) UxScenarioMalfContext );

	createCB_scenMalfMenuFileSaveDef( scenMalfMenuFileSaveDef,
			(XtPointer) UxScenarioMalfContext, (XtPointer) NULL );


	/* Creation of scenMalfMenuFileSep1 */
	scenMalfMenuFileSep1 = XtVaCreateManagedWidget( "scenMalfMenuFileSep1",
			xmSeparatorWidgetClass,
			scenMalfMenuFile1,
			NULL );
	UxPutContext( scenMalfMenuFileSep1, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfMenuFileQuitpb */
	scenMalfMenuFileQuitpb = XtVaCreateManagedWidget( "scenMalfMenuFileQuitpb",
			xmPushButtonWidgetClass,
			scenMalfMenuFile1,
			RES_CONVERT( XmNlabelString, QuitMenuLabel ),
			NULL );
	XtAddCallback( scenMalfMenuFileQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_scenMalfMenuFileQuitpb,
		(XtPointer) 0x0 );

	UxPutContext( scenMalfMenuFileQuitpb, (char *) UxScenarioMalfContext );


	/* Creation of malfMenu_top_File */
	malfMenu_top_File = XtVaCreateManagedWidget( "malfMenu_top_File",
			xmCascadeButtonWidgetClass,
			scenMalfMenu,
			RES_CONVERT( XmNlabelString, FileMenuLabel ),
			XmNsubMenuId, scenMalfMenuFile1,
			NULL );
	XtAddCallback( malfMenu_top_File, XmNcascadingCallback,
		(XtCallbackProc) cascadingCB_malfMenu_top_File,
		(XtPointer) UxScenarioMalfContext );

	UxPutContext( malfMenu_top_File, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfMenuEdit */
	scenMalfMenuEdit_shell = XtVaCreatePopupShell ("scenMalfMenuEdit_shell",
			xmMenuShellWidgetClass, scenMalfMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	scenMalfMenuEdit = XtVaCreateWidget( "scenMalfMenuEdit",
			xmRowColumnWidgetClass,
			scenMalfMenuEdit_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( scenMalfMenuEdit, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfEditAdd */
	scenMalfEditAdd = XtVaCreateManagedWidget( "scenMalfEditAdd",
			xmPushButtonWidgetClass,
			scenMalfMenuEdit,
			RES_CONVERT( XmNlabelString, AddItemLabel ),
			NULL );
	XtAddCallback( scenMalfEditAdd, XmNactivateCallback,
		(XtCallbackProc) activateCB_scenMalfEditAdd,
		(XtPointer) UxScenarioMalfContext );

	UxPutContext( scenMalfEditAdd, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfMenuEdit_b3 */
	scenMalfMenuEdit_b3 = XtVaCreateManagedWidget( "scenMalfMenuEdit_b3",
			xmSeparatorWidgetClass,
			scenMalfMenuEdit,
			NULL );
	UxPutContext( scenMalfMenuEdit_b3, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfEditDel */
	scenMalfEditDel = XtVaCreateManagedWidget( "scenMalfEditDel",
			xmPushButtonWidgetClass,
			scenMalfMenuEdit,
			RES_CONVERT( XmNlabelString, CutMenuLabel ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( scenMalfEditDel, XmNactivateCallback,
		(XtCallbackProc) activateCB_scenMalfEditDel,
		(XtPointer) UxScenarioMalfContext );

	UxPutContext( scenMalfEditDel, (char *) UxScenarioMalfContext );


	/* Creation of malfMenu_top_Edit */
	malfMenu_top_Edit = XtVaCreateManagedWidget( "malfMenu_top_Edit",
			xmCascadeButtonWidgetClass,
			scenMalfMenu,
			RES_CONVERT( XmNlabelString, EditMenuLabel ),
			XmNsubMenuId, scenMalfMenuEdit,
			NULL );
	UxPutContext( malfMenu_top_Edit, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfMenuHelp */
	scenMalfMenuHelp_shell = XtVaCreatePopupShell ("scenMalfMenuHelp_shell",
			xmMenuShellWidgetClass, scenMalfMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	scenMalfMenuHelp = XtVaCreateWidget( "scenMalfMenuHelp",
			xmRowColumnWidgetClass,
			scenMalfMenuHelp_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( scenMalfMenuHelp, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfMenuHelppb */
	scenMalfMenuHelppb = XtVaCreateManagedWidget( "scenMalfMenuHelppb",
			xmPushButtonWidgetClass,
			scenMalfMenuHelp,
			RES_CONVERT( XmNlabelString, "Commands" ),
			RES_CONVERT( XmNmnemonic, "C" ),
			NULL );
	XtAddCallback( scenMalfMenuHelppb, XmNactivateCallback,
		(XtCallbackProc) activateCB_scenMalfMenuHelppb,
		(XtPointer) UxScenarioMalfContext );

	UxPutContext( scenMalfMenuHelppb, (char *) UxScenarioMalfContext );


	/* Creation of malfMenu_top_b3 */
	malfMenu_top_b3 = XtVaCreateManagedWidget( "malfMenu_top_b3",
			xmCascadeButtonWidgetClass,
			scenMalfMenu,
			RES_CONVERT( XmNlabelString, HelpMenuLabel ),
			XmNsubMenuId, scenMalfMenuHelp,
			NULL );
	UxPutContext( malfMenu_top_b3, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfMenuShow */
	scenMalfMenuShow_shell = XtVaCreatePopupShell ("scenMalfMenuShow_shell",
			xmMenuShellWidgetClass, scenMalfMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	scenMalfMenuShow = XtVaCreateWidget( "scenMalfMenuShow",
			xmRowColumnWidgetClass,
			scenMalfMenuShow_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( scenMalfMenuShow, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfMenuShowSummaryMalf */
	scenMalfMenuShowSummaryMalf = XtVaCreateManagedWidget( "scenMalfMenuShowSummaryMalf",
			xmPushButtonWidgetClass,
			scenMalfMenuShow,
			RES_CONVERT( XmNlabelString, MALF_SUMMARY ),
			NULL );
	XtAddCallback( scenMalfMenuShowSummaryMalf, XmNactivateCallback,
		(XtCallbackProc) activateCB_scenMalfMenuShowSummaryMalf,
		(XtPointer) UxScenarioMalfContext );

	UxPutContext( scenMalfMenuShowSummaryMalf, (char *) UxScenarioMalfContext );


	/* Creation of malfMenu_top_Show */
	malfMenu_top_Show = XtVaCreateManagedWidget( "malfMenu_top_Show",
			xmCascadeButtonWidgetClass,
			scenMalfMenu,
			RES_CONVERT( XmNlabelString, ShowMenuLabel ),
			XmNsubMenuId, scenMalfMenuShow,
			NULL );
	UxPutContext( malfMenu_top_Show, (char *) UxScenarioMalfContext );


	/* Creation of frame16 */
	frame16 = XtVaCreateManagedWidget( "frame16",
			xmFrameWidgetClass,
			scenarioMalf,
			XmNx, 48,
			XmNy, 288,
			XmNwidth, 690,
			XmNheight, 80,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame16, (char *) UxScenarioMalfContext );


	/* Creation of form32 */
	form32 = XtVaCreateManagedWidget( "form32",
			xmFormWidgetClass,
			frame16,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 2,
			XmNy, 20,
			XmNwidth, 851,
			XmNheight, 82,
			NULL );
	UxPutContext( form32, (char *) UxScenarioMalfContext );


	/* Creation of quitMalfScenpb */
	quitMalfScenpb = XtVaCreateManagedWidget( "quitMalfScenpb",
			xmPushButtonWidgetClass,
			form32,
			XmNx, 530,
			XmNy, 15,
			XmNwidth, 80,
			XmNheight, 30,
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
	XtAddCallback( quitMalfScenpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_quitMalfScenpb,
		scenarioMalf );

	UxPutContext( quitMalfScenpb, (char *) UxScenarioMalfContext );


	/* Creation of actMalfScenpb */
	actMalfScenpb = XtVaCreateManagedWidget( "actMalfScenpb",
			xmPushButtonWidgetClass,
			form32,
			XmNx, 20,
			XmNy, 15,
			XmNwidth, 80,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, ACT_LABEL ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNrightAttachment, XmATTACH_NONE,
			XmNrightPosition, 0,
			XmNrightOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNsensitive, TRUE,
			NULL );
	XtAddCallback( actMalfScenpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_actMalfScenpb,
		(XtPointer) 0x0 );

	UxPutContext( actMalfScenpb, (char *) UxScenarioMalfContext );


	/* Creation of deactMalfScenpb */
	deactMalfScenpb = XtVaCreateManagedWidget( "deactMalfScenpb",
			xmPushButtonWidgetClass,
			form32,
			XmNx, 149,
			XmNy, 15,
			XmNwidth, 80,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, DEACT_LABEL ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftWidget, NULL,
			XmNrightAttachment, XmATTACH_NONE,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNsensitive, TRUE,
			XmNrightPosition, 0,
			XmNleftPosition, 20,
			NULL );
	XtAddCallback( deactMalfScenpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_deactMalfScenpb,
		(XtPointer) UxScenarioMalfContext );

	UxPutContext( deactMalfScenpb, (char *) UxScenarioMalfContext );


	/* Creation of scenarioMalfScrollWin */
	scenarioMalfScrollWin = XtVaCreateManagedWidget( "scenarioMalfScrollWin",
			xmScrolledWindowWidgetClass,
			scenarioMalf,
			XmNscrollingPolicy, XmAUTOMATIC,
			XmNx, 15,
			XmNy, 60,
			XmNwidth, 500,
			XmNheight, 255,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 20,
			XmNbottomWidget, frame16,
			XmNleftOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 20,
			XmNtopWidget, scenMalfMenu,
			NULL );
	UxPutContext( scenarioMalfScrollWin, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfRc */
	scenMalfRc = XtVaCreateManagedWidget( "scenMalfRc",
			xmRowColumnWidgetClass,
			scenarioMalfScrollWin,
			XmNx, 2,
			XmNy, 2,
			XmNwidth, 600,
			XmNheight, 230,
			NULL );
	UxPutContext( scenMalfRc, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfRcPopupMenu */
	scenMalfRcPopupMenu_shell = XtVaCreatePopupShell ("scenMalfRcPopupMenu_shell",
			xmMenuShellWidgetClass, scenarioMalf,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	scenMalfRcPopupMenu = XtVaCreateWidget( "scenMalfRcPopupMenu",
			xmRowColumnWidgetClass,
			scenMalfRcPopupMenu_shell,
			XmNrowColumnType, XmMENU_POPUP,
			XmNmenuAccelerator, "Shift <KeyUp>F10",
			NULL );
	UxPutContext( scenMalfRcPopupMenu, (char *) UxScenarioMalfContext );


	/* Creation of scenMfPopupSave */
	scenMfPopupSave = XtVaCreateManagedWidget( "scenMfPopupSave",
			xmPushButtonWidgetClass,
			scenMalfRcPopupMenu,
			RES_CONVERT( XmNlabelString, SaveMenuLabel ),
			NULL );
	XtAddCallback( scenMfPopupSave, XmNactivateCallback,
		(XtCallbackProc) activateCB_scenMfPopupSave,
		(XtPointer) UxScenarioMalfContext );

	UxPutContext( scenMfPopupSave, (char *) UxScenarioMalfContext );

	createCB_scenMfPopupSave( scenMfPopupSave,
			(XtPointer) UxScenarioMalfContext, (XtPointer) NULL );


	/* Creation of scenMfPopupSep1 */
	scenMfPopupSep1 = XtVaCreateManagedWidget( "scenMfPopupSep1",
			xmSeparatorWidgetClass,
			scenMalfRcPopupMenu,
			NULL );
	UxPutContext( scenMfPopupSep1, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfRcPopupAdd */
	scenMalfRcPopupAdd = XtVaCreateManagedWidget( "scenMalfRcPopupAdd",
			xmPushButtonWidgetClass,
			scenMalfRcPopupMenu,
			RES_CONVERT( XmNlabelString, AddItemLabel ),
			NULL );
	XtAddCallback( scenMalfRcPopupAdd, XmNactivateCallback,
		(XtCallbackProc) activateCB_scenMalfRcPopupAdd,
		(XtPointer) UxScenarioMalfContext );

	UxPutContext( scenMalfRcPopupAdd, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfPopupDel */
	scenMalfPopupDel = XtVaCreateManagedWidget( "scenMalfPopupDel",
			xmPushButtonWidgetClass,
			scenMalfRcPopupMenu,
			RES_CONVERT( XmNlabelString, CutMenuLabel ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( scenMalfPopupDel, XmNactivateCallback,
		(XtCallbackProc) activateCB_scenMalfPopupDel,
		(XtPointer) UxScenarioMalfContext );

	UxPutContext( scenMalfPopupDel, (char *) UxScenarioMalfContext );


	/* Creation of scenMfPopupSep2 */
	scenMfPopupSep2 = XtVaCreateManagedWidget( "scenMfPopupSep2",
			xmSeparatorWidgetClass,
			scenMalfRcPopupMenu,
			NULL );
	UxPutContext( scenMfPopupSep2, (char *) UxScenarioMalfContext );


	/* Creation of scenMfPopupSummary */
	scenMfPopupSummary = XtVaCreateManagedWidget( "scenMfPopupSummary",
			xmPushButtonWidgetClass,
			scenMalfRcPopupMenu,
			RES_CONVERT( XmNlabelString, MALF_SUMMARY ),
			NULL );
	XtAddCallback( scenMfPopupSummary, XmNactivateCallback,
		(XtCallbackProc) activateCB_scenMfPopupSummary,
		(XtPointer) UxScenarioMalfContext );

	UxPutContext( scenMfPopupSummary, (char *) UxScenarioMalfContext );


	/* Creation of scenMfPopupSep3 */
	scenMfPopupSep3 = XtVaCreateManagedWidget( "scenMfPopupSep3",
			xmSeparatorWidgetClass,
			scenMalfRcPopupMenu,
			NULL );
	UxPutContext( scenMfPopupSep3, (char *) UxScenarioMalfContext );


	/* Creation of scenMalfRcPopupQuit */
	scenMalfRcPopupQuit = XtVaCreateManagedWidget( "scenMalfRcPopupQuit",
			xmPushButtonWidgetClass,
			scenMalfRcPopupMenu,
			RES_CONVERT( XmNlabelString, QuitMenuLabel ),
			NULL );
	XtAddCallback( scenMalfRcPopupQuit, XmNactivateCallback,
		(XtCallbackProc) activateCB_scenMalfRcPopupQuit,
		scenarioMalf );

	UxPutContext( scenMalfRcPopupQuit, (char *) UxScenarioMalfContext );

	XtVaSetValues(scenMalfMenu,
			XmNmenuHelpWidget, malfMenu_top_b3,
			NULL );

	XtVaSetValues(form32,
			XmNdefaultButton, actMalfScenpb,
			NULL );


	XtAddCallback( scenarioMalf, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxScenarioMalfContext);

	XtAddEventHandler(scenarioMalf, ButtonPressMask,
			False, (XtEventHandler) _UxscenarioMalfMenuPost, (XtPointer) scenMalfRcPopupMenu );

	return ( scenarioMalf );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_scenarioMalf( _UxParent, _Uxtipo, _Uxnum_scen )
	Widget	_UxParent;
	int	_Uxtipo;
	int	_Uxnum_scen;
{
	Widget                  rtrn;
	_UxCscenarioMalf        *UxContext;
	static int		_Uxinit = 0;

	UxScenarioMalfContext = UxContext =
		(_UxCscenarioMalf *) UxNewContext( sizeof(_UxCscenarioMalf), False );

	Parent = _UxParent;
	tipo = _Uxtipo;
	num_scen = _Uxnum_scen;

	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewInterfaceClassId();
		_Uxinit = 1;
	}

	{
		int i;
		for (i=0; i<MAX_VAR_SCENARIO; i++)
		   {
		   moreDataAperto[i] = 0;
		   primo_giro[i] = 1;
		   valorePrec[i] = -999.999;
		   nuovoSnapAggiornamento[i] = 0;
		   }
		rtrn = _Uxbuild_scenarioMalf();
		UxPutClassCode( scenarioMalf, _UxIfClassId );

		if(readConfMalf (rtrn,tipo) >= 0)
		   {
		   attiva_timer_editPertMalf (masterMenu);
		   col_sfondo_scenMalf = read_background_color(rtrn); 
		   switch (tipo)
		     {
		     case MALFUNZIONI:
		printf ("MALF: num_scen = %d\n",num_scen);
		        scen_malf_select = -1; /* selezione da lista: no valid preset */
		        clearScenario (&scenario);
		/*      update_title (rtrn,NEW_MALF_SCEN); */
		        if ((num_scen > 0) && (scenari_malf.valido[num_scen-1] > 0)) 
		           {
		           caricaScenario (rtrn, &scenari_malf, &scenario, tipo, num_scen);
		           displayScenario (rtrn, &scenario,m_component,
		                      malf_set, m_comp_type);
		           update_title (rtrn,scenari_malf.s[num_scen-1].scen_header.commento);
		           }
		        break;
		      case SOMMARIO_MALF:
		printf ("Richiesta SOMMARIO MALF\n");
		         sommarioMalf = rtrn;
		         sommario_malf_active = 1;
		         scen_malf_select = -1; /* selezione da lista: no valid preset */
		         clearScenario (&scenario);
		         caricaScenario (rtrn, &scenari_malf, &scenario, tipo, num_scen);
		         displayScenario (rtrn, &scenario,m_component,
		                      malf_set, m_comp_type);
		         updateInterfaceFunctions (tipo); /* rimuove tasti non utilizzabili con sommario */
		         update_title (rtrn,MALF_LIST); 
		         attiva_timer_sommarioMalf (rtrn);
		         break;
		     case FUNZIONI_REMOTE:
		printf ("FREM: num_scen = %d\n",num_scen);
		        scen_malf_select = -1; /* selezione da lista: no valid preset */
		        clearScenario (&scenario);
		/*      update_title (rtrn,NEW_FREM_SCEN); */
		        if ((num_scen > 0) && (scenari_frem.valido[num_scen-1] > 0)) 
		           {
		           caricaScenario (rtrn, &scenari_frem, &scenario, tipo, num_scen);
		           displayScenario (rtrn, &scenario,fr_component,
		                      frem_set, fr_comp_type);
		           update_title (rtrn,scenari_frem.s[num_scen-1].scen_header.commento);
		           }
		        XtVaSetValues (scenMalfMenuShowSummaryMalf,
		                           RES_CONVERT(XmNlabelString,FREM_SUMMARY),NULL);
		        XtVaSetValues (scenMfPopupSummary,
		                           RES_CONVERT(XmNlabelString,FREM_SUMMARY),NULL);
		        break;
		      case SOMMARIO_FREM:
		printf ("Richiesta SOMMARIO FREM\n");
		         sommarioFrem = rtrn;
		         sommario_frem_active = 1;
		         scen_malf_select = -1; /* selezione da lista: no valid preset */
		         clearScenario (&scenario);
		         caricaScenario (rtrn, &scenari_frem, &scenario, tipo, num_scen);
		         displayScenario (rtrn, &scenario,fr_component,
		                      frem_set, fr_comp_type);
		         updateInterfaceFunctions (tipo); /* rimuove tasti non utilizzabili con sommario */
		         update_title (rtrn,FREM_LIST); 
		         attiva_timer_sommarioMalf (rtrn);
		         break;
		      }
		   attiva_timer_scenarioMalf (rtrn);
		   UxPopupInterface (rtrn, no_grab);
		   }
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

