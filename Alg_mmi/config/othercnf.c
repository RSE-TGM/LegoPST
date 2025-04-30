/**********************************************************************
*
*       C Source:               othercnf.c
*       Subsystem:              8
*       Description:
*       %created_by:    lopez %
*       %date_created:  Thu Oct 24 15:41:04 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: othercnf.c-13 %  (%full_filespec: othercnf.c-13:csrc:8 %)";
#endif
/*
   modulo othercnf.c
   tipo 
   release 5.2
   data 1/11/96
   reserved @(#)othercnf.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)othercnf.c	5.2\t1/11/96";
/*
        Fine sezione per SCCS
*/
/*----------------------------------------------------------------------- 
 *
 * Othercnf.c 
 *
 * Contents: modulo contenente le routine generali Ciumbia e Orca Madosca 
 *           Mannaccia la pupazza 
 *
 *----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xresource.h>
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <UxXt.h>
#include "config.h"
#include <draw.h>
#include "message.h"
#include "top_icon.h"  /* per topologia schemi regolazione */
#include <Xl/Xl.h>
#include <Xl/XlCore.h>
#include <Xl/XlDrawingUtil.h>
#include <Xl/XlPort.h>
#include <Xl/XlIconReg.h>
#include <Xl/XlDispReg.h>
#include <Xl/XlWidgetRes.h>
#include <Xd/Xd.h>
#include <Xd/XdConn.h>
#include <Ol/OlForm.h>
#include <Ol/OlConn.h>


#include "libutilx.h"

extern Display *UxDisplay;
extern int UxScreen;
extern Arg args[];
extern Cardinal lnargs;
extern GC xorGC;
extern void assign_name();
extern swidget topLevelShell;

static Boolean CreoNuovaInterfaccia();
extern void draw_conn(XdConnDraget);

extern int OlFindConnectionByPort(OlConnObject , char *, char* ,char *);
extern void get_child();
Boolean OlDelConnection(OlConnObject , int );
/*
	oggetto OlConn per la gestione delle connessioni di
	interfaccia
*/
OlConnObject conn_obj;


char *names[] = { "white", "black", "yellow", "green", "red", "blue",
                  "light grey",
                  "pale green","medium turquoise", "light blue" ,
                  "pink" };
int num_colors;
Pixel apix[50];
Colormap colormap;  
void proc_DeleteConn();

/*
int Step=1;
*/

#if defined VMS
static double rint();

static double rint(val)
double val;
{
int app_int;
double ret;
	app_int = (int)val;
        ret = (double)app_int;
return(ret);
}
#endif

/*
SetXorGc 
   ricava in base al colore di background della window su cui si disegna
   un colore adatto alla visualizzazione di spostamenti e ridimensionamenti 
   (rubber)
*/ 
GC SetXorGc(wid)
Widget wid;
{
XGCValues values;
XColor Colore;
unsigned int sommaColor;
Colormap cmap;
Display *display;
unsigned long valuemask= GCForeground | GCLineStyle | GCFunction
                          | GCSubwindowMode;
display= XtDisplay(wid);
/*
 Setta il GC per il disegno in xor dell'oggetto in costruzione
*/
/*
 per ottenere un disegno in nero deve tener conto del background
 della window su cui si sta disegnando
*/
get_something(wid, XmNbackground, (void*) &Colore.pixel);
get_something(wid, XmNcolormap, (void*) &cmap);
XQueryColor (display, cmap, &Colore);
values.function = GXxor;
sommaColor= Colore.red + Colore.green + Colore.blue;
if( sommaColor < (32000 * 3))
        values.foreground =  WhitePixel(display,0) ^ Colore.pixel;
else
        values.foreground =  BlackPixel(display,0) ^ Colore.pixel;

values.line_style = LineSolid;
values.subwindow_mode= IncludeInferiors;
return(XtGetGC(wid,valuemask,&values));
}

/*--------------------------------------------------
 *
 *  filtro
 *
 * routine per la gestione dello snapshot (step)
 *
 *--------------------------------------------------*/
int filtro(XEvent *eve)
{
   Window root,child;
   int x_root,y_root,wx,wy;
   unsigned int mask;
   int Step;
   float f_zoom;
   extern float  get_def_zoom();
   f_zoom= get_def_zoom(XtWindowToWidget(UxDisplay, eve->xmotion.window));
   Step= get_step(XtWindowToWidget(UxDisplay, eve->xmotion.window));
   Step = Step * f_zoom; 

   switch(eve->type)
   {
      case MotionNotify:
            while(XCheckTypedEvent(UxDisplay,MotionNotify,eve));
            XQueryPointer(UxDisplay,eve->xmotion.window,&root,&child,
                          &x_root,&y_root,&wx,&wy,&mask);
            eve->xmotion.x = (int)( rint((double)(wx/Step)) * Step );
            eve->xmotion.y = (int)( rint((double)(wy/Step)) * Step );
      break;
      otherwise:
      break;
   }
}

/*----------------------------------------------------
 *
 * which rectangle
 *
 * routine per la individuazione della area
 * sensibile alla move o resize di uno widget 
 *
 *----------------------------------------------------*/
int which_rectangle(Widget wid,Position x,Position y)
{
   extern void get_geometry();
   int i,ixp,iyp;
   Dimension width,height;
   Position wx,wy;


   get_geometry(wid,&wx,&wy,&width,&height);

   ixp = ((float)x/(float)width)*100.0;
   iyp = ((float)y/(float)height)*100.0;


   for(i=0;i<RectangleWidgetNum;i++)
      if(
          (ixp >=  RectanglesWidget[i].px ) &&
          (iyp >=  RectanglesWidget[i].py ) &&
          (ixp <= (RectanglesWidget[i].px + RectanglesWidget[i].pwidth) ) &&
          (iyp <= (RectanglesWidget[i].py + RectanglesWidget[i].pheight))
        )
        return(RectanglesWidget[i].rect);

   return(False);
}

/*-----------------------------------------------------------------
 *
 * point_on_grid
 *
 * Parametri:   int step;      dimensione in pixel dello step 
 *              short coord;   coordinata del punto
 *
 * Valore di ritorno:  short result;   numero di tic relativi alla
 *                                     coord
 *
 * Descrizione : questa routine effettua la conversione di una
 *               coordinata in numero di tic relativi alla griglia
 *
 *-------------------------------------------------------------------*/
 
short point_on_grid(int x,int y,int *xcoord,int *ycoord,int Step)
{
   float xround,yround;

   *xcoord = (int)(((double)(x/Step)) * Step);
   *ycoord = (int)(((double)(y/Step)) * Step);

   xround = abs(x%Step);
   yround = abs(y%Step);

   if(xround > (Step/2))
      *xcoord += Step ;
   if(yround > (Step/2))
      *ycoord += Step ;
   

/****************
   *xcoord = (int)(rint((double)(x/Step))) * Step;
   *ycoord = (int)(rint((double)(y/Step))) * Step;
****************/
}

/*--------------------------------------------------------------
 *
 *  get_numchild
 * 
 *  restituisce il numero dei children del widget wid
 *
 *--------------------------------------------------------------*/
void get_numchild(Widget wid,Cardinal *numChild)
{
    lnargs=0;
    XtSetArg(args[lnargs],XmNnumChildren,numChild);lnargs++;
    XtGetValues(wid,args,lnargs);
}

/*--------------------------------------------------------------
 *
 *  iswidsel 
 * 
 *  restituisce TRUE se lo widget wid e' selezionato altrimenti
 *              FALSE
 *
 *--------------------------------------------------------------*/
Boolean iswidsel(Widget wid)
{
    Boolean selected=False;
    lnargs=0;
    XtSetArg(args[lnargs],XlNselected,&selected);lnargs++;
    XtGetValues(wid,args,lnargs);
    return(selected);
}

/*-----------------------------------------------------------------
 *   get_selected
 *
 *   restituisce la lista e il numero degli widget selezionati 
 *   
 -----------------------------------------------------------------*/

int get_selected(PAGINA *pag,Widget **selezionati,Cardinal *num_selezionati)
{
   int i,j,top=False,notop=False;
   WidgetList children,childcomp;
   Cardinal num_children=0,num_childcomp;
   Cardinal num_childcomp_sel=0; /* numero di children di composite
                               selezionati */
   Boolean one_child_already_selected=False;

/* chiedo la lista dei children della drawing area */
   get_child(pag->drawing,&children,&num_children);
   
/* alloco inizialmente memoria per contenere la lista degli widget */
   *num_selezionati = 0;
   if( (*selezionati = (Widget *)alloca_memoria(num_children,sizeof(Widget) )) == NULL) 
      return(False);

   for(i=0;i<num_children;i++)
   {
      if( iswidsel(children[i]) )
      {
/* se lo widget e' selezionato lo inserisco nella lista 
   N.B. se un XlComposite e' selezionato ed ha un figlio selezionato a sua volta
        quest'ultimo non viene inserito nella lista
*/
         (*selezionati)[(*num_selezionati)] = children[i];
         (*num_selezionati)++;

         top = True;
      }
/* se uno widget XlComposite NON E' selezionato potrebbero esserlo dei suoi figli 
   non ammetto di avere selezionati due children con padri diversi 
*/
      else if( XlIsXlComposite(children[i]) && !iswidsel( children[i] ) )
      {

         get_child(children[i],&childcomp,&num_childcomp);
         for(j=0;j<num_childcomp;j++)
         {
            if( iswidsel(childcomp[j]) )
            {
               if(one_child_already_selected)
               {
                  return(False);
               }
		num_childcomp_sel++;
/* riservo ulteriore spazio nella lista */
               *selezionati = realloc(*selezionati, (num_children)*sizeof(Widget) + (num_childcomp_sel)*sizeof(Widget) ); 
               if( *selezionati == NULL)
                  return(False);
               (*selezionati)[(*num_selezionati)] = childcomp[j];
               (*num_selezionati)++;
               notop = True;
             }
          }
          if(notop)
             one_child_already_selected=True;
      }
   }

/* ho selezionato sia widget figli della drawing che di XlComposite 
   e per il momento non permetto operazioni
*/

   if(*num_selezionati == 0)
      return(False);

   if(top && notop)
   {
      SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Not Omogeneous Selection",NULL,False,NULL,False,NULL);
      libera_memoria((char*)selezionati);
      *num_selezionati=0;
      return(False);
   }

   return(True);
}


/*-----------------------------------------------------
 *
 * desel_widget
 *
 * deselezione singolo widget  
 *-----------------------------------------------------*/
void desel_widget(Widget wid)
{
   lnargs=0;
   XtSetArg(args[lnargs],XlNselected,False);lnargs++;
   XtSetValues(wid,args,lnargs);  
}

/*------------------------------------------------------------
 * pag_deselect_all
 * 
 * deseleziona tutti gli widget posti sulla drawing area
 *
 * funzione di interfaccia con il DRAW
 *
 *-----------------------------------------------------------*/
void pag_deselect_all(Widget drawing)
{
   WidgetList children,childcomp;
   Cardinal num_children,num_childcomp;
   int i,j;

   get_child(drawing,&children,&num_children);

   for(i=0;i<num_children;i++)
   {
      if( XlIsXlComposite(children[i]) )
      {
/* recupero i figli di XlComposite */
         get_child(children[i],&childcomp,&num_childcomp);
         for(j=0;j<num_childcomp;j++)
         {
            desel_widget(childcomp[j]);
         }
      }
      desel_widget(children[i]);
   }
}

/*-------------------------------------------------------------
 *
 * desel_all
 *
 * deseleziona di tutti gli widget selezionati in una pagina
 *------------------------------------------------------------*/
 
void desel_all(PAGINA *pag)
{
   WidgetList children,childcomp;
   Cardinal num_children,num_childcomp;
   int i,j;

/* recupero i figli della pagina */
   get_child(pag->drawing,&children,&num_children);
   
   for(i=0;i<num_children;i++)
   {
      if( XlIsXlComposite(children[i]) )
      {
/* recupero i figli di XlComposite */
         get_child(children[i],&childcomp,&num_childcomp);
         for(j=0;j<num_childcomp;j++)
         {
            desel_widget(childcomp[j]);
         }
      }
      desel_widget(children[i]);
   }

}


/*-----------------------------------------------------------
 *
 * select_bycoord
 *
 *   input:   PAGINA *pag;   puntatore alla pagina dove e' stata fatta la selezione
 *            int x0,y0;     coord di un punto dell'area da selezionare
 *            int x1,y1;     coord del secondo punto dell'area da selezionare
 *
 *-----------------------------------------------------------*/
 
XRectangle select_bycoord(PAGINA *pag,int x0,int y0,int x1,int y1)
{

   WidgetList children;
   Cardinal num_children;
   int i,appo;
   Dimension wx,wy,ww,wh;
   Boolean selez = True;
   XRectangle rect;

/* 
   x0,y0 sono le coord del punto ppiu' vicino all'origine 
   x1,y1 sono le coord del punto piu' lontano dall'origine
   l'origine sulla pagina e' il vertice in alto a sinistra
*/

   desel_all(pag);

   if(x0 > x1)
   {
     appo = x1;
     x1 = x0;
     x0 = appo; 
   }
   if( y0 > y1)
   {
      appo = y1;
      y1 = y0;
      y0 = appo;
   }

/* recupero i figli della pagina */
   get_child(pag->drawing,&children,&num_children); 

   for(i=0;i<num_children;i++)
   {
/* recupero la loro geometry */
      lnargs=0;
      XtSetArg(args[lnargs],XmNx,&wx);lnargs++;
      XtSetArg(args[lnargs],XmNy,&wy);lnargs++;
      XtSetArg(args[lnargs],XmNwidth,&ww);lnargs++;
      XtSetArg(args[lnargs],XmNheight,&wh);lnargs++;
      XtGetValues(children[i],args,lnargs);

/* se la coppia di coordinate della zona include uno widget 
   lo seleziono */
      if( (x0 <= wx) && (y0 <= wy) && (x1 >= (wx+ww) ) && (y1 >= (wy+wh)) )
      {
          lnargs=0;
          XtSetArg(args[lnargs],XlNselected,True);lnargs++;
          XtSetValues(children[i],args,lnargs);
      } 
   }
   rect.x = x0;
   rect.y = y0;
   rect.width  = x1 - x0;
   rect.height = y1 - y0;

   return(rect);
}
 

/*--------------------------------------------------------------------------
    select_zone 

    get x1,y1 relative al pushbutton
    loop eventi 
    draw rubber su motion 
    get x2,y2 relative alla releasebutton
    cancellazione rubber
    deselezione tutti gli widget
    selezione widget compresi nella zona (x1,y1)(x2,y2)
--------------------------------------------------------------------------*/

void select_zone(Widget wid,XEvent *eve,String *param,Cardinal *nparam)
{
   PAGINA *pag;
   int x1,y1,x2,y2;
   Position xcorner,ycorner;
   int x_root,y_root;
   int wx,wy;
   Position xprec,yprec,appo;
   Dimension w,h,wprec,hprec;
   int fine=False;
   unsigned int mask;
   Window root,child;
   XRectangle rett;
   XRectangle *sec_par;
   
   if( *nparam < 2)
   {
      rett.width = 0;
      return;
   }
                  
   sscanf(param[0],"%lx",&pag);
   sscanf(param[1],"%lx",&sec_par);
/*
   sec_par = param[1];
*/
/* inizializzazione variabili */
   xprec = x1 = eve->xbutton.x;
   yprec = y1 = eve->xbutton.y;

   w = wprec = h = hprec = 0;
   xcorner = ycorner = 0;
   

/* inizia la gestione locale degli eventi  */
   while(!fine)
   {
      XAllowEvents(UxDisplay,SyncPointer,CurrentTime);
      XNextEvent(UxDisplay, eve);
      switch (eve->type)
      {
         case ButtonRelease:       /* evento di release del pointer */
            x2 = eve->xbutton.x;
            y2 = eve->xbutton.y;
            fine = True;
         break;
         case MotionNotify:

/* effettua la query del pointer solo quando si e' fermato */

            while(XCheckTypedEvent(UxDisplay,MotionNotify,eve));
            XQueryPointer(UxDisplay,XtWindow(wid),&root,&child,
                          &x_root,&y_root,&wx,&wy,&mask);
             
            if( (wx > 0) && (wy > 0) )
            {
             
               w = abs(wx-x1);
               h = abs(wy-y1);
               if( (x1 < wx) && (y1 < wy) )
               {
                  xcorner = x1;
                  ycorner = y1;
               }
               else if( (x1 > wx) && (y1 < wy) )
               {
                  xcorner = wx;
                  ycorner = y1;
               }
               else if( (x1 < wx) && (y1 > wy) )
               {
                 xcorner = x1;
                 ycorner = wy;
               }
               else if( (x1 > wx) && (y1 > wy) )
               {
                  xcorner = wx;
                  ycorner = wy;
               }
/* disegno il rubber sulla drawing area e cancello quello prec  */
               XDrawRectangle(UxDisplay,XtWindow(wid),xorGC,
                              (int)xprec,(int)yprec,
                              (unsigned int)wprec,(unsigned int)hprec);
               XDrawRectangle(UxDisplay,XtWindow(wid),xorGC,
                              (int)xcorner,(int)ycorner,
                              (unsigned int)w,(unsigned int)h);
               xprec = xcorner;
               yprec = ycorner;
               wprec = w;
               hprec = h;
            }
          break;
          case Expose:
            XtDispatchEvent(eve);
         break;
      }
   }

/* ho terminato il ciclo con un buttonrelease */

/* cancello il rubber */
   XDrawRectangle(UxDisplay,XtWindow(wid),xorGC,
                  (int)xcorner,(int)ycorner,
                  (unsigned int)w,(unsigned int)h);

   rett = select_bycoord(pag,xcorner,ycorner,xcorner+w,ycorner+h);
   if(sec_par != NULL)
     *sec_par = rett;
     
}
 

 

