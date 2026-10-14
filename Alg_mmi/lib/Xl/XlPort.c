/**********************************************************************
*
*       C Source:               XlPort.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:36:07 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlPort.c-8.2.4 %  (%full_filespec: XlPort.c-8.2.4:csrc:1 %)";
#endif
/*
   modulo XlPort.c
   tipo 
   release 5.2
   data 1/5/96
   reserved @(#)XlPort.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlPort.c	5.2\t1/5/96";
/*
        Fine sezione per SCCS
*/
/*
 *  XlPort.c - widget port per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <X11/keysym.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlPortP.h>
#include <Xm/Label.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

#define DEFAULT_ARROW_WIDTH 8
#define DEFAULT_ARROW_HEIGHT 8
#define DEFAULT_SQUARE_WIDTH 4
#define DEFAULT_SQUARE_HEIGHT 4

#define PORT_SNAP  4

/* lista delle risorse  */
static XtResource resources[]= {
        {
        XlNdigitalPortColor,
        XlCDigitalPortColor,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlPortRec,port.color_digital_port),
        XmRString,
        XlDefaultColorDigitalPort
        },
        {
        XlNanalogPortColor,
        XlCAnalogPortColor,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlPortRec,port.color_analog_port),
        XmRString,
        XlDefaultColorAnalogPort
        },
        {
        XlNPortColorStartConnection,
        XlCPortColorStartConnection,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlPortRec,port.color_port_start_connection),
        XmRString,
        XlDefaultColorStartConnection
        },
        {
        XlNPortColorConnected,
        XlCPortColorConnected,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlPortRec,port.color_port_connected),
        XmRString,
        XlDefaultColorConnected
        },
        {
        XlNlabelForegroundColor,
        XlCLabelForegroundColor,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlPortRec,port.color_foreground_label),
        XmRString,
        "black"
        },
        {
        XlNlabelBackgroundColor,
        XlCLabelBackgroundColor,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlPortRec,port.color_background_label),
        XmRString,
        "white"
        },
        {
        XlNtipoPort,
        XlCTipoPort,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPortRec,port.tipo_port),
        XmRImmediate,
        (XtPointer)PORT_DEFAULT
        },
        {
        XlNpositionTag,
        XlCPositionTag,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPortRec,port.position_tag),
        XmRImmediate,
        (XtPointer)TAG_POSITION_DEFAULT
        },
        {
        XlNlayoutPort,
        XlCLayoutPort,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPortRec,port.layout_port),
        XmRImmediate,
        (XtPointer)DEFAULT_LAYOUT_PORT
        },
        {
        XlNinterfacePort,
        XlCInterfacePort,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPortRec,port.interface_port),
        XmRImmediate,
        (XtPointer)DEFAULT_INTERFACE_PORT
        },
        {
        XlNportName,
        XlCPortName,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPortRec,port.nome_port),
        XmRImmediate,
        "--"
        },
        {
        XlNportNameConnect,
        XlCPortNameConnect,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPortRec,port.nome_port_connect),
        XmRImmediate,
        ""
        },
        {
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPortRec,port.normalfont),
        XmRImmediate,
        "fixed"
        },
        {
        XlNconnectMode,
        XlCConnectMode,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPortRec,port.connectMode),
        XmRImmediate,
        (XtPointer)CONNESSIONE_PORTE_DEFAULT
        },
        {
        XlNvisibleMode,
        XlCVisibleMode,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPortRec,port.visibleMode),
        XmRImmediate,
        (XtPointer)VISIBLE_PORT_STATE_DEFAULT
        },
        };

/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
        {
        XlNdigitalPortColor,
        XlCDigitalPortColor,
        XlDdigitalPortColor,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlPortRec,port.color_digital_port),
        sizeof(Pixel)
        },
        {
        XlNanalogPortColor,
        XlCAnalogPortColor,
        XlDanalogPortColor,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlPortRec,port.color_analog_port),
        sizeof(Pixel)
        },
        {
        XlNlabelForegroundColor,
        XlCLabelForegroundColor,
        XlDlabelForegroundColor,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlPortRec,port.color_foreground_label),
        sizeof(Pixel)
        },
        {
        XlNlabelBackgroundColor,
        XlCLabelBackgroundColor,
        XlDlabelBackgroundColor,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlPortRec,port.color_background_label),
        sizeof(Pixel)
        },
        {
        XlNtipoPort,
        XlCTipoPort,
        XlDtipoPort,
        XlROption+LOCKED_PAG,
        NULL,
        XlOtipoPort,
        XlRGrIo,
        XtOffsetOf(XlPortRec,port.tipo_port),
        sizeof(int)
        },
        {
        XlNlayoutPort,
        XlCLayoutPort,
        XlDlayoutPort,
        XlROption+LOCKED_PAG,
        NULL,
        XlOlayoutPort,
        XlRGrIo,
        XtOffsetOf(XlPortRec,port.layout_port),
        sizeof(int)
        },
        {
        XlNinterfacePort,
        XlCInterfacePort,
        XlDinterfacePort,
        XlROption+LOCKED_PAG,
        NULL,
        XlOinterfacePort,
        XlRGrIo,
        XtOffsetOf(XlPortRec,port.interface_port),
        sizeof(int)
        },
        {
        XlNpositionTag,
        XlCPositionTag,
        XlDpositionTag,
        XlROption,
        NULL,
        XlOpositionTag,
        XlRGrIo,
        XtOffsetOf(XlPortRec,port.position_tag),
        sizeof(int)
        },
        {
        XlNportName,
        XlCPortName,
        XlDportName,
        XlRTipoVarPort+LOCKED_PAG,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPortRec,port.nome_port),
        sizeof(char *)
        },
        {
        XlNportNameConnect,
        XlCPortNameConnect,
        XlDportNameConnect,
        XlRText+LOCKED_PAG_LIB,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPortRec,port.nome_port_connect),
        sizeof(char*)
        },
        {
        XlNnormalFont,
        XlCNormalFont,
        XlDnormalFont,
        XlRFont,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPortRec,port.normalfont),
        sizeof(XFontStruct*)
        },
