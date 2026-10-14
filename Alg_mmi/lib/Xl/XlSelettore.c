/**********************************************************************
*
*       C Source:               XlSelettore.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:30:47 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlSelettore.c-3.2.3 %  (%full_filespec: XlSelettore.c-3.2.3:csrc:1 %)";
#endif
/*
   modulo XlSelettore.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlSelettore.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlSelettore.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlSelettore.c - widget selettore per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlSelettoreP.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

#define DEFAULTWIDTH 11
#define DEFAULTHEIGHT 11

static int min();

/* lista delle risorse  */
static XtResource resources[]= {
        {
        XlNnormFg,
        XlCNormFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlSelettoreRec,selettore.norm_fg),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNselettoreFg,
        XlCSelettoreFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlSelettoreRec,selettore.selettore_fg),
        XmRString,
        "white"
        },
        {
        XlNnormBg,
        XlCNormBg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlSelettoreRec,selettore.norm_bg),
        XmRString,
        XtDefaultBackground
        }, 
        {
        XlNtipoSel,
        XlCTipoSel,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlSelettoreRec,selettore.tipo_sel),
        XmRImmediate,
        (XtPointer)SELETTORE
        },
	{
	XlNvarOutput,
	XlCVarOutput,
	XmRString,
	sizeof(char *),
	XtOffsetOf(XlSelettoreRec,selettore.str_output),
	XmRImmediate,
	""
	},
	{
	XlNvarInput,
	XlCVarInput,
	XmRString,
	sizeof(char *),
	XtOffsetOf(XlSelettoreRec,selettore.str_input),
	XmRImmediate,
	""
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
        XtOffsetOf(XlSelettoreRec,selettore.norm_fg),
        sizeof(Pixel),
        },
        {
        XlNselettoreFg,
        XlCSelettoreFg,
	XlDselettoreFg,
	XlRColor,
	NULL,
	NULL,
        XlRGrColor,
        XtOffsetOf(XlSelettoreRec,selettore.selettore_fg),
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
        XtOffsetOf(XlSelettoreRec,selettore.norm_bg),
        sizeof(Pixel),
        }, 
        {
        XlNtipoSel,
        XlCTipoSel,
	XlDtipoSel,
	XlROption,
	NULL,
	XlOtipoSel,
	XlRGrIo,
        XtOffsetOf(XlSelettoreRec,selettore.tipo_sel),
        sizeof(int)
        },
	{
	XlNvarOutput, 
	XlCVarOutput,
        XlDvarOutput,
	XlRTipoVarOut,
        XlRVarDO,
	NULL,
	XlRGrIo,
        XtOffsetOf(XlSelettoreRec,selettore.out),
	sizeof(PUNT_VAROUT)
	},
	{
	XlNvarInput, 
	XlCVarInput,
        XlDvarInput,
	XlRTipoVarInp,
        XlRVarDX,
	NULL,
	XlRGrIo,
        XtOffsetOf(XlSelettoreRec,selettore.input),
	sizeof(PUNT_VARINP)
	}
        };

/* dichiarazioni funzioni varie */


         
/* dichiarazione dei metodi (methods) */

static void Initialize();
static void Redisplay();
static void Destroy();
static void Resize(); 
static Boolean Refresh();
static Boolean Compile();
#if defined XPRINTER_USED
static Boolean Print();
#endif

static Boolean SetValues();
static XtGeometryResult QueryGeometry();

/* actions del widget XlSelettore */
static void Seleziona();
static void AddSelez();
static void DrawSelAct();

/* translations  */
static char defaultTranslations[]=
	"~Shift<Btn1Down>:     DrawSelAct() Seleziona()   \n\
	Shift<Btn1Down>: AddSelez()";

static XtActionsRec actions[] = {
        {"DrawSelAct",DrawSelAct},
	{"Seleziona",Seleziona},
	{"AddSelez",AddSelez}
        };
