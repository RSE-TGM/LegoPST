/**********************************************************************
*
*       C Source:               XlIndic.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:24:12 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlIndic.c-3.1.4.2.3 %  (%full_filespec: XlIndic.c-3.1.4.2.3:csrc:1 %)";
#endif
/*
   modulo XlIndic.c 
   tipo 
   release 5.7
   data 1/30/96
   reserved @(#)XlIndic.c	5.7

*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlIndic.c	5.5\t1/16/96";
/*
        Fine sezione per SCCS
*/
/*
 *  XlIndic.c - widget indic per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlIndicP.h>
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
        XtOffsetOf(XlIndicRec,indic.norm_fg),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNagoFg,
        XlCAgoFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicRec,indic.ago_fg),
        XmRString,
        "red"
        },
        {
        XlNtipoInd,
        XlCTipoInd,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIndicRec,indic.tipo_ind),
        XmRImmediate,
        (XtPointer)INDICATORE_AGO
        },
        {
        XlNnumeroInt,
        XlCNumeroInt,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIndicRec,indic.numero_int),
        XmRImmediate,
        (XtPointer)NUMERO_INTERI_VISUALIZZATI
        },
        {
        XlNnumeroDec,
        XlCNumeroDec,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIndicRec,indic.numero_dec),
        XmRImmediate,
        (XtPointer)NUMERO_DECIMALI_VISUALIZZATI
        },
	{
        XlNscalamento,
        XlCScalamento,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicRec,indic.scalamento),
        XtRString,
        VALORE_SCALAMENTO
        },
	{
        XlNvaloreMinimo,
        XlCValoreMinimo,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicRec,indic.valore_minimo),
        XtRString,
        VALORE_MINIMO
        },
	{
        XlNvaloreMassimo,
        XlCValoreMassimo,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicRec,indic.valore_massimo),
        XtRString,
        VALORE_MASSIMO
        },
        {
        XlNextraRangeSup,
        XlCExtraRangeSup,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicRec,indic.extra_range_sup),
        XtRString,
        EXTRA_RANGE_SUP
        },
	{
        XlNoffset,
        XlCOffset,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicRec,indic.offset),
        XtRString,
        VALORE_OFFSET
        },
	{
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIndicRec,indic.normalfont),
        XmRImmediate,
        "fixed"
        },
        {
        XlNvarInputCambioColore1,
        XlCVarInputCambioColore1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNcoloreAutomatico1,
        XlCColoreAutomatico1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].automatico),
        XmRString,
        XlDefaultColoreAutomatico1
        },
        {
        XlNcoloreStimato1,
        XlCColoreStimato1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].stimato),
        XmRString,
        XlDefaultColoreStimato1
        },
        {
        XlNcoloreFuoriScansione1,
        XlCColoreFuoriScansione1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].fuoriScansione),
        XmRString,
        XlDefaultColoreFuoriScansione1
        },
        {
        XlNcoloreFuoriAttendib1,
        XlCColoreFuoriAttendib1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].fuoriAttendib),
        XmRString,
        XlDefaultColoreFuoriAttendib1
        },
        {
        XlNcoloreDigSet1,
        XlCColoreDigSet1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].digitaleOn),
        XmRString,
        XlDefaultColoreDigSet1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo1,
        XlCValoreBassissimo1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].bassissimo_val),
        XmRString,
        XlDefaultValoreBassissimo1
        },
#endif
        {
        XlNcoloreBassissimo1,
        XlCColoreBassissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].bassissimo),
        XmRString,
        XlDefaultColoreBassissimo1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso1,
        XlCValoreBassissimoBasso1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].bassissimo_basso_val),
        XmRString,
        XlDefaultValoreBassissimoBasso1
        },
#endif
        {
        XlNcoloreBassissimoBasso1,
        XlCColoreBassissimoBasso1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].bassissimo_basso),
        XmRString,
        XlDefaultColoreBassissimoBasso1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto1,
        XlCValoreBassoAlto1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].basso_alto_val),
        XmRString,
        XlDefaultValoreBassoAlto1
        },
#endif
        {
        XlNcoloreBassoAlto1,
        XlCColoreBassoAlto1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].basso_alto),
        XmRString,
        XlDefaultColoreBassoAlto1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo1,
        XlCValoreAltoAltissimo1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].alto_altissimo_val),
        XmRString,
        XlDefaultValoreAltoAltissimo1
        },
#endif
        {
        XlNcoloreAltoAltissimo1,
        XlCColoreAltoAltissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].alto_altissimo),
        XmRString,
        XlDefaultColoreAltoAltissimo1
        },
        {
        XlNcoloreAltissimo1,
        XlCColoreAltissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].altissimo),
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
        XtOffsetOf(XlIndicRec,indic.norm_fg),
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
        XtOffsetOf(XlIndicRec,indic.ago_fg),
        sizeof(Pixel),
        },
        {
        XlNtipoInd,
        XlCTipoInd,
        XlDtipoInd,
        XlROption,
        NULL,
        XlOtipoInd,
        XlRGrIo,
        XtOffsetOf(XlIndicRec,indic.tipo_ind),
        sizeof(int)
        },
        {
        XlNnumeroInt,
        XlCNumeroInt,
        XlDnumeroInt,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicRec,indic.numero_int),
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
        XtOffsetOf(XlIndicRec,indic.numero_dec),
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
        XtOffsetOf(XlIndicRec,indic.scalamento),
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
        XtOffsetOf(XlIndicRec,indic.valore_minimo),
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
        XtOffsetOf(XlIndicRec,indic.valore_massimo),
        sizeof(float)
        },
        {
        XlNextraRangeSup,
        XlCExtraRangeSup,
        XlDextraRangeSup,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicRec,indic.extra_range_sup),
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
        XtOffsetOf(XlIndicRec,indic.offset),
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
        XtOffsetOf(XlIndicRec,indic.normalfont),
        sizeof(XFontStruct*)
        },
        {
        XlNvarInputCambioColore1,
        XlCVarInputCambioColore1,
        XlDvarInputCambioColore1,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNcoloreAutomatico1,
        XlCColoreAutomatico1,
        XlDcoloreAutomatico1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].automatico),
        sizeof(Pixel)
        },
        {
        XlNcoloreStimato1,
        XlCColoreStimato1,
        XlDcoloreStimato1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].stimato),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriScansione1,
        XlCColoreFuoriScansione1,
        XlDcoloreFuoriScansione1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].fuoriScansione),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriAttendib1,
        XlCColoreFuoriAttendib1,
        XlDcoloreFuoriAttendib1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].fuoriAttendib),
        sizeof(Pixel)
        },
        {
        XlNcoloreDigSet1,
        XlCColoreDigSet1,
        XlDcoloreDigSet1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].digitaleOn),
        sizeof(Pixel)
        },
        {
        XlNcoloreBassissimo1,
        XlCColoreBassissimo1,
        XlDcoloreBassissimo1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].bassissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo1,
        XlCValoreBassissimo1,
        XlDvaloreBassissimo1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].bassissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassissimoBasso1,
        XlCColoreBassissimoBasso1,
        XlDcoloreBassissimoBasso1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].bassissimo_basso),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso1,
        XlCValoreBassissimoBasso1,
        XlDvaloreBassissimoBasso1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].bassissimo_basso_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassoAlto1,
        XlCColoreBassoAlto1,
        XlDcoloreBassoAlto1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].basso_alto),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto1,
        XlCValoreBassoAlto1,
        XlDvaloreBassoAlto1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].basso_alto_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltoAltissimo1,
        XlCColoreAltoAltissimo1,
        XlDcoloreAltoAltissimo1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].alto_altissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo1,
        XlCValoreAltoAltissimo1,
        XlDvaloreAltoAltissimo1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].alto_altissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltissimo1,
        XlCColoreAltissimo1,
        XlDcoloreAltissimo1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicRec,indic.cambio_colore[0].altissimo),
        sizeof(Pixel)
        }
	};
/* dichiarazioni funzioni varie */
static void BlinkProc(Widget, int, Boolean);
static int min();
static void GetNormFgGC();
static void GetClearGC();
static void GetAgoGC();
static void GetAllGCs();
static Boolean LoadFont();
static void DrawIntoDirectly();
static void DrawAgoValoreDirectly();
static void CalcIstog(Widget, Position *, Position *, Dimension *,
		Dimension *, float);
         
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
XlIndicClassRec xlIndicClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "Indic",
    /* widget_size              */      sizeof(XlIndicRec),
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
  { /* indic fields */
    /* empy                   */      0,
  }
};

