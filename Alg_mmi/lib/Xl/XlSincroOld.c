/**********************************************************************
*
*       C Source:               XlSincroOld.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:39:17 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlSincroOld.c-2.2.3 %  (%full_filespec: XlSincroOld.c-2.2.3:csrc:1 %)";
#endif
/*
   modulo XlSincroOld.c
   tipo 
   release 1.4
   data 1/15/96
   reserved @(#)XlSincroOld.c	1.4
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlSincroOld.c	1.4\t1/15/96";
/*
        Fine sezione per SCCS
*/
/*
 *  XlSincroOld.c - widget sincroOld per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlSincroOldP.h>
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
        XtOffsetOf(XlSincroOldRec,sincroOld.str_input_1),
        XmRString,
        ""
        },
        {
        XlNvarInput_2,
        XlCVarInput_2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlSincroOldRec,sincroOld.str_input_2),
        XmRString,
        ""
        },
        {
        XlNvarInput_3,
        XlCVarInput_3,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlSincroOldRec,sincroOld.str_input_3),
        XmRString,
        ""
        },
        {
        XlNvarInput_4,
        XlCVarInput_4,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlSincroOldRec,sincroOld.str_input_4),
        XmRString,
        ""
        },
        {
        XlNvarInput_5,
        XlCVarInput_5,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlSincroOldRec,sincroOld.str_input_5),
        XmRString,
        ""
        },
        {
        XlNvarInput_6,
        XlCVarInput_6,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlSincroOldRec,sincroOld.str_input_6),
        XmRString,
        ""
        },
 
        {
	XlNvarOut_1,
	XlCVarOut_1,
	XmRString,
	sizeof(char *),
	XtOffsetOf(XlSincroOldRec,sincroOld.str_out_1),
	XmRImmediate,
	""
	},       
  	{
	XlNvarOut_2,
	XlCVarOut_2,
	XmRString,
	sizeof(char *),
	XtOffsetOf(XlSincroOldRec,sincroOld.str_out_2),
	XmRImmediate,
	""
	}, 
        {
        XlNnormFg,
        XlCNormFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlSincroOldRec,sincroOld.norm_fg),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNagoFg,
        XlCAgoFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlSincroOldRec,sincroOld.ago_fg),
        XmRString,
        "red"
        },
        {
        XlNsincroOldDeltaFase,
        XlCSincroOldDeltaFase,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlSincroOldRec,sincroOld.delta_fase),
        XtRString,
        SINCRO_DELTA_FASE
        },
	{
        XlNsincroOldDeltaV,
        XlCSincroOldDeltaV,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlSincroOldRec,sincroOld.delta_v),
        XtRString,
        SINCRO_DELTA_V
        },
        {
        XlNsincroOldDeltaF,
        XlCSincroOldDeltaF,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlSincroOldRec,sincroOld.delta_f),
        XtRString,
        SINCRO_DELTA_F
        },
        {
        XlNsincroOldTimeAgg,
        XlCSincroOldTimeAgg,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlSincroOldRec,sincroOld.time_agg),
        XtRString,
        SINCRO_TIME_AGG
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
        XtOffsetOf(XlSincroOldRec,sincroOld.input_1),
        sizeof(PUNT_VARINP)
        },
        {
        XlNvarInput_2,
        XlCVarInput_2,
        XlDvarInput_2,
	XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSincroOldRec,sincroOld.input_2),
        sizeof(PUNT_VARINP)
        },
        {
        XlNvarInput_3,
        XlCVarInput_3,
        XlDvarInput_3,
	XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSincroOldRec,sincroOld.input_3),
        sizeof(PUNT_VARINP)
        },
        {
        XlNvarInput_4,
        XlCVarInput_4,
        XlDvarInput_4,
	XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSincroOldRec,sincroOld.input_4),
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
        XtOffsetOf(XlSincroOldRec,sincroOld.input_5),
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
        XtOffsetOf(XlSincroOldRec,sincroOld.input_6),
        sizeof(PUNT_VARINP)
        },
 
      	{
	XlNvarOut_1, 
	XlCVarOut_1,
        XlDvarOut_1,
	XlRTipoVarOut,
        XlRVarXO, 
	NULL,
	XlRGrIo,
        XtOffsetOf(XlSincroOldRec,sincroOld.out_1),
	sizeof(PUNT_VAROUT)
	},
	{
	XlNvarOut_2, 
	XlCVarOut_2,
        XlDvarOut_2,
	XlRTipoVarOut,
        XlRVarXO, 
	NULL,
	XlRGrIo,
        XtOffsetOf(XlSincroOldRec,sincroOld.out_2),
	sizeof(PUNT_VAROUT)
	},

        {
        XlNnormFg,
        XlCNormFg,
        XlDnormFg,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlSincroOldRec,sincroOld.norm_fg),
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
        XtOffsetOf(XlSincroOldRec,sincroOld.ago_fg),
        sizeof(Pixel),
        },
        {
        XlNsincroOldDeltaFase,
        XlCSincroOldDeltaFase,
        XlDsincroOldDeltaFase,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSincroOldRec,sincroOld.delta_fase),
        sizeof(float)
        },
        {
        XlNsincroOldDeltaV,
        XlCSincroOldDeltaV,
        XlDsincroOldDeltaV,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSincroOldRec,sincroOld.delta_v),
        sizeof(float)
        },
        {
        XlNsincroOldDeltaF,
        XlCSincroOldDeltaF,
        XlDsincroOldDeltaF,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSincroOldRec,sincroOld.delta_f),
        sizeof(float)
        },
        {
        XlNsincroOldTimeAgg,
        XlCSincroOldTimeAgg,
        XlDsincroOldTimeAgg,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSincroOldRec,sincroOld.time_agg),
        sizeof(float)
        },
        };
/* dichiarazioni funzioni varie */

