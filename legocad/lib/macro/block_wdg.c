/**********************************************************************
*
*       C Source:               block_wdg.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Apr  8 16:36:59 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: block_wdg.c-3 %  (%full_filespec: block_wdg.c-3:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)block_wdg.c	2.15\t4/11/95";
/*
        Fine sezione per SCCS
*/

/* BLOCK_WDG.C 		file di servizio per MACRO
			Crea gli widget dell'applicativo
			relativi alla pagina di un macroblocco
			(BLOCCHI)
   Versione 0.1		6.11.92			Buffo & Calleri  */

#include <stdio.h>
#include <math.h>
#ifdef VMS
#include <string.h>
#else
#include <memory.h>
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
#include <Xm/DrawingA.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/ScrolledW.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/BulletinB.h>
#include <Xm/ToggleB.h>
#include <Xm/CascadeBG.h>

#include "macro.h"
#include "file_icone.h"
#include "ico_mesg.h"
#include "libutilx.h"

#ifdef TOPOLOGIA
#include "lg1.h"
#include "errore.h"
extern int num_modulo;
extern char modulename[];
extern int main_menu_setup();
extern int add_block();
#endif

/* Variabili esterne dichiarate in MACRO.C ... */
extern Widget top_level;
extern Widget warning_widget;

extern Display *display;
extern int screen;
extern Cursor cursore_a_crocetta;

extern char *warning_mesg[];
extern Dialog_geometry geom_attention;

extern GC iconGC, xorGC, lineGC;

/* Variabili per utilizzare il 'find_kit' di libreria LIBUTILX */
Find_struct  find_module;

/**************************************************************/
/* variabili globali utilizzate nello spostamento delle label */
/* ed altre amenita' */
extern int mousex0, mousey0;
extern int mouseprecx, mouseprecy;
extern Position xlabel, ylabel;
extern Dimension hlabel, wlabel;
extern int macro_selezionato, num_win_blocchi;
extern int bottone_premuto;
/**************************************************************/

/* Strutture per la gestione degli widgets */
extern MacroBlockType *macroblocks;
extern int num_macro;

extern XmString stringa_nulla, cstring;

extern int snap_val;

extern Boolean modifiche;

extern Pixel apix[];
extern Arg args[];
extern Cardinal nargs;

static Widget widget_icona;

extern Widget dbx_nuovo_blocco, text_nome_blocco, text_descr_blocco;
extern Widget lista_moduli, scroll_icone, scroll_icone2;
extern Widget dbx_setup_line, text_line_thick, toggle_arrow;
extern Widget text_arrow_length, text_arrow_width;

extern unsigned int spessore_linea, lung_freccia, larg_freccia;

/*------------------------------------------------------------------*/
/*** void crea_finestra_blocchi(ind_macro)
 *** Parametri:
 ***    int ind_macro : indice dell'array macroblocks
 ***    Creazione della finestra dei blocchi di un macroblocco
 ***/
void crea_finestra_blocchi(ind_macro)
int ind_macro;
{
   char stringa[200];
   Widget win;
   Widget frame_block, header_block, menubar;
   Widget scroll_block, block_tavola;
   void funz_expose();
   int i;

   sprintf (stringa,"PAGE: %s", macroblocks[ind_macro].nome_macro);
   cstring = CREATE_CSTRING(stringa);


/* Creazione della finestra blocchi ( COME FORM ) */
   nargs=0;
   XtSetArg(args[nargs], XmNautoUnmanage,True); nargs++;
   XtSetArg(args[nargs], XmNnoResize,False); nargs++;
   XtSetArg(args[nargs], XmNdialogStyle,XmDIALOG_MODELESS); nargs++;
   XtSetArg(args[nargs], XmNdialogTitle, cstring); nargs++;
   XtSetArg(args[nargs], XmNdefaultPosition, False); nargs++;
   XtSetArg(args[nargs], XmNx,250+num_win_blocchi*30); nargs++;
   XtSetArg(args[nargs], XmNy,250+num_win_blocchi*30); nargs++;
   XtSetArg(args[nargs], XmNwidth,PAGE_WIDTH+50); nargs++;
   XtSetArg(args[nargs], XmNheight,PAGE_HEIGHT+100); nargs++;
   macroblocks[ind_macro].wwinblock = XmCreateFormDialog(top_level,
							"Finestra_blocchi",
							args,nargs);
   win = macroblocks[ind_macro].wwinblock;

/* MENUBAR */
   nargs=0;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNheight,30); nargs++;
   XtSetArg(args[nargs],XmNspacing,60); nargs++;
   XtSetArg(args[nargs],XmNorientation,XmHORIZONTAL); nargs++;
   menubar = XmCreateMenuBar (win, "Finestra_blocchi",args,nargs);
   XtManageChild (menubar);

/* Riempimento menu  */
   menu_finestra_blocchi(menubar, ind_macro);

/* SCROLLED WINDOW */

/* Creazione del frame che deve contenere il titolo della scroll window */
   nargs=0;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightOffset,5); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,5); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,menubar); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,15); nargs++;
   frame_block = XmCreateFrame (win,"Finestra_blocchi",args,nargs);
   XtManageChild (frame_block);

/* Creazione della label "MODEL DIAGRAM PAGE" */
   nargs=0;
   cstring = CREATE_CSTRING("MODEL DIAGRAM PAGE");
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   header_block = XmCreateLabel (frame_block,"Finestra_blocchi",args,nargs);
   XtManageChild (header_block);
   XmStringFree(cstring);

/* Creazione della scrolled window delle variabili che deve contenere 
   righe composte da widget di tipo diversi (gestiti da un'ulteriore form) */
   nargs=0;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightOffset,5); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,5); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,frame_block); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,15); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,5); nargs++;
   XtSetArg(args[nargs],XmNscrollBarDisplayPolicy,XmAS_NEEDED); nargs++;
   XtSetArg(args[nargs],XmNscrollingPolicy,XmAUTOMATIC); nargs++;
   XtSetArg(args[nargs],XmNvisualPolicy,XmCONSTANT); nargs++;
   XtSetArg(args[nargs],XmNscrollBarPlacement,XmBOTTOM_RIGHT); nargs++;
   scroll_block = XmCreateScrolledWindow(win,"Finestra_blocchi",args,nargs);
   XtManageChild (scroll_block);

/* La scrolled window contiene un Bulletin Board */
   nargs = 0;
   XtSetArg(args[nargs], XmNx, 0); nargs++;
   XtSetArg(args[nargs], XmNy, 0); nargs++;
   XtSetArg(args[nargs], XmNwidth, BB_WIDTH); nargs++;
   XtSetArg(args[nargs], XmNheight,BB_HEIGHT); nargs++;
   XtSetArg(args[nargs], XmNmarginWidth, 0); nargs++;
   XtSetArg(args[nargs], XmNmarginHeight,0); nargs++;
   XtSetArg(args[nargs], XmNresizePolicy, XmRESIZE_NONE); nargs++;
   XtSetArg(args[nargs], XmNbackground, apix[BLOCKS_TABLE_BG]);
   nargs++;
   XtSetArg(args[nargs], XmNuserData, &macroblocks[ind_macro]); nargs++;
   macroblocks[ind_macro].wtavblock = XmCreateDrawingArea(scroll_block,
				                "Block_tavola",args,nargs);
   XtAddEventHandler(macroblocks[ind_macro].wtavblock, ButtonPressMask,
		     False, start_select, (XtPointer)ind_macro);
   XtAddEventHandler(macroblocks[ind_macro].wtavblock, ButtonMotionMask,
		     False, continue_select, (XtPointer)ind_macro);
   XtAddEventHandler(macroblocks[ind_macro].wtavblock, ButtonReleaseMask,
		     False, end_select, (XtPointer)ind_macro);
   XtAddEventHandler(macroblocks[ind_macro].wtavblock, ExposureMask,
		     False, funz_expose, (XtPointer)ind_macro);
   popup_block(ind_macro);
   XtManageChild (macroblocks[ind_macro].wtavblock);
#ifdef DATI
   sprintf(stringa, STR_DESELECT_ALL_BLOCK, ind_macro,ind_macro);
#else
   sprintf(stringa, STR_DESELECT_ALL_BLOCK, ind_macro);
#endif
   add_translations( macroblocks[ind_macro].wtavblock, stringa );

}

/*------------------------------------------------------------------*/
/*** void deselect_all_block(w, ev, params, num_params)
 ...
 N.B. questa procedura deseleziona i blocchi di UN MACROBLOCCHO
      e non di altri macroblocchi eventualmente presenti
      e con blocchi selezionati
 ***/
void deselect_all_block(w, ev, params, num_params)
Widget w;
XEvent *ev;
String *params;
Cardinal *num_params;
{
   int ind_macro, i;
   MacroBlockType *ptr_macro;
   extern Widget widget_list_blocchi;

   if ( *num_params < 1 )
      return;

   sscanf(params[0],"%d",&ind_macro);
   ptr_macro = &macroblocks[ind_macro];
   for ( i=0 ; i<ptr_macro->num_blocchi ; i++ )
       if (ptr_macro->blocks[i].selezionato)
          seleziona_blocco(ptr_macro, &ptr_macro->blocks[i], False);

}

/*------------------------------------------------------------------*/
/*** void manage_select_block(w, ev, params, num_params)
 ...
 ***/
void manage_select_block(w, ev, params, num_params)
Widget w;
XEvent *ev;
String *params;
Cardinal *num_params;
{
   int modo, ind_macro, ind_blk;
   Boolean flag;

   if ( *num_params < 3 )
      return;

   sscanf(params[0],"%d",&modo);
   sscanf(params[1],"%d",&ind_macro);
   sscanf(params[2],"%d",&ind_blk);

   if (modo == 1) /* blocco selezionato senza lo Shift ... */
      {
#ifdef DATI
      desel_all_page_lista(w,ev,&params[1], num_params);
#endif
      deselect_all_block( w, ev, &params[1], num_params);
      }


   flag = !macroblocks[ind_macro].blocks[ind_blk].selezionato;
   seleziona_blocco(&macroblocks[ind_macro],
		    &macroblocks[ind_macro].blocks[ind_blk],
		    flag);
#ifdef DATI
   seleziona_blocco_lista(  &macroblocks[ind_macro],
			    &macroblocks[ind_macro].blocks[ind_blk],flag);
#endif

#ifdef TOPOLOGIA
   if(macroblocks[ind_macro].blocks[ind_blk].tipo == TP_BLOCK);
      main_menu_setup();
#endif
  
}

/*------------------------------------------------------------------*/
/*** void cancella_blocco(ptr_macro, ptr_block)
 ***   Parametri:
 ***   MacroBlockType *ptr_macro: macroblocco
 ***   BlockType *ptr_block: blocco
rimuove il widget del blocco (blocco/simbolo/remark) ed aggiorna la struttura
BlockType notificandone la cancellazione */
void cancella_blocco(ptr_macro, ptr_block)
MacroBlockType *ptr_macro;
BlockType *ptr_block;
{
   Boolean flag = False;

    if (ptr_block == NULL)
       return;

    modifiche = True;

/** aggiunta il 10-9-93 ***/
  if ((flag && ptr_block->selezionato) || (!flag && !ptr_block->selezionato) ||
       ptr_block->cancellato)
      ;
  else
  {
     ptr_block->selezionato = flag;
     ptr_macro->num_blocchi_selez--;
     if (ptr_block->wblock != NULL)
     {
        set_something_val(ptr_block->wblock,XmNborderColor, (XtArgVal) apix[BLOCKS_TABLE_BG]);
       cambia_edit_blk_menu(ptr_macro);  /* Abilita/disabilita voci menu pop-up */
     }
  }

    ptr_block->cancellato = True;

    if (ptr_block->wblock != NULL)
       XtDestroyWidget(ptr_block->wblock);
}

