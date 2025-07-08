/**********************************************************************
*
*
*       C Source:               funzioni.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Fri Feb 14 17:51:47 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: funzioni.c-1_deb %  (%full_filespec: funzioni.c-1_deb:csrc:1 %)";
#endif

/*
   modulo funzioni.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)funzioni.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)funzioni.c	5.1\t11/13/95";
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
#include "UxXt.h"
        


#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/ScrollBar.h>
#include <Xm/MessageB.h>
#include <Mrm/MrmPublic.h> 
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/List.h>
#include <X11/Shell.h>


#include <libutilx.h>

#include <sim_param.h>
#include <Xl/XlP.h>
#include <Xl/XlManagerP.h>
#include <Xl/XlGrafico.h>
#include <Xl/XlGraficoP.h>

/*
 * Stringhe di errore utilizzate
 */
char * err_write_gruppi=
 "File per memorizzazione gruppi appartenente ad altro utente"; 
char * err_crea_gruppi=
 "Impossibile creare file per memorizzazione gruppi";
char * err_file_nonspec=
 "File non specificato";
char * err_file_nones=
 "File non esistente";
char * err_tfin_tiniz=
 "Tempo iniziale = tempo finale";

/*
 * Global data
 */
extern S_XLGRAFICO *pXlGraf;
extern S_UNI_MIS uni_mis[]; /* tabella che descrive tutte le unita'
                                  di misura  */

int num_umis; /* numero unita' di misura definite in uni_misc.dat*/
unsigned int find_pos;   /* posizione corrente nel find  */

char *no_file_sel="NESSUN FILE GRAFICI SELEZIONATO";

extern Widget create_nyi();
extern Widget create_selmis_dialog();
extern Widget create_selzoom_dialog();
extern Widget create_umis_dialog();
extern Widget create_fsca_dialog();
extern Widget create_memgr_dialog();
extern Widget create_selgr_dialog();
extern Widget create_defumis_dialog();
extern Widget create_dir_dialog();
extern int read_22datGR(char,S_XLGRAFICO*);
 void prep_str_timGR(float,float,Widget);
static int cerca_stringa(char*,char **);
extern int rew_dati(S_XLGRAFICO *);
 void set_scala_unica(S_XLGRAFICO *);
 void formatta(char*,float);
 void reverse_draw(int);
extern int read_gruppi(int);
 void crea_lista_umis();
 void GetPuntXlGrafico (Widget);
extern void init_umis();




 
Widget (*widget_crea[])() = 
	{NULL,
	 NULL,
	 NULL,
	 NULL,
         create_nyi,   /* 4 */
     NULL,
     NULL,
	 NULL,
	 NULL,
	 NULL,
     NULL,             /* 10 */
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 create_selmis_dialog,  /* 20 */
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 create_selzoom_dialog,  /* 26 */
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 create_umis_dialog,      /* 32 */
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 create_fsca_dialog,      /* 41 */
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL, 
	 NULL,
	 create_selgr_dialog,     /* 50 */
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 create_memgr_dialog,     /* 57 */
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 NULL,
	 create_defumis_dialog,    /* 69 */
         NULL,
         NULL,
         NULL,
         NULL,
         NULL,
         create_dir_dialog,     /* 75 */
         };

/*
 * Forward declarations
 */

static void s_error();

void activate_proc();
void collima_proc();
void zoomord_proc();
void secondi_proc();
void memcollima_proc();
void memzoomord_proc();
void memsecondi_proc();
void zoom_proc();
void map_proc();
void PostIt();
void MoveMouse();
void draw_proc();
void resize_proc();
void create_proc();
void toggle_proc();
void ok_proc();
void cancel_proc();
void apply_proc();
void timer_proc();
void find_proc();
void HC_proc();
extern int cerca_umis(char*, int);
 int init_application(void);
extern  void open_path();
extern int open_gruppi();

static int font_unit = 400;

char *getenv();
Widget  create_S_MAIN_WINDOW();
int set_scala(int,S_XLGRAFICO *);
 int set_ordinate(int,S_XLGRAFICO *);
 void crea_sfondo(Widget,Dimension,Dimension);
 int cerca_nome(char*);
extern  int converti_tempo(float,long  *,long  *,long  *,long  *,long  *,long  *);
 void prep_draw(float,float,S_MIN_MAX *,Widget);
 void draw_grid(Display*,Window,Position,Position);
 int zoomed(XPoint,XPoint);
extern void agg_umis();
 void free_lista_umis();
extern void close_gruppi();
extern int write_gruppo(int);
void set_min_max1(S_DATI *,S_XLGRAFICO *);





/*******************************************
      Routine che dall'oggetto ricava il context associato e restituisce
	il puntatore alla struttura S_XLGRAFICO contenente tutte
	le variabili in common per l'oggetto grafics  
*******************************************/
void GetPuntXlGrafico (w)
Widget w;
{
FALSO_CTX *pfalsoctx;

pfalsoctx = (FALSO_CTX *)UxGetContext (w);
pXlGraf = pfalsoctx->UxPXlGrafico;
}

/******************************************************
 * OS transfer point.  The main routine does all the one-time setup and
 * then calls XtMainLoop.
 ******************************************************/
char *path_uid;
int open_22datGR(S_XLGRAFICO*);

