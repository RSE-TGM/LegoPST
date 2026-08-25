! UIMX ascii 2.8 key: 785                                                       

*masterMenu.class: formDialog
*masterMenu.classinc:
*masterMenu.classspec:
*masterMenu.classmembers:
*masterMenu.classconstructor:
*masterMenu.classdestructor:
*masterMenu.gbldecl: #include <stdio.h>\
#include <X11/Xatom.h>\
#include <Xm/Protocols.h>\
#include "master_monit.h"\
#include "init_sim.h"\
#ifndef DESIGN_TIME\
#include "interf_attive.h"\
#include "parametri.h"\
#endif\
#include "sked.h"\
#include "malfunzioni.h"\
#include "banco_globals.h"\
extern int stato_sim;\
#include "malfunzioni.h"\
#include "messaggi.h" \
#include "messages.h"\
#include "filtri.h"\
extern MESSAGES mStr[maxIndxMessages];\
extern Widget create_selVar();\
extern char *create_processStart();\
extern swidget create_programLauncher();\
extern Widget attiva_statistiche ();\
extern swidget create_scenarioMalf();\
extern swidget create_resourceSetting();\
extern swidget create_archiveSess();\
char defaultDisplay[MAX_DISPLAY_NAME];\

*masterMenu.ispecdecl: Widget selVarAing;\
Widget statisticheDisplayTask;
*masterMenu.ispeclist: selVarAing, statisticheDisplayTask
*masterMenu.ispeclist.selVarAing: "Widget", "%selVarAing%"
*masterMenu.ispeclist.statisticheDisplayTask: "Widget", "%statisticheDisplayTask%"
*masterMenu.funcdecl: swidget create_masterMenu()\

*masterMenu.funcname: create_masterMenu
*masterMenu.funcdef: "swidget", "<create_masterMenu>(%)"
*masterMenu.icode: Atom WM_DELETE_WINDOW;                  /* per close da MWM */\
\
selVarAing = (Widget)NULL;\
statisticheDisplayTask = (Widget)NULL;
*masterMenu.fcode: expandInterface (masterMenu);\
connessioni (rtrn);\
#ifndef DESIGN_TIME\
startup_simulatore(rtrn);\
/*strcpy (titolo_sessione,DEFAULT_TITLE);*/\
strcpy (titolo_sessione,mStr[_DEFAULT_TITLE].string);\
strcpy (init_from,DEFAULT_INIT); \
update_titolo_sessione (rtrn);\
XtAddEventHandler (UxGetWidget(areaMessaggi), ButtonPressMask, False, \
                    Popup_Menu, areaMessaggiPopupMenu);\
check_opzioni();\
\
attiva_timer_masterMenu(masterMenu);\
\
master_interface_active = 1;\
\
XtVaSetValues(XtParent(masterMenu),XmNdeleteResponse,XmDO_NOTHING,NULL);\
WM_DELETE_WINDOW=XmInternAtom(XtDisplay(XtParent(masterMenu)),"WM_DELETE_WINDOW",False);\
XmAddWMProtocolCallback(XtParent(masterMenu),WM_DELETE_WINDOW,activateCB_FileMenuShutdownpb,NULL);\
#endif\
UxPopupInterface(rtrn,no_grab);\
return(rtrn);\

*masterMenu.auxdecl: #ifndef DESIGN_TIME\
int aggiorna_opzioni_init (w, p)\
Widget w;\
OPTIONS_FLAGS *p;\
{\
   if (! p->options_init.clear)\
      XtUnmanageChild (init_clear_menu_pb);\
   else\
      XtManageChild (init_clear_menu_pb);\
\
   if (!p->options_init.from_bt)\
      XtUnmanageChild (init_bt_menu_pb); \
   else\
     XtManageChild (init_bt_menu_pb);\
\
   if (!p->options_init.from_ci)  \
     XtUnmanageChild (init_ci_menu_pb);\
   else\
     XtManageChild  (init_ci_menu_pb);\
}\
#endif\
/****************************************************/\
#ifndef DESIGN_TIME\
int aggiorna_opzioni_recording (w, p)\
Widget w;\
OPTIONS_FLAGS *p;\
{\
   if (p->options_recording.active)\
      {\
      XtManageChild (master_menu_recording);\
      if (!p->options_recording.backtrack)\
         XtUnmanageChild (FileMenuInitBtpb);\
      else\
         XtManageChild (FileMenuInitBtpb);\
\
      if (! p->options_recording.cr)\
         XtUnmanageChild (recording_cr_menu_cb);\
      else\
         XtManageChild (recording_cr_menu_cb);\
\
      if (!p->options_recording.ital)\
         XtUnmanageChild (recording_ITAL_menu_pb);\
      else\
         XtManageChild (recording_ITAL_menu_pb);\
\
      if (!p->options_recording.performance)\
         XtUnmanageChild (recording_perform_menu_pb);\
      else\
         XtManageChild (recording_perform_menu_pb);\
\
      if (!p->options_recording.archive)\
         {\
         XtUnmanageChild (recording_sep1_menu);\
         XtUnmanageChild (recording_archivio_menu_cb);\
         }\
      else\
         {\
         XtManageChild (recording_sep1_menu);\
         XtManageChild (recording_archivio_menu_cb);\
         }\
      }\
   else\
      XtUnmanageChild (master_menu_recording);\
}\
#endif\
/****************************************************/\
#ifndef DESIGN_TIME\
int aggiorna_opzioni_snap (w, p)\
Widget w;\
OPTIONS_FLAGS *p;\
{\
   if (!p->options_snapshot.active)\
      XtUnmanageChild (FileMenuInitIcpb);\
   else\
      XtManageChild (FileMenuInitIcpb);\
}\
#endif\
/****************************************************/\
#ifndef DESIGN_TIME\
int aggiorna_opzioni_mffr (w, p)\
Widget w;\
OPTIONS_FLAGS *p;\
{\
   if (!p->options_mffr.active)\
      XtUnmanageChild (master_menu_mffr);\
   else\
      XtManageChild (master_menu_mffr);\
}\
#endif\
/****************************************************/\
#ifndef DESIGN_TIME\
int aggiorna_opzioni_show (w, p)\
Widget w;\
OPTIONS_FLAGS *p;\
{\
   if (!p->options_show.active)\
     XtUnmanageChild (master_menu_show);\
   else\
     XtManageChild (master_menu_show);\
}\
#endif\
/****************************************************/\
int check_opzioni()\
{\
#ifndef DESIGN_TIME\
/* \
 * init\
 */\
   if (! options.options_init.clear)\
      XtUnmanageChild (init_clear_menu_pb);\
   if (!options.options_init.from_bt)\
      XtUnmanageChild (init_bt_menu_pb);\
   if (!options.options_init.from_ci)\
      XtUnmanageChild (init_ci_menu_pb);\
\
/*\
 * snapshot\
 */\
   if (!options.options_snapshot.active)\
      XtUnmanageChild (FileMenuInitIcpb);\
   \
/*\
 *recording\
 */\
   if (options.options_recording.active)\
      {\
      if (!options.options_recording.backtrack)\
         XtUnmanageChild (FileMenuInitBtpb);\
      if (! options.options_recording.cr)\
         XtUnmanageChild (recording_cr_menu_cb);\
      if (!options.options_recording.ital)\
         XtUnmanageChild (recording_ITAL_menu_pb);\
      if (!options.options_recording.performance)\
         XtUnmanageChild (recording_perform_menu_pb);\
      if (!options.options_recording.archive)\
         {\
         XtUnmanageChild (recording_sep1_menu);\
         XtUnmanageChild (recording_archivio_menu_cb);\
         }\
      }\
   else\
      XtUnmanageChild (master_menu_recording);\
/*\
 * mffr\
 */\
   if (!options.options_mffr.active)\
      XtUnmanageChild (master_menu_mffr);\
\
/*\
 * show\
 */\
   if (!options.options_show.active)\
     XtUnmanageChild (master_menu_show); \
\
\
\
   return(0);\
#endif\
}\
/**********************************************************/\
int update_menu ()\
{\
static int flag = 0;\
extern int last_backtrack;\
\
/*\
printf("update_menu: last_backtrack = %d\n", last_backtrack);\
*/\
   if (last_backtrack <= 0)   /* nessun backtrack registrato   */\
      {\
/*\
printf("update_menu: last_backtrack = %d  last_backtrack <= 0\n",last_backtrack);\
*/\
      flag = 1;\
      XtVaSetValues (init_bt_menu_pb, XmNsensitive, False, NULL);\
      XtVaSetValues (FileMenuInitBtpb, XmNsensitive, False, NULL);   \
/* tasto rapido backtrack */\
      XtVaSetValues (tastieraButton2, XmNsensitive, False, NULL);  \
      XtVaSetValues (masterKeyboardBtpb, XmNsensitive, False, NULL);\
      }\
   else if (flag && (last_backtrack > 0))\
      {\
/*\
printf("update_menu: last_backtrack = %d last_backtrack > 0\n",last_backtrack);\
*/\
      flag = 0;\
      XtVaSetValues (init_bt_menu_pb, XmNsensitive, True, NULL);\
      XtVaSetValues (FileMenuInitBtpb, XmNsensitive, True, NULL);    \
/* tasto rapido backtrack */\
      XtVaSetValues (tastieraButton2, XmNsensitive, True, NULL); \
      XtVaSetValues (masterKeyboardBtpb, XmNsensitive, True, NULL);\
      }\
   \
   return (0);\
}\
/******************************************/\
int compressInterface (Widget wgt)\
{\
XtManageChild (masterKeyboardForm);\
XtUnmanageChild (expandedMainForm);\
}\
/******************************************/\
int expandInterface (Widget wgt)\
{\
XtManageChild (expandedMainForm);\
XtUnmanageChild (masterKeyboardForm);\
}
*masterMenu.name.source: public
*masterMenu.static: false
*masterMenu.name: masterMenu
*masterMenu.parent: NO_PARENT
*masterMenu.parentExpression: UxWidgetToSwidget (UxTopLevel)
*masterMenu.defaultShell: topLevelShell
*masterMenu.unitType: "pixels"
*masterMenu.x: 510
*masterMenu.y: 0
*masterMenu.width: 700
*masterMenu.height: 150
*masterMenu.dialogTitle: " "
*masterMenu.labelFontList: "-Misc-Fixed-Medium-R-SemiCondensed--13-120-75-75-C-60-ISO8859-1"
*masterMenu.textFontList: "-Misc-Fixed-Medium-R-SemiCondensed--13-120-75-75-C-60-ISO8859-1"
*masterMenu.buttonFontList: "-Misc-Fixed-Medium-R-SemiCondensed--13-120-75-75-C-60-ISO8859-1"
*masterMenu.shadowThickness: 2
*masterMenu.allowOverlap: "true"
*masterMenu.autoUnmanage: "false"
*masterMenu.defaultPosition: "false"
*masterMenu.allowShellResize: "true"
*masterMenu.destroyCallback: {\
\
}

