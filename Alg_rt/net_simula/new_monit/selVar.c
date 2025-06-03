
/*******************************************************************************
       selVar.c
       (Generated from interface file selVar.i)
*******************************************************************************/

#include <stdio.h>

#ifdef MOTIF
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#endif /* MOTIF */

#include "UxXt.h"
#include <Xm/Text.h>
#include <Xm/BulletinB.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/TextF.h>
#include <Xm/ArrowB.h>
#include <Xm/Label.h>
#include <Xm/PanedW.h>
#include <Xm/Separator.h>
#include <Xm/ToggleB.h>
#include <Xm/CascadeB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
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

#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"


#include "cursore.h"
#include "malfunzioni.h"

extern M_COMP_TYPE *m_comp_type;
extern MALF_SET *malf_set;
extern M_COMPONENT *m_component;

extern M_COMP_TYPE *fr_comp_type;
extern MALF_SET *frem_set;
extern M_COMPONENT *fr_component;

#ifndef DESIGN_TIME
/* #include "interf_attive.h" */
#include "preview.h"
#include "masterMenu.h"
#include "previewSnap.h"
#endif
#include "cont_rec.h"
#ifndef MAX_LUN_NOME_VAR
#define MAX_LUN_NOME_VAR 9
#endif
char label_sel[MAX_LUN_NOME_VAR];
extern OPTIONS_FLAGS options;
extern Boolean *IcProt;


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
	Widget	UxselVar;
	Widget	Uxframe2;
	Widget	Uxform42;
	Widget	UxselOkpb;
	Widget	UxselQuitpb;
	Widget	Uxmenu4;
	Widget	UxselVarCrMenu1;
	Widget	UxselVarCrMenuFilepb1;
	Widget	Uxmenu3_top_b3;
	Widget	UxselVarMenuFilter1;
	Widget	UxselVarMenuOtherPane1;
	Widget	UxselByAllVariablestb1;
	Widget	UxselByModeltb1;
	Widget	UxselByModelOnlytb1;
	Widget	UxselVarMenuOtherPane_b1;
	Widget	UxselVarMenuKkstb1;
	Widget	UxselVarMenuOtherPane_b2;
	Widget	UxselVarMenuTipotb1;
	Widget	UxselVarMenuFilter_b1;
	Widget	Uxmenu3_top_b4;
	Widget	Uxframe27;
	Widget	UxselVarCrFilterForm1;
	Widget	Uxframe28;
	Widget	Uxform51;
	Widget	UxselVarCrAllVartb1;
	Widget	UxselVarCrModBlocktb1;
	Widget	UxselVarCrModtb1;
	Widget	Uxframe29;
	Widget	Uxform52;
	Widget	UxselVarCrKkstb1;
	Widget	UxselVarCrTypetb1;
	Widget	UxselVarCrSaveFilterpb1;
	Widget	UxselVarCrApplyFilterpb1;
	Widget	UxpanedWindow2;
	Widget	UxselByModelForm1;
	Widget	Uxlabel8;
	Widget	Uxlabel9;
	Widget	Uxform43;
	Widget	UxselVarModFindDown;
	Widget	UxselVarText2;
	Widget	UxselVarModFindUp;
	Widget	UxscrolledWindow10;
	Widget	UxselVarListaSistemi1;
	Widget	UxscrolledWindow11;
	Widget	UxselVarListaBlocchi1;
	Widget	UxselVarCrKksForm2;
	Widget	UxselVarCrKksBullBoard2;
	Widget	UxselVarCrKksText22;
	Widget	UxselVarCrKksSistemaLabel2;
	Widget	UxselVarCrKksSottosistLabel2;
	Widget	UxselVarCrKksComponentLabel2;
	Widget	UxselVarCrKksText23;
	Widget	UxselVarCrKksText24;
	Widget	UxselVarCrKksText25;
	Widget	UxselVarCrKksText26;
	Widget	UxselVarCrKksText27;
	Widget	UxselVarCrKksText28;
	Widget	UxselVarCrKksText29;
	Widget	UxselVarCrKksProgressLabel2;
	Widget	UxselVarCrKksText30;
	Widget	UxselVarCrKksText31;
	Widget	UxselVarCrKksText32;
	Widget	UxselVarCrKksNimpLabel2;
	Widget	UxpushButton4;
	Widget	UxselVarCrTipoForm1;
	Widget	UxselVarCrTipoPressionetb1;
	Widget	UxselVarCrTipoEntalpiatb1;
	Widget	UxselVarCrTipoPortatatb1;
	Widget	UxselVarCrTipoGiritb1;
	Widget	UxselVarCrTipoTemperaturatb1;
	Widget	UxselVarCrTipoAlzvalvtb1;
	Widget	UxselVarCrTipoPotenzatb1;
	Widget	UxpushButton5;
	Widget	UxselVarCrTipoAingable;
	Widget	UxselVarCrTipoInput;
	Widget	UxselVarCrTipoOutput;
	Widget	Uxform40;
	Widget	Uxlabel10;
	Widget	Uxform41;
	Widget	UxselVarFindUp1;
	Widget	UxselVarText1;
	Widget	UxselVarFindDown1;
	Widget	UxscrolledWindow12;
	Widget	UxselVarToSelect1;
	Widget	Uxmenu13;
	Widget	UxselVarMenuPopupSaveFilter;
	Widget	UxselVarMenuPopupApplyFilter;
	Widget	UxselVarMenuPopup_b3;
	Widget	UxselVarMenuPopupSelect;
	Widget	UxselVarMenuPopup_b5;
	Widget	UxselVarMenuPopupQuit;
	int	Uxmod_sel;
	int	Uxblocco_sel;
	int	Uxkks_filter_act;
	int	Uxtipo_filter_act;
	char	Uxstringa_kks[12];
	FILTRO_TIPI	Uxfiltro_tipo;
	int	*Uxpunt_var;
	int	*Uxpunt_var_kks;
	int	*Uxpunt_var_tipo;
	int	*Uxpunt_var_lista;
	int	Uxn_varlist;
	int	Uxn_varlist_kks;
	int	Uxn_varlist_tipo;
	int	Uxn_varlist_lista;
	int	Uxn_blocchi;
	int	Uxfirst_time;
	int	Uxsel_mod;
	Boolean	*Uxmatch;
	int	Uxlast_match;
	int	Uxnmatch;
	int	Uxfirst_time_find;
	char	*Uxstringa_match;
	char	*Uxold_stringa_match;
	int	Uxfirst_time_b;
	int	Uxsel_mod_b;
	Boolean	*Uxmatch_b;
	int	Uxlast_match_b;
	int	Uxnmatch_b;
	int	Uxfirst_time_find_b;
	char	*Uxstringa_match_b;
	char	*Uxold_stringa_match_b;
	int	*Uxpunt_conf_malf;
	int	Uxn_conf_malf;
	int	*Uxpunt_malf_kks;
	int	Uxn_conf_malf_kks;
	int	*Uxpunt_malf_tipo;
	int	Uxn_conf_malf_tipo;
	int	*Uxpunt_malf_lista;
	int	Uxn_conf_malf_lista;
	int	Uxfiltro_kks_malf_conf;
	char	Uxfilter_kks_malf_conf[12];
	int	Uxfiltro_tipo_malf_conf;
	FILTRO_TIPI	Uxfiltro_tipi_malf_conf;
	int	Uxconf_malf_select;
	int	Uxprimo_giro;
	Widget	Uxw;
	char	*Uxp;
	int	Uxchi;
	char	*Uxheader;
} _UxCselVar;

