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
   modulo SelettoreP.h
   tipo 
   release 1.2
   data 3/23/95
   reserved @(#)SelettoreP.h	1.2
*/
/*
 * SelettoreP.h - definizioni private per l'oggetto Selettore
 */ 
#ifndef _SelettoreP_h
#define _SelettoreP_h
/*
 Inclusione degli heder file privati delle classi superiori
*/
#include <Xm/XmP.h>
/*
 Inclusione dell'header file pubblico
*/
#include "Selettore.h"
/*
 Strutture relative alla nuova Classe
 */
typedef struct {
	int empty;
	} SelettoreClassPart;
/*
 Dichiarazione del record completo di classe
 */
typedef struct _SelettoreClassRec {
	 CoreClassPart       core_class;
	 SelettoreClassPart    selettore_class;
	} SelettoreClassRec;

extern SelettoreClassRec selettoreClassRec;

/*
 Nuovi campi per il record del widget Selettore
 */
typedef struct {
	/* risorse  */
	XtCallbackList callback_press;
	XtCallbackList callback_release;
	Pixel norm_fg;  /* foreground nello stato normale */
        Pixel norm_bg;  /* background nello stato normale */
	Pixmap selettore_0;  /* selettore nello stato 0 */
	Pixmap selettore_1; /* selettore nello stato 1 */
	int stato_fz;   /* stato a cui forzare il selettore */
	int disegno;    /* SELE_BITMAP, SELE_LEVA, SELE_IMPUGNATURA */

	/* variabili di stato (privato) */
	Pixmap pixmap_1;
	Pixmap pixmap_0;
        GC norm_gc;           /* GC stato normale  */
	GC clear_gc;
	int stato;            /* stato attuale del selettore */
	} SelettorePart;
/*
 Dichiarazione completa del record per instanziazione
 */
typedef struct _SelettoreRec {
	CorePart core;
	SelettorePart selettore;
	} SelettoreRec;

#endif /* _SelettoreP_h */
