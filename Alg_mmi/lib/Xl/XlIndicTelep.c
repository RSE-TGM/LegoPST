/**********************************************************************
*
*       C Source:               XlIndicTelep.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:40:53 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlIndicTelep.c-5.1.3 %  (%full_filespec: XlIndicTelep.c-5.1.3:csrc:1 %)";
#endif
/*
   modulo XlIndicTelep.c 
   tipo 
   release 5.7
   data 1/30/96
   reserved @(#)XlIndicTelep.c	5.7

*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlIndicTelep.c	5.5\t1/16/96";
/*
        Fine sezione per SCCS
*/
/*
 *  XlIndicTelep.c - widget indic per teleperm
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlIndicTelepP.h>
#include <Xl/XlDrawingUtil.h>
#include <Xl/XlPrint.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

#define DEFAULTWIDTH  5
#define DEFAULTHEIGHT 5


/* lista delle risorse  */
static XtResource resources[]= {
        {
        XlNagoFg_tel,
        XlCAgoFg_tel,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicTelepRec,indic.ago_fg),
        XmRString,
        "red"
        },
        {
        XlNowOpen,
        XlCOwOpen,
        XmRBoolean,
        sizeof(Boolean),
        XtOffsetOf(XlIndicTelepRec,indic.owOpen),
        XmRImmediate,
        False
        },
        {
        XlNbordinoFg,
        XlCBordinoFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIndicTelepRec,indic.bordino_fg),
        XmRString,
        "grey"
        },
        {
        XlNbordinoWidth,
        XlCBordinoWidth,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIndicTelepRec,indic.bordino_width),
        XmRImmediate,
        (XtPointer)1 
        },
        {
        XlNindicatorText,
        XlCIndicatorText,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIndicTelepRec,indic.indicText),
        XmRImmediate,
        "text"
        },
        {
        XlNtipoInd,
        XlCTipoInd,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIndicTelepRec,indic.tipo_ind),
        XmRImmediate,
        (XtPointer)INDICATORE_LED_RECT
        },
        {
        XlNnumeroInt,
        XlCNumeroInt,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIndicTelepRec,indic.numero_int),
        XmRImmediate,
        (XtPointer)NUMERO_INTERI_VISUALIZZATI
        },
        {
        XlNnumeroDec,
        XlCNumeroDec,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIndicTelepRec,indic.numero_dec),
        XmRImmediate,
        (XtPointer)NUMERO_DECIMALI_VISUALIZZATI
        },
	{
        XlNscalamento,
        XlCScalamento,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicTelepRec,indic.scalamento),
        XtRString,
        VALORE_SCALAMENTO
        },
	{
        XlNvaloreMinimo,
        XlCValoreMinimo,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicTelepRec,indic.valore_minimo),
        XtRString,
        VALORE_MINIMO
        },
	{
        XlNvaloreMassimo,
        XlCValoreMassimo,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicTelepRec,indic.valore_massimo),
        XtRString,
        VALORE_MASSIMO
        },
        {
        XlNextraRangeSup,
        XlCExtraRangeSup,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicTelepRec,indic.extra_range_sup),
        XtRString,
        EXTRA_RANGE_SUP
        },
	{
        XlNoffset,
        XlCOffset,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlIndicTelepRec,indic.offset),
        XtRString,
        VALORE_OFFSET
        },
	{
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIndicTelepRec,indic.normalfont),
        XmRImmediate,
        "fixed"
        },
        {
        XlNvarInputCambioColore1,
        XlCVarInputCambioColore1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIndicTelepRec,indic.cambio_colore[0].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNcoloreVarOn,
        XlCColoreVarOn,
        XtRAnimatedColor,
        sizeof(XlAnimatedColor),
        XtOffsetOf(XlIndicTelepRec,indic.cambio_colore[0].VarColorOn),
        XmRString,
        XlColoreVarOn
        },
        {
        XlNcoloreVarOff,
        XlCColoreVarOff,
        XtRAnimatedColor,
        sizeof(XlAnimatedColor),
        XtOffsetOf(XlIndicTelepRec,indic.cambio_colore[0].VarColorOff),
        XmRString,
        XlColoreVarOff
        },
        {
        XlNvarInputCambioColore2,
        XlCVarInputCambioColore2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIndicTelepRec,indic.colore_fault[0].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNvarInputCambioColore3,
        XlCVarInputCambioColore3,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIndicTelepRec,indic.colore_fault[1].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNcoloreFaultOn,
        XlCColoreFaultOn,
        XtRAnimatedColor,
        sizeof(XlAnimatedColor),
        XtOffsetOf(XlIndicTelepRec,indic.colore_fault[0].VarColorOn),
        XmRString,
        XlColoreFaultOn
        },
        {
        XlNframeWidth,
        XlCFrameWidth,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIndicTelepRec,indic.frame.frameWidth),
        XmRImmediate,
        (int)0
        },
        {
        XlNvarInputFrame,
        XlCVarInputFrame,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIndicTelepRec,indic.frame.str_input),
        XmRImmediate,
        ""
        },
        {
        XlNowFrameColor,
        XlCOwFrameColor,
        XtRAnimatedColor,
        sizeof(XlAnimatedColor),
        XtOffsetOf(XlIndicTelepRec,indic.frame.owFrameColor),
        XmRString,
        XlDefaultColore1
        },
        {
        XlNvarFrameColor,
        XlCVarFrameColor,
        XtRAnimatedColor,
        sizeof(XlAnimatedColor),
        XtOffsetOf(XlIndicTelepRec,indic.frame.varFrameColor),
        XmRString,
        XlDefaultColore1
        },
        };

/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
        {
        XlNagoFg_tel,
        XlCAgoFg_tel,
        XlDagoFg_tel,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlIndicTelepRec,indic.ago_fg),
        sizeof(Pixel),
        },
        {
        XlNbordinoFg,
        XlCBordinoFg,
        XlDbordinoFg,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlIndicTelepRec,indic.bordino_fg),
        sizeof(Pixel),
        },
        {
        XlNbordinoWidth,
        XlCBordinoWidth,
        XlDbordinoWidth,
        XlRInt,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlIndicTelepRec,indic.bordino_width),
        sizeof(int),
        },
        {
        XlNindicatorText,
        XlCIndicatorText,
        XlDindicatorText,
        XlRText,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlIndicTelepRec,indic.indicText),
        sizeof(char *),
        },
        {
        XlNtipoInd,
        XlCTipoInd,
        XlDtipoInd,
        XlROption,
        NULL,
        XlOtipoInd,
        XlRGrIo,
        XtOffsetOf(XlIndicTelepRec,indic.tipo_ind),
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
        XtOffsetOf(XlIndicTelepRec,indic.numero_int),
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
        XtOffsetOf(XlIndicTelepRec,indic.numero_dec),
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
        XtOffsetOf(XlIndicTelepRec,indic.scalamento),
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
        XtOffsetOf(XlIndicTelepRec,indic.valore_minimo),
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
        XtOffsetOf(XlIndicTelepRec,indic.valore_massimo),
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
        XtOffsetOf(XlIndicTelepRec,indic.extra_range_sup),
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
        XtOffsetOf(XlIndicTelepRec,indic.offset),
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
        XtOffsetOf(XlIndicTelepRec,indic.normalfont),
        sizeof(XFontStruct*)
        },
        {
        XlNframeWidth,
        XlCFrameWidth,
        XlDframeWidth,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicTelepRec,indic.frame.frameWidth),
        sizeof(int)
        },
        {
        XlNvarInputFrame,
        XlCVarInputFrame,
        XlDvarInputFrame,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicTelepRec,indic.frame.input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNowFrameColor,
        XlCOwFrameColor,
        XlDowFrameColor,
        XlRAnimatedColor,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicTelepRec,indic.frame.owFrameColor),
        sizeof(XlAnimatedColor)
        },
        {
        XlNvarFrameColor,
        XlCVarFrameColor,
        XlDvarFrameColor,
        XlRAnimatedColor,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIndicTelepRec,indic.frame.varFrameColor),
        sizeof(XlAnimatedColor)
        },
        {
        XlNvarInputCambioColore1,
        XlCVarInputCambioColore1,
        XlDvar,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicTelepRec,indic.cambio_colore[0].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNcoloreVarOn,
        XlCColoreVarOn,
        XlDcoloreVarOn,
        XlRAnimatedColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicTelepRec,indic.cambio_colore[0].VarColorOn),
        sizeof(XlAnimatedColor)
        },
        {
        XlNcoloreVarOff,
        XlCColoreVarOff,
        XlDcoloreVarOff,
        XlRAnimatedColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicTelepRec,indic.cambio_colore[0].VarColorOff),
        sizeof(XlAnimatedColor)
        },
        {
        XlNvarInputCambioColore2,
        XlCVarInputCambioColore2,
        XlDfault,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicTelepRec,indic.colore_fault[0].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNvarInputCambioColore3,
        XlCVarInputCambioColore3,
        XlDfaultRi,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicTelepRec,indic.colore_fault[1].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNcoloreFaultOn,
        XlCColoreFaultOn,
        XlDcoloreFaultOn,
        XlRAnimatedColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIndicTelepRec,indic.colore_fault[0].VarColorOn),
        sizeof(XlAnimatedColor)
        },
	};

