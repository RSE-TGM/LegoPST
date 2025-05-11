/**********************************************************************
*
*       C Header:               monit.h
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Fri Apr 12 16:16:32 1996 %
*
**********************************************************************/
/*
   modulo monit.h
   tipo 
   release 5.4
   data 1/24/96
   reserved @(#)monit.h	5.4
*/
/*
  monit.h
    contiene le definizioni di tipi utilizzati nel programma 
    monit
    contiene le definizioni di costanti di uso interno a monit
*/



#define NUM_COLORI 12

/* 
 struttura contenente i valori che devono essere aggiornati 
 periodicamente
*/
typedef struct valori_agg {
		float tempo_sim;        /* tempo di simulazione */
		char  tempo_sim_a;
		float tempo_reale;      /* tempo reale */
		char  tempo_reale_a;
		float timescaling_sim;     /* velocita' di simulazione */
		char  timescaling_sim_a;
		float stepscaling_sim;     /* step' di simulazione */
		char  stepscaling_sim_a;
		float endtime_sim;     /* tempo finale' di simulazione */
		char  endtime_sim_a;
		float tempo_ritardo;    /* tempo di ritardo */
		char  tempo_ritardo_a;
		int   grandezze_mem;    /* num. variabili selezionate
                                           per la grafica */
		char  grandezze_mem_a;
		float tempo_simMB;      /* tempo di simulaz./MB  */
		char  tempo_simMB_a;
		int passo_reg;        /* passo di registrazione per 
                                           la grafica  */
		char  passo_reg_a;
		int spazio_dsk;     /* spazio libero su disco */
		char  spazio_dsk_a;
		int stato_sked;	    /* stato dello schedulatore */
		char stato_sked_a;
		int stato_snapshot; /* stato proc snapshot */
		char stato_snapshot_a;
		int stato_prepf22; /* stato proc prepf22 */
		char stato_prepf22_a;
		int stato_task[MAX_MODEL]; /* stato task */
		char stato_task_a[MAX_MODEL];
#if defined BANCO_MANOVRA
                int stato_agg_manovra[MAX_SLAVE+2];
                char stato_agg_manovra_a[MAX_SLAVE+2];
                int stato_agg_manovra_pert[MAX_SLAVE+1];
                char stato_agg_manovra_pert_a[MAX_SLAVE+1];
                int stato_agg_manovra_stato;
                char stato_agg_manovra_stato_a;
#endif
#if defined SCADA
                int stato_agg_scada;
                char stato_agg_scada_a;
#endif
#if defined BACKTRACK
                int modo_bt;
                char modo_bt_a;
                int direzione_bt;
                char direzione_bt_a;
                int last_backtrack;
                char last_reg_bt_a;
                int last_load_bt;
                char last_load_bt_a;
                int first_session_bt;
                char first_session_bt_a;
                int last_session_bt;
                char last_session_bt_a;
                int step_selezionato_bt;
                char step_selezionato_bt_a;
#endif
		} VALORI_AGG;


#define TM_REFRESH  1000   /* tempo di refresh dei valori visualizzati
                              in millisecondi  */
#define TM_REF_STAT 5000   /* tempo di refresh dei valori visualizzati
                              nella statistica in millisecondi  */
#define TS_SLOW_R   1    /* tempo di refresh in secondi per i valori
                              ad aggiornamento lento  */
#define MAX_MESSAGGI 100   /* massimo numero di messaggi registrabili
							*/

#define MAX_LUNG_MESSAGGIO 80 /* massima lunghezza di un messaggio
								*/
struct messaggi_st 
	{
	char mess[MAX_MESSAGGI][MAX_LUNG_MESSAGGIO];
	};
typedef struct messaggi_st     MESSAGGI;

static char *stato_sked_label[]={"STOP",
			 "RUN",
			 "FREEZE",
			 "BACKTRACK",
			 "REPLAY",
			 "ERROR"};

#if defined BACKTRACK

#define BT_MANUALE          0
#define BT_AUTOMATICO       1
#define BT_INDIETRO         0
#define BT_AVANTI           1
#define TIMER_AUTO_BT       20  /* secondi tra 2 caricamenti */
#define INTERVALLO_BACKTRACK 120  /* secondi tra 2 registrazioni  */
static char *modo_backtrack[]=
                {"manuale",
                 "automatico"};

static char *direzione_backtrack[]=
                {"indietro",
                 "avanti"};

#endif

#if defined LEGOCAD
struct stato_monit_st
        {
        float timescaling;
        float endtime;
        float stepscaling;
        int stepcr;
        };
typedef struct stato_monit_st STATO_MONIT;
#endif

void testata(char *, char *);
int     SD_aggancio (int , char *);
int ControlParam(int );
void init_colors ();
void distruggi_voci_inutili();
int     SD_inizializza (int);
int vis_messaggio (char*,int);
int sommari_to_snap();
int     SD_savebt (int, int*, char*, char*);
int     SD_run (int);
int     SD_freeze (int);
int     SD_saveic (int, int*, char*, char*);
int     SD_lbtreg (int);
int     SD_backtrack (int);
int     SD_clear (int);
int     SD_replay (int);
int     SD_stop (int);
int     SD_sgancio (int);
int     SD_timescaling (int, float*);
int     SD_stepcr (int, int*);
int     SD_newvarcr (int);
int     SD_stepscaling (int, float*);
int     SD_goup ();
int     SD_listaci (int, char*, int);
int     SD_lsnapreg (int);
int     SD_lsnapload ( int);  


void   mo_var(Widget);
void  mo_var_sel(Widget);
void mo_blocchi(Widget);
void mo_modelli(Widget);
void carica_snapshot(Widget);
void elenca_snapshot(Widget);
void elenca_pert(Widget,Widget);
void elenca_modpert();
void mo_pert_sel(Widget);
void elenca_staz(Widget);
int attiva_graphics();
void about(Widget);
int monit_aggiorna(VALORI_AGG *);
void bt_al_centro();
int     SD_lbtload (int);
void limiti_sessione();
int     SD_loadbt ();
int snap_to_sommari();
int cambia_stato_menu ();
Boolean RtCheckPointer(void *);
int SD_editbt ();
int SD_editpert ();


