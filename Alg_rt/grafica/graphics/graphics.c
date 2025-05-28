/**********************************************************************
*
*       C Source:               graphics.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Feb 22 17:29:31 2007 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: graphics.c-16 %  (%full_filespec: graphics.c-16:csrc:1 %)";
#endif
/*
   modulo graphics.c
   tipo 
   release 1.7
   data 2/9/96
   reserved @(#)graphics.c	1.7
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)graphics.c	1.7\t2/9/96";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <stdlib.h>                             
#include <string.h>                           
#include <math.h>     
#if defined AIX || defined UNIX
#include <sys/types.h>     
#endif
#if defined VMS
#include <types.h>
#endif
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/ScrollBar.h>
#include <Xm/MessageB.h>
#include <Mrm/MrmPublic.h>
#include <Xm/ToggleB.h> // <--- AGGIUNGI QUESTO HEADER
#include <Xm/List.h>         



#include "libutilx.h"
#include "uni_mis.h"
#include "grsf22.h"
#include "graphics.h"
#include "sim_param.h"
#include "f22_circ.h"
#include <Rt/RtMemory.h>

typedef char* caddr_t;

char bufford[50];

/***** #define NUM_VAR 6000 *****/

/*
 * le costanti definite di seguito devono corrispondere a quelle presenti
 * nel modulo UIL.
 */

#define k_form  		 1
#define k_popgraf                2
#define k_popmis                 3
#define k_nyi                    4
#define k_draw0                  5
#define k_draw1                  6
#define k_mis1                   7
#define k_tim1			 8
#define k_ord1			 9
#define k_draw2                 10
#define k_mis2                  11
#define k_tim2			12
#define k_ord2			13
#define k_val1			14
#define k_val2 			15
#define k_tempo 		16
#define k_selmis_dialog		20
#define k_toggle_mis1		21
#define k_toggle_mis2		22
#define k_toggle_mis3		23
#define k_toggle_mis4		24
#define k_selmis_box		25
#define k_selzoom_dialog        26
#define k_toggle_zoom1          27
#define k_toggle_zoom2          28
#define k_toggle_zoom3          29
#define k_toggle_zoom4          30
#define k_toggle_zoom5          31
#define k_umis_dialog     	32
#define k_toggle_umis1		33
#define k_toggle_umis2		34
#define k_toggle_umis3		35
#define k_toggle_umis4		36
#define k_toggle2_mis1		37
#define k_toggle2_mis2		38
#define k_toggle2_mis3		39
#define k_toggle2_mis4		40
#define k_fsca_dialog		41
#define k_toggle3_mis1		42
#define k_toggle3_mis2		43
#define k_toggle3_mis3		44
#define k_toggle3_mis4		45
#define k_text_max		46
#define k_text_min		47
#define k_toggle_man            48
#define k_toggle_auto           49
#define k_selgr_dialog          50
#define k_text_nomegr           51
#define k_list_gr               52
#define k_label_grmis1            53
#define k_label_grmis2            54
#define k_label_grmis3            55
#define k_label_grmis4            56
#define k_memgr_dialog            57
#define k_list_memgr		  58
#define k_text_memgr		  59
#define k_find_text		  65
#define k_find_next		  66
#define k_find_previous		  67
#define k_selmis_elimina	  68
#define k_defumis_dialog	  69
#define k_list_defumis		  70
#define k_toggle2_umis1		  71
#define k_toggle2_umis2           72
#define k_toggle2_umis3           73
#define k_toggle2_umis4           74
#define k_dir_dialog              75
#define k_text_dir1       	  76
#define k_text_dir2      	  77
#define k_text_dir3       	  78
#define k_text_dir4      	  79
#define k_toggle_dir1    	  80
#define k_toggle_dir2    	  81
#define k_toggle_dir3    	  82
#define k_toggle_dir4    	  83
#define k_grafici_menu_entry      84
#define k_misure_menu_entry       85
#define k_selmis_control_button	  86
#define k_fsca_control_button	  87
#define k_umis_control_button	  88
#define k_selgra_control_button	  89
#define k_max_widget             89

#define MAX_WIDGETS (k_max_widget + 1)
#define NUM_BOOLEAN_MIS  (k_toggle_mis4-k_toggle_mis1+1)
#define NUM_BOOLEAN_ZOOM (k_toggle_zoom5-k_toggle_zoom1+1)
#define NUM_BOOLEAN_UMIS  (k_toggle_umis4-k_toggle_umis1+1)
#define NUM_BOOLEAN_FSCA  (k_toggle_auto-k_toggle_man+1)
#define NUM_BOOLEAN_DIR  (k_toggle_dir4-k_toggle_dir1+1)
/*
 * Stringhe di errore utilizzate
 */
char * err_write_gruppi=
 "File per memorizzazione gruppi appartenente ad altro utente"; 
char * err_crea_gruppi=
 "Impossibile creare file per memorizzazione gruppi";
char * err_file_nonspec=
 "File non specificato";
char * err_file_nones=
 "File non esistente";
char * err_tfin_tiniz=
 "Tempo iniziale = tempo finale";


/*
 * Global data
 */

static Widget toplevel_widget,          /* Root widget ID of our */
                                        /* application. */
  main_window_widget,                   /* Root widget ID of main */
                                        /* MRM fetch */
  widget_array[MAX_WIDGETS];            /* Place to keep all other */
                                        /* widget IDs */
char file_vis[150];   /* nome del file in visualizzazione */
char *no_file_sel="NESSUN FILE GRAFICI SELEZIONATO";
char *path_22dat;
char path[NUM_PATH_FILES][FILENAME_MAX];

int resize;
int nofile=1;  /* indica che non e' stato ancora selezionato il file
                  dati */
int scala_unica; /* flag che indica che si desidera una unica scala
                    per le variabili   */
unsigned long colors[6];  /* colori privati per la visualizzazione 
                             dei grafici (su due piani) */
unsigned long plane_masks[2]; /* maschera dei piani */

float t_ultimo;  /* tempo trascorso dall'inizio della simulazione */
float t_old;  /* ultimo tempo letto */
int HC_on;

static int var_attiva;   /*  variabile su cui si sta operando */
int indice_gruppo;       /* indice gruppo che si sta selezionando */
int indice_umis;       /* indice unita' di misura che si sta selezionando */
static char toggle_mis[NUM_BOOLEAN_MIS];
static char toggle_zoom[NUM_BOOLEAN_ZOOM];
static char toggle_umis[NUM_BOOLEAN_UMIS];
static char toggle3_mis[NUM_BOOLEAN_MIS];
static char toggle_fsca[NUM_BOOLEAN_FSCA];
static char toggle2_umis[NUM_BOOLEAN_UMIS];
static char toggle_dir[NUM_BOOLEAN_DIR];


int umis_sel[4];   /* memorizzazione di appoggio durante la scelta
		      delle unita' di misura */
int num_umis;	   /* numero unita' di misura definite in uni_misc.dat*/
int *umis_defsel;
int autoscaling[4]; /* mem. di appoggio per i valori di fondo scala */
char *app_min[4];   /* stringhe di appoggio per settaggio minimi
                           e massimi   */
char *app_max[4]; 
S_MIN_MAX sel_min_max[4];

/*
 * Struttura che definisce lo stato di ogni grafico presente
 */
S_GRAFICO sg;
extern S_UNI_MIS uni_mis[];   /* tabella che descrive tutte le unita'
                                  di misura  */
/*
 * Dati globali necessari per le routines di grafica
 */
Display *display;
int screen_num;
XtIntervalId timer; /* ID del timer utilizzato per la temporizzazione
                       del refresh grafici */

GC gc[4];     /* e' presente un GC per ogni misura all'interno
                 del singolo gruppo  */
GC gc2[4];    /* GC per hard copy in b/n */
GC gc_copy;   /* GC per lo scroll con funzione di Copy Area */
GC gc_grid1;   /* GC per disegno griglia */
GC gc_grid2;
GC gc_zoom;
float tz_iniziale,tz_finale;


XFontStruct *font_info;

char *colori[]={"cyan","yellow","green","white"};
Dimension font_height;
Dimension font_width;
Dimension draw_width,draw_height; /* dimensionamento aree di disegno grafici */
Dimension ord_height;
float pix_step;  /* step in pixels tra un campione ed il successivo */
int num_camp[]={ NUM_CAMP_Z0,NUM_CAMP_Z1,NUM_CAMP_Z2,NUM_CAMP_Z3,NUM_CAMP_Z4 };

/*
 * variabili per la gestione di zoom e collimazione
 */

int freeza;
int stato_zoom;
int iniz_rect;  /* indica il primo disegno del rettangolo  */
#define ZOOM_END  0  /* fine modalita' di definizione rettangolo */
#define ZOOM_WAIT 1  /* in attesa del primo click del mouse */
#define ZOOM_DRAG 2  /* fase di definizione rettangolo tramite
                         dragging del mouse */
/*
  origine e dimensioni del rettangolo di selezione per zoom
*/
static XPoint origine;
static Dimension width,height;


Cursor cursor_coll; /* cursore per collimazione */
Cursor cursor_zoom; /* cursore per zoom  */
Cursor cursor_wait; /* cursore per attesa */
int collima;
int zoomord;
int tempo_sec; /* indica se si vogliono i tempi sull'asse delle
                  ascisse in secondi */
Widget wcollima;
Widget wzoomord;
Widget wtempo_sec;

XmString x_collima_on;
XmString x_zoomord_on;
XmString x_secondi_on;
XmString x_collima_off;
XmString x_zoomord_off;
XmString x_secondi_off;
int x_collima;
float t_iniziale;
float t_finale;
/******* S_DATI_SIM dato; *********/        /* Non utilizzato */
/*****
S_SEL_DATI bufdati[4*80*80];
****/
S_SEL_DATI *bufdati=NULL;

int n_last;              /* posizione all'interno del buffer del prossimo
                          * campione */
S_MIN_MAX *min_max=NULL; /* valori minimi e massimi per ogni variabile ex
                          * allocazione statica di NUM_VAR */

/*
 * dati per selezione variabili
 */
XmString *x_simboli;
XmString *x_nomi_misure;
XmString x_sel_var[4];

S_HEAD1 header1;
S_HEAD2 header2;
/********* char nomi_misure[NUM_VAR+1][LUN_NOME+1]; *********/
char **nomi_misure=NULL;  /* tabella dei nomi delle misure con terminatore 0 
                           * aggiunto.Vecchia allocazione statica di NUM_VAR */
char descr_vuota[LUN_SIMB+1];
char **simboli;           /* tabella dei simboli (allocata dinamicamente) */

/* per selezione gruppi */
int gruppo_selezionato = -1;
char lista_gruppi[NUM_GRUPPI][LUN_SIMB+1];
XmString x_gruppi[NUM_GRUPPI+1];
REC_GRUPPO gruppi[NUM_GRUPPI];


/* per selezione defaults delle unita' di misura */

XmString *x_codumis;  /* codici delle unita' di misura */

/*
 * Dati per bufferizzazione valori per disegno grafici
 */

XPoint z_ini,z_fin;
XPoint points[4][NUM_CAMP_Z0*INC_SEC];
XPoint *pp[4];
int npp[4];
XPoint *pp_coll;  /* per collimazione */


static MrmHierarchy s_RMHierarchy;     /* MRM database hierarchy ID */
static MrmType dummy_class;            /* and class variable. */
static char *filename_uid=        /* Mrm.heirachy file list. */
	   "graphics.uid";

char *widget_name[] = 
	{"",
	 "",
	 "popgraf",
	 "popmis",
         "nyi",
     "draw0",
     "draw1",
	 "mis1",
	 "tim1",
	 "ord1",
     "draw2",
	 "mis2",
	 "tim2",
	 "ord2",
	 "val1",
	 "val2",
	 "",
	 "",
	 "",
	 "",
	 "selmis_dialog",
	 "toggle_mis1",
	 "toggle_mis2",
	 "toggle_mis3",
	 "toggle_mis4",
	 "selmis_box",
	 "selzoom_dialog",
	 "toggle_zoom1",
	 "toggle_zoom2",
	 "toggle_zoom3",
	 "toggle_zoom4",
	 "toggle_zoom5",
	 "umis_dialog",
	 "toggle_umis1",
	 "toggle_umis2",
	 "toggle_umis3",
	 "toggle_umis4",
	 "toggle2_mis1",
	 "toggle2_mis2",
	 "toggle2_mis3",
	 "toggle2_mis4",
	 "fsca_dialog",
	 "toggle3_mis1",
	 "toggle3_mis2",
	 "toggle3_mis3",
	 "toggle3_mis4",
	 "text_max",
	 "text_min",
	 "toggle_man", 
	 "toggle_auto",
	 "selgr_dialog",
	 "",
	 "",
	 "",
	 "",
	 "",
	 "",
	 "memgr_dialog",
	 "list_memgr",
	 "text_memgr",
	 "",
	 "",
	 "",
	 "",
	 "",
	 "",
	 "",
	 "",
	 "",
	 "defumis_dialog",
         "",
         "",
         "",
         "",
         "",
         "dir_dialog",
         };

static int db_filename_num = 1;

/*
 * Forward declarations
 */

static void s_error();

static void activate_proc();
static void collima_proc();
static void zoomord_proc();
static void secondi_proc();
static void memcollima_proc();
static void memzoomord_proc();
static void memsecondi_proc();
static void zoom_proc();
static void map_proc();
static void PostIt();
static void MoveMouse();
static void draw_proc(Widget , int *, XmDrawingAreaCallbackStruct *);
static void resize_proc();
static void create_proc();
static void quit_proc();
static void toggle_proc();
static void ok_proc();
static void cancel_proc();
static void apply_proc();
static void timer_proc();
static void find_proc();
static void HC_proc();
int cerca_umis(char*);
static int init_application();
Widget WidAttenzione(Widget,char*,int);
void d2free(char**);
extern int set_scala(int);
static int set_ordinate(int);
static void set_scala_unica();
static void crea_sfondo(Widget,Dimension,Dimension);
static void formatta(char*,float);
static int x_cerca_stringa(XmString,XmString*);
static int cerca_nome(char*);
static int converti_tempoGR(float,long  *,long  *,long  *,long  *,long  *,long  *);
static int AnnoBisGR(long anno);
static  int prep_draw(float,float,S_MIN_MAX *);
static  int draw_grid(Window);
extern void set_cur_wait();
void clr_cur_waitGR();
static int zoomed(XPoint,XPoint);
extern int open_gruppiGR();
extern int read_gruppiGR(int);
static void crea_lista_umis();
static  void handle_motion(Widget);
extern void agg_umis();
static void free_lista_umis();
extern int write_gruppo(int);
extern   void close_path();
extern Widget WidErrore(Widget,char*,int);

void init_gcs();
void load_file_header(char *nome_file);
void load_variablesGR(int num_nomi,char*nome);
int open_22dat_circGR();
int read_22dat_circGR(char flag);
void close_22dat_circ();
int cerca_stringa(char *stringa,char** lista);
void load_font(XFontStruct **font_info);
void prep_str_tim(float,float);
void abilita_menu_selez(int);
void abilita_menu(int flag);
void reload_f22();