/* dichiarazioni funzioni varie */
static void BlinkProc(Widget, int, Boolean);
static int min();
static void GetNormFgGC();
static void GetClearGC();
static void GetAgoGC();
static void GetFaultGC();
static void GetVarOnGC();
static void GetVarOffGC();
static void GetBordinoGC();
static void GetFrameGC();
static void GetAllGCs();
static Boolean LoadFont();
static void DrawLabel();
static void DrawIntoDirectly();
static void DrawAgoValoreDirectly();
static void CalcIstog(Widget, Position *, Position *, Dimension *,
		Dimension *, float);
static void DrawFrame(Widget,Boolean); /* disegna il frame */
         
static void CalcXYWH0(Widget, int *, int *,int *, int *);  /* calcola ingombro */

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
#if defined XPRINTER_USED && !defined LINUX
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
XlIndicTelepClassRec xlIndicTelepClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "IndicTelep",
    /* widget_size              */      sizeof(XlIndicTelepRec),
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
#if defined XPRINTER_USED && !defined LINUX
    /* print                    */      Print,
#endif
  },
  { /* indic fields */
    /* empy                   */      0,
  }
};

WidgetClass xlIndicTelepWidgetClass = (WidgetClass) &xlIndicTelepClassRec;


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
   XlIndicTelepWidget new = (XlIndicTelepWidget)tnew;
   int app_int;
   Arg arg[2];
   int zoom;


   /* la width non puo' essere minore di quella minima
   */
   if(new->core.width<DEFAULTWIDTH)
   {
      new->core.width=DEFAULTWIDTH;
      XlWarning("XlIndicTelep","Initialize",
	     "width minore della dimensione minima");
   }

   /* la heigth non puo' essere minore di quella minima
   */
   if(new->core.height<DEFAULTHEIGHT)
   {
      new->core.height=DEFAULTWIDTH;
      XlWarning("XlIndicTelep","Initialize",
		"height minore della dimensione minima");
   }

   /* il tipo deve essere tra quelli previsti
   */
   if(new->indic.tipo_ind>_INDIC_MAX_tel)
   {
      new->indic.tipo_ind=INDICATORE_DEFAULT;
      XlWarning("XlIndicTelep","Initialize",
		"indicatore inesistente, tipo settato al valore di default");
   }


    
   /* controllo la rotazione per l'Istogramma
   */
   if((new->core.width!=new->core.height)&&
      (new->indic.tipo_ind==INDICATORE_ISTOGRAMMA_CRES)||
      (new->indic.tipo_ind==INDICATORE_ISTOGRAMMA_DEC))
   {
      if((new->xlcore.rotate==ROTATE_90)||
         (new->xlcore.rotate==ROTATE_270))
      {
         app_int = new->core.width;
         new->core.width = new->core.height;
         new->core.height = app_int;
      }
   }

   /* l'indicatore di tipo DISPLAY non e' ruotabile
   */
   if((new->indic.tipo_ind==INDICATORE_DISPLAY)      ||
      (new->indic.tipo_ind==INDICATORE_LED_RECT)     ||
      (new->indic.tipo_ind==INDICATORE_LED_CIRCLE)   ||
      (new->indic.tipo_ind==INDICATORE_TEXT_FG_AN)   ||    
      (new->indic.tipo_ind==INDICATORE_TEXT_BG_AN))
   {
      new->xlcore.rotate=0;
      new->xlcore.ass_rotate=0;
   }

   /* l'indicatore istogramma ha come background il colore specificato dalla risorsa
      Ago or Bar color
   */
   if(((new->indic.tipo_ind==INDICATORE_ISTOGRAMMA_CRES)||
       (new->indic.tipo_ind==INDICATORE_ISTOGRAMMA_DEC))&& 
      new->xlcore.config != True)
   {
      new->core.background_pixel=new->indic.ago_fg;

      /* salvo le posizioni x e y 
      */
      new->indic.starting_x = new->core.x;
      new->indic.starting_y = new->core.y;
   }
   

   /* controllo il valore dei campi di rotazione
   */
   if(new->xlcore.rotate>3)
   {
      XlWarning("XlIndicTelep","Initialize","rotate > 3");
      new->xlcore.rotate=new->xlcore.rotate%4;
   }

   if(new->xlcore.ass_rotate>1)
   {
      XlWarning("XlIndicTelep","Initialize","assRotate > 1");
      new->xlcore.ass_rotate=new->xlcore.ass_rotate%2;
   }

   /* controllo i valori massimo e minimo
   */
   if(new->indic.valore_massimo<=new->indic.valore_minimo)
   {
      XlWarning("XlIndicTelep","SetVAlues",
	    "il valore massimo deve essere maggiore del minimo");
      new->indic.valore_massimo=atof(VALORE_MASSIMO);
      new->indic.valore_minimo=atof(VALORE_MINIMO);
   }

   /* controllo il valore extra-range
   */
   if(new->indic.extra_range_sup<0)
   {
      XlWarning("XlIndicTelep","SetVAlues",
	    "il valore dell' extra-range deve essere positivo");
      new->indic.extra_range_sup=atof(EXTRA_RANGE_SUP);
   }

   /* Il valore di extra range viene per ora utilizzato solo
       nel caso di  display a barra (negli altri casi il valore
       dell'extra range viene per il momento forzato a zero
   */
