/**********************************************************************
*
*       C Source:               XlChangePage.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Feb  3 15:04:57 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlChangePage.c-16 %  (%full_filespec: XlChangePage.c-16:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlChangePage.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlChangePage.c - widget cambio pagina
 */
#include <stdio.h>
#include <string.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlP.h>
#include <X11/CoreP.h>
#include <Xl/XlCoreP.h>
#include <X11/CompositeP.h>
#include <Xl/XlChangePageP.h>
#include "change_page.bmp"
#include "close_page.bmp"
#include "new_page.bmp"
#include "nullo.bmp"
#include "system.bmp"
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */
#include "fontfallback.h"


/* lista delle risorse  */
static XtResource resources[]= {
	{
        XlNnormFg,
        XlCNormFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlChangePageRec,changePage.norm_fg),
        XmRString,
        XtDefaultForeground
        },
	{
        XlNnormBg,
        XlCNormBg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlChangePageRec,changePage.norm_bg),
        XmRString,
        XtDefaultForeground
        },
	{
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlChangePageRec,changePage.normalfont),
        XmRImmediate,
        "fixed"
        },
        {
        XlNchangeType,
        XlCChangeType,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlChangePageRec,changePage.tipo),
        XmRString,
        (XtPointer)DEFAULT_CHANGE_MODE
        },
        {
        XlNchangeDraw,
        XlCChangeDraw,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlChangePageRec,changePage.draw),
        XmRString,
        (XtPointer)DEFAULT_DRAW
        },
	{
        XlNpageName,
        XlCPageName,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlChangePageRec,changePage.pagina),
        XmRImmediate,
        ""
        },
	{
        XlNlabelText,
        XlCLabelText,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlChangePageRec,changePage.label),
        XmRImmediate,
        ""
        },
        {
        XlNnameButton,
        XlCNameButton,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlChangePageRec,changePage.name_button),
        XmRImmediate,
        ""
        },
        {
        XlNnameBmp,
        XlCNameBmp,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlChangePageRec,changePage.name_bmp),
        XmRImmediate,
        ""
        },
        {
        XlNcommand,
        XlCCommand,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlChangePageRec,changePage.command),
        XmRImmediate,
        ""
        },
        {
        XlNfillButton,
        XlCFillButton,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlChangePageRec,changePage.fill_button),
        XmRString,
        (XtPointer)DEFAULT_FILL
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
        XtOffsetOf(XlChangePageRec,changePage.norm_fg),
        sizeof(Pixel),
        },
	{
        XlNnormBg,
        XlCNormBg,
        XlDnormBg,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlChangePageRec,changePage.norm_bg),
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
        XtOffsetOf(XlChangePageRec,changePage.normalfont),
        sizeof(XFontStruct*)
        },
        {
        XlNchangeType,
        XlCChangeType,
        XlDchangeType,
        XlROption,
        NULL,
        XlOchangeType,
        XlRGrIo,
        XtOffsetOf(XlChangePageRec,changePage.tipo),
        sizeof(int)
        },
        {
        XlNchangeDraw,
        XlCChangeDraw,
        XlDchangeDraw,
        XlROption,
        NULL,
        XlOchangeDraw,
        XlRGrIo,
        XtOffsetOf(XlChangePageRec,changePage.draw),
        sizeof(int)
        },
        {
        XlNpageName,
        XlCPageName,
        XlDpageName,
        XlRText,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlChangePageRec,changePage.pagina),
        sizeof(char*),
        },
        {
        XlNlabelText,
        XlCLabelText,
        XlDlabelText,
        XlRText,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlChangePageRec,changePage.label),
        sizeof(char*),
        },
        {
        XlNnameButton,
        XlCNameButton,
        XlDnameButton,
        XlRText,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlChangePageRec,changePage.name_button),
        sizeof(char*),
        },
        {
        XlNnameBmp,
        XlCNameBmp,
        XlDnameBmp,
        XlRFile,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlChangePageRec,changePage.name_bmp),
        sizeof(char*),
        },
        {
        XlNcommand,
        XlCCommand,
        XlDcommand,
        XlRText,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlChangePageRec,changePage.command),
        sizeof(char*),
        },
        {
        XlNfillButton,
        XlCFillButton,
        XlDfillButton,
        XlROption,
        NULL,
        XlOfillButton,
        XlRGrIo,
        XtOffsetOf(XlChangePageRec,changePage.fill_button),
        sizeof(int)
        },
	};

