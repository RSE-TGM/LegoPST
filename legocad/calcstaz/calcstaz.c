/**********************************************************************
*
*       C Source:               calcstaz.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed Jun 18 09:29:47 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: calcstaz.c-6 %  (%full_filespec: calcstaz.c-6:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)calcstaz.c	1.12\t3/30/95";
/*
        Fine sezione per SCCS
*/

/* CALCSTAZ.C		Attivita' LEGO: Calcolo in Regime Stazionario */
/*			versione: 0.9			11.6.92 */

/* Buffo Massimo, Calleri Nicola 26/06/1992 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h> 
#include <X11/StringDefs.h>
#include <X11/keysym.h>
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/Label.h>
#include <Xm/CascadeB.h>
#include <Xm/BulletinB.h>
#include <Xm/ScrolledW.h>
#include <Xm/SeparatoG.h>
#include <Xm/Text.h>
#include <Xm/MessageB.h>
#include <Xm/DialogS.h>
#include <Mrm/MrmAppl.h>

#include "calcstaz.h"
#include "libutilx.h"
#include "ipcdata.h"
#include "voci_menu.h"
#include "utile.h"
#include "messaggi.h"
#include "f03.h"
#include "f11.h"

extern Widget create_fileSelectionBox(); 
extern void free_array_XmString( XmString, int );

int fd_out, fd_stdout;
typedef struct {
		  char *mesg[10];
                  int  num_mesg;
	       } info;

/* messaggi di informazione nella status window */
info info_mesg[] = {
                      { { "Welcome to steady state computation window" } ,1 },
                      { { "Error while opening data file f11.dat." } ,1 },
                      { { "Previous data loaded." } ,1 },
                      { { "Last data loaded"} ,1 },
                      { { "Error while opening communication channel (PIPE)",
                          "Process LG3 not started!" } ,2 },
                      { { "Calculating steady state ..." } ,1 },
                      { { ">>> Process LG3 aborted.",
                          ">>> CALCULATION NOT TERMINATED." } ,2 },
                      { { ">>> End of calculation.",
                          ">>>  Steady state calculation OK.",
                          ">>> CONVERGENCE CRITERION SATISFIED" } ,3 },
                      { { ">>> Steady state calculation is not OK.",
                          ">>> CONVERGENCE PROBLEM" } ,2 }
                   };

/* messaggi di errore */
char *error_mesg[] = {
			"can't make a copy of file f14.dat.",
			"can't create named pipe to_calcstaz.",
			"error while opening hierarchy.",
			"error fetching MAIN WINDOW widget",
			"can't fetch dialog box",
			"memory allocation error",
			"error while opening file f14.dat.",
			"error while opening file f03.dat."   };

/* Variabili utilizzate dalle funzioni XLib */
Display	     *display;
XFontStruct  *finfo;
XmFontList    fontList;
/* char 	     *font_name = "timB14"; */
char	     *font_edit = "Rom14";  /*  PARAMETRO DI EDITOR()  */

/* Contatore delle righe nella finestra di stato */
int	row_count_status_window = 0;

/* Variabili globali per settare le impostazioni del calcolo */
short stato_calcolo = STAZIONARIO_NON_ATTIVATO;
short computing_mode = DEFAULT_MODE;
char  *model_name = "";
double tolerance = DEFAULT_TOLERANCE_VALUE;

/* Variabili utilizzate nella scrolled-list dei risultati */
char     blocco_selezionato[9];
int      numero_blocchi = 0;
int      numero_dati = 0;
XmString *lista_blocchi = NULL;
XmString *lista_dati = NULL;

/* variabili globali utilizzate nella diagnostica della matrice jacobiana */
int      iterazione_selezionata;
char	 jac_singolare;
int 	 n_null_row, n_null_column, ar_null_row[200], ar_null_column[200];

/* Variabile utilizzata nella visualizzazione dei risultati */
int	 toggle_choice_results = DISPLAY_VARIABLE;

/* Variabili utilizzate nelle find della window dei risultati */
int      block_pos=0;   /* posizione corrente nel find dei blocchi */
int      data_pos=0;   /* posizione corrente nel find  delle variabili */

/* Variabili utilizzate dalle list per inserire gli elementi */
XmString *elenco_item;
int	 numero_item = 0;

/* Variabili utilizzate nelle liste delle equazioni e delle iterazioni */
int      eqz_scelta, iter_scelta;

/* Variabili utilizzate per il dimensionamento di alcuni vettori utilizzati */
/* nelle scrolled-list */
int      num_calcolo_residui = 0, num_linee_datiF14 = 0;

/* Descrittore del file F11.DAT */
int      fd_f11 = -1;

/* File pointers */
FILE     *fp_f14, *fp_lg3;

/* File FIFO */
int      fd_pipe;

/* PID del processo lg3 e stato del processo */
int      lg3_pid;
int      processo_terminato;

/* variabili utilizzate per la funzione XtAppAddTimeout() */
XtIntervalId timer;
XtAppContext app_context;

/* header del messaggio e messaggio */
header_mesg hmsg;
char messaggio[ 128 ];

/* offset del file F11.DAT per recuperare i risultati e offset del file */
/* F14.DAT per recupera i dati geometrici e fisici */
long int offset_ris_f11,offset_dati_geometrici;

/* Variabili utilizzate per la visualizzazione della matrice jacobiana */
char     *int_colonna_jac, *riga_jac;
XmString *int_riga_jac;

/* Opzioni dalla linea di comando    <applicativo> -model <nome modello> */
static XrmOptionDescRec	options[] = {
	{"-model", "*model", XrmoptionSepArg, NULL }
       };

static XtResource resources[] = {
   	{"model", "Model", XtRString, sizeof(String), 0, XtRString, "" }
       };
			

/* Variabili del resource manager */
MrmHierarchy s_RMHierarchy;
MrmType *dummy_class;
/*-----------------------------------------------------------------------*/
char *filename_uid = "calcstaz.uid";
char name_uid[4096];
int db_filename_num = 1;
/*-----------------------------------------------------------------------*/
Widget toplevel_widget, main_window_widget, warning_widget;
Widget exit_widget, info_widget,editf14_widget,editlg3_widget,printf24_widget;
Widget testo_f14, testo_lg3; /* WIDGET DI TESTO */
Widget widget_array[ MAX_WIDGETS ];

/***********************************************************/ 
/* Definizione dei button e delle callback per vari widget
   gestiti all'interno di msgbox_button() **************** */

static Elenco_callback funzioni_f24 = {
	{ "Ok",     msgbox_button, STAMPA_F24 },
	{ "Cancel", msgbox_button, CANCELLA },
	{ NULL,     NULL,          NULL }
};

static Elenco_callback funzioni_err = {
        { "Ok",     msgbox_button, VIEW_LG3_OUT },
        { "Cancel", msgbox_button, CANCELLA },
        { NULL,     NULL,          NULL }
};

static Elenco_callback funzioni_lg3 = {
	{ "Print",  msgbox_button, STAMPA_LG3 },
	{ "Cancel", msgbox_button, CANCELLA },
	{ NULL,     NULL,          NULL }
};

static Elenco_callback funzioni_f14 = {
	{ "Save",   msgbox_button, SALVA_F14 },
	{ "Cancel", msgbox_button, CANCELLA },
	{ NULL,     NULL,          NULL }
};
/***********************************************************/ 
/* Strutture riguardanti il geometry management delle varie Dialog...
   Specificare, nell'ordine: { DefaultPosition TRUE/FALSE,XmNx,XmNy,
			       XmNwidth,XmNHeight }		    */
Dialog_geometry geom_information = { FALSE, 700, 865, NULL, 120};
Dialog_geometry geom_attention   = { TRUE, NULL, NULL, NULL, 120};
Dialog_geometry geom_editor      = { FALSE, 605, 20, 650, 750};
/***********************************************************/ 

/* Variabili per la registrazione delle funzioni di callback */
static MrmRegisterArg reglist[] = {
       { "create_proc",   (caddr_t) create_proc } ,
       { "cancel_proc",   (caddr_t) cancel_proc } ,
       { "ok_proc",       (caddr_t) ok_proc } ,
       { "dialog_proc",   (caddr_t) dialog_proc } ,
       { "activate_proc", (caddr_t) activate_proc } ,
       { "show_results",  (caddr_t) show_results } ,
       { "selez_proc",    (caddr_t) selez_proc } ,
       { "start_proc",    (caddr_t) start_proc } ,
       { "find_proc",     (caddr_t) find_proc } ,
       { "show_jacobian", (caddr_t) show_jacobian} ,
       { "selez_jac_proc",(caddr_t) selez_jac_proc}
};

static int reglist_num = (sizeof reglist / sizeof reglist[0]);

static int cerca_stringa(  );



