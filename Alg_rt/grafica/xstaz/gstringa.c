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
static char SccsID[] = "@(#)gstringa.c	1.6\t3/23/95";
/*
   modulo gstringa.c
   tipo 
   release 1.6
   data 3/23/95
   reserved @(#)gstringa.c	1.6
*/

/*
	contiene le routine di gestione degli oggetti di tipo STRINGA
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
extern XColor excolor_new[],excolor_blink[],sfondo_label,sfondo_staz;
extern int font_height;
extern int font_ascent;
extern int font_width;
extern XmFontList fontlist ,fontlist_big;

extern DATI_REFRESH *occupa_trefr();
extern DATI_REFRESH *occupa_tredraw();

void crstringa(istaz,wbox,p_ogg,p_r02)
int	istaz;		/* indice stazione */
Widget	wbox;		/* indice widget padre */
OGGETTO	*p_ogg;		/* puntatore oggetto in tabella new_staz */
TIPO_STRINGA_DESCR *p_r02;	/* puntatore oggetto in tabella stazioni */
{
int i;
Widget wstringa;
Dimension width_label;

/* crea il stringa */

if (strlen(p_r02->etic) == 0)  return;
i=0;
XtSetArg(args[i],XmNwidth,&width_label); i++;
XtGetValues(wbox,args,i);
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(p_r02->etic,XmSTRING_DEFAULT_CHARSET)); i++;
	XtSetArg(args[i],XmNx,p_ogg->x);i++;
	XtSetArg(args[i],XmNy,p_ogg->y);i++;
	XtSetArg(args[i],XmNwidth,width_label); i++;
switch (p_ogg->sottotipo)
{
	case STRINGA_GEN:
	XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
	XtSetArg(args[i],XmNfontList,fontlist); i++;
	XtSetArg(args[i],XmNalignment,XmALIGNMENT_BEGINNING); i++;
	break;

	case STRINGA_TESTO:
	XtSetArg(args[i],XmNalignment,XmALIGNMENT_CENTER); i++;
        XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
	XtSetArg(args[i],XmNfontList,fontlist_big); i++;
	break;

	case STRINGA_ETIC:
        XtSetArg(args[i],XmNwidth,width_label-p_ogg->x); i++;
        XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
        XtSetArg(args[i],XmNfontList,fontlist); i++;
        XtSetArg(args[i],XmNalignment,XmALIGNMENT_BEGINNING); i++;
        break;

	case STRINGA_ETIC_BIG:
        XtSetArg(args[i],XmNwidth,width_label-p_ogg->x); i++;
        XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
        XtSetArg(args[i],XmNfontList,fontlist_big); i++;
        XtSetArg(args[i],XmNalignment,XmALIGNMENT_BEGINNING); i++;
        break;
}

wstringa=(Widget) XmCreateLabel(wbox,"lab",args,i);
XtManageChild(wstringa);

}


