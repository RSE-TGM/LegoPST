/**********************************************************************
*
*       C Source:               XlManager.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jun 11 11:53:00 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlManager.c-11.1.1 %  (%full_filespec: XlManager.c-11.1.1:csrc:1 %)";
#endif
/*
   modulo XlManager.c
   tipo 
   release 5.2
   data 4/2/96
   reserved @(#)XlManager.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlManager.c	5.2\t4/2/96";
/*
        Fine sezione per SCCS
*/
/*
 *  XlManager.c - widget led per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlP.h>
#include <X11/CoreP.h>
#include <Xl/XlCoreP.h>
#include <X11/CompositeP.h>
#include <Xl/XlManagerP.h>
#include <Ol/OlPertP.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */


#define MIN_WIDTH	5
#define MIN_HEIGHT	5


/* lista delle risorse  */
static XtResource resources[]= {
	{
        XmNborderWidth,
        XmCBorderWidth,
        XmRDimension,
        sizeof(Dimension),
        XtOffsetOf(XlManagerRec,core.border_width),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNx0,
        XlCX0,
        XmRPosition,
        sizeof(Position),
        XtOffsetOf(XlManagerRec,xlmanager.x0),
        XmRImmediate,
        (XtPointer)9999
        },
        {
        XlNy0,
        XlCY0,
        XmRPosition,
        sizeof(Position),
        XtOffsetOf(XlManagerRec,xlmanager.y0),
        XmRImmediate,
        (XtPointer)9999
        },
        {
        XlNheight0,
        XlCHeight0,
        XmRDimension,
        sizeof(Dimension),
        XtOffsetOf(XlManagerRec,xlmanager.height0),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNwidth0,
        XlCWidth0,
        XmRDimension,
        sizeof(Dimension),
        XtOffsetOf(XlManagerRec,xlmanager.width0),
        XmRImmediate,
        (XtPointer)0
        },
	{
        XmNborderColor,
        XmCBorderColor,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlManagerRec,core.border_pixel),
        XmRString,
        "black"
        },
        {
        XlNnome,
        XlCNome,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlManagerRec,xlmanager.nome),
        XmRImmediate,
        "lmanager"
        },
        {
        XlNselected,
        XlCSelected,
        XmRBoolean,
        sizeof(Boolean),
        XtOffsetOf(XlManagerRec,xlmanager.selected),
        XmRImmediate,
        (XtPointer)False
        },
        {
        XlNselectable,
        XlCSelectable,
        XmRBoolean,
        sizeof(Boolean),
        XtOffsetOf(XlManagerRec,xlmanager.selectable),
        XmRImmediate,
        (XtPointer)True
        },
        {
        XlNinEdit,
        XlCInEdit,
        XmRBoolean,
        sizeof(Boolean),
        XtOffsetOf(XlManagerRec,xlmanager.in_edit),
        XmRImmediate,
        (XtPointer)False
        },
        {
        XlNwEdit,
        XlCWEdit,
        XmRWidget,
        sizeof(Widget),
        XtOffsetOf(XlManagerRec,xlmanager.w_edit),
        XmRImmediate,
        (XtPointer)NULL
        },
        {
        XlNfattZoom,
        XlCFattZoom,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlManagerRec,xlmanager.fatt_zoom),
        XmRImmediate,
        (XtPointer) 100
        },
	{
        XlNinheritBackground,
        XlCInheritBackground,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlManagerRec,xlmanager.inheritBackground),
        XmRImmediate,
        (XtPointer)True
        },
        {
        XlNerrore,
        XlCErrore,
        XmRBoolean,
        sizeof(Boolean),
        XtOffsetOf(XlManagerRec,xlmanager.errore),
        XmRImmediate,
        (XtPointer)False
        },
        {
        XlNselectedPixel,
        XlCSelectedPixel,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlManagerRec,xlmanager.selected_pixel),
        XmRString,
        "red"
        },
        {
        XlNconfig,
        XlCConfig,
        XmRBoolean,
        sizeof(Boolean),
        XtOffsetOf(XlManagerRec,xlmanager.config),
        XmRImmediate,
        (XtPointer)True
        },
        {
        XlNtrasparent,
        XlCTrasparent,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlManagerRec,xlmanager.trasparent),
        XmRImmediate,
        (XtPointer)False
        },
        {
        XlNpaginaStazioni,
        XlCPaginaStazioni,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlManagerRec,xlmanager.pagina_staz),
        XmRImmediate,
        ""
        },
	{
        XlNcompiled,
        XlCCompiled,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlManagerRec,xlmanager.compiled_str),
        XmRImmediate,
        "not compiled"
        },
	{
        XlNobjectTag,
        XlCObjectTag,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlManagerRec,xlmanager.object_tag),
        XmRImmediate,
        ""
        },
        {
        XlNuserLevel,
        XlCUserLevel,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlManagerRec,xlmanager.user_level),
        XmRString,
        (XtPointer)DEFAULT_ULEVEL,
        },
        };

