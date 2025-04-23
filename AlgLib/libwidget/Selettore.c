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


static void DrawIntoPixmap(w)
Widget w;
{
SelettoreWidget cw= (SelettoreWidget)w;
int delta;
int width,height;
width=cw->core.width-1;
height=cw->core.height-1;

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

