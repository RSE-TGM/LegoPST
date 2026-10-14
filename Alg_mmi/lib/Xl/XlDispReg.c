/**********************************************************************
*
*       C Source:               XlDispReg.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:36:53 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlDispReg.c-9.1.3 %  (%full_filespec: XlDispReg.c-9.1.3:csrc:1 %)";
#endif
/*
   modulo XlDispReg.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlDispReg.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlDispReg.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlDispReg.c - widget dispReg per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlDispRegP.h>
#include <Xl/XlDrawingUtil.h>
#include <Xl/XlPrint.h>
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
        XtOffsetOf(XlDispRegRec,dispReg.norm_fg),
        XmRString,
        XtDefaultForeground
        },
	{
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlDispRegRec,dispReg.normalfont),
        XmRImmediate,
        "fixed"
        },
	{
        XlNdispRegMode,
        XlCDispRegMode,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlDispRegRec,dispReg.dispReg_mode),
        XmRImmediate,
        (XtPointer)DISP_TAG_DEFAULT_MODE
        },
	{
        XlNdispRegBehavior,
        XlCDispRegBehavior,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlDispRegRec,dispReg.dispReg_behavior),
        XmRImmediate,
        (XtPointer)VALUE_BEHAVIOR
        },
	{
        XlNdispRegTagSave,
        XlCDispRegTagSave,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlDispRegRec,dispReg.tagSave),
        XmRImmediate,
        "XX"
        },
        {
        XlNnumeroInt,
        XlCNumeroInt,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlDispRegRec,dispReg.numero_int),
        XmRImmediate,
        (XtPointer)NUMERO_INTERI_VISUALIZZATI
        },
        {
        XlNnumeroDec,
        XlCNumeroDec,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlDispRegRec,dispReg.numero_dec),
        XmRImmediate,
        (XtPointer)NUMERO_DECIMALI_VISUALIZZATI
        },
        {
        XlNscalamento,
        XlCScalamento,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlDispRegRec,dispReg.scalamento),
        XtRString,
        VALORE_SCALAMENTO
        },
        {
        XlNvaloreMinimo,
        XlCValoreMinimo,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlDispRegRec,dispReg.valore_minimo),
        XtRString,
        VALORE_MINIMO
        },
        {
        XlNvaloreMassimo,
        XlCValoreMassimo,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlDispRegRec,dispReg.valore_massimo),
        XtRString,
        VALORE_MASSIMO
        },
        {
        XlNoffset,
        XlCOffset,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlDispRegRec,dispReg.offset),
        XtRString,
        VALORE_OFFSET
        },
        {
        XlNvarInputCambioColore1,
        XlCVarInputCambioColore1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].str_input),
        XmRImmediate,
        "XX"
        },
        {
        XlNcoloreAutomatico1,
        XlCColoreAutomatico1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].automatico),
        XmRString,
        XlDefaultColoreAutomatico1
        },
        {
        XlNcoloreStimato1,
        XlCColoreStimato1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].stimato),
        XmRString,
        XlDefaultColoreStimato1
        },
        {
        XlNcoloreFuoriScansione1,
        XlCColoreFuoriScansione1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].fuoriScansione),
        XmRString,
        XlDefaultColoreFuoriScansione1
        },
        {
        XlNcoloreFuoriAttendib1,
        XlCColoreFuoriAttendib1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].fuoriAttendib),
        XmRString,
        XlDefaultColoreFuoriAttendib1
        },
        {
        XlNcoloreDigSet1,
        XlCColoreDigSet1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].digitaleOn),
        XmRString,
        XlDefaultColoreDigSet1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo1,
        XlCValoreBassissimo1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].bassissimo_val),
        XmRString,
        XlDefaultValoreBassissimo1
        },
#endif
        {
        XlNcoloreBassissimo1,
        XlCColoreBassissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].bassissimo),
        XmRString,
        XlDefaultColoreBassissimo1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso1,
        XlCValoreBassissimoBasso1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].bassissimo_basso_val),
        XmRString,
        XlDefaultValoreBassissimoBasso1
        },
#endif
        {
        XlNcoloreBassissimoBasso1,
        XlCColoreBassissimoBasso1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].bassissimo_basso),
        XmRString,
        XlDefaultColoreBassissimoBasso1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto1,
        XlCValoreBassoAlto1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].basso_alto_val),
        XmRString,
        XlDefaultValoreBassoAlto1
        },
#endif
        {
        XlNcoloreBassoAlto1,
        XlCColoreBassoAlto1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].basso_alto),
        XmRString,
        XlDefaultColoreBassoAlto1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo1,
        XlCValoreAltoAltissimo1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].alto_altissimo_val),
        XmRString,
        XlDefaultValoreAltoAltissimo1
        },
#endif
        {
        XlNcoloreAltoAltissimo1,
        XlCColoreAltoAltissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].alto_altissimo),
        XmRString,
        XlDefaultColoreAltoAltissimo1
        },
        {
        XlNcoloreAltissimo1,
        XlCColoreAltissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].altissimo),
        XmRString,
        XlDefaultColoreAltissimo1
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
        XtOffsetOf(XlDispRegRec,dispReg.norm_fg),
        sizeof(Pixel),
        },
        {
        XlNnormalFont,
        XlCNormalFont,
        XlDnormalFont,
        XlRFont,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlDispRegRec,dispReg.normalfont),
        sizeof(XFontStruct*)
        },
        {
        XlNnumeroInt,
        XlCNumeroInt,
        XlDnumeroInt,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlDispRegRec,dispReg.numero_int),
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
        XtOffsetOf(XlDispRegRec,dispReg.numero_dec),
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
        XtOffsetOf(XlDispRegRec,dispReg.scalamento),
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
        XtOffsetOf(XlDispRegRec,dispReg.valore_minimo),
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
        XtOffsetOf(XlDispRegRec,dispReg.valore_massimo),
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
        XtOffsetOf(XlDispRegRec,dispReg.offset),
        sizeof(float)
        },
        {
        XlNdispRegBehavior,
        XlCDispRegBehavior,
        XlDdispRegBehavior,
        XlROption,
        NULL,
        XlOdispRegBehavior,
        XlRGrIo,
        XtOffsetOf(XlDispRegRec,dispReg.dispReg_behavior),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore1,
        XlCVarInputCambioColore1,
        XlDvarInputCambioColore1,
        XlRTipoVarReg+LOCKED_PAG,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNcoloreAutomatico1,
        XlCColoreAutomatico1,
        XlDcoloreAutomatico1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].automatico),
        sizeof(Pixel)
        },
        {
        XlNcoloreStimato1,
        XlCColoreStimato1,
        XlDcoloreStimato1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].stimato),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriScansione1,
        XlCColoreFuoriScansione1,
        XlDcoloreFuoriScansione1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].fuoriScansione),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriAttendib1,
        XlCColoreFuoriAttendib1,
        XlDcoloreFuoriAttendib1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].fuoriAttendib),
        sizeof(Pixel)
        },
        {
        XlNcoloreDigSet1,
        XlCColoreDigSet1,
        XlDcoloreDigSet1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].digitaleOn),
        sizeof(Pixel)
        },
        {
        XlNcoloreBassissimo1,
        XlCColoreBassissimo1,
        XlDcoloreBassissimo1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].bassissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo1,
        XlCValoreBassissimo1,
        XlDvaloreBassissimo1,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].bassissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassissimoBasso1,
        XlCColoreBassissimoBasso1,
        XlDcoloreBassissimoBasso1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].bassissimo_basso),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso1,
        XlCValoreBassissimoBasso1,
        XlDvaloreBassissimoBasso1,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].bassissimo_basso_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassoAlto1,
        XlCColoreBassoAlto1,
        XlDcoloreBassoAlto1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].basso_alto),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto1,
        XlCValoreBassoAlto1,
        XlDvaloreBassoAlto1,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].basso_alto_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltoAltissimo1,
        XlCColoreAltoAltissimo1,
        XlDcoloreAltoAltissimo1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].alto_altissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo1,
        XlCValoreAltoAltissimo1,
        XlDvaloreAltoAltissimo1,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,                                          
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].alto_altissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltissimo1,
        XlCColoreAltissimo1,
        XlDcoloreAltissimo1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlDispRegRec,dispReg.cambio_colore[0].altissimo),
        sizeof(Pixel)
        }
	};

/* dichiarazioni funzioni varie */
static void GetNormFgGC();
static void GetClearGC();
static void GetAllGCs();
static Boolean LoadFont();
static void DrawIntoDispReg();
static void DrawDispReg();
         
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
static Boolean CheckDispReg();
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
XlDispRegClassRec xlDispRegClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "DispReg",
    /* widget_size              */      sizeof(XlDispRegRec),
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
    /* DrawBorserIfSelected     */      XtInheritDrawBorderIfSelected,
    /* selectRunTime            */      NULL,