/* The names and addresses of things that DwtDrm.has to bind.  The names do
 * not have to be in alphabetical order.  */

static MRMRegisterArg reglist[] = {
    {"activate_proc", (caddr_t) activate_proc}, 
    {"map_proc", (caddr_t) map_proc}, 
    {"PostIt", (caddr_t) PostIt}, 
    {"MoveMouse", (caddr_t) MoveMouse},
    {"collima_proc",(caddr_t) collima_proc},
    {"zoomord_proc",(caddr_t) zoomord_proc},
    {"secondi_proc",(caddr_t) secondi_proc},
    {"memcollima_proc",(caddr_t) memcollima_proc},
    {"memzoomord_proc",(caddr_t) memzoomord_proc},
    {"memsecondi_proc",(caddr_t) memsecondi_proc},
    {"zoom_proc",(caddr_t) zoom_proc},
    {"draw_proc", (caddr_t) draw_proc}, 
    {"resize_proc",(caddr_t) resize_proc},
    {"create_proc", (caddr_t) create_proc}, 
    {"quit_proc", (caddr_t) quit_proc}, 
    {"toggle_proc", (caddr_t) toggle_proc}, 
    {"ok_proc", (caddr_t) ok_proc}, 
    {"cancel_proc", (caddr_t) cancel_proc}, 
    {"apply_proc", (caddr_t) apply_proc}, 
    {"find_proc", (caddr_t) find_proc}, 
    {"HC_proc", (caddr_t) HC_proc}, 
};


static int reglist_num = (sizeof reglist / sizeof reglist [0]);
static int font_unit = 400;


/*
        variabili per la nuova gestione del file circolare
*/
PUNT_FILE_F22 file_f22;
char *nome_file_f22;


char *getenv();

int    _MAX_SNAP_SHOT;
int    _MAX_BACK_TRACK;
int    _MAX_CAMPIONI;
int    _NUM_VAR;
int    _MAX_PERTUR;
int    _SPARE_SNAP;
int    _PERT_CLEAR;
int RileggiF22Par=1;

/*
 * OS transfer point.  The main routine does all the one-time setup and
 * then calls XtMainLoop.
 */
char *path_uid;







int main(argc, argv)
    unsigned int argc;                  /* Command line argument count. */
    char *argv[];                       /* Pointers to command line args. */
{
int i;
char name_uid[200];
name_uid[0]=0;

/* redirezione output  e stampa versione */
/*
testata("graphics",SccsID);
*/

MrmInitialize();                 /* Initialize MRM before initializing
                                        /* the X Toolkit. */
toplevel_widget = XtInitialize("graphics", 
                                        /* Main window banner text. */
      "Grafics",                        /* Root class name. */
      NULL,                             /* No option list. */
      0,                                /* Number of options. */
      &argc,                            /* Address of argc */
      argv);                            /* argv */
/*
 Gestione dell'argomento addizionale: nome del file  nomi variabili
*/
if (argc > 1)
	{
	if(argc>7)
		{
		printf("\n Errore command line:");
		printf("\n Uso corretto:  graphics path_file nome_var");
		exit(0);
		}
/*
	printf("\n numero argomenti = %d",argc);
	for(i=1;i<argc;i++)
		printf("\n argomento = %s",argv[i]);
*/
	}
/* Open the UID files (the output of the UIL compiler) in the hierarchy*/
#if defined (VMS)
path_uid=getenv("LEGORT_UID");
strcpy(name_uid,path_uid);
strcat(name_uid,filename_uid);
#else
path_uid=getenv("LEGORT_UID");
strcpy(name_uid,path_uid);
strcat(name_uid,"/");
strcat(name_uid,filename_uid);
#endif
path_uid=name_uid;
if (MrmOpenHierarchy(db_filename_num, /* Number of files. */
      &path_uid,                    /* Array of file names.  */
      NULL,                               /* Default OS extenstion. */
      &s_RMHierarchy)                   /* Pointer to returned MRM ID */
      != MrmSUCCESS)
        s_error("\ncan't open hierarchy");
//  init_application();
/*
 si posiziona nella directory dei defaults ($HOME/defaults) creandola
 se non esiste
*/
chdefaults();

/* per selezione defaults delle unita' di misura */
init_umis();
num_umis=cerca_num_umis();
umis_defsel= (int*) calloc (num_umis, sizeof (int));
x_codumis=(XmString *)XtCalloc(num_umis+1, sizeof(XmString));

void open_path();
/*
 *  Ricava i valori delle variabili necessarie per la creazione
 *  dei Grafic Context (GC)
 */
display=XtDisplay(toplevel_widget);
screen_num=DefaultScreen(display);
init_gcs();

/* Register the items MRM needs to bind for us. */

MrmRegisterNames(reglist, reglist_num);

if (MrmFetchWidget(s_RMHierarchy, "S_MAIN_WINDOW", toplevel_widget,
      &main_window_widget, &dummy_class) != MrmSUCCESS)
        s_error("can't fetch main window");

/*
  Creazione pixmap per griglia di riferimento
*/

cursor_wait=XCreateFontCursor(display,XC_watch);

/*
 *  Ricava i valori delle variabili necessarie per la creazione
 *  dei Grafic Context (GC)
 */
display=XtDisplay(main_window_widget);
screen_num=DefaultScreen(display);

/* 
 Manage the main part and realize everything.  The interface comes up
 on the display now.
 */
XtManageChild(main_window_widget);
XtRealizeWidget(toplevel_widget);

/* 
 crea le message boxes per visualizzazione errore e visualizzazione
 warning
 */
/*
WidAttenzione(main_window_widget,"",CREA);
WidErrore(main_window_widget,"",CREA);
*/
/*
 disabilita i menu di grafici e misure
 */
abilita_menu(1);
/*
  se vi sono argomenti addizionali significa che e' stato specificato
  il nome del file di grafica
*/
if(argc>1) 
	{
	if(strcmp(argv[1],"-scala")==0)
		scala_unica=1;
	load_file_header(argv[1+scala_unica]);
/*
 Se il file specificato esiste 
 */
	if(!nofile)
		load_variablesGR(argc-(2+scala_unica),argv[2+scala_unica]);
	}
XtMainLoop();
}



void load_file_header(char *nome_file)
//char *nome_file;
{
int flag;

path_22dat=XtMalloc(strlen(nome_file)+1);
strcpy(path_22dat,nome_file);
strcpy(file_vis,path_22dat);
if(open_22dat_circGR())
      {
      WidAttenzione(main_window_widget,err_file_nones,MAPPA);
      nofile=1;
      strcpy(file_vis,no_file_sel);
      return; /* esce se errore in apertura */
      }
nofile=0;
flag=TUTTI;
if(read_22dat_circGR(flag)==1)  /* lettura dell'header */
                {
                close_22dat_circ();
                open_22dat_circGR();
                if(read_22dat_circGR(flag)==1)
                        {
/*
   file non esistente
 */
                        close_22dat_circ();
                        XtFree(path_22dat);
                        widget_array[k_selmis_dialog]=0;
                        strcpy(file_vis,no_file_sel);
                        nofile=1;
                        }
                }
abilita_menu(nofile);

	

	
}

void load_variablesGR(int num_nomi,char*nome)
//int num_nomi;
//char *nome[];
{
int i,j,flag;
int indice;
int loaded=0; /* flag per indicare se e' stata caricata 
                 almeno una variabile */
S_GRAFICO *s;
s= &sg;
for(i=0;i<4;i++)
	{
/*
	cerca la stringa nella lista determinandone l'indice
*/
	if(i<num_nomi)
		{
// GUAG2025
// indice=cerca_stringa(nome[i],simboli);
		indice=cerca_stringa(&nome[i],simboli);
		}
	else
		indice= -1;
	if(indice==-1)
		{
                XmStringFree(x_sel_var[i]);
		x_sel_var[i]=XmStringCreateLtoR(descr_vuota,
				XmSTRING_DEFAULT_CHARSET);
		s->ind_mis[i]=(-1);
		}
	else
		{
                XmStringFree(x_sel_var[i]);
		x_sel_var[i]=XmStringCreateLtoR(simboli[indice],
				XmSTRING_DEFAULT_CHARSET);
#if defined VMS_FORTRAN
		s->ind_mis[i]=cerca_nome(simboli[indice]);
#else
		s->ind_mis[i]=indice;
#endif
		}
        XmStringFree(s->x_descr_mis[i]);
        s->x_descr_mis[i] = XmStringCopy(x_sel_var[i]);
	if(s->ind_mis[i]!=-1)
		{
		loaded=1;
		strcpy(s->descr_mis[i],simboli[indice]);
		(s->ind_umis[i])=cerca_umis(s->descr_mis[i]);
		if(s->ind_umis[i]==-1)
			{
// GUAG2025 s->ind_umis[i]=num_umis-1;
			s->ind_umis[i]=0; // Usa prima unità invece dell'ultima
            printf("AVVISO: load_variablesGR: unità di misura non trovata per variabile %d, uso default\n", i);			
			} 

			// Verifica che l'unità di misura sia inizializzata
	if(uni_mis[s->ind_umis[i]].A[s->umis_sel[i]] == 0 && 
   		uni_mis[s->ind_umis[i]].B[s->umis_sel[i]] == 0) {
   		 printf("ERRORE: load_variablesGR: unità di misura non inizializzata correttamente\n");
    // Imposta valori di default
   		 uni_mis[s->ind_umis[i]].A[s->umis_sel[i]] = 1.0;
    		uni_mis[s->ind_umis[i]].B[s->umis_sel[i]] = 0.0;
	}
		s->autoscaling[i]=1;
/*
  e' settato l'autoscaling: forza la ricerca di minimo e massimo
  modificando il valore del massimo.
*/
		s->sel_min_max[i].max=min_max[s->ind_mis[i]].max+1;
		s->umis_sel[i]=uni_mis[s->ind_umis[i]].sel;
		set_scala(i); 
                s->fix_min_max[i].max=(-1);
                s->fix_min_max[i].min=(-1);
		}
	else /* caso di misura non valida o eliminata */
		{
		strcpy(s->descr_mis[i],descr_vuota);
		set_ordinate(i); /* setta a blank le
                                           ordinate visualizzate */
		}
	}
if(scala_unica && loaded) {
if(open_22dat_circGR())
      {
      WidAttenzione(main_window_widget,err_file_nones,MAPPA);
      nofile=1;
      strcpy(file_vis,no_file_sel);
      return; /* esce se errore in apertura */
      }
flag = AGGIORNA;
if(read_22dat_circGR(flag)==1)  /* legge tutti i dati dall'inizio del file */
                {
                close_22dat_circ();
                open_22dat_circGR();
                if(read_22dat_circGR(flag)==1)
                        {
/*
   file non esistente
 */
                        close_22dat_circ();
                        XtFree(path_22dat);
                        widget_array[k_selmis_dialog]=0;
                        strcpy(file_vis,no_file_sel);
                        nofile=1;
                        }
                }
        set_scala_unica();
}
t_old=0.0;
timer=XtAppAddTimeOut(XtWidgetToApplicationContext(main_window_widget),(unsigned long)1,timer_proc,NULL);
XClearArea(display,XtWindow(s->w_mis),0,0,0,0,True);
}





void crea_sfondo(w,width,height)
Widget w;
Dimension width,height;
{
Pixmap stip;
width=(width%6)? width/6+1 : width/6;
height=(height%4)? height/4+1 : height/4;

if((stip=XCreatePixmap(display,
            RootWindow(display,screen_num),
            width,height,
            DefaultDepth(display,screen_num)))== False)
        {
        printf("\n Errore creazione pixmap per sfondo");
        exit(1);
        }
if(HC_on)
	{
	XFillRectangle(display,stip,gc_grid2,0,0,width,height);
	}
else
	{
	XFillRectangle(display,stip,gc_grid1,0,0,width,height);
	XDrawRectangle(display,stip,gc2[0],0,0,width,height);
	}
set_something(w,XmNbackgroundPixmap,(char*)stip);
}

/*
 * One-time initialization of application data structures.
 */

static int init_application()
{
int k,i;
nofile=1;
strcpy(file_vis,no_file_sel);
for(k=0;k<LUN_SIMB/2;k++)
	descr_vuota[k]=' ';
descr_vuota[LUN_SIMB/2]=0;
for (k = 0; k < MAX_WIDGETS; k++)
        widget_array[k] = NULL;
/* 
 Inizializza  gli indici in lista alle variabili visualizzate 
*/
for(k=0;k<4;k++)
	{
	sg.ind_mis[k]=(int)-1;
	sg.autoscaling[k]=1;
        strcpy(sg.descr_mis[k],descr_vuota);
	}
sg.zoom=0;
x_zoomord_on=XmStringCreateLtoR("Disabilita zoom ordinate",XmSTRING_DEFAULT_CHARSET);
x_zoomord_off=XmStringCreateLtoR("Abilita zoom ordinate",XmSTRING_DEFAULT_CHARSET);
x_collima_on=XmStringCreateLtoR("Disabilita collimazione",XmSTRING_DEFAULT_CHARSET);
x_collima_off=XmStringCreateLtoR("Abilita collimazione",XmSTRING_DEFAULT_CHARSET);
x_secondi_on=XmStringCreateLtoR("Abilita tempo in secondi",XmSTRING_DEFAULT_CHARSET);
x_secondi_off=XmStringCreateLtoR("Disabilita tempo in secondi",XmSTRING_DEFAULT_CHARSET);
}

/*
 * Inizializzazione dei GC utilizzati per le misure
 */
