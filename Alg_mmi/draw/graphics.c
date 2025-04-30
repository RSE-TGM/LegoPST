/**********************************************************************
*
*       C Source:               graphics.c
*       Subsystem:              2
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed Feb 14 15:13:04 2007 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: graphics.c-10 %  (%full_filespec: graphics.c-10:csrc:2 %)";
#endif

#include <stdio.h>

#ifdef sco
#include <stdlib.h>
#endif

#include <math.h>
#include <ctype.h>
#include <X11/Intrinsic.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <Xm/Xm.h>
#include <Xm/DrawingA.h>

#include <Xd/XdCore.h>
#include <Xd/XdConn.h>
#include <Xd/XdLine.h>
#include <Xd/XdRect.h>
#include <Xd/XdCircle.h>
#include <Xd/XdPoli.h>
#include <Xd/Xd.h>
#include <Xd/XdListaP.h>
#include <Xd/XdUndoP.h>
#include "libutilx.h"

/*
 inclusioni dei files di bitmap per i bottoni di comando
*/
#include "arco.bm"
#include "circle.bm"
#include "freccia.bm"
#include "freehand.bm"
#include "line.bm"
#include "poli.bm"
#include "rectangle.bm"
#include "zoom.bm" 
#include "draw.h"

#define COMPRESSIONE 1

extern Display		*UxDisplay;
extern int		UxScreen;
extern Window		UxRootWindow;
extern GC		UxDrawGC;


static GC		drawGC;
static Font		curfont;
static XFontStruct	*fontinfo;
static Draget dr_corrente;
static Draget *dr_list_selected;
static int dr_num_selected;
static XPoint		first,
			prev;

static Boolean		is_first, drawtext;
static char		text[80];
static int		textlen= 0;
extern XdLista get_lista();
extern XdLista get_lista_undo();
extern XdListaUndo get_lista_liste_undo();
extern void CreaUndoList();
extern void XdStartMove(Draget);
extern void XdWriteDraget(Draget,FILE *);
extern void XdPrintDraget(Draget);
void XdGetSize(Draget,int *,int*,int*,int*);
void XdModify(Draget,int,int,int,int,XEvent *);
extern void XdDestroyDraget(Draget);



#ifdef ON_PAGEDIT
extern XdLista clip_get_lista();
#endif
void CreaClipboardList();
/* void XdUndoDelete(); */
/*
 Liste per gestire la memorizzazione in clipboard dei draget
*/
static XdLista clip_lista_draget;

/*
 Flags utilizzati per la gestione dell'undo
*/
static int undo_flag=1, undo_move_flag=0;

void		DrawDelete(), 
		MoveTasti(),
		FirstPointLine(), 
		FirstPointRect(), 
		FirstPointCircle(), 
		FirstPointArc(), 
		FirstPointPoli(), 
		SecondPointConn(), 
		EndDrawConn(), 
		DrawDraget(), EndDrawDraget(),
		StartPick(), MovePick(), EndPick(),
		ExtPick(),
		UxDrawText(), UxNextLine(),
		UxLastPoint(), UxFirstText(), UxToggleFontCursor();
		
static void	erase_text();

void UxInitGraphics()
{
	extern XtAppContext	UxAppContext;
	Window root_win;

	static XtActionsRec  actions[]=
		{	
			{"draw_delete",		DrawDelete},
			{"move_tasti",		MoveTasti},
			{"start_pick", 		StartPick},
			{"move_pick",		MovePick},
			{"end_pick",		EndPick},
			{"ext_pick",		ExtPick},
			{"first_point_line",	FirstPointLine	},
			{"first_point_rect",	FirstPointRect	},
			{"first_point_circle",	FirstPointCircle},
			{"first_point_arc",	FirstPointArc},
			{"first_point_poli",	FirstPointPoli},
			{"second_point_conn",	SecondPointConn},
			{"draw_draget",		DrawDraget	},
			{"end_draw_draget",	EndDrawDraget	},
			{"end_draw_conn",	EndDrawConn	},
			{"first_text",		UxFirstText	},
			{"draw_text",		UxDrawText	},
			{"next_line",		UxNextLine	},
			{"last_point",		UxLastPoint	}
		};
   
	root_win= XRootWindow(UxDisplay,UxScreen);
	drawGC= XCreateGC(UxDisplay, root_win, 0, NULL);
	XSetFunction(UxDisplay, drawGC, GXxor);
	XSetForeground(UxDisplay, drawGC, 0xfc);

	UxSetDrawFont("fixed", NULL);

	XtAppAddActions(UxAppContext, actions, 15);
	create_ClipBoardUndo();
}

void CreaClipboardList()
{
clip_lista_draget=XdCreateLista();
}

extern Widget ClipBoard, ClipBoardUndo;


XdLista clip_get_lista()
{
return(clip_lista_draget);
}

void clip_set_lista(XdLista l)
{
clip_lista_draget = l;
}


GC CreaSfondoGC(wid)
Widget wid;
{
Pixel pix_sfondo;
unsigned long valuemask= GCForeground | GCSubwindowMode; 
XGCValues values;
get_something(wid, XmNbackground, (void*) &pix_sfondo);
values.foreground = pix_sfondo;
values.subwindow_mode= IncludeInferiors;
return(XtGetGC (wid, valuemask, &values));
}


/*
 definizione funzione di redisegno
*/
static void disegna_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
XRectangle rect;
static Boolean first=True;
extern Draget ListDragetNext();
Region region=NULL;
Draget dr;
#ifdef COMPRESSIONE
/*
 Gestione della compressione degli eventi di redraw
*/
if((region=get_region(w))==NULL)
        set_region(w,XCreateRegion());
region=get_region(w);
XtAddExposureToRegion(str->event,region);
set_region(w,region);


if(str->event->xexpose.count==0)
   {
   XClipBox(region,&rect);
   str->event->xexpose.x=rect.x;
   str->event->xexpose.y=rect.y;
   str->event->xexpose.width=rect.width;
   str->event->xexpose.height=rect.height;
#endif
/*
 Inizio codice CAPPE!!!
*/
if(undo_flag == 1) {
CreaUndoList(w);
undo_draget_copy(w, ClipBoardUndo);
undoListAdd(get_lista_liste_undo(w), get_lista_undo(w));
undo_flag = 0;
}
/*
 Fine codice CAPPE
*/
/* 
 si posiziona all'inizio della lista dei draget
*/
if(get_grid_on(w))  /* se richiesto disegna la griglia di riferimento */
	DrawGrid(w);
ListDragetRew(get_lista(w));
while((dr=ListDragetNext(get_lista(w))) != NULL)
	{
	XdDrawDraget(dr,str->event);
	}

#if defined ON_PAGEDIT
/*
 Nel caso di disegno di background e' necessario ridisegnare 
 i draget (altrimenti in caso di ridisegno parziale del background 
 nei casi di selezione di elementi di disegno il background va a
 coprire gli oggetti che dovrebbero costituire il foreground
*/
{
WidgetList children;
int i;
int rect_pos;
Cardinal  num_children=0;
Position x,y;
Dimension width,height;
/*
 esamina se qualche oggetto Xl si trova nella regione di ridisegno
*/
get_child(w,&children,&num_children);
for(i=0;i<num_children;i++)
   {
   get_geometry(children[i],&x,&y,&width,&height);
   if(XdIntersectRectangle(x,y,width,height,
		rect.x,rect.y,rect.width,rect.height))
/*
 effettua una clear area sull'oggetto per forzarne il ridisegno
*/
	XlRedisplay(children[i]);
   }
}
#endif

#ifdef COMPRESSIONE
   XDestroyRegion((region));
   (region)=NULL;
   set_region(w,region);
   }