*masterKeyboardForm.class: form
*masterKeyboardForm.static: true
*masterKeyboardForm.name: masterKeyboardForm
*masterKeyboardForm.parent: masterMenu
*masterKeyboardForm.resizePolicy: "resize_any"
*masterKeyboardForm.x: 0
*masterKeyboardForm.y: 370
*masterKeyboardForm.width: 400
*masterKeyboardForm.height: 45
*masterKeyboardForm.bottomAttachment: "attach_form"
*masterKeyboardForm.rightAttachment: "attach_none"
*masterKeyboardForm.leftAttachment: "attach_form"
*masterKeyboardForm.topAttachment: "attach_none"
*masterKeyboardForm.topWidget: ""
*masterKeyboardForm.topOffset: 0
*masterKeyboardForm.createManaged: "true"
*masterKeyboardForm.rubberPositioning: "true"

*masterMenuExpandPopup.class: rowColumn
*masterMenuExpandPopup.static: true
*masterMenuExpandPopup.name: masterMenuExpandPopup
*masterMenuExpandPopup.parent: masterKeyboardForm
*masterMenuExpandPopup.rowColumnType: "menu_popup"
*masterMenuExpandPopup.menuAccelerator: "Shift <KeyUp>F10"

*masterMenuExpandPopup_p1_title.class: label
*masterMenuExpandPopup_p1_title.static: true
*masterMenuExpandPopup_p1_title.name: masterMenuExpandPopup_p1_title
*masterMenuExpandPopup_p1_title.parent: masterMenuExpandPopup
*masterMenuExpandPopup_p1_title.labelString: "Master Menu"

*masterMenuExpandPopup_p1_b2.class: separator
*masterMenuExpandPopup_p1_b2.static: true
*masterMenuExpandPopup_p1_b2.name: masterMenuExpandPopup_p1_b2
*masterMenuExpandPopup_p1_b2.parent: masterMenuExpandPopup

*masterMenuExpandPopup_p1_b7.class: separator
*masterMenuExpandPopup_p1_b7.static: true
*masterMenuExpandPopup_p1_b7.name: masterMenuExpandPopup_p1_b7
*masterMenuExpandPopup_p1_b7.parent: masterMenuExpandPopup

*masterMenuPopupExpandpb.class: pushButton
*masterMenuPopupExpandpb.static: true
*masterMenuPopupExpandpb.name: masterMenuPopupExpandpb
*masterMenuPopupExpandpb.parent: masterMenuExpandPopup
*masterMenuPopupExpandpb.labelString: "Expand interface"
*masterMenuPopupExpandpb.activateCallback: {\
expandInterface (masterMenu);\
}

*masterMenuExpandPopup_p1_b3.class: separator
*masterMenuExpandPopup_p1_b3.static: true
*masterMenuExpandPopup_p1_b3.name: masterMenuExpandPopup_p1_b3
*masterMenuExpandPopup_p1_b3.parent: masterMenuExpandPopup

*masterMenuExpandPopupSpeed.class: pushButton
*masterMenuExpandPopupSpeed.static: true
*masterMenuExpandPopupSpeed.name: masterMenuExpandPopupSpeed
*masterMenuExpandPopupSpeed.parent: masterMenuExpandPopup
*masterMenuExpandPopupSpeed.labelString: SetSpeedLabel
*masterMenuExpandPopupSpeed.activateCallback: {\
#ifndef DESIGN_TIME\
richiesta_velocita (UxGetWidget(masterMenu));\
#endif\
}

*masterMenuExpandPopupMaxTime.class: pushButton
*masterMenuExpandPopupMaxTime.static: true
*masterMenuExpandPopupMaxTime.name: masterMenuExpandPopupMaxTime
*masterMenuExpandPopupMaxTime.parent: masterMenuExpandPopup
*masterMenuExpandPopupMaxTime.labelString: MaxTimeLabel
*masterMenuExpandPopupMaxTime.activateCallback: richiesta_maxtime (UxGetWidget(masterMenu));

*masterMenuExpandPopup_p1_b8.class: separator
*masterMenuExpandPopup_p1_b8.static: true
*masterMenuExpandPopup_p1_b8.name: masterMenuExpandPopup_p1_b8
*masterMenuExpandPopup_p1_b8.parent: masterMenuExpandPopup

*masterMenuExpandPopupShutdown.class: pushButton
*masterMenuExpandPopupShutdown.static: true
*masterMenuExpandPopupShutdown.name: masterMenuExpandPopupShutdown
*masterMenuExpandPopupShutdown.parent: masterMenuExpandPopup
*masterMenuExpandPopupShutdown.labelString: ShutdownLabel
*masterMenuExpandPopupShutdown.activateCallback: simulatorShutdown (masterMenu);

*masterKeyboardTasti.class: form
*masterKeyboardTasti.static: true
*masterKeyboardTasti.name: masterKeyboardTasti
*masterKeyboardTasti.parent: masterKeyboardForm
*masterKeyboardTasti.resizePolicy: "resize_any"
*masterKeyboardTasti.x: 0
*masterKeyboardTasti.y: 25
*masterKeyboardTasti.width: 340
*masterKeyboardTasti.height: 20
*masterKeyboardTasti.leftOffset: 0
*masterKeyboardTasti.bottomAttachment: "attach_form"
*masterKeyboardTasti.rubberPositioning: "false"

*masterKeyboardRunpb.class: pushButton
*masterKeyboardRunpb.static: true
*masterKeyboardRunpb.name: masterKeyboardRunpb
*masterKeyboardRunpb.parent: masterKeyboardTasti
*masterKeyboardRunpb.x: 0
*masterKeyboardRunpb.y: 15
*masterKeyboardRunpb.width: 70
*masterKeyboardRunpb.height: 20
*masterKeyboardRunpb.labelString: RUN_LABEL 
*masterKeyboardRunpb.activateCallback: {\
switch_to_run(masterMenu);\
}
*masterKeyboardRunpb.fontList: "-Adobe-Courier-Bold-R-Normal--17-120-100-100-M-100-ISO8859-1"
*masterKeyboardRunpb.foreground: "black"
*masterKeyboardRunpb.activateCallbackClientData: UxGetWidget(masterMenu)
*masterKeyboardRunpb.highlightOnEnter: "false"
*masterKeyboardRunpb.createManaged: "true"
*masterKeyboardRunpb.sensitive: "false"
*masterKeyboardRunpb.bottomAttachment: "attach_form"
*masterKeyboardRunpb.topOffset: 0
*masterKeyboardRunpb.topAttachment: "attach_form"

*masterKeyboardFreezepb.class: pushButton
*masterKeyboardFreezepb.static: true
*masterKeyboardFreezepb.name: masterKeyboardFreezepb
*masterKeyboardFreezepb.parent: masterKeyboardTasti
*masterKeyboardFreezepb.x: 70
*masterKeyboardFreezepb.y: 15
*masterKeyboardFreezepb.width: 70
*masterKeyboardFreezepb.height: 20
*masterKeyboardFreezepb.labelString: FREEZE_LABEL
*masterKeyboardFreezepb.activateCallback: {\
execute_freeze_button ((Widget)UxClientData);\
}
*masterKeyboardFreezepb.fontList: "-Adobe-Courier-Bold-R-Normal--17-120-100-100-M-100-ISO8859-1"
*masterKeyboardFreezepb.activateCallbackClientData: UxGetWidget(masterMenu)
*masterKeyboardFreezepb.multiClick: "multiclick_discard"
*masterKeyboardFreezepb.recomputeSize: "false"
*masterKeyboardFreezepb.createManaged: "true"
*masterKeyboardFreezepb.sensitive: "false"
*masterKeyboardFreezepb.bottomAttachment: "attach_form"
*masterKeyboardFreezepb.topOffset: 0
*masterKeyboardFreezepb.topAttachment: "attach_form"
*masterKeyboardFreezepb.leftAttachment: "attach_widget"
*masterKeyboardFreezepb.leftOffset: 0
*masterKeyboardFreezepb.leftWidget: "masterKeyboardRunpb"

