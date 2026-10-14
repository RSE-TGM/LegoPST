/**********************************************************************
*
*       C Source:               XlAllarmi.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:37:33 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlAllarmi.c-4.1.5 %  (%full_filespec: XlAllarmi.c-4.1.5:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlAllarmi.c	5.2\t1/5/96";
/*
        Fine sezione per SCCS
*/
/*
 *  XlAllarmi.c - widget allarmi per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlManagerP.h>
#include <Xl/XlAllarmiP.h>
#include <Xl/XlDrawingUtil.h>
#include <Ol/OlDatabasePuntiP.h>
#include <Cs/Cs.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */
/* in XlAllarmi_func.c; qui perche' ha un Boolean per valore (vedi XlProto.h) */
int setta_colori_led(S_XLALLARMI *PuntAllarmi, Boolean On);

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
        XtOffsetOf(XlAllarmiRec,allarmi.norm_fg),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNtroughScrollBar,
        XlCTroughScrollBar,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlAllarmiRec,allarmi.troughScrollBar),
        XmRString,
        XtDefaultForeground
        },
	{
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlAllarmiRec,allarmi.normalfont),
        XmRImmediate,
        "fixed"
        },
	{
        XlNtitoloFont,
        XlCTitoloFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlAllarmiRec,allarmi.titolofont),
        XmRImmediate,
        "fixed"
        },
        {
        XlNdatabaseAllarmi,
        XlCDatabaseAllarmi,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.database_allarmi),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona0,
        XlCZona0,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[0]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona1,
        XlCZona1,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[1]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona2,
        XlCZona2,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[2]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona3,
        XlCZona3,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[3]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona4,
        XlCZona4,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[4]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona5,
        XlCZona5,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[5]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona6,
        XlCZona6,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[6]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona7,
        XlCZona7,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[7]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona8,
        XlCZona8,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[8]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona9,
        XlCZona9,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[9]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona10,
        XlCZona10,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[10]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona11,
        XlCZona11,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[11]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona12,
        XlCZona12,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[12]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona13,
        XlCZona13,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[13]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona14,
        XlCZona14,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[14]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona15,
        XlCZona15,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[15]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona16,
        XlCZona16,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[16]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona17,
        XlCZona17,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[17]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona18,
        XlCZona18,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[18]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona19,
        XlCZona19,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[18]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona20,
        XlCZona20,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[20]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona21,
        XlCZona21,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[21]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona22,
        XlCZona22,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[22]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona23,
        XlCZona23,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[23]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona24,
        XlCZona24,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[24]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona25,
        XlCZona25,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[25]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona26,
        XlCZona26,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[26]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona27,
        XlCZona27,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[27]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona28,
        XlCZona28,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[28]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona29,
        XlCZona29,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[29]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona30,
        XlCZona30,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[30]),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNzona31,
        XlCZona31,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlAllarmiRec,allarmi.zone[31]),
        XmRImmediate,
        (XtPointer)0
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
        XtOffsetOf(XlAllarmiRec,allarmi.norm_fg),
        sizeof(Pixel),
        },
        {
        XlNtroughScrollBar,
        XlCTroughScrollBar,
        XlDtroughScrollBar,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlAllarmiRec,allarmi.troughScrollBar),
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
        XtOffsetOf(XlAllarmiRec,allarmi.normalfont),
        sizeof(XFontStruct*)
        },
        {
        XlNtitoloFont,
        XlCTitoloFont,
        XlDtitoloFont,
        XlRFont,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.titolofont),
        sizeof(XFontStruct*)
        },
        {
        XlNdatabaseAllarmi,
        XlCDatabaseAllarmi,
        XlDdatabaseAllarmi,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.database_allarmi),
        sizeof(int)
        },
        {
        XlNzona0,
        XlCZona0,
        XlDzona0,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[0]),
        sizeof(int)
        },
        {
        XlNzona1,
        XlCZona1,
        XlDzona1,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[1]),
        sizeof(int)
        },
        {
        XlNzona2,
        XlCZona2,
        XlDzona2,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[2]),
        sizeof(int)
        },
        {
        XlNzona3,
        XlCZona3,
        XlDzona3,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[3]),
        sizeof(int)
        },
        {
        XlNzona4,
        XlCZona4,
        XlDzona4,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[4]),
        sizeof(int)
        },
        {
        XlNzona5,
        XlCZona5,
        XlDzona5,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[5]),
        sizeof(int)
        },
        {
        XlNzona6,
        XlCZona6,
        XlDzona6,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[6]),
        sizeof(int)
        },
        {
        XlNzona7,
        XlCZona7,
        XlDzona7,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[7]),
        sizeof(int)
        },
        {
        XlNzona8,
        XlCZona8,
        XlDzona8,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[8]),
        sizeof(int)
        },
        {
        XlNzona9,
        XlCZona9,
        XlDzona9,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[9]),
        sizeof(int)
        },
        {
        XlNzona10,
        XlCZona10,
        XlDzona10,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[10]),
        sizeof(int)
        },
        {
        XlNzona11,
        XlCZona11,
        XlDzona11,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[11]),
        sizeof(int)
        },
        {
        XlNzona12,
        XlCZona12,
        XlDzona12,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[12]),
        sizeof(int)
        },
        {
        XlNzona13,
        XlCZona13,
        XlDzona13,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[13]),
        sizeof(int)
        },
        {
        XlNzona14,
        XlCZona14,
        XlDzona14,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[14]),
        sizeof(int)
        },
        {
        XlNzona15,
        XlCZona15,
        XlDzona15,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[15]),
        sizeof(int)
        },
        {
        XlNzona16,
        XlCZona16,
        XlDzona16,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[16]),
        sizeof(int)
        },
        {
        XlNzona17,
        XlCZona17,
        XlDzona17,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[17]),
        sizeof(int)
        },
        {
        XlNzona18,
        XlCZona18,
        XlDzona18,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[18]),
        sizeof(int)
        },
        {
        XlNzona19,
        XlCZona19,
        XlDzona19,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[19]),
        sizeof(int)
        },
        {
        XlNzona20,
        XlCZona20,
        XlDzona20,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[20]),
        sizeof(int)
        },
        {
        XlNzona21,
        XlCZona21,
        XlDzona21,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[21]),
        sizeof(int)
        },
        {
        XlNzona22,
        XlCZona22,
        XlDzona22,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[22]),
        sizeof(int)
        },
        {
        XlNzona23,
        XlCZona23,
        XlDzona23,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[23]),
        sizeof(int)
        },
        {
        XlNzona24,
        XlCZona24,
        XlDzona24,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[24]),
        sizeof(int)
        },
        {
        XlNzona25,
        XlCZona25,
        XlDzona25,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[25]),
        sizeof(int)
        },
        {
        XlNzona26,
        XlCZona26,
        XlDzona26,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[26]),
        sizeof(int)
        },
        {
        XlNzona27,
        XlCZona27,
        XlDzona27,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[27]),
        sizeof(int)
        },
        {
        XlNzona28,
        XlCZona28,
        XlDzona28,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[28]),
        sizeof(int)
        },
        {
        XlNzona29,
        XlCZona29,
        XlDzona29,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[29]),
        sizeof(int)
        },
        {
        XlNzona30,
        XlCZona30,
        XlDzona30,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[30]),
        sizeof(int)
        },
        {
        XlNzona31,
        XlCZona31,
        XlDzona31,
        XlROption,
        NULL,
        XlOzona0_31,
        XlRGrIo,
        XtOffsetOf(XlAllarmiRec,allarmi.zone[31]),
        sizeof(int)
        },
	};
