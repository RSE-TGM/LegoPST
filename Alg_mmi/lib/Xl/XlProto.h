/*
 * XlProto.h - dichiarazioni delle funzioni della libreria Xl (e di quelle che
 * la libreria chiama altrove) che i sorgenti usavano SENZA dichiarazione.
 *
 * PERCHE' ESISTE
 *   Una funzione chiamata senza dichiarazione per il C ritorna un int. Su un
 *   sistema a 64 bit, per una funzione che ritorna un PUNTATORE (un Widget, una
 *   stringa, un XFontStruct *...) vuol dire che il chiamante tiene solo i 32
 *   bit bassi e ne estende il segno: il puntatore e' troncato. Verificato nei
 *   binari mmi, config e draw (cltq dopo la chiamata). Oggi funzionava per
 *   caso - eseguibili non-PIE e allocazioni piccole in basso nella memoria - e
 *   si sarebbe rotto con un gcc che produce eseguibili PIE (Ubuntu/Debian), o
 *   con allocazioni in indirizzi alti. Lo stesso vale, in piccolo, per Boolean
 *   (char) e Pixel (unsigned long) letti come int.
 *
 * COME E' SCRITTO
 *   Il TIPO DI RITORNO e' sempre quello della definizione: e' quello che
 *   conta. Per le funzioni definite in stile K&R la dichiarazione e' senza
 *   parametri, cioe' compatibile con la definizione qualunque siano i tipi
 *   degli argomenti; il prototipo completo c'e' solo dove serve, cioe' dove la
 *   definizione ha argomenti che le promozioni di default cambierebbero (float,
 *   Boolean, short passati per valore).
 *
 *   Non e' incluso da XlP.h, che usano anche Ol, le applicazioni e i
 *   convertitori: lo includono solo i sorgenti della libreria che ne hanno
 *   bisogno. Se un sorgente nuovo chiama una di queste funzioni, includa
 *   questo file invece di contare sulla dichiarazione implicita (che il
 *   makefile di Xl ora tratta come errore).
 *
 *   Dove una funzione e' definita: vedi il commento accanto.
 */
#ifndef _XlProto_h
#define _XlProto_h

#include <X11/Intrinsic.h>
#include <X11/Xlib.h>

/* --- XlWidgetUtil.c --------------------------------------------------- */
Boolean      XlAddBlink(Widget w, int ind, float frequenza, void *funzione);
Boolean      XlRemoveBlink();
XFontStruct *XlGetFont();
Boolean      XlCvtStrToPixel();
void         XlGetCambioColoreGC();
void         XlFreeCambioColoreGC();
void         XlInitializeWidgetValue();
int          XlGetZoom();
int          XlGetRefreshFreq();
Boolean      XlFlagRiIsOn();
Boolean      XlFlagFzIsOn();
Pixel        XlFlagToPixel();
int          XlValueToBlinkPixel();
int          XlValueToBlinkPixmap();

/* --- XlDrawingUtil.c -------------------------------------------------- */
int          XlTestoSimple();
int          XlTestoSimpleCentre();
void         gest_trasf_coor_composite();
void         gest_detrasf_coor_composite();
void         gest_trasf_coor_ico();
void         gest_trasf_wh_ico();
void         gest_trasf_arc_ico();

/* --- XlCompilaWidget.c, XlManager.c, XlCore.c, XlClassUtil.c ---------- */
void         XlErrComp();
Boolean      XlIsXlManager();
int          XlDeselChildren();
int          XlDeselSiblings();
void         XlRedisplay();

/* --- XlIconReg.c, XlDispReg.c ----------------------------------------- */
Boolean      XlIsIconReg();
Boolean      XlIsDispReg();
Boolean      XlCheckDispReg();

/* --- XlAllarmi_func.c (setta_colori_led ha un Boolean per valore: il suo
       prototipo sta in XlAllarmi.c, che conosce S_XLALLARMI) ------------ */
Widget       create_top_allarmi();
int          raccogli_dati_allarmi();
int          refresh_allarmi();
int          LoadBitmapLed();

/* --- XlCurve.c, XlCurve_func.c, XlCurve_conf.c ------------------------ */
Widget       create_xlcurve();
Widget       create_legend();
Widget       create_confRunTime();
Boolean      convStringToPix();
void         ActFisScale();
void         CleanFisScale();
void         PushedEffect();
void         SettingColorTrend();
void         SettingVarInfo();

/* --- SourceGrafica/funzioni.c, SourceGrafica/run_time_graf.c ---------- */
void         CreateARCH_GRAF();
void         CreatePLOT_GRAF();
void         CreateRUN_GRAF();
void         R_load_variables();
void         draw_proc();
void         handle_motion();
int          set_scala();
void         timer_proc();

/* --- XlGif.c ---------------------------------------------------------- */
int          LoadGIF();

/* --- libreria Ol ------------------------------------------------------ */
char        *OlGetNameVar();          /* OlDatabaseTopologia.c */
char        *OlEstrRegTaskName();     /* OlForm_regol.c */
Boolean      OlSetConn();             /* OlSetRes.c */

/* --- AlgLib ----------------------------------------------------------- */
void        *RtRecord();              /* Rt/RtMemory.c */
void         set_something_val();     /* libutilx/set_something_val.c */
void         get_something();         /* libutilx/get_something.c */
int          data();                  /* libutil/data.c */
int          ora();                   /* libutil/ora.c */
void         tomaius();               /* libutil/utile.c */

/* --- definite da ogni applicazione (UxXt.c, UxMethod.c di config,
       run_time, draw) ----------------------------------------------- */
void        *UxNewContext();
void        *UxMethodLookup();
int          UxMethodRegister();
int          UxNewClassId();
int          UxPutClassCode();

#endif
