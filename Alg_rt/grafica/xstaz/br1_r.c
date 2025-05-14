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
static char SccsID[] = "@(#)br1_r.c	1.4\t3/23/95";
/*
   modulo br1_r.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)br1_r.c	1.4
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
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
 parametri geometrici propri della stazione BR1: vengono utilizzati
 nelle callback di redraw.
*/
int br1_l;       /* larghezza della stazione */
int br1_draw_h;  /* altezza dell'area di visualizzazione strumento
                    analogico  */

/*
  indici all'interno della struttura dei widget 
*/
#define k_toggle1  0
#define k_toggle2  1
#define k_toggle3  2
#define k_text     3
#define k_draw1    4

static void br1_del_callback();
static void br1_draw2_callback(); /* callback della area di disegno testi */
static void br1_btn1_callback();  /* callback del bottone sinistro */
static void br1_btn3_callback();  /* callback del bottone destro */
static void br1_refresh();        /* callback da chiamarsi scaduto il
                                      timeout di refresh */

float estr_sh();
void staz_br1_r(flag,is,ip3)
int *flag;   /* flag */
int *is;   /* indice stazione associata */
int *ip3;  /* indice nel descrittore pagine visualizzate */
{
char app[100];
int val;
int i;
Widget wDraw,wText,wLab,wSep;  /* area per disegno */
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
br1_l=ltot-2;
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
XtAddCallback(wDraw,XmNdestroyCallback,br1_del_callback,(XtPointer)*is-1);
                                                        

memset(app,' ',17);
memcpy(app,stazione[*is-1].descrizione,strlen((char*)stazione[*is-1].descrizione));
app[17]=0;
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,0); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNborderWidth,0); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
wLab=XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);

i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hlabel); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hbutton+1); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
wRadioBox=XmCreateDrawingArea(wDraw,"label",args,i);
XtManageChild(wRadioBox);

i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hlabel); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNtopShadowColor,excolor[0].pixel); i++;
XtSetArg(args[i],XmNbottomShadowColor,excolor[0].pixel); i++;
wSep=XmCreateSeparator(wDraw,"sep",args,i);
XtManageChild(wSep);

i=0;
memcpy(app,stazione[*is-1].etic[0],4);
app[4]=0;
XtSetArg(args[i],XmNx,5); i++;
XtSetArg(args[i],XmNy,4); i++;
XtSetArg(args[i],XmNmarginWidth,1); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,XmSTRING_DEFAULT_CHARSET)); i++;
wToggle1=XmCreatePushButton(wRadioBox,"push",args,i);
i=0;
memcpy(app,stazione[*is-1].etic[1],4);
app[4]=0;
XtSetArg(args[i],XmNx,ltot/2+5); i++;
XtSetArg(args[i],XmNy,4); i++;
XtSetArg(args[i],XmNmarginWidth,1); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,XmSTRING_DEFAULT_CHARSET)); i++;
wToggle3=XmCreatePushButton(wRadioBox,"push",args,i);
wstaz[*is-1].w[k_toggle1]=wToggle1;
wstaz[*is-1].w[k_toggle3]=wToggle3;
XtAddCallback(wToggle1,XmNactivateCallback,br1_btn1_callback,(XtPointer)*is-1);
XtAddCallback(wToggle3,XmNactivateCallback,br1_btn3_callback,(XtPointer)*is-1);
XtManageChild(wToggle1);
XtManageChild(wToggle3);
/*
 Aggiunge una routine di refresh alla stazione creata
 */
if(add_refresh((XtCallbackProc)br1_refresh,(void*)*is-1)==-1)
	printf("\n errore : refresh non aggiunto");

}

static void br1_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int is= (int)info;
del_refresh((void*)is);
}
       

static void br1_draw2_callback(w,info,str)
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
 Callback per il bottone 1
 */
static void br1_btn1_callback(w,info,toggle)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *toggle;
{
int is;
float valfin;
float gruppo1;
int tipo;
int ipunti,imodel,iret;
is=(int)info;
/*
 estrae dalla'area shared i valori di interesse
 */
gruppo1=estr_sh(stazione[is].rlaum[0],1);
if(gruppo1!=0)
	valfin=0.0;
else valfin=1.0;
/* setta il bottone in transizione */
XtSetArg(args[0],XmNforeground,excolor[2].pixel);
XtSetValues(w,args,1);
ipunti=stazione[is].rlaum[0];
imodel=stazione[is].rlaum[1];
tipo=STEP;
iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
}

/*
 Callback per il bottone 3
 */
static void br1_btn3_callback(w,info,toggle)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *toggle;
{
int is;
float gruppo2;
float valfin;
int tipo;
int ipunti,imodel,iret;
is=(int)info;
/*
 estrae dalla'area shared i valori di interesse
 */
gruppo2=estr_sh(stazione[is].rldim[0],1);
if(gruppo2!=0)
	valfin=0.0;
else valfin=1.0;
/* setta il bottone in transizione */
XtSetArg(args[0],XmNforeground,excolor[2].pixel);
XtSetValues(w,args,1);
ipunti=stazione[is].rldim[0];
imodel=stazione[is].rldim[1];
tipo=STEP;
iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);

}

/*
 Pseudo-Callback di refresh della stazione
 */

static void br1_refresh(info)
caddr_t info;
{
char app[10];
int is;
int pos_ago;
float valfin;
int ipunti,imodel,iret;
int tipo; /* tipo di perturbazione */
/* variabili raccolte dall'area di memoria shared */
float valore;
float delta;
int i;
float gruppo1;
float gruppo2;
is=(int)info;
/*
 raccoglie i valori delle variabili dall'area di memoria shared
 */
gruppo1=estr_sh(stazione[is].rlaum[0],1);
gruppo2=estr_sh(stazione[is].rldim[0],1);

/*
 esamina lo stato in cui si trova la stazione 
 */
if(gruppo1!=0)
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
if(gruppo2)
	{
        i=0;
        XtSetArg(args[i],XmNforeground,excolor[0].pixel); i++;
        XtSetArg(args[i],XmNbackground,excolor[2].pixel); i++;
        XtSetValues(wstaz[is].w[k_toggle3],args,i);
	}	
else
	{
        i=0;
        XtSetArg(args[i],XmNforeground,excolor[0].pixel); i++;
        XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
        XtSetValues(wstaz[is].w[k_toggle3],args,i);
        }
}