/************************************************

              le due risorse che seguono non possono essere configurate

        {
        XlNconnectMode,
        XlCConnectMode,
        XlDconnectMode,
        XlROption+LOCKED_PAG,
        NULL,
        XlOconnectMode,
        XlRGrIo,
        XtOffsetOf(XlPortRec,port.connectMode),
        sizeof(int),
        },
        {
        XlNvisibleMode,
        XlCVisibleMode,
        XlDvisibleMode,
        XlROption+LOCKED_PAG,
        NULL,
        XlOvisibleMode,
        XlRGrIo,
        XtOffsetOf(XlPortRec,port.visibleMode),
        sizeof(int),
        },
*****************************/
	};

/* dichiarazioni funzioni varie */
static void PosizionaLabel();
static void CreaLabel();
static void PortSelect();
static void CreaStringaInterface();
         
/* dichiarazione dei metodi (methods) */
static void Seleziona();
static void AddSelez();
static void MoveTasti();
static void EntraMouse();
static void EsciMouse();
static void Initialize();
static void Destroy();
static void Redisplay();
static Boolean SetValues();
static Boolean Compile();
static Boolean Refresh();
static Boolean CheckPort();
#if defined XPRINTER_USED
static Boolean Print();
#endif

/* dichiarazioni funzioni varie */
static Boolean LoadFont();

/* translations  */
static char defaultTranslations[]=
        "~Shift<Btn1Down>:     Seleziona()  \n\
        <Key>Return:     Seleziona()  \n\
        <KeyPress>Left:move_tasti()\n\
        <KeyPress>Right:move_tasti()\n\
        <KeyPress>Up:move_tasti()\n\
        <KeyPress>Down:move_tasti()\n\
        Shift<Btn1Down>:       AddSelez()    \n\
        <EnterWindow>:         EntraMouse()       \n\
        <LeaveWindow>:         EsciMouse()";

static XtActionsRec actions[] = {
        {"Seleziona",Seleziona},
        {"AddSelez",AddSelez},
        {"EntraMouse",EntraMouse},
        {"move_tasti",MoveTasti},
        {"EsciMouse",EsciMouse}
        };

/* Inizializzazione del class record */
XlPortClassRec xlPortClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "Port",
    /* widget_size              */      sizeof(XlPortRec),
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
  { /* port fields */
    /* stato delle porte */             CONNESSIONE_PORTE_NON_ATTIVA,
    /* indice della porta */		NULL,
    /* procedura da chiamare per
       la selezione             */      False,
    /* checkPort                */      CheckPort
  }
};

WidgetClass xlPortWidgetClass = (WidgetClass) &xlPortClassRec;

static void GetNormFgGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlPortWidget cw= (XlPortWidget) w;

#if defined XPRINTER_USED
if(modoPrint)
	values.background = XlPrintGetPixel(w,cw->core.background_pixel);
else
	values.background = cw->core.background_pixel;
#else
values.background = cw->core.background_pixel;
#endif
values.line_width = 0;
values.line_style = LineSolid;

#if defined XPRINTER_USED
if(modoPrint)
	{
	values.foreground = XlPrintGetPixel(w,cw->port.color_digital_port);
	cw->port.digital_gc = XCreateGC(XlDisplay(w), XlWindow(w), valuemask, &values);
	}