#endif

}

/*
 definizione funzione di print
*/
#ifdef XPRINTER_USED
print_callback(w)
Widget w;
{
Draget dr;
ListDragetRew(get_lista(w));
while((dr=ListDragetNext(get_lista(w))) != NULL)
	{
	XdPrintDraget(dr);
	}
}
#endif






void DrawGrid(wid)
Widget wid;
{
GC copyGC;
int i,j;
int step;
Dimension height,width;
XPoint *points;
int num_x,num_y,num_points;
copyGC= XdGetCopyGC(wid);
/*
 considera uno step della griglia pari
 a quello dello snap
*/
step= get_step(wid);

/*
 rende piu' grande la griglia nel caso di step 
 piccoli
*/
if (step == 1)
	step=4;
else if(step< 4)
	step= step*2;
/*
 tiene conto del fattore di zoom
*/
step= step * get_def_zoom(wid);
/*
 Ricava le dimensioni della pagina grafica
*/
get_something(wid, XmNheight, (void*) &height);
get_something(wid, XmNwidth, (void*) &width);
num_x=width/step;
num_y=height/step;
num_points= num_x * num_y;
points=(XPoint *) XtCalloc(num_points,sizeof(XPoint));
for(j=0; j<num_y; j++)
	{
	for(i=0;i<num_x;i++)
		{
		points[j*num_x+i].x= i*step;
		points[j*num_x+i].y= j*step;
		}
	}
XSetClipMask(UxDisplay,copyGC,None);
XDrawPoints(UxDisplay,XtWindow(wid),copyGC,points,num_points,CoordModeOrigin);
XtFree((char *)points);
}

void DrawSetSnap(wid,snap)
Widget wid;
int snap;
{
static Boolean bFirst=True;
extern Draget ListDragetNext();
Draget dr;
/*
 si posiziona all'inizio della lista dei draget
*/
ListDragetRew(get_lista(wid));
while((dr=ListDragetNext(get_lista(wid))) != NULL)
        {
        if(dr!=NULL)
		XdSetSnap(dr,snap);
	}
set_step(wid,snap);
/*
 se e' settata la griglia ne forza
 il ridisegno
*/
if(get_grid_on(wid) && !bFirst)
	XClearArea(XtDisplay(wid),XtWindow(wid),0,0,0,0,True);
	
bFirst=False;
}

DrawSetGrid(wid)
Widget wid;
{
if(get_grid_on(wid) == 0)   /* setta la griglia e pone a 1 grid on */
	set_grid_on(wid,1);
else
	set_grid_on(wid,0);
/*
 Forza un ridisegno della window
*/
XClearArea(XtDisplay(wid),XtWindow(wid),0,0,0,0,True);
}


/*
 appende la funzione di ridisegno alla drawing area su cui si sta
 operando
*/
append_redraw(wid)
Widget wid;
{
XtAddCallback(wid,XmNexposeCallback,disegna_callback,NULL);
}


void UxFirstPoint(wid, ev)
	Widget		wid;
	XEvent		*ev;
{
	if(drawtext == True)
		UxToggleFontCursor(wid);

	first.x= ev->xbutton.x;
	first.y= ev->xbutton.y;

	is_first= True;
	drawtext= False;
	textlen= 0;
}



void FirstPointLine(wid,ev)
Widget wid;
XEvent *ev;
{
Draget dr;
int curr_step;
float def_zoom;
Boolean def_filled;
def_filled=(Boolean)get_def_filled(wid);
def_zoom=get_def_zoom(wid);
curr_step=get_step(wid);
reset_bott_def(wid);

/*
 Creazione di un Draget Linea
*/
XtGrabPointer(wid,False,ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
		      GrabModeAsync,GrabModeAsync,XtWindow(wid),
                      None,CurrentTime);
dr=XdCreateLineDraget(wid,get_def_gc(wid), get_def_gc_bg(wid));
XdSetFilled(dr,def_filled);
XdSetZoom(dr,def_zoom, True);
XdSetSnap(dr,curr_step);
dr_corrente=dr;
ListDragetAdd(get_lista(wid),dr);
XdFirstPoint(dr,ev);
}



void FirstPointPoli(wid,ev)
Widget wid;
XEvent *ev;
{
Draget dr;
int curr_step;
float def_zoom;
Boolean def_filled;
def_filled=(Boolean)get_def_filled(wid);
def_zoom=get_def_zoom(wid);
curr_step=get_step(wid);
reset_bott_def(wid);
/*
 Se non e' definito il draget corrente allora
 siamo alla creazione della polilinea altrimenti stiamo
 disegnando i segmenti successivi.
*/
if(dr_corrente == NULL)
	{
	XtGrabPointer(wid,False,ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
		      GrabModeAsync,GrabModeAsync,XtWindow(wid),
                      None,CurrentTime);
	dr=XdCreatePoliDraget(wid,get_def_gc(wid),get_def_gc_bg(wid));
	XdSetFilled(dr,def_filled);
	XdSetZoom(dr,def_zoom, True);
	XdSetSnap(dr,curr_step);
	dr_corrente=dr;
	ListDragetAdd(get_lista(wid),dr);
	}
XdFirstPoint(dr_corrente,ev);
}



void FirstPointConn(wid,ev,gc)
Widget wid;
XEvent *ev;
GC gc;
{
Draget dr;
GC gcblack;
int curr_step;
float def_zoom;
Boolean def_filled;
def_filled=(Boolean)get_def_filled(wid);
def_zoom=get_def_zoom(wid);
curr_step=get_step(wid);
/*
 Se non e' definito il draget corrente allora
 siamo alla creazione della polilinea altrimenti stiamo
 disegnando i segmenti successivi.
*/
if(dr_corrente == NULL)
	{
/*
	XtGrabPointer(wid,False,ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
		      GrabModeAsync,GrabModeAsync,XtWindow(wid),
                      None,CurrentTime);
*/
	dr=XdCreateConnDraget(wid,gc,gc);
	XdSetFilled(dr,False);
	XdSetZoom(dr,def_zoom,True);
	XdSetSnap(dr,curr_step);
	dr_corrente=dr;
	ListDragetAdd(get_lista(wid),dr);
	}
XdFirstPoint(dr_corrente,ev);
}



void SecondPointConn(wid,ev)
Widget wid;
XEvent *ev;
{
XdFirstPoint(dr_corrente,ev);
}



void FirstPointRect(wid,ev)
Widget wid;
XEvent *ev;
{
Draget dr;
int curr_step;
float def_zoom;
Boolean def_filled;
def_filled=(Boolean)get_def_filled(wid);
def_zoom=get_def_zoom(wid);
curr_step=get_step(wid);
reset_bott_def(wid);

/*
 Creazione di un Draget Linea
*/
XtGrabPointer(wid,False,ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
		      GrabModeAsync,GrabModeAsync,XtWindow(wid),
                      None,CurrentTime);
dr=XdCreateRectDraget(wid,get_def_gc(wid),get_def_gc_bg(wid));
XdSetFilled(dr,def_filled);
XdSetZoom(dr,def_zoom,True);
XdSetSnap(dr,curr_step);
dr_corrente=dr;
ListDragetAdd(get_lista(wid),dr);
XdFirstPoint(dr,ev);
}



