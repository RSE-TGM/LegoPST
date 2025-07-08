/**********************************************************************
*
*       C Source:               run_time_graf.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Mon Nov  2 14:03:45 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: run_time_graf.c-4 %  (%full_filespec: run_time_graf.c-4:csrc:1 %)";
#endif
/*
   modulo run_time_graf.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)run_time_graf.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)run_time_graf.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <stdlib.h>                             
#include <string.h>                           
#include <math.h>     
#if defined AIX || defined ULTRIX
#include <sys/types.h>     
#endif
#if defined VMS
#include <types.h>
#endif
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/ScrollBar.h>
#include <Xm/MessageB.h>
#include <Mrm/MrmPublic.h>         

#include "libutilx.h"

#include "sim_param.h"
#include <Xl/XlP.h>
#include <Xl/XlGraficoP.h>

/*
 * Global data
 */
extern S_XLGRAFICO *pXlGraf;
extern S_UNI_MIS uni_mis[]; /* tabella che descrive tutte le unita'
                                  di misura  */
extern int num_umis;
extern char *no_file_sel;

extern void init_application ();
extern Widget create_S_MAIN_WINDOW_RUNTIME();

extern int set_scala(int,S_XLGRAFICO *);
extern void set_scala_unica(S_XLGRAFICO *);
extern  void crea_sfondo(Widget,Dimension,Dimension);
extern  void formatta(char*,float);
extern void prep_draw(float,float,S_MIN_MAX *,Widget);
extern void draw_grid(Window);
extern Widget	create_S_MAIN_WINDOW_PLOT(Widget,S_XLGRAFICO *);
extern int cerca_umis(char*,int);
extern int set_ordinate(int,S_XLGRAFICO *);
extern void GetPuntXlGrafico (Widget);



/************************************************
 Routine di inizializzazione del grafics che legge da database
*************************************************/
unsigned int R_inizializza1(argc,argv)
unsigned int argc;
char *argv[];
{
int i;

freeza = 1;

/* Gestione dell'argomento addizionale: nome del file  nomi variabili */
if (argc > 1)
  {
  if(argc>7)
    XlWarning ("XlGrafico","R_inizializza1","Errore command line");

  printf("Numero degli argomenti passati: %d\n",argc);
  for(i=1;i<argc;i++)
    printf("Argomento[%d]: %s\n",i,argv[i]);
  }

init_application();

init_umis();
num_umis=cerca_num_umis();
umis_defsel= (int*) calloc (num_umis, sizeof (int));
x_codumis=(XmString *)XtCalloc(num_umis+1, sizeof(XmString));

/*
 *  Ricava i valori delle variabili necessarie per la creazione
 *  dei Grafic Context (GC)
 */
toplevel_widget= topLevel_set;
screen_num=DefaultScreen(display_set);
main_window_widget= topLevel_set;
}

/********************************************
********************************************/
unsigned int R_inizializza2(argc, argv, main_wid)
    unsigned int argc;                  /* Command line argument count. */
    char *argv[];                       /* Pointers to command line args. */
    Widget main_wid;
{
main_window_widget=main_wid;

/* Creazione cursore per wait */
cursor_wait=XCreateFontCursor(display_set,XC_watch);

/* crea le message boxes per visualizzazione errore e visualizzazione warning */
WidAttenzione(main_window_widget,"",CREA);
WidErrore(main_window_widget,"",CREA);

scala_unica = 0;
}

/************************************************
 Routine di creazioni del grafico come oggetto
*************************************************/
void CreateRUN_GRAF(wparent,argc,argv,InRunTime)
Widget wparent;
unsigned int argc;                  /* Command line argument count. */
char *argv[];                       /* Pointers to command line args. */
Boolean InRunTime;
{
Widget mainIface;
Dimension NumPix;

R_inizializza1(argc,argv);
mainIface = create_S_MAIN_WINDOW_RUNTIME (wparent,pXlGraf);

/*  Allocazione dinamica delle strutture necessarie solo in fase di RUNTIME  */
if (InRunTime)
  {
  /*  Allocazione del vettore dei punti per il grafico pari alla
        larghezza della finestra che lo deve contenere  */
  get_something (widget_array[k_form], XmNwidth, (void*) &NumPix);
  points[0] = (XPoint *)calloc (NumPix+1,sizeof(XPoint));
  points[1] = (XPoint *)calloc (NumPix+1,sizeof(XPoint));
  points[2] = (XPoint *)calloc (NumPix+1,sizeof(XPoint));
  points[3] = (XPoint *)calloc (NumPix+1,sizeof(XPoint));
  }

R_inizializza2(argc,argv,mainIface);
stato_zoom = ZOOM_END;
}