/*-----------------------------------------------------------------------*/
void main(int argc,char **argv)
{
    Cardinal argcount;
    Arg args[10];
    char *path_uid;

/* copia il file f14.dat nel file temporaneo di lavoro (f14.tmp) */
    if ( copy_file(FILE_F14,FILE_F14_TMP) == -1 )
	s_error( APPLICATION_NAME, error_mesg, ECOPYF14, 1 );

/* crea la named pipe di comunicazione con il processo LG3 */
/* (se non esiste gia'!) */
   if ( access(COMM_PIPE, F_OK) )
      if ( mknod(COMM_PIPE, S_IFIFO | 0666, 0) )
	s_error( APPLICATION_NAME, error_mesg, ECREAPIPE, 1 );

/* Apri il file dei dati geometrici e leggi l'offset del primo blocco */
    leggi_dati_geometrici();

    fprintf(stdout,"initializing toolkit ...\n");
    MrmInitialize ();  /* INIZIALIZZARE Mrm PRIMA DEL TOOLKIT  */
    toplevel_widget = XtInitialize(argv[0], "Calcstaz", options, 
                                   XtNumber(options), &argc, argv);

    display = XtDisplay(toplevel_widget);

    argcount = 0;
    XtSetArg(args[argcount], XmNtitle, WINDOW_TITLE); argcount++;
    XtSetArg(args[argcount], XmNiconName, ICON_NAME); argcount++;
    XtSetValues (toplevel_widget, args, argcount);

/* Le UnitType di tutti gli widget sono in 100TH_FONT_UNITS; */
/* una unita' di font viene posta a 100 con XmSetFontUnit	*/
    XmSetFontUnit (display,100);

/* Creazione delle finestre di warning utilizzate per visualizzare dei */
/* messaggi */
    warning_widget = attention(toplevel_widget,"",CREA,geom_attention);

/* Creazione della finestra di information sullo jacobiano  */
    info_widget = information (toplevel_widget,"",CREA,geom_information);

/* Recupera eventualmente il nome del modello dalla linea di comando */
    XtGetApplicationResources(toplevel_widget, &model_name, resources,
			      XtNumber(resources), NULL, 0);

/* Recuperando il nome del modello dalla linea di comando bisogna abilitare */
/* il programma (calcstaz) a crearsi i percorsi di ricerca dei file F14.DAT */
/* F01.DAT, F03.DAT e F11.DAT del modello selezionato. */

/* lettura del file binario F03.DAT e dimensionamento dinamico dei vettori */
    if ( read_file_f03() )
	s_error( APPLICATION_NAME, error_mesg, EOPENF03, 1 );

    if ( dim_array_f11(neqsis,npvrt,nu,nvart) )
	s_error( APPLICATION_NAME, error_mesg, EMEMALLOC, 1 );

    model_name = (char *) XtCalloc(sizeof(char),(unsigned) 9);
    copy_n_car(model_name,sigla,8);

/* Costruisci il path corretto per il file UID */
#if defined (VMS)
    path_uid=getenv("LEGOCAD_UID");
    strcpy(name_uid,path_uid);
#else
    path_uid=getenv("LEGOCAD_UID");
    strcpy(name_uid,path_uid);
    strcat(name_uid,"/");
#endif
    strcat(name_uid,filename_uid);
    path_uid = name_uid;

    if (MrmOpenHierarchy(db_filename_num, &path_uid, NULL,
                         &s_RMHierarchy ) != MrmSUCCESS)
	s_error( APPLICATION_NAME, error_mesg, EOPENHIER, 1 );

/* Registrazione dei nomi delle funzioni di callback */
    MrmRegisterNames(reglist, reglist_num);
    if (MrmFetchWidget(s_RMHierarchy,"MAIN_WINDOW",toplevel_widget,
                       &main_window_widget, &dummy_class) != MrmSUCCESS )
	s_error( APPLICATION_NAME, error_mesg, EFETCHMWIN, 1 );

    XtManageChild(main_window_widget);

    app_context = XtWidgetToApplicationContext(main_window_widget);

/* Installazione degli acceleratori definiti nel file */
/* ../app-defaults/Calcstaz */
    XtInstallAccelerators(main_window_widget,widget_array[K_START]);
    XtInstallAccelerators(main_window_widget,widget_array[K_STEP]);
    XtInstallAccelerators(main_window_widget,widget_array[K_EXIT_PROGRAM]);
    XtRealizeWidget(toplevel_widget);

 /* Disabilita la selezione all'interno della window di stato */
    XSelectInput(display, XtWindow(widget_array[ K_STATUS_WINDOW ]),
                 ExposureMask);

/* Leggi le Options nel file FILE_USER_SETTING ( se esiste ) */
   read_user_setting(); 
 
/* Inizializza le voci dei menu */
    if ( Empty(model_name) )
    {
       cambia_stato_menu( numero_voci_menu, voci_senza_modello );
       cambia_stato_menu( numero_voci_button, button_senza_modello );
    }
    else
    {
       cambia_stato_menu( numero_voci_menu, voci_non_attivato );
       set_something(widget_array[K_MODEL_NAME], XmNlabelString,
		     (void*) CREATE_CSTRING(model_name));
  /* Attiva il push-button corretto ('Start' o 'Step') */
       if (computing_mode == DIRECT_MODE)
          cambia_stato_menu( numero_voci_button, button_direct );
       else
          cambia_stato_menu( numero_voci_button, button_interactive );
    }

    s_information( IWELCOME );

    exit_widget = NULL;
 
    XtMainLoop();
} /* Fine main() */

/*-----------------------------------------------------------------------*/
/* Funzioni specifiche del programma					 */
/*-----------------------------------------------------------------------*/
/*
 *** cambia_stato_menu()
 *** parametri:
 *** 	        short nvoci : numero di voci nell'array *voci_array 
 *** 	        VOCE_MENU *voci_array : contiene la coppia indice nel vettore
 ***					dei wigdet e stato del menu (attivo/
 ***					non attivo)
 Procedura per aggiornare lo stato delle voci di menu */
cambia_stato_menu( nvoci, voci_array )
short nvoci;
VOCE_MENU *voci_array;
{
   short i;

   for (i=0; i<nvoci; i++ )
      set_something(widget_array[voci_array[i].ind],XmNsensitive, (void*) voci_array[i].stato );
}
/*-----------------------------------------------------------------------*/

/*** read_user_setting()
 *** nessun parametro.
Funzione per la lettura delle options dal file FILE_USER_SETTING 
verificando che tale file esista. Se non esiste si settano i parametri di 
default */
void read_user_setting() 
{
   char   buffer[80], stringa[20], valore[10];
   double new_tolerance;
   short  new_mode;
   FILE	  *fp;

   if ( (fp=fopen(FILE_USER_SETTING,"r")) == NULL)
      return;

   new_tolerance = tolerance;
   new_mode = computing_mode;

   while ( fgets(buffer, 80, fp) != NULL )
   {
      /* Se il primo carattere di linea e' # allora si tratta di un commento */
      if ( buffer[0] == '#' )
         continue;

      sscanf(buffer,"%s %s",stringa,valore); 

      if ( !strcmp(stringa,"tolerance") )
	 new_tolerance = atof(valore);

      if ( !strcmp(stringa,"computing_mode") )
	 new_mode = atoi(valore);
   }

   modify_options(new_tolerance, new_mode);

   fclose( fp );
}

/*-----------------------------------------------------------------------*/
/*** modify_options( new_tolerance, new_mode )
 *** parametri
 ***      double new_tolerance: nuovo valore di tolleranza
 ***      short  new_mode: nuova modalita' di calcolo
Modifica i valori delle rispettive variabili globali e cambia i testi delle
rispettive label */
modify_options( new_tolerance, new_mode )
double new_tolerance;
short  new_mode;
{
char temp[20];

   if ( tolerance != new_tolerance )
   {
      tolerance = new_tolerance;
  /* Cambia il testo della label che contiene il valore della tolleranza */
      sprintf(temp,"%.8lf",tolerance);
      trim_zero(temp);
      set_something( widget_array[K_TOLERANCE_VALUE],
			XmNlabelString, (void*) CREATE_CSTRING(temp));
   }

   if ( computing_mode != new_mode )
   {
      computing_mode = new_mode;
      if (computing_mode == DIRECT_MODE)
      {
      	  set_something(widget_array[K_COMPUTING_MODE],
	   	        XmNlabelString, (void*) DIRECT_STRING);
          if ( !Empty(model_name) )
             cambia_stato_menu( numero_voci_button, button_direct );
      }
      else
      {
	  set_something(widget_array[K_COMPUTING_MODE],
	    	        XmNlabelString, (void*) INTERACTIVE_STRING);
          if ( !Empty(model_name) )
             cambia_stato_menu( numero_voci_button, button_interactive );
      }
   }
}
/*-----------------------------------------------------------------------*/

/*** mesg_status_window( messaggio )
 *** parametro:
 ***      char *messaggio;
Stampa un messaggio nella finestra di stato */
mesg_status_window( messaggio )
char *messaggio;
{
   Cardinal argcount;
   Arg args[10];

   if ( row_count_status_window > MAX_ROW_STATUS_WINDOW )
      XmListDeletePos(widget_array[ K_STATUS_WINDOW ], 1);
   else
      row_count_status_window++ ;

   XmListAddItemUnselected(widget_array[ K_STATUS_WINDOW ], 
		           CREATE_CSTRING(messaggio), 0);
   XmListSetBottomPos(widget_array[K_STATUS_WINDOW], 0);

   argcount = 0;
   XtSetArg(args[argcount], XmNvisibleItemCount,VISIBLE_ITEMS); argcount++;
   XtSetValues (widget_array[K_STATUS_WINDOW], args, argcount);
} 


/*-----------------------------------------------------------------------*/
/*** valori_var_blocco(indice, noto,val1,val2)
 *** Parametri:
 ***      indice : indice della variabile nel vettore var
 ***        noto : (char *) viene assegnato un * se la variabile e' nota
 ***        val1 : (double *) viene assegnato il valore iniziale della
 ***               variabile ( letto dal vettore xy0 ).
 ***        val2 : (double *) viene assegnato il valore corrente della
 ***               variabile ( letto dal vettore xy ).
Funzione che restituisce i valori di una data variabile ( indicata tramite
l'indice del vettore var ).La funzione fa' riferimento ai valori contentuti
nei vettori xy e xy0 utilizzando il vettore di indici ipvrs_f11.
(Attualmente, viene utilizzata nella funzione show_results). */
valori_var_blocco(indice, noto, val1, val2)
int indice;
char *noto;
double *val1, *val2;
{
   if ( ipvrs_f11[indice] > 0 )
   {
       *noto = ' ';
       *val1 = xy0[ ipvrs_f11[indice]-1 ] * cnxyu[indice];
       *val2 = xy[ ipvrs_f11[indice]-1 ] * cnxyu[indice];
   }
   else
   {
       *noto = '*';
       *val1 = uu[ -ipvrs_f11[indice]-1 ] * cnxyu[indice];
       *val2 = uu[ -ipvrs_f11[indice]-1 ] * cnxyu[indice];
    }
    return(0);
}

/*-----------------------------------------------------------------------*/
/*** alloca_memoria_x_items(num_items)
 *** Parametri:
 ***    num_items : numero di elementi da allocare.
Funzione utilizzata per allocare l'array di XmString elenco_item utilizzato per 
riempire le scrolled-list. Se l'allocazione fallisce ,ritorna un errore
al sistema operativo. */
alloca_memoria_x_items(num_items)
int num_items;
{
   elenco_item = (XmString *) XtCalloc(sizeof(XmString),num_items);
   if (elenco_item == NULL)
	s_error( APPLICATION_NAME, error_mesg, EMEMALLOC, 1 );
   return(0);
}

/*-----------------------------------------------------------------------*/
/* 	FUNZIONI DI CALLBACK... */
/*-----------------------------------------------------------------------*/

 int cancel_save()
{

   exit_widget = NULL;
}
/*-----------------------------------------------------
 * salva_as()    5 Jan 95
 *
 * questa routine e' attivata dallla callback del 
 * bottone di Ok della fileselectionbox ecit_widget
 * creata dalla callback di Exit di calcstaz
 *------------------------------------------------------*/
 
 int salva_as()
 {
   Arg arg[2];
   Cardinal narg;
   XmString cstring;
   char *str;

/* recupero il nome del file di output */
   narg=0;
   XtSetArg(arg[0],XmNtextString,&cstring);narg++;
   XtGetValues(exit_widget,arg,narg);
   str=extract_string(cstring);
   printf("Output file %s\n",str);

   if( str[strlen(str)-1] != '/' )
   {

/* salvo il file  e esco*/
      printf("Salvo il file proc/f24.dat su f14.dat \n");
      if ( copy_file(FILE_F24, str ) != -1 )
         fprintf(stdout, "Saved results in F14.DAT !\n");

      /* Chiudi il file dei dati geometrici e fisici */
      unlink(COMM_PIPE); 
      unlink(FILE_F14_TMP); 
      XtCloseDisplay (display);
      exit(0); 
   }
 }


/*-----------------------------------------------------
 * non_salvare()   5 Jan95 
 *
 * questa routine viene chiamata dalla callback del
 * bottone di help della fileselectionbox exit_widget
 * creata dalla callback di Exit di calcstaz
 *-----------------------------------------------------*/