/*----------------------------------------------------------------------------
 *
 * get_geometry0
 *
 * Parameter:  Widget wid;                ID widget
 *             Position      *x,*y;       posizione widget wid
 *             Position      *w,*h;       dimensione widget wid
 *
 * Description: rstituisce  la posizione e la dimensione di uno widget
 *              non tenendo conto della rotazione
 *
 *----------------------------------------------------------------------------*/

void get_geometry0(Widget wid,Position *x0,Position *y0,Dimension  *w0,Dimension *h0)
{
   lnargs = 0;
   XtSetArg(args[lnargs],XlNx0,x0);lnargs++;
   XtSetArg(args[lnargs],XlNy0,y0);lnargs++;
   XtSetArg(args[lnargs],XlNwidth0,w0);lnargs++;
   XtSetArg(args[lnargs],XlNheight0,h0);lnargs++;
   XtGetValues(wid,args,lnargs);
}

/*----------------------------------------------------------------------------
 *
 * get_geometry
 *
 * Parameter:  Widget wid;                ID widget
 *             Position      *x,*y;       posizione widget wid
 *             Position      *w,*h;       dimensione widget wid
 *
 * Description: rstituisce  la posizione e la dimensione di uno widget
 *              tenendo conto della rotazione
 *
 *----------------------------------------------------------------------------*/

void get_geometry(Widget wid,Position *x,Position *y,Dimension  *w,Dimension *h)
{
   lnargs = 0;
   XtSetArg(args[lnargs],XmNx,x);lnargs++;
   XtSetArg(args[lnargs],XmNy,y);lnargs++;
   XtSetArg(args[lnargs],XmNwidth,w);lnargs++;
   XtSetArg(args[lnargs],XmNheight,h);lnargs++;
   XtGetValues(wid,args,lnargs);
}

/*--------------------------------------------------------
 *
 * disegna_linee
 *
 * disegna le linee all'interno del rubber
 * suddividendolo nell dimensioni delle aree sensibili
 * della move e resize
 *------------------------------------------------------*/
void disegna_linee(Window win,XRectangle *rubb,int nrub)
{
   int i,j,nsegments;
   XSegment *segments;
   
/* alloco memoria per contenere i segmenti */
   nsegments = nrub*4;
   if( (segments = (XSegment *)alloca_memoria( nsegments, sizeof(XSegment) )) == NULL )
      return;
   
/* assegno le coordinate ai segmenti su ciascun rubber */
   for(i=0;i<nrub;i++)
   {
       segments[0].x1 = rubb[i].x + (int)( (float)rubb[i].width * 0.25 );
       segments[0].y1 = rubb[i].y;
       segments[0].x2 = segments[0].x1;
       segments[0].y2 = rubb[i].y + rubb[i].height -1;

       segments[1].x1 = rubb[i].x + (int)( (float)rubb[i].width * 0.75 );
       segments[1].y1 = rubb[i].y;
       segments[1].x2 = segments[1].x1;
       segments[1].y2 = rubb[i].y + rubb[i].height -1;

       segments[2].x1 = rubb[i].x;
       segments[2].y1 = rubb[i].y + (int)( (float)rubb[i].height * 0.25 );
       segments[2].x2 = rubb[i].x + rubb[i].width -1;
       segments[2].y2 = segments[2].y1;

       segments[3].x1 = rubb[i].x;
       segments[3].y1 = rubb[i].y + (int)( (float)rubb[i].height * 0.75 );
       segments[3].x2 = rubb[i].x + rubb[i].width -1;
       segments[3].y2 = segments[3].y1;
   }
   XDrawSegments(UxDisplay,win,xorGC,segments,nsegments);

   libera_memoria((char*)segments);
}

/*-----------------------------------------------------------------------
 *
 * draw_rubber
 *
 *
 *------------------------------------------------------------------------*/

int draw_rubber(Widget drawing,Window win,XRectangle *rubber,int nrubber,XRectangle limiti,int operaz)
{
   extern void MovePick();
   Boolean fine=False;
   XEvent eve;
   Window root,child;
   int x,y,winx,winy,xinizio,yinizio;
   int proot_x,proot_y; /* coordinate del pointer rispetto alla root */
   int pwin_x,pwin_y;   /* coordinate del pointer rispetto alla finestra */
   int xoffset,yoffset;
   unsigned int mask;
   WidgetList selected;
   Cardinal num_selected,i;
   int xold,yold,woffset,hoffset;
   int volte;
   XRectangle *locale;
   int intx,inty;
   Boolean prima_volta=True;
   Widget WidDraw;
   int Step;
   float f_zoom;
   extern float  get_def_zoom();
  


   if ( XGrabPointer(UxDisplay, win, True,
                      ButtonReleaseMask|PointerMotionMask,
                     GrabModeSync, GrabModeAsync,
                     None, None, CurrentTime) !=  GrabSuccess)
   {
      return(0);
   }

   if((locale = (XRectangle *)alloca_memoria(nrubber,(int)sizeof(XRectangle))) == NULL)
       return(False);

/* copio i dati in locale */
   memcpy(locale,rubber,(sizeof(XRectangle)*nrubber));

   WidDraw = XtWindowToWidget(UxDisplay,win);
   xorGC = SetXorGc(WidDraw);

   Step = get_step(drawing);
   f_zoom= get_def_zoom(drawing);
   Step= Step*f_zoom;

   volte = 0;
   while(!fine)
   {
      XAllowEvents(UxDisplay,SyncPointer,CurrentTime);
      XNextEvent(UxDisplay, &eve);
      
      switch(eve.type)
      {
         case  Expose:
           XtDispatchEvent(&eve);
         break;
         case MotionNotify:


/* effettua la query del pointer solo se il movemento del pointer e' consistente */ 
            while(XCheckTypedEvent(UxDisplay,MotionNotify,&eve));

            pwin_x = eve.xmotion.x;
            pwin_y = eve.xmotion.y;


            point_on_grid(pwin_x,pwin_y,&pwin_x,&pwin_y,Step);


            if(prima_volta)  
            {

               xinizio = pwin_x;
               yinizio = pwin_y;
                     
               xold = pwin_x; 
               yold = pwin_y; 


               prima_volta = False;
    
            }

           
            MovePick(XtWindowToWidget(UxDisplay,win),&eve);

            switch(operaz)
            {
               case RECTMOVE:


                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);

/* calcolo l'offset */

                  xoffset = pwin_x - xinizio;
                  yoffset = pwin_y - yinizio;


/* assegno le nuove coord al rubber */

                  for(i=0;i<nrubber;i++)
                  {
                     locale[i].x += (short)xoffset;
                     locale[i].y += (short)yoffset;
                  }

                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);

/* aggiorno le coord attuali */
                  xinizio = pwin_x;
                  yinizio = pwin_y;
               break;
               case RECT1:

/* set coordinate sulla griglia */
                  point_on_grid(pwin_x,pwin_y,&pwin_x,&pwin_y,Step);
                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;
                  disegna_linee(win,locale,nrubber);
                  if(volte == 0)
                     disegna_linee(win,locale,nrubber);

/* devo disegnare il rettangolo di dimensioni diverse */
                  xoffset = pwin_x - xinizio;
                  yoffset = pwin_y - yinizio;

                  woffset = xold - pwin_x;
                  hoffset = yold - pwin_y;

/* assegno le nuove coord  e dimensioni rubber */

                  locale[0].x += (short)xoffset;
                  locale[0].y += (short)yoffset;
                  locale[0].width  += (short)woffset;
                  locale[0].height += (short)hoffset;
                  /*  
                  Correzione su coordinate per disegno 
                  Rettangoli
                  */
                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;
                  disegna_linee(win,locale,nrubber);

/* aggiorno le coord attuali */
                  xinizio = pwin_x;
                  yinizio = pwin_y;
                  xold = pwin_x;
                  yold = pwin_y;
               break;
               case RECT2:

/* set coordinate sulla griglia */
                  point_on_grid(pwin_x,pwin_y,&pwin_x,&pwin_y,Step);

                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;
                  disegna_linee(win,locale,nrubber);
                  if(volte == 0)
                     disegna_linee(win,locale,nrubber);

                  yoffset = pwin_y - yinizio;
                  hoffset = yold - pwin_y;

/* assegno le nuove coord  e dimensioni rubber */
                  locale[0].y += (short)yoffset;
                  locale[0].height += (short)hoffset;

                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;
                  disegna_linee(win,locale,nrubber);

/* aggiorno le coord attuali */
                  xinizio = pwin_x;
                  yinizio = pwin_y;
                  xold = pwin_x;
                  yold = pwin_y;
               break;

               break;
               case RECT3:

/* set coordinate sulla griglia */
                  point_on_grid(pwin_x,pwin_y,&pwin_x,&pwin_y,Step);

                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;

                  disegna_linee(win,locale,nrubber);
                  if(volte == 0)
                     disegna_linee(win,locale,nrubber);

/* devo disegnare il rettangolo di dimensioni diverse */
                  yoffset = pwin_y - yinizio;

                  woffset = pwin_x - xold;
                  hoffset = yold - pwin_y;

/* assegno le nuove coord  e dimensioni rubber */
                  locale[0].y += (short)yoffset;
                  locale[0].width  += (short)woffset;
                  locale[0].height += (short)hoffset;

                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;
                  disegna_linee(win,locale,nrubber);

/* aggiorno le coord attuali */
                  xinizio = pwin_x;
                  yinizio = pwin_y;
                  xold = pwin_x;
                  yold = pwin_y;
               break;
               case RECT4:

/* set coordinate sulla griglia */
                  point_on_grid(pwin_x,pwin_y,&pwin_x,&pwin_y,Step);

locale[0].width  -=1;
locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
locale[0].width +=1;
locale[0].height +=1;

                  disegna_linee(win,locale,nrubber);
                  if(volte == 0)
                     disegna_linee(win,locale,nrubber);

/* devo disegnare il rettangolo di dimensioni diverse */
                  woffset = pwin_x - xold;

/* assegno le nuove coord  e dimensioni rubber */
                  locale[0].width  += (short)woffset;

locale[0].width  -=1;
locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
locale[0].width +=1;
locale[0].height +=1;

                  disegna_linee(win,locale,nrubber);

/* aggiorno le coord attuali */
                  xinizio = pwin_x;
                  yinizio = pwin_y;
                  xold = pwin_x;
                  yold = pwin_y;
               break;
               case RECT5:

/* set coordinate sulla griglia */
                  point_on_grid(pwin_x,pwin_y,&pwin_x,&pwin_y,Step);
                  woffset = pwin_x - xold;
                  hoffset = pwin_y - yold;
                  
                  if(woffset==0 && hoffset==0)
                     break;

/* devo disegnare il rettangolo di dimensioni diverse */
                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;

                  disegna_linee(win,locale,nrubber);
                  if(volte == 0)
                     disegna_linee(win,locale,nrubber);



/* assegno le nuove coord  e dimensioni rubber */

                  locale[0].width  += (short)woffset;
                  locale[0].height += (short)hoffset;


                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;
                  disegna_linee(win,locale,nrubber);

/* aggiorno le coord attuali */
                  xinizio = pwin_x;
                  yinizio = pwin_y;
                  xold = pwin_x;
                  yold = pwin_y;
               break;
               case RECT6:

/* set coordinate sulla griglia */
                  point_on_grid(pwin_x,pwin_y,&pwin_x,&pwin_y,Step);

                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;
                  disegna_linee(win,locale,nrubber);
                  if(volte == 0)
                     disegna_linee(win,locale,nrubber);

                  yoffset = pwin_y - yinizio;
                  hoffset = pwin_y - yold;

/* assegno le nuove coord  e dimensioni rubber */
                  locale[0].height += (short)hoffset;

                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;
                  disegna_linee(win,locale,nrubber);

/* aggiorno le coord attuali */
                  xinizio = pwin_x;
                  yinizio = pwin_y;
                  xold = pwin_x;
                  yold = pwin_y;
               break;
               case RECT7:

/* set coordinate sulla griglia */
                  point_on_grid(pwin_x,pwin_y,&pwin_x,&pwin_y,Step);

                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;
                  disegna_linee(win,locale,nrubber);
                  if(volte == 0)
                     disegna_linee(win,locale,nrubber);

/* devo disegnare il rettangolo di dimensioni diverse */
                  xoffset = pwin_x - xinizio;
                  woffset = xold - pwin_x;
                  hoffset = pwin_y - yold;

/* assegno le nuove coord  e dimensioni rubber */
                  locale[0].x += (short)xoffset;
                  locale[0].width  += (short)woffset;
                  locale[0].height += (short)hoffset;

                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;
                  disegna_linee(win,locale,nrubber);

/* aggiorno le coord attuali */
                  xinizio = pwin_x;
                  yinizio = pwin_y;
                  xold = pwin_x;
                  yold = pwin_y;
               break;
               case RECT8:

/* set coordinate sulla griglia */
                  point_on_grid(pwin_x,pwin_y,&pwin_x,&pwin_y,Step);
                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;
                  disegna_linee(win,locale,nrubber);
                  if(volte == 0)
                     disegna_linee(win,locale,nrubber);

/* devo disegnare il rettangolo di dimensioni diverse */
                  xoffset = pwin_x - xinizio;
                  woffset = xold - pwin_x;

/* assegno le nuove coord  e dimensioni rubber */
                  locale[0].x += (short)xoffset;
                  locale[0].width  += (short)woffset;
                  locale[0].width  -=1;
                  locale[0].height -=1;
                  XDrawRectangles(UxDisplay,win,xorGC,locale,nrubber);
                  locale[0].width +=1;
                  locale[0].height +=1;
                  disegna_linee(win,locale,nrubber);

/* aggiorno le coord attuali */
                  xinizio = pwin_x;
                  yinizio = pwin_y;
                  xold = pwin_x;
                  yold = pwin_y;
               break;
            }
         break; 
         case ButtonRelease:
            EndPick(XtWindowToWidget(UxDisplay,win),&eve);
/* ho terminato lo spostamento del rubber */
            XUngrabPointer(UxDisplay,CurrentTime);
            memcpy(rubber,locale,(sizeof(XRectangle)*nrubber));
            fine=True;
         break;
      }
      volte++;
   }
}

/*------------------------------------------------------------
 * isinlimit
 *
 * verifica se una una area  e' contenuto in certi limiti 
 *-------------------------------------------------------------*/

int isinlimit(Window win,XRectangle area,XRectangle limite)
{
   if( (area.x <= limite.x) || ( (area.x+area.width) >= limite.width  ) || 
       (area.y <= limite.y) || ( (area.y+area.height) >= limite.height )   )
   {
            XDrawRectangle(UxDisplay,win,
                           xorGC,area.x,area.y,area.width,area.height);
      return(False);
   }
   return(True);
}
  
/*------------------------------------------------------------
 * isinto
 *
 * verifica che un punto sia contenuto in una area 
 *-------------------------------------------------------------*/

int isinto(int x,int y,XRectangle limite)
{
   if( (x >= limite.x) && ( x <= (limite.x+limite.width ) ) && 
       (y >= limite.y) && ( y <= (limite.y+limite.height) )  )
      return(True);
   return(False);
}

/*-----------------------------------------------------------------------------
 * 
 * move_or_resize
 *
 * Parameter:  Widget wid;        indice widget 
 *             XEvent *eve;       struttura eventi 
 *             String *param;     stringa dei parametri
 *             Cardinal nparam;   numero parametri
 *
 * Description: Questa funzione di translation viene attivata con la Press    
 *              del Button2 e stabilisce se effettuare  lo spostamento o il resize
 *              widget selezionati.
 *
 *-----------------------------------------------------------------------------*/
int move_or_resize(Widget wid,XEvent *eve,String *param,Cardinal *nparam)
{
   PAGINA *pag;
   Cardinal num_selected,num_children,nnipoti;
   Widget *selected,widmove;
   WidgetList children,nipoti;
   int x,y;
   Position px,py;
   Dimension width,height;
   int id_rect,i,j,n;
   Boolean good=False; 
   Window wsrc,wdst,wind;

   if( *nparam < 1)
      return(False);

   sscanf(param[0],"%lx",&pag);
   if(pag == NULL)
   {
      return(False);
   }

/* recupero gli widget selezionati */
   if(get_selected(pag,&selected,&num_selected) == False)
   {
     return(False);
   }

   x = eve->xbutton.x;
   y = eve->xbutton.y;

/* ATTENZIONE allo widget XlComposite 
  puo' accadere che lo widget che riceve l'evento
  di button press non sia selezionato ma figlio di
  un XlComposite selezionato
*/

   for(i=0;i<num_selected;i++)
   {
      if( (wid != selected[i])  && XlIsXlComposite( selected[i]) )
      {
          get_child(selected[i],&children,&num_children);
           
          for(j=0;j<num_children;j++) 
          {
             if( wid == children[j] )
             {
                wsrc = XtWindow(wid);
                wdst = XtWindow(selected[i]);
                XTranslateCoordinates(UxDisplay,wsrc,wdst,
                                      eve->xbutton.x,eve->xbutton.y,
                                      &x,&y,&wind);
                wid = selected[i];
                break;
             }
          }
      }
/* gestione per goggetto grafico */
      else if( (wid != selected[i])  && XlIsXlManager( selected[i]) )
      {
          get_child(selected[i],&children,&num_children);
          for(j=0;j<num_children;j++) 
          {
             if( wid == children[j] )
             {
                wsrc = XtWindow(wid);
                wdst = XtWindow(selected[i]);
                XTranslateCoordinates(UxDisplay,wsrc,wdst,
                                      eve->xbutton.x,eve->xbutton.y,
                                      &x,&y,&wind);
                wid = selected[i];
                break;
             }
             else if( XtIsComposite(children[j]) )
             {
                get_child(children[j],&nipoti,&nnipoti);

                for(n=0;n<nnipoti;n++) 
	        {

                   if( wid == nipoti[n] )
                   {
                      wsrc = XtWindow(wid);
                      wdst = XtWindow(selected[i]);
                      XTranslateCoordinates(UxDisplay,wsrc,wdst,
                                      eve->xbutton.x,eve->xbutton.y,
                                      &x,&y,&wind);
                      wid = selected[i];
                      break;
                   }
	        }
             }
	  }
      }
   } 
   

/* verifico in quale area sensibile dello widget e' avvenuto l'evento */
   id_rect = which_rectangle(wid,(Position)x,(Position)y);
   switch(id_rect)
   {
      case RECT1:
      case RECT2:
      case RECT3:
      case RECT4:
      case RECT5:
      case RECT6:
      case RECT7:
      case RECT8:
      /* resize */
         if(num_selected == 1)
            resize_widget(pag,wid,eve,id_rect);
         else
            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Resize Only One Widget",NULL,False,NULL,False,NULL);
      break;
      case RECTMOVE:
      /* move */
         move_widget(pag,wid,eve);
      break;
      otherwise:
      /* none */
      libera_memoria((char*)(selected));
      return(False);
      break;
   }

   libera_memoria((char*)selected);

}