/********************************************
********************************************/
unsigned int inizializza1(argc, argv, pXlGraf)
unsigned int argc;                  /* Command line argument count. */
char *argv[];                       /* Pointers to command line args. */
S_XLGRAFICO *pXlGraf;
{
int i;
char name_uid[200];
name_uid[0]=0;
freeza=1;            /* blocca il refresh */

/* Gestione dell'argomento addizionale: nome del file  nomi variabili */
if (argc > 1)
  {
  if(argc>7)
    XlWarning ("XlGrafico","inizializza1","Errore command line");

  printf("Numero degli argomenti passati: %d\n",argc);
  for(i=1;i<argc;i++)
    printf("Argomento[%d]: %s\n",i,argv[i]);
  }

init_application();

/* per selezione defaults delle unita' di misura */
init_umis();
num_umis=cerca_num_umis();
umis_defsel= (int*) calloc (num_umis, sizeof (int));
x_codumis=(XmString *)XtCalloc(num_umis+1, sizeof(XmString));

open_path();

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
unsigned int inizializza2(argc, argv, main_wid,Oggetto)
    unsigned int argc;                  /* Command line argument count. */
    char *argv[];                       /* Pointers to command line args. */
    Widget main_wid;
    Widget Oggetto;
{
XlGraficoWidget WW = (XlGraficoWidget)Oggetto;

pXlGraf = WW->grafico.p;

main_window_widget=main_wid;

/* Creazione cursore per wait */
cursor_wait=XCreateFontCursor(display_set,XC_watch);

/* crea le message boxes per visualizzazione errore e visualizzazione warning */
WidAttenzione(main_window_widget,"",CREA);
WidErrore(main_window_widget,"",CREA);
/***** MOM
***/

/* disabilita i menu di grafici e misure */
abilita_menu(1);

/*
  se vi sono argomenti addizionali significa che e' stato specificato
  il nome del file di grafica
*/
if(argc>1) 
  {
  if(strcmp(argv[1],"-scala")==0)
    scala_unica=1;
  load_file(argv[1+scala_unica],pXlGraf);

  /* Se il file specificato esiste */
  if(!nofile)
    load_variables(argc-(2+scala_unica),(char**)&argv[2+scala_unica],(Widget)WW);
  }
}

/************************************************
 Routine di creazioni del grafico come oggetto
*************************************************/
void CreateARCH_GRAF(wparent,argc,argv,InRunTime)
Widget wparent;
unsigned int argc;                  /* Command line argument count. */
char *argv[];                       /* Pointers to command line args. */
Boolean InRunTime;
{
XlGraficoWidget cw = (XlGraficoWidget)wparent;
Widget mainIface;
Dimension NumPix,Largh;

pXlGraf = cw->grafico.p;

inizializza1(argc,argv,pXlGraf);
mainIface = create_S_MAIN_WINDOW(wparent,pXlGraf);

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

inizializza2(argc,argv,mainIface,cw);
}

/*************************************************
*************************************************/
void load_file(nome_file,pXlGraf__)
char *nome_file;
S_XLGRAFICO *pXlGraf__;
{
int flag;

pXlGraf=pXlGraf__;

path_22dat=XtMalloc(strlen(nome_file)+1);
strcpy(path_22dat,nome_file);
strcpy(file_vis,path_22dat);

printf ("Nome File: [%s] tipo ARCH [%d]\n",path_22dat,tipo_graf);

if(open_22datGR(pXlGraf))
  {
  WidAttenzione(topLevel_set,err_file_nones,MAPPA);
  nofile=1;
  strcpy(file_vis,no_file_sel);
  return; /* esce se errore in apertura */
  }

nofile=0;
flag=TUTTI;
if(read_22datGR(flag,pXlGraf)==1)  /* legge tutti i dati dall'inizio del file */
  {
  close_22dat();
  open_22datGR(pXlGraf);
  if(read_22datGR(flag,pXlGraf)==1)
    {
    /* file non esistente */
    close_22dat();
    XtFree(path_22dat);
    d2free(simboli);
    widget_array[k_selmis_dialog]=0;
    strcpy(file_vis,no_file_sel);
    nofile=1;
    }
  }

if(notify_overflow == True)
	WidAttenzione(topLevel_set,"Too many data for trend buffer!",MAPPA);

abilita_menu(nofile);
if(nofile==0)
  {	
  /* legge tempo finale e tempo iniziale dal buffer dei dati */
  t_finale=bufdati[n_last-1].t;
  t_iniziale=bufdati[0].t;
  prep_str_timGR(t_iniziale,t_finale,sg.w_draw);
  }
}

/*************************************************
*************************************************/
void load_variables(num_nomi,nome,Wdg)
int num_nomi;
char *nome[];
Widget Wdg;
{
XlGraficoWidget WW=(XlGraficoWidget)Wdg;
int i,j;
int indice;
int loaded=0; /* flag per indicare se e' stata caricata 
                 almeno una variabile */
S_GRAFICO *s;

pXlGraf = WW->grafico.p;

s= &sg;

printf("load_variables\n");
for(i=0;i<4;i++)
  {
/*printf("load_variables [%s]\n",nome[i]);*/
  /* cerca la stringa nella lista determinandone l'indice */
  if(i<num_nomi)
    indice=cerca_stringa(nome[i],simboli);
  else
    indice= -1;
  if(indice==-1)
    {
    x_sel_var[i]=XmStringCreateLtoR(descr_vuota,
		XmSTRING_DEFAULT_CHARSET);
    s->ind_mis[i]=(-1);
    }
  else
    {
    x_sel_var[i]=XmStringCreateLtoR(simboli[indice],
		XmSTRING_DEFAULT_CHARSET);
    s->ind_mis[i]=indice;
    }
  s->x_descr_mis[i]=x_sel_var[i];
  if(s->ind_mis[i]!=-1)
    {
    loaded=1;
    s->ind_umis[i]=cerca_umis(simboli[indice],0);
    if ( strlen(WW->grafico.descr_var[i])!=0 )
      strncpy (s->descr_mis[i],WW->grafico.descr_var[i],LUN_SIMB);
    else strcpy(s->descr_mis[i],simboli[indice]);

    if(s->ind_umis[i]==-1)
      {
      s->ind_umis[i]=num_umis-1;
      } 
    s->autoscaling[i]=1;
    }
  }
/*
 rilegge il file in modo da ricalcolare i valori di minimo e massimo
 per le variabili selezionate
*/
rew_dati(pXlGraf);
set_min_max1(NULL,pXlGraf);
read_22datGR(AGGIORNA,pXlGraf);

if(notify_overflow == True)
	WidAttenzione(topLevel_set,"Too many data for trend buffer!",MAPPA);
for(i=0;i<4;i++)
  {
  if(s->ind_mis[i]!=-1)
    {
    /*
      e' settato l'autoscaling: forza la ricerca di minimo e massimo
      modificando il valore del massimo.
    */
    s->sel_min_max[i].max=s->var_min_max[i].max+1;
    s->umis_sel[i]=uni_mis[s->ind_umis[i]].sel;
    set_scala(i,pXlGraf); 
    s->fix_min_max[i].max=(-1);
    s->fix_min_max[i].min=(-1);
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
rew_dati(pXlGraf);
freeza=0;
}

/*************************************************
*************************************************/
void crea_sfondo(w,width,height)
Widget w;
Dimension width,height;
{
Pixmap stip;

width=(width%6)? width/6+1 : width/6;
height=(height%4)? height/4+1 : height/4;

if((stip=XCreatePixmap(display_set,
            RootWindow(display_set,screen_num),
            width,height,
            DefaultDepth(display_set,screen_num)))== False)
  XlWarning ("XlGrafico","crea_sfondo","Errore creazione pixmap per sfondo");

if(HC_on)
  XFillRectangle(display_set,stip,gc_grid2,0,0,width,height);
else
  {
  XFillRectangle(display_set,stip,gc_grid1,0,0,width,height);
  XDrawRectangle(display_set,stip,gc2[0],0,0,width,height);
  }

set_something(w,XmNbackgroundPixmap,(void*) stip);
}

/****************************************************
 * One-time initialization of application data structures.
 ****************************************************/
int init_application()
{
int k,i;

nofile=1;
strcpy(file_vis,no_file_sel);

for(k=0;k<LUN_SIMB/2;k++)
  descr_vuota[k]=' ';
descr_vuota[LUN_SIMB/2]=0;

for (k = 0; k < MAX_WIDGETS; k++)
  widget_array[k] = NULL;

/* Inizializza  gli indici in lista alle variabili visualizzate */
for(k=0;k<4;k++)
  {
  sg.ind_mis[k]=(int)-1;
  sg.autoscaling[k]=1;
  strcpy(sg.descr_mis[k],descr_vuota);
  }
sg.zoom=0;

x_zoomord_on=XmStringCreateLtoR("Y Zoom off",XmSTRING_DEFAULT_CHARSET);
x_zoomord_off=XmStringCreateLtoR("Y Zoom on",XmSTRING_DEFAULT_CHARSET);
x_collima_on=XmStringCreateLtoR("Collimate off",XmSTRING_DEFAULT_CHARSET);
x_collima_off=XmStringCreateLtoR("Collimate on",XmSTRING_DEFAULT_CHARSET);
x_secondi_on=XmStringCreateLtoR("Time in seconds",XmSTRING_DEFAULT_CHARSET);
x_secondi_off=XmStringCreateLtoR("Time HH:MM:SS",XmSTRING_DEFAULT_CHARSET);
}

/***************************************************************************
 *
 * Utilities utilizzate in fase di inizializzazione.
 */

/*
 *  set_scala
 *      setta i nuovi valori di fondo scala se necessario
 *      torna 0 se non viene effettuato cambiamento di scala
 *      torna 1 se e' stato effettuato cambiamento di scala
 */

int set_scala(indice,pXlGraf)
int indice;   /* indice della variabile all'interno del gruppo
				 (da 0 a 3)    */
S_XLGRAFICO *pXlGraf;
{
S_GRAFICO *s;
s=(&sg);

if(s->autoscaling[indice])
	{
	if(memcmp(&s->sel_min_max[indice],&s->var_min_max[indice],sizeof(S_MIN_MAX)))
		{
		s->sel_min_max[indice]=s->var_min_max[indice];
		if(s->sel_min_max[indice].max == s->sel_min_max[indice].min)
			s->fatt_y[indice]=1;
		else
			s->fatt_y[indice]=draw_height/
                        (s->sel_min_max[indice].max-s->sel_min_max[indice].min);
/*
   Modifica i valori di fondo scala nel buffer contenente le stringhe 
   delle ordinate
*/
		set_ordinate(indice,pXlGraf);
		return(1);
		}
	return(0);
	}
else  /* caso di scala fissa determinata da utente   */
	{
	s->sel_min_max[indice].max=s->fix_min_max[indice].max;
	s->sel_min_max[indice].min=s->fix_min_max[indice].min;
	if(s->sel_min_max[indice].max == s->sel_min_max[indice].min)
		s->fatt_y[indice]=1;
	else
		s->fatt_y[indice]=draw_height/
                                  (s->sel_min_max[indice].max-
                                   s->sel_min_max[indice].min);
	set_ordinate(indice,pXlGraf);
	return(1);
	}
}

/*
 set_scala_unica
    setta il valore di fondo scala in modo fisso (non autoscaling)
    in base al massimo e minimo di tutte le variabili in
    visualizzazione;
    viene calcolato il massimo dei massimi ed il minimo dei minimi;
    max_max, min_min); viene assunto come massimo
		 max_max+|max_max-min_min|*0.1
    come minimo:
		 min_min-|max_max-min_min|*0.1
*/
void set_scala_unica(pXlGraf)
S_XLGRAFICO *pXlGraf;
{
int i;
float max_max,min_min;
float delta;
S_GRAFICO *s;
s=(&sg);
max_max=(-1.0E-37);
min_min=(1.0E+38);
for(i=0;i<4;i++)
	{
/*
 annulla se presente l'autoscaling
*/
	s->autoscaling[i]=0;
	if(s->ind_mis[i]!=-1)
		{
		if(max_max < s->var_min_max[i].max)
			max_max=s->var_min_max[i].max;
		if(min_min > s->var_min_max[i].min)
			min_min=s->var_min_max[i].min;
		}
	}
delta=fabs(max_max-min_min)*0.1;
/*
 corregge i valori di massimo e minimo aggiungendovi il 10% della
 differenza tra massimo e minimo
*/
max_max+=delta;
min_min-=delta;
for(i=0;i<4;i++)
	{
	s->fix_min_max[i].max=max_max;
	s->fix_min_max[i].min=min_min;
	s->sel_min_max[i]=s->fix_min_max[i];
        if(s->sel_min_max[i].max == s->sel_min_max[i].min)
                s->fatt_y[i]=1;
        else
                s->fatt_y[i]=draw_height/
                                  (s->sel_min_max[i].max-
                                   s->sel_min_max[i].min);
        set_ordinate(i,pXlGraf);
	}
}

/*
 *  set_ordinate
 *    setta il valore delle ordinate che vengono visualizzate
 *    in modo conforme ai valori di minimo e massimo
 */

int set_ordinate(ind,pXlGraf)
int ind;    /* indice che individua la variabile all'interno del
                       grafico (valori da 0 a 3)       */
S_XLGRAFICO *pXlGraf;
{
int uguali,ord_scritta;
int lun;
float delta;
float ord;
float min; /* minimo nell'unita' di misura prescelta */
float max; /* massimo nell'unita' di misura prescelta */
int i,j;
S_GRAFICO *s;
s=(&sg);
if(s->ind_mis[ind]==-1)
        {
        for(i=0;i<5;i++)
                sprintf(s->str_ord[i][ind],"         ");
        return(1);
        }
/*
 esamina se i fondo scala delle 4 misure sono uguali; in
 tal caso vengono esaminati solo i fondo scala relativi
 alla prima misura.
*/
uguali=0;
ord_scritta=0;
/*
  esamina se e' stata scritta almeno una ordinata
*/
for(i=ind;i>=0;i--)
        {
        if(s->ind_mis[i]!=-1) ord_scritta=1;
        }
if(ind>0 && ord_scritta)
        {
        uguali=1;
        for(i=0;i<4;i++)
                {
		if(s->ind_mis[i]!=(-1) &&(
(s->ind_umis[i]!=s->ind_umis[ind])||
                   (s->umis_sel[i]!=s->umis_sel[ind])||
                   (s->sel_min_max[i].max!=s->sel_min_max[ind].max) ||
                    (s->sel_min_max[i].min!=s->sel_min_max[ind].min)))
                        uguali=0;
                }
        if (s->ind_mis[0]== -1) uguali=0;
        if(uguali)
                {
                for(i=0;i<5;i++)
                        {
                        for(j=1;j<4;j++)
                                sprintf(s->str_ord[i][j],"         ");
                        }
                return(1);
                }
        }
       
min=s->sel_min_max[ind].min *
     uni_mis[s->ind_umis[ind]].A[s->umis_sel[ind]]+
     uni_mis[s->ind_umis[ind]].B[s->umis_sel[ind]];
max=s->sel_min_max[ind].max *
     uni_mis[s->ind_umis[ind]].A[s->umis_sel[ind]]+
     uni_mis[s->ind_umis[ind]].B[s->umis_sel[ind]];
delta=(max-min)/4.0;
ord=max;
for(i=0;i<5;i++)
        {
        formatta(s->str_ord[i][ind],ord);
        ord-=delta;
        }
}
             
/*
 * Ricerca di una compound string all'interno di una lista di compound strings
 * (la lista deve essere terminata da un NULL).
 */

int x_cerca_stringa(x_stringa,x_lista)
XmString x_stringa;
XmString *x_lista;
{
int i=0;
while(x_lista[i]!=NULL)
	{
	if(XmStringByteCompare(x_stringa,x_lista[i]))
		return(i);
	i++;
	}
return(-1);
}

int cerca_stringa(stringa,lista)
char *stringa;
char *lista[];
{
int i=0;
while(lista[i]!=NULL && i<header2.ncasi)
	{
	if(Utstrstr(lista[i],stringa))
		return(i);
	i++;
	}
return(-1);
}

/*
 ricerca il nome della stringa nei primi 8 caratteri dell'array
 simboli
*/
int cerca_nome(stringa)
char *stringa;
{
int i;
i=0;
while(simboli[i]!=0 && i<header1.nvar)
	{
        if(memcmp(stringa,simboli[i],LUN_NOME)==0)
		{
		return(i);
		}
	i++;
	}
return(-1);
}

/*
 * All errors are fatal.
 */

static void s_error(problem_string)
    char *problem_string;
{
    printf("%s\n", problem_string);
    exit(0);
}



/***************************************************************************
 *
 * This section contains callback routines.
 */

/*
 * Funzione di redraw per le window di visualizzazione trend grafici
 */

void timer_proc(w)
Widget w;
{
Boolean notify=False;
int ultimo;
XPoint *pp_coll;  /* per collimazione */
int iumis,iumis_sel;
int i,j,k;
int numis,selumis;
int zoom;
int y_line,x_line;
float f_pix;
int x_pix,x_pixprec;  /* posizione in pixel del tempo sull-asse del
le ascisse */
int y_pix;  /* posizione in pixel delle ordinate */
int indice; /* indice della misura da visualizzare */
int ind_buf;

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

notify=notify_overflow;
read_22datGR(AGGIORNA,pXlGraf);

if(notify_overflow == True && !notify)
	WidAttenzione(topLevel_set,"Too many data for trend buffer!",MAPPA);
/* legge tempo finale e tempo iniziale dal buffer dei dati */
t_finale=bufdati[n_last-1].t;
t_iniziale=bufdati[0].t;
if(t_finale==t_iniziale)
  {
  WidErrore(main_window_widget,err_tfin_tiniz,MAPPA);
  freeza=0;
  return;
  }

t_ultimo=t_finale;

/* se non c'e' un nuovo campione ricarica il timer ed esce */
if(t_old==t_finale)
  {
  freeza=0;
  return;
  }
else t_old=t_finale;

/* prepara le stringhe da visualizzare sull'asse dei tempi */
prep_str_timGR(t_iniziale,t_finale,w);
for(i=0;i<4;i++)
  {
  if(sg.autoscaling[i] && sg.ind_mis[i]!=-1)
    set_scala(i,pXlGraf);
  if(sg.ind_mis[i]!=-1)
    sg.ultimo[i]=bufdati[n_last-1].mis[i];
  }

/* 
 inserisce nelle stringhe della scala dei tempi i valori calcolati
 in base all'ultimo tempo acquisito 
*/
prep_str_timGR(t_iniziale,t_finale,w);

if(collima)
  t_ultimo=x_collima*(t_finale-t_iniziale)/draw_width+t_iniziale;

/*
 aggiorna il valore corrente dei dati visualizzati (nel caso di collimazione
 calcola il valore aggiornato del punto in stato di collimaz.).
*/
for(i=0;i<4;i++)
  {
  if(sg.ind_mis[i]!=-1)
    {
    if(collima)
      {
      pp_coll=points[i];
      while(pp_coll->x<x_collima)
        pp_coll++;
      sg.ultimo[i]=sg.sel_min_max[i].max-
      pp_coll->y/sg.fatt_y[i];
      }
    iumis=sg.ind_umis[i];
    iumis_sel=sg.umis_sel[i];
    sg.ultimo[i]=sg.ultimo[i]*uni_mis[iumis].A[iumis_sel]+
		 uni_mis[iumis].B[iumis_sel];
    }
  }

XClearArea(display_set,XtWindow(sg.w_draw),0,0,0,0,True);
XClearArea(display_set,XtWindow(sg.w_ord),0,0,0,0,True);
XClearArea(display_set,XtWindow(sg.w_val),0,0,0,0,True);
XClearArea(display_set,XtWindow(sg.w_tim),0,0,0,0,True);
XClearArea(display_set,XtWindow(widget_array[k_tempo]),0,0,0,0,True);
freeza=0;
}

/*******************************
 prep_str_timGR
 preparazione stringhe scala dei tempi
 inserisce nelle stringhe della scala dei tempi i valori calcolati
 in base all'ultimo tempo acquisito 
*******************************/
void prep_str_timGR(t_ini,t_fin,w)
float t_ini,t_fin;
Widget w;
{
int i;
float t_delta; /* intervallo di tempo per scrittura stringhe
                asse delle ascisse */
float t_asc;
int t_ore,t_minuti,t_secondi;
long  ora=0,min=0,sec=0,gior=0,mes=0,anno=1995;


GetPuntXlGrafico(w);

t_delta=(t_fin-t_ini)/6;
t_asc=t_ini;
for(i=0;i<7;i++)
  {
  sprintf(str_tims[i],"%.2f",t_asc);
  ora=0;
  min=0;
  sec=0;
  gior=0;
  mes=0;
  anno=1995;
  converti_tempo(t_asc,&ora,&min,&sec,&gior,&mes,&anno);
  t_ore=ora;
  t_minuti=min;
  t_secondi=sec;
  sprintf(str_tim[i],"%.2d:%.02d:%.02d",t_ore,t_minuti,t_secondi);
  t_asc+=t_delta;
  }

/**************************************
t_delta=(t_fin-t_ini)/6;
t_asc=((int)t_ini)%86400;
for(i=0;i<7;i++)
  {
  sprintf(str_tims[i],"%.2f",t_asc);
  t_ore=(((int)t_asc)/60)/60;
  t_minuti=(((int)t_asc)/60)%60;
  t_secondi=((int)t_asc)%60;
  if(t_ore==0)
    sprintf(str_tim[i],"%.2d:%.02d",t_minuti,t_secondi);
  else
    sprintf(str_tim[i],"%.2d:%.02d:%.02d",t_ore,t_minuti,t_secondi);
  t_asc+=t_delta;
  }
*******************/
}           

/*
 prep_draw
 preparazione del vettore dei punti da disegnare
*/
void prep_draw(pt_iniziale,pt_finale,pmin_max,ww)
float pt_iniziale,pt_finale;
S_MIN_MAX *pmin_max;
Widget ww;
{
float f_pix;
int x_pix,x_pixprec;  /* posizione in pixel del tempo sull-asse delle ascisse */
int y_pix;            /* posizione in pixel delle ordinate */
int ind_buf;
int indice,i;
XPoint *pp[4];        /* percorrono i vettori points */
S_SEL_DATI *DatiDaDisegnare; /*  Puntatore che scorre il vettore bufdati
				o il vettore bufdati_zoom a seconda se ci
				si trova in stato normale o di zoom  */

/*  Ricava il puntatore alla struttura delle variabili  */
GetPuntXlGrafico (ww);

/*  Se il tempo iniziale e' uguale al tempo finale non disegna nessun punto  */
if (pt_iniziale == pt_finale) return;

/*  Controlla se ci si trova in stato di zoom o no  */
if (stato_zoom == ZOOM_END)
  DatiDaDisegnare = bufdati;
else
  DatiDaDisegnare = bufdati_zoom;

/*
  step in pixel per un decimo di secondo
*/

pix_step=((float)draw_width)/(pt_finale-pt_iniziale);

ind_buf=0;
while (DatiDaDisegnare[ind_buf].t<pt_iniziale &&
		ind_buf<(n_elementi_bufdati-1) )
        ind_buf++;
f_pix=0.0;
x_pix=0;
x_pixprec=0;
for(i=0;i<4;i++)
        {
	if(pmin_max[i].max == pmin_max[i].min)
		sg.fatt_y[i]=1;
	else
		sg.fatt_y[i]=draw_height/
                (pmin_max[i].max-pmin_max[i].min);
        pp[i]=points[i];
        npp[i]=0;
        indice=sg.ind_mis[i];
        if(indice==-1) continue;
        y_pix=sg.fatt_y[i]*
              (pmin_max[i].max-DatiDaDisegnare[ind_buf].mis[i]);
        pp[i]->x=x_pix;
        pp[i]->y=y_pix;
        pp[i]++;
        npp[i]++;
        }
ind_buf++;
/*
  prepara il vettore dei punti da disegnare
*/
while(x_pix<draw_width && ind_buf<n_last)
        {
        f_pix=pix_step*(DatiDaDisegnare[ind_buf].t-pt_iniziale);
        x_pix=f_pix;
        if(x_pixprec==x_pix){ind_buf++;  continue;}
        for(i=0;i<4;i++)
                {
                indice=sg.ind_mis[i];
                if(indice==-1) continue;
                y_pix=sg.fatt_y[i]*
                (pmin_max[i].max-DatiDaDisegnare[ind_buf].mis[i]);
                pp[i]->x=x_pix;
                pp[i]->y=y_pix;
                pp[i]++;
                npp[i]++;
                }
        ind_buf++;
        x_pixprec=x_pix;
        }
}

void draw_proc(w, tag, str)
Widget w;
int *tag;
XmDrawingAreaCallbackStruct *str;
{
int ord_unica;
char appstr[50];
int t_ore,t_minuti,t_secondi;
Window win; 
int widget_num = (int)tag;              /* Convert tag to widget number. */
int i,k;
int numis,selumis;
int zoom;
Display *display;
int y_line,x_line; 
float f_pix;
int x_pix,x_pixprec; /* posizione in pixel del tempo sull-asse delle ascisse */
int y_pix;  /* posizione in pixel delle ordinate */
int j,jprec; /* scorron il buffer circolare dati */
int indice; /* indice della misura da visualizzare */
char app_file_vis[300];
char *app_char;
Position offx=0;
Position offy=0;
Position offxparent,offyparent;
win = str->window; 

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);
#if defined XPRINTER_USED
/*
 Se l'indice di window passato nella callback struct e'
 nullo significa che la draw_proc e' stata chiamata per la stampa
*/
if(win==0)
	{
	Dimension bordo;
/*
 calcola gli offset per il ridisegno sulla stampante
*/
  	get_something(w,XmNx, (void*) &offx);
	get_something(w,XmNy, (void*) &offy);
	get_something(w,XmNborderWidth, (void*) &bordo);
	offx+=bordo;
	offy+=bordo;
  	get_something(XtParent(w),XmNx, (void*) &offxparent);
	get_something(XtParent(w),XmNy, (void*) &offyparent);
	get_something(XtParent(w),XmNborderWidth, (void*) &bordo);
	offx+=bordo;
	offy+=bordo;
	offx+=offxparent;
	offy+=offyparent;
	get_something(XtParent(XtParent(w)),XmNx, (void*) &offxparent);
        get_something(XtParent(XtParent(w)),XmNy, (void*) &offyparent);
	get_something(XtParent(XtParent(w)),XmNborderWidth, (void*) &bordo);
	offx+=bordo;
	offy+=bordo;
        offx+=offxparent;
        offy+=offyparent;
	display=XlDisplay(w);
	
	}
else
	{
	display=display_set;
	}
#else
display=display_set;
#endif
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
    if(win !=0)
    	crea_sfondo(w,draw_width,draw_height);
    }
#if defined XPRINTER_USED
  if(win == 0)
        draw_grid(display,win,offx,offy);
#endif
  draw_height-=2;

  if(nofile)
    break;

  if(stato_zoom==ZOOM_END)
    prep_draw(t_iniziale,t_finale,&sg.sel_min_max[0],w);
  else
    prep_draw(tz_iniziale,tz_finale,&sg.sel_min_max[0],w);

#if defined XPRINTER_USED
/*
 Aggiunge gli offset della window di appartenenza
 per la stampa
*/
  if(win == 0)
	{
  	for(i=0;i<4;i++)
		{
  		for(j=0;j<npp[i];j++)
			{
			points[i][j].x= points[i][j].x+offx;
			points[i][j].y= points[i][j].y+offy;
			}
		}
	}
#endif
  for(i=0;i<4;i++)
    {
    indice=sg.ind_mis[i];
    if(indice == -1) continue;
    if(HC_on)
      {
      XDrawLines(display,win,gc2[i],
               points[i],npp[i],CoordModeOrigin);
      if(win !=0) /* escluso il caso di stampa */
      	draw_grid(display,win,offx,offy);
      }
    else
      XDrawLines(display,win,gc[i],
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
      XDrawLine(display,win,gc2[i],5+offx,y_line+offy,25+offx,y_line+offy);
      if(sg.ind_mis[i]==-1) continue;
      XDrawString(display,win,gc2[0],30+offx,
			font_height*(i+1)+offy,sg.descr_mis[i],
                      	strlen(sg.descr_mis[i]));
      }
    else
      {
      XDrawLine(display,win,gc[i],5+offx,y_line+offy,25+offx,y_line+offy);
      if(sg.ind_mis[i]==-1) continue;
        XDrawString(display,win,gc[i],30+offx,
			font_height*(i+1)+offy,sg.descr_mis[i],
                  	strlen(sg.descr_mis[i]));
      }
    }
  break;

  /****/
  /****  Scrive il nome del file attualmente aperto  ****/
  /****/
  case k_draw0:
#if defined SCADA_MMI
  strcpy(app_file_vis,file_vis);
  if((app_char=strstr(app_file_vis,"_"))!=NULL)
	{
	*app_char=' ';
	app_char=strstr(app_char,"_");
	if(app_char!=NULL)
		*app_char=0;
	sprintf(app_file_vis,"%s %s",app_file_vis,header1.titolo);
	}
  if(HC_on)
             XDrawString(display,win,gc2[0],10+offx,
		font_height+offy,app_file_vis,strlen(app_file_vis));
  else
   	     XDrawString(display,win,gc[1],10+offx,
		font_height+offy,app_file_vis,strlen(app_file_vis));
#else
  if(HC_on)
   XDrawString(display,win,gc2[0],10+offx,font_height+offy,file_vis,strlen(file_vis));
  else
   XDrawString(display,win,gc[1],10+offx,font_height+offy,file_vis,strlen(file_vis));
#endif
  break;

  /****/
  /****  Scrive i valori delle quattro misure selezionate  ****/
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
        XDrawString(display,win,gc2[0],5+offx,
               font_height*(i+1)+offy,uni_mis[numis].codm[selumis],
               strlen(uni_mis[numis].codm[selumis]));
        XDrawString(display,win,gc2[0],45+offx,font_height*(i+1)+offy,
                appstr,strlen(appstr));
	}
      else
	{
        XDrawString(display,win,gc[i],5+offx,
               font_height*(i+1)+offy,uni_mis[numis].codm[selumis],
               strlen(uni_mis[numis].codm[selumis]));
        XDrawString(display,win,gc[i],45+offx,font_height*(i+1)+offy,
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
    sprintf(appstr,"%.2d:%.2d:%.2d",t_ore,t_minuti,t_secondi);
    }
  if(HC_on)
    XDrawString(display,win,gc2[0],10+offx,y_line+offy,appstr,strlen(appstr));
  else
    XDrawString(display,win,gc[3],10+offx,y_line+offy,appstr,strlen(appstr));
  break;
	
  /****/
  /**** Setta i valori dei tempi nella barra sottostante la griglia ****/
  /****/
  case k_tim1:
  get_something(w,XmNwidth, (void*) &draw_width);
  for(k=0;k<7;k++)
    {
    zoom=sg.zoom;
    x_line=k*(draw_width/6)+offx;
    /*  correzioni per centrare meglio i valori estremi   */
    if(k==0) x_line+=5;
    if(k==6 && strlen(str_tims[k])>6) x_line-=45;
    else if(k==6) x_line-=35;
    if(k!= 6 && k!= 0 && strlen(str_tims[k])>6)
        x_line-=15;
    /*  Se la dimensione della window e' minore di 180 pixel vengono
	saltate le label dei tempi con indice 1,2,3,4,5.
	Se la dimensione della window e' compresa tra 180 e 420 pixel
	vengono saltate le label dei tempi con indice 1,2,4,5.
	Se la dimensione della window e' maggiore di 420 pixel vengono
	visualizzate tutte le label dei tempi  */
    if ( (draw_width>450)||((draw_width<=210)&&((k==0)||(k==6)))||
	(draw_width>210 && draw_width<=450 && ((k==0)||(k==3)||(k==6))) )
      {
      if(tempo_sec)
        {
        if(HC_on)
          XDrawString(display,win,gc2[0],x_line,font_height+3+offy,
                     str_tims[k],strlen(str_tims[k]));
        else
          XDrawString(display,win,gc[3],x_line,font_height+3+offy,
                     str_tims[k],strlen(str_tims[k]));
        }
      else
        {
        if(HC_on)
          XDrawString(display,win,gc2[0],x_line,font_height+3+offy,
                 str_tim[k],strlen(str_tim[k]));
        else
          XDrawString(display,win,gc[3],x_line,font_height+3+offy,
                str_tim[k],strlen(str_tim[k]));
        }
      }
    }
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
      y_line=k*((ord_height-5*font_height)/4)+offy;
      for(i=0;i<4;i++)
        {
        if(ord_unica)
          y_line+=(font_height*3);
        else
          y_line+=font_height;
        if(sg.ind_mis[i]==-1) continue;
        if(HC_on)
          XDrawString(display,win,gc2[0],20+offx,y_line,
                   			sg.str_ord[k][i],
					strlen(sg.str_ord[k][i]));
        else
          XDrawString(display,win,gc[i],20+offx,y_line,
                                       sg.str_ord[k][i],
					strlen(sg.str_ord[k][i]));
        }
      }
    }
  break;
  }  
}

