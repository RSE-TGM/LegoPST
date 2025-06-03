! UIMX ascii 2.5 key: 391                                                       

*selVarCr.class: formDialog
*selVarCr.gbldecl: #include <stdio.h>\
#include <X11/Xlib.h>\
#include <X11/Intrinsic.h>\
#include <Xm/Xm.h>\
#include <Xm/Text.h>\
#include <Xm/Label.h>\
#include <Xm/ToggleB.h>\
#include <malloc.h>\
#include "messaggi.h"\
\
#include "sim_param.h"\
#include "sim_types.h"\
#include "dispatcher.h"\
#include "bistrutt.h"\
#include "cursore.h"\
#include "option.h"\
\
#ifndef DESIGN_TIME\
#include "cont_rec.h"\
#endif\
\
#include "filtri.h"\
FILTRO_TIPI filtro_tipi;\
extern int sel_var_cr_interface_active;\
#include "file_selection.h"\
char filtro_kks[12];\
int modello_selezionato;\
int blocco_selezionato;\
int kks_filter;\
int tipo_filter;\
extern OPTIONS_FLAGS options;\
extern Boolean *IcProt;
*selVarCr.ispecdecl:
*selVarCr.funcdecl: swidget create_selVarCr(padre)\
Widget padre;
*selVarCr.funcname: create_selVarCr
*selVarCr.funcdef: "swidget", "<create_selVarCr>(%)"
*selVarCr.argdecl: Widget padre;
*selVarCr.arglist: padre
*selVarCr.arglist.padre: "Widget", "%padre%"
*selVarCr.icode: #ifndef DESIGN_TIME\
char *s;\
set_cursor (padre,CLOCK);\
#endif
*selVarCr.fcode: #ifndef DESIGN_TIME\
get_kks_filter (selVarCrKksBullBoard);\
presetFilterDefaults (rtrn);\
/*\
applicaFiltri (rtrn, FILTRO_CR2);\
*/\
set_var_cr_defaults (rtrn); \
UxPopupInterface (rtrn, no_grab);\
display_lista_sistema (selVarListaSistemi);\
display_lista_blocchi (selVarListaBlocchi);\
display_lista_var (selVarToSelect,\
                   modello_selezionato,blocco_selezionato,\
                   kks_filter,tipo_filter);\
s = (char *)malloc(strlen(SEL_VAR_TITLE)+strlen(FILE_REC_DEFAULT)+10);\
sprintf (s,"%s: %s",SEL_VAR_TITLE,FILE_REC_DEFAULT);\
update_title (UxGetWidget(rtrn),s);\
free(s);\
sel_var_cr_interface_active = 1;\
set_cursor (padre,NORMALE);\
#endif\
return(rtrn);\

*selVarCr.auxdecl: int presetFilterDefaults (w)\
Widget w;\
{\
	XmToggleButtonSetState (UxGetWidget(selByAllVariablestb),options.optionsFilterCr.ByAllVar,False);\
	XmToggleButtonSetState (UxGetWidget(selByModeltb),options.optionsFilterCr.ByModBlock,False);\
	XmToggleButtonSetState (UxGetWidget(selByModelOnlytb),options.optionsFilterCr.ByMod,False);\
	XmToggleButtonSetState (UxGetWidget(selVarMenuKkstb),options.optionsFilterCr.ByKks,False);\
	XmToggleButtonSetState (UxGetWidget(selVarMenuTipotb),options.optionsFilterCr.ByType,False);\
\
	XmToggleButtonSetState (UxGetWidget(selVarCrAllVartb),options.optionsFilterCr.ByAllVar,False);\
	XmToggleButtonSetState (UxGetWidget(selVarCrModBlocktb),options.optionsFilterCr.ByModBlock,False);\
	XmToggleButtonSetState (UxGetWidget(selVarCrModtb),options.optionsFilterCr.ByMod,False);\
	XmToggleButtonSetState (UxGetWidget(selVarCrKkstb),options.optionsFilterCr.ByKks,False);\
	XmToggleButtonSetState (UxGetWidget(selVarCrTypetb),options.optionsFilterCr.ByType,False);\
\
	return (0);\
}\
/******************************************************************/\
int set_var_cr_defaults (w)\
Widget w;\
{\
Boolean stato;\
#ifndef DESIGN_TIME\
/* legge lo stato del toggle button di filtro */\
   stato = XmToggleButtonGetState (UxGetWidget(selByAllVariablestb)); \
   switch (stato)\
      {\
      case 0:\
         modello_selezionato = 1;\
         blocco_selezionato = 1;\
         XtManageChild (UxGetWidget(selByModelForm));\
         break;\
      case 1:\
         modello_selezionato = NO_MOD;\
         blocco_selezionato = NO_BLOCK;\
         XtUnmanageChild (UxGetWidget(selByModelForm));\
         break;\
      }\
/* legge lo stato del toggle button di filtro */\
   stato = XmToggleButtonGetState (UxGetWidget(selByModeltb)); \
   switch (stato)\
      {\
      case 1:\
         modello_selezionato = 1;\
         blocco_selezionato = 1;\
         XtManageChild (UxGetWidget(selByModelForm));\
         break;\
      case 0:\
         modello_selezionato = NO_MOD;\
         blocco_selezionato = NO_BLOCK;\
         XtUnmanageChild (UxGetWidget(selByModelForm));\
         break;\
      }\
/* legge lo stato del toggle button del filtro kks */\
    stato = XmToggleButtonGetState (UxGetWidget(selVarMenuKkstb));  \
   switch (stato)\
      {\
      case 1:\
         kks_filter = 1;\
         XtManageChild (UxGetWidget(selVarCrKksForm));\
         break;\
      case 0:\
         kks_filter = 0;\
         XtUnmanageChild (UxGetWidget(selVarCrKksForm));\
         break;\
      }\
/* legge lo stato del toggle button del filtro per tipo */\
    stato = XmToggleButtonGetState (UxGetWidget(selVarMenuTipotb));  \
   switch (stato)\
      {\
      case 1:\
         tipo_filter = 1;\
         carica_filtro_tipo ();\
         XtManageChild (UxGetWidget(selVarCrTipoForm));\
         break;\
      case 0:\
         tipo_filter = 0;\
         XtUnmanageChild (UxGetWidget(selVarCrTipoForm));\
         break;\
      }\
#endif\
   return (0);\
}\
/*************************************************/\
int carica_filtro_tipo ()\
{\
#ifndef DESIGN_TIME\
   filtro_tipi.pressione = XmToggleButtonGetState (selVarCrTipoPressionetb);\
   filtro_tipi.temperatura = XmToggleButtonGetState (selVarCrTipoTemperaturatb);\
   filtro_tipi.entalpia = XmToggleButtonGetState (selVarCrTipoEntalpiatb);\
   filtro_tipi.portata = XmToggleButtonGetState (selVarCrTipoPortatatb);\
   filtro_tipi.potenza = XmToggleButtonGetState (selVarCrTipoPotenzatb);\
   filtro_tipi.giri = XmToggleButtonGetState (selVarCrTipoGiritb);\
   filtro_tipi.alzvalv = XmToggleButtonGetState (selVarCrTipoAlzvalvtb);\
   filtro_tipi.ingressi = XmToggleButtonGetState (selVarCrTipoInput1);\
   filtro_tipi.uscite = XmToggleButtonGetState (selVarCrTipoOutput1);\
   filtro_tipi.aingabili = XmToggleButtonGetState (selVarCrTipoAingable1);\
/* printf ("tipi:\n pressione %d \n \\
                 temperatura %d \n \\
                 entalpia %d \n \\
                 portata %d \n \\
                 potenza %d \n \\
                 giri %d \n \\
                 alzvalv %d\n \\
                 input %d \n \\
                 output %d \n \\
                 aing %d \n",\
                 filtro_tipi.pressione,filtro_tipi.temperatura,filtro_tipi.entalpia,\
                 filtro_tipi.portata,filtro_tipi.potenza,filtro_tipi.giri,\
                 filtro_tipi.alzvalv,filtro_tipi.ingressi,filtro_tipi.uscite,\
                 filtro_tipi.aingabili); */\
#endif\
   return (0);\
}\

*selVarCr.name.source: public
*selVarCr.static: false
*selVarCr.name: selVarCr
*selVarCr.parent: NO_PARENT
*selVarCr.defaultShell: topLevelShell
*selVarCr.unitType: "pixels"
*selVarCr.x: 730
*selVarCr.y: 350
*selVarCr.width: 540
*selVarCr.height: 800
*selVarCr.resizePolicy: "resize_grow"
*selVarCr.destroyCallback: {\
sel_var_cr_interface_active = 0;\
set_var_cr_defaults (selVarCr);\
}

*menu3.class: rowColumn
*menu3.static: true
*menu3.name: menu3
*menu3.parent: selVarCr
*menu3.rowColumnType: "menu_bar"
*menu3.menuAccelerator: "<KeyUp>F10"
*menu3.rightAttachment: "attach_form"
*menu3.leftAttachment: "attach_form"