int non_salvare()
{
   /* Chiudi il file dei dati geometrici e fisici */
   unlink(COMM_PIPE); 
   unlink(FILE_F14_TMP); 
   XtCloseDisplay (display);
   exit(0); 
}

/*** msgbox_button( w, client_data, call_data)
 ***
funzione di gestione callback dei bottoni dei transient widget  */
void msgbox_button( w, client_data, call_data)
Widget w;
int client_data;
XmAnyCallbackStruct *call_data;
{
   char comando[200];
   char *cont;
   /*  Widget testo == dichiarato EXTERN in 'calcstaz.c'
			       e GLOBALE in 'utilx.c'		*/
   switch (client_data) {

	case ESCI_E_SALVA :
             if ( copy_file(FILE_F24, FILE_F14) != -1 )
                fprintf(stdout, "Saved results in F14.DAT !\n");
	case ESCI_SENZA_SALVARE :
         /* Chiudi il file dei dati geometrici e fisici */
             unlink(COMM_PIPE); 
             unlink(FILE_F14_TMP); 
   	     XtCloseDisplay (display);
             exit(0); 

	case SALVA_F14:
	     fp_f14 = fopen (FILE_F14_TMP,"w");
	     if ((cont=XmTextGetString(testo_f14)) == NULL)
                return;
	     fputs (cont, fp_f14);
	     fclose (fp_f14);
             XtFree(cont);
	     break;

	case STAMPA_F24:
	     if (FILE_F24 != NULL)
	         print_file(FILE_F24);
	     break;

	case STAMPA_LG3:
	     fp_lg3 = fopen (FILE_LG3_TMP,"w");
	     if ((cont=XmTextGetString(testo_lg3)) == NULL)
                return;
	     fputs (cont, fp_lg3);
	     fclose (fp_lg3);
             XtFree(cont);
	     print_file(FILE_LG3_TMP);
	     break;

	case VIEW_LG3_OUT:
/*
             strcpy(comando,"edit_f01   ");
             strcat(comando,"lg3.out");
             strcat(comando,"  False &");

             if( system(NULL) != NULL)
                system( comando );
*/
	     editlg3_widget = editor (toplevel_widget,&testo_lg3,
		              FILE_LG3,TRUE,font_edit,funzioni_lg3,geom_editor);
             break;	

        case CANCELLA:  
             XtDestroyWidget (XtParent(w));
	     break;
   }
}


/*-----------------------------------------------------------------------*/
/*
 *** create_proc()
 *** parametri:
 *** 		Widget w : identificativo del widget che ha effettuato la
 ***			   callback
 ***		int *widget_num : parametro passato dalla callback
 ***         			 (client data)
 ***		unsigned long *reason : non rilevante
 Procedura di callback che aggiorna l'array dei widget */
void create_proc(w, widget_num, reason)
Widget w;
int *widget_num;
unsigned long *reason;
{
   widget_array[ *widget_num ] = w;
}

/*-----------------------------------------------------------------------*/
/*
 *** dialog_proc()
 *** parametri
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *param : 
 ***   unsigned long *reason : non rilevante
 Procedura di callback per l'attivazione delle DialogBox */
void dialog_proc(w, widget_num, reason)
Widget w;
int *widget_num;
unsigned long *reason;
{
   char temp[100];
   long *iterazioni;
   int  *n_equazione;
   Arg args[10];
   Cardinal argcount;
   XmString string = NULL;
   float residuo_max;
   int i, eqz_out, num_itert, prec_itert, trovato;
   char calcolo_jac, diff_conv;  /* jac_singolare  --->GLOBALE  */

   def_cursore (toplevel_widget, OROLOGIO);
   switch ( *widget_num ) {
    
/***************************************  dialog_proc   ****************/
       case K_TOLERANCE_DIALOG:
/***********************************************************************/
         if (widget_array[ *widget_num ] == NULL )
             if (MrmFetchWidget(s_RMHierarchy, "tolerance_dialog",
    			     toplevel_widget,
			     &widget_array[ *widget_num ],
			     &dummy_class) != MrmSUCCESS )
		s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );
         XtManageChild( widget_array[ *widget_num ] );

	 XtSetKeyboardFocus (widget_array[K_TOLERANCE_DIALOG],
			     widget_array[K_TOLERANCE_DIALOG_TEXT ]);
         sprintf(temp,"%.8lf",tolerance);
         trim_zero(temp);
         XmTextSetString(widget_array[K_TOLERANCE_DIALOG_TEXT],temp);
         break;

/***************************************  dialog_proc   ****************/
       case K_STEADY_STATE_RESULTS_DIALOG:
/***********************************************************************/
     /* verifica apertura del file F11.DAT */
         if ( fd_f11 == -1 )
         {
            s_information( IERROPENF11 );
            undef_cursore (toplevel_widget);
            return;
         }

         if (widget_array[ *widget_num ] == NULL )
             if (MrmFetchWidget(s_RMHierarchy, "steady_state_results_dialog",
    			     toplevel_widget,
			     &widget_array[ *widget_num ],
			     &dummy_class) != MrmSUCCESS )
		s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );

         XtManageChild( widget_array[ *widget_num ] );
/******* XtSetKeyboardFocus (widget_array[K_STEADY_STATE_RESULTS_DIALOG],
			     widget_array[K_BLOCK_LIST_RESULTS]); *******/

      /* Alloca la memoria per l'elenco dei blocchi , se non e' gia' */
      /* stata allocata. */
         if ( lista_blocchi == NULL )
            if ( (lista_blocchi = (XmString *) 
                                  XtCalloc(sizeof(XmString),nbl+1)) == NULL )
		s_error( APPLICATION_NAME, error_mesg, EMEMALLOC, 1 );

      /* Primo elemento della lista: selezione di tutti i blocchi */
         strcpy(temp,"********  ALL MODEL BLOCKS");
         lista_blocchi[ 0 ] = CREATE_CSTRING(temp);

      /* Lettura blocchi dal vettore nom_bloc letto dal file F03.DAT */
         for ( i=1 ; i<=nbl ; i++ )
         {
	    copy_n_car(temp, nom_bloc[i-1], 75);
            lista_blocchi[ i ] = CREATE_CSTRING(temp);
         }

         numero_blocchi = nbl+1;

      /* Aggiorna la scrolled list con la lista dei blocchi */
         argcount = 0;
         XtSetArg(args[argcount], XmNitemCount, numero_blocchi); argcount++; 
         XtSetArg(args[argcount], XmNitems, lista_blocchi ); argcount++;
         XtSetValues (widget_array[K_BLOCK_LIST_RESULTS], args, argcount);

         break;

/***************************************  dialog_proc   ****************/
       case K_EQUATION_WINDOW:
/***********************************************************************/

     /* verifica apertura del file F11.DAT */
         if ( fd_f11 == -1 )
         {
            s_information( IERROPENF11 );
            undef_cursore (toplevel_widget);
            return;
         }

         if (widget_array[ *widget_num ] == NULL )
             if (MrmFetchWidget(s_RMHierarchy, "equation_dialog",
    			     toplevel_widget,
			     &widget_array[ *widget_num ],
			     &dummy_class) != MrmSUCCESS )
		s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );

         XtManageChild( widget_array[ *widget_num ] );

	 XtSetKeyboardFocus (widget_array[K_EQUATION_WINDOW],
			     widget_array[K_EQUATION_LIST]);

         iterazioni = (long *) XtCalloc(sizeof(long),(unsigned) neqsis);
         n_equazione = (int *) XtCalloc(sizeof(int) ,(unsigned) neqsis);
         if ( iterazioni == NULL || n_equazione == NULL )
		s_error( APPLICATION_NAME, error_mesg, EMEMALLOC, 1 );

     /* Inizializzazione dei contatori */
     /* n_equazione contiene il numero dell'equazione corrispondente. */
     /* (all'inizio e' un numero progressivo.E' utile per recuperare il */
     /* numero dell'equazione dopo l'ordinamento ). */
         for( i=0 ; i<neqsis ; i++ )
         {
            iterazioni[i] = 0;
            n_equazione[i] = i;
         }

     /* Lettura dei dati del calcolo dello stazionario dal file F11.DAT */
         lseek(fd_f11, offset_ris_f11, SEEK_SET);

     /* ciclo di lettura dei dati */
         while( !read_data_f11( fd_f11 ))
             if ( irc11 == C_RESIDUI )
                for( i=0 ; i<neqsis; i++ )
                   iterazioni[i] += iconv[i];

     /* Visualizzazione dei dati nella window di diagnostica */
     /* Allocazione della memoria */
         alloca_memoria_x_items(neqsis);
         numero_item = 0;

         for ( i=0 ; i<neqsis ; i++ ) 
         {
            int eqz, j;

            for( j=i+1 ; j<neqsis ; j++ )
               if ( iterazioni[ i ] < iterazioni[ j ] )
               {
                  int iter;

                  iter = iterazioni[ i ];
                  eqz  = n_equazione[ i ];
                  iterazioni[ i ] = iterazioni[ j ];
                  n_equazione[ i ] = n_equazione[ j ];
                  iterazioni[ j ] = iter;
                  n_equazione[ j ] = eqz;
               }

   /* visualizza l'equazione solo se e' stata fuori per almeno una volta */
             if (iterazioni[ i ] > 0 )
             {
                eqz = n_equazione[ i ];
                sprintf(temp, SF_EQUATION_LIST, eqz+1,
                        noblc[ iresbl[eqz]-1 ],iterazioni[i]);
                elenco_item[ numero_item++ ] = CREATE_CSTRING(temp);
             }
         }

         argcount = 0;
         XtSetArg(args[argcount], XmNitemCount, numero_item); argcount++; 
         XtSetArg(args[argcount], XmNitems, elenco_item ); argcount++;
         XtSetArg(args[argcount], XmNvisibleItemCount ,12); argcount++;
         XtSetValues (widget_array[ K_EQUATION_LIST ], args, argcount);

    /* Libera la memoria utilizzata */
         free_array_XmString(elenco_item, numero_item ); 
         XtFree( iterazioni );
         XtFree( n_equazione );
         XtFree( elenco_item );
	 break;

/***************************************  dialog_proc   ****************/
       case K_ITERATION_WINDOW:
