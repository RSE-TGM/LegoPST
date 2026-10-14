/**********************************************************************
*
*       C Source:               XlComposite.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:25:25 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlComposite.c-3.1.3 %  (%full_filespec: XlComposite.c-3.1.3:csrc:1 %)";
#endif
/*
   modulo XlComposite.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlComposite.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlComposite.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlComposite.c - widget composite per stazioni
 */
#include <Xm/XmP.h>
#include <X11/StringDefs.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Xl/XlP.h>
#include <X11/CoreP.h>
#include <Xl/XlCoreP.h>
#include <X11/CompositeP.h>
#include <Xl/XlCompositeP.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */


#define MIN_WIDTH       5
#define MIN_HEIGHT      5


/* lista delle risorse  */
static XtResource resources[]= {
	{
        XlNcompositeName,
        XlCCompositeName,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCompositeRec,xlcomposite.composite_name),
        XmRImmediate,
        ""
        },
        {
        XlNchildrenLayout,
        XlCChildrenLayout,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCompositeRec,xlcomposite.childrenLayout),
        XmRImmediate,
        (XtPointer)True
        },
        {
        XlNrotate,
        XlCRotate,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCompositeRec,xlcomposite.rotate),
        XmRImmediate,
        (XtPointer)ROTATE_0
        },
        {
        XlNassRotate,
        XlCAssRotate,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCompositeRec,xlcomposite.ass_rotate),
        XmRImmediate,
        (XtPointer)NO_Y_ASS_ROTATE
        },
	{
        XlNnumFigli,
        XlCNumFigli,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCompositeRec,xlcomposite.num_figli),
        XmRImmediate,
        (XtPointer)0
	},
        {
        XlNlistChildren,
        XlCListChildren,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCompositeRec,xlcomposite.list_children),
        XmRImmediate,
        ""
        }
        };

/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
        {
        XlNcompositeName,
        XlCCompositeName,
        XlDcompositeName,
        XlRText,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlCompositeRec,xlcomposite.composite_name),
        sizeof(char*)
        },
        {
        XlNchildrenLayout,
        XlCChildrenLayout,
        XlDchildrenLayout,
        XlROption,
        NULL,
        XlOchildrenLayout,
        XlRGrIo,
        XtOffsetOf(XlCompositeRec,xlcomposite.childrenLayout),
        sizeof(int),
        },
        {
        XlNrotate,
        XlCRotate,
        XlDrotate,
        XlROption,
        NULL,
        XlOrotate,
        XlRGrIo,
        XtOffsetOf(XlCompositeRec,xlcomposite.rotate),
        sizeof(int),
        },
        {
        XlNassRotate,
        XlCAssRotate,
        XlDassRotate,
        XlROption,
        NULL,
        XlOassRotate,
        XlRGrIo,
        XtOffsetOf(XlCompositeRec,xlcomposite.ass_rotate),
        sizeof(int),
        },
	};


         
/* dichiarazione dei metodi (methods) */
static void Initialize();
static void Destroy();
static void Resize();
static Boolean SetValues();
static Boolean Refresh();
static void Redisplay();
static Boolean Compile();
static XtGeometryResult GeometryManager();
static void InsertChild();
static void DeleteChild();
#if defined XPRINTER_USED
static Boolean Print();
#endif

/* actions del widget XlComposite */
static void Seleziona();
static void AddSelez();
static void DoLayout();

/*  altre funzioni  */
#if defined VMS
static double rint();
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
XlCompositeClassRec xlCompositeClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlManagerClassRec,
    /* class_name               */      "XlComposite",
    /* widget_size              */      sizeof(XlCompositeRec),
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
    /* insert_child              */     InsertChild,
    /* delete_child              */     DeleteChild,
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
  { /* XlComposite fields */
    /* empty                    */      0
  }
};

WidgetClass xlCompositeWidgetClass = (WidgetClass) &xlCompositeClassRec;

