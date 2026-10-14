/**********************************************************************
*
*       C Source:               XlIndicErr.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:33:56 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlIndicErr.c-4.2.4 %  (%full_filespec: XlIndicErr.c-4.2.4:csrc:1 %)";
#endif
/*
   modulo XlIndicErr.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlIndicErr.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlIndicErr.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlIndicErr.c - widget indicErr per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlIndicErrP.h>
#include <Xl/XlDrawingUtil.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

#define DEFAULTWIDTH  5
#define DEFAULTHEIGHT 5
#define PIGRECO 3.14159265358979323846


/* lista delle risorse  */
static XtResource resources[]= {
        {
        XlNnormFg,
        XlCNormFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicErrRec,indicErr.norm_fg),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNagoFg_err,
        XlCAgoFg_err,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicErrRec,indicErr.ago_fg),
        XmRString,
        "red"
        },
        {
        XlNnumeroInt,
        XlCNumeroInt,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIndicErrRec,indicErr.numero_int),
        XmRImmediate,
        (XtPointer)NUMERO_INTERI_VISUALIZZATI
        },
        {
        XlNnumeroDec,
        XlCNumeroDec,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIndicErrRec,indicErr.numero_dec),
        XmRImmediate,
        (XtPointer)NUMERO_DECIMALI_VISUALIZZATI
        },
	{
        XlNscalamento,
        XlCScalamento,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicErrRec,indicErr.scalamento),
        XtRString,
        VALORE_SCALAMENTO
        },
	{
        XlNvaloreMinimo,
        XlCValoreMinimo,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicErrRec,indicErr.valore_minimo),
        XtRString,
        VALORE_MINIMO
        },
	{
        XlNvaloreMassimo,
        XlCValoreMassimo,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicErrRec,indicErr.valore_massimo),
        XtRString,
        VALORE_MASSIMO
        },
	{
        XlNoffset,
        XlCOffset,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicErrRec,indicErr.offset),
        XtRString,
        VALORE_OFFSET
        },
	{
        XlNscalamentoErr,
        XlCScalamentoErr,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicErrRec,indicErr.scalamento_err),
        XtRString,
        VALORE_SCALAMENTO
        },
	{
        XlNvaloreMinimoErr,
        XlCValoreMinimoErr,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicErrRec,indicErr.valore_minimo_err),
        XtRString,
        VALORE_MINIMO
        },
	{
        XlNvaloreMassimoErr,
        XlCValoreMassimoErr,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicErrRec,indicErr.valore_massimo_err),
        XtRString,
        VALORE_MASSIMO
        },
	{
        XlNoffsetErr,
        XlCOffsetErr,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicErrRec,indicErr.offset_err),
        XtRString,
        VALORE_OFFSET
        },
	{
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIndicErrRec,indicErr.normalfont),
        XmRImmediate,
        "fixed"
        },
	{
        XlNvarInput,
        XlCVarInput,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIndicErrRec,indicErr.str_input),
        XmRImmediate,
        ""
        },
	{
        XlNvarInputErr,
        XlCVarInputErr,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIndicErrRec,indicErr.str_input_err),
        XmRImmediate,
        ""
        },
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
        XtOffsetOf(XlIndicErrRec,indicErr.norm_fg),
        sizeof(Pixel),
        },
        {
        XlNagoFg_err,
        XlCAgoFg_err,
        XlDagoFg_err,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlIndicErrRec,indicErr.ago_fg),
        sizeof(Pixel),
        },
        {
        XlNnumeroInt,
        XlCNumeroInt,
        XlDnumeroInt,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicErrRec,indicErr.numero_int),
        sizeof(int)
        },
        {
        XlNnumeroDec,
        XlCNumeroDec,
        XlDnumeroDec,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicErrRec,indicErr.numero_dec),
        sizeof(int)
        },
        {
        XlNscalamento,
        XlCScalamento,
        XlDscalamento,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicErrRec,indicErr.scalamento),
        sizeof(float)
        },
        {
        XlNvaloreMinimo,
        XlCValoreMinimo,
        XlDvaloreMinimo,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicErrRec,indicErr.valore_minimo),
        sizeof(float)
        },
        {
        XlNvaloreMassimo,
        XlCValoreMassimo,
        XlDvaloreMassimo,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicErrRec,indicErr.valore_massimo),
        sizeof(float)
        },
        {
        XlNoffset,
        XlCOffset,
        XlDoffset,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicErrRec,indicErr.offset),
        sizeof(float)
        },
        {
        XlNscalamentoErr,
        XlCScalamentoErr,
        XlDscalamentoErr,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicErrRec,indicErr.scalamento_err),
        sizeof(float)
        },
        {
        XlNvaloreMinimoErr,
        XlCValoreMinimoErr,
        XlDvaloreMinimoErr,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicErrRec,indicErr.valore_minimo_err),
        sizeof(float)
        },
        {
        XlNvaloreMassimoErr,
        XlCValoreMassimoErr,
        XlDvaloreMassimoErr,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicErrRec,indicErr.valore_massimo_err),
        sizeof(float)
        },
        {
        XlNoffsetErr,
        XlCOffsetErr,
        XlDoffsetErr,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicErrRec,indicErr.offset_err),
        sizeof(float)
        },
        {
        XlNnormalFont,
        XlCNormalFont,
        XlDnormalFont,
        XlRFont,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicErrRec,indicErr.normalfont),
        sizeof(XFontStruct*)
        },
        {
        XlNvarInput,
        XlCVarInput,
        XlDvarInput,
	XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicErrRec,indicErr.input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNvarInputErr,
        XlCVarInputErr,
        XlDvarInputErr,
	XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicErrRec,indicErr.input_err),
        sizeof(PUNT_VARINP)
        }
	};