/***********************************************************************/
     /* verifica apertura del file F11.DAT */
         if ( fd_f11 == -1 )
         {
            s_information( IERROPENF11 );
            undef_cursore (toplevel_widget);
            return;
         }

         if (widget_array[ *widget_num ] == NULL )
             if (MrmFetchWidget(s_RMHierarchy, "iteration_dialog",
    			     toplevel_widget,
			     &widget_array[ *widget_num ],
			     &dummy_class) != MrmSUCCESS )
		s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );

         XtManageChild( widget_array[ *widget_num ] );

	 XtSetKeyboardFocus (widget_array[K_ITERATION_WINDOW],
			     widget_array[K_ITERATION_LIST]);

	 string = XmStringCreate(ITERATION_LIST_LABEL_TEXT_1, CS);
	 string = XmStringConcat(string,XmStringSeparatorCreate());
	 string = XmStringConcat(string,XmStringCreate
				 (ITERATION_LIST_LABEL_TEXT_2, CS));
	 set_something (widget_array[K_ITERATION_LABEL], XmNlabelString,
			(void*) string);

         sprintf(temp,"%4d",neqsis);
	 set_something (widget_array[K_NEQSIS_LABEL], XmNlabelString,
			(void*) CREATE_CSTRING(temp));

     /* Allocazione della memoria */
         alloca_memoria_x_items(num_calcolo_residui+1);

     /* Lettura dei dati del calcolo dello stazionario dal file F11.DAT */
         lseek(fd_f11, offset_ris_f11, SEEK_SET);

         num_itert = 0;
         calcolo_jac = ' ';
         diff_conv = ' ';
         numero_item = 0;
         trovato = 0;

     /* ciclo di lettura dei dati */
         while( !read_data_f11( fd_f11 ) )
         {
            double p_norma;

            if ( (irc11 == C_RESIDUI || irc11 == C_NON_OK || irc11 == C_OK)
                  && trovato )
            {
               if ( eqz_out > 0 )
               {
                  char str1[20],str2[20];

                  sprintf(temp, SF_ITERATION_LIST,
                          num_itert,calcolo_jac,diff_conv,
                          nzero(str1,residuo_max,"% 10.4e"," 0."),
                          nzero(str2,p_norma,"% 8.4e"," 0."),
                          eqz_out );
                  elenco_item[ numero_item++ ] = CREATE_CSTRING(temp);
               }
               calcolo_jac = ' ';
               diff_conv = ' ';
               num_itert = itert;
            }
  
            if ( irc11 == C_NON_OK || irc11 == C_OK )
               break;

            switch (irc11 )
            {
               case 1: 
                   residuo_max = tolerance;
                   eqz_out = 0;
                   trovato = 1;
                   p_norma = norma;

                /* Cerca il residuo max e calcola il numero di equazioni */
                /* che non convergono */
                   for( i=0 ; i<neqsis ; i++ )
                   {
                      if ( fabs(rn[i]) > fabs(residuo_max) )
                         residuo_max = rn[i];
                      eqz_out += iconv[i];
                   }
                   break;
           
               case 3:
                   calcolo_jac = '*';
                   break;

               case 4:
                   diff_conv = '*';
                   break;
            }
         }

         argcount = 0;
         XtSetArg(args[argcount], XmNitemCount, numero_item); argcount++; 
         XtSetArg(args[argcount], XmNitems, elenco_item ); argcount++;
         XtSetArg(args[argcount], XmNvisibleItemCount ,10); argcount++;
         XtSetValues (widget_array[ K_ITERATION_LIST ], args, argcount);

    /* Libera la memoria utilizzata */
         free_array_XmString(elenco_item, numero_item ); 
         XtFree(elenco_item);
	break;

/***************************************  dialog_proc   ****************/
       case K_JACOBIAN_WINDOW:
/***********************************************************************/
     /* verifica apertura del file F11.DAT */
         if ( fd_f11 == -1 )
         {
            s_information( IERROPENF11 );
            undef_cursore (toplevel_widget);
            return;
         }

         if (widget_array[ K_JACOBIAN_WINDOW ] == NULL )
             if (MrmFetchWidget(s_RMHierarchy, "jacobian_dialog",
    			     toplevel_widget,
			     &widget_array[ K_JACOBIAN_WINDOW ],
			     &dummy_class) != MrmSUCCESS )
		s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );

         XtManageChild( widget_array[ K_JACOBIAN_WINDOW ] );


	 XtSetKeyboardFocus (widget_array[K_JACOBIAN_WINDOW],
			     widget_array[K_JACOBIAN_LIST]);

     /* Allocazione della memoria */
         alloca_memoria_x_items(num_calcolo_residui);

         numero_item = 0;

     /* Lettura dei dati del calcolo dello stazionario dal file F11.DAT */
         lseek(fd_f11, offset_ris_f11, SEEK_SET);

         calcolo_jac = ' ';
         jac_singolare = ' ';
         num_itert = 0;

     /* ciclo di lettura dei dati */

         while( !read_data_f11(fd_f11) && irc11 != C_NON_OK && irc11 != C_OK )
         {
            if ( irc11 == C_JACOBIANO )
            {
               calcolo_jac = '*';
               if ( abs(iflag) == 1 || abs(iflag) == 2 )
                  jac_singolare = '*';
            }

            if ( itert > num_itert )
            {
               sprintf(temp, SF_JACOBIAN_LIST,
                            num_itert, calcolo_jac, jac_singolare);
               elenco_item[ numero_item++ ] = CREATE_CSTRING(temp);
               num_itert = itert;
               calcolo_jac = ' ';
               jac_singolare = ' ';
            }
         }

      /* Aggiorna la scrolled list con la lista dei blocchi */
         argcount = 0;
         XtSetArg(args[argcount], XmNitemCount, numero_item); argcount++; 
         XtSetArg(args[argcount], XmNitems, elenco_item ); argcount++;
         XtSetArg(args[argcount], XmNvisibleItemCount ,10); argcount++;
         XtSetValues (widget_array[K_JACOBIAN_LIST], args, argcount);

    /* Disabilito le scelte prima che avvenga la selezione... */
	 cambia_stato_menu (numero_voci_jacobiano,analisi_jacobiano_no);

    /* Alloca la memoria per le eventuali visualizzazioni della matrice */
         int_colonna_jac = (char *) XtCalloc(sizeof(char),neqsis*12+1);
         riga_jac = (char *) XtCalloc(sizeof(char),neqsis*12+1);
         int_riga_jac = (XmString *) XtCalloc(sizeof(XmString),neqsis);

    /* Libera la memoria utilizzata */
         free_array_XmString(elenco_item, numero_item ); 
         XtFree(elenco_item);
	break;
   }
   undef_cursore (toplevel_widget);
}