*masterKeyboardSnappb.class: pushButton
*masterKeyboardSnappb.static: true
*masterKeyboardSnappb.name: masterKeyboardSnappb
*masterKeyboardSnappb.parent: masterKeyboardTasti
*masterKeyboardSnappb.x: 138
*masterKeyboardSnappb.y: 0
*masterKeyboardSnappb.width: 30
*masterKeyboardSnappb.height: 20
*masterKeyboardSnappb.labelString: "S"
*masterKeyboardSnappb.activateCallback: {\
printf ("richiesta snapshot\n");\
#ifndef DESIGN_TIME\
if (snap_interface_active) \
   { \
   if (!exist_Widget(UxGetWidget(initialCondition)))\
      create_initialCondition(UxGetWidget(masterMenu));\
   }\
else\
   create_initialCondition(UxGetWidget(masterMenu)); \
#endif\
}
*masterKeyboardSnappb.bottomAttachment: "attach_form"
*masterKeyboardSnappb.topOffset: 0
*masterKeyboardSnappb.topAttachment: "attach_form"
*masterKeyboardSnappb.leftAttachment: "attach_widget"
*masterKeyboardSnappb.leftOffset: 0
*masterKeyboardSnappb.leftWidget: "masterKeyboardFreezepb"

*masterKeyboardBtpb.class: pushButton
*masterKeyboardBtpb.static: true
*masterKeyboardBtpb.name: masterKeyboardBtpb
*masterKeyboardBtpb.parent: masterKeyboardTasti
*masterKeyboardBtpb.x: 174
*masterKeyboardBtpb.y: 0
*masterKeyboardBtpb.width: 30
*masterKeyboardBtpb.height: 20
*masterKeyboardBtpb.labelString: "B"
*masterKeyboardBtpb.activateCallback: {\
printf ("registrazioni di backtrack\n");\
#ifndef DESIGN_TIME\
   if (bt_interface_active)\
      {\
      if (!exist_Widget(UxGetWidget(backtrackManagement)))\
         create_backtrackManagement(UxGetWidget(masterMenu));\
      }\
   else\
      create_backtrackManagement(UxGetWidget(masterMenu));\
#endif\
}
*masterKeyboardBtpb.bottomAttachment: "attach_form"
*masterKeyboardBtpb.topOffset: 0
*masterKeyboardBtpb.topAttachment: "attach_form"
*masterKeyboardBtpb.leftAttachment: "attach_widget"
*masterKeyboardBtpb.leftOffset: 0
*masterKeyboardBtpb.leftWidget: "masterKeyboardSnappb"

*masterKeyboardGrafpb.class: pushButton
*masterKeyboardGrafpb.static: true
*masterKeyboardGrafpb.name: masterKeyboardGrafpb
*masterKeyboardGrafpb.parent: masterKeyboardTasti
*masterKeyboardGrafpb.x: 210
*masterKeyboardGrafpb.y: 0
*masterKeyboardGrafpb.width: 30
*masterKeyboardGrafpb.height: 20
*masterKeyboardGrafpb.labelString: "G"
*masterKeyboardGrafpb.createManaged: "true"
*masterKeyboardGrafpb.activateCallback: {\
strcpy (defaultDisplay,create_processStart(masterMenu,"GRAPHICS"));\
if (!strcmp(defaultDisplay,"NO"))\
   return;\
attiva_graphics(UxGetWidget(masterMenu),defaultDisplay);\
}
*masterKeyboardGrafpb.bottomAttachment: "attach_form"
*masterKeyboardGrafpb.topOffset: 0
*masterKeyboardGrafpb.topAttachment: "attach_form"
*masterKeyboardGrafpb.leftAttachment: "attach_widget"
*masterKeyboardGrafpb.leftOffset: 0
*masterKeyboardGrafpb.leftWidget: "masterKeyboardBtpb"

*masterKeyboardAingpb.class: pushButton
*masterKeyboardAingpb.static: true
*masterKeyboardAingpb.name: masterKeyboardAingpb
*masterKeyboardAingpb.parent: masterKeyboardTasti
*masterKeyboardAingpb.x: 246
*masterKeyboardAingpb.y: 0
*masterKeyboardAingpb.width: 30
*masterKeyboardAingpb.height: 20
*masterKeyboardAingpb.labelString: "I"
*masterKeyboardAingpb.activateCallback: {\
if (!exist_Widget (selVarAing))\
   selVarAing = create_selVar (masterMenu, NULL,AING_CALLING,AING_SELECTION);\
printf (" chiamata da aing [%s]\n",AING_SELECTION);\
}
*masterKeyboardAingpb.bottomAttachment: "attach_form"
*masterKeyboardAingpb.topOffset: 0
*masterKeyboardAingpb.topAttachment: "attach_form"
*masterKeyboardAingpb.leftAttachment: "attach_widget"
*masterKeyboardAingpb.leftOffset: 0
*masterKeyboardAingpb.leftWidget: "masterKeyboardGrafpb"

*masterKeyboardMalfpb.class: pushButton
*masterKeyboardMalfpb.static: true
*masterKeyboardMalfpb.name: masterKeyboardMalfpb
*masterKeyboardMalfpb.parent: masterKeyboardTasti
*masterKeyboardMalfpb.x: 276
*masterKeyboardMalfpb.y: 0
*masterKeyboardMalfpb.width: 30
*masterKeyboardMalfpb.height: 20
*masterKeyboardMalfpb.labelString: "MF"
*masterKeyboardMalfpb.activateCallback: {\
attiva_lista_scenari(masterMenu,MALFUNZIONI);\
attiva_sommario (masterMenu,SOMMARIO_MALF);\
}
*masterKeyboardMalfpb.createManaged: "true"
*masterKeyboardMalfpb.bottomAttachment: "attach_form"
*masterKeyboardMalfpb.topOffset: 0
*masterKeyboardMalfpb.topAttachment: "attach_form"
*masterKeyboardMalfpb.leftAttachment: "attach_widget"
*masterKeyboardMalfpb.leftOffset: 0
*masterKeyboardMalfpb.leftWidget: "masterKeyboardAingpb"

*masterKeyboardHmipb.class: pushButton
*masterKeyboardHmipb.static: true
*masterKeyboardHmipb.name: masterKeyboardHmipb
*masterKeyboardHmipb.parent: masterKeyboardTasti
*masterKeyboardHmipb.x: 306
*masterKeyboardHmipb.y: 0
*masterKeyboardHmipb.width: 30
*masterKeyboardHmipb.height: 20
*masterKeyboardHmipb.labelString: "H"
*masterKeyboardHmipb.activateCallback: {\
attiva_lghmi(UxGetWidget(masterMenu));\
}
*masterKeyboardHmipb.createManaged: "true"
*masterKeyboardHmipb.bottomAttachment: "attach_form"
*masterKeyboardHmipb.topOffset: 0
*masterKeyboardHmipb.topAttachment: "attach_form"
*masterKeyboardHmipb.leftAttachment: "attach_widget"
*masterKeyboardHmipb.leftOffset: 0
*masterKeyboardHmipb.leftWidget: "masterKeyboardMalfpb"

*masterKeyboardRemFuncpb.class: pushButton
*masterKeyboardRemFuncpb.static: true
*masterKeyboardRemFuncpb.name: masterKeyboardRemFuncpb
*masterKeyboardRemFuncpb.parent: masterKeyboardTasti
*masterKeyboardRemFuncpb.x: 306
*masterKeyboardRemFuncpb.y: 0
*masterKeyboardRemFuncpb.width: 30
*masterKeyboardRemFuncpb.height: 20
*masterKeyboardRemFuncpb.labelString: "RF"
*masterKeyboardRemFuncpb.createManaged: "false"
*masterKeyboardRemFuncpb.activateCallback: {\
attiva_lista_scenari(masterMenu,FUNZIONI_REMOTE);\
attiva_sommario (masterMenu,SOMMARIO_FREM);\
}
*masterKeyboardRemFuncpb.bottomAttachment: "attach_form"
*masterKeyboardRemFuncpb.topOffset: 0
*masterKeyboardRemFuncpb.topAttachment: "attach_form"
*masterKeyboardRemFuncpb.leftAttachment: "attach_widget"
*masterKeyboardRemFuncpb.leftOffset: 0
*masterKeyboardRemFuncpb.leftWidget: "masterKeyboardMalfpb"

*masterKeyboardDati.class: form
*masterKeyboardDati.static: true
*masterKeyboardDati.name: masterKeyboardDati
*masterKeyboardDati.parent: masterKeyboardForm
*masterKeyboardDati.resizePolicy: "resize_any"
*masterKeyboardDati.x: 0
*masterKeyboardDati.y: 0
*masterKeyboardDati.width: 340
*masterKeyboardDati.height: 25
*masterKeyboardDati.bottomAttachment: "attach_widget"
*masterKeyboardDati.bottomWidget: "masterKeyboardTasti"
*masterKeyboardDati.leftOffset: 0
*masterKeyboardDati.topAttachment: "attach_form"
*masterKeyboardDati.leftAttachment: "attach_form"
*masterKeyboardDati.topOffset: 0
*masterKeyboardDati.rubberPositioning: "false"

