
/*******************************************************************************
	topVarWin1.c
	(Generated from interface file topVarWin1.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/CascadeB.h>
#include <Xm/ScrolledW.h>
#include <Xm/PanedW.h>
#include <Xm/Frame.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/MainW.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
   modulo topVarWin1.i
   tipo 
   release 1.25
   data 2/5/96
   reserved @(#)topVarWin1.i	1.25
*/
/*
        Variabile per identificazione della versione
*/
#ifndef DESIGN_TIME
#endif
/*
        Fine sezione per SCCS
*/

#include "lg1_topsim.h"
#include "varwin.h"
#include "filtri.h"

int gost_win(int, int);

extern SIMULATORE *simulatore;
VAR_SEL  move_var;
GOST_WIN gost_win_par;
int      gost_win_drag_on;

static int    highlight_on=False;
static Widget highlight_wid, del_wid;

extern GC gost_win_gc;
extern swidget create_msgToUser();

/*******************************************************************************
	The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxtopVarWin1MenuPost( wgt, client_data, event, ctd )
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
	Widget	UxtopVarWin1;
	Widget	UxvarForm1;
	Widget	UxvarTitleLb1;
	Widget	UxfilterLb1;
	Widget	UxdefFil1;
	Widget	UxfilterSel1;
	Widget	UxvarFrame1;
	Widget	UxpanedWindow3;
	Widget	UxingForm1;
	Widget	UxingLb1;
	Widget	UxingSW1;
	Widget	UxingList1;
	Widget	Uxmenu3;
	Widget	Uxmenu3_p1_b1;
	Widget	UxuscForm1;
	Widget	UxuscLb1;
	Widget	UxuscSW1;
	Widget	UxuscList1;
	Widget	Uxmenu5;
	Widget	Uxmenu5_p1_b1;
	Widget	Uxmenu4;
	Widget	Uxmenu4_p1;
	Widget	Uxmenu4_p1_b1;
	Widget	Uxmenu4_p1_b2;
	Widget	Uxmenu4_top_b1;
	VAR_SEL	Uxsel_var;
	int	Uxfiltro_attivo;
	FILTRI	*Uxfiltro_attuale;
	int	Uxblocco;
	MODELLO	*Uxlocmod;
	int	Uxindice_modello;
	int	Uxindice_blocco;
	int	Uxindice_variabile;
	Boolean	Uxshow_on;
	MODELLO	*Uxqtask;
	int	Uxind_modello;
	int	Uxind_blocco;
} _UxCtopVarWin1;

#define topVarWin1              UxTopVarWin1Context->UxtopVarWin1
#define varForm1                UxTopVarWin1Context->UxvarForm1
#define varTitleLb1             UxTopVarWin1Context->UxvarTitleLb1
#define filterLb1               UxTopVarWin1Context->UxfilterLb1
#define defFil1                 UxTopVarWin1Context->UxdefFil1
#define filterSel1              UxTopVarWin1Context->UxfilterSel1
#define varFrame1               UxTopVarWin1Context->UxvarFrame1
#define panedWindow3            UxTopVarWin1Context->UxpanedWindow3
#define ingForm1                UxTopVarWin1Context->UxingForm1
#define ingLb1                  UxTopVarWin1Context->UxingLb1
#define ingSW1                  UxTopVarWin1Context->UxingSW1
#define ingList1                UxTopVarWin1Context->UxingList1
#define menu3                   UxTopVarWin1Context->Uxmenu3
#define menu3_p1_b1             UxTopVarWin1Context->Uxmenu3_p1_b1
#define uscForm1                UxTopVarWin1Context->UxuscForm1
#define uscLb1                  UxTopVarWin1Context->UxuscLb1
#define uscSW1                  UxTopVarWin1Context->UxuscSW1
#define uscList1                UxTopVarWin1Context->UxuscList1
#define menu5                   UxTopVarWin1Context->Uxmenu5
#define menu5_p1_b1             UxTopVarWin1Context->Uxmenu5_p1_b1
#define menu4                   UxTopVarWin1Context->Uxmenu4
#define menu4_p1                UxTopVarWin1Context->Uxmenu4_p1
#define menu4_p1_b1             UxTopVarWin1Context->Uxmenu4_p1_b1
#define menu4_p1_b2             UxTopVarWin1Context->Uxmenu4_p1_b2
#define menu4_top_b1            UxTopVarWin1Context->Uxmenu4_top_b1
#define sel_var                 UxTopVarWin1Context->Uxsel_var
#define filtro_attivo           UxTopVarWin1Context->Uxfiltro_attivo
#define filtro_attuale          UxTopVarWin1Context->Uxfiltro_attuale
#define blocco                  UxTopVarWin1Context->Uxblocco
#define locmod                  UxTopVarWin1Context->Uxlocmod
#define indice_modello          UxTopVarWin1Context->Uxindice_modello
#define indice_blocco           UxTopVarWin1Context->Uxindice_blocco
#define indice_variabile        UxTopVarWin1Context->Uxindice_variabile
#define show_on                 UxTopVarWin1Context->Uxshow_on
#define qtask                   UxTopVarWin1Context->Uxqtask
#define ind_modello             UxTopVarWin1Context->Uxind_modello
#define ind_blocco              UxTopVarWin1Context->Uxind_blocco

static _UxCtopVarWin1	*UxTopVarWin1Context;

Widget	filterPane1;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_topVarWin1();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

void var_press();
int var_release();
int show_all_conn();
int no_show_all_connections();
int show_connections();
int no_show_connections(); 



/*-------------------------------------------
 *
 * cancella tutte le connessioni di un blocco
 *
 *-------------------------------------------*/ 

int block_delete_all_connections(BLOCCO bloc)
{
   extern Boolean simulator_changed;
   CONNMODEL *conn,*prec,*conn_ext;
   int i,j;   
   int var_int,mod_ext,bl_ext,var_ext;


   conn = bloc.var_conn_task;

   /* scorro tutte le connessioni di questo blocco  */
   while( conn != NULL)
   {

      /*
          per ogni connessione che trovo annullo la connessione
          nel blocco associato 
      */    
      var_int = conn->var_interna;
      mod_ext = conn->modello_esterno;
      bl_ext  = conn->blocco_esterno;
      var_ext = conn->var_esterna;  

      /* cerco la variabile connessa */
      conn_ext = simulatore->modelli[ mod_ext ]->blocchi[ bl_ext ].var_conn_task;
      prec = NULL;
      while( conn_ext != NULL )
      {

         if( conn_ext->var_esterna == var_int )
         {
            /* ho trovato la connessione corrispondente da eliminare */
            if( prec == NULL)
               simulatore->modelli[ mod_ext ]->blocchi[ bl_ext ].var_conn_task = conn_ext->succ;
            else
               prec->succ =  conn_ext->succ;        

            /* decremento le connessioni della variabile */

            if(simulatore->modelli[ mod_ext ]->blocchi[ bl_ext ].variabili[ var_ext].model_conn)
               simulatore->modelli[ mod_ext ]->blocchi[ bl_ext ].variabili[ var_ext].model_conn--;

         }  
         prec = conn_ext;
         conn_ext = conn_ext->succ;

         simulator_changed = TRUE;

      } /* end while */
     
      conn = conn->succ;
   } /* end while */
}
 
/*-------------------------------------------
 *
 * cancella tutte le connessioni di una task
 *
 *-------------------------------------------*/ 

int task_delete_all_connections(MODELLO *task)
{
   int i,j;


   for(i=0;i<task->num_blocchi;i++)
   {
      block_delete_all_connections( task->blocchi[i] );
      task->blocchi[i].var_conn_task=NULL;

      for(j=0;j< task->blocchi[i].num_variabili; j++)
         task->blocchi[i].variabili[j].model_conn = 0; 
   }

}


/*-----------------------------------------
 *
 * cancella le connessioni nella varWin
 * connessa
 * 
 ---------------------------------------*/ 