static XlConfInfo confinfo[] = {
        {
        XmNx,
        XmCPosition,
        XlDx,
        XlRDimension,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlManagerRec,core.x),
        sizeof(Position),
        },
        {
        XmNy,
        XmCPosition,
        XlDy,
        XlRDimension,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlManagerRec,core.y),
        sizeof(Position),
        },
        {
        XlNx0,
        XlCX0,
        XlDx0,
        XlRPosition,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlManagerRec,xlmanager.x0),
        sizeof(Position),
        },
        {
        XlNy0,
        XlCY0,
        XlDy0,
        XlRPosition,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlManagerRec,xlmanager.y0),
        sizeof(Position),
        },
        {
        XlNwidth0,
        XlCWidth0,
        XlDwidth0,
        XlRDimension,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlManagerRec,xlmanager.width0),
        sizeof(Dimension),
        },
        {
        XlNheight0,
        XlCHeight0,
        XlDheight0,
        XlRDimension,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlManagerRec,xlmanager.height0),
        sizeof(Dimension),
        },
        {
        XmNbackground,
        XmCBackground,
        XlDbackground,
        XlRColor,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlManagerRec,core.background_pixel),
        sizeof(Pixel),
        },
        {
        XlNinheritBackground,
        XlCInheritBackground,
        XlDinheritBackground,
        XlROption,
        NULL,
        XlOinheritBackground,
        XlRGrGeom,
        XtOffsetOf(XlManagerRec,xlmanager.inheritBackground),
        sizeof(int),
        },
        {
        XmNborderColor,
        XmCBorderColor,
        XlDborderColor,
        XlRColor,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlManagerRec,core.border_pixel),
        sizeof(Pixel),
        },
        {
        XmNborderWidth,
        XmCBorderWidth,
        XlDborderWidth,
        XlRDimension,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlManagerRec,core.border_width),
        sizeof(Dimension),
        },
        {
        XlNtrasparent,
        XlCTrasparent,
        XlDtrasparent,
        XlROption,
        NULL,
        XlOtrasparent,
        XlRGrGeom,
        XtOffsetOf(XlManagerRec,xlmanager.trasparent),
        sizeof(int),
        },
        {
        XlNobjectTag,
        XlCObjectTag,
        XlDObjectTag,
        XlRText,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlManagerRec,xlmanager.object_tag),
        sizeof(char*)
        },
        {
        XlNpaginaStazioni,
        XlCPaginaStazioni,
        XlDpaginaStazioni,
        XlRText,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlManagerRec,xlmanager.pagina_staz),
        sizeof(char*)
        },
        {
        XlNuserLevel,
        XlCUserLevel,
        XlDuserLevel,
        XlRInt,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlManagerRec,xlmanager.user_level),
        sizeof(int)
        },
        };

         
/* dichiarazione dei metodi (methods) */
static void ClassInitialize();
static void Initialize();
static void Destroy();
static void Select();
static void Resize();
static Boolean SetValues();
static Boolean Refresh();
static void Redisplay();
static void Realize();
static Boolean Compile();
static Boolean Read();
static Boolean Write();
static XtGeometryResult GeometryManager();
static void DrawBorderIfSelected();
#if defined XPRINTER_USED
static Boolean Print();
#endif

/* actions del widget XlManager */
static void Seleziona();
static void AddSelez();

