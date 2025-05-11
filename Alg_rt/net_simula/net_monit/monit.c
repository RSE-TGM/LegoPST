/**********************************************************************
*
*       C Source:               monit.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed Mar 30 18:53:31 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: monit.c-15 %  (%full_filespec: monit.c-15:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)monit.c	5.6\t3/14/96";
/*
   modulo monit.c
   tipo 
   release 5.6
   data 3/14/96
   reserved @(#)monit.c	5.6
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>
#include <Mrm/MrmPublic.h>
#include <Xm/Text.h>
#include <Xm/List.h>
#include <Xm/DrawingA.h>
#include <Xm/RowColumn.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <sys/timeb.h>
#endif
#if defined VMS
#include <stdlib.h>
# include "vmsipc.h"
#endif
#include "UxXt.h"
#include "monituil.h"
#include "sim_param.h"
#include "sim_types.h"
#include "sim_ipc.h"
#include "monit.h"
#include "sked.h"
#include "monit_menu.h"
#include "libutilx.h"
#include "dispatcher.h"
#if defined BANCO_MANOVRA
#include "agg_manovra.h"
#endif
#include <Rt/RtMemory.h>
#include <sqlite3.h>

#define ATTESA_SKED 20	/* tempo massimo di attesa dei messaggi da * NET_SKED */


/* procedure inserite */
void load_font ();
void init_gc ();
void s_error ();
void monit_aggancia ();
void refresh_proc ();
void dialog_proc ();


/* Dichiarazioni callback function */

void help_proc ();
void create_proc ();
void initialize_proc ();
void run_proc ();
void freeze_proc ();
void backtrack_proc ();
void replay_proc ();
void clear_proc ();
void set_ftime_proc ();
void stop_proc ();
void quit_proc ();
void timescaling_proc ();
void stepscaling_proc ();
void stepcr_proc ();
void confcr_proc ();
void confcrall_proc ();
void load_proc ();
void save_proc ();
void edit_proc ();
void statistics_proc ();
void procstat_proc ();
void input_proc ();
void inputstat_proc ();
void editpert_proc ();
void inputstat_refresh ();
void editpert_refresh ();
void go_proc ();
void graphics_proc ();
void about_proc ();
void tab_backtrack ();
void tab_snapshot ();
void tab_perturba ();
void clear_list_proc();
void clear_list_mouse();
extern  void monit_stat ();
extern  void seleziona_var ();
extern  void paste_var_selez ();
extern  void seleziona_activate ();
extern  void seleziona_file ();
extern  void salva_file ();
extern  void seleziona_snapshot ();
extern  void snapshot_activate ();
#if defined BACKTRACK
extern  void seleziona_backtrack ();
extern  void carica_backtrack ();
extern  void backtrack_activate ();
#endif
extern  void mo_per_sel (Widget);
extern  void seleziona_pert ();
extern  void seleziona_pert_activate ();
extern  void cerca_ingr_avanti ();
extern  void cerca_ingr_indietro ();
extern  void activate_staz ();
extern  void seleziona_staz ();
extern  void seleziona_inputstat();
extern  void seleziona_inputstat_activate();
extern  void seleziona_editpert();
extern  void seleziona_editpert_activate();
void	data_proc();
extern void seleziona_data();
extern void seleziona_editdata_activate();
#if defined MFFR
extern  void malf_proc();
extern  void frem_proc();
extern  void select_malf_proc();
extern int clear_snap_sked();
extern int clear_sommario_mf();
extern int clear_sommario_fr();
SNAP_SKED sommari;
#else
void malf_proc();
void frem_proc();
#endif
#if defined LEGOCAD
int save_stato_monit();
int load_stato_monit();
#endif

void refresh_stat();

int  numero_messaggi = 0;
FLAG_SEL flag_sel;
int     nu_var_sel;

/* pid processo aing */
extern int  pid_aing;
extern int  stato_aing;

/* pid processo staz */
extern int  pid_staz;
extern int  stato_staz;

/* coda di messaggi con sked */
extern int  id_msg_monit;

/* identificatore sh_var */
extern int  id_sh;

/* pid processo graphics */
int pid_grafi=-1;
int stato_grafi=-1;

extern char *ind_sh_top;/* puntatore inizio shared memory sh_var */

extern VARIABILI *variabili;    /* database delle variabili  */
extern int tot_variabili;       /* numero totale delle variabili      */

SNTAB * snapshot;               /* puntatore alla tabella snapshot  */

int BacktrackImmediato = 0;     /* Se 1 al RUN deve scrivere subito un bktk */

/* lista componenti disallineati */
#if defined BANCO_MANOVRA
int run_abilitato=1;        /* se = 1 e' abilitato */
int check_in_corso=0;       /* se = 1 in corso     */
int primo_check = 0;
ALLINEAMENTO lista_comp;
extern  void override_allineamento();
#endif


#if defined BACKTRACK
/* area descrizioni backtrack */
BKTAB * backtrack;	/* puntatore alla tabella backtrack  */
/*int punt_rec_backtrack[MAX_BACK_TRACK+1]; vecchia allocazione */
int *punt_rec_backtrack;
extern int bt_avanti();
extern int bt_indietro();
extern int bt_in_automatico();
extern int bt_in_manuale();
extern int prossimo_step();
#endif

/* area perturbazioni */
TIPO_PERT * perturbazioni;	/* puntatore alla tabella perturbazioni  */
TIPO_PERT * perturbazioni_old;	/* puntatore alla tabella perturbazioni  */

/* area nomi variabili degli slot degli snapshot */
char var_slot[MAX_VAL_SNAP][MAX_LUN_NOME_VAR];


/* massimo tempo di simulazione, superato tale tempo, se 
   tempo_finale_impostato ==1,  viene inviato un messaggio di FREEZE  */
float tempo_finale;
int tempo_finale_impostato;

/* variabili del simulatore */

char   *nomi_colori[] =
{
    "red", "green", "yellow", "red", "red" , "light_gray" ,"blu_sfondo",
        "light_blue", "Cyan", "Aquamarine","light_slate_gray","sky_blue"
};
Pixel pix_val[NUM_COLORI];

char   *getenv ();

/* struttura contenete i valori visualizzati */
VALORI_AGG val_agg;	/* struttura contenente i valori che * richiedono aggiornamento periodico  */
extern char *nome_file_selezione;/* nome del file contenente le * variabili da registrare per la * grafica */

/* variabili per il timer di refresh */
XtIntervalId timer_id, timer_stat, timer_refpert, timer_refeditpert;

/* variabili per la grafica   */
XFontStruct * font_info;
/******************************
 * font per mf e fr
 */
XFontStruct *font_mffr;
XmFontList  fontlist_mffr;
/******************************/

char   *colori[] =
{
    "black", "white", "red", "light gray", "cyan"
};

XColor excolor[12];
int     font_ascent;
int     font_height;
int     font_width;
GC gc[5];
Display * display;
int     screen_num;
Widget toplevel_widget;	/* Root widget ID of our */
Widget main_window_widget;/* Root widget ID of main */
/* MRM fetch application. */
Widget widget_array[MAX_WIDGETS];/* Place to keep all other */
/* widget IDs */


/* variabili relative al resource manager */

static  MrmHierarchy s_RMHierarchy;/* MRM database hierarchy ID */
static  MrmType  *dummy_class;/* and class variable. */
static char *filename_uid =/* DwtDrm.heirachy file list. */
            "net_monit.uid";

static int  db_filename_num = 1;

/* Variabili per registrazione delle Callback function */
static  MrmRegisterArg reglist[] =
{
    {
	"create_proc", (caddr_t) create_proc
    },
    {
	"initialize_proc", (caddr_t) initialize_proc
    }      ,
    {
	        "help_proc", (caddr_t) help_proc
    }      ,
    {
	        "run_proc", (caddr_t) run_proc
    }      ,
    {
	        "freeze_proc", (caddr_t) freeze_proc
    }      ,
    {
	        "backtrack_proc", (caddr_t) backtrack_proc
    }      ,
    {
	        "replay_proc", (caddr_t) replay_proc
    }      ,
    {
	        "clear_proc", (caddr_t) clear_proc
    }      ,
    {
	        "set_ftime_proc", (caddr_t) set_ftime_proc
    }      ,
    {
	        "stop_proc", (caddr_t) stop_proc
    }      ,
    {
	        "quit_proc", (caddr_t) quit_proc
    }      ,
    {
	        "timescaling_proc", (caddr_t) timescaling_proc
    }      ,
    {
	        "stepscaling_proc", (caddr_t) stepscaling_proc
    }      ,
    {
	        "stepcr_proc", (caddr_t) stepcr_proc
    }      ,
    {
	        "confcr_proc", (caddr_t) confcr_proc
    }      ,
    {
	        "confcrall_proc", (caddr_t) confcrall_proc
    }      ,
    {
	        "load_proc", (caddr_t) load_proc
    }      ,
    {
	        "save_proc", (caddr_t) save_proc
    }      ,
    {
	        "edit_proc", (caddr_t) edit_proc
    }      ,
    {
	        "statistics_proc", (caddr_t) statistics_proc
    }      ,
    {		
		"procstat_proc", (caddr_t) procstat_proc
    }      ,
    {
	        "input_proc", (caddr_t) input_proc
    }      ,
    {
	        "inputstat_proc", (caddr_t) inputstat_proc
    }      ,
    {
	        "seleziona_inputstat", (caddr_t) seleziona_inputstat
    }      ,
    {
	        "seleziona_inputstat_activate", (caddr_t) seleziona_inputstat_activate
    }      ,
    {
	        "editpert_proc", (caddr_t) editpert_proc
    }      ,
    {
	        "seleziona_editpert", (caddr_t) seleziona_editpert
    }      ,
    {
	        "seleziona_editpert_activate", (caddr_t) seleziona_editpert_activate
    }      ,
    {
	        "go_proc", (caddr_t) go_proc
    }      ,
    {
	        "graphics_proc", (caddr_t) graphics_proc
    }      ,
    {
	        "about_proc", (caddr_t) about_proc
    }      ,
    {
	        "monit_stat", (caddr_t) monit_stat
    }      ,
    {
	        "seleziona_var", (caddr_t) seleziona_var
    }      ,
    {
	        "seleziona_activate", (caddr_t) seleziona_activate
    }      ,
    {
	        "seleziona_file", (caddr_t) seleziona_file
    }      ,
    {
	        "salva_file", (caddr_t) salva_file
    }      ,
    {
	        "seleziona_snapshot", (caddr_t) seleziona_snapshot
    }      ,
#if defined BACKTRACK
    {
                "seleziona_backtrack", (caddr_t) seleziona_backtrack
    }      ,
#endif
    {
	        "snapshot_activate", (caddr_t) snapshot_activate
    }      ,
#if defined BACKTRACK
    {
                "backtrack_activate", (caddr_t) backtrack_activate
    }      ,
#endif
    {
	        "seleziona_pert", (caddr_t) seleziona_pert
    }      ,
    {
	        "seleziona_pert_activate", (caddr_t) seleziona_pert_activate
    }      ,
    {
	        "seleziona_staz", (caddr_t) seleziona_staz
    }      ,
    {
	        "activate_staz", (caddr_t) activate_staz
    } 	   ,
    {
	        "clear_list_proc", (caddr_t) clear_list_proc
    },
    {
	        "data_proc", (caddr_t) data_proc
    },
    {
	        "seleziona_data", (caddr_t) seleziona_data
    },
    {
	        "seleziona_editdata_activate", (caddr_t) seleziona_editdata_activate
    }       ,
#if defined BANCO_MANOVRA
    {
                 "override_allineamento", (caddr_t) override_allineamento
    }       ,
#endif
    {
           "malf_proc", (caddr_t) malf_proc
    }       ,
#if defined MFFR
    {
           "select_malf_proc", (caddr_t) select_malf_proc
    }       ,
#endif
    {
           "frem_proc", (caddr_t) frem_proc
    },
    {
           "paste_var_selez", (caddr_t) paste_var_selez
    },
};

