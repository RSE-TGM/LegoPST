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
static char SccsID[] = "@(#)Bottone.c	5.1\t11/7/95";
/*
   modulo Bottone.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)Bottone.c	5.1
*/
/*
 *  Bottone.c - widget bottone per stazioni
 */
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include "aggiunte_Xt.h"


#include <stdio.h>

#include "BottoneP.h"
#include "Rilievo.h"

#define DEFAULTWIDTH 11
#define DEFAULTHEIGHT 11

static int min(int,int);

/* lista delle risorse  */
static XtResource resources[]= {
	{
	XtNpressBtCallback,
	XtCPressBtCallback,
	XtRCallback,
	sizeof(XtPointer),
	XtOffsetOf(BottoneRec,bottone.callback_press),
	XtRCallback,
	NULL
	},
	{
	XtNreleaseBtCallback,
	XtCReleaseBtCallback,
        XtRCallback,
	sizeof(XtPointer),
        XtOffsetOf(BottoneRec,bottone.callback_release),
        XtRCallback,
        NULL
        },
        {
        XtNnormFg,
        XtCNormFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(BottoneRec,bottone.norm_fg),
        XmRString,
        XtDefaultForeground
        },
        {
        XtNactFg,
        XtCActFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(BottoneRec,bottone.act_fg),
        XmRString,
        XtDefaultForeground
        },
        {
        XtNnormBg,
        XtCNormBg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(BottoneRec,bottone.norm_bg),
        XmRString,
        XtDefaultBackground
        }, 
        {
        XtNtipoBt,
        XtCTipoBt,
        XmRInt,
        sizeof(int),
        XtOffsetOf(BottoneRec,bottone.tipo_bt),
        XmRImmediate,
        (XtPointer)BOTTONE_CON_LUCE
        },
        {
        XtNcolorLamp,
        XtCColorLamp,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(BottoneRec,bottone.color_lamp),
        XmRString,
        XtDefaultBackground
        },
        {
        XtNcolorBlink,
        XtCColorBlink,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(BottoneRec,bottone.color_blink),
        XmRString,
        XtDefaultForeground
        },
        {
        XtNblinkOn,
        XtCBlinkOn,
        XmRInt,
        sizeof(int),
        XtOffsetOf(BottoneRec,bottone.blink_on),
        XmRImmediate,
        (XtPointer)0
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

/* actions del widget Bottone */
static void DrawBottAct(),DrawBottDeact();

/* translations  */
static char defaultTranslations[]=
	"<Btn1Down>:     DrawBottAct()   \n\
        <Btn1Up>:       DrawBottDeact()";

static XtActionsRec actions[] = {
        {"DrawBottAct",DrawBottAct},
        {"DrawBottDeact",DrawBottDeact},
        };
/* Inizializzazione del class record */
BottoneClassRec bottoneClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &widgetClassRec,
    /* class_name               */      "Bottone",
    /* widget_size              */      sizeof(BottoneRec),
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
  { /* bottone fields */
    /* empty                    */      0
  }
};

WidgetClass bottoneWidgetClass = (WidgetClass) &bottoneClassRec;

static void GetNormFgGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
BottoneWidget cw= (BottoneWidget) w;
values.foreground = cw->bottone.norm_fg;
values.background = cw->core.background_pixel;
values.line_width = 0;
values.line_style = LineSolid;
cw->bottone.norm_gc = XtGetGC(w,
                                valuemask,
                                &values);
}

static void GetBlinkGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
BottoneWidget cw= (BottoneWidget) w;
values.foreground = cw->bottone.color_blink;
values.background = cw->core.background_pixel;
values.line_width = 0;
values.line_style = LineSolid;
cw->bottone.blink_gc = XtGetGC(w,
                                valuemask,
                                &values);
}
static void GetNormBgGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
BottoneWidget cw= (BottoneWidget) w;

values.foreground = cw->bottone.norm_bg;
values.background = cw->core.background_pixel;
values.line_width = 0;
values.line_style = LineSolid;
cw->bottone.norm_bg_gc = XtGetGC(w,
                                valuemask,
                                &values);
}
                                          
static void GetClearGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
BottoneWidget cw= (BottoneWidget) w;
values.foreground = cw->core.background_pixel;
values.background = cw->bottone.norm_fg;
values.line_width = 0;
values.line_style = LineSolid;
cw->bottone.clear_gc = XtGetGC(w,
                                valuemask,
                                &values);
}