/*------------------------------------------------------------------*/
/*** int alloca_nuovo_blocco(ptr_macro, x, y, tipo, selez, pixm_info,
			     nome_blocco, descr_blocco, nome_modulo )
 ***  Parametri:
 ***   MacroBlockType *ptr_macro: macroblocco
 ***   int posx, posy : posizione del blocco nella window
 ***   short int tipo: tipo blocco (blocco,simbolo o commento)
 ***   PixmapInfo *pixm_info: info relativa alla pixmap
 ***   char *nome_blocco : nome blocco (NULL se simbolo o commento)
 ***   char *descr_blocco: descrizione del blocco o commento (NULL se simbolo)
 ***   char *nome_modulo : nome del modulo (NULL se simbolo o commento)
inserisce un nuovo blocco/simbolo/commento nella lista blocks[] del 
macroblocco. Ritorna l'indice del nuovo blocco inserito */
int alloca_nuovo_blocco(MacroBlockType *ptr_macro, int posx, int posy, short int tipo, PixmapInfo *pixm_info,
			char*nome_blocco, char*descr_blocco, char*nome_modulo )
{
   int i;

   if (ptr_macro == NULL)
      return(0);

   i = ++ptr_macro->num_blocchi;
   if ( i >= ptr_macro->num_blocchi_alloc )
      ptr_macro->blocks = (BlockType *) realloc_mem( (char *)ptr_macro->blocks,
				     ptr_macro->num_blocchi_alloc += 100,
                                     sizeof(BlockType) );

   strcpy(ptr_macro->blocks[i-1].nome_blocco, nome_blocco);

   strcpy(ptr_macro->blocks[i-1].descr_blocco, descr_blocco);

   strcpy(ptr_macro->blocks[i-1].nome_modulo, nome_modulo);
   ptr_macro->blocks[i-1].pos_icona.posx = posx;
   ptr_macro->blocks[i-1].pos_icona.posy = posy;
   ptr_macro->blocks[i-1].selezionato = False;
   ptr_macro->blocks[i-1].cancellato = False;
   if (pixm_info != NULL)
       ptr_macro->blocks[i-1].num_icona = pixm_info->num_icona;
   ptr_macro->blocks[i-1].pixmap_info = pixm_info;
   ptr_macro->blocks[i-1].tipo = tipo;
   modifiche = True;

   return(i-1);
}

/*------------------------------------------------------------------*/
/*** block_bpress(w, ind_macro, ev, boh)
 *** Parametri:
 ...
 *** L'utente ha selezionato il blocco da spostare ***/
void block_bpress(w, ptr_macro, ev, boh)
Widget w;
MacroBlockType *ptr_macro;
XEvent *ev;
Boolean boh;
{
   BlockType *pointer_block, *blocks;
   int *indici, i;
   extern DimWidgets *dim_widgets;
   extern int num_dim_wdg;

   if (bottone_premuto == -1)
      bottone_premuto = ev->xbutton.button;
   else
      return;

   get_something( w, XmNuserData, (void*) &pointer_block );
   if (pointer_block->tipo == TP_REMARK)
      widget_icona = w; /* widget label of remark */
   else
      widget_icona = XtParent(w); /* widget form icona of the block */
/*
   switch (pointer_block->tipo) {
      case TP_BLOCK:
           printf("blocco premuto: <BLOCCO> %2d %8s\n",
					 pointer_block->num_icona,
					 pointer_block->nome_blocco);

           break;
      case TP_SYMBOL:
           printf("blocco premuto: <SYMBOL> %2d\n", pointer_block->num_icona);

           break;
      case TP_REMARK:
           printf("blocco premuto: <REMARK> %.20s\n",
				         pointer_block->descr_blocco);

           break;
   }
*/
   switch ( bottone_premuto ) {

      case Button1 :

           return;
           break;

      case Button2 : 

      /* recupera le dimensioni dei blocchi selezionati */
	   indici = cerca_blocchi_selezionati(ptr_macro);
           dim_widgets = (DimWidgets *) calloc_mem(ptr_macro->num_blocchi_selez,
                                                   sizeof(DimWidgets));
           blocks = ptr_macro->blocks;
	   for(i=0 ; i<ptr_macro->num_blocchi_selez ; i++ )
           {
               int ind;

               ind = indici[i];
	       dim_widgets[i].indice = ind;
	       dim_widgets[i].x = blocks[ind].pos_icona.posx;
               dim_widgets[i].y = blocks[ind].pos_icona.posy;
               get_something( blocks[ind].wblock, XmNwidth,
			      (void*) &dim_widgets[i].width );
               get_something( blocks[ind].wblock, XmNheight,
			      (void*) &dim_widgets[i].height );
           }
           num_dim_wdg = ptr_macro->num_blocchi_selez;
           XtFree((char *)indici);
           start_rubberband( ev, XtWindow(ptr_macro->wtavblock), dim_widgets,
			     num_dim_wdg, False );
           break;

      case Button3 :
/***
           nargs=0;
           XtSetArg (args[nargs], XmNx, ev->xbutton.x_root); nargs++; 
           XtSetArg (args[nargs], XmNy, ev->xbutton.y_root); nargs++;
           XtSetValues (ptr_macro->wpopup, args, nargs);
           XtManageChild (ptr_macro->wpopup);
***/
           break;

      default :
           break;
   }
}

/*------------------------------------------------------------------*/
/*** block_bcont(w, client_data, ev, boh)
 *** Parametri:
 ...
 *** L'utente tiene bene impugnato il bottone del mouse (ed ,ovviamente,
 *** non solo quello ...)
 ***/
void block_bcont(w, ptr_macro, ev, boh)
Widget w;
MacroBlockType *ptr_macro;
XEvent *ev;
Boolean boh;
{
   extern DimWidgets *dim_widgets;
   extern int num_dim_wdg;

   switch ( bottone_premuto ) {

       case Button2 :
            modifiche = True; /* Setta il flag di modifica ! */
            continue_rubberband( ev, XtWindow(ptr_macro->wtavblock),
				 dim_widgets, num_dim_wdg, False,
				 BB_WIDTH, BB_HEIGHT );
            break;

       default :
            break;
   }
}

/*------------------------------------------------------------------*/
/*** block_brelease(w, client_data, ev, boh)
 *** Parametri:
 ...
 *** L'utente ha rilasciato il blocco da spostare ***/
void block_brelease(w, ptr_macro, ev, boh)
Widget w;
MacroBlockType *ptr_macro;
XEvent *ev;
Boolean boh;
{
   BlockType *pointer_block, *blocks;
   int i;
   extern DimWidgets *dim_widgets;
   extern int num_dim_wdg;

   if (bottone_premuto == -1)
      return;

   get_something( w, XmNuserData, (void*) &pointer_block );

   switch (bottone_premuto)
   {
      case Button2:
        /* fine del trascinamento dell'icona */
           end_rubberband( ev, XtWindow(ptr_macro->wtavblock), dim_widgets,
			   num_dim_wdg, False, BB_WIDTH, BB_HEIGHT );

           blocks = ptr_macro->blocks;
	   for(i=0 ; i<num_dim_wdg ; i++ )
           {
              int ind;

              ind = dim_widgets[i].indice; /* recupera l'indice in blocks */

              nargs = 0;
              XtSetArg(args[nargs], XmNx, dim_widgets[i].x ); nargs++;
              XtSetArg(args[nargs], XmNy, dim_widgets[i].y ); nargs++;
              XtSetValues( blocks[ind].wblock, args, nargs );

              blocks[ind].pos_icona.posx = dim_widgets[i].x;
              blocks[ind].pos_icona.posy = dim_widgets[i].y;
           }
           XtFree((char *)dim_widgets);
           num_dim_wdg = 0;
           break;

      case Button3 :
/***
           XtUnmanageChild (ptr_macro->wpopup);
***/
           break;

      default : break;
   }
   bottone_premuto = -1;
}

/*------------------------------------------------------------------*/
/*** display_icone_macro( ind_macro )
 ***   Parametri:
 ***     int inf_macro : indice in macroblocs[].
crea le icone dei blocchi. */
void display_icone_macro( ind_macro )
int ind_macro;
{
   int i, num_icona;
   char nome_modulo[5];
   PixmapInfo *pixm_info;
   PixmapModuleStruct *pixm_module;
   MacroBlockType *mblock;
   extern PixmapSymbolStruct pixmap_symbol;

   mblock = &macroblocks[ind_macro];

   strcpy(nome_modulo, "");

   for ( i=0 ; i<mblock->num_blocchi; i++ )
   {
       if (mblock->blocks[i].cancellato)
          continue;

       num_icona = mblock->blocks[i].num_icona;
       switch (mblock->blocks[i].tipo)
       {
           case TP_BLOCK:
           case TP_BLOCK_REG:
               pixm_info = recupera_pixmap(mblock->blocks[i].tipo,
                                           mblock->blocks[i].num_icona,
                                           mblock->blocks[i].nome_modulo);
               crea_icona( mblock->wtavblock, pixm_info,
                           mblock->blocks[i].nome_blocco,
	                   mblock->blocks[i].pos_icona.posx,
                           mblock->blocks[i].pos_icona.posy, 
			   FROM_WINDOW_BLOCK, ind_macro, i);
               break;

           case TP_REMARK:
               crea_remark( mblock->wtavblock,
                            mblock->blocks[i].descr_blocco,
	                    mblock->blocks[i].pos_icona.posx,
                            mblock->blocks[i].pos_icona.posy, ind_macro, i);
               break;

           case TP_SYMBOL:
               pixm_info = recupera_pixmap(TP_SYMBOL,
                                           mblock->blocks[i].num_icona,
                                           NULL );
               crea_icona( mblock->wtavblock, pixm_info, "",
			   mblock->blocks[i].pos_icona.posx,
                           mblock->blocks[i].pos_icona.posy, 
			   FROM_WINDOW_BLOCK, ind_macro, i);
               break;
       }
   }
}

