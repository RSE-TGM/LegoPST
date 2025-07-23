/**********************************************************************
*
*       C Source:               edicole.c
*       Subsystem:              1
*       Description:
*       %created_by:    ciccotel %
*       %date_created:  Thu Nov  7 13:36:43 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: edicole.c-4 %  (%full_filespec: edicole.c-4:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)edicole.c	1.5\t3/20/95";
/*
        Fine sezione per SCCS
*/
/* edicole.c							*/

/*			versione: 0.1			18.9.92 */

/* Buffo Massimo, Calleri Nicola */

#include <stdio.h>

#ifndef VMS

#include <unistd.h>
/* #include <sys/stat.h> */
#include <sys/file.h>
#include <sys/types.h>

#else

#include <types.h>
#include <processes.h>
#include <unixio.h>
#include <unixlib.h>

#define F_OK	0
#define R_OK	4
#define W_OK	2
#define X_OK	1

#endif

#include <stdlib.h>
#include <string.h>
#include <signal.h>
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

#include <libutilx.h>
#include <utile.h>

#include "edicole.h"
#include "edi_menu.h"
#include "file_icone.h"
#include "messaggi.h"

#include "aggiunte.h"

int funzione;

/* messaggi di errore */
char *error_mesg[] = {
			"You must set LEGOCAD_UID with correct pathname.",
			"Error while opening hierarchy.",
			"Can't open dialog box ! ",
			"Open file module list failed!",
			"Icon file list creation failed!",
			"Open icon list file failed!",
			"Open scheme list file failed!",
			"Open icon standard file failed!",
			"Open symbol file failed!",
			"Can't put finger in the nose"
                     };

/* messaggi di warning */
char *warning_mesg[] = {
			"Can't clear bitmap file",
			"Can't add a new icon for this module.",
			"Can't create a file",
			"Can't copy from inesistent file.",
			"Can't read bitmap file. Check permissions.",
			"The selected module does not exist.",
			"Can't save icon information.",
			"Can't write bitmap file."
		       };	

/********************************************/
/* Variabili utilizzate dalle funzioni XLib */
/********************************************/
Display *display;
int screen;
Arg args[20];
Cardinal argcount;

/* D E F I N I Z I O N E   D E I   C O L O R I */
char *names[] = { "white", "black", "yellow", "green", "red", "blue"};

int   num_colors = 6;
Pixel apix[50];
short color_values[] = { WHITE,BLACK,YELLOW,GREEN,RED,BLUE };
short icon_fg;
short icon_bg = 0; /* DISABILITATO -- IMPIEGATO SOLO PER COMPATIBILITA' */
		   /* CON LE PROCEDURE DI LETTURA/SCRITTURA SU FILE     */
Colormap colormap;
Pixmap icon_pixmap, bmap_pixmap; 
char pixmap_creata = False;

/* NUMERO DI ICONE PER MODULO */
int tot_icone = 0;
int num_icona = 0;  /* numero d'ordine dell'icona */
int icona_corrente = -1;  /* numero d'ordine dell'icona */
int max_icone = 0;  /* numero massimo di icone */

/* VARIABILE GLOBALE DEL NOME DEL MODULO SELEZIONATO */
char nome_modulo[5];
char descr_modulo[51];

/* Flag per l'opzione dalla linea di comando */
Boolean nome_modulo_passato = False;

/* Graphics context della pixmap dell'icona */
GC gc_icona;

/******************************************************/
/* nome del file bitmap e path di tali files */
char filebitmap[100];
char only_filebmp_name[20];
char path_bmap[256];

int record_symb_size; /* lunghezza record file symbol.dat */

/******************************************************/
/* record del file icon_list.dat e altri file di dati */
IconFileRec   rec_modp;
SchemeFileRec rec_modnp;
IconStdRec    rec_std;
SymbolFileRec rec_symb;
/******************************************************/

/* tipo di icona (standard, processo, schema, simbolo) */
int tipo_icona;

XmFontList    fontList;
/* char	     *font_name = "timB14"; */

Dimension window_width, window_height;
Dimension icon_width, icon_height, default_icon_width, default_icon_height;
unsigned int bmap_width, bmap_height;
Dimension offset_label;
int  size;
int bmap_x_hot, bmap_y_hot;

/* File pointer icon_list.dat, lista_schemi.dat e icon_std.dat */
FILE *fp_modp, *fp_modnp, *fp_std, *fp_symb;
long offs_modulop, offs_scheme, offs_std;

/* Variabili per utilizzare il 'find_kit' di libreria LIBUTILX */
Find_struct  find_process, find_scheme;

/* Variabili del resource manager */
MrmHierarchy s_RMHierarchy;
MrmType *dummy_class;
/*-----------------------------------------------------------------------*/
char *filename_uid = "edicole.uid";
char name_uid[FILENAME_MAX+1];
int db_filename_num = 1;
/*-----------------------------------------------------------------------*/

/*  VARIABILI DI WIDGET  */
Widget toplevel_widget, main_window_widget;
Widget widget_array[ MAX_WIDGETS ];

/********************************************************************/
/* Strutture riguardanti il geometry management delle varie Dialog...
   Specificare, nell'ordine: { DefaultPosition TRUE/FALSE,XmNx,XmNy,
                               XmNwidth,XmNHeight }                 */
Dialog_geometry geom_attention   = { TRUE, NULL, NULL, NULL, 120};
/********************************************************************/

/* Variabili per la registrazione delle funzioni di callback */
static MrmRegisterArg reglist[] = {
       { "create_proc",   (caddr_t) create_proc } ,
       { "cancel_proc",   (caddr_t) cancel_proc } ,
       { "ok_proc",       (caddr_t) ok_proc } ,
       { "dialog_proc",   (caddr_t) dialog_proc } ,
       { "drag_proc",     (caddr_t) drag_proc } ,
       { "color_fg_proc", (caddr_t) color_fg_proc } ,
    /*   { "color_bg_proc", (caddr_t) color_bg_proc } ,  */
       { "cambio_dim",    (caddr_t) cambio_dim } ,
       { "select_icon",   (caddr_t) select_icon } ,
       { "select_modulo", (caddr_t) select_modulo } ,
       { "activate_proc", (caddr_t) activate_proc } 
};

static int reglist_num = (sizeof reglist / sizeof reglist[0]);

/* Array di compound strings */
XmString cstrings[500], cstring, cstring_null;

/********
typedef struct {
                  char *option_modulo;
                  Boolean stdlib;
               } AppData, *AppDataPtr;
********/
typedef struct {
                  char *option_modulo;
                  int stdlib;
               } AppData, *AppDataPtr;

/* Opzioni dalla linea di comando: */
/*    edicole -module <nome modulo> -stdlib */
/********
static XrmOptionDescRec options[] = {
        {"-module", "*module", XrmoptionSepArg, NULL },
        {"-stdlib", "*stdlib", XrmoptionNoArg, "True" }
       };
*********/

static XrmOptionDescRec options[] = {
        {"-module", "*module", XrmoptionSepArg, NULL },
        {"-stdlib", "*stdlib", XrmoptionSepArg, 0 }
       };

/*
static XtResource resources[] = {
      {   "module",
          "Module",
           XtRString,
           sizeof(String),
           XtOffset(AppDataPtr, option_modulo),
           XtRString,
           "" },

      {   "stdlib",
          "Stdlib",
          XtRBoolean,
          sizeof(Boolean),
          XtOffset(AppDataPtr, stdlib),
          XtRImmediate,
          False }};
*/
static XtResource resources[] = {
      {   "module",
          "Module",
           XtRString,
           sizeof(String),
           XtOffset(AppDataPtr, option_modulo),
           XtRString,
           "" },

      {   "stdlib",
          "Stdlib",
          XtRInt,
          sizeof(int),
          XtOffset(AppDataPtr, stdlib),
          XtRImmediate,
          0 }};

extern void free_array_XmString( XmString, int );


