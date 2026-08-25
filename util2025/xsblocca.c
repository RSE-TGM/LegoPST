/**********************************************************************
 *  xsblocca - diagnosi e sblocco del puntatore/tastiera su X.
 *
 *  Serve quando il mouse "sparisce" o non reagisce piu': quasi sempre e'
 *  un client X che ha lasciato attivo un GRAB (tipico dei menu Motif:
 *  xstaz, net_monit, banco, config aprono popup con grab del puntatore).
 *  Finche' quel client vive, il server X consegna tutti gli eventi solo a
 *  lui. Chiudendo la sua connessione il grab decade e il puntatore torna.
 *
 *  Uso:
 *      xsblocca               dice se puntatore/tastiera sono bloccati
 *      xsblocca -l            elenca le finestre di primo livello
 *      xsblocca -k <titolo>   chiude i client delle finestre il cui titolo
 *                             contiene <titolo>  (equivale a xkill)
 *
 *  Non richiede il mouse: si lancia dal terminale, che in WSLg e' una
 *  finestra Windows e resta utilizzabile anche a puntatore bloccato.
 **********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <X11/Xlib.h>

static Display *d;
static int trovate;

static int stato_grab(void) {
    Window r = DefaultRootWindow(d);
    int p, t, bloccato = 0;
    p = XGrabPointer(d, r, True, 0, GrabModeSync, GrabModeAsync, None, None, CurrentTime);
    if (p == GrabSuccess) { XUngrabPointer(d, CurrentTime); printf("  puntatore: libero\n"); }
    else { printf("  puntatore: BLOCCATO da un altro client (codice %d)\n", p); bloccato = 1; }
    t = XGrabKeyboard(d, r, True, GrabModeSync, GrabModeAsync, CurrentTime);
    if (t == GrabSuccess) { XUngrabKeyboard(d, CurrentTime); printf("  tastiera : libera\n"); }
    else { printf("  tastiera : BLOCCATA da un altro client (codice %d)\n", t); bloccato = 1; }
    return bloccato;
}

static void visita(Window w, const char *filtro, int uccidi) {
    Window root, parent, *figli; unsigned int n, i;
    char *nome = NULL;
    if (XFetchName(d, w, &nome) && nome) {
        if (strlen(nome) && (!filtro || strstr(nome, filtro))) {
            trovate++;
            printf("  0x%-9lx %s%s\n", w, nome, uccidi ? "   -> chiuso" : "");
            if (uccidi) { XKillClient(d, w); XFlush(d); }
        }
        XFree(nome);
    }
    if (XQueryTree(d, w, &root, &parent, &figli, &n)) {
        for (i = 0; i < n; i++) visita(figli[i], filtro, uccidi);
        if (figli) XFree(figli);
    }
}

int main(int argc, char **argv) {
    if (!(d = XOpenDisplay(NULL))) {
        fprintf(stderr, "xsblocca: display non raggiungibile (DISPLAY=%s)\n",
                getenv("DISPLAY") ? getenv("DISPLAY") : "");
        return 1;
    }
    if (argc == 1) {
        if (stato_grab())
            printf("\n  Per liberare:  xsblocca -l   (elenca)  poi  xsblocca -k <titolo>\n"
                   "  Oppure, se sono applicazioni LegoPST:  killsim\n");
    } else if (!strcmp(argv[1], "-l")) {
        visita(DefaultRootWindow(d), NULL, 0);
        printf("  %d finestre\n", trovate);
    } else if (!strcmp(argv[1], "-k") && argc > 2) {
        visita(DefaultRootWindow(d), argv[2], 1);
        printf("  %d client chiusi\n", trovate);
        XSync(d, False);
        stato_grab();
    } else {
        fprintf(stderr, "uso: xsblocca [-l | -k <titolo>]\n");
        return 2;
    }
    XCloseDisplay(d);
    return 0;
}
