/**********************************************************************
*
*       C Source:               XlLabel.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Mon Feb 23 14:16:50 2004 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlLabel.c-3 %  (%full_filespec: XlLabel.c-3:csrc:1 %)";
#endif
/*
   modulo XlLabel.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlLabel.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlLabel.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlLabel.c - widget label per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlLabelP.h>
#include <Xl/XlDrawingUtil.h>
#include <pagresdef.h>
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
        XtOffsetOf(XlLabelRec,label.norm_fg),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNtipoLabel,
        XlCTipoLabel,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlLabelRec,label.tipo_label),
        XmRImmediate,
        (XtPointer)LABEL_NORM
        },
	{
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlLabelRec,label.normalfont),
        XmRImmediate,
        "fixed"
        },
	{
        XlNlabelText,
        XlCLabelText,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlLabelRec,label.labeltext),
        XmRImmediate,
        "label"
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
        XtOffsetOf(XlLabelRec,label.norm_fg),
        sizeof(Pixel),
        },
        {
        XlNtipoLabel,
        XlCTipoLabel,
        XlDtipoLabel,
        XlROption,
        NULL,
        XlOtipoLabel,
        XlRGrIo,
        XtOffsetOf(XlLabelRec,label.tipo_label),
        sizeof(int)
        },
        {
        XlNnormalFont,
        XlCNormalFont,
        XlDnormalFont,
        XlRFont,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlLabelRec,label.normalfont),
        sizeof(XFontStruct*)
        },
        {
        XlNlabelText,
        XlCLabelText,
        XlDlabelText,
        XlRText,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlLabelRec,label.labeltext),
        sizeof(char*)
        },
	};
/* dichiarazioni funzioni varie */
static void GetNormFgGC();
static void GetClearGC();
static void GetAllGCs();
static Boolean LoadFont();
static void DrawIntoLabel();
static void DrawLabel();
static void RefreshTesto();
         
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
XlLabelClassRec xlLabelClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "Label",
    /* widget_size              */      sizeof(XlLabelRec),
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
  { /* bitmap fields */
    /* empty                    */      0
  }
};

WidgetClass xlLabelWidgetClass = (WidgetClass) &xlLabelClassRec;


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlLabelWidget new = (XlLabelWidget)tnew;
int app_int;
int zoom;

RefreshTesto(new);


if(new->core.width!=new->core.height)
        {
	if((new->xlcore.rotate==ROTATE_90)||
		(new->xlcore.rotate==ROTATE_270))
			{
			app_int = new->core.width;
			new->core.width = new->core.height;
			new->core.height = app_int;
			}
	}



if(new->xlcore.rotate>3)
	{
	XlWarning("XlLabel","Initialize","rotate > 2");
	new->xlcore.rotate=new->xlcore.rotate%4;
	}
if(new->xlcore.ass_rotate>1)
	{
	XlWarning("XlLabel","Initialize","assRotate > 1");
	new->xlcore.ass_rotate=new->xlcore.ass_rotate%2;
	}

if(!LoadFont(new,new->label.normalfont,False))
	if(!LoadFont(new,"fixed",False))
		XlError("XlLabel","Initialize","Impossibile caricare default font");

/*
	calcola la width  e la height
*/
if(new->label.tipo_label!=LABEL_BIG)
	{
	new->core.height=new->label.font_info->ascent+
			new->label.font_info->descent;
	new->core.width=XTextWidth(new->label.font_info,
		new->label.labeltext,strlen(new->label.labeltext));
	if(XlIsXlComposite(XtParent(new)))
                {
                zoom=XlGetZoom(XtParent(new));
                new->core.width = new->core.width * zoom/100;
                new->core.height = new->core.height * zoom/100;
                }
	new->xlcore.rotate=0;
	new->xlcore.ass_rotate=0;
	}

if(new->core.width<DEFAULTWIDTH)
	{
        new->core.width=DEFAULTWIDTH;
	XlWarning("XlLabel","Initialize",
	     "width minore della dimensione minima");
	}
if(new->core.height<DEFAULTHEIGHT)
	{
        new->core.width=DEFAULTWIDTH;
	XlWarning("XlLabel","Initialize",
		"height minore della dimensione minima");
	}
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
XlLabelWidget curcw= (XlLabelWidget) current;
XlLabelWidget newcw= (XlLabelWidget) new;
Boolean do_redisplay = False;
Arg comp_args[2];
int label_w,label_h;


