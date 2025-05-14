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
static char SccsID[] = "@(#)gindic.c	1.5\t3/23/95";
/*
   modulo gindic.c
   tipo 
   release 1.5
   data 3/23/95
   reserved @(#)gindic.c	1.5
*/
/*
	contiene le routine di gestione degli oggetti di tipo  indicatore
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/DrawingA.h>
#include "sim_param.h"
#include "xstaz.h"
#include "compstaz.h"

#include "barra1.bmp"
#include "barra2.bmp"
#include "barravd.bmp"
#include "barravs.bmp"
#include "am1.bmp"
#include "am2.bmp"
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
extern int screen_num;
extern XColor excolor_new[],excolor_blink[],sfondo_staz;
extern int font_height;
extern int font_ascent;
extern int font_width;
extern XmFontList fontlist;
extern  GC gc[];

extern DATI_REFRESH *occupa_trefr();
extern DATI_REFRESH *occupa_tredraw();
void indic_del_callback();
void indic_refresh();

float estr_sh();

GC	gc_indic[2];

#define PIGRECO 3.14159265358979323846

void crindic(istaz,wbox,p_ogg,p_r02,ltot)
int	istaz;		/* indice stazione */
Widget	wbox;		/* indice widget padre */
OGGETTO	*p_ogg;		/* puntatore oggetto in tabella new_staz */
TIPO_INDICATORE *p_r02;	/* puntatore oggetto in tabella stazioni */
int ltot;
{
int i, j, xlab, width, height, wborder, wfs;
int x[4],y[4];
Widget windic,wlab;
DATI_REFRESH *p_refr;
char aux[10];
Pixmap sfondo;
float val[4];

/* crea l'indicatore del tipo specificato nel campo sottotipo */

/*******crea_gc_indic();*******/
switch (p_ogg->sottotipo)
{
        case INDIC_BARRA1:
	crea_sfondoP(&sfondo,barra1_bits,barra1_width,barra1_height);
	width=barra1_width;
	height=barra1_height;
	wborder=0;
        break;

        case INDIC_BARRA2:
	crea_sfondoP(&sfondo,barra2_bits,barra2_width,barra2_height);
	width=barra2_width;
	height=barra2_height;
	wborder=0;
        break;

        case INDIC_BARRA_VD:
	crea_sfondoP(&sfondo,barravd_bits,barravd_width,barravd_height);
	width=barravd_width;
	height=barravd_height;
	wborder=0;
        break;

        case INDIC_BARRA_VS:
	crea_sfondoP(&sfondo,barravs_bits,barravs_width,barravs_height);
	width=barravs_width;
	height=barravs_height;
	wborder=0;
        break;

	case INDIC_AGO:
	crea_sfondoP(&sfondo,am1_bits,am1_width,am1_height);
	width=am1_width;
	height=am1_height;
	wborder=1;
	break;

        case INDIC_AGO_ERR:
        crea_sfondoP(&sfondo,am2_bits,am2_width,am2_height);
        width=am2_width;
        height=am2_height;
	wborder=1;
        break;
}


i=0;
XtSetArg(args[i],XmNx,p_ogg->x);i++;
XtSetArg(args[i],XmNy,p_ogg->y);i++;
XtSetArg(args[i],XmNwidth,width);i++;
XtSetArg(args[i],XmNheight,height);i++;
XtSetArg(args[i],XmNbackgroundPixmap,sfondo); i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNborderWidth,wborder); i++;
windic=(Widget) XmCreateDrawingArea(wbox,"draw_indic",args,i);
XtManageChild(windic);
XFreePixmap(display,sfondo);

/*
        inserisce il riferimento per l'oggetto nella tabella
        trefr_newstaz
*/

        p_refr= (DATI_REFRESH *) occupa_trefr(windic,p_r02);
        if (p_refr ==  NULL )
                exit(puts("gindic : troppi oggetti creati "));
/*
        aggiunge la callback di cancellazione dell 'oggetto
*/
XtAddCallback(windic,XmNdestroyCallback,indic_del_callback,p_refr);


/* aggiunge  i riferimenti dei fondi scala a seconda del tipo indicatore*/