/*
 * Disegna la griglia di riferimento (per HC)
 */
void draw_grid(display,win,offx,offy)
Display *display;
Window win;
Position offx,offy;
{
float dy,dx; /* ampiezza rettangoli griglia su asse y e su asse x */
int x_line,y_line;
int line_lenght; /* lunghezza delle linee di griglia da disegnare (nel caso di
                    rappresentazione per HardCopy in B/N e' pari a 10 pixel fisso
                    (tacca dell'asse coordinate)  nel caso normale e' pari alla
                    ampiezza della drawing area */
int i;
if(win ==0 && !HC_on) /* caso di stampa in modalita' normale 
                         (non reverse )    */
	{
  	XFillRectangle(display,win,gc_grid1,offx,offy,draw_width,draw_height);
	}
if(HC_on)
	line_lenght=10;	
else
	line_lenght= draw_width;
dx=draw_width/6;
dy=draw_height/4;
/*
 Linee orizzontali
*/
y_line=offy;
XDrawLine(display,win,gc2[0],offx,y_line,draw_width+offx,y_line);
for(i=1;i<4;i++)
        {
        y_line=(int)( (float)(dy*(float)i) )+offy;
        XDrawLine(display,win,gc2[0],offx,y_line,line_lenght+offx,y_line);
	XDrawLine(display,win,gc2[0],draw_width-line_lenght+offx,y_line,draw_width+offx,y_line);
	}
/* y_line=draw_height-1+offy; */
y_line=draw_height+offy;
XDrawLine(display,win,gc2[0],offx,y_line,draw_width+offx,y_line);
/*
 Linee verticali
*/
if(HC_on)
	line_lenght=10;	
else
	line_lenght= draw_height;
x_line=offx;
XDrawLine(display,win,gc2[0],x_line,offy,x_line,draw_height+offy);
for(i=1;i<6;i++)
        {
        x_line=(int)( (float)(dx*(float)i) )+offx;
	XDrawLine(display,win,gc2[0],x_line,draw_height+offy,x_line,draw_height-line_lenght+offy);
	XDrawLine(display,win,gc2[0],x_line,line_lenght+offy,x_line,offy);
	}
x_line=draw_width+offx;
XDrawLine(display,win,gc2[0],x_line,offy,x_line,draw_height+offy);
}