/* dichiarazioni funzioni varie */
static void GetNormFgGC();
static void GetClearGC();
static void GetAllGCs();
static Boolean LoadFont();
static void ActivateToggle();
static Boolean LoadBitmap();
static void LoadNullBitmap();
static void add_translation();
static void CreaFigli();
static void DistruggiFigli();
static void SettaActionsFigli();


         
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

/* actions del widget XlChangePage */
static void Seleziona();
static void AddSelez();
static void DoLayout();

/* translations  */
static char defaultTranslations[]=
        "~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static XtActionsRec actions[] = {
        {"Seleziona",Seleziona},
        {"AddSelez",AddSelez}
        };

static char textTranslations[]="#replace\n\
        ~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static char buttonTranslations[]="#replace\n\
        ~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";


/* Inizializzazione del class record */
XlChangePageClassRec xlChangePageClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlManagerClassRec,
    /* class_name               */      "ChangePage",
    /* widget_size              */      sizeof(XlChangePageRec),
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
    /* resize                   */      Resize,  /* XtInheritResize, */
    /* expose                   */      Redisplay/*XtInheritExpose */,
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
    /* geometry_manager          */ 	GeometryManager, /*XtInheritGeometryManager,  */
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
  { /* changePage fields */
    /* empty                    */      0
  }
};

WidgetClass xlChangePageWidgetClass = (WidgetClass) &xlChangePageClassRec;

static XtGeometryResult GeometryManager(w, request, reply)
Widget w;
XtWidgetGeometry *request;
XtWidgetGeometry *reply;
{
return XtGeometryYes;
}

static void Resize(w)
Widget w;
{
XlChangePageWidget cw= (XlChangePageWidget)w;

}
static void GetNormGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlChangePageWidget cw= (XlChangePageWidget) w;
values.foreground = cw->changePage.norm_fg;
if(cw->changePage.fill_button==DEFAULT_FILL)
	{
	values.background = cw->core.background_pixel;
	}
else
	values.background = cw->changePage.norm_bg;
values.line_width = 1;
values.line_style = LineSolid;
cw->changePage.norm_gc = XtGetGC(cw, valuemask, &values);
}
static void GetClearGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlChangePageWidget cw= (XlChangePageWidget) w;
if(cw->changePage.fill_button==DEFAULT_FILL)
	{
	values.foreground = cw->core.background_pixel;
//	printf("Preso il pixel di bakground di core per clearnormGC\n");
	}
else
	values.foreground = cw->changePage.norm_bg;
values.background = cw->changePage.norm_fg;
values.line_width = 0;
values.line_style = LineSolid;
cw->changePage.clear_gc = XtGetGC(cw, valuemask, &values);
}


static void GetAllGCs(w)
Widget w;
{
GetNormGC(w);
GetClearGC(w);
}


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlChangePageWidget new = (XlChangePageWidget)tnew;
int i,app_int;
Arg arg[25];
Pixmap pix;
Dimension w_font,h_font,text_width,text_height;
int bottone_width,bottone_height;

if(!LoadFont(new,new->changePage.normalfont))
        if(!LoadFont(new,"fixed"))
                XlError("XlChangePage","Initialize",
		"Impossibile caricare default font");
/*
Carico il graphic context e prima setto il tipo di fill
*/
if(new->changePage.fill_button==DEFAULT_FILL)
	new->changePage.norm_bg=new->core.background_pixel;
GetAllGCs(new);

new->changePage.text=NULL;
new->changePage.bottone=NULL;
new->changePage.bitmap_norm=NULL;
CreaFigli(new);
SettaActionsFigli(new);
}

#define superclass (&xlManagerClassRec)

