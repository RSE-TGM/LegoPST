/*
 *  Rilievo.h - primitive di disegno "in rilievo" per le stazioni di xstaz
 *
 *  Luce sempre in alto a sinistra. I colori di luce e ombra si ricavano
 *  dal colore base mescolandolo con bianco o nero (Rilievo.c).
 */
#ifndef _Rilievo_h
#define _Rilievo_h

#include <X11/Intrinsic.h>

typedef struct {
	int r,g,b;   /* 0..255 */
	} RilievoRgb;

/* verso della luce per sfumature e cornici */
#define RILIEVO_SPORGE    1
#define RILIEVO_INCAVATO -1

void RilievoPixelRgb(Widget w, Pixel p, RilievoRgb *c);
Pixel RilievoRgbPixel(Widget w, RilievoRgb *c);
RilievoRgb RilievoMescola(RilievoRgb a, RilievoRgb b, int t);
RilievoRgb RilievoSchiarisci(RilievoRgb a, int t);
RilievoRgb RilievoScurisci(RilievoRgb a, int t);

/* cerchio (x,y,d) sfumato: bordo -> medio a meta' raggio -> centro verso la luce */
void RilievoSfumaCerchio(Widget w, Drawable dr, GC gc, int x, int y, int d,
		RilievoRgb bordo, RilievoRgb medio, RilievoRgb centro, int verso);
void RilievoContornoCerchio(Widget w, Drawable dr, GC gc, int x, int y, int d,
		RilievoRgb c);
/* il punto chiaro del riflesso, verso la luce */
void RilievoRiflesso(Widget w, Drawable dr, GC gc, int x, int y, int d,
		RilievoRgb base, int forza);

/* rettangolo sfumato dall'alto al basso: alto -> medio a meta' -> basso */
void RilievoSfumaRett(Widget w, Drawable dr, GC gc, int x, int y,
		int larg, int alt, RilievoRgb alto, RilievoRgb medio, RilievoRgb basso);

/*
 Cornice di "spessore" pixel attorno al rettangolo (x,y,larg,alt), dentro
 il rettangolo stesso. Il pixel piu' esterno e' un contorno scuro che
 stacca l'oggetto da quello che ha attorno; gli altri sono luce e ombra.
 "sfondo" e' il colore da cui si ricavano luce e ombra.
*/
void RilievoCornice(Widget w, Drawable dr, GC gc, int x, int y,
		int larg, int alt, int spessore, Pixel sfondo, int verso);

#endif /* _Rilievo_h */