int cancella_var_conn(Widget wtop,CONNMODEL *connext)
{
   extern void getSecVarWin();
   Widget otherVarWin;
   char *salva_context1,*salva_context2;
   CONNMODEL *local_conn,*prec;
   VAR local_var;
   int i,j,k;
   Widget *wids,*wch;
   int numing,numusc;
   Arg arg[3];
   Cardinal narg;
   int tot,ning,nusc,irc;
   int blocco_vis;
   blocco_vis = False;

/* recupero lo id della altra varWin visualizzata */
   getSecVarWin(wtop,&otherVarWin);

   if( otherVarWin != NULL )
   {
       /* setto il context dell'altra varWin */
       set_var_win_list_context(otherVarWin,&salva_context1);

        /* verifico che sia relativa al blocco che contiene la connessione
           corrispondente */
       if( connext->blocco_esterno == blocco)
          blocco_vis = True;

       if( blocco_vis )
       {
          /* caso di connessione su ingresso */
          if(locmod->blocchi[connext->blocco_esterno].variabili[connext->var_esterna].tipo == INGRESSO )
          {
             /* recupero gli id dei frame degli ingressi
               ( i quali a loro volta contengono un row_col )
             */
             get_something(ingList1,XmNchildren,(void*) &wids);

             /* per determinare quale frame contiene la connessione applico
                la formula
                   irc =  nvaring - ( numtotvariabili - ind_variabile )

               questa vale sempre nella ipotesi che la varWin sia
               costruita a partire dalla lista della uscite
               quindi la prima uscita ha come indice variabile 0
             */

             /* ricavo il numero di uscite */
             for(i=0,nusc=0;
                 i<locmod->blocchi[connext->blocco_esterno].num_variabili;i++)
                if( locmod->blocchi[connext->blocco_esterno].variabili[i].tipo != INGRESSO )
                   nusc++;

             tot = locmod->blocchi[connext->blocco_esterno].num_variabili;
             ning = tot - nusc;
             irc =  ning - (tot- connext->var_esterna);

             /* recupero i row_col */
             get_something(wids[irc],XmNchildren,(void*) &wch);

             /* recupero ciascuna label interna all'item */
             get_something(wch[0],XmNchildren,(void*) &wids);

         } /* enf if == INGRESSO */
         else
         {

             /* recupero gli id dei frame degli ingressi
               ( i quali a loro volta contengono un row_col )
             */
             get_something(uscList1,XmNchildren,(void*) &wids);

             /* recupero il row_col */
             get_something(wids[connext->var_esterna],XmNchildren,(void*) &wch);

             /* recupero ciascuna label interna all'item */
             get_something(wch[0],XmNchildren,(void*) &wids);
         }

       } /* end if == blocco */
    } /* endif otherVarWin != NULL */

/* elimino la connessione */
    local_conn = simulatore->modelli[connext->modello_esterno]->blocchi[connext->blocco_esterno].var_conn_task;
    prec = NULL;

    /* cerco tale connessione */
    while( local_conn != NULL)
    {
       if( local_conn->var_interna == connext->var_esterna )
       {
          if(local_conn->var_esterna == connext->var_interna  )
          {
             /* ho trovato la connessione */

             /* elimino (ma non libero) la connessione */
             if( prec == NULL)
               simulatore->modelli[connext->modello_esterno]->blocchi[connext->blocco_esterno].var_conn_task = local_conn->succ;
             else
               prec->succ = local_conn->succ;

             /* decremento le connessioni della variabile*/
             simulatore->modelli[connext->modello_esterno]->blocchi[connext->blocco_esterno].variabili[connext->var_esterna].model_conn--;

          }
       }
       prec = local_conn;
       local_conn = local_conn->succ;
    } /* end while */

    if( blocco_vis )
    {
        /* se era la unica connessione elimino anche il pixmap */
        if( simulatore->modelli[connext->modello_esterno]->blocchi[connext->blocco_esterno].variabili[connext->var_esterna].model_conn < 1 )
            set_var_pixmap(wids[1],"void.bmp");

        /*devo eliminare l'item della connessione */
        no_show_connections(wch[0]);
        show_connections(connext->var_esterna ,wch[0]);
    }

    reset_var_win_list_context(salva_context1);
}




/* 
   delete_connection
   cancella la connessione selezionata.
   Nella var_press e var_release vengono assegnati gli opportuni
   valori ai dati nella struttura SEL_VAR, e in questa routine
   vengono utilizzati.    
*/
int delete_connection( Widget widctx )
{
   extern Boolean simulator_changed;
   int i;
   CONNMODEL *conn,*prec;
   char *salva_context;

   set_var_win_list_context( widctx, &salva_context);


   if( sel_var.status == SELEZIONATO )
   {

/* se e' stata selezionata una connessione */

      if( sel_var.conn )
      {

          conn = locmod->blocchi[indice_blocco].var_conn_task;
          prec = NULL;

          /* cerco tale connessione */
          i=0;
          while( conn != NULL)
          {
             if( conn->var_interna == indice_variabile )
             {
                if( i == (sel_var.conn-1) )
                {
                   /* ho trovato la connessione */
 
                       
                   no_show_connections(XtParent(sel_var.wid));      


                    /* devo aggiornare anche la variabile connessa*/


                    cancella_var_conn( topVarWin1, conn );


                   /* elimino (ma non libero) la connessione */ 
                   if( prec == NULL)
                      locmod->blocchi[indice_blocco].var_conn_task = conn->succ;
                   else
                      prec->succ = conn->succ;           
                     
                    /* decremento le connessioni della variabile*/
                    locmod->blocchi[indice_blocco].variabili[indice_variabile].model_conn--;

                    show_connections( indice_variabile,  XtParent(sel_var.wid));

                    /* se era la unica connessione elimino anche il pixmap */
                    if(!locmod->blocchi[indice_blocco].variabili[indice_variabile].model_conn) 
                        set_var_pixmap(sel_var.wid,"void.bmp");
         
                    simulator_changed = TRUE;
               }
               i++;
            }
            prec = conn; 
            conn = conn->succ;
         } /*end while */ 
      }
   }

   reset_var_win_list_context(salva_context);
}


/*---

   distrugge lo widget interno all'item della lista 
   dove si trova la label che descrive la connessione
   Ricordati la struttura di ciascun item! 

   rowcol  
   pixmap1  pixmap2  label1(variabile)
      label2(variabile connessa dell'altro modello)

---*/

no_show_connections(Widget var_wid)
{
   Arg arg[5];
   Cardinal narg;
   int i, num;
   Widget *wids;
      

/* recupero tutti gli widget interni ad un item della lista
   con indice 0 c'e' il pixmap connessione interna del modello
   con indice 1 c'e' il pixmap connessione con altro modello
   con indice 2 c'e' la label della variabile
   con indice 3 c'e' la label della variabile connessa 
*/   
   narg=0;
   XtSetArg( arg[narg], XmNnumChildren, &num );narg++;
   XtSetArg( arg[narg], XmNchildren, &wids );narg++;
   XtGetValues( var_wid, arg, narg );

/* distruggo tutti gli widget interni ad un item con indice
   uguale o maggiore a 3.
*/
    
   for ( i=3; i<num; i++ )
   {
      if ( sel_var.status == SELEZIONATO && sel_var.wid == wids[i] )
        sel_var.status = NON_SELEZIONATO;

      XtDestroyWidget( wids[i] );
   }    
}

/*
 elimina la visualizzazione di tutte le connessioni
*/
no_show_all_connections()
{
   Arg arg[10];
   Cardinal num,nc;
   Widget *wids,*rc;
   int i,ind_wid;

   show_on = False;

/* recupero tutti gli item della lista di ingresso
*/
   num=0;
   XtSetArg( arg[num], XmNchildren, &wids );num++;
   XtGetValues( UxGetWidget(ingList1), arg, num );

   for(i=ind_wid=0;i<locmod->blocchi[blocco].num_variabili;i++)
   {
      if( locmod->blocchi[blocco].variabili[i].tipo == USCITA ||
          locmod->blocchi[blocco].variabili[i].tipo == STATO )
          continue;

/* se questa veriabile e' connessa con un altro modello 
   elimino la visualizzazione della connessione
*/
      if( locmod->blocchi[blocco].variabili[i].model_conn )
      {
          num=0;
          XtSetArg( arg[num], XmNchildren, &rc );num++;
          XtGetValues( wids[ind_wid], arg, num );
          no_show_connections(rc[0]);
          ind_wid++;
      }      
      else
          ind_wid++;
      
   }

/* eseguo la stessa cosa per la lista di uscita 
*/
   num=0;
   XtSetArg( arg[num], XmNchildren, &wids );num++;
   XtGetValues( UxGetWidget(uscList1), arg, num );

   for(i=ind_wid=0;i<locmod->blocchi[blocco].num_variabili;i++)
   {
      if( locmod->blocchi[blocco].variabili[i].tipo == INGRESSO)
         break;


      if( locmod->blocchi[blocco].variabili[i].model_conn )
      {
          num=0;
          XtSetArg( arg[num], XmNchildren, &rc );num++;
          XtGetValues( wids[ind_wid], arg, num );
          no_show_connections(rc[0]);
          ind_wid++;
      }      
      else
          ind_wid++;
   }
}

