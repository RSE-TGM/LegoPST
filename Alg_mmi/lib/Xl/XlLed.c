/**********************************************************************
*
*       C Source:               XlLed.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:21:50 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlLed.c-3.2.3 %  (%full_filespec: XlLed.c-3.2.3:csrc:1 %)";
#endif
/*
   modulo XlLed.c
   tipo 
   release 5.2
   data 12/4/95
   reserved @(#)XlLed.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlLed.c	5.2\t12/4/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlLed.c - widget led per stazioni
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlLedP.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

#define DEFAULTWIDTH 11
#define DEFAULTHEIGHT 11

/* lista delle risorse  */
static XtResource resources[]= {
        {
        XlNcolorNorm,
        XlCColorNorm,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlLedRec,led.color_norm),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNcolorBlink,
        XlCColorBlink,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlLedRec,led.color_blink),
        XmRString,
        XtDefaultForeground
        },
        {
        XlNvarInputColore,
        XlCVarInputColore,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlLedRec,led.str_input_colore),
        XmRString,
        ""
        },
        {
        XlNvarInputBlink,
        XlCVarInputBlink,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlLedRec,led.str_input_blink),
        XmRString,
        ""
        },
        {
        XlNvarInputBlink2,
        XlCVarInputBlink2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlLedRec,led.str_input_blink2),
        XmRString,
        ""
        },
        {
        XlNfrequenza1,
        XlCFrequenza1,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlLedRec,led.frequenza1),
        XtRString,
        FREQUENZA1
        },
        {
        XlNfrequenza2,
        XlCFrequenza2,
        XtRFloat,
        sizeof(float),
        XtOffsetOf(XlLedRec,led.frequenza2),
        XtRString,
        FREQUENZA2
        },
        {
        XlNtipoLed,
        XlCTipoLed,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlLedRec,led.tipo_led),
        XmRImmediate,
        (XtPointer)LED_DEFAULT
        }
        };

/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
        {
        XlNcolorNorm,
        XlCColorNorm,
        XlDcolorNorm,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlLedRec,led.color_norm),
        sizeof(Pixel)
        },
        {
        XlNcolorBlink,
        XlCColorBlink,
        XlDcolorBlink,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlLedRec,led.color_blink),
        sizeof(Pixel)
        },
        {
        XlNvarInputColore,
        XlCVarInputColore,
        XlDvarInputColore,
	XlRTipoVarInp,
        XlRVarDX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlLedRec,led.input_colore),
        sizeof(PUNT_VARINP)
        },
        {
        XlNvarInputBlink,
        XlCVarInputBlink,
        XlDvarInputBlink,
	XlRTipoVarInp,
        XlRVarDX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlLedRec,led.input_blink),
        sizeof(PUNT_VARINP)
        },
        {
        XlNvarInputBlink2,
        XlCVarInputBlink2,
        XlDvarInputBlink2,
	XlRTipoVarInp,
        XlRVarDX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlLedRec,led.input_blink2),
        sizeof(PUNT_VARINP)
        },
        {
        XlNfrequenza1,
        XlCFrequenza1,
        XlDfrequenza1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlLedRec,led.frequenza1),
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
        XtOffsetOf(XlLedRec,led.frequenza2),
        sizeof(float)
        },
        {
        XlNtipoLed,
        XlCTipoLed,
        XlDtipoLed,
        XlROption,
        NULL,
        XlOtipoLed,
        XlRGrIo,
        XtOffsetOf(XlLedRec,led.tipo_led),
        sizeof(int)
        }
	};
/* dichiarazioni funzioni varie */
         
/* dichiarazione dei metodi (methods) */
static void Seleziona();
static void AddSelez();
static void Initialize();
static void Destroy();
static void Redisplay();
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
XlLedClassRec xlLedClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "Led",
    /* widget_size              */      sizeof(XlLedRec),
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
  { /* led fields */
    /* empty                    */      0
  }
};

WidgetClass xlLedWidgetClass = (WidgetClass) &xlLedClassRec;

/*
        calcola il valore di alterna, per il blink, in funzione
        della variabilr input_colore e input_blink
*/