*selVarCrMenu.class: rowColumn
*selVarCrMenu.static: true
*selVarCrMenu.name: selVarCrMenu
*selVarCrMenu.parent: menu3
*selVarCrMenu.rowColumnType: "menu_pulldown"

*selVarCrMenu_b4.class: pushButton
*selVarCrMenu_b4.static: true
*selVarCrMenu_b4.name: selVarCrMenu_b4
*selVarCrMenu_b4.parent: selVarCrMenu
*selVarCrMenu_b4.labelString: "Load ..."
*selVarCrMenu_b4.activateCallback: loadas_cr (selVarCr,1,VAR_FOR_CR); /* 1=in lettura */\


*selVarCrMenu_b3.class: pushButton
*selVarCrMenu_b3.static: true
*selVarCrMenu_b3.name: selVarCrMenu_b3
*selVarCrMenu_b3.parent: selVarCrMenu
*selVarCrMenu_b3.labelString: "Save ..."
*selVarCrMenu_b3.activateCallback: saveas_cr (selVarCr,2,VAR_FOR_CR);  /* 2 = in scrittura */\
 

*selVarCrMenu_b2.class: separator
*selVarCrMenu_b2.static: true
*selVarCrMenu_b2.name: selVarCrMenu_b2
*selVarCrMenu_b2.parent: selVarCrMenu

*selVarCrMenuFilepb.class: pushButton
*selVarCrMenuFilepb.static: true
*selVarCrMenuFilepb.name: selVarCrMenuFilepb
*selVarCrMenuFilepb.parent: selVarCrMenu
*selVarCrMenuFilepb.labelString: "Quit"
*selVarCrMenuFilepb.activateCallback: sel_var_cr_interface_active = 0;\
set_var_cr_defaults (UxGetWidget(selVarCr));\
DistruggiInterfaccia (XtParent(UxGetWidget(selVarCr)));\


*selVarCrMenuEdit.class: rowColumn
*selVarCrMenuEdit.static: true
*selVarCrMenuEdit.name: selVarCrMenuEdit
*selVarCrMenuEdit.parent: menu3
*selVarCrMenuEdit.rowColumnType: "menu_pulldown"

*SelVarCrMenuEditTimer.class: pushButton
*SelVarCrMenuEditTimer.static: true
*SelVarCrMenuEditTimer.name: SelVarCrMenuEditTimer
*SelVarCrMenuEditTimer.parent: selVarCrMenuEdit
*SelVarCrMenuEditTimer.labelString: "Set Record. Freq. ..."
*SelVarCrMenuEditTimer.activateCallback: {\
richiesta_timer_cr (selVarCr);\
}

*selVarMenuFilter.class: rowColumn
*selVarMenuFilter.static: true
*selVarMenuFilter.name: selVarMenuFilter
*selVarMenuFilter.parent: menu3
*selVarMenuFilter.rowColumnType: "menu_pulldown"
*selVarMenuFilter.createManaged: "false"

*selVarMenuFilter_b8.class: cascadeButton
*selVarMenuFilter_b8.static: true
*selVarMenuFilter_b8.name: selVarMenuFilter_b8
*selVarMenuFilter_b8.parent: selVarMenuFilter
*selVarMenuFilter_b8.labelString: "Select filter ..."
*selVarMenuFilter_b8.subMenuId: "selVarMenuOtherPane"

*selVarMenuFilter_b2.class: pushButton
*selVarMenuFilter_b2.static: true
*selVarMenuFilter_b2.name: selVarMenuFilter_b2
*selVarMenuFilter_b2.parent: selVarMenuFilter
*selVarMenuFilter_b2.labelString: "Save Selection"
*selVarMenuFilter_b2.activateCallback: {\
if (SD_optsave3 (BANCO, &options,IcProt) > 0)\
      fprintf (stderr,"OPZIONI salvate\n");\
else\
      fprintf (stderr,"*** errore save opzioni\n");\
}

*selVarMenuOtherPane.class: rowColumn
*selVarMenuOtherPane.static: true
*selVarMenuOtherPane.name: selVarMenuOtherPane
*selVarMenuOtherPane.parent: selVarMenuFilter
*selVarMenuOtherPane.rowColumnType: "menu_pulldown"

*selByAllVariablestb.class: toggleButton
*selByAllVariablestb.static: true
*selByAllVariablestb.name: selByAllVariablestb
*selByAllVariablestb.parent: selVarMenuOtherPane
*selByAllVariablestb.labelString: "All variables"
*selByAllVariablestb.indicatorType: "one_of_many"
*selByAllVariablestb.visibleWhenOff: "true"
*selByAllVariablestb.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVarCr),\
                  UxGetWidget(selByModelForm), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                   FILTER_ALL_VAR,FILTRO_CR);\
options.optionsFilterCr.ByAllVar = ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set;\
display_lista_var (UxGetWidget(selVarCr), modello_selezionato, blocco_selezionato,\
                     kks_filter,tipo_filter);\
}
*selByAllVariablestb.set: "false"

*selByModeltb.class: toggleButton
*selByModeltb.static: true
*selByModeltb.name: selByModeltb
*selByModeltb.parent: selVarMenuOtherPane
*selByModeltb.labelString: "models & blocks"
*selByModeltb.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVarCr),\
                  UxGetWidget(selByModelForm), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                   FILTER_MOD_BLOC,FILTRO_CR);\
options.optionsFilterCr.ByModBlock = ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set;\
display_lista_var (UxGetWidget(selVarCr), modello_selezionato, blocco_selezionato,\
                     kks_filter,tipo_filter);\
}
*selByModeltb.set: "true"
*selByModeltb.armCallback: {\
\
}
*selByModeltb.createCallback: {\
\
}
*selByModeltb.visibleWhenOff: "true"
*selByModeltb.indicatorType: "one_of_many"

*selByModelOnlytb.class: toggleButton
*selByModelOnlytb.static: true
*selByModelOnlytb.name: selByModelOnlytb
*selByModelOnlytb.parent: selVarMenuOtherPane
*selByModelOnlytb.labelString: "models only"
*selByModelOnlytb.indicatorType: "one_of_many"
*selByModelOnlytb.visibleWhenOff: "true"
*selByModelOnlytb.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVarCr),\
                  UxGetWidget(selByModelForm), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                   FILTER_MOD_ONLY,FILTRO_CR);\
options.optionsFilterCr.ByMod = ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set;\
display_lista_var (UxGetWidget(selVarCr), modello_selezionato, blocco_selezionato,\
                     kks_filter,tipo_filter);\
}

*selVarMenuOtherPane_b6.class: separator
*selVarMenuOtherPane_b6.static: true
*selVarMenuOtherPane_b6.name: selVarMenuOtherPane_b6
*selVarMenuOtherPane_b6.parent: selVarMenuOtherPane

*selVarMenuKkstb.class: toggleButton
*selVarMenuKkstb.static: true
*selVarMenuKkstb.name: selVarMenuKkstb
*selVarMenuKkstb.parent: selVarMenuOtherPane
*selVarMenuKkstb.labelString: "KKS filter"
*selVarMenuKkstb.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVarCr),\
                  UxGetWidget(selVarCrKksForm), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                  FILTER_KKS,FILTRO_CR);\
options.optionsFilterCr.ByKks = ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set;\
display_lista_var (UxGetWidget(selVarCr),\
                  modello_selezionato, blocco_selezionato,\
                  kks_filter,tipo_filter);\
}
*selVarMenuKkstb.set: "true"
*selVarMenuKkstb.visibleWhenOff: "true"
*selVarMenuKkstb.indicatorType: "n_of_many"

*selVarMenuOtherPane_b3.class: separator
*selVarMenuOtherPane_b3.static: true
*selVarMenuOtherPane_b3.name: selVarMenuOtherPane_b3
*selVarMenuOtherPane_b3.parent: selVarMenuOtherPane

*selVarMenuTipotb.class: toggleButton
*selVarMenuTipotb.static: true
*selVarMenuTipotb.name: selVarMenuTipotb
*selVarMenuTipotb.parent: selVarMenuOtherPane
*selVarMenuTipotb.labelString: "by type"
*selVarMenuTipotb.visibleWhenOff: "true"
*selVarMenuTipotb.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVarCr),\
                  UxGetWidget(selVarCrTipoForm), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                  FILTER_TYPE,FILTRO_CR);\
options.optionsFilterCr.ByType = ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set;\
display_lista_var (UxGetWidget(selVarCr),\
                  modello_selezionato, blocco_selezionato,\
                  kks_filter,tipo_filter);\
}

*menu3_top_b1.class: cascadeButton
*menu3_top_b1.static: true
*menu3_top_b1.name: menu3_top_b1
*menu3_top_b1.parent: menu3
*menu3_top_b1.labelString: "File"
*menu3_top_b1.subMenuId: "selVarCrMenu"

*menu3_top_b5.class: cascadeButton
*menu3_top_b5.static: true
*menu3_top_b5.name: menu3_top_b5
*menu3_top_b5.parent: menu3
*menu3_top_b5.labelString: "Edit"
*menu3_top_b5.subMenuId: "selVarCrMenuEdit"