#if defined XPRINTER_USED
    /* print                    */      Print,
#endif
  },
  { /* dispReg fields */
    /* checkDispReg                    */      CheckDispReg
  }
};

WidgetClass xlDispRegWidgetClass = (WidgetClass) &xlDispRegClassRec;


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlDispRegWidget new = (XlDispRegWidget)tnew;
int app_int;
int zoom;



if(!LoadFont(new,new->dispReg.normalfont,False))
	if(!LoadFont(new,"fixed",False))
		XlError("XlDispReg","Initialize","Impossibile caricare default font");

/*
	calcola la width  e la height e annulla le rotazioni
*/
if(new->dispReg.dispReg_mode == DISPREG_TAG)
   new->core.width=XTextWidth(new->dispReg.font_info,
	new->dispReg.cambio_colore[0].str_input,2);

if(new->dispReg.dispReg_mode == DISPREG_VALUE)
   new->core.width=XTextWidth(new->dispReg.font_info,
	new->dispReg.cambio_colore[0].str_input,
	new->dispReg.numero_int+new->dispReg.numero_dec+1);

new->core.height=new->dispReg.font_info->ascent+
		new->dispReg.font_info->descent;
new->xlcore.rotate=0;
new->xlcore.ass_rotate=0;

if(XlIsXlComposite(XtParent(new)))
        {
        zoom=XlGetZoom(XtParent(new));
        new->core.width = new->core.width * zoom/100;
        new->core.height = new->core.height * zoom/100;
        }