static _UxCselVar              *UxSelVarContext;
#define selVar                  UxSelVarContext->UxselVar
#define frame2                  UxSelVarContext->Uxframe2
#define form42                  UxSelVarContext->Uxform42
#define selOkpb                 UxSelVarContext->UxselOkpb
#define selQuitpb               UxSelVarContext->UxselQuitpb
#define menu4                   UxSelVarContext->Uxmenu4
#define selVarCrMenu1           UxSelVarContext->UxselVarCrMenu1
#define selVarCrMenuFilepb1     UxSelVarContext->UxselVarCrMenuFilepb1
#define menu3_top_b3            UxSelVarContext->Uxmenu3_top_b3
#define selVarMenuFilter1       UxSelVarContext->UxselVarMenuFilter1
#define selVarMenuOtherPane1    UxSelVarContext->UxselVarMenuOtherPane1
#define selByAllVariablestb1    UxSelVarContext->UxselByAllVariablestb1
#define selByModeltb1           UxSelVarContext->UxselByModeltb1
#define selByModelOnlytb1       UxSelVarContext->UxselByModelOnlytb1
#define selVarMenuOtherPane_b1  UxSelVarContext->UxselVarMenuOtherPane_b1
#define selVarMenuKkstb1        UxSelVarContext->UxselVarMenuKkstb1
#define selVarMenuOtherPane_b2  UxSelVarContext->UxselVarMenuOtherPane_b2
#define selVarMenuTipotb1       UxSelVarContext->UxselVarMenuTipotb1
#define selVarMenuFilter_b1     UxSelVarContext->UxselVarMenuFilter_b1
#define menu3_top_b4            UxSelVarContext->Uxmenu3_top_b4
#define frame27                 UxSelVarContext->Uxframe27
#define selVarCrFilterForm1     UxSelVarContext->UxselVarCrFilterForm1
#define frame28                 UxSelVarContext->Uxframe28
#define form51                  UxSelVarContext->Uxform51
#define selVarCrAllVartb1       UxSelVarContext->UxselVarCrAllVartb1
#define selVarCrModBlocktb1     UxSelVarContext->UxselVarCrModBlocktb1
#define selVarCrModtb1          UxSelVarContext->UxselVarCrModtb1
#define frame29                 UxSelVarContext->Uxframe29
#define form52                  UxSelVarContext->Uxform52
#define selVarCrKkstb1          UxSelVarContext->UxselVarCrKkstb1
#define selVarCrTypetb1         UxSelVarContext->UxselVarCrTypetb1
#define selVarCrSaveFilterpb1   UxSelVarContext->UxselVarCrSaveFilterpb1
#define selVarCrApplyFilterpb1  UxSelVarContext->UxselVarCrApplyFilterpb1
#define panedWindow2            UxSelVarContext->UxpanedWindow2
#define selByModelForm1         UxSelVarContext->UxselByModelForm1
#define label8                  UxSelVarContext->Uxlabel8
#define label9                  UxSelVarContext->Uxlabel9
#define form43                  UxSelVarContext->Uxform43
#define selVarModFindDown       UxSelVarContext->UxselVarModFindDown
#define selVarText2             UxSelVarContext->UxselVarText2
#define selVarModFindUp         UxSelVarContext->UxselVarModFindUp
#define scrolledWindow10        UxSelVarContext->UxscrolledWindow10
#define selVarListaSistemi1     UxSelVarContext->UxselVarListaSistemi1
#define scrolledWindow11        UxSelVarContext->UxscrolledWindow11
#define selVarListaBlocchi1     UxSelVarContext->UxselVarListaBlocchi1
#define selVarCrKksForm2        UxSelVarContext->UxselVarCrKksForm2
#define selVarCrKksBullBoard2   UxSelVarContext->UxselVarCrKksBullBoard2
#define selVarCrKksText22       UxSelVarContext->UxselVarCrKksText22
#define selVarCrKksSistemaLabel2 UxSelVarContext->UxselVarCrKksSistemaLabel2
#define selVarCrKksSottosistLabel2 UxSelVarContext->UxselVarCrKksSottosistLabel2
#define selVarCrKksComponentLabel2 UxSelVarContext->UxselVarCrKksComponentLabel2
#define selVarCrKksText23       UxSelVarContext->UxselVarCrKksText23
#define selVarCrKksText24       UxSelVarContext->UxselVarCrKksText24
#define selVarCrKksText25       UxSelVarContext->UxselVarCrKksText25
#define selVarCrKksText26       UxSelVarContext->UxselVarCrKksText26
#define selVarCrKksText27       UxSelVarContext->UxselVarCrKksText27
#define selVarCrKksText28       UxSelVarContext->UxselVarCrKksText28
#define selVarCrKksText29       UxSelVarContext->UxselVarCrKksText29
#define selVarCrKksProgressLabel2 UxSelVarContext->UxselVarCrKksProgressLabel2
#define selVarCrKksText30       UxSelVarContext->UxselVarCrKksText30
#define selVarCrKksText31       UxSelVarContext->UxselVarCrKksText31
#define selVarCrKksText32       UxSelVarContext->UxselVarCrKksText32
#define selVarCrKksNimpLabel2   UxSelVarContext->UxselVarCrKksNimpLabel2
#define pushButton4             UxSelVarContext->UxpushButton4
#define selVarCrTipoForm1       UxSelVarContext->UxselVarCrTipoForm1
#define selVarCrTipoPressionetb1 UxSelVarContext->UxselVarCrTipoPressionetb1
#define selVarCrTipoEntalpiatb1 UxSelVarContext->UxselVarCrTipoEntalpiatb1
#define selVarCrTipoPortatatb1  UxSelVarContext->UxselVarCrTipoPortatatb1
#define selVarCrTipoGiritb1     UxSelVarContext->UxselVarCrTipoGiritb1
#define selVarCrTipoTemperaturatb1 UxSelVarContext->UxselVarCrTipoTemperaturatb1
#define selVarCrTipoAlzvalvtb1  UxSelVarContext->UxselVarCrTipoAlzvalvtb1
#define selVarCrTipoPotenzatb1  UxSelVarContext->UxselVarCrTipoPotenzatb1
#define pushButton5             UxSelVarContext->UxpushButton5
#define selVarCrTipoAingable    UxSelVarContext->UxselVarCrTipoAingable
#define selVarCrTipoInput       UxSelVarContext->UxselVarCrTipoInput
#define selVarCrTipoOutput      UxSelVarContext->UxselVarCrTipoOutput
#define form40                  UxSelVarContext->Uxform40
#define label10                 UxSelVarContext->Uxlabel10
#define form41                  UxSelVarContext->Uxform41
#define selVarFindUp1           UxSelVarContext->UxselVarFindUp1
#define selVarText1             UxSelVarContext->UxselVarText1
#define selVarFindDown1         UxSelVarContext->UxselVarFindDown1
#define scrolledWindow12        UxSelVarContext->UxscrolledWindow12
#define selVarToSelect1         UxSelVarContext->UxselVarToSelect1
#define menu13                  UxSelVarContext->Uxmenu13
#define selVarMenuPopupSaveFilter UxSelVarContext->UxselVarMenuPopupSaveFilter
#define selVarMenuPopupApplyFilter UxSelVarContext->UxselVarMenuPopupApplyFilter
#define selVarMenuPopup_b3      UxSelVarContext->UxselVarMenuPopup_b3
#define selVarMenuPopupSelect   UxSelVarContext->UxselVarMenuPopupSelect
#define selVarMenuPopup_b5      UxSelVarContext->UxselVarMenuPopup_b5
#define selVarMenuPopupQuit     UxSelVarContext->UxselVarMenuPopupQuit
#define mod_sel                 UxSelVarContext->Uxmod_sel
#define blocco_sel              UxSelVarContext->Uxblocco_sel
#define kks_filter_act          UxSelVarContext->Uxkks_filter_act
#define tipo_filter_act         UxSelVarContext->Uxtipo_filter_act
#define stringa_kks             UxSelVarContext->Uxstringa_kks
#define filtro_tipo             UxSelVarContext->Uxfiltro_tipo
#define punt_var                UxSelVarContext->Uxpunt_var
#define punt_var_kks            UxSelVarContext->Uxpunt_var_kks
#define punt_var_tipo           UxSelVarContext->Uxpunt_var_tipo
#define punt_var_lista          UxSelVarContext->Uxpunt_var_lista
#define n_varlist               UxSelVarContext->Uxn_varlist
#define n_varlist_kks           UxSelVarContext->Uxn_varlist_kks
#define n_varlist_tipo          UxSelVarContext->Uxn_varlist_tipo
#define n_varlist_lista         UxSelVarContext->Uxn_varlist_lista
#define n_blocchi               UxSelVarContext->Uxn_blocchi
#define first_time              UxSelVarContext->Uxfirst_time
#define sel_mod                 UxSelVarContext->Uxsel_mod
#define match                   UxSelVarContext->Uxmatch
#define last_match              UxSelVarContext->Uxlast_match
#define nmatch                  UxSelVarContext->Uxnmatch
#define first_time_find         UxSelVarContext->Uxfirst_time_find
#define stringa_match           UxSelVarContext->Uxstringa_match
#define old_stringa_match       UxSelVarContext->Uxold_stringa_match
#define first_time_b            UxSelVarContext->Uxfirst_time_b
#define sel_mod_b               UxSelVarContext->Uxsel_mod_b
#define match_b                 UxSelVarContext->Uxmatch_b
#define last_match_b            UxSelVarContext->Uxlast_match_b
#define nmatch_b                UxSelVarContext->Uxnmatch_b
#define first_time_find_b       UxSelVarContext->Uxfirst_time_find_b
#define stringa_match_b         UxSelVarContext->Uxstringa_match_b
#define old_stringa_match_b     UxSelVarContext->Uxold_stringa_match_b
#define punt_conf_malf          UxSelVarContext->Uxpunt_conf_malf
#define n_conf_malf             UxSelVarContext->Uxn_conf_malf
#define punt_malf_kks           UxSelVarContext->Uxpunt_malf_kks
#define n_conf_malf_kks         UxSelVarContext->Uxn_conf_malf_kks
#define punt_malf_tipo          UxSelVarContext->Uxpunt_malf_tipo
#define n_conf_malf_tipo        UxSelVarContext->Uxn_conf_malf_tipo
#define punt_malf_lista         UxSelVarContext->Uxpunt_malf_lista
#define n_conf_malf_lista       UxSelVarContext->Uxn_conf_malf_lista
#define filtro_kks_malf_conf    UxSelVarContext->Uxfiltro_kks_malf_conf
#define filter_kks_malf_conf    UxSelVarContext->Uxfilter_kks_malf_conf
#define filtro_tipo_malf_conf   UxSelVarContext->Uxfiltro_tipo_malf_conf
#define filtro_tipi_malf_conf   UxSelVarContext->Uxfiltro_tipi_malf_conf
#define conf_malf_select        UxSelVarContext->Uxconf_malf_select
#define primo_giro              UxSelVarContext->Uxprimo_giro
#define w                       UxSelVarContext->Uxw
#define p                       UxSelVarContext->Uxp
#define chi                     UxSelVarContext->Uxchi
#define header                  UxSelVarContext->Uxheader


int default_aing (Widget);
int lettura_filtro_tipo (Widget);
int get_byte_kks (Widget,char*);
int add_preview_entry (Widget, PREVIEW_DATA *, int,int);
int attiva_aing (Widget,char*);
int add_item_scenario_malf (Widget, Widget, int, SCENARIO *, int);
int display_var (Widget, int, int, int, int);
int display_conf_malf (Widget, M_COMPONENT *, MALF_SET *, M_COMP_TYPE *, int, int, int, int);
int manage_widget_filter (Widget, Widget, Boolean, int,int);
int next_pos_bloc (Widget, int, Widget);
char *extract_string(XmString);
int readConfMalf (Widget , int );







/*******************************************************************************
       The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxselVarMenuPost( wgt, client_data, event, ctd )
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


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_selVar();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

int presetFilterDefaults1 (ppp)
Widget ppp;
{
if ((chi != MALFUNZIONI) && (chi != SOMMARIO_MALF) &&      /* aing o preview  */
    (chi != FUNZIONI_REMOTE) && (chi != SOMMARIO_FREM))
   {
	XmToggleButtonSetState (UxGetWidget(selByAllVariablestb1),options.optionsFilterAing.ByAllVar,False);
	XmToggleButtonSetState (UxGetWidget(selByModeltb1),options.optionsFilterAing.ByModBlock,False);
	XmToggleButtonSetState (UxGetWidget(selByModelOnlytb1),options.optionsFilterAing.ByMod,False);
	XmToggleButtonSetState (UxGetWidget(selVarMenuKkstb1),options.optionsFilterAing.ByKks,False);
	XmToggleButtonSetState (UxGetWidget(selVarMenuTipotb1),options.optionsFilterAing.ByType,False);

	XmToggleButtonSetState (UxGetWidget(selVarCrAllVartb1),options.optionsFilterAing.ByAllVar,False);
	XmToggleButtonSetState (UxGetWidget(selVarCrModBlocktb1),options.optionsFilterAing.ByModBlock,False);
	XmToggleButtonSetState (UxGetWidget(selVarCrModtb1),options.optionsFilterAing.ByMod,False);
	XmToggleButtonSetState (UxGetWidget(selVarCrKkstb1),options.optionsFilterAing.ByKks,False);
	XmToggleButtonSetState (UxGetWidget(selVarCrTypetb1),options.optionsFilterAing.ByType,False);
   }