*menu3_top_b2.class: cascadeButton
*menu3_top_b2.static: true
*menu3_top_b2.name: menu3_top_b2
*menu3_top_b2.parent: menu3
*menu3_top_b2.labelString: "Filter"
*menu3_top_b2.subMenuId: "selVarMenuFilter"
*menu3_top_b2.createManaged: "false"

*frame1.class: frame
*frame1.static: true
*frame1.name: frame1
*frame1.parent: selVarCr
*frame1.x: 0
*frame1.y: 480
*frame1.width: 540
*frame1.height: 80
*frame1.rightAttachment: "attach_form"
*frame1.leftAttachment: "attach_form"
*frame1.topAttachment: "attach_none"
*frame1.topOffset: 0
*frame1.bottomAttachment: "attach_form"

*form39.class: form
*form39.static: true
*form39.name: form39
*form39.parent: frame1
*form39.resizePolicy: "resize_none"
*form39.x: 10
*form39.y: 10
*form39.width: 490
*form39.height: 50
*form39.defaultButton: "selVarOkpb"

*selVarOkpb.class: pushButton
*selVarOkpb.static: true
*selVarOkpb.name: selVarOkpb
*selVarOkpb.parent: form39
*selVarOkpb.x: 20
*selVarOkpb.y: 20
*selVarOkpb.width: 100
*selVarOkpb.height: 40
*selVarOkpb.topOffset: 15
*selVarOkpb.bottomAttachment: "attach_form"
*selVarOkpb.bottomOffset: 15
*selVarOkpb.rightAttachment: "attach_self"
*selVarOkpb.topAttachment: "attach_form"
*selVarOkpb.labelString: CONF_ACCEPT
*selVarOkpb.leftAttachment: "attach_form"
*selVarOkpb.leftOffset: 20
*selVarOkpb.activateCallback: {\
#ifndef DESIGN_TIME\
set_cursor ((Widget)UxClientData, CLOCK);\
salva_sel_var (UxWidget, SEL_REG, FILE_REC_DEFAULT);\
newvarcr ((Widget)UxClientData);\
set_cursor ((Widget)UxClientData, NORMALE);\
/*** in destroy callback\
sel_var_cr_interface_active = 0;\
set_var_cr_defaults((Widget)UxClientData);\
***/\
DistruggiInterfaccia (XtParent((Widget)UxClientData));\
#endif\
}
*selVarOkpb.activateCallbackClientData: selVarCr

*selVarQuitpb.class: pushButton
*selVarQuitpb.static: true
*selVarQuitpb.name: selVarQuitpb
*selVarQuitpb.parent: form39
*selVarQuitpb.x: 410
*selVarQuitpb.y: 20
*selVarQuitpb.width: 100
*selVarQuitpb.height: 40
*selVarQuitpb.bottomAttachment: "attach_form"
*selVarQuitpb.bottomOffset: 15
*selVarQuitpb.leftAttachment: "attach_self"
*selVarQuitpb.leftOffset: 0
*selVarQuitpb.leftPosition: 0
*selVarQuitpb.rightAttachment: "attach_form"
*selVarQuitpb.rightOffset: 20
*selVarQuitpb.topAttachment: "attach_form"
*selVarQuitpb.topOffset: 15
*selVarQuitpb.activateCallbackClientData: UxGetWidget (selVarCr)
*selVarQuitpb.labelString: CONF_DISCARD
*selVarQuitpb.activateCallback: {\
#ifndef DESIGN_TIME\
/** in destroy callback\
   sel_var_cr_interface_active = 0;\
   set_var_cr_defaults ((Widget)UxClientData);\
***/\
   DistruggiInterfaccia (XtParent((Widget)UxClientData));\
#endif\
}

*frame24.class: frame
*frame24.static: true
*frame24.name: frame24
*frame24.parent: selVarCr
*frame24.x: 0
*frame24.y: 480
*frame24.width: 540
*frame24.height: 80
*frame24.bottomAttachment: "attach_none"
*frame24.bottomWidget: ""
*frame24.rightAttachment: "attach_form"
*frame24.topAttachment: "attach_widget"
*frame24.topOffset: 0
*frame24.topWidget: "menu3"
*frame24.leftAttachment: "attach_form"

*selVarCrFilterForm.class: form
*selVarCrFilterForm.static: true
*selVarCrFilterForm.name: selVarCrFilterForm
*selVarCrFilterForm.parent: frame24
*selVarCrFilterForm.resizePolicy: "resize_none"
*selVarCrFilterForm.x: 2
*selVarCrFilterForm.y: 2
*selVarCrFilterForm.width: 536
*selVarCrFilterForm.height: 58

*frame26.class: frame
*frame26.static: true
*frame26.name: frame26
*frame26.parent: selVarCrFilterForm
*frame26.x: 210
*frame26.y: 10
*frame26.width: 150
*frame26.height: 60
*frame26.bottomAttachment: "attach_form"
*frame26.bottomOffset: 2
*frame26.leftAttachment: "attach_none"
*frame26.leftOffset: 0
*frame26.topAttachment: "attach_form"
*frame26.topOffset: 2
*frame26.rightAttachment: "attach_form"
*frame26.rightOffset: 2

*form50.class: form
*form50.static: true
*form50.name: form50
*form50.parent: frame26
*form50.resizePolicy: "resize_none"
*form50.x: 50
*form50.y: 10
*form50.width: 80
*form50.height: 30

*selVarCrAllVartb.class: toggleButton
*selVarCrAllVartb.static: true
*selVarCrAllVartb.name: selVarCrAllVartb
*selVarCrAllVartb.parent: form50
*selVarCrAllVartb.x: 10
*selVarCrAllVartb.y: 0
*selVarCrAllVartb.width: 128
*selVarCrAllVartb.height: 20
*selVarCrAllVartb.indicatorType: "one_of_many"
*selVarCrAllVartb.labelString: "All Variables"
*selVarCrAllVartb.alignment: "alignment_beginning"
*selVarCrAllVartb.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVarCr),\
                  UxGetWidget(selByModelForm), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                   FILTER_ALL_VAR,FILTRO_CR2);\
}

*selVarCrModBlocktb.class: toggleButton
*selVarCrModBlocktb.static: true
*selVarCrModBlocktb.name: selVarCrModBlocktb
*selVarCrModBlocktb.parent: form50
*selVarCrModBlocktb.x: 10
*selVarCrModBlocktb.y: 20
*selVarCrModBlocktb.width: 128
*selVarCrModBlocktb.height: 20
*selVarCrModBlocktb.indicatorType: "one_of_many"
*selVarCrModBlocktb.labelString: "Mod.&Blocks"
*selVarCrModBlocktb.alignment: "alignment_beginning"
*selVarCrModBlocktb.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVarCr),\
                  UxGetWidget(selByModelForm), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                   FILTER_MOD_BLOC,FILTRO_CR2);\
}

*selVarCrModtb.class: toggleButton
*selVarCrModtb.static: true
*selVarCrModtb.name: selVarCrModtb
*selVarCrModtb.parent: form50
*selVarCrModtb.x: 10
*selVarCrModtb.y: 40
*selVarCrModtb.width: 130
*selVarCrModtb.height: 20
*selVarCrModtb.indicatorType: "one_of_many"
*selVarCrModtb.labelString: "Models Only"
*selVarCrModtb.alignment: "alignment_beginning"
*selVarCrModtb.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVarCr),\
                  UxGetWidget(selByModelForm), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                   FILTER_MOD_ONLY,FILTRO_CR2);\
}

*frame25.class: frame
*frame25.static: true
*frame25.name: frame25
*frame25.parent: selVarCrFilterForm
*frame25.x: 40
*frame25.y: 20
*frame25.width: 150
*frame25.height: 60
*frame25.bottomAttachment: "attach_form"
*frame25.bottomOffset: 2
*frame25.leftAttachment: "attach_none"
*frame25.leftOffset: 0
*frame25.topAttachment: "attach_form"
*frame25.topOffset: 2
*frame25.rightAttachment: "attach_widget"
*frame25.rightOffset: 2
*frame25.rightWidget: "frame26"

*form49.class: form
*form49.static: true
*form49.name: form49
*form49.parent: frame25
*form49.resizePolicy: "resize_none"
*form49.x: 10
*form49.y: 5
*form49.width: 138
*form49.height: 56

*selVarCrKkstb.class: toggleButton
*selVarCrKkstb.static: true
*selVarCrKkstb.name: selVarCrKkstb
*selVarCrKkstb.parent: form49
*selVarCrKkstb.x: 0
*selVarCrKkstb.y: 0
*selVarCrKkstb.width: 100
*selVarCrKkstb.height: 20
*selVarCrKkstb.alignment: "alignment_beginning"
*selVarCrKkstb.labelString: "by KKS"
*selVarCrKkstb.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVarCr),\
                  UxGetWidget(selVarCrKksForm), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                  FILTER_KKS,FILTRO_CR2);\
}

