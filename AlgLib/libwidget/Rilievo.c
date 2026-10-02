/*
 *  Rilievo.c - primitive di disegno "in rilievo" per le stazioni di xstaz
 *
 *  Le usano i widget di questa libreria (Bottone, Led, Selettore) e xstaz
 *  per le cornici di stazioni, display e indicatori. Tutto Xlib puro:
 *  le sfumature sono finte, fatte di cerchi o righe di colore crescente,
 *  e vanno disegnate una volta (in una pixmap o nella expose), non a ogni
 *  refresh.
 *
 *  Con visual TrueColor i Pixel si compongono e scompongono dalle maschere
 *  del visual, senza round trip al server; con una colormap (8 bit) si
 *  passa da XQueryColor/XAllocColor.
 */
#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>
#include "Rilievo.h"

static int ShiftMaschera(unsigned long m)
{
int s=0;
if(m==0) return(0);
while(!(m&1))
	{
	m>>=1;
	s++;
	}
return(s);
}

void RilievoPixelRgb(Widget w, Pixel p, RilievoRgb *c)
{
Visual *v=DefaultVisualOfScreen(XtScreen(w));
XColor xc;
unsigned long m[3];
int *comp[3];
int i,s;
if(v->class==TrueColor)
	{
	m[0]=v->red_mask; m[1]=v->green_mask; m[2]=v->blue_mask;
	comp[0]= &c->r; comp[1]= &c->g; comp[2]= &c->b;
	for(i=0;i<3;i++)
		{
		s=ShiftMaschera(m[i]);
		*comp[i]=(m[i]>>s) ? (int)(((p&m[i])>>s)*255/(m[i]>>s)) : 0;
		}
	return;
	}
xc.pixel=p;
XQueryColor(XtDisplay(w),w->core.colormap,&xc);
c->r=xc.red>>8;
c->g=xc.green>>8;
c->b=xc.blue>>8;
}

Pixel RilievoRgbPixel(Widget w, RilievoRgb *c)
{
Visual *v=DefaultVisualOfScreen(XtScreen(w));
XColor xc;
unsigned long m[3],p;
int val[3];
int i,s;
if(v->class==TrueColor)
	{
	m[0]=v->red_mask; m[1]=v->green_mask; m[2]=v->blue_mask;
	val[0]=c->r; val[1]=c->g; val[2]=c->b;
	p=0;
	for(i=0;i<3;i++)
		{
		s=ShiftMaschera(m[i]);
		p|= (((unsigned long)val[i]*(m[i]>>s)/255)<<s) & m[i];
		}
	return(p);
	}
/* visual a colormap: si alloca, e se non c'e' posto bianco o nero */
xc.red=c->r*257;
xc.green=c->g*257;
xc.blue=c->b*257;
xc.flags=DoRed|DoGreen|DoBlue;
if(XAllocColor(XtDisplay(w),w->core.colormap,&xc))
	return(xc.pixel);
if(c->r+c->g+c->b > 3*128)
	return(WhitePixelOfScreen(XtScreen(w)));
return(BlackPixelOfScreen(XtScreen(w)));
}

/* a + (b-a)*t/256 */
RilievoRgb RilievoMescola(RilievoRgb a, RilievoRgb b, int t)
{
RilievoRgb c;
c.r=a.r+(b.r-a.r)*t/256;
c.g=a.g+(b.g-a.g)*t/256;
c.b=a.b+(b.b-a.b)*t/256;
return(c);
}

RilievoRgb RilievoSchiarisci(RilievoRgb a, int t)
{
static RilievoRgb bianco={255,255,255};
return(RilievoMescola(a,bianco,t));
}

RilievoRgb RilievoScurisci(RilievoRgb a, int t)
{
static RilievoRgb nero={0,0,0};
return(RilievoMescola(a,nero,t));
}

/* tre punti di colore: t=0 primo, t=128 secondo, t=256 terzo */
static RilievoRgb TreStop(RilievoRgb a, RilievoRgb b, RilievoRgb c, int t)
{
if(t<128)
	return(RilievoMescola(a,b,t*2));
return(RilievoMescola(b,c,(t-128)*2));
}