/*------------------------------------------------------------------*/
/*** void menu_finestra_blocchi(w, ind_macro )
 *** PARAMETRI:
 ***   Widget w: ID della menubar
 ***   int ind_macro: indice dell'array macroblocks
Compone la menubar della finestra_blocchi.
Nel caso attivita' DATI il menu e' il seguente


FILE	EDIT 
====	====
Quit	Variables... 
    	Data...
	------
	Known/Unknown...
	------
	Normalization...


Il menu nel caso TOPOLOGIA e' composto da tre voci:

FILE	EDIT         LINE
====	====         ====
Quit	Copy         Draw
	Paste        Delete  
    	Connect      ------
	-------      Setup...
	Modify
	Delete
	-------
	Move
	-------
	Add Block
	Add Symbol
	Add Remark
 
Modifica al menu nel caso TOPOLOGIA  31-8-93
Motivo : integrazione con Lg1 

FILE    EDIT         GRAPHICS
====    ====         ========
Quit    Variables    Add Symbol 
        New Block*   Add Remark
        Modify       Move
        Delete       Lw_lineines*
        Move


        New Block -> Process          
                     Regulation

        Lines     -> Draw
                     Delete
                     Setup

***/
void menu_finestra_blocchi(w, ind_macro)
Widget w;
int ind_macro;
{
   Widget pushb, sep, pulldown, cascade, cascadesub1, submenu1;

/* Creazione del menu FILE */
   pulldown = XmCreatePulldownMenu (w,"Pulldown",NULL,0);
#ifdef TOPOLOGIA
   pushb = XmCreatePushButton (pulldown,"Close",NULL,0);
#else
   pushb = XmCreatePushButton (pulldown,"Quit",NULL,0);
#endif
   XtAddCallback (pushb,XmNactivateCallback,chiudi_window_blocchi,(XtPointer)ind_macro);
   XtManageChild (pushb);

   nargs=0;
   XtSetArg(args[nargs],XmNtraversalOn,True); nargs++;
   XtSetArg(args[nargs],XmNmnemonic,70); nargs++;
   XtSetArg(args[nargs],XmNsubMenuId,pulldown); nargs++;
   pushb = XmCreateCascadeButton (w,"File",args,nargs);
   XtManageChild (pushb);

/* Creazione del menu EDIT */
   pulldown = XmCreatePulldownMenu (w,"Pulldown",NULL,0);

/* Funzione definita in popup.c */
   block_edit_items (pulldown, ind_macro,
		     macroblocks[ind_macro].menu_edit_blocchi);

   nargs=0;
   XtSetArg(args[nargs],XmNtraversalOn,True); nargs++;
   XtSetArg(args[nargs],XmNmnemonic,69); nargs++;
   XtSetArg(args[nargs],XmNsubMenuId,pulldown); nargs++;
   cascade = XmCreateCascadeButton (w,"Edit",args,nargs);
   XtManageChild (cascade);

/****#ifndef DATI    menu utilizzato solo per costruzione topologica */

#ifdef TOPOLOGIA
/* Creazione del menu LINE */
   pulldown = XmCreatePulldownMenu (w,"Pulldown",NULL,0);

   pushb = XmCreatePushButton (pulldown,"Add symbol",NULL,0);
   XtAddCallback (pushb,XmNactivateCallback,add_symbol,(XtPointer)ind_macro);
   XtManageChild (pushb);
                         
   pushb = XmCreatePushButton (pulldown,"Add remark",NULL,0);
   XtAddCallback (pushb,XmNactivateCallback,add_remark,(XtPointer)ind_macro);
   XtManageChild (pushb);

/* creazione sottomenu disegno linee */
   submenu1 = XmCreatePulldownMenu(pulldown,"Submenu1",NULL,0);
 
   nargs=0;
   XtSetArg(args[nargs],XmNsubMenuId,submenu1); nargs++;
   cascadesub1 = (Widget)XmCreateCascadeButtonGadget (pulldown,"Add line",args,nargs);
   XtManageChild (cascadesub1);

   pushb = XmCreatePushButton (submenu1,"Draw",NULL,0);
   XtAddCallback (pushb,XmNactivateCallback,funz_draw_line,(XtPointer)ind_macro);
   XtManageChild (pushb);

   pushb = XmCreatePushButton (submenu1,"Delete",NULL,0);
   XtAddCallback (pushb,XmNactivateCallback,delete_line,(XtPointer)ind_macro);
   XtManageChild (pushb);

   pushb = XmCreatePushButton (submenu1,"Setup...",NULL,0);
   XtAddCallback (pushb,XmNactivateCallback,setup_line,NULL);
   XtManageChild (pushb);

   nargs=0;
   XtSetArg(args[nargs],XmNtraversalOn,True); nargs++;
   XtSetArg(args[nargs],XmNmnemonic,74); nargs++;
   XtSetArg(args[nargs],XmNsubMenuId,pulldown); nargs++;

   cascade = XmCreateCascadeButton (w,"Graphics",args,nargs);
   XtManageChild (cascade);

   pushb = XmCreatePushButton (pulldown,"Move",NULL,0);
   XtAddCallback (pushb,XmNactivateCallback, move_selected_blocks,(XtPointer)ind_macro);
   XtManageChild (pushb);
#else   
#ifndef DATI
/* Creazione del menu LINE */
   pulldown = XmCreatePulldownMenu (w,"Pulldown",NULL,0);

   pushb = XmCreatePushButton (pulldown,"Draw",NULL,0);
   XtAddCallback (pushb,XmNactivateCallback,funz_draw_line,ind_macro);
   XtManageChild (pushb);
                         
   pushb = XmCreatePushButton (pulldown,"Delete",NULL,0);
   XtAddCallback (pushb,XmNactivateCallback,delete_line,ind_macro);
   XtManageChild (pushb);

   pushb = XmCreatePushButton (pulldown,"Setup...",NULL,0);
   XtAddCallback (pushb,XmNactivateCallback,setup_line,ind_macro);
   XtManageChild (pushb);

   nargs=0;
   XtSetArg(args[nargs],XmNtraversalOn,True); nargs++;
   XtSetArg(args[nargs],XmNmnemonic,69); nargs++;
   XtSetArg(args[nargs],XmNsubMenuId,pulldown); nargs++;
   cascade = XmCreateCascadeButton (w,"Line",args,nargs);
   XtManageChild (cascade);

#endif
#endif

}

/*------------------------------------------------------------------*/
/*** void setup_line(w,par,call_data)
 *** configurazione delle line (spessore, colore, freccia)
 ***/
void setup_line(w, par, call_data)
Widget w;
caddr_t *par;
XmAnyCallbackStruct *call_data;
{
   char strnum[10];

   if (dbx_setup_line == NULL)
      dialog_setup_line();
   
   sprintf(strnum,"%-3d", spessore_linea);
   XmTextSetString(text_line_thick, strnum);
   sprintf(strnum,"%-3d", lung_freccia);
   XmTextSetString(text_arrow_length, strnum);
   sprintf(strnum,"%-3d", larg_freccia);
   XmTextSetString(text_arrow_width, strnum);
   XtManageChild(dbx_setup_line);
}

/*------------------------------------------------------------------*/
/*** void dialog_setup_line()
crea la dialog-box per la configurazione delle line (spessore,
colore, freccia) ***/
void dialog_setup_line()
{
   Widget row_col, pushb, label1, label2, label3;
   Widget label4, label5;
   Widget toggle, sub_menu, option_menu, sep;
   int i;
   extern char *names[];

/* Creazione della form-dialog */
   nargs=0;
   XtSetArg(args[nargs],XmNnoResize,True); nargs++;
   XtSetArg(args[nargs],XmNautoUnmanage,False); nargs++;
   XtSetArg(args[nargs],XmNheight,300); nargs++;
   XtSetArg(args[nargs],XmNwidth, 350); nargs++;
   XtSetArg(args[nargs],XmNdialogTitle, CREATE_CSTRING("setup lines ..."));
   nargs++;
   dbx_setup_line = XmCreateFormDialog(top_level,"Form",args,nargs);

   nargs=0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 25); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNlabelString,
                         CREATE_CSTRING("LINE THICKNESS:")); nargs++;
   label1 = XmCreateLabel(dbx_setup_line, "Label",args,nargs);
   XtManageChild (label1);

   nargs=0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 15); nargs++;
   XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs], XmNleftPosition, 55); nargs++;
   XtSetArg(args[nargs], XmNeditable,True); nargs++;
   XtSetArg(args[nargs], XmNeditMode, XmSINGLE_LINE_EDIT); nargs++;
   text_line_thick = XmCreateText(dbx_setup_line, "Text",args,nargs);
   XtManageChild(text_line_thick);

   nargs=0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs], XmNtopWidget, label1); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 25); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNlabelString,
                         CREATE_CSTRING("ARROW LENGHT:")); nargs++;
   label2 = XmCreateLabel(dbx_setup_line, "Label",args,nargs);
   XtManageChild (label2);

   nargs=0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs], XmNtopWidget, label1); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 15); nargs++;
   XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs], XmNleftPosition, 55); nargs++;
   XtSetArg(args[nargs], XmNeditable,True); nargs++;
   XtSetArg(args[nargs], XmNeditMode, XmSINGLE_LINE_EDIT); nargs++;
   text_arrow_length = XmCreateText(dbx_setup_line, "Text",args,nargs);
   XtManageChild(text_arrow_length);

   nargs=0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs], XmNtopWidget, label2); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 25); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNlabelString,
                         CREATE_CSTRING("ARROW WIDTH:")); nargs++;
   label3 = XmCreateLabel(dbx_setup_line, "Label",args,nargs);
   XtManageChild (label3);

   nargs=0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs], XmNtopWidget, label2); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 15); nargs++;
   XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs], XmNleftPosition, 55); nargs++;
   XtSetArg(args[nargs], XmNeditable,True); nargs++;
   XtSetArg(args[nargs], XmNeditMode, XmSINGLE_LINE_EDIT); nargs++;
   text_arrow_width = XmCreateText(dbx_setup_line, "Text",args,nargs);
   XtManageChild(text_arrow_width);

   nargs=0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs], XmNtopWidget, label3); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 25); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNlabelString,
                         CREATE_CSTRING("LINE COLOR:")); nargs++;
   label4 = XmCreateLabel(dbx_setup_line, "Label",args,nargs);
   XtManageChild(label4);

/* option menu per la scelta dei colori */
   sub_menu = XmCreatePulldownMenu(dbx_setup_line, "Pulldown", NULL, 0);

   for ( i=0 ; i<6 ; i++ )
   {
       pushb = XmCreatePushButton(sub_menu, names[i], NULL, 0);
       XtAddCallback( pushb, XmNactivateCallback, scelta_colore, (XtPointer)i);
       XtManageChild( pushb );
   }

   nargs=0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs], XmNtopWidget, label3); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 15); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs], XmNleftPosition, 55); nargs++;
   XtSetArg(args[nargs], XmNsubMenuId, sub_menu); nargs++;
   option_menu = XmCreateOptionMenu(dbx_setup_line, "Options", args, nargs);
   XtManageChild(option_menu);

   nargs=0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs], XmNtopWidget, label4); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 25); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNlabelString,
                         CREATE_CSTRING("ARROW AT THE END? :")); nargs++;
   label5 = XmCreateLabel(dbx_setup_line, "Label",args,nargs);
   XtManageChild (label5);

   nargs=0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs], XmNtopWidget, label4); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 25); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs], XmNleftPosition, 55); nargs++;
   XtSetArg(args[nargs], XmNset, True ); nargs++;
   XtSetArg(args[nargs], XmNlabelString, CREATE_CSTRING("Yes")); nargs++;
   toggle_arrow = XmCreateToggleButton(dbx_setup_line, "Toggle",args,nargs);
   XtAddCallback( toggle_arrow, XmNvalueChangedCallback, toggle_proc, 
							 (XtPointer)K_ARROW_TOGGLE );
   XtManageChild (toggle_arrow);

   nargs = 0;
   XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs], XmNtopWidget, label5); nargs++;
   XtSetArg(args[nargs], XmNtopOffset, 20); nargs++;
   XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
   XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
   sep = XmCreateSeparatorGadget(dbx_setup_line,"Separatore",args, nargs);
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
   XtSetArg(args[nargs], XmNnumColumns, 3); nargs++;
   row_col = XmCreateRowColumn(dbx_setup_line, "RadioBox", args, nargs );

   pushb = XmCreatePushButton (row_col,"Ok", NULL, 0);
   XtAddCallback(pushb, XmNactivateCallback, ok_proc, (XtPointer)DBOX_SETUP_LINE);
   XtManageChild(pushb);

   pushb = XmCreatePushButton (row_col,"Apply", NULL, 0);
   XtAddCallback(pushb, XmNactivateCallback, ok_proc, (XtPointer)DBOX_SETUP_LINE_APPLY);
   XtManageChild(pushb);

   pushb = XmCreatePushButton (row_col,"Cancel", NULL, 0);
   XtAddCallback(pushb, XmNactivateCallback, cancel_proc, dbx_setup_line);
   XtManageChild(pushb);

   XtManageChild(row_col);
}

