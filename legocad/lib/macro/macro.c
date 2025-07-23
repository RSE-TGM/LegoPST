/**********************************************************************
*
*       C Source:               macro.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Mar 11 16:17:29 2004 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: macro.c-8 %  (%full_filespec: macro.c-8:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)macro.c	2.14\t4/26/95";
/*
        Fine sezione per SCCS
*/

/*  MACRO.C	PROVE TECNICHE DI TRASMISSIONE
		Costruzione di un modello LEGO in modalita' grafica --
		gestione di blocchi, macroblocchi & magolli...

    Versione 0.1          5.11.92           Buffo & Calleri	    */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#ifdef VMS

#include <unixio.h>

#endif

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/cursorfont.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/SeparatoG.h>
#include <Xm/DialogS.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/BulletinB.h>
#include <Xm/ToggleB.h>

#include "libutilx.h"
#include "macro.h"
#include "utile.h"
#include "files.h"
#include "file_icone.h"
#include "ico_mesg.h"
 
#ifdef TOPOLOGIA
#include "lg1.h"  /* non so se serve realmente */
#endif


/*******************************************/
/* DEFINIZIONE  DELLE  VARIABILI  GLOBALI  */
/*******************************************/

XtActionsRec actions[] = { { "deselect_all_macro", deselect_all_macro },
			   { "select_macro", select_macro },
			   { "deselect_all_block", deselect_all_block },
#ifdef DATI
			 { "desel_all_page_lista", desel_all_page_lista },
#endif
			   { "manage_select_block", manage_select_block } };

/* messaggi di errore */
char *error_mesg[] = {   /* MESSAGGI SUL TERMINALE stderr */
                        "You must set UIDLEGO with correct pathname.",
                        "Error while opening hierarchy.",
                        "Can't open dialog box ! ",
                        "Open file module list failed!",
                        "Icon file list creation failed!",
                        "Open icon list file failed!",
                        "Open scheme list file failed!",
                        "Open icon standard file failed!",
			"Can't create file macroblocks.dat.",
                        "Symbol file list creation failed!",
                        "No symbol defined." };


/* messaggi di warning */
char *warning_mesg[] = {   /* MESSAGGI SU FINESTRELLA MOTIF */
                        "Can't clear bitmap file",
                        "Can't add a new icon for this module.",
                        "Can't create file",
                        "Can't copy from inexistent file.",
                        "Can't read bitmap file. Check permissions.",
                        "The selected module does not exist.",
	           	"Reached maximum of open block windows. Kill one or more.",
			"Can't clean up dirty ass.",
               		"The named macroblock already exists.",
                        "Set block name!",
                        "The mouse pointer isn't in the correct window.",
			"Can't open icon list file.",
			"Can't open icon standard file.",
			"Can't read bitmap file.",
                        "There are no symbol(s) defined.",
			"Can't open file f01.dat.",
			"Can't grab pointer or keyboard.",
                        "Block name already defined. Change it.",
                        "Maximum number of block reached. Sorry, i can't add it." };

/* VARIABILI UTILIZZATE NELLE FUNZIONI Xlib */
Display *display;
int screen;
Cursor cursore_a_crocetta;
Pixmap icona;
GC xorGC, iconGC, lineGC;

/***********************************************/
/* D E F I N I Z I O N E   D E I   C O L O R I */
/***********************************************/
/*** 	N.B.
   6	colori canonici
   1	colore per il rubber banding
  10	colori per i macroblocchi selezionati e per le rispettive finestre
***/

char *names[] = { "white", "black", "yellow", "green", "red", "blue",
		  "light grey", 
		  "pale green","medium turquoise", "light blue" ,
		  "pink" };

int   num_colors = (sizeof names / sizeof names[0]);
Pixel apix[50];
Colormap colormap;

/* VARIABILI DI WIDGET */

Widget top_level, main_window, widget_list_blocchi, dialog_shell;
Widget warning_widget, dialog_uscita;

/********************************************************************/
/* Strutture riguardanti il geometry management delle varie Dialog...
   Specificare, nell'ordine: { DefaultPosition TRUE/FALSE,XmNx,XmNy,
                               XmNwidth,XmNHeight }                 */
Dialog_geometry geom_attention   = { TRUE, NULL, NULL, NULL, 150 };
/********************************************************************/

Widget macro_tavola;
Widget dbx_new_macro, text_nome_new, text_descr_new;
Widget dbx_modify_macro, text_nome_mod, text_descr_mod;
Widget dbx_nuovo_blocco, text_nome_blocco, text_descr_blocco;
Widget dbx_new_rem, text_new_rem;
Widget dbx_modify_rem, text_modify_rem;
Widget dbx_modify_block, text_mod_nome_blk, text_mod_descr_blk;
Widget tavola_icone, tavola_icone2, lista_moduli, scroll_icone, scroll_icone2;
Widget popup_mcr, pop_edit_macro[7], menu_edit_macro[7];
Widget dbx_simboli, tavola_simboli, scroll_simboli;
Widget dbx_setup_line, text_line_thick, toggle_arrow;
Widget text_arrow_length, text_arrow_width;
Widget dbx_modify_snap, text_snap_val;

static Elenco_callback conferma_uscita = {
			      {	"Yes",    quit_session, 0 },
                              { "No" ,    quit_session, 1 },
			      { "Cancel" ,quit_session, 2 } };
Widget dialog_conferma_delete;

/**************************************************************/
/* VARIABILI GLOBALI UTILIZZATE NELLO SPOSTAMENTO DELLE LABEL */
/* ed altre amenita' */
int mousex0, mousey0;
int mouseprecx, mouseprecy, minposx, maxposx, minposy, maxposy;
Dimension xoffs, yoffs;
int bottone_premuto = -1;
DimWidgets *dim_widgets; /* variabile che contiene le dimensioni e le */
int num_dim_wdg;       /* posizioni degli oggetti (blocchi o macroblocchi) */
		       /* da spostare */
/**************************************************************/

/* Strutture per la gestione dei macroblocchi e dei blocchi ad essi associati */
int num_macro = 0, macro_allocate = 0;
MacroBlockType *macroblocks;
int ind_macro_tmp;

/* informazioni relative alle pixmap dei moduli di processo */
PixmapModuleStruct *pixmap_module;
int num_moduli = 0;

/* informazioni relative alle pixmap dei moduli di regolazione */
PixmapModuleStruct *pixmap_mod_schema;
int num_mod_schema = 0;

/* utilizzata nella visualizzazione della lista dei moduli (indica se la */
/* lista si riferisce ai moduli di processo oppure di regolazione) */
short int tipo_modulo = TP_BLOCK;

/* informazioni relative alle pixmap dei simboli */
PixmapSymbolStruct pixmap_symbol;

/* icona standard moduli di regolazione */
PixmapInfo pix_scheme_std;

/* ALTRE VARIABILI GLOBALI */
int tipo_modello; /* identifica la tipologia del modello:schema o processo */

/* variabili utilizzati per la modifica delle icone dei blocchi */
PixmapInfo *pixm_ico_selez;
Widget wdg_ico_selez;

int num_macro_selez = 0, macro_selez_tmp = -1;
BlockType *ptr_blk_mod, *ptr_rem_tmp;

/* posizione iniziale del macroblocco appena creato */
int posx_macro = 0, posy_macro = 0;

/* dati relativi alle linee grafiche */
Boolean disegna_freccia = True;
unsigned int spessore_linea = 2;
Pixel colore_linea = 1;
unsigned int lung_freccia = 12;
unsigned int larg_freccia = 6;

XmString stringa_nulla;  /* compound string vuota! */
XmString cstring;
char nome_modulo_selez[5];
char NomeModello[20];

/* percorso libreria utente */
char Path_Legocad[500];

/* snap */
int snap_val = PIXEL_STEP;
Boolean snap_status = True;
int num_win_blocchi = 0;

/* flag di modifica impianto */
Boolean modifiche = False;

Arg args[20];
Cardinal nargs;

#ifdef TOPOLOGIA
/******************** FUNZIONI EXTERNAL ******************/

extern int write_f01();

extern int bl_var_cb(); /* visualizzazione finestra delle variabili di un blocco
                           definita in lg1 */


extern int block_selection_activate(); /* attivazione interfaccia inserimento
                                          nuovo blocco , definita in BlockSelect                                          ionBox */ 


extern del_block_from_list(); /* effettua la cancellazione di un blocco 
                                 dalla lista dei blocchi in lg1 */

#endif

/********  INIZIO MAIN()  ********/
#ifdef MAIN

main(argc, argv)
int argc;
char *argv[];

#else

lancia_macro(toplevel_appl, listaBlocchi)
Widget toplevel_appl, listaBlocchi;

#endif

