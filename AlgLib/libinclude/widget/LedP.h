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
   modulo LedP.h
   tipo 
   release 1.2
   data 3/23/95
   reserved @(#)LedP.h	1.2
*/
/*
 * LedP.h - definizioni private per l'oggetto Led
 */ 
#ifndef _LedP_h
#define _LedP_h
/*
 Inclusione degli heder file privati delle classi superiori
*/
#include <Xm/XmP.h>
/*
 Inclusione dell'header file pubblico
*/
#include "Led.h"
/*
 Strutture relative alla nuova Classe
 */
typedef struct {
	int empty;
	} LedClassPart;
/*
 Dichiarazione del record completo di classe
 */
typedef struct _LedClassRec {
	 CoreClassPart       core_class;
	 LedClassPart    led_class;
	} LedClassRec;

extern LedClassRec ledClassRec;

/*
 Nuovi campi per il record del widget Led
 */
typedef struct {
	/* risorse  */
	Pixel color_blink;
	int blink_on;   /* stato di eventuale blink */

	int alterna;
	Pixel color_norm;
	XtIntervalId time_id; 
	int bordo;	/* spessore del bordo X richiesto, ridisegnato in rilievo */
	GC gc;		/* GC per il disegno del rilievo */
	} LedPart;
/*
 Dichiarazione completa del record per instanziazione
 */
typedef struct _LedRec {
	CorePart core;
	LedPart led;
	} LedRec;

#endif /* _LedP_h */
