
/*******************************************************************************
       masterMenu.c
       (Generated from interface file masterMenu.i)
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
#include <Xm/BulletinB.h>
#include <Xm/SeparatoG.h>
#include <Xm/CascadeB.h>
#include <Xm/Frame.h>
#include <Xm/PushB.h>
#include <Xm/Separator.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <X11/Xatom.h>
#include <Xm/Protocols.h>
#include "master_monit.h"
#include "init_sim.h"

#ifndef DESIGN_TIME
#include "interf_attive.h"
#include "parametri.h"
#endif

#include "sked.h"
#include "malfunzioni.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"
extern int stato_sim;
#include "malfunzioni.h"
#include "messaggi.h" 
#include "messages.h"
//#include "filtri.h"
extern MESSAGES mStr[maxIndxMessages];
extern Widget create_selVar();
extern char *create_processStart();
extern swidget create_programLauncher();
extern Widget attiva_statistiche ();
extern swidget create_scenarioMalf();
extern swidget create_resourceSetting();
extern swidget create_archiveSess();
char defaultDisplay[MAX_DISPLAY_NAME];

int richiesta_velocita (Widget);
int simulatorShutdown (Widget);
int switch_to_run(Widget);
int execute_freeze_button (Widget);
int attiva_lista_scenari(Widget,int);
int richiesta_maxtime (Widget);
int richiesta_stepscale (Widget);
int set_max_speed(Widget);
int recording_reset(Widget);
Widget	create_editPert( swidget  );
int showProcessList(Widget);
int switchDatabase();
int connessioni(Widget);
int startup_simulatore(Widget);
int update_titolo_sessione (Widget);
int attiva_timer_masterMenu(Widget);
int free_disk();
int check_data_save_req();
int richiesta_save_arch();



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
	Widget	UxmasterKeyboardForm;
	Widget	UxmasterMenuExpandPopup;
	Widget	UxmasterMenuExpandPopup_p1_title;
	Widget	UxmasterMenuExpandPopup_p1_b2;
	Widget	UxmasterMenuExpandPopup_p1_b7;
	Widget	UxmasterMenuPopupExpandpb;
	Widget	UxmasterMenuExpandPopup_p1_b3;
	Widget	UxmasterMenuExpandPopupSpeed;
	Widget	UxmasterMenuExpandPopupMaxTime;
	Widget	UxmasterMenuExpandPopup_p1_b8;
	Widget	UxmasterMenuExpandPopupShutdown;
	Widget	UxmasterKeyboardTasti;
	Widget	UxmasterKeyboardRunpb;
	Widget	UxmasterKeyboardFreezepb;
	Widget	UxmasterKeyboardSnappb;
	Widget	UxmasterKeyboardBtpb;
	Widget	UxmasterKeyboardGrafpb;
	Widget	UxmasterKeyboardAingpb;
	Widget	UxmasterKeyboardMalfpb;
	Widget	UxmasterKeyboardRemFuncpb;
	Widget	UxmasterKeyboardDati;
	Widget	UxmasterKeyboardStatusFrame;
	Widget	UxmasterKeyboardStatus;
	Widget	UxmasterKeyboardTime;
	Widget	UxexpandedMainForm;
	Widget	UxcommandMainForm;
	Widget	Uxmaster_menu;
	Widget	UxFileMenuPb;
	Widget	UxFileMenuInitIcpb;
	Widget	UxFileMenuInitBtpb;
	Widget	Uxinit_ci_menu_pb;
	Widget	Uxinit_bt_menu_pb;
	Widget	Uxinit_menu_pb_b5;
	Widget	Uxinit_menu_pb_b9;
	Widget	Uxinit_speed_menu_pb;
	Widget	Uxinit_maxspeed_menu_pb;
	Widget	Uxinit_menu_pb_b6;
	Widget	Uxinit_MaxTimepb;
	Widget	Uxinit_menu_pb_b10;
	Widget	Uxinit_clear_menu_pb;
	Widget	UxFileMenuPb_b11;
	Widget	UxFileMenuShutdownpb;
	Widget	Uxmaster_menu_init;
	Widget	Uxrecording_menu_pb;
	Widget	UxcontRecPane;
	Widget	UxcontRecMenuSelpb;
	Widget	Uxmaster_menu_p12_b2;
	Widget	UxcontRecMenuStartpb;
	Widget	Uxrecording_cr_menu_cb;
	Widget	Uxrecording_ITAL_menu_pb;
	Widget	Uxrecording_perform_menu_pb;
	Widget	Uxrecording_sep1_menu;
	Widget	Uxrecording_menu_archivio_pane;
	Widget	Uxrec_menu_arch_save_pb;
	Widget	Uxrecording_menu_archivio_pane_b2;
	Widget	Uxrec_menu_arch_edit_pb;
	Widget	Uxrecording_archivio_menu_cb;
	Widget	Uxmaster_menu_recording;
	Widget	Uxmffr_menu_pb;
	Widget	Uxmalf_pane;
	Widget	Uxmffr_malf_scenpb;
	Widget	Uxmalf_pane_b3;
	Widget	Uxmffr_malf_somm_pb;
	Widget	Uxmffr_menu_pb_b1;
	Widget	Uxmffr_menu_pb_b3;
	Widget	Uxfrem_pane;
	Widget	Uxmffr_frem_scenpb;
	Widget	Uxmaster_menu_p11_b2;
	Widget	Uxmffr_frem_sommpb;
	Widget	Uxmffr_menu_pb_b2;
	Widget	Uxmaster_menu_mffr;
	Widget	Uxshow_menu_pb;
	Widget	Uxshow_statistic_menu_pb;
	Widget	Uxshow_pert_pb;
	Widget	Uxshow_proc_list;
	Widget	Uxshow_param_menu_pb;
	Widget	Uxmaster_menu_show;
	Widget	Uxhelp_menu_pb;
	Widget	Uxhelp_1_menu_pb;
	Widget	Uxprog_menu_pb;
	Widget	UxmasterMenuAingpb;
	Widget	UxmasterMenuUserProgpb;
	Widget	Uxmaster_menu_top_b1;
	Widget	Uxoption_menu_pb;
	Widget	Uxoption_load_menu_pb;
	Widget	Uxoption_edit_menu_pb;
	Widget	Uxmaster_menu_options;
	Widget	Uxarchive_menu_pb;
	Widget	Uxarchive_session;
	Widget	Uxmaster_menu_top_b2;
	Widget	Uxform2;
	Widget	UxlabelStatusHead;
	Widget	UxlabelStatusFrame;
	Widget	UxlabelStatus;
	Widget	UxmasterMenuData;
	Widget	UxrowColumn1;
	Widget	UxsimTimelabel;
	Widget	UxdelayTimelabel1;
	Widget	UxdelayTimelabel2;
	Widget	UxtastieraMaster;
	Widget	UxtastieraButton5;
	Widget	UxtastieraButton4;
	Widget	UxtastieraButton3;
	Widget	UxtastieraButton2;
	Widget	UxtastieraButton1;
	Widget	UxtastieraButton10;
	Widget	UxtastieraButton9;
	Widget	UxtastieraButton8;
	Widget	UxtastieraButton7;
	Widget	UxtastieraButton6;
	Widget	UxmasterMenuCompressPopup;
	Widget	UxmasterMenuCompresspb;
	Widget	UxmessageForm;
	Widget	UxscrolledWindow1;
	Widget	Uxmenu2_p1_title;
	Widget	UxareaMessaggiPopupMenu_b2;
	Widget	UxareaMessaggiPopupClearpb;
	Widget	UxmessageLabel;
	Widget	Uxseparator2;
	Widget	UxselVarAing;
	Widget	UxstatisticheDisplayTask;
} _UxCmasterMenu;

static _UxCmasterMenu          *UxMasterMenuContext;
#define masterKeyboardForm      UxMasterMenuContext->UxmasterKeyboardForm
#define masterMenuExpandPopup   UxMasterMenuContext->UxmasterMenuExpandPopup
#define masterMenuExpandPopup_p1_title UxMasterMenuContext->UxmasterMenuExpandPopup_p1_title
#define masterMenuExpandPopup_p1_b2 UxMasterMenuContext->UxmasterMenuExpandPopup_p1_b2
#define masterMenuExpandPopup_p1_b7 UxMasterMenuContext->UxmasterMenuExpandPopup_p1_b7
#define masterMenuPopupExpandpb UxMasterMenuContext->UxmasterMenuPopupExpandpb
#define masterMenuExpandPopup_p1_b3 UxMasterMenuContext->UxmasterMenuExpandPopup_p1_b3
#define masterMenuExpandPopupSpeed UxMasterMenuContext->UxmasterMenuExpandPopupSpeed
#define masterMenuExpandPopupMaxTime UxMasterMenuContext->UxmasterMenuExpandPopupMaxTime
#define masterMenuExpandPopup_p1_b8 UxMasterMenuContext->UxmasterMenuExpandPopup_p1_b8
#define masterMenuExpandPopupShutdown UxMasterMenuContext->UxmasterMenuExpandPopupShutdown
#define masterKeyboardTasti     UxMasterMenuContext->UxmasterKeyboardTasti
#define masterKeyboardRunpb     UxMasterMenuContext->UxmasterKeyboardRunpb
#define masterKeyboardFreezepb  UxMasterMenuContext->UxmasterKeyboardFreezepb
#define masterKeyboardSnappb    UxMasterMenuContext->UxmasterKeyboardSnappb
#define masterKeyboardBtpb      UxMasterMenuContext->UxmasterKeyboardBtpb
#define masterKeyboardGrafpb    UxMasterMenuContext->UxmasterKeyboardGrafpb
#define masterKeyboardAingpb    UxMasterMenuContext->UxmasterKeyboardAingpb
#define masterKeyboardMalfpb    UxMasterMenuContext->UxmasterKeyboardMalfpb
#define masterKeyboardRemFuncpb UxMasterMenuContext->UxmasterKeyboardRemFuncpb
#define masterKeyboardDati      UxMasterMenuContext->UxmasterKeyboardDati
#define masterKeyboardStatusFrame UxMasterMenuContext->UxmasterKeyboardStatusFrame
#define masterKeyboardStatus    UxMasterMenuContext->UxmasterKeyboardStatus
#define masterKeyboardTime      UxMasterMenuContext->UxmasterKeyboardTime
#define expandedMainForm        UxMasterMenuContext->UxexpandedMainForm
#define commandMainForm         UxMasterMenuContext->UxcommandMainForm
#define master_menu             UxMasterMenuContext->Uxmaster_menu
#define FileMenuPb              UxMasterMenuContext->UxFileMenuPb
#define FileMenuInitIcpb        UxMasterMenuContext->UxFileMenuInitIcpb
#define FileMenuInitBtpb        UxMasterMenuContext->UxFileMenuInitBtpb
#define init_ci_menu_pb         UxMasterMenuContext->Uxinit_ci_menu_pb
#define init_bt_menu_pb         UxMasterMenuContext->Uxinit_bt_menu_pb
#define init_menu_pb_b5         UxMasterMenuContext->Uxinit_menu_pb_b5
#define init_menu_pb_b9         UxMasterMenuContext->Uxinit_menu_pb_b9
#define init_speed_menu_pb      UxMasterMenuContext->Uxinit_speed_menu_pb
#define init_maxspeed_menu_pb   UxMasterMenuContext->Uxinit_maxspeed_menu_pb
#define init_menu_pb_b6         UxMasterMenuContext->Uxinit_menu_pb_b6
#define init_MaxTimepb          UxMasterMenuContext->Uxinit_MaxTimepb
#define init_menu_pb_b10        UxMasterMenuContext->Uxinit_menu_pb_b10
#define init_clear_menu_pb      UxMasterMenuContext->Uxinit_clear_menu_pb
#define FileMenuPb_b11          UxMasterMenuContext->UxFileMenuPb_b11
#define FileMenuShutdownpb      UxMasterMenuContext->UxFileMenuShutdownpb
#define master_menu_init        UxMasterMenuContext->Uxmaster_menu_init
#define recording_menu_pb       UxMasterMenuContext->Uxrecording_menu_pb
#define contRecPane             UxMasterMenuContext->UxcontRecPane
#define contRecMenuSelpb        UxMasterMenuContext->UxcontRecMenuSelpb
#define master_menu_p12_b2      UxMasterMenuContext->Uxmaster_menu_p12_b2
#define contRecMenuStartpb      UxMasterMenuContext->UxcontRecMenuStartpb
#define recording_cr_menu_cb    UxMasterMenuContext->Uxrecording_cr_menu_cb
#define recording_ITAL_menu_pb  UxMasterMenuContext->Uxrecording_ITAL_menu_pb
#define recording_perform_menu_pb UxMasterMenuContext->Uxrecording_perform_menu_pb
#define recording_sep1_menu     UxMasterMenuContext->Uxrecording_sep1_menu
#define recording_menu_archivio_pane UxMasterMenuContext->Uxrecording_menu_archivio_pane
#define rec_menu_arch_save_pb   UxMasterMenuContext->Uxrec_menu_arch_save_pb
#define recording_menu_archivio_pane_b2 UxMasterMenuContext->Uxrecording_menu_archivio_pane_b2
#define rec_menu_arch_edit_pb   UxMasterMenuContext->Uxrec_menu_arch_edit_pb
#define recording_archivio_menu_cb UxMasterMenuContext->Uxrecording_archivio_menu_cb
#define master_menu_recording   UxMasterMenuContext->Uxmaster_menu_recording
#define mffr_menu_pb            UxMasterMenuContext->Uxmffr_menu_pb
#define malf_pane               UxMasterMenuContext->Uxmalf_pane
#define mffr_malf_scenpb        UxMasterMenuContext->Uxmffr_malf_scenpb
#define malf_pane_b3            UxMasterMenuContext->Uxmalf_pane_b3
#define mffr_malf_somm_pb       UxMasterMenuContext->Uxmffr_malf_somm_pb
#define mffr_menu_pb_b1         UxMasterMenuContext->Uxmffr_menu_pb_b1
#define mffr_menu_pb_b3         UxMasterMenuContext->Uxmffr_menu_pb_b3
#define frem_pane               UxMasterMenuContext->Uxfrem_pane
#define mffr_frem_scenpb        UxMasterMenuContext->Uxmffr_frem_scenpb
#define master_menu_p11_b2      UxMasterMenuContext->Uxmaster_menu_p11_b2
#define mffr_frem_sommpb        UxMasterMenuContext->Uxmffr_frem_sommpb
#define mffr_menu_pb_b2         UxMasterMenuContext->Uxmffr_menu_pb_b2
#define master_menu_mffr        UxMasterMenuContext->Uxmaster_menu_mffr
#define show_menu_pb            UxMasterMenuContext->Uxshow_menu_pb
#define show_statistic_menu_pb  UxMasterMenuContext->Uxshow_statistic_menu_pb
#define show_pert_pb            UxMasterMenuContext->Uxshow_pert_pb
#define show_proc_list          UxMasterMenuContext->Uxshow_proc_list
#define show_param_menu_pb      UxMasterMenuContext->Uxshow_param_menu_pb
#define master_menu_show        UxMasterMenuContext->Uxmaster_menu_show
#define help_menu_pb            UxMasterMenuContext->Uxhelp_menu_pb
#define help_1_menu_pb          UxMasterMenuContext->Uxhelp_1_menu_pb
#define prog_menu_pb            UxMasterMenuContext->Uxprog_menu_pb
#define masterMenuAingpb        UxMasterMenuContext->UxmasterMenuAingpb
#define masterMenuUserProgpb    UxMasterMenuContext->UxmasterMenuUserProgpb
#define master_menu_top_b1      UxMasterMenuContext->Uxmaster_menu_top_b1
#define option_menu_pb          UxMasterMenuContext->Uxoption_menu_pb
#define option_load_menu_pb     UxMasterMenuContext->Uxoption_load_menu_pb
#define option_edit_menu_pb     UxMasterMenuContext->Uxoption_edit_menu_pb
#define master_menu_options     UxMasterMenuContext->Uxmaster_menu_options
#define archive_menu_pb         UxMasterMenuContext->Uxarchive_menu_pb
#define archive_session         UxMasterMenuContext->Uxarchive_session
#define master_menu_top_b2      UxMasterMenuContext->Uxmaster_menu_top_b2
#define form2                   UxMasterMenuContext->Uxform2
#define labelStatusHead         UxMasterMenuContext->UxlabelStatusHead
#define labelStatusFrame        UxMasterMenuContext->UxlabelStatusFrame
#define labelStatus             UxMasterMenuContext->UxlabelStatus
#define masterMenuData          UxMasterMenuContext->UxmasterMenuData
#define rowColumn1              UxMasterMenuContext->UxrowColumn1
#define simTimelabel            UxMasterMenuContext->UxsimTimelabel
#define delayTimelabel1         UxMasterMenuContext->UxdelayTimelabel1
#define delayTimelabel2         UxMasterMenuContext->UxdelayTimelabel2
#define tastieraMaster          UxMasterMenuContext->UxtastieraMaster
#define tastieraButton5         UxMasterMenuContext->UxtastieraButton5
#define tastieraButton4         UxMasterMenuContext->UxtastieraButton4
#define tastieraButton3         UxMasterMenuContext->UxtastieraButton3
#define tastieraButton2         UxMasterMenuContext->UxtastieraButton2
#define tastieraButton1         UxMasterMenuContext->UxtastieraButton1
#define tastieraButton10        UxMasterMenuContext->UxtastieraButton10
#define tastieraButton9         UxMasterMenuContext->UxtastieraButton9
#define tastieraButton8         UxMasterMenuContext->UxtastieraButton8
#define tastieraButton7         UxMasterMenuContext->UxtastieraButton7
#define tastieraButton6         UxMasterMenuContext->UxtastieraButton6
#define masterMenuCompressPopup UxMasterMenuContext->UxmasterMenuCompressPopup
#define masterMenuCompresspb    UxMasterMenuContext->UxmasterMenuCompresspb
#define messageForm             UxMasterMenuContext->UxmessageForm
#define scrolledWindow1         UxMasterMenuContext->UxscrolledWindow1
#define menu2_p1_title          UxMasterMenuContext->Uxmenu2_p1_title
#define areaMessaggiPopupMenu_b2 UxMasterMenuContext->UxareaMessaggiPopupMenu_b2
#define areaMessaggiPopupClearpb UxMasterMenuContext->UxareaMessaggiPopupClearpb
#define messageLabel            UxMasterMenuContext->UxmessageLabel
#define separator2              UxMasterMenuContext->Uxseparator2
#define selVarAing              UxMasterMenuContext->UxselVarAing
#define statisticheDisplayTask  UxMasterMenuContext->UxstatisticheDisplayTask


/*******************************************************************************
       The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxmasterMenuMenuPost( wgt, client_data, event, ctd )
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

Widget	masterMenu;
Widget	master_menu_help;
Widget	masterMenuFreezepb;
Widget	masterMenuRunpb;
Widget	simTimeDisplay;
Widget	delayTimeDisplay;
Widget	speedDisplay;
Widget	areaMessaggi;
Widget	areaMessaggiPopupMenu;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_masterMenu();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
int aggiorna_opzioni_init (w, p)
Widget w;
OPTIONS_FLAGS *p;
{
   if (! p->options_init.clear)
      XtUnmanageChild (init_clear_menu_pb);
   else
      XtManageChild (init_clear_menu_pb);

   if (!p->options_init.from_bt)
      XtUnmanageChild (init_bt_menu_pb); 
   else
     XtManageChild (init_bt_menu_pb);

   if (!p->options_init.from_ci)  
     XtUnmanageChild (init_ci_menu_pb);
   else
     XtManageChild  (init_ci_menu_pb);
}
#endif
/****************************************************/
#ifndef DESIGN_TIME
int aggiorna_opzioni_recording (w, p)
Widget w;
OPTIONS_FLAGS *p;
{
   if (p->options_recording.active)
      {
      XtManageChild (master_menu_recording);
      if (!p->options_recording.backtrack)
         XtUnmanageChild (FileMenuInitBtpb);
      else
         XtManageChild (FileMenuInitBtpb);

      if (! p->options_recording.cr)
         XtUnmanageChild (recording_cr_menu_cb);
      else
         XtManageChild (recording_cr_menu_cb);

      if (!p->options_recording.ital)
         XtUnmanageChild (recording_ITAL_menu_pb);
      else
         XtManageChild (recording_ITAL_menu_pb);

      if (!p->options_recording.performance)
         XtUnmanageChild (recording_perform_menu_pb);
      else
         XtManageChild (recording_perform_menu_pb);

      if (!p->options_recording.archive)
         {
         XtUnmanageChild (recording_sep1_menu);
         XtUnmanageChild (recording_archivio_menu_cb);
         }
      else
         {
         XtManageChild (recording_sep1_menu);
         XtManageChild (recording_archivio_menu_cb);
         }
      }
   else
      XtUnmanageChild (master_menu_recording);
}
#endif
/****************************************************/
#ifndef DESIGN_TIME
int aggiorna_opzioni_snap (w, p)
Widget w;
OPTIONS_FLAGS *p;
{
   if (!p->options_snapshot.active)
      XtUnmanageChild (FileMenuInitIcpb);
   else
      XtManageChild (FileMenuInitIcpb);
}
#endif
/****************************************************/
#ifndef DESIGN_TIME
int aggiorna_opzioni_mffr (w, p)
Widget w;
OPTIONS_FLAGS *p;
{
   if (!p->options_mffr.active)
      XtUnmanageChild (master_menu_mffr);
   else
      XtManageChild (master_menu_mffr);
}
#endif
/****************************************************/
#ifndef DESIGN_TIME
int aggiorna_opzioni_show (w, p)
Widget w;
OPTIONS_FLAGS *p;
{
   if (!p->options_show.active)
     XtUnmanageChild (master_menu_show);
   else
     XtManageChild (master_menu_show);
}
#endif
/****************************************************/
int check_opzioni()
{
#ifndef DESIGN_TIME
/* 
 * init
 */
   if (! options.options_init.clear)
      XtUnmanageChild (init_clear_menu_pb);
   if (!options.options_init.from_bt)
      XtUnmanageChild (init_bt_menu_pb);
   if (!options.options_init.from_ci)
      XtUnmanageChild (init_ci_menu_pb);

/*
 * snapshot
 */
   if (!options.options_snapshot.active)
      XtUnmanageChild (FileMenuInitIcpb);
   
/*
 *recording
 */
   if (options.options_recording.active)
      {
      if (!options.options_recording.backtrack)
         XtUnmanageChild (FileMenuInitBtpb);
      if (! options.options_recording.cr)
         XtUnmanageChild (recording_cr_menu_cb);
      if (!options.options_recording.ital)
         XtUnmanageChild (recording_ITAL_menu_pb);
      if (!options.options_recording.performance)
         XtUnmanageChild (recording_perform_menu_pb);
      if (!options.options_recording.archive)
         {
         XtUnmanageChild (recording_sep1_menu);
         XtUnmanageChild (recording_archivio_menu_cb);
         }
      }
   else
      XtUnmanageChild (master_menu_recording);
/*
 * mffr
 */
   if (!options.options_mffr.active)
      XtUnmanageChild (master_menu_mffr);

/*
 * show
 */
   if (!options.options_show.active)
     XtUnmanageChild (master_menu_show); 



   return(0);
#endif
}
/**********************************************************/
int update_menu ()
{
static int flag = 0;
extern int last_backtrack;

/*
printf("update_menu: last_backtrack = %d\n", last_backtrack);
*/
   if (last_backtrack <= 0)   /* nessun backtrack registrato   */
      {
/*
printf("update_menu: last_backtrack = %d  last_backtrack <= 0\n",last_backtrack);
*/
      flag = 1;
      XtVaSetValues (init_bt_menu_pb, XmNsensitive, False, NULL);
      XtVaSetValues (FileMenuInitBtpb, XmNsensitive, False, NULL);   
/* tasto rapido backtrack */
      XtVaSetValues (tastieraButton2, XmNsensitive, False, NULL);  
      XtVaSetValues (masterKeyboardBtpb, XmNsensitive, False, NULL);
      }
   else if (flag && (last_backtrack > 0))
      {
/*
printf("update_menu: last_backtrack = %d last_backtrack > 0\n",last_backtrack);
*/
      flag = 0;
      XtVaSetValues (init_bt_menu_pb, XmNsensitive, True, NULL);
      XtVaSetValues (FileMenuInitBtpb, XmNsensitive, True, NULL);    
/* tasto rapido backtrack */
      XtVaSetValues (tastieraButton2, XmNsensitive, True, NULL); 
      XtVaSetValues (masterKeyboardBtpb, XmNsensitive, True, NULL);
      }
   
   return (0);
}
/******************************************/
int compressInterface (Widget wgt)
{
XtManageChild (masterKeyboardForm);
XtUnmanageChild (expandedMainForm);
}
/******************************************/
int expandInterface (Widget wgt)
{
XtManageChild (expandedMainForm);
XtUnmanageChild (masterKeyboardForm);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void  destroyCB_masterMenu( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterMenuPopupExpandpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	expandInterface (masterMenu);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterMenuExpandPopupSpeed( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	richiesta_velocita (UxGetWidget(masterMenu));
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterMenuExpandPopupMaxTime( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	richiesta_maxtime (UxGetWidget(masterMenu));
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterMenuExpandPopupShutdown( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	simulatorShutdown (masterMenu);
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterKeyboardRunpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	switch_to_run(masterMenu);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterKeyboardFreezepb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	execute_freeze_button ((Widget)UxClientData);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterKeyboardSnappb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	printf ("richiesta snapshot\n");
#ifndef DESIGN_TIME
	if (snap_interface_active) 
	   { 
	   if (!exist_Widget(UxGetWidget(initialCondition)))
	      create_initialCondition(UxGetWidget(masterMenu));
	   }
	else
	   create_initialCondition(UxGetWidget(masterMenu)); 
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterKeyboardBtpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	printf ("registrazioni di backtrack\n");
#ifndef DESIGN_TIME
	   if (bt_interface_active)
	      {
	      if (!exist_Widget(UxGetWidget(backtrackManagement)))
	         create_backtrackManagement(UxGetWidget(masterMenu));
	      }
	   else
	      create_backtrackManagement(UxGetWidget(masterMenu));
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterKeyboardGrafpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	strcpy (defaultDisplay,create_processStart(masterMenu,"GRAPHICS"));
	if (!strcmp(defaultDisplay,"NO"))
	   return;
	attiva_graphics(UxGetWidget(masterMenu),defaultDisplay);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterKeyboardAingpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	if (!exist_Widget (selVarAing))
	   selVarAing = create_selVar (masterMenu, NULL,AING_CALLING,AING_SELECTION);
	printf (" chiamata da aing [%s]\n",AING_SELECTION);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterKeyboardMalfpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	attiva_lista_scenari(masterMenu,MALFUNZIONI);
	attiva_sommario (masterMenu,SOMMARIO_MALF);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterKeyboardRemFuncpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	attiva_lista_scenari(masterMenu,FUNZIONI_REMOTE);
	attiva_sommario (masterMenu,SOMMARIO_FREM);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_FileMenuInitIcpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	if (snap_interface_active) 
	   { 
	   if (!exist_Widget(UxGetWidget(initialCondition)))
	      create_initialCondition(UxGetWidget(masterMenu));
	   }
	else
	   create_initialCondition(UxGetWidget(masterMenu)); 
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_FileMenuInitBtpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   if (bt_interface_active)
	      {
	      if (!exist_Widget(UxGetWidget(backtrackManagement)))
	         create_backtrackManagement(UxGetWidget(masterMenu));
	      }
	   else
	      create_backtrackManagement(UxGetWidget(masterMenu));
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_init_ci_menu_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	printf ("init da ci **********\n");
#ifndef DESIGN_TIME
	if (init_ci_interface_active) 
	   { 
	   if (!exist_Widget(UxGetWidget(initSimCi)))
	   create_initSimulator (UxGetWidget(masterMenu), INIT_FROM_CI);
	   }
	else
	   create_initSimulator (UxGetWidget(masterMenu), INIT_FROM_CI);
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_init_bt_menu_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	printf ("init da backtrack ********\n");
#ifndef DESIGN_TIME
	if (init_bt_interface_active) 
	   { 
	   if (!exist_Widget(UxGetWidget(initSimBt)))
	   create_initSimulator (UxGetWidget(masterMenu), INIT_FROM_BT);
	   }
	else
	   create_initSimulator (UxGetWidget(masterMenu), INIT_FROM_BT);
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_init_menu_pb_b9( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	richiesta_stepscale (UxGetWidget(masterMenu));
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_init_speed_menu_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	richiesta_velocita (UxGetWidget(masterMenu));
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_init_maxspeed_menu_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	set_max_speed (UxGetWidget(masterMenu));
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_init_MaxTimepb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	richiesta_maxtime (UxGetWidget(masterMenu));
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_init_clear_menu_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	recording_reset (UxGetWidget(masterMenu));
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_FileMenuShutdownpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	simulatorShutdown (masterMenu);
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_contRecMenuSelpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	printf ("richiesta selezione cr\n");
#ifndef DESIGN_TIME
	if (sel_var_cr_interface_active) 
	   { 
	   if (!exist_Widget (UxGetWidget(selVarCr)))
	      create_selVarCr (UxGetWidget(masterMenu));
	   }
	else
	   create_selVarCr (UxGetWidget(masterMenu)); 
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_contRecMenuStartpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	strcpy (defaultDisplay,create_processStart(masterMenu,"GRAPHICS"));
	if (!strcmp(defaultDisplay,"NO"))
	   return;
	attiva_graphics(UxGetWidget(masterMenu),defaultDisplay);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_recording_ITAL_menu_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{printf ("I&T al\n");}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_recording_perform_menu_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{printf ("perf. analisys\n");}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_rec_menu_arch_save_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	printf ("save archivio\n");
#ifndef DESIGN_TIME
	if (archsave_interface_active)
	   {
	   if (!exist_Widget(UxGetWidget(archiveSavereq)))
	      create_archiveSavereq();
	   }
	else
	   create_archiveSavereq();
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_rec_menu_arch_edit_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	printf ("editazione archivio\n");
	create_archiveEdit();
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_mffr_malf_scenpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	attiva_lista_scenari(masterMenu,MALFUNZIONI);
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_mffr_malf_somm_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	attiva_sommario (masterMenu,SOMMARIO_MALF);
	
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_mffr_frem_scenpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	attiva_lista_scenari(masterMenu,FUNZIONI_REMOTE);
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_mffr_frem_sommpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	attiva_sommario (masterMenu, SOMMARIO_FREM);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_show_statistic_menu_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	if (!exist_Widget (statisticheDisplayTask))
	   statisticheDisplayTask = attiva_statistiche (UxGetWidget(masterMenu));
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_show_pert_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	if (edit_pert_interface_active)
	   {
	   if (!exist_Widget(UxGetWidget(editPert)))
	      create_editPert (UxGetWidget(masterMenu));
	   }
	else
	   create_editPert (UxGetWidget(masterMenu));
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_show_proc_list( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	showProcessList (masterMenu);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_show_param_menu_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{printf ("richiesta show parametri\n");}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterMenuAingpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	if (!exist_Widget (selVarAing))
	   selVarAing = create_selVar (masterMenu, NULL,AING_CALLING,AING_SELECTION);
	printf (" chiamata da aing [%s]\n",AING_SELECTION);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterMenuUserProgpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	printf ("programmi utente\n");
	create_programLauncher (masterMenu);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_option_load_menu_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME 
	printf ("load Options\n");
	read_options();
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_option_edit_menu_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	printf ("edit opzioni\n");
	if (opt_interface_active)
	   {
	   if (!exist_Widget(UxGetWidget(optionSet)))
	      create_optionSet();
	   }
	else
	   create_optionSet();
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_archive_session( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	printf("Archiving session!!!\n");
	create_archiveSess(masterMenu);
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterMenuFreezepb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	execute_freeze_button ((Widget)UxClientData);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterMenuRunpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	switch_to_run(masterMenu);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_tastieraButton5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	if(attiva_lista_scenari(masterMenu,MALFUNZIONI) >= 0)
	   attiva_sommario (masterMenu,SOMMARIO_MALF);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_tastieraButton4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	if (!exist_Widget (selVarAing))
	   selVarAing = create_selVar (masterMenu, NULL,AING_CALLING,AING_SELECTION);
	printf (" chiamata da aing [%s]\n",AING_SELECTION);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_tastieraButton3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	strcpy (defaultDisplay,create_processStart(masterMenu,"GRAPHICS"));
	if (!strcmp(defaultDisplay,"NO"))
	   return;
	attiva_graphics(UxGetWidget(masterMenu),defaultDisplay);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_tastieraButton2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	printf ("registrazioni di backtrack\n");
#ifndef DESIGN_TIME
	   if (bt_interface_active)
	      {
	      if (!exist_Widget(UxGetWidget(backtrackManagement)))
	         create_backtrackManagement(UxGetWidget(masterMenu));
	      }
	   else
	      create_backtrackManagement(UxGetWidget(masterMenu));
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_tastieraButton1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	printf ("richiesta snapshot\n");
#ifndef DESIGN_TIME
	if (snap_interface_active) 
	   { 
	   if (!exist_Widget(UxGetWidget(initialCondition)))
	      create_initialCondition(UxGetWidget(masterMenu));
	   }
	else
	   create_initialCondition(UxGetWidget(masterMenu)); 
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_tastieraButton10( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	attiva_lista_scenari(masterMenu,FUNZIONI_REMOTE);
	attiva_sommario (masterMenu,SOMMARIO_FREM);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_tastieraButton9( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	create_resourceSetting (masterMenu);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_tastieraButton6( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	switchDatabase();
	
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_masterMenuCompresspb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
	compressInterface (masterMenu);
	}
	UxMasterMenuContext = UxSaveCtx;
}

static void  activateCB_areaMessaggiPopupClearpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmasterMenu          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMasterMenuContext;
	UxMasterMenuContext = UxContext =
			(_UxCmasterMenu *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	clear_message_list (areaMessaggi);
#endif
	}
	UxMasterMenuContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_masterMenu()
{
	Widget		_UxParent;
	Widget		masterMenuExpandPopup_shell;
	Widget		FileMenuPb_shell;
	Widget		recording_menu_pb_shell;
	Widget		contRecPane_shell;
	Widget		recording_menu_archivio_pane_shell;
	Widget		mffr_menu_pb_shell;
	Widget		malf_pane_shell;
	Widget		frem_pane_shell;
	Widget		show_menu_pb_shell;
	Widget		help_menu_pb_shell;
	Widget		prog_menu_pb_shell;
	Widget		option_menu_pb_shell;
	Widget		archive_menu_pb_shell;
	Widget		masterMenuCompressPopup_shell;
	Widget		areaMessaggiPopupMenu_shell;


	/* Creation of masterMenu */
	_UxParent = UxWidgetToSwidget (UxTopLevel);
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "masterMenu_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 510,
			XmNy, 0,
			XmNwidth, 700,
			XmNheight, 150,
			XmNallowShellResize, TRUE,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "masterMenu",
			NULL );

	masterMenu = XtVaCreateWidget( "masterMenu",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 700,
			XmNheight, 150,
			RES_CONVERT( XmNdialogTitle, " " ),
			XmNlabelFontList, UxConvertFontList("-Misc-Fixed-Medium-R-SemiCondensed--13-120-75-75-C-60-ISO8859-1" ),
			XmNtextFontList, UxConvertFontList("-Misc-Fixed-Medium-R-SemiCondensed--13-120-75-75-C-60-ISO8859-1" ),
			XmNbuttonFontList, UxConvertFontList("-Misc-Fixed-Medium-R-SemiCondensed--13-120-75-75-C-60-ISO8859-1" ),
			XmNshadowThickness, 2,
			XmNallowOverlap, TRUE,
			XmNautoUnmanage, FALSE,
			XmNdefaultPosition, FALSE,
			NULL );
	XtAddCallback( masterMenu, XmNdestroyCallback,
		(XtCallbackProc) destroyCB_masterMenu,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterMenu, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardForm */
	masterKeyboardForm = XtVaCreateManagedWidget( "masterKeyboardForm",
			xmFormWidgetClass,
			masterMenu,
			XmNresizePolicy, XmRESIZE_ANY,
			XmNx, 0,
			XmNy, 370,
			XmNwidth, 400,
			XmNheight, 45,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_NONE,
			XmNtopWidget, NULL,
			XmNtopOffset, 0,
			XmNrubberPositioning, TRUE,
			NULL );
	UxPutContext( masterKeyboardForm, (char *) UxMasterMenuContext );


	/* Creation of masterMenuExpandPopup */
	masterMenuExpandPopup_shell = XtVaCreatePopupShell ("masterMenuExpandPopup_shell",
			xmMenuShellWidgetClass, masterKeyboardForm,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	masterMenuExpandPopup = XtVaCreateWidget( "masterMenuExpandPopup",
			xmRowColumnWidgetClass,
			masterMenuExpandPopup_shell,
			XmNrowColumnType, XmMENU_POPUP,
			XmNmenuAccelerator, "Shift <KeyUp>F10",
			NULL );
	UxPutContext( masterMenuExpandPopup, (char *) UxMasterMenuContext );


	/* Creation of masterMenuExpandPopup_p1_title */
	masterMenuExpandPopup_p1_title = XtVaCreateManagedWidget( "masterMenuExpandPopup_p1_title",
			xmLabelWidgetClass,
			masterMenuExpandPopup,
			RES_CONVERT( XmNlabelString, "Master Menu" ),
			NULL );
	UxPutContext( masterMenuExpandPopup_p1_title, (char *) UxMasterMenuContext );


	/* Creation of masterMenuExpandPopup_p1_b2 */
	masterMenuExpandPopup_p1_b2 = XtVaCreateManagedWidget( "masterMenuExpandPopup_p1_b2",
			xmSeparatorWidgetClass,
			masterMenuExpandPopup,
			NULL );
	UxPutContext( masterMenuExpandPopup_p1_b2, (char *) UxMasterMenuContext );


	/* Creation of masterMenuExpandPopup_p1_b7 */
	masterMenuExpandPopup_p1_b7 = XtVaCreateManagedWidget( "masterMenuExpandPopup_p1_b7",
			xmSeparatorWidgetClass,
			masterMenuExpandPopup,
			NULL );
	UxPutContext( masterMenuExpandPopup_p1_b7, (char *) UxMasterMenuContext );


	/* Creation of masterMenuPopupExpandpb */
	masterMenuPopupExpandpb = XtVaCreateManagedWidget( "masterMenuPopupExpandpb",
			xmPushButtonWidgetClass,
			masterMenuExpandPopup,
			RES_CONVERT( XmNlabelString, "Expand interface" ),
			NULL );
	XtAddCallback( masterMenuPopupExpandpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterMenuPopupExpandpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterMenuPopupExpandpb, (char *) UxMasterMenuContext );


	/* Creation of masterMenuExpandPopup_p1_b3 */
	masterMenuExpandPopup_p1_b3 = XtVaCreateManagedWidget( "masterMenuExpandPopup_p1_b3",
			xmSeparatorWidgetClass,
			masterMenuExpandPopup,
			NULL );
	UxPutContext( masterMenuExpandPopup_p1_b3, (char *) UxMasterMenuContext );


	/* Creation of masterMenuExpandPopupSpeed */
	masterMenuExpandPopupSpeed = XtVaCreateManagedWidget( "masterMenuExpandPopupSpeed",
			xmPushButtonWidgetClass,
			masterMenuExpandPopup,
			RES_CONVERT( XmNlabelString, SetSpeedLabel ),
			NULL );
	XtAddCallback( masterMenuExpandPopupSpeed, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterMenuExpandPopupSpeed,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterMenuExpandPopupSpeed, (char *) UxMasterMenuContext );


	/* Creation of masterMenuExpandPopupMaxTime */
	masterMenuExpandPopupMaxTime = XtVaCreateManagedWidget( "masterMenuExpandPopupMaxTime",
			xmPushButtonWidgetClass,
			masterMenuExpandPopup,
			RES_CONVERT( XmNlabelString, MaxTimeLabel ),
			NULL );
	XtAddCallback( masterMenuExpandPopupMaxTime, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterMenuExpandPopupMaxTime,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterMenuExpandPopupMaxTime, (char *) UxMasterMenuContext );


	/* Creation of masterMenuExpandPopup_p1_b8 */
	masterMenuExpandPopup_p1_b8 = XtVaCreateManagedWidget( "masterMenuExpandPopup_p1_b8",
			xmSeparatorWidgetClass,
			masterMenuExpandPopup,
			NULL );
	UxPutContext( masterMenuExpandPopup_p1_b8, (char *) UxMasterMenuContext );


	/* Creation of masterMenuExpandPopupShutdown */
	masterMenuExpandPopupShutdown = XtVaCreateManagedWidget( "masterMenuExpandPopupShutdown",
			xmPushButtonWidgetClass,
			masterMenuExpandPopup,
			RES_CONVERT( XmNlabelString, ShutdownLabel ),
			NULL );
	XtAddCallback( masterMenuExpandPopupShutdown, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterMenuExpandPopupShutdown,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterMenuExpandPopupShutdown, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardTasti */
	masterKeyboardTasti = XtVaCreateManagedWidget( "masterKeyboardTasti",
			xmFormWidgetClass,
			masterKeyboardForm,
			XmNresizePolicy, XmRESIZE_ANY,
			XmNx, 0,
			XmNy, 25,
			XmNwidth, 340,
			XmNheight, 20,
			XmNleftOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrubberPositioning, FALSE,
			NULL );
	UxPutContext( masterKeyboardTasti, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardRunpb */
	masterKeyboardRunpb = XtVaCreateManagedWidget( "masterKeyboardRunpb",
			xmPushButtonWidgetClass,
			masterKeyboardTasti,
			XmNx, 0,
			XmNy, 15,
			XmNwidth, 70,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, RUN_LABEL ),
			XmNfontList, UxConvertFontList("-Adobe-Courier-Bold-R-Normal--17-120-100-100-M-100-ISO8859-1" ),
			RES_CONVERT( XmNforeground, "black" ),
			XmNhighlightOnEnter, FALSE,
			XmNsensitive, FALSE,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	XtAddCallback( masterKeyboardRunpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterKeyboardRunpb,
		UxGetWidget(masterMenu) );

	UxPutContext( masterKeyboardRunpb, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardFreezepb */
	masterKeyboardFreezepb = XtVaCreateManagedWidget( "masterKeyboardFreezepb",
			xmPushButtonWidgetClass,
			masterKeyboardTasti,
			XmNx, 70,
			XmNy, 15,
			XmNwidth, 70,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, FREEZE_LABEL ),
			XmNfontList, UxConvertFontList("-Adobe-Courier-Bold-R-Normal--17-120-100-100-M-100-ISO8859-1" ),
			XmNmultiClick, XmMULTICLICK_DISCARD,
			XmNrecomputeSize, FALSE,
			XmNsensitive, FALSE,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftWidget, masterKeyboardRunpb,
			NULL );
	XtAddCallback( masterKeyboardFreezepb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterKeyboardFreezepb,
		UxGetWidget(masterMenu) );

	UxPutContext( masterKeyboardFreezepb, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardSnappb */
	masterKeyboardSnappb = XtVaCreateManagedWidget( "masterKeyboardSnappb",
			xmPushButtonWidgetClass,
			masterKeyboardTasti,
			XmNx, 138,
			XmNy, 0,
			XmNwidth, 30,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "S" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftWidget, masterKeyboardFreezepb,
			NULL );
	XtAddCallback( masterKeyboardSnappb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterKeyboardSnappb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterKeyboardSnappb, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardBtpb */
	masterKeyboardBtpb = XtVaCreateManagedWidget( "masterKeyboardBtpb",
			xmPushButtonWidgetClass,
			masterKeyboardTasti,
			XmNx, 174,
			XmNy, 0,
			XmNwidth, 30,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "B" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftWidget, masterKeyboardSnappb,
			NULL );
	XtAddCallback( masterKeyboardBtpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterKeyboardBtpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterKeyboardBtpb, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardGrafpb */
	masterKeyboardGrafpb = XtVaCreateManagedWidget( "masterKeyboardGrafpb",
			xmPushButtonWidgetClass,
			masterKeyboardTasti,
			XmNx, 210,
			XmNy, 0,
			XmNwidth, 30,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "G" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftWidget, masterKeyboardBtpb,
			NULL );
	XtAddCallback( masterKeyboardGrafpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterKeyboardGrafpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterKeyboardGrafpb, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardAingpb */
	masterKeyboardAingpb = XtVaCreateManagedWidget( "masterKeyboardAingpb",
			xmPushButtonWidgetClass,
			masterKeyboardTasti,
			XmNx, 246,
			XmNy, 0,
			XmNwidth, 30,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "I" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftWidget, masterKeyboardGrafpb,
			NULL );
	XtAddCallback( masterKeyboardAingpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterKeyboardAingpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterKeyboardAingpb, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardMalfpb */
	masterKeyboardMalfpb = XtVaCreateManagedWidget( "masterKeyboardMalfpb",
			xmPushButtonWidgetClass,
			masterKeyboardTasti,
			XmNx, 276,
			XmNy, 0,
			XmNwidth, 30,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "MF" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftWidget, masterKeyboardAingpb,
			NULL );
	XtAddCallback( masterKeyboardMalfpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterKeyboardMalfpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterKeyboardMalfpb, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardRemFuncpb */
	masterKeyboardRemFuncpb = XtVaCreateWidget( "masterKeyboardRemFuncpb",
			xmPushButtonWidgetClass,
			masterKeyboardTasti,
			XmNx, 306,
			XmNy, 0,
			XmNwidth, 30,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "RF" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftWidget, masterKeyboardMalfpb,
			NULL );
	XtAddCallback( masterKeyboardRemFuncpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterKeyboardRemFuncpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterKeyboardRemFuncpb, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardDati */
	masterKeyboardDati = XtVaCreateManagedWidget( "masterKeyboardDati",
			xmFormWidgetClass,
			masterKeyboardForm,
			XmNresizePolicy, XmRESIZE_ANY,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 340,
			XmNheight, 25,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, masterKeyboardTasti,
			XmNleftOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNrubberPositioning, FALSE,
			NULL );
	UxPutContext( masterKeyboardDati, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardStatusFrame */
	masterKeyboardStatusFrame = XtVaCreateManagedWidget( "masterKeyboardStatusFrame",
			xmFrameWidgetClass,
			masterKeyboardDati,
			XmNx, 2,
			XmNy, 0,
			XmNwidth, 130,
			XmNheight, 20,
			XmNborderWidth, 2,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( masterKeyboardStatusFrame, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardStatus */
	masterKeyboardStatus = XtVaCreateManagedWidget( "masterKeyboardStatus",
			xmLabelWidgetClass,
			masterKeyboardStatusFrame,
			XmNx, 4,
			XmNy, 4,
			XmNwidth, 136,
			XmNheight, 14,
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNlabelString, "" ),
			XmNshadowThickness, 3,
			XmNborderWidth, 0,
			XmNmarginHeight, 0,
			XmNmarginWidth, 0,
			XmNrecomputeSize, FALSE,
			NULL );
	UxPutContext( masterKeyboardStatus, (char *) UxMasterMenuContext );


	/* Creation of masterKeyboardTime */
	masterKeyboardTime = XtVaCreateManagedWidget( "masterKeyboardTime",
			xmLabelWidgetClass,
			masterKeyboardDati,
			XmNx, 140,
			XmNy, 0,
			XmNwidth, 100,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_END,
			XmNtopOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftWidget, masterKeyboardStatusFrame,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( masterKeyboardTime, (char *) UxMasterMenuContext );


	/* Creation of expandedMainForm */
	expandedMainForm = XtVaCreateManagedWidget( "expandedMainForm",
			xmFormWidgetClass,
			masterMenu,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 10,
			XmNwidth, 680,
			XmNheight, 160,
			XmNtopOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomWidget, NULL,
			NULL );
	UxPutContext( expandedMainForm, (char *) UxMasterMenuContext );


	/* Creation of commandMainForm */
	commandMainForm = XtVaCreateManagedWidget( "commandMainForm",
			xmFormWidgetClass,
			expandedMainForm,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 800,
			XmNheight, 100,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( commandMainForm, (char *) UxMasterMenuContext );


	/* Creation of master_menu */
	master_menu = XtVaCreateManagedWidget( "master_menu",
			xmRowColumnWidgetClass,
			commandMainForm,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			RES_CONVERT( XmNlabelString, "" ),
			XmNorientation, XmHORIZONTAL,
			XmNy, 0,
			XmNx, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			NULL );
	UxPutContext( master_menu, (char *) UxMasterMenuContext );


	/* Creation of FileMenuPb */
	FileMenuPb_shell = XtVaCreatePopupShell ("FileMenuPb_shell",
			xmMenuShellWidgetClass, master_menu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	FileMenuPb = XtVaCreateWidget( "FileMenuPb",
			xmRowColumnWidgetClass,
			FileMenuPb_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( FileMenuPb, (char *) UxMasterMenuContext );


	/* Creation of FileMenuInitIcpb */
	FileMenuInitIcpb = XtVaCreateManagedWidget( "FileMenuInitIcpb",
			xmPushButtonWidgetClass,
			FileMenuPb,
			RES_CONVERT( XmNlabelString, InitIcLabel ),
			NULL );
	XtAddCallback( FileMenuInitIcpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_FileMenuInitIcpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( FileMenuInitIcpb, (char *) UxMasterMenuContext );


	/* Creation of FileMenuInitBtpb */
	FileMenuInitBtpb = XtVaCreateManagedWidget( "FileMenuInitBtpb",
			xmPushButtonWidgetClass,
			FileMenuPb,
			RES_CONVERT( XmNlabelString, InitBtLabel ),
			NULL );
	XtAddCallback( FileMenuInitBtpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_FileMenuInitBtpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( FileMenuInitBtpb, (char *) UxMasterMenuContext );


	/* Creation of init_ci_menu_pb */
	init_ci_menu_pb = XtVaCreateManagedWidget( "init_ci_menu_pb",
			xmPushButtonWidgetClass,
			FileMenuPb,
			RES_CONVERT( XmNlabelString, "Init from I.C. ..." ),
			NULL );
	XtAddCallback( init_ci_menu_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_init_ci_menu_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( init_ci_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of init_bt_menu_pb */
	init_bt_menu_pb = XtVaCreateManagedWidget( "init_bt_menu_pb",
			xmPushButtonWidgetClass,
			FileMenuPb,
			RES_CONVERT( XmNlabelString, "Init from BT  ..." ),
			NULL );
	XtAddCallback( init_bt_menu_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_init_bt_menu_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( init_bt_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of init_menu_pb_b5 */
	init_menu_pb_b5 = XtVaCreateManagedWidget( "init_menu_pb_b5",
			xmSeparatorWidgetClass,
			FileMenuPb,
			NULL );
	UxPutContext( init_menu_pb_b5, (char *) UxMasterMenuContext );


	/* Creation of init_menu_pb_b9 */
	init_menu_pb_b9 = XtVaCreateManagedWidget( "init_menu_pb_b9",
			xmPushButtonWidgetClass,
			FileMenuPb,
			RES_CONVERT( XmNlabelString, StepScalingLabel ),
			NULL );
	XtAddCallback( init_menu_pb_b9, XmNactivateCallback,
		(XtCallbackProc) activateCB_init_menu_pb_b9,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( init_menu_pb_b9, (char *) UxMasterMenuContext );


	/* Creation of init_speed_menu_pb */
	init_speed_menu_pb = XtVaCreateManagedWidget( "init_speed_menu_pb",
			xmPushButtonWidgetClass,
			FileMenuPb,
			RES_CONVERT( XmNlabelString, SetSpeedLabel ),
			NULL );
	XtAddCallback( init_speed_menu_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_init_speed_menu_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( init_speed_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of init_maxspeed_menu_pb */
	init_maxspeed_menu_pb = XtVaCreateManagedWidget( "init_maxspeed_menu_pb",
			xmPushButtonWidgetClass,
			FileMenuPb,
			RES_CONVERT( XmNlabelString, MaxSpeedLabel ),
			NULL );
	XtAddCallback( init_maxspeed_menu_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_init_maxspeed_menu_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( init_maxspeed_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of init_menu_pb_b6 */
	init_menu_pb_b6 = XtVaCreateManagedWidget( "init_menu_pb_b6",
			xmSeparatorWidgetClass,
			FileMenuPb,
			NULL );
	UxPutContext( init_menu_pb_b6, (char *) UxMasterMenuContext );


	/* Creation of init_MaxTimepb */
	init_MaxTimepb = XtVaCreateManagedWidget( "init_MaxTimepb",
			xmPushButtonWidgetClass,
			FileMenuPb,
			RES_CONVERT( XmNlabelString, MaxTimeLabel ),
			NULL );
	XtAddCallback( init_MaxTimepb, XmNactivateCallback,
		(XtCallbackProc) activateCB_init_MaxTimepb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( init_MaxTimepb, (char *) UxMasterMenuContext );


	/* Creation of init_menu_pb_b10 */
	init_menu_pb_b10 = XtVaCreateManagedWidget( "init_menu_pb_b10",
			xmSeparatorWidgetClass,
			FileMenuPb,
			NULL );
	UxPutContext( init_menu_pb_b10, (char *) UxMasterMenuContext );


	/* Creation of init_clear_menu_pb */
	init_clear_menu_pb = XtVaCreateManagedWidget( "init_clear_menu_pb",
			xmPushButtonWidgetClass,
			FileMenuPb,
			RES_CONVERT( XmNlabelString, ClearLabel ),
			RES_CONVERT( XmNmnemonic, "C" ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( init_clear_menu_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_init_clear_menu_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( init_clear_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of FileMenuPb_b11 */
	FileMenuPb_b11 = XtVaCreateManagedWidget( "FileMenuPb_b11",
			xmSeparatorWidgetClass,
			FileMenuPb,
			NULL );
	UxPutContext( FileMenuPb_b11, (char *) UxMasterMenuContext );


	/* Creation of FileMenuShutdownpb */
	FileMenuShutdownpb = XtVaCreateManagedWidget( "FileMenuShutdownpb",
			xmPushButtonWidgetClass,
			FileMenuPb,
			RES_CONVERT( XmNlabelString, ShutdownLabel ),
			NULL );
	XtAddCallback( FileMenuShutdownpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_FileMenuShutdownpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( FileMenuShutdownpb, (char *) UxMasterMenuContext );


	/* Creation of master_menu_init */
	master_menu_init = XtVaCreateManagedWidget( "master_menu_init",
			xmCascadeButtonWidgetClass,
			master_menu,
			RES_CONVERT( XmNlabelString, FileMenuLabel ),
			XmNsubMenuId, FileMenuPb,
			NULL );
	UxPutContext( master_menu_init, (char *) UxMasterMenuContext );


	/* Creation of recording_menu_pb */
	recording_menu_pb_shell = XtVaCreatePopupShell ("recording_menu_pb_shell",
			xmMenuShellWidgetClass, master_menu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	recording_menu_pb = XtVaCreateWidget( "recording_menu_pb",
			xmRowColumnWidgetClass,
			recording_menu_pb_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( recording_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of contRecPane */
	contRecPane_shell = XtVaCreatePopupShell ("contRecPane_shell",
			xmMenuShellWidgetClass, recording_menu_pb,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	contRecPane = XtVaCreateWidget( "contRecPane",
			xmRowColumnWidgetClass,
			contRecPane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( contRecPane, (char *) UxMasterMenuContext );


	/* Creation of contRecMenuSelpb */
	contRecMenuSelpb = XtVaCreateManagedWidget( "contRecMenuSelpb",
			xmPushButtonWidgetClass,
			contRecPane,
			RES_CONVERT( XmNlabelString, "Select variables" ),
			NULL );
	XtAddCallback( contRecMenuSelpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_contRecMenuSelpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( contRecMenuSelpb, (char *) UxMasterMenuContext );


	/* Creation of master_menu_p12_b2 */
	master_menu_p12_b2 = XtVaCreateManagedWidget( "master_menu_p12_b2",
			xmSeparatorWidgetClass,
			contRecPane,
			NULL );
	UxPutContext( master_menu_p12_b2, (char *) UxMasterMenuContext );


	/* Creation of contRecMenuStartpb */
	contRecMenuStartpb = XtVaCreateManagedWidget( "contRecMenuStartpb",
			xmPushButtonWidgetClass,
			contRecPane,
			RES_CONVERT( XmNlabelString, "View recorded data" ),
			NULL );
	XtAddCallback( contRecMenuStartpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_contRecMenuStartpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( contRecMenuStartpb, (char *) UxMasterMenuContext );


	/* Creation of recording_cr_menu_cb */
	recording_cr_menu_cb = XtVaCreateManagedWidget( "recording_cr_menu_cb",
			xmCascadeButtonWidgetClass,
			recording_menu_pb,
			RES_CONVERT( XmNlabelString, "Cont. recording" ),
			XmNsubMenuId, contRecPane,
			NULL );
	UxPutContext( recording_cr_menu_cb, (char *) UxMasterMenuContext );


	/* Creation of recording_ITAL_menu_pb */
	recording_ITAL_menu_pb = XtVaCreateManagedWidget( "recording_ITAL_menu_pb",
			xmPushButtonWidgetClass,
			recording_menu_pb,
			RES_CONVERT( XmNlabelString, "Instructor & Trainee A.L." ),
			RES_CONVERT( XmNmnemonic, "I" ),
			NULL );
	XtAddCallback( recording_ITAL_menu_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_recording_ITAL_menu_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( recording_ITAL_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of recording_perform_menu_pb */
	recording_perform_menu_pb = XtVaCreateManagedWidget( "recording_perform_menu_pb",
			xmPushButtonWidgetClass,
			recording_menu_pb,
			RES_CONVERT( XmNlabelString, "Performance analisys" ),
			RES_CONVERT( XmNmnemonic, "P" ),
			NULL );
	XtAddCallback( recording_perform_menu_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_recording_perform_menu_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( recording_perform_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of recording_sep1_menu */
	recording_sep1_menu = XtVaCreateManagedWidget( "recording_sep1_menu",
			xmSeparatorGadgetClass,
			recording_menu_pb,
			NULL );
	UxPutContext( recording_sep1_menu, (char *) UxMasterMenuContext );


	/* Creation of recording_menu_archivio_pane */
	recording_menu_archivio_pane_shell = XtVaCreatePopupShell ("recording_menu_archivio_pane_shell",
			xmMenuShellWidgetClass, recording_menu_pb,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	recording_menu_archivio_pane = XtVaCreateWidget( "recording_menu_archivio_pane",
			xmRowColumnWidgetClass,
			recording_menu_archivio_pane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( recording_menu_archivio_pane, (char *) UxMasterMenuContext );


	/* Creation of rec_menu_arch_save_pb */
	rec_menu_arch_save_pb = XtVaCreateManagedWidget( "rec_menu_arch_save_pb",
			xmPushButtonWidgetClass,
			recording_menu_archivio_pane,
			RES_CONVERT( XmNlabelString, "Save   ...  " ),
			NULL );
	XtAddCallback( rec_menu_arch_save_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_rec_menu_arch_save_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( rec_menu_arch_save_pb, (char *) UxMasterMenuContext );


	/* Creation of recording_menu_archivio_pane_b2 */
	recording_menu_archivio_pane_b2 = XtVaCreateManagedWidget( "recording_menu_archivio_pane_b2",
			xmSeparatorWidgetClass,
			recording_menu_archivio_pane,
			NULL );
	UxPutContext( recording_menu_archivio_pane_b2, (char *) UxMasterMenuContext );


	/* Creation of rec_menu_arch_edit_pb */
	rec_menu_arch_edit_pb = XtVaCreateManagedWidget( "rec_menu_arch_edit_pb",
			xmPushButtonWidgetClass,
			recording_menu_archivio_pane,
			RES_CONVERT( XmNlabelString, "Edit   ..." ),
			NULL );
	XtAddCallback( rec_menu_arch_edit_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_rec_menu_arch_edit_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( rec_menu_arch_edit_pb, (char *) UxMasterMenuContext );


	/* Creation of recording_archivio_menu_cb */
	recording_archivio_menu_cb = XtVaCreateManagedWidget( "recording_archivio_menu_cb",
			xmCascadeButtonWidgetClass,
			recording_menu_pb,
			RES_CONVERT( XmNlabelString, "Archivio" ),
			XmNsubMenuId, recording_menu_archivio_pane,
			NULL );
	UxPutContext( recording_archivio_menu_cb, (char *) UxMasterMenuContext );


	/* Creation of master_menu_recording */
	master_menu_recording = XtVaCreateManagedWidget( "master_menu_recording",
			xmCascadeButtonWidgetClass,
			master_menu,
			RES_CONVERT( XmNlabelString, "Recording" ),
			XmNsubMenuId, recording_menu_pb,
			NULL );
	UxPutContext( master_menu_recording, (char *) UxMasterMenuContext );


	/* Creation of mffr_menu_pb */
	mffr_menu_pb_shell = XtVaCreatePopupShell ("mffr_menu_pb_shell",
			xmMenuShellWidgetClass, master_menu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mffr_menu_pb = XtVaCreateWidget( "mffr_menu_pb",
			xmRowColumnWidgetClass,
			mffr_menu_pb_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( mffr_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of malf_pane */
	malf_pane_shell = XtVaCreatePopupShell ("malf_pane_shell",
			xmMenuShellWidgetClass, mffr_menu_pb,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	malf_pane = XtVaCreateWidget( "malf_pane",
			xmRowColumnWidgetClass,
			malf_pane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( malf_pane, (char *) UxMasterMenuContext );


	/* Creation of mffr_malf_scenpb */
	mffr_malf_scenpb = XtVaCreateManagedWidget( "mffr_malf_scenpb",
			xmPushButtonWidgetClass,
			malf_pane,
			RES_CONVERT( XmNlabelString, MALF_SCEN_LIST ),
			NULL );
	XtAddCallback( mffr_malf_scenpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_mffr_malf_scenpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( mffr_malf_scenpb, (char *) UxMasterMenuContext );


	/* Creation of malf_pane_b3 */
	malf_pane_b3 = XtVaCreateManagedWidget( "malf_pane_b3",
			xmSeparatorGadgetClass,
			malf_pane,
			NULL );
	UxPutContext( malf_pane_b3, (char *) UxMasterMenuContext );


	/* Creation of mffr_malf_somm_pb */
	mffr_malf_somm_pb = XtVaCreateManagedWidget( "mffr_malf_somm_pb",
			xmPushButtonWidgetClass,
			malf_pane,
			RES_CONVERT( XmNlabelString, MALF_LIST ),
			NULL );
	XtAddCallback( mffr_malf_somm_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_mffr_malf_somm_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( mffr_malf_somm_pb, (char *) UxMasterMenuContext );


	/* Creation of mffr_menu_pb_b1 */
	mffr_menu_pb_b1 = XtVaCreateManagedWidget( "mffr_menu_pb_b1",
			xmCascadeButtonWidgetClass,
			mffr_menu_pb,
			RES_CONVERT( XmNlabelString, MALF ),
			XmNsubMenuId, malf_pane,
			NULL );
	UxPutContext( mffr_menu_pb_b1, (char *) UxMasterMenuContext );


	/* Creation of mffr_menu_pb_b3 */
	mffr_menu_pb_b3 = XtVaCreateManagedWidget( "mffr_menu_pb_b3",
			xmSeparatorWidgetClass,
			mffr_menu_pb,
			NULL );
	UxPutContext( mffr_menu_pb_b3, (char *) UxMasterMenuContext );


	/* Creation of frem_pane */
	frem_pane_shell = XtVaCreatePopupShell ("frem_pane_shell",
			xmMenuShellWidgetClass, mffr_menu_pb,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	frem_pane = XtVaCreateWidget( "frem_pane",
			xmRowColumnWidgetClass,
			frem_pane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( frem_pane, (char *) UxMasterMenuContext );


	/* Creation of mffr_frem_scenpb */
	mffr_frem_scenpb = XtVaCreateManagedWidget( "mffr_frem_scenpb",
			xmPushButtonWidgetClass,
			frem_pane,
			RES_CONVERT( XmNlabelString, FREM_SCEN_LIST ),
			NULL );
	XtAddCallback( mffr_frem_scenpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_mffr_frem_scenpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( mffr_frem_scenpb, (char *) UxMasterMenuContext );


	/* Creation of master_menu_p11_b2 */
	master_menu_p11_b2 = XtVaCreateManagedWidget( "master_menu_p11_b2",
			xmSeparatorGadgetClass,
			frem_pane,
			NULL );
	UxPutContext( master_menu_p11_b2, (char *) UxMasterMenuContext );


	/* Creation of mffr_frem_sommpb */
	mffr_frem_sommpb = XtVaCreateManagedWidget( "mffr_frem_sommpb",
			xmPushButtonWidgetClass,
			frem_pane,
			RES_CONVERT( XmNlabelString, FREM_LIST ),
			NULL );
	XtAddCallback( mffr_frem_sommpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_mffr_frem_sommpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( mffr_frem_sommpb, (char *) UxMasterMenuContext );


	/* Creation of mffr_menu_pb_b2 */
	mffr_menu_pb_b2 = XtVaCreateWidget( "mffr_menu_pb_b2",
			xmCascadeButtonWidgetClass,
			mffr_menu_pb,
			RES_CONVERT( XmNlabelString, FREM ),
			XmNsubMenuId, frem_pane,
			NULL );
	UxPutContext( mffr_menu_pb_b2, (char *) UxMasterMenuContext );


	/* Creation of master_menu_mffr */
	master_menu_mffr = XtVaCreateManagedWidget( "master_menu_mffr",
			xmCascadeButtonWidgetClass,
			master_menu,
			RES_CONVERT( XmNlabelString, "MF/FR" ),
			XmNsubMenuId, mffr_menu_pb,
			NULL );
	UxPutContext( master_menu_mffr, (char *) UxMasterMenuContext );


	/* Creation of show_menu_pb */
	show_menu_pb_shell = XtVaCreatePopupShell ("show_menu_pb_shell",
			xmMenuShellWidgetClass, master_menu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	show_menu_pb = XtVaCreateWidget( "show_menu_pb",
			xmRowColumnWidgetClass,
			show_menu_pb_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( show_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of show_statistic_menu_pb */
	show_statistic_menu_pb = XtVaCreateManagedWidget( "show_statistic_menu_pb",
			xmPushButtonWidgetClass,
			show_menu_pb,
			RES_CONVERT( XmNlabelString, "Statistic" ),
			RES_CONVERT( XmNmnemonic, "S" ),
			NULL );
	XtAddCallback( show_statistic_menu_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_show_statistic_menu_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( show_statistic_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of show_pert_pb */
	show_pert_pb = XtVaCreateManagedWidget( "show_pert_pb",
			xmPushButtonWidgetClass,
			show_menu_pb,
			RES_CONVERT( XmNlabelString, "Pert list" ),
			NULL );
	XtAddCallback( show_pert_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_show_pert_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( show_pert_pb, (char *) UxMasterMenuContext );


	/* Creation of show_proc_list */
	show_proc_list = XtVaCreateManagedWidget( "show_proc_list",
			xmPushButtonWidgetClass,
			show_menu_pb,
			RES_CONVERT( XmNlabelString, "Process list" ),
			NULL );
	XtAddCallback( show_proc_list, XmNactivateCallback,
		(XtCallbackProc) activateCB_show_proc_list,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( show_proc_list, (char *) UxMasterMenuContext );


	/* Creation of show_param_menu_pb */
	show_param_menu_pb = XtVaCreateWidget( "show_param_menu_pb",
			xmPushButtonWidgetClass,
			show_menu_pb,
			RES_CONVERT( XmNlabelString, "Parameters" ),
			RES_CONVERT( XmNmnemonic, "P" ),
			NULL );
	XtAddCallback( show_param_menu_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_show_param_menu_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( show_param_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of master_menu_show */
	master_menu_show = XtVaCreateManagedWidget( "master_menu_show",
			xmCascadeButtonWidgetClass,
			master_menu,
			RES_CONVERT( XmNlabelString, "Show" ),
			XmNsubMenuId, show_menu_pb,
			NULL );
	UxPutContext( master_menu_show, (char *) UxMasterMenuContext );


	/* Creation of help_menu_pb */
	help_menu_pb_shell = XtVaCreatePopupShell ("help_menu_pb_shell",
			xmMenuShellWidgetClass, master_menu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	help_menu_pb = XtVaCreateWidget( "help_menu_pb",
			xmRowColumnWidgetClass,
			help_menu_pb_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( help_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of help_1_menu_pb */
	help_1_menu_pb = XtVaCreateManagedWidget( "help_1_menu_pb",
			xmPushButtonWidgetClass,
			help_menu_pb,
			RES_CONVERT( XmNlabelString, "pushButton" ),
			NULL );
	UxPutContext( help_1_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of master_menu_help */
	master_menu_help = XtVaCreateWidget( "master_menu_help",
			xmCascadeButtonWidgetClass,
			master_menu,
			RES_CONVERT( XmNlabelString, "Help" ),
			XmNsubMenuId, help_menu_pb,
			XmNalignment, XmALIGNMENT_CENTER,
			XmNsensitive, TRUE,
			NULL );
	UxPutContext( master_menu_help, (char *) UxMasterMenuContext );


	/* Creation of prog_menu_pb */
	prog_menu_pb_shell = XtVaCreatePopupShell ("prog_menu_pb_shell",
			xmMenuShellWidgetClass, master_menu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	prog_menu_pb = XtVaCreateWidget( "prog_menu_pb",
			xmRowColumnWidgetClass,
			prog_menu_pb_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( prog_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of masterMenuAingpb */
	masterMenuAingpb = XtVaCreateManagedWidget( "masterMenuAingpb",
			xmPushButtonWidgetClass,
			prog_menu_pb,
			RES_CONVERT( XmNlabelString, "Xaing    " ),
			NULL );
	XtAddCallback( masterMenuAingpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterMenuAingpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterMenuAingpb, (char *) UxMasterMenuContext );


	/* Creation of masterMenuUserProgpb */
	masterMenuUserProgpb = XtVaCreateManagedWidget( "masterMenuUserProgpb",
			xmPushButtonWidgetClass,
			prog_menu_pb,
			RES_CONVERT( XmNlabelString, "User programs ..." ),
			NULL );
	XtAddCallback( masterMenuUserProgpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterMenuUserProgpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterMenuUserProgpb, (char *) UxMasterMenuContext );


	/* Creation of master_menu_top_b1 */
	master_menu_top_b1 = XtVaCreateManagedWidget( "master_menu_top_b1",
			xmCascadeButtonWidgetClass,
			master_menu,
			RES_CONVERT( XmNlabelString, "Programs" ),
			XmNsubMenuId, prog_menu_pb,
			NULL );
	UxPutContext( master_menu_top_b1, (char *) UxMasterMenuContext );


	/* Creation of option_menu_pb */
	option_menu_pb_shell = XtVaCreatePopupShell ("option_menu_pb_shell",
			xmMenuShellWidgetClass, master_menu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	option_menu_pb = XtVaCreateWidget( "option_menu_pb",
			xmRowColumnWidgetClass,
			option_menu_pb_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( option_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of option_load_menu_pb */
	option_load_menu_pb = XtVaCreateWidget( "option_load_menu_pb",
			xmPushButtonWidgetClass,
			option_menu_pb,
			RES_CONVERT( XmNlabelString, "Load      " ),
			NULL );
	XtAddCallback( option_load_menu_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_option_load_menu_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( option_load_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of option_edit_menu_pb */
	option_edit_menu_pb = XtVaCreateManagedWidget( "option_edit_menu_pb",
			xmPushButtonWidgetClass,
			option_menu_pb,
			RES_CONVERT( XmNlabelString, "Edit     " ),
			NULL );
	XtAddCallback( option_edit_menu_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_option_edit_menu_pb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( option_edit_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of master_menu_options */
	master_menu_options = XtVaCreateManagedWidget( "master_menu_options",
			xmCascadeButtonWidgetClass,
			master_menu,
			RES_CONVERT( XmNlabelString, "Options" ),
			XmNsubMenuId, option_menu_pb,
			NULL );
	UxPutContext( master_menu_options, (char *) UxMasterMenuContext );


	/* Creation of archive_menu_pb */
	archive_menu_pb_shell = XtVaCreatePopupShell ("archive_menu_pb_shell",
			xmMenuShellWidgetClass, master_menu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	archive_menu_pb = XtVaCreateWidget( "archive_menu_pb",
			xmRowColumnWidgetClass,
			archive_menu_pb_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( archive_menu_pb, (char *) UxMasterMenuContext );


	/* Creation of archive_session */
	archive_session = XtVaCreateManagedWidget( "archive_session",
			xmPushButtonWidgetClass,
			archive_menu_pb,
			RES_CONVERT( XmNlabelString, "Session      " ),
			NULL );
	XtAddCallback( archive_session, XmNactivateCallback,
		(XtCallbackProc) activateCB_archive_session,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( archive_session, (char *) UxMasterMenuContext );


	/* Creation of master_menu_top_b2 */
	master_menu_top_b2 = XtVaCreateManagedWidget( "master_menu_top_b2",
			xmCascadeButtonWidgetClass,
			master_menu,
			RES_CONVERT( XmNlabelString, "Archiving" ),
			XmNsubMenuId, archive_menu_pb,
			NULL );
	UxPutContext( master_menu_top_b2, (char *) UxMasterMenuContext );


	/* Creation of form2 */
	form2 = XtVaCreateManagedWidget( "form2",
			xmFormWidgetClass,
			commandMainForm,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 5,
			XmNy, 31,
			XmNwidth, 295,
			XmNheight, 100,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 0,
			XmNtopWidget, master_menu,
			XmNleftOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( form2, (char *) UxMasterMenuContext );


	/* Creation of masterMenuFreezepb */
	masterMenuFreezepb = XtVaCreateManagedWidget( "masterMenuFreezepb",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 70,
			XmNy, 15,
			XmNwidth, 70,
			XmNheight, 40,
			RES_CONVERT( XmNlabelString, FREEZE_LABEL ),
			XmNfontList, UxConvertFontList("-Adobe-Courier-Bold-R-Normal--17-120-100-100-M-100-ISO8859-1" ),
			XmNmultiClick, XmMULTICLICK_DISCARD,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrecomputeSize, FALSE,
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( masterMenuFreezepb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterMenuFreezepb,
		UxGetWidget(masterMenu) );

	UxPutContext( masterMenuFreezepb, (char *) UxMasterMenuContext );


	/* Creation of masterMenuRunpb */
	masterMenuRunpb = XtVaCreateManagedWidget( "masterMenuRunpb",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 0,
			XmNy, 15,
			XmNwidth, 70,
			XmNheight, 40,
			RES_CONVERT( XmNlabelString, RUN_LABEL ),
			XmNfontList, UxConvertFontList("-Adobe-Courier-Bold-R-Normal--17-120-100-100-M-100-ISO8859-1" ),
			RES_CONVERT( XmNforeground, "black" ),
			XmNhighlightOnEnter, FALSE,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNbottomOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( masterMenuRunpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterMenuRunpb,
		UxGetWidget(masterMenu) );

	UxPutContext( masterMenuRunpb, (char *) UxMasterMenuContext );


	/* Creation of labelStatusHead */
	labelStatusHead = XtVaCreateManagedWidget( "labelStatusHead",
			xmLabelWidgetClass,
			form2,
			XmNx, 160,
			XmNy, 0,
			XmNwidth, 130,
			XmNheight, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftWidget, masterMenuFreezepb,
			XmNrightAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Simulator status:" ),
			NULL );
	UxPutContext( labelStatusHead, (char *) UxMasterMenuContext );


	/* Creation of labelStatusFrame */
	labelStatusFrame = XtVaCreateManagedWidget( "labelStatusFrame",
			xmFrameWidgetClass,
			form2,
			XmNx, 150,
			XmNy, 20,
			XmNwidth, 130,
			XmNheight, 30,
			XmNbottomOffset, 20,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftWidget, masterMenuFreezepb,
			XmNrightAttachment, XmATTACH_FORM,
			XmNborderWidth, 2,
			NULL );
	UxPutContext( labelStatusFrame, (char *) UxMasterMenuContext );


	/* Creation of labelStatus */
	labelStatus = XtVaCreateManagedWidget( "labelStatus",
			xmLabelWidgetClass,
			labelStatusFrame,
			XmNx, 140,
			XmNy, 20,
			XmNwidth, 151,
			XmNheight, 30,
			XmNhighlightThickness, 0,
			RES_CONVERT( XmNlabelString, "" ),
			XmNshadowThickness, 3,
			XmNborderWidth, 0,
			XmNmarginHeight, 0,
			XmNmarginWidth, 0,
			NULL );
	UxPutContext( labelStatus, (char *) UxMasterMenuContext );


	/* Creation of masterMenuData */
	masterMenuData = XtVaCreateManagedWidget( "masterMenuData",
			xmFormWidgetClass,
			commandMainForm,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 300,
			XmNy, 31,
			XmNwidth, 350,
			XmNheight, 99,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 0,
			XmNtopWidget, master_menu,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, form2,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( masterMenuData, (char *) UxMasterMenuContext );


	/* Creation of rowColumn1 */
	rowColumn1 = XtVaCreateManagedWidget( "rowColumn1",
			xmRowColumnWidgetClass,
			masterMenuData,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 190,
			XmNheight, 64,
			XmNnumColumns, 2,
			XmNpacking, XmPACK_COLUMN,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 60,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( rowColumn1, (char *) UxMasterMenuContext );


	/* Creation of simTimelabel */
	simTimelabel = XtVaCreateManagedWidget( "simTimelabel",
			xmLabelWidgetClass,
			rowColumn1,
			XmNx, 3,
			XmNy, 3,
			XmNwidth, 77,
			XmNheight, 20,
			XmNrecomputeSize, FALSE,
			RES_CONVERT( XmNlabelString, SIMTIME_LABEL ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( simTimelabel, (char *) UxMasterMenuContext );


	/* Creation of delayTimelabel1 */
	delayTimelabel1 = XtVaCreateManagedWidget( "delayTimelabel1",
			xmLabelWidgetClass,
			rowColumn1,
			XmNx, 0,
			XmNy, 20,
			XmNwidth, 77,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, RITARDO_LABEL ),
			NULL );
	UxPutContext( delayTimelabel1, (char *) UxMasterMenuContext );


	/* Creation of delayTimelabel2 */
	delayTimelabel2 = XtVaCreateManagedWidget( "delayTimelabel2",
			xmLabelWidgetClass,
			rowColumn1,
			XmNx, 0,
			XmNy, 50,
			XmNwidth, 77,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, SPEED_LABEL ),
			NULL );
	UxPutContext( delayTimelabel2, (char *) UxMasterMenuContext );


	/* Creation of simTimeDisplay */
	simTimeDisplay = XtVaCreateManagedWidget( "simTimeDisplay",
			xmLabelWidgetClass,
			rowColumn1,
			XmNx, 120,
			XmNy, 3,
			XmNwidth, 70,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_END,
			NULL );
	UxPutContext( simTimeDisplay, (char *) UxMasterMenuContext );


	/* Creation of delayTimeDisplay */
	delayTimeDisplay = XtVaCreateManagedWidget( "delayTimeDisplay",
			xmLabelWidgetClass,
			rowColumn1,
			XmNx, 120,
			XmNy, 28,
			XmNwidth, 70,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_END,
			NULL );
	UxPutContext( delayTimeDisplay, (char *) UxMasterMenuContext );


	/* Creation of speedDisplay */
	speedDisplay = XtVaCreateManagedWidget( "speedDisplay",
			xmLabelWidgetClass,
			rowColumn1,
			XmNx, 120,
			XmNy, 53,
			XmNwidth, 70,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_END,
			NULL );
	UxPutContext( speedDisplay, (char *) UxMasterMenuContext );


	/* Creation of tastieraMaster */
	tastieraMaster = XtVaCreateManagedWidget( "tastieraMaster",
			xmBulletinBoardWidgetClass,
			masterMenuData,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 200,
			XmNy, 0,
			XmNwidth, 150,
			XmNheight, 70,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, rowColumn1,
			NULL );
	UxPutContext( tastieraMaster, (char *) UxMasterMenuContext );


	/* Creation of tastieraButton5 */
	tastieraButton5 = XtVaCreateManagedWidget( "tastieraButton5",
			xmPushButtonWidgetClass,
			tastieraMaster,
			XmNx, 130,
			XmNy, 10,
			XmNwidth, 25,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "MF" ),
			NULL );
	XtAddCallback( tastieraButton5, XmNactivateCallback,
		(XtCallbackProc) activateCB_tastieraButton5,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( tastieraButton5, (char *) UxMasterMenuContext );


	/* Creation of tastieraButton4 */
	tastieraButton4 = XtVaCreateManagedWidget( "tastieraButton4",
			xmPushButtonWidgetClass,
			tastieraMaster,
			XmNx, 100,
			XmNy, 10,
			XmNwidth, 25,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "I" ),
			NULL );
	XtAddCallback( tastieraButton4, XmNactivateCallback,
		(XtCallbackProc) activateCB_tastieraButton4,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( tastieraButton4, (char *) UxMasterMenuContext );


	/* Creation of tastieraButton3 */
	tastieraButton3 = XtVaCreateManagedWidget( "tastieraButton3",
			xmPushButtonWidgetClass,
			tastieraMaster,
			XmNx, 70,
			XmNy, 10,
			XmNwidth, 25,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "G" ),
			NULL );
	XtAddCallback( tastieraButton3, XmNactivateCallback,
		(XtCallbackProc) activateCB_tastieraButton3,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( tastieraButton3, (char *) UxMasterMenuContext );


	/* Creation of tastieraButton2 */
	tastieraButton2 = XtVaCreateManagedWidget( "tastieraButton2",
			xmPushButtonWidgetClass,
			tastieraMaster,
			XmNx, 40,
			XmNy, 10,
			XmNwidth, 25,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "B" ),
			NULL );
	XtAddCallback( tastieraButton2, XmNactivateCallback,
		(XtCallbackProc) activateCB_tastieraButton2,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( tastieraButton2, (char *) UxMasterMenuContext );


	/* Creation of tastieraButton1 */
	tastieraButton1 = XtVaCreateManagedWidget( "tastieraButton1",
			xmPushButtonWidgetClass,
			tastieraMaster,
			XmNx, 0,
			XmNy, 10,
			XmNwidth, 25,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "S" ),
			NULL );
	XtAddCallback( tastieraButton1, XmNactivateCallback,
		(XtCallbackProc) activateCB_tastieraButton1,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( tastieraButton1, (char *) UxMasterMenuContext );


	/* Creation of tastieraButton10 */
	tastieraButton10 = XtVaCreateWidget( "tastieraButton10",
			xmPushButtonWidgetClass,
			tastieraMaster,
			XmNx, 130,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "RF" ),
			NULL );
	XtAddCallback( tastieraButton10, XmNactivateCallback,
		(XtCallbackProc) activateCB_tastieraButton10,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( tastieraButton10, (char *) UxMasterMenuContext );


	/* Creation of tastieraButton9 */
	tastieraButton9 = XtVaCreateWidget( "tastieraButton9",
			xmPushButtonWidgetClass,
			tastieraMaster,
			XmNx, 100,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "" ),
			NULL );
	XtAddCallback( tastieraButton9, XmNactivateCallback,
		(XtCallbackProc) activateCB_tastieraButton9,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( tastieraButton9, (char *) UxMasterMenuContext );


	/* Creation of tastieraButton8 */
	tastieraButton8 = XtVaCreateWidget( "tastieraButton8",
			xmPushButtonWidgetClass,
			tastieraMaster,
			XmNx, 70,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "" ),
			NULL );
	UxPutContext( tastieraButton8, (char *) UxMasterMenuContext );


	/* Creation of tastieraButton7 */
	tastieraButton7 = XtVaCreateWidget( "tastieraButton7",
			xmPushButtonWidgetClass,
			tastieraMaster,
			XmNx, 40,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "" ),
			NULL );
	UxPutContext( tastieraButton7, (char *) UxMasterMenuContext );


	/* Creation of tastieraButton6 */
	tastieraButton6 = XtVaCreateWidget( "tastieraButton6",
			xmPushButtonWidgetClass,
			tastieraMaster,
			XmNx, 10,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "X" ),
			NULL );
	XtAddCallback( tastieraButton6, XmNactivateCallback,
		(XtCallbackProc) activateCB_tastieraButton6,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( tastieraButton6, (char *) UxMasterMenuContext );


	/* Creation of masterMenuCompressPopup */
	masterMenuCompressPopup_shell = XtVaCreatePopupShell ("masterMenuCompressPopup_shell",
			xmMenuShellWidgetClass, commandMainForm,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	masterMenuCompressPopup = XtVaCreateWidget( "masterMenuCompressPopup",
			xmRowColumnWidgetClass,
			masterMenuCompressPopup_shell,
			XmNrowColumnType, XmMENU_POPUP,
			XmNmenuAccelerator, "Shift <KeyUp>F10",
			NULL );
	UxPutContext( masterMenuCompressPopup, (char *) UxMasterMenuContext );


	/* Creation of masterMenuCompresspb */
	masterMenuCompresspb = XtVaCreateManagedWidget( "masterMenuCompresspb",
			xmPushButtonWidgetClass,
			masterMenuCompressPopup,
			RES_CONVERT( XmNlabelString, "Compress interface" ),
			NULL );
	XtAddCallback( masterMenuCompresspb, XmNactivateCallback,
		(XtCallbackProc) activateCB_masterMenuCompresspb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( masterMenuCompresspb, (char *) UxMasterMenuContext );


	/* Creation of messageForm */
	messageForm = XtVaCreateManagedWidget( "messageForm",
			xmFormWidgetClass,
			expandedMainForm,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 100,
			XmNwidth, 700,
			XmNheight, 60,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopWidget, commandMainForm,
			NULL );
	UxPutContext( messageForm, (char *) UxMasterMenuContext );


	/* Creation of scrolledWindow1 */
	scrolledWindow1 = XtVaCreateManagedWidget( "scrolledWindow1",
			xmScrolledWindowWidgetClass,
			messageForm,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 5,
			XmNy, 0,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 20,
			XmNtraversalOn, TRUE,
			XmNrightOffset, 10,
			XmNleftOffset, 10,
			XmNbottomOffset, 10,
			NULL );
	UxPutContext( scrolledWindow1, (char *) UxMasterMenuContext );


	/* Creation of areaMessaggi */
	areaMessaggi = XtVaCreateManagedWidget( "areaMessaggi",
			xmListWidgetClass,
			scrolledWindow1,
			XmNwidth, 600,
			XmNheight, 75,
			XmNlistSizePolicy, XmVARIABLE,
			NULL );
	UxPutContext( areaMessaggi, (char *) UxMasterMenuContext );


	/* Creation of areaMessaggiPopupMenu */
	areaMessaggiPopupMenu_shell = XtVaCreatePopupShell ("areaMessaggiPopupMenu_shell",
			xmMenuShellWidgetClass, areaMessaggi,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	areaMessaggiPopupMenu = XtVaCreateWidget( "areaMessaggiPopupMenu",
			xmRowColumnWidgetClass,
			areaMessaggiPopupMenu_shell,
			XmNrowColumnType, XmMENU_POPUP,
			XmNmenuAccelerator, "<KeyUp>F4",
			NULL );
	UxPutContext( areaMessaggiPopupMenu, (char *) UxMasterMenuContext );


	/* Creation of menu2_p1_title */
	menu2_p1_title = XtVaCreateManagedWidget( "menu2_p1_title",
			xmLabelWidgetClass,
			areaMessaggiPopupMenu,
			RES_CONVERT( XmNlabelString, "MESSAGE" ),
			NULL );
	UxPutContext( menu2_p1_title, (char *) UxMasterMenuContext );


	/* Creation of areaMessaggiPopupMenu_b2 */
	areaMessaggiPopupMenu_b2 = XtVaCreateManagedWidget( "areaMessaggiPopupMenu_b2",
			xmSeparatorWidgetClass,
			areaMessaggiPopupMenu,
			NULL );
	UxPutContext( areaMessaggiPopupMenu_b2, (char *) UxMasterMenuContext );


	/* Creation of areaMessaggiPopupClearpb */
	areaMessaggiPopupClearpb = XtVaCreateManagedWidget( "areaMessaggiPopupClearpb",
			xmPushButtonWidgetClass,
			areaMessaggiPopupMenu,
			RES_CONVERT( XmNlabelString, "Clear list" ),
			NULL );
	XtAddCallback( areaMessaggiPopupClearpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_areaMessaggiPopupClearpb,
		(XtPointer) UxMasterMenuContext );

	UxPutContext( areaMessaggiPopupClearpb, (char *) UxMasterMenuContext );


	/* Creation of messageLabel */
	messageLabel = XtVaCreateManagedWidget( "messageLabel",
			xmLabelWidgetClass,
			messageForm,
			XmNx, 5,
			XmNy, 0,
			XmNwidth, 115,
			XmNheight, 20,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, scrolledWindow1,
			RES_CONVERT( XmNlabelString, "Messages" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 5,
			NULL );
	UxPutContext( messageLabel, (char *) UxMasterMenuContext );


	/* Creation of separator2 */
	separator2 = XtVaCreateManagedWidget( "separator2",
			xmSeparatorWidgetClass,
			messageForm,
			XmNx, 5,
			XmNy, 0,
			XmNwidth, 590,
			XmNheight, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( separator2, (char *) UxMasterMenuContext );

	XtVaSetValues(master_menu,
			XmNmenuHelpWidget, master_menu_help,
			NULL );


	XtAddCallback( masterMenu, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxMasterMenuContext);

	XtAddEventHandler(masterKeyboardForm, ButtonPressMask,
			False, (XtEventHandler) _UxmasterMenuMenuPost, (XtPointer) masterMenuExpandPopup );
	XtAddEventHandler(commandMainForm, ButtonPressMask,
			False, (XtEventHandler) _UxmasterMenuMenuPost, (XtPointer) masterMenuCompressPopup );
	XtAddEventHandler(areaMessaggi, ButtonPressMask,
			False, (XtEventHandler) _UxmasterMenuMenuPost, (XtPointer) areaMessaggiPopupMenu );

	return ( masterMenu );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_masterMenu()
{
	Widget                  rtrn;
	_UxCmasterMenu          *UxContext;
	static int		_Uxinit = 0;

	UxMasterMenuContext = UxContext =
		(_UxCmasterMenu *) UxNewContext( sizeof(_UxCmasterMenu), False );


	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewInterfaceClassId();
		_Uxinit = 1;
	}

	{
		Atom WM_DELETE_WINDOW;                  /* per close da MWM */
		
		selVarAing = (Widget)NULL;
		statisticheDisplayTask = (Widget)NULL;
		rtrn = _Uxbuild_masterMenu();
		UxPutClassCode( masterMenu, _UxIfClassId );

		expandInterface (masterMenu);
		connessioni (rtrn);
#ifndef DESIGN_TIME
		startup_simulatore(rtrn);
		/*strcpy (titolo_sessione,DEFAULT_TITLE);*/
		strcpy (titolo_sessione,mStr[_DEFAULT_TITLE].string);
		strcpy (init_from,DEFAULT_INIT); 
		update_titolo_sessione (rtrn);
		XtAddEventHandler (UxGetWidget(areaMessaggi), ButtonPressMask, False, 
		                    Popup_Menu, areaMessaggiPopupMenu);
		check_opzioni();
		
		attiva_timer_masterMenu(masterMenu);
		
		master_interface_active = 1;
		
		XtVaSetValues(XtParent(masterMenu),XmNdeleteResponse,XmDO_NOTHING,NULL);
		WM_DELETE_WINDOW=XmInternAtom(XtDisplay(XtParent(masterMenu)),"WM_DELETE_WINDOW",False);
		XmAddWMProtocolCallback(XtParent(masterMenu),WM_DELETE_WINDOW,activateCB_FileMenuShutdownpb,NULL);
#endif
		UxPopupInterface(rtrn,no_grab);
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

