/**********************************************************************
*
*       C Source:               XlBottone.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:22:48 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlBottone.c-3.2.3 %  (%full_filespec: XlBottone.c-3.2.3:csrc:1 %)";
#endif
/*
   modulo XlBottone.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlBottone.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlBottone.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlBottone.c - widget bottone per stazioni
 */
#include <Xm/XmP.h>
#include <X11/StringDefs.h>


#include <stdio.h>
#include <string.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlBottoneP.h>
#include <Ol/OlPert.h>

#define DEFAULTWIDTH 11
#define DEFAULTHEIGHT 11


/* lista delle risorse  */
static XtResource resources[]= {
        {
        XlNnormFg,
        XlCNormFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBottoneRec,bottone.norm_fg),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNactFg,
        XlCActFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBottoneRec,bottone.act_fg),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNnormBg,
        XlCNormBg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBottoneRec,bottone.norm_bg),
        XmRString,
        XtDefaultBackground
        }, 
        {
        XlNtipoBt,
        XlCTipoBt,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlBottoneRec,bottone.tipo_bt),
        XmRImmediate,
        (XtPointer)BOTTONE_CON_LUCE
        },
        {
        XlNcolorLamp,
        XlCColorLamp,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBottoneRec,bottone.color_lamp),
        XmRString,
        XtDefaultBackground
        },
        {
        XlNcolorBlink,
        XlCColorBlink,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBottoneRec,bottone.color_blink),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNblinkOn,
        XlCBlinkOn,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlBottoneRec,bottone.blink_on),
        XmRImmediate,
        (XtPointer)0
        },
	{
	XlNvarInputColore,
	XlCVarInputColore,
	XmRString,
	sizeof(char *),
	XtOffsetOf(XlBottoneRec,bottone.str_input_colore),
	XmRImmediate,
	""
	},
        {
        XlNvarInputBlink,
        XlCVarInputBlink,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlBottoneRec,bottone.str_input_blink),
        XmRImmediate,
        ""
        },
	{
	XlNvarOutput,
	XlCVarOutput,
	XmRString,
	sizeof(char *),
	XtOffsetOf(XlBottoneRec,bottone.str_output),
	XmRImmediate,
	""
	}
        };
/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
        {
        XlNnormFg,
        XlCNormFg,
	XlDnormFg,
	XlRColor,
	NULL,
	NULL,
        XlRGrColor,
        XtOffsetOf(XlBottoneRec,bottone.norm_fg),
        sizeof(Pixel),
        },
        {
        XlNactFg,
        XlCActFg,
	XlDactFg,
	XlRColor,
	NULL,
	NULL,
        XlRGrColor,
        XtOffsetOf(XlBottoneRec,bottone.act_fg),
        sizeof(Pixel),
        },
        {
        XlNnormBg,
        XlCNormBg,
        XlDnormBg,
	XlRColor,
	NULL,
	NULL,
        XlRGrColor,
        XtOffsetOf(XlBottoneRec,bottone.norm_bg),
        sizeof(Pixel),
        }, 
        {
        XlNtipoBt,
        XlCTipoBt,
	XlDtipoBt,
	XlROption,
	NULL,
	XlOtipoBt,
	XlRGrIo,
        XtOffsetOf(XlBottoneRec,bottone.tipo_bt),
        sizeof(int)
        },
        {
        XlNcolorLamp,
        XlCColorLamp,
	XlDcolorLamp,
	XlRColor,
	NULL,
	NULL,
        XlRGrColor,
        XtOffsetOf(XlBottoneRec,bottone.color_lamp),
	sizeof(Pixel)
        },
        {
        XlNcolorBlink,
        XlCColorBlink,
	XlDcolorBlink,
	XlRColor,
	NULL,
	NULL,
        XlRGrColor,
        XtOffsetOf(XlBottoneRec,bottone.color_blink),
	sizeof(Pixel)
        },
	{
	XlNvarInputColore,
	XlCVarInputColore,
        XlDvarInputColore,
	XlRTipoVarInp,
        XlRVarDX, /* XlRVarDA, */
	NULL,
	XlRGrIo,
        XtOffsetOf(XlBottoneRec,bottone.input_colore),
	sizeof(PUNT_VARINP)
	},
	{
	XlNvarInputBlink,
	XlCVarInputBlink,
        XlDvarInputBlink,
	XlRTipoVarInp,
        XlRVarDX, /* XlRVarDA, */
	NULL,
	XlRGrIo,
        XtOffsetOf(XlBottoneRec,bottone.input_blink),
	sizeof(PUNT_VARINP)
	},
	{
	XlNvarOutput, 
	XlCVarOutput,
        XlDvarOutput,
	XlRTipoVarOut,
        XlRVarXO, /* XlRVarDO, */
	NULL,
	XlRGrIo,
        XtOffsetOf(XlBottoneRec,bottone.out),
	sizeof(PUNT_VAROUT)
	},
        };

