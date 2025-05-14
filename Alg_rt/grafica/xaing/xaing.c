/**********************************************************************
*
*       C Source:               xaing.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Mar 31 11:04:18 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: xaing.c-9 %  (%full_filespec: xaing.c-9:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)xaing.c	1.17\t6/18/96";
/*
   modulo xaing.c
   tipo 
   release 1.17
   data 6/18/96
   reserved @(#)xaing.c	1.17
*/
/*
 * XAING
 *   processo aing realizzato sotto DEC_Windows
 */
#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <sys/msg.h>
#endif
#if defined VMS
# include "vmsipc.h"
#endif
# include <errno.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Text.h>
#include <Xm/MainW.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/BulletinB.h>
#include <Xm/Label.h>
#include <Xm/Separator.h>
#include <Xm/Form.h>
#include <Xm/Xm.h>

# include "uni_mis.h" 
#include "sim_param.h"
#include "sim_types.h"
#include "sim_ipc.h"
#include "comandi.h"
#include "xaing.h"
#include "libutilx.h"
#include "ric_aing.h"
# include <Rt/RtMemory.h>
# include <Rt/RtDbPunti.h>
# include <Rt/RtErrore.h>
#include <sqlite3.h>

int     font_widthn;

/*
 Definizione callbacks functions
 */

static  void aing_proc ();
static  void timer_proc ();
static  void pag_del_callback ();/* callback di distruzione window */
static  void aing_del_callback ();/* callback di uscita dal programma */
void fine_aing ();	/* callback di uscita dal programma con SIGTERM   */
static  void PostIt ();
static  void aing_refresh ();/* callback da chiamarsi scaduto il timeout di refresh */
static  void invia_callback ();/* callback del bottone invia step */
static  void impulso_callback ();/* callback del bottone impulso */
static  void impulsostop_callback ();/* callback del bottone stop impulso */
static  void not_callback ();/* callback del bottone not */
static  void up_callback ();/* callback up del bottone updown */
static  void down_callback ();/* callback down del bottone updown */
static  void start_callback ();/* callback del bottone start rampa */
static  void stop_callback ();/* callback del bottone stop rampa */
static  void rampa_grad_callback ();/* callback del bottone gradiente */
static  void rampa_delta_callback ();/* callback del bottone dt */
static  void rampa_time_callback ();/* callback del bottone dt */
static  void time_callback ();/* callback del bottone time */
static  void delta_callback ();/* callback del bottone delta */
static  void tinc_callback ();/* callback del bottone tinc */
static  void tipo_normal_callback ();/* callback del bottone tipo pert normal */
static  void tipo_periodic_callback ();  /* callback del bottone tipo 
						pert periodic */
static  void tipo_noise_callback ();  /* callback del bottone 
					tipo pert white noise */
static  void sinusoidal_start_callback(); /* start sinusoide */
static  void sinusoidal_stop_callback(); /* stop sinusoide */
static  void trapezoidal_start_callback(); /* start trapezoidal */
static  void trapezoidal_stop_callback(); /* stop trapezoidal */
static  void whitenoise_start_callback(); /* start whitenoise */
static  void whitenoise_stop_callback(); /* stop whitenoise */
static  void legge_elenco_display(); /* routine di lettura elenco display
					dal file Context    */
static  void open_display();
static  int cerca_display();

static  void malfunction_start_callback(); /* start malfunction */
static  void malfunction_stop_callback();  /* stop malfunction */
static  void tipo_malf_callback ();  /* callback del bottone 
					tipo pert malfunction */
/*
 Definizione funzioni di utilita'
 */
float   estr_sh ();

int add_refresh (XtCallbackProc, caddr_t);


RtDbPuntiOggetto dbpunti;
RtErroreOggetto errore_obj;

sqlite3 *db;


/*
	informazioni relative alle variabili di ingresso perturbabili
*/
char   *ind_sh_top;
VARIABILI * variabili;
int     tot_variabili;
int     nmod;

/*
 * Dati globali necessari per le routines di grafica
 */
Display * display;
int     screen_num;
XFontStruct * font_info[MAX_DISPLAY];
XmFontList font_list[MAX_DISPLAY];
char   *colori[] =
{
    "black", "white", "red", "light gray", "cyan"
};

XColor excolor[MAX_DISPLAY][12];
/*
char eeueeeeee[1000];
*/
int     font_ascent;
int     font_height;
int     font_width;
/*
char eeeeeeee[1000];
*/
GC gc[5][MAX_DISPLAY];
unsigned long   colors[6];/* colori privati per la visualizzazione dei grafici (su due piani) */

S_AING aing[MAX_AING];	/* informazioni relative ad ogni stazione AING visualizzata */
XaingCallback t_call[MAX_AING];/* callback functions che vengono associate al timeout */
extern  S_UNI_MIS uni_mis[];/* tabella che descrive tutte le unita' di misura  */
int     num_umis;	/* numero unita' di misura definite in tabella uni_mis */
XtAppContext app_context;
Widget topLevel;
Arg args[50];		/* per settaggio argomenti widgets */
Widget main_window_widget;
Widget main_window_shell;
XtIntervalId time_id, time2_id;

#define MAX_LUN_VALORESH 129
#define EF_LEGO 7
#define EF_AING 6
long    mask_ef_lego, mask_ef_aing;



/* definizioni per la coda di messaggio delle perturbazioni */
int     id_msg_pert;
MSG_PERT messaggio;

/* definizioni per la coda di messaggio con MONIT */
int     id_msg_aing;
int     id_msg_ret_aing;



/* identificatore var_sh */
int     id_sh;		/* identificatore sh_var */

/* identificatore tipo aing */
int     tipo_aing = 0;	/* 0 comunicazione con LEGOGRAF */
 /* 1 comunicazione con MONIT    */
RICHIESTA_AING richiesta_aing;
RITORNO_AING ritorno_aing;
TIPO_PERT perturbazione;

/*
	vettore dei display
*/
AN_DISPLAY *elenco_display;
int numero_display;
int disp_sel=0;
int versione_mmi;
char * nome_file_context;



void init_aing ();
void costruisci_var (char**, VARIABILI **, int*);
void init_gc (int );
int new_aing (int);
//int del_refresh (void*);
int del_refresh ();
int cerca_umis(char*);
void agg_stato (int);
//int add_refresh (XtCallbackProc, void*);
int pert_malfunction(int ,int ,float ,float ,float );
int pert_malfunction_stop(int ,int ,float );
void agg_stato (int);
//int add_refresh (XtCallbackProc, void*);
void load_font (XFontStruct **,int);









int main (argc, argv)
unsigned int    argc;	/* Command line argument count. */
char   *argv[];		/* Pointers to command line args. */
{
    Widget wbutton, wmenu;
char   *file;
char   *pathfile;
char   *path_aing_header;
char    pathdir[1024];

int     shr_usr_key;	/* chiave utente per shm */
int     i,j;
/*
pid_t  fpid;
*/
int     ier, mask;
long    mask_ef;
#if defined VMS
char    app_id_msg_aing_1[4];
char    app_id_msg_aing_2[4];
#endif
SIMULATOR *sim;

/* redirezione output  e stampa versione */
/***
    testata ("xaing",SccsID);
*********/

    tipo_aing = atoi (argv[1]);



    printf ("\t Tipo aing = %d  \n", tipo_aing);

	if(tipo_aing==2)
		{
		chdir(argv[2]);
    		nome_file_context=argv[3];
		}


/*
        si alloca l'area shared
*/
    shr_usr_key = atoi ((char *) getenv ("SHR_USR_KEY"));
    sim = (SIMULATOR*)calloc(1,sizeof(SIMULATOR));
    if (tipo_aing == 1)
    {
/**
	sim_shlego (shr_usr_key); **//* lato shedulatore */
        errore_obj = RtCreateErrore(RT_ERRORE_TERMINALE,"xaing");
   	dbpunti = RtCreateDbPunti(errore_obj,NULL,DB_PUNTI_SHARED,sim);
    }
    else
    {
/**
	sim_shraddr (shr_usr_key); **//* lato legograf    */
        errore_obj = RtCreateErrore(RT_ERRORE_TERMINALE,"xaing");
   	dbpunti = RtCreateDbPunti(errore_obj,NULL,DB_PUNTI_INT,sim);
    }
    free(sim);

    if(tipo_aing == 2)
    {
	versione_mmi=1;
    }
    else
	versione_mmi=0;

/* creazione o aggancio code messaggi */
    msg_create_fam (shr_usr_key, 0);
#if defined VMS
    memcpy (&app_id_msg_aing_1[0], &id_msg_aing, sizeof (int));
    memcpy (&app_id_msg_aing_2[0], &app_id_msg_aing_1[2], sizeof (short));
    memcpy (&app_id_msg_aing_2[2], &app_id_msg_aing_1[0], sizeof (short));
    memcpy (&id_msg_aing, &app_id_msg_aing_2[0], sizeof (int));
#endif
/*
  inizializza tabella per refresh
 */
    	init_aing ();

/*
    topLevel = XtInitialize ("xaing", "Xaing", NULL, 0,
	    &argc, argv);
*/
/***
    topLevel = XtAppInitialize( &app_context,"Xaing", NULL,0,
		 &argc, argv, NULL, NULL, 0);
******/
/*****
    app_context=XtWidgetToApplicationContext(topLevel);
*********/
topLevel = XtAppInitialize(&app_context, "xaing",
                                     NULL, 0, &argc, argv, NULL, NULL, 0);

        display = XtDisplay(topLevel);
        screen_num = XDefaultScreen(display);

        XtVaSetValues(topLevel,
                        XmNx, 0,
                        XmNy, 0,
                        XmNwidth, DisplayWidth(display, screen_num),
                        XmNheight, DisplayHeight(display, screen_num),
                        NULL);

 /*
	legge l'elenco dei display
  */
 if(tipo_aing==2)
	{
	legge_elenco_display();
	open_display();
	}
 else
	{
        numero_display=1;
	disp_sel=0;
        elenco_display=
		(AN_DISPLAY*)XtCalloc(numero_display+1,sizeof(AN_DISPLAY));
	elenco_display[0].topLevel=XtAppCreateShell("xaing","Xaing",
                        applicationShellWidgetClass,display,NULL,0);
	}



    /*   installazione signal    */
    signal (SIGTERM, fine_aing);

/*
 lettura file delle unita' di misura 
 */
    if (getcwd (pathdir, 100) == 0)
	printf ("\n errore in lettura path");
    chdefaults ();
    init_umis ();
    num_umis = cerca_num_umis ();
    chdir (pathdir);
/*
 lettura delle variabili di XAInG
 */

    costruisci_var (&ind_sh_top, &variabili, &id_sh);
    tot_variabili = numero_variabili (ind_sh_top);
    nmod = numero_modelli (ind_sh_top);


    for(j=0;j<numero_display;j++)
{
    set_something (elenco_display[j].topLevel, XmNiconic, (void*) True);
    i = 0;
    XtSetArg (args[i], XmNwidth, 150);
    i++;
    XtSetArg (args[i], XmNheight, 40);
    i++;
    main_window_widget = XmCreateMainWindow (elenco_display[j].topLevel, 
		"pagina1", args, i);



    init_gc (j);

    i = 0;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Quit", XmSTRING_DEFAULT_CHARSET));
    i++;
    wbutton = XmCreatePushButton (main_window_widget, "bottone", args, i);
    XtManageChild (wbutton);
    XtAddCallback (wbutton, XmNactivateCallback, aing_del_callback, NULL);

    XtManageChild (main_window_widget);


    XtRealizeWidget (elenco_display[j].topLevel);
}
/*
  timer per attesa di messaggio visualizzazione plot
*/
    time_id = XtAppAddTimeOut (app_context,
	    (unsigned long) 1000, aing_proc, NULL);
    time2_id = XtAppAddTimeOut (app_context,
	    (unsigned long) 400, timer_proc, NULL);


    XtAppMainLoop (app_context);
}

static  void timer_proc (client_data, id)
        void * client_data;
XtIntervalId * id;
{
int     i;

    for(i=0;i<MAX_AING;i++)
	if(t_call[i].callback != NULL)
		t_call[i].callback (t_call[i].closure);

    time2_id = XtAppAddTimeOut (app_context,
	    (unsigned long) 400, timer_proc, NULL);
}


static  void aing_proc (client_data, id)
        void *client_data;
XtIntervalId * id;
{
int     i, ip3;
long    mask_ef;

/*
 esamina se e' stata richiesta una stazione AING
 */
    if ((tipo_aing == 1)||(tipo_aing == 2))
    {
	if (msg_rcv (id_msg_aing, &richiesta_aing, 
			sizeof (RICHIESTA_AING)-sizeof(long),
		    	(long)RIC_AING, IPC_NOWAIT, 0) > 0)
	{
	    for (i = 0; i < MAX_AING; i++)
	    {
		if (!aing[i].attiva)
		    break;
	    }
	    if (i == MAX_AING)
	    {
		printf ("\n warning: raggiunto il massimo numero staz. AING");
	    }
	    else
	    {
		memcpy(&perturbazione,
			&richiesta_aing.perturbazione,sizeof(TIPO_PERT));
		new_aing (i);	/* ricerca la variabile richiesta se la variabile e' corretta visualizza la stazione AING */
	    }
	}
    }
/* 
 ricarica il time - out
 */
    time_id = XtAppAddTimeOut (app_context,
	    (unsigned long) 1000, aing_proc, NULL);
}