if(new->core.width<DEFAULTWIDTH)
	{
        new->core.width=DEFAULTWIDTH;
	XlWarning("XlDispReg","Initialize",
	     "width minore della dimensione minima");
	}
if(new->core.height<DEFAULTHEIGHT)
	{
        new->core.width=DEFAULTWIDTH;
	XlWarning("XlDispReg","Initialize",
		"height minore della dimensione minima");
	}

 memset(&(new->dispReg.OldFlag),sizeof(FLAG_MMI),0);
 new->dispReg.cambio_colore[0].input.valore = 0;
 new->dispReg.OldPixel = -1;

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
XlDispRegWidget curcw= (XlDispRegWidget) current;
XlDispRegWidget newcw= (XlDispRegWidget) new;
Boolean do_redisplay = False;
Arg comp_args[2];
int dispReg_w,dispReg_h;
char *app_char;



if(curcw->xlcore.rotate != newcw->xlcore.rotate)
	newcw->xlcore.rotate=curcw->xlcore.rotate;
if(curcw->xlcore.ass_rotate != newcw->xlcore.ass_rotate)
	newcw->xlcore.ass_rotate=curcw->xlcore.ass_rotate;

if(curcw->dispReg.dispReg_mode != newcw->dispReg.dispReg_mode)
	{
        if(curcw->dispReg.dispReg_mode == DISPREG_UNMANAGED)
		{
		XtManageChild(newcw);
		}
        if(newcw->dispReg.dispReg_mode == DISPREG_UNMANAGED)
		{
		XtUnmanageChild(newcw);
		}
         if((newcw->dispReg.dispReg_mode == DISPREG_TAG)||
	    (newcw->dispReg.dispReg_mode == DISPREG_VALUE) ||
	     (newcw->dispReg.dispReg_mode == DISPREG_MODUL))
                {
		/*
			calcola la width  e la height e annulla le rotazioni
		*/
		if((newcw->dispReg.dispReg_mode == DISPREG_TAG)||
			(newcw->dispReg.dispReg_mode == DISPREG_MODUL))
   		newcw->core.width=XTextWidth(newcw->dispReg.font_info,
			newcw->dispReg.cambio_colore[0].str_input,2);
		if(newcw->dispReg.dispReg_mode == DISPREG_VALUE)
   		newcw->core.width=XTextWidth(newcw->dispReg.font_info,
			newcw->dispReg.cambio_colore[0].str_input,
			newcw->dispReg.numero_int+newcw->dispReg.numero_dec+1);
		newcw->core.height=newcw->dispReg.font_info->ascent+
				newcw->dispReg.font_info->descent;
		}
	do_redisplay = True;
	}

if(curcw->dispReg.norm_fg != newcw->dispReg.norm_fg)
        {
        XtReleaseGC(curcw,curcw->dispReg.norm_gc);
        XtReleaseGC(curcw,curcw->dispReg.clear_gc);
        GetNormFgGC(newcw,False);
        GetClearGC(newcw,False);
        do_redisplay = True;
        }