/*
 *  gestione dei toggle button per selezione singola variabile
 *  o per gestione fattore di zoom.
 */
void toggle_proc(w, tag, toggle)
    Widget w;
    int *tag;
    XmToggleButtonCallbackStruct *toggle;
{
float fmin,fmax;
int i;
int widget_num=(int)tag;
S_GRAFICO *s;
int selumis,numis; 

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

s=(&sg);
switch(widget_num)
  {
  case k_toggle_mis1:
  case k_toggle_mis2:
  case k_toggle_mis3:
  case k_toggle_mis4:
    for(i=0;i<NUM_BOOLEAN_MIS;i++)
      toggle_mis[i]=0;
    toggle_mis[widget_num-k_toggle_mis1]=toggle->set;
  break;

  case k_toggle_zoom1:
  case k_toggle_zoom2:
  case k_toggle_zoom3:
  case k_toggle_zoom4:
  case k_toggle_zoom5:
    for(i=0;i<NUM_BOOLEAN_ZOOM;i++)
      toggle_zoom[i]=0;
    toggle_zoom[widget_num-k_toggle_zoom1]=toggle->set;
  break;

  case k_toggle_umis1:
  case k_toggle_umis2:
  case k_toggle_umis3:
  case k_toggle_umis4:
    toggle_umis[widget_num-k_toggle_umis1]=toggle->set;
    if(toggle->set) app_umis_sel[var_attiva]=(widget_num)-k_toggle_umis1;
  break;

  case k_toggle2_mis1:
  case k_toggle2_mis2:
  case k_toggle2_mis3:
  case k_toggle2_mis4:
    if(toggle->set==1)
      {
      var_attiva=(widget_num-k_toggle2_mis1);
      selumis=app_umis_sel[var_attiva];    
      numis=s->ind_umis[var_attiva];
      XmToggleButtonSetState(widget_array[k_toggle_umis1+selumis],True,True);
      for(i=k_toggle_umis1;i<=k_toggle_umis4;i++)
        {
        set_something(widget_array[i],XmNlabelString,
        (void*) XmStringCreateLtoR(uni_mis[numis].codm[i-k_toggle_umis1],
                                   XmSTRING_DEFAULT_CHARSET));
        }
      }
  break;

  case k_toggle3_mis1:
  case k_toggle3_mis2:
  case k_toggle3_mis3:
  case k_toggle3_mis4:
    if(toggle->set==1)
      {
      var_attiva=(widget_num-k_toggle3_mis1);
      toggle3_mis[var_attiva]=toggle->set;
      XmTextSetString(widget_array[k_text_max],app_max[var_attiva]);
      XmTextSetString(widget_array[k_text_min],app_min[var_attiva]);
      if(autoscalingG[var_attiva])
        XmToggleButtonSetState(widget_array[k_toggle_auto],True,True);
      else
        XmToggleButtonSetState(widget_array[k_toggle_man],True,True);
      }
    else 
      {
      if(toggle3_mis[widget_num-k_toggle3_mis1]==1)
        {
        XtFree(app_max[widget_num-k_toggle3_mis1]);
        XtFree(app_min[widget_num-k_toggle3_mis1]);
        app_max[widget_num-k_toggle3_mis1]=
              XmTextGetString(widget_array[k_text_max]);
        app_min[widget_num-k_toggle3_mis1]=
	       XmTextGetString(widget_array[k_text_min]);
        }
      toggle3_mis[widget_num-k_toggle3_mis1]=toggle->set;
      }
  break;

  case k_toggle_auto:
  case k_toggle_man:
    toggle_fsca[widget_num-k_toggle_man]=toggle->set;
    if(toggle->set)
      {
      autoscalingG[var_attiva]=(widget_num)-k_toggle_man;
      }
  break;

  case k_toggle2_umis1:
  case k_toggle2_umis2:
  case k_toggle2_umis3:
  case k_toggle2_umis4:
    toggle2_umis[widget_num-k_toggle2_umis1]=toggle->set;
    if(toggle->set) umis_defsel[indice_umis]=(widget_num)-k_toggle2_umis1;
  break;

  case k_toggle_dir1:
  case k_toggle_dir2:
  case k_toggle_dir3:
  case k_toggle_dir4:
    toggle_dir[widget_num-k_toggle_dir1]=toggle->set;
  break;
  }
}

void set_cur_wait()
{
XDefineCursor(display_set,XtWindow(main_window_widget),cursor_wait);
XSync(display_set,False);
}

void clr_cur_wait()
{
XUndefineCursor(display_set,XtWindow(main_window_widget));
XSync(display_set,False);
}

/*
	Gestisce il click del mouse nella window di disegno grafico
*/
void PostIt(w,num,str)
Widget w;
int *num;
XmDrawingAreaCallbackStruct *str;
{
XButtonEvent *event;
int widget_num;	
event=(XButtonEvent *) str->event;

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);
if( (w==sg.w_draw) && (event->button == Button1) && (event->type==ButtonPress) &&
	(stato_zoom==ZOOM_END) ) 
	(xlManagerClassRec.xlmanager_class.select)(XtParent(XtParent(w)),0);

/*
  se si e' in freeze per visualizzazione zoomata accetta solo gli
  input nella window di disegno
*/
if(nofile) return;
if(event->button == Button3) return;
if(event->button != Button1 && freeza && (stato_zoom==ZOOM_WAIT ||
                                          stato_zoom==ZOOM_DRAG))
  {
  if(w!=sg.w_draw) return;
  if(stato_zoom==ZOOM_DRAG)
    {
    /* Cancella l'ultimo rettangolo di selezione disegnato */
    if(zoomord)
      XDrawRectangle(display_set,XtWindow(sg.w_draw),gc_zoom,
			origine.x,origine.y,widthGraf,heightGraf);
    else
      {
      XDrawLine(display_set,XtWindow(sg.w_draw),gc_zoom,origine.x+widthGraf,0,
                                  origine.x+widthGraf,draw_width);
      XDrawLine(display_set,XtWindow(sg.w_draw),gc_zoom,origine.x
                                  ,0,origine.x,draw_width);
      }
    }
  XSync(display_set,False);
  stato_zoom=ZOOM_END;
  }

/* se si e' in modalita' di freeze e si attende il primo
   click del mouse -> permette solamente la gestione
   del rettangolo di zoom */
if(freeza && event->button == Button1 )
  {
  if(w!=sg.w_draw) return;
  if(stato_zoom==ZOOM_WAIT)
    {
    stato_zoom=ZOOM_DRAG;
    /* segnala di disegnare il primo rettangolo */
    iniz_rect=1;
    z_ini.x=event->x;
    z_ini.y=event->y;
    if(!zoomord)
      {
      XDrawLine(display_set,XtWindow(sg.w_draw),gc_zoom,
                        z_ini.x,0,z_ini.x,draw_width);
      }
    return;
    }
  else if(stato_zoom==ZOOM_DRAG)
    {
    z_fin.x=event->x;
    z_fin.y=event->y;
    stato_zoom=ZOOM_WAIT;
    zoomed(z_ini,z_fin);
    return;
    }
  }
}

