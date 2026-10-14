/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlCai.c	5.2\t12/4/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlCai.c - widget cai per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlCaiP.h>

#include "sdi_conf.h"  /* Per maschere gestione allarmi */
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */
#define DEFAULTWIDTH 11
#define DEFAULTHEIGHT 11


/* lista delle risorse  */
static XtResource resources[]= {
        {
        XlNtipoCai,
        XlCTipoCai,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCaiRec,cai.tipo_cai),
        XmRImmediate,
        (XtPointer)CAI_UNIVERSAL
        },
        {
        XlNcolorBord,
        XlCColorBord,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiRec,cai.color_bord),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorA_FG,
        XlCColorA_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiRec,cai.color_A_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorA_BG,
        XlCColorA_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiRec,cai.color_A_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorW_FG,
        XlCColorW_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiRec,cai.color_W_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorW_BG,
        XlCColorW_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiRec,cai.color_W_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorT_FG,
        XlCColorT_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiRec,cai.color_T_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorT_BG,
        XlCColorT_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiRec,cai.color_T_BG),
        XmRString,
        XtDefaultForeground
        },
	{
        XlNcolorS_FG,
        XlCColorS_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiRec,cai.color_S_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorS_BG,
        XlCColorS_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiRec,cai.color_S_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorM_FG,
        XlCColorM_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiRec,cai.color_M_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorM_BG,
        XlCColorM_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiRec,cai.color_M_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorF_FG,
        XlCColorF_FG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiRec,cai.color_F_FG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorF_BG,
        XlCColorF_BG,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCaiRec,cai.color_F_BG),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNfrequenza1,
        XlCFrequenza1,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlCaiRec,cai.frequenza1),
        XtRString,
        FREQUENZA1
        },
        {
        XlNfrequenza2,
        XlCFrequenza2,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlCaiRec,cai.frequenza2),
        XtRString,
        FREQUENZA2
        },
	{
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiRec,cai.normalfont),
        XmRImmediate,
        "fixed"
        },
	{
        XlNvarGerarchia,
        XlCVarGerarchia,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiRec,cai.str_allarm),
        XmRImmediate,
        "[-1][-1][-1][-1][-1][-1]"
        },
	{
        XlNvarString_A,
        XlCVarString_A,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiRec,cai.str_allarm_A),
        XmRImmediate,
        "A"
        },
	{
        XlNvarString_W,
        XlCVarString_W,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiRec,cai.str_allarm_W),
        XmRImmediate,
        "W"
        },
	{
        XlNvarString_T,
        XlCVarString_T,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiRec,cai.str_allarm_T),
        XmRImmediate,
        "T"
        },
	{
        XlNvarString_S,
        XlCVarString_S,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiRec,cai.str_allarm_S),
        XmRImmediate,
        "S"
        },
	{
        XlNvarString_M,
        XlCVarString_M,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiRec,cai.str_allarm_M),
        XmRImmediate,
        "M"
        },
	{
        XlNvarString_F,
        XlCVarString_F,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCaiRec,cai.str_allarm_F),
        XmRImmediate,
        "F"
        },
        };

/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
       {
        XlNtipoCai,
        XlCTipoCai,
        XlDtipoCai,
        XlROption,
        NULL,
        XlOtipoCai,
        XlRGrIo,
        XtOffsetOf(XlCaiRec,cai.tipo_cai),
        sizeof(int)
        },
        {
        XlNcolorBord,
        XlCColorBord,
        XlDcolorBord,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCaiRec,cai.color_bord),
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
        XtOffsetOf(XlCaiRec,cai.color_A_FG),
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
        XtOffsetOf(XlCaiRec,cai.color_A_BG),
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
        XtOffsetOf(XlCaiRec,cai.color_W_FG),
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
        XtOffsetOf(XlCaiRec,cai.color_W_BG),
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
        XtOffsetOf(XlCaiRec,cai.color_T_FG),
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
        XtOffsetOf(XlCaiRec,cai.color_T_BG),
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
        XtOffsetOf(XlCaiRec,cai.color_S_FG),
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
        XtOffsetOf(XlCaiRec,cai.color_S_BG),
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
        XtOffsetOf(XlCaiRec,cai.color_M_FG),
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
        XtOffsetOf(XlCaiRec,cai.color_M_BG),
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
        XtOffsetOf(XlCaiRec,cai.color_F_FG),
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
        XtOffsetOf(XlCaiRec,cai.color_F_BG),
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
        XtOffsetOf(XlCaiRec,cai.frequenza1),
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
        XtOffsetOf(XlCaiRec,cai.frequenza2),
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
        XtOffsetOf(XlCaiRec,cai.normalfont),
        sizeof(XFontStruct*)
        },
	{
	XlNvarGerarchia,
        XlCVarGerarchia,
        XlDvarGerarchia,
        XlRGerarchia,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlCaiRec,cai.gerarchia),
        sizeof(ALL_HIERARCHY)
	},
	};