/*-----------------------------------------------------------------------*/
/*
 *** selez_proc()
 *** parametri
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *param : 
 ***   XmListCallbackStruct *list_info : struttura che contiene le informazioni
			sulla scrolled-list (item selezionato , ecc.. )
 Procedura di callback delle scrolled list ogni volta che viene effettuata
una selezione di un elemento della lista */
void selez_proc(w, widget_num, list_info)
Widget               w;
int                  *widget_num;
XmListCallbackStruct *list_info;
{
   char *item, stringa[100],temp[10],temp2[11];
/* Modifica del 29/6/94  per Alfa
   long equazione, iterazione, offset_tmp;
*/
   long iterazione, offset_tmp;
   int equazione;
   int  i, blk_selez, jac_row, k, pos_item, flag;
   int  *n_equazione, *punt_int;
   float *residui_eqz;
   Cardinal argcount;
   Arg args[10];
   XmString c_string;

/* Converti la compound string selezionata in array di caratteri */
   item = extract_string( list_info->item );
   pos_item = list_info->item_position-1;

   def_cursore (widget_array[ *widget_num ], OROLOGIO);
   switch ( *widget_num ) {

/***************************************   selez_proc   ****************/
       case K_RESULTS_LIST: /* Dialog box dei risultati */
/***********************************************************************/
     /* Aggiorna la posizione dell'item selezionato con il mouse */
          data_pos = pos_item;
          break;
    
/***************************************   selez_proc   ****************/
       case K_BLOCK_LIST_RESULTS: /* Dialog box dei risultati */
/***********************************************************************/
   def_cursore (widget_array[K_STEADY_STATE_RESULTS_DIALOG], OROLOGIO);
          block_pos = list_info->item_position-1;
          copy_n_car(blocco_selezionato,item,8);
          show_results(NULL, &toggle_choice_results, NULL);
   undef_cursore (widget_array[K_STEADY_STATE_RESULTS_DIALOG]);
          break;

/***************************************   selez_proc   ****************/
       case K_EQUATION_LIST:
/***********************************************************************/
          if (widget_array[ K_ITERATION_EQUATION_WINDOW ] == NULL )
             if (MrmFetchWidget(s_RMHierarchy, "iteration_equation_dialog",
    	  		        toplevel_widget,
			        &widget_array[K_ITERATION_EQUATION_WINDOW],
			        &dummy_class) != MrmSUCCESS )
		s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );

          XtManageChild( widget_array[K_ITERATION_EQUATION_WINDOW] );

	  XtSetKeyboardFocus (widget_array[K_ITERATION_EQUATION_WINDOW],
			      widget_array[K_ITERATION_EQUATION_LIST]);

          if (widget_array[ K_VARIABLE_EQUATION_WINDOW ] != NULL )
              XtUnmanageChild( widget_array[K_VARIABLE_EQUATION_WINDOW] );

       /* Leggi il numero dell'equazione dall'item selezionato ( sono le 
          prime quattro cifre ) */
          sscanf(item,"%4d",&equazione);

/* Passaggio del titolo alla finestra dei residui  */
	  sprintf(stringa,
		  "RESIDUALS HISTORY AT SYSTEM EQUATION # %d",equazione);
          set_something( widget_array[K_ITERATION_EQUATION_WINDOW],
	   		 XmNdialogTitle,(void*) CREATE_CSTRING(stringa)); 
        
       /* Questa variabile viene settata per recuperare */
       /* il numero dell'equazione di sistema nella visualizzazione */
       /* delle variabili del blocco relativo all'equazione */
          eqz_scelta = equazione;

      /* Decrementa equazione ( gli array in 'C' cominciano da 0 ... ) */
          equazione--;

     /* Aggiornamento delle label della window */
          copy_n_car(stringa,noblc[ iresbl[equazione]-1 ],8);
          set_something( widget_array[K_BLOCK_VALUE_ITER_EQ_LABEL],
	 		 XmNlabelString,(void*) CREATE_CSTRING(stringa));
/*
          copy_n_car(stringa,nosub[ iresbl[equazione]-1 ],4);
          set_something( widget_array[K_MODULE_VALUE_ITER_EQ_LABEL],
	 		 XmNlabelString,(void*) CREATE_CSTRING(stringa));
*/
          copy_n_car(stringa,nom_bloc[ iresbl[equazione]-1 ]+39,40);
          set_something( widget_array[K_DESCRIPT_VALUE_ITER_EQ_LABEL],
	 		 XmNlabelString,(void*) CREATE_CSTRING(stringa));

          sprintf(stringa,"%4d",ibleqz[ equazione ]);
          set_something( widget_array[K_NUMEQ_VALUE_ITER_EQ_LABEL],
	 		 XmNlabelString,(void*) CREATE_CSTRING(stringa));

          copy_n_car(stringa,signeq[ equazione ],50);
          set_something( widget_array[K_MEANING_VALUE_ITER_EQ_LABEL],
	 		 XmNlabelString,(void*) CREATE_CSTRING(stringa));

          copy_n_car(stringa,uniteq[ equazione ],10);
          set_something( widget_array[K_UNIT_VALUE_ITER_EQ_LABEL],
	 		 XmNlabelString,(void*) CREATE_CSTRING(stringa));

       /* Alloca la memoria */
          alloca_memoria_x_items(num_calcolo_residui);

       /* Lettura dei dati del calcolo dello stazionario dal file F11.DAT */
          lseek(fd_f11, offset_ris_f11, SEEK_SET);

          numero_item = 0;

       /* ciclo di lettura dei dati ed inserimento nel vettore di compound
          string */
          while( !read_data_f11( fd_f11 ))
             if ( irc11 == C_RESIDUI )
             {
               char str1[20],str2[20];

                sprintf(stringa, SF_ITERATION_EQUATION_LIST, itert,
                     nzero(str1,rn[equazione],"% 8.4e"," 0."),
                     nzero(str2,rn[equazione]*cosnor[equazione],"% 8.4e"," "));
                elenco_item[ numero_item++ ] = CREATE_CSTRING(stringa);
             }

       /* Visualizzazione dei dati nella window di diagnostica */
          argcount = 0;
          XtSetArg(args[argcount], XmNitemCount, numero_item); argcount++; 
          XtSetArg(args[argcount], XmNitems, elenco_item ); argcount++;
          XtSetArg(args[argcount], XmNvisibleItemCount ,10); argcount++;
          XtSetValues (widget_array[ K_ITERATION_EQUATION_LIST ], args,
                       argcount);

       /* Libera la memoria utilizzata */
          free_array_XmString( elenco_item, numero_item );
          XtFree( elenco_item );

  	  break;

/***************************************   selez_proc   ****************/
       case K_ITERATION_LIST:
/***********************************************************************/
          if (widget_array[ K_EQUATION_ITERATION_WINDOW ] == NULL )
             if (MrmFetchWidget(s_RMHierarchy, "equation_iteration_dialog",
    	  		        toplevel_widget,
			        &widget_array[K_EQUATION_ITERATION_WINDOW],
			        &dummy_class) != MrmSUCCESS )
		s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );

          XtManageChild( widget_array[K_EQUATION_ITERATION_WINDOW] );

	  XtSetKeyboardFocus (widget_array[K_EQUATION_ITERATION_WINDOW],
			      widget_array[K_EQUATION_ITERATION_LIST]);

          if (widget_array[ K_VARIABLE_EQUATION_WINDOW ] != NULL )
              XtUnmanageChild( widget_array[K_VARIABLE_EQUATION_WINDOW] );

       /* Leggi il numero dell'iterazione dall'item selezionato ( sono le */
       /* prime quattro cifre ). Serve soltanto per la composizione del */
       /* titolo della finestra. */
          sscanf(item,"%4d",&iterazione);

       /* Questa variabile viene settata per recuperare */
       /* il numero dell'iterazione  nella visualizzazione */
       /* delle variabili del blocco relativo all'equazione */
          iter_scelta = pos_item;

       /* Passaggio del titolo alla finestra dei residui  */
	  sprintf(stringa,
		  "UNSATISFIED RESIDUALS LIST AT ITERATION # %d",iterazione);
          set_something( widget_array[K_EQUATION_ITERATION_WINDOW],
	   		 XmNdialogTitle,(void*) CREATE_CSTRING(stringa)); 
        
      /* Lettura dei dati del calcolo dello stazionario dal file F11.DAT */
          lseek(fd_f11, offset_ris_f11, SEEK_SET);

          numero_item = 0;

     /* Dato l'indice di posizione della scrolled-list in cui e' avvenuta */
     /* la selezione, riesco a sapere quante volte devo leggere il record */
     /* irc11=C_RESIDUI nel file f11.dat per arrivare a quello corretto. */
     /* Viene effettuato un controllo sulla fine del file , anche se non */
     /* necessario. */
          flag = FALSE;

          for(i=0; i<=pos_item && !flag; )
          {
             flag = read_data_f11( fd_f11 );
             if (irc11 == C_RESIDUI) 
                i++;
          }

          if ( flag )
             return;

          residui_eqz = (float *) XtCalloc(sizeof(float),(unsigned) neqsis);
          n_equazione = (int *) XtCalloc(sizeof(int) ,(unsigned) neqsis);
          if ( residui_eqz == NULL || n_equazione == NULL )
		s_error( APPLICATION_NAME, error_mesg, EMEMALLOC, 1 );

     /* Lettura dei residui delle equazioni non convergenti a quella */
     /* iterazione selezionata. */
          for( i=0 ; i<neqsis ; i++ )
             if ( iconv[i] )
             {
                n_equazione[ numero_item ] = i;
                residui_eqz[ numero_item ] = rn[i];
                numero_item++;
             }

     /* Allocazione della memoria */
          alloca_memoria_x_items(numero_item);

     /* Ordina i residui e aggiorna l'array di compound string */
          for( i=0; i<numero_item; i++ )
          {
            int    eqz, j;
            char str1[20],str2[20];


            for( j=i+1 ; j<numero_item ; j++ )
               if ( fabs(residui_eqz[i]) < fabs(residui_eqz[j]) )
               {
                  double res;

                  res = residui_eqz[ i ];
                  eqz = n_equazione[ i ];
                  residui_eqz[ i ] = residui_eqz[ j ];
                  n_equazione[ i ] = n_equazione[ j ];
                  residui_eqz[ j ] = res;
                  n_equazione[ j ] = eqz;
               }
            eqz = n_equazione[i];
            sprintf(stringa, SF_EQUATION_ITERATION_LIST,
                    eqz+1,noblc[ iresbl[eqz]-1 ],
                    nzero(str1,residui_eqz[i],"% 8.4e"," 0."),
                    nzero(str2,residui_eqz[i]*cosnor[eqz],"% 8.4e"," "),
                    uniteq[eqz]);
            elenco_item[ i ] = CREATE_CSTRING(stringa);
          }

       /* Visualizzazione dei dati nella window di diagnostica */
          argcount = 0;
          XtSetArg(args[argcount], XmNitemCount, numero_item); argcount++; 
          XtSetArg(args[argcount], XmNitems, elenco_item ); argcount++;
          XtSetArg(args[argcount], XmNvisibleItemCount ,10); argcount++;
          XtSetValues (widget_array[ K_EQUATION_ITERATION_LIST ], args,
                       argcount);

          free_array_XmString( elenco_item, numero_item ); 
          XtFree(elenco_item);
          XtFree( residui_eqz );
          XtFree( n_equazione );
          break;

/***************************************   selez_proc   ****************/
       case K_EQUATION_ITERATION_LIST:
       case K_ITERATION_EQUATION_LIST:
/***********************************************************************/
          if (widget_array[ K_VARIABLE_EQUATION_WINDOW ] == NULL )
             if (MrmFetchWidget(s_RMHierarchy, "variable_equation_dialog",
    	  		        toplevel_widget,
			        &widget_array[K_VARIABLE_EQUATION_WINDOW],
			        &dummy_class) != MrmSUCCESS )
		s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );

          XtManageChild( widget_array[K_VARIABLE_EQUATION_WINDOW] );

	  XtSetKeyboardFocus (widget_array[K_VARIABLE_EQUATION_WINDOW],
			      widget_array[K_VARIABLE_EQUATION_LIST]);

      /* Leggi il numero dell'iterazione selezionato */
      /* e recupera il numero dell'equazione */
          if ( *widget_num == K_ITERATION_EQUATION_LIST )
          {
             iterazione = pos_item;
             equazione = eqz_scelta;
          }
          else
          {
             sscanf(item,"%4d",&equazione);
             iterazione = iter_scelta;
          }

          equazione--;

       /* Passaggio del titolo alla finestra dei valori delle variabili */
	  sprintf (stringa,"VARIABLES VALUE AT THE %s COMPUTING TIME",
	    	   ordinale(iterazione,temp));
          set_something( widget_array[K_VARIABLE_EQUATION_WINDOW],
	       		 XmNdialogTitle,(void*) CREATE_CSTRING(stringa)); 

     /* Aggiornamento delle label della window */
          copy_n_car(stringa,noblc[ iresbl[equazione]-1 ],8);
          set_something( widget_array[K_BLOCK_VALUE_VAR_EQ_LABEL],
	 		 XmNlabelString,(void*) CREATE_CSTRING(stringa));
/*
          copy_n_car(stringa,nosub[ iresbl[equazione]-1 ],4);
          set_something( widget_array[K_MODULE_VALUE_VAR_EQ_LABEL],
	 		 XmNlabelString,(void*) CREATE_CSTRING(stringa));
*/
          copy_n_car(stringa,nom_bloc[ iresbl[equazione]-1 ]+39,40);
          set_something( widget_array[K_DESCRIPT_VALUE_VAR_EQ_LABEL],
	 		 XmNlabelString,(void*) CREATE_CSTRING(stringa));

      /*  recupera il vettore dei valori delle incognite xy a quella 
          iterazione ( ed il vettore dei coefficienti dello jacobiano ... ) */
          lseek(fd_f11, offset_ris_f11, SEEK_SET);

      /* ciclo di lettura dei risultati xy */
          offset_tmp = offset_ris_f11;
          i = 0;
          while( i <= iterazione && !read_data_f11( fd_f11 ))
          {
             if ( irc11 == C_RESIDUI )
                i++;
             else
                offset_tmp = lseek( fd_f11, 0L, SEEK_CUR);
          }

       /* Lettura dello jacobiano se calcolato in questa iterazione */
          read_data_f11( fd_f11 );

       /* Se non e' stato calcolato in questa iterazione, probabilmente e' */
       /* stato calcolato precedentemente. Se il valore di irc11 attuale */
       /* e' C_PROBLEMI o C_OK allora e' stato modificato il vettore xy letto */
       /* precedentemente...quindi rileggo i valori */
          if (irc11 != C_SOLUZIONI && (irc11 == C_PROBLEMI || irc11 == C_OK ))
          {
             lseek( fd_f11, offset_tmp, SEEK_SET );
             read_data_f11( fd_f11 );
          }

          blk_selez = iresbl[ equazione ]-1;

       /* Alloca la memoria */
          alloca_memoria_x_items(ip[blk_selez+1] - ip[blk_selez]);
             
          numero_item = 0;

          for( i=ip[ blk_selez ]-1 ; i < ip[blk_selez+1]-1 ; i++ )
          {
             float valore, valore_jac;
             char  noto;
             char str1[20],str2[20],descr_var[101];

          /* Copia il valore della variabile */
             if ( ipvrs_f11[i] > 0 )
             {
                noto = ' '; 
                valore = xy[ ipvrs_f11[i]-1 ] * cnxyu[i];

       /* Recupera il valore dello jacobiano per quella equazione/incognita */
                valore_jac = valore_jacobiano( equazione+1, ipvrs_f11[i]);
             }
             else
             {
                noto = '*'; 
                valore = uu[ -ipvrs_f11[i]-1 ] * cnxyu[i];  
                valore_jac = 0.0;
             }

             if ( ipvrs[i] > 0 )
                copy_n_car(descr_var,nom_sivar[ ipvrs[i]-1 ]+30,69);
             else
                copy_n_car(descr_var,nom_vari[ -ipvrs[i]-1 ]+30,69);

             sprintf(stringa, SF_VARIABLE_EQUATION_LIST,
                     noto,var[i],
                     nzero(str1,valore,"% 8.4e"," 0."),
                     nzero(str2,valore_jac,"% 8.4e"," 0."),
    	             descr_var);

             elenco_item[ numero_item++ ] = CREATE_CSTRING( stringa );
          }

       /* Visualizzazione dei dati nella window di diagnostica */
          argcount = 0;
          XtSetArg(args[argcount], XmNitemCount, numero_item); argcount++; 
          XtSetArg(args[argcount], XmNitems, elenco_item ); argcount++;
          XtSetArg(args[argcount], XmNvisibleItemCount ,10); argcount++;
          XtSetValues (widget_array[ K_VARIABLE_EQUATION_LIST ], args,
                       argcount);

          free_array_XmString( elenco_item, numero_item ); 
          XtFree(elenco_item);
         break;

   }
   undef_cursore (widget_array[ *widget_num ]);
   XtFree( item );
}