/*
   visualizza tutte le connessioni di una variabile
   del blocco 
*/

show_connections( int variabile, Widget var_wid)
{
   swidget lb;
   Widget  wid;
   CONNMODEL  *conn;
   int     count;
   Position swap;
   Dimension he;
   char    str[200];
   BLOCCO *bloc;


   bloc = locmod->blocchi;

/* creo tante label quante sono le connessioni di 
   questa variabile
*/

   for ( conn=bloc[blocco].var_conn_task, count=0;
         count<bloc[blocco].variabili[variabile].model_conn;
         conn = conn->succ )
      if ( conn->var_interna == variabile )
      {
         lb = UxCreateLabel( "var_lb_conn",
                             UxWidgetToSwidget(var_wid) );
        UxPutBackground( lb, UxGetBackground(var_wid) );
        UxPutForeground( lb, UxGetForeground(var_wid) );
        wid = UxCreateWidget( lb );

#ifndef DESIGN_TIME
        UxPutContext(wid, (char *) UxTopVarWin1Context );
#endif

       sprintf( str, "%s     %s  %s",
             simulatore->modelli[conn->modello_esterno]->nome,
             simulatore->modelli[conn->modello_esterno]->blocchi[conn->blocco_esterno].variabili[conn->var_esterna].nome,
             simulatore->modelli[conn->modello_esterno]->blocchi[conn->blocco_esterno].variabili[conn->var_esterna].descr );

        UxPutLabelString( lb, str );
        UxPutBackground( lb, UxGetBackground(var_wid) );
        UxPutForeground( lb, UxGetForeground(var_wid) );


        XtAddEventHandler( wid, ButtonPressMask | OwnerGrabButtonMask, False,
                           var_press, (XtPointer)variabile );
        XtAddEventHandler( wid, ButtonReleaseMask, False,
                           var_release, (XtPointer)variabile );
/**
        XtAddEventHandler( wid, EnterWindowMask, False, var_enter, NULL );
        XtAddEventHandler( wid, LeaveWindowMask, False, var_leave, NULL );
        XtAddEventHandler( wid, ButtonReleaseMask, False,
                           var_release, (XtPointer)variabile );
***/
        if (!count)
        {
           get_something(lb,XmNy,(void*) &swap);
           get_something(lb,XmNheight,(void*) &he);
        }

        swap += he;
        set_something_val(lb,XmNy, (XtArgVal) swap);

        count++;
      }
      
}


/* 
   visualizze tutte le connessioni di tutte le variabili di un blocco
*/

show_all_conn()
{
   Arg arg[10];
   Cardinal num,nc;
   Widget *wids,*rc;
   int i,ind_wid;


   show_on = True;

/* visualizzo tutte le connessioni della lista di ingresso */

   num=0;
   XtSetArg( arg[num], XmNchildren, &wids );num++;
   XtGetValues( UxGetWidget(ingList1), arg, num );


   for(i=ind_wid=0;i<locmod->blocchi[blocco].num_variabili;i++)
   {
      if( locmod->blocchi[blocco].variabili[i].tipo == USCITA ||
          locmod->blocchi[blocco].variabili[i].tipo == STATO )
          continue;

      if( locmod->blocchi[blocco].variabili[i].model_conn )
      {
          num=0;
          XtSetArg( arg[num], XmNchildren, &rc );num++;
          XtGetValues( wids[ind_wid], arg, num );
          show_connections(i,rc[0]);
          ind_wid++;
      }      
      else
          ind_wid++;
   }

/* visualizzo le connessioni di tutte le variabili di uscita */ 
   num=0;
   XtSetArg( arg[num], XmNchildren, &wids );num++;
   XtGetValues( UxGetWidget(uscList1), arg, num );

   for(i=ind_wid=0;i<locmod->blocchi[blocco].num_variabili;i++)
   {


/* esco dal ciclo se ho trovato una variabile di uscita.
   Questo perche' al momento della creazione della lista 
   creo prima quelle di uscita e dopo quelle di ingresso
   e gli indici seguono quindi questa disposizione
*/
      if( locmod->blocchi[blocco].variabili[i].tipo == INGRESSO )
          break;

      if( locmod->blocchi[blocco].variabili[i].model_conn )
      {
          num=0;
          XtSetArg( arg[num], XmNchildren, &rc );num++;
          XtGetValues( wids[ind_wid], arg, num );
          show_connections(i,rc[0]);
          ind_wid++;
      }      
      else
          ind_wid++;
   }

}


/*
 * inverti_colori()
 *
 * Inverte il background ed il foreground del widget argomento.
 */
inverti_colori(swidget  swid )
{
   char *background;

   background = UxGetBackground( swid );
   UxPutBackground( swid, UxGetForeground( swid ) );
   UxPutForeground( swid, background );
}


/* 
   event loop per effettuare le connessioni.
   Viene attivato dalla press del button2 (var_press)
*/
  
event_loop_for_conn(Widget w)
{
  int     fine=False;
  char    *name;
  XEvent  event;
  Widget  wid;
  swidget swid;
  char list_name[10];
  BLOCCO *bloc;
  Cursor cursor;


  bloc = locmod->blocchi;

  XGrabPointer( XtDisplay(w), XtWindow(w), True,
                 Button2MotionMask | ButtonReleaseMask |
                 LeaveWindowMask | EnterWindowMask,
                 GrabModeSync, GrabModeAsync,
                 None, NULL, CurrentTime );

   if ( bloc[blocco].variabili[move_var.var].tipo == INGRESSO )
     strcpy(list_name, "uscSW");
   else
     strcpy(list_name, "ingSW");

   while (!fine)
   {
      XAllowEvents(XtDisplay(w), SyncPointer, CurrentTime);
      XNextEvent(XtDisplay(w),&event);
      switch (event.type)
      {
         case ButtonRelease:

               fine = True;

               if ( move_var.status != MOVE_ON )
                 break;

               if ( gost_win_drag_on )
                 clear_gost_win();

               if ( highlight_on )
               {
                 highlight_on = False;
                 highlight( highlight_wid, "false" );
               }

               wid = XtWindowToWidget( XtDisplay(w), event.xbutton.window);
               if ( wid  &&  wid != w  &&
                    UxIsValidSwidget(swid = UxWidgetToSwidget(wid))  &&
                    (name = UxGetName(swid))  &&  !strcmp( name, "var_lb" )
                  )
                 UxDispatchEvent(&event);
               else
               {
                 move_var.status = NON_SELEZIONATO;
               }

               break;
         case EnterNotify:
         case LeaveNotify:
               wid = XtWindowToWidget(XtDisplay(w), event.xcrossing.window);
               if ( wid  &&  UxIsValidSwidget(swid = UxWidgetToSwidget(wid)) &&
                    (name = UxGetName(swid))  &&
                    ( !strcmp(name, "var_lb")  ||  !strcmp(name, list_name) )
                  )
                 UxDispatchEvent(&event);
               break;

         case MotionNotify:
               if ( move_var.status == MOVE_TEMP )
                  move_var.status = MOVE_ON;
               gost_win( event.xbutton.x_root, event.xbutton.y_root );
               break;

         case Expose:
               UxDispatchEvent(&event);
               break;
      }
   }

   XUngrabPointer(XtDisplay(w), CurrentTime);

}   

/* 
   set di un pixmap su una label
   utilizzato per evidenziare una variabile connessa sulla lista
 
*/
set_var_pixmap(Widget w,char *nome)
{
   Arg arg;
   Widget *wids;

   XtSetArg( arg, XmNchildren, &wids );
   XtGetValues( XtParent(w), &arg, 1 );

   UxPutLabelPixmap( wids[1], nome );

   set_something_val(wids[1],XmNx, (XtArgVal) 2);
}