static int tabella_blink(float input_colore, float input_blink,float input_blink2)
{
 	if(input_colore==1)/*Input colore==1->acceso*/
                return(1);
        if(input_blink==1)
                return(BLINK_ON_FQZ1);/*blink a freq1*/
  	if(input_blink2==1)
  		return(BLINK_ON_FQZ2); /*blink a freq2*/
  	return(0);	
}

static void GetNormFgGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlLedWidget cw= (XlLedWidget) w;
values.line_width = 0;
values.line_style = LineSolid;
#if defined XPRINTER_USED
if(modoPrint)
        {
        values.foreground = XlPrintGetPixel(w,cw->led.color_norm);
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->led.norm_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
        }
else
        {
        values.foreground = cw->led.color_norm;
        values.background = cw->core.background_pixel;
        cw->led.norm_gc = XtGetGC(cw, valuemask, &values);
        }
#else
values.foreground = cw->led.color_norm;
values.background = cw->core.background_pixel;
cw->led.norm_gc = XtGetGC(cw, valuemask, &values);
#endif

}

static void GetBlinkGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlLedWidget cw= (XlLedWidget) w;
values.line_width = 0;
values.line_style = LineSolid;
#if defined XPRINTER_USED
if(modoPrint)
        {
        values.foreground = XlPrintGetPixel(w,cw->led.color_blink);
        values.background = XlPrintGetPixel(w,cw->core.background_pixel);
        cw->led.blink_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
        }
else
        {
        values.foreground = cw->led.color_blink;
        values.background = cw->core.background_pixel;
        cw->led.blink_gc = XtGetGC(cw, valuemask, &values);
        }
#else
values.foreground = cw->led.color_blink;
values.background = cw->core.background_pixel;
cw->led.blink_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetClearGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlLedWidget cw= (XlLedWidget) w;
values.line_width = 0;
values.line_style = LineSolid;
#if defined XPRINTER_USED
if(modoPrint)
        {
        values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
        values.background = XlPrintGetPixel(w,cw->led.color_norm);
        cw->led.clear_gc = XCreateGC(XlDisplay(cw),XlWindow(cw),
                                 valuemask, &values);
        }
else
        {
        values.foreground = cw->core.background_pixel;
        values.background = cw->led.color_norm;
        cw->led.clear_gc = XtGetGC(cw, valuemask, &values);
        }
#else
values.foreground = cw->core.background_pixel;
values.background = cw->led.color_norm;
cw->led.clear_gc = XtGetGC(cw, valuemask, &values);
#endif
}

static void GetAllGCs(w,modoPrint)
Widget w;
Boolean modoPrint;
{
GetNormFgGC(w,modoPrint);
GetClearGC(w,modoPrint);
GetBlinkGC(w,modoPrint);
}

