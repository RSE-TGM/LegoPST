/*
	grilievo.c - cornici in rilievo al posto dei bordi neri di X

	Stazioni, display, indicatori e tasti si incorniciavano con il bordo
	della finestra X (XmNborderWidth), che e' solo nero e piatto. Qui il
	bordo si toglie e lo stesso spazio lo ridisegna il padre, in rilievo
	(la stazione sporge dal pannello) o incassato (display e strumenti
	sono finestre nel pannello).

	L'ingombro non cambia di un pixel: in X la posizione di una finestra
	e' quella dell'angolo esterno del bordo, quindi tolto il bordo la
	finestra si sposta in dentro di quanto era spesso e la cornice occupa
	esattamente il posto del bordo. Conta per il contenuto (resta dov'era)
	e per gli sprite di lgmkstaz, ritagliati dalle catture per geometria.
*/
#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>
#include <Xm/Xm.h>
#include "Rilievo.h"

typedef struct {
	Widget w;      /* il widget incorniciato */
	int spessore;  /* il bordo X che aveva */
	int verso;     /* RILIEVO_SPORGE o RILIEVO_INCAVATO */
	Pixel sfondo;  /* da cui si ricavano luce e ombra */
	} CORNICE;

static GC gc_cornice=NULL;

static void disegna_cornice(padre,client_data,event,continua)
Widget padre;
XtPointer client_data;
XEvent *event;
Boolean *continua;
{
CORNICE *c=(CORNICE *)client_data;
Widget w=c->w;
if(event->type!=Expose || !XtIsRealized(padre) || !XtIsManaged(w))
	return;
if(gc_cornice==NULL)
	gc_cornice=XCreateGC(XtDisplay(padre),XtWindow(padre),0,NULL);
RilievoCornice(padre,XtWindow(padre),gc_cornice,
	w->core.x-c->spessore,w->core.y-c->spessore,
	w->core.width+2*c->spessore,w->core.height+2*c->spessore,
	c->spessore,c->sfondo,c->verso);
}

static void togli_cornice(w,client_data,call_data)
Widget w;
XtPointer client_data,call_data;
{
CORNICE *c=(CORNICE *)client_data;
XtRemoveEventHandler(XtParent(w),ExposureMask,False,disegna_cornice,
	client_data);
XtFree((char *)c);
}

/*
 Sostituisce il bordo X di w con una cornice disegnata dal padre.
 Da chiamare subito dopo la creazione; se w non ha bordo non fa niente.
*/
void cornice_rilievo(w,verso,sfondo)
Widget w;
int verso;
Pixel sfondo;
{
CORNICE *c;
int b;
Arg a[3];
b=w->core.border_width;
if(b<=0)
	return;
c=(CORNICE *)XtMalloc(sizeof(CORNICE));
c->w=w;
c->spessore=b;
c->verso=verso;
c->sfondo=sfondo;
XtSetArg(a[0],XmNborderWidth,0);
XtSetArg(a[1],XmNx,w->core.x+b);
XtSetArg(a[2],XmNy,w->core.y+b);
XtSetValues(w,a,3);
XtAddEventHandler(XtParent(w),ExposureMask,False,disegna_cornice,
	(XtPointer)c);
XtAddCallback(w,XmNdestroyCallback,togli_cornice,(XtPointer)c);
}