/* dichiarazioni funzioni varie */
static int Refresh_all();
static int calcola_blk_fore();
static int calcola_norm_fore(); 
Boolean ConvToPixel ();
Boolean XlGetCaiState();
static void DrawDirectlyOff();
static void DrawRettangolo();
/* dichiarazione dei metodi (methods) */
static void Seleziona();
static void AddSelez();
static void Initialize();
static void Destroy();
static void Redisplay();
static void GetBordForGC();
static void GetNormCaiGC();
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
XlCaiClassRec xlCaiClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "Cai",
    /* widget_size              */      sizeof(XlCaiRec),
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
  { /* cai fields */
    /* empty                    */      0
  }
};

WidgetClass xlCaiWidgetClass = (WidgetClass) &xlCaiClassRec;

static void DrawRettangolo(Widget cw,Drawable drawable,GC gc,int x1,int y1,int x2,int y2)
{
XlLinea(cw,drawable,gc,0,0,100,0);
XlLinea(cw,drawable,gc,0,0,0,100);
XlLinea(cw,drawable,gc,0,100,100,100);
XlLinea(cw,drawable,gc,100,0,100,100);
}
Boolean ConvToPixel (Widget ogget,char *Stringa,Pixel *Pix)
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
Boolean XlGetCaiState(Widget w,char *tipo_all,int *stato)
{
XlCaiWidget cw= (XlCaiWidget) w;

if(cw==NULL)
	return(False);
if((cw->cai.tipo_all==ALLARM_CAI) || (cw->cai.tipo_all==WARNING_CAI) || (cw->cai.tipo_all==TOLERANCE_CAI) || (cw->cai.tipo_all==MANUAL_CAI) || 
	(cw->cai.tipo_all==FAULT_CAI))
	{
	*tipo_all=cw->cai.tipo_all;
	*stato=cw->cai.alterna;
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
XlCaiWidget cw= (XlCaiWidget) w;
if(cw->cai.tipo_all==ALLARM_CAI)
	{
	pix=cw->cai.color_A_FG;
	}
else if(cw->cai.tipo_all==WARNING_CAI)
	{
	pix=cw->cai.color_W_FG;
	}
else if(cw->cai.tipo_all==TOLERANCE_CAI)
        {
	pix=cw->cai.color_T_FG;
	}
        
else if(cw->cai.tipo_all==SIMUL_CAI)
        {
	pix=cw->cai.color_S_FG;
        }
else if(cw->cai.tipo_all==MANUAL_CAI)
        {
	pix=cw->cai.color_M_FG;
        }
else if(cw->cai.tipo_all==FAULT_CAI)
        {
	pix=cw->cai.color_F_FG;
        }
else if(cw->cai.tipo_all==NULL_CAI)
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
XlCaiWidget cw= (XlCaiWidget) w;
if(cw->cai.tipo_all==ALLARM_CAI)
	{
	pix=cw->cai.color_A_BG;
	}
else if(cw->cai.tipo_all==WARNING_CAI)
	{
	pix=cw->cai.color_W_BG;
	}
else if(cw->cai.tipo_all==TOLERANCE_CAI)
        {
	pix=cw->cai.color_T_BG;
	}
        
else if(cw->cai.tipo_all==SIMUL_CAI)
        {
	pix=cw->cai.color_S_BG;
        }
else if(cw->cai.tipo_all==MANUAL_CAI)
        {
        pix=cw->cai.color_M_BG;
        }
else if(cw->cai.tipo_all==FAULT_CAI)
        {
	pix=cw->cai.color_F_BG;
        }
else if(cw->cai.tipo_all==NULL_CAI)
	{
	pix=cw->core.background_pixel;
	}
else
	return(0);	/*nessun tipo previsto->errore*/
valori->foreground=pix;
return(1);		/*tutto OK*/
}
/*
Funzione che determina il comportamento del Cai, in base al
Refresh precedentemente fatto da XlCore.
La funzione di OlDatabasePunti che acquisisce ultimo valore situazione
allarmi della gerarchia associata al Cai, e' GetHierAll.
*/
static int Refresh_all(Widget w)
{
XlCaiWidget cw= (XlCaiWidget) w;
ALL_SITUATION situazione_all;

situazione_all.valore=(int)cw->cai.gerarchia.input_valore;
/*
Verifico tipo di allarme
*/
if ((situazione_all.valore & ALL_A_MASK) &&
	((cw->cai.tipo_cai==CAI_UNIVERSAL) || (cw->cai.tipo_cai==CAI_A)))
	{
	cw->cai.tipo_all=ALLARM_CAI;
	if(situazione_all.valore_bit.PEN_A)
		return(BLINK_ON_FQZ1);
	else if(situazione_all.valore_bit.RIE_A)
		return(BLINK_ON_FQZ2);
	else if(situazione_all.valore_bit.ARC_A)
		return(1);
	}
else if((situazione_all.valore & ALL_W_MASK) &&
	((cw->cai.tipo_cai==CAI_UNIVERSAL) || (cw->cai.tipo_cai==CAI_W)))
	{
	cw->cai.tipo_all=WARNING_CAI;
	 if(situazione_all.valore_bit.PEN_W)
                 return(BLINK_ON_FQZ1);/*blink a freq1*/
	else if(situazione_all.valore_bit.RIE_W)
                 return(BLINK_ON_FQZ2);/*blink a freq2*/
        else if(situazione_all.valore_bit.ARC_W)
                 return(1);
	}
else if((situazione_all.valore & ALL_T_MASK) &&
	((cw->cai.tipo_cai==CAI_UNIVERSAL) || (cw->cai.tipo_cai==CAI_T)))
	{
        cw->cai.tipo_all=TOLERANCE_CAI;
	if(situazione_all.valore_bit.PEN_T)
                 return(BLINK_ON_FQZ1);/*blink a freq1*/
	else if(situazione_all.valore_bit.RIE_T)
                 return(BLINK_ON_FQZ2);/*blink a freq2*/
        else if(situazione_all.valore_bit.ARC_T)
                 return(1);
        }

else if((situazione_all.valore & ALL_S_MASK) &&
	((cw->cai.tipo_cai==CAI_UNIVERSAL) || (cw->cai.tipo_cai==CAI_S)))
	{
        cw->cai.tipo_all=SIMUL_CAI;
	if(situazione_all.valore_bit.PEN_S)
                 return(BLINK_ON_FQZ1);/*blink a freq1*/
	else if(situazione_all.valore_bit.RIE_S)
                 return(BLINK_ON_FQZ2);/*blink a freq1*/
        else if(situazione_all.valore_bit.ARC_S)
                 return(1);
        }

else if((situazione_all.valore & ALL_M_MASK) &&
	((cw->cai.tipo_cai==CAI_UNIVERSAL) || (cw->cai.tipo_cai==CAI_M)))
	{
        cw->cai.tipo_all=MANUAL_CAI;
        if(situazione_all.valore_bit.PEN_M)
                 return(BLINK_ON_FQZ1);/*blink a freq1*/
        else if(situazione_all.valore_bit.RIE_M)
                 return(BLINK_ON_FQZ2);/*blink a freq2*/
        else if(situazione_all.valore_bit.ARC_M)
                 return(1);
	}
else if((situazione_all.valore & ALL_F_MASK) &&
	((cw->cai.tipo_cai==CAI_UNIVERSAL) || (cw->cai.tipo_cai==CAI_F)))
	{
        cw->cai.tipo_all=FAULT_CAI;
	if(situazione_all.valore_bit.PEN_F)
                 return(BLINK_ON_FQZ1);/*blink a freq1*/
	else if(situazione_all.valore_bit.RIE_F)
                 return(BLINK_ON_FQZ2);/*blink a freq2*/
        else if(situazione_all.valore_bit.ARC_F)
                 return(1);
	}
else 
	{
	return(0); /* Cai spento */
	}

}

static void GetNormCaiGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
Pixel pix;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;

XlCaiWidget cw= (XlCaiWidget) w;

values.line_width = 1;
values.line_style = LineSolid;
values.font = cw->cai.font_info->fid;
#if defined XPRINTER_USED
if(modoPrint)
        {
	if(!calcola_norm_fore(cw,&values))
        	{
        	XlWarning("XlCai","GetBlinkGC","Errore nel tipo di allarme!!!");
        	}
	values.foreground=XlPrintGetPixel(w,values.foreground);
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->cai.norm_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
	 if(!calcola_norm_fore(cw,&values))
        	{
        	XlWarning("XlCai","GetBlinkGC","Errore nel tipo di allarme!!!");
        	}
        values.background = cw->core.background_pixel;
        cw->cai.norm_gc = XtGetGC(cw, valuemask, &values);
        }
#else
if(!calcola_norm_fore(cw,&values))
        {
        XlWarning("XlCai","GetBlinkGC","Errore nel tipo di allarme!!!");
        }
values.background= cw->core.background_pixel;
printf("Valore in esadecimale del fore=%12x\n",values.foreground);
printf("Valore in esadecimale del back=%12x\n",values.background);
cw->cai.norm_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetBlinkGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;
XlCaiWidget cw= (XlCaiWidget) w;
values.line_width = 1;
values.line_style = LineSolid;
values.font = cw->cai.font_info->fid;
#if defined XPRINTER_USED
if(modoPrint)
        {
	if(!calcola_blk_fore(cw,&values))
        {
        XlWarning("XlCai","GetBlinkGC","Errore nel tipo di allarme!!!");
        }
	values.background= XlPrintGetPixel(w,values.background);
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->cai.blink_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
	if(!calcola_blk_fore(cw,&values))
        	{
        	XlWarning("XlCai","GetBlinkGC","Errore nel tipo di allarme!!!");
        	}
        values.background = cw->core.background_pixel;
        cw->cai.blink_gc = XtGetGC(cw, valuemask, &values);
        }
#else 	/* no stampa*/

if(!calcola_blk_fore(cw,&values))
	{
	XlWarning("XlCai","GetBlinkGC","Errore nel tipo di allarme!!!");
	}
values.background = cw->core.background_pixel;
cw->cai.blink_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetClearGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle /*| GCFont*/;
XlCaiWidget cw= (XlCaiWidget) w;
values.line_width = 0;
values.line_style = LineSolid;
/*values.font = cw->cai.font_info->fid;*/
#if defined XPRINTER_USED
if(modoPrint)
        {
        values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->cai.clear_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
        values.foreground = cw->core.background_pixel;
        values.background = cw->core.background_pixel;
        cw->cai.clear_gc = XtGetGC(cw, valuemask, &values);
        }
#else
values.foreground = cw->core.background_pixel;
cw->cai.clear_gc = XtGetGC(cw, valuemask, &values);
#endif
}
#ifdef DEBUG_MODE
static void GetClearGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle /*| GCFont*/;
XlCaiWidget cw= (XlCaiWidget) w;
values.line_width = 0;
values.line_style = LineSolid;
/*values.font = cw->cai.font_info->fid;*/
#if defined XPRINTER_USED
if(modoPrint)
        {
        values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
        values.background = XlPrintGetPixel(w,cw->cai.color_bord);
        cw->cai.clear_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
        values.foreground = cw->core.background_pixel;
        values.background = cw->cai.color_bord;
        cw->cai.clear_gc = XtGetGC(cw, valuemask, &values);
        }