/* dichiarazioni funzioni varie */
static int min();
static void GetNormFgGC();
static void GetClearGC();
static void GetAgoGC();
static void GetAllGCs();
static Boolean LoadFont();
static void DrawIntoDirectly();
static void DrawAgoValoreDirectly();
         
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
XlIndicErrClassRec xlIndicErrClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "IndicErr",
    /* widget_size              */      sizeof(XlIndicErrRec),
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
  { /* indicErr fields */
    /* empty                    */      0
  }
};

WidgetClass xlIndicErrWidgetClass = (WidgetClass) &xlIndicErrClassRec;


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlIndicErrWidget new = (XlIndicErrWidget)tnew;
int app_int;


if(new->core.width<DEFAULTWIDTH)
	{
        new->core.width=DEFAULTWIDTH;
	XlWarning("XlIndicErr","Initialize",
	     "width minore della dimensione minima");
	}
if(new->core.height<DEFAULTHEIGHT)
	{
        new->core.height=DEFAULTWIDTH;
	XlWarning("XlIndicErr","Initialize",
		"height minore della dimensione minima");
	}

if(new->core.width!=new->core.height)
        {
        new->core.width=min(new->core.width,new->core.height);
        new->core.height=new->core.width;
	XlWarning("XlIndicErr","Initialize","width diverso da height");
        }

if(new->xlcore.rotate>3)
	{
	XlWarning("XlIndicErr","Initialize","rotate > 3");
	new->xlcore.rotate=new->xlcore.rotate%4;
	}
if(new->xlcore.ass_rotate>1)
	{
	XlWarning("XlIndicErr","Initialize","assRotate > 1");
	new->xlcore.ass_rotate=new->xlcore.ass_rotate%2;
	}

if(new->indicErr.valore_massimo<=new->indicErr.valore_minimo)
	{
	XlWarning("XlIndicErr","SetVAlues",
	    "il valore massimo deve essere maggiore del minimo");
	new->indicErr.valore_massimo=atof(VALORE_MASSIMO);
	new->indicErr.valore_minimo=atof(VALORE_MINIMO);
	}

 new->indicErr.old_posizione = -1;
 new->indicErr.old_posizione_err = -1;
 new->indicErr.input.valore=0;
 new->indicErr.input_err.valore=0;

