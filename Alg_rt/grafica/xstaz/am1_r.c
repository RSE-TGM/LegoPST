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
static char SccsID[] = "@(#)am1_r.c	1.4\t3/23/95";
/*
   modulo am1_r.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)am1_r.c	1.4
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <Xm/Label.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/ToggleB.h>
#include <Xm/Separator.h>
#include <Xm/RowColumn.h>
#include "sim_param.h"
#include "xstaz.h"
#include "compstaz.h"


#include "am1.bmp"
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
 parametri geometrici propri della stazione AM1: vengono utilizzati
 nelle callback di redraw.
*/
int am1_l;       /* larghezza della stazione */
int am1_draw_h;  /* altezza dell'area di visualizzazione strumento
                    analogico  */

/*
  indici all'interno della struttura dei widget 
*/
#define k_toggle1  0
#define k_toggle2  1
#define k_toggle3  2
#define k_text     3
#define k_draw1    4
#define k_tedit    5

static void am1_draw1_callback(); /* callback della area di disegno  */
static void am1_del_callback();
static void am1_draw2_callback(); /* callback della area di disegno testi */
static void am1_btn2_callback();  /* callback del bottone centrale */
static void am1_btn1_callback();  /* callback del bottone sinistro */
static void am1_btn3_callback();  /* callback del bottone destro */
static void am1_init_text_callback();  /* callback del bottone destro */
static void am1_fine_text_callback();  /* callback del bottone destro */
static void am1_refresh();        /* callback da chiamarsi scaduto il
                                      timeout di refresh */
static void am1_edit_text_callback();

float estr_sh();
/*
 vettore contenente le posizioni delle stringhe dei valori
 di fondo scala
 */
XPoint am1_posiz[]={{0,90},  /* minimo del valore */
                    {73,23},  /* massimo valore */
		    {19,45}}; /* valore intermedio */	

//#define PIGRECO 3.14159265358979323846
#define PIGRECO 3.1415926535897932384665358979323846

void staz_am1_r(flag,is,ip3)
int *flag;   /* flag */
int *is;   /* indice stazione associata */
int *ip3;  /* indice nel descrittore pagine visualizzate */
{
char app[100],*px;
int val;
int i;
Pixmap sfondo;
Widget wDraw,wDraw1,wText,wLab,wSep;  /* area per disegno */
Widget wRadioBox,wToggle;
Widget wToggle1,wToggle2,wToggle3;
int htot; /* altezza totale */
int hdraw,xdraw,ydraw;       /* altezza e posizione area di disegno */
int hlabel,xlabel,ylabel;    /* altezza e posizione area testo */
int hbutton,xbutton,ybutton; /* altezzza area bottoni */
int ltot,hparz;    /* lunghezza totale */



/*
 Calcola i parametri geometrici della stazione
 */
htot=(stazione[*is-1].posiy1-stazione[*is-1].posiy0)*60;
ltot=(stazione[*is-1].posix1-stazione[*is-1].posix0)*60;
hdraw=2*htot/3;
hparz=htot/3;
hbutton=hparz/3;
hlabel=hbutton*2;
am1_l=ltot-2;

am1_draw_h=hdraw;
xdraw=stazione[*is-1].posix0*62+2;
ydraw=stazione[*is-1].posiy0*62+3;
ydraw=pagvis[*ip3].height-ydraw-htot;
xlabel=0;
ylabel=hdraw+1;
xbutton=0;
ybutton=ylabel+hlabel;
/*
 Inizializza le posizioni degli indicatori analogici
 per indicare il primo disegno
 */
stazione[*is-1].pos_ago[0]=(-1);
stazione[*is-1].pos_ago[1]=(-1);

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
                                       
i=0;
memcpy(app,stazione[*is-1].umis,6);
app[6]=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,76); i++;
XtSetArg(args[i],XmNy,5); i++;
XtSetArg(args[i],XmNwidth,40); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
wLab=(Widget) XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);

