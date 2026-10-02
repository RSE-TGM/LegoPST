/**********************************************************************
*
*       C Source:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: %  (%full_filespec: %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)Selettore.c	5.1\t11/7/95";
/*
   modulo Selettore.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)Selettore.c	5.1
*/
/*
 *  Selettore.c - widget selettore per stazioni
 */
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <aggiunte_Xt.h>

#include <stdio.h>

#include "SelettoreP.h"
#include "Rilievo.h"

#define DEFAULTWIDTH 11
#define DEFAULTHEIGHT 11

static int min(int,int);


/* lista delle risorse  */
static XtResource resources[]= {
	{
	XtNpressSelCallback,
	XtCPressSelCallback,
	XtRCallback,
	sizeof(XtPointer),
	XtOffsetOf(SelettoreRec,selettore.callback_press),
	XtRCallback,
	NULL
	},
	{
	XtNreleaseSelCallback,
	XtCReleaseSelCallback,
        XtRCallback,
	sizeof(XtPointer),
        XtOffsetOf(SelettoreRec,selettore.callback_release),
        XtRCallback,
        NULL
        },
        {
        XtNseleFg,
        XtCSeleFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(SelettoreRec,selettore.norm_fg),
        XmRString,
        XtDefaultForeground
        },
        {
        XtNseleBg,
        XtCSeleBg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(SelettoreRec,selettore.norm_bg),
        XmRString,
        XtDefaultBackground
        }, 
        {
        XtNstatoSel,
        XtCStatoSel,
        XmRInt,
        sizeof(int),
        XtOffsetOf(SelettoreRec,selettore.stato_fz),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XtNpixmap0,
        XtCPixmap0,
        XmRPixmap,
        sizeof(Pixmap),
        XtOffsetOf(SelettoreRec,selettore.selettore_0),
        XtRImmediate,
        (XtPointer) XtUnspecifiedPixmap
        },
        {
        XtNpixmap1,
        XtCPixmap1,
        XmRPixmap,
        sizeof(Pixmap),
        XtOffsetOf(SelettoreRec,selettore.selettore_1),
        XtRImmediate,
        (XtPointer) XtUnspecifiedPixmap
        },
        {
        XtNdisegnoSel,
        XtCDisegnoSel,
        XmRInt,
        sizeof(int),
        XtOffsetOf(SelettoreRec,selettore.disegno),
        XmRImmediate,
        (XtPointer)SELE_BITMAP
        },
        };

/* dichiarazioni funzioni varie */
static void blink_proc();
         
/* dichiarazione dei metodi (methods) */

static void Initialize();
static void Redisplay();
static void Destroy();
static void Resize(); 
static Boolean SetValues();
static XtGeometryResult QueryGeometry();

/* actions del widget Selettore */
static void ChangeDrawSelect();
static void SelDeact();

/* translations  */
static char defaultTranslations[]=
	"<Btn1Down>:     ChangeDrawSelect() \n\
          <Btn1Up>:       SelDeact()";

static XtActionsRec actions[] = {
        {"ChangeDrawSelect",ChangeDrawSelect},
	{"SelDeact",SelDeact},
        };

/* Inizializzazione del class record */
SelettoreClassRec selettoreClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &widgetClassRec,
    /* class_name               */      "Selettore",
    /* widget_size              */      sizeof(SelettoreRec),
    /* class_initialize         */      NULL,
    /* class_part_initialize    */      NULL,
    /* class_inited             */      FALSE,
    /* initialize               */      Initialize,
    /* initialize_hook          */      NULL,
    /* realize                  */      XtInheritRealize,
    /* actions                  */      actions,
    /* num_actions              */      XtNumber(actions),
    /* resources                */      resources,
    /* num_resources            */      XtNumber(resources),
    /* xrm_class                */      NULLQUARK,
    /* compress_motion          */      TRUE,
    /* compress_exposure        */      XtExposeCompressMultiple, /* TRUE*/
    /* compress_enterleave      */      TRUE,
    /* visible_interest         */      FALSE,
    /* destroy                  */      Destroy,
    /* resize                   */      Resize,
    /* expose                   */      Redisplay,
    /* set_values               */      SetValues,
    /* set_values_hook          */      NULL,
    /* set_values_almost        */      XtInheritSetValuesAlmost,
    /* get_values_hook          */      NULL,
    /* accept_focus             */      NULL,
    /* version                  */      XtVersion,
    /* callback_private         */      NULL,
    /* tm_table                 */      defaultTranslations,
    /* query_geometry           */      QueryGeometry,
    /* display_accelerator      */      XtInheritDisplayAccelerator,
    /* extension                */      NULL
  },
  { /* selettore fields */
    /* empty                    */      0
  }
};

