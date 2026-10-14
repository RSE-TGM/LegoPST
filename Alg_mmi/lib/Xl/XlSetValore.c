/**********************************************************************
*
*       C Source:               XlSetValore.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:31:28 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlSetValore.c-5.2.5 %  (%full_filespec: XlSetValore.c-5.2.5:csrc:1 %)";
#endif
/*
   modulo XlSetValore.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlSetValore.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlSetValore.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlSetValore.c - widget led per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlP.h>
#include <X11/CoreP.h>
#include <Xl/XlCoreP.h>
#include <X11/CompositeP.h>
#include <Xl/XlSetValoreP.h>
#include <Xm/ToggleB.h>
#include <Xm/Text.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */


#define MIN_WIDTH       5
#define MIN_HEIGHT      5


/* lista delle risorse  */
static XtResource resources[]= {
	{
        XlNnormFg,
        XlCNormFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlSetValoreRec,setValore.norm_fg),
        XmRString,
        XtDefaultForeground
        },
	{
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlSetValoreRec,setValore.normalfont),
        XmRImmediate,
        "fixed"
        },
        {
        XlNnumeroInt,
        XlCNumeroInt,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlSetValoreRec,setValore.numero_int),
        XmRImmediate,
        (XtPointer)NUMERO_INTERI_VISUALIZZATI
        },
        {
        XlNnumeroDec,
        XlCNumeroDec,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlSetValoreRec,setValore.numero_dec),
        XmRImmediate,
        (XtPointer)NUMERO_DECIMALI_VISUALIZZATI
        },
        {
        XlNscalamento,
        XlCScalamento,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlSetValoreRec,setValore.scalamento),
        XmRString,
        VALORE_SCALAMENTO
        },
        {
        XlNoffset,
        XlCOffset,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlSetValoreRec,setValore.offset),
        XmRString,
        VALORE_OFFSET
        },
	{
        XlNvarInput,
        XlCVarInput,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlSetValoreRec,setValore.str_input),
        XmRImmediate,
        ""
        },
	{
        XlNvarInibitSetV,
        XlCVarInibitSetV,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlSetValoreRec,setValore.str_inibit),
        XmRImmediate,
        ""
        },
	{
        XlNvarOutput,
        XlCVarOutput,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlSetValoreRec,setValore.str_output),
        XmRImmediate,
        ""
        },
	{
        XlNvarRltargetSetV,
        XlCVarRltargetSetV,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlSetValoreRec,setValore.str_rltarget),
        XmRImmediate,
        ""
        }
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
        XtOffsetOf(XlSetValoreRec,setValore.norm_fg),
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
        XtOffsetOf(XlSetValoreRec,setValore.normalfont),
        sizeof(XFontStruct*)
        },
        {
        XlNnumeroInt,
        XlCNumeroInt,
        XlDnumeroInt,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSetValoreRec,setValore.numero_int),
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
        XtOffsetOf(XlSetValoreRec,setValore.numero_dec),
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
        XtOffsetOf(XlSetValoreRec,setValore.scalamento),
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
        XtOffsetOf(XlSetValoreRec,setValore.offset),
        sizeof(float)
        },
	{
        XlNvarInput,
        XlCVarInput,
        XlDvarInput,
	XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSetValoreRec,setValore.input),
        sizeof(PUNT_VARINP)
        },
	{
        XlNvarInibitSetV,
        XlCVarInibitSetV,
        XlDvarInibitSetV,
	XlRTipoVarInp,
        XlRVarDA,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSetValoreRec,setValore.inibit),
        sizeof(PUNT_VARINP)
        },
	{
        XlNvarOutput,
        XlCVarOutput,
        XlDvarOutput,
	XlRTipoVarOut,
        XlRVarXO,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSetValoreRec,setValore.output),
        sizeof(PUNT_VAROUT)
        },
	{
        XlNvarRltargetSetV,
        XlCVarRltargetSetV,
        XlDvarRltargetSetV,
	XlRTipoVarOut,
        XlRVarDO,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlSetValoreRec,setValore.rltarget),
        sizeof(PUNT_VAROUT)
        }
	};