/************************************************
 Routine di creazioni del grafico come oggetto
*************************************************/
void CreatePLOT_GRAF(wparent,argc,argv,InRunTime)
Widget wparent;
unsigned int argc;                  /* Command line argument count. */
char *argv[];                       /* Pointers to command line args. */
Boolean InRunTime;
{
Widget mainIface;
Dimension NumPix;

R_inizializza1(argc,argv);
mainIface = create_S_MAIN_WINDOW_PLOT (wparent,pXlGraf);

/*  Allocazione dinamica delle strutture necessarie solo in fase di RUNTIME  */
if (InRunTime)
  {
  /*  Allocazione del vettore dei punti per il grafico pari alla
        larghezza della finestra che lo deve contenere  */
  get_something (widget_array[k_form], XmNwidth, (void*) &NumPix);
  points[0] = (XPoint *)calloc (NumPix+1,sizeof(XPoint));
  }

R_inizializza2(argc,argv,mainIface);
stato_zoom = ZOOM_END;
}

/*************************************************
*************************************************/
void R_load_variables(num_nomi,nome,Wdg)
int num_nomi;
char *nome[];
Widget Wdg;
{
XlGraficoWidget WW=(XlGraficoWidget)Wdg;
int i,j;
int indice;
int loaded=0; /* flag per indicare se e' stata caricata
                 almeno una variabile */
char *NomeVar;
S_GRAFICO *s;

pXlGraf = WW->grafico.p;

s= &sg;

for(i=0;i<4;i++)
  {
  if(i >= num_nomi)
    {
    x_sel_var[i]=XmStringCreateLtoR(descr_vuota,
                XmSTRING_DEFAULT_CHARSET);
    s->ind_mis[i]=(-1);
    }
  else
    {
    /*NomeVar = OlGetNameVar(Wdg,nome[i],i);*/
/*
Inserisco nomi variabili come nome[i]. Questa modifica e' stata fatta per
per evitare che la topologia venga caricata in shared memory quando lancio mmi.
La Topologia riguarderebbe solo le variabili di tipo SIMULATORE (origin==0).
In questo modo pero' non si ha a disposizione la descrizione della variabile.

*/
    NomeVar= XtCalloc(((MAX_LUN_NOME_VAR>MAX_LUN_NOME_VARS)?MAX_LUN_NOME_VAR:MAX_LUN_NOME_VARS),sizeof(char));
    if((nome[i])&&(NomeVar != NULL))
	strcpy(NomeVar,nome[i]);
    if (NomeVar != NULL)
      {
      x_sel_var[i]=XmStringCreateLtoR(NomeVar,
                XmSTRING_DEFAULT_CHARSET);
      s->ind_mis[i] = i;
      }
    }
  s->x_descr_mis[i]=x_sel_var[i];
  if(s->ind_mis[i]!=-1)
    {
    loaded=1;
    if ( NomeVar!=NULL )
      {
      if ( strlen(WW->grafico.descr_var[i])!=0 )
	{
        s->ind_umis[i]=cerca_umis(NomeVar,0);
	XtFree (NomeVar);
	NomeVar = XtNewString (WW->grafico.descr_var[i]);
        }
      strncpy(s->descr_mis[i],NomeVar,LUN_SIMB);
      if(s->ind_umis[i]==-1)
        {
         s->ind_umis[i]=num_umis-1;
        }
      XtFree (NomeVar);
      }
    /*
      e' settato l'autoscaling: forza la ricerca di minimo e massimo
      modificando il valore del massimo.
    */
    s->sel_min_max[i].max=s->var_min_max[i].max+1;
    s->umis_sel[i]=uni_mis[s->ind_umis[i]].sel;
    set_scala(i,pXlGraf);
    }
  else /* caso di misura non valida o eliminata */
    {
    strcpy(s->descr_mis[i],descr_vuota);
    set_ordinate(i,pXlGraf); /* setta a blank le ordinate visualizzate */
    }
  }

if(scala_unica && loaded)
        set_scala_unica(pXlGraf);
t_old=0.0;
freeza=0;
}

