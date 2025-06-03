/**********************************************************************
*
*       C Source:               refresh.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue May  4 10:50:53 1999 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: refresh.c-12 %  (%full_filespec: refresh.c-12:csrc:1 %)";
#endif

/*
 * refresh.c
 *
 * timer usati nell' applicazione banco istruttore
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <Xm/PushB.h>

#include "UxXt.h"
#include "sked.h"
#include "messaggi.h"
#include "init_sim.h"
#include "bistrutt.h"
#include "refresh.h"

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

#include "backtrack.h"

#include "filtri.h"
#include "option.h"
#include "dispatcher.h"
#include "tabelle_malf.h"

#include "banco_globals.h"
#include "masterMenu.h"
#include "malfConfig.h"
#include "listaScenari.h"
#include "initialCondition.h"
#include "scenarioMalf.h"
#include "parametri.h"
#include "malfunzioni.h"
//#include "dispatcher.h"


#ifndef OLD_STAT
extern STATISTICHE statistiche;   /* ricezione statistiche da sked */
#else
/*
 * provvisoriamente si usa la vecchia struttura statistiche
 */
STATISTICA_SKED statistiche;
#endif
VAL val;                          /* struttura per display         */

int freeze_rest = 0;		  /* Flag che segnala se almeno una task
                                     e' caduta e ripartita               */

extern TIPO_PERT *perturbazioni;
extern int _MAX_PERTUR;
extern int _MAX_SNAP_SHOT;
extern TIPO_PERT *pertMalf;			/* area pert per malfunzioni/funz. remote */
extern TIPO_PERT *pertMalfOld;			/* area pert per malfunzioni/funz. remote */
extern TIPO_PERT *perturbazioni_old;
extern int _MAX_PERTUR;

static XtIntervalId timer_masterMenu;
static XtIntervalId timer_mainIface;
extern XtIntervalId timer_initci;
extern XtIntervalId timer_initbt;
extern XtIntervalId timer_initialCondition;
extern XtIntervalId timer_replayOn;
extern XtIntervalId timer_editPert;
extern XtIntervalId timer_listaScenMalf;
extern XtIntervalId timer_scenarioMalf;
extern XtIntervalId timer_leggiPertMalf;
extern XtIntervalId timer_malfConfig;

extern int leggiPertMalfAttivo;

static void refresh_masterMenu();
static void refresh_mainIface();
void stop_timer();
static void refresh_initci();
static void refresh_initbt();
static void refresh_initialCondition();
static void refresh_replay();
static void refresh_editPert();
static void refresh_editPertMalf();
static void refresh_listaScenari();
static void refresh_sommarioMalf();
static void refresh_malfConfig();
extern void refresh_scenarioMalf();


float intervallo_backtrack;

extern Widget masterMenu;
extern Widget masterMenuRunpb;
extern Widget masterMenuFreezepb;
extern Widget icInitpb;
extern Widget simTimeDisplay;
extern Widget delayTimeDisplay;
extern Widget areaMessaggi;
extern Widget editPert;
extern Widget backtrackManagement;

extern int _MAX_BACK_TRACK;
extern float _TIME_BACK_TRACK;

extern int stato_sim;
extern int tasto_loadci_attivabile;
extern int tasto_loadbt_attivabile;
extern int tasto_initic_attivabile;
extern int isSuperuser;
extern int nuova_attivazione;
extern char *ind_sh_top;
extern int editPertRate;

extern M_COMP_TYPE *m_comp_type;
extern MALF_SET *malf_set;
extern M_COMPONENT *m_component;

extern M_COMP_TYPE *fr_comp_type;
extern MALF_SET *frem_set;
extern M_COMPONENT *fr_component;

extern int inizializzazione;
extern int nuovoSnapCaricato;
extern int nuovoSnapCaricato2;

int rep_freeze_err = 0;

char   *nome_modello (char *, int);

int read_background_color (Widget);
int colore_default;

int leggiPertMalf ();
int refreshSommarioMalf (Widget , SCENARIO *, TIPO_PERT *);
int aggiorna_pert (Widget);
int leggiPert ();
int displayPert (Widget );
int SD_editpert (int , TIPO_PERT *, int , int );
int update_tasti_initialCondition (Widget);
int reset_replay_mode(Widget);
int update_btload (Widget,int);
int refresh_colors_bt (Widget, int, int);
int update_tasti_initci(Widget);
int update_tasti_initbt(Widget);
int main_refresh(Widget);
int riceve_statistiche(Widget,int*);
int update_tasti_masterMenu(Widget);
int statistiche_refresh (Widget,int);
int trigger_backtrack(Widget);
int checkMaxTime(Widget);
int sblocca_tastiera(Widget);
int blocca_tastiera(Widget);
int simulator_error(Widget);
int preset_statistiche();
int check_statistiche();
int read_last_backtrack(Widget);
int esegui_snap_bt (int,char*);