/* Inizializzazione del class record */
XlSelettoreClassRec xlSelettoreClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "Selettore",
    /* widget_size              */      sizeof(XlSelettoreRec),
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
    /* query_geometry           */      QueryGeometry,
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
    /* refresh                  */	Refresh,
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

WidgetClass xlSelettoreWidgetClass = (WidgetClass) &xlSelettoreClassRec;

static void GetNormFgGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlSelettoreWidget cw= (XlSelettoreWidget) w;
values.foreground = cw->selettore.norm_fg;
values.background = cw->core.background_pixel;
values.line_width = 0;
values.line_style = LineSolid;
cw->selettore.norm_gc = XtGetGC(cw,
                                valuemask,
                                &values);
}

static void GetSelettoreFgGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlSelettoreWidget cw= (XlSelettoreWidget) w;
values.foreground = cw->selettore.selettore_fg;
values.background = cw->core.background_pixel;
values.line_width = 0;
values.line_style = LineSolid;
cw->selettore.selettore_gc = XtGetGC(cw,
                                valuemask,
                                &values);
}

static void GetNormBgGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlSelettoreWidget cw= (XlSelettoreWidget) w;

values.foreground = cw->selettore.norm_bg;
values.background = cw->core.background_pixel;
values.line_width = 0;
values.line_style = LineSolid;
cw->selettore.norm_bg_gc = XtGetGC(cw,
                                valuemask,
                                &values);
}
                                          
static void GetClearGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlSelettoreWidget cw= (XlSelettoreWidget) w;
values.foreground = cw->core.background_pixel;
values.background = cw->selettore.norm_fg;
values.line_width = 0;
values.line_style = LineSolid;
cw->selettore.clear_gc = XtGetGC(cw,
                                valuemask,
                                &values);
}

static void GetAllGCs(w)
Widget w;
{
GetNormFgGC(w);
GetSelettoreFgGC(w);
GetNormBgGC(w);
GetClearGC(w);
}

static void DrawInto(w)
Widget w;
{
XlSelettoreWidget cw= (XlSelettoreWidget)w;
int delta;
int width,height;

if(cw->xlcore.trasparent==False)
        XFillRectangle(XtDisplay(cw),XtWindow(cw),
                cw->selettore.clear_gc,0,0,cw->core.width,cw->core.height);

if(cw->selettore.tipo_sel==SELETTORE)
	{
	XlRiempiArco(cw,XtWindow(cw),cw->selettore.selettore_gc,
			1,1,98,98,0,360*64);
	XlArco(cw,XtWindow(cw),cw->selettore.norm_gc,1,1,98,98,0,360*64);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,27,17,83,73);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,83,73,73,83);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,27,17,17,27);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,17,27,73,83);
	}
if(cw->selettore.tipo_sel==SELETTORE_CON_CHIAVE)
	{
	XlRiempiArco(cw,XtWindow(cw),cw->selettore.selettore_gc,
			1,1,98,98,0,360*64);
	XlArco(cw,XtWindow(cw),cw->selettore.norm_gc,1,1,98,98,0,360*64);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,27,17,83,73);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,83,73,73,83);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,27,17,17,27);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,17,27,73,83);
	}

}



/*
 Disegna il selettore ruotato 
*/
static void DrawIntoSelected(w)
Widget w;
{
XlSelettoreWidget cw= (XlSelettoreWidget)w;

if(cw->xlcore.trasparent==False)
        XFillRectangle(XtDisplay(cw),XtWindow(cw),
                cw->selettore.clear_gc,0,0,cw->core.width,cw->core.height);

if(cw->selettore.tipo_sel==SELETTORE)
	{
	XlRiempiArco(cw,XtWindow(cw),cw->selettore.selettore_gc,
			1,1,98,98,0,360*64);
	XlArco(cw,XtWindow(cw),cw->selettore.norm_gc,1,1,98,98,0,360*64);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,73,17,17,73);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,17,73,27,83);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,73,17,83,27);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,83,27,27,83);
	}