/* dichiarazioni funzioni varie */
static void BlinkProc(Widget, int, Boolean);
static void GetNormFgGC();
static void GetClearGC();
static void GetAllGCs();
static Boolean LoadFont();
static Boolean LoadFontTitolo();
static void SettaActionsFigli();
static void add_translation();
static void CalcolaDimensioniOggetto();
         
/* dichiarazione dei metodi (methods) */
static void Seleziona();
static void AddSelez();
static void Initialize();
static Boolean Refresh();
#if defined XPRINTER_USED
static Boolean Print();
#endif
static void Redisplay();
static void Resize();
static void Destroy();
static Boolean Compile();
static Boolean SetValues();
static XtGeometryResult GeometryManager();


/* translations  */
static char defaultTranslations[]=
        "~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static char configTranslations[]="#replace\n\
        ~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static XtActionsRec actions[] = {
        {"Seleziona",Seleziona},
        {"AddSelez",AddSelez}
        };

/* Inizializzazione del class record */
XlAllarmiClassRec xlAllarmiClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlManagerClassRec,
    /* class_name               */      "Allarmi",
    /* widget_size              */      sizeof(XlAllarmiRec),
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
    /* query_geometry           */      XtInheritQueryGeometry,
    /* display_accelerator      */      XtInheritDisplayAccelerator,
    /* extension                */      NULL
  },
  { /* Constraint class fields   */
    /* geometry_manager          */     GeometryManager, 
					/*XtInheritGeometryManager, */
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
  { /* bitmap fields */
    /* empty                    */      0
  }
};

