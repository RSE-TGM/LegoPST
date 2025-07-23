/**********************************************************************
*
*       C Source:               macro_wdg.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 30 12:14:46 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: macro_wdg.c,2 %  (%full_filespec: 1,csrc,macro_wdg.c,2 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)macro_wdg.c	2.9\t4/11/95";
/*
        Fine sezione per SCCS
*/

/* MACRO_WDG.C 		file di servizio per MACRO
			Crea gli widget dell'applicativo
			relativi alla pagina indice (MACROBLOCCHI)
   Versione 0.1		5.11.92			Buffo & Calleri  */

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/cursorfont.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/Text.h>
#include <Xm/SeparatoG.h>
#include <Xm/DialogS.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/MainW.h>
#include <Xm/PushB.h>
#include <Xm/Frame.h>
#include <Xm/CascadeB.h>
#include <Xm/LabelG.h>
#include <Xm/BulletinB.h>
#include <Xm/ToggleB.h>
#include <Xm/MainW.h>

#include "macro.h"
#include "ico_mesg.h"

/* Variabili esterne dichiarate in MACRO.C ... */
extern Widget top_level, main_window;
extern Widget warning_widget;
extern Widget form_pagina_indice;
extern Widget menubar_pag_ind;
extern Widget scroll_index;
extern Widget macro_tavola; 
extern Widget popup_mcr, menu_edit_macro[7], pop_edit_macro[7];
extern Widget dbx_modify_snap, text_snap_val;

/* Strutture per la gestione degli widgets */
extern MacroBlockType *macroblocks;
extern int num_macro, num_macro_selez, macro_selez_tmp;

extern XmString    stringa_nulla, cstring;

extern Display *display;
extern int screen;
extern GC xorGC, iconGC, lineGC;

extern Pixel apix[];
extern Arg args[];
extern Cardinal nargs;
extern Boolean snap_status;
extern int snap_val;


/*------------------------------------------------------------------*/
/*
 *** void crea_pagina_indice()
Creazione della pagina indice. 
 ***/
void crea_pagina_indice()
{
   Widget frame_index, header_index, form_pagina_indice, scroll_index;
   Widget menubar_pag_ind;
   extern Widget dialog_shell;
   int i;
#ifdef MAIN
/* Creazione della pagina indice ( COME FORM) */
   nargs=0;
   XtSetArg(args[nargs], XmNwidth,500); nargs++;
   XtSetArg(args[nargs], XmNheight,400); nargs++;
   main_window = XmCreateMainWindow(top_level, "Pagina_indice", args,nargs);
/*   XtManageChild( main_window ); */

#else

/* Creazione della dialog shell */
   nargs=0;
   XtSetArg(args[nargs], XmNwidth,700); nargs++;
   XtSetArg(args[nargs], XmNheight,650); nargs++;
/*
   XtSetArg(args[nargs], XmNheight,600); nargs++;
*/
   XtSetArg(args[nargs],XmNminWidth,500); nargs++;
   XtSetArg(args[nargs],XmNminHeight,400); nargs++;
   XtSetArg(args[nargs],XmNtitle,MACRO_WINDOW_TITLE); nargs++;
   XtSetArg(args[nargs],XmNiconName,APPLICATION_NAME); nargs++;
   dialog_shell = XmCreateDialogShell(top_level, "Dialog_shell", args,nargs);

/* Creazione della pagina indice ( COME FORM) */
   nargs=0;
   XtSetArg(args[nargs], XmNwidth,700); nargs++;
   XtSetArg(args[nargs], XmNheight,600); nargs++;
   main_window = XmCreateMainWindow(dialog_shell, "Pagina_indice", args,nargs);
   XtManageChild( dialog_shell );
#endif

/* MENUBAR */
   nargs=0;
   XtSetArg(args[nargs],XmNheight,30); nargs++;
   XtSetArg(args[nargs],XmNspacing,60); nargs++;
   XtSetArg(args[nargs],XmNorientation,XmHORIZONTAL); nargs++;
   menubar_pag_ind = XmCreateMenuBar (main_window,"Pagina_indice",args,nargs);

   menu_pagina_indice(menubar_pag_ind);
   XtManageChild (menubar_pag_ind);

   form_pagina_indice = XmCreateForm(main_window, "Pagina_indice", NULL,0);

/* Creazione del frame che deve contenere il titolo della scroll window */
   nargs=0;
   XtSetArg(args[nargs],XmNheight, 30); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightOffset,5); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,5); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,15); nargs++;
   frame_index = XmCreateFrame (form_pagina_indice,"Pagina_indice",args,nargs);
   XtManageChild (frame_index);

/* Creazione della label "DIAGRAM PAGES' INDEX" */
   cstring=CREATE_CSTRING("DIAGRAM PAGES' INDEX");
   nargs=0;
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   header_index = XmCreateLabel (frame_index,"Pagina_indice",args,nargs);
   XtManageChild (header_index);
   XmStringFree(cstring);

/* Creazione della scrolled window delle variabili che deve contenere 
   righe composte da widget di tipo diversi (gestiti da un'ulteriore form) */
   nargs=0;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightOffset,5); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,5); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,frame_index); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,15); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,10); nargs++;
   XtSetArg(args[nargs],XmNscrollBarDisplayPolicy,XmAS_NEEDED); nargs++;
   XtSetArg(args[nargs],XmNscrollingPolicy,XmAUTOMATIC); nargs++;
   XtSetArg(args[nargs],XmNvisualPolicy,XmCONSTANT); nargs++;
   XtSetArg(args[nargs],XmNscrollBarPlacement,XmBOTTOM_RIGHT); nargs++;
   scroll_index = XmCreateScrolledWindow
		      (form_pagina_indice,"Scroll_index",args,nargs);
   XtManageChild (scroll_index);