/*-----------------------------------------------------------------------*/
void main(int argc,char **argv)
{
   char *path_uid, *path_lib, *path_libgraf, *path_std, buffer[40];
   int  flag_lib;
   AppData app_data;

   cstring_null = CREATE_CSTRING(""); /* compound string nulla di comodo */

   fprintf(stdout,"initializing toolkit ...\n");

   MrmInitialize ();  /* INIZIALIZZARE Mrm PRIMA DEL TOOLKIT  */

   toplevel_widget = XtInitialize(argv[0], "Edicole", options, 
                                  XtNumber(options), &argc, argv);

   argcount = 0;
   XtSetArg(args[argcount],XmNminWidth,680); argcount++;
   XtSetArg(args[argcount],XmNminHeight,410); argcount++;
   XtSetArg(args[argcount],XmNtitle, WINDOW_TITLE); argcount++;
   XtSetArg(args[argcount],XmNiconName, APPLICATION_NAME); argcount++;
   XtSetValues (toplevel_widget, args, argcount);

   XtGetApplicationResources(toplevel_widget,&app_data, resources,
                             XtNumber(resources), NULL, 0);

printf( "app_data.stdlib %d\n",app_data.stdlib);
   if (app_data.stdlib)
     flag_lib = app_data.stdlib;
/*
     flag_lib = LIB_STANDARD;
*/
   else
     flag_lib = LIB_UTENTE;
printf("flag lib: %d\n", flag_lib);

/* Crea il file icon_list.dat (lista moduli di processo) */
/* leggendo i moduli rispettivamente dal file libut/lista_moduli.dat */
/* (cioe' se i due file non esistono.) */
   fp_modp = apri_file_icoproc(CREA_TRUE, flag_lib);
   fp_modnp = apri_file_icoscheme(flag_lib);

/* percorso dei file BITMAP */
   if (flag_lib == LIB_UTENTE)
   {
      path_lib = (char *) getenv("LEGOCAD_USER");
      path_libgraf = PATH_LIBGRAF_UTE;
   }
   else
   {
      path_lib = (char *) getenv("PROCLIB");
      if (flag_lib == LIB_STANDARD_BASE)
         path_libgraf = PATH_LIBGRAF_STDBASE;
      else if (flag_lib == LIB_STANDARD_AUXILIARY )
         path_libgraf = PATH_LIBGRAF_STDAUXILIARY;
      else if (flag_lib == LIB_STANDARD_NUCLEAR )
         path_libgraf = PATH_LIBGRAF_STDNUCLEAR;
      else if (flag_lib == LIB_STANDARD_SPECIAL )
         path_libgraf = PATH_LIBGRAF_STDSPECIAL;

      path_std = PATH_LIBGRAF_STDGENERAL;
   }

   if (path_lib == NULL)
#ifdef VMS
      path_lib = "[]";
#else
      path_lib = ".";
#endif

#ifdef VMS
   sottodir_vms(path_bmap, path_lib, path_libgraf );
#else
   sprintf(path_bmap,"%s/%s/", path_lib, path_libgraf );
#endif

/* crea il file icon_std.dat che contiene le icone standard dei moduli di */
/* di processo e non (solo se non esiste) */ 
   fp_std = apri_file_std(CREA_TRUE, flag_lib);

/* Crea il file symbol.dat (SE NON ESISTE) */
   fp_symb = apri_file_symb(&rec_symb, CREA_TRUE, flag_lib);

/* Calcolo la lunghezza di un record del file symbol.dat */
   fgets(buffer,40,fp_symb);
   record_symb_size = strlen (buffer);

   display = XtDisplay(toplevel_widget);
   screen  = DefaultScreen(display);

/* Le UnitType di tutti gli widget sono in 100TH_FONT_UNITS; */
/* una unita' di font viene posta a 100 con XmSetFontUnit	*/
   XmSetFontUnit (display,100);

   init_mrm();

/*  Rende sensibili gli opportuni menu e push button */
   cambia_stato_menu (numero_voci, vm_inizio_programma);

/*  Recupera la Colormap e allocca i colori di fg e bg  */
   get_something(main_window_widget,XmNcolormap,(void*) &colormap);
   get_pixel(names,apix,num_colors);

/*  Registra i ridimensionamenti dell'applicativo per mantenere  */
/*  la centratura dell'icona nella Scrolled Window  */
   XtAddEventHandler(widget_array[K_VIEW_WINDOW], StructureNotifyMask, False,
                     cambio_dim, NULL);

/* Recupera le dimensioni della stringa di label dell'icona */
   get_something (widget_array[K_ICON_LABEL],XmNfontList,(void*) &fontList);
   cstring = CREATE_CSTRING("****");
   size = XmStringWidth(fontList,cstring);
   XmStringFree(cstring);

/* Dopodiche' setta le dimensioni della label di pixmap, XmScale ecc. */
   icon_defaults();

   XtManageChild(main_window_widget);

   XtRealizeWidget(toplevel_widget);

   gc_icona = XCreateGC(display, XtWindow(toplevel_widget), NULL, NULL);

/********************************************************************/
/* Recupera eventualmente il nome del modulo dalla linea di comando */
/********************************************************************/

   if (strcmp(app_data.option_modulo,""))
   {
      long int offset;
      char baffer[80];
      XmString cstr;

      strcpy(nome_modulo, app_data.option_modulo);
      nome_modulo_passato = True;

      if ( (offset = cerca_modulo_proc (fp_modp,&rec_modp,nome_modulo))
            == RETURN_ERROR)
      {
          sprintf (rec_modp.nome_modulo,"%.4s",nome_modulo); 
          sprintf (baffer,"%s       0   ",rec_modp.nome_modulo);
          cstr = CREATE_CSTRING (baffer);
          modulo_selezionato(cstr,K_MODULE_P_LIST);    
      }
      else
      {
          fseek (fp_modp,offset,SEEK_SET);
          leggi_record_file_ico( fp_modp, &rec_modp );
          sprintf (baffer,"%s     %2d   %.30s",rec_modp.nome_modulo,
                   rec_modp.num_icone, rec_modp.descr_modulo);
          cstr = CREATE_CSTRING (baffer);
          modulo_selezionato(cstr,K_MODULE_P_LIST);    
      }
      XmStringFree(cstr);
   }
/********************************************************************/
 
   XtMainLoop();
} 
/* Fine main() */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Funzioni specifiche del programma					 */
/*-----------------------------------------------------------------------*/

/*** cambia_stato_menu (numero_voci, stato_programma)
 *** PARAMETRI
 *** short numero_voci: numero di voci in *stato_programma
 *** Voci_menu *stato_programma: contiene la coppia di valori:
				 - indice di widget_array[] per l'ID
				 - stato di ogni voce del menu
Attiva/disattiva gli opportuni menu & push button a seconda della
fase dell'applicativo ***/
void cambia_stato_menu (numero_voci,stato_programma)
short numero_voci;
Voci_menu *stato_programma;
{
   short i;

   for (i=0; i<numero_voci; i++)
       set_something_val (widget_array[stato_programma[i].indice], XmNsensitive, (XtArgVal) stato_programma[i].stato);
}

/*-----------------------------------------------------------------------*/
/* 	FUNZIONI DI CALLBACK... */
/*-----------------------------------------------------------------------*/

/*** create_proc()
 *** parametri:
 ***            Widget w : identificativo del widget che ha effettuato la
 ***                       callback
 ***            int *widget_num : parametro passato dalla callback
 ***                             (client data)
 ***            unsigned long *reason : non rilevante
 Procedura di callback che aggiorna l'array dei widget */
void create_proc(w, widget_num, reason)
Widget w;
int *widget_num;
unsigned long *reason;
{
   widget_array[ *widget_num ] = w;
}
/*-----------------------------------------------------------------------*/

/*** activate_proc(w, widget_num, selez)
 *** parametri:
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *widget_num : indice in widget_array.
 ***   unsigned long *reason : non rilevante
 Procedure di callback utilizzate all'attivazione dei push-button 
 presenti nella menu-bar, ad esclusione di quelli che provocano
 la comparsa di dialog windows (DIALOG_PROC()) */