/* dichiarazioni funzioni varie */
static void GetNormFgGC();
static void GetClearGC();
static void GetAllGCs();
static Boolean LoadFont();
static int min();
static void DrawIntoSetValore();
static void ActivateToggle();

         
/* dichiarazione dei metodi (methods) */
static void Initialize();
static void Destroy();
static void Resize();
static Boolean SetValues();
static Boolean Refresh();
static void Redisplay();
static Boolean Compile();
static XtGeometryResult GeometryManager();
static void add_translation();
static void CreaFigli();
static void DistruggiFigli();
static void SettaActionsFigli();
#if defined XPRINTER_USED
static Boolean Print();
#endif

/* actions del widget XlSetValore */
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
XlSetValoreClassRec xlSetValoreClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlManagerClassRec,
    /* class_name               */      "SetValore",
    /* widget_size              */      sizeof(XlSetValoreRec),
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
  { /* setValore fields */
    /* empty                    */      0
  }
};

WidgetClass xlSetValoreWidgetClass = (WidgetClass) &xlSetValoreClassRec;

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
XlSetValoreWidget cw= (XlSetValoreWidget)w;

DrawIntoSetValore(cw);

}


static void GetClearGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlSetValoreWidget cw= (XlSetValoreWidget) w;
values.foreground = cw->core.background_pixel;
values.background = cw->setValore.norm_fg;
values.line_width = 0;
values.line_style = LineSolid;
cw->setValore.clear_gc = XtGetGC(cw, valuemask, &values);
}


static void GetAllGCs(w)
Widget w;
{
GetClearGC(w);
}


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlSetValoreWidget new = (XlSetValoreWidget)tnew;
int i,app_int;
Arg arg[25];
Dimension dim_bottone,w_font,h_font,text_width,text_height;
XtResourceList resources;
int num_resources;





if(!LoadFont(new,new->setValore.normalfont))
        if(!LoadFont(new,"fixed"))
                XlError("XlSetValore","Initialize","Impossibile caricare default font");

GetAllGCs(new);

/*
	inizializza lo stato del toggle button
*/
new->setValore.premuto=False;

/*
	controllo il valore dello scalamento
*/
if(new->setValore.scalamento==0)
	new->setValore.scalamento=1;
/*
	inizializza il vampo old_valore e valore
*/
new->setValore.old_valore= -99999999999.99;
new->setValore.input.valore = 0.0;
new->setValore.old_inibit=-1;

/*
	inizializza a NULL i puntatori agli oggetti text e button
*/
new->setValore.text=NULL;
new->setValore.bottone=NULL;

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
XlSetValoreWidget cw= (XlSetValoreWidget)w;

/*
 Richiama la routine di select di XlSetValore per la gestione della
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
XlSetValoreWidget cw= (XlSetValoreWidget)w;

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
Arg comp_args[2];
XlSetValoreWidget curcw= (XlSetValoreWidget) current;
XlSetValoreWidget newcw= (XlSetValoreWidget) new;
Boolean do_redisplay = False;

if(curcw->setValore.norm_fg!=newcw->setValore.norm_fg)
	{
	XtReleaseGC(curcw,curcw->setValore.clear_gc);
	GetClearGC(newcw);
	XtSetArg(comp_args[0],XmNforeground,newcw->setValore.norm_fg);
	XtSetValues(newcw,comp_args,1);
        do_redisplay = True;
        }
	
if(curcw->setValore.scalamento!=newcw->setValore.scalamento)
        {
        do_redisplay = True;
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

if(curcw->setValore.offset!=newcw->setValore.offset)
        {
        do_redisplay = True;
        }

if(strcmp(curcw->setValore.normalfont,newcw->setValore.normalfont))
        {
        if(!LoadFont(newcw,newcw->setValore.normalfont))
        if(!LoadFont(newcw,"fixed"))
                XlError("XlSetValore","SetValues",
                        "Impossibile caricare default font");
        do_redisplay = True;
        }

/*
        risorsa width
*/
if(curcw->core.width != newcw->core.width)
        {
        if(newcw->core.width<MIN_WIDTH)
                newcw->core.width=MIN_WIDTH;
        newcw->xlmanager.width0=newcw->core.width;
        do_redisplay = True;
        }

/*
        risorsa height
*/
if(curcw->core.height != newcw->core.height)
        {
        if(newcw->core.height<MIN_HEIGHT)
                newcw->core.height=MIN_HEIGHT;
        newcw->xlmanager.height0=newcw->core.height;
        do_redisplay = True;
        }