/*----------------------------------------------------
 * PagRemovePortSelected
 *
 * elimina dalla lista degli widget selezionati 
 * quelli di tipo XlIPort semplicemente ricompattando
 * la lista che viene passata come parametro
 *------------------------------------------------------*/
void PagRemovePortSelected(PAGINA *pag,Widget *selected,Cardinal *num_selected)
{
   int i,j,decrem;
   Cardinal num_selez;
   
   num_selez =  *num_selected;

   decrem = 0;

   if( num_selez )
   {
      for(i=0;i<num_selez;i++)
      {
         if( XlIsPort(selected[i]) )
         {
            decrem++;
            for(j=i;j<num_selez;j++)
            {
               if(j < (num_selez -1 ) )
                  selected[j]=selected[j+1];
            }
         }
      }    
   }
   num_selez -= decrem;
   *num_selected = num_selez;
}

/*----------------------------------------------------
 * PagRemoveIconRegSelected
 *
 * elimina dalla lista degli widget selezionati 
 * quelli di tipo XlIconReg semplicemente ricompattando
 * la lista che viene passata come parametro
 *------------------------------------------------------*/
void PagRemoveIconRegSelected(PAGINA *pag,Widget *selected,Cardinal *num_selected)
{
   int i,j,decrem;
   Cardinal num_selez;
   
   num_selez =  *num_selected;

   decrem = 0;

   if( num_selez )
   {
      for(i=0;i<num_selez;i++)
      {
         if( XlIsIconReg(selected[i]) )
         {
            decrem++;
            for(j=i;j<num_selez;j++)
            {
               if(j < (num_selez -1 ) )
                  selected[j]=selected[j+1];
            }
         }
      }    
   }
   num_selez -= decrem;
   *num_selected = num_selez;
}


/*----------------------------------------------------
 * PagRemoveIconRegChildSelected
 *
 * elimina dalla lista degli widget selezionati 
 * i children di XlIconReg semplicemente 
 * ricompattando la lista che viene passata come parametro
 *------------------------------------------------------*/
void PagRemoveIconRegChildSelected(PAGINA *pag,Widget *selected,Cardinal *num_selected)
{
   int i,j,decrem;
   Cardinal num_selez;
   
   num_selez =  *num_selected;

   decrem = 0;

   if( num_selez )
   {
      for(i=0;i<num_selez;i++)
      {
         if( XlIsIconReg(XtParent(selected[i])) )
         {
            decrem++;
            for(j=i;j<num_selez;j++)
            {
               if(j < (num_selez -1 ) )
                  selected[j]=selected[j+1];
            }
         }
      }    
   }
   num_selez -= decrem;
   *num_selected = num_selez;

}


/*--------------------------------------------------
   assegna le dimensioni fisiche allo widget
   e ricava e setta setta quelle a rotazione 0
  --------------------------------------------------*/

void assegna_whfisiche(XrmDatabase db,Widget wid,Dimension w,Dimension h)
{
   Position w0,h0;
   char appo[100];

   set_something(wid,XmNwidth,(void*) w);
   set_something(wid,XmNheight,(void*) h);

   get_something(wid,XlNwidth0, (void*) &w0);
   get_something(wid,XlNheight0, (void*) &h0);


   sprintf(appo,"%d",w0);
   XlSetResourceByWidget(&db,wid,XlNwidth0,appo); 
   sprintf(appo,"%d",h0);
   XlSetResourceByWidget(&db,wid,XlNheight0,appo); 
} 

/*--------------------------------------------------
   assegna le coordinate fisiche allo widget
   e ricava e setta setta quelle a rotazione 0
  --------------------------------------------------*/

void assegna_xyfisiche(XrmDatabase db,Widget wid,Position x,Position y)
{
   Position x0,y0;
   char appo[100];

   set_something(wid,XmNx,(void*) x);
   set_something(wid,XmNy,(void*) y);

   get_something(wid,XlNx0, (void*) &x0);
   get_something(wid,XlNy0, (void*) &y0);


   sprintf(appo,"%d",x0);
   XlSetResourceByWidget(&db,wid,XlNx0,appo); 
   sprintf(appo,"%d",y0);
   XlSetResourceByWidget(&db,wid,XlNy0,appo); 
}


/*--------------------------------------------------
   assegna le dimensioni logiche allo widget
  --------------------------------------------------*/

void assegna_whlogiche(XrmDatabase db,Widget wid,Dimension w0,Dimension h0)
{
   char appo[100];

   sprintf(appo,"%d",w0);
   XlSetResourceByWidget(&db,wid,XlNwidth0,appo); 
   sprintf(appo,"%d",h0);
   XlSetResourceByWidget(&db,wid,XlNheight0,appo); 
} 

/*--------------------------------------------------
   assegna le coordinate logiche allo widget
  --------------------------------------------------*/

void assegna_xylogiche(XrmDatabase db,Widget wid,Position x0,Position y0)
{
   char appo[100];

   sprintf(appo,"%d",x0);
   XlSetResourceByWidget(&db,wid,XlNx0,appo); 
   sprintf(appo,"%d",y0);
   XlSetResourceByWidget(&db,wid,XlNy0,appo); 
}



/*--------------------------------------------------------------
 *
 * resize_widget
 *
 * Descrizione:
 *
 *-------------------------------------------------------------*/

int resize_widget(PAGINA *pag,Widget wid,XEvent *eve,int id_rect)
{
   Widget *selected;
   Window wpag;
   Cardinal num_selected,num_children;
   int i,j,nrubber,into;
   XRectangle *rubber,limiti;
   char appo[200];
   String nome;
   WidgetClass classe;
   Widget parent,*children;
   Position x,y;
   Dimension width,height,wi0,he0;
   Position x0,y0;


/* recupero gli widget selezionati */
   if(get_selected(pag,&selected,&num_selected) == False)
   {
     printf("resize_widget: exit point 1\n");
     return(False);
   }

   
   PagRemoveIconRegChildSelected(pag,selected,&num_selected);
   PagRemoveIconRegSelected(pag,selected,&num_selected);
   PagRemovePortSelected(pag,selected,&num_selected);

/* recupero la Window su cui disegnare e le sue dimensioni */
   wpag = XtWindow(XtParent(selected[0]));
   get_geometry(XtParent(selected[0]),&limiti.x,&limiti.y,&limiti.width,&limiti.height);

/* setto i limiti di posizionamento */
   limiti.x = 0;
   limiti.y = 0;

/* alloco memoria per contenere i dati che utilizzo per il rubber */
   if( (rubber = (XRectangle *)alloca_memoria( num_selected,sizeof(XRectangle) )) == NULL)
      return(False);
   nrubber = num_selected;

/* devo riempire la struttura XRectangle con i rettangoli degli widget selezionati */
   for(i=0;i<num_selected;i++)
   {
      get_geometry(selected[i],&rubber[i].x,&rubber[i].y,&rubber[i].width,&rubber[i].height);
   }

/* verifico che button2 sia stato premuto all'interno di uno widget selezionato */
   into = 0;
   for(i=0;i<num_selected;i++)
   {
      if( XtWindowToWidget(UxDisplay,eve->xbutton.window) == selected[i] ||
          XtParent(XtWindowToWidget(UxDisplay,eve->xbutton.window)) == selected[i]  ||
          XtParent(XtParent(XtWindowToWidget(UxDisplay,eve->xbutton.window))) == selected[i] )
         into++;
   }

   if(!into)
   {
      libera_memoria((char*)selected);
      libera_memoria((char*)rubber);
      return(False);
   }

/* disegno i rubber */
   draw_rubber(pag->drawing,wpag,rubber,nrubber,limiti,id_rect);

/* verifico che la move sia avvenuta entro i limiti */
   for(i=0;i<num_selected;i++)
   {
      if( ! isinlimit(wpag,rubber[i],limiti) )
      {
         if(selected != NULL)
         {
            libera_memoria((char*)selected);
            libera_memoria((char*)rubber);
         }
         return(False);
      }
   }

/* effettuo la move e resize dello widget */
   for(i=0;i<num_selected;i++)
   {
      assegna_xyfisiche(pag->db,selected[i],rubber[i].x,rubber[i].y);
      assegna_whfisiche(pag->db,selected[i],rubber[i].width,rubber[i].height);

      if( XlIsXlComposite(selected[i]) )
      {
         get_child(selected[i],&children,&num_children);
         for(j=0;j<num_children;j++)
         {
            XtVaGetValues( children[j],
                       XlNx0, &x0,
                       XlNy0, &y0,
                       XlNwidth0, &wi0,
                       XlNheight0, &he0,
                       NULL );

            assegna_xylogiche(pag->db,children[j],x0,y0);
            assegna_whlogiche(pag->db,children[j],wi0,he0);
         }
      }
   }


   UxDisplay->db = pag->db;

/* libero la memoria occupata */
   if(selected != NULL)
   {
      libera_memoria((char*)selected);
      libera_memoria((char*)rubber);
   }
   return(True);
}



/*-----------------------------------------------------------
 *
 * move_widget
 *
 * Descrizione :  effettua la move di uno widget
 *
 *----------------------------------------------------------*/

int move_widget(PAGINA *pag,Widget wid,XEvent *eve)
{
   extern int PagGetType();
   int tipo_pag;
   Widget *selected, *figli_icon;
   Window wpag;
   Cardinal num_selected,num_figli_icon;
   int i,j,nrubber,into;
   XRectangle *rubber,limiti;
   char appo[200];
   Position x0,y0;
    

   tipo_pag = PagGetType(pag);

/* recupero gli widget selezionati */
   if(get_selected(pag,&selected,&num_selected) == False)
   {
     return(False);
   }

   if(tipo_pag != TYPE_LIBRERIA)
      PagRemoveIconRegChildSelected(pag,selected,&num_selected);


    
/* recupero la Window su cui disegnare e le sue dimensioni */
   wpag = XtWindow(XtParent(selected[0]));
   get_geometry(XtParent(selected[0]),&limiti.x,&limiti.y,&limiti.width,&limiti.height);
   limiti.x = -1;
   limiti.y = -1;

/* alloco memoria per contenere i dati che utilizzo per il rubber */
   if( (rubber = (XRectangle *)alloca_memoria( num_selected,sizeof(XRectangle) )) == NULL)
      return(False);
   nrubber = num_selected;

/* devo riempire la struttura XRectangle con i rettangoli degli widget selezionati */
   for(i=0;i<num_selected;i++)
   {
      get_geometry(selected[i],&rubber[i].x,&rubber[i].y,&rubber[i].width,&rubber[i].height);
      rubber[i].width  -= 1;
      rubber[i].height -= 1;
   }

/* verifico che button2 sia stato premuto all'interno di uno widget selezionato */
   into = 0;
   for(i=0;i<num_selected;i++)
   {
      if( XtWindowToWidget(UxDisplay,eve->xbutton.window) == selected[i] || 
          XtParent(XtWindowToWidget(UxDisplay,eve->xbutton.window)) == selected[i] ||
          XtParent(XtParent(XtWindowToWidget(UxDisplay,eve->xbutton.window))) == selected[i] )
      {
         into++;
         break;
      }
      
   }

   if(!into)
   {
      if(selected != NULL)
         libera_memoria((char*)selected);
      if(rubber != NULL)
         libera_memoria((char*)rubber);
      return(False);
   }

/* disegno i rubber */
   StartPageditMove(pag->drawing);
   draw_rubber(pag->drawing,wpag,rubber,nrubber,limiti,RECTMOVE);

/* verifico che la move sia avvenuta entro i limiti */
   for(i=0;i<num_selected;i++)
   {
      if( ! isinlimit(wpag,rubber[i],limiti) )
      {
         if(selected != NULL)
            libera_memoria((char*)selected);
         if(rubber != NULL)
            libera_memoria((char*)rubber);

         return(False); 
      }
   }

/*
 Nel caso di pagina di regolazione
 controlla se e' necessario effettuare degli spostamenti o
 dei resize delle connessioni
*/
   if(!strcmp(pag->geom.tipo,TIPO_REGOLAZIONE))
      select_connections(num_selected,selected,rubber,RECTMOVE);

/* effettuo la move dello widget */
   for(i=0;i<num_selected;i++)
      {
      assegna_xyfisiche(pag->db,selected[i],rubber[i].x,rubber[i].y);
      if(XlIsIconReg(selected[i]))
		{
		get_child(selected[i],&figli_icon,&num_figli_icon);
		for(j=0;j<num_figli_icon;j++)
                       {
			if(XlIsPort(figli_icon[j]))
				XlPortPosizionaLabel(figli_icon[j]);
                       XClearArea(XtDisplay(figli_icon[j]),XtWindow(figli_icon[j]),0,0,0,0,True);
                       }
		}
      }

/* libero la memoria occupata */
   if(selected != NULL)
      libera_memoria((char*)selected);
   if(rubber != NULL)
      libera_memoria((char*)rubber);

   return(True);
}

/*--------------------------------------
 *
 * routine delete in InterfaceMode On
 *
 * cancellazione esclusiva di interfacce
 * di connessione tra pagine 
 *---------------------------------------*/
void delete_interface(PAGINA *pag)
{
   extern int get_selected();
   Cardinal num_selected,num_figli;
   Widget *selected,wporta,*figli;
   int i,j,ret;
   Arg arg[1];
   char app_char;

/* recupero gli widget selezionati */
   if(get_selected(pag,&selected,&num_selected)==False)
        return;
   for(i=0;i<num_selected;i++)
	if(XlInterfaceIconRegIsConnected(selected[i]))
		{
		get_child(selected[i],&figli,&num_figli);
		for(j=0;j<num_figli;j++)
		   if(XlIsInterfacePort(figli[j]))
		      if((ret=OlFindConnectionByPort(conn_obj,XtName(figli[j]),
                            XtName(selected[i]),pag->nomepag))>=0)
				{
				OlDelConnection(conn_obj,ret);
	printf("Sconnetto l'icona di interfaccia %s porta %s pos = %d\n",
			XtName(selected[i]),XtName(figli[j]),ret);
				app_char = '\0';
                                XtSetArg(arg[0],XlNportNameConnect,app_char);
                		XtSetValues(figli[j],arg,1);

	                        XClearArea(XtDisplay(figli[j]),
					XtWindow(figli[j]),0,0,0,0,True);
				}
		}

   return;
}

/*------------------------------------------------------
 *
 *  delete_widget
 *
 *  cancellazione degli widget selezionati 
 *-----------------------------------------------------*/