/*-----------------------------------------------------------------------*/
/*
 *** show_results()
 *** parametri:
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *param : 
 ***   unsigned long *reason : non rilevante
 Procedura di callback */
void show_results(w, param, reason)
Widget w;
int *param;
unsigned long *reason;
{
   char temp[100], temp2[140];
   Cardinal argcount;
   Arg args[10];
   int i ,j;
   char noto; /*  con un '*' identifica le variabili note   */

/* libera la memoria per i risultati */
   if ( lista_dati != NULL )
   {
      free_array_XmString( lista_dati, numero_dati ); 
      XtFree( lista_dati );
   }

   numero_dati = 0;
   data_pos = 0;

   def_cursore (widget_array[K_STEADY_STATE_RESULTS_DIALOG], OROLOGIO);
   switch ( *param ) {

       case DISPLAY_VARIABLE	: 
       	    set_something(widget_array[K_DISPLAY_RESULTS_LABEL],
  		 	       XmNlabelString, (void*) DISPLAY_VARIABLE_STRING);

         /* Ricerca il blocco nel vettore noblc */
            for(i=0 ; 
#ifdef LINUX
                i<nbl && strncmp(blocco_selezionato,noblc[i],8) ; 
#else
                i<nbl && strncmp(blocco_selezionato,noblc[i],8) ; 
#endif
                i++ );

         /* Se il blocco non e' stato trovato e non e' stato selezionato */
         /* ALL MODEL BLOCKS ... */
            if ( i >= nbl && strncmp(blocco_selezionato,"********",8))
                break;
 
         /* leggi il vettore xy dei risultati finali del calcolo */
            lseek(fd_f11, offset_ris_f11, SEEK_SET);
            while(!read_data_f11(fd_f11) && irc11 != C_OK && irc11 != C_NON_OK);
              
         /* Se e' stato selezionato ALL MODEL BLOCKS */
            if ( !strncmp(blocco_selezionato,"********",8) )
            {
                double valore_ini, valore_fin;
                char str1[20],str2[20];

        /* Alloca la memoria necessaria. */
                if ( (lista_dati = (XmString *) 
                              XtCalloc(sizeof(XmString),neqsis+nu+5)) == NULL )
			s_error( APPLICATION_NAME, error_mesg, EMEMALLOC, 1 );

        /* Inserisci le variabili algebriche e di stato */
                lista_dati[ numero_dati++ ] = CREATE_CSTRING(
                                           "  STATE AND ALGEBRIC VARIABLES:" );
                lista_dati[ numero_dati++ ] = CREATE_CSTRING(" ");

                for(i=0; i<neqsis; i++)
                {
                   j = ipvrt[ ips[i]-1 ]-1;

        /* Leggi i valori della variabile puntata da j (in var) */
                   valori_var_blocco( j, &noto, &valore_ini, &valore_fin);

                   sprintf(temp, SF_RESULTS_LIST,
                         noto, sivar[i],
                         nzero(str1,valore_fin,"% 8.4e"," 0."),
                         nzero(str2,valore_ini,"% 8.4e"," 0."),
			 nom_sivar[i]+30);

                   lista_dati[ numero_dati++ ] = CREATE_CSTRING( temp ); 
                }

                lista_dati[ numero_dati++ ] = CREATE_CSTRING(" ");
                lista_dati[ numero_dati++ ] = CREATE_CSTRING(
                                           "  INPUT VARIABLES:" );
                lista_dati[ numero_dati++ ] = CREATE_CSTRING(" ");

        /* Inserisci le variabili ingresso */
                for(i=0; i<nu; i++)
                {
                   char str1[20],str2[20];

                   j = ipvri[ ipi[i]-1 ]-1;

        /* Leggi i valori della variabile puntata da j (in var) */
                   valori_var_blocco( j, &noto, &valore_ini, &valore_fin);

                   sprintf(temp, SF_RESULTS_LIST,
			 noto, vari[i],
                         nzero(str1,valore_fin,"% 8.4e"," 0."),
                         nzero(str2,valore_ini,"% 8.4e"," 0."),
			 nom_vari[i]+30);

                   lista_dati[ numero_dati++ ] = CREATE_CSTRING( temp ); 
                }
                break;
            }

        /* Caso in cui si devono visualizzare i risultati delle variabili */
        /* di un blocco selezionato */

        /* Alloca la memoria necessaria. */
            if ( (lista_dati = (XmString *) 
                            XtCalloc(sizeof(XmString),ip[i+1]-ip[i])) == NULL )
			s_error( APPLICATION_NAME, error_mesg, EMEMALLOC, 1 );

            for( j=ip[i]-1 ; j < ip[i+1]-1 ; j++ )
            {
               char str1[20],str2[20],descr_var[101];
               double valore_ini, valore_fin;

        /* Leggi i valori della variabile puntata da j (in var) */
               valori_var_blocco( j, &noto, &valore_ini, &valore_fin);

             /* Copia la descrizione della variabile */
               if ( ipvrs[j] > 0 )
                  copy_n_car(descr_var,nom_sivar[ ipvrs[j]-1 ]+30,69);
               else
                  copy_n_car(descr_var,nom_vari[ -ipvrs[j]-1 ]+30,69);

               sprintf(temp, SF_RESULTS_LIST,
			 noto,var[j],
                         nzero(str1,valore_fin,"% 8.4e"," 0."),
                         nzero(str2,valore_ini,"% 8.4e"," 0."),
			 descr_var);

               lista_dati[ numero_dati++ ] = CREATE_CSTRING( temp ); 
            }
	    break;

       case DISPLAY_DATA        :
            set_something(widget_array[K_DISPLAY_RESULTS_LABEL],
			  XmNlabelString, (void*) DISPLAY_DATA_STRING);
            if ( Empty(blocco_selezionato) )
                 break;

        /* Alloca la memoria necessaria. */
            if ( (lista_dati = (XmString *) 
                         XtCalloc(sizeof(XmString),num_linee_datiF14)) == NULL )
			s_error( APPLICATION_NAME, error_mesg, EMEMALLOC, 1 );

        /* Apri il file temporaneo f14.tmp */
            if ( (fp_f14 = fopen(FILE_F14_TMP, "r")) == NULL )
            {
               undef_cursore (widget_array[K_STEADY_STATE_RESULTS_DIALOG]);
               return;
            }

        /*  Visualizzazione dati geometrici e fisici...  */
            fseek( fp_f14, offset_dati_geometrici, SEEK_SET );

            strcpy(temp, HEADER_BLOCCO_F14);
            strcat(temp, blocco_selezionato);

        /* Ricerca dati geometrici del blocco (Se non e' stato selezionato */
        /* ALL MODEL BLOCKS) */
           if ( strncmp(blocco_selezionato,"********",8) )
           {
              while( fgets(temp2, 140,fp_f14) != NULL &&
                     strncmp(temp,temp2,strlen(temp)));

           /* Se il blocco non e' stato trovato ... */
              if ( strncmp(temp,temp2,strlen(temp)))
                   break;
           }

           while( fgets(temp2, 140, fp_f14) != NULL &&
                  (strncmp(temp2,HEADER_BLOCCO_F14,strlen(HEADER_BLOCCO_F14))||
                   !strncmp(blocco_selezionato,"********",8)) &&
                  strncmp(temp2,EOF_F14,strlen(EOF_F14)))
           {
               copy_n_car(temp,temp2,76);
               lista_dati[numero_dati++] = CREATE_CSTRING(temp);
           }
           fclose(fp_f14);
	   break;
   }

   toggle_choice_results = *param;

/* Inserisci i dati nella scrolled-list dei risultati */
   argcount = 0;
   XtSetArg(args[argcount], XmNitemCount, numero_dati); argcount++; 
   XtSetArg(args[argcount], XmNitems, lista_dati ); argcount++;
   XtSetArg(args[argcount], XmNvisibleItemCount ,12); argcount++;
   XtSetValues (widget_array[K_RESULTS_LIST], args, argcount);

   undef_cursore (widget_array[K_STEADY_STATE_RESULTS_DIALOG]);
} 

/*-----------------------------------------------------------------------*/
/* ok_proc(w, widget_num, selez)
 *** parametri:
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *widget_num : 
 ***   unsigned long *reason : non rilevante
 Procedura di callback utilizzate dalle dialog box in relazione al
 push button 'OK' */
void ok_proc(w, widget_num, selez)
    Widget w;
    int *widget_num;
    XmAnyCallbackStruct *selez;
{
   char *temp;

   switch(*widget_num) {

        case K_TOLERANCE_DIALOG:
	      temp = XmTextGetString(widget_array[K_TOLERANCE_DIALOG_TEXT]);
              if ( IsNumeric(temp) && !Empty(temp) )
              {
		 modify_options(atof(temp), computing_mode);
	      }
              XtFree(temp);
              XtUnmanageChild (widget_array[K_TOLERANCE_DIALOG]);
           break;
   }
}

/*-----------------------------------------------------------------------*/
/*** cancel_proc(w, widget_num, selez)
 *** parametri:
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *widget_num : 
 ***   unsigned long *reason : non rilevante
 Procedura di callback utilizzate dalle dialog box in relazione al
 push button 'CANCEL' */
