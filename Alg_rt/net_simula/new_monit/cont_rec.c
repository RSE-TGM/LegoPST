/**********************************************************************
*
*       C Source:               cont_rec.c
*       Subsystem:              1
*       Description:
*       %created_by:    ciccotel %
*       %date_created:  Tue Oct 22 13:31:56 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: cont_rec.c-7.1.1 %  (%full_filespec: cont_rec.c-7.1.1:csrc:1 %)";
#endif

/*
 * cont_rec.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include <X11/Xlib.h>
#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/Xm.h>

#include "UxXt.h"

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

#include "sked.h"

#include "parametri.h"
#include "messaggi.h"
#include "bistrutt.h"
#include "cursore.h"
#include "option.h"

#include "filtri.h"
#include "banco_globals.h"
#include "selVarCr.h"
#include "selVar.h"
#include "cont_rec.h"
#include "file_selection.h"
#include "refresh.h"
#include "tabelle_malf.h"

extern int modello_selezionato;
extern int blocco_selezionato;

extern int kks_filter;          /* flag filtro attivo        */
extern char filtro_kks[12];     /* stringa filtro            */
extern int tipo_filter;         /* flag filtro attivo        */
extern FILTRO_TIPI filtro_tipi; /* struttura filtro per tipo */

extern VARIABILI *variabili;
extern IND_VARIABILI *ind_variabili;
extern char *ind_sh_top;
extern STATISTICHE statistiche;
extern Widget areaMessaggi;
extern Widget selVarCr;
extern int tot_variabili;
extern char *create_fileSelection();
extern OPTIONS_FLAGS options;
extern   VAL val;


char   *nome_modello (char *, int);
char   *nome_blocco(char *,int,int);
legge_sel(char *);


int *punt_var_sel;   /* vettore dei puntatori delle variabili selezionate */
int *punt_var_unsel; /* vettore dei puntatori delle variabili non selezionate */
int *punt_sel_kks; /* vettore dei punt. var. sel. kks       */
int *punt_unsel_kks; /* vettore dei punt. var. unsel. kks       */
int *punt_sel_tipo; /* vettore dei punt. var. sel. tipo       */
int *punt_unsel_tipo; /* vettore dei punt. var. unsel. tipo       */

int *punt_sel_lista; /* puntatore delle var. selez. in lista        */
int *punt_unsel_lista; /* puntatore delle var. non selez. in lista        */

int sel_modificata; /* flag di modifica sel. per funzioni find      */
int unsel_modificata; /* flag di modifica sel. per funzioni find      */

int mod_modificata; /* flag di modifica sel. per funzioni find      */

int num_blocchi;

int n_tounsel;      /* numero var. selezionate (filtro mod/blocco) */
int n_tosel;        /* numero var. selezionabili (filtro mod/blocco) */
int n_tounsel_kks;  /* n. var. sel filtro mod/bloc && kks            */
int n_tosel_kks;    /* n. var. selezionabili filtro mod/bloc && kks  */
int n_tounsel_tipo;  /* n. var. sel filtro mod/bloc && kks && tipo   */
int n_tosel_tipo;    /* n. var. selezionabili filtro mod/bloc && kks && tipo */