static void GetActGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
BottoneWidget cw= (BottoneWidget) w;
values.foreground = cw->bottone.act_fg;
values.background = cw->core.background_pixel;
values.line_width = 3;
values.line_style = LineSolid;
cw->bottone.act_gc = XtGetGC(w,
                                valuemask,
                               &values);
}

static void GetLampGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
BottoneWidget cw= (BottoneWidget) w;
values.foreground = cw->bottone.color_lamp;
values.background = cw->core.background_pixel;
values.line_width = 1;
values.line_style = LineSolid;
cw->bottone.lamp_gc = XtGetGC(w,
                                valuemask,
                                &values);
}

static void GetAllGCs(w)
Widget w;
{
GetNormFgGC(w);
GetNormBgGC(w);
GetClearGC(w);
GetActGC(w);
GetLampGC(w);
GetBlinkGC(w);
}


static void CreatePixmap(w)
Widget w;
{
int screen_num;
BottoneWidget cw= (BottoneWidget)w;
screen_num=DefaultScreen(XtDisplay(cw));
cw->bottone.bottone_norm=
                XCreatePixmap(XtDisplay(cw),RootWindow(XtDisplay(cw),
                                    DefaultScreen(XtDisplay(cw))),
                                    cw->core.width+2,cw->core.height+2,
			            DefaultDepth(XtDisplay(cw),screen_num));
cw->bottone.bottone_blink=
                XCreatePixmap(XtDisplay(cw),RootWindow(XtDisplay(cw),
                                    DefaultScreen(XtDisplay(cw))),
                                    cw->core.width+2,cw->core.height+2,
				    DefaultDepth(XtDisplay(cw),screen_num));
}


/*
 Disegno in rilievo.
 Ogni elemento e' un cerchio riempito con una sfumatura radiale finta:
 cerchi concentrici sempre piu' piccoli e chiari, con il centro che si
 sposta verso la luce (in alto a sinistra). Cosi' il tasto sembra
 bombato; con la luce dalla parte opposta sembra incavato.
 I colori di luce e ombra si ricavano dal colore base mescolandolo con
 bianco o nero (Rilievo.c). Tutto finisce nelle due pixmap di sempre
 (normale e blink): a runtime il costo e' lo stesso di prima.

 Disegna il bottone nella pixmap: col_tasto e' il colore del tasto (o
 della lampada, per BOTTONE_LUCE), col_luce quello della luce interna di
 BOTTONE_CON_LUCE.
*/
static void DisegnaBottone(w,pix,col_tasto,col_luce)
Widget w;
Pixmap pix;
Pixel col_tasto,col_luce;
{
BottoneWidget cw= (BottoneWidget)w;
Display *dpy=XtDisplay(w);
GC gc;
RilievoRgb sfondo,tasto,luce;
int d,k,dc,sp,dl,kl;

d=cw->core.width-1;
gc=XCreateGC(dpy,pix,0,NULL);
XFillRectangle(dpy,pix,cw->bottone.clear_gc,0,0,
		cw->core.width+2,cw->core.height+2);
RilievoPixelRgb(w,cw->core.background_pixel,&sfondo);
RilievoPixelRgb(w,col_tasto,&tasto);
RilievoPixelRgb(w,col_luce,&luce);

/* la sede nel pannello: incavata, ombra in alto a sinistra */
RilievoSfumaCerchio(w,pix,gc,0,0,d,RilievoScurisci(sfondo,90),sfondo,RilievoSchiarisci(sfondo,80),RILIEVO_INCAVATO);
RilievoContornoCerchio(w,pix,gc,0,0,d,RilievoScurisci(sfondo,110));

if(cw->bottone.tipo_bt==BOTTONE_LUCE)
	{
/* lampada: una cupola di vetro colorato con il suo riflesso */
	k=1;
	dc=d-2*k;
	RilievoSfumaCerchio(w,pix,gc,k,k,dc,RilievoScurisci(tasto,70),tasto,RilievoSchiarisci(tasto,110),RILIEVO_SPORGE);
	RilievoRiflesso(w,pix,gc,k,k,dc,tasto,200);
	XFreeGC(dpy,gc);
	return;
	}

/* il tasto: bombato, o incavato quando premuto (con la luce interna
   spostata di un pixel, che a 15 pixel e' il movimento che si vede) */
k=d/14;
if(k<1) k=1;
dc=d-2*k;
sp=cw->bottone.premuto ? 1 : 0;
if(cw->bottone.premuto)
	RilievoSfumaCerchio(w,pix,gc,k,k,dc,RilievoScurisci(tasto,90),RilievoScurisci(tasto,30),tasto,RILIEVO_INCAVATO);
else
	RilievoSfumaCerchio(w,pix,gc,k,k,dc,RilievoScurisci(tasto,60),tasto,RilievoSchiarisci(tasto,90),RILIEVO_SPORGE);
RilievoContornoCerchio(w,pix,gc,k,k,dc,RilievoScurisci(tasto,120));

if(cw->bottone.tipo_bt==BOTTONE_CON_LUCE)
	{
/* la luce interna, una cupola piu' piccola dentro il tasto */
	kl=d/8;
	if(kl<3) kl=3;
	dl=d-2*kl;
	RilievoSfumaCerchio(w,pix,gc,kl+sp,kl+sp,dl,RilievoScurisci(luce,50),luce,RilievoSchiarisci(luce,120),RILIEVO_SPORGE);
	RilievoRiflesso(w,pix,gc,kl+sp,kl+sp,dl,luce,210);
	}
else if(!cw->bottone.premuto)
	RilievoRiflesso(w,pix,gc,k,k,dc,tasto,150);
XFreeGC(dpy,gc);
}

