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
static char SccsID[] = "@(#)monit_stat.c	5.1\t11/7/95";
/*
   modulo monit_stat.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)monit_stat.c	5.1
*/
# include <math.h>
# include <stdio.h>
# include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>
#include <X11/Xlib.h>
#include <string.h>
#include <Xm/DrawingA.h>
# include "sim_param.h"         /* parametri generali LEGO           */
# include "sim_types.h"
# include "monit.h"
# include "monituil.h"
# include "dispatcher.h"
# include <Rt/RtMemory.h>


extern int nmod;

extern char *ind_sh_top;     /* puntatore inizio shared memory sh_var */

extern Display *display;
extern GC gc[5];

extern STATISTICA_SKED statistica_sked;

extern XtIntervalId timer_id,timer_stat;

extern Widget toplevel_widget,
		main_window_widget,
		widget_array[MAX_WIDGETS];

char   *nome_modello (char *, int);



int monit_stat(w, tag, str)
    Widget w;
    int *tag;
    XmDrawingAreaCallbackStruct *str;
{
float ret, pf;
float cpu_tot;
int k;
char testo[50];
int riga = 10;
Window finestra_stat=str->window;
int altezza=25;
int larghezza=300;
float massimo = 0;
float totale = 0;
float fattore =1;


/*trova il valore massimo di occupazione della CPU */
for( k = 1 ; k <= nmod ; k++)
    {
    pf = statistica_sked.tempi_task[k-1];
    totale=totale+pf;
    if (pf>massimo) massimo=pf;
    }
fattore=1;

/* disegna l'istogramma */

for( k = 1 ; k <= nmod ; k++)
    {
    pf = statistica_sked.tempi_task[k-1]*fattore;
    	sprintf(testo,"%s->%3.1f",nome_modello(ind_sh_top,k),pf);
    	XDrawString(display,finestra_stat,gc[1],3,
          (int)(k*altezza)-5,testo,strlen(testo));
	if(massimo!=0)
    	XFillRectangle(display,finestra_stat,gc[4],
	  100,(k-1)*altezza+5,
	  (int)(pf*(larghezza-100)/massimo),altezza-5);
    }

cpu_tot = statistica_sked.tempo_cpu*fattore;
sprintf(testo,"tot. -> %4.0f",cpu_tot);

/* se il tempo di occcupazione della CPU e' maggiore tel tempo reale,
   ovvero la simulazione e' in riterdo viene disegnata una barra rossa */
/*
  
if (cpu_tot<=100)
    XFillRectangle(display,finestra_stat,gc[0],
        100,(int)((nmod+0.5)*altezza/(nmod+2)),
        (int)(cpu_tot*(larghezza-100)/100),(int)(1.5*altezza/(nmod+2))-1);
else
    XFillRectangle(display,finestra_stat,gc[2],
        100,(int)((nmod+0.5)*altezza/(nmod+2)),
        (int)(cpu_tot*(larghezza-100)/100),(int)(1.5*altezza/(nmod+2))-1);
*/

/*
XDrawString(display,finestra_stat,gc[2],3,
          (int)((nmod+2)*altezza/(nmod+2))-8,testo,strlen(testo));
*/
XDrawLine(display,finestra_stat,gc[1],99,0,99,25*nmod+5);
XDrawLine(display,finestra_stat,gc[1],98,0,98,25*nmod+5);
/*
XDrawLine(display,finestra_stat,gc[1],0,(int)((nmod+0.5)*altezza/(nmod+2)),
             larghezza,(int)((nmod+0.5)*altezza/(nmod+2)));
*/
};

/*
  refresh_stat
    si occupa di aggiornare periodicamente i valori riportati nella
    finestra della statistica
*/
void refresh_stat(client_data,id)
caddr_t client_data;
XtIntervalId *id;
{
XClearArea(display,XtWindow(widget_array[k_statistica_window]),0,0,0,0,True);
timer_stat=XtAppAddTimeOut(XtWidgetToApplicationContext(main_window_widget),
               (unsigned long)TM_REF_STAT,refresh_stat,NULL);
}
