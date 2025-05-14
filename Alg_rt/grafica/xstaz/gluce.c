/**********************************************************************
*
*       C Source:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: %  (%full_filespec: %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)gluce.c	1.4\t3/23/95";
/*
   modulo gluce.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)gluce.c	1.4
*/
/*
	contiene le routine di gestione degli oggetti di tipo LUCE
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include "sim_param.h"
#include "xstaz.h"
#include "compstaz.h"
#include "LedP.h"
#include "Led.h"

/*
  tabelle comuni utilizzate per tutte le stazioni
*/
extern Arg args[];
extern S_PAGINA *pagina;
extern S_STAZIONE *stazione;
extern S_PAGVIS pagvis[MAX3_PAG];
extern TIPI_NEWSTAZ new_staz[];
extern DATI_REFRESH trefr_newstaz[];


extern Display *display;
extern XColor excolor_new[],excolor_blink[];
extern int font_height;
extern int font_ascent;
extern int font_width;
extern XmFontList fontlist;

extern	int  stato_blink;

extern DATI_REFRESH *occupa_trefr();
extern DATI_REFRESH *occupa_tredraw();
void luce_refresh();
void luce_del_callback();

float estr_sh();


void crluce(istaz,wbox,p_ogg,p_r02)
int	istaz;		/* indice stazione */
Widget	wbox;		/* indice widget padre */
OGGETTO	*p_ogg;		/* puntatore oggetto in tabella new_staz */
TIPO_LUCE *p_r02;	/* puntatore oggetto in tabella stazioni */
{
int i;
Widget wluce;
DATI_REFRESH *p_refr;

/* crea l'oggetto  luce */

/*printf("\n colore %d var colore %d var blink %d \n",p_r02->colore,p_r02->input_colore,p_r02->input_blink);*/
i=0;
XtSetArg(args[i],XmNx,p_ogg->x);i++;
XtSetArg(args[i],XmNy,p_ogg->y);i++;
XtSetArg(args[i],XmNwidth,W_LUCE);i++;
XtSetArg(args[i],XmNheight,H_LUCE);i++;
XtSetArg(args[i],XmNbackground,excolor_new[p_r02->colore].pixel); i++;
XtSetArg(args[i],XtNledColorBlink,excolor_blink[p_r02->colore].pixel); i++;
/*XtSetArg(args[i],XtNledBlinkOn,0); i++;*/
wluce=XtCreateManagedWidget("bt",ledWidgetClass,wbox,args,i);


/*
        inserisce il riferimento per l'oggetto nella tabella
        trefr_newstaz
*/

        p_refr=(DATI_REFRESH *) occupa_trefr(wluce,p_r02);
        if (p_refr ==  NULL )
		exit(puts("gluce : troppi oggetti creati "));
/*
	aggiunge la callback di cancellazione dell 'oggetto
*/
XtAddCallback(wluce,XmNdestroyCallback,luce_del_callback,p_refr);

/* 
	aggiunge la callbacks  di refresh del luce
*/

	if(add_refresh((XtCallbackProc)luce_refresh,(caddr_t)p_refr)==-1)
	{
		libera_trefr(p_refr);
       		printf("\n errore : refresh non aggiunto");
	}
}

void luce_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
del_refresh((void*)info);
libera_trefr((DATI_REFRESH *)info);
}

void luce_refresh(info)
caddr_t info;
{
DATI_REFRESH *p_refr;
TIPO_LUCE *p_r02;
int	i, vcolore, vblink;

p_refr=(DATI_REFRESH *) info;
p_r02= (TIPO_LUCE *) p_refr->pdati;

/* raccoglie i valori delle variabili INPUT e INPUT_BLINK associate
	al led */

vcolore = (int)estr_sh(p_r02->input_colore,1+p_r02->neg_colore);
vblink = (int)estr_sh(p_r02->input_blink,1+p_r02->neg_blink);
i=0;
if (vcolore) 
{
	/* se input_colore = 1 led acceso fisso */
	XtSetArg(args[i],XmNbackground,excolor_blink[p_r02->colore].pixel); i++;
/****	XtSetArg(args[i],XtNledBlinkOn,0); i++;      *****/
}
else if ( vblink )
	{
	/* se solo input_blink = 1 led blinking */
/********	        XtSetArg(args[i],XtNledBlinkOn,1); i++;   *****/
	blink_luce(p_refr->w);
	}
else
{
/*	se entrambe a zero luce  spento e fisso */
	XtSetArg(args[i],XmNbackground,excolor_new[p_r02->colore].pixel); i++;
/****	XtSetArg(args[i],XtNledBlinkOn,0); i++;      *****/
}
XtSetValues(p_refr->w,args,i); 
}


void blink_luce(wled)
Widget wled;
{
int i;
Arg args[3];
LedWidget cw=(LedWidget) wled;
if(stato_blink)
        {
        i=0;
        XtSetArg(args[i],XmNbackground,cw->led.color_blink); i++;
        XtSetValues((Widget)cw,args,i);
        }
else
        {
        i=0;
        XtSetArg(args[i],XmNbackground,cw->led.color_norm); i++;
        XtSetValues((Widget)cw,args,i);
        }
}