int n_tounsel_display; /* numero var. selezionate in display       */
int n_tosel_display;   /* numero var. non selezionate in display   */
/**********************************************************/
int manage_widget_filter (padre, target, stato, filter,interfaccia)
Widget padre;   /* interfaccia chiamante      */
Widget target;  /* widget da gestire          */
Boolean stato;  /* stato richiesta 1=on 0=off */
int filter;     /* filtro richiesto           */
int interfaccia;/* chiamante                  */
{
Widget ListaSys;
Widget ListaBlock;
Widget ListaBlocchi;
Widget Kks_button;
Widget Per_tipo;
Widget Only_model;
Widget By_modblock;
Widget Label_blocks;
Widget Find_form;
Widget All_var;
Boolean new_state;
_UxCselVarCr *Ctx;
_UxCselVar   *Ctxvar;

	switch (interfaccia)
		{
		case FILTRO_CR: 
			Ctx = (_UxCselVarCr*)UxGetContext(padre);

   		ListaSys       = Ctx->UxselVarListaSistemi;
   		ListaBlocchi   = Ctx->UxselVarListaBlocchi;
   		ListaBlock     = Ctx->UxscrolledWindow7;
   		Kks_button     = Ctx->UxselVarMenuKkstb;
   		By_modblock    = Ctx->UxselByModeltb;
   		Only_model     = Ctx->UxselByModelOnlytb;
   		Label_blocks   = Ctx->Uxlabel5;
   		Find_form      = Ctx->Uxform44;
   		All_var        = Ctx->UxselByAllVariablestb;

			break;

		case FILTRO_CR2:
         Ctx = (_UxCselVarCr*)UxGetContext(padre);

         ListaSys       = Ctx->UxselVarListaSistemi;
         ListaBlocchi   = Ctx->UxselVarListaBlocchi;
         ListaBlock     = Ctx->UxscrolledWindow7;
         Kks_button     = Ctx->UxselVarCrKkstb;
         By_modblock    = Ctx->UxselVarCrModBlocktb;
         Only_model     = Ctx->UxselVarCrModtb;
         Label_blocks   = Ctx->Uxlabel5;
         Find_form      = Ctx->Uxform44;
         All_var        = Ctx->UxselVarCrAllVartb;

         break;
               
		case FILTRO_VAR:
         Ctxvar = (_UxCselVar*)UxGetContext(padre);

         ListaSys       = Ctxvar->UxselVarListaSistemi1;
         ListaBlocchi   = Ctxvar->UxselVarListaBlocchi1;
         ListaBlock     = Ctxvar->UxscrolledWindow11;
         Kks_button     = Ctxvar->UxselVarMenuKkstb1;
         By_modblock    = Ctxvar->UxselByModeltb1;
         Only_model     = Ctxvar->UxselByModelOnlytb1;
         Label_blocks   = Ctxvar->Uxlabel8;
         Find_form      = Ctxvar->Uxform43;
         All_var        = Ctxvar->UxselByAllVariablestb1;

			break;

      case FILTRO_VAR2:
      case FILTRO_MALF:
      case FILTRO_FREM:
         Ctxvar = (_UxCselVar*)UxGetContext(padre);

         ListaSys       = Ctxvar->UxselVarListaSistemi1;
         ListaBlocchi   = Ctxvar->UxselVarListaBlocchi1;
         ListaBlock     = Ctxvar->UxscrolledWindow11;
         Kks_button     = Ctxvar->UxselVarCrKkstb1;
         By_modblock    = Ctxvar->UxselVarCrModBlocktb1;
         Only_model     = Ctxvar->UxselVarCrModtb1;
         Label_blocks   = Ctxvar->Uxlabel8;
         Find_form      = Ctxvar->Uxform43;
         All_var        = Ctxvar->UxselVarCrAllVartb1;

         break;

		default:
			return (-1);
		}
	
switch (filter)
	{
	case FILTER_ALL_VAR:
		switch (stato)
      {
      case 1:
			switch (interfaccia)
            {
            case FILTRO_CR:
         		modello_selezionato = NO_MOD;
         		blocco_selezionato = NO_BLOCK;
         		XtUnmanageChild (target);
         		new_state = 0;
         		XmToggleButtonSetState (By_modblock,new_state,False);
         		XmToggleButtonSetState (Only_model,new_state,False);

					XtVaSetValues (All_var, XmNsensitive, False, NULL);
					XtVaSetValues (By_modblock, XmNsensitive, True, NULL);
         		XtVaSetValues (Only_model, XmNsensitive, True, NULL);
					break;
				case FILTRO_CR2:
					modello_selezionato = NO_MOD;
               blocco_selezionato = NO_BLOCK;
         		new_state = 0;
         		XmToggleButtonSetState (By_modblock,new_state,False);
         		XmToggleButtonSetState (Only_model,new_state,False);
					break;
				case FILTRO_VAR:
					Ctxvar->Uxmod_sel = NO_MOD;
					Ctxvar->Uxblocco_sel = NO_BLOCK;
					Ctxvar->Uxsel_mod = 1;
					Ctxvar->Uxsel_mod_b = 1;
         		XtUnmanageChild (target);
         		new_state = 0;
         		XmToggleButtonSetState (By_modblock,new_state,False);
         		XmToggleButtonSetState (Only_model,new_state,False);

					XtVaSetValues (All_var, XmNsensitive, False, NULL);
					XtVaSetValues (By_modblock, XmNsensitive, True, NULL);
         		XtVaSetValues (Only_model, XmNsensitive, True, NULL);
               break;
				case FILTRO_VAR2:
				case FILTRO_MALF:
				case FILTRO_FREM:
               Ctxvar->Uxmod_sel = NO_MOD;
               Ctxvar->Uxblocco_sel = NO_BLOCK;
               Ctxvar->Uxsel_mod = 1;
               Ctxvar->Uxsel_mod_b = 1;
               new_state = 0;
               XmToggleButtonSetState (By_modblock,new_state,False);
               XmToggleButtonSetState (Only_model,new_state,False);
               break;
				}
         break;
      }
		break;
	case FILTER_MOD_BLOC:
		switch (stato)
      {
      case 1:
			switch (interfaccia)
            {
            case FILTRO_CR:
					if (modello_selezionato == NO_MOD) modello_selezionato = 1;
               if (blocco_selezionato == NO_BLOCK) blocco_selezionato = 1;
					XtManageChild (target);
         		XtManageChild (Label_blocks);
         		XtManageChild (Find_form);
         		XtManageChild (ListaBlock);
         		display_lista_sistema (ListaSys);
					display_lista_blocchi (ListaBlocchi);
					new_state = 0;
					XmToggleButtonSetState (Only_model,new_state,False);
					XmToggleButtonSetState (All_var,new_state,False);

					XtVaSetValues (By_modblock, XmNsensitive, False, NULL);
					XtVaSetValues (All_var, XmNsensitive, True, NULL);
					XtVaSetValues (Only_model, XmNsensitive, True, NULL);
					break;
				case FILTRO_CR2:
               if (modello_selezionato == NO_MOD) modello_selezionato = 1;
               if (blocco_selezionato == NO_BLOCK) blocco_selezionato = 1;
					new_state = 0;
					XmToggleButtonSetState (Only_model,new_state,False);
					XmToggleButtonSetState (All_var,new_state,False);
               break;
				case FILTRO_VAR:
         		if (Ctxvar->Uxmod_sel == NO_MOD) Ctxvar->Uxmod_sel = 1;
         		if (Ctxvar->Uxblocco_sel == NO_BLOCK) Ctxvar->Uxblocco_sel = 1;
					Ctxvar->Uxsel_mod = 1;
					Ctxvar->Uxsel_mod_b = 1;
					XtManageChild (target);
         		XtManageChild (Label_blocks);
         		XtManageChild (Find_form);
         		XtManageChild (ListaBlock);
         		display_lista_sistema (ListaSys);
					display_blocchi (ListaBlocchi);
					new_state = 0;
					XmToggleButtonSetState (Only_model,new_state,False);
					XmToggleButtonSetState (All_var,new_state,False);

					XtVaSetValues (By_modblock, XmNsensitive, False, NULL);
					XtVaSetValues (All_var, XmNsensitive, True, NULL);
					XtVaSetValues (Only_model, XmNsensitive, True, NULL);
               break;
				case FILTRO_VAR2:
				case FILTRO_MALF:
				case FILTRO_FREM:
               if (Ctxvar->Uxmod_sel == NO_MOD) Ctxvar->Uxmod_sel = 1;
               if (Ctxvar->Uxblocco_sel == NO_BLOCK) Ctxvar->Uxblocco_sel = 1;
               Ctxvar->Uxsel_mod = 1;
               Ctxvar->Uxsel_mod_b = 1;
               new_state = 0;
               XmToggleButtonSetState (Only_model,new_state,False);
               XmToggleButtonSetState (All_var,new_state,False);
               break;
				}
         break;
      }
		break;
	case FILTER_MOD_ONLY:
		switch (stato)
      {
      case 1:
         switch (interfaccia)
            {
            case FILTRO_CR:
               if (modello_selezionato == NO_MOD) modello_selezionato = 1;
         		blocco_selezionato = NO_BLOCK;
         		XtManageChild (target);
         		XtUnmanageChild (ListaBlock);
         		XtUnmanageChild (Label_blocks);
         		XtUnmanageChild (Find_form);
         		display_lista_sistema (ListaSys);
         		new_state = 0;
         		XmToggleButtonSetState (By_modblock,new_state,False);
         		XmToggleButtonSetState (All_var,new_state,False);

         		XtVaSetValues (Only_model, XmNsensitive, False, NULL);
         		XtVaSetValues (All_var, XmNsensitive, True, NULL);
         		XtVaSetValues (By_modblock, XmNsensitive, True, NULL);
               break;
            case FILTRO_CR2:
               if (modello_selezionato == NO_MOD) modello_selezionato = 1;
         		blocco_selezionato = NO_BLOCK;
         		new_state = 0;
         		XmToggleButtonSetState (By_modblock,new_state,False);
         		XmToggleButtonSetState (All_var,new_state,False);
               break;
            case FILTRO_VAR:
               if (Ctxvar->Uxmod_sel == NO_MOD) Ctxvar->Uxmod_sel = 1;
               Ctxvar->Uxblocco_sel = NO_BLOCK;
					Ctxvar->Uxsel_mod = 1;
					Ctxvar->Uxsel_mod_b = 1;
         		XtManageChild (target);
         		XtUnmanageChild (ListaBlock);
         		XtUnmanageChild (Label_blocks);
         		XtUnmanageChild (Find_form);
         		display_lista_sistema (ListaSys);
         		new_state = 0;
         		XmToggleButtonSetState (By_modblock,new_state,False);
         		XmToggleButtonSetState (All_var,new_state,False);

         		XtVaSetValues (Only_model, XmNsensitive, False, NULL);
         		XtVaSetValues (All_var, XmNsensitive, True, NULL);
         		XtVaSetValues (By_modblock, XmNsensitive, True, NULL);
               break;
            case FILTRO_VAR2:
            case FILTRO_MALF:
            case FILTRO_FREM:
               if (Ctxvar->Uxmod_sel == NO_MOD) Ctxvar->Uxmod_sel = 1;
               Ctxvar->Uxblocco_sel = NO_BLOCK;
               Ctxvar->Uxsel_mod = 1;
               Ctxvar->Uxsel_mod_b = 1;
               new_state = 0;
               XmToggleButtonSetState (By_modblock,new_state,False);
               XmToggleButtonSetState (All_var,new_state,False);
               break;
				}
			break;
      }
		break;

	case FILTER_KKS:
		switch (stato)
      {
      case 1:
			switch (interfaccia)
				{
				case FILTRO_CR:
         		kks_filter = 1;
         		get_kks_filter(padre);
         		XtManageChild (target);
					break;
				case FILTRO_CR2:
         		kks_filter = 1;
         		get_kks_filter(padre);
					break;
				case FILTRO_VAR:
					Ctxvar->Uxkks_filter_act = 1;
					lettura_filtro_kks (padre);
					Ctxvar->Uxsel_mod = 1;
					Ctxvar->Uxsel_mod_b = 1;
         		XtManageChild (target);
					break;
            case FILTRO_VAR2:
            case FILTRO_MALF:
            case FILTRO_FREM:
               Ctxvar->Uxkks_filter_act = 1;
               lettura_filtro_kks (padre);
               Ctxvar->Uxsel_mod = 1;
               Ctxvar->Uxsel_mod_b = 1;
               break;
				}
         break;
      case 0:
			switch (interfaccia)
            {
            case FILTRO_CR:
         		kks_filter = 0;
         		XtUnmanageChild (target);
         		break;
            case FILTRO_CR2:
         		kks_filter = 0;
         		break;
				case FILTRO_VAR:
					Ctxvar->Uxkks_filter_act = 0;
					Ctxvar->Uxsel_mod = 1;
					Ctxvar->Uxsel_mod_b = 1;
         		XtUnmanageChild (target);
               break;
            case FILTRO_VAR2:
            case FILTRO_MALF:
            case FILTRO_FREM:
               Ctxvar->Uxkks_filter_act = 0;
               Ctxvar->Uxsel_mod = 1;
               Ctxvar->Uxsel_mod_b = 1;
               break;
				}
			break;
      }
		break;

	case FILTER_TYPE:
		switch (stato)
      {
      case 1:
			switch (interfaccia)
            {
            case FILTRO_CR:
         		tipo_filter = 1;
         		carica_filtro_tipo();
         		XtManageChild (target);
         		break;
            case FILTRO_CR2:
         		tipo_filter = 1;
         		carica_filtro_tipo();
         		break;
				case FILTRO_VAR:
					Ctxvar->Uxtipo_filter_act = 1;
					lettura_filtro_tipo (padre);
					Ctxvar->Uxsel_mod = 1;
					Ctxvar->Uxsel_mod_b = 1;
         		XtManageChild (target);
               break;
            case FILTRO_VAR2:
            case FILTRO_MALF:
            case FILTRO_FREM:
               Ctxvar->Uxtipo_filter_act = 1;
               lettura_filtro_tipo (padre);
               Ctxvar->Uxsel_mod = 1;
               Ctxvar->Uxsel_mod_b = 1;
               break;
				}
			break;
      case 0:
			switch (interfaccia)
            {
            case FILTRO_CR:
         		tipo_filter = 0;
         		XtUnmanageChild (target);
         		break;
            case FILTRO_CR2:
         		tipo_filter = 0;
         		break;
				case FILTRO_VAR:
					Ctxvar->Uxtipo_filter_act = 0;
					Ctxvar->Uxsel_mod = 1;
					Ctxvar->Uxsel_mod_b = 1;
         		XtUnmanageChild (target);
               break;
            case FILTRO_VAR2:
            case FILTRO_MALF:
            case FILTRO_FREM:
               Ctxvar->Uxtipo_filter_act = 0;
               Ctxvar->Uxsel_mod = 1;
               Ctxvar->Uxsel_mod_b = 1;
               break;
				}
			break;
      }
		break;
	}

	return (0);
}
/**********************************************************/
int applicaFiltri (padre, interfaccia)
Widget padre;
int interfaccia;
{
Widget ListaSys;
Widget ListaBlock;
Widget ListaBlocchi;
Widget Kks_button;
Widget Per_tipo;
Widget Only_model;
Widget By_modblock;
Widget Label_blocks;
Widget Find_form;
Widget All_var;
Boolean new_state;
Widget FormTipo;
Widget FormKks;
Widget FormModBlock;
_UxCselVarCr *Ctx;
_UxCselVar   *Ctxvar;

Boolean FiltroKks;
Boolean FiltroTipo;
Boolean FiltroAllVar;
Boolean FiltroModBlock;
Boolean FiltroMod;

int FlagKks;
int FlagTipo;
int nm;
int nb;

char *mess;

   switch (interfaccia)
      {
      case FILTRO_CR:
         Ctx = (_UxCselVarCr*)UxGetContext(padre);

         ListaSys       = Ctx->UxselVarListaSistemi;
         ListaBlocchi   = Ctx->UxselVarListaBlocchi;
         ListaBlock     = Ctx->UxscrolledWindow7;
         Kks_button     = Ctx->UxselVarMenuKkstb;
         By_modblock    = Ctx->UxselByModeltb;
         Only_model     = Ctx->UxselByModelOnlytb;
         Label_blocks   = Ctx->Uxlabel5;
         Find_form      = Ctx->Uxform44;
         All_var        = Ctx->UxselByAllVariablestb;

         break;

      case FILTRO_CR2:
         Ctx = (_UxCselVarCr*)UxGetContext(padre);

         ListaSys       = Ctx->UxselVarListaSistemi;
         ListaBlocchi   = Ctx->UxselVarListaBlocchi;
         ListaBlock     = Ctx->UxscrolledWindow7;
         Kks_button     = Ctx->UxselVarCrKkstb;
         By_modblock    = Ctx->UxselVarCrModBlocktb;
         Only_model     = Ctx->UxselVarCrModtb;
         Label_blocks   = Ctx->Uxlabel5;
         Find_form      = Ctx->Uxform44;
         All_var        = Ctx->UxselVarCrAllVartb;
			Per_tipo 		= Ctx->UxselVarCrTypetb;
			FormTipo			= Ctx->UxselVarCrTipoForm;
			FormKks			= Ctx->UxselVarCrKksForm;
			FormModBlock	= Ctx->UxselByModelForm;

         break;

      case FILTRO_VAR:
         Ctxvar = (_UxCselVar*)UxGetContext(padre);

         ListaSys       = Ctxvar->UxselVarListaSistemi1;
         ListaBlocchi   = Ctxvar->UxselVarListaBlocchi1;
         ListaBlock     = Ctxvar->UxscrolledWindow11;
         Kks_button     = Ctxvar->UxselVarMenuKkstb1;
         By_modblock    = Ctxvar->UxselByModeltb1;
         Only_model     = Ctxvar->UxselByModelOnlytb1;
         Label_blocks   = Ctxvar->Uxlabel8;
         Find_form      = Ctxvar->Uxform43;
         All_var        = Ctxvar->UxselByAllVariablestb1;
			Per_tipo 		= Ctxvar->UxselVarMenuTipotb1;
			FlagKks        = Ctxvar->Uxkks_filter_act;
         FlagTipo       = Ctxvar->Uxtipo_filter_act;
			nb					= Ctxvar->Uxblocco_sel;
			nm 				= Ctxvar->Uxmod_sel;
			break;

      case FILTRO_VAR2:
      case FILTRO_MALF:
      case FILTRO_FREM:
         Ctxvar = (_UxCselVar*)UxGetContext(padre);

         ListaSys       = Ctxvar->UxselVarListaSistemi1;
         ListaBlocchi   = Ctxvar->UxselVarListaBlocchi1;
         ListaBlock     = Ctxvar->UxscrolledWindow11;
         Kks_button     = Ctxvar->UxselVarCrKkstb1;
         By_modblock    = Ctxvar->UxselVarCrModBlocktb1;
         Only_model     = Ctxvar->UxselVarCrModtb1;
         Label_blocks   = Ctxvar->Uxlabel8;
         Find_form      = Ctxvar->Uxform43;
         All_var        = Ctxvar->UxselVarCrAllVartb1;
         Per_tipo       = Ctxvar->UxselVarCrTypetb1;
			FormTipo       = Ctxvar->UxselVarCrTipoForm1;
         FormKks        = Ctxvar->UxselVarCrKksForm2;
         FormModBlock   = Ctxvar->UxselByModelForm1;			
			FlagKks        = Ctxvar->Uxkks_filter_act;
         FlagTipo       = Ctxvar->Uxtipo_filter_act;
			nb					= Ctxvar->Uxblocco_sel;
			nm 				= Ctxvar->Uxmod_sel;

         break;

      default:
         return (-1);
      }
/*
 * lettura dei TB per identificare i filtri attivi
 */
	FiltroKks = XmToggleButtonGetState (Kks_button);
	FiltroTipo = XmToggleButtonGetState (Per_tipo);
	FiltroAllVar = XmToggleButtonGetState (All_var);
	FiltroModBlock = XmToggleButtonGetState (By_modblock);
	FiltroMod = XmToggleButtonGetState (Only_model);
/*
 * scrittura nelle opzioni
 */
   if ((interfaccia == FILTRO_CR) || (interfaccia == FILTRO_CR2))
      {
		options.optionsFilterCr.ByAllVar = FiltroAllVar;
		options.optionsFilterCr.ByModBlock = FiltroModBlock;
		options.optionsFilterCr.ByMod = FiltroMod;
		options.optionsFilterCr.ByKks = FiltroKks;
		options.optionsFilterCr.ByType = FiltroTipo;
		}
   if ((interfaccia == FILTRO_VAR) || (interfaccia == FILTRO_VAR2))
      {
      options.optionsFilterAing.ByAllVar = FiltroAllVar;
      options.optionsFilterAing.ByModBlock = FiltroModBlock;
      options.optionsFilterAing.ByMod = FiltroMod;
      options.optionsFilterAing.ByKks = FiltroKks;
      options.optionsFilterAing.ByType = FiltroTipo;
      }
	if (interfaccia == FILTRO_MALF)
      {
      options.optionsFilterMalf.ByAllVar = FiltroAllVar;
      options.optionsFilterMalf.ByModBlock = FiltroModBlock;
      options.optionsFilterMalf.ByMod = FiltroMod;
      options.optionsFilterMalf.ByKks = FiltroKks;
      options.optionsFilterMalf.ByType = FiltroTipo;
      }
	if (interfaccia == FILTRO_FREM)
      {
      options.optionsFilterFrem.ByAllVar = FiltroAllVar;
      options.optionsFilterFrem.ByModBlock = FiltroModBlock;
      options.optionsFilterFrem.ByMod = FiltroMod;
      options.optionsFilterFrem.ByKks = FiltroKks;
      options.optionsFilterFrem.ByType = FiltroTipo;
      }
/*
 * applicazione dei filtri attivi
 */
	if (FiltroKks)
		{
		if ((interfaccia==FILTRO_CR) || (interfaccia == FILTRO_CR2))
			kks_filter = 1;
		if ((interfaccia==FILTRO_VAR) || (interfaccia == FILTRO_VAR2) || 
			 (interfaccia == FILTRO_MALF) || (interfaccia == FILTRO_FREM))
			FlagKks = 1;
		XtManageChild (FormKks);
		}
	else
		{
		if ((interfaccia==FILTRO_CR) || (interfaccia == FILTRO_CR2))
			kks_filter = 0;
		if ((interfaccia==FILTRO_VAR) || (interfaccia == FILTRO_VAR2) || 
			 (interfaccia == FILTRO_MALF) || (interfaccia == FILTRO_FREM))
			FlagKks = 0;
		XtUnmanageChild (FormKks);
		}

	if (FiltroTipo)
		{
		if ((interfaccia==FILTRO_CR) || (interfaccia == FILTRO_CR2))
			tipo_filter = 1;
		if ((interfaccia==FILTRO_VAR) || (interfaccia == FILTRO_VAR2) || 
			 (interfaccia == FILTRO_MALF) || (interfaccia == FILTRO_FREM))
			FlagTipo = 1;
		XtManageChild (FormTipo);
		}
	else
		{
		if ((interfaccia==FILTRO_CR) || (interfaccia == FILTRO_CR2))
			tipo_filter = 0;
		if ((interfaccia==FILTRO_VAR) || (interfaccia == FILTRO_VAR2) || 
			 (interfaccia == FILTRO_MALF) || (interfaccia == FILTRO_FREM))
			FlagTipo = 0;
		XtUnmanageChild (FormTipo);
		}
/*
 * Tutte le variabili (nessun filtro)
 */
	if (FiltroAllVar) 
		{
		mess = (char *)malloc(strlen(NEW_FILTER)+strlen(ATTN_FILTER1)+strlen(ATTN_FILTER2)+10);
   	sprintf (mess,"%s\n%s\n%s",NEW_FILTER,ATTN_FILTER1,ATTN_FILTER2);
   	if (!create_attenzioneDialog (padre, mess,DIALOG_QUESTION))
      	{
      	free (mess);
			XmToggleButtonSetState (By_modblock,1,True);
			applicaFiltri (padre, interfaccia);
			return (-1);
      	}
		free (mess);
		XtUnmanageChild (FormModBlock);
		if ((interfaccia==FILTRO_CR) || (interfaccia == FILTRO_CR2))
			{
			modello_selezionato = NO_MOD;
      	blocco_selezionato = NO_BLOCK;
			display_lista_var (padre, modello_selezionato, blocco_selezionato,
                     kks_filter,tipo_filter);
			}
		if ((interfaccia==FILTRO_VAR) || (interfaccia == FILTRO_VAR2))
         {
			display_var (padre, NO_MOD, NO_BLOCK, FlagKks,FlagTipo);
			}
		if ((interfaccia == FILTRO_MALF) || (interfaccia == FILTRO_FREM))
			{
			visualizzaConfMalf (padre,interfaccia,NO_MOD, NO_BLOCK, FlagKks,FlagTipo); 	
    		}
		}
/* 
 * filtro per modulo e blocco
 */
	else if (FiltroModBlock)
		{
		XtManageChild (FormModBlock);
		XtManageChild (Label_blocks);
      XtManageChild (Find_form);
      XtManageChild (ListaBlock);
      display_lista_sistema (ListaSys);

		if ((interfaccia==FILTRO_CR) || (interfaccia == FILTRO_CR2))
         {
			if (modello_selezionato == NO_MOD) modello_selezionato = 1;
      	if (blocco_selezionato == NO_BLOCK) blocco_selezionato = 1;
      	display_lista_blocchi (ListaBlocchi);
			display_lista_var (padre, modello_selezionato, blocco_selezionato,
                     	kks_filter,tipo_filter);
			}
		if ((interfaccia==FILTRO_VAR) || (interfaccia == FILTRO_VAR2))
         {
			display_blocchi (ListaBlocchi);
			display_var (padre, nm, nb, FlagKks,FlagTipo);
			}
		if ((interfaccia == FILTRO_MALF) || (interfaccia == FILTRO_FREM))
         {
			visualizzaConfMalf (padre,interfaccia,nm, nb, FlagKks,FlagTipo);
			}
		}
/* 
 * filtro solo modello
 */
	else if (FiltroMod)
		{
		mess = (char *)malloc(strlen(NEW_FILTER)+strlen(ATTN_FILTER1)+strlen(ATTN_FILTER2)+10);
      sprintf (mess,"%s\n%s\n%s",NEW_FILTER,ATTN_FILTER1,ATTN_FILTER2);
      if (!create_attenzioneDialog (padre, mess,DIALOG_QUESTION))
         {
         free (mess);
         XmToggleButtonSetState (By_modblock,1,True);
         applicaFiltri (padre, interfaccia);
         return (-1);
         }
		XtManageChild (FormModBlock);
      XtUnmanageChild (Label_blocks);
      XtUnmanageChild (Find_form);
      XtUnmanageChild (ListaBlock);
      display_lista_sistema (ListaSys);
		if ((interfaccia==FILTRO_CR) || (interfaccia == FILTRO_CR2))
         {
			if (modello_selezionato == NO_MOD) modello_selezionato = 1;
      	blocco_selezionato = NO_BLOCK;
      	display_lista_var (padre, modello_selezionato, blocco_selezionato,
                     kks_filter,tipo_filter);
			}
		if ((interfaccia==FILTRO_VAR) || (interfaccia == FILTRO_VAR2))
         {
         display_blocchi (ListaBlocchi);
         display_var (padre, nm, nb, FlagKks,FlagTipo);
         }
		if ((interfaccia == FILTRO_MALF) || (interfaccia == FILTRO_FREM))
         {
         visualizzaConfMalf (padre,interfaccia,nm, nb, FlagKks,FlagTipo);
         }
		}
	
	return (0);
                              
}
/**********************************************************/
int get_kks_filter (w)
Widget w;
{
int i;
_UxCselVarCr *Ctx = (_UxCselVarCr*)UxGetContext(w);

	printf ("lettura filtro kks\n");
	get_byte_kks (Ctx->UxselVarCrKksText0,&filtro_kks[0]);
	get_byte_kks (Ctx->UxselVarCrKksText1,&filtro_kks[1]);
	get_byte_kks (Ctx->UxselVarCrKksText2,&filtro_kks[2]);
	get_byte_kks (Ctx->UxselVarCrKksText3,&filtro_kks[3]);
	get_byte_kks (Ctx->UxselVarCrKksText4,&filtro_kks[4]);
	get_byte_kks (Ctx->UxselVarCrKksText5,&filtro_kks[5]);
	get_byte_kks (Ctx->UxselVarCrKksText6,&filtro_kks[6]);
	get_byte_kks (Ctx->UxselVarCrKksText7,&filtro_kks[7]);
	get_byte_kks (Ctx->UxselVarCrKksText8,&filtro_kks[8]);
	get_byte_kks (Ctx->UxselVarCrKksText9,&filtro_kks[9]);
	get_byte_kks (Ctx->UxselVarCrKksText10,&filtro_kks[10]);
	filtro_kks[11] = '\0';

	printf ("kks_filtro = :%s:\n",filtro_kks);

	return (0);
}
/**********************************************************/
int get_byte_kks (w,n)
Widget w;
char *n;
{
char *stringa;
char s[5];
int app;
int app_1;

	stringa	= XmTextGetString (w);
	strcpy (s,stringa);
	XtFree (stringa);
	if ((s[0] == "") || (s[0] == '*'))
		*n = ' ';      /* any character */
	else
		{
		app = s[0];
		app_1 = toupper (app);
		s[0] = app_1;
		*n = s[0];
		}
	return (0);
}
/**********************************************************/
int display_lista_sistema (lista)
Widget lista;
{
   int       k;
   int       nmod;
   char      testo[200];
   char     *str;
   XmString  c_str;
   int       totale;

   nmod = numero_modelli(ind_sh_top);

   XtUnmanageChild(lista);
   XtVaGetValues(lista, XmNitemCount, &totale, NULL);
   if (totale)
      for (k = 1; k <= totale; k++, XmListDeletePos(lista, 1));

   for (k = 0; k < nmod; k++)
   {
      sprintf(testo, "%s", nome_modello(ind_sh_top, k + 1));
      str = (char *) XtMalloc(sizeof(char) * (strlen(testo) + 20));
      strcpy(str, testo);
      c_str = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
      XmListAddItemUnselected(lista, c_str, 0);
      XSync(UxDisplay, False);
      XtFree(str);
      XmStringFree(c_str);
   }
   XtManageChild(lista);
}
/**********************************************************/
int display_lista_blocchi (lista)
Widget lista;
{
   int       k;
   char      testo[200];
   char     *str;
   XmString  c_str;
   int       totale;

   XtUnmanageChild(lista);

	num_blocchi = 0;

   XtVaGetValues (lista, XmNitemCount, &totale, NULL);
   if (totale)
      for (k = 1; k <= totale; k++, XmListDeletePos(lista, 1));

   for (k = 0; k < numero_blocchi(ind_sh_top, modello_selezionato); k++)
   {
      sprintf(testo, "%s", nome_blocco(ind_sh_top, modello_selezionato, k + 1));
      str = (char *) XtMalloc(sizeof(char) * (strlen(testo) + 20));
      strcpy(str, testo);
      c_str = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
      XmListAddItemUnselected(lista, c_str, 0);
      XSync(UxDisplay, False);
		num_blocchi++;
		if (k == blocco_selezionato)
    		XmListSelectPos(lista, k, False);
      XtFree(str);
      XmStringFree(c_str);
   }
   if (k == blocco_selezionato)
      XmListSelectPos(lista, k, False);  
	mod_modificata = 1;
   XSync(UxDisplay, False);
   XtManageChild(lista);
}
/**********************************************************/
int next_bloc (lista, dir, text)
Widget lista;
int dir;
Widget text;
{
char *stringa;
static char *oldstringa;
static int primo_giro = 1;
static Boolean *match;
static int nmatch;
static int last_match;

   stringa = XmTextGetString (text);
   if (strlen(stringa) == 0) /* no input */
      {
      XtFree (stringa);
      return (0);
      }

   if (primo_giro)
      {
      oldstringa = (char *)malloc(strlen(stringa)+1);
      strcpy (oldstringa,stringa);
      match = (Boolean *)malloc(sizeof(Boolean)*num_blocchi);
      nmatch = lista_match_bloc (lista,stringa,match,
										num_blocchi,modello_selezionato);
      last_match = 0;
      primo_giro = 0;
      }
   if (strcmp(oldstringa,stringa) || mod_modificata )
      {
      free (oldstringa);
      oldstringa = (char *)malloc(strlen(stringa)+1);
      strcpy (oldstringa,stringa);
      free (match);
      match = (Boolean *)malloc(sizeof(Boolean)*num_blocchi);
      nmatch = lista_match_bloc (lista,stringa,match,
										num_blocchi,modello_selezionato);
      last_match = 0;
      if (mod_modificata) mod_modificata = 0;
      }

   if (nmatch > 0)
      posiziona_lista (lista, match,dir,&last_match);

   XtFree (stringa);
   return (0);
}
/**********************************************************/
int display_lista_var (lista,m,b,kks,tipo)
Widget lista;
int m; /* modello */
int b; /* blocco  */
int kks; /* filtro kks attivo/disattivo */
int tipo; /* filtro tipo attivo/disattivo */
{
Widget ListaToSel;
Widget ListaToUnsel;
Widget LabelToSel;
Widget LabelToUnsel;
int totale;
int i,k;
static int primo_giro = 1;
#define ALLOC_MIN		5000		/* minimo 5000 variabili */

int num_alloc_sel;
int num_alloc_unsel;

int num_alloc_sel_kks;
int num_alloc_unsel_kks;

int num_alloc_sel_tipo;
int num_alloc_unsel_tipo;

int size_alloc;

_UxCselVarCr *Ctx = (_UxCselVarCr*)UxGetContext(lista);

	ListaToSel   = Ctx->UxselVarToSelect;
	ListaToUnsel = Ctx->UxselVarToUnselect;
	LabelToSel   = Ctx->Uxlabel6;
	LabelToUnsel = Ctx->Uxlabel4;

	set_cursor (selVarCr, CLOCK);
/*
 * alloca i vettori per i puntatori al database variabili
 */
	if (!primo_giro)
		{
                if(punt_var_sel)
		   free (punt_var_sel);
                if(punt_var_unsel)
		   free (punt_var_unsel);
                if(punt_sel_kks)
		   free (punt_sel_kks);
                if(punt_unsel_kks)
		   free (punt_unsel_kks);
                if(punt_sel_tipo)
		   free (punt_sel_tipo);
                if(punt_unsel_tipo)
		   free (punt_unsel_tipo);
                if(punt_sel_lista)
		   free (punt_sel_lista);
                if(punt_unsel_lista)
		   free (punt_unsel_lista);
		}
	primo_giro = 0;

	num_alloc_sel = 1;
	num_alloc_unsel = 1;
	num_alloc_sel_kks = 1;
	num_alloc_unsel_kks = 1;
   num_alloc_sel_tipo  = 1;
   num_alloc_unsel_tipo = 1;
   

	sel_modificata = 1;  /* per funzioni find  */
	unsel_modificata = 1;  /* per funzioni find  */
/*
 * pulisce le liste
 */

	XtUnmanageChild(ListaToSel);
	XtVaGetValues (ListaToSel,XmNitemCount,&totale,NULL);
   if (totale)
      for (k = 1; k <= totale; k++, XmListDeletePos(ListaToSel, 1));

	XtUnmanageChild(ListaToUnsel);
   XtVaGetValues (ListaToUnsel,XmNitemCount,&totale,NULL);
   if (totale)
      for (k = 1; k <= totale; k++, XmListDeletePos(ListaToUnsel, 1));

/************************************************************************
 * FILTRI:
 *    sono previsti i seguenti filtri:
 *        	-	filtro lego (modulo/blocco)
 *				-	filtro KKS  (tag nella descrizione della variabile)=misure
 *				-  filtro tipo di grandezza (primo carattere della label )
 *
 *    Al momento attuale la relazione tra i filtri e' di tipo AND
 *   		nell' ordine specificato sopra. 
 *															30/1/96 fm
 ************************************************************************/
/* FILTRO PRINCIPALE
 * carica le liste
 */
	n_tounsel = 0;
	n_tosel = 0;
	size_alloc = ALLOC_MIN;
	punt_var_sel  = (int *)malloc (sizeof(int)*ALLOC_MIN*num_alloc_sel);
	punt_var_unsel= (int *)malloc (sizeof(int)*ALLOC_MIN*num_alloc_unsel);
	
	liste_modblock (m,b,
						&punt_var_sel,&n_tounsel,&num_alloc_sel,
						&punt_var_unsel,&n_tosel,&num_alloc_unsel,
						size_alloc);

printf ("liste_modblock terminata nsel %d nuns %d\n",n_tounsel,n_tosel);
printf ("liste_modblock terminata num_alloc_sel %d num_alloc_unsel %d\n",
							num_alloc_sel,num_alloc_unsel);

	punt_sel_kks  = (int *)malloc (sizeof(int)*ALLOC_MIN*num_alloc_sel);
	punt_unsel_kks=(int *)malloc(sizeof(int)*ALLOC_MIN*num_alloc_unsel);
	punt_sel_tipo  = (int *)malloc (sizeof(int)*ALLOC_MIN*num_alloc_sel);
	punt_unsel_tipo=(int *)malloc(sizeof(int)*ALLOC_MIN*num_alloc_unsel);
/*****************************
 * FILTRO KKS
 */

	if (kks)          /* filtro kks attivo  */
		{
printf ("*** FILTRO KKS ATTIVO  ***\n");
		lista_kks (punt_var_sel,n_tounsel,&n_tounsel_kks,punt_sel_kks,filtro_kks);
printf ("*** KKS sel %d \n",n_tounsel_kks);
		lista_kks (punt_var_unsel,n_tosel,&n_tosel_kks,punt_unsel_kks,filtro_kks);
printf ("*** KKS unsel %d\n",n_tosel_kks);
		}

/*****************************
 * FILTRO TIPO
 */
	if (tipo && !kks)
		{
printf ("*** FILTRO TIPO ATTIVO  ***\n");
		lista_tipo (punt_var_sel,n_tounsel,&n_tounsel_tipo,
										punt_sel_tipo,filtro_tipi);
		lista_tipo (punt_var_unsel,n_tosel,&n_tosel_tipo,
										punt_unsel_tipo,filtro_tipi);
		printf ("TIPI: %d to_unsel %d to_sel\n",n_tounsel_tipo,n_tosel_tipo);
		}

	if (tipo && kks)
      {
printf ("*** FILTRO TIPO ATTIVO  ***\n");
      lista_tipo (punt_sel_kks,n_tounsel_kks,&n_tounsel_tipo,
                              punt_sel_tipo,filtro_tipi);
      lista_tipo (punt_unsel_kks,n_tosel_kks,&n_tosel_tipo,
                              punt_unsel_tipo,filtro_tipi);
      printf ("TIPI: %d to_unsel %d to_sel\n",n_tounsel_tipo,n_tosel_tipo);
      }
/*****************************
 * DISPLAY DELLE LISTE
 * (N.B.: la relazione tra i filtri e AND)
 */

	size_alloc = ALLOC_MIN;

	num_alloc_sel = 0;
	num_alloc_unsel = 0;
	if (kks && !tipo)
		{
		punt_sel_lista  = (int *)malloc (sizeof(int)*n_tounsel_kks);
		punt_unsel_lista=(int *)malloc(sizeof(int)*n_tosel_kks);
		display_item_cr (ListaToUnsel,
						punt_sel_kks,n_tounsel_kks,
						&punt_sel_lista,&num_alloc_sel,size_alloc);
		preset_display_item_cr (ListaToSel,
						punt_unsel_kks,n_tosel_kks,
						&punt_unsel_lista,&num_alloc_unsel,size_alloc);
		elimina_doppie (ListaToSel,
						&punt_unsel_lista,&num_alloc_unsel,
						punt_sel_lista,num_alloc_sel);
		n_tounsel_display = num_alloc_sel;
		n_tosel_display = num_alloc_unsel; 
		update_labels (LabelToSel,n_tosel_display,LabelToUnsel,n_tounsel_display);
		}
	else if (tipo)
		{
		punt_sel_lista  = (int *)malloc (sizeof(int)*n_tounsel_tipo);
		punt_unsel_lista=(int *)malloc(sizeof(int)*n_tosel_tipo);
      display_item_cr (ListaToUnsel,
                  punt_sel_tipo,n_tounsel_tipo,
                  &punt_sel_lista,&num_alloc_sel,size_alloc);
      preset_display_item_cr (ListaToSel,
                  punt_unsel_tipo,n_tosel_tipo,
                  &punt_unsel_lista,&num_alloc_unsel,size_alloc);
		elimina_doppie (ListaToSel,
                  &punt_unsel_lista,&num_alloc_unsel,
                  punt_sel_lista,num_alloc_sel);
		n_tounsel_display = num_alloc_sel;
		n_tosel_display = num_alloc_unsel;
      update_labels (LabelToSel,n_tosel_display,LabelToUnsel,n_tounsel_display);
		}
	else
		{
		punt_sel_lista  = (int *)malloc (sizeof(int)*n_tounsel);
      punt_unsel_lista=(int *)malloc(sizeof(int)*n_tosel);
		display_item_cr (ListaToUnsel,
                  punt_var_sel,n_tounsel,
                  &punt_sel_lista,&num_alloc_sel,size_alloc);
		preset_display_item_cr (ListaToSel,
                  punt_var_unsel,n_tosel,
                  &punt_unsel_lista,&num_alloc_unsel,size_alloc);
		elimina_doppie (ListaToSel,
                  &punt_unsel_lista,&num_alloc_unsel,
                  punt_sel_lista,num_alloc_sel);
		n_tounsel_display = num_alloc_sel;
		n_tosel_display = num_alloc_unsel;
		update_labels (LabelToSel,n_tosel_display,LabelToUnsel,n_tounsel_display);
		}

	XtManageChild (ListaToSel);
	XtManageChild (ListaToUnsel);
	set_cursor (selVarCr, NORMALE);
	return(0);
}
/**********************************************************/
int liste_modblock (m,b,
						p_sel,n_sel,n_alloc_sel,
						p_unsel,n_unsel,n_alloc_unsel,
						dim)
