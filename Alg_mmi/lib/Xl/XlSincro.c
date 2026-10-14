/**********************************************************************
*
*       C Source:               XlSincro.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:38:40 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlSincro.c-2.2.3 %  (%full_filespec: XlSincro.c-2.2.3:csrc:1 %)";
#endif
/*
   modulo XlSincro.c
   tipo 
   release 1.5
   data 1/9/96
   reserved @(#)XlSincro.c	1.5
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlSincro.c	1.5\t1/9/96";
/*
        Fine sezione per SCCS
*/
/*
 *  XlSincro.c - widget sincro per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlSincroP.h>
#include <Xl/XlDrawingUtil.h>
#include <Xl/XlPrint.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

#define DEFAULTWIDTH  5
#define DEFAULTHEIGHT 5
#define PIGRECO 3.14159265358979323846


/* lista delle risorse  */
static XtResource resources[]= {
	{
        XlNvarInput_1,
        XlCVarInput_1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlSincroRec,sincro.str_input_1),
        XmRString,
        ""
        },
        {
        XlNvarInput_5,
        XlCVarInput_5,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlSincroRec,sincro.str_input_5),
        XmRString,
        ""
        },
        {
        XlNvarInput_6,
        XlCVarInput_6,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlSincroRec,sincro.str_input_6),
        XmRString,
        ""
        },
 	{
        XlNnormFg,
        XlCNormFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlSincroRec,sincro.norm_fg),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNagoFg,
        XlCAgoFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlSincroRec,sincro.ago_fg),
        XmRString,
        "red"
        },
        };

/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
	{
        XlNvarInput_1,
        XlCVarInput_1,
        XlDvarInput_1,
	XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSincroRec,sincro.input_1),
        sizeof(PUNT_VARINP)
        },
        {
        XlNvarInput_5,
        XlCVarInput_5,
        XlDvarInput_5,
	XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSincroRec,sincro.input_5),
        sizeof(PUNT_VARINP)
        },
        {
        XlNvarInput_6,
        XlCVarInput_6,
        XlDvarInput_6,
	XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSincroRec,sincro.input_6),
        sizeof(PUNT_VARINP)
        },
      	{
        XlNnormFg,
        XlCNormFg,
        XlDnormFg,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlSincroRec,sincro.norm_fg),
        sizeof(Pixel),
        },
        {
        XlNagoFg,
        XlCAgoFg,
        XlDagoFg,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlSincroRec,sincro.ago_fg),
        sizeof(Pixel),
        },
        };
/* dichiarazioni funzioni varie */

static int min();
static void GetNormFgGC();
static void GetClearGC();
static void GetAgoGC();
static void GetAllGCs();

static void DrawIntoDirectly();
static void DrawAgoValoreSincro();
         
/* dichiarazione dei metodi (methods) */
static void Seleziona();
static void AddSelez();
static void Initialize();
static Boolean Refresh();
static void Redisplay();
static void Resize();
static void Destroy();
static Boolean Compile();
static Boolean SetValues();
#if defined XPRINTER_USED
static Boolean Print();
#endif

/* translations  */
static char defaultTranslations[]=
        "~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static XtActionsRec actions[] = {
        {"Seleziona",Seleziona},
        {"AddSelez",AddSelez}
        };

/* Inizializzazione del class record */
XlSincroClassRec xlSincroClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "Sincro",
    /* widget_size              */      sizeof(XlSincroRec),
    /* class_initialize         */      NULL,
    /* class_part_initialize    */      NULL,
    /* class_inited             */      FALSE,
    /* initialize               */      Initialize,
    /* initialize_hook          */      NULL,
    /* realize                  */      XtInheritRealize,
    /* actions                  */      NULL,
    /* num_actions              */      0,
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
    /* query_geometry           */      XtInheritQueryGeometry,
    /* display_accelerator      */      XtInheritDisplayAccelerator,
    /* extension                */      NULL
  },
  { /* legocore fields                           */
    /* oggetto pert             */      NULL,
    /* oggetto database         */      NULL,
    /* oggetto database_topologia  */   NULL,
    /* oggetto compilatore      */      NULL,
    /* informazioni per la configurazione */
    /* confinfo                 */      confinfo,
    /* num_confinfo             */      XtNumber(confinfo),
    /* refresh                  */      Refresh,
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
  { /* sincro fields */
    /* empty                   */      0,
  }
};