{
   XtAppContext app_context;

#ifdef MAIN

/* Inizializzazione del Toolkit */
   top_level = XtInitialize(argv[0], "Macro", NULL,  0, &argc, argv);

/* Definizione titolo e nome icona */
   nargs = 0;
   XtSetArg(args[nargs],XmNminWidth,500); nargs++;
   XtSetArg(args[nargs],XmNminHeight,400); nargs++;
   XtSetArg(args[nargs],XmNtitle,MACRO_WINDOW_TITLE); nargs++;
   XtSetArg(args[nargs],XmNiconName,APPLICATION_NAME); nargs++;
   XtSetValues (top_level,args,nargs);

#else
  
   top_level = toplevel_appl;
   widget_list_blocchi = listaBlocchi;

#endif

   app_context = XtWidgetToApplicationContext(top_level);
   XtAppAddActions(app_context, actions, XtNumber(actions));

/* Variabile che da' di comodo ... */
   stringa_nulla = CREATE_CSTRING("");

/* percorso libreria utente */
/************************************************
   path_legocad = getenv("LEGOCAD_USER");
   if (path_legocad == NULL)
#ifdef VMS
      path_legocad = "[]";
#else
      path_legocad = ".";
#endif
****************************************************/
   strcpy(Path_Legocad , getenv("LEGOCAD_USER"));
   if( !strlen(Path_Legocad) )
#ifdef VMS
      strcpy(Path_Legocad , "[]");
#else
      strcpy(Path_Legocad , ".");
#endif


   display = XtDisplay(top_level);
   screen  = DefaultScreen(display);

/* definisci il cursore a crocetta (utilizzato durante i GRAB */
   cursore_a_crocetta = XCreateFontCursor(display, XC_crosshair);

/* Recupera la Colormap e allocca i colori di fg e bg  */
   get_something(top_level,XmNcolormap,(void*) &colormap);
   get_pixel(names,apix,num_colors);

/* recupera il nome del modello */
   leggi_nome_modello();

/* lettura dei macroblocchi dal file */
   leggi_macroblocchi( &macroblocks, &num_macro, &macro_allocate );

/* Creazione della pagina_indice */
   crea_pagina_indice();

/* Crea i popup menu dei macroblocchi e dei blocchi */
   popup_macro();

/* Crea la finestra per i messaggi all'utente latrante */
   warning_widget = attention(top_level,"",CREA,geom_attention);

#ifdef MAIN

   XtRealizeWidget (top_level);

#endif

/* Crea i graphics context */
   crea_gc();

/* creazione delle pixmap dei moduli */
   crea_pixmap_moduli();

/* creazione delle pixmap dei simboli */
   crea_pixmap_simboli();

#ifdef MAIN

   XtMainLoop();

#endif
}

/********  FINE MAIN()  ********/


/****************************/
/* Funzioni di CALLBACK ... */
/****************************/
/*------------------------------------------------------------------*/
/***
 *** void toggle_proc(w, client_data, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** caddr_t client_data: non utilizzato.
 *** XmAnyToggleCallbackStruct *call_data : non utilizzato. 
Gestisce il toggle snap on/off */
void toggle_proc (w, toggle_type, call_data)
Widget w;
int toggle_type;
XmAnyCallbackStruct *call_data;
{
   switch (toggle_type)
   {
       case K_SNAP_TOGGLE:
           if (snap_status)
           {
              cstring = CREATE_CSTRING("Snap off");
              snap_val = 1;
           }
           else
           {
              cstring = CREATE_CSTRING("Snap on");
              snap_val = PIXEL_STEP;
           }
           set_something_val(w, XmNlabelString, (XtArgVal) cstring);
           XmStringFree(cstring);
           snap_status = !snap_status;
           break;

       case K_ARROW_TOGGLE:
           if (disegna_freccia)
              cstring = CREATE_CSTRING("No");
           else
              cstring = CREATE_CSTRING("Yes");
           disegna_freccia = !disegna_freccia;
           set_something_val(toggle_arrow, XmNlabelString, (XtArgVal) cstring);
           XmStringFree(cstring);
           break;
   }
}

/*------------------------------------------------------------------*/
/*** void chiudi_window_blocchi((w, ind_macro, call_data)
 ***  Parametri:
 ***    Widget w: widget.
 ***    int ind_macro: indice in macroblocks[].
 ***    XmAnyCallbackStruct *call_data: dati.
Chiude e distrugge la window dei blocchi */
void chiudi_window_blocchi(w, ind_macro, call_data)
Widget w;
int ind_macro;
XmAnyCallbackStruct *call_data;
{

#ifdef TOPOLOGIA
/*----- 5-4-95 Micheletti -----*/
int i;
   
/* deseleziono tutti i blocchi */
   for(i=0;i<macroblocks[ind_macro].num_blocchi;i++)
   {
       seleziona_blocco(&(macroblocks[ind_macro]),&(macroblocks[ind_macro].blocks[i]),False);
       macroblocks[ind_macro].blocks[i].wblock = NULL;
   }
/*-----------------------------*/
#endif
#ifdef DATI
/*----- 26-4-95 Micheletti -----*/
extern int seleziona_blocco_lista();
int i;
   
/* deseleziono tutti i blocchi */
   for(i=0;i<macroblocks[ind_macro].num_blocchi;i++)
   {
       seleziona_blocco(&(macroblocks[ind_macro]),&(macroblocks[ind_macro].blocks[i]),False);
       seleziona_blocco_lista(&(macroblocks[ind_macro]),&(macroblocks[ind_macro].blocks[i]),False);
       macroblocks[ind_macro].blocks[i].wblock = NULL;
   }
#endif

   if (macroblocks[ind_macro].wwinblock != NULL)
   {
      XtDestroyWidget (macroblocks[ind_macro].wpopup);
      XtDestroyWidget (macroblocks[ind_macro].wwinblock);
   }
   macroblocks[ind_macro].wwinblock = NULL;
   num_win_blocchi--;
}

/*********************************
****  menu_dati_callb
**** funzione di callback utilizzata per l'attivizione dei bottoni di
**** menu tipici della attivita' DATI
**********************************/
#ifdef DATI
#include "dati.h"
void menu_dati_callb(w, scelta, call_data)
Widget w;
int scelta;
XmAnyCallbackStruct *call_data;
{
extern Widget popup_vardata();
extern int n_editors;
extern Widget popup_NormParamDialog();
int param_vardata;
switch(scelta)
	{
	case K_VAR_BLOCK:
	param_vardata= ALL_VAR;
	printf("\n parametro lancio = %d",param_vardata);
	popup_vardata(param_vardata);
	n_editors++;
	break;

	case K_DATA_BLOCK:
	param_vardata=ALL_DATA;
	printf("\n parametro lancio = %d",param_vardata);
	popup_vardata(param_vardata);
	n_editors++;
	break;

	case K_KNOW_BLOCK:
	printf("\n FUNZIONE NON IMPLEMENTATA");
	break;

	case K_NORM_BLOCK:
	popup_NormParamDialog();
	break;
	}
}
#endif

