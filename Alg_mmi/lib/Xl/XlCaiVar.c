/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlCaiVar.c	5.2\t12/4/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlCaiVar.c - widget caiVar per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlCaiVarP.h>

#include "sdi_conf.h"  /* Per maschere gestione allarmi */
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */
#define DEFAULTWIDTH 11
#define DEFAULTHEIGHT 11

/* lista delle risorse  */
static XtResource resources[]= {
        {
        XlNcolorBord,
        XlCColorBord,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiVarRec,caiVar.color_bord),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorA_FG,
        XlCColorA_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiVarRec,caiVar.color_A_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorA_BG,
        XlCColorA_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiVarRec,caiVar.color_A_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorW_FG,
        XlCColorW_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiVarRec,caiVar.color_W_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorW_BG,
        XlCColorW_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiVarRec,caiVar.color_W_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorT_FG,
        XlCColorT_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiVarRec,caiVar.color_T_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorT_BG,
        XlCColorT_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiVarRec,caiVar.color_T_BG),
        XmRString,
        XtDefaultForeground
        },
	{
        XlNcolorS_FG,
        XlCColorS_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiVarRec,caiVar.color_S_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorS_BG,
        XlCColorS_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiVarRec,caiVar.color_S_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorM_FG,
        XlCColorM_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiVarRec,caiVar.color_M_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorM_BG,
        XlCColorM_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiVarRec,caiVar.color_M_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorF_FG,
        XlCColorF_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiVarRec,caiVar.color_F_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorF_BG,
        XlCColorF_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiVarRec,caiVar.color_F_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNfrequenza1,
        XlCFrequenza1,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlCaiVarRec,caiVar.frequenza1),
        XtRString,
        FREQUENZA1
        },
        {
        XlNfrequenza2,
        XlCFrequenza2,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlCaiVarRec,caiVar.frequenza2),
        XtRString,
        FREQUENZA2
        },
	{
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.normalfont),
        XmRImmediate,
        "fixed"
        },
	{
        XlNvarInput1,
        XlCVarInput1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[0].str_input),
        XmRImmediate,
        "  "
        },
	{
        XlNvarInput2,
        XlCVarInput2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[1].str_input),
        XmRImmediate,
        ""
        },
	{
        XlNvarInput3,
        XlCVarInput3,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[2].str_input),
        XmRImmediate,
        ""
        },
	{
        XlNvarInput4,
        XlCVarInput4,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[3].str_input),
        XmRImmediate,
        ""
        },
	{
        XlNvarInput5,
        XlCVarInput5,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[4].str_input),
        XmRImmediate,
        ""
        },
	{
        XlNvarInput6,
        XlCVarInput6,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[5].str_input),
        XmRImmediate,
        ""
        },
	{
        XlNvarInput7,
        XlCVarInput7,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[6].str_input),
        XmRImmediate,
        ""
        },
	{
        XlNvarInput8,
        XlCVarInput8,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[7].str_input),
        XmRImmediate,
        ""
        },
	{
        XlNvarInput9,
        XlCVarInput9,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[8].str_input),
        XmRImmediate,
        ""
        },
	{
        XlNvarInput10,
        XlCVarInput10,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[9].str_input),
        XmRImmediate,
        ""
        },
	{
        XlNvarString_A,
        XlCVarString_A,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.str_allarm_A),
        XmRImmediate,
        "A"
        },
	{
        XlNvarString_W,
        XlCVarString_W,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.str_allarm_W),
        XmRImmediate,
        "W"
        },
	{
        XlNvarString_T,
        XlCVarString_T,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.str_allarm_T),
        XmRImmediate,
        "T"
        },
	{
        XlNvarString_S,
        XlCVarString_S,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.str_allarm_S),
        XmRImmediate,
        "S"
        },
	{
        XlNvarString_M,
        XlCVarString_M,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.str_allarm_M),
        XmRImmediate,
        "M"
        },
	{
        XlNvarString_F,
        XlCVarString_F,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiVarRec,caiVar.str_allarm_F),
        XmRImmediate,
        "F"
        },
        };