WidgetClass selettoreWidgetClass = (WidgetClass) &selettoreClassRec;

static void GetSeleFgGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
SelettoreWidget cw= (SelettoreWidget) w;
values.foreground = cw->selettore.norm_fg;
values.background = cw->selettore.norm_bg;
values.line_width = 0;
values.line_style = LineSolid;
cw->selettore.norm_gc = XtGetGC((Widget)cw,
                                valuemask,
                                &values);
}


static void GetClearSeleGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
SelettoreWidget cw= (SelettoreWidget) w;
values.foreground = cw->core.background_pixel;
values.background = cw->selettore.norm_fg;
values.line_width = 0;
values.line_style = LineSolid;
cw->selettore.clear_gc = XtGetGC((Widget)cw,
                                valuemask,
                                &values);
}
static void GetAllGCs(w)
Widget w;
{
GetSeleFgGC(w);
GetClearSeleGC(w);
}


static void CreatePixmap(w)
Widget w;
{
int screen_num;
SelettoreWidget cw= (SelettoreWidget)w;
screen_num=DefaultScreen(XtDisplay(cw));
cw->selettore.pixmap_0=
                XCreatePixmap(XtDisplay(cw),RootWindow(XtDisplay(cw),
                                    DefaultScreen(XtDisplay(cw))),
                                    cw->core.width+2,cw->core.height+2,
			            DefaultDepth(XtDisplay(cw),screen_num));
cw->selettore.pixmap_1=
                XCreatePixmap(XtDisplay(cw),RootWindow(XtDisplay(cw),
                                    DefaultScreen(XtDisplay(cw))),
                                    cw->core.width+2,cw->core.height+2,
				    DefaultDepth(XtDisplay(cw),screen_num));
}


