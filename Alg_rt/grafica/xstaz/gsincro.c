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
static char SccsID[] = "@(#)gsincro.c	1.2\t3/23/95";
/*
   modulo gsincro.c
   tipo 
   release 1.2
   data 3/23/95
   reserved @(#)gsincro.c	1.2
*/
/*
	contiene le routine di gestione degli oggetti di tipo  indicatore sincro
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

#include "sincro.bmp"
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
extern int id_msg_pert;
void sincro_del_callback();
void sincro_refresh();

float estr_sh();

GC	gc_sincro[2];

#define PIGRECO 3.14159265358979323846

void crsincro(istaz,wbox,p_ogg,p_r02,ltot)
int	istaz;		/* indice stazione */
Widget	wbox;		/* indice widget padre */
OGGETTO	*p_ogg;		/* puntatore oggetto in tabella new_staz */
TIPO_INDICATORE_SINCRO *p_r02;	/* puntatore oggetto in tabella stazioni */
int ltot;
{
int i, j, xlab, width, height, wborder, wfs;
int x[4],y[4];
Widget wsincro,wlab;
DATI_REFRESH *p_refr;
char aux[10];
Pixmap sfondo;
float val[4];

/* crea l'indicatore del tipo specificato nel campo sottotipo */

crea_sfondoP(&sfondo,sincro_bits,sincro_width,sincro_height);
width=sincro_width;
height=sincro_height;
wborder=0;


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
wsincro=(Widget) XmCreateDrawingArea(wbox,"draw_sincro",args,i);
XtManageChild(wsincro);
XFreePixmap(display,sfondo);

/*
        inserisce il riferimento per l'oggetto nella tabella
        trefr_newstaz
*/

        p_refr= (DATI_REFRESH *) occupa_trefr(wsincro,p_r02);
        if (p_refr ==  NULL )
                exit(puts("gsincro : troppi oggetti creati "));
/*
        aggiunge la callback di cancellazione dell 'oggetto
*/
XtAddCallback(wsincro,XmNdestroyCallback,sincro_del_callback,p_refr);


/* aggiunge  i riferimenti dei fondi scala a seconda del tipo indicatore*/

x[0]= -1;
wfs=25;


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

        if(add_refresh((XtCallbackProc)sincro_refresh,(caddr_t)p_refr)==-1)
        {
                libera_trefr(p_refr);
                printf("\n errore : refresh non aggiunto");
        }
}


void sincro_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
del_refresh((void*)info);
libera_trefr((DATI_REFRESH *)info);
}


void sincro_refresh(info)
caddr_t info;
{
DATI_REFRESH *p_refr;
TIPO_INDICATORE_SINCRO *p_r02;
float valore_1,valore_2,valore_3,valore_4,valore_5,valore_6,valore,val_rad;
static float old_valore=0;
static int old_parallelo_ok= -1;
int iret, parallelo_ok;
float valfin;


p_refr=(DATI_REFRESH *) info;
p_r02= (TIPO_INDICATORE_SINCRO *) p_refr->pdati;

XClearArea(display,XtWindow(p_refr->w),0,0,0,0,False);

/* raccoglie il valore della variabile e la moltiplica per il fattore 
   di scalamento  */

valore_1 = estr_sh(p_r02->input_1,0);
valore_2 = estr_sh(p_r02->input_2,0);
valore_3 = estr_sh(p_r02->input_3,0);
valore_4 = estr_sh(p_r02->input_4,0);
valore_5 = estr_sh(p_r02->input_5,0);
valore_6 = estr_sh(p_r02->input_6,0);

if(valore_5!=0)
   {
   valore = old_valore + (valore_2-valore_1)*SINCRO_TIME_AGG*360/1000;
   old_valore=valore;
   draw_ago_sincro(p_refr->w,valore,p_r02);
   /*
	verifica il parallelo
   */
   val_rad =(valore/90.0*(PIGRECO/2));
   if((cos(val_rad)>cos(SINCRO_DELTA_FASE*PIGRECO/180))&&
	(fabs(valore_2-valore_1)<SINCRO_DELTA_F)&&(fabs(valore_4-valore_3)<SINCRO_DELTA_V))
	parallelo_ok=1;
   else
	parallelo_ok=0;
   if(parallelo_ok!=old_parallelo_ok)
	{
	if(parallelo_ok==1)
   		iret=pert_up(id_msg_pert,p_r02->out_1.indice,0.);
	else
   		iret=pert_down(id_msg_pert,p_r02->out_1.indice,0.);
	old_parallelo_ok=parallelo_ok;
	}
   if((valore_6 == 1)&&(estr_sh(p_r02->out_2.indice) != (float)parallelo_ok))
	iret=pert_scalino(id_msg_pert,p_r02->out_2.indice,(float)parallelo_ok,0.);
   }


}


/* draw ago per indicatore analogico per sincronoscopio*/
void draw_ago_sincro(w,valore,p_r02)
Widget w;
float valore;
TIPO_INDICATORE_SINCRO *p_r02;
{
int icolor,x1,x2,y1,y2;
int pos_ago;
float delta;
double dx,dy;
double dpos_ago;


icolor=2;                   /* ago sempre rosso */

/* calcolo posizione ago */

pos_ago=(valore);






dpos_ago=pos_ago+90;
dpos_ago=(dpos_ago/90.0*(PIGRECO/2));
dx=cos(dpos_ago);
dy=sin(dpos_ago);
x1=(int)((float)(12)*dx)+R_INDICATORE_SINCRO;
y1=R_INDICATORE_SINCRO-(int)((float)(12)*dy);
x2=(int)((float)(R_INDICATORE_SINCRO-2)*dx)+R_INDICATORE_SINCRO;
y2=R_INDICATORE_SINCRO-(int)((float)(R_INDICATORE_SINCRO-2)*dy);

XDrawLine(display,XtWindow(w),gc[icolor],x1,y1,x2,y2);

}

void crea_gc_sincro()
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCCapStyle | GCJoinStyle |
                           GCFunction;

values.foreground = excolor_new[C_ROSSO].pixel;
values.background = WhitePixel(display,screen_num);
values.line_width = 2;
values.line_style = LineSolid;
gc_sincro[0]= XCreateGC(display,RootWindow(display,screen_num), valuemask,
&values);

values.foreground = excolor_new[GRIGIO].pixel;
values.background = WhitePixel(display,screen_num);
values.line_width = 2;
values.line_style = LineSolid;
gc_sincro[1]= XCreateGC(display,RootWindow(display,screen_num), valuemask,
&values);
}