/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
        {
        XlNcolorBord,
        XlCColorBord,
        XlDcolorBord,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiVarRec,caiVar.color_bord),
        sizeof(Pixel)
        },
        {
        XlNcolorA_FG,
        XlCColorA_FG,
        XlDcolorA_FG,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiVarRec,caiVar.color_A_FG),
        sizeof(Pixel)
        },
        {
        XlNcolorA_BG,
        XlCColorA_BG,
        XlDcolorA_BG,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiVarRec,caiVar.color_A_BG),
        sizeof(Pixel)
        },
        {
        XlNcolorW_FG,
        XlCColorW_FG,
        XlDcolorW_FG,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiVarRec,caiVar.color_W_FG),
        sizeof(Pixel)
        },
        {
        XlNcolorW_BG,
        XlCColorW_BG,
        XlDcolorW_BG,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiVarRec,caiVar.color_W_BG),
        sizeof(Pixel)
	},
        {
        XlNcolorT_FG,
        XlCColorT_FG,
        XlDcolorT_FG,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiVarRec,caiVar.color_T_FG),
        sizeof(Pixel)
        },
        {
        XlNcolorT_BG,
        XlCColorT_BG,
        XlDcolorT_BG,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiVarRec,caiVar.color_T_BG),
        sizeof(Pixel)
        },
        {
        XlNcolorS_FG,
        XlCColorS_FG,
        XlDcolorS_FG,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiVarRec,caiVar.color_S_FG),
        sizeof(Pixel)
        },
        {
        XlNcolorS_BG,
        XlCColorS_BG,
        XlDcolorS_BG,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiVarRec,caiVar.color_S_BG),
        sizeof(Pixel)
        },
        {
        XlNcolorM_FG,
        XlCColorM_FG,
        XlDcolorM_FG,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiVarRec,caiVar.color_M_FG),
        sizeof(Pixel)
        },
        {
        XlNcolorM_BG,
        XlCColorM_BG,
        XlDcolorM_BG,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiVarRec,caiVar.color_M_BG),
        sizeof(Pixel)
        },
        {
        XlNcolorF_FG,
        XlCColorF_FG,
        XlDcolorF_FG,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiVarRec,caiVar.color_F_FG),
        sizeof(Pixel)
        },
        {
        XlNcolorF_BG,
        XlCColorF_BG,
        XlDcolorF_BG,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiVarRec,caiVar.color_F_BG),
        sizeof(Pixel)
        },
        {
        XlNfrequenza1,
        XlCFrequenza1,
        XlDfrequenza1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlCaiVarRec,caiVar.frequenza1),
        sizeof(float)
        },
        {
        XlNfrequenza2,
        XlCFrequenza2,
        XlDfrequenza2,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlCaiVarRec,caiVar.frequenza2),
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
        XtOffsetOf(XlCaiVarRec,caiVar.normalfont),
        sizeof(XFontStruct*)
        },
	{
	XlNvarInput1,
        XlCVarInput1,
        XlDvarInput1,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[0].allarme),
        sizeof(PUNT_VARINP)
	},
	{
	XlNvarInput2,
        XlCVarInput2,
        XlDvarInput2,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[1].allarme),
        sizeof(PUNT_VARINP)
	},
	{
	XlNvarInput3,
        XlCVarInput3,
        XlDvarInput3,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[2].allarme),
        sizeof(PUNT_VARINP)
	},
	{
	XlNvarInput4,
        XlCVarInput4,
        XlDvarInput4,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[3].allarme),
        sizeof(PUNT_VARINP)
	},
	{
	XlNvarInput5,
        XlCVarInput5,
        XlDvarInput5,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[4].allarme),
        sizeof(PUNT_VARINP)
	},
	{
	XlNvarInput6,
        XlCVarInput6,
        XlDvarInput6,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[5].allarme),
        sizeof(PUNT_VARINP)
	},
	{
	XlNvarInput7,
        XlCVarInput7,
        XlDvarInput7,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[6].allarme),
        sizeof(PUNT_VARINP)
	},
	{
	XlNvarInput8,
        XlCVarInput8,
        XlDvarInput8,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[7].allarme),
        sizeof(PUNT_VARINP)
	},
	{
	XlNvarInput9,
        XlCVarInput9,
        XlDvarInput9,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[8].allarme),
        sizeof(PUNT_VARINP)
	},
	{
	XlNvarInput10,
        XlCVarInput10,
        XlDvarInput10,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlCaiVarRec,caiVar.input_all[9].allarme),
        sizeof(PUNT_VARINP)
	},
	};
/* dichiarazioni funzioni varie */
static Boolean assign_type();
static int Refresh_all();
static int calcola_blk_fore();
static int calcola_norm_fore(); 
static Boolean ConvToPixel ();
Boolean XlGetCaiVarState();
static void DrawDirectlyOff();
static void DrawRettangolo();
/* dichiarazione dei metodi (methods) */
static void Seleziona();
static void AddSelez();
static void Initialize();
static void Destroy();
static void Redisplay();
static void GetBordForGC();
static void GetNormCaiVarGC();
static void GetClearGC();
static void GetAllGCs();
static Boolean LoadFont();
static Boolean SetValues();
static Boolean Compile();
static Boolean Refresh();
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
XlCaiVarClassRec xlCaiVarClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "CaiVar",
    /* widget_size              */      sizeof(XlCaiVarRec),
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
    /* resize                   */      XtInheritResize,
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
  { /* caiVar fields */
    /* empty                    */      0
  }
};

WidgetClass xlCaiVarWidgetClass = (WidgetClass) &xlCaiVarClassRec;

static void DrawRettangolo(Widget cw,Drawable drawable,GC gc,int x1,int y1,int x2,int y2)
{
XlLinea(cw,drawable,gc,0,0,100,0);
XlLinea(cw,drawable,gc,0,0,0,100);
XlLinea(cw,drawable,gc,0,100,100,100);
XlLinea(cw,drawable,gc,100,0,100,100);
}
static Boolean ConvToPixel (Widget ogget,char *Stringa,Pixel *Pix)
{
        Colormap colmap;
        XColor xcol,hardw;


        colmap = DefaultColormap ( XtDisplay(ogget),
                        DefaultScreen (XtDisplay(ogget)) );
        if ( !XLookupColor (XtDisplay(ogget) , colmap , Stringa , &xcol,&hardw) )
           {
           printf("Errore XLookupColor: colore inesistente\n");
           return(False);
           }
        if(!XAllocColor(XtDisplay(ogget),colmap,&xcol))
           {
           printf("Errore XAllocColor: impossibile allocare un nuovo colore\n");
           return(False);
           }
        *Pix = xcol.pixel;
        return(True);
}