/*
   visualizza il pixmap di avvenuta connessione "U"
   sulla item della lista della variabile
*/ 
visualizza_connessione(Widget w, int var)
{
   Arg arg[2];
   Cardinal narg,num;
   char *salva_context;
   char *path_pxm;
   char nome_file[1024];

   narg=0;
   XtSetArg( arg[narg], XmNnumChildren, &num );narg++;
   XtGetValues( XtParent(w), &arg, narg );

   if ( path_pxm = getenv( "LEGOCAD_ICO" ) )
   {
     strcpy( nome_file, path_pxm );
     strcat( nome_file, "/" );
   }
   else
     strcpy( nome_file, "" );

   strcat( nome_file, "topconn.bmp" );
   set_var_pixmap( w, nome_file );
   set_var_pixmap( move_var.wid, nome_file );
   show_connections(var,XtParent(w));
}

/* 
   effettua la connessione tra due variabili 
*/

make_task_connection(int mod1,int bl1, int var1,int mod2, int bl2, int var2)
{
    extern Boolean simulator_changed;
   CONNMODEL *new_conn;
   BLOCCO *b1,*b2,*dummy;
   int idummy;

   b1=simulatore->modelli[mod1]->blocchi;
   b2=simulatore->modelli[mod2]->blocchi;

/* in b1 metto per comodita' l'ingresso
   se necessario scambio b1 e b2
*/
   if( b2[bl2].variabili[var2].tipo == INGRESSO )
   {
      dummy = b2;
      b2 = b1;
      b1 = dummy;   

      idummy = mod2;
      mod2 = mod1;
      mod1 = idummy;

      idummy = bl2;
      bl2 = bl1;
      bl1 = idummy;

      idummy = var2;
      var2 = var1;
      var1 = idummy;
   }


/* connessione dell'ingresso (blocco 1) */   

   new_conn = (CONNMODEL *) calloc(1,sizeof(CONNMODEL) );
   if(new_conn == NULL)
   {
     printf("errore allocazione\n");
     return(FALSE);  
   }

   new_conn->modello_interno = mod1;
   new_conn->modello_esterno = mod2;
   new_conn->blocco_interno = bl1;
   new_conn->blocco_esterno = bl2;
   new_conn->var_interna = var1;
   new_conn->var_esterna = var2;

   new_conn->succ = b1[bl1].var_conn_task;
   b1[bl1].var_conn_task = new_conn;

   if( b1[bl1].variabili[var1].tipo == INGRESSO)
       b1[bl1].variabili[var1].model_conn = 1;
   else
       b1[bl1].variabili[var1].model_conn++;


/* connessione del blocco 2 */
   new_conn = (CONNMODEL *) calloc(1,sizeof(CONNMODEL) );
   if(new_conn == NULL)
   {
     printf("errore allocazione\n");
     return(FALSE);  
   }

   new_conn->modello_interno = mod2;
   new_conn->modello_esterno = mod1;
   new_conn->blocco_interno = bl2;
   new_conn->blocco_esterno = bl1;
   new_conn->var_interna = var2;
   new_conn->var_esterna = var1;

   new_conn->succ = b2[bl2].var_conn_task;
   b2[bl2].var_conn_task = new_conn;

   if( b2[bl2].variabili[var2].tipo == INGRESSO)
      b2[bl2].variabili[var2].model_conn = 1;
   else
      b2[bl2].variabili[var2].model_conn++;

   simulator_changed = TRUE;
   
   return(TRUE);
}

/*
   verifico se' la connessione e' effettuabile
*/

tenta_connessione(int mod1,int bl1, int var1,int mod2, int bl2, int var2)
{
   CONNMODEL *conn;
   BLOCCO *b1,*b2,*dummy;
   int idummy;
   char msg[200];

   printf("tenta_connessione \n %s con %s\n",
           simulatore->modelli[mod1]->blocchi[bl1].variabili[var1].nome,
           simulatore->modelli[mod2]->blocchi[bl2].variabili[var2].nome);


   b1=simulatore->modelli[mod1]->blocchi;
   b2=simulatore->modelli[mod2]->blocchi;

/* non permetto la connessione tra due variabili dello stesso tipo */
 
   if( (b1[bl1].variabili[var1].tipo == INGRESSO &&
       b2[bl2].variabili[var2].tipo == INGRESSO)
       ||
       (b1[bl1].variabili[var1].tipo != INGRESSO &&
       b2[bl2].variabili[var2].tipo != INGRESSO)
     )
   {
         sprintf(msg,"Can Not Connect Same TypeVariable"); 
         create_msgToUser(0,msg);

         printf("ERRORE CONNESSIONE TRA DUE TIPI UGUALI\n");
         return(FALSE);
   }


/* in b1 metto sempre per comodita' l'ingresso, 
   se e' il caso scambio b1 e b2 in modo da
   avere in b1 l'ingresso
 */

   if( b2[bl2].variabili[var2].tipo == INGRESSO )
   {
      dummy = b2;
      b2 = b1;
      b1 = dummy;   

      idummy = mod2;
      mod2 = mod1;
      mod1 = idummy;

      idummy = bl2;
      bl2 = bl1;
      bl1 = idummy;

      idummy = var2;
      var2 = var1;
      var1 = idummy;
   }



/* un ingresso puo' ricevere una sola connessione,
   in caso contrario emetto un messaggio e impedisco
   la seconda connessione 
*/ 

   if( (b1[bl1].variabili[var1].connessioni + b1[bl1].variabili[var1].model_conn) )
   {
         printf("ERRORE INGRESSO GIA' CONNESSO %s\n",b1[bl1].variabili[var1].nome);

         sprintf(msg,"Input Variable %s Already Connected",b1[bl1].variabili[var1].nome); 
         create_msgToUser(0,msg);
         return(FALSE);
   }


/* una variabile di uscita puo' essere connessa con una sola variabile di ingresso, 
   quindi verifico che non sia gia' stata connessa co un ingresso del blocco proposto.
   (Ricorda in b1 c'e' l'ingresso in b2' l'uscita)
*/

   conn = b2[bl2].var_conn_task;
   while( conn != NULL )
   {
      if( conn->modello_esterno ==  mod1 && conn->blocco_esterno == bl1 && conn->var_interna == var2)
      {
         sprintf(msg,"Output Variable %s Already Connected With This Block Variable %s",
         b2[bl2].variabili[var2].nome,b1[bl1].variabili[conn->var_esterna].nome);
         create_msgToUser(0,msg);

         printf("ERRORE L'USCITA  %s E' GIA' STATA CONNESSA CON IL BLOCCO %s%s\n",
         b2[bl2].variabili[var2].nome,b1[bl1].sigla_modulo,b1[bl1].sigla_blocco);

         return(FALSE);
      }
      conn = conn->succ;
   } 

   return( make_task_connection(mod1,bl1,var1,mod2,bl2,var2) );

}


/*** 
   ritorna il numero di connessioni di una variabile
   tramite l'indice di widget
***/
 
int num_conn(Widget w)
{
  Arg    arg[3];
  Cardinal narg;
  int  numc,i;
  Widget *wids;

/* ricavo il numero di connessioni da
   numero_widget figli del row_column - 3
   il 3 e' ricavato da 
      2 pixmap di connessione +
      1 label della variabile
*/

/* ricavo i children del row_column */   
  narg=0;
  XtSetArg( arg[narg], XmNnumChildren, &numc );narg++;
  XtSetArg( arg[narg], XmNchildren, &wids );narg++;
  XtGetValues( XtParent(w), arg, narg );

/* restituisco il numero di connessioni */

  for(i=3;i<numc;i++)
     if( wids[i] ==  w)
     {
        return(i-2);
        break;
     } 
  return(0);
}



var_menu_set()
{
   printf("var_menu_set NULL FUNCTION\n"); 
}


/***
  effettua il reset della lista della variabili 
***/

int clear_list( swidget list )
{
   Arg arg[4];
   Cardinal i, num,narg;
   WidgetList wids;

/* recupero i children della lista */
   narg = 0;
   XtSetArg( arg[narg], XmNnumChildren, &num );narg++;
   XtSetArg( arg[narg], XmNchildren, &wids );narg++;
   XtGetValues( UxWidgetToSwidget(list), arg, narg );

/* set stato variabile non selezionato */
   sel_var.status = NON_SELEZIONATO;

/* distruggo tutti gli widget della lista */
   for ( i=0; i<num; i++ )
     XtDestroyWidget( wids[i] );

}