if(!LoadFont(new,new->indicErr.normalfont))
	if(!LoadFont(new,"fixed"))
		XlError("XlIndicErr","Initialize","Impossibile caricare default font");
GetAllGCs(new);
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
XlIndicErrWidget curcw= (XlIndicErrWidget) current;
XlIndicErrWidget newcw= (XlIndicErrWidget) new;
Boolean do_redisplay = False;
Arg core_args[2];

int app_int;


if(curcw->indicErr.norm_fg != newcw->indicErr.norm_fg)
        {
        XtReleaseGC(curcw,curcw->indicErr.norm_gc);
        XtReleaseGC(curcw,curcw->indicErr.clear_gc);
        GetNormFgGC(newcw);
        GetClearGC(newcw);
        DrawIntoDirectly(newcw);
        DrawAgoValoreDirectly(newcw,1);
        do_redisplay = True;
        }

if(curcw->indicErr.ago_fg != newcw->indicErr.ago_fg)
        {
        XtReleaseGC(curcw,curcw->indicErr.ago_gc);
        GetAgoGC(newcw);
        DrawIntoDirectly(newcw);
        DrawAgoValoreDirectly(newcw,1);
        do_redisplay = True;
        }

if(curcw->core.background_pixel != newcw->core.background_pixel)
        {
        XtReleaseGC(curcw,curcw->indicErr.norm_gc);
        XtReleaseGC(curcw,curcw->indicErr.clear_gc);
        XtReleaseGC(curcw,curcw->indicErr.ago_gc);
        GetAllGCs(newcw);
        DrawIntoDirectly(newcw);
        DrawAgoValoreDirectly(newcw,1);
        do_redisplay = True;
        }


if(curcw->indicErr.scalamento!=newcw->indicErr.scalamento)
        {
        DrawIntoDirectly(newcw);
        DrawAgoValoreDirectly(newcw,1);
        do_redisplay = True;
        }
	
if(curcw->indicErr.valore_minimo!=newcw->indicErr.valore_minimo)
        {
	if(newcw->indicErr.valore_minimo>=newcw->indicErr.valore_massimo)
		{
		XlWarning("XlIndicErr","SetVAlues",
		    "il valore minimo deve essere minore del massimo");
		newcw->indicErr.valore_minimo=curcw->indicErr.valore_minimo;
		}
	else
		{
        	DrawIntoDirectly(newcw);
        	DrawAgoValoreDirectly(newcw,1);
        	do_redisplay = True;
		}
        }

if(curcw->indicErr.valore_massimo!=newcw->indicErr.valore_massimo)
        {
	if(newcw->indicErr.valore_massimo<=newcw->indicErr.valore_minimo)
		{
		XlWarning("XlIndicErr","SetVAlues",
		    "il valore massimo deve essere maggiore del minimo");
		newcw->indicErr.valore_massimo=curcw->indicErr.valore_massimo;
		}
	else
		{
        	DrawIntoDirectly(newcw);
        	DrawAgoValoreDirectly(newcw,1);
        	do_redisplay = True;
		}
        }

if(curcw->indicErr.offset!=newcw->indicErr.offset)
        {
        DrawIntoDirectly(newcw);
        DrawAgoValoreDirectly(newcw,1);
        do_redisplay = True;
        }

if(strcmp(curcw->indicErr.normalfont,newcw->indicErr.normalfont))
        {
	if(!LoadFont(newcw,newcw->indicErr.normalfont))
        if(!LoadFont(newcw,"fixed"))
                XlError("XlIndicErr","SetValues",
			"Impossibile caricare default font");

        DrawIntoDirectly(newcw);
        DrawAgoValoreDirectly(newcw,1);
        do_redisplay = True;
        }