/* La scrolled window contiene un Bulletin Board */
   nargs = 0;
   XtSetArg(args[nargs], XmNx, 0); nargs++;
   XtSetArg(args[nargs], XmNy, 0); nargs++;
   XtSetArg(args[nargs], XmNwidth, BB_WIDTH); nargs++;
   XtSetArg(args[nargs], XmNheight,BB_HEIGHT); nargs++;
   XtSetArg(args[nargs], XmNmarginWidth, 0); nargs++;
   XtSetArg(args[nargs], XmNmarginHeight,0); nargs++;
   XtSetArg(args[nargs], XmNnoResize,True); nargs++;
   XtSetArg(args[nargs], XmNresizePolicy, XmRESIZE_NONE); nargs++;
   macro_tavola = XmCreateBulletinBoard (scroll_index, "Scroll_index",
					 args,nargs);
   XtManageChild (macro_tavola);
   add_translations( macro_tavola, STR_DESELECT_ALL_MACRO );

/* Oggetti posizionabili dentro la scrolled window: LABELS */
   for (i=0 ; i < num_macro ; i++)
      disegna_macroblocco( i );
   XtManageChild (form_pagina_indice);
   XmMainWindowSetAreas( main_window, menubar_pag_ind, NULL, NULL, NULL,
			 form_pagina_indice );
   XtManageChild( main_window );
}

/*** disegna_macroblocco( indice )
 *** Parametri:
 ***    int indice : indice in macroblocks[]
crea la label corrispondente del macroblocco nella window pagina indice.
( MacroBlockType macroblocks[] e' globale ). */
disegna_macroblocco(indice)
int indice;
{
   cstring = CREATE_CSTRING( macroblocks[indice].nome_macro );

/* LABELS */
   nargs = 0;
   XtSetArg(args[nargs], XmNx, macroblocks[indice].pos_icona.posx ); nargs++;
   XtSetArg(args[nargs], XmNy, macroblocks[indice].pos_icona.posy ); nargs++;
   XtSetArg(args[nargs], XmNheight, MACROBLOCKS_HEIGHT); nargs++;
   XtSetArg(args[nargs], XmNwidth, MACROBLOCKS_WIDTH); nargs++;
   XtSetArg(args[nargs], XmNrecomputeSize, False); nargs++;
   XtSetArg(args[nargs], XmNlabelString, cstring); nargs++;
   XtSetArg(args[nargs], XmNalignment, XmALIGNMENT_CENTER); nargs++;
   XtSetArg(args[nargs], XmNborderColor, apix[ BLACK ]); nargs++;
   XtSetArg(args[nargs], XmNborderWidth,BORDER_WIDTH); nargs++;
   XtSetArg(args[nargs], XmNbackground, apix[WHITE]); nargs++;
   XtSetArg(args[nargs], XmNhighlightOnEnter,True); nargs++;
   XtSetArg(args[nargs], XmNhighlightThickness,0); nargs++;
   XtSetArg(args[nargs], XmNhighlightPixmap,XmUNSPECIFIED_PIXMAP);nargs++;
   macroblocks[indice].wmacro = XmCreateLabel( macro_tavola, "Macro_label",
				               args, nargs );

/* definisci le translations e gli event-handler per gestire la selezione */
/* e lo spostamento di un macroblocco */
   def_handler_macro(indice);

   XtManageChild (macroblocks[indice].wmacro);
   XmStringFree(cstring);
}

/*---------------------------------------------------------------------*/
/*** void def_handler_macro(indice)
 ***  Parametri:
 ***     indice : indice in macroblocks[];
definisce le translation per la selezione/deselezione del macroblocco e
gli event-handler utilizzati per spostare i macroblocchi */
void def_handler_macro(indice)
{
   char stringa[100];

/* gestione della selezione del macroblocco */
   sprintf(stringa,STR_SELECT_MACRO1,indice);
   add_translations( macroblocks[indice].wmacro, stringa );
   sprintf(stringa,STR_SELECT_MACRO2,indice);
   add_translations( macroblocks[indice].wmacro, stringa );

/* gestione dello spostamento dei macroblocchi */
   XtAddEventHandler( macroblocks[indice].wmacro, ButtonPressMask, False,
			 macro_bpress, indice );
#ifndef DATI
   XtAddEventHandler( macroblocks[indice].wmacro, ButtonMotionMask, False,
			 macro_bcont, NULL ); 
   XtAddEventHandler( macroblocks[indice].wmacro, ButtonReleaseMask, False,
			 macro_brelease, indice );
#endif
}

/**************************************************************/
/* variabili globali utilizzate nello spostamento delle label */
/* ed altre amenita' */
extern int mousex0, mousey0;
extern int mouseprecx, mouseprecy, minposx, maxposx, minposy, maxposy;
extern Dimension xoffs, yoffs;
extern int bottone_premuto;
/**************************************************************/

/*---------------------------------------------------------------------*/
/*** void deselect_all_macro(w, ev, params, num_params)
 ...
 ***/