/**********************
 * routine per la preparazione della grafica zoomata
 **********************/
int zoomed(pz_ini,pz_fin)
XPoint pz_ini;
XPoint pz_fin;
{
int ind_buf;
int ultimo;
int iumis,iumis_sel;
int i,j,k;
int numis,selumis;
int zoom;
int indice; /* indice della misura da visualizzare */
int tempo_ini,tempo_fin, t_limite_alto, t_limite_basso;
float z_massimo,z_minimo;

for(i=0;i<4;i++)
  if(sg.ind_mis[i]!=-1) break;
if(i==4) return(0);  /* non e' ancora stata selezionata alcuna misura */

tempo_ini=pz_ini.x/(pix_step)+tz_iniziale; /* converte in secondi */
tempo_fin=pz_fin.x/(pix_step)+tz_iniziale;

if(tempo_ini==tempo_fin)  /* zoom non effettuabile */
        {
        t_old=0.0;
        XClearArea(display_set,XtWindow(sg.w_draw),0,0,0,0,True);
        return(0);
        }
if(tempo_ini>tempo_fin)
        { tz_finale=tempo_ini; tz_iniziale=tempo_fin;}
else
        { tz_finale=tempo_fin; tz_iniziale=tempo_ini;}

/*
 i valori di tempo per lo zoom non possono essere esterni
 all'intervallo t_limite_alto, t_limite_basso
*/
ind_buf=0;
while (bufdati_zoom[ind_buf].t!= -1 && 
	ind_buf<(n_elementi_bufdati-1) )
       	 ind_buf++;
if(ind_buf) ind_buf--;
t_limite_alto=bufdati_zoom[ind_buf].t;
t_limite_basso=bufdati_zoom[0].t;

if(tz_finale > t_limite_alto) tz_finale=t_limite_alto;
if(tz_iniziale < t_limite_basso) tz_iniziale=t_limite_basso;

if(pz_ini.y>pz_fin.y)
        { z_massimo=(float)pz_fin.y;  z_minimo=(float)pz_ini.y; }
else
        { z_massimo=(float)pz_ini.y;  z_minimo=(float)pz_fin.y; }

t_ultimo=t_finale;
for(i=0;i<4;i++)
        {
        if(sg.ind_mis[i]==-1) continue;
        if(zoomord)
                {
                sg.sel_min_max[i].min=sg.sel_min_max[i].max-
                                 (z_minimo/sg.fatt_y[i]);
                sg.sel_min_max[i].max-=(z_massimo/sg.fatt_y[i]);
                if(sg.sel_min_max[i].max == sg.sel_min_max[i].min)
                        sg.fatt_y[i]=1;
                else
                        sg.fatt_y[i]=
                         draw_height/(sg.sel_min_max[i].max-
				sg.sel_min_max[i].min);
                set_ordinate(i,pXlGraf);
                }
        }

/* inserisce nelle stringhe della scala dei tempi i valori calcolati
   in base all'ultimo tempo acquisito */
prep_str_timGR(tz_iniziale,tz_finale,sg.w_draw);

t_ultimo=t_finale;
t_old=0.0;
XClearArea(display_set,XtWindow(sg.w_draw),0,0,0,0,True);
XClearArea(display_set,XtWindow(sg.w_ord),0,0,0,0,True);
XClearArea(display_set,XtWindow(sg.w_val),0,0,0,0,True);
XClearArea(display_set,XtWindow(sg.w_tim),0,0,0,0,True);
XClearArea(display_set,XtWindow(widget_array[k_tempo]),0,0,0,0,True);
}

/*
 * routine per la gestione del movimento del mouse nella window
 * di presentazione grafici
 */
void MoveMouse(w,spare,event)
Widget w;
int *spare;
XPointerMovedEvent *event;
{
int iumis,iumis_sel;
int ind_buf;
XPoint *pp_coll;  /* per collimazione */
Dimension width_new, height_new;
int i;
S_SEL_DATI *DatiDaDisegnare; /*  Puntatore che scorre il vettore bufdati
				o il vettore bufdati_zoom a seconda se ci
				si trova in stato normale o di zoom  */

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

if(collima)
  {
  x_collima=event->x;
  if(!freeza)
    t_ultimo=x_collima*(t_finale-t_iniziale)/draw_width+t_iniziale;
  else /* caso di modalita' di zoom */
    t_ultimo=x_collima*(tz_finale-tz_iniziale)/draw_width+tz_iniziale;
/*  Controlla se ci si trova in stato di zoom o no  */
  if (stato_zoom == ZOOM_END)
 	 DatiDaDisegnare = bufdati;
  else
  	DatiDaDisegnare = bufdati_zoom;
  ind_buf=0;
  while (DatiDaDisegnare[ind_buf].t<t_ultimo &&
        	DatiDaDisegnare[ind_buf].t!= -1 &&
		ind_buf<(n_elementi_bufdati-1) )
       		 ind_buf++;
  if(ind_buf) ind_buf--;
/*
 Caso limite
*/
  if(DatiDaDisegnare[ind_buf].t== -1 && ind_buf)
	{
	ind_buf--;
	}
  for(i=0;i<4;i++)
    {
    if(sg.ind_mis[i]!=-1)
	{
	t_ultimo=DatiDaDisegnare[ind_buf].t;
	sg.ultimo[i]= DatiDaDisegnare[ind_buf].mis[i];
    	iumis=sg.ind_umis[i];
    	iumis_sel=sg.umis_sel[i];
    	sg.ultimo[i]=sg.ultimo[i] *
                    uni_mis[iumis].A[iumis_sel]+
                    uni_mis[iumis].B[iumis_sel];
	}
    }

  XClearArea(display_set,XtWindow(sg.w_val),0,0,0,0,True);
  XClearArea(display_set,XtWindow(widget_array[k_tempo]),0,0,0,0,True);
  }

if (freeza && stato_zoom==ZOOM_DRAG)
  {
  if(!iniz_rect)  /* rettangoli successivi al primo */
    {
    if(zoomord)
      XDrawRectangle(display_set,XtWindow(sg.w_draw),gc_zoom,
			origine.x,origine.y,widthGraf,heightGraf);
    else
      XDrawLine(display_set,XtWindow(sg.w_draw),gc_zoom,
			origine.x+widthGraf,0,origine.x+widthGraf,draw_width);

    }
  else iniz_rect=0;
  z_fin.x=event->x;
  z_fin.y=event->y;
  width_new=(z_fin.x-z_ini.x);
  if(width_new<0)
    {
    width_new=(-width_new);
    origine.x=z_fin.x;
    }
  else origine.x=z_ini.x;
  height_new=(z_fin.y-z_ini.y);
  if(height_new<0)
    {
    height_new=(-height_new);
    origine.y=z_fin.y; }
  else origine.y=z_ini.y;
               
  if(widthGraf!=width_new || heightGraf != height_new)
    {
    widthGraf=width_new;
    heightGraf=height_new;
    if(zoomord)
      XDrawRectangle(display_set,XtWindow(sg.w_draw),gc_zoom,origine.x,origine.y,widthGraf,heightGraf);
    else
      XDrawLine(display_set,XtWindow(sg.w_draw),gc_zoom,
                        origine.x+widthGraf,0,origine.x+widthGraf,draw_width);

    }
  }
}

/***************************
****************************/
void collima_proc(w,reason)
    Widget w;
    unsigned long *reason;
{
/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

if(collima==0)
        {
        collima=1;
        XDefineCursor(display_set,XtWindow(sg.w_draw),cursor_coll);
        set_something(wcollima,XmNlabelString,(void*) x_collima_on);
        }
else
        {
        collima=0;
        XUndefineCursor(display_set,XtWindow(sg.w_draw));
        set_something(wcollima,XmNlabelString,(void*) x_collima_off);
        }
}

/***************************
***************************/
void zoomord_proc(w,reason)
    Widget w;
    unsigned long *reason;
{
/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

zoomord=(!zoomord);
if(zoomord)
        set_something(wzoomord,XmNlabelString,(void*) x_zoomord_on);
else
        set_something(wzoomord,XmNlabelString,(void*) x_zoomord_off);

}

/***************************
****************************/
void secondi_proc(w,reason)
    Widget w;
    unsigned long *reason;
{
/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

tempo_sec=(!tempo_sec);
if(tempo_sec)
        set_something(wtempo_sec,XmNlabelString,(void*) x_secondi_off);
else
        set_something(wtempo_sec,XmNlabelString,(void*) x_secondi_on);
XClearArea(display_set,XtWindow(sg.w_tim),0,0,0,0,True);
XClearArea(display_set,XtWindow(widget_array[k_tempo]),0,0,0,0,True);
}

/*****************************
*****************************/
void memzoomord_proc(w,reason)
    Widget w;
    unsigned long *reason;
{
/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

wzoomord=w;
}

/*****************************
*****************************/
void memsecondi_proc(w,reason)
    Widget w;
    unsigned long *reason;
{
/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

wtempo_sec=w;
}
   
void memcollima_proc(w,reason)
    Widget w;
    unsigned long *reason;
{
/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

wcollima=w;
}

/*******************************
*******************************/
void zoom_proc(w,tag,reason)
Widget w;
int *tag;
unsigned long *reason;
{
int zoom_on,i;

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

zoom_on=(int)(tag);
freeza=zoom_on;
if(zoom_on)
  {
  if(tz_iniziale==0 && tz_finale==0)
    {
    tz_iniziale=t_iniziale;
    tz_finale=t_finale;
    }
  /* se e' in collimazione esce da tale modalita'    */
  /*      if(collima) collima=0;  */

  /* mette in freeze la visualizzazione rimuovendo il time-out */
  freeza=1;
  XSync(display_set,False);
  timer=0;
  stato_zoom=ZOOM_WAIT;
  XDefineCursor(display_set,XtWindow(sg.w_draw),cursor_zoom);
/*
 Alloca un buffer per contenere i dati in zoom in modo che mentre vengono
 fatte le operazioni di zoom bufdati vanga utilizzato per 
 la acquisizione dei nuovi valori
*/ 
  /*  Alloca memoria per il bufdati di zoom  */
  bufdati_zoom = (S_SEL_DATI *)XtCalloc (n_elementi_bufdati,
					sizeof(S_SEL_DATI));
  memcpy (bufdati_zoom,bufdati,n_elementi_bufdati*(sizeof(S_SEL_DATI)));
  }
else    /* fine zoom */
  {
  tz_iniziale=0;
  tz_finale=0;
  t_old=0; /* per forzare il redraw nella timer proc */
  stato_zoom=ZOOM_END;
  XUndefineCursor(display_set,XtWindow(sg.w_draw));
  if(collima)
    XDefineCursor(display_set,XtWindow(sg.w_draw),cursor_coll);
  freeza=0;

  /*  Libero il vettore di appoggio per i dati di zoom  */
 // GUAG2025 era XtFree (bufdati_zoom); 
  XtFree ((void *)bufdati_zoom);

  /* ripristina il tipo di scala presente prima di effettuare lo zoom */
  for(i=0;i<4;i++)
	set_scala(i,pXlGraf);

  prep_str_timGR (t_iniziale,t_finale,sg.w_draw);
  XClearArea (XtDisplay(sg.w_ord),XtWindow(sg.w_ord),0,0,0,0,True);
  XClearArea (XtDisplay(sg.w_draw),XtWindow(sg.w_draw),0,0,0,0,True);
  XClearArea (XtDisplay(sg.w_tim),XtWindow(sg.w_tim),0,0,0,0,True);
  }
}