int delete_widget(PAGINA *pag)
{
   extern int DrawDelete();
   extern Boolean StateInterfaceMode;
   extern void delete_interface();
   extern int PagGetType();
   extern int get_selected();
   int xin,yin,xfin,yfin,xfilo,yfilo;
   Cardinal i,j,num_selected,num_children,num_figli;
   Widget *selected,*children;
   char xrm_type,*ListaNew,*ListaOld,*parent_name,*deleted_name,*occurrence;
   XrmValue xrm_value;
   char strNres[100],strCres[100],strappo[100];
   Widget parent;
   int tipo_pag;

   ListaNew=NULL;
   ListaOld=NULL;
   parent_name=NULL;
   deleted_name=NULL;
   occurrence=NULL;

   if( StateInterfaceMode )
   {
      delete_interface(pag);
      return(True);
   }
/* recupero gli widget selezionati */
   if(get_selected(pag,&selected,&num_selected)==False)
   {
      return(False);
   }

   tipo_pag = PagGetType(pag);

   if(tipo_pag != TYPE_LIBRERIA)
     PagRemoveIconRegChildSelected(pag,selected,&num_selected);

/* determino il tipo di pagina */

   for(i=0;i<num_selected;i++)
   {
      /*
	se l'icona di interfaccia e' connessa non puo' essere cancellata
      */
      if(XlInterfaceIconRegIsConnected(selected[i]))
         { 
            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Selected Widget is a connected Interface IconReg",NULL,False,NULL,False,NULL);
            return(False);
         }

      /*
                se l'icona e' connessa con una icona di interfaccia
                connessa non puo' essere cancellata
      */
      if(XlIsIconReg(selected[i])&&(!XlIsInterfaceIconReg(selected[i])))
        {
        get_child(selected[i],&children,&num_children);
        for(j=0;j<num_children;j++)
                if(XlPortIsConnected(children[j]))
                   {
		   Widget conn_child;
		 
   	           conn_child = XlPortGetConnectedPort(children[j]);

		   if(conn_child != NULL)
		    {
                   if(XlInterfaceIconRegIsConnected(XtParent(conn_child)))
                      {
                      SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo",
        "Selected Widget is connected with a connected Interface IconReg",
                        NULL,False,NULL,False,NULL);
                      return(False);
                      }
	             }
                   }
        }

      /*
	se la porta di interfaccia e' connessa non puo' essere cancellata
      */
      if(XlInterfacePortIsConnected(selected[i]))
         {
            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Selected Widget is a connected Interface Port",NULL,False,NULL,False,NULL);
            return(False);
         }


      if(  !XlIsXlComposite(selected[i]) )
      {
         if( XlIsInEdit(selected[i]) ) 
         { 
            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Selected Widget In Edit",NULL,False,NULL,False,NULL);
            return(False);
         }
      }
      else
      {
         get_child(selected[i],&children,&num_children);
         for(j=0;j<num_children;j++)
         {
            if( XlIsInEdit(children[j]) )
            {
               SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Selected Widget In Edit",NULL,False,NULL,False,NULL);
               return(False);
            }
/*
 esamina se il child e' una porta: in tal caso elimina la connessione
*/
	 if(XlIsPort(children[j]))
		{
/*
 Ricerca le coordinate di inizio della connessione
*/
		get_punto_porta(children[j],&xfilo,&yfilo);
/*
 Individua la connessione e la cancella aggiornando i records di
 porta connessa delle due porte interessate alla connessione
*/
		delete_estr_conn(XtParent(selected[i]),xfilo,yfilo,
				&xin,&yin,&xfin,&yfin);
/***** Viene chiamata dall'oggetto con quando viene cancellato !!
		proc_DeleteConn(XtParent(selected[i]),xin,yin,xfin,yfin);
*/
		}
         }
      }
   }


   for(i=0;i<num_selected;i++)
   {
      if( !XlIsXlComposite(XtParent(selected[i])) )
         XtDestroyWidget(selected[i]);
      else if( XlIsXlComposite(XtParent(selected[i])) )
      {
         parent = XtParent(selected[i]);
         parent_name = XtName(parent);

         deleted_name = XtName(selected[i]);

         strcpy(strNres,parent_name);
         strcat(strNres,".");
         strcat(strNres,"listChildren");

         strcpy(strCres,parent_name);
         strcat(strCres,".");
         strcat(strCres,"ListChildren");

         if( XrmGetResource(pag->db, strNres, strCres, (char**)&xrm_type, &xrm_value) == 0)
         {
            printf("DELETE WIDGET errore XrmGetResource ListChildren\n");
         }


         if( (ListaOld = alloca_memoria(strlen(xrm_value.addr)+1,sizeof(char) ))  == NULL )
            return(False);


         strcpy(ListaOld, xrm_value.addr);

         strcpy(strNres,parent_name);
         strcat(strNres,".");
         strcat(strNres,"numFigli");

         strcpy(strCres,parent_name);
         strcat(strCres,".");
         strcat(strCres,"NumFigli");

         if( XrmGetResource(pag->db, strNres, strCres, (char **)&xrm_type, &xrm_value) == 0)
            return(False);

         num_figli = atoi(xrm_value.addr);

         if(num_figli < 1)
         {
            libera_memoria((char*)selected);
            return(False);
         }

         if( (ListaNew = alloca_memoria(strlen(ListaOld)+1,sizeof(char)) ) == NULL )
            return(False);

/* esisteva un unico figlio o piu' figli */
         if(num_figli == 1)
         {
            strcpy(ListaNew,"");
            num_figli = 0;
         }
         else if( (occurrence = strstr(ListaOld,deleted_name)) != NULL)
         {
            if( occurrence == ListaOld  )
            {
               occurrence = ( strchr(occurrence,' ') +1 );
               occurrence = ( strchr(occurrence,' ') +1 );
               strcpy(ListaNew,occurrence);
               num_figli--;
            } 
            else
            {
               *(occurrence -1) = '\0';
               strcpy(ListaNew,ListaOld);

               occurrence = strchr(occurrence,' ');

               for(j=0;;j++)
               {
                  occurrence++;
                  if(occurrence[0] == ' ')
                  {
                     strncat(ListaNew,occurrence,strlen(occurrence) );
                     num_figli--;
                     break;
                  }
                  else if(occurrence[0] == '\0')
                  {
                     num_figli--;
                     break;
                  }
               }
            }
         }

         XlSetResourceByWidget(&pag->db,parent,XlNlistChildren,ListaNew);
         sprintf(strappo,"%d",num_figli);
         XlSetResourceByWidget(&pag->db,parent,XlNnumFigli,strappo);
         XtDestroyWidget(selected[i]);
      }
   }


   if(ListaNew != NULL)
      libera_memoria((char*)ListaNew);
   if(ListaOld != NULL)
      libera_memoria((char*)ListaOld);
   if(selected != NULL)
      libera_memoria((char*)selected);
   UxDisplay->db = pag->db;
   return(True);
}

/*---------------------------------------------------------------------
 * add_def_translation
 *
 * permette di agganciare run-time ad un widget una translation
 *
 * input :
 *          Widget wid;      widget a cui agganciare la translation
 *          char *stringa;   stringa ove e' definita la translation
 *
 *----------------------------------------------------------------------*/

void add_def_translation(Widget wid,char *stringa)
{
   XtTranslations tr;
   char *policy;
/* verifico la consistenza dei parametri */
   if (wid && stringa)
   {
/* converto la stringa in XtTranslation */
       tr = XtParseTranslationTable(stringa);
       policy = stringa;
       if (*policy == '#')
       {
           policy++;
           switch( *policy )
           {
               case 'r':
                 set_something(wid, XtNtranslations, (void*) tr);
               break;
               case 'a':
                  XtAugmentTranslations(wid, tr);
               break;
               case 'o':
               default:
                  XtOverrideTranslations(wid, tr);
               break;
           }
       }
       else
           XtOverrideTranslations(wid, tr);
   }
}

/*-------------------------------------------------------------------------
 *
 * crea_gc_xor
 *
 * crea un graphics context inpostatdo la function di xor utilizzata nel
 * rubberbanding
 *
 *------------------------------------------------------------------------*/

void crea_gc_xor(GC *gc)
{
   unsigned long valuemask = 0;  /* ignora XGCvalues e usa i default */
   XGCValues values;

   num_colors =  (sizeof names / sizeof names[0]);
   values.line_width = 1;
   values.line_style = LineSolid;
   values.cap_style = CapButt;
   values.join_style = JoinMiter;
   values.function = GXxor;
   values.subwindow_mode = IncludeInferiors;
   values.graphics_exposures = True;

   values.foreground = ~0;

   valuemask = GCLineWidth | GCLineStyle | GCCapStyle | GCFunction |
               GCJoinStyle | GCForeground | GCSubwindowMode;

   *gc = XCreateGC (UxDisplay, RootWindow(UxDisplay, UxScreen),
                      valuemask, &values);
}
 

/*---------------------------------------------------------------------------
 *
 * Validwindow
 *
 * notifica che la window sulla quale e' avvenuto il ReleaseButton
 * sia relativa ad una pagina o uno widget (di tipo Composite) interno 
 * ad essa. 
 *
 * input:
 *        Widget widsorg;  ID Widget sorgente
 *        Window windev;   window sulla quale e' stato rilasciato il button2
 * output:
 *        Widget *parent;  widget parent
 *        PAGINA **pag;    dati della pagina su cui viene posto lo widget
 *        int *dove;       il nuovo Widget viene posto SUDRAWING o SUCOMPOSITE
 *                         ( sulla pagina o su un XlComposite che sono gli unici widget validi  )
 * return value:
 *                True;    se la window che riceve l'oggetto e' valida 
 *                False;   se la window che riceve l'oggetto non e' valida 
 *
 *-------------------------------------------------------------------------*/

int ValidWindow(Widget widsorg,Window windev,Widget *parent,PAGINA **pag,int *dove)
{
   extern PAGINA *pagine;
   int i,tipoPag; 
   PAGINA *succ; 
   Widget wdgeve;
   WidgetList children;
   Cardinal num_children=0;

/* inizializzazione puntatore */
   succ = pagine;
   
/* ricavo widget lo relativo alla finestra dove e' stato rilasciato il mouse */
   wdgeve = XtWindowToWidget(UxDisplay,windev); 

/* verifico che sia all'interno di una finestra valida */
   while( (succ != NULL) )
   {


/* il release button e' avvenuto sulla drawing area di una pagina 
   e l'oggetto non e' una porta  */

      if(( wdgeve == succ->drawing )&&
		(!(XlIsPort(widsorg)||XlIsDispReg(widsorg))))
      {

        tipoPag = PagGetType(succ);
       /* le porte e i dispreg possono essere istanziati solo in libbreria */
	if( (XlIsPort(widsorg)||XlIsDispReg(widsorg)) 
		&& (tipoPag != TYPE_LIBRERIA))
               		return(False);      
/* le IconReg posso istanziarle solo sulle pagine di regolazione */
         if( XlIsIconReg(widsorg) )
            if( (tipoPag != TYPE_REGOLAZIONE ) && (tipoPag != TYPE_LIBRERIA) )
               return(False);      

/* l'IconReg possono essere istanziate sulla pagina solo se e' consistente */
          if( XlIsIconReg(widsorg) )
		{
		if(tipoPag == TYPE_REGOLAZIONE )
			{
			if(!XlCheckIconReg(widsorg))
				return(False);
			}
		}
        

         *parent = wdgeve;
         *pag = succ;
         *dove = SUDRAWING;
         return(True);
      }
      else  /* il release non e' avvenuto sulla drawing area di una pagina */
      {

/* verifico se e' avvenuto su un figlio della drawing area della una pagina */
         get_child(succ->drawing,&children,&num_children);

         for(i=0;i<num_children;i++)
         {

/* si e' un children della pagina */
            if(wdgeve == children[i])
            {

/* si e' un oggetto che puo' accettare a sua volta un figlio */
/*
               if( XtIsComposite(wdgeve) )
*/
               if( XlIsXlComposite(wdgeve) )
               {
/* un padre composite non puo' accettare un figlio somposite */
                  if( XlIsXlComposite(widsorg) )
                  {
                     return(False);
                  }
                  else
                  {
                  /* se l'oggetto e' un porta puo' essere contenuto
		     solo in un IconReg    */
                     if(XlIsPort(widsorg)&&(!XlIsIconReg(wdgeve)))
			return(False);
                  /* se l'oggetto e' un DispReg puo' essere contenuto
		     solo in un IconReg   non di interfaccia */
                     if(XlIsDispReg(widsorg)&&(!XlIsIconReg(wdgeve)))
			return(False);
                     if(XlIsDispReg(widsorg)&&(XlIsInterfaceIconReg(wdgeve)))
			return(False);
                     *parent = wdgeve;
                     *pag = succ;
                     *dove = SUCOMPOSITE;  /* e' un XlComposite */
                     return(True);
                  }
               }
               else
               {
                  return(False);      
               }
            }
         }
      }

/* cerco sulla pagina successiva */
      num_children = 0;
      succ = succ->pagsucc; 
   }

/*  se non e' su una pagina o su un children di una pagina  
    il bottone del mouse e' stato rilasciato all'esterno 
    di una pagina */
   return(False);

}


/*-------------------------------------------------------------------------
 *
 * transferRes
 *
 * PAGINA *pagina;        dati della PAGINA
 * Widget wid;            oggetto da cui ricavare le risorse 
 * int dove;              SUCOMPOSITE o SUDRAWING 
 * XrmDatabase *pSorgDb;  puntatore al data base da cui ricavare le risorse
 *                        per l'oggetto da creare 
 * XrmDatabase *pDestDb;  puntatore al data base delle risorse in cui
 *                        inserire le risorse ricavate dall'oggetto 
 * char *nome_dest;       nome dell'oggetto cui attribuire le nuove risorse
 *                        nome del nuovo oggetto da creare 
 * Widget parent;         indica dove il widget e' stato posto, se sulla 
 *                        drawing area di una pagina o se su un suo figlio  
 *-------------------------------------------------------------------------*/

void transferRes(PAGINA *pagina,Widget widsorg, int dove,XrmDatabase *pSorgDb, XrmDatabase *pDestDb,char *nome_dest,Widget parent)
{
   int i,tipo_sorg,tipo_dest;
   char str_risorsa_sorg[150],str_risorsa_dest[150];
   char *nome_sorg,*nome_child,*nome_padre,*str_type;
   WidgetList children;
   Cardinal num_children=0;


/* stabilisco se lo widget sorgente e' di tipo XlComposite oppure no */
   if( XlIsXlComposite(widsorg) )
      tipo_sorg = COMPOSTO;      
   else
      tipo_sorg = SEMPLICE;


/* ricava i nomi dello widget padre  e dello widget sorgente */
   nome_padre = XtName(parent);
   nome_sorg  = XtName(widsorg);

/* ricavo dove deve essere posto lo widget da 
   istanziare se sulla Drawing Area o su un XlComposite */
   if(dove == SUCOMPOSITE)              
     tipo_dest = COMPOSTO;
   else
     tipo_dest = SEMPLICE;

/* genera il nome delle risorse sorgente */
   strcpy(str_risorsa_sorg,nome_sorg);
   strcat(str_risorsa_sorg,".");

/* compone il nome delle risorse sorgente  destinazione */
   strcpy(str_risorsa_dest,"*");
   strcat(str_risorsa_dest,nome_dest);
   strcat(str_risorsa_dest,".");


/* trasferisco le risorse dalla sorgente alla destinazione */
   XlTransferResource(pSorgDb,pDestDb,widsorg,str_risorsa_sorg,str_risorsa_dest);

}



/*------------------------------------------------------------------
 * add_composite_child
 *
 * Parametri :  PAGINA *pag;
 *              Widget parent;           parent dello widget da aggiungere 
 *              WidgetClass wid_class;   puntatore alla classe del nuovo widget
 *              char * new_name;         nome da inserire nella lista dei children
 *              x,y,w,h;                 dimensioni del nuovo widget
 *               
 *  aggiunge un children ad un XlComposite Widget
 *------------------------------------------------------------------*/

int add_composite_child(PAGINA *pagina,Widget parent,WidgetClass wid_class,char *new_name,
                        Position wx,Position wy,Dimension w,Dimension h)
{
   Cardinal num_children=0;
   char *nome_padre,new_value[2000];
   char *str_type;
   XrmValue xrm_value,xrm_num;
   char appo[300],XlN[100],XlC[100];
   int i,ret;
   char nome_classe[100];
   Boolean first_child=False; /* ATTENZIONE questo boolean e' stato introdotto
                                 per verificare l'aggiunta del primo child
                                 di un XlComposite, perche la getResource
                                 che restituisce un NULL lo fa' anche quando
                                 numFigli e' 0 e cioe' nella istanziaziazione
                                 per la prima volta di figlio ad XlComposite */

   nome_padre = XtName(parent);

/* set delle resource nome,x,y,w,h */
   XlSetResourceByName(&pagina->db,new_name,XlNnome,new_name);
   sprintf(appo,"%d",w);
   XlSetResourceByName(&pagina->db,new_name,XlNwidth0,appo);
   sprintf(appo,"%d",h);
   XlSetResourceByName(&pagina->db,new_name,XlNheight0,appo);
/**
   sprintf(appo,"%d",wx);
   XlSetResourceByName(&pagina->db,new_name,XmNx,appo);
   sprintf(appo,"%d",wy);
   XlSetResourceByName(&pagina->db,new_name,XmNy,appo);
**/
   sprintf(appo,"%d",wx);
   XlSetResourceByName(&pagina->db,new_name,XlNx0,appo);
   sprintf(appo,"%d",wy);
   XlSetResourceByName(&pagina->db,new_name,XlNy0,appo);

/* recupero la resource listChildren */
   strcpy(XlN,nome_padre);
   strcat(XlN,".");
   strcat(XlN,"listChildren");
   strcpy(XlC,nome_padre);
   strcat(XlC,".");
   strcat(XlC,"ListChildren");
   ret = XrmGetResource(pagina->db, XlN, XlC, &str_type, &xrm_value);
   if(ret == 0)
   {
      printf("error on GetResource listChildren\n");
      return(0);
   }

/* recupero la resource numFigli */
   strcpy(XlN,nome_padre);
   strcat(XlN,".");
   strcat(XlN,"numFigli");
   strcpy(XlC,nome_padre);
   strcat(XlC,".");
   strcat(XlC,"NumFigli");
   ret = XrmGetResource(pagina->db, XlN, XlC, &str_type, &xrm_num);
   if(ret == 0)
   {
      printf("GetResource numFigli = 0\n");
   }
   else
     num_children = atoi(xrm_num.addr);

   if(num_children==0)
   {
      first_child = True;
      num_children = 1;
   }
   
/* costruisco la stringa della listChildren */
   new_value[0] = 0;
   if(!first_child) 
      for(i=0;i<(num_children*2);i++)
      {
         sscanf(xrm_value.addr,"%s",appo);
         strcat(new_value,appo);
         strcat(new_value," ");
         xrm_value.addr += (strlen(appo) +1);
      }
         
/* e vi aggiungo il nuovo widget */
   strcat(new_value,new_name);
   strcat(new_value," ");
   strcat(new_value,XlGetClassName(wid_class));


/* aggiorno anche numFigli */
   if(!first_child) 
      sprintf(appo,"%d",num_children+1);
   else
      sprintf(appo,"%d",num_children);

/* e setto le nuove risorse */
   XlSetResourceByName(&pagina->db,nome_padre,XlNlistChildren,new_value);
   XlSetResourceByName(&pagina->db,nome_padre,XlNnumFigli,appo);

}