static int  reglist_num = (sizeof reglist / sizeof reglist[0]);

int settato_pointer=0;

/*
   Definizioni per la finestra di dialogo degli errori nei parametri di
   attivazione rispetto alle registazioni nei file delle registrazioni attive.
*/
#define CONTINUE   1
#define SHOW       2
#define EXIT       3
void nuovi_parametri();
Widget change_param;
Widget Informa=NULL;
Widget InformaSHM=NULL;
static Elenco_callback parametri_errati = {
           {"Continue",             nuovi_parametri, CONTINUE  },
           {"Show differences",     nuovi_parametri, SHOW      },
           {"Exit" ,                nuovi_parametri, EXIT      }
                                          };
int err_startup=0;
STATO_CR stato_cr;

#if defined LEGOCAD
STATO_MONIT stato_monit;
#endif

#if defined BACKTRACK
float intervallo_backtrack=0.0;
static float old_tempo_sim;
time_t timer_auto_bt;
time_t old_time_bt;
int last_backtrack=0;        /* indice del record nel file   */
int modo_bt=BT_MANUALE;
int direzione_bt=BT_INDIETRO;
int last_load_bt;
int first_session_bt;
int last_session_bt;
int step_selezionato;      /* indice di lista del record selezionato */
int num_bt_list;           /* numero di record nella lista */
/*int sessioni_bt[MAX_BACK_TRACK];  vecchia allocazione */
int *sessioni_bt;  /* indice delle sessioni registrate */
int backtrack_selezionato = 0;
#endif

struct timeb tempo;       /* tempo di orologio */

char   *path_uid;


int _MAX_SNAP_SHOT;
int _MAX_BACK_TRACK;
int _MAX_CAMPIONI;
int _NUM_VAR;
int _MAX_PERTUR;
int _SPARE_SNAP;
int _PERT_CLEAR;
int errsked=0,errdisp=0,errshm=0,errmonit=0,errtotali=0;

sqlite3 *db;

Widget Conf (Widget,char *,Elenco_callback);


int main (argc, argv)
unsigned int    argc;	/* Command line argument count */
char   *argv[];		/* Pointers to command line args. */
{
char    name_uid[FILENAME_MAX+1];
int i;
char *errori;
int app;
int ret;

   for(i=0;i<argc-1;i++)
      {
      if(strcmp(argv[i],ARGV_N_SNAP)==0)
                  _MAX_SNAP_SHOT  = atoi(argv[i+1]);
      if(strcmp(argv[i],ARGV_N_BKTK)==0)
                  _MAX_BACK_TRACK = atoi(argv[i+1]);
      if(strcmp(argv[i],ARGV_N_CAMP_CR)==0)
                  _MAX_CAMPIONI   = atoi(argv[i+1]);
      if(strcmp(argv[i],ARGV_N_VAR_CR)==0)
                  _NUM_VAR        = atoi(argv[i+1]);
      if(strcmp(argv[i],ARGV_N_PERT_ACTIVE)==0)
                  _MAX_PERTUR     = atoi(argv[i+1]);
      if(strcmp(argv[i],ARGV_N_SPARE)==0)
                  _SPARE_SNAP     = atoi(argv[i+1]);
      if(strcmp(argv[i],ARGV_N_PERTCL)==0)
                  _PERT_CLEAR     = atoi(argv[i+1]);
      }

/*  Allocazioni */
    punt_rec_backtrack = (int*)calloc(_MAX_BACK_TRACK+1,sizeof(int));
    sessioni_bt        = (int*)calloc(_MAX_BACK_TRACK  ,sizeof(int));

    name_uid[0] = 0;

    MrmInitialize ();		/* Initialize MRM before */
/* the X Toolkit. */

/* redirezione dell'output  e stampa versione */
testata("net_monit",SccsID);

/* Initialize the X Toolkit. We get back a top level shell widget. */

    toplevel_widget = XtInitialize ("monit",
/* Main window banner text. */
	    "Monit",		/* Root class name. */
	    NULL,		/* No option list. */
	    0,			/* Number of options. */
	    &argc,		/* Address of argc */
	    argv);		/* argv */

/* aggancio shared memory */

    monit_aggancia ();


/*  Aggancio al dispatcher */
    errori = (char*)calloc(ERRSTARTUP*sizeof(int),sizeof(char));
    ret = SD_aggancio (MONIT,errori);
    if( ret < 0 )
        fprintf(stderr,"Error Acknowledge AGGANCIO not received");
    errmonit = ControlParam(MONIT);
    i=0;
    memcpy(&errsked,&errori[i*sizeof(int)],sizeof(int));i++;
    memcpy(&errdisp,&errori[i*sizeof(int)],sizeof(int));i++;
    memcpy(&errshm, &errori[i*sizeof(int)],sizeof(int));i++;
    errtotali = errsked+errdisp+errshm+errmonit;
    if ( errtotali!=0 )
      printf("Errori in fase di Startup (sk=%d disp=%d monit=%d shm=%d)\n",
              errsked,errdisp,errmonit,errshm);
/*
    for(i=0;i<ERRSTARTUP;i++)
       {
       memcpy(&app,&errori[i*sizeof(int)],sizeof(int));
       errtotali += app;
       }
*/
    if( errtotali > 0 )
        err_startup = 1;


    free(errori);

#if defined MFFR
/*
 * pulisce i sommari MF e  FR
 */
        clear_snap_sked();   /* pulisce il buffer per lo snapshot        */
        clear_sommario_mf(); /* pulisce la struttura per la grafica      */
        clear_sommario_fr(); /* pulisce la struttura per la grafica      */
#endif
/*
 * carica i font e definisce i cahrset per mf e fr
 */
	if ((font_mffr = XLoadQueryFont (XtDisplay(toplevel_widget),
                             /*    "variable")) == NULL)    */
                                 "8x13bold")) == NULL)
				printf ("FONT_MFFR non trovato\n");
				else
				fontlist_mffr = XmFontListCreate (font_mffr, "XmSTRING_DEFAULT_CHARSET");
/*****************************************************/
#ifndef SCO_UNIX
    sigblock (1 << (SIGTERM - 1));/* blocca il segnale SIGTERM */
#endif


    display = XtDisplay (toplevel_widget);
    screen_num = DefaultScreen (display);
/* Open the UID files (the output of the UIL compiler) in the hierarchy     */

    path_uid = getenv ("LEGORT_UID");
#if defined UNIX
    strcpy (name_uid, path_uid);
    strcat (name_uid, "/");
    strcat (name_uid, filename_uid);
#endif
#if defined VMS
    strncpy (name_uid, path_uid, strlen (path_uid) - 1);
    sprintf (name_uid, "%s]%s", name_uid, filename_uid);
#endif
    path_uid = name_uid;
    if (MrmOpenHierarchy (db_filename_num,/* Number of files. */
		&path_uid,	/* Array of file names.  */
		NULL,		/* Default OS extenstion. */
		&s_RMHierarchy)	/* Pointer to returned MRM ID */
	    != MrmSUCCESS)
	s_error ("can't open hierarchy");

/* registrazione nomi funzioni di callback  */

    MrmRegisterNames (reglist, reglist_num);

    if (MrmFetchWidget (s_RMHierarchy, "S_MAIN_WINDOW", toplevel_widget,
		&main_window_widget, (MrmType *)&dummy_class) != MrmSUCCESS)
	s_error ("can't fetch main window");
    init_colors ();
/* Manage the main part and realize everything.  The interface comes on the
 * display now. */
    XtManageChild (main_window_widget);
    XtRealizeWidget (toplevel_widget);

/* inizializza i GC per la graficia */
    init_gc ();


/* setta il timer per la funzione di refresh dei valori visualizzati */
    timer_id = XtAppAddTimeOut (
	    XtWidgetToApplicationContext (main_window_widget),
	    (unsigned long) TM_REFRESH, refresh_proc, NULL);


#if defined MFFR
        malf_proc();
        frem_proc();
#endif
    if ( err_startup == 1 )
            {
            change_param = (Widget)Conf (main_window_widget,
                                         STARTUP_ERROR,
                                         parametri_errati); 
            if ( (errshm!=0)  )
               {
char msg[100];
Dialog_geometry geom;
               strcpy(msg,"Shared memory already exist. Choose Exit !!!\n");
               geom.default_pos = 1;
               geom.height      = 200;
               InformaSHM = (Widget)information (main_window_widget, msg,
                                           CREA|MAPPA, geom); 
               }
            }

    XtMainLoop ();
}

void init_colors ()
{
int     i;
    for (i = 0; i < NUM_COLORI; i++)
    {
	if (MrmFetchColorLiteral (s_RMHierarchy, nomi_colori[i],
		    display, (Colormap)NULL, &pix_val[i]) != MrmSUCCESS)
	    fprintf (stderr," monit : errore FETCH colori\n");
    }
}

void
init_gc ()
{
    XGCValues values;
    XColor color;
    Colormap default_cmap;
    Pixel pixel;

unsigned int    line_width = 0;
static int  line_style[] =
{
    LineSolid, LineSolid, LineSolid, LineSolid, LineSolid
};
int     cap_style = CapRound;
int     join_style = JoinRound;
static int  dash_offset[] =
{
    0, 0, 0, 5
};
static char dash_list[4][2] =
{
    {
	0, 0
    },
    {
	0, 0
    },
    {
	5, 5
    },
    {
	5, 5
    }
};
int     list_lenght = 2;
int     i;
unsigned long   valuemask = GCForeground | GCBackground | GCLineWidth
|               GCLineStyle | GCCapStyle | GCJoinStyle |
                GCFunction;

    load_font (&font_info);
    font_ascent = font_info -> ascent;
    font_height = font_info -> ascent + font_info -> descent;
    font_width = font_info -> max_bounds.width;
    values.function = GXcopy;
    values.line_width = line_width;
    values.cap_style = cap_style;
    values.join_style = join_style;
    values.background = WhitePixel (display, screen_num);
    default_cmap = DefaultColormap (display, screen_num);

    for (i = 0; i < 5; i++)
    {
	XAllocNamedColor (display,
		default_cmap,
		colori[i], &color, &excolor[i]);
    }
    for (i = 0; i < 5; i++)
    {
	values.line_style = line_style[i];
	values.foreground = excolor[i].pixel;
	values.background = WhitePixel (display, screen_num);

	gc[i] = XCreateGC (display, RootWindow (display, screen_num), valuemask,
		&values);
	XSetFont (display, gc[i], font_info -> fid);
    }
}