/* dichiarazioni funzioni varie */


         
/* dichiarazione dei metodi (methods) */

static void Initialize();
static void Redisplay();
static void Destroy();
static void Resize(); 
static Boolean Refresh();
static Boolean Compile();
#if defined XPRINTER_USED
static Boolean Print();
#endif

static Boolean SetValues();
static XtGeometryResult QueryGeometry();

/* actions del widget XlBottone */
static void Seleziona();
static void AddSelez();
static void DrawBottAct(),DrawBottDeact();

/* translations  */
static char defaultTranslations[]=
	"~Shift<Btn1Down>:     DrawBottAct() Seleziona()   \n\
        <Btn1Up>:       DrawBottDeact() \n\
	Shift<Btn1Down>: AddSelez()";

static XtActionsRec actions[] = {
        {"DrawBottAct",DrawBottAct},
        {"DrawBottDeact",DrawBottDeact},
	{"Seleziona",Seleziona},
	{"AddSelez",AddSelez}
        };
/* Inizializzazione del class record */
XlBottoneClassRec xlBottoneClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "Bottone",
    /* widget_size              */      sizeof(XlBottoneRec),
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
  { /* legocore fields                           */
    /* oggetto pert             */      NULL,
    /* oggetto database         */      NULL,
    /* oggetto database_topologia */    NULL,
    /* oggetto compilatore      */      NULL,
    /* informazioni per la configurazione */
    /* confinfo                 */      confinfo,
    /* num_confinfo             */      XtNumber(confinfo),
    /* refresh                  */	Refresh,
    /* compile                  */      Compile,
    /* read                     */      XtInheritRead,
    /* write                    */      XtInheritWrite,
    /* select                   */      XtInheritSelect,
    /* DrawBorserIfSelected     */      XtInheritDrawBorderIfSelected,
    /* selectRunTime            */      NULL,
#if defined XPRINTER_USED
    /* print                    */      Print,
#endif
  },
  { /* bottone fields */
    /* empty                    */      0
  }
};

WidgetClass xlBottoneWidgetClass = (WidgetClass) &xlBottoneClassRec;

/*
        calcola il valore di alterna, per il blink, in funzione
        della variabilr input_colore e input_blink
*/
static int tabella_blink(int alterna, float input_colore, float input_blink)
{
        if((input_colore==0)&&(input_blink==1))
                return(!alterna);
        if((input_colore==1)&&(input_blink==0))
                return(1);
        if((input_colore==1)&&(input_blink==1))
                return(1);
        if((input_colore==0)&&(input_blink==0))
                return(0);
}





static void GetNormFgGC(w,modo)
Widget w;
Boolean modo;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlBottoneWidget cw= (XlBottoneWidget) w;
values.line_width = 0;
values.line_style = LineSolid;
#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cw->bottone.norm_fg);
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
	cw->bottone.norm_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
				 valuemask, &values);
	}
