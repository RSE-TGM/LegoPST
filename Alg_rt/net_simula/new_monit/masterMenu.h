
/*******************************************************************************
       masterMenu.h
       This header file is included by masterMenu.c

*******************************************************************************/

#ifndef	_MASTERMENU_INCLUDED
#define	_MASTERMENU_INCLUDED

#include <stdio.h>

#ifdef MOTIF
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#endif /* MOTIF */

#include "UxXt.h"

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
	Widget	UxtastieraHmipb;
	Widget	UxmasterKeyboardHmipb;
} _UxCmasterMenu;

#ifdef CONTEXT_MACRO_ACCESS
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
#define tastieraHmipb           UxMasterMenuContext->UxtastieraHmipb
#define masterKeyboardHmipb     UxMasterMenuContext->UxmasterKeyboardHmipb

#endif /* CONTEXT_MACRO_ACCESS */

extern Widget	masterMenu;
extern Widget	master_menu_help;
extern Widget	masterMenuFreezepb;
extern Widget	masterMenuRunpb;
extern Widget	simTimeDisplay;
extern Widget	delayTimeDisplay;
extern Widget	speedDisplay;
extern Widget	areaMessaggi;
extern Widget	areaMessaggiPopupMenu;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_masterMenu();

#endif	/* _MASTERMENU_INCLUDED */
