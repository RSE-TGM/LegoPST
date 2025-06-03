! UIMX ascii 2.8 key: 5327                                                      

*selVar.class: formDialog
*selVar.gbldecl: #include <stdio.h>\
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
#include "filtri.h"\
#include "malfunzioni.h"\
#include "tabelle_malf.h"\
\
extern M_COMP_TYPE *m_comp_type;\
extern MALF_SET *malf_set;\
extern M_COMPONENT *m_component;\
\
extern M_COMP_TYPE *fr_comp_type;\
extern MALF_SET *frem_set;\
extern M_COMPONENT *fr_component;\
\
#ifndef DESIGN_TIME\
/* #include "interf_attive.h" */\
#include "preview.h"\
#include "masterMenu.h"\
#include "previewSnap.h"\
#endif\
#include "cont_rec.h"\
#ifndef MAX_LUN_NOME_VAR\
#define MAX_LUN_NOME_VAR 9\
#endif\
char label_sel[MAX_LUN_NOME_VAR];\
extern OPTIONS_FLAGS options;\
extern Boolean *IcProt;\

*selVar.ispecdecl: int mod_sel;\
int blocco_sel;\
int kks_filter_act;\
int tipo_filter_act;\
char stringa_kks[12];\
FILTRO_TIPI filtro_tipo;\
int *punt_var; /* vettore dei puntatori delle variabili selezionate */\
int *punt_var_kks;\
int *punt_var_tipo;\
int *punt_var_lista;\
int n_varlist;\
int n_varlist_kks;\
int n_varlist_tipo;\
int n_varlist_lista;\
int n_blocchi;\
\
/* per find variabili */\
int first_time;\
int sel_mod;    /* selezione filtri modificata */\
Boolean *match; /* vettore elementi matching   */\
int last_match; /* ultimo elemento selezionato */\
int nmatch;     /* numero elementi matching   */\
int first_time_find; /* primo giro per find    */\
char *stringa_match; /* input utente            */\
char *old_stringa_match;\
\
/* per find blocchi */\
int first_time_b;\
int sel_mod_b;    /* selezione filtri modificata */\
Boolean *match_b; /* vettore elementi matching   */\
int last_match_b; /* ultimo elemento selezionato */\
int nmatch_b;     /* numero elementi matching   */\
int first_time_find_b; /* primo giro per find    */\
char *stringa_match_b; /* input utente            */\
char *old_stringa_match_b;\
/*\
 * aggiunte per filtri malfunzioni\
 */\
int *punt_conf_malf;    /* puntatori in m_component di selezMalf  */\
int n_conf_malf;\
int *punt_malf_kks;     /* puntatori in m_component di KKS filtrati */\
int n_conf_malf_kks;\
int *punt_malf_tipo;    /* puntatori in m_component di tipo filtrati */\
int n_conf_malf_tipo;\
int *punt_malf_lista;   /* puntatori in m_component in lista         */\
int n_conf_malf_lista;\
int filtro_kks_malf_conf;\
char filter_kks_malf_conf[12];\
int filtro_tipo_malf_conf;\
FILTRO_TIPI filtro_tipi_malf_conf;\
int conf_malf_select;\
int primo_giro;\

*selVar.ispeclist: mod_sel, blocco_sel, kks_filter_act, tipo_filter_act, stringa_kks, filtro_tipo, punt_var, punt_var_kks, punt_var_tipo, punt_var_lista, n_varlist, n_varlist_kks, n_varlist_tipo, n_varlist_lista, n_blocchi, first_time, sel_mod, match, last_match, nmatch, first_time_find, stringa_match, old_stringa_match, first_time_b, sel_mod_b, match_b, last_match_b, nmatch_b, first_time_find_b, stringa_match_b, old_stringa_match_b, punt_conf_malf, n_conf_malf, punt_malf_kks, n_conf_malf_kks, punt_malf_tipo, n_conf_malf_tipo, punt_malf_lista, n_conf_malf_lista, filtro_kks_malf_conf, filter_kks_malf_conf, filtro_tipo_malf_conf, filtro_tipi_malf_conf, conf_malf_select, primo_giro
*selVar.ispeclist.mod_sel: "int", "%mod_sel%"
*selVar.ispeclist.blocco_sel: "int", "%blocco_sel%"
*selVar.ispeclist.kks_filter_act: "int", "%kks_filter_act%"
*selVar.ispeclist.tipo_filter_act: "int", "%tipo_filter_act%"
*selVar.ispeclist.stringa_kks: "char", "%stringa_kks%[12]"
*selVar.ispeclist.filtro_tipo: "FILTRO_TIPI", "%filtro_tipo%"
*selVar.ispeclist.punt_var: "int", "*%punt_var%"
*selVar.ispeclist.punt_var_kks: "int", "*%punt_var_kks%"
*selVar.ispeclist.punt_var_tipo: "int", "*%punt_var_tipo%"
*selVar.ispeclist.punt_var_lista: "int", "*%punt_var_lista%"
*selVar.ispeclist.n_varlist: "int", "%n_varlist%"
*selVar.ispeclist.n_varlist_kks: "int", "%n_varlist_kks%"
*selVar.ispeclist.n_varlist_tipo: "int", "%n_varlist_tipo%"
*selVar.ispeclist.n_varlist_lista: "int", "%n_varlist_lista%"
*selVar.ispeclist.n_blocchi: "int", "%n_blocchi%"
*selVar.ispeclist.first_time: "int", "%first_time%"
*selVar.ispeclist.sel_mod: "int", "%sel_mod%"
*selVar.ispeclist.match: "Boolean", "*%match%"
*selVar.ispeclist.last_match: "int", "%last_match%"
*selVar.ispeclist.nmatch: "int", "%nmatch%"
*selVar.ispeclist.first_time_find: "int", "%first_time_find%"
*selVar.ispeclist.stringa_match: "char", "*%stringa_match%"
*selVar.ispeclist.old_stringa_match: "char", "*%old_stringa_match%"
*selVar.ispeclist.first_time_b: "int", "%first_time_b%"
*selVar.ispeclist.sel_mod_b: "int", "%sel_mod_b%"
*selVar.ispeclist.match_b: "Boolean", "*%match_b%"
*selVar.ispeclist.last_match_b: "int", "%last_match_b%"
*selVar.ispeclist.nmatch_b: "int", "%nmatch_b%"
*selVar.ispeclist.first_time_find_b: "int", "%first_time_find_b%"
*selVar.ispeclist.stringa_match_b: "char", "*%stringa_match_b%"
*selVar.ispeclist.old_stringa_match_b: "char", "*%old_stringa_match_b%"
*selVar.ispeclist.punt_conf_malf: "int", "*%punt_conf_malf%"
*selVar.ispeclist.n_conf_malf: "int", "%n_conf_malf%"
*selVar.ispeclist.punt_malf_kks: "int", "*%punt_malf_kks%"
*selVar.ispeclist.n_conf_malf_kks: "int", "%n_conf_malf_kks%"
*selVar.ispeclist.punt_malf_tipo: "int", "*%punt_malf_tipo%"
*selVar.ispeclist.n_conf_malf_tipo: "int", "%n_conf_malf_tipo%"
*selVar.ispeclist.punt_malf_lista: "int", "*%punt_malf_lista%"
*selVar.ispeclist.n_conf_malf_lista: "int", "%n_conf_malf_lista%"
*selVar.ispeclist.filtro_kks_malf_conf: "int", "%filtro_kks_malf_conf%"
*selVar.ispeclist.filter_kks_malf_conf: "char", "%filter_kks_malf_conf%[12]"
*selVar.ispeclist.filtro_tipo_malf_conf: "int", "%filtro_tipo_malf_conf%"
*selVar.ispeclist.filtro_tipi_malf_conf: "FILTRO_TIPI", "%filtro_tipi_malf_conf%"
*selVar.ispeclist.conf_malf_select: "int", "%conf_malf_select%"
*selVar.ispeclist.primo_giro: "int", "%primo_giro%"
*selVar.funcdecl: Widget create_selVar (w, p, chi, header)\
Widget w;        /* widget padre */\
char *p; /* struttura PREVIEW_DATA da riempire o scenario malf/frem */\
int chi; /* interfaccia chiamante               */\
char *header; /* titolo finestra                  */
*selVar.funcname: create_selVar
*selVar.funcdef: "Widget", "<create_selVar>(%)"
*selVar.argdecl: Widget w;\
char *p;\
int chi;\
char *header;
*selVar.arglist: w, p, chi, header
*selVar.arglist.w: "Widget", "%w%"
*selVar.arglist.p: "char", "*%p%"
*selVar.arglist.chi: "int", "%chi%"
*selVar.arglist.header: "char", "*%header%"
*selVar.icode: char *s;\
set_cursor (w,CLOCK);\
first_time = 1;\
first_time_b = 1;\
first_time_find = 1;\
first_time_find_b = 1;\
strcpy (label_sel,"");\
/* per malf */\
primo_giro = 1; /* primo run di questa istanza */\
conf_malf_select = -1; /* preset invalido  */
*selVar.fcode: XtVaSetValues (XtParent(UxGetWidget(rtrn)), \
         XmNtitle, header, XmNiconName, header, NULL);\
presetFilterDefaults1 (rtrn);\
set_var_default (selVarListaSistemi1);\
\
if ((chi != MALFUNZIONI) && (chi != SOMMARIO_MALF) &&\
    (chi != FUNZIONI_REMOTE) && (chi != SOMMARIO_FREM))\
   {\
   UxPopupInterface (rtrn, no_grab);\
   lettura_filtro_kks (selVarListaSistemi1);\
   lettura_filtro_tipo (selVarListaSistemi1);\
   display_lista_sistema (selVarListaSistemi1);\
   display_blocchi (selVarListaBlocchi1);\
   display_var (selVarToSelect1,mod_sel,blocco_sel,\
               kks_filter_act,tipo_filter_act);\
   }\
