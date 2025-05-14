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
static char SccsID[] = "@(#)gdisplay.c	1.5\t3/23/95";
/*
   modulo gdisplay.c
   tipo 
   release 1.5
   data 3/23/95
   reserved @(#)gdisplay.c	1.5
*/
/*
	contiene le routine di gestione degli oggetti di tipo DISPLAY
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>

#include "sim_param.h"
#include "xstaz.h"
#include "compstaz.h"
#include "LedP.h"
#include "Led.h"


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
extern XColor excolor_new[],excolor_blink[],sfondo_staz;
extern int font_height;
extern int font_ascent;
extern int font_width;
extern XmFontList fontlist,fontlist_big;

extern	int  stato_blink;

extern DATI_REFRESH *occupa_trefr();
extern DATI_REFRESH *occupa_tredraw();
void display_refresh();
void display_del_callback();

float estr_sh();

void crdisplay(istaz,wbox,p_ogg,p_r02)
int	istaz;		/* indice stazione */
Widget	wbox;		/* indice widget padre */
OGGETTO	*p_ogg;		/* puntatore oggetto in tabella new_staz */
TIPO_DISPLAY *p_r02;	/* puntatore oggetto in tabella stazioni */
{
int i;
Widget wdisplay;
DATI_REFRESH *p_refr;
char app[10];
/*********
XFontStruct *font;
XmFontList fontlist_big = NULL;
#if defined  AIX
char *font_name = "Rom22";

#elif defined  UNIX
char *font_name = "times_roman18";

#else
char *font_name = "fixed";

#endif



if((font = XLoadQueryFont(display,font_name)) == NULL)
        {
        printf("\ngdisplay: Cannot open font");
        }
fontlist_big = XmFontListCreate (font,(XmStringCharSet) XmSTRING_DEFAULT_CHARSET);
if (fontlist_big == NULL) printf("FONTLIST NULL");
******/

/* crea il display */


i=0;
XtSetArg(args[i],XmNx,p_ogg->x);i++;
XtSetArg(args[i],XmNy,p_ogg->y);i++;
XtSetArg(args[i],XmNwidth,W_DISPLAY);i++;
XtSetArg(args[i],XmNheight,H_DISPLAY);i++;
XtSetArg(args[i],XmNbackground,sfondo_staz.pixel); i++;
XtSetArg(args[i],XmNfontList,fontlist_big);i++;
XtSetArg(args[i],XmNrecomputeSize,False); i++;
XtSetArg(args[i],XmNborderWidth,2); i++;
XtSetArg(args[i],XmNalignment,XmALIGNMENT_END); i++;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("----",XmSTRING_DEFAULT_CHARSET)); i++;
wdisplay=XmCreateLabel(wbox,"lab",args,i);
XtManageChild(wdisplay);

/*
        inserisce il riferimento per l'oggetto nella tabella
        trefr_newstaz
*/
        p_refr= (DATI_REFRESH *) occupa_trefr(wdisplay,p_r02);
        if (p_refr ==  NULL )
		exit(puts("gdisplay : troppi oggetti creati "));
/*
	aggiunge la callback di cancellazione dell 'oggetto
*/
XtAddCallback(wdisplay,XmNdestroyCallback,display_del_callback,p_refr);

/* 
	aggiunge la callbacks  di refresh del display
*/

	if(add_refresh((XtCallbackProc)display_refresh,(caddr_t)p_refr)==-1)
	{
		libera_trefr(p_refr);
       		printf("\n errore : refresh non aggiunto");
	}
}

void display_del_callback(w,info,str)
Widget w;
caddr_t info;
XmDrawingAreaCallbackStruct *str;
{
del_refresh((void*)info);
libera_trefr((DATI_REFRESH *)info);
}

void display_refresh(info)
caddr_t info;
{
DATI_REFRESH *p_refr;
TIPO_DISPLAY *p_r02;
int i;
float valore;
char app[10];
XmString c_str1,c_str2;

p_refr=(DATI_REFRESH *) info;
p_r02= (TIPO_DISPLAY *) p_refr->pdati;

/* raccoglie il  valore della variabile  di  INPUT e la visualizzq 
  se e' variato ripestto al precedente 
*/

valore = estr_sh(p_r02->input,0);
sprintf(app,"%7.2f",valore);
/*sprintf(app,"%4d",valore);*/
c_str1=XmStringCreateLtoR(app,XmSTRING_DEFAULT_CHARSET);

/* legge il valore visualizzato precedentemente */

XtSetArg(args[0],XmNlabelString,&c_str2);
XtGetValues(p_refr->w,args,1);
if(XmStringCompare(c_str1,c_str2)==0)
        {
	i=0;
	XtSetArg(args[i],XmNlabelString,c_str1);i++;
	XtSetValues(p_refr->w,args,i); 
	}
XtFree((char*)c_str1);
XtFree((char*)c_str2);
}