WidgetClass xlIndicWidgetClass = (WidgetClass) &xlIndicClassRec;


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlIndicWidget new = (XlIndicWidget)tnew;
int app_int;
Arg arg[2];


if(new->core.width<DEFAULTWIDTH)
	{
        new->core.width=DEFAULTWIDTH;
	XlWarning("XlIndic","Initialize",
	     "width minore della dimensione minima");
	}
if(new->core.height<DEFAULTHEIGHT)
	{
        new->core.height=DEFAULTWIDTH;
	XlWarning("XlIndic","Initialize",
		"height minore della dimensione minima");
	}

if(new->indic.tipo_ind>_INDIC_MAX)
	{
	new->indic.tipo_ind=INDICATORE_DEFAULT;
	XlWarning("XlIndic","Initialize",
		"indicatore inesistente, tipo settato al valore di default");
	}


if((new->core.width!=new->core.height)&&
	(new->indic.tipo_ind==INDICATORE_AGO))
        {
        new->core.width=min(new->core.width,new->core.height);
        new->core.height=new->core.width;
	XlWarning("XlIndic","Initialize","width diverso da height");
        }
if((new->core.width!=new->core.height)&&
        ((new->indic.tipo_ind==INDICATORE_BARRA)||
        (new->indic.tipo_ind==INDICATORE_ISTOGRAMMA_CRES)||
        (new->indic.tipo_ind==INDICATORE_ISTOGRAMMA_DEC)))
        {
	if((new->xlcore.rotate==ROTATE_90)||
		(new->xlcore.rotate==ROTATE_270))
			{
			app_int = new->core.width;
			new->core.width = new->core.height;
			new->core.height = app_int;
			}
	}

/*
	l'indicatore di tipo DISPLAY non e' ruotabile
*/
if(new->indic.tipo_ind==INDICATORE_DISPLAY)
	{
	new->xlcore.rotate=0;
	new->xlcore.ass_rotate=0;
	}

if(((new->indic.tipo_ind==INDICATORE_ISTOGRAMMA_CRES)||
	(new->indic.tipo_ind==INDICATORE_ISTOGRAMMA_DEC))&& 
	new->xlcore.config != True)
	{
	/* l'indicatore istogramma ha come background il colore dell'ago */
	new->core.background_pixel=new->indic.ago_fg;
	/* salvo le posizioni x e y */
	new->indic.starting_x = new->core.x;
	new->indic.starting_y = new->core.y;
	}
   

if(new->xlcore.rotate>3)
	{
	XlWarning("XlIndic","Initialize","rotate > 3");
	new->xlcore.rotate=new->xlcore.rotate%4;
	}

if(new->xlcore.ass_rotate>1)
	{
	XlWarning("XlIndic","Initialize","assRotate > 1");
	new->xlcore.ass_rotate=new->xlcore.ass_rotate%2;
	}

if(new->indic.valore_massimo<=new->indic.valore_minimo)
	{
	XlWarning("XlIndic","SetVAlues",
	    "il valore massimo deve essere maggiore del minimo");
	new->indic.valore_massimo=atof(VALORE_MASSIMO);
	new->indic.valore_minimo=atof(VALORE_MINIMO);
	}
