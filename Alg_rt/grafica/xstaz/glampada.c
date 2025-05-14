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
static char SccsID[] = "@(#)glampada.c	1.4\t3/23/95";
/*
   modulo glampada.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)glampada.c	1.4
*/
/*
	contiene le routine di gestione degli oggetti di tipo LAMPADA
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
#include "BottoneP.h"
#include "Bottone.h"

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

extern	int	stato_blink;

extern DATI_REFRESH *occupa_trefr();
extern DATI_REFRESH *occupa_tredraw();
void lampada_refresh();
void lampada_del_callback();

float estr_sh();

void crlampada(istaz,wbox,p_ogg,p_r02)
int	istaz;		/* indice stazione */
Widget	wbox;		/* indice widget padre */
OGGETTO	*p_ogg;		/* puntatore oggetto in tabella new_staz */
TIPO_LAMPADA *p_r02;	/* puntatore oggetto in tabella stazioni */
{
int i;
Widget wlampada;
DATI_REFRESH *p_refr;
char app[10];

/* crea il le lampada d */

/*printf("\n colore %d var colore %d var blink %d ",p_r02->colore,p_r02->input_colore,p_r02->input_blink);*/
i=0;
XtSetArg(args[i],XmNx,p_ogg->x);i++;
XtSetArg(args[i],XmNy,p_ogg->y);i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel);i++;
XtSetArg(args[i],XmNwidth,W_LAMPADA);i++;
XtSetArg(args[i],XmNheight,H_LAMPADA);i++;
XtSetArg(args[i],XtNtipoBt,BOTTONE_LUCE); i++;
XtSetArg(args[i],XtNnormBg,excolor_new[p_r02->colore].pixel); i++;
XtSetArg(args[i],XtNcolorBlink,excolor_blink[p_r02->colore].pixel); i++;
wlampada=XtCreateManagedWidget("bt",bottoneWidgetClass,wbox,args,i);


/*
        inserisce il riferimento per l'oggetto nella tabella
        trefr_newstaz
*/

        p_refr=occupa_trefr(wlampada,p_r02);
        if (p_refr ==  NULL )
		exit(puts("glampada : troppi oggetti creati "));
/*
	aggiunge la callback di cancellazione dell 'oggetto
*/
XtAddCallback(wlampada,XmNdestroyCallback,lampada_del_callback,p_refr);

/* 
	aggiunge la callbacks  di refresh della lampada
*/

	if(add_refresh((XtCallbackProc)lampada_refresh,(caddr_t)p_refr)==-1)
	{
		libera_trefr(p_refr);
       		printf("\n errore : refresh non aggiunto");
	}
}

void lampada_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
del_refresh((void*)info);
libera_trefr((DATI_REFRESH *)info);
}

void lampada_refresh(info)
caddr_t info;
{
DATI_REFRESH *p_refr;
TIPO_LAMPADA *pr02;
int	i, vcolore, vblink;
BottoneWidget cw;

p_refr=(DATI_REFRESH *) info;
pr02= (TIPO_LAMPADA *) p_refr->pdati;
cw = (BottoneWidget) p_refr->w;

/* raccoglie i valori delle variabili INPUT e INPUT_BLINK associate
	alla lampada  */

vcolore = (int)estr_sh(pr02->input_colore,1+pr02->neg_colore);
vblink = (int)estr_sh(pr02->input_blink,1+pr02->neg_blink);
i=0;
if (vcolore) 
{
	/* se input_colore = 1 led acceso fisso */
/*******
	XtSetArg(args[i],XtNnormBg,excolor_blink[pr02->colore].pixel); i++;
	XtSetValues(p_refr->w,args,i);  
****/
        XCopyArea(XtDisplay(cw),cw->bottone.bottone_blink,
          XtWindow(cw),cw->bottone.norm_gc,0,0,
          cw->core.width,cw->core.height,0,0);
}
else if ( vblink)
	{
	/* se solo input_blink = 1 led blinking */
/********	        XtSetArg(args[i],XtNblinkOn,1); i++;   *****/
	blink_lampada(p_refr->w,pr02);
	}
else
{
/*	se entrambe a zero led spento e fisso */
/****
	XtSetArg(args[i],XtNnormBg,excolor_new[pr02->colore].pixel); i++; 
	XtSetValues(p_refr->w,args,i);  
***/
        XCopyArea(XtDisplay(cw),cw->bottone.bottone_norm,
          XtWindow(cw),cw->bottone.norm_gc,0,0,
          cw->core.width,cw->core.height,0,0);
}
/***XtSetValues(p_refr->w,args,i);  ***/
}


void blink_lampada(wled,pr02)
Widget wled;
TIPO_LAMPADA *pr02;
{
int i=0;
BottoneWidget cw= (BottoneWidget) wled;
if(stato_blink)
        {
        XCopyArea(XtDisplay(cw),cw->bottone.bottone_blink,
          XtWindow(cw),cw->bottone.norm_gc,0,0,
          cw->core.width,cw->core.height,0,0);
/***********
        XtSetArg(args[i],XtNnormBg,excolor_blink[pr02->colore].pixel); i++;
	XtSetValues(wled,args,i);
**********/
        }
else
        {
        XCopyArea(XtDisplay(cw),cw->bottone.bottone_norm,
          XtWindow(cw),cw->bottone.norm_gc,0,0,
          cw->core.width,cw->core.height,0,0);
/******
        XtSetArg(args[i],XtNnormBg,excolor_new[pr02->colore].pixel); i++;
	XtSetValues(wled,args,i);
*******/
        }
}