if(curcw->label.tipo_label!=LABEL_BIG)
	{
	if(curcw->xlcore.rotate != newcw->xlcore.rotate)
		newcw->xlcore.rotate=curcw->xlcore.rotate;
	if(curcw->xlcore.ass_rotate != newcw->xlcore.ass_rotate)
		newcw->xlcore.ass_rotate=curcw->xlcore.ass_rotate;
	}

if(curcw->label.norm_fg != newcw->label.norm_fg)
        {
        XtReleaseGC(curcw,curcw->label.norm_gc);
        XtReleaseGC(curcw,curcw->label.clear_gc);
        GetNormFgGC(newcw);
        GetClearGC(newcw);
        do_redisplay = True;
        }

if(strcmp(curcw->label.normalfont,newcw->label.normalfont))
        {
        if(!LoadFont(newcw,newcw->label.normalfont),False)
        if(!LoadFont(newcw,"fixed"),False)
                XlError("XlLabel","SetValues",
                        "Impossibile caricare default font");
	if(newcw->label.tipo_label!=LABEL_BIG)
		{
		label_h=newcw->label.font_info->ascent+
				newcw->label.font_info->descent;
		label_w=XTextWidth(newcw->label.font_info,
			newcw->label.labeltext,strlen(newcw->label.labeltext));
		XtSetArg(comp_args[0],XmNwidth,label_w);
		XtSetArg(comp_args[1],XmNwidth,label_h);
		XtSetValues(newcw,comp_args,2);
		}
        do_redisplay = True;
        }

if(curcw->label.labeltext !=  newcw->label.labeltext)
	{
	if(newcw->label.tipo_label!=LABEL_BIG)
		{
		label_h=newcw->label.font_info->ascent+
				newcw->label.font_info->descent;
		label_w=XTextWidth(newcw->label.font_info,
			newcw->label.labeltext,strlen(newcw->label.labeltext));
		XtSetArg(comp_args[0],XmNwidth,label_w);
		XtSetArg(comp_args[1],XmNwidth,label_h);
		XtSetValues(newcw,comp_args,2);
		}
        do_redisplay = True;
	}

if(curcw->label.tipo_label !=  newcw->label.tipo_label)
	{
        do_redisplay = True;
	}

return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlLabelWidget cw= (XlLabelWidget) w;


/* rilascio i 2 GC */
if(cw->label.norm_gc)
	XtReleaseGC(cw,cw->label.norm_gc);

if(cw->label.clear_gc)
	XtReleaseGC(cw,cw->label.clear_gc);

/* libero la font_struct */
/*
if(cw->label.font_info)
	XFreeFont(XtDisplay(cw),cw->label.font_info);
*/

}

static void GetNormFgGC(w,modo)
Widget w;
Boolean modo;
{
XGCValues values;
XlLabelWidget cw= (XlLabelWidget) w;

unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;


values.line_width = 1;
values.line_style = LineSolid;
#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cw->label.norm_fg);
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->label.norm_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cw->label.norm_fg;
	values.background = cw->core.background_pixel;
        cw->label.norm_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->label.norm_fg;
values.background = cw->core.background_pixel;
cw->label.norm_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetClearGC(w,modo)
Widget w;
Boolean modo;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlLabelWidget cw= (XlLabelWidget) w;
values.line_width = 0;
values.line_style = LineSolid;
#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
	values.background = XlPrintGetPixel(w,cw->label.norm_fg);
        cw->label.clear_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cw->core.background_pixel;
	values.background = cw->label.norm_fg;
        cw->label.clear_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->core.background_pixel;
values.background = cw->label.norm_fg;
cw->label.clear_gc = XtGetGC(cw, valuemask, &values);
#endif
}


static void GetAllGCs(w,modo)
Widget w;
Boolean modo;
{
GetNormFgGC(w,modo);
GetClearGC(w,modo);
}


static void DrawIntoLabel(w,modo)
Widget w;
Boolean modo;
{
XlLabelWidget cw= (XlLabelWidget)w;

/*
	se necessario sbianca lo sfondo
*/
if((cw->xlcore.trasparent!=True) && !modo)
   XlRiempiRettangolo(cw,XlWindow(cw),cw->label.clear_gc,0,0,100,100);
	
DrawLabel(w);
	
}




