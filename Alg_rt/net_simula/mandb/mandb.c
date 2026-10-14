/**********************************************************************
*
*       C Source:               mandb.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Mar 31 10:02:25 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: mandb.c-10 %  (%full_filespec: mandb.c-10:csrc:1 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)mandb.c	5.1\t11/7/95";
/*
   modulo mandb.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)mandb.c	5.1
*/
/*
	utility mandb 
*/
#include <stdio.h>                              /* For printf and so on. */
#include <string.h>                           /* For pcreaterintf and so on. */
#include <errno.h>

#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/List.h>
#include <Xm/ToggleB.h>

#include "libutilx.h"


#if defined UNIX

#include <sys/types.h>   /* include per la lettura della coda di messaggi */
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

#elif defined VMS
# include "vmsipc.h"

#else
 exit( puts( "ATTENZIONE: compilare con -DAIX (o -DVMS / -DULTRIX)"));
#endif

//#define IPC_NOWAIT      0004000         /* error if request must wait */
                                             
#include "sim_param.h"
#include "sim_types.h"
#include "sim_ipc.h"
#include "sked.h"
#include "sim_types.h"
#include "mandb.h"
#include <Rt/RtDbPunti.h>
#include <Rt/RtErrore.h>
#include <Rt/RtMemory.h>
#include <sqlite3.h>

#define RtRecord(punt)  RtIntRecord(punt,__FILE__,__LINE__);
void * RtIntRecord(void *, char *, int);

int read_all_page_mandb();
int s_error(char*);
void init_color();
void lista_var(Widget);
void crea_lista_valori(int);
void crea_lista_page(Widget);
void cerca_pagina(Widget) ;
void aggiungi_var(int,int);
int read_page_mandb(int);
void crea_lista_pagevis(Widget);
int write_page_mandb(int,int);
void cancella_var(int);
void reset_valori_sp();
	



/*
 * le costanti definite di seguito devono corrispondere a quelle presenti
 * nel modulo UIL.
 */

/* widget figli della main_window */

#define k_start          1
#define k_stop           2
#define k_select_button  3
#define k_vispage_list   4
#define k_selpag_dialog  5
#define k_bull_main      6
#define k_list_selpag           7
#define k_selpag_button         8
#define k_selpag_ok             9
#define k_selmis_dialog         10
#define k_selmis_box            11
#define k_find_text             12
#define k_find_next             13
#define k_find_previous         14
#define k_selmis_elimina        15
#define k_appoggio_dialog	16
#define k_singlevar_dialog      17


/* widget figli di ogni pagina aperta */
#define k_toggle_var1    20
#define k_toggle_var2    21
#define k_toggle_var3    22
#define k_toggle_var4    23
#define k_toggle_var5    24
#define k_toggle_var6    25
#define k_toggle_var7    26
#define k_toggle_var8    27
#define k_toggle_var9    28
#define k_toggle_var10   29
#define k_toggle_var11   30
#define k_toggle_var12   31
#define k_toggle_var13   32
#define k_toggle_var14   33
#define k_toggle_var15   34
#define k_toggle_var16   35
#define k_toggle_var17   36
#define k_toggle_var18   37
#define k_toggle_var19   38
#define k_toggle_var20   39

#define k_valore_var1    40
#define k_valore_var2    41
#define k_valore_var3    42
#define k_valore_var4    43
#define k_valore_var5    44
#define k_valore_var6    45
#define k_valore_var7    46
#define k_valore_var8    47
#define k_valore_var9    48
#define k_valore_var10   49
#define k_valore_var11   50
#define k_valore_var12   51
#define k_valore_var13   52
#define k_valore_var14   53
#define k_valore_var15   54
#define k_valore_var16   55
#define k_valore_var17   56
#define k_valore_var18   57
#define k_valore_var19   58
#define k_valore_var20   59

#define k_descr_var1    60
#define k_descr_var2    61
#define k_descr_var3    62
#define k_descr_var4    63
#define k_descr_var5    64
#define k_descr_var6    65
#define k_descr_var7    66
#define k_descr_var8    67
#define k_descr_var9    68
#define k_descr_var10   69
#define k_descr_var11   70
#define k_descr_var12   71
#define k_descr_var13   72
#define k_descr_var14   73
#define k_descr_var15   74
#define k_descr_var16   75
#define k_descr_var17   76
#define k_descr_var18   77
#define k_descr_var19   78
#define k_descr_var20   79

#define k_valore_sp1    80
#define k_valore_sp2    81
#define k_valore_sp3    82
#define k_valore_sp4    83
#define k_valore_sp5    84
#define k_valore_sp6    85
#define k_valore_sp7    86
#define k_valore_sp8    87
#define k_valore_sp9    88
#define k_valore_sp10   89
#define k_valore_sp11   90
#define k_valore_sp12   91
#define k_valore_sp13   92
#define k_valore_sp14   93
#define k_valore_sp15   94
#define k_valore_sp16   95
#define k_valore_sp17   96
#define k_valore_sp18   97
#define k_valore_sp19   98
#define k_valore_sp20   99

#define k_tempo_sp1    100
#define k_tempo_sp2    101
#define k_tempo_sp3    102
#define k_tempo_sp4    103
#define k_tempo_sp5    104
#define k_tempo_sp6    105
#define k_tempo_sp7    106
#define k_tempo_sp8    107
#define k_tempo_sp9    108
#define k_tempo_sp10   109
#define k_tempo_sp11   110
#define k_tempo_sp12   111
#define k_tempo_sp13   112
#define k_tempo_sp14   113
#define k_tempo_sp15   114
#define k_tempo_sp16   115
#define k_tempo_sp17   116
#define k_tempo_sp18   117
#define k_tempo_sp19   118
#define k_tempo_sp20   119

#define k_toggle_sp1    120
#define k_toggle_sp2    121
#define k_toggle_sp3    122
#define k_toggle_sp4    123
#define k_toggle_sp5    124
#define k_toggle_sp6    125
#define k_toggle_sp7    126
#define k_toggle_sp8    127
#define k_toggle_sp9    128
#define k_toggle_sp10   129
#define k_toggle_sp11   120
#define k_toggle_sp12   131
#define k_toggle_sp13   132
#define k_toggle_sp14   133
#define k_toggle_sp15   134
#define k_toggle_sp16   135
#define k_toggle_sp17   136
#define k_toggle_sp18   137
#define k_toggle_sp19   138
#define k_toggle_sp20   139

#define k_descr_mempag          140
#define k_text_mempag           141
#define k_list_history          142
#define k_name_history          143
#define k_cancel_history        144
#define k_cut_button            145
#define k_paste_button          146
#define k_clear_page            147
#define k_close_page            148
#define k_mempag_dialog        149
#define k_list_mempag           150
#define k_history_dialog       151
#define k_err_dialog	       152

#define k_max_widget             153

#define MAX_WIDGETS (k_max_widget + 1)


Widget toplevel_widget,                 /* Root widget ID of our */
                                        /* application. */
  main_window_widget,                   /* Root widget ID of main */
                                        /* MRM fetch */
  widget_selpag_dialog,
  widget_selmis_dialog,widget_selmis_box,widget_list_pagevis,widget_appoggio_dialog,
  widget_singlevar_dialog, widget_err_dialog;

/* si alloca la tabella per le pagine massime che possono essere visualizzate */