void activate_proc(w, widget_num, selez)
Widget w;
int *widget_num;
XmAnyCallbackStruct *selez;
{
   FILE *fp;
   char temp[80];
   short dummy;
   register int i;
   int prec_icona;

   switch(*widget_num) {

/***************************************  activate_proc   **************/
	case K_QUIT_PROGRAM:
/***********************************************************************/

	   XFreePixmap(display, bmap_pixmap);
	   XFreePixmap(display, icon_pixmap);
           XFreeColors(display,colormap,apix, num_colors, AllPlanes);
	   XFreeGC(display, gc_icona);
	   fclose( fp_modp );
	   fclose( fp_modnp );
	   fclose( fp_std );
	   fclose( fp_symb );

           XtCloseDisplay(display);
           exit(0);

/***************************************  activate_proc   **************/
	case K_CLEAR:
/***********************************************************************/
    /*  clear del file bitmap */
	  if (crea_file_vuoto(filebitmap, bmap_width, bmap_height) == -1)
             s_warning( toplevel_widget, &geom_attention, APPLICATION_NAME,
                        warning_mesg, WCLRBMAP );
          else
             set_something_val(widget_array[K_ICON_PIXMAP] ,XmNlabelPixmap, (XtArgVal) XmUNSPECIFIED_PIXMAP);
	  break;

/***************************************  activate_proc   **************/
	case K_STD_MODULE_P:
	case K_STD_MODULE_NP:
/***********************************************************************/

/* Se si stava agendo sui simboli, vengono reimpostati i settaggi */
/* per le icone "normali" (standard, process, scheme) */
         set_something_val(widget_array[K_ICON_LABEL],XmNbackground, (XtArgVal) apix[WHITE]);
         if (tipo_icona == TP_SYMBOL)
            icon_defaults();

         if ( *widget_num == K_STD_MODULE_P )
         {
            tipo_icona = TP_STD_PROCESS;
            offs_std = leggi_file_std_ico(fp_std, &rec_std, MODULE_PROCESS);
            strcpy(nome_modulo, "STDP");
            strcpy(descr_modulo, "Standard icon for process modules");
         }
         else
         {
            tipo_icona = TP_STD_SCHEME;
            offs_std = leggi_file_std_ico(fp_std, &rec_std, MODULE_SCHEME);
            strcpy(nome_modulo, "STDNP");
            strcpy(descr_modulo, "Standard icon for scheme modules");
         }

      /* recupero i dati dell'icona */
         max_icone = 1;
         num_icona = rec_std.num_icone;
         tot_icone = num_icona;
         strcpy(filebitmap,path_bmap);
         strcat(filebitmap, rec_std.bmap_record.nome_file);
         strcpy(only_filebmp_name, rec_std.bmap_record.nome_file);
         icon_fg = rec_std.bmap_record.colore_fg;
/*          icon_bg = rec_std.bmap_record.colore_bg;   */
         icon_width = rec_std.bmap_record.base;
         icon_height = rec_std.bmap_record.altezza + offset_label;
         set_label(widget_array[K_ICON_LABEL],nome_modulo);
         set_label(widget_array[K_MODULE_NAME], nome_modulo);
         set_label(widget_array[K_MODULE_DESCRIPTION], descr_modulo);
         if ( num_icona == 0 )
         {
            set_default_config();
            set_something_val(widget_array[K_ICON_PIXMAP] ,XmNlabelPixmap, (XtArgVal) XmUNSPECIFIED_PIXMAP);
            strcpy(only_filebmp_name, "<NO ICONS>");
            pixmap_creata = False;
            cambia_stato_menu (numero_voci, vm_nessuna_icona);
         }
         else
         {
            cambia_stato_menu (numero_voci, vm_edi_icona_std);
            crea_pixmap(filebitmap);
         }
         set_icon_config();
	 break;

/***************************************  activate_proc   **************/
	case K_DELETE:  
/***********************************************************************/
    /* Cancella la pixmap presente sullo schermo e cancella anche il */
    /* FILE di BITMAP */
    /* N.B. disabilitato per i MODULI di SCHEMA  */
          if (tot_icone == 0)
             return;

          tot_icone--;

     /* cancella il file bitmap */ 
#ifndef VMS

          unlink(filebitmap);

#else

          remove(filebitmap);

#endif

	  switch (tipo_icona)
	  {
	    case TP_MODULE_PROCESS:

	      /* Setta a 0 il campo 'esiste' dell'icona corrente */
              rec_modp.bmap_record[icona_corrente].esiste = FALSE; 

              if (tot_icone == 0)
              {
                   /* visualizzare l'icona standard se necessario ... */
                   visualizza_icona_standard();
                   strcpy(only_filebmp_name, "<NO ICON>");
                   cambia_stato_menu (numero_voci, vm_nessuna_icona);
                   num_icona--;
              }
              else
	      {
                   if ( num_icona-1 == tot_icone )
                   {
                      next_icon_available( S_PREVIOUS, rec_modp.bmap_record,
					   MAX_ICON_X_PMODULE );
                      num_icona--;
                   }
                   else
                      next_icon_available( S_NEXT, rec_modp.bmap_record,
					   MAX_ICON_X_PMODULE );
	      }

       	     /* registra le modifiche nel file icon_list.dat */
              rec_modp.num_icone--;
              fseek( fp_modp, offs_modulop , SEEK_SET);
              scrivi_record_file_ico( fp_modp, &rec_modp );

            break;

            case TP_SYMBOL:

	      /* Setta a 0 il campo 'esiste' dell'icona corrente */
              rec_symb.bmap_record[icona_corrente].esiste = FALSE; 

       	      /* registra le modifiche nel file symbol.dat */
              rec_symb.num_simboli--;
              fseek(fp_symb,icona_corrente*record_symb_size,SEEK_SET); 
              scrivi_file_simboli(fp_symb, 
				  &rec_symb.bmap_record[icona_corrente]);

              if (tot_icone == 0)
              {
                  /* visualizzare l'icona standard se necessario ... */
                  visualizza_icona_standard();
                  strcpy(only_filebmp_name, "<NO ICON>");
                  cambia_stato_menu (numero_voci, vm_nessuna_icona);
                  num_icona--;
              }
              else
	      {    
		   if ( num_icona-1 == tot_icone )
                   {
                      next_icon_available( S_PREVIOUS, rec_symb.bmap_record,
					   MAX_SYMBOL );
                      num_icona--;
                   }
                   else
                      next_icon_available( S_NEXT, rec_symb.bmap_record,
               				   MAX_SYMBOL );
	      }

            break;

            case TP_STD_PROCESS:
            case TP_STD_SCHEME:
              if (tot_icone == 0)
              {
		     icon_defaults();
                  /* visualizzare l'icona standard se necessario ... */
                     visualizza_icona_standard();
                     strcpy(only_filebmp_name, "<NO ICON>");
                     cambia_stato_menu (numero_voci, vm_nessuna_icona);
                     num_icona--;
              }
              else
                   num_icona--;

   	     /* registra le informazioni nel file  icon_std.dat */
              rec_std.num_icone = 0;
              fseek(fp_std, offs_std, SEEK_SET);
              scrivi_file_std_ico(fp_std, &rec_std);

            break;
	  }  /*** FINE SWITCH INTERNO ***/

          set_icon_config();

	break;

/***************************************  activate_proc   **************/
	case K_NEW:
/***********************************************************************/
     /* controlla che il massimo numero di icone per modulo non sia */
     /* superiore a max_icone */
         if (tot_icone >= max_icone)
         {  
            s_warning( toplevel_widget, &geom_attention, APPLICATION_NAME,
                       warning_mesg, WADDICON );
            return;
         }

    /* se non c'era nessuna icona in editazione , allora */
    /* attiva gli opportuni menu e push button per la nuova icona */
         if (num_icona == 0)
	    cambia_stato_menu (numero_voci, vm_moduli_p_ico);

   /* recupera il nome dal file se si tratta di una icona dei moduli di */
   /* processo  o di un'icona di simboli  */
         if (tipo_icona == TP_MODULE_PROCESS)
         {
            cerca_file_disponibile();
            strcpy(filebitmap, path_bmap);
  	    strcat(filebitmap, rec_modp.bmap_record[icona_corrente].nome_file);
            strcpy(only_filebmp_name, 
		   rec_modp.bmap_record[icona_corrente].nome_file);
	    rec_modp.bmap_record[icona_corrente].esiste = TRUE;
            num_icona = icona_corrente+1;
         }
         else if (tipo_icona == TP_SYMBOL)
         {
            cerca_file_disponibile();
            strcpy(filebitmap, path_bmap);
  	    strcat(filebitmap, rec_symb.bmap_record[icona_corrente].nome_file);
            strcpy(only_filebmp_name, 
		   rec_symb.bmap_record[icona_corrente].nome_file);
	    rec_symb.bmap_record[icona_corrente].esiste = TRUE;
            num_icona = icona_corrente+1;
	 }
         else  /*  per gli altri casi (ICONE STANDARD & SCHEMA)  */
         {
            strcpy(only_filebmp_name, rec_std.bmap_record.nome_file);
            num_icona++;
	 }

     /* Crea il file bitmap vuoto */
         if (crea_file_vuoto(filebitmap, icon_width,
			     icon_height-offset_label) == -1)
         {
            s_warning( toplevel_widget, &geom_attention, APPLICATION_NAME,
                       warning_mesg, WCREAFILE );
            return;
         }

    /* incrementa il numero di icone del modulo e registrale nel file */
    /* icon_list.dat */
         tot_icone++;

    /* Setta la configurazione di default per la nuova icona */
         if (tipo_icona == TP_SYMBOL)
	     set_default_symbol();
         else
             set_default_config();

         set_icon_config();

    /* registrazione nel file */
         salva_icona( tipo_icona );

         set_something_val(widget_array[K_ICON_PIXMAP] ,XmNlabelPixmap, (XtArgVal) XmUNSPECIFIED_PIXMAP);
         pixmap_creata = False;
        break;

/*************************************  activate_proc   ****************/
	case K_SAVE:
/***********************************************************************/

     /* Salva nel file icon_list.dat le caratteristiche dell'icona in */
     /* editazione */
	 salva_bitmap();
         salva_icona( tipo_icona );
	 break;

/***************************************  activate_proc   **************/
	case K_SYMBOL:
/***********************************************************************/

/* Se si stava agendo sulle icone "normali" (standard, process, scheme), */
/* vengono reimpostati i settaggi dei simboli */
	 if (tipo_icona != TP_SYMBOL)
	     symbol_defaults();

	 tipo_icona = TP_SYMBOL;
	 max_icone  = MAX_SYMBOL;
         tot_icone = rec_symb.num_simboli;
         icona_corrente = -1;

   /* recupero i dati dell'icona */

   /* Se non ci sono simboli gia' definiti */
         if ( tot_icone == 0 )
         {
         /* Rende sensibili gli opportuni menu e push button */
            cambia_stato_menu (numero_voci, vm_nessuna_icona);
            num_icona = 0;
            visualizza_icona_standard();
            set_label(widget_array[K_FILE_BITMAP], "<NO ICON>");
         }
         else
         {
         /* Rende sensibili gli opportuni menu e push button */
            cambia_stato_menu (numero_voci, vm_moduli_p_ico);
            num_icona = 1;

            next_icon_available( S_NEXT, rec_symb.bmap_record, MAX_SYMBOL );
    
      /* Configurazione dell'icona con le dimensioni e colori stabiliti. */
            set_icon_config();
          } 
	  break;
   } /*** FINE SWITCH ***/
}

/*-----------------------------------------------------------------------*/
/*** dialog_proc(w, widget_num, selez)
 *** parametri:
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *widget_num : indice in widget_array.
 ***   unsigned long *reason : non rilevante
 ***   Procedure di callback utilizzate all'attivazione dei push-button 
 ***   presenti nella menu-bar che provocano la comparsa di 
 ***   dialog windows */
void dialog_proc(w, widget_num, selez)
   Widget w;
   int *widget_num;
   XmAnyCallbackStruct *selez;
{
   FILE *fp;
   char temp[100], temp2[40];
   short dummy;
   char  *bmp_bits, *bmp_width, *bmp_height;
   Pixel *fg_color, *bg_color;

#if defined OSF1 || defined LINUX
   pid_t   pid;
   int i, status;
#else
   int   pid, i, status;
#endif

   switch(*widget_num) {

/***************************************  dialog_proc   ****************/
	case K_MODULE_P_DIALOG:
/***********************************************************************/

         if (widget_array[ *widget_num ] == NULL )
             if (MrmFetchWidget(s_RMHierarchy, "module_p_dialog",
                             toplevel_widget,
                             &widget_array[ *widget_num ],
                             &dummy_class) != MrmSUCCESS )
                s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );
         XtManageChild( widget_array[ *widget_num ] );
	 crea_find(widget_num);

    /* leggi i moduli dal file icon_list.dat (moduli di processo) */
    /* ed inserisci i nomi nella scroll-list */
         fseek( fp_modp, 0L, SEEK_SET);
         i = 0;
         while( leggi_record_file_ico( fp_modp, &rec_modp ) == RETURN_OK )
         {
             sprintf(temp, "%s     %2d   %.30s", 
                     rec_modp.nome_modulo, rec_modp.num_icone,
                     rec_modp.descr_modulo);
             cstrings[i++] = CREATE_CSTRING(temp);
         }
      /* Aggiorna la scrolled list con la lista dei blocchi */
         argcount = 0;
         XtSetArg(args[argcount], XmNitemCount, i); argcount++;
         XtSetArg(args[argcount], XmNitems, cstrings ); argcount++;
         XtSetValues (widget_array[K_MODULE_P_LIST], args, argcount);
         free_array_XmString(cstrings, i);
	 break;

/***************************************  dialog_proc   ****************/
	case K_MODULE_NP_DIALOG:
/***********************************************************************/

         if (widget_array[ *widget_num ] == NULL )
             if (MrmFetchWidget(s_RMHierarchy, "module_np_dialog",
                             toplevel_widget,
                             &widget_array[ *widget_num ],
                             &dummy_class) != MrmSUCCESS )
                s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );
         XtManageChild( widget_array[ *widget_num ] );
	 crea_find(widget_num);

    /* leggi i moduli dal file icon_list.dat (moduli di processo) */
    /* ed inserisci i nomi nella scroll-list */
         fseek( fp_modnp, 0L, SEEK_SET);
         i = 0;
         while( leggi_file_regola( fp_modnp, &rec_modnp ) == RETURN_OK )
         {
             sprintf(temp, "%s     %.40s", 
                     rec_modnp.nome_modulo, rec_modnp.descr_modulo);
             cstrings[i++] = CREATE_CSTRING(temp);
         }
      /* Aggiorna la scrolled list con la lista dei blocchi */
         argcount = 0;
         XtSetArg(args[argcount], XmNitemCount, i); argcount++;
         XtSetArg(args[argcount], XmNitems, cstrings ); argcount++;
         XtSetValues (widget_array[K_MODULE_NP_LIST], args, argcount);
         free_array_XmString(cstrings, i);
	 break;