static void DrawIntoPixmap(w)
Widget w;
{
BottoneWidget cw= (BottoneWidget)w;
DisegnaBottone(w,cw->bottone.bottone_norm,
		cw->bottone.norm_bg,cw->bottone.color_lamp);
}

/* 
 Disegna il Pixmap per blink: lampeggia la luce interna se c'e',
 altrimenti tutto il tasto (o la lampada)
*/
static void DrawIntoPixmapBlink(w)
Widget w;
{
BottoneWidget cw= (BottoneWidget)w;
if(cw->bottone.tipo_bt==BOTTONE_CON_LUCE)
	DisegnaBottone(w,cw->bottone.bottone_blink,
		cw->bottone.norm_bg,cw->bottone.color_blink);
else
	DisegnaBottone(w,cw->bottone.bottone_blink,
		cw->bottone.color_blink,cw->bottone.color_lamp);
}
  
static void DrawBottAct(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
BottoneWidget cw= (BottoneWidget)w;
if(cw->bottone.tipo_bt!= BOTTONE_LUCE)
	{
	cw->bottone.premuto=1;
	DrawIntoPixmap(cw);
	DrawIntoPixmapBlink(cw);
	Redisplay(cw,NULL);
	XtCallCallbacks(w,XtNpressBtCallback,NULL);
	}
}


static void DrawBottDeact(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
BottoneWidget cw= (BottoneWidget)w;
cw->bottone.premuto=0;
DrawIntoPixmap(cw);
DrawIntoPixmapBlink(cw);
Redisplay(w,NULL);
XtCallCallbacks(w,XtNreleaseBtCallback,NULL);
}
   
static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
BottoneWidget new = (BottoneWidget)tnew;

if(new->core.width<DEFAULTWIDTH)
	new->core.width=DEFAULTWIDTH;
if(new->core.width!=new->core.height)
	{
	new->core.width=min(new->core.width,new->core.height);
	new->core.height=new->core.width;
	}
new->core.border_width=0;
new->bottone.alterna=0;
new->bottone.premuto=0;
GetAllGCs(new);
CreatePixmap(new);
DrawIntoPixmap(new);
DrawIntoPixmapBlink(new);
if(new->bottone.blink_on)
	new->bottone.time_id=XtAddTimeOut(400,blink_proc,new);
}

static void blink_proc(client_data,id)
XtPointer client_data;
XtIntervalId *id;
{
BottoneWidget cw= (BottoneWidget)client_data;
if(cw->bottone.alterna)
	{
	XCopyArea(XtDisplay(cw),cw->bottone.bottone_norm,
          XtWindow(cw),cw->bottone.norm_gc,0,0,
          cw->core.width,cw->core.height,0,0);
	}
else
	{
	XCopyArea(XtDisplay(cw),cw->bottone.bottone_blink,
          XtWindow(cw),cw->bottone.norm_gc,0,0,
          cw->core.width,cw->core.height,0,0);
	}
cw->bottone.alterna=(!(cw->bottone.alterna));
if(cw->bottone.blink_on)
	cw->bottone.time_id=XtAddTimeOut(400,blink_proc,cw);
else /* ridisegna il bottone in stato normale e non ricarica il
        timer */
	{
	XCopyArea(XtDisplay(cw),cw->bottone.bottone_norm,
          XtWindow(cw),cw->bottone.norm_gc,0,0,
          cw->core.width,cw->core.height,0,0);
	}
}