PAGE_VIS pagina[MAX_PAGINE];

/* variabili per la grafica   */

#define NUM_COLORI 6
char *nomi_colori[]={"black","white","red","light gray","cyan","yellow"};
Pixel pix_val[NUM_COLORI];

XColor excolor[12];

XColor sfondo_label={0L,0xcc00,0xe500,0xcb00,0,0};

int font_ascent;
int font_height;
int font_width;
Display *display;
int screen_num;
/*
 variabili relative al resource manager
 */

static MrmHierarchy s_RMHierarchy;     /* MRM database hierarchy ID */
static MrmType dummy_class;            /* and class variable. */
static int db_filename_num =1;

#if defined (VMS)
static char *filename_uid = "mandb.uid";   /* DwtDrm.heirachy file list. */

#else
static char *filename_uid = "/mandb.uid";   /* DwtDrm.heirachy file list. */
#endif

char *getenv();

/*
 Dichiarazioni callback function
 */

float estr_sh();

static void create_proc(); 
static void activate_proc();
static void toggle_proc();
static void quit_proc();
static void ok_proc();
static void apply_proc();
static void find_proc();
static void cancel_proc();
static void map_proc();
static void timer_proc();
static void modify_proc();
static void focus_proc();
static void PostIt();
static MrmRegisterArg reglist[] = {
    {"create_proc", (caddr_t) create_proc},
    {"activate_proc", (caddr_t) activate_proc},
    {"toggle_proc",(caddr_t) toggle_proc},
    {"quit_proc",(caddr_t) quit_proc},
    {"apply_proc",(caddr_t) apply_proc},
    {"ok_proc",(caddr_t) ok_proc},
    {"find_proc",(caddr_t) find_proc},
    {"map_proc",(caddr_t) map_proc},
    {"cancel_proc",(caddr_t) cancel_proc},
    {"modify_proc",(caddr_t) modify_proc},
    {"focus_proc",(caddr_t) focus_proc},
    {"PostIt",(caddr_t) PostIt},
};


static int reglist_num = (sizeof reglist / sizeof reglist [0]);

XtIntervalId time_id;
/*
	Variabili per l'area shared delle variabili */

char *ind_sh_top;
int id_sh;
VARIABILI *variabili;
int tot_variabili;
int mod,nbloc;

RtDbPuntiOggetto dbpunti;
RtErroreOggetto errore_obj;



/*
 * dati per selezione variabili
 */

int indice_sel_var;   /* indice variabile selezionata nella lista
                         di selezione misure selmis (parallela
                         alla struttura variabili */
int indice_var_cut;   /* indice variabile copiata con il cut */

/*
	area di descrizione delle pagine memorizzate sul file page_mandb.dat
*/

char descr_page[MAX_PAGINE][MAX_LUN_DESCRPAGE];

/*
	area dei valori di forzamento variabili e tempi relativi
*/


float	valori_sp[MAX_TOTVAR];
float	tempi_sp[MAX_TOTVAR];

XmString x_list_history[MAX_CAMPIONI];
XmString x_list_pagevis[MAX_PAGINE];

int    *p_indvar;
int    *p_pointin;
CAMP  *p_valori;
PAGE_VIS *p_pagcur;
float *p_valori_sp;
float *p_tempi_sp;

/*
	dati per l' area shared privata di mandb
*/

DBASE_MANDB *p_db;
int     shmvid_mandb;                  /* identificativo shm               */
int    indice_pagina;                  /*indice pagina corrente */
int    indice_pagina_new;              /* indice nuova pagina
                                           selezionata */
sqlite3 *db;

/*   identificativo per la coda di messaggi di mandb */

int msgqid;                      /* identificativo coda di messaggi  */

MSG msg_acqmandb;		/* buffer di accodamento messaggi */

int pid_acqmandb;		/* pid processo acq_mandb */

int flag_retsavepage=0;		/* flag che specifica se la funzione
                                   di save pagine e' attivata dalla quit_proc
                                   o dalla activate_proc o dalla ok_proc
                                   della selezione delle pagine */

int	stato_agg=0;		/* flag aggiornamento in corso ( 0 stop )*/

int	cambio_pagina;		/* flag cambio pagina avvenuto */

int	idsem_locale;		/* semaforo locale tra mandb e acqmandb
*/
char err_message[100];

float old_time= -1;

int num_pagevis = 0;

void testata(char *, char *);
void costruisci_var (char**, VARIABILI **, int*);