void init_gcs()
{
XGCValues values;
XColor color,excolor;
XColor exact_defs[12];
Colormap default_cmap;
Pixel pixel;

Boolean dynamic_colors=False;	/*to check if dynamic colors are supported*/
unsigned int line_width = 0;

static int line_style2[] = {LineSolid,LineOnOffDash,
                            LineOnOffDash,LineSolid};

static int line_style[] = {LineSolid,LineSolid,LineSolid,LineSolid};
int cap_style = CapRound;
int join_style = JoinRound;
static int dash_offset[] = {0,0,0,5};
static char dash_list[5][2] ={ {0,0},{1,1},{2,2},{0,0},{10,10}};
int list_lenght = 2;
int i;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCCapStyle | GCJoinStyle |
                           GCFunction;

load_font(&font_info);
font_height= font_info->ascent + font_info->descent;
font_width= font_info->max_bounds.width;
values.function=GXcopy;
values.line_width = line_width;
values.cap_style = cap_style;
values.join_style = join_style;
values.background = WhitePixel(display,screen_num);

/*
Inizio di una parte differenziata.In SCO_UNIX viene fatta una
allocazione dei colori in read/only
*/
default_cmap=DefaultColormap(display,screen_num);
/* 
Verifica se e' possibile allocare i colori dinamici.
*/

if(XAllocColorCells(display,default_cmap,False,plane_masks,1,colors,6)== 0)
	{
	/*Restituito NULL: impossibile allocare color dyn */
	printf("\n spazio per allocazione colormap esaurito\n ");
	printf("Impossibile allocare i colori dinamici\n");
	dynamic_colors=False;	
	}
else
	{
	dynamic_colors=True;
	}

for(i=0;i<4;i++)
   {
   XParseColor(display,
	    default_cmap,
            colori[i],&exact_defs[i]);
   exact_defs[i].flags=DoRed | DoGreen | DoBlue;

/*
in Sco alloca i colori  statici read only
*/
if(dynamic_colors==False)
	{
	if(!XAllocColor(display,default_cmap,&exact_defs[i]))
       	    {
       	    printf("Impossibile allocare il colore statico %d\n",i);
       	    exit(1);
      	     }
	}/*end if dyn*/
   }/*end for*/

XParseColor(display,
	    default_cmap,
            "dark slate gray",&exact_defs[10]);
exact_defs[10].flags=DoRed | DoGreen | DoBlue;

if(dynamic_colors==False)
	{
	if(!XAllocColor(display,default_cmap,&exact_defs[10]))
		{
                 printf("Impossibile allocare il colore statico  10\n");
          	 exit(1);
          	 }
	}

XParseColor(display,
	    default_cmap,
            "black",&exact_defs[4]);
exact_defs[4].flags=DoRed | DoGreen | DoBlue;
if(dynamic_colors==False)
	{
	if(!XAllocColor(display,default_cmap,&exact_defs[4]))
	           {
	      	    printf("Impossibile allocare il colore 4\n");
	           exit(1);
	           }
	}

exact_defs[5]=exact_defs[4]; 

if(dynamic_colors==True)
	{
	exact_defs[0].pixel=colors[0];
	exact_defs[1].pixel=colors[1];
	exact_defs[2].pixel=colors[2];
	exact_defs[3].pixel=colors[3];
	exact_defs[4].pixel=colors[4];  /*nero*/
	exact_defs[5].pixel=colors[5];  /*nero */

	exact_defs[6]=exact_defs[0];
	exact_defs[7]=exact_defs[1];
	exact_defs[8]=exact_defs[2];
	exact_defs[9]=exact_defs[3];
	exact_defs[11]=exact_defs[5];

	exact_defs[6].pixel=colors[0] | plane_masks[0];
	exact_defs[7].pixel=colors[1]| plane_masks[0];
	exact_defs[8].pixel=colors[2]| plane_masks[0];
	exact_defs[9].pixel=colors[3]| plane_masks[0];
	exact_defs[10].pixel=colors[4]| plane_masks[0];
	exact_defs[11].pixel=colors[5]| plane_masks[0];

	XStoreColors(display,default_cmap,exact_defs,12);
	}/*end if (dynamic_colors==True)*/
/*
Da qui in poi la differenza fra colori dinamici e statici non importa
*/

for(i=0;i<4;i++)
   {
   values.line_style = line_style[i];
   values.foreground = exact_defs[i].pixel;
   values.background = exact_defs[5].pixel;

   gc[i]= XCreateGC(display,RootWindow(display,screen_num), valuemask, &values);
   
XSetPlaneMask(display,gc[i],~plane_masks[0]);

/*
gc2 : utilizzato per hard copy
*/
   values.foreground = BlackPixel(display,screen_num);
   values.line_style = line_style2[i];
   if(i==3)
        values.line_width = 2;
   gc2[i]=XCreateGC(display,RootWindow(display,screen_num), valuemask,
             &values);
   if(line_style2[i]==LineOnOffDash)
      {
      XSetDashes(display,gc2[i],dash_offset[i],dash_list[i],list_lenght);
      }
   XSetFont(display,gc[i],font_info->fid);
   }
XSetFont(display,gc2[0],font_info->fid);
values.line_style = LineSolid;
values.line_width = 0;
values.foreground =exact_defs[10].pixel; 
values.background = exact_defs[5].pixel;

gc_grid1=XCreateGC(display,RootWindow(display,screen_num), valuemask,
                  &values);
XSetFillStyle(display,gc_grid1,FillSolid);
values.foreground = WhitePixel(display,screen_num);
gc_grid2=XCreateGC(display,RootWindow(display,screen_num), valuemask,
                  &values);
XSetFillStyle(display,gc_grid2,FillSolid);

/*
  GC per disegno rettangolo di zoom.
*/
values.function=GXxor;
values.line_style = line_style[0];
values.background = exact_defs[5].pixel;
values.foreground=WhitePixel(display,screen_num)| exact_defs[4].pixel;
gc_zoom=XCreateGC(display,RootWindow(display,screen_num),valuemask,
&values);
}

/***************************************************************************
 *
 * Utilities utilizzate in fase di inizializzazione.
 */

void load_font(XFontStruct **font_info)
//XFontStruct **font_info;
{
char *font_name = "fixed";
/* Carica il font ottenendo la descrizione del font stesso */
if((*font_info = XLoadQueryFont(display,font_name)) == NULL)
	{
	s_error("\nCannot open font");
	}
}



/*
 *  set_scala
 *      setta i nuovi valori di fondo scala se necessario
 *      torna 0 se non viene effettuato cambiamento di scala
 *      torna 1 se e' stato effettuato cambiamento di scala
 */

int set_scala(int indice)
//int indice;   indice della variabile all'interno del gruppo
//				 (da 0 a 3)    */
{
S_GRAFICO *s;
s=(&sg);

if(s->autoscaling[indice])
	{
	if(memcmp(&s->sel_min_max[indice],&min_max[s->ind_mis[indice]],sizeof(S_MIN_MAX)))
		{
		s->sel_min_max[indice]=min_max[s->ind_mis[indice]];
		if(s->sel_min_max[indice].max == s->sel_min_max[indice].min)
			s->fatt_y[indice]=1;
		else
			s->fatt_y[indice]=draw_height/
                        (s->sel_min_max[indice].max-s->sel_min_max[indice].min);
/*
   Modifica i valori di fondo scala nel buffer contenente le stringhe 
   delle ordinate
*/
		set_ordinate(indice);
		return(1);
		}
	return(0);
	}
else  /* caso di scala fissa determinata da utente   */
	{
	s->sel_min_max[indice].max=s->fix_min_max[indice].max;
	s->sel_min_max[indice].min=s->fix_min_max[indice].min;
	if(s->sel_min_max[indice].max == s->sel_min_max[indice].min)
		s->fatt_y[indice]=1;
	else
		s->fatt_y[indice]=draw_height/
                                  (s->sel_min_max[indice].max-
                                   s->sel_min_max[indice].min);
	set_ordinate(indice);
	return(1);
	}
}

/*
 set_scala_unica
    setta il valore di fondo scala in modo fisso (non autoscaling)
    in base al massimo e minimo di tutte le variabili in
    visualizzazione;
    viene calcolato il massimo dei massimi ed il minimo dei minimi;
    max_max, min_min); viene assunto come massimo
		 max_max+|max_max-min_min|*0.1
    come minimo:
		 min_min-|max_max-min_min|*0.1
*/
void set_scala_unica()
{
int i;
float max_max,min_min;
float delta;
S_GRAFICO *s;
s=(&sg);
max_max=(-1.0E-37);
min_min=(1.0E+38);
for(i=0;i<4;i++)
	{
/*
 annulla se presente l'autoscaling
*/
	s->autoscaling[i]=0;
	if(s->ind_mis[i]!=-1)
		{
		if(max_max < min_max[s->ind_mis[i]].max)
			max_max=min_max[s->ind_mis[i]].max;
		if(min_min > min_max[s->ind_mis[i]].min)
			min_min=min_max[s->ind_mis[i]].min;
		}
	}
delta=fabs(max_max-min_min)*0.1;
/*
 corregge i valori di massimo e minimo aggiungendovi il 10% della
 differenza tra massimo e minimo
*/
max_max+=delta;
min_min-=delta;
for(i=0;i<4;i++)
	{
	s->fix_min_max[i].max=max_max;
	s->fix_min_max[i].min=min_min;
	s->sel_min_max[i]=s->fix_min_max[i];
        if(s->sel_min_max[i].max == s->sel_min_max[i].min)
                s->fatt_y[i]=1;
        else
                s->fatt_y[i]=draw_height/
                                  (s->sel_min_max[i].max-
                                   s->sel_min_max[i].min);
        set_ordinate(i);
	}
}

/*
 *  set_ordinate
 *    setta il valore delle ordinate che vengono visualizzate
 *    in modo conforme ai valori di minimo e massimo
 */

// 2. Aggiungere controlli in set_ordinate()
int set_ordinate(int ind)
{
    int uguali,ord_scritta;
    int lun;
    float delta;
    float ord;
    float min; /* minimo nell'unita' di misura prescelta */
    float max; /* massimo nell'unita' di misura prescelta */
    int i,j;
    S_GRAFICO *s;
    s=(&sg);

    if(s->ind_mis[ind]==-1)
    {
        for(i=0;i<5;i++)
            sprintf(s->str_ord[i][ind],"         ");
        return(1);
    }
    
    // Verifica che l'unità di misura sia valida
    if(s->ind_umis[ind] < 0 || s->ind_umis[ind] >= num_umis) {
        printf("ERRORE: ind_umis[%d] = %d non valido\n", ind, s->ind_umis[ind]);
        s->ind_umis[ind] = 0; // Default all'unità di misura 0
    }
    
    // Verifica che umis_sel sia valido
    if(s->umis_sel[ind] < 0 || s->umis_sel[ind] >= 4) {
        printf("ERRORE: umis_sel[%d] = %d non valido\n", ind, s->umis_sel[ind]);
        s->umis_sel[ind] = 0;
    }
    
    // Debug: stampa i valori prima della conversione
    printf("DEBUG set_ordinate(%d):\n", ind);
    printf("  sel_min_max: min=%f, max=%f\n", 
           s->sel_min_max[ind].min, s->sel_min_max[ind].max);
    printf("  ind_umis=%d, umis_sel=%d\n", 
           s->ind_umis[ind], s->umis_sel[ind]);
    printf("  A=%f, B=%f\n", 
           uni_mis[s->ind_umis[ind]].A[s->umis_sel[ind]],
           uni_mis[s->ind_umis[ind]].B[s->umis_sel[ind]]);
    
    // [Resto del codice esistente con le conversioni...]
    
    min=s->sel_min_max[ind].min *
         uni_mis[s->ind_umis[ind]].A[s->umis_sel[ind]]+
         uni_mis[s->ind_umis[ind]].B[s->umis_sel[ind]];
    max=s->sel_min_max[ind].max *
         uni_mis[s->ind_umis[ind]].A[s->umis_sel[ind]]+
         uni_mis[s->ind_umis[ind]].B[s->umis_sel[ind]];
    
    printf("  min convertito=%f, max convertito=%f\n", min, max);
    
    // Controllo valori validi
    if(min == max) {
        // Se min e max sono uguali, crea un range artificiale
        if(min == 0.0) {
            min = -1.0;
            max = 1.0;
        } else {
            float delta_range = fabs(min) * 0.1;
            if(delta_range < 0.1) delta_range = 0.1;
            min -= delta_range;
            max += delta_range;
        }
    }
    
    delta=(max-min)/4.0;
    ord=max;
    for(i=0;i<5;i++)
    {
//        sprintf(bufford, "%s", sg.str_ord[i][ind]);
        strcpy(bufford, "CiaoCiao");
        
//        printf("set_ordinate DEBUG - prima di formatta - bufford=%s, ord=%f\n",bufford,ord);
//        formatta(bufford,ord);
//        printf("set_ordinate DEBUG - DOPO formatta - bufford=%s, ord=%f\n",bufford,ord);

//        printf("set_ordinate DEBUG - prima di formatta - s->str_ord[i][ind]=%s, ord=%f\n",s->str_ord[i][ind],ord);
        formatta(s->str_ord[i][ind],ord);
//        printf("set_ordinate DEBUG - DOPO formatta - s->str_ord[i][ind]=%s, ord=%f\n",s->str_ord[i][ind],ord);

//        formatta(s->str_ord[i][ind],ord);
        ord-=delta;
    }
    
    return(0);
}
             

                  
                   

/*
 * Ricerca di una compound string all'interno di una lista di compound strings
 * (la lista deve essere terminata da un NULL).
 */

int x_cerca_stringa(XmString x_stringa,XmString*x_lista)
//XmString x_stringa;
//XmString *x_lista;
{
int i=0;
while(x_lista[i]!=NULL)
	{
	if(XmStringByteCompare(x_stringa,x_lista[i]))
		return(i);
	i++;
	}
return(-1);
}

int cerca_stringa(char *stringa,char** lista)
//char *stringa;
//char *lista[];
{
int i=0;
while(lista[i]!=NULL && i<header2.ncasi)
	{
	if(Utstrstr(lista[i],stringa))
		return(i);
	i++;
	}
return(-1);
}




int cerca_nome(stringa)
char *stringa;
{
int i;
i=0;
while(nomi_misure[i][0]!=0 && i<header1.nvar)
	{
#if defined VMS_FORTRAN
/*
 Cerca la posizione nella lista dei nomi che non coincide
 con la lista dei simboli
*/
        if(memcmp(stringa,nomi_misure[i],LUN_NOME)==0)
#else
        if(memcmp(stringa,simboli[i],LUN_NOME)==0)
#endif
		{
		return(i);
		}
	i++;
	}
return(-1);
}

/*
 * All errors are fatal.
 */

static void s_error(problem_string)
    char *problem_string;
{
    printf("%s\n", problem_string);
    exit(0);
}



/***************************************************************************
 *
 * This section contains callback routines.
 */

/*
 * Funzione di redraw per le window di visualizzazione trend grafici
 */


char str_tim[7][30];
char str_tims[7][30];