void FirstPointCircle(wid,ev)
Widget wid;
XEvent *ev;
{
Draget dr;
int curr_step;
float def_zoom;
Boolean def_filled;
def_filled=(Boolean)get_def_filled(wid);
def_zoom=get_def_zoom(wid);
curr_step=get_step(wid);
reset_bott_def(wid);

/*
 Creazione di un Draget Cerchio
*/
XtGrabPointer(wid,False,ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
		      GrabModeAsync,GrabModeAsync,XtWindow(wid),
                      None,CurrentTime);
dr=XdCreateCircleDraget(wid,get_def_gc(wid),get_def_gc_bg(wid));
XdSetFilled(dr,def_filled);
XdSetZoom(dr,def_zoom,True);
XdSetSnap(dr,curr_step);
dr_corrente=dr;
ListDragetAdd(get_lista(wid),dr);
XdFirstPoint(dr,ev);
}



void FirstPointArc(wid,ev)
Widget wid;
XEvent *ev;
{
Draget dr;
int curr_step;
float def_zoom;
Boolean def_filled;
def_filled=(Boolean)get_def_filled(wid);
def_zoom=get_def_zoom(wid);
curr_step=get_step(wid);
reset_bott_def(wid);

/*
 Creazione di un Draget Ellisse
*/
XtGrabPointer(wid,False,ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
		      GrabModeAsync,GrabModeAsync,XtWindow(wid),
                      None,CurrentTime);
dr=XdCreateArcDraget(wid,get_def_gc(wid),get_def_gc_bg(wid));
XdSetFilled(dr,def_filled);
XdSetZoom(dr,def_zoom,True);
XdSetSnap(dr,curr_step);
dr_corrente=dr;
ListDragetAdd(get_lista(wid),dr);
XdFirstPoint(dr,ev);
}



void DrawDraget(wid,ev)
Widget wid;
XEvent *ev;
{
if(dr_corrente)
	{
	XdFirstDraw(dr_corrente,ev);
	}
}

void EndDrawDraget(wid,ev)
Widget wid;
XEvent *ev;
{
if(dr_corrente == NULL) return;
XtUngrabPointer(wid,CurrentTime);
if(XdLastPoint(dr_corrente,ev)==False)
	{
/*
	Se XdLastPoint da come ritorno False significa che 
	l'oggetto grafico non e' stato instanziato completamente
 	(per esempio se i punti sono coincidenti, quindi 
	l'oggetto e' di dimensione nulla).
*/
	ListDeleteDraget(get_lista(wid),dr_corrente);
	XdDestroyDraget(dr_corrente);
/*
	Riporta Draw nella modalita' puntatore per selezione
*/
	set_arrow_mode(wid);
	}
	else {
	
	/*
 	 Inizio codice CAPPE
	*/
	CreaUndoList(wid);
	undo_draget_copy(wid, ClipBoardUndo);
	undoListAdd(get_lista_liste_undo(wid),get_lista_undo(wid));
	/*
 	 Fine codice CAPPE
	*/
	}
dr_corrente=NULL;
dr_num_selected=0;
}

void EndDrawConn(wid,ev)
Widget wid;
XEvent *ev;
{
#ifdef ON_PAGEDIT
extern Boolean is_drawing_background();
#endif
if(dr_corrente == NULL) return;
XtUngrabPointer(wid,CurrentTime);
if(XdLastPoint(dr_corrente,ev)==False)
	{
/*
	Se XdLastPoint da come ritorno False significa che 
	l'oggetto grafico non e' stato instanziato completamente
 	(per esempio se i punti sono coincidenti, quindi 
	l'oggetto e' di dimensione nulla).
*/
	ListDeleteDraget(get_lista(wid),dr_corrente);
	XdDestroyDraget(dr_corrente);
/*
 Resetta il campo di connessione in corso nella classe delle
 porte
*/
	XlPortResetConnection();
	}
/*
        Riporta Draw nella modalita' puntatore per selezione
*/
set_draw_translations(wid);

#ifdef ON_PAGEDIT
        aggiungi_pagedit_translation(wid);
/*
 Se e' attivo l'editing del background setta il bottone
 di draw per la selezione
*/
	if(is_drawing_background(wid))
		set_arrow_mode(wid);
#endif

dr_corrente=NULL;
dr_num_selected=0;
}



void StartPageditMove(wid)
Widget wid;
{
int i;
/*
 richiede la lista degli oggetti selezionati
*/
dr_num_selected=ListGetSelected(get_lista(wid),&dr_list_selected);
/*
  abilita la funzione di move su tutti gli oggetti selezionati
*/
if(dr_num_selected > 1)
        {
	XtGrabPointer(wid,False,ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
		      GrabModeAsync,GrabModeAsync,XtWindow(wid),
                      None,CurrentTime);
        for(i=0; i<dr_num_selected; i++)
                XdStartMove(dr_list_selected[i]);
        return;
        }
else if (dr_num_selected == 1)
	{
	XtGrabPointer(wid,False,ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
		      GrabModeAsync,GrabModeAsync,XtWindow(wid),
                      None,CurrentTime);
	dr_corrente=dr_list_selected[0];
	XdStartMove(dr_corrente);
	XtFree((char *)dr_list_selected);
	}

}

/*
 Selezione rettangolare
*/
XRectangle draw_selection(Widget wid,XEvent *eve)
{
 XRectangle area;
 GC xorGC;
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
/* inizializzazione variabili */
   xprec = x1 = eve->xbutton.x;
   yprec = y1 = eve->xbutton.y;
   xorGC= XdGetXorGC(wid);
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

   rett.x = (int)xcorner;
   rett.y = (int)ycorner;
   rett.width= (int)w;
   rett.height= (int)h;
return(rett);

}


void StartPick(wid,ev)
Widget wid;
XEvent *ev;
{
int i;
Draget dr;
Boolean selected; /* indica che uno degli oggetti e' stato selezionato */
XRectangle rect;
extern XRectangle pagedit_selection(); 
#ifdef ON_PAGEDIT
extern void pag_deselect_all();
#endif

/*
 esamina se il click e' avvenuto su uno degli oggetti selezionati
*/
/* 
 richiede la lista degli oggetti selezionati
*/
dr_num_selected=ListGetSelected(get_lista(wid),&dr_list_selected);
selected= False;
for(i=0; i<dr_num_selected; i++)
	{
/*
 la routine di pick viene richiamata in modalita' estesa perche si
 desidera che venga preservata la selezione multipla
*/
	if(XdPickExtDraget(dr_list_selected[i],ev) == True)
		{
		selected=True;
		break;
		}
        }

/*
  se e' stata riscontrato click su un oggetto gia' in selezione
  abilita la funzione di move su tutti gli oggetti selezionati
*/
if (selected == True && dr_num_selected > 1)
	{
	XtGrabPointer(wid,False,ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
		      GrabModeAsync,GrabModeAsync,XtWindow(wid),
                      None,CurrentTime);
	for(i=0; i<dr_num_selected; i++)
		XdStartMove(dr_list_selected[i]);        	
	return;
	}

/* 
 si posiziona all'inizio della lista dei draget
*/
if(dr_num_selected) ListSetPosByDraget(get_lista(wid),dr_list_selected[0]);
else ListDragetRew(get_lista(wid));

dr_num_selected=0;
selected= False;
while((dr=ListDragetNext(get_lista(wid))) != NULL)
	{
	if(selected == False)
		{
		if(XdPickDraget(dr,ev) == True)
			{
			dr_num_selected=1;
			dr_corrente=dr;
			selected= True;
			}
		else
			XdDeselectDraget(dr,True);
		}
	else 
		XdDeselectDraget(dr,True);
	
	}
#ifdef ON_PAGEDIT
/*
 deseleziona tutti i widget 
*/
pag_deselect_all(wid);
#endif
rect.width=0;
if(dr_num_selected == 0)
	{
#ifdef ON_PAGEDIT
	rect=pagedit_selection(wid,ev);
#else
	rect=draw_selection(wid,ev);
#endif
	}
if(rect.width != 0)
	SelectDragetRect(wid,&rect);
}