static XtGeometryResult GeometryManager(w, request, reply)
Widget w;
XtWidgetGeometry *request;
XtWidgetGeometry *reply;
{
XlCompositeWidget new;  /* indice di widget del composite */
int i;
new= (XlCompositeWidget)XtParent(w);
/*
 individua l'indice progressivo del child
*/
for(i=0; i< new->composite.num_children; i++)
	{
	if(new->composite.children[i]==w)
		break;
	}
if(request->request_mode & CWX)
	{
	new->xlcomposite.prect_figli[i].nx= 
		(float) request->x / (float) new->core.width;
	XtMoveWidget(w,request->x,w->core.y);
	}
if(request->request_mode & CWY)
	{
	new->xlcomposite.prect_figli[i].ny= 
		(float) request->y / (float) new->core.height;
	XtMoveWidget(w,w->core.x,request->y);
	}
if(request->request_mode & CWWidth)
	{
	new->xlcomposite.prect_figli[i].nwidth= 
		(float) request->width / (float) new->core.width;
	XtResizeWidget(w,request->width,w->core.height, w->core.border_width);
	}
if(request->request_mode & CWHeight)
	{
	new->xlcomposite.prect_figli[i].nheight= 
		(float) request->height / (float) new->core.height;
	XtResizeWidget(w,w->core.width,request->height, w->core.border_width);
	}
return XtGeometryYes;
}

static void Resize(w)
Widget w;
{
XlCompositeWidget cw= (XlCompositeWidget)w;

DoLayout(cw);
}

static void DoLayout(w)
Widget w;
{
int i,k;
Arg args[5];
Dimension width,height;
Position x,y;
double rint();
XlCompositeWidget cw= (XlCompositeWidget)w;
float f_zoom=1.;

/*
	se la gestione del layout dei children e' inibita ritorna
*/
if((cw->xlcomposite.childrenLayout == 0 )&&
     (cw->xlcomposite.fatt_zoom_last==cw->xlmanager.fatt_zoom))
	return;

/*
Verifico uscita da zoom
*/
if((cw->xlcomposite.childrenLayout == 0 )&&
     (cw->xlcomposite.fatt_zoom_last>cw->xlmanager.fatt_zoom))
        {
        f_zoom=(float)((float)cw->xlmanager.fatt_zoom / (float)cw->xlcomposite.fatt_zoom_last);
        cw->xlcomposite.fatt_zoom_last=cw->xlmanager.fatt_zoom;
        /*
        Ripristino le coordinate da rientro da zoom
        */        
        for(i=0;i<cw->composite.num_children;i++)
	   {
           /*
           Acquisisco valori risorse oggetti
           */
           XtVaGetValues(cw->composite.children[i],XmNx,&x,NULL);
           XtVaGetValues(cw->composite.children[i],XmNy,&y,NULL);
           XtVaGetValues(cw->composite.children[i],XmNheight,&height,NULL);
           XtVaGetValues(cw->composite.children[i],XmNwidth,&width,NULL);

/*
 Calcola la nuova posizione e dimensione dei figli utilizzando
 le dimensioni normalizzate
*/
	   x=rint((double)(x * f_zoom ));
	   y=rint((double)(y *  f_zoom));
	   width=rint((double)(width *  f_zoom));
	   height=rint((double)(height *  f_zoom));

	XtSetArg(args[0],XmNx,x); 
	XtSetArg(args[1],XmNy,y);
	XtSetArg(args[2],XmNwidth,width);
	XtSetArg(args[3],XmNheight,height);
	XtSetValues(cw->composite.children[i],args,4);
           /*
           Setto valori dim e posiz. children senza zoom
           */

           }




	return;
        }

/*
Entrata in layoutChildren
*/
cw->xlcomposite.fatt_zoom_last=cw->xlmanager.fatt_zoom;


for(i=0;i<cw->composite.num_children;i++)
	{
/*
 Calcola la nuova posizione e dimensione dei figli utilizzando
 le dimensioni normalizzate
*/
	x=rint((double)(cw->xlcomposite.prect_figli[i].nx * (float) cw->core.width));
	y=rint((double)(cw->xlcomposite.prect_figli[i].ny * (float) cw->core.height));
	width=rint((double)(cw->xlcomposite.prect_figli[i].nwidth * (float) cw->core.width));
	height=rint((double)(cw->xlcomposite.prect_figli[i].nheight * (float) cw->core.height));

/* 
 ridimensiona e riposiziona il child
*/
	XtSetArg(args[0],XmNx,x); 
	XtSetArg(args[1],XmNy,y);
	XtSetArg(args[2],XmNwidth,width);
	XtSetArg(args[3],XmNheight,height);
	XtSetValues(cw->composite.children[i],args,4);
	}
}