/*------------------------------------------------------------------*/
/*** void menu_macro(w, scelta, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int scelta : voce di menu selezionata.
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
attiva la dialog-box di inserimento di un macroblocco ***/
void menu_macro(w, scelta, call_data)
Widget w;
int scelta;
XmAnyCallbackStruct *call_data;
{
extern int seleziona_blocco_lista();
   int i, j,k;
   int *indici;  /* indici in macroblocks[] relativi ai macroblocchi */
                 /* selezionati */
   char str[10];

   switch (scelta)
   {
/********************************************************* menu_macro() ***/
       case K_SAVE:
/* Carlo
            def_cursore (top_level, OROLOGIO);
*/
	    modifiche = False;
            salva_macroblocchi(macroblocks, &num_macro);
#ifdef TOPOLOGIA
            write_f01();
#endif
/* Carlo
            undef_cursore (top_level);
*/
            break;

/********************************************************* menu_macro() ***/
       case K_QUIT:
/* 2-2-95 Micheletti */
#ifndef TOPOLOGIA
            if (modifiche)
            {
               if (dialog_uscita == NULL)
                  dialog_uscita = conferma(top_level,
                           "Do you want save changes ?",
                           conferma_uscita);
               XtManageChild(dialog_uscita);
	    }
            else
               quit_session(NULL, 1, NULL);
#endif

            break;

/********************************************************* menu_macro() ***/
       case K_NEW_MACRO:
            if (dbx_new_macro == NULL)
               dialog_box_gen(&dbx_new_macro, "CREATE NEW PAGE",
			      &text_nome_new, "NAME:", 14,
			      &text_descr_new,"DESCRIPTION:", 50,
                              DBOX_NEW_MACRO );
            XmTextSetString( text_nome_new , "" );
            XmTextSetString( text_descr_new, "" );
            XtManageChild(dbx_new_macro);
            break;

/********************************************************* menu_macro() ***/
       case K_MODIFY_MACRO:
            indici = cerca_macro_selezionati();
            macro_selez_tmp = indici[0];
	    free(indici);

            if (dbx_modify_macro == NULL)
               dialog_box_gen(&dbx_modify_macro, "MODIFY PAGE NAME",
                              &text_nome_mod, "NAME:", 14,
		              &text_descr_mod,"DESCRIPTION:", 50,
			      DBOX_MODIFY_MACRO);
            XmTextSetString( text_nome_mod , 
			     macroblocks[macro_selez_tmp].nome_macro );
            XmTextSetString( text_descr_mod, 
			     macroblocks[macro_selez_tmp].descr_macro );
            XtManageChild(dbx_modify_macro);
            break;

/********************************************************* menu_macro() ***/
       case K_DELETE_MACRO:
            modifiche = True;
            conferma_delete_macro();
            break;

/********************************************************* menu_macro() ***/
       case K_OPEN_MACRO:
        /* Visualizzazione della finestra dei blocchi. */
/* Lorenzo
            def_cursore (top_level, OROLOGIO);
*/

            indici = cerca_macro_selezionati();


            for ( i = 0 ; i < num_macro_selez ; i++  )
            {
                j = indici[i];


         /* se la window dei blocchi e' gia' attiva allora passa al */
         /* macroblocco successivo */
                if ( macroblocks[j].wwinblock != NULL )
                   continue;


         /* Attiva la window dei blocchi e carica le icone */
                crea_finestra_blocchi(j);


                display_icone_macro(j);
#ifndef MAIN


                display_icone_selezionate(j);
#endif


                cambia_edit_blk_menu(&macroblocks[j]);
                num_win_blocchi++;
            }


         /* visualizza le window */
            for ( i = 0 ; i < num_macro_selez ; i++  )
               XtManageChild(macroblocks[indici[i]].wwinblock);
/* Lorenzo
            undef_cursore (top_level);
*/

/*----- 5-4-95 Micheletti ------*/
#ifdef TOPOLOGIA
            for ( i = 0 ; i < num_macro_selez ; i++  )
            {
                j = indici[i];
/* deseleziono tutti i blocchi (anche nella lista) */
                printf("nel blocco ci sono %d blocchi\n",macroblocks[j].num_blocchi);
/*
                for ( k=0 ; k<macroblocks[j].num_blocchi ; k++ )
                {
                   printf("deselziono il blocco %d\n",k);
                   seleziona_blocco(&(macroblocks[j]), &(macroblocks[j].blocks[k]), True);
                }
*/
                for ( k=0 ; k<macroblocks[j].num_blocchi ; k++ )
                {
                   printf("deselziono il blocco %d\n",k);
                   seleziona_blocco(&(macroblocks[j]), &(macroblocks[j].blocks[k]), False);
                }
            }
#endif
#ifdef DATI
/* 21-4-95 Micheletti */
            for ( i = 0 ; i < num_macro_selez ; i++  )
            {
                j = indici[i];
                for ( k=0 ; k<macroblocks[j].num_blocchi ; k++ )
                {
                   printf("deselziono il blocco %d\n",k);
                   seleziona_blocco(&(macroblocks[j]), &(macroblocks[j].blocks[k]), False);
                   seleziona_blocco_lista(&(macroblocks[j]),&(macroblocks[j].blocks[k]),False);
                }
	    }
#endif

            free(indici);
            break;

/********************************************************* menu_macro() ***/
       case K_LOAD_ICONS:
/* Carlo
            def_cursore (top_level, OROLOGIO);
*/
            dealloca_pixmap();

         /* creazione delle pixmap dei moduli */
            crea_pixmap_moduli();

         /* creazione delle pixmap dei simboli */
            crea_pixmap_simboli();

         /* aggiorna la dialog-box per l'inserimento di un nuovo blocco */
            if (dbx_nuovo_blocco != NULL)
            {
               XmStringTable items;
               int num_items;
               char *nome_modulo;

               get_something( lista_moduli, XmNselectedItemCount, (void*) &num_items);
               if (num_items)
               {
                  get_something( lista_moduli, XmNselectedItems, (void*) &items);
                  nome_modulo=extract_string(items[0]);
                  sprintf(nome_modulo,"%.4s",nome_modulo);
                  carica_icone_modulo(nome_modulo, FROM_ADD_BLOCK);
               }
            }

         /* aggiorna la dialog-box per la modifica di un blocco */
            if (dbx_modify_block != NULL)
               carica_icone_modulo(ptr_blk_mod->nome_modulo, FROM_MODIFY_BLOCK);

         /* aggiorna la dialog-box per l'inserimento dei simboli  */
            if (dbx_simboli != NULL)
               carica_simboli();

         /* aggiorna le icone dei blocchi */
            for ( i = 0 ; i < num_macro ; i++  )
            {
               int j;
	       MacroBlockType *mblock;

               mblock = &macroblocks[i];

               if ( mblock->wwinblock == NULL )
                  continue;

               for (j=0 ; j<mblock->num_blocchi ; j++)
               {
                  PixmapInfo *pixm_info;

	       /* non considerare i blocchi cancellati */
                  if (mblock->blocks[j].cancellato)
		     continue;

                  pixm_info = recupera_pixmap(mblock->blocks[j].tipo,
					      mblock->blocks[j].num_icona,
					      mblock->blocks[j].nome_modulo);

               /* cambia la pixmap del blocco o simbolo che sia */
                  cambia_pixmap_icona(&mblock->blocks[j], pixm_info);
               }
            }
/* Carlo
            undef_cursore (top_level);
*/
            break;

/********************************************************* menu_macro() ***/
       case K_MODIFY_SNAP:
            if (dbx_modify_snap == NULL)
               dialog_modify_snap();
            XtManageChild(dbx_modify_snap);
            sprintf(str,"%2d", snap_val);
            XmTextSetString(text_snap_val, str);
            break;
   }
}

/*------------------------------------------------------------------*/
/*** void quit_session(w, devo_salvare, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int opzione : 0 -> uscire e salvare,
 ***		   1 -> uscire senza salvare,
 ***		   2 -> non uscire dal magollo.
 *** XmAnyCallbackStruct *call_data : non utilizzato. ***/
void quit_session(w, opzione, call_data)
Widget w;
int opzione;
XmAnyCallbackStruct *call_data;
{
   int i;

   if ( opzione == 2 )
   {
      XtUnmanageChild(dialog_uscita);
      return;
   }

   if ( opzione == 0 )
      salva_macroblocchi(macroblocks, &num_macro);

#ifndef MAIN

   for( i=0 ; i<num_macro ; i++ )
	chiudi_window_blocchi(NULL, i, NULL);

   dealloca_pixmap();
   XtDestroyWidget (dialog_shell);

#endif

   XFreeGC(display, xorGC);
   XFreeGC(display, iconGC);
   XFreeGC(display, lineGC);

#ifdef MAIN

   XtCloseDisplay (display);
   exit(0);

#endif

}

/*------------------------------------------------------------------*/
/*** conferma_delete_macro()
 ***   nessun parametro.
gestisce la dialog-box di conferma cancellazione macroblocco */
void conferma_delete_macro()
{
   int *indici, i, j;

   indici = cerca_macro_selezionati();
   for ( i = 0 ; i < num_macro_selez ; i++  )
   {
      j = indici[i];
      macroblocks[j].selezionato = False;
      if (macroblocks[j].wwinblock != NULL)
      {
         free(macroblocks[j].blocks); /* Dealloca la memoria */
	 macroblocks[j].blocks = NULL;
/* modifica 7-9-93 C.M.*/
         macroblocks[j].num_blocchi = 0;
         XtDestroyWidget(macroblocks[j].wwinblock); 
      }
      macroblocks[j].cancellato = True;
      XtDestroyWidget(macroblocks[j].wmacro); 
   }
   num_macro_selez = 0;
   free(indici);
 
/* Disabilita i push-button del popup-menu */
   cambia_edit_mcr_menu();
   macro_selez_tmp = -1;
}

/*------------------------------------------------------------------*/
/*** void new_block(w, tipo, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** short int tipo : tipo del blocco da istanziare (processo o regolazione)
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
attiva la dialog-box di inserimento di un macroblocco ***/
void new_block(w, tipo, call_data)
Widget w;
short int tipo;
XmAnyCallbackStruct *call_data;
{
   tipo_modulo = tipo;
   if (dbx_nuovo_blocco == NULL)
      dialog_nuovo_blocco();
   XmTextSetString( text_nome_blocco , "" );
   XmTextSetString( text_descr_blocco, "" );
   carica_lista_moduli(tipo);
   XtManageChild(dbx_nuovo_blocco);
   XmListSelectPos(lista_moduli,1,True);
}