else
	{
	values.foreground = cw->bottone.norm_fg;
	values.background = cw->core.background_pixel;
	cw->bottone.norm_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->bottone.norm_fg;
values.background = cw->core.background_pixel;
cw->bottone.norm_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetBlinkGC(w,modo)
Widget w;
Boolean modo;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlBottoneWidget cw= (XlBottoneWidget) w;
values.line_width = 0;
values.line_style = LineSolid;
#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cw->bottone.color_blink);
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
	cw->bottone.blink_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
				 valuemask, &values);
	}
else
	{
	values.foreground = cw->bottone.color_blink;
	values.background = cw->core.background_pixel;
	cw->bottone.blink_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->bottone.color_blink;
values.background = cw->core.background_pixel;
cw->bottone.blink_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetNormBgGC(w,modo)
Widget w;
Boolean modo;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlBottoneWidget cw= (XlBottoneWidget) w;

values.line_width = 0;
values.line_style = LineSolid;
#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cw->bottone.norm_bg);
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
	cw->bottone.norm_bg_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
				 valuemask, &values);
	}
else
	{
	values.foreground = cw->bottone.norm_bg;
	values.background = cw->core.background_pixel;
	cw->bottone.norm_bg_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->bottone.norm_bg;
values.background = cw->core.background_pixel;
cw->bottone.norm_bg_gc = XtGetGC(cw, valuemask, &values);
#endif
}
                                          
static void GetClearGC(w,modo)
Widget w;
Boolean modo;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlBottoneWidget cw= (XlBottoneWidget) w;
values.line_width = 0;
values.line_style = LineSolid;
#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
	values.background = XlPrintGetPixel(w,cw->bottone.norm_fg);
	cw->bottone.clear_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
				 valuemask, &values);
	}
else
	{
	values.foreground = cw->core.background_pixel;
	values.background = cw->bottone.norm_fg;
	cw->bottone.clear_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->core.background_pixel;
values.background = cw->bottone.norm_fg;
cw->bottone.clear_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetActGC(w,modo)
Widget w;
Boolean modo;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlBottoneWidget cw= (XlBottoneWidget) w;
values.line_width = 3;
values.line_style = LineSolid;
#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cw->bottone.act_fg);
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
	cw->bottone.act_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
				 valuemask, &values);
	}
else
	{
	values.foreground = cw->bottone.act_fg;
	values.background = cw->core.background_pixel;
	cw->bottone.act_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->bottone.act_fg;
values.background = cw->core.background_pixel;
cw->bottone.act_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetLampGC(w,modo)
Widget w;
Boolean modo;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlBottoneWidget cw= (XlBottoneWidget) w;
values.line_width = 1;
values.line_style = LineSolid;
#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cw->bottone.color_lamp);
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
	cw->bottone.lamp_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
				 valuemask, &values);
	}