static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
int i,app_int;
int numero_figli;
Dimension width,height;
Position x,y;
int app_x,app_y;
extern WidgetClass XlGetClassWid();
Widget wchild;
char *str_elenco;
char class_name[20];
AN_OBJ *plista_an_obj;
Arg arg[1];


XlCompositeWidget new = (XlCompositeWidget)tnew;


if(new->core.width!=new->core.height)
        {
        if((new->xlcomposite.rotate==ROTATE_90)||
                (new->xlcomposite.rotate==ROTATE_270))
                        {
                        app_int = new->core.width;
                        new->core.width = new->core.height;
                        new->core.height = app_int;
                        }
        }



str_elenco=new->xlcomposite.list_children;
/*
 Alloca la lista degli oggetti da creare riservando l'ultimo
 elemento per contenere un NULL come terminatore della lista
*/
new->xlcomposite.pfigli = (AN_OBJ *)calloc(new->xlcomposite.num_figli+1,
                                                          sizeof(AN_OBJ));
/*
 Alloca la lista delle dimensioni dei figli normalizzate rispetto al
 composite che li contiene
*/
new->xlcomposite.prect_figli= NULL;

/*******
new->xlcomposite.prect_figli= (XlNormRectangle *)calloc(new->xlcomposite.num_figli,
						sizeof(XlNormRectangle));
********/
plista_an_obj=new->xlcomposite.pfigli;

numero_figli=new->xlcomposite.num_figli;

for(i=0;i<numero_figli; i++)
	{
	sscanf(str_elenco,"%s",plista_an_obj[i].nome);
	str_elenco+=(strlen(plista_an_obj[i].nome)+1);
	sscanf(str_elenco,"%s",class_name);
        str_elenco+=(strlen(class_name)+1);
	plista_an_obj[i].classe=XlGetClassWid(class_name);
/*
 Crea l' oggetto figlio assegnando l'attributo di config del padre
*/
        XtSetArg(arg[0],XlNconfig,new->xlmanager.config);
        wchild=XtCreateWidget(plista_an_obj[i].nome,
                                        plista_an_obj[i].classe,
                                        new,
                                        arg,1);



	XtManageChild(wchild);
        }

/*
Inizializzo l' ultimo fattore di zoom utilizzato
Utile per determinare se rifare il layout dei figli in caso
di zoom con childrenLayout=NO
*/
new->xlcomposite.fatt_zoom_last=new->xlmanager.fatt_zoom;

}

#define superclass (&xlManagerClassRec)

static void Seleziona(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/*
 Richiama la routine di select di XlComposite per la gestione della
 selezione
*/
(*superclass->xlmanager_class.select) (w,0);
}

static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/* richiama il metodo di select */
(*superclass->xlmanager_class.select)(w,1);
}