/***************************************  dialog_proc   ****************/
	case K_MODIFY_DIALOG:
/***********************************************************************/
    /* Recupero i valori dagli scale-widgets */

    /* Se la bitmap non e' stata creata allora definisci le dimensioni */
    /* della bitmap */
         if (bmap_width == 0 || bmap_height == 0)
         {
         /* Registra la dimensione dell'icona */
            salva_icona( tipo_icona );

            bmap_width  = (unsigned int) icon_width;
            bmap_height = (unsigned int) icon_height - offset_label;
            crea_file_vuoto(filebitmap, bmap_width, bmap_height);
         }

    /* Composizione della linea di comando per chiamare 'bitmap'*/
	 sprintf (temp ,"%dx%d", bmap_width*15,bmap_height*15);
	 sprintf (temp2,"%dx%d", bmap_width  ,bmap_height  );

#ifndef VMS

    /* abilita la ricezione del segnale di terminazione del child */
         signal(SIGCLD, end_bmap_edit);

    /* Rende sensibili gli opportuni menu e push button */
         cambia_stato_menu (numero_voci, vm_sospendi_tutto);

    /* Attiva un nuovo sotto-processo per l'editazione della bitmap */
         pid = fork();

#else

         pid = vfork();

#endif

         if ( pid == 0 )
         {
            char bitmap_exe[100], *percorso;

            percorso = getenv("LEGOCAD_BIN");
#ifdef VMS
            if ( percorso == NULL )
               strcpy(bitmap_exe, "");
            else
               strcpy(bitmap_exe, percorso );
            strcat(bitmap_exe,BITMAP_EXE);
#else
            if ( percorso == NULL )
               strcpy(bitmap_exe, ".");
            else
               strcpy(bitmap_exe, percorso );
            strcat(bitmap_exe,"/");
            strcat(bitmap_exe,BITMAP_EXE);
#endif
#ifndef OSF1
            execl(bitmap_exe,"bitmap","-geometry",temp,
                  filebitmap,temp2,(char *)0);
#else
            execl(bitmap_exe,"bitmap","-geometry",temp, filebitmap,(char *)0);
#endif
         }

#ifdef VMS

         else
         {
             wait(&status);
             crea_pixmap( filebitmap ) ;
	     set_something_val(widget_array[K_BBOARD_ICON],XmNwidth, (XtArgVal) icon_width);
	     set_something_val(widget_array[K_BBOARD_ICON],XmNheight, (XtArgVal) icon_height);
         }

#endif

	 break;

/***************************************  dialog_proc   ****************/
	case K_COPY_DIALOG:
/***********************************************************************/
         if (widget_array[ *widget_num ] == NULL )
         {
	     char pathname[256];

             if (MrmFetchWidget(s_RMHierarchy, "copy_dialog",
                             toplevel_widget,
                             &widget_array[ *widget_num ],
                             &dummy_class) != MrmSUCCESS )
                s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );

         /* Setta il percorso di defaults (dir. corrente) */
/* commentato ...
#ifdef VMS

	    getcwd(pathname,256,1);     0 - DEC/Shell   1 - VMS

#else

	    getcwd(pathname,256);

#endif

            XmTextSetString(widget_array[K_COPY_TEXT], pathname);
***/
         }
         XtManageChild( widget_array[ *widget_num ] );

         XtSetKeyboardFocus (widget_array[*widget_num],
                             widget_array[K_COPY_TEXT]);
	 break;
   }
}

/*-----------------------------------------------------------------------*/
/*** drag_proc(w, widget_num, selez)
 *** parametri:
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *widget_num : indice in widget_array.
 ***   unsigned long *reason : non rilevante
 ***   Procedure di callback utilizzata quando si trascina il cursore   
 ***   negli Scale Widget della altezza e larghezza delle icone.  */
void drag_proc(w, widget_num, selez)
   Widget w;
   int *widget_num;
   XmAnyCallbackStruct *selez;
{
   int i;

   switch(*widget_num) {

	case K_ICON_WIDTH:
	     get_something(widget_array[*widget_num],XmNvalue,(void*) &i);
             icon_width = i * STEPPING;
	     centra_icona_dinamica(WIDTH);
	break;

	case K_ICON_HEIGHT:
	     get_something(widget_array[*widget_num],XmNvalue,(void*) &i);
             icon_height = i * STEPPING + offset_label;
	     centra_icona_dinamica(HEIGHT);
	break;
   }
}

/*-----------------------------------------------------------------------*/
/*** void cambio_dim(w, ev, params, num_params)
 *** Ad ogni ridimensionamento dell'applicativo, legge le nuove
 *** dimensioni della scrolled window delle icone
 *** e le passa al bulletin board all'interno di questa.
 *** Lo scopo e' quello di avere sempre l'icona centrata nella window.
 ***/
void cambio_dim(w, client_data, ev, boh)
Widget w;
char *client_data;
XEvent *ev;
Boolean boh;
{
 /* Legge le dimensioni della scrolled window e del bulletin board
    all'interno */
    get_something(w, XmNwidth, (void*) &window_width );
    get_something(w, XmNheight, (void*) &window_height );
/***
    get_something(widget_array[K_BBOARD_ICON], XmNwidth, (void*) &bb_width );
    get_something(widget_array[K_BBOARD_ICON], XmNheight, (void*) &bb_height );
***/

    if (icon_width < window_width)
    {
        set_something_val(widget_array[K_BBOARD_WINDOW],XmNwidth, (XtArgVal) window_width);
	set_something_val(widget_array[K_BBOARD_ICON],XmNx, (XtArgVal) (window_width-icon_width)/2);
    }
    else
    {
        set_something_val(widget_array[K_BBOARD_WINDOW],XmNwidth, (XtArgVal) icon_width);
	set_something_val(widget_array[K_BBOARD_ICON],XmNx, (XtArgVal) 0);
    }

    if (icon_height < window_height)
    {
        set_something_val(widget_array[K_BBOARD_WINDOW],XmNheight, (XtArgVal) window_height);
	set_something_val(widget_array[K_BBOARD_ICON],XmNy, (XtArgVal) (window_height-icon_height)/2);
    }
    else
    {
        set_something_val(widget_array[K_BBOARD_WINDOW],XmNheight, (XtArgVal) icon_height);
        set_something_val(widget_array[K_BBOARD_ICON],XmNy, (XtArgVal) 0);
    }
}

/*-----------------------------------------------------------------------*/
/*** select_modulo(w, widget_num, selez)
 *** parametri:
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *widget_num : indice in widget_array.
 ***   XmListCallbackStruct *selez : info sulla scroll-list.
Tale procedura viene invocata quando l'utente seleziona un modulo nella
window dei moduli di processo oppure nella window dei modulo non di processo
(schemi di regolazione). */
void select_modulo(w, widget_num, list_info)
Widget w;
int *widget_num;
XmListCallbackStruct *list_info;
{
   char *icon_filename;
   int  pos, size;

   modulo_selezionato( list_info->item, *widget_num );
}

/*-----------------------------------------------------------------------*/
/*** ok_proc(w, widget_num, selez)
 *** parametri:
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *widget_num : indice in widget_array.
 ***   unsigned long *reason : non rilevante
 Procedure di callback utilizzata quando si preme 'Ok'
 nelle varie Dialog Box  */
void ok_proc(w, widget_num, selez)
Widget w;
int *widget_num;
XmAnyCallbackStruct *selez;
{
   char *icon_filename;
   int  fd;
   XmStringTable lista_select;

   switch(*widget_num) {

	case K_MODULE_P_DIALOG:
             get_something(widget_array[K_MODULE_P_LIST],XmNselectedItems,
			   (void*) &lista_select);
             modulo_selezionato(lista_select[0], K_MODULE_P_LIST);
          break;

	case K_MODULE_NP_DIALOG:
             get_something(widget_array[K_MODULE_NP_LIST],XmNselectedItems,
			   (void*) &lista_select);
             modulo_selezionato(lista_select[0], K_MODULE_NP_LIST);
          break;

	case K_COPY_DIALOG:
             icon_filename = XmTextGetString(widget_array[K_COPY_TEXT]);
             tominus(icon_filename);
             if ( access(icon_filename, F_OK | R_OK) == -1 )
             {
                s_warning( toplevel_widget, &geom_attention, APPLICATION_NAME,
                           warning_mesg, WCOPYFILE );
		return;
             }
             copia_file(icon_filename, filebitmap);
             crea_pixmap(filebitmap);
             set_icon_config(); /* Ok! setta la nuova configurazione */
             XmStringFree(icon_filename);
	     XtUnmanageChild (widget_array[*widget_num]);
          break;
   }
}

/*-----------------------------------------------------------------------*/
/*** cancel_proc(w, widget_num, selez)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int *widget_num : indice in widget_array.
 *** unsigned long *reason : non rilevante
 *** Procedure di callback utilizzata quando si preme 'Ok'
 *** nelle varie Dialog Box  */
