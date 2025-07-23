
/*******************************************************************************
	mainWindow1.c
	(Generated from interface file mainWindow1.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/CascadeB.h>
#include <Xm/ToggleB.h>
#include <Xm/CascadeBG.h>
#include <Xm/Text.h>
#include <Xm/Frame.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/LabelG.h>
#include <Xm/SeparatoG.h>
#include <Xm/Label.h>
#include <Xm/PanedW.h>
#include <Xm/Form.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo mainWindow1.i
   tipo 
   release 2.24
   data 5/9/95
   reserved @(#)mainWindow1.i	2.24
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif

/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/

#ifndef DESIGN_TIME
#include <stdlib.h>
#endif
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <Xm/Xm.h>

#ifndef LIBUTILX
#include <libutilx.h>
#define LIBUTILX
#endif

#ifndef LIBICOFILE
#include <file_icone.h>
#define LIBICOFILE
#endif

#include "definizioni.h"

/* 12-1-95 Micheletti */
#include <aggiunte.h>

/* hyperhelp e' solo su decalpha */
#ifdef HYPERHELP_USED
#include "winhelp.h"
#endif

#define LIBENV "librarian_debug.cfg"

/****************************************************************/
/* VARIABILI GLOBALI ESTERNE					*/
/****************************************************************/

extern Display *display;
extern Arg args[];
extern Cardinal argcount;

extern char *path_legocad, *path_lego, *path_rego;
extern Boolean nmod_def_managed;
extern Boolean nmod_def_managed;
extern Boolean dbox_fileselect_managed;
extern Boolean dbox_doc_managed;

/* Array di compound strings */
extern XmString cstrings[], cstring, cstring_null;

extern char message[];

/* Variabili globali del modulo */
extern char nome_modulo[], descr_modulo[];

extern IconFileRec record;

extern char file_jac_temp[];
extern Dialog_geometry geom_editor;

extern Widget dialog_test_env;

/****************************************************************/
/* VARIABILI GLOBALI						*/
/****************************************************************/

/* Struttura per l'editing del FORTRAN del modulo. */
/* Per adesso, e' permessa l'editazione di un modulo per volta e quindi */
/* l'array e' dimensionato a 1. Se in futuro si vuole avere piu' finestre */
/* di editing contemporaneamente bisogna dimensionare tale array e apportare */
/* alcune modifiche alla Callback della voce di menu EDIT CODE */
EditFtnStruct edit_struct[1];

/* Nomi dei percorsi delle varie librerie */
char *path_librerie[] = {  PATH_LIBUT_UTE,    PATH_LIBUT_STD,
			   PATH_LIBUTREG_UTE, PATH_LIBUTREG_STD,
			   PATH_LIBREG_UTE,   PATH_LIBREG_STD,
			   PATH_LIBGRAF_UTE,  PATH_LIBGRAF_STD  };


char *font = "-adobe-courier-bold-r-normal--14-140-75-75-m-90-iso8859-1";

/*** Questo tipo di specificazione provoca su Alfa il kill dell'XServer
char *font = "-Adobe-Courier-Bold-R-Normal--14-140-*";
***/
XFontStruct *finfo;
XmFontList fontList;

Find_struct parametri_userlist;

byte tipo_modulo = -1;

Boolean menu_sbloccato;
Boolean fine_edicole = True;
Boolean dbox_code_managed, dbox_listamod_managed = False;

Widget dialog_delete;
Widget weditcode, wwinedit;

/***************************************************/
/* Struttura di callback per gli widget di conferma*/
/***************************************************/

static Elenco_callback cancella_modulo = {
	{"Yes", delete_module, YES },
	{"No" , delete_module, NO  },
	{ NULL, NULL,   NULL       }};

/***************************************************/
/* Callback editor codice FORTRAN		   */
/***************************************************/

static Elenco_callback funz_edit_code = {
        {"Ok",    edit_code_callback, C_OK },
        {"Apply", edit_code_callback, C_APPLY },
        {"Cancel",edit_code_callback, C_CANCEL }};

/*******************************************************************************
	The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxmainWindow1MenuPost( wgt, client_data, event, ctd )
	Widget		wgt;
	XtPointer	client_data;
	XEvent		*event;
	Boolean		*ctd;
{
	Widget	menu = (Widget) client_data;
	int 	which_button;

	XtVaGetValues( menu, XmNwhichButton, &which_button, NULL );

	if ( event->xbutton.button == which_button )
	{
		XmMenuPosition( menu, event );
		XtManageChild( menu );
	}
}

/*******************************************************************************
	The definition of the context structure:
	If you create multiple instances of your interface, the context
	structure ensures that your callbacks use the variables for the
	correct instance.

	For each swidget in the interface, each argument to the Interface
	function, and each variable in the Instance Specific section of the
	Declarations Editor, there is an entry in the context structure.
	and a #define.  The #define makes the variable name refer to the
	corresponding entry in the context structure.
*******************************************************************************/

typedef	struct
{
	Widget	UxmainWindow2;
	Widget	Uxform1;
	Widget	UxpanedWindow1;
	Widget	Uxform10;
	Widget	Uxform3;
	Widget	Uxstatic_module_label;
	Widget	UxseparatorGadget6;
	Widget	Uxform12;
	Widget	Uxlabel_user_path;
	Widget	Uxlabel_path_lib;
	Widget	UxlabelGadget17;
	Widget	UxscrolledWindow13;
	Widget	Uxopt_tipolib;
	Widget	Uxshow_libut_pb;
	Widget	Uxshow_libutreg_pb;
	Widget	Uxshow_libreg_pb;
	Widget	Uxmenu_tipolib;
	Widget	Uxform11;
	Widget	Uxframe1;
	Widget	UxlabelGadget3;
	Widget	UxscrolledWindow2;
	Widget	Uxfinestra_messaggi;
	Widget	Uxmessage_menu;
	Widget	Uxmenu1_p1_title;
	Widget	Uxpopup_separator;
	Widget	Uxpopup_pbutton;
	Widget	Uxmenubar;
	Widget	Uxmb_file;
	Widget	Uxpb_quit;
	Widget	Uxmenu1_top_b1;
	Widget	Uxmb_module;
	Widget	Uxmb_call_newmod;
	Widget	Uxpb_create_newmod;
	Widget	Uxpb_modify_newmod;
	Widget	Uxcb_call_newmod;
	Widget	Uxmb_copy;
	Widget	Uxpb_user;
	Widget	Uxpb_stdlib;
	Widget	Uxpb_stdlib_base;
	Widget	Uxpb_stdlib_auxiliary;
	Widget	Uxpb_stdlib_nuclear;
	Widget	Uxpb_stdlib_special;
	Widget	Uxcb_copy;
	Widget	Uxpb_edit_ftn;
	Widget	Uxpb_delete;
	Widget	Uxmenu1_top_b2;
	Widget	Uxmb_icon;
	Widget	Uxpb_edicole;
	Widget	Uxmenubar_top_b1;
	Widget	Uxmb_doc;
	Widget	Uxpb_view;
	Widget	Uxmenubar_top_b2;
	Widget	Uxmb_compile;
	Widget	Uxpb_library;
	Widget	Uxmenubar_top_b3;
	Widget	Uxmb_options;
	Widget	Uxtb_debug;
	Widget	Uxmb_force;
	Widget	Uxmb_force_all;
	Widget	Uxmb_force_selected;
	Widget	Uxmb_options_b3;
	Widget	Uxmenubar_top_b4;
} _UxCmainWindow1;

#define mainWindow2             UxMainWindow1Context->UxmainWindow2
#define form1                   UxMainWindow1Context->Uxform1
#define panedWindow1            UxMainWindow1Context->UxpanedWindow1
#define form10                  UxMainWindow1Context->Uxform10
#define form3                   UxMainWindow1Context->Uxform3
#define static_module_label     UxMainWindow1Context->Uxstatic_module_label
#define separatorGadget6        UxMainWindow1Context->UxseparatorGadget6
#define form12                  UxMainWindow1Context->Uxform12
#define label_user_path         UxMainWindow1Context->Uxlabel_user_path
#define label_path_lib          UxMainWindow1Context->Uxlabel_path_lib
#define labelGadget17           UxMainWindow1Context->UxlabelGadget17
#define scrolledWindow13        UxMainWindow1Context->UxscrolledWindow13
#define opt_tipolib             UxMainWindow1Context->Uxopt_tipolib
#define show_libut_pb           UxMainWindow1Context->Uxshow_libut_pb
#define show_libutreg_pb        UxMainWindow1Context->Uxshow_libutreg_pb
#define show_libreg_pb          UxMainWindow1Context->Uxshow_libreg_pb
#define menu_tipolib            UxMainWindow1Context->Uxmenu_tipolib
#define form11                  UxMainWindow1Context->Uxform11
#define frame1                  UxMainWindow1Context->Uxframe1
#define labelGadget3            UxMainWindow1Context->UxlabelGadget3
#define scrolledWindow2         UxMainWindow1Context->UxscrolledWindow2
#define finestra_messaggi       UxMainWindow1Context->Uxfinestra_messaggi
#define message_menu            UxMainWindow1Context->Uxmessage_menu
#define menu1_p1_title          UxMainWindow1Context->Uxmenu1_p1_title
#define popup_separator         UxMainWindow1Context->Uxpopup_separator
#define popup_pbutton           UxMainWindow1Context->Uxpopup_pbutton
#define menubar                 UxMainWindow1Context->Uxmenubar
#define mb_file                 UxMainWindow1Context->Uxmb_file
#define pb_quit                 UxMainWindow1Context->Uxpb_quit
#define menu1_top_b1            UxMainWindow1Context->Uxmenu1_top_b1
#define mb_module               UxMainWindow1Context->Uxmb_module
#define mb_call_newmod          UxMainWindow1Context->Uxmb_call_newmod
#define pb_create_newmod        UxMainWindow1Context->Uxpb_create_newmod
#define pb_modify_newmod        UxMainWindow1Context->Uxpb_modify_newmod
#define cb_call_newmod          UxMainWindow1Context->Uxcb_call_newmod
#define mb_copy                 UxMainWindow1Context->Uxmb_copy
#define pb_user                 UxMainWindow1Context->Uxpb_user
#define pb_stdlib               UxMainWindow1Context->Uxpb_stdlib
#define pb_stdlib_base          UxMainWindow1Context->Uxpb_stdlib_base
#define pb_stdlib_auxiliary     UxMainWindow1Context->Uxpb_stdlib_auxiliary
#define pb_stdlib_nuclear       UxMainWindow1Context->Uxpb_stdlib_nuclear
#define pb_stdlib_special       UxMainWindow1Context->Uxpb_stdlib_special
#define cb_copy                 UxMainWindow1Context->Uxcb_copy
#define pb_edit_ftn             UxMainWindow1Context->Uxpb_edit_ftn
#define pb_delete               UxMainWindow1Context->Uxpb_delete
#define menu1_top_b2            UxMainWindow1Context->Uxmenu1_top_b2
#define mb_icon                 UxMainWindow1Context->Uxmb_icon
#define pb_edicole              UxMainWindow1Context->Uxpb_edicole
#define menubar_top_b1          UxMainWindow1Context->Uxmenubar_top_b1
#define mb_doc                  UxMainWindow1Context->Uxmb_doc
#define pb_view                 UxMainWindow1Context->Uxpb_view
#define menubar_top_b2          UxMainWindow1Context->Uxmenubar_top_b2
#define mb_compile              UxMainWindow1Context->Uxmb_compile
#define pb_library              UxMainWindow1Context->Uxpb_library
#define menubar_top_b3          UxMainWindow1Context->Uxmenubar_top_b3
#define mb_options              UxMainWindow1Context->Uxmb_options
#define tb_debug                UxMainWindow1Context->Uxtb_debug
#define mb_force                UxMainWindow1Context->Uxmb_force
#define mb_force_all            UxMainWindow1Context->Uxmb_force_all
#define mb_force_selected       UxMainWindow1Context->Uxmb_force_selected
#define mb_options_b3           UxMainWindow1Context->Uxmb_options_b3
#define menubar_top_b4          UxMainWindow1Context->Uxmenubar_top_b4