void
load_font (font_info)
XFontStruct ** font_info;
{
/*char   *font_name = "fixed"; */
/*char   *font_name = "-misc-fixed-normal-r-semicondensed--10-100-75-75-c-50-iso8859-1"; */
char   *font_name = "8x13bold";
/* Carica il font ottenendo la descrizione del font stesso */
    if ((*font_info = XLoadQueryFont (display, font_name)) == NULL)
    {
	fprintf (stderr,"Cannot open font 8x13\n");
		if ((*font_info = XLoadQueryFont (display,"fixed")) == NULL)
				{
				fprintf (stderr,"Cannot open font fixed\n");
				exit (0);
				}
    }
}

/* s_error per errori FATALI : stampa un messaggio ed esce dall'applicazione */

void
s_error (problem_string)
char   *problem_string;
{
    fprintf (stderr,"%s\n", problem_string);
    exit (0);
}

/* create_proc funzione richiamata all'atto della creazione dei widget */
void
create_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
int     i;


widget_array[widget_num] = w;
/*
	aggiunge callback per cancellazione lista
*/
if(widget_num == k_messaggi)
  XtAddEventHandler(w,ButtonPressMask,False,
		clear_list_mouse,(XtPointer)k_clear_list_popup);
}

void distruggi_voci_inutili()
{
/*
	Verifica a seconda della versione quali tasti
	devono essere eventualmente eliminati
*/
#ifndef LEGOCAD
XtDestroyWidget( widget_array[k_editpert] );
widget_array[k_editpert]=0;
#endif
#ifndef BACKTRACK
XtDestroyWidget( widget_array[k_backtrack] );
widget_array[k_backtrack]=0;
XtDestroyWidget( widget_array[k_azzera_cascade] );
widget_array[k_clear]=0;
widget_array[k_azzera_cascade]=0;
#endif
#ifndef MFFR
XtDestroyWidget( widget_array[k_malf_menu_entry] );
widget_array[k_malf_menu_entry]=0;
#endif
XtDestroyWidget( widget_array[k_conflg] );
widget_array[k_conflg]=0;
}

void initialize_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
char app_char[100];
char app_var[MAX_LUN_NOME_VAR];
STATISTICA_SKED statistica_sked;


    def_cursore (main_window_widget, OROLOGIO);
    settato_pointer=1;

    if(SD_inizializza (MONIT)>0)
        {
        vis_messaggio ("Acknowledge INITIALIZE received",0);
        if ( Informa != NULL )
           XtDestroyWidget(Informa);
	}
    else
        {
        vis_messaggio ("Error acknowledge INITIALIZE not received",1);
        undef_cursore (main_window_widget);
        settato_pointer=0;
	}


/*
 setta il timer per la funzione di refresh dei valori visualizzati
*/
    XtRemoveTimeOut (timer_id);
    timer_id = XtAppAddTimeOut (
            XtWidgetToApplicationContext (main_window_widget),
            (unsigned long) TM_REFRESH, refresh_proc, NULL);
}

void help_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;

    switch (widget_num)
    {
        case k_bt_help_load:
            {
                                printf ("chiesto help load bt\n");
                                break;
                                }
        case k_bt_help_ok:
            {
            printf ("chiesto help ok bt\n");
            break;
            }
        case k_bt_help_exit:
            {
            printf ("chiesto help exit bt\n");
            break;
            }
        case k_bt_help_procedure:
            {
            printf ("chiesto help procedure bt\n");
            break;
            }
                        default:
                                printf ("help non trattato\n");
         }
}




/* 
     Manda il comando di run allo sked e se e' il caso ( a seguito
     del caricamento di una condizione iniziale esegue un backtrack.
 */
void run_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
MSG_SKDIS messaggio;


    def_cursore (main_window_widget, OROLOGIO);
/******************************************************
 *  Per sincronizzare al medesimo passo di tempo la fase di
 *  savebt si inserisce qui.
 *****************************************************/
     if ( BacktrackImmediato==1 )
        {
        BacktrackImmediato = 0;
        intervallo_backtrack = (float)INTERVALLO_BACKTRACK;
        ++last_backtrack;
        if (last_backtrack>_MAX_BACK_TRACK)
           last_backtrack=1;
#if defined MFFR
        sommari_to_snap();
        if( SD_savebt(MONIT,&last_backtrack,"BT SNAP after LOADIC",
                      (char*)&sommari)>0 )
#else
        if( SD_savebt(MONIT,&last_backtrack,"BT SNAP after LOADIC",
                      NULL)>0 )
#endif
            vis_messaggio("Command immediately SAVE B.T. send",0);
        else
            vis_messaggio("Error Acknowledge immediately SAVE B.T. not received",1);
        }

/*  Invio il comando di run */
    if(SD_run (MONIT) > 0 )
        vis_messaggio ("Command RUN send",0);
    else
        vis_messaggio ("Error acknowledge RUN not received",1);

    undef_cursore (main_window_widget);
}



 
void freeze_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
    MSG_SKDIS messaggio;

    def_cursore (main_window_widget, OROLOGIO);
/* manda il comando di freeze allo sked */
    if(SD_freeze (MONIT)>0)
        vis_messaggio ("Command FREEZE send",0);
    else
        vis_messaggio ("Error acknowledge FREEZE not received",1);

    undef_cursore (main_window_widget);
}

void backtrack_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
FILE *fp;
int     widget_num = *tag;
char app_char[100];
#if defined BACKTRACK
int i;
char descrizione[80];
/*
 * procedura per l' entrata in modo backtrack (se ci sono records):
 *  1)  snapshot su ci 60
 *  2)  cambio di stato
 *  3)  caricamento ultimo backtrack (eliminato)
 *  4)  direzione indietro
 *  5)  modo manuale
 */

      def_cursore (toplevel_widget, OROLOGIO);

      i = _MAX_SNAP_SHOT;
/*    strcpy (descrizione,"AUTO");  */
      vis_messaggio("Command SAVE I.C. send",0);

#if defined MFFR
      sommari_to_snap();
      if(SD_saveic(MONIT,&i,"SNAP AUTOMATICO",(char*)&sommari)>0)
#else
      if(SD_saveic(MONIT,&i,"SNAP AUTOMATICO",NULL)>0)
#endif
         vis_messaggio("Acknowledge SAVE I.C. received",0);
      else
         vis_messaggio("Error Acknowledge SAVE I.C. not received",1);

/*   Per essere definito last_bktk_save deve essere maggiore di zero */
     if( (stato_cr.last_bktk_save=SD_lbtreg(MONIT)) >0 )
        {
        last_backtrack=stato_cr.last_bktk_save;
/*      printf("\t\t backtrack_proc con last_backtrack=%d\n",
               last_backtrack);*/
        sprintf(app_char, "Last backtrack saved -> %d",stato_cr.last_bktk_save);
        vis_messaggio(app_char,0);

        if(SD_backtrack (MONIT)>0)
            vis_messaggio ("Command BACKTRACK send",0);
        else
            vis_messaggio ("Error acknowledge BACKTRACK not received",1);

/*      Accesso alla tabella dei backtrack */
        tab_backtrack();

        sprintf (app_char,"backtrack_dialog");
        dialog_proc(k_backtrack_dialog,app_char);
/*      Legge la lista dei backtrack */
        carica_backtrack (widget_array[k_caricamento_lista_bt]);
        bt_in_manuale();
        bt_indietro();
        }
     else
        vis_messaggio ("Last backtrack saved NOT DEFINED",0);
        

   undef_cursore (toplevel_widget);

#endif

}
                                                             

void
clear_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
int     ret;

/* manda il comando di clear allo sked */
    if( (ret=SD_clear (MONIT)) >0 )
	{
#if defined BACKTRACK
        BacktrackImmediato = 1;
	intervallo_backtrack = 0.;
        old_tempo_sim = 0.;
        last_backtrack=0;
#endif
        vis_messaggio ("Command CLEAR send",0);
	}
    else
        vis_messaggio ("Error acknowledge CLEAR not received",1);

    undef_cursore (main_window_widget);
}


void replay_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
#if defined REPLAY
int     widget_num = *tag;

/* manda il comando di clear allo sked */

     switch (widget_num)
        {
        case k_replay:
             if(SD_replay (MONIT)>0)
                 vis_messaggio ("Command REPLAY send",0);
             else
                 vis_messaggio ("Error acknowledge REPLAY not received",1);
             break;

         case k_stop_replay:
              if(SD_backtrack (MONIT)>0)
                 vis_messaggio ("Command BACKTRACK send (stop_replay)",0);
              else
                 vis_messaggio ("Error acknowledge BACKTRACK not received",1);
              break;
        }
#endif
}


void
set_ftime_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
char    app[30];
char   *testo;
char riga[80];

    switch (widget_num)
    {
        case k_set_ftime:
            {
                sprintf (app, "set_ftime_dialog");
                dialog_proc (k_set_ftime_dialog, app);
                break;
            }
        case k_set_ftime_dialog_ok:
            {
                testo = XmTextGetString
                    (widget_array[k_set_ftime_dialog_text]);
                strcpy (app, testo);
                XtFree (testo);
		if (atof (app) >= val_agg.tempo_sim) 
                {
                    tempo_finale = atof (app);
		    tempo_finale_impostato=1;
		    sprintf(riga,
			"Max simulation time set = %f",tempo_finale);
        	  vis_messaggio ( riga,0);
		  val_agg.endtime_sim=tempo_finale;
		  val_agg.endtime_sim_a=1; 
#if defined LEGOCAD
		      /*
			Salvo sul file stato_monit il nuovo tempo finale
		      */
		      stato_monit.endtime= tempo_finale;	
		      save_stato_monit(&stato_monit);
#endif
                }
		else
		{
        	  vis_messaggio ( "Warning: Max simulation time not set",1);
		}
                break;
            }
        case k_set_ftime_dialog_annulla:
            {
                break;
            }
    }
}

void
stop_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
    MSG_SKDIS messaggio;
int i;

    def_cursore (main_window_widget, OROLOGIO);
#if defined LASTSNAP
/* salva uno snapshot in posizione MAX_SNAP_SHOT con il nome Stop-Snapshot */
   i=_MAX_SNAP_SHOT;
#if defined LEGOCAD
   i = 20;
#endif
#if defined MFFR
		sommari_to_snap();
                if(SD_saveic(MONIT,&i,"Stop - Snapshot",&sommari)>0)
#else
                if(SD_saveic(MONIT,&i,"Stop - Snapshot",NULL)>0)
#endif
                        vis_messaggio("Acknowledge SAVE I.C. received",0);
                else
                        vis_messaggio("Error Acknowledge SAVE I.C. not received"
,1);
#endif

