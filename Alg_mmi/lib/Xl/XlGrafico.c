/**********************************************************************
*
*       C Source:               XlGrafico.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 16:46:20 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlGrafico.c-3.9 %  (%full_filespec: XlGrafico.c-3.9:csrc:1 %)";
#endif
/*
   modulo XlGrafico.c
   tipo 
   release 5.5
   data 3/28/96
   reserved @(#)XlGrafico.c	5.5
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlGrafico.c	5.5\t3/28/96";
/*
        Fine sezione per SCCS
*/
/*
 *  XlGrafico.c - widget cambio pagina
 */
#include <stdio.h>
#if defined VMS
#include <stdlib.h>
#endif
#include <string.h>
#include <math.h>
#include <limits.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlP.h>
#include <X11/CoreP.h>
#include <Xl/XlCoreP.h>
#include <X11/CompositeP.h>
#include <Xl/XlGraficoP.h>

#define MIN_WIDTH       5
#define MIN_HEIGHT      5


/* lista delle risorse  */
static XtResource resources[]= {
 	{
        XlNgraficoType,
        XlCGraficoType,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlGraficoRec,grafico.tipo),
        XmRImmediate,
        (XtPointer)ARCH_GRAF
        },
        {
        XlNcolorTrend1,
        XlCColorTrend1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlGraficoRec,grafico.col_trend[0]),
        XmRString,
        "#7b7b7b7bffff"
        },
        {
        XlNcolorTrend2,
        XlCColorTrend2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlGraficoRec,grafico.col_trend[1]),
        XmRString,
        "#ffffc1c10000"
        },
        {
        XlNcolorTrend3,
        XlCColorTrend3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlGraficoRec,grafico.col_trend[2]),
        XmRString,
        "#e3e34d4d3131"
        },
        {
        XlNcolorTrend4,
        XlCColorTrend4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlGraficoRec,grafico.col_trend[3]),
        XmRString,
        "#bbbbb7b7c3c3"
        },
        {
        XlNcolorGrid,
        XlCColorGrid,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlGraficoRec,grafico.col_grid),
        XmRString,
        "dark slate gray"
        },
        {
        XlNarchiveFile,
        XlCArchiveFile,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlGraficoRec,grafico.nome_file_f22),
        XmRImmediate,
        ""
        },
        {
        XlNvarName1,
        XlCVarName1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlGraficoRec,grafico.nome_var[0]),
        XmRImmediate,
        ""
        },
        {
        XlNvarName2,
        XlCVarName2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlGraficoRec,grafico.nome_var[1]),
        XmRImmediate,
        ""
        },
        {
        XlNvarName3,
        XlCVarName3,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlGraficoRec,grafico.nome_var[2]),
        XmRImmediate,
        ""
        },
        {
        XlNvarName4,
        XlCVarName4,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlGraficoRec,grafico.nome_var[3]),
        XmRImmediate,
        ""
        },
        {
        XlNvarDescr1,
        XlCVarDescr1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlGraficoRec,grafico.descr_var[0]),
        XmRImmediate,
        ""
        },
        {
        XlNvarDescr2,
        XlCVarDescr2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlGraficoRec,grafico.descr_var[1]),
        XmRImmediate,
        ""
        },
        {
        XlNvarDescr3,
        XlCVarDescr3,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlGraficoRec,grafico.descr_var[2]),
        XmRImmediate,
        ""
        },
        {
        XlNvarDescr4,
        XlCVarDescr4,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlGraficoRec,grafico.descr_var[3]),
        XmRImmediate,
        ""
        },
	{
	XlNdeltaTime,
	XlCDeltaTime,
	XmRInt,
	sizeof(int),
	XtOffsetOf(XlGraficoRec,grafico.delta_tempo),
	XmRImmediate,
	(XtPointer)60
	},
	{
	XlNmaxSamples,
	XlCMaxSamples,
	XmRInt,
	sizeof(int),
	XtOffsetOf(XlGraficoRec,grafico.max_samples),
	XmRImmediate,
        (XtPointer)14400
	},
	{
	XlNgraphTitle,
	XlCGraphTitle,
	XmRString,
	sizeof(char *),
	XtOffsetOf(XlGraficoRec,grafico.titolo_graf),
	XmRImmediate,
	""
	},
	{
	XlNautoScaling1,
	XlCAutoScaling1,
	XmRInt,
	sizeof(int),
	XtOffsetOf(XlGraficoRec,grafico.autoscaling[0]),
	XmRImmediate,
	(XtPointer)1
	},
	{
	XlNmaxVal1,
	XlCMaxVal1,
	XmRFloat,
	sizeof(float),
	XtOffsetOf(XlGraficoRec,grafico.max_val[0]),
	XmRString,
	(XtPointer)VALORE_MASSIMO_SCALA
	},
	{
	XlNminVal1,
	XlCMinVal1,
	XmRFloat,
	sizeof(float),
	XtOffsetOf(XlGraficoRec,grafico.min_val[0]),
	XmRString,
	(XtPointer)VALORE_MINIMO_SCALA
	},
	{
	XlNautoScaling2,
	XlCAutoScaling2,
	XmRInt,
	sizeof(int),
	XtOffsetOf(XlGraficoRec,grafico.autoscaling[1]),
	XmRImmediate,
	(XtPointer)1
	},
	{
	XlNmaxVal2,
	XlCMaxVal2,
	XmRFloat,
	sizeof(float),
	XtOffsetOf(XlGraficoRec,grafico.max_val[1]),
	XmRString,
	(XtPointer)VALORE_MASSIMO_SCALA
	},
	{
	XlNminVal2,
	XlCMinVal2,
	XmRFloat,
	sizeof(float),
	XtOffsetOf(XlGraficoRec,grafico.min_val[1]),
	XmRString,
	(XtPointer)VALORE_MINIMO_SCALA
	},
	{
	XlNautoScaling3,
	XlCAutoScaling3,
	XmRInt,
	sizeof(int),
	XtOffsetOf(XlGraficoRec,grafico.autoscaling[2]),
	XmRImmediate,
	(XtPointer)1
	},
	{
	XlNmaxVal3,
	XlCMaxVal3,
	XmRFloat,
	sizeof(float),
	XtOffsetOf(XlGraficoRec,grafico.max_val[2]),
	XmRString,
	(XtPointer)VALORE_MASSIMO_SCALA
	},
	{
	XlNminVal3,
	XlCMinVal3,
	XmRFloat,
	sizeof(float),
	XtOffsetOf(XlGraficoRec,grafico.min_val[2]),
	XmRString,
	(XtPointer)VALORE_MINIMO_SCALA
	},
	{
	XlNautoScaling4,
	XlCAutoScaling4,
	XmRInt,
	sizeof(int),
	XtOffsetOf(XlGraficoRec,grafico.autoscaling[3]),
	XmRImmediate,
	(XtPointer)1
	},
	{
	XlNmaxVal4,
	XlCMaxVal4,
	XmRFloat,
	sizeof(float),
	XtOffsetOf(XlGraficoRec,grafico.max_val[3]),
	XmRString,
	(XtPointer)VALORE_MASSIMO_SCALA
	},
	{
	XlNminVal4,
	XlCMinVal4,
	XmRFloat,
	sizeof(float),
	XtOffsetOf(XlGraficoRec,grafico.min_val[3]),
	XmRString,
	(XtPointer)VALORE_MINIMO_SCALA
	},
	{
	XlNfirstRefresh,
	XlCFirstRefresh,
	XmRBoolean,
	sizeof(Boolean),
	XtOffsetOf(XlGraficoRec,grafico.primo_refresh),
	XmRImmediate,
	(XtPointer)True
	}
        };