/* dichiarazioni funzioni varie */
static void GetSelectGC();
static void GetSfondoGC();
static void GetAllGCs();
static XlConfInfo * get_confinfo();
static void ReadCompiled();



/* translations  */
static char defaultTranslations[]=
        "~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static XtActionsRec actions[] = {
        {"Seleziona",Seleziona},
        {"AddSelez",AddSelez}
        };

/* Inizializzazione del class record */
XlManagerClassRec xlManagerClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &compositeClassRec,
    /* class_name               */      "XlManager",
    /* widget_size              */      sizeof(XlManagerRec),
    /* class_initialize         */      ClassInitialize,
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
    /* read                     */      Read,
    /* write                    */      Write,
    /* select                   */      Select,
    /* drawBorderIfSelected     */      DrawBorderIfSelected,
    /* selectRunTime            */      NULL,
#if defined XPRINTER_USED
    /* print                    */      Print,
#endif
  }
};

WidgetClass xlManagerWidgetClass = (WidgetClass) &xlManagerClassRec;

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
XlManagerWidget cw= (XlManagerWidget)w;
}

static void Select(w,type)
Widget w;
int type; /* specifica se selezione esclusiva o inclusiva */
{
int     i, num_children;
Widget  *children;
Arg args[2];
XlManagerWidget cw= (XlManagerWidget)w;
XlManagerClassRec * wclass; /* puntatore alla widget class del widget */


/*
 Se il widget ha il flag di selezionabile  a 0
 ( il widget non puo' essere selezionato)
 il metodo di select non esegue alcuna operazione
*/
if(!cw->xlmanager.selectable)
	{
	printf("l'oggetto non e' selezionabile\n");
        return;
	}
/*
 Se il widget non e' in fase di configurazione
 forza la selezione di tipo esclusivo settando
 type=0
*/
if(cw->xlmanager.config==False)
        {
        type=0;
        }

/* 
   deseleziona i suoi figli
*/
if(type == 0)
	XlDeselChildren(cw);
/*
 deseleziona tutti  i suoi fratelli
*/
if(type == 0)
	XlDeselSiblings(cw,w);

/*
 Se l'oggetto non e' selezionato lo seleziona
*/
if(!cw->xlmanager.selected)
        {
        XtSetArg(args[0],XlNselected,True);
        XtSetValues(cw,args,1);
	}
/*
	Richiama la procedura memorizzata in selectRunTime
*/
if(type == 0)
	{
	wclass=(XlManagerClassRec*)XtClass(w);
	if(wclass->xlmanager_class.selectRunTime!=NULL)
         	(*wclass->xlmanager_class.selectRunTime)(cw);
	}
}

static void ClassInitialize()
{
   char *getenv();
   char ulevel_c[10];
   int ulevel;

   ulevel = DEFAULT_ULEVEL;

   if( getenv("MMI_ULEVEL") != NULL)
   {
      strcpy(ulevel_c,getenv("MMI_ULEVEL"));
      ulevel = atoi(ulevel_c);
   }

   xlManagerClassRec.xlmanager_class.ulevel = ulevel;

}

static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
Dimension width,height;
Position x,y;
extern WidgetClass XlGetClassWid();
Widget wchild;
char *str_elenco;
char class_name[20];
AN_OBJ *plista_an_obj;
XlManagerClassRec * wclass; /* puntatore alla widget class del widget */
XtResourceList resources;
int i,num_resources;
char *old_string, *new_string;
Arg arg[15];
Pixel back_parent;
XlManagerWidget new = (XlManagerWidget)tnew;
#if defined RECUPERO_OLD
XrmDatabase db;
char appo[50];
#endif


if( (new->xlmanager.config!=True) &&
    (new->xlmanager.user_level > xlManagerClassRec.xlmanager_class.ulevel) )
   new->xlmanager.hidden = True;
else
   new->xlmanager.hidden = False;



/*
        registra width e height da width0 e height0
*/
if(new->xlmanager.width0<MIN_WIDTH)
	new->xlmanager.width0=MIN_WIDTH;
if(new->xlmanager.height0<MIN_HEIGHT)
	new->xlmanager.height0=MIN_HEIGHT;