/* chiude le eventuali finestre rimaste aperte */
    if (UtStatoWidget (widget_array[k_statistica_dialog]))
	XtUnmanageChild (widget_array[k_statistica_dialog]);
    if (UtStatoWidget (widget_array[k_elenco_pert_dialog]))
	XtUnmanageChild (widget_array[k_elenco_pert_dialog]);
    if (UtStatoWidget (widget_array[k_timescaling_dialog]))
	XtUnmanageChild (widget_array[k_timescaling_dialog]);
    if (UtStatoWidget (widget_array[k_stepscaling_dialog]))
	XtUnmanageChild (widget_array[k_stepscaling_dialog]);
    if (UtStatoWidget (widget_array[k_stepcr_dialog]))
	XtUnmanageChild (widget_array[k_stepcr_dialog]);
    if (UtStatoWidget (widget_array[k_editing_dialog]))
	XtUnmanageChild (widget_array[k_editing_dialog]);
    if (UtStatoWidget (widget_array[k_scrittura_dialog]))
	XtUnmanageChild (widget_array[k_scrittura_dialog]);
    if (UtStatoWidget (widget_array[k_caricamento_dialog]))
	XtUnmanageChild (widget_array[k_caricamento_dialog]);
    if (UtStatoWidget (widget_array[k_backtrack_dialog]))
        XtUnmanageChild (widget_array[k_backtrack_dialog]);

if(SD_stop (MONIT)>0)
        {
        vis_messaggio ("Command STOP send",0);
        system("mv parametri.out parametri.old");
/* elimina il processo aing */
    if (pid_aing != (-1))
	kill (pid_aing, SIGTERM);


/* elimina il processo staz */
    if (pid_staz != (-1))
	kill (pid_staz, SIGTERM);

/* elimina il processo graphics */
    if (pid_grafi != (-1))
	kill (pid_grafi, SIGTERM);

/* sgancio code messaggi */
    msg_close_fam ();

/* sgancio sh_var */
    distruggi_var (id_sh);

    exit (0);
	}
  else
        {
        vis_messaggio ("Error acknowledge STOP not received",1);
    undef_cursore (main_window_widget);
        }

}


void
clear_list_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;

	XmListDeleteAllItems(widget_array[widget_num]);
	numero_messaggi = 0;

}

void
quit_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;

    if(val_agg.stato_sked!=STATO_ERRORE)
        if(SD_sgancio (MONIT)<0)
            {
            vis_messaggio ("Error acknowledge QUIT not received",1);
            sleep(2);
            exit(0);
            }

/* elimina il processo aing */
    if (pid_aing != (-1))
	kill (pid_aing, SIGTERM);


/* elimina il processo staz */
    if (pid_staz != (-1))
	kill (pid_staz, SIGTERM);


/* sgancio sh_var */
    distruggi_var (id_sh);

/* sgancio code messaggi */
    msg_close_fam ();

    exit (0);
}

void
timescaling_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
char    app[30];
char   *testo;
float velocita;
    MSG_SKDIS messaggio;


    switch (widget_num)
    {
	case k_timescaling: 
	    {
		sprintf (app,"timescaling_dialog");
		dialog_proc (k_timescaling_dialog, app);
		break;
	    }
	case k_timescaling_dialog_ok: 
	    {
		testo = XmTextGetString
		    (widget_array[k_timescaling_dialog_text]);
		strcpy (app, testo);
		XtFree (testo);
		if ((atof (app) >= 0.1)||
			(atof (app) == -1))
		{
 		    velocita = (float)atof (app);
                    if(SD_timescaling (MONIT, &velocita)>0)
		      {
                      vis_messaggio ("Command TIMESCALING send",0);
#if defined LEGOCAD
		      /*
			Salvo sul file stato_monit la nuova velocita'
		      */
		      stato_monit.timescaling= velocita;	
		      save_stato_monit(&stato_monit);
#endif
		      }
                    else
                      vis_messaggio (
                         "Error acknowledge TIMESCALING not received",1);
		}
		break;
	    }
	case k_timescaling_dialog_max: 
	    {
 	        velocita = -1.;
                if(SD_timescaling (MONIT, &velocita)>0)
		     {
                     vis_messaggio ("Command TIMESCALING send",0);
#if defined LEGOCAD
		      /*
			Salvo sul file stato_monit la nuova velocita'
		      */
		      stato_monit.timescaling= velocita;	
		      save_stato_monit(&stato_monit);
#endif
		      }
                else
                vis_messaggio("Error acknowledge TIMESCALING not received",1);
		break;
	    }
	case k_timescaling_dialog_annulla: 
	    {
		break;
	    }
    }
}

void
stepscaling_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
char    app[30];
char   *testo;
float fattore;


    switch (widget_num)
    {
	case k_stepscaling: 
	    {
		sprintf (app, "stepscaling_dialog");
		dialog_proc (k_stepscaling_dialog, app);
		break;
	    }
	case k_stepscaling_dialog_ok: 
	    {
		testo = XmTextGetString
		    (widget_array[k_stepscaling_dialog_text]);
		strcpy (app, testo);
		XtFree (testo);
		if (atof (app) >= 0.0001)
		{
 		    fattore = (float)atof (app);
                    if(SD_stepscaling (MONIT, &fattore)>0)
		      {
                      vis_messaggio ("Command STEPSCALING send",0);
#if defined LEGOCAD
		      /*
			Salvo sul file stato_monit il nuovo step integration
		      */
		      stato_monit.stepscaling = fattore;	
		      save_stato_monit(&stato_monit);
#endif
		       }
                    else
                      vis_messaggio (
                         "Error acknowledge STEPTIMESCALING not received",1);
		}
		break;
	    }
	case k_stepscaling_dialog_annulla: 
	    {
		break;
	    }
    }
}


void
stepcr_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
int     passocr;
char    app[30];
char   *testo;
    MSG_SKDIS messaggio;

    switch (widget_num)
    {
	case k_stepcr: 
	    {
		sprintf (app, "stepcr_dialog");
		dialog_proc (k_stepcr_dialog, app);
		break;
	    }
	case k_stepcr_dialog_ok: 
	    {
		testo = XmTextGetString
		    (widget_array[k_stepcr_dialog_text]);
		strcpy (app, testo);
		XtFree (testo);
if ((atoi (app) >= 1) ||
                    (atoi (app) == 0))
                {
                    passocr = atoi (app);
                    if(SD_stepcr (MONIT, &passocr)>0)
                        {
                        vis_messaggio ("Command STEPCR send",0);
                        if(passocr == 0)
                            vis_messaggio("Continuos Recording off",1);
#if defined LEGOCAD
		      /*
			Salvo sul file stato_monit il nuovo stepcr
		      */
		      stato_monit.stepcr = passocr;	
		      save_stato_monit(&stato_monit);
#endif
                        }
                    else
                        vis_messaggio
                          ("Error acknowledge STEPCR not received",1);
                }
		break;
	    }
	case k_stepcr_dialog_annulla: 
	    {
		break;
	    }
    }
}

void
confcrall_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
int k,j;

    def_cursore (main_window_widget, OROLOGIO);

    strcpy (nome_file_selezione, "recorder.edf");
    flag_sel = SEL_REG;

    for (k = 0; k < tot_variabili; k++)
      		variabili[k].typ = GRAF_SEL;

    for (k = 0; k < tot_variabili; k++)
      if (variabili[k].typ == GRAF_SEL)
      {
         for (j = k + 1; j < tot_variabili; j++)
            if (variabili[k].addr == variabili[j].addr)
               variabili[j].typ = NO_GRAF_SEL;
      }

    scrive_sel(nome_file_selezione);
    elimina_f22();
    undef_cursore (main_window_widget);
}


void
confcr_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
char    app[30];
    XmString c_app;

    switch (widget_num)
    {
	case k_confcr: 
	case k_conflg: 
	    {
		if (widget_num == k_confcr)
		{
		    strcpy (nome_file_selezione, "recorder.edf");
		    flag_sel = SEL_REG;
		}
		else
		{
		    strcpy (nome_file_selezione, "varforlgf.edf");
		    flag_sel = SEL_LGPH;
		}
		legge_sel (nome_file_selezione);
		sprintf (app, "elenco_variabili_dialog");
		dialog_proc (k_elenco_variabili_dialog, app);

		sprintf (app, "File %s\0", nome_file_selezione);
		c_app = XmStringCreateLtoR (app, XmSTRING_DEFAULT_CHARSET);
		set_something (widget_array[k_elenco_variabili_dialog_label],
			XmNlabelString, (void*) c_app);
		XmStringFree (c_app);
		set_something (widget_array[k_elenco_variabili_dialog_label],
			XmNwidth, (void*) 300);
		set_something (widget_array[k_elenco_variabili_dialog_label],
			XmNheight, (void*) 35);

		sprintf (app, "Num. var. sel. >> %d", nu_var_sel);
		c_app = XmStringCreateLtoR (app, XmSTRING_DEFAULT_CHARSET);
		set_something (widget_array[k_elenco_variabili_nu_var_sel],
			XmNlabelString, (void*) c_app);
		XmStringFree (c_app);

		mo_var (widget_array[k_elenco_variabili]);
		mo_var_sel (widget_array[k_elenco_variabili_sel]);
		mo_blocchi (widget_array[k_elenco_blocchi]);
		mo_modelli (widget_array[k_elenco_modelli]);
		break;
	    }
	case k_salva_file_dialog_annulla: 
	    {
		XtUnmanageChild (widget_array[k_salva_file_dialog]);
		break;
	    }
	case k_elenco_variabili_dialog_ok: 
	    {
                XtUnmanageChild (widget_array[k_elenco_variabili_dialog]);
                def_cursore (main_window_widget, OROLOGIO);
                if (flag_sel == SEL_REG)
                        {
                        scrive_sel ("recorder.edf");
                        if(val_agg.stato_sked == STATO_FREEZE)
                                SD_newvarcr(MONIT);
                        else
				elimina_f22();
                        }
                else
                    scrive_sel ("varforlgf.edf");
                undef_cursore (main_window_widget);
                break;
	    }
	case k_elenco_variabili_annulla: 
	    {
		if (flag_sel == SEL_REG)
		    legge_sel ("recorder.edf");
		else
		    legge_sel ("varforlgf.edf");
		XtUnmanageChild (widget_array[k_elenco_variabili_dialog]);
		break;
	    }
	case k_selezione_file_dialog: 
	    {
		sprintf (app, "selezione_file_dialog");
		dialog_proc (k_selezione_file_dialog, app);
		break;
	    }
	case k_salva_file_dialog: 
	    {
		sprintf (app, "salva_file_dialog");
		dialog_proc (k_salva_file_dialog, app);
		break;
	    }
    }
}

void
load_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
int     i;
    MSG_SKDIS messaggio;
char    app[30];
    SNTAB app_snap;

        tab_snapshot ();
/* visualizza la finestra */
    sprintf (app, "caricamento_dialog");
    dialog_proc (k_caricamento_dialog, app);
    carica_snapshot (widget_array[k_caricamento_lista]);
}

void
save_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
int     i;
    MSG_SKDIS messaggio;
char    app[30];
    SNTAB app_snap;


/* visualizza la fibestra */
    sprintf (app, "scrittura_dialog");
    dialog_proc (k_scrittura_dialog, app);
}

void
edit_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
int     i;
int     size;
int     ret;
    SNTAB app_snap;
char    app[30];

    def_cursore (main_window_widget, OROLOGIO);

    tab_snapshot ();
    sprintf (app, "editing_dialog");
    dialog_proc (k_editing_dialog, app);
    elenca_snapshot (widget_array[k_editing_lista]);
    undef_cursore (main_window_widget);
}

