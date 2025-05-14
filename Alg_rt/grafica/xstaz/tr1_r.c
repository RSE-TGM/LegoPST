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
static char SccsID[] = "@(#)tr1_r.c	1.4\t3/23/95";
/*
   modulo tr1_r.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)tr1_r.c	1.4
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/DrawingA.h>
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

extern XColor excolor[],excolor_new[],sfondo_label;
extern int font_height;
extern int font_ascent;
extern int font_width;
extern GC gc[4];
extern XmFontList fontlist;
/*
 parametri geometrici propri della stazione TR1: vengono utilizzati
 nelle callback di redraw.
*/
int tr1_l;       /* larghezza della stazione */
int tr1_draw_h;  /* altezza dell'area di visualizzazione strumento
                    analogico  */

/*
  indici all'interno della struttura dei widget 
*/
#define k_toggle1  0
#define k_toggle3  2
#define k_text     3
#define k_draw1    4
#define k_tedit    5

static void tr1_draw1_callback(); /* callback della area di disegno  */
static void tr1_del_callback();
static void tr1_init_text_callback();  /* callback del bottone destro */
static void tr1_fine_text_callback();  /* callback del bottone destro */
static void tr1_refresh();        /* callback da chiamarsi scaduto il
                                      timeout di refresh */
static void tr1_edit_text_callback();

float estr_sh();
void staz_tr1_r(flag,is,ip3)
int *flag;   /* flag */
int *is;   /* indice stazione associata */
int *ip3;  /* indice nel descrittore pagine visualizzate */
{
char app[100];
int val;
int i;
Widget wDraw,wText,wLab,wSep;  /* area per disegno */
Widget wRadioBox,wToggle,wbox;
Widget wToggle1,wToggle2,wToggle3;
int htot; /* altezza totale */
int hdraw,xdraw,ydraw;       /* altezza e posizione area di disegno */
int hlabel,xlabel,ylabel;    /* altezza e posizione area testo */
int hbutton,xbutton,ybutton; /* altezzza area bottoni */
int ltot;    	             /* lunghezza totale */
/*
 Calcola i parametri geometrici della stazione
 */

htot=(stazione[*is-1].posiy1-stazione[*is-1].posiy0)*60;
ltot=(stazione[*is-1].posix1-stazione[*is-1].posix0)*60;
hdraw=htot/2;
tr1_l=ltot-2;
tr1_draw_h=hdraw;
hlabel=hdraw/2;
ylabel=hdraw;
xdraw=stazione[*is-1].posix0*62+2;
ydraw=stazione[*is-1].posiy0*62+3;
ydraw=pagvis[*ip3].height-ydraw-htot;


i=0;
XtSetArg(args[i],XmNx,xdraw); i++;
XtSetArg(args[i],XmNy,ydraw); i++;
XtSetArg(args[i],XmNwidth,ltot);i++;
XtSetArg(args[i],XmNheight,htot);i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNborderWidth,2);i++;
XtSetArg(args[i],XmNmarginWidth,0);i++;
XtSetArg(args[i],XmNmarginHeight,0);i++;
XtSetArg(args[i],XmNresizePolicy,XmRESIZE_NONE);i++;
wbox=XmCreateDrawingArea(pagvis[*ip3].w,"box",args,i);
XtAddCallback(wbox,XmNdestroyCallback,tr1_del_callback,(XtPointer)*is-1);
XtManageChild(wbox);


/*
 crea area di input testo
 */


i=0;
XtSetArg(args[i],XmNx,30); i++;
XtSetArg(args[i],XmNy,0); i++;
XtSetArg(args[i],XmNwidth,2*ltot/3+20); i++;
XtSetArg(args[i],XmNheight,htot/2); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNcolumns,10); i++;
XtSetArg(args[i],XmNmaxLength,10); i++;
XtSetArg(args[i],XmNborderWidth,0); i++;
/*
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
*/
XtSetArg(args[i],XmNresizeHeight,False); i++;
XtSetArg(args[i],XmNresizeWidth,False); i++;
XtSetArg(args[i],XmNshadowThickness,0); i++;
XtSetArg(args[i],XmNeditMode,XmSINGLE_LINE_EDIT); i++;
wText=(Widget) XmCreateText(wbox,"text",args,i);
XtManageChild(wText);
XmTextSetEditable(wText,False);
wstaz[*is-1].w[k_text]=wText;