static void timer_proc(client_data,id)
caddr_t client_data;
XtIntervalId *id;
{
int ultimo;
int iumis,iumis_sel;
int i,j,k;
int numis,selumis;
int zoom;
int y_line,x_line;
float f_pix;
int x_pix,x_pixprec;  /* posizione in pixel del tempo sull-asse del
le ascisse */
int y_pix;  /* posizione in pixel delle ordinate */
int indice; /* indice della misura da visualizzare */
int ind_buf;
/*
 Controllo necessario perche' un messaggio di timer puo' essere
 gia' in coda quando l'utente passa in zoom. In questo caso la 
 zoom_proc pur eliminando il timer non elimina il messaggio gia'
 in coda.
*/
if(freeza)
	{
	timer=0;
	return;
	}
read_22dat_circGR(AGGIORNA);
/*
 legge tempo finale e tempo iniziale dal buffer dei dati
*/
t_finale=bufdati[n_last].t;
t_iniziale=bufdati[0].t;
/**
printf("timer_proc Tempo finale=%f tempo_iniziale=%f n_last=%d\n",
        t_finale,t_iniziale,n_last);
**/
if(t_finale==t_iniziale)
        {
	timer=
        XtAppAddTimeOut(XtWidgetToApplicationContext(main_window_widget),
                                   (unsigned long)4000,timer_proc,NULL);
        return;
/**
	WidErrore(main_window_widget,err_tfin_tiniz,MAPPA);
	s_error("t_finale==t_iniziale");
        exit(0);
**/
        }

t_ultimo=t_finale;
/*
 se non c'e' un nuovo campione ricarica il timer ed esce
*/
if(t_old==t_finale)
        {
	timer=
        XtAppAddTimeOut(XtWidgetToApplicationContext(main_window_widget),
                                   (unsigned long)4000,timer_proc,NULL);
        return;
        }
else t_old=t_finale;

/*
 prepara le stringhe da visualizzare sull'asse dei tempi
*/
prep_str_tim(t_iniziale,t_finale);
for(i=0;i<4;i++)
        {
        if(/*sg.autoscaling[i] &&*/ sg.ind_mis[i]!=-1)
                set_scala(i);
        if(sg.ind_mis[i]!=-1)
                {
                sg.ultimo[i]=bufdati[n_last].mis[i];
                }
        }
/* 
 inserisce nelle stringhe della scala dei tempi i valori calcolati
 in base all'ultimo tempo acquisito 
*/
prep_str_tim(t_iniziale,t_finale);

if(collima)
        {
        t_ultimo=x_collima*(t_finale-t_iniziale)/draw_width+t_iniziale;
        }
/*
 aggiorna il valore corrente dei dati visualizzati (nel caso di collimazione
 calcola il valore aggiornato del punto in stato di collimaz.).
*/
for(i=0;i<4;i++)
        {
        if(sg.ind_mis[i]!=-1)
                {
                if(collima)
                        {
                        pp_coll=points[i];
                        while(pp_coll->x<x_collima)
                                pp_coll++;
			sg.ultimo[i]=sg.sel_min_max[i].max-
                                        pp_coll->y/sg.fatt_y[i];
                        }
                iumis=sg.ind_umis[i];
                iumis_sel=sg.umis_sel[i];
                sg.ultimo[i]=sg.ultimo[i] *
                                uni_mis[iumis].A[iumis_sel]+
                                uni_mis[iumis].B[iumis_sel];
                }
        }

XClearArea(display,XtWindow(sg.w_draw),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_ord),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_val),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_tim),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_mis),0,0,0,0,True);
XClearArea(display,XtWindow(widget_array[k_tempo]),0,0,0,0,True);

//GUAG2025 
    // Forza il redraw chiamando direttamente draw_proc
    if(sg.w_ord && XtWindow(sg.w_ord))
    {
        XmDrawingAreaCallbackStruct cb_data;
        int tag = k_ord1;
        cb_data.window = XtWindow(sg.w_ord);
        cb_data.event = NULL;
        cb_data.reason = XmCR_EXPOSE;
        draw_proc(sg.w_ord, &tag, &cb_data);
    }
    
    if(sg.w_tim && XtWindow(sg.w_tim))
    {
        XmDrawingAreaCallbackStruct cb_data;
        int tag = k_tim1;
        cb_data.window = XtWindow(sg.w_tim);
        cb_data.event = NULL;
        cb_data.reason = XmCR_EXPOSE;
        draw_proc(sg.w_tim, &tag, &cb_data);
    }
    
    // Forza il flush
    XFlush(display);
    XSync(display, False);
    
// GUAG2025 FINE


timer= XtAppAddTimeOut(XtWidgetToApplicationContext(main_window_widget),
                       (unsigned long)4000,timer_proc,NULL);
}

/*
 prep_str_tim
 preparazione stringhe scala dei tempi
 inserisce nelle stringhe della scala dei tempi i valori calcolati
 in base all'ultimo tempo acquisito 
*/
void prep_str_tim(t_ini,t_fin)
float t_ini,t_fin;
{
    int i;
    float t_delta;
    float t_asc;
    int t_ore,t_minuti,t_secondi;
    long ora,min,sec,giorno,mese,anno;

    // Debug
    printf("DEBUG prep_str_tim: t_ini=%f, t_fin=%f\n", t_ini, t_fin);
    
    // Controllo validità tempi
    if(t_fin <= t_ini) {
        printf("ERRORE: t_fin(%f) <= t_ini(%f)\n", t_fin, t_ini);
        t_fin = t_ini + 1.0; // Forza almeno 1 secondo di differenza
    }
    
    t_delta=(t_fin-t_ini)/6;
// GUAG2025 meglio t_fin?
//    t_asc=t_ini;
    t_asc=t_fin;
    
    for(i=0;i<7;i++)
    {
        anno=0;
        mese=1;
        giorno=1;
        ora=0;
        min=0;
        sec=0;
        
        // Verifica valore valido prima di convertire
        if(t_asc < 0 || t_asc > 1e9) {
            printf("ERRORE: t_asc non valido: %f\n", t_asc);
            sprintf(str_tims[i],"ERR");
            sprintf(str_tim[i],"ERR");
        } else {

            converti_tempoGR(t_asc,&ora,&min,&sec,&giorno,&mese,&anno);
            sprintf(str_tims[i],"%.2f",t_asc);
            sprintf(str_tim[i],"%.2d:%.02d:%.02d",ora,min,sec);
            if(i==0)
                sprintf(str_tim[i],"%s [%.2d-%.2d-%.4d]",
                        str_tim[i],giorno,mese,anno);
        }
        t_asc+=t_delta;
    }
}


/*
 prep_draw
 preparazione del vettore dei punti da disegnare
*/
int prep_draw(float t_iniziale,float t_finale,S_MIN_MAX *min_max)
//float t_iniziale,t_finale;
//S_MIN_MAX *min_max;
{
float f_pix;
int x_pix,x_pixprec;  /* posizione in pixel del tempo sull-asse delle ascisse */
int y_pix;  /* posizione in pixel delle ordinate */
int ind_buf;
int indice,i;




if(t_iniziale == t_finale)
	{
	return(1);
	}
/*
  step in pixel per un decimo di secondo
*/
pix_step=((float)draw_width)/((t_finale-t_iniziale));
ind_buf=0;
while (bufdati[ind_buf].t<t_iniziale)
        ind_buf++;
f_pix=0.0;
x_pix=0;
x_pixprec=0;
for(i=0;i<4;i++)
        {
	if(min_max[i].max == min_max[i].min)
		sg.fatt_y[i]=1;
	else
		sg.fatt_y[i]=draw_height/
                (min_max[i].max-min_max[i].min);
        pp[i]=points[i];
        npp[i]=0;
        indice=sg.ind_mis[i];
        if(indice==-1) continue;
        y_pix=sg.fatt_y[i]*
              (min_max[i].max-bufdati[ind_buf].mis[i]);
        pp[i]->x=x_pix;
        pp[i]->y=y_pix;
        pp[i]++;
        npp[i]++;
        }
ind_buf++;
/*
  prepara il vettore dei punti da disegnare
*/
while(x_pix<draw_width && ind_buf<=n_last)
        {
        f_pix=pix_step*(bufdati[ind_buf].t-t_iniziale);
        x_pix=f_pix;
        if(x_pixprec==x_pix){ind_buf++;  continue;}
        for(i=0;i<4;i++)
                {
                indice=sg.ind_mis[i];
                if(indice==-1) continue;
                y_pix=sg.fatt_y[i]*
                (min_max[i].max-bufdati[ind_buf].mis[i]);
                pp[i]->x=x_pix;
                pp[i]->y=y_pix;
                pp[i]++;
                npp[i]++;
                }
        ind_buf++;
        x_pixprec=x_pix;
        }
}

static void draw_proc(Widget w, int *tag, XmDrawingAreaCallbackStruct *str)
//Widget w;
//int *tag;
//XmDrawingAreaCallbackStruct *str;
{
int ord_unica;
char appstr[50];
int t_ore,t_minuti,t_secondi;
Window win = str->window; 
int widget_num = *tag;              /* Convert tag to widget number. */
int i,k;
int numis,selumis;
int zoom;
int y_line,x_line; 
float f_pix;
int x_pix,x_pixprec; /* posizione in pixel del tempo sull-asse delle ascisse */
int y_pix;  /* posizione in pixel delle ordinate */
int j,jprec; /* scorron il buffer circolare dati */
int indice; /* indice della misura da visualizzare */
long anno,mese,giorno,ora,min,sec;
  switch(widget_num)
	{
  	case k_draw1:  
	get_something(w,XmNwidth,(char *)&draw_width);
	get_something(w,XmNheight,(char *)&draw_height);
	if(resize)
		{
		get_something(w,XmNwidth,(char *)&draw_width);
		get_something(w,XmNheight,(char *)&draw_height);
		resize=0;
		crea_sfondo(w,draw_width,draw_height);
		}
	draw_height-=2;
	if(nofile)
		{
		break;
		}
	if(stato_zoom==ZOOM_END)
		prep_draw(t_iniziale,t_finale,&sg.sel_min_max[0]);
	else
		prep_draw(tz_iniziale,tz_finale,&sg.sel_min_max[0]);

	for(i=0;i<4;i++)
		{
		indice=sg.ind_mis[i];
		if(indice == -1) continue;
		if(HC_on)
			{
			XDrawLines(display,win,gc2[i],
                                   points[i],npp[i],CoordModeOrigin);
			draw_grid(win);
			}
		else
			{
			XDrawLines(display,win,gc[i],
				points[i],npp[i],CoordModeOrigin);
			}
		}
	break;
	case k_mis1:
	for(i=0;i<4;i++)
		{
		y_line= font_height*i+font_height/2;
		selumis=sg.umis_sel[i];
		numis=sg.ind_umis[i];

		if(HC_on)
			{
                	XDrawLine(display,win,gc2[i],5,y_line,25,y_line);
			if(sg.ind_mis[i]==-1) continue;
                	XDrawString(display,win,gc2[0],30,font_height*(i+1),
				sg.descr_mis[i],
                       		strlen(sg.descr_mis[i]));
			XDrawString(display,win,gc2[0],30+font_width*105,
	         		font_height*(i+1),uni_mis[numis].codm[selumis],
                        	strlen(uni_mis[numis].codm[selumis]));
                	}
                else
			{
    			XDrawLine(display,win,gc[i],5,y_line,25,y_line);
                	if(sg.ind_mis[i]==-1) continue;
        		XDrawString(display,win,gc[i],30,font_height*(i+1),
                   		sg.descr_mis[i],
                   		strlen(sg.descr_mis[i]));
                	XDrawString(display,win,gc[i],30+font_width*105,
                    		font_height*(i+1),uni_mis[numis].codm[selumis],
                    		strlen(uni_mis[numis].codm[selumis]));
			}
		}
	break;
       case k_draw0:  /* scrive il nome del file attualmente in
                          apertura */
	if(HC_on)
        XDrawString(display,win,gc2[1],10,font_height,
                   file_vis,strlen(file_vis));
	else
	XDrawString(display,win,gc[1],10,font_height,
                   file_vis,strlen(file_vis));
        break;
	case k_val1:
	for(i=0;i<4;i++)
		{
		y_line= font_height*i+font_height/2;
		if(sg.ind_mis[i]!=-1)
			{
                        formatta(appstr,sg.ultimo[i]);
			if(HC_on)
			XDrawString(display,win,gc2[0],10,font_height*(i+1),
                                appstr,strlen(appstr));
                        else
        		XDrawString(display,win,gc[i],10,font_height*(i+1),
                   		appstr,strlen(appstr));
			}
		else
			sprintf(appstr,"        ");
		}
	break;
	case k_tempo:
        y_line=font_height+2;
      anno=0;
      mese=1;
      giorno=1;
      ora=0;
      min=0;
      sec=0;
      converti_tempoGR(t_ultimo,&ora,&min,&sec,&giorno,&mese,&anno);
	if(tempo_sec)
                {
                sprintf(appstr,"sec. %.2f",t_ultimo);
                }
        else
		{
       		t_ore=((((int)t_ultimo)%86400)/60)/60;
	        t_minuti=(((int)t_ultimo)/60)%60;
       		t_secondi=((int)t_ultimo)%60;
       		sprintf(appstr,"  %.2d:%.2d:%.2d",ora,min,sec);
		}
	if(HC_on)
        	XDrawString(display,win,gc2[0],10,y_line,
                appstr,strlen(appstr));
	else
		XDrawString(display,win,gc[3],10,y_line,
                appstr,strlen(appstr));
   if(!tempo_sec)
		{
      sprintf (appstr," %.2d-%.2d-%.4d",giorno,mese,anno);
	   if(HC_on)
        	XDrawString(display,win,gc2[0],10,y_line*2+4,
                appstr,strlen(appstr));
	   else
		   XDrawString(display,win,gc[3],10,y_line*2+4,
                appstr,strlen(appstr));
		}
	break;
	
	case k_tim1:
	get_something(w,XmNwidth,(char *)&draw_width);
	for(k=0;k<7;k++)
		{
		zoom=sg.zoom;
		x_line=k*(draw_width/6);
/*  correzioni per centrare meglio i valori estremi   */
		if(k==0) x_line+=5;
		if(k==6) x_line-=50;
                if(tempo_sec)
			{
			if(HC_on)
			XDrawString(display,win,gc2[0],x_line,font_height+3,
                        str_tims[k],strlen(str_tims[k]));
			else
                        XDrawString(display,win,gc[3],x_line,font_height+3,
                        str_tims[k],strlen(str_tims[k]));
			}
                else
			{
			if(HC_on)
			XDrawString(display,win,gc2[0],x_line,font_height+3,
                        str_tim[k],strlen(str_tim[k]));
			else
                        XDrawString(display,win,gc[3],x_line,font_height+3,
                        str_tim[k],strlen(str_tim[k]));
			}
                }
	break;

	case k_ord1:
	get_something(widget_array[k_ord1],XmNheight,(char *)&ord_height);
	for(i=1;i<4;i++)
		{
		if(strcmp(sg.str_ord[0][i],"         ")==0)
			ord_unica=1;
		else
			{
			ord_unica=0;
			break;
			}
		}
	for(k=0;k<5;k++)
		{
		y_line=k*((ord_height-5*font_height)/4);
		for(i=0;i<4;i++)
			{
			if(ord_unica)
				y_line+=(font_height*3);
			else
				y_line+=font_height;
                        if(sg.ind_mis[i]==-1) continue;
			if(HC_on)
            			XDrawString(display,win,gc2[0],20,y_line,
                   			sg.str_ord[k][i],
					strlen(sg.str_ord[k][i]));
			else
				XDrawString(display,win,gc[i],20,y_line,
                                       sg.str_ord[k][i],
					strlen(sg.str_ord[k][i]));
			}
		}
	break;

// 	case k_ord1:
//     get_something(widget_array[k_ord1],XmNheight,(char *)&ord_height);
    
//     // Calcolo semplificato delle posizioni
//     for(k=0;k<5;k++)
//     {
//         // Posizione base per ogni linea di ordinata
//         y_line = (k + 1) * ((ord_height - font_height) / 5);
        
//         for(i=0;i<4;i++)
//         {
//             if(sg.ind_mis[i]==-1) continue;
            
//             // Se non c'è scala unica o è la prima variabile, disegna
//             if(!ord_unica || i==0)
//             {
//                 if(HC_on)
//                     XDrawString(display,win,gc2[0],20,y_line,
//                                sg.str_ord[k][i],
//                                strlen(sg.str_ord[k][i]));
//                 else
//                     XDrawString(display,win,gc[i],20,y_line,
//                                sg.str_ord[k][i],
//                                strlen(sg.str_ord[k][i]));
//             }
//         }
//     }
//     break;

  }  // fine switch
}