static _UxCmainWindow1	*UxMainWindow1Context;

Widget	mainWindow1;
Widget	selected_module_label;
Widget	lista_moduli_utente;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_mainWindow1();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*** get_debug()
 ***
 ***/
get_debug()
{
   extern int scrivi_messaggio();
   char varenv[256];
   FILE *fenv; 

   strcpy(varenv,"");

   if( (fenv = fopen(LIBENV,"r")) != NULL)
   {
       fscanf(fenv,"%s",varenv);
       fclose(fenv);
   }
   else
   {
      if( getenv("LEGOCAD_MOD_DEBUG") != NULL )
         strcpy(varenv,getenv("LEGOCAD_MOD_DEBUG")); 
   } 

   if( strcmp(varenv,"YES") == 0 )
   {
       XmToggleButtonSetState(tb_debug,True,True);
   }
   else if( strcmp(varenv,"NO") == 0 )
       XmToggleButtonSetState(tb_debug,False,True);
   else
   {
       scrivi_messaggio("Environment variable LEGOCAD_MOD_DEBUG  not set assume default = YES"); 
       XmToggleButtonSetState(tb_debug,True,True);
   }

}

/*** set_debug(Boolean)
 ***
 ***/

set_debug(Boolean yes)
{
  Boolean cista=False;
  char *dove= NULL;
  char varenv[256]; 
  static char var[256],var2[256];


  strcpy(varenv,"");
  if( getenv("F_FLAGS") != NULL)
     strcpy(varenv,getenv("F_FLAGS")); 
  strcpy(var,"");
  strcpy(var2,"");

  if( (dove=strstr(varenv,"-g")) != NULL )
     cista = True;

  if(yes && !cista )
  {
     strcat(varenv," -g");
  }
  else if( !yes && cista )
  {
     dove[0] = ' ';
     dove[1] = ' ';    
  }
   
  sprintf(var,"F_FLAGS=%s",varenv);

  putenv(var);

  if( yes )
    sprintf(var2,"LEGOCAD_MOD_DEBUG=YES");
  else 
    sprintf(var2,"LEGOCAD_MOD_DEBUG=NO");

printf("settata LEGOCAD_MOD_DEBUG=%s\n",getenv("LEGOCAD_MOD_DEBUG"));

  putenv(var2);

}
    
force_compile(Boolean yes)
{
   char path[200];

   percorso_libreria(path,LIBUT,LIB_UTENTE);
}
 
/*** scrivi_messaggio(message)
 ***  Parametri : 
 ***    char *message: messaggio da visualizzare.
 ***
 ***  Descrizione:
 ***    Visualizza il messaggio nella scroll-list dei messaggi della
 ***    main-window.
 ***/
scrivi_messaggio(message)
char *message;
{
    XmTextPosition pos;

    pos = XmTextGetLastPosition(UxGetWidget(finestra_messaggi));
    XmTextInsert(UxGetWidget(finestra_messaggi),pos, message);
    XmTextInsert(UxGetWidget(finestra_messaggi),pos+strlen(message),"\n");
    XmTextSetInsertionPosition(UxGetWidget(finestra_messaggi),
                               pos+strlen(message)+1);
}

/*** menu_modulo_non_attivo()
 *** 
 *** Descrizione:
 ***   Disattiva alcune voci di menu della main-window nel caso in cui non
 ***   e' selezionato nessun modulo.
 ***/
menu_modulo_non_attivo ()
{

   set_something_val (UxGetWidget(pb_edit_ftn),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_modify_newmod),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_delete),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_edicole),XmNsensitive, (XtArgVal) False);
/***
   set_something_val (UxGetWidget(pb_library),XmNsensitive, (XtArgVal) False);
***/
   menu_sbloccato = False;
}


/*** menu_modulo_attivo()
 ***
 *** Descrizione:
 ***   Attiva alcune voci di menu della main-window nel caso in cui si e'
 ***   selezionato un modulo.
 ***/
menu_modulo_attivo ()
{
   set_something_val (UxGetWidget(pb_edit_ftn),XmNsensitive, (XtArgVal) True);
   set_something_val (UxGetWidget(pb_delete),XmNsensitive, (XtArgVal) True);
   set_something_val (UxGetWidget(pb_edicole),XmNsensitive, (XtArgVal) True);
   set_something_val (UxGetWidget(pb_library),XmNsensitive, (XtArgVal) True);

/* Abilita la modifica con la NEWMOD nel caso dei moduli di processo */
   if (tipo_modulo == LIBUT)
      set_something_val (UxGetWidget(pb_modify_newmod),XmNsensitive, (XtArgVal) True);

   menu_sbloccato = True;
}

/*** delete_module(w,risposta,call_data)
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
void delete_module(w,risposta,call_data)
Widget w;
int risposta;
XmAnyCallbackStruct *call_data;
{
   char file_to_cancel[256];

   if (risposta == YES)
   {
   /* Compongo il nome del file FORTRAN da cancellare */
      componi_file_modulo(file_to_cancel,nome_modulo,FILE_FTN,tipo_modulo,
                          LIB_UTENTE);

   /* Cancello il modulo FORTRAN */
      if (!unlink(file_to_cancel))
      {
         sprintf (message," Module %s erased !", nome_modulo);
         scrivi_messaggio(message);
      }
      else
      {
         sprintf(message," Couldn't delete file %s. Failure.",file_to_cancel);
         scrivi_messaggio (message);
      }

   /* Cancellazione del file INTERFACCIA nel caso libreria moduli processo */
   /* (LIBUT) */
      if (tipo_modulo == LIBUT)
      {
         componi_file_modulo(file_to_cancel,nome_modulo,FILE_IFACE,tipo_modulo,
                             LIB_UTENTE);

         if (access(file_to_cancel, F_OK) == 0)
            unlink(file_to_cancel);
      }

   /* Cancellazione dei files con estensione DAT e _01.DAT nel caso libreria */
   /* moduli di regolazione (LIBUTREG) */
      if (tipo_modulo == LIBUTREG)
      {
         componi_file_modulo(file_to_cancel,nome_modulo,FILE_DAT,tipo_modulo,
                             LIB_UTENTE);

         unlink(file_to_cancel);

         componi_file_modulo(file_to_cancel,nome_modulo,FILE_01DAT,tipo_modulo,
                             LIB_UTENTE);

         unlink(file_to_cancel);
      }

   /* Cancello la riga corrispondente nel file lista moduli (processo o */
   /* regolazione). */
      if ( cancella_modulo_lista(tipo_modulo) )
         scrivi_messaggio (" Module row undeleted. Failure.");

   /*************************************************************************/
   /*   N.B. CANCELLARE ANCHE ICONE E RECORD ICON_LIST (moduli di PROCESSO) */
   /*************************************************************************/
      if ( tipo_modulo == LIBUT )
         if (cancella_record_file_ico(nome_modulo,LIB_UTENTE) == RETURN_ERROR)
            scrivi_messaggio (" Icon record undeleted. Failure.\n");

   /* nessun modulo selezionato */
      unselect_module();

   /* Visualizza la lista dei moduli aggiornata */
      cambia_tipo_modulo(NULL, tipo_modulo, NULL);
   }

   XtDestroyWidget(dialog_delete);
}

/*** cancella_modulo_lista(tipo_lib)
 ***   Parametri:
 ***     byte tipo_lib : tipo libreria (LIBUT, LIBUTREG o LIBREG)
 ***
 ***   Descrizione:
 ***     Cancellazione della riga di un modulo nel file lista moduli
 ***     (processo o regolazione).
 ***/
cancella_modulo_lista(tipo_lib)
byte tipo_lib;
{
   FILE *fp, *fp_tmp;
   char buffer[81], file_tmp[256], file_lista[256];
   char flag;

/* recupera il nome del file dati della libreria corretta */
   file_dati_lib(file_lista, tipo_lib, LIB_UTENTE);

   if ((fp = fopen(file_lista,"r")) == NULL)
   {
      sprintf(buffer,"Can't open file %s for deleting line.",FILE_DATI_LIBUT);
      scrivi_messaggio(buffer);
      return (-1);
   }

   if ((fp_tmp = fopen(tmpnam(file_tmp),"w+")) == NULL)
   {
      scrivi_messaggio("Can't open temporary file for deleting line.");
      return (-1);
   }

   while (fgets(buffer,80,fp) != NULL)
      if (strncmp(buffer,nome_modulo,4))
         fputs (buffer,fp_tmp);

   fclose (fp);
   fclose (fp_tmp);

   flag = copia_file (file_tmp, file_lista);
   unlink (file_tmp);

   if ( flag != 0)
   {
      scrivi_messaggio ("Couldn't hold file copy. Failure.");
      return (-1);
   }

   return (0);
}


/*------------------------------------------------------------
 *  esiste_foraux
 *
 *   carica in meoria il file fortan del modulo 'nome_modulo' 
 *   dalla libreria utente e verifica se e' presente il 
 *   fortran ausiliario.
 *
 *----------------------------------------------------------*/
 