/*------------------------------------------------------------------*/
/***
 *** void add_remark(w, ind_macro, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** caddr_t client_data : non utilizzato.
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
attiva la dialog-box di inserimento di un commento (LABEL) ***/
void add_remark(w, ind_macro, call_data)
Widget w;
int ind_macro;
XmAnyCallbackStruct *call_data;
{
    if (dbx_new_rem == NULL)
       dialog_box_gen(&dbx_new_rem, "ADD REMARK",
		      &text_new_rem, "REMARK:", 50, NULL, NULL, 0,
                      DBOX_ADD_REMARK );
    XmTextSetString( text_new_rem, "" );
    XtManageChild(dbx_new_rem);
    macro_selez_tmp = ind_macro; /* indice in macroblocks che viene */
	   		 /* recuperato in ok_proc (DBOX_ADD_REMARK) */
}

/*------------------------------------------------------------------*/
/***
 *** void add_symbol(w, ind_macro, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** caddr_t client_data : non utilizzato.
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
attiva la dialog-box di inserimento di un symbol ***/
void add_symbol(w, ind_macro, call_data)
Widget w;
int ind_macro;
XmAnyCallbackStruct *call_data;
{
    if (dbx_simboli == NULL)
       dialog_simboli();
    XtManageChild(dbx_simboli);
    carica_simboli();
}

#ifdef TOPOLOGIA
/*------------------------------------------------------------------*
 *** void variables_block(w,ind_macro,call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la chiamata
 *** int ind_macro : indice in macroblocks[]
 *** XmAnyCallbackStruct *call_data : non utilizzato
effettua la chiamata alla funzione di visualizzazione delle variabili
di un blocco definita in lg1 ***/ 

void variables_block(Widget w,int ind_macro, XmAnyCallbackStruct *call_data)
{
   bl_var_cb(); 
}


/*--------------------------------------------------------------------*
 *** void add_process_block(w,ind_macro,call_data)
 *** parametri
 *** Widget w : identificativo del widget che ha effettuato la chiamata
 *** int ind_macro : indice in macroblocks[]
 *** XmAnyCallbackStruct *call_data : non utilizzato
     effettua la chiamata alla funzione di inserimento nuovo blocco di
     processo ***/

void add_process_block(Widget w,int ind_macro,XmAnyCallbackStruct *call_data)
{
   tipo_modulo = TP_BLOCK;
   if (dbx_nuovo_blocco == NULL)
      dialog_nuovo_blocco();

   XmTextSetString( text_nome_blocco , "" );
   XmTextSetString( text_descr_blocco, "" );
   set_something_val( text_nome_blocco, XmNeditable, (XtArgVal) True);
   carica_lista_moduli(TP_BLOCK);
   XtManageChild(dbx_nuovo_blocco);
   XmListSelectPos(lista_moduli,1,True);
}

void add_regolation_block(Widget w,int ind_macro,XmAnyCallbackStruct *call_data)
{
   tipo_modulo = TP_BLOCK_REG;
   if (dbx_nuovo_blocco == NULL)
      dialog_nuovo_blocco();

   printf("test stamp add_regolation block\n");
   set_something_val( text_nome_blocco, XmNeditable, (XtArgVal) False);
   XmTextSetString( text_nome_blocco , "" );
   XmTextSetString( text_descr_blocco, "" );
   carica_lista_moduli(TP_BLOCK_REG);
   XtManageChild(dbx_nuovo_blocco);
   XmListSelectPos(lista_moduli,1,True);
/* istanzia senza icona - annullato
   block_selection_activate(REGOLAZIONE);
*/
}

#endif

/*------------------------------------------------------------------*/
/*** void modify_block(w, ind_macro, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int ind_macro : indice in macroblocks[]
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
attiva la dialog-box di modificas blocco (nome, descrizione e pixmap) ***/
void modify_block(w, ind_macro, call_data)
Widget w;
int ind_macro;
XmAnyCallbackStruct *call_data;
{
   int *indici;
   BlockType *blocks;
   char *strappo,descriz[100];

   blocks = macroblocks[ind_macro].blocks;
   indici = cerca_blocchi_selezionati(&macroblocks[ind_macro]);

   if (blocks[indici[0]].tipo == TP_SYMBOL)
   {
      free(indici);
      return;
   }

/* Setta il flag di modifica */
   set_something_val(macroblocks[ind_macro].wwinblock, XmNuserData, (XtArgVal) True);

   if (blocks[indici[0]].tipo == TP_BLOCK ||
       blocks[indici[0]].tipo == TP_BLOCK_REG)
   {
      tipo_modulo = blocks[indici[0]].tipo;
      pixm_ico_selez = NULL;
      wdg_ico_selez = NULL;
      if (dbx_modify_block == NULL)
         dialog_modify_block();

#ifdef TOPOLOGIA
      strappo = &blocks[indici[0]].nome_blocco[4];
      XmTextSetString( text_mod_nome_blk , strappo );
      Iget_descr_blocco(strappo,descriz);
      XmTextSetString( text_mod_descr_blk, descriz); 
#else
      XmTextSetString( text_mod_nome_blk , blocks[indici[0]].nome_blocco );
      XmTextSetString( text_mod_descr_blk, blocks[indici[0]].descr_blocco );
#endif

      XtManageChild(dbx_modify_block);

      carica_icone_modulo(blocks[indici[0]].nome_modulo, FROM_MODIFY_BLOCK);
      ptr_blk_mod = &blocks[indici[0]];
   }
   else
   {
      if (dbx_modify_rem == NULL)
         dialog_box_gen(&dbx_modify_rem, "MODIFY REMARK",
		        &text_modify_rem, "REMARK:", 50,
		        NULL, NULL, 0, DBOX_MODIFY_REMARK );
      XmTextSetString( text_modify_rem , blocks[indici[0]].descr_blocco );
      XtManageChild(dbx_modify_rem);
      ptr_rem_tmp = &blocks[indici[0]];
   }
   free(indici);
}



/*------------------------------------------------------------------*/
/***
 *** void delete_selected_blocks(w, ind_macro, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int ind_macro : indice in macroblocks[]
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
cancellazione di un blocco ***/
void delete_selected_blocks(w, ind_macro, call_data)
Widget w;
int ind_macro;
XmAnyCallbackStruct *call_data;
{
   MacroBlockType *ptr_macro;
   int *indici, i,j, num_blocchi;

   ptr_macro = &macroblocks[ind_macro];
   indici = cerca_blocchi_selezionati(&macroblocks[ind_macro]);
   num_blocchi = ptr_macro->num_blocchi_selez;

/*
#ifdef TOPOLOGIA
del_block_from_list();
#endif
*/

   for ( i = 0 ; i < num_blocchi ; i++ )
      cancella_blocco(ptr_macro, &ptr_macro->blocks[indici[i]]);

/* Setta il flag di modifica */
   modifiche = True;

/* disabilita i bottoni modify/delete del popup-menu */
   cambia_edit_blk_menu(ptr_macro);
   free(indici);

/*************
#ifdef TOPOLOGIA
del_block_from_list();
#endif
************/

}