void deselect_all_macro(w, ev, params, num_params)
Widget w;
XEvent *ev;
String *params;
Cardinal *num_params;
{
   int i;

   for ( i=0 ; i<num_macro ; i++ )
       if (macroblocks[i].selezionato && !macroblocks[i].cancellato)
       {
          macroblocks[i].selezionato = False;
          set_something_val(macroblocks[i].wmacro, XmNbackground, (XtArgVal) apix[WHITE]);
/*
 la condizione seguente e' stata aggiunta come patch da L.Castiglioni
*/
	if(num_macro_selez > 0)
          num_macro_selez--;
       }

/* aggiorna il sensitive delle voci di menu */
    cambia_edit_mcr_menu();
}

/*---------------------------------------------------------------------*/
/*** void select_macro(w, ev, params, num_params)
 ...
 ***/
void select_macro(w, ev, params, num_params)
Widget w;
XEvent *ev;
String *params;
Cardinal *num_params;
{
   int ind_macro, modo;

   if ( *num_params < 2 )
      return;

   sscanf(params[0], "%d", &modo);
   sscanf(params[1], "%d", &ind_macro);
   
   printf("indice macro: %2d modo: %1d\n", ind_macro, modo);
   if (modo == 1) /* macroblocco selezionato senza lo Shift ... */
      deselect_all_macro(NULL,NULL,NULL,NULL);

   macroblocks[ind_macro].selezionato = !macroblocks[ind_macro].selezionato;

/* incrementa o decrementa il contatore delle macro selezionate */
printf("\n NUMERO SELEZIONATI PRIMA %d",num_macro_selez);
   num_macro_selez = num_macro_selez + 
			((macroblocks[ ind_macro ].selezionato) ? 1 : -1);
printf("\n MACROBLOCCO SELEZIONATO = %d",macroblocks[ ind_macro ].selezionato);
   printf("m: %s %d - num. selez: %d\n",
	      macroblocks[ ind_macro ].nome_macro,
	      macroblocks[ ind_macro ].selezionato, num_macro_selez);
/* Attiva / Disattiva gli opportuni push-button dell'option menu */
/* in relazione ai macroblocchi selezionati dall'utente */
   cambia_edit_mcr_menu();

/* Seleziona il macroblocco corrente e resetta il precedente (COLOR_BG) */
   if ( macroblocks[ ind_macro ].selezionato ) 
      set_something_val(w, XmNbackground, (XtArgVal) apix[MACRO_SELEZ_BG]);
   else
      set_something_val(w, XmNbackground, (XtArgVal) apix[WHITE]);
}

/*------------------------------------------------------------------*/
/*** macro_bpress(w, ind_macro, ev, boh)
 *** Parametri:
 ...
 *** L'utente ha selezionato il macroblocco da spostare ***/
void macro_bpress(w, ind_macro, ev, boh)
Widget w;
int ind_macro;
XEvent *ev;
Boolean boh;
{
   int i, *indici;
   extern DimWidgets *dim_widgets;
   extern int num_dim_wdg;

   if (bottone_premuto == -1)
      bottone_premuto = ev->xbutton.button;
   else
      return;

   XRaiseWindow( display, XtWindow(w) ); /* label in primo-piano */

   switch (bottone_premuto)
   {
       case Button2 :
            def_cursore(macro_tavola,XC_fleur);

         /* recupera le dimensioni dei macroblocchi selezionati */
            indici = cerca_macro_selezionati();
            dim_widgets = (DimWidgets *) calloc_mem(num_macro_selez,
                                                   sizeof(DimWidgets));
            for(i=0 ; i<num_macro_selez ; i++ )
            {
                int ind;

                ind = indici[i];
                dim_widgets[i].indice = ind;
                dim_widgets[i].x = macroblocks[ind].pos_icona.posx;
                dim_widgets[i].y = macroblocks[ind].pos_icona.posy;
                get_something( macroblocks[ind].wmacro, XmNwidth,
                               (void*) &dim_widgets[i].width );
                get_something( macroblocks[ind].wmacro, XmNheight,
                               (void*) &dim_widgets[i].height );
            }
            num_dim_wdg = num_macro_selez;
            XtFree(indici);
            start_rubberband(ev, XtWindow(macro_tavola), dim_widgets,
                             num_dim_wdg, False ); 
            break;

       default :
            break;
   }
}

/*------------------------------------------------------------------*/
/*** macro_bcont(w, ind_macro, ev, boh)
 *** Parametri:
 ...
 *** L'utente tiene il bottone del mouse premuto (e non solo quello ...)
 ***/
void macro_bcont(w, ind_macro, ev, boh)
Widget w;
int ind_macro;
XEvent *ev;
Boolean boh;
{
   extern DimWidgets *dim_widgets;
   extern int num_dim_wdg;
   extern Boolean modifiche;

   switch (bottone_premuto)
   {
      case Button2 : 
           modifiche = True;
           continue_rubberband( ev, XtWindow(macro_tavola),
                                dim_widgets, num_dim_wdg, False,
                                BB_WIDTH, BB_HEIGHT );
           break;

      default :
           break;
   }
}

/*------------------------------------------------------------------*/
/*** macro_brelease(w, ind_macro, ev, boh)
 *** Parametri:
 ...
 *** L'utente ha sollevato il bottone del mouse (label del macroblocco)
 ***/
