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
static char SccsID[] = "@(#)sd1_r.c	1.4\t3/23/95";
/*
   modulo sd1_r.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)sd1_r.c	1.4
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
 parametri geometrici propri della stazione SD1: vengono utilizzati
 nelle callback di redraw.
*/
int sd1_l;       /* larghezza della stazione */
int sd1_draw_h;  /* altezza dell'area di visualizzazione strumento
                    analogico  */

/*
  indici all'interno della struttura dei widget 
*/
#define k_text_dig 0

static void sd1_draw1_callback(); /* callback della area per descrizione
                                     variabile  */
static void sd1_del_callback();
static void sd1_draw2_callback(); /* callback della area di disegno testi */
static void sd1_refresh();        /* callback da chiamarsi scaduto il
                                      timeout di refresh */

float estr_sh();

void staz_sd1_r(flag,is,ip3)
int *flag;   /* flag */
int *is;   /* indice stazione associata */
int *ip3;  /* indice nel descrittore pagine visualizzate */
{
char app[100];
int val;
int i;
XmString c_str;
Widget wDraw,wText,wRow1,wLab,wSep;  /* area per disegno */
Widget wRadioBox,wToggle;
Widget wToggle1,wToggle2,wToggle3;
int htot; /* altezza totale */
int hdraw,xdraw,ydraw;       /* altezza e posizione area di disegno */
int hlabel,xlabel,ylabel;    /* altezza e posizione area testo */
int ltot;    			/* lunghezza totale */
/*
 Calcola i parametri geometrici della stazione
 */
htot=(stazione[*is-1].posiy1-stazione[*is-1].posiy0)*60;
ltot=(stazione[*is-1].posix1-stazione[*is-1].posix0)*60;
hdraw=htot/2;
hlabel=hdraw/2;
sd1_l=ltot-2;
sd1_draw_h=hdraw;
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
XtAddCallback(wDraw,XmNdestroyCallback,sd1_del_callback,(XtPointer)*is-1);


c_str=XmStringCreateLtoR("",XmSTRING_DEFAULT_CHARSET);
i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,7); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNborderWidth,0); i++;
XtSetArg(args[i],XmNlabelString,c_str); i++;
wText=XmCreateLabel(wDraw,"text",args,i);
XtFree((char*)c_str);
wstaz[*is-1].w[k_text_dig]=wText;
XtManageChild(wText);



i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hdraw); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
wSep=XmCreateSeparator(wDraw,"sep",args,i);
XtManageChild(wSep);


/*area delle labels */


memset(app,' ',17);
memcpy(&app[5],stazione[*is-1].umis,strlen(stazione[*is-1].umis));
app[17]=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,ylabel); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wLab=XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);


memset(app,' ',17);
memcpy(app,stazione[*is-1].descrizione[0],strlen(stazione[*is-1].descrizione[0])
);
app[17]=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,ylabel+hlabel); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wLab=XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);


/*
 Aggiunge una routine di refresh alla stazione creata
 */
if(add_refresh((XtCallbackProc)sd1_refresh,(void*)*is-1)==-1)
	printf("\n errore : refresh non aggiunto");

}

static void sd1_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int is= (int)info;
del_refresh((void*)is);
}
     
         
static void sd1_draw1_callback(w,info,str)
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
xpos=(sd1_l-font_width*i)/2;
XDrawString(display,win,gc[0],xpos,(font_height*3+font_height/2+1),
            app,strlen(app));
memcpy(app,stazione[is].umis,6);
app[6]=0;
i=6;
while((app[i]==' ' || app[i]==0) && i>0)  i--;
xpos=(sd1_l-font_width*i)/2;
XDrawString(display,win,gc[0],xpos,(font_height*3)-font_height/2+2,
            app,strlen(app));
}


/*
 Pseudo-Callback di refresh della stazione
 */

static void sd1_refresh(info)
caddr_t info;
{
char app[10];
XmString c_str1,c_str2;
int is;
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

/*
 setta il nuovo valore nell'indicatore digitale
*/
sprintf(app,"%7.3f",valore);
c_str1=XmStringCreateLtoR(app,XmSTRING_DEFAULT_CHARSET);
XtSetArg(args[0],XmNlabelString,&c_str2);
XtGetValues(wstaz[is].w[k_text_dig],args,1);
if(XmStringCompare(c_str1,c_str2)==0)
	{
	i=0;
/* esamina se si e' raggiunto il fondo scala */
	delta=0.05*(stazione[is].fs[1]-stazione[is].fs[0]);
	if(valore<(stazione[is].fs[0]+delta) || 
           valore>(stazione[is].fs[1]-delta))
		{
		XtSetArg(args[i],XmNforeground,excolor[4].pixel);
		i++;
		}
	else
		{
		XtSetArg(args[i],XmNforeground,excolor[0].pixel);
                i++;
		}
	XtSetArg(args[i],XmNlabelString,c_str1);  i++;
	XtSetValues(wstaz[is].w[k_text_dig],args,i);
	}
/*XtFree(c_str1);*/
}