static void InsertChild(w,args,num_args)
Widget w;
ArgList args;
Cardinal *num_args;
{
XlCompositeWidget new;  /* indice di widget del composite */
Position x,y,app_x,app_y;
Dimension width,height;
Cardinal size_list_children;
int i;
float zoom;
new= (XlCompositeWidget)XtParent(w);
(*superclass->composite_class.insert_child) (w);
/*
 Ingrandisce lo spazio per la memorizzazione di nome e classe
 del figlio inserito
*/
/*
size_list_children= strlen(new->xlcomposite.list_children);
new->xlcomposite.list_children= XtRealloc(new->xlcomposite.list_children,_LUN_NAME_CLASS * new->composite.num_children);
*/
/*
 Inserisce nome e classe del figlio nella lista
*/
/*
sprintf(&new->xlcomposite.list_children[strlen(new->xlcomposite.list_children)],
	" %s %s",XtName(w),XlGetClassName(XtClass(w)));
*/
/*
 aggiorna il contatore dei figli di xlcomposite
*/
new->xlcomposite.num_figli= new->composite.num_children;
/*
 Realloca la lista delle dimensioni dei figli normalizzate rispetto al
 composite che li contiene
*/
if(new->xlcomposite.num_figli==1)
	new->xlcomposite.prect_figli=NULL;

new->xlcomposite.prect_figli= 
	(XlNormRectangle *)XtRealloc(new->xlcomposite.prect_figli,
				new->xlcomposite.num_figli *
				sizeof(XlNormRectangle));

/*
	Posiziona il figli rispetto alle eventuali rotazioni del padre 
*/
zoom=(float)new->xlmanager.fatt_zoom/100.;
XtVaGetValues( w, XlNx0, &x, XlNy0, &y, 
		XmNwidth, &width, XmNheight, &height, NULL );
app_x = x*zoom; app_y = y*zoom;
if(XlIsIconReg(new))
	gest_trasf_coor_composite(&app_x,&app_y,width,height, 
		new->core.width-1,new->core.height-1,
		new->xlcomposite.rotate,new->xlcomposite.ass_rotate);
else
	gest_trasf_coor_composite(&app_x,&app_y,width,height, 
		new->core.width,new->core.height,
		new->xlcomposite.rotate,new->xlcomposite.ass_rotate);
x = app_x; y = app_y;
XtVaSetValues( w, XmNx, x, XmNy, y, NULL );

/*
 ricava le dimensioni del figlio
*/
XtVaGetValues( w,
               XmNx, &x,
               XmNy, &y,
               XmNwidth, &width,
               XmNheight, &height,
               NULL );
/*
 Normalizza rispetto alle dimensione di composite le dimensioni
 dei figli e le loro coordinate
*/	

i=new->xlcomposite.num_figli-1;
new->xlcomposite.prect_figli[i].nx= (float) x / (float) new->core.width;
new->xlcomposite.prect_figli[i].ny= (float) y / (float) new->core.height;
new->xlcomposite.prect_figli[i].nheight= (float) height / (float) new->core.height;
new->xlcomposite.prect_figli[i].nwidth= (float) width / (float) new->core.width;
}


static void DeleteChild(w,args,num_args)
Widget w;
ArgList args;
Cardinal *num_args;
{
int i;
Dimension width,height;
Position x,y;
XlCompositeWidget new;  /* indice di widget del composite */
(*superclass->composite_class.delete_child) (w);
new= (XlCompositeWidget)XtParent(w);
/*
 aggiorna il contatore dei figli di xlcomposite
*/
new->xlcomposite.num_figli= new->composite.num_children;
/*
 Libera la lista dei rettangoli descriventi i figli e la
 ricalcola per i figli attuali
*/
XtFree(new->xlcomposite.prect_figli); 
new->xlcomposite.prect_figli= (XlNormRectangle *)XtCalloc(
					new->xlcomposite.num_figli,
					sizeof(XlNormRectangle));
for(i=0; i< new->composite.num_children; i++)
	{
/*
 ricava le dimensioni del figlio
*/
	XtVaGetValues( new->composite.children[i],
               XmNx, &x,
               XmNy, &y,
               XmNwidth, &width,
               XmNheight, &height,
               NULL );
/*
 Normalizza rispetto alle dimensione di composite le dimensioni
 dei figli e le loro coordinate
*/	
	new->xlcomposite.prect_figli[i].nx= (float) x / (float) new->core.width;
	new->xlcomposite.prect_figli[i].ny= (float) y / (float) new->core.height;
	new->xlcomposite.prect_figli[i].nheight= (float) height / (float) new->core.height;
	new->xlcomposite.prect_figli[i].nwidth= (float) width / (float) new->core.width;

	}
}

