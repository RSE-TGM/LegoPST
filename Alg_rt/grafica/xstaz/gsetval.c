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
static char SccsID[] = "@(#)gsetval.c	1.5\t3/23/95";
/*
   modulo gsetval.c
   tipo 
   release 1.5
   data 3/23/95
   reserved @(#)gsetval.c	1.5
*/
/*
	contiene le routine di gestione degli oggetti di tipo  SET_VALORE
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>
#include <Xm/DrawingA.h>
#include "sim_param.h"
#include "xstaz.h"
#include "compstaz.h"

/*
  tabelle comuni utilizzate per tutte le stazioni
*/
extern Arg args[];
extern S_PAGINA *pagina;
extern S_STAZIONE *stazione;
extern S_PAGVIS pagvis[MAX3_PAG];
extern TIPI_NEWSTAZ new_staz[];
extern DATI_REFRESH trefr_newstaz[];


extern Display *display;
extern int screen_num;
extern XColor excolor_new[],excolor_blink[],sfondo_staz;
extern int font_height;
extern int font_ascent;
extern int font_width;
extern XmFontList fontlist;
extern  GC gc[];

extern DATI_REFRESH *occupa_trefr();
extern DATI_REFRESH *occupa_tredraw();
void setval_del_callback();
void setval_refresh();
void init_text_callback();
void fine_text_callback();
void setval_press();


float estr_sh();


void crsetval(istaz,wbox,p_ogg,p_r02)
int	istaz;		/* indice stazione */
Widget	wbox;		/* indice widget padre */
OGGETTO	*p_ogg;		/* puntatore oggetto in tabella new_staz */
TIPO_SET_VALORE *p_r02;	/* puntatore oggetto in tabella stazioni */
{
int i, j, xlab, width, height, wborder, wfs;
Widget wsetval,wlab,wtext,wbutton;
DATI_REFRESH *p_refr;
char aux[10];
float val;

i=0;
XtSetArg(args[i],XmNx,p_ogg->x);i++;
XtSetArg(args[i],XmNy,p_ogg->y);i++;
XtSetArg(args[i],XmNwidth,W_SETVALORE);i++;
XtSetArg(args[i],XmNheight,H_SETVALORE);i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNborderWidth,0); i++;
XtSetArg(args[i],XmNresizePolicy,XmRESIZE_NONE);i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel);i++;
wsetval=(Widget) XmCreateDrawingArea(wbox,"draw_set",args,i);
XtManageChild(wsetval);

/*
        inserisce il riferimento per l'oggetto nella tabella
        trefr_newstaz
*/
        p_refr= (DATI_REFRESH *) occupa_trefr(wsetval,p_r02);
        if (p_refr ==  NULL )
                exit(puts("gsetval : troppi oggetti creati "));
	XtAddCallback(wsetval,XmNdestroyCallback,setval_del_callback,p_refr);
/*
	aggiunge il toggle_button per l' input del valore
*/
i=0;
XtSetArg(args[i],XmNx,0); i++;
XtSetArg(args[i],XmNy,0); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNspacing,0); i++;
XtSetArg(args[i],XmNbackground,excolor_new[BIANCO].pixel);i++;
wbutton=XmCreateToggleButton(wsetval,"text",args,i);
XtAddCallback(wbutton,XmNvalueChangedCallback,setval_press,p_refr);
XtManageChild(wbutton);

/*
	aggiunge il campo text 
*/

i=0;
XtSetArg(args[i],XmNx,15); i++;
XtSetArg(args[i],XmNy,0); i++;
XtSetArg(args[i],XmNmaxLength,10); i++;
XtSetArg(args[i],XmNborderWidth,1); i++;
XtSetArg(args[i],XmNresizeHeight,False); i++;
XtSetArg(args[i],XmNresizeWidth,False); i++;
XtSetArg(args[i],XmNwidth,55); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
XtSetArg(args[i],XmNeditMode,XmSINGLE_LINE_EDIT); i++;
XtSetArg(args[i],XmNscrollHorizontal,True); i++;
XtSetArg(args[i],XmNscrollLeftSide,True); i++;
XtSetArg(args[i],XmNshadowThickness,0); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel);i++;
wtext= XmCreateText(wsetval,"text",args,i);
XmTextSetEditable(wtext,False);
XtManageChild(wtext);

i=0;
XtSetArg(args[i],XmNx,75); i++;
XtSetArg(args[i],XmNy,2); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR(p_r02->etic,XmSTRING_DEFAULT_CHARSET)); i++;
XtSetArg(args[i],XmNwidth,25); i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNmarginWidth,0); i++;
XtSetArg(args[i],XmNmarginHeight,0); i++;
XtSetArg(args[i],XmNalignment,XmALIGNMENT_BEGINNING); i++;
XtSetArg(args[i],XmNfontList,fontlist); i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel);i++;
wlab=XmCreateLabel(wsetval,"lab",args,i);
XtManageChild(wlab);