int change_active_block(MODELLO *ttask,int in_blocco,Widget thiswin)
{
   char *salva_context;
   Arg  arg;
   Widget *wids;
   Pixel bck;

   set_var_win_list_context( thiswin, &salva_context);

   blocco = in_blocco;
   indice_blocco = in_blocco;

   show_on = False;
 
   get_something(ingList1,XmNbackground,(void*) &bck);

   set_title(ttask->blocchi[in_blocco]);

   clear_list(ingList1);
   clear_list(uscList1);
   add_variables(ttask->blocchi[in_blocco]);

   set_something_val(ingList1,XmNbackground, (XtArgVal) bck);

   reset_var_win_list_context(salva_context);
 
}


int filtra(char *nome)
{
   printf("I FILTRI NON SONO ANCORA ATTIVI \n");
   return(True);
}


#ifndef DESIGN_TIME

set_var_win_list_context(Widget w, char **salva_context)
{
   swidget swid;

   swid            = UxWidgetToSwidget(w);
   *salva_context  = (char *) UxTopVarWin1Context;
   UxTopVarWin1Context = (_UxCtopVarWin1 *) UxGetContext(swid);
}



reset_var_win_list_context(char *salva_context)
{
   UxTopVarWin1Context = (_UxCtopVarWin1 *) salva_context;
}

#endif


/*
 * highlight()
 *
 * Setta l'attributo highlightOnEnter per i figli 'var_lb' del widget 'w'.
 */

int highlight( Widget w, char *val )
{
   Arg arg[2];
   Widget *sl_child, *rc, *rc_children, *fr_child, *rc_var_children;
   int i, num;

   XtSetArg( arg[0], XmNchildren, &sl_child );
   XtGetValues( w, arg, 1 );

   XtSetArg( arg[0], XmNchildren, &rc );
   XtGetValues( *sl_child, arg, 1 );

   XtSetArg( arg[0], XmNnumChildren, &num );
   XtSetArg( arg[1], XmNchildren, &rc_children );
   XtGetValues( *rc, arg, 2 );

   for ( i=0; i<num; i++ )
   {
     XtSetArg( arg[0], XmNchildren, &fr_child );
     XtGetValues( rc_children[i], arg, 1 );

     XtSetArg( arg[0], XmNchildren, &rc_var_children );
     XtGetValues( *fr_child, arg, 1 );

     UxPutHighlightOnEnter( UxWidgetToSwidget(rc_var_children[1]), val);
   }

}


int set_title(BLOCCO bloc)
{
   char str[100];

   sprintf(str,"%s%s %s",bloc.sigla_modulo,bloc.sigla_blocco,bloc.descr_blocco);
   trim_blank(str);
   UxPutLabelString( varTitleLb1, str );
   
}

/* 
   event handler sulla lista
*/
    
int list_leave(Widget w,XtPointer cd,XLeaveWindowEvent *event)
{
   char *salva_context;
   Arg  arg;
   Widget *wids;

   set_var_win_list_context( XtParent(w), &salva_context);

   if ( event->mode == NotifyNormal  &&  event->detail != NotifyInferior )
   {

      if ( sel_var.status == SEL_ON )
         XGrabPointer( XtDisplay(w), XtWindow(w), False,
                       EnterWindowMask | ButtonReleaseMask,
                       GrabModeAsync, GrabModeAsync,
                       None, NULL, CurrentTime );

      if ( move_var.status == MOVE_ON  &&  highlight_on )
      {
         highlight_on = False;
         highlight( w, "false" );
         gost_win_drag_on = True;
      }
   }

   reset_var_win_list_context(salva_context);
}

/*
   event handler sulla lista
   enter
*/

int list_enter(Widget w, XtPointer cd, XEvent *event )
{
   char   *salva_context;
   Arg    arg;
   Widget *wids;


   set_var_win_list_context(XtParent(w), &salva_context);

   if ( sel_var.status == SEL_ON &&
        (
          event->type==EnterNotify && ((XEnterWindowEvent*)event)->mode==NotifyNormal &&  ((XEnterWindowEvent*)event)->detail!=NotifyInferior 
             ||
          event->type == ButtonRelease
        )
      )
   {
     if ( event->type == ButtonRelease )
     {
        sel_var.status = NON_SELEZIONATO;
        var_menu_set();
     }
     XUngrabPointer( XtDisplay(w), CurrentTime );
   }
   else if ( move_var.status == MOVE_ON  &&  event->type==EnterNotify && ((XEnterWindowEvent*)event)->detail!=NotifyInferior &&  move_var.bloc != blocco 
           )
   {
      /*
       * Setta l'highlight per la scolled window
       */
      XSetInputFocus( XtDisplay(w), XtWindow(w), RevertToNone, CurrentTime );
      highlight_on = True;
      highlight_wid = w;
      highlight( w, "true" );
      /*
       * mentre si e' nella scrolled window non viene disegnata la gost window
       */
      clear_gost_win();
      gost_win_drag_on = False;
   }


   reset_var_win_list_context(salva_context);

}

/*
  event handler sugli item delle variabili
  release
*/

int var_release(Widget w,XtPointer variable,XButtonEvent *event)
{
  Widget wid;
  char *salva_context;
  char str[1024];
  int var;
  BLOCCO *bloc;
  Arg arg;

var = (int)variable;

   /* La routine puo' essere chiamata o per una release su un "var_lb" o
    * "var_lb_conn"  oppure per un release grabbato dalla scrolled window
    * contenete il widget su cui si e' effettuato il press (questo nel caso che
    * il pointer sia uscito dalla scrolled window). Nel secondo caso la scrolled
    * window non e' un swidget, mentre in ambedue i casi lo e' sempre il parent.
    * Ecco perche' si setta il contesto del padre.
    */

   set_var_win_list_context(XtParent(w), &salva_context);


   bloc = locmod->blocchi;

   switch ( event->button )
   {

      case Button1:
                   /*
                    * Se era preselezionata uuna variabile, (quindi il release
                    * e' avvenuto nel widget associato a tale variabile) allora
                    * la seleziona, altrimenti il release e avvenuto al difuori
                    * di un 'var_lb' o di un 'var_lb_conn' widget e quindi
                    * nessuna viene selezionata.
                    */
                   if ( sel_var.status == SEL_TEMP )
                   {
                      sel_var.wid    = w;
                      sel_var.var    = var;
                      sel_var.status = SELEZIONATO;
                      sel_var.conn   = num_conn( w );
                   }
                   else if ( sel_var.status == SEL_ON )
                      sel_var.status = NON_SELEZIONATO;

                   break;

      case Button2:
                   /*
                    * Col secondo bottone premuto vengono dispacciati i soli
                    * eventi di release che avvengono su un 'var_lb' widget.
                    */
                   if ( move_var.status == MOVE_ON  ||
                        move_var.status == MOVE_TEMP )
                     if ( move_var.model == indice_modello )
                     {
/***
                        err_level = ERROR;
                        errore( "ERROR  same block" );
***/
                        printf("ERRORE STESSO MODELLO \n");
                     }
                     else 
                     { 

                       if ( tenta_connessione(indice_modello, blocco, var, move_var.model,move_var.bloc, move_var.var) )
                           visualizza_connessione( w, var );
                     }
                     move_var.status = NON_SELEZIONATO;
                     break;
  }
  reset_var_win_list_context(salva_context);

}


/* event handel sugli widget delle variabili
   press
*/
 