void cancel_proc(w, widget_num, selez)
Widget w;
int *widget_num;
XmAnyCallbackStruct *selez;
{

   switch(*widget_num) {

	case K_MODULE_P_DIALOG:
              XtUnmanageChild (widget_array[*widget_num]);
	break;

	case K_MODULE_NP_DIALOG:
              XtUnmanageChild (widget_array[*widget_num]);
	break;

	case K_COPY_DIALOG:
              XtUnmanageChild (widget_array[*widget_num]);
	break;
   }
}

/*-----------------------------------------------------------------------*/
/*** select_icon(w, widget_num, selez)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int *widget_num : indice in widget_array.
 *** unsigned long *reason : non rilevante
 *** Procedure di callback utilizzata quando si preme 'Previous' o 'Next'
 *** per selezionare una delle icone di uno stesso modulo LEGO 
 *** oppure un simbolo dal file symbol.dat
 ***/
void select_icon(w, widget_num, selez)
Widget w;
int *widget_num;
XmAnyCallbackStruct *selez;
{
   BitmapFileRec *rec_bitmap;

   switch(*widget_num) {

	case K_PREVIOUS_ICON:
           if (num_icona == 1)
              return;
           else
	   {
              num_icona--;

              if (tipo_icona == TP_MODULE_PROCESS)
	          next_icon_available( S_PREVIOUS, rec_modp.bmap_record,
				       MAX_ICON_X_PMODULE );
              if (tipo_icona == TP_SYMBOL)
	          next_icon_available( S_PREVIOUS, rec_symb.bmap_record,
				       MAX_SYMBOL);
           }
	break;

	case K_NEXT_ICON:
           if (num_icona == tot_icone)
              return;
           else
	   {
              num_icona++;

              if (tipo_icona == TP_MODULE_PROCESS)
	          next_icon_available( S_NEXT, rec_modp.bmap_record,
				       MAX_ICON_X_PMODULE );
              if (tipo_icona == TP_SYMBOL)
	          next_icon_available( S_NEXT, rec_symb.bmap_record,
				       MAX_SYMBOL);
           }
	break;
   }

   strcpy(filebitmap, path_bmap);

   switch (tipo_icona)
   {
      case TP_MODULE_PROCESS:
           rec_bitmap = &rec_modp.bmap_record[icona_corrente];
	   break;

      case TP_SYMBOL:
           rec_bitmap = &rec_symb.bmap_record[icona_corrente];
	   break;
   }

   strcat(filebitmap, rec_bitmap->nome_file);
   strcpy(only_filebmp_name,rec_bitmap->nome_file);
   icon_fg = rec_bitmap->colore_fg;
/* icon_bg = rec_bitmap->colore_bg;   */
   icon_width = rec_bitmap->base;
   icon_height = rec_bitmap->altezza + offset_label;

/* crea la pixmap */
   crea_pixmap(filebitmap);

   set_icon_config(); /* Ok! setta la nuova configurazione */
}

/*-----------------------------------------------------------------------*/
/*** color_fg_proc(w, color_num, selez)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int *color_num : indice dei colori.
 *** unsigned long *reason : non rilevante
 ***/
void color_fg_proc(w, color_num, selez)
Widget w;
int *color_num;
XmAnyCallbackStruct *selez;
{
   icon_fg = (short) *color_num;
   set_something_val(widget_array[K_ICON_PIXMAP],XmNforeground, (XtArgVal) apix[icon_fg]);
   if ( !pixmap_creata )
      return;

   XSetForeground (display, gc_icona, apix[icon_fg]);
   XCopyPlane(display, bmap_pixmap, icon_pixmap, gc_icona, 0, 0,
              bmap_width, bmap_height, 0, 0, 1);
   set_something_val(widget_array[K_ICON_PIXMAP],XmNlabelPixmap, (XtArgVal) icon_pixmap);
   XSync(display, False);
}

/*-----------------------------------------------------------------------*/
/*** color_bg_proc(w, color_num, selez)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int *color_num : indice dei colori.
 *** unsigned long *reason : non rilevante
 ***/

/*** DISABILITATO
   void color_bg_proc(w, color_num, selez)
   Widget w;
   int *color_num;
   XmAnyCallbackStruct *selez;
   {
      icon_bg = *color_num;
      set_something_val(widget_array[K_ICON_PIXMAP],XmNbackground, (XtArgVal) apix[icon_bg]);
      if ( !pixmap_creata )
         return;

      XSetBackground (display, gc_icona, apix[icon_bg]);
      XCopyPlane(display, bmap_pixmap, icon_pixmap, gc_icona, 0, 0,
                 bmap_width, bmap_height, 0, 0, 1);
      set_something_val(widget_array[K_ICON_PIXMAP],XmNlabelPixmap, (XtArgVal) icon_pixmap);
   }
***/

/*-----------------------------------------------------------------------*/
/* FUNZIONI DI UTILITA'							 */
/*-----------------------------------------------------------------------*/
/*** void centra_icona_dinamica (dimensione)
 *** Parametri:
 *** int dimensione: nuova dimensione dell'icona (alternativamente
 *** larghezza o altezza) determinata muovendo il cursore dello Scale.
 *** Centra l'icona all'interno della scrolled window.
 ***/
void centra_icona_dinamica(dimensione)
int dimensione;
{
    switch (dimensione)  {

	case WIDTH:

       /* dimensiona l'icona */
	   set_something_val(widget_array[K_BBOARD_ICON],XmNwidth, (XtArgVal) icon_width);

       /* centra l'icona */
    	   if (icon_width < window_width)
	   {
              set_something_val(widget_array[K_BBOARD_WINDOW],
		     XmNwidth, (XtArgVal) window_width);
	      set_something_val(widget_array[K_BBOARD_ICON],XmNx, (XtArgVal) (window_width-icon_width)/2);
	   }
	   else
              set_something_val(widget_array[K_BBOARD_WINDOW],
		     XmNwidth, (XtArgVal) icon_width);
	break;

	case HEIGHT:

       /* dimensiona l'icona */
	   set_something_val(widget_array[K_BBOARD_ICON],XmNheight, (XtArgVal) icon_height);

       /* centra l'icona */
    	   if (icon_height < window_height)
	   {
              set_something_val(widget_array[K_BBOARD_WINDOW],
		     XmNheight, (XtArgVal) window_height);
	      set_something_val(widget_array[K_BBOARD_ICON],XmNy, (XtArgVal) (window_height-icon_height)/2);
	   }
	   else
              set_something_val(widget_array[K_BBOARD_WINDOW],
		     XmNheight, (XtArgVal) icon_height);
	break;
    }
}

/*-----------------------------------------------------------------------*/
/*** void get_pixel(names,apix,count)
 *** PARAMETRI:
 *** char *names: stringhe contenenti i nomi dei colori
 *** Pixel *apix: array per ospitare i valori di pixel (?)
 *** int   count: contatore ENEL
 Funzione che recupera i valori RGB dei nomi dei colori specificati.  ***/
void get_pixel(names,apix,count)
char *names[];
Pixel *apix;
int count;
{
   XColor defc;
   int i;

   for (i=0 ; i < count ; i++ )
   {
      if ( !XParseColor( display, colormap, names[i], &defc  ))
         fprintf(stderr,"il colore %s non esiste nel database.\n",names[i]);
      else
         if ( !XAllocColor( display, colormap, &defc))
            fprintf(stderr,"non e' possibile allocare il colore %s.\n",
                    names[i]);
         else
            apix[i] = defc.pixel;
   }
}

/*-----------------------------------------------------------------------*/
/*** void crea_find(num)
 *** PARAMETRI
 *** int *num: parametro per il switch.
 *** Crea i FIND di libreria per le scroll list (Process & Scheme).
 ***/
void crea_find(num)
int *num;
{
   static Widget find_form_p, find_form_np;

   switch (*num) {

   /* FIND dei moduli di processo */
   case K_MODULE_P_DIALOG:
     find_process.ID_lista = widget_array[K_MODULE_P_LIST];
     find_process.num_elem = 100;
     find_process.vis_elem = VISIBLE_ITEM_COUNT;
     find_process.pos      = 0;   /* posizione corrente nel find */
     argcount=0;
     XtSetArg(args[argcount],XmNleftAttachment,XmATTACH_FORM); argcount++;
     XtSetArg(args[argcount],XmNtopAttachment,XmATTACH_POSITION); argcount++;
     XtSetArg(args[argcount],XmNtopPosition,82); argcount++;
     if (find_form_p == NULL)
         find_form_p = find_kit(widget_array[K_MODULE_P_FORM],
     				args,argcount,&find_process);
   break;

   /* FIND dei moduli di schema */
   case K_MODULE_NP_DIALOG:
     find_scheme.ID_lista = widget_array[K_MODULE_NP_LIST];
     find_scheme.num_elem = 100;
     find_scheme.vis_elem = VISIBLE_ITEM_COUNT;
     find_scheme.pos      = 0;   /* posizione corrente nel find */
     argcount=0;
     XtSetArg(args[argcount],XmNleftAttachment,XmATTACH_FORM); argcount++;
     XtSetArg(args[argcount],XmNtopAttachment,XmATTACH_POSITION); argcount++;
     XtSetArg(args[argcount],XmNtopPosition,82); argcount++;
     if (find_form_np == NULL)
         find_form_np = find_kit(widget_array[K_MODULE_NP_FORM],
     				 args,argcount,&find_scheme);
   break;
   }
}

#define BUFFER_SIZE	512