#else
values.foreground = cw->core.background_pixel;
cw->cai.clear_gc = XtGetGC(cw, valuemask, &values);
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
XlCaiWidget cw= (XlCaiWidget) w;
values.line_width = 1;
values.line_style = LineSolid;
values.font = cw->cai.font_info->fid;
#if defined XPRINTER_USED
if(modoPrint)
        {
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        values.foreground = XlPrintGetPixel(w,cw->cai.color_bord);
        cw->cai.for_gc = XCreateGC(XlDisplay((Widget)cw),XlWindow((Widget)cw),
                                 valuemask, &values);
        }
else
        {
        values.background = cw->core.background_pixel;
        values.foreground = cw->cai.color_bord;
        cw->cai.for_gc = XtGetGC(cw, valuemask, &values);
        }
#else
values.foreground = cw->cai.color_bord;
values.background=cw->core.background_pixel;
cw->cai.for_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetAllGCs(w,modoPrint)
Widget w;
Boolean modoPrint;
{
/*
Il blink e' ottenuto riempiendo l' oggetto con i 2 GC norm_gc e blink_gc
alternativamente (la lettera e' settata al momento della config).
Il GC  clear GC e' usato per lo stato di Cai spento mentre il for_gc
e' usato per la lettera indicate il tipo_allarme.
*/
GetNormCaiGC(w,modoPrint);
GetClearGC(w,modoPrint);
GetBlinkGC(w,modoPrint);
GetBordForGC(w,modoPrint);
}