new->core.width=new->xlmanager.width0;
new->core.height=new->xlmanager.height0;
/*
 Inizializza il flag di compiled_allocated 
*/
new->xlmanager.compiled_allocated=False;
/*
        setta i valori di X e y
*/
#if defined RECUPERO_OLD
if(new->xlmanager.x0 != 9999)
        new->core.x = new->xlmanager.x0;
else
	{
        db = XrmGetDatabase(XtDisplay(XtParent(new)));
        sprintf(appo,"%d",new->core.x);
        XlSetResourceByWidget(&db,new,XlNx0,appo);
        XlWarning("XlManager","Initialize","Setto la nuova risorsa XlNx0");
        new->xlmanager.x0 = new->core.x;
        }

if(new->xlmanager.y0 != 9999)
        new->core.y = new->xlmanager.y0;
else
	{
        db = XrmGetDatabase(XtDisplay(XtParent(new)));
        sprintf(appo,"%d",new->core.y);
        XlSetResourceByWidget(&db,new,XlNy0,appo);
        XlWarning("XlManager","Initialize","Setto la nuova risorsa XlNy0");
        new->xlmanager.y0 = new->core.y;
        }
#else
new->core.x = new->xlmanager.x0;
new->core.y = new->xlmanager.y0;
#endif


/*
        se inhertiBackground == 1 setta il background del padre
*/
if(new->xlmanager.inheritBackground == 1)
        {
        i=0;
        XtSetArg(arg[i],XmNbackground,&back_parent);i++;
        XtGetValues(XtParent(tnew),arg,i);
        new->core.background_pixel=back_parent;
        }


/*
        Salva le risorse di tipo stringa
*/
/******
wclass=(XlManagerClassRec*)XtClass(tnew);
XtGetResourceList(wclass,&resources,&num_resources);
RtRecord(resources);
XlAllocWidgetString(tnew,resources,num_resources);
XtFree(resources);
*******/
GetAllGCs(new);
XlInitializeWidgetValue(new);
/*
 Legge i campi compilati solo se l'oggetto e'
 in run-time
*/
if(new->xlmanager.config==False)
	ReadCompiled(new);
}

static void GetAllGCs(w)
Widget w;
{
GetSelectGC(w);
GetSfondoGC(w);
}

static void GetSelectGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFunction;
XlManagerWidget cw= (XlManagerWidget) w;

values.foreground = cw->xlmanager.selected_pixel;
values.background = cw->core.background_pixel;
values.line_width = 0;
values.line_style = LineSolid;
values.function=GXcopy;
cw->xlmanager.select_gc = XtGetGC(cw, valuemask, &values);
}


static void GetSfondoGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCLineWidth
                         | GCLineStyle | GCFunction | GCSubwindowMode;
XlManagerWidget cw= (XlManagerWidget) w;

values.foreground = cw->core.background_pixel;
values.line_width = 0;
values.line_style = LineSolid;
values.function=GXcopy;
values.subwindow_mode= IncludeInferiors;
cw->xlmanager.sfondo_gc = XtGetGC(cw, valuemask, &values);
}


static void Seleziona(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/*
 Richiama la routine di select di XlManager per la gestione della
 selezione
*/
(*xlManagerClassRec.xlmanager_class.select) (w,0);
}

static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/* richiama il metodo di select */
(*xlManagerClassRec.xlmanager_class.select)(w,1);
}



static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
Arg comp_args[2];
XlManagerWidget curcw= (XlManagerWidget) current;
XlManagerWidget newcw= (XlManagerWidget) new;
Boolean do_redisplay = False;
float zoom=1.;

/*
 Spostamento dell'oggetto
*/
if(curcw->core.x != newcw->core.x)
	{
        zoom=(float)newcw->xlmanager.fatt_zoom/100.0;
	newcw->xlmanager.x0 = newcw->core.x/zoom;
        do_redisplay = True;
	}

if(curcw->core.y != newcw->core.y)
	{
        zoom=(float)newcw->xlmanager.fatt_zoom/100.0;
	newcw->xlmanager.y0 = newcw->core.y/zoom;
        do_redisplay = True;
	}
/*
	risorsa select
*/
if(curcw->xlmanager.selected != newcw->xlmanager.selected)
        {
        do_redisplay = True;
        }