/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
        {
        XlNgraficoType,
        XlCGraficoType,
        XlDgraficoType,
        XlROption,
        NULL,
        XlOgraficoType,
        XlRGrIo,
        XtOffsetOf(XlGraficoRec,grafico.tipo),
        sizeof(int)
        },
        {
        XlNcolorTrend1,
        XlCColorTrend1,
        XlDcolorTrend1,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlGraficoRec,grafico.col_trend[0]),
        sizeof(Pixel)
        },
        {
        XlNcolorTrend2,
        XlCColorTrend2,
        XlDcolorTrend2,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlGraficoRec,grafico.col_trend[1]),
        sizeof(Pixel)
        },
        {
        XlNcolorTrend3,
        XlCColorTrend3,
        XlDcolorTrend3,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlGraficoRec,grafico.col_trend[2]),
        sizeof(Pixel)
        },
        {
        XlNcolorTrend4,
        XlCColorTrend4,
        XlDcolorTrend4,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlGraficoRec,grafico.col_trend[3]),
        sizeof(Pixel)
        },
        {
        XlNcolorGrid,
        XlCColorGrid,
        XlDcolorGrid,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlGraficoRec,grafico.col_grid),
        sizeof(Pixel)
        },
	{
	XlNarchiveFile,
	XlCArchiveFile,
	XlDarchiveFile,
	XlRText,
	NULL,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.nome_file_f22),
	sizeof(char *)
	},
	{
	XlNvarName1,
	XlCVarName1,
	XlDvarName1,
	XlRTipoVarInp,
	XlRVarXX,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.input[0]),
	sizeof(PUNT_VARINP)
	},
        {
        XlNvarName2,
        XlCVarName2,
        XlDvarName2,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlGraficoRec,grafico.input[1]),
        sizeof(PUNT_VARINP)
        },
        {
        XlNvarName3,
        XlCVarName3,
        XlDvarName3,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlGraficoRec,grafico.input[2]),
        sizeof(PUNT_VARINP)
        },
        {
        XlNvarName4,
        XlCVarName4,
        XlDvarName4,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlGraficoRec,grafico.input[3]),
        sizeof(PUNT_VARINP)
        },
	{
	XlNvarDescr1,
	XlCVarDescr1,
	XlDvarDescr1,
	XlRText,
	NULL,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.descr_var[0]),
	sizeof(char *)
	},
	{
	XlNvarDescr2,
	XlCVarDescr2,
	XlDvarDescr2,
	XlRText,
	NULL,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.descr_var[1]),
	sizeof(char *)
	},
	{
	XlNvarDescr3,
	XlCVarDescr3,
	XlDvarDescr3,
	XlRText,
	NULL,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.descr_var[2]),
	sizeof(char *)
	},
	{
	XlNvarDescr4,
	XlCVarDescr4,
	XlDvarDescr4,
	XlRText,
	NULL,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.descr_var[3]),
	sizeof(char *)
	},
        {
        XlNdeltaTime,
        XlCDeltaTime,
        XlDdeltaTime,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlGraficoRec,grafico.delta_tempo),
        sizeof(int)
        },
        {
        XlNmaxSamples,
        XlCMaxSamples,
        XlDmaxSamples,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlGraficoRec,grafico.max_samples),
        sizeof(int)
        },
        {
        XlNgraphTitle,
        XlCGraphTitle,
        XlDgraphTitle,
        XlRText,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlGraficoRec,grafico.titolo_graf),
        sizeof(char *)
        },
	{
	XlNautoScaling1,
	XlCAutoScaling1,
	XlDautoScaling1,
	XlROption,
	NULL,
	XlOautoScaling1,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.autoscaling[0]),
	sizeof(Boolean)
	},
	{
	XlNmaxVal1,
	XlCMaxVal1,
	XlDmaxVal1,
	XlRFloat,
	NULL,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.max_val[0]),
	sizeof(float)
	},
	{
	XlNminVal1,
	XlCMinVal1,
	XlDminVal1,
	XlRFloat,
	NULL,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.min_val[0]),
	sizeof(float)
	},
	{
	XlNautoScaling2,
	XlCAutoScaling2,
	XlDautoScaling2,
	XlROption,
	NULL,
	XlOautoScaling2,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.autoscaling[1]),
	sizeof(Boolean)
	},
	{
	XlNmaxVal2,
	XlCMaxVal2,
	XlDmaxVal2,
	XlRFloat,
	NULL,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.max_val[1]),
	sizeof(float)
	},
	{
	XlNminVal2,
	XlCMinVal2,
	XlDminVal2,
	XlRFloat,
	NULL,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.min_val[1]),
	sizeof(float)
	},
	{
	XlNautoScaling3,
	XlCAutoScaling3,
	XlDautoScaling3,
	XlROption,
	NULL,
	XlOautoScaling3,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.autoscaling[2]),
	sizeof(Boolean)
	},
	{
	XlNmaxVal3,
	XlCMaxVal3,
	XlDmaxVal3,
	XlRFloat,
	NULL,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.max_val[2]),
	sizeof(float)
	},
	{
	XlNminVal3,
	XlCMinVal3,
	XlDminVal3,
	XlRFloat,
	NULL,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.min_val[2]),
	sizeof(float)
	},
	{
	XlNautoScaling4,
	XlCAutoScaling4,
	XlDautoScaling4,
	XlROption,
	NULL,
	XlOautoScaling4,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.autoscaling[3]),
	sizeof(Boolean)
	},
	{
	XlNmaxVal4,
	XlCMaxVal4,
	XlDmaxVal4,
	XlRFloat,
	NULL,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.max_val[3]),
	sizeof(float)
	},
	{
	XlNminVal4,
	XlCMinVal4,
	XlDminVal4,
	XlRFloat,
	NULL,
	NULL,
	XlRGrIo,
	XtOffsetOf(XlGraficoRec,grafico.min_val[3]),
	sizeof(float)
	},
	};

/* dichiarazioni funzioni varie */
/********
static void GetClearGC();
static Boolean LoadFont();
static void ActivateToggle();
static Boolean LoadBitmap();
static void CreaFigli();
****/
static void GetAllGCs();
static void GetTrendGCs();
static void SettaActionsFigli();
static void add_translation();
static void RefreshRUN_GRAF();
static int ScalaBufDati();
static Boolean UpdateBufdati();
static Boolean ResetBufdati(float *, float *, float, int);

/* dichiarazione dei metodi (methods) */
static void Initialize();
static void Destroy();
static void Resize();
static Boolean SetValues();
static Boolean Refresh();
static void Redisplay();
static Boolean Compile();
static XtGeometryResult GeometryManager();
#if defined XPRINTER_USED
static Boolean Print();
#endif

/* actions del widget XlGrafico */
static void Seleziona();
static void AddSelez();
static void DoLayout();
static void PopupInterno();
extern void set_min_max(S_DATI *,S_XLGRAFICO *);


#define POPUP_MENU_TRANS "#override <Btn2Down>: PopupInterno(%ld)"

/* translations  */

static XtActionsRec actions[] = {
        {"Seleziona",Seleziona},
        {"AddSelez",AddSelez},
	{"PopupInterno",PopupInterno}
        };

static char defaultTranslations[]=
        "~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static char configTranslations[]="#replace\n\
        ~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static char popupTranslations[]="#replace\n\
        <Btn2Down>:       PopupInterno()";


/* Inizializzazione del class record */
XlGraficoClassRec xlGraficoClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlManagerClassRec,
    /* class_name               */      "Grafico",
    /* widget_size              */      sizeof(XlGraficoRec),
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
    /* resize                   */      Resize, /* XtInheritResize,*/ 
    /* expose                   */      Redisplay, /* XtInheritExpose, */
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
  { /* Constraint class fields   */
    /* geometry_manager          */ 	GeometryManager, /*XtInheritGeometryManager, */
    /* change_managed            */     XtInheritChangeManaged,
    /* insert_child              */     XtInheritInsertChild,
    /* delete_child              */     XtInheritDeleteChild,
    /* extension                 */     NULL
  },
  { /* legomanager fields                           */
    /* oggetto pert             */      NULL,
    /* oggetto database         */      NULL,
    /* oggetto database_topologia */    NULL,
    /* oggetto compilatore      */      NULL,
    /* informazioni per la configurazione */
    /* confinfo                 */      confinfo,
    /* num_confinfo             */      XtNumber(confinfo),
    /* refresh                  */      Refresh,
    /* compile                  */      Compile,
    /* read                     */      XtInheritRead,
    /* write                    */      XtInheritWrite,
    /* select                   */      XtInheritSelect,
    /* drawBorderIfSelected     */      XtInheritDrawBorderIfSelected,
    /* selectRunTime            */      NULL,
#if defined XPRINTER_USED
    /* print                    */      Print,
#endif
  },
  { /* grafico fields */
    /* empty                    */      0
  }
};

WidgetClass xlGraficoWidgetClass = (WidgetClass) &xlGraficoClassRec;

/*  Dichiarazione del puntatore alla struttura delle variabili comuni
	all'oggetto grafics  */
S_XLGRAFICO *pXlGraf;
extern S_UNI_MIS uni_mis[]; /* tabella che descrive tutte le unita'
                                  di misura  */

static XtGeometryResult GeometryManager(w, request, reply)
Widget w;
XtWidgetGeometry *request;
XtWidgetGeometry *reply;
{
if(request->request_mode & CWWidth)
        {
        XtResizeWidget(w,(request->width),(w->core.height),(w->core.border_width));
        }
if(request->request_mode & CWHeight)
        {
        XtResizeWidget(w,(w->core.width),(request->height),(w->core.border_width));
        }

return XtGeometryYes;
}

static void Resize(w)
Widget w;
{
XlGraficoWidget newcw= (XlGraficoWidget)w;
        Arg args[3];
        Cardinal lnargs,numChild;
        Dimension wform,hform;
        WidgetList childList;
        lnargs=0;
        XtSetArg(args[lnargs],XmNnumChildren,&numChild);lnargs++;
        XtSetArg(args[lnargs],XmNchildren,&childList);lnargs++;
        XtGetValues(newcw,args,lnargs);

        lnargs=0;
        wform=newcw->xlmanager.width0-10;
        hform= newcw->xlmanager.height0-10;
        XtSetArg(args[lnargs],XmNwidth,wform);lnargs++;
        XtSetArg(args[lnargs],XmNheight,hform);lnargs++;
        XtSetValues(childList[0],args,lnargs);
}