switch (p_ogg->sottotipo)
{
	case INDIC_BARRA1:
		x[0]=p_ogg->x-10;
		x[1]=p_ogg->x+LARG_INDIC-15;
		y[0]=y[1]=p_ogg->y-ALTEZZA_INDIC+2;
		val[0]= (p_r02->basso_norm);
		val[1]= (p_r02->alto_norm);
		x[2]= -1;
		wfs=25;
	break;

	case INDIC_BARRA2:
                x[0]=p_ogg->x-10;
                x[1]=p_ogg->x+LARG_INDIC-15;
                x[2]=p_ogg->x-10+(LARG_INDIC- DELTA_X_BARRA2);
                y[0]=y[1]=y[2]= p_ogg->y-ALTEZZA_INDIC+2;
                val[0]= (p_r02->basso_norm);
                val[1]= (p_r02->alto_norm);
                val[2]=  p_r02->alto_norm - (float) DELTA_X_BARRA2;
		x[3]= -1;
		wfs=25;

	break;

	case INDIC_BARRA_VS:
		x[0]=p_ogg->x+2;
		x[1]=p_ogg->x+2;
		x[2]=p_ogg->x+2;
		y[0]=p_ogg->y-20+ALTEZZA_INDIC_VERT;
		y[1]=p_ogg->y+1;
		y[2]=p_ogg->y-9+ALTEZZA_INDIC_VERT/2;
		val[0]= (p_r02->basso_norm);
		val[1]= (p_r02->alto_norm);
		val[2]= (p_r02->alto_norm+p_r02->basso_norm)/2;
		x[3]= -1;
		wfs=25;
	break;

	case INDIC_BARRA_VD:
		x[0]=p_ogg->x+28;
		x[1]=p_ogg->x+28;
		x[2]=p_ogg->x+28;
		y[0]=p_ogg->y-20+ALTEZZA_INDIC_VERT;
		y[1]=p_ogg->y+1;
		y[2]=p_ogg->y-9+ALTEZZA_INDIC_VERT/2;
		val[0]= (p_r02->basso_norm);
		val[1]= (p_r02->alto_norm);
		val[2]= (p_r02->alto_norm+p_r02->basso_norm)/2;
		x[3]= -1;
		wfs=25;
	break;


	case  INDIC_AGO:
               x[0]=p_ogg->x+1;
               x[1]=p_ogg->x+LARG_INDIC-25;
               x[2]=p_ogg->x+(LARG_INDIC/2)-35;
                y[0]=p_ogg->y+ height-20;
                y[1]=p_ogg->y+8;
                y[2]=p_ogg->y + (height/2) -15;
                val[0]= (p_r02->basso_norm);
                val[1]= (p_r02->alto_norm);
                val[2]=  (p_r02->alto_norm + p_r02->basso_norm)/2;
		x[3]= -1;
		wfs=20;
	break;

        case  INDIC_AGO_ERR:
               x[0]=p_ogg->x+1;
               x[1]=p_ogg->x+LARG_INDIC-25;
               x[2]=x[0]+2;
               x[3]=x[0]+35;
                y[0]=p_ogg->y+ height-20;
                y[1]=p_ogg->y+15;
                y[2]=y[0]-18;
                y[3]=y[1]+3;
                val[0]= (p_r02->basso_norm);
                val[1]= (p_r02->alto_norm);
                val[2]= (p_r02->basso_err);
                val[3]= (p_r02->alto_err);
		wfs=20;
        break;

}

/* segna fondo scala basso se e' definito  */

for (j=0; j<4 && x[j] != -1; j++)
{ 
	sprintf(aux,"%3d",(int) val[j]);
	if ((p_ogg->sottotipo == INDIC_BARRA1 || 
	     p_ogg->sottotipo == INDIC_BARRA2) && x[j+1] == -1) strcat(aux,"%");
	i=0;
	XtSetArg(args[i],XmNx,x[j]);i++;
	XtSetArg(args[i],XmNy,y[j]);i++;
	XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(aux,XmSTRING_DEFAULT_CHARSET)); i++;
	XtSetArg(args[i],XmNwidth,wfs); i++;
	XtSetArg(args[i],XmNfontList,fontlist); i++;
	XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
	XtSetArg(args[i],XmNalignment,XmALIGNMENT_END); i++;
	wlab=(Widget) XmCreateLabel(wbox,"lab",args,i);
	XtManageChild(wlab);

}