/*
Funzione chiamata da esterno 
che restituisce lo stato ed il tipo di allarme di uno widget
*/
Boolean XlGetCaiVarState(Widget w,char *tipo_all,int *stato)
{
XlCaiVarWidget cw= (XlCaiVarWidget) w;

if(cw==NULL)
	return(False);
if((cw->caiVar.tipo_all==ALLARM_CAIVAR) || (cw->caiVar.tipo_all==WARNING_CAIVAR) || (cw->caiVar.tipo_all==TOLERANCE_CAIVAR) || (cw->caiVar.tipo_all==MANUAL_CAIVAR) || 
	(cw->caiVar.tipo_all==FAULT_CAIVAR))
	{
	*tipo_all=cw->caiVar.tipo_all;
	*stato=cw->caiVar.alterna;
	return(True);
	}
else return(False);

}
/*
        calcola il valore di alterna, per il blink, in funzione
        della variabilr allarm_pend e allarm_norm
*/
static int calcola_norm_fore(w,valori)
Widget w;
XGCValues *valori;
{
Pixel pix;
char Stringa[MAXLUNCOLORNAME];
/*
Calcola foreground a seconda del tipo di allarme
N.B.:
	Per segnali(A,W,T,S) uso colori impostati; cablati per gli altri all.
*/
XlCaiVarWidget cw= (XlCaiVarWidget) w;
if(cw->caiVar.tipo_all==ALLARM_CAIVAR)
	{
	pix=cw->caiVar.color_A_FG;
	}
else if(cw->caiVar.tipo_all==WARNING_CAIVAR)
	{
	pix=cw->caiVar.color_W_FG;
	}
else if(cw->caiVar.tipo_all==TOLERANCE_CAIVAR)
        {
	pix=cw->caiVar.color_T_FG;
	}
        
else if(cw->caiVar.tipo_all==SIMUL_CAIVAR)
        {
	pix=cw->caiVar.color_S_FG;
        }
else if(cw->caiVar.tipo_all==MANUAL_CAIVAR)
        {
	pix=cw->caiVar.color_M_FG;
        }
else if(cw->caiVar.tipo_all==FAULT_CAIVAR)
        {
	pix=cw->caiVar.color_F_FG;
        }
else if(cw->caiVar.tipo_all==NULL_CAIVAR)
	{
	pix=cw->core.background_pixel;
	}
else
	return(0);	/*nessun tipo previsto->errore*/
valori->foreground=pix; /*setto il foreground*/
return(1);		/*tutto OK*/
}

static int calcola_blk_fore(w,valori)
Widget w;
XGCValues *valori;
{
Pixel pix;
char Stringa[MAXLUNCOLORNAME];
/*
Calcola background a seconda del tipo di allarme
N.B.:
	Per segnali(A,W,T,S) uso colori impostati; cablati per gli altri all.
*/
XlCaiVarWidget cw= (XlCaiVarWidget) w;
if(cw->caiVar.tipo_all==ALLARM_CAIVAR)
	{
	pix=cw->caiVar.color_A_BG;
	}
else if(cw->caiVar.tipo_all==WARNING_CAIVAR)
	{
	pix=cw->caiVar.color_W_BG;
	}
else if(cw->caiVar.tipo_all==TOLERANCE_CAIVAR)
        {
	pix=cw->caiVar.color_T_BG;
	}
        
else if(cw->caiVar.tipo_all==SIMUL_CAIVAR)
        {
	pix=cw->caiVar.color_S_BG;
        }
else if(cw->caiVar.tipo_all==MANUAL_CAIVAR)
        {
        pix=cw->caiVar.color_M_BG;
        }
else if(cw->caiVar.tipo_all==FAULT_CAIVAR)
        {
	pix=cw->caiVar.color_F_BG;
        }
else if(cw->caiVar.tipo_all==NULL_CAIVAR)
	{
	pix=cw->core.background_pixel;
	}
else
	return(0);	/*nessun tipo previsto->errore*/
valori->foreground=pix;
return(1);		/*tutto OK*/
}
/*
Funzione che determina il comportamento del CaiVar, in base alla situazione
delle variabili in input, aggiornata dal precedente refresh di Core.
Si va a verificare, dando priorita' alle var con indice + basso,
qual e' la situazione della var secondo la tabella:
	Valore	RI	STATE
	0	0	OFF
	1	1	BLINK1
	0	1	BLINK2
	1	0	ON 
Valore restituito: intero che identifica comportamento CAIVAR.
*/
static int Refresh_all(Widget w)
{
XlCaiVarWidget cw= (XlCaiVarWidget) w;
register int i;
Boolean found=False;
/*
Finche' non ho trovato una situazione significativa per gli allarmi
scorro tutte le variabili
*/
for(i=0;i<MAXNUMINPUT ;i++)
	{
	if((cw->caiVar.input_all[i].allarme.valore==1.0) &&
		(cw->caiVar.input_all[i].allarme.flag.flag_digitale.ri))
		{
		cw->caiVar.tipo_all=cw->caiVar.input_all[i].type;
		return(BLINK_ON_FQZ1);
		}
	else if((cw->caiVar.input_all[i].allarme.valore==1.0) &&
		!(cw->caiVar.input_all[i].allarme.flag.flag_digitale.ri))
		{
		cw->caiVar.tipo_all=cw->caiVar.input_all[i].type;
		return(1);    /* CaiVar acceso */
		}
	else if((cw->caiVar.input_all[i].allarme.valore==0.0) &&
                (cw->caiVar.input_all[i].allarme.flag.flag_digitale.ri))
                {
		cw->caiVar.tipo_all=cw->caiVar.input_all[i].type;
                return(BLINK_ON_FQZ2);    /* CaiVar acceso */
                }
	}
return(0);    /* CaiVar spento */
}

