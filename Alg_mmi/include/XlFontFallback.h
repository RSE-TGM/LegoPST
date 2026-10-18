/*
   XlFontFallback.h

   Caricamento font core X11 con fallback a un font 8-bit
   (vedi Alg_mmi/lib/Xl/XlFontFallback.c).
*/
#ifndef XL_FONT_FALLBACK_H
#define XL_FONT_FALLBACK_H

#include <X11/Xlib.h>

#ifdef __cplusplus
extern "C" {
#endif

XFontStruct *XlLoadFontFallback(Display *disp, const char *fontname);
void XlInstallFontFallback(void);

#ifdef __cplusplus
}
#endif

#endif
