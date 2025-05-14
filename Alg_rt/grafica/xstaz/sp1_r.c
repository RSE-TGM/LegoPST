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
static char SccsID[] = "@(#)sp1_r.c	1.4\t3/23/95";
/*
   modulo sp1_r.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)sp1_r.c	1.4
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
#include "sp1.bmp"
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
extern XmFontList fontlist;
extern Display *display;

extern XColor excolor[],excolor_new[],sfondo_label,sfondo_staz;
extern int font_height;
extern int font_ascent;
extern int font_width;
extern GC gc[4];
/*
 parametri geometrici propri della stazione SP1: vengono utilizzati
 nelle callback di redraw.
*/
int sp1_l;       /* larghezza della stazione */
int sp1_draw_h;  /* altezza dell'area di visualizzazione strumento
                    analogico  */

/*
  indici all'interno della struttura dei widget 
*/
#define k_toggle1  0
#define k_toggle3  2
#define k_text     3
#define k_draw1    4
#define k_tedit    5

static void sp1_draw1_callback(); /* callback della area di disegno  */
static void sp1_del_callback();
static void sp1_draw2_callback(); /* callback della area di disegno testi */
static void sp1_btn1_callback();  /* callback del bottone sinistro */
static void sp1_btn3_callback();  /* callback del bottone destro */
static void sp1_init_text_callback();  /* callback del bottone destro */
static void sp1_fine_text_callback();  /* callback del bottone destro */
static void sp1_refresh();        /* callback da chiamarsi scaduto il
                                      timeout di refresh */

static void sp1_edit_text_callback();

float estr_sh();
/*
 vettore contenente le posizioni delle stringhe dei valori
 di fondo scala
 */
XPoint sp1_posiz[]={{0,90},  /* minimo del valore */
                    {73,23},  /* massimo valore */
		    {19,45}}; /* valore intermedio */	
#define PIGRECO 3.14159265358979323846
void staz_sp1_r(flag,is,ip3)
int *flag;   /* flag */
int *is;   /* indice stazione associata */
int *ip3;  /* indice nel descrittore pagine visualizzate */
{
char app[100],*px;
int val;
int i;
Pixmap sfondo;
Widget wDraw,wText,wDraw1,wLab,wSep;  
Widget wRadioBox,wToggle;
Widget wToggle1,wToggle2,wToggle3;
int htot; /* altezza totale */
int hdraw,xdraw,ydraw;       /* altezza e posizione area di disegno */
int hlabel,xlabel,ylabel;    /* altezza e posizione area testo */
int hbutton,xbutton,ybutton; /* altezzza area bottoni */
int ltot;    /* lunghezza totale */
int hparz;
/*
 Calcola i parametri geometrici della stazione
 */
htot=(stazione[*is-1].posiy1-stazione[*is-1].posiy0)*60;
ltot=(stazione[*is-1].posix1-stazione[*is-1].posix0)*60;
hdraw=2*htot/3;
hparz=htot/3;
hbutton=hparz/3;
hlabel=hbutton*2;
sp1_l=ltot-2;
sp1_draw_h=hdraw;
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
crea_sfondoP(&sfondo,sp1_bits,sp1_width,sp1_height);

for(i=0;i<2;i++)
	{
	val=stazione[*is-1].fs[i];
	sprintf(app,"%4d",val);
	XDrawString(display,sfondo,gc[0],sp1_posiz[i].x,sp1_posiz[i].y,
            app,strlen(app));
	}
/* visualizza il valore intermedio */
val=((stazione[*is-1].fs[1]-stazione[*is-1].fs[0])/2)+stazione[*is-1].fs[0];
sprintf(app,"%4d",val);
XDrawString(display,sfondo,gc[0],sp1_posiz[2].x,sp1_posiz[2].y,
            app,strlen(app));

i=0;
XtSetArg(args[i],XmNx,8); i++;
XtSetArg(args[i],XmNy,20); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hdraw); i++;
XtSetArg(args[i],XmNbackgroundPixmap,sfondo); i++;
wDraw1=XmCreateDrawingArea(wDraw,"draw",args,i);
wstaz[*is-1].w[k_draw1]=wDraw1;

XtAddCallback(wDraw1,XmNexposeCallback,sp1_draw1_callback,(XtPointer)*is-1);
XtAddCallback(wDraw1,XmNdestroyCallback,sp1_del_callback,(XtPointer)*is-1);
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
wText=XmCreateText(wDraw,"text",args,i);
XtManageChild(wText);
XmTextSetEditable(wText,False);
wstaz[*is-1].w[k_text]=wText;


i=0;
XtSetArg(args[i],XmNx,2); i++;
XtSetArg(args[i],XmNy,2); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(" ",XmSTRING_DEFAULT_CHARSET)
); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wstaz[*is-1].w[k_tedit]=XmCreateToggleButton(wDraw,"text",args,i);
XtManageChild(wstaz[*is-1].w[k_tedit]);
XtAddCallback(wstaz[*is-1].w[k_tedit],XmNvalueChangedCallback,sp1_edit_text_callback,(XtPointer)*is-1);

i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hdraw); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNtopShadowColor,excolor[0].pixel); i++;
XtSetArg(args[i],XmNbottomShadowColor,excolor[0].pixel); i++;
wSep=XmCreateSeparator(wDraw,"sep",args,i);
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
XtSetArg(args[i],XmNy,ylabel+hlabel/2); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel/2); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wLab=XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);