WidgetClass xlAllarmiWidgetClass = (WidgetClass) &xlAllarmiClassRec;


/*  Dichiarazione del puntatore alla struttura delle variabili comuni
        all'oggetto allarmi  */
S_XLALLARMI *pXlAllarmi;


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlAllarmiWidget new = (XlAllarmiWidget)tnew;
int app_int;
Widget ret;

/*  Allocazione struttura per le variabili di uso comune a tutte
        le routine per l'eliminazione delle variabili globali  */
pXlAllarmi = (S_XLALLARMI *)calloc ( 1, sizeof(S_XLALLARMI) );
new->allarmi.p=pXlAllarmi;

/*
	setto il background e il foregroundper la pagina allarmi
*/
background_allarmi=new->core.background_pixel;
foreground_allarmi=new->allarmi.norm_fg;
trough_allarmi=new->allarmi.troughScrollBar;

if(new->core.width<DEFAULTWIDTH)
	{
        new->core.width=DEFAULTWIDTH;
	XlWarning("XlAllarmi","Initialize",
	     "width minore della dimensione minima");
	}
if(new->core.height<DEFAULTHEIGHT)
	{
        new->core.width=DEFAULTWIDTH;
	XlWarning("XlAllarmi","Initialize",
		"height minore della dimensione minima");
	}

if(!LoadFont(new,new->allarmi.normalfont))
	if(!LoadFont(new,"fixed"))
		XlError("XlAllarmi","Initialize","Impossibile caricare default font");

if(!LoadFontTitolo(new,new->allarmi.titolofont))
	if(!LoadFontTitolo(new,"fixed"))
		XlError("XlAllarmi","Initialize","Impossibile caricare default font");
/*
	assegna i font
*/
font_normale=new->allarmi.font_list;
font_titolo=new->allarmi.font_list_titolo;
/*
	assegna le zone
*/
memcpy(&(zone_impianto[0]),&(new->allarmi.zone[0]),NUMERO_ZONE*sizeof(int));

/*
	azzera il campo old_allarmi
*/
memset(&(new->allarmi.old_allarmi),0,sizeof(DATI_ALLARMI_SHM));
new->allarmi.old_allarmi.num_all_da_ric= -1;
new->allarmi.old_allarmi.num_all_arc= -1;
new->allarmi.old_allarmi.allarmi_da= -1;
new->allarmi.old_allarmi.allarmi_a= -1;
all_da_ric=0;
all_in_arch=0;
all_da=0;
all_a=0;

/*
	assegna il database allarmi
*/
db_allarmi=new->allarmi.database_allarmi;

/*
	assegna il database dei punti
*/
databasePunti=xlAllarmiClassRec.xlmanager_class.database;

/*
	azzero la struttura interna dei dati degli allarmi
*/
memset(dati_allarmi,0,sizeof(DATI_ALLARMI)*NUMERO_ALLARMI_VIS);

/*
	assegna le dimensioni
*/
CalcolaDimensioniOggetto(new);
/*
	Carica il Blink
*/
XlAddBlink(new,0,FREQUENZA_BASE,(void*)BlinkProc);

GetAllGCs(new);

ret=(Widget)create_top_allarmi(new,new->allarmi.p);
if(ret!=NULL)
	XtManageChild(ret);
SettaActionsFigli(new);
}

#define superclass (&xlManagerClassRec)

static void Seleziona(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
Widget wmanager=w;


while(!XlIsXlManager(wmanager))
	wmanager=XtParent(wmanager);
/*
 Richiama la routine di select di XlManager per la gestione della
 selezione
*/
(*superclass->xlmanager_class.select) (wmanager,0);
}

static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
Widget wmanager=w;


while(!XlIsXlManager(wmanager))
	wmanager=XtParent(wmanager);

/* richiama il metodo di select */
(*superclass->xlmanager_class.select)(wmanager,1);
}