/*---------------------------------------------------------------------
 * do_rubber
 *
 * cattura gli eventi generati dopo che c'e' stato un evento di press
 * del Button2 all'interno della window IconLibrary e disegna il rubber
 * dell'oggetto catturato; al momento del release del Button2 verifica
 * che l'evento avvenga in una window valida e CREA UNA NUOVA ISTANZA
 * DELL'OGGETTO, al quale viene assegnato un nome con un criterio di
 * progressione.
 *
 * input:
 *        WidgetClass wid_class;  classe dello widget da istanziare
 *        Widget wid;             indice dello widget catturato
 *        Dimension w;            width del rubber
 *        Dimension h;            height del rubber
 *
 *---------------------------------------------------------------------*/

void do_rubber(ICONLIB *plib,WidgetClass wid_class,Widget wid,Dimension w,Dimension h)
{
   extern void PagSetTagReg();
   void SetSetMode();
   extern PAGINA *GetPaginaFromWidget();
   Window root,child;
   XEvent event;
   int fine=False,dove,sucompo,i,ret;
   Cardinal num_children=0;
   int x_root,y_root;
   int wx,wy;
   int xprec,yprec;
   Position x,y;
   Dimension width,height;
   unsigned int mask;
   Widget topwidget,parent,new_wid,wc;
   XrmDatabase SaveDb;
   PAGINA *pagina;
   char new_name[150],child_name[150],*nome_padre,new_value[200];
   char appo[100],XlN[100],XlC[100],lista[2000];
   char value[200];
   char *str_type;
   XrmValue xrm_value;
   char nome_classe[100];
   WidgetList children;
   Boolean volte=False;
   int TipoIco;
   Position x0,y0,app_x,app_y;
   Dimension app_w,app_h,wch,hch;
   int app_rotate,app_ass_rotate,parent_rotate,parent_ass_rotate;
   WidgetClass appWC;
   int Step;
   float f_zoom;
   extern float get_def_zoom();
   


/* grab (cattura) del mouse pointer */
   if ( XGrabPointer(UxDisplay, XtWindow(wid), True,
                      ButtonReleaseMask|PointerMotionMask,
                     GrabModeSync, GrabModeAsync,
                     None, None, CurrentTime) !=  GrabSuccess)
   {
      return;
   }

   XQueryPointer(UxDisplay,RootWindow(UxDisplay,UxScreen),&root,&child,
                 &x_root,&y_root,&wx,&wy,&mask);

   xprec = x_root;
   yprec = y_root;


/* inizia la gestione locale degli eventi  */
   while(!fine)
   {
      XAllowEvents(UxDisplay,SyncPointer,CurrentTime);
      XNextEvent(UxDisplay, &event);
      switch (event.type)
      {
          case MotionNotify:       /* evento di motion del pointer */

/* effettua la query del pointer solo quando si e' fermato */
            while(XCheckTypedEvent(UxDisplay,MotionNotify,&event));
            XQueryPointer(UxDisplay,RootWindow(UxDisplay,UxScreen),&root,&child,
                          &x_root,&y_root,&wx,&wy,&mask);

/* disegno il rubber sulla main window */
            if(volte)
               XDrawRectangle(UxDisplay,RootWindow(UxDisplay,UxScreen),
                           xorGC,xprec,yprec,(unsigned int )w,(unsigned int )h);
      
            
            XDrawRectangle(UxDisplay,RootWindow(UxDisplay,UxScreen),
                           xorGC,x_root,y_root,(unsigned int )w,(unsigned int )h);
            yprec = y_root;
            xprec = x_root;
             
            volte = True;

         break;

         case ButtonRelease:      /* evento di release del button2 */

/* sul button release assegno le coordinate e termino il ciclo */
            fine = True;
            XQueryPointer(UxDisplay,event.xbutton.window,&root,&child,
                          &x_root,&y_root,&wx,&wy,&mask);
         break;
         case Expose:
            XtDispatchEvent(&event);
         break;
      }
   }

/* ho terminato il ciclo con un buttonrelease */
   XUngrabPointer(UxDisplay,CurrentTime);

/* inizializzazione lista children */
   strcpy(lista,"");


/* verifico che l'evento sia avvenuto su una pagina */
   if( ValidWindow(wid,event.xbutton.window,&parent,&pagina,&dove) )
   {
      Step = get_step(pagina->drawing);
      f_zoom= get_def_zoom(pagina->drawing);
      Step= Step * f_zoom;
      point_on_grid(wx,wy,&wx,&wy,Step);

/* cancello il rubber */
      XDrawRectangle(UxDisplay,RootWindow(UxDisplay,UxScreen),
                  xorGC,xprec,yprec,w,h);

/* assegno il nome allo widget se lo ho posto sulla pagina */
      assign_name(pagina,dove,parent,new_name);

/* trasferisco le resources dalla iconlib alla pagina */
      transferRes(pagina,wid,dove,&plib->db,&pagina->db,new_name,parent);
      

      UxDisplay->db = pagina->db;

/* e' necessaria la creazione di uno widget temporaneo perche' 
   successivamente serve il riferimento all'indice dello widget 
*/
      new_wid = XtCreateWidget(new_name, wid_class,parent, NULL,0);

      set_something(new_wid,XmNx,(void*) wx);  
      set_something(new_wid,XmNy,(void*) wy);  
     
      get_something(new_wid,XlNx0, (void*) &x0);  
      get_something(new_wid,XlNy0, (void*) &y0);  

      sprintf(appo,"%d",x0);
      XlSetResourceByName(&pagina->db,new_name,XlNx0,appo);
      sprintf(appo,"%d",y0);
      XlSetResourceByName(&pagina->db,new_name,XlNy0,appo);

      /*
	se il padre e' un composite controllo le eventuali rotazioni
      */
      if(XlIsXlComposite(parent))
      {
      get_something(parent,XlNrotate, (void*) &parent_rotate);  
      get_something(parent,XlNassRotate, (void*) &parent_ass_rotate);  
      get_something(new_wid,XlNrotate, (void*) &app_rotate);  
      get_something(new_wid,XlNassRotate, (void*) &app_ass_rotate);  
      if((parent_rotate != ROTATE_0) || (parent_ass_rotate!=NO_Y_ASS_ROTATE))
	{
	app_rotate = app_rotate - parent_rotate ;
	if(app_rotate<0)
		app_rotate += ROTATE_COMPLETO;
	app_ass_rotate = app_ass_rotate - parent_ass_rotate;
	if(app_ass_rotate < NO_Y_ASS_ROTATE)
		app_ass_rotate = Y_ASS_ROTATE;
	if(((parent_rotate == ROTATE_90) || (parent_rotate == ROTATE_270))&&
		(parent_ass_rotate == Y_ASS_ROTATE))
		{
		app_rotate = app_rotate +ROTATE_180;
		if(app_rotate >= ROTATE_COMPLETO)
			app_rotate -= ROTATE_COMPLETO;
		}
        set_something(new_wid,XlNrotate,(void*) &app_rotate);  
        sprintf(appo,"%d",app_rotate);
        XlSetResourceByName(&pagina->db,new_name,XlNrotate,appo);
        set_something(new_wid,XlNassRotate,(void*) &app_ass_rotate);  
        sprintf(appo,"%d",app_ass_rotate);
        XlSetResourceByName(&pagina->db,new_name,XlNassRotate,appo);
	}
      }



/* Gestisco adesso il caso di trasferimento dalla libreria 
   di un XlComposite che ha dei figli. Quindi devo gestire singolarmente 
   anche tutti i figli dello XlComposite 

      wid e' l'id dello widget sorgente */

      if(XlIsXlComposite(wid))
      {
/* determino quanti e quali sono i figli dello widget XlComposite sorgente */
         get_child(wid,&children,&num_children);
         if(num_children > 0)
         {
            sucompo = SUCOMPOSITE;
            for(i=0;i<num_children;i++)
            {
/* assegno il nome al children da aggiungere al nuovo XlComposite */
               assign_name(pagina,sucompo,new_wid,child_name);
               strcat(lista,child_name);
               strcat(lista," ");
               strcpy(appo,XlGetClassName(XtClass(children[i])));
               strcat(lista,appo);
               strcat(lista," ");

/* trasferisco le risorse dal children dell'XlComposite sorgente, 

   il nuovo widget che riceve le risorse avra'
   come parent il nuovo_widget appena creato cioe'
   l'XlComposite destinazione*/ 

               transferRes(pagina,children[i],sucompo,&plib->db,&pagina->db,child_name,new_wid);
               XlSetResourceByName(&pagina->db,child_name,XlNnome,child_name);
              
            }
         }
/* set della lista children sul nuovo XlComposite */
         XlSetResourceByName(&pagina->db,new_name,XlNlistChildren,lista);
      }
 
         

/* se ho istanziato un widget sulla drawing area */
      if(dove == SUDRAWING)
      {


         pagina->num_widget++;
      }
      else if(dove == SUCOMPOSITE)
      {
/* se ho istanziato uno widget su un XlComposite */
         add_composite_child(pagina,parent,wid_class,new_name,
                             x0,y0,w,h);
      }

/* devo distruggere e ricreare il nuovo Widget */

      if( dove == SUCOMPOSITE)
      {
	extern float get_def_zoom();
        int perc_zoom;
        float f_zoom;
      XtDestroyWidget(new_wid);
      new_wid = XtCreateManagedWidget(new_name, wid_class,
					parent, NULL,0);
	f_zoom= get_def_zoom(pagina->drawing);
	perc_zoom =100.0 * f_zoom;
   sprintf(appo,"%s",(w*f_zoom));
   set_something(new_wid,XmNwidth,  (void*) appo);  
   sprintf(appo,"%s",(h*f_zoom));
   set_something(new_wid,XmNheight, (void*) appo);    
      // set_something(new_wid,XmNwidth,  (w*f_zoom));  
      // set_something(new_wid,XmNheight, (h*f_zoom));  
      }
      else
      {
	extern float get_def_zoom();
	int perc_zoom;
        float f_zoom;
      XtDestroyWidget(new_wid);
      new_wid = XtCreateManagedWidget(new_name, wid_class,parent, NULL,0);
	f_zoom= get_def_zoom(pagina->drawing);
	perc_zoom =100.0 * f_zoom;
   // GUAG2025 modifica di set_something con appo
      // set_something(new_wid,XmNx, (void*) (wx/f_zoom));  
      // set_something(new_wid,XmNy, (void*) (wy/f_zoom));  
      // set_something(new_wid,XlNfattZoom,(void*) perc_zoom);

       sprintf(appo,"%s",(wx/f_zoom));
       set_something(new_wid,XmNx, (void*) appo);  
       sprintf(appo,"%s",(wy/f_zoom));
       set_something(new_wid,XmNy, (void*) appo); 
       sprintf(appo,"%s",perc_zoom);
       set_something(new_wid,XlNfattZoom, (void*) appo);  

      get_something(new_wid,XlNx0, (void*) &x0);  
      get_something(new_wid,XlNy0, (void*) &y0);  

      sprintf(appo,"%d",x0);
      XlSetResourceByName(&pagina->db,new_name,XlNx0,appo);
      sprintf(appo,"%d",y0);
      XlSetResourceByName(&pagina->db,new_name,XlNy0,appo);
      }

      if(XlIsIconReg(new_wid))
      {
         get_something(new_wid,XlNiconRegType, (void*) &TipoIco);
         if(TipoIco == NORMAL_ICONREG)
         {
            PagSetTagReg(pagina,new_wid);
            if( PagGetConnectMode(pagina->drawing) == True)
               set_connect(pagina->drawing,True);
         }
         else
         {
            if( PagGetConnectMode(pagina->drawing) == True)
               set_connect(pagina->drawing,True);
         }
      }

     /*
	setto il visual mode attivo sulla pagina
     */
     SetSetMode(new_wid);

/* put dei dati modificati nel database delle risorse della pagina  
   e non ancora trattati  */
      sprintf(value,"%d",pagina->num_widget);
      XrmPutStringResource(&pagina->db,"*num_widget",value); 
   
      sprintf(value,"%d",pagina->next_num);
      XrmPutStringResource(&pagina->db,"*nextnum",value); 



/* aggiunge le opportune translation ai nuovi widget istanziati */
      AddTransWid(pagina,new_wid);

      UxDisplay->db = pagina->db;
   }
   else
   {
/* cancello il rubber */
      XDrawRectangle(UxDisplay,RootWindow(UxDisplay,UxScreen),
                  xorGC,xprec,yprec,w,h);

      SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Proposed Parent Not Valid",NULL,False,NULL,False,NULL);
   }

}

/*---------------------------------------------------------------------
 * istanzia_widget
 *
 * Descrizione:    Questa funzione  rappresenta la callback
 *                 attivata con la ButtonPress del bottone n.2 del mouse
 *                 sopra uno widget presente nella drawing area di una
 *                 librearia 
 *
 * input:
 *        Widget wid;          id widget catturato dal mouse
 *        String *param;       parametri, contiene la ICONLIB *
 *        Cardinal *nparam;    numero parametri
 *
 *---------------------------------------------------------------------*/

void istanzia_widget(Widget wid,XEvent *eve,String *param,Cardinal *nparam)
{
   Dimension w,h;
   ICONLIB *plib;
   WidgetClass classe;


   if( *nparam < 1)
      return;


   sscanf(param[0],"%lx",&plib);
   classe = XtClass(wid); 
   get_something(wid,XmNwidth, (void*) &w);
   get_something(wid,XmNheight, (void*) &h);
   do_rubber(plib,classe,wid,w,h);
}


void istanzia_widget_parent(Widget wid,XEvent *eve,String *param,Cardinal *nparam)
{

   if( *nparam < 1)
      return;

   istanzia_widget(XtParent(wid),eve,param,nparam);
}

void istanzia_widget_parent_parent(Widget wid,XEvent *eve,String *param,Cardinal *nparam)
{

   if( *nparam < 1)
      return;


   istanzia_widget(XtParent(XtParent(wid)),eve,param,nparam);
}


/*-----------------------------------------------------------------------
 * duplicate_widget
 *
 * Parametri: PAGINA *pagina;
 *            Widget *lista_widget;  widget list da duplicare (quelli selezionati)
 *            Cardinal num_widget;   numero degli widget da duplicare (quelli selezionati)
 *
 * duplica gli widget (selezionati) della lista
 * l'oggetto da duplicare viene deselezionato
 * il nuovo oggetto viene visualizzato selezionato
 * e spostato di DELTAX DELTAY rispetto allo widget sorgente
 *-----------------------------------------------------------------------*/