static  void pag_del_callback (w, info, str)
        Widget w;
void *info;
XmSelectionBoxCallbackStruct * str;
{
int     is = (int) info;
Display *display;

    if (aing[is].w != 0)
    {
	if(aing[is].legocad == 1)
	    {
	    ritorno_aing.mtype = 1;
	    if (w == aing[is].sw[k_quit])
		ritorno_aing.modifica_pert= PERT_ANNULLA;
	    else
		ritorno_aing.modifica_pert= PERT_MODIFICATA;
            msg_snd(id_msg_ret_aing,&ritorno_aing,
			sizeof(RITORNO_AING) - sizeof(long),IPC_NOWAIT);
	    }
	display=XtDisplay(aing[is].wtopLevel);
	del_refresh (info);
	XtUnmanageChild (aing[is].wtopLevel);
	aing[is].attiva = 0;
	XSync (display, False);
	aing[is].w = 0;
	aing[is].legocad = 0;
    }
}


static  void aing_del_callback (w, info, str)
        Widget w;
void   *info;
XmSelectionBoxCallbackStruct * str;
{
/* sgancio code messaggi */
    msg_close_fam ();
    exit (0);
}

void fine_aing (signum, code, scp)
int     signum, code;
struct sigcontext  *scp;
{

/* sgancio code messaggi */
    msg_close_fam ();

    exit (0);
}

int new_aing (ip3)
int     ip3;
{
    Widget wTopLev;
    Widget wmenu, wbutton, wpopup, wdescr, wapp, wRow;
char    app[MAX_LUN_VALORESH];
char    appstr[50];
int     tot_width;	/* larghezza totale */
int     tot_height;	/* altezza totale */
int     val_width;	/* ampiezza label per visualizzazione valore */
static Dimension  x = -50;
static Dimension  y = 500;
int     ivar;
int     i;
float   k;
int     flag, it, is;
int     ind_umis;
float   valstep;
float   tempo_imp;
int     tot_inc=15;
int modifica = 0;

    if (tipo_aing == 1)
    {
	x = x + 50;
	if (x > 400)
	    x = 0;
	y = y + 50;
	if (y > 800)
	    y = 500;
	strncpy (app, richiesta_aing.nome_variabile, MAX_LUN_NOME_VAR);
	disp_sel=cerca_display(richiesta_aing.nome_display);
    }
    if (tipo_aing == 2)
	{
	x=richiesta_aing.x;
	y=richiesta_aing.y;
	strncpy (app, richiesta_aing.nome_variabile, MAX_LUN_NOME_VAR);
	disp_sel=cerca_display(richiesta_aing.nome_display);
	}


/*
 ricerca la variabile richiesta nella lista delle variabili
 ricavandone l'indice
 */
    for (i = 0; i < tot_variabili; i++)
    {
	if ((memcmp (variabili[i].nome, app, strlen (app)) == 0) && (variabili[i].tipo == INGRESSO_NC))
	{
	    ivar = i;
	    break;
	}
    }
    if (i == tot_variabili)
    {
	printf (" \n VARIABILE %s NON TROVATA ", app);
	return (1);		/* ritorno per pagina non trovata */
    }
    /*
	Verifica che non sia una aing di modifica
    */
    if(perturbazione.indirizzo== variabili[ivar].addr)
	{
	modifica=1;
	printf("AING MODIFICA\n");
	}

/*
 verifica che la variable non sia gia' presente in un'altra stazione
 visualizzata.
 */
if(tipo_aing!=2)
    for (i = 0; i < MAX_AING; i++)
    {
	if (aing[i].attiva && aing[i].ind_var == ivar)
	    {
	    if(modifica == 1)
		{
	    	ritorno_aing.mtype = 1;
	    	ritorno_aing.modifica_pert= PERT_ERRORE;
            	msg_snd(id_msg_ret_aing,&ritorno_aing,
			sizeof(RITORNO_AING) - sizeof(long),IPC_NOWAIT);
		}
	    return (2);
	    }
    }
else
    for (i = 0; i < MAX_AING; i++)
	 if (aing[i].attiva && aing[i].ind_var == ivar &&
		(strcmp(aing[i].nome_display,richiesta_aing.nome_display)==0))
            return(2);
/*
 riempimento struttura descrittiva della pagina visualizzata e
 creazione della window di rappresentazione della pagina
*/
    strcpy (aing[ip3].nome_display,richiesta_aing.nome_display);
    aing[ip3].tipo_pert = TIPO_PERT_NORMAL;
    aing[ip3].grad_dt_time = 0;
    aing[ip3].delta_time = 0;
    aing[ip3].valt = 0.;
    aing[ip3].attiva = 1;
    aing[ip3].ind_var = ivar;
    aing[ip3].x = x;
    aing[ip3].y = y;
    aing[ip3].select = variabili[ivar].addr;
    aing[ip3].modello = variabili[ivar].mod;
    strcpy (aing[ip3].nome_var, app);
/* estrae la descrizione della variabile */
    i = 21;
    while (variabili[ivar].descr[i] == ' ' && i < LUN_DES_VAR)
	i++;
    memcpy (aing[ip3].descrizione_v, &variabili[ivar].descr[i],
	    LUN_SIMB69 - i);
    aing[ip3].descrizione_v[LUN_SIMB69 - i] = 0;

    for (i = strlen (aing[ip3].descrizione_v) - 1; i > 0; i--)
    {
	if (aing[ip3].descrizione_v[i] == ' ')
	{
	    aing[ip3].descrizione_v[i] = 0;
	}
	else
	    break;
    }
/*
 ricava l'unita' di misura utilizzata
 */
    ind_umis = cerca_umis (aing[ip3].nome_var);
    aing[ip3].a = uni_mis[ind_umis].A[uni_mis[ind_umis].sel];
    aing[ip3].b = uni_mis[ind_umis].B[uni_mis[ind_umis].sel];
    strcpy (aing[ip3].umis, uni_mis[ind_umis].codm[uni_mis[ind_umis].sel]);

/*
 ricava la situazione relativa alla eventuale rampa
 per il momento non viene fatto il test e si ammette che la rampa
 in quanto non e' possibile attivare una AING di una variabile di cui
 e' in corso una perturbazione.
 */
    aing[ip3].valfin = estr_sh (aing[ip3].select);
    aing[ip3].rateo = ((1.- aing[ip3].b) / aing[ip3].a)/60.;
    aing[ip3].ists = 0;
    aing[ip3].isegn = 1;

    valstep = estr_sh (aing[ip3].select);
    tempo_imp = 1;
/* inserisce i valori dello step */
    aing[ip3].tempo_imp = 1;
    aing[ip3].val_imp = estr_sh (aing[ip3].select);



    if(modifica == 1)
	{
	aing[ip3].legocad = 1;
        aing[ip3].valt =  perturbazione.t;
	perturbazione.valore_delta = perturbazione.valore_delta * aing[ip3].a +
					aing[ip3].b;
	switch (perturbazione.tipo)
		{
		case PERT_MALFUNCTION:
			{
                        aing[ip3].valfin = perturbazione.valore_delta;
                        printf("\t PERT_MALFUNCTION modifica valfin=%f\n",
                                aing[ip3].valfin);
			break;
			}
		case PERT_MALFUNCTION_STOP:
			{
                        aing[ip3].valfin = perturbazione.valore_delta;
                        printf("\t PERT_MALFUNCTION_STOP modifica valfin=%f\n",
                                aing[ip3].valfin);
			break;
			}
		case PERT_SCALINO:
			{
    			valstep = perturbazione.valore_delta;
			break;
			}
		case PERT_RAMPA:
			{
			aing[ip3].rateo= 
				(perturbazione.rateo_durata*aing[ip3].a+
				  aing[ip3].b);
    			aing[ip3].valfin = perturbazione.valore_delta;
			break;
			}
		case PERT_RAMPA_STOP:
			{
			aing[ip3].rateo= 
				(perturbazione.rateo_durata*aing[ip3].a+
				  aing[ip3].b) * 60.;
    			aing[ip3].valfin = perturbazione.valore_delta;
			break;
			}
		case PERT_IMPULSO:
			{
    			aing[ip3].tempo_imp = perturbazione.rateo_durata;
    			aing[ip3].val_imp =  perturbazione.valore_delta;
			break;
			}
		case PERT_IMPULSO_STOP:
			{
    			aing[ip3].tempo_imp = perturbazione.rateo_durata;
    			aing[ip3].val_imp =  perturbazione.valore_delta;
			break;
			}
		case PERT_UP:
			{
			break;
			}
		case PERT_DOWN:
			{
			break;
			}
		case PERT_NOT:
			{
			break;
			}
		case PERT_TO:
			{
		        printf("PERTURBAZIONE PERT_TO\n");
    			aing[ip3].grad_dt_time = 1;
			aing[ip3].rateo = -perturbazione.rateo_durata/60.;
    			aing[ip3].valfin = perturbazione.valore_delta;
			break;
			}
		case PERT_TO_STOP:
			{
    			aing[ip3].grad_dt_time = 1;
			aing[ip3].rateo = -perturbazione.rateo_durata/60.;
    			aing[ip3].valfin = perturbazione.valore_delta;
			break;
			}
		}
	}




    i = 0;
    font_widthn = 6;
/*printf("font_info->max_bounds.width=%d %d\n",font_width,
	(int)(font_info->max_bounds.width));*/
    tot_width = font_widthn * 78 + 5 + 45+10;/* vecchio valore 52 */
    tot_height = 202-5;
/*printf("tot_width=%d tot_height=%d\n",tot_width,tot_height);*/
if(tipo_aing==2)
	{
	x=x-tot_width/2;
	y=y-(tot_height+tot_inc)/2;
	}
    XtSetArg (args[i], XmNx, x); i++;
    XtSetArg (args[i], XmNy, y); i++;

    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNwidth, tot_width); i++;
    XtSetArg (args[i], XmNheight, tot_height+tot_inc); i++;
    XtSetArg (args[i], XmNminWidth, tot_width); i++;
    XtSetArg (args[i], XmNminHeight, tot_height+tot_inc); i++;
    XtSetArg (args[i], XmNwindowGroup, XtUnspecifiedWindowGroup); i++;
    XtSetArg (args[i], XmNdialogStyle, XmDIALOG_MODELESS);
    i++;
    XtSetArg (args[i], XmNnoResize, True);
    i++;
    XtSetArg (args[i], XmNresizePolicy, XmRESIZE_GROW);
    i++;
    XtSetArg (args[i], XmNwindowGroup,
	 RootWindow(XtDisplay(elenco_display[disp_sel].topLevel),
	DefaultScreen(XtDisplay(elenco_display[disp_sel].topLevel))));i++;
if(modifica == 1)
	{
	sprintf(app,"Modify input on %s",aing[ip3].nome_var);
    XtSetArg (args[i], XmNdialogTitle, XmStringCreateLtoR (app, XmSTRING_DEFAULT_CHARSET)); i++;
	}
else
	{
    XtSetArg (args[i], XmNdialogTitle, XmStringCreateLtoR (aing[ip3].nome_var, XmSTRING_DEFAULT_CHARSET)); i++;
	}
    XtSetArg (args[i], XmNdefaultPosition, False);
    i++;

/*
aing[ip3].w=XmCreateBulletinBoardDialog(topLevel,aing[ip3].nome_var,args,i);
*/
if(modifica == 1)
         aing[ip3].wtopLevel = XtCreatePopupShell (app, 
		topLevelShellWidgetClass,elenco_display[disp_sel].topLevel, args, i);
else
         aing[ip3].wtopLevel = XtCreatePopupShell (aing[ip3].nome_var, 
		topLevelShellWidgetClass,elenco_display[disp_sel].topLevel, args, i);

    i = 0;
    XtSetArg (args[i], XmNresizePolicy, XmRESIZE_GROW);i++;
    aing[ip3].w = XmCreateForm (aing[ip3].wtopLevel, "form_aing", args, i);





/* ****************
 *                *
 * CREAZIONE DEI  *
 * WIDGET FIGLI   *
 *                *
 * **************** */
/*
    ROW COLUMN
*/
    i = 0;
    XtSetArg (args[i], XmNx, 0);
    i++;
    XtSetArg (args[i], XmNy, 0);
    i++;
    XtSetArg (args[i], XmNspacing, 0);
    i++;
    XtSetArg (args[i], XmNorientation, XmVERTICAL);
    i++;
    XtSetArg (args[i], XmNpacking, XmPACK_NONE);
    i++;
    XtSetArg (args[i], XmNresizeHeight, False);
    i++;
    XtSetArg (args[i], XmNresizeWidth, False);
    i++;
    XtSetArg (args[i], XmNwidth, tot_width);
    i++;
    XtSetArg (args[i], XmNheight, tot_height+tot_inc);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    wRow = XmCreateRowColumn (aing[ip3].w, app, args, i);
    XtManageChild (wRow);

