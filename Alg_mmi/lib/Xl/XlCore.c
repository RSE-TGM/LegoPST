/**********************************************************************
*
*       C Source:               XlCore.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jun 11 11:52:28 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlCore.c-6.1.7 %  (%full_filespec: XlCore.c-6.1.7:csrc:1 %)";
#endif
/*
   modulo XlCore.c
   tipo 
   release 5.3
   data 4/2/96
   reserved @(#)XlCore.c	5.3
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlCore.c	5.3\t4/2/96";
/*
        Fine sezione per SCCS
*/
/*
 *  XlCore.c - widget bottone per stazioni
 */
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <Xl/Xl.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlManagerP.h>
#include <Xl/XlCompositeP.h>
#include <Ol/OlPertP.h>
 

#if __STDC__
#define Const const
#else
#define Const /**/
#endif

#define MIN_WIDTH       5
#define MIN_HEIGHT      5

/* dichiarazione converter risorse AnimatedColor */
extern Boolean XlCvtAnimatedColor();

static Const String XtNwrongParameters = "wrongParameters";

/* lista delle risorse  */
static XtResource resources[]= {
        {
        XmNborderWidth,
        XmCBorderWidth,
        XmRDimension,
        sizeof(Dimension),
        XtOffsetOf(XlCoreRec,core.border_width),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNx0,
        XlCX0,
        XmRPosition,
        sizeof(Position),
        XtOffsetOf(XlCoreRec,xlcore.x0),
        XmRImmediate,
        (XtPointer)9999
        },
        {
        XlNy0,
        XlCY0,
        XmRPosition,
        sizeof(Position),
        XtOffsetOf(XlCoreRec,xlcore.y0),
        XmRImmediate,
        (XtPointer)9999
        },
        {
        XlNheight0,
        XlCHeight0,
        XmRDimension,
        sizeof(Dimension),
        XtOffsetOf(XlCoreRec,xlcore.height0),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNwidth0,
        XlCWidth0,
        XmRDimension,
        sizeof(Dimension),
        XtOffsetOf(XlCoreRec,xlcore.width0),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNnome,
        XlCNome,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCoreRec,xlcore.nome),
        XmRImmediate,
        "lcore"
        },
        {
        XlNselected,
        XlCSelected,
        XmRBoolean,
        sizeof(Boolean),
        XtOffsetOf(XlCoreRec,xlcore.selected),
        XmRImmediate,
        (XtPointer)False
        },
        {
        XlNinEdit,
        XlCInEdit,
        XmRBoolean,
        sizeof(Boolean),
        XtOffsetOf(XlCoreRec,xlcore.in_edit),
        XmRImmediate,
        (XtPointer)False
        },
        {
        XlNwEdit,
        XlCWEdit,
        XmRWidget,
        sizeof(Widget),
        XtOffsetOf(XlCoreRec,xlcore.w_edit),
        XmRImmediate,
        (XtPointer)NULL
        },
        {
        XlNfattZoom,
        XlCFattZoom,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCoreRec,xlcore.fatt_zoom),
        XmRImmediate,
        (XtPointer) 100
        },
        {
        XlNinheritBackground,
        XlCInheritBackground,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCoreRec,xlcore.inheritBackground),
        XmRImmediate,
        (XtPointer)True
        },
	{
        XlNerrore,
        XlCErrore,
        XmRBoolean,
        sizeof(Boolean),
        XtOffsetOf(XlCoreRec,xlcore.errore),
        XmRImmediate,
        (XtPointer)False
	},
        {
        XlNselectable,
        XlCSelectable,
        XmRBoolean,
        sizeof(Boolean),
        XtOffsetOf(XlCoreRec,xlcore.selectable),
        XmRImmediate,
        (XtPointer)True
        },
        {
        XlNselectedPixel,
        XlCSelectedPixel,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCoreRec,xlcore.selected_pixel),
        XmRString,
        "red"
        },
        {
        XmNborderColor,
        XmCBorderColor,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlCoreRec,core.border_pixel),
        XmRString,
        "black"
        },
        {
        XlNconfig,
        XlCConfig,
        XmRBoolean,
        sizeof(Boolean),
        XtOffsetOf(XlCoreRec,xlcore.config),
        XmRImmediate,
        (XtPointer)True
        },
        {
        XlNrotate,
        XlCRotate,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCoreRec,xlcore.pub_rotate),
        XmRImmediate,
        (XtPointer)ROTATE_0
        },
        {
        XlNassRotate,
        XlCAssRotate,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCoreRec,xlcore.pub_ass_rotate),
        XmRImmediate,
        (XtPointer)NO_Y_ASS_ROTATE
        },
        {
        XlNtrasparent,
        XlCTrasparent,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCoreRec,xlcore.trasparent),
        XmRImmediate,
        (XtPointer)False
        },
        {
        XlNpaginaStazioni,
        XlCPaginaStazioni,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCoreRec,xlcore.pagina_staz),
        XmRImmediate,
        ""
        },
        {
        XlNcompiled,
        XlCCompiled,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCoreRec,xlcore.compiled_str),
        XmRImmediate,
        "not compiled"
        },
        {
        XlNobjectTag,
        XlCObjectTag,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCoreRec,xlcore.object_tag),
        XmRImmediate,
        ""
	},
        {
        XlNuserLevel,
        XlCUserLevel,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCoreRec,xlcore.user_level),
        XmRString,
        (XtPointer)DEFAULT_ULEVEL,
	},
        };