static int min();
static void GetNormFgGC();
static void GetClearGC();
static void GetAgoGC();
static void GetAllGCs();

static void DrawIntoDirectly();
static void DrawAgoValoreSincroOld();
         
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
XlSincroOldClassRec xlSincroOldClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "SincroOld",
    /* widget_size              */      sizeof(XlSincroOldRec),
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
  { /* sincroOld fields */
    /* empty                   */      0,
  }
};

WidgetClass xlSincroOldWidgetClass = (WidgetClass) &xlSincroOldClassRec;


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlSincroOldWidget new = (XlSincroOldWidget)tnew;
int app_int;
Arg arg[2];



if(new->core.width<DEFAULTWIDTH)
	{
        new->core.width=DEFAULTWIDTH;
	XlWarning("XlSincroOld","Initialize",
	     "width minore della dimensione minima");
	}
if(new->core.height<DEFAULTHEIGHT)
	{
        new->core.height=DEFAULTWIDTH;
	XlWarning("XlSincroOld","Initialize",
		"height minore della dimensione minima");
	}


if((new->core.width!=new->core.height))
        {
        new->core.width=min(new->core.width,new->core.height);
        new->core.height=new->core.width;
	XlWarning("XlSincroOld","Initialize","width diverso da height");
        }

/*******************************
ATTENZIONE DA CONTROLLARE!!!!
*****************/

   

if(new->xlcore.rotate>3)
	{
	XlWarning("XlSincroOld","Initialize","rotate > 3");
	new->xlcore.rotate=new->xlcore.rotate%4;
	}

if(new->xlcore.ass_rotate>1)
	{
	XlWarning("XlSincroOld","Initialize","assRotate > 1");
	new->xlcore.ass_rotate=new->xlcore.ass_rotate%2;
	}

if((new->sincroOld.delta_fase>360)||(new->sincroOld.delta_fase<-360))
	{
	XlWarning("XlSincroOld","Initialize",
	    "La differenza di fase è fuori fondo_scala");
	new->sincroOld.delta_fase=atof(SINCRO_DELTA_FASE);
	}
if(new->sincroOld.time_agg<0)	/* tempo di agg. positivo*/
	{
	XlWarning("XlSincroOld","Initialize",
	    "Il valore del tempo fra 2 Refresh_Fast deve essere positivo");
	new->sincroOld.time_agg=atof(SINCRO_TIME_AGG);
	}