/*
    DESCRIZIONE
*/
    i = 0;
    XtSetArg (args[i], XmNx, 2);
    i++;
    XtSetArg (args[i], XmNy, 2);
    i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNheight, font_height + 4);
    i++;
    XtSetArg (args[i], XmNwidth, /* font_widthn*78 */ tot_width - 4);
    i++;
    XtSetArg (args[i], XmNrecomputeSize, False);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR (aing[ip3].descrizione_v, XmSTRING_DEFAULT_CHARSET));
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    XtSetArg (args[i], XmNforeground, excolor[disp_sel][2].pixel);
    i++;
    wdescr = XmCreateLabel (wRow, "label", args, i);
    XtManageChild (wdescr);


/*
    VAL IMP
*/
    val_width = 80;
    i = 0;
    XtSetArg (args[i], XmNx, (tot_width - val_width) / 2 - 70 -30);
    i++;
    XtSetArg (args[i], XmNy, font_height * 1.8+5);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    XtSetArg (args[i], XmNwidth, val_width);
    i++;
    sprintf (appstr, "%10.2f", aing[ip3].val_imp);
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR (appstr, XmSTRING_DEFAULT_CHARSET));
    i++;
    XtSetArg (args[i], XmNborderWidth, 2);
    i++;
    aing[ip3].sw[k_val_corr] = XmCreateLabel (wRow, "label", args, i);
    XtManageChild (aing[ip3].sw[k_val_corr]);

/* 
    UNITA" DI MISURA
*/
    i = 0;
    XtSetArg (args[i], XmNx, (tot_width - val_width) / 2 + val_width + 5 - 70 -35);
    i++;
    XtSetArg (args[i], XmNy, font_height * 2 + 4);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNwidth, val_width);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR (aing[ip3].umis, XmSTRING_DEFAULT_CHARSET));
    i++;
    wapp = XmCreateLabel (wRow, "label", args, i);
    XtManageChild (wapp);

/* 
    CURRENT VALUE
*/
    i = 0;
    XtSetArg (args[i], XmNx, (tot_width) / 2 - 165 - 70 - 20);
    i++;
    XtSetArg (args[i], XmNy, font_height * 2 + 4+5);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNwidth, val_width);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Current Value",
		XmSTRING_DEFAULT_CHARSET));
    i++;
    wapp = XmCreateLabel (wRow, "label", args, i);
    XtManageChild (wapp);

/*
*/
    i = 0;
    XtSetArg (args[i], XmNx, tot_width / 3);
    i++;
    XtSetArg (args[i], XmNy, font_height * 5+tot_inc);
    i++;
    XtSetArg (args[i], XmNorientation, XmVERTICAL);
    i++;
    XtSetArg (args[i], XmNheight, tot_height - font_height * 5);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    wapp = XmCreateSeparator (wRow, "separator", args, i);
    XtManageChild (wapp);

/*
*/
    i = 0;
    XtSetArg (args[i], XmNx, 10 + 2 * tot_width / 3);
    i++;
    XtSetArg (args[i], XmNy, font_height * 5+tot_inc);
    i++;
    XtSetArg (args[i], XmNorientation, XmVERTICAL);
    i++;
    XtSetArg (args[i], XmNheight, tot_height - font_height * 5);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    wapp = XmCreateSeparator (wRow, "separator", args, i);
    XtManageChild (wapp);

/* 
	label STEP
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 4+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, (tot_width / 3 - font_widthn * 4) / 2);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNforeground, excolor[disp_sel][2].pixel);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("STEP", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_string_step] = XmCreateLabel (wRow, "label", args, i);
    XtManageChild ( aing[ip3].sw[k_string_step]);

/* 
	label GENERAL
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 4+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, (tot_width / 3 - font_widthn * 4) / 2);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNforeground, excolor[disp_sel][2].pixel);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("GENERAL", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_string_general] = XmCreateLabel (wRow, "label", args, i);

/* 
	label WHITE NOISE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 4+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, (tot_width / 3 - font_widthn * 4) / 2);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNforeground, excolor[disp_sel][2].pixel);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("WHITE NOISE", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_string_whitenoise] = XmCreateLabel (wRow, "label", args, i);

/* 
	label MALFUNCTION
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 4+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, (tot_width / 3 - font_widthn * 4) / 2);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNforeground, excolor[disp_sel][2].pixel);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("MALFUNCTION", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_string_malfunction] = XmCreateLabel (wRow, "label", args, i);


/*
	label PULSE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 4+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, tot_width / 3 + (tot_width / 3 - font_widthn * 5) / 2);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNforeground, excolor[disp_sel][2].pixel);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("PULSE", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_string_pulse] = XmCreateLabel (wRow, "label", args, i);
    XtManageChild (aing[ip3].sw[k_string_pulse]);

/*
	label SINUSOIDAL
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 4+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, tot_width / 3 + (tot_width / 3 - font_widthn * 5) / 2);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNforeground, excolor[disp_sel][2].pixel);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("SINUSOIDAL", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_string_sinusoidal] = XmCreateLabel (wRow, "label", args, i);



/*
	label RAMP
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 4+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, 10 + 2 * tot_width / 3 + (tot_width / 3 - font_widthn * 4) / 2);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNforeground, excolor[disp_sel][2].pixel);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("RAMP", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_string_ramp] = XmCreateLabel (wRow, "label", args, i);
    XtManageChild (aing[ip3].sw[k_string_ramp]);

/*
	label TRAPEZOIDAL
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 4+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, 10 + 2 * tot_width / 3 + (tot_width / 3 - font_widthn * 4) / 2);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNforeground, excolor[disp_sel][2].pixel);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("TRAPEZOIDAL", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_string_trapezoidal] = XmCreateLabel (wRow, "label", args, i);


/*
	label PERIOD
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Period :", XmSTRING_DEFAULT_CHARSET)); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    aing[ip3].sw[k_string_period] = XmCreateLabel (wRow, "label", args, i);

/*
	TEXT PERIOD
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn * 14); i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNcolumns, 10); i++;
    XtSetArg (args[i], XmNmaxLength, 10); i++;
    XtSetArg (args[i], XmNresizeHeight, False); i++;
    XtSetArg (args[i], XmNresizeWidth, False); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_text_period] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "0.");
    XmTextSetString (aing[ip3].sw[k_text_period], appstr);

/*
        label TARGET MALFUNCTION
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Target :", XmSTRING_DEFAULT_CHARSET)); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    aing[ip3].sw[k_string_targetmalf] = XmCreateLabel (wRow, "label", args, i);

/*
	TEXT TARGET MALFUNCTION 
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn * 14); i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNcolumns, 10); i++;
    XtSetArg (args[i], XmNmaxLength, 10); i++;
    XtSetArg (args[i], XmNresizeHeight, False); i++;
    XtSetArg (args[i], XmNresizeWidth, False); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_text_targetmalf] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "%10.2f", aing[ip3].valfin * aing[ip3].a + aing[ip3].b);
    XmTextSetString (aing[ip3].sw[k_text_targetmalf], appstr);

/*
        label TYPE MALFUNCTION
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 8+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Type :", XmSTRING_DEFAULT_CHARSET)); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    aing[ip3].sw[k_string_typemalf] = XmCreateLabel (wRow, "label", args, i);

/*
        TEXT TYPE MALFUNCTION
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 8+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn * 14); i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNcolumns, 10); i++;
    XtSetArg (args[i], XmNmaxLength, 10); i++;
    XtSetArg (args[i], XmNresizeHeight, False); i++;
    XtSetArg (args[i], XmNresizeWidth, False); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_text_typemalf] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "0.");
    XmTextSetString (aing[ip3].sw[k_text_typemalf], appstr);


/*
	label VARIANCE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Variance :", XmSTRING_DEFAULT_CHARSET)); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    aing[ip3].sw[k_string_variance] = XmCreateLabel (wRow, "label", args, i);

/*
	TEXT VARIANCE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn * 14); i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNcolumns, 10); i++;
    XtSetArg (args[i], XmNmaxLength, 10); i++;
    XtSetArg (args[i], XmNresizeHeight, False); i++;
    XtSetArg (args[i], XmNresizeWidth, False); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_text_variance] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "0.");
    XmTextSetString (aing[ip3].sw[k_text_variance], appstr);



/*
	label PHASE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 12+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Phase :", XmSTRING_DEFAULT_CHARSET)); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    aing[ip3].sw[k_string_phase] = XmCreateLabel (wRow, "label", args, i);

/*
	TEXT PHASE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 12+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn * 14); i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNcolumns, 10); i++;
    XtSetArg (args[i], XmNmaxLength, 10); i++;
    XtSetArg (args[i], XmNresizeHeight, False); i++;
    XtSetArg (args[i], XmNresizeWidth, False); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_text_phase] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "0.");
    XmTextSetString (aing[ip3].sw[k_text_phase], appstr);

/*
	label MEAN VALUE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 8+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Mean value :", XmSTRING_DEFAULT_CHARSET)); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    aing[ip3].sw[k_string_meanvalue] = XmCreateLabel (wRow, "label", args, i);

/*
	TEXT MEAN VALUE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 8+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn * 14); i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNcolumns, 10); i++;
    XtSetArg (args[i], XmNmaxLength, 10); i++;
    XtSetArg (args[i], XmNresizeHeight, False); i++;
    XtSetArg (args[i], XmNresizeWidth, False); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_text_meanvalue] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "%10.2f", valstep * aing[ip3].a + aing[ip3].b);
    XmTextSetString (aing[ip3].sw[k_text_meanvalue], appstr);

/*
	label WIDE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 10+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Wide :", XmSTRING_DEFAULT_CHARSET)); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    aing[ip3].sw[k_string_wide] = XmCreateLabel (wRow, "label", args, i);

/*
	TEXT WIDE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 10+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn * 14); i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNcolumns, 10); i++;
    XtSetArg (args[i], XmNmaxLength, 10); i++;
    XtSetArg (args[i], XmNresizeHeight, False); i++;
    XtSetArg (args[i], XmNresizeWidth, False); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_text_wide] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "0.");
    XmTextSetString (aing[ip3].sw[k_text_wide], appstr);

/*
	label T1
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc); i++;
    XtSetArg (args[i], XmNx, 2*tot_width/3 + font_widthn+15); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("T1 :", XmSTRING_DEFAULT_CHARSET)); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    aing[ip3].sw[k_string_t1] = XmCreateLabel (wRow, "label", args, i);

/*
	TEXT T1
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc); i++;
    XtSetArg (args[i], XmNx, 2*tot_width/3 + font_widthn * 14+5); i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNcolumns, 10); i++;
    XtSetArg (args[i], XmNmaxLength, 10); i++;
    XtSetArg (args[i], XmNresizeHeight, False); i++;
    XtSetArg (args[i], XmNresizeWidth, False); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_text_t1] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "0.");
    XmTextSetString (aing[ip3].sw[k_text_t1], appstr);


/*
	label T2
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 8+tot_inc); i++;
    XtSetArg (args[i], XmNx, 2*tot_width/3 + font_widthn+15); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("T2 :", XmSTRING_DEFAULT_CHARSET)); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    aing[ip3].sw[k_string_t2] = XmCreateLabel (wRow, "label", args, i);

/*
	TEXT T2
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 8+tot_inc); i++;
    XtSetArg (args[i], XmNx, 2*tot_width/3 + font_widthn * 14+5); i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNcolumns, 10); i++;
    XtSetArg (args[i], XmNmaxLength, 10); i++;
    XtSetArg (args[i], XmNresizeHeight, False); i++;
    XtSetArg (args[i], XmNresizeWidth, False); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_text_t2] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "0.");
    XmTextSetString (aing[ip3].sw[k_text_t2], appstr);

/*
	label T3
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 10+tot_inc); i++;
    XtSetArg (args[i], XmNx, 2*tot_width/3 + font_widthn+15); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("T3 :", XmSTRING_DEFAULT_CHARSET)); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    aing[ip3].sw[k_string_t3] = XmCreateLabel (wRow, "label", args, i);

/*
	TEXT T3
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 10+tot_inc); i++;
    XtSetArg (args[i], XmNx, 2*tot_width/3 + font_widthn * 14+5); i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNcolumns, 10); i++;
    XtSetArg (args[i], XmNmaxLength, 10); i++;
    XtSetArg (args[i], XmNresizeHeight, False); i++;
    XtSetArg (args[i], XmNresizeWidth, False); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_text_t3] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "0.");
    XmTextSetString (aing[ip3].sw[k_text_t3], appstr);



/* 
	label TARGET STEP
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Target :", XmSTRING_DEFAULT_CHARSET)); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    aing[ip3].sw[k_string_targetstep] = XmCreateLabel (wRow, "label", args, i);
    XtManageChild (aing[ip3].sw[k_string_targetstep]);

/*
	TEXT TARGET STEP
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc); i++;
    XtSetArg (args[i], XmNx, font_widthn * 12); i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNcolumns, 10); i++;
    XtSetArg (args[i], XmNmaxLength, 10); i++;
    XtSetArg (args[i], XmNresizeHeight, False); i++;
    XtSetArg (args[i], XmNresizeWidth, False); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_val_step] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "%10.2f", valstep * aing[ip3].a + aing[ip3].b);
    XmTextSetString (aing[ip3].sw[k_val_step], appstr);
    XtManageChild (aing[ip3].sw[k_val_step]);

/*
	TEXT DELAY
*/
    i = 0;
    XtSetArg (args[i], XmNx, (tot_width - val_width) / 2 + val_width + 125 -75); i++;
    XtSetArg (args[i], XmNy, font_height * 2 - 2+5 ); i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNcolumns, 10); i++;
    XtSetArg (args[i], XmNmaxLength, 10); i++;
    XtSetArg (args[i], XmNresizeHeight, False); i++;
    XtSetArg (args[i], XmNresizeWidth, False); i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_delay] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "%10.1f", aing[ip3].valt);
    XmTextSetString (aing[ip3].sw[k_delay], appstr);
    XtManageChild (aing[ip3].sw[k_delay]);