*selVarCrTypetb.class: toggleButton
*selVarCrTypetb.static: true
*selVarCrTypetb.name: selVarCrTypetb
*selVarCrTypetb.parent: form49
*selVarCrTypetb.x: 0
*selVarCrTypetb.y: 20
*selVarCrTypetb.width: 100
*selVarCrTypetb.height: 20
*selVarCrTypetb.alignment: "alignment_beginning"
*selVarCrTypetb.labelString: "by Type"
*selVarCrTypetb.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVarCr),\
                  UxGetWidget(selVarCrTipoForm), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                  FILTER_TYPE,FILTRO_CR2);\
}

*selVarCrSaveFilterpb.class: pushButton
*selVarCrSaveFilterpb.static: true
*selVarCrSaveFilterpb.name: selVarCrSaveFilterpb
*selVarCrSaveFilterpb.parent: selVarCrFilterForm
*selVarCrSaveFilterpb.x: 10
*selVarCrSaveFilterpb.y: 10
*selVarCrSaveFilterpb.width: 190
*selVarCrSaveFilterpb.height: 30
*selVarCrSaveFilterpb.labelString: SAVEFILTER
*selVarCrSaveFilterpb.activateCallback: {\
if (SD_optsave3 (BANCO, &options,IcProt) > 0)\
      fprintf (stderr,"OPZIONI salvate\n");\
else\
      fprintf (stderr,"*** errore save opzioni\n");\
}

*selVarCrApplyFilterpb.class: pushButton
*selVarCrApplyFilterpb.static: true
*selVarCrApplyFilterpb.name: selVarCrApplyFilterpb
*selVarCrApplyFilterpb.parent: selVarCrFilterForm
*selVarCrApplyFilterpb.x: 10
*selVarCrApplyFilterpb.y: 40
*selVarCrApplyFilterpb.width: 190
*selVarCrApplyFilterpb.height: 30
*selVarCrApplyFilterpb.labelString: APPLYFILTER
*selVarCrApplyFilterpb.activateCallback: {\
applicaFiltri (selVarCr, FILTRO_CR2);\
\
}

*panedWindow1.class: panedWindow
*panedWindow1.static: true
*panedWindow1.name: panedWindow1
*panedWindow1.parent: selVarCr
*panedWindow1.x: 0
*panedWindow1.y: 110
*panedWindow1.width: 540
*panedWindow1.height: 390
*panedWindow1.bottomAttachment: "attach_widget"
*panedWindow1.bottomWidget: "frame1"
*panedWindow1.rightAttachment: "attach_form"
*panedWindow1.leftAttachment: "attach_form"
*panedWindow1.topAttachment: "attach_widget"
*panedWindow1.topWidget: "frame24"

*selByModelForm.class: form
*selByModelForm.static: true
*selByModelForm.name: selByModelForm
*selByModelForm.parent: panedWindow1
*selByModelForm.resizePolicy: "resize_none"
*selByModelForm.x: 0
*selByModelForm.y: 10
*selByModelForm.width: 540
*selByModelForm.height: 150
*selByModelForm.createManaged: "true"
*selByModelForm.paneMinimum: 50

*label5.class: label
*label5.static: true
*label5.name: label5
*label5.parent: selByModelForm
*label5.x: 220
*label5.y: 0
*label5.width: 170
*label5.height: 30
*label5.alignment: "alignment_beginning"
*label5.topOffset: 0
*label5.labelString: BLOCK_LIST 

*label7.class: label
*label7.static: true
*label7.name: label7
*label7.parent: selByModelForm
*label7.x: 20
*label7.y: 0
*label7.width: 170
*label7.height: 30
*label7.alignment: "alignment_beginning"
*label7.labelString: SYSTEM_LIST ? SYSTEM_LIST : "SYSTEMS"

*form44.class: form
*form44.static: true
*form44.name: form44
*form44.parent: selByModelForm
*form44.resizePolicy: "resize_none"
*form44.x: 0
*form44.y: 120
*form44.width: 534
*form44.height: 30
*form44.bottomAttachment: "attach_form"
*form44.rightAttachment: "attach_form"
*form44.leftAttachment: "attach_form"

*selVarFindDown2.class: arrowButton
*selVarFindDown2.static: true
*selVarFindDown2.name: selVarFindDown2
*selVarFindDown2.parent: form44
*selVarFindDown2.x: 250
*selVarFindDown2.y: 0
*selVarFindDown2.width: 60
*selVarFindDown2.height: 30
*selVarFindDown2.arrowDirection: "arrow_down"
*selVarFindDown2.leftAttachment: "attach_none"
*selVarFindDown2.leftOffset: 0
*selVarFindDown2.leftWidget: ""
*selVarFindDown2.bottomOffset: 3
*selVarFindDown2.bottomAttachment: "attach_form"
*selVarFindDown2.topAttachment: "attach_form"
*selVarFindDown2.topOffset: 3
*selVarFindDown2.activateCallback: {\
#ifndef DESIGN_TIME\
next_bloc (selVarListaBlocchi,SEARCH_DOWN,selVarText3);\
#endif\
}
*selVarFindDown2.rightAttachment: "attach_form"

*selVarText3.class: textField
*selVarText3.static: true
*selVarText3.name: selVarText3
*selVarText3.parent: form44
*selVarText3.x: 20
*selVarText3.y: 10
*selVarText3.width: 140
*selVarText3.height: 20
*selVarText3.bottomAttachment: "attach_form"
*selVarText3.topAttachment: "attach_form"
*selVarText3.leftAttachment: "attach_none"
*selVarText3.leftOffset: 0
*selVarText3.leftWidget: ""
*selVarText3.bottomOffset: 0
*selVarText3.topOffset: 0
*selVarText3.rightAttachment: "attach_widget"
*selVarText3.rightOffset: 5
*selVarText3.rightWidget: "selVarFindDown2"
*selVarText3.marginHeight: 2

*selVarFindUp2.class: arrowButton
*selVarFindUp2.static: true
*selVarFindUp2.name: selVarFindUp2
*selVarFindUp2.parent: form44
*selVarFindUp2.x: 10
*selVarFindUp2.y: 0
*selVarFindUp2.width: 60
*selVarFindUp2.height: 30
*selVarFindUp2.arrowDirection: "arrow_up"
*selVarFindUp2.bottomAttachment: "attach_form"
*selVarFindUp2.leftOffset: 0
*selVarFindUp2.topAttachment: "attach_form"
*selVarFindUp2.bottomOffset: 3
*selVarFindUp2.topOffset: 3
*selVarFindUp2.activateCallbackClientData: (XtPointer) 0x0
*selVarFindUp2.activateCallback: {\
#ifndef DESIGN_TIME\
next_bloc (selVarListaBlocchi,SEARCH_UP,selVarText3);\
#endif\
}
*selVarFindUp2.leftAttachment: "attach_none"
*selVarFindUp2.rightAttachment: "attach_widget"
*selVarFindUp2.rightOffset: 5
*selVarFindUp2.rightWidget: "selVarText3"

*scrolledWindow6.class: scrolledWindow
*scrolledWindow6.static: true
*scrolledWindow6.name: scrolledWindow6
*scrolledWindow6.parent: selByModelForm
*scrolledWindow6.scrollingPolicy: "application_defined"
*scrolledWindow6.x: 0
*scrolledWindow6.y: 30
*scrolledWindow6.visualPolicy: "variable"
*scrolledWindow6.scrollBarDisplayPolicy: "static"
*scrolledWindow6.shadowThickness: 0
*scrolledWindow6.bottomAttachment: "attach_widget"
*scrolledWindow6.bottomOffset: 5
*scrolledWindow6.leftOffset: 20
*scrolledWindow6.topAttachment: "attach_widget"
*scrolledWindow6.topWidget: "label7"
*scrolledWindow6.width: 180
*scrolledWindow6.leftAttachment: "attach_form"
*scrolledWindow6.bottomWidget: "form44"

*selVarListaSistemi.class: scrolledList
*selVarListaSistemi.static: true
*selVarListaSistemi.name: selVarListaSistemi
*selVarListaSistemi.parent: scrolledWindow6
*selVarListaSistemi.width: 170
*selVarListaSistemi.height: 115
*selVarListaSistemi.singleSelectionCallback: {\
#ifndef DESIGN_TIME\
XmListCallbackStruct *reason;\
   reason = (XmListCallbackStruct *)UxCallbackArg;\
   modello_selezionato = reason->item_position;\
\
printf ("selezionato modello %d\n",modello_selezionato);\
/*   display_lista_sistema (selVarListaSistemi); */\
   display_lista_blocchi (selVarListaBlocchi); \
   display_lista_var (selVarListaSistemi, modello_selezionato, blocco_selezionato,\
                      kks_filter,tipo_filter);\
#endif\
}
*selVarListaSistemi.automaticSelection: "true"
*selVarListaSistemi.selectionPolicy: "single_select"