/*
 Deseleziona tutti i draget presenti sulla pagina
*/
void DeselectAllDraget(wid, do_expose)
Widget wid;
Boolean do_expose;
{
Draget dr;
ListDragetRew(get_lista(wid));
while((dr=ListDragetNext(get_lista(wid))) != NULL)
	{
	if(XdIsSelected(dr))
		XdDeselectDraget(dr,do_expose);
	}
}


/*
 Scrittura di tutto il disegno su file
*/
WriteBackground(wid,nome_file)
Widget wid;
char *nome_file;
{
int xmin,ymin,xmax,ymax;
int xm,ym,xM,yM;
Draget dr;
FILE *fp;
#ifdef VMS
char nome_file_vms[1024];
strcpy(nome_file_vms,XlConvPathVms(nome_file));
fp=fopen(nome_file_vms, "w+");
#else
fp=fopen(nome_file, "w+"); 
if (fp== NULL)
{
perror("Stai cercando di scrivere su un file protetto");
exit(7);
}
#endif
/*
 calcola il punto minimo e massimo del disegno
*/
xmin=ymin=10000;
xmax=ymax=-10000;
ListDragetRew(get_lista(wid));
while((dr=ListDragetNext(get_lista(wid))) != NULL)
	{
	XdGetSize(dr,&xm,&ym,&xM,&yM);
	xmin=(xmin < xm) ? xmin: xm;
	ymin=(ymin < ym) ? ymin: ym;
	xmax=(xmax > xM) ? xmax: xM;
	ymax=(ymax > yM) ? ymax: yM;
	}
/*
 scrive i punti minimo e massimo del disegno
*/
fprintf(fp,"x_min_d %d\n", xmin);
fprintf(fp,"y_min_d %d\n", ymin);
fprintf(fp,"x_max_d %d\n", xmax);
fprintf(fp,"y_max_d %d\n", ymax);
/*
 scrive il numero di oggetti contenuti
*/
fprintf(fp,"num_d %d\n",ListGetNumAllDraget(get_lista(wid)));
/*
 scrive i singoli draget contenuti
*/
ListDragetRew(get_lista(wid));
while((dr=ListDragetNext(get_lista(wid))) != NULL)
                XdWriteDraget(dr,fp);
fclose(fp);
}

/*
 Lettura del disegno da file
*/
ReadBackground(wid,nome_file)
Widget wid;
char *nome_file;
{
int xmin,ymin,xmax,ymax;
char buf[10];
int numero_draget;
Draget dr;
XdGC xdgc;
FILE *fp;
fp=fopen(nome_file, "r");
if(fp==NULL) return;
/*
 legge i punti minimo e massimo del disegno
*/
fscanf(fp,"%s %d\n",buf,&xmin);
fscanf(fp,"%s %d\n",buf,&ymin);
fscanf(fp,"%s %d\n",buf,&xmax);
fscanf(fp,"%s %d\n",buf,&ymax);
fscanf(fp,"%s %d",buf,&numero_draget);
while((dr=XdReadRecord(wid,&xdgc,fp))!=NULL)
	{
        ListDragetAdd(get_lista(wid),dr);
	XdSetSnap(dr,get_step(wid));
	}
fclose(fp);
/*
	Porta Draw nella modalita' puntatore per selezione
*/
/*
set_arrow_mode(wid);
*/
}


SelectDragetRect(wid,rect)
Widget wid;
XRectangle *rect;
{
Draget dr;
ListDragetRew(get_lista(wid));
while((dr=ListDragetNext(get_lista(wid))) != NULL)
	{
	if(XdDragetInRect(dr,rect))
		XdSelectDraget(dr,True);
	}
}

DrawSetZoom(draw_wid,zoom)
Widget draw_wid;
float zoom;
{
Draget dr;
Dimension width,height;
float zoom_prec; /* fattore di zoom applicato la volta precedente */
ListDragetRew(get_lista(draw_wid));
while((dr=ListDragetNext(get_lista(draw_wid))) != NULL)
	XdChangeZoom(dr,zoom);
/*
 Modifica le dimensioni della pagina in corrispondenza dello
 zoom
*/
get_something(draw_wid, XmNheight, (void*) &height);
get_something(draw_wid, XmNwidth, (void*) &width);
/*
  calcola le dimensioni originali ( a zoom 1.0) della
  window di disegno
*/
zoom_prec= get_def_zoom(draw_wid);
height = height / zoom_prec;
width = width / zoom_prec;
set_something(draw_wid, XmNheight, (void*) (Dimension)(height * zoom));
set_something(draw_wid, XmNwidth, (void*) (Dimension)(width * zoom));
set_def_zoom(draw_wid, zoom);

XClearArea(XtDisplay(draw_wid),XtWindow(draw_wid),0,0,0,0,True);
}

void DrawDeleteAll(wid,ev)
Widget wid;
XEvent *ev;
{
int dr_num_sel;
int i;
Draget *dr_list_sel = NULL; /* lista dei draget selezionati */
/* 
 richiede la lista degli oggetti 
*/
dr_num_sel=ListGetAll(get_lista(wid),&dr_list_sel);
for(i=0; i<dr_num_sel ;i++)
	{
	ListDeleteDraget(get_lista(wid),dr_list_sel[i]);
	XdDestroyDragetNoClear(dr_list_sel[i]);
	}
if(dr_list_sel)
	XtFree((char *)dr_list_sel);
}

void DrawDelete(wid,ev)
Widget wid;
XEvent *ev;
{
int dr_num_sel;
int i;
Draget *dr_list_sel; /* lista dei draget selezionati */
dr_list_sel=NULL;

/* 
 richiede la lista degli oggetti selezionati
*/
dr_num_sel=ListGetSelected(get_lista(wid),&dr_list_sel);
for(i=0; i<dr_num_sel ;i++)
	{
	ListDeleteDraget(get_lista(wid),dr_list_sel[i]);
	XdDestroyDraget(dr_list_sel[i]);
	}
if(dr_num_sel) {
	XtFree((char *)dr_list_sel);
	undo_flag = 1;
	}
}

void MoveTasti(wid,ev)
Widget wid;
XEvent *ev;
{
KeySym tasto;
Modifiers mod;
int x,y;

x=ev->xkey.x;
y=ev->xkey.y;
tasto=XtGetActionKeysym(ev,&mod);
if(tasto == XK_Left) x -=1;
if(tasto == XK_Right) x +=1;
if(tasto == XK_Up) y -=1;
if(tasto == XK_Down) y +=1;
XWarpPointer(XtDisplay(wid),XtWindow(wid),XtWindow(wid),
	        (int)0,(int)0,(unsigned int)0,(unsigned int)0, x,y);
}


/*
void XdUndoDelete()
{
int dr_num_sel;
int i;
Draget *dr_list_sel;

dr_num_sel=ListGetAll(undo_get_lista(),&dr_list_sel);
for(i=0; i<dr_num_sel ;i++)
        {
        ListDeleteDraget(undo_get_lista(),dr_list_sel[i]);
        XdDestroyDraget(dr_list_sel[i]);
        }
if(dr_list_sel!=NULL)
        XtFree((char *)dr_list_sel);
}
*/