/*
i=0;
XtSetArg(args[i],XmNx,(tot_width-val_width)/2+val_width+105); i++;
XtSetArg(args[i],XmNy,font_height*2+4+tot_inc); i++;
XtSetArg(args[i],XmNfont,font_list[disp_sel]); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("Time/deltaT",XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNbackground,excolor[disp_sel][3].pixel); i++;
wapp=XmCreateLabel(wRow,"label",args,i);
XtManageChild(wapp);
*/

/*
    MENU' OPTION PER LA SCELTA FRA PERTURBAZIONI NORMALI E PERIODICHE
*/
    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    aing[ip3].sw[k_tipo_pull] = XmCreatePulldownMenu (wRow, "tipo_optionsubmenu", args, i);

/*
	label e P.BUTTON NORMAL (option menu)
*/
    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Normal:", XmSTRING_DEFAULT_CHARSET)); i++;
    aing[ip3].sw[k_tipo_normal] = XmCreatePushButton (aing[ip3].sw[k_tipo_pull], "tipo_option_normal", args, i);
    XtAddCallback (aing[ip3].sw[k_tipo_normal], XmNactivateCallback, tipo_normal_callback, (XtPointer)ip3);
    XtManageChild (aing[ip3].sw[k_tipo_normal]);

/*
	label e P.BUTTON PERIODIC (option menu)
*/
    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Periodic:", XmSTRING_DEFAULT_CHARSET)); i++;
    aing[ip3].sw[k_tipo_periodic] = XmCreatePushButton (aing[ip3].sw[k_tipo_pull], "tipo_option_periodic", args, i);
    XtAddCallback (aing[ip3].sw[k_tipo_periodic], XmNactivateCallback, tipo_periodic_callback, (XtPointer)ip3);
    XtManageChild (aing[ip3].sw[k_tipo_periodic]);

/*
	label e P.BUTTON NOISE (option menu)
*/
    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Noise :", XmSTRING_DEFAULT_CHARSET)); i++;
    aing[ip3].sw[k_tipo_noise] = XmCreatePushButton (aing[ip3].sw[k_tipo_pull], "tipo_option_noise", args, i);
    XtAddCallback (aing[ip3].sw[k_tipo_noise], XmNactivateCallback, tipo_noise_callback, (XtPointer)ip3);
    XtManageChild (aing[ip3].sw[k_tipo_noise]);

/*
	label e P.BUTTON MALFUNCTION (option menu)
*/
    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Malf. :", XmSTRING_DEFAULT_CHARSET)); i++;
    aing[ip3].sw[k_tipo_malfunction] = XmCreatePushButton (aing[ip3].sw[k_tipo_pull], "tipo_option_malf", args, i);
    XtAddCallback (aing[ip3].sw[k_tipo_malfunction], XmNactivateCallback, tipo_malf_callback, (XtPointer)ip3);
    XtManageChild (aing[ip3].sw[k_tipo_malfunction]);

    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel); i++;
    XtSetArg (args[i], XmNsubMenuId, aing[ip3].sw[k_tipo_pull]); i++;
    XtSetArg (args[i], XmNx, (tot_width - val_width) / 2 + val_width + 5 +115); i++;
    XtSetArg (args[i], XmNy, font_height * 2 - 6+5); i++;
   XtSetArg (args[i], XmNfont, font_list[disp_sel]); i++;
    aing[ip3].sw[k_tipo_option] = XmCreateOptionMenu (wRow, "tipo_option_menu", args, i);
    XtManageChild (aing[ip3].sw[k_tipo_option]);
set_something(XmOptionLabelGadget(aing[ip3].sw[k_tipo_option]),
	XmNlabelString,(char *)XmStringCreateLtoR ("", XmSTRING_DEFAULT_CHARSET));
/*
	fine scelta fra perturbazioni normali e periodiche
*/



/*
    MENU' OPTION PER LA SCELTA DEL TIPO DI DELAY
*/
    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    aing[ip3].sw[k_time_pull] = XmCreatePulldownMenu (wRow, "time_optionsubmenu", args, i);

/*
	label e P.BUTTON DELAY (option menu)
*/
    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Delay:", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_time_delta] = XmCreatePushButton (aing[ip3].sw[k_time_pull], "time_option_dt", args, i);
    XtAddCallback (aing[ip3].sw[k_time_delta], XmNactivateCallback, delta_callback, (XtPointer)ip3);
    XtManageChild (aing[ip3].sw[k_time_delta]);

/*
	label e P.BUTTON START AT (option menu)
*/
    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Start at:", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_time_time] = XmCreatePushButton (aing[ip3].sw[k_time_pull], "time_option_dt", args, i);
    XtAddCallback (aing[ip3].sw[k_time_time], XmNactivateCallback, time_callback, (XtPointer)ip3);
    XtManageChild (aing[ip3].sw[k_time_time]);

/*
	label e P.BUTTON TIME (option menu)
*/
    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Time:", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_time_tinc] = XmCreatePushButton (aing[ip3].sw[k_time_pull], "time_option_dt", args, i);
    XtAddCallback (aing[ip3].sw[k_time_tinc], XmNactivateCallback, tinc_callback, (XtPointer)ip3);
    XtManageChild (aing[ip3].sw[k_time_tinc]);

    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNsubMenuId, aing[ip3].sw[k_time_pull]);
    i++;
    XtSetArg (args[i], XmNx, (tot_width - val_width) / 2 + val_width + 5 -65);
    i++;
    XtSetArg (args[i], XmNy, font_height * 2 - 6+5);
    i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]);
    i++;
    aing[ip3].sw[k_time_option] = XmCreateOptionMenu (wRow, "time_option_menu", args, i);
    XtManageChild (aing[ip3].sw[k_time_option]);
set_something(XmOptionLabelGadget(aing[ip3].sw[k_time_option]),
	XmNlabelString,(char*)XmStringCreateLtoR ("", XmSTRING_DEFAULT_CHARSET));
/*
    Fine menu' option per la scelta del tipo di delay
*/


/*
	label TARGET IMPULSO
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, 10 + 2 * tot_width / 3 + font_widthn);
    i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Target :", XmSTRING_DEFAULT_CHARSET));
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    aing[ip3].sw[k_string_targetimp] = XmCreateLabel (wRow, "label", args, i);
    XtManageChild (aing[ip3].sw[k_string_targetimp]);

/*
	label TARGET RAMP
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, tot_width / 3 + font_widthn);
    i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Target :", XmSTRING_DEFAULT_CHARSET));
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    aing[ip3].sw[k_string_targetramp]  = XmCreateLabel (wRow, "label", args, i);
    XtManageChild (aing[ip3].sw[k_string_targetramp]);

/*
	TEXT TARGET RAMP
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, 10 + 2 * tot_width / 3 + font_widthn * 14+5);
    i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNcolumns, 10);
    i++;
    XtSetArg (args[i], XmNmaxLength, 10);
    i++;
    XtSetArg (args[i], XmNresizeHeight, False);
    i++;
    XtSetArg (args[i], XmNresizeWidth, False);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    aing[ip3].sw[k_val_finale] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "%10.2f", aing[ip3].valfin * aing[ip3].a + aing[ip3].b);
    XmTextSetString (aing[ip3].sw[k_val_finale], appstr);
    XtManageChild (aing[ip3].sw[k_val_finale]);

/*
	TEXT TARGET IMPULSO
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 6+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, tot_width / 3 + font_widthn * 14);
    i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNcolumns, 10);
    i++;
    XtSetArg (args[i], XmNmaxLength, 10);
    i++;
    XtSetArg (args[i], XmNresizeHeight, False);
    i++;
    XtSetArg (args[i], XmNresizeWidth, False);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    aing[ip3].sw[k_val_imp] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "%10.2f", aing[ip3].val_imp * aing[ip3].a + aing[ip3].b);
    XmTextSetString (aing[ip3].sw[k_val_imp], appstr);
    XtManageChild (aing[ip3].sw[k_val_imp]);

/*
	label WIDE(s) IMPULSO
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 8+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, tot_width / 3 + font_widthn);
    i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Wide(s):", XmSTRING_DEFAULT_CHARSET));
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    aing[ip3].sw[k_string_wide_s] = XmCreateLabel (wRow, "label", args, i);
    XtManageChild (aing[ip3].sw[k_string_wide_s]);

/* vecchio gradiente 
i=0;
XtSetArg(args[i],XmNy,font_height*8+tot_inc); i++;
XtSetArg(args[i],XmNx,10+2*tot_width/3+font_widthn); i++;
XtSetArg(args[i],XmNfont,font_list[disp_sel]); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("Grad.:",XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNbackground,excolor[disp_sel][3].pixel); i++;
wapp=XmCreateLabel(wRow,"label",args,i);
XtManageChild(wapp);
*/



/*
    MENU' OPTION PER LA SCELTA DEL TIPO DI RAMPA
*/
    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    aing[ip3].sw[k_rampa_pull] = XmCreatePulldownMenu (wRow, "optionsubmenu", args, i);

/*
	label e P.BUTTON DELTA e GRAD (option menu)
*/
if ((modifica == 1) && (perturbazione.tipo == PERT_TO ))
    {
    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Delta:", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_rampa_delta] = XmCreatePushButton (aing[ip3].sw[k_rampa_pull], "option_dt", args, i);
    XtAddCallback (aing[ip3].sw[k_rampa_delta], XmNactivateCallback, rampa_delta_callback, (XtPointer)ip3);
    XtManageChild (aing[ip3].sw[k_rampa_delta]);

    i = 0;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Grad:", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_rampa_grad] =
	XmCreatePushButton (aing[ip3].sw[k_rampa_pull], "option_grad", args, i);
    XtAddCallback (aing[ip3].sw[k_rampa_grad], XmNactivateCallback, rampa_grad_callback, (XtPointer)ip3);
    XtManageChild (aing[ip3].sw[k_rampa_grad]);
    }
else
    {
    i = 0;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Grad:", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_rampa_grad] =
	XmCreatePushButton (aing[ip3].sw[k_rampa_pull], "option_grad", args, i);
    XtAddCallback (aing[ip3].sw[k_rampa_grad], XmNactivateCallback, rampa_grad_callback, (XtPointer)ip3);
    XtManageChild (aing[ip3].sw[k_rampa_grad]);

/*
	label e P.BUTTON TIME (option menu)
*/
    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Delta:", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_rampa_delta] = XmCreatePushButton (aing[ip3].sw[k_rampa_pull], "option_dt", args, i);
    XtAddCallback (aing[ip3].sw[k_rampa_delta], XmNactivateCallback, rampa_delta_callback, (XtPointer)ip3);
    XtManageChild (aing[ip3].sw[k_rampa_delta]);
    }

    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Time:", XmSTRING_DEFAULT_CHARSET));
    i++;
    aing[ip3].sw[k_rampa_time] = XmCreatePushButton (aing[ip3].sw[k_rampa_pull], "option_dt", args, i);
    XtAddCallback (aing[ip3].sw[k_rampa_time], XmNactivateCallback, rampa_time_callback, (XtPointer)ip3);
    XtManageChild (aing[ip3].sw[k_rampa_time]);

    i = 0;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    XtSetArg (args[i], XmNsubMenuId, aing[ip3].sw[k_rampa_pull]);
    i++;
    XtSetArg (args[i], XmNy, font_height * 8 - 3+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, 2 * tot_width / 3 + font_widthn - 8);
    i++;
    XtSetArg (args[i], XmNfont, font_list[disp_sel]);
    i++;
/*
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("",XmSTRING_DEFAULT_CHARSET)); i++;
*/
    aing[ip3].sw[k_rampa_option] = XmCreateOptionMenu (wRow, "option_menu", args, i);
    XtManageChild (aing[ip3].sw[k_rampa_option]);
set_something(XmOptionLabelGadget(aing[ip3].sw[k_rampa_option]),
	XmNlabelString,(char*)XmStringCreateLtoR ("", XmSTRING_DEFAULT_CHARSET));
/*
    Fine creazione menu' option per la scelta del tipo di rampa
*/



