/*
   iconafinestra.h

   Icona a colori delle finestre dei programmi Motif (config, xstaz, mmi)
   sulla barra delle applicazioni: proprieta' _NET_WM_ICON (ARGB), quella
   che usano WSLg, GNOME e KDE. La vecchia XmNiconPixmap (WM_HINTS, un bit)
   non la guarda piu' nessuno.
   Vedi AlgLib/libutilx/iconafinestra.c e icone/README.md.
*/
#ifndef ICONAFINESTRA_H
#define ICONAFINESTRA_H

#include <X11/Intrinsic.h>

#ifdef __cplusplus
extern "C" {
#endif

/* pixel di un'icona nel formato di _NET_WM_ICON: per ogni lato larghezza,
   altezza e larghezza*altezza pixel ARGB. n = numero di valori */
typedef struct {
    int                 n;
    const unsigned int *dati;
} IconaArgb;

/* generate da icone/icone_c.py (AlgLib/libutilx/icona_<nome>.c) */
extern const IconaArgb IconaConfig;
extern const IconaArgb IconaXstaz;
extern const IconaArgb IconaMmi;

/* mette l'icona sulla shell di w, se e' gia' realizzata */
void IconaFinestra(Widget w, const IconaArgb *icona);

/* mette l'icona sulla shell di w e su tutte le finestre del programma
   realizzate da ora in poi sullo stesso display (hook di Xt): basta
   chiamarla una volta, dopo XtAppInitialize */
void IconaFinestreApp(Widget w, const IconaArgb *icona);

#ifdef __cplusplus
}
#endif

#endif