/*------------------------------------------------------------------*/
/*** void dialog_nuovo_blocco()
 *** Crea la finestra di scelta dell'icona per un blocco.
 ***/
void dialog_nuovo_blocco()
{
   Widget label_lista_moduli, w_find;
   Widget label_scroll_icone;
   Widget rc_blocco;
   Widget label_nome_blocco;
   Widget label_descr_blocco;
   Widget ok_nuovo_blocco, cancel_nuovo_blocco;
   Widget sep1, sep2, sep3;

/* Creazione della finestra di inserimento nome nuovo blocco (FORM) */
   cstring = CREATE_CSTRING("NEW BLOCK CREATION");
   nargs=0;
   XtSetArg(args[nargs],XmNdefaultPosition,False); nargs++;
   XtSetArg(args[nargs],XmNx,500); nargs++;
   XtSetArg(args[nargs],XmNy,180); nargs++;
   XtSetArg(args[nargs],XmNautoUnmanage,True); nargs++;
   XtSetArg(args[nargs],XmNnoResize,False); nargs++;
   XtSetArg(args[nargs],XmNdialogTitle, cstring); nargs++;
   XtSetArg(args[nargs], XmNwidth,600); nargs++;
   XtSetArg(args[nargs], XmNheight,550); nargs++;
   XtSetArg(args[nargs], XmNverticalSpacing, 10); nargs++;
   XtSetArg(args[nargs], XmNhorizontalSpacing, 10); nargs++;
   dbx_nuovo_blocco = XmCreateFormDialog (top_level,"Finestra_blocchi",
					  args,nargs);
   XmStringFree(cstring);

/* Label della lista dei moduli */
   cstring = CREATE_CSTRING("Module     Icons     Description");
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   label_lista_moduli = XmCreateLabel (dbx_nuovo_blocco,"Finestra_blocchi",
				       args,nargs);
   XtManageChild (label_lista_moduli);
   XmStringFree(cstring);

/* Scrolled list dei moduli */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightOffset,10); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget, label_lista_moduli); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
   XtSetArg(args[nargs],XmNtraversalOn,False); nargs++;
   XtSetArg(args[nargs],XmNlistSizePolicy,XmRESIZE_IF_POSSIBLE); nargs++;
   XtSetArg(args[nargs],XmNselectionPolicy,XmSINGLE_SELECT); nargs++;
   XtSetArg(args[nargs],XmNscrollBarDisplayPolicy,XmAS_NEEDED); nargs++;
   XtSetArg(args[nargs],XmNvisibleItemCount,VISIBLE_ITEMS); nargs++;
   lista_moduli = XmCreateScrolledList (dbx_nuovo_blocco,"Lista_moduli",
       				        args,nargs);
   XtManageChild (lista_moduli);
   XtAddCallback (lista_moduli,XmNsingleSelectionCallback,selez_modulo,
		  NULL);

/* Inserimento del FIND di libreria */
   w_find = crea_find(lista_moduli);

/* Separatore */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget, w_find); nargs++;
   XtSetArg(args[nargs],XmNtopOffset, 5); nargs++;
   XtSetArg(args[nargs],XmNorientation,XmHORIZONTAL); nargs++;
   sep1 = XmCreateSeparatorGadget (dbx_nuovo_blocco,"Finestra_blocchi",
				   args,nargs);
   XtManageChild (sep1);

/* Label della scrolled window delle icone */
   cstring = CREATE_CSTRING("ASSOCIATED ICONS");
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget, sep1); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,15); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   label_scroll_icone = XmCreateLabel (dbx_nuovo_blocco,"Finestra_blocchi",
				       args,nargs);
   XtManageChild (label_scroll_icone);
   XmStringFree(cstring);

/* button 'OK' */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,10); nargs++;
   XtSetArg(args[nargs],XmNlabelString,CREATE_CSTRING("Fine")); nargs++;
   ok_nuovo_blocco = XmCreatePushButton (dbx_nuovo_blocco,"Finestra_blocchi",
					 args,nargs);
   XtAddCallback (ok_nuovo_blocco,XmNactivateCallback,ok_proc,
                  (XtPointer)DBOX_NEW_BLOCK);
   XtManageChild (ok_nuovo_blocco);

/* button 'Cancel' */
   nargs=0;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,10); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget,ok_nuovo_blocco); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,20); nargs++;
   XtSetArg(args[nargs],XmNlabelString,CREATE_CSTRING("Cancel")); nargs++;
   cancel_nuovo_blocco = XmCreatePushButton 
			(dbx_nuovo_blocco,"Finestra_blocchi",args,nargs);
   XtAddCallback (cancel_nuovo_blocco,XmNactivateCallback,ok_proc,
                  (XtPointer)DBOX_NEW_BLOCK);
   XtManageChild (cancel_nuovo_blocco);

/* Separatore */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNbottomWidget, ok_nuovo_blocco); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,15); nargs++;
   XtSetArg(args[nargs],XmNorientation,XmHORIZONTAL); nargs++;
   sep3 = XmCreateSeparatorGadget (dbx_nuovo_blocco,"Finestra_blocchi",
				   args,nargs);
   XtManageChild (sep3);

/** ULTERIORE FORM per i dati del nuovo blocco **/
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNbottomWidget, sep3); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,15); nargs++;
   rc_blocco = XmCreateForm (dbx_nuovo_blocco,"Finestra_blocchi",args,nargs);
   XtManageChild (rc_blocco);

/* Nome del nuovo blocco */
/* LABEL */
   cstring = CREATE_CSTRING("BLOCK NAME:");
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   label_nome_blocco = XmCreateLabel(rc_blocco,"Finestra_blocchi",args,nargs);
   XtManageChild (label_nome_blocco);
   XmStringFree(cstring);

/* TEXT  */
   nargs=0;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget, label_nome_blocco); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,10); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNrightPosition, 35); nargs++;
   XtSetArg(args[nargs],XmNeditable,True); nargs++;
   XtSetArg(args[nargs],XmNeditMode,XmSINGLE_LINE_EDIT); nargs++;
   XtSetArg(args[nargs],XmNmaxLength,4); nargs++;
   text_nome_blocco = XmCreateText (rc_blocco,"Text",args,nargs);
   XtManageChild (text_nome_blocco);

/* Descrizione del nuovo blocco */
/* LABEL */
   cstring = CREATE_CSTRING("DESCRIPTION:");
   nargs=0;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget, text_nome_blocco); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,20); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   label_descr_blocco = XmCreateLabel(rc_blocco,"Finestra_blocchi",args,nargs);
   XtManageChild (label_descr_blocco);
   XmStringFree(cstring);

/* TEXT  */
   nargs=0;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget, label_descr_blocco); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,10); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNeditable,True); nargs++;
   XtSetArg(args[nargs],XmNeditMode,XmSINGLE_LINE_EDIT); nargs++;
   XtSetArg(args[nargs],XmNmaxLength,50); nargs++;
   text_descr_blocco = XmCreateText (rc_blocco,"Text",args,nargs);
   XtManageChild (text_descr_blocco);

/* Separatore */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNbottomWidget, rc_blocco); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,15); nargs++;
   XtSetArg(args[nargs],XmNorientation,XmHORIZONTAL); nargs++;
   sep2 = XmCreateSeparatorGadget (dbx_nuovo_blocco,"Finestra_blocchi",
				   args,nargs);
   XtManageChild (sep2);

/* Creazione della scrolled window delle icone */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget, label_scroll_icone); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNbottomWidget, sep2); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,10); nargs++;
   XtSetArg(args[nargs],XmNheight,300); nargs++;
   XtSetArg(args[nargs],XmNscrollBarDisplayPolicy,XmAS_NEEDED); nargs++;
   XtSetArg(args[nargs],XmNscrollingPolicy,XmAUTOMATIC); nargs++;
   XtSetArg(args[nargs],XmNvisualPolicy,XmCONSTANT); nargs++;
   XtSetArg(args[nargs],XmNscrollBarPlacement,XmBOTTOM_RIGHT); nargs++;
   scroll_icone = XmCreateScrolledWindow
		      (dbx_nuovo_blocco,"Finestra_blocchi",args,nargs);
   XtManageChild (scroll_icone);

}

/*------------------------------------------------------------------*/
/*** void dialog_modify_block()
 *** Parametri:
 ***    Niuno.
Crea la finestra di modifica dell'icona del blocco ***/
void dialog_modify_block()
{
   Widget label_scroll_icone;
   Widget rc_blocco;
   Widget label_nome_blocco;
   Widget label_descr_blocco;
   Widget ok_button, cancel_button;
   Widget sep1, sep2;

   extern Widget dbx_modify_block, text_mod_nome_blk, text_mod_descr_blk;

/* Creazione della finestra di inserimento nome nuovo blocco (FORM) */
   cstring = CREATE_CSTRING("MODIFY BLOCK");
   nargs=0;
   XtSetArg(args[nargs],XmNdefaultPosition,False); nargs++;
   XtSetArg(args[nargs],XmNx,500); nargs++;
   XtSetArg(args[nargs],XmNy,180); nargs++;
   XtSetArg(args[nargs],XmNheight,300); nargs++;
   XtSetArg(args[nargs],XmNwidth,600); nargs++;
   XtSetArg(args[nargs],XmNautoUnmanage,True); nargs++;
   XtSetArg(args[nargs],XmNnoResize,False); nargs++;
   XtSetArg(args[nargs],XmNdialogTitle, cstring); nargs++;
   XtSetArg(args[nargs], XmNverticalSpacing, 10); nargs++;
   XtSetArg(args[nargs], XmNhorizontalSpacing, 10); nargs++;
   XtSetArg(args[nargs], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); nargs++;
   dbx_modify_block = XmCreateFormDialog (top_level,"Finestra_blocchi",
					  args,nargs);
   XmStringFree(cstring);

/* Label della scrolled window delle icone */
   cstring = CREATE_CSTRING("ICONS OF THE MODULE");
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,15); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   label_scroll_icone = XmCreateLabel (dbx_modify_block,"Finestra_blocchi",
				       args,nargs);
   XtManageChild (label_scroll_icone);
   XmStringFree(cstring);

/* button 'OK' */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,15); nargs++;
   XtSetArg(args[nargs],XmNlabelString,CREATE_CSTRING("Ok")); nargs++;
   ok_button = XmCreatePushButton (dbx_modify_block,"Finestra_blocchi",
		 	           args,nargs);
   XtAddCallback (ok_button,XmNactivateCallback,ok_proc, (XtPointer)DBOX_MODIFY_BLOCK);
   XtManageChild (ok_button);

/* button 'Cancel' */
   nargs=0;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,15); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget,ok_button); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,20); nargs++;
   XtSetArg(args[nargs],XmNlabelString,CREATE_CSTRING("Cancel")); nargs++;
   cancel_button = XmCreatePushButton (dbx_modify_block,"Finestra_blocchi",
				       args,nargs);
   XtAddCallback (cancel_button,XmNactivateCallback,cancel_proc, 
		  dbx_modify_block);
   XtManageChild (cancel_button);

/* Separatore */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNbottomWidget, ok_button); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,15); nargs++;
   XtSetArg(args[nargs],XmNorientation,XmHORIZONTAL); nargs++;
   sep2 = XmCreateSeparatorGadget (dbx_modify_block,"Finestra_blocchi",
				   args,nargs);
   XtManageChild (sep2);