if(cw->selettore.tipo_sel==SELETTORE_CON_CHIAVE)
	{
	XlRiempiArco(cw,XtWindow(cw),cw->selettore.norm_gc,
			1,1,98,98,0,360*64);
	XlArco(cw,XtWindow(cw),cw->selettore.norm_gc,1,1,98,98,0,360*64);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,73,17,17,73);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,17,73,27,83);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,73,17,83,27);
	XlLinea(cw,XtWindow(cw),cw->selettore.norm_gc,83,27,27,83);
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

static void DrawSelAct(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
XlSelettoreWidget cw= (XlSelettoreWidget)w;

/*
 Se in fase di configurazione il selettore non esegue alcuna
 operazione alla sua attivazione
*/
if(cw->xlcore.config) return;
/*
	registra la pressione del selettore
*/
cw->selettore.premuto=!cw->selettore.premuto;

Redisplay(cw,NULL);
printf("pert up ind %d\n", cw->selettore.out.pdb.indice);
(olPertClassRec.olpert_class.pertsend)(xlSelettoreClassRec.xlcore_class.pert,
                        cw->selettore.out);
}


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlSelettoreWidget new = (XlSelettoreWidget)tnew;

if(new->core.width<DEFAULTWIDTH)
	new->core.width=DEFAULTWIDTH;
if(new->core.height<DEFAULTHEIGHT)
	new->core.height=DEFAULTHEIGHT;
if(new->core.width!=new->core.height)
	{
	new->core.width=min(new->core.width,new->core.height);
	new->core.height=new->core.width;
	}

new->selettore.premuto=False;
new->selettore.old_premuto=True;
GetAllGCs(new);
}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlSelettoreWidget cw= (XlSelettoreWidget)w;
register int x,y;
unsigned int width,height;
XExposeEvent evento_nullo;

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
	evento_nullo.x=0;
	evento_nullo.y=0;
	evento_nullo.width=width;
	evento_nullo.height=height;
	event= &evento_nullo;
        }

(*superclass->core_class.expose) (w,event,NULL);

if(cw->selettore.premuto)
	DrawIntoSelected(cw);
else
	DrawInto(cw);

/*
        Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
*/
(*superclass->xlcore_class.drawBorderIfSelected) (w);
}

static void Resize(w)
Widget w;
{
XlSelettoreWidget cw= (XlSelettoreWidget)w;
if(cw->core.width<DEFAULTWIDTH)
	cw->core.width=DEFAULTWIDTH;
if(cw->core.width!=cw->core.height)
	{
	cw->core.height=min(cw->core.width,cw->core.height);
	cw->core.width=cw->core.height;
	}
DrawInto(cw);
}