/*
   if(new->indic.tipo_ind!=INDICATORE_BARRA)
	new->indic.extra_range_sup=atof(EXTRA_RANGE_SUP);
*/

   /* inizializzo altri campi
   */
   new->indic.primo_refresh = 1;
   new->indic.cambio_colore[0].input.valore = 0; /* input variable */
   new->indic.colore_fault[0].input.valore = 0;  /* fault variable */
   new->indic.blinking = False;
   new->indic.OldPixel = -1;
   new->indic.old_posizione = -1;
   new->indic.frame.blink = False;
   new->indic.frame.input.valore = 0;
   new->indic.old_frameval = 0;
   memset(&(new->indic.OldFlag),sizeof(FLAG_MMI),0);


   /* se non risco a caricare il font specificato
      provo con il fixed
      se anche questo fallisce c'e' un errore
   */
   if(!LoadFont(new,new->indic.normalfont,False))
     if(!LoadFont(new,"fixed",False))
	XlError("XlIndicTelep","Initialize","Impossibile caricare default font");

   /* calcola la width  e la height per l'indicatore tipo testo
   */
   if(new->indic.tipo_ind==INDICATORE_TEXT_FG_AN || 
      new->indic.tipo_ind==INDICATORE_TEXT_BG_AN    )
   {
      new->core.height=new->indic.font_info->ascent+
                        new->indic.font_info->descent;
      new->core.width=XTextWidth(new->indic.font_info,
                new->indic.indicText,strlen(new->indic.indicText));
      if(XlIsXlComposite(XtParent(new)))
      {
                zoom=XlGetZoom(XtParent(new));
                new->core.width = new->core.width * zoom/100;
                new->core.height = new->core.height * zoom/100;
      }
      new->xlcore.rotate=0;
      new->xlcore.ass_rotate=0;
   }


   /* inizializzo i vari gc
   */
   GetAllGCs(new,False);

   new->indic.owOpen = False;

   /* in run time setto l'indicatore operabile o meno in base
      alla configurazione della variabile di controllo del frame
   */
   /* 26-6-97 su richiesta di Bolis correggo questa condizione.
      L'indicatore e' operabile se la OW associata e' configurata
   */ 
   if(new->xlcore.config!=True)
   {
/*
      if( strlen(new->indic.frame.str_input) > 0 )
*/
      if( strlen(new->xlcore.pagina_staz) > 0 ) 
         new->indic.operable = True;
      else
         new->indic.operable = False;
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



static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
   XlIndicTelepWidget curcw= (XlIndicTelepWidget) current;
   XlIndicTelepWidget newcw= (XlIndicTelepWidget) new;
   Boolean do_redisplay = False;
   Arg core_args[2];

   int app_int;


/* print debug
printf("Chiamata SetValues config = %d\n",curcw->xlcore.config);
printf("newcw->indic.owOpen=%d\n",newcw->indic.owOpen);
*/

   if( curcw->indic.owOpen != newcw->indic.owOpen )
   {
           if( newcw->indic.owOpen == False )
            {
               newcw->indic.frame.blink = False;
               XlRemoveBlink(new,1);
            }
      do_redisplay=True;
   }

   /*
	l'indicatore tipo INDICATORE_DISPLAY non e ruotabile
   */
   if((curcw->xlcore.rotate != newcw->xlcore.rotate)&&
	(curcw->indic.tipo_ind == INDICATORE_DISPLAY))
	   newcw->xlcore.rotate=0;

   if((curcw->xlcore.ass_rotate != newcw->xlcore.ass_rotate)&&
	(curcw->indic.tipo_ind == INDICATORE_DISPLAY))
	   newcw->xlcore.ass_rotate=0;

      if((newcw->indic.indicText!=NULL) && (strlen(newcw->indic.indicText)>(MAXLUNTESTO-1)))
                {
		XlWarning("XlIndicTelep","SetVAlues",
		    "Text too long !");
                DrawAgoValoreDirectly(new,1,False);
                }
          

   if(curcw->indic.ago_fg != newcw->indic.ago_fg)
   {
      XtReleaseGC(current,curcw->indic.ago_gc);
      GetAgoGC(newcw);

      if( curcw->xlcore.config == True )
      {
         DrawIntoDirectly(new,False);
         DrawAgoValoreDirectly(new,1,False);
      }

      do_redisplay = True;
   }

   if(curcw->core.background_pixel != newcw->core.background_pixel)
   {
/*
printf("CAMBIA IL BKG\n");
*/
        XtReleaseGC(current,curcw->indic.norm_gc);
        XtReleaseGC(current,curcw->indic.clear_gc);
        XtReleaseGC(current,curcw->indic.ago_gc);
        XtReleaseGC(current,curcw->indic.bordino_gc);
        XtReleaseGC(current,curcw->indic.cambio_colore[0].varOn_gc);
        XtReleaseGC(current,curcw->indic.cambio_colore[0].varOff_gc);
        XtReleaseGC(current,curcw->indic.colore_fault[0].varOn_gc);
        XtReleaseGC(current,curcw->indic.frame.frame_gc);
        XtReleaseGC(current,curcw->indic.frame.owFrame_gc);

        GetAllGCs(newcw,False);


        if( curcw->xlcore.config==True )
        {
           DrawIntoDirectly(new,False);
           DrawAgoValoreDirectly(new,1,False);
        }

        do_redisplay = True;
   }


   if(curcw->indic.tipo_ind != newcw->indic.tipo_ind)
   {
        if( curcw->xlcore.config==True )
        {
        DrawIntoDirectly(new,False);
        DrawAgoValoreDirectly(new,1,False);
        }

        do_redisplay = True;
   }

   if(curcw->indic.scalamento!=newcw->indic.scalamento)
   {
        if( curcw->xlcore.config==True )
        {
        DrawIntoDirectly(new,False);
        DrawAgoValoreDirectly(new,1,False);
        }
        do_redisplay = True;
   }
	
   if(curcw->indic.valore_minimo!=newcw->indic.valore_minimo)
   {
	if(newcw->indic.valore_minimo>=newcw->indic.valore_massimo)
	{
		XlWarning("XlIndicTelep","SetVAlues",
		    "il valore minimo deve essere minore del massimo");
		newcw->indic.valore_minimo=curcw->indic.valore_minimo;
	}
	else
	{
             if( curcw->xlcore.config==True )
             {
        	DrawIntoDirectly(new,False);
        	DrawAgoValoreDirectly(new,1,False);
             }
       	     do_redisplay = True;
        }
   }

   if(curcw->indic.valore_massimo!=newcw->indic.valore_massimo)
   {
	if(newcw->indic.valore_massimo<=newcw->indic.valore_minimo)
	{
		XlWarning("XlIndicTelep","SetVAlues",
		    "il valore massimo deve essere maggiore del minimo");
		newcw->indic.valore_massimo=curcw->indic.valore_massimo;
	}
	else
	{
             if( curcw->xlcore.config==True )
             {
        	DrawIntoDirectly(new,False);
        	DrawAgoValoreDirectly(new,1,False);
             }
             do_redisplay = True;
	}
   }

   if(curcw->indic.extra_range_sup!=newcw->indic.extra_range_sup)
   {
	if(newcw->indic.extra_range_sup<newcw->indic.valore_massimo)
        {
		XlWarning("XlIndicTelep","SetVAlues",
		    "il valore extra range sup. deve essere maggiore del massimo");
		newcw->indic.extra_range_sup=atof(EXTRA_RANGE_SUP);
	}
	else
	{
             if( curcw->xlcore.config==True )
             {
        	DrawIntoDirectly(newcw,False);
        	DrawAgoValoreDirectly(new,1,False);
             }
       	     do_redisplay = True;
	}
   }

   if(curcw->indic.offset!=newcw->indic.offset)
   {
        if( curcw->xlcore.config==True )
        {
        DrawIntoDirectly(new,False);
        DrawAgoValoreDirectly(new,1,False);
        }

        do_redisplay = True;
   }

   if(strcmp(curcw->indic.normalfont,newcw->indic.normalfont))
   {
	if(!LoadFont(new,newcw->indic.normalfont,False))
        if(!LoadFont(new,"fixed",False))
                XlError("XlIndicTelep","SetValues",
			"Impossibile caricare default font");
        if( curcw->xlcore.config==True )
        {
        DrawIntoDirectly(new,False);
        DrawAgoValoreDirectly(new,1,False);
        }
        do_redisplay = True;
   }

   if(curcw->indic.bordino_width!=newcw->indic.bordino_width)
   {
        if( curcw->xlcore.config==True )
        {
        DrawIntoDirectly(new,False);
        DrawAgoValoreDirectly(new,1,False);
        }
        do_redisplay = True;
   }
   /*
    risorse connesse con sistema di simulazione
   */

   /* input variable */
   if(strcmp(newcw->indic.cambio_colore[0].str_input,
                  curcw->indic.cambio_colore[0].str_input))
   {
        Compile(newcw);
        do_redisplay = False;
   }

   /* fault variable */
   if(strcmp(newcw->indic.colore_fault[0].str_input,
                  curcw->indic.colore_fault[0].str_input))
   {
        Compile(newcw);
        do_redisplay = False;
   }

   /* ack variable */
   if(strcmp(newcw->indic.colore_fault[1].str_input,
                  curcw->indic.colore_fault[1].str_input))
   {
        Compile(newcw);
        do_redisplay = False;
   }

   /* frame control variable */
   if(strcmp(newcw->indic.frame.str_input,
                  curcw->indic.frame.str_input))
   {
        Compile(newcw);
        do_redisplay = False;
   }

   return do_redisplay;
}

static void Destroy(w)
Widget w;
{
   float bfreq;
   XlIndicTelepWidget cw= (XlIndicTelepWidget) w;

   /* elimina, se necessario, il widget dalla lista di blink */
    if( cw->indic.blinking )
	XlRemoveBlink(w,0);

    if( cw->indic.owOpen )
	XlRemoveBlink(w,1);

   /* rilascio i GC */

   if(cw->indic.frame.frame_gc)
	XtReleaseGC(w,cw->indic.frame.frame_gc);

   if(cw->indic.frame.owFrame_gc)
	XtReleaseGC(w,cw->indic.frame.owFrame_gc);

   if(cw->indic.norm_gc)
	XtReleaseGC(w,cw->indic.norm_gc);

   if(cw->indic.clear_gc)
	XtReleaseGC(w,cw->indic.clear_gc);

   if(cw->indic.ago_gc)
	XtReleaseGC(w,cw->indic.ago_gc);

   if(cw->indic.bordino_gc)
	XtReleaseGC(w,cw->indic.bordino_gc);

   if(cw->indic.colore_fault[0].varOn_gc)
	XtReleaseGC(w,cw->indic.colore_fault[0].varOn_gc);

   if(cw->indic.cambio_colore[0].varOn_gc)
	XtReleaseGC(w,cw->indic.cambio_colore[0].varOn_gc);

   if(cw->indic.cambio_colore[0].varOff_gc)
	XtReleaseGC(w,cw->indic.cambio_colore[0].varOff_gc);

   /* libero la font_struct */
/*
   if(cw->indic.font_info)
	XFreeFont(XtDisplay(w),cw->indic.font_info);
*/
}

/* instaziazione dei GC legato al frame
*/
static void GetFrameGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
   XGCValues values;
   XlIndicTelepWidget cw= (XlIndicTelepWidget) w;

   unsigned long valuemask= GCForeground | GCBackground | GCLineWidth | GCLineStyle ;


   values.line_width = cw->indic.frame.frameWidth;
   values.line_style = LineSolid;

