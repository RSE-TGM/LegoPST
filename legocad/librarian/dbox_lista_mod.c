
/*******************************************************************************
	dbox_lista_mod.c
	(Generated from interface file dbox_lista_mod.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/RowColumn.h>
#include <Xm/LabelG.h>
#include <Xm/Frame.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Form.h>
#include <Xm/SeparatoG.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo dbox_lista_mod.i
   tipo 
   release 2.22
   data 5/9/95
   reserved @(#)dbox_lista_mod.i	2.22
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

#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#ifndef LIBUTILX
#include <libutilx.h>
#define LIBUTILX
#endif

#ifndef LIBICOFILE
#include <file_icone.h>
#define LIBICOFILE
#endif

#include "definizioni.h"
#include "aggiunte.h"

/********************************************************/
/* VARIABILI GLOBALI ESTERNE				*/
/********************************************************/

extern Arg args[];
extern Cardinal argcount;

extern Display *display;

extern Widget UxTopLevel, attention_wdg;
extern Dialog_geometry geom_attention;

extern byte tipo_modulo;

/* Array di compound strings */
extern XmString cstrings[], cstring, cstring_null;

extern char message[];

/* VARIABILE GLOBALE DEL NOME DEL MODULO SELEZIONATO */
extern char nome_modulo[];
extern char descr_modulo[];

/* utilizzata nella copia da un altro utente LEGOCAZ */
extern char *path_altro_utente; 

extern Boolean menu_sbloccato;
extern char doc_of_module[];

extern char module_sorg[];
extern char file_moduli_libut[];
extern Boolean nome_mod_cambiato;

extern Widget UxTopLevel;

extern swidget selected_module_label, lista_moduli_utente;

/********************************************************/
/* VARIABILI GLOBALI 					*/
/********************************************************/

int selected_item;

byte tipo_copia;

Boolean aspetta_conferma;

Find_struct parametri_list;

XEvent evento;

/***************************************************/
/* Struttura di callback per gli widget di conferma*/
/***************************************************/
static Elenco_callback copre_moduli = {
	{"Yes", overwrite_yes, 0 },
	{"No" , overwrite_no,  0  },
	{ NULL, NULL,          NULL} }; 

/* E' consentita la conferma/cancellazione di moduli con tag '!' */
/* fino ad un massimo di 50 */
ModuleCheck module_check[50];
XmString cstring_module_to_overwrite[50];

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
	Widget	Uxdbox_lista_mod;
	Widget	Uxpb_listamod_ok;
	Widget	Uxpb_listamod_cancel;
	Widget	Uxlabel_lista_mod;
	Widget	UxseparatorGadget5;
	Widget	Uxform7;
	Widget	UxscrolledWindow1;
	Widget	UxscrolledList_moduli;
	Widget	Uxform_counter;
	Widget	Uxframe5;
	Widget	Uxlabel_select_item;
	Widget	UxlabelGadget7;
	Widget	Uxdeselect_all_pb;
	Widget	Uxselect_all_pb;
	Widget	Uxopt_tipocopia;
	Widget	Uxcopy_both_pb;
	Widget	Uxcopy_fortran_pb;
	Widget	Uxcopy_icons_pb;
	Widget	Uxmenu_tipocopia;
	FILE	*Uxfp;
	byte	Uxtipo_libreria;
	byte	Uxflag_list;
	Boolean	*Uxflag_manage;
} _UxCdbox_lista_mod;

#define dbox_lista_mod          UxDbox_lista_modContext->Uxdbox_lista_mod
#define pb_listamod_ok          UxDbox_lista_modContext->Uxpb_listamod_ok
#define pb_listamod_cancel      UxDbox_lista_modContext->Uxpb_listamod_cancel
#define label_lista_mod         UxDbox_lista_modContext->Uxlabel_lista_mod
#define separatorGadget5        UxDbox_lista_modContext->UxseparatorGadget5
#define form7                   UxDbox_lista_modContext->Uxform7
#define scrolledWindow1         UxDbox_lista_modContext->UxscrolledWindow1
#define scrolledList_moduli     UxDbox_lista_modContext->UxscrolledList_moduli
#define form_counter            UxDbox_lista_modContext->Uxform_counter
#define frame5                  UxDbox_lista_modContext->Uxframe5
#define label_select_item       UxDbox_lista_modContext->Uxlabel_select_item
#define labelGadget7            UxDbox_lista_modContext->UxlabelGadget7
#define deselect_all_pb         UxDbox_lista_modContext->Uxdeselect_all_pb
#define select_all_pb           UxDbox_lista_modContext->Uxselect_all_pb
#define opt_tipocopia           UxDbox_lista_modContext->Uxopt_tipocopia
#define copy_both_pb            UxDbox_lista_modContext->Uxcopy_both_pb
#define copy_fortran_pb         UxDbox_lista_modContext->Uxcopy_fortran_pb
#define copy_icons_pb           UxDbox_lista_modContext->Uxcopy_icons_pb
#define menu_tipocopia          UxDbox_lista_modContext->Uxmenu_tipocopia
#define fp                      UxDbox_lista_modContext->Uxfp
#define tipo_libreria           UxDbox_lista_modContext->Uxtipo_libreria
#define flag_list               UxDbox_lista_modContext->Uxflag_list
#define flag_manage             UxDbox_lista_modContext->Uxflag_manage

static _UxCdbox_lista_mod	*UxDbox_lista_modContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_lista_moduli();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/
extern void free_array_XmString( XmString, int );

/***********************************************************************/
/*** riempi_lista_moduli (file, lista_moduli)
 ***   Parametri:
 ***     FILE *file: file pointer lista dei moduli.
 ***     Widget lista_moduli: scroll-list.
 ***
 ***   Descrizione:
 ***     Visualizza nella scrolledList appropriata una lista di moduli
 ***    (LIBUT,LIBUTREG,LIBREG)
 ***/
riempi_lista_moduli (file, lista_moduli)
FILE *file;
Widget lista_moduli;
{
   int i;
   char temp[81];

   i=0;
   while (fgets(temp,80,file) != NULL)
   {
   /* Tolgo i due eventuali caratteri dopo il nome */
/* ma non l'asterisco che indica l'esistenza fortran aux

      if (strlen(temp) > 5)
         temp[4] = ' ';
*/
      if (strlen(temp) > 6)
         temp[5] = ' ';

   /* Tolgo il \n alla fine della stringa */
      temp[strlen(temp)-1] = '\0'; 
      cstrings[i++] = CREATE_CSTRING(temp);
   }

/* Aggiorna la scrolled list con la lista dei blocchi */
   argcount = 0;
   XtSetArg(args[argcount], XmNitemCount, i); argcount++;
   XtSetArg(args[argcount], XmNitems, cstrings ); argcount++;
   XtSetValues (lista_moduli, args, argcount);

   free_array_XmString(cstrings, i);
}