/*
 risorse connesse con sistema di simulazione
*/
if(strcmp(newcw->indicErr.str_input,curcw->indicErr.str_input))
        {
        Compile(newcw);
        do_redisplay = False;
        }


return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlIndicErrWidget cw= (XlIndicErrWidget) w;

/* rilascio i 3 GC */
if(cw->indicErr.norm_gc)
	XtReleaseGC(cw,cw->indicErr.norm_gc);

if(cw->indicErr.clear_gc)
	XtReleaseGC(cw,cw->indicErr.clear_gc);

if(cw->indicErr.ago_gc)
	XtReleaseGC(cw,cw->indicErr.ago_gc);

/* libero la font_struct */
/*
if(cw->indicErr.font_info)
	XFreeFont(XtDisplay(cw),cw->indicErr.font_info);
*/

/* libero la font_list  */
/*
if(cw->indicErr.font_list)
	XmFontListFree(cw->indicErr.font_list);
*/
}

static void GetNormFgGC(w)
Widget w;
{
XGCValues values;
XlIndicErrWidget cw= (XlIndicErrWidget) w;

unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;


values.foreground = cw->indicErr.norm_fg;
values.background = cw->core.background_pixel;
values.line_width = 1;
values.line_style = LineSolid;
values.font = cw->indicErr.font_info->fid;
cw->indicErr.norm_gc = XtGetGC(cw, valuemask, &values);
}

static void GetClearGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlIndicErrWidget cw= (XlIndicErrWidget) w;
values.foreground = cw->core.background_pixel;
values.background = cw->indicErr.norm_fg;
values.line_width = 0;
values.line_style = LineSolid;
cw->indicErr.clear_gc = XtGetGC(cw, valuemask, &values);
}

static void GetAgoGC(w)
Widget w;
{
XGCValues values;
Pixel b;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFunction | GCFont ;
XlIndicErrWidget cw= (XlIndicErrWidget) w;
Widget Padre;

values.foreground = cw->indicErr.ago_fg ^ cw->core.background_pixel;
values.function=GXxor;
values.background = cw->core.background_pixel;
values.line_width = 0;
values.line_style = LineSolid;
values.font = cw->indicErr.font_info->fid;
cw->indicErr.ago_gc = XtGetGC(cw, valuemask, &values);
}

static void GetAllGCs(w)
Widget w;
{
GetNormFgGC(w);
GetAgoGC(w);
GetClearGC(w);
}