static void DrawDirectly(w,On)
Widget w;
Boolean On; /*Se On=True accendi Led (blink_gc)*/
{
XlLedWidget cw= (XlLedWidget)w;
Window cwin;
int width,height;
width=cw->core.width-1;
height=cw->core.height-1;

cwin=XlWindow(cw);
/*
Se On =False disegna il Led col blink_gc else col norm_gc
*/
if(!On)
	{
	if(cw->led.tipo_led==LED_RETTANGOLARE)
		{
		XlRiempiRettangolo(cw,cwin,cw->led.norm_gc,0,0,100,100);
		}	
	else
		XlRiempiArco(cw,cwin,cw->led.norm_gc,1,1,98,98,0,360*64);
	}
else
	{
	if(cw->led.tipo_led==LED_RETTANGOLARE)
		{
		XlRiempiRettangolo(cw,cwin,cw->led.blink_gc,0,0,100,100);
		}
	else
		XlRiempiArco(cw,cwin,cw->led.blink_gc,
				   1,1,98,98,0,360*64);
	}		

}


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{

XlLedWidget new = (XlLedWidget)tnew;
new->led.alterna=0;
GetAllGCs(new,False);



if((new->led.frequenza1)<=0)
	{
	new->led.frequenza1=atof(FREQUENZA1);
	XlWarning("XlLed","Initialize","frequenza1 deve essere >0");
	}
if((new->led.frequenza2<=0))
	{
	new->led.frequenza2=atof(FREQUENZA2);
	XlWarning("XlLed","Initialize","frequenza2 deve essere >0");
	}	
if((new->led.frequenza1)>4)
	{
	new->led.frequenza1=atof(FREQUENZA1);
	XlWarning("XlLed","Initialize","frequenza1 deve essere <4");
	}
if((new->led.frequenza2>4))
	{
	new->led.frequenza2=atof(FREQUENZA2);
	XlWarning("XlLed","Initialize","frequenza2 deve essere <4");
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



static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
XlLedWidget curcw= (XlLedWidget) current;
XlLedWidget newcw= (XlLedWidget) new;
Boolean do_redisplay = False;

if(curcw->led.color_norm != newcw->led.color_norm)
        {
        XtReleaseGC(curcw,curcw->led.norm_gc);
        XtReleaseGC(curcw,curcw->led.clear_gc);
        GetNormFgGC(newcw);
        GetClearGC(newcw);
        do_redisplay = True;
        }

if(curcw->led.color_blink != newcw->led.color_blink)
        {
        XtReleaseGC(curcw,curcw->led.norm_gc);
        GetBlinkGC(newcw);
        do_redisplay = True;
        }
if((curcw->led.frequenza1) !=  newcw->led.frequenza1)
	{
	Compile(newcw);
	do_redisplay = False;
	}
if((curcw->led.frequenza2) !=  newcw->led.frequenza2)
	{
	Compile(newcw);
	do_redisplay = False;
	}	
if(curcw->led.tipo_led !=  newcw->led.tipo_led)
	{
	do_redisplay = True;
	}
/*
 risorse connesse con sistema di simulazione
*/
if(strcmp(newcw->led.str_input_colore,curcw->led.str_input_colore)||
   strcmp(newcw->led.str_input_blink,curcw->led.str_input_blink)
   || strcmp(newcw->led.str_input_blink2,curcw->led.str_input_blink2))
        {
        Compile(newcw);
        do_redisplay = False;
        }


return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlLedWidget cw= (XlLedWidget) w;

/*
Elimina il widget nella lista_blink
*/

XlRemoveBlink(cw,0);
/*
        Rilascio i GC
*/
if (cw->led.norm_gc)
        XtReleaseGC(cw,cw->led.norm_gc);
if (cw->led.blink_gc)
        XtReleaseGC(cw,cw->led.blink_gc);
if (cw->led.clear_gc)
        XtReleaseGC(cw,cw->led.clear_gc);

}


static Boolean Compile(Widget w)
{
Boolean errors=False;
XlLedWidget cw= (XlLedWidget)w;

if(!XtIsWidget(w))
        {
        XlWarning("XlLed","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }
/*
 Devono esistere le due variabili di input
*/
if(strlen(cw->led.str_input_colore)==0)
	{
	XlWarning("XlLed","Compile","First input not defined");
	}
if(strlen(cw->led.str_input_blink)==0)
        {
	XlWarning("XlLed","Compile","Second input not defined");
	}
if(strlen(cw->led.str_input_blink2)==0)
        {
	XlWarning("XlLed","Compile","Third input not defined");
	}

if(cw->led.frequenza1<=0)
	{
	cw->led.frequenza1=atof(FREQUENZA1);
	XlErrComp(w,"Compile","frequenza1 must be positive",NULL);
	errors=True;
	}
if(cw->led.frequenza2<=0)
	{
	cw->led.frequenza2=atof(FREQUENZA2);
	XlErrComp(w,"Compile","frequenza2 must be positive",NULL);
	errors=True;
	}
if(cw->led.frequenza1>4)
	{
	cw->led.frequenza1=atof(FREQUENZA1);
	XlErrComp(w,"Compile","frequenza1 must be <4",NULL);
	errors=True;
	}
if(cw->led.frequenza2>4)
	{
	cw->led.frequenza2=atof(FREQUENZA2);
	XlErrComp(w,"Compile","frequenza2 must be <4",NULL);
	errors=True;
	}		
if(xlLedClassRec.xlcore_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
        return(False);
        }

if(!(*superclass->xlcore_class.compile) (w,
     xlLedClassRec.xlcore_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
        return(False);
        }
XlEvError(cw,errors);
return(True);
}

static void BlinkLedProc(Widget w,int ind,Boolean On)
{
/*
Funzione chiamata da XlBlink ogni BLINK_RATE ms. che ridisegna il Led
nello stato indicato da On
*/

DrawDirectly(w,On);

}

static Boolean Refresh(Widget w)
{
XlLedWidget cw= (XlLedWidget)w;
int core_width=cw->core.width;
int core_height=cw->core.height;
float already_inserted;

/*printf("Sono arrivato nella Refresh\n");*/
if(cw->xlcore.config==True)
        return(True);

if(!XtIsWidget(w))
        {
        XlWarning("XlLed","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlLedClassRec.xlcore_class.database==NULL)
        {
        XlWarning("XlLed","Refresh","database non definito");
        return(False);
        }
if(!(*superclass->xlcore_class.refresh) (w))
        {
        XlWarning("XlLed","Refresh","errore refresh classe superiore");
        return(False);
        }

/*
	se non e'  definita la variabile di blink setta a 0 il valore
*/
if(cw->led.input_blink.pdb.indice == -1)
	cw->led.input_blink.valore = 0;
if(cw->led.input_blink2.pdb.indice == -1)
	cw->led.input_blink2.valore = 0;	
/*
Inizio modifica di Fabio
*/	

already_inserted=XlGetBlink(cw,0); /*cw già inserito in lista_blink ?*/

/*
cw->led.alterna=risultato tabella_blink
*/
cw->led.alterna=tabella_blink(  cw->led.input_colore.valore,
                                cw->led.input_blink.valore,
                                cw->led.input_blink2.valore
                                );
 
        switch(cw->led.alterna)
        	{
/*
Se il risultato di tabella_blink è BLINK_ON :se oggetto già presente nella
lista degli oggetti che blinkano esci, altrimenti inseriscilo nella lista
*/                       
        	
	
		case BLINK_ON_FQZ1:
		if (already_inserted==(cw->led.frequenza1))
			{
			break;
			}
		if(already_inserted==0)/*blink led not found */	
			{
			XlAddBlink(cw,0,cw->led.frequenza1,(void*)BlinkLedProc);
			break;
			}
		XlRemoveBlink(cw,0);/*blink freq!=freq1*/
		XlAddBlink(cw,0,cw->led.frequenza1,(void*)BlinkLedProc);
		break;
		
		case BLINK_ON_FQZ2:
		if (already_inserted==(cw->led.frequenza2))
			break;
		if(already_inserted==0)/*blink led not found */	
			{
			XlAddBlink(cw,0,cw->led.frequenza2,(void*)BlinkLedProc);
			break;
			}			
		XlRemoveBlink(cw,0);
		XlAddBlink(cw,0,cw->led.frequenza2,(void*)BlinkLedProc);
		break;		
		
		case 1:
		if(already_inserted!=0)
			XlRemoveBlink(cw,0);
		DrawDirectly(cw,1);	/*Disegna in blink_gc*/
		break;
		
		case 0:
		if(already_inserted!=0)
			XlRemoveBlink(cw,0);
		DrawDirectly(cw,0);	/*Disegna in blink_nc*/
		break;
		}	
return(True);
}



static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlLedWidget cw= (XlLedWidget)w;
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

(*superclass->core_class.expose) (w,event,NULL);
DrawDirectly(w,0); /*modifica di Fabio*/

/*
        Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
*/
(*superclass->xlcore_class.drawBorderIfSelected) (w);

}



/*
        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
XlLedWidget cw= (XlLedWidget) w;
char *printerFont;

(*superclass->xlcore_class.print) (w);

/*
        Rilascio i GC
*/
if (cw->led.norm_gc)
        XtReleaseGC(cw,cw->led.norm_gc);
if (cw->led.blink_gc)
        XtReleaseGC(cw,cw->led.blink_gc);
if (cw->led.clear_gc)
        XtReleaseGC(cw,cw->led.clear_gc);

GetAllGCs(cw,True);
DrawDirectly(cw,0); /*modifica di Fabio*/	

/* rilascio i 3 GC della stampante */

if (cw->led.norm_gc)
        XFreeGC(XlDisplay(cw),cw->led.norm_gc);
if (cw->led.blink_gc)
        XFreeGC(XlDisplay(cw),cw->led.blink_gc);
if (cw->led.clear_gc)
        XFreeGC(XlDisplay(cw),cw->led.clear_gc);

GetAllGCs(cw,False);

return(True);
}
#endif