static void Colore(Widget w, GC gc, RilievoRgb c)
{
XSetForeground(XtDisplay(w),gc,RilievoRgbPixel(w,&c));
}

/*
 Cerchi concentrici sempre piu' piccoli, con il centro che si sposta
 verso la luce: in alto a sinistra se sporge, in basso a destra se
 incavato. Lo spostamento resta sotto (d-dd)/2, quindi nessun cerchio
 esce dal bordo.
*/
void RilievoSfumaCerchio(Widget w, Drawable dr, GC gc, int x, int y, int d,
		RilievoRgb bordo, RilievoRgb medio, RilievoRgb centro, int verso)
{
int n,i,dd,off;
n=d/2;
if(n<1) n=1;
for(i=0;i<n;i++)
	{
	dd=d-(i*d)/n;
	off=((d-dd)*3)/10;
	Colore(w,gc,TreStop(bordo,medio,centro,(i*256)/n));
	XFillArc(XtDisplay(w),dr,gc,
		x+(d-dd)/2-verso*off,y+(d-dd)/2-verso*off,
		dd,dd,0,360*64);
	}
}

void RilievoContornoCerchio(Widget w, Drawable dr, GC gc, int x, int y, int d,
		RilievoRgb c)
{
Colore(w,gc,c);
XDrawArc(XtDisplay(w),dr,gc,x,y,d,d,0,360*64);
}

void RilievoRiflesso(Widget w, Drawable dr, GC gc, int x, int y, int d,
		RilievoRgb base, int forza)
{
int dim,pos;
dim=d/4;
if(dim<2) dim=2;
pos=d/5;
Colore(w,gc,RilievoSchiarisci(base,forza));
XFillArc(XtDisplay(w),dr,gc,x+pos,y+pos,dim,dim,0,360*64);
}

void RilievoSfumaRett(Widget w, Drawable dr, GC gc, int x, int y,
		int larg, int alt, RilievoRgb alto, RilievoRgb medio, RilievoRgb basso)
{
int j;
if(larg<=0 || alt<=0) return;
for(j=0;j<alt;j++)
	{
	Colore(w,gc,TreStop(alto,medio,basso,alt>1 ? (j*256)/(alt-1) : 128));
	XFillRectangle(XtDisplay(w),dr,gc,x,y+j,larg,1);
	}
}

/* una riga di cornice: lato alto e sinistro in un colore, basso e destro nell'altro */
static void Anello(Widget w, Drawable dr, GC gc, int x, int y, int larg, int alt,
		RilievoRgb alto_sx, RilievoRgb basso_dx)
{
Display *dpy=XtDisplay(w);
if(larg<=0 || alt<=0) return;
Colore(w,gc,basso_dx);
XFillRectangle(dpy,dr,gc,x,y+alt-1,larg,1);
XFillRectangle(dpy,dr,gc,x+larg-1,y,1,alt);
Colore(w,gc,alto_sx);
XFillRectangle(dpy,dr,gc,x,y,larg-1,1);
XFillRectangle(dpy,dr,gc,x,y,1,alt-1);
}

void RilievoCornice(Widget w, Drawable dr, GC gc, int x, int y,
		int larg, int alt, int spessore, Pixel sfondo, int verso)
{
RilievoRgb base,luce,ombra,scuro;
int i,primo;
RilievoPixelRgb(w,sfondo,&base);
luce=RilievoSchiarisci(base,150);
ombra=RilievoScurisci(base,110);
scuro=RilievoScurisci(base,180);
primo=0;
if(spessore>=2)
	{
/* il contorno scuro, che stacca l'oggetto da quello che ha attorno */
	Anello(w,dr,gc,x,y,larg,alt,scuro,scuro);
	primo=1;
	}
else
	{
/* con un pixel solo luce e ombra devono bastare anche da contorno */
	luce=RilievoSchiarisci(base,150);
	ombra=RilievoScurisci(base,180);
	}
for(i=primo;i<spessore;i++)
	{
	if(verso==RILIEVO_SPORGE)
		Anello(w,dr,gc,x+i,y+i,larg-2*i,alt-2*i,luce,ombra);
	else
		Anello(w,dr,gc,x+i,y+i,larg-2*i,alt-2*i,ombra,luce);
	}
}