void cancel_proc(w, widget_num, selez)
    Widget w;
    int *widget_num;
    XmAnyCallbackStruct *selez;
{
   int i;

   switch(*widget_num) {
        case K_TOLERANCE_DIALOG:
              XtUnmanageChild (widget_array[*widget_num]);
              break;

        case K_STEADY_STATE_RESULTS_DIALOG:
              block_pos = 0;
              data_pos = 0;
	      toggle_choice_results = DISPLAY_VARIABLE;
              XtDestroyWidget(widget_array[*widget_num]); 
              widget_array[*widget_num] = NULL; 

           /* Libera la memoria occupata */
              free_array_XmString(lista_blocchi, numero_blocchi);

              XtFree(lista_blocchi);

              if ( lista_dati != NULL )
              {
                 free_array_XmString( lista_dati, numero_dati ); 
                 XtFree( lista_dati );
              }

              lista_blocchi = NULL;
              lista_dati = NULL;
	      numero_dati = 0;
	      numero_blocchi = 0;
              break;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
        case K_EQUATION_WINDOW:
              XtUnmanageChild (widget_array[*widget_num]);
              if (widget_array[K_ITERATION_EQUATION_WINDOW] != NULL) 
                 XtUnmanageChild (widget_array[K_ITERATION_EQUATION_WINDOW]);
              if (widget_array[K_VARIABLE_EQUATION_WINDOW] != NULL)
                 XtUnmanageChild (widget_array[K_VARIABLE_EQUATION_WINDOW]);
              break;

	case K_ITERATION_EQUATION_WINDOW:
              XtUnmanageChild (widget_array[*widget_num]);
              if (widget_array[K_VARIABLE_EQUATION_WINDOW] != NULL)
                 XtUnmanageChild (widget_array[K_VARIABLE_EQUATION_WINDOW]);
              break;
  	      
	case K_VARIABLE_EQUATION_WINDOW:
              XtUnmanageChild (widget_array[*widget_num]);
              break;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
        case K_ITERATION_WINDOW:
              XtUnmanageChild (widget_array[*widget_num]);
              if (widget_array[K_EQUATION_ITERATION_WINDOW] != NULL) 
                 XtUnmanageChild (widget_array[K_EQUATION_ITERATION_WINDOW]);
              if (widget_array[K_VARIABLE_EQUATION_WINDOW] != NULL)
                 XtUnmanageChild (widget_array[K_VARIABLE_EQUATION_WINDOW]);
              break;

	case K_EQUATION_ITERATION_WINDOW:
              XtUnmanageChild (widget_array[*widget_num]);
              if (widget_array[K_VARIABLE_EQUATION_WINDOW] != NULL)
                 XtUnmanageChild (widget_array[K_VARIABLE_EQUATION_WINDOW]);
              break;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
        case K_JACOBIAN_WINDOW:
              if (widget_array[K_JACOBIAN_BLOCKS_WINDOW] != NULL) 
                 XtUnmanageChild (widget_array[K_JACOBIAN_BLOCKS_WINDOW]);
              if (widget_array[K_JACOBIAN_VARIABLES_WINDOW] != NULL) 
                 XtUnmanageChild (widget_array[K_JACOBIAN_VARIABLES_WINDOW]);
              if (widget_array[K_JAC_SING_ROW_WINDOW] != NULL) 
                 XtUnmanageChild (widget_array[K_JAC_SING_ROW_WINDOW]);
              if (widget_array[K_JAC_SING_COL_WINDOW] != NULL) 
                 XtUnmanageChild (widget_array[K_JAC_SING_COL_WINDOW]);

              XtDestroyWidget(widget_array[*widget_num]); 

          /* Libera la memoria allocata per la matrice */

              XtFree( int_colonna_jac );
              XtFree( int_riga_jac );
              XtFree( riga_jac );
              widget_array[*widget_num] = NULL; 
              break;

        case K_JACOBIAN_BLOCKS_WINDOW:
              XtUnmanageChild (widget_array[*widget_num]);
              break;

        case K_JACOBIAN_VARIABLES_WINDOW:
              XtUnmanageChild (widget_array[*widget_num]);
              break;

        case K_JAC_SING_ROW_WINDOW:
              XtUnmanageChild (widget_array[*widget_num]);
              break;

        case K_JAC_SING_COL_WINDOW:
              XtUnmanageChild (widget_array[*widget_num]);
              break;
   }
}

void chiudi_prog_legocad(signal,code,scp)
int signal,code;
struct sigcontext *scp;
{
printf("\n terminato NN\n");
}

/*-----------------------------------------------------------------------*/
/*** activate_proc(w, widget_num, selez)
 *** parametri:
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *widget_num : 
 ***   unsigned long *reason : non rilevante
 Procedura di callback utilizzate all'attivazione dei push-button presenti
 nella menu-bar */
void activate_proc(w, widget_num, selez)
    Widget w;
    int *widget_num;
    XmAnyCallbackStruct *selez;
{
   FILE *fp;
   int pid_nn;
   char temp[80];
   short dummy;
   Widget filter_text,output_text,help_widget;
   Arg arg[2];
   Cardinal narg;
   char filter_string[256];
   XmString cstring;


   switch(*widget_num) {

        case K_EXIT_PROGRAM:
                if (stato_calcolo == STAZIONARIO_SODDISFATTO)
                {
/*
                   exit_widget = conferma(toplevel_widget, 
                                          "SAVE RESULTS IN F14.DAT ?",
                                          funzioni_uscita);
*/
/* creo una file selection box con okcb = salva_as e helpcb = non_salvare */
/*create_fileSelectionBox(okcb,cancelcb,filtercb,helpcb)*/
           if( exit_widget == NULL)
             exit_widget = create_fileSelectionBox(salva_as,cancel_save,NULL,non_salvare);

/* setto il titolo della file selection box */
             cstring = XmStringCreateSimple("Save Result On Selected File");
             narg=0;
             XtSetArg(arg[narg],XmNdialogTitle,cstring);narg++;
             XtSetValues(exit_widget,arg,narg); 

/* setto l'Help button con Quit (esce senza dal calcolo staz. senza salvare i risultati )  */
             cstring = XmStringCreateSimple("Quit");
             narg=0;
             XtSetArg(arg[narg],XmNhelpLabelString,cstring);narg++;
             XtSetValues(exit_widget,arg,narg); 

/* recupero l'id widget del textfield di Filtro e lo setto come 'f14.*' */
             filter_text = XmFileSelectionBoxGetChild(exit_widget,XmDIALOG_FILTER_TEXT);
             strcpy(filter_string,XmTextGetString(filter_text));
             printf("stringa di filtro di default %s\n",filter_string);
             strcpy( &(filter_string[strlen(filter_string)-1]) , "f14.*");
             printf("setto la stringa di filtro a %s\n",filter_string);
             XmTextSetString(filter_text,filter_string);

/* aggiorno la lista dei file trovati eseguendo la Search Procedure */
             cstring = XmStringCreateSimple(filter_string);
             XmFileSelectionDoSearch(exit_widget,cstring);

/* per default il file di output viene settato come f14.dat */
             output_text = XmFileSelectionBoxGetChild(exit_widget,XmDIALOG_TEXT);
             strcpy( &(filter_string[strlen(filter_string)-1]) , "dat");
             XmTextSetString(output_text,filter_string);

                }
                else
                   msgbox_button (w, ESCI_SENZA_SALVARE, NULL);
/*
                def_cursore (exit_widget, TESCHIO);
*/
               break;

        case K_DIRECT_MODE : modify_options(tolerance,DIRECT_MODE);
			    break;

        case K_INTERACTIVE_MODE : modify_options(tolerance,INTERACTIVE_MODE);
			    break;

        case K_SAVE_CURRENT_SETTING:
		if ( (fp = fopen(FILE_USER_SETTING,"w")) != NULL)
		{
		   sprintf(temp,"tolerance %.8lf\n",tolerance);
		   fputs(temp,fp);
		   fputs("# 0 direct mode, 1 interactive mode\n",fp);
		   sprintf(temp,"computing_mode %d\n",computing_mode);
		   fputs(temp,fp);
		   fclose(fp);
		}
		break;

	case K_SET_DEFAULT_SETTING:
		modify_options(DEFAULT_TOLERANCE_VALUE,DEFAULT_MODE);
		break;

        case K_LOAD_PREVIOUS_DATA:
                if (!copy_file(FILE_F14,FILE_F14_TMP))
                   s_information( IPRVDATALOAD );
                break;

        case K_LOAD_LAST_DATA:
                if (!copy_file(FILE_F24,FILE_F14_TMP))
                   s_information( ILASTDATALOAD );
                break;

	case K_SYSTEM_DATA_MODIFY:
/*	        editf14_widget = editor (toplevel_widget,&testo_f14,
		          FILE_F14_TMP,TRUE,font_edit,funzioni_f14,geom_editor); */
       		break;

	case K_SYSTEM_RESULTS:
	        printf24_widget= conferma (toplevel_widget,
		       "Send file F24.DAT to print ?",funzioni_f24);
       		break;

        case K_COMPUTING_REPORTS:
		editlg3_widget = editor (toplevel_widget,&testo_lg3,
		              FILE_LG3,TRUE,font_edit,funzioni_lg3,geom_editor);
       		break;
        case K_UNKNOWN_VAR_SELECTION:
                pid_nn = attiva_prog_par("nn",".",(char *)0);
                break;
   }
}

/*-----------------------------------------------------------------------*/
/*   start_proc(w, widget_num, reason)
 *** parametri:
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *widget_num : 
 ***   unsigned long *reason : non rilevante
 Procedura di callback utilizzata all'avviamento del calcolo  */
void start_proc(w, widget_num, reason)
    Widget w;
    int *widget_num;
    XmAnyCallbackStruct *reason;
{
   char ctolle[20],c;

   if (computing_mode != DIRECT_MODE)
      return;

   switch(*widget_num) {
        case K_START:

        /* Apri il canale di comunicazione */
             fd_pipe = open("to_calcstaz",O_RDONLY | O_NDELAY);
             if ( fd_pipe == -1 )
             {
                s_information( IERROPENPIPE );
                return;
             }
	     system("rm -f proc/ok_staz");
             stato_calcolo = STAZIONARIO_IN_CORSO;

  /* Disabilita l'uso dei button stat e step */
             cambia_stato_menu( numero_voci_menu, voci_diretta );
             cambia_stato_menu( numero_voci_button, button_senza_modello );

  /* Chiudi il file di dati F11.DAT se precedentemente aperto */
             if ( fd_f11 > -1 )
                  close( fd_f11 );

  /* Cancella il file F11.DAT se esiste */
             if ( !access(FILE_F11, F_OK) )
                  unlink(FILE_F11);

             s_information( ICALCULATING );
  /* Prova di "collegamento" con il fortran del calcolo */

        /* Svuota la pipe (se c'e' qualcosa ... ) */
             while( read(fd_pipe,&c,1) > 0 );

        /* abilita la ricezione del segnale di terminazione del child */
             processo_terminato = FALSE;
             signal(SIGCLD, sig_child);

        /* Crea il processo lg3 */
             sprintf(ctolle, "%.8lf", tolerance);
             lg3_pid = fork();
             if ( lg3_pid == 0 )
             {
                FILE *fp_out;

        /* Redirezione dello standard output sul file FILE_LG3 */
        /* (Solo se l'apertura del file FILE_LG3 ha avuto successo!) */
#if defined LINUX
                if ((fp_out = freopen(FILE_LG3,"w", stdout)) == NULL)
                   {
                    printf ("Errore in freopen! \n");
                   }
#else
                fclose(&_iob[1]);
                if ((fp_out = fopen(FILE_LG3,"w")) != NULL)
                   _iob[1] = *fp_out;
#endif
 
        /* Esecuzione del processo lg3 */
                execl("proc/lg3","lg3",ctolle,(char *)0);
                exit(1);
             }
        /* Timer per la lettura dei messaggi. Ogni 1/2 secondo viene */
        /* attivata la procedura di callback che si preoccupa di vedere */
        /* se c'e' qualche messaggio in coda */
             timer = XtAppAddTimeOut(app_context, 500, mesg_from_lg3, NULL);
   }
}