static void GetNormCaiVarGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
Pixel pix;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;

XlCaiVarWidget cw= (XlCaiVarWidget) w;

values.line_width = 1;
values.line_style = LineSolid;
values.font = cw->caiVar.font_info->fid;
#if defined XPRINTER_USED
if(modoPrint)
        {
	if(!calcola_norm_fore(cw,&values))
        	{
        	XlWarning("XlCaiVar","GetBlinkGC","Errore nel tipo di allarme!!!");
        	}
	values.foreground=XlPrintGetPixel(w,values.foreground);
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->caiVar.norm_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
	 if(!calcola_norm_fore(cw,&values))
        	{
        	XlWarning("XlCaiVar","GetBlinkGC","Errore nel tipo di allarme!!!");
        	}
        values.background = cw->core.background_pixel;
        cw->caiVar.norm_gc = XtGetGC(cw, valuemask, &values);
        }
#else
if(!calcola_norm_fore(cw,&values))
        {
        XlWarning("XlCaiVar","GetBlinkGC","Errore nel tipo di allarme!!!");
        }
values.background= cw->core.background_pixel;
cw->caiVar.norm_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetBlinkGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;
XlCaiVarWidget cw= (XlCaiVarWidget) w;
values.line_width = 1;
values.line_style = LineSolid;
values.font = cw->caiVar.font_info->fid;
#if defined XPRINTER_USED
if(modoPrint)
        {
	if(!calcola_blk_fore(cw,&values))
        {
        XlWarning("XlCaiVar","GetBlinkGC","Errore nel tipo di allarme!!!");
        }
	values.background= XlPrintGetPixel(w,values.background);
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->caiVar.blink_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
	if(!calcola_blk_fore(cw,&values))
        	{
        	XlWarning("XlCaiVar","GetBlinkGC","Errore nel tipo di allarme!!!");
        	}
        values.background = cw->core.background_pixel;
        cw->caiVar.blink_gc = XtGetGC(cw, valuemask, &values);
        }
#else 	/* no stampa*/

if(!calcola_blk_fore(cw,&values))
	{
	XlWarning("XlCaiVar","GetBlinkGC","Errore nel tipo di allarme!!!");
	}
values.background = cw->core.background_pixel;
cw->caiVar.blink_gc = XtGetGC(cw, valuemask, &values);
#endif
}
static void GetClearGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
Pixel parent_bkg;
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle /*| GCFont*/;
XlCaiVarWidget cw= (XlCaiVarWidget) w;
values.line_width = 0;
values.line_style = LineSolid;
/*values.font = cw->caiVar.font_info->fid;*/
/*
XtVaGetValues(XtParent(cw),XmNbackground,&parent_bkg,NULL);
*/
#if defined XPRINTER_USED
if(modoPrint)
        {
        values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->caiVar.clear_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
        values.foreground = cw->core.background_pixel;
        values.background = cw->core.background_pixel;
        cw->caiVar.clear_gc = XtGetGC(cw, valuemask, &values);
        }
#else
values.foreground = cw->core.background_pixel;
cw->caiVar.clear_gc = XtGetGC(cw, valuemask, &values);
#endif
}
/* 
Clear GC per modalita' di DEBUG in cui il Cai spento 
e' ben visibile
*/
#ifdef DEBUG_MODE
static void GetClearGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle /*| GCFont*/;
XlCaiVarWidget cw= (XlCaiVarWidget) w;
values.line_width = 0;
values.line_style = LineSolid;
/*values.font = cw->caiVar.font_info->fid;*/
#if defined XPRINTER_USED
if(modoPrint)
        {
        values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
        values.background = XlPrintGetPixel(w,cw->caiVar.color_bord);
        cw->caiVar.clear_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
        values.foreground = cw->core.background_pixel;
        values.background = cw->caiVar.color_bord;
        cw->caiVar.clear_gc = XtGetGC(cw, valuemask, &values);
        }
