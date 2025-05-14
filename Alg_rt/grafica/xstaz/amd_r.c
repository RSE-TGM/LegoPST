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
static char SccsID[] = "@(#)amd_r.c	1.4\t3/23/95";
/*
   modulo amd_r.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)amd_r.c	1.4
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
 parametri geometrici propri della stazione AMD: vengono utilizzati
 nelle callback di redraw.
*/
int amd_l;       /* larghezza della stazione */
int amd_draw_h;  /* altezza dell'area di visualizzazione strumento
                    analogico  */

/*
  indici all'interno della struttura dei widget 
*/
#define k_toggle1  0
#define k_toggle2  1
#define k_toggle3  2
#define k_text     3
#define k_draw1    4

static void amd_draw1_callback(); /* callback della area per descrizione
                                     variabile  */
static void amd_del_callback();
static void amd_draw2_callback(); /* callback della area di disegno testi */
static void amd_btn2_callback();  /* callback del bottone centrale */
static void amd_btn1_callback();  /* callback del bottone sinistro */
static void amd_btn3_callback();  /* callback del bottone destro */
static void amd_refresh();        /* callback da chiamarsi scaduto il
                                      timeout di refresh */

float estr_sh();

void staz_amd_r(flag,is,ip3)
int *flag;   /* flag */
int *is;   /* indice stazione associata */
int *ip3;  /* indice nel descrittore pagine visualizzate */
{
char app[100],*px;
int val;
int i;
XmString c_str;
Widget wDraw,wText,wSep,wLab,wDraw1;  /* area per disegno */
Widget wRadioBox,wToggle;
Widget wToggle1,wToggle2,wToggle3;
int htot; /* altezza totale */
int hdraw,xdraw,ydraw;       /* altezza e posizione area di disegno */
int hlabel,xlabel,ylabel;    /* altezza e posizione area testo */
int hbutton,xbutton,ybutton; /* altezzza area bottoni */
int ltot;    /* lunghezza totale */
/*
 Calcola i parametri geometrici della stazione
 */
htot=(stazione[*is-1].posiy1-stazione[*is-1].posiy0)*60;
ltot=(stazione[*is-1].posix1-stazione[*is-1].posix0)*60;
hdraw=htot/2;
hlabel=hdraw/3;
hbutton=hlabel;
amd_l=ltot-2;
amd_draw_h=hdraw;
xdraw=stazione[*is-1].posix0*62+2;
ydraw=stazione[*is-1].posiy0*62+3;
ydraw=pagvis[*ip3].height-ydraw-htot;
xlabel=0;
ylabel=0;
xbutton=0;
ybutton=hdraw+hlabel*2;

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
XtAddCallback(wDraw,XmNdestroyCallback,amd_del_callback,(void*)*is-1);
XtManageChild(wDraw);


                                                 
c_str=XmStringCreateLtoR("",XmSTRING_DEFAULT_CHARSET);
i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,0); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
XtSetArg(args[i],XmNborderWidth,0); i++;
XtSetArg(args[i],XmNlabelString,c_str); i++;
wDraw1=XmCreateLabel(wDraw,"draw",args,i);  
wstaz[*is-1].w[k_text]=wDraw1;
XtManageChild(wDraw1);
XtFree((char*)c_str);

i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hlabel); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNtopShadowColor,excolor[0].pixel); i++;
XtSetArg(args[i],XmNbottomShadowColor,excolor[0].pixel); i++;
wSep=XmCreateSeparator(wDraw,"sep",args,i);
XtManageChild(wSep);

i=0;
memset(app,' ',17);
memcpy(&app[5],stazione[*is-1].umis,strlen(stazione[*is-1].umis));
app[17]=0;
c_str=XmStringCreateLtoR(app,XmSTRING_DEFAULT_CHARSET);
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,ylabel+hlabel); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel+2); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNfontList,fontlist);
wLab=XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);

memset(app,' ',17);
memcpy(app,stazione[*is-1].mod[0],17);
app[17]=0;
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,ylabel+2+hlabel*2); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNfontList,fontlist);
wLab=XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);

i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hdraw); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNtopShadowColor,excolor[0].pixel); i++;
XtSetArg(args[i],XmNbottomShadowColor,excolor[0].pixel); i++;
wSep=XmCreateSeparator(wDraw,"sep",args,i);
XtManageChild(wSep);


memset(app,' ',17);
memcpy(app,stazione[*is-1].descrizione,strlen((const char *)stazione[*is-1].descrizione));
app[17]=0;
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hdraw+1); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNfontList,fontlist);
wLab=XmCreateLabel(wDraw,"lab",args,i);
XtManageChild(wLab);