/***********************************************************************/
/***
 *** modulo_selezionato (tipo_lib, flag)
 ***  Parametri:
 ***    byte tipo_lib : tipo libreria (LIBUT, LIBUTREG, LIBREG)
 ***    byte flag: scopo (visualizzazione documentazione, copia da
 ***                    libreria standard o copia da altro utente).
 ***
 ***  Descrizione:
 ***    La funzione viene chiamata quando si seleziona un modulo dalla 
 ***    scroll-list lista dei moduli.
 ***    La funzione viene chiamata in due casi:
 *** a) quando si seleziona un modulo dalla libreria standard per recuperare
 ***    la documentazione (se esistente):
 ***    aggiorna soltanto la variabile globale doc_of_module
 *** b) quando si selezionano uno o piu' moduli dalla libreria standard o da
 ***    un altro utente LEGOCAD per COPIARLI nella libut dell'utente:
 ***    (la copia puo' riguardare soltanto il Fortran, soltanto le icone o 
 ***     entrambi)
 ***    Se non esistono:
 ***    occorre inserire i nomi dei moduli nel file lista_moduli.dat dell'utente.
 ***    Se invece esistono: 
 ***    b1) segnalare l'esistenza e attendere conferma/cancellazione 
 ***        per i moduli settati a '!' (SIGNIFICA CHE SONO STATI MODIFICATI);
 ***    b2) per i moduli presenti nella lista ma non settati a '!'
 ***        effettuare la copia senza segnalazioni e NON aggiornare la lista_moduli.
 ***
 ***    La label SELECTED MODULE e le variabili locali sul nome e 
 ***    descrizione del modulo sono settate sull'ultimo modulo, nel caso di
 ***    selezione multipla.
 ***/
modulo_selezionato (tipo_lib, flag)
byte tipo_lib;
byte flag;
{
   char nome_mod[5], descr_mod[81], *nome_e_descr, modulo_da_selez[100];
   int  i, j=0, num_moduli_selez, modulo_esiste;
   XmString *moduli_selez; 
   ModuleCheck *mchk;
   long offset;
   Boolean predproc();

   get_something(UxGetWidget(scrolledList_moduli), XmNselectedItemCount,
                                                   &num_moduli_selez);
   get_something(UxGetWidget(scrolledList_moduli), XmNselectedItems,
                                                   &moduli_selez);
   printf("TIPO LIBRERIA: %d\n", tipo_lib);
   if (num_moduli_selez == 0)
   {
      sprintf(message," No item selected. Please select one...");
      attention_wdg =  (Widget) attention (UxTopLevel,message,MAPPA,
                                           geom_attention);
      scrivi_messaggio(message);
      return(1);
   }

   switch (flag)
   {
      case STDLIB_MODULES_DOC:
           nome_e_descr = (char *) extract_string(moduli_selez[0]);
           sprintf (doc_of_module,"%.4s",nome_e_descr);
           prepara_file_doc(doc_of_module);
           XtFree(nome_e_descr);
           break;

      case STDLIB_MODULES_COPY:     /* LIBRERIA STANDARD */
      case STDLIBBASE_MODULES_COPY:    
      case STDLIBAUXILIARY_MODULES_COPY:
      case STDLIBNUCLEAR_MODULES_COPY:    
      case STDLIBSPECIAL_MODULES_COPY:
      case USER_MODULES_SELECTION:  /* LIBRERIA ALTRO UTENTE */

           for (i=0 ; i<num_moduli_selez ; i++)
           {
               nome_e_descr = (char *) extract_string(moduli_selez[i]);
               recupera_nome_e_descr(nome_e_descr,nome_mod,descr_mod);

            /* caso b1):  */
            /* Se il modulo esiste nel file lista dei moduli UTENTE ed e' */
            /* settato a '!' occorre segnalare e attendere conferma e/o */
            /* cancellazione senza inserire in lista_moduli ma soltanto */
            /* cancellando il tag */

               modulo_esiste = controlla_lista_moduli (tipo_lib, nome_mod,
						       &offset); 
               if (modulo_esiste == 1 && tipo_copia != ONLY_ICONS)
               {
                 /********************************************/
                 /* Warning e attesa di conferma dall'utente */
                 /********************************************/

                   aspetta_conferma = True;

                   mchk = &module_check[j];

                   sprintf (message,
                            "Module %s was previously imported / modified. Overwrite it ?",
                            nome_mod);

                /* Aggiorno la cstring di nome e descrizione del modulo */
                /* da controllare */
                   mchk->cstring_modulo = XmStringCopy(moduli_selez[i]);
                   mchk->tipo_copia = tipo_copia;
		   mchk->flag_copia = flag;
		   mchk->tipo_lib = tipo_lib;
                   if (flag == USER_MODULES_SELECTION)
                      strcpy(mchk->percorso, path_altro_utente);

                /* Aggiorno gli INDICI DI WIDGET della struttura */
                /* Elenco_callback di conferma */
                /* (N.B.  0 ok_button  1 cancel_ button) */
                   copre_moduli[0].parametro = copre_moduli[1].parametro = j;

                /* Aggiorno gli indici della struttura ModuleCheck */
                   mchk->offset = offset;
                   mchk->conferma_overwrite = (Widget) conferma (UxTopLevel,
                                                                 message,
								 copre_moduli);
                   XtManageChild (mchk->conferma_overwrite);

                /* aspetta la conferma dell'utente */
                   while (aspetta_conferma)
                   {
                      while (XCheckIfEvent(display, &evento, predproc, NULL))
                         XtDispatchEvent(&evento);
                   }
               }
               else  
            /* casi b) e b2): */
               {
                /********************************************************/
                /*   N.B. COPIARE FORTRAN                               */
                /********************************************************/
                   if (tipo_copia != ONLY_ICONS)
                      copia_nella_libreria(tipo_lib, nome_mod, descr_mod,
                                           (int)flag,
                                           path_altro_utente);

                /********************************************************/
                /*   N.B. COPIARE ICONE E RECORD ICON_LIST              */
                /********************************************************/

                /* Se e' selezionato ONLY_ICONS e il modulo non esiste nel */
                /* file lista dei moduli l'azione viene imbibita */
                   if ( tipo_copia == ONLY_ICONS && modulo_esiste == 0 )
                   {
                      sprintf (message, 
                               "Incongruence. You are not allowed to import icons associated to a module (%s)\n",
                               nome_mod);
                      strcat (message, "that does not exist in your library.");
                      scrivi_messaggio (message);
                      attention_wdg = (Widget) attention(UxTopLevel,message,
                                                         MAPPA,
                                                         geom_attention);
                      continue; /* esce e passa al modulo successivo */
                   }          

                /* Se e' selezionato ONLY_FORTRAN viene comunque creato un */
                /* record vuoto in icon_list.dat (sempre che non esista gia') */
                   if ( tipo_lib == LIBUT )
                      copia_nella_libgraf(nome_mod,descr_mod,tipo_copia,
                                          (int)flag ,
                                          path_altro_utente);

                   strcpy(modulo_da_selez,nome_e_descr);
/***               XtFree(nome_e_descr); ***/
               }
           }

        /* Se sono stati copiati dei sorgenti FORTRAN, aggiorna la */
        /* scroll-list della main window */
           if (tipo_copia != ONLY_ICONS && tipo_lib == tipo_modulo)
           {
              cambia_tipo_modulo(NULL, tipo_modulo, NULL);

           /* Seleziona l'ultimo modulo copiato (Svuoto le richieste al */
              setta_label_modsel(modulo_da_selez);
           }
           break;
   }

   return(0);
}

