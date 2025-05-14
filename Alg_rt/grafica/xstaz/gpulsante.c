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
static char SccsID[] = "@(#)gpulsante.c	1.5\t3/23/95";
/*
   modulo gpulsante.c
   tipo 
   release 1.5
   data 3/23/95
   reserved @(#)gpulsante.c	1.5
*/
/*
	contiene le routine di gestione degli oggetti di tipo PULSANTE
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


#if defined  MOTIF10
typedef char*  XtPointer;
#endif

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
static void pressato_pulsante(),rilasciato_pulsante();


void crpulsante(istaz,wbox,p_ogg,p_r02)
int	istaz;		/* indice stazione */
Widget	wbox;		/* indice widget padre */
OGGETTO	*p_ogg;		/* puntatore oggetto in tabella new_staz */
TIPO_PULS *p_r02;	/* puntatore oggetto in tabella stazioni */
{
int i;
Widget wpuls;
DATI_REFRESH *p_refr;
char app[10];

/* crea il le lampada d */

i=0;
XtSetArg(args[i],XmNx,p_ogg->x);i++;
XtSetArg(args[i],XmNy,p_ogg->y);i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel);i++;
XtSetArg(args[i],XmNwidth,W_PULSANTE);i++;
XtSetArg(args[i],XmNheight,H_PULSANTE);i++;
XtSetArg(args[i],XtNnormBg,excolor_new[p_r02->colore].pixel); i++;
XtSetArg(args[i],XtNtipoBt,BOTTONE); i++;
wpuls=XtCreateManagedWidget("bt",bottoneWidgetClass,wbox,args,i);
XtAddCallback(wpuls,XtNpressBtCallback,pressato_pulsante,p_r02);
XtAddCallback(wpuls,XtNreleaseBtCallback,rilasciato_pulsante,p_r02);

}


static void pressato_pulsante(w,client_data,info)
Widget w;
XtPointer client_data;
XtPointer info;
{
int i=0,iret;
TIPO_PULS *p_r02;
float valfin ;

p_r02= (TIPO_PULS *) client_data;

/* se l' indice del comando e'  -1 non lo esegue */

if (p_r02->out.indice == -1 ) return;

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

/* gestisce solo le perturbazioni di tipo UP-DOWN     */

static void rilasciato_pulsante(w,client_data,info)
Widget w;
XtPointer client_data;
XtPointer info;
{
int i=0,iret;
TIPO_PULS *p_r02;
float valfin;

/* se l' indice del comando e'  -1 non lo esegue */

p_r02= (TIPO_PULS*) client_data;
if (p_r02->out.indice == -1 ) return;

if (p_r02->out.tipo_pert == UP_DOWN)
{
	valfin= -1.;
	iret=g_perturba(0,p_r02->out.tipo_pert,valfin,0.,0,p_r02->out.nmod,
							p_r02->out.indice);
}
}