#else
values.foreground = cw->core.background_pixel;
cw->caiVar.clear_gc = XtGetGC(cw, valuemask, &values);
#endif
}
#endif

static void GetBordForGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
Pixel pix;
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;
XlCaiVarWidget cw= (XlCaiVarWidget) w;
values.line_width = 1;
values.line_style = LineSolid;
values.font = cw->caiVar.font_info->fid;
#if defined XPRINTER_USED
if(modoPrint)
        {
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        values.foreground = XlPrintGetPixel(w,cw->caiVar.color_bord);
        cw->caiVar.for_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
        values.background = cw->core.background_pixel;
        values.foreground = cw->caiVar.color_bord;
        cw->caiVar.for_gc = XtGetGC(cw, valuemask, &values);
        }
#else
values.foreground = cw->caiVar.color_bord;
values.background=cw->core.background_pixel;
cw->caiVar.for_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetAllGCs(w,modoPrint)
Widget w;
Boolean modoPrint;
{
/*
Il blink e' ottenuto riempiendo l' oggetto con i 2 GC norm_gc e blink_gc
alternativamente (la lettera e' settata al momento della config).
Il GC  clear GC e' usato per lo stato di CaiVar spento mentre il for_gc
e' usato per la lettera indicate il tipo_allarme.
*/
GetNormCaiVarGC(w,modoPrint);
GetClearGC(w,modoPrint);
GetBlinkGC(w,modoPrint);
GetBordForGC(w,modoPrint);
}

static void DrawDirectlyOff(w)
Widget w;
{
/*
Funzione che disegna vuoto il CaiVar
*/
XlCaiVarWidget cw= (XlCaiVarWidget)w;
Window cwin;
int width,height;
char testo[3];  /* identificatore del tipo di allarme  ( max 2 car )*/
width=cw->core.width;
height=cw->core.height;

cwin=XlWindow((Widget)cw);
/*
Rettangolo riempito con colore sfondo, colore lettera configurabile
*/
XlRiempiRettangolo((Widget)cw,cwin,cw->caiVar.clear_gc,0,0,100,100);
DrawRettangolo((Widget)cw,cwin,cw->caiVar.clear_gc,0,0,100,100);
#ifndef DEBUG_MODE
return;
#endif
DrawRettangolo((Widget)cw,cwin,cw->caiVar.for_gc,0,0,100,100);
switch(cw->caiVar.tipo_all)
	{
	case ALLARM_CAIVAR:
	strcpy(testo,cw->caiVar.str_allarm_A);
	break;
	case WARNING_CAIVAR:
	strcpy(testo,cw->caiVar.str_allarm_W);
	break;
	case TOLERANCE_CAIVAR:
	strcpy(testo,cw->caiVar.str_allarm_T);
	break;
	case SIMUL_CAIVAR:
	strcpy(testo,cw->caiVar.str_allarm_S);
	break;
	case MANUAL_CAIVAR:
	strcpy(testo,cw->caiVar.str_allarm_M);
	break;
	case FAULT_CAIVAR:
	strcpy(testo,cw->caiVar.str_allarm_F);
	break;
	case NULL_CAIVAR:
	sprintf(testo,"%c",cw->caiVar.tipo_all);
	break;
	default:
	sprintf(testo,"%c",cw->caiVar.tipo_all);
	break;
	}

XlTestoSimpleCentre((Widget)cw,cwin,cw->caiVar.for_gc,
		50,50,testo,cw->caiVar.font_info);
}
static void DrawDirectly(w,On)
Widget w;
Boolean On; /*Se On=True accendi CaiVar (blink_gc)*/
{
XlCaiVarWidget cw= (XlCaiVarWidget)w;
Window cwin;
int width,height;
char testo[3];	/* identificatore del tipo di allarme */
width=cw->core.width;
height=cw->core.height;

cwin=XlWindow((Widget)cw);
switch(cw->caiVar.tipo_all)
	{
	case ALLARM_CAIVAR:
	strcpy(testo,cw->caiVar.str_allarm_A);
	break;
	case WARNING_CAIVAR:
	strcpy(testo,cw->caiVar.str_allarm_W);
	break;
	case TOLERANCE_CAIVAR:
	strcpy(testo,cw->caiVar.str_allarm_T);
	break;
	case SIMUL_CAIVAR:
	strcpy(testo,cw->caiVar.str_allarm_S);
	break;
	case MANUAL_CAIVAR:
	strcpy(testo,cw->caiVar.str_allarm_M);
	break;
	case FAULT_CAIVAR:
	strcpy(testo,cw->caiVar.str_allarm_F);
	break;
	case NULL_CAIVAR:
	sprintf(testo,"%c",cw->caiVar.tipo_all);
	break;
	default:
	sprintf(testo,"%c",cw->caiVar.tipo_all);
	break;
	}

/*
Se On =False disegna il CaiVar col blink_gc else col norm_gc
*/
if(On)
	{
	XlRiempiRettangolo((Widget)cw,cwin,cw->caiVar.blink_gc,0,0,100,100);
	DrawRettangolo((Widget)cw,cwin,cw->caiVar.norm_gc,0,0,100,100);
	XlTestoSimpleCentre((Widget)cw,cwin,cw->caiVar.norm_gc,
               	50,50,testo,cw->caiVar.font_info);
	}
else
	{
		
	XlRiempiRettangolo((Widget)cw,cwin,cw->caiVar.norm_gc,0,0,100,100);
	DrawRettangolo((Widget)cw,cwin,cw->caiVar.blink_gc,0,0,100,100);
	XlTestoSimpleCentre((Widget)cw,cwin,cw->caiVar.blink_gc,
                       50,50,testo,cw->caiVar.font_info);
	}		
}
/*
Funzione che, in base al tipo di allarme, (lettera dopo carattere $ nel nome 
dell' input) assegna il tipo ad ogni input dell' oggetto.
Se tutto ok ritorna True.
*/
static Boolean assign_type(w)
Widget w;
{
XlCaiVarWidget cw= (XlCaiVarWidget)w;
char *pdollar;
int i;
for(i=0;i<MAXNUMINPUT;i++)
	{
	if(strlen(cw->caiVar.input_all[i].str_input)) /* Per variabili impostate */
		{
		pdollar=strstr(cw->caiVar.input_all[i].str_input,"$"); /* trovo $ */
		if(pdollar!=NULL)
        		{
			/* 
			assegno all' input il tipo di allarme identificato
			dalla lettera dopo $
			*/	
        		cw->caiVar.input_all[i].type=*(++pdollar);
        		}
		else
			{
			printf(" PROBLEM  found with Input %d :\n");
			return(False);
			}
		}
	}
return(True);
}