/* definizione confinfo */
static XlConfInfo confinfo[] = {
        {
        XmNx,
        XmCPosition,
        XlDx,
        XlRPosition,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlCoreRec,core.x),
        sizeof(Position),
        },
        {
        XmNy,
        XmCPosition,
        XlDy,
        XlRPosition,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlCoreRec,core.y),
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
        XtOffsetOf(XlCoreRec,xlcore.x0),
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
        XtOffsetOf(XlCoreRec,xlcore.y0),
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
        XtOffsetOf(XlCoreRec,xlcore.width0),
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
        XtOffsetOf(XlCoreRec,xlcore.height0),
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
        XtOffsetOf(XlCoreRec,core.background_pixel),
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
	XtOffsetOf(XlCoreRec,xlcore.inheritBackground),
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
        XtOffsetOf(XlCoreRec,core.border_pixel),
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
        XtOffsetOf(XlCoreRec,core.border_width),
        sizeof(Dimension),
        },
	{
	XlNrotate,
	XlCRotate,
	XlDrotate,
	XlROption,
        NULL,
	XlOrotate,
	XlRGrGeom,
	XtOffsetOf(XlCoreRec,xlcore.pub_rotate),
	sizeof(int),
	},
        {
        XlNassRotate,
        XlCAssRotate,
        XlDassRotate,
        XlROption,
        NULL,
        XlOassRotate,
        XlRGrGeom,
        XtOffsetOf(XlCoreRec,xlcore.pub_ass_rotate),
        sizeof(int),
        },
/*
        {
        XlNtrasparent,
        XlCTrasparent,
        XlDtrasparent,
        XlROption,
        NULL,
        XlOtrasparent,
        XlRGrGeom,
        XtOffsetOf(XlCoreRec,xlcore.trasparent),
        sizeof(int),
        },
*/
	{
        XlNpaginaStazioni,
        XlCPaginaStazioni,
        XlDpaginaStazioni,
        XlRText,
        NULL,
        NULL,
        XlRGrGeom, /* XlRGrIO   */
        XtOffsetOf(XlCoreRec,xlcore.pagina_staz),
        sizeof(char*)
        },
	{
        XlNobjectTag,
        XlCObjectTag,
        XlDobjectTag,
        XlRText,
        NULL,
        NULL,
        XlRGrGeom,
        XtOffsetOf(XlCoreRec,xlcore.object_tag),
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
        XtOffsetOf(XlCoreRec,xlcore.user_level),
        sizeof(int)
        },
        };



/* dichiarazioni funzioni varie */
static XlConfInfo * get_confinfo();
static void GetSelectGC();
static void GetAllGCs();
                                     

         
/* dichiarazione dei metodi (methods) */
static void ClassInitialize();
static void Initialize();
static void Redisplay();
static void Realize();
static void Destroy();
static void Resize(); 
static Boolean Refresh();
static void Select();
static Boolean Compile();
static Boolean Read();
static Boolean Write();
static Boolean SetValues();
static void DrawBorderIfSelected();
#if defined XPRINTER_USED
static Boolean Print();
#endif

/* actions del widget XlCore */
static void Seleziona();
static void AddSelez();

/* dichiarazione di procedure varie */
static int separa_str();
static void ReadCompiled();

/* typedef per separa_str() */
typedef struct {
 char *stringa;
 int lun_stringa;
 } STRIN_ST;

/* translations  */
static char defaultTranslations[]= 
		"~Shift<Btn1Down>:  Seleziona() \n\
		 Shift<Btn1Down>: AddSelez()";

static XtActionsRec actions[] = {
	{"Seleziona",Seleziona },
	{"AddSelez", AddSelez} };