void
statistics_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
char    app[30];


    if (widget_num == k_statistics)
    {
	sprintf (app, "statistica_dialog");
	/* dialog_proc(k_statistica_dialog,app); */
	if (widget_array[k_statistica_dialog] == NULL)
	    if (MrmFetchWidget (s_RMHierarchy, app, toplevel_widget,
			&widget_array[k_statistica_dialog], (MrmType *)&dummy_class) != MrmSUCCESS)
		s_error ("can't fetch dialog box");
    sprintf(app,"%d",25 * numero_modelli (ind_sh_top) + 85);    
	set_something (widget_array[k_statistica_dialog],
		XmNheight,  app);
    sprintf(app,"%d",25 * numero_modelli (ind_sh_top) + 5);
	set_something (widget_array[k_statistica_window],
		XmNheight,  app);
	sprintf(app,"%d",25 * numero_modelli (ind_sh_top) + 25);
    set_something (widget_array[k_statistica_dialog_fine],
		XmNy,       app);
	XtManageChild (widget_array[k_statistica_dialog]);
	refresh_stat ();
    }
}


void
procstat_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
int	ex_result, fork_result;
char	*list_par[4];

    	if (widget_num == k_procstat) {
	   if((fork_result=vfork()) != 0) {
	      if(fork_result == -1) {
		 printf("Parent: vfork failed\n");
	      }
	      else {
		 printf("Parent: Waiting for child\n");
		 printf("Parent: Child process id %d\n",fork_result);
	      }
	   }
	   else {
	      list_par[0] = "xterm";
	      list_par[1] = "-e";
	      list_par[2] = "proc_monit";
	      list_par[3] = NULL;
	      if((ex_result=execvp("xterm", list_par)) == -1) {
	         printf("Parent: execvp failed\n");
	         exit(1);
	      }
	   }
	}

}



void
inputstat_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
char    app[30];


if(widget_num!=k_inputstat_quit)
	{
    	def_cursore (main_window_widget, OROLOGIO);
	tab_perturba();
        memcpy(perturbazioni_old,perturbazioni,sizeof(TIPO_PERT)*_MAX_PERTUR*2);
    	sprintf (app, "input_statistics_dialog");
    	dialog_proc (k_inputstat_dialog, app);
        elenca_pert (widget_array[k_inputstat_lista_1],
			widget_array[k_inputstat_lista_2]);
    	undef_cursore (main_window_widget);
        /* 
                    setta il timer per la funzione di refresh 
		    visualizzazione perturbazioni 
	*/
        timer_refpert = XtAppAddTimeOut (
	    XtWidgetToApplicationContext (main_window_widget),
	    (unsigned long) TM_REFRESH*4, inputstat_refresh, NULL);
	}
else
	{
        XtRemoveTimeOut (timer_refpert);
	XtUnmanageChild (widget_array[k_inputstat_dialog]);
	}
	
}

void
inputstat_refresh()
{

	tab_perturba();
        if(memcmp(perturbazioni_old,
		perturbazioni,sizeof(TIPO_PERT)*_MAX_PERTUR*2)!=0)
	  {
	  memcpy(perturbazioni_old,
		perturbazioni,sizeof(TIPO_PERT)*_MAX_PERTUR*2);
          elenca_pert (widget_array[k_inputstat_lista_1],
			widget_array[k_inputstat_lista_2]);
	  }
/* setta il timer per la funzione di refresh visualizzazione perturbazioni */
    timer_refpert = XtAppAddTimeOut (
	    XtWidgetToApplicationContext (main_window_widget),
	    (unsigned long) TM_REFRESH*4, inputstat_refresh, NULL);
}

void
editpert_refresh()
{

        tab_perturba();
        if(memcmp(perturbazioni_old,
                perturbazioni,sizeof(TIPO_PERT)*_MAX_PERTUR*2)!=0)
          {
          memcpy(perturbazioni_old,
                perturbazioni,sizeof(TIPO_PERT)*_MAX_PERTUR*2);
          elenca_modpert (widget_array[k_editpert_lista_1],
                        widget_array[k_editpert_lista_2],1);
          elenca_modpert (widget_array[k_editpert_lista_1],
                        widget_array[k_editpert_lista_2],2);
          }
/* setta il timer per la funzione di refresh visualizzazione perturbazioni */
    timer_refeditpert = XtAppAddTimeOut (
            XtWidgetToApplicationContext (main_window_widget),
            (unsigned long) TM_REFRESH*2, editpert_refresh, NULL);
}

void
editpert_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
char    app[30];

if(widget_num!=k_editpert_quit)
	{
    	def_cursore (main_window_widget, OROLOGIO);
    	sprintf (app, "editpert_statistics_dialog");
    	dialog_proc (k_editpert_dialog, app);
	tab_perturba ();
        elenca_modpert (widget_array[k_editpert_lista_1],
			widget_array[k_editpert_lista_2],1);
        elenca_modpert (widget_array[k_editpert_lista_1],
			widget_array[k_editpert_lista_2],2);
	/*
		attiva la finestra di richiamo delle AING
	*/
	sprintf (app, "elenco_pert_dialog");
	dialog_proc (k_elenco_pert_dialog, app);
	mo_pert_sel (widget_array[k_elenco_ingressi]);
	mo_blocchi (widget_array[k_elenco_blocchi_pert]);
	mo_modelli (widget_array[k_elenco_modelli_pert]);
    	undef_cursore (main_window_widget);
        /* 
                    setta il timer per la funzione di refresh 
		    visualizzazione perturbazioni 
	*/
        timer_refeditpert = XtAppAddTimeOut (
	    XtWidgetToApplicationContext (main_window_widget),
	    (unsigned long) TM_REFRESH*4, editpert_refresh, NULL);
	}
else
	{
        XtRemoveTimeOut (timer_refeditpert);
	XtUnmanageChild (widget_array[k_editpert_dialog]);
	}
}

void
input_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
char    app[100];
static  Widget attenzione = NULL;
    Dialog_geometry geom;
    FILE * fp;

    switch (widget_num)
    {
	case k_input: 
	    sprintf (app, "elenco_pert_dialog");
	    dialog_proc (k_elenco_pert_dialog, app);
	    mo_pert_sel (widget_array[k_elenco_ingressi]);
	    mo_blocchi (widget_array[k_elenco_blocchi_pert]);
	    mo_modelli (widget_array[k_elenco_modelli_pert]);
	    break;

	case k_staz_button: 
	    fp = fopen ("r02.dat", "r+");
	    if (fp == NULL)
	    {
		geom.default_pos = 1;
		geom.height = 200;
		if (attenzione == NULL)
		    attenzione = attention (main_window_widget,
			    "File r02.dat non esistente",
			    CREA | MAPPA, geom);
		else
		    attenzione = attention (main_window_widget,
			    "File r02.dat non esistente",
			    MAPPA, geom);
		break;
	    }
	    fclose (fp);
	    sprintf (app, "staz_dialog");
	    dialog_proc (k_staz_dialog, app);
	    elenca_staz (widget_array[k_staz_lista]);
	    break;
    }
}

void
data_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
char    app[100];

	sprintf (app, "data_dialog");
	dialog_proc (k_data_dialog, app);
        mo_blocchi (widget_array[k_elenco_blocchi_modat]);
	mo_modelli (widget_array[k_elenco_modelli_modat]);
}

void graphics_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
/*
    ancio di graphics fatto 
    tramite una execve per poter gestirne poi la terminazione.
*/
        attiva_graphics();

        vis_messaggio ("Process graphics activated",0);

}

void go_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
    MSG_SKDIS messaggio;


    if (val_agg.stato_sked == STATO_FREEZE)
    {
	/* manda il comando di freeze allo sked */
	if (SD_goup (MONIT) > 0)
	    vis_messaggio ("Acknowledge STEP received", 0);
	else
	    vis_messaggio ("Error acknowledge STEP not received", 1);
    }
}

void about_proc (w, tag, reason)
Widget w;
int    *tag;
unsigned long  *reason;
{
int     widget_num = *tag;
    MSG_SKDIS messaggio;

	about(toplevel_widget);

}


/* 
 * refresh_proc si occupa di aggiornare periodicamente i valori riportati
 * nella main window di monit 
 */
