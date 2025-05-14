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
static char SccsID[] = "@(#)gled.c	1.4\t3/23/95";
/*
   modulo gled.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)gled.c	1.4
*/
/*
	contiene le routine di gestione degli oggetti di tipo LED
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>
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
extern XColor excolor_new[],excolor_blink[],sfondo_staz;
extern int font_height;
extern int font_ascent;
extern int font_width;
extern XmFontList fontlist;

extern	int  stato_blink;

extern DATI_REFRESH *occupa_trefr();
extern DATI_REFRESH *occupa_tredraw();
void led_refresh();
void led_del_callback();

float estr_sh();

void crled(istaz,wbox,p_ogg,p_r02)
int	istaz;		/* indice stazione */
Widget	wbox;		/* indice widget padre */
OGGETTO	*p_ogg;		/* puntatore oggetto in tabella new_staz */
TIPO_LED *p_r02;	/* puntatore oggetto in tabella stazioni */
{
int i, xetic, yetic, width_etic;
Widget wled,wlab;
DATI_REFRESH *p_refr;
char app[10];

/* crea il led */

i=0;
XtSetArg(args[i],XmNx,p_ogg->x);i++;
XtSetArg(args[i],XmNy,p_ogg->y);i++;
XtSetArg(args[i],XmNwidth,W_LED);i++;
XtSetArg(args[i],XmNheight,H_LED);i++;
/*
	i led spendi hanno il colore dello sfondo 
*/
/*XtSetArg(args[i],XmNbackground,excolor_new[p_r02->colore].pixel); i++;*/
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
XtSetArg(args[i],XtNledColorBlink,excolor_blink[p_r02->colore].pixel); i++;
/*XtSetArg(args[i],XtNledBlinkOn,0); i++;*/
wled=XtCreateManagedWidget("bt",ledWidgetClass,wbox,args,i);


/*
        inserisce il riferimento per l'oggetto nella tabella
        trefr_newstaz
*/

        p_refr=occupa_trefr(wled,p_r02);
        if (p_refr ==  NULL )
		exit(puts("gled : troppi oggetti creati "));
/*
	aggiunge la callback di cancellazione dell 'oggetto
*/
XtAddCallback(wled,XmNdestroyCallback,led_del_callback,p_refr);

/* aggiunge la descrizione del led a destra dello stesso o sotto
   a seconda di come e' specificato dal sottotipo */

i=0;
if (strlen(p_r02->etic))
{
	if (p_ogg->sottotipo == ETIC_SOTTO) 
	{
		xetic=p_ogg->x-5;
		yetic=p_ogg->y +H_LED+2;
		XtSetArg(args[i],XmNalignment,XmALIGNMENT_CENTER); i++;
	}
	else
	{
       		 xetic=p_ogg->x +W_LED+4;
    		    yetic=p_ogg->y-2;
		XtSetArg(args[i],XmNalignment,XmALIGNMENT_BEGINNING); i++;
	}

  	width_etic = font_width*6;
	if (p_ogg->flag == ETIC_MAX) width_etic = font_width*20;

	XtSetArg(args[i],XmNx,xetic);i++;
	XtSetArg(args[i],XmNy,yetic);i++;
	XtSetArg(args[i],XmNfontList,fontlist);i++;
	XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(p_r02->etic,XmSTRING_DEFAULT_CHARSET)); i++;
	XtSetArg(args[i],XmNmarginWidth,0); i++;
	XtSetArg(args[i],XmNwidth,width_etic); i++;
	XtSetArg(args[i],XmNmarginHeight,0); i++;
	XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
	wlab=XmCreateLabel(wbox,"lab",args,i);
	XtManageChild(wlab);
}

/* 
	aggiunge la callbacks  di refresh del led
*/

	if(add_refresh((XtCallbackProc)led_refresh,(caddr_t)p_refr)==-1)
	{
		libera_trefr(p_refr);
       		printf("\n errore : refresh non aggiunto");
	}
}

void led_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
del_refresh((void*)info);
libera_trefr((DATI_REFRESH *)info);
}

void led_refresh(info)
caddr_t info;
{
DATI_REFRESH *p_refr;
TIPO_LED *pled;
int	i, vcolore, vblink;

p_refr=(DATI_REFRESH *) info;
pled= (TIPO_LED *) p_refr->pdati;

/* raccoglie i valori delle variabili INPUT e INPUT_BLINK associate
	al led */

vcolore = (int)estr_sh(pled->input_colore,1+pled->neg_colore);
vblink = (int)estr_sh(pled->input_blink,1+pled->neg_blink);
i=0;
if (vcolore) 
{
	/* se input_colore = 1 led acceso fisso */
	XtSetArg(args[i],XmNbackground,excolor_blink[pled->colore].pixel); i++;
/****	XtSetArg(args[i],XtNledBlinkOn,0); i++;      *****/
}
else if ( vblink )
	{
	/* se solo input_blink = 1 led blinking */
/********	        XtSetArg(args[i],XtNledBlinkOn,1); i++;   *****/
	blink_led(p_refr->w);
	}
else
{
/*	se entrambe a zero led spento e fisso del colore dello sfondo*/

/*	XtSetArg(args[i],XmNbackground,excolor_new[pled->colore].pixel); i++;*/
	XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
/****	XtSetArg(args[i],XtNledBlinkOn,0); i++;      *****/
}
XtSetValues(p_refr->w,args,i); 
}


void blink_led(wled)
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