/*-----------------------------------------------------------------------*/
/*** void end_bmap_edit()
funzione di segnalazione di terminazione del processo child.
Viene chiamata dal signal SIGCLD */
void end_bmap_edit(sig, code, scp, addr)
int sig, code;
struct sigcontext *scp;
char *addr;
{
    int status;

    wait(&status);

 /* Se il file bitmap e' stato creato/modificato allora visualizza la */
 /* bitmap */
    crea_pixmap( filebitmap ) ;
    set_something_val(widget_array[K_BBOARD_ICON],XmNwidth, (XtArgVal) icon_width);
    set_something_val(widget_array[K_BBOARD_ICON],XmNheight, (XtArgVal) icon_height);

 /* Rende sensibili gli opportuni menu e push button */
    switch (tipo_icona) {

        case TP_MODULE_PROCESS:
	case TP_SYMBOL:
             cambia_stato_menu (numero_voci, vm_moduli_p_ico);
             break;

        case TP_MODULE_SCHEME:
             cambia_stato_menu (numero_voci, vm_nessuna_icona);
             break;

        case TP_STD_PROCESS:
        case TP_STD_SCHEME:
             cambia_stato_menu (numero_voci, vm_edi_icona_std);
             break;
    }
    XSync(display, False);
}

/*-----------------------------------------------------------------------*/
/*** int crea_pixmap( filebitmap )
 *** Parametri:
 ***    char *filebitmap: nome del file bitmap.
funzione che crea la pixmap con il suo GC dal bitmap file.
Vengono settate le varibili globali icona (pixmap) e gc_icona.
Eventualmente la funzione si preoccupa di deallocare la memoria dalla 
pixmap precedente. */
crea_pixmap( filebitmap )
char *filebitmap;
{
   XGCValues gc_val;
   Widget    w;

   w = widget_array[K_ICON_PIXMAP];

/* deallocare la memoria della pixmap dell'icona  */
   if (icon_pixmap != NULL)
      if (pixmap_creata)
      {
         XFreePixmap (display,icon_pixmap);
         XFreePixmap (display,bmap_pixmap);
      }

/* Leggi il file bitmap. Se non si puo' leggere il file , ritorna! */
   if ( XReadBitmapFile(display,XtWindow(w),filebitmap,
                        &bmap_width, &bmap_height, &bmap_pixmap, &bmap_x_hot,
		        &bmap_y_hot) != BitmapSuccess )
   {
        s_warning( toplevel_widget, &geom_attention, APPLICATION_NAME,
                   warning_mesg, WREADBMAP );
        set_something_val(widget_array[K_ICON_PIXMAP] ,XmNlabelPixmap, (XtArgVal) XmUNSPECIFIED_PIXMAP);
        pixmap_creata = False;
	return(-1);
   }

   icon_pixmap = XCreatePixmap(display, XtWindow(w), bmap_width, bmap_height,
			 DefaultDepth(display,screen));

   XSetForeground( display, gc_icona, apix[icon_fg]);
   XSetBackground( display, gc_icona, apix[icon_bg]); 

/* trasformazione della pixmap di prof. 1 a pixmap di prof. della window */
/* (solitamente 8) */
   XCopyPlane(display, bmap_pixmap, icon_pixmap, gc_icona, 0, 0,
              bmap_width, bmap_height, 0, 0, 1);

/* Assegnazione della pixmap creata al widget dell'icona */
   set_something_val(w ,XmNlabelPixmap, (XtArgVal) icon_pixmap);
   set_something_val(w ,XmNbackground, (XtArgVal) apix[icon_bg]); 
   pixmap_creata = True;
   return(0);  /* ritorno con successo! */
}

/*-----------------------------------------------------------------------*/
/*** set_label( w, string )
 *** Parametri: 
 ***    Widget w:  label widget o qualsiasi altro widget che contiene
 ***               la risorsa XmNlabelString.
 *** char *string: stringa di caratteri.
funzione che assegna una stringa di caratteri alla risorsa XmNlabelString
del widget */
set_label( w, string )
Widget w;
char   *string;
{
   XmString compound_str;

   compound_str = CREATE_CSTRING(string);
   set_something_val(w, XmNlabelString, (XtArgVal) compound_str);
   XmStringFree(compound_str);
}

/*-----------------------------------------------------------------------*/
/*** set_icon_config()
 *** Parametri:
 ***     Nothing!
procedura per il settaggio della configurazione dell'icona in base alle
variabili globali che ne identificano la dimensione (icon_height e icon_width)
ed i colori (icon_fg e icon_bg=0=WHITE).
Settare tali variabili prima di chiamare la procedura */
set_icon_config()
{
   char temp[10];
   int  min_width, min_height;

/* Controlla che le dimensioni dell'icona non siano minori dei valori */
/* minimi stabiliti ( 2x5 per i simboli e 10x5 per i moduli ) */
   get_something(widget_array[K_ICON_WIDTH],XmNminimum,(void*) &min_width);
   get_something(widget_array[K_ICON_HEIGHT],XmNminimum,(void*) &min_height);
   min_width *= STEPPING;
   min_height = min_height * STEPPING + offset_label;
   icon_width = (icon_width > min_width) ? icon_width : min_width;
   icon_height = (icon_height > min_height) ? icon_height : min_height;

   sprintf(temp,"%2d", tot_icone); 
   set_label( widget_array[K_ICONS_OF_MODULE], temp);
   sprintf(temp,"%2d", num_icona); 
   set_label( widget_array[K_ORDER_OF_ICONS ], temp);

/* Visualizza il nome del file bitmap senza percorso */
   set_label(widget_array[K_FILE_BITMAP], only_filebmp_name);

/* posiziona correttamente gli slider degli scale-widgets */
   set_something_val(widget_array[K_ICON_WIDTH],XmNvalue, (XtArgVal) icon_width / STEPPING);
   set_something_val(widget_array[K_ICON_HEIGHT],
		 XmNvalue, (XtArgVal) (icon_height - offset_label) / STEPPING );

   centra_icona_dinamica(WIDTH);
   centra_icona_dinamica(HEIGHT);

/* setta le opzioni corrette sui menu-option dei colori fg/bg */
   set_something_val(widget_array[K_FG_MENU],XmNmenuHistory, (XtArgVal) widget_array[K_WHITE_FG + icon_fg]);
/*   set_something_val(widget_array[K_BG_MENU],XmNmenuHistory, (XtArgVal) widget_array[K_WHITE_BG + icon_bg]);   */

/* Setta i colori della label della pixmap */
   set_something_val(widget_array[K_ICON_LABEL],XmNbackground, (XtArgVal) apix[WHITE]);
   set_something_val(widget_array[K_ICON_PIXMAP],XmNbackground, (XtArgVal) apix[icon_bg]); 
}

/*-----------------------------------------------------------------------*/
/*** crea_file_vuoto(nomefile)
 *** Parametri:
 ***   char *nomefile: nome del file da creare.
crea un file bitmap con i bits a 0 */
crea_file_vuoto(nomefile, width, height)
char *nomefile;
int width, height;
{
   register int i, raster_length;
   FILE *fp;

   if (( fp = fopen(nomefile, "w")) == NULL)
      return(-1);
   fprintf (fp, "#define %s_width %d\n", NOME_VAR_BITMAP, width);
   fprintf (fp, "#define %s_height %d\n", NOME_VAR_BITMAP, height);
   fprintf (fp, "#define %s_x_hot %d\n", NOME_VAR_BITMAP, -1);
   fprintf (fp, "#define %s_y_hot %d\n", NOME_VAR_BITMAP, -1);
   fprintf (fp, "static char %s_bits[] = {\n   0x%02x",
            NOME_VAR_BITMAP, (unsigned char) 0);

   raster_length = round((double) width/8) * height;
   for (i=1;i<raster_length;i++)
   {
      fprintf (fp, ",");
      fprintf (fp, (i % 12) ? " " : "\n   ");
      fprintf (fp, "0x%02x", (unsigned char) 0);
   }
   fprintf (fp, "};\n");

   fclose(fp);
   return(0);
}

/*-----------------------------------------------------------------------*/
/*** round(valore)
 *** Parametro:
 ***     double var;
arrotonda un valore double */
int round(valore)
double valore;
{
    return( (valore > (int) valore) ? (int) valore + 1 : (int) valore);
}

/*-----------------------------------------------------------------------*/
/*** set_default_config()
 *** Parametri:
 ***    U Belin
Setta le variabili globali relative all'icona ai valori di default */
set_default_config()
{

/* Dimensione della bitmap non definite . */
   bmap_width  = 0;
   bmap_height = 0;
   icon_fg = BLACK;
/* icon_bg = WHITE;  */
   icon_width = default_icon_width;
   icon_height = default_icon_height; 
/*   icon_height = default_icon_height + ICON_LABEL_HEIGHT;  */
}

/*-----------------------------------------------------------------------*/
/*** set_default_symbol()
 *** Parametri:
 ***    U l'aiva una fetta de bura...
 *** Setta le variabili globali relative all'icona ai valori di default 
 *** dei SIMBOLI
 ***/
set_default_symbol()
{

/* Dimensione della bitmap non definite . */
   bmap_width  = 0;
   bmap_height = 0;
   icon_fg = BLACK;
   icon_width  = SYMBOL_MIN_DIMENSION * STEPPING;
   icon_height = SYMBOL_MIN_DIMENSION * STEPPING;
}

