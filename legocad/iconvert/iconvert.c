/**********************************************************************
*
*       C Source:               iconvert.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 30 12:05:29 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: iconvert.c,2 %  (%full_filespec: 1,csrc,iconvert.c,2 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)iconvert.c	1.3\t3/28/95";
/*
        Fine sezione per SCCS
*/

/*  ICONVERT.C	utilita' fatta in casa come le tagliatelle
		per l'elaborazione grafica con MACRO & EDICOLE
		di modelli Lego gia' esistenti;
		legge il file F01.DAT e ne cava quel che serve
		Se e' gia' stato creato il file MACROBLOCKS.DAT
		effettua il confronto con il file F01.DAT

    Versione 0.1          13.01.93           Buffo & Calleri
    Modificato il  -----> 11.11.93                                  */



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>


#ifdef VMS

#include <unixio.h>
#define F_OK	0
#define W_OK	2
#define R_OK	4

#else

#include <unistd.h>

#endif

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/Text.h>
#include <Xm/MainW.h>
#include <Xm/SeparatoG.h>
#include <Xm/DialogS.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/BulletinB.h>
#include <Xm/ToggleB.h>
#include <Xm/List.h>
 
#include <file_icone.h>
#include <libutilx.h>

#include "iconvert.h"
#include "files.h"

/* messaggi di warning */
char *warning_mesg[] = {
                      "No macroblock selected: assignment impossible." ,
		      "Can't accept a blank macroblock name." ,
		      "Can't accept an existing macroblock name." ,
		      "Can't retrieve the selected macroblock." ,
                      "No block(s) selected.",
                      "Sorry: 'default' is a reserved word for a macroblock.",
		      "Can't find macroblocks" };

/* messaggi di errore */
char *error_mesg[] = { "Can't open file macroblocks.dat. Can't continue!" };

/*******************************************/
/* DEFINIZIONE  DELLE  VARIABILI  GLOBALI  */
/*******************************************/

/* VARIABILI UTILIZZATE NELLE FUNZIONI Xlib */
Display	    *display;

/* VARIABILI DI WIDGET */
Widget top_level, main_window;
Widget form_widget; 
Widget block_list, macro_list, rel_block_list;
Widget block_add_list, block_cancel_list;
Widget macro_text, macro_descr_text;
Widget label_nome_modello;
Widget warning_widget;
Widget w_find;
/* FIND */
Find_struct find_block;

/********************************************************************/
/* Strutture riguardanti il geometry management delle varie Dialog...
   Specificare, nell'ordine: { DefaultPosition TRUE/FALSE,XmNx,XmNy,
                               XmNwidth,XmNHeight }                 */
Dialog_geometry geom_attention   = { TRUE, NULL, NULL, NULL, 120};
/********************************************************************/

/* ALTRE VARIABILI GLOBALI */
int row_count = 0; /* conteggia il numero di item nella scroll-list */
XmString lista_blocchi[500];
int num_lista_blocchi;
int num_nuovi_blocchi;

MacroBlockType macroblocks[100];
int num_macro = 0;

char nome_modello[80];
char nome_macro[15];

PosXY pos_macro, pos_block;

typedef struct {
		  char     nome[9];
		  char     descr[51];
		  XmString cstring;
		  int      aggiungi;
		} BlockF01;

BlockF01 blocchi_f01[500];

/* FILE POINTERS */
FILE *fp_f01;

/* compound string vuota! */
XmString stringa_nulla, cstring;

Arg args[20];
Cardinal nargs;

/********  INIZIO MAIN()  ********/
main (int argc, char **argv)
{
  int i;
  char item_macro[80];

/* Variabile stringa NULLA di comodo ... */
  stringa_nulla = CREATE_CSTRING("");

/* Inizializzazione del Toolkit */
  top_level = XtInitialize(argv[0], "Iconvert", NULL,  0, &argc, argv);

  display = XtDisplay(top_level);

/* Definizione titolo e nome icona */
  nargs=0;
  XtSetArg(args[nargs],XmNtitle,WINDOW_TITLE); nargs++;
  XtSetArg(args[nargs],XmNiconName,ICON_NAME); nargs++;
/***
  XtSetArg(args[nargs],XmNminHeight,650); nargs++;
  XtSetArg(args[nargs],XmNminWidth, 700); nargs++;
 ***/
  XtSetValues (top_level,args,nargs);

/* Lettura delle informazioni dal file F01.DAT */
  leggi_file_f01();

/* Controllo sull'esistenza di MACROBLOCKS.DAT */
/* Se gia' esiste, viene effettuato un controllo sul file F01.DAT  */
/* Gli eventuali NUOVI blocchi vengono scaraventati nel macroblocco */
/* 'default' visualizzando una finestra con 2 liste e 1 messaggio */

  if (access(FILE_MACROBLOCKS, F_OK) == 0)
  {
     crea_distinta_blocchi();
     update_macro();
  }
  else
/* VISUALIZZA L'APPLICATIVO NELLA FORMA CONSUETA */
  {
  /* Creazione della main window */
    crea_main_window();

  /* Aggiornamento lista dei blocchi */
    aggiorna_lista_blocchi();

  /* Creazione del file macroblocks.dat */
    leggi_macroblocchi( macroblocks, &num_macro );

    pos_macro.posy = 0;
    pos_macro.posx = 0;

  /* Crea la finestra per i messaggi all'utente latrante */
    warning_widget = attention(top_level,"",CREA,geom_attention);
  }

  XtManageChild (main_window);

  XtRealizeWidget (top_level);

/* Disabilita la selezione all'interno delle opportune liste */
  if (rel_block_list != NULL)
     XSelectInput(display, XtWindow(rel_block_list), ExposureMask);

  if (block_add_list != NULL)
     XSelectInput(display, XtWindow(block_add_list), ExposureMask);

  if (block_cancel_list != NULL)
     XSelectInput(display, XtWindow(block_cancel_list), ExposureMask);

  XtMainLoop();
}
/********  FINE MAIN()  ********/