void macro_brelease(w, ind_macro, ev, boh)
Widget w;
int ind_macro;
XEvent *ev;
Boolean boh;
{
   int i;
   extern DimWidgets *dim_widgets;
   extern int num_dim_wdg;

   if (bottone_premuto == -1)
      return;

   switch (bottone_premuto)
   {
      case Button2:
        /* fine del trascinamento dell'icona */
           end_rubberband( ev, XtWindow(macro_tavola), dim_widgets,
                           num_dim_wdg, False, BB_WIDTH, BB_HEIGHT );

           for(i=0 ; i<num_dim_wdg ; i++)
           {
              int ind;

              ind = dim_widgets[i].indice;

              nargs = 0;
              XtSetArg(args[nargs], XmNx, dim_widgets[i].x ); nargs++;
              XtSetArg(args[nargs], XmNy, dim_widgets[i].y ); nargs++;
              XtSetValues( macroblocks[ind].wmacro, args, nargs );

              macroblocks[ ind ].pos_icona.posx = dim_widgets[i].x;
              macroblocks[ ind ].pos_icona.posy = dim_widgets[i].y;
           }
           XtFree(dim_widgets);
           num_dim_wdg = 0;

        /* Resetta il cursore */
           undef_cursore(macro_tavola);
           break;

      default :
           break;
   }
   bottone_premuto = -1;
}

/*------------------------------------------------------------------*/
/*** start_rubberband( ev, window, dim_widgets, num_dim_wdg, usare_root_coord )
 *** Parametri:
 ***  XEvent ev : struttura eventi relativa al ButtonPress
 ***  Window window : window dove si esegue lo spostamento degli oggetti 
 ***  DimWidgets dim_widgets : dimensioni e posizioni degli oggetti da spostare
 ***  int num_dim_wdg : numero di oggetti da spostare
 ***  Boolean usare_root_coord : booleano. Se True indica che bisogna 
 ***			considerare le coordinate della root-window piuttosto
 ***			delle coordinate "locali" della window
 ***                    (viene utilizzato quando bisogna spostare un oggetto
 ***			da una finestra all'altra).
Funzione per lo spostamento di oggetti all'interno delle window.  ***/
void start_rubberband( ev, window, dim_widgets, num_dim_wdg,
		       usare_root_coord )
XEvent *ev;
Window window;
DimWidgets *dim_widgets;
int num_dim_wdg;
Boolean usare_root_coord;
{
   int i;

   mousex0 = mouseprecx = ev->xbutton.x_root;
   mousey0 = mouseprecy = ev->xbutton.y_root;

   xoffs = ev->xbutton.x;
   yoffs = ev->xbutton.y;

   maxposx = maxposy = 0;
   if (num_dim_wdg)
   {
      minposx = dim_widgets[0].x;
      minposy = dim_widgets[0].y;
   }
   else
      minposx = minposy = 0;

/* Disegna i vari rettangoli che seguono lo spostamento del cursore.  */
/* Le dimensioni dei rettangoli sono le stesse delle label selezionate, */
/* PIU' 1 PIXEL per le cornici */
   for ( i=0 ; i<num_dim_wdg ; i++ )
   {
   /* recupera le posizioni minime e massime degli oggetti selezionati */
      minposx = (minposx < dim_widgets[i].x) ? minposx : dim_widgets[i].x;
      maxposx = (maxposx > dim_widgets[i].x+dim_widgets[i].width)
 			   ? maxposx : dim_widgets[i].x+dim_widgets[i].width;
      minposy = (minposy < dim_widgets[i].y) ? minposy : dim_widgets[i].y;
      maxposy = (maxposy > dim_widgets[i].y+dim_widgets[i].height)
			   ? maxposy : dim_widgets[i].y+dim_widgets[i].height;

      if (usare_root_coord)
      {
         XDrawRectangle (display, window ,xorGC,
                         mousex0 - xoffs + dim_widgets[i].x,
			 mousey0 - yoffs + dim_widgets[i].y,
                         (unsigned int) dim_widgets[i].width+1,
		         (unsigned int) dim_widgets[i].height+1);
      }
      else
      {
         XDrawRectangle (display, window ,xorGC,
                         dim_widgets[i].x, dim_widgets[i].y,
                         (unsigned int) dim_widgets[i].width+1,
		         (unsigned int) dim_widgets[i].height+1);
      }
   }
}

/*------------------------------------------------------------------*/
/*** end_rubberband( ev, dim_widgets, num_dim_wdg )
 *** Parametri:
 ***  XEvent ev : struttura eventi relativa al ButtonMotion
 ***  Window window : window dove si esegue lo spostamento degli oggetti 
 ***  DimWidgets dim_widgets : dimensioni e posizioni degli oggetti da spostare
 ***  int num_dim_wdg : numero di oggetti da spostare
 ***  Boolean usare_root_coord : booleano. Se True indica che bisogna 
 ***			considerare le coordinate della root-window piuttosto
 ***			delle coordinate "locali" della window
 ***                    (viene utilizzato quando bisogna spostare un oggetto
 ***			da una finestra all'altra).
 ***  int maxcoordx, maxcoordy : limiti in cui deve avvenire lo spostamento
 ***                    (il controllo sul limite viene effettuato solo nel caso
 ***			 in cui si utilizzano le coordinate locali della
 ***			 window)
Funzione per lo spostamento di oggetti all'interno delle window.  ***/
void end_rubberband( ev, window, dim_widgets, num_dim_wdg,
		     usare_root_coord, maxcoordx, maxcoordy )