*scrolledWindow7.class: scrolledWindow
*scrolledWindow7.static: true
*scrolledWindow7.name: scrolledWindow7
*scrolledWindow7.parent: selByModelForm
*scrolledWindow7.scrollingPolicy: "application_defined"
*scrolledWindow7.x: 215
*scrolledWindow7.y: 30
*scrolledWindow7.visualPolicy: "variable"
*scrolledWindow7.scrollBarDisplayPolicy: "static"
*scrolledWindow7.shadowThickness: 0
*scrolledWindow7.bottomAttachment: "attach_widget"
*scrolledWindow7.bottomOffset: 5
*scrolledWindow7.leftAttachment: "attach_widget"
*scrolledWindow7.leftOffset: 15
*scrolledWindow7.leftWidget: "scrolledWindow6"
*scrolledWindow7.topAttachment: "attach_widget"
*scrolledWindow7.topWidget: "label5"
*scrolledWindow7.width: 300
*scrolledWindow7.rightAttachment: "attach_form"
*scrolledWindow7.rightOffset: 20
*scrolledWindow7.bottomWidget: "form44"

*selVarListaBlocchi.class: scrolledList
*selVarListaBlocchi.static: true
*selVarListaBlocchi.name: selVarListaBlocchi
*selVarListaBlocchi.parent: scrolledWindow7
*selVarListaBlocchi.width: 270
*selVarListaBlocchi.height: 115
*selVarListaBlocchi.singleSelectionCallback: {\
#ifndef DESIGN_TIME\
XmListCallbackStruct *reason;\
reason = (XmListCallbackStruct *)UxCallbackArg;\
\
   blocco_selezionato = reason->item_position;\
printf ("selezionato blocco %d\n",blocco_selezionato);\
   display_lista_var (selVarListaSistemi, modello_selezionato, blocco_selezionato,\
                      kks_filter,tipo_filter);\
#endif\
}
*selVarListaBlocchi.automaticSelection: "true"
*selVarListaBlocchi.selectionPolicy: "single_select"

*selVarCrKksForm.class: form
*selVarCrKksForm.static: true
*selVarCrKksForm.name: selVarCrKksForm
*selVarCrKksForm.parent: panedWindow1
*selVarCrKksForm.resizePolicy: "resize_none"
*selVarCrKksForm.x: 0
*selVarCrKksForm.y: 200
*selVarCrKksForm.width: 540
*selVarCrKksForm.height: 160
*selVarCrKksForm.paneMinimum: 80
*selVarCrKksForm.paneMaximum: 80

*selVarCrKksBullBoard.class: bulletinBoard
*selVarCrKksBullBoard.static: true
*selVarCrKksBullBoard.name: selVarCrKksBullBoard
*selVarCrKksBullBoard.parent: selVarCrKksForm
*selVarCrKksBullBoard.resizePolicy: "resize_none"
*selVarCrKksBullBoard.x: 0
*selVarCrKksBullBoard.y: 0
*selVarCrKksBullBoard.width: 530
*selVarCrKksBullBoard.height: 100
*selVarCrKksBullBoard.bottomAttachment: "attach_form"
*selVarCrKksBullBoard.topAttachment: "attach_form"
*selVarCrKksBullBoard.rightAttachment: "attach_form"
*selVarCrKksBullBoard.leftAttachment: "attach_form"

*selVarCrKksText1.class: text
*selVarCrKksText1.static: true
*selVarCrKksText1.name: selVarCrKksText1
*selVarCrKksText1.parent: selVarCrKksBullBoard
*selVarCrKksText1.x: 50
*selVarCrKksText1.y: 30
*selVarCrKksText1.width: 25
*selVarCrKksText1.height: 25
*selVarCrKksText1.columns: 1
*selVarCrKksText1.maxLength: 1
*selVarCrKksText1.marginHeight: 2
*selVarCrKksText1.marginWidth: 5

*selVarCrKksSistemaLabel.class: label
*selVarCrKksSistemaLabel.static: true
*selVarCrKksSistemaLabel.name: selVarCrKksSistemaLabel
*selVarCrKksSistemaLabel.parent: selVarCrKksBullBoard
*selVarCrKksSistemaLabel.x: 30
*selVarCrKksSistemaLabel.y: 10
*selVarCrKksSistemaLabel.width: 110
*selVarCrKksSistemaLabel.height: 20
*selVarCrKksSistemaLabel.labelString: KKS_SYST_LABEL

*selVarCrKksSottosistLabel.class: label
*selVarCrKksSottosistLabel.static: true
*selVarCrKksSottosistLabel.name: selVarCrKksSottosistLabel
*selVarCrKksSottosistLabel.parent: selVarCrKksBullBoard
*selVarCrKksSottosistLabel.x: 140
*selVarCrKksSottosistLabel.y: 10
*selVarCrKksSottosistLabel.width: 70
*selVarCrKksSottosistLabel.height: 20
*selVarCrKksSottosistLabel.labelString: KKS_SSYST_LABEL

*selVarCrKksComponentLabel.class: label
*selVarCrKksComponentLabel.static: true
*selVarCrKksComponentLabel.name: selVarCrKksComponentLabel
*selVarCrKksComponentLabel.parent: selVarCrKksBullBoard
*selVarCrKksComponentLabel.x: 210
*selVarCrKksComponentLabel.y: 10
*selVarCrKksComponentLabel.width: 70
*selVarCrKksComponentLabel.height: 20
*selVarCrKksComponentLabel.labelString: KKS_COMP_LABEL

*selVarCrKksText2.class: text
*selVarCrKksText2.static: true
*selVarCrKksText2.name: selVarCrKksText2
*selVarCrKksText2.parent: selVarCrKksBullBoard
*selVarCrKksText2.x: 80
*selVarCrKksText2.y: 30
*selVarCrKksText2.width: 25
*selVarCrKksText2.height: 25
*selVarCrKksText2.maxLength: 1
*selVarCrKksText2.marginHeight: 2
*selVarCrKksText2.marginWidth: 5

*selVarCrKksText3.class: text
*selVarCrKksText3.static: true
*selVarCrKksText3.name: selVarCrKksText3
*selVarCrKksText3.parent: selVarCrKksBullBoard
*selVarCrKksText3.x: 110
*selVarCrKksText3.y: 30
*selVarCrKksText3.width: 25
*selVarCrKksText3.height: 25
*selVarCrKksText3.maxLength: 1
*selVarCrKksText3.marginHeight: 2
*selVarCrKksText3.marginWidth: 5

*selVarCrKksText4.class: text
*selVarCrKksText4.static: true
*selVarCrKksText4.name: selVarCrKksText4
*selVarCrKksText4.parent: selVarCrKksBullBoard
*selVarCrKksText4.x: 150
*selVarCrKksText4.y: 30
*selVarCrKksText4.width: 25
*selVarCrKksText4.height: 25
*selVarCrKksText4.maxLength: 1
*selVarCrKksText4.marginHeight: 2
*selVarCrKksText4.marginWidth: 5

*selVarCrKksText5.class: text
*selVarCrKksText5.static: true
*selVarCrKksText5.name: selVarCrKksText5
*selVarCrKksText5.parent: selVarCrKksBullBoard
*selVarCrKksText5.x: 180
*selVarCrKksText5.y: 30
*selVarCrKksText5.width: 25
*selVarCrKksText5.height: 25
*selVarCrKksText5.maxLength: 1
*selVarCrKksText5.marginHeight: 2
*selVarCrKksText5.marginWidth: 5

*selVarCrKksText6.class: text
*selVarCrKksText6.static: true
*selVarCrKksText6.name: selVarCrKksText6
*selVarCrKksText6.parent: selVarCrKksBullBoard
*selVarCrKksText6.x: 220
*selVarCrKksText6.y: 30
*selVarCrKksText6.width: 25
*selVarCrKksText6.height: 25
*selVarCrKksText6.maxLength: 1
*selVarCrKksText6.marginHeight: 2
*selVarCrKksText6.marginWidth: 5

*selVarCrKksText7.class: text
*selVarCrKksText7.static: true
*selVarCrKksText7.name: selVarCrKksText7
*selVarCrKksText7.parent: selVarCrKksBullBoard
*selVarCrKksText7.x: 250
*selVarCrKksText7.y: 30
*selVarCrKksText7.width: 25
*selVarCrKksText7.height: 25
*selVarCrKksText7.maxLength: 1
*selVarCrKksText7.marginHeight: 2
*selVarCrKksText7.marginWidth: 5

*selVarCrKksText8.class: text
*selVarCrKksText8.static: true
*selVarCrKksText8.name: selVarCrKksText8
*selVarCrKksText8.parent: selVarCrKksBullBoard
*selVarCrKksText8.x: 290
*selVarCrKksText8.y: 30
*selVarCrKksText8.width: 25
*selVarCrKksText8.height: 25
*selVarCrKksText8.columns: 1
*selVarCrKksText8.maxLength: 1
*selVarCrKksText8.marginHeight: 2
*selVarCrKksText8.marginWidth: 5

*selVarCrKksProgressLabel.class: label
*selVarCrKksProgressLabel.static: true
*selVarCrKksProgressLabel.name: selVarCrKksProgressLabel
*selVarCrKksProgressLabel.parent: selVarCrKksBullBoard
*selVarCrKksProgressLabel.x: 270
*selVarCrKksProgressLabel.y: 10
*selVarCrKksProgressLabel.width: 110
*selVarCrKksProgressLabel.height: 20
*selVarCrKksProgressLabel.labelString: KKS_PROG_LABEL