/*
	aggiunge la callbacks di expose dell' indicatore e sfrutta il campo
	flag della struttuta indicata da prefr per specificare il sottotipo 
*/
p_refr->flag = p_ogg->sottotipo;
/*
        aggiunge la callbacks  di refresh 
*/

        if(add_refresh((XtCallbackProc)indic_refresh,(caddr_t)p_refr)==-1)
        {
                libera_trefr(p_refr);
                printf("\n errore : refresh non aggiunto");
        }
}


void indic_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
del_refresh((void*)info);
libera_trefr((DATI_REFRESH *)info);
}


void indic_refresh(info)
caddr_t info;
{
DATI_REFRESH *p_refr;
TIPO_INDICATORE *p_r02;
float valore,valerr;

p_refr=(DATI_REFRESH *) info;
p_r02= (TIPO_INDICATORE *) p_refr->pdati;

XClearArea(display,XtWindow(p_refr->w),0,0,0,0,False);

/* raccoglie il valore della variabile e la moltiplica per il fattore 
   di scalamento  */

valore = estr_sh(p_r02->input,0);
valore = valore * p_r02->scalam;
valore = valore + p_r02->offset;

switch (p_refr->flag)
{
        case    INDIC_BARRA1:
        case    INDIC_BARRA2:
               draw_barra(p_refr->w,valore,p_r02);
	break;

        case    INDIC_BARRA_VD:
        case    INDIC_BARRA_VS:
               draw_barra_or(p_refr->w,valore,p_r02,p_refr->flag);
	break;

	case	INDIC_AGO:
		draw_ago(p_refr->w,valore,p_r02);
	break;

        case    INDIC_AGO_ERR:
		valerr = estr_sh(p_r02->input_err,0);
		valerr = valerr * p_r02->scalam_err;
                draw_ago_err(p_refr->w,valore,valerr,p_r02);
        break;

}

}

/* draw per indicatore a barra */

void draw_barra(w,valore,p_r02)
Widget w;
float valore;
TIPO_INDICATORE *p_r02;
{
int icolor,x1,x2,y1,y2;
int pos_ago;
float delta;

/* esamina se si e' raggiunto il fondo scala */

delta=0.05*(p_r02->alto_norm - p_r02->basso_norm);
/********
if(valore<(p_r02->basso_norm+delta) || valore>(p_r02->alto_norm-delta))
	icolor =4;
else 
	icolor=2;
********/
icolor=2;  /* ago sempre rosso */

/* calcolo posizione ago */

pos_ago=LARG_INDIC*((valore - p_r02->basso_norm)/(p_r02->alto_norm- p_r02->basso_norm));
if(pos_ago<= 0)  pos_ago=3;
if(pos_ago>= LARG_INDIC) pos_ago=LARG_INDIC-3;

x1=x2=pos_ago;
y1=23;
y2=0;

XDrawLine(display,XtWindow(w),gc[icolor],x1,y1,x2,y2);
/*XDrawLine(display,XtWindow(w),gc_indic[icolor],x1,y1,x2,y2);*/

}

/* draw per indicatore a barra verticale (disegna barra orizzontale) */

void draw_barra_or(w,valore,p_r02,flag)
Widget w;
float valore;
TIPO_INDICATORE *p_r02;
int flag;
{
int icolor,x1,x2,y1,y2;
int pos_ago;

icolor=2;  /* ago sempre rosso */

/* calcolo posizione ago */

pos_ago=ALTEZZA_INDIC_VERT-ALTEZZA_INDIC_VERT*((valore - p_r02->basso_norm)/(p_r02->alto_norm- p_r02->basso_norm));
if(pos_ago<= 0)  pos_ago=3;
if(pos_ago>= ALTEZZA_INDIC_VERT) pos_ago=ALTEZZA_INDIC_VERT-3;

y1=y2=pos_ago;
if(flag==INDIC_BARRA_VD)
	{
	x1=6;
	x2=38;
	}
else
	{
	x1=27;
	x2=54;
	}

XDrawLine(display,XtWindow(w),gc[icolor],x1,y1,x2,y2);
}