XEvent *ev;
Window window;
DimWidgets *dim_widgets;
int num_dim_wdg;
Boolean usare_root_coord;
int maxcoordx, maxcoordy;
{
   int mousex, mousey, i;

   mousex = ev->xbutton.x_root;
   mousey = ev->xbutton.y_root;

   mousex = ((mousex-mouseprecx)%snap_val) ? mouseprecx : mousex;
   mousey = ((mousey-mouseprecy)%snap_val) ? mouseprecy : mousey;

/* calcola le posizioni minime e massime degli oggetti */
   if (!usare_root_coord)
   {
      if (minposx+mousex-mousex0 < 0 ||
          maxposx+mousex-mousex0 > maxcoordx-snap_val)
         mousex = mouseprecx;

      if (minposy+mousey-mousey0 < 0 ||
	  maxposy+mousey-mousey0 >= maxcoordy-snap_val)
         mousey = mouseprecy;
   }
   else
   {
/*
      minposx += mousex - mousex0;
      maxposx += mousex - mousex0;
      minposy += mousey - mousey0;
      maxposy += mousey - mousey0;
*/
   }

   for (i=0 ; i < num_dim_wdg ; i++ )
   {
       if (usare_root_coord)
          XDrawRectangle (display, window, xorGC,
                          mouseprecx - xoffs + dim_widgets[i].x,
		          mouseprecy - yoffs + dim_widgets[i].y,
                          (unsigned int) dim_widgets[i].width+1,
		          (unsigned int) dim_widgets[i].height+1);
       else
          XDrawRectangle (display, window, xorGC,
                          dim_widgets[i].x + mouseprecx - mousex0,
		          dim_widgets[i].y + mouseprecy - mousey0,
                          (unsigned int) dim_widgets[i].width+1,
		          (unsigned int) dim_widgets[i].height+1);

    /* nuova posizione della label */
       dim_widgets[i].x += (mousex - mousex0);
       dim_widgets[i].y += (mousey - mousey0);
   }
}

/*------------------------------------------------------------------*/
/*** continue_rubberband( ev, dim_widgets, num_dim_wdg )
 *** Parametri:
 ***  XEvent ev : struttura eventi relativa al ButtonMotion
 ***  Window window : window dove si esegue lo spostamento degli oggetti 
 ***  DimWidgets dim_widgets : dimensioni e posizioni degli oggetti da spostare
 ***  int num_dim_wdg : numero di oggetti da spostare
 ***  Boolean usare_root_coord : booleano. Se True indica che bisogna 
 ***			considerare le coordinate della root-window piuttosto
 ***			delle coordinate "locali" della window
 ***                    (viene utilizzato quando bisogna spostare un oggetto
 ***			da una finestra all'altra).
 ***  int maxcoordx, maxcoordy : limiti in cui deve avvenire lo spostamento
 ***                    (il controllo sul limite viene effettuato solo nel caso
 ***			 in cui si utilizzano le coordinate locali della
 ***			 window)
Funzione per lo spostamento di oggetti all'interno delle window.  ***/
void continue_rubberband( ev, window, dim_widgets, num_dim_wdg,
			  usare_root_coord, maxcoordx, maxcoordy )
XEvent *ev;
Window window;
DimWidgets *dim_widgets;
int num_dim_wdg;
Boolean usare_root_coord;
int maxcoordx, maxcoordy;
{
   int mousex, mousey, i;

   mousex = ev->xmotion.x_root;
   mousey = ev->xmotion.y_root;

   mousex = ((mousex-mouseprecx)%snap_val) ? mouseprecx : mousex;
   mousey = ((mousey-mouseprecy)%snap_val) ? mouseprecy : mousey;

/* controlla che lo spostamento avvenga nei limiti stabiliti. */
   if (!usare_root_coord)
   {
      if (minposx+mousex-mousex0 < 0 ||
	  maxposx+mousex-mousex0 > maxcoordx-snap_val)
         mousex = mouseprecx;

      if (minposy+mousey-mousey0 < 0 ||
 	  maxposy+mousey-mousey0 > maxcoordy-snap_val)
         mousey = mouseprecy;
   }

   for (i=0 ; i<num_dim_wdg ; i++ )
      if (usare_root_coord)
      {
         XDrawRectangle (display, window, xorGC,
   		         mouseprecx - xoffs + dim_widgets[i].x,
	  	         mouseprecy - yoffs + dim_widgets[i].y,
                         (unsigned int) dim_widgets[i].width+1,
		         (unsigned int) dim_widgets[i].height+1);

         XDrawRectangle (display,window, xorGC,
		         mousex - xoffs + dim_widgets[i].x,
		         mousey - yoffs + dim_widgets[i].y,
		         (unsigned int) dim_widgets[i].width+1,
		         (unsigned int) dim_widgets[i].height+1);
      }
      else
      {
         XDrawRectangle (display, window, xorGC,
   		      dim_widgets[i].x + mouseprecx - mousex0,
	  	      dim_widgets[i].y + mouseprecy - mousey0,
                      (unsigned int) dim_widgets[i].width+1,
		      (unsigned int) dim_widgets[i].height+1);

         XDrawRectangle (display,window, xorGC,
		      dim_widgets[i].x + mousex - mousex0,
		      dim_widgets[i].y + mousey - mousey0, 
		      (unsigned int) dim_widgets[i].width+1,
		      (unsigned int) dim_widgets[i].height+1);
      }

   mouseprecx = mousex;
   mouseprecy = mousey;
}