/*** Boolean predproc(display, event, arg)
 ***
 ***    Descrizione:
 ***       Accetta qualsiasi tipo di evento.
 ***/
Boolean predproc(display, event, arg)
Display *display;
XEvent *event;
char *arg;
{
    return(True);
}

/************************************************************************/
/***
 *** overwrite_yes(w, ind_struct, call_data)
 ***   Parametri:
 ***     Widget w: widget.
 ***     int ind_struct : indice dell'array module_check[]
 ***     XmAnyCallbackStruct *call_data: non utilizzato.
 ***
 ***   Descrizione:
 ***     Callback chiamata premendo l'Ok_Button nella dbox di conferma
 ***     sovrascrittura di un modulo da copiare nella libut.
 ***/
void overwrite_yes(w, ind_struct, call_data)
Widget w;
int ind_struct;  /* indice della struttura ModuleCheck (copia da STDLIB) */
XmAnyCallbackStruct *call_data;
{
   char label_modulo[81], nome_mod[5], descr_mod[81];
   char *str_modulo;
   ModuleCheck *mchk;

   mchk = &module_check[ind_struct];

   str_modulo = extract_string(mchk->cstring_modulo);
   recupera_nome_e_descr(str_modulo, nome_mod, descr_mod);

   if ( mchk->tipo_copia != ONLY_ICONS )
   {
      copia_nella_libreria(mchk->tipo_lib, nome_mod, descr_mod, 
                           (int)mchk->flag_copia,
                           mchk->percorso );

   /* Aggiorna la scroll-list della main-window solo le libreria e' la stessa */
      if (tipo_modulo == mchk->tipo_lib)
         cambia_tipo_modulo(NULL, tipo_modulo, NULL);
   }

   if ( mchk->tipo_lib == LIBUT )
      copia_nella_libgraf(nome_mod, descr_mod, mchk->tipo_copia,
                          (int)mchk->flag_copia,
                          mchk->percorso);

   if ( (mchk->flag_copia == STDLIB_MODULES_COPY)          ||
        (mchk->flag_copia == STDLIBBASE_MODULES_COPY)      ||
        (mchk->flag_copia == STDLIBAUXILIARY_MODULES_COPY) ||
        (mchk->flag_copia == STDLIBNUCLEAR_MODULES_COPY)   ||
        (mchk->flag_copia == STDLIBSPECIAL_MODULES_COPY) )
      rimuovi_tag_record(mchk->tipo_lib, mchk->offset);
   else
      apponi_tag_record(mchk->tipo_lib, mchk->offset);

   XtDestroyWidget (mchk->conferma_overwrite);
   setta_label_modsel(str_modulo);
   XtFree(str_modulo);

   sprintf (message, "module %s overwritten !",nome_modulo);
   scrivi_messaggio (message);

/* Sblocco dei menu */
   if (menu_sbloccato == False)
      menu_modulo_attivo();
   aspetta_conferma = False;
}

/************************************************************************/
/*** 
 *** overwrite_no(w, ind_struct, call_data)
 ***   Parametri:
 ***     Widget w: widget.
 ***     int ind_struct : indice dell'array module_check[]
 ***     XmAnyCallbackStruct *call_data: non utilizzato.
 ***
 ***   Descrizione:
 ***     Callback chiamata premendo il Cancel_Button nella dbox di conferma
 ***     sovrascrittura di un modulo
 ***/
void overwrite_no(w, ind_struct, call_data)
Widget w;
int ind_struct;
XmAnyCallbackStruct *call_data;
{
   aspetta_conferma = False;
   XtDestroyWidget (module_check[ind_struct].conferma_overwrite);
}

/************************************************************************/
/*** copia_nella_libreria(tipo_lib, nome_mod, descr_mod, tipo_lib, flag,
 ***			  percorso)
 ***
 ***   Parametri:
 ***     byte tipo_lib : tipo libreria utente (LIBUT, LIBUTREG o LIBREG).
 ***     char *nome_mod, *descr_mod : nome modulo e descrizione.
 ***     Boolean flag : indica se il modulo viene copiato dalla libreria
 ***                    standard (True). In questo caso il parametro successivo
 ***                    non viene considerato.
 ***     char *percorso : se flag e' False questo parametro indica il percorso
 ***                      del file da copiare.
 ***
 ***   Descrizione:
 ***     Copia un modulo nella libreria utente. Se il modulo e' inedito 
 ***     aggiorna anche il file lista dei moduli della libreria opportuna
 ***     (LIBUT, LIBUTREG o LIBREG).
 ***/