/***************************************************************/
int attiva_timer_mainIface (w)
Widget w;
{
	timer_mainIface = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)200, refresh_mainIface, w);	
	return(0);
}
/***************************************************************/
int attiva_timer_masterMenu (w)
Widget w;
{
   timer_masterMenu = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)1000, refresh_masterMenu, NULL);
   return(0);
}
/***************************************************************/
int attiva_timer_initialCondition (w)
Widget w;
{
   timer_initialCondition = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)300, refresh_initialCondition, w);
   return(0);
}
/***************************************************************/
int attiva_timer_initSimci (w)
Widget w;
{
   timer_initci = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)300, refresh_initci, w);
   return(0);
}
/***************************************************************/
int attiva_timer_sommarioMalf (w)
Widget w;
{
_UxCscenarioMalf *Ctx;
	Ctx = (_UxCscenarioMalf *)UxGetContext(w);
   Ctx->Uxtimer_sommarioMalf = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)(1500), refresh_sommarioMalf, w);
   return(0);
}
/***************************************************************/
int remove_timer_editPertMalf ()
{
   leggiPertMalfAttivo --;
   if (leggiPertMalfAttivo == 0)
      {
printf ("DISATTIVAZIONE timer_leggiPertMalf ********************\n");
		free (pertMalf);
		free (pertMalfOld);
		stop_timer (timer_leggiPertMalf);
      }
   return(0);
}
/***************************************************************/
int attiva_timer_editPertMalf (w)
Widget w;
{
	leggiPertMalfAttivo ++;
	if (leggiPertMalfAttivo == 1)
		{
printf ("ATTIVAZIONE timer_leggiPertMalf ********************\n");
		pertMalf = (TIPO_PERT *)malloc (sizeof(TIPO_PERT)*_MAX_PERTUR*2);
      pertMalfOld = (TIPO_PERT *)malloc (sizeof(TIPO_PERT)*_MAX_PERTUR*2);
   	timer_leggiPertMalf = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)(editPertRate*1000), refresh_editPertMalf, w);
		}
   return(0);
}
/***************************************************************/
int riattiva_timer_editPertMalf (w)
Widget w;
{
   timer_leggiPertMalf = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)(editPertRate*1500), refresh_editPertMalf, w);
   return(0);
}
/***************************************************************/
void refresh_editPertMalf (data)
XtPointer data;
{
Widget w = (Widget)data;
_UxCscenarioMalf *Ctx;

		  Ctx = (_UxCscenarioMalf *)UxGetContext(w);
        leggiPertMalf ();
        riattiva_timer_editPertMalf (w);
        return;
}
/***************************************************************/
void refresh_sommarioMalf (data)
XtPointer data;
{
Widget w = (Widget)data;
_UxCscenarioMalf *Ctx;
	Ctx = (_UxCscenarioMalf *)UxGetContext(w);
   refreshSommarioMalf (w, &Ctx->Uxscenario, pertMalf);
	attiva_timer_sommarioMalf (w);
}
/***************************************************************/
int leggiPertMalf ()
{
int k;
/*** anche in freeze per vedere caricamenti CI e BT
	if (stato_sim != STATO_FREEZE) 
		{
***********/
		if (SD_editpert (BANCO, pertMalf,0,_MAX_PERTUR) > 0)
			{
/******
      	fprintf (stderr,"PERT MALF ricevute\n");   
			for (k=0; k<_MAX_PERTUR*2; k++)
				{
				if	((pertMalf[k].tipo == PERT_MALFUNCTION) ||
                     (pertMalf[k].tipo == PERT_MALFUNCTION_STOP))
					{
					printf ("RICEVUTO MALF POS. %d\n",k);
					}
				}
***************/
			}
   	else
      	fprintf (stderr,"PERT MALF *** ERRORE ricezione PERT ***\n");
/******
		}
**********/
   return (0);
}
/***************************************************************/
int attiva_timer_editPert (w)
Widget w;
{
   timer_editPert = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)(editPertRate*1000), refresh_editPert, w);
   return(0);
}
/***************************************************************/
void refresh_editPert (data)
XtPointer data;
{
Widget w = (Widget)data;

		  aggiorna_pert (w);	
        attiva_timer_editPert (w);
        return;
}
/***************************************************************/
int aggiorna_pert (w)
Widget w;
{
	leggiPert ();
	if (memcmp (perturbazioni_old,
					perturbazioni,sizeof(TIPO_PERT)*_MAX_PERTUR*2) != 0)
		{
		memcpy(perturbazioni_old,
      			perturbazioni,sizeof(TIPO_PERT)*_MAX_PERTUR*2);
		displayPert (w);
		}
	return (0);
}
/***************************************************************/
int attiva_timer_listaScenari (w)
Widget w;
{
_UxClistaScenari *Ctx;
	Ctx = (_UxClistaScenari *)UxGetContext(w);
   Ctx->Uxtimer_listaScenari = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)5000, refresh_listaScenari, w);
   return(0);
}
/***************************************************************/
void refresh_listaScenari (data)
XtPointer data;
{
_UxClistaScenari *Ctx;
Widget w = (Widget)data;
int tipo;

   Ctx = (_UxClistaScenari *)UxGetContext(w);
	tipo = Ctx->Uxtipo;
		  aggListaScenari (w,tipo);
        attiva_timer_listaScenari (w);
        return;
}
/***************************************************************/
int attiva_timer_malfConfig (Widget w)
{
_UxCmalfConfig *Ctx;

   Ctx = (_UxCmalfConfig *)UxGetContext(w);
   Ctx->Uxtimer_malfConfig = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)3000, refresh_malfConfig, w);
   return(0);
}
/***************************************************************/
void refresh_malfConfig (data)
XtPointer data;
{
Widget w = (Widget)data;
_UxCmalfConfig *Ctx;
	
	Ctx = (_UxCmalfConfig *)UxGetContext(w);
	switch (Ctx->Uxtipo)
		{
		case MALFUNZIONI:
		case SOMMARIO_MALF:
			displayDatiMalfConfig (Ctx->UxmalfConfig,
                                 Ctx->UxUxParent,
                                 m_comp_type, malf_set, m_component);
			break;
		case FUNZIONI_REMOTE:
		case SOMMARIO_FREM:
			displayDatiMalfConfig (Ctx->UxmalfConfig,
                                 Ctx->UxUxParent,
                                 fr_comp_type, frem_set, fr_component);
			break;
		}
	
   attiva_timer_malfConfig (w);
   return;
}
/***************************************************************/
int attiva_timer_scenarioMalf (Widget w)
{
_UxCscenarioMalf *Ctx;

	Ctx = (_UxCscenarioMalf *)UxGetContext(w);
   Ctx->Uxtimer_scenarioMalf = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)1500, refresh_scenarioMalf, w);
   return(0);
}
/***************************************************************/
void refresh_initialCondition (data)
XtPointer data;
{
Widget w = (Widget)data;

        update_tasti_initialCondition (w);
        attiva_timer_initialCondition (w);
        return;
}
/***************************************************************/
int attiva_timer_replayOn (w)
Widget w;
{
   timer_replayOn = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)3000, refresh_replay, w);
   return(0);
}
/***************************************************************/
void refresh_replay (data)
XtPointer data;
{
Widget w = (Widget)data;
int n;
int old_n;
int first;
int static stato_sim_old=-1;

	first = 0;
/*
printf("refresh_replay DEBUG: stato_sim = %d\n", stato_sim);
printf("refresh_replay DEBUG: stato_sim_old = %d\n", stato_sim_old);
*/
	if (stato_sim == STATO_BACKTRACK)
		{
		reset_replay_mode (w);
                stato_sim_old = stato_sim;
		return;
		}
	if (reload_lastBtLoad (w,&n,&old_n,first) > 0)
		{
		update_btload (w,n);
		refresh_colors_bt (w, n, old_n);
		}
	attiva_timer_replayOn (w);
        stato_sim_old=stato_sim;
	return;
}
/***************************************************************/
void refresh_initci(data)
XtPointer data;
{
Widget w = (Widget)data;

        update_tasti_initci (w);
        attiva_timer_initSimci (w);
        return;
}
/***************************************************************/
int attiva_timer_initSimbt (w)
Widget w;
{
   timer_initbt = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)300, refresh_initbt, w);
   return(0);
}
/***************************************************************/
void refresh_initbt(data)
XtPointer data;
{
Widget w = (Widget)data;

        update_tasti_initbt (w);
        attiva_timer_initSimbt (w);
        return;
}
/***************************************************************/
void refresh_mainIface(data)
XtPointer data;
{
Widget w = (Widget)data;

		  main_refresh (w);
        attiva_timer_mainIface (w);
        return;
}
/***************************************************************/
void refresh_masterMenu()
{
int stat_ricevute;


   riceve_statistiche(masterMenu,&stat_ricevute); 
   /* dati asincroni da sked */
   update_tasti_masterMenu (masterMenu); /* refresh tasti          */
   update_menu ();                       /* refresh menu           */
   statistiche_refresh (masterMenu,stat_ricevute);  
   /* refresh display        */
   trigger_backtrack (masterMenu);
   checkMaxTime (masterMenu);
			
   attiva_timer_masterMenu (masterMenu);

   if((stato_sim == STATO_REPLAY) && (freeze_rest == 1)) 
      {
/*
printf("refresh_replay DEBUG: sono in REPLAY e crash di una task!!!\n");
*/
      reset_replay_mode (backtrackManagement);
      }

return;
}
/***************************************************************/
int update_tasti_masterMenu (w)
Widget w;
{
static int stato_old = -1;
// static int colore_default;
static int first_update = 1;
_UxCmasterMenu *Ctx;

   Ctx = (_UxCmasterMenu *)UxGetContext(w);

   if (first_update)
      {
// GUAG per il momemnto commentato
      colore_default = read_background_color (w);
      first_update = 0;
      }
/*
printf("update_tasti_masterMenu: stato_sim = %d stato_old = %d\n",stato_sim,stato_old);
*/
   if ((stato_sim == stato_old) && (freeze_rest==0))
      return(0);
   stato_old = stato_sim;

   switch (stato_sim)
      {
      case STATO_FREEZE:
         if(freeze_rest == 0) {

            if(rep_freeze_err == 0) {
               XtVaSetValues (Ctx->UxlabelStatus,
			   RES_CONVERT(XmNlabelString,"FREEZE"),
			   XmNbackground, colore_app[COLORE_FREEZE],NULL);
               XtVaSetValues (Ctx->UxmasterKeyboardStatus,
                           RES_CONVERT(XmNlabelString,"FREEZE"),
                           XmNbackground, colore_app[COLORE_FREEZE],NULL);
            }
            else {
               XtVaSetValues (Ctx->UxlabelStatus,
	       		   RES_CONVERT(XmNlabelString,"FREEZE ERROR"),
			   XmNbackground, colore_app[COLORE_FREEZE],NULL);
               XtVaSetValues (Ctx->UxmasterKeyboardStatus,
                           RES_CONVERT(XmNlabelString,"FREEZE"),
                           XmNbackground, colore_app[COLORE_FREEZE],NULL);
               rep_freeze_err = 0;
            }   
         }
         else {
/*
printf("refresh_masterMenu DEBUG: freeze_rest = %d\n",freeze_rest);
*/
            XtVaSetValues (Ctx->UxlabelStatus,
			RES_CONVERT(XmNlabelString,"FREEZE ERROR"),
			XmNbackground, colore_app[COLORE_FREEZE],NULL);
            XtVaSetValues (Ctx->UxmasterKeyboardStatus,
                        RES_CONVERT(XmNlabelString,"FREEZE"),
                        XmNbackground, colore_app[COLORE_FREEZE],NULL);
         }

         XtVaSetValues (Ctx->Uxinit_clear_menu_pb,XmNsensitive,True,NULL);

         XtVaSetValues (masterMenuRunpb,XmNsensitive,True,NULL);
         XtVaSetValues (Ctx->UxmasterKeyboardRunpb,XmNsensitive,True,NULL);

         XtVaSetValues (masterMenuFreezepb,
			XmNsensitive,True,
			RES_CONVERT (XmNlabelString,STEP_LABEL),
			NULL);
         XtVaSetValues (Ctx->UxmasterKeyboardFreezepb,
               XmNsensitive,True,
               RES_CONVERT (XmNlabelString,STEP_LABEL),
               NULL);

         sblocca_tastiera (w);
      break;
      case STATO_RUN:
/* settaggi ridondanti per il caso di restart del solo banco */
         XtVaSetValues (Ctx->UxlabelStatus,
			RES_CONVERT(XmNlabelString,"RUN"),
			XmNbackground, colore_app[COLORE_RUN],NULL);
         XtVaSetValues (Ctx->UxmasterKeyboardStatus,
                        RES_CONVERT(XmNlabelString,"RUN"),
                        XmNbackground, colore_app[COLORE_RUN],NULL);

         XtVaSetValues (Ctx->Uxinit_clear_menu_pb,XmNsensitive,False,NULL);

         XtVaSetValues (masterMenuRunpb,XmNsensitive,False,NULL);
         XtVaSetValues (Ctx->UxmasterKeyboardRunpb,XmNsensitive,False,NULL);

         XtVaSetValues (masterMenuFreezepb,
			XmNsensitive,True,
			RES_CONVERT (XmNlabelString,FREEZE_LABEL), NULL);
			XtVaSetValues (Ctx->UxmasterKeyboardFreezepb,
			XmNsensitive,True,
                        RES_CONVERT (XmNlabelString,FREEZE_LABEL), NULL);

         sblocca_tastiera (w);
      break;
      case STATO_BACKTRACK:
         if(rep_freeze_err == 0) {
            XtVaSetValues (Ctx->UxlabelStatus,
	        	   RES_CONVERT(XmNlabelString,"BACKTRACK"),
			   XmNbackground, colore_app[COLORE_BACKTRACK],NULL);
         }
         else {
            XtVaSetValues (Ctx->UxlabelStatus,
                        RES_CONVERT(XmNlabelString,"BACKT ERROR"),
                        XmNbackground, colore_app[COLORE_BACKTRACK],NULL);
         }
         XtVaSetValues (Ctx->Uxinit_clear_menu_pb,XmNsensitive,False,NULL);
         XtVaSetValues (masterMenuRunpb,XmNsensitive,False,NULL);
         XtVaSetValues (masterMenuFreezepb,XmNsensitive,False,NULL);

         XtVaSetValues (Ctx->UxmasterKeyboardStatus,
			RES_CONVERT(XmNlabelString,"BACKTRACK"),
                        XmNbackground, colore_app[COLORE_BACKTRACK],NULL);
         XtVaSetValues (Ctx->UxmasterKeyboardRunpb,XmNsensitive,False,NULL);
         XtVaSetValues (Ctx->UxmasterKeyboardFreezepb,XmNsensitive,False,NULL);
      break;
      case STATO_REPLAY:
         XtVaSetValues (Ctx->UxlabelStatus,
			RES_CONVERT(XmNlabelString,"REPLAY"),
			XmNbackground, colore_app[COLORE_REPLAY],NULL);
         XtVaSetValues (Ctx->Uxinit_clear_menu_pb,XmNsensitive,False,NULL);
         XtVaSetValues (masterMenuRunpb,XmNsensitive,False,NULL);
         XtVaSetValues (masterMenuFreezepb,XmNsensitive,False,NULL);

         XtVaSetValues (Ctx->UxmasterKeyboardStatus,
			RES_CONVERT(XmNlabelString,"REPLAY"),
                        XmNbackground, colore_app[COLORE_REPLAY],NULL);
         XtVaSetValues (masterMenuRunpb,XmNsensitive,False,NULL);
         XtVaSetValues (masterMenuFreezepb,XmNsensitive,False,NULL);
      break;
      case STATO_STOP:
         XtVaSetValues (Ctx->UxlabelStatus,
			RES_CONVERT(XmNlabelString,"STOP"),
			XmNbackground, colore_app[COLORE_STOP],NULL);
         XtVaSetValues (Ctx->Uxinit_clear_menu_pb,XmNsensitive,False,NULL);
         XtVaSetValues (masterMenuRunpb,XmNsensitive,False,NULL);
         XtVaSetValues (masterMenuFreezepb,XmNsensitive,False,NULL);

         XtVaSetValues (Ctx->UxlabelStatus,
			RES_CONVERT(XmNlabelString,"STOP"),
                        XmNbackground, colore_app[COLORE_STOP],NULL);
         XtVaSetValues (masterMenuRunpb,XmNsensitive,False,NULL);
         XtVaSetValues (masterMenuFreezepb,XmNsensitive,False,NULL);

         blocca_tastiera (w);
      break;
      case STATO_ERRORE:
         XtVaSetValues (Ctx->UxlabelStatus,
            		RES_CONVERT(XmNlabelString,"ERROR"),
			XmNbackground, colore_app[COLORE_ERROR],NULL);
         XtVaSetValues (Ctx->Uxinit_clear_menu_pb,XmNsensitive,False,NULL);
         XtVaSetValues (masterMenuRunpb,XmNsensitive,False,NULL);
         XtVaSetValues (masterMenuFreezepb,XmNsensitive,False,NULL);

         XtVaSetValues (Ctx->UxlabelStatus,
			RES_CONVERT(XmNlabelString,"ERROR"),
                        XmNbackground, colore_app[COLORE_ERROR],NULL);
         XtVaSetValues (masterMenuRunpb,XmNsensitive,False,NULL);
         XtVaSetValues (masterMenuFreezepb,XmNsensitive,False,NULL);
			simulator_error (w);
			break;
      }
return (0);
}
/***************************************************************/
int blocca_tastiera (w)
Widget w;
{
_UxCmasterMenu *Ctx;

   Ctx = (_UxCmasterMenu *)UxGetContext(w);
	XtVaSetValues (Ctx->UxtastieraButton1,XmNsensitive,False,NULL);
	XtVaSetValues (Ctx->UxtastieraButton2,XmNsensitive,False,NULL);
	XtVaSetValues (Ctx->UxtastieraButton3,XmNsensitive,False,NULL);
	XtVaSetValues (Ctx->UxtastieraButton4,XmNsensitive,False,NULL);
	XtVaSetValues (Ctx->UxtastieraButton5,XmNsensitive,False,NULL);
	XtVaSetValues (Ctx->UxtastieraButton6,XmNsensitive,False,NULL);
	XtVaSetValues (Ctx->UxtastieraButton7,XmNsensitive,False,NULL);
	XtVaSetValues (Ctx->UxtastieraButton8,XmNsensitive,False,NULL);
	XtVaSetValues (Ctx->UxtastieraButton9,XmNsensitive,False,NULL);
	XtVaSetValues (Ctx->UxtastieraButton10,XmNsensitive,False,NULL);

   XtVaSetValues (Ctx->UxmasterKeyboardRunpb,XmNsensitive,False,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardFreezepb,XmNsensitive,False,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardSnappb,XmNsensitive,False,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardBtpb,XmNsensitive,False,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardGrafpb,XmNsensitive,False,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardAingpb,XmNsensitive,False,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardMalfpb,XmNsensitive,False,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardRemFuncpb,XmNsensitive,False,NULL);
	return (0);
}
/***************************************************************/
int sblocca_tastiera (w)
Widget w;
{
_UxCmasterMenu *Ctx;

   Ctx = (_UxCmasterMenu *)UxGetContext(w);
   XtVaSetValues (Ctx->UxtastieraButton1,XmNsensitive,True,NULL);
/*
 * tasto 2 associato al backtrack e gestito in altro modo
 */
/*   XtVaSetValues (Ctx->UxtastieraButton2,XmNsensitive,True,NULL); */
   XtVaSetValues (Ctx->UxtastieraButton3,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxtastieraButton4,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxtastieraButton5,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxtastieraButton6,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxtastieraButton7,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxtastieraButton8,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxtastieraButton9,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxtastieraButton10,XmNsensitive,True,NULL);

	XtVaSetValues (Ctx->UxmasterKeyboardRunpb,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardFreezepb,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardSnappb,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardBtpb,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardGrafpb,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardAingpb,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardMalfpb,XmNsensitive,True,NULL);
   XtVaSetValues (Ctx->UxmasterKeyboardRemFuncpb,XmNsensitive,True,NULL);
   return (0);
}
/***************************************************************/
int update_tasti_initialCondition (w)
Widget w;
{
_UxCinitialCondition *Ctx;
static int stato_old = -1;
static int attivabile = -1;

	Ctx = (_UxCinitialCondition *)UxGetContext(w);

   if ((stato_sim == stato_old) && 
		 (attivabile == tasto_initic_attivabile) &&
		 (!nuova_attivazione))
      return(0);

	nuova_attivazione = 0;
   stato_old = stato_sim;
   attivabile = tasto_initic_attivabile;

   switch (stato_sim)
      {
      case STATO_FREEZE:
         XtVaSetValues (Ctx->UxicInitpb,
               XmNsensitive, True & tasto_initic_attivabile, NULL);
         XtVaSetValues (Ctx->UxIcRcPopupLoad,
               XmNsensitive, True & tasto_initic_attivabile, NULL);
         break;
      case STATO_RUN:
         XtVaSetValues (Ctx->UxicInitpb, XmNsensitive, False, NULL);
         XtVaSetValues (Ctx->UxIcRcPopupLoad, XmNsensitive, False, NULL);
         break;
      case STATO_BACKTRACK:
         XtVaSetValues (Ctx->UxicInitpb, XmNsensitive, False, NULL);
         XtVaSetValues (Ctx->UxIcRcPopupLoad, XmNsensitive, False, NULL);
         break;
      case STATO_REPLAY:
         XtVaSetValues (Ctx->UxicInitpb, XmNsensitive, False, NULL);
         XtVaSetValues (Ctx->UxIcRcPopupLoad, XmNsensitive, False, NULL);
         break;
      case STATO_STOP:
         XtVaSetValues (Ctx->UxicInitpb, XmNsensitive, False, NULL);
         XtVaSetValues (Ctx->UxIcRcPopupLoad, XmNsensitive, False, NULL);
         break;
      case STATO_ERRORE:
         XtVaSetValues (Ctx->UxicInitpb, XmNsensitive, False, NULL);
         XtVaSetValues (Ctx->UxIcRcPopupLoad, XmNsensitive, False, NULL);
         break;
      }
   return (0);
}
/***************************************************************/
int update_tasti_initci (w)
Widget w;
{
static int stato_old = -1;
static int attivabile = -1;

   if ((stato_sim == stato_old) && (attivabile == tasto_loadci_attivabile))
      return(0);
   stato_old = stato_sim;
	attivabile = tasto_loadci_attivabile;

   switch (stato_sim)
      {
      case STATO_FREEZE:
			XtVaSetValues (tasto_load_ci,
					XmNsensitive, True & tasto_loadci_attivabile,
					NULL);
         break;
      case STATO_RUN:
			XtVaSetValues (tasto_load_ci, XmNsensitive, False, NULL);
         break;
      case STATO_BACKTRACK:
			XtVaSetValues (tasto_load_ci, XmNsensitive, False, NULL);
         break;
      case STATO_REPLAY:
			XtVaSetValues (tasto_load_ci, XmNsensitive, False, NULL);
         break;
      case STATO_STOP:
			XtVaSetValues (tasto_load_ci, XmNsensitive, False, NULL);
         break;
      case STATO_ERRORE:
			XtVaSetValues (tasto_load_ci, XmNsensitive, False, NULL);
         break;
      }
   return (0);
}
/***************************************************************/
int update_tasti_initbt (w)
Widget w;
{
static int stato_old = -1;
static int attivabile = -1;

   if ((stato_sim == stato_old) && (attivabile == tasto_loadbt_attivabile))
      return(0);
   stato_old = stato_sim;
	attivabile = tasto_loadbt_attivabile;

   switch (stato_sim)
      {
      case STATO_FREEZE:
         XtVaSetValues (tasto_load_bt,
               XmNsensitive, True & tasto_loadbt_attivabile,
               NULL);
         break;
      case STATO_RUN:
         XtVaSetValues (tasto_load_bt, XmNsensitive, False, NULL);
         break;
      case STATO_BACKTRACK:
         XtVaSetValues (tasto_load_bt, XmNsensitive, False, NULL);
         break;
      case STATO_REPLAY:
         XtVaSetValues (tasto_load_bt, XmNsensitive, False, NULL);
         break;
      case STATO_STOP:
         XtVaSetValues (tasto_load_bt, XmNsensitive, False, NULL);
         break;
      case STATO_ERRORE:
         XtVaSetValues (tasto_load_bt, XmNsensitive, False, NULL);
         break;
      }
   return (0);
}
/********************************************************************/
int main_refresh (w)
Widget w;
{
}
/********************************************************************/
int statistiche_refresh (w,flag)
Widget w;
int flag;          /* = 1 se sono state ricevute le statistiche   */
{
int err;
char stringa[80];
int i;
char *messaggio;
// static int colore_default;
int colore;
static int first_update = 1;
static int counter = 0;
_UxCmasterMenu *Ctx;
long ora, minuti, secondi, giorno, mese, anno;

	Ctx = (_UxCmasterMenu *)UxGetContext(w);

   if (first_update)
      {
// GUAG per il momemnto commentato
      colore_default = read_background_color (w);

      first_update = 0;
      }

	if (flag) 
		{
		if ((stato_sim == STATO_RUN) ||
		 	(stato_sim == STATO_REPLAY) ||
		 	(stato_sim == STATO_STOP))
			{
			if (!counter)
				{
				counter++;
				colore = colore_default;
				}
			else
				{
				counter = 0;
				colore = colore_app[COLORE_BLINK];
				}
/*			XtVaSetValues (Ctx->UxsimRunningFlag,XmNbackground,colore,NULL);   **/
			XtVaSetValues (Ctx->UxlabelStatusFrame,XmNborderColor,colore,NULL);
			XtVaSetValues (Ctx->UxmasterKeyboardStatusFrame,XmNborderColor,colore,NULL);
			}
		}
/*
 * sezione per master menu (sempre presente)
 */
	if (val.flags.stato_sim)
		{
		stato_sim = val.actual.stato_sim;
		val.flags.stato_sim = 0;
		if ((stato_sim != STATO_RUN) && (stato_sim != STATO_REPLAY) &&
			 (stato_sim != STATO_STOP))
			{
			colore = colore_default;
/*			XtVaSetValues (Ctx->UxsimRunningFlag,XmNbackground,colore,NULL);  */
			XtVaSetValues (Ctx->UxlabelStatusFrame,XmNborderColor,colore,NULL);
			XtVaSetValues (Ctx->UxmasterKeyboardStatusFrame,XmNborderColor,colore,NULL);
			}
		}
	if (val.flags.tempo_sim)
		{
		int t_int,t_cent;
/* 
 memorizzala parte intera del tempo
*/
		t_int=val.actual.tempo_sim;
/*
 calcola la frazione in centesimi di secondo
*/
		t_cent=(int)((float)val.actual.tempo_sim*100.0)-t_int*100;
		ora     = 0;
      minuti  = 0;
      secondi = 0;
      giorno  = 1;
      mese    = 1;
      anno    = 2000;
		converti_tempo (val.actual.tempo_sim,
					&ora, &minuti, &secondi, &giorno, &mese, &anno);
		sprintf (stringa,"%2d:%2d:%2d.%d",ora,minuti,secondi,t_cent);
/**
printf ("REFRESH: %f [%s]\n",val.actual.tempo_sim,stringa);
**/
		XtVaSetValues (simTimeDisplay,RES_CONVERT(XmNlabelString,stringa),NULL);
		XtVaSetValues (Ctx->UxmasterKeyboardTime,RES_CONVERT(XmNlabelString,stringa),NULL);
		val.flags.tempo_sim = 0;
		}

	if (val.flags.tempo_ritardo)
      {
      sprintf (stringa,"%8.1f",val.actual.tempo_ritardo);
		XtVaSetValues (delayTimeDisplay,RES_CONVERT(XmNlabelString,stringa),NULL);
      val.flags.tempo_ritardo = 0;
      }

   if (val.flags.timescaling_sim)
      {
		if (val.actual.timescaling_sim > 0.)
      	sprintf (stringa,"%8.1f",val.actual.timescaling_sim);
		else
			strcpy (stringa,"< MAX >");
      XtVaSetValues (speedDisplay,RES_CONVERT(XmNlabelString,stringa),NULL);
      val.flags.timescaling_sim = 0;
      }

	messaggio = (char *)malloc (strlen(ABORT_TASK)+MAX_LUN_NOME_MODELLO+20);
	for (i=0; i<numero_modelli(ind_sh_top); i++)
      {
		if (val.flags.stato_task[i])
			{
			if (val.actual.stato_task[i] == 1)      /* startup   */
				{
				sprintf(messaggio,"%s %s",START_TASK,nome_modello(ind_sh_top,i+1));
				add_message (areaMessaggi,messaggio,LIVELLO_1);
				}
			else if(val.actual.stato_task[i] == 2) /* restart */ 
				{
                                sprintf (messaggio,"%s   %s (%d)",
                                                RESTART_TASK,nome_modello(ind_sh_top,i+1),i+1);
                                add_message (areaMessaggi,messaggio,LIVELLO_1);
                                }
                        else                                   /* abort */
                                {
				sprintf (messaggio,"%s \n %s (%d)",
						ABORT_TASK,nome_modello(ind_sh_top,i+1),i+1);
				attenzione(w,messaggio);
				}
			val.flags.stato_task[i]=0;
			}
		}
	free (messaggio);
	return(0);
}
/********************************************************************/
void stop_timer (t)
XtIntervalId t;
{
     if(t>0) 
	XtRemoveTimeOut(t);
}
/********************************************************************/
/*
 * riceve le statistiche secondo il formato specificato in compilazione
 * e aggiorna la relativa struttura 
 */