/*
        risorsa width0
*/
if(curcw->xlmanager.width0 != newcw->xlmanager.width0)
        {
        if(newcw->xlmanager.width0<MIN_WIDTH)
                newcw->xlmanager.width0=MIN_WIDTH;
        newcw->core.width=newcw->xlmanager.width0;
        do_redisplay = True;
        }

/*
        risorsa height0
*/
if(curcw->xlmanager.height0 != newcw->xlmanager.height0)
        {
        if(newcw->xlmanager.height0<MIN_HEIGHT)
                newcw->xlmanager.height0=MIN_HEIGHT;
        newcw->core.height=newcw->xlmanager.height0;
        do_redisplay = True;
        }



/*
 risorse connesse con sistema di simulazione
*/
if(strcmp(newcw->setValore.str_input,curcw->setValore.str_input)||
   strcmp(newcw->setValore.str_inibit,curcw->setValore.str_inibit)||
   strcmp(newcw->setValore.str_rltarget,curcw->setValore.str_rltarget)||
   strcmp(newcw->setValore.str_output,curcw->setValore.str_output))
        {
        Compile(newcw);
        do_redisplay = True;
        }


return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlSetValoreWidget cw= (XlSetValoreWidget) w;

/*
	Rilascio i GC
*/
#ifdef ELIMINAZIONENORMGC
if(cw->setValore.norm_gc)
        XtReleaseGC(cw,cw->setValore.norm_gc);
#endif

if(cw->setValore.clear_gc)
        XtReleaseGC(cw,cw->setValore.clear_gc);

/* libero la font_struct */
/*
if(cw->setValore.font_info)
        XFreeFont(XtDisplay(cw),cw->setValore.font_info);
*/

/* libero la font_list  */
if(cw->setValore.font_list)
        XmFontListFree(cw->setValore.font_list);
}