if(curcw->xlmanager.errore != newcw->xlmanager.errore)
        {
        do_redisplay = True;
        }



/*
        La risorsa di trasparenza puo' essere settato solo
        alla creazione dell'oggetto
*/
if(curcw->xlmanager.trasparent != newcw->xlmanager.trasparent)
        {
        newcw->xlmanager.trasparent=curcw->xlmanager.trasparent;
        XlWarning("XlManager","SetValues",
             "attributo di trasparenza non puo' essere modificato");
        }

/*
        risorsa config
*/
if(curcw->xlmanager.config != newcw->xlmanager.config)
        {
        if(newcw->xlmanager.selected)
                {
                newcw->xlmanager.selected = False;
                do_redisplay = True;
                }
        }

/*
        risorsa selectable
*/
if(curcw->xlmanager.selectable != newcw->xlmanager.selectable)
        {
        if(newcw->xlmanager.selected)
                {
                newcw->xlmanager.selected = False;
                do_redisplay = True;
                }
        }

/*
        risorsa selected_pixel
*/
if(curcw->xlmanager.selected_pixel != newcw->xlmanager.selected_pixel)
        {
        XtReleaseGC(newcw,newcw->xlmanager.select_gc);
        GetSelectGC(newcw);
        do_redisplay = True;
        }

/*
        risorsa width
*/
/*
if(curcw->core.width != newcw->core.width)
        {
        if(newcw->core.width<MIN_WIDTH)
                newcw->core.width=MIN_WIDTH;
        newcw->xlmanager.width0=newcw->core.width;
        do_redisplay = True;
        }
*/

/*
        risorsa height
*/
/*
if(curcw->core.height != newcw->core.height)
        {
        if(newcw->core.height<MIN_HEIGHT)
                newcw->core.height=MIN_HEIGHT;
        newcw->xlmanager.height0=newcw->core.height;
        do_redisplay = True;
        }
*/

/*
        risorsa width0
*/
/*
if(curcw->xlmanager.width0 != newcw->xlmanager.width0)
        {
        if(newcw->xlmanager.width0<MIN_WIDTH)
                newcw->xlmanager.width0=MIN_WIDTH;
        newcw->core.width=newcw->xlmanager.width0;
        do_redisplay = True;
        }
*/

/*
        risorsa height0
*/
/*
if(curcw->xlmanager.height0 != newcw->xlmanager.height0)
        {
        if(newcw->xlmanager.height0<MIN_HEIGHT)
                newcw->xlmanager.height0=MIN_HEIGHT;
        newcw->core.height=newcw->xlmanager.height0;
        do_redisplay = True;
        }
*/

/*
        modifica fattore di zoom
*/
if(curcw->xlmanager.fatt_zoom != newcw->xlmanager.fatt_zoom)
        {
        float rapp_zoom;
        Dimension new_width,new_height;
        Position new_x,new_y;
	rapp_zoom= (float)newcw->xlmanager.fatt_zoom / 
		(float)curcw->xlmanager.fatt_zoom;

        new_width= (Dimension) ((float)curcw->core.width * rapp_zoom);
        new_height= (Dimension)  ((float)curcw->core.height * rapp_zoom);
        new_x= (Position) ((float)curcw->core.x * rapp_zoom);
        new_y= (Position) ((float)curcw->core.y * rapp_zoom);

        newcw->core.width= new_width;
        newcw->core.height= new_height;
        newcw->core.x= new_x;
        newcw->core.y= new_y;

        do_redisplay = True;
        }



return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlManagerWidget cw= (XlManagerWidget) w;
XlManagerClassRec * wclass; /* puntatore alla widget class del widget */
XtResourceList resources;
int num_resources;


/*
        libera i GC
*/
if(cw->xlmanager.select_gc)
        XtReleaseGC(cw,cw->xlmanager.select_gc);
if(cw->xlmanager.sfondo_gc)
        XtReleaseGC(cw,cw->xlmanager.sfondo_gc);

/*
 libera compiled se allocata internamente
*/
if(cw->xlmanager.compiled_allocated && cw->xlmanager.compiled_str)
        XtFree(cw->xlmanager.compiled_str);