/*
 Crea area di draw
 */
crea_sfondoP(&sfondo,am1_bits,am1_width,am1_height);

for(i=0;i<2;i++)
	{
	val=stazione[*is-1].fs[i];
	sprintf(app,"%4d",val);
	XDrawString(display,sfondo,gc[0],am1_posiz[i].x,am1_posiz[i].y,
            app,strlen(app));
	}
/* visualizza il valore intermedio */
val=((stazione[*is-1].fs[1]-stazione[*is-1].fs[0])/2)+stazione[*is-1].fs[0];
sprintf(app,"%4d",val);
XDrawString(display,sfondo,gc[0],am1_posiz[2].x,am1_posiz[2].y,
            app,strlen(app));

i=0;
XtSetArg(args[i],XmNx,8); i++;
XtSetArg(args[i],XmNy,20); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hdraw); i++;
XtSetArg(args[i],XmNbackgroundPixmap,sfondo); i++;
wDraw1=(Widget) XmCreateDrawingArea(wDraw,"draw",args,i);
wstaz[*is-1].w[k_draw1]=wDraw1;

XtAddCallback(wDraw1,XmNexposeCallback,am1_draw1_callback,(XtPointer)*is-1);
XtAddCallback(wDraw1,XmNdestroyCallback,am1_del_callback,(XtPointer)*is-1);
XtManageChild(wDraw1);


/*
 crea area di input testo
 */
i=0;
XtSetArg(args[i],XmNx,30); i++;
XtSetArg(args[i],XmNy,4); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNcolumns,5); i++;
XtSetArg(args[i],XmNmaxLength,5); i++;
XtSetArg(args[i],XmNborderWidth,0); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNresizeHeight,False); i++;
XtSetArg(args[i],XmNresizeWidth,False); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
XtSetArg(args[i],XmNeditMode,XmSINGLE_LINE_EDIT); i++;
wText=(Widget) XmCreateText(wDraw,"text",args,i);
XtManageChild(wText);
XmTextSetEditable(wText,False);
wstaz[*is-1].w[k_text]=wText;


i=0;
XtSetArg(args[i],XmNx,2); i++;
XtSetArg(args[i],XmNy,2); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(" ",XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wstaz[*is-1].w[k_tedit]=(Widget) XmCreateToggleButton(wDraw,"text",args,i);
XtManageChild(wstaz[*is-1].w[k_tedit]);
XtAddCallback(wstaz[*is-1].w[k_tedit],XmNvalueChangedCallback,am1_edit_text_callback,(XtPointer)*is-1);

i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hdraw); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNtopShadowColor,excolor[0].pixel); i++;
XtSetArg(args[i],XmNbottomShadowColor,excolor[0].pixel); i++;
wSep= XmCreateSeparator(wDraw,"sep",args,i);
XtManageChild(wSep);

/*
 crea area delle label
 */

memcpy(app,stazione[*is-1].descrizione[0],17);
app[17]=0;
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,ylabel); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel/2); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
wLab=(Widget) XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);



app[0]=0;
px=strncat(app,stazione[*is-1].etic[0],4);
strcat(px,"  ");
strncat(px,stazione[*is-1].etic[1],4);
strcat(px,"  ");
strncat(px,stazione[*is-1].etic[2],4);
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,ylabel+hlabel/2); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel/2); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wLab=(Widget) XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);


i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,ybutton); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNtopShadowColor,excolor[0].pixel); i++;
XtSetArg(args[i],XmNbottomShadowColor,excolor[0].pixel); i++;
wSep= XmCreateSeparator(wDraw,"sep",args,i);
XtManageChild(wSep);

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
wRadioBox=(Widget) XmCreateRowColumn(wDraw,"label",args,i);
XtManageChild(wRadioBox);