static Boolean Refresh(Widget w)
{
XlSetValoreWidget cw= (XlSetValoreWidget) w;

if(cw->xlmanager.config==True)
        return(True);

if(!XtIsWidget(w))
        {
        XlWarning("XlSetValore","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlSetValoreClassRec.xlmanager_class.database==NULL)
        {
        XlWarning("XlSetValore","Refresh","database non definito");
        return(False);
        }
if(!(*superclass->xlmanager_class.refresh) (w))
        {
        XlWarning("XlSetVAlore","Refresh","errore refresh classe superiore");
        return(False);
        }

DrawIntoSetValore(cw);

return(True);
}

static Boolean Compile(Widget w)
{
int i;
Boolean errors=False;

XlSetValoreWidget cw= (XlSetValoreWidget)w;

if(!XtIsWidget(w))
        {
        XlWarning("XlSetValue","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(strlen(cw->setValore.str_input)== NULL)
	{
	XlErrComp(w,"Compile","Input variable not defined",NULL);
        errors=True;
        }

if(strlen(cw->setValore.str_output)== NULL)
        {
	XlErrComp(w,"Compile","Output variable not defined",NULL);
        errors=True;
        }

if(xlSetValoreClassRec.xlmanager_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
        return(False);
        }

if(!(*superclass->xlmanager_class.compile) (w,
     xlSetValoreClassRec.xlmanager_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
        return(False);
        }

XlEvError(cw,errors);
return(True);
}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlSetValoreWidget cw= (XlSetValoreWidget)w;
register int x,y;
unsigned int width,height;
XmDrawingAreaCallbackStruct str;


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
XClearArea(XtDisplay(cw),XtWindow(cw->setValore.bottone),0,0,0,0,True);
XClearArea(XtDisplay(cw),XtWindow(cw->setValore.text),0,0,0,0,True);

DrawIntoSetValore(w);

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
XlSetValoreWidget Xl_cw = (XlSetValoreWidget) cw;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
/* Carica il font ottenendo la descrizione del font stesso */

char **pf;
int i,nf;

if((Xl_cw->setValore.font_info = XlGetFont(XtDisplay(cw),fontname)) == NULL
)
        {
        XlWarning("XlSetValore","LoadFont","Cannot open font");
        return(False);
        }
        else
        {
        Xl_cw->setValore.font_list =
                XmFontListCreate ( Xl_cw->setValore.font_info, charset);
        if (Xl_cw->setValore.font_list == NULL)
                {
                XlWarning("XlSetValore","LoadFont","FONTLIST NULL");
                return(False);
                }
        }
return(True);
}

static int min(a,b)
int a,b;
{
return((a<b)? a:b);
}


static void DrawIntoSetValore(w)
Widget w;
{
XlSetValoreWidget cw= (XlSetValoreWidget)w;
int width,height;
int dx=10;
int dy=10;
char testo[15];
float valore = cw->setValore.input.valore;
int inibit = (int)(cw->setValore.inibit.valore);
float scalamento = cw->setValore.scalamento;
float offset = cw->setValore.offset;
char app_char[20];

if(cw->xlmanager.config==True)
	return;

/*
	se la variabile inibit non e' settata la pone = 1
*/
if(cw->setValore.inibit.pdb.indice<0)
	inibit=1;


width=cw->core.width;
height=cw->core.height;

/*
        se necessario sbianca lo sfondo
*/
if(cw->xlmanager.trasparent!=True)
   XlRiempiRettangolo(cw,XtWindow(cw),cw->setValore.clear_gc,0,0,100,100);

/*
	Se il tasto e' premuto non esegue il refresh
*/
if(cw->setValore.premuto)
	return;

if(valore!=cw->setValore.old_valore)
	{
	valore = valore * scalamento;
	valore = valore + offset;
	sprintf(app_char,"%*.*f",
		cw->setValore.numero_int+cw->setValore.numero_dec+1,
		cw->setValore.numero_dec, valore);
	XmTextSetString(cw->setValore.text,app_char);
	}
cw->setValore.old_valore=valore;

if(inibit!=cw->setValore.old_inibit)
	{
	if(inibit)
		XtSetSensitive(cw->setValore.bottone,True);
	else
		XtSetSensitive(cw->setValore.bottone,False);
	}
cw->setValore.old_inibit=inibit;
}

static void ActivateToggle(w,info,str)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *str;
{
XlSetValoreWidget cw= (XlSetValoreWidget)XtParent(w);
char *pstring;
float scalamento = cw->setValore.scalamento;
float offset = cw->setValore.offset;


if(cw->xlmanager.config)
	{
	XmToggleButtonSetState(cw->setValore.bottone,False,False);
	return;
	}


cw->setValore.premuto= ! cw->setValore.premuto;

if(cw->setValore.premuto)
	{
	XmTextSetEditable(cw->setValore.text,True);
	}
else
	{
	XmTextSetEditable(cw->setValore.text,False);
	}
/*
	se e' necessario invia la perturbazione
*/
if((!cw->setValore.premuto)&&(cw->setValore.output.pdb.indice>=0))
     {
     pstring=(char *) XmTextGetString(cw->setValore.text);
     RtRecord(pstring);
     sscanf(pstring,"%f",&(cw->setValore.output.value));
/*
Il valore viene riscalato in modo che sia congruente con l'output
*/
     cw->setValore.output.value=((cw->setValore.output.value - offset) / scalamento);
     XtFree(pstring);
     (olPertClassRec.olpert_class.pertsend)
		(xlSetValoreClassRec.xlmanager_class.pert,
                        cw->setValore.output);
     /*
	se e' stata settata la perturbazione rltarget la invia
     */
     if(cw->setValore.rltarget.pdb.indice>=0)
         (olPertClassRec.olpert_class.pertsend)
		(xlSetValoreClassRec.xlmanager_class.pert,
                        cw->setValore.rltarget);
     }
}

/*---------------------------------------------------------------------
 * add_translation  (finzione gia' presente nel pagedit)
 *
 * permette di agganciare run-time ad un widget una translation
 *
 * input :
 *          Widget wid;      widget a cui agganciare la translation
 *          char *stringa;   stringa ove e' definita la translation
 *
 *----------------------------------------------------------------------*/


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
                 set_something_val(wid, XmNtranslations, (XtArgVal) tr);
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
XlSetValoreWidget new = (XlSetValoreWidget)tnew;
int i,app_int;
Arg arg[25];
Dimension dim_bottone,w_font,h_font,text_width,text_height;


/*
        creazione widget bottone button
*/
dim_bottone=min(new->core.width/4,new->core.height-6)*0.6;
i=0;
XtSetArg(arg[i],XmNx,(new->core.width/4-dim_bottone)/2); i++;
XtSetArg(arg[i],XmNy,(new->core.height-dim_bottone)/2); i++;
XtSetArg(arg[i],XmNwidth,dim_bottone+1); i++;
XtSetArg(arg[i],XmNheight,dim_bottone+1); i++;
XtSetArg(arg[i],XmNfontList,new->setValore.font_list); i++;
XtSetArg(arg[i],XmNmarginHeight,0); i++;
XtSetArg(arg[i],XmNmarginWidth,0); i++;
XtSetArg(arg[i],XmNspacing,0); i++;
XtSetArg(arg[i],XmNindicatorSize,dim_bottone);i++;
/*
Aggiunte da Fabio 30-9-96
XmNnavigationType,XmNONE e XmNtraversalOn
per incompatibilita' con nuova versione X su piattaforma AIX
*/ 
XtSetArg(arg[i],XmNnavigationType,XmNONE);i++;
XtSetArg(arg[i],XmNtraversalOn,FALSE); i++;
XtSetArg(arg[i],XmNlabelString,XmStringCreateLtoR("",XmSTRING_DEFAULT_CHARSET));
 i++;
XtSetArg(arg[i],XmNbackground,new->core.background_pixel);i++;
new->setValore.bottone=XmCreateToggleButton(new,"__bottone",arg,i);
XtAddCallback(new->setValore.bottone,
                XmNvalueChangedCallback,ActivateToggle,NULL);
XtManageChild(new->setValore.bottone);
/*
        creazione text
*/
h_font=new->setValore.font_info->ascent+new->setValore.font_info->descent;
w_font=new->setValore.font_info->max_bounds.width;
text_width=new->core.width-new->core.width/4-8;
text_height=new->core.height-10;
i=0;
XtSetArg(arg[i],XmNx,new->core.width/4+2); i++;
XtSetArg(arg[i],XmNy,5); i++;
XtSetArg(arg[i],XmNwidth,text_width); i++;
XtSetArg(arg[i],XmNheight,text_height); i++;
XtSetArg(arg[i],XmNmaxLength,10); i++;
XtSetArg(arg[i],XmNborderWidth,1); i++;
XtSetArg(arg[i],XmNresizeHeight,False); i++;
XtSetArg(arg[i],XmNresizeWidth,False); i++;
XtSetArg(arg[i],XmNfontList,new->setValore.font_list); i++;
XtSetArg(arg[i],XmNeditMode,XmSINGLE_LINE_EDIT); i++;
XtSetArg(arg[i],XmNscrollHorizontal,True); i++;
XtSetArg(arg[i],XmNscrollLeftSide,True); i++;
XtSetArg(arg[i],XmNshadowThickness,0); i++;
XtSetArg(arg[i],XmNmarginWidth,0); i++;
XtSetArg(arg[i],XmNmarginHeight,0); i++;

/*
Aggiunte da Fabio 30-9-96
XmNnavigationType,XmNONE e XmNtraversalOn
per incompatibilita' con nuova versione X su piattaforma AIX
*/ 
XtSetArg(arg[i],XmNnavigationType,XmNONE);i++;
XtSetArg(arg[i],XmNtraversalOn,FALSE); i++;


XtSetArg(arg[i],XmNforeground,new->setValore.norm_fg); i++;
XtSetArg(arg[i],XmNbackground,new->core.background_pixel);i++;
new->setValore.text= XmCreateText(new,"__text",arg,i);
XmTextSetEditable(new->setValore.text,False);
XtManageChild(new->setValore.text);
}

static void DistruggiFigli(tnew)
Widget tnew;
{
XlSetValoreWidget new = (XlSetValoreWidget)tnew;

if(new->setValore.text!=NULL)
	XtDestroyWidget(new->setValore.text);
if(new->setValore.bottone!=NULL)
	XtDestroyWidget(new->setValore.text);

new->setValore.text=NULL;
new->setValore.bottone=NULL;

}

static void SettaActionsFigli(tnew)
Widget tnew;
{
XlSetValoreWidget new = (XlSetValoreWidget)tnew;

if(new->xlmanager.config==True)
	{
	add_translation(new->setValore.text,textTranslations);
	add_translation(new->setValore.bottone,buttonTranslations);
	}
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