if(new->indic.extra_range_sup<0)
	{
	XlWarning("XlIndic","SetVAlues",
	    "il valore dell' extra-range deve essere positivo");
	new->indic.extra_range_sup=atof(EXTRA_RANGE_SUP);
	}
/*
 Il valore di extra range viene per ora utilizzato solo
 nel caso di  display a barra (negli altri casi il valore
 dell'extra range viene per il momento forzato a zero
*/
if(new->indic.tipo_ind!=INDICATORE_BARRA)
	new->indic.extra_range_sup=atof(EXTRA_RANGE_SUP);

 new->indic.old_posizione = -1;
 memset(&(new->indic.OldFlag),sizeof(FLAG_MMI),0);
 new->indic.cambio_colore[0].input.valore = 0;
 new->indic.OldPixel = -1;
 new->indic.primo_refresh = 1;

if(!LoadFont(new,new->indic.normalfont,False))
	if(!LoadFont(new,"fixed",False))
		XlError("XlIndic","Initialize","Impossibile caricare default font");
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
XlIndicWidget curcw= (XlIndicWidget) current;
XlIndicWidget newcw= (XlIndicWidget) new;
Boolean do_redisplay = False;
Arg core_args[2];

int app_int;

/*
	l'indicatore tipo INDICATORE_DISPLAY non e ruotabile
*/
if((curcw->xlcore.rotate != newcw->xlcore.rotate)&&
	(curcw->indic.tipo_ind == INDICATORE_DISPLAY))
	   newcw->xlcore.rotate=0;

if((curcw->xlcore.ass_rotate != newcw->xlcore.ass_rotate)&&
	(curcw->indic.tipo_ind == INDICATORE_DISPLAY))
	   newcw->xlcore.ass_rotate=0;

if(curcw->indic.norm_fg != newcw->indic.norm_fg)
        {
        XtReleaseGC(curcw,curcw->indic.norm_gc);
        XtReleaseGC(curcw,curcw->indic.clear_gc);
        GetNormFgGC(newcw);
        GetClearGC(newcw);
        DrawIntoDirectly(newcw,False);
        DrawAgoValoreDirectly(newcw,1,False);
        do_redisplay = True;
        }

if(curcw->indic.ago_fg != newcw->indic.ago_fg)
        {
        XtReleaseGC(curcw,curcw->indic.ago_gc);
        GetAgoGC(newcw);
        DrawIntoDirectly(newcw,False);
        DrawAgoValoreDirectly(newcw,1,False);
        do_redisplay = True;
        }

if(curcw->core.background_pixel != newcw->core.background_pixel)
        {
        XtReleaseGC(curcw,curcw->indic.norm_gc);
        XtReleaseGC(curcw,curcw->indic.clear_gc);
        XtReleaseGC(curcw,curcw->indic.ago_gc);
        GetAllGCs(newcw,False);
        DrawIntoDirectly(newcw,False);
        DrawAgoValoreDirectly(newcw,1,False);
        do_redisplay = True;
        }


if(curcw->indic.tipo_ind != newcw->indic.tipo_ind)
        {
        DrawIntoDirectly(newcw,False);
        DrawAgoValoreDirectly(newcw,1,False);
        do_redisplay = True;
        }

if(curcw->indic.scalamento!=newcw->indic.scalamento)
        {
        DrawIntoDirectly(newcw,False);
        DrawAgoValoreDirectly(newcw,1,False);
        do_redisplay = True;
        }
	
if(curcw->indic.valore_minimo!=newcw->indic.valore_minimo)
        {
	if(newcw->indic.valore_minimo>=newcw->indic.valore_massimo)
		{
		XlWarning("XlIndic","SetVAlues",
		    "il valore minimo deve essere minore del massimo");
		newcw->indic.valore_minimo=curcw->indic.valore_minimo;
		}
	else
		{
        	DrawIntoDirectly(newcw,False);
        	DrawAgoValoreDirectly(newcw,1,False);
        	do_redisplay = True;
		}
        }

if(curcw->indic.valore_massimo!=newcw->indic.valore_massimo)
        {
	if(newcw->indic.valore_massimo<=newcw->indic.valore_minimo)
		{
		XlWarning("XlIndic","SetVAlues",
		    "il valore massimo deve essere maggiore del minimo");
		newcw->indic.valore_massimo=curcw->indic.valore_massimo;
		}
	else
		{
        	DrawIntoDirectly(newcw,False);
        	DrawAgoValoreDirectly(newcw,1,False);
        	do_redisplay = True;
		}
        }

if(curcw->indic.extra_range_sup!=newcw->indic.extra_range_sup)
        {
	if(newcw->indic.extra_range_sup<newcw->indic.valore_massimo)
		{
		XlWarning("XlIndic","SetVAlues",
		    "il valore extra range sup. deve essere maggiore del massimo");
		newcw->indic.extra_range_sup=atof(EXTRA_RANGE_SUP);
		}
	else
		{
        	DrawIntoDirectly(newcw,False);
        	DrawAgoValoreDirectly(newcw,1,False);
        	do_redisplay = True;
		}
        }
if(curcw->indic.offset!=newcw->indic.offset)
        {
        DrawIntoDirectly(newcw,False);
        DrawAgoValoreDirectly(newcw,1,False);
        do_redisplay = True;
        }

if(strcmp(curcw->indic.normalfont,newcw->indic.normalfont))
        {
	if(!LoadFont(newcw,newcw->indic.normalfont,False))
        if(!LoadFont(newcw,"fixed",False))
                XlError("XlIndic","SetValues",
			"Impossibile caricare default font");

        DrawIntoDirectly(newcw,False);
        DrawAgoValoreDirectly(newcw,1,False);
        do_redisplay = True;
        }

