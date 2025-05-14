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
static char SccsID[] = "@(#)cnewstaz.c	1.7\t3/23/95";
/*
   modulo cnewstaz.c
   tipo 
   release 1.7
   data 3/23/95
   reserved @(#)cnewstaz.c	1.7
*/
/*
	Creazione stazioni di tipo nuovo 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <X11/Xlib.h>
#include <Xm/DrawingA.h>
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
extern DATI_REFRESH *trefr_newstaz;
extern DATI_REFRESH *tredraw_newstaz;
extern HEAD_R02 header;

extern XColor excolor_new[],excolor_blink[],sfondo_staz;
extern int font_height;
extern int font_ascent;
extern int font_width;


void crea_new_staz(flag,it,is,ip3)
int flag;   /* flag */
int it;	    /* tipo stazione indice tabella new_staz */
int is;   /* indice stazione associata */
int ip3;  /* indice nel descrittore pagine visualizzate */

{
Widget wbox,wsep;
int	htot, ltot, istaz, itipo, xdraw, ydraw;
int	i,j,lun;
char  *p_r02;

/* Calcola le dimensioni della stazione */

/*printf("\n tipo stazione  %d indice %d pagina %d \n",it,is,ip3);*/
istaz =is - 1;
itipo = it -  MINIMO_INDICE_NEW_STAZ;
htot=(stazione[istaz].posiy1-stazione[istaz].posiy0)*DIM_UNITSTAZ;
ltot=(stazione[istaz].posix1-stazione[istaz].posix0)*DIM_UNITSTAZ;

xdraw=(stazione[istaz].posix0*DIM_UNITSTAZ);
ydraw=(stazione[istaz].posiy0*DIM_UNITSTAZ);
ydraw=pagvis[ip3].height-ydraw-htot;

/*  Crea il contenitore in posizione e di dimensioni prefissate*/


i=0;
XtSetArg(args[i],XmNx,xdraw);i++;
XtSetArg(args[i],XmNy,ydraw);i++;
XtSetArg(args[i],XmNwidth,ltot-2);i++;
XtSetArg(args[i],XmNheight,htot-2);i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
XtSetArg(args[i],XmNborderWidth,2);i++;
XtSetArg(args[i],XmNmarginWidth,0);i++;
XtSetArg(args[i],XmNmarginHeight,0);i++;
XtSetArg(args[i],XmNresizePolicy,XmRESIZE_NONE);i++;
wbox=XmCreateDrawingArea(pagvis[ip3].w,"box",args,i);
XtManageChild(wbox);


/***********
i=0;
XtSetArg(args[i],XmNx,xdraw);i++;
XtSetArg(args[i],XmNy,ydraw+htot);i++;
XtSetArg(args[i],XmNwidth,ltot);i++;
wsep= XmCreateSeparator(wbox,"sep",args,i);
**********/

/* dato il tipo di stazione, accede alla tabella new_staz[tipo]
   e crea gli oggetti definiti in tabella */

p_r02 = (char *) stazione[istaz].descrizione;
for (i=0; i< new_staz[itipo].num_oggetti; i++)
{
	switch (new_staz[itipo].ogg[i].tipo)
	{
	   case LED:

		crled(istaz,wbox,&new_staz[itipo].ogg[i],(TIPO_LED *)p_r02);
		p_r02+=sizeof (TIPO_LED);
	   break;

	   case PULS_LUCE:
		crpulsluce(istaz,wbox,&new_staz[itipo].ogg[i],(TIPO_PULS_LUCE *)p_r02);
		p_r02+=sizeof (TIPO_PULS_LUCE);
	   break;

	  case  STRINGA_DESCR:
		crstringa(istaz,wbox,&new_staz[itipo].ogg[i],(TIPO_STRINGA_DESCR *)p_r02);
		p_r02+=sizeof (TIPO_STRINGA_DESCR);
	  break;

           case LAMPADA:
                crlampada(istaz,wbox,&new_staz[itipo].ogg[i],(TIPO_LAMPADA *)p_r02);
                p_r02+=sizeof (TIPO_LAMPADA);
           break;

           case PULSANTE:
                crpulsante(istaz,wbox,&new_staz[itipo].ogg[i],(TIPO_PULS *)p_r02);
                p_r02+=sizeof (TIPO_PULS);
           break;

           case SELETTORE:
                crselet(istaz,wbox,&new_staz[itipo].ogg[i],(TIPO_SELETTORE *)p_r02,ltot);
                p_r02+=sizeof (TIPO_SELETTORE);
           break;

           case INDICATORE:
                crindic(istaz,wbox,&new_staz[itipo].ogg[i],(TIPO_INDICATORE *)p_r02,ltot);
                p_r02+=sizeof (TIPO_INDICATORE);
           break;

           case DISPLAY:
                crdisplay(istaz,wbox,&new_staz[itipo].ogg[i],(TIPO_DISPLAY *)p_r02);
                p_r02+=sizeof (TIPO_DISPLAY);
	   break;

           case LUCE:
                crluce(istaz,wbox,&new_staz[itipo].ogg[i],(TIPO_LUCE *)p_r02);
                p_r02+=sizeof (TIPO_LUCE);
           break;

           case TASTO:
                crtasto(istaz,wbox,&new_staz[itipo].ogg[i],(TIPO_TASTO *)p_r02);
                p_r02+=sizeof (TIPO_TASTO);
           break;

/* per AIX bisogna installare motif vers 2.0 perche' la versione attuale
   non gestisce la XmNchild */
           case SET_VALORE:
                crsetval(istaz,wbox,&new_staz[itipo].ogg[i],(TIPO_SET_VALORE *)p_r02);
                p_r02+=sizeof (TIPO_SET_VALORE);
           break;

	   case DISPLAY_SCALATO:
                crdisplaysc(istaz,wbox,&new_staz[itipo].ogg[i],(TIPO_DISPLAY_SCALATO *)p_r02);
                p_r02+=sizeof (TIPO_DISPLAY_SCALATO);
	   break;

	   case INDICATORE_SINCRO:
                crsincro(istaz,wbox,&new_staz[itipo].ogg[i],(TIPO_INDICATORE_SINCRO *)p_r02,ltot);
                p_r02+=sizeof (TIPO_INDICATORE_SINCRO);
           break;
                                                         
	}
}


}