/** ULTERIORE FORM per i dati del nuovo blocco **/
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNbottomWidget, sep2); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,15); nargs++;
   rc_blocco = XmCreateForm (dbx_modify_block,"Finestra_blocchi",args,nargs);
   XtManageChild (rc_blocco);

/* Nome del nuovo blocco */
/* LABEL */
   cstring = CREATE_CSTRING("BLOCK NAME:");
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopOffset, 10); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   label_nome_blocco = XmCreateLabel(rc_blocco,"Finestra_blocchi",args,nargs);
   XtManageChild (label_nome_blocco);
   XmStringFree(cstring);

/* TEXT  */
   nargs=0;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget, label_nome_blocco); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,10); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg(args[nargs],XmNrightPosition, 40); nargs++;
   XtSetArg(args[nargs],XmNeditable,True); nargs++; 
   XtSetArg(args[nargs],XmNeditMode,XmSINGLE_LINE_EDIT); nargs++;
   XtSetArg(args[nargs],XmNmaxLength,4); nargs++;
/*   XtSetArg(args[nargs],XmNbackground,1); nargs++; */
   text_mod_nome_blk = XmCreateText (rc_blocco,"Text",args,nargs);
   XtManageChild (text_mod_nome_blk);

/* Descrizione del nuovo blocco */
/* LABEL */
   cstring = CREATE_CSTRING("DESCRIPTION:");
   nargs=0;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopOffset, 10); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget, text_mod_nome_blk); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,20); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   label_descr_blocco = XmCreateLabel(rc_blocco,"Finestra_blocchi",args,nargs);
   XtManageChild (label_descr_blocco);
   XmStringFree(cstring);

/* TEXT  */
   nargs=0;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNleftWidget, label_descr_blocco); nargs++;
   XtSetArg(args[nargs],XmNleftOffset,10); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNeditable,True); nargs++; 
   XtSetArg(args[nargs],XmNeditMode,XmSINGLE_LINE_EDIT); nargs++;
   XtSetArg(args[nargs],XmNmaxLength,50); nargs++;
/*   XtSetArg(args[nargs],XmNbackground,1); nargs++; */
   text_mod_descr_blk = XmCreateText (rc_blocco,"Text",args,nargs);
   XtManageChild (text_mod_descr_blk);

/* Separatore */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNbottomWidget, rc_blocco); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,15); nargs++;
   XtSetArg(args[nargs],XmNorientation,XmHORIZONTAL); nargs++;
   sep1 = XmCreateSeparatorGadget (dbx_modify_block,"Finestra_blocchi",
				   args,nargs);
   XtManageChild (sep1);

/* Creazione della scrolled window delle icone */
   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNrightAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget, label_scroll_icone); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,10); nargs++;
   XtSetArg(args[nargs],XmNbottomAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNbottomWidget, sep1); nargs++;
   XtSetArg(args[nargs],XmNbottomOffset,10); nargs++;
   XtSetArg(args[nargs],XmNheight,300); nargs++;
   XtSetArg(args[nargs],XmNscrollBarDisplayPolicy,XmAS_NEEDED); nargs++;
   XtSetArg(args[nargs],XmNscrollingPolicy,XmAUTOMATIC); nargs++;
   XtSetArg(args[nargs],XmNvisualPolicy,XmCONSTANT); nargs++;
   XtSetArg(args[nargs],XmNscrollBarPlacement,XmBOTTOM_RIGHT); nargs++;
   scroll_icone2 = XmCreateScrolledWindow (dbx_modify_block,"Finestra_blocchi",
					  args,nargs);
   XtManageChild (scroll_icone2);
}

/*------------------------------------------------------------------*/
/*** void carica_lista_moduli(tipo)
 ***   Parametri:
 ***     short int tipo : indica se la lista deve essere di moduli di processo o di
 ***                      regolazione )
 *** Visualizza la lista dei moduli di processo o di regolazione (file icon_list.dat o
 *** lista_schemi.dat) nella scrolled list appropriata.
 ***/
void carica_lista_moduli(tipo)
short int tipo;
{
   XmString *cstrings;
   char temp[100];
   int i, num;
   PixmapModuleStruct *pixmod;

   extern PixmapModuleStruct *pixmap_module, *pixmap_mod_schema;
   extern int num_moduli, num_mod_schema;

   if ( tipo == TP_BLOCK )
   {
      num = num_moduli;
      pixmod = pixmap_module;
   }
   else
   {
      num = num_mod_schema;
      pixmod = pixmap_mod_schema;
   }

   cstrings = (XmString *) calloc_mem(num, sizeof(XmString));

/* recupera la lista dei moduli */
   for( i=0 ; i<num ; i++ )
   {
       sprintf(temp, "%s     %2d   %.30s",
               pixmod[i].nome_modulo, pixmod[i].num_icone, pixmod[i].descr_modulo);
       cstrings[i] = CREATE_CSTRING(temp);
   }

/* Aggiorna la scrolled list con la lista dei moduli */
   nargs = 0;
   XtSetArg(args[nargs], XmNitemCount, num); nargs++;
   XtSetArg(args[nargs], XmNitems, cstrings ); nargs++;
   XtSetValues (lista_moduli,args,nargs);

   
   if( cstrings != NULL)
   {
   for( i=0 ; i<num_moduli ; i++ )
      XmStringFree(cstrings[i]);
   XtFree((char*)cstrings);
   }
}

/*------------------------------------------------------------------*/
/*** carica_icone_modulo(nome_modulo)
 *** Parametri:
 ***     char *nome_modulo;
 *** Visualizza le icone del modulo selezionato nella lista-moduli.
 ***/
void carica_icone_modulo(nome_modulo, flag)
char *nome_modulo;
short int flag;
{
   int xx = 10, yy = 10, i, j;
   Widget scroll_ico, *tav_ico;
   PixmapModuleStruct *pixm_module;

   extern Widget tavola_icone, tavola_icone2;
   extern short int tipo_modulo;

/* Assegna le variabili globali appropriate nei casi in cui */
/* si aggiunge un blocco oppure lo si modifica */
   if (flag == FROM_ADD_BLOCK)
   {
      scroll_ico = scroll_icone;
      tav_ico = &tavola_icone;
   }
   else
   {
      scroll_ico = scroll_icone2;
      tav_ico = &tavola_icone2;
   }

/* recupera i dati delle icone */
   if ((pixm_module = ricerca_modulo(nome_modulo,tipo_modulo)) == NULL)
      return;

   if ( *tav_ico != NULL )
      XtDestroyWidget(*tav_ico);

   nargs = 0;
   XtSetArg(args[nargs], XmNx, 0); nargs++;
   XtSetArg(args[nargs], XmNy, 0); nargs++;
   XtSetArg(args[nargs], XmNwidth, 680); nargs++;
   XtSetArg(args[nargs], XmNheight,350); nargs++;
   XtSetArg(args[nargs], XmNmarginWidth, 0); nargs++;
   XtSetArg(args[nargs], XmNmarginHeight,0); nargs++;
   XtSetArg(args[nargs], XmNnoResize,True); nargs++;
   XtSetArg(args[nargs], XmNresizePolicy, XmRESIZE_ANY); nargs++;
   XtSetArg(args[nargs], XmNuserData, tipo_modulo); nargs++;
   *tav_ico = XmCreateBulletinBoard (scroll_ico,"Finestra_blocchi",args,nargs);
   XtManageChild (*tav_ico);

   for ( i = 0 ; i < pixm_module->num_icone ; i++)
   {
      crea_icona( *tav_ico, &pixm_module->pixmap_info[i], nome_modulo,
		  xx, yy, flag, 0, 0 );
      xx += pixm_module->pixmap_info[i].width + 20;
   }
}

/*-----------------------------------------------------------------------*/
/*** crea_remark( wdest, commento, posx, posy, ind_macro, ind_blk )
 *** Parametri :
 ***    Widget wdest: widget su cui disegnare l'icona
 ***    char *commento : stringa del commento
 ***    int posx, posy : posizione del commento.
 ***  int ind_macro: indice in macroblocks[].
 ***  int ind_blk : indice del blocco.
crea e visualizza la label del commento (REMARK). */
void crea_remark( wdest, commento, posx, posy, ind_macro, ind_blk )
Widget wdest;
char *commento;
int posx, posy;
int ind_macro;
int ind_blk;
{
   Widget label;
   MacroBlockType *ptr_macro;
   char stringa[80];

   ptr_macro = &macroblocks[ind_macro];

   nargs=0;
   cstring = CREATE_CSTRING(commento);
   XtSetArg(args[nargs],XmNx, posx); nargs++;
   XtSetArg(args[nargs],XmNy, posy); nargs++;
   XtSetArg(args[nargs],XmNlabelString, cstring); nargs++;
   XtSetArg(args[nargs],XmNbackground,apix[ BLOCKS_TABLE_BG ]); nargs++;
   XtSetArg(args[nargs],XmNborderColor,apix[ BLOCKS_TABLE_BG]); nargs++;
   XtSetArg(args[nargs],XmNborderWidth,1); nargs++;
   XtSetArg(args[nargs],XmNuserData, &ptr_macro->blocks[ind_blk]); nargs++;
   label = XmCreateLabel ( wdest, "Remark_label",args,nargs);
   XtManageChild(label);
   XmStringFree(cstring);

/* aggiorna la struttura del blocco */
   ptr_macro->blocks[ind_blk].wblock  = label;
   ptr_macro->blocks[ind_blk].wpixmap = label;
   ptr_macro->blocks[ind_blk].wlabel  = label;
   ptr_macro->blocks[ind_blk].pixmap_info = NULL;
   strcpy(ptr_macro->blocks[ind_blk].descr_blocco,commento);

   def_handler_block(ind_macro, ind_blk);
   def_translations_block(ind_macro, ind_blk);
}