WidgetClass xlSincroWidgetClass = (WidgetClass) &xlSincroClassRec;


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlSincroWidget new = (XlSincroWidget)tnew;
int app_int;
Arg arg[2];



if(new->core.width<DEFAULTWIDTH)
	{
        new->core.width=DEFAULTWIDTH;
	XlWarning("XlSincro","Initialize",
	     "width minore della dimensione minima");
	}
if(new->core.height<DEFAULTHEIGHT)
	{
        new->core.height=DEFAULTWIDTH;
	XlWarning("XlSincro","Initialize",
		"height minore della dimensione minima");
	}


if((new->core.width!=new->core.height))
        {
        new->core.width=min(new->core.width,new->core.height);
        new->core.height=new->core.width;
	XlWarning("XlSincro","Initialize","width diverso da height");
        }

/*******************************
ATTENZIONE DA CONTROLLARE!!!!
*****************/

   

if(new->xlcore.rotate>3)
	{
	XlWarning("XlSincro","Initialize","rotate > 3");
	new->xlcore.rotate=new->xlcore.rotate%4;
	}

if(new->xlcore.ass_rotate>1)
	{
	XlWarning("XlSincro","Initialize","assRotate > 1");
	new->xlcore.ass_rotate=new->xlcore.ass_rotate%2;
	}



/*
 Il valore di extra range viene per ora utilizzato solo
 nel caso di  display a barra (negli altri casi il valore
 dell'extra range viene per il momento forzato a zero
*/

 new->sincro.old_posizione = -1;	 	/*private*/
 
 memset(&(new->sincro.OldFlag),sizeof(FLAG_MMI),0);

 /*
 Inizializzazione delle variabili di input ed ouput
 */
 new->sincro.input_1.valore=0;
 

 new->sincro.input_5.valore=0;	
 new->sincro.input_6.valore=0;
/*
Inizializz. variabili private
*/
 
 new->sincro.OldPixel = -1;


 
 	
GetAllGCs(new,False);

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



static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
XlSincroWidget curcw= (XlSincroWidget) current;
XlSincroWidget newcw= (XlSincroWidget) new;
Boolean do_redisplay = False;
Arg core_args[2];

int app_int;




if(curcw->sincro.norm_fg != newcw->sincro.norm_fg)
        {
        XtReleaseGC(curcw,curcw->sincro.norm_gc);
        XtReleaseGC(curcw,curcw->sincro.clear_gc);
        GetNormFgGC(newcw);
        GetClearGC(newcw);
        DrawIntoDirectly(newcw,False);
        DrawAgoValoreSincro(newcw,1,False);
        do_redisplay = True;
        }

if(curcw->sincro.ago_fg != newcw->sincro.ago_fg)
        {
        XtReleaseGC(curcw,curcw->sincro.ago_gc);
        GetAgoGC(newcw);
        DrawIntoDirectly(newcw,False);
        DrawAgoValoreSincro(newcw,1,False);
        do_redisplay = True;
        }

if(curcw->core.background_pixel != newcw->core.background_pixel)
        {
        XtReleaseGC(curcw,curcw->sincro.norm_gc);
        XtReleaseGC(curcw,curcw->sincro.clear_gc);
        XtReleaseGC(curcw,curcw->sincro.ago_gc);
        GetAllGCs(newcw,False);
        DrawIntoDirectly(newcw,False);
        DrawAgoValoreSincro(newcw,1,False);
        do_redisplay = True;
        }

	






/*
 risorse connesse con sistema di simulazione
*/


if(strcmp(newcw->sincro.str_input_1,curcw->sincro.str_input_1))
        {
        Compile(newcw);
        do_redisplay = False;
        }



if(strcmp(newcw->sincro.str_input_5,curcw->sincro.str_input_5))
        {
        Compile(newcw);
        do_redisplay = False;
        }

if(strcmp(newcw->sincro.str_input_6,curcw->sincro.str_input_6))
        {
        Compile(newcw);
        do_redisplay = False;
        }



return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlSincroWidget cw= (XlSincroWidget) w;

/*
Se c'è stato il primo refresh allora rimuovi l' evento di time out
*/


/* rilascio i 3 GC */
if(cw->sincro.norm_gc)
	XtReleaseGC(cw,cw->sincro.norm_gc);

if(cw->sincro.clear_gc)
	XtReleaseGC(cw,cw->sincro.clear_gc);

if(cw->sincro.ago_gc)
	XtReleaseGC(cw,cw->sincro.ago_gc);

/*Verificare se è necessario inserire innesco Refresh_Fast */

}