/*
 risorse connesse con sistema di simulazione
*/
if(strcmp(newcw->indic.cambio_colore[0].str_input,curcw->indic.cambio_colore[0].str_input))
        {
        Compile(newcw);
        do_redisplay = False;
        }


return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlIndicWidget cw= (XlIndicWidget) w;

/* elimina, se necessario, il widget dalla lista di blink */
if(XlFlagRiIsOn(cw->indic.cambio_colore[0]))
	XlRemoveBlink(cw,0);

/* rilascio i 3 GC */
if(cw->indic.norm_gc)
	XtReleaseGC(cw,cw->indic.norm_gc);

if(cw->indic.clear_gc)
	XtReleaseGC(cw,cw->indic.clear_gc);

if(cw->indic.ago_gc)
	XtReleaseGC(cw,cw->indic.ago_gc);

XlFreeCambioColoreGC(cw,cw->indic.cambio_colore[0],False);

/* libero la font_struct */
/***
if(cw->indic.font_info)
	XFreeFont(XtDisplay(cw),cw->indic.font_info);
***/

/* libero la font_list  */
/*****
if(cw->indic.font_list)
	XmFontListFree(cw->indic.font_list);
*****/

}

static void GetNormFgGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
XlIndicWidget cw= (XlIndicWidget) w;

unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;