i=0;
XtSetArg(args[i],XmNx,2); i++;
XtSetArg(args[i],XmNy,2); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("",XmSTRING_DEFAULT_CHARSET
)); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
wstaz[*is-1].w[k_tedit]=(Widget) XmCreateToggleButton(wbox,"text",args,i);
XtManageChild(wstaz[*is-1].w[k_tedit]);
XtAddCallback(wstaz[*is-1].w[k_tedit],XmNvalueChangedCallback,tr1_edit_text_callback,(XtPointer)*is-1);

i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hdraw); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNtopShadowColor,excolor[0].pixel); i++;
XtSetArg(args[i],XmNbottomShadowColor,excolor[0].pixel); i++;
wSep=(Widget) XmCreateSeparator(wbox,"sep",args,i);
XtManageChild(wSep);

/*area delle labels */

i=0;
memset(app,' ',17);
memcpy(&app[5],stazione[*is-1].umis,strlen(stazione[*is-1].umis));
app[17]=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,ylabel+1); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel-1); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
wLab=(Widget) XmCreateLabel(wbox,"lab",args,i);
XtManageChild(wLab);


i=0;
memset(app,' ',17);
memcpy(app,stazione[*is-1].descrizione,strlen((char*)stazione[*is-1].descrizione));
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
wLab=(Widget) XmCreateLabel(wbox,"lab",args,i);
XtManageChild(wLab);

/*
 Aggiunge una routine di refresh alla stazione creata
 */

if(add_refresh((XtCallbackProc)tr1_refresh,(void*)*is-1)==-1)
	printf("\n errore : refresh non aggiunto");

}



static void tr1_edit_text_callback(w,info,str)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *str;
{

if (str->set)  tr1_init_text_callback(w,info,str);
						/* set bottone */
else           tr1_fine_text_callback(w,info,str); /* reset */
}


static void tr1_init_text_callback(w,info,str)
Widget w;
caddr_t info;
XmAnyCallbackStruct *str;
{
int slstaz;
int is;
is=(int)info;
        XmTextSetEditable(wstaz[is].w[k_text],True);
        XmTextSetString(wstaz[is].w[k_text],"");
}


static void tr1_fine_text_callback(w,info,str)
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
int rltarget;
int rlaum;
int rldim;
is=(int)info;

if(XmTextGetEditable(wstaz[is].w[k_text])==False) return;
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
valfin=valore/stazione[is].scalam;
ipunti=stazione[is].target[0];
imodel=stazione[is].target[1];
tipo=STEP;
iret=g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
XtFree(pstring);
XmTextSetEditable(wstaz[is].w[k_text],False);
}

         
static void tr1_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int is= (int)info;
del_refresh((void*)is);
}

static void tr1_draw1_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int i;
int xpos;
char app[20]; /* stringa di appoggio */
int is= (int)info;
Window win = str->window;


/*************
memcpy(app,stazione[is].descrizione[0],17);
app[17]=0;
i=16;
while((app[i]==' ' || app[i]==0) && i>0)  i--;
xpos=(tr1_l-font_width*i)/2;
XDrawString(display,win,gc[0],xpos,(font_height*3+font_height/2+1),
            app,strlen(app));
memcpy(app,stazione[is].umis,6);
app[6]=0;
i=6;
while((app[i]==' ' || app[i]==0) && i>0)  i--;
xpos=(tr1_l-font_width*i)/2;
XDrawString(display,win,gc[0],xpos,(font_height*3)-font_height/2+2,
            app,strlen(app));
*************/
}
                                   
                  

/*
 Pseudo-Callback di refresh della stazione
 */

static void tr1_refresh(info)
caddr_t info;
{
char app[10];
int is;
int i;
int pos_ago;
float valfin;
char *pstr;
int ipunti,imodel,iret;
int tipo; /* tipo di perturbazione */
/* variabili raccolte dall'area di memoria shared */
float valore,delta;
float *pstato; /* puntatore floating di appoggio */
is=(int)info;
/*
 raccoglie i valori delle variabili dall'area di memoria shared
 */
valore=estr_sh(stazione[is].target[0],0);
valore=(valore) * stazione[is].scalam;
/*
 setta il nuovo valore nell'indicatore digitale
*/
sprintf(app,"%7.3f",valore);
pstr=(char *) XmTextGetString(wstaz[is].w[k_text]);
if(strcmp(pstr,app)!=0 && XmTextGetEditable(wstaz[is].w[k_text])==False )
        {
			XmTextSetString(wstaz[is].w[k_text],app);
        }
XtFree(pstr);
}