/******
wclass=(XlCoreClassRec*)XtClass(cw);
XtGetResourceList(wclass,&resources,&num_resources);
RtRecord(resources);
XlFreeWidgetString(cw,resources,num_resources);
XtFree(resources);
******/

}

static Boolean Refresh(Widget w)
{
XlManagerWidget cw= (XlManagerWidget)w;
XlConfInfo *confinfo;
int num_confinfo;
int i,ret;
static Boolean prima_volta=True;
XlManagerClassRec *wManagerClass;
OlDatabasePuntiObject database,database_app;


database_app=xlManagerClassRec.xlmanager_class.database;

wManagerClass=(XlManagerClassRec *)XtClass(w);
database=wManagerClass->xlmanager_class.database;
if(database==NULL)
        {
        XlWarning("XlManager","Refresh","Database non definito");
        return(False);
        }

/*
if(prima_volta)
	{
	prima_volta=False;
   	ReadCompiled(w);
	}
*/

confinfo=get_confinfo(w,& num_confinfo);

ret=(olDatabasePuntiClassRec.oldatabasePunti_class.refreshconfinfo)
                (database,w,confinfo,num_confinfo);

if(!ret)
	XlWarning("XlManager","Refresh",
		"errore refresh (metodo refreshconfinfo)");
return(ret);
}

static Boolean Compile(w,compilatore )
Widget w;
OlCompilerObject compilatore;
{
int i;
XtResourceList resources;
XlConfInfo *confinfo;
int num_confinfo;
int num_resources;
Boolean ret;
XlManagerClassRec *wclass;
XlManagerWidget cw= (XlManagerWidget)w;

if(compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
        return(False);
        }

wclass=(XlManagerClassRec*)XtClass(w);
XtGetResourceList(wclass,&resources,&num_resources);
RtRecord(resources);
confinfo=get_confinfo(w,& num_confinfo);
/*
 libera la memoria di compiled_str solo se e' stata
 allocata in una precedente compilazione
*/
if (cw->xlmanager.compiled_allocated && cw->xlmanager.compiled_str)
	XtFree(cw->xlmanager.compiled_str);

/******
XtFree(cw->xlmanager.compiled_str);
*******/
cw->xlmanager.compiled_str=NULL;
ret=(olCompilerClassRec.olcompiler_class.compileconfinfo)
                (compilatore,w,confinfo,num_confinfo,resources,num_resources,
		&(cw->xlmanager.compiled_str));

if(!ret)
	XlErrComp(w,"Compile","Error in CompileConfinfo",NULL);
/*
 Il metodo compile confinfo ha allocato spazio per una
 nuova compiled_str; lo notifico con il flag
 compiled_allocated
*/
if(cw->xlmanager.compiled_str)
        cw->xlmanager.compiled_allocated=True;


XtFree(resources);
return(ret);
}

static Boolean Read(w,fp)
Widget w;
FILE *fp;
{
printf("\n richiamato metodo di read composite");
return(True);
}

static Boolean Write(w,fp)
Widget w;
FILE *fp;
{
printf("\n richiamato metodo di write composite");
return(True);
}


#define superclass (&coreClassRec)

static void Realize(w, valueMask, attributes)
Widget w;
XtValueMask *valueMask;
XSetWindowAttributes *attributes;
{
XlManagerWidget cw= (XlManagerWidget)w;

/*
        setta i bit per ottenere la trasparenza del fondo dell'oggetto
*/
if(cw->xlmanager.trasparent==True)
        {
        *valueMask &=(~CWBackPixel);
        *valueMask &= (~CWBackPixmap);
        }

(*superclass->core_class.realize) (w, valueMask, attributes);
}



static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlManagerWidget cw= (XlManagerWidget)w;
register int x,y;
unsigned int width,height;
XmDrawingAreaCallbackStruct str;
XExposeEvent evento;
XlManagerWidget padre_composite;
XlManagerClassRec *padre_class;
int xsave,ysave;
Window windsave;
Widget padre;


if( cw->xlmanager.hidden )
{
  XtUnmapWidget(w);

printf("\n\tTENTA LA UNMAP \n");
  return;
}

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
        richiama la routine di expose del padre dell'oggetto