/*-----------------------------------------------------------------------*/
/*****  find_proc     
***     attivata se si preme '<RETURN>' or 'Next'(=default button) 
***	opp. 'Previous' 
****/
void find_proc(w, widget_num, selez)
Widget w;
int *widget_num;
XmAnyCallbackStruct *selez;
{
   char *temp;
   int  n_item,i;
   Widget default_button;
       
   switch(*widget_num) {

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
    I primi 2 case servono ad assegnare al BulletinBoardDialog
    il defaultButton corretto quando si preme <RETURN> nel find (chiaro, no?) 
*/
      case K_FIND_BLOCK:
       default_button = widget_array[K_FIND_NEXT_BLOCK];
       set_something (widget_array[K_STEADY_STATE_RESULTS_DIALOG],
  		      XmNdefaultButton,(void*) default_button);
     break;

     case K_FIND_DATA:
       default_button = widget_array[K_FIND_NEXT_DATA];
       set_something (widget_array[K_STEADY_STATE_RESULTS_DIALOG],
  		      XmNdefaultButton,(void*) default_button);
     break;
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

        case K_FIND_NEXT_BLOCK:
              temp = XmTextGetString(widget_array[K_FIND_BLOCK]);
              cerca_stringa (temp,lista_blocchi,numero_blocchi,
                             &block_pos,FIND_NEXT,K_BLOCK_FOUND);
              UtListaScroll (widget_array[ K_BLOCK_LIST_RESULTS ],
                             numero_blocchi,12,block_pos);
              XtFree (temp);
          break;

        case K_FIND_PREVIOUS_BLOCK:
              temp = XmTextGetString(widget_array[K_FIND_BLOCK]);
              cerca_stringa (temp,lista_blocchi,numero_blocchi,
                             &block_pos,FIND_PREVIOUS,K_BLOCK_FOUND);
              UtListaScroll (widget_array[ K_BLOCK_LIST_RESULTS ],
                             numero_blocchi,12,block_pos);
              XtFree (temp);
          break;


        case K_FIND_NEXT_DATA:
              temp = XmTextGetString(widget_array[K_FIND_DATA]);
              cerca_stringa (temp,lista_dati,numero_dati,
                             &data_pos,FIND_NEXT,K_DATA_FOUND);
              UtListaScroll (widget_array[ K_RESULTS_LIST ],
                             numero_dati,12,data_pos);
              XtFree (temp);
          break;

        case K_FIND_PREVIOUS_DATA:
              temp = XmTextGetString(widget_array[K_FIND_DATA]);
              cerca_stringa (temp,lista_dati,numero_dati,
                             &data_pos,FIND_PREVIOUS,K_DATA_FOUND);
              UtListaScroll (widget_array[ K_RESULTS_LIST ],
                             numero_dati,12,data_pos);
              XtFree (temp);
          break;
   }
}

/*-----------------------------------------------------------------------*/
/* FUNZIONI DI UTILITA'							 */
/*-----------------------------------------------------------------------*/
/*** cerca_stringa( stringa, list, num_elem, pos, flag, label_ID )
 *** parametri:
 ***         char *stringa
 ***         XmString list[]    : array di compound string
 ***                              (ITEM di una ScrolledList)
 ***         int   num_elem     : numero totale di item
 ***         int  *pos  : posizione della stringa nell'array
 ***         int   flag : commutatore per la ricerca NEXT <> PREVIOUS
 ***         int   label_ID   : ID della  Label che riceve messaggi
funzione utilizzata per ricercare una stringa all'interno di una list   */
int cerca_stringa( stringa, list, num_elem, pos, flag, label_ID )
char *stringa;
XmString list[];
int   num_elem;
int  *pos;
int   flag;
int   label_ID;
{
    int i;
    XmString messaggio[2];

    if ( !num_elem )
       return;

    messaggio[0] = CREATE_CSTRING ("");
    messaggio[1] = CREATE_CSTRING ("<NO  PATTERN  MATCHED>");
    switch(flag)
    {
    case FIND_NEXT:
         i = *pos; 
         do
         {
            if (i < num_elem-1)
               i++;
            else
            {
               set_something(widget_array[label_ID],XmNlabelString,
                             (void*) messaggio[1]);
               break;
            }

            if(Utstrstr(extract_string(list[i]),stringa)!=NULL)
            {
               *pos = i;
               set_something(widget_array[label_ID],XmNlabelString,
                             (void*) messaggio[0]);
               break;
            }
         }
         while (i != *pos);

      break;

    case FIND_PREVIOUS:
         i = *pos;
         do
         {
            if( i > 0 )
              i--;
            else
            {
               set_something(widget_array[label_ID],XmNlabelString,
                             (void*) messaggio[1]);
               break;
            }

            if(Utstrstr(extract_string(list[i]),stringa)!=NULL)
            {
               *pos = i;
               set_something(widget_array[label_ID],XmNlabelString,
                             (void*) messaggio[0]);
               break;
            }
         }
         while(i != *pos);
      break;
    }
}

/*-----------------------------------------------------------------------*/
/*** leggi_dati_geometrici()
funzione che apre il file f14.dat in lettura e setta l'offset relativo
al primo blocco nel file.
Ritorna 0 se tutto ok, altrimenti ritorna -1 */
leggi_dati_geometrici()
{
   char temp[140];

   if ( (fp_f14 = fopen( FILE_F14, "r")) == NULL)
	s_error( APPLICATION_NAME, error_mesg, EOPENF14, 1 );

/* Cerca il primo blocco di dati geometrici e fisici */
   while(fgets(temp,140,fp_f14) != NULL &&
         strncmp(temp,HEADER_BLOCCO_F14,strlen(HEADER_BLOCCO_F14)) );

   if ( !strncmp(temp,HEADER_BLOCCO_F14,strlen(HEADER_BLOCCO_F14)) )
   {
      offset_dati_geometrici = ftell(fp_f14)-strlen(temp);

 /* Conta il numero di linee dei dati geometrici e fisici. (serve,successiva= */
 /* mente per sapere quanta memoria allocare ). */
      for( num_linee_datiF14=1; fgets(temp,140,fp_f14) != NULL && 
                strncmp(temp,EOF_F14,strlen(EOF_F14)); num_linee_datiF14++ );
   }
   else
      offset_dati_geometrici = -1;

   fclose(fp_f14);

   if ( offset_dati_geometrici == -1)
      return(-1);

   return(0);
}

/*-----------------------------------------------------------------------*/
/*** mesg_from_lg3()
funzione che legge i messaggi dalla pipe che collega il fortran ( calcolo 
dello stazionario ) con il 'C'. */
void mesg_from_lg3(client_data, id)
char *client_data;
XtIntervalId *id;
{
   int lg3_terminato = FALSE;

/* lettura del messaggio */
   if (recv_message( fd_pipe, messaggio, &hmsg ) == NOT_OK)
   {
  /* Se non ci sono messaggi in coda allora controlla che il processo */
  /* LG3 non sia terminato. Se e' terminato allora informa l'utente che */
  /* tale processo non ha completato i calcoli correttamente */
      if ( processo_terminato )
      {
         char *mesg;
/*
         mesg="Process  LG3  aborted!\n \
               It's possible to examine partial results.";

         warning_widget = attention(toplevel_widget,mesg,MAPPA,geom_attention);
*/
         s_information( ICALCNOTEND );
         conferma(toplevel_widget, 
           "Process  LG3  aborted!\n \
	    It's possible to examine partial results.\n \
            View LG3 error LOGGING (lg3.out)?",
                    funzioni_err);
         lg3_terminato = TRUE;

      }
      else
      {
         timer = XtAppAddTimeOut(app_context, 500, mesg_from_lg3, NULL);
         return;
      }
   }

   switch (hmsg.codice) {
        case 0:
            mesg_status_window(messaggio);
           break;

        case 1:
            stato_calcolo = STAZIONARIO_SODDISFATTO;
            warning_widget = attention(toplevel_widget, 
   	      "OK.  Convergence Criterion Satisfied.", MAPPA, geom_attention );
            s_information( ICALCOK );
	    system("touch proc/ok_staz");
           break;

        case 2:
            stato_calcolo = STAZIONARIO_NON_SODDISFATTO;
/*
            warning_widget = attention(toplevel_widget, 
			     "Convergence Problem ... ", MAPPA, geom_attention);
*/
            s_information( ICALCNOTOK );
            conferma(toplevel_widget, 
                    "Convergence Problem !\nView LG3 error LOGGING (lg3.out)?",
                    funzioni_err);
           break;
   }

/* Se il calcolo e' andato a termine oppure il processo lg3 e' terminato */
/* allora apri il file dei risultati */
/* F11.DAT e cambia i push button della menubar */
   if ( hmsg.codice == 1 || hmsg.codice == 2 || lg3_terminato )
   {
      /* Apri il nuovo file di dati F11.DAT e leggi l'header */
            fd_f11 = open( FILE_F11, O_RDONLY );
            if ( fd_f11 != -1 )
            {
               offset_ris_f11 = read_header_f11( fd_f11, FALSE );

         /* Conta quante volte e' stato effettuato il calcolo dei residui */
         /* ( e' utile,poi, per il dimensionamneto di qualche vettore ). */
               for( num_calcolo_residui = 0; 
                    !read_data_f11( fd_f11 ) && irc11 != C_NON_OK
					     && irc11 != C_OK;
                    num_calcolo_residui++);

               lseek(fd_f11, offset_ris_f11, SEEK_SET);
               cambia_stato_menu( numero_voci_menu, voci_terminato );
            }
            else
            {
               s_information( IERROPENF11 );
               cambia_stato_menu( numero_voci_menu, voci_non_attivato );
            }

            if (computing_mode == DIRECT_MODE)
               cambia_stato_menu( numero_voci_button, button_direct );
            else
               cambia_stato_menu( numero_voci_button, button_interactive );

        /* Chiudi il canale di comunicazione */
            close( fd_pipe );
   }
   else /* Altrimenti abilita la ricezione di altri messaggi ... */
      timer = XtAppAddTimeOut(app_context, 500, mesg_from_lg3, NULL);
}
/*-----------------------------------------------------------------------*/

/*** void sig_child()
funzione di segnalazione di terminazione del processo child.
Viene chiamata dal signal SIGCLD */
void sig_child(sig, code, scp, addr)
int sig, code;
struct sigcontext *scp;
char *addr;
{
    int status;

    wait(&status);
    processo_terminato = TRUE;
}


/*-----------------------------------------------------------------------*/
/*** Funzione s_information(codice_info)
 *** Parametri:
 ***      codice_info : codice informazione
stampa messaggio di informazione nella status window */
s_information(codice_info)
int codice_info;
{
   int i;

   for(i=0; i<info_mesg[codice_info].num_mesg ; i++)
      mesg_status_window(info_mesg[codice_info].mesg[i]);
   mesg_status_window(" ");
}

/* fine calcstaz.c */
