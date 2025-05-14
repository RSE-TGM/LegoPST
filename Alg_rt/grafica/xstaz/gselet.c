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
static char SccsID[] = "@(#)gselet.c	1.5\t3/23/95";
/*
   modulo gselet.c
   tipo 
   release 1.5
   data 3/23/95
   reserved @(#)gselet.c	1.5
*/
/*
	contiene le routine di gestione degli oggetti di tipo selettore
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

#include "Selettore.h"
#include "sele_a_0.bmp"
#include "sele_a_1.bmp"
#include "sele_b_0.bmp"
#include "sele_b_1.bmp"
/*
  tabelle comuni utilizzate per tutte le stazioni
*/
extern Arg args[];
extern S_PAGINA *pagina;
extern S_STAZIONE *stazione;
extern S_PAGVIS pagvis[MAX3_PAG];
extern TIPI_NEWSTAZ new_staz[];
extern DATI_REFRESH trefr_newstaz[];
extern DATI_REFRESH tredraw_newstaz[];


extern Display *display;
extern XColor excolor_new[],excolor_blink[],sfondo_staz;
extern int font_height;
extern int font_ascent;
extern int font_width;
extern XmFontList fontlist;

extern DATI_REFRESH *occupa_trefr();
extern DATI_REFRESH *occupa_tredraw();
void selet_del_callback();
static void press_selet();
void selet_redraw();

float estr_sh();

Pixmap pixmap_0,pixmap_1;

#if defined  MOTIF10
typedef char*  XtPointer;
#endif

void crselet(istaz,wbox,p_ogg,p_r02,ltot)
int	istaz;		/* indice stazione */
Widget	wbox;		/* indice widget padre */
OGGETTO	*p_ogg;		/* puntatore oggetto in tabella new_staz */
TIPO_SELETTORE *p_r02;	/* puntatore oggetto in tabella stazioni */
int ltot;
{
int i,  xlab,vstato;
Widget wselet,wlab;
DATI_REFRESH *p_redraw;
char app[10];

/* crea il selet */

/*printf("\n  oggetto selettore, variabile di input %d %s %s ",p_r02->input,p_r02->etic_0,p_r02->etic_1);*/
LoadPixmapSelet(wbox,p_ogg->sottotipo);

/* raccoglie il valore della variabile di INPUT  che definisce se stato
   iniziale del selettore 0 o 1 */

vstato = (int)estr_sh(p_r02->input,0);
i=0;
XtSetArg(args[i],XmNx,p_ogg->x);i++;
XtSetArg(args[i],XmNy,p_ogg->y);i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel);i++;
XtSetArg(args[i],XmNwidth,23);i++;
XtSetArg(args[i],XmNheight,23);i++;
XtSetArg(args[i],XtNpixmap0,pixmap_0); i++;
XtSetArg(args[i],XtNpixmap1,pixmap_1); i++;
XtSetArg(args[i],XtNstatoSel,vstato); i++;
XtSetArg(args[i],XtNseleBg,sfondo_staz.pixel ); i++;
wselet=XtCreateManagedWidget("sele",selettoreWidgetClass,wbox,args,i);
/*
	aggiunge le callbacks di presa e rilascio del selettore
*/
XtAddCallback(wselet,XtNpressSelCallback,press_selet,p_r02);

/* libera i pixmap */

XFreePixmap(XtDisplay(wbox),pixmap_0);
XFreePixmap(XtDisplay(wbox),pixmap_1);

/*
        inserisce il riferimento per l'oggetto nella tabella
        di redraw tredraw_newstaz
*/

        p_redraw=(DATI_REFRESH *) occupa_tredraw(wselet,p_r02);
        if (p_redraw ==  NULL )
                exit(puts("gselet : troppi oggetti creati "));
/*
        aggiunge la callback di cancellazione dell 'oggetto
*/
XtAddCallback(wselet,XmNdestroyCallback,selet_del_callback,p_redraw);
                                                                