/*  routine che aggiungono un record alla tabella trefr_newstaz o
	che lo eliminano
*/

DATI_REFRESH *occupa_trefr(wogg,ps)
Widget wogg;		/* widget nuovo oggetto creato */
char *ps;		/* punatore all' oggetto nella struttura stazioni */
{
DATI_REFRESH *p_ret;
int i;

p_ret = (DATI_REFRESH *) &trefr_newstaz[0].w;
for (i=0; i< header.tot_staz*MAX_WIDGET_STAZ; i++, p_ret++)
{
	if (p_ret->w != 0 ) continue;
	p_ret->w = wogg;
	p_ret->pdati=ps;
	return(p_ret);
}
p_ret=NULL;
return(p_ret);
}

int libera_trefr(p_refr)
DATI_REFRESH *p_refr;
{

if (p_refr->w == 0 ) return(-1);
p_refr->w = 0;
p_refr->pdati = NULL;
return(0);
}

/*  routine che aggiungono un record alla tabella tredraw o
        che lo eliminano
*/

DATI_REFRESH *occupa_tredraw(wogg,ps)
Widget wogg;            /* widget nuovo oggetto creato */
char *ps;               /* punatore all' oggetto nella struttura
stazioni */
{
DATI_REFRESH *p_ret;
int i;

p_ret = (DATI_REFRESH *) &tredraw_newstaz[0].w;
for (i=0; i<  header.tot_staz*MAX_WIDGET_STAZ; i++, p_ret++)
{
        if (p_ret->w != 0 ) continue;
        p_ret->w = wogg;
        p_ret->pdati=ps;
        return(p_ret);
}
p_ret=NULL;
return(p_ret);
}

int libera_tredraw(p_refr)
DATI_REFRESH *p_refr;
{

if (p_refr->w == 0 ) return(-1);
p_refr->w = 0;
p_refr->pdati = NULL;
return(0);
}