/****************************/
/* Funzioni di CALLBACK ... */
/****************************/

/*------------------------------------------------------------------*/
/*
 *** void f01_checked(w, client_data, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** caddr_t client_data : non utilizzato.
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
 *** callback di terminazione dell'applicativo 
 ***/
f01_checked(w, client_data, call_data)
Widget w;
caddr_t client_data; 
XmAnyCallbackStruct *call_data;
{
   assign_default(block_add_list,num_nuovi_blocchi);
   fine_trasmissioni();
}

/*------------------------------------------------------------------*/
/*
 *** void quit_iconvert(w, client_data, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** caddr_t client_data : non utilizzato.
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
 *** callback di terminazione dell'applicativo 
 ***/
quit_iconvert(w, client_data, call_data)
Widget w;
caddr_t client_data; 
XmAnyCallbackStruct *call_data;
{
  /* Se sono rimasti blocchi nella lista, vengono assegnati al
     macroblocco di default */
  assign_default(block_list,num_lista_blocchi);

  fine_trasmissioni();
}

/*------------------------------------------------------------------*/
/***
 *** void macro_selected(w, client_data, list_info)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** caddr_t client_data : non utilizzato.
 *** XmListCallbackStruct *list_info : informazioni sulla widget-list
 *** callback di refresh della block_list
 ***/
macro_selected(w, client_data, list_info)
Widget w;
caddr_t client_data; 
XmListCallbackStruct *list_info;
{
   char *nome, stringa[100];
   int ind_macro, i, block_height, block_width;
   int maxy = 0, height_tmp = 0;
   XmString cs_blocks[1000];
   BlockType *blk;

   nome = extract_string( list_info->item );
   sprintf(nome, "%.14s", nome);
   trim_blank(nome);
   if ((ind_macro = cerca_macroblocco( macroblocks, num_macro, nome )) == -1)
   {
      s_warning(top_level, &geom_attention, APPLICATION_NAME,
                warning_mesg, WSEARCHMACRO );
      return;
   }

   blk = macroblocks[ind_macro].blocks;

  pos_block.posy = 0;
/* visualizza i blocchi  nella widget-list */
   for( i=0 ; i<macroblocks[ind_macro].num_blocchi ; i++ )
      if (blk[i].tipo == TP_BLOCK)
      {
         sprintf(stringa, "%s - %s", blk[i].nome_blocco, blk[i].descr_blocco );
         cs_blocks[i] = CREATE_CSTRING(stringa);
/*         XmListAddItemUnselected(rel_block_list, cstring, 0); */
         recupera_dim_icona( blk[i].nome_modulo, blk[i].num_icona,
			     &block_width, &block_height, LIB_UTENTE );
         if ( maxy < blk[i].pos_icona.posy + block_height )
         {
	    pos_block.posy = blk[i].pos_icona.posy;
            height_tmp = block_height;
         }
      }

   pos_block.posx = PIXEL_STEP;
   pos_block.posy = pos_block.posy + height_tmp + 25;
   pos_block.posy = pos_block.posy + pos_block.posy % PIXEL_STEP;

   XtSetArg(args[nargs], XmNitems, cs_blocks ); nargs++;
   XtSetArg(args[nargs], XmNitemCount, macroblocks[ind_macro].num_blocchi);
   nargs++;
   XtSetValues (rel_block_list, args, nargs);
   strcpy(nome_macro, nome);
   XtFree(nome);
   for(i=0 ; i<macroblocks[ind_macro].num_blocchi ; i++)
      XmStringFree(cs_blocks[i]);
}

/*------------------------------------------------------------------*/
/***
 *** void assign_block(w, client_data, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** caddr_t client_data : non utilizzato.
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
 *** Assegna i blocchi selezionati al macroblocco corrente.
 ***/