else
	{
	values.foreground = cw->bottone.color_lamp;
	values.background = cw->core.background_pixel;
	cw->bottone.lamp_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->bottone.color_lamp;
values.background = cw->core.background_pixel;
cw->bottone.lamp_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetAllGCs(w,modo)
Widget w;
Boolean modo; /* True: get GC per xprinter */
{
GetNormFgGC(w,modo);
GetNormBgGC(w,modo);
GetClearGC(w,modo);
GetActGC(w,modo);
GetLampGC(w,modo);
GetBlinkGC(w,modo);
}



static void DrawInto(w,modo)  /* modo=True -> per stampante */
Widget w;
Boolean modo;
{
XlBottoneWidget cw= (XlBottoneWidget)w;
Position offX,offY;
int delta;
int width,height;
width=cw->core.width;
height=cw->core.height;


offX=XlPrintOffsetX(w);
offY=XlPrintOffsetY(w);



if((cw->xlcore.trasparent==False) && !modo)
	XFillRectangle(XlDisplay(cw),XlWindow(cw),
                cw->bottone.clear_gc,0+offX,0+offY,cw->core.width,cw->core.height);

if((cw->bottone.tipo_bt==BOTTONE)||(cw->bottone.tipo_bt==BOTTONE_CON_LUCE))
	{
	XFillArc(XlDisplay(w),XlWindow(cw),cw->bottone.norm_bg_gc,
               1+offX,1+offY,width-2,height-2,0,360*64);
	XDrawArc(XlDisplay(w),XlWindow(cw),cw->bottone.norm_gc,
                      1+offX,1+offY, width-2,height-2, 0,360*64);
	}
else
	{
	XFillRectangle(XlDisplay(w),XlWindow(cw),cw->bottone.norm_bg_gc,
               0+offX,0+offY,width,height);
	XDrawRectangle(XlDisplay(w),XlWindow(cw),cw->bottone.norm_gc,
                      0+offX,0+offY, width,height);
	}



if(cw->bottone.tipo_bt==BOTTONE_CON_LUCE)
        {
	/*
 	Disegna la luce interna
	*/
	delta=width/8;
	if(delta<4) delta=4;
	XFillArc(XlDisplay(w),XlWindow(cw),cw->bottone.lamp_gc,
            delta+offX,delta+offY, width-delta*2, height-delta*2, 0,360*64);
	}
if(cw->bottone.tipo_bt==BOTTONE_RETTANGOLARE_CON_LUCE)
        {
	/*
 	Disegna la luce interna
	*/
	delta=width/8;
	if(delta<3) delta=3;
	XFillRectangle(XlDisplay(w),XlWindow(cw),cw->bottone.lamp_gc,
            delta+offX,delta+offY, width-delta*2, height-delta*2);
	}
/*
 Disegna una circonferenza per dare l'idea di bottone schiacciabile
 */
width-=6;
height-=6;
if((cw->bottone.tipo_bt==BOTTONE)||(cw->bottone.tipo_bt==BOTTONE_CON_LUCE))
	XDrawArc(XlDisplay(w),XlWindow(cw),cw->bottone.norm_gc,
          3+offX,3+offY,width,height,0,360*64);
else
	XDrawRectangle(XlDisplay(w),XlWindow(cw),cw->bottone.norm_gc,
          3+offX,3+offY,width,height);
}



/*
 Disegna il bottone premuto 
*/
static void DrawIntoPush(w)
Widget w;
{
XlBottoneWidget cw= (XlBottoneWidget)w;

if((cw->bottone.tipo_bt==BOTTONE)||(cw->bottone.tipo_bt==BOTTONE_CON_LUCE))
	XDrawArc(XtDisplay(cw),XtWindow(cw),cw->bottone.act_gc,
                 2,2, cw->core.width-4, cw->core.height-4,0,360*64);
else
	XDrawRectangle(XtDisplay(cw),XtWindow(cw),cw->bottone.act_gc,
                 1,1, cw->core.width-2, cw->core.height-2);

}


/*
 Disegna il bottone premuto per blink.
*/
static void DrawIntoBlinkPush(w)
Widget w;
{
XlBottoneWidget cw= (XlBottoneWidget)w;

if((cw->bottone.tipo_bt==BOTTONE)||(cw->bottone.tipo_bt==BOTTONE_CON_LUCE))
	XDrawArc(XtDisplay(cw),XtWindow(cw),cw->bottone.act_gc,
                 2,2, cw->core.width-4,cw->core.height-4,0,360*64);
else
	XDrawRectangle(XtDisplay(cw),XtWindow(cw),cw->bottone.act_gc,
                 1,1, cw->core.width-2,cw->core.height-2);

}


/* 
 Disegna  per blink
*/
static void DrawIntoBlink(w)
Widget w;
{
XlBottoneWidget cw= (XlBottoneWidget)w;
int delta;
int width,height;

	width=cw->core.width;
	height=cw->core.height;
	width--;
	height--;
	if(cw->bottone.tipo_bt==BOTTONE_CON_LUCE)
		{
		/*
 		Disegna la luce interna
		*/
		delta=width/8;
		if(delta<4) delta=4;
		XFillArc(XtDisplay(w),XtWindow(w),cw->bottone.blink_gc,
		       delta,delta, width-delta*2+1, height-delta*2+1,0,360*64);
		}
	if(cw->bottone.tipo_bt==BOTTONE_RETTANGOLARE_CON_LUCE)
		{
		/*
 		Disegna la luce interna
		*/
		delta=width/8;
		if(delta<3) delta=3;
		XFillRectangle(XtDisplay(w),XtWindow(w),cw->bottone.blink_gc,
			delta,delta, width-delta*2+1, height-delta*2+1);
		}
}
  
#define superclass (&xlCoreClassRec)

static void Seleziona(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/*
 Richiama la routine di select di XlCore per la gestione della
 selezione
*/
(*superclass->xlcore_class.select) (w,0);
}

static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/* richiama il metodo di select */
(*superclass->xlcore_class.select)(w,1);
}