/*
 * Disegna la griglia di riferimento (per HC)
 */
int draw_grid(win)
Window win;
{
float dy,dx; /* ampiezza rettangoli griglia su asse y e su asse x */
int x_line,y_line;
int i;
dx=draw_width/6;
dy=draw_height/4;
/*
 Linee orizzontali
*/
y_line=0;
XDrawLine(display,win,gc2[0],0,y_line,draw_width,y_line);
for(i=1;i<4;i++)
        {
        y_line=(int)( (float)(dy*(float)i) );
        XDrawLine(display,win,gc2[0],0,y_line,10,y_line);
	XDrawLine(display,win,gc2[0],draw_width-10,y_line,draw_width,y_line);
	}
y_line=draw_height-1;
XDrawLine(display,win,gc2[0],0,y_line,draw_width,y_line);
/*
 Linee verticali
*/
x_line=0;
XDrawLine(display,win,gc2[0],x_line,0,x_line,draw_height);
for(i=1;i<6;i++)
        {
        x_line=(int)( (float)(dx*(float)i) );
	XDrawLine(display,win,gc2[0],x_line,draw_height,x_line,draw_height-10);
	XDrawLine(display,win,gc2[0],x_line,10,x_line,0);
	}
x_line=draw_width-1;
XDrawLine(display,win,gc2[0],x_line,0,x_line,draw_height);
}

/*
 *  gestione dei toggle button per selezione singola variabile
 *  o per gestione fattore di zoom.
 */
static void toggle_proc(w, tag, toggle)
    Widget w;
    int *tag;
    XmToggleButtonCallbackStruct *toggle;
{
float fmin,fmax;
int i;
S_GRAFICO *s;
int selumis,numis; 
XmString app;

s=(&sg);
switch(*tag)
	{
	case k_toggle_mis1:
	case k_toggle_mis2:
	case k_toggle_mis3:
	case k_toggle_mis4:
        for(i=0;i<NUM_BOOLEAN_MIS;i++)
               toggle_mis[i]=0;
        toggle_mis[*tag-k_toggle_mis1]=toggle->set;
	break;
	case k_toggle_zoom1:
	case k_toggle_zoom2:
	case k_toggle_zoom3:
	case k_toggle_zoom4:
	case k_toggle_zoom5:
        for(i=0;i<NUM_BOOLEAN_ZOOM;i++)
                toggle_zoom[i]=0;
        toggle_zoom[*tag-k_toggle_zoom1]=toggle->set;
	break;
	case k_toggle_umis1:
	case k_toggle_umis2:
	case k_toggle_umis3:
	case k_toggle_umis4:
	toggle_umis[*tag-k_toggle_umis1]=toggle->set;
	if(toggle->set) umis_sel[var_attiva]=(*tag)-k_toggle_umis1;
	break;
	case k_toggle2_mis1:
	case k_toggle2_mis2:
	case k_toggle2_mis3:
	case k_toggle2_mis4:
	if(toggle->set==1)
		{
		var_attiva=(*tag-k_toggle2_mis1);
		selumis=umis_sel[var_attiva];    
		numis=s->ind_umis[var_attiva];
		XmToggleButtonSetState(widget_array[k_toggle_umis1+selumis],True,True);
		for(i=k_toggle_umis1;i<=k_toggle_umis4;i++)
			{
                        app = XmStringCreateLtoR(uni_mis[numis].codm[i-k_toggle_umis1],XmSTRING_DEFAULT_CHARSET);
			set_something(widget_array[i],XmNlabelString, (char*)app);
                        XmStringFree(app);
			}
		}
	break;
	case k_toggle3_mis1:
	case k_toggle3_mis2:
	case k_toggle3_mis3:
	case k_toggle3_mis4:
	if(toggle->set==1)
		{
		var_attiva=(*tag-k_toggle3_mis1);
		toggle3_mis[var_attiva]=toggle->set;
		XmTextSetString(widget_array[k_text_max],app_max[var_attiva]);
		XmTextSetString(widget_array[k_text_min],app_min[var_attiva]);
		if(autoscaling[var_attiva])
			XmToggleButtonSetState(widget_array[k_toggle_auto],True,True);
		else
			XmToggleButtonSetState(widget_array[k_toggle_man],True,True);
		}
	else 
	    {
	    if(toggle3_mis[*tag-k_toggle3_mis1]==1)
		{
		XtFree(app_max[*tag-k_toggle3_mis1]);
		XtFree(app_min[*tag-k_toggle3_mis1]);
		app_max[*tag-k_toggle3_mis1]=
                       XmTextGetString(widget_array[k_text_max]);
		app_min[*tag-k_toggle3_mis1]=
		       XmTextGetString(widget_array[k_text_min]);
		}
	    toggle3_mis[*tag-k_toggle3_mis1]=toggle->set;
	    }
	break;
	case k_toggle_auto:
	case k_toggle_man:
	toggle_fsca[*tag-k_toggle_man]=toggle->set;
	if(toggle->set)
		{
		autoscaling[var_attiva]=(*tag)-k_toggle_man;
		}
	break;
	case k_toggle2_umis1:
	case k_toggle2_umis2:
	case k_toggle2_umis3:
	case k_toggle2_umis4:
	toggle2_umis[*tag-k_toggle2_umis1]=toggle->set;
	if(toggle->set) umis_defsel[indice_umis]=(*tag)-k_toggle2_umis1;
	break;

        case k_toggle_dir1:
        case k_toggle_dir2:
        case k_toggle_dir3:
        case k_toggle_dir4:
        toggle_dir[*tag-k_toggle_dir1]=toggle->set;
        break;
	}
}


void set_cur_wait()
{
XDefineCursor(display,XtWindow(toplevel_widget)
        ,/*RootWindow(display,screen_num)*/cursor_wait);
XSync(display,False);
}

void clr_cur_waitGR()
{
XUndefineCursor(display,XtWindow(toplevel_widget));
XSync(display,False);
}

/*
	Posiziona e gestisce il popup menu 
*/
static void PostIt(w,num,str)
Widget w;
int *num;
XmDrawingAreaCallbackStruct *str;
{
XButtonEvent *event;
int widget_num;	
event=(XButtonEvent *) str->event;
/*
  se si e' in freeze per visualizzazione zoomata accetta solo gli
  input nella window di disegno
*/
if(nofile) return;
if(event->button == Button3) return;
if(event->button != Button1 && freeza && (stato_zoom==ZOOM_WAIT ||
                                          stato_zoom==ZOOM_DRAG))
        {
        if(w!=sg.w_draw) return;
        if(stato_zoom==ZOOM_DRAG)
                {
		XtUngrabPointer(w,CurrentTime);
/*
        Cancella l'ultimo rettangolo di selezione disegnato
*/
                if(zoomord)
                XDrawRectangle(display,XtWindow(sg.w_draw),gc_zoom,origine.x,origine.y,width,height);
                else
			{
                        XDrawLine(display,XtWindow(sg.w_draw),
                                  gc_zoom,origine.x+width,0,
                                  origine.x+width,draw_width);
			XDrawLine(display,XtWindow(sg.w_draw),
                                  gc_zoom,origine.x
                                  ,0,origine.x,draw_width);
			}
                }
	XSync(display,False);
        stato_zoom=ZOOM_END;
        }
/* se si e' in modalita' di freeze e si attende il primo
   click del mouse -> permette solamente la gestione
   del rettangolo di zoom */
if(freeza && event->button == Button1 )
        {
        if(w!=sg.w_draw) return;
        if(stato_zoom==ZOOM_WAIT)
                {
		XtGrabPointer(w,False,
		      ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
                      GrabModeAsync,GrabModeAsync,XtWindow(w),
                      None,CurrentTime);

                stato_zoom=ZOOM_DRAG;
                iniz_rect=1; /* segnala di disegnare il primo
                                rettangolo */
                z_ini.x=event->x;
                z_ini.y=event->y;
                if(!zoomord)
                {
                XDrawLine(display,XtWindow(sg.w_draw),gc_zoom,
                        z_ini.x,0,z_ini.x,draw_width);
                }

                return;
                }
        else if(stato_zoom==ZOOM_DRAG)
                {
		XtUngrabPointer(w,CurrentTime);
                z_fin.x=event->x;
                z_fin.y=event->y;
                stato_zoom=ZOOM_WAIT;
                zoomed(z_ini,z_fin);
                return;
                }
        }
}

/*
 * routine per la preparazione della grafica zoomata
 */

int zoomed(z_ini,z_fin)
XPoint z_ini;
XPoint z_fin;
{
int ind_buf;
int ultimo;
int iumis,iumis_sel;
int i,j,k;
int numis,selumis;
int zoom;
int indice; /* indice della misura da visualizzare */
float z_massimo,z_minimo;
/*
int z_ini_x,z_ini_y;
int z_fin_x,z_fin_y;
*/
float z_ini_x,z_ini_y;
float z_fin_x,z_fin_y;

z_ini_x=z_ini.x;
z_ini_y=z_ini.y;
z_fin_x=z_fin.x;
z_fin_y=z_fin.y;


for(i=0;i<4;i++)
        {
        if(sg.ind_mis[i]!=-1) break;
        }
if(i==4) return(0);  /* non e' ancora stata selezionata alcuna misura */


z_ini_x=((float)z_ini_x/(pix_step))+tz_iniziale;
					 /* converte in secondi */
z_fin_x=((float)z_fin_x/(pix_step))+tz_iniziale;

if(z_ini_x==z_fin_x)  /* zoom non effettuabile */
        {
        t_old=0.0;
        XClearArea(display,XtWindow(sg.w_draw),0,0,0,0,True);
        return(0);
        }

if(z_ini_x>z_fin_x)
        { tz_finale=z_ini_x; tz_iniziale=z_fin_x;}
else
        { tz_finale=z_fin_x; tz_iniziale=z_ini_x;}

if(z_ini_y>z_fin_y)
        { z_massimo=(float)z_fin_y;  z_minimo=(float)z_ini_y; }
else
        { z_massimo=(float)z_ini_y;  z_minimo=(float)z_fin_y; }


t_ultimo=t_finale;
for(i=0;i<4;i++)
        {
        if(sg.ind_mis[i]==-1) continue;
        if(zoomord)
                {
                sg.sel_min_max[i].min=sg.sel_min_max[i].max-
                                 (z_minimo/sg.fatt_y[i]);
                sg.sel_min_max[i].max-=(z_massimo/sg.fatt_y[i]);
                if(sg.sel_min_max[i].max == sg.sel_min_max[i].min)
                        sg.fatt_y[i]=1;
                else
                        sg.fatt_y[i]=
                         draw_height/(sg.sel_min_max[i].max-
				sg.sel_min_max[i].min);
                set_ordinate(i);
                }
        }
/* inserisce nelle stringhe della scala dei tempi i valori calcolati
   in base all'ultimo tempo acquisito */
prep_str_tim(tz_iniziale,tz_finale);

t_ultimo=t_finale;
t_old=0.0;
XClearArea(display,XtWindow(sg.w_draw),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_ord),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_val),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_tim),0,0,0,0,True);
XClearArea(display,XtWindow(widget_array[k_tempo]),0,0,0,0,True);
}

/*
 * routine per la gestione del movimento del mouse nella window
 * di presentazione grafici
 */
static void MoveMouse(w,spare,event)
Widget w;
int *spare;
XPointerMovedEvent *event;
{
int iumis,iumis_sel;
Dimension width_new, height_new;
int i;
if(collima)
        {
        x_collima=event->x;
        if(!freeza)
                t_ultimo=x_collima*(t_finale-t_iniziale)/draw_width+t_iniziale;
        else /* caso di modalita' di zoom */
                t_ultimo=x_collima*(tz_finale-tz_iniziale)/draw_width+tz_iniziale;

        for(i=0;i<4;i++)
                {
                if(sg.ind_mis[i]!=-1)
                        {
                        pp_coll=points[i];
                        while(pp_coll->x<x_collima)
                                pp_coll++;
sg.ultimo[i]=sg.sel_min_max[i].max-pp_coll->y/sg.fatt_y[i];
               
                        }
                iumis=sg.ind_umis[i];
                iumis_sel=sg.umis_sel[i];
                sg.ultimo[i]=sg.ultimo[i] *
                                uni_mis[iumis].A[iumis_sel]+
                                uni_mis[iumis].B[iumis_sel];
                }

        XClearArea(display,XtWindow(sg.w_val),0,0,0,0,True);
        XClearArea(display,XtWindow(widget_array[k_tempo]),0,0,0,0
,True);

        }

if (freeza && stato_zoom==ZOOM_DRAG)
        {
        if(!iniz_rect)  /* rettangoli successivi al primo */
                {
                if(zoomord)
                XDrawRectangle(display,XtWindow(sg.w_draw),gc_zoom,origine.x,origine.y,width,height);
                else
                XDrawLine(display,XtWindow(sg.w_draw),gc_zoom,origine.x+width,0,origine.x+width,draw_width);

                }
        else iniz_rect=0;
        z_fin.x=event->x;
        z_fin.y=event->y;
        width_new=(z_fin.x-z_ini.x);
        if(width_new<0)
                {width_new=(-width_new);        origine.x=z_fin.x;
}
        else origine.x=z_ini.x;
        height_new=(z_fin.y-z_ini.y);
        if(height_new<0)
                {height_new=(-height_new); origine.y=z_fin.y; }
        else origine.y=z_ini.y;
               
               
        if(width!=width_new || height != height_new)
                {
                width=width_new;
                height=height_new;
                if(zoomord)
                XDrawRectangle(display,XtWindow(sg.w_draw),gc_zoom,origine.x,origine.y,width,height);
                else
                XDrawLine(display,XtWindow(sg.w_draw),gc_zoom,
                        origine.x+width,0,origine.x+width,draw_width);

                }
        }

}

static void collima_proc(w,reason)
    Widget w;
    unsigned long *reason;
{
if(collima==0)
        {
        collima=1;
/* mette fine ad una possibile definizione di rettangolo per zoom
 */
/*        stato_zoom=ZOOM_END; */
        XDefineCursor(display,XtWindow(sg.w_draw),cursor_coll);
        set_something(wcollima,XmNlabelString,(char*)x_collima_on);
        }
else
        {
        collima=0;
        XUndefineCursor(display,XtWindow(sg.w_draw));
        set_something(wcollima,XmNlabelString,(char*)x_collima_off);
        }
}


static void zoomord_proc(w,reason)
    Widget w;
    unsigned long *reason;
{
zoomord=(!zoomord);
if(zoomord)
        set_something(wzoomord,XmNlabelString,(char*)x_zoomord_on);
else
        set_something(wzoomord,XmNlabelString,(char*)x_zoomord_off);

}