else
	{
	values.foreground = cw->port.color_digital_port;
	cw->port.digital_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->port.color_digital_port;
cw->port.digital_gc = XtGetGC(cw, valuemask, &values);
#endif

#if defined XPRINTER_USED
if(modoPrint)
	{
	values.foreground = XlPrintGetPixel(w,cw->port.color_analog_port);
	cw->port.analog_gc = XCreateGC(XlDisplay(w), XlWindow(w), valuemask, &values);
	}
else
	{
	values.foreground = cw->port.color_analog_port;
	cw->port.analog_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->port.color_analog_port;
cw->port.analog_gc = XtGetGC(cw, valuemask, &values);
#endif

#if defined XPRINTER_USED
if(modoPrint)
	{
	values.foreground = XlPrintGetPixel(w,cw->port.color_port_start_connection);
	cw->port.start_connection_gc = XCreateGC(XlDisplay(w), XlWindow(w), valuemask, &values);
	}
else
	{
	values.foreground = cw->port.color_port_start_connection;
	cw->port.start_connection_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->port.color_port_start_connection;
cw->port.start_connection_gc = XtGetGC(cw, valuemask, &values);
#endif

#if defined XPRINTER_USED
if(modoPrint)
	{
	values.foreground = XlPrintGetPixel(w,cw->port.color_port_connected);
	cw->port.connected_gc = XCreateGC(XlDisplay(w), XlWindow(w), valuemask, &values);
	}
else
	{
	values.foreground = cw->port.color_port_connected;
	cw->port.connected_gc = XtGetGC(cw, valuemask, &values);
	}
#else
values.foreground = cw->port.color_port_connected;
cw->port.connected_gc = XtGetGC(cw, valuemask, &values);
#endif



}

static void GetAllGCs(w,modoPrint)
Widget w;
Boolean modoPrint;
{
GetNormFgGC(w,modoPrint);
}

static void DrawDirectly(w)
Widget w;
{
XlPortWidget cw= (XlPortWidget)w;
XPoint p[5];
int x,y;
int wx,wy;
Position offX,offY;

offX=XlPrintOffsetX(w);
offY=XlPrintOffsetY(w);


if(cw->port.visibleMode == PORT_UNMANAGED)
	return;


/*
	Se la porta di interfaccia e' connessa viene evidenziata
*/
if(cw->port.nome_port_connect != NULL)
  if((strlen(cw->port.nome_port_connect)!=0)&&XlIsInterfacePort((Widget)cw))
	{
	x =  0 ; y = 0 ; gest_trasf_coor((Widget)cw,&x,&y); p[0].x=x + offX;  p[0].y=y + offY;
	x = 100 ; y = 0 ; gest_trasf_coor((Widget)cw,&x,&y); p[1].x=x + offX;  p[1].y=y + offY;
	x = 100 ; y = 100 ; gest_trasf_coor((Widget)cw,&x,&y); p[2].x=x + offX;  p[2].y=y + offY;
	x =  0 ; y = 100 ; gest_trasf_coor((Widget)cw,&x,&y); p[3].x=x + offX;  p[3].y=y + offY;
	x =  0 ; y = 0 ; gest_trasf_coor((Widget)cw,&x,&y); p[4].x=x + offX;  p[4].y=y + offY;
	XFillPolygon(XlDisplay(w),XlWindow(w),cw->port.connected_gc,
             p,5,Complex,CoordModeOrigin);
	}

/*
	Disegna la porta di start della connessione
*/
if(cw == (XlPortWidget)xlPortClassRec.xlport_class.wport)
	{
	x =  0 ; y = 0 ; gest_trasf_coor((Widget)cw,&x,&y); p[0].x=x + offX;  p[0].y=y + offY;
	x = 100 ; y = 0 ; gest_trasf_coor((Widget)cw,&x,&y); p[1].x=x + offX;  p[1].y=y + offY;
	x = 100 ; y = 100 ; gest_trasf_coor((Widget)cw,&x,&y); p[2].x=x + offX;  p[2].y=y + offY;
	x =  0 ; y = 100 ; gest_trasf_coor((Widget)cw,&x,&y); p[3].x=x + offX;  p[3].y=y + offY;
	x =  0 ; y = 0 ; gest_trasf_coor((Widget)cw,&x,&y); p[4].x=x + offX;  p[4].y=y + offY;
	XFillPolygon(XlDisplay(w),XlWindow(w),cw->port.start_connection_gc,
             p,5,Complex,CoordModeOrigin);
	}

if(cw->port.layout_port == PORT_ARROW)
  {
  if((cw->port.tipo_port==OUTPUT_PORT_A)||(cw->port.tipo_port==OUTPUT_PORT_D))
	{
	x = 0 ; y = 60 ; gest_trasf_coor((Widget)cw,&x,&y); p[0].x=x + offX;  p[0].y=y + offY;
	x = 0 ; y = 60 ; gest_trasf_coor((Widget)cw,&x,&y); p[1].x=x + offX;  p[1].y=y + offY;
	x = 100 ; y = 20 ; gest_trasf_coor((Widget)cw,&x,&y); p[2].x=x + offX;  p[2].y=y + offY;
	x = 100 ; y = 90 ; gest_trasf_coor((Widget)cw,&x,&y); p[3].x=x + offX;  p[3].y=y + offY;
	}
  else
	{
	x = 0 ; y = 20 ; gest_trasf_coor((Widget)cw,&x,&y); p[0].x=x + offX;  p[0].y=y + offY;
	x = 0 ; y = 90 ; gest_trasf_coor((Widget)cw,&x,&y); p[1].x=x + offX;  p[1].y=y + offY;
	x = 100 ; y = 60 ; gest_trasf_coor((Widget)cw,&x,&y); p[2].x=x + offX;  p[2].y=y + offY;
	x = 100 ; y = 60 ; gest_trasf_coor((Widget)cw,&x,&y); p[3].x=x + offX;  p[3].y=y + offY;
	}

  if((cw->port.tipo_port==OUTPUT_PORT_D)||(cw->port.tipo_port==INPUT_PORT_D))
	XFillPolygon(XlDisplay(w),XlWindow(w),cw->port.digital_gc,
             p,4,Complex,CoordModeOrigin);
  else
	XFillPolygon(XlDisplay(w),XlWindow(w),cw->port.analog_gc,
             p,4,Complex,CoordModeOrigin);
  }
else
  {
if(cw->port.nome_port_connect != NULL)
  if((strlen(cw->port.nome_port_connect)!=0)&&XlIsInterfacePort((Widget)cw))
	{
	x=20;
	y=20;
	wx=60;
	wy=60;
	}	
  else
	{
	x=0;
        y=0;
        wx=100;
        wy=100;
	}
  if((cw->port.tipo_port==OUTPUT_PORT_D)||(cw->port.tipo_port==INPUT_PORT_D))
	XlRiempiRettangolo(w,XlWindow(w),cw->port.digital_gc,
             x,y,wx,wy);
  else
	XlRiempiRettangolo(w,XlWindow(w),cw->port.analog_gc,
             x,y,wx,wy);
  }
}


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
XlPortWidget new = (XlPortWidget)tnew;
Arg arg[15];
Pixel back_parent;
int i;
Dimension pw,ph,x,y;

/*
	le dimensioni dell'oggetto XlPort sono fisse
*/
if(new->port.layout_port == PORT_ARROW)
	{
	if(new->xlcore.width0 != DEFAULT_ARROW_WIDTH)
		new->xlcore.width0 = DEFAULT_ARROW_WIDTH;
	if(new->core.width != DEFAULT_ARROW_WIDTH)
		new->core.width = DEFAULT_ARROW_WIDTH;
	if(new->xlcore.height0 != DEFAULT_ARROW_HEIGHT)
		new->xlcore.height0 = DEFAULT_ARROW_HEIGHT;
	if(new->core.height != DEFAULT_ARROW_HEIGHT)
		new->core.height = DEFAULT_ARROW_HEIGHT;
	}
else
	{
	if(new->xlcore.width0 != DEFAULT_SQUARE_WIDTH)
		new->xlcore.width0 = DEFAULT_SQUARE_WIDTH;
	if(new->core.width != DEFAULT_SQUARE_WIDTH)
		new->core.width = DEFAULT_SQUARE_WIDTH;
	if(new->xlcore.height0 != DEFAULT_SQUARE_HEIGHT)
		new->xlcore.height0 = DEFAULT_SQUARE_HEIGHT;
	if(new->core.height != DEFAULT_SQUARE_HEIGHT)
		new->core.height = DEFAULT_SQUARE_HEIGHT;
	}

/*
	Se la porta non e' in config non puo' essere in connectMode
*/
if(new->xlcore.config==False)
	new->port.connectMode = False;

new->port.wLabel=NULL;

/*
	Se la porta e' in config deve essere visibile
*/
if(new->xlcore.config==True)
	new->port.visibleMode = VISIBLE_PORT_ON;

if(!LoadFont(new,new->port.normalfont,False))
        if(!LoadFont(new,"fixed",False))
                XlError("XlPort","Initialize",
			"Impossibile caricare default font");

GetAllGCs(new,False);

/*
	se necessario crea la label
*/
if((new->port.visibleMode == PORT_TAG)&&(new->port.nome_port!=NULL))
 {
  CreaLabel(new);
  PosizionaLabel(new);
  XtManageChild(new->port.wLabel);
  XRaiseWindow(XtDisplay(new->port.wLabel),
  XtWindow(new->port.wLabel));
 }

}

#define superclass (&xlCoreClassRec)

static void MoveTasti(wid,ev,params,num_params)
Widget wid;
XEvent *ev;
String *params;
Cardinal *num_params;
{
KeySym tasto;
Modifiers mod;
int x,y;


x=ev->xkey.x;
y=ev->xkey.y;
tasto=XtGetActionKeysym(ev,&mod);
if(tasto == XK_Left) x -=1;
if(tasto == XK_Right) x +=1;
if(tasto == XK_Up) y -=1;
if(tasto == XK_Down) y +=1;
XWarpPointer(XtDisplay(wid),XtWindow(wid),XtWindow(wid),
                (int)0,(int)0,(unsigned int)0,(unsigned int)0, x,y);
}

static void Seleziona(w,event,params,num_params)
Widget w;
XEvent *event;
String *params;
Cardinal *num_params;
{
/*
 Richiama la routine di select di XlCore per la gestione della
 selezione
*/
(*superclass->xlcore_class.select) (w,0);

/*
	Richiama la Routine di selezione delle porte per il
	disegno delle connessioni
*/
PortSelect(w);
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

static void EntraMouse(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
XlPortWidget cw= (XlPortWidget) w;

if(!cw->port.connectMode)
	return;

if(XlIsInterfacePort((Widget)cw))
	return;

	CreaLabel(cw);
	PosizionaLabel(cw);
	XtManageChild(cw->port.wLabel);
	XRaiseWindow(XtDisplay(cw->port.wLabel),XtWindow(cw->port.wLabel));
}

static void EsciMouse(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
XlPortWidget cw= (XlPortWidget) w;

if(!cw->port.connectMode)
	return;

if(XlIsInterfacePort((Widget)cw))
	return;

if(cw->port.wLabel!=NULL)
	{
	XtDestroyWidget(cw->port.wLabel);
	cw->port.wLabel=NULL;
	}
}


static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
XlPortWidget curcw= (XlPortWidget) current;
XlPortWidget newcw= (XlPortWidget) new;
char *app_char;
Boolean do_redisplay = False;

if(curcw->xlcore.width0 != newcw->xlcore.width0)
	newcw->xlcore.width0=curcw->xlcore.width0;

if(curcw->xlcore.height0 != newcw->xlcore.height0)
	newcw->xlcore.height0=curcw->xlcore.height0;

if(curcw->port.nome_port_connect != newcw->port.nome_port_connect)
	{
/*
	XtFree(curcw->port.nome_port_connect);
*/
	app_char=newcw->port.nome_port_connect;
	newcw->port.nome_port_connect=
                XtNewString(app_char);
	}

if(curcw->port.visibleMode != newcw->port.visibleMode)
        {
        if(newcw->port.visibleMode == PORT_MANAGED)
                XtManageChild(newcw);
        if(newcw->port.visibleMode == PORT_UNMANAGED)
		{
                XtUnmanageChild(newcw);
		}
        if(newcw->port.visibleMode == PORT_TAG)
                {
		if(!XtIsManaged(newcw))
                	XtManageChild(newcw);
		 if((newcw->port.wLabel==NULL)&&
			(newcw->port.nome_port!=NULL))
			{
		  CreaLabel(newcw);
		  PosizionaLabel(newcw);
		  XtManageChild(newcw->port.wLabel);
		  XRaiseWindow(XtDisplay(newcw->port.wLabel),
			  XtWindow(newcw->port.wLabel));
			}
                }
	else
		{
		 if(newcw->port.wLabel!=NULL)
			{
			  XtDestroyWidget(newcw->port.wLabel);
			  newcw->port.wLabel=NULL;
			}
		}
        do_redisplay = True;
        }

return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlPortWidget cw= (XlPortWidget) w;

/*
	se necessario distruggo la label
*/
if(cw->port.wLabel!=NULL)
	 XtDestroyWidget(cw->port.wLabel);
/*
        Rilascio i GC
*/
if (cw->port.digital_gc)
        XtReleaseGC(cw,cw->port.digital_gc);
if (cw->port.analog_gc)
        XtReleaseGC(cw,cw->port.analog_gc);

if (cw->port.start_connection_gc)
	XtReleaseGC(cw,cw->port.start_connection_gc);

if (cw->port.connected_gc)
	XtReleaseGC(cw,cw->port.connected_gc);

/* libero la font_struct */
/*
if(cw->port.font_info)
        XFreeFont(XtDisplay(cw),cw->port.font_info);
*/


}


static Boolean Compile(Widget w)
{
Boolean errors=False;
XlPortWidget cw= (XlPortWidget)w;

if(!XtIsWidget(w))
        {
        XlWarning("XlPort","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlPortClassRec.xlcore_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
        return(False);
        }

if(!(*superclass->xlcore_class.compile) (w,
     xlPortClassRec.xlcore_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
        return(False);
        }
XlEvError(cw,errors);
return(True);
}

static Boolean Refresh(Widget w)
{
XlPortWidget cw= (XlPortWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;


if(cw->xlcore.config==True)
        return(True);

if(!XtIsWidget(w))
        {
        XlWarning("XlPort","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlPortClassRec.xlcore_class.database==NULL)
        {
        XlWarning("XlPort","Refresh","database non definito");
        return(False);
        }
return(True);
}




static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlPortWidget cw= (XlPortWidget)w;
register int x,y;
unsigned int width,height;


if(cw->port.visibleMode == PORT_UNMANAGED)
	{
	XtUnmanageChild(cw);
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


(*superclass->core_class.expose) (w,event,NULL);

DrawDirectly(w);

/*
        Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
*/
(*superclass->xlcore_class.drawBorderIfSelected) (w);

}

static Boolean LoadFont( cw,fontname,modoPrint)
Widget cw;
char fontname[];
Boolean modoPrint;
{
XlPortWidget Xl_cw = (XlPortWidget) cw;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
/* Carica il font ottenendo la descrizione del font stesso */

char **pf;
int i,nf;
Display *disp;

if(modoPrint)
	{
        disp=XlDisplay(cw);
	if((Xl_cw->port.font_info = XLoadQueryFont(disp,fontname)) == NULL)
        	{
        	XlWarning("XlPort","LoadFont","Cannot open font");
        	return(False);
        	}
	}
else
	{
        disp=XtDisplay(cw);
	if((Xl_cw->port.font_info = XlGetFont(disp,fontname)) == NULL)
        	{
        	XlWarning("XlPort","LoadFont","Cannot open font");
        	return(False);
        	}
	}
return(True);
}

static void CreaLabel(Widget w)
{
XlPortWidget new = (XlPortWidget) w;
Widget PadreLabel;
Arg arg[15];
int i;
Dimension pw,ph,x,y;
char stringa_interface[15];
XmString xm_stringa;

/*
	creazione della label
*/
if(XlIsXlManager(XtParent(new)))
	PadreLabel=XtParent(XtParent(new));
else
	PadreLabel=XtParent(new);
/*
PadreLabel=XtParent(PadreLabel);
*/

if(new->core.x<ph/2)
	x = new->core.x + DEFAULT_ARROW_WIDTH;
else
	x = new->core.x -20;
y=new->core.y;

new->port.font_list =
XmFontListCreate ( new->port.font_info, XmSTRING_DEFAULT_CHARSET);
if (new->port.font_list == NULL)
       	{
       	XlWarning("XlPort","LoadFont","FONTLIST NULL");
       	}
i=0;
XtSetArg(arg[i],XmNx,x);i++;
XtSetArg(arg[i],XmNy,y);i++;
/*
XtSetArg(arg[i],XmNwidth,20);i++;
XtSetArg(arg[i],XmNheight,20);i++;
*/
XtSetArg(arg[i],XmNrecomputeSize,True);i++;
XtSetArg(arg[i],XmNfontList,new->port.font_list);i++;
XtSetArg(arg[i],XmNforeground,new->port.color_foreground_label);i++;
XtSetArg(arg[i],XmNbackground,new->port.color_background_label);i++;
if(XlIsInterfacePort((Widget)new))
	{
	CreaStringaInterface(new, stringa_interface);
	xm_stringa= XmStringCreateSimple(stringa_interface);
	XtSetArg(arg[i],XmNlabelString,
			xm_stringa);
	i++;
	}
else
	{
	xm_stringa= XmStringCreateSimple(new->port.nome_port);
	XtSetArg(arg[i],XmNlabelString,
		xm_stringa);i++;
	}
new->port.wLabel=XmCreateLabel(PadreLabel,NOME_LABEL_PORTE_INTERFACCIA,arg,i);
XmStringFree(xm_stringa);
XmFontListFree(new->port.font_list);
}



static void PosizionaLabel(Widget w)
{
XlPortWidget cw = (XlPortWidget) w;
int i;
Dimension pw,ph;
Dimension lw,lh;
Position x,y,cx,cy;
Arg arg[15];


i=0;
XtSetArg(arg[i],XmNwidth,&pw);i++;
XtSetArg(arg[i],XmNheight,&ph);i++;
XtSetArg(arg[i],XmNx,&cx);i++;
XtSetArg(arg[i],XmNy,&cy);i++;
XtGetValues(XtParent(cw),arg,i);
i=0;
XtSetArg(arg[i],XmNwidth,&lw);i++;
XtSetArg(arg[i],XmNheight,&lh);i++;
XtGetValues(cw->port.wLabel,arg,i);

/*
	calcola la posizione y
*/
y=cw->core.y+cy-lh/2+DEFAULT_ARROW_HEIGHT/2;
if(cw->port.position_tag == TAG_POSITION_BOTTOM)
	y=cw->core.y+cy+DEFAULT_ARROW_HEIGHT;
if(cw->port.position_tag == TAG_POSITION_TOP)
	y=cw->core.y+cy-lh;

if(XlIsInterfacePort((Widget)cw))
	{
	if((cw->port.tipo_port == INPUT_PORT_A)||
		(cw->port.tipo_port == INPUT_PORT_D))
			x = cw->core.x + cx - lw -1;
	else
			x = cw->core.x + cx + DEFAULT_ARROW_WIDTH+1;
	}
else
	{
	if(cw->core.x<ph/2)
		x = cw->core.x + DEFAULT_ARROW_WIDTH+1+cx;
	else
		x = cw->core.x - lw - 1+cx;
	}

i=0;
XtSetArg(arg[i],XmNx,x);i++;
XtSetArg(arg[i],XmNy,y);i++;
XtSetValues(cw->port.wLabel,arg,i);
}


/*
 XlIsPort(w) funzione per sapere se l'oggetto w
 e' del tipo XlPort
*/
Boolean XlIsPort(Widget w)
{
CoreWidgetClass wclass;
wclass=XtClass(w);
if(strcmp(wclass->core_class.class_name,"Port")==0)
        return(True);
else
        return(False);
}

Boolean XlIsInterfacePort(Widget w)
{
XlPortWidget cw = (XlPortWidget) w;

/* verifico che l'oggetto sia una porta */
if(!XlIsPort(w))
	return(False);

if(cw->port.interface_port == INTERFACE_PORT)
	return(True);
else
	return(False);
}



static void PortSelect(Widget w)
{
XlPortWidget cw = (XlPortWidget) w;
XlPortWidget wseconda; /* utilizzato nel caso di conclusione di una
 			  connessione: rappresenta il widget della seconda 
                          porta utilizzata nella connessione */
Arg arg[1];
int tipo_1,tipo_2;
Boolean ret;
GC gc_export;
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;

if(!cw->port.connectMode)
	return;

/*
	non possono essere connesse port di interfaccia con port normali
*/
/*
if(XlIsInterfacePort((Widget)w))
   if(!XlIsInterfacePort((Widget)xlPortClassRec.xlport_class.wport))
	   {
          XlWarning("XlPort","PortSelect",
		"Try to connect not homogenous port (interface with normal)");
          return;
          }
if(XlIsInterfacePort((Widget)xlPortClassRec.xlport_class.wport))
   if(!XlIsInterfacePort((Widget)w))
	   {
          XlWarning("XlPort","PortSelect",
		"Try to connect not homogenous port (interface with normal)");
	  XlPortResetConnection();
          return;
          }
*/

if(!XlIsInterfacePort((Widget)w))
  {
  if(xlPortClassRec.xlport_class.stato_porte == CONNESSIONE_PORTE_ATTIVA)
     {
     if(XtParent(XtParent(xlPortClassRec.xlport_class.wport))!=
	  XtParent(XtParent(w))) 
	  {
	  XlWarning("XlPort","PortSelect",
		  "Connection active on a different window");
	  return;
          }
      if(XlIsInterfacePort((Widget)xlPortClassRec.xlport_class.wport))
          {
          XlWarning("XlPort","PortSelect",
                "Try to connect not homogenous port (interface with normal)");
	  XlPortResetConnection();
          return;
          }

     }
  }
else
  {
  if(xlPortClassRec.xlport_class.stato_porte == CONNESSIONE_PORTE_ATTIVA)
     {
     if(XtParent(XtParent(xlPortClassRec.xlport_class.wport))==
	  XtParent(XtParent(w))) 
	  {
	  XlWarning("XlPort","PortSelect",
		  "Connection active allowed on different window");
	  XlPortResetConnection();
	  return;
	  }
      if(!XlIsInterfacePort((Widget)xlPortClassRec.xlport_class.wport))
          {
          XlWarning("XlPort","PortSelect",
                "Try to connect not homogenous port (interface with normal)");
	  XlPortResetConnection();
          return;
          }
     }
  }

/*
	verifico che le porte possano essere connesse fra loro
*/
if(xlPortClassRec.xlport_class.stato_porte == CONNESSIONE_PORTE_ATTIVA)
	{
	XtSetArg(arg[0],XlNtipoPort,&tipo_1);
	XtGetValues(xlPortClassRec.xlport_class.wport,arg,1);
	XtSetArg(arg[0],XlNtipoPort,&tipo_2);
	XtGetValues(w,arg,1);
	if(
		((tipo_1 == INPUT_PORT_A) &&(tipo_2 != OUTPUT_PORT_A)) ||
		((tipo_1 == OUTPUT_PORT_A)&&(tipo_2 != INPUT_PORT_A))  ||
		((tipo_1 == INPUT_PORT_D) &&(tipo_2 != OUTPUT_PORT_D)) ||
		((tipo_1 == OUTPUT_PORT_D)&&(tipo_2 != INPUT_PORT_D)) 
	  )
		{
		XlWarning("XlPort","PortSelect",
			"Try to connect not homogenous port");
		return;
		}
	}





if(xlPortClassRec.xlport_class.stato_porte== CONNESSIONE_PORTE_NON_ATTIVA)
	{
	/*
 	Esamina se la porta e' gia' connessa: in tal caso la connessione non
 	viene permessa in quanto e' prevista soltanto la connessione
 	1 a 1 tra le porte.
	*/
        if(cw->port.nome_port_connect!=NULL)
	  if(strlen(cw->port.nome_port_connect)!=0)
		{
		XlWarning("XlPort","PortSelect",
			"Tryed to connect busy port !!");
                if(XlIsInterfacePort((Widget)cw))
	        	XlPortResetConnection();
		return;
		}
	wseconda=NULL; /* non e' ancora nota la seconda porta */
	xlPortClassRec.xlport_class.stato_porte = CONNESSIONE_PORTE_ATTIVA;
	}
else
	{
        if(cw->port.nome_port_connect!=NULL)
	    if(strlen(cw->port.nome_port_connect)!=0)
		{
		XlWarning("XlPort","PortSelect",
			"Tryed to connect busy port !!");
                if(XlIsInterfacePort((Widget)cw))
	        	XlPortResetConnection();
		return;
		}
	xlPortClassRec.xlport_class.stato_porte = CONNESSIONE_PORTE_NON_ATTIVA;
	wseconda=(XlPortWidget)xlPortClassRec.xlport_class.wport;
	}

xlPortClassRec.xlport_class.wport = w;

/*
	richiamo la routine di disegno memorizzata nel campo
	selectPort della classe
*/
if(*xlPortClassRec.xlport_class.selectPort!=NULL)
  	if((cw->port.tipo_port == OUTPUT_PORT_A) ||
	  (cw->port.tipo_port == INPUT_PORT_A))
	{
	values.foreground = cw->port.color_analog_port;
	values.background = cw->core.background_pixel;
	values.line_width = 0;
	values.line_style = LineSolid;
	gc_export=XtGetGC(cw, valuemask, &values);
     	ret = (*xlPortClassRec.xlport_class.selectPort)(w,wseconda,
		xlPortClassRec.xlport_class.stato_porte, gc_export);
	}
else
	{
	values.foreground = cw->port.color_digital_port;
	values.background = cw->core.background_pixel;
	values.line_width = 0;
	values.line_style = LineSolid;
	gc_export=XtGetGC(cw, valuemask, &values);
     	ret = (*xlPortClassRec.xlport_class.selectPort)(w,wseconda,
		xlPortClassRec.xlport_class.stato_porte, gc_export);
	}

XtReleaseGC(cw,gc_export);


if(xlPortClassRec.xlport_class.stato_porte == CONNESSIONE_PORTE_NON_ATTIVA)
	xlPortClassRec.xlport_class.wport = NULL;

if(!ret && 
	(xlPortClassRec.xlport_class.stato_porte == CONNESSIONE_PORTE_ATTIVA))
	{
	xlPortClassRec.xlport_class.wport = NULL;
        xlPortClassRec.xlport_class.stato_porte = CONNESSIONE_PORTE_NON_ATTIVA;
	}

/*
	richiamo la routine di redisplay 
*/
Redisplay(cw,NULL);
}

/*
	resetta lo stato di connessione delle porte
*/
void XlPortResetConnection()
{
        if(xlPortClassRec.xlport_class.wport != NULL)
		XClearArea( XtDisplay(xlPortClassRec.xlport_class.wport),
			XtWindow(xlPortClassRec.xlport_class.wport),
			0,0,0,0,True);
	xlPortClassRec.xlport_class.wport = NULL;
        xlPortClassRec.xlport_class.stato_porte = CONNESSIONE_PORTE_NON_ATTIVA;
}

Boolean XlPortIsConnected(Widget w)
{
XlPortWidget cw = (XlPortWidget) w;

	/* verifico che l'oggetto sia una porta */
	if(!XlIsPort(w))
		return(False);

        if(cw->port.nome_port_connect!=NULL)
	  {
	  if(strlen(cw->port.nome_port_connect)!=0)
		return(True);
	  else
		return(False);
          }
return(False);
}

Boolean XlInterfacePortIsConnected(Widget w)
{
/* verifico che l'oggetto sia una porta */
if(!XlIsPort(w))
	return(False);
if(!XlIsInterfacePort((Widget)w))
	return(False);
if(!XlPortIsConnected(w))
	return(False);
else
	return(True);
}

static void CreaStringaInterface(Widget w, char * testo)
{
XlPortWidget new = (XlPortWidget) w;
char w1[10],p1[10],w2[10],p2[10];
char *app,*app1;
char app_testo[50];

testo[0]=0;
	
if(new->port.nome_port_connect!=NULL)
	{
	w1[0]=0;
	p1[0]=0;
	w2[0]=0;
	p2[0]=0;
	strcpy(app_testo,new->port.nome_port_connect);
	app1=app_testo;
	app=strstr(app1," ");
	if(app == NULL) return;
	*app=0;
	strcpy(w1,app1);
	app1=app+1;

	app=strstr(app1," ");
	if(app == NULL) return;
	*app=0;
	strcpy(p1,app1);
	app1=app+1;

	app=strstr(app1," ");
	if(app == NULL) return;
	*app=0;
	strcpy(w2,app1);
	app1=app+1;
	
	strcpy(p2,app1);
	
	app=strstr(w1,"w");
	if(app == NULL) return;
	*app=0;

	app=strstr(w2,"w");
	if(app == NULL) return;
	*app=0;

	if((new->port.tipo_port == INPUT_PORT_A)||
		(new->port.tipo_port == INPUT_PORT_D))
			sprintf(testo,"%s%s-%s%s",w1,p1,w2,p2);
	else
			sprintf(testo,"%s%s-%s%s",w2,p2,w1,p1);
	}
else
	strcpy(testo,"none");
}

static Boolean CheckPort(Widget w)
{
XlPortWidget cw = (XlPortWidget) w;
/*
	controllo che alla porta sia stato assegnato un nome
*/
if(cw->port.nome_port == NULL)
	{
        fprintf(stderr,"WARNING: Obj =%s has not any name associated\n",XtName(cw));
	XlEvError(cw,True);
	return(False);
	}

/*
	se la porta e' di interfaccia ritorno
*/
if(XlIsInterfacePort((Widget)cw))
	return(True);

if(strlen(cw->port.nome_port)!=2)
	{
        fprintf(stderr,"WARNING: Obj =%s with nome_port too long or too short\n",XtName(cw));
	XlEvError(cw,True);
	return(False);
	}

if(strcmp(cw->port.nome_port,"--") == 0)
	{
        fprintf(stderr,"WARNING: Obj =%s with nome_port = --\n",XtName(cw));
	XlEvError(cw,True);
	return(False);
	}

return(True);
}

Boolean XlCheckPort(Widget w)
{
XlPortWidget cw = (XlPortWidget) w;

return((*xlPortClassRec.xlport_class.checkPort)(cw));
}

Boolean XlPortIsInput(Widget w)
{
XlPortWidget cw = (XlPortWidget) w;

/* verifico che l'oggetto sia una porta */
if(!XlIsPort(w))
	return(False);

if((cw->port.tipo_port==INPUT_PORT_A)||(cw->port.tipo_port==INPUT_PORT_D))
	return(True);
else
	return(False);
}

Boolean XlPortIsOutput(Widget w)
{
XlPortWidget cw = (XlPortWidget) w;

/* verifico che l'oggetto sia una porta */
if(!XlIsPort(w))
	return(False);

if((cw->port.tipo_port==OUTPUT_PORT_A)||(cw->port.tipo_port==OUTPUT_PORT_D))
	return(True);
else
	return(False);
}

void XlPortPosizionaLabel(Widget w)
{
XlPortWidget cw = (XlPortWidget) w;

if(cw->port.wLabel!=NULL)
	PosizionaLabel(cw);
}

Widget XlPortGetConnectedPort(Widget w)
{
XlPortWidget cw = (XlPortWidget) w;
WidgetList widget_figli;
Cardinal num_figli;
int i;
Widget ret;

if(XlIsInterfacePort(w))
	return(NULL);
if(!XlPortIsConnected(w))
	return(NULL);
                                      
XtVaGetValues( XtParent(XtParent(cw)),XmNnumChildren, &num_figli,
                XmNchildren, &widget_figli, NULL );
for(i=0;i<num_figli;i++)
	{
	ret=XtNameToWidget(widget_figli[i],cw->port.nome_port_connect);
	if(ret!=NULL)
		return(ret);
	}
return(NULL);
}



/*
        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
XlPortWidget cw = (XlPortWidget) w;
Position x,y;
char  stringa[50];
char *printerFont;



if(cw->port.visibleMode == PORT_UNMANAGED)
	return(True);

(*superclass->xlcore_class.print) (w);

printerFont=XtNewString(XlPrinterGetFont(w,
	cw->port.normalfont,cw->port.font_info));

/* libero la font_struct */
/*
if(cw->port.font_info)
        XFreeFont(XtDisplay(cw),cw->port.font_info);
*/

/* rilascio i 4 GC */
if(cw->port.digital_gc)
        XtReleaseGC(cw,cw->port.digital_gc);
if(cw->port.analog_gc)
        XtReleaseGC(cw,cw->port.analog_gc);
if(cw->port.connected_gc)
        XtReleaseGC(cw,cw->port.connected_gc);
if(cw->port.start_connection_gc)
        XtReleaseGC(cw,cw->port.start_connection_gc);

if(!LoadFont(cw,printerFont,True))
        LoadFont(cw,"fixed",True);
XtFree(printerFont);
GetAllGCs(cw,True);

DrawDirectly(cw);
/*
	esegue l'hardcopy della label
*/
if((cw->port.visibleMode == PORT_TAG)&&(cw->port.nome_port!=NULL))
	{
	GC gc_print;
	XGCValues values;
	unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFont;

	values.line_width = 0;
	values.line_style = LineSolid;
	values.font = cw->port.font_info->fid;
	values.foreground = XlPrintGetPixel(w,cw->port.color_foreground_label);
	values.background = XlPrintGetPixel(w,cw->port.color_background_label);
	gc_print = XCreateGC(XlDisplay(w), XlWindow(w), valuemask, &values);

        if(XlIsInterfacePort((Widget)cw))
		CreaStringaInterface(cw, stringa);
	else
		sprintf(stringa,cw->port.nome_port);
	XtVaGetValues(cw->port.wLabel,XmNx,&x,XmNy,&y,NULL);
	XDrawImageString( XlDisplay(cw), 
		XlWindow(cw),gc_print,
			x,y+cw->port.font_info->ascent+2,
			stringa,strlen(stringa));
        XFreeGC(XlDisplay(w),gc_print);
	}


/* rilascio i 4 GC della stampante */
if(cw->port.digital_gc)
        XFreeGC(XlDisplay(w),cw->port.digital_gc);
if(cw->port.analog_gc)
        XFreeGC(XlDisplay(w),cw->port.analog_gc);
if(cw->port.connected_gc)
        XFreeGC(XlDisplay(w),cw->port.connected_gc);
if(cw->port.start_connection_gc)
        XFreeGC(XlDisplay(w),cw->port.start_connection_gc);

/* rilascio il font della stampante */
if(cw->port.font_info)
        XFreeFont(XlDisplay(cw),cw->port.font_info);

if(!LoadFont(cw,cw->port.normalfont,False))
        LoadFont(cw,"fixed",False);

if(cw->port.wLabel!=NULL)
	XtVaSetValues(cw->port.wLabel,XmNfontList,cw->port.font_list,NULL);

GetAllGCs(cw,False);



return(True);
}
#endif