/*-----------------------------------------------------------------------*/
/*** modulo_selezionato( cstr, tipo_modulo )
 *** Parametri:
 ***     XmString cstr : compound string modulo selezionato
 ***     int tipo_modulo : tipo modulo ( se processo o schema )
Procedura chiamata quando l'utente ha selezionato un modulo dalle scroll-list
dei moduli di processi o dei moduli di schema */
modulo_selezionato( cstr, tipo_modulo )
XmString cstr;
int tipo_modulo;
{

   sprintf(nome_modulo,"%.4s", extract_string( cstr ));
 
   offs_modulop = offs_scheme = 0;
   if ( tipo_modulo == K_MODULE_P_LIST )
   {
   /* Cancellazione dell'una finestra */
      if ( nome_modulo_passato == False )
         XtUnmanageChild ( widget_array[K_MODULE_P_DIALOG]);
      nome_modulo_passato = False;
      offs_modulop = cerca_modulo_proc( fp_modp, &rec_modp, nome_modulo );
      strcpy( descr_modulo, rec_modp.descr_modulo);
   }
   else
   {
   /* Cancellazione dell'altra finestra */
      XtUnmanageChild (widget_array[K_MODULE_NP_DIALOG]);
      offs_scheme = cerca_modulo_regola( fp_modnp, &rec_modnp, nome_modulo );
      strcpy( descr_modulo, rec_modnp.descr_modulo);
   }

   if ( offs_modulop == RETURN_ERROR || offs_scheme == RETURN_ERROR )
   {
      s_warning( toplevel_widget, &geom_attention, APPLICATION_NAME,
                 warning_mesg, WNOMODULE );
      return;
   }

/* Aggiorna la label del nome del modulo selezionato */
   set_label(widget_array[K_MODULE_NAME], nome_modulo);
   set_label(widget_array[K_MODULE_DESCRIPTION], descr_modulo);
   set_label(widget_array[K_ICON_LABEL],nome_modulo);
   set_something_val(widget_array[K_ICON_LABEL],XmNbackground, (XtArgVal) apix[WHITE]);

/* Se si stava agendo sui simboli, vengono reimpostati i settaggi */
/* per le icone "normali" (standard, process, scheme) */
   if (tipo_icona == TP_SYMBOL)
      icon_defaults();

/* Se l'utente ha scelto nella lista dei moduli non di processo (schema) */
/* allora visualizza l'icona standard (che e' uguale per tutti i moduli */
/* di schema ed e' unica!) */
   if ( tipo_modulo == K_MODULE_NP_LIST )
   {						
      cambia_stato_menu (numero_voci, vm_moduli_np);
      max_icone = 0;
      num_icona = 0;
      tot_icone = 0;
      tipo_icona = TP_MODULE_SCHEME;
      visualizza_icona_standard();
      return;
   }

/***********************************************************/
/* si considerano di seguito solo i moduli di processo ... */
   tipo_icona = TP_MODULE_PROCESS;
   max_icone = MAX_ICON_X_PMODULE;
   tot_icone = rec_modp.num_icone;
   icona_corrente = -1;

   if ( tot_icone == 0 )
   {
/* Rende sensibili gli opportuni menu e push button */
      cambia_stato_menu (numero_voci, vm_nessuna_icona);

      num_icona = 0;
      visualizza_icona_standard();
      strcpy(only_filebmp_name, "<NO ICON>");
   }
   else
   {
   /* Rende sensibili gli opportuni menu e push button */
      cambia_stato_menu (numero_voci, vm_moduli_p_ico);

      num_icona = 1;
      next_icon_available( S_NEXT, rec_modp.bmap_record, MAX_ICON_X_PMODULE );
   }

/* Configurazione dell'icona con le dimensioni e colori stabiliti. */
   set_icon_config();
}

/*-----------------------------------------------------------------------*/
/*** salva_bitmap()
 *** Parametri:
 ***    Niuno.
salva la bitmap bmap_pixmap (globale --> indi deve essere settata prima di 
chiamare al funzione) nel file con le dimensioni recuperate dagli
scale widgets (risetta le dimensioni bmap_width e bmap_height) ***/
salva_bitmap()
{
   Pixmap app_pixmap;
   unsigned int dimw, dimh;
   Dimension bbw, bbh, bbw2, bbh2;

   dimw = (unsigned int) icon_width;
   dimh = (unsigned int) icon_height - offset_label;

/***
printf("vecchie dim. %d , %d\n", bmap_width, bmap_height);
printf("nuove   dim. %d , %d\n", dimw, dimh);
get_something(widget_array[K_ICON_PIXMAP], XmNheight, (void*) &bbh);
get_something(widget_array[K_ICON_PIXMAP], XmNwidth, (void*) &bbw);
get_something(widget_array[K_ICON_LABEL], XmNheight, (void*) &bbh2);
get_something(widget_array[K_ICON_LABEL], XmNwidth, (void*) &bbw2);
printf("dim icon pixmap %d , %d label %d, %d\n", bbw, bbh, bbw2, bbh2);
***/

   app_pixmap = XCreatePixmap(display, bmap_pixmap, dimw, dimh,
                              DefaultDepth(display,screen));

   XSetForeground (display, gc_icona, apix[BLACK]);
   XFillRectangle(display, app_pixmap, gc_icona, 0, 0, dimw, dimh);

   XSetForeground (display, gc_icona, apix[BLACK]);
   XSetFunction(display, gc_icona, GXcopyInverted);
   XCopyPlane(display, bmap_pixmap, app_pixmap, gc_icona, 
	      (dimw < bmap_width) ? (bmap_width-dimw)/2 : 0, 
	      (dimh < bmap_height) ? (bmap_height-dimh)/2 : 0,
              bmap_width, bmap_height,
	      (dimw > bmap_width) ? (dimw-bmap_width)/2 : 0, 
	      (dimh > bmap_height) ? (dimh-bmap_height)/2 : 0, 1);

/* Setta le nuove dimensioni della bitmap */
   bmap_width  = dimw;
   bmap_height = dimh;
   if (XWriteBitmapFile(display, filebitmap,
	                app_pixmap, bmap_width, bmap_height,
		        -1, -1) != BitmapSuccess)
      s_warning( toplevel_widget, &geom_attention, APPLICATION_NAME,
                 warning_mesg, WCANTWRITE );

   XSetForeground (display, gc_icona, apix[icon_fg]);
   XSetFunction(display, gc_icona, GXcopy);
   XFreePixmap(display, app_pixmap);

/* rileggi il file bitmap per assegnare la nuova bitmap a bmap_pixmap */
   crea_pixmap(filebitmap);
}

/*-----------------------------------------------------------------------*/
/*** salva_icona( tipo_icona )
 *** Parametri:
 ***     int tipo_icona : tipo di icona ( standard o no ).
salva nel file opportuno la configurazione dell'icona.
Per quanto riguarda le icone non standard riferite ai moduli di processo
bisogna settare la variabile globale icona_corrente. */
salva_icona( tipo_icona )
int tipo_icona;
{
   switch ( tipo_icona ) {
      case TP_MODULE_PROCESS:
         if ( (icona_corrente >= 0 || icona_corrente < 10) &&
              offs_modulop >= 0 )
         {
            rec_modp.num_icone = tot_icone;
            rec_modp.bmap_record[icona_corrente].colore_fg = icon_fg;
            rec_modp.bmap_record[icona_corrente].colore_bg = icon_bg; 
            rec_modp.bmap_record[icona_corrente].base = icon_width;
            rec_modp.bmap_record[icona_corrente].altezza = icon_height -
                                                           ICON_LABEL_HEIGHT;
            fseek( fp_modp, offs_modulop, SEEK_SET );
            scrivi_record_file_ico( fp_modp, &rec_modp);
         }
         else
            s_warning( toplevel_widget, &geom_attention, APPLICATION_NAME,
                       warning_mesg, WCANTSAVE );
         break;

      case TP_SYMBOL:
         if (icona_corrente >= 0 || icona_corrente < MAX_SYMBOL
	     && icona_corrente*record_symb_size >= 0) 
         {
            rec_symb.num_simboli = tot_icone;
            rec_symb.bmap_record[icona_corrente].colore_fg = icon_fg;
            rec_symb.bmap_record[icona_corrente].colore_bg = icon_bg; 
            rec_symb.bmap_record[icona_corrente].base = icon_width;
            rec_symb.bmap_record[icona_corrente].altezza = icon_height;

            fseek( fp_symb, icona_corrente * record_symb_size, SEEK_SET ); 
            scrivi_file_simboli(fp_symb, 
				&rec_symb.bmap_record[icona_corrente] );
         }
         else
            s_warning( toplevel_widget, &geom_attention, APPLICATION_NAME,
                       warning_mesg, WCANTSAVE );
         break;

      case TP_STD_PROCESS:
      case TP_STD_SCHEME:
         if ( offs_std >= 0 )
         {
            rec_std.num_icone = tot_icone;
            rec_std.bmap_record.colore_fg = icon_fg;
            rec_std.bmap_record.colore_bg = icon_bg; 
            rec_std.bmap_record.base = icon_width;
            rec_std.bmap_record.altezza = icon_height - ICON_LABEL_HEIGHT;
            fseek(fp_std, offs_std, SEEK_SET);
            scrivi_file_std_ico(fp_std, &rec_std);
         }
         else
            s_warning( toplevel_widget, &geom_attention, APPLICATION_NAME,
                       warning_mesg, WCANTSAVE );
         break;
   }
}

/*-----------------------------------------------------------------------*/
/*** next_icon_available(verso_ricerca)
 *** Parametri: 
 *** int verso_ricerca: avanti/indietro
 *** BitmapFileRec bmap_record[]: array di strutture BitmapFileRec
 *** int max_figures: massimo numero di icone o simboli
 *** Ricerca la successiva icona disponibile a partire dall'icona corrente.
 *** Settare quindi icona_corrente prima di chiamare questa procedura 
 ***/
next_icon_available( verso_ricerca, bmap_record, max_figures )
int verso_ricerca;
BitmapFileRec bmap_record[];
int max_figures;
{
   int i;

   if ( verso_ricerca == S_NEXT )
       for(i = icona_corrente+1 ; i < max_figures ; i++)
          if (bmap_record[i].esiste)
             break;

   if ( verso_ricerca == S_PREVIOUS )
       for(i = icona_corrente-1 ; i >= 0 ; i--)
          if (bmap_record[i].esiste)
             break;

   if ( i < 0 || i >= max_figures )
      return(-1);

   icona_corrente = i;

   strcpy(filebitmap, path_bmap);
   strcat(filebitmap, bmap_record[icona_corrente].nome_file);
   icon_fg     = bmap_record[icona_corrente].colore_fg;
   icon_width  = bmap_record[icona_corrente].base;
   icon_height = bmap_record[icona_corrente].altezza + offset_label;

/* Visualizza il nome del file bitmap senza percorso */
   strcpy(only_filebmp_name,bmap_record[icona_corrente].nome_file);
   set_label(widget_array[K_FILE_BITMAP], only_filebmp_name);

   crea_pixmap(filebitmap);
   return(0);
}