static void secondi_proc(w,reason)
    Widget w;
    unsigned long *reason;
{
tempo_sec=(!tempo_sec);
if(tempo_sec)
        set_something(wtempo_sec,XmNlabelString,(char*)x_secondi_off);
else
        set_something(wtempo_sec,XmNlabelString,(char*)x_secondi_on);
XClearArea(display,XtWindow(sg.w_tim),0,0,0,0,True);
XClearArea(display,XtWindow(widget_array[k_tempo]),0,0,0,0,True);
}


static void memzoomord_proc(w,reason)
    Widget w;
    unsigned long *reason;
{
wzoomord=w;
}


static void memsecondi_proc(w,reason)
    Widget w;
    unsigned long *reason;
{
wtempo_sec=w;
}
   
static void memcollima_proc(w,reason)
    Widget w;
    unsigned long *reason;
{
wcollima=w;
}


static void zoom_proc(w,tag,reason)
Widget w;
int *tag;
unsigned long *reason;
{
int zoom_on;
zoom_on=(*tag);
freeza=zoom_on;
if(zoom_on)
        {
        tz_iniziale=t_iniziale;
        tz_finale=t_finale;
/* se e' in collimazione esce da tale modalita'    */
/*      if(collima) collima=0;  */
/* mette in freeze la visualizzazione rimuovendo il time-out */
        XSync(display,False); 
        if (timer )XtRemoveTimeOut(timer);
        timer=0;
        XSync(display,False); 
        stato_zoom=ZOOM_WAIT;
        XDefineCursor(display,XtWindow(sg.w_draw),cursor_zoom);
	abilita_menu_selez(0);
        }
else    /* fine zoom */
        {
	abilita_menu_selez(1);
        tz_iniziale=t_iniziale;
        tz_finale=t_finale;
        t_old=0; /* per forzare il redraw nella timer proc */
        stato_zoom=ZOOM_END;
        XUndefineCursor(display,XtWindow(sg.w_draw));
        if(collima)
                XDefineCursor(display,XtWindow(sg.w_draw),cursor_coll);
/* attiva nuovamente il time out */
        timer=XtAppAddTimeOut(XtWidgetToApplicationContext(main_window_widget),(unsigned long)4,timer_proc,NULL);
        }
}


void reverse_draw(flag)
int flag;
{
static Pixel draw0_bg,draw1_bg,mis1_bg,ord1_bg,tim1_bg,tempo_bg,form_bg,val1_bg;
if(flag)
	{
	get_something(widget_array[k_draw1],XmNbackground,(char*)&draw1_bg);
        get_something(widget_array[k_draw0],XmNbackground,(char*)&draw0_bg);
	get_something(widget_array[k_mis1],XmNbackground,(char*)&mis1_bg);
	get_something(widget_array[k_ord1],XmNbackground,(char*)&ord1_bg);
        get_something(widget_array[k_val1],XmNbackground,(char*)&val1_bg);
	get_something(widget_array[k_tim1],XmNbackground,(char*)&tim1_bg);
	get_something(widget_array[k_tempo],XmNbackground,(char*)&tempo_bg);
	get_something(widget_array[k_form],XmNbackground,(char*)&form_bg);
	set_something(widget_array[k_draw1],XmNbackground,
                      (char*)WhitePixel(display,screen_num));
	set_something(widget_array[k_mis1],XmNbackground,
                      (char*)WhitePixel(display,screen_num));
	set_something(widget_array[k_ord1],XmNbackground,
                      (char*)WhitePixel(display,screen_num));
        set_something(widget_array[k_tim1],XmNbackground,
                      (char*)WhitePixel(display,screen_num));
        set_something(widget_array[k_tempo],XmNbackground,
                      (char*)WhitePixel(display,screen_num));
        set_something(widget_array[k_val1],XmNbackground,
                      (char*)WhitePixel(display,screen_num));
        set_something(widget_array[k_form],XmNbackground,
                      (char*)WhitePixel(display,screen_num));
        set_something(widget_array[k_draw0],XmNbackground,
                      (char*)WhitePixel(display,screen_num));
	}
else
	{
        set_something(widget_array[k_draw1],XmNbackground,
                      (char*)draw1_bg);
        set_something(widget_array[k_mis1],XmNbackground,
                      (char*)mis1_bg);
        set_something(widget_array[k_ord1],XmNbackground,
                      (char*)ord1_bg);
        set_something(widget_array[k_tim1],XmNbackground,
                      (char*)tim1_bg);
        set_something(widget_array[k_tempo],XmNbackground,
                      (char*)tempo_bg); 
        set_something(widget_array[k_val1],XmNbackground,
                      (char*)val1_bg); 
        set_something(widget_array[k_form],XmNbackground,
                      (char*)form_bg); 
        set_something(widget_array[k_draw0],XmNbackground,
                      (char*)draw0_bg); 
	}
}