void R_draw_proc(w, tag, str)
Widget w;
int *tag;
XmDrawingAreaCallbackStruct *str;
{
int ord_unica;
char appstr[50];
int t_ore,t_minuti,t_secondi;
Window win = str->window;
int widget_num = (int)tag;              /* Convert tag to widget number. */
int i,k;
int numis,selumis;
int zoom;
int y_line,x_line;
float f_pix;
double fraz,intero;
int x_pix,x_pixprec; /* posizione in pixel del tempo sull-asse delle ascisse */
int y_pix;  /* posizione in pixel delle ordinate */
int j,jprec; /* scorron il buffer circolare dati */
int indice; /* indice della misura da visualizzare */
XlGraficoWidget wid = (XlGraficoWidget)XtParent(XtParent(w));

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

switch(widget_num)
  {
  /****/
  /****  Prepara il grafico  ****/
  /****/
  case k_draw1:
  get_something(w,XmNwidth, (void*) &draw_width);
  get_something(w,XmNheight, (void*) &draw_height);
  if(resize)
    {
    get_something(w,XmNwidth, (void*) &draw_width);
    get_something(w,XmNheight, (void*) &draw_height);
    resize=0;
    crea_sfondo(w,draw_width,draw_height);
    }
  draw_height-=2;

  if (wid->xlmanager.config == True) return;

  if(stato_zoom==ZOOM_END)
    prep_draw(t_iniziale,t_finale,&sg.sel_min_max[0],w);
  else
    prep_draw(tz_iniziale,tz_finale,&sg.sel_min_max[0],w);

  for(i=0;i<4;i++)
    {
    indice=sg.ind_mis[i];
    if(indice == -1) continue;
    if(HC_on)
      {
      XDrawLines(display_set,win,gc2[i],
               points[i],npp[i],CoordModeOrigin);
      draw_grid(win);
      }
    else
      XDrawLines(display_set,win,gc[i],
               points[i],npp[i],CoordModeOrigin);
    }
  break;

  /****/
  /****  Scrive le stringhe delle quattro misure selezionate  ****/
  /****/
  case k_mis1:
  for(i=0;i<4;i++)
    {
    y_line= font_height*i+font_height/2;

    if(HC_on)
      {
      if(sg.ind_mis[i]==-1) continue;
      XDrawLine(display_set,win,gc2[i],5,y_line,25,y_line);
      XDrawString(display_set,win,gc2[0],30,font_height*(i+1),sg.descr_mis[i],
                                strlen(sg.descr_mis[i]));
      }
    else
      {
      if(sg.ind_mis[i]==-1) continue;
      XDrawLine(display_set,win,gc[i],5,y_line,25,y_line);
      XDrawString(display_set,win,gc[i],30,font_height*(i+1),sg.descr_mis[i],
                                strlen(sg.descr_mis[i]));
      }
    }
  break;

  /****/
  /****  Scrive il nome del file attualmente aperto  ****/
  /****/
  case k_draw0:
  if(HC_on)
    XDrawString(display_set,win,gc2[1],10,font_height,file_vis,strlen(file_vis));
  else
    XDrawString(display_set,win,gc[1],10,font_height,file_vis,strlen(file_vis));
  break;

  /****/
  /****  Scrive i valori delle quattro misure selezionate preceduti dalla
	relativa unita' di misura  ****/
  /****/
  case k_val1:
  for(i=0;i<4;i++)
    {
    y_line= font_height*i+font_height/2;
    selumis=sg.umis_sel[i];
    numis=sg.ind_umis[i];

    if(sg.ind_mis[i]!=-1)
      {
      formatta(appstr,sg.ultimo[i]);
      if(HC_on)
	{
	XDrawString(display_set,win,gc2[i],5,
                    font_height*(i+1),uni_mis[numis].codm[selumis],
                    strlen(uni_mis[numis].codm[selumis]));
        XDrawString(display_set,win,gc2[0],45,font_height*(i+1),
                appstr,strlen(appstr));
	}
      else
	{
        XDrawString(display_set,win,gc[i],5,
                    font_height*(i+1),uni_mis[numis].codm[selumis],
                    strlen(uni_mis[numis].codm[selumis]));
        XDrawString(display_set,win,gc[i],45,font_height*(i+1),
                appstr,strlen(appstr));
	}
      }
    else
      sprintf(appstr,"        ");
    }
  break;

  /****/
  /****  Setta la label del tempo corrente  ****/
  /****/
  case k_tempo:
  y_line=font_height+2;
  if(tempo_sec)
    {
    sprintf(appstr,"sec. %.2f",t_ultimo);
    }
  else
    {
    t_ore=((((int)t_ultimo)%86400)/60)/60;
    t_minuti=((((int)t_ultimo)%86400)/60)%60;
    t_secondi=(((int)t_ultimo)%86400)%60;
    fraz=modf(t_ultimo,&intero);
            fraz=fraz * 1000;
    if(t_ore==0)
      sprintf(appstr," %.2d:%.2d:%.3d",t_minuti,t_secondi,(int)fraz);
    else
      sprintf(appstr,"hh.%.2d:%.2d:%.2d",t_ore,t_minuti,t_secondi);
    }
  if(HC_on)
    XDrawString(display_set,win,gc2[0],10,y_line,appstr,strlen(appstr));
  else
    XDrawString(display_set,win,gc[3],10,y_line,appstr,strlen(appstr));
  break;

  /****/
  /****  Scrive i valori delle misure a fianco della griglia  ****/
  /****/
  case k_ord1:
  get_something(widget_array[k_ord1],XmNheight, (void*) &ord_height);
  for(i=1;i<4;i++)
    {
    if(strcmp(sg.str_ord[0][i],"         ")==0)
        ord_unica=1;
    else
      {
      ord_unica=0;
      break;
      }
    }

  for(k=0;k<5;k++)
    {
    /*  Se la dimensione della window e' minore di 170 pixel vengono
        saltate le label delle misure con indice 1,2,3.
        Se la dimensione della window e' compresa tra 170 e 280 pixel
        vengono saltate le label dei tempi con indice 1,3.
        Se la dimensione della window e' maggiore di 280 pixel vengono
        visualizzate tutte le label dei tempi  */
    if ( (ord_height>280)||((ord_height<=170)&&((k==0)||(k==4)))||
        (ord_height>170 && ord_height<=280 && ((k==0)||(k==2)||(k==4))) )
      {
      y_line=k*((ord_height-5*font_height)/4);
      for(i=0;i<4;i++)
        {
        if(ord_unica)
          y_line+=(font_height*3);
        else
          y_line+=font_height;
        if(sg.ind_mis[i]==-1) continue;
        if(HC_on)
          XDrawString(display_set,win,gc2[0],20,y_line,
                                        sg.str_ord[k][i],
                                        strlen(sg.str_ord[k][i]));
        else
          XDrawString(display_set,win,gc[i],20,y_line,
                                       sg.str_ord[k][i],
                                        strlen(sg.str_ord[k][i]));
        }
      }
    }
  break;

  /****/
  /**** Setta i valori dei tempi nella barra sottostante la griglia ****/
  /****/
  case k_tim1:
  get_something(w,XmNwidth, (void*) &draw_width);
  for(k=0;k<7;k++)
    {
    zoom=sg.zoom;
    x_line=k*(draw_width/6);
    /*  correzioni per centrare meglio i valori estremi   */
    if(k==0) x_line+=5;
    if(k==6) x_line-=35;
    /*  Se la dimensione della window e' minore di 180 pixel vengono
        saltate le label dei tempi con indice 1,2,3,4,5.
        Se la dimensione della window e' compresa tra 180 e 420 pixel
        vengono saltate le label dei tempi con indice 1,2,4,5.
        Se la dimensione della window e' maggiore di 420 pixel vengono
        visualizzate tutte le label dei tempi  */
    if ( (draw_width>420)||((draw_width<=180)&&((k==0)||(k==6)))||
        (draw_width>180 && draw_width<=420 && ((k==0)||(k==3)||(k==6))) )
      {
      if(tempo_sec)
        {
        if(HC_on)
          XDrawString(display_set,win,gc2[0],x_line,font_height+3,
                     str_tims[k],strlen(str_tims[k]));
        else
          XDrawString(display_set,win,gc[3],x_line,font_height+3,
                     str_tims[k],strlen(str_tims[k]));
        }
      else
        {
        if(HC_on)
          XDrawString(display_set,win,gc2[0],x_line,font_height+3,
                 str_tim[k],strlen(str_tim[k]));
        else
          XDrawString(display_set,win,gc[3],x_line,font_height+3,
                str_tim[k],strlen(str_tim[k]));
        }
      }
    }
  break;
  } 
}