*masterKeyboardStatusFrame.class: frame
*masterKeyboardStatusFrame.static: true
*masterKeyboardStatusFrame.name: masterKeyboardStatusFrame
*masterKeyboardStatusFrame.parent: masterKeyboardDati
*masterKeyboardStatusFrame.x: 2
*masterKeyboardStatusFrame.y: 0
*masterKeyboardStatusFrame.width: 130
*masterKeyboardStatusFrame.height: 20
*masterKeyboardStatusFrame.borderWidth: 2
*masterKeyboardStatusFrame.bottomAttachment: "attach_form"
*masterKeyboardStatusFrame.leftOffset: 0
*masterKeyboardStatusFrame.topAttachment: "attach_form"

*masterKeyboardStatus.class: label
*masterKeyboardStatus.static: true
*masterKeyboardStatus.name: masterKeyboardStatus
*masterKeyboardStatus.parent: masterKeyboardStatusFrame
*masterKeyboardStatus.x: 4
*masterKeyboardStatus.y: 4
*masterKeyboardStatus.width: 136
*masterKeyboardStatus.height: 14
*masterKeyboardStatus.highlightThickness: 0
*masterKeyboardStatus.labelString: ""
*masterKeyboardStatus.shadowThickness: 3
*masterKeyboardStatus.borderWidth: 0
*masterKeyboardStatus.marginHeight: 0
*masterKeyboardStatus.marginWidth: 0
*masterKeyboardStatus.recomputeSize: "false"

*masterKeyboardTime.class: label
*masterKeyboardTime.static: true
*masterKeyboardTime.name: masterKeyboardTime
*masterKeyboardTime.parent: masterKeyboardDati
*masterKeyboardTime.x: 140
*masterKeyboardTime.y: 0
*masterKeyboardTime.width: 100
*masterKeyboardTime.height: 20
*masterKeyboardTime.labelString: ""
*masterKeyboardTime.alignment: "alignment_end"
*masterKeyboardTime.topOffset: 0
*masterKeyboardTime.bottomAttachment: "attach_form"
*masterKeyboardTime.leftAttachment: "attach_widget"
*masterKeyboardTime.leftOffset: 0
*masterKeyboardTime.leftWidget: "masterKeyboardStatusFrame"
*masterKeyboardTime.topAttachment: "attach_form"

*expandedMainForm.class: form
*expandedMainForm.static: true
*expandedMainForm.name: expandedMainForm
*expandedMainForm.parent: masterMenu
*expandedMainForm.resizePolicy: "resize_none"
*expandedMainForm.x: 0
*expandedMainForm.y: 10
*expandedMainForm.width: 680
*expandedMainForm.height: 160
*expandedMainForm.topOffset: 0
*expandedMainForm.rightAttachment: "attach_form"
*expandedMainForm.leftAttachment: "attach_form"
*expandedMainForm.topAttachment: "attach_form"
*expandedMainForm.bottomAttachment: "attach_form"
*expandedMainForm.bottomWidget: ""

*commandMainForm.class: form
*commandMainForm.static: true
*commandMainForm.name: commandMainForm
*commandMainForm.parent: expandedMainForm
*commandMainForm.resizePolicy: "resize_none"
*commandMainForm.x: 0
*commandMainForm.y: 0
*commandMainForm.width: 800
*commandMainForm.height: 100
*commandMainForm.rightAttachment: "attach_form"
*commandMainForm.leftAttachment: "attach_form"
*commandMainForm.topAttachment: "attach_form"

*master_menu.class: rowColumn
*master_menu.static: true
*master_menu.name: master_menu
*master_menu.parent: commandMainForm
*master_menu.rowColumnType: "menu_bar"
*master_menu.menuAccelerator: "<KeyUp>F10"
*master_menu.labelString: ""
*master_menu.orientation: "horizontal"
*master_menu.menuHelpWidget: "master_menu_help"
*master_menu.y: 0
*master_menu.x: 0
*master_menu.rightAttachment: "attach_form"
*master_menu.leftAttachment: "attach_form"
*master_menu.topOffset: 0

*FileMenuPb.class: rowColumn
*FileMenuPb.static: true
*FileMenuPb.name: FileMenuPb
*FileMenuPb.parent: master_menu
*FileMenuPb.rowColumnType: "menu_pulldown"

*FileMenuInitIcpb.class: pushButton
*FileMenuInitIcpb.static: true
*FileMenuInitIcpb.name: FileMenuInitIcpb
*FileMenuInitIcpb.parent: FileMenuPb
*FileMenuInitIcpb.labelString: InitIcLabel
*FileMenuInitIcpb.activateCallback: {\
#ifndef DESIGN_TIME\
if (snap_interface_active) \
   { \
   if (!exist_Widget(UxGetWidget(initialCondition)))\
      create_initialCondition(UxGetWidget(masterMenu));\
   }\
else\
   create_initialCondition(UxGetWidget(masterMenu)); \
#endif\
}

*FileMenuInitBtpb.class: pushButton
*FileMenuInitBtpb.static: true
*FileMenuInitBtpb.name: FileMenuInitBtpb
*FileMenuInitBtpb.parent: FileMenuPb
*FileMenuInitBtpb.labelString: InitBtLabel
*FileMenuInitBtpb.activateCallback: {\
#ifndef DESIGN_TIME\
   if (bt_interface_active)\
      {\
      if (!exist_Widget(UxGetWidget(backtrackManagement)))\
         create_backtrackManagement(UxGetWidget(masterMenu));\
      }\
   else\
      create_backtrackManagement(UxGetWidget(masterMenu));\
#endif\
}

*init_ci_menu_pb.class: pushButton
*init_ci_menu_pb.static: true
*init_ci_menu_pb.name: init_ci_menu_pb
*init_ci_menu_pb.parent: FileMenuPb
*init_ci_menu_pb.labelString: "Init from I.C. ..."
*init_ci_menu_pb.activateCallback: {\
printf ("init da ci **********\n");\
#ifndef DESIGN_TIME\
if (init_ci_interface_active) \
   { \
   if (!exist_Widget(UxGetWidget(initSimCi)))\
   create_initSimulator (UxGetWidget(masterMenu), INIT_FROM_CI);\
   }\
else\
   create_initSimulator (UxGetWidget(masterMenu), INIT_FROM_CI);\
#endif\
}

*init_bt_menu_pb.class: pushButton
*init_bt_menu_pb.static: true
*init_bt_menu_pb.name: init_bt_menu_pb
*init_bt_menu_pb.parent: FileMenuPb
*init_bt_menu_pb.labelString: "Init from BT  ..."
*init_bt_menu_pb.activateCallback: {\
printf ("init da backtrack ********\n");\
#ifndef DESIGN_TIME\
if (init_bt_interface_active) \
   { \
   if (!exist_Widget(UxGetWidget(initSimBt)))\
   create_initSimulator (UxGetWidget(masterMenu), INIT_FROM_BT);\
   }\
else\
   create_initSimulator (UxGetWidget(masterMenu), INIT_FROM_BT);\
#endif\
}\


*init_menu_pb_b5.class: separator
*init_menu_pb_b5.static: true
*init_menu_pb_b5.name: init_menu_pb_b5
*init_menu_pb_b5.parent: FileMenuPb

*init_menu_pb_b9.class: pushButton
*init_menu_pb_b9.static: true
*init_menu_pb_b9.name: init_menu_pb_b9
*init_menu_pb_b9.parent: FileMenuPb
*init_menu_pb_b9.labelString: StepScalingLabel
*init_menu_pb_b9.activateCallback: {\
richiesta_stepscale (UxGetWidget(masterMenu));\
}

*init_speed_menu_pb.class: pushButton
*init_speed_menu_pb.static: true
*init_speed_menu_pb.name: init_speed_menu_pb
*init_speed_menu_pb.parent: FileMenuPb
*init_speed_menu_pb.labelString: SetSpeedLabel
*init_speed_menu_pb.activateCallback: {\
#ifndef DESIGN_TIME\
richiesta_velocita (UxGetWidget(masterMenu));\
#endif\
}

*init_maxspeed_menu_pb.class: pushButton
*init_maxspeed_menu_pb.static: true
*init_maxspeed_menu_pb.name: init_maxspeed_menu_pb
*init_maxspeed_menu_pb.parent: FileMenuPb
*init_maxspeed_menu_pb.labelString: MaxSpeedLabel
*init_maxspeed_menu_pb.activateCallback: {\
set_max_speed (UxGetWidget(masterMenu));\
}

*init_menu_pb_b6.class: separator
*init_menu_pb_b6.static: true
*init_menu_pb_b6.name: init_menu_pb_b6
*init_menu_pb_b6.parent: FileMenuPb

*init_MaxTimepb.class: pushButton
*init_MaxTimepb.static: true
*init_MaxTimepb.name: init_MaxTimepb
*init_MaxTimepb.parent: FileMenuPb
*init_MaxTimepb.labelString: MaxTimeLabel
*init_MaxTimepb.activateCallback: richiesta_maxtime (UxGetWidget(masterMenu));

*init_menu_pb_b10.class: separator
*init_menu_pb_b10.static: true
*init_menu_pb_b10.name: init_menu_pb_b10
*init_menu_pb_b10.parent: FileMenuPb

*init_clear_menu_pb.class: pushButton
*init_clear_menu_pb.static: true
*init_clear_menu_pb.name: init_clear_menu_pb
*init_clear_menu_pb.parent: FileMenuPb
*init_clear_menu_pb.labelString: ClearLabel
*init_clear_menu_pb.mnemonic: "C"
*init_clear_menu_pb.activateCallback: {\
#ifndef DESIGN_TIME\
recording_reset (UxGetWidget(masterMenu));\
#endif\
}
*init_clear_menu_pb.createManaged: "true"
*init_clear_menu_pb.sensitive: "false"