static void activate_proc(w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;
{
int widget_num = *tag;              /* Convert tag to widget number. */
int pos_y;
switch(widget_num)
	{
	case k_selgr_dialog:
	case k_memgr_dialog:
/*
 se errore in apertura files gruppi
*/
		if(open_gruppiGR())  
			{
			WidErrore(main_window_widget,err_crea_gruppi,MAPPA);
			return;
			}
	break;
	}
if (widget_array[widget_num] == NULL)
        {                         

        if (MrmFetchWidget(s_RMHierarchy, widget_name[widget_num],
 	toplevel_widget,&widget_array[widget_num], &dummy_class) != MrmSUCCESS) 
		{
	        s_error("can't fetch widget");
    	    }
        }

get_something(sg.w_draw,XmNy,(char *)&pos_y);
set_something(widget_array[widget_num],XmNy,(char *)pos_y);
XtManageChild(widget_array[widget_num]);

}

static void resize_proc(w, tag, str)
    Widget w;
    int *tag;
    XmDrawingAreaCallbackStruct *str;
{
int widget_num = *tag;
if(widget_num==k_draw1)
	resize=1;
if(str->window!=0)
XClearArea(display,str->window,0,0,0,0,True);
}


static void create_proc(w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;
{
int widget_num = *tag;
int i;
widget_array[widget_num] = w;
switch(widget_num)
	{
	case k_draw1:
	get_something(w,XmNwidth,(char *)&draw_width);
	get_something(w,XmNheight,(char *)&draw_height);
	draw_height-=2;
        handle_motion(w);
	sg.w_draw=w;
	break;
	case k_mis1:

	for(i=0;i<4;i++)
		{
		sg.x_descr_mis[i]=
			XmStringCreateLtoR(sg.descr_mis[i],
			XmSTRING_DEFAULT_CHARSET);
		}
	for(i=0;i<4;i++)
		{
		x_sel_var[i]=
			XmStringCreateLtoR(" ",
			XmSTRING_DEFAULT_CHARSET);
		}
        for(i=0;i<NUM_GRUPPI;i++)
                {
                sprintf(lista_gruppi[i],"%.2d - disponibile -           ",
                        i+1); 
		x_gruppi[i]=XmStringGenerate(lista_gruppi[i],NULL,XmCHARSET_TEXT,NULL);

                }
	sg.w_mis=w;
	break;
	case k_tim1:
	sg.w_tim=w;
	break;
	case k_ord1:
	sg.w_ord=w;
	break;
	case k_val1:
	sg.w_val=w;
	break;
	case k_selmis_box:

        set_cur_wait();
	x_simboli=(XmString *)XtMalloc((header2.ncasi+1)*
			  sizeof(XmString));
	x_nomi_misure=(XmString *)XtMalloc((header1.nvar+1)*
			  sizeof(XmString));
	for(i=0;i<header2.ncasi;i++)
		{
		x_simboli[i]=XmStringCreateLtoR(simboli[i],
			XmSTRING_DEFAULT_CHARSET);
		}
	x_simboli[i]=NULL;

	for(i=0;i<header1.nvar;i++)
		{
		x_nomi_misure[i]=XmStringCreateLtoR(nomi_misure[i],
			XmSTRING_DEFAULT_CHARSET);
		}
        for(i=0;i<header2.ncasi;i++)
                {
                XmListAddItem(w,x_simboli[i],0); 
                }
        clr_cur_waitGR();
	break;

	case k_toggle_mis1:
	case k_toggle_mis2:
	case k_toggle_mis3:
	case k_toggle_mis4:
        XmStringFree(x_sel_var[widget_num-k_toggle_mis1]);
        x_sel_var[widget_num-k_toggle_mis1]=
	XmStringCopy(sg.x_descr_mis[widget_num-k_toggle_mis1]);
	set_something(w,XmNlabelString,
 			(char*)x_sel_var[widget_num-k_toggle_mis1]);
	break;

	case k_toggle2_mis1:
	case k_toggle2_mis2:
	case k_toggle2_mis3:
	case k_toggle2_mis4:
        XmStringFree(x_sel_var[widget_num-k_toggle2_mis1]);
        x_sel_var[widget_num-k_toggle2_mis1]=XmStringCopy(sg.x_descr_mis[widget_num-k_toggle2_mis1]);
	set_something(w,XmNlabelString,(char*)x_sel_var[widget_num-k_toggle2_mis1]);
	break;

	case k_toggle3_mis1:
	case k_toggle3_mis2:
	case k_toggle3_mis3:
	case k_toggle3_mis4:
        XmStringFree(x_sel_var[widget_num-k_toggle3_mis1]);
        x_sel_var[widget_num-k_toggle3_mis1]=XmStringCopy(sg.x_descr_mis[widget_num-k_toggle3_mis1]);
	set_something(w,XmNlabelString,(char*)x_sel_var[widget_num-k_toggle3_mis1]);
	break;

	case k_list_gr:
	gruppo_selezionato = -1;
	read_gruppiGR(1);
        for(i=0;i<NUM_GRUPPI;i++)
        	{
		XmListAddItem(widget_array[k_list_gr],x_gruppi[i],0);
                }
	break;

	case k_list_memgr:
	read_gruppiGR(1);
        for(i=0;i<NUM_GRUPPI;i++)   /* per VMS   */
        	{
		XmListAddItem(widget_array[k_list_memgr],x_gruppi[i],0);
                }
	break;

	case k_list_defumis:
	crea_lista_umis();
        for(i=0;i<num_umis;i++)
                {
                umis_defsel[i]=uni_mis[i].sel;
		XmListAddItem(widget_array[k_list_defumis],x_codumis[i],0);
                }
	break;
	}
}


static void map_proc(w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;
{
int widget_num = *tag;
XmString x_void;
int i,j;
int numis,selumis;
float fmin,fmax;
XmString app;

switch(widget_num)
	{
	case k_defumis_dialog:
	     XmListSelectItem(widget_array[k_list_defumis],x_codumis[0],True);
	break;

	case k_selmis_dialog:
	XmToggleButtonSetState(widget_array[k_toggle_mis1],True,True);
	for(i=k_toggle_mis1;i<=k_toggle_mis4;i++)
		{


                XmStringFree(x_sel_var[i-k_toggle_mis1]);
 		x_sel_var[i-k_toggle_mis1]=
            	XmStringCopy(sg.x_descr_mis[i-k_toggle_mis1]);
       		set_something(widget_array[i],XmNlabelString,
			(char*)x_sel_var[i-k_toggle_mis1]);
		}
	break;

	case k_umis_dialog:
	var_attiva=0;
	XmToggleButtonSetState(widget_array[k_toggle2_mis1],True,True);
	for(i=k_toggle2_mis1;i<=k_toggle2_mis4;i++)
		{
                XmStringFree(x_sel_var[i-k_toggle2_mis1]);
                x_sel_var[i-k_toggle2_mis1]=XmStringCopy(sg.x_descr_mis[i-k_toggle2_mis1]);
		set_something(widget_array[i],XmNlabelString,
				(char*)x_sel_var[i-k_toggle2_mis1]);
		}
	selumis=sg.umis_sel[var_attiva];
	numis=sg.ind_umis[var_attiva];
	XmToggleButtonSetState(widget_array[k_toggle_umis1+selumis],True,True);
	for(i=k_toggle_umis1;i<=k_toggle_umis4;i++)
		{
                app = XmStringCreateLtoR(uni_mis[numis].codm[i-k_toggle_umis1],
                                         XmSTRING_DEFAULT_CHARSET); 
		set_something(widget_array[i],XmNlabelString, (char*)app);
                XmStringFree(app);
		}
/* memorizza il settaggio delle unita' di misura come appoggio */
	for(i=0;i<4;i++)
		umis_sel[i]=sg.umis_sel[i];
	break;

	case k_fsca_dialog:
	for(i=k_toggle3_mis1;i<=k_toggle3_mis4;i++)
		{
                XmStringFree(x_sel_var[i-k_toggle3_mis1]);
                x_sel_var[i-k_toggle3_mis1]=XmStringCopy(sg.x_descr_mis[i-k_toggle3_mis1]);
		set_something(widget_array[i],XmNlabelString,(char*)x_sel_var[i-k_toggle3_mis1]);
		}
	for(i=0;i<4;i++)
		{
		autoscaling[i]=sg.autoscaling[i];
                app_max[i]=(char *)XtCalloc(20,sizeof(char));
                app_min[i]=(char *)XtCalloc(20,sizeof(char));

		if(sg.ind_mis[i]!=-1)
			{
                        if(sg.fix_min_max[i].min==(-1)&&
                            sg.fix_min_max[i].max==(-1))
                        {
                        sg.fix_min_max[i].min= sg.sel_min_max[i].min;
                        sg.fix_min_max[i].max= sg.sel_min_max[i].max;
                        }

			fmin=sg.fix_min_max[i].min *
   			uni_mis[sg.ind_umis[i]].A[sg.umis_sel[i]]+
    			uni_mis[sg.ind_umis[i]].B[sg.umis_sel[i]];
			fmax=sg.fix_min_max[i].max *
    		        uni_mis[sg.ind_umis[i]].A[sg.umis_sel[i]]+
     		        uni_mis[sg.ind_umis[i]].B[sg.umis_sel[i]];
                        formatta(app_max[i],fmax);
                        formatta(app_min[i],fmin);

			}
		else
			{
			sprintf(app_max[i],"             ");
			sprintf(app_min[i],"             ");
			}
		}
	XmToggleButtonSetState(widget_array[k_toggle3_mis1],True,True);
	toggle3_mis[0]=1;

	XmTextSetString(widget_array[k_text_max],app_max[0]);
	XmTextSetString(widget_array[k_text_min],app_min[0]);
	if(autoscaling[0])
		XmToggleButtonSetState(widget_array[k_toggle_auto],True,True);
	else
		XmToggleButtonSetState(widget_array[k_toggle_man],True,True);

	break;

	case k_selgr_dialog:
        x_void = XmStringGenerate(" ",NULL, XmCHARSET_TEXT, NULL);
	set_something(widget_array[k_label_grmis1],XmNlabelString,(char*)x_void);
	set_something(widget_array[k_label_grmis2],XmNlabelString,(char*)x_void);
	set_something(widget_array[k_label_grmis3],XmNlabelString,(char*)x_void);
	set_something(widget_array[k_label_grmis4],XmNlabelString,(char*)x_void);
	XmStringFree(x_void);
	break;

        case k_dir_dialog:
        for(i=0;i<NUM_PATH_FILES;i++)
           XmTextSetString(widget_array[k_text_dir1+i],path[i]);
	XmToggleButtonSetState(widget_array[k_toggle_dir1],True,True);
        break;

	}
}


static void formatta(char *str,float fval)
{
         // Debug per vedere cosa viene passato
//    printf("DEBUG formatta: stringa=%s input=%f \n", str, fval);
       
    // Aggiungi controllo per valore esattamente zero
    if(fval == 0.0) {
        sprintf(str,"   0.0000");
        return;
    }

	if(fval>999999.9 || fval<-99999.9 || (fval<0.01 && fval>-0.01))
        sprintf(str,"%9.4E",fval);
	else
        sprintf(str,"%9.4f",fval);
}



/* gestisce il movimento del mouse nella
   window del grafico (per visualizzazione
   valore e per zoom */
void handle_motion(w)
Widget w;
{
XtAddEventHandler(w,PointerMotionMask,False,MoveMouse,NULL);
/* crea il cursore per collimazione   */
cursor_coll=XCreateFontCursor(display,XC_sb_h_double_arrow);
cursor_zoom=XCreateFontCursor(display,XC_cross);
}


/*
 * uscita dalla dialog box di selezione misura
 */

static void ok_proc(w, tag, selez)
    Widget w;
    int *tag;
    XmSelectionBoxCallbackStruct *selez;
{
int flag;
int i,j;
int k;
int indice;
int iumis,iumis_sel;
char *path_app;
char *pmax,*pmin;
float fmax,fmin;
double dfmax,dfmin;
Boolean just_sel; /* indica  se la misura era già
                     visualizzata nella precedente
                    selezione */
S_GRAFICO *s;
s=(&sg);



switch(*tag)
	{
        case k_defumis_dialog:
        for(i=0;i<num_umis;i++)
                {
                uni_mis[i].sel=umis_defsel[i];
                }
        agg_umis();
        free_lista_umis();
        XtDestroyWidget(widget_array[k_defumis_dialog]);
        widget_array[k_defumis_dialog]=0;
	break;

	case k_selmis_dialog:
	n_last = 0;
	scala_unica=0;
	for(i=0;i<4;i++)
		{
		just_sel=False;

                XmStringFree(s->x_descr_mis[i]);
                s->x_descr_mis[i] = XmStringCopy(x_sel_var[i]);

/*
	cerca la stringa nella lista determinandone l'indice
*/
		indice=x_cerca_stringa(s->x_descr_mis[i],x_simboli);
		if(indice!=-1)
#if defined VMS_FORTRAN
			s->ind_mis[i]=cerca_nome(simboli[indice]);
#else
                {
		if(indice==s->ind_mis[i])
			{
			just_sel=True;
			}	
		else
			{
			just_sel=False;
			s->ind_mis[i]=indice;
			}
                }
#endif
		else 
                {
			s->ind_mis[i]=(-1);
                /*printf("k_selmis_dialog (-1)\n");*/
                }
		
		if(s->ind_mis[i]!=-1 && just_sel==False)
			{
			strcpy(s->descr_mis[i],simboli[indice]);
			(s->ind_umis[i])=cerca_umis(s->descr_mis[i]);
			if(s->ind_umis[i]==-1)
				{
				s->ind_umis[i]=num_umis-1;
				} 
			s->autoscaling[i]=1;
/*
  e' settato l'autoscaling: forza la ricerca di minimo e massimo
  modificando il valore del massimo.
*/
			s->sel_min_max[i].max=min_max[s->ind_mis[i]].max+1;
			s->umis_sel[i]=uni_mis[s->ind_umis[i]].sel;

/* MAURIC
			set_scala(i);
*/ 
                        s->fix_min_max[i].max=(-1);
                        s->fix_min_max[i].min=(-1);
			}
		else if (s->ind_mis[i]== -1)/* caso di misura non valida o eliminata */
			{
			strcpy(s->descr_mis[i],descr_vuota);
			set_ordinate(i); /* setta a blank le
                                               ordinate visualizzate */
			}
		}
	XtUnmanageChild (widget_array[k_selmis_dialog]);
        t_old=0.0;
        if(timer!=0)
                XtRemoveTimeOut(timer);
        timer=XtAppAddTimeOut(XtWidgetToApplicationContext(main_window_widget)
                   ,(unsigned long)1,timer_proc,NULL);
        XClearArea(display,XtWindow(s->w_mis),0,0,0,0,True);


	break;
	case k_selzoom_dialog:
	i=0;
	while(i<NUM_BOOLEAN_ZOOM && toggle_zoom[i]==0) 
		i++;
	if(i==NUM_BOOLEAN_ZOOM) s->zoom=0;
	else s->zoom=i;
	s->pendente=0;
	/*XtUnmanageChild (widget_array[k_selzoom_dialog]);  */
	XClearArea(display,XtWindow(s->w_tim),0,0,0,0,True); 
	XClearArea(display,XtWindow(s->w_draw),0,0,0,0,True);
	break;
	case k_umis_dialog:
	for(i=0;i<4;i++)
		{
		s->umis_sel[i]=umis_sel[i];
		set_ordinate(i);
                if(sg.ind_mis[i]!=-1)
                        sg.ultimo[i]=bufdati[n_last].mis[i];
                iumis=sg.ind_umis[i];
                iumis_sel=sg.umis_sel[i];
                sg.ultimo[i]=sg.ultimo[i] *
                                uni_mis[iumis].A[iumis_sel]+
                                uni_mis[iumis].B[iumis_sel];
		}
	XtUnmanageChild (widget_array[k_umis_dialog]);
	XClearArea(display,XtWindow(s->w_ord),0,0,0,0,True);
	XClearArea(display,XtWindow(s->w_val),0,0,0,0,True);
	XClearArea(display,XtWindow(s->w_mis),0,0,0,0,True);
	break;
	case k_fsca_dialog:
	n_last = 0;
	scala_unica=0;
	i=0;
	while(i<NUM_BOOLEAN_MIS && toggle3_mis[i]==0) 
		i++;
	if(i==NUM_BOOLEAN_MIS) return;
	var_attiva=i;
	i=0;

/* estrae i valori settati per l'ultima variabile selezionata  */

	if(s->ind_mis[var_attiva]!=-1)
		{
		free(app_max[var_attiva]);
		free(app_min[var_attiva]);
		app_max[var_attiva]=XmTextGetString(widget_array[k_text_max]);
		app_min[var_attiva]=XmTextGetString(widget_array[k_text_min]);
		}

	for(i=0;i<4;i++)
		{
		if(s->ind_mis[i]!=-1)
		{
		s->autoscaling[i]=autoscaling[i];
/*
  se non e' selezionato l'autoscaling setta i valori minimo e massimo
  utilizzando i valori specificati dall'utente; se i valori non sono
  accettabili segnala l'errore e forza in modalita' di autoscaling.
*/
		if(s->autoscaling[i]==0)
			{
			pmax=app_max[i];
			pmin=app_min[i];
			fmax=atof(pmax);
			fmin=atof(pmin);
			if(fmax>fmin)   /* valori accettabili */
				{
fmin=(fmin-uni_mis[s->ind_umis[i]].B[s->umis_sel[i]])/
      uni_mis[s->ind_umis[i]].A[s->umis_sel[i]];
fmax=(fmax-uni_mis[s->ind_umis[i]].B[s->umis_sel[i]])/
      uni_mis[s->ind_umis[i]].A[s->umis_sel[i]];
				s->fix_min_max[i].max=fmax;
				s->fix_min_max[i].min=fmin;
				set_scala(i);
				}
			else
				s->autoscaling[i]=1;
			}
		else   /* e' richiesto l'autoscaling */
			{
			set_scala(i);
			}
		}
		free(app_max[i]);
		free(app_min[i]);
		}
	for(i=0;i<NUM_BOOLEAN_MIS;i++) toggle3_mis[i]=0; 
	for(i=0;i<NUM_BOOLEAN_FSCA;i++) toggle_fsca[i]=0; 
	XtUnmanageChild(widget_array[k_fsca_dialog]);
        t_old=0.0;
	XClearArea(display,XtWindow(s->w_ord),0,0,0,0,True);
	XClearArea(display,XtWindow(s->w_val),0,0,0,0,True);
	XClearArea(display,XtWindow(s->w_draw),0,0,0,0,True);
	break;

	case k_selgr_dialog:

if (gruppo_selezionato<0)
	printf ("gruppo non selezionato\n");
else
	{
/*	printf ("gruppo selezionato = :%s:\n",lista_gruppi[gruppo_selezionato-1]);*/
	XtVaSetValues (toplevel_widget, 
					XmNtitle, lista_gruppi[gruppo_selezionato-1], 
					XmNiconName, lista_gruppi[gruppo_selezionato-1],
					NULL);
	}

	n_last = 0;
	scala_unica=0;
	memcpy(&s->autoscaling[0], &gruppi[indice_gruppo].gr,sizeof(S_GRUPPO));
	XtDestroyWidget(widget_array[k_selgr_dialog]);
	widget_array[k_selgr_dialog]=0;
	for(i=0;i<4;i++)
		{
		XmStringFree(s->x_descr_mis[i]);
		s->x_descr_mis[i]=XmStringCreateLtoR(s->descr_mis[i],XmSTRING_DEFAULT_CHARSET);
	   /*     x_sel_var[i]=DwtCStrcpy(s->x_descr_mis); */
     
                XmStringFree(x_sel_var[i]);
                x_sel_var[i] = XmStringCopy(s->x_descr_mis[i]);
#if defined VMS_FORTRAN
		s->ind_mis[i]=cerca_nome(s->descr_mis[i]);
#else
		s->ind_mis[i]=cerca_nome(s->descr_mis[i]);
#endif
		if(s->ind_mis[i]!=-1)
			{
/*
  se e' settato l'autoscaling forza la ricerca di minimo e massimo
  modificando il valore del massimo.
*/
			if(s->autoscaling[i]==1)
				s->sel_min_max[i].max=min_max[s->ind_mis[i]].max+1;
			set_scala(i);
			}
		else   /* misura non presente: setta a blank le stringhe
                          delle ordinate  */
			set_ordinate(i);
		}
        t_old=0.0;
        if(timer!=0)
                XtRemoveTimeOut(timer);
        timer=XtAppAddTimeOut(XtWidgetToApplicationContext(main_window_widget),(unsigned long)1,timer_proc,NULL);
        XClearArea(display,XtWindow(s->w_mis),0,0,0,0,True);
	break;

	case k_memgr_dialog:
	gruppi[indice_gruppo].pieno=1;
	pmax=XmTextGetString(widget_array[k_text_memgr]);
	strcpy(gruppi[indice_gruppo].descr,pmax);
	XtFree(pmax);
	memcpy(&gruppi[indice_gruppo].gr,&s->autoscaling[0],sizeof(S_GRUPPO));
        /*printf("Prima di chiamare write_gruppo GR[%d]...>ind_mis=%d umis_Sel=%d\n",
                  indice_gruppo,gruppi[indice_gruppo].gr.ind_mis[0],
                  gruppi[indice_gruppo].gr.umis_sel[0]);*/
	if(write_gruppo(indice_gruppo))
		WidErrore(main_window_widget,err_write_gruppi,MAPPA);
		
		
	XtDestroyWidget(widget_array[k_memgr_dialog]);
	widget_array[k_memgr_dialog]=0;
	XSync(display,False);
	break;
        case k_dir_dialog:
        /* Inizializza  gli indici in lista alle variabili visualizzate */
        XtUnmanageChild (widget_array[k_dir_dialog]);

/*
  elimina il timeout di refresh
*/
        if(timer!=0)
                XtRemoveTimeOut(timer);
        timer=0;

        for(k=0;k<4;k++)
            {
            sg.ind_mis[k]=(int)-1;
            sg.autoscaling[k]=1;
            }
        sg.zoom=0;

        if(nofile==0)  /* se era gia' stato selezionato un
                                 files */
                {
                if(widget_array[k_selmis_dialog]!=0)
                        {
                        XtDestroyWidget(widget_array[k_selmis_dialog]);
                        for(i=0;i<header2.ncasi;i++)
                                {
                                XmStringFree(x_simboli[i]);
                                }
                        for(i=0;i<header1.nvar;i++)
                                {
                                XmStringFree(x_nomi_misure[i]);
                                }
                        free(x_simboli);
                        free(x_nomi_misure);
			}

                for(i=0;i<4;i++)
                      {
                      XmStringFree(sg.x_descr_mis[i]);
                         sg.x_descr_mis[i]=XmStringCreateLtoR("    ",XmSTRING_DEFAULT_CHARSET);
                     }
                close_22dat_circ();
                XtFree(path_22dat);
                d2free(simboli);
                widget_array[k_selmis_dialog]=0;
		XClearArea(display,XtWindow(sg.w_draw),0,0,0,0,True);
		XClearArea(display,XtWindow(sg.w_ord),0,0,0,0,True);
		XClearArea(display,XtWindow(sg.w_val),0,0,0,0,True);
		XClearArea(display,XtWindow(sg.w_tim),0,0,0,0,True);
		XClearArea(display,XtWindow(sg.w_mis),0,0,0,0,True);
		XClearArea(display,XtWindow(widget_array[k_tempo]),
                           0,0,0,0,True);
                }

/*
  memorizza la situazione di tutti i 4 path names
*/
        for(i=0;i<NUM_PATH_FILES;i++)
                {
                path_app=XmTextGetString(widget_array[k_text_dir1+i]);
                strcpy(path[i],path_app);
                XtFree(path_app);
                }

        i=0;
        while(i<NUM_BOOLEAN_DIR && toggle_dir[i]==0)
                i++;
        if(i==NUM_BOOLEAN_DIR) 
	{
	       WidAttenzione(main_window_widget,err_file_nonspec,MAPPA);
                nofile=1;
                strcpy(file_vis,no_file_sel);
		abilita_menu(nofile);
                XClearArea(display,XtWindow(widget_array[k_draw0]),
                           0,0,0,0,True);
                break; /* esce se errore in apertura */
	}
        path_22dat=XmTextGetString(widget_array[k_text_dir1+i]);
        strcpy(file_vis,path_22dat);
/*
 * fprintf(stderr,"\t\t open_22dat_cir in k_dir_dialog\n");
 * fflush(stderr);
 */
/*      Fisso il flag di rilettura dei parametri */
        RileggiF22Par = 1;
        /*fprintf(stderr,"\t open_22dat_cir (k_dir_dialog RileggiF22Par)\n");*/
        if(open_22dat_circGR())
                {
                WidAttenzione(main_window_widget,err_file_nones,MAPPA);
                nofile=1;
                strcpy(file_vis,no_file_sel);
                XClearArea(display,XtWindow(widget_array[k_draw0]),
                           0,0,0,0,True);
                break; /* esce se errore in apertura */
                }
/* se e' gia' aperto un file obbliga l'uscita */
/*        nofile=0; */
        flag=TUTTI;
        /*printf("\t LETTURA FILE HEADER\n");*/
        if(read_22dat_circGR(flag)==1)  /* legge tutti i dati da inizio file */
                {
                close_22dat_circ();
                open_22dat_circGR();
                if(read_22dat_circGR(flag)==1)
                        {
/*                      file non esistente */
                        close_22dat_circ();
                        XtFree(path_22dat);
                        widget_array[k_selmis_dialog]=0;
                        strcpy(file_vis,no_file_sel);
                        nofile=1;
			clr_cur_waitGR();
			break;
                        }
                }
/*
 * fprintf(stderr,"\t\t Fatta prima lettura read_22dat_circ\n");
 * fflush(stderr);
 */
/* !!!! */
	nofile=0;

	abilita_menu(nofile);
        XClearArea(display,XtWindow(widget_array[k_draw0]),0,0,0,0,True);
        break;
	}
}



static void cancel_proc(w, tag, selez)
    Widget w;
    int *tag;
    XmSelectionBoxCallbackStruct *selez;
{
int i;
switch(*tag)
	{
	case k_selmis_dialog:
	XtUnmanageChild (widget_array[k_selmis_dialog]);
	break;
	case k_selzoom_dialog:
	XtUnmanageChild (widget_array[k_selzoom_dialog]);
	break;
	case k_umis_dialog:
	XtUnmanageChild (widget_array[k_umis_dialog]);
	break;
	case k_fsca_dialog:
	for(i=0;i<4;i++)
		{
		free(app_max[i]);
		free(app_min[i]);
		toggle3_mis[i]=0; 
		toggle_fsca[i]=0; 
		}
	XtUnmanageChild (widget_array[k_fsca_dialog]);
	break;
	case k_selgr_dialog:
	XtUnmanageChild (widget_array[k_selgr_dialog]);
	XtDestroyWidget(widget_array[k_selgr_dialog]);
	widget_array[k_selgr_dialog]=0;
	break;
	case k_memgr_dialog:
	XtUnmanageChild (widget_array[k_memgr_dialog]);
	XtDestroyWidget(widget_array[k_memgr_dialog]);
	widget_array[k_memgr_dialog]=0;
	break;
	case k_defumis_dialog:
	free_lista_umis();
	XtDestroyWidget(widget_array[k_defumis_dialog]);
	widget_array[k_defumis_dialog]=0;
	break;
	case k_dir_dialog:
/* MAURIC */
	XtUnmanageChild (widget_array[k_dir_dialog]);

        XtDestroyWidget(widget_array[k_dir_dialog]);
        widget_array[k_dir_dialog]=0;
	break;
	}

}

static void apply_proc(w, tag, selez)
    Widget w;
    int *tag;
    XmListCallbackStruct *selez;
{
XmString x_app;
int i,j1;
i=0;
switch(*tag)
    {
    case k_list_memgr:
	indice_gruppo=x_cerca_stringa(selez->item,x_gruppi);
        XmTextSetString(widget_array[k_text_memgr],
                gruppi[indice_gruppo].descr);
	break;
    case k_list_gr:
	gruppo_selezionato = selez->item_position;
	indice_gruppo=x_cerca_stringa(selez->item,x_gruppi);

        x_app = XmStringGenerate(gruppi[indice_gruppo].gr.descr_mis[0],NULL,
                                   XmCHARSET_TEXT, NULL);
	set_something(widget_array[k_label_grmis1],XmNlabelString,(char*)x_app);
        XmStringFree(x_app);

        x_app = XmStringGenerate(gruppi[indice_gruppo].gr.descr_mis[1],NULL,
                                   XmCHARSET_TEXT, NULL);
	set_something(widget_array[k_label_grmis2],XmNlabelString,(char*)x_app);
        XmStringFree(x_app);

        x_app = XmStringGenerate(gruppi[indice_gruppo].gr.descr_mis[2],NULL,
                                   XmCHARSET_TEXT, NULL);
	set_something(widget_array[k_label_grmis3],XmNlabelString,(char*)x_app);
        XmStringFree(x_app);

        x_app = XmStringGenerate(gruppi[indice_gruppo].gr.descr_mis[3],NULL,
                                   XmCHARSET_TEXT, NULL);
	set_something(widget_array[k_label_grmis4],XmNlabelString,(char*)x_app);
        XmStringFree(x_app);
	break;

    case k_selmis_elimina:
	while(i<NUM_BOOLEAN_MIS && toggle_mis[i]==0) 
		i++;
	if(i==NUM_BOOLEAN_MIS) return;
/*
  i indica la variabile da modificare
*/
        XmStringFree(x_sel_var[i]);
	x_sel_var[i]=XmStringCreateLtoR("   ",XmSTRING_DEFAULT_CHARSET);
	set_something(widget_array[k_toggle_mis1+i],XmNlabelString,(char*)x_sel_var[i]);
	break;
    case k_list_defumis:
	indice_umis=x_cerca_stringa(selez->item,x_codumis);
	for(i=0;i<4;i++)
		{
		x_app=XmStringCreateLtoR(uni_mis[indice_umis].codm[i],
                                         XmSTRING_DEFAULT_CHARSET);
		set_something(widget_array[k_toggle2_umis1+i],XmNlabelString,(char*)x_app);
		XmStringFree(x_app);
		}
	XmToggleButtonSetState(widget_array[k_toggle2_umis1+umis_defsel[indice_umis]],True,True);
	break;
    default:
	while(i<NUM_BOOLEAN_MIS && toggle_mis[i]==0) 
		i++;
	if(i==NUM_BOOLEAN_MIS) return;
/*
  i indica la variabile da modificare
*/


        XmStringFree(x_sel_var[i]);
	x_sel_var[i]=XmStringCopy(selez->item);
	set_something(widget_array[k_toggle_mis1+i],XmNlabelString,(char*)selez->item);
	break;
    }
}

unsigned int find_pos;   /* posizione corrente nel find  */

static void find_proc(w, tag, selez)
    Widget w;
    int *tag;
    XmSelectionBoxCallbackStruct *selez;
{
Widget wbar;
int i,trovata, j;
char *pstr;   /* puntatore alla stringa editata (da ricercare) */
i=0;
pstr=XmTextGetString(widget_array[k_find_text]);
trovata=0;
switch(*tag)
    {
    case k_find_next:
    i=find_pos;
    do
	{
    	if(i<header2.ncasi)
		i++;
    	else
		i=0;
	if(Utstrstr(simboli[i],pstr)!=NULL) /* se la stringa e' stata 
                                              trovata  */
		{
		trovata=1;
		find_pos=i;
		break;
		}
	}
    while(i!=find_pos);

    if(trovata)   /* se la variabile e' stata trovata la seleziona nella
                     lista visualizzata  */
	{
	UtListaScroll(widget_array[k_selmis_box],header2.ncasi,8,find_pos);
	}
    break;
    case k_find_previous:
    i=find_pos;
    do
	{
    	if(i>0)
		i--;
    	else
		i=header2.ncasi;
	if(Utstrstr(simboli[i],pstr)!=NULL) /* se la stringa e' stata 
                                              trovata  */
		{
		trovata=1;
		find_pos=i;
		break;
		}
	}
    while(i!=find_pos);

    if(trovata)   /* se la variabile e' stata trovata la seleziona nella
                     lista visualizzata  */
	{
	UtListaScroll(widget_array[k_selmis_box],header2.ncasi,8,find_pos);
	}
    break;
    }
XtFree(pstr);
}

/*
 * The user pushed the quit button, so the application exits.
 */
static void quit_proc(w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;
{
    if (tag != NULL)
        printf("Quitting - %s\n", tag);
    close_path();
    exit(1);
}



/*
 *  Predispone per HardCopy in bianco e nero.
 */
static void HC_proc (w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;

{
/* pid_t  fpid; */
char app[20];
int j;
if(HC_on)
        {
        HC_on=0;
	if(!stato_zoom && !nofile)
        timer=XtAppAddTimeOut(
			   XtWidgetToApplicationContext(main_window_widget),
                           (unsigned long)4000,timer_proc,NULL);
        }
else
        {
        HC_on=1;
        if(timer)
                {
                XtRemoveTimeOut(timer);
                timer=0;
                }
        }
reverse_draw(HC_on);
resize=1; /* per forzare il ridisegno della griglia */
XClearArea(display,XtWindow(sg.w_draw),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_mis),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_val),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_tim),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_ord),0,0,0,0,True);
XClearArea(display,XtWindow(widget_array[k_tempo]),0,0,0,0,True);
/*
	strcpy(app,"hardcopy.sh");
	fpid=fork();
	if (fpid == 0)
	{
		execlp(app,0);
		exit(0);
	}
*/
}

void abilita_menu(int flag)
//int flag;
{
int valore;
if(flag)
	valore=False;
else
	valore=True;
set_something(widget_array[k_grafici_menu_entry],XmNsensitive,(char*)valore);
set_something(widget_array[k_misure_menu_entry],XmNsensitive,(char*)valore);
}

/*
 abilita/disabilita i menu per la selezione di variabili e/o gruppi
 durante le operazioni di zoom 
*/
void abilita_menu_selez(flag)
int flag;
{
int valore;
if(flag)
	valore=True;
else
	valore=False;

set_something(widget_array[k_selmis_control_button],XmNsensitive,(char*)valore);
set_something(widget_array[k_fsca_control_button],XmNsensitive,(char*)valore);
set_something(widget_array[k_umis_control_button],XmNsensitive,(char*)valore);
set_something(widget_array[k_selgra_control_button],XmNsensitive,(char*)valore);
}
/*
 *  crea_lista_umis
 *     crea le lista dei codici delle unita' di misura
 */
void crea_lista_umis()
{
int i;
for(i=0;i<num_umis;i++)
    x_codumis[i]=XmStringCreateLtoR(uni_mis[i].codice,XmSTRING_DEFAULT_CHARSET);
x_codumis[num_umis]=NULL;
}

void free_lista_umis()
{
int i;
for(i=0;i<num_umis;i++)
        {
        XmStringFree(x_codumis[i]);
        }
}

void reset_graphics()
{
/*
  elimina il timeout di refresh
*/
if(timer!=0)
       XtRemoveTimeOut(timer);
timer=0;
/*
 ricarica il file f22
*/
reload_f22();
}

/*
 reload_f22
 funzione utilizzata nel caso siano variate le veriabili
 in selezione in f22
*/
void reload_f22()
{
int i,k;

for(k=0;k<4;k++)
        {
        sg.ind_mis[k]=(int)-1;
        sg.autoscaling[k]=1;
        }
sg.zoom=0;
if(widget_array[k_selmis_dialog]!=0)
        {
        XtDestroyWidget(widget_array[k_selmis_dialog]);
        for(i=0;i<header2.ncasi;i++)
               {
               XmStringFree(x_simboli[i]);
               }
for(i=0;i<header1.nvar;i++)
              {
               XmStringFree(x_nomi_misure[i]);
              }
	free(x_simboli);
	free(x_nomi_misure);
        }
for(i=0;i<4;i++)
        {
        XmStringFree(sg.x_descr_mis[i]);
        sg.x_descr_mis[i]=XmStringCreateLtoR("    ",XmSTRING_DEFAULT_CHARSET);
        }
close_22dat_circ();
d2free(simboli);
widget_array[k_selmis_dialog]=0;
XClearArea(display,XtWindow(sg.w_draw),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_ord),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_val),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_tim),0,0,0,0,True);
XClearArea(display,XtWindow(sg.w_mis),0,0,0,0,True);
XClearArea(display,XtWindow(widget_array[k_tempo]),
                           0,0,0,0,True);