/*------------------------------------------------------------------*/
/*** attiva_popup(w, popup_widget, ev, boh)
 *** Parametri:
 ...
 *** Procedura per l'attivazione di un popup-menu
 ***/
void attiva_popup(w, popup_widget, event)
Widget w;
Widget *popup_widget;
XButtonEvent *event;
{
   if (event->button == Button3)
   {
      nargs=0;
      XmMenuPosition(popup_widget,event);
      XtManageChild (popup_widget);
   }
}

/*------------------------------------------------------------------*/
/*** void menu_pagina_indice(w)
 *** PARAMETRI
 *** Widget w: ID della menubar
 *** Compone la menubar della pagina indice.
 *** Il menu e' composto da tre voci (=CascadeButton)
 *** Il menu e' differente se utilizzato per l'attivita' DATI o
 *** per l'attivita' topologia (coincidente con MACRO).

Attivita' DATI:

EDIT 
====
Open

Attivita' TOPOLOGIA:

FILE		EDIT       SNAP
====		====       ====
Load icons	Open       modify value
Save		Copy       ------------
Close		-----      snap on/off		
		New
		Modify
		Delete

 ***/
void menu_pagina_indice(w)
Widget w;
{
   Widget cascade;
   Widget pulldown;
   Widget pushbutton, sep;
/*
 Menu ridotti per attivita' dati
*/
#ifdef DATI  
/* Creazione del menu EDIT */
   pulldown = XmCreatePulldownMenu (w,"Pulldown2",NULL,0);
/* Funzione definita in popup.c */
   macro_edit_items (pulldown, menu_edit_macro);
   nargs=0;
   XtSetArg(args[nargs],XmNtraversalOn,True); nargs++;
   XtSetArg(args[nargs],XmNmnemonic,69); nargs++;
   XtSetArg(args[nargs],XmNsubMenuId,pulldown); nargs++;
   cascade = XmCreateCascadeButton (w,"Edit",args,nargs);
   XtManageChild (cascade);
#else

#ifdef MAIN
/* Creazione del menu FILE */
   pulldown = XmCreatePulldownMenu (w,"Pulldown1",NULL,0);

   pushbutton = XmCreatePushButton (pulldown,"Load icons",NULL,0);
   XtAddCallback (pushbutton,XmNactivateCallback,menu_macro,K_LOAD_ICONS);
   XtManageChild (pushbutton);

   pushbutton = XmCreatePushButton (pulldown,"Save",NULL,0);
   XtAddCallback (pushbutton,XmNactivateCallback,menu_macro,K_SAVE);
   XtManageChild (pushbutton);

   pushbutton = XmCreatePushButton (pulldown,"Quit",NULL,0);
/* 2-2-95 Micheletti */
#ifndef TOPOLOGIA
   XtAddCallback (pushbutton,XmNactivateCallback,menu_macro,K_QUIT);
#endif
   XtManageChild (pushbutton);



   nargs=0;
   XtSetArg(args[nargs],XmNtraversalOn,True); nargs++;
   XtSetArg(args[nargs],XmNmnemonic,70); nargs++;
   XtSetArg(args[nargs],XmNsubMenuId,pulldown); nargs++;
   cascade = XmCreateCascadeButton (w,"File",args,nargs);
   XtManageChild (cascade);

#endif

/* Creazione del menu EDIT */
   pulldown = XmCreatePulldownMenu (w,"Pulldown2",NULL,0);

/* Funzione definita in popup.c */
   macro_edit_items (pulldown, menu_edit_macro);

   nargs=0;
   XtSetArg(args[nargs],XmNtraversalOn,True); nargs++;
   XtSetArg(args[nargs],XmNmnemonic,69); nargs++;
   XtSetArg(args[nargs],XmNsubMenuId,pulldown); nargs++;
   cascade = XmCreateCascadeButton (w,"Edit",args,nargs);
   XtManageChild (cascade);

/* Creazione del menu FILE */
   pulldown = XmCreatePulldownMenu (w,"Pulldown3",NULL,0);

   pushbutton = XmCreatePushButton (pulldown,"modify value",NULL,0);
   XtAddCallback (pushbutton,XmNactivateCallback,menu_macro,K_MODIFY_SNAP);
   XtManageChild (pushbutton);

   snap_status = True;
   nargs = 0;
   XtSetArg(args[nargs],XmNset,True); nargs++;
   XtSetArg(args[nargs],XmNindicatorOn,False); nargs++;
   XtSetArg(args[nargs],XmNvisibleWhenOff,False); nargs++;
   pushbutton = XmCreateToggleButton (pulldown,"Snap on",args,nargs);
   XtAddCallback (pushbutton,XmNvalueChangedCallback,toggle_proc,
						     K_SNAP_TOGGLE);
   XtManageChild (pushbutton);

   nargs=0;
   sep = XmCreateSeparatorGadget(pulldown, "Separator", NULL, 0);
   XtManageChild (sep);

   nargs=0;
   XtSetArg(args[nargs],XmNtraversalOn,True); nargs++;
   XtSetArg(args[nargs],XmNmnemonic,72); nargs++;
   XtSetArg(args[nargs],XmNsubMenuId,pulldown); nargs++;
   cascade = XmCreateCascadeButton (w,"Snap",args,nargs);
   XtManageChild (cascade);
#endif
}