app[0]=0;
px=strncat(app,stazione[*is-1].etic[0],4);
strcat(px,"  ");
strncat(px,stazione[*is-1].etic[1],4);
strcat(px,"  ");
strncat(px,stazione[*is-1].etic[2],4);
c_str=XmStringCreateLtoR(app,XmSTRING_DEFAULT_CHARSET);
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(app,
XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,hdraw+hlabel); i++;
XtSetArg(args[i],XmNwidth,ltot); i++;
XtSetArg(args[i],XmNheight,hlabel); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist);
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
wRadioBox=XmCreateRowColumn(wDraw,"radio",args,i);
XtManageChild(wRadioBox);


i=0;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("  ",XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNbackground,excolor[1].pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist);i++;
wToggle1=XmCreateToggleButton(wRadioBox,"",args,i);
wToggle2=XmCreateToggleButton(wRadioBox,"",args,i);
wToggle3=XmCreateToggleButton(wRadioBox,"",args,i);
wstaz[*is-1].w[k_toggle1]=wToggle1;
wstaz[*is-1].w[k_toggle2]=wToggle2;
wstaz[*is-1].w[k_toggle3]=wToggle3;
XtAddCallback(wToggle2,XmNvalueChangedCallback,amd_btn2_callback,(XtPointer)*is-1);
XtAddCallback(wToggle1,XmNvalueChangedCallback,amd_btn1_callback,(XtPointer)*is-1);
XtAddCallback(wToggle3,XmNvalueChangedCallback,amd_btn3_callback,(XtPointer)*is-1);
XtManageChild(wToggle1);
XtManageChild(wToggle2);
XtManageChild(wToggle3);
/*
 Aggiunge una routine di refresh alla stazione creata
 */
if(add_refresh((XtCallbackProc)amd_refresh,(void *)*is-1)==-1)
	printf("\n errore : refresh non aggiunto");

}

static void amd_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int is= (int)info;
del_refresh((void*)is);
}
       

         
static void amd_draw1_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
int i;
int xpos;
char app[20]; /* stringa di appoggio */
int is= (int)info;
Window win = str->window;
memcpy(app,stazione[is].mod[0],17);
app[17]=0;
i=16;
while((app[i]==' ' || app[i]==0) && i>0)  i--;
xpos=(amd_l-font_width*i)/2;
XDrawString(display,win,gc[0],xpos,(font_height*3+font_height/2+2),
            app,strlen(app));
memcpy(app,stazione[is].umis,6);
app[6]=0;
i=6;
while((app[i]==' ' || app[i]==0) && i>0)  i--;
xpos=(amd_l-font_width*i)/2;
XDrawString(display,win,gc[0],xpos,(font_height*3)-font_height/2+2,
            app,strlen(app));
}



static void amd_draw2_callback(w,info,str)
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

static void amd_btn2_callback(w,info,toggle)
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
if(stazione[is].incorso!=(-1) ||  slesamr!=rlstaz )
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

/*
 annulla l'eventuale richiesta di valore di target
*/
/************
valfin=0.0;
ipunti=stazione[is].rltarget[0];
imodel=stazione[is].rltarget[1];
g_perturba(0,tipo,valfin,0.,0,imodel,ipunti);
************/

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

static void amd_btn1_callback(w,info,toggle)
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
 Callback per il bottone 3
 */
static void amd_btn3_callback(w,info,toggle)
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
 Pseudo-Callback di refresh della stazione
 */
#define FISSO  1 /* stato dei bottoni definito */
#define WAIT   0 /* stato dei bottoni in transizione */

static void amd_refresh(info)
caddr_t info;
{
char app[10];
XmString c_str,c_str2;
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
        amd_set_button(stazione[is].incorso,WAIT,is);
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
                amd_set_button(slstaz,FISSO,is);
                }
        else
                {
                amd_set_button(!slstaz,WAIT,is);
                }
        }
/*
 setta il nuovo valore nell'indicatore digitale
*/
sprintf(app,"%7.3f",valore);
c_str=XmStringCreateLtoR(app,XmSTRING_DEFAULT_CHARSET);
XtSetArg(args[0],XmNlabelString,&c_str2);
XtGetValues(wstaz[is].w[k_text],args,1);
if(XmStringCompare(c_str,c_str2)==0)
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
	XtSetArg(args[i],XmNlabelString,c_str);  i++;
	XtSetValues(wstaz[is].w[k_text],args,i);
	}
/*XtFree(c_str);*/
}



/*
 setta lo stato dei bottoni in base allo stato della stazione 
*/
void amd_set_button(stato,tipo,is)
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
		}
	else  /* in attesa di risposta */
		{
/* setta lo sfondo del bottone di automatico  */
		XtSetArg(args[0],XmNforeground,excolor[2].pixel);
		XtSetValues(wstaz[is].w[k_toggle2],args,1);
		}
	}
}