#if defined XPRINTER_USED && !defined LINUX
   if(modoPrint)
   {
      values.foreground = XlPrintGetPixel(w,cw->indic.frame.varFrameColor.colore);
      values.background = XlPrintGetPixel(w,cw->core.background_pixel);
      cw->indic.frame.frame_gc = XCreateGC(XlDisplay(w),XlWindow(w),valuemask, &values);

      values.foreground = XlPrintGetPixel(w,cw->indic.frame.owFrameColor.colore);
      cw->indic.frame.owFrame_gc = XCreateGC(XlDisplay(w),XlWindow(w),valuemask, &values);
   }
   else
   {
      values.foreground = cw->indic.frame.varFrameColor.colore;
      values.background = cw->core.background_pixel;
      cw->indic.frame.frame_gc = XtGetGC(w, valuemask, &values);

      values.foreground = cw->indic.frame.owFrameColor.colore;
      cw->indic.frame.owFrame_gc = XtGetGC(w, valuemask, &values);
   }
#else
   values.foreground = cw->indic.frame.varFrameColor.colore;
   values.background = cw->core.background_pixel;
   cw->indic.frame.frame_gc = XtGetGC(w, valuemask, &values);

   values.foreground = cw->indic.frame.owFrameColor.colore;
   cw->indic.frame.owFrame_gc = XtGetGC(w, valuemask, &values);
#endif
}

/* instaziazione GC legato alla risora NormalState foreground 
*/
static void GetNormFgGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
   XGCValues values;
   XlIndicTelepWidget cw= (XlIndicTelepWidget) w;

   unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;


   values.line_width = 1;
   values.line_style = LineSolid;
   values.font = cw->indic.font_info->fid;

#if defined XPRINTER_USED && !defined LINUX
   if(modoPrint)
   {
      values.foreground = XlPrintGetPixel(w,cw->indic.ago_fg);
      values.background = XlPrintGetPixel(w,cw->core.background_pixel);
      cw->indic.norm_gc = XCreateGC(XlDisplay(w),XlWindow(w),valuemask, &values);
   }
   else
   {
      values.foreground = cw->indic.ago_fg;
      values.background = cw->core.background_pixel;
      cw->indic.norm_gc = XtGetGC(cw, valuemask, &values);
   }
#else
   values.foreground = cw->indic.ago_fg;
   values.background = cw->core.background_pixel;
   cw->indic.norm_gc = XtGetGC(cw, valuemask, &values);
#endif
}

/* instaziazione GC legato alla risora NormalState foreground
*/
static void GetBordinoGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
   XGCValues values;
   XlIndicTelepWidget cw= (XlIndicTelepWidget) w;

   unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle ;


   values.line_width = cw->indic.bordino_width;
   values.line_style = LineSolid;

#if defined XPRINTER_USED && !defined LINUX
   if(modoPrint)
   {
      values.foreground = XlPrintGetPixel(w,cw->indic.bordino_fg);
      values.background = XlPrintGetPixel(w,cw->core.background_pixel);
      cw->indic.bordino_gc = XCreateGC(XlDisplay(w),XlWindow(w),valuemask, &values);
   }
   else
   {
      values.foreground = cw->indic.bordino_fg;
      values.background = cw->core.background_pixel;
      cw->indic.bordino_gc = XtGetGC(cw, valuemask, &values);
   }
#else
   values.foreground = cw->indic.bordino_fg;
   values.background = cw->core.background_pixel;
   cw->indic.bordino_gc = XtGetGC(cw, valuemask, &values);
#endif
}

/* instaziazione GC che setta il foreground uguale allo background
   cio' permette di ottenre l'effetto clear
*/
static void GetClearGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
   XGCValues values;
   unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;
   XlIndicTelepWidget cw= (XlIndicTelepWidget) w;

   values.line_width = cw->indic.frame.frameWidth;
   values.line_style = LineSolid;
   values.font = cw->indic.font_info->fid;

#if defined XPRINTER_USED && !defined LINUX
   if(modoPrint)
   {
      values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
      values.background = XlPrintGetPixel(w,cw->indic.cambio_colore[0].VarColorOn.colore);
      cw->indic.clear_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
   }
   else
   {
      values.foreground = cw->core.background_pixel;
      values.background = cw->indic.cambio_colore[0].VarColorOn.colore;
      cw->indic.clear_gc = XtGetGC(cw, valuemask, &values);
   }
#else
   values.foreground = cw->core.background_pixel;
   values.background = cw->indic.cambio_colore[0].VarColorOn.colore;
   cw->indic.clear_gc = XtGetGC(cw, valuemask, &values);
#endif
}

/* instaziazione GC legato alla risora Ago or bar color
   l'ago color viene settato con funzione Xor
   o in Xcopy se siamo in modoPrint
*/
static void GetAgoGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
   XGCValues values;
   Pixel b;
   unsigned long valuemask = GCForeground | GCBackground | GCLineWidth
                             | GCLineStyle | GCFunction | GCFont ;
   XlIndicTelepWidget cw= (XlIndicTelepWidget) w;
   Widget Padre;

   values.line_width = 0;
   values.line_style = LineSolid;
   values.font = cw->indic.font_info->fid;

/* NOTA
    per poter disegnare l'ago su un background
   con Xor function senza alterare 
   il colore desiderato faccio lo XOR
   tra il colore desiderato e il background 
   perche altrimenti otterrei il colore somma
   (Es. bkg = Blue Ago =Green senza XOR disegnerei l'ago di Giallo (Blue+Green)
   Questo e' il significato del simbolo: ^
*/

#if defined XPRINTER_USED && !defined LINUX
   if(modoPrint)
   {
      values.function=GXcopy;
      values.background = XlPrintGetPixel(w,cw->core.background_pixel);
      values.foreground = XlPrintGetPixel(w,cw->indic.ago_fg);
      cw->indic.ago_gc = XCreateGC(XlDisplay(w),XlWindow(w),
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

/* instaziazione GC che setta i colori fg e bg in base alla variabili di fault
   utilizzando questo GC si ottiene un disegno con il colore impostato
*/
static void GetFaultGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
   XGCValues values;
   unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle ;
   XlIndicTelepWidget cw= (XlIndicTelepWidget) w;

   values.line_width = 0;
   values.line_style = LineSolid;

#if defined XPRINTER_USED && !defined LINUX
   if(modoPrint)
   {
      values.foreground = XlPrintGetPixel(w,cw->indic.colore_fault[0].VarColorOn.colore);
      values.background = XlPrintGetPixel(w,cw->indic.colore_fault[0].VarColorOn.colore);
      cw->indic.colore_fault[0].varOn_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
   }
   else
   {
      values.foreground = cw->indic.colore_fault[0].VarColorOn.colore;
      values.background = cw->indic.colore_fault[0].VarColorOn.colore;
      cw->indic.colore_fault[0].varOn_gc = XtGetGC(cw, valuemask, &values);
   }
#else
   values.foreground = cw->indic.colore_fault[0].VarColorOn.colore;
   values.background = cw->indic.colore_fault[0].VarColorOn.colore;
   cw->indic.colore_fault[0].varOn_gc = XtGetGC(cw, valuemask, &values);
#endif
}

/* instaziazione GC che setta i colori fg e bg in base allo stato 0 della variabile
   utilizzando questo GC si ottiene un disegno con il colore impostato
*/
static void GetVarOffGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
   XGCValues values;
   unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;
   XlIndicTelepWidget cw= (XlIndicTelepWidget) w;

   values.line_width = 0;
   values.line_style = LineSolid;
   values.font = cw->indic.font_info->fid;

#if defined XPRINTER_USED && !defined LINUX
   if(modoPrint)
   {
      values.foreground = XlPrintGetPixel(w,cw->indic.cambio_colore[0].VarColorOff.colore);
      values.background = XlPrintGetPixel(w,cw->indic.cambio_colore[0].VarColorOff.colore);
      cw->indic.cambio_colore[0].varOff_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
   }
   else
   {
      values.foreground = cw->indic.cambio_colore[0].VarColorOff.colore;
      values.background = cw->indic.cambio_colore[0].VarColorOff.colore;
      cw->indic.cambio_colore[0].varOff_gc = XtGetGC(cw, valuemask, &values);
   }
#else
   values.foreground = cw->indic.cambio_colore[0].VarColorOff.colore;
   values.background = cw->indic.cambio_colore[0].VarColorOff.colore;
   cw->indic.cambio_colore[0].varOff_gc = XtGetGC(cw, valuemask, &values);
#endif
}

/* instaziazione GC che setta i colori fg e bg in base allo stato 1 della variabile
   utilizzando questo GC si ottiene un disegno con il colore impostato
*/
static void GetVarOnGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
   XGCValues values;
   unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;
   XlIndicTelepWidget cw= (XlIndicTelepWidget) w;

   values.line_width = 0;
   values.line_style = LineSolid;
   values.font = cw->indic.font_info->fid;