void XdClipDelete()
{
int dr_num_sel;
int i;
Draget *dr_list_sel; /* lista dei draget selezionati */
/*
 richiede la lista degli oggetti selezionati
*/
dr_num_sel=ListGetSelected(clip_get_lista(),&dr_list_sel);
for(i=0; i<dr_num_sel ;i++)
        {
        ListDeleteDraget(clip_get_lista(),dr_list_sel[i]);
        XdDestroyDraget(dr_list_sel[i]);
        }
if(dr_list_sel!=NULL)
	XtFree((char *)dr_list_sel);
}


void DrawPutTop(draw_wid)
Widget draw_wid;
{
int dr_num_sel;
int i;
Draget *dr_list_sel; /* lista dei draget selezionati */
/*
 richiede la lista degli oggetti selezionati
*/
dr_num_sel=ListGetSelected(get_lista(draw_wid),&dr_list_sel);
for(i=0; i<dr_num_sel ;i++)
        ListPutTop(get_lista(draw_wid),dr_list_sel[i]);
for(i=0; i<dr_num_sel ;i++)
        XdRedrawDraget(dr_list_sel[i]);
XtFree((char *)dr_list_sel);
/*
XClearArea(XtDisplay(draw_wid),XtWindow(draw_wid),0,0,0,0,True);
*/
}

void DrawPutBottom(draw_wid)
Widget draw_wid;
{
int dr_num_sel;
int i;
Draget *dr_list_sel; /* lista dei draget selezionati */
/*
 richiede la lista degli oggetti selezionati
*/
dr_num_sel=ListGetSelected(get_lista(draw_wid),&dr_list_sel);
for(i=0; i<dr_num_sel ;i++)
        ListPutBottom(get_lista(draw_wid),dr_list_sel[i]);
for(i=0; i<dr_num_sel ;i++)
        XdClearDraget(dr_list_sel[i]);
XtFree((char *)dr_list_sel);
/*
XClearArea(XtDisplay(draw_wid),XtWindow(draw_wid),0,0,0,0,True);
*/
}

/*
 Raggruppa i draget in selezione corrente
 */
void DrawGroup(draw_wid)
Widget draw_wid;
{
Draget dr;
int curr_step;
float def_zoom;

def_zoom=get_def_zoom(draw_wid);
curr_step=get_step(draw_wid);
dr=XdFirstGroupDraget(draw_wid,get_lista(draw_wid));
ListDragetAdd(get_lista(draw_wid),dr);

undo_flag = 1;
/*
XdSetZoom(dr,def_zoom);
XdSetSnap(dr,curr_step);
*/
}


DrawUngroup(draw_wid)
Widget draw_wid;
{
int dr_num_sel;
int i;
Draget *dr_list_sel; /* lista dei draget selezionati */

/*
 Ricerca tra i draget selezionati se sono presenti
 dei draget di tipo gruppo
*/
/*
 richiede la lista degli oggetti selezionati
*/
dr_num_sel=ListGetSelected(get_lista(draw_wid),&dr_list_sel);
for(i=0; i<dr_num_sel ;i++)
	{
        if(XdIsGroup(dr_list_sel[i]))
        	XdUngroupDraget(dr_list_sel[i],get_lista(draw_wid));
        }
undo_flag = 1;
}


void MovePick(wid,ev)
Widget wid;
XEvent *ev;
{
int i;

if (dr_num_selected == 0) return;
if(dr_num_selected > 1)
	{
	XtGrabPointer(wid,False,ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
		      GrabModeAsync,GrabModeAsync,XtWindow(wid),
                      None,CurrentTime);
	for(i=0; i< dr_num_selected; i++)
		XdMoveDraget(dr_list_selected[i],ev);
	}
else {
	/*
 	 Inizio codice CAPPE
	*/
	undo_move_flag++;
	/*
 	 Fine codice CAPPE
	*/
	XtGrabPointer(wid,False,ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
		      GrabModeAsync,GrabModeAsync,XtWindow(wid),
                      None,CurrentTime);
	XdMoveDraget(dr_corrente,ev);
}
}

void EndPick(wid,ev)
Widget wid;
XEvent *ev;
{
int i;
int xin, yin, xfin, yfin;
if (dr_num_selected == 0) return;
if(dr_num_selected > 1)
	{
	XtUngrabPointer(wid,CurrentTime);
	for(i=0; i< dr_num_selected; i++)
		XdEndMoveDraget(dr_list_selected[i],ev);
	/*
 	 Inizio codice CAPPE
	*/
	CreaUndoList(wid);
	undo_draget_copy(wid, ClipBoardUndo);
	undoListAdd(get_lista_liste_undo(wid),get_lista_undo(wid));
	/*
 	 Fine codice CAPPE
	*/
	XtFree((char *)dr_list_selected);
	dr_num_selected=0;
	}
else {
	if(dr_corrente) {
		XtUngrabPointer(wid,CurrentTime);
		XdEndMoveDraget(dr_corrente,ev);
	/*
 	 Inizio codice CAPPE
	*/
	if(undo_move_flag) {
		undo_move_flag = 0;
/*
		SALVATAGGIO PER MOVE SINGOLA
*/
                CreaUndoList(wid);
                undo_draget_copy(wid, ClipBoardUndo);
		undoListAdd(get_lista_liste_undo(wid),get_lista_undo(wid));
	} 
	/*
 	 Fine codice CAPPE
	*/
	}
}
dr_corrente=NULL;
dr_num_selected=0;
}


/*
 Selezione con Shift Key (selezione estesa o multipla)
*/
void ExtPick(wid,ev)
Widget wid;
XEvent *ev;
{
Draget dr;
/* 
 si posiziona all'inizio della lista dei draget
*/
ListDragetRew(get_lista(wid));
while((dr=ListDragetNext(get_lista(wid))) != NULL)
	{
	if(dr!=NULL)
		XdPickExtDraget(dr,ev); 
	}
}




void UxFirstText(wid, ev)
	Widget		wid;
	XEvent		*ev;
{
	if(drawtext == True)
		UxToggleFontCursor(wid);

	first.x= ev->xbutton.x;
	first.y= ev->xbutton.y;

	is_first= True;
	drawtext= True;
	textlen= 0;

	UxToggleFontCursor(wid);

	XFlush(UxDisplay);
}

void UxToggleFontCursor(wid)
	Widget		wid;
{
	XCharStruct	info;
	int		xpos, dir, ascent, descent;

	XTextExtents(fontinfo, text, textlen, &dir, &ascent, &descent,
		&info);

	xpos= first.x + info.width + 1;

	XDrawLine(UxDisplay, XtWindow(wid), drawGC, xpos, first.y-ascent,
		xpos, first.y+descent);
}

void UxDoneDrawingText(wid)
	Widget		wid;
{
	XCharStruct	info;
	int		xpos, dir, ascent, descent;

	if(drawtext == False)
		return;

	UxToggleFontCursor(wid);
	drawtext= False;

	XFlush(UxDisplay);
}

void UxResetText(wid)
	Widget		wid;
{
	int		width;

	if((drawtext == False) || (wid == NULL))
		return;

	if(fontinfo == NULL)
		return;

	UxToggleFontCursor(wid);
	width= XTextWidth(fontinfo, text, textlen);

	first.x+= width;
	textlen= 0;

	XFlush(UxDisplay);
}

