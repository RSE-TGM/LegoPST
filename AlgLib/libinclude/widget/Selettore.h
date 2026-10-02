/**********************************************************************
*
*       C Header:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
/*
   modulo Selettore.h
   tipo 
   release 1.2
   data 3/23/95
   reserved @(#)Selettore.h	1.2
*/
/*
 * Selettore.h - file di include pubblico per l'utilizzo del widget
 *             selettore
 */
#ifndef _Selettore_h
#define _Selettore_h

/*
 nomi di risorse utilizzate internamente al selettore
 */
#define XtNpixmap0  "pixmap0"
#define XtCPixmap0  "Pixmap0"
#define XtNpixmap1  "pixmap1"
#define XtCPixmap1  "Pixmap1"
#define XtNstatoSel "statoSel"
#define XtCStatoSel "StatoSel"
#define XtNseleFg "seleFg"
#define XtCSeleFg "SeleFg"
#define XtNseleBg "seleBg"
#define XtCSeleBg "SeleBg"

/*
 come si disegna il selettore: con le bitmap di pixmap0/pixmap1 (com'era)
 oppure in rilievo, con la leva dritta (tipo A di xstaz) o a impugnatura
 (tipo B)
*/
#define XtNdisegnoSel "disegnoSel"
#define XtCDisegnoSel "DisegnoSel"
#define SELE_BITMAP      0
#define SELE_LEVA        1
#define SELE_IMPUGNATURA 2

#define XtNpressSelCallback "pressSelCallback"
#define XtCPressSelCallback "PressSelCallback"
#define XtNreleaseSelCallback "releaseSelCallback"
#define XtCReleaseSelCallback "ReleaseSelCallback"


/* declare the class constant */
extern WidgetClass selettoreWidgetClass;

/* dichiarazione tipi per Classe e Instanziazione di Selettore */
typedef struct _SelettoreClassRec *SelettoreWidgetClass;
typedef struct _SelettoreRec *SelettoreWidget;

#endif /* _Selettore_h */