int m;		        /* modello selezionato                        */
int b;              /* blocco selezionato                         */
int **p_sel;        /* punt. lista elementi selez. matching       */
int *n_sel;         /* n. elementi selez. matching                */
int *n_alloc_sel;   /* numero realloc selez.                      */
int **p_unsel;      /* punt. lista elementi non selez. matching   */
int *n_unsel;       /* n. elementi non selez. matching            */
int *n_alloc_unsel; /* numero realloc non selez.                  */
int dim;            /* elementi preallocati                       */
{
int i,k;
int n_s;
int n_u;

	n_s = *n_sel;
	n_u = *n_unsel;

	printf ("liste_modblock: ns = %d n_u = %d\n",n_s,n_u);

	if ((m == NO_MOD) && (b == NO_BLOCK))
		{
		for (i=0; i<tot_variabili; i++)
			{
			rialloca_puntatore (p_sel,n_s,n_alloc_sel,dim);
			rialloca_puntatore (p_unsel,n_u,n_alloc_unsel,dim);  
			if ((variabili[i].typ==GRAF_SEL)||(variabili[i].typ==GRAF_SEL_MOM))
            {
            (*p_sel)[n_s] = i;
            n_s++;
            }
         else
            {
            (*p_unsel)[n_u] = i;
            n_u++;
            }
			}
		printf ("NO_MOD NO_BLOCK nsel = %d nunsel = %d\n",n_s,n_u);
		}
	else if (( m >= 0) && (b == NO_BLOCK))
      {
		for (i=0; i<tot_variabili; i++)
         {
         rialloca_puntatore (p_sel,n_s,n_alloc_sel,dim);
         rialloca_puntatore (p_unsel,n_u,n_alloc_unsel,dim);
         if ( variabili[i].mod == m )
            {
            if ((flag_sel == SEL_REG && (variabili[i].typ == GRAF_SEL  ||
                                     variabili[i].typ == GRAF_SEL_MOM))
                                    ||
                (flag_sel == SEL_LGPH && variabili[i].typlg == GRAF_SEL  ||
                                    variabili[i].typlg == GRAF_SEL_MOM))
               {
               (*p_sel)[n_s] = i;
               n_s++;
               }
            else
               {
               (*p_unsel)[n_u] = i;
               n_u++;
               }
            }
			}
		printf ("NO_BLOCK nsel = %d nunsel = %d\n",n_s,n_u);
      }
	else if (( m >= 0) && (b  >= 0))
      {
		for (i=0; i<tot_variabili; i++)
         {
         rialloca_puntatore (p_sel,n_s,n_alloc_sel,dim);
         rialloca_puntatore (p_unsel,n_u,n_alloc_unsel,dim);
			if ( variabili[i].mod == m && (b == numero_blocchi(ind_sh_top, m)+1 ||
                                                     variabili[i].blocco == b))
				{
            if ((flag_sel == SEL_REG && (variabili[i].typ == GRAF_SEL  ||
                                     variabili[i].typ == GRAF_SEL_MOM))
                                    ||
                (flag_sel == SEL_LGPH && variabili[i].typlg == GRAF_SEL  ||
                                    variabili[i].typlg == GRAF_SEL_MOM))
            	{
            	(*p_sel)[n_s] = i;
            	n_s++;
            	}
         	else
            	{
            	(*p_unsel)[n_u] = i;
            	n_u++;
            	}
				}
			}
		printf ("nsel = %d nunsel = %d\n",n_s,n_u);
      }
	*n_sel = n_s;
	*n_unsel = n_u;
	return (0);
}
/**********************************************************/
int lista_modblock (m,b,
                  p_sel,n_sel,n_alloc_sel,
                  dim)