/*------------------------------------------------------------------*/
/*** void dialog_modify_snap()
crea la dialog-box per la dello snap ***/
void dialog_modify_snap()
{
   Widget row_col, pushb, label, sep;

/* Creazione della form-dialog */
   nargs=0;
   XtSetArg(args[nargs],XmNnoResize,True); nargs++;
   XtSetArg(args[nargs],XmNautoUnmanage,False); nargs++;
   XtSetArg(args[nargs],XmNheight,140); nargs++;
   XtSetArg(args[nargs],XmNwidth, 250); nargs++;
   XtSetArg(args[nargs],XmNdialogTitle, CREATE_CSTRING("modify snap value"));
   nargs++;
   dbx_modify_snap = XmCreateFormDialog(top_level,"Form",args,nargs);

   nargs=0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 25); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNlabelString,
                         CREATE_CSTRING("SNAP VALUE:")); nargs++;
   label = XmCreateLabel(dbx_modify_snap, "Label",args,nargs);
   XtManageChild (label);

   nargs=0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 15); nargs++;
   XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs], XmNleftPosition, 55); nargs++;
   XtSetArg(args[nargs], XmNeditable,True); nargs++;
   XtSetArg(args[nargs], XmNeditMode, XmSINGLE_LINE_EDIT); nargs++;
   text_snap_val = XmCreateText(dbx_modify_snap, "Text",args,nargs);
   XtManageChild(text_snap_val);

   nargs = 0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs], XmNtopWidget, label); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 20); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
   sep = XmCreateSeparatorGadget(dbx_modify_snap,"Separatore",args, nargs);
   XtManageChild(sep);

   nargs = 0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs], XmNtopWidget, sep); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 10); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNmarginHeight, 5); nargs++;
   XtSetArg(args[nargs], XmNmarginWidth, 5); nargs++;
   XtSetArg(args[nargs], XmNorientation, XmVERTICAL); nargs++;
   XtSetArg(args[nargs], XmNpacking, XmPACK_COLUMN); nargs++;
   XtSetArg(args[nargs], XmNadjustLast, False); nargs++;
   XtSetArg(args[nargs], XmNentryAlignment, XmALIGNMENT_CENTER); nargs++;
   XtSetArg(args[nargs], XmNnumColumns, 2); nargs++;
   row_col = XmCreateRowColumn(dbx_modify_snap, "RadioBox", args, nargs );

   pushb = XmCreatePushButton (row_col,"Ok", NULL, 0);
   XtAddCallback(pushb, XmNactivateCallback, ok_proc, DBOX_MODIFY_SNAP);
   XtManageChild(pushb);

   pushb = XmCreatePushButton (row_col,"Cancel", NULL, 0);
   XtAddCallback(pushb, XmNactivateCallback, cancel_proc, dbx_modify_snap);
   XtManageChild(pushb);

   XtManageChild(row_col);
}

/*------------------------------------------------------------------*/
/*** void dialog_box_gen( dbox_window, window_title, 
 ***                      text1, title1, len1, text2, title2, len2,
 ***		          parametro )
 ***    Parametri:
 ***
 ***      Widget *dbox_window: widget della dialog-box (output)
 ***      char *window_title: titolo dialog-box
 ***      Widget *text1: widget text 1
 ***      char *title1: titolo text 1
 ***      int len1: lunghezza text 1
 ***      Widget *text2: widget text 2
 ***      char *title2: titolo text 2
 ***      int len2: lunghezza text 2
 ***      int parametro : parametro da passare alla callback quando si preme
 ***			  il bottone 'Ok'.
 *** Dialog_box utilizzato per:
 *** - la creazione di un nuovo macroblocco / commento
 *** - la modifica del nome/descrizione del macroblocco / blocco
 ***/
void dialog_box_gen( dbox_window, window_title, 
                     text1, title1, len1, text2, title2, len2, parametro )
Widget *dbox_window;
char *window_title;
Widget *text1;
char *title1;
int len1;
Widget *text2;
char *title2;
int len2;
int parametro;
{
   Widget label1, label2;
   Widget ok_dbox, cancel_dbox, separatore;
   XmString cstring;

/* Creazione della finestra di inserimento nome nuovo macroblocco (FORM) */
   nargs=0;
   cstring = CREATE_CSTRING(window_title);
   XtSetArg(args[nargs],XmNdefaultPosition,False); nargs++;
/*   XtSetArg(args[nargs], XmNresizePolicy,XmRESIZE_GROW); nargs++; */
   XtSetArg(args[nargs], XmNnoResize,True); nargs++;
   XtSetArg(args[nargs], XmNwidth,450); nargs++;
   XtSetArg(args[nargs], XmNheight,185); nargs++;
   XtSetArg(args[nargs], XmNverticalSpacing, 10); nargs++;
   XtSetArg(args[nargs], XmNhorizontalSpacing, 10); nargs++;
   XtSetArg(args[nargs], XmNdialogStyle,XmDIALOG_FULL_APPLICATION_MODAL);
   nargs++;
   XtSetArg(args[nargs],XmNdialogTitle, cstring); nargs++;
   *dbox_window = XmCreateFormDialog(top_level,"Pagina_indice",args,nargs);
   XmStringFree(cstring);

/* LABEL */
   cstring = CREATE_CSTRING(title1);
   nargs=0;

   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,25); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,5); nargs++;