static void DrawDirectlyOff(w)
Widget w;
{
/*
Funzione che disegna vuoto il Cai
*/
XlCaiWidget cw= (XlCaiWidget)w;
Window cwin;
int width,height;
char testo[3];  /* identificatore del tipo di allarme  ( max 2 car )*/
width=cw->core.width;
height=cw->core.height;

cwin=XlWindow((Widget)cw);
/*
Rettangolo riempito con colore sfondo, colore lettera configurabile
*/
XlRiempiRettangolo((Widget)cw,cwin,cw->cai.clear_gc,0,0,100,100);
DrawRettangolo((Widget)cw,cwin,cw->cai.clear_gc,0,0,100,100);
/*
Se non sono in modalita' di debug ritorno dalla funzione
*/
#ifndef DEBUG_MODE
return;
#endif
DrawRettangolo((Widget)cw,cwin,cw->cai.for_gc,0,0,100,100);
switch(cw->cai.tipo_all)
	{
	case ALLARM_CAI:
	strcpy(testo,cw->cai.str_allarm_A);
	break;
	case WARNING_CAI:
	strcpy(testo,cw->cai.str_allarm_W);
	break;
	case TOLERANCE_CAI:
	strcpy(testo,cw->cai.str_allarm_T);
	break;
	case SIMUL_CAI:
	strcpy(testo,cw->cai.str_allarm_S);
	break;
	case MANUAL_CAI:
	strcpy(testo,cw->cai.str_allarm_M);
	break;
	case FAULT_CAI:
	strcpy(testo,cw->cai.str_allarm_F);
	break;
	case NULL_CAI:
	sprintf(testo,"%c",cw->cai.tipo_all);
	break;
	default:
	sprintf(testo,"%c",cw->cai.tipo_all);
	break;
	}

XlTestoSimpleCentre((Widget)cw,cwin,cw->cai.for_gc,
		50,50,testo,cw->cai.font_info);
}
static void DrawDirectly(w,On)
Widget w;
Boolean On; /*Se On=True accendi Cai (blink_gc)*/
{
XlCaiWidget cw= (XlCaiWidget)w;
Window cwin;
int width,height;
char testo[3];	/* identificatore del tipo di allarme */
width=cw->core.width;
height=cw->core.height;

cwin=XlWindow((Widget)cw);
switch(cw->cai.tipo_all)
	{
	case ALLARM_CAI:
	strcpy(testo,cw->cai.str_allarm_A);
	break;
	case WARNING_CAI:
	strcpy(testo,cw->cai.str_allarm_W);
	break;
	case TOLERANCE_CAI:
	strcpy(testo,cw->cai.str_allarm_T);
	break;
	case SIMUL_CAI:
	strcpy(testo,cw->cai.str_allarm_S);
	break;
	case MANUAL_CAI:
	strcpy(testo,cw->cai.str_allarm_M);
	break;
	case FAULT_CAI:
	strcpy(testo,cw->cai.str_allarm_F);
	break;
	case NULL_CAI:
	sprintf(testo,"%c",cw->cai.tipo_all);
	break;
	default:
	sprintf(testo,"%c",cw->cai.tipo_all);
	break;
	}

/*
Se On =False disegna il Cai col blink_gc else col norm_gc
*/
if(On)
	{
	XlRiempiRettangolo((Widget)cw,cwin,cw->cai.blink_gc,0,0,100,100);
	DrawRettangolo((Widget)cw,cwin,cw->cai.norm_gc,0,0,100,100);
	XlTestoSimpleCentre((Widget)cw,cwin,cw->cai.norm_gc,
               	50,50,testo,cw->cai.font_info);
	}
else
	{
		
	XlRiempiRettangolo((Widget)cw,cwin,cw->cai.norm_gc,0,0,100,100);
	DrawRettangolo((Widget)cw,cwin,cw->cai.blink_gc,0,0,100,100);
	XlTestoSimpleCentre((Widget)cw,cwin,cw->cai.blink_gc,
                       50,50,testo,cw->cai.font_info);
	}		
}