void refresh_proc (client_data, id)
caddr_t client_data;
XtIntervalId * id;
{
XmString c_app_str;
char    app_str[10];
char    app_str_l[50];
int     i;
static int initial_query=1;
static int prima_volta=1;
FILE *fp;
int slot=0;
char app_char[100];
char app_var[MAX_LUN_NOME_VAR];
int sessione;
static int cinque_cicli = 0;
static int memoria_replay = 0;
static int bt_caricato;
int default_stepcr = 1;
long    ora,min,sec,giorno,mese,anno;


/* Routine che aggiorna i valori  da visualizzare  */
   monit_aggiorna (&val_agg);

/*
 * prende il tempo di orologio
 */
   ftime(&tempo);
                        
/*
    LETTURE INIZIALI.
    In caso di stato backtrack si rileggono gli ultimi salvati/caricati
*/
if((initial_query==1)&&(val_agg.stato_sked==STATO_FREEZE))
   {
   initial_query=0;
   if(SD_stepcr (MONIT,  &default_stepcr)>0)
      vis_messaggio ("Command STEPCR send",0);
   else
      vis_messaggio("Error acknowledge STEPCR not received",1);
                                                              
/* Apertura file listaci.edf, se esiste */
   if(fp=fopen("listaci.edf","r"))
	{
	vis_messaggio("File listaci.edf found",0);
	while(fscanf(fp,"%s",app_var)>0)
	   {
	   /* invio LISTACI */
	   ++slot;
           if((SD_listaci (MONIT,app_var,slot)>0)&&(slot<=MAX_VAL_SNAP))
	     {
	     sprintf(app_char,"Acknowledge LISTACI received - %s in slot %d",
	             app_var,slot);
             vis_messaggio(app_char,0);
             strncpy(&var_slot[slot-1][0],app_var,MAX_LUN_NOME_VAR);
             }
           else
	     {
	     if(slot<MAX_VAL_SNAP)
		{
		--slot;
        	vis_messaggio("Error Acknowledge LISTACI not received",1);
		}
             else
		vis_messaggio("Error - too many variables",0);
	     }
	   } /* Fine while */
	fclose(fp);
	}
    else
	vis_messaggio("File listaci.edf not found",0);

/* Lettura ultimi snap e bktk caricati e salvati */
   if((stato_cr.last_snap_save=SD_lsnapreg(MONIT))>0)
	{
	sprintf(app_char,
                "Last snapshot saved -> %d",stato_cr.last_snap_save);
       	vis_messaggio(app_char,0);
	}
   if((stato_cr.last_snap_load=SD_lsnapload(MONIT))>0)
	{
	sprintf(app_char,
                "Last snapshot loaded -> %d",stato_cr.last_snap_load);
       	vis_messaggio(app_char,0);
	}
#if defined BACKTRACK
   if((stato_cr.last_bktk_save=SD_lbtreg(MONIT))>0)
	{
	last_backtrack=stato_cr.last_bktk_save;
	sprintf(app_char,
                "Last backtrack saved -> %d",stato_cr.last_bktk_save);
       	vis_messaggio(app_char,0);
	}
   if((stato_cr.last_bktk_load=SD_lbtload(MONIT))>0)
	{
/*      Aggiunta (2-9-96) per settare il valore iniziale da visualizzare */
	last_load_bt=stato_cr.last_bktk_load;
	sprintf(app_char,
                "Last backtrack loaded -> %d",stato_cr.last_bktk_load);
       	vis_messaggio(app_char,0);
	}
#endif

#if defined LEGOCAD
	/*
		legge da file:
			timescaling
			endtime
			stepscaling
			stepcr
		e li invia allo sked
	*/
	load_stato_monit(&stato_monit);

        if(SD_timescaling (MONIT, &stato_monit.timescaling)>0)
              vis_messaggio ("Command TIMESCALING send",0);
        else
              vis_messaggio ("Error acknowledge TIMESCALING not received",1);

	if ( stato_monit.endtime >= val_agg.tempo_sim) 
           {
           tempo_finale = stato_monit.endtime;
           tempo_finale_impostato=1;
           sprintf(app_char,"Max simulation time set = %f",tempo_finale);
           vis_messaggio ( app_char,0);
           val_agg.endtime_sim=tempo_finale;
           val_agg.endtime_sim_a=1; 
           }

        if(SD_stepscaling (MONIT,  &stato_monit.stepscaling)>0)
           vis_messaggio ("Command STEPSCALING send",0);
        else
           vis_messaggio("Error acknowledge STEPTIMESCALING not received",1);

        if(SD_stepcr (MONIT,  &stato_monit.stepcr)>0)
           vis_messaggio ("Command STEPCR send",0);
        else
           vis_messaggio("Error acknowledge STEPCR not received",1);
#endif
   } /* Fine if su initial_query==1 & stato FREEZE */
#if defined BACKTRACK_CONREFRESH
else 
   if( val_agg.stato_sked==STATO_BACKTRACK )
   {
   if((stato_cr.last_bktk_save=SD_lbtreg(MONIT))>0)
	{
	last_backtrack=stato_cr.last_bktk_save;
	sprintf(app_char,
                "Last backtrack saved -> %d",stato_cr.last_bktk_save);
       	vis_messaggio(app_char,0);
	}
   if((stato_cr.last_bktk_load=SD_lbtload(MONIT))>0)
	{
/*      Aggiunta (2-9-96) per settare il valore iniziale da visualizzare */
	last_load_bt=stato_cr.last_bktk_load;
	sprintf(app_char,
                "Last backtrack loaded -> %d",stato_cr.last_bktk_load);
       	vis_messaggio(app_char,0);
	}
   }
#endif


/* Aggiornamento TEMPO SIMULAZIONE */
   if (val_agg.tempo_sim_a == 1)
      {
      /*printf("\t\t\t MONIT temposim=%f\n",val_agg.tempo_sim);*/
      sprintf (app_str, "%7.1f", val_agg.tempo_sim);
      c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
      set_something (widget_array[k_tempo_sim_val], XmNlabelString, (void*) c_app_str);
      XmStringFree (c_app_str);
      val_agg.tempo_sim_a = 0;
      }

/*
   Verifica che non sia stato raggiunto il TEMPO MASSIMO DI SIMULAZIONE
*/
   if((tempo_finale_impostato==1)&&(val_agg.tempo_sim>=tempo_finale))
      {
      vis_messaggio ("Final time reached",0);
      tempo_finale_impostato=0;
      /* manda il comando di freeze allo sked */
      if(SD_freeze (MONIT)>0)
         vis_messaggio ("Command FREEZE send",0);
      else
         vis_messaggio ("Error acknowledge FREEZE not received",1);
      val_agg.endtime_sim=0;
      val_agg.endtime_sim_a=1;
#if defined LASTSNAP
     /* 
      salva uno snapshot in posizione MAX_SNAP_SHOT -1 
      con il nome End time - Snapshot 
      */
      i=_MAX_SNAP_SHOT-1;
#if defined LEGOCAD
      i = 19;
#endif
#if defined MFFR
      sommari_to_snap();
      if(SD_saveic(MONIT,&i,"End time - Snapshot",&sommari)>0)
#else
      if(SD_saveic(MONIT,&i,"End time - Snapshot",NULL)>0)
#endif
          vis_messaggio("Acknowledge SAVE I.C. received",0);
      else
          vis_messaggio("Error Acknowledge SAVE I.C. not received",1);
#endif
      }

#if defined BACKTRACK
/* Verifica la necessita di SALVARE UN BACKTRACK */
   if ((prima_volta == 1) && (val_agg.stato_sked == STATO_FREEZE))
     {
     old_tempo_sim = val_agg.tempo_sim;
     prima_volta=0;
     }
   else
     {
     if (val_agg.stato_sked == STATO_RUN)
        {
        intervallo_backtrack -= (val_agg.tempo_sim - old_tempo_sim);
        old_tempo_sim = val_agg.tempo_sim;
        if (intervallo_backtrack <= 0.)
            {
            printf ("monit triggera backtrack\n");
            intervallo_backtrack = (float)INTERVALLO_BACKTRACK;
            ++last_backtrack;
            if(last_backtrack>_MAX_BACK_TRACK)
                last_backtrack=1;
#if defined MFFR
            sommari_to_snap();
            strcpy (app_str,"BT SNAP");
            if(SD_savebt(MONIT,&last_backtrack,app_str,(char *)&sommari)>0)
#else
            strcpy (app_str,"BT SNAP");
            if(SD_savebt(MONIT,&last_backtrack,app_str,NULL)>0)
#endif
                         vis_messaggio("Command SAVE B.T. send",0);
            else
               vis_messaggio("Error Acknowledge SAVE B.T. not received",1);
            }
        }
     }

/*
 * Gestione STATO REPLAY
 */
   if (val_agg.stato_sked == STATO_REPLAY)
      {
      if (++cinque_cicli >= 5)
         {
         cinque_cicli = 0;
         if((stato_cr.last_bktk_load=SD_lbtload(MONIT))>0)
            {
/*          Aggiunta (2-9-96) per settare il valore corrente da visualizzare */
            last_load_bt=stato_cr.last_bktk_load;
            sprintf(app_char,
            "Last backtrack loaded -> %d",stato_cr.last_bktk_load);
               vis_messaggio(app_char,0);
            if (bt_caricato != stato_cr.last_bktk_load)
                {
                bt_caricato = stato_cr.last_bktk_load;
                for (i=1; i<= _MAX_BACK_TRACK; i++)
                     {
                     if (punt_rec_backtrack[i] == bt_caricato)
                         {
/*                       Sposto la riga di evidenziazione */
/*                       printf ("\t\t evidenziare riga %d\n",i);     */
                         XmListSelectPos(widget_array[k_caricamento_lista_bt],
                                         i,TRUE);
                         bt_al_centro();
                         limiti_sessione();

/*                       Evidenzio il numero del bktk caricato */
                         sprintf (app_str, "%d",last_load_bt);
                         c_app_str = XmStringCreateLtoR (app_str,
                                                    XmSTRING_DEFAULT_CHARSET);
                         set_something (widget_array[k_lastload_bt_val],
                                        XmNlabelString, (void*) c_app_str);
                         XmStringFree (c_app_str);

                         break;
                         }
                     }
                 }
            }
         }
      }

/*
 * Gestisce BACKTRACK AUTOMATICO
 */
     if ((val_agg.stato_sked == STATO_BACKTRACK) &&
         (modo_bt == BT_AUTOMATICO))
          {
          timer_auto_bt -= (tempo.time - old_time_bt);
          old_time_bt = tempo.time;
          if (timer_auto_bt <= 0)       /* timer scaduto */
              {
              printf ("\t\t backtrack automatico\n");
              timer_auto_bt = TIMER_AUTO_BT;
              vis_messaggio("Command AUTO LOAD B.T. sent",0); 
              def_cursore (toplevel_widget, OROLOGIO);
	      def_cursore (toplevel_widget, OROLOGIO);

              XmListSelectPos( widget_array[k_caricamento_lista_bt],
                              step_selezionato, TRUE);
              bt_al_centro();
              limiti_sessione();

#if defined MFFR
              if( SD_loadbt(MONIT,&backtrack_selezionato,
                           &sommari) >0 )
#else
              if( SD_loadbt(MONIT,&backtrack_selezionato,
                           NULL) >0 )
#endif
                  {
#if defined MFFR
                  if (snap_to_sommari() < 0)
                      vis_messaggio("BT: sommari non validi",0);
#endif
                  last_load_bt=step_selezionato;
                  sessione = sessioni_bt[step_selezionato-1];
                  vis_messaggio(
                      "Acknowledge AUTO LOAD B.T. received",0);
                  }
              else
                  vis_messaggio( "Error Acknowledge LOAD B.T. not received",1);
              undef_cursore (toplevel_widget);

/* seleziona il prossimo step */
              if (direzione_bt == BT_AVANTI)
                  {
                  if (step_selezionato == num_bt_list)
                     {
/*                   direzione_bt = BT_INDIETRO; */
/*                   modo_bt=BT_MANUALE;         */
/*                   step_selezionato--;         */
                     bt_in_manuale();
                     bt_indietro();
                     }
                  else
                     step_selezionato++;
                  }
              else                     /* direzione indietro */
                  {
                  if (step_selezionato == 1)
                     {
/*                   direzione_bt = BT_AVANTI;      */
/*                   modo_bt = BT_MANUALE;          */
/*                   step_selezionato++;            */
                     bt_in_manuale();
                     bt_avanti();
                     }
                  else
                     step_selezionato--;
                  }

              if (sessione != sessioni_bt[step_selezionato-1])
/* fuori sessione */
/*                modo_bt = BT_MANUALE;              */
                  bt_in_manuale();
              }
          else
             printf("\t TIMER \n\n\n");
          }  /* Fine if su BACKTRACK AUTOMATICO */

/*
 * Aggiorna alcune LABELS del backtrack dialog
 */
        if (val_agg.stato_sked == STATO_BACKTRACK)
           {
           if (modo_bt == BT_MANUALE)
              strcpy(app_str,"Manuale");
           else
              strcpy(app_str,"Automatico");
           c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
           set_something (widget_array[k_modo_bt_val], XmNlabelString, 
                          (void*) c_app_str);
           XmStringFree (c_app_str);

           if (direzione_bt == BT_AVANTI)
              strcpy(app_str,"Avanti");
           else
              strcpy(app_str,"Indietro");

           c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
           set_something (widget_array[k_direz_bt_val], XmNlabelString,
                          (void*) c_app_str);
           XmStringFree (c_app_str);

/*         sprintf (app_str, "%d",last_backtrack);   */
           sprintf (app_str, "%d",num_bt_list);
           c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
           set_something (widget_array[k_lastrec_bt_val], XmNlabelString, 
                          (void*) c_app_str);
/*printf("\t\t\t\t SETTO last_backtrack=%d",last_backtrack);*/
           XmStringFree (c_app_str);

           sprintf (app_str, "%d",last_load_bt);
/*printf(" SETTO last_load_bt=%d\n",last_load_bt);*/
           c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
           set_something (widget_array[k_lastload_bt_val],XmNlabelString, 
                         (void*) c_app_str);
           XmStringFree (c_app_str);

           sprintf (app_str, "%d",step_selezionato);
           c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
           set_something (widget_array[k_nextstep_bt_val],XmNlabelString, 
                          (void*) c_app_str);
           XmStringFree (c_app_str);
           }
#endif

/*  Aggiornamento TEMPO RITARDO */
    if (val_agg.tempo_ritardo_a == 1)
        {
	sprintf (app_str, "%7.1f", val_agg.tempo_ritardo);
	c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
	set_something (widget_array[k_tempo_ritardo_val], XmNlabelString,
                       (void*) c_app_str);
	XmStringFree (c_app_str);
	val_agg.tempo_ritardo_a = 0;
        }

/* Aggiornamento TEMPO REALE */
    if (val_agg.tempo_reale_a == 1)
        {
	sprintf (app_str, "%7.1f", val_agg.tempo_reale);
	c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
	set_something (widget_array[k_tempo_reale_val], XmNlabelString,
                       (void*) c_app_str);
	XmStringFree (c_app_str);
	val_agg.tempo_reale_a = 0;
        }

/* Aggiornamento TIME SCALING */
    if (val_agg.timescaling_sim_a == 1)
        {
	if(val_agg.timescaling_sim>0)
		sprintf (app_str, "%7.1f", val_agg.timescaling_sim);
	else
		sprintf (app_str, "Max. Speed");
	c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
	set_something (widget_array[k_timescaling_sim_val], XmNlabelString,
                       (void*) c_app_str);
	XmStringFree (c_app_str);
	val_agg.timescaling_sim_a = 0;
        }

/*  Aggiornamento STEP SCALING */
    if (val_agg.stepscaling_sim_a == 1)
        {
	if(val_agg.stepscaling_sim>0)
		sprintf (app_str, "%7.1f", val_agg.stepscaling_sim);
	c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
	set_something (widget_array[k_stepscaling_sim_val], XmNlabelString, (void*) c_app_str);
	XmStringFree (c_app_str);
	val_agg.stepscaling_sim_a = 0;
        }


/*  Aggiornamento TEMPO FINALE SIMULAZIONE */
    if (val_agg.endtime_sim_a == 1)
        {
        if(val_agg.endtime_sim > val_agg.tempo_sim)
                sprintf (app_str, "%7.1f", val_agg.endtime_sim);
        else
                sprintf (app_str, "none");
        c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
        set_something (widget_array[k_endtime_sim_val], XmNlabelString, 
		       (void*) c_app_str);
        XmStringFree (c_app_str);
        val_agg.endtime_sim_a = 0;
        }

/*  Aggiornamento PASSO_REG */
    if (val_agg.passo_reg_a == 1)
        {
	if(val_agg.passo_reg>0)
		sprintf (app_str, "%d", val_agg.passo_reg);
	else
		sprintf (app_str, "Off");
	c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
	set_something (widget_array[k_passo_reg_val], XmNlabelString, 
                      (void*) c_app_str);
	XmStringFree (c_app_str);
	val_agg.passo_reg_a = 0;
        }

/*  Aggiornamento VARIABILI REGISTRATE PER LA GRAFICA */
    if (val_agg.grandezze_mem_a == 1)
        {
	sprintf (app_str, "%7d", val_agg.grandezze_mem);
	c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
	set_something (widget_array[k_grandezze_mem_val], XmNlabelString, 
                      (void*) c_app_str);
	XmStringFree (c_app_str);
	val_agg.grandezze_mem_a = 0;
        }

/*  Aggiornamento TEMPO SIM MB */
    if (val_agg.tempo_simMB_a == 1)
        {
	sprintf (app_str, "%7.1f", val_agg.tempo_simMB);
	c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
	set_something (widget_array[k_tempo_simMB_val], XmNlabelString,
                       (void*) c_app_str);
	XmStringFree (c_app_str);
	val_agg.tempo_simMB_a = 0;
        }

/*  Aggiornamento SPAZIO DISCO */
    if (val_agg.spazio_dsk_a == 1)
        {
	sprintf (app_str, "%7d", val_agg.spazio_dsk);
	c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
	set_something (widget_array[k_spazio_dsk_val], XmNlabelString,
                       (void*) c_app_str);
	XmStringFree (c_app_str);
	val_agg.spazio_dsk_a = 0;
        }

/*  Aggiorna lo STATO DELLE TASK */
    for (i = 0; i < numero_modelli (ind_sh_top); i++)
        {
	if (val_agg.stato_task_a[i] == 1)
	    {
	    if (val_agg.stato_task[i] == 1)
	        {
		sprintf (app_str_l, "Task %s started", 
                         nome_modello (ind_sh_top, i + 1));
		vis_messaggio (app_str_l,0);
	        }
	    else
	        {
		sprintf (app_str_l, "Task %s aborted", 
                         nome_modello (ind_sh_top, i + 1));
		vis_messaggio (app_str_l,1);
	        }
	    val_agg.stato_task_a[i] = 0;
	    }
        }

/* Aggiorna lo STATO DEI PROCESSI AUSILIARI */
/* if(val_agg.stato_snapshot_a==1) { if(val_agg.stato_snapshot==1)
 * vis_messaggio("Process SNAPSHOT started"); else vis_messaggio("Process
 * SNAPSHOT aborted"); val_agg.stato_snapshot_a=0; } */

#if defined BANCO_MANOVRA
    for(i=0;i<MAX_SLAVE+2;i++)
	if (val_agg.stato_agg_manovra_a[i] == 1)
	{
	if( val_agg.stato_agg_manovra[i] <= 0 )
	  {
	  if(i==0)
	      vis_messaggio ("Process agg_manovra MASTER aborted",1);
	  if(i==1)
	      vis_messaggio ("Process agg_manovra SLAVE local aborted",1);
	  if(i>1)
	      vis_messaggio ("Process agg_manovra SLAVE remote aborted",1);
	  val_agg.stato_agg_manovra_a[i] = 0;
	  }
	else
	  val_agg.stato_agg_manovra_a[i] = 0;
	}

    for(i=0;i<MAX_SLAVE+1;i++)
	if (val_agg.stato_agg_manovra_pert_a[i] ==1)
	{
	if( val_agg.stato_agg_manovra_pert[i] <=0 )
	  {
	  if(i==0)
	      vis_messaggio ("Process agg_manovra_pert local aborted",1);
	  if(i>=1)
	      vis_messaggio ("Process agg_manovra_pert SLAVE remote aborted",1);
	  val_agg.stato_agg_manovra_pert_a[i] = 0;
	  }
	else
	  val_agg.stato_agg_manovra_pert_a[i] = 0;
	}

    if (val_agg.stato_agg_manovra_stato_a == 1)
    {
      if (val_agg.stato_agg_manovra_stato <= 0)
	{
	vis_messaggio ("Process agg_manovra_stato aborted",1);
	val_agg.stato_agg_manovra_stato_a = 0;
	}
      else
	val_agg.stato_agg_manovra_stato_a = 0;
    }

#endif
#if defined SCADA
    if (val_agg.stato_agg_scada_a == 1)
    {
      if (val_agg.stato_agg_scada <= 0)
	{
	vis_messaggio ("Process agg_scada aborted",1);
	val_agg.stato_agg_scada_a = 0;
	}
      else
	val_agg.stato_agg_scada_a = 0;
    }
#endif

    if (val_agg.stato_prepf22_a == 1)
    {
	if (val_agg.stato_prepf22 == 1)
            {
	    vis_messaggio ("Process PREP_F22 started",0);
            }
	else
            {
	    vis_messaggio ("Process PREP_F22 aborted",1);
            }
	val_agg.stato_prepf22_a = 0;
    }

#if defined BANCO_MANOVRA
/*
 * blocca i comandi durante il controllo allineamento
 */
        if ((check_in_corso == 1) && (primo_check == 0))
                {
                primo_check = 1;
                cambia_stato_menu (voci_check);
                }

        if ((primo_check == 1) && (check_in_corso == 0))
                {
                primo_check = 0;
                val_agg.stato_sked_a = 1;  /* rinfresca con lo stato attuale */
                }
#endif

/* aggiornamento stato simulazione aggiornando la label di stato e i menu */
    if (val_agg.stato_sked_a == 1)
    {
        /* resetta il pointer */
	if(settato_pointer==1)
	    undef_cursore(main_window_widget);
		
	/* aggiorna label di stato  */
	sprintf (app_str, "%s", stato_sked_label[val_agg.stato_sked]);
	c_app_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
	set_something (widget_array[k_stato_sim_val], XmNlabelString, (void*) c_app_str);
	XmStringFree (c_app_str);

	/* aggiorna menu	    */
	switch (val_agg.stato_sked)
	{
	    case STATO_STOP: 
		{
		    cambia_stato_menu (voci_stop);
		    /* fissa il colore dello sfondo */
		    set_something (widget_array[k_stato_sim_val],
			    XmNbackground, (void*) pix_val[0]);
		    break;
		}
	    case STATO_RUN: 
		{
		    cambia_stato_menu (voci_run);
		    /* fissa il colore dello sfondo */
		    set_something (widget_array[k_stato_sim_val],
			    XmNbackground, (void*) pix_val[1]);
		    break;
		}
#if defined BACKTRACK
            case STATO_BACKTRACK:
                {
                                printf ("memoria_replay = %d\n",memoria_replay);
                                         if (memoria_replay == 1)
                                                {
                                printf ("backtrack da replay\n");
                                                memoria_replay = 0;
                                                if((stato_cr.last_bktk_load=SD_lbtload(MONIT))>0)
                     {
                     sprintf(app_char,
                     "Last backtrack loaded -> %d",stato_cr.last_bktk_load);
                        vis_messaggio(app_char,0);
                     }
                                                }
                cambia_stato_menu (voci_backtrack);
                /* fissa il colore dello sfondo */
                set_something (widget_array[k_stato_sim_val],
                        XmNbackground, (void*) pix_val[2]);
                break;
                }
            case STATO_REPLAY:
                {
                                         memoria_replay = 1;
                                         bt_caricato = 0;
                 cambia_stato_menu (voci_replay);
                 /* fissa il colore dello sfondo */
                 set_something (widget_array[k_stato_sim_val],
                         XmNbackground, (void*) pix_val[1]);
                    break;
                }
#endif
	    case STATO_FREEZE: 
		{
		    cambia_stato_menu (voci_freeze);
		    /* fissa il colore dello sfondo */
		    set_something (widget_array[k_stato_sim_val],
			    XmNbackground, (void*) pix_val[2]);
		    break;
		}
	    case STATO_ERRORE: 
		{
		    cambia_stato_menu (voci_errore);
		    /* fissa il colore dello sfondo */
		    set_something (widget_array[k_stato_sim_val],
			    XmNbackground, (void*) pix_val[0]);
		    break;
		}
	}
	val_agg.stato_sked_a = 0;
    }

/* aggiorna label stato sked */

/* setta il timer per la funzione di refresh dei valori visualizzati */
    timer_id = XtAppAddTimeOut (
	    XtWidgetToApplicationContext (main_window_widget),
	    (unsigned long) TM_REFRESH, refresh_proc, NULL);
} /* FINE refresh_proc */