int duplicate_widget(PAGINA *pag,Widget *lista_widget,Cardinal num_wid)
{
   int i,j;
   Cardinal numchild;
   char new_name[100],new_childname[200],appo[200];
   Position x0,y0;
   Dimension width0,height0;
   Widget new_wid,new_child;
   WidgetList child;
   int Step;  
   float f_zoom;
   int fatt_zoom;
   extern float get_def_zoom();
   
/* ATTENZIONE !!!!!!!!!!!
   la lista degli widget che arriva sono quelli selezionati, tale lista
   contiene  solo gli widget che sono figli diretti della DRAWING AREA
   vedere la get_selected
*/

   UxDisplay->db= pag->db;

   if(num_wid <= 0)
   {
      return(False);
   }


/* deseleziono gli widget selezionati */
   desel_all(pag);

   Step = get_step(pag->drawing);
   f_zoom= get_def_zoom(pag->drawing);
   fatt_zoom = f_zoom *100;
   Step = Step * f_zoom;

/* per ciascun widget selezionato */
   for(i=0;i<num_wid;i++)
   {

/* genero un nuovo nome */
      if(XlIsXlComposite(XtParent(lista_widget[i])) )
         assign_name(pag,SUCOMPOSITE,XtParent(lista_widget[i]),new_name);
      else 
         assign_name(pag,SUDRAWING,pag->drawing,new_name);

      
/* trasferisco le risorse dai widget sorg ai widget dest */
      if(XlIsXlComposite(XtParent(lista_widget[i])) )
      {
         transferRes(pag,lista_widget[i], SUCOMPOSITE,&pag->db, 
                     &pag->db,new_name,XtParent(lista_widget[i]));

/* recupero la geometry del widget da duplicare */
         get_geometry0(lista_widget[i],&x0,&y0,&width0,&height0);
         if(Step == 1)
         {
            x0 -= DELTAX;
            y0 -= DELTAY;
         }
         else
         {
           x0 -= Step;
           y0 -= Step;
         }

/* duplicando un figlio di Xlcomposite devo aggiornare i suoi child */
         add_composite_child(pag,XtParent(lista_widget[i]),
                             XtClass(lista_widget[i]),new_name,x0,y0,width0,height0);

         new_wid = XtCreateManagedWidget(new_name, XtClass(lista_widget[i]),
                                         XtParent(lista_widget[i]), NULL,0);
         AddTransWid(pag,new_wid);
      }
      else
      {
/* trasferisco le risorse dai widget sorg ai widget dest */
         transferRes(pag,lista_widget[i], SUDRAWING,&pag->db, 
                     &pag->db,new_name,pag->drawing);

          
/* recupero la geometria del widget da duplicare */
         get_geometry0(lista_widget[i],&x0,&y0,&width0,&height0);
         if(Step == 1)
         {
            x0 -= DELTAX;
            y0 -= DELTAY;
         }
         else
         {
           x0 -= Step;
           y0 -= Step;
         }
         
/* attribuisco le nuove risorse */

         sprintf(appo,"%d",x0);
         XlSetResourceByName(&pag->db,new_name,XlNx0,appo); 
         sprintf(appo,"%d",y0);
         XlSetResourceByName(&pag->db,new_name,XlNy0,appo); 
         sprintf(appo,"%d",width0);
         XlSetResourceByName(&pag->db,new_name,XlNwidth0,appo); 
         sprintf(appo,"%d",height0);
         XlSetResourceByName(&pag->db,new_name,XlNheight0,appo); 
         XlSetResourceByName(&pag->db,new_name,XlNnome,new_name);
         
/* creo il nuovo widget */
         new_wid = XtCreateManagedWidget(new_name, XtClass(lista_widget[i]),
                                         pag->drawing, NULL,0);

         AddTransWid(pag,new_wid);

      }

/* se ho copiato un XlComposite devo assegnare nuovi nomi ai suoi figli
   ed aggiungergli i figli  dopo avere aggiornato la listChildren */

      if( XlIsXlComposite(new_wid) )
      {
/* reset numero e lista dei children dei composite  */
         XlSetResourceByWidget(&pag->db,new_wid,XlNlistChildren,"");
         XlSetResourceByWidget(&pag->db,new_wid,XlNnumFigli,"0");

/* recupero i children dello widget sorgente */
         get_child(lista_widget[i],&child,&numchild);
         for(j=0;j<numchild;j++)
         {
            get_geometry0(child[j],&x0,&y0,&width0,&height0);
/* definisco il nome del nuovo figlio */
            assign_name(pag,SUCOMPOSITE,new_wid,new_childname);
/* aggiungo all'XlComposite il nuovo figlio */
            add_composite_child(pag,new_wid,XtClass(child[j]),
                                new_childname,x0,y0,width0,height0);
/* duplico le risorse dal figlio sorgente a quello destinazione */
             transferRes(pag,child[j], SUCOMPOSITE,&pag->db, 
                        &pag->db,new_childname,pag->drawing);
            XlSetResourceByName(&pag->db,new_childname,XlNnome,new_childname);
         } 

/* distruggo l'XlComposite (e' necessario per fargli vedere i nuovi figli */
         XtDestroyWidget(new_wid);
/* e alla fine creo il nuovo Widget con caratteristiche identiche
   a quello sorgente 
*/
         new_wid = XtCreateManagedWidget(new_name, XtClass(lista_widget[i]),
                                         pag->drawing, NULL,0);
         AddTransWid(pag,new_wid);
      }

      lnargs=0;
/* lo visualizzo come selezionato */
      XtSetArg(args[lnargs],XlNselected,True);lnargs++;
/*
 Se l'oggetto e' un children di composite non deve essere
 settato il fattore di zoom perche' e' gia' ingrandito
 a causa del ridimensionamento del padre
*/
      if(!XlIsXlComposite(XtParent(new_wid)))
        {
        XtSetArg(args[lnargs],XlNfattZoom,fatt_zoom);lnargs++;
        }
       XtSetValues(new_wid,args,lnargs);

   }

   UxDisplay->db= pag->db;

   if(PagGetType(pag) == TYPE_REGOLAZIONE)
      if( PagGetConnectMode(pag->drawing) == True )
         set_connect(pag->drawing,True);

   return(True);
}

/*----------------------------------------------------------
 *  copy_new
 *
 *   effettua la chiamata alla routine di duplicazione 
 *   degli widget selezionati 
 *----------------------------------------------------------*/

int copy_new(PAGINA *pag)
{
   Widget *selected;
   Cardinal num_selected;
   int i;
   Position x,y;
   Dimension width,height;
   int Step;


/* recupero gli widget selezionati */
   if( get_selected(pag,&selected,&num_selected) == False)
      return(False);

   if(duplicate_widget(pag,selected,num_selected) == False)
   {
      libera_memoria((char*)selected);
      return(False);
   }

   libera_memoria((char*)selected);
   return(True);
}

/*--------------------------------------------------------
 *
 * funzioni di interfaccia con il draw 
 *
 *--------------------------------------------------------*/

XRectangle pagedit_selection(Widget draw_wid,XEvent *eve)
{
   extern PAGINA *GetPaginaFromWidget();
   PAGINA *pag;
   Cardinal nparam;
   char  *param[2];
   char appo[50];
   char appo2[50];
   XRectangle area;

   
   pag = GetPaginaFromWidget(draw_wid);


   nparam = 2;
   sprintf(appo,"%lx",pag);
   param[0] = appo;
   sprintf(appo2,"%lx",&area);
   param[1] = appo2;

   select_zone(draw_wid,eve,param,&nparam);
   return(area);

}

void aggiungi_pagedit_translation(Widget draw_wid)
{
}


void proc_deselect(Widget wgtsel)
{
   extern PAGINA *pagine;
   Widget drawing;
   PAGINA *succ;
   Boolean IsAPag = False;

   succ = pagine;

   if(wgtsel)
   {
      if( XlIsXlComposite(XtParent(wgtsel)) || XlIsXlManager(XtParent(wgtsel)) )
         drawing = XtParent( XtParent(wgtsel) );
      else
         drawing = XtParent(wgtsel);
      

      while( (succ != NULL) )
      {
         if(drawing == succ->drawing)
         {
            IsAPag = True;
            break;
         }
         succ = succ->pagsucc;
      }


      if(IsAPag)
      {

         pag_deselect_all(drawing);

//         DeselectAllDraget(drawing);
// GUAG2025 forse True
         DeselectAllDraget(drawing, True);
         set_something(wgtsel,XlNselected,(void*) True);
      }
   }
}

void PropagaValore(Widget w1,Widget w2)
{
   extern PAGINA *GetPaginaFromWidget();
Widget sorg,dest;
char *tag1, *tag2;
int val1,val2;
Arg arg[4];
OlFormObject form;
char *nome_var;
char * valore;
char *value;
char *newvalue;
PAGINA *pag;

        XtSetArg(arg[0],XlNtagName,&tag1);
        XtGetValues(XtParent(w1),arg,1);
        XtSetArg(arg[0],XlNtagName,&tag2);
        XtGetValues(XtParent(w2),arg,1);
        if(((int)tag1[0]*1000+(int)tag1[1])>((int)tag2[0]*1000+(int)tag2[1]))
                { sorg = w2; dest = w1; }
        else
                { sorg = w1; dest = w2; }
        /*
                ricavo il valore dal Widget sorg
        */
        XtSetArg(arg[0],XlNportName,&nome_var);
        XtGetValues(sorg,arg,1);

        XtSetArg(arg[0],XlNobjectForm,&form);
        XtSetArg(arg[1],XlNinputValue,&value);
        XtGetValues(XtParent(sorg),arg,2);
/*
 Estrae dalla form il valore della variabile in base al nome della
 tag.
 Il valore e' una stringa allocata internamente alla funzione.
 deve essere successivamente liberato con free.
*/
        valore = EstrTagVal(form,OL_FORM_INPUT_VALUE,nome_var,2,value);

        if ( valore==NULL )
            {
            fprintf(stderr,"PropagaValore: EstrTagVal restituisce NULL\n");
            show_message("Valore in stringa input non corretto\n");
            return;
            }
        /*
                inserisco il valore nella risorsa e nel database
        */
        XtSetArg(arg[0],XlNportName,&nome_var);
        XtGetValues(dest,arg,1);

       XtSetArg(arg[0],XlNobjectForm,&form);
        XtSetArg(arg[1],XlNinputValue,&value);
        XtGetValues(XtParent(dest),arg,2);
/* 
 Poiche' InsTagVal agisce facendo una realloc, passo a InsTagVal
 una copia della risorsa di input value ricavata dall'oggetto
*/
	newvalue=XtNewString(value);
        InsTagVal(form,OL_FORM_INPUT_VALUE,nome_var,2,&newvalue,valore);
	XtFree(valore);
/*
 Inserisco nell'oggetto il nuovo valore
*/
        XtSetArg(arg[0],XlNinputValue,newvalue);
        XtSetValues(XtParent(dest),arg,1);
/*
 Poiche' nell'oggetti IconReg il settaggio di inputValue implica la sua
 copia posso liberare il valore allocato
*/
	XtFree(newvalue);

        pag = GetPaginaFromWidget(XtParent(XtParent(dest)));
        XlSetResourceByWidget(&pag->db,XtParent(dest),XlNinputValue,value);

}

Boolean proc_SelPort(Widget w,Widget wseconda,int modo,GC gcPort)
{
   extern PAGINA *GetPaginaFromWidget();
PAGINA *pag, *pagPort;
char resPorta[MAXCHAR_PAGNAME+20];
extern Boolean StateInterfaceMode;
extern void PropagaValore();
XEvent ev;
Arg arg[4];
Dimension x,y,px,py,width,height;
static char     *regPoliTable = "#replace\n\
	<Btn1Down>:second_point_conn()\n\
	<Btn2Down>:end_draw_conn()\n\
	<Btn3Down>:end_draw_conn()\n\
	<Key>F11:end_draw_conn()\n\
	<Key>F5:draw_delete()\n\
	<KeyPress>Left:move_tasti()\n\
	<KeyPress>Right:move_tasti()\n\
	<KeyPress>Up:move_tasti()\n\
	<KeyPress>Down:move_tasti()\n\
	<KeyPress>Return:second_point_conn()\n\
	<Motion>:draw_draget()\n";

XtSetArg(arg[0],XmNx,&x);
XtSetArg(arg[1],XmNy,&y);
XtSetArg(arg[2],XmNwidth,&width);
XtSetArg(arg[3],XmNheight,&height);
XtGetValues(w,arg,4);
XtSetArg(arg[0],XmNx,&px);
XtSetArg(arg[1],XmNy,&py);
XtGetValues(XtParent(w),arg,2);
ev.xbutton.x = x + px + width/2;
ev.xbutton.y = y + py + height/2;

/*
	se la porta e' di interfaccia puo' essere connessa solo
	in InterfaceMode
*/
if(XlIsInterfacePort(w) && !StateInterfaceMode)
	return(False);

/*
	se si e' in InterfaceMode possono essere connesse solo le
	porte di interfaccia
*/
if(!XlIsInterfacePort(w) && StateInterfaceMode)
	return(False);

if(modo == 1)
        {
        /* inizio connessione, se la porta non e' di interfaccia vengono
	   aggiunte le translation per il disegno della connessione      */
	if(!XlIsInterfacePort(w))
		{
        	FirstPointConn(XtParent(XtParent(w)),&ev,gcPort);
        	add_def_translation(XtParent(XtParent(w)), regPoliTable);
		}
        }
else
        {
        /* termine commessione */
/*
	se ci si trova in InterfaceMode controlla se
        la connessione e' possibile ed in caso
        affermativo registra la connessione
*/
	if(StateInterfaceMode)
		{
		Boolean ConnOk;
		ConnOk=CreoNuovaInterfaccia(w,wseconda);
		if(ConnOk==False)
			return(False);
		}
/*
 Setta le risorse di nome porta connessa per entrambe le porte interessate
 dalla connessione
*/

   	pag = GetPaginaFromWidget(XtParent(XtParent(w)));
   	pagPort = GetPaginaFromWidget(XtParent(XtParent(wseconda)));
        if(XlIsInterfacePort(wseconda))
            sprintf(resPorta,"%s %s",XtName(wseconda),pagPort->nomepag);
        else
            sprintf(resPorta,"%s",XtName(wseconda));
        XlSetResourceByWidget(&pag->db,w,XlNportNameConnect,resPorta);
        XtSetArg(arg[0],XlNportNameConnect,resPorta);
        XtSetValues(w,arg,1);
printf("DEBUG:other_c.c: sel wid =%s set a %s\n",XtName(w),resPorta);

   	pag = GetPaginaFromWidget(XtParent(XtParent(wseconda)));
   	pagPort = GetPaginaFromWidget(XtParent(XtParent(w)));
        if(XlIsInterfacePort(w))
            sprintf(resPorta,"%s %s",XtName(w),pagPort->nomepag);
	else
            sprintf(resPorta,"%s",XtName(w));
        XlSetResourceByWidget(&pag->db,wseconda,XlNportNameConnect,resPorta);
        XtSetArg(arg[0],XlNportNameConnect,resPorta);
        XtSetValues(wseconda,arg,1);
printf("DEBUG:other_c.c: sel2 wid =%s set a %s\n",XtName(w),resPorta);


        /*
		se erano state aggiunte le translation viene fermato il
		disegno della connessione
	*/
	if(!XlIsInterfacePort(wseconda))
        	EndDrawConn(XtParent(XtParent(w)),&ev);
	/*
		genera un expose su entrambe le porte
	*/
	XClearArea(XtDisplay(w),XtWindow(w),0,0,0,0,True);
	XClearArea(XtDisplay(wseconda),XtWindow(wseconda),0,0,0,0,True);
	/*
		se ci si trova in InterfaceMode registra la connessione
	*/
/*
	if(StateInterfaceMode)
		CreoNuovaInterfaccia(w,wseconda);
*/
        /*
	  se le icone non sono di interfaccia 
	*/
	if(!(XlIsInterfaceIconReg(XtParent(w)) || 
		XlIsInterfaceIconReg(XtParent(wseconda))))
        		PropagaValore(w,wseconda);
        }
return(True);
}

/*
 get_punto_porta
 calcola il punto in cui parte la connessione
*/
get_punto_porta(w,xpunto,ypunto)
Widget w; /* widget della porta */
int *xpunto;
int *ypunto;
{
Arg arg[4];
Dimension width,height;
Position x,y,px,py;

XtSetArg(arg[0],XmNx,&x);
XtSetArg(arg[1],XmNy,&y);
XtSetArg(arg[2],XmNwidth,&width);
XtSetArg(arg[3],XmNheight,&height);
XtGetValues(w,arg,4);
XtSetArg(arg[0],XmNx,&px);
XtSetArg(arg[1],XmNy,&py);
XtGetValues(XtParent(w),arg,2);
*xpunto = (int) (x + px + width/2);
*ypunto = (int) (y + py + height/2);
}

/*
 get_posiz_porta
 calcola la posizione della porta rispetto all'origine della
 drawing area
*/
get_posiz_porta(w,xpunto,ypunto)
Widget w; /* widget della porta */
int *xpunto;
int *ypunto;
{
Arg arg[4];
Position x,y,px,py;


XtSetArg(arg[0],XmNx,&x);
XtSetArg(arg[1],XmNy,&y);
XtGetValues(w,arg,2);
XtSetArg(arg[0],XmNx,&px);
XtSetArg(arg[1],XmNy,&py);
XtGetValues(XtParent(w),arg,2);
*xpunto = (int) (x + px);
*ypunto = (int) (y + py);
}

/*
 su_porta
 resituisce True se l'estremo della connessione (filo) identificata dalle
 coordinate xfilo yfilo si trova sulla porta avente come indice di widget
 w
*/
Boolean su_porta(w,xfilo,yfilo)
Widget w; /* widget porta */
int xfilo,yfilo;
{
Arg arg[4];
int xporta,yporta; /* coordinate del punto di arrivo o partenza
                    della connessione */

get_punto_porta(w,&xporta,&yporta);


if(xfilo == xporta && yfilo == yporta)
	return(True);
else
	return(False);
}

/*
 sconnetti
 sconnette la porta di indice wid settando la risorsa nome della porta
 connessa ad una stringa di lunghezza nulla
 
*/
sconnetti(wid)
Widget wid; /* porta da sconnettere */
{
   extern Widget ClipBoardForCopy;
   extern PAGINA *GetPaginaFromWidget();
PAGINA *pag;
Arg arg[4];
/*
 poiche' la routine viene chiamata anche nel caso di
 clear della clipboard e' necessario uscire senza
 far nulla  (la pagina associata non e' infatti
 definita).
*/
if(XtParent(XtParent(wid)) == ClipBoardForCopy)
   return;
pag = GetPaginaFromWidget(XtParent(XtParent(wid)));
XlSetResourceByWidget(&pag->db,wid,XlNportNameConnect,"");
XtSetArg(arg[0],XlNportNameConnect,"");
XtSetValues(wid,arg,1);
printf("DEBUG:other_c.c: sconnetti wid =%s set a %s\n",XtName(wid)," ");
}

/*
 proc_DeleteConn
 funzione di callback richiamata all'atto della cancellazione 
 di una connessione.
 Gli argomenti della callback sono il widget di drawing area su cui
 e' disegnata la connessione e le coordinate dei punti estremi della
 connessione stessa.
 La funzione ricerca tra tutti gli oggetti presenti sulla pagina le
 porte e per ciascuna di essa controlla se la connessione interessa 
 la porta.
 In caso affermativo aggiorna l'attributo di nome porta connessa
 sostituendolo con una stringa di lunghezza nulla.
*/
void proc_DeleteConn(drawing,xin,yin,xfin,yfin)
Widget drawing;
int xin,yin,xfin,yfin;
{
   WidgetList children,childcomp;
   Cardinal num_children,num_childcomp;
   int i,j;

   get_child(drawing,&children,&num_children);

   for(i=0;i<num_children;i++)
   {
      if( XlIsIconReg(children[i]) )
      {
/* recupero i figli di XlComposite */
         get_child(children[i],&childcomp,&num_childcomp);
         for(j=0;j<num_childcomp;j++)
         {
	 if(XlIsPort(childcomp[j]))
		{
		if(su_porta(childcomp[j],xin,yin))
	        	sconnetti(childcomp[j]);
		if(su_porta(childcomp[j],xfin,yfin))
			sconnetti(childcomp[j]);
		}
         }
      }
   }
}