copia_nella_libreria(tipo_lib, nome_mod, descr_mod, flag, percorso)
byte tipo_lib;
char *nome_mod, *descr_mod;
int flag;
char *percorso;
{
   FILE *fp_lista;
   char file_dest[256], file_sorg[256], path_sorg[256];
   char minus_nome[5];

/* Percorso del file FORTRAN */
   if ( flag == STDLIB_MODULES_COPY )          /* mantenuta per la libreg */
      componi_file_modulo(file_sorg,nome_mod,FILE_FTN,tipo_lib,LIB_STANDARD);
   else if ( flag == STDLIBBASE_MODULES_COPY )
      componi_file_modulo(file_sorg,nome_mod,FILE_FTN,tipo_lib,LIB_STANDARD_BASE);
   else if ( flag == STDLIBAUXILIARY_MODULES_COPY )
      componi_file_modulo(file_sorg,nome_mod,FILE_FTN,tipo_lib,LIB_STANDARD_AUXILIARY);
   else if ( flag == STDLIBNUCLEAR_MODULES_COPY )
      componi_file_modulo(file_sorg,nome_mod,FILE_FTN,tipo_lib,LIB_STANDARD_NUCLEAR);
   else if ( flag == STDLIBSPECIAL_MODULES_COPY )
      componi_file_modulo(file_sorg,nome_mod,FILE_FTN,tipo_lib,LIB_STANDARD_SPECIAL);
   else
   {
   /* Copia da altro utente */
      strcpy(minus_nome, nome_mod);
      tominus(minus_nome);

#ifdef VMS
      sottodir_vms(path_sorg, percorso,sottodir_utente(tipo_lib, LIB_UTENTE));
      sprintf(file_sorg, "%s%s.ftn",path_sorg,nome_mod);
#else
      sprintf(path_sorg,"%s%s/",percorso,sottodir_utente(tipo_lib, LIB_UTENTE));
      sprintf(file_sorg,"%s%s.f",path_sorg,minus_nome);
#endif
   }

/* Copia file nella libreria utente */
   componi_file_modulo(file_dest,nome_mod,FILE_FTN,tipo_lib,LIB_UTENTE);

   printf("file sorg.: %s\n", file_sorg);
   printf("file dest.: %s\n", file_dest);

   if ( copia_file (file_sorg,file_dest) )
   {
      sprintf (message," Can't copy file %s. Check permissions.", file_sorg);
      attention_wdg = (Widget) attention(UxTopLevel,message,MAPPA,
                                         geom_attention);
      scrivi_messaggio (message);
      return(1);
   }

/* aggiornamento del file lista_moduli.dat (se e' il caso) */
   if(flag == USER_MODULES_SELECTION)
      aggiungi_modulo_lista(tipo_lib, nome_mod, descr_mod, True );
   else
      aggiungi_modulo_lista(tipo_lib, nome_mod, descr_mod, False );


/* Per la libreria LIBUT bisogna copiare anche i file INTERFACCIA della */
/* NEWMOD <nome_modulo>.i */
   if (tipo_lib == LIBUT)
   {
      if ( flag == STDLIBBASE_MODULES_COPY )
         componi_file_modulo(file_sorg,nome_mod,FILE_IFACE,tipo_lib,LIB_STANDARD_BASE);
      else if ( flag == STDLIBAUXILIARY_MODULES_COPY )
         componi_file_modulo(file_sorg,nome_mod,FILE_IFACE,tipo_lib,LIB_STANDARD_AUXILIARY);
      else if ( flag == STDLIBNUCLEAR_MODULES_COPY )
         componi_file_modulo(file_sorg,nome_mod,FILE_IFACE,tipo_lib,LIB_STANDARD_NUCLEAR);
      else if ( flag == STDLIBSPECIAL_MODULES_COPY )
         componi_file_modulo(file_sorg,nome_mod,FILE_IFACE,tipo_lib,LIB_STANDARD_SPECIAL);
      else
      /* Copia da altro utente */
         sprintf(file_sorg, "%s%s.i",path_sorg, minus_nome);

   /* Copia del file INTERFACCIA nella libut utente (se esiste) */
      if (access(file_sorg, F_OK) == 0)
      {
         printf("file sorg.: %s\n", file_sorg);

         componi_file_modulo(file_dest,nome_mod,FILE_IFACE,tipo_lib,LIB_UTENTE);
         printf("file dest.: %s\n", file_dest);

         copia_file (file_sorg,file_dest);
      }
   }

/* per la libreria LIBUT_REG bisogna copiare anche i file <modulo>.dat e */
/* <modulo>_01.dat */
   if (tipo_lib == LIBUTREG)
   {
   /* Copia file <nome_modulo>.dat */
      if ( flag )
         componi_file_modulo(file_sorg,nome_mod,FILE_DAT,tipo_lib,
                             LIB_STANDARD);
      else
      /* Copia da altro utente */
         sprintf(file_sorg, "%s%s.dat",path_sorg, minus_nome);

      printf("file sorg.: %s\n", file_sorg);

      if (access(file_sorg, F_OK) == 0)
      {
         componi_file_modulo(file_dest,nome_mod,FILE_DAT,tipo_lib,LIB_UTENTE);
         printf("file dest.: %s\n", file_dest);
         copia_file (file_sorg,file_dest);
      }

   /* Copia file <nome_modulo>_01.dat */
      if ( flag )
         componi_file_modulo(file_sorg,nome_mod,FILE_01DAT,tipo_lib,
                             LIB_STANDARD);
      else
      /* Copia da altro utente */
         sprintf(file_sorg, "%s%s_01.dat",path_sorg, minus_nome);

      printf("file sorg.: %s\n", file_sorg);

      if (access(file_sorg, F_OK) == 0)
      {
         componi_file_modulo(file_dest,nome_mod,FILE_01DAT,tipo_lib,LIB_UTENTE);
         printf("file dest.: %s\n", file_dest);
         copia_file (file_sorg,file_dest);
      }
   }



/* in qualunque caso provo a copiare la documentazione */

    if ( flag == STDLIBBASE_MODULES_COPY )
    {
       componi_file_modulo(file_sorg,nome_mod,FILE_DOC,tipo_lib,LIB_STANDARD_BASE);
       componi_file_modulo(file_dest,nome_mod,FILE_DOC,tipo_lib,LIB_UTENTE);
    }
    else if ( flag == STDLIBAUXILIARY_MODULES_COPY )
       componi_file_modulo(file_sorg,nome_mod,FILE_DOC,tipo_lib,LIB_STANDARD_AUXILIARY);
    else if ( flag == STDLIBNUCLEAR_MODULES_COPY )
       componi_file_modulo(file_sorg,nome_mod,FILE_DOC,tipo_lib,LIB_STANDARD_NUCLEAR);
    else if ( flag == STDLIBSPECIAL_MODULES_COPY )
       componi_file_modulo(file_sorg,nome_mod,FILE_DOC,tipo_lib,LIB_STANDARD_SPECIAL);
/*
    sprintf(file_dest,"%s/%s/%s.doc",getenv("LEGOCAD_USER"),PATH_LIBUT_UTE,tominus(nome_mod));
*/
    printf("copia documentazione file sorg file dest %s %s\n",file_sorg, file_dest);
    copia_file (file_sorg,file_dest);



   sprintf(message, "Module %s copied in the user library\n", nome_mod);
   scrivi_messaggio(message);
   return(0);
}

/*******************************************************************************/
/***
 *** copia_nella_libgraf(nome_mod, descr_mod, tipo_copia, flag, percorso)
 ***   Parametri:
 ***     char *nome_mod, *descr_mod : nome modulo e descrizione
 ***     byte tipo_copia : tipo copia (solo FTN, solo ICONE o entrambi)
 ***     Boolean flag : indica se le icone vengono copiate dalla libreria 
 ***                     standard (True). In questo caso il parametro successivo
 ***                     non viene considerato.
 ***     char *percorso : se flag e' False questo parametro indica il percorso
 ***                      delle icone da copiare. 
 *** Copia i file bitmap di un modulo dalla lib_graf standard o di altro utente
 *** alla lib_graf utente. Aggiorna anche il file lib_graf/icon_list.dat
 *** ( chiamando aggiungi_record_icona() )
 ***/