*selVarCrKksText9.class: text
*selVarCrKksText9.static: true
*selVarCrKksText9.name: selVarCrKksText9
*selVarCrKksText9.parent: selVarCrKksBullBoard
*selVarCrKksText9.x: 320
*selVarCrKksText9.y: 30
*selVarCrKksText9.width: 25
*selVarCrKksText9.height: 25
*selVarCrKksText9.maxLength: 1
*selVarCrKksText9.marginHeight: 2
*selVarCrKksText9.marginWidth: 5

*selVarCrKksText10.class: text
*selVarCrKksText10.static: true
*selVarCrKksText10.name: selVarCrKksText10
*selVarCrKksText10.parent: selVarCrKksBullBoard
*selVarCrKksText10.x: 350
*selVarCrKksText10.y: 30
*selVarCrKksText10.width: 25
*selVarCrKksText10.height: 25
*selVarCrKksText10.maxLength: 1
*selVarCrKksText10.marginHeight: 2
*selVarCrKksText10.marginWidth: 5

*selVarCrKksText0.class: text
*selVarCrKksText0.static: true
*selVarCrKksText0.name: selVarCrKksText0
*selVarCrKksText0.parent: selVarCrKksBullBoard
*selVarCrKksText0.x: 10
*selVarCrKksText0.y: 30
*selVarCrKksText0.width: 25
*selVarCrKksText0.height: 25
*selVarCrKksText0.columns: 1
*selVarCrKksText0.maxLength: 1
*selVarCrKksText0.marginHeight: 2
*selVarCrKksText0.marginWidth: 5

*selVarCrKksNimpLabel.class: label
*selVarCrKksNimpLabel.static: true
*selVarCrKksNimpLabel.name: selVarCrKksNimpLabel
*selVarCrKksNimpLabel.parent: selVarCrKksBullBoard
*selVarCrKksNimpLabel.x: 10
*selVarCrKksNimpLabel.y: 10
*selVarCrKksNimpLabel.width: 30
*selVarCrKksNimpLabel.height: 20
*selVarCrKksNimpLabel.labelString: KKS_NIMP_LABEL

*pushButton1.class: pushButton
*pushButton1.static: true
*pushButton1.name: pushButton1
*pushButton1.parent: selVarCrKksBullBoard
*pushButton1.x: 410
*pushButton1.y: 30
*pushButton1.width: 120
*pushButton1.height: 25
*pushButton1.labelString: KKS_APPLY_FILTER
*pushButton1.activateCallback: {\
get_kks_filter (selVarCr);\
display_lista_var (selVarCr, modello_selezionato, blocco_selezionato,\
                 kks_filter,tipo_filter);\
}

*selVarCrTipoForm.class: form
*selVarCrTipoForm.static: true
*selVarCrTipoForm.name: selVarCrTipoForm
*selVarCrTipoForm.parent: panedWindow1
*selVarCrTipoForm.resizePolicy: "resize_none"
*selVarCrTipoForm.x: 10
*selVarCrTipoForm.y: 140
*selVarCrTipoForm.width: 520
*selVarCrTipoForm.height: 30
*selVarCrTipoForm.paneMinimum: 80
*selVarCrTipoForm.paneMaximum: 80

*selVarCrTipoPressionetb.class: toggleButton
*selVarCrTipoPressionetb.static: true
*selVarCrTipoPressionetb.name: selVarCrTipoPressionetb
*selVarCrTipoPressionetb.parent: selVarCrTipoForm
*selVarCrTipoPressionetb.x: 10
*selVarCrTipoPressionetb.y: 0
*selVarCrTipoPressionetb.width: 80
*selVarCrTipoPressionetb.height: 20
*selVarCrTipoPressionetb.labelString: TIPO_PRESSIONE
*selVarCrTipoPressionetb.alignment: "alignment_beginning"
*selVarCrTipoPressionetb.valueChangedCallback: {\
carica_filtro_tipo();\
}

*selVarCrTipoEntalpiatb.class: toggleButton
*selVarCrTipoEntalpiatb.static: true
*selVarCrTipoEntalpiatb.name: selVarCrTipoEntalpiatb
*selVarCrTipoEntalpiatb.parent: selVarCrTipoForm
*selVarCrTipoEntalpiatb.x: 90
*selVarCrTipoEntalpiatb.y: 40
*selVarCrTipoEntalpiatb.width: 80
*selVarCrTipoEntalpiatb.height: 20
*selVarCrTipoEntalpiatb.alignment: "alignment_beginning"
*selVarCrTipoEntalpiatb.labelString: TIPO_ENTALPIA
*selVarCrTipoEntalpiatb.valueChangedCallback: {\
carica_filtro_tipo();\
}

*selVarCrTipoPortatatb.class: toggleButton
*selVarCrTipoPortatatb.static: true
*selVarCrTipoPortatatb.name: selVarCrTipoPortatatb
*selVarCrTipoPortatatb.parent: selVarCrTipoForm
*selVarCrTipoPortatatb.x: 10
*selVarCrTipoPortatatb.y: 40
*selVarCrTipoPortatatb.width: 80
*selVarCrTipoPortatatb.height: 20
*selVarCrTipoPortatatb.alignment: "alignment_beginning"
*selVarCrTipoPortatatb.labelString: TIPO_PORTATA
*selVarCrTipoPortatatb.valueChangedCallback: {\
carica_filtro_tipo();\
}

*selVarCrTipoGiritb.class: toggleButton
*selVarCrTipoGiritb.static: true
*selVarCrTipoGiritb.name: selVarCrTipoGiritb
*selVarCrTipoGiritb.parent: selVarCrTipoForm
*selVarCrTipoGiritb.x: 90
*selVarCrTipoGiritb.y: 20
*selVarCrTipoGiritb.width: 80
*selVarCrTipoGiritb.height: 20
*selVarCrTipoGiritb.alignment: "alignment_beginning"
*selVarCrTipoGiritb.labelString: TIPO_GIRI
*selVarCrTipoGiritb.valueChangedCallback: {\
carica_filtro_tipo();\
}

*selVarCrTipoTemperaturatb.class: toggleButton
*selVarCrTipoTemperaturatb.static: true
*selVarCrTipoTemperaturatb.name: selVarCrTipoTemperaturatb
*selVarCrTipoTemperaturatb.parent: selVarCrTipoForm
*selVarCrTipoTemperaturatb.x: 10
*selVarCrTipoTemperaturatb.y: 20
*selVarCrTipoTemperaturatb.width: 80
*selVarCrTipoTemperaturatb.height: 20
*selVarCrTipoTemperaturatb.alignment: "alignment_beginning"
*selVarCrTipoTemperaturatb.labelString: TIPO_TEMPERATURA
*selVarCrTipoTemperaturatb.valueChangedCallback: {\
carica_filtro_tipo();\
}

*selVarCrTipoAlzvalvtb.class: toggleButton
*selVarCrTipoAlzvalvtb.static: true
*selVarCrTipoAlzvalvtb.name: selVarCrTipoAlzvalvtb
*selVarCrTipoAlzvalvtb.parent: selVarCrTipoForm
*selVarCrTipoAlzvalvtb.x: 170
*selVarCrTipoAlzvalvtb.y: 0
*selVarCrTipoAlzvalvtb.width: 90
*selVarCrTipoAlzvalvtb.height: 20
*selVarCrTipoAlzvalvtb.alignment: "alignment_beginning"
*selVarCrTipoAlzvalvtb.labelString: TIPO_ALZVALV
*selVarCrTipoAlzvalvtb.valueChangedCallback: {\
carica_filtro_tipo();\
}

*selVarCrTipoPotenzatb.class: toggleButton
*selVarCrTipoPotenzatb.static: true
*selVarCrTipoPotenzatb.name: selVarCrTipoPotenzatb
*selVarCrTipoPotenzatb.parent: selVarCrTipoForm
*selVarCrTipoPotenzatb.x: 90
*selVarCrTipoPotenzatb.y: 0
*selVarCrTipoPotenzatb.width: 80
*selVarCrTipoPotenzatb.height: 20
*selVarCrTipoPotenzatb.alignment: "alignment_beginning"
*selVarCrTipoPotenzatb.labelString: TIPO_POTENZA
*selVarCrTipoPotenzatb.valueChangedCallback: {\
carica_filtro_tipo();\
}

*pushButton2.class: pushButton
*pushButton2.static: true
*pushButton2.name: pushButton2
*pushButton2.parent: selVarCrTipoForm
*pushButton2.x: 410
*pushButton2.y: 40
*pushButton2.width: 120
*pushButton2.height: 25
*pushButton2.labelString: KKS_APPLY_FILTER
*pushButton2.activateCallback: {\
carica_filtro_tipo(); \
display_lista_var (selVarCr, modello_selezionato, blocco_selezionato,\
            kks_filter,tipo_filter);\
}