i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,ybutton); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNtopShadowColor,excolor[0].pixel); i++;
XtSetArg(args[i],XmNbottomShadowColor,excolor[0].pixel); i++;
wSep=XmCreateSeparator(wDraw,"sep",args,i);
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
wRadioBox=XmCreateRowColumn(wDraw,"label",args,i);
XtManageChild(wRadioBox);


i=0;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("    ",XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wToggle1=XmCreateToggleButton(wRadioBox,"",args,i);
wToggle3=XmCreateToggleButton(wRadioBox,"",args,i);
wstaz[*is-1].w[k_toggle1]=wToggle1;
wstaz[*is-1].w[k_toggle3]=wToggle3;
XtAddCallback(wToggle1,XmNvalueChangedCallback,sp1_btn1_callback,(XtPointer)*is-1);
XtAddCallback(wToggle3,XmNvalueChangedCallback,sp1_btn3_callback,(XtPointer)*is-1);
XtManageChild(wToggle1);
XtManageChild(wToggle3);
/*
 Aggiunge una routine di refresh alla stazione creata
 */
if(add_refresh((XtCallbackProc)sp1_refresh,(void*)*is-1)==-1)
	printf("\n errore : refresh non aggiunto");

}


         
static void sp1_edit_text_callback(w,info,str)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *str;
{

if (str->set)  sp1_init_text_callback(w,info,str); /* set bottone */
else           sp1_fine_text_callback(w,info,str); /* reset */
}



static void sp1_init_text_callback(w,info,str)
Widget w;
caddr_t info;
XmAnyCallbackStruct *str;
{
int slstaz;
int is;
is=(int)info;
slstaz=(int)estr_sh(stazione[is].slstaz,1);
printf("\n init_text stato %d ",slstaz);

if(!slstaz )
        {
        XmTextSetEditable(wstaz[is].w[k_text],True);
        XmTextSetString(wstaz[is].w[k_text],"");
        }
else    {
        XmTextSetEditable(wstaz[is].w[k_text],False);
        XmToggleButtonSetState(w,False,False);
        }

}




static void sp1_fine_text_callback(w,info,str)
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

pstring=XmTextGetString(wstaz[is].w[k_text]);
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
if(!slstaz )
        {
        valfin=valore/stazione[is].scalam;
        ipunti=stazione[is].target[0];
        imodel=stazione[is].target[1];
        tipo=STEP;
        iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
        valfin=1.0;
        ipunti=stazione[is].rltarget[0];
        imodel=stazione[is].rltarget[1];
        tipo=IMPULSO;
        iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
        }
XtFree(pstring);
XmTextSetEditable(wstaz[is].w[k_text],False);
}

static void sp1_del_callback(w,info,str)
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

static void sp1_draw1_callback(w,info,str)
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
sp1_ago_mis(win,valore,is);
}

void sp1_ago_mis(win,valore,is)
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


static void sp1_draw2_callback(w,info,str)
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
XDrawString(display,win,gc[0],5+11*font_width,font_height+font_ascent,
            app,strlen(app)); 
}


/*
 Callback per il bottone 1
 */

static void sp1_btn3_callback(w,info,toggle)
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
        valfin=1.0;
        ipunti=stazione[is].rlaum[0];
        imodel=stazione[is].rlaum[1];
        tipo=IMPULSO;
        iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);       
/* setta il bottone in wait */
        XtSetArg(args[0],XmNtopShadowColor,excolor[2].pixel);
        XtSetArg(args[0],XmNbottomShadowColor,excolor[2].pixel);
	XtSetValues(wstaz[is].w[k_toggle3],args,1);
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
 Callback per il bottone 1
 */
static void sp1_btn1_callback(w,info,toggle)
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
        valfin=1.0;
        ipunti=stazione[is].rldim[0];
        imodel=stazione[is].rldim[1];
	tipo=IMPULSO;
        iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);       
/* setta il bottone in wait */
        XtSetArg(args[0],XmNtopShadowColor,excolor[2].pixel);
        XtSetArg(args[0],XmNbottomShadowColor,excolor[2].pixel);
	XtSetValues(wstaz[is].w[k_toggle1],args,1);
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

static void sp1_refresh(info)
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
int slesamr;
is=(int)info;
/*
 raccoglie i valori delle variabili dall'area di memoria shared
 */
valore=estr_sh(stazione[is].valore,0);
valore=(valore) * stazione[is].scalam;
slstaz=(int)estr_sh(stazione[is].slstaz,1);
/*
 esamina lo stato in cui si trova la stazione ed abilita o
 disabilita in corrispospondenza i due bottoni 
 */
sp1_set_button(slstaz,is);
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
void sp1_set_button(stato,is)
int stato;
int is;
{
XtSetArg(args[0],XmNtopShadowColor,excolor[0].pixel);
XtSetArg(args[0],XmNbottomShadowColor,excolor[0].pixel);
XtSetValues(wstaz[is].w[k_toggle1],args,1);
XtSetValues(wstaz[is].w[k_toggle3],args,1);


if(stato)  /*  automatico */
	{
/* resetta i due bottoni senza notifica */
        XmToggleButtonSetState(wstaz[is].w[k_toggle1],False,False);
	XmToggleButtonSetState(wstaz[is].w[k_toggle3],False,False);
	XtSetSensitive(wstaz[is].w[k_tedit],False);
	}
else  /* manuale */
	{
/* setta i due bottoni senza notifica */
	XmToggleButtonSetState(wstaz[is].w[k_toggle1],True,False);
	XmToggleButtonSetState(wstaz[is].w[k_toggle3],True,False);
	XtSetSensitive(wstaz[is].w[k_tedit],True);
	}
}