/*------------------------------------------------------------------*/
/***
 *** void move_selected_blocks(w, ind_macro, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int ind_macro : indice in macroblocks[]
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
move di blocchi da una finestra all'altra ***/
void move_selected_blocks(w, ind_macro1, call_data)
Widget w;
int ind_macro1;
XmAnyCallbackStruct *call_data;
{
   Cursor cursore;
   MacroBlockType *ptr_macro1, *ptr_macro2;
   int ind_macro2;
   Widget wdg;
   XEvent event;
   BlockType *blocks;
   int *indici, i;
   XEvent last_event_motion;
   Boolean fine = False, flag = True;
   int offsetx, offsety, posx0, posy0;

   extern DimWidgets *dim_widgets;
   extern int num_dim_wdg;

   ptr_macro1 = &macroblocks[ind_macro1];

/* grab (cattura) del mouse pointer */
   if ( XGrabPointer(display, XtWindow(ptr_macro1->wtavblock), True, 
		     ButtonReleaseMask|PointerMotionMask,
		     GrabModeSync, GrabModeAsync, 
		     None, cursore_a_crocetta, CurrentTime) !=  GrabSuccess)
   {
      s_warning( top_level, &geom_attention, APPLICATION_NAME,
                 warning_mesg, WCANTGRAB );
      return;
   }

/* recupera le posizioni e dimensioni dei blocchi/label/simboli selezionati */
   indici = cerca_blocchi_selezionati(ptr_macro1);
   if(ptr_macro1->num_blocchi_selez == 0)
   {
      XUngrabPointer(display, CurrentTime);
      printf("Nothing is selected, select one block first\n");
      return;
   }       

   dim_widgets = (DimWidgets *) calloc_mem(ptr_macro1->num_blocchi_selez,
                                                   sizeof(DimWidgets));
   if(dim_widgets == NULL)
   {
      XUngrabPointer(display, CurrentTime);
      printf("move_selected_blocks -> calloc returned NULL pointer\n");
      return;
   }


   blocks = macroblocks[ind_macro1].blocks;
   posx0 = blocks[indici[0]].pos_icona.posx;
   posy0 = blocks[indici[0]].pos_icona.posy;
   for(i=0 ; i<ptr_macro1->num_blocchi_selez ; i++ )
   {
       int ind;

       ind = indici[i];
       dim_widgets[i].indice = ind;
       dim_widgets[i].x = blocks[ind].pos_icona.posx-posx0;
       dim_widgets[i].y = blocks[ind].pos_icona.posy-posy0;
       get_something( blocks[ind].wblock, XmNwidth, (void*) &dim_widgets[i].width );
       get_something( blocks[ind].wblock, XmNheight, (void*) &dim_widgets[i].height );
   }
   num_dim_wdg = ptr_macro1->num_blocchi_selez;

/* selezione della window destinzione */
   while (!fine)
   {
      XAllowEvents(display, SyncPointer, CurrentTime);
      XNextEvent(display, &event);
      switch (event.type)
      {
         case ButtonRelease:
              end_rubberband( &event, RootWindow(display, screen), 
		              dim_widgets, num_dim_wdg, True, 
			      BB_WIDTH, BB_HEIGHT );
	      fine = True;
              break;

         case MotionNotify:
              if (flag)
              {
                 flag = False;
		 for (i=0 ; i<num_dim_wdg ; i++)
		 {
                    offsetx = event.xbutton.x;
		    offsety = event.xbutton.y;
		    dim_widgets[i].x += offsetx;
		    dim_widgets[i].y += offsety;
		 }
                 start_rubberband( &event, RootWindow(display, screen), 
				   dim_widgets, num_dim_wdg, True );
              }
              else
              {
                 continue_rubberband( &event, RootWindow(display, screen),
     			             dim_widgets, num_dim_wdg, True );
              }
              break;

	 case Expose:
	      XtDispatchEvent(&event);
	      break;
      }
   }

   XUngrabPointer(display, CurrentTime);

/* recupero il widget della window in cui e' stato rilasciato il bottone */
   if ( (wdg = XtWindowToWidget(display, event.xbutton.window)) == NULL)
   {
      free(indici);
      return;
   }

   get_something( wdg, XmNuserData, (void*) &ptr_macro2 );

   if (ptr_macro2 != NULL && wdg == ptr_macro2->wtavblock && 
       ptr_macro2 != ptr_macro1 )         /* ok! il puntatore e' nella */
   {	  				  /* window corretta */
      extern int minposx, maxposx, minposy, maxposy;

   /* recupero  l'indice del macroblocco. L'operazione viene effettuata */
   /* sugli indirizzi di memoria */
      ind_macro2 = ((int)((long)ptr_macro2-(long)macroblocks))/
						   sizeof(MacroBlockType);

   /* Setta il flag di modifica */
      modifiche = True;

      if (minposx - offsetx + event.xbutton.x < 0)
	 event.xbutton.x = offsetx - minposx;
      else
         if (maxposx - offsetx + event.xbutton.x > BB_WIDTH)
            event.xbutton.x = BB_WIDTH - maxposx + offsetx - 2;

      if (minposy - offsety + event.xbutton.y < 0)
	 event.xbutton.y = offsety - minposy;
      else
         if (maxposy - offsety + event.xbutton.y > BB_HEIGHT )
            event.xbutton.y = BB_HEIGHT - maxposy + offsety - 2;

   /* cancella i blocchi dalla window di origine e crea i blocchi nella */
   /* nuova window */
      for (i=0 ; i<num_dim_wdg ; i++)
      {
	 int ind1, ind2, posx, posy;
	 PixmapInfo *pixmap_info;

         ind1 = indici[i];

         posx = blocks[ind1].pos_icona.posx-posx0+event.xbutton.x;
         posy = blocks[ind1].pos_icona.posy-posy0+event.xbutton.y;

      /* cancella il blocco */
	 cancella_blocco(ptr_macro1, &ptr_macro1->blocks[ind1]);

      /* crea il nuovo blocco */
	 pixmap_info = ptr_macro1->blocks[ind1].pixmap_info;
         ind2 = alloca_nuovo_blocco(ptr_macro2, posx, posy, 
			     ptr_macro1->blocks[ind1].tipo, True,
			     ptr_macro1->blocks[ind1].pixmap_info,
			     ptr_macro1->blocks[ind1].nome_blocco,
			     ptr_macro1->blocks[ind1].descr_blocco,
			     ptr_macro1->blocks[ind1].nome_modulo );
			     
         if (ptr_macro2->blocks[ind2].tipo == TP_REMARK)
            crea_remark( wdg, ptr_macro2->blocks[ind2].descr_blocco,
			 posx, posy, ind_macro2, ind2 );
         else
            crea_icona( wdg, pixmap_info, ptr_macro2->blocks[ind2].nome_blocco,
		        posx, posy, FROM_WINDOW_BLOCK, ind_macro2, ind2 );
	 seleziona_blocco(ptr_macro2, &ptr_macro2->blocks[ind2], True);
      }
   }
   else
      s_warning( top_level, &geom_attention, APPLICATION_NAME,
                 warning_mesg, WWINWRONG );
}

/*------------------------------------------------------------------*/
/***
 *** void cancel_proc(w, widget, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** Widget widget : widget da cancellare
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
 *** Chiude la finestra widget ***/
void cancel_proc(w, widget, call_data)
Widget w;
Widget widget;
XmAnyCallbackStruct *call_data;
{
   XtUnmanageChild( widget );
}