void var_press( Widget w, XtPointer variabile, XButtonEvent *event )
{
   swidget swid;
   int width, height, var;
   char *salva_context;

   var = (int)variabile;

   set_var_win_list_context(w, &salva_context);


   switch ( event->button )
   {
     case Button1:
                    /*
                     * Se era gia' stata selezionata una variabile la deseleziona.
                     */

                    if ( sel_var.status == SELEZIONATO )
                    {
                       swid = UxWidgetToSwidget( sel_var.wid );

                       inverti_colori( swid );

                    }
                    /*
                     * Rende visibile la selezione della variabile
                     */
                    swid = UxWidgetToSwidget( w );

                    inverti_colori( swid );



                    /*
                     * Preseleziona la variabile ( status = SEL_TEMP ).
                     * La selezione diverra' effettiva solo se il release
                     * avviene su un 'var_lb' o un 'var_lb_con'
                     */
                    sel_var.status = SEL_TEMP;
                    sel_var.wid    = w;
                    sel_var.var    = var;
                    sel_var.bloc   = blocco;
                    indice_blocco = blocco;
                    indice_variabile = var;

                    break;
      case Button2:
                    swid   = UxWidgetToSwidget( w );

                       /*
                        * Setta i parametri per disegnare la gost window che segue
                        * il movimento del cursore fino al release di Button2
                        */
                       gost_win_par.width   = UxGetWidth( swid );
                       gost_win_par.height  = UxGetHeight( swid );
                       gost_win_par.mouse_x =  event->x;
                       gost_win_par.mouse_y =  event->y;

                       gost_win_drag_on     = True;

                       /*
                        * memorizza i parametri della variabile da collegare
                        */
                       move_var.status = MOVE_TEMP;
                       move_var.wid    = w;
                       move_var.var    = var;
                       move_var.bloc   = blocco;
                       move_var.model = indice_modello;

                       /*

                        * Event loop per eseguire il collegamento della
                        * variabile. Il loop viene terminato dal release
                        * di Button2.
                        */
                       event_loop_for_conn(w);

                       break;
   }

   reset_var_win_list_context(salva_context);
}



int add_var_list_event_handler(Widget wid)
{
  XtAddEventHandler( wid, LeaveWindowMask, False, list_leave, NULL );
  XtAddEventHandler( wid, EnterWindowMask | ButtonReleaseMask,False, list_enter, NULL );
  XtAddEventHandler( wid, ButtonReleaseMask, False, var_release, 0 );
}


make_var_entry(swidget lista,BLOCCO bloc, int ind_var)
{
  swidget fr, rc, pm, pmconn,lb;
  Widget  wid;
  char str[100], nome_file[1024];
  char *path_pxm,back[100],fore[100];

/**** DEBUG PRINT
printf("make_var_entry ind_var = %d ",ind_var);
******/

  strcpy(back,UxGetBackground(lista));  
  strcpy(fore,UxGetForeground(lista));  

  fr = UxCreateFrame( "var_fr", lista );

  UxPutBackground(fr,back);

  wid = UxCreateWidget( fr );

#ifndef DESIGN_TIME
  UxPutContext(wid, (char *) UxTopVarWin1Context );
#endif

  /* cattura un eventuale release event accaduto al di fuori di un "var_lb" */
  XtAddEventHandler( fr, ButtonReleaseMask, False,
                     var_release, (XtPointer)ind_var );

  rc = UxCreateRowColumn( "var_rc", fr );
  UxPutOrientation( rc, "horizontal" );
  UxPutPacking( rc, "pack_none" );
  UxPutBackground(rc,back);
  wid = UxCreateWidget( rc );

#ifndef DESIGN_TIME
  UxPutContext(wid, (char *) UxTopVarWin1Context );
#endif

  /* cattura un eventuale release event accaduto al di fuori di un "var_lb" */
  XtAddEventHandler( wid, ButtonReleaseMask, False,
                     var_release, (XtPointer)ind_var );

  pm = UxCreateLabel( "var_pm", rc );
  UxPutBackground(pm,back);
  UxPutForeground(pm,fore);
  wid = UxCreateWidget( pm );

  set_something_val(pm,XmNx, (XtArgVal) 20);

#ifndef DESIGN_TIME
  UxPutContext(wid, (char *) UxTopVarWin1Context );
#endif

  UxPutLabelType( pm, "pixmap" );

  /*
   * costruzione del nome del file contenente i bitmap per i widget 'pm'
   */
  if ( path_pxm = getenv( "LEGOCAD_ICO" ) )
  {
    strcpy( nome_file, path_pxm );
    strcat( nome_file, "/" );
  }
  else
    strcpy( nome_file, "" );

  if ( bloc.variabili[ind_var].connessioni )
    strcat( nome_file, "conn.bmp" );
  else
    strcat( nome_file, "void.bmp" );

  UxPutLabelPixmap( pm, nome_file );

  /* cattura un eventuale release event accaduto al di fuori di un "var_lb" */
  XtAddEventHandler( wid, ButtonReleaseMask, False,
                     var_release, (XtPointer)ind_var );



  pmconn = UxCreateLabel( "var_pmconn", rc );
  UxPutBackground(pmconn,back);
  UxPutForeground(pmconn,fore);
  wid = UxCreateWidget( pmconn );

  set_something_val(pmconn,XmNx, (XtArgVal) 2);

#ifndef DESIGN_TIME
  UxPutContext(wid, (char *) UxTopVarWin1Context );
#endif

  UxPutLabelType( pmconn, "pixmap" );

  /*
   * costruzione del nome del file contenente i bitmap per i widget 'pm'
   */
  if ( path_pxm = getenv( "LEGOCAD_ICO" ) )
  {
    strcpy( nome_file, path_pxm );
    strcat( nome_file, "/" );
  }
  else
    strcpy( nome_file, "" );

  if ( bloc.variabili[ind_var].model_conn )
    strcat( nome_file, "topconn.bmp" );
  else
    strcat( nome_file, "void.bmp" );

  UxPutLabelPixmap( pmconn, nome_file );

  /* cattura un eventuale release event accaduto al di fuori di un "var_lb" */
  XtAddEventHandler( wid, ButtonReleaseMask, False,
                     var_release, (XtPointer)ind_var );


  lb = UxCreateLabel( "var_lb", rc );
  UxPutBackground(lb,back);
  UxPutForeground(lb,fore);
  wid = UxCreateWidget( lb );
  sprintf( str, "%s  %s", bloc.variabili[ind_var].nome,
                          bloc.variabili[ind_var].descr );


  UxPutLabelString( lb, str );
  UxPutHighlightThickness( lb, 3 );

  set_something_val(lb,XmNx, (XtArgVal) 40);


  /*
   * Setta il funzionamento dei widget "var_lb".
   */


  XtAddEventHandler( wid, ButtonPressMask | OwnerGrabButtonMask, False,var_press, (XtPointer)ind_var );
/**
  XtAddEventHandler( wid, EnterWindowMask, False, var_enter, NULL );
  XtAddEventHandler( wid, LeaveWindowMask, False, var_leave, NULL );
**/
  XtAddEventHandler( wid, ButtonReleaseMask, False,var_release, (XtPointer)ind_var );
  XtAddEventHandler( wid, Button2MotionMask, False, gost_win, NULL );

#ifndef DESIGN_TIME
  UxPutContext(wid, (char *) UxTopVarWin1Context );
#endif


}

int add_variables(BLOCCO bloc)
{
   int i,j;
   char filtvar[100];


   if(filtro_attivo)
   {

      strcpy(filtvar,"Filter on ->");

      for(i=0;i<filtro_attuale->num_var;i++)
      {
         strcat(filtvar,filtro_attuale->var[i] );
         strcat(filtvar,",");
      }
  
      /* elimino la ultima virgola */
      filtvar[ strlen(filtvar) ] = 0;


      for(j=0;j<bloc.num_variabili; j++ )
      {          
          if( filtra(bloc.variabili[j].nome) )
          {
             if ( bloc.variabili[j].tipo == INGRESSO )
                 make_var_entry( ingList1, bloc, j );
             else
                 make_var_entry( uscList1, bloc, j );
          }
      }
   }
   else
   {

      strcpy(filtvar,"");
      for ( i=0; i<bloc.num_variabili; i++ )
      {
         if ( bloc.variabili[i].tipo == INGRESSO )
           make_var_entry( ingList1, bloc, i );
         else
           make_var_entry( uscList1, bloc, i );   
      }
   }
}  


/*
 * gost_win()
 *
 * La funzione viene attivata su ogni evento di motion con il button2 premuto.
 * Disegna su root un rettangolo che segue lo spostamento del cursore.
 */
int gost_win(int event_x,int event_y )
{
  int x, y;


  if ( move_var.status == MOVE_ON && gost_win_drag_on )
  {
     /*
      * calcola le coordinate del vertice alto sinistro della gost window
      */
     x = event_x - gost_win_par.mouse_x;
     y = event_y - gost_win_par.mouse_y;
     if ( ! gost_win_par.primo )
        /*
         * Cancella la gost window precedente.
         */
        XDrawRectangle( UxDisplay, DefaultRootWindow(UxDisplay), gost_win_gc, gost_win_par.save_x, gost_win_par.save_y,  gost_win_par.width, gost_win_par.height );

     gost_win_par.primo  = False;

     /*  
      * conserva le nuove coordinate per la successiva cancellazione della
      * gost window
      */
     gost_win_par.save_x = x;
     gost_win_par.save_y = y;

     /*
      * Disegna la nuova gost window.
      */
     XDrawRectangle( UxDisplay, DefaultRootWindow(UxDisplay), gost_win_gc, x, y, gost_win_par.width, gost_win_par.height );
  }

}