if(strcmp(curcw->dispReg.normalfont,newcw->dispReg.normalfont))
        {
        if(!LoadFont(newcw,newcw->dispReg.normalfont,False))
        if(!LoadFont(newcw,"fixed",False))
                XlError("XlDispReg","SetValues",
                        "Impossibile caricare default font");
	dispReg_h=newcw->dispReg.font_info->ascent+
			newcw->dispReg.font_info->descent;
	dispReg_w=XTextWidth(newcw->dispReg.font_info,
		newcw->dispReg.cambio_colore[0].str_input,strlen(newcw->dispReg.cambio_colore[0].str_input));
	XtSetArg(comp_args[0],XmNwidth,dispReg_w);
	XtSetArg(comp_args[1],XmNwidth,dispReg_h);
	XtSetValues(newcw,comp_args,2);
        do_redisplay = True;
        }

if(curcw->dispReg.cambio_colore[0].str_input !=  newcw->dispReg.cambio_colore[0].str_input)
	{
/*
	XtFree(curcw->dispReg.cambio_colore[0].str_input);
*/
	app_char=newcw->dispReg.cambio_colore[0].str_input;
        newcw->dispReg.cambio_colore[0].str_input= XtNewString(app_char);
        do_redisplay = False;
	}
if(curcw->dispReg.tagSave !=  newcw->dispReg.tagSave)
	{
	app_char=newcw->dispReg.tagSave;
        newcw->dispReg.tagSave = XtNewString(app_char);
        do_redisplay = False;
	}

return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlDispRegWidget cw= (XlDispRegWidget) w;

/* rilascio i 2 GC */
if(cw->dispReg.norm_gc)
	XtReleaseGC(cw,cw->dispReg.norm_gc);

if(cw->dispReg.clear_gc)
	XtReleaseGC(cw,cw->dispReg.clear_gc);

XlFreeCambioColoreGC(cw,cw->dispReg.cambio_colore[0],False);

/* libero la font_struct */
/*
if(cw->dispReg.font_info)
	XFreeFont(XtDisplay(cw),cw->dispReg.font_info);
*/

}

static void GetNormFgGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
XlDispRegWidget cw= (XlDispRegWidget) w;

unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;


values.line_width = 1;
values.line_style = LineSolid;
#if defined XPRINTER_USED
if(modoPrint)
        {
        values.foreground = XlPrintGetPixel(w,cw->dispReg.norm_fg);
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->dispReg.norm_gc = XCreateGC(XlDisplay(w),(Drawable)XlWindow(w),
                                 valuemask, &values);
        }
else
        {
        values.foreground = cw->dispReg.norm_fg;
        values.background = cw->core.background_pixel;
        cw->dispReg.norm_gc = XtGetGC(cw, valuemask, &values);
        }
#else
values.foreground = cw->dispReg.norm_fg;
values.background = cw->core.background_pixel;
cw->dispReg.norm_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetClearGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlDispRegWidget cw= (XlDispRegWidget) w;
values.line_width = 0;
values.line_style = LineSolid;

#if defined XPRINTER_USED
if(modoPrint)
        {
        values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
        values.background = XlPrintGetPixel(w,cw->dispReg.norm_fg);
        cw->dispReg.clear_gc = XCreateGC(XlDisplay(w),(Drawable)XlWindow(w),
                                 valuemask, &values);
        }
else
        {
        values.foreground = cw->core.background_pixel;
        values.background = cw->dispReg.norm_fg;
        cw->dispReg.clear_gc = XtGetGC(cw, valuemask, &values);
        }
#else
values.foreground = cw->core.background_pixel;
values.background = cw->dispReg.norm_fg;
cw->dispReg.clear_gc = XtGetGC(cw, valuemask, &values);
#endif
}


static void GetAllGCs(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XlDispRegWidget cw= (XlDispRegWidget)w;

GetNormFgGC(w,modoPrint);
GetClearGC(w,modoPrint);
XlGetCambioColoreGC(cw,&(cw->dispReg.cambio_colore[0]),
                cw->dispReg.font_info->fid,NULL,modoPrint);
}