/*   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_POSITION); nargs++; */
/*   XtSetArg(args[nargs],XmNbottomPosition, 30); nargs++; */

   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   label1 = XmCreateLabel (*dbox_window,"Pagina_indice",args,nargs);
   XtManageChild (label1);
   XmStringFree(cstring);

/* TEXT  */
   nargs=0;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,15); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightOffset,5); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget, label1); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,10); nargs++;
   
   XtSetArg(args[nargs],XmNeditable,True); nargs++;
   XtSetArg(args[nargs],XmNeditMode,XmSINGLE_LINE_EDIT); nargs++;
   XtSetArg(args[nargs],XmNmaxLength,len1); nargs++;
   XtSetArg(args[nargs],XmNtopCharacter,0); nargs++;
   XtSetArg(args[nargs],XmNbackground,apix[WHITE]); nargs++;
   *text1 = XmCreateText (*dbox_window,"Text",args,nargs);
   XtManageChild (*text1);

   if (title2 != NULL)
   {
   /* LABEL */
      cstring = CREATE_CSTRING(title2);
      nargs=0;

      XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
      XtSetArg(args[nargs],XmNtopWidget,label1); nargs++;
      XtSetArg(args[nargs],XmNtopOffset,30); nargs++;
      XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
      XtSetArg(args[nargs],XmNleftOffset,5); nargs++;
      
      XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
      label2 = XmCreateLabel (*dbox_window,"Pagina_indice",args,nargs);
      XtManageChild (label2);
      XmStringFree(cstring);

   /* TEXT  */
      nargs=0;
      XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
      XtSetArg(args[nargs],XmNtopWidget,label1); nargs++;
      XtSetArg(args[nargs],XmNtopOffset,20); nargs++;
      XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
      XtSetArg(args[nargs],XmNleftWidget,label2); nargs++;
      XtSetArg(args[nargs],XmNleftOffset,10); nargs++;
      XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
      XtSetArg(args[nargs],XmNrightOffset,5); nargs++;

      XtSetArg(args[nargs],XmNeditable,True); nargs++;
      XtSetArg(args[nargs],XmNeditMode,XmSINGLE_LINE_EDIT); nargs++;
      XtSetArg(args[nargs],XmNmaxLength,len2); nargs++;
      XtSetArg(args[nargs],XmNtopCharacter,0); nargs++;
      XtSetArg(args[nargs],XmNbackground,apix[WHITE]); nargs++;
      *text2 = XmCreateText (*dbox_window,"Text",args,nargs);
      XtManageChild (*text2);
   }

/* button 'OK' */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,10); nargs++;
   XtSetArg(args[nargs],XmNlabelString,CREATE_CSTRING("Ok")); nargs++;
   ok_dbox = XmCreatePushButton (*dbox_window,"Pagina_indice",args,nargs);
   XtManageChild (ok_dbox);
   XtAddCallback (ok_dbox,XmNactivateCallback,ok_proc,parametro);

/* button 'Cancel' */
   nargs=0;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,10); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget,ok_dbox); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,20); nargs++;
   XtSetArg(args[nargs],XmNlabelString,CREATE_CSTRING("Cancel")); nargs++;
   cancel_dbox = XmCreatePushButton(*dbox_window,"Pagina_indice",args,nargs);
   XtManageChild (cancel_dbox);
   XtAddCallback (cancel_dbox,XmNactivateCallback,cancel_proc, *dbox_window);

/* Definizione dei default button DOPO LA CREAZIONE DEL B_BOARD_DIALOG */
   XtSetArg(args[nargs],XmNdefaultButton, ok_dbox); nargs++;
   XtSetArg(args[nargs],XmNcancelButton, cancel_dbox); nargs++;
   XtSetValues (*dbox_window,args,nargs);

/* Separatore */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNbottomWidget,ok_dbox); nargs++;
/***
   if (title2 != NULL)
      XtSetArg(args[nargs],XmNtopWidget, *text2);
   else
      XtSetArg(args[nargs],XmNtopWidget, *text1);
   nargs++;
***/
   XtSetArg(args[nargs],XmNbottomOffset,20); nargs++;
   XtSetArg(args[nargs],XmNorientation,XmHORIZONTAL); nargs++;
   separatore=XmCreateSeparatorGadget(*dbox_window,"Pagina_indice",args,nargs);
   XtManageChild (separatore);

   return;
}

/*---------------------------------------------------------------------*/
void crea_gc()
{
   unsigned long valuemask = 0;  /* ignora XGCvalues e usa i default */
   XGCValues values;

   values.line_width = 1;
   values.line_style = LineSolid;
   values.cap_style = CapButt;
   values.join_style = JoinMiter;
   values.function = GXxor; 
   values.subwindow_mode = IncludeInferiors;
   values.foreground = apix[ BORDER_MOVE_FG ];
   valuemask = GCLineWidth | GCLineStyle | GCCapStyle | GCFunction | 
               GCJoinStyle | GCForeground | GCSubwindowMode;
   xorGC = XCreateGC (display, RootWindow(display, screen),
		      valuemask, &values);
   valuemask = GCLineWidth | GCLineStyle;
   values.line_width = 4;
   values.join_style = JoinMiter;
   iconGC = XCreateGC (display, RootWindow(display, screen), 
		       valuemask, &values);
   lineGC = XCreateGC (display, RootWindow(display, screen), 0, NULL);
}

/* fine macro_wdg.c */