copia_nella_libgraf(nome_mod, descr_mod, tipo_copia, flag, percorso)
char *nome_mod, *descr_mod;
byte tipo_copia;
int flag;
char *percorso;
{
   char path_graf[256], file_dest[256], file_orig[256];
   char modulo_da_cercare[5];
   long int std_offset = 0;
   short int num_icone, i, icons_copied = 0;
   Boolean modulo_inedito = True;
   IconFileRec rec_icone;
   FILE *fp_ico;

/* Se bisogna copiare solo il FORTRAN , controlla che nel file icon_list.dat */
/* non esista gia' il modulo. In tal caso si puo' uscire in quanto i file */
/* bitmap non devono essere copiati */
   if (tipo_copia == ONLY_FORTRAN)
   {
      if ((fp_ico = apri_file_icoproc(CREA_FALSE, LIB_UTENTE)) == NULL)
      {
         sprintf(message,"Can't open icon file %s", FILE_ICONE_MODP);
         attention_wdg = (Widget) attention (UxTopLevel, message, MAPPA,
                                             geom_attention);
         scrivi_messaggio (message);
         return;
      }

   /* Se nel file icon_list.dat esiste il record relativo al modulo si */
   /* puo' sciortire */
      if (cerca_modulo_proc(fp_ico, &rec_icone, nome_mod) != RETURN_ERROR)
      {
         fclose(fp_ico);
         return;
      }
   }

/* Composizione filename  e path della icon_list appropriata (standard */
/* oppure  altro utente) */
   if ( flag )
   {
/*
       percorso_libreria(path_graf, LIBGRAF, LIB_STANDARD);
*/

       if ( flag == STDLIBBASE_MODULES_COPY )
          percorso_libreria(path_graf, LIBGRAF, LIB_STANDARD_BASE );
       else if ( flag == STDLIBAUXILIARY_MODULES_COPY )
          percorso_libreria(path_graf, LIBGRAF, LIB_STANDARD_AUXILIARY );
       else if ( flag == STDLIBNUCLEAR_MODULES_COPY )
          percorso_libreria(path_graf, LIBGRAF, LIB_STANDARD_NUCLEAR );
       else if ( flag == STDLIBSPECIAL_MODULES_COPY )
          percorso_libreria(path_graf, LIBGRAF, LIB_STANDARD_SPECIAL );
   }
   else
   {
#ifdef VMS
      sottodir_vms(path_graf,percorso,PATH_LIBGRAF_UTE);
#else
      sprintf(path_graf,"%s%s/",percorso,PATH_LIBGRAF_UTE);
#endif
   }

   sprintf(file_orig,"%s%s",path_graf,FILE_ICONE_MODP);

   printf("file icone sorgente: %s\n",file_orig);

   strcpy (modulo_da_cercare,nome_mod);

/* Se il file icon_list.dat (DA LEGGERE)  si puo' aprire e se contiene il */
/* modulo in questione */
   if ( tipo_copia != ONLY_FORTRAN &&
        (fp_ico = fopen(file_orig,"r")) != NULL &&
        (std_offset = cerca_modulo_proc (fp_ico, &rec_icone, 
                                         modulo_da_cercare)) != RETURN_ERROR )
   {
   /* Recupera il malloppo di record delle icone del modulo */
      fseek (fp_ico, std_offset, SEEK_SET);
      leggi_record_file_ico (fp_ico,&rec_icone);
      fclose (fp_ico);
   }
   else
   {
   /* Riempie le altre informazioni essenziali per la struttura */
      strcpy (rec_icone.nome_modulo,nome_mod);
      strcpy (rec_icone.descr_modulo,descr_mod);
      rec_icone.num_icone = 0;
   }

   printf("numero icone: %d\n",rec_icone.num_icone);
 
/* Inserimento del record alla posizione opportuna */
   if (aggiungi_record_file_ico(nome_mod,&rec_icone,LIB_UTENTE) == RETURN_ERROR) 
   {
      sprintf (message," Can't update file %s.", FILE_ICONE_MODP);
      attention_wdg = (Widget) attention (UxTopLevel, message, MAPPA,
                                          geom_attention);
      scrivi_messaggio (message);
      return;
   }

/* Se e' selezionato ONLY_FORTRAN non si procede alla copia degli eventuali */
/* file bmp (bitmap) */
   if ( tipo_copia == ONLY_FORTRAN )
      return;

/* Copia gli n file bitmap associati al nuovo modulo */
   for (i=0; i<MAX_ICON_X_PMODULE; i++)
      if (rec_icone.bmap_record[i].esiste)
      {
      /* Composizione filename sorgente */
         sprintf(file_orig,"%s%s",path_graf,rec_icone.bmap_record[i].nome_file);

      /* Composizione filename destinazione */
         percorso_libreria(file_dest, LIBGRAF, LIB_UTENTE);
/* questo non serve piu'
#ifndef VMS
         strcat(file_dest, "/");
#endif
*/
         strcat(file_dest, rec_icone.bmap_record[i].nome_file);

         if ( copia_file (file_orig,file_dest) )
             sprintf (message,"Failed to copy bitmap file %s !",file_dest);
         else
             sprintf (message,"bitmap file %s copied.",
                              rec_icone.bmap_record[i].nome_file);
         scrivi_messaggio (message);
         if (++icons_copied == rec_icone.num_icone)
            break;
      }

/* Esito del tutto */
   sprintf (message," %d icons associated to %s.",icons_copied, nome_mod);
   scrivi_messaggio (message);
}

/*-----------------------------------------------------------------------*/
/*** cambia_filebmp_record (record)
 ***
 *** Chiamata quando si cambia il nome di un modulo durante la copia.
 *** Aggiorna i bitmap filenames nel record di icon_list.dat
 *** (la funzione, attualmente, NON E' UTILIZZATA).
 ***/
cambia_filebmp_record (record)
IconFileRec *record;
{
   int  i;
   char kuskus[15];

/* Ciclo di lettura per i record di ogni icona */
   for(i = 0; i<MAX_ICON_X_PMODULE; i++)
   {
      sprintf (kuskus,"%s%s",record->nome_modulo,
                record->bmap_record[i].nome_file+4);
      tominus (kuskus);
      strcpy (record->bmap_record[i].nome_file, kuskus);
   }
}


/*****************************************************************/
/***
 *** void setta_label_modsel(modulo)
 ***
 *** Aggiorna la label del modulo selezionato nella main window.
 ***/
void setta_label_modsel(nome_e_descr)
char *nome_e_descr;
{
   int num;

   set_label(UxGetWidget(selected_module_label),nome_e_descr);
   cstring = CREATE_CSTRING(nome_e_descr);
   XmListSelectItem(UxGetWidget(lista_moduli_utente), cstring, False);
   XmStringFree(cstring);

   recupera_nome_e_descr(nome_e_descr, nome_modulo, descr_modulo);

   if (menu_sbloccato == False)
      menu_modulo_attivo();
}

/************************************************************************/
/***
 *** rimuovi_tag_record(tipo_lib, offset)
 ***   Parametri:
 ***     byte tipo_lib : tipo libreria (LIBUT, LIBUTREG, LIBREG)
 ***     long int offset : posizione del record nel file
 ***
 *** Rimuove il tag '!' alla posizione 6 del record relativo al modulo
 *** copiato dall'utente selezionando 'Yes' dalla dbox di conferma
 ***/
rimuovi_tag_record(tipo_lib, offset)
byte tipo_lib;
long int offset;
{
   FILE *fp_lista;
   char buffer[100], file_lista[256];

   file_dati_lib( file_lista, tipo_lib, LIB_UTENTE );
   if ((fp_lista = fopen(file_lista,"r+")) == NULL)
   {
      sprintf(message," Can't open file %s for updating.",file_lista);
      attention_wdg = (Widget) attention (UxTopLevel, message, MAPPA,
                                          geom_attention);
      return;
   }

   fseek(fp_lista, offset, SEEK_SET);
   fgets(buffer,80,fp_lista);

   buffer[5] = ' ';

   fseek(fp_lista, offset, SEEK_SET);
   fputs(buffer,fp_lista);

   fclose(fp_lista);
}


/************************************************************************/
/***
 *** apponi_tag_record(tipo_lib, offset)
 ***   Parametri:
 ***     byte tipo_lib : tipo libreria (LIBUT, LIBUTREG, LIBREG)
 ***     long int offset : posizione del record nel file
 ***
 *** Appone il tag '!' alla posizione 6 del record relativo al modulo
 *** modificato dall'utente selezionando Module-->Modify-->FORTRAN code
 ***/