/*
 Il selettore in rilievo: un quadrante incassato nel pannello con sopra
 la manopola bombata, e la leva scura in diagonale - verso la tacca in
 alto a sinistra nello stato 0, verso quella in alto a destra nello
 stato 1, come nelle bitmap di una volta. La leva ha la sua ombra sulla
 manopola e una punta chiara dalla parte della tacca che indica; nel
 tipo a impugnatura la meta' bassa e' piu' larga.
 Le misure sono quelle delle bitmap 23x23 di xstaz, scalate se il widget
 e' di un'altra misura.
*/
static void DisegnaLeva(cw,pix,stato)
SelettoreWidget cw;
Pixmap pix;
int stato;
{
Widget w=(Widget)cw;
Display *dpy=XtDisplay(w);
GC gc;
RilievoRgb sfondo,leva,ombra,luce,punta;
XPoint tacca[3];
int lato,d,x0,ytop,xtop,ybot,xbot,larg,s,i;

lato=cw->core.width;
gc=XCreateGC(dpy,pix,0,NULL);
RilievoPixelRgb(w,cw->core.background_pixel,&sfondo);
leva=RilievoScurisci(sfondo,190);
ombra=RilievoScurisci(sfondo,120);
luce=RilievoSchiarisci(leva,90);
punta=RilievoSchiarisci(sfondo,200);

XFillRectangle(dpy,pix,cw->selettore.clear_gc,0,0,lato+2,lato+2);

/* le due tacche delle posizioni, negli angoli alti */
s=(lato*5)/23;
XSetForeground(dpy,gc,RilievoRgbPixel(w,&leva));
for(i=0;i<2;i++)
	{
	x0= i ? lato-1 : 0;
	tacca[0].x=x0;                tacca[0].y=0;
	tacca[1].x=x0+(i ? -s : s);   tacca[1].y=0;
	tacca[2].x=x0;                tacca[2].y=s;
	XFillPolygon(dpy,pix,gc,tacca,3,Convex,CoordModeOrigin);
	}

/* il quadrante incassato e la manopola */
x0=(lato*2)/23;
d=lato-2*x0-1;
RilievoSfumaCerchio(w,pix,gc,x0,x0,d,RilievoScurisci(sfondo,110),sfondo,
	RilievoSchiarisci(sfondo,90),RILIEVO_INCAVATO);
RilievoContornoCerchio(w,pix,gc,x0,x0,d,RilievoScurisci(sfondo,170));
RilievoSfumaCerchio(w,pix,gc,x0+2,x0+2,d-4,RilievoScurisci(sfondo,70),sfondo,
	RilievoSchiarisci(sfondo,140),RILIEVO_SPORGE);

/* la leva: estremo alto verso la tacca dello stato */
ytop=(lato*6)/23;
ybot=lato-1-ytop;
xtop= stato ? ybot : ytop;
xbot= stato ? ytop : ybot;
larg=(lato*4)/23;
if(larg<2) larg=2;

/* ombra sulla manopola */
XSetForeground(dpy,gc,RilievoRgbPixel(w,&ombra));
XSetLineAttributes(dpy,gc,larg,LineSolid,CapRound,JoinRound);
XDrawLine(dpy,pix,gc,xtop+1,ytop+1,xbot+1,ybot+1);

/* il corpo */
XSetForeground(dpy,gc,RilievoRgbPixel(w,&leva));
XDrawLine(dpy,pix,gc,xtop,ytop,xbot,ybot);
if(cw->selettore.disegno==SELE_IMPUGNATURA)
	{
	XSetLineAttributes(dpy,gc,larg+2,LineSolid,CapRound,JoinRound);
	XDrawLine(dpy,pix,gc,(xtop+xbot)/2,(ytop+ybot)/2,xbot,ybot);
	}

/* il filo di luce lungo la leva e la punta chiara verso la tacca */
XSetLineAttributes(dpy,gc,1,LineSolid,CapRound,JoinRound);
XSetForeground(dpy,gc,RilievoRgbPixel(w,&luce));
XDrawLine(dpy,pix,gc,xtop+(stato ? 0 : -1),ytop-1+(stato ? 0 : 1),
	(xtop+xbot)/2+(stato ? 0 : -1),(ytop+ybot)/2-1+(stato ? 0 : 1));
XSetForeground(dpy,gc,RilievoRgbPixel(w,&punta));
XFillArc(dpy,pix,gc,xtop-1,ytop-1,3,3,0,360*64);
XFreeGC(dpy,gc);
}

static void DrawIntoPixmap(w)
Widget w;
{
SelettoreWidget cw= (SelettoreWidget)w;
int delta;
int width,height;
width=cw->core.width-1;
height=cw->core.height-1;

if(cw->selettore.disegno!=SELE_BITMAP)
	{
	DisegnaLeva(cw,cw->selettore.pixmap_0,0);
	DisegnaLeva(cw,cw->selettore.pixmap_1,1);
	return;
	}

XFillRectangle(XtDisplay(cw),cw->selettore.pixmap_0,
		cw->selettore.clear_gc,0,0,cw->core.width+2,cw->core.height+2);

XFillRectangle(XtDisplay(cw),cw->selettore.pixmap_1,
		cw->selettore.clear_gc,0,0,cw->core.width+2,cw->core.height+2);

if(cw->selettore.selettore_0!=XtUnspecifiedPixmap)
	XCopyPlane(XtDisplay(cw),cw->selettore.selettore_0,
   	cw->selettore.pixmap_0,cw->selettore.norm_gc,0,0,width,height,0,0,1);

if(cw->selettore.selettore_1!=XtUnspecifiedPixmap)
	XCopyPlane(XtDisplay(cw),cw->selettore.selettore_1,
   	cw->selettore.pixmap_1,cw->selettore.norm_gc,0,0,width,height,0,0,1);
}

static void ChangeDrawSelect(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
SelettoreWidget cw= (SelettoreWidget)w;
cw->selettore.stato=(!(cw->selettore.stato));
cw->selettore.stato_fz=cw->selettore.stato;
Redisplay(w,0);
XtCallCallbacks((Widget)cw,XtNpressSelCallback,NULL);
}

