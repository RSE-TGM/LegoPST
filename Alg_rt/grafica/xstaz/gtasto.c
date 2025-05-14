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
static char SccsID[] = "@(#)gtasto.c	1.4\t3/23/95";
/*
   modulo gtasto.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)gtasto.c	1.4
*/
/*
	contiene le routine di gestione degli oggetti di tipo TASTO
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include "sim_param.h"
#include "xstaz.h"
#include "compstaz.h"


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

extern	int	stato_blink;


static void pressato_tasto();
extern DATI_REFRESH *occupa_trefr();
extern DATI_REFRESH *occupa_tredraw();

void crtasto(istaz,wbox,p_ogg,p_r02)
int	istaz;		/* indice stazione */
Widget	wbox;		/* indice widget padre */
OGGETTO	*p_ogg;		/* puntatore oggetto in tabella new_staz */
TIPO_TASTO *p_r02;	/* puntatore oggetto in tabella stazioni */
{
int i,htasto,wtasto;
Widget wpuls;
DATI_REFRESH *p_refr;
char app[10];

/* crea il tasto*/

switch (p_ogg->sottotipo)
{
	case  TASTO_NORM:
		wtasto=W_TASTO;
		htasto=H_TASTO;
	break;

        case  TASTO_GROSSO:
                wtasto=W_TASTO_GROSSO ;
                htasto=H_TASTO_GROSSO ;
        break;
}

i=0;
XtSetArg(args[i],XmNx,p_ogg->x);i++;
XtSetArg(args[i],XmNy,p_ogg->y);i++;
XtSetArg(args[i],XmNwidth,wtasto );i++;
XtSetArg(args[i],XmNheight,htasto );i++;
XtSetArg(args[i],XmNbackground,excolor_new[p_r02->colore].pixel); i++;
XtSetArg(args[i],XmNborderWidth,2);i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("",XmSTRING_DEFAULT_CHARSET)); i++;
wpuls=XmCreatePushButton(wbox,"tasto",args,i);
XtManageChild(wpuls);
XtAddCallback(wpuls,XmNactivateCallback,pressato_tasto,p_r02);

}


static void pressato_tasto(w,info,pushb)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *pushb;

{
int i=0,iret;
TIPO_TASTO *p_r02;
float valfin ;

p_r02= (TIPO_TASTO *) info;

switch	(p_r02->out.tipo_pert)
{
	case	STEP:
	case	IMPULSO:
		valfin=p_r02->out.value;
	break;
	case	NEGAZIONE:
	case	UP_DOWN:
		valfin =0;	/* non significativo */
	break;
}
iret=g_perturba(0,p_r02->out.tipo_pert,valfin,0.,0,p_r02->out.nmod,p_r02->out.indice);
}