assign_block(w, client_data, call_data)
Widget w;
caddr_t client_data; 
XmAnyCallbackStruct *call_data;
{
   char *nome_blocco1, nome[100], *nome_blocco2;
   int i, num_items, *pos_list, pos_count, num_macro, max_height = 0;
   int j, block_height, block_width;
   XmStringTable cstrings, macro_selez;
   BlockType blocks_tmp[1000];

   /* Leggo il nome/i del blocco/i selezionato/i  e il loro numero */
   get_something (block_list, XmNselectedItemCount, (void*) &num_items);
   get_something (block_list,XmNselectedItems,(void*) &cstrings);
 
   if (num_items == 0 || num_lista_blocchi == 0)
   {
      s_warning(top_level, &geom_attention, APPLICATION_NAME,
                warning_mesg, WNOBLKSELECT );
      return; /* stampare WARNING */
   }

   /* Verifico qual'e' il macroblocco selezionato */
   get_something (macro_list,XmNselectedItemCount,(void*) &num_macro);

   if (!num_macro)
   {
       s_warning(top_level, &geom_attention, APPLICATION_NAME,
                 warning_mesg, WNOMCRSELECT );
       return;
   }

   def_cursore (top_level, OROLOGIO);

   for(i=0; i<num_items; i++)
   {
      nome_blocco1 = extract_string(cstrings[i]);
      sprintf(blocks_tmp[i].nome_blocco, "%.4s%.4s",
                                         nome_blocco1, nome_blocco1+18);
      sprintf(blocks_tmp[i].descr_blocco, "%.50s",nome_blocco1+26);
      sprintf(blocks_tmp[i].nome_modulo, "%.4s",nome_blocco1);
      blocks_tmp[i].tipo = TP_BLOCK;
      blocks_tmp[i].flag = 0;
      blocks_tmp[i].num_icona = 
		 prima_icona_disponibile( blocks_tmp[i].nome_modulo,
					  &block_width, &block_height,
                                          LIB_UTENTE );
      max_height = (max_height < block_height) ? block_height : max_height;
      if ( (pos_block.posx + block_width + 2*PIXEL_STEP) >= BB_WIDTH)
      {
         pos_block.posy = pos_block.posy + max_height + 2*PIXEL_STEP;
         pos_block.posy = pos_block.posy + pos_block.posy % PIXEL_STEP;
         pos_block.posx = PIXEL_STEP;
         max_height = 0;
      }
      blocks_tmp[i].pos_icona.posx = pos_block.posx;
      blocks_tmp[i].pos_icona.posy = pos_block.posy;
      pos_block.posx = pos_block.posx + block_width + 2*PIXEL_STEP;
      pos_block.posx = pos_block.posx + pos_block.posx % PIXEL_STEP;
      sprintf(nome, "%.4s%.4s - %.50s", nome_blocco1, nome_blocco1+18,
				        nome_blocco1+26 );

      for( j = 0 ; j < num_lista_blocchi ; j++ )
      {
         nome_blocco2 = extract_string(lista_blocchi[j]);
         if ( !strcmp(nome_blocco1, nome_blocco2) )
         {
            num_lista_blocchi--;
            XmStringFree (lista_blocchi[j]);
            for(; j < num_lista_blocchi ; j++)
               lista_blocchi[j] = XmStringCopy(lista_blocchi[j+1]);
            XmStringFree (lista_blocchi[j+1]);
            break;
         }
      }
   /* Lo assegno al macroblocco corrente, dopo aver verificato qual e' */
      cstring = CREATE_CSTRING(nome);
      XmListAddItemUnselected(rel_block_list, cstring, 0); 
      XmStringFree(cstring);
   }
 
   add_blocks_macro( nome_macro, blocks_tmp, num_items );

   nargs = 0;
   XtSetArg(args[nargs], XmNitemCount, num_lista_blocchi); nargs++;
   XtSetArg(args[nargs], XmNitems, lista_blocchi ); nargs++;
   XtSetValues (block_list, args, nargs);

   XmListDeselectAllItems(block_list);
   XSync(display, False);
   find_block.pos = 0;   /* setta la posizione al primo elemento della lista */

   undef_cursore (top_level);
}

/*------------------------------------------------------------------*/
/*
 *** void new_macroblock(w, client_data, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** caddr_t client_data : non utilizzato.
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
 *** Prende dal Text Widget nome e descrizione del nuovo macroblocco.
 *** Il nome viene inserito nella lista dei macroblocchi.
 ***/
new_macroblock(w, client_data, call_data)
Widget w;
caddr_t client_data; 
XmAnyCallbackStruct *call_data;
{
   char *nome_macro, *descr_macro;
   char item_macro[80];
   XmString new_macro;

/* Leggo il nome del macroblocco, controllando che non sia blank */
/* e che non esista gia' e che non sia 'default'... */
   nome_macro = XmTextGetString (macro_text);
   trim_blank(nome_macro);

   if (Empty(nome_macro))
   {
       s_warning(top_level, &geom_attention, APPLICATION_NAME,
                 warning_mesg, WMCRNAMEBLANK );
       return;
   }

   if (strcmp(nome_macro,MACRO_DEFAULT) == 0)
   {
       s_warning(top_level, &geom_attention, APPLICATION_NAME,
                 warning_mesg, WMCRNAMERES );
       return;
   }

   if (cerca_macroblocco( macroblocks, num_macro, nome_macro ) >= 0)
   {
       s_warning(top_level, &geom_attention, APPLICATION_NAME,
                 warning_mesg, WMCRNAMEEXIST );
       return;
   }

   /* Leggo la descrizione del macroblocco */
   descr_macro = XmTextGetString (macro_descr_text);

   sprintf (item_macro,"%-14s  %s", nome_macro, descr_macro);

   /* Inserimento del nuovo macroblocco nella lista */
   new_macro = CREATE_CSTRING (item_macro);

   XmListAddItemUnselected (macro_list,new_macro,0);
   XmListSetBottomPos (macro_list,0);
   XmStringFree(new_macro);
   strcpy(macroblocks[num_macro].nome_macro, nome_macro);
   strcpy(macroblocks[num_macro].descr_macro, descr_macro);
   macroblocks[num_macro].pos_icona.posx = pos_macro.posx;
   macroblocks[num_macro].pos_icona.posy = pos_macro.posy;
/*   aggiungi_macroblocco( &macroblocks[num_macro] ); */
   num_macro++;

   pos_macro.posx = pos_macro.posx + MACROBLOCKS_WIDTH + 2*PIXEL_STEP;
   pos_macro.posx = pos_macro.posx + pos_macro.posx % PIXEL_STEP;
   if ( (pos_macro.posx + MACROBLOCKS_WIDTH + 2*PIXEL_STEP) >= BB_WIDTH)
   {
      pos_macro.posy = pos_macro.posy + MACROBLOCKS_HEIGHT + 2*PIXEL_STEP;
      pos_macro.posy = pos_macro.posy + pos_macro.posy % PIXEL_STEP;
      pos_macro.posx = PIXEL_STEP;
   }
}

/*------------------------------------------------------------------*/
/***
 *** void restart_conv(w, client_data, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** caddr_t client_data : non utilizzato.
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
 *** Ricomincia da zero l'attivita' di assegnazione, riempiendo la lista
 *** dei blocchi e cancellando il file macroblocks.dat.
 ***/