/*
 * clear_gost_win()
 *
 * Cancella l'ultima gost window dopo che aver rilasciato il button2
 */
clear_gost_win()
{

   XDrawRectangle( UxDisplay, DefaultRootWindow(UxDisplay), gost_win_gc,
                   gost_win_par.save_x, gost_win_par.save_y,
                   gost_win_par.width, gost_win_par.height );

   gost_win_par.primo = True;
}


create_gost_win_gc()
{

  gost_win_gc  = XCreateGC( UxDisplay, DefaultRootWindow(UxDisplay), 0, NULL );

  XSetFunction( UxDisplay, gost_win_gc, GXxor );
  XSetSubwindowMode( UxDisplay, gost_win_gc, IncludeInferiors );
  XSetForeground( UxDisplay, gost_win_gc, ~0 );
  XSetLineAttributes( UxDisplay, gost_win_gc, 0, LineSolid, CapButt, JoinMiter );

}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	createCB_filterSel1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCtopVarWin1          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopVarWin1Context;
	UxTopVarWin1Context = UxContext =
			(_UxCtopVarWin1 *) UxGetContext( UxWidget );
	{
	
	}
	UxTopVarWin1Context = UxSaveCtx;
}

static void	activateCB_menu3_p1_b1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCtopVarWin1          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopVarWin1Context;
	UxTopVarWin1Context = UxContext =
			(_UxCtopVarWin1 *) UxGetContext( UxWidget );
	{
	   delete_connection(UxWidget);
	}
	UxTopVarWin1Context = UxSaveCtx;
}

static void	activateCB_menu5_p1_b1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCtopVarWin1          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopVarWin1Context;
	UxTopVarWin1Context = UxContext =
			(_UxCtopVarWin1 *) UxGetContext( UxWidget );
	{
	   delete_connection(UxWidget);
	}
	UxTopVarWin1Context = UxSaveCtx;
}

static void	activateCB_menu4_p1_b1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCtopVarWin1          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopVarWin1Context;
	UxTopVarWin1Context = UxContext =
			(_UxCtopVarWin1 *) UxGetContext( UxWidget );
	show_all_conn();
	UxTopVarWin1Context = UxSaveCtx;
}

