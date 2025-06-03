
/*******************************************************************************
       initialCondition.c
       (Generated from interface file initialCondition.i)
*******************************************************************************/

#include <stdio.h>

#ifdef MOTIF
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#endif /* MOTIF */

#include "UxXt.h"
#include <Xm/ScrolledW.h>
#include <Xm/Frame.h>
#include <Xm/CascadeB.h>
#include <Xm/Separator.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <malloc.h>
#include <X11/Intrinsic.h>

#ifndef DESIGN_TIME
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

extern int _MAX_SNAP_SHOT;
 
#include "init_ci.h"
#include "cursore.h"

XtIntervalId timer_initialCondition;
int tasto_initic_attivabile = False;

#include "parametri.h" 
#include "bistrutt.h"
#include "preview.h"
#include "option.h"
extern OPTIONS_FLAGS options;
extern Boolean *IcProt;


#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"

extern int snap_interface_active;
SNTAB *snap_header;
int colore_sfondo;
#endif

extern int inizializzazione;
extern int isSuperuser;

extern int get_colors();
extern int get_colore_sfondo();
extern int set_colore_riga_snap();
extern int read_lista_snap();
extern int carica_ci();
extern int carica_bt();
extern int esegui_snap();
Boolean cutCopyCiBuffer; /* 1 = pieno */
extern swidget masterMenu;
extern swidget icCommentReqform;
extern swidget create_icCommentReqform();
extern swidget previewSnap;
extern swidget create_previewSnap();
int nuova_attivazione;
#include "messaggi.h"

int crea_snapshot_entry(Widget);
int attiva_timer_initialCondition (Widget);


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
	Widget	UxicMenu;
	Widget	UxicMenuFile;
	Widget	UxicMenuFilePrint;
	Widget	UxicMenuFile_b2;
	Widget	UxicMenuFileQuitpb;
	Widget	Uxic_menu_file;
	Widget	UxicMenuEdit;
	Widget	UxicMenuEditSep1;
	Widget	UxicMenuEdit_b5;
	Widget	UxicMenuEditCheck;
	Widget	UxicMenuEditCheckAll;
	Widget	Uxic_menu_edit;
	Widget	UxicMenuHelp;
	Widget	UxicMenuHelpCommand;
	Widget	Uxic_menu_help;
	Widget	Uxframe8;
	Widget	Uxform27;
	Widget	UxicQuitpb;
	Widget	UxicInitpb;
	Widget	UxicSavepb;
	Widget	UxicViewpb;
	Widget	UxinitialConditionScrollW;
	Widget	UxIcRc;
	Widget	UxIcRcPopupMenu;
	Widget	UxIcRcPopupView;
	Widget	UxIcRcPopupMenu_p1_b2;
	Widget	UxIcRcPopupLoad;
	Widget	UxIcRcPopupSave;
	Widget	UxIcRcPopupMenu_p1_b5;
	Widget	UxIcRcPopupMenuPrint;
	Widget	UxIcRcPopupMenu_p1_b7;
	Widget	UxIcRcPopupQuit;
	Widget	Uxpadre;
} _UxCinitialCondition;