else\
   {\
   readConfMalf (UxGetWidget(rtrn),chi);\
/*   setConfMalfDefaults (rtrn); */\
/*   getKksFilterMalfConf (selVarToSelect1,filter_kks_malf_conf); */\
   getKksFilterMalfConf (selVarToSelect1,stringa_kks);\
   lettura_filtro_tipo (selVarListaSistemi1);\
   display_lista_sistema (selVarListaSistemi1);\
   display_blocchi (selVarListaBlocchi1);\
   UxPopupInterface (rtrn, no_grab);\
\
   switch (chi)\
      {\
      case MALFUNZIONI:\
      case SOMMARIO_MALF:  \
         printf ("SelezMalf : Display malf \n");\
         display_conf_malf (selVarToSelect1,m_component,\
                   malf_set, m_comp_type,mod_sel,blocco_sel,\
                   kks_filter_act,tipo_filter_act);\
   \
         break;\
      case FUNZIONI_REMOTE:\
      case SOMMARIO_FREM:\
         printf ("SelezMalf : Display frem \n");\
         display_conf_malf (selVarToSelect1,fr_component,\
                   frem_set, fr_comp_type,mod_sel,blocco_sel,\
                   kks_filter_act,tipo_filter_act);\
   \
         break;\
      }\
   XtVaSetValues (selOkpb,XmNsensitive, False, NULL);\
   XtVaSetValues (selVarMenuPopupSelect,XmNsensitive, False, NULL);\
   }\
\
set_cursor (w,NORMALE);\
return(UxGetWidget(rtrn));\

*selVar.auxdecl: int presetFilterDefaults1 (ppp)\
Widget ppp;\
{\
if ((chi != MALFUNZIONI) && (chi != SOMMARIO_MALF) &&      /* aing o preview  */\
    (chi != FUNZIONI_REMOTE) && (chi != SOMMARIO_FREM))\
   {\
	XmToggleButtonSetState (UxGetWidget(selByAllVariablestb1),options.optionsFilterAing.ByAllVar,False);\
	XmToggleButtonSetState (UxGetWidget(selByModeltb1),options.optionsFilterAing.ByModBlock,False);\
	XmToggleButtonSetState (UxGetWidget(selByModelOnlytb1),options.optionsFilterAing.ByMod,False);\
	XmToggleButtonSetState (UxGetWidget(selVarMenuKkstb1),options.optionsFilterAing.ByKks,False);\
	XmToggleButtonSetState (UxGetWidget(selVarMenuTipotb1),options.optionsFilterAing.ByType,False);\
\
	XmToggleButtonSetState (UxGetWidget(selVarCrAllVartb1),options.optionsFilterAing.ByAllVar,False);\
	XmToggleButtonSetState (UxGetWidget(selVarCrModBlocktb1),options.optionsFilterAing.ByModBlock,False);\
	XmToggleButtonSetState (UxGetWidget(selVarCrModtb1),options.optionsFilterAing.ByMod,False);\
	XmToggleButtonSetState (UxGetWidget(selVarCrKkstb1),options.optionsFilterAing.ByKks,False);\
	XmToggleButtonSetState (UxGetWidget(selVarCrTypetb1),options.optionsFilterAing.ByType,False);\
   }\
else       /* malfunzioni o funzioni remote */\
   {\
   if ((chi == MALFUNZIONI) || (chi == SOMMARIO_MALF))\
      {\
      XmToggleButtonSetState (UxGetWidget(selVarCrAllVartb1),options.optionsFilterMalf.ByAllVar,False);\
      XmToggleButtonSetState (UxGetWidget(selVarCrModBlocktb1),options.optionsFilterMalf.ByModBlock,False);\
      XmToggleButtonSetState (UxGetWidget(selVarCrModtb1),options.optionsFilterMalf.ByMod,False);\
      XmToggleButtonSetState (UxGetWidget(selVarCrKkstb1),options.optionsFilterMalf.ByKks,False);\
      XmToggleButtonSetState (UxGetWidget(selVarCrTypetb1),options.optionsFilterMalf.ByType,False);\
      }\
   if ((chi == FUNZIONI_REMOTE) || (chi == SOMMARIO_FREM))\
      {\
      XmToggleButtonSetState (UxGetWidget(selVarCrAllVartb1),options.optionsFilterFrem.ByAllVar,False);\
      XmToggleButtonSetState (UxGetWidget(selVarCrModBlocktb1),options.optionsFilterFrem.ByModBlock,False);\
      XmToggleButtonSetState (UxGetWidget(selVarCrModtb1),options.optionsFilterFrem.ByMod,False);\
      XmToggleButtonSetState (UxGetWidget(selVarCrKkstb1),options.optionsFilterFrem.ByKks,False);\
      XmToggleButtonSetState (UxGetWidget(selVarCrTypetb1),options.optionsFilterFrem.ByType,False);\
      }\
\
   }\
\
	return (0);\
}\
/**********************************************************/\
int set_var_default (oggetto)\
Widget oggetto;\
{\
#ifndef DESIGN_TIME\
Boolean stato;\
\
_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(oggetto);\
\
   if (Ctx->Uxchi == AING_CALLING)   /* blocca il filtro per tipo*/\
      default_aing (oggetto);\
      \
/* legge lo stato del toggle button di filtro */\
   stato = XmToggleButtonGetState (Ctx->UxselVarCrAllVartb1); \
   switch (stato)\
      {\
      case 0:\
         Ctx->Uxmod_sel = 1;\
         Ctx->Uxblocco_sel = 1;\
         XtManageChild (Ctx->UxselByModelForm1);\
         break;\
      case 1:\
         Ctx->Uxmod_sel = NO_MOD;\
         Ctx->Uxblocco_sel = NO_BLOCK;\
         XtUnmanageChild (Ctx->UxselByModelForm1);\
         break;\
      }\
/* legge lo stato del toggle button di filtro */\
   stato = XmToggleButtonGetState (Ctx->UxselVarCrModBlocktb1); \
   switch (stato)\
      {\
      case 1:\
         Ctx->Uxmod_sel = 1;\
         Ctx->Uxblocco_sel = 1;\
         XtManageChild (Ctx->UxselByModelForm1);\
         break;\
      case 0:\
         Ctx->Uxmod_sel = NO_MOD;\
         Ctx->Uxblocco_sel = NO_BLOCK;\
         XtUnmanageChild (Ctx->UxselByModelForm1);\
         break;\
      }\
/* legge lo stato del toggle button del filtro kks */\
    stato = XmToggleButtonGetState (Ctx->UxselVarCrKkstb1);  \
   switch (stato)\
      {\
      case 1:\
         Ctx->Uxkks_filter_act = 1;\
         XtManageChild (Ctx->UxselVarCrKksForm2);\
         break;\
      case 0:\
         Ctx->Uxkks_filter_act = 0;\
         XtUnmanageChild (Ctx->UxselVarCrKksForm2);\
         break;\
      }\
/* legge lo stato del toggle button del filtro per tipo */\
    stato = XmToggleButtonGetState (Ctx->UxselVarCrTypetb1);  \
   switch (stato)\
      {\
      case 1:\
         Ctx->Uxtipo_filter_act = 1;\
         lettura_filtro_tipo (Ctx->UxselVarCrTipoForm1);\
         XtManageChild (Ctx->UxselVarCrTipoForm1);\
         break;\
      case 0:\
         Ctx->Uxtipo_filter_act = 0;\
         XtUnmanageChild (Ctx->UxselVarCrTipoForm1);\
         break;\
      }\
#endif\
   return (0);\
}\
/*************************************************/\
int lettura_filtro_tipo (oggetto)\
Widget oggetto;\
{\
#ifndef DESIGN_TIME\
_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(oggetto);\
\
   Ctx->Uxfiltro_tipo.pressione = XmToggleButtonGetState (Ctx->UxselVarCrTipoPressionetb1);\
   Ctx->Uxfiltro_tipo.temperatura = XmToggleButtonGetState (Ctx->UxselVarCrTipoTemperaturatb1);\
   Ctx->Uxfiltro_tipo.entalpia = XmToggleButtonGetState (Ctx->UxselVarCrTipoEntalpiatb1);\
   Ctx->Uxfiltro_tipo.portata = XmToggleButtonGetState (Ctx->UxselVarCrTipoPortatatb1);\
   Ctx->Uxfiltro_tipo.potenza = XmToggleButtonGetState (Ctx->UxselVarCrTipoPotenzatb1);\
   Ctx->Uxfiltro_tipo.giri = XmToggleButtonGetState (Ctx->UxselVarCrTipoGiritb1);\
   Ctx->Uxfiltro_tipo.alzvalv = XmToggleButtonGetState (Ctx->UxselVarCrTipoAlzvalvtb1);\
   Ctx->Uxfiltro_tipo.ingressi = XmToggleButtonGetState (Ctx->UxselVarCrTipoInput);\
   Ctx->Uxfiltro_tipo.uscite = XmToggleButtonGetState (Ctx->UxselVarCrTipoOutput);\
   Ctx->Uxfiltro_tipo.aingabili = XmToggleButtonGetState (Ctx->UxselVarCrTipoAingable);\
\
   return (0);\
#endif\
}\
/*************************************************/\
int default_aing (oggetto)\
Widget oggetto;\
{\
#ifndef DESIGN_TIME\
_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(oggetto);\
\
   XmToggleButtonSetState (Ctx->UxselVarMenuTipotb1,True,False);\
   XtVaSetValues (Ctx->UxselVarMenuTipotb1,XmNsensitive,False,NULL);\
\
   XmToggleButtonSetState (Ctx->UxselVarCrTipoAingable,True,False);\
   XtVaSetValues (Ctx->UxselVarCrTipoAingable,XmNsensitive,False,NULL);\
\
   XmToggleButtonSetState (Ctx->UxselVarCrTypetb1,True,False);\
   XtVaSetValues (Ctx->UxselVarCrTypetb1,XmNsensitive,False,NULL);\
\
   XtUnmanageChild (Ctx->UxselVarCrTipoOutput);\
   XtUnmanageChild (Ctx->UxselVarCrTipoInput);\
#endif\
return(0);\
}\
/*************************************************/\
int getKksFilterMalfConf (oggetto, f)\
Widget oggetto;\
char *f;\
{\
_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(oggetto);\
#ifndef DESIGN_TIME\
   printf ("lettura filtro kks\n");\
   get_byte_kks (Ctx->UxselVarCrKksText32, &f[0]);\
   get_byte_kks (Ctx->UxselVarCrKksText22,&f[1]);\
   get_byte_kks (Ctx->UxselVarCrKksText23,&f[2]);\
   get_byte_kks (Ctx->UxselVarCrKksText24,&f[3]);\
   get_byte_kks (Ctx->UxselVarCrKksText25,&f[4]);\
   get_byte_kks (Ctx->UxselVarCrKksText26,&f[5]);\
   get_byte_kks (Ctx->UxselVarCrKksText27,&f[6]);\
   get_byte_kks (Ctx->UxselVarCrKksText28,&f[7]);\
   get_byte_kks (Ctx->UxselVarCrKksText29,&f[8]);\
   get_byte_kks (Ctx->UxselVarCrKksText30,&f[9]);\
   get_byte_kks (Ctx->UxselVarCrKksText31,&f[10]);\
   f[11] = '\0';\
   printf ("kks_filtro = :%s:\n",f);\
#endif\
   return (0);\
}\
/*************************************************/\
int lettura_filtro_kks (oggetto)\
Widget oggetto;\
{\
_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(oggetto);\
#ifndef DESIGN_TIME\
   printf ("lettura filtro kks\n");\
   get_byte_kks (Ctx->UxselVarCrKksText32, &Ctx->Uxstringa_kks[0]);\
   get_byte_kks (Ctx->UxselVarCrKksText22,&Ctx->Uxstringa_kks[1]);\
   get_byte_kks (Ctx->UxselVarCrKksText23,&Ctx->Uxstringa_kks[2]);\
   get_byte_kks (Ctx->UxselVarCrKksText24,&Ctx->Uxstringa_kks[3]);\
   get_byte_kks (Ctx->UxselVarCrKksText25,&Ctx->Uxstringa_kks[4]);\
   get_byte_kks (Ctx->UxselVarCrKksText26,&Ctx->Uxstringa_kks[5]);\
   get_byte_kks (Ctx->UxselVarCrKksText27,&Ctx->Uxstringa_kks[6]);\
   get_byte_kks (Ctx->UxselVarCrKksText28,&Ctx->Uxstringa_kks[7]);\
   get_byte_kks (Ctx->UxselVarCrKksText29,&Ctx->Uxstringa_kks[8]);\
   get_byte_kks (Ctx->UxselVarCrKksText30,&Ctx->Uxstringa_kks[9]);\
   get_byte_kks (Ctx->UxselVarCrKksText31,&Ctx->Uxstringa_kks[10]);\
   Ctx->Uxstringa_kks[11] = '\0';\
   printf ("kks_filtro = :%s:\n",Ctx->Uxstringa_kks);\
#endif\
   return (0);\
}\
/*************************************************/\
int setConfMalfDefaults  (wgt)\
Widget wgt;\
{\
   filtro_kks_malf_conf = 1;\
   filtro_tipo_malf_conf = 0;\
}\
/*************************************************/\
int selectSelVarOk (Widget wgt)\
{\
_UxCselVar *Ctx = (_UxCselVar *)UxGetContext(wgt);\
int i;\
#ifndef DESIGN_TIME\
if (chi == PREVIEW_CALLING)\
   {\
   if (strcmp(label_sel,""))\
      {\
      i = get_label_indx (label_sel);\
printf ("selvarok: :%s: ind %d\n",label_sel,i);\
      add_preview_entry (w, p, NON_NOTA, i);\
      get_val_view (w);\
      display_view (w);\
      }\
   }\
else if (chi == AING_CALLING)\
   {\
   printf ("Chiamata da AING\n");\
   if (strcmp(label_sel,""))\
      {\
      attiva_aing (wgt, label_sel);\
      }\
   }\
else   /* malf o frem */\
   {\
   if (add_item_scenario_malf (w,selVar,\
                 conf_malf_select, p, chi) > MAX_VAR_SCENARIO)\
      {  \
      XtVaSetValues (selOkpb,XmNsensitive,False,NULL);\
      XtVaSetValues (selVarMenuPopupSelect,XmNsensitive,False,NULL);\
      }\
   }\
#endif\
}\
/****************************************************/\
int displayLista ()\
{\
\
set_cursor (selVar, CLOCK);\
\
if ((chi != MALFUNZIONI) && (chi != SOMMARIO_MALF) &&\
    (chi != FUNZIONI_REMOTE) && (chi != SOMMARIO_FREM))\
   {\
   display_var (selVarToSelect1, mod_sel, blocco_sel,\
            kks_filter_act,tipo_filter_act);\
   }\
else\
   {\
   switch (chi)\
      {\
      case MALFUNZIONI:\
      case SOMMARIO_MALF:\
         display_conf_malf (selVarToSelect1,m_component,\
                   malf_set, m_comp_type,mod_sel,blocco_sel,\
                   kks_filter_act,tipo_filter_act);\
   \
         break;\
      case FUNZIONI_REMOTE:\
      case SOMMARIO_FREM:\
         display_conf_malf (selVarToSelect1,fr_component,\
                   frem_set, fr_comp_type,mod_sel,blocco_sel,\
                   kks_filter_act,tipo_filter_act);\
   \
         break;\
      }\
   }\
set_cursor (selVar, NORMALE);\
}
*selVar.static: true
*selVar.name: selVar
*selVar.parent: NO_PARENT
*selVar.parentExpression: w
*selVar.defaultShell: topLevelShell
*selVar.unitType: "pixels"
*selVar.x: 290
*selVar.y: 10
*selVar.width: 550
*selVar.height: 700
*selVar.defaultButton: "selOkpb"
*selVar.destroyCallback: {\
#ifndef DESIGN_TIME\
_UxCpreviewSnap *CtxPsnap;\
_UxCmasterMenu *CtxMaster;\
if (chi == PREVIEW_CALLING)\
   {\
   CtxPsnap = (_UxCpreviewSnap *)UxGetContext(w);\
   CtxPsnap->UxselVarWidg = (Widget)NULL;\
   }\
else if (chi == AING_CALLING)\
   {\
   CtxMaster = (_UxCmasterMenu *)UxGetContext(w);\
   CtxMaster->UxselVarAing = (Widget)NULL;\
   }\
#endif\
}