*/
if(event)
        {
        evento.x=event->x;
        evento.y=event->y;
        evento.width=event->width;
        evento.height=event->height;
        evento.window=event->window;
        }
else
        {
        evento.x=0;
        evento.y=0;
        evento.width=cw->core.width;
        evento.height=cw->core.height;
        evento.window=XtWindow(cw);;
        }

if(cw->xlmanager.trasparent==True)
        {
        padre=XtParent(cw);
        xsave= evento.x;
        ysave= evento.y;
        windsave= evento.window;
       evento.x+=cw->core.x;
        evento.y+=cw->core.y;
        if(XlIsXlComposite(XtParent(cw)))
                {
/****
                padre_composite=(XlManagerWidget)padre;
                if(padre_composite->xlmanager.trasparent==False)
                        {
                        padre_class=(XlManagerClassRec *)
                                        XtClass(padre_composite);
                        (padre_class->core_class.expose)
                                                (padre_composite,&evento,NULL);
                        return;
                        }
*******/
                evento.x+=padre->core.x;
                evento.y+=padre->core.y;
                padre=XtParent(padre);
                }
        evento.window=XtWindow(padre);
        evento.count=0;
        str.event=(XEvent*)(&evento);
        str.window=XtWindow(padre);
        evento.window=XtWindow(padre);
        evento.display=XtDisplay(padre);
        evento.type=Expose;
        XtCallCallbacks(padre,XmNexposeCallback,&str);

        if(event)
                {
                event->x = xsave;
                event->y = ysave;
                event->window = windsave;
                }
        }

}

      
/*
 XlIsXlManager(w) funzione per sapere se l'oggetto w
 e' del tipo XlManager
*/
Boolean XlIsXlManager(Widget w)
{

if(XtIsSubclass(w,&xlManagerClassRec))
        return(True);
else
        return(False);

}


static void DrawBorderIfSelected(Widget w)
{
XlManagerWidget cw= (XlManagerWidget) w;
unsigned int width=cw->core.width;
unsigned int height=cw->core.height;

if((cw->xlmanager.errore==True)&&(cw->xlmanager.config==True))
        {
/*
 Evidenzia con una X il fatto che l'oggetto contiene un errore in
 compilazione
*/
        XDrawRectangle(XtDisplay(cw),XtWindow(cw),cw->xlmanager.select_gc,
                0,0,width-1,height-1);
        XDrawLine(XtDisplay(cw),XtWindow(cw),cw->xlmanager.select_gc,
                0,0,width-1,height-1);
        XDrawLine(XtDisplay(cw),XtWindow(cw),cw->xlmanager.select_gc,
                0,height-1,width-1,0);
        }


if((cw->xlmanager.selected!=1)||(cw->xlmanager.config!=True))
        return;

XDrawRectangle(XtDisplay(cw),XtWindow(cw),
                cw->xlmanager.select_gc,0,0,width-1,height-1);

XFillRectangle(XtDisplay(cw),XtWindow(cw),
                cw->xlmanager.select_gc,0,0,DIM_Q_SEL,DIM_Q_SEL);
XFillRectangle(XtDisplay(cw),XtWindow(cw),
                cw->xlmanager.select_gc,0,height-DIM_Q_SEL,DIM_Q_SEL,DIM_Q_SEL);

XFillRectangle(XtDisplay(cw),XtWindow(cw),
                cw->xlmanager.select_gc,width-DIM_Q_SEL,0,DIM_Q_SEL,DIM_Q_SEL);
XFillRectangle(XtDisplay(cw),XtWindow(cw),
                cw->xlmanager.select_gc,width-DIM_Q_SEL,
		height-DIM_Q_SEL,DIM_Q_SEL,DIM_Q_SEL);

if(height>DIM_Q_SEL*4)
        {
        XFillRectangle(XtDisplay(cw),XtWindow(cw), cw->xlmanager.select_gc,
                        0,(height-DIM_Q_SEL)/2,
                        DIM_Q_SEL,DIM_Q_SEL);
        XFillRectangle(XtDisplay(cw),XtWindow(cw), cw->xlmanager.select_gc,
                        width-DIM_Q_SEL,(height-DIM_Q_SEL)/2,
                        DIM_Q_SEL,DIM_Q_SEL);
        }

if(width>DIM_Q_SEL*4)
        {
        XFillRectangle(XtDisplay(cw),XtWindow(cw),cw->xlmanager.select_gc,
                        (width-DIM_Q_SEL)/2,height-DIM_Q_SEL,
                        DIM_Q_SEL,DIM_Q_SEL);
        XFillRectangle(XtDisplay(cw),XtWindow(cw),cw->xlmanager.select_gc,
                        (width-DIM_Q_SEL)/2,0,
                        DIM_Q_SEL,DIM_Q_SEL);
        }


}