/* draw ago per indicatore analogico */
void draw_ago(w,valore,p_r02)
Widget w;
float valore;
TIPO_INDICATORE *p_r02;
{
int icolor,x1,x2,y1,y2;
int pos_ago;
float delta;
double dx,dy;
double dpos_ago;

delta=0.05*(p_r02->alto_norm - p_r02->basso_norm);
/*******************************************
if(valore<(p_r02->basso_norm+delta) || valore>(p_r02->alto_norm-delta))
        icolor=4;
else
        icolor=2;
********************************************/
icolor=2;                   /* ago sempre rosso */

/* calcolo posizione ago */

pos_ago=90*((valore - p_r02->basso_norm)/(p_r02->alto_norm- p_r02->basso_norm));
if(pos_ago<= 0)  pos_ago=0;
if(pos_ago>= 90) pos_ago=90;

x1=90;
y1=90;

dpos_ago=pos_ago;
dpos_ago=(dpos_ago/90.0*(PIGRECO/2));
dx=cos(dpos_ago);
dy=sin(dpos_ago);
x2=(int)((float)90.0-65.0*dx);
y2=(int)((float)90.0-65.0*dy);

XDrawLine(display,XtWindow(w),gc[icolor],x1,y1,x2,y2);

}

/* draw ago per indicatore analogico  piu' indicatore di errore */
void draw_ago_err(w,valore,valerr,p_r02)
Widget w;
float valore,valerr;
TIPO_INDICATORE *p_r02;
{
int icolor,x1,x2,y1,y2;
int pos_ago;
float delta;
double dx,dy;
double dpos_ago;

/* calcolo posizione ago misura */

delta=0.05*(p_r02->alto_norm - p_r02->basso_norm);
/***********************************************
if(valore<(p_r02->basso_norm+delta) || valore>(p_r02->alto_norm-delta))
        icolor=4;
else
        icolor=2;
*******************************************************/
icolor=2;		/* ago sempre rosso */
pos_ago=90*((valore - p_r02->basso_norm)/(p_r02->alto_norm- p_r02->basso_norm));
if(pos_ago<= 0)  pos_ago=0;
if(pos_ago>= 90) pos_ago=90;
x1=90;
y1=90;
dpos_ago=pos_ago;
dpos_ago=(dpos_ago/90.0*(PIGRECO/2));
dx=cos(dpos_ago);
dy=sin(dpos_ago);
x2=(int)((float)90.0-40.0*dx);
y2=(int)((float)90.0-40.0*dy);
XDrawLine(display,XtWindow(w),gc[icolor],x1,y1,x2,y2);

/* calcolo posizione ago errore */

delta=0.05*(p_r02->alto_err - p_r02->basso_err);
/*************************************************
if(valerr<(p_r02->basso_err+delta) || valerr>(p_r02->alto_err-delta))
        icolor=4;
else
        icolor=2;
******************************************************************/

icolor=2;		/* ago sempre rosso */
pos_ago=90*((valerr - p_r02->basso_err)/(p_r02->alto_err- p_r02->basso_err));
if(pos_ago< 22)  pos_ago=22;
if(pos_ago> 67) pos_ago=67;
dpos_ago=pos_ago;
dpos_ago=(dpos_ago/90.0*(PIGRECO/2));
dx=cos(dpos_ago);
dy=sin(dpos_ago);
x1=(int)((float)90.0-50.0*dx);
y1=(int)((float)90.0-50.0*dy);
x2=(int)((float)90.0-65.0*dx);
y2=(int)((float)90.0-65.0*dy);

XDrawLine(display,XtWindow(w),gc[icolor],x1,y1,x2,y2);

}

void crea_gc_indic()
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCCapStyle | GCJoinStyle |
                           GCFunction;

values.foreground = excolor_new[C_ROSSO].pixel;
values.background = WhitePixel(display,screen_num);
values.line_width = 2;
values.line_style = LineSolid;
gc_indic[0]= XCreateGC(display,RootWindow(display,screen_num), valuemask,
&values);

values.foreground = excolor_new[GRIGIO].pixel;
values.background = WhitePixel(display,screen_num);
values.line_width = 2;
values.line_style = LineSolid;
gc_indic[1]= XCreateGC(display,RootWindow(display,screen_num), valuemask,
&values);
}