/*
 Il valore di extra range viene per ora utilizzato solo
 nel caso di  display a barra (negli altri casi il valore
 dell'extra range viene per il momento forzato a zero
*/

 new->sincroOld.old_posizione = -1;	 	/*private*/
 
 memset(&(new->sincroOld.OldFlag),sizeof(FLAG_MMI),0);

 /*
 Inizializzazione delle variabili di input ed ouput
 */
 new->sincroOld.input_1.valore=0;
 new->sincroOld.input_2.valore=0;
 new->sincroOld.input_3.valore=0;
 new->sincroOld.input_4.valore=0;
 new->sincroOld.input_5.valore=0;	
 new->sincroOld.input_6.valore=0;
 
/*
Inizializz. variabili private
*/
 new->sincroOld.old_valore=0;
 new->sincroOld.OldPixel = -1;
 new->sincroOld.primo_refresh = 1;
 new->sincroOld.old_parallelo_ok=0;
 new->sincroOld.reset_output=1;
 	
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
XlSincroOldWidget curcw= (XlSincroOldWidget) current;
XlSincroOldWidget newcw= (XlSincroOldWidget) new;
Boolean do_redisplay = False;
Arg core_args[2];

int app_int;


if(curcw->sincroOld.norm_fg != newcw->sincroOld.norm_fg)
        {
        XtReleaseGC(curcw,curcw->sincroOld.norm_gc);
        XtReleaseGC(curcw,curcw->sincroOld.clear_gc);
        GetNormFgGC(newcw);
        GetClearGC(newcw);
        DrawIntoDirectly(newcw,False);
        DrawAgoValoreSincroOld(newcw,1,False);
        do_redisplay = True;
        }

if(curcw->sincroOld.ago_fg != newcw->sincroOld.ago_fg)
        {
        XtReleaseGC(curcw,curcw->sincroOld.ago_gc);
        GetAgoGC(newcw);
        DrawIntoDirectly(newcw,False);
        DrawAgoValoreSincroOld(newcw,1,False);
        do_redisplay = True;
        }

if(curcw->core.background_pixel != newcw->core.background_pixel)
        {
        XtReleaseGC(curcw,curcw->sincroOld.norm_gc);
        XtReleaseGC(curcw,curcw->sincroOld.clear_gc);
        XtReleaseGC(curcw,curcw->sincroOld.ago_gc);
        GetAllGCs(newcw,False);
        DrawIntoDirectly(newcw,False);
        DrawAgoValoreSincroOld(newcw,1,False);
        do_redisplay = True;
        }

	
if(curcw->sincroOld.delta_fase!=newcw->sincroOld.delta_fase)
        {
	if((newcw->sincroOld.delta_fase>360)||(newcw->sincroOld.delta_fase<-360))
		{
		XlWarning("XlSincroOld","SetVAlues",
		    "Differenza Fasi furi dai limiti");
		newcw->sincroOld.delta_fase=curcw->sincroOld.delta_fase;
		}
	else
		{
        	DrawIntoDirectly(newcw,False);
        	DrawAgoValoreSincroOld(newcw,1,False);
        	do_redisplay = True;
		}
        }

if(curcw->sincroOld.delta_fase!=newcw->sincroOld.delta_fase)
        {
	DrawIntoDirectly(newcw,False);
        DrawAgoValoreSincroOld(newcw,1,False);
        do_redisplay = True;
	}
if(curcw->sincroOld.delta_v!=newcw->sincroOld.delta_v)
        {
	DrawIntoDirectly(newcw,False);
        DrawAgoValoreSincroOld(newcw,1,False);
        do_redisplay = True;
	}
if(curcw->sincroOld.delta_f!=newcw->sincroOld.delta_f)
        {
	DrawIntoDirectly(newcw,False);
        DrawAgoValoreSincroOld(newcw,1,False);
        do_redisplay = True;
	}

if(curcw->sincroOld.time_agg!=newcw->sincroOld.time_agg)
        {
	if(newcw->sincroOld.time_agg<0)
		{
		XlWarning("XlSincroOld","SetVAlues",
		    "Il valore dell' intervallo di tempo per il refresh deve essere>0");
		newcw->sincroOld.time_agg=atof(SINCRO_TIME_AGG);
		}
	else
		{
        	DrawIntoDirectly(newcw,False);
        	DrawAgoValoreSincroOld(newcw,1,False);
        	do_redisplay = True;
		}
        }



/*
 risorse connesse con sistema di simulazione
*/