else       /* malfunzioni o funzioni remote */
   {
   if ((chi == MALFUNZIONI) || (chi == SOMMARIO_MALF))
      {
      XmToggleButtonSetState (UxGetWidget(selVarCrAllVartb1),options.optionsFilterMalf.ByAllVar,False);
      XmToggleButtonSetState (UxGetWidget(selVarCrModBlocktb1),options.optionsFilterMalf.ByModBlock,False);
      XmToggleButtonSetState (UxGetWidget(selVarCrModtb1),options.optionsFilterMalf.ByMod,False);
      XmToggleButtonSetState (UxGetWidget(selVarCrKkstb1),options.optionsFilterMalf.ByKks,False);
      XmToggleButtonSetState (UxGetWidget(selVarCrTypetb1),options.optionsFilterMalf.ByType,False);
      }
   if ((chi == FUNZIONI_REMOTE) || (chi == SOMMARIO_FREM))
      {
      XmToggleButtonSetState (UxGetWidget(selVarCrAllVartb1),options.optionsFilterFrem.ByAllVar,False);
      XmToggleButtonSetState (UxGetWidget(selVarCrModBlocktb1),options.optionsFilterFrem.ByModBlock,False);
      XmToggleButtonSetState (UxGetWidget(selVarCrModtb1),options.optionsFilterFrem.ByMod,False);
      XmToggleButtonSetState (UxGetWidget(selVarCrKkstb1),options.optionsFilterFrem.ByKks,False);
      XmToggleButtonSetState (UxGetWidget(selVarCrTypetb1),options.optionsFilterFrem.ByType,False);
      }

   }

	return (0);
}
/**********************************************************/
int set_var_default (oggetto)
Widget oggetto;
{
#ifndef DESIGN_TIME
Boolean stato;

_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(oggetto);

   if (Ctx->Uxchi == AING_CALLING)   /* blocca il filtro per tipo*/
      default_aing (oggetto);
      
/* legge lo stato del toggle button di filtro */
   stato = XmToggleButtonGetState (Ctx->UxselVarCrAllVartb1); 
   switch (stato)
      {
      case 0:
         Ctx->Uxmod_sel = 1;
         Ctx->Uxblocco_sel = 1;
         XtManageChild (Ctx->UxselByModelForm1);
         break;
      case 1:
         Ctx->Uxmod_sel = NO_MOD;
         Ctx->Uxblocco_sel = NO_BLOCK;
         XtUnmanageChild (Ctx->UxselByModelForm1);
         break;
      }
/* legge lo stato del toggle button di filtro */
   stato = XmToggleButtonGetState (Ctx->UxselVarCrModBlocktb1); 
   switch (stato)
      {
      case 1:
         Ctx->Uxmod_sel = 1;
         Ctx->Uxblocco_sel = 1;
         XtManageChild (Ctx->UxselByModelForm1);
         break;
      case 0:
         Ctx->Uxmod_sel = NO_MOD;
         Ctx->Uxblocco_sel = NO_BLOCK;
         XtUnmanageChild (Ctx->UxselByModelForm1);
         break;
      }
/* legge lo stato del toggle button del filtro kks */
    stato = XmToggleButtonGetState (Ctx->UxselVarCrKkstb1);  
   switch (stato)
      {
      case 1:
         Ctx->Uxkks_filter_act = 1;
         XtManageChild (Ctx->UxselVarCrKksForm2);
         break;
      case 0:
         Ctx->Uxkks_filter_act = 0;
         XtUnmanageChild (Ctx->UxselVarCrKksForm2);
         break;
      }
/* legge lo stato del toggle button del filtro per tipo */
    stato = XmToggleButtonGetState (Ctx->UxselVarCrTypetb1);  
   switch (stato)
      {
      case 1:
         Ctx->Uxtipo_filter_act = 1;
         lettura_filtro_tipo (Ctx->UxselVarCrTipoForm1);
         XtManageChild (Ctx->UxselVarCrTipoForm1);
         break;
      case 0:
         Ctx->Uxtipo_filter_act = 0;
         XtUnmanageChild (Ctx->UxselVarCrTipoForm1);
         break;
      }
#endif
   return (0);
}
/*************************************************/
int lettura_filtro_tipo (oggetto)
Widget oggetto;
{
#ifndef DESIGN_TIME
_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(oggetto);

   Ctx->Uxfiltro_tipo.pressione = XmToggleButtonGetState (Ctx->UxselVarCrTipoPressionetb1);
   Ctx->Uxfiltro_tipo.temperatura = XmToggleButtonGetState (Ctx->UxselVarCrTipoTemperaturatb1);
   Ctx->Uxfiltro_tipo.entalpia = XmToggleButtonGetState (Ctx->UxselVarCrTipoEntalpiatb1);
   Ctx->Uxfiltro_tipo.portata = XmToggleButtonGetState (Ctx->UxselVarCrTipoPortatatb1);
   Ctx->Uxfiltro_tipo.potenza = XmToggleButtonGetState (Ctx->UxselVarCrTipoPotenzatb1);
   Ctx->Uxfiltro_tipo.giri = XmToggleButtonGetState (Ctx->UxselVarCrTipoGiritb1);
   Ctx->Uxfiltro_tipo.alzvalv = XmToggleButtonGetState (Ctx->UxselVarCrTipoAlzvalvtb1);
   Ctx->Uxfiltro_tipo.ingressi = XmToggleButtonGetState (Ctx->UxselVarCrTipoInput);
   Ctx->Uxfiltro_tipo.uscite = XmToggleButtonGetState (Ctx->UxselVarCrTipoOutput);
   Ctx->Uxfiltro_tipo.aingabili = XmToggleButtonGetState (Ctx->UxselVarCrTipoAingable);

   return (0);
#endif
}
/*************************************************/
int default_aing (oggetto)
Widget oggetto;
{
#ifndef DESIGN_TIME
_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(oggetto);

   XmToggleButtonSetState (Ctx->UxselVarMenuTipotb1,True,False);
   XtVaSetValues (Ctx->UxselVarMenuTipotb1,XmNsensitive,False,NULL);

   XmToggleButtonSetState (Ctx->UxselVarCrTipoAingable,True,False);
   XtVaSetValues (Ctx->UxselVarCrTipoAingable,XmNsensitive,False,NULL);

   XmToggleButtonSetState (Ctx->UxselVarCrTypetb1,True,False);
   XtVaSetValues (Ctx->UxselVarCrTypetb1,XmNsensitive,False,NULL);

   XtUnmanageChild (Ctx->UxselVarCrTipoOutput);
   XtUnmanageChild (Ctx->UxselVarCrTipoInput);
#endif
return(0);
}
/*************************************************/
int getKksFilterMalfConf (oggetto, f)
Widget oggetto;
char *f;
{
_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(oggetto);
#ifndef DESIGN_TIME
   printf ("lettura filtro kks\n");
   get_byte_kks (Ctx->UxselVarCrKksText32, &f[0]);
   get_byte_kks (Ctx->UxselVarCrKksText22,&f[1]);
   get_byte_kks (Ctx->UxselVarCrKksText23,&f[2]);
   get_byte_kks (Ctx->UxselVarCrKksText24,&f[3]);
   get_byte_kks (Ctx->UxselVarCrKksText25,&f[4]);
   get_byte_kks (Ctx->UxselVarCrKksText26,&f[5]);
   get_byte_kks (Ctx->UxselVarCrKksText27,&f[6]);
   get_byte_kks (Ctx->UxselVarCrKksText28,&f[7]);
   get_byte_kks (Ctx->UxselVarCrKksText29,&f[8]);
   get_byte_kks (Ctx->UxselVarCrKksText30,&f[9]);
   get_byte_kks (Ctx->UxselVarCrKksText31,&f[10]);
   f[11] = '\0';
   printf ("kks_filtro = :%s:\n",f);
#endif
   return (0);
}
/*************************************************/
int lettura_filtro_kks (oggetto)
Widget oggetto;
{
_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(oggetto);
#ifndef DESIGN_TIME
   printf ("lettura filtro kks\n");
   get_byte_kks (Ctx->UxselVarCrKksText32, &Ctx->Uxstringa_kks[0]);
   get_byte_kks (Ctx->UxselVarCrKksText22,&Ctx->Uxstringa_kks[1]);
   get_byte_kks (Ctx->UxselVarCrKksText23,&Ctx->Uxstringa_kks[2]);
   get_byte_kks (Ctx->UxselVarCrKksText24,&Ctx->Uxstringa_kks[3]);
   get_byte_kks (Ctx->UxselVarCrKksText25,&Ctx->Uxstringa_kks[4]);
   get_byte_kks (Ctx->UxselVarCrKksText26,&Ctx->Uxstringa_kks[5]);
   get_byte_kks (Ctx->UxselVarCrKksText27,&Ctx->Uxstringa_kks[6]);
   get_byte_kks (Ctx->UxselVarCrKksText28,&Ctx->Uxstringa_kks[7]);
   get_byte_kks (Ctx->UxselVarCrKksText29,&Ctx->Uxstringa_kks[8]);
   get_byte_kks (Ctx->UxselVarCrKksText30,&Ctx->Uxstringa_kks[9]);
   get_byte_kks (Ctx->UxselVarCrKksText31,&Ctx->Uxstringa_kks[10]);
   Ctx->Uxstringa_kks[11] = '\0';
   printf ("kks_filtro = :%s:\n",Ctx->Uxstringa_kks);
#endif
   return (0);
}
/*************************************************/
int setConfMalfDefaults  (wgt)
Widget wgt;
{
   filtro_kks_malf_conf = 1;
   filtro_tipo_malf_conf = 0;
}
/*************************************************/
int selectSelVarOk (Widget wgt)
{
_UxCselVar *Ctx = (_UxCselVar *)UxGetContext(wgt);
int i;
#ifndef DESIGN_TIME
if (chi == PREVIEW_CALLING)
   {
   if (strcmp(label_sel,""))
      {
      i = get_label_indx (label_sel);
printf ("selvarok: :%s: ind %d\n",label_sel,i);
      add_preview_entry (w, (PREVIEW_DATA *)p, NON_NOTA, i);
      get_val_view (w);
      display_view (w);
      }
   }
else if (chi == AING_CALLING)
   {
   printf ("Chiamata da AING label_sel=|%s|\n", label_sel);
   if (strcmp(label_sel,""))
      {
      attiva_aing (wgt, label_sel);
      }
   }
else   /* malf o frem */
   {
   if (add_item_scenario_malf (w,selVar,
                 conf_malf_select, (SCENARIO *)p, chi) > MAX_VAR_SCENARIO)
      {  
      XtVaSetValues (selOkpb,XmNsensitive,False,NULL);
      XtVaSetValues (selVarMenuPopupSelect,XmNsensitive,False,NULL);
      }
   }
#endif
}
/****************************************************/
int displayLista ()
{

set_cursor (selVar, CLOCK);

if ((chi != MALFUNZIONI) && (chi != SOMMARIO_MALF) &&
    (chi != FUNZIONI_REMOTE) && (chi != SOMMARIO_FREM))
   {
   display_var (selVarToSelect1, mod_sel, blocco_sel,
            kks_filter_act,tipo_filter_act);
   }
else
   {
   switch (chi)
      {
      case MALFUNZIONI:
      case SOMMARIO_MALF:
         display_conf_malf (selVarToSelect1,m_component,
                   malf_set, m_comp_type,mod_sel,blocco_sel,
                   kks_filter_act,tipo_filter_act);
  
         break;
      case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         display_conf_malf (selVarToSelect1,fr_component,
                   frem_set, fr_comp_type,mod_sel,blocco_sel,
                   kks_filter_act,tipo_filter_act);
   
         break;
      }
   }
set_cursor (selVar, NORMALE);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void  destroyCB_selVar( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	_UxCpreviewSnap *CtxPsnap;
	_UxCmasterMenu *CtxMaster;
	if (chi == PREVIEW_CALLING)
	   {
	   CtxPsnap = (_UxCpreviewSnap *)UxGetContext(w);
	   CtxPsnap->UxselVarWidg = (Widget)NULL;
	   }
	else if (chi == AING_CALLING)
	   {
	   CtxMaster = (_UxCmasterMenu *)UxGetContext(w);
	   CtxMaster->UxselVarAing = (Widget)NULL;
	   }
#endif
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_selOkpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	selectSelVarOk (UxWidget);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_selQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	/********** in destroy callback di selVar
#ifndef DESIGN_TIME
	_UxCpreviewSnap *CtxPsnap;
	_UxCmasterMenu *CtxMaster;
	if (chi == PREVIEW_CALLING)
	   {
	   CtxPsnap = (_UxCpreviewSnap *)UxGetContext(w);
	   CtxPsnap->UxselVarWidg = (Widget)NULL;
	   }
	else if (chi == AING_CALLING)
	   {
	   CtxMaster = (_UxCmasterMenu *)UxGetContext(w);
	   CtxMaster->UxselVarAing = (Widget)NULL;
	   }
#endif
	************/
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_selVarCrMenuFilepb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	/*** in destroy callback di selVar
#ifndef DESIGN_TIME
	_UxCpreviewSnap *CtxPsnap;
	_UxCmasterMenu *CtxMaster;
	if (chi == PREVIEW_CALLING)
	   {
	   CtxPsnap = (_UxCpreviewSnap *)UxGetContext(w);
	   CtxPsnap->UxselVarWidg = (Widget)NULL;
	   }
	else if (chi == AING_CALLING)
	   {
	   CtxMaster = (_UxCmasterMenu *)UxGetContext(w);
	   CtxMaster->UxselVarAing = (Widget)NULL;
	   }
#endif
	***************/
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
	
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selByAllVariablestb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVar),
	                  UxGetWidget(selByModelForm1), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                   FILTER_ALL_VAR,FILTRO_VAR);
	display_var (UxGetWidget(selVarToSelect1), mod_sel, blocco_sel,
	                     kks_filter_act,tipo_filter_act);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selByModeltb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVar),
	                  UxGetWidget(selByModelForm1), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                   FILTER_MOD_BLOC,FILTRO_VAR);
	display_var (UxGetWidget(selVarToSelect1), mod_sel, blocco_sel,
	                     kks_filter_act,tipo_filter_act);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  armCB_selByModeltb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	
	}
	UxSelVarContext = UxSaveCtx;
}