apponi_tag_record(tipo_lib, offset)
byte tipo_lib;
long int offset;
{
   char buffer[100], file_lista[256];
   FILE *fp_lista;

   file_dati_lib( file_lista, tipo_lib, LIB_UTENTE );
   if ((fp_lista = fopen (file_lista,"r+")) == NULL)
   {
      sprintf(message," Can't open file %s for updating.",file_lista);
      attention_wdg = (Widget) attention (UxTopLevel, message, MAPPA,
                                          geom_attention);
      scrivi_messaggio (message);
      return;
   }

   fseek (fp_lista, offset, SEEK_SET);
   fgets (buffer,80,fp_lista);

/* Precauzione per i moduli privi di descrizione */
   if ( strlen(buffer) <= 6 )
   {
       shift_file_bytes (fp_lista, STRMODULE_MIN_LENGTH-strlen(buffer));

    /* Se il 5^ carattere e' '*' (=ESISTE FORTRAN AUSILIARIO MODULO) */
    /* va conservato */
       if (buffer[4] == '*')
          sprintf(buffer,"%.4s* \n",buffer);
       else
          sprintf(buffer,"%.4s  \n",buffer);
   }

   buffer[5] = '!'; 
   fseek (fp_lista, offset, SEEK_SET);
   fputs (buffer,fp_lista);

   fclose  (fp_lista);
}

/*-----------------------------------------------------------------------*/
/*** void cambia_tipo_copia (w,tipo,struct)
 ***
 *** Callback invocata agendo sull'optionMenu della copia di moduli
 *** dalla standard lib.
 ***/
void cambia_tipo_copia (w,tipo,zip)
Widget w;
byte tipo;
XmPushButtonCallbackStruct *zip;
{

   if (tipo == tipo_copia)
      return;

   tipo_copia = tipo;
}
 
/*** recupera_nome_e_descr(record, nome, descr)
 ***   Parametri:
 ***     char *record : record lista_moduli.dat
 ***     char *nome, *descr : nome e descrizione modulo (uscita)
La funzione recupera il nome e la descrizione del modulo dal record del file
lista_moduli.dat ***/
recupera_nome_e_descr(record, nome, descr)
char *record, *nome, *descr;
{
   sprintf ( nome,"%.4s", record);

/* Precauzione per i moduli privi di descrizione */
   if (strlen(record) > 6)
      sprintf (descr,"%.80s", record+6);
   else 
      strcpy(descr,"");
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_pb_listamod_ok( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_lista_mod      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_lista_modContext;
	UxDbox_lista_modContext = UxContext =
			(_UxCdbox_lista_mod *) UxGetContext( UxWidget );
	{
#ifdef tipo_libreria
	/* Gestisci la selezione del modulo */
	   if (modulo_selezionato(tipo_libreria, flag_list) == 0)
	   {
	       *flag_manage = False;
	/*
	       UxDestroySwidget(dbox_lista_mod);
	*/
	       UxDestroyInterface(dbox_lista_mod);
	   }
#endif
	}
	UxDbox_lista_modContext = UxSaveCtx;
}

static void	activateCB_pb_listamod_cancel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_lista_mod      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_lista_modContext;
	UxDbox_lista_modContext = UxContext =
			(_UxCdbox_lista_mod *) UxGetContext( UxWidget );
	{
	/* UxDestroySwidget(dbox_lista_mod); */
	 *flag_manage = False;
	 UxDestroyInterface(dbox_lista_mod);
	}
	UxDbox_lista_modContext = UxSaveCtx;
}

static void	multipleSelectionCB_scrolledList_moduli( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_lista_mod      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_lista_modContext;
	UxDbox_lista_modContext = UxContext =
			(_UxCdbox_lista_mod *) UxGetContext( UxWidget );
	{
	char item_label[4];
	
	get_something (UxGetWidget(scrolledList_moduli),XmNselectedItemCount,
	               &selected_item);
	
	sprintf (item_label,"%3d",selected_item);
	cstring = CREATE_CSTRING(item_label);
	set_something_val (UxGetWidget(label_select_item), XmNlabelString, (XtArgVal) cstring);
	XmStringFree(cstring);
	}
	UxDbox_lista_modContext = UxSaveCtx;
}

static void	singleSelectionCB_scrolledList_moduli( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_lista_mod      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_lista_modContext;
	UxDbox_lista_modContext = UxContext =
			(_UxCdbox_lista_mod *) UxGetContext( UxWidget );
	{
	char item_label[4];
	
	get_something (UxGetWidget(scrolledList_moduli), XmNselectedItemCount,
	               &selected_item);
	
	sprintf (item_label,"%3d",selected_item);
	cstring = CREATE_CSTRING(item_label);
	set_something_val (UxGetWidget(label_select_item),XmNlabelString, (XtArgVal) cstring);
	XmStringFree(cstring);
	}
	UxDbox_lista_modContext = UxSaveCtx;
}

static void	defaultActionCB_scrolledList_moduli( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_lista_mod      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_lista_modContext;
	UxDbox_lista_modContext = UxContext =
			(_UxCdbox_lista_mod *) UxGetContext( UxWidget );
	{
#ifdef tipo_libreria
	/* Gestisci la selezione del modulo */
	   if (modulo_selezionato(tipo_libreria, flag_list) == 0)
	   {
	      UxDestroySwidget(dbox_lista_mod);
	      *flag_manage = False;
	   }
#endif
	}
	UxDbox_lista_modContext = UxSaveCtx;
}

static void	activateCB_deselect_all_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_lista_mod      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_lista_modContext;
	UxDbox_lista_modContext = UxContext =
			(_UxCdbox_lista_mod *) UxGetContext( UxWidget );
	{
	/* Deseleziona tutti gli items selezionati nella scrolledList dei moduli */
	XmListDeselectAllItems (UxGetWidget(scrolledList_moduli));
	
	/* Azzera il contatore degli items selezionati */
	set_something_val (UxGetWidget(label_select_item),XmNlabelString,
	               CREATE_CSTRING("0"));
	}
	UxDbox_lista_modContext = UxSaveCtx;
}