*selVarCrTipoAingable1.class: toggleButton
*selVarCrTipoAingable1.static: true
*selVarCrTipoAingable1.name: selVarCrTipoAingable1
*selVarCrTipoAingable1.parent: selVarCrTipoForm
*selVarCrTipoAingable1.x: 260
*selVarCrTipoAingable1.y: 0
*selVarCrTipoAingable1.width: 120
*selVarCrTipoAingable1.height: 20
*selVarCrTipoAingable1.alignment: "alignment_beginning"
*selVarCrTipoAingable1.labelString: TIPO_AINGABLE
*selVarCrTipoAingable1.valueChangedCallback: {\
carica_filtro_tipo ();\
}

*selVarCrTipoInput1.class: toggleButton
*selVarCrTipoInput1.static: true
*selVarCrTipoInput1.name: selVarCrTipoInput1
*selVarCrTipoInput1.parent: selVarCrTipoForm
*selVarCrTipoInput1.x: 260
*selVarCrTipoInput1.y: 20
*selVarCrTipoInput1.width: 120
*selVarCrTipoInput1.height: 20
*selVarCrTipoInput1.alignment: "alignment_beginning"
*selVarCrTipoInput1.labelString: TIPO_INPUT
*selVarCrTipoInput1.valueChangedCallback: {\
carica_filtro_tipo ();\
}

*selVarCrTipoOutput1.class: toggleButton
*selVarCrTipoOutput1.static: true
*selVarCrTipoOutput1.name: selVarCrTipoOutput1
*selVarCrTipoOutput1.parent: selVarCrTipoForm
*selVarCrTipoOutput1.x: 260
*selVarCrTipoOutput1.y: 40
*selVarCrTipoOutput1.width: 120
*selVarCrTipoOutput1.height: 20
*selVarCrTipoOutput1.alignment: "alignment_beginning"
*selVarCrTipoOutput1.labelString: TIPO_OUTPUT
*selVarCrTipoOutput1.valueChangedCallback: {\
carica_filtro_tipo ();\
}

*form36.class: form
*form36.static: true
*form36.name: form36
*form36.parent: panedWindow1
*form36.resizePolicy: "resize_none"
*form36.x: 13
*form36.y: 13
*form36.width: 540
*form36.height: 150
*form36.paneMinimum: 140

*label6.class: label
*label6.static: true
*label6.name: label6
*label6.parent: form36
*label6.x: 0
*label6.y: 0
*label6.width: 534
*label6.height: 30
*label6.leftOffset: 0
*label6.rightAttachment: "attach_form"
*label6.leftAttachment: "attach_form"
*label6.labelString: ""

*form38.class: form
*form38.static: true
*form38.name: form38
*form38.parent: form36
*form38.resizePolicy: "resize_none"
*form38.x: 0
*form38.y: 120
*form38.width: 534
*form38.height: 30
*form38.bottomAttachment: "attach_form"
*form38.rightAttachment: "attach_form"
*form38.leftAttachment: "attach_form"

*selVarFindUp.class: arrowButton
*selVarFindUp.static: true
*selVarFindUp.name: selVarFindUp
*selVarFindUp.parent: form38
*selVarFindUp.x: 10
*selVarFindUp.y: 0
*selVarFindUp.width: 60
*selVarFindUp.height: 30
*selVarFindUp.arrowDirection: "arrow_up"
*selVarFindUp.bottomAttachment: "attach_form"
*selVarFindUp.leftOffset: 20
*selVarFindUp.topAttachment: "attach_form"
*selVarFindUp.bottomOffset: 3
*selVarFindUp.topOffset: 3
*selVarFindUp.activateCallbackClientData: (XtPointer) 0x0
*selVarFindUp.activateCallback: {\
#ifndef DESIGN_TIME\
next_pos_to_sel (selVarToSelect,SEARCH_UP,selVarText);\
#endif\
}

*selVarText.class: textField
*selVarText.static: true
*selVarText.name: selVarText
*selVarText.parent: form38
*selVarText.x: 20
*selVarText.y: 10
*selVarText.width: 140
*selVarText.height: 20
*selVarText.bottomAttachment: "attach_form"
*selVarText.topAttachment: "attach_form"
*selVarText.leftAttachment: "attach_widget"
*selVarText.leftOffset: 5
*selVarText.leftWidget: "selVarFindUp"
*selVarText.bottomOffset: 0
*selVarText.topOffset: 0
*selVarText.marginHeight: 2

*selVarFindDown.class: arrowButton
*selVarFindDown.static: true
*selVarFindDown.name: selVarFindDown
*selVarFindDown.parent: form38
*selVarFindDown.x: 250
*selVarFindDown.y: 0
*selVarFindDown.width: 60
*selVarFindDown.height: 30
*selVarFindDown.arrowDirection: "arrow_down"
*selVarFindDown.leftAttachment: "attach_widget"
*selVarFindDown.leftOffset: 5
*selVarFindDown.leftWidget: "selVarText"
*selVarFindDown.bottomOffset: 3
*selVarFindDown.bottomAttachment: "attach_form"
*selVarFindDown.topAttachment: "attach_form"
*selVarFindDown.topOffset: 3
*selVarFindDown.activateCallback: {\
#ifndef DESIGN_TIME\
next_pos_to_sel (selVarToSelect,SEARCH_DOWN,selVarText);\
#endif\
}

*aggiungiVarSelpb.class: pushButton
*aggiungiVarSelpb.static: true
*aggiungiVarSelpb.name: aggiungiVarSelpb
*aggiungiVarSelpb.parent: form38
*aggiungiVarSelpb.x: 410
*aggiungiVarSelpb.y: 0
*aggiungiVarSelpb.width: 100
*aggiungiVarSelpb.height: 30
*aggiungiVarSelpb.leftAttachment: "attach_none"
*aggiungiVarSelpb.rightAttachment: "attach_form"
*aggiungiVarSelpb.rightOffset: 20
*aggiungiVarSelpb.topOffset: 3
*aggiungiVarSelpb.bottomAttachment: "attach_form"
*aggiungiVarSelpb.topAttachment: "attach_form"
*aggiungiVarSelpb.bottomOffset: 3
*aggiungiVarSelpb.labelString: SELECT_VAR
*aggiungiVarSelpb.activateCallback: {\
add_var (UxGetWidget(selVarToSelect), UxGetWidget(selVarToUnselect));\
}

*selezionaAllpb.class: pushButton
*selezionaAllpb.static: true
*selezionaAllpb.name: selezionaAllpb
*selezionaAllpb.parent: form38
*selezionaAllpb.x: 280
*selezionaAllpb.y: 0
*selezionaAllpb.width: 100
*selezionaAllpb.height: 30
*selezionaAllpb.bottomAttachment: "attach_form"
*selezionaAllpb.leftAttachment: "attach_widget"
*selezionaAllpb.leftOffset: 5
*selezionaAllpb.leftWidget: "selVarFindDown"
*selezionaAllpb.topAttachment: "attach_form"
*selezionaAllpb.topOffset: 3
*selezionaAllpb.bottomOffset: 3
*selezionaAllpb.labelString: SELECT_ALL
*selezionaAllpb.activateCallback: {\
set_cursor (padre,CLOCK);\
seleziona_all_to_sel (UxGetWidget(selVarToSelect),\
                (Widget)UxClientData);\
set_cursor (padre,NORMALE);\
}
*selezionaAllpb.activateCallbackClientData: UxGetWidget(selVarText)

*scrolledWindow8.class: scrolledWindow
*scrolledWindow8.static: true
*scrolledWindow8.name: scrolledWindow8
*scrolledWindow8.parent: form36
*scrolledWindow8.scrollingPolicy: "application_defined"
*scrolledWindow8.x: 20
*scrolledWindow8.y: 30
*scrolledWindow8.visualPolicy: "variable"
*scrolledWindow8.scrollBarDisplayPolicy: "static"
*scrolledWindow8.shadowThickness: 0
*scrolledWindow8.bottomAttachment: "attach_widget"
*scrolledWindow8.bottomOffset: 5
*scrolledWindow8.bottomWidget: "form38"
*scrolledWindow8.rightAttachment: "attach_form"
*scrolledWindow8.rightOffset: 20
*scrolledWindow8.leftAttachment: "attach_form"
*scrolledWindow8.leftOffset: 20
*scrolledWindow8.topAttachment: "attach_widget"
*scrolledWindow8.topOffset: 5
*scrolledWindow8.topWidget: "label6"

*selVarToSelect.class: scrolledList
*selVarToSelect.static: true
*selVarToSelect.name: selVarToSelect
*selVarToSelect.parent: scrolledWindow8
*selVarToSelect.width: 490
*selVarToSelect.height: 80
*selVarToSelect.selectionPolicy: "extended_select"

*form37.class: form
*form37.static: true
*form37.name: form37
*form37.parent: panedWindow1
*form37.resizePolicy: "resize_none"
*form37.x: 13
*form37.y: 171
*form37.width: 540
*form37.height: 150
*form37.paneMinimum: 140

*form35.class: form
*form35.static: true
*form35.name: form35
*form35.parent: form37
*form35.resizePolicy: "resize_none"
*form35.x: 0
*form35.y: 120
*form35.width: 534
*form35.height: 30
*form35.bottomAttachment: "attach_form"
*form35.rightAttachment: "attach_form"
*form35.rightOffset: 0
*form35.leftAttachment: "attach_form"