i=0;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("  ",XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wToggle1=(Widget) XmCreateToggleButton(wRadioBox,"",args,i);
i=0;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("  ",XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wToggle2=(Widget) XmCreateToggleButton(wRadioBox,"",args,i);
i=0;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("  ",XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wToggle3=(Widget) XmCreateToggleButton(wRadioBox,"",args,i);
wstaz[*is-1].w[k_toggle1]=wToggle1;
wstaz[*is-1].w[k_toggle2]=wToggle2;
wstaz[*is-1].w[k_toggle3]=wToggle3;
XtAddCallback(wToggle2,XmNvalueChangedCallback,am1_btn2_callback,(XtPointer)*is-1);
XtAddCallback(wToggle1,XmNvalueChangedCallback,am1_btn1_callback,(XtPointer)*is-1);
XtAddCallback(wToggle3,XmNvalueChangedCallback,am1_btn3_callback,(XtPointer)*is-1);
XtManageChild(wToggle1);
XtManageChild(wToggle2);
XtManageChild(wToggle3);
/*
 Aggiunge una routine di refresh alla stazione creata
 */
if(add_refresh((XtCallbackProc)am1_refresh,(void*)*is-1)==-1)
	printf("\n errore : refresh non aggiunto");

}

static void am1_edit_text_callback(w,info,str)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *str;
{

if (str->set)  am1_init_text_callback(w,info,str); /* set bottone */
else           am1_fine_text_callback(w,info,str); /* reset */
}


static void am1_init_text_callback(w,info,str)
Widget w;
caddr_t info;
XmAnyCallbackStruct *str;
{
int slstaz;
int is;
is=(int)info;
slstaz=(int)estr_sh(stazione[is].slstaz,1);

if(!slstaz && stazione[is].incorso==(-1))
	{
	XmTextSetEditable(wstaz[is].w[k_text],True);
	XmTextSetString(wstaz[is].w[k_text],"");
	}
else    {
	XmTextSetEditable(wstaz[is].w[k_text],False);
	XmToggleButtonSetState(w,False,False);
	}

}


static void am1_fine_text_callback(w,info,str)
Widget w;
caddr_t info;
XmAnyCallbackStruct *str;
{
float valore;
char *pstring;
float valfin;
int is;
int ipunti,imodel,iret;
int tipo; /* tipo di perturbazione */
/* variabili raccolte dall'area di memoria shared */
int slstaz;
/*
 se il testo non e' editabile non accetto il valore attuale
*/

is=(int)info;
if(XmTextGetEditable(wstaz[is].w[k_text])==False) return;
slstaz=(int)estr_sh(stazione[is].slstaz,1);

pstring=(char *) XmTextGetString(wstaz[is].w[k_text]);
iret=sscanf(pstring,"%f",&valore);
/*
 in caso di valore inserito non valido annulla l'operazione
 */
if(iret!=1)
	{
	XtFree(pstring);
	XmTextSetEditable(wstaz[is].w[k_text],False);
	return;
	}
/*
 il valore inserito e' accettabile
 */
if(!slstaz && stazione[is].incorso==-1)
	{
/*
 Invia il valore di target richiesto dall'utente
*/
	valfin=valore/stazione[is].scalam;
	ipunti=stazione[is].target[0];
	imodel=stazione[is].target[1];
        tipo=STEP;
        iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
/*
 Invia un impulso sulla variabile di richiesta target
*/
	valfin=1.0;
	ipunti=stazione[is].rltarget[0];
        imodel=stazione[is].rltarget[1];
        tipo=IMPULSO;
        iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
	}
XtFree(pstring);
XmTextSetEditable(wstaz[is].w[k_text],False);
}

         
static void am1_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int is= (int)info;
del_refresh((void*)is);
}

/*
 disegno della window contenente gli indicatori analogici
 */

static void am1_draw1_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int i;
float valore;
char app[20]; /* stringa di appoggio */
int is= (int)info;
Window win = str->window;
int val;

valore=estr_sh(stazione[is].valore,0);
valore=(valore) * stazione[is].scalam;

/*
   Disegna l'indicatore della misura
 */
am1_ago_mis(win,valore,is);
}

