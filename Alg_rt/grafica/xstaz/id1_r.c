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
static char SccsID[] = "@(#)id1_r.c	1.4\t3/23/95";
/*
   modulo id1_r.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)id1_r.c	1.4
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/Label.h>
#include <Xm/ToggleB.h>
#include <Xm/Separator.h>
#include <Xm/RowColumn.h>
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
 parametri geometrici propri della stazione ID1: vengono utilizzati
 nelle callback di redraw.
*/
int id1_l;       /* larghezza della stazione */
int id1_draw_h;  /* altezza dell'area di visualizzazione strumento
                    analogico  */

/*
  indici all'interno della struttura dei widget 
*/
#define k_toggle1  0
#define k_toggle2  1
#define k_toggle3  2
#define k_text     3
#define k_draw1    4

static void id1_draw1_callback(); /* callback della area per descrizione
                                     variabile  */
static void id1_del_callback();
static void id1_draw2_callback(); /* callback della area di disegno testi */
static void id1_btn1_callback();  /* callback del bottone sinistro */
static void id1_btn3_callback();  /* callback del bottone destro */
static void id1_refresh();        /* callback da chiamarsi scaduto il
                                      timeout di refresh */

float estr_sh();
void staz_id1_r(flag,is,ip3)
int *flag;   /* flag */
int *is;   /* indice stazione associata */
int *ip3;  /* indice nel descrittore pagine visualizzate */
{
char app[100],*px;
int val;
int i;
XmString c_str;
Widget wDraw,wText,wRow1,wSep,wLab;  /* area per disegno */
Widget wRadioBox,wToggle;
Widget wToggle1,wToggle3;
int htot; /* altezza totale */
int xdraw,ydraw;       /* altezza e posizione area di disegno */
int label,xlabel,ylabel,hlabel;    /* altezza e posizione area testo */
int hbutton,xbutton,ybutton; /* altezzza area bottoni */
int ltot;    /* lunghezza totale */
/*
 Calcola i parametri geometrici della stazione
 */
htot=(stazione[*is-1].posiy1-stazione[*is-1].posiy0)*60;
ltot=(stazione[*is-1].posix1-stazione[*is-1].posix0)*60;
/**
hbutton=htot/2;
hlabel=hbutton/2;
**/
hbutton=htot/3;
hlabel=2*htot/3;
id1_l=ltot-2;
xdraw=stazione[*is-1].posix0*62+2;
ydraw=stazione[*is-1].posiy0*62+3;
ydraw=pagvis[*ip3].height-ydraw-htot;
xlabel=0;
ylabel=0;
xbutton=0;
ybutton=hlabel;
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
XtManageChild(wDraw);
XtAddCallback(wDraw,XmNdestroyCallback,id1_del_callback,(XtPointer)*is-1);


/*
 crea area delle label
 */


memcpy(app,stazione[*is-1].descrizione[0],17);
app[17]=0;
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,0); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel/2); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wLab=XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);


app[0]=0;
px=strncat(app,stazione[*is-1].etic[0],4);
strcat(px,"    ");
strncat(px,stazione[*is-1].etic[1],4);
strcat(px,"    ");
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hlabel/2); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel/2); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wLab=XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);



/* crea la radioBox */
i=0;
XtSetArg(args[i],XmNx,10); i++;
XtSetArg(args[i],XmNy,ybutton+2); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hbutton-2); i++;
XtSetArg(args[i],XmNorientation,XmHORIZONTAL); i++;
XtSetArg(args[i],XmNspacing,0); i++;
XtSetArg(args[i],XmNborderWidth,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
wRadioBox=XmCreateRowColumn(wDraw,"label",args,i);
XtManageChild(wRadioBox);

i=0;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("     ",XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wToggle1=XmCreateToggleButton(wRadioBox,"",args,i);
wToggle3=XmCreateToggleButton(wRadioBox,"",args,i);
wstaz[*is-1].w[k_toggle1]=wToggle1;
wstaz[*is-1].w[k_toggle3]=wToggle3;
XtAddCallback(wToggle1,XmNvalueChangedCallback,id1_btn1_callback,(XtPointer)*is-1);
XtAddCallback(wToggle3,XmNvalueChangedCallback,id1_btn3_callback,(XtPointer)*is-1);
XtManageChild(wToggle1);
XtManageChild(wToggle3);

i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hlabel); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNtopShadowColor,excolor[0].pixel); i++;
XtSetArg(args[i],XmNbottomShadowColor,excolor[0].pixel); i++;
wSep=XmCreateSeparator(wDraw,"sep",args,i);
XtManageChild(wSep);