/*
    Fisso il flag di rilettura dei parametri
*/
RileggiF22Par = 1;
if(open_22dat_circGR())
      {
      return; /* esce se errore in apertura */
      }
/*
        legge tutti i dati da inizio file
*/
if(read_22dat_circGR(TUTTI)==1)
      {
      return;
      }
/*
 legge tempo finale e tempo iniziale dal buffer dei dati
*/
t_finale=bufdati[n_last].t;
t_iniziale=bufdati[0].t;
}

int AnnoBis(long );

/*
    converti_tempoGR

      Converte il tempo di simulazione espresso come valore floating 
      sempre crescente, come data e ora a partire dalle 
      ora:min:sec del gior/mes/anno.
             
   Parametri

      flot temposim: tempo di simulazione in secondi (valore multiplo del
                    passo di simulazione di 1.2 sec)
      long *ora:    ora di partenza da cui calcolare il tempo trascorso;
      long *min:    minuto di partenza da cui calcolare il tempo trascorso;
      long *sec:    secondo di partenza da cui calcolare il tempo trascorso;
      long *gior:   giorno di partenza da cui calcolare il tempo trascorso;
      long *mes:    mese di partenza da cui calcolare il tempo trascorso;
      long *anno:   anno di partenza da cui calcolare il tempo trascorso;

*/

static int converti_tempoGR(temposim,ora,min,sec,gior,mes,anno)
float temposim;
long  *ora,*min,*sec,*gior,*mes,*anno;
{
float tsim;
short i,incr; 
short giorni;
short lastgiorno;
static short giomese[]={31,28,31,30,31,30,31,31,30,31,30,31};
long appoggio;


   giorni=(short)(temposim/86400.);           /* giorni trascorsi */
   tsim=temposim-giorni*86400.; 		
   incr=(short)(tsim/3600.);              /* ore trascorse */
   (*ora)=(*ora)+(long)incr;
   tsim=tsim-incr*3600.; 		
   incr=(short)(tsim/60.);                /* minuti trascorsi */
   (*min)=(*min)+(long)incr;
   incr=(short)(tsim-incr*60.); 		
   (*sec)=(*sec)+(long)incr;                           /* secondi trascorsi */
   
/*
   test overflow
*/
   if((*sec)>=60)          /* secondi */
   {
      (*sec)=0;
      (*min)++;
   }
   if((*min)>=60)           /* minuti */
   {
      (*min)=0;
      (*ora)++;
   }
   if((*ora)>=24)              /* ore */
   {
      (*ora)=(*ora)-24;
      giorni++;
   }

/*
   data
*/   
   appoggio = (*mes) - 1;

   lastgiorno=giomese[(short)appoggio];

   if(AnnoBisGR((*anno)) && (*mes)==2) lastgiorno++;
   for (i=0;i<giorni;i++)
   {
      (*gior)++;
      if((*gior)>lastgiorno)        /* giorno */
      {
         (*gior)=1;
         (*mes)++;
         if((*mes)>12)                          /* mese */
         {
            (*mes)=1;
            (*anno)++;
         }
         lastgiorno=giomese[(short)(*mes)-1];
         if(AnnoBis((*anno)) && (*mes)==2) lastgiorno++;
      }
   }
}


/*
   AnnoBis()
      Dato un anno verifica se e' bisestile.

   Parametri:
      short anno: anno da analizzare

   Ritorno:
      0:     anno non bisestile
      1:     anno bisestile

   9 Dicembre 1992
*/
static int AnnoBisGR(long anno)
{
   if(!(anno % 4) && (anno % 100) || !(anno % 400)) return(1);
   return(0);
}