*frame2.class: frame
*frame2.static: true
*frame2.name: frame2
*frame2.parent: selVar
*frame2.x: 0
*frame2.y: 480
*frame2.width: 540
*frame2.height: 80
*frame2.bottomAttachment: "attach_form"
*frame2.rightAttachment: "attach_form"
*frame2.leftAttachment: "attach_form"

*form42.class: form
*form42.static: true
*form42.name: form42
*form42.parent: frame2
*form42.resizePolicy: "resize_none"
*form42.x: 10
*form42.y: 10
*form42.width: 490
*form42.height: 50
*form42.defaultButton: "selOkpb"

*selOkpb.class: pushButton
*selOkpb.static: true
*selOkpb.name: selOkpb
*selOkpb.parent: form42
*selOkpb.x: 20
*selOkpb.y: 20
*selOkpb.width: 100
*selOkpb.height: 40
*selOkpb.topOffset: 15
*selOkpb.bottomAttachment: "attach_form"
*selOkpb.bottomOffset: 15
*selOkpb.rightAttachment: "attach_self"
*selOkpb.topAttachment: "attach_form"
*selOkpb.labelString: SELVAR_CONFIRM ? SELVAR_CONFIRM : "OK"
*selOkpb.leftAttachment: "attach_form"
*selOkpb.leftOffset: 20
*selOkpb.activateCallback: {\
selectSelVarOk (UxWidget);\
}
*selOkpb.activateCallbackClientData: UxGetWidget(selVar)

*selQuitpb.class: pushButton
*selQuitpb.static: true
*selQuitpb.name: selQuitpb
*selQuitpb.parent: form42
*selQuitpb.x: 410
*selQuitpb.y: 20
*selQuitpb.width: 100
*selQuitpb.height: 40
*selQuitpb.bottomAttachment: "attach_form"
*selQuitpb.bottomOffset: 15
*selQuitpb.leftAttachment: "attach_self"
*selQuitpb.leftOffset: 0
*selQuitpb.leftPosition: 0
*selQuitpb.rightAttachment: "attach_form"
*selQuitpb.rightOffset: 20
*selQuitpb.topAttachment: "attach_form"
*selQuitpb.topOffset: 15
*selQuitpb.activateCallbackClientData: selVar
*selQuitpb.labelString: CONF_DISCARD
*selQuitpb.activateCallback: {\
/********** in destroy callback di selVar\
#ifndef DESIGN_TIME\
_UxCpreviewSnap *CtxPsnap;\
_UxCmasterMenu *CtxMaster;\
if (chi == PREVIEW_CALLING)\
   {\
   CtxPsnap = (_UxCpreviewSnap *)UxGetContext(w);\
   CtxPsnap->UxselVarWidg = (Widget)NULL;\
   }\
else if (chi == AING_CALLING)\
   {\
   CtxMaster = (_UxCmasterMenu *)UxGetContext(w);\
   CtxMaster->UxselVarAing = (Widget)NULL;\
   }\
#endif\
************/\
DistruggiInterfaccia (XtParent((Widget)UxClientData));\
}

*menu4.class: rowColumn
*menu4.static: true
*menu4.name: menu4
*menu4.parent: selVar
*menu4.rowColumnType: "menu_bar"
*menu4.menuAccelerator: "<KeyUp>F10"
*menu4.rightAttachment: "attach_form"
*menu4.leftAttachment: "attach_form"

*selVarCrMenu1.class: rowColumn
*selVarCrMenu1.static: true
*selVarCrMenu1.name: selVarCrMenu1
*selVarCrMenu1.parent: menu4
*selVarCrMenu1.rowColumnType: "menu_pulldown"

*selVarCrMenuFilepb1.class: pushButton
*selVarCrMenuFilepb1.static: true
*selVarCrMenuFilepb1.name: selVarCrMenuFilepb1
*selVarCrMenuFilepb1.parent: selVarCrMenu1
*selVarCrMenuFilepb1.labelString: "Quit"
*selVarCrMenuFilepb1.activateCallback: {\
/*** in destroy callback di selVar\
#ifndef DESIGN_TIME\
_UxCpreviewSnap *CtxPsnap;\
_UxCmasterMenu *CtxMaster;\
if (chi == PREVIEW_CALLING)\
   {\
   CtxPsnap = (_UxCpreviewSnap *)UxGetContext(w);\
   CtxPsnap->UxselVarWidg = (Widget)NULL;\
   }\
else if (chi == AING_CALLING)\
   {\
   CtxMaster = (_UxCmasterMenu *)UxGetContext(w);\
   CtxMaster->UxselVarAing = (Widget)NULL;\
   }\
#endif\
***************/\
DistruggiInterfaccia (XtParent((Widget)UxClientData));\
\
}
*selVarCrMenuFilepb1.activateCallbackClientData: selVar