static void DrawBottAct(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
XlBottoneWidget cw= (XlBottoneWidget)w;

/*
 Se in fase di configurazione il bottone non esegue alcuna
 operazione alla sua attivazione
*/
if(cw->xlcore.config) return;
/*
	registra la pressione del bottone
*/
cw->bottone.premuto=True;

Redisplay(cw,NULL);

(olPertClassRec.olpert_class.pertsend)(xlBottoneClassRec.xlcore_class.pert,
			cw->bottone.out);
}


static void DrawBottDeact(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
XlBottoneWidget cw= (XlBottoneWidget)w;
/*
 Se in fase di configurazione il bottone non esegue alcuna
 operazione alla sua disattivazione
*/
if(cw->xlcore.config) return;
/*
	registra il rilascio del bottone
*/
cw->bottone.premuto=False;

Redisplay(w,NULL);
/*
	verifica che la perturbazione richiesta non sia un UP,
	se cosi' inserisce un DOWN al posto dell'UP
*/
if(cw->bottone.out.tipo_pert==PERT_UP)
	{
	cw->bottone.out.tipo_pert=PERT_DOWN;
(olPertClassRec.olpert_class.pertsend)(xlBottoneClassRec.xlcore_class.pert,
			cw->bottone.out);
	cw->bottone.out.tipo_pert=PERT_UP;
	}
}
   
static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlBottoneWidget new = (XlBottoneWidget)tnew;


if(new->core.width<DEFAULTWIDTH)
	new->core.width=DEFAULTWIDTH;
if(new->core.height<DEFAULTHEIGHT)
	new->core.height=DEFAULTHEIGHT;
if((new->core.width!=new->core.height)&&
  ((new->bottone.tipo_bt==BOTTONE)||(new->bottone.tipo_bt==BOTTONE_CON_LUCE)))
	{
	new->core.width=min(new->core.width,new->core.height);
	new->core.height=new->core.width;
	}

new->bottone.alterna=0;
new->bottone.premuto=False;
/*
new->bottone.str_input_colore= calloc(10,sizeof(char));
*/
GetAllGCs(new,False);
}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlBottoneWidget cw= (XlBottoneWidget)w;
register int x,y;
unsigned int width,height;
XExposeEvent evento_nullo;

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
	evento_nullo.x=0;
	evento_nullo.y=0;
	evento_nullo.width=width;
	evento_nullo.height=height;
	event= &evento_nullo;
        }

(*superclass->core_class.expose) (w,event,NULL);

DrawInto(cw,False);
if(cw->bottone.premuto)
	DrawIntoPush(cw);

/*
        Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
*/
(*superclass->xlcore_class.drawBorderIfSelected) (w);
}

static void Resize(w)
Widget w;
{
XlBottoneWidget cw= (XlBottoneWidget)w;
/**********
if(cw->core.width<DEFAULTWIDTH)
	cw->core.width=DEFAULTWIDTH;
if(cw->core.width!=cw->core.height)
	{
	cw->core.height=min(cw->core.width,cw->core.height);
	cw->core.width=cw->core.height;
	}
************/
DrawInto(cw,False);
}