static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
XlAllarmiWidget curcw= (XlAllarmiWidget) current;
XlAllarmiWidget newcw= (XlAllarmiWidget) new;
Boolean do_redisplay = False;
Arg comp_args[2];
Arg arg[2];
int allarmi_w,allarmi_h;
S_XLALLARMI *pXlAllarmi;

pXlAllarmi= curcw->allarmi.p;


if(curcw->allarmi.norm_fg != newcw->allarmi.norm_fg)
        {
        XtReleaseGC(curcw,curcw->allarmi.norm_gc);
        XtReleaseGC(curcw,curcw->allarmi.clear_gc);
        GetNormFgGC(newcw);
        GetClearGC(newcw);
        do_redisplay = True;
        }

if(strcmp(curcw->allarmi.normalfont,newcw->allarmi.normalfont))
        {
        if(!LoadFont(newcw,newcw->allarmi.normalfont))
        if(!LoadFont(newcw,"fixed"))
                XlError("XlAllarmi","SetValues",
                        "Impossibile caricare default font");
        do_redisplay = True;
        }
if(curcw->xlmanager.width0!=newcw->xlmanager.width0 ||
        curcw->xlmanager.height0!=newcw->xlmanager.height0)
        {
	XtSetArg(arg[0],XmNwidth,newcw->xlmanager.width0);
	XtSetArg(arg[0],XmNheight,newcw->xlmanager.height0);
	XtSetValues(arg,form_allarmi,2);
	do_redisplay = True;
	}

return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlAllarmiWidget cw= (XlAllarmiWidget) w;
S_XLALLARMI *pXlAllarmi;

pXlAllarmi= cw->allarmi.p;

/*
	invia il messaggio di chiusura della pagina allarmi
*/
if(cw->xlmanager.config==False)
   {
   if(!(olDatabasePuntiClassRec.oldatabasePunti_class.commandAllarmi)
           (xlAllarmiClassRec.xlmanager_class.database,OL_CHIUDI_ALLARMI))
	XlWarning("XlAllarmi","Destroy", "chiusura pagina allarmi");
   }
/*
Resetto il flag di sync. fra pagAll di tipo teleperm e quella
di tipo Stazione
*/
if(cw->xlmanager.config==False)
   OlSetOpeningAllarmPage(xlAllarmiClassRec.xlmanager_class.database,0);

/*
	Elimina il timer del blink
*/
XlRemoveBlink(cw,0);

/* rilascio i 2 GC */
if(cw->allarmi.norm_gc)
	XtReleaseGC(cw,cw->allarmi.norm_gc);

if(cw->allarmi.clear_gc)
	XtReleaseGC(cw,cw->allarmi.clear_gc);


/* libero la font_struct */
/*
if(cw->allarmi.font_info)
	XFreeFont(XtDisplay(cw),cw->allarmi.font_info);
if(cw->allarmi.font_info_titolo)
	XFreeFont(XtDisplay(cw),cw->allarmi.font_info_titolo);
*/

/* libero la font_list  */
if(cw->allarmi.font_list)
	XmFontListFree(cw->allarmi.font_list);
if(cw->allarmi.font_list_titolo)
	XmFontListFree(cw->allarmi.font_list_titolo);

}

static void GetNormFgGC(w)
Widget w;
{
XGCValues values;
XlAllarmiWidget cw= (XlAllarmiWidget) w;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;
S_XLALLARMI *pXlAllarmi;

pXlAllarmi= cw->allarmi.p;

values.foreground = cw->allarmi.norm_fg;
values.background = cw->core.background_pixel;
values.line_width = 1;
values.line_style = LineSolid;
values.font = cw->allarmi.font_info->fid;
cw->allarmi.norm_gc = XtGetGC(cw, valuemask, &values);
}

static void GetClearGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlAllarmiWidget cw= (XlAllarmiWidget) w;
S_XLALLARMI *pXlAllarmi;

pXlAllarmi= cw->allarmi.p;


values.foreground = cw->core.background_pixel;
values.background = cw->allarmi.norm_fg;
values.line_width = 0;
values.line_style = LineSolid;
cw->allarmi.clear_gc = XtGetGC(cw, valuemask, &values);
}


static void GetAllGCs(w)
Widget w;
{
GetNormFgGC(w);
GetClearGC(w);
}