/*
 vis_messaggio
    visualizza un messaggio nell'area della mein window dedicata
    ai messaggi di diagnostica
*/
int vis_messaggio (str,finestra)
char   *str;
int finestra;
{
    XmString c_str;
char    app_str[200];
static Widget attenzione=NULL;
Dialog_geometry geom;


/* registrazione dei messaggi  */
/*
++numero_messaggi;
strncpy(messaggi.mess[numero_messaggi],str,MAX_LUNG_MESSAGGIO);*/
    sprintf (app_str, "%s (t=%4.1f)", str, val_agg.tempo_sim);


    ++numero_messaggi;
    c_str = XmStringCreateLtoR (app_str, XmSTRING_DEFAULT_CHARSET);
    XmListAddItem (widget_array[k_messaggi], c_str, 0);
    XmStringFree (c_str);
    if (numero_messaggi > 5)
        XmListSetPos (widget_array[k_messaggi], numero_messaggi - 5);
    XSync (display, False);
    if(finestra==1)
        {
        geom.default_pos=1;
        geom.height=200;
        if(attenzione==NULL)
                attenzione=attention(main_window_widget,
                         str,CREA|MAPPA,geom);
        else
                attenzione=attention(main_window_widget,
                        str,MAPPA,geom);
        }
}

int cambia_stato_menu (voci_array)
VOCE_MENU * voci_array;
{
int     i;
static int prima_volta=1;
if (prima_volta)
	{
	distruggi_voci_inutili();
	prima_volta=0;
	}

for (i = 0; i < NUMERO_VOCI_MENU; i++)
    {
	if(widget_array[voci_array[i].ind]!=NULL)
	set_something (widget_array[voci_array[i].ind], XmNsensitive, (void*) voci_array[i].stato);
    }
}