static _UxCinitialCondition    *UxInitialConditionContext;
#define icMenu                  UxInitialConditionContext->UxicMenu
#define icMenuFile              UxInitialConditionContext->UxicMenuFile
#define icMenuFilePrint         UxInitialConditionContext->UxicMenuFilePrint
#define icMenuFile_b2           UxInitialConditionContext->UxicMenuFile_b2
#define icMenuFileQuitpb        UxInitialConditionContext->UxicMenuFileQuitpb
#define ic_menu_file            UxInitialConditionContext->Uxic_menu_file
#define icMenuEdit              UxInitialConditionContext->UxicMenuEdit
#define icMenuEditSep1          UxInitialConditionContext->UxicMenuEditSep1
#define icMenuEdit_b5           UxInitialConditionContext->UxicMenuEdit_b5
#define icMenuEditCheck         UxInitialConditionContext->UxicMenuEditCheck
#define icMenuEditCheckAll      UxInitialConditionContext->UxicMenuEditCheckAll
#define ic_menu_edit            UxInitialConditionContext->Uxic_menu_edit
#define icMenuHelp              UxInitialConditionContext->UxicMenuHelp
#define icMenuHelpCommand       UxInitialConditionContext->UxicMenuHelpCommand
#define ic_menu_help            UxInitialConditionContext->Uxic_menu_help
#define frame8                  UxInitialConditionContext->Uxframe8
#define form27                  UxInitialConditionContext->Uxform27
#define icQuitpb                UxInitialConditionContext->UxicQuitpb
#define icInitpb                UxInitialConditionContext->UxicInitpb
#define icSavepb                UxInitialConditionContext->UxicSavepb
#define icViewpb                UxInitialConditionContext->UxicViewpb
#define initialConditionScrollW UxInitialConditionContext->UxinitialConditionScrollW
#define IcRc                    UxInitialConditionContext->UxIcRc
#define IcRcPopupMenu           UxInitialConditionContext->UxIcRcPopupMenu
#define IcRcPopupView           UxInitialConditionContext->UxIcRcPopupView
#define IcRcPopupMenu_p1_b2     UxInitialConditionContext->UxIcRcPopupMenu_p1_b2
#define IcRcPopupLoad           UxInitialConditionContext->UxIcRcPopupLoad
#define IcRcPopupSave           UxInitialConditionContext->UxIcRcPopupSave
#define IcRcPopupMenu_p1_b5     UxInitialConditionContext->UxIcRcPopupMenu_p1_b5
#define IcRcPopupMenuPrint      UxInitialConditionContext->UxIcRcPopupMenuPrint
#define IcRcPopupMenu_p1_b7     UxInitialConditionContext->UxIcRcPopupMenu_p1_b7
#define IcRcPopupQuit           UxInitialConditionContext->UxIcRcPopupQuit
#define padre                   UxInitialConditionContext->Uxpadre