/*------------------------------------------------------------------*/
/*** void ok_proc(w, widget, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int which_dialog : riferimento della dialog-box
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
Gestisce i button 'Ok' delle dialog-box */
void ok_proc(w, which_dialog, call_data)
Widget w;
int which_dialog;
XmAnyCallbackStruct *call_data;
{
#ifdef TOPOLOGIA
   extern int changes_in_F01;
#endif
/***
   extern char *str_toupper();
***/
   char *nome_macro, *descr_macro;
   char *nome_blocco, *descr_blocco, *remark;

   switch (which_dialog) {

/********************************************************* ok_proc() ***/
       case DBOX_NEW_MACRO:
            nome_macro  = XmTextGetString( text_nome_new  );
            descr_macro = XmTextGetString( text_descr_new );
            sscanf(nome_macro, "%s", nome_macro);

        /* controllare che l'utente non abbia inserito una stringa vuota */
            if ( strcmp( nome_macro, "" ) )
            {
            /* Se il macroblocco non esiste gia', aggiorna il file e l'array */
               if ( cerca_macroblocco(macroblocks, num_macro, nome_macro) != 0 )
               {
               /* crea la label del macroblocco */
                  strcpy(macroblocks[ num_macro ].nome_macro, nome_macro);
                  strcpy(macroblocks[ num_macro ].descr_macro, descr_macro);
                  macroblocks[ num_macro ].pos_icona.posx = posx_macro;
                  macroblocks[ num_macro ].pos_icona.posy = posy_macro;
                  macroblocks[ num_macro ].cancellato = False;
                  macroblocks[ num_macro ].blocks = (BlockType *) 
					       calloc_mem(100,
                                                          sizeof(BlockType));
                  macroblocks[ num_macro ].num_blocchi_alloc = 100; 
                  macroblocks[ num_macro ].num_blocchi = 0; 
                  macroblocks[ num_macro ].line = (GLine *) calloc_mem(50,
							        sizeof(GLine));
                  macroblocks[ num_macro ].num_line_alloc = 50; 
                  macroblocks[ num_macro ].num_line = 0; 
/* Micheletti 2-2-95 */
                  macroblocks[ num_macro ].wwinblock = 0; 

                  disegna_macroblocco( num_macro );
                  num_macro++;

		  posx_macro += 10;
		  posy_macro += 10;
                  if (posx_macro  > BB_WIDTH || posy_macro  > BB_HEIGHT)
		  {
		     posx_macro = 0;
		     posy_macro = 0;
		  }
		
              /* Alloca altri 100 elementi se necessario */
                  if ( num_macro > macro_allocate )
                     macroblocks = (MacroBlockType *)
                                 realloc_mem( macroblocks,
					      macro_allocate += 100,
                                              sizeof(MacroBlockType));
		  modifiche = True; /* Setta il flag di modifica ! */
               }
               else
	          s_warning (top_level, &geom_attention, APPLICATION_NAME,
      			     warning_mesg, WMACROEXIST);
            }
#ifdef LINUX
            XtFree( nome_macro );
            XtFree( descr_macro);
#else
            XmStringFree( nome_macro );
            XmStringFree( descr_macro);
#endif
	    XtUnmanageChild (dbx_new_macro);
            break;

/********************************************************* ok_proc() ***/
       case DBOX_MODIFY_MACRO:
            nome_macro  = XmTextGetString( text_nome_mod  );
            descr_macro = XmTextGetString( text_descr_mod );
            sscanf(nome_macro, "%s", nome_macro);

        /* controllare che l'utente non abbia inserito una stringa vuota */
            if ( strcmp( nome_macro, "" ) )
            {
            /* Se il macroblocco non esiste gia', aggiorna il file e l'array */
               if ( !strcmp(macroblocks[macro_selez_tmp].nome_macro,nome_macro)
		    || cerca_macroblocco(macroblocks,num_macro,nome_macro) != 0)
               {
		  strcpy(macroblocks[macro_selez_tmp].nome_macro, nome_macro);
		  strcpy(macroblocks[macro_selez_tmp].descr_macro,
			 descr_macro );

               /* modifica la label del macroblocco */
		  set_label( macroblocks[macro_selez_tmp].wmacro, nome_macro );
		  modifiche = True; /* Setta il flag di modifica ! */
               }
               else
	          s_warning (top_level, &geom_attention, APPLICATION_NAME,
      			     warning_mesg, WMACROEXIST);
            }
            XtFree( nome_macro );
            XtFree( descr_macro);
	    XtUnmanageChild (dbx_modify_macro);
            break;

/********************************************************* ok_proc() ***/
       case DBOX_NEW_BLOCK:
            if (tavola_icone != NULL)
               XtDestroyWidget(tavola_icone);
	    tavola_icone=NULL;
            XtUnmanageChild(dbx_nuovo_blocco);
#ifdef TOPOLOGIA
            modifiche = True;
            changes_in_F01 = True;
#endif
            break;

/********************************************************* ok_proc() ***/
       case DBOX_ADD_REMARK:
            remark = XmTextGetString(text_new_rem);
            if (strcmp(remark, ""))
            {
	       int ind;

	       modifiche = True; /* Setta il flag di modifica ! */

               ind = alloca_nuovo_blocco(&macroblocks[macro_selez_tmp], 0, 0, 
			                 TP_REMARK, False, NULL,"",remark,"");

            /* Crea la label di commento */
	       crea_remark(macroblocks[macro_selez_tmp].wtavblock,
			   remark, 0, 0, macro_selez_tmp, ind );

            }
  	    XtFree(remark);
            XtUnmanageChild(dbx_new_rem);
            break;

/********************************************************* ok_proc() ***/
       case DBOX_MODIFY_REMARK:
            descr_blocco = XmTextGetString( text_modify_rem );

        /* controllare che l'utente non abbia inserito una stringa vuota */
            if ( strcmp( descr_blocco, "" ) )
            {
	       modifiche = True; /* Setta il flag di modifica ! */
               strcpy( ptr_rem_tmp->descr_blocco, descr_blocco );
	       set_label( ptr_rem_tmp->wblock, descr_blocco );
            }
            XtFree( descr_blocco );
            XtUnmanageChild(dbx_modify_rem);
            break;

/********************************************************* ok_proc() ***/
       case DBOX_MODIFY_BLOCK:
       {
         char *strappo;

            if (tavola_icone2 != NULL)
               XtDestroyWidget(tavola_icone2);
	    tavola_icone2=NULL;

            nome_blocco  = XmTextGetString( text_mod_nome_blk );
            descr_blocco = XmTextGetString( text_mod_descr_blk );

    /* controllare che l'utente non abbia inserito il nome di blocco nullo */

            printf("ok_proc nome_blocco -> [%s]\n",nome_blocco);
            trim_blank(nome_blocco);
            if ( strcmp( nome_blocco,"") )
            {
               if( strlen(nome_blocco) < 4)
                  strncat(nome_blocco,"    ", 4 - strlen(nome_blocco) );
               tomaius(nome_blocco);

/* 24-1-95 Micheletti */
#ifdef TOPOLOGIA

/* verifico se e' stato modificato anche il nome del blocco 
   se il nome del blocco e' stato modificato deve risultare 
   comunque univoco
*/

               strappo = &ptr_blk_mod->nome_blocco[4];
               if( strcmp( strappo, nome_blocco ))
               {
                  printf("risulta modificato il nome blocco\n");
/* controllo univocita del nome blocco */
                  if(!Iverifica_nome_blocco(nome_blocco)) /* non deve essere duplicato */
                  {
                     s_warning( top_level, &geom_attention, APPLICATION_NAME,warning_mesg, WDUPBLKNAME );
                     return;
                  }
               }
#endif
        
#ifdef TOPOLOGIA
               set_new_descr(nome_blocco,descr_blocco);
#else
               strcpy( ptr_blk_mod->descr_blocco, descr_blocco );
               strcpy( ptr_blk_mod->nome_blocco, nome_blocco );
               strappo = &ptr_blk_mod->nome_blocco[4];
               strcpy(strappo,nome_blocco);
               set_label(ptr_blk_mod->wlabel,ptr_blk_mod->nome_blocco);
#endif
            }
 

   /* se l'utente ha selezionato una icona differente cambia la pixmap */
            if (pixm_ico_selez != NULL &&
		pixm_ico_selez->num_icona != ptr_blk_mod->num_icona)
               cambia_pixmap_icona(ptr_blk_mod, pixm_ico_selez);

            XtFree( nome_blocco );
            XtFree( descr_blocco );
            XtUnmanageChild(dbx_modify_block);
	    modifiche = True; /* Setta il flag di modifica ! */
#ifdef TOPOLOGIA
/* 25-1-95 Micheletti */
            changes_in_F01 = True;
#endif
            }
            break;

/********************************************************* ok_proc() ***/
       case DBOX_SETUP_LINE:
       case DBOX_SETUP_LINE_APPLY:
            remark = XmTextGetString( text_line_thick );
            spessore_linea = atoi(remark);
	    XtFree(remark);
            if (spessore_linea <= 0)
            { 
               spessore_linea = 1;
               XmTextSetString(text_arrow_length, "1");
            } 

            remark = XmTextGetString( text_arrow_length );
            lung_freccia = atoi(remark);
	    XtFree(remark);
            if (lung_freccia <= 0)
            { 
               lung_freccia = 2;
               XmTextSetString(text_arrow_length, "2");
            } 

            remark = XmTextGetString( text_arrow_width );
            larg_freccia = atoi(remark);
            XtFree(remark);
            if (larg_freccia < spessore_linea/2)
            { 
               char strnum[10];

               larg_freccia = spessore_linea/2;
	       sprintf(strnum,"%-3d", larg_freccia);
               XmTextSetString(text_arrow_width, strnum);
               XtManageChild(dbx_setup_line);
            }

            if ( which_dialog == DBOX_SETUP_LINE )
               XtUnmanageChild(dbx_setup_line);
            break;

/********************************************************* ok_proc() ***/
       case DBOX_MODIFY_SNAP:
            remark = XmTextGetString( text_snap_val );
            if ( strcmp(remark, "") )
            {
               sscanf(remark,"%d",&snap_val);
	       if (snap_val <= 0)
		  snap_val = 1;
            }
            XtFree(remark);
            XtUnmanageChild( dbx_modify_snap );
            break;

/********************************************************* ok_proc() ***/
       default:
            break;
   }
}

/*------------------------------------------------------------------*/
/*** void scelta_colore(w, indice_colore, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int indice_colore : indice in apix[]
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
cambia il colore della linea ***/
void scelta_colore(w, indice_colore, call_data)
Widget w;
int indice_colore;
XmAnyCallbackStruct *call_data;
{
   colore_linea = indice_colore;
}

/*------------------------------------------------------------------*/
/*** selez_modulo(w, widget_num, list_info)
 *** parametri:
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *widget_num : indice in widget_array.
 ***   unsigned long *reason : non rilevante
 Procedure di callback utilizzata quando si seleziona un modulo.
 ***/
void selez_modulo(w, widget_num, list_info)
Widget w;
int *widget_num;
XmListCallbackStruct *list_info;
{
   char *item;

   item = extract_string( list_info->item );
   copy_n_car(nome_modulo_selez, item, 4);
   carica_icone_modulo(nome_modulo_selez, FROM_ADD_BLOCK);
/* se il modulo e' di regolazione il nome del blocco e' il nome del modulo */
   if ( tipo_modulo == TP_BLOCK_REG )
      XmTextSetString( text_nome_blocco , nome_modulo_selez );
   XtFree( item );
}

/**************** FUNZIONI SPECIFICHE DELL'APPLICATIVO *******************/