static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
Arg comp_args[2];
XlCompositeWidget curcw= (XlCompositeWidget) current;
XlCompositeWidget newcw= (XlCompositeWidget) new;
Boolean do_redisplay = False;
float zoom;


/*
        risorsa width
*/
if(curcw->core.width != newcw->core.width)
        {
	zoom=(float)newcw->xlmanager.fatt_zoom/100;
        if(newcw->core.width<MIN_WIDTH)
                newcw->core.width=MIN_WIDTH;
        if((newcw->xlcomposite.rotate==ROTATE_90)||
                (newcw->xlcomposite.rotate==ROTATE_270))
                        newcw->xlmanager.height0=newcw->core.width/zoom;
        else
                        newcw->xlmanager.width0=newcw->core.width/zoom;
        do_redisplay = True;
        }

/*
        risorsa height
*/
if(curcw->core.height != newcw->core.height)
        {
	zoom=(float)newcw->xlmanager.fatt_zoom/100;
        if(newcw->core.height<MIN_HEIGHT)
                newcw->core.height=MIN_HEIGHT;
        if((newcw->xlcomposite.rotate==ROTATE_90)||
                (newcw->xlcomposite.rotate==ROTATE_270))
                newcw->xlmanager.width0=newcw->core.height/zoom;
        else
                newcw->xlmanager.height0=newcw->core.height/zoom;
        do_redisplay = True;
        }

/*
        risorsa width0
*/
if(curcw->xlmanager.width0 != newcw->xlmanager.width0)
        {
	zoom=(float)newcw->xlmanager.fatt_zoom/100;
        if(newcw->xlmanager.width0<MIN_WIDTH)
                newcw->xlmanager.width0=MIN_WIDTH;
        if((newcw->xlcomposite.rotate==ROTATE_90)||
                (newcw->xlcomposite.rotate==ROTATE_270))
                newcw->core.height=newcw->xlmanager.width0*zoom;
        else
                newcw->core.width=newcw->xlmanager.width0*zoom;
        do_redisplay = True;
        }
/*
        risorsa height0
*/
if(curcw->xlmanager.height0 != newcw->xlmanager.height0)
        {
	zoom=(float)newcw->xlmanager.fatt_zoom/100;
        if(newcw->xlmanager.height0<MIN_HEIGHT)
                newcw->xlmanager.height0=MIN_HEIGHT;
        if((newcw->xlcomposite.rotate==ROTATE_90)||
                (newcw->xlcomposite.rotate==ROTATE_270))
                newcw->core.width=newcw->xlmanager.height0*zoom;
        else
                newcw->core.height=newcw->xlmanager.height0*zoom;
        do_redisplay = True;
        }


return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlCompositeWidget cw= (XlCompositeWidget) w;
if(cw->xlcomposite.pfigli!=NULL)
	XtFree(cw->xlcomposite.pfigli);
if(cw->xlcomposite.prect_figli!=NULL)
	XtFree(cw->xlcomposite.prect_figli);
}