*FileMenuPb_b11.class: separator
*FileMenuPb_b11.static: true
*FileMenuPb_b11.name: FileMenuPb_b11
*FileMenuPb_b11.parent: FileMenuPb

*FileMenuShutdownpb.class: pushButton
*FileMenuShutdownpb.static: true
*FileMenuShutdownpb.name: FileMenuShutdownpb
*FileMenuShutdownpb.parent: FileMenuPb
*FileMenuShutdownpb.labelString: ShutdownLabel
*FileMenuShutdownpb.activateCallback: simulatorShutdown (masterMenu);

*recording_menu_pb.class: rowColumn
*recording_menu_pb.static: true
*recording_menu_pb.name: recording_menu_pb
*recording_menu_pb.parent: master_menu
*recording_menu_pb.rowColumnType: "menu_pulldown"

*recording_cr_menu_cb.class: cascadeButton
*recording_cr_menu_cb.static: true
*recording_cr_menu_cb.name: recording_cr_menu_cb
*recording_cr_menu_cb.parent: recording_menu_pb
*recording_cr_menu_cb.labelString: "Cont. recording"
*recording_cr_menu_cb.subMenuId: "contRecPane"

*recording_ITAL_menu_pb.class: pushButton
*recording_ITAL_menu_pb.static: true
*recording_ITAL_menu_pb.name: recording_ITAL_menu_pb
*recording_ITAL_menu_pb.parent: recording_menu_pb
*recording_ITAL_menu_pb.labelString: "Instructor & Trainee A.L."
*recording_ITAL_menu_pb.mnemonic: "I"
*recording_ITAL_menu_pb.activateCallback: {printf ("I&T al\n");}

*recording_perform_menu_pb.class: pushButton
*recording_perform_menu_pb.static: true
*recording_perform_menu_pb.name: recording_perform_menu_pb
*recording_perform_menu_pb.parent: recording_menu_pb
*recording_perform_menu_pb.labelString: "Performance analisys"
*recording_perform_menu_pb.mnemonic: "P"
*recording_perform_menu_pb.activateCallback: {printf ("perf. analisys\n");}

*recording_sep1_menu.class: separatorGadget
*recording_sep1_menu.static: true
*recording_sep1_menu.name: recording_sep1_menu
*recording_sep1_menu.parent: recording_menu_pb

*recording_archivio_menu_cb.class: cascadeButton
*recording_archivio_menu_cb.static: true
*recording_archivio_menu_cb.name: recording_archivio_menu_cb
*recording_archivio_menu_cb.parent: recording_menu_pb
*recording_archivio_menu_cb.labelString: "Archivio"
*recording_archivio_menu_cb.subMenuId: "recording_menu_archivio_pane"

*contRecPane.class: rowColumn
*contRecPane.static: true
*contRecPane.name: contRecPane
*contRecPane.parent: recording_menu_pb
*contRecPane.rowColumnType: "menu_pulldown"

*contRecMenuSelpb.class: pushButton
*contRecMenuSelpb.static: true
*contRecMenuSelpb.name: contRecMenuSelpb
*contRecMenuSelpb.parent: contRecPane
*contRecMenuSelpb.labelString: "Select variables"
*contRecMenuSelpb.activateCallback: {\
printf ("richiesta selezione cr\n");\
#ifndef DESIGN_TIME\
if (sel_var_cr_interface_active) \
   { \
   if (!exist_Widget (UxGetWidget(selVarCr)))\
      create_selVarCr (UxGetWidget(masterMenu));\
   }\
else\
   create_selVarCr (UxGetWidget(masterMenu)); \
#endif\
}

*master_menu_p12_b2.class: separator
*master_menu_p12_b2.static: true
*master_menu_p12_b2.name: master_menu_p12_b2
*master_menu_p12_b2.parent: contRecPane

*contRecMenuStartpb.class: pushButton
*contRecMenuStartpb.static: true
*contRecMenuStartpb.name: contRecMenuStartpb
*contRecMenuStartpb.parent: contRecPane
*contRecMenuStartpb.labelString: "View recorded data"
*contRecMenuStartpb.activateCallback: {\
strcpy (defaultDisplay,create_processStart(masterMenu,"GRAPHICS"));\
if (!strcmp(defaultDisplay,"NO"))\
   return;\
attiva_graphics(UxGetWidget(masterMenu),defaultDisplay);\
}

*recording_menu_archivio_pane.class: rowColumn
*recording_menu_archivio_pane.static: true
*recording_menu_archivio_pane.name: recording_menu_archivio_pane
*recording_menu_archivio_pane.parent: recording_menu_pb
*recording_menu_archivio_pane.rowColumnType: "menu_pulldown"

*rec_menu_arch_save_pb.class: pushButton
*rec_menu_arch_save_pb.static: true
*rec_menu_arch_save_pb.name: rec_menu_arch_save_pb
*rec_menu_arch_save_pb.parent: recording_menu_archivio_pane
*rec_menu_arch_save_pb.labelString: "Save   ...  "
*rec_menu_arch_save_pb.activateCallback: {\
printf ("save archivio\n");\
#ifndef DESIGN_TIME\
if (archsave_interface_active)\
   {\
   if (!exist_Widget(UxGetWidget(archiveSavereq)))\
      create_archiveSavereq();\
   }\
else\
   create_archiveSavereq();\
#endif\
}

*recording_menu_archivio_pane_b2.class: separator
*recording_menu_archivio_pane_b2.static: true
*recording_menu_archivio_pane_b2.name: recording_menu_archivio_pane_b2
*recording_menu_archivio_pane_b2.parent: recording_menu_archivio_pane

*rec_menu_arch_edit_pb.class: pushButton
*rec_menu_arch_edit_pb.static: true
*rec_menu_arch_edit_pb.name: rec_menu_arch_edit_pb
*rec_menu_arch_edit_pb.parent: recording_menu_archivio_pane
*rec_menu_arch_edit_pb.labelString: "Edit   ..."
*rec_menu_arch_edit_pb.activateCallback: {\
#ifndef DESIGN_TIME\
printf ("editazione archivio\n");\
create_archiveEdit();\
#endif\
}

*mffr_menu_pb.class: rowColumn
*mffr_menu_pb.static: true
*mffr_menu_pb.name: mffr_menu_pb
*mffr_menu_pb.parent: master_menu
*mffr_menu_pb.rowColumnType: "menu_pulldown"

*mffr_menu_pb_b1.class: cascadeButton
*mffr_menu_pb_b1.static: true
*mffr_menu_pb_b1.name: mffr_menu_pb_b1
*mffr_menu_pb_b1.parent: mffr_menu_pb
*mffr_menu_pb_b1.labelString: MALF
*mffr_menu_pb_b1.subMenuId: "malf_pane"

*mffr_menu_pb_b3.class: separator
*mffr_menu_pb_b3.static: true
*mffr_menu_pb_b3.name: mffr_menu_pb_b3
*mffr_menu_pb_b3.parent: mffr_menu_pb

*mffr_menu_pb_b2.class: cascadeButton
*mffr_menu_pb_b2.static: true
*mffr_menu_pb_b2.name: mffr_menu_pb_b2
*mffr_menu_pb_b2.parent: mffr_menu_pb
*mffr_menu_pb_b2.labelString: FREM
*mffr_menu_pb_b2.subMenuId: "frem_pane"
*mffr_menu_pb_b2.createManaged: "false"

*malf_pane.class: rowColumn
*malf_pane.static: true
*malf_pane.name: malf_pane
*malf_pane.parent: mffr_menu_pb
*malf_pane.rowColumnType: "menu_pulldown"

*mffr_malf_scenpb.class: pushButton
*mffr_malf_scenpb.static: true
*mffr_malf_scenpb.name: mffr_malf_scenpb
*mffr_malf_scenpb.parent: malf_pane
*mffr_malf_scenpb.labelString: MALF_SCEN_LIST
*mffr_malf_scenpb.activateCallback: attiva_lista_scenari(masterMenu,MALFUNZIONI);\


*malf_pane_b3.class: separatorGadget
*malf_pane_b3.static: true
*malf_pane_b3.name: malf_pane_b3
*malf_pane_b3.parent: malf_pane

*mffr_malf_somm_pb.class: pushButton
*mffr_malf_somm_pb.static: true
*mffr_malf_somm_pb.name: mffr_malf_somm_pb
*mffr_malf_somm_pb.parent: malf_pane
*mffr_malf_somm_pb.labelString: MALF_LIST
*mffr_malf_somm_pb.activateCallback: {\
attiva_sommario (masterMenu,SOMMARIO_MALF);\
\
}

*frem_pane.class: rowColumn
*frem_pane.static: true
*frem_pane.name: frem_pane
*frem_pane.parent: mffr_menu_pb
*frem_pane.rowColumnType: "menu_pulldown"
*frem_pane.createManaged: "false"

*mffr_frem_scenpb.class: pushButton
*mffr_frem_scenpb.static: true
*mffr_frem_scenpb.name: mffr_frem_scenpb
*mffr_frem_scenpb.parent: frem_pane
*mffr_frem_scenpb.labelString: FREM_SCEN_LIST
*mffr_frem_scenpb.activateCallback: attiva_lista_scenari(masterMenu,FUNZIONI_REMOTE);

*master_menu_p11_b2.class: separatorGadget
*master_menu_p11_b2.static: true
*master_menu_p11_b2.name: master_menu_p11_b2
*master_menu_p11_b2.parent: frem_pane