values.line_width = 1;
values.line_style = LineSolid;
values.font = cw->indic.font_info->fid;
#if defined XPRINTER_USED
if(modoPrint)
	{
	values.foreground = XlPrintGetPixel(w,cw->indic.norm_fg);
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->indic.norm_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cw->indic.norm_fg;
	values.background = cw->core.background_pixel;
        cw->indic.norm_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->indic.norm_fg;
values.background = cw->core.background_pixel;
cw->indic.norm_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetClearGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlIndicWidget cw= (XlIndicWidget) w;
values.line_width = 0;
values.line_style = LineSolid;
values.font = cw->indic.font_info->fid;
#if defined XPRINTER_USED
if(modoPrint)
	{
	values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
	values.background = XlPrintGetPixel(w,cw->indic.norm_fg);
        cw->indic.clear_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cw->core.background_pixel;
	values.background = cw->indic.norm_fg;
        cw->indic.clear_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->core.background_pixel;
values.background = cw->indic.norm_fg;
cw->indic.clear_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetAgoGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
Pixel b;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFunction;
XlIndicWidget cw= (XlIndicWidget) w;
Widget Padre;

values.line_width = 0;
values.line_style = LineSolid;
values.font = cw->indic.font_info->fid;
#if defined XPRINTER_USED
if(modoPrint)
	{
	values.function=GXcopy;
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
	values.foreground = XlPrintGetPixel(w,cw->indic.ago_fg);
        cw->indic.ago_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
	}
else
	{
	values.function=GXxor;
	values.background = cw->core.background_pixel;
	values.foreground = cw->indic.ago_fg ^ cw->core.background_pixel;
        cw->indic.ago_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.function=GXxor;
values.background = cw->core.background_pixel;
values.foreground = cw->indic.ago_fg ^ cw->core.background_pixel;
cw->indic.ago_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetAllGCs(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XlIndicWidget cw= (XlIndicWidget)w;

GetNormFgGC(w,modoPrint);
GetAgoGC(w,modoPrint);
GetClearGC(w,modoPrint);
XlGetCambioColoreGC(cw,&(cw->indic.cambio_colore[0]),
		cw->indic.font_info->fid,NULL,modoPrint);
}

static void DrawIntoDirectly(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XlIndicWidget cw= (XlIndicWidget)w;
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
float massimo; 	/*valore massimo della var=100-dx-(cw->indic.extra_range_sup)*/
float scala;	
float lunghezza;


width=cw->core.width;
height=cw->core.height;
lunghezza=(cw->indic.valore_massimo)+(cw->indic.extra_range_sup)-(cw->indic.valore_minimo);
scala=((100-(2*dx))/lunghezza);
massimo=dx+(cw->indic.valore_massimo-cw->indic.valore_minimo)*scala;
/*
        se necessario sbianca lo sfondo
*/
if((cw->xlcore.trasparent!=True)&& !modoPrint)
	XlRiempiRettangolo(cw,XlWindow(cw),cw->indic.clear_gc,0,0,100,100);
	
switch(cw->indic.tipo_ind){
  case INDICATORE_AGO:
	{
	XlArco(cw,XlWindow(cw),cw->indic.norm_gc,
		-100+3*dx+dx_label_1,0+dy+dy_label_1,
		2*(100-2*dx-dx_label_1),2*(100-2*dy-dy_label_1),0*64,90*64);
	XlArco(cw,XlWindow(cw),cw->indic.norm_gc,
		dx/2,100-3*dy/2,dx,dy,0*64,360*64);
	XlLinea(cw,XlWindow(cw),cw->indic.norm_gc,
		100-dx-dx_label_1, 100-dy, 100-2*dx-dx_label_1, 100-dy);
	XlLinea(cw,XlWindow(cw),cw->indic.norm_gc,
		dx, dy+dy_label_1, dx, 2*dy+dy_label_1);
	sprintf(testo,"%*.*f",cw->indic.numero_int+cw->indic.numero_dec+1,
			cw->indic.numero_dec,cw->indic.valore_minimo);
	XlTesto(cw,XlWindow(cw),cw->indic.norm_gc, 
		100-(dx+dx_label_1)/2,100-dy,testo,
		cw->indic.font_info);
	sprintf(testo,"%*.*f", cw->indic.numero_int+cw->indic.numero_dec+1,
			cw->indic.numero_dec, cw->indic.valore_massimo);
	XlTesto(cw,XlWindow(cw),cw->indic.norm_gc, 
		dx,(dy+dy_label_1)/2,testo,cw->indic.font_info);
	r1=100-2*dx-dx_label_1+1;
	r2=100-2*dx-dx_label_1-dx+1;
	r3=100-dx_label_1;
	sprintf(testo,"%*.*f", cw->indic.numero_int+cw->indic.numero_dec+1,
		cw->indic.numero_dec, 
		(cw->indic.valore_massimo+cw->indic.valore_minimo)/2);
	XlTesto(cw,XlWindow(cw),cw->indic.norm_gc, 
		 dx+(int)(r3*cos(PIGRECO/4)),
			100-dy-(int)(r3*sin(PIGRECO/4)),
		testo,cw->indic.font_info);
	XlLinea(cw,XlWindow(cw),cw->indic.norm_gc,
		 dx+(int)(r2*cos(PIGRECO/4)),
			100-dy-(int)(r2*sin(PIGRECO/4)),
		 dx+(int)(r1*cos(PIGRECO/4)),
			100-dy-(int)(r1*sin(PIGRECO/4)));
	r2=100-2*dx-dx_label_1-dx/2+1;
	XlLinea(cw,XlWindow(cw),cw->indic.norm_gc,
		 dx+(int)(r2*cos(PIGRECO/8)),
			100-dy-(int)(r2*sin(PIGRECO/8)),
		 dx+(int)(r1*cos(PIGRECO/8)),
			100-dy-(int)(r1*sin(PIGRECO/8)));
	XlLinea(cw,XlWindow(cw),cw->indic.norm_gc,
		 dx+(int)(r2*cos(3*PIGRECO/8)),
			100-dy-(int)(r2*sin(3*PIGRECO/8)),
		 dx+(int)(r1*cos(3*PIGRECO/8)),
			100-dy-(int)(r1*sin(3*PIGRECO/8)));
	break;
	}
  case INDICATORE_BARRA:
	{
	XlLinea(cw,XtWindow(cw),cw->indic.norm_gc,
		dx, 100-dy, 100-dx, 100-dy); /*linea orzz.*/
	
	XlLinea(cw,XtWindow(cw),cw->indic.norm_gc,
		dx, 100-dy, dx, dy_label_2+(100-dy_label_2-dy)/3);/*vert. min*/
	
	XlLinea(cw,XtWindow(cw),cw->indic.norm_gc,
		100-dx, 100-dy, 100-dx,dy_label_2+(100-dy_label_2-dy)/3);/*extra_range*/
		
	XlLinea(cw,XtWindow(cw),cw->indic.norm_gc,
		 (int)massimo,
		 100-dy, 
		 (int)massimo, 
		 dy_label_2+(100-dy_label_2-dy)/3);/*valore_massimo*/	
		
	XlLinea(cw,XtWindow(cw),cw->indic.norm_gc,
		(int)((massimo-dx)/2+dx),
		 100-dy,
		 (int)((massimo-dx)/2+dx), 
		 dy_label_2+(100-dy_label_2-dy)/3);/*metà scala*/

	XlLinea(cw,XtWindow(cw),cw->indic.norm_gc,
		(int)((massimo-dx)/4+dx),
		 100-dy, 
		(int)((massimo-dx)/4+dx),
		 dy_label_2+2*(100-dy_label_2-dy)/3);/*1/4 val. massimo*/

	XlLinea(cw,XtWindow(cw),cw->indic.norm_gc,
		(int)((massimo-dx)*3/4+dx),
		 100-dy, 
		(int)((massimo-dx)*3/4+dx),
		 dy_label_2+2*(100-dy_label_2-dy)/3);/*3/4 val. massimo*/
/*
Valore minimo
*/		 
	sprintf(testo,"%*.*f", cw->indic.numero_int+cw->indic.numero_dec+1,
			cw->indic.numero_dec, cw->indic.valore_minimo);
	XlTesto(cw,XtWindow(cw),cw->indic.norm_gc, 
		dx,dy_label_2/2,testo,cw->indic.font_info);

/*
Extra range Sup.
*/
	sprintf(testo,"%*.*f", cw->indic.numero_int+cw->indic.numero_dec+1,
			cw->indic.numero_dec, 
			(cw->indic.valore_massimo)+(cw->indic.extra_range_sup));
	XlTesto(cw,XtWindow(cw),cw->indic.norm_gc,
		100-dx,dy_label_2/2,testo,cw->indic.font_info);

/*
Valore medio
*/
	sprintf(testo,"%*.*f", cw->indic.numero_int+cw->indic.numero_dec+1,
		cw->indic.numero_dec,
		(cw->indic.valore_massimo+cw->indic.valore_minimo)/2);
	XlTesto(cw,XtWindow(cw),cw->indic.norm_gc,
		(int)((massimo-dx)/2+dx),dy_label_2/2,testo,cw->indic.font_info);
/*
Valore massimo
*/	
	sprintf(testo,"%*.*f", cw->indic.numero_int+cw->indic.numero_dec+1,
			cw->indic.numero_dec, 
			(cw->indic.valore_massimo));
	XlTesto(cw,XtWindow(cw),cw->indic.norm_gc,
		(int)(massimo),dy_label_2/2,testo,cw->indic.font_info);
	break;
	}
  case INDICATORE_ISTOGRAMMA_CRES:
  case INDICATORE_ISTOGRAMMA_DEC:
	{
/**
	XlLinea(cw,XtWindow(cw),cw->indic.norm_gc,
		dx, 100-dy, 100-dx, 100-dy);
	XlLinea(cw,XtWindow(cw),cw->indic.norm_gc,
		dx, 100-dy, dx, dy_label_2);
	XlLinea(cw,XtWindow(cw),cw->indic.norm_gc,
		100-dx, 100-dy, 100-dx, dy_label_2);
	XlLinea(cw,XtWindow(cw),cw->indic.norm_gc,
		50, 100-dy, 50, dy_label_2+(100-dy_label_2-dy)/3);
	XlLinea(cw,XtWindow(cw),cw->indic.norm_gc,
		dx+(50-dx)/2, 100-dy, 
		dx+(50-dx)/2, dy_label_2+2*(100-dy_label_2-dy)/3);
	XlLinea(cw,XtWindow(cw),cw->indic.norm_gc,
		50+(50-dx)/2, 100-dy, 
		50+(50-dx)/2, dy_label_2+2*(100-dy_label_2-dy)/3);
	sprintf(testo,"%*.*f", cw->indic.numero_int+cw->indic.numero_dec+1,
			cw->indic.numero_dec, cw->indic.valore_minimo);
	XlTesto(cw,XtWindow(cw),cw->indic.norm_gc, 
		dx,dy_label_2/2,testo,cw->indic.font_info);
	sprintf(testo,"%*.*f", cw->indic.numero_int+cw->indic.numero_dec+1,
			cw->indic.numero_dec, cw->indic.valore_massimo);
	XlTesto(cw,XtWindow(cw),cw->indic.norm_gc,
		100-dx,dy_label_2/2,testo,cw->indic.font_info);
	sprintf(testo,"%*.*f", cw->indic.numero_int+cw->indic.numero_dec+1,
		cw->indic.numero_dec,
		(cw->indic.valore_massimo-cw->indic.valore_minimo)/2);
	XlTesto(cw,XtWindow(cw),cw->indic.norm_gc,
		50,dy_label_2/2,testo,cw->indic.font_info);
*****/
	break;
	}
  case INDICATORE_DISPLAY:
	{
	/*
		disegna il rettangolo di contorno
	*/
	/*
	XlRettangolo(cw,XtWindow(cw),cw->indic.norm_gc,2,2,96,96);
	*/
	break;
	}
  default:
	XtWarning("(XlIndic) tipo inesistente");
	}
	
}




static Boolean Refresh(Widget w)
{
XlIndicWidget cw= (XlIndicWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;
int core_x=cw->core.x;
int core_y=cw->core.y;
Boolean old_blink;


if(cw->xlcore.config==True)
        return(True);


if(!XtIsWidget(w))
	{
	XlWarning("XlIndic","Refresh",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}

if(xlIndicClassRec.xlcore_class.database==NULL)
	{
	XlWarning("XlIndic","Refresh","database non definito");
	return(False);
	}

if(cw->indic.primo_refresh== 1) /* primo refresh */
        {
        cw->indic.primo_refresh = 0;
        if(XlFlagRiIsOn(cw->indic.cambio_colore[0]))
                                XlAddBlink(cw,0,FREQUENZA_BASE,(void*)BlinkProc);
        }

old_blink=XlFlagRiIsOn(cw->indic.cambio_colore[0]);
if(!(*superclass->xlcore_class.refresh) (w))
	{
	XlWarning("XlIndic","Refresh","errore refresh classe superiore");
	return(False);
	}
if(XlFlagRiIsOn(cw->indic.cambio_colore[0]) != old_blink)
	{
	if(old_blink)
		XlRemoveBlink(cw,0);
	else
		XlAddBlink(cw,0,FREQUENZA_BASE,(void*)BlinkProc);/*aggiungi alla lista blink*/
	}


DrawAgoValoreDirectly(w,0,False);

return(True);
}


static void Resize(w)
Widget w;
{
XlIndicWidget cw= (XlIndicWidget)w;
int app_int;

DrawIntoDirectly(cw,False);
DrawAgoValoreDirectly(cw,1,False);
}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlIndicWidget cw= (XlIndicWidget)w;
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
DrawAgoValoreDirectly(cw,1,False);
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

static Boolean LoadFont( cw,fontname,modoPrint)
Widget cw;
char fontname[];
Boolean modoPrint;
{
XlIndicWidget Xl_cw = (XlIndicWidget) cw;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
/* Carica il font ottenendo la descrizione del font stesso */

char **pf;
int i,nf;
Display *disp;

if(modoPrint)
	{
	disp=XlDisplay(cw);
	if((Xl_cw->indic.font_info = XLoadQueryFont(disp,fontname)) == NULL)
       		{	
		XlWarning("XlIndic","LoadFont","Cannot open font");
		return(False);
        	}
	}
else
	{
	disp=XtDisplay(cw);
	if((Xl_cw->indic.font_info = XlGetFont(disp,fontname)) == NULL)
       		{	
		XlWarning("XlIndic","LoadFont","Cannot open font");
		return(False);
        	}
	}

return(True);
}


static void DrawAgoValoreDirectly(w,modo,modoPrint)
Widget w;
int modo;
Boolean modoPrint;
{
XlIndicWidget cw= (XlIndicWidget)w;
float valore = cw->indic.cambio_colore[0].input.valore;
float scalamento = cw->indic.scalamento;
float offset = cw->indic.offset;
float min = cw->indic.valore_minimo;
float max = cw->indic.valore_massimo+cw->indic.extra_range_sup;
int dx=10;
int dy=10;
int dx_label_1=15;
int dy_label_1=15;
int dx_label_2=10;
int dy_label_2=50;
float posizione;
int r1,r2;
char testo[40];
XExposeEvent event;
Dimension indic_w,indic_h,indic_b;
Position indic_x,indic_y;
Pixel pixel_set;
GC gc_set;
Arg arg[2];
Boolean modo_blink=False;
Boolean On;

/*
	per la gestione dell'eventuale stato di blink.
	
	Se modo>=10->la funz. è stata invocata da BlinkProc o 
	da qualsiasi altra funz. associata ad un oggetto contenuto 
	nella lista degli oggetti-blink.Se modo>10 On=True 
	
*/
if(modo >= 10)
	{
	On = modo - 10;
	modo = 1;
	modo_blink = True;
	}



if(cw->xlcore.config==True)
	valore=(max-min)*3/5+min;
valore = valore*scalamento+offset;
if(scalamento<0)
	printf("Valore = %f\n",valore);
if(valore>max)
	valore=max;
else
	if(valore<min)
        	valore=min;

if(min==max)
	valore=min;

switch(cw->indic.tipo_ind){
  case INDICATORE_AGO:
	{
	if(modo_blink) 
		return;
	posizione=((valore-min)/(max-min))*PIGRECO/2;
	if((cw->indic.old_posizione!=posizione)||modo)
	   {
	   if((cw->indic.old_posizione>=0)&& !modo)
	      {
	      r1=dx/2;
	      r2=100-2*dx-dx_label_1-dx+1;
	      XlLinea(cw,XtWindow(cw),cw->indic.ago_gc,
		       dx+(int)(r1*cos(cw->indic.old_posizione)),
                              100-dy-(int)(r1*sin(cw->indic.old_posizione)),
		       dx+(int)(r2*cos(cw->indic.old_posizione)),
                              100-dy-(int)(r2*sin(cw->indic.old_posizione)));
	      }
	   cw->indic.old_posizione=posizione;
	   r1=dx/2;
	   r2=100-2*dx-dx_label_1-dx+1;
	   XlLinea(cw,XtWindow(cw),cw->indic.ago_gc,
	      dx+(int)(r1*cos(posizione)),
                           100-dy-(int)(r1*sin(posizione)),
	      dx+(int)(r2*cos(posizione)),
                           100-dy-(int)(r2*sin(posizione)));
	   }
	break;
	}
  case INDICATORE_BARRA:
	{
	if(modo_blink) 
		return;
        posizione=((valore-min)/(max-min))*(100-2*dx)+dx;
	if((cw->indic.old_posizione!=posizione)||modo)
	   {
	   if((cw->indic.old_posizione>=0) && !modo)
	      {
	      XlLinea(cw,XtWindow(cw),cw->indic.ago_gc,
		   (int)cw->indic.old_posizione, 100-dy, 
		   (int)cw->indic.old_posizione, 
		    dy_label_2);
	      }
	   cw->indic.old_posizione=posizione;
           XlLinea(cw,XtWindow(cw),cw->indic.ago_gc,
                  (int)posizione, 100-dy, (int)posizione,
                  dy_label_2);
	   }
	break;
	}
  case INDICATORE_ISTOGRAMMA_CRES:
  case INDICATORE_ISTOGRAMMA_DEC:
        {
	if(modo_blink) 
		return;
	dx=0;
	dy=0;
	if(cw->indic.tipo_ind == INDICATORE_ISTOGRAMMA_CRES)
        	posizione=((valore-min)/(max-min))*(100-2*dx)+dx;
	else
        	posizione=100.0-(((valore-min)/(max-min))*(100-2*dx)+dx);
	if(cw->xlcore.config==True)
	   {
           if((cw->indic.old_posizione!=posizione)||modo)
              {
              if((cw->indic.old_posizione>=0) && (!modo))
                 {
	         XlRiempiRettangolo(cw,XtWindow(cw),cw->indic.ago_gc,
		   dx,dy,(int)cw->indic.old_posizione-dx,100-2*dy);
	   	
                 }
              cw->indic.old_posizione=posizione;
	      XlRiempiRettangolo(cw,XtWindow(cw),cw->indic.ago_gc,
		      dx,dy,(int)posizione-dx,100-2*dy);
              }
	   }
	else
	   {
	   /* run-time */
	   pixel_set = XlFlagToPixel(cw->indic.cambio_colore[0]);
	   if(pixel_set != cw->indic.OldPixel)
		{
	   	cw->indic.OldPixel = pixel_set;
	   	XtSetArg(arg[0],XmNbackground,pixel_set);
	   	XtSetValues(cw,arg,1);
		}
	   CalcIstog(cw,&indic_x,&indic_y,&indic_w,&indic_h,posizione);
/*        
	per gestione blink
	   if(modo_blink && On)
		{
		indic_w = 0;
		indic_h = 0;
		}
*/
	   if(indic_w > 0 && indic_h>0)
		{
		if(!XtIsManaged(cw))
			XtManageChild(cw);
	   	XtConfigureWidget(cw,indic_x,indic_y,	
			indic_w,indic_h,cw->core.border_width);
		}
	   else
		XtUnmanageChild(cw);
	   }
        break;
        }
  case INDICATORE_DISPLAY:
	{
	if((cw->indic.old_posizione!=valore)||
    (memcmp(&(cw->indic.OldFlag),&(cw->indic.cambio_colore[0].input.flag),sizeof(FLAG_MMI))!=0)||
		(cw->indic.old_posizione== -1)||modo)
	   {
	   if(cw->indic.numero_dec==0)
		sprintf(testo,"%*d", cw->indic.numero_int,(int)valore);
	   else
	   	sprintf(testo,"%*.*f", cw->indic.numero_int+cw->indic.numero_dec+1,
			cw->indic.numero_dec, valore);
	  /*
        	   se necessario sbianca lo sfondo
	   */
	   gc_set = XlFlagToGC(cw->indic.cambio_colore[0]);
	   if((cw->xlcore.trasparent!=True) && !modoPrint)
	      {
	      if(XlFlagFzIsOn(cw->indic.cambio_colore[0]))
                 {
	         XlRiempiRettangolo(cw,XtWindow(cw),gc_set,
			   1,1,100,100);
                 if(!(modo_blink && On))
                        XlTestoSimpleCentre(cw,XtWindow(cw),cw->indic.clear_gc,
				50,50,testo,cw->indic.font_info);
		 }
	      else
                 {
	           XlRiempiRettangolo(cw,XtWindow(cw),cw->indic.clear_gc,
			   1,1,100,100);
		   if(!(modo_blink && On))
                      XlTestoSimpleCentre(cw,XtWindow(cw),gc_set,50,50,
			testo,cw->indic.font_info);
		 }
	      }
	   else
	      {
/*
Se l' invocazione è avvenuta da BlinkProc e 
*/	      
	      if(!(modo_blink && On)) 
	          XlTestoSimpleCentre(cw,XtWindow(cw),gc_set,50,50,
                        testo,cw->indic.font_info);
              }
	   cw->indic.old_posizione=valore;
	   memcpy(&(cw->indic.OldFlag),
		&(cw->indic.cambio_colore[0].input.flag),sizeof(FLAG_MMI));
	   }
	break;
	}
   }
}

static Boolean Compile(Widget w)
{
Boolean errors=False;
XlIndicWidget cw= (XlIndicWidget)w;

if(!XtIsWidget(w))
	{
	XlWarning("XlIndic","Compile",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}

/*
 Deve esistere la variabile di input
*/
if(strlen(cw->indic.cambio_colore[0].str_input)==0)
        {
	XlErrComp(w,"Compile","Input variable not defined",NULL);
        errors=True;
        }

if(xlIndicClassRec.xlcore_class.compilatore==NULL)
	{
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
	return(False);
	}

if(!(*superclass->xlcore_class.compile) (w,
     xlIndicClassRec.xlcore_class.compilatore))
	{
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
	return(False);
	}
XlEvError(cw,errors);
return(True);
}

void CalcIstog(Widget wid,Position *pos_x, Position *pos_y, 
		Dimension *w, Dimension *h,float pos)
{
XlIndicWidget cw= (XlIndicWidget)wid;

	   *pos_x = cw->core.x;
	   *pos_y = cw->core.y;
	   *w = cw->core.width;
	   *h = cw->xlcore.height0*(pos/100.0);
	switch(cw->xlcore.rotate){
                case ROTATE_0:
			{
	   		*w = cw->xlcore.width0*(pos/100.0);
	   		*h = cw->core.height;
	   		*pos_x = cw->core.x;
	   		*pos_y = cw->core.y;
			if(cw->xlcore.ass_rotate == Y_ASS_ROTATE)
				{
				*pos_x = cw->indic.starting_x - *w +
					cw->xlcore.width0;
				}
			break;
			}
                case ROTATE_90:
			{
	   		*pos_x = cw->core.x;
	   		*pos_y = cw->core.y;
	   		*w = cw->core.width;
	   		*h = cw->xlcore.width0*(pos/100.0);
			break;
			}
                case ROTATE_180:
			{
	   		*w = cw->xlcore.width0*(pos/100.0);
	   		*h = cw->core.height;
	   		*pos_x = cw->indic.starting_x - *w + cw->xlcore.width0;
	   		*pos_y = cw->core.y;
			if(cw->xlcore.ass_rotate == Y_ASS_ROTATE)
				{
				*pos_x = cw->indic.starting_x -
					cw->xlcore.width0+ cw->xlcore.width0;
				}
			break;
			}
                case ROTATE_270:
			{
	   		*w = cw->core.width;
	   		*h = cw->xlcore.width0*(pos/100.0);
	   		*pos_x = cw->core.x;
	   		*pos_y = cw->indic.starting_y + cw->xlcore.width0 - *h;
			break;
			}
		}
}

static void BlinkProc(Widget w,int ind,Boolean On)
{
XlIndicWidget cw= (XlIndicWidget) w;
	
/*
	printf("Metodo di Blink oggetto %s ind=%d On=%d\n",
		XtName(cw),ind,On);
*/
        DrawAgoValoreDirectly(w,10+On);
}

/*
        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
XlIndicWidget cw= (XlIndicWidget) w;
char *printerFont;

/*
	richiamo il metodo di print di Core
*/
if(!((cw->indic.tipo_ind==INDICATORE_ISTOGRAMMA_CRES)||
        (cw->indic.tipo_ind==INDICATORE_ISTOGRAMMA_DEC)))
(*superclass->xlcore_class.print) (w);


/*
        richiamo il ridisegno
*/
printerFont=XtNewString(XlPrinterGetFont(w,
		cw->indic.normalfont,cw->indic.font_info));
/* libero il font */
/*
if(cw->indic.font_info)
	XFreeFont(XtDisplay(w),cw->indic.font_info);
*/
/* rilascio i 3 GC */
if(cw->indic.norm_gc)
	XtReleaseGC(w,cw->indic.norm_gc);
if(cw->indic.clear_gc)
	XtReleaseGC(w,cw->indic.clear_gc);
if(cw->indic.ago_gc)
	XtReleaseGC(w,cw->indic.ago_gc);
XlFreeCambioColoreGC(cw,cw->indic.cambio_colore[0],False);
		

if(!LoadFont(w,printerFont,True))
	LoadFont(w,"fixed",True);
XtFree(printerFont);
GetAllGCs(w,True);


/*
	redisplay dell'indic
*/
DrawIntoDirectly(cw,True);
DrawAgoValoreDirectly(cw,1,True);


/* rilascio i 3 GC  della stampante*/
if(cw->indic.norm_gc)
        XFreeGC(XlDisplay(w),cw->indic.norm_gc);
if(cw->indic.clear_gc)
        XFreeGC(XlDisplay(w),cw->indic.clear_gc);
if(cw->indic.ago_gc)
        XFreeGC(XlDisplay(w),cw->indic.ago_gc);
XlFreeCambioColoreGC(cw,cw->indic.cambio_colore[0],True);
/* libero il font della stampante */
if(cw->indic.font_info)
	XFreeFont(XlDisplay(w),cw->indic.font_info);

if(!LoadFont(cw,cw->indic.normalfont,False))
	LoadFont(cw,"fixed",False);
GetAllGCs(cw,False);

return(True);
}
#endif