#if defined XPRINTER_USED && !defined LINUX
   if(modoPrint)
   {
      values.foreground = XlPrintGetPixel(w,cw->indic.cambio_colore[0].VarColorOn.colore);
      values.background = XlPrintGetPixel(w,cw->indic.cambio_colore[0].VarColorOn.colore);
      cw->indic.cambio_colore[0].varOn_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
   }
   else
   {
      values.foreground = cw->indic.cambio_colore[0].VarColorOn.colore;
      values.background = cw->indic.cambio_colore[0].VarColorOn.colore;
      cw->indic.cambio_colore[0].varOn_gc = XtGetGC(cw, valuemask, &values);
   }
#else
   values.foreground = cw->indic.cambio_colore[0].VarColorOn.colore;
   values.background = cw->indic.cambio_colore[0].VarColorOn.colore;
   cw->indic.cambio_colore[0].varOn_gc = XtGetGC(cw, valuemask, &values);
#endif
}

/* riutine di inizializzazione dei vari GC's
*/
static void GetAllGCs(w,modoPrint)
Widget w;
Boolean modoPrint;
{
   XlIndicTelepWidget cw= (XlIndicTelepWidget)w;

   GetFrameGC(w,modoPrint);
   GetNormFgGC(w,modoPrint);
   GetAgoGC(w,modoPrint);
   GetBordinoGC(w,modoPrint);
   GetClearGC(w,modoPrint);

   GetFaultGC(w,modoPrint);
   GetVarOnGC(w,modoPrint);
   GetVarOffGC(w,modoPrint);

}

static void DrawIntoDirectly(w,modoPrint)
Widget w;
Boolean modoPrint;
{
   XlIndicTelepWidget cw= (XlIndicTelepWidget)w;

   /* ripulisce lo sfondo non deve essere trasparent
   */
   if((cw->xlcore.trasparent!=True)&& !modoPrint)
	XlRiempiRettangolo(w,XlWindow(w),cw->indic.clear_gc,0,0,100,100);
}


/* 
  verifica se la variabile di fault e' a 1 o 0
  e ritorna True/False e la frequenza di blink
*/
static Boolean  XlFaultIsOn(CAMBIO_COLORE_VAR cambio_colore,float *blink_freq)
{
   if( !cambio_colore.input.valore )
   {
      *blink_freq = 0.0;
      return(False);
   }

   *blink_freq = cambio_colore.VarColorOn.blink_rate;
   return(True);
}
 
/* 
   verifica il flag RI della variabile
*/
Boolean XlRiIsOn(PUNT_VARINP *punt_var)
{
   /* se la variabile non e' stata configurata 
      l'ack deve essere ignorato:
      viene quindi ritornato sempre True
   */
   if(punt_var->pdb.indice == -1)
	return(True);

   /* punto digitale */
   if(OlIsDigital(punt_var->pdb.tipo,punt_var->pdb.origin))
   {
      if(punt_var->flag.flag_digitale.ri == 1)
             return(True);
      else
             return(False);
   }
   else /* punto analogico */
   {
      if(punt_var->flag.flag_analogico.ri == 1)
              return(True);
      else
              return(False);
   }
}

static Boolean Refresh(Widget w)
{
   XlIndicTelepWidget cw= (XlIndicTelepWidget)w;
   int core_width=cw->core.width;
   int core_height=cw->core.height;
   int core_x=cw->core.x;
   int core_y=cw->core.y;
   Boolean blinking,fault,ack;
   float blink_freq;

   /* in configurazione
   */
   if(cw->xlcore.config==True)
      return(True);

   /* verifica parametri 
   */
   if(!XtIsWidget(w) || (w == NULL))
   {
      XlWarning("XlIndicTelep","Refresh",
		"il parametro in ingresso non e' un Widget");
      return(False);
   }

   /* verifica dello stato del database
   */
   if(xlIndicTelepClassRec.xlcore_class.database==NULL)
   {
      XlWarning("XlIndicTelep","Refresh","database non definito");
      return(False);
   }


/* in caso di fault attivo gli indicatori devono blinkare 
   con il colore di fault configurato. Al momento che il
   fault viene riconosciuto dall'operatore l'indicatore
   smette di blinkare e riprende il suo colore normale
   (come se l'allarme non ci fosse piu')
   L'oggetto effettua il blink quindi quando il fault e' on
   e la variabile non e' stata ancora riconosciuta, cioe' flag Ri = 1)
*/

   /* verifico lo stato della variabile di fault
   */
   fault = XlFaultIsOn(cw->indic.colore_fault[0],&blink_freq);

   /* se il flag RI e' a 1 nessuno a riconosciuto la variabile
      faccio blinkare
   */
   ack = XlRiIsOn( &cw->indic.colore_fault[1].input ); 

   if( fault && ack )                /* il fault non e' riconosciuto */
      blinking = True;
   else if( fault && !ack ) /* il fault e' riconosciuto */
      blinking = False;
   else                     /* non c'e' fault */
      blinking = False;


   /* stabilisco che deve blinkare 
      se la freq di blink > 0
      se gia' sta blinkando non lo aggiungo
   */
   if( blinking  && (!cw->indic.blinking) && ( blink_freq > 0.0 ) )
   {
       XlAddBlink(w,0,blink_freq,(void*)BlinkProc);
       cw->indic.blinking = True;
   }

   /* se non deve blinkare ma il blink era stato attivato
      in precedenza lo rimuovo
   */ 
   if( !blinking && cw->indic.blinking)
   {
      cw->indic.blinking = False;
      XlRemoveBlink(w,0); 

      /* dopo avere rimosso il blink ridisegno subito gli oggetti
         chiamando la routine con il parametro di modo = 10 che effettua
         il ridisegno con modalita' blink=off. Il led fa eccezione
         perche' deve riassumere il colore della variabile di stato 
      */
      if( (cw->indic.tipo_ind == INDICATORE_LED_RECT)   || 
          (cw->indic.tipo_ind == INDICATORE_LED_CIRCLE) ||  
          (cw->indic.tipo_ind == INDICATORE_TEXT_FG_AN) ||
          (cw->indic.tipo_ind == INDICATORE_TEXT_BG_AN) ||
          (cw->indic.tipo_ind == INDICATORE_ISTOGRAMMA_DEC) ||
          (cw->indic.tipo_ind == INDICATORE_ISTOGRAMMA_CRES))
         DrawAgoValoreDirectly(w,1,False);
      else
         DrawAgoValoreDirectly(w,10,False);
   }

   if(!(*superclass->xlcore_class.refresh) (w))
   {
      XlWarning("XlIndicTelep","Refresh","errore refresh classe superiore");
      return(False);
   }


   /* gestione del frame per gli indicatori operabili
      eseguo questo pezzo di codece se
      l'indicatore e' operabile e 
      se la Ow risulta aperta oppure se il valore della variabile di controllo
      e' cambiato
   */

/*
   if( cw->indic.operable ) 
   {
*/
      /* verifico se la O/W e' aperta
      */
      if( cw->indic.owOpen )
      {
        if( cw->indic.frame.owFrameColor.blink_rate > 0.0 )
        {
           if( !cw->indic.frame.blink )
           {
             cw->indic.frame.blink = True;
             XlAddBlink(w,1,cw->indic.frame.owFrameColor.blink_rate,(void*)BlinkProc);
           }
        }
        else
           DrawFrame(w,0);
      }
      else if (cw->indic.frame.input.valore != cw->indic.old_frameval)
      {
         /* la O/W e' chiusa
           se la variabile del frame e' a 1 deve essere disegnato il frame
           altrimenti il frame non ci deve essere
         */
         if( cw->indic.frame.input.valore )
         {
            if( cw->indic.frame.varFrameColor.blink_rate > 0.0 )
            {
               if(!cw->indic.frame.blink)
               {
                  cw->indic.frame.blink = True;
                  XlAddBlink(w,1,cw->indic.frame.varFrameColor.blink_rate,
                               (void*)BlinkProc);
               }
            }
            else
               DrawFrame(w,0);
         }
         else /* non e' aperta non e' settata la variabile il frame non ci vuole */
         {
            /* se blinkava devo rimuovere il blink
            */
            if( cw->indic.frame.blink )
            {
               cw->indic.frame.blink = False;
               XlRemoveBlink(w,1);
            }
            /* qui devo fare scomparire il frame (se c'era) */
            DrawAgoValoreDirectly(w,1,False);
         }
      }

      /* aggiorno il valore precedente del frame
      */
      cw->indic.old_frameval =  cw->indic.frame.input.valore;
/*
   }
*/
   

   /* ridisegna l'oggetto se non e' blinkante o smette di blinkare
      NB il ridisegno viene effettuato solo se le variabili di controllo
         dell'oggetto hanno cambiato stato 
   */
   if( !cw->indic.blinking || (!blinking && cw->indic.blinking)  || cw->indic.primo_refresh )
      DrawAgoValoreDirectly(w,0,False);

   cw->indic.primo_refresh=0;
   
   return(True);
}