unsigned int main(argc, argv)
    unsigned int argc;   /* Command line argument count */
    char *argv[];        /* Pointers to command line args. */
{
char *ind;
int shr_usr_key;      /* chiave utente per shared  */
int i,size,j;
char name_uid[FILENAME_MAX];
char *path_uid;
char appoggio_execl[100];
char nome_proc[30];
HEADER_REGISTRAZIONI hdreg;
SIMULATOR *simpar;


/* redirezione output  e stampa versione */
testata("mandb",SccsID);

/* Initialize the X Toolkit. We get back a top level shell widget.
*/

MrmInitialize();                 /* Initialize MRM before */
                                        /* the X Toolkit. */
toplevel_widget = XtInitialize("mandb",
                                        /* Main window banner text. */
      "Mandb",                          /* Root class name. */
      NULL,                             /* No option list. */
      0,                                /* Number of options. */
      &argc,                            /* Address of argc */
      argv);                            /* argv */

    display=XtDisplay(toplevel_widget);
    screen_num=DefaultScreen(display);

/* legge SHR_USR_KEY */
shr_usr_key = atoi((char *)getenv("SHR_USR_KEY"));

simpar  = (SIMULATOR*)calloc(1,sizeof(SIMULATOR));
errore_obj = RtCreateErrore(RT_ERRORE_TERMINALE,"mandb");
dbpunti = RtCreateDbPunti(errore_obj,NULL,DB_PUNTI_INT,simpar);
if(dbpunti == NULL)
	{
	exit(printf("mandb: simulazione non attiva\n"));
	}
free(simpar);
/*
RtDbPGetHeaderReg(dbpunti,&hdreg);
printf("\t\t\t MANDB estrae [%d %d %d %d %d %d %d ]\n",
        hdreg.simulator.max_snap_shot,
        hdreg.simulator.max_back_track,
        hdreg.simulator.max_campioni,
        hdreg.simulator.num_var,
        hdreg.simulator.max_pertur,
        hdreg.simulator.spare_snap,
        hdreg.simulator.pert_clear);
*/



/*
        attende che la simulazione sia in run
*/
for(i=0;i<20;i++)
        {
	int stato;
	RtDbPGetStato(dbpunti,&stato);
        if(stato==STATO_RUN)
                break;
        sleep(3);
        }
if(i==20)
	{
	printf("Scaduta attesa simulazione in RUN\n");
	exit(0);
	}


#if defined UNIX
/*
if (shmget(shr_usr_key+ID_SHM_SIM, 2, 0777 & (!IPC_CREAT)) == -1)
	{
		perror("MANDB");
		printf("SIMULAZIONE FERMA ; MANDB TERMINATO \n");
               exit(0);
        }
*/
#endif
/*
	crea l'area shared 
*/

   p_db = (DBASE_MANDB *) crea_shrmem(shr_usr_key+ID_SHM_MDB,sizeof(DBASE_MANDB),&shmvid_mandb);
   memset ((char *) p_db,0,sizeof (DBASE_MANDB));

/* si alloca  l'area shared delle variabili */

        costruisci_var(&ind_sh_top,&variabili,&id_sh);
        mod=numero_modelli(ind_sh_top);
        tot_variabili=numero_variabili(ind_sh_top);
/********
	printf("tot_vcar %d\n",tot_variabili);
for (i=0; i<tot_variabili; i++)
	printf("%d  %d  %s \n",i,variabili[i].addr,variabili[i].nome);
********/

/* si alloca  l'area shared del simulatore */

/*
    sim_shraddr(shr_usr_key);
*/

/*
 Creazione della coda di messaggi
 */
    msgqid = msg_create(shr_usr_key+ID_MSG_MANDB,0);
    if(msgqid==(-1))
    {
  	  perror("\nmandb: coda messaggi mandb");
          exit(1);
    }

/*
 Creazione del semaforo che regola l'accesso all' area shared tra 
 i processi mandb e acqmandb 
*/

	idsem_locale = sem_create(shr_usr_key+ID_SEM_MDB_4,1); 
	if (idsem_locale == -1 )
		fprintf(stderr, " impossibile creare semaforo locale ");
		exit(99);


/*
	legge il file delle pagine di mandb 
*/
	read_all_page_mandb();

/*
	attiva il processo acqmandb 
*/
#if defined UNIX
	sprintf(appoggio_execl,"%s/acqmandb",getenv("LEGORT_BIN"));
#else
	sprintf(appoggio_execl,"%sacqmandb",getenv("LEGORT_BIN"));
#endif
	sprintf(nome_proc,"acqmandb%s",getenv("SHR_USR_KEY"));
	pid_acqmandb=vfork();
	if (pid_acqmandb == -1) 
	{
		perror("Errore in creazione processo ACQMANDB");
		exit(1);
	}
	if (!pid_acqmandb )
	{
		if(execl(appoggio_execl,nome_proc,(char*)0)== -1)
			{
			perror("\n errore execl");
			exit(0);
			}
	}

/* Open the UID files (the output of the UIL compiler) in the
   hierarchy     */

name_uid[0]=0;
path_uid=getenv("LEGORT_UID");
strcpy(name_uid,path_uid);
strcat(name_uid,filename_uid);
path_uid=name_uid;
if (MrmOpenHierarchy(db_filename_num, /* Number of files. */
      &path_uid,                    /* Array of file names.  */
      NULL,                               /* Default OS extenstion. */
      &s_RMHierarchy)                   /* Pointer to returned MRM ID */
      != MrmSUCCESS)
        s_error("can't open hierarchy");

/* inizializza i colori  */

    init_color();
/*   crea la main window */

    MrmRegisterNames(reglist, reglist_num);
    if (MrmFetchWidget(s_RMHierarchy, "S_MAIN_WINDOW", toplevel_widget,
      &main_window_widget, &dummy_class) != MrmSUCCESS)
        s_error("can't fetch main window");

/* Manage the main part and realize everything.  The interface comes
 * on the display now. */
    XtManageChild(main_window_widget);
    XtRealizeWidget(toplevel_widget);


    /* accoda il messaggio di start al processo acqmandb */

    msg_acqmandb.mtype=MSG_START;
    strcpy(msg_acqmandb.mtext,"messaggio start");
    if (msg_snd(msgqid,&msg_acqmandb,LUN_MSG_MAX,!IPC_NOWAIT) == -1)
            perror ("accodamento ");

    XtMainLoop();
}


void init_color()
{
XColor color;
Colormap default_cmap;
Pixel pixel;

int i;

default_cmap=DefaultColormap(display,screen_num);

for(i=0;i<NUM_COLORI;i++)
   {
   XAllocNamedColor(display,
            default_cmap,
            nomi_colori[i],&color,&excolor[i]);
   }

XAllocColor(display, default_cmap,&sfondo_label);

}


/*
  create_proc
     funzione richiamata all'atto della creazione dei widget
 */
static void create_proc(w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;
{
char stringa2[100],stringa[100];
int widget_num = *tag;
int i;
Widget wx;
XmString x_app;

/* per tutti i widget figli delle pagine memorizza gli indici di widget */

if (widget_num >= k_toggle_var1  && widget_num <= k_max_widget)
{
	pagina[indice_pagina].pogg[widget_num]=w; 
}
switch (widget_num)
	{

	case k_appoggio_dialog:

	widget_appoggio_dialog=w;
	break;

        case k_singlevar_dialog:

        widget_singlevar_dialog=w;
        break;

        case k_selmis_box:
		widget_selmis_box=w;
		lista_var(w);
	break;
	
        case k_list_selpag:
	   crea_lista_page(w);
	break;

        case k_vispage_list:
	   widget_list_pagevis=w;
        memset(stringa,' ',MAX_LUN_DESCRPAGE+5);
        stringa[MAX_LUN_DESCRPAGE+5-1]=0;
	x_list_pagevis[0]=XmStringCreateLtoR(stringa,XmSTRING_DEFAULT_CHARSET);
        XmListAddItem(w,x_list_pagevis[0],1);
	num_pagevis++;
        break;

        case k_list_mempag:
           crea_lista_page(w);
        break;
                                                                    
	case k_list_history:
	   crea_lista_valori(p_pagcur->toggle_attivo);
           for (i=0; i< MAX_CAMPIONI; i++)
           XmListAddItemUnselected(w,x_list_history[i],0);
        for (i=0; i< MAX_CAMPIONI; i++)
        {
         if (x_list_history[i] != NULL) XmStringFree(x_list_history[i]);
        }
	break;

	case k_name_history:
	   strcpy(stringa,variabili[p_indvar[p_pagcur->toggle_attivo]].nome);
	   strcat (stringa,"  -  ");
	   strcat(stringa,variabili[p_indvar[p_pagcur->toggle_attivo]].descr);
           x_app=XmStringCreateLtoR(stringa,XmSTRING_DEFAULT_CHARSET);
	   set_something_val(w,XmNlabelString,(XtArgVal) x_app);
	   XmStringFree(x_app);
	break;


	case k_descr_mempag:

            sprintf(stringa,"Salvare la pagina n.  %d  come ",indice_pagina+1);
           x_app=XmStringCreateLtoR(stringa,XmSTRING_DEFAULT_CHARSET);
           set_something_val(w,XmNlabelString,(XtArgVal) x_app);
           XmStringFree(x_app);
	break;

      case k_text_mempag:

	   if (strlen(descr_page[indice_pagina]))
			XmTextSetString(w,descr_page[indice_pagina]);
	break;

	case k_stop:
          set_something_val(w,XmNbackground,(XtArgVal) excolor[5].pixel);
        break;

	case k_bull_main:
/*	XtAddEventHandler(w,ButtonPressMask,False,PostIt,k_bull_main);*/
	break;

	}

}

/*  
	funzione chiamata dalla selezione nella lista delle variabili
*/
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
	case k_selmis_box:
        indice_sel_var=selez->item_position - 1;
        break;

        case k_list_selpag:
        indice_pagina_new= selez->item_position - 1;
	break;

                                                          
        case k_list_mempag:
        p_pagcur->indice_pagina_save= selez->item_position - 1;
	break;
    }
}

/* Callback relativa ai toggle button di modifica valore */

