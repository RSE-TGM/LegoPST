/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlCaiAll.c	5.2\t12/4/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlCaiAll.c - widget caiAll per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlCaiAllP.h>

#include "sdi_conf.h"  /* Per maschere gestione allarmi */
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */
#include "fontfallback.h"
#define DEFAULTWIDTH 11
#define DEFAULTHEIGHT 11

/* lista delle risorse  */
static XtResource resources[]= {
        {
        XlNtipoCaiAll,
        XlCTipoCaiAll,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCaiAllRec,caiAll.tipo_caiAll),
        XmRImmediate,
        (XtPointer)CAIALL_W
        },
        {
        XlNcolorBord,
        XlCColorBord,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiAllRec,caiAll.color_bord),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorA_FG,
        XlCColorA_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiAllRec,caiAll.color_A_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorA_BG,
        XlCColorA_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiAllRec,caiAll.color_A_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorW_FG,
        XlCColorW_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiAllRec,caiAll.color_W_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorW_BG,
        XlCColorW_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiAllRec,caiAll.color_W_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorT_FG,
        XlCColorT_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiAllRec,caiAll.color_T_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorT_BG,
        XlCColorT_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiAllRec,caiAll.color_T_BG),
        XmRString,
        XtDefaultForeground
        },
	{
        XlNcolorS_FG,
        XlCColorS_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiAllRec,caiAll.color_S_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorS_BG,
        XlCColorS_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiAllRec,caiAll.color_S_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorM_FG,
        XlCColorM_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiAllRec,caiAll.color_M_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorM_BG,
        XlCColorM_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiAllRec,caiAll.color_M_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorF_FG,
        XlCColorF_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiAllRec,caiAll.color_F_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorF_BG,
        XlCColorF_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiAllRec,caiAll.color_F_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNfrequenza1,
        XlCFrequenza1,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlCaiAllRec,caiAll.frequenza1),
        XtRString,
        FREQUENZA1
        },
        {
        XlNfrequenza2,
        XlCFrequenza2,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlCaiAllRec,caiAll.frequenza2),
        XtRString,
        FREQUENZA2
        },
	{
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiAllRec,caiAll.normalfont),
        XmRImmediate,
        "fixed"
        },
	{
        XlNvarGerarchia,
        XlCVarGerarchia,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiAllRec,caiAll.str_allarm),
        XmRImmediate,
        "[-1][-1][-1][-1][-1][-1]"
        },
	{
        XlNvarString_A,
        XlCVarString_A,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiAllRec,caiAll.str_allarm_A),
        XmRImmediate,
        "A"
        },
	{
        XlNvarString_W,
        XlCVarString_W,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiAllRec,caiAll.str_allarm_W),
        XmRImmediate,
        "W"
        },
	{
        XlNvarString_T,
        XlCVarString_T,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiAllRec,caiAll.str_allarm_T),
        XmRImmediate,
        "T"
        },
	{
        XlNvarString_S,
        XlCVarString_S,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiAllRec,caiAll.str_allarm_S),
        XmRImmediate,
        "S"
        },
	{
        XlNvarString_M,
        XlCVarString_M,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiAllRec,caiAll.str_allarm_M),
        XmRImmediate,
        "M"
        },
	{
        XlNvarString_F,
        XlCVarString_F,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiAllRec,caiAll.str_allarm_F),
        XmRImmediate,
        "F"
        },
	{
        XlNstatus,
        XlCStatus,
        XmRInt,
        sizeof(int ),
        XtOffsetOf(XlCaiAllRec,caiAll.status),
        XmRImmediate,
        (XtPointer)SPENTO 
        },
        };