/*
*/
    i = 0;
    XtSetArg (args[i], XmNy, (int) (font_height * 9.5)+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, 10 + 2 * tot_width / 3 + font_widthn * 8);
    i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("/min.", XmSTRING_DEFAULT_CHARSET));
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][3].pixel);
    i++;
    aing[ip3].sw[k_string_min] = XmCreateLabel (wRow, "label", args, i);
    XtManageChild (aing[ip3].sw[k_string_min]);

/*
	TEXT TEMPO IMPULSO
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 8+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, tot_width / 3 + font_widthn * 14);
    i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNcolumns, 10);
    i++;
    XtSetArg (args[i], XmNmaxLength, 10);
    i++;
    XtSetArg (args[i], XmNresizeHeight, False);
    i++;
    XtSetArg (args[i], XmNresizeWidth, False);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    aing[ip3].sw[k_tempo_imp] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "%10.2f", aing[ip3].tempo_imp);
    XmTextSetString (aing[ip3].sw[k_tempo_imp], appstr);
    XtManageChild (aing[ip3].sw[k_tempo_imp]);

/*
	START IMPULSO
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 11+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, tot_width / 3 + font_widthn * 3);
    i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Start", XmSTRING_DEFAULT_CHARSET));
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    aing[ip3].sw[k_but_imp] = XmCreatePushButton (wRow, "button", args, i);
    XtAddCallback (aing[ip3].sw[k_but_imp], XmNactivateCallback, impulso_callback, (XtPointer)ip3);
    if(modifica == 1)
    XtAddCallback (aing[ip3].sw[k_but_imp], XmNactivateCallback, pag_del_callback, (XtPointer)ip3);

    XtManageChild (aing[ip3].sw[k_but_imp]);

/*
	START SINUSOIDE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 12+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, tot_width / 3 + font_widthn * 3);
    i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Start", XmSTRING_DEFAULT_CHARSET));
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    aing[ip3].sw[k_sin_start] = XmCreatePushButton (wRow, "button", args, i);
    XtAddCallback (aing[ip3].sw[k_sin_start], XmNactivateCallback, sinusoidal_start_callback, (XtPointer)ip3);
    if(modifica == 1)
    XtAddCallback (aing[ip3].sw[k_sin_start], XmNactivateCallback, pag_del_callback, (XtPointer)ip3);



/*
	START WHITE NOISE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 12+tot_inc+5);
    i++;
    XtSetArg (args[i], XmNx, font_widthn * 2);
    i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Start", XmSTRING_DEFAULT_CHARSET));
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    aing[ip3].sw[k_whitenoise_start] = XmCreatePushButton (wRow, "button", args, i);
    XtAddCallback (aing[ip3].sw[k_whitenoise_start], XmNactivateCallback, whitenoise_start_callback, (XtPointer)ip3);
    if(modifica == 1)
    XtAddCallback (aing[ip3].sw[k_whitenoise_start], XmNactivateCallback, pag_del_callback, (XtPointer)ip3);

/*
	STOP WHITE NOISE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 12+tot_inc+5);
    i++;
    XtSetArg (args[i], XmNx, font_widthn * 2 + 70);
    i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Stop", XmSTRING_DEFAULT_CHARSET));
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    aing[ip3].sw[k_whitenoise_stop] = XmCreatePushButton (wRow, "button", args, i);
    XtAddCallback (aing[ip3].sw[k_whitenoise_stop], XmNactivateCallback, whitenoise_stop_callback, (XtPointer)ip3);
    if(modifica == 1)
    XtAddCallback (aing[ip3].sw[k_whitenoise_stop], XmNactivateCallback, pag_del_callback, (XtPointer)ip3);


/*
	START MALFUNCTION
*/
    i = 0;
    XtSetArg (args[i], XmNy, 
              font_height * 12+tot_inc+5); i++;
    XtSetArg (args[i], XmNx, 
              font_widthn * 2); i++;
    XtSetArg (args[i], XmNfontList, 
              font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, 
              XmStringCreateLtoR ("Start", XmSTRING_DEFAULT_CHARSET)); i++;
    XtSetArg (args[i], XmNbackground, 
              excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_malfunction_start] = 
              XmCreatePushButton (wRow, "button", args, i);
    XtAddCallback (aing[ip3].sw[k_malfunction_start], 
                   XmNactivateCallback, malfunction_start_callback, (XtPointer)ip3);
    if(modifica == 1)
        XtAddCallback (aing[ip3].sw[k_malfunction_start], XmNactivateCallback, 
                       pag_del_callback, (XtPointer)ip3);

/*
	STOP MALFUNCTION
*/
    i = 0;
    XtSetArg (args[i], XmNy, 
              font_height * 12+tot_inc+5); i++;
    XtSetArg (args[i], XmNx, 
              font_widthn * 2 + 70); i++;
    XtSetArg (args[i], XmNfontList, 
              font_list[disp_sel]); i++;
    XtSetArg (args[i], XmNlabelString, 
              XmStringCreateLtoR ("Stop", XmSTRING_DEFAULT_CHARSET)); i++;
    XtSetArg (args[i], XmNbackground, 
              excolor[disp_sel][1].pixel); i++;
    aing[ip3].sw[k_malfunction_stop] = 
              XmCreatePushButton (wRow, "button", args, i);
    XtAddCallback (aing[ip3].sw[k_malfunction_stop], 
                   XmNactivateCallback, malfunction_stop_callback, (XtPointer)ip3);
    if(modifica == 1)
       XtAddCallback (aing[ip3].sw[k_malfunction_stop], 
                      XmNactivateCallback, pag_del_callback, (XtPointer)ip3);

/*
	STOP IMPULSO
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 11+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, tot_width / 3 + font_widthn * 3 + 70);
    i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Stop", XmSTRING_DEFAULT_CHARSET));
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    aing[ip3].sw[k_but_impstop] = XmCreatePushButton (wRow, "button", args, i);
    XtAddCallback (aing[ip3].sw[k_but_impstop], XmNactivateCallback, impulsostop_callback, (XtPointer)ip3);
    if(modifica == 1)
    XtAddCallback (aing[ip3].sw[k_but_impstop], XmNactivateCallback, pag_del_callback, (XtPointer)ip3);

    XtManageChild (aing[ip3].sw[k_but_impstop]);


/*
	STOP SINUSOIDE
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 12+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, tot_width / 3 + font_widthn * 3 + 70);
    i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Stop", XmSTRING_DEFAULT_CHARSET));
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    aing[ip3].sw[k_sin_stop] = XmCreatePushButton (wRow, "button", args, i);
    XtAddCallback (aing[ip3].sw[k_sin_stop], XmNactivateCallback, sinusoidal_stop_callback, (XtPointer)ip3);
    if(modifica == 1)
    XtAddCallback (aing[ip3].sw[k_sin_stop], XmNactivateCallback, pag_del_callback, (XtPointer)ip3);




/*
	TEXT GRAD
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 8+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, 10 + 2 * tot_width / 3 + font_widthn * 14+5);
    i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNcolumns, 10);
    i++;
    XtSetArg (args[i], XmNmaxLength, 10);
    i++;
    XtSetArg (args[i], XmNresizeHeight, False);
    i++;
    XtSetArg (args[i], XmNresizeWidth, False);
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    aing[ip3].sw[k_val_grad] = XmCreateText (wRow, "text", args, i);
    sprintf (appstr, "%10.2f", (aing[ip3].rateo * aing[ip3].a + aing[ip3].b) * 60);
    XmTextSetString (aing[ip3].sw[k_val_grad], appstr);
    XtManageChild (aing[ip3].sw[k_val_grad]);


/*
	START TRAPEZOIDALE
*/
	i = 0;
	XtSetArg (args[i], XmNy, font_height * 12+tot_inc);
	i++;
	XtSetArg (args[i], XmNx, 10 + 2 * tot_width / 3 + font_widthn * 3);
	i++;
	XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
	i++;
	XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Start", XmSTRING_DEFAULT_CHARSET));
	i++;
	XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
	i++;
	aing[ip3].sw[k_trap_start] = XmCreatePushButton (wRow, "button", args, i);
	XtAddCallback (aing[ip3].sw[k_trap_start], XmNactivateCallback, trapezoidal_start_callback, (XtPointer)ip3);
    if(modifica == 1)
	XtAddCallback (aing[ip3].sw[k_trap_start], XmNactivateCallback, pag_del_callback, (XtPointer)ip3);
/*
	STOP TRAPEZOIDALE
*/
	i = 0;
	XtSetArg (args[i], XmNy, font_height * 12+tot_inc);
	i++;
	XtSetArg (args[i], XmNx, 10 + 2 * tot_width / 3 + font_widthn * 15);
	i++;
	XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
	i++;
	XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Stop", XmSTRING_DEFAULT_CHARSET));
	i++;
	XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
	i++;
	aing[ip3].sw[k_trap_stop] = XmCreatePushButton (wRow, "button", args, i);
	XtAddCallback (aing[ip3].sw[k_trap_stop], XmNactivateCallback,
		trapezoidal_stop_callback, (XtPointer)ip3);
    if(modifica == 1)
	XtAddCallback (aing[ip3].sw[k_trap_stop], XmNactivateCallback,
		pag_del_callback, (XtPointer)ip3);





    if (aing[ip3].nome_var[0] != 'J')
    {
/*
	START RAMPA 
*/
	i = 0;
	XtSetArg (args[i], XmNy, font_height * 11+tot_inc);
	i++;
	XtSetArg (args[i], XmNx, 10 + 2 * tot_width / 3 + font_widthn * 3);
	i++;
	XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
	i++;
	XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Start", XmSTRING_DEFAULT_CHARSET));
	i++;
	XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
	i++;
	aing[ip3].sw[k_but_start] = XmCreatePushButton (wRow, "button", args, i);
	XtAddCallback (aing[ip3].sw[k_but_start], XmNactivateCallback, start_callback, (XtPointer)ip3);
    if(modifica == 1)
	XtAddCallback (aing[ip3].sw[k_but_start], XmNactivateCallback, pag_del_callback, (XtPointer)ip3);

	XtManageChild (aing[ip3].sw[k_but_start]);

/*
	STOP RAMPA 
*/
	i = 0;
	XtSetArg (args[i], XmNy, font_height * 11+tot_inc);
	i++;
	XtSetArg (args[i], XmNx, 10 + 2 * tot_width / 3 + font_widthn * 15);
	i++;
	XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
	i++;
	XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Stop", XmSTRING_DEFAULT_CHARSET));
	i++;
	XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
	i++;
	aing[ip3].sw[k_but_stop] = XmCreatePushButton (wRow, "button", args, i);
	XtAddCallback (aing[ip3].sw[k_but_stop], XmNactivateCallback,
		stop_callback, (XtPointer)ip3);
    if(modifica == 1)
	XtAddCallback (aing[ip3].sw[k_but_stop], XmNactivateCallback,
		pag_del_callback, (XtPointer)ip3);

	XtManageChild (aing[ip3].sw[k_but_stop]);
    }

/*
	START RAMPA 
*/
    i = 0;
    XtSetArg (args[i], XmNy, font_height * 11+tot_inc);
    i++;
    XtSetArg (args[i], XmNx, font_widthn * 2);
    i++;
    XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
    i++;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Start", XmSTRING_DEFAULT_CHARSET));
    i++;
    XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
    i++;
    aing[ip3].sw[k_but_invia] = XmCreatePushButton (wRow, "button", args, i);
    XtAddCallback (aing[ip3].sw[k_but_invia], XmNactivateCallback,
	    invia_callback, (XtPointer)ip3);
    if(modifica == 1)
    XtAddCallback (aing[ip3].sw[k_but_invia], XmNactivateCallback,
	    pag_del_callback, (XtPointer)ip3);
    XtManageChild (aing[ip3].sw[k_but_invia]);

/*
	NOT
*/
    if (aing[ip3].nome_var[0] == 'J')
    {
/* il pulsante NOT viene creato solo nel caso di variabilie digitale */
	i = 0;
	XtSetArg (args[i], XmNy, font_height * 11+tot_inc);
	i++;
	XtSetArg (args[i], XmNx, font_widthn * 10);
	i++;
	XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
	i++;
	XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR (" Not ", XmSTRING_DEFAULT_CHARSET));
	i++;
	XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
	i++;
	aing[ip3].sw[k_not] = XmCreatePushButton (wRow, "button", args, i);
	XtAddCallback (aing[ip3].sw[k_not], XmNactivateCallback,
		not_callback, (XtPointer)ip3);
    if(modifica == 1)
	XtAddCallback (aing[ip3].sw[k_not], XmNactivateCallback,
		pag_del_callback, (XtPointer)ip3);
	XtManageChild (aing[ip3].sw[k_not]);
    }