/*-----------------------------------------------------------------------*/
/*** crea_icona(wdest,pixmap_info,nome_modulo,x,y,flag,ptr_macro,ind_blk)
 *** Parametri :
 *** Widget wdest: widget su cui disegnare l'icona
 *** PixmapInfo pixmap_info: informazioni sulla pixmap del modulo;
 ***    char nome_modulo : nome del modulo (stringa della label dell'icona)
 ***    int x, y: posizione nella tavola delle icone
 *** Boolean flag : indica se l'icona deve essere "sensibile" al mouse
 ***                per il caricamento nella window dei blocchi.
 *** MacroBlockType *ptr_macro: puntatore al macroblocco. Ha significato solo
 ***				se flag e' False
 *** int ind_blk : indice del blocco nell'array blocks puntato da *ptr_macro.
 ***               Ha significato se flag e' False
 ***                   
crea e visualizza l'icona nella tavola di selezione. */
int crea_icona( Widget wdest, PixmapInfo *pixmap_info, char *nome_modulo, int x, int y, short int flag ,int ind_macro,int ind_blk )
{
   Widget form_icona, icon_pixmap, icon_label;
   int  base = 0, altezza = 0;
   Pixmap pixm;
   XWindowAttributes xwa;
   long event_mask;
   MacroBlockType *ptr_macro;
   char stringa[80];

   if (pixmap_info != NULL)
   {
      base = pixmap_info->width;
      altezza = pixmap_info->height;
      pixm = pixmap_info->pixmap;
   }
   else
   {
      base = 50;
      altezza = 50;
      pixm = 0;
   }

   base = (base <= 0) ? 50 : base;
   altezza = (altezza <= 0) ? 50 : altezza;

/* Crea gli widget relativi alla icona */
   nargs=0;
   XtSetArg(args[nargs],XmNresizePolicy,XmRESIZE_ANY); nargs++;
/*
   XtSetArg(args[nargs],XmNnoResize,False); nargs++;
*/
   XtSetArg(args[nargs],XmNmarginWidth,0); nargs++;
   XtSetArg(args[nargs],XmNmarginHeight,0); nargs++;
   XtSetArg(args[nargs],XmNx,x); nargs++;
   XtSetArg(args[nargs],XmNy,y); nargs++;
   XtSetArg(args[nargs],XmNwidth,base); nargs++;
/*
   XtSetArg(args[nargs],XmNheight,altezza + ICON_LABEL_HEIGHT); nargs++;
*/
   XtSetArg(args[nargs],XmNborderColor,apix[ BLOCKS_TABLE_BG ]); nargs++;
   XtSetArg(args[nargs],XmNborderWidth, 1); nargs++;
   form_icona = XmCreateBulletinBoard(wdest,"Form",args,nargs);
   XtManageChild ( form_icona );

/* Creazione label nome-blocco e pixmap */
   nargs=0;
   cstring = CREATE_CSTRING(nome_modulo);
/*
   XtSetArg(args[nargs],XmNrecomputeSize,False); nargs++;
*/
   XtSetArg(args[nargs],XmNx,0); nargs++;
   XtSetArg(args[nargs],XmNy,altezza); nargs++;
   XtSetArg(args[nargs],XmNwidth,base); nargs++;
   XtSetArg(args[nargs],XmNmarginWidth,0); nargs++;
   XtSetArg(args[nargs],XmNmarginHeight,0); nargs++;
#ifdef PROVA
   if ( nome_modulo == NULL )
      XtSetArg(args[nargs],XmNheight,ICON_LABEL_HEIGHT);
   else  /* si tratta di simboli ... quindi niente label! */
      XtSetArg(args[nargs],XmNheight,0);
   nargs++;
#endif
   XtSetArg(args[nargs],XmNlabelString,cstring); nargs++;
   XtSetArg(args[nargs],XmNbackground,apix[ WHITE ]); nargs++;
   XtSetArg(args[nargs],XmNbackground,apix[ BLOCKS_TABLE_BG ]); nargs++;
   icon_label = XmCreateLabel (form_icona ,"icon_label",args,nargs);
   XtManageChild(icon_label);
   XmStringFree(cstring);

   nargs=0;
   XtSetArg(args[nargs],XmNlabelType,XmPIXMAP); nargs++;
   XtSetArg(args[nargs],XmNrecomputeSize,True); nargs++;
   XtSetArg(args[nargs],XmNmarginWidth,0); nargs++;
   XtSetArg(args[nargs],XmNmarginHeight,0); nargs++;
   if (pixm == NULL)
      XtSetArg(args[nargs],XmNlabelPixmap,XmUNSPECIFIED_PIXMAP);
   else
      XtSetArg(args[nargs],XmNlabelPixmap,pixm);
   nargs++;
   XtSetArg(args[nargs],XmNx,0); nargs++;
   XtSetArg(args[nargs],XmNy,0); nargs++;
   XtSetArg(args[nargs],XmNwidth,base); nargs++;
   XtSetArg(args[nargs],XmNheight,altezza); nargs++;
   XtSetArg(args[nargs],XmNbackground,apix[ BLOCKS_TABLE_BG ]); nargs++;
   icon_pixmap = XmCreateLabel (form_icona ,"Label",args,nargs);
   XtManageChild(icon_pixmap);

   switch (flag)
   {
      case FROM_ADD_BLOCK:
        /* in questo modo, l'icona riceve l'evento ButtonPress per essere */
        /* trasferita nella window dei blocchi */

           XtAddEventHandler( icon_pixmap, ButtonPressMask, False,
                        icon_press, pixmap_info );
           XGetWindowAttributes(display, XtWindow(icon_pixmap), &xwa );
           event_mask = xwa.your_event_mask | OwnerGrabButtonMask |
                        ButtonReleaseMask | ButtonMotionMask;
           XSelectInput(display, XtWindow(icon_pixmap), event_mask);
           break;

      case FROM_WINDOW_BLOCK:
   /* In questo caso l'icona viene creata nella window dei blocchi. */
   /* ( l'utente ha selezionato un macroblocco ed eseguito la OPEN ) */
   /* L'icona e' , comunque, sensibile agli eventi del mouse */
   /* ma solo per effettuare spostamenti all'interno della window dei */
   /* blocchi oppure per attivare un pornaz-menu */

           ptr_macro = &macroblocks[ind_macro];

           set_something_val(icon_pixmap,XmNuserData, (XtArgVal) &ptr_macro->blocks[ind_blk]);

        /* aggiorna la struttura del blocco */
           ptr_macro->blocks[ind_blk].wblock  = form_icona;
           ptr_macro->blocks[ind_blk].wlabel  = icon_label;
           ptr_macro->blocks[ind_blk].wpixmap = icon_pixmap;
           ptr_macro->blocks[ind_blk].pixmap_info  = pixmap_info;

           def_handler_block(ind_macro, ind_blk);
           def_translations_block(ind_macro, ind_blk);
           break;

      case FROM_MODIFY_BLOCK:
           XtAddEventHandler( icon_pixmap, ButtonPressMask, False,
                              seleziona_icona, pixmap_info );
           break;
   }
   return(0);
}

/*** cambia_pixmap_icona(ptr_block, new_pixmap)
 ***  Parametri:
 ***    BlockType *ptr_block: puntatore alla struttura blocco
 ***    PixmapInfo *new_pixmap: puntatore alla struttura pixmap.
Modifica la pixmap dell'icona di un blocco. ***/
void cambia_pixmap_icona(ptr_block, new_pixmap)
BlockType *ptr_block;
PixmapInfo *new_pixmap;
{
   Dimension label_height;

   if (ptr_block->wlabel == NULL)
      return;
      
   if ( new_pixmap != NULL)
   {
   /* recupera l'altezza della label del nome del blocco */
      get_something(ptr_block->wlabel, XmNheight, (void*) &label_height);

      nargs = 0;
      XtSetArg(args[nargs],XmNwidth, new_pixmap->width); nargs++;
      XtSetArg(args[nargs],XmNheight,new_pixmap->height+ label_height);
      nargs++;
      XtSetValues(ptr_block->wblock, args, nargs);

      nargs = 0;
      XtSetArg(args[nargs],XmNx, 0); nargs++;
      XtSetArg(args[nargs],XmNy, 0); nargs++;
      XtSetArg(args[nargs],XmNlabelPixmap, new_pixmap->pixmap); nargs++;
      XtSetArg(args[nargs],XmNwidth, new_pixmap->width); nargs++;
      XtSetArg(args[nargs], XmNheight,new_pixmap->height); nargs++;
      XtSetArg(args[nargs],XmNmarginWidth, 0); nargs++;
      XtSetArg(args[nargs],XmNmarginHeight, 0); nargs++;
      XtSetValues(ptr_block->wpixmap, args, nargs);

      nargs = 0;
      XtSetArg(args[nargs], XmNx, 0); nargs++;
      XtSetArg(args[nargs], XmNy, new_pixmap->height); nargs++;
      XtSetArg(args[nargs], XmNwidth, new_pixmap->width); nargs++;
      XtSetArg(args[nargs],XmNmarginWidth, 0); nargs++;
      XtSetArg(args[nargs],XmNmarginHeight, 0); nargs++;
      XtSetValues(ptr_block->wlabel, args, nargs);
      ptr_block->num_icona = new_pixmap->num_icona;
      ptr_block->pixmap_info = new_pixmap;
   }
}

/*** void seleziona_icona(w, client_data, ev, boh)
 *** Parametri:
 ***    Widget w: widget in cui e' avvenuto l'evento
 *** caddr_t client_data: non utilizzato
 ***   XEvent *ev: info evento
 *** boh : evento non-mascherabile (vero/falso)
L'utente ha selezionato una icona nella window di modifica del blocco */
void seleziona_icona(w, pixm_info, ev, boh)
Widget w;
PixmapInfo *pixm_info;
XEvent *ev;
Boolean boh;
{
    extern Widget wdg_ico_selez;
    extern PixmapInfo *pixm_ico_selez;

    if (wdg_ico_selez != NULL)
       set_something_val (XtParent(wdg_ico_selez), XmNborderColor,
		      (char*)apix[BLOCKS_TABLE_BG]);
    set_something_val( XtParent(w), XmNborderColor, (XtArgVal) apix[ RED ]);
    wdg_ico_selez = w;
    pixm_ico_selez = pixm_info;
}