static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{

XlCaiVarWidget new = (XlCaiVarWidget)tnew;
new->caiVar.alterna=0;
new->caiVar.tipo_all_old=NULL_CAIVAR; /* Setto tipo Cai vecchio a type default*/
new->caiVar.tipo_all=NULL_CAIVAR; /* Setto tipo Cai  a type default*/

/*
Setto il tipo di allarme per config a W cosi' e' visibile durante configuraz.
*/

if(new->xlcore.config==True)
	new->caiVar.tipo_all=WARNING_CAIVAR;   
/*
Assegno i type (A,W,T,S) alle variabili di input
*/
if(!assign_type(new))
	{
	XlWarning("XlCaiVar","Initialize",
                "Tipo di allarme ingresso  incompatibile");
	}
if((new->caiVar.frequenza1)<=0)
	{
	new->caiVar.frequenza1=atof(FREQUENZA1);
	XlWarning("XlCaiVar","Initialize","frequenza1 deve essere >0");
	}
if((new->caiVar.frequenza2<=0))
	{
	new->caiVar.frequenza2=atof(FREQUENZA2);
	XlWarning("XlCaiVar","Initialize","frequenza2 deve essere >0");
	}	
if((new->caiVar.frequenza1)>4)
	{
	new->caiVar.frequenza1=atof(FREQUENZA1);
	XlWarning("XlCaiVar","Initialize","frequenza1 deve essere <4");
	}
if((new->caiVar.frequenza2>4))
	{
	new->caiVar.frequenza2=atof(FREQUENZA2);
	XlWarning("XlCaiVar","Initialize","frequenza2 deve essere <4");
	}	
if(!LoadFont(new,new->caiVar.normalfont,False))
        if(!LoadFont(new,"fixed",False))
                XlError("XlCaiVar","Initialize",
			"Impossibile caricare default font");
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
XlCaiVarWidget curcw= (XlCaiVarWidget) current;
XlCaiVarWidget newcw= (XlCaiVarWidget) new;
Boolean do_redisplay = False;
int i;
/*
Verifico che le variabili in input non siano cambiate
*/
for(i=0;i<MAXNUMINPUT;i++)
	{
	if(strcmp(newcw->caiVar.input_all[i].str_input,curcw->caiVar.input_all[i].str_input))
        	{
       		Compile(newcw);
        	do_redisplay = False;
        	}
	}

/*
Il colore della lettera, nel caso di CaiVar off e' dato dalla risorsa
color_bord
*/
if(curcw->caiVar.color_bord != newcw->caiVar.color_bord)
        {
        XtReleaseGC(curcw,curcw->caiVar.for_gc);
        XtReleaseGC(curcw,curcw->caiVar.clear_gc);
        GetBordForGC(newcw,0);
        GetClearGC(newcw,0);
	DrawDirectly(newcw,1);
        do_redisplay = True;
        }
if((curcw->caiVar.frequenza1) !=  newcw->caiVar.frequenza1)
	{
	do_redisplay = False;
	}
if((curcw->caiVar.frequenza2) !=  newcw->caiVar.frequenza2)
	{
	do_redisplay = False;
	}	
/*
 risorse connesse con sistema di simulazione
*/
if(strcmp(curcw->caiVar.normalfont,newcw->caiVar.normalfont))
        {
        if(!LoadFont(newcw,newcw->caiVar.normalfont,False))
        if(!LoadFont(newcw,"fixed",False))
                XlError("XlCaiVar","SetValues",
                        "Impossibile caricare default font");
	DrawDirectly(newcw,1);
        do_redisplay = True;
        }
return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlCaiVarWidget cw= (XlCaiVarWidget) w;

/*
Elimina il widget nella lista_blink
*/

XlRemoveBlink(cw,0);
/*
        Rilascio i GC
*/
if (cw->caiVar.norm_gc)
        XtReleaseGC(cw,cw->caiVar.norm_gc);
if (cw->caiVar.blink_gc)
        XtReleaseGC(cw,cw->caiVar.blink_gc);
if (cw->caiVar.clear_gc)
        XtReleaseGC(cw,cw->caiVar.clear_gc);
if (cw->caiVar.for_gc)
        XtReleaseGC(cw,cw->caiVar.for_gc);

/* libero la font_struct */
/*
if(cw->caiVar.font_info)
        XFreeFont(XtDisplay(cw),cw->caiVar.font_info);
*/
}