static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
int f_refresh;
char ParVar1[MAX_LUN_NOME_VAR],ParVar2[MAX_LUN_NOME_VAR];
char ParVar3[MAX_LUN_NOME_VAR],ParVar4[MAX_LUN_NOME_VAR];
char *argv[10];
int Lette,argc,i;
XlGraficoWidget newcw= (XlGraficoWidget) tnew;

argc = 1;

/*  Allocazione struttura per le variabili di uso comune a tutte
	le routine per l'eliminazione delle variabili globali  */
pXlGraf = (S_XLGRAFICO *)calloc ( 1, sizeof(S_XLGRAFICO) );
newcw->grafico.p=pXlGraf;

/*
	inizializza i campi screen_num, display_set, appContext_set,
				topLevel_set
*/
display_set = XtDisplay(tnew);
appContext_set = XtWidgetToApplicationContext(tnew);
screen_num = DefaultScreen(display_set);
if(XlIsXlComposite(XtParent(tnew)))
	topLevel_set = XtParent(XtParent(tnew));
else
	topLevel_set = XtParent(XtParent(tnew));

/*  Inizializza la variabile per la visualizzazione di un eventuale sforo
	del bufdati nel caso di lettua da f22  */
notify_overflow = False;

/*  Imposta tutti i GC che verranno utilizzati  */
GetAllGCs(tnew,False);

/*  Se l'oggetto si trova in configurazione viene creato senza 
	nessuna situazione di default  */
if (newcw->xlmanager.config==True)
  {
  switch (newcw->grafico.tipo)
    {
    case ARCH_GRAF:
      tipo_graf = ARCH_GRAF;
      CreateARCH_GRAF(tnew,0,NULL,False);
      break;
    case ARCH_CIRC_GRAF:
      tipo_graf = ARCH_CIRC_GRAF;
      CreateARCH_GRAF(tnew,0,NULL,False);
      break;
    case RUN_GRAF:
      tipo_graf = RUN_GRAF;
      CreateRUN_GRAF(tnew,0,NULL,False);
    break;
    case PLOT_GRAF:
      tipo_graf = PLOT_GRAF;
      CreatePLOT_GRAF(tnew,0,NULL,False);
    break;
    }
  }
else
  {
  t_restore = -1.0;
  /*  Individua il tipo di grafico  */
  switch (newcw->grafico.tipo)
    {

    /*****  Grafics di 4 misure lette da file  *****/
    case ARCH_GRAF:
    case ARCH_CIRC_GRAF:

    if(newcw->grafico.tipo==ARCH_GRAF)
    	tipo_graf = ARCH_GRAF;
    else
	tipo_graf = ARCH_CIRC_GRAF;

    /*  Allocazione della memoria necessaria per il vettore bufdati  */

    n_elementi_bufdati = newcw->grafico.max_samples;
    bufdati = (S_SEL_DATI *)calloc (n_elementi_bufdati,sizeof(S_SEL_DATI));

    /* inizializza bufdati - Lorenzo 23 -11 -94 */
    for (i=0;i<n_elementi_bufdati;i++)
      bufdati[i].t = -1;
    /* fine Lorenzo  */
#if defined SCADA_MMI
      CreateARCH_GRAF (tnew,0,NULL,True);
      newcw->grafico.primo_refresh = True;
#else
    /*  Legge le risorse dell'oggetto per impostarlo leggendo da file f22  */
    if (strlen (newcw->grafico.nome_file_f22) != 0)
      {
      /* Trovato f22 configurato  */
      argv[argc]=newcw->grafico.nome_file_f22;
      argc++;

      /*  Ricavo i nomi delle variabili definite  */
      if(strlen (newcw->grafico.nome_var[0]) != 0) 
        {
	/*  Leggo la prima variabile definita  */
        Lette = sscanf (newcw->grafico.nome_var[0],"%s",ParVar1);
	if (Lette == 1)
	  {
	  argv[argc]=ParVar1;
	  argc++;
	  }
	}
      if(strlen (newcw->grafico.nome_var[1]) != 0) 
        {
	/*  Leggo la seconda variabile definita  */
        Lette = sscanf (newcw->grafico.nome_var[1],"%s",ParVar2);
	if (Lette == 1)
	  {
	  argv[argc]=ParVar2;
	  argc++;
	  }
	}
      if(strlen (newcw->grafico.nome_var[2]) != 0) 
        {
	/*  Leggo la terza variabile definita  */
        Lette = sscanf (newcw->grafico.nome_var[2],"%s",ParVar3);
	if (Lette == 1)
	  {
	  argv[argc]=ParVar3;
	  argc++;
	  }
	}
      if(strlen (newcw->grafico.nome_var[3]) != 0) 
        {
	/*  Leggo la quarta variabile definita  */
        Lette = sscanf (newcw->grafico.nome_var[3],"%s",ParVar4);
	if (Lette == 1)
	  {
	  argv[argc]=ParVar4;
	  argc++;
	  }
	}
      CreateARCH_GRAF (tnew,argc,argv,True);
      }
    else
      {
      /*  Impossibile partire con situazione di default  */
      CreateARCH_GRAF (tnew,0,NULL,True);
      }
#endif
    break;

    /*****  Grafics di 4 misure lette da database  *****/
    case RUN_GRAF:
    tipo_graf = RUN_GRAF;

    /*  Allocazione della memoria necessaria per il vettore bufdati  */
    f_refresh = XlGetRefreshFreq (newcw);
    n_elementi_bufdati = (float)((float)newcw->grafico.delta_tempo/(float)f_refresh)*10.0+1;
    bufdati = (S_SEL_DATI *)calloc (n_elementi_bufdati,sizeof(S_SEL_DATI));

    nofile = 0;
    for (i=0;i<n_elementi_bufdati;i++)
      bufdati[i].t = -1;
    n_last = 0;

    CreateRUN_GRAF (tnew,0,NULL,True);

    /*  Inizializza il vettore points  */
    points[0][0].x = 0;
    points[1][0].x = 0;
    points[2][0].x = 0;
    points[3][0].x = 0;
    points[0][0].y = 0;
    points[1][0].y = 0;
    points[2][0].y = 0;
    points[3][0].y = 0;
    
    /*  Inizializza i tempi per lo zoom  */
    tz_iniziale = 0;
    tz_finale = 0;

    break;

    /*****  Grafics di 1 misura letta da database  *****/
    case PLOT_GRAF:
    tipo_graf = PLOT_GRAF;

    /*  Allocazione della memoria necessaria per il vettore bufdati  */
    f_refresh = XlGetRefreshFreq (newcw);
    n_elementi_bufdati = (float)((float)newcw->grafico.delta_tempo/(float)f_refresh)*10.0+1;
    bufdati = (S_SEL_DATI *)calloc (n_elementi_bufdati,sizeof(S_SEL_DATI));

    nofile = 0;
    for (i=0;i<n_elementi_bufdati;i++)
      bufdati[i].t = -1;
    n_last = 0;

    CreatePLOT_GRAF (tnew,0,NULL,True);

    /*  Inizializza il vettore points  */
    points[0][0].x = 0;
    points[0][0].y = 0;

    /*  Inizializza i tempi per lo zoom  */
    tz_iniziale = 0;
    tz_finale = 0;

    break;
    }
  }
SettaActionsFigli(tnew);
}

#define superclass (&xlManagerClassRec)

static void Seleziona(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/*
 Richiama la routine di select di XlGrafico per la gestione della
 selezione
*/
if((strcmp(XtName(w),"draw0")==0)||(strcmp(XtName(w),"draw1")==0) ||
	(strcmp(XtName(w),"ord1")==0)||(strcmp(XtName(w),"mis1")==0) ||
	(strcmp(XtName(w),"tim1")==0) || (strcmp(XtName(w),"val1")==0) ||
	(strcmp(XtName(w),"tempo")==0) )
	{
	if(XlIsXlManager(XtParent(XtParent(w))))
          (*superclass->xlmanager_class.select) (XtParent(XtParent(w)),0);
	else 
          printf("\n NON E' UN MANAGER 1\n");
	}
else if( strcmp(XtName(w),"contenitore")==0) 
	{
	if(XlIsXlManager(XtParent(w)))
        	(*superclass->xlmanager_class.select) (XtParent(w),0);
	else 
                printf("\n NON E' UN MANAGER 2\n");
        }
else
	{
        if(XlIsXlManager(w))
                (*superclass->xlmanager_class.select) (w,0);
        else
                printf("\n NON E' UN MANAGER 3 e sono %s \n",XtName(w));

	}

        
}