/* Inizializzazione del class record */
XlCoreClassRec xlCoreClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &coreClassRec /*&widgetClassRec*/,
    /* class_name               */      "XlCore",
    /* widget_size              */      sizeof(XlCoreRec),
    /* class_initialize         */      ClassInitialize,
    /* class_part_initialize    */      NULL,
    /* class_inited             */      FALSE,
    /* initialize               */      Initialize,
    /* initialize_hook          */      NULL,
    /* realize                  */      /*Realize*/XtInheritRealize,
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
    /* resize                   */      XtInheritResize,
    /* expose                   */      Redisplay/*XtInheritExpose*/,
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
  { /* fields presenti per ogni oggetto di lego-mmi */
    /* informazioni per la configurazione */
    /* oggetto pert             */      NULL,
    /* oggetto database         */      NULL,
    /* oggetto database_topologia  */   NULL,
    /* oggetto compilatore      */      NULL,
    /* confinfo                 */      confinfo,
    /* num_confinfo             */      XtNumber(confinfo),     
    /* refresh                  */	Refresh,
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

WidgetClass xlCoreWidgetClass = (WidgetClass) &xlCoreClassRec;

static void Select(w,type)
Widget w;
int type; /* specifica se selezione esclusiva o inclusiva */
{
Arg args[2];
XlCoreWidget cw= (XlCoreWidget)w;
XlCoreClassRec * wclass; /* puntatore alla widget class del widget */
/*
 Se il widget ha il flag di selezionabile  a 0
 ( il widget non puo' essere selezionato) 
 il metodo di select non esegue alcuna operazione
*/
if(!cw->xlcore.selectable)
	return;
/*
 Se il widget non e' in fase di configurazione
 forza la selezione di tipo esclusivo settando
 type=0 
*/
if(cw->xlcore.config==False) 
        {
        type=0;
        }

/*
	deseleziona i fratelli
*/
if(type ==0 )
	XlDeselSiblings(cw,w);
/*
 Se l'oggetto e' contenuto in un XlComposite deseleziona tutti gli
 altri fratelli di XlComposite
*/
if(XlIsXlComposite(XtParent(cw)) && type == 0)
	XlDeselSiblings(XtParent(cw),w);

/*
 Se l'oggetto non e' selezionato lo seleziona
*/
if(!cw->xlcore.selected)
	{
	XtSetArg(args[0],XlNselected,True);
	XtSetValues(cw,args,1);
	}
/*
	Chiama la procedura selectRunTime, se e' stata settata
*/
if(type == 0 )
	{
	wclass=(XlCoreClassRec*)XtClass(w);
	if(wclass->xlcore_class.selectRunTime!=NULL)
       		(*wclass->xlcore_class.selectRunTime)(cw);
	}
}

XlDeselSiblings(cw,w)
Widget cw;
Widget w;
{
Arg args[2];
int num_children,i;
Widget *children;
XtVaGetValues( XtParent(cw),
               XmNnumChildren, &num_children,
               XmNchildren, &children,
               NULL );
for(i=0;i<num_children;i++)
     if(children[i] != w)
           {
	   if(XlIsXlComposite(children[i]))
		XlDeselChildren(children[i]);
	   if(XlIsXlCore(children[i]))
		{
           	XtSetArg(args[0],XlNselected,False);
           	XtSetValues(children[i],args,1);
		}
           }
}

XlDeselChildren(cw)
Widget cw;
{
Arg args[2];
int num_children,i;
Widget *children;
XtVaGetValues( cw,
               XmNnumChildren, &num_children,
               XmNchildren, &children,
               NULL );
XtSetArg(args[0],XlNselected,False);
for(i=0;i<num_children;i++)
       {
	if(XlIsXlCore(children[i]))
      	 	XtSetValues(children[i],args,1);
       }
}

static Boolean Refresh(Widget w)
{
PUNT_DB pdb;
PUNT_VARINP varinp;
XlConfInfo *confinfo;
int num_confinfo;
int i,ret;
static Boolean prima_volta=True;
XlCoreClassRec *wCoreClass;
OlDatabasePuntiObject database,database_app ;

database_app=xlCoreClassRec.xlcore_class.database;
wCoreClass=(XlCoreClassRec *)XtClass(w);
database=wCoreClass->xlcore_class.database;


if(database==NULL)
	{
	XlWarning("XlCore","Refresh","Database non definito");
	return(False);
	}
/*
	Se l'oggetti non e' in config legge le risorse compilate
*/
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
	XlWarning("XlCore","Refresh",
		"errore refresh (metodo refreshconfinfo");

return(ret);
}

static Boolean Compile(Widget w, OlCompilerObject compilatore)
{
XlCoreWidget cw= (XlCoreWidget)w;
XtResourceList resources;
XlConfInfo *confinfo;
int num_confinfo;
int num_resources;
int i,k;
Boolean ret;
char *var;
int app_int;
STRIN_ST strin [3];
XlCoreClassRec * wclass; /* puntatore alla widget class del widget */


if(compilatore==NULL)
	{
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	return(False);
	}

wclass=(XlCoreClassRec*)XtClass(w);
XtGetResourceList(wclass,&resources,&num_resources);
RtRecord(resources);
confinfo=get_confinfo(w,& num_confinfo);
/*
 libera la memoria di compiled_str solo se e' stata
 allocata in una precedente compilazione
*/
if (cw->xlcore.compiled_allocated && cw->xlcore.compiled_str)
	XtFree(cw->xlcore.compiled_str);

/******
XtFree(cw->xlcore.compiled_str);
*******/
cw->xlcore.compiled_str=NULL;
ret=(olCompilerClassRec.olcompiler_class.compileconfinfo)
		(compilatore,w,confinfo,num_confinfo,resources,num_resources,
		&(cw->xlcore.compiled_str));
if(!ret)
	XlErrComp(w,"Compile","Error in CompileConfinfo",NULL);
/*
 Il metodo compile confinfo ha allocato spazio per una
 nuova compiled_str; lo notifico con il flag 
 compiled_allocated
*/
if(cw->xlcore.compiled_str)
	cw->xlcore.compiled_allocated=True;
XtFree(resources);
return(ret);
}

static Boolean Read(w,fp)
Widget w;
FILE *fp;
{
printf("\n richiamato metodo di read bottone");
return(True);
}

static Boolean Write(w,fp)
Widget w;
FILE *fp;
{
        printf("\n richiamato metodo di write bottone");
        return(True);
}

static void ClassInitialize()
{
   char *getenv();
   char ulevel_c[10];
   int ulevel;
   
XtSetTypeConverter(XmRString,XtRAnimatedColor,XlCvtAnimatedColor,
                   (XtConvertArgList)NULL,0,XtCacheNone,NULL);


   ulevel = DEFAULT_ULEVEL;

   if( getenv("MMI_ULEVEL") != NULL)
   {
      strcpy(ulevel_c,getenv("MMI_ULEVEL")); 
      ulevel = atoi(ulevel_c);
   }

   xlCoreClassRec.xlcore_class.ulevel = ulevel;

}

static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlCoreWidget new = (XlCoreWidget)tnew;
XlCoreClassRec * wclass; /* puntatore alla widget class del widget */
XtResourceList resources;
int i,num_resources;
char *old_string, *new_string;
int parent_rotate,parent_ass_rotate;
Arg arg[15];
Pixel back_parent;
XlManagerWidget cwman;
float zoom;

if( (new->xlcore.config!=True) && 
    (new->xlcore.user_level > xlCoreClassRec.xlcore_class.ulevel) )
   new->xlcore.hidden = True;
else
   new->xlcore.hidden = False;


/*
	registra le risorse rotate e ass_rotate
*/
new->xlcore.rotate=new->xlcore.pub_rotate;
new->xlcore.ass_rotate=new->xlcore.pub_ass_rotate;

/*
	verifica lo stato dello zomm del padre, se composite
*/
if(XlIsXlComposite(XtParent(new)))
	{
	cwman=(XlManagerWidget)XtParent(new);
	zoom=(float)cwman->xlmanager.fatt_zoom/100.0;
	}
else
	zoom=(float)new->xlcore.fatt_zoom/100.0;;

/*
	setta i valori di X e y
*/
new->core.x = new->xlcore.x0*zoom;
new->core.y = new->xlcore.y0*zoom;

/* DEBUG
printf("DEBUG:zoom=%f new->xlcore.fatt_zoom=%f\n",zoom,(float)new->xlcore.fatt_zoom);
printf("DEBUG: x=%d y=%d x0=%d y0=%d\n",new->core.x,new->core.y,new->xlcore.x0,new->xlcore.y0);
*/

/*
	Inizializza il flag di compiled_allocated a false
*/
new->xlcore.compiled_allocated=False;
/*
	verifico l'eventuale rotazione del padre
*/
if(XlIsXlComposite(XtParent(new)))
	{
	i = 0;
	XtSetArg(arg[i],XlNrotate,&parent_rotate);i++;
	XtSetArg(arg[i],XlNassRotate,&parent_ass_rotate);i++;
	XtGetValues(XtParent(new),arg,i);

        /*
		casi particolari
	*/
	if((parent_rotate == ROTATE_90)||(parent_rotate == ROTATE_270))
		if(new->xlcore.ass_rotate == Y_ASS_ROTATE)
			new->xlcore.rotate = new->xlcore.rotate + ROTATE_180;

	new->xlcore.rotate=new->xlcore.rotate+parent_rotate;
	if(new->xlcore.rotate>ROTATE_270)
		new->xlcore.rotate = new->xlcore.rotate - ROTATE_COMPLETO;
	new->xlcore.ass_rotate=new->xlcore.ass_rotate+parent_ass_rotate;
	if(new->xlcore.ass_rotate > Y_ASS_ROTATE)
		new->xlcore.ass_rotate = NO_Y_ASS_ROTATE;
	}

/*
	registra width e height da width0 e height0
*/
if(new->xlcore.width0<MIN_WIDTH)
        new->xlcore.width0=MIN_WIDTH;
if(new->xlcore.height0<MIN_HEIGHT)
        new->xlcore.height0=MIN_HEIGHT;
new->core.width=new->xlcore.width0*zoom;
new->core.height=new->xlcore.height0*zoom;

/*
	se inhertiBackground == 1 setta il background del padre
*/
if(new->xlcore.inheritBackground == 1)
	{
	i=0;
	XtSetArg(arg[i],XmNbackground,&back_parent);i++;
	XtGetValues(XtParent(tnew),arg,i);	
	new->core.background_pixel=back_parent;
	}


GetAllGCs(new);

/*
	Salva le risorse di tipo stringa
*/
/*******
wclass=(XlCoreClassRec*)XtClass(tnew);
XtGetResourceList(wclass,&resources,&num_resources);
RtRecord(resources);
XlAllocWidgetString(tnew,resources,num_resources);
XtFree(resources);
********/
XlInitializeWidgetValue(new);
/*
 Legge gli eventuali campi compilati dell'oggetto solo
 se l'oggetto e' in fase run time
*/
if(new->xlcore.config==False) 
	ReadCompiled(new);
}