static Boolean Compile(Widget w)
{
Boolean errors=False;
XlCaiVarWidget cw= (XlCaiVarWidget)w;
int i;

if(!XtIsWidget(w))
        {
        XlWarning("XlCaiVar","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }
#ifdef DEBUG_MODE
/*
 Devono le variabili di input essere abilitate
*/
for(i=0;i<MAXNUMINPUT;i++)
	{
	if(!strlen(cw->caiVar.input_all[i].str_input))
		{
		fprintf(stderr,"WARNING:Input %d :\n",i);
		XlWarning("XlCaiVar","Compile",
			"Input  not defined");
		}
	}
#endif
if(cw->caiVar.frequenza1<0)
	{
	cw->caiVar.frequenza1=atof(FREQUENZA1);
	XlErrComp(w,"Compile","frequenza1 must be positive",NULL);
	errors=True;
	}
if(cw->caiVar.frequenza2<0)
	{
	cw->caiVar.frequenza2=atof(FREQUENZA2);
	XlErrComp(w,"Compile","frequenza2 must be positive",NULL);
	errors=True;
	}
if(cw->caiVar.frequenza1>4)
	{
	cw->caiVar.frequenza1=atof(FREQUENZA1);
	XlErrComp(w,"Compile","frequenza1 must be <4",NULL);
	errors=True;
	}
if(cw->caiVar.frequenza2>4)
	{
	cw->caiVar.frequenza2=atof(FREQUENZA2);
	XlErrComp(w,"Compile","frequenza2 must be <4",NULL);
	errors=True;
	}		
if(xlCaiVarClassRec.xlcore_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
        return(False);
        }

if(!(*superclass->xlcore_class.compile) (w,
     xlCaiVarClassRec.xlcore_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
        return(False);
        }
XlEvError(cw,errors);
return(True);
}

static void BlinkCaiVarProc(Widget w,int ind,Boolean On)
{
/*
Funzione chiamata da XlBlink ogni BLINK_RATE ms. che ridisegna il CaiVar
nello stato indicato da On
N.B.
ind non gestita
*/
DrawDirectly(w,On);

}

static Boolean Refresh(Widget w)
{
XlCaiVarWidget cw= (XlCaiVarWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;
float already_inserted;

if(cw->xlcore.config==True)
        return(True);
/*
Modifica per eventuale CaiVar sulla parte principale della pagina teleperm
Aspetto che teleperm sia stata realizzata prima di visualizzare i CaiVar.
*/
if(!XlWindow(cw))        
	return(True);
if(!XtIsWidget(w))
        {
        XlWarning("XlCaiVar","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlCaiVarClassRec.xlcore_class.database==NULL)
        {
        XlWarning("XlCaiVar","Refresh","database non definito");
        return(False);
        }
if(!(*superclass->xlcore_class.refresh) (w))
        {
        XlWarning("XlCaiVar","Refresh","errore refresh classe superiore");
        return(False);
        }

/*
Inizio modifica di Fabio
*/	

already_inserted=XlGetBlink((Widget)cw,0); /*cw già inserito in lista_blink ?*/


/* 
Funzione per detrminare comportamento del caiVar .Vengono lette le variabili
in input , e viene settato sia il tipo di allarme 
(A,W,T,S) sia la situazione (PEN e/o ARC).
Il risultato e' posto in cw->caiVar.alterna
*/

cw->caiVar.alterna=Refresh_all( (Widget)cw );
/*
Poiche' il tipo di allarme potrebbe essere cambiato, cambio il GC
Non uso XchangeGC e rilascio il GC con XtRelease perche' il GC era stato allocato con XtGetGC.
*/
if((cw->caiVar.tipo_all!=cw->caiVar.tipo_all_old))
	{
	XtReleaseGC(cw,cw->caiVar.norm_gc);
	XtReleaseGC(cw,cw->caiVar.blink_gc);
	XtReleaseGC(cw,cw->caiVar.clear_gc);
	GetNormCaiVarGC(cw,False);
	GetBlinkGC(cw,False);
	GetClearGC(cw,False);
	cw->caiVar.tipo_all_old=cw->caiVar.tipo_all;
	}
        switch(cw->caiVar.alterna)
        	{
/*
Se il risultato di Refresh_all è BLINK_ON :se oggetto già presente nella
lista degli oggetti che blinkano esci, altrimenti inseriscilo nella lista
*/                       
		case BLINK_ON_FQZ1:
		if (already_inserted==(cw->caiVar.frequenza1))
			{
		/*	printf("BLINK1-CAIVARalready_inserted INDEX=%ld\n",cw);*/
			break;
			}
		if(already_inserted==0)/*blink caiVar not found */	
			{
		/*	printf("BLINK1-CAIVAR NOT-already_inserted INDEX=%ld\n",cw);*/
			XlAddBlink(cw,0,cw->caiVar.frequenza1,(void*)BlinkCaiVarProc);
			break;
			}
		/*printf("BLINK1-CAIVAR INDEX=%ld\n",cw);*/
		XlRemoveBlink(cw,0);/*blink freq!=freq1*/
		XlAddBlink(cw,0,cw->caiVar.frequenza1,(void*)BlinkCaiVarProc);
		break;
		
		case BLINK_ON_FQZ2:
		if (already_inserted==(cw->caiVar.frequenza2))
			break;
		if(already_inserted==0)/*blink caiVar not found */	
			{
			XlAddBlink(cw,0,cw->caiVar.frequenza2,(void*)BlinkCaiVarProc);
			break;
			}			
		XlRemoveBlink(cw,0);
		XlAddBlink(cw,0,cw->caiVar.frequenza2,(void*)BlinkCaiVarProc);
		break;		
		
		case 1:
		if(already_inserted!=0)
			XlRemoveBlink(cw,0);
		DrawDirectly(cw,1);	/*Disegna in blink_gc*/
		break;			/*CaiVar acceso*/
		
		case 0:
		if(already_inserted!=0)
			XlRemoveBlink(cw,0);
		DrawDirectlyOff(cw);	/*Disegna in normal_gc*/
		break;			/*CaiVar spento*/
		}	
return(True);
}



static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlCaiVarWidget cw= (XlCaiVarWidget)w;
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
if(cw->xlcore.config==True)
	DrawDirectly(w,1); /*modifica di Fabio*/

/*
        Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
*/

(*superclass->xlcore_class.drawBorderIfSelected) (w);

}

/*
Funzione che carica il font
*/
static Boolean LoadFont( cw,fontname,modoPrint)
Widget cw;
char fontname[];
Boolean modoPrint;
{
XlCaiVarWidget Xl_cw = (XlCaiVarWidget) cw;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
/* Carica il font ottenendo la descrizione del font stesso */

char **pf;
int i,nf;
Display *disp;

if(modoPrint)
	{
        disp=XlDisplay(cw);
	if((Xl_cw->caiVar.font_info = XLoadQueryFont(disp,fontname)) == NULL)
        	{
        	XlWarning("XlCaiVar","LoadFont","Cannot open font");
        	return(False);
        	}
	}
else
	{
        disp=XtDisplay(cw);
	if((Xl_cw->caiVar.font_info = XlGetFont(disp,fontname)) == NULL)
        	{
        	XlWarning("XlCaiVar","LoadFont","Cannot open font");
        	return(False);
        	}
	}

return(True);
}

/*
        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
XlCaiVarWidget cw= (XlCaiVarWidget) w;
char *printerFont;

(*superclass->xlcore_class.print) (w);

printerFont=XtNewString(XlPrinterGetFont(w,
        cw->caiVar.normalfont,cw->caiVar.font_info));

/*
        Rilascio i GC
*/
if (cw->caiVar.norm_gc)
        XtReleaseGC(cw,cw->caiVar.norm_gc);
if (cw->caiVar.blink_gc)
        XtReleaseGC(cw,cw->caiVar.blink_gc);
if (cw->caiVar.clear_gc)
        XtReleaseGC(cw,cw->caiVar.clear_gc);
if (cw->caiVar.for_gc)
        XtReleaseGC(cw,cw->caiVar.for_gc);

if(!LoadFont(cw,printerFont,True))
        LoadFont(cw,"fixed",True);
XtFree(printerFont);

GetAllGCs(cw,True);
if(cw->caiVar.alterna == 0)
	DrawDirectlyOff(cw);
else
	DrawDirectly(cw,0); /*modifica di Fabio*/	

if(cw->caiVar.font_info)
        XFreeFont(XlDisplay(cw),cw->caiVar.font_info);/* rilascio il font della stampante */
/* rilascio i 3 GC della stampante */

if (cw->caiVar.norm_gc)
        XFreeGC(XlDisplay((Widget)cw),cw->caiVar.norm_gc);
if (cw->caiVar.blink_gc)
        XFreeGC(XlDisplay((Widget)cw),cw->caiVar.blink_gc);
if (cw->caiVar.clear_gc)
        XFreeGC(XlDisplay((Widget)cw),cw->caiVar.clear_gc);
if (cw->caiVar.for_gc)
        XFreeGC(XlDisplay((Widget)cw),cw->caiVar.for_gc);

if(!LoadFont(cw,cw->caiVar.normalfont,False))
        LoadFont(cw,"fixed",False);

GetAllGCs(cw,False);

return(True);
}
#endif