/*
	UP-DOWN
*/
    if (aing[ip3].nome_var[0] == 'J')
    {
/* il pulsante UP-DOWN viene creato solo nel caso di variabilie digitale */
	i = 0;
	XtSetArg (args[i], XmNy, font_height * 11+tot_inc);
	i++;
	XtSetArg (args[i], XmNx, font_widthn * 18);
	i++;
	XtSetArg (args[i], XmNfontList, font_list[disp_sel]);
	i++;
	XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Up-down", XmSTRING_DEFAULT_CHARSET));
	i++;
	XtSetArg (args[i], XmNbackground, excolor[disp_sel][1].pixel);
	i++;
	aing[ip3].sw[k_updown] = XmCreatePushButton (wRow, "button", args, i);
	XtAddCallback (aing[ip3].sw[k_updown], XmNarmCallback,
		up_callback, (XtPointer)ip3);
    if(modifica == 1)
	XtAddCallback (aing[ip3].sw[k_updown], XmNarmCallback,
		pag_del_callback, (XtPointer)ip3);
	XtAddCallback (aing[ip3].sw[k_updown], XmNdisarmCallback,
		down_callback, (XtPointer)ip3);
	XtManageChild (aing[ip3].sw[k_updown]);
    }

/* aggiorna lo stato dei bottoni */
    agg_stato (ip3);

    XtManageChild (aing[ip3].w);
    i = 0;
    XtSetArg (args[i], XmNwhichButton, Button3);
    i++;
    wpopup = XmCreatePopupMenu (aing[ip3].w, "pulldown", args, i);
    i = 0;
    XtSetArg (args[i], XmNlabelString, XmStringCreateLtoR ("Quit ", XmSTRING_DEFAULT_CHARSET));
    i++;
    wbutton = XmCreatePushButton (wpopup, " bottone", args, i);
    XtManageChild (wbutton);
    aing[ip3].sw[k_quit]=wbutton;
    XtAddCallback (wbutton, XmNactivateCallback, pag_del_callback, (XtPointer)ip3);
    XtAddEventHandler (aing[ip3].w, ButtonPressMask, False, PostIt, wpopup);
    if (add_refresh ((void*) aing_refresh, (void *)ip3) == -1)
	printf ("\n errore : refresh non aggiunto");


    XtManageChild(aing[ip3].wtopLevel);
    return (0);			/* ritorno corretto */
}   /* FINE DELLA ROUTINE new_aing */






/* ****************
 *                *
 * CALLBACK       *
 * OPTION MENU    *
 *                *
 * **************** */
/* ****************
 *    GRADINO     *
 * **************** */
static  void invia_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo;
float   valore;
float   tempo;
char   *pstring;
int     is = (int) info;

/* 
 estrae il valore dello step dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_val_step]);
    iret = sscanf (pstring, "%f", &valore);

/* 
 estrae il valore del tempo dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);

/*
 in caso di valore inserito non valido annulla l'operazione
 */
    if (valore < (-MAX_VALORE) || valore > MAX_VALORE || iret != 1)
    {
	XtFree (pstring);
	return;
    }
/*
 il valore inserito e' accettabile
 */
    valore = (valore - aing[is].b) / aing[is].a;
    tipo = 1;


    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;


    iret = pert_scalino (id_msg_pert, aing[is].select, valore, tempo);

}



/* ****************
 *    UP-DOWN     *
 * **************** */
static  void not_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo;
float   tempo;
float   valore;
char   *pstring;
int     is = (int) info;

    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);

    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;

    iret = pert_nega (id_msg_pert, aing[is].select, tempo);
}

static  void up_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo;
float   valore;
float   tempo;
char   *pstring;
int     is = (int) info;

/*
 estrae il valore del tempo dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);



    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;

    iret = pert_up (id_msg_pert, aing[is].select, tempo);
}

static  void down_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo;
float   valore;
float   tempo;
char   *pstring;
int     is = (int) info;

/*
 estrae il valore del tempo dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);

    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;

    iret = pert_down (id_msg_pert, aing[is].select, tempo);

}



/* ****************
 *    RAMPE       *
 * **************** */
/*
    Start della rampa: PERT_RAMPA oppure PERT_TO
*/
static  void start_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo, isegn;
float   valore, valfin, rateo;
float   tempo;
char   *pstring;
int     is = (int) info;

/* 
 estrae il valore del gradiente dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_val_grad]);
    iret = sscanf (pstring, "%f", &valore);

/*
 estrae il valore del tempo dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);

/*
 in caso di valore inserito non valido annulla l'operazione
 */
    if (valore < -MAX_VALORE || valore > MAX_VALORE || iret != 1)
    {
	XtFree (pstring);
	return;
    }
/*
 il valore inserito e' accettabile
 */
    aing[is].rateo = valore;

/* 
 estrae il valore finale rampa dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_val_finale]);
    iret = sscanf (pstring, "%f", &valore);
/*
 in caso di valore inserito non valido annulla l'operazione
 */
    if (valore < -MAX_VALORE || valore > MAX_VALORE || iret != 1)
    {
	XtFree (pstring);
	return;
    }
/*
 il valore inserito e' accettabile
 */
    aing[is].valfin = valore;
    aing[is].ists = 1;
    aing[is].flag_rampa = 1;
    aing[is].isegn = (((aing[is].valfin - aing[is].b) / aing[is].a) >
	    estr_sh (aing[is].select)) ? 1 : -1;
    tipo = 2;
    valfin = (aing[is].valfin - aing[is].b) / aing[is].a;

    rateo = (float) fabs ((double) (aing[is].rateo));

    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;


    if (aing[is].grad_dt_time == 0)
    {
	rateo = ((aing[is].rateo - aing[is].b) / aing[is].a)/60.0;
	iret = pert_rampa (id_msg_pert, aing[is].select, valfin, rateo, tempo);
    }

    if (aing[is].grad_dt_time == 1)
    {
	rateo = -rateo;
	printf ("aing[is].grad_dt_time=%d rateo=%f\n", aing[is].grad_dt_time, rateo);
	iret = pert_to (id_msg_pert, aing[is].select, valfin, rateo, tempo);
    }

    if (aing[is].grad_dt_time == 2)
    {
	printf ("aing[is].grad_dt_time=%d rateo=%f\n", aing[is].grad_dt_time, rateo);
	iret = pert_to (id_msg_pert, aing[is].select, valfin, rateo, tempo);
    }

    /* aggiorna la casella del delay */
    if((aing[is].delta_time==2)&& (aing[is].grad_dt_time == 1))
	{
	sprintf(pstring,"%5.2f",tempo-rateo);
	XmTextSetString(aing[is].sw[k_delay],pstring);
	}
    if((aing[is].delta_time==2)&& (aing[is].grad_dt_time == 2))
	{
	sprintf(pstring,"%5.2f",rateo);
	XmTextSetString(aing[is].sw[k_delay],pstring);
	}
}

/*
    Stop della rampa: PERT_RAMPA_STOP oppure PERT_TO_STOP
*/
static  void stop_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo, isegn;
float   valore, valfin, rateo;
float   tempo;
char   *pstring;
int     is = (int) info;

    isegn = 0;
    tipo = 2;
    valfin = (aing[is].valfin - aing[is].b) / aing[is].a;
    rateo = ((aing[is].rateo - aing[is].b) / aing[is].a)/60.0;


/*
 estrae il valore del tempo dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);

    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;

    if (aing[is].grad_dt_time == 0)
	{
	iret = pert_rampa_stop (id_msg_pert, aing[is].select, tempo);
	}
    else
	{
	iret = pert_to_stop (id_msg_pert, aing[is].select, tempo);
	}

    if (iret == 0)
	printf ("errore stop rampa");
}



/* ****************
 *    IMPULSO    *
 * **************** */
/*
    Start impulso
*/
static  void impulso_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo, isegn;
float   tempo;
float   valore, valfin, tempo_imp;
char   *pstring;
int     is = (int) info;


/*
 estrae il valore finale dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_val_imp]);
    iret = sscanf (pstring, "%f", &valore);

/*
 in caso di valore inserito non valido annulla l'operazione
 */
    if (valore < -MAX_VALORE || valore > MAX_VALORE || iret != 1)
    {
	XtFree (pstring);
	return;
    }
/*
 il valore inserito e' accettabile
 */
    aing[is].val_imp = valore;

/*
 estrae il valore della durata dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_tempo_imp]);
    iret = sscanf (pstring, "%f", &valore);

/*
 estrae il valore del tempo dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);

/*
 in caso di valore inserito non valido annulla l'operazione
 */
    if (valore < -MAX_VALORE || valore > MAX_VALORE || iret != 1)
    {
	XtFree (pstring);
	return;
    }
/*
 il valore inserito e' accettabile
 */
    aing[is].tempo_imp = valore;
/*
 se il delta dell'impulso e' nullo esce
*/
    if (aing[is].val_imp == aing[is].valore)
	return;

    aing[is].ists = 1;
    aing[is].flag_impulso = 1;
    aing[is].isegn = (aing[is].valfin > aing[is].valore) ? 1 : -1;
    tipo = 3;
    valfin = (aing[is].val_imp - aing[is].b) / aing[is].a;
    tempo_imp = aing[is].tempo_imp;





    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;


    iret = pert_impulso (id_msg_pert, aing[is].select, valfin, tempo_imp, tempo);

    /* aggiorna la casella del delay */
    if(aing[is].delta_time==2)
	{
	sprintf(pstring,"%5.2f",tempo+tempo_imp);
	XmTextSetString(aing[is].sw[k_delay],pstring);
	}

    agg_stato (is);
}

/*
    Stop impulso
*/
static  void impulsostop_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo, isegn;
float   valore, valfin, rateo;
float   tempo;
char   *pstring;
int     is = (int) info;

    aing[is].ists = 0;


/*
 estrae il valore del tempo dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);

    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;

    iret = pert_impulso_stop (id_msg_pert, aing[is].select, tempo);

    if (iret == 0)
	printf ("errore stop impulso");

    agg_stato (is);
}



/* ****************
 *   MALFUNCTION  *
 * **************** */
/*
        Avvia la malfunzione inserendo uno scalino
        sulla variabile chiamando pert_malfunction.
*/
static  void malfunction_start_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo, isegn;
float   tempo;
float   valore, valfin, tempo_imp;
char   *pstring;
int     is = (int) info;
float   targetMF,typeMF;
Widget attenzione=NULL;
Dialog_geometry geom;
char testo[100];


/*
     Estrae il valore del tempo dal widget di testo : 
     il ritardo e poi il target.
     Se valore inserito non valido annulla l'operazione.
*/
     pstring = XmTextGetString (aing[is].sw[k_delay]);
     iret = sscanf (pstring, "%f", &tempo);


     pstring = XmTextGetString (aing[is].sw[k_text_targetmalf]);
     iret = sscanf (pstring, "%f", &targetMF);

     if (targetMF < -MAX_VALORE || targetMF > MAX_VALORE || iret != 1)
        {
	XtFree (pstring);
	return;
        }

/*
     Estrae il valore di type dal widget di testo.
     Se valore inserito non valido annulla l'operazione.
*/
     pstring = XmTextGetString (aing[is].sw[k_text_typemalf]);
     iret = sscanf (pstring, "%f", &typeMF);

     if (typeMF < -MAX_VALORE || typeMF > MAX_VALORE || iret != 1)
         {
	 XtFree (pstring);
	 return;
         }

/*
     Per evitare la nuova spedizione devo pero' controllare
     anche il type che quindi devo averlo nella struttura
     aing.
     if ( aing[is].valore == targetMF)
        {
        printf("\t\tQuesta non la spedirei.... \n");
        }
*/

     targetMF = ( targetMF - aing[is].b) / aing[is].a;
     tipo = 1;

     tempo = (float) fabs ((double) tempo);
     if (aing[is].delta_time == 0)
	tempo = -tempo;


     iret = pert_malfunction (id_msg_pert, aing[is].select,
                             targetMF,typeMF,tempo);

} /* fine malfunction_start_callback */

/*
    Termina la malfunzione inviando uno scalino a zero.
*/
static  void malfunction_stop_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo;
float   tempo;
float   zero=0.0;
int     is = (int) info;
char    *pstring;




/*
     Estrae il valore del ritardo 
*/
     pstring = XmTextGetString (aing[is].sw[k_delay]);
     iret = sscanf (pstring, "%f", &tempo);

     tempo = (float) fabs ((double) tempo);
     if (aing[is].delta_time == 0)
	tempo = -tempo;

     XmTextSetString(aing[is].sw[k_text_targetmalf],"0.00");

     iret = pert_malfunction_stop (id_msg_pert, aing[is].select,
                                   tempo);


} /* fine malfunction_stop_callback */



/* ****************
 *   WHITE NOISE  *
 * **************** */
/*
    Invia pert_whitenoise
*/
static  void whitenoise_start_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo, isegn;
float   tempo;
float   valore, valfin, tempo_imp;
char   *pstring;
int     is = (int) info;
float meanvalue,variance;
Widget attenzione=NULL;
Dialog_geometry geom;
char testo[100];



/*
 estrae il valore del tempo dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);


pstring = XmTextGetString (aing[is].sw[k_text_meanvalue]);
iret = sscanf (pstring, "%f", &meanvalue);
/*
 in caso di valore inserito non valido annulla l'operazione
 */
if (valore < -MAX_VALORE || valore > MAX_VALORE || iret != 1)
    {
	XtFree (pstring);
	return;
    }