static void modify_proc(w, tag, str)
    Widget w;
    int *tag;
    XmToggleButtonCallbackStruct *str;
{
XmString x_app;
int indice,indice_text;
char *pmax,stringa[20];
float valsp;
MOD_VALORI mod_val;

indice = (int) *tag - k_toggle_sp1;
cerca_pagina(XtParent(w));
	
if (str->set)  
	{
	/*  sbianca il campo text */

	if (p_indvar[indice] == -1 )
	{
/* se non e' associata una variabile non accetta la richiesta */
		XmToggleButtonSetState(w,False,False);
		return;
	}
	XmTextSetEditable(p_pagcur->pogg[k_valore_sp1+indice],True);
	XmTextSetString(p_pagcur->pogg[k_valore_sp1+indice],"");
	}
	else
	{
           pmax=XmTextGetString(p_pagcur->pogg[k_valore_sp1+indice]);
	   if (strlen (pmax))
	   {
        	p_pagcur->modifica_valsp=1;
		sscanf(pmax,"%f",&mod_val.valore);
		mod_val.indice_var=variabili[p_indvar[indice]].addr;
		msg_acqmandb.mtype=MSG_MOD_VALORI;
		memcpy(&msg_acqmandb.mtext[0],&mod_val.indice_var,sizeof (MOD_VALORI));
       		 if (msg_snd(msgqid,&msg_acqmandb,sizeof (MOD_VALORI),IPC_NOWAIT) != -1 )
		{

		p_valori_sp[indice]=mod_val.valore;
		RtDbPGetTimeTask(dbpunti,variabili[p_indvar[indice]].mod-1,
			&(p_tempi_sp[indice]));
       		 sprintf(stringa,"%6.2f",p_tempi_sp[indice]);
       		 x_app=XmStringCreateLtoR(stringa,XmSTRING_DEFAULT_CHARSET);
       		 set_something_val(p_pagcur->pogg[k_tempo_sp1+indice],XmNlabelString,(XtArgVal) x_app);
       		 set_something_val(p_pagcur->pogg[k_tempo_sp1+indice],XmNborderWidth,(XtArgVal)1);
		XmStringFree(x_app);
	        XmTextSetEditable(p_pagcur->pogg[k_valore_sp1+indice],False);
	        }
		else  printf("\n MODIFICA VALORE VARIABILE  NON ACCODATA");
	   }
	}
}


static void ok_proc(w, tag, selez)
    Widget w;
    int *tag;
    XmSelectionBoxCallbackStruct *selez;
{
XmString x_app;
int indice,i,j, kk, k;
RESET_HISTORY m_reset;
char *pmax;
Arg args[10];
Widget attenzione;
Dialog_geometry geom;
int flag;
switch(*tag)
        {
        case k_selmis_dialog:

/* aggiunge la variabile nella pagina selezionata nella posizione
   di toggle_attivo */

             aggiungi_var(p_pagcur->toggle_attivo,indice_sel_var);
	break;

	case k_singlevar_dialog:

	        pmax=XmTextGetString(XtNameToWidget
					(widget_singlevar_dialog,"text_singlevar"));
		if (!strlen (pmax)) break;
		flag=0;
		for( k =0 ; k < tot_variabili ; k++ )
		{	
			if(Utstrstr(variabili[k].nome,pmax)!=NULL)
			{
		             aggiungi_var(p_pagcur->toggle_attivo,k);
			     flag=1;
				break;
			}
		}
		if (!flag)
		{
	        geom.default_pos=1;
	        geom.height=200;
	      attenzione=attention(widget_singlevar_dialog,
			"Variabile non esistente !!!",CREA|MAPPA,geom);
		}
	break;


	case k_selpag_ok:

             indice_pagina=indice_pagina_new;
	     if (pagina[indice_pagina].w != 0) break;
	     memset(pagina[indice_pagina].sigle_vis,0,MAX_VAR*MAX_LUN_NOME_VAR);
	     read_page_mandb(indice_pagina);

/*
	si alloca l' array degli oggetti per tutta la pagina */

	     pagina[indice_pagina].pogg=(Widget *) XtMalloc(MAX_WIDGETS * sizeof (Widget));
	     pagina[indice_pagina].toggle_attivo= -1;
	     i=0;
	     XtSetArg(args[i],XmNx,indice_pagina*30);i++;
	     XtSetArg(args[i],XmNy,indice_pagina*30);i++;
	     pagina[indice_pagina].w=NULL;
	     if (MrmFetchWidgetOverride(s_RMHierarchy, "page_dialog",toplevel_widget,
            NULL,args,i,&pagina[indice_pagina].w,&dummy_class) != MrmSUCCESS)
              s_error("can't fetch dialog box");
            XtManageChild(pagina[indice_pagina].w);
		crea_lista_pagevis(widget_list_pagevis);
	break;

	case k_mempag_dialog:

	     cerca_pagina(XtParent(XtParent(XtParent(w))));
	
/* verifica se il save e' richiesto sulla pagina corrente o su un' altra */

             if (indice_pagina != p_pagcur->indice_pagina_save)
		{

/* se richiede la memorizzazione di un' altra pagina ma questa e' gia
	aperta abbandona la funzione */
            	    if (pagina[p_pagcur->indice_pagina_save].w != NULL)
               	    {
                	p_pagcur->pogg[k_err_dialog]=0;
                	strcpy(err_message,"La pagina selezionata per il save e' aperta ");
                	x_app=XmStringCreateLtoR(err_message,XmSTRING_DEFAULT_CHARSET);
                	XtSetArg(args[0],XmNmessageString,x_app);
                	if (MrmFetchWidgetOverride(s_RMHierarchy, "err_dialog", p_pagcur->w,NULL,args,1,&p_pagcur->pogg[k_err_dialog],&dummy_class) != MrmSUCCESS)
	              s_error("can't fetch dialog box");
        	        XtManageChild(p_pagcur->pogg[k_err_dialog]);
                	break;
                   }

		/* accoda  il messaggio di reset del history al processo acq_mandb per tutte le variabili presenti nella pagina di save */

                   sem_wait(idsem_locale);
		   for (i=0; i < MAX_VAR; i++)
		   {
		    	if (p_db->indice_var[(p_pagcur->indice_pagina_save*MAX_VAR)+i] != -1)
 		   	{

			   p_db->pointin[(p_pagcur->indice_pagina_save*MAX_VAR)+i]=0;
	                   for (j=0; j< MAX_CAMPIONI; j++)
	                   {
	                       p_db->valori[(p_pagcur->indice_pagina_save*MAX_VAR)+i].valore[j]=0.;
	                       p_db->valori[(p_pagcur->indice_pagina_save*MAX_VAR)+i].tempo[j]=0.;
	                   }
	                }
		        p_db->indice_var[(p_pagcur->indice_pagina_save*MAX_VAR)+i]=p_indvar[i];
	                valori_sp[p_pagcur->indice_pagina_save*MAX_VAR+i]=p_valori_sp[i];
		        p_db->indice_var[(p_pagcur->indice_pagina_save*MAX_VAR)+i]=p_indvar[i];
		        p_db->indice_var[(p_pagcur->indice_pagina_save*MAX_VAR)+i]=p_indvar[i];
		        p_db->indice_var[(p_pagcur->indice_pagina_save*MAX_VAR)+i]=p_indvar[i];
		        p_db->indice_var[(p_pagcur->indice_pagina_save*MAX_VAR)+i]=p_indvar[i];
	                tempi_sp[p_pagcur->indice_pagina_save*MAX_VAR+i]=p_tempi_sp[i];
		   }
		   memcpy(pagina[p_pagcur->indice_pagina_save].sigle_vis,
			  p_pagcur->sigle_vis,MAX_VAR*MAX_LUN_NOME_VAR);
                   sem_signal(idsem_locale);
	        }

/* legge la nuova descrizione della pagina */
		pmax=XmTextGetString(p_pagcur->pogg[k_text_mempag]);
                if (!strlen (pmax))
                  sprintf(descr_page[p_pagcur->indice_pagina_save],"pagina%d",p_pagcur->indice_pagina_save+1);
                else
                  strcpy(descr_page[p_pagcur->indice_pagina_save],pmax);
	       	
 	        write_page_mandb(p_pagcur->indice_pagina_save,1);  /* salva tutta la pagina */
	        p_pagcur->modifica_var=0;
	        p_pagcur->modifica_valsp=0;
                XtDestroyWidget(p_pagcur->pogg[k_mempag_dialog]);
                p_pagcur->pogg[k_mempag_dialog]=NULL;
	        if (!flag_retsavepage)
                	XtCallCallbacks(p_pagcur->pogg[k_close_page],XmNactivateCallback,(XtPointer)k_close_page);

        break;

	}
}