int esiste_foraux(char *nome_modulo)
{
   FILE *fp;
   char *pattern = {"~FORAUS_"};
   char *stringa=NULL,*ptr2=NULL;
   char nomefile[200];
   struct stat info_file;

   componi_file_modulo(nomefile, nome_modulo, FILE_FTN, LIBUT, LIB_UTENTE);
   if( (fp = fopen(nomefile,"r")) == NULL)
   {
      fclose(fp);
      return(False);
   }

   if ( !fstat(fileno(fp),&info_file))
   {
      stringa = (char *) XtMalloc ((unsigned) info_file.st_size+1);
   }
   else
      stringa = (char *) XtMalloc (sizeof(char) * 100000);

/* leggo tutto il file */
   ptr2 = stringa;
   while (fgets(ptr2,140,fp) != NULL)
      ptr2 = ptr2+strlen(ptr2);

   if( strstr(stringa,pattern) != NULL)
   {
       printf("\n\n\tTROVATO PATTERN IN %s\n",nomefile);
       fclose(fp);
       free(stringa);
       return(True);
   }
   else
   {
      printf("\n\n\tNON TROVATO PATTERN IN %s\n",nomefile);
      fclose(fp);
      free(stringa);
      return(False);
   }

}



/*** aggiungi_modulo_lista(tipo_lib, nome, descr, tag)
 ***  Parametri:
 ***    byte tipo_lib : tipo libreria (LIBUT,LIBUTREG,LIBREG)
 ***    char *nomefile : nome del file lista moduli (processo o regolazione)
 ***    char *nome, *descr : nome modulo e descrizione
 ***    Boolean tag : indica se bisogna inserire il tag nella riga (car. !)
 ***
 ***  Descrizione:
 ***    Inserimento o aggiornamento della riga di un modulo nel file lista dei
 ***    moduli della libreria appropriata (LIBUT,LIBUTREG o LIBREG).
 ***/
aggiungi_modulo_lista(tipo_lib, nome, descr, tag)
byte tipo_lib;
char *nome, *descr;
Boolean tag;
{
   FILE *fp, *fp_tmp;
   char buffer[81], nuova_riga[81], flag;
   char file_tmp[256], file_lista[256];
   Boolean modulo_inserito = False;

/* recupera il nome del file dati della libreria corretta */
   file_dati_lib(file_lista, tipo_lib, LIB_UTENTE);

   if ((fp = fopen(file_lista,"r")) == NULL)
   {
      sprintf(buffer,"Can't open file %s for inserting line.",FILE_DATI_LIBUT);
      scrivi_messaggio(buffer);
      return (-1);
   }

   if ((fp_tmp = fopen(tmpnam(file_tmp),"w+")) == NULL)
   {
      scrivi_messaggio("Can't open file temporary file for inserting line.");
      return (-1);
   }

/* Composizione riga da inserire nella lista */
   sprintf (nuova_riga,"%s  %s\n",nome,descr);

/* se e' presente fortran ausiliario aggiungo il simbolo * idopo il nome */
   if( esiste_foraux(nome) )
     nuova_riga[4] = '*';

/* Se si sta copiando da un altro utente, settare il tag '!' */
   if (tag)
      nuova_riga[5] = '!';

   while (fgets(buffer,80,fp) != NULL)
   {
      if ( !modulo_inserito )
         if ( strncmp (buffer,nome,4) >= 0 )
         {
            fputs (nuova_riga,fp_tmp);
            modulo_inserito = True;

         /* Se il modulo e' uguale allora passa al modulo successivo */
            if (strncmp (buffer,nome,4) == 0)
               if (fgets(buffer,80,fp) == NULL)
                  break;
         }
      fputs (buffer,fp_tmp);
   }

/* Se la nuova riga va inserita dopo l'ultimo dei moduli */
   if ( !modulo_inserito )
      fputs (nuova_riga,fp_tmp);

   fclose (fp);
   fclose (fp_tmp);

   flag = copia_file (file_tmp,file_lista);
   unlink (file_tmp);

   if ( flag != 0)
   {
      scrivi_messaggio ("Copy failure. Modules list not updated.\n");
      return (-1);
   }

   return (0);
}

/***
 *** controlla_lista_moduli(tipo_lib, nome_inserito, offset_listamod)
 ***   Parametri:
 ***     byte tipo_lib : tipo libreria (LIBUT, LIBUTREG, LIBREG)
 ***     char *nome_inserito : nome del modulo
 ***     long *offset_listamod : offset del file lista moduli dove si trova il
 ***                             modulo (se esiste). (uscita)
 ***
 ***   Descrizione:
 ***     La funzione controlla se il nome di un modulo e' gia' presente nel file
 ***     lista dei moduli dell'utente.Viene chiamata per tutte e due le attivita'
 ***     di copia di un modulo (da altro utente o da libreria standard).
 ***
 ***   VALORI RITORNATI:
 ***     0	tutto ok: il nome di modulo non si trova nella lista
 ***     1	il modulo esiste ed ha il tag '!' 
 ***    -1	il modulo esiste e non ha il tag '!' 
 ***/
controlla_lista_moduli(tipo_lib, nome_inserito, offset_listamod)
byte tipo_lib;
char *nome_inserito;
long *offset_listamod;
{
  extern Dialog_geometry geom_attention;
  extern Widget attention_wdg, UxTopLevel;
  FILE *fp_lista;
  int  value = 0;
  char baffer[81], file_lista[256];

/* Apertura file dati  in lettura */
  file_dati_lib(file_lista, tipo_lib, LIB_UTENTE);
  if ( (fp_lista = fopen (file_lista,"r")) == NULL)
  {
     sprintf(message," Can't open data file for checking module names");
     attention_wdg = (Widget) attention (UxTopLevel, message, MAPPA,
					 geom_attention);
     scrivi_messaggio(message);
     return;
  }

  /* Controllo che il nome del modulo non esista gia' */
  while ( fgets(baffer,80,fp_lista) != NULL)
  {
     if (!strncmp(baffer,nome_inserito,4))
     {
     /* Se c'e' anche il tag '!' NON bisogna copiare */
        if (baffer[5] == '!')
           value = 1;
        else /* Se non c'e' il tag il file va copiato */
           value = -1;

      /* Setto l'offset */
         *offset_listamod = ftell (fp_lista) - strlen(baffer);
         fclose(fp_lista);
         return (value); 
     }
  }

  fclose(fp_lista);
  return (value);
}

/*** end_edicole(sig, code, scp, addr)
 ***
 ***  Descrizione:
 ***    Registra l'eventuale uscita dal processo EDICOLE,
 ***    mentre si e' ancora in LIBRARIAN. Viene chiamata dal signal SIGCLD.
 ***/
void end_edicole(sig, code, scp, addr)
int sig, code;
struct sigcontext *scp;
char *addr;
{
   int status;

/* Per eliminare la folla di ZOMBIE */
   wait(&status);

   fine_edicole = True;
}

/*** void cambia_tipo_modulo (w,tipo,zip)
 ***   Parametri:
 ***     Widget w: widget
 ***     byte tipo : tipo libreria selezionata (LIBUT,LIBUTREG,LIBREG)
 ***     XmPushButtonCallbackStruct *zip: non utilizzato.
 ***
 ***   Descrizione:
 ***     Callback invocata agendo sull'optionMenu del tipo di moduli
 ***     (processo o regolazione).
 ***/
void cambia_tipo_modulo (w,tipo,zip)
Widget w;
byte tipo;
XmPushButtonCallbackStruct *zip;
{
   FILE *fp;
   char nomefile[256];

   if (tipo == tipo_modulo && zip != NULL)
      return;

   switch (tipo)
   {
      case LIBUT:
         percorso_libreria(nomefile, LIBUT, LIB_UTENTE);
         set_label(UxGetWidget(label_path_lib), nomefile);
         set_something_val(UxGetWidget(pb_create_newmod),XmNsensitive, (XtArgVal) True);

/*11-1-95 Micheletti */
         /* set_something_val(UxGetWidget(pb_stdlib),XmNsensitive, (XtArgVal) True); */
         set_something_val(UxGetWidget(pb_stdlib),XmNsensitive, (XtArgVal) False); 
         set_something_val(UxGetWidget(pb_stdlib_base),XmNsensitive, (XtArgVal) True);
         set_something_val(UxGetWidget(pb_stdlib_auxiliary),XmNsensitive, (XtArgVal) True );
         set_something_val(UxGetWidget(pb_stdlib_nuclear),XmNsensitive, (XtArgVal) True );
         set_something_val(UxGetWidget(pb_stdlib_special),XmNsensitive, (XtArgVal) True );

         strcat(nomefile, FILE_DATI_LIBUT);
      break;

      case LIBUTREG:
         percorso_libreria(nomefile, LIBUTREG, LIB_UTENTE);
         set_label(UxGetWidget(label_path_lib), nomefile);
         set_something_val(UxGetWidget(pb_create_newmod),XmNsensitive, (XtArgVal) False);
         set_something_val(UxGetWidget(pb_stdlib),XmNsensitive, (XtArgVal) False);

/*11-1-95 Micheletti */
         set_something_val(UxGetWidget(pb_stdlib_base),XmNsensitive, (XtArgVal) False);
         set_something_val(UxGetWidget(pb_stdlib_auxiliary),XmNsensitive, (XtArgVal) False);
         set_something_val(UxGetWidget(pb_stdlib_nuclear),XmNsensitive, (XtArgVal) False);
         set_something_val(UxGetWidget(pb_stdlib_special),XmNsensitive, (XtArgVal) False);

         strcat(nomefile, FILE_DATI_LIBUTREG);
      break;

      case LIBREG:
         percorso_libreria(nomefile, LIBREG, LIB_UTENTE);
         set_label(UxGetWidget(label_path_lib), nomefile);
         set_something_val(UxGetWidget(pb_create_newmod),XmNsensitive, (XtArgVal) False);
         set_something_val(UxGetWidget(pb_stdlib),XmNsensitive, (XtArgVal) True);

/*11-1-95 Micheletti */
         set_something_val(UxGetWidget(pb_stdlib_base),XmNsensitive, (XtArgVal) False);
         set_something_val(UxGetWidget(pb_stdlib_auxiliary),XmNsensitive, (XtArgVal) False);
         set_something_val(UxGetWidget(pb_stdlib_nuclear),XmNsensitive, (XtArgVal) False);
         set_something_val(UxGetWidget(pb_stdlib_special),XmNsensitive, (XtArgVal) False);

         strcat(nomefile, FILE_DATI_LIBREG);
      break;
   }

   tipo_modulo = tipo;
   XmListDeselectAllItems(UxGetWidget(lista_moduli_utente));
   XmListDeleteAllItems(UxGetWidget(lista_moduli_utente));

   if ( (fp = fopen(nomefile, "r")) == NULL )
   {
       sprintf(message, "Can't open file %s.",nomefile); 
       scrivi_messaggio (message);
       return;
   }

/* Visualizza la lista dei moduli selezionata (processo o regolazione) */
   riempi_lista_moduli (fp,UxGetWidget(lista_moduli_utente));
   fclose (fp);
}