static void  createCB_selByModeltb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxContext = UxSelVarContext;
	{
	
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selByModelOnlytb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVar),
	                  UxGetWidget(selByModelForm1), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                   FILTER_MOD_ONLY,FILTRO_VAR);
	display_var (UxGetWidget(selVarToSelect1), mod_sel, blocco_sel,
	                     kks_filter_act,tipo_filter_act);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarMenuKkstb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVar),
	                  UxGetWidget(selVarCrKksForm2), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                  FILTER_KKS,FILTRO_VAR);
	display_var (UxGetWidget(selVarToSelect1),
	                  mod_sel, blocco_sel,
	                  kks_filter_act,tipo_filter_act);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarMenuTipotb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVar),
	                  UxGetWidget(selVarCrTipoForm1), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                  FILTER_TYPE,FILTRO_VAR);
	display_var (UxGetWidget(selVarToSelect1),
	                  mod_sel, blocco_sel,
	                  kks_filter_act,tipo_filter_act);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrAllVartb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVar),
	                  UxGetWidget(selByModelForm1), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                   FILTER_ALL_VAR,FILTRO_VAR2);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrModBlocktb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVar),
	                  UxGetWidget(selByModelForm1), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                   FILTER_MOD_BLOC,FILTRO_VAR2);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrModtb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVar),
	                  UxGetWidget(selByModelForm1), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                   FILTER_MOD_ONLY,FILTRO_VAR2);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrKkstb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVar),
	                  UxGetWidget(selVarCrKksForm2), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                  FILTER_KKS,FILTRO_VAR2);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrTypetb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	manage_widget_filter (UxGetWidget(selVar),
	                  UxGetWidget(selVarCrTipoForm1), 
	                  ((XmToggleButtonCallbackStruct *)UxCallbackArg)->set,
	                  FILTER_TYPE,FILTRO_VAR2);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_selVarCrSaveFilterpb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	if (SD_optsave3 (BANCO, (char*)&options,IcProt) > 0)
	      fprintf (stderr,"OPZIONI salvate\n");
	else
	      fprintf (stderr,"*** errore save opzioni\n");
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_selVarCrApplyFilterpb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	if ((chi == MALFUNZIONI) || (chi == SOMMARIO_MALF))
	   applicaFiltri (selVar, FILTRO_MALF);
	else if ((chi == FUNZIONI_REMOTE) || (chi == SOMMARIO_FREM))
	   applicaFiltri (selVar, FILTRO_FREM);
	else
	   applicaFiltri (selVar, FILTRO_VAR2);
	
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_selVarModFindDown( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	next_pos_bloc (selVarListaBlocchi1,SEARCH_DOWN,selVarText2);
#endif
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_selVarModFindUp( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	next_pos_bloc (selVarListaBlocchi1,SEARCH_UP,selVarText2);
#endif
	}
	UxSelVarContext = UxSaveCtx;
}

static void  singleSelectionCB_selVarListaSistemi1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	XmListCallbackStruct *reason;
	   reason = (XmListCallbackStruct *)UxCallbackArg;
	   mod_sel = reason->item_position;
	
	printf ("selezionato modello %d\n",mod_sel);
	/*   display_lista_sistema (selVarListaSistemi1); */
	   display_blocchi (selVarListaBlocchi1);
	/***
	   display_var (selVarToSelect1, mod_sel, blocco_sel,
	                kks_filter_act,tipo_filter_act);
	***/
	   displayLista ();
	}
	UxSelVarContext = UxSaveCtx;
}