/*** void crea_pixmap_moduli()
 ***   Parametri:
 ***      nessuno.
La funzione crea le pixmap di tutti i moduli. Le informazioni relative alle
pixmap vengono memorizzate nell'array pixmap_module */
void crea_pixmap_moduli()
{
   FILE *fp_modp, *fp_modnp, *fp_std;
   IconFileRec rec_modp;
   SchemeFileRec rec_modnp;
   IconStdRec rec_std;
   char nome_modulo[5];
   PixmapInfo *pix_info;
   int i, j, k;

   fp_modp = apri_file_icoproc(CREA_FALSE, LIB_UTENTE);
   fp_std  = apri_file_std(CREA_FALSE, LIB_UTENTE);

/* conta il numero di moduli */
   for( num_moduli = 0 ; leggi_record_file_ico( fp_modp, &rec_modp ) == 0 ;
        num_moduli++ );

/* dimensiona l'array pixmap dei moduli */
   pixmap_module = (PixmapModuleStruct *) calloc_mem(num_moduli,
					      sizeof(PixmapModuleStruct));

   fseek(fp_modp, 0L, SEEK_SET);
   for( k=0 ; leggi_record_file_ico( fp_modp, &rec_modp ) == RETURN_OK ; k++ )
   {

      strcpy(pixmap_module[k].nome_modulo, rec_modp.nome_modulo);
      strcpy(pixmap_module[k].descr_modulo, rec_modp.descr_modulo);
      pixmap_module[k].num_icone = rec_modp.num_icone;

      if (rec_modp.num_icone)
      {
         for( i=0, j=0 ; j<rec_modp.num_icone && i<10 ; i++ )
            if (rec_modp.bmap_record[i].esiste)
            {
               pix_info = &pixmap_module[k].pixmap_info[j];

               pix_info->num_icona = i;
               pix_info->width = rec_modp.bmap_record[i].base;
               pix_info->height = rec_modp.bmap_record[i].altezza;
               pix_info->pixmap = crea_pixmap(&rec_modp.bmap_record[i]);
               j++;
            }
      }
      else
      {
         if (leggi_file_std_ico( fp_std, &rec_std, MODULE_PROCESS ) == 
								    RETURN_OK)
         {
            pix_info = &pixmap_module[k].pixmap_info[0];

            pix_info->num_icona = 0;
            pix_info->width = rec_std.bmap_record.base;
            pix_info->height = rec_std.bmap_record.altezza;
            pix_info->pixmap = crea_pixmap(&rec_std.bmap_record);
            pixmap_module[k].num_icone = 1;
         }
         else
         {
            pix_info->pixmap = NULL;
         }
      }
   }

/* icona standard moduli di regolazione (schemi) */
   if (leggi_file_std_ico( fp_std, &rec_std, MODULE_SCHEME ) != RETURN_ERROR)
   {
      pix_scheme_std.num_icona = 0;
      pix_scheme_std.width = rec_std.bmap_record.base;
      pix_scheme_std.height = rec_std.bmap_record.altezza;
      pix_scheme_std.pixmap = crea_pixmap(&rec_std.bmap_record);
   }
   else
      pix_scheme_std.pixmap = NULL;

/* lista dei moduli di regolazione */
   fp_modnp = apri_file_icoscheme(LIB_UTENTE);

/* conta il numero dei moduli */
   for ( num_mod_schema = 0;
         leggi_file_regola( fp_modnp, &rec_modnp ) == 0;
         num_mod_schema++ );

/* dimensiona l'array dei moduli di regolazione */
   pixmap_mod_schema = (PixmapModuleStruct *) calloc_mem(num_mod_schema,
                                              sizeof(PixmapModuleStruct));
   fseek(fp_modnp, 0L, SEEK_SET);
   for ( k=0 ; leggi_file_regola( fp_modnp, &rec_modnp ) == RETURN_OK ; k++ )
   {
       strcpy(pixmap_mod_schema[k].nome_modulo, rec_modnp.nome_modulo);
       strcpy(pixmap_mod_schema[k].descr_modulo, rec_modnp.descr_modulo);
       if ( pix_scheme_std.pixmap != NULL )
       {
          pixmap_mod_schema[k].num_icone = 1;
          memcpy( &(pixmap_mod_schema[k].pixmap_info[0]), &pix_scheme_std,
                  sizeof(PixmapInfo) );
       }
       else
          pixmap_mod_schema[k].num_icone = 0;
   }

   fclose(fp_modp);
   fclose(fp_modnp);
   fclose(fp_std);
}

/*** void crea_pixmap_simboli()
 ***   Parametri:
 ***      nessuno.
La funzione crea le pixmap dei simboli. Le informazioni relative alle
pixmap vengono memorizzate nell'array pixmap_symbol */
void crea_pixmap_simboli()
{
   int i, k;
   FILE *fp_symb;
   SymbolFileRec  rec_symb;
   PixmapInfo *pixm_info;

   pixmap_symbol.num_icone = 0;
   fp_symb = apri_file_symb( &rec_symb, CREA_FALSE, LIB_UTENTE);
   for( k=0, i=0 ; i<MAX_SYMBOL && k < rec_symb.num_simboli ; i++)
      if (rec_symb.bmap_record[i].esiste)
      {
         pixm_info = &pixmap_symbol.pixmap_info[k++];

         pixm_info->num_icona = i;
         pixm_info->width = rec_symb.bmap_record[i].base;
         pixm_info->height = rec_symb.bmap_record[i].altezza;
         pixm_info->pixmap = crea_pixmap(&rec_symb.bmap_record[i]);
         pixmap_symbol.num_icone++;
      }
   fclose(fp_symb);
}

/*** PixmapInfo *ricerca_icona(pixm_info, tot_icone, num_icona)
 ***   Parametri:
 ***     char *pixm_info: informazioni sulle pixmap.
 ***     int  tot_icone: numero icone.
 ***     int  num_icona: numero di icona da cercare.
ricerca le informazioni di una icona. Se trovata ritorna le informazioni */
/* della pixmap dell'icona altrimenti ritorna NULL */
PixmapInfo *ricerca_icona(pixm_info, tot_icone, num_icona)
PixmapInfo *pixm_info;
int tot_icone, num_icona;
{
   int i;

   if (pixm_info != NULL)
      for( i=0 ; i<tot_icone; i++ )
         if ( num_icona == pixm_info[i].num_icona )
	    return(&pixm_info[i]);
   return(NULL);
}

/*** PixmapModuleStruct *ricerca_modulo(nome_modulo, tipo)
 ***   Parametri:
 ***     char *nome_modulo: nome del modulo da cercare.
 ***     short int tipo: identifica se processo (TP_BLOCK) o regolazione (TP_BLOCK_REG)
ricerca il modulo nell'array pixmap_module[] e ritorna il puntatore
all'elemento ricercato. Se il nome del modulo non viene trovato ritorna NULL */
PixmapModuleStruct *ricerca_modulo(nome_modulo,tipo)
char *nome_modulo;
short int tipo;
{
   int i, num;
   PixmapModuleStruct *pixmod;

   if (nome_modulo == NULL)
      return(NULL);

   if (tipo == TP_BLOCK)
   {
      pixmod = pixmap_module;
      num = num_moduli;
   }
   else
   {
      pixmod = pixmap_mod_schema;
      num = num_mod_schema;
   }

   for( i=0 ; i<num; i++ )
      if ( !strcmp(nome_modulo, pixmod[i].nome_modulo) )
	 return(&pixmod[i]);
   return(NULL);
}

/*** PixmapInfo *recupera_pixmap(tipo, num_icona, nome_modulo)
 ***    short int tipo: tipo blocco (blocco, simbolo o commento)
 ***    short int num_icona: numero icona
 ***    char *nome_modulo : nome del modulo (valido solo nel caso in cui il
 ***			    tipo e' blocco
recupera la struttura PixmapInfo relativa ad un blocco o simbolo */
PixmapInfo *recupera_pixmap(tipo, num_icona, nome_modulo)
short int tipo, num_icona;
char *nome_modulo;
{
   PixmapInfo *pixm_info;
   PixmapModuleStruct *pixm_module;

   switch (tipo)
   {
       case TP_BLOCK:
       case TP_BLOCK_REG:
            pixm_module = ricerca_modulo( nome_modulo, tipo );
            if (pixm_module != NULL)
	        pixm_info = ricerca_icona(pixm_module->pixmap_info,
				                  pixm_module->num_icone,
                                                  num_icona);
	    else
		pixm_info = NULL;
            break;

       case TP_SYMBOL:
            pixm_info = ricerca_icona(pixmap_symbol.pixmap_info,
			              pixmap_symbol.num_icone,
			              num_icona); 
            break;

       default:
            return(NULL);
            break;
   }
   return(pixm_info);
}