static Boolean Refresh(Widget w)
{
XlSelettoreWidget cw= (XlSelettoreWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;
XExposeEvent evento_nullo;


if(cw->xlcore.config==True)
        return(True);

if(!XtIsWidget(w))
        {
        XlWarning("XlSelettore","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlSelettoreClassRec.xlcore_class.database==NULL)
        {
        XlWarning("XlSelettore","Refresh","database non definito");
        return(False);
        }
if(!(*superclass->xlcore_class.refresh) (w))
        {
        XlWarning("XlSelettore","Refresh","errore refresh classe superiore");
        return(False);
        }


if(cw->selettore.old_premuto!=cw->selettore.premuto)
	{
	evento_nullo.x=0;
	evento_nullo.y=0;
	evento_nullo.width=core_width;
	evento_nullo.height=core_height;
	(*superclass->core_class.expose) (w,&evento_nullo,NULL);
	
	if(cw->selettore.premuto)
		DrawIntoSelected(cw);
	else
		DrawInto(cw);
	
	cw->selettore.old_premuto=cw->selettore.premuto;
	}
cw->selettore.premuto=cw->selettore.input.valore;

return(True);
}

static Boolean Compile(Widget w)
{
Boolean errors=False;
XlSelettoreWidget cw= (XlSelettoreWidget)w;

if(!XtIsWidget(w))
        {
        XlWarning("XlSelettore","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(strlen(cw->selettore.str_input)==NULL)
        {
	XlErrComp(w,"Compile","Input variable not defined",NULL);
        errors=True;
        }
if(strlen(cw->selettore.str_output)==NULL)
        {
	XlErrComp(w,"Compile","Output variable not defined",NULL);
        errors=True;
        }

if(xlSelettoreClassRec.xlcore_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Input variable not defined",NULL);
        XlEvError(cw,True);
        return(False);
        }

if(!(*superclass->xlcore_class.compile) (w,
     xlSelettoreClassRec.xlcore_class.compilatore))
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
        XlEvError(cw,True);
        return(False);
        }
XlEvError(cw,errors);
return(True);
}




static Boolean Read(w,fp)
Widget w;
FILE *fp;
{
printf("\n richiamato metodo di read selettore");
return(True);
}

static Boolean Write(w,fp)
Widget w;
FILE *fp;
{
printf("\n richiamato metodo di write selettore");
return(True);
}

static min(a,b)
int a,b;
{
return((a<b)? a:b);
}

static XtGeometryResult QueryGeometry(w,proposed,answer)
Widget w;
XtWidgetGeometry *proposed,*answer;
{
XlSelettoreWidget cw= (XlSelettoreWidget)w;
/* setta i campi di interesse */
answer->request_mode= CWWidth | CWHeight;
/* provvisorio */
answer->width=20;
answer->height=20;
if((proposed->request_mode & (CWWidth | CWHeight)) &&
    (proposed->width == answer->width &&
     proposed->height==answer->height))
	return XtGeometryYes;
else if (answer->width == cw->core.width &&
         answer->height == cw->core.height)
	return XtGeometryNo;
else
	return XtGeometryAlmost;
}


static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
XlSelettoreWidget curcw= (XlSelettoreWidget) current;
XlSelettoreWidget newcw= (XlSelettoreWidget) new;
Boolean do_redisplay = False;

if(curcw->selettore.norm_fg != newcw->selettore.norm_fg)
	{
	XtReleaseGC(curcw,curcw->selettore.norm_gc);
	XtReleaseGC(curcw,curcw->selettore.clear_gc);
	GetNormFgGC(newcw);
	GetClearGC(newcw);
	do_redisplay = True;
	}

if(curcw->selettore.selettore_fg != newcw->selettore.selettore_fg)
	{
	XtReleaseGC(curcw,curcw->selettore.selettore_gc);
	GetSelettoreFgGC(newcw);
	do_redisplay = True;
	}

if(curcw->selettore.norm_bg != newcw->selettore.norm_bg)
        {
        XtReleaseGC(curcw,curcw->selettore.norm_bg_gc);
        GetNormBgGC(newcw);
        do_redisplay = True;
        }

if(curcw->selettore.tipo_sel != newcw->selettore.tipo_sel)
	{
	do_redisplay = True;
	}

/*
 risorse connesse con sistema di simulazione
*/
if(strcmp(newcw->selettore.str_input,curcw->selettore.str_input)||
   strcmp(newcw->selettore.str_output,curcw->selettore.str_output))
        {
        Compile(newcw);
        do_redisplay = False;
        }


return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlSelettoreWidget cw= (XlSelettoreWidget) w;

/*
	Rilascio i GC
*/
if (cw->selettore.norm_gc)
	XtReleaseGC(cw,cw->selettore.norm_gc);
if (cw->selettore.clear_gc)
	XtReleaseGC(cw,cw->selettore.clear_gc);
if (cw->selettore.norm_bg_gc)
	XtReleaseGC(cw,cw->selettore.norm_bg_gc);
if (cw->selettore.selettore_gc)
	XtReleaseGC(cw,cw->selettore.selettore_gc);
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