static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlCaiWidget req = (XlCaiWidget)treq;
XlCaiWidget new = (XlCaiWidget)tnew;
new->cai.alterna=0;
new->cai.tipo_all_old=NULL_CAI;

/*
printf("Cai Initialize-new:alt=%ld ,lungh=%ld font=%s\n",new->xlcore.height0,new->xlcore.width0,new->cai.normalfont);
*/
/*
Setto il tipo di allarme per config a W cosi' e' visibile durante configuraz.
*/

if(new->cai.tipo_cai==CAI_UNIVERSAL) 
	{
	if(new->xlcore.config==True)
		new->cai.tipo_all=WARNING_CAI;   
	
	else
		new->cai.tipo_all=NULL_CAI;  /* tipo neutro per mmi run_time: la 
					   prima visualizzazione sara' fatta
					   con il colore del bkg pagina     */
	}
else if(new->cai.tipo_cai==CAI_A) 
	new->cai.tipo_all=ALLARM_CAI;
else if(new->cai.tipo_cai==CAI_W)
	new->cai.tipo_all=WARNING_CAI;
else if(new->cai.tipo_cai==CAI_T)
	new->cai.tipo_all=TOLERANCE_CAI;
else if(new->cai.tipo_cai==CAI_S)
	new->cai.tipo_all=SIMUL_CAI;