*selVarMenuFilter1.class: rowColumn
*selVarMenuFilter1.static: true
*selVarMenuFilter1.name: selVarMenuFilter1
*selVarMenuFilter1.parent: menu4
*selVarMenuFilter1.rowColumnType: "menu_pulldown"
*selVarMenuFilter1.createManaged: "false"

*selVarMenuFilter_b1.class: cascadeButton
*selVarMenuFilter_b1.static: true
*selVarMenuFilter_b1.name: selVarMenuFilter_b1
*selVarMenuFilter_b1.parent: selVarMenuFilter1
*selVarMenuFilter_b1.labelString: "Select filter ..."
*selVarMenuFilter_b1.subMenuId: "selVarMenuOtherPane1"

*selVarMenuOtherPane1.class: rowColumn
*selVarMenuOtherPane1.static: true
*selVarMenuOtherPane1.name: selVarMenuOtherPane1
*selVarMenuOtherPane1.parent: selVarMenuFilter1
*selVarMenuOtherPane1.rowColumnType: "menu_pulldown"

*selByAllVariablestb1.class: toggleButton
*selByAllVariablestb1.static: true
*selByAllVariablestb1.name: selByAllVariablestb1
*selByAllVariablestb1.parent: selVarMenuOtherPane1
*selByAllVariablestb1.labelString: "All variables"
*selByAllVariablestb1.indicatorType: "one_of_many"
*selByAllVariablestb1.visibleWhenOff: "true"
*selByAllVariablestb1.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVar),\
                  UxGetWidget(selByModelForm1), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                   FILTER_ALL_VAR,FILTRO_VAR);\
display_var (UxGetWidget(selVarToSelect1), mod_sel, blocco_sel,\
                     kks_filter_act,tipo_filter_act);\
}
*selByAllVariablestb1.set: "true"

*selByModeltb1.class: toggleButton
*selByModeltb1.static: true
*selByModeltb1.name: selByModeltb1
*selByModeltb1.parent: selVarMenuOtherPane1
*selByModeltb1.labelString: "models & blocks"
*selByModeltb1.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVar),\
                  UxGetWidget(selByModelForm1), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                   FILTER_MOD_BLOC,FILTRO_VAR);\
display_var (UxGetWidget(selVarToSelect1), mod_sel, blocco_sel,\
                     kks_filter_act,tipo_filter_act);\
}
*selByModeltb1.set: "false"
*selByModeltb1.armCallback: {\
\
}
*selByModeltb1.createCallback: {\
\
}
*selByModeltb1.visibleWhenOff: "true"
*selByModeltb1.indicatorType: "one_of_many"

*selByModelOnlytb1.class: toggleButton
*selByModelOnlytb1.static: true
*selByModelOnlytb1.name: selByModelOnlytb1
*selByModelOnlytb1.parent: selVarMenuOtherPane1
*selByModelOnlytb1.labelString: "models only"
*selByModelOnlytb1.indicatorType: "one_of_many"
*selByModelOnlytb1.visibleWhenOff: "true"
*selByModelOnlytb1.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVar),\
                  UxGetWidget(selByModelForm1), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                   FILTER_MOD_ONLY,FILTRO_VAR);\
display_var (UxGetWidget(selVarToSelect1), mod_sel, blocco_sel,\
                     kks_filter_act,tipo_filter_act);\
}

*selVarMenuOtherPane_b1.class: separator
*selVarMenuOtherPane_b1.static: true
*selVarMenuOtherPane_b1.name: selVarMenuOtherPane_b1
*selVarMenuOtherPane_b1.parent: selVarMenuOtherPane1

*selVarMenuKkstb1.class: toggleButton
*selVarMenuKkstb1.static: true
*selVarMenuKkstb1.name: selVarMenuKkstb1
*selVarMenuKkstb1.parent: selVarMenuOtherPane1
*selVarMenuKkstb1.labelString: "KKS filter"
*selVarMenuKkstb1.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVar),\
                  UxGetWidget(selVarCrKksForm2), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                  FILTER_KKS,FILTRO_VAR);\
display_var (UxGetWidget(selVarToSelect1),\
                  mod_sel, blocco_sel,\
                  kks_filter_act,tipo_filter_act);\
}
*selVarMenuKkstb1.set: "true"
*selVarMenuKkstb1.visibleWhenOff: "true"
*selVarMenuKkstb1.indicatorType: "n_of_many"

*selVarMenuOtherPane_b2.class: separator
*selVarMenuOtherPane_b2.static: true
*selVarMenuOtherPane_b2.name: selVarMenuOtherPane_b2
*selVarMenuOtherPane_b2.parent: selVarMenuOtherPane1

*selVarMenuTipotb1.class: toggleButton
*selVarMenuTipotb1.static: true
*selVarMenuTipotb1.name: selVarMenuTipotb1
*selVarMenuTipotb1.parent: selVarMenuOtherPane1
*selVarMenuTipotb1.labelString: "by type"
*selVarMenuTipotb1.visibleWhenOff: "true"
*selVarMenuTipotb1.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVar),\
                  UxGetWidget(selVarCrTipoForm1), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                  FILTER_TYPE,FILTRO_VAR);\
display_var (UxGetWidget(selVarToSelect1),\
                  mod_sel, blocco_sel,\
                  kks_filter_act,tipo_filter_act);\
}

*menu3_top_b3.class: cascadeButton
*menu3_top_b3.static: true
*menu3_top_b3.name: menu3_top_b3
*menu3_top_b3.parent: menu4
*menu3_top_b3.labelString: "File"
*menu3_top_b3.subMenuId: "selVarCrMenu1"

*menu3_top_b4.class: cascadeButton
*menu3_top_b4.static: true
*menu3_top_b4.name: menu3_top_b4
*menu3_top_b4.parent: menu4
*menu3_top_b4.labelString: "Filter"
*menu3_top_b4.subMenuId: "selVarMenuFilter1"
*menu3_top_b4.createManaged: "false"

*frame27.class: frame
*frame27.static: true
*frame27.name: frame27
*frame27.parent: selVar
*frame27.x: 0
*frame27.y: 480
*frame27.width: 540
*frame27.height: 80
*frame27.rightAttachment: "attach_form"
*frame27.topAttachment: "attach_widget"
*frame27.topOffset: 0
*frame27.topWidget: "menu4"
*frame27.leftAttachment: "attach_form"

*selVarCrFilterForm1.class: form
*selVarCrFilterForm1.static: true
*selVarCrFilterForm1.name: selVarCrFilterForm1
*selVarCrFilterForm1.parent: frame27
*selVarCrFilterForm1.resizePolicy: "resize_none"
*selVarCrFilterForm1.x: 2
*selVarCrFilterForm1.y: 2
*selVarCrFilterForm1.width: 536
*selVarCrFilterForm1.height: 58

*frame28.class: frame
*frame28.static: true
*frame28.name: frame28
*frame28.parent: selVarCrFilterForm1
*frame28.x: 210
*frame28.y: 10
*frame28.width: 150
*frame28.height: 60
*frame28.bottomAttachment: "attach_form"
*frame28.bottomOffset: 2
*frame28.leftAttachment: "attach_none"
*frame28.leftOffset: 0
*frame28.topAttachment: "attach_form"
*frame28.topOffset: 2
*frame28.rightAttachment: "attach_form"
*frame28.rightOffset: 2

*form51.class: form
*form51.static: true
*form51.name: form51
*form51.parent: frame28
*form51.resizePolicy: "resize_none"
*form51.x: 50
*form51.y: 10
*form51.width: 80
*form51.height: 30

*selVarCrAllVartb1.class: toggleButton
*selVarCrAllVartb1.static: true
*selVarCrAllVartb1.name: selVarCrAllVartb1
*selVarCrAllVartb1.parent: form51
*selVarCrAllVartb1.x: 10
*selVarCrAllVartb1.y: 0
*selVarCrAllVartb1.width: 128
*selVarCrAllVartb1.height: 20
*selVarCrAllVartb1.indicatorType: "one_of_many"
*selVarCrAllVartb1.labelString: "All Variables"
*selVarCrAllVartb1.alignment: "alignment_beginning"
*selVarCrAllVartb1.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVar),\
                  UxGetWidget(selByModelForm1), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                   FILTER_ALL_VAR,FILTRO_VAR2);\
}

*selVarCrModBlocktb1.class: toggleButton
*selVarCrModBlocktb1.static: true
*selVarCrModBlocktb1.name: selVarCrModBlocktb1
*selVarCrModBlocktb1.parent: form51
*selVarCrModBlocktb1.x: 10
*selVarCrModBlocktb1.y: 20
*selVarCrModBlocktb1.width: 128
*selVarCrModBlocktb1.height: 20
*selVarCrModBlocktb1.indicatorType: "one_of_many"
*selVarCrModBlocktb1.labelString: "Mod.&Blocks"
*selVarCrModBlocktb1.alignment: "alignment_beginning"
*selVarCrModBlocktb1.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVar),\
                  UxGetWidget(selByModelForm1), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                   FILTER_MOD_BLOC,FILTRO_VAR2);\
}

*selVarCrModtb1.class: toggleButton
*selVarCrModtb1.static: true
*selVarCrModtb1.name: selVarCrModtb1
*selVarCrModtb1.parent: form51
*selVarCrModtb1.x: 10
*selVarCrModtb1.y: 40
*selVarCrModtb1.width: 130
*selVarCrModtb1.height: 20
*selVarCrModtb1.indicatorType: "one_of_many"
*selVarCrModtb1.labelString: "Models Only"
*selVarCrModtb1.alignment: "alignment_beginning"
*selVarCrModtb1.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVar),\
                  UxGetWidget(selByModelForm1), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                   FILTER_MOD_ONLY,FILTRO_VAR2);\
}

*frame29.class: frame
*frame29.static: true
*frame29.name: frame29
*frame29.parent: selVarCrFilterForm1
*frame29.x: 40
*frame29.y: 20
*frame29.width: 150
*frame29.height: 60
*frame29.bottomAttachment: "attach_form"
*frame29.bottomOffset: 2
*frame29.leftAttachment: "attach_none"
*frame29.leftOffset: 0
*frame29.topAttachment: "attach_form"
*frame29.topOffset: 2
*frame29.rightAttachment: "attach_widget"
*frame29.rightOffset: 2
*frame29.rightWidget: "frame28"

