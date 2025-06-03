/**********************************************************************
*
*       C Header:               banco_globals.h
*       Subsystem:              1
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed May 21 17:34:39 1997 %
*
**********************************************************************/
/*
 * banco_globals.h
 *
 * contains all the global definition
 */
#include <stdio.h>
#ifndef NO_PARENT
#define NO_PARENT    ((swidget)0)
#endif

#ifndef MAX_LUN_COMMENTO
#define MAX_LUN_COMMENTO	40
#endif

/*
 * widgets
 */
extern swidget masterMenu;
swidget create_masterMenu();

extern swidget initialCondition;
swidget create_initialCondition();

extern swidget archiveEdit;
swidget create_archiveEdit();

extern swidget archiveSavereq;
swidget create_archiveSavereq();

extern swidget initSimulator;
swidget create_initSimulator();
/*
 * dati globali
 */
char titolo_sessione[MAX_LUN_COMMENTO];
char init_from[MAX_LUN_COMMENTO];
/*
 * dati da statistiche
 */
int stato_sim;         /* stato simulatore                */
int last_backtrack;    /* ultimo backtrack registrato(record da 1 a ...)*/
                               
int tempo_finale_impostato;	/* = 1 e' stato imposto un limite di tempo */
/********************************************************/

int exist_Widget (Widget);
int attiva_graphics();
int attiva_sommario (Widget,int);
int read_options();
int DistruggiInterfaccia (Widget );
int read_txt(Widget,char*);
int write_txt(Widget,char*);
void set_cursor (Widget,int);
void stop_timer (XtIntervalId);
int read_background_color (Widget);
int esegui_snap (int,char*);
int replay_mode (Widget, Boolean);
int carica_bt (Widget,int);
int switch_to_freeze(Widget);
Widget	create_previewSnap( int, int, unsigned char	* );
int     SD_replay (int);
int     SD_backtrack (int);
int read_lista_snap (SNTAB *);
Widget	create_selVar();
int update_title (Widget,char*);
int SD_optsave3 (int, char*, Boolean*);
int salva_sel_var (Widget, int, char *);
int newvarcr (Widget);
int del_var (Widget, Widget);
int get_label_indx (char*);
int get_val_view (Widget);
int display_view (Widget);
int applicaFiltri (Widget,int);
int display_blocchi (Widget);
int next_pos (Widget,int,Widget);
int display_lista_sistema (Widget);
int istogramma_tasks (Widget);
int attiva_timer_statistiche (Widget);
int clearResourceList (Widget );
int applyResource (Widget );
int caricaDatiRisorse (Widget );
int displayRisorse (Widget);
int attenzione (Widget,char*);
int add_message (Widget,char*, int);
int SD_savebt (int,int*,char*,char*);
int freeze_sim();
int btload_ok (Widget);
int SD_loadic (int,int*,int*);
void data(int*, int*, int*);
int	create_attenzioneDialog( Widget, unsigned char	*, int );
int SD_snapcopy (int,int*);
int SD_snappaste (int,int*);
Widget	create_icCommentReqform();
int reload_lastBtLoad (Widget, int*, int*, int);
int aggiorna_opzioni_init (Widget, OPTIONS_FLAGS *);
int aggiorna_opzioni_snap(Widget, OPTIONS_FLAGS *);
int aggiorna_opzioni_recording(Widget, OPTIONS_FLAGS *);
int aggiorna_opzioni_mffr(Widget, OPTIONS_FLAGS *);
int aggiorna_opzioni_show(Widget, OPTIONS_FLAGS *);
int lista_kks (int*,int,int*,int*,char*);
int lista_tipo (int*,int,int*,int*,FILTRO_TIPI);
int display_item_cr (Widget, int*, int, int**, int*, int);
int posiziona_lista (Widget, Boolean *, int, int*);
int lista_match_bloc (Widget,char*,Boolean *,int,int);
int add_message (Widget, char*, int);
int aggListaScenari (Widget, int);
int displayDatiMalfConfig (Widget , Widget , M_COMP_TYPE *, MALF_SET *, M_COMPONENT *);
int update_menu ();
int existFile (char *);
void ApplyResDb (XrmDatabase , XrmDatabase *, Display *, int );
int SD_stato (int, RICHIESTA_STAT *);
void costruisci_var (char**, VARIABILI **, int*);
int simulatorShutdown (Widget );
int clear_lista (Widget );










