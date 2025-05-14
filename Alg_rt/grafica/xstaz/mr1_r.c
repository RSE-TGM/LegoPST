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
static char SccsID[] = "@(#)mr1_r.c	1.4\t3/23/95";
/*
   modulo mr1_r.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)mr1_r.c	1.4
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/Label.h>
#include <Xm/Separator.h>
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
extern S_WSTAZIONE *wstaz;
extern S_PAGVIS pagvis[MAX3_PAG];
/*
  variabili globali utilizzate per la grafica
 */
XFontStruct *font_info;
extern XmFontList fontlist;
XFontStruct font;
extern Display *display;

extern XColor excolor[],sfondo_label,excolor_new[],sfondo_staz;
extern int font_height;
extern int font_ascent;
extern int font_width;
extern GC gc[4];
/*
 parametri geometrici propri della stazione MR1: vengono utilizzati
 nelle callback di redraw.
*/
int mr1_l;       /* larghezza della stazione */
int mr1_draw_h;  /* altezza dell'area di visualizzazione strumento
                    analogico  */

/*
  indici all'interno della struttura dei widget 
*/
#define k_toggle1  0
#define k_toggle2  1
#define k_toggle3  2
#define k_text     3
#define k_draw1    4

static void mr1_del_callback();
static void mr1_draw2_callback(); /* callback della area di disegno testi */
static void mr1_btn1_callback();  /* callback del bottone  */
static void mr1_refresh();        /* callback da chiamarsi scaduto il
                                      timeout di refresh */

float estr_sh();
void staz_mr1_r(flag,is,ip3)
int *flag;   /* flag */
int *is;   /* indice stazione associata */
int *ip3;  /* indice nel descrittore pagine visualizzate */
{
char app[100];
int val;
int i;
int stato;
Widget wDraw,wText,wLab,wRow1,wSep;  /* area per disegno */
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
mr1_l=ltot-2;
xdraw=stazione[*is-1].posix0*62+2;
ydraw=stazione[*is-1].posiy0*62+3;
ydraw=pagvis[*ip3].height-ydraw-htot;
xlabel=xdraw;
ylabel=ydraw;
xbutton=xdraw;
ybutton=hlabel+1;
/*
 crea area delle label
 */


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
XtAddCallback(wDraw,XmNdestroyCallback,mr1_del_callback,(XtPointer)*is-1);
XtManageChild(wDraw);


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
XtSetArg(args[i],XmNborderWidth,0); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
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
XtSetArg(args[i],XmNx,(mr1_l-font_width*6)/2); i++;
XtSetArg(args[i],XmNy,7); i++;
XtSetArg(args[i],XmNwidth,font_width*6); i++;
XtSetArg(args[i],XmNmarginWidth,1); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("   ",XmSTRING_DEFAULT_CHARSET)); i++;
wToggle1=XmCreatePushButton(wRadioBox,"push",args,i);
wstaz[*is-1].w[k_toggle1]=wToggle1;
XtManageChild(wToggle1);
XtAddCallback(wToggle1,XmNactivateCallback,mr1_btn1_callback,(XtPointer)*is-1);

stato=(int)estr_sh(stazione[*is-1].slstaz,1);
stazione[*is-1].incorso=stato; 
/*
 Aggiunge una routine di refresh alla stazione creata
 */
if(add_refresh((XtCallbackProc)mr1_refresh,(void*)*is-1)==-1)
	printf("\n errore : refresh non aggiunto");

}

static void mr1_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int is= (int)info;
del_refresh((void*)is);
}
       


/*
 Pseudo-Callback di refresh della stazione
 */

static void mr1_refresh(info)
caddr_t info;
{
int is;
int ipunti,imodel,iret;
int tipo;
float valfin;
/* variabili raccolte dall'area di memoria shared */
int stato;
int rlstaz;
int slesamr;
int i;
is=(int)info;
/*
 raccoglie i valori delle variabili dall'area di memoria shared
 */
stato=(int)estr_sh(stazione[is].slstaz,1);
rlstaz=(int)estr_sh(stazione[is].rlstaz[0],1);
slesamr=(int)estr_sh(stazione[is].slesamr,1);
/*
 se non ci sono richieste in corso di attuazione o forzamenti
 aggiorna la stato del bottone ed esce
*/
if(stazione[is].incorso == stato)
	{
/*
 esamina lo stato in cui si trova la stazione 
 */
		i=0;
		XtSetArg(args[i],XmNtopShadowColor,excolor[0].pixel); i++;
		XtSetArg(args[i],XmNbottomShadowColor,excolor[0].pixel); i++;
        	XtSetValues(wstaz[is].w[k_toggle1],args,i);
	}
else if(rlstaz)
	{
/*********
	valfin=0.0;
	ipunti=stazione[is].rlstaz[0];
	imodel=stazione[is].rlstaz[1];
	tipo=1;
	iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
*******/
	}
/*
 caso di forzamento
 */
else if(stazione[is].incorso==0 && stato)
	{
	stazione[is].incorso=1;
	i=0;
	XtSetArg(args[i],XmNtopShadowColor,excolor[0].pixel); i++;
	XtSetArg(args[i],XmNbottomShadowColor,excolor[0].pixel); i++;
       	XtSetValues(wstaz[is].w[k_toggle1],args,i);
	}
/*
 richiesta in corso di attuazione
 */
else if(stazione[is].incorso==1 && slesamr)
	{
	stazione[is].incorso=2; 
	i=0;
	XtSetArg(args[i],XmNtopShadowColor,excolor[2].pixel); i++;
	XtSetArg(args[i],XmNbottomShadowColor,excolor[2].pixel); i++;
	XtSetValues(wstaz[is].w[k_toggle1],args,i);
	}
/*
 richiesta rifiutata
 */
else if(stazione[is].incorso==2 && !slesamr)
	{
	stazione[is].incorso=0;
	}
}

/*
 Callback per il bottone 
 */
static void mr1_btn1_callback(w,info,toggle)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *toggle;
{
int ipunti,imodel,iret;
int stato;
float valfin;
int is,i,tipo;
is=(int)info;
/*
 raccoglie i valori delle variabili dall'area di memoria shared
 */
stato=(int)estr_sh(stazione[is].slstaz,1);
if(stato) return;

valfin=1.0;
ipunti=stazione[is].rlstaz[0];
imodel=stazione[is].rlstaz[1];
tipo=IMPULSO;
iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
/*
 pulsante in transizione
*/
stazione[is].incorso=1;
i=0;
XtSetArg(args[i],XmNtopShadowColor,excolor[2].pixel); i++;
XtSetArg(args[i],XmNbottomShadowColor,excolor[2].pixel); i++;
XtSetValues(wstaz[is].w[k_toggle1],args,i);
}