int UxSetDrawFont(fname, wid)
	char		*fname;
	Widget		wid;
{
	static int	init= 0;
	Font		new;

	new= XLoadFont(UxDisplay, fname);

	if((new != BadName) && (new != BadAlloc) && (new != NULL))
	{	UxResetText(wid);

		if(init)
			XUnloadFont(UxDisplay, curfont);
		else
			init= 1;

		curfont= new;

		fontinfo= XQueryFont(UxDisplay, curfont);

		if(fontinfo == NULL)
		{	fprintf(stderr, "Could not find font %s\n", fname);

			if(strcmp(fname, "fixed") == 0)
			{	fprintf(stderr, "need fixed: fatal error\n");
				exit(1);
			}

			fprintf(stderr, "  -> using fixed instead\n");

			UxSetDrawFont("fixed", wid);
			return(0);
		}

		XSetFont(UxDisplay, drawGC, curfont);

		if(drawtext && (wid != NULL))
			UxToggleFontCursor(wid);
	}
}

static int add_to_string(ev, wid)
	XEvent		*ev;
	Widget		wid;
{
	KeySym		info;
	char		key[5];

	XLookupString(ev, key, 4, &info, NULL);

	if(!isprint(key[0]) || (key[0] < ' '))
	{	/* must be a special key: ignore it */

		if((key[0] == '\010') || (key[0] == '\177'))
			erase_text(wid);

		return(0);
	}

	if(textlen == 80)
	{	XBell(UxDisplay, 40);
		return(0);
	}

	text[textlen++]= key[0];

	return(1);
}

void UxDrawText(wid, ev)
	Widget		wid;
	XEvent		*ev;
{
	int		changed, width;

	if(drawtext == False)
	{	XBell(UxDisplay, 40);
		return;
	}

	UxToggleFontCursor(wid);

	changed= add_to_string(ev, wid);

	if(!changed)
	{	UxToggleFontCursor(wid);
		return;
	}

	if(is_first == True)
		is_first= False;

	width= XTextWidth(fontinfo, text, textlen-1);

				/* drawGC only the last character */
	XDrawString(UxDisplay, XtWindow(wid), drawGC, first.x+width, first.y,
		text+textlen-1, 1);

	UxToggleFontCursor(wid);

	XFlush(UxDisplay);
}

static void erase_text(wid)
	Widget		wid;
{
	int		changed, width;

	if((drawtext == False) || (textlen == 0))
	{	XBell(UxDisplay, 40);
		return;
	}

	width= XTextWidth(fontinfo, text, textlen-1);

				/* erase only the last character */
	XDrawString(UxDisplay, XtWindow(wid), drawGC, first.x+width, first.y,
		text+textlen-1, 1);

	textlen--;

	XFlush(UxDisplay);
}

void UxNextLine(wid)
	Widget		wid;
{
	int		dir, ascent, descent;
	XCharStruct	info;

	if(drawtext == False)
	{	XBell(UxDisplay, 40);
		return;
	}

	XTextExtents(fontinfo, text, textlen, &dir, &ascent, &descent,
		&info);

	UxToggleFontCursor(wid);

	first.y+= ascent+descent;
	textlen= 0;

	UxToggleFontCursor(wid);

	XFlush(UxDisplay);
}

void UxLastPoint()
{
	drawtext= False;
}

set_current_gcs(draw_wid)
Widget draw_wid;
{
Widget bott_fill_def,bott_border_def;
GC curr_gc;
GC curr_gc_bg;
int num_selected, i;
Draget *list_dr_sel; /* lista dei Draget selezionati */
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCSubwindowMode; 
XGCValues values;
Pixel pixel_fg, pixel_bg;
get_something(get_bott_border(draw_wid), XmNbackground, (void*) &pixel_fg);
get_something(get_bott_fill(draw_wid),XmNbackground, (void*) &pixel_bg);
values.foreground = pixel_fg;
values.background = pixel_bg;
values.line_width = get_line_width(draw_wid);
values.line_style = get_line_style(draw_wid);
values.subwindow_mode= IncludeInferiors;
curr_gc= XtGetGC (draw_wid, valuemask, &values);
set_gc(draw_wid,curr_gc);
values.foreground = pixel_bg;
values.background = pixel_fg;
curr_gc_bg = XtGetGC (draw_wid, valuemask, &values);
set_gc_bg(draw_wid,curr_gc_bg);
/*
 Esamina se vi sono oggetti selezionati: in questo caso la scelta
 riguarda il settaggio del GC solo per l'oggetto selezionato
*/
if(num_selected=ListGetSelected(get_lista(draw_wid),&list_dr_sel))
	{
	for(i=0; i<num_selected; i++)
		XdSetGC(list_dr_sel[i],curr_gc,curr_gc_bg);
	XtFree((char *)list_dr_sel);
	return(LOCAL_GC_SET);
	}
/*
 altrimenti viene settato il GC di default
*/
else
	{
	set_def_gc(draw_wid,curr_gc);
	set_def_gc_bg(draw_wid,curr_gc_bg);
	set_def_line_width(draw_wid,get_line_width(draw_wid));
	set_def_line_style(draw_wid,get_line_style(draw_wid));
	bott_fill_def= get_bott_fill(draw_wid);
	bott_border_def= get_bott_border(draw_wid);
	set_def_bott_fill(draw_wid,bott_fill_def);
	set_def_bott_border(draw_wid,bott_border_def);
	return(DEFAULT_GC_SET);
	}
}

/*
 set_current_filled
	setta lo stile di disegno per l'oggetto selezionato o per il
	default a disegno filled o not filled
*/
set_current_filled(draw_wid,flag)
Widget draw_wid;
Boolean flag;  /* se True disegno filled */
{
Boolean filled;
int num_selected, i;
Draget *list_dr_sel; /* lista dei Draget selezionati */
/*
 Esamina se vi sono oggetti selezionati: in questo caso la scelta
 riguarda il settaggio dell' attributo di filles 
 solo per l'oggetto selezionato (viene invertito
 comunque l'attributo)
*/

if(num_selected=ListGetSelected(get_lista(draw_wid),&list_dr_sel))
	{
	for(i=0; i<num_selected; i++)
		{
		filled=XdGetFilled(list_dr_sel[i]);
		XdSetFilled(list_dr_sel[i],(int)(!filled));
		}
	XtFree((char *)list_dr_sel);
	}
else
/*
 la scelta riguarda il default
*/
	filled= get_def_filled(draw_wid);
	set_def_filled(draw_wid,(int)(!filled));
}


/*
 get_name_clip
 ricava il path name del file usato per la clipboard
*/
Boolean get_name_clip(file_name,nome)
char *file_name;
char *nome;
{
char *p_tmpdir;
extern char *getenv();
p_tmpdir=getenv("TMPDIR");
if(p_tmpdir==NULL)
        {
        printf("\n variabile di environment TMPDIR non definita\n");
        return(False);
        }
strcpy(file_name,p_tmpdir);
#ifndef VMS
strcat(file_name,"/");
#endif
strcat(file_name,nome);
strcpy(file_name,XlConvPathVms(file_name));
return(True);
}


/*
 undo_draget_copy
 Copia i draget presenti nell'area di disegno in clipboard per
 permettere la undo
*/
void undo_draget_copy(draw_wid,draw_clip)
Widget draw_wid;
Widget draw_clip;
{

Draget dr;
int i,num_selected;
Draget *list_dr_sel;

/* XdUndoDelete();  */
if(num_selected=ListGetAll(get_lista(draw_wid),&list_dr_sel))
        {
        for(i=0;i <num_selected; i++)
                {
                dr=XdDuplicateDraget(list_dr_sel[i],draw_clip);
                ListDragetAdd(get_lista_undo(draw_wid),dr);
                }
        XtFree((char *)list_dr_sel);
        }
}


