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
   modulo BottoneP.h
   tipo 
   release 1.2
   data 3/23/95
   reserved @(#)BottoneP.h	1.2
*/
/*
 * BottoneP.h - definizioni private per l'oggetto Bottone
 */ 
#ifndef _BottoneP_h
#define _BottoneP_h
/*
 Inclusione degli heder file privati delle classi superiori
*/
#include <Xm/XmP.h>
/*
 Inclusione dell'header file pubblico
*/
#include "Bottone.h"
/*
 Strutture relative alla nuova Classe
 */
typedef struct {
	int empty;
	} BottoneClassPart;
/*
 Dichiarazione del record completo di classe
 */
typedef struct _BottoneClassRec {
	 CoreClassPart       core_class;
	 BottoneClassPart    bottone_class;
	} BottoneClassRec;

extern BottoneClassRec bottoneClassRec;

/*
 Nuovi campi per il record del widget Bottone
 */
typedef struct {
	/* risorse  */
	XtCallbackList callback_press;
	XtCallbackList callback_release;
	Pixel norm_fg;  /* foreground nello stato normale */
        Pixel act_fg;   /* foreground nello stato attivato */
        Pixel norm_bg;  /* background nello stato normale */
        int tipo_bt;  /* tipo di bottone */
        Pixel color_lamp; /* colore della lampada */
	Pixel color_blink; /* colore stato blinkante */
	int blink_on;      /* 1 se lampada in stato blinkante */

	/* variabili di stato (privato) */
	Pixmap bottone_norm;  /* bottone nello stato normale */
	Pixmap bottone_blink; /* bottone nello stato blinkante */
        GC norm_gc;           /* GC stato normale  */
	GC norm_bg_gc;        /* GC background bottone stato normale */
	GC act_gc;            /* GC stato attivato */
        GC clear_gc;  	      /* GC sbiancamento iniziale pixmap */
	GC lamp_gc;	      /* GC colorazione lampada */
	GC blink_gc;          /* GC lampada in stato di blinking */
	XtIntervalId time_id; /* id per timeout blinking */
	int alterna; /* per realizzazione blinking */
	int premuto; /* 1 tra Btn1Down e Btn1Up: tasto disegnato incavato */
	} BottonePart;
/*
 Dichiarazione completa del record per instanziazione
 */
typedef struct _BottoneRec {
	CorePart core;
	BottonePart bottone;
	} BottoneRec;

#endif /* _BottoneP_h */
