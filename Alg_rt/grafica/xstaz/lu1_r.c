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
static char SccsID[] = "@(#)lu1_r.c	1.4\t3/23/95";
/*
   modulo lu1_r.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)lu1_r.c	1.4
*/
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/Label.h>
#include <Xm/Separator.h>
#include "sim_param.h"
#include "xstaz.h"
#include "compstaz.h"

/*
  tabelle comuni utilizzate per tutte le stazioni
*/
extern Arg args[];
extern S_PAGINA *pagina;
extern S_STAZIONE *stazione;
extern S_WSTAZIONE *wstaz;
extern S_PAGVIS pagvis[MAX3_PAG];
/*
  variabili globali utilizzate per la grafica
 */
XFontStruct *font_info;
XFontStruct font;
extern Display *display;

extern XColor excolor[],excolor_new[],sfondo_label,sfondo_staz;
extern int font_height;
extern int font_ascent;
extern int font_width;
extern GC gc[4];
extern XmFontList fontlist;
/*
 parametri geometrici propri della stazione LU1: vengono utilizzati
 nelle callback di redraw.
*/
int lu1_l;       /* larghezza della stazione */
int lu1_draw_h;  /* altezza dell'area di visualizzazione strumento
                    analogico  */

/*
  indici all'interno della struttura dei widget 
*/
#define k_toggle1  0
#define k_toggle2  1
#define k_toggle3  2
#define k_text     3
#define k_draw1    4

static void lu1_del_callback();
static void lu1_draw2_callback(); /* callback della area di disegno testi */
static void lu1_refresh();        /* callback da chiamarsi scaduto il
                                      timeout di refresh */

float estr_sh();
void staz_lu1_r(flag,is,ip3)
int *flag;   /* flag */
int *is;   /* indice stazione associata */
int *ip3;  /* indice nel descrittore pagine visualizzate */
{
char app[100];
int val;
int i;
Widget wDraw,wText,wSep,wLab,wRow1; /* area per disegno */
Widget wRadioBox,wToggle;
Widget wToggle1,wToggle3;
int htot; /* altezza totale */
int xdraw,ydraw;       /* altezza e posizione area di disegno */
int hlabel,xlabel,ylabel;    /* altezza e posizione area testo */
int hbutton,xbutton,ybutton; /* altezzza area bottoni */
int ltot;    /* lunghezza totale */
/*
 Calcola i parametri geometrici della stazione
 */
htot=(stazione[*is-1].posiy1-stazione[*is-1].posiy0)*60;
ltot=(stazione[*is-1].posix1-stazione[*is-1].posix0)*60;
hbutton=2*htot/3;
hlabel=htot/3;
lu1_l=ltot-2;
xdraw=stazione[*is-1].posix0*62+2;
ydraw=stazione[*is-1].posiy0*62+3;
ydraw=pagvis[*ip3].height-ydraw-htot;
xlabel=xdraw;
ylabel=ydraw;
xbutton=xdraw;
ybutton=hlabel;

i=0;
XtSetArg(args[i],XmNx,xdraw); i++;
XtSetArg(args[i],XmNy,ydraw); i++;
XtSetArg(args[i],XmNwidth,ltot);i++;
XtSetArg(args[i],XmNheight,htot);i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
XtSetArg(args[i],XmNborderWidth,2);i++;
XtSetArg(args[i],XmNmarginWidth,0);i++;
XtSetArg(args[i],XmNmarginHeight,0);i++;
XtSetArg(args[i],XmNresizePolicy,XmRESIZE_NONE);i++;
wDraw=XmCreateDrawingArea(pagvis[*ip3].w,"box",args,i);
XtManageChild(wDraw);
XtAddCallback(wDraw,XmNdestroyCallback,lu1_del_callback,(XtPointer)*is-1);


/*
 crea area delle label
 */

memset(app,' ',17);
memcpy(app,stazione[*is-1].descrizione,strlen((char*)stazione[*is-1].descrizione));
app[17]=0;
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,0); i++;
XtSetArg(args[i],XmNwidth,ltot-1); i++;
XtSetArg(args[i],XmNheight,hlabel); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNborderWidth,1); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
wLab=XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);



i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hlabel); i++;
XtSetArg(args[i],XmNwidth,ltot-1); i++;
XtSetArg(args[i],XmNheight,hbutton); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNborderWidth,1); i++;
wRadioBox=XmCreateDrawingArea(wDraw,"label",args,i);
XtManageChild(wRadioBox);

i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hbutton); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNtopShadowColor,excolor[0].pixel); i++;
XtSetArg(args[i],XmNbottomShadowColor,excolor[0].pixel); i++;
wSep=XmCreateSeparator(wRadioBox,"sep",args,i);
XtManageChild(wSep);


i=0;
memcpy(app,stazione[*is-1].etic[0],4);
app[4]=0;
XtSetArg(args[i],XmNx,(lu1_l-font_width*6)/2); i++;
XtSetArg(args[i],XmNy,10); i++;
XtSetArg(args[i],XmNwidth,font_width*6); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
wToggle1=XmCreateLabel(wRadioBox,"push",args,i);
wstaz[*is-1].w[k_toggle1]=wToggle1;
XtManageChild(wToggle1);
/*
 Aggiunge una routine di refresh alla stazione creata
 */
if(add_refresh((XtCallbackProc)lu1_refresh,(void*)*is-1)==-1)
	printf("\n errore : refresh non aggiunto");

}

static void lu1_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int is= (int)info;
del_refresh((void*)is);
}
       

static void lu1_draw2_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int i;
char app[50]; /* stringa di appoggio */
int is= (int)info;
Window win = str->window;

memcpy(app,stazione[is].descrizione[0],17);
stazione[is].descrizione[0][17]=0;
app[17]=0;
XDrawString(display,win,gc[0],5,font_ascent,
            app,strlen(app));

}


/*
 Pseudo-Callback di refresh della stazione
 */

static void lu1_refresh(info)
caddr_t info;
{
int is;
/* variabili raccolte dall'area di memoria shared */
float valore;
int i, ival;

is=(int)info;
/*
 raccoglie i valori delle variabili dall'area di memoria shared
 */
ival= (int) estr_sh(stazione[is].valore,1);

/*
 esamina lo stato in cui si trova la stazione 
 */
if(ival)
	{
	i=0;
        XtSetArg(args[i],XmNforeground,excolor[0].pixel); i++;
        XtSetArg(args[i],XmNbackground,excolor[2].pixel); i++;
        XtSetValues(wstaz[is].w[k_toggle1],args,i);
	}
else
	{
	i=0;
        XtSetArg(args[i],XmNforeground,excolor[0].pixel); i++;
        XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
        XtSetValues(wstaz[is].w[k_toggle1],args,i);
	}
}