*form52.class: form
*form52.static: true
*form52.name: form52
*form52.parent: frame29
*form52.resizePolicy: "resize_none"
*form52.x: 10
*form52.y: 5
*form52.width: 138
*form52.height: 56

*selVarCrKkstb1.class: toggleButton
*selVarCrKkstb1.static: true
*selVarCrKkstb1.name: selVarCrKkstb1
*selVarCrKkstb1.parent: form52
*selVarCrKkstb1.x: 0
*selVarCrKkstb1.y: 0
*selVarCrKkstb1.width: 100
*selVarCrKkstb1.height: 20
*selVarCrKkstb1.alignment: "alignment_beginning"
*selVarCrKkstb1.labelString: "by KKS"
*selVarCrKkstb1.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVar),\
                  UxGetWidget(selVarCrKksForm2), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                  FILTER_KKS,FILTRO_VAR2);\
}

*selVarCrTypetb1.class: toggleButton
*selVarCrTypetb1.static: true
*selVarCrTypetb1.name: selVarCrTypetb1
*selVarCrTypetb1.parent: form52
*selVarCrTypetb1.x: 0
*selVarCrTypetb1.y: 20
*selVarCrTypetb1.width: 100
*selVarCrTypetb1.height: 20
*selVarCrTypetb1.alignment: "alignment_beginning"
*selVarCrTypetb1.labelString: "by Type"
*selVarCrTypetb1.valueChangedCallback: {\
manage_widget_filter (UxGetWidget(selVar),\
                  UxGetWidget(selVarCrTipoForm1), \
                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,\
                  FILTER_TYPE,FILTRO_VAR2);\
}

*selVarCrSaveFilterpb1.class: pushButton
*selVarCrSaveFilterpb1.static: true
*selVarCrSaveFilterpb1.name: selVarCrSaveFilterpb1
*selVarCrSaveFilterpb1.parent: selVarCrFilterForm1
*selVarCrSaveFilterpb1.x: 10
*selVarCrSaveFilterpb1.y: 10
*selVarCrSaveFilterpb1.width: 190
*selVarCrSaveFilterpb1.height: 30
*selVarCrSaveFilterpb1.labelString: SAVEFILTER
*selVarCrSaveFilterpb1.activateCallback: {\
if (SD_optsave3 (BANCO, &options,IcProt) > 0)\
      fprintf (stderr,"OPZIONI salvate\n");\
else\
      fprintf (stderr,"*** errore save opzioni\n");\
}

*selVarCrApplyFilterpb1.class: pushButton
*selVarCrApplyFilterpb1.static: true
*selVarCrApplyFilterpb1.name: selVarCrApplyFilterpb1
*selVarCrApplyFilterpb1.parent: selVarCrFilterForm1
*selVarCrApplyFilterpb1.x: 10
*selVarCrApplyFilterpb1.y: 40
*selVarCrApplyFilterpb1.width: 190
*selVarCrApplyFilterpb1.height: 30
*selVarCrApplyFilterpb1.labelString: APPLYFILTER
*selVarCrApplyFilterpb1.activateCallback: {\
if ((chi == MALFUNZIONI) || (chi == SOMMARIO_MALF))\
   applicaFiltri (selVar, FILTRO_MALF);\
else if ((chi == FUNZIONI_REMOTE) || (chi == SOMMARIO_FREM))\
   applicaFiltri (selVar, FILTRO_FREM);\
else\
   applicaFiltri (selVar, FILTRO_VAR2);\
\
}

*panedWindow2.class: panedWindow
*panedWindow2.static: true
*panedWindow2.name: panedWindow2
*panedWindow2.parent: selVar
*panedWindow2.x: 0
*panedWindow2.y: 40
*panedWindow2.width: 540
*panedWindow2.height: 440
*panedWindow2.bottomAttachment: "attach_widget"
*panedWindow2.bottomWidget: "frame2"
*panedWindow2.rightAttachment: "attach_form"
*panedWindow2.leftAttachment: "attach_form"
*panedWindow2.topAttachment: "attach_widget"
*panedWindow2.topWidget: "frame27"

*selByModelForm1.class: form
*selByModelForm1.static: true
*selByModelForm1.name: selByModelForm1
*selByModelForm1.parent: panedWindow2
*selByModelForm1.resizePolicy: "resize_none"
*selByModelForm1.x: 0
*selByModelForm1.y: 10
*selByModelForm1.width: 540
*selByModelForm1.height: 150
*selByModelForm1.createManaged: "true"
*selByModelForm1.paneMinimum: 100

*label8.class: label
*label8.static: true
*label8.name: label8
*label8.parent: selByModelForm1
*label8.x: 220
*label8.y: 0
*label8.width: 170
*label8.height: 30
*label8.alignment: "alignment_beginning"
*label8.topOffset: 0
*label8.labelString: BLOCK_LIST 

*label9.class: label
*label9.static: true
*label9.name: label9
*label9.parent: selByModelForm1
*label9.x: 20
*label9.y: 0
*label9.width: 170
*label9.height: 30
*label9.alignment: "alignment_beginning"
*label9.labelString: SYSTEM_LIST ? SYSTEM_LIST : "SYSTEMS"

*form43.class: form
*form43.static: true
*form43.name: form43
*form43.parent: selByModelForm1
*form43.resizePolicy: "resize_none"
*form43.x: 10
*form43.y: 140
*form43.width: 534
*form43.height: 30
*form43.bottomAttachment: "attach_form"
*form43.leftOffset: 0
*form43.rightAttachment: "attach_form"
*form43.leftAttachment: "attach_form"

*selVarModFindDown.class: arrowButton
*selVarModFindDown.static: true
*selVarModFindDown.name: selVarModFindDown
*selVarModFindDown.parent: form43
*selVarModFindDown.x: 250
*selVarModFindDown.y: 0
*selVarModFindDown.width: 60
*selVarModFindDown.height: 30
*selVarModFindDown.arrowDirection: "arrow_down"
*selVarModFindDown.leftAttachment: "attach_none"
*selVarModFindDown.leftOffset: 5
*selVarModFindDown.leftWidget: ""
*selVarModFindDown.bottomOffset: 3
*selVarModFindDown.bottomAttachment: "attach_form"
*selVarModFindDown.topAttachment: "attach_form"
*selVarModFindDown.topOffset: 3
*selVarModFindDown.activateCallback: {\
#ifndef DESIGN_TIME\
next_pos_bloc (selVarListaBlocchi1,SEARCH_DOWN,selVarText2);\
#endif\
}
*selVarModFindDown.rightAttachment: "attach_form"
*selVarModFindDown.rightOffset: 15

*selVarText2.class: textField
*selVarText2.static: true
*selVarText2.name: selVarText2
*selVarText2.parent: form43
*selVarText2.x: 320
*selVarText2.y: 0
*selVarText2.width: 150
*selVarText2.height: 30
*selVarText2.bottomAttachment: "attach_form"
*selVarText2.topAttachment: "attach_form"
*selVarText2.leftAttachment: "attach_none"
*selVarText2.leftOffset: 0
*selVarText2.leftWidget: ""
*selVarText2.bottomOffset: 0
*selVarText2.topOffset: 0
*selVarText2.rightAttachment: "attach_widget"
*selVarText2.rightOffset: 5
*selVarText2.rightWidget: "selVarModFindDown"
*selVarText2.marginHeight: 2

*selVarModFindUp.class: arrowButton
*selVarModFindUp.static: true
*selVarModFindUp.name: selVarModFindUp
*selVarModFindUp.parent: form43
*selVarModFindUp.x: 10
*selVarModFindUp.y: 0
*selVarModFindUp.width: 60
*selVarModFindUp.height: 30
*selVarModFindUp.arrowDirection: "arrow_up"
*selVarModFindUp.bottomAttachment: "attach_form"
*selVarModFindUp.leftOffset: 0
*selVarModFindUp.topAttachment: "attach_form"
*selVarModFindUp.bottomOffset: 3
*selVarModFindUp.topOffset: 3
*selVarModFindUp.activateCallbackClientData: (XtPointer) 0x0
*selVarModFindUp.activateCallback: {\
#ifndef DESIGN_TIME\
next_pos_bloc (selVarListaBlocchi1,SEARCH_UP,selVarText2);\
#endif\
}
*selVarModFindUp.leftAttachment: "attach_none"
*selVarModFindUp.rightAttachment: "attach_widget"
*selVarModFindUp.rightOffset: 5
*selVarModFindUp.rightWidget: "selVarText2"

*scrolledWindow10.class: scrolledWindow
*scrolledWindow10.static: true
*scrolledWindow10.name: scrolledWindow10
*scrolledWindow10.parent: selByModelForm1
*scrolledWindow10.scrollingPolicy: "application_defined"
*scrolledWindow10.x: 0
*scrolledWindow10.y: 30
*scrolledWindow10.visualPolicy: "variable"
*scrolledWindow10.scrollBarDisplayPolicy: "static"
*scrolledWindow10.shadowThickness: 0
*scrolledWindow10.bottomAttachment: "attach_widget"
*scrolledWindow10.bottomOffset: 5
*scrolledWindow10.leftOffset: 20
*scrolledWindow10.topAttachment: "attach_widget"
*scrolledWindow10.topWidget: "label9"
*scrolledWindow10.width: 180
*scrolledWindow10.leftAttachment: "attach_form"
*scrolledWindow10.bottomWidget: "form43"

*selVarListaSistemi1.class: scrolledList
*selVarListaSistemi1.static: true
*selVarListaSistemi1.name: selVarListaSistemi1
*selVarListaSistemi1.parent: scrolledWindow10
*selVarListaSistemi1.width: 170
*selVarListaSistemi1.height: 115
*selVarListaSistemi1.singleSelectionCallback: {\
XmListCallbackStruct *reason;\
   reason = (XmListCallbackStruct *)UxCallbackArg;\
   mod_sel = reason->item_position;\
\
printf ("selezionato modello %d\n",mod_sel);\
/*   display_lista_sistema (selVarListaSistemi1); */\
   display_blocchi (selVarListaBlocchi1);\
/***\
   display_var (selVarToSelect1, mod_sel, blocco_sel,\
                kks_filter_act,tipo_filter_act);\
***/\
   displayLista ();\
}
*selVarListaSistemi1.automaticSelection: "true"
*selVarListaSistemi1.selectionPolicy: "single_select"