/*** void edit_code_callback(w, par, data)
 ***   Parametri:
 ***     Widget w: widget.
 ***     int par: bottone premuto (C_OK, C_APPLY, C_CANCEL)
 ***     XmAnyCallbackStruct *data: non utilizzato.
 ***
 ***   Descrizione:
 ***     Funzione di callback per l'editor del codice FORTRAN ***/
void edit_code_callback(w, par, data)
Widget w;
int par;
XmAnyCallbackStruct *data;
{
   Text_find_struct *ptr;
   char *buffer_to_save, nomefile[256];
   FILE *fp_ftn;
   long lung, offset;
   EditFtnStruct *edit_data;

   if (par == C_OK || par == C_APPLY)
   {   
   /* struttura edit del modulo */
      get_something(weditcode, XmNuserData, (void*) &edit_data);

   /* Percorso del file FORTRAN da editare. */
      componi_file_modulo(nomefile,edit_data->nome_mod,FILE_FTN,
                          edit_data->tipo_lib,LIB_UTENTE);
      printf ("salva su file : %s\n", nomefile);
     
      if ((fp_ftn = fopen (nomefile,"w+")) == NULL)
      {
         sprintf (message,"Can't open file %s for updating.",nomefile);
         scrivi_messaggio (message);
      }
      else
      {
      /* Salva il contenuto del text-widget sul file */
         buffer_to_save = XmTextGetString(weditcode);
         lung = strlen(buffer_to_save);

      /* Se e' stato accidentalmente cancellato */
      /* inserisce il carattere di fine riga */
         if ( buffer_to_save[lung-1] != '\n')
            strcat(buffer_to_save,"\n");

         fputs (buffer_to_save, fp_ftn);
         fclose (fp_ftn);
         XtFree (buffer_to_save);

      /* Setta il tag '!' alla posizione 6  */   
      /* Recupera l'offset del nome del modulo nel file lista selezionato */
      /* (LIBUT, LIBUTREG, LIBREG) */
         if (controlla_lista_moduli (edit_data->tipo_lib, edit_data->nome_mod,
                                     &offset) == -1)
            apponi_tag_record(edit_data->tipo_lib, offset);
      }
   }

/* Chiudi la window di editor se necessario (Ok o Cancel) */
   if (par == C_CANCEL || par == C_OK)
   {

   /* Dealloca la struttura utlizzata dall'editor per il find */
      get_something(wwinedit, XmNuserData, (void*) &ptr);
      XtFree(ptr);

      dbox_code_managed = False;
      XtDestroyWidget(wwinedit);
   }
}

/*** unselect_module()
 ***    Nessun parametro.
 ***
 ***    Descrizione:
 ***      La funzione viene chiamata per deselezionare il modulo corrente 
 ***/
unselect_module()
{
/* Riporto la label del modulo selezionato a '<none>'  */
   set_label(UxGetWidget(selected_module_label),"<none>"); 

/* Svuoto le variabili globali di nome e descrizione modulo */
   nome_modulo[0]  = '\0';
   descr_modulo[0] = '\0';

/* Blocco i menu */
   menu_modulo_non_attivo();
}

/*** char *componi_file_modulo(nomefile, nome_mod, tipo_file, tipo_lib,
 ***                           quale_lib)
 ***   Parametri:
 ***     char *nomefile : nome del file (uscita)
 ***     char *nome_mod : nome del modulo
 ***     byte tipo_file : tipo file (FORTRAN, DOC, INTERFACCIA, DATI)
 ***     byte tipo_lib : tipo libreria (LIBUT, LIBUTREG o LIBREG)
 ***     byte quale_lib : quale libreria: utente (LIB_UTENTE) oppure
 ***                               standard (LIB_STANDARD).
 ***
 ***   Descrizione:
 ***     Restituisce il nome del file dati corretto.
 ***/
char *componi_file_modulo(nomefile, nome_mod, tipo_file, tipo_lib, quale_lib)
char *nomefile, *nome_mod;
byte tipo_file, tipo_lib, quale_lib;
{
   char minus_nome[10];

   strcpy(minus_nome, nome_mod);
   tominus(minus_nome);

   switch (tipo_file)
   {
      case FILE_FTN:
#ifdef VMS
           strcat(minus_nome,".ftn");
#else
           strcat(minus_nome,".f");
#endif
           break;

      case FILE_DOC:
           strcat(minus_nome,".doc");
           break;

      case FILE_IFACE:
           strcat(minus_nome,".i");
           break;

      case FILE_DAT:
           strcat(minus_nome,".dat");
           break;

      case FILE_01DAT:
           strcat(minus_nome,"_01.dat");
           break;
   }

   percorso_libreria(nomefile, tipo_lib, quale_lib);
   strcat(nomefile, minus_nome);
   return(nomefile);
}

/*** char *file_dati_lib(nomefile, tipo_lib, quale_lib)
 ***   Parametri:
 ***     char *nomefile : nome del file (uscita)
 ***     byte tipo_lib : tipo libreria (LIBUT,LIBUTREG,LIBREG,LIBGRAF)
 ***     byte quale_lib : quale libreria: utente (LIB_UTENTE) oppure
 ***                               standard (LIB_STANDARD).
 ***
 ***   Descrizione:
 ***     Restituisce il nome del file dati corretto.
 ***/
char *file_dati_lib(nomefile, tipo_lib, quale_lib)
char *nomefile;
byte tipo_lib, quale_lib;
{
   char *file_lista;

   switch (tipo_lib)
   {
       case LIBUT:
            file_lista = FILE_DATI_LIBUT;
            break;

       case LIBUTREG:
            file_lista = FILE_DATI_LIBUTREG;
            break;

       case LIBREG:
            file_lista = FILE_DATI_LIBREG;
            break;

       case LIBGRAF:
            file_lista = FILE_ICONE_MODP;
            break;

       default:
            return(NULL);
            break;
   }

   percorso_libreria(nomefile, tipo_lib, quale_lib);

   strcat(nomefile,file_lista);
   return(nomefile);
}

/*** char *sottodir_utente(tipo_lib, quale_lib)
 ***   Parametri:
 ***     byte tipo_lib : tipo libreria.
 ***     byte quale_lib : quale libreria (standard o utente?)
 ***
 ***   Descrizione:
 ***     Restituisce il sottodirettorio della directory legocad dell'utente 
 ***/
char *sottodir_utente(tipo_lib, quale_lib)
byte tipo_lib, quale_lib;
{

/* 12-1-95 Micheletti */
/***
   return(path_librerie[2*tipo_lib+quale_lib]);
***/

/*
   questa modifica puo' essere migliorata adattando la matrice
   path_librerie per gestire gli ulteriori casi
   prima di farla pero' e' necessario verificare l'impatto
   sugli altri programmi.
   Inoltre se viene cambiata la matrice deve essere rivista
   la routine percorso_libreria. 
*/ 

   if( (quale_lib == LIB_UTENTE) || (quale_lib == LIB_STANDARD) )
      return(path_librerie[2*tipo_lib+quale_lib]);
   else if( quale_lib == LIB_STANDARD_BASE )
      return( PATH_LIBUT_STD_BASE );
   else if( quale_lib == LIB_STANDARD_AUXILIARY )
      return( PATH_LIBUT_STD_AUXILIARY );
   else if( quale_lib == LIB_STANDARD_NUCLEAR )
      return( PATH_LIBUT_STD_NUCLEAR );
   else if( quale_lib == LIB_STANDARD_SPECIAL )
      return( PATH_LIBUT_STD_SPECIAL );
   else if( quale_lib == LIB_STANDARD_GENERAL )
      return( PATH_LIBUT_STD_GENERAL );
}

/*** char *percorso_libreria(percorso, tipo_lib, quale_lib)
 ***   Parametri:
 ***     char *percorso : percorso libreria (uscita)
 ***     byte tipo_lib : tipo libreria (LIBUT, LIBUTREG o LIBREG).
 ***     byte quale_lib : quale libreria: utente (LIB_UTENTE) oppure
 ***                               standard (LIB_STANDARD).
 ***  Restituisce il giusto percorso della libreria.
 ***/
char *percorso_libreria(percorso, tipo_lib, quale_lib)
char *percorso;
byte tipo_lib, quale_lib;
{
   char *path_base, *subdir;

   if ( (quale_lib == LIB_UTENTE)  ) 
      path_base = path_legocad;
   else if (tipo_lib== LIBREG)
      path_base = path_rego;
   else 
      path_base = path_lego;

   subdir = sottodir_utente(tipo_lib, quale_lib);

#ifdef VMS
   sottodir_vms(percorso, path_base, subdir);
#else
   sprintf(percorso, "%s/%s/", path_base, subdir);
#endif
}

/*** test_environment()
 ***
 ***   Descrizione:
 ***     Attiva il processo make per il controllo dell'ambiente LEGOCAD
 ***     dell'utente.
 ***/
test_environment()
{
    return(esegui_comando(TEST_ENVIRONMENT));
}

/*** crea_environment()
 ***
 ***   Descrizione:
 ***     Attiva il processo make per la creazione dell'ambiente LEGOCAD
 ***     dell'utente extra-vergine.
 ***/
crea_environment()
{
    return(lancia_comando(display,CREA_ENVIRONMENT));
}

/*** copia_std_environment()
 ***
 ***   Descrizione:
 ***     Attiva il processo make per la copia della libreria standard 
 ***     nell'ambiente LEGOCAD dell'utente (vergihe o no).
 ***/
copia_std_environment()
{
    return(lancia_comando(display,COPIA_STD_ENVIRONMENT));
}

/*** cad_crealibut()
 ***
 ***   Descrizione:
 ***      Compila e crea la libreria libut (moduli di processo)
 ***/
cad_crealibut()
{
    return(lancia_comando(display,CREA_LIBUT));
}

/*** cad_crealibut_reg()
 ***
 ***   Descrizione:
 ***      Compila e crea la libreria libut_reg (moduli di regolazione)
 ***/
cad_crealibut_reg()
{
    return(lancia_comando(display,CREA_LIBUT_REG));
}

/*** cad_crealibreg()
 ***
 ***   Descrizione:
 ***      Compila e crea la libreria libreg (moduletti di regolazione)
 ***/
cad_crealibreg()
{
    return(lancia_comando(display,CREA_LIBREG));
}