static void GetNormFgGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
XlSincroWidget cw= (XlSincroWidget) w;

unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;


values.line_width = 1;
values.line_style = LineSolid;

#if defined XPRINTER_USED
if(modoPrint)
	{
	values.foreground = XlPrintGetPixel(w,cw->sincro.norm_fg);
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->sincro.norm_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cw->sincro.norm_fg;
	values.background = cw->core.background_pixel;
        cw->sincro.norm_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->sincro.norm_fg;
values.background = cw->core.background_pixel;
cw->sincro.norm_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetClearGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle ;
XlSincroWidget cw= (XlSincroWidget) w;
values.line_width = 0;
values.line_style = LineSolid;

#if defined XPRINTER_USED
if(modoPrint)
	{
	values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
	values.background = XlPrintGetPixel(w,cw->sincro.norm_fg);
        cw->sincro.clear_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cw->core.background_pixel;
	values.background = cw->sincro.norm_fg;
        cw->sincro.clear_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->core.background_pixel;
values.background = cw->sincro.norm_fg;
cw->sincro.clear_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetAgoGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
Pixel b;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFunction ;
XlSincroWidget cw= (XlSincroWidget) w;
Widget Padre;

values.line_width = 0;
values.line_style = LineSolid;

#if defined XPRINTER_USED
if(modoPrint)
	{
	values.function=GXcopy;
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
	values.foreground = XlPrintGetPixel(w,cw->sincro.ago_fg);
        cw->sincro.ago_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
	}
else
	{
	values.function=GXxor;
	values.background = cw->core.background_pixel;
	values.foreground = cw->sincro.ago_fg ^ cw->core.background_pixel;
        cw->sincro.ago_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.function=GXxor;
values.background = cw->core.background_pixel;
values.foreground = cw->sincro.ago_fg ^ cw->core.background_pixel;
cw->sincro.ago_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetAllGCs(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XlSincroWidget cw= (XlSincroWidget)w;

GetNormFgGC(w,modoPrint);
GetAgoGC(w,modoPrint);
GetClearGC(w,modoPrint);

}

static void DrawIntoDirectly(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XlSincroWidget cw= (XlSincroWidget)w;
int width,height;
/* questo gruppo di variabili deve essere riportato nella routine DrawAgoValore */
int dx=10;
int dy=10;
int dx_label_1=10;
int dy_label_1=10;
int dx_label_2=10;
int dy_label_2=50;
int r1,r2,r3;
register int i;




width=cw->core.width;
height=cw->core.height;

/*
        se necessario sbianca lo sfondo
*/
if((cw->xlcore.trasparent!=True)&& !modoPrint)
	XlRiempiRettangolo(cw,XlWindow(cw),cw->sincro.clear_gc,0,0,100,100);
	
r1=50-3*dx/2;
r2=r1-dx/2;	
	XlArco(cw,XlWindow(cw),cw->sincro.norm_gc,
		50-r1,50-r1,
		2*r1,2*r1,0*64,360*64);
/*
**********************Linee verticali***************
*/
	XlLinea(cw,XlWindow(cw),cw->sincro.norm_gc,
		50,50-r1,50,50-r1+dy/2);	
	XlLinea(cw,XlWindow(cw),cw->sincro.norm_gc,
		51,50-(int)(r1*sin(PIGRECO/2-0.05)),
		51,50-(int)(r1*sin(PIGRECO/2-0.05))+dy/2);

/*
Linee a 45°
*/	
	/*XlLinea(cw,XlWindow(cw),cw->sincro.norm_gc,
		50-(int)(r1*cos(PIGRECO/2-0.05)),50-(int)(r1*sin(PIGRECO/2-0.05))+dy/2,
		50,50-(int)(r1*sin(PIGRECO/2-0.05))+2*dy/3);	
		
	XlLinea(cw,XlWindow(cw),cw->sincro.norm_gc,
		50+(int)(r1*cos(PIGRECO/2-0.05)),50-(int)(r1*sin(PIGRECO/2-0.05))+dy/2,
		50,50-(int)(r1*sin(PIGRECO/2-0.05))+2*dy/3);			*/




/*
cerchio piccolo 
*/	
	XlArco(cw,XlWindow(cw),cw->sincro.norm_gc,
		50-dx/2,50-dy/2,
		dx,dy,0*64,360*64);

	XlRiempiArco(cw,XlWindow(cw),cw->sincro.norm_gc,
		50-dx/2,50-dy/2,
		dx,dy,0*64,360*64);
/*
cornici esterne	x=49 ampiezza 2
*/
	
	XlRettangolo(cw,XlWindow(cw),cw->sincro.norm_gc,
		dx,dy,100-2*dx,100-2*dy);
	
/*
Disegna rettangolo per indicatore e riempilo
*/		
	XlRettangolo(cw,XlWindow(cw),cw->sincro.norm_gc,
		50-(int)(r1*cos(PIGRECO/2-0.05)),50-(int)(r1*sin(PIGRECO/2-0.05)),
		(2*(int)(r1*cos(PIGRECO/2-0.05))),(int)(dy/2));	

XlRiempiRettangolo(cw,XlWindow(cw),cw->sincro.norm_gc,
		50-(int)(r1*cos(PIGRECO/2-0.05)),50-(int)(r1*sin(PIGRECO/2-0.05)),
		(2*(int)(r1*cos(PIGRECO/2-0.05))),(int)(dy/2));

/*
Arco disegnato come punta dell' indicatore e riempito
*/


		
XlArco(cw,XlWindow(cw),cw->sincro.norm_gc,
		50-(int)(r1*cos(PIGRECO/2-0.05)),50-(int)(r1*sin(PIGRECO/2-0.05))+dy/2-(int)(r1*cos(PIGRECO/2-0.05)),
		(2*(int)(r1*cos(PIGRECO/2-0.05))),(2*(int)(r1*cos(PIGRECO/2-0.05))),
		180*64,360*64);

XlRiempiArco(cw,XlWindow(cw),cw->sincro.norm_gc,
		50-(int)(r1*cos(PIGRECO/2-0.05)),50-(int)(r1*sin(PIGRECO/2-0.05))+dy/2-(int)(r1*cos(PIGRECO/2-0.05)),
		(2*(int)(r1*cos(PIGRECO/2-0.05))),(2*(int)(r1*cos(PIGRECO/2-0.05))),
		180*64,360*64);
	
}

static Boolean Refresh(Widget w)
{
XlSincroWidget cw= (XlSincroWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;
int core_x=cw->core.x;
int core_y=cw->core.y;
Boolean old_blink;
XtIntervalId timer_id;

if(cw->xlcore.config==True)
        return(True);

/*La Refresh aggiorna i valori delle variabili,mentre la Refresh_Fast disegna
l' ago in base a tali valori*/

if(!XtIsWidget(w))
	{
	XlWarning("XlSincro","Refresh",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}

if(xlSincroClassRec.xlcore_class.database==NULL)
	{
	XlWarning("XlSincro","Refresh","database non definito");
	return(False);
	}

/*
Refresh di core:acquisizione dei valori delle 6 var. di input
*/

if(!(*superclass->xlcore_class.refresh) (w))
	{
	XlWarning("XlSincro","Refresh","errore refresh classe superiore");
	return(False);
	}
/*
Se la è la 1a invocazione di Refresh, allora innesca la Refresh_Fast
*/	



DrawAgoValoreSincro(w,0,False);      
       

return(True);
}


static void Resize(w)
Widget w;
{
XlSincroWidget cw= (XlSincroWidget)w;
int app_int;

DrawIntoDirectly(cw,False);
DrawAgoValoreSincro(cw,1,False);
}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlSincroWidget cw= (XlSincroWidget)w;
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


(*superclass->core_class.expose) (w,event,NULL);
DrawIntoDirectly(cw,False);
DrawAgoValoreSincro(cw,1,False);
/*
        Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
*/
(*superclass->xlcore_class.drawBorderIfSelected) (w);
}

static int min(a,b)
int a,b;
{
return((a<b)? a:b);
}

static void DisegnaAgo(Widget w,float posizione,int modo)
{
XlSincroWidget cw= (XlSincroWidget)w;
int dx=10;
int dy=10;
int r1;

r1=50-3*dx/2;	/*raggio esterno*/

		
		
	        if(!modo) 	/*cancella ago in vecchia pos.*/
	        	{
	      
	      		XlLinea(cw,XtWindow(cw),cw->sincro.ago_gc,
		       		50+(int)((dx/2+1)*cos(cw->sincro.old_posizione)),
                              	50-(int)((dy/2+1)*sin(cw->sincro.old_posizione)),
		       		50+(int)(r1*cos(cw->sincro.old_posizione)),
                              	50-(int)(r1*sin(cw->sincro.old_posizione)));
	      		}
	   	cw->sincro.old_posizione=posizione;
/*
Disegna ago in nuova posizione
*/
	   
	   	XlLinea(cw,XtWindow(cw),cw->sincro.ago_gc,
	      	50+(int)((dx/2+1)*cos(posizione)),
                50-(int)((dy/2+1)*sin(posizione)),
	      	50+(int)(r1*cos(posizione)),
                50-(int)(r1*sin(posizione)));
	   

}
static void DrawAgoValoreSincro(w,modo,modoPrint)
Widget w;
int modo;	/*=0 se invocata da Refresh e/o RefreshFast */
Boolean modoPrint;
{
XlSincroWidget cw= (XlSincroWidget)w;

float valore;	/*valore angolo in °*/
float posizione; /*posizione angolare dell' ago */	

/*
Se input_5!=0 o se invocaz. non da Refresh, allora
calcola valore 
*/
if((cw->sincro.input_5.valore!=0)||modo)
	{
	if (modo)/*Se invocazione non da refresh*/
		{
		valore=0;/*invocazione non da refresh*/
		}
	else	/*invocaz. da refresh*/
		{
/*
Se input_6==1, allora la fase deve essere 0 
*/		
		if(cw->sincro.input_6.valore==1)	
			valore=0;
		else	
			valore=(olDatabasePuntiClassRec.oldatabasePunti_class.valvar)
                           (xlSincroClassRec.xlcore_class.database,cw->sincro.input_1)*cw->sincro.input_1.unimis.a+
			   cw->sincro.input_1.unimis.b;

		}

/*
calcola valore posizione
*/
	posizione=valore+PIGRECO/2;	/*Lo 0 è a pi/2*/

/*
Se la posizione è cambiata o l' invocaz. non è da refresh ridisegna l'ago
*/
	if((cw->sincro.old_posizione!=posizione)||modo)
		DisegnaAgo(cw,posizione,modo);

        }/*end if input_5!=0*/

else if((!modo)&&(cw->sincro.input_5.valore==0)) 
	{
	DisegnaAgo(cw,3*PIGRECO/2,modo);	/* ago a 180°*/
	}/*end if input_5==0*/
}

static Boolean Compile(Widget w)
{
Boolean errors=False;
XlSincroWidget cw= (XlSincroWidget)w;

if(!XtIsWidget(w))
	{
	XlWarning("XlSincro","Compile",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}

/*
 Deve esistere la variabile di input
*/

if(strlen(cw->sincro.str_input_1)==0)
        {
	XlErrComp(w,"Compile","Input number 1 not defined",NULL);
        errors=True;
        }


if(strlen(cw->sincro.str_input_5)==0)
        {
	XlErrComp(w,"Compile","Input number 5 not defined",NULL);
        errors=True;
        }
if(strlen(cw->sincro.str_input_6)==0)
        {
	XlErrComp(w,"Compile","Input number 6 not defined",NULL);
        errors=True;
        }

        
if(xlSincroClassRec.xlcore_class.compilatore==NULL)
	{
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
	return(False);
	}


if(!(*superclass->xlcore_class.compile) (w,
     xlSincroClassRec.xlcore_class.compilatore))
	{
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
	return(False);
	}
XlEvError(cw,errors);
return(True);
}



#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
XlSincroWidget cw= (XlSincroWidget) w;
char *printerFont;

/*
	richiamo il metodo di print di Core
*/



/*
        richiamo il ridisegno
*/


/* rilascio i 3 GC */
if(cw->sincro.norm_gc)
	XtReleaseGC(w,cw->sincro.norm_gc);
if(cw->sincro.clear_gc)
	XtReleaseGC(w,cw->sincro.clear_gc);
if(cw->sincro.ago_gc)
	XtReleaseGC(w,cw->sincro.ago_gc);

		


GetAllGCs(w,True);


/*
	redisplay del sincro
*/
DrawIntoDirectly(cw,True);
DrawAgoValoreSincro(cw,1,True);


/* rilascio i 3 GC  della stampante*/
if(cw->sincro.norm_gc)
        XFreeGC(XlDisplay(w),cw->sincro.norm_gc);
if(cw->sincro.clear_gc)
        XFreeGC(XlDisplay(w),cw->sincro.clear_gc);
if(cw->sincro.ago_gc)
        XFreeGC(XlDisplay(w),cw->sincro.ago_gc);
/*XlFreeCambioColoreGC(cw,cw->sincro.cambio_colore[0],True);*/


GetAllGCs(cw,False);

return(True);
}
#endif