*mffr_frem_sommpb.class: pushButton
*mffr_frem_sommpb.static: true
*mffr_frem_sommpb.name: mffr_frem_sommpb
*mffr_frem_sommpb.parent: frem_pane
*mffr_frem_sommpb.labelString: FREM_LIST
*mffr_frem_sommpb.activateCallback: {\
attiva_sommario (masterMenu, SOMMARIO_FREM);\
}

*show_menu_pb.class: rowColumn
*show_menu_pb.static: true
*show_menu_pb.name: show_menu_pb
*show_menu_pb.parent: master_menu
*show_menu_pb.rowColumnType: "menu_pulldown"

*show_statistic_menu_pb.class: pushButton
*show_statistic_menu_pb.static: true
*show_statistic_menu_pb.name: show_statistic_menu_pb
*show_statistic_menu_pb.parent: show_menu_pb
*show_statistic_menu_pb.labelString: "Statistic"
*show_statistic_menu_pb.mnemonic: "S"
*show_statistic_menu_pb.activateCallback: {\
if (!exist_Widget (statisticheDisplayTask))\
   statisticheDisplayTask = attiva_statistiche (UxGetWidget(masterMenu));\
}

*show_pert_pb.class: pushButton
*show_pert_pb.static: true
*show_pert_pb.name: show_pert_pb
*show_pert_pb.parent: show_menu_pb
*show_pert_pb.labelString: "Pert list"
*show_pert_pb.activateCallback: {\
#ifndef DESIGN_TIME\
if (edit_pert_interface_active)\
   {\
   if (!exist_Widget(UxGetWidget(editPert)))\
      create_editPert (UxGetWidget(masterMenu));\
   }\
else\
   create_editPert (UxGetWidget(masterMenu));\
#endif\
}

*show_proc_list.class: pushButton
*show_proc_list.static: true
*show_proc_list.name: show_proc_list
*show_proc_list.parent: show_menu_pb
*show_proc_list.labelString: "Process list"
*show_proc_list.activateCallback: {\
showProcessList (masterMenu);\
}

*show_param_menu_pb.class: pushButton
*show_param_menu_pb.static: true
*show_param_menu_pb.name: show_param_menu_pb
*show_param_menu_pb.parent: show_menu_pb
*show_param_menu_pb.labelString: "Parameters"
*show_param_menu_pb.mnemonic: "P"
*show_param_menu_pb.activateCallback: {printf ("richiesta show parametri\n");}
*show_param_menu_pb.createManaged: "false"

*help_menu_pb.class: rowColumn
*help_menu_pb.static: true
*help_menu_pb.name: help_menu_pb
*help_menu_pb.parent: master_menu
*help_menu_pb.rowColumnType: "menu_pulldown"

*help_1_menu_pb.class: pushButton
*help_1_menu_pb.static: true
*help_1_menu_pb.name: help_1_menu_pb
*help_1_menu_pb.parent: help_menu_pb
*help_1_menu_pb.labelString: "pushButton"
*help_1_menu_pb.activateCallback.source: public
*help_1_menu_pb.activateCallback: 

*prog_menu_pb.class: rowColumn
*prog_menu_pb.static: true
*prog_menu_pb.name: prog_menu_pb
*prog_menu_pb.parent: master_menu
*prog_menu_pb.rowColumnType: "menu_pulldown"

*masterMenuAingpb.class: pushButton
*masterMenuAingpb.static: true
*masterMenuAingpb.name: masterMenuAingpb
*masterMenuAingpb.parent: prog_menu_pb
*masterMenuAingpb.labelString: "Xaing    "
*masterMenuAingpb.activateCallback: {\
if (!exist_Widget (selVarAing))\
   selVarAing = create_selVar (masterMenu, NULL,AING_CALLING,AING_SELECTION);\
printf (" chiamata da aing [%s]\n",AING_SELECTION);\
}

*masterMenuUserProgpb.class: pushButton
*masterMenuUserProgpb.static: true
*masterMenuUserProgpb.name: masterMenuUserProgpb
*masterMenuUserProgpb.parent: prog_menu_pb
*masterMenuUserProgpb.labelString: "User programs ..."
*masterMenuUserProgpb.activateCallback: {\
printf ("programmi utente\n");\
create_programLauncher (masterMenu);\
}

*option_menu_pb.class: rowColumn
*option_menu_pb.static: true
*option_menu_pb.name: option_menu_pb
*option_menu_pb.parent: master_menu
*option_menu_pb.rowColumnType: "menu_pulldown"

*option_load_menu_pb.class: pushButton
*option_load_menu_pb.static: true
*option_load_menu_pb.name: option_load_menu_pb
*option_load_menu_pb.parent: option_menu_pb
*option_load_menu_pb.labelString: "Load      "
*option_load_menu_pb.activateCallback: {\
#ifndef DESIGN_TIME \
printf ("load Options\n");\
read_options();\
#endif\
}
*option_load_menu_pb.createManaged: "false"

*option_edit_menu_pb.class: pushButton
*option_edit_menu_pb.static: true
*option_edit_menu_pb.name: option_edit_menu_pb
*option_edit_menu_pb.parent: option_menu_pb
*option_edit_menu_pb.labelString: "Edit     "
*option_edit_menu_pb.activateCallback: {\
#ifndef DESIGN_TIME\
printf ("edit opzioni\n");\
if (opt_interface_active)\
   {\
   if (!exist_Widget(UxGetWidget(optionSet)))\
      create_optionSet();\
   }\
else\
   create_optionSet();\
#endif\
}

*archive_menu_pb.class: rowColumn
*archive_menu_pb.static: true
*archive_menu_pb.name: archive_menu_pb
*archive_menu_pb.parent: master_menu
*archive_menu_pb.rowColumnType: "menu_pulldown"

*archive_session.class: pushButton
*archive_session.static: true
*archive_session.name: archive_session
*archive_session.parent: archive_menu_pb
*archive_session.labelString: "Session      "
*archive_session.activateCallback: printf("Archiving session!!!\n");\
create_archiveSess(masterMenu);\


*master_menu_init.class: cascadeButton
*master_menu_init.static: true
*master_menu_init.name: master_menu_init
*master_menu_init.parent: master_menu
*master_menu_init.labelString: FileMenuLabel
*master_menu_init.subMenuId: "FileMenuPb"

*master_menu_recording.class: cascadeButton
*master_menu_recording.static: true
*master_menu_recording.name: master_menu_recording
*master_menu_recording.parent: master_menu
*master_menu_recording.labelString: "Recording"
*master_menu_recording.subMenuId: "recording_menu_pb"

*master_menu_mffr.class: cascadeButton
*master_menu_mffr.static: true
*master_menu_mffr.name: master_menu_mffr
*master_menu_mffr.parent: master_menu
*master_menu_mffr.labelString: "MF/FR"
*master_menu_mffr.subMenuId: "mffr_menu_pb"

*master_menu_show.class: cascadeButton
*master_menu_show.static: true
*master_menu_show.name: master_menu_show
*master_menu_show.parent: master_menu
*master_menu_show.labelString: "Show"
*master_menu_show.subMenuId: "show_menu_pb"

*master_menu_help.class: cascadeButton
*master_menu_help.name.source: public
*master_menu_help.static: false
*master_menu_help.name: master_menu_help
*master_menu_help.parent: master_menu
*master_menu_help.labelString: "Help"
*master_menu_help.subMenuId: "help_menu_pb"
*master_menu_help.alignment: "alignment_center"
*master_menu_help.sensitive: "true"
*master_menu_help.createManaged: "false"

*master_menu_top_b1.class: cascadeButton
*master_menu_top_b1.static: true
*master_menu_top_b1.name: master_menu_top_b1
*master_menu_top_b1.parent: master_menu
*master_menu_top_b1.labelString: "Programs"
*master_menu_top_b1.subMenuId: "prog_menu_pb"

*master_menu_options.class: cascadeButton
*master_menu_options.static: true
*master_menu_options.name: master_menu_options
*master_menu_options.parent: master_menu
*master_menu_options.labelString: "Options"
*master_menu_options.subMenuId: "option_menu_pb"

*master_menu_top_b2.class: cascadeButton
*master_menu_top_b2.static: true
*master_menu_top_b2.name: master_menu_top_b2
*master_menu_top_b2.parent: master_menu
*master_menu_top_b2.labelString: "Archiving"
*master_menu_top_b2.subMenuId: "archive_menu_pb"

*form2.class: form
*form2.static: true
*form2.name: form2
*form2.parent: commandMainForm
*form2.resizePolicy: "resize_none"
*form2.x: 5
*form2.y: 31
*form2.width: 295
*form2.height: 100
*form2.topAttachment: "attach_widget"
*form2.topOffset: 0
*form2.topWidget: "master_menu"
*form2.leftOffset: 0
*form2.bottomAttachment: "attach_form"

*masterMenuFreezepb.class: pushButton
*masterMenuFreezepb.name.source: public
*masterMenuFreezepb.static: false
*masterMenuFreezepb.name: masterMenuFreezepb
*masterMenuFreezepb.parent: form2
*masterMenuFreezepb.x: 70
*masterMenuFreezepb.y: 15
*masterMenuFreezepb.width: 70
*masterMenuFreezepb.height: 40
*masterMenuFreezepb.labelString: FREEZE_LABEL
*masterMenuFreezepb.activateCallback: {\
execute_freeze_button ((Widget)UxClientData);\
}
*masterMenuFreezepb.fontList: "-Adobe-Courier-Bold-R-Normal--17-120-100-100-M-100-ISO8859-1"
*masterMenuFreezepb.activateCallbackClientData: UxGetWidget(masterMenu)
*masterMenuFreezepb.multiClick: "multiclick_discard"
*masterMenuFreezepb.topOffset: 15
*masterMenuFreezepb.topAttachment: "attach_form"
*masterMenuFreezepb.recomputeSize: "false"
*masterMenuFreezepb.createManaged: "true"
*masterMenuFreezepb.sensitive: "false"

