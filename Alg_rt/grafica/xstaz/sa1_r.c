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
static char SccsID[] = "@(#)sa1_r.c	1.4\t3/23/95";
/*
   modulo sa1_r.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)sa1_r.c	1.4
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
#include "sim_param.h"
#include "xstaz.h"
#include "compstaz.h"

#include "sa1.bmp"
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
extern XmFontList fontlist;

extern XColor excolor[],excolor_new[],sfondo_label,sfondo_staz;
extern int font_height;
extern int font_ascent;
extern int font_width;
extern GC gc[4];
/*
 parametri geometrici propri della stazione SA1: vengono utilizzati
 nelle callback di redraw.
*/
int sa1_l;       /* larghezza della stazione */
int sa1_draw_h;  /* altezza dell'area di visualizzazione strumento
                    analogico  */

/*
  indici all'interno della struttura dei widget 
*/
#define k_draw_ana 0
static void sa1_draw1_callback(); /* callback della area per descrizione
                                     variabile  */
static void sa1_del_callback();
static void sa1_draw2_callback(); /* callback della area di disegno testi */
static void sa1_refresh();        /* callback da chiamarsi scaduto il
                                      timeout di refresh */

float estr_sh();

/*
 vettore contenente le posizioni delle stringhe dei valori
 di fondo scala
 */
XPoint sa1_posiz[]={{2,8},  /* minimo del valore */
                    {73,8},  /* massimo valore */
                    {42,8}}; /* valore intermedio */

void staz_sa1_r(flag,is,ip3)
int *flag;   /* flag */
int *is;   /* indice stazione associata */
int *ip3;  /* indice nel descrittore pagine visualizzate */
{
char app[100];
int val;
int i;
Pixmap sfondo;
Widget wDraw,wText,wDraw1,wSep,wLab;  /* area per disegno */
int htot; /* altezza totale */
int hdraw,xdraw,ydraw;       /* altezza e posizione area di disegno */
int hlabel,xlabel,ylabel;    /* altezza e posizione area testo */
int ltot;    /* lunghezza totale */
/*
 Calcola i parametri geometrici della stazione
 */
htot=(stazione[*is-1].posiy1-stazione[*is-1].posiy0)*60;
ltot=(stazione[*is-1].posix1-stazione[*is-1].posix0)*60;
hdraw=htot/2-2;
hlabel=htot/4+1;
sa1_l=ltot-2;
sa1_draw_h=hdraw;
xdraw=stazione[*is-1].posix0*62+2;
ydraw=stazione[*is-1].posiy0*62+3;
ydraw=pagvis[*ip3].height-ydraw-htot;
xlabel=0;
ylabel=hdraw+1;

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

crea_sfondoP(&sfondo,sa1_bits,sa1_width,sa1_height);
for(i=0;i<2;i++)
        {
        val=stazione[*is-1].fs[i];
	if (i == 0)  sprintf(app,"%d",val);
        else   sprintf(app,"%4d",val);
        XDrawString(display,sfondo,gc[0],sa1_posiz[i].x,sa1_posiz[i].y,
            app,strlen(app));
        }
/* visualizza il valore intermedio */
val=((stazione[*is-1].fs[1]+stazione[*is-1].fs[0])/2);
sprintf(app,"%d",val);
XDrawString(display,sfondo,gc[0],sa1_posiz[2].x-(strlen(app)-1),sa1_posiz[2].y,
            app,strlen(app));

i=0;
XtSetArg(args[i],XmNx,12); i++;
XtSetArg(args[i],XmNy,5); i++;
XtSetArg(args[i],XmNwidth,sa1_width); i++;
XtSetArg(args[i],XmNheight,hdraw); i++;
XtSetArg(args[i],XmNbackgroundPixmap,sfondo); i++;
XtSetArg(args[i],XmNborderWidth,0); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
wDraw1=XmCreateDrawingArea(wDraw,"draw",args,i);
wstaz[*is-1].w[k_draw_ana]=wDraw1;
XtManageChild(wDraw1);
XtAddCallback(wDraw1,XmNexposeCallback,sa1_draw2_callback,(XtPointer)*is-1);
XtAddCallback(wDraw1,XmNdestroyCallback,sa1_del_callback,(XtPointer)*is-1);

i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hdraw); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNtopShadowColor,excolor[0].pixel); i++;
XtSetArg(args[i],XmNbottomShadowColor,excolor[0].pixel); i++;
wSep=XmCreateSeparator(wDraw,"sep",args,i);
XtManageChild(wSep);

/* 
  area labels 
*/

printf("\n %s ",stazione[*is-1].umis);
memset(app,' ',17);
memcpy(app,stazione[*is-1].umis,6);
app[17]=0;
printf("\n %s ",app);
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,ylabel); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wLab=XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);

memset(app,' ',17);
memcpy(app,stazione[*is-1].descrizione[0],17);
app[17]=0;
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNy,ylabel+hlabel); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
wLab=XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);


/*
 Aggiunge una routine di refresh alla stazione creata
 */
if(add_refresh((XtCallbackProc)sa1_refresh,(void*)*is-1)==-1)
	printf("\n errore : refresh non aggiunto");

}

static void sa1_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int is= (int)info;
del_refresh((void*)is);
}
      
         
static void sa1_draw1_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int i;
int xpos;
char app[20]; /* stringa di appoggio */
int is= (int)info;
Window win = str->window;
memcpy(app,stazione[is].descrizione[0],17);
app[17]=0;
i=16;
while((app[i]==' ' || app[i]==0) && i>0)  i--;
xpos=(sa1_l-font_width*i)/2;
XDrawString(display,win,gc[0],xpos,(font_height*3+font_height/2+1),
            app,strlen(app));
memcpy(app,stazione[is].umis,6);
app[6]=0;
i=6;
while((app[i]==' ' || app[i]==0) && i>0)  i--;
xpos=(sa1_l-font_width*i)/2;
XDrawString(display,win,gc[0],xpos,(font_height*3)-font_height/2+2,
            app,strlen(app));
}


/*
 disegno della window contenente l'indicatore analogici
 */
static void sa1_draw2_callback(w,info,str)
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
sa1_ago_mis(win,valore,is);
}


void sa1_ago_mis(win,valore,is)
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
pos_ago=3+90*(valore-stazione[is].fs[0])/(stazione[is].fs[1]-
                                        stazione[is].fs[0]);
if(pos_ago<3) pos_ago=3;
if(pos_ago>93) pos_ago=93;

x1=x2=pos_ago;
y1=23;
y2=10;

XDrawLine(display,win,gc[icolor],x1,y1,x2,y2);
}

/*
 Pseudo-Callback di refresh della stazione
 */

static void sa1_refresh(info)
caddr_t info;
{
char app[10];
int is;
int pos_ago;
/* variabili raccolte dall'area di memoria shared */
float valore;
float delta;
int i;
is=(int)info;
/*
 raccoglie i valori delle variabili dall'area di memoria shared
 */
valore=estr_sh(stazione[is].valore,0);
valore=(valore) * stazione[is].scalam;

/* calcolo posizione ago */
pos_ago=90*(valore-stazione[is].fs[0])/(stazione[is].fs[1]-
                                        stazione[is].fs[0]);
if(pos_ago<0)  pos_ago=0;
if(pos_ago>90) pos_ago=90;
if(stazione[is].pos_ago[0]!=pos_ago)
        {
        XClearArea(display,XtWindow(wstaz[is].w[k_draw_ana]),0,0,0,0,True);
        stazione[is].pos_ago[0]=pos_ago;
        }
}