pstring = XmTextGetString (aing[is].sw[k_text_variance]);
iret = sscanf (pstring, "%f", &variance);
/*
 in caso di valore inserito non valido annulla l'operazione
 */
if (valore < -MAX_VALORE || valore > MAX_VALORE || iret != 1)
    {
	XtFree (pstring);
	return;
    }


    aing[is].ists = 1;
    aing[is].flag_impulso = 1;
    aing[is].isegn = (aing[is].valfin > aing[is].valore) ? 1 : -1;
    tipo = 3;



    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;


    /*
	test correttezza parametri
    */
    if(variance <=0)
	{
	geom.default_pos=1;
        geom.height=200;
	sprintf(testo,"Warning: Variance > 0");
        attenzione=attention(w,testo,CREA|MAPPA,geom);
	return;
	}

    iret = pert_whitenoise (id_msg_pert, aing[is].select, 
		meanvalue,variance,tempo);

    /* aggiorna la casella del delay */
    if(aing[is].delta_time==2)
	{
	sprintf(pstring,"%5.2f",tempo+tempo_imp);
	XmTextSetString(aing[is].sw[k_delay],pstring);
	}

    agg_stato (is);
}

/*
   Termina white noise inviano pert_whitenoise_stop
*/
static  void whitenoise_stop_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo, isegn;
float   valore, valfin, rateo;
float   tempo;
char   *pstring;
int     is = (int) info;

    aing[is].ists = 0;


/*
 estrae il valore del tempo dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);

    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;

    iret = pert_whitenoise_stop (id_msg_pert, aing[is].select, tempo);

    if (iret == 0)
	printf ("errore stop whitenois");

    agg_stato (is);
}



/* ****************
 *   SINUSOIDAL   *
 * **************** */
/*
    Invia pert_sinusoidal
*/
static  void sinusoidal_start_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo, isegn;
float   tempo;
float   valore, valfin, tempo_imp;
char   *pstring;
int     is = (int) info;
float period,phase,meanvalue,wide,t1,t2;
Widget attenzione=NULL;
Dialog_geometry geom;
char testo[100];



/*
 estrae il valore del tempo dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);

pstring = XmTextGetString (aing[is].sw[k_text_period]);
iret = sscanf (pstring, "%f", &period);

pstring = XmTextGetString (aing[is].sw[k_text_phase]);
iret = sscanf (pstring, "%f", &phase);

pstring = XmTextGetString (aing[is].sw[k_text_meanvalue]);
iret = sscanf (pstring, "%f", &meanvalue);

pstring = XmTextGetString (aing[is].sw[k_text_wide]);
iret = sscanf (pstring, "%f", &wide);

pstring = XmTextGetString (aing[is].sw[k_text_t1]);
iret = sscanf (pstring, "%f", &t1);

pstring = XmTextGetString (aing[is].sw[k_text_t2]);
iret = sscanf (pstring, "%f", &t2);


    aing[is].ists = 1;
    aing[is].flag_impulso = 1;
    aing[is].isegn = (aing[is].valfin > aing[is].valore) ? 1 : -1;
    tipo = 3;



    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;


    /*
	test correttezza parametri
    */
    if(wide <=0)
	{
	geom.default_pos=1;
        geom.height=200;
	sprintf(testo,"Warning: Wide > 0");
        attenzione=attention(w,testo,CREA|MAPPA,geom);
	return;
	}
    if(period <=0)
	{
	geom.default_pos=1;
        geom.height=200;
	sprintf(testo,"Warning: Period > 0");
        attenzione=attention(w,testo,CREA|MAPPA,geom);
	return;
	}
    if(phase <0)
	{
	geom.default_pos=1;
        geom.height=200;
	sprintf(pstring,"%5.2f",tempo+tempo_imp);
	XmTextSetString(aing[is].sw[k_delay],pstring);
	}

 iret = pert_period (id_msg_pert, aing[is].select, -1., -1.,
                period,phase,meanvalue,wide,0.,tempo);

    /* aggiorna la casella del delay */
    if(aing[is].delta_time==2)
        {
        sprintf(pstring,"%5.2f",tempo+tempo_imp);
        XmTextSetString(aing[is].sw[k_delay],pstring);
        }

    agg_stato (is);
}

/*
    Invia pert_sinusoidal_stop
*/
static  void sinusoidal_stop_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo, isegn;
float   valore, valfin, rateo;
float   tempo;
char   *pstring;
int     is = (int) info;

    aing[is].ists = 0;


/*
 estrae il valore del tempo dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);

    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;

    iret = pert_period_stop (id_msg_pert, aing[is].select, tempo);

    if (iret == 0)
	printf ("errore stop periodic");

    agg_stato (is);
}



/* ****************
 *   TRAPEZOIDAL  *
 * **************** */
/*
    Invia pert_trapezoidal
*/
static  void trapezoidal_start_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo, isegn;
float   tempo;
float   valore, valfin, tempo_imp;
char   *pstring;
int     is = (int) info;
float period,phase,meanvalue,wide,t1,t2,tempo_null;
Widget attenzione=NULL;
Dialog_geometry geom;
char testo[100];


/*
 estrae il valore del tempo dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);


pstring = XmTextGetString (aing[is].sw[k_text_period]);
iret = sscanf (pstring, "%f", &period);

pstring = XmTextGetString (aing[is].sw[k_text_phase]);
iret = sscanf (pstring, "%f", &phase);

pstring = XmTextGetString (aing[is].sw[k_text_meanvalue]);
iret = sscanf (pstring, "%f", &meanvalue);

pstring = XmTextGetString (aing[is].sw[k_text_wide]);
iret = sscanf (pstring, "%f", &wide);

pstring = XmTextGetString (aing[is].sw[k_text_t1]);
iret = sscanf (pstring, "%f", &t1);

pstring = XmTextGetString (aing[is].sw[k_text_t2]);
iret = sscanf (pstring, "%f", &t2);

pstring = XmTextGetString (aing[is].sw[k_text_t3]);
iret = sscanf (pstring, "%f", &tempo_null);


    aing[is].ists = 1;
    aing[is].flag_impulso = 1;
    aing[is].isegn = (aing[is].valfin > aing[is].valore) ? 1 : -1;
    tipo = 3;



    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;


    /*
	test correttezza parametri
    */
    if(wide <=0)
	{
	geom.default_pos=1;
        geom.height=200;
	sprintf(testo,"Warning: Wide > 0");
        attenzione=attention(w,testo,CREA|MAPPA,geom);
	return;
	}
    if(period <=0)
	{
	geom.default_pos=1;
        geom.height=200;
	sprintf(testo,"Warning: Period > 0");
        attenzione=attention(w,testo,CREA|MAPPA,geom);
	return;
	}
    if(phase <0)
	{
	geom.default_pos=1;
        geom.height=200;
	sprintf(testo,"Warning: Phase >= 0");
        attenzione=attention(w,testo,CREA|MAPPA,geom);
	return;
	}
    if(t1 <0)
	{
	geom.default_pos=1;
        geom.height=200;
	sprintf(testo,"Warning: T1 >= 0");
        attenzione=attention(w,testo,CREA|MAPPA,geom);
	return;
	}
    if(t2 <0)
	{
	geom.default_pos=1;
        geom.height=200;
	sprintf(testo,"Warning: T2 >= 0");
        attenzione=attention(w,testo,CREA|MAPPA,geom);
	return;
	}
    if(tempo_null <0)
	{
	geom.default_pos=1;
        geom.height=200;
	sprintf(testo,"Warning: T3 >= 0");
        attenzione=attention(w,testo,CREA|MAPPA,geom);
	return;
	}
    if((tempo_null+t1+t2) > period)
	{
	geom.default_pos=1;
        geom.height=200;
	sprintf(testo,"Warning: T1+T2+T3 <= Period");
        attenzione=attention(w,testo,CREA|MAPPA,geom);
	return;
	}

    iret = pert_period (id_msg_pert, aing[is].select, t1, t2, 
		period,phase,meanvalue,wide,tempo_null,tempo);

    /* aggiorna la casella del delay */
    if(aing[is].delta_time==2)
	{
	sprintf(pstring,"%5.2f",tempo+tempo_imp);
	XmTextSetString(aing[is].sw[k_delay],pstring);
	}

    agg_stato (is);
}

/*
    Invia pert_trapezoidal_stop
*/
static  void trapezoidal_stop_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     i, iret, tipo, isegn;
float   valore, valfin, rateo;
float   tempo;
char   *pstring;
int     is = (int) info;

    aing[is].ists = 0;


/*
 estrae il valore del tempo dal widget di testo
*/
    pstring = XmTextGetString (aing[is].sw[k_delay]);
    iret = sscanf (pstring, "%f", &tempo);

    tempo = (float) fabs ((double) tempo);
    if (aing[is].delta_time == 0)
	tempo = -tempo;

    iret = pert_period_stop (id_msg_pert, aing[is].select, tempo);

    if (iret == 0)
	printf ("errore stop periodic");

    agg_stato (is);
}






/* ****************
 *                *
 * CALLBACK       *
 * OPTION MENU    *
 *                *
 * **************** */

/* ****************
 * 1) TIPO RAMPA
 * **************** */
/*
    Rampa con scelta opzione "Grad:" (PERT_RAMPA)
*/
static  void rampa_grad_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     is = (int) info;

    aing[is].grad_dt_time = 0;
}

/*
    Rampa con scelta opzione "Delta:" (PERT_TO)
*/
static  void rampa_delta_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     is = (int) info;

    aing[is].grad_dt_time = 1;
}

/*
    Rampa con scelta opzione "Time:" (PERT_TO)
*/
static  void rampa_time_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     is = (int) info;

    aing[is].grad_dt_time = 2;
}



/* ****************
 * 2) TIPO RITARDO (3)
 * **************** */
/*
    Per scelta ritardo di tipo "Start at:"
*/
static  void time_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     is = (int) info;

    XmTextSetEditable (aing[is].sw[k_delay], True);
    aing[is].delta_time = 1;
}

/*
    Per scelta di ritardo di tipo "Time:"
*/
static  void tinc_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     is = (int) info;

    XmTextSetEditable (aing[is].sw[k_delay], False);
    aing[is].delta_time = 2;
}

/*
    Per scelta di ritardo di tipo "Delay:"
*/
static  void delta_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     is = (int) info;

    XmTextSetEditable (aing[is].sw[k_delay], True);
    aing[is].delta_time = 0;
}



/* ****************
 * 3) ALTRE PERT (4)
 * **************** */
/*
   Per scelta di perturbazioni NORMAL 
*/
static  void tipo_normal_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     is = (int) info;
int k;

    aing[is].tipo_pert = TIPO_PERT_NORMAL;

    for(k=0;k<NUMERO_VOCI;k++)
	if(aing[is].sw[voce_normal[k].ind]!=NULL)
		if(voce_normal[k].stato)
    			XtManageChild (aing[is].sw[voce_normal[k].ind]);
		else
    			XtUnmanageChild (aing[is].sw[voce_normal[k].ind]);
}

/*
   Per scelta di perturbazioni PERIODIC 
*/
static  void tipo_periodic_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     is = (int) info;
int k;

    aing[is].tipo_pert = TIPO_PERT_PERIODIC;

    for(k=0;k<NUMERO_VOCI;k++)
	if(aing[is].sw[voce_periodic[k].ind]!=NULL)
		if(voce_periodic[k].stato)
    			XtManageChild (aing[is].sw[voce_periodic[k].ind]);
		else
    			XtUnmanageChild (aing[is].sw[voce_periodic[k].ind]);
		
}

/*
   Per scelta di perturbazione WHITE NOISE
*/
static  void tipo_noise_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     is = (int) info;
int k;

    aing[is].tipo_pert = TIPO_PERT_WHITENOISE;

    for(k=0;k<NUMERO_VOCI;k++)
	if(aing[is].sw[voce_noise[k].ind]!=NULL)
		if(voce_noise[k].stato)
    			XtManageChild (aing[is].sw[voce_noise[k].ind]);
		else
    			XtUnmanageChild (aing[is].sw[voce_noise[k].ind]);
		
}

/*
   Per scelta di perturbazione MALFUNCTION
*/
static  void tipo_malf_callback (w, info, str)
        Widget w;
void * info;
XmAnyCallbackStruct * str;
{
int     is = (int) info;
int k;


    aing[is].tipo_pert = TIPO_PERT_MALFUNCTION;

    for(k=0;k<NUMERO_VOCI;k++)
	if(aing[is].sw[voce_malf[k].ind]!=NULL)
		if(voce_malf[k].stato)
    			XtManageChild (aing[is].sw[voce_malf[k].ind]);
		else
    			XtUnmanageChild (aing[is].sw[voce_malf[k].ind]);
}