/* aggiunge  le descrizioni degli stati  del selettore*/


i=0;
xlab=(ltot-10)/2;
XtSetArg(args[i],XmNx,5);i++;
XtSetArg(args[i],XmNy,p_ogg->y-20);i++;
XtSetArg(args[i],XmNfontList,fontlist);i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(p_r02->etic_0,XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNwidth,xlab); i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
wlab=(Widget) XmCreateLabel(wbox,"lab",args,i);
XtManageChild(wlab);

i=0;
XtSetArg(args[i],XmNx,5+xlab);i++;
XtSetArg(args[i],XmNy,p_ogg->y-20);i++;
XtSetArg(args[i],XmNfontList,fontlist);i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(p_r02->etic_1,XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNwidth,xlab); i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
wlab=(Widget) XmCreateLabel(wbox,"lab",args,i);
XtManageChild(wlab);

/*
        aggiunge la callbacks  di redraw del selettore
*/

        if(add_redraw((XtCallbackProc)selet_redraw,(caddr_t)p_redraw)==-1)
        {
                libera_tredraw(p_redraw);
                printf("\n errore : redraw non aggiunto");
        }

}

void selet_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
del_redraw(info);
libera_tredraw((DATI_REFRESH *)info);
}

/* vengono inviate solo se perturbazioni di tipo UP-DOWN */
static void press_selet(w,client_data,info)
Widget w;
XtPointer client_data;
XtPointer info;
{
int i=0,iret;
TIPO_SELETTORE *p_r02;
float valfin ;

p_r02=(TIPO_SELETTORE *) client_data;
/* se l' indice del comando e'  -1 non lo esegue */

if (p_r02->out.indice == -1 ) return;
/*printf("\n press selettore su variabile %d",p_r02->input);*/
if (p_r02->out.tipo_pert == NEGAZIONE)
{
        iret=g_perturba(0,p_r02->out.tipo_pert,valfin,0.,0,p_r02->out.nmod,
                                                        p_r02->out.indice);
}
}


/*  crea i 2 pixmap per il selettore del tipo  A o B */
void LoadPixmapSelet(w,tipo_selet)
Widget w;
int tipo_selet;
{
int height,width;
char *bits;
Display *displ;

displ=XtDisplay(w);
if (tipo_selet  == SELET_TIPO_A)
{
	width=sele_a_0_width;
	height=sele_a_0_height;
	bits=sele_a_0_bits;
}
else
{
        width=sele_b_0_width;
        height=sele_b_0_height;
        bits=sele_b_0_bits;
}

if((pixmap_0= XCreateBitmapFromData(displ,
			RootWindow(displ,DefaultScreen(displ)),
			bits,width,height))== False)
        {
        printf("\n Errore creazione pixmap per sfondo");
        exit(1);
        }


if (tipo_selet  == SELET_TIPO_A)
{
        width=sele_a_1_width;
        height=sele_a_1_height;
        bits=sele_a_1_bits;
}
else
{
        width=sele_b_1_width;
        height=sele_b_1_height;
        bits=sele_b_1_bits;
}
if((pixmap_1= XCreateBitmapFromData(displ,
                        RootWindow(displ,DefaultScreen(displ)),
                        bits,width,height))== False)
        {
        printf("\n Errore creazione pixmap per sfondo");
        exit(1);
        }
}

void selet_redraw(info)
caddr_t info;
{
DATI_REFRESH *p_redraw;
TIPO_SELETTORE *p_r02;
int vstato,i;

p_redraw=(DATI_REFRESH *) info;
p_r02= (TIPO_SELETTORE *) p_redraw->pdati;

/* raccoglie il valore della variabile di INPUT  che definisce se stato
   iniziale del selettore 0 o 1 */

vstato = (int)estr_sh(p_r02->input,0);
i=0;
XtSetArg(args[i],XtNstatoSel,vstato); i++;
XtSetValues(p_redraw->w,args,i);
}