static Boolean Refresh(Widget w)
{
XlAllarmiWidget cw= (XlAllarmiWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;
int core_x=cw->core.x;
int core_y=cw->core.y;
S_XLALLARMI *pXlAllarmi;
Boolean ret;
DATI_ALLARMI_SHM app_allarmi;
int i;

pXlAllarmi= cw->allarmi.p;
/*
	assegna il database dei punti
*/
databasePunti=xlAllarmiClassRec.xlmanager_class.database;
if(!XtIsWidget(w))
	{
	XlWarning("XlAllarmi","Refresh",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}

ret=(olDatabasePuntiClassRec.oldatabasePunti_class.getDataAllarmi)
                (xlAllarmiClassRec.xlmanager_class.database,&app_allarmi);

/*
	riempe la struttura interna dei dati degli allarmi
*/
if(ret)
 {
 if(memcmp(&app_allarmi,&(cw->allarmi.old_allarmi),sizeof(DATI_ALLARMI_SHM))!=0)
	{
	memcpy(&(cw->allarmi.old_allarmi),&app_allarmi,
		sizeof(DATI_ALLARMI_SHM));
	for(i=0;i<NUMERO_ALLARMI_VIS;i++)
		{
                strcpy(dati_allarmi[i].testo,app_allarmi.descr[i]);
                strcpy(dati_allarmi[i].data,app_allarmi.data[i]);
                if(app_allarmi.descr[i]!=0)
                        sprintf(dati_allarmi[i].testo,"%s %s",
                                app_allarmi.data[i], app_allarmi.descr[i]);
                else
                        sprintf(dati_allarmi[i].testo,"        ");
                dati_allarmi[i].colore_testo=app_allarmi.col_str[i];
                dati_allarmi[i].colore_astr=app_allarmi.col_ast[i];


                /*
                Momentaneo
                */
                dati_allarmi[i].esiste_valore=0;


#ifdef ALLDEBUG
                dati_allarmi[i].esiste_valore=app_allarmi.esiste_val[i];
#endif


                dati_allarmi[i].valore=app_allarmi.val[i];
		}
	all_da_ric=app_allarmi.num_all_da_ric;
	all_in_arch=app_allarmi.num_all_arc;
	all_da=app_allarmi.allarmi_da;
	all_a=app_allarmi.allarmi_a;
	raccogli_dati_allarmi(cw->allarmi.p);
	}
  }
return(True);
}


static void Resize(w)
Widget w;
{
XlAllarmiWidget cw= (XlAllarmiWidget)w;
S_XLALLARMI *pXlAllarmi;

pXlAllarmi= cw->allarmi.p;

}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlAllarmiWidget cw= (XlAllarmiWidget)w;
register int x,y;
unsigned int width,height;
S_XLALLARMI *pXlAllarmi;

pXlAllarmi= cw->allarmi.p;


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
        Richiamo il metodo di Redisplay di XlManager per la gestione della
        trasparenza e del contorno di selezione dell'oggetto
*/
(*superclass->core_class.expose) (w,event,NULL);


/*
        Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
*/
(*superclass->xlmanager_class.drawBorderIfSelected) (w);

}


static Boolean LoadFont( cw,fontname)
Widget cw;
char fontname[];
{
XlAllarmiWidget Xl_cw = (XlAllarmiWidget) cw;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
char **pf;
int i,nf;
S_XLALLARMI *pXlAllarmi;

pXlAllarmi= Xl_cw->allarmi.p;

/* Carica il font ottenendo la descrizione del font stesso */
if((Xl_cw->allarmi.font_info = XlGetFont(XtDisplay(cw),fontname)) == NULL)
        {
	XlWarning("XlAllarmi","LoadFont","Cannot open font");
	return(False);
        }
        else
        {
        Xl_cw->allarmi.font_list = 
		XmFontListCreate ( Xl_cw->allarmi.font_info, charset);
        if (Xl_cw->allarmi.font_list == NULL) 
		{
		XlWarning("XlAllarmi","LoadFont","FONTLIST NULL");
		return(False);
		}
        }
/*
	calcola le dimensioni del font
*/
h_font_normale=Xl_cw->allarmi.font_info->max_bounds.ascent+
		Xl_cw->allarmi.font_info->max_bounds.descent;
w_font_normale=Xl_cw->allarmi.font_info->max_bounds.width;
return(True);
}


static Boolean LoadFontTitolo( cw,fontname)
Widget cw;
char fontname[];
{
XlAllarmiWidget Xl_cw = (XlAllarmiWidget) cw;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
char **pf;
int i,nf;
S_XLALLARMI *pXlAllarmi;

pXlAllarmi= Xl_cw->allarmi.p;

/* Carica il font ottenendo la descrizione del font stesso */
if((Xl_cw->allarmi.font_info_titolo = XlGetFont(XtDisplay(cw),fontname)) == NULL)
        {
	XlWarning("XlAllarmi","LoadFont","Cannot open font");
	return(False);
        }
        else
        {
        Xl_cw->allarmi.font_list_titolo = 
		XmFontListCreate ( Xl_cw->allarmi.font_info_titolo, charset);
        if (Xl_cw->allarmi.font_list_titolo == NULL) 
		{
		XlWarning("XlAllarmi","LoadFont","FONTLIST NULL");
		return(False);
		}
        }
/*
	calcola le dimensioni del font
*/
h_font_titolo=Xl_cw->allarmi.font_info_titolo->max_bounds.ascent+
		Xl_cw->allarmi.font_info_titolo->max_bounds.descent;
w_font_titolo=Xl_cw->allarmi.font_info_titolo->max_bounds.width;
return(True);
}

static Boolean Compile(Widget w)
{
XlAllarmiWidget cw= (XlAllarmiWidget)w;
S_XLALLARMI *pXlAllarmi;
int i;
Boolean errors=False;

pXlAllarmi= cw->allarmi.p;

if(!XtIsWidget(w))
	{
	XlWarning("XlAllarmi","Compile",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}
if(xlAllarmiClassRec.xlmanager_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
        return(False);
        }

if(!(*superclass->xlmanager_class.compile) (w,
   xlAllarmiClassRec.xlmanager_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
        return(False);
        }

/*
	il database allarmi deve essere compreso tra 1 e
	NUMERO_DATABASE_ALLARMI(XlAllarmi.h)
*/
if(cw->allarmi.database_allarmi<1)
	{
	XlErrComp(w,"Compile","Il database allarmi deve essere > 0",NULL);
        errors=True;
	XlEvError(cw,errors);
	return(False);
	}
if(cw->allarmi.database_allarmi>NUMERO_DATABASE_ALLARMI)
	{
	XlErrComp(w,"Compile","Superato il num. max di database allarmi",NULL);
        errors=True;
	XlEvError(cw,errors);
	return(False);
	}
for(i=0;i<NUMERO_ZONE;i++)
	if(cw->allarmi.zone[i]==1)
		break;
if(i==NUMERO_ZONE)
	{
	XlErrComp(w,"Compile","Nessuna zona di impianto definita",NULL);
        errors=True;
	XlEvError(cw,errors);
	return(False);
	}

XlEvError(cw,errors);
return(True);
}
	


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
                 set_something_val(wid, XmNtranslations, (XtArgVal)tr);
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
S_XLALLARMI *pXlAllarmi;
XlAllarmiWidget new = (XlAllarmiWidget)tnew;

pXlAllarmi = new->allarmi.p;


if(new->xlmanager.config==True)
  {
  add_translation(top_allarmi,configTranslations);
  add_translation(form_allarmi,configTranslations);
  add_translation(form_tasti,configTranslations);
  add_translation(form_tasto_riconosci,configTranslations);
  add_translation(tasto_riconosci,configTranslations);
  add_translation(separatorGadget1,configTranslations);
  add_translation(label_da,configTranslations);
  add_translation(label_a,configTranslations);
  add_translation(form_altri_tasti,configTranslations);
  add_translation(tasto_archivio,configTranslations);
  add_translation(tasto_cong,configTranslations);
  add_translation(tasto_sing,configTranslations);
  add_translation(separatorGadget2,configTranslations);
  add_translation(form_label,configTranslations);
  add_translation(label_titolo,configTranslations);
  add_translation(label1,configTranslations);
  add_translation(label2,configTranslations);
  add_translation(form_testo,configTranslations);
  add_translation(scrollBar_allarmi,configTranslations);
  add_translation(separatorGadget3,configTranslations);
  add_translation(scrolledWindow_allarmi,configTranslations);
  add_translation(form_testi_allarmi,configTranslations);
  add_translation(label_titoli,configTranslations);
  add_translation(testo_1,configTranslations);
  add_translation(testo_2,configTranslations);
  add_translation(testo_3,configTranslations);
  add_translation(testo_4,configTranslations);
  add_translation(testo_5,configTranslations);
  add_translation(testo_6,configTranslations);
  add_translation(testo_7,configTranslations);
  add_translation(testo_8,configTranslations);
  add_translation(testo_9,configTranslations);
  add_translation(testo_10,configTranslations);
  add_translation(testo_11,configTranslations);
  add_translation(testo_12,configTranslations);
  add_translation(testo_13,configTranslations);
  add_translation(testo_14,configTranslations);
  add_translation(testo_15,configTranslations);
  add_translation(testo_16,configTranslations);
  add_translation(separatorGadget4,configTranslations);
  add_translation(valore_1,configTranslations);
  add_translation(valore_2,configTranslations);
  add_translation(valore_3,configTranslations);
  add_translation(valore_4,configTranslations);
  add_translation(valore_5,configTranslations);
  add_translation(valore_6,configTranslations);
  add_translation(valore_7,configTranslations);
  add_translation(valore_8,configTranslations);
  add_translation(valore_9,configTranslations);
  add_translation(valore_10,configTranslations);
  add_translation(valore_11,configTranslations);
  add_translation(valore_12,configTranslations);
  add_translation(valore_13,configTranslations);
  add_translation(valore_14,configTranslations);
  add_translation(valore_15,configTranslations);
  add_translation(valore_16,configTranslations);
  add_translation(toggle_1,configTranslations);
  add_translation(toggle_2,configTranslations);
  add_translation(toggle_3,configTranslations);
  add_translation(toggle_4,configTranslations);
  add_translation(toggle_5,configTranslations);
  add_translation(toggle_6,configTranslations);
  add_translation(toggle_7,configTranslations);
  add_translation(toggle_8,configTranslations);
  add_translation(toggle_9,configTranslations);
  add_translation(toggle_10,configTranslations);
  add_translation(toggle_11,configTranslations);
  add_translation(toggle_12,configTranslations);
  add_translation(toggle_13,configTranslations);
  add_translation(toggle_14,configTranslations);
  add_translation(toggle_15,configTranslations);
  add_translation(toggle_16,configTranslations);
  add_translation(led_1,configTranslations);
  add_translation(led_2,configTranslations);
  add_translation(led_3,configTranslations);
  add_translation(led_4,configTranslations);
  add_translation(led_5,configTranslations);
  add_translation(led_6,configTranslations);
  add_translation(led_7,configTranslations);
  add_translation(led_8,configTranslations);
  add_translation(led_9,configTranslations);
  add_translation(led_10,configTranslations);
  add_translation(led_11,configTranslations);
  add_translation(led_12,configTranslations);
  add_translation(led_13,configTranslations);
  add_translation(led_14,configTranslations);
  add_translation(led_15,configTranslations);
  add_translation(led_16,configTranslations);
  }
}

static void CalcolaDimensioniOggetto(Widget w)
{
S_XLALLARMI *pXlAllarmi;
XlAllarmiWidget cw = (XlAllarmiWidget)w;
Dimension width,height;

pXlAllarmi= cw->allarmi.p;


label_height=h_font_normale+4;
if(label_height<20)
	label_height=20;


/*width=50+LUN_TESTO_ALLARMI*w_font_normale;*/
width=cw->core.width;
height=HEIGHT_FORM_TASTI+16*(label_height+5)+50+26/*scrolled*/;

form_height=height;
form_width=width;
w_form_ric=13*w_font_normale;
w_form_arch=13*w_font_normale;

cw->core.height=form_height+2*OFFSET_FORM_ESTERNA;
cw->core.width=form_width+2*OFFSET_FORM_ESTERNA;
cw->xlmanager.height0=form_height+2*OFFSET_FORM_ESTERNA;
cw->xlmanager.width0=form_width+2*OFFSET_FORM_ESTERNA;
}

/*
 XlIsAllarmi(w) funzione per sapere se l'oggetto w
 e' del tipo XlAllarmi
*/
Boolean XlIsAllarmi(Widget w)
{
CoreWidgetClass wclass;
wclass=XtClass(w);
if(strcmp(wclass->core_class.class_name,"Allarmi")==0)
        return(True);
else
        return(False);
}

static void BlinkProc(Widget w,int ind,Boolean On)
{
XlAllarmiWidget cw= (XlAllarmiWidget) w;

                setta_colori_led(cw->allarmi.p,On);
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