restart_conv(w, client_data, call_data)
Widget w;
caddr_t client_data; 
XmAnyCallbackStruct *call_data;
{
   int i;

/* Orologizza il cursorino */
   def_cursore (top_level, OROLOGIO);

/* Dealloca la memoria utilizzata */
   for (i=0 ; i<num_macro ; i++)
   {
      free(macroblocks[i].blocks);
      macroblocks[i].num_blocchi = 0;
      macroblocks[i].blocks = NULL;
   }
   num_macro = 0;

   XmListDeselectAllItems (macro_list);

/* Rileggi/ricrea il file macroblocks.dat */
   leggi_macroblocchi( macroblocks, &num_macro );

   /* Cancella i contenuti delle liste dei macroblocchi & blocchi associati */
   nargs = 0;
   XtSetArg(args[nargs], XmNitemCount, 0); nargs++;
   XtSetArg(args[nargs], XmNitems, NULL ); nargs++;
   XtSetValues (rel_block_list, args, nargs);

   nargs = 0;
   XtSetArg(args[nargs], XmNitemCount, 0); nargs++;
   XtSetArg(args[nargs], XmNitems, NULL ); nargs++;
   XtSetValues (macro_list, args, nargs);

   /* Cancella i contenuti dei Text dei macroblocchi & loro descrizione */
   set_something_val (macro_text,XmNvalue, (XtArgVal) "");
   set_something_val (macro_descr_text,XmNvalue, (XtArgVal) "");

   /* Legge da capo il contenuto del file f01.dat e lo schiaffa */
   /* nella lista dei blocchi */
   leggi_file_f01();
   aggiorna_lista_blocchi();

   /* Deseleziona i blocchi eventualmente selezionati */
   XmListDeselectAllItems (block_list);

   /* Omogeneizza il cursorino */
   undef_cursore (top_level);

   XSync(display,False);
}

/*------------------------------------------------------------------*/
/***
 *** void global_selection(w, client_data, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int parameter :  utilizzato.per lo switch
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
 *** Seleziona o de-seleziona tutti gli item presenti nella lista dei blocchi
 ***/
global_selection(w, parameter, call_data)
Widget w;
int parameter; 
XmAnyCallbackStruct *call_data;
{
   int i, num_items;

   switch (parameter) {

     case True: /* SELEZIONARE TUTTO */
          get_something (block_list,XmNitemCount,(void*) &num_items);
          for (i=0; i<num_items; i++)
	      XmListSelectPos (block_list,i,True);
     break;

     case False: /* DESELEZIONARE TUTTO */
          XmListDeselectAllItems (block_list);
     break;
   }
}

/*********************************/
/* ALTRE FUNZIONI ...		 */
/*********************************/

/*********************************************************************/
/***
 *** assign_default (block_to_add,num_lista_blocchi)
 *** PARAMETRI:
 *** Widget block_to_add: lista di blocchi da schiaffare in 'default'
 *** int num_lista_blocchi: numero di tali blocchi
 ***
 *** Chiamata se, quando l'utente preme 'Quit', sono rimasti dei blocchi
 *** non assegnati ad alcun macroblocco.
 *** Schiaffa tali blocchi in un macroblocco di nome 'default'.
 ***/
assign_default(block_to_add,num_lista_blocchi)
Widget block_to_add;
int num_lista_blocchi;
{
   int i, *pos_list, pos_count, max_height = 0, num_blk;
   XmStringTable cstrings;
   char *nome_blocco, nome[100];
   int block_height, block_width;
   BlockType *blk, blocks_tmp[1000];

   if (!num_lista_blocchi)
      return;

/* Aggiungo il macroblocco 'default' alla lista */
/* Solo se non esiste gia' */
   for (i=0; i<num_macro; i++)
     if ( !strcmp(macroblocks[i].nome_macro, MACRO_DEFAULT) )
        break;

   if (i >= num_macro)
   {
      strcpy(macroblocks[i].nome_macro, MACRO_DEFAULT);
      macroblocks[i].pos_icona.posx = pos_macro.posx;
      macroblocks[i].pos_icona.posy = pos_macro.posy;
      macroblocks[i].num_blocchi = 0;
      num_macro++;

     /* Posiziona il macroblocco nella pagina indice del modello */
      pos_macro.posx = pos_macro.posx + MACROBLOCKS_WIDTH + 2*PIXEL_STEP;
      pos_macro.posx = pos_macro.posx + pos_macro.posx % PIXEL_STEP;
      if ( (pos_macro.posx + MACROBLOCKS_WIDTH + 2*PIXEL_STEP) >= BB_WIDTH)
      {
         pos_macro.posy = pos_macro.posy + MACROBLOCKS_HEIGHT + 2*PIXEL_STEP;
         pos_macro.posy = pos_macro.posy + pos_macro.posy % PIXEL_STEP;
         pos_macro.posx = PIXEL_STEP;
      }
   }

/* Azzero i valori di posx e posy per i blocchi del macroblocco di default */
   pos_block.posx = PIXEL_STEP;
   num_blk = macroblocks[i].num_blocchi;
   blk = macroblocks[i].blocks;

   if (num_blk)
   {
      recupera_dim_icona( blk[num_blk-1].nome_modulo,blk[num_blk-1].num_icona,
			  &block_width, &block_height, LIB_UTENTE );

      pos_block.posy = blk[num_blk-1].pos_icona.posy + block_height +
		       block_height % PIXEL_STEP;
   }
   else
     pos_block.posy = 2*PIXEL_STEP; 

/* Leggo il nome/i del blocco/i residuo/i */
   get_something (block_to_add,XmNitems,(void*) &cstrings);
   for(i=0; i < num_lista_blocchi; i++)
   {
      nome_blocco = extract_string(cstrings[i]);

      sprintf(blocks_tmp[i].nome_blocco, "%.4s%.4s",nome_blocco,
                                                    nome_blocco+18);
      sprintf(blocks_tmp[i].nome_modulo, "%.4s",nome_blocco);

      blocks_tmp[i].tipo = TP_BLOCK;
      blocks_tmp[i].flag = 0;
      blocks_tmp[i].num_icona = 
                         prima_icona_disponibile( blocks_tmp[i].nome_modulo,
					          &block_width, &block_height,
                                                  LIB_UTENTE );
      blocks_tmp[i].cancellato = False;

   /* Posizionamento delle icone nella finestra macro */
      max_height = (max_height < block_height) ? block_height : max_height;
      if ( (pos_block.posx + block_width + 2*PIXEL_STEP) >= BB_WIDTH)
      {
         pos_block.posy = pos_block.posy + max_height + 25;
         pos_block.posy = pos_block.posy + pos_block.posy % PIXEL_STEP;
         pos_block.posx = PIXEL_STEP;
         max_height = 0;
      }
      blocks_tmp[i].pos_icona.posx = pos_block.posx;
      blocks_tmp[i].pos_icona.posy = pos_block.posy;
      pos_block.posx = pos_block.posx + block_width + 2*PIXEL_STEP;
      pos_block.posx = pos_block.posx + pos_block.posx % PIXEL_STEP;

      sprintf(nome, "%.4s%.4s - %.50s", nome_blocco, nome_blocco + 18,
                                        nome_blocco + 26 );
   }

   add_blocks_macro( MACRO_DEFAULT, blocks_tmp, num_lista_blocchi );
}