/*
 draget_copy
 Copia i draget selezionati nell'area di disegno in clipboard
*/
#ifdef ON_PAGEDIT
draget_copy(draw_wid,draw_clip)
Widget draw_wid;
Widget draw_clip;                /* id drawing area della clipboard */
#else 
draget_copy(draw_wid)
Widget draw_wid;
#endif
{
FILE *fp;
Draget dr;
char file_name[200];
int num_selected,i;
Draget *list_dr_sel; /* lista dei Draget selezionati */

#ifdef ON_PAGEDIT
XdClipDelete();
if(num_selected=ListGetSelected(get_lista(draw_wid),&list_dr_sel))
        {
        for(i=0;i <num_selected; i++)
                {
 		dr=XdDuplicateDraget(list_dr_sel[i],draw_clip);
                ListDragetAdd(clip_get_lista(),dr);
		}
	XtFree((char *)list_dr_sel);
        }
#else
if(get_name_clip(file_name,"clip.bkg") == False)
	return;

fp=fopen(file_name,"w+");
if(num_selected=ListGetSelected(get_lista(draw_wid),&list_dr_sel))
        {
        fprintf(fp,"num_d %d\n",num_selected);
        for(i=0;i <num_selected; i++)
                XdWriteDraget(list_dr_sel[i],fp);
	XtFree((char *)list_dr_sel);
	undo_flag = 1;
        }
fclose(fp);
#endif
}


/*
 draget_cut
 Copia i draget selezionati nell'area di disegno in clipboard
 e successivamente li cancella.
*/
#ifdef ON_PAGEDIT
draget_cut(draw_wid,draw_clip)
Widget draw_wid;
Widget draw_clip;              /* id drawing area della clipboard */
#else
draget_cut(draw_wid)
Widget draw_wid;
#endif
{
FILE *fp;
char file_name[200];
Draget dr;

int num_selected,i;
Draget *list_dr_sel; /* lista dei Draget selezionati */

#ifdef ON_PAGEDIT
XdClipDelete();
if(num_selected=ListGetSelected(get_lista(draw_wid),&list_dr_sel))
        {
        for(i=0;i <num_selected; i++)
                {
                dr=XdDuplicateDraget(list_dr_sel[i],draw_clip);
                ListDragetAdd(clip_get_lista(),dr);
                ListDeleteDraget(get_lista(draw_wid),list_dr_sel[i]);
                XdDestroyDraget(list_dr_sel[i]);
                }
	XtFree((char *)list_dr_sel);
        }
#else
if(get_name_clip(file_name,"clip.bkg") == False)
	return;

fp=fopen(file_name,"w+");
if(num_selected=ListGetSelected(get_lista(draw_wid),&list_dr_sel))
        {
        fprintf(fp,"num_d %d\n",num_selected);
        for(i=0;i <num_selected; i++)
		{
                XdWriteDraget(list_dr_sel[i],fp);
		ListDeleteDraget(get_lista(draw_wid),list_dr_sel[i]);
		XdDestroyDraget(list_dr_sel[i]);
		}
	XtFree((char *)list_dr_sel);
        }
fclose(fp);
#endif
undo_flag = 1;
}


/*
 undo_draget_paste
 Copia i draget presenti in clipboard nell'area di disegno in
 corrispondenza di una undo
*/
undo_draget_paste(draw_wid)
Draget draw_wid;
{
int num_selected,i;
Draget *list_dr_sel;
Draget dr;
XdListaUndo lista_liste_undo;
XdLista undo_lista_draget;

/*
 Ricava tutti gli oggetti presenti nella lista di undo
*/

/*
 Inizio codice CAPPE!!!!
*/
undo_lista_draget=get_lista_undo(draw_wid);
lista_liste_undo=get_lista_liste_undo(draw_wid);
if (lista_liste_undo->num_liste > 1) {
	DrawDeleteAll(draw_wid, NULL);
/*
 Fine codice CAPPE
*/
#ifdef MAURIC
*(undo_lista_draget) = lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-2]; 
#endif
undo_lista_draget->ListaDraget =
   lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-2].ListaDraget;
undo_lista_draget->num_draget =
   lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-2].num_draget;
undo_lista_draget->ind_draget =
   lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-2].ind_draget;
undo_lista_draget->start_pos =
   lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-2].start_pos;
undo_lista_draget->stop_pos =
   lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-2].stop_pos;
undo_lista_draget->in_use =
   lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-2].in_use;

if(num_selected=ListGetAll(get_lista_undo(draw_wid),&list_dr_sel))
        {
        for(i=0;i <num_selected; i++)
                {
                dr=XdDuplicateDraget(list_dr_sel[i],draw_wid);
                ListDragetAdd(get_lista(draw_wid),dr);
		XdDeselectDraget(dr, False);		
                XdSetSnap(dr,get_step(draw_wid));
                }
	XtFree((char *)list_dr_sel);
	}
	undoListDelete(lista_liste_undo);
#ifdef MAURIC
	if(lista_liste_undo->num_liste != 0)
	*(undo_lista_draget) = lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-1]; 
#endif
	if(lista_liste_undo->num_liste != 0) {
undo_lista_draget->ListaDraget =
   lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-1].ListaDraget;
undo_lista_draget->num_draget =
   lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-1].num_draget;
undo_lista_draget->ind_draget =
   lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-1].ind_draget;
undo_lista_draget->start_pos =
   lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-1].start_pos;
undo_lista_draget->stop_pos =
   lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-1].stop_pos;
undo_lista_draget->in_use =
   lista_liste_undo->ListeUndo[lista_liste_undo->num_liste-1].in_use;
}

	XClearArea(XtDisplay(draw_wid),XtWindow(draw_wid),0,0,0,0,True);
}
}



/*
 draget_paste
 Copia i draget presenti in clipboard nell'area di disegno
*/
draget_paste(draw_wid)
Widget draw_wid;
{
float def_zoom;
int num_selected,i;
Draget *list_dr_sel; /* lista dei Draget selezionati */
FILE *fp;
char buf[10];
char file_name[200];
int numero_draget;
Draget dr;
XdGC xdgc;


DeselectAllDraget(draw_wid, False);

#ifdef ON_PAGEDIT
if(num_selected=ListGetSelected(clip_get_lista(),&list_dr_sel))
	{
        for(i=0;i <num_selected; i++)
		{
		dr=XdDuplicateDraget(list_dr_sel[i],draw_wid);
		ListDragetAdd(get_lista(draw_wid),dr);
		XdSelectDraget(dr,False);
		XdSetSnap(dr,get_step(draw_wid));
		}
	XtFree((char *)list_dr_sel);
	}
#else
if(get_name_clip(file_name,"clip.bkg") == False)
        return;

fp=fopen(file_name, "r");
if (fp == NULL) return;

fscanf(fp,"%s %d",buf,&numero_draget);

while((dr=XdReadRecord(draw_wid,&xdgc,fp))!=NULL)
	{
	ListDragetAdd(get_lista(draw_wid),dr);
	XdSetSnap(dr,get_step(draw_wid));
	def_zoom=get_def_zoom(draw_wid);
	XdSetZoom(dr,def_zoom,False);
	XdSelectDraget(dr,False);
	}
fclose(fp);
#endif
XClearArea(XtDisplay(draw_wid),XtWindow(draw_wid),0,0,0,0,True);
undo_flag = 1;
}