*deleteVarSelpb.class: pushButton
*deleteVarSelpb.static: true
*deleteVarSelpb.name: deleteVarSelpb
*deleteVarSelpb.parent: form35
*deleteVarSelpb.x: 410
*deleteVarSelpb.y: 0
*deleteVarSelpb.width: 100
*deleteVarSelpb.height: 30
*deleteVarSelpb.leftAttachment: "attach_none"
*deleteVarSelpb.rightAttachment: "attach_form"
*deleteVarSelpb.rightOffset: 20
*deleteVarSelpb.topOffset: 3
*deleteVarSelpb.bottomAttachment: "attach_form"
*deleteVarSelpb.topAttachment: "attach_form"
*deleteVarSelpb.bottomOffset: 3
*deleteVarSelpb.labelString: UNSELECT_VAR
*deleteVarSelpb.activateCallback: {\
del_var (UxGetWidget(selVarToUnselect), UxGetWidget(selVarToSelect));\
}

*unselVarFindUp.class: arrowButton
*unselVarFindUp.static: true
*unselVarFindUp.name: unselVarFindUp
*unselVarFindUp.parent: form35
*unselVarFindUp.x: 10
*unselVarFindUp.y: 0
*unselVarFindUp.width: 60
*unselVarFindUp.height: 30
*unselVarFindUp.arrowDirection: "arrow_up"
*unselVarFindUp.bottomAttachment: "attach_form"
*unselVarFindUp.leftOffset: 20
*unselVarFindUp.topAttachment: "attach_form"
*unselVarFindUp.topOffset: 3
*unselVarFindUp.bottomOffset: 3
*unselVarFindUp.activateCallback: {\
#ifndef DESIGN_TIME\
next_pos_to_unsel (selVarToUnselect,SEARCH_UP,unselVarText);\
#endif\
}

*unselVarText.class: textField
*unselVarText.static: true
*unselVarText.name: unselVarText
*unselVarText.parent: form35
*unselVarText.x: 20
*unselVarText.y: 10
*unselVarText.width: 140
*unselVarText.height: 20
*unselVarText.topOffset: 0
*unselVarText.leftAttachment: "attach_widget"
*unselVarText.leftOffset: 5
*unselVarText.leftWidget: "unselVarFindUp"
*unselVarText.bottomAttachment: "attach_form"
*unselVarText.topAttachment: "attach_form"
*unselVarText.marginHeight: 2

*unselVarFindDown.class: arrowButton
*unselVarFindDown.static: true
*unselVarFindDown.name: unselVarFindDown
*unselVarFindDown.parent: form35
*unselVarFindDown.x: 250
*unselVarFindDown.y: 0
*unselVarFindDown.width: 60
*unselVarFindDown.height: 30
*unselVarFindDown.arrowDirection: "arrow_down"
*unselVarFindDown.leftAttachment: "attach_widget"
*unselVarFindDown.leftOffset: 5
*unselVarFindDown.leftWidget: "unselVarText"
*unselVarFindDown.topOffset: 3
*unselVarFindDown.bottomAttachment: "attach_form"
*unselVarFindDown.bottomOffset: 3
*unselVarFindDown.topAttachment: "attach_form"
*unselVarFindDown.activateCallback: {\
#ifndef DESIGN_TIME\
next_pos_to_unsel (selVarToUnselect,SEARCH_DOWN,unselVarText);\
#endif\
}

*deselezionaAllpb.class: pushButton
*deselezionaAllpb.static: true
*deselezionaAllpb.name: deselezionaAllpb
*deselezionaAllpb.parent: form35
*deselezionaAllpb.x: 280
*deselezionaAllpb.y: 0
*deselezionaAllpb.width: 100
*deselezionaAllpb.height: 30
*deselezionaAllpb.leftAttachment: "attach_widget"
*deselezionaAllpb.leftOffset: 5
*deselezionaAllpb.leftWidget: "unselVarFindDown"
*deselezionaAllpb.bottomOffset: 3
*deselezionaAllpb.bottomAttachment: "attach_form"
*deselezionaAllpb.topAttachment: "attach_form"
*deselezionaAllpb.topOffset: 3
*deselezionaAllpb.labelString: SELECT_ALL
*deselezionaAllpb.activateCallback: {\
set_cursor (padre,CLOCK);\
seleziona_all_to_unsel (UxGetWidget (selVarToUnselect),\
                (Widget)UxClientData);\
set_cursor (padre,NORMALE);\
}
*deselezionaAllpb.activateCallbackClientData: UxGetWidget(unselVarText)

*label4.class: label
*label4.static: true
*label4.name: label4
*label4.parent: form37
*label4.x: 0
*label4.y: 0
*label4.width: 534
*label4.height: 30
*label4.labelString: ""

*scrolledWindow9.class: scrolledWindow
*scrolledWindow9.static: true
*scrolledWindow9.name: scrolledWindow9
*scrolledWindow9.parent: form37
*scrolledWindow9.scrollingPolicy: "application_defined"
*scrolledWindow9.x: -2
*scrolledWindow9.y: 8
*scrolledWindow9.visualPolicy: "variable"
*scrolledWindow9.scrollBarDisplayPolicy: "static"
*scrolledWindow9.shadowThickness: 0
*scrolledWindow9.bottomAttachment: "attach_widget"
*scrolledWindow9.bottomOffset: 5
*scrolledWindow9.bottomWidget: "form35"
*scrolledWindow9.leftOffset: 20
*scrolledWindow9.rightAttachment: "attach_form"
*scrolledWindow9.rightOffset: 20
*scrolledWindow9.leftAttachment: "attach_form"
*scrolledWindow9.topAttachment: "attach_widget"
*scrolledWindow9.topWidget: "label4"

*selVarToUnselect.class: scrolledList
*selVarToUnselect.static: true
*selVarToUnselect.name: selVarToUnselect
*selVarToUnselect.parent: scrolledWindow9
*selVarToUnselect.width: 490
*selVarToUnselect.height: 60
*selVarToUnselect.selectionPolicy: "extended_select"

*selVarCrPopup.class: rowColumn
*selVarCrPopup.static: true
*selVarCrPopup.name: selVarCrPopup
*selVarCrPopup.parent: panedWindow1
*selVarCrPopup.rowColumnType: "menu_popup"

*selVarCrPopupSaveFilter.class: pushButton
*selVarCrPopupSaveFilter.static: true
*selVarCrPopupSaveFilter.name: selVarCrPopupSaveFilter
*selVarCrPopupSaveFilter.parent: selVarCrPopup
*selVarCrPopupSaveFilter.labelString: SAVEFILTER
*selVarCrPopupSaveFilter.activateCallback: {\
if (SD_optsave3 (BANCO, &options,IcProt) > 0)\
      fprintf (stderr,"OPZIONI salvate\n");\
else\
      fprintf (stderr,"*** errore save opzioni\n");\
}

*selVarCrPopupApply.class: pushButton
*selVarCrPopupApply.static: true
*selVarCrPopupApply.name: selVarCrPopupApply
*selVarCrPopupApply.parent: selVarCrPopup
*selVarCrPopupApply.labelString: APPLYFILTER
*selVarCrPopupApply.activateCallback: {\
applicaFiltri (selVarCr, FILTRO_CR2);\
}

*menu9_p1_b3.class: separator
*menu9_p1_b3.static: true
*menu9_p1_b3.name: menu9_p1_b3
*menu9_p1_b3.parent: selVarCrPopup

*selVarCrPopupSave.class: pushButton
*selVarCrPopupSave.static: true
*selVarCrPopupSave.name: selVarCrPopupSave
*selVarCrPopupSave.parent: selVarCrPopup
*selVarCrPopupSave.labelString: CONF_ACCEPT
*selVarCrPopupSave.activateCallback: {\
#ifndef DESIGN_TIME\
set_cursor ((Widget)UxClientData, CLOCK);\
salva_sel_var (UxWidget, SEL_REG, FILE_REC_DEFAULT);\
newvarcr ((Widget)UxClientData);\
set_cursor ((Widget)UxClientData, NORMALE);\
sel_var_cr_interface_active = 0;\
set_var_cr_defaults((Widget)UxClientData);\
DistruggiInterfaccia (XtParent((Widget)UxClientData));\
#endif\
}
*selVarCrPopupSave.activateCallbackClientData: selVarCr

*menu9_p1_b5.class: separator
*menu9_p1_b5.static: true
*menu9_p1_b5.name: menu9_p1_b5
*menu9_p1_b5.parent: selVarCrPopup

*selVarCrPopupQuit.class: pushButton
*selVarCrPopupQuit.static: true
*selVarCrPopupQuit.name: selVarCrPopupQuit
*selVarCrPopupQuit.parent: selVarCrPopup
*selVarCrPopupQuit.labelString: CONF_DISCARD
*selVarCrPopupQuit.activateCallback: {\
DistruggiInterfaccia (XtParent(selVarCr));\
}