/*
 set_connect
 setta al valore di flag (1 o 0) l'attributo di connessioni abilitate
 di tutte le porte presenti sulla pagina drawing
*/
void set_connect(drawing,set)
Widget drawing;
int set;
{
   Arg arg[4];
   WidgetList children,childcomp;
   Cardinal num_children,num_childcomp;
   int i,j;

   get_child(drawing,&children,&num_children);

   if(set == 1)
        /*
		ingresso in connect mode
	*/
	{
	XlSetSomething(children,num_children,"Port",
                XlNconnectMode,CONNESSIONE_PORTE_ATTIVA);
	XlSetSomething(children,num_children,NULL, XlNselected,0);
	XlSetSomething(children,num_children,"Port", XlNselectable,0);
	XlSetSomething(children,num_children,"IconReg", XlNselectable,1);
	}
   else
        /*
		uscita dal connect mode
	*/
	{
	XlSetSomething(children,num_children,"Port",
                XlNconnectMode,CONNESSIONE_PORTE_NON_ATTIVA);
	XlSetSomething(children,num_children,NULL, XlNselectable,1);
	}
} 

trasla_conn(wid,xnew,ynew)
Widget wid;
int xnew,ynew;
{
int xold,yold,dx,dy;
int xfilo,yfilo;
Widget draw_wid; /* wid della drawing area */
get_posiz_porta(wid,&xold,&yold);
dx=xnew-xold;
dy=ynew-yold;
get_punto_porta(wid,&xfilo,&yfilo);
draw_wid=XtParent(XtParent(wid));
modifica_conn(draw_wid,xfilo,yfilo,dx,dy,TRASLA_CONN);
}
 
seleziona_conn(wid)
Widget wid;
{
int xfilo,yfilo;
get_punto_porta(wid,&xfilo,&yfilo);
DrawSelectConn(XtParent(XtParent(wid)),xfilo,yfilo);
}


resize_conn(wid,xnew,ynew)
Widget wid;
int xnew,ynew;
{
int xold,yold,dx,dy;
int xfilo,yfilo;
Widget draw_wid; /* wid della drawing area */
get_posiz_porta(wid,&xold,&yold);
dx=xnew-xold;
dy=ynew-yold;
get_punto_porta(wid,&xfilo,&yfilo);
draw_wid=XtParent(XtParent(wid));
modifica_conn(draw_wid,xfilo,yfilo,dx,dy,RESIZE_CONN);
}

/*
 Gestione delle connessioni nel caso sia stato eseguito
 un apply in resource editor per quanto riguarda le
 icone di regolazione.
 Vengono utilizzate 2 routines
 	apply_gest_conn1
	apply_gest_conn2
 richiamate rispettivamente prima di distruggere
 l'oggetto e dopo averlo ricreato
 in base alle risorse settate in ResourceEditor.
 apply_gest_conn1: memorizza la posizione delle porte e del punto 
 	di connessione prima dell'apply.
 apply_gest_conn2; effettua la differenza tra le posizione vecchie 
        e le nuove e modifica congruentemente le connessioni.
*/
typedef struct s_pos_porta {
		int x; /* posizione della porta rispetto all'origine
 			  della drawing area (punto di collegamento) */
		int y;
		} S_POS_PORTA;
S_POS_PORTA *p_pos_porta;

apply_gest_conn1(wid)
Widget wid; /* icon di regolazione */
{
WidgetList childcomp;
Cardinal num_childcomp;
int j,ind;

/* recupero i figli di XlComposite */
get_child(wid,&childcomp,&num_childcomp);

/*
 Si alloca le strutture per memorizzare le informazioni relative
 alle porte; il numero massimo di porte e' pari al numero dei figli
 del composite (IconReg).
 Viene allocato un vettore di strutture dimensionato a num_childcomp.
*/
p_pos_porta=(S_POS_PORTA *)XtCalloc(num_childcomp, sizeof(S_POS_PORTA));
ind=0;
for(j=0;j<num_childcomp;j++)
        {
         if(XlIsPort(childcomp[j]))
                {
		get_punto_porta(childcomp[j],&(p_pos_porta[ind].x),
					     &(p_pos_porta[ind].y));
		ind++;
		}
	}
}

apply_gest_conn2(wid)
Widget wid; /* icon di regolazione */
{
WidgetList childcomp;
Cardinal num_childcomp;
int j,ind;
int xnew,ynew,xold,yold;

/* recupero i figli di XlComposite */
get_child(wid,&childcomp,&num_childcomp);

ind=0;
for(j=0;j<num_childcomp;j++)
        {
         if(XlIsPort(childcomp[j]))
                {
		xold=p_pos_porta[ind].x;
		yold=p_pos_porta[ind].y;
		get_punto_porta(childcomp[j],&xnew,&ynew);
		modifica_conn(XtParent(wid),xold,yold,
				xnew-xold,ynew-yold,RESIZE_CONN);
		ind++;
		}
	}
manage_all_conn(XtParent(wid));
XtFree(p_pos_porta);
}


int select_connections(num_selected,selected,rubber,operazione)
int num_selected; /* numero di oggetti selezionati */
Widget *selected; /* lista degli oggetti selezionati per la move */
XRectangle *rubber; /* lista dei rettangoli corrispondenti alle posizioni
                       che i singoli oggetti andranno ad occupare dopo
                       la move */
int operazione;     /* indica la operazione di MOVE o COPY IN CLIPBOARD */
{
int i;
XRectangle rect_nullo;

rect_nullo.x=0;
rect_nullo.y=0;
rect_nullo.width=0;
rect_nullo.height=0;

for(i=0;i<num_selected;i++)
   {
   if( XlIsIconReg(selected[i]) )
      {
      if(rubber==NULL)
		select_conn_icon(selected[i],rect_nullo,operazione);
      else
      		select_conn_icon(selected[i],rubber[i],operazione);
      }
   }
/*
 terminato il trattamento per le icone di regolazione resetta
 a managed tutte le connessioni
*/
if(operazione == RECTMOVE && num_selected)
      manage_all_conn(XtParent(selected[0]));
}

select_conn_icon(selected,rubber,operazione)
Widget selected; /* icona di regolazione */
XRectangle rubber; /* rettangolo corrispondente alla posizione
                       che l'icona di regolazione andra' a occupare dopo
                       la move */
int operazione;     /* indica la operazione di MOVE o COPY IN CLIPBOARD */
{	
int i,j;
Arg arg[4];
Position xporta,yporta;
int xnewporta,ynewporta;
WidgetList childcomp;
Cardinal num_childcomp;
Widget wid_porta_conn;
Widget wid_icon_conn;
int tipo_porta;
char *nome_porta_conn;
nome_porta_conn=NULL;
/* recupero i figli di XlComposite */
get_child(selected,&childcomp,&num_childcomp);
for(j=0;j<num_childcomp;j++)
         {
/*
 la porta per avere una connessione non deve essere una porta
 di interfaccia
*/
	 if(XlIsPort(childcomp[j]) && !XlIsInterfacePort(childcomp[j]))
		{
/*
 esamina se la porta e' connessa
*/
		XtSetArg(arg[0],XlNportNameConnect,&nome_porta_conn);
    		XtGetValues(childcomp[j],arg,1);
/*
		printf("MOVE: porta %s connessa con %s \n",
			XtName(childcomp[j]),nome_porta_conn);
*/
		
		if(strlen(nome_porta_conn)!=0)
			{
/*
 ricava l'indice di widget dell'icona a cui e' connessa 
 la porta a partire dal nome della porta connessa e dallo
 indice di widget della drawing area
*/
			wid_icon_conn=get_wid_icon_conn(XtParent(selected),nome_porta_conn);
			if(wid_icon_conn==NULL)
				{
				printf("\n Regulation schema is corrupted");
				printf("\n Port %s not found", nome_porta_conn);
				continue;
				}
/*
 calcola la coordinata della porta all'interno dell'icona IconReg
*/

			if( operazione == RECTMOVE ) 
                            {
			       XtSetArg(arg[0],XmNx,&xporta);
			       XtSetArg(arg[1],XmNy,&yporta);
    		               XtGetValues(childcomp[j],arg,2);
			       xnewporta=(int)rubber.x+(int)xporta;
			       ynewporta=(int)rubber.y+(int)yporta;

			    }

/*
 esamina se l'iconReg cui si e' collegati e'
 anch'essa selezionata
*/
			if (iswidsel(wid_icon_conn))
				{
/*
 In caso di porta di ingresso effettua la traslazione della connessione
*/
        			XtSetArg(arg[0],XlNtipoPort,&tipo_porta);
        			XtGetValues(childcomp[j],arg,1);
				if((tipo_porta == INPUT_PORT_A)||
					(tipo_porta == INPUT_PORT_D))
					{
                                           if( operazione == RECTMOVE)
					      trasla_conn(childcomp[j], xnewporta,
				               ynewporta);
                                        else
                                           seleziona_conn(childcomp[j]);
/*
 se selezione
	seleziona conn
*/
					}
				}
			else
/*
 La porta destinazione non e' selezionata: viene quindi effettuato
 un resize della connessione (viene solo spostato il vertice della connessione
*/
				if( operazione == RECTMOVE)
					resize_conn(childcomp[j],
						    xnewporta,
			    		    	    ynewporta);
                                 
/*
 se selezione
	non fa nulla
*/
			}
			
		}
         }
}
 
/* 
  creazione topologia schemi di regolazione 
*/
int CompReg(PAGINA* pag, int macroblock)
{
int flag=0;         /* Compilazione di tutte la pagine */
char msg[FILENAME_MAX];

   if(compila_reg(pag,flag,macroblock)>=0) {
      strcpy(msg,"Compilation correct of the pag ");
      strcat(msg,pag->nomepag);
      if(PagGetInUse(pag)==False)
        strcat(msg," which is not in use");
   }
   else {
      strcpy(msg,"Error compilation of the pag ");
      strcat(msg,pag->nomepag);
   } 
   SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo",msg,
         NULL,False,NULL,False,NULL);
   strcat(msg,"\n");
   show_message(msg);
}


void destroy_conn()
{
   OlDestroyConn(conn_obj);
}

void save_conn()
{
   OlSaveConn(conn_obj);
}

void set_interface(Boolean Istate)
{
char file_conn[200]; /* Path name per Connessioni.reg */

#ifndef VMS
   strcpy(file_conn,XlGetenv(ENVPAG));
   strcat(file_conn,"/");
#else
   strcpy(file_conn,XlConvDirVms(XlGetenv(ENVPAG)));
#endif
   strcat(file_conn,"Connessioni.reg");

   if(Istate == True)
	conn_obj=OlCreateConn(file_conn);
   else
	{
	if(conn_obj != NULL)
		{
		OlSaveConn(conn_obj);
		OlDestroyConn(conn_obj);
		conn_obj=NULL;
		}
	}

   /*
	resetta lo stato di connessione delle porte
   */
   XlPortResetConnection();

   return;
}

/*
 Memorizza il collegamento tra due porte di interfaccia
*/
static Boolean CreoNuovaInterfaccia(Widget w1, Widget w2)
{
   extern PAGINA *GetPaginaFromWidget();
CONNESSIONE connessione;
PAGINA *pag_ing, *pag_usc;
OlFormObject form;
Widget w_ing,w_usc,wf_ing,wf_usc,port_usc,port_ing,icon_usc,icon_ing;
char *nome_porta_usc, *nome_porta_ing, *nome_var_ing, *nome_var_usc;
char *nome_mod_ing, *nome_mod_usc, *nome_tag_ing, *nome_tag_usc;
char *descr_usc, *descr_ing, *value;
WidgetList wlist;
Cardinal num;
Arg arg[10];
int tipo;
int i;
float valore_variabile;
char *da_liberare;
extern Boolean ContextModified;


/*
	identifico la porta di ingresso e di uscita
*/
XtSetArg(arg[0],XlNtipoPort,&tipo);
XtGetValues(w1,arg,1);
if((tipo == INPUT_PORT_A) || (tipo == INPUT_PORT_D))
	{ w_ing = w1; w_usc = w2; }
else
	{ w_ing = w2; w_usc = w1; }

/*
	ricavo il nome delle pagine (solo gli indici)
        cui le due interfaccie appartengono
*/
pag_ing = GetPaginaFromWidget(XtParent(XtParent(w_ing)));
pag_usc = GetPaginaFromWidget(XtParent(XtParent(w_usc)));

/*
	ricavo le porte corrispondenti alle porte di interfaccia e
	affacciate all'interno dello schema
        ricava le informazioni relative
*/
/*
 Ricava i figli della icona di regolazione (tipo interfaccia)
 Tra i figli individua la porta rivolta verso l'interno dello
 schema (per individuarla si cerca tra i figli l'oggetto che
 sia di tipo Porta e sia diverso dalla porta rivolta verso
 l'esterno).
*/
get_child(XtParent(w_ing),&wlist,&num);
for(i=0;i<num;i++)
	if(XlIsPort(wlist[i]) && (wlist[i] != w_ing))
		break;
if(i == num)
	{
	fprintf(stderr,
		"Errore nella costruzione dell'icona di interfaccia di ing.\n");
	return(False);
	}
/*
 individuata la porta di input lato interno allo schema
*/
wf_ing = wlist[i];
/*
 ricava il nome della porta connessa alla porta
 interna allo schema
*/
XtSetArg(arg[0],XlNportNameConnect,&nome_porta_ing);
XtGetValues(wf_ing,arg,1);
/*
 Se dal lato interno la porta non e' ancora collegata torna
 segnalando un errore
*/
if(nome_porta_ing == NULL || strlen(nome_porta_ing)==0)
	{
	fprintf(stderr,
		"Cannot connect Interface Port Not Internally Connected\n");
        SetMsg(topLevelShell,NULL,INFMSG,"Warning","Cannot connect Interface Port That Is Not Internally Connected",NULL,False,NULL,False,NULL);
	return(False);
	}
/*
 Ricava l'indice di widget della IconReg interna connessa e
 della porta che attua la connessione.
*/
icon_ing=get_wid_icon_conn(XtParent(XtParent(wf_ing)),nome_porta_ing);
port_ing=get_wid_port(icon_ing,nome_porta_ing);
/*
 Ricava nomi e tags relativi a icona e porta
*/
XtSetArg(arg[0],XlNportName,&nome_var_ing);
XtGetValues(port_ing,arg,1);
XtSetArg(arg[0],XlNmodulName,&nome_mod_ing);
XtSetArg(arg[1],XlNtagName,&nome_tag_ing);
XtSetArg(arg[2],XlNiconDescr,&descr_ing);
XtGetValues(icon_ing,arg,3);

/*
 Ricava i figli della icona di regolazione (tipo interfaccia)
 Tra i figli individua la porta rivolta verso l'esterno dello
 schema (per individuarla si cerca tra i figli l'oggetto che
 sia di tipo Porta e sia diverso dalla porta di interfaccia)
*/
get_child(XtParent(w_usc),&wlist,&num);
for(i=0;i<num;i++)
	if(XlIsPort(wlist[i]) && (wlist[i] != w_usc))
		break;
if(i == num)
	{
	fprintf(stderr,
                "Errore nella costruzione dell'icona di interfaccia di usc.\n");
	return(False);
	}
/*
 individuata la porta di output lato interno allo schema
*/
wf_usc = wlist[i];
/*
 Se dal lato interno la porta non e' ancora collegata torna
 segnalando un errore
*/
XtSetArg(arg[0],XlNportNameConnect,&nome_porta_usc);
XtGetValues(wf_usc,arg,1);
if(nome_porta_usc == NULL || strlen(nome_porta_usc)==0)
	{
	fprintf(stderr,
		"Cannot connect Interface Port Not Internally Connected\n");
        SetMsg(topLevelShell,NULL,INFMSG,"Warning","Cannot connect Interface Port That Is Not Internally Connected",NULL,False,NULL,False,NULL);
	return(False);
	}
icon_usc=get_wid_icon_conn(XtParent(XtParent(wf_usc)),nome_porta_usc);
port_usc=get_wid_port(icon_usc,nome_porta_usc);
XtSetArg(arg[0],XlNportName,&nome_var_usc);
XtGetValues(port_usc,arg,1);
XtSetArg(arg[0],XlNmodulName,&nome_mod_usc);
XtSetArg(arg[1],XlNtagName,&nome_tag_usc);
XtSetArg(arg[2],XlNiconDescr,&descr_usc);
XtSetArg(arg[3],XlNobjectForm,&form);
XtSetArg(arg[4],XlNinputValue,&value);
XtGetValues(icon_usc,arg,5);

/*
	estrae il valore
*/
da_liberare=EstrTagVal(form,OL_FORM_INPUT_VALUE,nome_var_usc,2,value);
if( da_liberare==NULL )
   {
   valore_variabile= 0.0;
   fprintf(stderr,"CreoNuovaInterfaccia: EstrTagVal restituisce valore NULL\n");
   return(False);
   }
else
   {
   valore_variabile= atof(da_liberare);
   XtFree(da_liberare);
   }



OlCreateStructConn( &connessione,
	pag_usc->nomepag,XtName(w_usc),XtName(XtParent(w_usc)),
	XtName(port_usc),XtName(icon_usc),nome_var_usc,nome_mod_ing,
	nome_tag_usc,descr_usc,
	pag_ing->nomepag,XtName(w_ing),XtName(XtParent(w_ing)),
	XtName(port_ing),XtName(icon_ing),nome_var_ing,nome_mod_usc,
	nome_tag_ing,descr_ing,valore_variabile);
/*
	inserisce la nuova connessione e registra la modifica
*/
ContextModified = True;
return(OlInsNewConn(conn_obj,connessione));
}

/*** Aggiornamenti del 28-10-94 per Cut/Copy/Paste IconReg ***/