/*-----------------------------------------------------------------------*/
/*** cerca_file_disponibile()
 *** Parametri:
 ***    nessuno
 ricerca un nome di file disponibile nella lista delle icone del modulo
 selezionato . Viene settata la variabile globale icona_corrente. */
cerca_file_disponibile()
{
   int i;

   switch (tipo_icona) {

   case TP_MODULE_PROCESS:
        for(i = 0 ; i < 10 ; i++)
           if ( !rec_modp.bmap_record[i].esiste )
              break;
        icona_corrente = i;

        if ( i < 10 )
           return(0);
        else
           return(-1);
   break;


   case TP_SYMBOL:
        for(i = 0 ; i < MAX_SYMBOL ; i++)
           if ( !rec_symb.bmap_record[i].esiste )
              break;

        icona_corrente = i;

        if ( i < MAX_SYMBOL )
           return(0);
        else
           return(-1);
   break;
   }
}

/*-----------------------------------------------------------------------*/
/*** visualizza_icona_standard()
 *** Parametri:
 ***             nada ...
visualizza l'icona standard. Viene fatto riferimento alla variabile globale
tipo_icona. */
visualizza_icona_standard()
{
   switch (tipo_icona)
   {
      case TP_MODULE_PROCESS:
      case TP_MODULE_SCHEME :
            if ( tipo_icona == TP_MODULE_PROCESS )
               offs_std = leggi_file_std_ico(fp_std, &rec_std, MODULE_PROCESS);
            else
               offs_std = leggi_file_std_ico(fp_std, &rec_std, MODULE_SCHEME);

            if ( rec_std.num_icone )
            {
            /* recupero i dati dell'icona */
               strcpy(filebitmap, path_bmap);
               strcat(filebitmap, rec_std.bmap_record.nome_file);
      	       strcpy(only_filebmp_name,rec_std.bmap_record.nome_file);
               icon_fg = rec_std.bmap_record.colore_fg;
/*             icon_bg = rec_std.bmap_record.colore_bg;  */
               icon_width = rec_std.bmap_record.base;
               icon_height = rec_std.bmap_record.altezza + offset_label;
               crea_pixmap(filebitmap);
            }
            else
            {
               set_default_config();
               set_something_val(widget_array[K_ICON_PIXMAP] ,XmNlabelPixmap, (XtArgVal) XmUNSPECIFIED_PIXMAP);
               pixmap_creata = False;
            }
            set_icon_config();
           break;

      case TP_SYMBOL	    :
            set_default_symbol();
            set_something_val(widget_array[K_ICON_PIXMAP] ,XmNlabelPixmap, (XtArgVal) XmUNSPECIFIED_PIXMAP);
            pixmap_creata = False;
            set_icon_config();
           break;



      case TP_STD_PROCESS   :
      case TP_STD_SCHEME    :
           set_something_val(widget_array[K_ICON_PIXMAP] ,XmNlabelPixmap, (XtArgVal) XmUNSPECIFIED_PIXMAP);
           pixmap_creata = False;
           break;
   }
}

/*-----------------------------------------------------------------------*/
/*** icon_defaults()
 *** Imposta i default quando viene selezionata le voci di menu:
 *** EDIT > Standard >> Process modules | Scheme modules
 *** EDIT > Modules  >> Process modules | Scheme modules
 ***/
void icon_defaults()
{
   unsigned int scaled_icon_width, scaled_icon_height;
   Dimension bbw, bbh, bbw2, bbh2;

/* Fissa la larghezza della label dell'icona  e aggiorna */
/* il valore dello Scale della larghezza dell'icona      */
/* LARGHEZZA */
    scaled_icon_width  = size*2 / STEPPING + 1;
    default_icon_width  = scaled_icon_width  * STEPPING;

/* altezza della label (nome modulo) */
    offset_label = ICON_LABEL_HEIGHT;

    argcount=0;
    XtSetArg(args[argcount],XmNwidth,default_icon_width); argcount++;
    XtSetArg(args[argcount],XmNheight,ICON_LABEL_HEIGHT); argcount++;
    XtSetArg(args[argcount],XmNmarginHeight, 0); argcount++;
    XtSetArg(args[argcount],XmNmarginWidth, 0); argcount++;
    XtSetValues(widget_array[K_ICON_LABEL],args,argcount);

    set_something_val(widget_array[K_ICON_WIDTH],XmNvalue, (XtArgVal) scaled_icon_width);
    set_something_val(widget_array[K_ICON_WIDTH],XmNminimum, (XtArgVal) scaled_icon_width);
    set_something_val(widget_array[K_ICON_WIDTH],XmNmaximum, (XtArgVal) MAX_DIMENSION / STEPPING);

/* ALTEZZA */
    scaled_icon_height = DEFAULT_ICON_HEIGHT;
    default_icon_height = scaled_icon_height * STEPPING;

    set_something_val(widget_array[K_ICON_PIXMAP],XmNheight, (XtArgVal) default_icon_height);
    set_something_val(widget_array[K_ICON_PIXMAP],XmNwidth, (XtArgVal) default_icon_width);

    set_something_val(widget_array[K_ICON_HEIGHT],XmNvalue, (XtArgVal) scaled_icon_height);
    set_something_val(widget_array[K_ICON_HEIGHT],XmNminimum, (XtArgVal) scaled_icon_height);
    set_something_val(widget_array[K_ICON_HEIGHT],XmNmaximum, (XtArgVal) MAX_DIMENSION / STEPPING);

/* dimensioni icona nulle ... */
    icon_height = 0;
    icon_width = 0;
    set_something_val(widget_array[K_BBOARD_ICON], XmNwidth, (XtArgVal) icon_width );
    set_something_val(widget_array[K_BBOARD_ICON], XmNheight, (XtArgVal) icon_height );
get_something(widget_array[K_ICON_PIXMAP], XmNheight, (void*) &bbh);
get_something(widget_array[K_ICON_PIXMAP], XmNwidth, (void*) &bbw);
get_something(widget_array[K_ICON_LABEL], XmNheight, (void*) &bbh2);
get_something(widget_array[K_ICON_LABEL], XmNwidth, (void*) &bbw2);
}

/*-----------------------------------------------------------------------*/
/*** symbol_defaults()
 *** Imposta i default quando viene selezionata la voce di menu
 *** EDIT > Symbols
 ***/
void symbol_defaults()
{
    Dimension bbh,bbw,bbh2,bbw2;

    offset_label = 0; /* non c'e la label nei simboli ... */

    argcount=0;
    XtSetArg(args[argcount],XmNwidth,SYMBOL_MIN_DIMENSION); argcount++;
    XtSetArg(args[argcount],XmNheight, 0); argcount++;
    XtSetArg(args[argcount],XmNmarginHeight,0); argcount++;
    XtSetArg(args[argcount],XmNmarginWidth, 0); argcount++;
    XtSetArg(args[argcount],XmNlabelString,cstring_null); argcount++;
    XtSetValues(widget_array[K_ICON_LABEL],args,argcount);

/* recupera la dimensione della label (potrebbe non essere 0) */
    get_something(widget_array[K_ICON_LABEL], XmNheight, (void*) &offset_label);

    argcount=0;
    XtSetArg(args[argcount],XmNlabelPixmap,XmUNSPECIFIED_PIXMAP);argcount++; 
    XtSetArg(args[argcount],XmNwidth, SYMBOL_MIN_DIMENSION); argcount++;
    XtSetArg(args[argcount],XmNheight,SYMBOL_MIN_DIMENSION); argcount++;
    XtSetValues(widget_array[K_ICON_PIXMAP],args,argcount);

    argcount=0;
    XtSetArg(args[argcount],XmNwidth,
	     SYMBOL_MIN_DIMENSION*STEPPING); argcount++;
    XtSetArg(args[argcount],XmNheight,
	     SYMBOL_MIN_DIMENSION*STEPPING); argcount++;
    XtSetValues(widget_array[K_BBOARD_ICON],args,argcount);

/* Fissa la larghezza della label dell'icona  e aggiorna */
/* il valore dello Scale della larghezza dell'icona      */
/* LARGHEZZA */
    set_something_val(widget_array[K_ICON_WIDTH],XmNminimum, (XtArgVal) SYMBOL_MIN_DIMENSION);
    set_something_val(widget_array[K_ICON_WIDTH],XmNvalue, (XtArgVal) SYMBOL_MIN_DIMENSION);
/* ALTEZZA */
    set_something_val(widget_array[K_ICON_HEIGHT],XmNminimum, (XtArgVal) SYMBOL_MIN_DIMENSION);
    set_something_val(widget_array[K_ICON_HEIGHT],XmNvalue, (XtArgVal) SYMBOL_MIN_DIMENSION);

   icon_width  = SYMBOL_MIN_DIMENSION * STEPPING;
   icon_height = SYMBOL_MIN_DIMENSION * STEPPING;
   centra_icona_dinamica(WIDTH);
   centra_icona_dinamica(HEIGHT);

/* Aggiorna le label sulla main window */
   set_label(widget_array[K_MODULE_NAME],"<SYMBOL>");
   set_label(widget_array[K_MODULE_DESCRIPTION],
             "<Symbol for modules connection>");
}

init_mrm()
{
   char *path_uid, buffer[40];

#ifndef MAIN

   extern Widget UxTopLevel;

   toplevel_widget = UxTopLevel;

#endif

/* Costruisci il path corretto per il file UID ed i files BITMAP */
#ifdef VMS

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
      s_error( APPLICATION_NAME, error_mesg, ESETUIDPATH, 1 );

/*  Registrazione dei nomi delle funzioni di callback */
   MrmRegisterNames(reglist, reglist_num);
   if (MrmFetchWidget(s_RMHierarchy,"main_window",toplevel_widget,
		      &main_window_widget, &dummy_class) != MrmSUCCESS )
      s_error( APPLICATION_NAME, error_mesg, EOPENHIER, 1 );
}

/* fine edicole.c */