static  void aing_refresh (info)
        void * info;
{
int     is;
char    appstr[20];
float   valore;
    XmString appCstr;
    is = (int) info;
/*
 raccoglie il valore della variabile dall'area di memoria shared
 */
    valore = estr_sh (aing[is].select);
    if (valore != aing[is].valore)/* se il valore e' diverso da quello estratto l'iterazione precedente */
    {
/*
 setta il display del valore
*/
	aing[is].valore = valore;
	sprintf (appstr, "%10.2f", valore * aing[is].a + aing[is].b);
	appCstr = XmStringCreateLtoR (appstr, XmSTRING_DEFAULT_CHARSET);
	XtSetArg (args[0], XmNlabelString, appCstr);
	XtSetValues (aing[is].sw[k_val_corr], args, 1);
	XmStringFree (appCstr);
    }
/*
 se e' in start una rampa vede se e' terminata
*/
    if (aing[is].flag_rampa)
    {
	valore = valore * aing[is].a + aing[is].b;
	if ((aing[is].isegn == 1 && valore >= aing[is].valfin) ||
		(aing[is].isegn == -1 && valore <= aing[is].valfin))
	{
	    aing[is].ists = 0;
	    aing[is].flag_rampa = 0;
	    agg_stato (is);
	}
    }
/*
 se e' in corso un impulso verifica che l'impuso sia stato
 preso in considerazione, se si' pone a 2 il flag
*/

    if ((aing[is].flag_impulso == 1) && (valore == aing[is].val_imp))
    {
	aing[is].flag_impulso = 2;
	agg_stato (is);
    }

/* se e' in corso un impulso vede se e' terminato */

    if ((aing[is].flag_impulso == 2) && (valore != aing[is].val_imp))
    {
	aing[is].ists = 0;
	aing[is].flag_impulso = 0;
	agg_stato (is);
    }

}

/*
 agg_stato
    aggiorna la visualizzazione dei bottoni e dei campi di
    editing relativi alla rampa in base allo stato di START
    o STOP della rampa stessa
*/
void agg_stato (is)
int     is;
{
    if (0)
    {
/*
 esamina lo stato della rampa e visualizza come attivato
 il bottone corrispondente allo stato corrente
*/
	if (aing[is].ists == 1)
	{
/*
 la rampa o l'impulso sono in stato di start
*/
	    if (aing[is].flag_rampa == 1)
	    {
		/* colora il bottone di start */
		XtSetArg (args[0], XmNbackground, excolor[disp_sel][2].pixel);
		XtSetValues (aing[is].sw[k_but_start], args, 1);
		XtSetArg (args[0], XmNbackground, excolor[disp_sel][1].pixel);
		XtSetValues (aing[is].sw[k_but_stop], args, 1);
		/* colora come disabilitato il bottone di pulse updown e nega */
		XtSetArg (args[0], XmNforeground, excolor[disp_sel][3].pixel);
		XtSetValues (aing[is].sw[k_but_imp], args, 1);
		XtSetArg (args[0], XmNforeground, excolor[disp_sel][3].pixel);
		XtSetValues (aing[is].sw[k_not], args, 1);
		XtSetArg (args[0], XmNforeground, excolor[disp_sel][3].pixel);
		XtSetValues (aing[is].sw[k_updown], args, 1);
		/* rende non editabili i campi relativi alla rampa */
		XmTextSetEditable (aing[is].sw[k_val_finale], False);
		XmTextSetEditable (aing[is].sw[k_val_grad], False);
	    }
	    else
	    {
		/* colora come disabilitati i pulsanti di start, stop pulse not e updown */
		XtSetArg (args[0], XmNforeground, excolor[disp_sel][3].pixel);
		XtSetValues (aing[is].sw[k_but_stop], args, 1);
		XtSetValues (aing[is].sw[k_but_start], args, 1);
		XtSetValues (aing[is].sw[k_but_imp], args, 1);
		XtSetValues (aing[is].sw[k_not], args, 1);
		XtSetValues (aing[is].sw[k_updown], args, 1);
		/* rende non editabili il campo relativo all' impulso */
		XmTextSetEditable (aing[is].sw[k_val_imp], False);
	    }
/* colora come disabilitato il bottone di invia */
	    XtSetArg (args[0], XmNforeground, excolor[disp_sel][3].pixel);
	    XtSetValues (aing[is].sw[k_but_invia], args, 1);
	}
	else
	{
/*
 rampa o impulso in stato di stop
*/
/* colora il bottone di stop e start*/
	    XtSetArg (args[0], XmNbackground, excolor[disp_sel][1].pixel);
	    XtSetValues (aing[is].sw[k_but_start], args, 1);
	    XtSetArg (args[0], XmNbackground, excolor[disp_sel][2].pixel);
	    XtSetValues (aing[is].sw[k_but_stop], args, 1);
	    XtSetArg (args[0], XmNforeground, excolor[disp_sel][0].pixel);
	    XtSetValues (aing[is].sw[k_but_start], args, 1);
	    XtSetArg (args[0], XmNforeground, excolor[disp_sel][0].pixel);
	    XtSetValues (aing[is].sw[k_but_stop], args, 1);
/* colora come abilitato il bottone di invia */
	    XtSetArg (args[0], XmNforeground, excolor[disp_sel][0].pixel);
	    XtSetValues (aing[is].sw[k_but_invia], args, 1);
/* colora come abilitato il bottone di pulse */
	    XtSetArg (args[0], XmNforeground, excolor[disp_sel][0].pixel);
	    XtSetValues (aing[is].sw[k_but_imp], args, 1);
/* colora come abilitato il bottone di not */
	    XtSetValues (aing[is].sw[k_not], args, 1);
/* colora come abilitato il bottone di updown */
	    XtSetValues (aing[is].sw[k_updown], args, 1);
/* rende editabili i campi relativi alla rampa e all'impulso*/
	    XmTextSetEditable (aing[is].sw[k_val_finale], True);
	    XmTextSetEditable (aing[is].sw[k_val_grad], True);
	    XmTextSetEditable (aing[is].sw[k_val_imp], True);
	}

    }
}


static  void PostIt (w, num, event)
        Widget w;
int    *num;
XButtonEvent * event;
{
    Widget wpop = (Widget) num;
    if (event -> button == Button3)
    {
	XmMenuPosition (wpop, event);
	/*  Visualizza il popup menu */
	XtManageChild (wpop);
    }
}





void init_aing ()
{
int     i;
    for (i = 0; i < MAX_AING; i++)
    {
	t_call[i].closure = (void *) (-1);
    }
}

/* 
 add_refresh 
    permette di associare al timeout una nuova funzione di refresh;
    verra' generalmente associata una routine di refresh per ogni
    stazione creata
 */
int add_refresh (proc, closure)
XtCallbackProc proc;
caddr_t closure;
//void * closure;
{
int     i;
    for (i = 0; i < MAX_AING; i++)
    {
	if (t_call[i].closure == (void *) (-1))
	{
// GUAG2025
//	    t_call[i].callback = (XtCallbackProc)proc;
        t_call[i].callback = (void (*)(void *)) proc;
	    t_call[i].closure = closure;
	    return (0);
	}
    }
    return (-1);		/* mancanza di memoria per aggiunta routine di refresh */
}


/*
  elimina la funzione di refresh associata ad una stazione
 */

int del_refresh (closure)
void * closure;
{
int     i;
    for (i = 0; i < MAX_AING; i++)
    {
	if (t_call[i].closure == closure)
	{
	    t_call[i].callback = NULL;
	    t_call[i].closure = (void *) (-1);
	    return (0);
	}
    }
    return (-1);		/* routine di callback non trovata */
}


void init_gc (int disp)
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

    load_font (&(font_info[disp]),disp);
    font_list[disp] = XmFontListCreate (font_info[disp], XmSTRING_DEFAULT_CHARSET);
    font_ascent = font_info[disp] -> ascent;
    font_height = font_info[disp] -> ascent + font_info[disp] -> descent;
    font_width = (int) (font_info[disp] -> max_bounds.width);
    values.function = GXcopy;
    values.line_width = line_width;
    values.cap_style = cap_style;
    values.join_style = join_style;
    values.background = WhitePixel (XtDisplay(elenco_display[disp].topLevel), 
		DefaultScreen(XtDisplay(elenco_display[disp].topLevel)));
    default_cmap = DefaultColormap (XtDisplay(elenco_display[disp].topLevel), 
		DefaultScreen(XtDisplay(elenco_display[disp].topLevel)));
    for (i = 0; i < 5; i++)
    {
	XAllocNamedColor (XtDisplay(elenco_display[disp].topLevel),
		default_cmap,
		colori[i], &color, &(excolor[disp][i]));
    }

    for (i = 0; i < 5; i++)
    {
	values.line_style = line_style[i];
	values.foreground = excolor[disp][i].pixel;
	values.background = WhitePixel (XtDisplay(elenco_display[disp].topLevel), 
		DefaultScreen(XtDisplay(elenco_display[disp].topLevel)));

	gc[i][disp] = XCreateGC (XtDisplay(elenco_display[disp].topLevel), 
	  RootWindow (XtDisplay(elenco_display[disp].topLevel), DefaultScreen(XtDisplay(elenco_display[disp].topLevel))), valuemask,
		&values);
	XSetFont (XtDisplay(elenco_display[disp].topLevel), 
		gc[i][disp], font_info[disp] -> fid);
    }
}


void load_font (font_info,disp)
XFontStruct ** font_info;
int disp;
{
char   *font_name = "fixed";
/* Carica il font ottenendo la descrizione del font stesso */
    if ((*font_info = XLoadQueryFont (XtDisplay(elenco_display[disp].topLevel), 
		font_name)) == NULL)

    {
	printf ("\nCannot open font");
	exit (0);
    }

}

/*
 * estr_sh
 *    funzione utilizzata per estrarre un valore dall'area
 *    shared di memoria; e' necessaria in quanto in tale area
 *    dichiarata come array di interi i valori sono memorizzati
 *    come floating
 */
float   estr_sh (indice)
int     indice;
{
int     i;
float   papp;		/* puntatore di appoggio */

    if (tipo_aing == 1)
    {
	RtDbPGetValue(dbpunti,indice,&papp);
    }
    else
    {
	RtDbPGetValue(dbpunti,indice,&papp);
    }

    return (papp);
}

static void legge_elenco_display()
{
XrmDatabase RisDb;
char *cdisp;
int i,lun;
char *getenv();



static XtResource res_elenco_display[]= {
                                {
                                XlNdisplayList,
                                XlCDisplayList,
                                XmRString,
                                sizeof(char *),
                                0,
                                XmRString,
                                ""
                                }
                                };
static XtResource res_numero_display[]= {
                                {
                                XlNnumDisplay,
                                XlCNumDisplay,
                                XmRInt,
                                sizeof(int),
                                0,
                                XmRImmediate,
                                0
                                }
                                };
RisDb=XrmGetFileDatabase(nome_file_context);
#if defined SCO_UNIX || defined TRU64_51 || defined LINUX
XrmSetDatabase(XtDisplay(topLevel),RisDb);
#else
display->db=RisDb;
#endif

XtVaGetApplicationResources(topLevel,&numero_display,res_numero_display,
                               XtNumber(res_numero_display),NULL);
XtVaGetApplicationResources(topLevel,&cdisp,res_elenco_display,
                               XtNumber(res_elenco_display),NULL);

numero_display=numero_display+1;
elenco_display=(AN_DISPLAY*)XtCalloc(numero_display+1,sizeof(AN_DISPLAY));
for(i=0;i<numero_display;i++)
        {
        if(i==0)
                {
                strcpy(elenco_display[i].nome,getenv("DISPLAY"));
                lun=strlen(elenco_display[i].nome);
                elenco_display[i].nome[lun-4]=0;
                }
        else
                {
                sscanf(cdisp,"%s",elenco_display[i].nome);
                cdisp+=(strlen(elenco_display[i].nome)+1);
                }
        }
for(i=0;i<numero_display;i++)
	printf("display[%d] = %s\n",i,elenco_display[i].nome);
}



static void open_display()
{
Arg args[5];		/* per settaggio argomenti widgets */
int i,j;
Display *app_display;
char testo_display[100];
int zero=0;



elenco_display[0].topLevel=XtAppCreateShell("xaing","Xaing", 
			applicationShellWidgetClass,display,NULL,0);

for(i=1;i<numero_display;i++)
        {
        printf("Apro Display %d %s:0.0\n",i,elenco_display[i].nome);
        sprintf(testo_display,"%s:0.0",elenco_display[i].nome);
        app_display=XtOpenDisplay(app_context,testo_display,"xaing","Xaing",
                NULL,0,&zero,NULL);
        if(app_display==NULL)
                {
		fprintf(stderr,"Xaing: Errore apertura display %s\n",testo_display);
		elenco_display[i].topLevel=NULL;
		}
	else
		{
		elenco_display[i].topLevel=XtAppCreateShell("xaing","Xaing", 
			applicationShellWidgetClass,app_display,NULL,0);
		}
	}
disp_sel=0;
}

static  int cerca_display(char *nome_display)
{
int j;

if(tipo_aing!=2)
	return(0);

    for(j=0;j<numero_display;j++)
	{
	if((strncmp(elenco_display[j].nome,nome_display,
			strlen(elenco_display[j].nome))==0)&&
			(elenco_display[j].topLevel!=NULL))
		return(j);
	}
if(j==numero_display)
	return(0);
}