else if(new->cai.tipo_cai==CAI_M)
	new->cai.tipo_all=MANUAL_CAI;
else if(new->cai.tipo_cai==CAI_F)
	new->cai.tipo_all=FAULT_CAI;


if((new->cai.frequenza1)<=0)
	{
	new->cai.frequenza1=atof(FREQUENZA1);
	XlWarning("XlCai","Initialize","frequenza1 deve essere >0");
	}
if((new->cai.frequenza2<=0))
	{
	new->cai.frequenza2=atof(FREQUENZA2);
	XlWarning("XlCai","Initialize","frequenza2 deve essere >0");
	}	
if((new->cai.frequenza1)>4)
	{
	new->cai.frequenza1=atof(FREQUENZA1);
	XlWarning("XlCai","Initialize","frequenza1 deve essere <4");
	}
if((new->cai.frequenza2>4))
	{
	new->cai.frequenza2=atof(FREQUENZA2);
	XlWarning("XlCai","Initialize","frequenza2 deve essere <4");
	}	
/*
Alloco all' interno dell' oggetto Cai 
la sua gerarchia
*/
new->cai.str_allarm=(char *) XtCalloc(1,strlen(req->cai.str_allarm)+1);
strcpy(new->cai.str_allarm,req->cai.str_allarm);

/*
Inserisco nella struttura ALL_HIERARCHY la gerarchia nel 
giusto formato
*/
format_hierarchy_new(new->cai.gerarchia.str_input,new->cai.str_allarm);
/*
Inizializzo stato del cai
*/

if(!strcmp(new->cai.gerarchia.str_input,"errore"))
	{ XlWarning("XlCai","Initialize",
                        "Valore della gerarchia non corretto !!!");
	}
/*
else
	printf("XlCai:Initialize:valore gerarchia in esame=%s\n",new->cai.gerarchia.str_input);
*/
if(!LoadFont(new,new->cai.normalfont,False))
        if(!LoadFont(new,"fixed",False))
                XlError("XlCai","Initialize",
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
XlCaiWidget curcw= (XlCaiWidget) current;
XlCaiWidget newcw= (XlCaiWidget) new;
Boolean do_redisplay = False;

/*
Configuro una diversa gerarchia del Cai
*/

if(strcmp(newcw->cai.str_allarm,curcw->cai.str_allarm))
        {
	format_hierarchy_new(newcw->cai.gerarchia.str_input,newcw->cai.str_allarm);
	DrawDirectly(newcw,1);
        do_redisplay = True;
        }
/*
Il colore della lettera, nel caso di Cai off e' dato dalla risorsa
color_bord
*/
if(curcw->cai.color_bord != newcw->cai.color_bord)
        {
        XtReleaseGC(curcw,curcw->cai.for_gc);
        XtReleaseGC(curcw,curcw->cai.clear_gc);
        GetBordForGC(newcw,0);
        GetClearGC(newcw,0);
	DrawDirectly(newcw,1);
        do_redisplay = True;
        }
if((curcw->cai.frequenza1) !=  newcw->cai.frequenza1)
	{
	do_redisplay = False;
	}
if((curcw->cai.frequenza2) !=  newcw->cai.frequenza2)
	{
	do_redisplay = False;
	}	
/*
 risorse connesse con sistema di simulazione
*/
if(strcmp(curcw->cai.normalfont,newcw->cai.normalfont))
        {
printf("Da set values carico il font\n");
        if(!LoadFont(newcw,newcw->cai.normalfont,False))
        if(!LoadFont(newcw,"fixed",False))
                XlError("XlCai","SetValues",
                        "Impossibile caricare default font");
	DrawDirectly(newcw,1);
        do_redisplay = True;
        }
return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlCaiWidget cw= (XlCaiWidget) w;

/*
Elimina il widget nella lista_blink
*/

XlRemoveBlink(cw,0);
/*
        Rilascio i GC
*/
if (cw->cai.norm_gc)
        XtReleaseGC(cw,cw->cai.norm_gc);
if (cw->cai.blink_gc)
        XtReleaseGC(cw,cw->cai.blink_gc);
if (cw->cai.clear_gc)
        XtReleaseGC(cw,cw->cai.clear_gc);
if (cw->cai.for_gc)
        XtReleaseGC(cw,cw->cai.for_gc);

/* libero la font_struct */
if(cw->cai.str_allarm)
      XtFree(cw->cai.str_allarm);
/*
if(cw->cai.font_info)
        XFreeFont(XtDisplay(cw),cw->cai.font_info);
*/
}

