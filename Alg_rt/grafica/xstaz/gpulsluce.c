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
static char SccsID[] = "@(#)gpulsluce.c	1.5\t3/23/95";
/*
   modulo gpulsluce.c
   tipo 
   release 1.5
   data 3/23/95
   reserved @(#)gpulsluce.c	1.5
*/
/*
	contiene le routine di gestione degli oggetti di tipo PULS_LUCE
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
typedef char *      XtPointer;
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
static void puls_del_callback();
static void puls_refresh();
static void pressato(),rilasciato();

float estr_sh();

void crpulsluce(istaz,wbox,p_ogg,p_r02)
int	istaz;		/* indice stazione */
Widget	wbox;		/* indice widget padre */
OGGETTO	*p_ogg;		/* puntatore oggetto in tabella new_staz */
TIPO_PULS_LUCE *p_r02;	/* puntatore oggetto in tabella stazioni */
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
XtSetArg(args[i],XtNcolorLamp,excolor_new[p_r02->colore].pixel); i++;
XtSetArg(args[i],XtNcolorBlink,excolor_blink[p_r02->colore].pixel); i++;
wpuls=XtCreateManagedWidget("bt",bottoneWidgetClass,wbox,args,i);
XtAddCallback(wpuls,XtNpressBtCallback,pressato,p_r02);
XtAddCallback(wpuls,XtNreleaseBtCallback,rilasciato,p_r02);

/*
        inserisce il riferimento per l'oggetto nella tabella
        trefr_newstaz
*/

        p_refr=(DATI_REFRESH *) occupa_trefr(wpuls,p_r02);
        if (p_refr ==  NULL )
		exit(puts("gpulsluce : troppi oggetti creati "));
/*
	aggiunge la callback di cancellazione dell 'oggetto
*/
XtAddCallback(wpuls,XmNdestroyCallback,puls_del_callback,p_refr);

/* 
	aggiunge la callbacks  di refresh della lampada
*/

	if(add_refresh((XtCallbackProc)puls_refresh,(caddr_t)p_refr)==-1)
	{
		libera_trefr(p_refr);
       		printf("\n errore : refresh non aggiunto");
	}
}

static void puls_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
del_refresh((void*)info);
libera_trefr((DATI_REFRESH *)info);
}

static void puls_refresh(info)
caddr_t info;
{
DATI_REFRESH *p_refr;
TIPO_PULS_LUCE *pr02;
int	i, vcolore, vblink;
BottoneWidget cw;

p_refr=(DATI_REFRESH *) info;
pr02= (TIPO_PULS_LUCE *) p_refr->pdati;
cw = (BottoneWidget) p_refr->w;

/* raccoglie i valori delle variabili INPUT e INPUT_BLINK associate
	al pulsante   */

vcolore = (int)estr_sh(pr02->input_colore,1+pr02->neg_colore);
vblink = (int)estr_sh(pr02->input_blink,1+pr02->neg_blink);
i=0;
if (vcolore) 
{
	/* se input_colore = 1 led acceso fisso */
/***
	XtSetArg(args[i],XtNcolorLamp,excolor_blink[pr02->colore].pixel); i++;
	XtSetValues(p_refr->w,args,i);  
***/
        XCopyArea(XtDisplay(cw),cw->bottone.bottone_blink,
          XtWindow(cw),cw->bottone.norm_gc,0,0,
          cw->core.width,cw->core.height,0,0);
}
else if ( vblink)
	{
	/* se solo input_blink = 1 led blinking */
/********	        XtSetArg(args[i],XtNblinkOn,1); i++;   *****/
	blink_puls(p_refr->w,pr02);
	}
else
{
/*	se entrambe a zero spento e fisso */
/***
	XtSetArg(args[i],XtNcolorLamp,excolor_new[pr02->colore].pixel); i++; 
	XtSetValues(p_refr->w,args,i);  
***/
        XCopyArea(XtDisplay(cw),cw->bottone.bottone_norm,
          XtWindow(cw),cw->bottone.norm_gc,0,0,
          cw->core.width,cw->core.height,0,0);
}
}


void blink_puls(wled,pr02)
Widget wled;
TIPO_PULS_LUCE *pr02;
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

static void pressato(w,client_data,info)
Widget w;
XtPointer client_data;
XtPointer info;
{
int i=0,iret;
TIPO_PULS_LUCE *p_r02;
float valfin ;

p_r02= (TIPO_PULS_LUCE *) client_data;
/* se l' indice del comando e'  -1 non lo esegue */

if (p_r02->out.indice == -1 ) return;
printf("\n pressato bottone modello %d indice %d tipo_pret %d",p_r02->out.nmod,
				p_r02->out.indice,p_r02->out.tipo_pert);

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

static void rilasciato(w,client_data,info)
Widget w;
XtPointer client_data;
XtPointer info;
{
int i=0,iret;
TIPO_PULS_LUCE *p_r02;
float valfin;

p_r02= (TIPO_PULS_LUCE *) client_data;
/* se l' indice del comando e'  -1 non lo esegue */

if (p_r02->out.indice == -1 ) return;
if (p_r02->out.tipo_pert == UP_DOWN)
{
printf("\n mollato bottone modello %d indice %d tipo_pret %d",p_r02->out.nmod,
                                p_r02->out.indice,p_r02->out.tipo_pert);
	valfin= -1.;
	iret=g_perturba(0,p_r02->out.tipo_pert,valfin,0.,0,p_r02->out.nmod,
							p_r02->out.indice);
}
}