if(strcmp(newcw->sincroOld.str_input_1,curcw->sincroOld.str_input_1))
        {
        Compile(newcw);
        do_redisplay = False;
        }

if(strcmp(newcw->sincroOld.str_input_2,curcw->sincroOld.str_input_2))
        {
        Compile(newcw);
        do_redisplay = False;
        }
if(strcmp(newcw->sincroOld.str_input_3,curcw->sincroOld.str_input_3))
        {
        Compile(newcw);
        do_redisplay = False;
        }

if(strcmp(newcw->sincroOld.str_input_4,curcw->sincroOld.str_input_4))
        {
        Compile(newcw);
        do_redisplay = False;
        }
if(strcmp(newcw->sincroOld.str_input_5,curcw->sincroOld.str_input_5))
        {
        Compile(newcw);
        do_redisplay = False;
        }

if(strcmp(newcw->sincroOld.str_input_6,curcw->sincroOld.str_input_6))
        {
        Compile(newcw);
        do_redisplay = False;
        }
              
        
if(strcmp(newcw->sincroOld.str_out_1,curcw->sincroOld.str_out_1))
        {
        Compile(newcw);
        do_redisplay = False;
        }

if(strcmp(newcw->sincroOld.str_out_2,curcw->sincroOld.str_out_2))
        {
        Compile(newcw);
        do_redisplay = False;
        }


return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlSincroOldWidget cw= (XlSincroOldWidget) w;

/*
Se c'è stato il primo refresh allora rimuovi l' evento di time out
*/

if(!(cw->sincroOld.primo_refresh))
	XtRemoveTimeOut(cw->sincroOld.tempo_id);

/* rilascio i 3 GC */
if(cw->sincroOld.norm_gc)
	XtReleaseGC(cw,cw->sincroOld.norm_gc);

if(cw->sincroOld.clear_gc)
	XtReleaseGC(cw,cw->sincroOld.clear_gc);

if(cw->sincroOld.ago_gc)
	XtReleaseGC(cw,cw->sincroOld.ago_gc);

/*Verificare se è necessario inserire innesco Refresh_Fast */

}

static void GetNormFgGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
XlSincroOldWidget cw= (XlSincroOldWidget) w;

unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;


values.line_width = 1;
values.line_style = LineSolid;