void am1_ago_mis(win,valore,is)
Window win;
float valore;
int is;
{
int x1,y1,x2,y2;
double dx,dy;
int pos_ago;
double dpos_ago;
float delta;
int icolor;

/* esamina se si e' raggiunto il fondo scala */
delta=0.05*(stazione[is].fs[1]-stazione[is].fs[0]);
if(valore<(stazione[is].fs[0]+delta) || valore>(stazione[is].fs[1]-delta))
	icolor=4;
else
	icolor=2;
/* calcolo posizione ago */
pos_ago=90*(valore-stazione[is].fs[0])/(stazione[is].fs[1]-
                                        stazione[is].fs[0]);
if(pos_ago<0) pos_ago=0;
if(pos_ago>90) pos_ago=90;

x1=90;
y1=90;
dpos_ago=pos_ago;
dpos_ago=(dpos_ago/90.0*(PIGRECO/2));
dx=cos(dpos_ago);
dy=sin(dpos_ago);
x2=(int)((float)90.0-65.0*dx);
y2=(int)((float)90.0-65.0*dy);

XDrawLine(display,win,gc[icolor],x1,y1,x2,y2);
}


static void am1_draw2_callback(w,info,str)
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
XDrawString(display,win,gc[0],5,font_height+font_ascent,
            app,strlen(app));

memcpy(app,stazione[is].etic[1],4);
app[4]=0;
XDrawString(display,win,gc[0],5+5*font_width,font_height+font_ascent,
            app,strlen(app));

memcpy(app,stazione[is].etic[2],4);
app[4]=0;
XDrawString(display,win,gc[0],5+10*font_width,font_height+font_ascent,
            app,strlen(app)); 
}

static void am1_btn2_callback(w,info,toggle)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *toggle;
{
int is;
float valore;
float valfin;
int slstaz;
int tipo;
int ipunti,imodel,iret;
int ilogic,istaza;
int rlstaz;
int slesamr;

is=(int)info;

rlstaz=(int)estr_sh(stazione[is].rlstaz[0],1);
slesamr=(int)estr_sh(stazione[is].slesamr,1);

/*
 se vi sono in corso delle richieste non considera
 la richiesta attuale
 */
if(stazione[is].incorso!=-1 ||  slesamr!=rlstaz )
	{
/*
 annulla il settaggio del bottone
 */
	XmToggleButtonSetState(wstaz[is].w[k_toggle2],False,False);
	return;
	}

/*
 raccoglie i valori delle variabili dall'area di memoria shared
*/
valore=estr_sh(stazione[is].valore,0);
valfin=(valore) * stazione[is].scalam;
slstaz=(int)estr_sh(stazione[is].slstaz,1);

valfin=!rlstaz;
tipo=STEP;
stazione[is].incorso=valfin;
stazione[is].timer=0;
if(slstaz==0)
        {
/*
  pone la stazione in stato automatico
 */
/* resetta gli atri due bottoni senza notifica */
        XmToggleButtonSetState(wstaz[is].w[k_toggle1],False,False);
        XmToggleButtonSetState(wstaz[is].w[k_toggle3],False,False);
        XmToggleButtonSetState(wstaz[is].w[k_toggle2],False,False);
        }
else
        {
/*
  pone la stazione in stato manuale
*/
        XmToggleButtonSetState(wstaz[is].w[k_toggle2],False,False);
        }

ipunti=stazione[is].rlstaz[0];
imodel=stazione[is].rlstaz[1];
g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
/*
 esamina se esiste una stazione associata
 */
if(stazione[is].stassoc[0]!=0)
	{
	istaza=stazione[is].stassoc[0];
	ilogic=stazione[is].stassoc[1];
	valfin=estr_sh(stazione[istaza].valore,0);
	ipunti=stazione[istaza].target[0];
	imodel=stazione[istaza].target[1];
        iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
	if(slstaz)
		{
		if(ilogic)
			{
			valfin=0.0;
			stazione[istaza].incorso=0;
			}
		else
			{
			valfin=1.0;
			stazione[istaza].incorso=1;
			}
		}
	else
		{
		if(ilogic)
			{
			valfin=1.0;
			stazione[istaza].incorso=1;
			}
		else
			{
			valfin=0.0;
			stazione[istaza].incorso=0;
			}
		}
	ipunti=stazione[istaza].rlstaz[0];
	imodel=stazione[istaza].rlstaz[1];
        iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
	}
}