static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/* richiama il metodo di select */
if((strcmp(XtName(w),"draw0")==0)||(strcmp(XtName(w),"draw1")==0) ||
	(strcmp(XtName(w),"ord1")==0)||(strcmp(XtName(w),"mis1")==0) ||
	(strcmp(XtName(w),"tim1")==0) || (strcmp(XtName(w),"val1")==0) ||
	(strcmp(XtName(w),"tempo")==0) ||
	(strcmp(XtName(XtParent(w)),"contenitore")==0) )
        (*superclass->xlmanager_class.select) (XtParent(XtParent(w)),1);
else
        (*superclass->xlmanager_class.select) (XtParent(w),1);
}

static void PopupInterno(w,event,param,nparam)
Widget w;
XEvent *event;
String *param;
Cardinal *nparam;
{
Widget w_popup;

printf("Richiamata routine: PopupInterno\n");
if( *nparam < 1)
      return;
printf("Nome del Widgetpopup=%s\n",param[0]);
sscanf(param[0],"%ld",&w_popup);
printf("Valore di w_popup=%d\n",w_popup);
XmMenuPosition( w_popup, event );
XtManageChild( w_popup );
}

static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
Arg comp_args[2];
XlGraficoWidget curcw= (XlGraficoWidget) current;
XlGraficoWidget newcw= (XlGraficoWidget) new;
char *app_char;
Boolean do_redisplay = False;


if(curcw->grafico.nome_file_f22!=newcw->grafico.nome_file_f22)
	{
	app_char=newcw->grafico.nome_file_f22;
	newcw->grafico.nome_file_f22=XtNewString(app_char);
	printf("Settato nuovo path <%s>\n",newcw->grafico.nome_file_f22);
        do_redisplay = True;
        }
  
/*********
if(curcw->grafico.norm_fg!=newcw->grafico.norm_fg)
	{
	XtReleaseGC(curcw,curcw->grafico.clear_gc);
	GetClearGC(newcw);
	XtSetArg(comp_args[0],XmNforeground,newcw->grafico.norm_fg);
	XtSetValues(newcw,comp_args,1);
        do_redisplay = True;
        }
	

if(curcw->xlmanager.config!=newcw->xlmanager.config)
        {
        if(newcw->xlmanager.config==True)
                SettaActionsFigli(newcw);
        else
                {
                CreaFigli(newcw);
		handle_motion(widget_array[k_draw1]);
                }
        do_redisplay = True;
        }



if(strcmp(curcw->grafico.normalfont,newcw->grafico.normalfont))
        {
        if(!LoadFont(newcw,newcw->grafico.normalfont))
        if(!LoadFont(newcw,"fixed"))
                XlError("XlGrafico","SetValues",
                        "Impossibile caricare default font");
        do_redisplay = True;
        }
**************/
if(curcw->xlmanager.config!=newcw->xlmanager.config)
        {
        if(newcw->xlmanager.config==True)
                SettaActionsFigli(newcw);
        else
                {
/*****
                CreaFigli(newcw);
*****/
printf("\n PASSA DI QUI \n");
                }
        do_redisplay = False;
        }
if(curcw->xlmanager.width0!=newcw->xlmanager.width0 ||
	curcw->xlmanager.height0!=newcw->xlmanager.height0)
        {
	Arg args[3];
	Cardinal lnargs,numChild;
	Dimension wform,hform;
	WidgetList childList;
        lnargs=0;
        XtSetArg(args[lnargs],XmNnumChildren,&numChild);lnargs++;
        XtSetArg(args[lnargs],XmNchildren,&childList);lnargs++;
        XtGetValues(newcw,args,lnargs);

	lnargs=0;
	wform=newcw->xlmanager.width0-10;
	hform= newcw->xlmanager.height0-10;
        XtSetArg(args[lnargs],XmNwidth,wform);lnargs++;
        XtSetArg(args[lnargs],XmNheight,hform);lnargs++;
	XtSetValues(childList[0],args,lnargs);
	do_redisplay = True;

	}

/*
        risorsa width
*/
if(curcw->core.width != newcw->core.width)
        {
        if(newcw->core.width<MIN_WIDTH)
                newcw->core.width=MIN_WIDTH;
        newcw->xlmanager.width0=newcw->core.width;
        do_redisplay = True;
        }

/*
        risorsa height
*/
if(curcw->core.height != newcw->core.height)
        {
        if(newcw->core.height<MIN_HEIGHT)
                newcw->core.height=MIN_HEIGHT;
        newcw->xlmanager.height0=newcw->core.height;
        do_redisplay = True;
        }

/*
        risorsa width0
*/
if(curcw->xlmanager.width0 != newcw->xlmanager.width0)
        {
        if(newcw->xlmanager.width0<MIN_WIDTH)
                newcw->xlmanager.width0=MIN_WIDTH;
        newcw->core.width=newcw->xlmanager.width0;
        do_redisplay = True;
        }

/*
        risorsa height0
*/
if(curcw->xlmanager.height0 != newcw->xlmanager.height0)
        {
        if(newcw->xlmanager.height0<MIN_HEIGHT)
                newcw->xlmanager.height0=MIN_HEIGHT;
        newcw->core.height=newcw->xlmanager.height0;
        do_redisplay = True;
        }

return do_redisplay;
}

static void Destroy(w)
Widget w;
{
int i;
S_XLGRAFICO *pXlGraf;
XlGraficoWidget cw= (XlGraficoWidget) w;
#if defined SCADA_MMI
char comando[200];
#endif

pXlGraf = cw->grafico.p;

/*  Rilascio i GC  */
for (i=0;i<4;i++)
  {
  if (gc[i]) XtReleaseGC (cw,gc[i]);
  if (gc2[i]) XtReleaseGC (cw,gc2[i]);
  }
if (gc_grid1) XtReleaseGC (cw,gc_grid1);
if (gc_grid2) XtReleaseGC (cw,gc_grid2);
if (gc_zoom) XtReleaseGC (cw,gc_zoom);

if (!cw->xlmanager.config)
  {
  /*  Libero il vettore di punti per disegnare il grafico  
	che viene allocato solo in RunTime  */
  XtFree (points[0]);
  if (cw->grafico.tipo != PLOT_GRAF)
    {
    XtFree (points[1]);
    XtFree (points[2]);
    XtFree (points[3]);
    }
  /*  Libero il vettore bufdati  */
  XtFree (bufdati);
#if defined SCADA_MMI
  /* cancella il file contenete i trend */
  if (((cw->grafico.tipo == ARCH_GRAF)||(cw->grafico.tipo == ARCH_CIRC_GRAF))
		&&(!nofile))
	XlRemove(cw->grafico.nome_file_f22);
#endif
  }
}