static void DrawIntoDispReg(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XlDispRegWidget cw= (XlDispRegWidget)w;
int width,height;
/* questo gruppo di variabili deve essere riportato nella routine DrawDispReg */
int dx=10;
int dy=10;
char testo[15];


if(cw->dispReg.dispReg_mode == DISPREG_UNMANAGED)
	{
	XLowerWindow(XtDisplay(cw),XtWindow(cw));
	XtUnmanageChild(cw);
	return;
	}

width=cw->core.width;
height=cw->core.height;

/*
	se necessario sbianca lo sfondo
*/
if((cw->xlcore.trasparent!=True) && !modoPrint)
   XlRiempiRettangolo(w,(Drawable)XlWindow(w),cw->dispReg.clear_gc,0,0,100,100);
	
DrawDispReg(w,modoPrint);
}




static Boolean Refresh(Widget w)
{
XlDispRegWidget cw= (XlDispRegWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;
int core_x=cw->core.x;
int core_y=cw->core.y;


if(!XtIsWidget(w))
	{
	XlWarning("XlDispReg","Refresh",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}

if(xlDispRegClassRec.xlcore_class.database==NULL)
        {
        XlWarning("XlDispReg","Refresh","database non definito");
        return(False);
        }

/***
if(cw->dispReg.dispReg_mode != DISPREG_VALUE)
	return(True);
**/

if(!(*superclass->xlcore_class.refresh) (w))
        {
        XlWarning("XlDispReg","Refresh","errore refresh classe superiore");
        return(False);
        }

DrawIntoDispReg(cw,False);

return(True);
}


static void Resize(w)
Widget w;
{
XlDispRegWidget cw= (XlDispRegWidget)w;

/*
DrawIntoDispReg(cw,False);
*/
}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlDispRegWidget cw= (XlDispRegWidget)w;
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


/*
        Richiamo il metodo di Redisplay di XlCore per la gestione della
        trasparenza e del contorno di selezione dell'oggetto
*/
(*superclass->core_class.expose) (w,event,NULL);

DrawIntoDispReg(w,False);

/*
        Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
*/
(*superclass->xlcore_class.drawBorderIfSelected) (w);

}


static Boolean LoadFont( cw,fontname,modoPrint)
Widget cw;
char fontname[];
Boolean modoPrint;
{
extern XFontStruct* XlGetFont();
XlDispRegWidget Xl_cw = (XlDispRegWidget) cw;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
/* Carica il font ottenendo la descrizione del font stesso */

char **pf;
int i,nf;
Display *disp;

if(modoPrint)
	{
        disp=XlDisplay(cw);
	if((Xl_cw->dispReg.font_info = XLoadQueryFont(disp,fontname)) == NULL)
        	{
		XlWarning("XlDispReg","LoadFont","Cannot open font");
		return(False);
       		}

	}
else
	{
        disp=XtDisplay(cw);
	if((Xl_cw->dispReg.font_info = XlGetFont(disp,fontname)) == NULL)
        	{
		XlWarning("XlDispReg","LoadFont","Cannot open font");
		return(False);
       		}
	}

return(True);
}



static Boolean Compile(Widget w)
{
XlDispRegWidget cw= (XlDispRegWidget)w;

if(!XtIsWidget(w))
	{
	XlWarning("XlDispReg","Compile",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}
if(xlDispRegClassRec.xlcore_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
        return(False);
        }

if(!(*superclass->xlcore_class.compile) (w,
     xlDispRegClassRec.xlcore_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
        return(False);
        }
return(True);
}

static void DrawDispReg(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XlDispRegWidget cw= (XlDispRegWidget)w;
char *c;
int nchar=0;
int i;
int step;
char app_t[10];
int font_height;
float valore = cw->dispReg.cambio_colore[0].input.valore;
float scalamento = cw->dispReg.scalamento;
float offset = cw->dispReg.offset;
float min = cw->dispReg.valore_minimo;
float max = cw->dispReg.valore_massimo;
char testo[40];
Arg args[1];
GC gc_set;


if(cw->xlcore.config==True)
	c=cw->dispReg.cambio_colore[0].str_input;
else
	c=cw->dispReg.tagSave;


if(cw->dispReg.dispReg_mode == DISPREG_MODUL)
    {
    XtSetArg(args[0],XlNtagName,&c);
    XtGetValues(XtParent(w),args,1);
    strcpy(testo,c);
    testo[2]=0;
    XlTestoSimple(cw,XlWindow(w),cw->dispReg.norm_gc,
              0,50,testo, cw->dispReg.font_info);
    }

if(cw->dispReg.dispReg_mode == DISPREG_TAG)
    {
    strncpy(testo,c,2);
    testo[2]=0;
    XlTestoSimple(cw,XlWindow(w),cw->dispReg.norm_gc,
              0,50,testo, cw->dispReg.font_info);
    }

if(cw->dispReg.dispReg_mode == DISPREG_VALUE)
   {
   if(cw->xlcore.config==True)
        valore=(max-min)*3/5+min;
   valore = valore*scalamento+offset;
   if(valore>max)
        valore=max;
   else
        if(valore<min)
                valore=min;
   if(min==max)
        valore=min;
   sprintf(testo,"%-*.*f", cw->dispReg.numero_int+cw->dispReg.numero_dec+1,
                 cw->dispReg.numero_dec, valore);
   /*
           se necessario sbianca lo sfondo
   */
   if((cw->xlcore.trasparent!=True) && !modoPrint)
      XlRiempiRettangolo(w,(Drawable)XlWindow(w),cw->dispReg.clear_gc,
                   1,1,100,100);
   gc_set = XlFlagToGC(cw->dispReg.cambio_colore[0]);
   if(cw->dispReg.dispReg_behavior==VALUE_BEHAVIOR)
   	XlTestoSimpleCentre(cw,XlWindow(w),gc_set,50,50,
                testo,cw->dispReg.font_info);
   else
	{
	if(cw->dispReg.dispReg_behavior==VERTICAL_BAR_BEHAVIOR)
		{
      		XlRiempiRettangolo(w,(Drawable)XlWindow(w),gc_set,
                   45,1,55,100);
		}
	else
		{
      		XlRiempiRettangolo(w,(Drawable)XlWindow(w),gc_set,
                   1,45,100,55);
		}
	}
   }
}


/*
 XlIsDispReg(w) funzione per sapere se l'oggetto w
 e' del tipo XlDispReg
*/
Boolean XlIsDispReg(Widget w)
{
CoreWidgetClass wclass;
wclass=XtClass(w);
if(strcmp(wclass->core_class.class_name,"DispReg")==0)
        return(True);
else
        return(False);
}

static Boolean CheckDispReg(Widget w)
{
XlDispRegWidget cw = (XlDispRegWidget) w;

/*
	verifico che sia stata assegnata la variabile associata
*/
if(cw->dispReg.cambio_colore[0].str_input == NULL)
        {
        XlEvError(cw,True);
        return(False);
        }
if(strlen(cw->dispReg.cambio_colore[0].str_input) != 2)
        {
        XlEvError(cw,True);
        return(False);
        }
if(strcmp(cw->dispReg.cambio_colore[0].str_input,"XX") == 0)
        {
        XlEvError(cw,True);
        return(False);
        }

return(True);
}

Boolean XlCheckDispReg(Widget w)
{
XlDispRegWidget cw = (XlDispRegWidget) w;

return((*xlDispRegClassRec.xldispReg_class.checkDispReg)(cw));
}


/*
        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
XlDispRegWidget cw= (XlDispRegWidget) w;
char *printerFont;


(*superclass->xlcore_class.print) (w);

printerFont=XtNewString(XlPrinterGetFont(w,
		cw->dispReg.normalfont,cw->dispReg.font_info));

/* rilascio il font */
/*
if(cw->dispReg.font_info)
        XFreeFont(XtDisplay(cw),cw->dispReg.font_info);
*/
/* rilascio i 2 GC */
if(cw->dispReg.norm_gc)
        XtReleaseGC(cw,cw->dispReg.norm_gc);
if(cw->dispReg.clear_gc)
        XtReleaseGC(cw,cw->dispReg.clear_gc);

XlFreeCambioColoreGC(cw,cw->dispReg.cambio_colore[0],False);

if(!LoadFont(cw,printerFont,True))
        LoadFont(cw,"fixed",True);
XtFree(printerFont);
GetAllGCs(cw,True);

DrawIntoDispReg(w,True);

/* rilascio il font della stampante */
if(cw->dispReg.font_info)
        XFreeFont(XlDisplay(w),cw->dispReg.font_info);
/* rilascio i 2 GC della stampante */
if(cw->dispReg.norm_gc)
        XFreeGC(XlDisplay(w),cw->dispReg.norm_gc);
if(cw->dispReg.clear_gc)
        XFreeGC(XlDisplay(w),cw->dispReg.clear_gc);


if(!LoadFont(cw,cw->dispReg.normalfont,False))
        LoadFont(cw,"fixed",False);
GetAllGCs(cw,False);

return(True);
}
#endif