/*********************************************************************/
/***
 *** leggi_file_f01()
 *** Fa quello che dice.
 ***/
leggi_file_f01()
{
   char buffer[100], temp[5];
   int i = 0;

   if ( (fp_f01 = fopen ("f01.dat","r")) == NULL)
   {
	printf ("Error opening file f01.dat\n");
        return;
   }

   strcpy (temp,"****");

/* Salto la prima riga del F01.DAT, composta da '****' */
   fgets(buffer,100,fp_f01);

   i = 0;
   while (fgets(buffer,100,fp_f01) != NULL &&  strncmp(temp,buffer,4))
   {
      buffer[strlen(buffer)-1] = '\0';

   /* Riempio l'array dei nomi dei blocchi per i successivi controlli */
      sprintf (blocchi_f01[i].nome,"%.4s%.4s",buffer,buffer+18);
      sprintf (blocchi_f01[i].descr,"%.50s",buffer+26);
   /* Setto a True il flag 'aggiungi' (default) */
      blocchi_f01[i].aggiungi = True;

      blocchi_f01[i].cstring = CREATE_CSTRING(buffer);
      lista_blocchi[i] = blocchi_f01[i].cstring;
      i++;
   }
   num_lista_blocchi = i;

   fgets(buffer,100,fp_f01);
   strcpy(nome_modello, buffer);
   nome_modello[strlen(nome_modello)-1] = '\0'; /* Tolgo il \n alla fine */

   fclose (fp_f01);
}

/*-----------------------------------------------------------------------*/
/*** aggiorna_lista_blocchi()
 ***/
aggiorna_lista_blocchi()
{
   nargs=0;
   XtSetArg(args[nargs], XmNitemCount, num_lista_blocchi); nargs++;
   XtSetArg(args[nargs], XmNitems, lista_blocchi ); nargs++;
   XtSetValues (block_list, args, nargs);

   cstring = CREATE_CSTRING(nome_modello);
   set_something_val (label_nome_modello,XmNlabelString, (XtArgVal) cstring);
   XmStringFree(cstring);
}

/*-----------------------------------------------------------------------*/
/*** void crea_find(widget_sup)
 ***   Parametri:
 ***     Widget widget_sup : widget sopra il find (da appiccicare ...)
 *** Crea i FIND di libreria per la scroll list dei blocchi. 
 *** La funzione ritorna il widget della form del blocco di find. 
 ***/
Widget crea_find(widget_sup)
Widget widget_sup;
{
   static Widget find_form;

   find_block.ID_lista = block_list;
   find_block.num_elem = 500;
   find_block.vis_elem = VISIBLE_ITEMS;
   find_block.pos      = 0;   /* posizione corrente nel find */

   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,51); nargs++;
/***
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,widget_sup); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
 ***/
   if (find_form == NULL)
       find_form = find_kit(form_widget,args,nargs,&find_block);

   set_something_val (find_block.ID_testo,XmNbackground, (XtArgVal) 1);
   return(find_form);
}

/*------------------------------------------------------------------*/
/*
 *** void crea_main_window()
 *** creazione della main-window 
 ***/
crea_main_window()
{
   /* Push-buttons */
   Widget lb1, lb2, lb3, lb4, lb5, lb6;
   Widget sep1, sep2, sep3, sep4;
   Widget select, deselect;
   Widget ok, assign, quit, restart;

/* Creazione della main window vera e propria */
   nargs=0;
   XtSetArg(args[nargs],XmNmainWindowMarginHeight,10); nargs++;
   XtSetArg(args[nargs],XmNmainWindowMarginWidth, 10); nargs++;
   main_window = XmCreateMainWindow (top_level,"Main_window",args,nargs);

/* Creazione della work region (= form) */
   nargs=0;
   XtSetArg(args[nargs],XmNheight,650); nargs++;
   XtSetArg(args[nargs],XmNwidth, 700); nargs++;
   form_widget = XmCreateForm (main_window,"Form",args,nargs);

   cstring = CREATE_CSTRING("Assign blocks to proper macroblocks.   Residual blocks are assigned to a default macroblock.");
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   lb1 = XmCreateLabel (form_widget,"Label",args,nargs);
   XtManageChild (lb1);
   XmStringFree(cstring);

/* Separatore */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,lb1); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,5); nargs++;
   sep1 = XmCreateSeparatorGadget (form_widget,"",args,nargs);
   XtManageChild (sep1);