#define superclass (&coreClassRec)

static void Realize(w, valueMask, attributes)
Widget w;
XtValueMask *valueMask;
XSetWindowAttributes *attributes;
{
XlCoreWidget cw= (XlCoreWidget)w;


/*
	setta i bit per ottenere la trasparenza del fondo dell'oggetto
*/
if(cw->xlcore.trasparent==True)
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
XlCoreWidget cw= (XlCoreWidget)w;
int xsave,ysave;
Window windsave;
register int x,y;
unsigned int width,height;
XmDrawingAreaCallbackStruct str;
Widget padre;
XlCompositeWidget padre_composite;
XlCompositeClassRec *padre_class;
XExposeEvent evento;


if( cw->xlcore.hidden )
{
  XtUnmapWidget(w);
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

if(cw->xlcore.trasparent==True)
	{
	padre=XtParent(cw);
	xsave= evento.x;
	ysave= evento.y;
	windsave= evento.window;

	evento.x+=cw->core.x;
	evento.y+=cw->core.y;
	if(XlIsXlComposite(XtParent(cw)))
		{
		padre_composite=(XlCompositeWidget)padre;
/*
		if(padre_composite->xlmanager.trasparent==False)
			{
			padre_class=(XlCompositeClassRec *)
					XtClass(padre_composite);
			(padre_class->core_class.expose) 
						(padre_composite,&evento,NULL);
			return;
			}
*/
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

static void Seleziona(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
int type; /* tipo di selezione richiesta */
XlCoreWidget cw= (XlCoreWidget)w;
/* richiama il metodo di select */
(*xlCoreClassRec.xlcore_class.select)(w,0);
}

static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
int type; /* tipo di selezione richiesta */
XlCoreWidget cw= (XlCoreWidget)w;
/* richiama il metodo di select */
(*xlCoreClassRec.xlcore_class.select)(w,1);
}


static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
Arg core_args[4];
CoreWidgetClass wclass;
XlCoreWidget curcw= (XlCoreWidget) current;
XlCoreWidget newcw= (XlCoreWidget) new;
XlManagerWidget cwman;
int app_int;
Boolean do_redisplay = False;
Position app_x,app_y;
Dimension app_w,app_h;
Widget Parent;
int app_rotate,app_ass_rotate;
float zoom;


/*
	La risorsa di trasparenza puo' essere settato solo
	alla creazione dell'oggetto
*/
if(curcw->xlcore.trasparent != newcw->xlcore.trasparent)
        {
	newcw->xlcore.trasparent=curcw->xlcore.trasparent;
	XlWarning("XlCore","SetValues",
             "attributo di trasparenza non puo' essere modificato");
	}

/*
	risorsa selected
*/
if(curcw->xlcore.selected != newcw->xlcore.selected)
        {
	do_redisplay = True;
	}
/*
        risorsa errore
*/
if(curcw->xlcore.errore != newcw->xlcore.errore)
        {
        do_redisplay = True;
        }

/*
 	modifica fattore di zoom
*/
if(curcw->xlcore.fatt_zoom != newcw->xlcore.fatt_zoom)
	{
	float rapp_zoom;
	Dimension new_width,new_height;
	Position new_x,new_y;

	rapp_zoom= (float)newcw->xlcore.fatt_zoom / (float)curcw->xlcore.fatt_zoom;
	new_width= (Dimension) ((float)curcw->core.width * rapp_zoom);
	new_height= (Dimension)  ((float)curcw->core.height * rapp_zoom);
	new_x= (Position) ((float)curcw->core.x * rapp_zoom);
	new_y= (Position) ((float)curcw->core.y * rapp_zoom);

	newcw->core.width= new_width;
	newcw->core.height= new_height;
	newcw->core.x= new_x;
	newcw->core.y= new_y;

        printf("SetValues: (float)newcw->xlcore.fatt_zoom=%f \n",(float)newcw->xlcore.fatt_zoom);
       	do_redisplay = True;
	}




/*
        risorsa x & y
*/
if(curcw->core.x != newcw->core.x)
        {
	if(XlIsXlComposite(XtParent(curcw)))
		{
		cwman=(XlManagerWidget)XtParent(curcw);
		zoom=(float)cwman->xlmanager.fatt_zoom/100.0;
		XtVaGetValues( XtParent(curcw),
                        XmNwidth, &app_w, XmNheight, &app_h,
			XlNrotate,&app_rotate, XlNassRotate,&app_ass_rotate,
			NULL );
		app_x = newcw->core.x;
		app_y = newcw->core.y;
		if(XlIsIconReg(XtParent(curcw)))
			gest_detrasf_coor_composite(&app_x,&app_y,
				newcw->core.width,newcw->core.height,
				app_w-1,app_h-1,app_rotate,app_ass_rotate);
		else
			gest_detrasf_coor_composite(&app_x,&app_y,
				newcw->core.width,newcw->core.height,
				app_w,app_h,app_rotate,app_ass_rotate);
		newcw->xlcore.x0 = app_x/zoom;
		newcw->xlcore.y0 = app_y/zoom;
		}
	else
		{
		zoom=(float)newcw->xlcore.fatt_zoom/100.0;
		newcw->xlcore.x0 = newcw->core.x/zoom;
		curcw->xlcore.x0 = newcw->core.x/zoom;
                printf("SetValues: x0=%d zoom=%f \n",newcw->xlcore.x0,zoom);
                }
        do_redisplay = True;
        }

if(curcw->core.y != newcw->core.y)
        {
	if(XlIsXlComposite(XtParent(curcw)))
		{
		cwman=(XlManagerWidget)XtParent(curcw);
		zoom=(float)cwman->xlmanager.fatt_zoom/100.0;
		XtVaGetValues( XtParent(curcw),
                        XmNwidth, &app_w, XmNheight, &app_h,
			XlNrotate,&app_rotate, XlNassRotate,&app_ass_rotate,
			NULL );
		app_x = newcw->core.x;
		app_y = newcw->core.y;
		if(XlIsIconReg(XtParent(curcw)))
			gest_detrasf_coor_composite(&app_x,&app_y,
				newcw->core.width,newcw->core.height,
				app_w-1,app_h-1,app_rotate,app_ass_rotate);
		else
			gest_detrasf_coor_composite(&app_x,&app_y,
				newcw->core.width,newcw->core.height,
				app_w,app_h,app_rotate,app_ass_rotate);
		newcw->xlcore.x0 = app_x/zoom;
		newcw->xlcore.y0 = app_y/zoom;
		}
	else
		{
		zoom=(float)newcw->xlcore.fatt_zoom/100.0;
		newcw->xlcore.y0 = newcw->core.y/zoom;
		curcw->xlcore.y0 = newcw->core.y/zoom;
                printf("SetValues: y0=%d zoom=%f \n",newcw->xlcore.y0,zoom);
		}
        do_redisplay = True;
        }

/*
	risorsa width
*/
if(curcw->core.width != newcw->core.width)
	{
	if(XlIsXlComposite(XtParent(curcw)))
		{
		cwman=(XlManagerWidget)XtParent(curcw);
		zoom=(float)cwman->xlmanager.fatt_zoom/100.0;
		}
	else
		zoom=(float)newcw->xlcore.fatt_zoom/100.0;
	if(newcw->core.width<MIN_WIDTH)
                newcw->core.width=MIN_WIDTH;
	if((newcw->xlcore.rotate==ROTATE_90)||
		(newcw->xlcore.rotate==ROTATE_270))
			newcw->xlcore.height0=newcw->core.width/zoom;
	else
			newcw->xlcore.width0=newcw->core.width/zoom;
        do_redisplay = True;
	}

/*
	risorsa height
*/
if(curcw->core.height != newcw->core.height)
	{
	if(XlIsXlComposite(XtParent(curcw)))
		{
		cwman=(XlManagerWidget)XtParent(curcw);
		zoom=(float)cwman->xlmanager.fatt_zoom/100.0;
		}
	else
		zoom=(float)newcw->xlcore.fatt_zoom/100.0;
	if(newcw->core.height<MIN_HEIGHT)
                newcw->core.height=MIN_HEIGHT;
	if((newcw->xlcore.rotate==ROTATE_90)||
		(newcw->xlcore.rotate==ROTATE_270))
		newcw->xlcore.width0=newcw->core.height/zoom;
	else
		newcw->xlcore.height0=newcw->core.height/zoom;
        do_redisplay = True;
	}

/*
	risorsa width0
*/
if(curcw->xlcore.width0 != newcw->xlcore.width0)
	{
	if(XlIsXlComposite(XtParent(curcw)))
		{
		cwman=(XlManagerWidget)XtParent(curcw);
		zoom=(float)cwman->xlmanager.fatt_zoom/100.0;
		}
	else
		zoom=(float)newcw->xlcore.fatt_zoom/100.0;
	if(newcw->xlcore.width0<MIN_WIDTH)
                newcw->xlcore.width0=MIN_WIDTH;
	if((newcw->xlcore.rotate==ROTATE_90)||
		(newcw->xlcore.rotate==ROTATE_270))
		newcw->core.height=newcw->xlcore.width0*zoom;
	else
		newcw->core.width=newcw->xlcore.width0*zoom;
        do_redisplay = True;
	}

/*
	risorsa height0
*/
if(curcw->xlcore.height0 != newcw->xlcore.height0)
	{
	if(XlIsXlComposite(XtParent(curcw)))
		{
		cwman=(XlManagerWidget)XtParent(curcw);
		zoom=(float)cwman->xlmanager.fatt_zoom/100.0;
		}
	else
		zoom=(float)newcw->xlcore.fatt_zoom/100.0;
	if(newcw->xlcore.height0<MIN_HEIGHT)
                newcw->xlcore.height0=MIN_HEIGHT;
	if((newcw->xlcore.rotate==ROTATE_90)||
		(newcw->xlcore.rotate==ROTATE_270))
		newcw->core.width=newcw->xlcore.height0*zoom;
	else
		newcw->core.height=newcw->xlcore.height0*zoom;
        do_redisplay = True;
	}



/*
	risorsa config
*/
if(curcw->xlcore.config != newcw->xlcore.config)
        {
	if(newcw->xlcore.selected)
		{
		newcw->xlcore.selected = False;
        	do_redisplay = True;
		}
	}

/*
	risorsa selectable
*/
if(curcw->xlcore.selectable != newcw->xlcore.selectable)
        {
	if(newcw->xlcore.selected)
		{
		newcw->xlcore.selected = False;
        	do_redisplay = True;
		}
        }

/*
	risorsa rotate
*/
if(curcw->xlcore.rotate != newcw->xlcore.rotate)
        {
        if(newcw->xlcore.rotate>3)
                newcw->xlcore.rotate=newcw->xlcore.rotate%4;
        if((newcw->xlcore.rotate - curcw->xlcore.rotate== ROTATE_90)||
           (newcw->xlcore.rotate - curcw->xlcore.rotate== -ROTATE_90) ||
           (newcw->xlcore.rotate - curcw->xlcore.rotate== ROTATE_270) ||
           (newcw->xlcore.rotate - curcw->xlcore.rotate== -ROTATE_270))
                 {
		 if(curcw->core.height!=curcw->core.width)
		   {
                   XtSetArg(core_args[0],XtNwidth,curcw->core.height);
                   XtSetArg(core_args[1],XtNheight,curcw->core.width);
                   XtSetValues(newcw,core_args,2);
		   do_redisplay = False;
		   }
		 else
		   {
        	   do_redisplay = True;
		   }
                 }
	else
        	 do_redisplay = True;
	if(do_redisplay)
		{
		wclass=XtClass(curcw);
		(*wclass->core_class.resize)(newcw);
		}
        }
/*
	risorsa ass_rotate
*/
if(curcw->xlcore.ass_rotate != newcw->xlcore.ass_rotate)
	{
      	do_redisplay = True;
	}

/*
	risorsa selected_pixel
*/
if(curcw->xlcore.selected_pixel != newcw->xlcore.selected_pixel)
        {
        XtReleaseGC(newcw,newcw->xlcore.select_gc);
	GetSelectGC(newcw);
      	do_redisplay = True;
	}

return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlCoreWidget cw= (XlCoreWidget) w;
XlCoreClassRec * wclass; /* puntatore alla widget class del widget */
XtResourceList resources;
int num_resources;


/*
	libera il GC
*/
if(cw->xlcore.select_gc)
        XtReleaseGC(cw,cw->xlcore.select_gc);
/*
 libera compiled se allocata internamente
*/
if(cw->xlcore.compiled_allocated && cw->xlcore.compiled_str)
	XtFree(cw->xlcore.compiled_str);
/*******
wclass=(XlCoreClassRec*)XtClass(cw);
XtGetResourceList(wclass,&resources,&num_resources);
RtRecord(resources);
XlFreeWidgetString(cw,resources,num_resources);
XtFree(resources);
********/

}



static XlConfInfo * get_confinfo(Widget w,int * num_confinfo)
{
XlConfInfo *confinfo;
int num_risorse;
int i;
XlCoreClassRec * wclass; /* puntatore alla widget class del widget */
wclass=(XlCoreClassRec*)XtClass(w);
confinfo=wclass->xlcore_class.confinfo;
memcpy(num_confinfo,&(wclass->xlcore_class.num_confinfo),sizeof(int));
return(confinfo);
}

             
static int separa_str(
 char riga[],
 int lun,
 int nstr,
 STRIN_ST strin[])
{
char *s;
int i;
for (s=riga, i=0; i<nstr; i++) {
  strin[i].stringa = s = strtok( s, " \t");
  if(strin[i].stringa==(char*)NULL)
     {
     strin[i].stringa=(char*)malloc(3);
     sprintf(strin[i].stringa," ");
     strin[i].lun_stringa = strlen(strin[i].stringa);
     }
  else
     {
     strin[i].lun_stringa = ( s ? strlen( s) : 0 );
     }
  s = (char*)NULL; }
}

static void GetAllGCs(w)
Widget w;
{
GetSelectGC(w);
}

static void GetSelectGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFunction;
XlCoreWidget cw= (XlCoreWidget) w;

values.foreground = cw->xlcore.selected_pixel;
values.background = cw->core.background_pixel;
values.line_width = 0;
values.line_style = LineSolid;
values.function=GXcopy;
cw->xlcore.select_gc = XtGetGC(cw, valuemask, &values);
}




static void DrawBorderIfSelected(Widget w)
{
XlCoreWidget cw= (XlCoreWidget) w;
unsigned int width=cw->core.width;
unsigned int height=cw->core.height;

if((cw->xlcore.errore==True)&&(cw->xlcore.config==True))
	{
/*
 Evidenzia con una X il fatto che l'oggetto contiene un errore in
 compilazione
*/
	XDrawRectangle(XtDisplay(cw),XtWindow(cw),cw->xlcore.select_gc,
                0,0,width-1,height-1);
	XDrawLine(XtDisplay(cw),XtWindow(cw),cw->xlcore.select_gc,
                0,0,width-1,height-1);
	XDrawLine(XtDisplay(cw),XtWindow(cw),cw->xlcore.select_gc,
                0,height-1,width-1,0);
	}

if((cw->xlcore.selected!=1)||(cw->xlcore.config!=True))
	return;


XDrawRectangle(XtDisplay(cw),XtWindow(cw),cw->xlcore.select_gc,
		0,0,width-1,height-1);

XFillRectangle(XtDisplay(cw),XtWindow(cw),cw->xlcore.select_gc,
		0,0,DIM_Q_SEL,DIM_Q_SEL);
XFillRectangle(XtDisplay(cw),XtWindow(cw),cw->xlcore.select_gc,
		0,height-DIM_Q_SEL,DIM_Q_SEL,DIM_Q_SEL);
XFillRectangle(XtDisplay(cw),XtWindow(cw),cw->xlcore.select_gc,
		width-DIM_Q_SEL,0,DIM_Q_SEL,DIM_Q_SEL);
XFillRectangle(XtDisplay(cw),XtWindow(cw),cw->xlcore.select_gc,
		width-DIM_Q_SEL,height-DIM_Q_SEL,DIM_Q_SEL,DIM_Q_SEL);

if(height>DIM_Q_SEL*4)
	{
	XFillRectangle(XtDisplay(cw),XtWindow(cw), cw->xlcore.select_gc,
			0,(height-DIM_Q_SEL)/2,
			DIM_Q_SEL,DIM_Q_SEL);
	XFillRectangle(XtDisplay(cw),XtWindow(cw), cw->xlcore.select_gc,
			width-DIM_Q_SEL,(height-DIM_Q_SEL)/2,
			DIM_Q_SEL,DIM_Q_SEL);
	}

if(width>DIM_Q_SEL*4)
	{
	XFillRectangle(XtDisplay(cw),XtWindow(cw),cw->xlcore.select_gc,
			(width-DIM_Q_SEL)/2,height-DIM_Q_SEL,
			DIM_Q_SEL,DIM_Q_SEL);
	XFillRectangle(XtDisplay(cw),XtWindow(cw),cw->xlcore.select_gc,
			(width-DIM_Q_SEL)/2,0,
			DIM_Q_SEL,DIM_Q_SEL);
	}
}

/*
 XlIsXlCore(w) funzione per sapere se l'oggetto w
 e' del tipo XlCore
*/
Boolean XlIsXlCore(Widget w)
{
CoreWidgetClass wclass;

if(XtIsSubclass(w,&xlCoreClassRec))
        return(True);
else
        return(False);
}

static void ReadCompiled(Widget w)
{
XlCoreWidget cw = (XlCoreWidget)w;
PUNT_DB pdb;
PUNT_VAROUT varout;
PUNT_VARINP varinp;
XlConfInfo *confinfo;
int num_confinfo;
int i;
char *punt;
char campo[30];


if(strncmp(cw->xlcore.compiled_str,"#",1))
	return;

punt=cw->xlcore.compiled_str;
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
printf("dati letti [XlRTipoVarInp] %s %d %d %d %d %d %f %f %s\n",XtName(cw),
                varinp.pdb.nmod,varinp.pdb.indice,varinp.pdb.tipo,varinp.pdb.origin,
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
printf("dati letti [XlRTipoVarOut] %s %d %d %d %d -%d %f %f %f %f %s\n",XtName(cw),
                varout.pdb.nmod,varout.pdb.indice,varout.pdb.tipo,varout.pdb.origin,
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
XlCoreWidget cw = (XlCoreWidget)w;

XlPrintBackgroundAndBorder(w,cw->xlcore.trasparent);
return(True);
}
#endif