static Boolean Compile(Widget w)
{
Boolean errors=False;
XlCaiWidget cw= (XlCaiWidget)w;
char appo[256];

if(!XtIsWidget(w))
        {
        XlWarning("XlCai","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }
/*
 Devono esistere una gerarchia significativa
*/
if(strlen(cw->cai.str_allarm)==0)
	{
	XlErrComp(w,"Compile","Hierarchy not set ",NULL);
        errors=True;
	XlWarning("XlCai","Compile","First input not defined");
	}
format_hierarchy_new(appo,cw->cai.str_allarm);
if(!strcmp(appo,"errore"))
	{
	XlErrComp(w,"Compile","Hierarchy not set correctly !!!",NULL);
        errors=True;
	}
if(cw->cai.frequenza1<=0)
	{
	cw->cai.frequenza1=atof(FREQUENZA1);
	XlErrComp(w,"Compile","frequenza1 must be positive",NULL);
	errors=True;
	}
if(cw->cai.frequenza2<=0)
	{
	cw->cai.frequenza2=atof(FREQUENZA2);
	XlErrComp(w,"Compile","frequenza2 must be positive",NULL);
	errors=True;
	}
if(cw->cai.frequenza1>4)
	{
	cw->cai.frequenza1=atof(FREQUENZA1);
	XlErrComp(w,"Compile","frequenza1 must be <4",NULL);
	errors=True;
	}
if(cw->cai.frequenza2>4)
	{
	cw->cai.frequenza2=atof(FREQUENZA2);
	XlErrComp(w,"Compile","frequenza2 must be <4",NULL);
	errors=True;
	}		
if(xlCaiClassRec.xlcore_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
        return(False);
        }

if(!(*superclass->xlcore_class.compile) (w,
      xlCaiClassRec.xlcore_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
        return(False);
        }
XlEvError(cw,errors);
return(True);
}

//static void BlinkCaiProc(Widget w,int ind,Boolean On)
void BlinkCaiProc(Widget w,int ind,Boolean On)
{
/*
Funzione chiamata da XlBlink ogni BLINK_RATE ms. che ridisegna il Cai
nello stato indicato da On
N.B.
ind non gestita
*/
DrawDirectly(w,On);

}

static Boolean Refresh(Widget w)
{
XlCaiWidget cw= (XlCaiWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;
float already_inserted;

if(cw->xlcore.config==True)
        return(True);
/*
Modifica per Cai sulla parte principale della pagina teleperm
Aspetto che teleperm sia stata realizzata prima di visualizzare i Cai.
*/
if(!XlWindow(cw))        
	return(True);
if(!XtIsWidget(w))
        {
        XlWarning("XlCai","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlCaiClassRec.xlcore_class.database==NULL)
        {
        XlWarning("XlCai","Refresh","database non definito");
        return(False);
        }
if(!(*superclass->xlcore_class.refresh) (w))
        {
        XlWarning("XlCai","Refresh","errore refresh classe superiore");
        return(False);
        }

/*
Inizio modifica di Fabio
*/	

already_inserted=XlGetBlink((Widget)cw,0); /*cw già inserito in lista_blink ?*/


/* 
Funzione per detrminare comportamento del cai .Viene letto il float
contenente la situazione allarmi, e viene settato sia il tipo di allarme 
(A,W,T,S) sia la situazione (PEN e/o ARC).
Il risultato e' posto in cw->cai.alterna
*/

cw->cai.alterna=Refresh_all( (Widget)cw );
/*
Poiche' il tipo di allarme potrebbe essere cambiato, cambio il GC
Non uso XchangeGC e rilascio il GC con XtRelease perche' il GC era stato allocato con XtGetGC.
*/
if((cw->cai.tipo_all!=cw->cai.tipo_all_old))
	{
	XtReleaseGC(cw,cw->cai.norm_gc);
	XtReleaseGC(cw,cw->cai.blink_gc);
	XtReleaseGC(cw,cw->cai.clear_gc);
	GetNormCaiGC(cw,False);
	GetBlinkGC(cw,False);
	GetClearGC(cw,False);
	cw->cai.tipo_all_old=cw->cai.tipo_all;
	}
        switch(cw->cai.alterna)
        	{
/*
Se il risultato di Refresh_all è BLINK_ON :se oggetto già presente nella
lista degli oggetti che blinkano esci, altrimenti inseriscilo nella lista
*/                       
		case BLINK_ON_FQZ1:
		if (already_inserted==(cw->cai.frequenza1))
			{
		/*	printf("BLINK1-CAIalready_inserted INDEX=%ld\n",cw);*/
			break;
			}
		if(already_inserted==0)/*blink cai not found */	
			{
		/*	printf("BLINK1-CAI NOT-already_inserted INDEX=%ld\n",cw);*/
			XlAddBlink(cw,0,cw->cai.frequenza1,(void*)BlinkCaiProc);
			break;
			}
		/*printf("BLINK1-CAI INDEX=%ld\n",cw);*/
		XlRemoveBlink(cw,0);/*blink freq!=freq1*/
		XlAddBlink(cw,0,cw->cai.frequenza1,(void*)BlinkCaiProc);
		break;
		
		case BLINK_ON_FQZ2:
		if (already_inserted==(cw->cai.frequenza2))
			break;
		if(already_inserted==0)/*blink cai not found */	
			{
			XlAddBlink(cw,0,cw->cai.frequenza2,(void*)BlinkCaiProc);
			break;
			}			
		XlRemoveBlink(cw,0);
		XlAddBlink(cw,0,cw->cai.frequenza2,(void*)BlinkCaiProc);
		break;
		
		case 1:
		if(already_inserted!=0)
			XlRemoveBlink(cw,0);
		DrawDirectly(cw,1);	/*Disegna in blink_gc*/
		break;			/*Cai acceso*/
		
		case 0:
		if(already_inserted!=0)
			XlRemoveBlink(cw,0);
		DrawDirectlyOff(cw);	/*Disegna in normal_gc*/
		break;			/*Cai spento*/
		}	
return(True);
}



static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlCaiWidget cw= (XlCaiWidget)w;
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
XlCaiWidget Xl_cw = (XlCaiWidget) cw;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
/* Carica il font ottenendo la descrizione del font stesso */

char **pf;
int i,nf;
Display *disp;

if(modoPrint)
	{
        disp=XlDisplay(cw);
	if((Xl_cw->cai.font_info = XLoadQueryFont(disp,fontname)) == NULL)
        	{
        	XlWarning("XlCai","LoadFont","Cannot open font");
        	return(False);
        	}
	}
else
	{
        disp=XtDisplay(cw);
	if((Xl_cw->cai.font_info = XlGetFont(disp,fontname)) == NULL)
        	{
        	XlWarning("XlCai","LoadFont","Cannot open font");
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
XlCaiWidget cw= (XlCaiWidget) w;
char *printerFont;

(*superclass->xlcore_class.print) (w);
printerFont=XtNewString(XlPrinterGetFont(w,
        cw->cai.normalfont,cw->cai.font_info));

/*
        Rilascio i GC
*/
if (cw->cai.norm_gc)
        XtReleaseGC(cw,cw->cai.norm_gc);
if (cw->cai.blink_gc)
        XtReleaseGC(cw,cw->cai.blink_gc);
if (cw->cai.clear_gc)
        XtReleaseGC(cw,cw->cai.clear_gc);
if (cw->cai.for_gc)
        XtReleaseGC(cw,cw->cai.for_gc);

XtFree(printerFont);
if(!LoadFont(cw,printerFont,True))
        LoadFont(cw,"fixed",True);

GetAllGCs(cw,True);
if(cw->cai.alterna == 0)
        DrawDirectlyOff(cw);
else
	DrawDirectly(cw,0); /*modifica di Fabio*/	

/* rilascio i 3 GC della stampante */

if (cw->cai.norm_gc)
        XFreeGC(XlDisplay((Widget)cw),cw->cai.norm_gc);
if (cw->cai.blink_gc)
        XFreeGC(XlDisplay((Widget)cw),cw->cai.blink_gc);
if (cw->cai.clear_gc)
        XFreeGC(XlDisplay((Widget)cw),cw->cai.clear_gc);
if (cw->cai.for_gc)
        XFreeGC(XlDisplay((Widget)cw),cw->cai.for_gc);


if(!LoadFont(cw,cw->cai.normalfont,False))
        LoadFont(cw,"fixed",False);

GetAllGCs(cw,False);

return(True);
}
#endif