/* Creazione della scroll list  dei nomi dei blocchi*/
   cstring = CREATE_CSTRING ("BLOCKS OF THE MODEL");
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep1); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring ); nargs++;
   lb2 = XmCreateLabel (form_widget,"Label",args,nargs);
   XtManageChild (lb2);
   XmStringFree(cstring);

/* Creazione della label del nome del modello */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget,lb2); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,10); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep1); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
   label_nome_modello = XmCreateLabel (form_widget,"Nome_modello",args,nargs);
   XtManageChild (label_nome_modello);

   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,lb2); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,5); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,49); nargs++;
   XtSetArg(args[nargs],XmNtraversalOn,True); nargs++;
   XtSetArg(args[nargs],XmNlistSizePolicy,XmRESIZE_IF_POSSIBLE); nargs++;
   XtSetArg(args[nargs],XmNscrollBarDisplayPolicy,XmAS_NEEDED); nargs++;
   XtSetArg(args[nargs],XmNselectionPolicy,XmMULTIPLE_SELECT); nargs++;
   XtSetArg(args[nargs],XmNvisibleItemCount,VISIBLE_ITEMS); nargs++;
   block_list = XmCreateScrolledList (form_widget,"List",
       				         args,nargs);
   XtManageChild (block_list);

/* Inserimento find */
   w_find = crea_find(block_list);

/* Separatore */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,58); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,w_find); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
 **/
   sep2 = XmCreateSeparatorGadget (form_widget,"",args,nargs);
   XtManageChild (sep2);

/* Creazione del push button di Deselect_all */
   nargs=0;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,56); nargs++;
   deselect = XmCreatePushButton (form_widget,"Deselect all",args,nargs);
   XtManageChild (deselect);
   XtAddCallback (deselect, XmNactivateCallback, global_selection, False);

/* Creazione del push button di Select_all */
   nargs=0;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNrightWidget,deselect); nargs++;
   XtSetArg(args[nargs],XmNrightOffset,20); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,56); nargs++;
   select = XmCreatePushButton (form_widget,"Select all",args,nargs);
   XtManageChild (select);
   XtAddCallback (select, XmNactivateCallback, global_selection, True);

/* Creazione della lista dei macroblocchi */
   cstring = CREATE_CSTRING("MACROBLOCK");
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,60); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,62); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep2); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
 **/
   XtSetArg(args[nargs],XmNlabelString,cstring); nargs++;
   lb3 = XmCreateLabel (form_widget,"Label",args,nargs);
   XtManageChild (lb3);
   XmStringFree(cstring);

   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNrightPosition,35); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,63); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,80); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,lb3); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,5); nargs++;
 **/
   XtSetArg(args[nargs],XmNtraversalOn,True); nargs++;
   XtSetArg(args[nargs],XmNlistSizePolicy,XmRESIZE_IF_POSSIBLE); nargs++;
   XtSetArg(args[nargs],XmNscrollBarDisplayPolicy,XmAS_NEEDED); nargs++;
   XtSetArg(args[nargs],XmNselectionPolicy,XmSINGLE_SELECT); nargs++;
   XtSetArg(args[nargs],XmNvisibleItemCount,VISIBLE_ITEMS/2+1); nargs++;
   macro_list = XmCreateScrolledList (form_widget,"List",args,nargs);
   XtAddCallback (macro_list,XmNsingleSelectionCallback,macro_selected,NULL);
   XtManageChild (macro_list);

/* Creazione della lista dei blocchi del macroblocco selezionato */
   cstring = CREATE_CSTRING("RELATED BLOCKS");
   nargs=0;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,60); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,62); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep2); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
 **/
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNleftPosition,38); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   lb4 = XmCreateLabel (form_widget,"Label",args,nargs);
   XtManageChild (lb4);
   XmStringFree(cstring);

   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNleftPosition,38); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,63); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,80); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,lb4); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,5); nargs++;
 **/
   XtSetArg(args[nargs],XmNtraversalOn,True); nargs++;
   XtSetArg(args[nargs],XmNlistSizePolicy,XmRESIZE_IF_POSSIBLE); nargs++;
   XtSetArg(args[nargs],XmNscrollBarDisplayPolicy,XmAS_NEEDED); nargs++;
   XtSetArg(args[nargs],XmNselectionPolicy,XmSINGLE_SELECT); nargs++;
   XtSetArg(args[nargs],XmNvisibleItemCount,VISIBLE_ITEMS/2+1); nargs++;
   rel_block_list = XmCreateScrolledList (form_widget,"List",args,nargs);
   XtManageChild (rel_block_list);

/* Separatore */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,82); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,macro_list); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
 **/
   sep3 = XmCreateSeparatorGadget (form_widget,"",args,nargs);
   XtManageChild (sep3);

/* Creazione del text del nome del macroblocco */
   cstring = CREATE_CSTRING ("New macroblock:");
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,89); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep3); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,20); nargs++;
 **/
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   lb5 = XmCreateLabel (form_widget,"Label",args,nargs);
   XtManageChild (lb5);
   XmStringFree(cstring);

   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget,lb5); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,10); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,84); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,90); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep3); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,11); nargs++;
 **/
   XtSetArg(args[nargs],XmNeditMode, XmSINGLE_LINE_EDIT); nargs++;
   XtSetArg(args[nargs],XmNcolumns, 14); nargs++;
   XtSetArg(args[nargs],XmNmaxLength,14); nargs++;
   XtSetArg(args[nargs],XmNwidth, 150); nargs++;
   XtSetArg(args[nargs],XmNheight,40); nargs++;
   macro_text = XmCreateText (form_widget,"Text",args,nargs);
   XtManageChild (macro_text);