int m;              /* modello selezionato                        */
int b;              /* blocco selezionato                         */
int **p_sel;        /* punt. lista elementi selez. matching       */
int *n_sel;         /* n. elementi selez. matching                */
int *n_alloc_sel;   /* numero realloc selez.                      */
int dim;            /* elementi preallocati                       */
{
int i,k,j;
int n_s;
int ind_start,ind_end;
int nmod;

   n_s = *n_sel;

   printf ("lista_modblock: ns = %d \n",n_s);

   nmod = numero_modelli(ind_sh_top);

   if ((m == NO_MOD) && (b == NO_BLOCK))
      {
      for (i=0; i<tot_variabili; i++)
         {
         rialloca_puntatore (p_sel,n_s,n_alloc_sel,dim);
         (*p_sel)[n_s] = i;
         n_s++;
         }
      printf ("NO_MOD NO_BLOCK nsel = %d \n",n_s);
      }
   else if (( m >= 0) && (b == NO_BLOCK))
      {
      for(j=0;j<nmod;j++)
         if(m == ind_variabili[j].ind_mod)
            break;
      ind_start = ind_variabili[j].ind_var;
      if(j == (nmod-1))
         ind_end = tot_variabili;
      else
         ind_end = ind_variabili[j+1].ind_var;

printf("ind_start = %d ind_end = %d\n",ind_start,ind_end);

      for (i=ind_start; i<ind_end; i++)
         {
         rialloca_puntatore (p_sel,n_s,n_alloc_sel,dim);
         (*p_sel)[n_s] = i;
         n_s++;
         }
/*
         if ( variabili[i].mod == m )
            {
            (*p_sel)[n_s] = i;
            n_s++;
            }
*/
      printf ("NO_BLOCK nsel = %d \n",n_s);
      }
   else if (( m >= 0) && (b  >= 0))
      {
      for(j=0;j<nmod;j++)
         {
         if(m == ind_variabili[j].ind_mod)
            break;
         }
      ind_start = ind_variabili[j].ind_var;
      if(j == (nmod-1))
         ind_end = tot_variabili;
      else
         ind_end = ind_variabili[j+1].ind_var;

printf("ind_start = %d ind_end = %d\n",ind_start,ind_end);

      for (i=ind_start; i<ind_end; i++)
         {
         rialloca_puntatore (p_sel,n_s,n_alloc_sel,dim);
         if (b == numero_blocchi(ind_sh_top, m)+1 || variabili[i].blocco == b)
            {
            (*p_sel)[n_s] = i;
            n_s++;
            }
         }
/*
         if ( variabili[i].mod == m && (b == numero_blocchi(ind_sh_top, m)+1 ||
                                                     variabili[i].blocco == b))
            {
            (*p_sel)[n_s] = i;
            n_s++;
            }
*/
      printf (" VAR SELEZ MOD & BLOCK nsel = %d \n",n_s);
      }
   *n_sel = n_s;
   return (0);
}
/**********************************************************/
int lista_kks (p_in,n_in,n_sel,p_sel,filtro)
int *p_in;        /* punt. lista elementi per check     */
int n_in;         /* n. elementi per check              */
int *n_sel;       /* n. elementi matching               */
int *p_sel;       /* punt. lista elementi matching      */
char *filtro;     /* filtro kks                         */
{
int i,k;
int n;
/*
      n = *n_sel;
*/
      for (i=0,k=0,n=0; i<n_in; i++)
         {
         if (check_kks(p_in[i],filtro)>0) 
            {
            p_sel[k] = p_in[i];
            n++;
            k++;
            }
         }
		*n_sel = n;
		return (0);
}
/**********************************************************/
int lista_tipo (p_in,n_in,n_sel,p_sel,filtro)
int *p_in;           /* punt. lista elementi per check     */
int n_in;            /* n. elementi per check              */
int *n_sel;          /* n. elementi matching               */
int *p_sel;          /* punt. lista elementi matching      */
FILTRO_TIPI filtro;  /* filtro tipo                        */
{
int i,k;
int n;
/*
      n = *n_sel;
*/
      for (i=0,k=0,n=0; i<n_in; i++)
         {
         if (check_tipo(p_in[i],filtro)>0)
            {
            p_sel[k] = p_in[i];
            n++;
            k++;
            }
         }
      *n_sel = n;
      return (0);
}
/**********************************************************/
int display_item_cr (w, p_in, n_in, p_disp, n_disp, dim)
Widget w;			/* lista destinazione            */
int *p_in;        /* punt. lista elementi per display   */
int n_in;         /* numero elementi da analizzare      */
int **p_disp;      /* punt. lista elementi in display    */
int *n_disp;      /* numero elementi in lista           */
int dim;          /* dimensione dei realloc             */
{
int i,k;
int n; 
int esiste;
/******* modifica del 3 aprile 96 per eliminare doppie variabili
      for (i=0; i<n_in; i++)
         {
         add_lista (w, p_in[i]);
         (*p_disp)[i] = p_in[i];
         }
		*n_disp = n_in;
****************************************************************/
		for (i=0,k=0; i<n_in; i++)
         {
			esiste = 0;    /* per default non esiste */
			if (k != 0)
				{
				for (n=0; n<k; n++)
					{
					if (!strcmp(variabili[p_in[i]].nome,
									variabili[(*p_disp)[n]].nome))
						{
						esiste = 1;
						break;
						}
					}
				}
			if (!esiste)
				{
         	add_lista (w, p_in[i]);
         	(*p_disp)[k] = p_in[i];
				(*n_disp)++;
				k++;
				}
         }
		return (0);
}
/**********************************************************/
int elimina_doppie (w, p_disp, n_disp, p_conf, n_conf)
Widget w;         /* lista destinazione            */
int **p_disp;      /* punt. lista elementi in display    */
int *n_disp;      /* numero elementi in lista           */
int *p_conf;        /* punt. lista elementi per confronto   */
int n_conf;         /* numero elementi da analizzare      */
{
int i,k;
int n;

		for (i=0; i<(*n_disp); i++)
			{
			for (k=0; k<n_conf; k++)
				{
				if (!strcmp(variabili[(*p_disp)[i]].nome,
							variabili[p_conf[k]].nome))
					{
/*
printf ("doppia [%s] [%s]\n",variabili[(*p_disp)[i]].nome,
										variabili[p_conf[k]].nome);
*/
					(*p_disp)[i] = -1; /* doppia da eliminare     */
					break;
					}
				}
			}

		n = 0;
		for (i=0; i<(*n_disp); i++)
         {
			if ((*p_disp)[i] >= 0)     /* se e' valido */
				{
				(*p_disp)[n] = (*p_disp)[i];
				add_lista (w, (*p_disp)[n]);
				n++;	
				}
			}	
		(*n_disp) = n;

      return (0);
}
 