/*** void icon_press(w, client_data, ev, boh)
 *** Parametri:
 ***    Widget w: widget in cui e' avvenuto l'evento
 *** caddr_t client_data: non utilizzato
 ***   XEvent *ev: info evento
 *** boh : evento non-mascherabile (vero/falso)
L'utente ha selezionato una icona da inserire nelle window dei macroblocchi */
void icon_press(w, pixmap_info, ev, boh)
Widget w;
PixmapInfo *pixmap_info;
XEvent *ev;
Boolean boh;
{
   extern int Iset_parico();

   MacroBlockType *pointer_macro;
   XEvent ev2;
   Widget wdg;
   char *nome_blocco, label_blocco[9], nome_modulo[5],*descriz_block;
   Boolean fine = False;
   int j, tipo, ind_macro;

   extern char nome_modulo_selez[];
   extern unsigned short xoffs, yoffs;
   extern DimWidgets *dim_widgets;
   extern int num_dim_wdg;

   get_something( XtParent(XtParent(w)), XmNuserData, (void*) &tipo );

   if ( tipo == TP_BLOCK || tipo == TP_BLOCK_REG)
   {
     nome_blocco = XmTextGetString( text_nome_blocco );
  /* il nome deve essere maiuscolo */
     tomaius(nome_blocco);

  /* controlla che il nome del blocco sia stato definito. */
     if ( nome_blocco == NULL || strlen(nome_blocco) == 0)
     {
        s_warning( top_level, &geom_attention, APPLICATION_NAME, 
                   warning_mesg, WNOBLKNAME );
        return;
      }   


      descriz_block = XmTextGetString( text_descr_blocco );

      if(strlen(nome_blocco) < 4)
         strncat(nome_blocco,"    ",4-strlen(nome_blocco) );

#ifdef TOPOLOGIA

      /* controllo univocita del nome blocco */

      if(!Iverifica_nome_blocco(nome_blocco)) /* non deve essere duplicato */
      {
         s_warning( top_level, &geom_attention, APPLICATION_NAME,warning_mesg, WDUPBLKNAME );
         XtFree(nome_blocco);
         XtFree(descriz_block);
         return;
      }

#endif

   /* nome del blocco da istanziare */
      strcpy(nome_modulo, nome_modulo_selez);
      sprintf(label_blocco, "%.4s%.4s",nome_modulo,nome_blocco);
#ifndef TOPOLOGIA
      XtFree(nome_blocco);
      XtFree(descriz_block);
#else
      strcpy(blockname,nome_blocco);
      strcpy(modulename,nome_modulo);
      strcpy(blockdescr,descriz_block);
      for(j=0;j<MAX_MODULI;j++)
      {
         if(!strcmp(modulename,moduli[j].sigla))
            num_modulo = j+1;
      }
#endif
   }
   else  /* Altrimenti e' di tipo simbolo ... */
   {
      strcpy(label_blocco,"");
      strcpy(nome_modulo,"");
   }
     
   if ( pixmap_info == NULL )
      return;

/* Attiva il rubber-band */
   num_dim_wdg = 1;
   dim_widgets = (DimWidgets *) calloc_mem(1, sizeof(DimWidgets));
   get_something( w, XmNwidth, (void*) &dim_widgets[0].width );
   get_something( w, XmNheight, (void*) &dim_widgets[0].height );
   get_something( w, XmNx, (void*) &dim_widgets[0].x );
   get_something( w, XmNy, (void*) &dim_widgets[0].y );
   
   start_rubberband( ev, RootWindow(display, screen), dim_widgets,
		     num_dim_wdg, True );

/* Loop sugli event ButtonMotionMask e ButtonReleaseMask terminando */
/* con quest'ultimo evento */
   while ( !fine )
   {
      XNextEvent( display, &ev2 );
      switch ( ev2.type ) {
         case MotionNotify:
              continue_rubberband( &ev2, RootWindow(display, screen),
				   dim_widgets, num_dim_wdg, True );
              break;

         case ButtonRelease:
              end_rubberband( &ev2, RootWindow(display, screen), dim_widgets,
	           	        num_dim_wdg, True, BB_WIDTH, BB_HEIGHT );
              XtFree(dim_widgets);
              num_dim_wdg = 0;
              fine = True;
              break;

	case Expose:
	      XtDispatchEvent(&ev2);
	      break;

         default : 
              break;
      }
   }
/* L'utente ha rilasciato il bottone del mouse. */
/* Adesso bisogna verificare se il puntatore del mouse e' nella window */
/* dei blocchi */

/* recupero il widget della window in cui e' stato rilasciato il bottone */
   if ( (wdg = XtWindowToWidget( display, ev2.xbutton.window)) == NULL)
      return;

   get_something( wdg, XmNuserData, (void*) &pointer_macro );

   if ( pointer_macro != NULL &&
        pointer_macro >= macroblocks && 
        pointer_macro <= &macroblocks[num_macro] &&
        wdg == pointer_macro->wtavblock)  /* ok! il puntatore e' nella */
   {	  				   /* window corretta */
      int xx, yy, k;
      int *ind = &pointer_macro->num_blocchi;
      int *num_blk_alloc = &pointer_macro->num_blocchi_alloc;
      char *blockname;

#ifdef TOPOLOGIA

   /* controllo univocita del nome blocco */

   if(tipo == TP_BLOCK || tipo == TP_BLOCK_REG)
   {
      if(!Iverifica_nome_blocco(nome_blocco)) /* non deve essere duplicato */
      {
         s_warning( top_level, &geom_attention, APPLICATION_NAME,warning_mesg, WDUPBLKNAME );
         XtFree(nome_blocco);
         XtFree(descriz_block);
         return;
      }
   }

   if(tipo == TP_BLOCK || tipo == TP_BLOCK_REG)
   {
      XtFree(nome_blocco);
      XtFree(descriz_block);
   } 
   
   /* adesso devo effettuare le verifica che non venga superato il numero
      massimo di blocchi allocati */

   if(pointer_macro->num_blocchi == pointer_macro->num_blocchi_alloc  )
   {
         s_warning( top_level, &geom_attention, APPLICATION_NAME,
                    warning_mesg, WMAXBLKMODEL );
         return;
   }

/* inizio nuova revisione 6-3-95 */

   /* recupero  l'indice del macroblocco. L'operazione viene effettuata */
   /* sugli indirizzi di memoria */
      ind_macro = ((int)((long)pointer_macro-(long)macroblocks))/
						   sizeof(MacroBlockType);

      modifiche = True; /* Setta il flag di modifica ! */

   /* controlla la posizione x,y dell'icona */
      xx = ev2.xbutton.x - xoffs;
      yy = ev2.xbutton.y - yoffs;
      if ( xx < 0 )
         xx = 0;
      else
         if ( xx + pixmap_info->width > BB_WIDTH )
            xx = BB_WIDTH - pixmap_info->width - 1;

      if ( yy < 0 )
         yy = 0;
      else
         if ( yy + pixmap_info->height > BB_HEIGHT )
            yy = BB_HEIGHT - pixmap_info->height - 1;

/* Aggiustamento della posizione in relazione alla griglia */
      xx = ((int) xx/snap_val)*snap_val;
      yy = yy - yy%snap_val;

      Iset_parico(pointer_macro, xx, yy, tipo, False,
			pixmap_info, label_blocco, 
			nome_modulo, wdg, ind_macro );

   if(tipo == TP_BLOCK)
   {
      do_dialogo();
      if(Iverifica_errore() == 1 )
         return;
   }
   else if (tipo == TP_BLOCK_REG)
   {
      add_block(REGOLAZIONE);
   }
   else /* symobolo */
   {
   /* Aggiorna la struttura blocks del macroblocco relativo */
      ind = alloca_nuovo_blocco(pointer_macro, xx, yy, tipo, False,
			pixmap_info, label_blocco, "", /* <- descr.blk */
			nome_modulo );

      crea_icona( wdg, pixmap_info, label_blocco, xx, yy, 
		  FROM_WINDOW_BLOCK, ind_macro, ind );
   
   }
    
#endif

#ifndef TOPOLOGIA
/* nuova revisione 6-3-95 */

   /* recupero  l'indice del macroblocco. L'operazione viene effettuata */
   /* sugli indirizzi di memoria */
      ind_macro = ((int)((long)pointer_macro-(long)macroblocks))/
						   sizeof(MacroBlockType);

      modifiche = True; /* Setta il flag di modifica ! */

   /* controlla la posizione x,y dell'icona */
      xx = ev2.xbutton.x - xoffs;
      yy = ev2.xbutton.y - yoffs;
      if ( xx < 0 )
         xx = 0;
      else
         if ( xx + pixmap_info->width > BB_WIDTH )
            xx = BB_WIDTH - pixmap_info->width - 1;

      if ( yy < 0 )
         yy = 0;
      else
         if ( yy + pixmap_info->height > BB_HEIGHT )
            yy = BB_HEIGHT - pixmap_info->height - 1;

/* Aggiustamento della posizione in relazione alla griglia */
      xx = ((int) xx/snap_val)*snap_val;
      yy = yy - yy%snap_val;

   /* Aggiorna la struttura blocks del macroblocco relativo */
      ind = alloca_nuovo_blocco(pointer_macro, xx, yy, tipo, False,
			pixmap_info, label_blocco, "", /* <- descr.blk */
			nome_modulo );

      crea_icona( wdg, pixmap_info, label_blocco, xx, yy, 
		  FROM_WINDOW_BLOCK, ind_macro, ind );

/* fine revisione 6-3-95 */
#endif
   }
   else
      s_warning( top_level, &geom_attention, APPLICATION_NAME, 
                 warning_mesg, WWINWRONG );
}

/*-----------------------------------------------------------------------*/
/*** void crea_find(widget_sup)
 ***   Parametri:
 ***     Widget widget_sup : widget sopra il find (da appiccicare ...)
Crea i FIND di libreria per la scroll list dei moduli. La funzione
ritorna il widget della form del blocco di find. */
Widget crea_find(widget_sup)
Widget widget_sup;
{
   static Widget find_form;

   find_module.ID_lista = lista_moduli;
   find_module.num_elem = 100;
   find_module.vis_elem = VISIBLE_ITEMS;
   find_module.pos      = 0;   /* posizione corrente nel find */

   nargs=0;
   XtSetArg(args[nargs],XmNleftAttachment,XmATTACH_FORM); nargs++;
   XtSetArg(args[nargs],XmNtopAttachment,XmATTACH_WIDGET); nargs++;
   XtSetArg(args[nargs],XmNtopWidget,widget_sup); nargs++;
   XtSetArg(args[nargs],XmNtopOffset,5); nargs++;
   if (find_form == NULL)
       find_form = find_kit(dbx_nuovo_blocco,args,nargs,&find_module);

   set_something_val (find_module.ID_testo,XmNbackground, (XtArgVal) apix[WHITE]);
   return(find_form);
}

/*** cambia_edit_blk_menu(ptr)
 *** Parametri:
 ***    MacroBlockType *ptr : macroblocco associata alla window dei blocchi
attiva/disattiva le voci del menu edit della window dei blocchi */
void cambia_edit_blk_menu(ptr)
MacroBlockType *ptr;
{
   int i,tipo;

#ifdef DATI
   set_something_val(ptr->menu_edit_blocchi[K_VAR_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0) ? True : False );
   set_something_val(ptr->menu_edit_blocchi[K_DATA_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0) ? True : False );
   set_something_val(ptr->menu_edit_blocchi[K_KNOW_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0) ? True : False );
   set_something_val(ptr->menu_edit_blocchi[K_NORM_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0) ? True : False );
   set_something_val(ptr->pop_edit_blocchi[K_VAR_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0) ? True : False );
   set_something_val(ptr->pop_edit_blocchi[K_DATA_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0) ? True : False );
   set_something_val(ptr->pop_edit_blocchi[K_KNOW_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0) ? True : False );
   set_something_val(ptr->pop_edit_blocchi[K_NORM_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0) ? True : False );
#elif TOPOLOGIA

  if(ptr->num_blocchi_selez == 1)
    for(i=0;i<ptr->num_blocchi;i++)
        if(ptr->blocks[i].selezionato)
           tipo = ptr->blocks[i].tipo;
  

  set_something_val(ptr->pop_edit_blocchi[K_VARIABLES_BLOCK], XmNsensitive, (XtArgVal) ( (ptr->num_blocchi_selez == 1  && tipo == TP_BLOCK) ) ? True : False );
  set_something_val(ptr->pop_edit_blocchi[K_NEW_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez == 0) ? True : False );
  set_something_val(ptr->pop_edit_blocchi[K_NEW_PROCESS_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez == 0) ? True : False );
  set_something_val(ptr->pop_edit_blocchi[K_NEW_REGULATION_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez == 0) ? True : False );
   set_something_val(ptr->pop_edit_blocchi[K_MODIFY_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez == 1) ? True : False );
   set_something_val(ptr->pop_edit_blocchi[K_DELETE_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0 ) ? True : False );

  set_something_val(ptr->menu_edit_blocchi[K_VARIABLES_BLOCK], XmNsensitive, (XtArgVal) ((ptr->num_blocchi_selez == 1 && tipo == TP_BLOCK)) ? True : False );
  set_something_val(ptr->menu_edit_blocchi[K_NEW_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez == 0) ? True : False );
  set_something_val(ptr->menu_edit_blocchi[K_NEW_PROCESS_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez == 0) ? True : False );
  set_something_val(ptr->menu_edit_blocchi[K_NEW_REGULATION_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez == 0) ? True : False );
   set_something_val(ptr->menu_edit_blocchi[K_MODIFY_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez == 1) ? True : False );
   set_something_val(ptr->menu_edit_blocchi[K_DELETE_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0 ) ? True : False );
/******************************************************************************
   set_something_val(ptr->menu_edit_blocchi[K_MOVE_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0 ) ? True : False );
   set_something_val(ptr->pop_edit_blocchi[K_MODIFY_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez == 1) ? True : False );
   set_something_val(ptr->pop_edit_blocchi[K_DELETE_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0 ) ? True : False );
   set_something_val(ptr->pop_edit_blocchi[K_MOVE_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0 ) ? True : False );
*******************************************************************************/
#else
   set_something_val(ptr->menu_edit_blocchi[K_MODIFY_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez == 1) ? True : False );
   set_something_val(ptr->menu_edit_blocchi[K_DELETE_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0 ) ? True : False );
   set_something_val(ptr->menu_edit_blocchi[K_MOVE_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0 ) ? True : False );
   set_something_val(ptr->pop_edit_blocchi[K_MODIFY_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez == 1) ? True : False );
   set_something_val(ptr->pop_edit_blocchi[K_MOVE_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0 ) ? True : False );
   set_something_val(ptr->pop_edit_blocchi[K_DELETE_BLOCK], XmNsensitive, (XtArgVal) (ptr->num_blocchi_selez > 0 ) ? True : False );
#endif
}