/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
      /* {
        XlNtipoCaiAll,
        XlCTipoCaiAll,
        XlDtipoCaiAll,
        XlROption,
        NULL,
        XlOtipoCaiAll,
        XlRGrIo,
        XtOffsetOf(XlCaiAllRec,caiAll.tipo_caiAll),
        sizeof(int)
        },*/
        {
        XlNcolorBord,
        XlCColorBord,
        XlDcolorBord,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiAllRec,caiAll.color_bord),
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
        XtOffsetOf(XlCaiAllRec,caiAll.color_A_FG),
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
        XtOffsetOf(XlCaiAllRec,caiAll.color_A_BG),
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
        XtOffsetOf(XlCaiAllRec,caiAll.color_W_FG),
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
        XtOffsetOf(XlCaiAllRec,caiAll.color_W_BG),
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
        XtOffsetOf(XlCaiAllRec,caiAll.color_T_FG),
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
        XtOffsetOf(XlCaiAllRec,caiAll.color_T_BG),
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
        XtOffsetOf(XlCaiAllRec,caiAll.color_S_FG),
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
        XtOffsetOf(XlCaiAllRec,caiAll.color_S_BG),
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
        XtOffsetOf(XlCaiAllRec,caiAll.color_M_FG),
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
        XtOffsetOf(XlCaiAllRec,caiAll.color_M_BG),
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
        XtOffsetOf(XlCaiAllRec,caiAll.color_F_FG),
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
        XtOffsetOf(XlCaiAllRec,caiAll.color_F_BG),
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
        XtOffsetOf(XlCaiAllRec,caiAll.frequenza1),
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
        XtOffsetOf(XlCaiAllRec,caiAll.frequenza2),
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
        XtOffsetOf(XlCaiAllRec,caiAll.normalfont),
        sizeof(XFontStruct*)
        },
/*	{
	XlNvarGerarchia,
        XlCVarGerarchia,
        XlDvarGerarchia,
        XlRGerarchia,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlCaiAllRec,caiAll.gerarchia),
        sizeof(ALL_HIERARCHY)
	}, */
	};
/* dichiarazioni funzioni varie */
static int RefreshAll();
static int calcola_blk_fore();
static int calcola_norm_fore(); 
Boolean XlGetCaiAllState();
static void DrawDirectlyOff();
static void DrawRettangolo();
/* dichiarazione dei metodi (methods) */
static void Seleziona();
static void AddSelez();
static void Initialize();
static void Destroy();
static void Redisplay();
static void GetBordForGC();
static void GetNormCaiAllGC();
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
XlCaiAllClassRec xlCaiAllClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "CaiAll",
    /* widget_size              */      sizeof(XlCaiAllRec),
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
  { /* caiAll fields */
    /* empty                    */      0
  }
};

WidgetClass xlCaiAllWidgetClass = (WidgetClass) &xlCaiAllClassRec;

static void DrawRettangolo(Widget cw,Drawable drawable,GC gc,int x1,int y1,int x2,int y2)
{
XlLinea(cw,drawable,gc,0,0,100,0);
XlLinea(cw,drawable,gc,0,0,0,100);
XlLinea(cw,drawable,gc,0,100,100,100);
XlLinea(cw,drawable,gc,100,0,100,100);
}