/*** cad_crealg1()
 ***
 ***   Descrizione:
 ***      Compila e crea l'applicativo LEGOCAD LG1
 ***/
cad_crealg1()
{
    return(lancia_comando(display,CREA_LG1));
}

/*** show_error( mess )
 ***   Parametri:
 ***     char mess: messaggio di errore da visualizzare
 ***
 ***   Descrizione:
 ***      Visualizza un messaggio di errore nella window dei messaggi
 ***      (chiamata da esegui_comando() -> lc_errore())
 ***/
void show_error(mess)
char *mess;
{
   scrivi_messaggio(mess);
}

/*** show_warning( mess )
 ***   Parametri:
 ***     char mess: messaggio di errore da visualizzare
 ***
 ***   Descrizione:
 ***      Visualizza un messaggio di warning nella window dei messaggi
 ***      (chiamata da esegui_comando() -> lc_errore())
 ***/
void show_warning(mess)
char *mess;
{
   scrivi_messaggio(mess);
}

/*** chiudi_prog_legocad()
 ***
 ***   Descrizione:
 ***     Fine applicativo (chiamato da close_prog_legocad() nel file lc_ops.c)
 ***/
chiudi_prog_legocad()
{
   XtCloseDisplay(display);
   exit(1);
}

/*** void conf_test_env(w,risposta,call_data)
 ***  Parametri:
 ***    Widget w: widget
 ***    int risposta : 	0 -> crea ambiente vuoto
 ***		       	1 -> crea e copia tutta la libreria standard
 ***			2 -> esci e torna a casa
 ***    XmAnyCallbackStruct *call_data : non utilizzato.
 ***
 ***  Descrizione:
 ***    Callback chiamata durante il test sull'ambiente LEGOCAD.
 ***/
void conf_test_env(w,risposta,call_data)
Widget w;
int risposta;
XmAnyCallbackStruct *call_data;
{
   switch (risposta)
   {
      case 0 :
         crea_environment();
         break;

      case 1:
         copia_std_environment();

      /* Aggiorna la lista dei moduli */
         cambia_tipo_modulo(NULL, LIB_UTENTE, NULL);
         break;

      case 2:
         chiudi_prog_legocad();
         break;
   }

   XtDestroyWidget(dialog_test_env);
}

#ifdef HYPERHELP_USED
/**
*** void hhelp_doc_modulo
*** parametri: nessuno
*** descrizione: richiama hyperhelp viewer visualizzando
***              la documentazione del modulo selezionato
*** Note: questa funzione viene chiamata solo se la l'eseguibile
***       e' stato generato con -DHYPERHELP_USED
**/

