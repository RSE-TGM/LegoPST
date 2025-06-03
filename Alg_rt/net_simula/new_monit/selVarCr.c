
/*******************************************************************************
	selVarCr.c

       Associated Header file: selVarCr.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/BulletinB.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/TextF.h>
#include <Xm/ArrowB.h>
#include <Xm/Label.h>
#include <Xm/PanedW.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/ToggleB.h>
#include <Xm/CascadeB.h>
#include <Xm/Separator.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/ToggleB.h>
#include <malloc.h>
#include "messaggi.h"

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "bistrutt.h"
#include "cursore.h"
#include "option.h"


#ifndef DESIGN_TIME
#include "cont_rec.h"
#endif

#include "filtri.h"
FILTRO_TIPI filtro_tipi;
extern int sel_var_cr_interface_active;
#include "file_selection.h"
char filtro_kks[12];
int modello_selezionato;
int blocco_selezionato;
int kks_filter;
int tipo_filter;
extern OPTIONS_FLAGS options;
extern Boolean *IcProt;

#include "tabelle_malf.h"
#include "banco_globals.h"

/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "selVarCr.h"
#undef CONTEXT_MACRO_ACCESS

/*******************************************************************************
       The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxselVarCrMenuPost(
			Widget wgt, 
			XtPointer client_data, 
			XEvent *event)
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

Widget	selVarCr;


int carica_filtro_tipo ();
int loadas_cr (Widget,int,int);
int saveas_cr (Widget,int,int);
int richiesta_timer_cr (Widget);
int manage_widget_filter (Widget, Widget, Boolean, int,int);
int display_lista_var (Widget,int,int,int,int);
int applicaFiltri (Widget,int);
int next_bloc (Widget, int, Widget);
int display_lista_blocchi (Widget);
int get_kks_filter (Widget);
int next_pos_to_sel (Widget, int, Widget);
int add_var (Widget, Widget);
int seleziona_all_to_sel(Widget, Widget);
int del_var (Widget, Widget);
int next_pos_to_unsel (Widget, int, Widget);
int seleziona_all_to_unsel(Widget, Widget);
int display_lista_sistema (Widget);





/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

int presetFilterDefaults (w)
Widget w;
{
	XmToggleButtonSetState (UxGetWidget(selByAllVariablestb),options.optionsFilterCr.ByAllVar,False);
	XmToggleButtonSetState (UxGetWidget(selByModeltb),options.optionsFilterCr.ByModBlock,False);
	XmToggleButtonSetState (UxGetWidget(selByModelOnlytb),options.optionsFilterCr.ByMod,False);
	XmToggleButtonSetState (UxGetWidget(selVarMenuKkstb),options.optionsFilterCr.ByKks,False);
	XmToggleButtonSetState (UxGetWidget(selVarMenuTipotb),options.optionsFilterCr.ByType,False);

	XmToggleButtonSetState (UxGetWidget(selVarCrAllVartb),options.optionsFilterCr.ByAllVar,False);
	XmToggleButtonSetState (UxGetWidget(selVarCrModBlocktb),options.optionsFilterCr.ByModBlock,False);
	XmToggleButtonSetState (UxGetWidget(selVarCrModtb),options.optionsFilterCr.ByMod,False);
	XmToggleButtonSetState (UxGetWidget(selVarCrKkstb),options.optionsFilterCr.ByKks,False);
	XmToggleButtonSetState (UxGetWidget(selVarCrTypetb),options.optionsFilterCr.ByType,False);

	return (0);
}
/******************************************************************/
int set_var_cr_defaults (w)
Widget w;
{
Boolean stato;
#ifndef DESIGN_TIME
/* legge lo stato del toggle button di filtro */
   stato = XmToggleButtonGetState (UxGetWidget(selByAllVariablestb)); 
   switch (stato)
      {
      case 0:
         modello_selezionato = 1;
         blocco_selezionato = 1;
         XtManageChild (UxGetWidget(selByModelForm));
         break;
      case 1:
         modello_selezionato = NO_MOD;
         blocco_selezionato = NO_BLOCK;
         XtUnmanageChild (UxGetWidget(selByModelForm));
         break;
      }
/* legge lo stato del toggle button di filtro */
   stato = XmToggleButtonGetState (UxGetWidget(selByModeltb)); 
   switch (stato)
      {
      case 1:
         modello_selezionato = 1;
         blocco_selezionato = 1;
         XtManageChild (UxGetWidget(selByModelForm));
         break;
      case 0:
         modello_selezionato = NO_MOD;
         blocco_selezionato = NO_BLOCK;
         XtUnmanageChild (UxGetWidget(selByModelForm));
         break;
      }
/* legge lo stato del toggle button del filtro kks */
    stato = XmToggleButtonGetState (UxGetWidget(selVarMenuKkstb));  
   switch (stato)
      {
      case 1:
         kks_filter = 1;
         XtManageChild (UxGetWidget(selVarCrKksForm));
         break;
      case 0:
         kks_filter = 0;
         XtUnmanageChild (UxGetWidget(selVarCrKksForm));
         break;
      }
/* legge lo stato del toggle button del filtro per tipo */
    stato = XmToggleButtonGetState (UxGetWidget(selVarMenuTipotb));  
   switch (stato)
      {
      case 1:
         tipo_filter = 1;
         carica_filtro_tipo ();
         XtManageChild (UxGetWidget(selVarCrTipoForm));
         break;
      case 0:
         tipo_filter = 0;
         XtUnmanageChild (UxGetWidget(selVarCrTipoForm));
         break;
      }
#endif
   return (0);
}
/*************************************************/
int carica_filtro_tipo ()
{
#ifndef DESIGN_TIME
   filtro_tipi.pressione = XmToggleButtonGetState (selVarCrTipoPressionetb);
   filtro_tipi.temperatura = XmToggleButtonGetState (selVarCrTipoTemperaturatb);
   filtro_tipi.entalpia = XmToggleButtonGetState (selVarCrTipoEntalpiatb);
   filtro_tipi.portata = XmToggleButtonGetState (selVarCrTipoPortatatb);
   filtro_tipi.potenza = XmToggleButtonGetState (selVarCrTipoPotenzatb);
   filtro_tipi.giri = XmToggleButtonGetState (selVarCrTipoGiritb);
   filtro_tipi.alzvalv = XmToggleButtonGetState (selVarCrTipoAlzvalvtb);
   filtro_tipi.ingressi = XmToggleButtonGetState (selVarCrTipoInput1);
   filtro_tipi.uscite = XmToggleButtonGetState (selVarCrTipoOutput1);
   filtro_tipi.aingabili = XmToggleButtonGetState (selVarCrTipoAingable1);
/* printf ("tipi:\n pressione %d \n \
                 temperatura %d \n \
                 entalpia %d \n \
                 portata %d \n \
                 potenza %d \n \
                 giri %d \n \
                 alzvalv %d\n \
                 input %d \n \
                 output %d \n \
                 aing %d \n",
                 filtro_tipi.pressione,filtro_tipi.temperatura,filtro_tipi.entalpia,
                 filtro_tipi.portata,filtro_tipi.potenza,filtro_tipi.giri,
                 filtro_tipi.alzvalv,filtro_tipi.ingressi,filtro_tipi.uscite,
                 filtro_tipi.aingabili); */
#endif
   return (0);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	destroyCB_selVarCr(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	sel_var_cr_interface_active = 0;
	set_var_cr_defaults (selVarCr);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarCrMenu_b4(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	loadas_cr (selVarCr,1,VAR_FOR_CR); /* 1=in lettura */
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarCrMenu_b3(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	saveas_cr (selVarCr,2,VAR_FOR_CR);  /* 2 = in scrittura */
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarCrMenuFilepb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	sel_var_cr_interface_active = 0;
	set_var_cr_defaults (UxGetWidget(selVarCr));
	DistruggiInterfaccia (XtParent(UxGetWidget(selVarCr)));
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_SelVarCrMenuEditTimer(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	richiesta_timer_cr (selVarCr);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selByAllVariablestb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVarCr),
	                  UxGetWidget(selByModelForm), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                   FILTER_ALL_VAR,FILTRO_CR);
	options.optionsFilterCr.ByAllVar = ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set;
	display_lista_var (UxGetWidget(selVarCr), modello_selezionato, blocco_selezionato,
	                     kks_filter,tipo_filter);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selByModeltb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVarCr),
	                  UxGetWidget(selByModelForm), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                   FILTER_MOD_BLOC,FILTRO_CR);
	options.optionsFilterCr.ByModBlock = ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set;
	display_lista_var (UxGetWidget(selVarCr), modello_selezionato, blocco_selezionato,
	                     kks_filter,tipo_filter);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	armCB_selByModeltb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	createCB_selByModeltb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxContext = UxSelVarCrContext;
	{
	
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selByModelOnlytb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVarCr),
	                  UxGetWidget(selByModelForm), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                   FILTER_MOD_ONLY,FILTRO_CR);
	options.optionsFilterCr.ByMod = ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set;
	display_lista_var (UxGetWidget(selVarCr), modello_selezionato, blocco_selezionato,
	                     kks_filter,tipo_filter);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarMenuKkstb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVarCr),
	                  UxGetWidget(selVarCrKksForm), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                  FILTER_KKS,FILTRO_CR);
	options.optionsFilterCr.ByKks = ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set;
	display_lista_var (UxGetWidget(selVarCr),
	                  modello_selezionato, blocco_selezionato,
	                  kks_filter,tipo_filter);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarMenuTipotb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVarCr),
	                  UxGetWidget(selVarCrTipoForm), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                  FILTER_TYPE,FILTRO_CR);
	options.optionsFilterCr.ByType = ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set;
	display_lista_var (UxGetWidget(selVarCr),
	                  modello_selezionato, blocco_selezionato,
	                  kks_filter,tipo_filter);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarMenuFilter_b2(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	if (SD_optsave3 (BANCO, (char*)&options,IcProt) > 0)
	      fprintf (stderr,"OPZIONI salvate\n");
	else
	      fprintf (stderr,"*** errore save opzioni\n");
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarOkpb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	set_cursor ((Widget)UxClientData, CLOCK);
	salva_sel_var (UxWidget, SEL_REG, FILE_REC_DEFAULT);
	newvarcr ((Widget)UxClientData);
	set_cursor ((Widget)UxClientData, NORMALE);
	/*** in destroy callback
	sel_var_cr_interface_active = 0;
	set_var_cr_defaults((Widget)UxClientData);
	***/
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
#endif
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarQuitpb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	/** in destroy callback
	   sel_var_cr_interface_active = 0;
	   set_var_cr_defaults ((Widget)UxClientData);
	***/
	   DistruggiInterfaccia (XtParent((Widget)UxClientData));
#endif
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrAllVartb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVarCr),
	                  UxGetWidget(selByModelForm), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                   FILTER_ALL_VAR,FILTRO_CR2);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrModBlocktb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVarCr),
	                  UxGetWidget(selByModelForm), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                   FILTER_MOD_BLOC,FILTRO_CR2);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrModtb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVarCr),
	                  UxGetWidget(selByModelForm), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                   FILTER_MOD_ONLY,FILTRO_CR2);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrKkstb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVarCr),
	                  UxGetWidget(selVarCrKksForm), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                  FILTER_KKS,FILTRO_CR2);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrTypetb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVarCr),
	                  UxGetWidget(selVarCrTipoForm), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                  FILTER_TYPE,FILTRO_CR2);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarCrSaveFilterpb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	if (SD_optsave3 (BANCO, (char*)&options,IcProt) > 0)
	      fprintf (stderr,"OPZIONI salvate\n");
	else
	      fprintf (stderr,"*** errore save opzioni\n");
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarCrApplyFilterpb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	applicaFiltri (selVarCr, FILTRO_CR2);
	
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarFindDown2(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	next_bloc (selVarListaBlocchi,SEARCH_DOWN,selVarText3);
#endif
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarFindUp2(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	next_bloc (selVarListaBlocchi,SEARCH_UP,selVarText3);
#endif
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	singleSelectionCB_selVarListaSistemi(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	XmListCallbackStruct *reason;
	   reason = (XmListCallbackStruct *)UxCallbackArg;
	   modello_selezionato = reason->item_position;
	
	printf ("selezionato modello %d\n",modello_selezionato);
	/*   display_lista_sistema (selVarListaSistemi); */
	   display_lista_blocchi (selVarListaBlocchi); 
	   display_lista_var (selVarListaSistemi, modello_selezionato, blocco_selezionato,
	                      kks_filter,tipo_filter);
#endif
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	singleSelectionCB_selVarListaBlocchi(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	XmListCallbackStruct *reason;
	reason = (XmListCallbackStruct *)UxCallbackArg;
	
	   blocco_selezionato = reason->item_position;
	printf ("selezionato blocco %d\n",blocco_selezionato);
	   display_lista_var (selVarListaSistemi, modello_selezionato, blocco_selezionato,
	                      kks_filter,tipo_filter);
#endif
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_pushButton1(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	get_kks_filter (selVarCr);
	display_lista_var (selVarCr, modello_selezionato, blocco_selezionato,
	                 kks_filter,tipo_filter);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrTipoPressionetb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	carica_filtro_tipo();
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrTipoEntalpiatb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	carica_filtro_tipo();
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrTipoPortatatb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	carica_filtro_tipo();
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrTipoGiritb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	carica_filtro_tipo();
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrTipoTemperaturatb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	carica_filtro_tipo();
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrTipoAlzvalvtb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	carica_filtro_tipo();
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrTipoPotenzatb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	carica_filtro_tipo();
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_pushButton2(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	carica_filtro_tipo(); 
	display_lista_var (selVarCr, modello_selezionato, blocco_selezionato,
	            kks_filter,tipo_filter);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrTipoAingable1(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	carica_filtro_tipo ();
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrTipoInput1(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	carica_filtro_tipo ();
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	valueChangedCB_selVarCrTipoOutput1(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	carica_filtro_tipo ();
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarFindUp(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	next_pos_to_sel (selVarToSelect,SEARCH_UP,selVarText);
#endif
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarFindDown(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	next_pos_to_sel (selVarToSelect,SEARCH_DOWN,selVarText);
#endif
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_aggiungiVarSelpb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	add_var (UxGetWidget(selVarToSelect), UxGetWidget(selVarToUnselect));
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selezionaAllpb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	set_cursor (padre,CLOCK);
	seleziona_all_to_sel (UxGetWidget(selVarToSelect),
	                (Widget)UxClientData);
	set_cursor (padre,NORMALE);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_deleteVarSelpb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	del_var (UxGetWidget(selVarToUnselect), UxGetWidget(selVarToSelect));
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_unselVarFindUp(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	next_pos_to_unsel (selVarToUnselect,SEARCH_UP,unselVarText);
#endif
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_unselVarFindDown(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	next_pos_to_unsel (selVarToUnselect,SEARCH_DOWN,unselVarText);
#endif
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_deselezionaAllpb(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	set_cursor (padre,CLOCK);
	seleziona_all_to_unsel (UxGetWidget (selVarToUnselect),
	                (Widget)UxClientData);
	set_cursor (padre,NORMALE);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarCrPopupSaveFilter(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	if (SD_optsave3 (BANCO, (char*)&options,IcProt) > 0)
	      fprintf (stderr,"OPZIONI salvate\n");
	else
	      fprintf (stderr,"*** errore save opzioni\n");
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarCrPopupApply(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	applicaFiltri (selVarCr, FILTRO_CR2);
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarCrPopupSave(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	set_cursor ((Widget)UxClientData, CLOCK);
	salva_sel_var (UxWidget, SEL_REG, FILE_REC_DEFAULT);
	newvarcr ((Widget)UxClientData);
	set_cursor ((Widget)UxClientData, NORMALE);
	sel_var_cr_interface_active = 0;
	set_var_cr_defaults((Widget)UxClientData);
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
#endif
	}
	UxSelVarCrContext = UxSaveCtx;
}

static	void	activateCB_selVarCrPopupQuit(
			Widget wgt, 
			XtPointer cd, 
			XtPointer cb)
{
	_UxCselVarCr            *UxSaveCtx, *UxContext;
	Widget                  UxWidget = wgt;
	XtPointer               UxClientData = cd;
	XtPointer               UxCallbackArg = cb;

	UxSaveCtx = UxSelVarCrContext;
	UxSelVarCrContext = UxContext =
			(_UxCselVarCr *) UxGetContext( UxWidget );
	{
	DistruggiInterfaccia (XtParent(selVarCr));
	}
	UxSelVarCrContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_selVarCr()
{
	Widget		_UxParent;
	Widget		selVarCrMenu_shell;
	Widget		selVarCrMenuEdit_shell;
	Widget		selVarMenuFilter_shell;
	Widget		selVarMenuOtherPane_shell;
	Widget		selVarCrPopup_shell;
	char		*UxTmp0;


	/* Creation of selVarCr */
	_UxParent = XtVaCreatePopupShell( "selVarCr_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 730,
			XmNy, 350,
			XmNwidth, 540,
			XmNheight, 800,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "selVarCr",
			NULL );

	selVarCr = XtVaCreateWidget( "selVarCr",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 540,
			XmNheight, 800,
			XmNresizePolicy, XmRESIZE_GROW,
			NULL );
	XtAddCallback( selVarCr, XmNdestroyCallback,
		(XtCallbackProc) destroyCB_selVarCr,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCr, (char *) UxSelVarCrContext );


	/* Creation of menu3 */
	menu3 = XtVaCreateManagedWidget( "menu3",
			xmRowColumnWidgetClass,
			selVarCr,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( menu3, (char *) UxSelVarCrContext );


	/* Creation of selVarCrMenu */
	selVarCrMenu_shell = XtVaCreatePopupShell ("selVarCrMenu_shell",
			xmMenuShellWidgetClass, menu3,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	selVarCrMenu = XtVaCreateWidget( "selVarCrMenu",
			xmRowColumnWidgetClass,
			selVarCrMenu_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( selVarCrMenu, (char *) UxSelVarCrContext );


	/* Creation of selVarCrMenu_b4 */
	selVarCrMenu_b4 = XtVaCreateManagedWidget( "selVarCrMenu_b4",
			xmPushButtonWidgetClass,
			selVarCrMenu,
			RES_CONVERT( XmNlabelString, "Load ..." ),
			NULL );
	XtAddCallback( selVarCrMenu_b4, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarCrMenu_b4,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrMenu_b4, (char *) UxSelVarCrContext );


	/* Creation of selVarCrMenu_b3 */
	selVarCrMenu_b3 = XtVaCreateManagedWidget( "selVarCrMenu_b3",
			xmPushButtonWidgetClass,
			selVarCrMenu,
			RES_CONVERT( XmNlabelString, "Save ..." ),
			NULL );
	XtAddCallback( selVarCrMenu_b3, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarCrMenu_b3,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrMenu_b3, (char *) UxSelVarCrContext );


	/* Creation of selVarCrMenu_b2 */
	selVarCrMenu_b2 = XtVaCreateManagedWidget( "selVarCrMenu_b2",
			xmSeparatorWidgetClass,
			selVarCrMenu,
			NULL );
	UxPutContext( selVarCrMenu_b2, (char *) UxSelVarCrContext );


	/* Creation of selVarCrMenuFilepb */
	selVarCrMenuFilepb = XtVaCreateManagedWidget( "selVarCrMenuFilepb",
			xmPushButtonWidgetClass,
			selVarCrMenu,
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );
	XtAddCallback( selVarCrMenuFilepb, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarCrMenuFilepb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrMenuFilepb, (char *) UxSelVarCrContext );


	/* Creation of menu3_top_b1 */
	menu3_top_b1 = XtVaCreateManagedWidget( "menu3_top_b1",
			xmCascadeButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "File" ),
			XmNsubMenuId, selVarCrMenu,
			NULL );
	UxPutContext( menu3_top_b1, (char *) UxSelVarCrContext );


	/* Creation of selVarCrMenuEdit */
	selVarCrMenuEdit_shell = XtVaCreatePopupShell ("selVarCrMenuEdit_shell",
			xmMenuShellWidgetClass, menu3,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	selVarCrMenuEdit = XtVaCreateWidget( "selVarCrMenuEdit",
			xmRowColumnWidgetClass,
			selVarCrMenuEdit_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( selVarCrMenuEdit, (char *) UxSelVarCrContext );


	/* Creation of SelVarCrMenuEditTimer */
	SelVarCrMenuEditTimer = XtVaCreateManagedWidget( "SelVarCrMenuEditTimer",
			xmPushButtonWidgetClass,
			selVarCrMenuEdit,
			RES_CONVERT( XmNlabelString, "Set Record. Freq. ..." ),
			NULL );
	XtAddCallback( SelVarCrMenuEditTimer, XmNactivateCallback,
		(XtCallbackProc) activateCB_SelVarCrMenuEditTimer,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( SelVarCrMenuEditTimer, (char *) UxSelVarCrContext );


	/* Creation of menu3_top_b5 */
	menu3_top_b5 = XtVaCreateManagedWidget( "menu3_top_b5",
			xmCascadeButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Edit" ),
			XmNsubMenuId, selVarCrMenuEdit,
			NULL );
	UxPutContext( menu3_top_b5, (char *) UxSelVarCrContext );


	/* Creation of selVarMenuFilter */
	selVarMenuFilter_shell = XtVaCreatePopupShell ("selVarMenuFilter_shell",
			xmMenuShellWidgetClass, menu3,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	selVarMenuFilter = XtVaCreateWidget( "selVarMenuFilter",
			xmRowColumnWidgetClass,
			selVarMenuFilter_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( selVarMenuFilter, (char *) UxSelVarCrContext );


	/* Creation of selVarMenuOtherPane */
	selVarMenuOtherPane_shell = XtVaCreatePopupShell ("selVarMenuOtherPane_shell",
			xmMenuShellWidgetClass, selVarMenuFilter,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	selVarMenuOtherPane = XtVaCreateWidget( "selVarMenuOtherPane",
			xmRowColumnWidgetClass,
			selVarMenuOtherPane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( selVarMenuOtherPane, (char *) UxSelVarCrContext );


	/* Creation of selByAllVariablestb */
	selByAllVariablestb = XtVaCreateManagedWidget( "selByAllVariablestb",
			xmToggleButtonWidgetClass,
			selVarMenuOtherPane,
			RES_CONVERT( XmNlabelString, "All variables" ),
			XmNindicatorType, XmONE_OF_MANY,
			XmNvisibleWhenOff, TRUE,
			XmNset, FALSE,
			NULL );
	XtAddCallback( selByAllVariablestb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selByAllVariablestb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selByAllVariablestb, (char *) UxSelVarCrContext );


	/* Creation of selByModeltb */
	selByModeltb = XtVaCreateManagedWidget( "selByModeltb",
			xmToggleButtonWidgetClass,
			selVarMenuOtherPane,
			RES_CONVERT( XmNlabelString, "models & blocks" ),
			XmNset, TRUE,
			XmNvisibleWhenOff, TRUE,
			XmNindicatorType, XmONE_OF_MANY,
			NULL );
	XtAddCallback( selByModeltb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selByModeltb,
		(XtPointer) UxSelVarCrContext );
	XtAddCallback( selByModeltb, XmNarmCallback,
		(XtCallbackProc) armCB_selByModeltb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selByModeltb, (char *) UxSelVarCrContext );

	createCB_selByModeltb( selByModeltb,
			(XtPointer) UxSelVarCrContext, (XtPointer) NULL );


	/* Creation of selByModelOnlytb */
	selByModelOnlytb = XtVaCreateManagedWidget( "selByModelOnlytb",
			xmToggleButtonWidgetClass,
			selVarMenuOtherPane,
			RES_CONVERT( XmNlabelString, "models only" ),
			XmNindicatorType, XmONE_OF_MANY,
			XmNvisibleWhenOff, TRUE,
			NULL );
	XtAddCallback( selByModelOnlytb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selByModelOnlytb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selByModelOnlytb, (char *) UxSelVarCrContext );


	/* Creation of selVarMenuOtherPane_b6 */
	selVarMenuOtherPane_b6 = XtVaCreateManagedWidget( "selVarMenuOtherPane_b6",
			xmSeparatorWidgetClass,
			selVarMenuOtherPane,
			NULL );
	UxPutContext( selVarMenuOtherPane_b6, (char *) UxSelVarCrContext );


	/* Creation of selVarMenuKkstb */
	selVarMenuKkstb = XtVaCreateManagedWidget( "selVarMenuKkstb",
			xmToggleButtonWidgetClass,
			selVarMenuOtherPane,
			RES_CONVERT( XmNlabelString, "KKS filter" ),
			XmNset, TRUE,
			XmNvisibleWhenOff, TRUE,
			XmNindicatorType, XmN_OF_MANY,
			NULL );
	XtAddCallback( selVarMenuKkstb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarMenuKkstb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarMenuKkstb, (char *) UxSelVarCrContext );


	/* Creation of selVarMenuOtherPane_b3 */
	selVarMenuOtherPane_b3 = XtVaCreateManagedWidget( "selVarMenuOtherPane_b3",
			xmSeparatorWidgetClass,
			selVarMenuOtherPane,
			NULL );
	UxPutContext( selVarMenuOtherPane_b3, (char *) UxSelVarCrContext );


	/* Creation of selVarMenuTipotb */
	selVarMenuTipotb = XtVaCreateManagedWidget( "selVarMenuTipotb",
			xmToggleButtonWidgetClass,
			selVarMenuOtherPane,
			RES_CONVERT( XmNlabelString, "by type" ),
			XmNvisibleWhenOff, TRUE,
			NULL );
	XtAddCallback( selVarMenuTipotb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarMenuTipotb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarMenuTipotb, (char *) UxSelVarCrContext );


	/* Creation of selVarMenuFilter_b8 */
	selVarMenuFilter_b8 = XtVaCreateManagedWidget( "selVarMenuFilter_b8",
			xmCascadeButtonWidgetClass,
			selVarMenuFilter,
			RES_CONVERT( XmNlabelString, "Select filter ..." ),
			XmNsubMenuId, selVarMenuOtherPane,
			NULL );
	UxPutContext( selVarMenuFilter_b8, (char *) UxSelVarCrContext );


	/* Creation of selVarMenuFilter_b2 */
	selVarMenuFilter_b2 = XtVaCreateManagedWidget( "selVarMenuFilter_b2",
			xmPushButtonWidgetClass,
			selVarMenuFilter,
			RES_CONVERT( XmNlabelString, "Save Selection" ),
			NULL );
	XtAddCallback( selVarMenuFilter_b2, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarMenuFilter_b2,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarMenuFilter_b2, (char *) UxSelVarCrContext );


	/* Creation of menu3_top_b2 */
	menu3_top_b2 = XtVaCreateWidget( "menu3_top_b2",
			xmCascadeButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Filter" ),
			XmNsubMenuId, selVarMenuFilter,
			NULL );
	UxPutContext( menu3_top_b2, (char *) UxSelVarCrContext );


	/* Creation of frame1 */
	frame1 = XtVaCreateManagedWidget( "frame1",
			xmFrameWidgetClass,
			selVarCr,
			XmNx, 0,
			XmNy, 480,
			XmNwidth, 540,
			XmNheight, 80,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_NONE,
			XmNtopOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame1, (char *) UxSelVarCrContext );


	/* Creation of form39 */
	form39 = XtVaCreateManagedWidget( "form39",
			xmFormWidgetClass,
			frame1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 10,
			XmNy, 10,
			XmNwidth, 490,
			XmNheight, 50,
			NULL );
	UxPutContext( form39, (char *) UxSelVarCrContext );


	/* Creation of selVarOkpb */
	selVarOkpb = XtVaCreateManagedWidget( "selVarOkpb",
			xmPushButtonWidgetClass,
			form39,
			XmNx, 20,
			XmNy, 20,
			XmNwidth, 100,
			XmNheight, 40,
			XmNtopOffset, 15,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNrightAttachment, XmATTACH_SELF,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, CONF_ACCEPT ),
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			NULL );
	XtAddCallback( selVarOkpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarOkpb,
		selVarCr );

	UxPutContext( selVarOkpb, (char *) UxSelVarCrContext );


	/* Creation of selVarQuitpb */
	selVarQuitpb = XtVaCreateManagedWidget( "selVarQuitpb",
			xmPushButtonWidgetClass,
			form39,
			XmNx, 410,
			XmNy, 20,
			XmNwidth, 100,
			XmNheight, 40,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftAttachment, XmATTACH_SELF,
			XmNleftOffset, 0,
			XmNleftPosition, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			RES_CONVERT( XmNlabelString, CONF_DISCARD ),
			NULL );
	XtAddCallback( selVarQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarQuitpb,
		UxGetWidget (selVarCr) );

	UxPutContext( selVarQuitpb, (char *) UxSelVarCrContext );


	/* Creation of frame24 */
	frame24 = XtVaCreateManagedWidget( "frame24",
			xmFrameWidgetClass,
			selVarCr,
			XmNx, 0,
			XmNy, 480,
			XmNwidth, 540,
			XmNheight, 80,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNbottomWidget, NULL,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 0,
			XmNtopWidget, menu3,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame24, (char *) UxSelVarCrContext );


	/* Creation of selVarCrFilterForm */
	selVarCrFilterForm = XtVaCreateManagedWidget( "selVarCrFilterForm",
			xmFormWidgetClass,
			frame24,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 2,
			XmNy, 2,
			XmNwidth, 536,
			XmNheight, 58,
			NULL );
	UxPutContext( selVarCrFilterForm, (char *) UxSelVarCrContext );


	/* Creation of frame26 */
	frame26 = XtVaCreateManagedWidget( "frame26",
			xmFrameWidgetClass,
			selVarCrFilterForm,
			XmNx, 210,
			XmNy, 10,
			XmNwidth, 150,
			XmNheight, 60,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 2,
			XmNleftAttachment, XmATTACH_NONE,
			XmNleftOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 2,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 2,
			NULL );
	UxPutContext( frame26, (char *) UxSelVarCrContext );


	/* Creation of form50 */
	form50 = XtVaCreateManagedWidget( "form50",
			xmFormWidgetClass,
			frame26,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 50,
			XmNy, 10,
			XmNwidth, 80,
			XmNheight, 30,
			NULL );
	UxPutContext( form50, (char *) UxSelVarCrContext );


	/* Creation of selVarCrAllVartb */
	selVarCrAllVartb = XtVaCreateManagedWidget( "selVarCrAllVartb",
			xmToggleButtonWidgetClass,
			form50,
			XmNx, 10,
			XmNy, 0,
			XmNwidth, 128,
			XmNheight, 20,
			XmNindicatorType, XmONE_OF_MANY,
			RES_CONVERT( XmNlabelString, "All Variables" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	XtAddCallback( selVarCrAllVartb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrAllVartb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrAllVartb, (char *) UxSelVarCrContext );


	/* Creation of selVarCrModBlocktb */
	selVarCrModBlocktb = XtVaCreateManagedWidget( "selVarCrModBlocktb",
			xmToggleButtonWidgetClass,
			form50,
			XmNx, 10,
			XmNy, 20,
			XmNwidth, 128,
			XmNheight, 20,
			XmNindicatorType, XmONE_OF_MANY,
			RES_CONVERT( XmNlabelString, "Mod.&Blocks" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	XtAddCallback( selVarCrModBlocktb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrModBlocktb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrModBlocktb, (char *) UxSelVarCrContext );


	/* Creation of selVarCrModtb */
	selVarCrModtb = XtVaCreateManagedWidget( "selVarCrModtb",
			xmToggleButtonWidgetClass,
			form50,
			XmNx, 10,
			XmNy, 40,
			XmNwidth, 130,
			XmNheight, 20,
			XmNindicatorType, XmONE_OF_MANY,
			RES_CONVERT( XmNlabelString, "Models Only" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	XtAddCallback( selVarCrModtb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrModtb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrModtb, (char *) UxSelVarCrContext );


	/* Creation of frame25 */
	frame25 = XtVaCreateManagedWidget( "frame25",
			xmFrameWidgetClass,
			selVarCrFilterForm,
			XmNx, 40,
			XmNy, 20,
			XmNwidth, 150,
			XmNheight, 60,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 2,
			XmNleftAttachment, XmATTACH_NONE,
			XmNleftOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 2,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 2,
			XmNrightWidget, frame26,
			NULL );
	UxPutContext( frame25, (char *) UxSelVarCrContext );


	/* Creation of form49 */
	form49 = XtVaCreateManagedWidget( "form49",
			xmFormWidgetClass,
			frame25,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 10,
			XmNy, 5,
			XmNwidth, 138,
			XmNheight, 56,
			NULL );
	UxPutContext( form49, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKkstb */
	selVarCrKkstb = XtVaCreateManagedWidget( "selVarCrKkstb",
			xmToggleButtonWidgetClass,
			form49,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 100,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "by KKS" ),
			NULL );
	XtAddCallback( selVarCrKkstb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrKkstb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrKkstb, (char *) UxSelVarCrContext );


	/* Creation of selVarCrTypetb */
	selVarCrTypetb = XtVaCreateManagedWidget( "selVarCrTypetb",
			xmToggleButtonWidgetClass,
			form49,
			XmNx, 0,
			XmNy, 20,
			XmNwidth, 100,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "by Type" ),
			NULL );
	XtAddCallback( selVarCrTypetb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTypetb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrTypetb, (char *) UxSelVarCrContext );


	/* Creation of selVarCrSaveFilterpb */
	selVarCrSaveFilterpb = XtVaCreateManagedWidget( "selVarCrSaveFilterpb",
			xmPushButtonWidgetClass,
			selVarCrFilterForm,
			XmNx, 10,
			XmNy, 10,
			XmNwidth, 190,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, SAVEFILTER ),
			NULL );
	XtAddCallback( selVarCrSaveFilterpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarCrSaveFilterpb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrSaveFilterpb, (char *) UxSelVarCrContext );


	/* Creation of selVarCrApplyFilterpb */
	selVarCrApplyFilterpb = XtVaCreateManagedWidget( "selVarCrApplyFilterpb",
			xmPushButtonWidgetClass,
			selVarCrFilterForm,
			XmNx, 10,
			XmNy, 40,
			XmNwidth, 190,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, APPLYFILTER ),
			NULL );
	XtAddCallback( selVarCrApplyFilterpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarCrApplyFilterpb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrApplyFilterpb, (char *) UxSelVarCrContext );


	/* Creation of panedWindow1 */
	panedWindow1 = XtVaCreateManagedWidget( "panedWindow1",
			xmPanedWindowWidgetClass,
			selVarCr,
			XmNx, 0,
			XmNy, 110,
			XmNwidth, 540,
			XmNheight, 390,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, frame1,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, frame24,
			NULL );
	UxPutContext( panedWindow1, (char *) UxSelVarCrContext );


	/* Creation of selByModelForm */
	selByModelForm = XtVaCreateManagedWidget( "selByModelForm",
			xmFormWidgetClass,
			panedWindow1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 10,
			XmNwidth, 540,
			XmNheight, 150,
			XmNpaneMinimum, 50,
			NULL );
	UxPutContext( selByModelForm, (char *) UxSelVarCrContext );


	/* Creation of label5 */
	label5 = XtVaCreateManagedWidget( "label5",
			xmLabelWidgetClass,
			selByModelForm,
			XmNx, 220,
			XmNy, 0,
			XmNwidth, 170,
			XmNheight, 30,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopOffset, 0,
			RES_CONVERT( XmNlabelString, BLOCK_LIST ),
			NULL );
	UxPutContext( label5, (char *) UxSelVarCrContext );

	UxTmp0 = SYSTEM_LIST ? SYSTEM_LIST : "SYSTEMS";

	/* Creation of label7 */
	label7 = XtVaCreateManagedWidget( "label7",
			xmLabelWidgetClass,
			selByModelForm,
			XmNx, 20,
			XmNy, 0,
			XmNwidth, 170,
			XmNheight, 30,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, UxTmp0 ),
			NULL );
	UxPutContext( label7, (char *) UxSelVarCrContext );


	/* Creation of form44 */
	form44 = XtVaCreateManagedWidget( "form44",
			xmFormWidgetClass,
			selByModelForm,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 120,
			XmNwidth, 534,
			XmNheight, 30,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( form44, (char *) UxSelVarCrContext );


	/* Creation of selVarFindDown2 */
	selVarFindDown2 = XtVaCreateManagedWidget( "selVarFindDown2",
			xmArrowButtonWidgetClass,
			form44,
			XmNx, 250,
			XmNy, 0,
			XmNwidth, 60,
			XmNheight, 30,
			XmNarrowDirection, XmARROW_DOWN,
			XmNleftAttachment, XmATTACH_NONE,
			XmNleftOffset, 0,
			XmNleftWidget, NULL,
			XmNbottomOffset, 3,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 3,
			XmNrightAttachment, XmATTACH_FORM,
			NULL );
	XtAddCallback( selVarFindDown2, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarFindDown2,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarFindDown2, (char *) UxSelVarCrContext );


	/* Creation of selVarText3 */
	selVarText3 = XtVaCreateManagedWidget( "selVarText3",
			xmTextFieldWidgetClass,
			form44,
			XmNx, 20,
			XmNy, 10,
			XmNwidth, 140,
			XmNheight, 20,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_NONE,
			XmNleftOffset, 0,
			XmNleftWidget, NULL,
			XmNbottomOffset, 0,
			XmNtopOffset, 0,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightWidget, selVarFindDown2,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarText3, (char *) UxSelVarCrContext );


	/* Creation of selVarFindUp2 */
	selVarFindUp2 = XtVaCreateManagedWidget( "selVarFindUp2",
			xmArrowButtonWidgetClass,
			form44,
			XmNx, 10,
			XmNy, 0,
			XmNwidth, 60,
			XmNheight, 30,
			XmNarrowDirection, XmARROW_UP,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomOffset, 3,
			XmNtopOffset, 3,
			XmNleftAttachment, XmATTACH_NONE,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightWidget, selVarText3,
			NULL );
	XtAddCallback( selVarFindUp2, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarFindUp2,
		(XtPointer) 0x0 );

	UxPutContext( selVarFindUp2, (char *) UxSelVarCrContext );


	/* Creation of scrolledWindow6 */
	scrolledWindow6 = XtVaCreateManagedWidget( "scrolledWindow6",
			xmScrolledWindowWidgetClass,
			selByModelForm,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 0,
			XmNy, 30,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 5,
			XmNleftOffset, 20,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, label7,
			XmNwidth, 180,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, form44,
			NULL );
	UxPutContext( scrolledWindow6, (char *) UxSelVarCrContext );


	/* Creation of selVarListaSistemi */
	selVarListaSistemi = XtVaCreateManagedWidget( "selVarListaSistemi",
			xmListWidgetClass,
			scrolledWindow6,
			XmNwidth, 170,
			XmNheight, 115,
			XmNautomaticSelection, TRUE,
			XmNselectionPolicy, XmSINGLE_SELECT,
			NULL );
	XtAddCallback( selVarListaSistemi, XmNsingleSelectionCallback,
		(XtCallbackProc) singleSelectionCB_selVarListaSistemi,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarListaSistemi, (char *) UxSelVarCrContext );


	/* Creation of scrolledWindow7 */
	scrolledWindow7 = XtVaCreateManagedWidget( "scrolledWindow7",
			xmScrolledWindowWidgetClass,
			selByModelForm,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 215,
			XmNy, 30,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 5,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 15,
			XmNleftWidget, scrolledWindow6,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, label5,
			XmNwidth, 300,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNbottomWidget, form44,
			NULL );
	UxPutContext( scrolledWindow7, (char *) UxSelVarCrContext );


	/* Creation of selVarListaBlocchi */
	selVarListaBlocchi = XtVaCreateManagedWidget( "selVarListaBlocchi",
			xmListWidgetClass,
			scrolledWindow7,
			XmNwidth, 270,
			XmNheight, 115,
			XmNautomaticSelection, TRUE,
			XmNselectionPolicy, XmSINGLE_SELECT,
			NULL );
	XtAddCallback( selVarListaBlocchi, XmNsingleSelectionCallback,
		(XtCallbackProc) singleSelectionCB_selVarListaBlocchi,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarListaBlocchi, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksForm */
	selVarCrKksForm = XtVaCreateManagedWidget( "selVarCrKksForm",
			xmFormWidgetClass,
			panedWindow1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 200,
			XmNwidth, 540,
			XmNheight, 160,
			XmNpaneMinimum, 80,
			XmNpaneMaximum, 80,
			NULL );
	UxPutContext( selVarCrKksForm, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksBullBoard */
	selVarCrKksBullBoard = XtVaCreateManagedWidget( "selVarCrKksBullBoard",
			xmBulletinBoardWidgetClass,
			selVarCrKksForm,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 530,
			XmNheight, 100,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( selVarCrKksBullBoard, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksText1 */
	selVarCrKksText1 = XtVaCreateManagedWidget( "selVarCrKksText1",
			xmTextWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 50,
			XmNy, 30,
			XmNwidth, 25,
			XmNheight, 25,
			XmNcolumns, 1,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			XmNmarginWidth, 5,
			NULL );
	UxPutContext( selVarCrKksText1, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksSistemaLabel */
	selVarCrKksSistemaLabel = XtVaCreateManagedWidget( "selVarCrKksSistemaLabel",
			xmLabelWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 30,
			XmNy, 10,
			XmNwidth, 110,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, KKS_SYST_LABEL ),
			NULL );
	UxPutContext( selVarCrKksSistemaLabel, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksSottosistLabel */
	selVarCrKksSottosistLabel = XtVaCreateManagedWidget( "selVarCrKksSottosistLabel",
			xmLabelWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 140,
			XmNy, 10,
			XmNwidth, 70,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, KKS_SSYST_LABEL ),
			NULL );
	UxPutContext( selVarCrKksSottosistLabel, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksComponentLabel */
	selVarCrKksComponentLabel = XtVaCreateManagedWidget( "selVarCrKksComponentLabel",
			xmLabelWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 210,
			XmNy, 10,
			XmNwidth, 70,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, KKS_COMP_LABEL ),
			NULL );
	UxPutContext( selVarCrKksComponentLabel, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksText2 */
	selVarCrKksText2 = XtVaCreateManagedWidget( "selVarCrKksText2",
			xmTextWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 80,
			XmNy, 30,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			XmNmarginWidth, 5,
			NULL );
	UxPutContext( selVarCrKksText2, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksText3 */
	selVarCrKksText3 = XtVaCreateManagedWidget( "selVarCrKksText3",
			xmTextWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 110,
			XmNy, 30,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			XmNmarginWidth, 5,
			NULL );
	UxPutContext( selVarCrKksText3, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksText4 */
	selVarCrKksText4 = XtVaCreateManagedWidget( "selVarCrKksText4",
			xmTextWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 150,
			XmNy, 30,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			XmNmarginWidth, 5,
			NULL );
	UxPutContext( selVarCrKksText4, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksText5 */
	selVarCrKksText5 = XtVaCreateManagedWidget( "selVarCrKksText5",
			xmTextWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 180,
			XmNy, 30,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			XmNmarginWidth, 5,
			NULL );
	UxPutContext( selVarCrKksText5, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksText6 */
	selVarCrKksText6 = XtVaCreateManagedWidget( "selVarCrKksText6",
			xmTextWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 220,
			XmNy, 30,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			XmNmarginWidth, 5,
			NULL );
	UxPutContext( selVarCrKksText6, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksText7 */
	selVarCrKksText7 = XtVaCreateManagedWidget( "selVarCrKksText7",
			xmTextWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 250,
			XmNy, 30,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			XmNmarginWidth, 5,
			NULL );
	UxPutContext( selVarCrKksText7, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksText8 */
	selVarCrKksText8 = XtVaCreateManagedWidget( "selVarCrKksText8",
			xmTextWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 290,
			XmNy, 30,
			XmNwidth, 25,
			XmNheight, 25,
			XmNcolumns, 1,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			XmNmarginWidth, 5,
			NULL );
	UxPutContext( selVarCrKksText8, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksProgressLabel */
	selVarCrKksProgressLabel = XtVaCreateManagedWidget( "selVarCrKksProgressLabel",
			xmLabelWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 270,
			XmNy, 10,
			XmNwidth, 110,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, KKS_PROG_LABEL ),
			NULL );
	UxPutContext( selVarCrKksProgressLabel, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksText9 */
	selVarCrKksText9 = XtVaCreateManagedWidget( "selVarCrKksText9",
			xmTextWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 320,
			XmNy, 30,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			XmNmarginWidth, 5,
			NULL );
	UxPutContext( selVarCrKksText9, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksText10 */
	selVarCrKksText10 = XtVaCreateManagedWidget( "selVarCrKksText10",
			xmTextWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 350,
			XmNy, 30,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			XmNmarginWidth, 5,
			NULL );
	UxPutContext( selVarCrKksText10, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksText0 */
	selVarCrKksText0 = XtVaCreateManagedWidget( "selVarCrKksText0",
			xmTextWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 10,
			XmNy, 30,
			XmNwidth, 25,
			XmNheight, 25,
			XmNcolumns, 1,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			XmNmarginWidth, 5,
			NULL );
	UxPutContext( selVarCrKksText0, (char *) UxSelVarCrContext );


	/* Creation of selVarCrKksNimpLabel */
	selVarCrKksNimpLabel = XtVaCreateManagedWidget( "selVarCrKksNimpLabel",
			xmLabelWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 10,
			XmNy, 10,
			XmNwidth, 30,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, KKS_NIMP_LABEL ),
			NULL );
	UxPutContext( selVarCrKksNimpLabel, (char *) UxSelVarCrContext );


	/* Creation of pushButton1 */
	pushButton1 = XtVaCreateManagedWidget( "pushButton1",
			xmPushButtonWidgetClass,
			selVarCrKksBullBoard,
			XmNx, 410,
			XmNy, 30,
			XmNwidth, 120,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, KKS_APPLY_FILTER ),
			NULL );
	XtAddCallback( pushButton1, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton1,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( pushButton1, (char *) UxSelVarCrContext );


	/* Creation of selVarCrTipoForm */
	selVarCrTipoForm = XtVaCreateManagedWidget( "selVarCrTipoForm",
			xmFormWidgetClass,
			panedWindow1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 10,
			XmNy, 140,
			XmNwidth, 520,
			XmNheight, 30,
			XmNpaneMinimum, 80,
			XmNpaneMaximum, 80,
			NULL );
	UxPutContext( selVarCrTipoForm, (char *) UxSelVarCrContext );


	/* Creation of selVarCrTipoPressionetb */
	selVarCrTipoPressionetb = XtVaCreateManagedWidget( "selVarCrTipoPressionetb",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm,
			XmNx, 10,
			XmNy, 0,
			XmNwidth, 80,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, TIPO_PRESSIONE ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	XtAddCallback( selVarCrTipoPressionetb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoPressionetb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrTipoPressionetb, (char *) UxSelVarCrContext );


	/* Creation of selVarCrTipoEntalpiatb */
	selVarCrTipoEntalpiatb = XtVaCreateManagedWidget( "selVarCrTipoEntalpiatb",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm,
			XmNx, 90,
			XmNy, 40,
			XmNwidth, 80,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_ENTALPIA ),
			NULL );
	XtAddCallback( selVarCrTipoEntalpiatb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoEntalpiatb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrTipoEntalpiatb, (char *) UxSelVarCrContext );


	/* Creation of selVarCrTipoPortatatb */
	selVarCrTipoPortatatb = XtVaCreateManagedWidget( "selVarCrTipoPortatatb",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm,
			XmNx, 10,
			XmNy, 40,
			XmNwidth, 80,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_PORTATA ),
			NULL );
	XtAddCallback( selVarCrTipoPortatatb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoPortatatb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrTipoPortatatb, (char *) UxSelVarCrContext );


	/* Creation of selVarCrTipoGiritb */
	selVarCrTipoGiritb = XtVaCreateManagedWidget( "selVarCrTipoGiritb",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm,
			XmNx, 90,
			XmNy, 20,
			XmNwidth, 80,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_GIRI ),
			NULL );
	XtAddCallback( selVarCrTipoGiritb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoGiritb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrTipoGiritb, (char *) UxSelVarCrContext );


	/* Creation of selVarCrTipoTemperaturatb */
	selVarCrTipoTemperaturatb = XtVaCreateManagedWidget( "selVarCrTipoTemperaturatb",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm,
			XmNx, 10,
			XmNy, 20,
			XmNwidth, 80,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_TEMPERATURA ),
			NULL );
	XtAddCallback( selVarCrTipoTemperaturatb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoTemperaturatb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrTipoTemperaturatb, (char *) UxSelVarCrContext );


	/* Creation of selVarCrTipoAlzvalvtb */
	selVarCrTipoAlzvalvtb = XtVaCreateManagedWidget( "selVarCrTipoAlzvalvtb",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm,
			XmNx, 170,
			XmNy, 0,
			XmNwidth, 90,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_ALZVALV ),
			NULL );
	XtAddCallback( selVarCrTipoAlzvalvtb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoAlzvalvtb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrTipoAlzvalvtb, (char *) UxSelVarCrContext );


	/* Creation of selVarCrTipoPotenzatb */
	selVarCrTipoPotenzatb = XtVaCreateManagedWidget( "selVarCrTipoPotenzatb",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm,
			XmNx, 90,
			XmNy, 0,
			XmNwidth, 80,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_POTENZA ),
			NULL );
	XtAddCallback( selVarCrTipoPotenzatb, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoPotenzatb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrTipoPotenzatb, (char *) UxSelVarCrContext );


	/* Creation of pushButton2 */
	pushButton2 = XtVaCreateManagedWidget( "pushButton2",
			xmPushButtonWidgetClass,
			selVarCrTipoForm,
			XmNx, 410,
			XmNy, 40,
			XmNwidth, 120,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, KKS_APPLY_FILTER ),
			NULL );
	XtAddCallback( pushButton2, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton2,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( pushButton2, (char *) UxSelVarCrContext );


	/* Creation of selVarCrTipoAingable1 */
	selVarCrTipoAingable1 = XtVaCreateManagedWidget( "selVarCrTipoAingable1",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm,
			XmNx, 260,
			XmNy, 0,
			XmNwidth, 120,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_AINGABLE ),
			NULL );
	XtAddCallback( selVarCrTipoAingable1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoAingable1,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrTipoAingable1, (char *) UxSelVarCrContext );


	/* Creation of selVarCrTipoInput1 */
	selVarCrTipoInput1 = XtVaCreateManagedWidget( "selVarCrTipoInput1",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm,
			XmNx, 260,
			XmNy, 20,
			XmNwidth, 120,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_INPUT ),
			NULL );
	XtAddCallback( selVarCrTipoInput1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoInput1,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrTipoInput1, (char *) UxSelVarCrContext );


	/* Creation of selVarCrTipoOutput1 */
	selVarCrTipoOutput1 = XtVaCreateManagedWidget( "selVarCrTipoOutput1",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm,
			XmNx, 260,
			XmNy, 40,
			XmNwidth, 120,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_OUTPUT ),
			NULL );
	XtAddCallback( selVarCrTipoOutput1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoOutput1,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrTipoOutput1, (char *) UxSelVarCrContext );


	/* Creation of form36 */
	form36 = XtVaCreateManagedWidget( "form36",
			xmFormWidgetClass,
			panedWindow1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 13,
			XmNy, 13,
			XmNwidth, 540,
			XmNheight, 150,
			XmNpaneMinimum, 140,
			NULL );
	UxPutContext( form36, (char *) UxSelVarCrContext );


	/* Creation of label6 */
	label6 = XtVaCreateManagedWidget( "label6",
			xmLabelWidgetClass,
			form36,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 534,
			XmNheight, 30,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "" ),
			NULL );
	UxPutContext( label6, (char *) UxSelVarCrContext );


	/* Creation of form38 */
	form38 = XtVaCreateManagedWidget( "form38",
			xmFormWidgetClass,
			form36,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 120,
			XmNwidth, 534,
			XmNheight, 30,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( form38, (char *) UxSelVarCrContext );


	/* Creation of selVarFindUp */
	selVarFindUp = XtVaCreateManagedWidget( "selVarFindUp",
			xmArrowButtonWidgetClass,
			form38,
			XmNx, 10,
			XmNy, 0,
			XmNwidth, 60,
			XmNheight, 30,
			XmNarrowDirection, XmARROW_UP,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomOffset, 3,
			XmNtopOffset, 3,
			NULL );
	XtAddCallback( selVarFindUp, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarFindUp,
		(XtPointer) 0x0 );

	UxPutContext( selVarFindUp, (char *) UxSelVarCrContext );


	/* Creation of selVarText */
	selVarText = XtVaCreateManagedWidget( "selVarText",
			xmTextFieldWidgetClass,
			form38,
			XmNx, 20,
			XmNy, 10,
			XmNwidth, 140,
			XmNheight, 20,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, selVarFindUp,
			XmNbottomOffset, 0,
			XmNtopOffset, 0,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarText, (char *) UxSelVarCrContext );


	/* Creation of selVarFindDown */
	selVarFindDown = XtVaCreateManagedWidget( "selVarFindDown",
			xmArrowButtonWidgetClass,
			form38,
			XmNx, 250,
			XmNy, 0,
			XmNwidth, 60,
			XmNheight, 30,
			XmNarrowDirection, XmARROW_DOWN,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, selVarText,
			XmNbottomOffset, 3,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 3,
			NULL );
	XtAddCallback( selVarFindDown, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarFindDown,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarFindDown, (char *) UxSelVarCrContext );


	/* Creation of aggiungiVarSelpb */
	aggiungiVarSelpb = XtVaCreateManagedWidget( "aggiungiVarSelpb",
			xmPushButtonWidgetClass,
			form38,
			XmNx, 410,
			XmNy, 0,
			XmNwidth, 100,
			XmNheight, 30,
			XmNleftAttachment, XmATTACH_NONE,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNtopOffset, 3,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomOffset, 3,
			RES_CONVERT( XmNlabelString, SELECT_VAR ),
			NULL );
	XtAddCallback( aggiungiVarSelpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_aggiungiVarSelpb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( aggiungiVarSelpb, (char *) UxSelVarCrContext );


	/* Creation of selezionaAllpb */
	selezionaAllpb = XtVaCreateManagedWidget( "selezionaAllpb",
			xmPushButtonWidgetClass,
			form38,
			XmNx, 280,
			XmNy, 0,
			XmNwidth, 100,
			XmNheight, 30,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, selVarFindDown,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 3,
			XmNbottomOffset, 3,
			RES_CONVERT( XmNlabelString, SELECT_ALL ),
			NULL );
	XtAddCallback( selezionaAllpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_selezionaAllpb,
		UxGetWidget(selVarText) );

	UxPutContext( selezionaAllpb, (char *) UxSelVarCrContext );


	/* Creation of scrolledWindow8 */
	scrolledWindow8 = XtVaCreateManagedWidget( "scrolledWindow8",
			xmScrolledWindowWidgetClass,
			form36,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 20,
			XmNy, 30,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 5,
			XmNbottomWidget, form38,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 5,
			XmNtopWidget, label6,
			NULL );
	UxPutContext( scrolledWindow8, (char *) UxSelVarCrContext );


	/* Creation of selVarToSelect */
	selVarToSelect = XtVaCreateManagedWidget( "selVarToSelect",
			xmListWidgetClass,
			scrolledWindow8,
			XmNwidth, 490,
			XmNheight, 80,
			XmNselectionPolicy, XmEXTENDED_SELECT,
			NULL );
	UxPutContext( selVarToSelect, (char *) UxSelVarCrContext );


	/* Creation of form37 */
	form37 = XtVaCreateManagedWidget( "form37",
			xmFormWidgetClass,
			panedWindow1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 13,
			XmNy, 171,
			XmNwidth, 540,
			XmNheight, 150,
			XmNpaneMinimum, 140,
			NULL );
	UxPutContext( form37, (char *) UxSelVarCrContext );


	/* Creation of form35 */
	form35 = XtVaCreateManagedWidget( "form35",
			xmFormWidgetClass,
			form37,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 120,
			XmNwidth, 534,
			XmNheight, 30,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( form35, (char *) UxSelVarCrContext );


	/* Creation of deleteVarSelpb */
	deleteVarSelpb = XtVaCreateManagedWidget( "deleteVarSelpb",
			xmPushButtonWidgetClass,
			form35,
			XmNx, 410,
			XmNy, 0,
			XmNwidth, 100,
			XmNheight, 30,
			XmNleftAttachment, XmATTACH_NONE,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNtopOffset, 3,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomOffset, 3,
			RES_CONVERT( XmNlabelString, UNSELECT_VAR ),
			NULL );
	XtAddCallback( deleteVarSelpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_deleteVarSelpb,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( deleteVarSelpb, (char *) UxSelVarCrContext );


	/* Creation of unselVarFindUp */
	unselVarFindUp = XtVaCreateManagedWidget( "unselVarFindUp",
			xmArrowButtonWidgetClass,
			form35,
			XmNx, 10,
			XmNy, 0,
			XmNwidth, 60,
			XmNheight, 30,
			XmNarrowDirection, XmARROW_UP,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 3,
			XmNbottomOffset, 3,
			NULL );
	XtAddCallback( unselVarFindUp, XmNactivateCallback,
		(XtCallbackProc) activateCB_unselVarFindUp,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( unselVarFindUp, (char *) UxSelVarCrContext );


	/* Creation of unselVarText */
	unselVarText = XtVaCreateManagedWidget( "unselVarText",
			xmTextFieldWidgetClass,
			form35,
			XmNx, 20,
			XmNy, 10,
			XmNwidth, 140,
			XmNheight, 20,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, unselVarFindUp,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( unselVarText, (char *) UxSelVarCrContext );


	/* Creation of unselVarFindDown */
	unselVarFindDown = XtVaCreateManagedWidget( "unselVarFindDown",
			xmArrowButtonWidgetClass,
			form35,
			XmNx, 250,
			XmNy, 0,
			XmNwidth, 60,
			XmNheight, 30,
			XmNarrowDirection, XmARROW_DOWN,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, unselVarText,
			XmNtopOffset, 3,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 3,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	XtAddCallback( unselVarFindDown, XmNactivateCallback,
		(XtCallbackProc) activateCB_unselVarFindDown,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( unselVarFindDown, (char *) UxSelVarCrContext );


	/* Creation of deselezionaAllpb */
	deselezionaAllpb = XtVaCreateManagedWidget( "deselezionaAllpb",
			xmPushButtonWidgetClass,
			form35,
			XmNx, 280,
			XmNy, 0,
			XmNwidth, 100,
			XmNheight, 30,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, unselVarFindDown,
			XmNbottomOffset, 3,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 3,
			RES_CONVERT( XmNlabelString, SELECT_ALL ),
			NULL );
	XtAddCallback( deselezionaAllpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_deselezionaAllpb,
		UxGetWidget(unselVarText) );

	UxPutContext( deselezionaAllpb, (char *) UxSelVarCrContext );


	/* Creation of label4 */
	label4 = XtVaCreateManagedWidget( "label4",
			xmLabelWidgetClass,
			form37,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 534,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "" ),
			NULL );
	UxPutContext( label4, (char *) UxSelVarCrContext );


	/* Creation of scrolledWindow9 */
	scrolledWindow9 = XtVaCreateManagedWidget( "scrolledWindow9",
			xmScrolledWindowWidgetClass,
			form37,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, -2,
			XmNy, 8,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 5,
			XmNbottomWidget, form35,
			XmNleftOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, label4,
			NULL );
	UxPutContext( scrolledWindow9, (char *) UxSelVarCrContext );


	/* Creation of selVarToUnselect */
	selVarToUnselect = XtVaCreateManagedWidget( "selVarToUnselect",
			xmListWidgetClass,
			scrolledWindow9,
			XmNwidth, 490,
			XmNheight, 60,
			XmNselectionPolicy, XmEXTENDED_SELECT,
			NULL );
	UxPutContext( selVarToUnselect, (char *) UxSelVarCrContext );


	/* Creation of selVarCrPopup */
	selVarCrPopup_shell = XtVaCreatePopupShell ("selVarCrPopup_shell",
			xmMenuShellWidgetClass, panedWindow1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	selVarCrPopup = XtVaCreateWidget( "selVarCrPopup",
			xmRowColumnWidgetClass,
			selVarCrPopup_shell,
			XmNrowColumnType, XmMENU_POPUP,
			NULL );
	UxPutContext( selVarCrPopup, (char *) UxSelVarCrContext );


	/* Creation of selVarCrPopupSaveFilter */
	selVarCrPopupSaveFilter = XtVaCreateManagedWidget( "selVarCrPopupSaveFilter",
			xmPushButtonWidgetClass,
			selVarCrPopup,
			RES_CONVERT( XmNlabelString, SAVEFILTER ),
			NULL );
	XtAddCallback( selVarCrPopupSaveFilter, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarCrPopupSaveFilter,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrPopupSaveFilter, (char *) UxSelVarCrContext );


	/* Creation of selVarCrPopupApply */
	selVarCrPopupApply = XtVaCreateManagedWidget( "selVarCrPopupApply",
			xmPushButtonWidgetClass,
			selVarCrPopup,
			RES_CONVERT( XmNlabelString, APPLYFILTER ),
			NULL );
	XtAddCallback( selVarCrPopupApply, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarCrPopupApply,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrPopupApply, (char *) UxSelVarCrContext );


	/* Creation of menu9_p1_b3 */
	menu9_p1_b3 = XtVaCreateManagedWidget( "menu9_p1_b3",
			xmSeparatorWidgetClass,
			selVarCrPopup,
			NULL );
	UxPutContext( menu9_p1_b3, (char *) UxSelVarCrContext );


	/* Creation of selVarCrPopupSave */
	selVarCrPopupSave = XtVaCreateManagedWidget( "selVarCrPopupSave",
			xmPushButtonWidgetClass,
			selVarCrPopup,
			RES_CONVERT( XmNlabelString, CONF_ACCEPT ),
			NULL );
	XtAddCallback( selVarCrPopupSave, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarCrPopupSave,
		selVarCr );

	UxPutContext( selVarCrPopupSave, (char *) UxSelVarCrContext );


	/* Creation of menu9_p1_b5 */
	menu9_p1_b5 = XtVaCreateManagedWidget( "menu9_p1_b5",
			xmSeparatorWidgetClass,
			selVarCrPopup,
			NULL );
	UxPutContext( menu9_p1_b5, (char *) UxSelVarCrContext );


	/* Creation of selVarCrPopupQuit */
	selVarCrPopupQuit = XtVaCreateManagedWidget( "selVarCrPopupQuit",
			xmPushButtonWidgetClass,
			selVarCrPopup,
			RES_CONVERT( XmNlabelString, CONF_DISCARD ),
			NULL );
	XtAddCallback( selVarCrPopupQuit, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarCrPopupQuit,
		(XtPointer) UxSelVarCrContext );

	UxPutContext( selVarCrPopupQuit, (char *) UxSelVarCrContext );

	XtVaSetValues(form39,
			XmNdefaultButton, selVarOkpb,
			NULL );


	XtAddCallback( selVarCr, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxSelVarCrContext);

	XtAddEventHandler(panedWindow1, ButtonPressMask,
			False, (XtEventHandler) _UxselVarCrMenuPost, (XtPointer) selVarCrPopup );

	return ( selVarCr );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_selVarCr( Widget _Uxpadre )
{
	Widget                  rtrn;
	_UxCselVarCr            *UxContext;

	UxSelVarCrContext = UxContext =
		(_UxCselVarCr *) UxNewContext( sizeof(_UxCselVarCr), False );

	padre = _Uxpadre;

	{
#ifndef DESIGN_TIME
		char *s;
		set_cursor (padre,CLOCK);
#endif
		rtrn = _Uxbuild_selVarCr();

#ifndef DESIGN_TIME
		get_kks_filter (selVarCrKksBullBoard);
		presetFilterDefaults (rtrn);
		/*
		applicaFiltri (rtrn, FILTRO_CR2);
		*/
		set_var_cr_defaults (rtrn); 
		UxPopupInterface (rtrn, no_grab);
		display_lista_sistema (selVarListaSistemi);
		display_lista_blocchi (selVarListaBlocchi);
		display_lista_var (selVarToSelect,
		                   modello_selezionato,blocco_selezionato,
		                   kks_filter,tipo_filter);
		s = (char *)malloc(strlen(SEL_VAR_TITLE)+strlen(FILE_REC_DEFAULT)+10);
		sprintf (s,"%s: %s",SEL_VAR_TITLE,FILE_REC_DEFAULT);
		update_title (UxGetWidget(rtrn),s);
		free(s);
		sel_var_cr_interface_active = 1;
		set_cursor (padre,NORMALE);
#endif
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