/*
Funzione chiamata da esterno 
che restituisce lo stato ed il tipo di allarme di uno widget
*/
Boolean XlGetCaiAllState(Widget w,char *tipo_all,int *stato)
{
XlCaiAllWidget cw= (XlCaiAllWidget) w;

if(cw==NULL)
	return(False);
if((cw->caiAll.tipo_all==ALLARM_CAIALL) || (cw->caiAll.tipo_all==WARNING_CAIALL) || (cw->caiAll.tipo_all==TOLERANCE_CAIALL) || (cw->caiAll.tipo_all==MANUAL_CAIALL) || 
	(cw->caiAll.tipo_all==FAULT_CAIALL))
	{
	*tipo_all=cw->caiAll.tipo_all;
	*stato=cw->caiAll.alterna;
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
XlCaiAllWidget cw= (XlCaiAllWidget) w;
if(cw->caiAll.tipo_all==ALLARM_CAIALL)
	{
	pix=cw->caiAll.color_A_FG;
	}
else if(cw->caiAll.tipo_all==WARNING_CAIALL)
	{
	pix=cw->caiAll.color_W_FG;
	}
else if(cw->caiAll.tipo_all==TOLERANCE_CAIALL)
        {
	pix=cw->caiAll.color_T_FG;
	}
        
else if(cw->caiAll.tipo_all==SIMUL_CAIALL)
        {
	pix=cw->caiAll.color_S_FG;
        }
else if(cw->caiAll.tipo_all==MANUAL_CAIALL)
        {
	pix=cw->caiAll.color_M_FG;
        }
else if(cw->caiAll.tipo_all==FAULT_CAIALL)
        {
	pix=cw->caiAll.color_F_FG;
        }
else if(cw->caiAll.tipo_all==NULL_CAIALL)
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
XlCaiAllWidget cw= (XlCaiAllWidget) w;
if(cw->caiAll.tipo_all==ALLARM_CAIALL)
	{
	pix=cw->caiAll.color_A_BG;
	}
else if(cw->caiAll.tipo_all==WARNING_CAIALL)
	{
	pix=cw->caiAll.color_W_BG;
	}
else if(cw->caiAll.tipo_all==TOLERANCE_CAIALL)
        {
	pix=cw->caiAll.color_T_BG;
	}
        
else if(cw->caiAll.tipo_all==SIMUL_CAIALL)
        {
	pix=cw->caiAll.color_S_BG;
        }
else if(cw->caiAll.tipo_all==MANUAL_CAIALL)
        {
        pix=cw->caiAll.color_M_BG;
        }
else if(cw->caiAll.tipo_all==FAULT_CAIALL)
        {
	pix=cw->caiAll.color_F_BG;
        }
else if(cw->caiAll.tipo_all==NULL_CAIALL)
	{
	pix=cw->core.background_pixel;
	}
else
	return(0);	/*nessun tipo previsto->errore*/
valori->foreground=pix;
return(1);		/*tutto OK*/
}
/*
Funzione che determina il comportamento del CaiAll. 
Tale comportamento e' determinato dallo stato e dal tipo
di allarme
*/
static int RefreshAll(Widget w)
{
XlCaiAllWidget cw= (XlCaiAllWidget) w;

/*
Verifico tipo di allarme in base allo stato dell' allarme associato
*/
if(cw->caiAll.status==EMISSIONE)
	{
	return(BLINK_ON_FQZ1);
	}
else if(cw->caiAll.status==RIENTRO)
	{
        return(BLINK_ON_FQZ2);/*blink a freq1*/
	}
else if(cw->caiAll.status==RICONOSCIUTO)
	{
        return(1);
        }
else

   return(0); /* CaiAll spento */
}

static void GetNormCaiAllGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
Pixel pix;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;

XlCaiAllWidget cw= (XlCaiAllWidget) w;

values.line_width = 1;
values.line_style = LineSolid;
values.font = cw->caiAll.font_info->fid;
#if defined XPRINTER_USED
if(modoPrint)
        {
	if(!calcola_norm_fore(cw,&values))
        	{
        	XlWarning("XlCaiAll","GetBlinkGC","Errore nel tipo di allarme!!!");
        	}
	values.foreground=XlPrintGetPixel(w,values.foreground);
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->caiAll.norm_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
	 if(!calcola_norm_fore(cw,&values))
        	{
        	XlWarning("XlCaiAll","GetBlinkGC","Errore nel tipo di allarme!!!");
        	}
        values.background = cw->core.background_pixel;
        cw->caiAll.norm_gc = XtGetGC(cw, valuemask, &values);
        }
#else
if(!calcola_norm_fore(cw,&values))
        {
        XlWarning("XlCaiAll","GetBlinkGC","Errore nel tipo di allarme!!!");
        }
values.background= cw->core.background_pixel;
printf("Valore in esadecimale del fore=%12x\n",values.foreground);
printf("Valore in esadecimale del back=%12x\n",values.background);
cw->caiAll.norm_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetBlinkGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;
XlCaiAllWidget cw= (XlCaiAllWidget) w;
values.line_width = 1;
values.line_style = LineSolid;
values.font = cw->caiAll.font_info->fid;
#if defined XPRINTER_USED
if(modoPrint)
        {
	if(!calcola_blk_fore(cw,&values))
        {
        XlWarning("XlCaiAll","GetBlinkGC","Errore nel tipo di allarme!!!");
        }
	values.background= XlPrintGetPixel(w,values.background);
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->caiAll.blink_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
	if(!calcola_blk_fore(cw,&values))
        	{
        	XlWarning("XlCaiAll","GetBlinkGC","Errore nel tipo di allarme!!!");
        	}
        values.background = cw->core.background_pixel;
        cw->caiAll.blink_gc = XtGetGC(cw, valuemask, &values);
        }
#else 	/* no stampa*/

if(!calcola_blk_fore(cw,&values))
	{
	XlWarning("XlCaiAll","GetBlinkGC","Errore nel tipo di allarme!!!");
	}
values.background = cw->core.background_pixel;
cw->caiAll.blink_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetClearGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle /*| GCFont*/;
XlCaiAllWidget cw= (XlCaiAllWidget) w;
values.line_width = 0;
values.line_style = LineSolid;
/*values.font = cw->caiAll.font_info->fid;*/
#if defined XPRINTER_USED
if(modoPrint)
        {
        values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
        values.background = XlPrintGetPixel(w,cw->caiAll.color_bord);
        cw->caiAll.clear_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
        values.foreground = cw->core.background_pixel;
        values.background = cw->caiAll.color_bord;
        cw->caiAll.clear_gc = XtGetGC(cw, valuemask, &values);
        }
#else
values.foreground = cw->core.background_pixel;
cw->caiAll.clear_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetBordForGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
Pixel pix;
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;
XlCaiAllWidget cw= (XlCaiAllWidget) w;
values.line_width = 1;
values.line_style = LineSolid;
values.font = cw->caiAll.font_info->fid;
#if defined XPRINTER_USED
if(modoPrint)
        {
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        values.foreground = XlPrintGetPixel(w,cw->caiAll.color_bord);
        cw->caiAll.for_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
        values.background = cw->core.background_pixel;
        values.foreground = cw->caiAll.color_bord;
        cw->caiAll.for_gc = XtGetGC(cw, valuemask, &values);
        }
#else
values.foreground = cw->caiAll.color_bord;
values.background=cw->core.background_pixel;
cw->caiAll.for_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetAllGCs(w,modoPrint)
Widget w;
Boolean modoPrint;
{
/*
Il blink e' ottenuto riempiendo l' oggetto con i 2 GC norm_gc e blink_gc
alternativamente (la lettera e' settata al momento della config).
Il GC  clear GC e' usato per lo stato di CaiAll spento mentre il for_gc
e' usato per la lettera indicate il tipo_allarme.
*/
GetNormCaiAllGC(w,modoPrint);
GetClearGC(w,modoPrint);
GetBlinkGC(w,modoPrint);
GetBordForGC(w,modoPrint);
}

static void DrawDirectlyOff(w)
Widget w;
{
/*
Funzione che disegna vuoto il CaiAll
*/
XlCaiAllWidget cw= (XlCaiAllWidget)w;
Window cwin;
int width,height;
char testo[3];  /* identificatore del tipo di allarme  ( max 2 car )*/
width=cw->core.width;
height=cw->core.height;

cwin=XlWindow((Widget)cw);
/*
Rettangolo riempito con colore sfondo, colore lettera configurabile
*/
XlRiempiRettangolo((Widget)cw,cwin,cw->caiAll.clear_gc,0,0,100,100);
DrawRettangolo((Widget)cw,cwin,cw->caiAll.clear_gc,0,0,100,100);
DrawRettangolo((Widget)cw,cwin,cw->caiAll.for_gc,0,0,100,100);
switch(cw->caiAll.tipo_all)
	{
	case ALLARM_CAIALL:
	strcpy(testo,cw->caiAll.str_allarm_A);
	break;
	case WARNING_CAIALL:
	strcpy(testo,cw->caiAll.str_allarm_W);
	break;
	case TOLERANCE_CAIALL:
	strcpy(testo,cw->caiAll.str_allarm_T);
	break;
	case SIMUL_CAIALL:
	strcpy(testo,cw->caiAll.str_allarm_S);
	break;
	case MANUAL_CAIALL:
	strcpy(testo,cw->caiAll.str_allarm_M);
	break;
	case FAULT_CAIALL:
	strcpy(testo,cw->caiAll.str_allarm_F);
	break;
	case NULL_CAIALL:
	sprintf(testo,"%c",cw->caiAll.tipo_all);
	break;
	default:
	sprintf(testo,"%c",cw->caiAll.tipo_all);
	break;
	}

XlTestoSimpleCentre((Widget)cw,cwin,cw->caiAll.for_gc,
		50,50,testo,cw->caiAll.font_info);
}
static void DrawDirectly(w,On)
Widget w;
Boolean On; /*Se On=True accendi CaiAll (blink_gc)*/
{
XlCaiAllWidget cw= (XlCaiAllWidget)w;
Window cwin;
int width,height;
char testo[3];	/* identificatore del tipo di allarme */
width=cw->core.width;
height=cw->core.height;

cwin=XlWindow((Widget)cw);
switch(cw->caiAll.tipo_all)
	{
	case ALLARM_CAIALL:
	strcpy(testo,cw->caiAll.str_allarm_A);
	break;
	case WARNING_CAIALL:
	strcpy(testo,cw->caiAll.str_allarm_W);
	break;
	case TOLERANCE_CAIALL:
	strcpy(testo,cw->caiAll.str_allarm_T);
	break;
	case SIMUL_CAIALL:
	strcpy(testo,cw->caiAll.str_allarm_S);
	break;
	case MANUAL_CAIALL:
	strcpy(testo,cw->caiAll.str_allarm_M);
	break;
	case FAULT_CAIALL:
	strcpy(testo,cw->caiAll.str_allarm_F);
	break;
	case NULL_CAIALL:
	sprintf(testo,"%c",cw->caiAll.tipo_all);
	break;
	default:
	sprintf(testo,"%c",cw->caiAll.tipo_all);
	break;
	}

/*
Se On =False disegna il CaiAll col blink_gc else col norm_gc
*/
if(On)
	{
	XlRiempiRettangolo((Widget)cw,cwin,cw->caiAll.blink_gc,0,0,100,100);
	DrawRettangolo((Widget)cw,cwin,cw->caiAll.norm_gc,0,0,100,100);
	XlTestoSimpleCentre((Widget)cw,cwin,cw->caiAll.norm_gc,
               	50,50,testo,cw->caiAll.font_info);
	}
else
	{
		
	XlRiempiRettangolo((Widget)cw,cwin,cw->caiAll.norm_gc,0,0,100,100);
	DrawRettangolo((Widget)cw,cwin,cw->caiAll.blink_gc,0,0,100,100);
	XlTestoSimpleCentre((Widget)cw,cwin,cw->caiAll.blink_gc,
                       50,50,testo,cw->caiAll.font_info);
	}		
}

static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{

XlCaiAllWidget new = (XlCaiAllWidget)tnew;
new->caiAll.alterna=0;
new->caiAll.tipo_all_old=NULL_CAIALL;
/*
Eliminata inizializzazione per miniASD
*/
/*new->caiAll.status=SPENTO;*/

/*
printf("CaiAll Initialize-new:alt=%ld ,lungh=%ld font=%s\n",new->xlcore.height0,new->xlcore.width0,new->caiAll.normalfont);
*/
/*
Setto il tipo di allarme per config a W cosi' e' visibile durante configuraz.
*/

if(new->caiAll.tipo_caiAll==CAIALL_UNIVERSAL) 
	{
	if(new->xlcore.config==True)
		new->caiAll.tipo_all=WARNING_CAIALL;   
	else
		new->caiAll.tipo_all=NULL_CAIALL;  /* tipo neutro per mmi run_time: la 
					   prima visualizzazione sara' fatta
					   con il colore del bkg pagina     */
	}
else if(new->caiAll.tipo_caiAll==CAIALL_A) 
	new->caiAll.tipo_all=ALLARM_CAIALL;
else if(new->caiAll.tipo_caiAll==CAIALL_W)
	new->caiAll.tipo_all=WARNING_CAIALL;
else if(new->caiAll.tipo_caiAll==CAIALL_T)
	new->caiAll.tipo_all=TOLERANCE_CAIALL;
else if(new->caiAll.tipo_caiAll==CAIALL_S)
	new->caiAll.tipo_all=SIMUL_CAIALL;
else if(new->caiAll.tipo_caiAll==CAIALL_M)
	new->caiAll.tipo_all=MANUAL_CAIALL;
else if(new->caiAll.tipo_caiAll==CAIALL_F)
	new->caiAll.tipo_all=FAULT_CAIALL;


if((new->caiAll.frequenza1)<=0)
	{
	new->caiAll.frequenza1=atof(FREQUENZA1);
	XlWarning("XlCaiAll","Initialize","frequenza1 deve essere >0");
	}
if((new->caiAll.frequenza2<=0))
	{
	new->caiAll.frequenza2=atof(FREQUENZA2);
	XlWarning("XlCaiAll","Initialize","frequenza2 deve essere >0");
	}	
if((new->caiAll.frequenza1)>4)
	{
	new->caiAll.frequenza1=atof(FREQUENZA1);
	XlWarning("XlCaiAll","Initialize","frequenza1 deve essere <4");
	}
if((new->caiAll.frequenza2>4))
	{
	new->caiAll.frequenza2=atof(FREQUENZA2);
	XlWarning("XlCaiAll","Initialize","frequenza2 deve essere <4");
	}	
/*
Inserisco nella struttura ALL_HIERARCHY la gerarchia nel 
giusto formato
*/
if(!LoadFont(new,new->caiAll.normalfont,False))
        if(!LoadFont(new,"fixed",False))
                XlError("XlCaiAll","Initialize",
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
XlCaiAllWidget curcw= (XlCaiAllWidget) current;
XlCaiAllWidget newcw= (XlCaiAllWidget) new;
Boolean do_redisplay = False;

/*
Configuro una diversa gerarchia del CaiAll
*/
/*
if(newcw->caiAll.status!=curcw->caiAll.status)
        {
printf("SetValues: curcwstatus_caiAll=%d\n",curcw->caiAll.status);
printf("SetValues: newcwstatus_caiAll=%d\n",newcw->caiAll.status);
	curcw->caiAll.status=newcw->caiAll.status;
        do_redisplay = True;
        }
*/
if(newcw->caiAll.tipo_caiAll!=curcw->caiAll.tipo_caiAll)
        {
printf("SetValues: curcwtipo_caiAll=%d\n",curcw->caiAll.tipo_caiAll);
printf("SetValues: newcwtipo_caiAll=%d\n",newcw->caiAll.tipo_caiAll);
	curcw->caiAll.tipo_caiAll=newcw->caiAll.tipo_caiAll;
	if(newcw->caiAll.tipo_caiAll==CAIALL_A) 
		{
		newcw->caiAll.tipo_all=ALLARM_CAIALL;
		printf("SETVALUES newcw->caiAll.tipo_all=%d\n",newcw->caiAll.tipo_all);
		}
	else if(newcw->caiAll.tipo_caiAll==CAIALL_W)
		newcw->caiAll.tipo_all=WARNING_CAIALL;
	else if(newcw->caiAll.tipo_caiAll==CAIALL_T)
		newcw->caiAll.tipo_all=TOLERANCE_CAIALL;
	else if(newcw->caiAll.tipo_caiAll==CAIALL_S)
		newcw->caiAll.tipo_all=SIMUL_CAIALL;
	else if(newcw->caiAll.tipo_caiAll==CAIALL_M)
		newcw->caiAll.tipo_all=MANUAL_CAIALL;
	else if(newcw->caiAll.tipo_caiAll==CAIALL_F)
		newcw->caiAll.tipo_all=FAULT_CAIALL;
	DrawDirectly(newcw,1);
        do_redisplay = True;
        }
if(strcmp(newcw->caiAll.str_allarm,curcw->caiAll.str_allarm))
        {
	format_hierarchy_new(newcw->caiAll.gerarchia.str_input,curcw->caiAll.str_allarm);
	DrawDirectly(curcw,1);
        do_redisplay = True;
        }
/*
Il colore della lettera, nel caso di CaiAll off e' dato dalla risorsa
color_bord
*/
if(curcw->caiAll.color_bord != newcw->caiAll.color_bord)
        {
        XtReleaseGC(curcw,curcw->caiAll.for_gc);
        XtReleaseGC(curcw,curcw->caiAll.clear_gc);
        GetBordForGC(newcw,0);
        GetClearGC(newcw,0);
	DrawDirectly(newcw,1);
        do_redisplay = True;
        }
if((curcw->caiAll.frequenza1) !=  newcw->caiAll.frequenza1)
	{
	do_redisplay = False;
	}
if((curcw->caiAll.frequenza2) !=  newcw->caiAll.frequenza2)
	{
	do_redisplay = False;
	}	
/*
 risorse connesse con sistema di simulazione
*/
if(strcmp(curcw->caiAll.normalfont,newcw->caiAll.normalfont))
        {
printf("Da set values carico il font\n");
        if(!LoadFont(newcw,newcw->caiAll.normalfont,False))
        if(!LoadFont(newcw,"fixed",False))
                XlError("XlCaiAll","SetValues",
                        "Impossibile caricare default font");
	DrawDirectly(newcw,1);
        do_redisplay = True;
        }
return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlCaiAllWidget cw= (XlCaiAllWidget) w;

/*
Elimina il widget nella lista_blink
*/

XlRemoveBlink(cw,0);
/*
        Rilascio i GC
*/
if (cw->caiAll.norm_gc)
        XtReleaseGC(cw,cw->caiAll.norm_gc);
if (cw->caiAll.blink_gc)
        XtReleaseGC(cw,cw->caiAll.blink_gc);
if (cw->caiAll.clear_gc)
        XtReleaseGC(cw,cw->caiAll.clear_gc);
if (cw->caiAll.for_gc)
        XtReleaseGC(cw,cw->caiAll.for_gc);

/* libero la font_struct */
if(cw->caiAll.font_info)
        XFreeFont(XtDisplay(cw),cw->caiAll.font_info);
}

static Boolean Compile(Widget w)
{
Boolean errors=False;
XlCaiAllWidget cw= (XlCaiAllWidget)w;
char appo[256];

if(!XtIsWidget(w))
        {
        XlWarning("XlCaiAll","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }
/*
 Devono esistere una gerarchia significativa
*/
if(strlen(cw->caiAll.str_allarm)==0)
	{
	XlErrComp(w,"Compile","Hierarchy not set ",NULL);
        errors=True;
	XlWarning("XlCaiAll","Compile","First input not defined");
	}
format_hierarchy_new(appo,cw->caiAll.str_allarm);
if(!strcmp(appo,"errore"))
	{
	XlErrComp(w,"Compile","Hierarchy not set correctly !!!",NULL);
        errors=True;
	}
if(cw->caiAll.frequenza1<=0)
	{
	cw->caiAll.frequenza1=atof(FREQUENZA1);
	XlErrComp(w,"Compile","frequenza1 must be positive",NULL);
	errors=True;
	}
if(cw->caiAll.frequenza2<=0)
	{
	cw->caiAll.frequenza2=atof(FREQUENZA2);
	XlErrComp(w,"Compile","frequenza2 must be positive",NULL);
	errors=True;
	}
if(cw->caiAll.frequenza1>4)
	{
	cw->caiAll.frequenza1=atof(FREQUENZA1);
	XlErrComp(w,"Compile","frequenza1 must be <4",NULL);
	errors=True;
	}
if(cw->caiAll.frequenza2>4)
	{
	cw->caiAll.frequenza2=atof(FREQUENZA2);
	XlErrComp(w,"Compile","frequenza2 must be <4",NULL);
	errors=True;
	}		
if(xlCaiAllClassRec.xlcore_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
        return(False);
        }

if(!(*superclass->xlcore_class.compile) (w,
     xlCaiAllClassRec.xlcore_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
        return(False);
        }
XlEvError(cw,errors);
return(True);
}

static void BlinkCaiAllProc(Widget w,int ind,Boolean On)
{
/*
Funzione chiamata da XlBlink ogni BLINK_RATE ms. che ridisegna il CaiAll
nello stato indicato da On
N.B.
ind non gestita
*/
DrawDirectly(w,On);

}

static Boolean Refresh(Widget w)
{
XlCaiAllWidget cw= (XlCaiAllWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;
float already_inserted;

if(cw->xlcore.config==True)
        return(True);
/*
Modifica per CaiAll sulla parte principale della pagina teleperm
Aspetto che teleperm sia stata realizzata prima di visualizzare i CaiAll.
*/
if(!XlWindow(cw))        
	return(True);
if(!XtIsWidget(w))
        {
        XlWarning("XlCaiAll","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlCaiAllClassRec.xlcore_class.database==NULL)
        {
        XlWarning("XlCaiAll","Refresh","database non definito");
        return(False);
        }
/*
Verificare se ha senso 
*/
if(!(*superclass->xlcore_class.refresh) (w))
        {
        XlWarning("XlCaiAll","Refresh","errore refresh classe superiore");
        return(False);
        }

/*
Inizio modifica di Fabio
*/	

already_inserted=XlGetBlink((Widget)cw,0); /*cw già inserito in lista_blink ?*/


/* 
Funzione per detrminare comportamento del caiAll .Viene letto il float
contenente la situazione allarmi, e viene settato sia il tipo di allarme 
(A,W,T,S) sia la situazione (PEN e/o ARC).
Il risultato e' posto in cw->caiAll.alterna
*/

cw->caiAll.alterna=RefreshAll( (Widget)cw );
/*
Poiche' il tipo di allarme potrebbe essere cambiato, cambio il GC
Non uso XchangeGC e rilascio il GC con XtRelease perche' il GC era stato allocato con XtGetGC.
*/
if((cw->caiAll.tipo_all!=cw->caiAll.tipo_all_old))
	{
	XtReleaseGC(cw,cw->caiAll.norm_gc);
	XtReleaseGC(cw,cw->caiAll.blink_gc);
	XtReleaseGC(cw,cw->caiAll.clear_gc);
	GetNormCaiAllGC(cw,False);
	GetBlinkGC(cw,False);
	GetClearGC(cw,False);
	cw->caiAll.tipo_all_old=cw->caiAll.tipo_all;
	}
        switch(cw->caiAll.alterna)
        	{
/*
Se il risultato di RefreshAll è BLINK_ON :se oggetto già presente nella
lista degli oggetti che blinkano esci, altrimenti inseriscilo nella lista
*/                       
		case BLINK_ON_FQZ1:
		if (already_inserted==(cw->caiAll.frequenza1))
			{
		/*	printf("BLINK1-CAIALLalready_inserted INDEX=%ld\n",cw);*/
			break;
			}
		if(already_inserted==0)/*blink caiAll not found */	
			{
		/*	printf("BLINK1-CAIALL NOT-already_inserted INDEX=%ld\n",cw);*/
			XlAddBlink(cw,0,cw->caiAll.frequenza1,(void*)BlinkCaiAllProc);
			break;
			}
		/*printf("BLINK1-CAIALL INDEX=%ld\n",cw);*/
		XlRemoveBlink(cw,0);/*blink freq!=freq1*/
		XlAddBlink(cw,0,cw->caiAll.frequenza1,(void*)BlinkCaiAllProc);
		break;
		
		case BLINK_ON_FQZ2:
		if (already_inserted==(cw->caiAll.frequenza2))
			break;
		if(already_inserted==0)/*blink caiAll not found */	
			{
			XlAddBlink(cw,0,cw->caiAll.frequenza2,(void*)BlinkCaiAllProc);
			break;
			}			
		XlRemoveBlink(cw,0);
		XlAddBlink(cw,0,cw->caiAll.frequenza2,(void*)BlinkCaiAllProc);
		break;		
		
		case 1:
		if(already_inserted!=0)
			XlRemoveBlink(cw,0);
		DrawDirectly(cw,1);	/*Disegna in blink_gc*/
		break;			/*CaiAll acceso*/
		
		case 0:
		if(already_inserted!=0)
			XlRemoveBlink(cw,0);
		DrawDirectlyOff(cw,0);	/*Disegna in normal_gc*/
		break;			/*CaiAll spento*/
		}	
return(True);
}



static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlCaiAllWidget cw= (XlCaiAllWidget)w;
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

(*superclass->core_class.expose) (w,event, NULL);
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
XlCaiAllWidget Xl_cw = (XlCaiAllWidget) cw;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
/* Carica il font ottenendo la descrizione del font stesso */

char **pf;
int i,nf;
Display *disp;

if(modoPrint)
        disp=XlDisplay(cw);
else
        disp=XtDisplay(cw);


if((Xl_cw->caiAll.font_info = modoPrint ? XLoadQueryFont(disp,fontname)
                                         : FontFallbackLoad(disp,fontname)) == NULL)
        {
        XlWarning("XlCaiAll","LoadFont","Cannot open font");
        return(False);
        }
return(True);
}

/*
        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
XlCaiAllWidget cw= (XlCaiAllWidget) w;
char *printerFont;

(*superclass->xlcore_class.print) (w);

printerFont=XtNewString(XlPrinterGetFont(w,
        cw->caiAll.normalfont,cw->caiAll.font_info));

/*
        Rilascio i GC
*/
if (cw->caiAll.norm_gc)
        XtReleaseGC(cw,cw->caiAll.norm_gc);
if (cw->caiAll.blink_gc)
        XtReleaseGC(cw,cw->caiAll.blink_gc);
if (cw->caiAll.clear_gc)
        XtReleaseGC(cw,cw->caiAll.clear_gc);

if(!LoadFont(cw,printerFont,True))
        LoadFont(cw,"fixed",True);
XtFree(printerFont);

GetAllGCs(cw,True);
DrawDirectly(cw,0); /*modifica di Fabio*/

if(cw->caiAll.font_info)
        XFreeFont(XlDisplay(cw),cw->caiAll.font_info);/* rilascio il font della
stampante */
/* rilascio i 3 GC della stampante */

if (cw->caiAll.norm_gc)
        XFreeGC(XlDisplay((Widget)cw),cw->caiAll.norm_gc);
if (cw->caiAll.blink_gc)
        XFreeGC(XlDisplay((Widget)cw),cw->caiAll.blink_gc);
if (cw->caiAll.clear_gc)
        XFreeGC(XlDisplay((Widget)cw),cw->caiAll.clear_gc);


if(!LoadFont(cw,cw->caiAll.normalfont,False))
        LoadFont(cw,"fixed",False);

GetAllGCs(cw,False);

return(True);
}
#endif