/*******************************
*******************************/
void reverse_draw(flag)
int flag;
{
static Pixel draw0_bg,draw1_bg,mis1_bg,ord1_bg,tim1_bg,tempo_bg,form_bg,val1_bg;
if(flag)
	{
	get_something(widget_array[k_draw1],XmNbackground, (void*) &draw1_bg);
        get_something(widget_array[k_draw0],XmNbackground, (void*) &draw0_bg);
	get_something(widget_array[k_mis1],XmNbackground, (void*) &mis1_bg);
	get_something(widget_array[k_ord1],XmNbackground, (void*) &ord1_bg);
        get_something(widget_array[k_val1],XmNbackground, (void*) &val1_bg);
	get_something(widget_array[k_tim1],XmNbackground, (void*) &tim1_bg);
	get_something(widget_array[k_tempo],XmNbackground, (void*) &tempo_bg);
	get_something(widget_array[k_form],XmNbackground, (void*) &form_bg);
	set_something(widget_array[k_draw1],XmNbackground,
    (char*)WhitePixel(display_set, screen_num));
	set_something(widget_array[k_mis1],XmNbackground,
    (char*)WhitePixel(display_set, screen_num));
	set_something(widget_array[k_ord1],XmNbackground,
    (char*)WhitePixel(display_set,  screen_num));
        set_something(widget_array[k_tim1],XmNbackground,
          (char*)WhitePixel(display_set,  screen_num));
        set_something(widget_array[k_tempo],XmNbackground,
          (char*)WhitePixel(display_set,  screen_num));
        set_something(widget_array[k_val1],XmNbackground,
          (char*)WhitePixel(display_set,  screen_num));
        set_something(widget_array[k_form],XmNbackground,
          (char*)WhitePixel(display_set,  screen_num));
        set_something(widget_array[k_draw0],XmNbackground,
          (char*)WhitePixel(display_set,  screen_num));
	}
else
	{
        set_something(widget_array[k_draw1],XmNbackground,
                      (void*) draw1_bg);
        set_something(widget_array[k_mis1],XmNbackground,
                      (void*) mis1_bg);
        set_something(widget_array[k_ord1],XmNbackground,
                      (void*) ord1_bg);
        set_something(widget_array[k_tim1],XmNbackground,
                      (void*) tim1_bg);
        set_something(widget_array[k_tempo],XmNbackground,
                      (void*) tempo_bg); 
        set_something(widget_array[k_val1],XmNbackground,
                      (void*) val1_bg); 
        set_something(widget_array[k_form],XmNbackground,
                      (void*) form_bg); 
        set_something(widget_array[k_draw0],XmNbackground,
                      (void*) draw0_bg); 
	}
}