static void	activateCB_menu4_p1_b2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCtopVarWin1          *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopVarWin1Context;
	UxTopVarWin1Context = UxContext =
			(_UxCtopVarWin1 *) UxGetContext( UxWidget );
	no_show_all_connections();
	UxTopVarWin1Context = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_topVarWin1()
{
	Widget	topVarWin1_shell;
	Widget	filterPane1_shell;
	Widget	menu3_shell;
	Widget	menu5_shell;
	Widget	menu4_p1_shell;

	topVarWin1_shell = XtVaCreatePopupShell( "topVarWin1_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 800,
			XmNy, 440,
			XmNwidth, 560,
			XmNheight, 400,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "topVarWin1",
			XmNiconName, "topVarWin1",
			NULL );

	topVarWin1 = XtVaCreateManagedWidget( "topVarWin1",
			xmMainWindowWidgetClass, topVarWin1_shell,
			XmNheight, 400,
			XmNwidth, 560,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( topVarWin1, (char *) UxTopVarWin1Context );

	varForm1 = XtVaCreateManagedWidget( "varForm1",
			xmFormWidgetClass, topVarWin1,
			NULL );

	UxPutContext( varForm1, (char *) UxTopVarWin1Context );

	varTitleLb1 = XtVaCreateManagedWidget( "varTitleLb1",
			xmLabelWidgetClass, varForm1,
			RES_CONVERT( XmNlabelString, "Block Name Selected" ),
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--18-180-75-75-p-103-iso8859-1" ),
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 35,
			XmNwidth, 545,
			XmNy, 10,
			XmNx, 5,
			NULL );

	UxPutContext( varTitleLb1, (char *) UxTopVarWin1Context );

	filterLb1 = XtVaCreateManagedWidget( "filterLb1",
			xmLabelWidgetClass, varForm1,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopWidget, varTitleLb1,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNmarginLeft, 10,
			RES_CONVERT( XmNlabelString, "Filter" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNy, 0,
			XmNx, 0,
			NULL );

	UxPutContext( filterLb1, (char *) UxTopVarWin1Context );

	filterPane1_shell = XtVaCreatePopupShell ("filterPane1_shell",
			xmMenuShellWidgetClass, varForm1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	filterPane1 = XtVaCreateWidget( "filterPane1",
			xmRowColumnWidgetClass, filterPane1_shell,
			XmNpacking, XmPACK_COLUMN,
			XmNwidth, 120,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( filterPane1, (char *) UxTopVarWin1Context );

	defFil1 = XtVaCreateManagedWidget( "defFil1",
			xmPushButtonWidgetClass, filterPane1,
			RES_CONVERT( XmNmnemonic, "N" ),
			RES_CONVERT( XmNlabelString, "No Filter" ),
			NULL );

	UxPutContext( defFil1, (char *) UxTopVarWin1Context );

	filterSel1 = XtVaCreateManagedWidget( "filterSel1",
			xmRowColumnWidgetClass, varForm1,
			XmNleftWidget, filterLb1,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNtopWidget, varTitleLb1,
			XmNtopOffset, 3,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNorientation, XmVERTICAL,
			XmNpacking, XmPACK_COLUMN,
			XmNx, 5,
			XmNspacing, 0,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNy, 25,
			XmNsubMenuId, filterPane1,
			XmNrowColumnType, XmMENU_OPTION,
			NULL );

	UxPutContext( filterSel1, (char *) UxTopVarWin1Context );

	createCB_filterSel1( filterSel1,
			(XtPointer) UxTopVarWin1Context, (XtPointer) NULL );

	varFrame1 = XtVaCreateManagedWidget( "varFrame1",
			xmFrameWidgetClass, varForm1,
			XmNtopWidget, filterSel1,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 310,
			XmNwidth, 540,
			XmNy, 155,
			XmNx, 10,
			NULL );

	UxPutContext( varFrame1, (char *) UxTopVarWin1Context );

	panedWindow3 = XtVaCreateManagedWidget( "panedWindow3",
			xmPanedWindowWidgetClass, varFrame1,
			XmNheight, 155,
			XmNwidth, 565,
			XmNy, 155,
			XmNx, 15,
			NULL );

	UxPutContext( panedWindow3, (char *) UxTopVarWin1Context );

	ingForm1 = XtVaCreateManagedWidget( "ingForm1",
			xmFormWidgetClass, panedWindow3,
			XmNpaneMinimum, 100,
			XmNheight, 160,
			XmNwidth, 535,
			XmNy, 10,
			XmNx, 15,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( ingForm1, (char *) UxTopVarWin1Context );

	ingLb1 = XtVaCreateManagedWidget( "ingLb1",
			xmLabelWidgetClass, ingForm1,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Input" ),
			XmNheight, 25,
			XmNwidth, 150,
			XmNy, 185,
			XmNx, 25,
			NULL );

	UxPutContext( ingLb1, (char *) UxTopVarWin1Context );

	ingSW1 = XtVaCreateManagedWidget( "ingSW1",
			xmScrolledWindowWidgetClass, ingForm1,
			XmNtopWidget, ingLb1,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 230,
			XmNwidth, 215,
			XmNy, 25,
			XmNx, 5,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( ingSW1, (char *) UxTopVarWin1Context );

	ingList1 = XtVaCreateManagedWidget( "ingList1",
			xmRowColumnWidgetClass, ingSW1,
			XmNspacing, 0,
			XmNpacking, XmPACK_TIGHT,
			XmNisAligned, TRUE,
			XmNentryAlignment, XmALIGNMENT_END,
			XmNadjustLast, TRUE,
			XmNadjustMargin, TRUE,
			XmNheight, 100,
			XmNwidth, 475,
			XmNy, 2,
			XmNx, 7,
			NULL );

	UxPutContext( ingList1, (char *) UxTopVarWin1Context );

	menu3_shell = XtVaCreatePopupShell ("menu3_shell",
			xmMenuShellWidgetClass, ingList1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu3 = XtVaCreateWidget( "menu3",
			xmRowColumnWidgetClass, menu3_shell,
			XmNmenuAccelerator, "<KeyUp>F4",
			XmNrowColumnType, XmMENU_POPUP,
			NULL );

	UxPutContext( menu3, (char *) UxTopVarWin1Context );

	menu3_p1_b1 = XtVaCreateManagedWidget( "menu3_p1_b1",
			xmPushButtonWidgetClass, menu3,
			RES_CONVERT( XmNlabelString, "Delete Connection" ),
			NULL );

	UxPutContext( menu3_p1_b1, (char *) UxTopVarWin1Context );

	uscForm1 = XtVaCreateManagedWidget( "uscForm1",
			xmFormWidgetClass, panedWindow3,
			XmNpaneMinimum, 100,
			XmNheight, 160,
			XmNwidth, 52,
			XmNy, 65,
			XmNx, 20,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( uscForm1, (char *) UxTopVarWin1Context );

	uscLb1 = XtVaCreateManagedWidget( "uscLb1",
			xmLabelWidgetClass, uscForm1,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Output" ),
			XmNheight, 25,
			XmNwidth, 165,
			XmNy, 190,
			XmNx, 355,
			NULL );

	UxPutContext( uscLb1, (char *) UxTopVarWin1Context );

	uscSW1 = XtVaCreateManagedWidget( "uscSW1",
			xmScrolledWindowWidgetClass, uscForm1,
			RES_CONVERT( XmNtranslations, "" ),
			XmNshadowThickness, 2,
			XmNtopWidget, uscLb1,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 230,
			XmNwidth, 180,
			XmNy, 225,
			XmNx, 330,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( uscSW1, (char *) UxTopVarWin1Context );

	uscList1 = XtVaCreateManagedWidget( "uscList1",
			xmRowColumnWidgetClass, uscSW1,
			XmNpacking, XmPACK_TIGHT,
			XmNtraversalOn, TRUE,
			XmNspacing, 0,
			XmNsensitive, TRUE,
			RES_CONVERT( XmNbottomShadowColor, "#420048000000" ),
			RES_CONVERT( XmNtopShadowColor, "#0000cb00da00" ),
			XmNshadowThickness, 0,
			XmNheight, 105,
			XmNwidth, 480,
			XmNy, 2,
			XmNx, 2,
			NULL );

	UxPutContext( uscList1, (char *) UxTopVarWin1Context );

	menu5_shell = XtVaCreatePopupShell ("menu5_shell",
			xmMenuShellWidgetClass, uscList1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu5 = XtVaCreateWidget( "menu5",
			xmRowColumnWidgetClass, menu5_shell,
			XmNmenuAccelerator, "<KeyUp>F4",
			XmNrowColumnType, XmMENU_POPUP,
			NULL );

	UxPutContext( menu5, (char *) UxTopVarWin1Context );

	menu5_p1_b1 = XtVaCreateManagedWidget( "menu5_p1_b1",
			xmPushButtonWidgetClass, menu5,
			RES_CONVERT( XmNlabelString, "Delete Connectio" ),
			NULL );

	UxPutContext( menu5_p1_b1, (char *) UxTopVarWin1Context );

	menu4 = XtVaCreateManagedWidget( "menu4",
			xmRowColumnWidgetClass, varForm1,
			XmNancestorSensitive, TRUE,
			XmNmappedWhenManaged, TRUE,
			XmNwidth, 50,
			XmNheight, 11,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNtopWidget, varTitleLb1,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, filterSel1,
			XmNleftOffset, 300,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( menu4, (char *) UxTopVarWin1Context );

	menu4_p1_shell = XtVaCreatePopupShell ("menu4_p1_shell",
			xmMenuShellWidgetClass, menu4,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu4_p1 = XtVaCreateWidget( "menu4_p1",
			xmRowColumnWidgetClass, menu4_p1_shell,
			XmNwidth, 50,
			XmNheight, 21,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( menu4_p1, (char *) UxTopVarWin1Context );

	menu4_p1_b1 = XtVaCreateManagedWidget( "menu4_p1_b1",
			xmPushButtonWidgetClass, menu4_p1,
			RES_CONVERT( XmNlabelString, "Show Connections" ),
			NULL );

	UxPutContext( menu4_p1_b1, (char *) UxTopVarWin1Context );

	menu4_p1_b2 = XtVaCreateManagedWidget( "menu4_p1_b2",
			xmPushButtonWidgetClass, menu4_p1,
			RES_CONVERT( XmNlabelString, "No Show Connections" ),
			NULL );

	UxPutContext( menu4_p1_b2, (char *) UxTopVarWin1Context );

	menu4_top_b1 = XtVaCreateManagedWidget( "menu4_top_b1",
			xmCascadeButtonWidgetClass, menu4,
			XmNalignment, XmALIGNMENT_CENTER,
			XmNsubMenuId, menu4_p1,
			RES_CONVERT( XmNlabelString, "Options" ),
			NULL );

	UxPutContext( menu4_top_b1, (char *) UxTopVarWin1Context );

	XtAddCallback( topVarWin1, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxTopVarWin1Context );

	XtVaSetValues(filterPane1,
			XmNmenuHistory, NULL,
			NULL );

	XtAddCallback( menu3_p1_b1, XmNactivateCallback,
			activateCB_menu3_p1_b1,
			(XtPointer) UxTopVarWin1Context );

	XtAddCallback( menu5_p1_b1, XmNactivateCallback,
			activateCB_menu5_p1_b1,
			(XtPointer) UxTopVarWin1Context );

	XtAddCallback( menu4_p1_b1, XmNactivateCallback,
			activateCB_menu4_p1_b1,
			(XtPointer) UxTopVarWin1Context );

	XtAddCallback( menu4_p1_b2, XmNactivateCallback,
			activateCB_menu4_p1_b2,
			(XtPointer) UxTopVarWin1Context );


	XtAddEventHandler( ingList1, ButtonPressMask,
			False, _UxtopVarWin1MenuPost, menu3 );
	XtAddEventHandler( uscList1, ButtonPressMask,
			False, _UxtopVarWin1MenuPost, menu5 );
	XmMainWindowSetAreas( topVarWin1, (Widget) NULL, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, varForm1 );

	return ( topVarWin1 );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_topVarWin1( _Uxqtask, _Uxind_modello, _Uxind_blocco )
	MODELLO	*_Uxqtask;
	int	_Uxind_modello;
	int	_Uxind_blocco;
{
	Widget                  rtrn;
	_UxCtopVarWin1          *UxContext;

	UxTopVarWin1Context = UxContext =
		(_UxCtopVarWin1 *) XtMalloc( sizeof(_UxCtopVarWin1) );

	qtask = _Uxqtask;
	ind_modello = _Uxind_modello;
	ind_blocco = _Uxind_blocco;
	{
		Cardinal nargs;
		Arg args[10];
		
		if( qtask == NULL)
		   return(NULL);
		
		indice_modello = ind_modello;
		blocco = ind_blocco;
		move_var.status = sel_var.status = NON_SELEZIONATO;
		gost_win_par.primo = True;
		
		filtro_attivo = False;
		
		locmod = qtask;
		
		show_on = False;
		printf("create BLOCCO =%d\n",blocco);
		rtrn = _Uxbuild_topVarWin1();

		filtro_attivo = False;
		nargs=0;
		                
		XtSetArg( args[nargs], XmNkeyboardFocusPolicy, XmPOINTER );nargs++;                
		XtSetValues( XtParent(UxGetWidget(rtrn)), args, nargs );
		
		set_title(qtask->blocchi[ind_blocco]);
		
		add_var_list_event_handler(ingSW1);
		add_var_list_event_handler(uscSW1);
		
		add_variables(qtask->blocchi[ind_blocco]);
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_topVarWin1( _Uxqtask, _Uxind_modello, _Uxind_blocco )
	MODELLO	*_Uxqtask;
	int	_Uxind_modello;
	int	_Uxind_blocco;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_topVarWin1( _Uxqtask, _Uxind_modello, _Uxind_blocco );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