*scrolledWindow11.class: scrolledWindow
*scrolledWindow11.static: true
*scrolledWindow11.name: scrolledWindow11
*scrolledWindow11.parent: selByModelForm1
*scrolledWindow11.scrollingPolicy: "application_defined"
*scrolledWindow11.x: 215
*scrolledWindow11.y: 30
*scrolledWindow11.visualPolicy: "variable"
*scrolledWindow11.scrollBarDisplayPolicy: "static"
*scrolledWindow11.shadowThickness: 0
*scrolledWindow11.bottomAttachment: "attach_widget"
*scrolledWindow11.bottomOffset: 5
*scrolledWindow11.leftAttachment: "attach_widget"
*scrolledWindow11.leftOffset: 15
*scrolledWindow11.leftWidget: "scrolledWindow10"
*scrolledWindow11.topAttachment: "attach_widget"
*scrolledWindow11.topWidget: "label8"
*scrolledWindow11.width: 300
*scrolledWindow11.rightAttachment: "attach_form"
*scrolledWindow11.rightOffset: 20
*scrolledWindow11.bottomWidget: "form43"

*selVarListaBlocchi1.class: scrolledList
*selVarListaBlocchi1.static: true
*selVarListaBlocchi1.name: selVarListaBlocchi1
*selVarListaBlocchi1.parent: scrolledWindow11
*selVarListaBlocchi1.width: 270
*selVarListaBlocchi1.height: 115
*selVarListaBlocchi1.singleSelectionCallback: {\
XmListCallbackStruct *reason;\
reason = (XmListCallbackStruct *)UxCallbackArg;\
\
   blocco_sel = reason->item_position;\
printf ("selezionato blocco %d\n",blocco_sel);\
/*****\
   display_var (selVarToSelect1, mod_sel, blocco_sel,\
                 kks_filter_act,tipo_filter_act);\
**********/\
   displayLista ();\
\
}
*selVarListaBlocchi1.automaticSelection: "true"
*selVarListaBlocchi1.selectionPolicy: "single_select"

*selVarCrKksForm2.class: form
*selVarCrKksForm2.static: true
*selVarCrKksForm2.name: selVarCrKksForm2
*selVarCrKksForm2.parent: panedWindow2
*selVarCrKksForm2.resizePolicy: "resize_none"
*selVarCrKksForm2.x: 0
*selVarCrKksForm2.y: 200
*selVarCrKksForm2.width: 540
*selVarCrKksForm2.height: 160
*selVarCrKksForm2.paneMinimum: 80
*selVarCrKksForm2.paneMaximum: 80

*selVarCrKksBullBoard2.class: bulletinBoard
*selVarCrKksBullBoard2.static: true
*selVarCrKksBullBoard2.name: selVarCrKksBullBoard2
*selVarCrKksBullBoard2.parent: selVarCrKksForm2
*selVarCrKksBullBoard2.resizePolicy: "resize_none"
*selVarCrKksBullBoard2.x: 0
*selVarCrKksBullBoard2.y: 0
*selVarCrKksBullBoard2.width: 530
*selVarCrKksBullBoard2.height: 100
*selVarCrKksBullBoard2.bottomAttachment: "attach_form"
*selVarCrKksBullBoard2.topAttachment: "attach_form"
*selVarCrKksBullBoard2.rightAttachment: "attach_form"
*selVarCrKksBullBoard2.leftAttachment: "attach_form"

*selVarCrKksText22.class: text
*selVarCrKksText22.static: true
*selVarCrKksText22.name: selVarCrKksText22
*selVarCrKksText22.parent: selVarCrKksBullBoard2
*selVarCrKksText22.x: 50
*selVarCrKksText22.y: 40
*selVarCrKksText22.width: 25
*selVarCrKksText22.height: 25
*selVarCrKksText22.columns: 1
*selVarCrKksText22.maxLength: 1
*selVarCrKksText22.marginHeight: 2

*selVarCrKksSistemaLabel2.class: label
*selVarCrKksSistemaLabel2.static: true
*selVarCrKksSistemaLabel2.name: selVarCrKksSistemaLabel2
*selVarCrKksSistemaLabel2.parent: selVarCrKksBullBoard2
*selVarCrKksSistemaLabel2.x: 60
*selVarCrKksSistemaLabel2.y: 20
*selVarCrKksSistemaLabel2.width: 70
*selVarCrKksSistemaLabel2.height: 20
*selVarCrKksSistemaLabel2.labelString: KKS_SYST_LABEL

*selVarCrKksSottosistLabel2.class: label
*selVarCrKksSottosistLabel2.static: true
*selVarCrKksSottosistLabel2.name: selVarCrKksSottosistLabel2
*selVarCrKksSottosistLabel2.parent: selVarCrKksBullBoard2
*selVarCrKksSottosistLabel2.x: 140
*selVarCrKksSottosistLabel2.y: 20
*selVarCrKksSottosistLabel2.width: 70
*selVarCrKksSottosistLabel2.height: 20
*selVarCrKksSottosistLabel2.labelString: KKS_SSYST_LABEL

*selVarCrKksComponentLabel2.class: label
*selVarCrKksComponentLabel2.static: true
*selVarCrKksComponentLabel2.name: selVarCrKksComponentLabel2
*selVarCrKksComponentLabel2.parent: selVarCrKksBullBoard2
*selVarCrKksComponentLabel2.x: 210
*selVarCrKksComponentLabel2.y: 20
*selVarCrKksComponentLabel2.width: 70
*selVarCrKksComponentLabel2.height: 20
*selVarCrKksComponentLabel2.labelString: KKS_COMP_LABEL

*selVarCrKksText23.class: text
*selVarCrKksText23.static: true
*selVarCrKksText23.name: selVarCrKksText23
*selVarCrKksText23.parent: selVarCrKksBullBoard2
*selVarCrKksText23.x: 80
*selVarCrKksText23.y: 40
*selVarCrKksText23.width: 25
*selVarCrKksText23.height: 25
*selVarCrKksText23.maxLength: 1
*selVarCrKksText23.marginHeight: 2

*selVarCrKksText24.class: text
*selVarCrKksText24.static: true
*selVarCrKksText24.name: selVarCrKksText24
*selVarCrKksText24.parent: selVarCrKksBullBoard2
*selVarCrKksText24.x: 110
*selVarCrKksText24.y: 40
*selVarCrKksText24.width: 25
*selVarCrKksText24.height: 25
*selVarCrKksText24.maxLength: 1
*selVarCrKksText24.marginHeight: 2

*selVarCrKksText25.class: text
*selVarCrKksText25.static: true
*selVarCrKksText25.name: selVarCrKksText25
*selVarCrKksText25.parent: selVarCrKksBullBoard2
*selVarCrKksText25.x: 150
*selVarCrKksText25.y: 40
*selVarCrKksText25.width: 25
*selVarCrKksText25.height: 25
*selVarCrKksText25.maxLength: 1
*selVarCrKksText25.marginHeight: 2

*selVarCrKksText26.class: text
*selVarCrKksText26.static: true
*selVarCrKksText26.name: selVarCrKksText26
*selVarCrKksText26.parent: selVarCrKksBullBoard2
*selVarCrKksText26.x: 180
*selVarCrKksText26.y: 40
*selVarCrKksText26.width: 25
*selVarCrKksText26.height: 25
*selVarCrKksText26.maxLength: 1
*selVarCrKksText26.marginHeight: 2

*selVarCrKksText27.class: text
*selVarCrKksText27.static: true
*selVarCrKksText27.name: selVarCrKksText27
*selVarCrKksText27.parent: selVarCrKksBullBoard2
*selVarCrKksText27.x: 220
*selVarCrKksText27.y: 40
*selVarCrKksText27.width: 25
*selVarCrKksText27.height: 25
*selVarCrKksText27.maxLength: 1
*selVarCrKksText27.marginHeight: 2

*selVarCrKksText28.class: text
*selVarCrKksText28.static: true
*selVarCrKksText28.name: selVarCrKksText28
*selVarCrKksText28.parent: selVarCrKksBullBoard2
*selVarCrKksText28.x: 250
*selVarCrKksText28.y: 40
*selVarCrKksText28.width: 25
*selVarCrKksText28.height: 25
*selVarCrKksText28.maxLength: 1
*selVarCrKksText28.marginHeight: 2

*selVarCrKksText29.class: text
*selVarCrKksText29.static: true
*selVarCrKksText29.name: selVarCrKksText29
*selVarCrKksText29.parent: selVarCrKksBullBoard2
*selVarCrKksText29.x: 290
*selVarCrKksText29.y: 40
*selVarCrKksText29.width: 25
*selVarCrKksText29.height: 25
*selVarCrKksText29.columns: 1
*selVarCrKksText29.maxLength: 1
*selVarCrKksText29.marginHeight: 2

*selVarCrKksProgressLabel2.class: label
*selVarCrKksProgressLabel2.static: true
*selVarCrKksProgressLabel2.name: selVarCrKksProgressLabel2
*selVarCrKksProgressLabel2.parent: selVarCrKksBullBoard2
*selVarCrKksProgressLabel2.x: 280
*selVarCrKksProgressLabel2.y: 20
*selVarCrKksProgressLabel2.width: 110
*selVarCrKksProgressLabel2.height: 20
*selVarCrKksProgressLabel2.labelString: KKS_PROG_LABEL

*selVarCrKksText30.class: text
*selVarCrKksText30.static: true
*selVarCrKksText30.name: selVarCrKksText30
*selVarCrKksText30.parent: selVarCrKksBullBoard2
*selVarCrKksText30.x: 320
*selVarCrKksText30.y: 40
*selVarCrKksText30.width: 25
*selVarCrKksText30.height: 25
*selVarCrKksText30.maxLength: 1
*selVarCrKksText30.marginHeight: 2

*selVarCrKksText31.class: text
*selVarCrKksText31.static: true
*selVarCrKksText31.name: selVarCrKksText31
*selVarCrKksText31.parent: selVarCrKksBullBoard2
*selVarCrKksText31.x: 350
*selVarCrKksText31.y: 40
*selVarCrKksText31.width: 25
*selVarCrKksText31.height: 25
*selVarCrKksText31.maxLength: 1
*selVarCrKksText31.marginHeight: 2