/*
        aggiunge la callbacks  di refresh 
*/

        if(add_refresh((XtCallbackProc)setval_refresh,(caddr_t)p_refr)==-1)
        {
                libera_trefr(p_refr);
                printf("\n errore : refresh non aggiunto");
        }
}


void setval_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
del_refresh((void*)info);
libera_trefr((DATI_REFRESH *)info);
}


void setval_refresh(info)
caddr_t info;
{
DATI_REFRESH *p_refr;
TIPO_SET_VALORE *p_r02;
float valore;
char app[20];
WidgetList listw;
Widget wbutton,wedit;
int i,stato_inib;

p_refr=(DATI_REFRESH *) info;
p_r02= (TIPO_SET_VALORE *) p_refr->pdati;

get_something(p_refr->w,XmNchildren,(void*) &listw);
wbutton=(Widget)(listw[0]);
wedit=(Widget)(listw[1]);

/*
   raccoglie il valore di inibizione e setta il bottone   
   se inibizione non e' definita ( indice = -1 ) setta sempre l'
abilitazione
*/
stato_inib = (int) estr_sh(p_r02->inibiz,0);
if (stato_inib)
{
  XmToggleButtonSetState(wbutton,False,False);
  XtSetSensitive(wbutton,False);
}
else
{
  XtSetSensitive(wbutton,True);
}
                         

/* raccoglie il valore della variabile e la moltiplica per il fattore 
   di scalamento  */

if(XmTextGetEditable(wedit)==False)
{
	valore = estr_sh(p_r02->input,0);
	valore = valore * p_r02->scalam;
	valore = valore + p_r02->offset;
	sprintf(app,"%10.3f",valore);
	XmTextSetString(wedit,app);
}
}

void setval_press(w,info,str)
Widget w;
caddr_t info;
XmToggleButtonCallbackStruct *str;
{
DATI_REFRESH *p_refr;
TIPO_SET_VALORE *p_r02;
p_refr=(DATI_REFRESH *) info;
p_r02= (TIPO_SET_VALORE *) p_refr->pdati;

/* se l' indice del comando non e' abilitato non abilita l' input sul
text */
if (p_r02->out.indice == -1 ) return;
if (str->set)  init_text_callback(w,info,str); /* set bottone */
else           fine_text_callback(w,info,str); /* reset */
}
                                                     

void init_text_callback(w,info,str)
Widget w;
caddr_t info;
XmAnyCallbackStruct *str;
{
WidgetList listw;
Widget wtext;


get_something(XtParent(w),XmNchildren,(void*) &listw);
wtext=(Widget)(listw[1]);
XmTextSetEditable(wtext,True);
XmTextSetString(wtext,"");

}


void fine_text_callback(w,info,str)
Widget w;
caddr_t info;
XmAnyCallbackStruct *str;
{
float valore;
char *pstring;
WidgetList listw;
Widget wtext;
DATI_REFRESH *p_refr;
TIPO_SET_VALORE *p_r02;
int iret;

p_refr=(DATI_REFRESH *) info;
p_r02= (TIPO_SET_VALORE *) p_refr->pdati;
get_something(XtParent(w),XmNchildren,(void*) &listw);
wtext=(Widget)(listw[1]);
/*
 se il testo non e' editabile non accetto il valore attuale
*/

if(XmTextGetEditable(wtext)==False) return;

pstring=(char *) XmTextGetString(wtext);
iret=sscanf(pstring,"%f",&valore);
/*
 in caso di valore inserito non valido annulla l'operazione
 */
if(iret!=1)
        {
        XtFree(pstring);
        XmTextSetEditable(wtext,False);
        return;
        }

/*
   invia la perturbazione a STEP e la perturbazione di tipo impulso
   per rltarget 
*/

if  (p_r02->out.tipo_pert == STEP)
{
	valore = (valore - p_r02->offset)/p_r02->scalam;
	iret=g_perturba(0,p_r02->out.tipo_pert,valore,0.,0,p_r02->out.nmod,p_r02->out.indice);
	if  (p_r02->rltarget.tipo_pert == IMPULSO)
	{
	iret=g_perturba(0,p_r02->rltarget.tipo_pert,valore,0.,0,p_r02->rltarget.nmod,p_r02->rltarget.indice);
	}
	else
	printf("perturbazione rltarget  non inviata perche' non di tipo IMPULSO\n ");
}
else
printf("perturbazione valore  non inviata perche' non di tipo STEP\n ");

XtFree(pstring);
XmTextSetString(wtext,"");
XmTextSetEditable(wtext,False);

}

