/*
	grilievo.c - cornici in rilievo al posto dei bordi neri di X

	Stazioni, display, indicatori e tasti si incorniciano con il bordo
	della finestra X (XmNborderWidth), che di suo e' a tinta unita: nero.
	Qui il bordo RESTA, ma invece del colore riceve una pixmap
	(XtNborderPixmap) con dentro la cornice in rilievo - sporgente per la
	stazione, incassata per display e strumenti.

	Perche' il bordo e non un disegno del padre: il bordo fa parte della
	finestra, quindi lo disegna il server, sta sopra i fratelli (un'etichetta
	che lo tocca non lo copre) e la geometria non cambia di un pixel - ne'
	quella del widget ne' quella del padre, che si dimensiona sui figli. Una
	prima versione toglieva il bordo e faceva disegnare la cornice al padre:
	il BulletinBoard della pagina si restringeva di quanto era il bordo e le
	stazioni sul perimetro perdevano i lati esterni.

	La pixmap del bordo si ripete come una piastrella con l'origine
	nell'angolo INTERNO della finestra, quindi il pixel del bordo in (-b,-b)
	cade in (W-b,H-b) della piastrella. La cornice si disegna normalmente in
	una pixmap grande quanto la finestra con il bordo, poi si ruota di b
	pixel in orizzontale e in verticale (Ruota).
*/
#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>
#include "Rilievo.h"

typedef struct {
	int verso;      /* RILIEVO_SPORGE o RILIEVO_INCAVATO */
	Pixel sfondo;   /* da cui si ricavano luce e ombra */
	Pixmap pix;     /* la piastrella del bordo in uso */
	int larg,alt;   /* la misura per cui e' stata fatta */
	} CORNICE;

/* copia la cornice disegnata (W x H) nella piastrella, ruotata di b */
static void Ruota(dpy,da,a,gc,W,H,b)
Display *dpy;
Pixmap da,a;
GC gc;
int W,H,b;
{
XCopyArea(dpy,da,a,gc,b,b,W-b,H-b,0,0);
XCopyArea(dpy,da,a,gc,0,b,b,H-b,W-b,0);
XCopyArea(dpy,da,a,gc,b,0,W-b,b,0,H-b);
XCopyArea(dpy,da,a,gc,0,0,b,b,W-b,H-b);
}

static void fai_bordo(w,c)
Widget w;
CORNICE *c;
{
Display *dpy=XtDisplay(w);
Window radice=RootWindowOfScreen(XtScreen(w));
int b,W,H;
Pixmap disegno,piastrella;
GC gc;
Arg a[1];

b=w->core.border_width;
if(b<=0 || w->core.width==0 || w->core.height==0)
	return;
W=w->core.width+2*b;
H=w->core.height+2*b;
disegno=XCreatePixmap(dpy,radice,W,H,w->core.depth);
piastrella=XCreatePixmap(dpy,radice,W,H,w->core.depth);
gc=XCreateGC(dpy,disegno,0,NULL);
XSetForeground(dpy,gc,c->sfondo);
XFillRectangle(dpy,disegno,gc,0,0,W,H);
RilievoCornice(w,disegno,gc,0,0,W,H,b,c->sfondo,c->verso);
Ruota(dpy,disegno,piastrella,gc,W,H,b);
XFreeGC(dpy,gc);
XFreePixmap(dpy,disegno);

XtSetArg(a[0],XtNborderPixmap,piastrella);
XtSetValues(w,a,1);
/* la vecchia si libera solo ora: fino a qui era quella del widget */
if(c->pix!=None)
	XFreePixmap(dpy,c->pix);
c->pix=piastrella;
c->larg=w->core.width;
c->alt=w->core.height;
}

/*
 Se il widget cambia misura (prima di comparire o dopo), la piastrella
 fatta per la misura vecchia si ripeterebbe sfasata: si rifa'.
*/
static void controlla_misura(w,client_data,event,continua)
Widget w;
XtPointer client_data;
XEvent *event;
Boolean *continua;
{
CORNICE *c=(CORNICE *)client_data;
if(event->type!=ConfigureNotify && event->type!=MapNotify)
	return;
if(w->core.width!=c->larg || w->core.height!=c->alt)
	fai_bordo(w,c);
}

static void togli_cornice(w,client_data,call_data)
Widget w;
XtPointer client_data,call_data;
{
CORNICE *c=(CORNICE *)client_data;
XtRemoveEventHandler(w,StructureNotifyMask,False,controlla_misura,client_data);
if(c->pix!=None)
	XFreePixmap(XtDisplay(w),c->pix);
XtFree((char *)c);
}

/*
 Disegna in rilievo il bordo X di w, dello spessore che ha gia'.
 Da chiamare subito dopo la creazione; se w non ha bordo non fa niente.
*/
void cornice_rilievo(w,verso,sfondo)
Widget w;
int verso;
Pixel sfondo;
{
CORNICE *c;
if(w->core.border_width<=0)
	return;
c=(CORNICE *)XtMalloc(sizeof(CORNICE));
c->verso=verso;
c->sfondo=sfondo;
c->pix=None;
c->larg=c->alt=0;
fai_bordo(w,c);
XtAddEventHandler(w,StructureNotifyMask,False,controlla_misura,(XtPointer)c);
XtAddCallback(w,XmNdestroyCallback,togli_cornice,(XtPointer)c);
}