static void Seleziona(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/*
 Richiama la routine di select di XlChangePage per la gestione della
 selezione
*/
if((strcmp(XtName(w),"__text")==0)||(strcmp(XtName(w),"__bottone")==0))
        (*superclass->xlmanager_class.select) (XtParent(w),0);
else
        (*superclass->xlmanager_class.select) (w,0);
}

static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/* richiama il metodo di select */
if((strcmp(XtName(w),"__text")==0)||(strcmp(XtName(w),"__bottone")==0))
        (*superclass->xlmanager_class.select) (XtParent(w),1);
else
        (*superclass->xlmanager_class.select) (w,1);
}

static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
Arg comp_args[2],argum[3];
XlChangePageWidget curcw= (XlChangePageWidget) current;
XlChangePageWidget newcw= (XlChangePageWidget) new;
Boolean do_redisplay = False;
XmString str; /* stringa per label bottone */

if(curcw->changePage.norm_fg!=newcw->changePage.norm_fg)
	{
	XtReleaseGC(curcw,curcw->changePage.clear_gc);
        XtReleaseGC(curcw,curcw->changePage.norm_gc);
        GetAllGCs(newcw);
	XtSetArg(comp_args[0],XmNforeground,newcw->changePage.norm_fg);
	XtSetValues(newcw,comp_args,1);
        do_redisplay = True;
        }
if(curcw->changePage.norm_bg!=newcw->changePage.norm_bg)
	{
	XtReleaseGC(curcw,curcw->changePage.clear_gc);
	XtReleaseGC(curcw,curcw->changePage.norm_gc);
	GetAllGCs(newcw);
	XtSetArg(comp_args[0],XmNbackground,newcw->changePage.norm_bg);
	XtSetValues(newcw,comp_args,1);
        do_redisplay = True;
        }
if(curcw->changePage.fill_button!=newcw->changePage.fill_button)
	{
	if(newcw->changePage.fill_button==DEFAULT_FILL)
                newcw->changePage.norm_bg=newcw->core.background_pixel;
	XtReleaseGC(curcw,curcw->changePage.clear_gc);
        XtReleaseGC(curcw,curcw->changePage.norm_gc);
        GetAllGCs(newcw);
	}
if(curcw->xlmanager.config!=newcw->xlmanager.config)
        {
        if(newcw->xlmanager.config==True)
                SettaActionsFigli(newcw);
        else
                {
                DistruggiFigli(newcw);
                CreaFigli(newcw);
                }
        do_redisplay = True;
        }
/*
Controlla 
*/
if(newcw->changePage.tipo==SYSTEM)  /*se bitmap*/
        if(strcmp(curcw->changePage.command,newcw->changePage.command))
                {
		strcpy(newcw->changePage.command,curcw->changePage.command);
		}

/*
Controlla che il bitmap non sia nullo, se draw=DRAW_BITMAP
*/
if(newcw->changePage.draw==DRAW_BITMAP || (newcw->changePage.draw==DRAW_GIF))  /*se bitmap*/
	if(strcmp(curcw->changePage.name_bmp,newcw->changePage.name_bmp))
        	{
/*
Libero la pixmap del vecchio widget e rialloco la nuova
*/
		XFreePixmap(XtDisplay(curcw),curcw->changePage.bitmap_norm);
		LoadBitmap(newcw);
		XtSetArg(argum[0],XtNwidth,newcw->changePage.button_width);     
		XtSetArg(argum[1],XtNheight,newcw->changePage.button_height);
		XtSetArg(argum[2],XmNlabelPixmap,(newcw->changePage.bitmap_norm));
		XtSetValues(newcw,argum,3);
        	do_redisplay = True;
                }
if(newcw->changePage.draw==DRAW_LABEL)  /* se label su bottone */
        if(strcmp(curcw->changePage.name_button,newcw->changePage.name_button))
                {
		str=XmStringCreateSimple(newcw->changePage.name_button);
		XtSetArg(argum[0],XmNlabelString,str);		
		XtSetValues(newcw,argum,1);
		XmStringFree(str);
		}