static void Resize(w)
Widget w;
{
   XlIndicTelepWidget cw= (XlIndicTelepWidget)w;
   int app_int;

   if( cw->xlcore.config == True )
   {
      DrawIntoDirectly(w,False);
      DrawAgoValoreDirectly(w,1,False);
   }
}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
   XlIndicTelepWidget cw= (XlIndicTelepWidget)w;
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

/* only fore debug
printf("Redisplay cw->xlcore.config=%d\n",cw->xlcore.config);
*/

   if( cw->xlcore.config==True)
   {
      DrawIntoDirectly(w,False);
      DrawAgoValoreDirectly(w,1,False);
   }
   else if( !((cw->indic.tipo_ind == INDICATORE_ISTOGRAMMA_DEC) ||
              (cw->indic.tipo_ind == INDICATORE_ISTOGRAMMA_CRES)) )
   {
      DrawIntoDirectly(w,False);
      DrawAgoValoreDirectly(w,1,False);
   }

/*
   if( cw->xlcore.config==True)
   {
      DrawIntoDirectly(w,False);
      DrawAgoValoreDirectly(w,1,False);
   }
*/

   /* Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
   */
   (*superclass->xlcore_class.drawBorderIfSelected) (w);

}

static int min(a,b)
int a,b;
{
   return((a<b)? a:b);
}

static Boolean LoadFont( w,fontname,modoPrint)
Widget w;
char fontname[];
Boolean modoPrint;
{
   XlIndicTelepWidget cw = (XlIndicTelepWidget) w;
   static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
   /* Carica il font ottenendo la descrizione del font stesso */

   char **pf;
   int i,nf;
   Display *disp;
   char msg[255];

   if(modoPrint)
	{
	disp=XlDisplay(w);
	if((cw->indic.font_info = XLoadQueryFont(disp,fontname)) == NULL)
        	{
        	XlWarning("XlIndicTelep","LoadFont","Cannot open font");
        	return(False);
        	}
	}
   else
	{
	disp=XtDisplay(w);
	if((cw->indic.font_info = XlGetFont(disp,fontname)) == NULL)
        	{
                sprintf(msg,"%s Cannot open font",fontname);
        	XlWarning("XlIndicTelep","LoadFont",msg);
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
   XlIndicTelepWidget cw= (XlIndicTelepWidget)w;
   float valore = cw->indic.cambio_colore[0].input.valore;
   float scalamento = cw->indic.scalamento;
   float offset = cw->indic.offset;
   float min = cw->indic.valore_minimo;
   float max = cw->indic.valore_massimo+cw->indic.extra_range_sup;
   int dx=10;
   int dy=10;
   unsigned int dw=10;
   unsigned int dh=10;
   float posizione;
   char testo[MAXLUNTESTO];
   XExposeEvent event;
   Dimension indic_w,indic_h,indic_b;
   Position indic_x,indic_y;
   Pixel pixel_set;
   GC gc_set,gc_bkg,gc_bord;
   Arg arg[2];
   Boolean modo_blink=False;
   Boolean On;
   int xtrans,ytrans;
   Window childw;
   Position offX,offY;


   offX=XlPrintOffsetX(w);
   offY=XlPrintOffsetY(w);

   if(!XtIsWidget(w))
   {
      XlWarning("XlIndicTelep","DrawAgoValoreDirectly",
                "il parametro in ingresso non e' un Widget");
      return;
   }


   /* per la gestione dell'eventuale stato di blink.
      10 corrisponde a modo blink, stato off
      11 corrisponde a modo blink, stato on 
      <10 corrisponde a modo non blink. 
	Se modo>=10->la funz. è stata invocata da BlinkProc o 
	da qualsiasi altra funz. associata ad un oggetto contenuto 
	nella lista degli oggetti-blink.
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

   if(valore>max)
      valore=max;
   else if(valore<min) 
      valore=min;

   if(min==max)
      valore=min;

   switch(cw->indic.tipo_ind)
   {
      case INDICATORE_LED_RECT:
      case INDICATORE_LED_CIRCLE:

         dx = offX+cw->indic.bordino_width/2;
         dy = offY+cw->indic.bordino_width/2;
         dw = cw->core.width - cw->indic.bordino_width;
         dh = cw->core.height - cw->indic.bordino_width;

         if( dx < 1)
            dx = 1;
         if( dy <1 );
            dy = 1;

         if( (dw < 1) || (dh < 1) )
           return;

         if((cw->indic.cambio_colore[0].input.valore != cw->indic.old_posizione) ||
             modo || (cw->indic.old_posizione == -1))
         {

            /* Se la variabile di stato del led e' a 1 o a 0 disegno il led con
               colori relativi ai singoli stati
               il bordo e' sempre quello impostato con il normal foreground (color attr)
               se c'e' un fault setto il gc relativo 
               N.B. Il blink e' riservato al fault (cioe' viene forzato l'uso 
                                                     del gc del fault)
            */

           gc_bord = cw->indic.bordino_gc;
           if( modo_blink && On )
           {
              gc_bkg  = cw->indic.colore_fault[0].varOn_gc;
              gc_bord = cw->indic.colore_fault[0].varOn_gc;
           }
           else if( modo_blink && !On )
           {
              gc_bkg = cw->indic.clear_gc;
              gc_bord = cw->indic.clear_gc;
           }
           else if( cw->indic.cambio_colore[0].input.valore == 1)
              gc_bkg = cw->indic.cambio_colore[0].varOn_gc;
           else if( cw->indic.cambio_colore[0].input.valore == 0)
              gc_bkg = cw->indic.cambio_colore[0].varOff_gc;
           else
              gc_bkg = cw->indic.cambio_colore[0].varOff_gc;
           

           /* disegno il led rettangolare o circolare con bordino
           */
           if( cw->indic.tipo_ind == INDICATORE_LED_RECT )
           {
              XFillRectangle(XlDisplay(w),XlWindow(w),gc_bkg,
                                   dx,dy,dw,dh);
              XDrawRectangle(XlDisplay(w),XlWindow(w),gc_bord,
                                   dx,dy,dw,dh);

           }
           else
           {
              XFillArc(XlDisplay(w),XlWindow(w),gc_bkg,
                     dx,dy,dw,dh,(int)0,(int)360*64);
              XDrawArc(XlDisplay(w),XlWindow(w),gc_bord,
                     dx,dy,dw,dh,(int)0,(int)360*64);
           }

           /* matengo il valore precedente
           */
           cw->indic.old_posizione = cw->indic.cambio_colore[0].input.valore;

        }

      break;
      case INDICATORE_ISTOGRAMMA_CRES:
      case INDICATORE_ISTOGRAMMA_DEC:

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
	          XlRiempiRettangolo(w,XlWindow(w),cw->indic.ago_gc,
		     dx,dy,(int)cw->indic.old_posizione-dx,100-2*dy);
               }
               cw->indic.old_posizione=posizione;
	       XlRiempiRettangolo(w,XlWindow(w),cw->indic.ago_gc,
		      dx,dy,(int)posizione-dx,100-2*dy);
            }
	 }
	 else /* run_time */
	 {

           /* calcolo le dimensioni del bar
           */
	   CalcIstog(w,&indic_x,&indic_y,&indic_w,&indic_h,posizione);



           /* se c'e' un fault modo_blink deve risultatre settato 
              quindi i colori possibili sono tra quello normale ago_fg 
              e quello di fault
           */
           if( modo_blink ) 
              pixel_set = cw->indic.colore_fault[0].VarColorOn.colore;
           else
              pixel_set = cw->indic.ago_fg;

           /* il blink lo fa tra il colore di blink e il bkg della pagina
              per cui forzo a 0 le dim del widget in modo
              che esso venga unmanaged
           */
           if( modo_blink && !On )
           {
              indic_w = 0;
              indic_h = 0;
           }

	   if(indic_w > 0 && indic_h>0)
	   {
              /* set del colore se e' cambiato
                 (cambiamento del modo_blink)
              */
      	      if(pixel_set != cw->indic.OldPixel)
              {
	   	cw->indic.OldPixel = pixel_set;
	   	XtSetArg(arg[0],XmNbackground,pixel_set);
	   	XtSetValues(cw,arg,1);
              }

	      if(!XtIsManaged(cw))
			XtManageChild(cw);
	      XtConfigureWidget(cw,indic_x,indic_y,	
			indic_w,indic_h,cw->core.border_width);
           }
	   else
		XtUnmanageChild(cw);


         }
      break;
      case INDICATORE_TEXT_FG_AN:
         /* nel display deve blinkare il background
            percio' disegno un rettangolo pieno con il gc di fill
         */


         /* le coordinate sono relative allo widget
            e non alla drawing area x e y sono coincidenti
            con lo spessore del frame
         */

         dx = offX+cw->indic.frame.frameWidth;
         dy = offY+cw->indic.frame.frameWidth;
         dw = cw->core.width-cw->indic.frame.frameWidth*2;
         dh = cw->core.height-cw->indic.frame.frameWidth*2;

         if((cw->indic.cambio_colore[0].input.valore != cw->indic.old_posizione) ||
             modo || (cw->indic.old_posizione== -1))
         {
            /* riempio il testo con la stringa 
               se stringa e' definita
            */
            if((cw->indic.indicText!=NULL) && ((strlen(cw->indic.indicText))<               (MAXLUNTESTO-1)))
               sprintf(testo,"%s", cw->indic.indicText);
            else 
               {
               strncpy(testo,cw->indic.indicText,MAXLUNTESTO-1);
               testo[MAXLUNTESTO-1]=0;
               }

            /* definisco con quale colore visualizzare la stringa
               se l'utente vuole la stringa non visibile deve 
               forzare il colore Off uguale a quello di sfondo della pagina
            */
            if( (cw->indic.cambio_colore[0].input.valore == 1) || cw->xlcore.config )
               gc_set = cw->indic.cambio_colore[0].varOn_gc;
            else
               gc_set = cw->indic.cambio_colore[0].varOff_gc;

            if((cw->xlcore.trasparent!=True) && !modoPrint)
            {
              /* se e' settata la variabile di fault
                 disegno lo sfondo secondo le specifiche
                 del colore animato
              */
              if( (modo_blink && On) )
                 gc_bkg = cw->indic.colore_fault[0].varOn_gc;
              else
                 gc_bkg = cw->indic.clear_gc;

              
              XFillRectangle(XlDisplay(w),XlWindow(w),
                                      gc_bkg,
                                      dx,dy,dw,dh);
            }
            else
            {
               if( (modo_blink && On) )
                  gc_bkg = cw->indic.colore_fault[0].varOn_gc;
               else
                  gc_bkg = cw->indic.clear_gc;


              XFillRectangle(XlDisplay(w),XlWindow(w),
                                      gc_bkg,
                                      dx,dy,dw,dh);
           }

           /* disegno il testo
           */

           XlTestoSimple(w,XlWindow(w),gc_set,0,50,
                               testo,cw->indic.font_info);

           /* uso questo campo per tenere
              il valore precedente
           */
	   cw->indic.old_posizione=valore;

        }
      break;
      case INDICATORE_TEXT_BG_AN:
         /* indicatore tipo text con animazione sul bkg in base al valore della
            variabile di input - la scritta resta fissa
         */
           
         dx = offX+cw->indic.frame.frameWidth;
         dy = offY+cw->indic.frame.frameWidth;
         dw = cw->core.width-cw->indic.frame.frameWidth*2;
         dh = cw->core.height-cw->indic.frame.frameWidth*2;
         if((cw->indic.cambio_colore[0].input.valore != cw->indic.old_posizione) ||
             modo || (cw->indic.old_posizione== -1))
         {
            /* 
            definisco il testo che deve essere scritto
            */
            if((cw->indic.indicText!=NULL) && ((strlen(cw->indic.indicText))<                              (MAXLUNTESTO-1)))
               sprintf(testo,"%s", cw->indic.indicText);
            else 
               {
               strncpy(testo,cw->indic.indicText,MAXLUNTESTO-1);
               testo[MAXLUNTESTO-1]=0;
	       }

            /* 
            setto il gc per la scritta
            */
            gc_set = cw->indic.norm_gc;

            /* se c'e' il fault attivo questa routine viene invocata
               con il modo_blink settao. Quindi definisco in questo caso
               il bkg di fault che ha priorita su quello relativo
               allo stato della variabile.
               se non c'e' fault
               setto il bkg in base allo stato della variabile
            */
            if( modo_blink && On )
               gc_bkg = cw->indic.colore_fault[0].varOn_gc;
            else if( modo_blink && !On )
               gc_bkg = cw->indic.clear_gc;
            else if( cw->indic.cambio_colore[0].input.valore == 1  )
               gc_bkg = cw->indic.cambio_colore[0].varOn_gc;
            else
               gc_bkg = cw->indic.cambio_colore[0].varOff_gc;
            


           /* disegno lo sfondo 
           */
           XFillRectangle(XlDisplay(w),XlWindow(w),
                                      gc_bkg,
                                      dx,dy,dw,dh);

           /* disegno il testo
           */

           XlTestoSimple(w,XlWindow(w),gc_set,0,50,
                               testo,cw->indic.font_info);

           /* uso questo campo per tenere
              il valore precedente
           */
           cw->indic.old_posizione=valore;

        }
      break;
      case INDICATORE_DISPLAY:

         /* nel display deve blinkare il background
            percio' disegno un rettangolo pieno con il gc di fill
         */


         /* le coordinate sono relative allo widget 
            e non alla drawing area x e y sono coincidenti
            con lo spessore del frame
         */

         dx = 0;
         dy = 0;
         dw = cw->core.width; 
         dh = cw->core.height;

         /* se la variabile di frame e' settata
            il rettangolo di sfondo deve essere piu' piccolo
            di una quantita' pari alla width del frame
         */
         if( cw->indic.frame.input.valore || cw->indic.owOpen )
         {
            dx = offX+cw->indic.frame.frameWidth;
            dy = offY+cw->indic.frame.frameWidth;
            dw -= cw->indic.frame.frameWidth*2;
            dh -= cw->indic.frame.frameWidth*2;

            
         }

         if((cw->indic.old_posizione != valore)||
            (cw->indic.old_posizione== -1)     || modo )
	 {


           if(cw->indic.numero_dec==0)
#if defined LINUX
                sprintf(testo,"%*d", cw->indic.numero_int,(int)valore);
#else
                sprintf(testo,"%*d", cw->indic.numero_int,(int)nintf(valore));
#endif
           else
	    	sprintf(testo,"%*.*f", cw->indic.numero_int+cw->indic.numero_dec+1,
			cw->indic.numero_dec, valore);

            gc_set = cw->indic.norm_gc;

            if((cw->xlcore.trasparent!=True) && !modoPrint)
            {
              /* se e' settata la variabile di fault
                 disegno lo sfondo secondo le specifiche 
                 del colore animato
              */
              if( (modo_blink && On) )
                 gc_bkg = cw->indic.colore_fault[0].varOn_gc;
              else
                 gc_bkg = cw->indic.clear_gc;


              XFillRectangle(XlDisplay(w),XlWindow(w),
                                      gc_bkg, 
                                      dx,dy,dw,dh);
	    }
	    else
	    {
	       if( (modo_blink && On) )
                  gc_bkg = cw->indic.colore_fault[0].varOn_gc;
               else
                  gc_bkg = cw->indic.clear_gc;


              XFillRectangle(XlDisplay(w),XlWindow(w),
                                      gc_bkg, 
                                      dx,dy,dw,dh);

           }

           /* disegno il testo
           */
           XlTestoSimpleCentre(w,XlWindow(w),gc_set,50,50,
	                       testo,cw->indic.font_info);

           /* setto il valore
           */
	   cw->indic.old_posizione=valore;

           /* gestisco il frame sull'expose
           */
           if( (cw->xlcore.config!=True) && cw->indic.frame.input.valore )
               DrawFrame(w,0);

	}
	break;
	
   }
}

