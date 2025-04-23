/**********************************************************************
*
*       C Source:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: %  (%full_filespec: %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)Led.c	5.1\t11/7/95";
/*
   modulo Led.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)Led.c	5.1
*/
/*
 *  Led.c - widget led per stazioni
 */
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <aggiunte_Xt.h>

#include <stdio.h>

#include "LedP.h"

#define DEFAULTWIDTH 11
#define DEFAULTHEIGHT 11

/* lista delle risorse  */
static XtResource resources[]= {
        {
        XtNledColorBlink,
        XtCLedColorBlink,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(LedRec,led.color_blink),
        XmRString,
        XtDefaultForeground
        },
        {
        XtNledBlinkOn,
        XtCLedBlinkOn,
        XmRInt,
        sizeof(int),
        XtOffsetOf(LedRec,led.blink_on),
        XmRImmediate,
        (XtPointer)0
        },
        };
/* dichiarazioni funzioni varie */
static void blink_proc();
         
/* dichiarazione dei metodi (methods) */

static void Initialize();
static void Destroy();
static Boolean SetValues();

/* Inizializzazione del class record */
LedClassRec ledClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &widgetClassRec,
    /* class_name               */      "Led",
    /* widget_size              */      sizeof(LedRec),
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
    /* expose                   */      XtInheritExpose,
    /* set_values               */      SetValues,
    /* set_values_hook          */      NULL,
    /* set_values_almost        */      XtInheritSetValuesAlmost,
    /* get_values_hook          */      NULL,
    /* accept_focus             */      NULL,
    /* version                  */      XtVersion,
    /* callback_private         */      NULL,
    /* tm_table                 */      NULL,
    /* query_geometry           */      XtInheritQueryGeometry,
    /* display_accelerator      */      XtInheritDisplayAccelerator,
    /* extension                */      NULL
  },
  { /* led fields */
    /* empty                    */      0
  }
};

WidgetClass ledWidgetClass = (WidgetClass) &ledClassRec;


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
LedWidget new = (LedWidget)tnew;
new->led.color_norm=new->core.background_pixel;
new->led.alterna=0;
if(new->led.blink_on)
	new->led.time_id=XtAddTimeOut(400,blink_proc,new);
}

static void blink_proc(client_data,id)
XtPointer client_data;
XtIntervalId *id;
{
int i;
Arg args[3];
LedWidget cw=(LedWidget)client_data;
if(cw->led.alterna)
	{
	i=0;
	XtSetArg(args[i],XmNbackground,cw->led.color_blink); i++;
	XtSetValues((Widget)cw,args,i);
	}
else
	{
	i=0;
        XtSetArg(args[i],XmNbackground,cw->led.color_norm); i++;
        XtSetValues((Widget)cw,args,i);
	}
cw->led.alterna=(!(cw->led.alterna));
if(cw->led.blink_on)
        cw->led.time_id=XtAddTimeOut(400,blink_proc,cw);
else /* ridisegna il led in stato normale e non ricarica il
        timer */
        {
	i=0;
        XtSetArg(args[i],XmNbackground,cw->led.color_norm); i++;
        XtSetValues((Widget)cw,args,i);
        }
}



static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
LedWidget curcw= (LedWidget) current;
LedWidget newcw= (LedWidget) new;
Boolean do_redisplay = False;
/*
if(curcw->core.background_pixel != newcw->core.background_pixel)
	{
	newcw->led.color_norm=newcw->core.background_pixel;
	do_redisplay = True;
	}
*/
if( newcw->led.blink_on == 1 && curcw->led.blink_on == 0)
	{
	newcw->led.time_id=XtAddTimeOut(400,blink_proc,new);
	do_redisplay = False;
	}
return do_redisplay;
}

static void Destroy(w)
Widget w;
{
LedWidget cw= (LedWidget) w;
if (cw->led.blink_on==1 && cw->led.time_id)
	XtRemoveTimeOut(cw->led.time_id);
}

