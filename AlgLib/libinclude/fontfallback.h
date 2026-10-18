/*
   fontfallback.h

   Caricamento font core X11 con fallback a un font 8-bit, contro il
   testo a riquadri di Motif con i font iso10646 (2-byte).
   Vedi AlgLib/libutilx/fontfallback.c.
*/
#ifndef FONTFALLBACK_H
#define FONTFALLBACK_H

#include <X11/Xlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* carica un font 8-bit: il nome richiesto o il sostituto piu' vicino */
XFontStruct *FontFallbackLoad(Display *disp, const char *fontname);

/* da chiamare dopo XtAppInitialize: fallback in tutte le conversioni
   String->FontList di Motif (risorse e UxConvertFontList) */
void FontFallbackInstall(void);

#ifdef __cplusplus
}
#endif

#endif