/*
 Callback per il bottone 1
 */

static void am1_btn1_callback(w,info,toggle)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *toggle;
{
int is;
float valfin;
int tipo;
int slstaz;
int ipunti,imodel,iret;
is=(int)info;
/*
 estrae dalla'area shared i valori di interesse
 */
slstaz=(int)estr_sh(stazione[is].slstaz,1);
if(slstaz==0 && stazione[is].incorso==-1)
	{
/*
 Invia un impulso sulla variabile di richiesta diminuzione
*/
        valfin=1.0;
        ipunti=stazione[is].rldim[0];
        imodel=stazione[is].rldim[1];
        tipo=IMPULSO;
        iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
/* setta il bottone in wait */
        XtSetArg(args[0],XmNtopShadowColor,excolor[2].pixel);
        XtSetArg(args[0],XmNbottomShadowColor,excolor[2].pixel);
	XtSetValues(wstaz[is].w[k_toggle1],args,1);
/*
 Porta il bottone in stato selezionato senza provocare la callback di
 value changed
*/
	XmToggleButtonSetState(w,False,False);
	}
else
	{
/* 
  stazione in manuale o in condizione transitoria
  mantiene il bottone non settato
 */
	XmToggleButtonSetState(w,False,True);
	}
}

/*
 Callback per il bottone 3
 */
static void am1_btn3_callback(w,info,toggle)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *toggle;
{
int is;
float valfin;
int tipo;
int slstaz;
int ipunti,imodel,iret;
is=(int)info;
/*
 estrae dalla'area shared i valori di interesse
 */

slstaz=(int)estr_sh(stazione[is].slstaz,1);
if(slstaz==0 && stazione[is].incorso==-1)
	{
/*
 Invia un impulso sulla variabile di richiesta aumento
*/
         valfin=1.0;
         ipunti=stazione[is].rlaum[0];
         imodel=stazione[is].rlaum[1];
         tipo=IMPULSO;
         iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);

/* setta il bottone in wait */

        XtSetArg(args[0],XmNtopShadowColor,excolor[2].pixel);
        XtSetArg(args[0],XmNbottomShadowColor,excolor[2].pixel);
/*
 Porta il bottone in stato selezionato senza provocare la callback di
 value changed
*/
	XmToggleButtonSetState(w,False,False);
	}
else
	{
/* 
  stazione in manuale o in condizione transitoria
  mantiene il bottone non settato
 */
	XmToggleButtonSetState(w,False,False);
	}
}

/*
 Pseudo-Callback di refresh della stazione
 */
#define FISSO  1 /* stato dei bottoni definito */
#define WAIT   0 /* stato dei bottoni in transizione */

static void am1_refresh(info)
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
int slstaz;
int rlstaz;
int slesamr;
is=(int)info;
/*
 raccoglie i valori delle variabili dall'area di memoria shared
 */
valore=estr_sh(stazione[is].valore,0);
valore=(valore) * stazione[is].scalam;
slstaz=(int)estr_sh(stazione[is].slstaz,1);
rlstaz=(int)estr_sh(stazione[is].rlstaz[0],1);
slesamr=(int)estr_sh(stazione[is].slesamr,1);

/*
 esamina lo stato in cui si trova la stazione 
 */
/*
 se e' in corso una richiesta e non e' ancora scattato il timer
 incrementa il timer
*/