/*** start_select(w, ind_macro, ev, boh)
 *** Parametri :
 ***   Widget w: Bulletin board dei blocchi
 ***   int ind_macro : indice del macroblocco
 ***   XEvent *ev : struttura eventi
 ***   Boolean boh: non utilizzato
Gestisce la selezione multipla dei blocchi con il tasto 2 del mouse. */
void start_select(w, ind_macro, ev, boh)
Widget w;
int ind_macro;
XEvent *ev;
Boolean boh;
{
    if (bottone_premuto != -1)
       return;

    bottone_premuto = ev->xbutton.button;
    mousex0 = ev->xbutton.x;
    mousey0 = ev->xbutton.y;
    mouseprecx = mousex0;
    mouseprecy = mousey0;
}

/*** continue_select(w, ind_macro, ev, boh)
 *** Parametri :
 ***   Widget w: Bulletin board dei blocchi
 ***   int ind_macro : indice del macroblocco
 ***   XEvent *ev : struttura eventi
 ***   Boolean boh: non utilizzato
Gestisce la selezione multipla dei blocchi con il tasto 2 del mouse. */
void continue_select(w, ind_macro, ev, boh)
Widget w;
int ind_macro;
XEvent *ev;
Boolean boh;
{
   int mousex, mousey;

   if (bottone_premuto == -1)
      return;

   mousex = (ev->xbutton.x < 0) ? 0 : ev->xbutton.x;
   mousey = (ev->xbutton.y < 0) ? 0 : ev->xbutton.y;

   if ( mousex0 != mouseprecx || mousey0 != mouseprecy )
      XDrawRectangle(display, XtWindow(w), xorGC, mousex0, mousey0,
                     mouseprecx - mousex0, mouseprecy - mousey0 );

   XDrawRectangle(display, XtWindow(w), xorGC, mousex0, mousey0,
                  mousex - mousex0,mousey - mousey0 );

   mouseprecx = mousex;
   mouseprecy = mousey;
}

/*** end_select(w, ind_macro, ev, boh)
 *** Parametri :
 ***   Widget w: Bulletin board dei blocchi
 ***   int ind_macro : indice del macroblocco
 ***   XEvent *ev : struttura eventi
 ***   Boolean boh: non utilizzato
Gestisce la selezione multipla dei blocchi. */
void end_select(w, ind_macro, ev, boh)
Widget w;
int ind_macro;
XEvent *ev;
Boolean boh;
{
   int maxx, maxy, minx, miny, i, mousex, mousey;
   BlockType *blocks;
   Dimension width, height;

   if (bottone_premuto == -1)
      return;

   mousex = (ev->xbutton.x < 0) ? 0 : ev->xbutton.x;
   mousey = (ev->xbutton.y < 0) ? 0 : ev->xbutton.y;

   if ( mousex0 != mouseprecx || mousey0 != mouseprecy )
      XDrawRectangle(display, XtWindow(w), xorGC, mousex0, mousey0,
                     mouseprecx - mousex0, mouseprecy - mousey0 );

   minx = ( mousex > mousex0 ) ? mousex0 : mousex;
   miny = ( mousey > mousey0 ) ? mousey0 : mousey;
   maxx = ( mousex < mousex0 ) ? mousex0 : mousex;
   maxy = ( mousey < mousey0 ) ? mousey0 : mousey;

   blocks = macroblocks[ind_macro].blocks;

/* Seleziona i blocchi compresi nel rettangolo */
   for ( i=0 ; i<macroblocks[ind_macro].num_blocchi ; i++ )
   {
      if (blocks[i].cancellato) /* i blocchi marcati cancellati non sono */
				/* visualizzati e non devono essere */
				/* considerati. */
         continue;

      get_something(blocks[i].wblock, XmNheight, (void*) &height);
      get_something(blocks[i].wblock, XmNwidth, (void*) &width);
      if ( blocks[i].pos_icona.posx >= minx &&
           blocks[i].pos_icona.posx + width < maxx &&
           blocks[i].pos_icona.posy >= miny &&
           blocks[i].pos_icona.posy + height < maxy )
	{
         seleziona_blocco(&macroblocks[ind_macro], &blocks[i], True);
#ifdef DATI
         seleziona_blocco_lista(  &macroblocks[ind_macro],
			    &macroblocks[ind_macro].blocks[i],True);
#endif
	}
   }
   bottone_premuto = -1;
}

/*------------------------------------------------------------------*/
/*** seleziona_blocco( ind_macro, ind_blocco, flag )
 ***   Parametri:
 ***     macro : struttura macroblocks[]
 ***     blocco: struttura blocks[] in macroblocks[]
 ***     flag : indica se bisogna selezionare o deselezionare
seleziona/deseleziona un blocco ***/
void seleziona_blocco( macro, blocco, flag )
MacroBlockType *macro;
BlockType *blocco;
Boolean flag;
{
   int i;
   extern Widget widget_list_blocchi;

/* se il blocco da selezionare e' gia' selezionato oppure il blocco da */
/* deselezionare e' gia' deselezionato allora esci dalla funzione */

   if ((flag && blocco->selezionato) || (!flag && !blocco->selezionato) ||
       blocco->cancellato)
      return;

   blocco->selezionato = flag; 

   if (flag)
   {
      macro->num_blocchi_selez++;
      if( blocco->wblock != NULL )
      {
         set_something_val (blocco->wblock, XmNborderColor, (XtArgVal) apix[ RED ]);
      }
#ifdef TOPOLOGIA
      if(blocco->tipo == TP_BLOCK || blocco->tipo == TP_BLOCK_REG)
         Iselect_this_block_from_list(widget_list_blocchi,blocco->nome_blocco);
#endif
   }
   else
   {
      macro->num_blocchi_selez--;
      if( blocco->wblock  != NULL )
         set_something_val (blocco->wblock, XmNborderColor, (XtArgVal) apix[BLOCKS_TABLE_BG]);
#ifdef TOPOLOGIA
      if(blocco->tipo == TP_BLOCK || blocco->tipo == TP_BLOCK_REG)
         Ideselect_this_block_from_list(widget_list_blocchi,blocco->nome_blocco);
#endif
   }

   if( blocco->wblock != NULL )
      cambia_edit_blk_menu(macro);  /* Abilita/disabilita voci menu pop-up */
#ifdef TOPOLOGIA
   if(blocco->tipo == TP_BLOCK || blocco->tipo == TP_BLOCK_REG)
      main_menu_setup();
#endif

}

/*------------------------------------------------------------------*/
/*** void def_translations_block(ind_macro,ind_block)
 ***  Parametri:
 ***      int ind_macro: indice in macroblocks[]
 ***      int ind_block: indice in macroblocks[ind_macro].blocks[]
definisce le translations relative ad un blocco */
void def_translations_block(ind_macro,ind_block)
int ind_macro, ind_block;
{
   char stringa[120];
   BlockType *block;

   block = &macroblocks[ind_macro].blocks[ind_block];

/* gestione della selezione */
   sprintf(stringa, STR_SELECT_BLOCK1, ind_macro, ind_block);
   add_translations( block->wpixmap, stringa );
   add_translations( block->wlabel, stringa );
   sprintf(stringa, STR_SELECT_BLOCK2, ind_macro, ind_block);
   add_translations( block->wpixmap, stringa );
   add_translations( block->wlabel, stringa );
}

/*------------------------------------------------------------------*/
/*** void def_handler_block(ind_macro,ind_block)
 ***  Parametri:
 ***      int ind_macro: indice in macroblocks[]
 ***      int ind_block: indice in macroblocks[ind_macro].blocks[]
definisce le translation e gli event-handler relativi ad un blocco */
void def_handler_block(ind_macro,ind_block)
int ind_macro, ind_block;
{
   BlockType *block;

   block = &macroblocks[ind_macro].blocks[ind_block];

/* gestione dello spostamento */
   XtAddEventHandler( block->wpixmap, ButtonPressMask, False,
                                 block_bpress, &macroblocks[ind_macro] );
#ifndef DATI
   XtAddEventHandler( block->wpixmap, ButtonMotionMask, False,
                                 block_bcont, &macroblocks[ind_macro] );
   XtAddEventHandler( block->wpixmap, ButtonReleaseMask, False,
                                 block_brelease, &macroblocks[ind_macro] );
#endif
}

#ifndef MAIN
#ifndef DATI
#include "lg1MainWindowType.h"
#endif
char *ricostruisci_nome_blocco();

/*** selegraf_blocco(nome_blocco, selezionato)
 *** Parametri:
 ***    char *nome_blocco : nome del blocco da selezionare/deselezionare
 ***    int selezionato : flag , indica se il blocco e' selezionato o meno
seleziona un blocco se la window dei blocchi relativa e' aperta */
selgraf_blocco(nome_blocco, selezionato)
char *nome_blocco;
int selezionato;
{
   int i, j;
   Boolean trovato=False;
   BlockType *blocks;

/*   trim_blank(nome_blocco); */
   for( i=0 ; i<num_macro ; i++ )
   {
/*
      if (macroblocks[i].blocks != NULL && macroblocks[i].wwinblock != NULL )
      {
*/
      if (macroblocks[i].blocks != NULL )
      {
         blocks = macroblocks[i].blocks;
         for( j=0 ; j<macroblocks[i].num_blocchi; j++ )
            if (blocks[j].tipo == TP_BLOCK || blocks[j].tipo == TP_BLOCK_REG)
               if ( !strcmp(blocks[j].nome_blocco,nome_blocco) )
               {
                  if( blocks[j].cancellato )
                     continue;
                  trovato=True;
                  seleziona_blocco( &macroblocks[i], &blocks[j], selezionato );
                  return;
               }
      }
   }
}

/*** display_icone_selezionate(j)
 *** Parametri:
 ***    ind_macro : indice in macroblocks[]
evidenzia le icone selezionate all'apertura della 
finestra dei blocchi */
display_icone_selezionate(ind_macro)
{
   extern Widget widget_list_blocchi;
   int num_blk_selez, i, j;
   BlockType *blocks;
   char *nome_blocco;

   if ( !XtIsManaged(widget_list_blocchi) )
      return;
               
   get_something(widget_list_blocchi, XmNselectedItemCount, (void*) &num_blk_selez);
   if ( !num_blk_selez )
      return;

   blocks = macroblocks[ind_macro].blocks;
   for( i=0 ; i<num_blk_selez ; i++ )  
   {
      for( j=0 ; j<macroblocks[ind_macro].num_blocchi; j++ )
         if ( blocks[j].tipo == TP_BLOCK && blocks[j].selezionato )
            set_something_val (blocks[j].wblock, XmNborderColor, (XtArgVal) apix[ RED ]);
   }

}


char * ricostruisci_nome_blocco(riga)
char* riga;
{
char appoggio[9];
#ifdef DATI

memcpy( appoggio,&riga[32], 4 );

memcpy( &appoggio[4] , &riga[14], 4 );

#else
/* copia in appoggio la sigla del modulo */
strncpy( appoggio, riga, 4 );
appoggio[4] = '\0';

strncat( appoggio , &riga[6], 4 );
appoggio[8] = '\0';
#endif
memcpy(riga,appoggio,8);
return(riga);
}

#endif

/* fine block_wdg.c */