if(strcmp(curcw->changePage.normalfont,newcw->changePage.normalfont))
        {
        if(!LoadFont(newcw,newcw->changePage.normalfont))
        if(!LoadFont(newcw,"fixed"))
                XlError("XlChangePage","SetValues",
                        "Impossibile caricare default font");
        do_redisplay = True;
        }

/*
        risorsa width
*/
if(curcw->core.width != newcw->core.width)
	newcw->core.width=curcw->core.width;

/*
        risorsa height
*/
if(curcw->core.height != newcw->core.height)
	newcw->core.height=curcw->core.height;

/*
        risorsa width0
*/
if(curcw->xlmanager.width0 != newcw->xlmanager.width0)
	newcw->xlmanager.width0 = curcw->xlmanager.width0;

/*
        risorsa height0
*/
if(curcw->xlmanager.height0 != newcw->xlmanager.height0)
	newcw->xlmanager.height0 = curcw->xlmanager.height0;

return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlChangePageWidget cw= (XlChangePageWidget) w;

/*
	Rilascio i GC
*/
if(cw->changePage.norm_gc)
        XtReleaseGC(cw,cw->changePage.norm_gc);
if(cw->changePage.clear_gc)
        XtReleaseGC(cw,cw->changePage.clear_gc);

/* libero la font_struct */
if(cw->changePage.font_info)
        XFreeFont(XtDisplay(cw),cw->changePage.font_info);

/* libero la font_list  */
if(cw->changePage.font_list)
        XmFontListFree(cw->changePage.font_list);
/* libero la Pixmap */
if(cw->changePage.bitmap_norm)
        XFreePixmap(XtDisplay(cw),cw->changePage.bitmap_norm);

}