/*********************************
*********************************/
void activate_proc(w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;
{
int widget_num = (int)tag;              /* Convert tag to widget number. */
Position pos_y;

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

switch(widget_num)
  {
  case k_selgr_dialog:
  case k_memgr_dialog:
  /* se errore in apertura files gruppi */
  if(open_gruppi())  
    {
    WidErrore(main_window_widget,err_crea_gruppi,MAPPA);
    return;
    }
  break;
  }

if (widget_array[widget_num] == NULL)
  {                         
  if(widget_crea[widget_num]!=NULL)
    {
    widget_array[widget_num]=widget_crea[widget_num](pXlGraf);
    }
  }

if (widget_array[widget_num] == NULL)
  printf("\n Errore creazione dialog");
else
  {
  get_something(widget_array[k_draw1],XmNy,(void*) &pos_y);
  set_something(widget_array[widget_num],XmNy,(void*) pos_y);
  XtManageChild(widget_array[widget_num]);
  }
}

/******************************
******************************/
void resize_proc(w, tag, str)
    Widget w;
    int *tag;
    XmDrawingAreaCallbackStruct *str;
{
int widget_num = (int)tag;

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

if(widget_num==k_draw1)
	resize=1;
if(str->window!=0)
XClearArea(display_set,str->window,0,0,0,0,True);
}

/******************************
******************************/
void create_proc(w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;
{
int widget_num =(int)tag;
int i;

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);
widget_array[widget_num] = w;

switch(widget_num)
	{
	case k_draw1:
	get_something(w,XmNwidth,(void*) &draw_width);
	get_something(w,XmNheight,(void*) &draw_height);
	draw_height-=2;
/**
        handle_motion(w);
**/
	sg.w_draw=w;
	break;
	case k_mis1:
	for(i=0;i<4;i++)
		{
		sg.x_descr_mis[i]=
			XmStringCreateLtoR(sg.descr_mis[i],
			XmSTRING_DEFAULT_CHARSET);
		}
	sg.w_mis=w;
	break;
	case k_tim1:
	sg.w_tim=w;
	break;
	case k_ord1:
	sg.w_ord=w;
	break;
	case k_val1:
	sg.w_val=w;
	break;
	case k_selmis_box:
	x_simboli=(XmString *)XtMalloc((header2.ncasi+1)*
			  sizeof(XmString));
	for(i=0;i<header2.ncasi;i++)
		{
		x_simboli[i]=XmStringCreateLtoR(simboli[i],
			XmSTRING_DEFAULT_CHARSET);
		}
	x_simboli[i]=NULL;

        for(i=0;i<header2.ncasi;i++)
                {
                XmListAddItem(w,x_simboli[i],0); 
                }
	break;
	case k_toggle_mis1:
	case k_toggle_mis2:
	case k_toggle_mis3:
	case k_toggle_mis4:
        x_sel_var[widget_num-k_toggle_mis1]=
	XmStringCopy(sg.x_descr_mis[widget_num-k_toggle_mis1]);
	set_something(w,XmNlabelString,
 			(void*) x_sel_var[widget_num-k_toggle_mis1]);
	break;
	case k_toggle2_mis1:
	case k_toggle2_mis2:
	case k_toggle2_mis3:
	case k_toggle2_mis4:
	x_sel_var[widget_num-k_toggle2_mis1]=sg.x_descr_mis[widget_num-k_toggle2_mis1];
	set_something(w,XmNlabelString,(void*) x_sel_var[widget_num-k_toggle2_mis1]);
	break;
	case k_toggle3_mis1:
	case k_toggle3_mis2:
	case k_toggle3_mis3:
	case k_toggle3_mis4:
	x_sel_var[widget_num-k_toggle3_mis1]=sg.x_descr_mis[widget_num-k_toggle3_mis1];
	set_something(w,XmNlabelString,(void*) x_sel_var[widget_num-k_toggle3_mis1]);
	break;
	case k_list_gr:
	read_gruppi(1);
        for(i=0;i<NUM_GRUPPI;i++)
        	{
		XmListAddItem(widget_array[k_list_gr],x_gruppi[i],0);
                }
	break;
	case k_list_memgr:
	read_gruppi(1);
        for(i=0;i<NUM_GRUPPI;i++)   /* per VMS   */
        	{
		XmListAddItem(widget_array[k_list_memgr],x_gruppi[i],0);
                }
	break;
	case k_list_defumis:
	crea_lista_umis();
        for(i=0;i<num_umis;i++)
                {
                umis_defsel[i]=uni_mis[i].sel;
		XmListAddItem(widget_array[k_list_defumis],x_codumis[i],0);
                }
	break;
	}
}

/******************************
*******************************/
void map_proc(w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;
{
int widget_num = *tag;
XmString x_void;
int i,j;
int numis,selumis;
float fmin,fmax;

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

switch(widget_num)
	{
	case k_defumis_dialog:
	     XmListSelectItem(widget_array[k_list_defumis],x_codumis[0],True);
	break;
	case k_selmis_dialog:
	XmToggleButtonSetState(widget_array[k_toggle_mis1],True,True);
	for(i=k_toggle_mis1;i<=k_toggle_mis4;i++)
		{
 		x_sel_var[i-k_toggle_mis1]=
            	XmStringCopy(sg.x_descr_mis[i-k_toggle_mis1]);
       		set_something(widget_array[i],XmNlabelString,
			(void*) x_sel_var[i-k_toggle_mis1]);
		}
	break;
	case k_umis_dialog:
	var_attiva=0;
	XmToggleButtonSetState(widget_array[k_toggle2_mis1],True,True);
	for(i=k_toggle2_mis1;i<=k_toggle2_mis4;i++)
		{
		x_sel_var[i-k_toggle2_mis1]=sg.x_descr_mis[i-k_toggle2_mis1];
		set_something(widget_array[i],XmNlabelString,
				(void*) x_sel_var[i-k_toggle2_mis1]);
		}
	selumis=sg.umis_sel[var_attiva];
	numis=sg.ind_umis[var_attiva];
	XmToggleButtonSetState(widget_array[k_toggle_umis1+selumis],True,True);
	for(i=k_toggle_umis1;i<=k_toggle_umis4;i++)
		{
		set_something(widget_array[i],XmNlabelString,
                (void*) XmStringCreateLtoR(uni_mis[numis].codm[i-k_toggle_umis1],
                                   XmSTRING_DEFAULT_CHARSET));
		}
/* memorizza il settaggio delle unita' di misura come appoggio */
	for(i=0;i<4;i++)
		app_umis_sel[i]=sg.umis_sel[i];
	break;
	case k_fsca_dialog:
	for(i=k_toggle3_mis1;i<=k_toggle3_mis4;i++)
		{
		x_sel_var[i-k_toggle3_mis1]=sg.x_descr_mis[i-k_toggle3_mis1];
		set_something(widget_array[i],XmNlabelString,(void*) x_sel_var[i-k_toggle3_mis1]);
		}
	for(i=0;i<4;i++)
		{
		autoscalingG[i]=sg.autoscaling[i];
                app_max[i]=(char *)XtCalloc(20,sizeof(char));
                app_min[i]=(char *)XtCalloc(20,sizeof(char));

		if(sg.ind_mis[i]!=-1)
			{
                        if(sg.fix_min_max[i].min==(-1)&&
                            sg.fix_min_max[i].max==(-1))
                        {
                        sg.fix_min_max[i].min= sg.sel_min_max[i].min;
                        sg.fix_min_max[i].max= sg.sel_min_max[i].max;
                        }

			fmin=sg.fix_min_max[i].min *
   			uni_mis[sg.ind_umis[i]].A[sg.umis_sel[i]]+
    			uni_mis[sg.ind_umis[i]].B[sg.umis_sel[i]];
			fmax=sg.fix_min_max[i].max *
    		        uni_mis[sg.ind_umis[i]].A[sg.umis_sel[i]]+
     		        uni_mis[sg.ind_umis[i]].B[sg.umis_sel[i]];
                        formatta(app_max[i],fmax);
                        formatta(app_min[i],fmin);

			}
		else
			{
			sprintf(app_max[i],"             ");
			sprintf(app_min[i],"             ");
			}
		}
	XmToggleButtonSetState(widget_array[k_toggle3_mis1],True,True);
	toggle3_mis[0]=1;

	XmTextSetString(widget_array[k_text_max],app_max[0]);
	XmTextSetString(widget_array[k_text_min],app_min[0]);
	if(autoscalingG[0])
		XmToggleButtonSetState(widget_array[k_toggle_auto],True,True);
	else
		XmToggleButtonSetState(widget_array[k_toggle_man],True,True);

	break;
	case k_selgr_dialog:
	x_void=XmStringCreateLtoR("   ",XmSTRING_DEFAULT_CHARSET);
	set_something(widget_array[k_label_grmis1],XmNlabelString,(void*) x_void);
	set_something(widget_array[k_label_grmis2],XmNlabelString,(void*) x_void);
	set_something(widget_array[k_label_grmis3],XmNlabelString,(void*) x_void);
	set_something(widget_array[k_label_grmis4],XmNlabelString,(void*) x_void);
	XtFree((char*)x_void);
	break;
        case k_dir_dialog:
        for(i=0;i<NUM_PATH_FILES;i++)
           XmTextSetString(widget_array[k_text_dir1+i],path[i]);
	XmToggleButtonSetState(widget_array[k_toggle_dir1],True,True);
        break;

	}
}

/******************************
******************************/
void formatta(str,fval)
char *str;
float fval;
{
if(fval>999999.9 || fval<-99999.9 || (fval<0.01 && fval>-0.01))
        sprintf(str,"%9.3E",fval);
else
        sprintf(str,"%9.3f",fval);
}

/*******************************
   gestisce il movimento del mouse nella
   window del grafico (per visualizzazione
   valore e per zoom
*********************************/
void handle_motion(w)
Widget w;
{
XtAddEventHandler(w,PointerMotionMask,False,MoveMouse,NULL);
/* crea il cursore per collimazione   */
cursor_coll=XCreateFontCursor(display_set,XC_sb_h_double_arrow);
cursor_zoom=XCreateFontCursor(display_set,XC_cross);
}

/***************************
 * uscita dalla dialog box di selezione misura
 ***************************/
void ok_proc(w, tag, selez)
    Widget w;
    int *tag;
    XmSelectionBoxCallbackStruct *selez;
{
int flag;
int i,j;
int k;
int indice;
int iumis,iumis_sel;
char *path_app;
char *pmax,*pmin;
float fmax,fmin;
double dfmax,dfmin;
int ind_widget;
S_GRAFICO *s;

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);
s=(&sg);
ind_widget=(int)tag;
switch(ind_widget)
	{
        case k_defumis_dialog:
        for(i=0;i<num_umis;i++)
                {
                uni_mis[i].sel=umis_defsel[i];
                }
        agg_umis();
        free_lista_umis();
        XtDestroyWidget(XtParent(widget_array[k_defumis_dialog]));
        widget_array[k_defumis_dialog]=0;
	break;

	case k_selmis_dialog:
	scala_unica=0;
	for(i=0;i<4;i++)
		{
		s->x_descr_mis[i]=x_sel_var[i];
/*
	cerca la stringa nella lista determinandone l'indice
*/
		indice=x_cerca_stringa(s->x_descr_mis[i],x_simboli);
		if(indice!=-1)
			s->ind_mis[i]=indice;
		else 
			s->ind_mis[i]=(-1);
		
		if(s->ind_mis[i]!=-1)
			{
			strcpy(s->descr_mis[i],simboli[indice]);
			(s->ind_umis[i])=cerca_umis(s->descr_mis[i],0);
			if(s->ind_umis[i]==-1)
				{
				s->ind_umis[i]=num_umis-1;
				} 
			s->autoscaling[i]=1;
			}
		}
/*
 rilegge il file in modo da ricalcolare i valori di minimo e massimo
 per le variabili selezionate
*/
	rew_dati(pXlGraf);
	set_min_max1(NULL,pXlGraf);
	read_22datGR(AGGIORNA,pXlGraf);

	if(notify_overflow == True)
		WidAttenzione(topLevel_set,"Too many data for trend buffer!",MAPPA);

	for(i=0;i<4;i++)
  		{
  		if(s->ind_mis[i]!=-1)
    			{

/*
  e' settato l'autoscaling: forza la ricerca di minimo e massimo
  modificando il valore del massimo.
*/
			s->sel_min_max[i].max=s->var_min_max[i].max+1;
			s->umis_sel[i]=uni_mis[s->ind_umis[i]].sel;
			set_scala(i,pXlGraf); 
                        s->fix_min_max[i].max=(-1);
                        s->fix_min_max[i].min=(-1);
			}
		else /* caso di misura non valida o eliminata */
			{
			strcpy(s->descr_mis[i],descr_vuota);
			set_ordinate(i,pXlGraf); /* setta a blank le
                                               ordinate visualizzate */
			}
		}
	XtUnmanageChild (widget_array[k_selmis_dialog]);
        t_old=0.0;
        rew_dati(pXlGraf);
	freeza=0;
        XClearArea(display_set,XtWindow(s->w_mis),0,0,0,0,True);
	break;

	case k_selzoom_dialog:
	i=0;
	while(i<NUM_BOOLEAN_ZOOM && toggle_zoom[i]==0) 
		i++;
	if(i==NUM_BOOLEAN_ZOOM) s->zoom=0;
	else s->zoom=i;
	s->pendente=0;
	/*XtUnmanageChild (widget_array[k_selzoom_dialog]);  */
	XClearArea(display_set,XtWindow(s->w_tim),0,0,0,0,True); 
	XClearArea(display_set,XtWindow(s->w_draw),0,0,0,0,True);
	break;
	case k_umis_dialog:
	for(i=0;i<4;i++)
		{
		s->umis_sel[i]=app_umis_sel[i];
		set_ordinate(i,pXlGraf);
                if(sg.ind_mis[i]!=-1)
                        sg.ultimo[i]=bufdati[n_last-1].mis[i];
                iumis=sg.ind_umis[i];
                iumis_sel=sg.umis_sel[i];
                sg.ultimo[i]=sg.ultimo[i] *
                                uni_mis[iumis].A[iumis_sel]+
                                uni_mis[iumis].B[iumis_sel];
		}
	XtUnmanageChild (widget_array[k_umis_dialog]);
	XClearArea(display_set,XtWindow(s->w_ord),0,0,0,0,True);
	XClearArea(display_set,XtWindow(s->w_val),0,0,0,0,True);
	XClearArea(display_set,XtWindow(s->w_mis),0,0,0,0,True);
	break;
	case k_fsca_dialog:
	scala_unica=0;
	i=0;
	while(i<NUM_BOOLEAN_MIS && toggle3_mis[i]==0) 
		i++;
	if(i==NUM_BOOLEAN_MIS) return;
	var_attiva=i;
	i=0;

/* estrae i valori settati per l'ultima variabile selezionata  */

	if(s->ind_mis[var_attiva]!=-1)
		{
		free(app_max[var_attiva]);
		free(app_min[var_attiva]);
		app_max[var_attiva]=XmTextGetString(widget_array[k_text_max]);
		app_min[var_attiva]=XmTextGetString(widget_array[k_text_min]);
		}

	for(i=0;i<4;i++)
		{
		if(s->ind_mis[i]!=-1)
		{
		s->autoscaling[i]=autoscalingG[i];
/*
  se non e' selezionato l'autoscaling setta i valori minimo e massimo
  utilizzando i valori specificati dall'utente; se i valori non sono
  accettabili segnala l'errore e forza in modalita' di autoscaling.
*/
		if(s->autoscaling[i]==0)
			{
			pmax=app_max[i];
			pmin=app_min[i];
			fmax=atof(pmax);
			fmin=atof(pmin);
			if(fmax>fmin)   /* valori accettabili */
				{
fmin=(fmin-uni_mis[s->ind_umis[i]].B[s->umis_sel[i]])/
      uni_mis[s->ind_umis[i]].A[s->umis_sel[i]];
fmax=(fmax-uni_mis[s->ind_umis[i]].B[s->umis_sel[i]])/
      uni_mis[s->ind_umis[i]].A[s->umis_sel[i]];
				s->fix_min_max[i].max=fmax;
				s->fix_min_max[i].min=fmin;
				set_scala(i,pXlGraf);
				}
			else
				s->autoscaling[i]=1;
			}
		else   /* e' richiesto l'autoscaling */
			{
			set_scala(i,pXlGraf);
			}
		}
		free(app_max[i]);
		free(app_min[i]);
		}
	for(i=0;i<NUM_BOOLEAN_MIS;i++) toggle3_mis[i]=0; 
	for(i=0;i<NUM_BOOLEAN_FSCA;i++) toggle_fsca[i]=0; 
	XtUnmanageChild(widget_array[k_fsca_dialog]);
        t_old=0.0;
	XClearArea(display_set,XtWindow(s->w_ord),0,0,0,0,True);
	XClearArea(display_set,XtWindow(s->w_val),0,0,0,0,True);
	XClearArea(display_set,XtWindow(s->w_draw),0,0,0,0,True);
	break;
	case k_selgr_dialog:
	scala_unica=0;
	memcpy(&s->autoscaling[0],&gruppi[indice_gruppo].gr,sizeof(S_GRUPPO));
	close_gruppi();
	XtDestroyWidget(XtParent(widget_array[k_selgr_dialog]));
	widget_array[k_selgr_dialog]=0;
	for(i=0;i<4;i++)
		{
		XtFree((char*)s->x_descr_mis[i]);
		s->x_descr_mis[i]=XmStringCreateLtoR(s->descr_mis[i],XmSTRING_DEFAULT_CHARSET);
		x_sel_var[i]=s->x_descr_mis[i]; 
		s->ind_mis[i]=cerca_nome(s->descr_mis[i]);
		}
/*
 rilegge il file in modo da ricalcolare i valori di minimo e massimo
 per le variabili selezionate
*/
	rew_dati(pXlGraf);
	set_min_max1(NULL,pXlGraf);
	read_22datGR(AGGIORNA,pXlGraf);
	if(notify_overflow == True)
	WidAttenzione(topLevel_set,"Too many data for trend buffer!",MAPPA);

	for(i=0;i<4;i++)
  		{
		if(s->ind_mis[i]!=-1)
			{
/*
  se e' settato l'autoscaling forza la ricerca di minimo e massimo
  modificando il valore del massimo.
*/
			if(s->autoscaling[i]==1)
				s->sel_min_max[i].max=s->var_min_max[i].max+1;
			set_scala(i,pXlGraf);
			}
		else   /* misura non presente: setta a blank le stringhe
                          delle ordinate  */
			set_ordinate(i,pXlGraf);
		}
        t_old=0.0;
        rew_dati(pXlGraf);
	freeza=0;
        XClearArea(display_set,XtWindow(s->w_mis),0,0,0,0,True);
	break;

	case k_memgr_dialog:
	gruppi[indice_gruppo].pieno=1;
	pmax=XmTextGetString(widget_array[k_text_memgr]);
	strcpy(gruppi[indice_gruppo].descr,pmax);
	XtFree(pmax);
	memcpy(&gruppi[indice_gruppo].gr,&s->autoscaling[0],sizeof(S_GRUPPO));
	if(write_gruppo(indice_gruppo))
		WidErrore(main_window_widget,err_write_gruppi,MAPPA);
	close_gruppi();
	XtDestroyWidget(XtParent(widget_array[k_memgr_dialog]));
	widget_array[k_memgr_dialog]=0;
	break;
        case k_dir_dialog:
        /* Inizializza  gli indici in lista alle variabili visualizzate */
        XtUnmanageChild (widget_array[k_dir_dialog]);

	freeza=1;
        timer=0;

        for(k=0;k<4;k++)
            {
            sg.ind_mis[k]=(int)-1;
            sg.autoscaling[k]=1;
            }
        sg.zoom=0;

        if(nofile==0)  /* se era gia' stato selezionato un
                                 files */
                {
                if(widget_array[k_selmis_dialog]!=0)
                        {
                        XtDestroyWidget(XtParent(widget_array[k_selmis_dialog]));
                        for(i=0;i<header2.ncasi;i++)
                                {
                                XtFree((char*)x_simboli[i]);
                                }
                        free(x_simboli);
			}

                for(i=0;i<4;i++)
                      {
                      XtFree((char*)sg.x_descr_mis[i]);
                         sg.x_descr_mis[i]=XmStringCreateLtoR("    ",XmSTRING_DEFAULT_CHARSET);
                     }
                close_22dat();
                XtFree(path_22dat);
                d2free(simboli);
                widget_array[k_selmis_dialog]=0;
		XClearArea(display_set,XtWindow(sg.w_draw),0,0,0,0,True);
		XClearArea(display_set,XtWindow(sg.w_ord),0,0,0,0,True);
		XClearArea(display_set,XtWindow(sg.w_val),0,0,0,0,True);
		XClearArea(display_set,XtWindow(sg.w_tim),0,0,0,0,True);
		XClearArea(display_set,XtWindow(sg.w_mis),0,0,0,0,True);
		XClearArea(display_set,XtWindow(widget_array[k_tempo]),
                           0,0,0,0,True);
                }

/*
  memorizza la situazione di tutti i 4 path names
*/
        for(i=0;i<NUM_PATH_FILES;i++)
                {
                path_app=XmTextGetString(widget_array[k_text_dir1+i]);
                strcpy(path[i],path_app);
                XtFree(path_app);
                }

        i=0;
        while(i<NUM_BOOLEAN_DIR && toggle_dir[i]==0)
                i++;
        if(i==NUM_BOOLEAN_DIR) 
	{
	       WidAttenzione(main_window_widget,err_file_nonspec,MAPPA);
                nofile=1;
                strcpy(file_vis,no_file_sel);
		abilita_menu(nofile);
                XClearArea(display_set,XtWindow(widget_array[k_draw0]),
                           0,0,0,0,True);
                break; /* esce se errore in apertura */
	}
        path_22dat=XmTextGetString(widget_array[k_text_dir1+i]);
        strcpy(file_vis,path_22dat);
        if(open_22datGR(pXlGraf))
                {
                WidAttenzione(main_window_widget,err_file_nones,MAPPA);
                nofile=1;
                strcpy(file_vis,no_file_sel);
                XClearArea(display_set,XtWindow(widget_array[k_draw0]),
                           0,0,0,0,True);
                break; /* esce se errore in apertura */
                }
/* se e' gia' aperto un file obbliga l'uscita */
/*        nofile=0; */
        flag=TUTTI;
        if(read_22datGR(flag,pXlGraf)==1)  /* legge tutti i dati dall'inizio
 del file */
                {
                close_22dat();
                open_22datGR(pXlGraf);
                if(read_22datGR(flag,pXlGraf)==1)
                        {
/*
   file non esistente
 */
                        close_22dat();
                        XtFree(path_22dat);
                        d2free(simboli);
                        widget_array[k_selmis_dialog]=0;
                        strcpy(file_vis,no_file_sel);
                        nofile=1;
                        }
                }
/* !!!! */
	if(notify_overflow == True)
		WidAttenzione(topLevel_set,"Too many data for trend buffer!",MAPPA);
	nofile=0;
/*
 legge tempo finale e tempo iniziale dal buffer dei dati
*/
	t_finale=bufdati[n_last-1].t;
	t_iniziale=bufdati[0].t;
	abilita_menu(nofile);
        XClearArea(display_set,XtWindow(widget_array[k_draw0]),0,0,0,0,True);
        break;
	}
}

/******************************
******************************/
void cancel_proc(w, tag, selez)
    Widget w;
    int *tag;
    XmSelectionBoxCallbackStruct *selez;
{
int i;
int indice_widget;
indice_widget=(int)tag;

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

switch(indice_widget)
	{
	case k_selmis_dialog:
	XtUnmanageChild (widget_array[k_selmis_dialog]);
	break;
	case k_selzoom_dialog:
	XtUnmanageChild (widget_array[k_selzoom_dialog]);
	break;
	case k_umis_dialog:
	XtUnmanageChild (widget_array[k_umis_dialog]);
	break;
	case k_fsca_dialog:
	for(i=0;i<4;i++)
		{
		free(app_max[i]);
		free(app_min[i]);
		toggle3_mis[i]=0; 
		toggle_fsca[i]=0; 
		}
	XtUnmanageChild (widget_array[k_fsca_dialog]);
	break;
	case k_selgr_dialog:
	close_gruppi();
	XtDestroyWidget(XtParent(widget_array[k_selgr_dialog]));
	widget_array[k_selgr_dialog]=0;
	break;
	case k_memgr_dialog:
	close_gruppi();
	XtDestroyWidget(XtParent(widget_array[k_memgr_dialog]));
	widget_array[k_memgr_dialog]=0;
	break;
	case k_defumis_dialog:
	free_lista_umis();
	XtDestroyWidget(XtParent(widget_array[k_defumis_dialog]));
	widget_array[k_defumis_dialog]=0;
	break;
	case k_dir_dialog:
        XtDestroyWidget(XtParent(widget_array[k_dir_dialog]));
        widget_array[k_dir_dialog]=0;
	break;
	}
}

/*******************************
*******************************/
void apply_proc(w, tag, selez)
    Widget w;
    int *tag;
    XmListCallbackStruct *selez;
{
XmString x_app;
int i,j1;
int indice_widget;
i=0;
indice_widget=(int)tag;

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

switch(indice_widget)
    {
    case k_list_memgr:
	indice_gruppo=x_cerca_stringa(selez->item,x_gruppi);
        XmTextSetString(widget_array[k_text_memgr],
                gruppi[indice_gruppo].descr);
	break;
    case k_list_gr:
	indice_gruppo=x_cerca_stringa(selez->item,x_gruppi);
	set_something(widget_array[k_label_grmis1],XmNlabelString,
                     (void*) XmStringCreateLtoR(gruppi[indice_gruppo].gr.descr_mis[0],
					XmSTRING_DEFAULT_CHARSET));
	set_something(widget_array[k_label_grmis2],XmNlabelString,
                     (void*) XmStringCreateLtoR(gruppi[indice_gruppo].gr.descr_mis[1],
							XmSTRING_DEFAULT_CHARSET));
	set_something(widget_array[k_label_grmis3],XmNlabelString,
                     (void*) XmStringCreateLtoR(gruppi[indice_gruppo].gr.descr_mis[2],
							XmSTRING_DEFAULT_CHARSET));
	set_something(widget_array[k_label_grmis4],XmNlabelString,
                     (void*) XmStringCreateLtoR(gruppi[indice_gruppo].gr.descr_mis[3],
							XmSTRING_DEFAULT_CHARSET));
	break;

    case k_selmis_elimina:
	while(i<NUM_BOOLEAN_MIS && toggle_mis[i]==0) 
		i++;
	if(i==NUM_BOOLEAN_MIS) return;
/*
  i indica la variabile da modificare
*/
	x_sel_var[i]=XmStringCreateLtoR("   ",XmSTRING_DEFAULT_CHARSET);
	set_something(widget_array[k_toggle_mis1+i],XmNlabelString,(void*) x_sel_var[i]);
	break;
    case k_list_defumis:
	indice_umis=x_cerca_stringa(selez->item,x_codumis);
	for(i=0;i<4;i++)
		{
		x_app=XmStringCreateLtoR(uni_mis[indice_umis].codm[i],XmSTRING_DEFAULT_CHARSET);
		set_something(widget_array[k_toggle2_umis1+i],XmNlabelString,(void*) x_app);
		XtFree((char*)x_app);
		}
	XmToggleButtonSetState(widget_array[k_toggle2_umis1+umis_defsel[indice_umis]],True,True);
	break;
    default:
	while(i<NUM_BOOLEAN_MIS && toggle_mis[i]==0) 
		i++;
	if(i==NUM_BOOLEAN_MIS) return;
/*
  i indica la variabile da modificare
*/
	x_sel_var[i]=XmStringCopy(selez->item);
	set_something(widget_array[k_toggle_mis1+i],XmNlabelString,(void*) selez->item);
	break;
    }
}

/****************************
****************************/
void find_proc(w, tag, selez)
    Widget w;
    int *tag;
    XmSelectionBoxCallbackStruct *selez;
{
Widget wbar;
int i,trovata, j;
int indice_widget;
char *pstr;   /* puntatore alla stringa editata (da ricercare) */

/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);

i=0;
pstr=XmTextGetString(widget_array[k_find_text]);
trovata=0;
indice_widget=(int)tag;
switch(indice_widget)
    {
    case k_find_next:
    i=find_pos;
    do
	{
    	if(i<header2.ncasi)
		i++;
    	else
		i=0;
	if(Utstrstr(simboli[i],pstr)!=NULL) /* se la stringa e' stata 
                                              trovata  */
		{
		trovata=1;
		find_pos=i;
		break;
		}
	}
    while(i!=find_pos);

    if(trovata)   /* se la variabile e' stata trovata la seleziona nella
                     lista visualizzata  */
	{
	UtListaScroll(widget_array[k_selmis_box],header2.ncasi,8,find_pos);
	}
    break;
    case k_find_previous:
    i=find_pos;
    do
	{
    	if(i>0)
		i--;
    	else
		i=header2.ncasi;
	if(Utstrstr(simboli[i],pstr)!=NULL) /* se la stringa e' stata 
                                              trovata  */
		{
		trovata=1;
		find_pos=i;
		break;
		}
	}
    while(i!=find_pos);

    if(trovata)   /* se la variabile e' stata trovata la seleziona nella
                     lista visualizzata  */
	{
	UtListaScroll(widget_array[k_selmis_box],header2.ncasi,8,find_pos);
	}
    break;
    }
XtFree(pstr);
}