static void SelDeact(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
SelettoreWidget cw= (SelettoreWidget)w;
XtCallCallbacks((Widget)cw,XtNreleaseSelCallback,NULL);
}

   
static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
SelettoreWidget new = (SelettoreWidget)tnew;
if(new->core.width<DEFAULTWIDTH)
	new->core.width=DEFAULTWIDTH;
if(new->core.width!=new->core.height)
	{
	new->core.width=min(new->core.width,new->core.height);
	new->core.height=new->core.width;
	}
new->core.border_width=0;
new->selettore.stato=new->selettore.stato_fz;
GetAllGCs(new);
CreatePixmap(new);
DrawIntoPixmap(new);
}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
SelettoreWidget cw= (SelettoreWidget)w;
register int x,y;
unsigned int width,height;
if(event)
        {
        x=event->x;
        y=event->y;
        width=event->width;
        height=event->height;
        }
else
        {
        x=0;
        y=0;
        width=cw->core.width;
        height=cw->core.height;
        }
if(cw->selettore.stato)
	{
	XCopyArea(XtDisplay(cw),cw->selettore.pixmap_1,
          XtWindow(cw),cw->selettore.norm_gc,0,0,
          cw->core.width,cw->core.height,0,0);
	}
else
	{
	XCopyArea(XtDisplay(cw),cw->selettore.pixmap_0,
          XtWindow(cw),cw->selettore.norm_gc,0,0,
          cw->core.width,cw->core.height,0,0);
	}
}

static void Resize(w)
Widget w;
{
SelettoreWidget cw= (SelettoreWidget)w;
if(cw->core.width<DEFAULTWIDTH)
	cw->core.width=DEFAULTWIDTH;
if(cw->core.width!=cw->core.height)
	{
	cw->core.height=min(cw->core.width,cw->core.height);
	cw->core.width=cw->core.height;
	}
XFreePixmap(XtDisplay(cw),cw->selettore.pixmap_0);
XFreePixmap(XtDisplay(cw),cw->selettore.pixmap_1);
CreatePixmap(cw);
DrawIntoPixmap(cw);
}

int min(a,b)
int a,b;
{
return((a<b)? a:b);
}

static XtGeometryResult QueryGeometry(w,proposed,answer)
Widget w;
XtWidgetGeometry *proposed,*answer;
{
SelettoreWidget cw= (SelettoreWidget)w;
/* setta i campi di interesse */
answer->request_mode= CWWidth | CWHeight;
/* provvisorio */
answer->width=20;
answer->height=20;
if((proposed->request_mode & (CWWidth | CWHeight)) &&
    (proposed->width == answer->width &&
     proposed->height==answer->height))
	return XtGeometryYes;
else if (answer->width == cw->core.width &&
         answer->height == cw->core.height)
	return XtGeometryNo;
else
	return XtGeometryAlmost;
}


static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
SelettoreWidget curcw= (SelettoreWidget) current;
SelettoreWidget newcw= (SelettoreWidget) new;
Boolean do_redisplay = False;

if(curcw->selettore.norm_fg != newcw->selettore.norm_fg ||
   curcw->selettore.norm_bg != newcw->selettore.norm_bg)
	{
	XtReleaseGC((Widget)curcw,curcw->selettore.norm_gc);
	GetSeleFgGC(newcw);
	DrawIntoPixmap(newcw);
	do_redisplay = True;
	}
if(newcw->selettore.stato_fz!=newcw->selettore.stato)
	{
	newcw->selettore.stato=newcw->selettore.stato_fz;
	do_redisplay = True;
	}
return do_redisplay;
}

static void Destroy(w)
Widget w;
{
SelettoreWidget cw= (SelettoreWidget) w;
if (cw->selettore.pixmap_0)
	XFreePixmap(XtDisplay(cw),cw->selettore.pixmap_0);
if (cw->selettore.pixmap_1)
	XFreePixmap(XtDisplay(cw),cw->selettore.pixmap_1);
if (cw->selettore.norm_gc)
	XtReleaseGC((Widget)cw,cw->selettore.norm_gc);
}