static void DrawLabel(w)
Widget w;
{
   XlIndicTelepWidget cw= (XlIndicTelepWidget)w;
   char *c=cw->indic.indicText;
   int nchar=0;
   int i;
   int step;
   char app_t[10];
   int font_height;

   XlTestoSimple(w,XlWindow(w),cw->indic.norm_gc,
                        0,50,c, cw->indic.font_info);

}

static Boolean Compile(Widget w)
{
   Boolean errors=False;
   XlIndicTelepWidget cw= (XlIndicTelepWidget)w;

   if(!XtIsWidget(w))
   {
	XlWarning("XlIndicTelep","Compile",
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

   if(strlen(cw->indic.colore_fault[0].str_input)==0)
   {
	XlErrComp(w,"Compile","Fault variable not defined",NULL);
        errors=True;
   }

   if(strlen(cw->indic.colore_fault[1].str_input)==0)
   {
        XlErrComp(w,"Compile","Ack variable not defined",NULL);
        errors=True;
   }

/*
   if(strlen(cw->indic.frame.str_input)==0)
*/
   if(strlen(cw->xlcore.pagina_staz)==0)
   {
      cw->indic.operable = False;
/*
      XlErrComp(w,"Compile","OW  variable not defined. Indicator is inoperable",NULL);
*/
   }
   

   if(xlIndicTelepClassRec.xlcore_class.compilatore==NULL)
   {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
	return(False);
   }

   if(!(*superclass->xlcore_class.compile) (w,
        xlIndicTelepClassRec.xlcore_class.compilatore))
   {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
	return(False);
   }
   XlEvError(cw,errors);
   return(True);
}

/* routine di calcolo dimensioni di ingombro dello widget
*/
void CalcXYWH0(Widget wid,int *x, int *y, int *w,int *h)
{
   XlIndicTelepWidget cw= (XlIndicTelepWidget)wid;

   switch(cw->xlcore.rotate)
   {
      case ROTATE_0:
 	*x = cw->core.x;
	*y = cw->core.y;
	*w = cw->xlcore.width0;
 	*h = cw->core.height;
      break;
      case ROTATE_90:
	*x = cw->core.x;
 	*y = cw->core.y;
   	*w = cw->core.width;
	*h = cw->xlcore.width0;
      break;
      case ROTATE_180:
   	*x = cw->indic.starting_x;
	*y = cw->core.y;
	*w = cw->xlcore.width0;
  	*h = cw->core.height;
      break;
      case ROTATE_270:
	*w = cw->core.width;
   	*h = cw->xlcore.width0;
	*x = cw->core.x;
	*y = cw->indic.starting_y;
      break;
   }
}

void CalcIstog(Widget wid,Position *pos_x, Position *pos_y, 
		Dimension *w, Dimension *h,float pos)
{
   XlIndicTelepWidget cw= (XlIndicTelepWidget)wid;

   *pos_x = cw->core.x;
   *pos_y = cw->core.y;
   *w = cw->core.width;
   *h = cw->xlcore.height0*(pos/100.0);

   switch(cw->xlcore.rotate)
   {
      case ROTATE_0:
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
      case ROTATE_90:
	*pos_x = cw->core.x;
 	*pos_y = cw->core.y;
   	*w = cw->core.width;
	*h = cw->xlcore.width0*(pos/100.0);
      break;
      case ROTATE_180:
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
      case ROTATE_270:
	*w = cw->core.width;
   	*h = cw->xlcore.width0*(pos/100.0);
	*pos_x = cw->core.x;
	*pos_y = cw->indic.starting_y + cw->xlcore.width0 - *h;
      break;
   }
}

static void BlinkProc(Widget w,int ind,Boolean On)
{
   XlIndicTelepWidget cw= (XlIndicTelepWidget) w;
	
   if( ind != 1) 
      DrawAgoValoreDirectly(w,10+On,False);
   else
     DrawFrame(w,On);
}

/*
        metodo di stampa
*/
#if defined XPRINTER_USED && !defined LINUX
static Boolean Print(Widget w)
{
   XlIndicTelepWidget cw= (XlIndicTelepWidget) w;
   char *printerFont;

   /* richiamo il metodo di print di Core
   */
/*   if(!((cw->indic.tipo_ind==INDICATORE_ISTOGRAMMA_CRES)||
        (cw->indic.tipo_ind==INDICATORE_ISTOGRAMMA_DEC)))
*/
      (*superclass->xlcore_class.print) (w);


   /* richiamo il ridisegno
   */
   printerFont=XtNewString(XlPrinterGetFont(w,
		cw->indic.normalfont,cw->indic.font_info));

   /* libero il font */
/*
   if(cw->indic.font_info)
	XFreeFont(XtDisplay(w),cw->indic.font_info);
*/
   /* rilascio i  GC */
   if(cw->indic.norm_gc)
	XtReleaseGC(w,cw->indic.norm_gc);
   if(cw->indic.clear_gc)
	XtReleaseGC(w,cw->indic.clear_gc);
   if(cw->indic.ago_gc)
	XtReleaseGC(w,cw->indic.ago_gc);
   if(cw->indic.bordino_gc)
	XtReleaseGC(w,cw->indic.bordino_gc);
   if(cw->indic.colore_fault[0].varOn_gc)
	XtReleaseGC(w,cw->indic.colore_fault[0].varOn_gc);
   if(cw->indic.cambio_colore[0].varOn_gc)
	XtReleaseGC(w,cw->indic.cambio_colore[0].varOn_gc);
   if(cw->indic.cambio_colore[0].varOff_gc)
	XtReleaseGC(w,cw->indic.cambio_colore[0].varOff_gc);
   if(cw->indic.frame.frame_gc)
        XtReleaseGC(w,cw->indic.frame.frame_gc);
   if(cw->indic.frame.owFrame_gc)
        XtReleaseGC(w,cw->indic.frame.owFrame_gc);

   if(!LoadFont(w,printerFont,True))
	LoadFont(w,"fixed",True);

   XtFree(printerFont);

   GetAllGCs(w,True);


   /* redisplay dell'indic
   */
   DrawIntoDirectly(w,True);
   DrawAgoValoreDirectly(w,1,True);



   /* rilascio i GC  della stampante*/
   if(cw->indic.norm_gc)
        XFreeGC(XlDisplay(w),cw->indic.norm_gc);
   if(cw->indic.clear_gc)
        XFreeGC(XlDisplay(w),cw->indic.clear_gc);
   if(cw->indic.ago_gc)
        XFreeGC(XlDisplay(w),cw->indic.ago_gc);
   if(cw->indic.bordino_gc)
        XFreeGC(XlDisplay(w),cw->indic.bordino_gc);
   if(cw->indic.colore_fault[0].varOn_gc)
	XFreeGC(XlDisplay(w),cw->indic.colore_fault[0].varOn_gc);
   if(cw->indic.cambio_colore[0].varOn_gc)
	XFreeGC(XlDisplay(w),cw->indic.cambio_colore[0].varOn_gc);
   if(cw->indic.colore_fault[0].varOff_gc)
	XFreeGC(XlDisplay(w),cw->indic.cambio_colore[0].varOff_gc);
   if(cw->indic.frame.frame_gc)
	XFreeGC(XlDisplay(w),cw->indic.frame.frame_gc);
   if(cw->indic.frame.owFrame_gc)
	XFreeGC(XlDisplay(w),cw->indic.frame.owFrame_gc);

   /* libero il font della stampante */
   if(cw->indic.font_info)
	XFreeFont(XlDisplay(w),cw->indic.font_info);

   if(!LoadFont(w,cw->indic.normalfont,False))
	LoadFont(w,"fixed",False);

   GetAllGCs(w,False);


   return(True);
}
#endif

/*
 * Routine di disegno del frame sull'oggetto XlIndic
 * Il frame viene disegnato quando viene aperta la OperationWindow
 * associata all'oggetto o quando e' attiva la variabile di animazione
 * associata al frame.
*/

static void DrawFrame(Widget w,Boolean blink)
{
   XlIndicTelepWidget cw= (XlIndicTelepWidget) w;
   unsigned int width=cw->core.width;
   unsigned int height=cw->core.height;
   unsigned int wbord=cw->core.border_width;
   unsigned int x=cw->core.x;
   unsigned int y=cw->core.y;
   int frameWidth = cw->indic.frame.frameWidth;
   GC gc_set;

   /* non faccio niente se sono inconfigurazione
   */
   if(cw->xlcore.config==True)
        return;

   if( (width == 0) || (height == 0) )
      return;

   /* disegno il frame
      le cui coordinate x,y sono relative 
      al parent dello widget in modo da fare risultare
      il frame esterno allo widget
      spessore frame = configurabile
   */


  /*
     che ha priorita' minore rispetto alla owOpen
  */
  if( cw->indic.frame.input.valore  )
     gc_set = cw->indic.frame.frame_gc;

  if( cw->indic.owOpen )
     gc_set = cw->indic.frame.owFrame_gc;

  /* la blink proc invoca la funzione alternativamente
     con 1 o con 0
     per cui setto il colore ==  a quello di sfondo
  */
  if( blink == 1 )
     gc_set = cw->indic.clear_gc;

   /* disegno il rettangolo rappresentante il frame
      che deve essere contenuto tutto dentro lo widget per
      cui aggiusto x,y,width,heigth in base allo spessore del frame stesso
   */

/* versione con frame interno allo widget
   quella attualmente in uso
*/
   XDrawRectangle(XlDisplay(w),XlWindow(w),gc_set,
                     0+(frameWidth/2),0+(frameWidth/2),
                     width-frameWidth,height-frameWidth);

/***********************************************************************
   versione per disegnare il frame esterno allo widget 
   non attualmente in uso. 
   Non cancellata per eventuali ripensamenti

   if( frameWidth%2 )
   {
     x = x-(frameWidth/2)-wbord+1;
     y = y-(frameWidth/2)-wbord+1;
   }
   else
   {
     x = x-(frameWidth/2)-wbord;
     y = y-(frameWidth/2)-wbord;
   }

   width = width+frameWidth+wbord*2;
   height = height+frameWidth+wbord*2;
   
   XDrawRectangle(XlDisplay(w),XlWindow(XtParent(w)),gc_set,
                     x,y,
                     width,height);
****************************************/

}