*masterMenuRunpb.class: pushButton
*masterMenuRunpb.name.source: public
*masterMenuRunpb.static: false
*masterMenuRunpb.name: masterMenuRunpb
*masterMenuRunpb.parent: form2
*masterMenuRunpb.x: 0
*masterMenuRunpb.y: 15
*masterMenuRunpb.width: 70
*masterMenuRunpb.height: 40
*masterMenuRunpb.labelString: RUN_LABEL 
*masterMenuRunpb.activateCallback: {\
switch_to_run(masterMenu);\
}
*masterMenuRunpb.fontList: "-Adobe-Courier-Bold-R-Normal--17-120-100-100-M-100-ISO8859-1"
*masterMenuRunpb.foreground: "black"
*masterMenuRunpb.activateCallbackClientData: UxGetWidget(masterMenu)
*masterMenuRunpb.highlightOnEnter: "false"
*masterMenuRunpb.bottomAttachment: "attach_none"
*masterMenuRunpb.bottomOffset: 20
*masterMenuRunpb.topAttachment: "attach_form"
*masterMenuRunpb.topOffset: 15
*masterMenuRunpb.createManaged: "true"
*masterMenuRunpb.sensitive: "false"

*labelStatusHead.class: label
*labelStatusHead.static: true
*labelStatusHead.name: labelStatusHead
*labelStatusHead.parent: form2
*labelStatusHead.x: 160
*labelStatusHead.y: 0
*labelStatusHead.width: 130
*labelStatusHead.height: 10
*labelStatusHead.topAttachment: "attach_form"
*labelStatusHead.leftAttachment: "attach_widget"
*labelStatusHead.leftOffset: 0
*labelStatusHead.leftWidget: "masterMenuFreezepb"
*labelStatusHead.rightAttachment: "attach_form"
*labelStatusHead.labelString: "Simulator status:"

*labelStatusFrame.class: frame
*labelStatusFrame.static: true
*labelStatusFrame.name: labelStatusFrame
*labelStatusFrame.parent: form2
*labelStatusFrame.x: 150
*labelStatusFrame.y: 20
*labelStatusFrame.width: 130
*labelStatusFrame.height: 30
*labelStatusFrame.bottomOffset: 20
*labelStatusFrame.leftAttachment: "attach_widget"
*labelStatusFrame.leftOffset: 0
*labelStatusFrame.leftWidget: "masterMenuFreezepb"
*labelStatusFrame.rightAttachment: "attach_form"
*labelStatusFrame.borderWidth: 2

*labelStatus.class: label
*labelStatus.static: true
*labelStatus.name: labelStatus
*labelStatus.parent: labelStatusFrame
*labelStatus.x: 140
*labelStatus.y: 20
*labelStatus.width: 151
*labelStatus.height: 30
*labelStatus.highlightThickness: 0
*labelStatus.labelString: ""
*labelStatus.shadowThickness: 3
*labelStatus.borderWidth: 0
*labelStatus.marginHeight: 0
*labelStatus.marginWidth: 0

*masterMenuData.class: form
*masterMenuData.static: true
*masterMenuData.name: masterMenuData
*masterMenuData.parent: commandMainForm
*masterMenuData.resizePolicy: "resize_none"
*masterMenuData.x: 300
*masterMenuData.y: 31
*masterMenuData.width: 350
*masterMenuData.height: 99
*masterMenuData.rightAttachment: "attach_form"
*masterMenuData.topAttachment: "attach_widget"
*masterMenuData.topOffset: 0
*masterMenuData.topWidget: "master_menu"
*masterMenuData.leftAttachment: "attach_widget"
*masterMenuData.leftWidget: "form2"
*masterMenuData.bottomAttachment: "attach_form"

*rowColumn1.class: rowColumn
*rowColumn1.static: true
*rowColumn1.name: rowColumn1
*rowColumn1.parent: masterMenuData
*rowColumn1.x: 0
*rowColumn1.y: 0
*rowColumn1.width: 190
*rowColumn1.height: 64
*rowColumn1.numColumns: 2
*rowColumn1.packing: "pack_column"
*rowColumn1.bottomAttachment: "attach_form"
*rowColumn1.topAttachment: "attach_form"
*rowColumn1.rightAttachment: "attach_position"
*rowColumn1.rightPosition: 60
*rowColumn1.leftAttachment: "attach_form"

*simTimelabel.class: label
*simTimelabel.static: true
*simTimelabel.name: simTimelabel
*simTimelabel.parent: rowColumn1
*simTimelabel.x: 3
*simTimelabel.y: 3
*simTimelabel.width: 77
*simTimelabel.height: 20
*simTimelabel.recomputeSize: "false"
*simTimelabel.labelString: SIMTIME_LABEL
*simTimelabel.alignment: "alignment_beginning"

*delayTimelabel1.class: label
*delayTimelabel1.static: true
*delayTimelabel1.name: delayTimelabel1
*delayTimelabel1.parent: rowColumn1
*delayTimelabel1.x: 0
*delayTimelabel1.y: 20
*delayTimelabel1.width: 77
*delayTimelabel1.height: 20
*delayTimelabel1.labelString: RITARDO_LABEL

*delayTimelabel2.class: label
*delayTimelabel2.static: true
*delayTimelabel2.name: delayTimelabel2
*delayTimelabel2.parent: rowColumn1
*delayTimelabel2.x: 0
*delayTimelabel2.y: 50
*delayTimelabel2.width: 77
*delayTimelabel2.height: 20
*delayTimelabel2.labelString: SPEED_LABEL

*simTimeDisplay.class: label
*simTimeDisplay.name.source: public
*simTimeDisplay.static: false
*simTimeDisplay.name: simTimeDisplay
*simTimeDisplay.parent: rowColumn1
*simTimeDisplay.x: 120
*simTimeDisplay.y: 3
*simTimeDisplay.width: 70
*simTimeDisplay.height: 20
*simTimeDisplay.labelString: ""
*simTimeDisplay.alignment: "alignment_end"

*delayTimeDisplay.class: label
*delayTimeDisplay.name.source: public
*delayTimeDisplay.static: false
*delayTimeDisplay.name: delayTimeDisplay
*delayTimeDisplay.parent: rowColumn1
*delayTimeDisplay.x: 120
*delayTimeDisplay.y: 28
*delayTimeDisplay.width: 70
*delayTimeDisplay.height: 20
*delayTimeDisplay.labelString: ""
*delayTimeDisplay.alignment: "alignment_end"

*speedDisplay.class: label
*speedDisplay.name.source: public
*speedDisplay.static: false
*speedDisplay.name: speedDisplay
*speedDisplay.parent: rowColumn1
*speedDisplay.x: 120
*speedDisplay.y: 53
*speedDisplay.width: 70
*speedDisplay.height: 20
*speedDisplay.labelString: ""
*speedDisplay.alignment: "alignment_end"

*tastieraMaster.class: bulletinBoard
*tastieraMaster.static: true
*tastieraMaster.name: tastieraMaster
*tastieraMaster.parent: masterMenuData
*tastieraMaster.resizePolicy: "resize_none"
*tastieraMaster.x: 200
*tastieraMaster.y: 0
*tastieraMaster.width: 150
*tastieraMaster.height: 70
*tastieraMaster.rightAttachment: "attach_form"
*tastieraMaster.bottomAttachment: "attach_form"
*tastieraMaster.leftAttachment: "attach_widget"
*tastieraMaster.leftWidget: "rowColumn1"

*tastieraHmipb.class: pushButton
*tastieraHmipb.static: true
*tastieraHmipb.name: tastieraHmipb
*tastieraHmipb.parent: tastieraMaster
*tastieraHmipb.x: 55
*tastieraHmipb.y: 40
*tastieraHmipb.width: 45
*tastieraHmipb.height: 25
*tastieraHmipb.labelString: "HMI"
*tastieraHmipb.activateCallback: {\
attiva_lghmi(UxGetWidget(masterMenu));\
}
*tastieraHmipb.createManaged: "true"

*tastieraButton5.class: pushButton
*tastieraButton5.static: true
*tastieraButton5.name: tastieraButton5
*tastieraButton5.parent: tastieraMaster
*tastieraButton5.x: 130
*tastieraButton5.y: 10
*tastieraButton5.width: 25
*tastieraButton5.height: 25
*tastieraButton5.labelString: "MF"
*tastieraButton5.activateCallback: {\
if(attiva_lista_scenari(masterMenu,MALFUNZIONI) >= 0)\
   attiva_sommario (masterMenu,SOMMARIO_MALF);\
}
*tastieraButton5.createManaged: "true"

*tastieraButton4.class: pushButton
*tastieraButton4.static: true
*tastieraButton4.name: tastieraButton4
*tastieraButton4.parent: tastieraMaster
*tastieraButton4.x: 100
*tastieraButton4.y: 10
*tastieraButton4.width: 25
*tastieraButton4.height: 25
*tastieraButton4.labelString: "I"
*tastieraButton4.activateCallback: {\
if (!exist_Widget (selVarAing))\
   selVarAing = create_selVar (masterMenu, NULL,AING_CALLING,AING_SELECTION);\
printf (" chiamata da aing [%s]\n",AING_SELECTION);\
}