static void	activateCB_select_all_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCdbox_lista_mod      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDbox_lista_modContext;
	UxDbox_lista_modContext = UxContext =
			(_UxCdbox_lista_mod *) UxGetContext( UxWidget );
	{
	int num_items, i;
	
	/* Seleziona tutti gli item della scrolledList dei moduli */
	get_something (UxGetWidget(scrolledList_moduli),XmNitemCount,(void*) &num_items);
	
	for (i=0; i<num_items; i++)
	   XmListSelectPos (UxGetWidget(scrolledList_moduli),i,True);
	}
	UxDbox_lista_modContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_dbox_lista_mod()
{
	Widget	dbox_lista_mod_shell;
	Widget	opt_tipocopia_shell;

	dbox_lista_mod_shell = XtVaCreatePopupShell( "dbox_lista_mod_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 600,
			XmNy, 185,
			XmNwidth, 650,
			XmNheight, 400,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "dbox_lista_mod",
			NULL );

	dbox_lista_mod = XtVaCreateWidget( "dbox_lista_mod",
			xmFormWidgetClass, dbox_lista_mod_shell,
			XmNautoUnmanage, FALSE,
			XmNdefaultPosition, FALSE,
			RES_CONVERT( XmNdialogTitle, "MODULES LIST" ),
			XmNheight, 400,
			XmNwidth, 650,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( dbox_lista_mod, (char *) UxDbox_lista_modContext );

	pb_listamod_ok = XtVaCreateManagedWidget( "pb_listamod_ok",
			xmPushButtonWidgetClass, dbox_lista_mod,
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNalignment, XmALIGNMENT_CENTER,
			XmNheight, 30,
			XmNwidth, 50,
			XmNy, 310,
			XmNx, 10,
			NULL );

	UxPutContext( pb_listamod_ok, (char *) UxDbox_lista_modContext );

	pb_listamod_cancel = XtVaCreateManagedWidget( "pb_listamod_cancel",
			xmPushButtonWidgetClass, dbox_lista_mod,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftWidget, pb_listamod_ok,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 70,
			XmNy, 310,
			XmNx, 90,
			NULL );

	UxPutContext( pb_listamod_cancel, (char *) UxDbox_lista_modContext );

	label_lista_mod = XtVaCreateManagedWidget( "label_lista_mod",
			xmLabelWidgetClass, dbox_lista_mod,
			XmNrightPosition, 60,
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 20,
			XmNwidth, 480,
			XmNy, 10,
			XmNx, 10,
			NULL );

	UxPutContext( label_lista_mod, (char *) UxDbox_lista_modContext );

	separatorGadget5 = XtVaCreateManagedWidget( "separatorGadget5",
			xmSeparatorGadgetClass, dbox_lista_mod,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, pb_listamod_ok,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 10,
			XmNwidth, 470,
			XmNy, 350,
			XmNx, 20,
			NULL );

	UxPutContext( separatorGadget5, (char *) UxDbox_lista_modContext );

	form7 = XtVaCreateManagedWidget( "form7",
			xmFormWidgetClass, dbox_lista_mod,
			XmNrightOffset, 10,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, separatorGadget5,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 35,
			XmNwidth, 550,
			XmNy, 320,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form7, (char *) UxDbox_lista_modContext );

	scrolledWindow1 = XtVaCreateManagedWidget( "scrolledWindow1",
			xmScrolledWindowWidgetClass, dbox_lista_mod,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, form7,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNtopWidget, label_lista_mod,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 40,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow1, (char *) UxDbox_lista_modContext );

	scrolledList_moduli = XtVaCreateManagedWidget( "scrolledList_moduli",
			xmListWidgetClass, scrolledWindow1,
			XmNlistSizePolicy, XmRESIZE_IF_POSSIBLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNselectionPolicy, XmMULTIPLE_SELECT,
			XmNfontList, UxConvertFontList( "-Adobe-Courier-Bold-R-Normal--14-140-75-75-M-90-ISO8859-1" ),
			XmNheight, 260,
			XmNwidth, 480,
			NULL );

	UxPutContext( scrolledList_moduli, (char *) UxDbox_lista_modContext );

	form_counter = XtVaCreateManagedWidget( "form_counter",
			xmFormWidgetClass, dbox_lista_mod,
			XmNtopWidget, NULL,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 400,
			XmNy, 360,
			XmNx, 370,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form_counter, (char *) UxDbox_lista_modContext );

	frame5 = XtVaCreateManagedWidget( "frame5",
			xmFrameWidgetClass, form_counter,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 20,
			XmNwidth, 30,
			XmNy, 5,
			XmNx, 70,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( frame5, (char *) UxDbox_lista_modContext );

	label_select_item = XtVaCreateManagedWidget( "label_select_item",
			xmLabelWidgetClass, frame5,
			XmNrecomputeSize, FALSE,
			RES_CONVERT( XmNforeground, "yellow" ),
			RES_CONVERT( XmNlabelString, "0" ),
			XmNheight, 10,
			XmNwidth, 40,
			XmNy, 10,
			XmNx, 10,
			NULL );

	UxPutContext( label_select_item, (char *) UxDbox_lista_modContext );

	labelGadget7 = XtVaCreateManagedWidget( "labelGadget7",
			xmLabelGadgetClass, form_counter,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightWidget, frame5,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Selected items: " ),
			XmNheight, 20,
			XmNwidth, 120,
			XmNy, 0,
			XmNx, -170,
			NULL );

	UxPutContext( labelGadget7, (char *) UxDbox_lista_modContext );

	deselect_all_pb = XtVaCreateManagedWidget( "deselect_all_pb",
			xmPushButtonWidgetClass, form_counter,
			XmNrightWidget, labelGadget7,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Deselect all" ),
			XmNheight, 30,
			XmNwidth, 100,
			XmNy, 0,
			XmNx, 35,
			NULL );

	UxPutContext( deselect_all_pb, (char *) UxDbox_lista_modContext );

	select_all_pb = XtVaCreateManagedWidget( "select_all_pb",
			xmPushButtonWidgetClass, form_counter,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightWidget, deselect_all_pb,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNbottomOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Select all" ),
			XmNheight, 30,
			XmNwidth, 90,
			XmNy, -5,
			XmNx, 34,
			NULL );

	UxPutContext( select_all_pb, (char *) UxDbox_lista_modContext );

	opt_tipocopia_shell = XtVaCreatePopupShell ("opt_tipocopia_shell",
			xmMenuShellWidgetClass, dbox_lista_mod,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	opt_tipocopia = XtVaCreateWidget( "opt_tipocopia",
			xmRowColumnWidgetClass, opt_tipocopia_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( opt_tipocopia, (char *) UxDbox_lista_modContext );

	copy_both_pb = XtVaCreateManagedWidget( "copy_both_pb",
			xmPushButtonWidgetClass, opt_tipocopia,
			RES_CONVERT( XmNlabelString, "copy both" ),
			NULL );

	UxPutContext( copy_both_pb, (char *) UxDbox_lista_modContext );

	copy_fortran_pb = XtVaCreateManagedWidget( "copy_fortran_pb",
			xmPushButtonWidgetClass, opt_tipocopia,
			RES_CONVERT( XmNlabelString, "only Fortran" ),
			NULL );

	UxPutContext( copy_fortran_pb, (char *) UxDbox_lista_modContext );

	copy_icons_pb = XtVaCreateManagedWidget( "copy_icons_pb",
			xmPushButtonWidgetClass, opt_tipocopia,
			RES_CONVERT( XmNlabelString, "only icon(s)" ),
			NULL );

	UxPutContext( copy_icons_pb, (char *) UxDbox_lista_modContext );

	menu_tipocopia = XtVaCreateManagedWidget( "menu_tipocopia",
			xmRowColumnWidgetClass, dbox_lista_mod,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNx, 470,
			RES_CONVERT( XmNlabelString, "Copy mode:" ),
			XmNsubMenuId, opt_tipocopia,
			XmNrowColumnType, XmMENU_OPTION,
			NULL );

	UxPutContext( menu_tipocopia, (char *) UxDbox_lista_modContext );

	XtAddCallback( dbox_lista_mod, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxDbox_lista_modContext );

	XtVaSetValues(dbox_lista_mod,
			XmNdefaultButton, NULL,
			NULL );

	XtAddCallback( pb_listamod_ok, XmNactivateCallback,
			activateCB_pb_listamod_ok,
			(XtPointer) UxDbox_lista_modContext );

	XtAddCallback( pb_listamod_cancel, XmNactivateCallback,
			activateCB_pb_listamod_cancel,
			(XtPointer) UxDbox_lista_modContext );

	XtAddCallback( scrolledList_moduli, XmNmultipleSelectionCallback,
			multipleSelectionCB_scrolledList_moduli,
			(XtPointer) UxDbox_lista_modContext );
	XtAddCallback( scrolledList_moduli, XmNsingleSelectionCallback,
			singleSelectionCB_scrolledList_moduli,
			(XtPointer) UxDbox_lista_modContext );
	XtAddCallback( scrolledList_moduli, XmNdefaultActionCallback,
			defaultActionCB_scrolledList_moduli,
			(XtPointer) UxDbox_lista_modContext );

	XtAddCallback( deselect_all_pb, XmNactivateCallback,
			activateCB_deselect_all_pb,
			(XtPointer) UxDbox_lista_modContext );

	XtAddCallback( select_all_pb, XmNactivateCallback,
			activateCB_select_all_pb,
			(XtPointer) UxDbox_lista_modContext );

	XtAddCallback( copy_both_pb, XmNactivateCallback,
			cambia_tipo_copia,
			(XtPointer) COPY_BOTH );

	XtAddCallback( copy_fortran_pb, XmNactivateCallback,
			cambia_tipo_copia,
			(XtPointer) ONLY_FORTRAN );

	XtAddCallback( copy_icons_pb, XmNactivateCallback,
			cambia_tipo_copia,
			(XtPointer) ONLY_ICONS );



	return ( dbox_lista_mod );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_lista_moduli( _Uxfp, _Uxtipo_libreria, _Uxflag_list, _Uxflag_manage )
	FILE	*_Uxfp;
	byte	_Uxtipo_libreria;
	byte	_Uxflag_list;
	Boolean	*_Uxflag_manage;
{
	Widget                  rtrn;
	_UxCdbox_lista_mod      *UxContext;

	UxDbox_lista_modContext = UxContext =
		(_UxCdbox_lista_mod *) XtMalloc( sizeof(_UxCdbox_lista_mod) );

	fp = _Uxfp;
	tipo_libreria = _Uxtipo_libreria;
	flag_list = _Uxflag_list;
	flag_manage = _Uxflag_manage;
	{
		char *titolo_finestra;
		byte tipo_selezione;
		Widget list_find_form;
		rtrn = _Uxbuild_dbox_lista_mod();

		/* Riempie la scrolledList con i record opportuni */
		  riempi_lista_moduli (fp,UxGetWidget(scrolledList_moduli));
		
		/* Abilita la selezione opportuna e aggiorna il titolo */
		  switch (flag_list)
		  {
		     case USER_MODULES_SELECTION:
		          tipo_selezione = XmMULTIPLE_SELECT;
		          titolo_finestra = "USER MODULES SELECTION";
		          break;
		
		     case STDLIB_MODULES_DOC:
		          tipo_selezione = XmSINGLE_SELECT;
		          titolo_finestra = "STANDARD MODULES SELECTION for documentation";
		          break;
		
		     case STDLIB_MODULES_COPY:
		          tipo_selezione = XmMULTIPLE_SELECT;
		          titolo_finestra = "STANDARD MODULE(S) COPY";
		          break;
		
		     case STDLIBBASE_MODULES_COPY:
		          tipo_selezione = XmMULTIPLE_SELECT;
		          titolo_finestra = "STANDARD BASE MODULE(S) COPY";
		          break;
		
		     case STDLIBAUXILIARY_MODULES_COPY:
		          tipo_selezione = XmMULTIPLE_SELECT;
		          titolo_finestra = "STANDARD AUXILIARY MODULE(S) COPY";
		          break;
		
		     case STDLIBNUCLEAR_MODULES_COPY:
		          tipo_selezione = XmMULTIPLE_SELECT;
		          titolo_finestra = "STANDARD NUCLEAR MODULE(S) COPY";
		          break;
		
		     case STDLIBSPECIAL_MODULES_COPY:
		          tipo_selezione = XmMULTIPLE_SELECT;
		          titolo_finestra = "STANDARD SPECIAL MODULE(S) COPY";
		          break;
		  }
		
		  set_something_val(UxGetWidget(scrolledList_moduli),XmNselectionPolicy,
		                                                 tipo_selezione);
		  set_label(UxGetWidget(label_lista_mod),titolo_finestra);
		
		/* Inserimento find di libreria */
		
		  parametri_list.ID_lista = UxGetWidget(scrolledList_moduli);
		  parametri_list.num_elem = 100;
		  parametri_list.vis_elem = 1;
		  parametri_list.pos      = 0;
		
		  argcount=0;
		  XtSetArg(args[argcount],XmNleftAttachment,XmATTACH_FORM); argcount++;
		  XtSetArg(args[argcount],XmNleftOffset,0); argcount++;
		  XtSetArg(args[argcount],XmNrightAttachment,XmATTACH_FORM); argcount++;
		  XtSetArg(args[argcount],XmNrightOffset,0); argcount++;
		  XtSetArg(args[argcount],XmNbottomAttachment,XmATTACH_FORM); argcount++;
		  XtSetArg(args[argcount],XmNbottomOffset,0); argcount++;
		  XtSetArg(args[argcount],XmNtopAttachment,XmATTACH_FORM); argcount++;
		  XtSetArg(args[argcount],XmNtopOffset,0); argcount++;
		
		  list_find_form = find_kit(UxGetWidget(form7),args,argcount,&parametri_list);
		
		  UxPopupInterface(rtrn, no_grab);
		  *flag_manage = True;
		
		/* Visualizza il conta-moduli e l'optionMenu solo se si sta copiando dalla */
		/* libreria standard */
		  if ( tipo_selezione == XmSINGLE_SELECT )
		     XtUnmanageChild (UxGetWidget(form_counter));
		  else
		  {
		  /* Label contatore inizializzata "0" */
		     cstring = CREATE_CSTRING("  0  ");
		     set_something_val (UxGetWidget(label_select_item), XmNlabelString, (XtArgVal) cstring);
		     XmStringFree(cstring);
		  }
		
		  tipo_copia = COPY_BOTH;
		
		/* Nelle librerie LIBUTREG e LIBREG non sono previste copie di icone ... */
		  if (tipo_libreria != LIBUT || flag_list == STDLIB_MODULES_DOC)
		     XtUnmanageChild (UxGetWidget(menu_tipocopia));
		
		  return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_lista_moduli( _Uxfp, _Uxtipo_libreria, _Uxflag_list, _Uxflag_manage )
	FILE	*_Uxfp;
	byte	_Uxtipo_libreria;
	byte	_Uxflag_list;
	Boolean	*_Uxflag_manage;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_lista_moduli( _Uxfp, _Uxtipo_libreria, _Uxflag_list, _Uxflag_manage );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