/*******************************************************************************
       The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxinitialConditionMenuPost( wgt, client_data, event, ctd )
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

Widget	initialCondition;
Widget	icMenuEditCutPb;
Widget	icMenuEditCopy;
Widget	icMenuEditPaste;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_initialCondition();
int stampa_ci (Widget);
//int cancella_ci (Widget);
int copia_ci (Widget,Widget, int);
int paste_ci (Widget, int);
int accetta_ci(Widget, int);
int salva_ci(Widget, int);
int AllocaAreeSnapshot();
int display_header_snap (SNTAB *);
int cancella_ci(Widget, int);






/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/************************************************************************/
int add_item_initial_condition(parent)
Widget parent;
{

   crea_snapshot_entry(parent);	
 	   
}
/******************************************************************/
void IcSelActivate (parent, dati, reason)
Widget parent;
char *dati;
XmAnyCallbackStruct *reason;
{
#ifndef DESIGN_TIME
 
int tipo_entry;
static int prec_sel = 0;
Boolean permissivo;
Boolean permissivo_paste;
   
   ic_sel = atoi(dati);
	
   tipo_entry = reason->reason;

   switch (tipo_entry)
      {
      case XmCR_FOCUS:
	if ((prec_sel > 0) && (prec_sel != ic_sel))
	   set_colore_riga_snap(prec_sel-1, colore_sfondo);
        set_colore_riga_snap(ic_sel-1, colore_app[0]); 
	break;       	   
      case XmCR_LOSING_FOCUS:
      default:
         break; 
      }
   
/*   printf ("selezionato ic %d\n",ic_sel);   */
/*
 * abilitazione superuser
 */
   if (!isSuperuser)       /* se non siamo superuser     */ 
      {
      if (*(IcProt+ic_sel-1)) /* e la ic e' protetta */
         permissivo = False;
      else
         permissivo = True;
      }
   else                    /* se siamo superuser          */
      permissivo = True;
/* 
 * abilitazione tasto check
 */
   if (isSuperuser)       /* se siamo superuser     */
     {
     if ((snap_header+(ic_sel-1))->mod == 1)    /* se e' marcata mofificata da net_compi */
         {
         XtVaSetValues (icMenuEditCheck, XmNsensitive, True, NULL);
         }
     else
         {
         XtVaSetValues (icMenuEditCheck, XmNsensitive, False, NULL);
         }
     }
/*
 * per cut/copy and paste
 */
   permissivo_paste = cutCopyCiBuffer & permissivo;

   if (((snap_header+(ic_sel-1))->stat == 1) && /* snap occupato  */
       ((snap_header+(ic_sel-1))->mod == 0))     /* congruente con il simulatore */
      {
      tasto_initic_attivabile = True;
      XtVaSetValues (UxGetWidget(icSavepb), XmNsensitive, True & permissivo, NULL);
      XtVaSetValues (UxGetWidget(icViewpb), XmNsensitive, True, NULL);
      XtVaSetValues (UxGetWidget(icMenuEditCutPb), XmNsensitive, True &  permissivo, NULL);
      XtVaSetValues (UxGetWidget(icMenuEditCopy), XmNsensitive, True, NULL); 

      XtVaSetValues (UxGetWidget(IcRcPopupSave), XmNsensitive, True & permissivo, NULL);
      XtVaSetValues (UxGetWidget(IcRcPopupView), XmNsensitive, True, NULL);
      }
   else
      {
      tasto_initic_attivabile = False;
      XtVaSetValues (UxGetWidget(icSavepb), XmNsensitive, True & permissivo, NULL);
      XtVaSetValues (UxGetWidget(icViewpb), XmNsensitive, False, NULL);

      if ((snap_header+(ic_sel-1))->mod == 1) /* incongruente col simulatore */
          XtVaSetValues (UxGetWidget(icMenuEditCutPb), XmNsensitive, True, NULL);
      else
          XtVaSetValues (UxGetWidget(icMenuEditCutPb), XmNsensitive, False, NULL);

      XtVaSetValues (UxGetWidget(icMenuEditCopy), XmNsensitive, False, NULL); 

      XtVaSetValues (UxGetWidget(IcRcPopupView), XmNsensitive,False, NULL);
      XtVaSetValues (UxGetWidget(IcRcPopupSave), XmNsensitive, True & permissivo, NULL);
      }

   XtVaSetValues (UxGetWidget(icMenuEditPaste), XmNsensitive, permissivo_paste, NULL);

   prec_sel = ic_sel;
   return;

#endif
}
/******************************************************************/
#ifndef DESIGN_TIME
int aggiorna_opt_snap (w,p)
Widget w;
OPTIONS_FLAGS *p;
{
   if (! p->options_snapshot.save)
      XtUnmanageChild (UxGetWidget(icSavepb));
   else
      XtManageChild (UxGetWidget(icSavepb));

   if (! p->options_snapshot.delete)
      XtUnmanageChild (UxGetWidget(icMenuEditCutPb));
   else
      XtManageChild (UxGetWidget(icMenuEditCutPb));

   if (! p->options_snapshot.copy_and_paste)
      {
      XtUnmanageChild (UxGetWidget(icMenuEditCopy));
      XtUnmanageChild (UxGetWidget(icMenuEditPaste));
      }
   else
      {
      XtManageChild (UxGetWidget(icMenuEditCopy));
      XtManageChild (UxGetWidget(icMenuEditPaste));
      }
   return (0);   
}
#endif

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void  destroyCB_initialCondition( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	set_cursor (padre,CLOCK);
	snap_interface_active = 0;
	tasto_initic_attivabile = False;
	free (snap_header);
	stop_timer (timer_initialCondition);
	set_cursor (padre,NORMALE);
#endif
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_icMenuFilePrint( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
	stampa_ci (initialCondition);
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_icMenuFileQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
	/****
#ifndef DESIGN_TIME
	set_cursor (padre,CLOCK);
	snap_interface_active = 0;
	tasto_initic_attivabile = False;
	XtDestroyWidget (XtParent(UxGetWidget(initialCondition)));
	free (snap_header);
	stop_timer (timer_initialCondition);
	set_cursor (padre,NORMALE);
#endif
	**********/
	DistruggiInterfaccia (XtParent(initialCondition));
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_icMenuEditCutPb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	int n;
	n = ic_sel;
	   cancella_ci (UxGetWidget(initialCondition),n);
#endif
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_icMenuEditCopy( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME 
	int n;
	n = ic_sel;
	copia_ci (UxGetWidget(initialCondition),
	          UxGetWidget(icMenuEditPaste),
	          n);
#endif
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_icMenuEditPaste( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	int n;
	n = ic_sel;
	paste_ci (UxWidget, n);
#endif
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_icMenuEditCheck( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	int n;
	n = ic_sel;
	accetta_ci (UxGetWidget(initialCondition),n);
#endif
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_icMenuEditCheckAll( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	int i;
	for (i=0; i<_MAX_SNAP_SHOT; i++)
	   {
	   if ((snap_header+i)->mod == 1) /* modificato simulatore */
	      {
	      accetta_ci (UxGetWidget(initialCondition),i+1);
	      }
	   }
#endif
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_icMenuHelpCommand( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{printf ("help comandi\n");}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_icQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
	/****** in destroy callback di initialCondition
#ifndef DESIGN_TIME
	set_cursor (padre,CLOCK);
	snap_interface_active = 0;
	tasto_initic_attivabile = False;
	free (snap_header);
	stop_timer (timer_initialCondition);
	set_cursor (padre,NORMALE);
#endif
	***************/
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_icInitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	carica_ci ((Widget)UxClientData, ic_sel);
#endif
	
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_icSavepb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	salva_ci (UxGetWidget(initialCondition), ic_sel);
#endif
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_icViewpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
	char testo[20];
#ifndef DESIGN_TIME
	printf ("preview ic n. %d \n",ic_sel);
	sprintf (testo,"I.C. %d",ic_sel);
	create_previewSnap(CI_VIEW,ic_sel,testo);
#endif
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_IcRcPopupView( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
	char testo[20];
#ifndef DESIGN_TIME
	printf ("preview ic n. %d \n",ic_sel);
	sprintf (testo,"I.C. %d",ic_sel);
	create_previewSnap(CI_VIEW,ic_sel,testo);
#endif
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_IcRcPopupLoad( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	carica_ci ((Widget)UxClientData, ic_sel);
#endif
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_IcRcPopupSave( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	salva_ci (UxGetWidget(initialCondition), ic_sel);
#endif
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_IcRcPopupMenuPrint( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
	stampa_ci (initialCondition);
	}
	UxInitialConditionContext = UxSaveCtx;
}

static void  activateCB_IcRcPopupQuit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCinitialCondition    *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInitialConditionContext;
	UxInitialConditionContext = UxContext =
			(_UxCinitialCondition *) UxGetContext( UxWidget );
	{
	/************
#ifndef DESIGN_TIME
	set_cursor (padre,CLOCK);
	snap_interface_active = 0;
	tasto_initic_attivabile = False;
	free (snap_header);
	stop_timer (timer_initialCondition);
	set_cursor (padre,NORMALE);
#endif
	***********/
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
	}
	UxInitialConditionContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_initialCondition()
{
	Widget		_UxParent;
	Widget		icMenuFile_shell;
	Widget		icMenuEdit_shell;
	Widget		icMenuHelp_shell;
	Widget		IcRcPopupMenu_shell;


	/* Creation of initialCondition */
	_UxParent = XtVaCreatePopupShell( "initialCondition_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 324,
			XmNy, 90,
			XmNwidth, 650,
			XmNheight, 433,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "initialCondition",
			XmNiconName, "initialCondition",
			NULL );

	initialCondition = XtVaCreateManagedWidget( "initialCondition",
			xmFormWidgetClass,
			_UxParent,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNunitType, XmPIXELS,
			XmNwidth, 650,
			XmNheight, 433,
			XmNbuttonFontList, UxConvertFontList("9x15bold" ),
			NULL );
	XtAddCallback( initialCondition, XmNdestroyCallback,
		(XtCallbackProc) destroyCB_initialCondition,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( initialCondition, (char *) UxInitialConditionContext );


	/* Creation of icMenu */
	icMenu = XtVaCreateWidget( "icMenu",
			xmRowColumnWidgetClass,
			initialCondition,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNpacking, XmPACK_TIGHT,
			NULL );
	UxPutContext( icMenu, (char *) UxInitialConditionContext );


	/* Creation of icMenuFile */
	icMenuFile_shell = XtVaCreatePopupShell ("icMenuFile_shell",
			xmMenuShellWidgetClass, icMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	icMenuFile = XtVaCreateWidget( "icMenuFile",
			xmRowColumnWidgetClass,
			icMenuFile_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( icMenuFile, (char *) UxInitialConditionContext );


	/* Creation of icMenuFilePrint */
	icMenuFilePrint = XtVaCreateManagedWidget( "icMenuFilePrint",
			xmPushButtonWidgetClass,
			icMenuFile,
			RES_CONVERT( XmNlabelString, "Print" ),
			NULL );
	XtAddCallback( icMenuFilePrint, XmNactivateCallback,
		(XtCallbackProc) activateCB_icMenuFilePrint,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( icMenuFilePrint, (char *) UxInitialConditionContext );


	/* Creation of icMenuFile_b2 */
	icMenuFile_b2 = XtVaCreateManagedWidget( "icMenuFile_b2",
			xmSeparatorWidgetClass,
			icMenuFile,
			NULL );
	UxPutContext( icMenuFile_b2, (char *) UxInitialConditionContext );


	/* Creation of icMenuFileQuitpb */
	icMenuFileQuitpb = XtVaCreateManagedWidget( "icMenuFileQuitpb",
			xmPushButtonWidgetClass,
			icMenuFile,
			RES_CONVERT( XmNlabelString, QuitMenuLabel ),
			RES_CONVERT( XmNmnemonic, "Q" ),
			NULL );
	XtAddCallback( icMenuFileQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_icMenuFileQuitpb,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( icMenuFileQuitpb, (char *) UxInitialConditionContext );


	/* Creation of ic_menu_file */
	ic_menu_file = XtVaCreateManagedWidget( "ic_menu_file",
			xmCascadeButtonWidgetClass,
			icMenu,
			RES_CONVERT( XmNlabelString, FileMenuLabel ),
			RES_CONVERT( XmNmnemonic, "F" ),
			XmNsubMenuId, icMenuFile,
			NULL );
	UxPutContext( ic_menu_file, (char *) UxInitialConditionContext );


	/* Creation of icMenuEdit */
	icMenuEdit_shell = XtVaCreatePopupShell ("icMenuEdit_shell",
			xmMenuShellWidgetClass, icMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	icMenuEdit = XtVaCreateWidget( "icMenuEdit",
			xmRowColumnWidgetClass,
			icMenuEdit_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( icMenuEdit, (char *) UxInitialConditionContext );


	/* Creation of icMenuEditCutPb */
	icMenuEditCutPb = XtVaCreateManagedWidget( "icMenuEditCutPb",
			xmPushButtonWidgetClass,
			icMenuEdit,
			RES_CONVERT( XmNlabelString, CutMenuLabel ),
			RES_CONVERT( XmNmnemonic, "C" ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( icMenuEditCutPb, XmNactivateCallback,
		(XtCallbackProc) activateCB_icMenuEditCutPb,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( icMenuEditCutPb, (char *) UxInitialConditionContext );


	/* Creation of icMenuEditSep1 */
	icMenuEditSep1 = XtVaCreateManagedWidget( "icMenuEditSep1",
			xmSeparatorWidgetClass,
			icMenuEdit,
			XmNseparatorType, XmSHADOW_ETCHED_IN,
			NULL );
	UxPutContext( icMenuEditSep1, (char *) UxInitialConditionContext );


	/* Creation of icMenuEditCopy */
	icMenuEditCopy = XtVaCreateManagedWidget( "icMenuEditCopy",
			xmPushButtonWidgetClass,
			icMenuEdit,
			RES_CONVERT( XmNlabelString, "Copy" ),
			RES_CONVERT( XmNmnemonic, "p" ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( icMenuEditCopy, XmNactivateCallback,
		(XtCallbackProc) activateCB_icMenuEditCopy,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( icMenuEditCopy, (char *) UxInitialConditionContext );


	/* Creation of icMenuEditPaste */
	icMenuEditPaste = XtVaCreateManagedWidget( "icMenuEditPaste",
			xmPushButtonWidgetClass,
			icMenuEdit,
			RES_CONVERT( XmNlabelString, "Paste" ),
			RES_CONVERT( XmNmnemonic, "P" ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( icMenuEditPaste, XmNactivateCallback,
		(XtCallbackProc) activateCB_icMenuEditPaste,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( icMenuEditPaste, (char *) UxInitialConditionContext );


	/* Creation of icMenuEdit_b5 */
	icMenuEdit_b5 = XtVaCreateManagedWidget( "icMenuEdit_b5",
			xmSeparatorWidgetClass,
			icMenuEdit,
			NULL );
	UxPutContext( icMenuEdit_b5, (char *) UxInitialConditionContext );


	/* Creation of icMenuEditCheck */
	icMenuEditCheck = XtVaCreateManagedWidget( "icMenuEditCheck",
			xmPushButtonWidgetClass,
			icMenuEdit,
			RES_CONVERT( XmNlabelString, "Check" ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( icMenuEditCheck, XmNactivateCallback,
		(XtCallbackProc) activateCB_icMenuEditCheck,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( icMenuEditCheck, (char *) UxInitialConditionContext );


	/* Creation of icMenuEditCheckAll */
	icMenuEditCheckAll = XtVaCreateManagedWidget( "icMenuEditCheckAll",
			xmPushButtonWidgetClass,
			icMenuEdit,
			RES_CONVERT( XmNlabelString, CheckAllLabel ),
			NULL );
	XtAddCallback( icMenuEditCheckAll, XmNactivateCallback,
		(XtCallbackProc) activateCB_icMenuEditCheckAll,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( icMenuEditCheckAll, (char *) UxInitialConditionContext );


	/* Creation of ic_menu_edit */
	ic_menu_edit = XtVaCreateManagedWidget( "ic_menu_edit",
			xmCascadeButtonWidgetClass,
			icMenu,
			RES_CONVERT( XmNlabelString, EditMenuLabel ),
			RES_CONVERT( XmNmnemonic, "E" ),
			XmNsubMenuId, icMenuEdit,
			NULL );
	UxPutContext( ic_menu_edit, (char *) UxInitialConditionContext );


	/* Creation of icMenuHelp */
	icMenuHelp_shell = XtVaCreatePopupShell ("icMenuHelp_shell",
			xmMenuShellWidgetClass, icMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	icMenuHelp = XtVaCreateWidget( "icMenuHelp",
			xmRowColumnWidgetClass,
			icMenuHelp_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( icMenuHelp, (char *) UxInitialConditionContext );


	/* Creation of icMenuHelpCommand */
	icMenuHelpCommand = XtVaCreateManagedWidget( "icMenuHelpCommand",
			xmPushButtonWidgetClass,
			icMenuHelp,
			RES_CONVERT( XmNlabelString, "Commands" ),
			RES_CONVERT( XmNmnemonic, "C" ),
			NULL );
	XtAddCallback( icMenuHelpCommand, XmNactivateCallback,
		(XtCallbackProc) activateCB_icMenuHelpCommand,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( icMenuHelpCommand, (char *) UxInitialConditionContext );


	/* Creation of ic_menu_help */
	ic_menu_help = XtVaCreateManagedWidget( "ic_menu_help",
			xmCascadeButtonWidgetClass,
			icMenu,
			RES_CONVERT( XmNlabelString, HelpMenuLabel ),
			RES_CONVERT( XmNmnemonic, "H" ),
			XmNsubMenuId, icMenuHelp,
			NULL );
	UxPutContext( ic_menu_help, (char *) UxInitialConditionContext );


	/* Creation of frame8 */
	frame8 = XtVaCreateManagedWidget( "frame8",
			xmFrameWidgetClass,
			initialCondition,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 690,
			XmNheight, 80,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame8, (char *) UxInitialConditionContext );


	/* Creation of form27 */
	form27 = XtVaCreateManagedWidget( "form27",
			xmFormWidgetClass,
			frame8,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 2,
			XmNy, 20,
			XmNwidth, 851,
			XmNheight, 82,
			NULL );
	UxPutContext( form27, (char *) UxInitialConditionContext );


	/* Creation of icQuitpb */
	icQuitpb = XtVaCreateManagedWidget( "icQuitpb",
			xmPushButtonWidgetClass,
			form27,
			XmNx, 715,
			XmNy, 15,
			XmNwidth, 100,
			XmNheight, 52,
			RES_CONVERT( XmNlabelString, "QUIT" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_SELF,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNbottomOffset, 15,
			NULL );
	XtAddCallback( icQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_icQuitpb,
		UxGetWidget(initialCondition) );

	UxPutContext( icQuitpb, (char *) UxInitialConditionContext );


	/* Creation of icInitpb */
	icInitpb = XtVaCreateManagedWidget( "icInitpb",
			xmPushButtonWidgetClass,
			form27,
			XmNx, 20,
			XmNy, 15,
			XmNwidth, 100,
			XmNheight, 52,
			RES_CONVERT( XmNlabelString, "LOAD" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNrightAttachment, XmATTACH_SELF,
			XmNrightPosition, 0,
			XmNrightOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( icInitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_icInitpb,
		initialCondition );

	UxPutContext( icInitpb, (char *) UxInitialConditionContext );


	/* Creation of icSavepb */
	icSavepb = XtVaCreateManagedWidget( "icSavepb",
			xmPushButtonWidgetClass,
			form27,
			XmNx, 148,
			XmNy, 15,
			XmNwidth, 100,
			XmNheight, 52,
			RES_CONVERT( XmNlabelString, "SAVE" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 20,
			XmNleftWidget, icInitpb,
			XmNrightAttachment, XmATTACH_SELF,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( icSavepb, XmNactivateCallback,
		(XtCallbackProc) activateCB_icSavepb,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( icSavepb, (char *) UxInitialConditionContext );


	/* Creation of icViewpb */
	icViewpb = XtVaCreateManagedWidget( "icViewpb",
			xmPushButtonWidgetClass,
			form27,
			XmNx, 275,
			XmNy, 15,
			XmNwidth, 100,
			XmNheight, 52,
			RES_CONVERT( XmNlabelString, "VIEW" ),
			XmNsensitive, FALSE,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 20,
			XmNleftWidget, icSavepb,
			XmNrightAttachment, XmATTACH_SELF,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			NULL );
	XtAddCallback( icViewpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_icViewpb,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( icViewpb, (char *) UxInitialConditionContext );


	/* Creation of initialConditionScrollW */
	initialConditionScrollW = XtVaCreateManagedWidget( "initialConditionScrollW",
			xmScrolledWindowWidgetClass,
			initialCondition,
			XmNscrollingPolicy, XmAUTOMATIC,
			XmNx, 15,
			XmNy, 60,
			XmNwidth, 700,
			XmNheight, 255,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 10,
			XmNbottomWidget, frame8,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 15,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 15,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 20,
			XmNtopWidget, icMenu,
			NULL );
	UxPutContext( initialConditionScrollW, (char *) UxInitialConditionContext );


	/* Creation of IcRc */
	IcRc = XtVaCreateManagedWidget( "IcRc",
			xmRowColumnWidgetClass,
			initialConditionScrollW,
			XmNx, 2,
			XmNy, 2,
			XmNwidth, 600,
			XmNheight, 280,
			RES_CONVERT( XmNbackground, "#c2bab5" ),
			NULL );
	UxPutContext( IcRc, (char *) UxInitialConditionContext );


	/* Creation of IcRcPopupMenu */
	IcRcPopupMenu_shell = XtVaCreatePopupShell ("IcRcPopupMenu_shell",
			xmMenuShellWidgetClass, IcRc,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	IcRcPopupMenu = XtVaCreateWidget( "IcRcPopupMenu",
			xmRowColumnWidgetClass,
			IcRcPopupMenu_shell,
			XmNrowColumnType, XmMENU_POPUP,
			XmNmenuAccelerator, "Shift <KeyUp>F10",
			NULL );
	UxPutContext( IcRcPopupMenu, (char *) UxInitialConditionContext );


	/* Creation of IcRcPopupView */
	IcRcPopupView = XtVaCreateManagedWidget( "IcRcPopupView",
			xmPushButtonWidgetClass,
			IcRcPopupMenu,
			RES_CONVERT( XmNlabelString, "View " ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( IcRcPopupView, XmNactivateCallback,
		(XtCallbackProc) activateCB_IcRcPopupView,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( IcRcPopupView, (char *) UxInitialConditionContext );


	/* Creation of IcRcPopupMenu_p1_b2 */
	IcRcPopupMenu_p1_b2 = XtVaCreateManagedWidget( "IcRcPopupMenu_p1_b2",
			xmSeparatorWidgetClass,
			IcRcPopupMenu,
			NULL );
	UxPutContext( IcRcPopupMenu_p1_b2, (char *) UxInitialConditionContext );


	/* Creation of IcRcPopupLoad */
	IcRcPopupLoad = XtVaCreateManagedWidget( "IcRcPopupLoad",
			xmPushButtonWidgetClass,
			IcRcPopupMenu,
			RES_CONVERT( XmNlabelString, "Load" ),
			NULL );
	XtAddCallback( IcRcPopupLoad, XmNactivateCallback,
		(XtCallbackProc) activateCB_IcRcPopupLoad,
		initialCondition );

	UxPutContext( IcRcPopupLoad, (char *) UxInitialConditionContext );


	/* Creation of IcRcPopupSave */
	IcRcPopupSave = XtVaCreateManagedWidget( "IcRcPopupSave",
			xmPushButtonWidgetClass,
			IcRcPopupMenu,
			RES_CONVERT( XmNlabelString, SaveMenuLabel ),
			NULL );
	XtAddCallback( IcRcPopupSave, XmNactivateCallback,
		(XtCallbackProc) activateCB_IcRcPopupSave,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( IcRcPopupSave, (char *) UxInitialConditionContext );


	/* Creation of IcRcPopupMenu_p1_b5 */
	IcRcPopupMenu_p1_b5 = XtVaCreateManagedWidget( "IcRcPopupMenu_p1_b5",
			xmSeparatorWidgetClass,
			IcRcPopupMenu,
			NULL );
	UxPutContext( IcRcPopupMenu_p1_b5, (char *) UxInitialConditionContext );


	/* Creation of IcRcPopupMenuPrint */
	IcRcPopupMenuPrint = XtVaCreateManagedWidget( "IcRcPopupMenuPrint",
			xmPushButtonWidgetClass,
			IcRcPopupMenu,
			RES_CONVERT( XmNlabelString, "Print" ),
			NULL );
	XtAddCallback( IcRcPopupMenuPrint, XmNactivateCallback,
		(XtCallbackProc) activateCB_IcRcPopupMenuPrint,
		(XtPointer) UxInitialConditionContext );

	UxPutContext( IcRcPopupMenuPrint, (char *) UxInitialConditionContext );


	/* Creation of IcRcPopupMenu_p1_b7 */
	IcRcPopupMenu_p1_b7 = XtVaCreateManagedWidget( "IcRcPopupMenu_p1_b7",
			xmSeparatorWidgetClass,
			IcRcPopupMenu,
			NULL );
	UxPutContext( IcRcPopupMenu_p1_b7, (char *) UxInitialConditionContext );


	/* Creation of IcRcPopupQuit */
	IcRcPopupQuit = XtVaCreateManagedWidget( "IcRcPopupQuit",
			xmPushButtonWidgetClass,
			IcRcPopupMenu,
			RES_CONVERT( XmNlabelString, QuitMenuLabel ),
			NULL );
	XtAddCallback( IcRcPopupQuit, XmNactivateCallback,
		(XtCallbackProc) activateCB_IcRcPopupQuit,
		UxGetWidget(initialCondition) );

	UxPutContext( IcRcPopupQuit, (char *) UxInitialConditionContext );

	XtVaSetValues(icMenu,
			XmNmenuHelpWidget, ic_menu_help,
			NULL );

	XtVaSetValues(form27,
			XmNdefaultButton, icViewpb,
			NULL );


	XtAddCallback( initialCondition, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxInitialConditionContext);

	XtAddEventHandler(IcRc, ButtonPressMask,
			False, (XtEventHandler) _UxinitialConditionMenuPost, (XtPointer) IcRcPopupMenu );

	return ( initialCondition );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_initialCondition( _Uxpadre )
	Widget	_Uxpadre;
{
	Widget                  rtrn;
	_UxCinitialCondition    *UxContext;
	static int		_Uxinit = 0;

	UxInitialConditionContext = UxContext =
		(_UxCinitialCondition *) UxNewContext( sizeof(_UxCinitialCondition), False );

	padre = _Uxpadre;

	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewInterfaceClassId();
		_Uxinit = 1;
	}

	{
#ifndef DESIGN_TIME
		set_cursor (padre,CLOCK);
#endif
		cutCopyCiBuffer = False;
		rtrn = _Uxbuild_initialCondition();
		UxPutClassCode( initialCondition, _UxIfClassId );

#ifndef DESIGN_TIME
		XtManageChild (UxGetWidget(icMenu));
		AllocaAreeSnapshot ();
		aggiorna_opt_snap (UxGetWidget(IcRc), &options);
		add_item_initial_condition(UxGetWidget(IcRc));
		snap_header = (SNTAB*)malloc(sizeof(SNTAB)*_MAX_SNAP_SHOT);
		read_lista_snap (snap_header);
		display_header_snap (snap_header);
		
		colore_sfondo = read_background_color (rtrn);
		UxPopupInterface (initialCondition, no_grab);
		snap_interface_active = 1; /* memoria di interfaccia attiva */
		set_cursor (padre,NORMALE);
		tasto_initic_attivabile = False;
		nuova_attivazione = 1; /* per refresh */
		attiva_timer_initialCondition(initialCondition);
#endif
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