*tastieraButton3.class: pushButton
*tastieraButton3.static: true
*tastieraButton3.name: tastieraButton3
*tastieraButton3.parent: tastieraMaster
*tastieraButton3.x: 70
*tastieraButton3.y: 10
*tastieraButton3.width: 25
*tastieraButton3.height: 25
*tastieraButton3.labelString: "G"
*tastieraButton3.createManaged: "true"
*tastieraButton3.activateCallback: {\
strcpy (defaultDisplay,create_processStart(masterMenu,"GRAPHICS"));\
if (!strcmp(defaultDisplay,"NO"))\
   return;\
attiva_graphics(UxGetWidget(masterMenu),defaultDisplay);\
}

*tastieraButton2.class: pushButton
*tastieraButton2.static: true
*tastieraButton2.name: tastieraButton2
*tastieraButton2.parent: tastieraMaster
*tastieraButton2.x: 40
*tastieraButton2.y: 10
*tastieraButton2.width: 25
*tastieraButton2.height: 25
*tastieraButton2.labelString: "B"
*tastieraButton2.activateCallback: {\
printf ("registrazioni di backtrack\n");\
#ifndef DESIGN_TIME\
   if (bt_interface_active)\
      {\
      if (!exist_Widget(UxGetWidget(backtrackManagement)))\
         create_backtrackManagement(UxGetWidget(masterMenu));\
      }\
   else\
      create_backtrackManagement(UxGetWidget(masterMenu));\
#endif\
}

*tastieraButton1.class: pushButton
*tastieraButton1.static: true
*tastieraButton1.name: tastieraButton1
*tastieraButton1.parent: tastieraMaster
*tastieraButton1.x: 0
*tastieraButton1.y: 10
*tastieraButton1.width: 25
*tastieraButton1.height: 25
*tastieraButton1.labelString: "S"
*tastieraButton1.activateCallback: {\
printf ("richiesta snapshot\n");\
#ifndef DESIGN_TIME\
if (snap_interface_active) \
   { \
   if (!exist_Widget(UxGetWidget(initialCondition)))\
      create_initialCondition(UxGetWidget(masterMenu));\
   }\
else\
   create_initialCondition(UxGetWidget(masterMenu)); \
#endif\
}

*tastieraButton10.class: pushButton
*tastieraButton10.static: true
*tastieraButton10.name: tastieraButton10
*tastieraButton10.parent: tastieraMaster
*tastieraButton10.x: 130
*tastieraButton10.y: 40
*tastieraButton10.width: 25
*tastieraButton10.height: 25
*tastieraButton10.labelString: "RF"
*tastieraButton10.createManaged: "false"
*tastieraButton10.activateCallback: {\
attiva_lista_scenari(masterMenu,FUNZIONI_REMOTE);\
attiva_sommario (masterMenu,SOMMARIO_FREM);\
}

*tastieraButton9.class: pushButton
*tastieraButton9.static: true
*tastieraButton9.name: tastieraButton9
*tastieraButton9.parent: tastieraMaster
*tastieraButton9.x: 100
*tastieraButton9.y: 40
*tastieraButton9.width: 25
*tastieraButton9.height: 25
*tastieraButton9.labelString: ""
*tastieraButton9.createManaged: "false"
*tastieraButton9.activateCallback: {\
create_resourceSetting (masterMenu);\
}

*tastieraButton8.class: pushButton
*tastieraButton8.static: true
*tastieraButton8.name: tastieraButton8
*tastieraButton8.parent: tastieraMaster
*tastieraButton8.x: 70
*tastieraButton8.y: 40
*tastieraButton8.width: 25
*tastieraButton8.height: 25
*tastieraButton8.labelString: ""
*tastieraButton8.createManaged: "false"

*tastieraButton7.class: pushButton
*tastieraButton7.static: true
*tastieraButton7.name: tastieraButton7
*tastieraButton7.parent: tastieraMaster
*tastieraButton7.x: 40
*tastieraButton7.y: 40
*tastieraButton7.width: 25
*tastieraButton7.height: 25
*tastieraButton7.labelString: ""
*tastieraButton7.createManaged: "false"

*tastieraButton6.class: pushButton
*tastieraButton6.static: true
*tastieraButton6.name: tastieraButton6
*tastieraButton6.parent: tastieraMaster
*tastieraButton6.x: 10
*tastieraButton6.y: 40
*tastieraButton6.width: 25
*tastieraButton6.height: 25
*tastieraButton6.labelString: "X"
*tastieraButton6.createManaged: "false"
*tastieraButton6.activateCallback: {\
switchDatabase();\
\
}

*masterMenuCompressPopup.class: rowColumn
*masterMenuCompressPopup.static: true
*masterMenuCompressPopup.name: masterMenuCompressPopup
*masterMenuCompressPopup.parent: commandMainForm
*masterMenuCompressPopup.rowColumnType: "menu_popup"
*masterMenuCompressPopup.menuAccelerator: "Shift <KeyUp>F10"

*masterMenuCompresspb.class: pushButton
*masterMenuCompresspb.static: true
*masterMenuCompresspb.name: masterMenuCompresspb
*masterMenuCompresspb.parent: masterMenuCompressPopup
*masterMenuCompresspb.labelString: "Compress interface"
*masterMenuCompresspb.activateCallback: {\
compressInterface (masterMenu);\
}

*messageForm.class: form
*messageForm.static: true
*messageForm.name: messageForm
*messageForm.parent: expandedMainForm
*messageForm.resizePolicy: "resize_none"
*messageForm.x: 0
*messageForm.y: 100
*messageForm.width: 700
*messageForm.height: 60
*messageForm.bottomAttachment: "attach_form"
*messageForm.rightAttachment: "attach_form"
*messageForm.topAttachment: "attach_widget"
*messageForm.leftAttachment: "attach_form"
*messageForm.topWidget: "commandMainForm"

*scrolledWindow1.class: scrolledWindow
*scrolledWindow1.static: true
*scrolledWindow1.name: scrolledWindow1
*scrolledWindow1.parent: messageForm
*scrolledWindow1.scrollingPolicy: "application_defined"
*scrolledWindow1.x: 5
*scrolledWindow1.y: 0
*scrolledWindow1.visualPolicy: "variable"
*scrolledWindow1.scrollBarDisplayPolicy: "static"
*scrolledWindow1.shadowThickness: 0
*scrolledWindow1.rightAttachment: "attach_form"
*scrolledWindow1.leftAttachment: "attach_form"
*scrolledWindow1.bottomAttachment: "attach_form"
*scrolledWindow1.topAttachment: "attach_form"
*scrolledWindow1.topOffset: 20
*scrolledWindow1.traversalOn: "true"
*scrolledWindow1.rightOffset: 10
*scrolledWindow1.leftOffset: 10
*scrolledWindow1.bottomOffset: 10

*areaMessaggi.class: scrolledList
*areaMessaggi.name.source: public
*areaMessaggi.static: false
*areaMessaggi.name: areaMessaggi
*areaMessaggi.parent: scrolledWindow1
*areaMessaggi.width: 600
*areaMessaggi.height: 75
*areaMessaggi.listSizePolicy: "variable"

*areaMessaggiPopupMenu.class: rowColumn
*areaMessaggiPopupMenu.name.source: public
*areaMessaggiPopupMenu.static: false
*areaMessaggiPopupMenu.name: areaMessaggiPopupMenu
*areaMessaggiPopupMenu.parent: areaMessaggi
*areaMessaggiPopupMenu.rowColumnType: "menu_popup"
*areaMessaggiPopupMenu.menuAccelerator: "<KeyUp>F4"

*menu2_p1_title.class: label
*menu2_p1_title.static: true
*menu2_p1_title.name: menu2_p1_title
*menu2_p1_title.parent: areaMessaggiPopupMenu
*menu2_p1_title.labelString: "MESSAGE"

*areaMessaggiPopupMenu_b2.class: separator
*areaMessaggiPopupMenu_b2.static: true
*areaMessaggiPopupMenu_b2.name: areaMessaggiPopupMenu_b2
*areaMessaggiPopupMenu_b2.parent: areaMessaggiPopupMenu

*areaMessaggiPopupClearpb.class: pushButton
*areaMessaggiPopupClearpb.static: true
*areaMessaggiPopupClearpb.name: areaMessaggiPopupClearpb
*areaMessaggiPopupClearpb.parent: areaMessaggiPopupMenu
*areaMessaggiPopupClearpb.labelString: "Clear list"
*areaMessaggiPopupClearpb.activateCallback: {\
#ifndef DESIGN_TIME\
clear_message_list (areaMessaggi);\
#endif\
}

*messageLabel.class: label
*messageLabel.static: true
*messageLabel.name: messageLabel
*messageLabel.parent: messageForm
*messageLabel.x: 5
*messageLabel.y: 0
*messageLabel.width: 115
*messageLabel.height: 20
*messageLabel.bottomAttachment: "attach_widget"
*messageLabel.bottomWidget: "scrolledWindow1"
*messageLabel.labelString: "Messages"
*messageLabel.alignment: "alignment_beginning"
*messageLabel.topAttachment: "attach_form"
*messageLabel.topOffset: 5

*separator2.class: separator
*separator2.static: true
*separator2.name: separator2
*separator2.parent: messageForm
*separator2.x: 5
*separator2.y: 0
*separator2.width: 590
*separator2.height: 5
*separator2.rightAttachment: "attach_form"
*separator2.leftAttachment: "attach_form"