/*** Pixmap crea_pixmap(record)
 ***   Parametri:
 ***     BitmapFileRec *record;
crea la pixmap dal file bitmap */
Pixmap crea_pixmap(record)
BitmapFileRec *record;
{
   char filebitmap[256], *path_bmap;
   unsigned int bmap_width, bmap_height;
   int bmap_x_hot, bmap_y_hot;
   Pixmap pixm, bmap_pixmap;

   if (record != NULL)
   {
/* percorso dei file bitmap */
#ifdef VMS
      sottodir_vms(filebitmap, Path_Legocad, PATH_LIBGRAF_UTE);
#else
      sprintf(filebitmap, "%s/%s/", Path_Legocad, PATH_LIBGRAF_UTE);
#endif
      tominus(record->nome_file); /* converti la stringa in caratteri minuscoli */
      strcat(filebitmap, record->nome_file);

/* Leggi il file bitmap. Se non si puo' leggere il file , visualizza una */
/* pixmap vuota. */
      if ( XReadBitmapFile(display, RootWindow(display,screen), filebitmap,
                        &bmap_width,
                        &bmap_height, &bmap_pixmap, &bmap_x_hot,
                        &bmap_y_hot) != BitmapSuccess )
      {
          s_warning( top_level, &geom_attention, APPLICATION_NAME,
                     warning_mesg, WREADBMAP );
          printf(">>> can't read file: %s\n", filebitmap );
          pixm = NULL;
      }
      else
      {  
          pixm = XCreatePixmap(display, RootWindow(display,screen), bmap_width,
                            bmap_height, DefaultDepth(display,screen));

          XSetForeground(display, iconGC, apix[record->colore_fg]);
          XSetBackground(display, iconGC, apix[ BLOCKS_TABLE_BG ]);

    /* trasformazione della pixmap di prof. 1 a pixmap di prof. della window */
    /* (solitamente 8) */
          XCopyPlane(display, bmap_pixmap, pixm, iconGC, 0, 0,
                     bmap_width, bmap_height, 0, 0, 1);
      }
   }
   else
      pixm = NULL;
   return(pixm);
}

/*** int *cerca_macro_selezionati()
 ***   Parametri:
 ***      nessuno.
La funzione cerca nell'array macroblocks[] tutti i macroblocchi che sono
stati selezionati dall'utente e resituisce un array degli indici relativi.
Tale array viene allocato dinamicamente e, quindi, ci si dovra' preoccupare
di deallocare la memoria quando tale array non viene piu' utilizzato.
ATTENZIONE!!! la funzioni si riferisce alle variabili globali:
macroblocks[], num_macro e num_macro_selez. Quindi assicurarsi che tali
variabili abbiano i valori appropriati prima di chiamare la funzione. */
int *cerca_macro_selezionati()
{
   int *ptr, i, j;

/* alloca la memoria necessaria */
   ptr = (int *) calloc_mem( num_macro_selez, sizeof(int) );

   for( i=0, j=0 ; i<num_macro && j < num_macro_selez ; i++ )
      if (macroblocks[i].selezionato)
         ptr[j++] = i;
   return(ptr);
}

/*** int *cerca_blocchi_selezionati(ptr_macro)
 ***   Parametri:
 ***    MacroBlockType *ptr_macro : puntatore alla struttura macroblocchi.
La funzione cerca nell'array macroblocks[ind_macro].blocks tutti i blocchi 
che sono stati selezionati dall'utente e resituisce un array degli indici
relativi.
Tale array viene allocato dinamicamente e, quindi, ci si dovra' preoccupare
di deallocare la memoria quando tale array non viene piu' utilizzato. */
int *cerca_blocchi_selezionati(ptr_macro)
MacroBlockType *ptr_macro;
{
   int *ptr, i, j;
   int num_blk, num_blk_selez;
   BlockType *blocks;

   num_blk = ptr_macro->num_blocchi;
   num_blk_selez = ptr_macro->num_blocchi_selez;
   blocks = ptr_macro->blocks;

/* alloca la memoria necessaria */
   ptr = (int *) calloc_mem( num_blk_selez, sizeof(int) );

   for( i=0, j=0 ; i<num_blk && j < num_blk_selez ; i++ )
      if (blocks[i].selezionato)
         ptr[j++] = i;
   return(ptr);
}

/*** cambia_edit_mcr_menu()
attiva/disattiva le voci del menu edit della window dei macroblocchi.
ATTENZIONE !!! La funzione si riferisce alla variabile globale
num_macro_selez */
void cambia_edit_mcr_menu()
{
#ifdef DATI
   set_something_val(pop_edit_macro[K_OPEN_MACRO], XmNsensitive, (XtArgVal) (num_macro_selez > 0 ) ? True : False );
   set_something_val(menu_edit_macro[K_OPEN_MACRO], XmNsensitive, (XtArgVal) (num_macro_selez > 0 ) ? True : False );
#else
   set_something_val(menu_edit_macro[K_MODIFY_MACRO], XmNsensitive, (XtArgVal) (num_macro_selez == 1) ? True : False );
   set_something_val(menu_edit_macro[K_OPEN_MACRO], XmNsensitive, (XtArgVal) (num_macro_selez > 0 ) ? True : False );
   set_something_val(menu_edit_macro[K_DELETE_MACRO], XmNsensitive, (XtArgVal) (num_macro_selez > 0 ) ? True : False );
   set_something_val(pop_edit_macro[K_MODIFY_MACRO], XmNsensitive, (XtArgVal) (num_macro_selez == 1) ? True : False );
   set_something_val(pop_edit_macro[K_OPEN_MACRO], XmNsensitive, (XtArgVal) (num_macro_selez > 0 ) ? True : False );
   set_something_val(pop_edit_macro[K_DELETE_MACRO], XmNsensitive, (XtArgVal) (num_macro_selez > 0 ) ? True : False );
#endif
}

/*** void dealloca_pixmap()
 ***   Parametri:
 ***        None.
dealloca le pixmap delle icone allocate precedentemente. */
void dealloca_pixmap()
{
   int i, j;

/* Dealloca le pixmap delle icone dei moduli */
/*
   for( i=0 ; i<num_moduli ; i++ )
      for( j=0 ; j<pixmap_module[i].num_icone ; j++ )
          XFreePixmap(display, pixmap_module[i].pixmap_info[j].pixmap);
*/

/* Dealloca le pixmap delle icone dei simboli */
/*
   for( i=0 ; i<pixmap_symbol.num_icone ; i++ )
       XFreePixmap(display, pixmap_symbol.pixmap_info[i].pixmap);
*/
   free(pixmap_module);
}

/*-----------------------------------------------------------------------*/
/*** void leggi_nome_modelli()
 ***  nessun parametro.
 *** recupera il nome del modello dal file f01.dat */
void leggi_nome_modello()
{
   FILE *fp;
   int i = 0;
   char buffer[80];

   if ((fp=fopen(FILE_F01,"r")) == NULL)
     printf ("errore: non trovo il file %S \n",FILE_F01); 
/**
      s_warning( top_level, &geom_attention, APPLICATION_NAME,
                 warning_mesg, WOPENF01 );
**/
   else
   {
      while (i < 2 && fgets(buffer,80,fp) != NULL)
         if (!strncmp(buffer,"****",4))
            i++;
      if ( i == 2 )
      {
         fgets(buffer,80,fp);
         buffer[strlen(buffer)-1] = '\0';  /* toglie il '\n' alla fine */
         strncpy(NomeModello, buffer,10);
      }
      else
      {
         strcpy(NomeModello,"noname");
      }
      fclose(fp);
   }
}

/************************ FUNZIONI DI UTILITA' ***************************/

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
/*** set_label( w, string )
 *** Parametri:
 ***    Widget w:  label widget o qualsiasi altro widget che contiene
 ***               la risorsa XmNlabelString.
 *** char *string: stringa di caratteri.
funzione che assegna una stringa di caratteri alla risorsa XmNlabelString
del widget */
void set_label( w, string )
Widget w;
String string;
{
   XmString compound_str;

   compound_str = CREATE_CSTRING(string);
   set_something_val(w, XmNlabelString, (XtArgVal) compound_str);
   XmStringFree(compound_str);
}

/*-----------------------------------------------------------------------*/
/*** void add_translations( w, t )
 ***   Parametri:
 ***     Widget w : widget 
 ***     String t : stringa contenente la translations.
Aggiorna la translation-table di un widget. ***/
void add_translations(w,t)
Widget w;
String  t;
{
   XtTranslations tr;
   char *policy;

   if (w && t)
   {
       tr = XtParseTranslationTable(t);
       policy = t;
       if (*policy == '#')
       {
           policy++;
           switch( *policy )
           {
               case 'r':
                         set_something_val(w, XtNtranslations, (XtArgVal) tr);
                         break;
               case 'a':
                         XtAugmentTranslations(w, tr);
                         break;
               case 'o':
               default:
                         XtOverrideTranslations(w, tr);
                         break;
           }
       }
       else
           XtOverrideTranslations(w, tr);
   }
}

/* fine macro.c */
