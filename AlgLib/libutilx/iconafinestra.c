/*
   iconafinestra.c

   Icona a colori (_NET_WM_ICON) sulle finestre dei programmi Motif.
   I pixel sono compilati dentro il programma (icona_<nome>.c, generati da
   icone/icone_c.py): non si legge niente all'avvio, non servono LEGOROOT,
   libpng o libXpm.

   IconaFinestreApp si aggancia al changeHook di Xt (XtHooksOfDisplay), che
   Xt chiama a ogni XtRealizeWidget: cosi' ogni shell del programma (finestra
   principale, pagine, dialoghi) riceve l'icona appena ha una Window, senza
   toccare il punto in cui ciascuna viene creata. Le shell override (i menu)
   non sono WMShell e restano fuori.
*/
#include <stdlib.h>
#include <string.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xatom.h>
#include "iconafinestra.h"

/* con formato 32 Xlib vuole un long per valore (64 bit su x86_64), non un
   uint32: i valori si copiano in un array di long */
static void scrivi_icona(Display *dpy, Window win, const IconaArgb *icona)
{
    long *v;
    int i;

    if (dpy == NULL || win == None || icona == NULL || icona->n <= 0)
        return;
    v = (long *) malloc(icona->n * sizeof(long));
    if (v == NULL)
        return;
    for (i = 0; i < icona->n; i++)
        v[i] = (long) icona->dati[i];
    XChangeProperty(dpy, win, XInternAtom(dpy, "_NET_WM_ICON", False),
                    XA_CARDINAL, 32, PropModeReplace,
                    (unsigned char *) v, icona->n);
    free(v);
}

static Widget shell_di(Widget w)
{
    while (w != NULL && !XtIsShell(w))
        w = XtParent(w);
    return w;
}

void IconaFinestra(Widget w, const IconaArgb *icona)
{
    w = shell_di(w);
    if (w != NULL && XtIsWMShell(w) && XtIsRealized(w))
        scrivi_icona(XtDisplay(w), XtWindow(w), icona);
}

/* changeHook: tipo XtHrealizeWidget, widget appena realizzato */
static void dopo_realize(Widget hook, XtPointer client, XtPointer call)
{
    XtChangeHookDataRec *d = (XtChangeHookDataRec *) call;

    if (d == NULL || d->type == NULL || strcmp(d->type, XtHrealizeWidget) != 0)
        return;
    if (d->widget != NULL && XtIsWMShell(d->widget))
        IconaFinestra(d->widget, (const IconaArgb *) client);
}

void IconaFinestreApp(Widget w, const IconaArgb *icona)
{
    Widget hook;

    if (w == NULL)
        return;
    hook = XtHooksOfDisplay(XtDisplay(w));
    /* una sola volta per display, anche se chiamata di nuovo */
    XtRemoveCallback(hook, XtNchangeHook, dopo_realize, (XtPointer) icona);
    XtAddCallback(hook, XtNchangeHook, dopo_realize, (XtPointer) icona);
    IconaFinestra(w, icona);
}