static Boolean Refresh(Widget w)
{
char *argv[5];
int i,argc,Lette;
S_XLGRAFICO *pXlGraf;
XlGraficoWidget cw= (XlGraficoWidget) w;
char ParVar1[MAX_LUN_NOME_VAR],ParVar2[MAX_LUN_NOME_VAR];
char ParVar3[MAX_LUN_NOME_VAR],ParVar4[MAX_LUN_NOME_VAR];
float valore1 = cw->grafico.input[0].valore;
float valore2 = cw->grafico.input[1].valore;
float valore3 = cw->grafico.input[2].valore;
float valore4 = cw->grafico.input[3].valore;
float t_vecchio,Controllo;
S_DATI buf;
int ind_buf;
Boolean Scalato;
#if defined SCADA_MMI
char *nomi[4];
int num_mis;
#endif

pXlGraf= cw->grafico.p;
/*
 Nel caso il grafico sia rappresentato in run time (MMI) e sia il primo refresh
 viene appesa la handle motion per la gestione di zoom e collimazione
*/
if((cw->grafico.primo_refresh) && (cw->xlmanager.config == False))
	{
	printf("\n setta handle motion \n");
	handle_motion(widget_array[k_draw1]);
#ifndef SCADA_MMI
	if((cw->grafico.tipo==ARCH_GRAF)||(cw->grafico.tipo==ARCH_CIRC_GRAF))
		{
		cw->grafico.primo_refresh=False;
		return(True);
		}
#endif
	}

if ( (cw->xlmanager.config) && (cw->grafico.primo_refresh) )
  {
  strcpy (file_vis,cw->grafico.titolo_graf);
  XClearArea(XtDisplay(widget_array[k_draw0]),
	XtWindow(widget_array[k_draw0]),0,0,0,0,True);

  cw->grafico.primo_refresh = False;
  }

if(cw->xlmanager.config==True)
        return(True);

if(!XtIsWidget(w))
        {
        XlWarning("XlGrafico","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }
if(xlGraficoClassRec.xlmanager_class.database==NULL)
        {
        XlWarning("XlGrafico","Refresh","database non definito");
        return(False);
        }
if(!(*superclass->xlmanager_class.refresh) (w))
        {
        XlWarning("XlGrafico","Refresh","errore refresh classe superiore");
        return(False);
        }

#if defined SCADA_MMI
if ((cw->grafico.primo_refresh)&&((cw->grafico.tipo==ARCH_GRAF)||
		(cw->grafico.tipo==ARCH_CIRC_GRAF)))
{
	printf("\n\n\nPRIMO REFRESH GRAFICO ARCH <%s>\n\n\n",
		cw->grafico.nome_file_f22);
	cw->grafico.primo_refresh=False;
	load_file(cw->grafico.nome_file_f22,pXlGraf);
        /*  Ricavo i nomi delle variabili definite  */
	num_mis=0;
        if(strlen (cw->grafico.nome_var[0]) != 0) 
          {
	  /*  Leggo la prima variabile definita  */
          Lette = sscanf (cw->grafico.nome_var[0],"%s",ParVar1);
	  if (Lette == 1)
	    {
	    nomi[0]=ParVar1;
	    num_mis++;
	    }
	  }
        if(strlen (cw->grafico.nome_var[1]) != 0) 
          {
	  /*  Leggo la seconda variabile definita  */
          Lette = sscanf (cw->grafico.nome_var[1],"%s",ParVar2);
	  if (Lette == 1)
	    {
	    nomi[1]=ParVar2;
	    num_mis++;
	    }
	  }
        if(strlen (cw->grafico.nome_var[2]) != 0) 
          {
	  /*  Leggo la terza variabile definita  */
          Lette = sscanf (cw->grafico.nome_var[2],"%s",ParVar3);
	  if (Lette == 1)
	    {
	    nomi[2]=ParVar3;
	    num_mis++;
	    }
	  }
        if(strlen (cw->grafico.nome_var[3]) != 0) 
          {
	  /*  Leggo la quarta variabile definita  */
          Lette = sscanf (cw->grafico.nome_var[3],"%s",ParVar4);
	  if (Lette == 1)
	    {
	    nomi[3]=ParVar4;
	    num_mis++;
	    }
	  }
	if(!nofile)
		load_variables(num_mis,nomi,cw);
	XClearArea(XtDisplay(cw),XtWindow(widget_array[k_draw1]),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(widget_array[k_draw0]),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(widget_array[k_mis1]),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(widget_array[k_val1]),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(widget_array[k_tim1]),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(widget_array[k_ord1]),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(widget_array[k_tempo]),0,0,0,0,True);
	XClearArea(XtDisplay(cw),
		XtWindow(XtParent(widget_array[k_tempo])),0,0,0,0,True);
/* Lorenzo 5-95 */
timer_proc(widget_array[k_draw1]);
close_22dat();
return(True);

}
else if ((!cw->grafico.primo_refresh)&&
	((cw->grafico.tipo==ARCH_GRAF)||(cw->grafico.tipo==ARCH_CIRC_GRAF)))
{
return(True);
}
/* fine mod Lorenzo 5-95 */
#endif


if (cw->grafico.primo_refresh)
  {
  /*  Controllo per settaggio autoscaling delle variabili  */
  for (i=0;i<4;i++)
    {
    if (cw->grafico.autoscaling[i] == 0)
      {
      sg.autoscaling[i] = 0;
      sg.fix_min_max[i].max = cw->grafico.max_val[i];
      sg.fix_min_max[i].min = cw->grafico.min_val[i];
      }
    else
      {
      sg.autoscaling[i] = 1;
      sg.fix_min_max[i].max = -1;
      sg.fix_min_max[i].min = -1;
      }
    }
  }

if ((cw->grafico.tipo==ARCH_GRAF)||(cw->grafico.tipo==ARCH_CIRC_GRAF))
  {
  if(freeza==0)
    timer_proc(widget_array[k_draw1]);
  }
else if (cw->grafico.tipo==RUN_GRAF)
  {
  if (cw->grafico.primo_refresh)
    {
    set_min_max(NULL,pXlGraf);
    argc = 0;
    /*  Ricavo i nomi delle variabili definite  */
    if(strlen (cw->grafico.nome_var[0]) != 0) 
      {
      /*  Leggo la prima variabile definita  */
      Lette = sscanf (cw->grafico.nome_var[0],"%s",ParVar1);
      if (Lette == 1)
        {
        argv[argc]=ParVar1;
        argc++;
        }
      }
    if(strlen (cw->grafico.nome_var[1]) != 0) 
      {
      Lette = sscanf (cw->grafico.nome_var[1],"%s",ParVar2);
      if (Lette == 1)
        {
        argv[argc]=ParVar2;
        argc++;
        }
      }
    if(strlen (cw->grafico.nome_var[2]) != 0) 
      {
      Lette = sscanf (cw->grafico.nome_var[2],"%s",ParVar3);
      if (Lette == 1)
        {
        argv[argc]=ParVar3;
        argc++;
        }
      }
    if(strlen (cw->grafico.nome_var[3]) != 0) 
      {
      Lette = sscanf (cw->grafico.nome_var[3],"%s",ParVar4);
      if (Lette == 1)
        {
        argv[argc]=ParVar4;
        argc++;
        }
      }

    R_load_variables (argc,argv,cw);
    nofile = 0;
    XClearArea (XtDisplay(sg.w_mis),XtWindow(sg.w_mis),0,0,0,0,True);

    /*  Inizializzo il tempo finale e il tempo iniziale con il tempo
        della simulazione  */
    t_iniziale = (olDatabasePuntiClassRec.oldatabasePunti_class.getTime)(xlGraficoClassRec.xlmanager_class.database);

    /*  Setto il valore del tempo finale  */
    t_finale = t_iniziale + cw->grafico.delta_tempo;

    prep_str_tim (t_iniziale,t_finale,sg.w_draw);
    XClearArea (XtDisplay(sg.w_tim),XtWindow(sg.w_tim),0,0,0,0,True);

    strcpy (file_vis,cw->grafico.titolo_graf);
    XClearArea(XtDisplay(widget_array[k_draw0]),
	XtWindow(widget_array[k_draw0]),0,0,0,0,True);

    XClearArea(XtDisplay(widget_array[k_val1]),
        XtWindow(widget_array[k_val1]),0,0,0,0,True);

    cw->grafico.primo_refresh = False;
    freeza = 0;
    }
  if (freeza == 0)
    RefreshRUN_GRAF(cw);
  else
    {
    /*  Aggiornamento bufdati in caso di stato zoom  */
    Scalato = UpdateBufdati (cw);
    }
  }
else if (cw->grafico.tipo==PLOT_GRAF)
  {
  if (cw->grafico.primo_refresh)
    {
    set_min_max(NULL,pXlGraf);
    argc = 0;
    /*  Ricavo i nomi delle variabili definite  */
    if(strlen (cw->grafico.nome_var[0]) != 0)
      {
      /*  Leggo la prima variabile definita  */
      Lette = sscanf (cw->grafico.nome_var[0],"%s",ParVar1);
      if (Lette == 1)
        {
        argv[argc]=ParVar1;
        argc++;
        }
      }

    /*  Pulisco l'area dei grafici e delle ordinate  */
    XClearArea (XtDisplay(sg.w_draw),XtWindow(sg.w_draw),0,0,0,0,True);
    XClearArea (XtDisplay(sg.w_ord),XtWindow(sg.w_ord),0,0,0,0,True);

    R_load_variables (argc,argv,cw);
    nofile = 0;
    XClearArea (XtDisplay(sg.w_mis),XtWindow(sg.w_mis),0,0,0,0,True);

    /*  Inizializzo il tempo finale e il tempo iniziale con il tempo
        della simulazione  */
    t_iniziale = (olDatabasePuntiClassRec.oldatabasePunti_class.getTime)(xlGraficoClassRec.xlmanager_class.database);

    /*  Setto il valore del tempo finale  */
    t_finale = t_iniziale + cw->grafico.delta_tempo;
    t_corrente = t_iniziale;

    prep_str_tim (t_iniziale,t_finale,sg.w_draw);
    XClearArea (XtDisplay(sg.w_tim),XtWindow(sg.w_tim),0,0,0,0,True);

    strcpy (file_vis,cw->grafico.titolo_graf);
    XClearArea(XtDisplay(widget_array[k_draw0]),
                XtWindow(widget_array[k_draw0]),0,0,0,0,True);

    cw->grafico.primo_refresh = False;
    freeza = 0;
    }

  if (freeza == 0)
    RefreshRUN_GRAF(cw);
  else
    {
    /*  Aggiornamento bufdati in caso di stato zoom  */
    Scalato = UpdateBufdati(cw);
    }
  }
return(True);
}

static Boolean Compile(Widget w)
{
int i;
Boolean errors=False;


XlGraficoWidget cw= (XlGraficoWidget)w;

if(!XtIsWidget(w))
        {
        XlWarning("XlManager","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlGraficoClassRec.xlmanager_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
        return(False);
        }
if(!(*superclass->xlmanager_class.compile) (w,
     xlGraficoClassRec.xlmanager_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
        errors=True;
        XlEvError(cw,True);
        return(False);
        }
return(True);
}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlGraficoWidget cw= (XlGraficoWidget)w;
S_XLGRAFICO *pXlGraf;
register int x,y;
unsigned int width,height;
XmDrawingAreaCallbackStruct str;

pXlGraf = cw->grafico.p;

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
XClearArea(XtDisplay(cw),XtWindow(cw),0,0,0,0,False);

/*
	clear area oggetti MOTIF inclusi
*/

XClearArea(XtDisplay(cw),XtWindow(widget_array[k_draw1]),0,0,0,0,True);
XClearArea(XtDisplay(cw),XtWindow(widget_array[k_draw0]),0,0,0,0,True);
XClearArea(XtDisplay(cw),XtWindow(widget_array[k_mis1]),0,0,0,0,True);
XClearArea(XtDisplay(cw),XtWindow(widget_array[k_val1]),0,0,0,0,True);
XClearArea(XtDisplay(cw),XtWindow(widget_array[k_tim1]),0,0,0,0,True);
XClearArea(XtDisplay(cw),XtWindow(widget_array[k_ord1]),0,0,0,0,True);
XClearArea(XtDisplay(cw),XtWindow(widget_array[k_tempo]),0,0,0,0,True);
XClearArea(XtDisplay(cw),
	XtWindow(XtParent(widget_array[k_tempo])),0,0,0,0,True);
/*
        Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
*/
(*superclass->xlmanager_class.drawBorderIfSelected) (w);
}

static void add_translation(Widget wid,char *stringa)
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

static void SettaActionsFigli(tnew)
Widget tnew;
{
S_XLGRAFICO *pXlGraf;
Arg args[3];
Cardinal lnargs;
Cardinal numChild;
WidgetList childList;
char stringa[200];
Widget menu_popup;

XlGraficoWidget new = (XlGraficoWidget)tnew;

pXlGraf = new->grafico.p;

if(new->xlmanager.config==True)
  {
  add_translation(widget_array[k_draw1],configTranslations);
  add_translation(widget_array[k_draw0],configTranslations);
  add_translation(widget_array[k_mis1],configTranslations);
  add_translation(widget_array[k_val1],configTranslations);
  add_translation(widget_array[k_tim1],configTranslations);
  add_translation(widget_array[k_ord1],configTranslations);
  add_translation(widget_array[k_tempo],configTranslations);
  add_translation(XtParent(widget_array[k_tempo]),configTranslations);
  }
else
  {
  /*
	ricava l'indice di widget del menu popup che e' figlio
	della drawing area
  */
  sprintf(stringa,POPUP_MENU_TRANS,widget_array[k_popup]);
printf("SettaActionsFigli:valore param[0]=%s\n",stringa);
  add_translation(widget_array[k_draw1],stringa);
  }
}

static Boolean LoadFont( w,fontname,modoPrint)
Widget w;
char fontname[];
Boolean modoPrint;
{
S_XLGRAFICO *pXlGraf;
XlGraficoWidget cw = (XlGraficoWidget) w;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

char **pf;
int i,nf;
Display *disp;

pXlGraf= cw->grafico.p;
if(modoPrint)
	{
        disp=XlDisplay(w);
/* Carica il font ottenendo la descrizione del font stesso */
	if((font_info_gr = XLoadQueryFont(disp,fontname)) == NULL)
        	{
        	XlWarning("XlPrinter","LoadFont","Cannot open font");
        	return(False);
        	}
	}
else
	{
        disp=XtDisplay(w);
	if((font_info_gr = XlGetFont(disp,fontname)) == NULL)
        	{
        	XlWarning("XlPrinter","LoadFont","Cannot open font");
        	return(False);
        	}
	}
return(True);
}
  

static void GetAllGCs(w,modo)
Widget w;
Boolean modo;
{
GetTrendGCs(w,modo);
}

static void GetTrendGCs(w,modo)
Widget w;
Boolean modo;
{
S_XLGRAFICO *pXlGraf;
XGCValues values;
int i;
char *printerFont;
static int line_style2[] = {LineSolid,LineOnOffDash,
                            LineOnOffDash,LineSolid};
static int dash_offset[] = {0,0,0,5};
static char dash_list[5][2] ={ {0,0},{1,1},{2,2},{0,0},{10,10}};
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFunction;

XlGraficoWidget cw= (XlGraficoWidget) w;
pXlGraf= cw->grafico.p;

#if defined XPRINTER_USED
if(modo) /* se stampante */
	{
	printerFont=XtNewString(XlPrinterGetFont(w,
                "fixed",font_info_gr));
	/* libero il font  per ridisegno */
/*
	if(font_info_gr)
       		 XFreeFont(XtDisplay(w),font_info_gr);
*/
        /* carico il font per la stampante */
	if(!LoadFont(w,printerFont,True))
        	LoadFont(w,"fixed",True);
	XtFree(printerFont);
	}
else
	{
	/* libero il font della stampante */
	if(font_info_gr)
        	XFreeFont(XlDisplay(w),font_info_gr);
	if(!LoadFont(cw,"fixed",False))
        	LoadFont(cw,"fixed",False);
	}
#else
LoadFont(cw,"fixed",False);
#endif
font_height = font_info_gr->ascent + font_info_gr->descent;
font_width  = font_info_gr->max_bounds.width;

/*  Setto i GC per ogni trend grafico  */
values.background = cw->core.background_pixel;
values.line_width = 0;
values.line_style = LineSolid;
values.function=GXcopy;
#if defined XPRINTER_USED
if(modo)
	{
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
	for (i=0;i<4;i++)
  		{
  		values.foreground = XlPrintGetPixel(w,cw->grafico.col_trend[i]);
  		gc[i] = XCreateGC(XlDisplay(w),XlWindow(w),valuemask,&values);
  		XSetFont(XlDisplay(w),gc[i],font_info_gr->fid);
  		}
/*  GC per Hard Copy  */
	values.foreground = BlackPixel(XlDisplay(w),screen_num);
	for (i=0;i<4;i++)
  		{
  		values.line_style = line_style2[i];
  		if(i==3)
    			values.line_width = 2;
  		gc2[i]=XCreateGC(XlDisplay(w),XlWindow(w),valuemask,&values);
  		if(line_style2[i]==LineOnOffDash)
    			{
    			XSetDashes(XlDisplay(w),gc2[i],dash_offset[i],
       		           dash_list[i],2);
    			}
  		}
	XSetFont(XlDisplay(w),gc2[0],font_info_gr->fid);
/*  GC per sfondo grid HC  */
	values.line_style = LineSolid;
	values.line_width = 0;
	values.foreground = WhitePixel(XlDisplay(w),screen_num);
	gc_grid2 = XCreateGC (XlDisplay(w),XlWindow(w),valuemask,&values);

/*  GC per colore grid  */
	values.foreground = XlPrintGetPixel(w,cw->grafico.col_grid);
	gc_grid1 = XCreateGC (XlDisplay(w),XlWindow(w),valuemask,&values);

/*  GC per disegno rettangolo di zoom  */
	values.function=GXcopy;
	gc_zoom=XCreateGC (XlDisplay(w),XlWindow(w),valuemask,&values);
	}
else
	{
	values.background = cw->core.background_pixel;
	for (i=0;i<4;i++)
  		{
  		values.foreground = cw->grafico.col_trend[i];
  		gc[i] = XtGetGC(cw,valuemask,&values);
  		XSetFont (XtDisplay(cw),gc[i],font_info_gr->fid);
  		}

/*  GC per Hard Copy  */
	values.foreground = BlackPixel(XtDisplay(cw),screen_num);
	for (i=0;i<4;i++)
  		{
  		values.line_style = line_style2[i];
  		if(i==3)
    			values.line_width = 2;
  		gc2[i]=XtGetGC (cw,valuemask,&values);
  		if(line_style2[i]==LineOnOffDash)
    			{
    			XSetDashes(XtDisplay(cw),gc2[i],dash_offset[i],
       		           dash_list[i],2);
    			}
  		}
	XSetFont(XtDisplay(cw),gc2[0],font_info_gr->fid);

/*  GC per sfondo grid HC  */
	values.line_style = LineSolid;
	values.line_width = 0;
	values.foreground = WhitePixel(XtDisplay(cw),screen_num);
	gc_grid2 = XtGetGC (cw,valuemask,&values);

/*  GC per colore grid  */
	values.foreground = cw->grafico.col_grid;
	gc_grid1 = XtGetGC (cw,valuemask,&values);

/*  GC per disegno rettangolo di zoom  */
	values.function=GXxor;
	gc_zoom=XtGetGC (cw,valuemask,&values);
	}
#else
for (i=0;i<4;i++)
  {
  values.foreground = cw->grafico.col_trend[i];
  gc[i] = XtGetGC(cw,valuemask,&values);
  XSetFont (XtDisplay(cw),gc[i],font_info_gr->fid);
  }

/*  GC per Hard Copy  */
values.foreground = BlackPixel(XtDisplay(cw),screen_num);
for (i=0;i<4;i++)
  {
  values.line_style = line_style2[i];
  if(i==3)
    values.line_width = 2;
  gc2[i]=XtGetGC (cw,valuemask,&values);
  if(line_style2[i]==LineOnOffDash)
    {
    XSetDashes(XtDisplay(cw),gc2[i],dash_offset[i],
                  dash_list[i],2);
    }
  }
XSetFont(XtDisplay(cw),gc2[0],font_info_gr->fid);

/*  GC per sfondo grid HC  */
values.line_style = LineSolid;
values.line_width = 0;
values.foreground = WhitePixel(XtDisplay(cw),screen_num);
gc_grid2 = XtGetGC (cw,valuemask,&values);

/*  GC per colore grid  */
values.foreground = cw->grafico.col_grid;
gc_grid1 = XtGetGC (cw,valuemask,&values);

/*  GC per disegno rettangolo di zoom  */
values.function=GXxor;
gc_zoom=XtGetGC (cw,valuemask,&values);
#endif
}

/*************************************/
/*  Routine di refresh dello widget  */
/*************************************/
static void RefreshRUN_GRAF (w)
Widget w;
{
int i,iumis,iumis_sel;
XPoint *pp_coll;  /* per collimazione */
S_XLGRAFICO *pXlGraf;
XlGraficoWidget cw= (XlGraficoWidget) w;
float t_vecchio;

pXlGraf= cw->grafico.p;

t_vecchio = t_corrente;
/*
printf("RefreshRUN_GRAF DEBUG: t_vecchio = %f\tt_corrente = %f\n",
       t_vecchio, t_corrente);
*/
if ( UpdateBufdati (cw) )
  {
/*
printf("RefreshRUN_GRAF DEBUG: t_iniziale = %f\tt_finale = %f\n",
       t_iniziale, t_finale);
*/
  prep_str_tim (t_iniziale,t_finale,sg.w_draw);
  XClearArea (XtDisplay(sg.w_tim),XtWindow(sg.w_tim),0,0,0,0,True);
  XClearArea(XtDisplay (sg.w_draw),XtWindow(sg.w_draw),0,0,0,0,True);
  }

t_ultimo = t_corrente;
if(t_old==t_corrente)
  {
  freeza=0;
  return;
  }
else t_old=t_corrente;

for(i=0;i<4;i++)
  {
  if(sg.autoscaling[i] && sg.ind_mis[i]!=-1)
    {
    if ( set_scala(i,pXlGraf) == 1 )
      {
      XClearArea (XtDisplay(sg.w_ord),XtWindow(sg.w_ord),0,0,0,0,True);
      XClearArea(XtDisplay (sg.w_draw),XtWindow(sg.w_draw),0,0,0,0,True);
      }
    else
      XClearArea (XtDisplay(sg.w_draw),XtWindow(sg.w_draw),0,0,1,1,True);
    }
  else XClearArea(XtDisplay (sg.w_draw),XtWindow(sg.w_draw),0,0,1,1,True);
  if(sg.ind_mis[i]!=-1)
	{
  	  sg.ultimo[i]=bufdati[n_last-1].mis[i];
	}
  }

if(collima)
  t_ultimo=x_collima*(t_finale-t_iniziale)/draw_width+t_iniziale;

/*
 aggiorna il valore corrente dei dati visualizzati (nel caso di collimazione
 calcola il valore aggiornato del punto in stato di collimaz.).
*/
for(i=0;i<4;i++)
  {
  if(sg.ind_mis[i]!=-1)
    {
    if(collima)
      {
      pp_coll=points[i];
      while(pp_coll->x<x_collima)
        pp_coll++;
      sg.ultimo[i]=sg.sel_min_max[i].max-
      pp_coll->y/sg.fatt_y[i];
      }
    iumis=sg.ind_umis[i];
    iumis_sel=sg.umis_sel[i];
    sg.ultimo[i]=sg.ultimo[i]*uni_mis[iumis].A[iumis_sel]+
                 uni_mis[iumis].B[iumis_sel];
    }
  }
/*
Modifica di Fabio 13/2/96 rimaneggiata Lorenzo 27/3/96
viene fatto comunque un primo refresh
altrimenti se tutti i valori sono e permangono
a 0 (o costante) la window non e' mai refreshata
*/
  if( n_last>1  &&
    ((bufdati[n_last-1].mis[0] != bufdati[n_last-2].mis[0]) ||
        (bufdati[n_last-1].mis[1] != bufdati[n_last-2].mis[1]) ||
        (bufdati[n_last-1].mis[2] != bufdati[n_last-2].mis[2]) ||
        (bufdati[n_last-1].mis[3] != bufdati[n_last-2].mis[3]) ))
	{
  	XClearArea(XtDisplay (sg.w_val),XtWindow(sg.w_val),0,0,0,0,True);
	}

/*  Controllo il tempo corrente con il tempo precedente per sapere se 
	eseguire la clear area del tempo corrente  */
if ( t_corrente != t_vecchio )
  XClearArea(XtDisplay (widget_array[k_tempo]),XtWindow(widget_array[k_tempo]),0,0,0,0,True);
 
freeza=0;
}

/*  Shifta il vettore bufdati a sinistra di un quarto della sua lunghezza
	e azzera i campioni liberatesi alla sua destra  */
static int ScalaBufDati (Buf,Ind,Wid)
S_SEL_DATI **Buf;
int Ind;
Widget Wid;
{
XlGraficoWidget Wdg = (XlGraficoWidget)Wid;
int i,k,kk;
float fattore_delta;
int t_intero,t_appoggio;
S_SEL_DATI *Appo;

pXlGraf = Wdg->grafico.p;
/*  Calcolo i coefficienti per lo shift opportuno dei tempi  */
fattore_delta = (float)Wdg->grafico.delta_tempo/6.;
t_intero = (int)( (rint(t_corrente/fattore_delta)) + 1);
/*  Ricalcolo il nuovo tempo finale e il nuovo tempo iniziale  */
t_appoggio = (int)(t_intero*fattore_delta);
t_finale = t_appoggio;
t_iniziale = t_finale - Wdg->grafico.delta_tempo;
Appo = *Buf;

/*  Controllo quanti campioni devo buttare  */
i=0;
while (Appo[i].t < t_iniziale)
  i++;

/*  Ricopro i primi campioni da buttare  */
kk=0;
for (k=i;k<Ind;k++)
  {
  Appo[kk].t = Appo[k].t;
  Appo[kk].mis[0] = Appo[k].mis[0];
  Appo[kk].mis[1] = Appo[k].mis[1];
  Appo[kk].mis[2] = Appo[k].mis[2];
  Appo[kk].mis[3] = Appo[k].mis[3];
  kk++;
  }

for (i=kk;i<Ind;i++)
  {
  Appo[i].t = -1;
  Appo[i].mis[0] = 0.;
  Appo[i].mis[1] = 0.;
  Appo[i].mis[2] = 0.;
  Appo[i].mis[3] = 0.;
  }
return (kk-1);
}

/*
   Si ridefiniscono i limiti dell'intervallo di visualizzazione (t_init e
   t_fin) e si inizializza il buffer contenente i valori da visualizzare
*/
static Boolean ResetBufdati(float *t_init, float *t_fin, float t_corr, int delta_t) 
{
int ind_buf = 0;

   *t_init = t_corr;
   *t_fin = *t_init + delta_t;
   while (bufdati[ind_buf].t != -1 && ind_buf< n_elementi_bufdati) {
      bufdati[ind_buf].t = -1;
      ind_buf++;
   }

return(0);

}


/*  Aggiorna bufdati ad ogni refresh della pagina  */
/*    Ritorna true se e stata eseguita la ScalaBufDati altrimenti se 
	e' stato eseguito un semplice aggiornamento ritorna False  */
static Boolean UpdateBufdati (Widget w)
{
XlGraficoWidget cw = (XlGraficoWidget)w;
/*  Legge i valori dopo il refresh della memoria  */
float valore1 = cw->grafico.input[0].valore;
float valore2 = cw->grafico.input[1].valore;
float valore3 = cw->grafico.input[2].valore;
float valore4 = cw->grafico.input[3].valore;
Boolean ValoreDiRitorno = False;
S_DATI buf;
int ind_buf;
float Controllo,pix_inc;
float t_prec;

pXlGraf= cw->grafico.p;

/*printf("\n valori letti grafici: %f %f %f %f",valore1,
           valore2, valore3, valore4);*/

t_prec = t_corrente;
t_corrente = (olDatabasePuntiClassRec.oldatabasePunti_class.getTime)(xlGraficoClassRec.xlmanager_class.database);
/*
printf("UpdateBufdati DEBUG: t_prec = %f\tt_corrente = %f\n",
       t_prec, t_corrente);
*/

/*
   Si controlla se il tempo al passo corrente e' inferiore al tempo al
   passo precedente. Se cio' accade significa che e' stato attivato il
   replay oppure che e' stato caricato un backtrack
*/ 
if(t_corrente < t_prec) {
/*
   E' stato attivato il replay oppure e' stato caricato un backtrack.
   Si ridefinisce l'intervallo di visualizzazione e si resetta il
   buffer dei valori da visualizzare
   
*/

printf("UpdateBufdati DEBUG: t_corrente < t_prec!!!\n");

   ResetBufdati(&t_iniziale, &t_finale, t_corrente, cw->grafico.delta_tempo);
   ind_buf = 0;
   ValoreDiRitorno = True;
   t_restore = t_prec;

printf("UpdateBufdati DEBUG: t_restore=%f t_corrente=%f t_prec=%f\n",
       t_restore);

}
else {
/*
printf("UpdateBufdati DEBUG: t_restore = %f\tt_corrente = %f\n",
       t_restore, t_corrente);
*/
   if(t_corrente == t_restore) {
/*
   Rientrati da un replay oppure ripristinato il tempo corrente dopo aver
   caricato un backtrack. Si ridefinisce l'intervallo di
   visualizzazione e si resetta il buffer dei valori da visualizzare
*/

printf("UpdateBufdati DEBUG: rientrati da backtrack o da replay!!!\n");

      ResetBufdati(&t_iniziale, &t_finale, t_corrente, cw->grafico.delta_tempo);
      ind_buf = 0;
      ValoreDiRitorno = True;
   }
   else {
/*
   Situazione normale: si inserisce nel buffer dei valori il valore
   corrente da visualizzare
*/
      ind_buf = 0;
      while (bufdati[ind_buf].t != -1 && ind_buf< n_elementi_bufdati)
         ind_buf++;
   } 
}
/*
printf("UpdateBufdati DEBUG: ind_buf = %d\n", ind_buf);
*/
pix_inc=((float)draw_width)/(t_finale-t_iniziale);

if (ind_buf == 0)
  Controllo = 0;
else Controllo = pix_inc*(rint((bufdati[ind_buf-1].t+1)*10)/10-rint(t_iniziale*10)/10);

/*
Modifica di Fabio 13/2/96
t_corrente viene calcolata prima
*/
/*
Il controllo viene fatto calcolando il tempo finale e verificando quando
il tempo corrente e' maggiore di quello finale.
Se cosi' fosse, i dati verrebbero scalati
*/
/*
if ( Controllo >= ((float)draw_width))
*/

if(t_corrente>t_finale)
  {
/*
printf("UpdateBufdati DEBUG prima di ScalaBufDati: ind_buf = %d t_finale = %f\n", ind_buf,
       t_finale);
*/
  ind_buf = ScalaBufDati (&bufdati,ind_buf,cw);
/*
printf("UpdateBufdati DEBUG dopo di ScalaBufDati: ind_buf = %d t_finale = %f\n", ind_buf,
       t_finale);
*/

  ValoreDiRitorno = True;
  }

if( ind_buf == 0 || bufdati[ind_buf-1].t!= t_corrente)
	{
	bufdati[ind_buf].t = t_corrente;
	if (sg.ind_mis[0] != -1) bufdati[ind_buf].mis[0] = valore1;
	if (sg.ind_mis[1] != -1) bufdati[ind_buf].mis[1] = valore2;
	if (sg.ind_mis[2] != -1) bufdati[ind_buf].mis[2] = valore3;
	if (sg.ind_mis[3] != -1) bufdati[ind_buf].mis[3] = valore4;
	ind_buf++;
	n_last=ind_buf;

/*  Setto i valori di minimo e massimo se necessario  */
	buf.t = bufdati[ind_buf-1].t;
	if (sg.ind_mis[0] != -1) buf.mis[0] = bufdati[ind_buf-1].mis[0];
	if (sg.ind_mis[1] != -1) buf.mis[1] = bufdati[ind_buf-1].mis[1];
	if (sg.ind_mis[2] != -1) buf.mis[2] = bufdati[ind_buf-1].mis[2];
	if (sg.ind_mis[3] != -1) buf.mis[3] = bufdati[ind_buf-1].mis[3];
	set_min_max (&buf,pXlGraf);
	if (sg.ind_mis[0] != -1) bufdati[ind_buf-1].mis[0] = buf.mis[0];
	if (sg.ind_mis[1] != -1) bufdati[ind_buf-1].mis[1] = buf.mis[1];
	if (sg.ind_mis[2] != -1) bufdati[ind_buf-1].mis[2] = buf.mis[2];
	if (sg.ind_mis[3] != -1) bufdati[ind_buf-1].mis[3] = buf.mis[3];
	}

return (ValoreDiRitorno);
}


/*
        metodo di stampa
*/
#if defined XPRINTER_USED
XmDrawingAreaCallbackStruct str_callb_stub;

static Boolean Print(Widget w)
{
int i;
char *printerFont;
XlGraficoWidget cw = (XlGraficoWidget)w;

pXlGraf= cw->grafico.p;
str_callb_stub.window=0;

/*  Rilascio i GC  */
for (i=0;i<4;i++)
  {
  if (gc[i]) XtReleaseGC (cw,gc[i]);
  if (gc2[i]) XtReleaseGC (cw,gc2[i]);
  }
if (gc_grid1) XtReleaseGC (cw,gc_grid1);
if (gc_grid2) XtReleaseGC (cw,gc_grid2);
if (gc_zoom) XtReleaseGC (cw,gc_zoom);

GetAllGCs(cw,True);

/*
 richiama la routine di stampa
*/
XlPrintBackgroundAndBorder(cw,0);
XlPrintBackgroundAndBorder(XtParent(widget_array[k_draw0]),100);
XlPrintBackgroundAndBorder(widget_array[k_draw0],100);
XlPrintBackgroundAndBorder(widget_array[k_draw1],100);
XlPrintBackgroundAndBorder(widget_array[k_mis1],100);
XlPrintBackgroundAndBorder(widget_array[k_val1],100);
XlPrintBackgroundAndBorder(widget_array[k_tim1],100);
XlPrintBackgroundAndBorder(widget_array[k_ord1],100);
XlPrintBackgroundAndBorder(widget_array[k_tempo],100);
draw_proc(widget_array[k_draw0],k_draw0,&str_callb_stub);
draw_proc(widget_array[k_draw1],k_draw1,&str_callb_stub);
draw_proc(widget_array[k_mis1],k_mis1,&str_callb_stub);
draw_proc(widget_array[k_val1],k_val1,&str_callb_stub);
draw_proc(widget_array[k_tim1],k_tim1,&str_callb_stub);
draw_proc(widget_array[k_ord1],k_ord1,&str_callb_stub);
draw_proc(widget_array[k_tempo],k_tempo,&str_callb_stub);

/* libero i gc della stampante */
for (i=0;i<4;i++)
  {
  if (gc[i]) XFreeGC (XlDisplay(cw),gc[i]);
  if (gc2[i]) XFreeGC (XlDisplay(cw),gc2[i]);
  }
if (gc_grid1) XFreeGC (XlDisplay(cw),gc_grid1);
if (gc_grid2) XFreeGC (XlDisplay(cw),gc_grid2);
if (gc_zoom) XFreeGC (XlDisplay(cw),gc_zoom);
printf("\n liberati GC stampante \n");
GetAllGCs(cw,False);

return(True);
/*
return(XlHardCopyWidget(w));
*/

}
#endif


Boolean XlSetVarGrafico(Widget w, char *Variab, int indice, int tipo, 
	int modello)
{
XlGraficoWidget Wdg = (XlGraficoWidget)w;
char NomeV[MAX_LUN_NOME_VAR],*Descrizione;
char P1[20],P2[20],P3[20],P4[20],DescrUM[MAX_LUN_UNIMIS];
float ParA,ParB;
char SSParA[20],SSParB[20];

int i,num;

/*  Setto il flag per la rilettura della variabile  */
Wdg->grafico.primo_refresh = True;

/*  Setto la nuova stringa nella risorsa dell'oggetto  */
/*XtFree (Wdg->grafico.nome_var[0]);*/
Wdg->grafico.nome_var[0] = XtNewString(Variab);

/*  Ricavo dalla stringa di descrizione della variabile i parametri
        relativi alla definizione della unita' di misura  */
num = sscanf (Variab,"%s %s %s %s %s %s %s",NomeV,P2,P3,P4,SSParA,SSParB,DescrUM
);
if (num==7)
  {
  ParA = atof (SSParA);
  ParB = atof (SSParB);
  Wdg->grafico.input[0].unimis.a = ParA;
  Wdg->grafico.input[0].unimis.b = ParB;
  strcpy (Wdg->grafico.input[0].unimis.descr,DescrUM);
  Descrizione = OlGetNameVar (Wdg,NomeV,0);
/*  XtFree (Wdg->grafico.descr_var[0]); */
  Wdg->grafico.descr_var[0] = XtNewString (Descrizione);
/*  XtFree (Descrizione); */
  }

Wdg->grafico.input[0].pdb.indice=indice;
Wdg->grafico.input[0].pdb.tipo=tipo;
Wdg->grafico.input[0].pdb.nmod=modello;
Wdg->grafico.input[0].pdb.origin=SIMULAID;

return(True);
}