static Boolean Refresh(Widget w, OlDatabasePuntiObject database)
{
XlBottoneWidget cw= (XlBottoneWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;
int old_alterna;

if(cw->xlcore.config == True)
	return(True);


if(!XtIsWidget(w))
        {
        XlWarning("XlBottone","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlBottoneClassRec.xlcore_class.database==NULL)
        {
        XlWarning("XlBottone","Refresh","database non definito");
        return(False);
        }
if(!(*superclass->xlcore_class.refresh) (w))
        {
        XlWarning("XlBottone","Refresh","errore refresh classe superiore");
        return(False);
        }

if((cw->bottone.tipo_bt==BOTTONE_CON_LUCE)||
	(cw->bottone.tipo_bt==BOTTONE_RETTANGOLARE_CON_LUCE))
	{
	old_alterna=cw->bottone.alterna;
        /*
        se non e'  definita la variabile di blink setta a 0 il valore
	*/
	if(cw->bottone.input_blink.pdb.indice == -1)
        	cw->bottone.input_blink.valore = 0;

	cw->bottone.alterna=tabella_blink(cw->bottone.alterna,
				cw->bottone.input_colore.valore,
				cw->bottone.input_blink.valore);

/*******
	if(old_alterna==cw->bottone.alterna)
		return(True);
*********/

	if(cw->bottone.alterna)
		{
		DrawIntoBlink(cw);
			if(cw->bottone.premuto)
				DrawIntoBlinkPush(cw);
		}
	else
		{
		DrawInto(cw,False);
			if(cw->bottone.premuto)
				DrawIntoPush(cw);
		}
	}
return(True);
}


static Boolean Compile(Widget w)
{
XlBottoneWidget cw= (XlBottoneWidget)w;
Boolean errors=False;

if(!XtIsWidget(w))
        {
        XlWarning("XlBottone","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }
/*
 Evidenzia se non sono stati editati dei campi
*/
if((cw->bottone.tipo_bt==BOTTONE_CON_LUCE)||
	(cw->bottone.tipo_bt==BOTTONE_RETTANGOLARE_CON_LUCE))
	{
/*
 Se bottoni con luce devono esistere le due variabili di input
*/
	if(strlen(cw->bottone.str_input_colore)==0)
		{
		XlErrComp(w,"Compile","Input variable not defined",NULL);
                errors=True;
                }

	 if(strlen(cw->bottone.str_input_blink)==0)
		{
		XlErrComp(w,"Compile","Input variable for blink not defined",NULL);
		errors=True;
		}
	}
/*
 Deve sempre esistere la variabile in uscita
*/
if(strlen(cw->bottone.str_output)==0)
	{
	XlErrComp(w,"Compile","Output variable not defined",NULL);
	errors=True;
        } 

if(xlBottoneClassRec.xlcore_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
/*
 Evidenzia graficamente sull'oggetto la presenza/assenza di errore
*/
	XlEvError(cw,True);
        return(False);
        }

if(!(*superclass->xlcore_class.compile) (w,
     xlBottoneClassRec.xlcore_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
/*
 Evidenzia graficamente sull'oggetto la presenza/assenza di errore
*/
	XlEvError(cw,True);
        return(False);
        }
	
XlEvError(cw,errors);
return(True);
}


static Boolean Read(w,fp)
Widget w;
FILE *fp;
{
printf("\n richiamato metodo di read bottone");
}

static Boolean Write(w,fp)
Widget w;
FILE *fp;
{
printf("\n richiamato metodo di write bottone");
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
XlBottoneWidget cw= (XlBottoneWidget)w;
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
XlBottoneWidget curcw= (XlBottoneWidget) current;
XlBottoneWidget newcw= (XlBottoneWidget) new;
Boolean do_redisplay = False;

/*
	Rilascio i GC
*/
if(curcw->bottone.norm_fg != newcw->bottone.norm_fg)
	{
	XtReleaseGC(curcw,curcw->bottone.norm_gc);
	GetNormFgGC(newcw);
	do_redisplay = True;
	}

if(curcw->bottone.norm_bg != newcw->bottone.norm_bg)
        {
        XtReleaseGC(curcw,curcw->bottone.norm_bg_gc);
        GetNormBgGC(newcw);
        do_redisplay = True;
        }

if(curcw->bottone.act_fg != newcw->bottone.act_fg)
        {
        XtReleaseGC(curcw,curcw->bottone.act_gc);
	GetActGC(newcw);
	do_redisplay = True;
        }

if(curcw->bottone.color_lamp != newcw->bottone.color_lamp)
        {
        XtReleaseGC(curcw,curcw->bottone.lamp_gc);
	GetLampGC(newcw);
	do_redisplay = True;
        }

if(curcw->bottone.color_blink != newcw->bottone.color_blink)
	{
	XtReleaseGC(curcw,curcw->bottone.blink_gc);
        GetBlinkGC(newcw);
        do_redisplay = False;
        }

/*
 risorse connesse con sistema di simulazione 
*/
if(strcmp(newcw->bottone.str_input_colore,curcw->bottone.str_input_colore)||
   strcmp(newcw->bottone.str_input_blink,curcw->bottone.str_input_blink)||
   strcmp(newcw->bottone.str_output,curcw->bottone.str_output))
	{
	Compile(newcw);
	do_redisplay = True;
	}

return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlBottoneWidget cw= (XlBottoneWidget) w;

/*
	Rilascio i GC
*/
if (cw->bottone.norm_gc)
	XtReleaseGC(cw,cw->bottone.norm_gc);
if (cw->bottone.clear_gc)
	XtReleaseGC(cw,cw->bottone.clear_gc);
if (cw->bottone.act_gc)
        XtReleaseGC(cw,cw->bottone.act_gc);
if (cw->bottone.lamp_gc)
        XtReleaseGC(cw,cw->bottone.lamp_gc);
if (cw->bottone.blink_gc)
	XtReleaseGC(cw,cw->bottone.blink_gc);
if (cw->bottone.norm_bg_gc)
	XtReleaseGC(cw,cw->bottone.norm_bg_gc);
}


/*
        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
XlBottoneWidget cw= (XlBottoneWidget) w;


/*
	richiamo il ridisegno
*/
(*superclass->xlcore_class.print) (w);
/*
	Rilascio i GC
*/
if (cw->bottone.norm_gc)
	XtReleaseGC(cw,cw->bottone.norm_gc);
if (cw->bottone.clear_gc)
	XtReleaseGC(cw,cw->bottone.clear_gc);
if (cw->bottone.act_gc)
        XtReleaseGC(cw,cw->bottone.act_gc);
if (cw->bottone.lamp_gc)
        XtReleaseGC(cw,cw->bottone.lamp_gc);
if (cw->bottone.blink_gc)
	XtReleaseGC(cw,cw->bottone.blink_gc);
if (cw->bottone.norm_bg_gc)
	XtReleaseGC(cw,cw->bottone.norm_bg_gc);
GetAllGCs(cw,True);

/*
	redisplay del bottone
*/
DrawInto(cw,True);

/*
	Rilascio i GC della stampante
*/
if (cw->bottone.norm_gc)
	XFreeGC(XlDisplay(w),cw->bottone.norm_gc);
if (cw->bottone.clear_gc)
	XFreeGC(XlDisplay(w),cw->bottone.clear_gc);
if (cw->bottone.act_gc)
        XFreeGC(XlDisplay(w),cw->bottone.act_gc);
if (cw->bottone.lamp_gc)
        XFreeGC(XlDisplay(w),cw->bottone.lamp_gc);
if (cw->bottone.blink_gc)
	XFreeGC(XlDisplay(w),cw->bottone.blink_gc);
if (cw->bottone.norm_bg_gc)
	XFreeGC(XlDisplay(w),cw->bottone.norm_bg_gc);
GetAllGCs(cw,False);

return(True);
}
#endif