static Boolean Refresh(w)
Widget w;
{
int i;
XlCoreClassRec *wclass;
XlManagerClassRec *wclassM;

XlCompositeWidget cw= (XlCompositeWidget)w;
for(i=0;i<cw->composite.num_children;i++)
	{
	if(XlIsXlCore(cw->composite.children[i]))
	   {
	   wclass= (XlCoreClassRec *)XtClass(cw->composite.children[i]);
	   if(!((wclass->xlcore_class.refresh)(cw->composite.children[i])))
		{
		XlWarning("XlManager","Refresh",
			"Errore refresh di un oggetto contenuto");
		return(False);
		}
	   }
	else
	   {
	   wclassM= (XlManagerClassRec *)XtClass(cw->composite.children[i]);
	   if(!((wclassM->xlmanager_class.refresh)(cw->composite.children[i])))
		{
		XlWarning("XlManager","Refresh",
			"Errore refresh di un oggetto contenuto");
		return(False);
		}
	   }
	}
return(True);
}

static Boolean Compile(w)
Widget w;
{
int i;
XlCoreClassRec *wclass;
XlManagerClassRec *wclassM;
Boolean ret=True;

XlCompositeWidget cw= (XlCompositeWidget)w;
for(i=0;i<cw->composite.num_children;i++)
	{
	if(XlIsXlCore(cw->composite.children[i]))
		  {
		  wclass= (XlCoreClassRec *)XtClass(cw->composite.children[i]);
		  if(!((wclass->xlcore_class.compile)(cw->composite.children[i],
                        wclass->xlcore_class.compilatore)))
			  {
			   XlErrComp(w,"Compile","Error in children",
			 	XtName(cw->composite.children[i]));
			  ret=False;
		          }
		  }
	  else
		  {
		  wclassM= (XlManagerClassRec *)XtClass(cw->composite.children[i]);
		  if(!((wclassM->xlmanager_class.compile)(cw->composite.children[i],
                        wclassM->xlmanager_class.compilatore)))
			  {
			   XlErrComp(w,"Compile","Error in children",
			 	XtName(cw->composite.children[i]));
			  ret=False;
			  }
		  }
	}
return(ret);
}


static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlCompositeWidget cw= (XlCompositeWidget)w;
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


XClearArea(XtDisplay(cw),XtWindow(cw),x,y,width,height,False);
/*
XFillRectangle(XtDisplay(cw),XtWindow(cw),
	cw->xlmanager.sfondo_gc,x,y,width,height);
*/

/*
        Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
*/
(*superclass->xlmanager_class.drawBorderIfSelected) (w);


}

      
/*
 XlIsXlComposite(w) funzione per sapere se l'oggetto w
 e' del tipo XlComposite
*/
Boolean XlIsXlComposite(Widget w)
{
CoreWidgetClass wclass;
wclass=XtClass(w);
if((strcmp(wclass->core_class.class_name,"XlComposite")==0) ||
	(XtIsSubclass(w,&xlCompositeClassRec)))
	return(True);
else
	return(False);
}


#if defined VMS
static double rint( val)
double val;
{
int app_int;
double ret;
        app_int=(int)val;
        ret=(double)app_int;
return(ret);
}
#endif


/*
        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
int i;
XlCoreClassRec *wclass;
XlManagerClassRec *wclassM;
Boolean ret=True;
XlCompositeWidget cw= (XlCompositeWidget)w;


XlPrintBackgroundAndBorder(w,cw->xlmanager.trasparent);

/*
	richiama il metodo di stampa dei figli
*/
/****
for(i=0;i<cw->composite.num_children;i++)
	{
	if(XlIsXlCore(cw->composite.children[i]))
		  {
		  wclass= (XlCoreClassRec *)XtClass(cw->composite.children[i]);
		  if(!((wclass->xlcore_class.print)(cw->composite.children[i])))
			  ret=False;
		  }
	  else
		  {
		  wclassM= (XlManagerClassRec *)XtClass(cw->composite.children[i]);
		  if(!((wclassM->xlmanager_class.print)(cw->composite.children[i])))
			  ret=False;
		  }
	}
****/
return(ret);
}
#endif