/*
 Aggiunge una routine di refresh alla stazione creata
 */
if(add_refresh((XtCallbackProc)id1_refresh,(void*)*is-1)==-1)
	printf("\n errore : refresh non aggiunto");

}

static void id1_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int is= (int)info;
del_refresh((void*)is);
}
       

static void id1_draw2_callback(w,info,str)
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

memcpy(app,stazione[is].etic[0],4);
app[4]=0;
XDrawString(display,win,gc[0],5+2*font_width,font_height+font_ascent,
            app,strlen(app));

memcpy(app,stazione[is].etic[1],4);
app[4]=0;
XDrawString(display,win,gc[0],5+9*font_width,font_height+font_ascent,
            app,strlen(app)); 
}

/*
 Callback per il bottone 1
 */
static void id1_btn1_callback(w,info,toggle)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *toggle;
{
int is;
float valfin;
int tipo;
int slstaz;
int ipunti,imodel,iret;
int rlstaz;
int slesamr;

is=(int)info;
/*
 estrae dalla'area shared i valori di interesse
 */
rlstaz=(int)estr_sh(stazione[is].rlstaz[0],1);
slesamr=(int)estr_sh(stazione[is].slesamr,1);
slstaz=(int)estr_sh(stazione[is].slstaz,1);
if(slstaz==1 && stazione[is].incorso==-1 && slesamr==rlstaz)
	{
/* setta il bottone in wait */
        XtSetArg(args[0],XmNtopShadowColor,excolor[2].pixel);
        XtSetArg(args[0],XmNbottomShadowColor,excolor[2].pixel);
	XtSetValues(w,args,1);
	XmToggleButtonSetState(wstaz[is].w[k_toggle3],False,False);
	XmToggleButtonSetState(w,False,False);
        valfin=0.0;
        ipunti=stazione[is].rlstaz[0];
        imodel=stazione[is].rlstaz[1];
        tipo=STEP;
	iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
	stazione[is].incorso=0;
	}
else if (stazione[is].incorso==-1 || slesamr!=rlstaz  )
	{
/* 
  stazione in condizione transitoria
  mantiene il bottone non settato
 */
	XmToggleButtonSetState(w,False,False);
	}
else   /* il bottone e' gia nella condizione corretta */
	{
	XmToggleButtonSetState(w,True,False);
	}

}

/*
 Callback per il bottone 3
 */
static void id1_btn3_callback(w,info,toggle)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *toggle;
{
int is;
float valfin;
int tipo;
int slstaz;
int rltarget;
int rlaum;
int rldim;
int ipunti,imodel,iret;
int rlstaz;
int slesamr;

is=(int)info;
/*
 estrae dalla'area shared i valori di interesse
 */

rlstaz=(int)estr_sh(stazione[is].rlstaz[0],1);
slesamr=(int)estr_sh(stazione[is].slesamr,1);
slstaz=(int)estr_sh(stazione[is].slstaz,1);
if(slstaz==0 && stazione[is].incorso==-1 && slesamr==rlstaz)
	{
/* setta il bottone in wait */
        XtSetArg(args[0],XmNtopShadowColor,excolor[2].pixel);
        XtSetArg(args[0],XmNbottomShadowColor,excolor[2].pixel);
	XtSetValues(w,args,1);
	XmToggleButtonSetState(wstaz[is].w[k_toggle1],False,False);
	XmToggleButtonSetState(w,False,False);
        valfin=1.0;
        ipunti=stazione[is].rlstaz[0];
        imodel=stazione[is].rlstaz[1];
        tipo=STEP;
	iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
	stazione[is].incorso=1;
	}
else if (stazione[is].incorso==-1 || slesamr!=rlstaz )
	{
/* 
  stazione in condizione transitoria
  mantiene il bottone non settato
 */
	XmToggleButtonSetState(w,False,False);
	}
else
	{
	XmToggleButtonSetState(w,True,False);
	}

}