#if defined XPRINTER_USED
if(modoPrint)
	{
	values.foreground = XlPrintGetPixel(w,cw->sincroOld.norm_fg);
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->sincroOld.norm_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cw->sincroOld.norm_fg;
	values.background = cw->core.background_pixel;
        cw->sincroOld.norm_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->sincroOld.norm_fg;
values.background = cw->core.background_pixel;
cw->sincroOld.norm_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetClearGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle ;
XlSincroOldWidget cw= (XlSincroOldWidget) w;
values.line_width = 0;
values.line_style = LineSolid;

#if defined XPRINTER_USED
if(modoPrint)
	{
	values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
	values.background = XlPrintGetPixel(w,cw->sincroOld.norm_fg);
        cw->sincroOld.clear_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cw->core.background_pixel;
	values.background = cw->sincroOld.norm_fg;
        cw->sincroOld.clear_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->core.background_pixel;
values.background = cw->sincroOld.norm_fg;
cw->sincroOld.clear_gc = XtGetGC(cw, valuemask, &values);
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
XlSincroOldWidget cw= (XlSincroOldWidget) w;
Widget Padre;

values.line_width = 0;
values.line_style = LineSolid;

#if defined XPRINTER_USED
if(modoPrint)
	{
	values.function=GXcopy;
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
	values.foreground = XlPrintGetPixel(w,cw->sincroOld.ago_fg);
        cw->sincroOld.ago_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
	}
else
	{
	values.function=GXxor;
	values.background = cw->core.background_pixel;
	values.foreground = cw->sincroOld.ago_fg ^ cw->core.background_pixel;
        cw->sincroOld.ago_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.function=GXxor;
values.background = cw->core.background_pixel;
values.foreground = cw->sincroOld.ago_fg ^ cw->core.background_pixel;
cw->sincroOld.ago_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetAllGCs(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XlSincroOldWidget cw= (XlSincroOldWidget)w;

GetNormFgGC(w,modoPrint);
GetAgoGC(w,modoPrint);
GetClearGC(w,modoPrint);

}

static void DrawIntoDirectly(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XlSincroOldWidget cw= (XlSincroOldWidget)w;
int width,height;
/* questo gruppo di variabili deve essere riportato nella routine DrawAgoValore */
int dx=10;
int dy=10;
int dx_label_1=10;
int dy_label_1=10;
int dx_label_2=10;
int dy_label_2=50;
int r1,r2,r3;
char testo[15];




width=cw->core.width;
height=cw->core.height;

/*
        se necessario sbianca lo sfondo
*/
if((cw->xlcore.trasparent!=True)&& !modoPrint)
	XlRiempiRettangolo(cw,XlWindow(cw),cw->sincroOld.clear_gc,0,0,100,100);
	
r1=50-3*dx/2;
r2=r1-dx/2;	
	XlArco(cw,XlWindow(cw),cw->sincroOld.norm_gc,
		50-r1,50-r1,
		2*r1,2*r1,0*64,360*64);
/*
**********************Linee verticali***************

	XlLinea(cw,XlWindow(cw),cw->sincroOld.norm_gc,
		50,50-r1,50,50-r1+dy);	*/
/*
correzione
*/
	XlLinea(cw,XlWindow(cw),cw->sincroOld.norm_gc,
		51,50-(int)(r1*sin(PIGRECO/2-0.05)),
		51,50-(int)(r1*sin(PIGRECO/2-0.05))+dy/2);
		

/*
cerchio piccolo 
*/	
	XlArco(cw,XlWindow(cw),cw->sincroOld.norm_gc,
		50-dx/2,50-dy/2,
		dx,dy,0*64,360*64);

	XlRiempiArco(cw,XlWindow(cw),cw->sincroOld.norm_gc,
		50-dx/2,50-dy/2,
		dx,dy,0*64,360*64);
/*
cornici esterne	x=49 ampiezza 2
*/
	
	XlRettangolo(cw,XlWindow(cw),cw->sincroOld.norm_gc,
		dx,dy,100-2*dx,100-2*dy);
	
/*
Disegna rettangolo per indicatore e riempilo
*/		
	XlRettangolo(cw,XlWindow(cw),cw->sincroOld.norm_gc,
		50-(int)(r1*cos(PIGRECO/2-0.05)),50-(int)(r1*sin(PIGRECO/2-0.05)),
		(2*(int)(r1*cos(PIGRECO/2-0.05))),(int)(dy/2));	

XlRiempiRettangolo(cw,XlWindow(cw),cw->sincroOld.norm_gc,
		50-(int)(r1*cos(PIGRECO/2-0.05)),50-(int)(r1*sin(PIGRECO/2-0.05)),
		(2*(int)(r1*cos(PIGRECO/2-0.05))),(int)(dy/2));

/*
Arco disegnato come punta dell' indicatore e riempito
*/

XlArco(cw,XlWindow(cw),cw->sincroOld.norm_gc,
		50-(int)(r1*cos(PIGRECO/2-0.05)),50-(int)(r1*sin(PIGRECO/2-0.05))+dy/2-(int)(r1*cos(PIGRECO/2-0.05)),
		(2*(int)(r1*cos(PIGRECO/2-0.05))),(2*(int)(r1*cos(PIGRECO/2-0.05))),
		180*64,360*64);

XlRiempiArco(cw,XlWindow(cw),cw->sincroOld.norm_gc,
		50-(int)(r1*cos(PIGRECO/2-0.05)),50-(int)(r1*sin(PIGRECO/2-0.05))+dy/2-(int)(r1*cos(PIGRECO/2-0.05)),
		(2*(int)(r1*cos(PIGRECO/2-0.05))),(2*(int)(r1*cos(PIGRECO/2-0.05))),
		180*64,360*64);
	
}
/*Refresh veloce innescata per la prima volta in Refresh*/
void Refresh_FastOld(Widget w)
{
XtIntervalId timer_id;	/*non static*/
XlSincroOldWidget cw= (XlSincroOldWidget)w;


DrawAgoValoreSincroOld(w,0,False);	/*modo 0*/

/*
Innesca nuovamente la Refresh_FastOld che viene invocata dopo time_agg istanti 
di tempo
*/
cw->sincroOld.tempo_id = XtAppAddTimeOut (
            XtWidgetToApplicationContext(w),
            (unsigned long) (cw->sincroOld.time_agg), Refresh_FastOld,(Widget) w);
}

static Boolean Refresh(Widget w)
{
XlSincroOldWidget cw= (XlSincroOldWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;
int core_x=cw->core.x;
int core_y=cw->core.y;
Boolean old_blink;
XtIntervalId timer_id;

if(cw->xlcore.config==True)
        return(True);

/*La Refresh aggiorna i valori delle variabili,mentre la Refresh_FastOld disegna
l' ago in base a tali valori*/

if(!XtIsWidget(w))
	{
	XlWarning("XlSincroOld","Refresh",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}

if(xlSincroOldClassRec.xlcore_class.database==NULL)
	{
	XlWarning("XlSincroOld","Refresh","database non definito");
	return(False);
	}

/*
Refresh di core:acquisizione dei valori delle 6 var. di input
*/

if(!(*superclass->xlcore_class.refresh) (w))
	{
	XlWarning("XlSincroOld","Refresh","errore refresh classe superiore");
	return(False);
	}
/*
Se la è la 1a invocazione di Refresh, allora innesca la Refresh_FastOld
*/	

if(cw->sincroOld.primo_refresh== 1) 	/*Se primo refresh... */
        {

/*
La Refresh_fast non deve più essere innescata da Refresh, quindi
la var privata primo_refresh viene posta a 0
*/
        cw->sincroOld.primo_refresh = 0;  

       
        				/*...innesco la Refresh_Fast*/
       
        cw->sincroOld.tempo_id = XtAppAddTimeOut (
            XtWidgetToApplicationContext(w),
            (unsigned long) cw->sincroOld.time_agg, Refresh_FastOld,w);
        }


return(True);
}


static void Resize(w)
Widget w;
{
XlSincroOldWidget cw= (XlSincroOldWidget)w;
int app_int;

DrawIntoDirectly(cw,False);
DrawAgoValoreSincroOld(cw,1,False);
}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlSincroOldWidget cw= (XlSincroOldWidget)w;
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
DrawAgoValoreSincroOld(cw,1,False);
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


 
float RefreshOutOld(Widget w, OlDatabasePuntiObject database)
{
/*
Esegue l' estrazione del valore corrente di output_2.value
*/
XlSincroOldWidget cw= (XlSincroOldWidget)w;
/*
valvar ha un parametro di tipo PUNT_VARINP: una conversione viene effettuata 
*/
PUNT_VARINP varinp;

varinp.pdb=cw->sincroOld.out_2.pdb;
varinp.unimis=cw->sincroOld.out_2.unimis;
/*
chiamata a valvar
*/
varinp.valore=(olDatabasePuntiClassRec.oldatabasePunti_class.valvar)
                           (database,varinp)*varinp.unimis.a+
			   varinp.unimis.b;
return(varinp.valore); 
}

static void DisegnaAgo(Widget w,float posizione,int modo)
{
XlSincroOldWidget cw= (XlSincroOldWidget)w;
int dx=10;
int dy=10;
int r1;

r1=50-3*dx/2;	/*raggio esterno*/

		
		
	        if(!modo) 	/*cancella ago in vecchia pos.*/
	        	{
	      
	      		XlLinea(cw,XtWindow(cw),cw->sincroOld.ago_gc,
		       		50+(int)((dx/2+1)*cos(cw->sincroOld.old_posizione)),
                              	50-(int)((dy/2+1)*sin(cw->sincroOld.old_posizione)),
		       		50+(int)(r1*cos(cw->sincroOld.old_posizione)),
                              	50-(int)(r1*sin(cw->sincroOld.old_posizione)));
	      		}
	   	cw->sincroOld.old_posizione=posizione;
/*
Disegna ago in nuova posizione
*/
	   
	   	XlLinea(cw,XtWindow(cw),cw->sincroOld.ago_gc,
	      	50+(int)((dx/2+1)*cos(posizione)),
                50-(int)((dy/2+1)*sin(posizione)),
	      	50+(int)(r1*cos(posizione)),
                50-(int)(r1*sin(posizione)));
	   

}
static void DrawAgoValoreSincroOld(w,modo,modoPrint)
Widget w;
int modo;	/*=0 se invocata da Refresh e/o RefreshFastOld */
Boolean modoPrint;
{
XlSincroOldWidget cw= (XlSincroOldWidget)w;


int parallelo_ok; /*consenso ad effettuare il parallelo */
float valore;	/*valore angolo in °*/

float val_rad;	/*valore espresso in radianti*/

float posizione; /*posizione angolare dell' ago */	
float delta;	/*differenza fra input2 e 1*/
float delta2;	/*differenza fra input4 e 3*/
float actual_out_2;	/*valore reale di out_2*/




parallelo_ok=0;
      	
/*
Calcolo valore posizione
*/

delta=cw->sincroOld.input_2.valore-cw->sincroOld.input_1.valore;
delta2=cw->sincroOld.input_4.valore-cw->sincroOld.input_3.valore;

/*
Se input_5!=0 o se invocaz. non da Refresh, allora
calcola valore 
*/
if((cw->sincroOld.input_5.valore!=0)||modo)
	{
	if (modo)/*Se invocazione non da refresh*/
		{
		valore=cw->sincroOld.old_valore+delta*360/1000;/*invocazione non da refresh*/
		}
	else	/*invocaz. da refresh*/
		{
/*
Se input_6==1, allora la fase deve essere 0 
*/		
		if(cw->sincroOld.input_6.valore==1)	
			valore=0;
		else	
			valore=cw->sincroOld.old_valore+delta*cw->sincroOld.time_agg*360/1000;/*incr. posizione in °*/
/*
Poichè l' invocazione è avvenuta da refresh i valori degli ouput potrebbero 
cambiare: setta la var privata reset output ad 1.Se input_5 andrà a 0 gli
ouputs saranno resettati 
*/
		
		cw->sincroOld.reset_output=1;	
		}
	cw->sincroOld.old_valore=valore;/*aggiorna old valore*/


/*
calcola valore posizione
*/
	valore=valore+90;	/*Lo 0 è a90°*/
	posizione=valore*(PIGRECO/2)/90;	/*posizione in radianti*/


/*
Se la posizione è cambiata o l' invocaz. non è da refresh ridisegna l'ago
*/
	if((cw->sincroOld.old_posizione!=posizione)||modo)
		DisegnaAgo(cw,posizione,modo);


	
/*
Setta le uscite in base al verificarsi di certe condizioni
*/	if(!modo)
	 	val_rad=(valore-90.0)/90.0*(PIGRECO/2);

/*
Se invocaz. non da refresh e le tre condiz. sono verificate dai conenso al
parallelo 
*/

	 if((!modo)&&(cos(val_rad)>cos(cw->sincroOld.delta_fase*PIGRECO/180))&&
	 (fabs(delta)<cw->sincroOld.delta_f)&&
	 (fabs(delta2)<cw->sincroOld.delta_v))
	 	{
	 	parallelo_ok=1;
	 	}
	 else if(!modo)
	 	{
	       	parallelo_ok=0;
	       	}
/*
Se il consenso è cambiato out1=1 se consenso OK altrimenti out1=0
*/

	 if((!modo)&&(parallelo_ok!=cw->sincroOld.old_parallelo_ok))
	       	{
		cw->sincroOld.out_2.tipo_pert=PERT_SCALINO;
	       	if(parallelo_ok==1)
	       		{
	       		
	       		
	       		cw->sincroOld.out_1.tipo_pert=PERT_UP;
     			cw->sincroOld.out_2.value=0;
			printf("Ho fatto PERT_UP\n");
			
			}
	       	else if(parallelo_ok==0)
	       		{

	       		
		       	cw->sincroOld.out_1.tipo_pert=PERT_DOWN;
     			cw->sincroOld.out_2.value=1;		       	
	       		
			printf("Ho fatto la PERT_DOWN\n");
						
			}

		(olPertClassRec.olpert_class.pertsend)(xlSincroOldClassRec.xlcore_class.pert,
		cw->sincroOld.out_2);        				 	

		(olPertClassRec.olpert_class.pertsend)(xlSincroOldClassRec.xlcore_class.pert,
		cw->sincroOld.out_1);
	       	cw->sincroOld.old_parallelo_ok=parallelo_ok;
	       	}

	
		
        }/*end if input_5!=0*/

else if((!modo)&&(cw->sincroOld.input_5.valore==0)) 
	{
	DisegnaAgo(cw,3*PIGRECO/2,modo);	/* ago a 180°*/
	if(cw->sincroOld.reset_output)		/*controlla se resettare*/

		{
		cw->sincroOld.reset_output=0;
/*		
Esegui una PERT_DOWN->out1=0;
*/		
		cw->sincroOld.out_1.tipo_pert=PERT_DOWN;
		(olPertClassRec.olpert_class.pertsend)(xlSincroOldClassRec.xlcore_class.pert,
		cw->sincroOld.out_1);
/*
Esegui una step=0->out1=0;
*/		cw->sincroOld.out_2.tipo_pert=PERT_SCALINO;
		cw->sincroOld.out_2.value=0;
		(olPertClassRec.olpert_class.pertsend)(xlSincroOldClassRec.xlcore_class.pert,
		cw->sincroOld.out_2); 
		}/*end if reset_output==1*/
	}/*end if input_5==0*/
}

static Boolean Compile(Widget w)
{
Boolean errors=False;
XlSincroOldWidget cw= (XlSincroOldWidget)w;

if(!XtIsWidget(w))
	{
	XlWarning("XlSincroOld","Compile",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}

/*
 Deve esistere la variabile di input
*/

if(strlen(cw->sincroOld.str_input_1)==0)
        {
	XlErrComp(w,"Compile","Input number 1 not defined",NULL);
        errors=True;
        }
if(strlen(cw->sincroOld.str_input_2)==0)
        {
	XlErrComp(w,"Compile","Input number 2 not defined",NULL);
        errors=True;
        }
if(strlen(cw->sincroOld.str_input_3)==0)
        {
	XlErrComp(w,"Compile","Input number 3 not defined",NULL);
        errors=True;
        }
if(strlen(cw->sincroOld.str_input_4)==0)
        {
	XlErrComp(w,"Compile","Input number 4 not defined",NULL);
        errors=True;
        }
if(strlen(cw->sincroOld.str_input_5)==0)
        {
	XlErrComp(w,"Compile","Input number 5 not defined",NULL);
        errors=True;
        }
if(strlen(cw->sincroOld.str_input_6)==0)
        {
	XlErrComp(w,"Compile","Input number 6 not defined",NULL);
        errors=True;
        }
              
if(strlen(cw->sincroOld.str_out_1)==0)
        {
	XlErrComp(w,"Compile","Output number 1 not defined",NULL);
        errors=True;
        }

if(strlen(cw->sincroOld.str_out_2)==0)
        {
	XlErrComp(w,"Compile","Output number 2 not defined",NULL);
        errors=True;
        }
		

if(xlSincroOldClassRec.xlcore_class.compilatore==NULL)
	{
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
	return(False);
	}


if(!(*superclass->xlcore_class.compile) (w,
     xlSincroOldClassRec.xlcore_class.compilatore))
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
XlSincroOldWidget cw= (XlSincroOldWidget) w;
char *printerFont;

/*
	richiamo il metodo di print di Core
*/



/*
        richiamo il ridisegno
*/


/* rilascio i 3 GC */
if(cw->sincroOld.norm_gc)
	XtReleaseGC(w,cw->sincroOld.norm_gc);
if(cw->sincroOld.clear_gc)
	XtReleaseGC(w,cw->sincroOld.clear_gc);
if(cw->sincroOld.ago_gc)
	XtReleaseGC(w,cw->sincroOld.ago_gc);

		


GetAllGCs(w,True);


/*
	redisplay del sincroOld
*/
DrawIntoDirectly(cw,True);
DrawAgoValoreSincroOld(cw,1,True);


/* rilascio i 3 GC  della stampante*/
if(cw->sincroOld.norm_gc)
        XFreeGC(XlDisplay(w),cw->sincroOld.norm_gc);
if(cw->sincroOld.clear_gc)
        XFreeGC(XlDisplay(w),cw->sincroOld.clear_gc);
if(cw->sincroOld.ago_gc)
        XFreeGC(XlDisplay(w),cw->sincroOld.ago_gc);
/*XlFreeCambioColoreGC(cw,cw->sincroOld.cambio_colore[0],True);*/


GetAllGCs(cw,False);

return(True);
}
#endif