static void  singleSelectionCB_selVarListaBlocchi1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	XmListCallbackStruct *reason;
	reason = (XmListCallbackStruct *)UxCallbackArg;
	
	   blocco_sel = reason->item_position;
	printf ("selezionato blocco %d\n",blocco_sel);
	/*****
	   display_var (selVarToSelect1, mod_sel, blocco_sel,
	                 kks_filter_act,tipo_filter_act);
	**********/
	   displayLista ();
	
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_pushButton4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	lettura_filtro_kks (UxWidget);
	displayLista ();
	/*******
	display_var (selVarToSelect1, mod_sel, blocco_sel,
	                 kks_filter_act,tipo_filter_act);
	AGGIUNTE LISTE MALF
	 **********/
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrTipoPressionetb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	lettura_filtro_tipo (UxWidget);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrTipoEntalpiatb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	lettura_filtro_tipo (UxWidget);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrTipoPortatatb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	lettura_filtro_tipo (UxWidget);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrTipoGiritb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	lettura_filtro_tipo (UxWidget);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrTipoTemperaturatb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	lettura_filtro_tipo (UxWidget);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrTipoAlzvalvtb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	lettura_filtro_tipo (UxWidget);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrTipoPotenzatb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	lettura_filtro_tipo (UxWidget);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_pushButton5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	lettura_filtro_tipo (UxWidget); 
	displayLista ();
	/*******
	if ((chi != MALFUNZIONI) && (chi != SOMMARIO_MALF) &&
	    (chi != FUNZIONI_REMOTE) && (chi != SOMMARIO_FREM))
	   {
	   display_var (selVarToSelect1, mod_sel, blocco_sel,
	            kks_filter_act,tipo_filter_act);
	   }
	else
	   {
	   switch (chi)
	      {
	      case MALFUNZIONI:
	      case SOMMARIO_MALF:
	         display_conf_malf (selVarToSelect1,m_component,
	                   malf_set, m_comp_type,mod_sel,blocco_sel,
	                   kks_filter_act,tipo_filter_act);
	   
	         break;
	      case FUNZIONI_REMOTE:
	      case SOMMARIO_FREM:
	         display_conf_malf (selVarToSelect1,fr_component,
	                   frem_set, fr_comp_type,mod_sel,blocco_sel,
	                   kks_filter_act,tipo_filter_act);
	   
	         break;
	      }
	   }
	*************/
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrTipoAingable( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	lettura_filtro_tipo (UxWidget);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrTipoInput( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	lettura_filtro_tipo (UxWidget);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  valueChangedCB_selVarCrTipoOutput( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	lettura_filtro_tipo (UxWidget);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_selVarFindUp1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	next_pos (selVarToSelect1,SEARCH_UP,selVarText1);
#endif
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_selVarFindDown1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	next_pos (selVarToSelect1,SEARCH_DOWN,selVarText1);
#endif
	}
	UxSelVarContext = UxSaveCtx;
}

static void  singleSelectionCB_selVarToSelect1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	int n; 
	SCENARIO *pscen;
	char stringa[200];
	XmListCallbackStruct *reason = (XmListCallbackStruct *)UxCallbackArg;
	
	switch (chi)
	   {
	   case PREVIEW_CALLING:
	   case AING_CALLING:
	      strcpy (stringa, extract_string (reason->item));
	      strncpy (label_sel, stringa, MAX_LUN_NOME_VAR-1);
	      label_sel[MAX_LUN_NOME_VAR-1] = '\0';
	      XtVaSetValues (selOkpb, XmNsensitive, True, NULL);
	      XtVaSetValues (selVarMenuPopupSelect, XmNsensitive, True, NULL);
	      break;
	   case MALFUNZIONI:
	   case SOMMARIO_MALF:
	   case FUNZIONI_REMOTE:
	   case SOMMARIO_FREM:
	      conf_malf_select = reason->item_position;
	      printf ("lista: conf_malf_select = %d\n",conf_malf_select);
	      pscen = (SCENARIO *)p;
	      if (pscen->scen_header.numvar >= MAX_VAR_SCENARIO)
	         {
	         XtVaSetValues (selOkpb,XmNsensitive,False,NULL);
	         XtVaSetValues (selVarMenuPopupSelect,XmNsensitive,False,NULL);
	         }
	      else
	         {
	         XtVaSetValues (selOkpb, XmNsensitive, True, NULL);
	         XtVaSetValues (selVarMenuPopupSelect, XmNsensitive, True, NULL);
	         }
	      break;
	   }
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_selVarMenuPopupSaveFilter( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	if (SD_optsave3 (BANCO, (char*)&options,IcProt) > 0)
	      fprintf (stderr,"OPZIONI salvate\n");
	else
	      fprintf (stderr,"*** errore save opzioni\n");
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_selVarMenuPopupApplyFilter( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	if ((chi == MALFUNZIONI) || (chi == SOMMARIO_MALF))
	   applicaFiltri (selVar, FILTRO_MALF);
	else if ((chi == FUNZIONI_REMOTE) || (chi == SOMMARIO_FREM))
	   applicaFiltri (selVar, FILTRO_FREM);
	else
	   applicaFiltri (selVar, FILTRO_VAR2);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_selVarMenuPopupSelect( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	selectSelVarOk (UxWidget);
	}
	UxSelVarContext = UxSaveCtx;
}

static void  activateCB_selVarMenuPopupQuit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselVar              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelVarContext;
	UxSelVarContext = UxContext =
			(_UxCselVar *) UxGetContext( UxWidget );
	{
	/******** in destroy callback di selVar
#ifndef DESIGN_TIME
	_UxCpreviewSnap *CtxPsnap;
	_UxCmasterMenu *CtxMaster;
	if (chi == PREVIEW_CALLING)
	   {
	   CtxPsnap = (_UxCpreviewSnap *)UxGetContext(w);
	   CtxPsnap->UxselVarWidg = (Widget)NULL;
	   }
	else if (chi == AING_CALLING)
	   {
	   CtxMaster = (_UxCmasterMenu *)UxGetContext(w);
	   CtxMaster->UxselVarAing = (Widget)NULL;
	   }
#endif
	*************/
	DistruggiInterfaccia (XtParent(selVar));
	}
	UxSelVarContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_selVar()
{
	Widget		_UxParent;
	Widget		selVarCrMenu1_shell;
	Widget		selVarMenuFilter1_shell;
	Widget		selVarMenuOtherPane1_shell;
	Widget		menu13_shell;
	char		*UxTmp0;


	/* Creation of selVar */
	_UxParent = w;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "selVar_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 290,
			XmNy, 10,
			XmNwidth, 550,
			XmNheight, 700,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "selVar",
			NULL );

	selVar = XtVaCreateWidget( "selVar",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 550,
			XmNheight, 700,
			NULL );
	XtAddCallback( selVar, XmNdestroyCallback,
		(XtCallbackProc) destroyCB_selVar,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVar, (char *) UxSelVarContext );


	/* Creation of frame2 */
	frame2 = XtVaCreateManagedWidget( "frame2",
			xmFrameWidgetClass,
			selVar,
			XmNx, 0,
			XmNy, 480,
			XmNwidth, 540,
			XmNheight, 80,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame2, (char *) UxSelVarContext );


	/* Creation of form42 */
	form42 = XtVaCreateManagedWidget( "form42",
			xmFormWidgetClass,
			frame2,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 10,
			XmNy, 10,
			XmNwidth, 490,
			XmNheight, 50,
			NULL );
	UxPutContext( form42, (char *) UxSelVarContext );

	UxTmp0 = SELVAR_CONFIRM ? SELVAR_CONFIRM : "OK";

	/* Creation of selOkpb */
	selOkpb = XtVaCreateManagedWidget( "selOkpb",
			xmPushButtonWidgetClass,
			form42,
			XmNx, 20,
			XmNy, 20,
			XmNwidth, 100,
			XmNheight, 40,
			XmNtopOffset, 15,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNrightAttachment, XmATTACH_SELF,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, UxTmp0 ),
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			NULL );
	XtAddCallback( selOkpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_selOkpb,
		UxGetWidget(selVar) );

	UxPutContext( selOkpb, (char *) UxSelVarContext );


	/* Creation of selQuitpb */
	selQuitpb = XtVaCreateManagedWidget( "selQuitpb",
			xmPushButtonWidgetClass,
			form42,
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
	XtAddCallback( selQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_selQuitpb,
		selVar );

	UxPutContext( selQuitpb, (char *) UxSelVarContext );


	/* Creation of menu4 */
	menu4 = XtVaCreateManagedWidget( "menu4",
			xmRowColumnWidgetClass,
			selVar,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( menu4, (char *) UxSelVarContext );


	/* Creation of selVarCrMenu1 */
	selVarCrMenu1_shell = XtVaCreatePopupShell ("selVarCrMenu1_shell",
			xmMenuShellWidgetClass, menu4,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	selVarCrMenu1 = XtVaCreateWidget( "selVarCrMenu1",
			xmRowColumnWidgetClass,
			selVarCrMenu1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( selVarCrMenu1, (char *) UxSelVarContext );


	/* Creation of selVarCrMenuFilepb1 */
	selVarCrMenuFilepb1 = XtVaCreateManagedWidget( "selVarCrMenuFilepb1",
			xmPushButtonWidgetClass,
			selVarCrMenu1,
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );
	XtAddCallback( selVarCrMenuFilepb1, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarCrMenuFilepb1,
		selVar );

	UxPutContext( selVarCrMenuFilepb1, (char *) UxSelVarContext );


	/* Creation of menu3_top_b3 */
	menu3_top_b3 = XtVaCreateManagedWidget( "menu3_top_b3",
			xmCascadeButtonWidgetClass,
			menu4,
			RES_CONVERT( XmNlabelString, "File" ),
			XmNsubMenuId, selVarCrMenu1,
			NULL );
	UxPutContext( menu3_top_b3, (char *) UxSelVarContext );


	/* Creation of selVarMenuFilter1 */
	selVarMenuFilter1_shell = XtVaCreatePopupShell ("selVarMenuFilter1_shell",
			xmMenuShellWidgetClass, menu4,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	selVarMenuFilter1 = XtVaCreateWidget( "selVarMenuFilter1",
			xmRowColumnWidgetClass,
			selVarMenuFilter1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( selVarMenuFilter1, (char *) UxSelVarContext );


	/* Creation of selVarMenuOtherPane1 */
	selVarMenuOtherPane1_shell = XtVaCreatePopupShell ("selVarMenuOtherPane1_shell",
			xmMenuShellWidgetClass, selVarMenuFilter1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	selVarMenuOtherPane1 = XtVaCreateWidget( "selVarMenuOtherPane1",
			xmRowColumnWidgetClass,
			selVarMenuOtherPane1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( selVarMenuOtherPane1, (char *) UxSelVarContext );


	/* Creation of selByAllVariablestb1 */
	selByAllVariablestb1 = XtVaCreateManagedWidget( "selByAllVariablestb1",
			xmToggleButtonWidgetClass,
			selVarMenuOtherPane1,
			RES_CONVERT( XmNlabelString, "All variables" ),
			XmNindicatorType, XmONE_OF_MANY,
			XmNvisibleWhenOff, TRUE,
			XmNset, TRUE,
			NULL );
	XtAddCallback( selByAllVariablestb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selByAllVariablestb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selByAllVariablestb1, (char *) UxSelVarContext );


	/* Creation of selByModeltb1 */
	selByModeltb1 = XtVaCreateManagedWidget( "selByModeltb1",
			xmToggleButtonWidgetClass,
			selVarMenuOtherPane1,
			RES_CONVERT( XmNlabelString, "models & blocks" ),
			XmNset, FALSE,
			XmNvisibleWhenOff, TRUE,
			XmNindicatorType, XmONE_OF_MANY,
			NULL );
	XtAddCallback( selByModeltb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selByModeltb1,
		(XtPointer) UxSelVarContext );
	XtAddCallback( selByModeltb1, XmNarmCallback,
		(XtCallbackProc) armCB_selByModeltb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selByModeltb1, (char *) UxSelVarContext );

	createCB_selByModeltb1( selByModeltb1,
			(XtPointer) UxSelVarContext, (XtPointer) NULL );


	/* Creation of selByModelOnlytb1 */
	selByModelOnlytb1 = XtVaCreateManagedWidget( "selByModelOnlytb1",
			xmToggleButtonWidgetClass,
			selVarMenuOtherPane1,
			RES_CONVERT( XmNlabelString, "models only" ),
			XmNindicatorType, XmONE_OF_MANY,
			XmNvisibleWhenOff, TRUE,
			NULL );
	XtAddCallback( selByModelOnlytb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selByModelOnlytb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selByModelOnlytb1, (char *) UxSelVarContext );


	/* Creation of selVarMenuOtherPane_b1 */
	selVarMenuOtherPane_b1 = XtVaCreateManagedWidget( "selVarMenuOtherPane_b1",
			xmSeparatorWidgetClass,
			selVarMenuOtherPane1,
			NULL );
	UxPutContext( selVarMenuOtherPane_b1, (char *) UxSelVarContext );


	/* Creation of selVarMenuKkstb1 */
	selVarMenuKkstb1 = XtVaCreateManagedWidget( "selVarMenuKkstb1",
			xmToggleButtonWidgetClass,
			selVarMenuOtherPane1,
			RES_CONVERT( XmNlabelString, "KKS filter" ),
			XmNset, TRUE,
			XmNvisibleWhenOff, TRUE,
			XmNindicatorType, XmN_OF_MANY,
			NULL );
	XtAddCallback( selVarMenuKkstb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarMenuKkstb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarMenuKkstb1, (char *) UxSelVarContext );


	/* Creation of selVarMenuOtherPane_b2 */
	selVarMenuOtherPane_b2 = XtVaCreateManagedWidget( "selVarMenuOtherPane_b2",
			xmSeparatorWidgetClass,
			selVarMenuOtherPane1,
			NULL );
	UxPutContext( selVarMenuOtherPane_b2, (char *) UxSelVarContext );


	/* Creation of selVarMenuTipotb1 */
	selVarMenuTipotb1 = XtVaCreateManagedWidget( "selVarMenuTipotb1",
			xmToggleButtonWidgetClass,
			selVarMenuOtherPane1,
			RES_CONVERT( XmNlabelString, "by type" ),
			XmNvisibleWhenOff, TRUE,
			NULL );
	XtAddCallback( selVarMenuTipotb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarMenuTipotb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarMenuTipotb1, (char *) UxSelVarContext );


	/* Creation of selVarMenuFilter_b1 */
	selVarMenuFilter_b1 = XtVaCreateManagedWidget( "selVarMenuFilter_b1",
			xmCascadeButtonWidgetClass,
			selVarMenuFilter1,
			RES_CONVERT( XmNlabelString, "Select filter ..." ),
			XmNsubMenuId, selVarMenuOtherPane1,
			NULL );
	UxPutContext( selVarMenuFilter_b1, (char *) UxSelVarContext );


	/* Creation of menu3_top_b4 */
	menu3_top_b4 = XtVaCreateWidget( "menu3_top_b4",
			xmCascadeButtonWidgetClass,
			menu4,
			RES_CONVERT( XmNlabelString, "Filter" ),
			XmNsubMenuId, selVarMenuFilter1,
			NULL );
	UxPutContext( menu3_top_b4, (char *) UxSelVarContext );


	/* Creation of frame27 */
	frame27 = XtVaCreateManagedWidget( "frame27",
			xmFrameWidgetClass,
			selVar,
			XmNx, 0,
			XmNy, 480,
			XmNwidth, 540,
			XmNheight, 80,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 0,
			XmNtopWidget, menu4,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame27, (char *) UxSelVarContext );


	/* Creation of selVarCrFilterForm1 */
	selVarCrFilterForm1 = XtVaCreateManagedWidget( "selVarCrFilterForm1",
			xmFormWidgetClass,
			frame27,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 2,
			XmNy, 2,
			XmNwidth, 536,
			XmNheight, 58,
			NULL );
	UxPutContext( selVarCrFilterForm1, (char *) UxSelVarContext );


	/* Creation of frame28 */
	frame28 = XtVaCreateManagedWidget( "frame28",
			xmFrameWidgetClass,
			selVarCrFilterForm1,
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
	UxPutContext( frame28, (char *) UxSelVarContext );


	/* Creation of form51 */
	form51 = XtVaCreateManagedWidget( "form51",
			xmFormWidgetClass,
			frame28,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 50,
			XmNy, 10,
			XmNwidth, 80,
			XmNheight, 30,
			NULL );
	UxPutContext( form51, (char *) UxSelVarContext );


	/* Creation of selVarCrAllVartb1 */
	selVarCrAllVartb1 = XtVaCreateManagedWidget( "selVarCrAllVartb1",
			xmToggleButtonWidgetClass,
			form51,
			XmNx, 10,
			XmNy, 0,
			XmNwidth, 128,
			XmNheight, 20,
			XmNindicatorType, XmONE_OF_MANY,
			RES_CONVERT( XmNlabelString, "All Variables" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	XtAddCallback( selVarCrAllVartb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrAllVartb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrAllVartb1, (char *) UxSelVarContext );


	/* Creation of selVarCrModBlocktb1 */
	selVarCrModBlocktb1 = XtVaCreateManagedWidget( "selVarCrModBlocktb1",
			xmToggleButtonWidgetClass,
			form51,
			XmNx, 10,
			XmNy, 20,
			XmNwidth, 128,
			XmNheight, 20,
			XmNindicatorType, XmONE_OF_MANY,
			RES_CONVERT( XmNlabelString, "Mod.&Blocks" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	XtAddCallback( selVarCrModBlocktb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrModBlocktb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrModBlocktb1, (char *) UxSelVarContext );


	/* Creation of selVarCrModtb1 */
	selVarCrModtb1 = XtVaCreateManagedWidget( "selVarCrModtb1",
			xmToggleButtonWidgetClass,
			form51,
			XmNx, 10,
			XmNy, 40,
			XmNwidth, 130,
			XmNheight, 20,
			XmNindicatorType, XmONE_OF_MANY,
			RES_CONVERT( XmNlabelString, "Models Only" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	XtAddCallback( selVarCrModtb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrModtb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrModtb1, (char *) UxSelVarContext );


	/* Creation of frame29 */
	frame29 = XtVaCreateManagedWidget( "frame29",
			xmFrameWidgetClass,
			selVarCrFilterForm1,
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
			XmNrightWidget, frame28,
			NULL );
	UxPutContext( frame29, (char *) UxSelVarContext );


	/* Creation of form52 */
	form52 = XtVaCreateManagedWidget( "form52",
			xmFormWidgetClass,
			frame29,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 10,
			XmNy, 5,
			XmNwidth, 138,
			XmNheight, 56,
			NULL );
	UxPutContext( form52, (char *) UxSelVarContext );


	/* Creation of selVarCrKkstb1 */
	selVarCrKkstb1 = XtVaCreateManagedWidget( "selVarCrKkstb1",
			xmToggleButtonWidgetClass,
			form52,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 100,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "by KKS" ),
			NULL );
	XtAddCallback( selVarCrKkstb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrKkstb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrKkstb1, (char *) UxSelVarContext );


	/* Creation of selVarCrTypetb1 */
	selVarCrTypetb1 = XtVaCreateManagedWidget( "selVarCrTypetb1",
			xmToggleButtonWidgetClass,
			form52,
			XmNx, 0,
			XmNy, 20,
			XmNwidth, 100,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "by Type" ),
			NULL );
	XtAddCallback( selVarCrTypetb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTypetb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrTypetb1, (char *) UxSelVarContext );


	/* Creation of selVarCrSaveFilterpb1 */
	selVarCrSaveFilterpb1 = XtVaCreateManagedWidget( "selVarCrSaveFilterpb1",
			xmPushButtonWidgetClass,
			selVarCrFilterForm1,
			XmNx, 10,
			XmNy, 10,
			XmNwidth, 190,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, SAVEFILTER ),
			NULL );
	XtAddCallback( selVarCrSaveFilterpb1, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarCrSaveFilterpb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrSaveFilterpb1, (char *) UxSelVarContext );


	/* Creation of selVarCrApplyFilterpb1 */
	selVarCrApplyFilterpb1 = XtVaCreateManagedWidget( "selVarCrApplyFilterpb1",
			xmPushButtonWidgetClass,
			selVarCrFilterForm1,
			XmNx, 10,
			XmNy, 40,
			XmNwidth, 190,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, APPLYFILTER ),
			NULL );
	XtAddCallback( selVarCrApplyFilterpb1, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarCrApplyFilterpb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrApplyFilterpb1, (char *) UxSelVarContext );


	/* Creation of panedWindow2 */
	panedWindow2 = XtVaCreateManagedWidget( "panedWindow2",
			xmPanedWindowWidgetClass,
			selVar,
			XmNx, 0,
			XmNy, 40,
			XmNwidth, 540,
			XmNheight, 440,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, frame2,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, frame27,
			NULL );
	UxPutContext( panedWindow2, (char *) UxSelVarContext );


	/* Creation of selByModelForm1 */
	selByModelForm1 = XtVaCreateManagedWidget( "selByModelForm1",
			xmFormWidgetClass,
			panedWindow2,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 10,
			XmNwidth, 540,
			XmNheight, 150,
			XmNpaneMinimum, 100,
			NULL );
	UxPutContext( selByModelForm1, (char *) UxSelVarContext );


	/* Creation of label8 */
	label8 = XtVaCreateManagedWidget( "label8",
			xmLabelWidgetClass,
			selByModelForm1,
			XmNx, 220,
			XmNy, 0,
			XmNwidth, 170,
			XmNheight, 30,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopOffset, 0,
			RES_CONVERT( XmNlabelString, BLOCK_LIST ),
			NULL );
	UxPutContext( label8, (char *) UxSelVarContext );

	UxTmp0 = SYSTEM_LIST ? SYSTEM_LIST : "SYSTEMS";

	/* Creation of label9 */
	label9 = XtVaCreateManagedWidget( "label9",
			xmLabelWidgetClass,
			selByModelForm1,
			XmNx, 20,
			XmNy, 0,
			XmNwidth, 170,
			XmNheight, 30,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, UxTmp0 ),
			NULL );
	UxPutContext( label9, (char *) UxSelVarContext );


	/* Creation of form43 */
	form43 = XtVaCreateManagedWidget( "form43",
			xmFormWidgetClass,
			selByModelForm1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 10,
			XmNy, 140,
			XmNwidth, 534,
			XmNheight, 30,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( form43, (char *) UxSelVarContext );


	/* Creation of selVarModFindDown */
	selVarModFindDown = XtVaCreateManagedWidget( "selVarModFindDown",
			xmArrowButtonWidgetClass,
			form43,
			XmNx, 250,
			XmNy, 0,
			XmNwidth, 60,
			XmNheight, 30,
			XmNarrowDirection, XmARROW_DOWN,
			XmNleftAttachment, XmATTACH_NONE,
			XmNleftOffset, 5,
			XmNleftWidget, NULL,
			XmNbottomOffset, 3,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 3,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 15,
			NULL );
	XtAddCallback( selVarModFindDown, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarModFindDown,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarModFindDown, (char *) UxSelVarContext );


	/* Creation of selVarText2 */
	selVarText2 = XtVaCreateManagedWidget( "selVarText2",
			xmTextFieldWidgetClass,
			form43,
			XmNx, 320,
			XmNy, 0,
			XmNwidth, 150,
			XmNheight, 30,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_NONE,
			XmNleftOffset, 0,
			XmNleftWidget, NULL,
			XmNbottomOffset, 0,
			XmNtopOffset, 0,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightWidget, selVarModFindDown,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarText2, (char *) UxSelVarContext );


	/* Creation of selVarModFindUp */
	selVarModFindUp = XtVaCreateManagedWidget( "selVarModFindUp",
			xmArrowButtonWidgetClass,
			form43,
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
			XmNrightWidget, selVarText2,
			NULL );
	XtAddCallback( selVarModFindUp, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarModFindUp,
		(XtPointer) 0x0 );

	UxPutContext( selVarModFindUp, (char *) UxSelVarContext );


	/* Creation of scrolledWindow10 */
	scrolledWindow10 = XtVaCreateManagedWidget( "scrolledWindow10",
			xmScrolledWindowWidgetClass,
			selByModelForm1,
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
			XmNtopWidget, label9,
			XmNwidth, 180,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, form43,
			NULL );
	UxPutContext( scrolledWindow10, (char *) UxSelVarContext );


	/* Creation of selVarListaSistemi1 */
	selVarListaSistemi1 = XtVaCreateManagedWidget( "selVarListaSistemi1",
			xmListWidgetClass,
			scrolledWindow10,
			XmNwidth, 170,
			XmNheight, 115,
			XmNautomaticSelection, TRUE,
			XmNselectionPolicy, XmSINGLE_SELECT,
			NULL );
	XtAddCallback( selVarListaSistemi1, XmNsingleSelectionCallback,
		(XtCallbackProc) singleSelectionCB_selVarListaSistemi1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarListaSistemi1, (char *) UxSelVarContext );


	/* Creation of scrolledWindow11 */
	scrolledWindow11 = XtVaCreateManagedWidget( "scrolledWindow11",
			xmScrolledWindowWidgetClass,
			selByModelForm1,
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
			XmNleftWidget, scrolledWindow10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, label8,
			XmNwidth, 300,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNbottomWidget, form43,
			NULL );
	UxPutContext( scrolledWindow11, (char *) UxSelVarContext );


	/* Creation of selVarListaBlocchi1 */
	selVarListaBlocchi1 = XtVaCreateManagedWidget( "selVarListaBlocchi1",
			xmListWidgetClass,
			scrolledWindow11,
			XmNwidth, 270,
			XmNheight, 115,
			XmNautomaticSelection, TRUE,
			XmNselectionPolicy, XmSINGLE_SELECT,
			NULL );
	XtAddCallback( selVarListaBlocchi1, XmNsingleSelectionCallback,
		(XtCallbackProc) singleSelectionCB_selVarListaBlocchi1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarListaBlocchi1, (char *) UxSelVarContext );


	/* Creation of selVarCrKksForm2 */
	selVarCrKksForm2 = XtVaCreateManagedWidget( "selVarCrKksForm2",
			xmFormWidgetClass,
			panedWindow2,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 200,
			XmNwidth, 540,
			XmNheight, 160,
			XmNpaneMinimum, 80,
			XmNpaneMaximum, 80,
			NULL );
	UxPutContext( selVarCrKksForm2, (char *) UxSelVarContext );


	/* Creation of selVarCrKksBullBoard2 */
	selVarCrKksBullBoard2 = XtVaCreateManagedWidget( "selVarCrKksBullBoard2",
			xmBulletinBoardWidgetClass,
			selVarCrKksForm2,
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
	UxPutContext( selVarCrKksBullBoard2, (char *) UxSelVarContext );


	/* Creation of selVarCrKksText22 */
	selVarCrKksText22 = XtVaCreateManagedWidget( "selVarCrKksText22",
			xmTextWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 50,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			XmNcolumns, 1,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarCrKksText22, (char *) UxSelVarContext );


	/* Creation of selVarCrKksSistemaLabel2 */
	selVarCrKksSistemaLabel2 = XtVaCreateManagedWidget( "selVarCrKksSistemaLabel2",
			xmLabelWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 60,
			XmNy, 20,
			XmNwidth, 70,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, KKS_SYST_LABEL ),
			NULL );
	UxPutContext( selVarCrKksSistemaLabel2, (char *) UxSelVarContext );


	/* Creation of selVarCrKksSottosistLabel2 */
	selVarCrKksSottosistLabel2 = XtVaCreateManagedWidget( "selVarCrKksSottosistLabel2",
			xmLabelWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 140,
			XmNy, 20,
			XmNwidth, 70,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, KKS_SSYST_LABEL ),
			NULL );
	UxPutContext( selVarCrKksSottosistLabel2, (char *) UxSelVarContext );


	/* Creation of selVarCrKksComponentLabel2 */
	selVarCrKksComponentLabel2 = XtVaCreateManagedWidget( "selVarCrKksComponentLabel2",
			xmLabelWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 210,
			XmNy, 20,
			XmNwidth, 70,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, KKS_COMP_LABEL ),
			NULL );
	UxPutContext( selVarCrKksComponentLabel2, (char *) UxSelVarContext );


	/* Creation of selVarCrKksText23 */
	selVarCrKksText23 = XtVaCreateManagedWidget( "selVarCrKksText23",
			xmTextWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 80,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarCrKksText23, (char *) UxSelVarContext );


	/* Creation of selVarCrKksText24 */
	selVarCrKksText24 = XtVaCreateManagedWidget( "selVarCrKksText24",
			xmTextWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 110,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarCrKksText24, (char *) UxSelVarContext );


	/* Creation of selVarCrKksText25 */
	selVarCrKksText25 = XtVaCreateManagedWidget( "selVarCrKksText25",
			xmTextWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 150,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarCrKksText25, (char *) UxSelVarContext );


	/* Creation of selVarCrKksText26 */
	selVarCrKksText26 = XtVaCreateManagedWidget( "selVarCrKksText26",
			xmTextWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 180,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarCrKksText26, (char *) UxSelVarContext );


	/* Creation of selVarCrKksText27 */
	selVarCrKksText27 = XtVaCreateManagedWidget( "selVarCrKksText27",
			xmTextWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 220,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarCrKksText27, (char *) UxSelVarContext );


	/* Creation of selVarCrKksText28 */
	selVarCrKksText28 = XtVaCreateManagedWidget( "selVarCrKksText28",
			xmTextWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 250,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarCrKksText28, (char *) UxSelVarContext );


	/* Creation of selVarCrKksText29 */
	selVarCrKksText29 = XtVaCreateManagedWidget( "selVarCrKksText29",
			xmTextWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 290,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			XmNcolumns, 1,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarCrKksText29, (char *) UxSelVarContext );


	/* Creation of selVarCrKksProgressLabel2 */
	selVarCrKksProgressLabel2 = XtVaCreateManagedWidget( "selVarCrKksProgressLabel2",
			xmLabelWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 280,
			XmNy, 20,
			XmNwidth, 110,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, KKS_PROG_LABEL ),
			NULL );
	UxPutContext( selVarCrKksProgressLabel2, (char *) UxSelVarContext );


	/* Creation of selVarCrKksText30 */
	selVarCrKksText30 = XtVaCreateManagedWidget( "selVarCrKksText30",
			xmTextWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 320,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarCrKksText30, (char *) UxSelVarContext );


	/* Creation of selVarCrKksText31 */
	selVarCrKksText31 = XtVaCreateManagedWidget( "selVarCrKksText31",
			xmTextWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 350,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarCrKksText31, (char *) UxSelVarContext );


	/* Creation of selVarCrKksText32 */
	selVarCrKksText32 = XtVaCreateManagedWidget( "selVarCrKksText32",
			xmTextWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 10,
			XmNy, 40,
			XmNwidth, 25,
			XmNheight, 25,
			XmNcolumns, 1,
			XmNmaxLength, 1,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarCrKksText32, (char *) UxSelVarContext );


	/* Creation of selVarCrKksNimpLabel2 */
	selVarCrKksNimpLabel2 = XtVaCreateManagedWidget( "selVarCrKksNimpLabel2",
			xmLabelWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 10,
			XmNy, 20,
			XmNwidth, 40,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, KKS_NIMP_LABEL ),
			NULL );
	UxPutContext( selVarCrKksNimpLabel2, (char *) UxSelVarContext );


	/* Creation of pushButton4 */
	pushButton4 = XtVaCreateManagedWidget( "pushButton4",
			xmPushButtonWidgetClass,
			selVarCrKksBullBoard2,
			XmNx, 420,
			XmNy, 40,
			XmNwidth, 120,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, KKS_APPLY_FILTER ),
			NULL );
	XtAddCallback( pushButton4, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton4,
		(XtPointer) UxSelVarContext );

	UxPutContext( pushButton4, (char *) UxSelVarContext );


	/* Creation of selVarCrTipoForm1 */
	selVarCrTipoForm1 = XtVaCreateManagedWidget( "selVarCrTipoForm1",
			xmFormWidgetClass,
			panedWindow2,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 10,
			XmNy, 140,
			XmNwidth, 520,
			XmNheight, 30,
			XmNpaneMinimum, 80,
			XmNpaneMaximum, 80,
			NULL );
	UxPutContext( selVarCrTipoForm1, (char *) UxSelVarContext );


	/* Creation of selVarCrTipoPressionetb1 */
	selVarCrTipoPressionetb1 = XtVaCreateManagedWidget( "selVarCrTipoPressionetb1",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm1,
			XmNx, 10,
			XmNy, 0,
			XmNwidth, 90,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, TIPO_PRESSIONE ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	XtAddCallback( selVarCrTipoPressionetb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoPressionetb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrTipoPressionetb1, (char *) UxSelVarContext );


	/* Creation of selVarCrTipoEntalpiatb1 */
	selVarCrTipoEntalpiatb1 = XtVaCreateManagedWidget( "selVarCrTipoEntalpiatb1",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm1,
			XmNx, 100,
			XmNy, 40,
			XmNwidth, 90,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_ENTALPIA ),
			NULL );
	XtAddCallback( selVarCrTipoEntalpiatb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoEntalpiatb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrTipoEntalpiatb1, (char *) UxSelVarContext );


	/* Creation of selVarCrTipoPortatatb1 */
	selVarCrTipoPortatatb1 = XtVaCreateManagedWidget( "selVarCrTipoPortatatb1",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm1,
			XmNx, 10,
			XmNy, 40,
			XmNwidth, 90,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_PORTATA ),
			NULL );
	XtAddCallback( selVarCrTipoPortatatb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoPortatatb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrTipoPortatatb1, (char *) UxSelVarContext );


	/* Creation of selVarCrTipoGiritb1 */
	selVarCrTipoGiritb1 = XtVaCreateManagedWidget( "selVarCrTipoGiritb1",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm1,
			XmNx, 100,
			XmNy, 20,
			XmNwidth, 90,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_GIRI ),
			NULL );
	XtAddCallback( selVarCrTipoGiritb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoGiritb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrTipoGiritb1, (char *) UxSelVarContext );


	/* Creation of selVarCrTipoTemperaturatb1 */
	selVarCrTipoTemperaturatb1 = XtVaCreateManagedWidget( "selVarCrTipoTemperaturatb1",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm1,
			XmNx, 10,
			XmNy, 20,
			XmNwidth, 90,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_TEMPERATURA ),
			NULL );
	XtAddCallback( selVarCrTipoTemperaturatb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoTemperaturatb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrTipoTemperaturatb1, (char *) UxSelVarContext );


	/* Creation of selVarCrTipoAlzvalvtb1 */
	selVarCrTipoAlzvalvtb1 = XtVaCreateManagedWidget( "selVarCrTipoAlzvalvtb1",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm1,
			XmNx, 200,
			XmNy, 0,
			XmNwidth, 90,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_ALZVALV ),
			NULL );
	XtAddCallback( selVarCrTipoAlzvalvtb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoAlzvalvtb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrTipoAlzvalvtb1, (char *) UxSelVarContext );


	/* Creation of selVarCrTipoPotenzatb1 */
	selVarCrTipoPotenzatb1 = XtVaCreateManagedWidget( "selVarCrTipoPotenzatb1",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm1,
			XmNx, 100,
			XmNy, 0,
			XmNwidth, 90,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_POTENZA ),
			NULL );
	XtAddCallback( selVarCrTipoPotenzatb1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoPotenzatb1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrTipoPotenzatb1, (char *) UxSelVarContext );


	/* Creation of pushButton5 */
	pushButton5 = XtVaCreateManagedWidget( "pushButton5",
			xmPushButtonWidgetClass,
			selVarCrTipoForm1,
			XmNx, 420,
			XmNy, 40,
			XmNwidth, 120,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, KKS_APPLY_FILTER ),
			NULL );
	XtAddCallback( pushButton5, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton5,
		(XtPointer) UxSelVarContext );

	UxPutContext( pushButton5, (char *) UxSelVarContext );


	/* Creation of selVarCrTipoAingable */
	selVarCrTipoAingable = XtVaCreateManagedWidget( "selVarCrTipoAingable",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm1,
			XmNx, 290,
			XmNy, 0,
			XmNwidth, 120,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_AINGABLE ),
			NULL );
	XtAddCallback( selVarCrTipoAingable, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoAingable,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrTipoAingable, (char *) UxSelVarContext );


	/* Creation of selVarCrTipoInput */
	selVarCrTipoInput = XtVaCreateManagedWidget( "selVarCrTipoInput",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm1,
			XmNx, 290,
			XmNy, 20,
			XmNwidth, 120,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_INPUT ),
			NULL );
	XtAddCallback( selVarCrTipoInput, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoInput,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrTipoInput, (char *) UxSelVarContext );


	/* Creation of selVarCrTipoOutput */
	selVarCrTipoOutput = XtVaCreateManagedWidget( "selVarCrTipoOutput",
			xmToggleButtonWidgetClass,
			selVarCrTipoForm1,
			XmNx, 290,
			XmNy, 40,
			XmNwidth, 120,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, TIPO_OUTPUT ),
			NULL );
	XtAddCallback( selVarCrTipoOutput, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_selVarCrTipoOutput,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarCrTipoOutput, (char *) UxSelVarContext );


	/* Creation of form40 */
	form40 = XtVaCreateManagedWidget( "form40",
			xmFormWidgetClass,
			panedWindow2,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 13,
			XmNy, 13,
			XmNwidth, 540,
			XmNheight, 150,
			XmNpaneMinimum, 140,
			NULL );
	UxPutContext( form40, (char *) UxSelVarContext );


	/* Creation of label10 */
	label10 = XtVaCreateManagedWidget( "label10",
			xmLabelWidgetClass,
			form40,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 534,
			XmNheight, 30,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "" ),
			NULL );
	UxPutContext( label10, (char *) UxSelVarContext );


	/* Creation of form41 */
	form41 = XtVaCreateManagedWidget( "form41",
			xmFormWidgetClass,
			form40,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 300,
			XmNwidth, 534,
			XmNheight, 30,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( form41, (char *) UxSelVarContext );


	/* Creation of selVarFindUp1 */
	selVarFindUp1 = XtVaCreateManagedWidget( "selVarFindUp1",
			xmArrowButtonWidgetClass,
			form41,
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
	XtAddCallback( selVarFindUp1, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarFindUp1,
		(XtPointer) 0x0 );

	UxPutContext( selVarFindUp1, (char *) UxSelVarContext );


	/* Creation of selVarText1 */
	selVarText1 = XtVaCreateManagedWidget( "selVarText1",
			xmTextFieldWidgetClass,
			form41,
			XmNx, 20,
			XmNy, 10,
			XmNwidth, 140,
			XmNheight, 20,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, selVarFindUp1,
			XmNbottomOffset, 0,
			XmNtopOffset, 0,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( selVarText1, (char *) UxSelVarContext );


	/* Creation of selVarFindDown1 */
	selVarFindDown1 = XtVaCreateManagedWidget( "selVarFindDown1",
			xmArrowButtonWidgetClass,
			form41,
			XmNx, 250,
			XmNy, 0,
			XmNwidth, 60,
			XmNheight, 30,
			XmNarrowDirection, XmARROW_DOWN,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, selVarText1,
			XmNbottomOffset, 3,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 3,
			NULL );
	XtAddCallback( selVarFindDown1, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarFindDown1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarFindDown1, (char *) UxSelVarContext );


	/* Creation of scrolledWindow12 */
	scrolledWindow12 = XtVaCreateManagedWidget( "scrolledWindow12",
			xmScrolledWindowWidgetClass,
			form40,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 20,
			XmNy, 30,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 5,
			XmNbottomWidget, form41,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 5,
			XmNtopWidget, label10,
			NULL );
	UxPutContext( scrolledWindow12, (char *) UxSelVarContext );


	/* Creation of selVarToSelect1 */
	selVarToSelect1 = XtVaCreateManagedWidget( "selVarToSelect1",
			xmListWidgetClass,
			scrolledWindow12,
			XmNwidth, 490,
			XmNheight, 80,
			XmNselectionPolicy, XmSINGLE_SELECT,
			NULL );
	XtAddCallback( selVarToSelect1, XmNsingleSelectionCallback,
		(XtCallbackProc) singleSelectionCB_selVarToSelect1,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarToSelect1, (char *) UxSelVarContext );


	/* Creation of menu13 */
	menu13_shell = XtVaCreatePopupShell ("menu13_shell",
			xmMenuShellWidgetClass, selVar,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu13 = XtVaCreateWidget( "menu13",
			xmRowColumnWidgetClass,
			menu13_shell,
			XmNrowColumnType, XmMENU_POPUP,
			NULL );
	UxPutContext( menu13, (char *) UxSelVarContext );


	/* Creation of selVarMenuPopupSaveFilter */
	selVarMenuPopupSaveFilter = XtVaCreateManagedWidget( "selVarMenuPopupSaveFilter",
			xmPushButtonWidgetClass,
			menu13,
			RES_CONVERT( XmNlabelString, "Save Filter" ),
			NULL );
	XtAddCallback( selVarMenuPopupSaveFilter, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarMenuPopupSaveFilter,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarMenuPopupSaveFilter, (char *) UxSelVarContext );


	/* Creation of selVarMenuPopupApplyFilter */
	selVarMenuPopupApplyFilter = XtVaCreateManagedWidget( "selVarMenuPopupApplyFilter",
			xmPushButtonWidgetClass,
			menu13,
			RES_CONVERT( XmNlabelString, "Apply Filter" ),
			NULL );
	XtAddCallback( selVarMenuPopupApplyFilter, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarMenuPopupApplyFilter,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarMenuPopupApplyFilter, (char *) UxSelVarContext );


	/* Creation of selVarMenuPopup_b3 */
	selVarMenuPopup_b3 = XtVaCreateManagedWidget( "selVarMenuPopup_b3",
			xmSeparatorWidgetClass,
			menu13,
			NULL );
	UxPutContext( selVarMenuPopup_b3, (char *) UxSelVarContext );

	UxTmp0 = SELVAR_CONFIRM ? SELVAR_CONFIRM : "OK";

	/* Creation of selVarMenuPopupSelect */
	selVarMenuPopupSelect = XtVaCreateManagedWidget( "selVarMenuPopupSelect",
			xmPushButtonWidgetClass,
			menu13,
			RES_CONVERT( XmNlabelString, UxTmp0 ),
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( selVarMenuPopupSelect, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarMenuPopupSelect,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarMenuPopupSelect, (char *) UxSelVarContext );


	/* Creation of selVarMenuPopup_b5 */
	selVarMenuPopup_b5 = XtVaCreateManagedWidget( "selVarMenuPopup_b5",
			xmSeparatorWidgetClass,
			menu13,
			NULL );
	UxPutContext( selVarMenuPopup_b5, (char *) UxSelVarContext );


	/* Creation of selVarMenuPopupQuit */
	selVarMenuPopupQuit = XtVaCreateManagedWidget( "selVarMenuPopupQuit",
			xmPushButtonWidgetClass,
			menu13,
			RES_CONVERT( XmNlabelString, CONF_DISCARD ),
			NULL );
	XtAddCallback( selVarMenuPopupQuit, XmNactivateCallback,
		(XtCallbackProc) activateCB_selVarMenuPopupQuit,
		(XtPointer) UxSelVarContext );

	UxPutContext( selVarMenuPopupQuit, (char *) UxSelVarContext );

	XtVaSetValues(selVar,
			XmNdefaultButton, selOkpb,
			NULL );

	XtVaSetValues(form42,
			XmNdefaultButton, selOkpb,
			NULL );


	XtAddCallback( selVar, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxSelVarContext);

	XtAddEventHandler(selVar, ButtonPressMask,
			False, (XtEventHandler) _UxselVarMenuPost, (XtPointer) menu13 );

	return ( selVar );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_selVar( _Uxw, _Uxp, _Uxchi, _Uxheader )
	Widget	_Uxw;
	char	*_Uxp;
	int	_Uxchi;
	char	*_Uxheader;
{
	Widget                  rtrn;
	_UxCselVar              *UxContext;
	static int		_Uxinit = 0;

	UxSelVarContext = UxContext =
		(_UxCselVar *) UxNewContext( sizeof(_UxCselVar), False );

	w = _Uxw;
	p = _Uxp;
	chi = _Uxchi;
	header = _Uxheader;

	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewInterfaceClassId();
		_Uxinit = 1;
	}

	{
		char *s;
		set_cursor (w,CLOCK);
		first_time = 1;
		first_time_b = 1;
		first_time_find = 1;
		first_time_find_b = 1;
		strcpy (label_sel,"");
		/* per malf */
		primo_giro = 1; /* primo run di questa istanza */
		conf_malf_select = -1; /* preset invalido  */
		rtrn = _Uxbuild_selVar();
		UxPutClassCode( selVar, _UxIfClassId );

		XtVaSetValues (XtParent(UxGetWidget(rtrn)), 
		         XmNtitle, header, XmNiconName, header, NULL);
		presetFilterDefaults1 (rtrn);
		set_var_default (selVarListaSistemi1);
		
		if ((chi != MALFUNZIONI) && (chi != SOMMARIO_MALF) &&
		    (chi != FUNZIONI_REMOTE) && (chi != SOMMARIO_FREM))
		   {
		   UxPopupInterface (rtrn, no_grab);
		   lettura_filtro_kks (selVarListaSistemi1);
		   lettura_filtro_tipo (selVarListaSistemi1);
		   display_lista_sistema (selVarListaSistemi1);
		   display_blocchi (selVarListaBlocchi1);
		   display_var (selVarToSelect1,mod_sel,blocco_sel,
		               kks_filter_act,tipo_filter_act);
		   }
		else
		   {
		   readConfMalf (UxGetWidget(rtrn),chi);
		/*   setConfMalfDefaults (rtrn); */
		/*   getKksFilterMalfConf (selVarToSelect1,filter_kks_malf_conf); */
		   getKksFilterMalfConf (selVarToSelect1,stringa_kks);
		   lettura_filtro_tipo (selVarListaSistemi1);
		   display_lista_sistema (selVarListaSistemi1);
		   display_blocchi (selVarListaBlocchi1);
		   UxPopupInterface (rtrn, no_grab);
		
		   switch (chi)
		      {
		      case MALFUNZIONI:
		      case SOMMARIO_MALF:  
		         printf ("SelezMalf : Display malf \n");
		         display_conf_malf (selVarToSelect1,m_component,
		                   malf_set, m_comp_type,mod_sel,blocco_sel,
		                   kks_filter_act,tipo_filter_act);
		   
		         break;
		      case FUNZIONI_REMOTE:
		      case SOMMARIO_FREM:
		         printf ("SelezMalf : Display frem \n");
		         display_conf_malf (selVarToSelect1,fr_component,
		                   frem_set, fr_comp_type,mod_sel,blocco_sel,
		                   kks_filter_act,tipo_filter_act);
		   
		         break;
		      }
		   XtVaSetValues (selOkpb,XmNsensitive, False, NULL);
		   XtVaSetValues (selVarMenuPopupSelect,XmNsensitive, False, NULL);
		   }
		
		set_cursor (w,NORMALE);
		return(UxGetWidget(rtrn));
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