static void DrawIntoDirectly(w)
Widget w;
{
XlIndicErrWidget cw= (XlIndicErrWidget)w;
int width,height;
/* questo gruppo di variabili deve essere riportato nella routine DrawAgoValore */
int dx=10;
int dy=10;
int dx_label_1=25;
int dy_label_1=25;
int dx_label_2=25;
int dy_label_2=50;
int r1,r2,r3;
char testo[15];




width=cw->core.width;
height=cw->core.height;

/*
        se necessario sbianca lo sfondo
*/
if(cw->xlcore.trasparent!=True)
	XlRiempiRettangolo(cw,XtWindow(cw),cw->indicErr.clear_gc,0,0,100,100);
	
	XlArco(cw,XtWindow(cw),cw->indicErr.norm_gc,
		-100+3*dx+(dx_label_1-15),0+dy+(dy_label_1-15),
		2*(100-2*dx-(dx_label_1-15)),2*(100-2*dy-(dy_label_1-15)),
		23*64,45*64);
	XlArco(cw,XtWindow(cw),cw->indicErr.norm_gc,
		-100+3*dx+dx_label_1,0+dy+dy_label_1,
		2*(100-2*dx-dx_label_1),2*(100-2*dy-dy_label_1),0*64,90*64);
	XlArco(cw,XtWindow(cw),cw->indicErr.norm_gc,
		dx/2,100-3*dy/2,dx,dy,0*64,360*64);
	XlLinea(cw,XtWindow(cw),cw->indicErr.norm_gc,
		100-dx-dx_label_1, 100-dy, 100-2*dx-dx_label_1, 100-dy);
	XlLinea(cw,XtWindow(cw),cw->indicErr.norm_gc,
		dx, dy+dy_label_1, dx, 2*dy+dy_label_1);
	sprintf(testo,"%*.*f",cw->indicErr.numero_int+cw->indicErr.numero_dec+1,
			cw->indicErr.numero_dec,cw->indicErr.valore_minimo);
	XlTesto(cw,XtWindow(cw),cw->indicErr.norm_gc, 
		100-(dx+dx_label_1)/2,100-dy,testo,
		cw->indicErr.font_info);
	sprintf(testo,"%*.*f", cw->indicErr.numero_int+cw->indicErr.numero_dec+1,
			cw->indicErr.numero_dec, cw->indicErr.valore_massimo);
	XlTesto(cw,XtWindow(cw),cw->indicErr.norm_gc, 
		dx,(dy+dy_label_1)/2,testo,cw->indicErr.font_info);
	r1=100-2*dx-dx_label_1+1;
	r2=100-2*dx-dx_label_1-dx+1;
	r3=100-dx_label_1-15;
	sprintf(testo,"%*.*f", cw->indicErr.numero_int+cw->indicErr.numero_dec+1,
		cw->indicErr.numero_dec, 
		(cw->indicErr.valore_massimo+cw->indicErr.valore_minimo)/2);
	XlLinea(cw,XtWindow(cw),cw->indicErr.norm_gc,
		 dx+(int)(r2*cos(PIGRECO/4)),
			100-dy-(int)(r2*sin(PIGRECO/4)),
		 dx+(int)(r1*cos(PIGRECO/4)),
			100-dy-(int)(r1*sin(PIGRECO/4)));
	r2=100-2*dx-dx_label_1-dx/2+1;
	XlLinea(cw,XtWindow(cw),cw->indicErr.norm_gc,
		 dx+(int)(r2*cos(PIGRECO/8)),
			100-dy-(int)(r2*sin(PIGRECO/8)),
		 dx+(int)(r1*cos(PIGRECO/8)),
			100-dy-(int)(r1*sin(PIGRECO/8)));
	XlLinea(cw,XtWindow(cw),cw->indicErr.norm_gc,
		 dx+(int)(r2*cos(3*PIGRECO/8)),
			100-dy-(int)(r2*sin(3*PIGRECO/8)),
		 dx+(int)(r1*cos(3*PIGRECO/8)),
			100-dy-(int)(r1*sin(3*PIGRECO/8)));
        /*
		indicatore errore
	*/
	r1=100-2*dx-(dx_label_1-15)+1;
	r2=100-2*dx-(dx_label_1-15)-dx+2;
	r3=100-(dx_label_1-15)-10;
	XlLinea(cw,XtWindow(cw),cw->indicErr.norm_gc,
		 dx+(int)(r2*cos(PIGRECO/8)),
			100-dy-(int)(r2*sin(PIGRECO/8)),
		 dx+(int)(r1*cos(PIGRECO/8)),
			100-dy-(int)(r1*sin(PIGRECO/8)));
	XlLinea(cw,XtWindow(cw),cw->indicErr.norm_gc,
		 dx+(int)(r2*cos(3*PIGRECO/8)),
			100-dy-(int)(r2*sin(3*PIGRECO/8)),
		 dx+(int)(r1*cos(3*PIGRECO/8)),
			100-dy-(int)(r1*sin(3*PIGRECO/8)));
	r2=100-2*dx-(dx_label_1-15)-dx/2+1;
	XlLinea(cw,XtWindow(cw),cw->indicErr.norm_gc,
		 dx+(int)(r2*cos(PIGRECO/4)),
			100-dy-(int)(r2*sin(PIGRECO/4)),
		 dx+(int)(r1*cos(PIGRECO/4)),
			100-dy-(int)(r1*sin(PIGRECO/4)));
        sprintf(testo,"%*.*f", cw->indicErr.numero_int+cw->indicErr.numero_dec+1,
                cw->indicErr.numero_dec,cw->indicErr.valore_massimo_err);
        XlTesto(cw,XtWindow(cw),cw->indicErr.norm_gc,
                 dx+(int)(r3*cos(3*PIGRECO/8)),
                        100-dy-(int)(r3*sin(3*PIGRECO/8)),
                testo,cw->indicErr.font_info);
        sprintf(testo,"%*.*f", cw->indicErr.numero_int+cw->indicErr.numero_dec+1,
                cw->indicErr.numero_dec,cw->indicErr.valore_minimo_err);
        XlTesto(cw,XtWindow(cw),cw->indicErr.norm_gc,
                 dx+(int)(r3*cos(PIGRECO/8)),
                        100-dy-(int)(r3*sin(PIGRECO/8)),
                testo,cw->indicErr.font_info);

}