/**********************************************************/
/*
 * prepara la lista dei puntatori che verra' successivamente filtrato
 */
int preset_display_item_cr (w, p_in, n_in, p_disp, n_disp, dim)
Widget w;         /* lista destinazione            */
int *p_in;        /* punt. lista elementi per display   */
int n_in;         /* numero elementi da analizzare      */
int **p_disp;      /* punt. lista elementi in display    */
int *n_disp;      /* numero elementi in lista           */
int dim;          /* dimensione dei realloc             */
{
int i,k;
int n;
int esiste;

      for (i=0,k=0; i<n_in; i++)
         {
         esiste = 0;    /* per default non esiste */
         if (k != 0)
            {
            for (n=0; n<k; n++)
               {
               if (!strcmp(variabili[p_in[i]].nome,
                           variabili[(*p_disp)[n]].nome))
                  {
                  esiste = 1;
                  break;
                  }
               }
            }
         if (!esiste)
            {
            (*p_disp)[k] = p_in[i];
            (*n_disp)++;
            k++;
            }
         }
      return (0);
}
/**********************************************************/
int check_kks (indx, filtro)
int indx;			/* indice in database		*/
char *filtro;
{
int i,k;
char *p;
char delimiter_kks[] = "@#K@";
char stringa[MAX_LUN_DESCR_VAR];
char *s;
char *kks_label;
int match = 1;
	
	strcpy (stringa,variabili[indx].descr);
	s = (char *)malloc(strlen(filtro)+1);
	kks_label = (char *)malloc(strlen(filtro)+1);
	strcpy (s,filtro);

	for (i=0; i<strlen(stringa); i++)
		{
		if ((stringa[i] == '@') && (stringa[i+1] == '#') &&
			 (stringa[i+2] == 'K') && (stringa[i+3] == '@'))
			{
			for (k=0,i+=4; k<strlen(filtro); k++,i++)
				kks_label[k] = stringa[i];
			kks_label[strlen(filtro)] = '\0';
			
			for (k=0; k<strlen(filtro); k++)
				{
				if (filtro[k] == ' ')  /* any character      */
					continue;
				if (filtro[k] == kks_label[k]) /* matching   */
					continue;
				match = -1;
				}
			break;
			}
		}
	if (i == strlen(stringa))
		match = -1;

	free (s);
	free (kks_label);
	return (match);
	
}
/**********************************************************/
int check_tipo (indx, f)
int indx;         /* indice in database      */
FILTRO_TIPI f;
{
int i,k;
char label[MAX_LUN_NOME_VAR];
  
   strcpy (label,variabili[indx].nome);
	if ((test_tipo(label,f,indx) > 0) && (test_inout(label,f,indx) > 0))
		return (1);
	return (-1);
}
/**********************************************************/
int test_tipo (stringa, f, n)
char *stringa;
FILTRO_TIPI f;
{
char s[10];
int match = -1;
int test;
/*
 * se nessun filtro di tipo e' selezionato = nessun limite
 */

	if ((test = (f.pressione | f.temperatura | f.entalpia | f.portata |
		 f.potenza | f.giri | f.alzvalv)) == 0)
		{
		match = 1;
		return (match);
		}

	if (f.pressione) 
		{
		strcpy (s,FLAG_TIPO_PRESSIONE);
		if (!strncmp(stringa,s,strlen(s)))
			match = 1;
		}
   if (f.temperatura) 
      {
      strcpy (s,FLAG_TIPO_TEMPERATURA);
      if (!strncmp(stringa,s,strlen(s)))
			match = 1;
      strcpy (s,FLAG_TIPO_TEMPERATURA_X);
      if (!strncmp(stringa,s,strlen(s)))
			match = 1;
      }
   if (f.entalpia) 
      {
      strcpy (s,FLAG_TIPO_ENTALPIA);
      if (!strncmp(stringa,s,strlen(s)))
         match = 1;         
      }
   if (f.portata) 
      {
      strcpy (s,FLAG_TIPO_PORTATA);
      if (!strncmp(stringa,s,strlen(s)))
         match = 1;   
      }
   if (f.potenza) 
      {
      strcpy (s,FLAG_TIPO_POTENZA);
      if (!strncmp(stringa,s,strlen(s)))
         match = 1;
      }
   if (f.giri) 
      {
      strcpy (s,FLAG_TIPO_GIRI);
      if (!strncmp(stringa,s,strlen(s)))
         match = 1;
      strcpy (s,FLAG_TIPO_GIRI_RPM);
      if (!strncmp(stringa,s,strlen(s)))
         match = 1;
      }
   if (f.alzvalv) 
      {
      strcpy (s,FLAG_TIPO_ALZVALV);
      if (!strncmp(stringa,s,strlen(s)))
         match = 1;
      }

   return (match);
  
}
/**********************************************************/
int test_inout (stringa, f, n)
char *stringa;
FILTRO_TIPI f;
{
char s[10];
int match = -1;
int test;
/*
 * se nessun filtro di tipo e' selezionato = nessun limite
 */
	if ((test = (f.ingressi | f.uscite | f.aingabili)) == 0)
		{
		match = 1;
		return (match);
		}

	if (f.ingressi)
      {
      strcpy (s,FLAG_TIPO_INPUT);
      if (!string_exist (variabili[n].descr,s))
         match = 1;
      }
   if (f.uscite)
      {
      strcpy (s,FLAG_TIPO_OUTPUT);
      if (!string_exist (variabili[n].descr,s))
         match = 1;
      }
   if (f.aingabili)
      {
      if (!is_aingable (n))
         match = 1;
      }
	return (match);
 
}
/**********************************************************/
int string_exist (s1, s2)
char *s1;
char *s2;
{
int i;
int k;
	for (i=0; i<strlen(s1)-strlen(s2)+1; i++)
		{
		if (!strncmp (&s1[i],s2,strlen(s2)))
			return (0);
		}
	return (1);

}
/**********************************************************/
int is_aingable (i)
int i;
{
	if (variabili[i].tipo == INGRESSO_NC)
		return (0);
	return (1);
}
/**********************************************************/
int rialloca_puntatore (p,n,n_alloc,dim)
int **p;           /* puntatore da riallocare              */
int n;            /* numero di elementi allocati          */
int *n_alloc;      /* numero di realloc eseguite           */
int dim;          /* dimensione per realloc               */
{
int n_a;

	n_a = *n_alloc;
/**********
	if ((n>0) && (((n/dim)+1) > n_a))
      {
		printf ("** SI RIALLOCA \n");
      n_a++;
      *p = (int *)realloc (*p, sizeof(int)*dim*(n_a));  
      printf ("CONT_REC: realloc sel n. %d var sel %d\n",n_a,n);
      }
	*n_alloc = n_a;
************/
   if ((n>0) && (((n/dim)+1) > n_a))
      {
      printf ("** SI RIALLOCA n= %d \n",n);
      *p = (int *)realloc (*p, sizeof(int)*dim*((n/dim)+1));   
      printf ("CONT_REC: realloc sel n. %d var sel %d\n",(n/dim)+1,n);
      }
   *n_alloc = (n/dim)+1;
	return (0);
}
/**********************************************************/
int update_labels (w1,n1,w2,n2)
Widget w1;
int n1;
Widget w2;
int n2;
{
	
char *app;

	app = (char *)malloc(strlen(VAR_TO_SELECT)+50);
	sprintf (app,"%s: %d (tot. %d)",VAR_TO_SELECT,n1,tot_variabili);
	XtVaSetValues (w1,RES_CONVERT(XmNlabelString,app), NULL);
	free (app);

	app = (char *)malloc(strlen(VAR_SELECTED)+20);
	sprintf (app,"%s: %d (tot. %d)",VAR_SELECTED,n2,nu_var_sel);
	XtVaSetValues (w2,RES_CONVERT(XmNlabelString,app), NULL);
   free (app);

	return (0);
}
/**********************************************************/
int add_lista (w, i)
Widget w;
int i;    /* indice variabile */
{
char      testo[500];
char     *str;
XmString  c_str;

    sprintf( testo, "%s - %s", variabili[i].nome, variabili[i].descr);
    str = (char *)XtMalloc(sizeof(char)*((strlen(testo)) + 20));
    strcpy(str, testo);
    c_str = XmStringCreate (str, XmSTRING_DEFAULT_CHARSET);
    XmListAddItemUnselected(w, c_str, 0); 
    XSync(UxDisplay, False);
    XtFree(str);
    XmStringFree(c_str);

	return (0);
}
/**********************************************************/
int add_var (from, to)
Widget from;           /* lista di provenienza        */
Widget to;             /* lista di destinazione       */
{
int i;
int nselect;
int *elenco;

	if (!XmListGetSelectedPos (from, &elenco,&nselect))
		{
		printf ("nessun item selezionato\n");
		return (-1);
		}
	printf ("selezionati %d items\n",nselect);
	for (i=0; i<nselect; i++)
		{
		if (flag_sel == SEL_REG)
			{
/*			variabili[punt_var_unsel[elenco[i]-1]].typ = GRAF_SEL;   */
			variabili[punt_unsel_lista[elenco[i]-1]].typ = GRAF_SEL;
			nu_var_sel++;
			}
		}

	free (elenco);
	display_lista_var (from, modello_selezionato, blocco_selezionato,
							kks_filter,tipo_filter);	
	return (0);
}
/**********************************************************/
int del_var (from, to)
Widget from;           /* lista di provenienza        */
Widget to;             /* lista di destinazione       */
{
int i;
int nselect;
int *elenco;

   if (!XmListGetSelectedPos (from, &elenco,&nselect))
      {
      printf ("nessun item selezionato\n");
      return (-1);
      }
   printf ("selezionati %d items\n",nselect);
   for (i=0; i<nselect; i++)
      {
      if (flag_sel == SEL_REG)
			{
/*         variabili[punt_var_sel[elenco[i]-1]].typ = NO_GRAF_SEL;   **/
         variabili[punt_sel_lista[elenco[i]-1]].typ = NO_GRAF_SEL;
			nu_var_sel--;
			}
      }

   free (elenco);
   display_lista_var (from, modello_selezionato, blocco_selezionato,
							kks_filter,tipo_filter);
   return (0);
}
/**********************************************************/
int seleziona_all_to_sel (lista, text)
Widget lista;
Widget text;
{
int totale;
int k;
char *stringa;
Boolean *match;
int nmatch;

	stringa = XmTextGetString (text);
   if (strlen(stringa) == 0) /* no input */
		{
		XtFree (stringa);
		XtVaGetValues(lista, XmNitemCount, &totale, NULL);
   	if (totale)
			{
			XmListDeselectAllItems (lista);
			XtVaSetValues (lista, XmNselectionPolicy, XmMULTIPLE_SELECT, NULL);
      	for (k = 1; k <= totale; k++) 
				XmListSelectPos(lista,k,True);        
			XtVaSetValues (lista, XmNselectionPolicy, XmEXTENDED_SELECT, NULL);
			}
		}
	else   /* seleziona tutti i matches  */
		{
		XtVaGetValues(lista, XmNitemCount, &totale, NULL);
		if (totale)
         {
			match = (Boolean *)malloc(sizeof(Boolean)*totale);
			nmatch = lista_match_to_sel (lista,stringa,match);
			if (nmatch > 0)
				{
				XmListDeselectAllItems (lista);
				XtVaSetValues (lista, XmNselectionPolicy, XmMULTIPLE_SELECT, NULL);
				for (k = 1; k <= totale; k++)
					{
					if (match[k-1])
						XmListSelectPos(lista,k,True);
					}
				XtVaSetValues (lista,XmNselectionPolicy,XmEXTENDED_SELECT,NULL);
				}
			else         /* nessun match, seleziona tutto (opinabile)   */
				{
				XmListDeselectAllItems (lista);
				XtVaSetValues (lista, XmNselectionPolicy, XmMULTIPLE_SELECT, NULL);
         	for (k = 1; k <= totale; k++)
            	XmListSelectPos(lista,k,True);
         	XtVaSetValues (lista,XmNselectionPolicy,XmEXTENDED_SELECT,NULL);
				}
			}
		}

	return (0);
}
/**********************************************************/
int seleziona_all_to_unsel (lista, text)
Widget lista;
Widget text;
{
int totale;
int k;
char *stringa;
Boolean *match;
int nmatch;

   stringa = XmTextGetString (text);
   if (strlen(stringa) == 0) /* no input */
      {
      XtFree (stringa);
      XtVaGetValues(lista, XmNitemCount, &totale, NULL);
      if (totale)
         {
         XmListDeselectAllItems (lista);
         XtVaSetValues (lista, XmNselectionPolicy, XmMULTIPLE_SELECT, NULL);
         for (k = 1; k <= totale; k++)
            XmListSelectPos(lista,k,True);
         XtVaSetValues (lista, XmNselectionPolicy, XmEXTENDED_SELECT, NULL);
         }
      }
   else   /* seleziona tutti i matches  */
      {
      XtVaGetValues(lista, XmNitemCount, &totale, NULL);
      if (totale)
         {
         match = (Boolean *)malloc(sizeof(Boolean)*totale);
         nmatch = lista_match_to_unsel (lista,stringa,match);
         if (nmatch > 0)
            {
            XmListDeselectAllItems (lista);
            XtVaSetValues (lista, XmNselectionPolicy, XmMULTIPLE_SELECT, NULL);
            for (k = 1; k <= totale; k++)
               {
               if (match[k-1])
                  XmListSelectPos(lista,k,True);
               }
            XtVaSetValues (lista,XmNselectionPolicy,XmEXTENDED_SELECT,NULL);
            }
         else         /* nessun match, seleziona tutto (opinabile)   */
            {
            XmListDeselectAllItems (lista);
            XtVaSetValues (lista, XmNselectionPolicy, XmMULTIPLE_SELECT, NULL);
            for (k = 1; k <= totale; k++)
               XmListSelectPos(lista,k,True);
            XtVaSetValues (lista,XmNselectionPolicy,XmEXTENDED_SELECT,NULL);
            }
         }
      }

   return (0);
}
/**********************************************************/
int salva_sel_var (w, flg, s)
Widget w;      /* per avere in context   */
int flg;    
char *s;       /* file name              */
{
char *messaggio;
int esito = 0;

	set_cursor (selVarCr,CLOCK);
	messaggio = malloc (strlen(SAVE_FILE_CR)+strlen(s)+strlen(OPER_FALLITA)+20);
	if (flg == SEL_REG)
		{
		scrive_sel (s);
		sprintf (messaggio,"%s:%s",SAVE_FILE_CR,s);
		}
	else
		{
		sprintf (messaggio,"%s:%s %s",SAVE_FILE_CR,s,OPER_FALLITA);
		attenzione (selVarCr,messaggio);
		}
	add_message (areaMessaggi,messaggio,LIVELLO_1);
	free (messaggio);
	set_cursor (selVarCr,NORMALE);
   return (esito);
}
/**********************************************************/
int newvarcr (w)
Widget w;
{
char *messaggio;
int esito = 0;

	set_cursor (selVarCr,CLOCK);
   messaggio = malloc (strlen(NEW_VAR_CR) + strlen(OPER_FALLITA) + 20);
	if (stato_sim == STATO_FREEZE)
		if (SD_newvarcr (BANCO) > 0)
			sprintf (messaggio,"%s",NEW_VAR_CR);
		else
			{
			sprintf (messaggio,"%s : %s",NEW_VAR_CR, OPER_FALLITA);
			esito = -1;
			attenzione(w,messaggio);
			}
	else
		{
		elimina_f22();
		sprintf (messaggio,"%s",NEW_VAR_CR);
		}

	add_message (areaMessaggi,messaggio,LIVELLO_1);
	free (messaggio);
	set_cursor (selVarCr,NORMALE);
	return (esito);
}
/**********************************************************/
int attiva_graphics (w,d)
Widget w;
char *d;                      /* display         */
{
char *path;
char path_com[MAX_PATH_LEN];
char comando[400];
char *messaggio;
int esito = 0;

	set_cursor (w,CLOCK);
	messaggio = malloc (strlen(PROGRAM_ACT)+strlen(VIEW_PROGRAM)+
								strlen(OPER_FALLITA) + 20);
   path = (char *) getcwd((char *) NULL, 300);
   if(path == NULL)
		{
   	sprintf (messaggio,"%s : %s %s",PROGRAM_ACT,VIEW_PROGRAM,OPER_FALLITA);
		add_message (areaMessaggi,messaggio,LIVELLO_1);
		attenzione(w,messaggio);
		esito = -1;
		set_cursor (w,NORMALE);
      return(esito);
		}
	strcpy (path_com, getenv("LEGORT_BIN"));
/*
 * se il display non e' settato o e' settato il local
 */
	if ((!strcmp(d,DEFAULT_DISPLAY)) || (!strcmp(d,"")))
		sprintf (comando,"%s/%s %s/%s &",path_com,VIEW_PROGRAM,
									path,FILE_DATA_DEFAULT);
	else
		sprintf (comando,"%s/%s %s/%s -d %s &",path_com,VIEW_PROGRAM,
                           path,FILE_DATA_DEFAULT,d);
   free(path);

printf ("ATTIVAZIONE: [%s]\n",comando);
   system(comando);
   sprintf (messaggio,"%s : %s",PROGRAM_ACT,VIEW_PROGRAM);
	add_message (areaMessaggi,messaggio,LIVELLO_1);
	free (messaggio);
	set_cursor (w,NORMALE);

	return (esito);
}
/**********************************************************/
int loadas_cr (w,modo,tipo)
Widget w;         /* chiamante        */
int modo; /* lettura scrittura              */
int tipo; /* VAR_FOR_CR, PREVIEW_FILE, etc) */
{
char nome[MAX_PATH_LEN];
char *messaggio;
int ret;

   if ((ret = selezione_file (selVarCr,modo,tipo,nome)) < 0)
      return (ret);
   messaggio = (char *)malloc(sizeof(SAVE_FILE_CR)+sizeof(nome)+10);
   set_cursor (w,CLOCK);
   legge_sel (nome);   /* fa anche il clear preliminare della banca dati */
	init_sel_var_cr (w,nome);
   sprintf (messaggio,"%s \n %s", LOAD_FILE_CR,nome);
   set_cursor (w,NORMALE);
   add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);
   return (0);
}
/**********************************************************/
int saveas_cr (w,modo,tipo)
Widget w;         /* chiamante        */
int modo; /* lettura scrittura              */
int tipo; /* VAR_FOR_CR, PREVIEW_FILE, etc) */
{
char nome[MAX_PATH_LEN];
char *messaggio;
int ret;

	if ((ret = selezione_file (selVarCr,modo,tipo,nome)) < 0)
		return (ret);

	messaggio = (char *)malloc(sizeof(SAVE_FILE_CR)+sizeof(nome)+10);
	set_cursor (w,CLOCK);
	scrive_sel (nome);
   sprintf (messaggio,"%s:%s",SAVE_FILE_CR,nome);
	set_cursor (w,NORMALE);
	add_message (areaMessaggi,messaggio,LIVELLO_3);
	free (messaggio);
	return (0);
}
/***********************************************************/
int init_sel_var_cr (w,s)
Widget w;
char *s;
{
char *stringa;

   printf ("*** Numero variabili selezionate = %d\n",nu_var_sel);
   display_lista_var (w, 
			modello_selezionato,blocco_selezionato,
										kks_filter,tipo_filter);
   stringa = (char *)malloc(strlen(SEL_VAR_TITLE)+
                            strlen(s) +10);
   sprintf (stringa,"%s: %s",SEL_VAR_TITLE,s);
   update_title (selVarCr,stringa);
   free (stringa);
   return (0);
}
/**********************************************************/
int selezione_file (w,modo,tipo,nome)
Widget w; /* interfaccia chiamante          */
int modo; /* lettura scrittura              */
int tipo; /* VAR_FOR_CR, PREVIEW_FILE, etc) */
char *nome;  /* file name                   */
{
/*
 * restituisce il filename gia' filtrato
 */
	strncpy (nome, create_fileSelection (w,modo,tipo),MAX_PATH_LEN-1);
	nome[MAX_PATH_LEN-1] = '\0';
	if (!strcmp(nome,""))           /* cancel da colloquio*/
		return (-2);
	printf ("selez. file. :%s:\n",nome);
	if (!strcmp(nome,FILE_ERRORE)) /* file non validato  */
		return (-1);
	return (0);
}
/***********************************************************/
int next_pos_to_sel (lista, dir, text)
Widget lista;
int dir;
Widget text;
{
/*
 * sdoppiate le funzioni per bantenere i 2 match in parallelo
 */
char *stringa;
static char *oldstringa;
static int primo_giro = 1;
static Boolean *match;
static int nmatch;
static int last_match;

	stringa = XmTextGetString (text);
   if (strlen(stringa) == 0) /* no input */
		{
		XtFree (stringa);
		return (0);
		}

	if (primo_giro)
		{
		oldstringa = (char *)malloc(strlen(stringa)+1);
		strcpy (oldstringa,stringa);
		match = (Boolean *)malloc(sizeof(Boolean)*n_tosel);
		nmatch = lista_match_to_sel (lista,stringa,match);
		last_match = 0;
		primo_giro = 0;
		}
	if (strcmp(oldstringa,stringa) || 
		/*		sel_modificata ||*/ unsel_modificata)
		{
		printf ("Rigenera match con [%s]\n",stringa);
		free (oldstringa);
		oldstringa = (char *)malloc(strlen(stringa)+1);
      strcpy (oldstringa,stringa);
		free (match);
		match = (Boolean *)malloc(sizeof(Boolean)*n_tosel);
		nmatch = lista_match_to_sel (lista,stringa,match);
		last_match = 0;
/*		if (sel_modificata) sel_modificata = 0;   */
		if (unsel_modificata) unsel_modificata = 0;
		}
	if (nmatch > 0)
		posiziona_lista (lista, match,dir,&last_match);
	XtFree (stringa);	
	return (0);
}
/***********************************************************/
int next_pos_to_unsel (lista, dir, text)
Widget lista;
int dir;
Widget text;
{
/*
 * sdoppiate le funzioni per bantenere i 2 match in parallelo
 */
char *stringa;
static char *oldstringa;
static int primo_giro = 1;
static Boolean *match;
static int nmatch;
static int last_match;

   stringa = XmTextGetString (text);
   if (strlen(stringa) == 0) /* no input */
      {
      XtFree (stringa);
      return (0);
      }

   if (primo_giro)
      {
      oldstringa = (char *)malloc(strlen(stringa)+1);
      strcpy (oldstringa,stringa);
      match = (Boolean *)malloc(sizeof(Boolean)*n_tounsel);
      nmatch = lista_match_to_unsel (lista,stringa,match);
      last_match = 0;
      primo_giro = 0;
      }
   if (strcmp(oldstringa,stringa) || 
			sel_modificata) /* || unsel_modificata) */
      {
		printf ("Rigenera match con [%s]\n",stringa);
      free (oldstringa);
      oldstringa = (char *)malloc(strlen(stringa)+1);
      strcpy (oldstringa,stringa);
      free (match);
      match = (Boolean *)malloc(sizeof(Boolean)*n_tounsel);
      nmatch = lista_match_to_unsel (lista,stringa,match);
      last_match = 0;
      if (sel_modificata) sel_modificata = 0;
 /*     if (unsel_modificata) unsel_modificata = 0; */
      }
   if (nmatch > 0)
      posiziona_lista (lista, match,dir,&last_match);
   XtFree (stringa);
   return (0);
}
/***********************************************************/
int lista_match_to_sel (lista,s,flag)
Widget lista;
char *s;
Boolean *flag;
{
int i;
int nmatch = 0;

	for (i=0; i<n_tosel_display; i++)
		{
/*		if ((Utstrstr (variabili[punt_var_unsel[i]].nome, s) != NULL) ||
			 (Utstrstr (variabili[punt_var_unsel[i]].descr, s) != NULL))  */

		if ((Utstrstr (variabili[punt_unsel_lista[i]].nome, s) != NULL) ||
			 (Utstrstr (variabili[punt_unsel_lista[i]].descr, s) != NULL))
			{
			flag[i] = True;
			nmatch ++;
			}
		else
			flag[i] = False;
		}
	return (nmatch);
}
/***********************************************************/
int lista_match_to_unsel (lista,s,flag)
Widget lista;
char *s;
Boolean *flag;
{
int i;
int nmatch = 0;

   for (i=0; i<n_tounsel_display; i++)
      {
/*      if ((Utstrstr (variabili[punt_var_sel[i]].nome, s) != NULL) ||
          (Utstrstr (variabili[punt_var_sel[i]].descr, s) != NULL))  ****/
      if ((Utstrstr (variabili[punt_sel_lista[i]].nome, s) != NULL) ||
          (Utstrstr (variabili[punt_sel_lista[i]].descr, s) != NULL))
         {
         flag[i] = True;
         nmatch ++;
         }
      else
         flag[i] = False;
      }
   return (nmatch);
}
/***********************************************************/
int posiziona_lista (w, flag, dir, last)
Widget w;
Boolean *flag;
int dir;
int *last;
{
int i;
int n;
int primo;
int tot;
int first_item;
int next;

	XtVaGetValues (w,
         XmNvisibleItemCount, &n,
         XmNtopItemPosition , &primo,
         XmNitemCount       , &tot,
         NULL);

	next = next_match (flag,dir,tot,last);

	printf ("vis. n= %d  primo %d it. cnt. %d next = %d\n",
					n,primo,tot,next+1);
	switch (dir)
		{
		case SEARCH_DOWN:
			if (((next+1) > (primo+n-1)) ||
				 ((next+1) < primo))
				{
				first_item = next+1;  /* il primo in alto */
            XtVaSetValues (w, XmNtopItemPosition, first_item, NULL);
				}
			XmListSelectPos (w, next+1, True);
			break;
		case SEARCH_UP:
			 if (tot > n)     /* in lista una parte del totale */
            {
				if (((next+1) > (primo+n-1)) || 
					 ((next+1) < primo))  /* elemento da selezionare non visibile */
					{
            	if ((first_item = next+1-n+1) < 1)
						first_item = 1;
            	XtVaSetValues (w, XmNtopItemPosition, first_item, NULL);
					}
            }
         XmListSelectPos (w, next+1, True);
			break;
		}
	
}
/***********************************************************/
int next_match (flag, dir, n, last)
Boolean *flag;
int dir;
int n;
int *last;
{
int i;
int count;
int ultimo;

	ultimo = *last;
/*
	printf ("ultimo =  %d\n",ultimo);
*/

	switch (dir)
      {
      case SEARCH_DOWN:
			for (i=++(*last),count=0; count<n; i++,count++)
				{
				if (i == n) 
					i=0;
				if (flag[i])
					{
					*last = i;
					return (i);
					}
				}
			return (ultimo);
         break;
      case SEARCH_UP:
			for (i=--(*last),count=0; count<n; i--,count++)
				{
				if (i < 0)
					i = n-1;
				if (flag[i])
               {
               *last = i;
               return (i);
               }
				}
			return (ultimo);
         break;
      }
}
/**************************************************************/
int richiesta_timer_cr (w)
Widget w;
{
char risp [10];
char old_val[10];
int new_val;
char *messaggio;
int esito;

#ifndef OLD_STAT
   sprintf (old_val,"%4.1f",statistiche.stato_parametri.passo_reg_cr);
#else
   sprintf (old_val,"%4.1f",val.actual.passo_reg_cr);
#endif
   create_richiestaDati (w, NEW_STEP_CR, old_val, risp);
   new_val = atoi(risp);
   printf ("risp = %s: %d\n",risp,new_val);

   esito = 0;
   messaggio = (char *)malloc (strlen(NEW_STEP_CR)+strlen(OPER_FALLITA)+20);

   if (SD_stepcr(BANCO, &new_val) >0)
      sprintf (messaggio,"%s :%d",NEW_STEP_CR,new_val);
   else
      {
      sprintf (messaggio,"%s :%d %s",NEW_STEP_CR,new_val,OPER_FALLITA);
      esito = -1;
      attenzione(w,messaggio);
      }

   add_message (areaMessaggi,messaggio,LIVELLO_1);
   free (messaggio);
   return(esito);
}