if(stazione[is].incorso!=(-1) && stazione[is].timer< TWAIT)
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
        am1_set_button(stazione[is].incorso,WAIT,is);
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
                am1_set_button(slstaz,FISSO,is);
                }
        else
                {
                am1_set_button(!slstaz,WAIT,is);
                }
        }
/*
 esamina se e' variata la posizione dell'ago: in tal caso forza
 il refresh della window
*/
/* calcolo posizione ago */
pos_ago=90*(valore-stazione[is].fs[0])/(stazione[is].fs[1]-
                                        stazione[is].fs[0]);
if(pos_ago<0)  pos_ago=0;
if(pos_ago>90) pos_ago=90;

if(stazione[is].pos_ago[0]!=pos_ago) 
	{
	XClearArea(display,XtWindow(wstaz[is].w[k_draw1]),0,0,0,0,True);
	stazione[is].pos_ago[0]=pos_ago;
	}
if(XmTextGetEditable(wstaz[is].w[k_text])==False)
	{
	sprintf(app,"%5.1f",valore);
	XmTextSetString(wstaz[is].w[k_text],app);
	}
}



/*
 setta lo stato dei bottoni in base allo stato della stazione 
*/
void am1_set_button(stato,tipo,is)
int stato;
int tipo;
int is;
{
if(stato)  /*  automatico */
	{
	if(tipo==FISSO)
		{
/* setta il bottone centrale   */
		XmToggleButtonSetState(wstaz[is].w[k_toggle2],True,False);
/* resetta gli atri due bottoni senza notifica */
	        XtSetArg(args[0],XmNtopShadowColor,excolor[0].pixel);
                XtSetArg(args[0],XmNbottomShadowColor,excolor[0].pixel);
                XtSetValues(wstaz[is].w[k_toggle1],args,1);
		XtSetValues(wstaz[is].w[k_toggle3],args,1);
		XtSetValues(wstaz[is].w[k_toggle2],args,1);
		XmToggleButtonSetState(wstaz[is].w[k_toggle1],False,False);
		XmToggleButtonSetState(wstaz[is].w[k_toggle3],False,False);
		XmTextSetEditable(wstaz[is].w[k_text],False);
		XmToggleButtonSetState(wstaz[is].w[k_tedit],False,False);
		XtSetSensitive(wstaz[is].w[k_tedit],False);
		}
	else  /* richiesta in attesa di risposta */
		{
/* setta gli sfondi dei bottoni di manuale  */

       		XtSetArg(args[0],XmNtopShadowColor,excolor[2].pixel);
                XtSetArg(args[0],XmNbottomShadowColor,excolor[2].pixel);
		XtSetValues(wstaz[is].w[k_toggle1],args,1);
		XtSetValues(wstaz[is].w[k_toggle3],args,1);
		}
	}
else  /* manuale */
	{
	if(tipo==FISSO)
		{
/* resetta il bottone centrale   */
       		XtSetArg(args[0],XmNtopShadowColor,excolor[0].pixel);
                XtSetArg(args[0],XmNbottomShadowColor,excolor[0].pixel);
                XtSetValues(wstaz[is].w[k_toggle2],args,1);
		XmToggleButtonSetState(wstaz[is].w[k_toggle2],False,False);
/* setta gli atri due bottoni senza notifica */
                XtSetValues(wstaz[is].w[k_toggle1],args,1);
                XtSetValues(wstaz[is].w[k_toggle3],args,1);
		XmToggleButtonSetState(wstaz[is].w[k_toggle1],True,False);
		XmToggleButtonSetState(wstaz[is].w[k_toggle3],True,False);
		XtSetSensitive(wstaz[is].w[k_tedit],True);
		}
	else  /* in attesa di risposta */
		{
/* setta lo sfondo del bottone di automatico  */
       		XtSetArg(args[0],XmNtopShadowColor,excolor[2].pixel);
                XtSetArg(args[0],XmNbottomShadowColor,excolor[2].pixel);
		XtSetValues(wstaz[is].w[k_toggle2],args,1);
		}
	}
}