/****************************
 *  Predispone per HardCopy in bianco e nero.
 ****************************/
void HC_proc (w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;

{
/* pid_t  fpid; */
char app[20];
int j;
/*  Ricavo il puntatore alla struttutra delle variabili dall'ID dell'oggetto */
GetPuntXlGrafico (w);
if(HC_on)
        {
        HC_on=0;
	freeza=0;
	}
else
        {
        HC_on=1;
        if(timer)
                {
		freeza=1;
                timer=0;
                }
        }
reverse_draw(HC_on);
resize=1; /* per forzare il ridisegno della griglia */
XClearArea(display_set,XtWindow(sg.w_draw),0,0,0,0,True);
XClearArea(display_set,XtWindow(sg.w_mis),0,0,0,0,True);
XClearArea(display_set,XtWindow(sg.w_val),0,0,0,0,True);
XClearArea(display_set,XtWindow(sg.w_tim),0,0,0,0,True);
XClearArea(display_set,XtWindow(sg.w_ord),0,0,0,0,True);
XClearArea(display_set,XtWindow(widget_array[k_tempo]),0,0,0,0,True);
/*
	strcpy(app,"hardcopy.sh");
	fpid=fork();
	if (fpid == 0)
	{
		execlp(app,0);
		exit(0);
	}
*/
}

/****************************
****************************/
void abilita_menu(flag)
int flag;
{
int valore;
if(flag)
	valore=False;
else
	valore=True;
set_something(widget_array[k_grafici_menu_entry],XmNsensitive,(void*) valore);
set_something(widget_array[k_misure_menu_entry],XmNsensitive,(void*) valore);
}


/******************************
 *  crea_lista_umis
 *     crea le lista dei codici delle unita' di misura
 ******************************/
void crea_lista_umis()
{
int i;
for(i=0;i<num_umis;i++)
    x_codumis[i]=XmStringCreateLtoR(uni_mis[i].codice,XmSTRING_DEFAULT_CHARSET);
x_codumis[num_umis]=NULL;
}

void free_lista_umis()
{
int i;
for(i=0;i<num_umis;i++)
        {
        XtFree((char*)x_codumis[i]);
        }
}

/**********************************************************
 *  set_min_max1
 *      aggiorna i valori di minimo e massimo per ogni variabile 
 *      appartenente al record.
 *		se il parametro passato come argomento e' =NULL inizializza
 *      a valori estremi i valori di minimo e massimo.
 **********************************************************/
void set_min_max1(rec,pXlGraf)
S_DATI *rec;   /* record dati */
S_XLGRAFICO *pXlGraf;
{
register int i;
float delta;
int variato;
for(i=0;i<4;i++)
  {
  if(rec==NULL)
    {
    sg.var_min_max[i].max=(-1.0E-37);
    sg.var_min_max[i].min=1.0E+38;
    }
  else
    {
    variato=0;
    if(sg.var_min_max[i].min>rec->mis[sg.ind_mis[i]])
      {
      sg.var_min_max[i].min=rec->mis[sg.ind_mis[i]];
      variato=1;
      }

    if(sg.var_min_max[i].max<rec->mis[sg.ind_mis[i]])
      {
      sg.var_min_max[i].max=rec->mis[sg.ind_mis[i]];
      variato=1;
      }

    if(variato && (sg.var_min_max[i].max>=sg.var_min_max[i].min)
          && (sg.var_min_max[i].max-sg.var_min_max[i].min)<=
			(0.001*sg.var_min_max[i].max))
      {
      delta=sg.var_min_max[i].max * 0.001; 
      if(delta <0.001)
        delta=0.001;
      sg.var_min_max[i].max+=delta;
      sg.var_min_max[i].min-=delta;
      }
    }
  }

}