/*-------------------------------------------
 Dato lo Widget di una IconReg torna la lista 
 e il numero delle sue porte
---------------------------------------------*/
Boolean getPortList(Widget Icon,WidgetList *Porte,Cardinal *nPorte)
{
   WidgetList listaChild;
   Cardinal nChild,numPorte;
   int i;

/* recupero la lista dei figli di IconReg */
   get_child(Icon,&listaChild,&nChild);

/* conto quante porte possiede */
   numPorte=0;

   for(i=0;i<nChild;i++)
      if(XlIsPort(listaChild[i]))
         numPorte++;

   if(!numPorte)
      return(False);

   Porte = XtMalloc( sizeof(Widget) * numPorte );

/* assegno il numero porte alla variabile di input */
   *nPorte = nChild;

/* assegno l'id di ciascuna porta al vettore di input */
   for(i=0;i<nChild;i++)
      if(XlIsPort(listaChild[i]))
         Porte[i]=listaChild[i];
 
   return(True);
}

             
/*---------------------------------------------------
 Restituisce come output la lista delle connessioni
 di tutte le porte della icona Icon
 e.s. la Icon 1w ha 3 Porte 1w1c,1w2c e 1w3c 
 ed ha le porte 1w2c e 1w3c connesse con

 2w1c e 3w2c rispettivamente la lista di ritorno sara'  
      1w2c 2w1c
      1w3c 3w2c
---------------------------------------------------*/
      
 
typedef struct {
                  char IconPortName[100];
                  char NamePortConn[100];
               } LISTACONN;
 
Boolean getListNamePortConnession(Widget Icon,LISTACONN listaConn[],int *nConn)
{
   WidgetList Port;
   Cardinal nPort;
   int i,numconn;
   char nome_porta_conn[100];
   Widget wid_icon_conn;

   if( XlIsIconReg(Icon) )
   {

/* inizializzo gli argomenti */
      listaConn = NULL;
      numconn = 0;

/* recupero la lista delle porte della IconReg */
      getPortList(Icon,&Port,&nPort);

      for(i=0;i<nPort;i++)
      {
/*
 la porta per avere una connessione non deve essere una porta
 di interfaccia
*/
	 if( XlIsPort(Port[i]) && !XlIsInterfacePort(Port[i]) )
	 {
/*
 esamina se la porta e' connessa
*/
            lnargs=0;
  	    XtSetArg(args[lnargs],XlNportNameConnect,&nome_porta_conn);lnargs++;
    	    XtGetValues(Port[i],args,lnargs);
		
	    if(strlen(nome_porta_conn)!=0)
     	    {
/*
 ricava l'indice di widget dell'icona a cui e' connessa 
 la porta a partire dal nome della porta connessa e dallo
 indice di widget della drawing area
*/
	       wid_icon_conn=get_wid_icon_conn( XtParent(Icon),nome_porta_conn);

               /* esamina se e' connessa */
	       if(wid_icon_conn==NULL)
	       {
			printf("\n Regulation schema is corrupted");
			printf("\n Port %s not found", nome_porta_conn);
			continue;
	       }
               else
               {
                  if( !numconn )
                    listaConn = alloca_memoria(1,sizeof(LISTACONN));
                  else
                    listaConn = XtRealloc( listaConn, (sizeof(LISTACONN ) * numconn) );    

                  strcpy( listaConn[i].IconPortName,XtName(Port[i]) );
                  strcpy( listaConn[i].NamePortConn,nome_porta_conn );
                  printf("\n\n\tPorta %s connessa con %s \n", listaConn[i].IconPortName, listaConn[i].NamePortConn);
                 numconn++;
                 *nConn = numconn;
               }
	    }
         }
      }
   }
}


Boolean WriteResource(PAGINA *pag)
{
XlWidgetResList lista;
int num_res;

XlGetResWidget(pag->drawing,&lista,&num_res);
XlPrintResWidget(lista,num_res,pag->nomepag);
XtFree(lista);
return(True);
}

XrmDatabase CompressDb=NULL;
Boolean CompressData(PAGINA *pag)
{
XrmDatabase app;
FILE *fp;

fp=fopen("Compress.pag","w");
if(fp==(FILE*)NULL)
	return(False);
pag_wgeometry(fp,pag);
fclose(fp);
CompressDb=XrmGetFileDatabase("Compress.pag");

dbcopy(&(pag->db),&CompressDb,pag->drawing);

/*
XrmDestroyDatabase(pag->db);
pag->db=CompressDb;
UxDisplay->db=pag->db;
*/

return(True);
}

/*
	a partire dalla drawing area prende i valori settati della
	visualizzazione degli oggetti di regolazione
*/
Boolean GetRegoDisplayMode(Widget drawing,int *dispReg, 
		int *port, int *interfacePort)
{
PAGINA *pag = NULL;

pag = GetPaginaFromWidget(drawing);

if(pag == NULL)
	return(False);

*dispReg = pag->dispreg_visual_mode;
*port = pag->port_visual_mode;
*interfacePort = pag->interfaceport_visual_mode;

return(True);
}
/*
	a partire dalla drawing area setta i valori per 
	visualizzazione degli oggetti di regolazione
*/
Boolean SetRegoDisplayMode(Widget drawing,int dispReg, 
		int port, int interfacePort)
{
PAGINA *pag = NULL;
WidgetList m_figli,figli,nipoti,m_nipoti;
Cardinal num_figli=0,num_nipoti=0;
int i,j;

pag = GetPaginaFromWidget(drawing);

if(pag == NULL)
	return(False);

get_child(drawing,&figli,&num_figli);
m_figli=(WidgetList)XtMalloc(sizeof(Widget)*num_figli);
memcpy(m_figli,figli,num_figli*sizeof(Widget));
for(i=0;i<num_figli;i++)
  {
  if(!XlIsIconReg(m_figli[i]))
	continue;
  get_child(m_figli[i],&nipoti,&num_nipoti);
  m_nipoti=(WidgetList)XtMalloc(sizeof(Widget)*num_nipoti);
  memcpy(m_nipoti,nipoti,num_nipoti*sizeof(Widget));
  for(j=0;j<num_nipoti;j++)
	{
	if((pag->dispreg_visual_mode != dispReg) && XlIsDispReg(m_nipoti[j]))
	    set_something(m_nipoti[j],XlNdispRegMode,(void*) dispReg);
	if((pag->port_visual_mode != port) && XlIsPort(m_nipoti[j]) && 
		!XlIsInterfacePort(m_nipoti[j]))
	    	set_something(m_nipoti[j],XlNvisibleMode,(void*) port);
	if((pag->interfaceport_visual_mode != interfacePort) &&  
		XlIsInterfacePort(m_nipoti[j]))
	    	set_something(m_nipoti[j],XlNvisibleMode,(void*) interfacePort);
	}
  XtFree(m_nipoti);
  }
XtFree(m_figli);

pag->dispreg_visual_mode = dispReg;
pag->port_visual_mode = port;
pag->interfaceport_visual_mode = interfacePort;

return(True);
}

/*
	setta su una porta o su un dispReg il visual mode
	settato nella pagina
*/
void SetSetMode(Widget w)
{
int dispReg,port,interfacePort,i;
Cardinal num_figli;
WidgetList figli;

if(!(XlIsIconReg(w)||XlIsDispReg(w)||XlIsPort(w)))
	return;

if(XlIsIconReg(w))
	{
	if(!GetRegoDisplayMode(XtParent(w),
		&dispReg,&port,&interfacePort))
	return;
	}
else
	{
	if(!GetRegoDisplayMode(XtParent(XtParent(w)),
		&dispReg,&port,&interfacePort))
	return;
	}

if(XlIsDispReg(w))
    set_something(w,XlNdispRegMode,(void*) dispReg);
if(XlIsPort(w) && !XlIsInterfacePort(w))
    	set_something(w,XlNvisibleMode,(void*) port);
if(XlIsInterfacePort(w))
    	set_something(w,XlNvisibleMode,(void*) interfacePort);
if(XlIsIconReg(w))
  {
  get_child(w,&figli,&num_figli);
  for(i=0;i<num_figli;i++)
	{
	if(XlIsDispReg(figli[i]))
    		set_something(figli[i],XlNdispRegMode,(void*) dispReg);
	if(XlIsPort(figli[i]) && !XlIsInterfacePort(figli[i]))
    		set_something(figli[i],XlNvisibleMode,(void*) port);
	if(XlIsInterfacePort(figli[i]))
    		set_something(figli[i],XlNvisibleMode,(void*) interfacePort);
	}
  }
}


/*
	scrive il template della std.lib
*/
void WriteTemplateStdLib(FILE *fp)
{
   fprintf(fp,"%s\n","*nomePag: std");
   fprintf(fp,"%s\n","*top_tipo: Libreria");
   fprintf(fp,"%s\n","*top_descrizione: Standard Library");
   fprintf(fp,"%s\n","*top_x: 10");
   fprintf(fp,"%s\n","*top_y: 10");
   fprintf(fp,"%s\n","*top_width: 400");
   fprintf(fp,"%s\n","*top_height: 900");
   fprintf(fp,"%s\n","*drawing_width:  900");
   fprintf(fp,"%s\n","*drawing_height: 900");
   fprintf(fp,"%s\n","*drawing_background: lime green");
   fprintf(fp,"%s\n","*tagPag: 00");
   fprintf(fp,"%s\n","*nextTagReg: 01");
   fprintf(fp,"%s\n","*numTagReg: 1");

   fprintf(fp,"%s\n","*elenco_wid0: 1w Indic 2w Bottone 3w Label 4w XlComposite 5w Bitmap 6w Led 7w SetValore 8w Icon 9w ChangePage 10w Grafico 11w Port 12w IconReg 13w DispReg 14w Selettore 15w Sincro 16w SincroOld 17w Cai 18w CaiVar 19w Allarmi 20w IndicTelep 21w OperableKeys 22w Pictograph 23w Pixvar");
   fprintf(fp,"%s\n","*num_widget: 23");
   fprintf(fp,"%s\n","*nextnum: 24");

   fprintf(fp,"%s\n","*1w.width0:  100");
   fprintf(fp,"%s\n","*1w.height0: 100");
   fprintf(fp,"%s\n","*1w.x0: 50");
   fprintf(fp,"%s\n","*1w.y0: 50");

   fprintf(fp,"%s\n","*2w.width0:  50");
   fprintf(fp,"%s\n","*2w.height0: 50");
   fprintf(fp,"%s\n","*2w.x0: 50");
   fprintf(fp,"%s\n","*2w.y0: 160");

   fprintf(fp,"%s\n","*3w.width0:  100");
   fprintf(fp,"%s\n","*3w.height0: 50");
   fprintf(fp,"%s\n","*3w.x0: 50");
   fprintf(fp,"%s\n","*3w.y0: 220");
   fprintf(fp,"%s\n","*3w.normalFont: courier_bold14");

   fprintf(fp,"%s\n","*4w.width0:  100");
   fprintf(fp,"%s\n","*4w.height0: 100");
   fprintf(fp,"%s\n","*4w.x0: 50");
   fprintf(fp,"%s\n","*4w.y0: 280");
   fprintf(fp,"%s\n","*4w.numFigli: 0");
   fprintf(fp,"%s\n","*4w.background: gray");
   fprintf(fp,"%s\n","*4w.inheritBackground: 0");

   fprintf(fp,"%s\n","*5w.width0:  100");
   fprintf(fp,"%s\n","*5w.height0: 100");
   fprintf(fp,"%s\n","*5w.x0: 50");
   fprintf(fp,"%s\n","*5w.y0: 390");

   fprintf(fp,"%s\n","*6w.width0:  50");
   fprintf(fp,"%s\n","*6w.height0: 30");
   fprintf(fp,"%s\n","*6w.x0: 50");
   fprintf(fp,"%s\n","*6w.y0: 500");

   fprintf(fp,"%s\n","*7w.width0:  80");
   fprintf(fp,"%s\n","*7w.height0: 30");
   fprintf(fp,"%s\n","*7w.x0: 50");
   fprintf(fp,"%s\n","*7w.y0: 600");

   fprintf(fp,"%s\n","*8w.width0:  80");
   fprintf(fp,"%s\n","*8w.height0: 30");
   fprintf(fp,"%s\n","*8w.background: orange");
   fprintf(fp,"%s\n","*8w.x0: 50");
   fprintf(fp,"%s\n","*8w.y0: 650");
   fprintf(fp,"%s\n","*8w.nomeFile: valv");

   fprintf(fp,"%s\n","*9w.x0: 50");
   fprintf(fp,"%s\n","*9w.y0: 750");

   fprintf(fp,"%s\n","*10w.width0:  150");
   fprintf(fp,"%s\n","*10w.height0: 130");
   fprintf(fp,"%s\n","*10w.x0: 50");
   fprintf(fp,"%s\n","*10w.y0: 810");

   fprintf(fp,"%s\n","*11w.width0:  15");
   fprintf(fp,"%s\n","*11w.height0: 15");
   fprintf(fp,"%s\n","*11w.x0: 200");
   fprintf(fp,"%s\n","*11w.y0: 50");

   fprintf(fp,"%s\n","*12w.width0:  50");
   fprintf(fp,"%s\n","*12w.height0: 50");
   fprintf(fp,"%s\n","*12w.x0: 200");
   fprintf(fp,"%s\n","*12w.y0: 100");

   fprintf(fp,"%s\n","*13w.width0:  50");
   fprintf(fp,"%s\n","*13w.height0: 30");
   fprintf(fp,"%s\n","*13w.x0: 250");
   fprintf(fp,"%s\n","*13w.y0: 50");

   fprintf(fp,"%s\n","*14w.width0:  30");
   fprintf(fp,"%s\n","*14w.height0: 30");
   fprintf(fp,"%s\n","*14w.x0: 250");
   fprintf(fp,"%s\n","*14w.y0: 600");

   fprintf(fp,"%s\n","*15w.width0:  100");
   fprintf(fp,"%s\n","*15w.height0: 100");
   fprintf(fp,"%s\n","*15w.x0: 190");
   fprintf(fp,"%s\n","*15w.y0: 340");
   
   fprintf(fp,"%s\n","*16w.width0:  100");
   fprintf(fp,"%s\n","*16w.height0: 100");
   fprintf(fp,"%s\n","*16w.x0: 190");
   fprintf(fp,"%s\n","*16w.y0: 455");

   fprintf(fp,"%s\n","*17w.width0:  50");
   fprintf(fp,"%s\n","*17w.height0: 30");
   fprintf(fp,"%s\n","*17w.x0: 50");
   fprintf(fp,"%s\n","*17w.y0: 550");
   fprintf(fp,"%s\n","*17w.tipoCai: 1");
   fprintf(fp,"%s\n","*17w.foreAllTypeA:      #ff0000000000");

   fprintf(fp,"%s\n","*18w.width0:  50");
   fprintf(fp,"%s\n","*18w.height0: 30");
   fprintf(fp,"%s\n","*18w.x0: 120");
   fprintf(fp,"%s\n","*18w.y0: 550");
   fprintf(fp,"%s\n","*18w.foreAllTypeA:      #ff0000000000");
   
   fprintf(fp,"%s\n","*19w.width0:  50");
   fprintf(fp,"%s\n","*19w.height0: 50");
   fprintf(fp,"%s\n","*19w.x0: 316");
   fprintf(fp,"%s\n","*19w.y0: 340");
   fprintf(fp,"%s\n","*19w.background:      #c9c9c9c9c9c9");

   fprintf(fp,"%s\n","*20w.width0:  50");
   fprintf(fp,"%s\n","*20w.height0: 50");
   fprintf(fp,"%s\n","*20w.x0: 190");
   fprintf(fp,"%s\n","*20w.y0: 650");
   fprintf(fp,"%s\n","*20w.background:      #c9c9c9c9c9c9");
   
   fprintf(fp,"%s\n","*20w.width0:  50");
   fprintf(fp,"%s\n","*20w.height0: 50");
   fprintf(fp,"%s\n","*20w.x0: 190");
   fprintf(fp,"%s\n","*20w.y0: 280");
   fprintf(fp,"%s\n","*20w.background:      #c9c9c9c9c9c9");
	
   fprintf(fp,"%s\n","*21w.width0:  30");
   fprintf(fp,"%s\n","*21w.height0: 30");
   fprintf(fp,"%s\n","*21w.x0: 320");
   fprintf(fp,"%s\n","*21w.y0: 50");
   fprintf(fp,"%s\n","*21w.background:      #c9c9c9c9c9c9");

   fprintf(fp,"%s\n","*22w.width0:  30");
   fprintf(fp,"%s\n","*22w.height0: 30");
   fprintf(fp,"%s\n","*22w.x0: 300");
   fprintf(fp,"%s\n","*22w.y0: 220");
   fprintf(fp,"%s\n","*22w.background:      #c9c9c9c9c9c9");

   fprintf(fp,"%s\n","*23w.width0:  100");
   fprintf(fp,"%s\n","*23w.height0: 30");
   fprintf(fp,"%s\n","*23w.x0: 500");
   fprintf(fp,"%s\n","*23w.y0: 50");
   fprintf(fp,"%s\n","*23w.background:      #c9c9c9c9c9c9");
   fprintf(fp,"%s\n","*23w.nomePix1: pixvar");
   fprintf(fp,"%s\n","*23w.pixType1: 1");
}

Widget PrintSetup(Widget cw)
{
#if defined XPRINTER_USED && !defined LINUX
return(XlPrintSetup(cw));
#else
return(NULL);
#endif
}
   
Boolean  PrintPag(Widget cw)
{
#if defined XPRINTER_USED && !defined LINUX
XlPrintOpen(cw);
print_callback(cw);
XlPrint(cw);
XlPrintClose();
return(True);
#else
return(False);
#endif
}