/*
 Pseudo-Callback di refresh della stazione
 */
#define FISSO  1 /* stato dei bottoni definito */
#define WAIT   0 /* stato dei bottoni in transizione */

static void id1_refresh(info)
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
float *pstato; /* puntatore floating di appoggio */
float delta;
int i;
int slstaz;
int rlstaz;
int slesamr;
is=(int)info;
/*
 raccoglie i valori delle variabili dall'area di memoria shared
 */

slstaz=(int)estr_sh(stazione[is].slstaz,1);
rlstaz=(int)estr_sh(stazione[is].rlstaz[0],1);
slesamr=(int)estr_sh(stazione[is].slesamr,1);

/*
 esamina lo stato in cui si trova la stazione
 */
if(stazione[is].incorso !=(-1) && stazione[is].timer< TWAIT)
        {
        stazione[is].timer++;
/*
 se la richiesta e' stata ricevuta dal modello la richiesta non
 e' piu' in corso
*/
        if(rlstaz==stazione[is].incorso)
                {
                stazione[is].incorso=(-1);
                stazione[is].timer=0;
                }
        id1_set_button(stazione[is].incorso,WAIT,is);
        }
else
        {
        stazione[is].timer=0;
        stazione[is].incorso=(-1);
        if(rlstaz==slesamr)  /* stazione in stato fisso */
                {
/*
  Setta i bottoni (se necessario) in uno stato definito
 */

                id1_set_button(slstaz,FISSO,is);
                }
        else
                {
                id1_set_button(!slstaz,WAIT,is);
                }
        }

}


/*
 setta lo stato dei bottoni in base allo stato della stazione 
*/
void id1_set_button(stato,tipo,is)
int stato;
int tipo;
int is;
{
if(!stato)  /*  bottone sinistro */
	{
	if(tipo==FISSO)
		{
/* setta il bottone sinistro   */
		XmToggleButtonSetState(wstaz[is].w[k_toggle1],True,False);
/* resetta l'altro bottone senza notifica */
                XtSetArg(args[0],XmNtopShadowColor,excolor[0].pixel);
       	        XtSetArg(args[0],XmNbottomShadowColor,excolor[0].pixel);
                XtSetValues(wstaz[is].w[k_toggle1],args,1);
		XtSetValues(wstaz[is].w[k_toggle3],args,1);
		XmToggleButtonSetState(wstaz[is].w[k_toggle3],False,False);
		}
	else  /* richiesta in attesa di risposta */
		{
/* setta lo sfondo del bottone destro  */
                XtSetArg(args[0],XmNtopShadowColor,excolor[2].pixel);
                XtSetArg(args[0],XmNbottomShadowColor,excolor[2].pixel);
		XtSetValues(wstaz[is].w[k_toggle3],args,1);
		}
	}
else  /* manuale */
	{
	if(tipo==FISSO)
		{
/* resetta il bottone sinistro   */
                XtSetArg(args[0],XmNtopShadowColor,excolor[0].pixel);
                XtSetArg(args[0],XmNbottomShadowColor,excolor[0].pixel);
                XtSetValues(wstaz[is].w[k_toggle1],args,1);
		XmToggleButtonSetState(wstaz[is].w[k_toggle1],False,False);
/* setta il bottone destro */
                XtSetValues(wstaz[is].w[k_toggle3],args,1);
		XmToggleButtonSetState(wstaz[is].w[k_toggle3],True,False);
		}
	else  /* in attesa di risposta */
		{
/* setta lo sfondo del bottone di automatico  */
                 XtSetArg(args[0],XmNtopShadowColor,excolor[2].pixel);
                 XtSetArg(args[0],XmNbottomShadowColor,excolor[2].pixel);
		 XtSetValues(wstaz[is].w[k_toggle1],args,1);
		}
	}
}