static Boolean Refresh(Widget w)
{
XlIndicErrWidget cw= (XlIndicErrWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;
int core_x=cw->core.x;
int core_y=cw->core.y;


if(cw->xlcore.config==True)
        return(True);


if(!XtIsWidget(w))
	{
	XlWarning("XlIndicErr","Refresh",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}

if(xlIndicErrClassRec.xlcore_class.database==NULL)
	{
	XlWarning("XlIndicErr","Refresh","database non definito");
	return(False);
	}
if(!(*superclass->xlcore_class.refresh) (w))
	{
	XlWarning("XlIndicErr","Refresh","errore refresh classe superiore");
	return(False);
	}

DrawAgoValoreDirectly(w,0);

return(True);
}


static void Resize(w)
Widget w;
{
XlIndicErrWidget cw= (XlIndicErrWidget)w;
int app_int;

DrawIntoDirectly(cw);
DrawAgoValoreDirectly(cw,1);
}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlIndicErrWidget cw= (XlIndicErrWidget)w;
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
DrawIntoDirectly(cw);
DrawAgoValoreDirectly(cw,1);
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

static Boolean LoadFont( cw,fontname)
Widget cw;
char fontname[];
{
XlIndicErrWidget Xl_cw = (XlIndicErrWidget) cw;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
/* Carica il font ottenendo la descrizione del font stesso */

char **pf;
int i,nf;

if((Xl_cw->indicErr.font_info = XlGetFont(XtDisplay(cw),fontname)) == NULL)
        {
	XlWarning("XlIndicErr","LoadFont","Cannot open font");
	return(False);
        }
return(True);
}


static void DrawAgoValoreDirectly(w,modo)
Widget w;
int modo;
{
XlIndicErrWidget cw= (XlIndicErrWidget)w;
float valore = cw->indicErr.input.valore;
float scalamento = cw->indicErr.scalamento;
float offset = cw->indicErr.offset;
float min = cw->indicErr.valore_minimo;
float max = cw->indicErr.valore_massimo;
float valore_err = cw->indicErr.input_err.valore;
float scalamento_err = cw->indicErr.scalamento_err;
float offset_err = cw->indicErr.offset_err;
float min_err = cw->indicErr.valore_minimo_err;
float max_err = cw->indicErr.valore_massimo_err;
int dx=10;
int dy=10;
int dx_label_1=25;
int dy_label_1=25;
int dx_label_2=25;
int dy_label_2=50;
float posizione;
float posizione_err;
int r1,r2;
char testo[40];



if(cw->xlcore.config==True)
	{
        valore=(max-min)*3/5+min;
        valore_err=(max_err-min_err)*3/5+min_err;
	}


valore = valore*scalamento+offset;
if(valore>max)
	valore=max;
else
	if(valore<min)
        	valore=min;
if(min==max)
	valore=min;

	posizione=((valore-min)/(max-min))*PIGRECO/2;
	if((cw->indicErr.old_posizione!=posizione)||modo)
	   {
	   if((cw->indicErr.old_posizione>=0)&& !modo)
	      {
	      r1=dx/2;
	      r2=100-2*dx-dx_label_1-dx+1;
	      XlLinea(cw,XtWindow(cw),cw->indicErr.ago_gc,
		       dx+(int)(r1*cos(cw->indicErr.old_posizione)),
                              100-dy-(int)(r1*sin(cw->indicErr.old_posizione)),
		       dx+(int)(r2*cos(cw->indicErr.old_posizione)),
                              100-dy-(int)(r2*sin(cw->indicErr.old_posizione)));
	      }
	   cw->indicErr.old_posizione=posizione;
	   r1=dx/2;
	   r2=100-2*dx-dx_label_1-dx+1;
	   XlLinea(cw,XtWindow(cw),cw->indicErr.ago_gc,
	      dx+(int)(r1*cos(posizione)),
                           100-dy-(int)(r1*sin(posizione)),
	      dx+(int)(r2*cos(posizione)),
                           100-dy-(int)(r2*sin(posizione)));
            }
/*
	Ago errore
*/

valore_err = valore_err*scalamento_err+offset_err;
if(valore_err>max_err)
	valore_err=max_err;
else
	if(valore_err<min_err)
        	valore_err=min_err;
if(min_err==max_err)
	valore_err=min_err;

	posizione_err=((valore_err-min_err)/(max_err-min_err))*PIGRECO/4+
		PIGRECO/8;
	if((cw->indicErr.old_posizione_err!=posizione_err)||modo)
	   {
	   if((cw->indicErr.old_posizione_err>=0)&& !modo)
	      {
	      r1=100-2*dx-(dx_label_1-12)-dx+1;
	      r2=100-2*dx-(dx_label_1-22)-dx+1;
	      XlLinea(cw,XtWindow(cw),cw->indicErr.ago_gc,
		       dx+(int)(r1*cos(cw->indicErr.old_posizione_err)),
                              100-dy-(int)(r1*sin(cw->indicErr.old_posizione_err)),
		       dx+(int)(r2*cos(cw->indicErr.old_posizione_err)),
                              100-dy-(int)(r2*sin(cw->indicErr.old_posizione_err)));
	      }
	   cw->indicErr.old_posizione_err=posizione_err;
	   r1=100-2*dx-(dx_label_1-12)-dx+1;
	   r2=100-2*dx-(dx_label_1-22)-dx+1;
	   XlLinea(cw,XtWindow(cw),cw->indicErr.ago_gc,
	      dx+(int)(r1*cos(posizione_err)),
                           100-dy-(int)(r1*sin(posizione_err)),
	      dx+(int)(r2*cos(posizione_err)),
                           100-dy-(int)(r2*sin(posizione_err)));
	   }
}

static Boolean Compile(Widget w)
{
Boolean errors=False;
XlIndicErrWidget cw= (XlIndicErrWidget)w;

if(!XtIsWidget(w))
	{
	XlWarning("XlIndicErr","Compile",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}
/*
 Deve esistere la variabile di input
*/
if(strlen(cw->indicErr.str_input)==0)
        {
	XlErrComp(w,"Compile","Input variable not defined",NULL);
        errors=True;
	}
if(strlen(cw->indicErr.str_input_err)==0)
        {
	XlErrComp(w,"Compile","Input variable for error not defined",NULL);
        errors=True;
        }


if(xlIndicErrClassRec.xlcore_class.compilatore==NULL)
	{
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
	return(False);
	}

if(!(*superclass->xlcore_class.compile) (w,
     xlIndicErrClassRec.xlcore_class.compilatore))
	{
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
	return(False);
	}
XlEvError(cw,errors);
return(True);
}


/*
        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
return(XlHardCopyWidget(w));
}
#endif