/* Creazione del push button di Ok */
   nargs=0;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,84); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,90); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep3); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
 **/
   XtSetArg(args[nargs],XmNheight,40); nargs++;
   ok = XmCreatePushButton (form_widget,"Ok",args,nargs);
   XtManageChild (ok);
   XtAddCallback (ok, XmNactivateCallback, new_macroblock, NULL);

/* Creazione del text della descrizione del macroblocco */
   cstring = CREATE_CSTRING ("Description:");
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget,macro_text); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,10); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,89); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep3); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,20); nargs++;
 **/
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   lb6 = XmCreateLabel (form_widget,"Label",args,nargs);
   XtManageChild (lb6);
   XmStringFree(cstring);

   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget,lb6); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,10); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNrightWidget,ok); nargs++;
   XtSetArg(args[nargs],XmNrightOffset,10); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,84); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,90); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep3); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,11); nargs++;
 **/
   XtSetArg(args[nargs],XmNeditMode, XmSINGLE_LINE_EDIT); nargs++;
   XtSetArg(args[nargs],XmNcolumns, 20); nargs++;
   XtSetArg(args[nargs],XmNmaxLength,50); nargs++;
   XtSetArg(args[nargs],XmNheight,40); nargs++;
   macro_descr_text = XmCreateText (form_widget,"Text",args,nargs);
   XtManageChild (macro_descr_text);

/* Separatore */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,92); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,macro_text); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
 **/
   sep4 = XmCreateSeparatorGadget (form_widget,"",args,nargs);
   XtManageChild (sep4);

/* Creazione del push button di Assign */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,95); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep4); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
 **/
   assign = XmCreatePushButton (form_widget,"Assign",args,nargs);
   XtManageChild (assign);
   XtAddCallback (assign, XmNactivateCallback, assign_block, NULL);

/* Creazione del push button di Restart */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget,assign); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,20); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,95); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep4); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
 **/
   restart = XmCreatePushButton (form_widget,"Restart",args,nargs);
   XtManageChild (restart);
   XtAddCallback (restart, XmNactivateCallback, restart_conv, NULL);

/* Creazione del push button di Quit */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget,restart); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,20); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNtopPosition,95); nargs++;
/**
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep4); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
 **/
   quit = XmCreatePushButton (form_widget,"Quit",args,nargs);
   XtManageChild (quit);
   XtAddCallback (quit, XmNactivateCallback, quit_iconvert, NULL);

/* Completamento della main window */
   XmMainWindowSetAreas(main_window,NULL,NULL,NULL,NULL,form_widget);
   XtManageChild (form_widget); 
}



/*------------------------------------------------------------------*/
/***
 *** fine_trasmissioni ()
 *** Augura la buona notte a tutti.
 ***/
fine_trasmissioni ()
{
   XtCloseDisplay (display);
   fprintf(stdout, "Saving in macroblocks.dat...\n");
   salva_macroblocchi( macroblocks, &num_macro );
   fprintf(stdout, "Done.\n");
   printf("Model %s is now ready for graphical processing.\n\n",nome_modello);
   exit(0);
}

/*------------------------------------------------------------------*/
/*** update_macro()
 *** Controlla se ci sono differenze nei blocchi contenuti in F01.DAT
 *** e MACROBLOCKS.DAT, dando la priorita' a F01.DAT.
 *** N.B.  NON VISUALIZZA L'APPLICATIVO MA SOLTANTO UN MESSAGGIO
 ***/
update_macro()
{
   int i, j, k;
   BlockType *blk;

/* Lettura del file macroblocks.dat */
   leggi_macroblocchi( macroblocks, &num_macro );

   for (i=0; i<num_macro; i++)
   {
    /* Confronto i nomi dei blocchi del macroblocco uno per uno */
    /* con la lista di tutti i blocchi in f01.dat 		*/
       for (j=0; j<macroblocks[i].num_blocchi; j++)
       {
           blk = macroblocks[i].blocks;

           if (blk[j].tipo != TP_BLOCK)
              continue;

           for (k=0; k<num_lista_blocchi; k++)
           {
              if ( !strcmp(blk[j].nome_blocco,blocchi_f01[k].nome) )
              {
                    blocchi_f01[k].aggiungi = False;
                    break;
              }
           }

        /* Se il blocco NON esiste in F01.DAT va cancellato */
           if (k >= num_lista_blocchi)
           {
               blk[j].cancellato = True;

            /* aggiorna la lista dei blocchi da cancellare (DESTRA) */
               cstring = CREATE_CSTRING(blk[j].nome_blocco);
               XmListAddItemUnselected(block_cancel_list, cstring, 0);
               XmStringFree(cstring);
           }
       }
   }

   /* Blocchi da aggiungere nel macroblocco 'default' in macroblocks.dat */
   num_nuovi_blocchi = 0;
   for (i=0; i<num_lista_blocchi; i++)
       if (blocchi_f01[i].aggiungi)
       {
         /* aggiorna la lista dei blocchi da aggiungere (SINISTRA) */
            XmListAddItemUnselected(block_add_list, blocchi_f01[i].cstring, 0);
            XmStringFree(blocchi_f01[i].cstring);
            num_nuovi_blocchi++;
       }
}