static Boolean Refresh(Widget w)
{
XlChangePageWidget cw= (XlChangePageWidget) w;

if(cw->xlmanager.config==True)
        return(True);

if(!XtIsWidget(w))
        {
        XlWarning("XlChangePage","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlChangePageClassRec.xlmanager_class.database==NULL)
        {
        XlWarning("XlChangePage","Refresh","database non definito");
        return(False);
        }
if(!(*superclass->xlmanager_class.refresh) (w))
        {
        XlWarning("XlSetVAlore","Refresh","errore refresh classe superiore");
        return(False);
        }
return(True);
}

static Boolean Compile(Widget w)
{
int i;

XlChangePageWidget cw= (XlChangePageWidget)w;

if(!XtIsWidget(w))
        {
        XlWarning("XlManager","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlChangePageClassRec.xlmanager_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
        return(False);
        }

if(!(*superclass->xlmanager_class.compile) (w,
     xlChangePageClassRec.xlmanager_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
        return(False);
        }

return(True);
}


static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlChangePageWidget cw= (XlChangePageWidget)w;
register int x,y;
unsigned int width,height;
XmDrawingAreaCallbackStruct str;
Arg args[3];

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

XClearArea(XtDisplay(cw),XtWindow(cw->changePage.bottone),0,0,0,0,True);
if(cw->changePage.text!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->changePage.text),0,0,0,0,True);


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
XlChangePageWidget Xl_cw = (XlChangePageWidget) cw;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
/* Carica il font ottenendo la descrizione del font stesso */
Display *disp;

char **pf;
int i,nf;
if((Xl_cw->changePage.font_info = FontFallbackLoad(XtDisplay(cw),fontname)) == NULL
)
        {
        XlWarning("XlChangePage","LoadFont","Cannot open font");
        return(False);
        }
        else
        {
        Xl_cw->changePage.font_list =
                XmFontListCreate ( Xl_cw->changePage.font_info, charset);
        if (Xl_cw->changePage.font_list == NULL)
                {
                XlWarning("XlChangePage","LoadFont","FONTLIST NULL");
                return(False);
                }
        }


return(True);
}

static void ActivateToggle(w,info,str)
Widget w;
XtPointer info,str;
/*caddr_t info;
XmToggleButtonCallbackStruct *str;*/
{
XlChangePageWidget cw= (XlChangePageWidget)XtParent(w);
char *pstring;
XlManagerClassRec * wclass; /* puntatore alla widget class del widget */

/*
Se il tipo e' System allora esegui la system
*/
if(cw->changePage.tipo==SYSTEM)
	{
	system(cw->changePage.command);
	return;
	}
/*
        Chiama la procedura selectRunTime, se e' stata settata
N.B.:
----
----
La select run time che interessa il changePage e' settata dall' mmi 
nel file csrc other_mmi.c .In SettaSelect viene settata con XlSetSelectRunTime
la funzione SelectRunTime come funzione-risorsa di XlchangePage.
In tale SelectRunTime viene fatto lo switch sul changePage.tipo per 
lanciare la funzione corretta.
*/

(*superclass->xlmanager_class.select) (cw,0);
}

extern LoadGif();
static Boolean LoadBitmap(w)
Widget w;
{
/*
Procedura per la lettura da file del bitmap che si vuole caricare.
Se il file contenente il bitmap cercato non esiste o non si riesce a
caricare il bitmap per qualunque altro motivo viene "ritornato" un valore
di fallimento che permettera' l' invocazione di LoadNullBitmap per 
caricamento bitmap di default
*/

XlChangePageWidget cw= (XlChangePageWidget)w;
int xh,yh;
int screen_num=DefaultScreen(XtDisplay(w));
int ret;		/*valore di ritorno*/
Arg args[2];
char path_bitmap[1024];
char *disegni_dir;
FILE *fp;
char _nome_file_bitmap[MAXLUNGBMP];
Pixmap pix;	/*pixmap di appoggio*/
Boolean IsGif=True;
if(cw->changePage.draw == DRAW_GIF)
	IsGif=True;
else
	IsGif=False;
/*
        concateno al nome del file l'estensione
*/
if(IsGif==True)
	sprintf(_nome_file_bitmap,"%s.gif",cw->changePage.name_bmp);
else
	sprintf(_nome_file_bitmap,"%s.bmp",cw->changePage.name_bmp);
/*
        verifico che cw->changePage.name_bmp contenga qualche cosa
*/
if(cw->changePage.name_bmp==NULL)
        {
        XlWarning("XlBitmap","LoadBitmap",
                "nome del file non definito");
        LoadNullBitmap(cw);
        return(False);
        }
/*
        verifico se il file si trova nella directory corrente
*/
if((fp=fopen(_nome_file_bitmap,"r"))!=NULL)
        {
        fclose(fp);
        strcpy(path_bitmap,_nome_file_bitmap);
        }
else
        {
	disegni_dir=getenv("LEGOMMI_ICO");	/*trovo la dir delle bitmap*/
	if(disegni_dir==NULL)
                {
                XlWarning("XlChangePage","LoadBitmap",
                        "impossibile leggere la variabili LEGOMMI_ICO");
                LoadNullBitmap(cw);
                return(False);
                }
	strcpy(path_bitmap,disegni_dir);
        strcat(path_bitmap,"/");
        strcat(path_bitmap,_nome_file_bitmap);
	 if((fp=fopen(path_bitmap,"r"))!=NULL)
                {
                fclose(fp);
                }
        else
                {
                XlWarning("XlChangePage","LoadBitmap",
                        "impossibile aprire il file in LEGOMMI_ICO--->");
                XlWarning("XlChangePage","LoadBitmap",
                        path_bitmap);
        	LoadNullBitmap(cw);
        	return(False);
		}
		
        }
if (!IsGif) /* caso di file bitmap */
	{
/*
Leggo dapprima il file bitmap e ricavo le dimensioni del bitmap
messe in button_width/heighti. Il bitmap e' posto nel pixmap di appoggio pix
*/
ret=XReadBitmapFile(XtDisplay(w),
                RootWindow(XtDisplay(w),DefaultScreen(XtDisplay(w))),
                path_bitmap,&(cw->changePage.button_width),
                &(cw->changePage.button_height),
                &pix,&xh,&yh);
if(ret==BitmapSuccess)
	pix=pix;
//	printf("Bitmap caricato correttamente\n");
else if(ret==BitmapOpenFailed)
        printf("Apertura fallita\n");
else if(ret==BitmapFileInvalid)
        printf("File bitmap non valido \n");
else if(ret==BitmapNoMemory)
        printf("No memory\n");
if(ret!=BitmapSuccess)
	{
	XlWarning("XlchangePage","LoadBitmap",
                "Impossibile leggere il file con XReadBitmap");
	return(False);
	}
/*
Ora creo il pixmap che verra' poi importato dal bottone.
Viene messo nella risorsa bitmap_norm
*/
cw->changePage.bitmap_norm=XCreatePixmap(XtDisplay(w), 
			RootWindow(XtDisplay(w),DefaultScreen(XtDisplay(w))),
			cw->changePage.button_width,
			cw->changePage.button_height,
			DefaultDepth(XtDisplay(w),DefaultScreen(XtDisplay(w))));

/*
Copio il pix ID (letta da file) nella risorsa bitmap_norm
*/
XCopyPlane(XtDisplay(w),pix,cw->changePage.bitmap_norm,
	cw->changePage.norm_gc,0,0,
	cw->changePage.button_width,cw->changePage.button_height,
	0, 0, 1);
/*
Libero il pixmap di appoggio
*/
XFreePixmap(XtDisplay(w),pix);
	}
else   /* caso di file .gif */
	{
	GC theGC= DefaultGC(XtDisplay(w),XScreenNumberOfScreen(XtScreen(w)));
	XImage *theImage;
	LoadGIF(w,path_bitmap,&theImage);
	cw->changePage.button_width = theImage->width;
	cw->changePage.button_height = theImage->height; 
	cw->changePage.bitmap_norm = XCreatePixmap (XtDisplay(w), 
					RootWindow(XtDisplay(w),DefaultScreen(XtDisplay(w))), 
					theImage->width,
                           		theImage->height,
                            (unsigned int)DefaultDepth(XtDisplay(w), 
				XScreenNumberOfScreen(XtScreen(w))));
	XPutImage(XtDisplay(w),cw->changePage.bitmap_norm,
			theGC,theImage,0,0,0,0,
			theImage->width,theImage->height);
	XDestroyImage(theImage);
	}
/*
Tutto OK
*/
return(True);
}

static void LoadNullBitmap(w)
Widget w;
{
/*
Funzione che carica comunque un bitmap di default (change_page.bmp) 
importato in una include
*/
XlChangePageWidget cw= (XlChangePageWidget)w;
Arg args[2];
int screen_num=DefaultScreen(XtDisplay(w));
char *bmpNameBits;
unsigned int bmpNameWidth,bmpNameHeight;

XlWarning("XlchangePage","LoadNullBitmap",
                "utilizzo i bitmaps di default");
/*
Carico il bitmap di default coerente con il tipo di bottone
*/
if(cw->changePage.tipo==CHANGE_PAGE)
	{
	bmpNameBits=change_page_bits;
	bmpNameWidth=change_page_width;
	bmpNameHeight=change_page_height;
	}
else if (cw->changePage.tipo==NEW_PAGE)
        {
        bmpNameBits=new_page_bits;
        bmpNameWidth=new_page_width;
        bmpNameHeight=new_page_height;
        }
else if (cw->changePage.tipo==CLOSE_PAGE)
        {
        bmpNameBits=close_page_bits;
        bmpNameWidth=close_page_width;
        bmpNameHeight=close_page_height;
        }
else if (cw->changePage.tipo==SYSTEM)
        {
        bmpNameBits=system_bits;
        bmpNameWidth=system_width;
        bmpNameHeight=system_height;
        }
else
        {
        bmpNameBits=nullo_bits;
        bmpNameWidth=nullo_width;
        bmpNameHeight=nullo_height;
        }
/*
uso XCreateBitmapFromData perche' il bitmap e' gia' stato caricato come
var globale includendo con include il .bmp
*/
if((cw->changePage.bitmap_norm=XCreatePixmapFromBitmapData(XtDisplay(w),
		 RootWindow(XtDisplay(w),DefaultScreen(XtDisplay(w))),
		bmpNameBits,
                bmpNameWidth,bmpNameHeight,
                cw->changePage.norm_fg,cw->changePage.norm_bg,
                DefaultDepth(XtDisplay(w),DefaultScreen(XtDisplay(w)))))== False)
		        {
	XlWarning("XlchangePage","LoadNullBitmap",
                "Errore nella creazione del pixmap");
        return;
        }

cw->changePage.button_width=change_page_width;
cw->changePage.button_height=change_page_height;

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

static void CreaFigli(tnew)
Widget tnew;
{
XlChangePageWidget new = (XlChangePageWidget)tnew;
int i,app_int;
Arg arg[25];
Dimension w_font,h_font,text_width,text_height;
Dimension bottone_width,bottone_height;
XmString str;

/*
Inizializzazione fittizia per 3rd
Le dimensioni sono poi settate automaticam.
*/
bottone_width=10;
bottone_height=10;

h_font=new->changePage.font_info->ascent+new->changePage.font_info->descent;
if(new->changePage.draw==DRAW_BITMAP || (new->changePage.draw==DRAW_GIF))
	{
	new->changePage.button_width=DEFAULT_WIDTH;
	new->changePage.button_height=DEFAULT_HEIGHT;
	if(!LoadBitmap(new))
        	{
        	XlWarning("XlChangePage","Initialize",
			"Carico il bitmap di default");
		}
	/*
	Riempio ampiezza e altezza con quelle ricavate in LoadBitmap
	*/
	bottone_width=new->changePage.button_width;
	bottone_height=new->changePage.button_height;
	}/*end if DRAW_BITMAP*/

else if(new->changePage.draw==DRAW_LABEL)  /*carica label*/
	{
	if(!strcmp(new->changePage.name_button,""))
		str=XmStringCreateSimple("Button");
	else		
		str=XmStringCreateSimple(new->changePage.name_button);	
	}
/*
        creazione widget bottone button
*/
i=0;
XtSetArg(arg[i],XmNx,3); i++;
XtSetArg(arg[i],XmNy,3); i++;

if(new->changePage.draw==DRAW_BITMAP || new->changePage.draw==DRAW_GIF) /*se carico bitmap*/
	{
	/*
	Setto le Dim del bottone in base a quellle del pixmap
	*/
	/*
	Setto ampiezza e altezza di core (come in ogni Initialize)
	in base alle dimensioni del pixmap e quindi del bottone
	*/
	new->core.width=bottone_width+6;
	new->core.height=bottone_height+6;
	XtSetArg(arg[i],XmNwidth,bottone_width); i++;
	XtSetArg(arg[i],XmNheight,bottone_height); i++;
	XtSetArg(arg[i],XmNspacing,0); i++;
	XtSetArg(arg[i],XmNlabelType,XmPIXMAP); i++;
	/*
	Carica pixmap sul bottone
	*/	
	if((new->changePage.bitmap_norm)!=NULL)
		XtSetArg(arg[i],XmNlabelPixmap,
			(new->changePage.bitmap_norm)); i++;
	}/*end if DRAW_BITMAP */
/*
Se carico il nome del bottone
*/
else if(new->changePage.draw==DRAW_LABEL) 
	{
	XtSetArg(arg[i],XmNlabelType,XmSTRING); i++;
	XtSetArg(arg[i],XmNlabelString, str); i++;
	XtSetArg(arg[i],XmNfontList,new->changePage.font_list); i++;
	XtSetArg(arg[i],XmNforeground,new->changePage.norm_fg); i++;
	}
/*
Calcola automaticamente le dimensioni del bottone in base a label
*/
XtSetArg(arg[i],XmNrecomputeSize,True); i++;
/*
Parte modificata introducendo nuova risorsa
*/
XtSetArg(arg[i],XmNbackground,new->changePage.norm_bg);i++;
XtSetArg(arg[i],XmNmarginWidth,0);i++;
XtSetArg(arg[i],XmNmarginHeight,0);i++;
XtSetArg(arg[i],XmNborderWidth,0);i++;

XtSetArg(arg[i],XmNnavigationType,XmNONE);i++;
XtSetArg(arg[i],XmNtraversalOn,FALSE); i++;
new->changePage.bottone=XmCreatePushButton(new,"__bottone",arg,i);
XtAddCallback(new->changePage.bottone,
                XmNactivateCallback,ActivateToggle,NULL);
XtManageChild(new->changePage.bottone);

new->xlmanager.width0=bottone_width;
new->xlmanager.height0=bottone_height;
/*
Libero la stringa Motif e calcolo le dimensioni di core
*/
if(new->changePage.draw==DRAW_LABEL)
	{
	XmStringFree(str);
/*
Leggo le dimensioni del bottone e setto le dimensioni di core
*/
	XtSetArg(arg[0],XmNwidth,&bottone_width);
	XtSetArg(arg[1],XmNheight,&bottone_height);
	XtGetValues(new->changePage.bottone,arg,2);
/*
	Setto l' altezza di core
	*/
	new->core.width=bottone_width+6;
	new->core.height=bottone_height+6;
	}
/*
        creazione text widget
*/
if(strcmp(new->changePage.label,"")) /* setto la heigth di core per text */
	new->core.height=bottone_height+6+h_font;

h_font=new->changePage.font_info->ascent+new->changePage.font_info->descent;
w_font=new->changePage.font_info->max_bounds.width;
if(strcmp(new->changePage.label,""))
{
   i=0;
   XtSetArg(arg[i],XmNx,3); i++;
   XtSetArg(arg[i],XmNy,bottone_height+3); i++;
   XtSetArg(arg[i],XmNwidth,new->core.width); i++;
   XtSetArg(arg[i],XmNheight,new->core.height-bottone_height-6); i++;
   XtSetArg(arg[i],XmNmaxLength,10); i++;
   XtSetArg(arg[i],XmNborderWidth,0); i++;
   XtSetArg(arg[i],XmNresizeHeight,False); i++;
   XtSetArg(arg[i],XmNresizeWidth,False); i++;
   XtSetArg(arg[i],XmNfontList,new->changePage.font_list); i++;
   XtSetArg(arg[i],XmNshadowThickness,0); i++;
   XtSetArg(arg[i],XmNmarginWidth,0); i++;
   XtSetArg(arg[i],XmNmarginHeight,0); i++;
   XtSetArg(arg[i],XmNforeground,new->changePage.norm_fg); i++;
   XtSetArg(arg[i],XmNbackground,new->core.background_pixel);i++;
   XtSetArg(arg[i],XmNlabelString,
		XmStringCreateLtoR
		(new->changePage.label,XmSTRING_DEFAULT_CHARSET));i++;
   new->changePage.text= XmCreateLabel(new,"__text",arg,i);
   XtManageChild(new->changePage.text);
}
else
	new->changePage.text=NULL;
}

static void DistruggiFigli(tnew)
Widget tnew;
{
XlChangePageWidget new = (XlChangePageWidget)tnew;

if(new->changePage.text!=NULL)
        XtDestroyWidget(new->changePage.text);
if(new->changePage.bottone!=NULL)
        XtDestroyWidget(new->changePage.bottone);

new->changePage.text=NULL;
new->changePage.bottone=NULL;

}

static void SettaActionsFigli(tnew)
Widget tnew;
{
XlChangePageWidget new = (XlChangePageWidget)tnew;

if(new->xlmanager.config==True)
        {
	if(new->changePage.text!=NULL)
        	add_translation(new->changePage.text,textTranslations);
	if(new->changePage.bottone!=NULL)
        	add_translation(new->changePage.bottone,buttonTranslations);
        }
}


/* XlIsChangePage(w) restituisce True se lo widget 
   e' della classe ChangePage
*/
Boolean XlIsChangePage( Widget w)
{
   CoreWidgetClass wclass;
   wclass=XtClass(w);

   if(strcmp(wclass->core_class.class_name,"ChangePage")==0)
        return(True);
   else
        return(False);
}

/*        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
return(XlHardCopyWidget(w));
}
#endif