/*
     funzione richiamata all'atto della creazione dei widget
 */
static void activate_proc(w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;
{
int widget_num = *tag;              /* Convert tag to widget number. */
int pos_y;
XmString x_app;
Arg args[10]; /* per settaggio argomenti widgets */
static int history_x=0,history_y=0;
int i,j;
RESET_HISTORY m_reset;


switch (widget_num)
        {
      case k_selmis_dialog:

         if (widget_selmis_dialog==NULL)
  	if (MrmFetchWidget(s_RMHierarchy, "selmis_dialog", widget_appoggio_dialog,
                       &widget_selmis_dialog, &dummy_class) != MrmSUCCESS)
              s_error("can't fetch dialog box");
         XtManageChild(widget_selmis_dialog);
/*         set_something_val(XtNameToWidget(main_window_widget,"m_select_button"),(void*) XmNsensitive,False);*/
         break;

      case k_selpag_dialog:

            if (MrmFetchWidget(s_RMHierarchy, "selpag_dialog", widget_appoggio_dialog,
                   &widget_selpag_dialog, &dummy_class) != MrmSUCCESS)
              s_error("can't fetch dialog box");
            XtManageChild(widget_selpag_dialog);
         break;

      case k_singlevar_dialog:

        if (widget_singlevar_dialog==NULL)
        if (MrmFetchWidget(s_RMHierarchy, "singlevar_dialog", widget_appoggio_dialog,
                       &widget_singlevar_dialog, &dummy_class) != MrmSUCCESS)
              s_error("can't fetch dialog box");
         XtManageChild(widget_singlevar_dialog);
         break;

      case k_mempag_dialog :

            cerca_pagina(XtParent(XtParent(XtParent(XtParent(w)))));
            p_pagcur->pogg[k_mempag_dialog]=NULL;
	    flag_retsavepage=1;
            if (MrmFetchWidgetOverride(s_RMHierarchy, "mempag_dialog",p_pagcur->w,NULL,NULL,0,&p_pagcur->pogg[k_mempag_dialog], &dummy_class) !=MrmSUCCESS)
              s_error("can't fetch dialog box");
            XtManageChild(p_pagcur->pogg[k_mempag_dialog]);
      break;

      case k_history_dialog:

	cerca_pagina(XtParent(XtParent(XtParent(XtParent(w)))));
	if (p_indvar[p_pagcur->toggle_attivo] == -1) break;
	p_pagcur->pogg[widget_num]=NULL;
	i=0;
	history_x = (history_x > 600) ? 0 : history_x+100;
	history_y = (history_y > 800) ? 0 : history_y+100;
	XtSetArg(args[i],XmNx,history_x);i++;
	XtSetArg(args[i],XmNy,history_y); i++;
        if (MrmFetchWidgetOverride(s_RMHierarchy, "history_dialog", p_pagcur->w,
            NULL,args,i,&p_pagcur->pogg[widget_num],&dummy_class) != MrmSUCCESS)
              s_error("can't fetch dialog box");
         XtManageChild(p_pagcur->pogg[widget_num]);
         break;

	case k_start:
	if (!stato_agg) 
	{
	 stato_agg=1;
        time_id=XtAppAddTimeOut(XtWidgetToApplicationContext(main_window_widget),(unsigned long)500,timer_proc,NULL);
          set_something_val(w,XmNbackground,(XtArgVal)excolor[5].pixel);
          set_something_val(XtNameToWidget(XtParent(w),"stop_mandb"),(void*) XmNbackground,(XtArgVal)excolor[1].pixel);
	}
	break;
	
	case k_stop:
	if (stato_agg )
	{
	XtRemoveTimeOut(time_id);
	stato_agg=0;
	time_id =0;
          set_something_val(XtNameToWidget(XtParent(w),"start_mandb"),(void*) XmNbackground,(XtArgVal)excolor[1].pixel);
          set_something_val(w,XmNbackground,(XtArgVal)excolor[5].pixel);
	}
	break;

	case k_selmis_elimina:

		cancella_var(p_pagcur->toggle_attivo);
	break;

	case k_clear_page:
            cerca_pagina(XtParent(XtParent(XtParent(XtParent(w)))));
	    for (i=0; i<MAX_VAR; i++)
		if (p_indvar[i] != -1) cancella_var(i);
	break;


	case k_close_page:

	    cerca_pagina(XtParent(XtParent(XtParent(XtParent(w)))));

/* se e' stata modificata la pagina corrente (tolte o inserite variabili )
   viene attivata la box di salvataggio pagina , se invece sono stati modificati
   solo i valori salva automaticamente solo quelli*/

           if (p_pagcur->modifica_var )
	   {
		flag_retsavepage=0;
	        p_pagcur->pogg[k_mempag_dialog]=NULL;
          	if (MrmFetchWidgetOverride(s_RMHierarchy, "mempag_dialog",p_pagcur->w,NULL,NULL,0,&p_pagcur->pogg[k_mempag_dialog], &dummy_class) != MrmSUCCESS)
              	s_error("can't fetch dialog box");
           	XtManageChild(p_pagcur->pogg[k_mempag_dialog]);
          }
          else
          {
                if (p_pagcur->modifica_valsp )
                {
		  write_page_mandb(indice_pagina,0);  /* salva solo i valori */
                  p_pagcur->modifica_valsp =0;
                }
	  /*
		chiude la pagina corrente
	  */
/*		XtRemoveEventHandler(p_pagcur->w,ButtonPressMask,False,PostIt,k_bull_main);*/
		XtFree((char*)p_pagcur->pogg);	
                XtDestroyWidget(p_pagcur->w);
		p_pagcur->w=0;
		crea_lista_pagevis(widget_list_pagevis);
          }
	break;

        case k_cut_button:

            cerca_pagina(XtParent(XtParent(XtParent(XtParent(w)))));
	    if (p_indvar[p_pagcur->toggle_attivo] == -1) break;
	    indice_var_cut=p_indvar[p_pagcur->toggle_attivo];
	    cancella_var(p_pagcur->toggle_attivo);
	    
	break;

	case k_paste_button:
	     if (indice_var_cut == -1) break;
             cerca_pagina(XtParent(XtParent(XtParent(XtParent(w)))));
	     aggiungi_var(p_pagcur->toggle_attivo,indice_var_cut);
	break;
        }
}


static void map_proc(w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;
{
int i;
XmString x_app,x_blank;
char stringa[20],stringa2[101];
char stringa3[20],stringa4[20];
int gborder;
int widget_num = *tag;

switch(widget_num)
        {
	 
	case k_bull_main:

        x_blank=XmStringCreateLtoR("          ",XmSTRING_DEFAULT_CHARSET);
	cerca_pagina(w);
	for (i=0; i< MAX_VAR; i++)
	{
	  if (p_indvar[i] != -1)
	  {
	    if (p_pagcur->toggle_attivo == -1 ) p_pagcur->toggle_attivo=i;
            strcpy(stringa,variabili[p_indvar[i]].nome);
            strcpy(stringa2,variabili[p_indvar[i]].descr);
	    if (p_tempi_sp[i] != -1)
	    {
	       sprintf(stringa3,"%9.3f",p_valori_sp[i]);
	        sprintf(stringa4,"%6.2f",p_tempi_sp[i]);
		gborder=1;
            }
	    else
	    {
		strcpy(stringa3,"        ");
		strcpy(stringa4,"     ");
		gborder=0;
	    }
	  }
    	  else
	  {
            strcpy(stringa,"           ");
            strcpy(stringa2,"                                                ");
            strcpy(stringa3,"        ");
            strcpy(stringa4,"     ");
            set_something_val(p_pagcur->pogg[i+k_valore_var1],XmNbackground,(XtArgVal)excolor[1].pixel);
	    gborder=0;

	  }
           x_app=XmStringCreateLtoR(stringa,XmSTRING_DEFAULT_CHARSET);
           set_something_val(p_pagcur->pogg[i+k_toggle_var1],XmNlabelString,(XtArgVal)x_app);
	XmStringFree(x_app);
           x_app=XmStringCreateLtoR(stringa2,XmSTRING_DEFAULT_CHARSET);
           set_something_val(p_pagcur->pogg[i+k_descr_var1],XmNlabelString,(XtArgVal)x_app);
           set_something_val(p_pagcur->pogg[i+k_valore_var1],XmNlabelString,(XtArgVal)x_blank);
	XmStringFree(x_app);
	    XmTextSetString(p_pagcur->pogg[i+k_valore_sp1],stringa3);
	   x_app=XmStringCreateLtoR(stringa4,XmSTRING_DEFAULT_CHARSET);
           set_something_val(p_pagcur->pogg[i+k_tempo_sp1],XmNlabelString,(XtArgVal)x_app);
          set_something_val(p_pagcur->pogg[i+k_tempo_sp1],XmNborderWidth,(XtArgVal)gborder);
	XmStringFree(x_app);
	}
	if (p_pagcur->toggle_attivo == -1 ) p_pagcur->toggle_attivo=0;
	sprintf(stringa2," PAGINA  %d  --  ",indice_pagina+1);
        if (strlen (descr_page[indice_pagina]))
                        strcat(stringa2,descr_page[indice_pagina]);
        else
                        strcat(stringa2,"  disponibile  ");
        x_app=XmStringCreateLtoR(stringa2,XmSTRING_DEFAULT_CHARSET);
        set_something_val(p_pagcur->w,XmNdialogTitle,(XtArgVal)x_app);
        XmToggleButtonSetState(p_pagcur->pogg[k_toggle_var1+p_pagcur->toggle_attivo],True,True);
	XmStringFree(x_app);
	XmStringFree(x_blank);
	break;
	}
}

unsigned int find_pos;   /* posizione corrente nel find  */

static void find_proc(w, tag, selez)
    Widget w;
    int *tag;
    XmSelectionBoxCallbackStruct *selez;
{
int i,trovata, j;
char *pstr;   /* puntatore alla stringa editata (da ricercare) */
Widget wl;

i=0;
pstr=XmTextGetString(XtNameToWidget(widget_selmis_dialog,"find_text"));
RtRecord(pstr);
trovata=0;
switch(*tag)
    {
    case k_find_next:
    i=find_pos;
    do
        {
        if(i<tot_variabili)
                i++;
        else
                i=0;
        if(Utstrstr(variabili[i].nome,pstr)!=NULL 
		|| Utstrstr(variabili[i].descr,pstr)!=NULL)
                {
 		/* se la stringa e' stata trovata  */
                trovata=1;
                find_pos=i;
                break;
                }
        }
    while(i!=find_pos);

    if(trovata)   /* se la variabile e' stata trovata la seleziona nella
                     lista visualizzata  */
        {
        UtListaScroll(widget_selmis_box,tot_variabili,16,find_pos);
        }
    break;
    case k_find_previous:
    i=find_pos;
    do
        {
        if(i>0)
                i--;
        else
                i=tot_variabili;
        if(Utstrstr(variabili[i].nome,pstr)!=NULL  ||
        	Utstrstr(variabili[i].descr,pstr)!=NULL)
                {
		 /* se la stringa e' stata trovata  */
                trovata=1;
                find_pos=i;
                break;
                }
        }
    while(i!=find_pos);

    if(trovata)   /* se la variabile e' stata trovata la seleziona nella
                     lista visualizzata  */
        {
        UtListaScroll(widget_selmis_box,tot_variabili,16,find_pos);
        }
    break;
    }
XtFree(pstr);
}


/*
     funzione richiamata all'atto della creazione dei widget
 */
static void toggle_proc(w, tag, toggle)
    Widget w;
    int *tag;
    XmToggleButtonCallbackStruct *toggle;
{
int widget_num = *tag;

/* per trovare l' indice di widget della pagina fa 2 xtparent perche'
   la catena e' la seguente : pagina -> radiobox -> toggle_button */
cerca_pagina(XtParent(XtParent(w)));
p_pagcur->toggle_attivo= widget_num - k_toggle_var1;
}

/*
	Calbacks per i tasti di 'Abbandona'
 */
static void cancel_proc(w, tag, selez)
    Widget w;
    int *tag;
    XmSelectionBoxCallbackStruct *selez;
{
XmString x_app;
int i;
Widget wtasto;

switch(*tag)
        {
        case k_selmis_dialog:
        XtUnmanageChild (widget_selmis_dialog);
/*         set_something_val(XtNameToWidget(main_window_widget,"m_select_button"),(void*) XmNsensitive,True); */
        break;

        case k_mempag_dialog:

        cerca_pagina(XtParent(XtParent(XtParent(w))));
        XtDestroyWidget(p_pagcur->pogg[k_mempag_dialog]);
        p_pagcur->pogg[k_mempag_dialog]=NULL;
	p_pagcur->modifica_var=0;
	if (!flag_retsavepage)
        	XtCallCallbacks(p_pagcur->pogg[k_close_page],XmNactivateCallback,(XtPointer)k_close_page);
        break;

        case k_selpag_dialog:

        XtDestroyWidget(widget_selpag_dialog);
        widget_selpag_dialog=0;
        break;

        case k_singlevar_dialog:

        XtDestroyWidget(widget_singlevar_dialog);
        widget_singlevar_dialog=0;
        break;


        case k_history_dialog:

        XtDestroyWidget(XtParent(w));
        break;

	}
}

/*
 * The user pushed the quit button, so the application exits.
 */
static void quit_proc(w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;
{
int i;
Widget werr_dialog;
XmString x_app;
Arg args[10];

    if (tag != NULL)
/* verifica se ci sono delle pagine aperte con dati da salvare
   e in caso affermativo avverte l'utente */

	for (i=0; i< MAX_PAGINE; i++)
	{
		if (pagina[i].w != NULL && pagina[i].modifica_var )
		{
			werr_dialog=0;
                        sprintf(err_message,"La pagina < %d  -  %s >  deve essere salvata",i+1,descr_page[i]);
                        x_app=XmStringCreateLtoR(err_message,XmSTRING_DEFAULT_CHARSET);
                        XtSetArg(args[0],XmNmessageString,x_app);
                        if (MrmFetchWidgetOverride(s_RMHierarchy, "err_dialog" , widget_appoggio_dialog,NULL,args,1,&werr_dialog,&dummy_class) != MrmSUCCESS)
                      s_error("can't fetch dialog box");
                        XtManageChild(werr_dialog);
			XmStringFree(x_app);
                        return;
                   }
	}

#if defined UNIX
        kill(pid_acqmandb,SIGUSR1);
	sleep(2);
#endif

/*
	quando esce vengono azzerati tutti i valori di set point e
relativi tempi di tutte le pagine */
	reset_valori_sp();
	    sem_close(idsem_locale);
	    msg_close(msgqid);
            distruggi_var(id_sh);
            distruggi_shrmem(shmvid_mandb);
/*            sim_shraddr_free(); */
	    exit(1);
}

/*
 s_error
    per errori FATALI : stampa un messaggio ed esce dall'applicazione
 */

int s_error(problem_string)
    char *problem_string;
{
    printf("%s\n", problem_string);
    exit(0);
}

void lista_var(lista)
Widget lista;
/* *************** Definizione variabili locali  ***************** */
{
int k;
char testo[MAX_LUN_NOME_VAR+ MAX_LUN_DESCR_VAR +1];
XmString x_simboli;


for( k =0 ; k < tot_variabili ; k++ )
{
   sprintf(testo,"%s - %s",variabili[k].nome,variabili[k].descr);
   x_simboli=XmStringCreateLtoR(testo,XmSTRING_DEFAULT_CHARSET);
   XmListAddItemUnselected(lista,x_simboli,0); 
   XmStringFree(x_simboli);
}
}


/*
	task periodico provvisorio che visualizza i valori contenuti in 
	area shared
*/
static void timer_proc(client_data,id)
caddr_t client_data;
XtIntervalId *id;
{
int ind_valore;
float valore;
char stringa[20];
XmString x_app;
Arg args[10]; /* per settaggio argomenti widgets */
int    *p_ind;
int    *p_point;
CAMP  *p_val;
PAGE_VIS *p_pag;
int i,ipag,j,old_pointin;
float tempo;

/*
   esegue il refresh solo se e' stato incrementato il tempo di simulazione 
*/
RtDbPGetTime(dbpunti,&tempo);
if (old_time != tempo)
{
   RtDbPGetTime(dbpunti,&old_time);

/*  Per tutte le pagine aperte visualizza il valore delle variabili prelevandolo direttamente dall' area shared del simulatore */
	
  for (ipag=0; ipag< MAX_PAGINE; ipag++)
  {
    if (pagina[ipag].w != NULL)
    {
           p_ind =  &p_db->indice_var[ipag*MAX_VAR];
           p_point =  &p_db->pointin[ipag*MAX_VAR];
           p_val =  &p_db->valori[ipag*MAX_VAR];
           p_pag = &pagina[ipag];

	   for (i=0; i< MAX_VAR; i++)
	   {
		if (p_ind[i] == -1 ) continue;
		ind_valore= variabili[p_ind[i]].addr;
		valore  = estr_sh(ind_valore,0);
		sprintf(stringa,"%9.3f",valore);
		x_app= XmStringCreateLtoR(stringa,XmSTRING_DEFAULT_CHARSET);
       		 j=0;
		XtSetArg(args[j],XmNlabelString,x_app);j++;
		old_pointin=p_point[i]-1;
        	if (old_pointin < 0 ) old_pointin=MAX_CAMPIONI -1 ;	

/*
	se il tempo del campione precedente e' 0 significa che la
	variabile e' appena stata aggiunta percio' escluse il test sul
	valore precedente 
*/
		if (p_val[i].tempo[old_pointin] != 0.)
		{
	
/*printf("\n i : %d   pointin %d valore %f  old_pointin %d  old_val %f\n",i,p_pointin[i],valore,old_pointin ,p_valori[i].valore[old_pointin]);*/
			if (valore != p_val[i].valore[old_pointin])
			{
				XtSetArg(args[j],XmNbackground,sfondo_label.pixel);j++;
			}
			else
			{
	                	XtSetArg(args[j],XmNbackground,excolor[1].pixel);j++;
			}
		}
		else
		{
			XtSetArg(args[j],XmNbackground,excolor[1].pixel);j++;
		}

		XtSetValues(p_pag->pogg[i+k_valore_var1],args,j);
		XmStringFree(x_app);

    	  }
    }
  }

}
time_id=XtAppAddTimeOut(XtWidgetToApplicationContext(main_window_widget),
                                   (unsigned long)500,timer_proc,NULL);
}

float estr_sh(indice,funct)
int indice;
int funct;  /* se >0 converte in logical  */
{


float val; /* puntatore di appoggio */

val=RtDbPGetValueD(dbpunti,indice);

return(val);
}

void crea_lista_page(w)
Widget w;
{
int i;
char stringa[MAX_LUN_DESCRPAGE+10];
XmString x_app;


for (i=0; i< MAX_PAGINE; i++)
{
        sprintf(stringa,"%2d - ",i+1);
        if (strlen(descr_page[i])) strcat(stringa,descr_page[i]);
        else strcat(stringa,"disponibile                       ");
        x_app=XmStringCreateLtoR(stringa,XmSTRING_DEFAULT_CHARSET);
        XmListAddItemUnselected(w,x_app,0);
	XmStringFree(x_app);
}
        XmListSelectPos(w,indice_pagina+1,True);
}


void crea_lista_pagevis(w)
Widget w;
{
int i,pos;
char stringa[MAX_LUN_DESCRPAGE+10];
char aux[10];


XmListDeleteItem(w,x_list_pagevis[0]);
XmStringFree(x_list_pagevis[0]);
for (i=1; i< (num_pagevis-1); i++) {
   XmListDeleteItem(w,x_list_pagevis[i]);
/*
   printf("crea_lista_pagevis: DEBUG num_pagevis = %d\n",num_pagevis);
*/
   XmStringFree(x_list_pagevis[i]);
}
num_pagevis=1;
for (i=0; i< MAX_PAGINE; i++)
{
	if (pagina[i].w == NULL) continue;
	memset(stringa,' ',MAX_LUN_DESCRPAGE+5);
	stringa[MAX_LUN_DESCRPAGE+5-1]=0;
        sprintf(aux,"%2d - ",i+1);
	memcpy(stringa,aux,5);
       	if (strlen(descr_page[i])) memcpy(&stringa[5],descr_page[i],strlen(descr_page[i]));
        /* stringa e' gia' piena di spazi: basta il testo. Prima si copiavano
           MAX_LUN_DESCRPAGE byte da una costante che ne aveva uno di meno. */
        else memcpy(&stringa[5],"nuova pagina",12);
        x_list_pagevis[num_pagevis-1]=XmStringCreateLtoR(stringa,XmSTRING_DEFAULT_CHARSET);
        XmListAddItem(w,x_list_pagevis[num_pagevis-1],num_pagevis);
	num_pagevis++;
}
if (num_pagevis == 1)
{
        memset(stringa,' ',MAX_LUN_DESCRPAGE+5);
        stringa[MAX_LUN_DESCRPAGE+5-1]=0;
	x_list_pagevis[0]=XmStringCreateLtoR(stringa,XmSTRING_DEFAULT_CHARSET);
        XmListAddItem(w,x_list_pagevis[0],1);
}
}

/*
	crea la lista degli ultimi 50  valori e dei tempi per la variabile
	selezionata
*/


void crea_lista_valori(indvar)
int indvar;
{
int i;
char stringa[MAX_CAMPIONI];
int p_inizio;


p_inizio = p_db->pointin[indvar] -1;
if (p_inizio < 0) p_inizio= MAX_CAMPIONI -1;
for (i=0; i< MAX_CAMPIONI; i++)
{
        sprintf(stringa,"  %2d    --    %9.3f    --    %6.2f",(i+1),p_valori[indvar].valore[p_inizio],p_valori[indvar].tempo[p_inizio]);
        x_list_history[i]=XmStringCreateLtoR(stringa,XmSTRING_DEFAULT_CHARSET);
	p_inizio--;
	if (p_inizio < 0) p_inizio= MAX_CAMPIONI -1;
}
}

/*
	cerca pagina
	sistema tutti i puntatori alla pagina corrente 
*/
void cerca_pagina(w) 
Widget w;
{
int ipag;

if ( p_pagcur != NULL && w == p_pagcur->w) return;
for (ipag=0; ipag<MAX_PAGINE; ipag++)
{
	if (pagina[ipag].w == w )
	{
             p_indvar =  &p_db->indice_var[ipag*MAX_VAR];
             p_pointin =  &p_db->pointin[ipag*MAX_VAR];
             p_valori =  &p_db->valori[ipag*MAX_VAR];
             p_valori_sp= &valori_sp[ipag*MAX_VAR];
             p_tempi_sp= &tempi_sp[ipag*MAX_VAR];
	     p_pagcur = &pagina[ipag];
             indice_pagina=ipag;
	     break;
	}
}
if (ipag == MAX_PAGINE) { printf("PAGINA NON ESISTENTE \n");exit (puts(" grave errore "));}
}

/*
	cancella dalla pagina in visualizzazione una variabile
*/
void cancella_var(indice)
int indice;
{
XmString x_app;
int j;

         if (p_indvar[indice] != -1)
         {
                sem_wait(idsem_locale);
                p_pointin[indice]=0;
                for (j=0; j< MAX_CAMPIONI; j++)
                {
                  p_valori[indice].valore[j]=0.;
                  p_valori[indice].tempo[j]=0.;
                }
                sem_signal(idsem_locale);
        }

	x_app = XmStringCreateLtoR("                         ",XmSTRING_DEFAULT_CHARSET);
	set_something_val(p_pagcur->pogg[k_toggle_var1+indice],XmNlabelString,(XtArgVal)x_app);
	set_something_val(p_pagcur->pogg[k_descr_var1+indice],XmNlabelString,(XtArgVal)x_app);
        set_something_val(p_pagcur->pogg[k_valore_var1+indice],XmNlabelString,(XtArgVal)x_app);
        set_something_val(p_pagcur->pogg[k_valore_var1+indice],XmNbackground,(XtArgVal)excolor[1].pixel);
        set_something_val(p_pagcur->pogg[k_tempo_sp1+indice],XmNlabelString, (XtArgVal)x_app);
        set_something_val(p_pagcur->pogg[k_tempo_sp1+indice],XmNborderWidth,(XtArgVal)0);
	XmTextSetString(p_pagcur->pogg[k_valore_sp1+indice],"");
        p_indvar[indice] = -1;
        p_valori_sp[indice]=0.;
        p_tempi_sp[indice]= -1.;
        memset(p_pagcur->sigle_vis[indice],0,MAX_LUN_NOME_VAR);
        p_pagcur->modifica_var=1;
        XmStringFree(x_app);
}

/*
	aggiunge_var
	inserisce una variabile in una pagina nella posizione
	toggle_attivo 
*/

void aggiungi_var(posiz,indice_sel)
int posiz;		/* posizione all' interno della pagaina */
int indice_sel;		/* indice variabile selezionata nella lista */
{
int i,j;
XmString x_app;

/* se esisteva gia una variabile annulla tutti i campioni memorizzati */

if (p_indvar[posiz] != -1)
	{
		sem_wait(idsem_locale);
		p_pointin[posiz]=0;
		for (j=0; j< MAX_CAMPIONI; j++)
		{
		   p_valori[posiz].valore[j]=0.;
		   p_valori[posiz].tempo[j]=0.;
		}
		sem_signal(idsem_locale);
	}

x_app=XmStringCreateLtoR(variabili[indice_sel].nome,XmSTRING_DEFAULT_CHARSET);
set_something_val(p_pagcur->pogg[k_toggle_var1+posiz],XmNlabelString,(XtArgVal)x_app);
XmStringFree(x_app);

x_app=XmStringCreateLtoR(variabili[indice_sel].descr,XmSTRING_DEFAULT_CHARSET);
set_something_val(p_pagcur->pogg[k_descr_var1+posiz],XmNlabelString,(XtArgVal)x_app);
XmStringFree(x_app);

x_app=XmStringCreateLtoR("",XmSTRING_DEFAULT_CHARSET);
XmTextSetString(p_pagcur->pogg[k_valore_sp1+posiz],"");
set_something_val(p_pagcur->pogg[k_tempo_sp1+posiz],XmNlabelString,(XtArgVal)x_app);
set_something_val(p_pagcur->pogg[k_tempo_sp1+posiz],XmNborderWidth,(XtArgVal)0);
XmStringFree(x_app);
p_valori_sp[posiz]=0.;
p_tempi_sp[posiz]= -1.;
p_indvar[posiz] = indice_sel;
strcpy(p_pagcur->sigle_vis[posiz],variabili[indice_sel].nome);
/*
for (i=0; i<MAX_VAR; i++)
	printf(" %d  %s \n ",p_indvar[i],p_pagcur->sigle_vis[i]);
*/
p_pagcur->modifica_var=1;
}


static void PostIt(w,num,event)
Widget w;
int *num;
XButtonEvent *event;
{
cerca_pagina(w);
printf(" postit %d pagina attiva %d\n",w,indice_pagina);
}

static void focus_proc(w, tag, selez)
    Widget w;
    int *tag;
    XmAnyCallbackStruct *selez;
{

if (*tag == k_bull_main) 
	{ cerca_pagina(w);
	printf(" postit %d pagina attiva %d\n",*tag,indice_pagina);
	}
}