/* 
 draget_duplicate
 copia & paste sulla stessa pagina
*/
draget_duplicate(draw_wid,dx,dy)
Widget draw_wid;
int dx,dy;
{

FILE *fp;
int num_selected,i;
float def_zoom;
char file_name[200];
Draget *list_dr_sel; /* lista dei Draget selezionati */
char buf[10];
int numero_draget;
Draget dr;
XdGC xdgc;

#ifdef ON_PAGEDIT
XdClipDelete();
if(num_selected=ListGetSelected(get_lista(draw_wid),&list_dr_sel))
        {
        for(i=0;i <num_selected; i++)
                {
		if(!XdIsConn(list_dr_sel[i]))
			{
 			dr=XdDuplicateDraget(list_dr_sel[i],draw_wid);
                	ListDragetAdd(clip_get_lista(),dr);
			}
		}
	XtFree((char *)list_dr_sel);
        }

DeselectAllDraget(draw_wid, True);
if(num_selected=ListGetSelected(clip_get_lista(),&list_dr_sel))
	{
        for(i=0;i <num_selected; i++)
		{
		dr=XdDuplicateDraget(list_dr_sel[i],draw_wid);
		ListDragetAdd(get_lista(draw_wid),dr);
		XdModify(dr,-dx,-dy,0,0,NULL);
		XdSelectDraget(dr,False);
		XdSetSnap(dr,get_step(draw_wid));
		XdClearDraget(dr,NULL);
		}
	XtFree((char *)list_dr_sel);
	}
#else


if(get_name_clip(file_name,"dup.bkg") == False)
	return;
fp=fopen(file_name,"w+");

if(num_selected=ListGetSelected(get_lista(draw_wid),&list_dr_sel))
        {
        fprintf(fp,"num_d %d\n",num_selected);
        for(i=0;i <num_selected; i++)
                {
		XdDeselectDraget(list_dr_sel[i],True);
                XdWriteDraget(list_dr_sel[i],fp);
                }
        }
fclose(fp);

fp=fopen(file_name, "r");
if (fp == NULL) return;
fscanf(fp,"%s %d",buf,&numero_draget);
DeselectAllDraget(draw_wid, True);
while((dr=XdReadRecord(draw_wid,&xdgc,fp))!=NULL)
        {
        ListDragetAdd(get_lista(draw_wid),dr);
	XdSetSnap(dr,get_step(draw_wid));
	def_zoom=get_def_zoom(draw_wid);
	XdSetZoom(dr,def_zoom,True);
	XdModify(dr,-dx,-dy,0,0,NULL);
        XdSelectDraget(dr,True);
        }
fclose(fp);
#endif
undo_flag = 1;
}


reset_bott_def(Widget draw_wid)
{
Widget bott_border_def, bott_border_selected;
Widget bott_fill_def, bott_fill_selected;

bott_fill_selected=get_bott_fill(draw_wid);
bott_border_selected=get_bott_border(draw_wid);
bott_fill_def=get_def_bott_fill(draw_wid);
bott_border_def=get_def_bott_border(draw_wid);

if(bott_border_def != bott_border_selected)
	wselez_bott_border(bott_border_def);
if( bott_fill_def !=  bott_fill_selected)
	wselez_bott_fill(bott_fill_def);
reset_line_def(get_def_line_width(draw_wid),get_def_line_style(draw_wid),bott_fill_def);
}

#ifndef ON_PAGEDIT
/*---------------------------------------------------------------------
 * add_translation  (finzione gia' presente nel pagedit)
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
                 set_something(wid, XmNtranslations, (void*) tr);
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

#endif

/*
 LoadDrawPixmap
	carica i bitmap per i bottoni di comando
*/
LoadDrawPixmap(vett_pix,wid)
Pixmap *vett_pix; /* vettore dei bitmap */
Widget wid;
{
LoadBitmap(wid,&vett_pix[0],freccia_bits,freccia_width,freccia_height);
LoadBitmap(wid,&vett_pix[1],zoom_bits,zoom_width,zoom_height);
LoadBitmap(wid,&vett_pix[2],bline_bits,bline_width,bline_height);
LoadBitmap(wid,&vett_pix[3],rectangle_bits,rectangle_width,rectangle_height);
LoadBitmap(wid,&vett_pix[4],circle_bits,circle_width,circle_height);
LoadBitmap(wid,&vett_pix[5],arc_bits,arc_width,arc_height);
LoadBitmap(wid,&vett_pix[6],poli_bits,poli_width,poli_height);
LoadBitmap(wid,&vett_pix[7],freehand_bits,freehand_width,freehand_height);
}

/*******************************************
 Routines utilizzate per la gestione delle connessioni
********************************************/
Draget find_conn(w,x,y,num)
Widget w; /* drawing area */
int x,y;
int *num;
{
Draget dr;
int xin,yin,xfin,yfin;
ListDragetRew(get_lista(w));
while((dr=ListDragetNext(get_lista(w))) != NULL)
	{
/*
 vengono considerate solo le connessioni che non
 sono state gia' trattate per l'icona di regolazione
 in oggetto (per segnalare che una connessione e' gia' stata
 trattata il metodo di modify delle connessioni setta a False 
 l'attributo managed
*/
	if(XdIsConn(dr) && XdIsManaged(dr))
		{
		XdGetSize(dr,&xin,&yin,&xfin,&yfin);
		if(x==xin && y==yin)
			{
			*num=0;
			return(dr);
			}
		if(x==xfin && y==yfin)
			{
			*num=1;
			return(dr);
			}
		}
	}
return(NULL);
}
 
/*
 manage_all_conn
 resetta a True l'attributo di managed delle connessione
 viene utilizzata una volta terminata la gestione dello
 spostamento delle connessione per una icona di regolazione
*/
manage_all_conn(w)
Widget w; /* drawing area */
{
Draget dr;
ListDragetRew(get_lista(w));
while((dr=ListDragetNext(get_lista(w))) != NULL)
	{
	if(XdIsConn(dr))
		XdSetManaged(dr,True);
	}
}


modifica_conn(wid,xfilo,yfilo,dx,dy,mode)
Widget wid; /* drawing area */
int xfilo,yfilo;
int dx,dy;
int mode;
{
Draget dr;
int num_estr;
if(dr=find_conn(wid,xfilo,yfilo,&num_estr))
	{
	if(mode == RESIZE_CONN)
		{
		if(num_estr)
			XdModify(dr,0,0,dx,dy,NULL);
		else
			XdModify(dr,dx,dy,0,0,NULL);
		}
	else
		XdModify(dr,dx,dy,dx,dy,NULL);
	}
}

DrawSelectConn(wid,x,y)
Widget wid;
int x,y;
{
Draget dr;
int num_estr;

if(dr=find_conn(wid,x,y,&num_estr))
        {
	XdSelectDraget(dr,True);
	}
}

/*
 Trova gli estremi di una connessione dato uno dei due estremi
 e cancella la connessione corrispondente
*/
int delete_estr_conn(wid,xfilo,yfilo,xin,yin,xfin,yfin)
Widget wid; /* drawing area */
int xfilo,yfilo;
int *xin,*yin,*xfin,*yfin;
{
Draget dr;
int num_estr;
if(dr=find_conn(wid,xfilo,yfilo,&num_estr))
        {
	XdGetSize(dr,xin,yin,xfin,yfin);
	ListDeleteDraget(get_lista(wid),dr);
	XdDestroyDraget(dr);
	return(1);
	}
return(0);
}