int riceve_statistiche (w,flag)
Widget w;
int *flag;
{
int i;
int tipo;
int comando;
int size;
int dati_prelevati;
static int primo_giro = 1;

	dati_prelevati = 0;
	*flag = 0;

/* printf ("test per ricezione statistiche ......\n"); */
	comando=DATI_ASINCRONI;
	from_dispatcher(BANCO,&comando,&tipo,(char*)&statistiche,&size,IPC_NOWAIT);
	if(size>0)
		{
		while(size>0)
   		{
   		comando=DATI_ASINCRONI;
   		from_dispatcher(BANCO,&comando,&tipo,(char*)&statistiche,&size,IPC_NOWAIT);
   		}
		dati_prelevati=1;
		*flag = 1;
		}
	if (dati_prelevati == 1)
		{
/*printf ("statistiche ricevute .......\n");*/ 
		if (primo_giro)
			{
			preset_statistiche();
printf ("PRESET STAT: val.actual.passo_reg_cr = %f\n",val.actual.passo_reg_cr);
			primo_giro = 0;
			}
		else
			check_statistiche ();
		}
	return (0);
}
/********************************************************************/
int preset_statistiche ()
{
int i;
#ifndef OLD_STAT
	val.actual.stato_sim = statistiche.stato_tempo.stato_sim;
	val.flags.stato_sim =1;
	val.actual.tempo_sim = statistiche.stato_tempo.tempo_sim;
	val.flags.tempo_sim =1;
	val.actual.tempo_ritardo = statistiche.stato_tempo.tempo_ritardo;
	val.flags.tempo_ritardo = 1;

	val.actual.timescaling_sim = statistiche.stato_parametri.timescaling_sim;
	val.flags.timescaling_sim = 1;
	val.actual.stepscaling_sim = statistiche.stato_parametri.stepscaling_sim;
	val.flags.stepscaling_sim = 1;
	val.actual.grandezze_mem = statistiche.stato_parametri.grandezze_mem;
	val.flags.grandezze_mem = 1;
	val.actual.passo_reg_cr = statistiche.stato_parametri.passo_reg_cr;
	val.flags.passo_reg_cr = 1;

/* settato localmente
	val.actual.passo_reg_bt = statistiche.stato_parametri.passo_reg_bt;
	val.flags.passo_reg_bt = 1;
*********************************/
	val.actual.passo_pres_bt = statistiche.stato_parametri.passo_pres_bt;
	val.flags.passo_pres_bt = 1;

	for (i=0; i<numero_modelli(ind_sh_top); i++)
		{
		val.actual.tempo[i] = statistiche.tempi_task.tempo[i];
		val.flags.tempo[i] = 1;
		}
	for (i=0; i<MAX_SLAVE; i++)
		{
      val.actual.t_cpu[i] = statistiche.tempi_task.t_cpu[i];
		val.flags.t_cpu[i] = 1;
		}

	for (i=0; i<numero_modelli(ind_sh_top); i++)
		{
      val.actual.stato_task[i] = statistiche.stato_task.stato_task[i];
		val.flags.stato_task[i] = 1;
		}
	for (i=0; i<MAX_SLAVE; i++)
		{
      val.actual.stato_sked[i] = statistiche.stato_task.stato_sked[i];
		val.flags.stato_sked[i] = 1;
		}

	val.actual.operazione_eseguita = 
								statistiche.stato_operazione.operazione_eseguita;
	val.flags.operazione_eseguita = 1;
	val.actual.num_operazione = statistiche.stato_operazione.num_operazione;
	val.flags.num_operazione = 1;

#else  /* vecchia struttura *************************/

	val.actual.stato_sim = statistiche.stato_sked;
   val.flags.stato_sim =1;
/*
printf("PRESET_STATISTICHE DEBUG: val.actual.stato_sim = %d\n",
       val.actual.stato_sim);
*/
	val.actual.tempo_sim = statistiche.tempo_sim;
   val.flags.tempo_sim =1;
   val.actual.tempo_ritardo = statistiche.tempo_ritardo;
   val.flags.tempo_ritardo = 1;

   val.actual.timescaling_sim = statistiche.timescaling_sim;
   val.flags.timescaling_sim = 1;
   val.actual.stepscaling_sim = statistiche.stepscaling_sim;
   val.flags.stepscaling_sim = 1;
   val.actual.grandezze_mem = statistiche.grandezze_mem;
   val.flags.grandezze_mem = 1;
   val.actual.passo_reg_cr = statistiche.passo_reg; /*  per CR  */
   val.flags.passo_reg_cr = 1;
printf ("PRESET: passo_reg presettato a %f\n",val.actual.passo_reg_cr);
/*
PER SICRE
*/
   val.actual.passo_reg_bt = _TIME_BACK_TRACK;
   val.flags.passo_reg_bt = 1;
   val.actual.passo_pres_bt = 99;
   val.flags.passo_pres_bt = 1;

   for (i=0; i<numero_modelli(ind_sh_top); i++)
      {
      val.actual.tempo[i] = statistiche.tempi_task[i];
      val.flags.tempo[i] = 1;
      }

   val.actual.t_cpu[0] = statistiche.tempo_cpu;
   val.flags.t_cpu[0] = 1;

   for (i=0; i<numero_modelli(ind_sh_top); i++)
      {
      if(statistiche.stato_task[i]==2)
         freeze_rest = 1;
      val.actual.stato_task[i] = statistiche.stato_task[i];
      val.flags.stato_task[i] = 1;
      }

   val.actual.operazione_eseguita =
                        statistiche.operazione_eseguita;
   val.flags.operazione_eseguita = 1;
   val.actual.num_operazione = statistiche.num_operazione;
   val.flags.num_operazione = 1;
#endif
	return(0);
}
/********************************************************************/
int check_statistiche ()
{
int i;

#ifndef OLD_STAT
	if (val.actual.stato_sim == statistiche.stato_tempo.stato_sim)
      val.flags.stato_sim  = 0;
   else
      {
      val.actual.stato_sim = statistiche.stato_tempo.stato_sim;
      val.flags.stato_sim =1;
      }

   if (val.actual.tempo_sim == statistiche.stato_tempo.tempo_sim)
		val.flags.tempo_sim  = 0;
	else
		{
		val.actual.tempo_sim = statistiche.stato_tempo.tempo_sim;
   	val.flags.tempo_sim =1;
		}

	if (val.actual.tempo_ritardo == statistiche.stato_tempo.tempo_ritardo)
		val.flags.tempo_ritardo = 0;
	else
		{
   	val.actual.tempo_ritardo = statistiche.stato_tempo.tempo_ritardo;
   	val.flags.tempo_ritardo = 1;
		}
	if (val.actual.timescaling_sim == 
							statistiche.stato_parametri.timescaling_sim)
		val.flags.timescaling_sim = 0;
	else
		{
   	val.actual.timescaling_sim = statistiche.stato_parametri.timescaling_sim;
   	val.flags.timescaling_sim = 1;
		}
	if (val.actual.stepscaling_sim == 
							statistiche.stato_parametri.stepscaling_sim)
		val.flags.stepscaling_sim = 0;
	else
		{
   	val.actual.stepscaling_sim = statistiche.stato_parametri.stepscaling_sim;
   	val.flags.stepscaling_sim = 1;
		}
	if (val.actual.grandezze_mem == statistiche.stato_parametri.grandezze_mem)
		val.flags.grandezze_mem = 0;
	else
		{
   	val.actual.grandezze_mem = statistiche.stato_parametri.grandezze_mem;
   	val.flags.grandezze_mem = 1;
		}
	if (val.actual.passo_reg_cr == statistiche.stato_parametri.passo_reg_cr)
		val.flags.passo_reg_cr = 0;
	else
		{
   	val.actual.passo_reg_cr = statistiche.stato_parametri.passo_reg_cr;
   	val.flags.passo_reg_cr = 1;
		}
	if (val.actual.passo_reg_bt == statistiche.stato_parametri.passo_reg_bt)
		val.flags.passo_reg_bt = 0;
	else
		{
   	val.actual.passo_reg_bt = statistiche.stato_parametri.passo_reg_bt;
   	val.flags.passo_reg_bt = 1;
		}
	if (val.actual.passo_pres_bt == statistiche.stato_parametri.passo_pres_bt)
		val.flags.passo_pres_bt = 0;
	else
		{
   	val.actual.passo_pres_bt = statistiche.stato_parametri.passo_pres_bt;
   	val.flags.passo_pres_bt = 1;
		}

   for (i=0; i<numero_modelli(ind_sh_top); i++)
      {
		if (val.actual.tempo[i] == statistiche.tempi_task.tempo[i])
			val.flags.tempo[i] = 0;
		else
			{
      	val.actual.tempo[i] = statistiche.tempi_task.tempo[i];
      	val.flags.tempo[i] = 1;
			}
      }

   for (i=0; i<MAX_SLAVE; i++)
      {
		if (val.actual.t_cpu[i] == statistiche.tempi_task.t_cpu[i])
			val.flags.t_cpu[i] = 0;
		else
			{
      	val.actual.t_cpu[i] = statistiche.tempi_task.t_cpu[i];
      	val.flags.t_cpu[i] = 1;
			}
      }

   for (i=0; i<numero_modelli(ind_sh_top); i++)
      {


         if (val.actual.stato_task[i] == statistiche.stato_task.stato_task[i])
            val.flags.stato_task[i] = 0;
	 else
            {
      	    val.actual.stato_task[i] = statistiche.stato_task.stato_task[i];
      	    val.flags.stato_task[i] = 1;
         }
      }
   for (i=0; i<MAX_SLAVE; i++)
      {
		if (val.actual.stato_sked[i] == statistiche.stato_task.stato_sked[i])
			val.flags.stato_sked[i] = 0;
		else
			{
      	val.actual.stato_sked[i] = statistiche.stato_task.stato_sked[i];
      	val.flags.stato_sked[i] = 1;
			}
      }

	if (val.actual.operazione_eseguita ==
								statistiche.stato_operazione.operazione_eseguita)
		val.flags.operazione_eseguita = 0;
	else
		{
   	val.actual.operazione_eseguita =
                        statistiche.stato_operazione.operazione_eseguita;
   	val.flags.operazione_eseguita = 1;
		}
	if (val.actual.num_operazione == statistiche.stato_operazione.num_operazione)
		val.flags.num_operazione = 0;
	else
		{
   	val.actual.num_operazione = statistiche.stato_operazione.num_operazione;
   	val.flags.num_operazione = 1;
		}
#else   /* vecchie statistiche          **********/
   freeze_rest = 0;

   if (val.actual.stato_sim == statistiche.stato_sked)
      val.flags.stato_sim  = 0;
   else
      {
      val.actual.stato_sim = statistiche.stato_sked;
      val.flags.stato_sim =1;
      }

   if (val.actual.tempo_sim == statistiche.tempo_sim)
		val.flags.tempo_sim  = 0;
	else
		{
		val.actual.tempo_sim = statistiche.tempo_sim;
   	val.flags.tempo_sim =1;
		}

	if (val.actual.tempo_ritardo == statistiche.tempo_ritardo)
		val.flags.tempo_ritardo = 0;
	else
		{
   	val.actual.tempo_ritardo = statistiche.tempo_ritardo;
   	val.flags.tempo_ritardo = 1;
		}
	if (val.actual.timescaling_sim == 
							statistiche.timescaling_sim)
		val.flags.timescaling_sim = 0;
	else
		{
   	val.actual.timescaling_sim = statistiche.timescaling_sim;
   	val.flags.timescaling_sim = 1;
		}
	if (val.actual.stepscaling_sim == 
							statistiche.stepscaling_sim)
		val.flags.stepscaling_sim = 0;
	else
		{
   	val.actual.stepscaling_sim = statistiche.stepscaling_sim;
   	val.flags.stepscaling_sim = 1;
		}
	if (val.actual.grandezze_mem == statistiche.grandezze_mem)
		val.flags.grandezze_mem = 0;
	else
		{
   	val.actual.grandezze_mem = statistiche.grandezze_mem;
   	val.flags.grandezze_mem = 1;
		}
	if (val.actual.passo_reg_cr == statistiche.passo_reg)
		val.flags.passo_reg_cr = 0;
	else
		{
   	val.actual.passo_reg_cr = statistiche.passo_reg;
   	val.flags.passo_reg_cr = 1;
		}

   for (i=0; i<numero_modelli(ind_sh_top); i++)
      {
		if (val.actual.tempo[i] == statistiche.tempi_task[i])
			val.flags.tempo[i] = 0;
		else
			{
      	val.actual.tempo[i] = statistiche.tempi_task[i];
      	val.flags.tempo[i] = 1;
			}
      }

		if (val.actual.t_cpu[0] == statistiche.tempo_cpu)
			val.flags.t_cpu[0] = 0;
		else
			{
      	val.actual.t_cpu[0] = statistiche.tempo_cpu;
      	val.flags.t_cpu[0] = 1;
			}

   for (i=0; i<numero_modelli(ind_sh_top); i++)
      {
/*
printf("check_statistiche: DEBUG i=%d   val=%d  stato_task=%d\n",
        i,val.actual.stato_task[i],statistiche.stato_task[i]); 
*/
      if (val.actual.stato_task[i] == statistiche.stato_task[i] &&
          (val.actual.stato_task[i] != 2))
         val.flags.stato_task[i] = 0;
      else
         {
/*
printf("check_stat DEBUG: val.actual.stato_task == statistiche.stato_task\n");
printf("check_stat DEBUG: statistiche.stato_sked = %d\n",statistiche.stato_sked);
*/

/*
   Si controlla se almeno una task e' caduta e ripartita. In questo caso ci
   sara' il passaggio in freeze del simulatore con visualizzazione
   "FREEZE ERROR"
*/ 
            if(statistiche.stato_task[i]==2) {
               freeze_rest = 1;
               if(statistiche.stato_sked == STATO_REPLAY)
                  rep_freeze_err = 1;
               }
/*
printf("check_statistiche DEBUG: freeze_rest = %d\n",freeze_rest);
*/
      	    val.actual.stato_task[i] = statistiche.stato_task[i];
      	    val.flags.stato_task[i] = 1;
         }
      }

	if (val.actual.operazione_eseguita ==
								statistiche.operazione_eseguita)
		val.flags.operazione_eseguita = 0;
	else
		{
   	val.actual.operazione_eseguita =
                        statistiche.operazione_eseguita;
   	val.flags.operazione_eseguita = 1;
		}
	if (val.actual.num_operazione == statistiche.num_operazione)
		val.flags.num_operazione = 0;
	else
		{
   	val.actual.num_operazione = statistiche.num_operazione;
   	val.flags.num_operazione = 1;
		}
#endif
   return(0);
}
/****************************************************************/
int trigger_backtrack (w)
Widget w;
{
static int prima_volta = 1;
static float old_tempo_sim;
char app_str[100];

int       ic_num;
char      *messaggio;
SNAP_SKED old_mffr;

if ((prima_volta == 1) && (stato_sim == STATO_FREEZE))
	{
	old_tempo_sim = val.actual.tempo_sim;
	prima_volta = 0;
	intervallo_backtrack = -1.; /* triggera bt al primo passo */
	read_last_backtrack(w);
	return (0);
	}

if ((prima_volta == 1) && (stato_sim == STATO_REPLAY))
	{
	old_tempo_sim = val.actual.tempo_sim;
	prima_volta = 0;
	intervallo_backtrack = -1.; /* triggera bt al primo passo */
	read_last_backtrack(w);

        ic_num = _MAX_SNAP_SHOT;
        messaggio = (char *)malloc (strlen(LOAD_CI)+strlen(OPER_FALLITA) + 20);
        if (SD_loadic (BANCO,&ic_num,(int*)&old_mffr) > 0)
           {
           sprintf (messaggio,"%s %d",LOAD_CI,ic_num);
/*
           inizializzazione = 1;
*/
           nuovoSnapCaricato = 1;   /* memoria caricamento avvenuto */
           nuovoSnapCaricato2 ++;   /* contatore snap caricati      */
           freeze_sim();
        }
        else {
           sprintf (messaggio,"%s %d %s",LOAD_CI,ic_num,OPER_FALLITA);
           attenzione(masterMenu,messaggio);
        }
        add_message (areaMessaggi,messaggio,LIVELLO_1);
        free (messaggio);
	return (0);
	}
/*
printf("trigger_backtrack: prima_volta = %d\n",prima_volta);
printf("trigger_backtrack: intervallo_bkt = %f\n",intervallo_backtrack);
printf("trigger_backtrack: last_backtrack = %d\n",last_backtrack);
*/
if (stato_sim == STATO_RUN)
	{
	if ((intervallo_backtrack < 0.) ||  /* ex. per clear           */
       (intervallo_backtrack -= (val.actual.tempo_sim - old_tempo_sim)) <= 0.)
      {
      printf ("monit triggera backtrack e ricarica %f\n",
								val.actual.passo_reg_bt);
      intervallo_backtrack = val.actual.passo_reg_bt;
      if(prima_volta == 0) {
         ++last_backtrack;
         if(last_backtrack>_MAX_BACK_TRACK)
            last_backtrack=1;
         strcpy (app_str,titolo_sessione);
         esegui_snap_bt (last_backtrack, app_str);
      }
      else {
	 prima_volta = 0;
	 read_last_backtrack(w);
      }
      }
   old_tempo_sim = val.actual.tempo_sim;
	}
return (0);
}
/********************************************************************/
int checkMaxTime (w)
Widget w;
{
	if (tempo_finale_impostato) 	/* se e' stato dato un limite */
		{
		if (val.actual.tempo_sim >= val.actual.max_time)	/* limite raggiunto	*/
			{
			tempo_finale_impostato = 0; 							/* reset della flag	*/
			freeze_sim ();
			}
		}
	return (0);
}
/********************************************************************/
int showProcessList (padre)
Widget padre;
{
int   ex_result, fork_result;
char  *list_par[4];

      if((fork_result=vfork()) != 0) 
			{
         if(fork_result == -1) 
				{
       		fprintf(stderr,"Errore Open Process showProcessList\n");
         	}
         else 
				{
       		fprintf(stderr,"Parent: Waiting for child\n");
       		fprintf(stderr,"Parent: Child process id %d\n",fork_result);
         	}
      	}
      else 
			{
         list_par[0] = "xterm";
         list_par[1] = "-e";
         list_par[2] = "proc_monit";
         list_par[3] = NULL;
         if((ex_result=execvp("xterm", list_par)) == -1) 
				{
            fprintf(stderr,"showProcessList Parent: execvp failed\n");
            return (-1);
         	}
      	}
}