static XlConfInfo * get_confinfo(Widget w,int * num_confinfo)
{
XlConfInfo *confinfo;
int num_risorse;
int i;
XlManagerClassRec * wclass; /* puntatore alla widget class del widget */
wclass=(XlManagerClassRec*)XtClass(w);
confinfo=wclass->xlmanager_class.confinfo;
memcpy(num_confinfo,&(wclass->xlmanager_class.num_confinfo),sizeof(int));
return(confinfo);
}


static void ReadCompiled(Widget w)
{
XlManagerWidget cw = (XlManagerWidget)w;
PUNT_DB pdb;
PUNT_VAROUT varout;
PUNT_VARINP varinp;
XlConfInfo *confinfo;
int num_confinfo;
int i;
char *punt;
char campo[30];


if(strncmp(cw->xlmanager.compiled_str,"#",1))
        return;

punt=cw->xlmanager.compiled_str;
punt += 2;
confinfo=get_confinfo(w,& num_confinfo);
for(i=0;i<num_confinfo;i++)
        {
        switch(confinfo[i].confinfo_type){
          case XlRTipoVarInp:
          case XlRTipoVarReg:
          case XlRTipoVarInp+LOCKED_PAG:
          case XlRTipoVarReg+LOCKED_PAG:
             {
             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varinp.pdb.nmod=atoi(campo);

             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varinp.pdb.indice=atoi(campo);

             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varinp.pdb.tipo=atoi(campo);

	     sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varinp.pdb.origin=atoi(campo);

             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varinp.tipo_elab=atoi(campo);

             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varinp.unimis.a=atof(campo);

             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varinp.unimis.b=atof(campo);

             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             strcpy(varinp.unimis.descr,campo);
	
	     sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);

             memcpy((char*)(confinfo[i].compiled_offset+(char *)w),

                        &varinp, confinfo[i].compiled_size);

/*
printf("dati letti [XlRTipoVarInp] %s %d %d %d %d %f %f %s\n",XtName(cw),
                varinp.pdb.nmod,varinp.pdb.indice,varinp.pdb.tipo,
                varinp.tipo_elab,varinp.unimis.a,varinp.unimis.b,
                varinp.unimis.descr);
*/


             break;
             }
          case XlRTipoVarOut:
          case XlRTipoVarOut+LOCKED_PAG:
             {


             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varout.pdb.nmod=atoi(campo);

             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varout.pdb.indice=atoi(campo);

             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varout.pdb.tipo=atoi(campo);

	     sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varout.pdb.origin=atoi(campo);

             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varout.tipo_pert=atoi(campo);

             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varout.value=atof(campo);


             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varout.rateodurata=atof(campo);

             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varout.unimis.a=atof(campo);

             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             varout.unimis.b=atof(campo);

             sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);
             strcpy(varout.unimis.descr,campo);

	     sscanf(punt,"%s",campo);
             punt += (strlen(campo)+1);

             memcpy((char*)(confinfo[i].compiled_offset+(char *)w),
                        &varout, confinfo[i].compiled_size);

/*
printf("dati letti [XlRTipoVarOut] %s %d %d %d %d %f %f %f %f %s\n",XtName(cw),
                varout.pdb.nmod,varout.pdb.indice,varout.pdb.tipo,
                varout.tipo_pert,varout.value,varout.rateodurata,
                varout.unimis.a,varout.unimis.b,varout.unimis.descr);
*/


             break;
             }
          default:
                break;
          }
        }
}



/*
        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
printf("XlManager: metodo di Print\n");
return(True);
}
#endif