/*------------------------------------------------------------------*/
/*
 *** void crea_distinta_blocchi()
 *** Creazione della window dove vengono visualizzati i blocchi
 *** da aggiungere e/o cancellare dopo il confronto tra f01.dat
 *** e macroblocks.dat.
 ***/
crea_distinta_blocchi()
{
   /* Push-buttons */
   Widget lb1, lb2, lb3, lb4, lb5, lb6;
   Widget sep1, sep2, sep3, sep4;
   Widget ok;

/* Creazione della main window vera e propria */
   nargs=0;
   XtSetArg(args[nargs],XmNmainWindowMarginHeight,10); nargs++;
   XtSetArg(args[nargs],XmNmainWindowMarginWidth, 10); nargs++;
   main_window = XmCreateMainWindow (top_level,"Main_window",args,nargs);

/* Creazione della work region (= form) */
   nargs=0;
   XtSetArg(args[nargs],XmNheight,500); nargs++;
   XtSetArg(args[nargs],XmNwidth, 700); nargs++;
   form_widget = XmCreateForm (main_window,"Form",args,nargs);

   cstring = CREATE_CSTRING("REPORT FROM COMPARING FILES  f01.dat  AND  macroblocks.dat:");
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   lb1 = XmCreateLabel (form_widget,"Label",args,nargs);
   XtManageChild (lb1);
   XmStringFree(cstring);

/* Separatore */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,lb1); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,5); nargs++;
   sep1 = XmCreateSeparatorGadget (form_widget,"",args,nargs);
   XtManageChild (sep1);


/* Creazione della scroll list dei blocchi aggiunti */
   cstring = CREATE_CSTRING ("BLOCKS ADDED from f01.dat");
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep1); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring ); nargs++;
   lb2 = XmCreateLabel (form_widget,"Label",args,nargs);
   XtManageChild (lb2);
   XmStringFree(cstring);

   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,lb2); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,5); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,44); nargs++;
   XtSetArg(args[nargs],XmNtraversalOn,True); nargs++;
   XtSetArg(args[nargs],XmNlistSizePolicy,XmRESIZE_IF_POSSIBLE); nargs++;
   XtSetArg(args[nargs],XmNscrollBarDisplayPolicy,XmAS_NEEDED); nargs++;
   XtSetArg(args[nargs],XmNselectionPolicy,XmMULTIPLE_SELECT); nargs++;
   XtSetArg(args[nargs],XmNvisibleItemCount,VISIBLE_ITEMS); nargs++;
   block_add_list = XmCreateScrolledList (form_widget,"List",
       				          args,nargs);
   XtManageChild (block_add_list);


/* Separatore */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,46); nargs++;
   sep2 = XmCreateSeparatorGadget (form_widget,"",args,nargs);
   XtManageChild (sep2);

/* Creazione della lista dei blocchi da cancellare */
   cstring = CREATE_CSTRING ("BLOCKS ERASED in macroblocks.dat");
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,sep2); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   lb3 = XmCreateLabel (form_widget,"Label",args,nargs);
   XtManageChild (lb3);
   XmStringFree(cstring);

   nargs=0;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNrightPosition,51); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,lb3); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,5); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNbottomPosition,88); nargs++;
   XtSetArg(args[nargs],XmNtraversalOn,True); nargs++;
   XtSetArg(args[nargs],XmNlistSizePolicy,XmRESIZE_IF_POSSIBLE); nargs++;
   XtSetArg(args[nargs],XmNscrollBarDisplayPolicy,XmAS_NEEDED); nargs++;
   XtSetArg(args[nargs],XmNselectionPolicy,XmMULTIPLE_SELECT); nargs++;
   XtSetArg(args[nargs],XmNvisibleItemCount,VISIBLE_ITEMS); nargs++;
   block_cancel_list = XmCreateScrolledList (form_widget,"List",
       				             args,nargs);
   XtManageChild (block_cancel_list);

/* Creazione del push button di Ok */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNheight,40); nargs++;
   ok = XmCreatePushButton (form_widget," Ok ",args,nargs);
   XtManageChild (ok);
   XtAddCallback (ok, XmNactivateCallback, f01_checked, NULL);

/* Completamento della main window */
   XmMainWindowSetAreas(main_window,NULL,NULL,NULL,NULL,form_widget);
   XtManageChild (form_widget); 
}

/*** add_blocks_macro(nome_macro, blk, num_blk)
 ***   Parametri:
 ***     char *nome_macro: nome macroblocco.
 ***     Blocks blk : blocchi da aggiungere nel macroblocco.
 ***     int num_blk : numero di blocchi.
Aggiunge i blocchi nel macroblocco in memoria ***/
add_blocks_macro(nome_macro, blk, num_blk)
char *nome_macro;
BlockType blk[];
int num_blk;
{
   int i, ind;

   for ( i=0 ; i<num_macro ; i++ )
      if (strcmp(nome_macro, macroblocks[i].nome_macro) == 0)
         break;
   if ( i >= num_macro )
   {
      s_warning(top_level, &geom_attention, APPLICATION_NAME,
                warning_mesg, WMACRONOTFOUND );
      return(-1);
   }

   ind = macroblocks[i].num_blocchi;
   macroblocks[i].num_blocchi += num_blk;
   macroblocks[i].num_blocchi_alloc = macroblocks[i].num_blocchi;
   macroblocks[i].blocks = (BlockType *) 
                                realloc_mem( macroblocks[i].blocks,
                                             macroblocks[i].num_blocchi_alloc,
				             sizeof(BlockType));

   memcpy((char *) &macroblocks[i].blocks[ind], (char *) blk,
          num_blk * sizeof(BlockType));
   return(0);
}
    
/*** fine iconvert.c ***/