void
dialog_proc (widget_num, widget_name)
int     widget_num;
char   *widget_name;
{

    if (widget_array[widget_num] == NULL)
	if (MrmFetchWidget (s_RMHierarchy,widget_name , toplevel_widget,
		    &widget_array[widget_num], (MrmType *)&dummy_class) != MrmSUCCESS)
	    s_error ("can't fetch dialog box");
    XtManageChild (widget_array[widget_num]);
}

void
tab_snapshot ()
{

    vis_messaggio ("Command EDIT I.C. send",0);
    if(SD_editic (MONIT, snapshot,0,_MAX_SNAP_SHOT)>0)
        vis_messaggio ("Snapshot table received",0);
    else
        vis_messaggio ("Error snapshot table not received",1);
}

#if defined BACKTRACK
void tab_backtrack ()
{
   if(RtCheckPointer(NULL) == False)
      printf("!!! ATTENZIONE SPORCAMENTO MEMORIA ingresso tab_backtrack\n");

   vis_messaggio ("Command EDIT B.T. send",0);
   if(SD_editbt (MONIT, backtrack,0,_MAX_BACK_TRACK)>0)
      vis_messaggio ("Backtrack table received",0);
   else
       vis_messaggio ("Error backtrack table not received",1);
   if(RtCheckPointer(NULL) == False)
      printf("!!! ATTENZIONE SPORCAMENTO MEMORIA uscita tab_backtrack\n");
}
#endif

void
tab_perturba ()
{
    if(SD_editpert (MONIT, perturbazioni,0,_MAX_PERTUR)<0)
        vis_messaggio ("Error pert table not received",1);
}



void clear_list_mouse(w,num,event)
Widget w;
int *num;
XButtonEvent *event;
{

if (event -> button == Button3)
	{
	if (widget_array[k_clear_list_popup] == NULL)
    	   {  
            if (MrmFetchWidget(s_RMHierarchy, "clear_list_popup" ,
		widget_array[k_messaggi], &widget_array[k_clear_list_popup], 
		(MrmType *)&dummy_class) != MrmSUCCESS) {
                       s_error("can't fetch pop widget");
                    }
           }
	XmMenuPosition(widget_array[k_clear_list_popup],event);
        /*  Visualizza il popup menu */
	XtManageChild(widget_array[k_clear_list_popup]);
	}
}



#if defined LEGOCAD

int save_stato_monit(stato_monit)
STATO_MONIT *stato_monit;
{
FILE *fp;

if(fp=fopen("stato_monit.rtf","w"))
        {
        fwrite(stato_monit,sizeof(STATO_MONIT),1,fp);
        fclose(fp);
        return(1);
        }
else
        {
        perror("dispatcher(impossibile salvare file statocr.rtf)");
        return( -1);
        }
}

int load_stato_monit(stato_monit)
STATO_MONIT *stato_monit;
{
FILE *fp;

if(fp=fopen("stato_monit.rtf","r"))
        {
        fread(stato_monit,sizeof(STATO_MONIT),1,fp);
        fclose(fp);
        return(1);
        }
        else
        {
        stato_monit->timescaling= 1.;
        stato_monit->endtime= 0.;
        stato_monit->stepscaling= 1.;
        stato_monit->stepcr= 1.;
        return(-1);
        }
}
#endif



#ifndef MFFR
void malf_proc()
{
}
void frem_proc()
{
}
#endif


int attiva_graphics()
{
char *proc_argv[4];
char *proc_envp[10];
char proc_name[FILENAME_MAX+1];
char *getenv();
char *path=NULL;
char *path_loc=NULL;
int i,k;


    path=getenv("LEGORT_BIN");
    path_loc = (char *) getcwd((char *) NULL, FILENAME_MAX+1);
    if( (path_loc==NULL)||(path==NULL) )
       {
       printf("Errori nei pathname [%s] [%s]\n",path,path_loc);
       return(0);
       }

    for(i=0;i<2;i++)
        proc_argv[i]=(char*)malloc(FILENAME_MAX+1);
#if defined UNIX
    for(i=0;i<7;i++)
       proc_envp[i]=(char*)malloc(FILENAME_MAX+1);
#endif
                                     
#if defined UNIX
    sprintf(proc_name,"%s/graphics\00",path);
    sprintf(proc_argv[0],"graphics\00");
    sprintf(proc_argv[1],"%s/f22circ\00",path_loc);
#endif
#if defined VMS
    sprintf(proc_name,"%sgraphics\00",path);
    sprintf(proc_argv[0],"graphics\00");
    sprintf(proc_argv[1],"%sf22circ\00",path_loc);
#endif
    proc_argv[2]=NULL;
#if defined UNIX
    sprintf(proc_envp[0],"DISPLAY=%s\00",getenv("DISPLAY"));
    sprintf(proc_envp[1],"SHR_USR_KEY=%s\00",getenv("SHR_USR_KEY"));
    sprintf(proc_envp[2],"SHR_TAV_KEY=%s\00",getenv("SHR_TAV_KEY"));
    sprintf(proc_envp[3],"LEGORT_BIN=%s\00",getenv("LEGORT_BIN"));
    sprintf(proc_envp[4],"HOME=%s\00",getenv("HOME"));
    sprintf(proc_envp[5],"DEBUG=%s\00",getenv("DEBUG"));
    sprintf(proc_envp[6], "LEGORT_UID=%s\00", getenv("LEGORT_UID"));
    proc_envp[7]=NULL;
#endif
#if defined VMS
    proc_envp[0]=NULL;
#endif
#ifndef SCO_UNIX
    pid_grafi = vfork();
#else
    pid_grafi = fork();
#endif
    stato_grafi=pid_grafi;
    if(stato_grafi==(-1))
         perror("vfork graphics");
    if ( stato_grafi == 0 )
      {
#ifndef SCO_UNIX
      sigsetmask(0);      /* sblocca tutti i segnali */
#endif
      execve(proc_name,proc_argv,proc_envp);
      perror("execve graphics");
      exit(0);
      }
    for(i=0;i<2;i++)
        free(proc_argv[i]);
#if defined UNIX
    for(i=0;i<7;i++)
       free(proc_envp[i]);
#endif
    free(path_loc);
}


/*** nuovi_parametri(w,risposta,call_data)
 ***  Parametri:
 ***    Widget w: widget
 ***    int risposta : YES o NO.
 ***    XmAnyCallbackStruct *call_data : non utilizzato.
 ***
 ***  Descrizione:
 ***    Callback chiamata premendo i Buttons nella dbox di conferma
 ***    cancellazione di un modulo.
 ***    Cancella i file del modulo e delle eventuali icone, aggiornando le
 ***    rispettive liste.
 ***/
void nuovi_parametri(w,risposta,call_data)
Widget w;
int risposta;
XmAnyCallbackStruct *call_data;
{
char comando[100];
char *msg;
char *msgcok;
char *msgcko;
int ret=0,i,j;
static Widget comunica=NULL;
Dialog_geometry geom;
typedef struct eliminazioni_st
        {
        int  num_canc;
        char *file[5];
        }ELIMI;
ELIMI canc[8]=
{
0, ""             , ""                 , ""                 , 
   ""                 , "",
2, "snapshot.dat" , "stato_cr.rtf"     , ""                 , 
   ""                 , "",
3, "backtrack.dat", "stato_cr.rtf"     , "perturbazioni.dat", 
   ""                 , "",
4, "snapshot.dat" , "backtrack.dat"    , "stato_cr.rtf"     , 
   "perturbazioni.dat", "",
2, "f22circ.dat"  , "perturbazioni.dat", ""                 , 
   ""                 , "",
4, "snapshot.dat" , "f22circ.dat"      , "stato_cr.rtf"     , 
   "perturbazioni.dat", "",
4, "backtrack.dat", "f22circ.dat"      , "stato_cr.rtf"     , 
   "perturbazioni.dat", "",
5, "snapshot.dat" , "backtrack.dat"    , "f22circ.dat"      , 
   "stato_cr.rtf"     , "perturbazioni.dat"
};

/*
printf("\t\t  errsked=%d,errdisp=%d,errshm=%d,errmonit=%d,errtotali=%d\n",
        errsked,errdisp,errshm,errmonit,errtotali);
*/
   if (risposta == CONTINUE)
      {
      if ( (errsked==errdisp) && (errsked==errmonit) )
         {
         ret = 0;
         msgcok = (char*)calloc(1,sizeof(char));
         msgcko = (char*)calloc(1,sizeof(char));
         for(j=0;j<canc[errsked].num_canc;j++)
            {
            ret = unlink(canc[errsked].file[j]);
            /* printf("%d) [%s] %d\n",j,canc[errsked].file[j],ret); */
            if( ret==0 )
               {
               msgcok = realloc(msgcok, 
                                strlen(msgcok)+strlen(canc[errsked].file[j])+4);
               sprintf(&msgcok[strlen(msgcok)],"-*- %s ",
                       canc[errsked].file[j]);
               }
            else
               {
               msgcko = realloc(msgcko, 
                                strlen(msgcko)+strlen(canc[errsked].file[j])+4);
               sprintf(&msgcko[strlen(msgcko)],"-%s- ",canc[errsked].file[j]);
               }
            }
         msg = calloc(strlen(msgcko)+strlen(msgcok),sizeof(char));
         strcpy(msg,"Deleted:\n");
         strcat(msg,msgcok);
         /*printf("\t\t\t lung=%d [%s]\n",strlen(msg),msg);*/
         if ( strlen(msgcko)>0 )
            {
            strcat(msg,"\n Not deleted\n");
            strcat(msg,msgcko);
            }
         geom.default_pos = 1;
         geom.height      = 200;
         Informa = (Widget)information (main_window_widget, msg,
                                        CREA|MAPPA, geom); 
         XtDestroyWidget(change_param);
         }
      }
   if (risposta == SHOW)
      {
      sprintf(comando,"xterm -T %s -e vi parametri.out &",PAREDF);
      system(comando);
      }
   if (risposta == EXIT)
      {
      XtDestroyWidget(change_param);
      system("killsim &");
      }
}