static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
BottoneWidget cw= (BottoneWidget)w;
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
XCopyArea(XtDisplay(cw),cw->bottone.bottone_norm,
          XtWindow(cw),cw->bottone.norm_gc,x,y,width,height,x,y);
}

static void Resize(w)
Widget w;
{
BottoneWidget cw= (BottoneWidget)w;
if(cw->core.width<DEFAULTWIDTH)
	cw->core.width=DEFAULTWIDTH;
if(cw->core.width!=cw->core.height)
	{
	cw->core.height=min(cw->core.width,cw->core.height);
	cw->core.width=cw->core.height;
	}
XFreePixmap(XtDisplay(cw),cw->bottone.bottone_norm);
XFreePixmap(XtDisplay(cw),cw->bottone.bottone_blink);
CreatePixmap(cw);
DrawIntoPixmap(cw);
DrawIntoPixmapBlink(cw);
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
BottoneWidget cw= (BottoneWidget)w;
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
BottoneWidget curcw= (BottoneWidget) current;
BottoneWidget newcw= (BottoneWidget) new;
Boolean do_redisplay = False;

if(curcw->bottone.norm_fg != newcw->bottone.norm_fg)
	{
	XtReleaseGC((Widget)curcw,curcw->bottone.norm_gc);
	GetNormFgGC(newcw);
	DrawIntoPixmap(newcw);
	DrawIntoPixmapBlink(newcw);
	do_redisplay = True;
	}
if(curcw->bottone.norm_bg != newcw->bottone.norm_bg)
        {
        XtReleaseGC((Widget)curcw,curcw->bottone.norm_bg_gc);
        GetNormBgGC(newcw);
	DrawIntoPixmap(newcw);
	DrawIntoPixmapBlink(newcw);
        do_redisplay = True;
        }
if(curcw->bottone.act_fg != newcw->bottone.act_fg)
        {
        XtReleaseGC((Widget)curcw,curcw->bottone.act_gc);
	GetActGC(newcw);
	DrawIntoPixmap(newcw);
	DrawIntoPixmapBlink(newcw);
	do_redisplay = True;
        }
if(curcw->bottone.color_lamp != newcw->bottone.color_lamp)
        {
        XtReleaseGC((Widget)curcw,curcw->bottone.lamp_gc);
	GetLampGC(newcw);
	DrawIntoPixmap(newcw);
	DrawIntoPixmapBlink(newcw);
	do_redisplay = True;
        }
if(curcw->bottone.color_blink != newcw->bottone.color_blink)
	{
	XtReleaseGC((Widget)curcw,curcw->bottone.blink_gc);
        GetBlinkGC(newcw);
        DrawIntoPixmapBlink(newcw);
        do_redisplay = False;
        }
if( newcw->bottone.blink_on == 1 && curcw->bottone.blink_on == 0)
	{
	newcw->bottone.time_id=XtAddTimeOut(400,blink_proc,new);
	do_redisplay = False;
	}
return do_redisplay;
}

static void Destroy(w)
Widget w;
{
BottoneWidget cw= (BottoneWidget) w;
if (cw->bottone.bottone_norm)
	XFreePixmap(XtDisplay(cw),cw->bottone.bottone_norm);
if (cw->bottone.bottone_blink)
	XFreePixmap(XtDisplay(cw),cw->bottone.bottone_blink);

if (cw->bottone.norm_gc)
	XtReleaseGC((Widget)cw,cw->bottone.norm_gc);
if (cw->bottone.clear_gc)
	XtReleaseGC((Widget)cw,cw->bottone.clear_gc);
if (cw->bottone.act_gc)
        XtReleaseGC((Widget)cw,cw->bottone.act_gc);
if (cw->bottone.lamp_gc)
        XtReleaseGC((Widget)cw,cw->bottone.lamp_gc);
if (cw->bottone.blink_gc)
	XtReleaseGC((Widget)cw,cw->bottone.blink_gc);
}