static Boolean Refresh(Widget w)
{
XlLabelWidget cw= (XlLabelWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;
int core_x=cw->core.x;
int core_y=cw->core.y;


if(!XtIsWidget(w))
	{
	XlWarning("XlLabel","Refresh",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}

return(True);
}


static void Resize(w)
Widget w;
{
XlLabelWidget cw= (XlLabelWidget)w;

DrawIntoLabel(cw,False);
}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlLabelWidget cw= (XlLabelWidget)w;
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

DrawIntoLabel(w,False);

/*
        Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
*/
(*superclass->xlcore_class.drawBorderIfSelected) (w);

}



static Boolean LoadFont( cw,fontname,modo)
Widget cw;
char fontname[];
Boolean modo;
{
XlLabelWidget Xl_cw = (XlLabelWidget) cw;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
char ** pippo;
int actual_count, j;
/* Carica il font ottenendo la descrizione del font stesso */

char **pf;
int i,nf;
Display *disp;
/* Caso di stampa */
if(modo)
	{
        disp=XlDisplay(cw);
printf("Stampa: fontname=%s \n", fontname);

 



pippo=XListFonts(disp, "*", 1000, &actual_count);
if (pippo != NULL)
{
 printf ( "actual_count=%d\n",actual_count);
for ( j=0 ; j< actual_count ; j++)
    {
    printf ( "Fontname[%d]=%s \n", j, pippo[j]);
    }
}
else
{
printf ( "Pippo NULL \n");
}
printf ( "dopo Pippo \n");


	if((Xl_cw->label.font_info = XLoadQueryFont(disp,fontname)) == NULL)
        	{
printf("Stampa: falses \n");
		return(False);
        	}
printf("Stampa: true \n");
	return(True);
	}
else /* caso di display su video */
	{
        disp=XtDisplay(cw);
//printf("Video: fontname=%s \n", fontname);
	if((Xl_cw->label.font_info = XlGetFont(disp,fontname)) == NULL)
        	{
		return(False);
        	}
	return(True);
	}
}



static Boolean Compile(Widget w)
{
XlLabelWidget cw= (XlLabelWidget)w;

if(!XtIsWidget(w))
	{
	XlWarning("XlLabel","Compile",
		"il parametro in ingresso non e' un Widget");
	return(False);
	}
if(xlLabelClassRec.xlcore_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
        return(False);
        }

if(!(*superclass->xlcore_class.compile) (w,
     xlLabelClassRec.xlcore_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
        return(False);
        }
return(True);
}

static void DrawLabel(w)
Widget w;
{
XlLabelWidget cw= (XlLabelWidget)w;
char *c=cw->label.labeltext;
int nchar=0;
int i;
int step;
char app_t[10];
int font_height;


if(cw->label.tipo_label!=LABEL_BIG)
	XlTestoSimple(cw,XlWindow(cw),cw->label.norm_gc,
                        0,50,c, cw->label.font_info);


if(cw->label.tipo_label==LABEL_BIG)
	{
	while(c[nchar]!=0)
		nchar++;
	step=90/nchar;
	for(i=0;i<nchar;i++)
		{
		sprintf(app_t,"%c",c[i]);
		XlTesto(cw,XlWindow(cw),cw->label.norm_gc,
                	10+step*i,50,app_t, cw->label.font_info);
		}
	}
}

static void RefreshTesto(Widget w)
{
XlLabelWidget cw= (XlLabelWidget)w;
char *tipo=NULL;
XrmValue value;
Boolean retval;
XrmDatabase db;
int use;


if((cw->label.tipo_label==LABEL_BIG)||(cw->label.tipo_label==LABEL_NORM))
	return;

db=XrmGetDatabase(XtDisplay(XtParent(w)));
/*
if(cw->label.labeltext!=NULL)
        XtFree(cw->label.labeltext);
*/

if(cw->label.tipo_label==LABEL_NOME_PAGINA)
	{
	retval = XrmGetResource(db, XlNnomePag, XlCNomePag, &tipo, &value);
	if(retval && value.size>1)
		{
		cw->label.labeltext=XtMalloc(value.size);
		strncpy(cw->label.labeltext,value.addr,value.size);
		}
	else
	        cw->label.labeltext=XtNewString("Page's name not found");
	}
if(cw->label.tipo_label==LABEL_DESCR_PAGINA_1)
        {
        retval = XrmGetResource(db, XlNdescrPag, XlCDescrPag, &tipo, &value);
        if(retval && value.size>1)
                {
                cw->label.labeltext=XtMalloc(value.size);
                strncpy(cw->label.labeltext,value.addr,value.size);
                }
        else
		cw->label.labeltext=XtNewString("Page's description 1 not found");
 	}
if(cw->label.tipo_label==LABEL_DESCR_PAGINA_2)
        {
        retval = XrmGetResource(db, XlNdescrPag_2, XlCDescrPag_2, 
		&tipo, &value);
        if(retval && value.size>1)
                {
                cw->label.labeltext=XtMalloc(value.size);
                strncpy(cw->label.labeltext,value.addr,value.size);
                }
        else
		cw->label.labeltext=XtNewString("Page's description 2 not found");
 	}
if(cw->label.tipo_label==LABEL_DESCR_PAGINA_3)
        {
        retval = XrmGetResource(db, XlNdescrPag_3, XlCDescrPag_3, 
		&tipo, &value);
        if(retval && value.size>1)
                {
                cw->label.labeltext=XtMalloc(value.size);
                strncpy(cw->label.labeltext,value.addr,value.size);
                }
        else
		cw->label.labeltext=XtNewString("Page's description 3 not found");
 	}
if(cw->label.tipo_label==LABEL_DESCR_PAGINA_4)
        {
        retval = XrmGetResource(db, XlNdescrPag_4, XlCDescrPag_4, 
		&tipo, &value);
        if(retval && value.size>1)
                {
                cw->label.labeltext=XtMalloc(value.size);
                strncpy(cw->label.labeltext,value.addr,value.size);
                }
        else
		cw->label.labeltext=XtNewString("Page's description 4 not found");
 	}
if(cw->label.tipo_label==LABEL_DATE_COMPILE)
        {
        retval = XrmGetResource(db, XlNcompilationDate, XlCCompilationDate, 
		&tipo, &value);
        if(retval && value.size>1)
                {
                cw->label.labeltext=XtMalloc(value.size);
                strncpy(cw->label.labeltext,value.addr,value.size);
                }
        else
		cw->label.labeltext=XtNewString("00/00/00 00:00:00");
 	}
if(cw->label.tipo_label==LABEL_DATE_REG)
        {
        retval = XrmGetResource(db, XlNregolationDate, XlCRegolationDate, 
		&tipo, &value);
        if(retval && value.size>1)
                {
                cw->label.labeltext=XtMalloc(value.size);
                strncpy(cw->label.labeltext,value.addr,value.size);
                }
        else
		cw->label.labeltext=XtNewString("00/00/00 00:00:00");
 	}

if(cw->label.tipo_label==LABEL_TAG_REG)
        {
        retval = XrmGetResource(db, XlNtagPag, XlCTagPag, &tipo, &value);
        if(retval && value.size>1)
                {
                cw->label.labeltext=XtMalloc(value.size);
                strncpy(cw->label.labeltext,value.addr,value.size);
                }
        else
		cw->label.labeltext=XtNewString("Tag not found");
 	}

if(cw->label.tipo_label==LABEL_IN_USE)
        {
        retval = XrmGetResource(db, XlNschemeInUse, XlCSchemeInUse, &tipo, &value);
        if(retval && value.size>1)
                {
		use=atoi(value.addr);
		if(use==1)
			cw->label.labeltext=XtNewString("in use");
		else
			cw->label.labeltext=XtNewString("not in use");
                }
        else
		cw->label.labeltext=XtNewString("Scheme in use not found");
 	}
}


/*
        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
XlLabelWidget cw= (XlLabelWidget) w;
char *printerFont;

(*superclass->xlcore_class.print) (w);

printerFont=XtNewString(XlPrinterGetFont(w,
	cw->label.normalfont,cw->label.font_info));

/* rilascio il font */
/*
if(cw->label.font_info)
        XFreeFont(XtDisplay(cw),cw->label.font_info);
*/
/* rilascio i 2 GC */
if(cw->label.norm_gc)
	XtReleaseGC(cw,cw->label.norm_gc);
if(cw->label.clear_gc)
	XtReleaseGC(cw,cw->label.clear_gc);

if(!LoadFont(cw,printerFont,True))
        LoadFont(cw,"fixed",True);
XtFree(printerFont);
GetAllGCs(cw,True);

DrawIntoLabel(w,True);

/* rilascio il font della stampante */
if(cw->label.font_info)
        XFreeFont(XlDisplay(cw),cw->label.font_info);
/* rilascio i 2 GC della stampante */
if(cw->label.norm_gc)
	XFreeGC(XlDisplay(w),cw->label.norm_gc);
if(cw->label.clear_gc)
	XFreeGC(XlDisplay(w),cw->label.clear_gc);


if(!LoadFont(cw,cw->label.normalfont,False))
        LoadFont(cw,"fixed",False);
GetAllGCs(cw,False);

return(True);
}
#endif