void hhelp_doc_modulo()
{
  static char tok[100];

   if( tipo_modulo == LIBUT )
   {
     printf("Help for Process Module %s\n",nome_modulo);
     sprintf(tok,"%s process module",nome_modulo);

   }
   else if( tipo_modulo ==  LIBREG )
   {
     strcpy(tok,nome_modulo);
     strcat(tok," regulation module");  
     printf("Help for Regulation Module %s\n",nome_modulo);  
     printf("Start HyperHelp with parameter [%s]\n",tok); 
     WinHelp(UxDisplay,"lglibreg.hlp",HELP_KEY,(unsigned long)tok);
   }
}
#endif

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	singleSelectionCB_lista_moduli_utente( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	   int n;
	   XmStringTable items;
	   char *buffer;
	
	   get_something(UxGetWidget(lista_moduli_utente), XmNselectedItems, (void*) &items);
	   get_something(UxGetWidget(lista_moduli_utente), XmNselectedItemCount, (void*) &n);
	
	   if ( n == 0 )
	      unselect_module();
	   else
	   {
	      buffer = extract_string(items[0]);
	      setta_label_modsel(buffer);
	      XtFree(buffer);
	   }
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_popup_pbutton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	XmTextSetString(UxGetWidget(finestra_messaggi),"");
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_quit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	   FILE *fenv;
	
	/* Cancellazione eventuale file jac temporaneo */
	if (!access(file_jac_temp,F_OK))
	   if ( unlink(file_jac_temp) )
	      printf ("Can't delete jacobian temporary file. Failure.\n");
	
	if( (fenv = fopen(LIBENV,"w")) != NULL)
	{
	   
	   fprintf(fenv,"%s",getenv("LEGOCAD_MOD_DEBUG"));
	   fclose(fenv);
	}
	
	XtCloseDisplay(display);
	exit(0);   
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_create_newmod( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	if ( nmod_def_managed )
	{
	    scrivi_messaggio("NEWMOD definition window already in use.");
	    return;
	}
	
	create_nmod_shell(False);
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_modify_newmod( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	char nome_file[256];
	
	if ( nmod_def_managed )
	{
	    scrivi_messaggio("NEWMOD definition window already in use.");
	    return;
	}
	
	/* percorso del file interfaccia (libreria moduli di processo LIBUT) */
	componi_file_modulo(nome_file,nome_modulo,FILE_IFACE,LIBUT,LIB_UTENTE);
	
	if ( access(nome_file, F_OK) )
	{
	    scrivi_messaggio("The module doesn't contain NEWMOD information.");
	    return;
	}
	
	/* Chiama la NEWMOD per le modifiche */
	create_nmod_shell(True);
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_user( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	/* crea la dialog-box di selezione files per la copia */
	if ( !dbox_fileselect_managed )
	   create_file_selection();
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_stdlib( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	  char *path_lib, file_lista[256];
	  FILE *fp;
	
	/* Se la dialogBox di selezione dei moduli */
	/* e' visualizzata, ritorna */
	   if (dbox_listamod_managed)
	   {
	      scrivi_messaggio ("Modules list window already in use.");
	      return;
	   }
	
	/* Composizione percorso e nome file lista dei moduli di libreria */
	
	   file_dati_lib(file_lista, tipo_modulo, LIB_STANDARD);
	printf("tipo_modulo = [%d]\n",tipo_modulo);
	
	
	/* Se il file lista_moduli.dat di libreria non esiste */
	/* ritorna con errore */
	printf("File lista dei moduli [%s]\n",file_lista);
	   if ( (fp = fopen (file_lista, "r")) == NULL )
	   {
	      scrivi_messaggio ("Can't open data file.");
	      return;
	   }
	
	/* Visualizza la lista moduli e setta la reason della lista */
	   create_lista_moduli(fp,tipo_modulo,STDLIB_MODULES_COPY,
	                       &dbox_listamod_managed);
	
	   fclose(fp);
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_stdlib_base( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	  char *path_lib, file_lista[256];
	  FILE *fp;
	
	/* Se la dialogBox di selezione dei moduli */
	/* e' visualizzata, ritorna */
	   if (dbox_listamod_managed)
	   {
	      scrivi_messaggio ("Modules list window already in use.");
	      return;
	   }
	
	/* Composizione percorso e nome file lista_moduli.dat di libreria */
	   file_dati_lib(file_lista, tipo_modulo, LIB_STANDARD_BASE);
	
	/* Se il file lista_moduli.dat di libreria non esiste */
	/* ritorna con errore */
	   if ( (fp = fopen (file_lista, "r")) == NULL )
	   {
	      scrivi_messaggio ("Can't open data file.");
	      return;
	   }
	
	/* Visualizza la lista moduli e setta la reason della lista */
	   create_lista_moduli(fp,tipo_modulo,STDLIBBASE_MODULES_COPY,
	                       &dbox_listamod_managed);
	
	   fclose(fp);
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_stdlib_auxiliary( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	  char *path_lib, file_lista[256];
	  FILE *fp;
	
	/* Se la dialogBox di selezione dei moduli */
	/* e' visualizzata, ritorna */
	   if (dbox_listamod_managed)
	   {
	      scrivi_messaggio ("Modules list window already in use.");
	      return;
	   }
	
	/* Composizione percorso e nome file lista_moduli.dat di libreria */
	   file_dati_lib(file_lista, tipo_modulo, LIB_STANDARD_AUXILIARY);
	
	/* Se il file lista_moduli.dat di libreria non esiste */
	/* ritorna con errore */
	   if ( (fp = fopen (file_lista, "r")) == NULL )
	   {
	      scrivi_messaggio ("Can't open data file.");
	      return;
	   }
	
	/* Visualizza la lista moduli e setta la reason della lista */
	   create_lista_moduli(fp,tipo_modulo,STDLIBAUXILIARY_MODULES_COPY,
	                       &dbox_listamod_managed);
	
	   fclose(fp);
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_stdlib_nuclear( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	  char *path_lib, file_lista[256];
	  FILE *fp;
	
	/* Se la dialogBox di selezione dei moduli */
	/* e' visualizzata, ritorna */
	   if (dbox_listamod_managed)
	   {
	      scrivi_messaggio ("Modules list window already in use.");
	      return;
	   }
	
	/* Composizione percorso e nome file lista_moduli.dat di libreria */
	   file_dati_lib(file_lista, tipo_modulo, LIB_STANDARD_NUCLEAR);
	
	/* Se il file lista_moduli.dat di libreria non esiste */
	/* ritorna con errore */
	   if ( (fp = fopen (file_lista, "r")) == NULL )
	   {
	      scrivi_messaggio ("Can't open data file.");
	      return;
	   }
	
	/* Visualizza la lista moduli e setta la reason della lista */
	   create_lista_moduli(fp,tipo_modulo,STDLIBNUCLEAR_MODULES_COPY,
	                       &dbox_listamod_managed);
	
	   fclose(fp);
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_stdlib_special( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	  char *path_lib, file_lista[256];
	  FILE *fp;
	
	/* Se la dialogBox di selezione dei moduli */
	/* e' visualizzata, ritorna */
	   if (dbox_listamod_managed)
	   {
	      scrivi_messaggio ("Modules list window already in use.");
	      return;
	   }
	
	/* Composizione percorso e nome file lista_moduli.dat di libreria */
	   file_dati_lib(file_lista, tipo_modulo, LIB_STANDARD_SPECIAL);
	
	/* Se il file lista_moduli.dat di libreria non esiste */
	/* ritorna con errore */
	   if ( (fp = fopen (file_lista, "r")) == NULL )
	   {
	      scrivi_messaggio ("Can't open data file.");
	      return;
	   }
	
	/* Visualizza la lista moduli e setta la reason della lista */
	   create_lista_moduli(fp,tipo_modulo,STDLIBSPECIAL_MODULES_COPY,
	                       &dbox_listamod_managed);
	
	   fclose(fp);
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_edit_ftn( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	char nomefile[256];
	static char *font_text = "-Adobe-Courier-Bold-R-Normal--14-140-75-75-M-90-ISO8859-1";
	if (!dbox_code_managed)
	{
	/* Percorso del file FORTRAN da editare. */
	   componi_file_modulo(nomefile,nome_modulo,FILE_FTN,tipo_modulo,LIB_UTENTE);
	   printf ("FILE FTN: %s\n",nomefile);
	
	   wwinedit = (Widget) editor(UxTopLevel,&weditcode,nomefile,True,font_text,
	                              funz_edit_code,geom_editor);
	   if (wwinedit == NULL)
	   {
	      sprintf (message,"Can't open file %s for processing.", nomefile);
	      scrivi_messaggio (message);      
	   }
	   else
	   {
	      dbox_code_managed = True;
	
	   /* Memorizza nello userData i dati relativi al modulo */
	      strcpy(edit_struct[0].nome_mod, nome_modulo);
	      strcpy(edit_struct[0].descr_mod, descr_modulo);
	      edit_struct[0].tipo_lib = tipo_modulo;
	
	      set_something_val(weditcode, XmNuserData, (XtArgVal) edit_struct);
	   }
	}
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_delete( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	sprintf (message, "Do you really want to delete the module %s ?", nome_modulo);
	dialog_delete = (Widget) conferma (UxTopLevel, message, cancella_modulo);
	XtManageChild (dialog_delete);
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_menu1_top_b2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_edicole( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	char *path, program[256];
	int   pid, i, status;
	
	
	  if ( !fine_edicole )
	  {
	     scrivi_messaggio ("EDICOLE already in use.");
	     return;
	  }
	
	  scrivi_messaggio ("Loading EDICOLE...");
	  fine_edicole = False;
	
	/* abilita la ricezione del segnale di terminazione del child */
	  signal(SIGCLD, end_edicole);
	
#ifndef VMS
	
	/* Attiva un nuovo sotto-processo per l'editazione della bitmap */
	  pid = fork();
	
#else
	
	  pid = vfork();
	
#endif
	
	  if ( pid == 0 )
	  {
	     path = (char *) getenv ("LEGOCAD_BIN");
	     strcpy (program,path);
	     strcat (program,"/edicole");
	
	     execl (program,"edicole","-module",nome_modulo,"&", (char *)0);
	  }
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_view( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
#if defined HYPERHELP_USED
	   /* utilizzo hyperhelp per la documentazione dei moduli */
	   hhelp_doc_modulo();
#else
	   /* utilizzo i file documentazione di libreria */
	  if (!dbox_doc_managed)
	    create_doc_modulo();
#endif
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_pb_library( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	int flag;
	
	switch (tipo_modulo)
	{
	    case LIBUT:
	         flag = cad_crealibut();
	         break;
	
	    case LIBUTREG:
	         flag = cad_crealibut_reg();
	         break;
	
	    case LIBREG:
	         flag = cad_crealibreg();
	         break;
	}
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	valueChangedCB_tb_debug( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	Boolean stato;
	XmString xmstr;
	
	stato = XmToggleButtonGetState(UxWidget);
	
	if( stato )
	    xmstr = XmStringCreateSimple("Compiler Debug ON");
	else
	    xmstr = XmStringCreateSimple("Compiler Debug OFF");
	
	set_something_val(UxWidget,XmNlabelString, (XtArgVal) xmstr);
	
	set_debug(stato);
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_mb_force_all( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	char direc[256];
	char comando[256];
	
	percorso_libreria(direc, LIBUT, LIB_UTENTE);
	
	sprintf(comando,"touch -c %s*.f\n",direc);
	
	system(comando);
	}
	UxMainWindow1Context = UxSaveCtx;
}

static void	activateCB_mb_force_selected( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCmainWindow1         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMainWindow1Context;
	UxMainWindow1Context = UxContext =
			(_UxCmainWindow1 *) UxGetContext( UxWidget );
	{
	char nomefile[256];
	static char *font_text = "-Adobe-Courier-Bold-R-Normal--14-140-75-75-M-90-ISO8859-1";
	char comando[256];
	
	if (!dbox_code_managed)
	{
	/* Percorso del file FORTRAN da editare. */
	   componi_file_modulo(nomefile,nome_modulo,FILE_FTN,tipo_modulo,LIB_UTENTE);
	
	   if( strcmp(nomefile,"") != 0)
	   {
	      sprintf(comando,"touch -c %s\n",nomefile);
	      system(comando);
	   }
	   else
	      scrivi_messaggio("No Module Selected");
	}
	}
	UxMainWindow1Context = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_mainWindow1()
{
	Widget	opt_tipolib_shell;
	Widget	message_menu_shell;
	Widget	mb_file_shell;
	Widget	mb_module_shell;
	Widget	mb_call_newmod_shell;
	Widget	mb_copy_shell;
	Widget	mb_icon_shell;
	Widget	mb_doc_shell;
	Widget	mb_compile_shell;
	Widget	mb_options_shell;
	Widget	mb_force_shell;

	mainWindow1 = XtVaCreatePopupShell( "mainWindow1",
			applicationShellWidgetClass, UxTopLevel,
			XmNheight, 800,
			XmNwidth, 600,
			XmNy, 480,
			XmNx, 238,
			XmNiconName, "LIBRARIAN",
			XmNtitle, "USER MODULES LIBRARY",
			NULL );

	UxPutContext( mainWindow1, (char *) UxMainWindow1Context );

	mainWindow2 = XtVaCreateManagedWidget( "mainWindow2",
			xmMainWindowWidgetClass, mainWindow1,
			XmNheight, 800,
			XmNwidth, 600,
			XmNy, 480,
			XmNx, 238,
			NULL );

	UxPutContext( mainWindow2, (char *) UxMainWindow1Context );

	form1 = XtVaCreateManagedWidget( "form1",
			xmFormWidgetClass, mainWindow2,
			XmNheight, 400,
			XmNwidth, 549,
			NULL );

	UxPutContext( form1, (char *) UxMainWindow1Context );

	panedWindow1 = XtVaCreateManagedWidget( "panedWindow1",
			xmPanedWindowWidgetClass, form1,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNheight, 769,
			XmNwidth, 680,
			XmNy, 0,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( panedWindow1, (char *) UxMainWindow1Context );

	form10 = XtVaCreateManagedWidget( "form10",
			xmFormWidgetClass, panedWindow1,
			XmNheight, 373,
			XmNwidth, 594,
			XmNy, 0,
			XmNx, 3,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form10, (char *) UxMainWindow1Context );

	form3 = XtVaCreateManagedWidget( "form3",
			xmFormWidgetClass, form10,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 40,
			XmNwidth, 580,
			XmNy, 336,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form3, (char *) UxMainWindow1Context );

	static_module_label = XtVaCreateManagedWidget( "static_module_label",
			xmLabelWidgetClass, form3,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Selected module:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 130,
			XmNy, 5,
			XmNx, 230,
			NULL );

	UxPutContext( static_module_label, (char *) UxMainWindow1Context );

	selected_module_label = XtVaCreateManagedWidget( "selected_module_label",
			xmLabelWidgetClass, form3,
			XmNrightOffset, 5,
			XmNresizable, FALSE,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftWidget, static_module_label,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "<none>" ),
			RES_CONVERT( XmNforeground, "yellow" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 90,
			XmNy, 20,
			XmNx, 140,
			NULL );

	UxPutContext( selected_module_label, (char *) UxMainWindow1Context );

	separatorGadget6 = XtVaCreateManagedWidget( "separatorGadget6",
			xmSeparatorGadgetClass, form10,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, form3,
			XmNbottomOffset, 0,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 10,
			XmNwidth, 580,
			XmNy, 310,
			XmNx, 10,
			NULL );

	UxPutContext( separatorGadget6, (char *) UxMainWindow1Context );

	form12 = XtVaCreateManagedWidget( "form12",
			xmFormWidgetClass, form10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, separatorGadget6,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 40,
			XmNwidth, 580,
			XmNy, 270,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form12, (char *) UxMainWindow1Context );

	label_user_path = XtVaCreateManagedWidget( "label_user_path",
			xmLabelWidgetClass, form10,
			XmNrecomputeSize, FALSE,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "USER LIBRARY PATH:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 20,
			XmNheight, 20,
			XmNwidth, 160,
			XmNy, 20,
			XmNx, 10,
			NULL );

	UxPutContext( label_user_path, (char *) UxMainWindow1Context );

	label_path_lib = XtVaCreateManagedWidget( "label_path_lib",
			xmLabelWidgetClass, form10,
			RES_CONVERT( XmNforeground, "Red" ),
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftWidget, label_user_path,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 440,
			XmNy, 20,
			XmNx, 140,
			NULL );

	UxPutContext( label_path_lib, (char *) UxMainWindow1Context );

	labelGadget17 = XtVaCreateManagedWidget( "labelGadget17",
			xmLabelGadgetClass, form10,
			XmNtopWidget, label_user_path,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "USER MODULES" ),
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_NONE,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNheight, 30,
			XmNwidth, 180,
			XmNy, 10,
			XmNx, 0,
			NULL );

	UxPutContext( labelGadget17, (char *) UxMainWindow1Context );

	scrolledWindow13 = XtVaCreateManagedWidget( "scrolledWindow13",
			xmScrolledWindowWidgetClass, form10,
			XmNbottomWidget, form12,
			XmNbottomOffset, 5,
			XmNtopWidget, labelGadget17,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomPosition, 0,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 50,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow13, (char *) UxMainWindow1Context );

	lista_moduli_utente = XtVaCreateManagedWidget( "lista_moduli_utente",
			xmListWidgetClass, scrolledWindow13,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNlistSizePolicy, XmVARIABLE,
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNfontList, UxConvertFontList( "-Adobe-Courier-Bold-R-Normal--14-140-75-75-M-90-ISO8859-1" ),
			XmNheight, 310,
			XmNwidth, 590,
			NULL );

	UxPutContext( lista_moduli_utente, (char *) UxMainWindow1Context );

	opt_tipolib_shell = XtVaCreatePopupShell ("opt_tipolib_shell",
			xmMenuShellWidgetClass, form10,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	opt_tipolib = XtVaCreateWidget( "opt_tipolib",
			xmRowColumnWidgetClass, opt_tipolib_shell,
			RES_CONVERT( XmNlabelString, "Library:" ),
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( opt_tipolib, (char *) UxMainWindow1Context );

	show_libut_pb = XtVaCreateManagedWidget( "show_libut_pb",
			xmPushButtonWidgetClass, opt_tipolib,
			RES_CONVERT( XmNlabelString, "libut" ),
			NULL );

	UxPutContext( show_libut_pb, (char *) UxMainWindow1Context );

	show_libutreg_pb = XtVaCreateManagedWidget( "show_libutreg_pb",
			xmPushButtonWidgetClass, opt_tipolib,
			RES_CONVERT( XmNlabelString, "libut_reg" ),
			NULL );

	UxPutContext( show_libutreg_pb, (char *) UxMainWindow1Context );

	show_libreg_pb = XtVaCreateManagedWidget( "show_libreg_pb",
			xmPushButtonWidgetClass, opt_tipolib,
			RES_CONVERT( XmNlabelString, "libreg" ),
			NULL );

	UxPutContext( show_libreg_pb, (char *) UxMainWindow1Context );

	menu_tipolib = XtVaCreateManagedWidget( "menu_tipolib",
			xmRowColumnWidgetClass, form10,
			RES_CONVERT( XmNlabelString, "Library:" ),
			XmNtopWidget, label_path_lib,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_NONE,
			XmNsubMenuId, opt_tipolib,
			XmNrowColumnType, XmMENU_OPTION,
			NULL );

	UxPutContext( menu_tipolib, (char *) UxMainWindow1Context );

	form11 = XtVaCreateManagedWidget( "form11",
			xmFormWidgetClass, panedWindow1,
			XmNheight, 380,
			XmNwidth, 600,
			XmNy, 380,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form11, (char *) UxMainWindow1Context );

	frame1 = XtVaCreateManagedWidget( "frame1",
			xmFrameWidgetClass, form11,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNheight, 30,
			XmNwidth, 600,
			XmNy, 61,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( frame1, (char *) UxMainWindow1Context );

	labelGadget3 = XtVaCreateManagedWidget( "labelGadget3",
			xmLabelGadgetClass, frame1,
			RES_CONVERT( XmNlabelString, "MESSAGES" ),
			XmNheight, 30,
			XmNwidth, 240,
			XmNy, 10,
			XmNx, 170,
			NULL );

	UxPutContext( labelGadget3, (char *) UxMainWindow1Context );

	scrolledWindow2 = XtVaCreateManagedWidget( "scrolledWindow2",
			xmScrolledWindowWidgetClass, form11,
			XmNbottomOffset, 20,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopWidget, frame1,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 10,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 105,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow2, (char *) UxMainWindow1Context );

	finestra_messaggi = XtVaCreateManagedWidget( "finestra_messaggi",
			xmTextWidgetClass, scrolledWindow2,
			XmNverifyBell, FALSE,
			XmNcursorPositionVisible, FALSE,
			XmNfontList, UxConvertFontList( "*Courier-Bold-R-Normal--14-*" ),
			XmNresizeWidth, TRUE,
			XmNresizeHeight, TRUE,
			XmNeditable, FALSE,
			XmNeditMode, XmMULTI_LINE_EDIT ,
			XmNcolumns, 80,
			XmNy, 105,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( finestra_messaggi, (char *) UxMainWindow1Context );

	message_menu_shell = XtVaCreatePopupShell ("message_menu_shell",
			xmMenuShellWidgetClass, finestra_messaggi,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	message_menu = XtVaCreateWidget( "message_menu",
			xmRowColumnWidgetClass, message_menu_shell,
			XmNentryAlignment, XmALIGNMENT_CENTER,
			XmNmenuAccelerator, "<KeyUp>F4",
			XmNrowColumnType, XmMENU_POPUP,
			NULL );

	UxPutContext( message_menu, (char *) UxMainWindow1Context );

	menu1_p1_title = XtVaCreateManagedWidget( "menu1_p1_title",
			xmLabelWidgetClass, message_menu,
			RES_CONVERT( XmNlabelString, "MESSAGE" ),
			NULL );

	UxPutContext( menu1_p1_title, (char *) UxMainWindow1Context );

	popup_separator = XtVaCreateManagedWidget( "popup_separator",
			xmSeparatorGadgetClass, message_menu,
			NULL );

	UxPutContext( popup_separator, (char *) UxMainWindow1Context );

	popup_pbutton = XtVaCreateManagedWidget( "popup_pbutton",
			xmPushButtonWidgetClass, message_menu,
			XmNmarginRight, 0,
			RES_CONVERT( XmNacceleratorText, "" ),
			XmNaccelerator, "",
			RES_CONVERT( XmNlabelString, "   Clear window" ),
			NULL );

	UxPutContext( popup_pbutton, (char *) UxMainWindow1Context );

	menubar = XtVaCreateManagedWidget( "menubar",
			xmRowColumnWidgetClass, mainWindow2,
			XmNheight, 35,
			XmNspacing, 25,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( menubar, (char *) UxMainWindow1Context );

	mb_file_shell = XtVaCreatePopupShell ("mb_file_shell",
			xmMenuShellWidgetClass, menubar,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mb_file = XtVaCreateWidget( "mb_file",
			xmRowColumnWidgetClass, mb_file_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( mb_file, (char *) UxMainWindow1Context );

	pb_quit = XtVaCreateManagedWidget( "pb_quit",
			xmPushButtonWidgetClass, mb_file,
			RES_CONVERT( XmNmnemonic, "Q" ),
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );

	UxPutContext( pb_quit, (char *) UxMainWindow1Context );

	menu1_top_b1 = XtVaCreateManagedWidget( "menu1_top_b1",
			xmCascadeButtonGadgetClass, menubar,
			RES_CONVERT( XmNmnemonic, "F" ),
			XmNsubMenuId, mb_file,
			RES_CONVERT( XmNlabelString, "File" ),
			NULL );

	UxPutContext( menu1_top_b1, (char *) UxMainWindow1Context );

	mb_module_shell = XtVaCreatePopupShell ("mb_module_shell",
			xmMenuShellWidgetClass, menubar,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mb_module = XtVaCreateWidget( "mb_module",
			xmRowColumnWidgetClass, mb_module_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( mb_module, (char *) UxMainWindow1Context );

	mb_call_newmod_shell = XtVaCreatePopupShell ("mb_call_newmod_shell",
			xmMenuShellWidgetClass, mb_module,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mb_call_newmod = XtVaCreateWidget( "mb_call_newmod",
			xmRowColumnWidgetClass, mb_call_newmod_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( mb_call_newmod, (char *) UxMainWindow1Context );

	pb_create_newmod = XtVaCreateManagedWidget( "pb_create_newmod",
			xmPushButtonWidgetClass, mb_call_newmod,
			RES_CONVERT( XmNmnemonic, "C" ),
			RES_CONVERT( XmNlabelString, "Create" ),
			NULL );

	UxPutContext( pb_create_newmod, (char *) UxMainWindow1Context );

	pb_modify_newmod = XtVaCreateManagedWidget( "pb_modify_newmod",
			xmPushButtonWidgetClass, mb_call_newmod,
			RES_CONVERT( XmNmnemonic, "M" ),
			RES_CONVERT( XmNlabelString, "Modify" ),
			NULL );

	UxPutContext( pb_modify_newmod, (char *) UxMainWindow1Context );

	cb_call_newmod = XtVaCreateManagedWidget( "cb_call_newmod",
			xmCascadeButtonGadgetClass, mb_module,
			XmNsubMenuId, mb_call_newmod,
			RES_CONVERT( XmNmnemonic, "N" ),
			RES_CONVERT( XmNlabelString, "Call (NEWMOD)" ),
			NULL );

	UxPutContext( cb_call_newmod, (char *) UxMainWindow1Context );

	mb_copy_shell = XtVaCreatePopupShell ("mb_copy_shell",
			xmMenuShellWidgetClass, mb_module,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mb_copy = XtVaCreateWidget( "mb_copy",
			xmRowColumnWidgetClass, mb_copy_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( mb_copy, (char *) UxMainWindow1Context );

	pb_user = XtVaCreateManagedWidget( "pb_user",
			xmPushButtonWidgetClass, mb_copy,
			RES_CONVERT( XmNmnemonic, "u" ),
			RES_CONVERT( XmNlabelString, "Different user" ),
			NULL );

	UxPutContext( pb_user, (char *) UxMainWindow1Context );

	pb_stdlib = XtVaCreateManagedWidget( "pb_stdlib",
			xmPushButtonWidgetClass, mb_copy,
			RES_CONVERT( XmNmnemonic, "l" ),
			RES_CONVERT( XmNlabelString, "Standard library" ),
			NULL );

	UxPutContext( pb_stdlib, (char *) UxMainWindow1Context );

	pb_stdlib_base = XtVaCreateManagedWidget( "pb_stdlib_base",
			xmPushButtonWidgetClass, mb_copy,
			RES_CONVERT( XmNlabelString, "Base Standard Library" ),
			NULL );

	UxPutContext( pb_stdlib_base, (char *) UxMainWindow1Context );

	pb_stdlib_auxiliary = XtVaCreateManagedWidget( "pb_stdlib_auxiliary",
			xmPushButtonWidgetClass, mb_copy,
			RES_CONVERT( XmNlabelString, "Auxiliary Standard Library" ),
			NULL );

	UxPutContext( pb_stdlib_auxiliary, (char *) UxMainWindow1Context );

	pb_stdlib_nuclear = XtVaCreateManagedWidget( "pb_stdlib_nuclear",
			xmPushButtonWidgetClass, mb_copy,
			RES_CONVERT( XmNlabelString, "Nuclear Standard Library" ),
			NULL );

	UxPutContext( pb_stdlib_nuclear, (char *) UxMainWindow1Context );

	pb_stdlib_special = XtVaCreateManagedWidget( "pb_stdlib_special",
			xmPushButtonWidgetClass, mb_copy,
			RES_CONVERT( XmNlabelString, "Special Standard Library" ),
			NULL );

	UxPutContext( pb_stdlib_special, (char *) UxMainWindow1Context );

	cb_copy = XtVaCreateManagedWidget( "cb_copy",
			xmCascadeButtonGadgetClass, mb_module,
			XmNsubMenuId, mb_copy,
			RES_CONVERT( XmNmnemonic, "y" ),
			RES_CONVERT( XmNlabelString, "Copy from" ),
			NULL );

	UxPutContext( cb_copy, (char *) UxMainWindow1Context );

	pb_edit_ftn = XtVaCreateManagedWidget( "pb_edit_ftn",
			xmPushButtonWidgetClass, mb_module,
			RES_CONVERT( XmNmnemonic, "c" ),
			RES_CONVERT( XmNlabelString, "Edit  code" ),
			NULL );

	UxPutContext( pb_edit_ftn, (char *) UxMainWindow1Context );

	pb_delete = XtVaCreateManagedWidget( "pb_delete",
			xmPushButtonWidgetClass, mb_module,
			RES_CONVERT( XmNmnemonic, "D" ),
			RES_CONVERT( XmNlabelString, "Delete" ),
			NULL );

	UxPutContext( pb_delete, (char *) UxMainWindow1Context );

	menu1_top_b2 = XtVaCreateManagedWidget( "menu1_top_b2",
			xmCascadeButtonGadgetClass, menubar,
			RES_CONVERT( XmNmnemonic, "M" ),
			XmNsubMenuId, mb_module,
			RES_CONVERT( XmNlabelString, "Module" ),
			NULL );

	UxPutContext( menu1_top_b2, (char *) UxMainWindow1Context );

	mb_icon_shell = XtVaCreatePopupShell ("mb_icon_shell",
			xmMenuShellWidgetClass, menubar,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mb_icon = XtVaCreateWidget( "mb_icon",
			xmRowColumnWidgetClass, mb_icon_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( mb_icon, (char *) UxMainWindow1Context );

	pb_edicole = XtVaCreateManagedWidget( "pb_edicole",
			xmPushButtonWidgetClass, mb_icon,
			RES_CONVERT( XmNmnemonic, "E" ),
			RES_CONVERT( XmNlabelString, "Edit (EDICOLE)" ),
			NULL );

	UxPutContext( pb_edicole, (char *) UxMainWindow1Context );

	menubar_top_b1 = XtVaCreateManagedWidget( "menubar_top_b1",
			xmCascadeButtonGadgetClass, menubar,
			XmNsubMenuId, mb_icon,
			RES_CONVERT( XmNmnemonic, "I" ),
			RES_CONVERT( XmNlabelString, "Icon" ),
			NULL );

	UxPutContext( menubar_top_b1, (char *) UxMainWindow1Context );

	mb_doc_shell = XtVaCreatePopupShell ("mb_doc_shell",
			xmMenuShellWidgetClass, menubar,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mb_doc = XtVaCreateWidget( "mb_doc",
			xmRowColumnWidgetClass, mb_doc_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( mb_doc, (char *) UxMainWindow1Context );

	pb_view = XtVaCreateManagedWidget( "pb_view",
			xmPushButtonWidgetClass, mb_doc,
			RES_CONVERT( XmNmnemonic, "V" ),
			RES_CONVERT( XmNlabelString, "View" ),
			NULL );

	UxPutContext( pb_view, (char *) UxMainWindow1Context );

	menubar_top_b2 = XtVaCreateManagedWidget( "menubar_top_b2",
			xmCascadeButtonGadgetClass, menubar,
			XmNsubMenuId, mb_doc,
			RES_CONVERT( XmNmnemonic, "D" ),
			RES_CONVERT( XmNlabelString, "Documentation" ),
			NULL );

	UxPutContext( menubar_top_b2, (char *) UxMainWindow1Context );

	mb_compile_shell = XtVaCreatePopupShell ("mb_compile_shell",
			xmMenuShellWidgetClass, menubar,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mb_compile = XtVaCreateWidget( "mb_compile",
			xmRowColumnWidgetClass, mb_compile_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( mb_compile, (char *) UxMainWindow1Context );

	pb_library = XtVaCreateManagedWidget( "pb_library",
			xmPushButtonWidgetClass, mb_compile,
			RES_CONVERT( XmNmnemonic, "L" ),
			RES_CONVERT( XmNlabelString, "Library" ),
			NULL );

	UxPutContext( pb_library, (char *) UxMainWindow1Context );

	menubar_top_b3 = XtVaCreateManagedWidget( "menubar_top_b3",
			xmCascadeButtonGadgetClass, menubar,
			XmNsubMenuId, mb_compile,
			RES_CONVERT( XmNmnemonic, "C" ),
			RES_CONVERT( XmNlabelString, "Compile" ),
			NULL );

	UxPutContext( menubar_top_b3, (char *) UxMainWindow1Context );

	mb_options_shell = XtVaCreatePopupShell ("mb_options_shell",
			xmMenuShellWidgetClass, menubar,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mb_options = XtVaCreateWidget( "mb_options",
			xmRowColumnWidgetClass, mb_options_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( mb_options, (char *) UxMainWindow1Context );

	tb_debug = XtVaCreateManagedWidget( "tb_debug",
			xmToggleButtonWidgetClass, mb_options,
			XmNindicatorType, XmONE_OF_MANY,
			RES_CONVERT( XmNmnemonic, "g" ),
			RES_CONVERT( XmNlabelString, "Compiler Debug OFF" ),
			NULL );

	UxPutContext( tb_debug, (char *) UxMainWindow1Context );

	mb_force_shell = XtVaCreatePopupShell ("mb_force_shell",
			xmMenuShellWidgetClass, mb_options,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mb_force = XtVaCreateWidget( "mb_force",
			xmRowColumnWidgetClass, mb_force_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( mb_force, (char *) UxMainWindow1Context );

	mb_force_all = XtVaCreateManagedWidget( "mb_force_all",
			xmPushButtonWidgetClass, mb_force,
			RES_CONVERT( XmNlabelString, "All Module" ),
			NULL );

	UxPutContext( mb_force_all, (char *) UxMainWindow1Context );

	mb_force_selected = XtVaCreateManagedWidget( "mb_force_selected",
			xmPushButtonWidgetClass, mb_force,
			RES_CONVERT( XmNlabelString, "Selected Module" ),
			NULL );

	UxPutContext( mb_force_selected, (char *) UxMainWindow1Context );

	mb_options_b3 = XtVaCreateManagedWidget( "mb_options_b3",
			xmCascadeButtonWidgetClass, mb_options,
			XmNsubMenuId, mb_force,
			RES_CONVERT( XmNlabelString, "Touch" ),
			NULL );

	UxPutContext( mb_options_b3, (char *) UxMainWindow1Context );

	menubar_top_b4 = XtVaCreateManagedWidget( "menubar_top_b4",
			xmCascadeButtonGadgetClass, menubar,
			XmNsubMenuId, mb_options,
			RES_CONVERT( XmNmnemonic, "O" ),
			RES_CONVERT( XmNlabelString, "Options" ),
			NULL );

	UxPutContext( menubar_top_b4, (char *) UxMainWindow1Context );

	XtAddCallback( mainWindow1, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( lista_moduli_utente, XmNsingleSelectionCallback,
			singleSelectionCB_lista_moduli_utente,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( show_libut_pb, XmNactivateCallback,
			cambia_tipo_modulo,
			(XtPointer) LIBUT );

	XtAddCallback( show_libutreg_pb, XmNactivateCallback,
			cambia_tipo_modulo,
			(XtPointer) LIBUTREG );

	XtAddCallback( show_libreg_pb, XmNactivateCallback,
			cambia_tipo_modulo,
			(XtPointer) LIBREG );

	XtAddCallback( popup_pbutton, XmNactivateCallback,
			activateCB_popup_pbutton,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_quit, XmNactivateCallback,
			activateCB_pb_quit,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_create_newmod, XmNactivateCallback,
			activateCB_pb_create_newmod,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_modify_newmod, XmNactivateCallback,
			activateCB_pb_modify_newmod,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_user, XmNactivateCallback,
			activateCB_pb_user,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_stdlib, XmNactivateCallback,
			activateCB_pb_stdlib,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_stdlib_base, XmNactivateCallback,
			activateCB_pb_stdlib_base,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_stdlib_auxiliary, XmNactivateCallback,
			activateCB_pb_stdlib_auxiliary,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_stdlib_nuclear, XmNactivateCallback,
			activateCB_pb_stdlib_nuclear,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_stdlib_special, XmNactivateCallback,
			activateCB_pb_stdlib_special,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_edit_ftn, XmNactivateCallback,
			activateCB_pb_edit_ftn,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_delete, XmNactivateCallback,
			activateCB_pb_delete,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( menu1_top_b2, XmNactivateCallback,
			activateCB_menu1_top_b2,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_edicole, XmNactivateCallback,
			activateCB_pb_edicole,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_view, XmNactivateCallback,
			activateCB_pb_view,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( pb_library, XmNactivateCallback,
			activateCB_pb_library,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( tb_debug, XmNvalueChangedCallback,
			valueChangedCB_tb_debug,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( mb_force_all, XmNactivateCallback,
			activateCB_mb_force_all,
			(XtPointer) UxMainWindow1Context );

	XtAddCallback( mb_force_selected, XmNactivateCallback,
			activateCB_mb_force_selected,
			(XtPointer) UxMainWindow1Context );


	XtAddEventHandler( finestra_messaggi, ButtonPressMask,
			False, _UxmainWindow1MenuPost, message_menu );
	XmMainWindowSetAreas( mainWindow2, menubar, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, form1 );

	return ( mainWindow1 );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_mainWindow1()
{
	Widget                  rtrn;
	_UxCmainWindow1         *UxContext;

	UxMainWindow1Context = UxContext =
		(_UxCmainWindow1 *) XtMalloc( sizeof(_UxCmainWindow1) );

	{
		Widget userlist_find_form;
		FILE *fp;
		rtrn = _Uxbuild_mainWindow1();

		/* Creazione Font per Text */
		   finfo = XLoadQueryFont(XtDisplay(UxGetWidget(mainWindow1)),font);
		   fontList = XmFontListCreate (finfo,XmSTRING_DEFAULT_CHARSET);
		
		/* Inserimento find di libreria */
		   parametri_userlist.ID_lista = UxGetWidget(lista_moduli_utente);
		   parametri_userlist.num_elem = 100;
		   parametri_userlist.vis_elem = 1;
		   parametri_userlist.pos      = 0;
		
		   argcount=0;
		   XtSetArg(args[argcount],XmNleftAttachment,XmATTACH_FORM);
		   argcount++;
		   XtSetArg(args[argcount],XmNleftOffset,0);
		   argcount++;
		   XtSetArg(args[argcount],XmNrightAttachment,XmATTACH_FORM);
		   argcount++;
		   XtSetArg(args[argcount],XmNrightOffset,0);
		   argcount++;
		   XtSetArg(args[argcount],XmNbottomAttachment,XmATTACH_FORM);
		   argcount++;
		   XtSetArg(args[argcount],XmNbottomOffset,0);
		   argcount++;
		   XtSetArg(args[argcount],XmNtopAttachment,XmATTACH_FORM);
		   argcount++;
		   XtSetArg(args[argcount],XmNtopOffset,0);
		   argcount++;
		   userlist_find_form = find_kit (UxGetWidget(form12),args,argcount,
		                                  &parametri_userlist);
		
		/* Visualizza la lista dei moduli della libreria LIBUT */
		   cambia_tipo_modulo(NULL, LIB_UTENTE, NULL);
		
		/* debug in compilazione moduli */
		   get_debug();
		
		   UxPopupInterface(rtrn, no_grab);
		   return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_mainWindow1()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_mainWindow1();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