*selVarCrKksText32.class: text
*selVarCrKksText32.static: true
*selVarCrKksText32.name: selVarCrKksText32
*selVarCrKksText32.parent: selVarCrKksBullBoard2
*selVarCrKksText32.x: 10
*selVarCrKksText32.y: 40
*selVarCrKksText32.width: 25
*selVarCrKksText32.height: 25
*selVarCrKksText32.columns: 1
*selVarCrKksText32.maxLength: 1
*selVarCrKksText32.marginHeight: 2

*selVarCrKksNimpLabel2.class: label
*selVarCrKksNimpLabel2.static: true
*selVarCrKksNimpLabel2.name: selVarCrKksNimpLabel2
*selVarCrKksNimpLabel2.parent: selVarCrKksBullBoard2
*selVarCrKksNimpLabel2.x: 10
*selVarCrKksNimpLabel2.y: 20
*selVarCrKksNimpLabel2.width: 40
*selVarCrKksNimpLabel2.height: 20
*selVarCrKksNimpLabel2.labelString: KKS_NIMP_LABEL

*pushButton4.class: pushButton
*pushButton4.static: true
*pushButton4.name: pushButton4
*pushButton4.parent: selVarCrKksBullBoard2
*pushButton4.x: 420
*pushButton4.y: 40
*pushButton4.width: 120
*pushButton4.height: 25
*pushButton4.labelString: KKS_APPLY_FILTER
*pushButton4.activateCallback: {\
lettura_filtro_kks (UxWidget);\
displayLista ();\
/*******\
display_var (selVarToSelect1, mod_sel, blocco_sel,\
                 kks_filter_act,tipo_filter_act);\
AGGIUNTE LISTE MALF\
 **********/\
}

*selVarCrTipoForm1.class: form
*selVarCrTipoForm1.static: true
*selVarCrTipoForm1.name: selVarCrTipoForm1
*selVarCrTipoForm1.parent: panedWindow2
*selVarCrTipoForm1.resizePolicy: "resize_none"
*selVarCrTipoForm1.x: 10
*selVarCrTipoForm1.y: 140
*selVarCrTipoForm1.width: 520
*selVarCrTipoForm1.height: 30
*selVarCrTipoForm1.paneMinimum: 80
*selVarCrTipoForm1.paneMaximum: 80

*selVarCrTipoPressionetb1.class: toggleButton
*selVarCrTipoPressionetb1.static: true
*selVarCrTipoPressionetb1.name: selVarCrTipoPressionetb1
*selVarCrTipoPressionetb1.parent: selVarCrTipoForm1
*selVarCrTipoPressionetb1.x: 10
*selVarCrTipoPressionetb1.y: 0
*selVarCrTipoPressionetb1.width: 90
*selVarCrTipoPressionetb1.height: 20
*selVarCrTipoPressionetb1.labelString: TIPO_PRESSIONE
*selVarCrTipoPressionetb1.alignment: "alignment_beginning"
*selVarCrTipoPressionetb1.valueChangedCallback: {\
lettura_filtro_tipo (UxWidget);\
}

*selVarCrTipoEntalpiatb1.class: toggleButton
*selVarCrTipoEntalpiatb1.static: true
*selVarCrTipoEntalpiatb1.name: selVarCrTipoEntalpiatb1
*selVarCrTipoEntalpiatb1.parent: selVarCrTipoForm1
*selVarCrTipoEntalpiatb1.x: 100
*selVarCrTipoEntalpiatb1.y: 40
*selVarCrTipoEntalpiatb1.width: 90
*selVarCrTipoEntalpiatb1.height: 20
*selVarCrTipoEntalpiatb1.alignment: "alignment_beginning"
*selVarCrTipoEntalpiatb1.labelString: TIPO_ENTALPIA
*selVarCrTipoEntalpiatb1.valueChangedCallback: {\
lettura_filtro_tipo (UxWidget);\
}

*selVarCrTipoPortatatb1.class: toggleButton
*selVarCrTipoPortatatb1.static: true
*selVarCrTipoPortatatb1.name: selVarCrTipoPortatatb1
*selVarCrTipoPortatatb1.parent: selVarCrTipoForm1
*selVarCrTipoPortatatb1.x: 10
*selVarCrTipoPortatatb1.y: 40
*selVarCrTipoPortatatb1.width: 90
*selVarCrTipoPortatatb1.height: 20
*selVarCrTipoPortatatb1.alignment: "alignment_beginning"
*selVarCrTipoPortatatb1.labelString: TIPO_PORTATA
*selVarCrTipoPortatatb1.valueChangedCallback: {\
lettura_filtro_tipo (UxWidget);\
}

*selVarCrTipoGiritb1.class: toggleButton
*selVarCrTipoGiritb1.static: true
*selVarCrTipoGiritb1.name: selVarCrTipoGiritb1
*selVarCrTipoGiritb1.parent: selVarCrTipoForm1
*selVarCrTipoGiritb1.x: 100
*selVarCrTipoGiritb1.y: 20
*selVarCrTipoGiritb1.width: 90
*selVarCrTipoGiritb1.height: 20
*selVarCrTipoGiritb1.alignment: "alignment_beginning"
*selVarCrTipoGiritb1.labelString: TIPO_GIRI
*selVarCrTipoGiritb1.valueChangedCallback: {\
lettura_filtro_tipo (UxWidget);\
}

*selVarCrTipoTemperaturatb1.class: toggleButton
*selVarCrTipoTemperaturatb1.static: true
*selVarCrTipoTemperaturatb1.name: selVarCrTipoTemperaturatb1
*selVarCrTipoTemperaturatb1.parent: selVarCrTipoForm1
*selVarCrTipoTemperaturatb1.x: 10
*selVarCrTipoTemperaturatb1.y: 20
*selVarCrTipoTemperaturatb1.width: 90
*selVarCrTipoTemperaturatb1.height: 20
*selVarCrTipoTemperaturatb1.alignment: "alignment_beginning"
*selVarCrTipoTemperaturatb1.labelString: TIPO_TEMPERATURA
*selVarCrTipoTemperaturatb1.valueChangedCallback: {\
lettura_filtro_tipo (UxWidget);\
}

*selVarCrTipoAlzvalvtb1.class: toggleButton
*selVarCrTipoAlzvalvtb1.static: true
*selVarCrTipoAlzvalvtb1.name: selVarCrTipoAlzvalvtb1
*selVarCrTipoAlzvalvtb1.parent: selVarCrTipoForm1
*selVarCrTipoAlzvalvtb1.x: 200
*selVarCrTipoAlzvalvtb1.y: 0
*selVarCrTipoAlzvalvtb1.width: 90
*selVarCrTipoAlzvalvtb1.height: 20
*selVarCrTipoAlzvalvtb1.alignment: "alignment_beginning"
*selVarCrTipoAlzvalvtb1.labelString: TIPO_ALZVALV
*selVarCrTipoAlzvalvtb1.valueChangedCallback: {\
lettura_filtro_tipo (UxWidget);\
}

*selVarCrTipoPotenzatb1.class: toggleButton
*selVarCrTipoPotenzatb1.static: true
*selVarCrTipoPotenzatb1.name: selVarCrTipoPotenzatb1
*selVarCrTipoPotenzatb1.parent: selVarCrTipoForm1
*selVarCrTipoPotenzatb1.x: 100
*selVarCrTipoPotenzatb1.y: 0
*selVarCrTipoPotenzatb1.width: 90
*selVarCrTipoPotenzatb1.height: 20
*selVarCrTipoPotenzatb1.alignment: "alignment_beginning"
*selVarCrTipoPotenzatb1.labelString: TIPO_POTENZA
*selVarCrTipoPotenzatb1.valueChangedCallback: {\
lettura_filtro_tipo (UxWidget);\
}

*pushButton5.class: pushButton
*pushButton5.static: true
*pushButton5.name: pushButton5
*pushButton5.parent: selVarCrTipoForm1
*pushButton5.x: 420
*pushButton5.y: 40
*pushButton5.width: 120
*pushButton5.height: 25
*pushButton5.labelString: KKS_APPLY_FILTER
*pushButton5.activateCallback: {\
lettura_filtro_tipo (UxWidget); \
displayLista ();\
/*******\
if ((chi != MALFUNZIONI) && (chi != SOMMARIO_MALF) &&\
    (chi != FUNZIONI_REMOTE) && (chi != SOMMARIO_FREM))\
   {\
   display_var (selVarToSelect1, mod_sel, blocco_sel,\
            kks_filter_act,tipo_filter_act);\
   }\
else\
   {\
   switch (chi)\
      {\
      case MALFUNZIONI:\
      case SOMMARIO_MALF:\
         display_conf_malf (selVarToSelect1,m_component,\
                   malf_set, m_comp_type,mod_sel,blocco_sel,\
                   kks_filter_act,tipo_filter_act);\
   \
         break;\
      case FUNZIONI_REMOTE:\
      case SOMMARIO_FREM:\
         display_conf_malf (selVarToSelect1,fr_component,\
                   frem_set, fr_comp_type,mod_sel,blocco_sel,\
                   kks_filter_act,tipo_filter_act);\
   \
         break;\
      }\
   }\
*************/\
}

*selVarCrTipoAingable.class: toggleButton
*selVarCrTipoAingable.static: true
*selVarCrTipoAingable.name: selVarCrTipoAingable
*selVarCrTipoAingable.parent: selVarCrTipoForm1
*selVarCrTipoAingable.x: 290
*selVarCrTipoAingable.y: 0
*selVarCrTipoAingable.width: 120
*selVarCrTipoAingable.height: 20
*selVarCrTipoAingable.alignment: "alignment_beginning"
*selVarCrTipoAingable.labelString: TIPO_AINGABLE
*selVarCrTipoAingable.valueChangedCallback: {\
lettura_filtro_tipo (UxWidget);\
}

*selVarCrTipoInput.class: toggleButton
*selVarCrTipoInput.static: true
*selVarCrTipoInput.name: selVarCrTipoInput
*selVarCrTipoInput.parent: selVarCrTipoForm1
*selVarCrTipoInput.x: 290
*selVarCrTipoInput.y: 20
*selVarCrTipoInput.width: 120
*selVarCrTipoInput.height: 20
*selVarCrTipoInput.alignment: "alignment_beginning"
*selVarCrTipoInput.labelString: TIPO_INPUT
*selVarCrTipoInput.valueChangedCallback: {\
lettura_filtro_tipo (UxWidget);\
}

*selVarCrTipoOutput.class: toggleButton
*selVarCrTipoOutput.static: true
*selVarCrTipoOutput.name: selVarCrTipoOutput
*selVarCrTipoOutput.parent: selVarCrTipoForm1
*selVarCrTipoOutput.x: 290
*selVarCrTipoOutput.y: 40
*selVarCrTipoOutput.width: 120
*selVarCrTipoOutput.height: 20
*selVarCrTipoOutput.alignment: "alignment_beginning"
*selVarCrTipoOutput.labelString: TIPO_OUTPUT
*selVarCrTipoOutput.valueChangedCallback: {\
lettura_filtro_tipo (UxWidget);\
}

*form40.class: form
*form40.static: true
*form40.name: form40
*form40.parent: panedWindow2
*form40.resizePolicy: "resize_none"
*form40.x: 13
*form40.y: 13
*form40.width: 540
*form40.height: 150
*form40.paneMinimum: 140

*label10.class: label
*label10.static: true
*label10.name: label10
*label10.parent: form40
*label10.x: 0
*label10.y: 0
*label10.width: 534
*label10.height: 30
*label10.leftOffset: 0
*label10.rightAttachment: "attach_form"
*label10.leftAttachment: "attach_form"
*label10.labelString: ""

*form41.class: form
*form41.static: true
*form41.name: form41
*form41.parent: form40
*form41.resizePolicy: "resize_none"
*form41.x: 0
*form41.y: 300
*form41.width: 534
*form41.height: 30
*form41.bottomAttachment: "attach_form"
*form41.rightAttachment: "attach_form"
*form41.leftAttachment: "attach_form"

*selVarFindUp1.class: arrowButton
*selVarFindUp1.static: true
*selVarFindUp1.name: selVarFindUp1
*selVarFindUp1.parent: form41
*selVarFindUp1.x: 10
*selVarFindUp1.y: 0
*selVarFindUp1.width: 60
*selVarFindUp1.height: 30
*selVarFindUp1.arrowDirection: "arrow_up"
*selVarFindUp1.bottomAttachment: "attach_form"
*selVarFindUp1.leftOffset: 20
*selVarFindUp1.topAttachment: "attach_form"
*selVarFindUp1.bottomOffset: 3
*selVarFindUp1.topOffset: 3
*selVarFindUp1.activateCallbackClientData: (XtPointer) 0x0
*selVarFindUp1.activateCallback: {\
#ifndef DESIGN_TIME\
next_pos (selVarToSelect1,SEARCH_UP,selVarText1);\
#endif\
}

*selVarText1.class: textField
*selVarText1.static: true
*selVarText1.name: selVarText1
*selVarText1.parent: form41
*selVarText1.x: 20
*selVarText1.y: 10
*selVarText1.width: 140
*selVarText1.height: 20
*selVarText1.bottomAttachment: "attach_form"
*selVarText1.topAttachment: "attach_form"
*selVarText1.leftAttachment: "attach_widget"
*selVarText1.leftOffset: 5
*selVarText1.leftWidget: "selVarFindUp1"
*selVarText1.bottomOffset: 0
*selVarText1.topOffset: 0
*selVarText1.marginHeight: 2

*selVarFindDown1.class: arrowButton
*selVarFindDown1.static: true
*selVarFindDown1.name: selVarFindDown1
*selVarFindDown1.parent: form41
*selVarFindDown1.x: 250
*selVarFindDown1.y: 0
*selVarFindDown1.width: 60
*selVarFindDown1.height: 30
*selVarFindDown1.arrowDirection: "arrow_down"
*selVarFindDown1.leftAttachment: "attach_widget"
*selVarFindDown1.leftOffset: 5
*selVarFindDown1.leftWidget: "selVarText1"
*selVarFindDown1.bottomOffset: 3
*selVarFindDown1.bottomAttachment: "attach_form"
*selVarFindDown1.topAttachment: "attach_form"
*selVarFindDown1.topOffset: 3
*selVarFindDown1.activateCallback: {\
#ifndef DESIGN_TIME\
next_pos (selVarToSelect1,SEARCH_DOWN,selVarText1);\
#endif\
}

*scrolledWindow12.class: scrolledWindow
*scrolledWindow12.static: true
*scrolledWindow12.name: scrolledWindow12
*scrolledWindow12.parent: form40
*scrolledWindow12.scrollingPolicy: "application_defined"
*scrolledWindow12.x: 20
*scrolledWindow12.y: 30
*scrolledWindow12.visualPolicy: "variable"
*scrolledWindow12.scrollBarDisplayPolicy: "static"
*scrolledWindow12.shadowThickness: 0
*scrolledWindow12.bottomAttachment: "attach_widget"
*scrolledWindow12.bottomOffset: 5
*scrolledWindow12.bottomWidget: "form41"
*scrolledWindow12.rightAttachment: "attach_form"
*scrolledWindow12.rightOffset: 20
*scrolledWindow12.leftAttachment: "attach_form"
*scrolledWindow12.leftOffset: 20
*scrolledWindow12.topAttachment: "attach_widget"
*scrolledWindow12.topOffset: 5
*scrolledWindow12.topWidget: "label10"

*selVarToSelect1.class: scrolledList
*selVarToSelect1.static: true
*selVarToSelect1.name: selVarToSelect1
*selVarToSelect1.parent: scrolledWindow12
*selVarToSelect1.width: 490
*selVarToSelect1.height: 80
*selVarToSelect1.selectionPolicy: "single_select"
*selVarToSelect1.singleSelectionCallback: {\
int n; \
SCENARIO *pscen;\
char stringa[200];\
XmListCallbackStruct *reason = (XmListCallbackStruct *)UxCallbackArg;\
\
switch (chi)\
   {\
   case PREVIEW_CALLING:\
   case AING_CALLING:\
      strcpy (stringa, extract_string (reason->item));\
      strncpy (label_sel, stringa, MAX_LUN_NOME_VAR-1);\
      label_sel[MAX_LUN_NOME_VAR-1] = '\0';\
      XtVaSetValues (selOkpb, XmNsensitive, True, NULL);\
      XtVaSetValues (selVarMenuPopupSelect, XmNsensitive, True, NULL);\
      break;\
   case MALFUNZIONI:\
   case SOMMARIO_MALF:\
   case FUNZIONI_REMOTE:\
   case SOMMARIO_FREM:\
      conf_malf_select = reason->item_position;\
      printf ("lista: conf_malf_select = %d\n",conf_malf_select);\
      pscen = (SCENARIO *)p;\
      if (pscen->scen_header.numvar >= MAX_VAR_SCENARIO)\
         {\
         XtVaSetValues (selOkpb,XmNsensitive,False,NULL);\
         XtVaSetValues (selVarMenuPopupSelect,XmNsensitive,False,NULL);\
         }\
      else\
         {\
         XtVaSetValues (selOkpb, XmNsensitive, True, NULL);\
         XtVaSetValues (selVarMenuPopupSelect, XmNsensitive, True, NULL);\
         }\
      break;\
   }\
}

*menu13.class: rowColumn
*menu13.static: true
*menu13.name: menu13
*menu13.parent: selVar
*menu13.rowColumnType: "menu_popup"

*selVarMenuPopupSaveFilter.class: pushButton
*selVarMenuPopupSaveFilter.static: true
*selVarMenuPopupSaveFilter.name: selVarMenuPopupSaveFilter
*selVarMenuPopupSaveFilter.parent: menu13
*selVarMenuPopupSaveFilter.labelString: "Save Filter"
*selVarMenuPopupSaveFilter.activateCallback: {\
if (SD_optsave3 (BANCO, &options,IcProt) > 0)\
      fprintf (stderr,"OPZIONI salvate\n");\
else\
      fprintf (stderr,"*** errore save opzioni\n");\
}

*selVarMenuPopupApplyFilter.class: pushButton
*selVarMenuPopupApplyFilter.static: true
*selVarMenuPopupApplyFilter.name: selVarMenuPopupApplyFilter
*selVarMenuPopupApplyFilter.parent: menu13
*selVarMenuPopupApplyFilter.labelString: "Apply Filter"
*selVarMenuPopupApplyFilter.activateCallback: {\
if ((chi == MALFUNZIONI) || (chi == SOMMARIO_MALF))\
   applicaFiltri (selVar, FILTRO_MALF);\
else if ((chi == FUNZIONI_REMOTE) || (chi == SOMMARIO_FREM))\
   applicaFiltri (selVar, FILTRO_FREM);\
else\
   applicaFiltri (selVar, FILTRO_VAR2);\
}

*selVarMenuPopup_b3.class: separator
*selVarMenuPopup_b3.static: true
*selVarMenuPopup_b3.name: selVarMenuPopup_b3
*selVarMenuPopup_b3.parent: menu13

*selVarMenuPopupSelect.class: pushButton
*selVarMenuPopupSelect.static: true
*selVarMenuPopupSelect.name: selVarMenuPopupSelect
*selVarMenuPopupSelect.parent: menu13
*selVarMenuPopupSelect.labelString: SELVAR_CONFIRM ? SELVAR_CONFIRM : "OK"
*selVarMenuPopupSelect.activateCallback: {\
selectSelVarOk (UxWidget);\
}
*selVarMenuPopupSelect.sensitive: "false"

*selVarMenuPopup_b5.class: separator
*selVarMenuPopup_b5.static: true
*selVarMenuPopup_b5.name: selVarMenuPopup_b5
*selVarMenuPopup_b5.parent: menu13

*selVarMenuPopupQuit.class: pushButton
*selVarMenuPopupQuit.static: true
*selVarMenuPopupQuit.name: selVarMenuPopupQuit
*selVarMenuPopupQuit.parent: menu13
*selVarMenuPopupQuit.labelString: CONF_DISCARD
*selVarMenuPopupQuit.activateCallback: {\
/******** in destroy callback di selVar\
#ifndef DESIGN_TIME\
_UxCpreviewSnap *CtxPsnap;\
_UxCmasterMenu *CtxMaster;\
if (chi == PREVIEW_CALLING)\
   {\
   CtxPsnap = (_UxCpreviewSnap *)UxGetContext(w);\
   CtxPsnap->UxselVarWidg = (Widget)NULL;\
   }\
else if (chi == AING_CALLING)\
   {\
   CtxMaster = (_UxCmasterMenu *)UxGetContext(w);\
   CtxMaster->UxselVarAing = (Widget)NULL;\
   }\
#endif\
*************/\
DistruggiInterfaccia (XtParent(selVar));\
}

