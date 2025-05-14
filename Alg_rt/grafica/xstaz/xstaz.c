/**********************************************************************
*
*       C Source:               xstaz.c
*       Subsystem:              1
*       Description:
*       %created_by:    ciccotel %
*       %date_created:  Mon Jun 17 15:38:12 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: xstaz.c-4 %  (%full_filespec: xstaz.c-4:csrc:1 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)xstaz.c	1.13\t9/13/95";
/*
   modulo xstaz.c
   tipo 
   release 1.13
   data 9/13/95
   reserved @(#)xstaz.c	1.13
*/
/*
* XSTAZ
 *   processo staz realizzato sotto MOTIF 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>

#include <Xm/Xm.h>
#include <X11/Xlib.h>
#include <X11/Shell.h>
#include <Mrm/MrmAppl.h>                /* Motif Toolkit and MRM */
#include <Xm/MainW.h>
#include <Xm/BulletinB.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/ScrolledW.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/RowColumn.h>

#if defined UNIX
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>

#elif defined VMS
# include "vmsipc.h"
#endif

/* exit( puts( "ATTENZIONE: compilare con -DAIX (o -DVMS / -DULTRIX)"));*/
#include <Rt/RtDbPunti.h>
#include <Rt/RtErrore.h>
#include "sim_param.h"
#include "sim_ipc.h"
#include "sim_types.h"
#include "comandi.h"
#include "xstaz.h"
#include "compstaz.h"

/* include tabella di definizione delle nuove stazioni TIPI_NEW_STAZ */
#include "newstaz.h"
//#include "libutilx.h"


/* definizioni per la coda di messaggio delle perturbazioni 
   per macchine AIX e ULTRIX				    */

int id_msg_pert;
MSG_PERT messaggio;
/*
 Definizione callbacks functions
 */

static void staz_proc();
static void timer_proc();
static void pag_del_callback();  /* callback di distruzione window */
static void staz_del_callback();  /* callback di uscita dal programma */
static void PostIt();
/*
 Definizione funzioni di utilita'
 */
float estr_sh();


/*
 * Dati globali necessari per le routines di grafica
 */
Display *display;
int screen_num;
XFontStruct *font_info;
XFontStruct *font_info_big;
XmFontList fontlist = NULL;
XmFontList fontlist_big = NULL;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;


char *colori[]={"black","white","red","light gray","SeaGreen"};
XColor excolor[12];


/* gestione colori per nuove staz */

#define	NUM_COLORI 7
char *colori_new[]={"black","white","light yellow","sea green","orange","light gray","dark turquoise"};

char *colori_blink[]={"black","white","yellow","spring green","red","gray50","blue"};

XColor excolor_new[NUM_COLORI]={
                  {0L,0x0000,0x0000,0x0000,0,0},    /* nero */
                  {0L,0xcb00,0xcb00,0xcb00,0,0},    /* bianco */
                  {0L,0xe400,0xc400,0x8800,0,0},    /* giallo scuro */
                  {0L,0x0000,0x8a00,0x0000,0,0},    /* verde scuro */
                  {0L,0xb300,0x0000,0x0000,0,0},    /* rosso scuro */
                  {0L,0x5c00,0x5c00,0x5c00,0,0},    /* grigio scuro */
                  {0L,0x0000,0x0000,0x7600,0,0},    /* blu scuro */
			};

XColor excolor_blink[NUM_COLORI]={
                  {0L,0x0000,0x0000,0x0000,0,0},    /* nero */
                  {0L,0xff00,0xff00,0xff00,0,0},    /* bianco */
                  {0L,0xff00,0xff00,0x0000,0,0},    /* giallo chiaro */
		  {0L,0x0000,0xff00,0x0000,0,0},    /* verde chiaro */
		  {0L,0xff00,0x3200,0x0000,0,0},    /* rosso chiaro */
		  {0L,0xcb00,0xcb00,0xcb00,0,0},    /* grigio chiaro */
		  {0L,0x0000,0xe500,0xff00,0,0},    /* blu chiaro */
			};

XColor sfondo_label={0L,0xcc00,0xe500,0xcb00,0,0};
XColor sfondo_window={0L,0xcc00,0xe500,0xe500,0,0};
XColor sfondo_scroll={0L,0x0000,0xe500,0xff00,0,0};
XColor sfondo_staz=  {0L,0xd500,0xe500,0xd500,0,0};
XColor sfondo_bianco={0L,0xff00,0xff00,0xff00,0,0};

/***************/

int font_ascent;
int font_height;
int font_width;
GC gc[5];   

S_PAGINA *pagina;
S_STAZIONE *stazione;
DATI_REFRESH  *trefr_newstaz;
DATI_REFRESH  *tredraw_newstaz;
XtCallbackRec *t_call;
XtCallbackRec *t_call_redraw;
int	*offset_staz;
S_WSTAZIONE *wstaz;

S_PAGVIS pagvis[MAX3_PAG];

XtAppContext app_context;
Widget topLevel;
Arg args[50]; /* per settaggio argomenti widgets */
Widget main_window_widget;
XtIntervalId time_id,time2_id;

int stato_blink =0;		/* variabile di stato generale dei blink
					1 on 0 off */
int     draw_width,draw_height;
HEAD_R02 header;


/* identificatore tipo staz */
int tipo_staz=0;         /* 0 comunicazione con LEGOGRAF */
                         /* 1 comunicazione con MONIT    */
RICHIESTA_STAZ richiesta_staz;
int id_msg_staz;


RtDbPuntiOggetto dbpunti;
RtErroreOggetto errore_obj;

void testata(char *, char *);

int main(argc,argv)
unsigned int argc;                  /* Command line argument count. */
char *argv[];                       /* Pointers to command line args.*/
{
Widget wbutton;
char file[60];
FILE *fp;
int i;
#if defined VMS
char app_id_msg_staz_1[4];
char app_id_msg_staz_2[4];
#endif
SIMULATOR *sim;



int shr_usr_key;            /* chiave utente per shm */
int semid_mod,semid_aux;         /* identificativi semafori          */

/* redirezione output  e stampa versione */
testata("xstaz",SccsID);

if(argc>=2)
        tipo_staz=atoi(argv[1]);

printf("Tipo xstaz = %d\n",tipo_staz);

/*
        si alloca l'area shared
*/
    shr_usr_key = atoi((char *)getenv("SHR_USR_KEY"));
    sim = (SIMULATOR*)calloc(1,sizeof(SIMULATOR));
if(tipo_staz==1)
    {
	errore_obj = RtCreateErrore(RT_ERRORE_TERMINALE,"xstaz");
        dbpunti = RtCreateDbPunti(errore_obj,NULL,DB_PUNTI_SHARED,sim);
    }
else
    {
    /*
	  inizializza cluster event flags e attende flag di animazione
	  legograf
    */
     ef_cluster();
	printf("\n iXSTAZ ATTESA LEGOGRAF ");
     wait_ef(7,1);
     tra_nom_log_s(file,50,"S04_PATH","EASE$LNM");
     chdir(file);
	errore_obj = RtCreateErrore(RT_ERRORE_TERMINALE,"xstaz");
        dbpunti = RtCreateDbPunti(errore_obj,NULL,DB_PUNTI_INT,sim);
    }
free(sim);
                                            
/*   installazione signal    */
signal(SIGTERM,staz_del_callback);

/*
 Aggancio o creazione code di messaggi ambiente di simulazione
*/
msg_create_fam(shr_usr_key,0);
#if defined VMS
memcpy(&app_id_msg_staz_1[0],&id_msg_staz,sizeof(int));
memcpy(&app_id_msg_staz_2[0],&app_id_msg_staz_1[2],sizeof(short));
memcpy(&app_id_msg_staz_2[2],&app_id_msg_staz_1[0],sizeof(short));
memcpy(&id_msg_staz,&app_id_msg_staz_2[0],sizeof(int));
#endif

/* legge il file r02.dat */

fp=fopen("r02.dat","r+");
if (fp == NULL) 
{
	fprintf(stderr,"\n file r02.dat  non esistente ");
	exit(0); 
}

/* lettura header file r02.dat e allocamento area di memoria necessaria */

fread(header.data,sizeof (HEAD_R02), 1, fp);
/*printf("\n num. pag %d  num. staz %d \n",header.tot_pagine,header.tot_staz);*/

pagina=(S_PAGINA *) XtMalloc (header.tot_pagine * sizeof (S_PAGINA));
if (pagina == NULL){perror("errore allocamento PAGINE"); exit(0);}

stazione=(S_STAZIONE *) XtMalloc (header.tot_staz * sizeof (S_STAZIONE));
if (stazione == NULL){perror("errore allocamento STAZIONE"); exit(0);}

offset_staz = (int *) XtMalloc (header.tot_staz * sizeof (int));
if (offset_staz == NULL){perror("errore allocamento offset_staz"); exit(0);}

trefr_newstaz = (DATI_REFRESH *) XtMalloc (header.tot_staz * MAX_WIDGET_STAZ * sizeof (DATI_REFRESH));
if (trefr_newstaz == NULL){perror("errore allocamento trefr_newstaz"); exit(0);}

tredraw_newstaz = (DATI_REFRESH *) XtMalloc (header.tot_staz * MAX_WIDGET_STAZ * sizeof (DATI_REFRESH));
if (tredraw_newstaz == NULL){perror("errore allocamento tredraw_newstaz"); exit(0);}

t_call = (XtCallbackRec *) XtMalloc (header.tot_staz * MAX_WIDGET_STAZ * sizeof ( XtCallbackRec));
if (t_call == NULL){perror("errore allocamento t_call"); exit(0);}

t_call_redraw = (XtCallbackRec *) XtMalloc (header.tot_staz * MAX_WIDGET_STAZ * sizeof ( XtCallbackRec));
if (t_call_redraw == NULL){perror("errore allocamento t_call_redraw"); exit(0);}

wstaz = (S_WSTAZIONE *) XtMalloc (header.tot_staz * sizeof (S_WSTAZIONE));
if (wstaz == NULL){perror("errore allocamento wstaz"); exit(0);}

/* legge il resto di r02.dat */

for (i=0; i< header.tot_pagine; i++)
	fread(&pagina[i],sizeof (S_PAGINA), 1, fp);

fread(offset_staz, header.tot_staz * sizeof (int) , 1, fp);

for (i=0; i< header.tot_staz; i++)
	fread(&stazione[i],sizeof (S_STAZIONE), 1, fp);
fclose(fp);


/*
 lettura files di definizione pagine e stazioni
*/
init_staz();
/********
pr_pagina();
pr_stazione();
******************/



topLevel= XtInitialize("xstaz","Xstaz",NULL,0,
                             &argc,argv);

LoadColor(topLevel);
set_something(topLevel,XmNiconic,(void*) True);
display=XtDisplay(topLevel);
screen_num=DefaultScreen(display);

init_gc();
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("Quit ",
XmSTRING_DEFAULT_CHARSET)); i++;
wbutton=(Widget) XmCreatePushButton(topLevel,"bottone",args,i);
XtAddCallback(wbutton,XmNactivateCallback,staz_del_callback,NULL);
XtManageChild(wbutton); 
XtRealizeWidget(topLevel);
/*
  timer per attesa di messaggio visualizzazione plot
*/
time_id=XtAppAddTimeOut(XtWidgetToApplicationContext(topLevel),
                                   (unsigned long)1000,staz_proc,NULL);
time2_id=XtAppAddTimeOut(XtWidgetToApplicationContext(topLevel),
                                  (unsigned long)TREFRESH_FAST,timer_proc,NULL);
/*
   preleva le dimensioni dello schermo
*/
draw_width=XDisplayWidth(display,screen_num) -10 ;
draw_height=XDisplayHeight(display,screen_num) - 50;

XtMainLoop();
}

static void timer_proc(client_data,id)
caddr_t client_data;
XtIntervalId *id;
{
register int i;
DATI_REFRESH *p_refr;
int tipo_oggetto;
static int cont_ref = 0;

/* verifica se si e' in condizione di refresh lento */
cont_ref = cont_ref +1;
if(cont_ref>DELTA_REF)
	cont_ref=0;

for(i=0;i<header.tot_staz*MAX_WIDGET_STAZ;i++)
  if(t_call[i].callback!=NULL)
	{
	/* 
	  identifica il tipo di oggetto: per le nuove stazioni 
	  il tipo di oggetto si trova nella struttura puntata
	  da t_call[i].closure mentre per le vecchie stazioni
	  il tipo e' contenuto direttamente in t_call[i].closure.
	  Tipicamente, per le nuove stazioni t_call[i].closure essendo
	  un puntatore in memoria sara' >>>>> 1000.
	  Quando verranno definitivamente eliminate le vecchie stazioni
	  sara necessario eliminare il test.
	*/
	if((int)(t_call[i].closure)>1000)
		{  /* test per nuove stazioni  */
		p_refr=(DATI_REFRESH *) t_call[i].closure;
		memcpy(&tipo_oggetto,p_refr->pdati,sizeof(int));
		}
	else
		{  /* test per vecchie stazioni  */
		tipo_oggetto=(int)(t_call[i].closure);
		}
	if(cont_ref==DELTA_REF)
		/*t_call[i].callback(t_call[i].closure);*/
		t_call[i].callback;
	else
		if(tipo_oggetto==INDICATORE_SINCRO)
			t_call[i].callback;
			/*t_call[i].callback(t_call[i].closure);*/
	}
time2_id=XtAppAddTimeOut(XtWidgetToApplicationContext(topLevel),
                                  (unsigned long)TREFRESH_FAST,timer_proc,NULL);
if(cont_ref==DELTA_REF)
	stato_blink = !(stato_blink);
}


static void staz_proc(client_data,id)
caddr_t client_data;
XtIntervalId *id;
{
int i,ip3;

/*
 esamina se legograf in animazione
 */

if (tipo_staz == 0)
  {
    if(!ef_is_set(7,1))   
	{
	for(i=0;i<MAX3_PAG;i++)
		{
		if(pagvis[i].w!=0)
			{
			XtDestroyWidget(pagvis[i].w);
			pagvis[i].attiva=0;
			XSync(display,False);
			pagvis[i].w=0;
			}
		}
        wait_ef(7,1); 
	set_ef(7,1); 	/* per la versione AIX non serve !!!!!!!!!! (per ultrix si)*/
	}
  }
/*
 esamina se e' stata richiesta una nuova pagina di comando
 */

if (tipo_staz == 0)
   {
   if(ef_is_set(5,1))
	{
	for(i=0;i<MAX3_PAG;i++)
		{
		if(!pagvis[i].attiva) break;
		}
	if(i==MAX3_PAG)
		{
fprintf(stderr,"\n warning: raggiunto il massimo numero pagine in visualizzazione");
		}
	else
		{
		ip3=i;   /* indice per memorizzazione pagina */
		cnew_staz(ip3);    /* ricerca la stazione richiesta */
		                  /* e la visualizza */
                clr_ef(5,1);
		}
	}
   }

/*
  se lato schedulatore verifica se e' stata accodata una richiesta
*/
if(tipo_staz==1)
{
   if(msg_rcv(id_msg_staz,&richiesta_staz,sizeof(richiesta_staz.nome_pagina),(long)RIC_STAZ, IPC_NOWAIT,0)>0)
        {
printf(" scodato messaggio \n");
        for(i=0;i<MAX3_PAG;i++)
                {
                if(!pagvis[i].attiva) break;
                }
        if(i==MAX3_PAG)
                {
fprintf(stderr,"\n warning: raggiunto il massimo numero pagine in visualizzazione");
                }
        else
                {
                ip3=i;   /* indice per memorizzazione pagina */
                cnew_staz(ip3);    /* ricerca la stazione richiesta */
                                  /* e la visualizza */
                }
        }
}
/*
  scoda gli eventuali messaggi di redraw ed esegue le callbacks
  di redraw degli oggetti
*/
for(i=0;i<header.tot_staz*MAX_WIDGET_STAZ;i++)
	if(t_call_redraw[i].callback!=NULL)
		/*t_call_redraw[i].callback(t_call_redraw[i].closure);*/
		t_call_redraw[i].callback;
/* 
 ricarica il time - out
 */
time_id=XtAppAddTimeOut(XtWidgetToApplicationContext(topLevel),
                                   (unsigned long)1000,staz_proc,NULL);
}


static void pag_del_callback(w,info,str)
Widget w;
caddr_t info;
XmSelectionBoxCallbackStruct *str;
{
int is= (int)info;
if(pagvis[is].w!=0)
	{
	XtDestroyWidget(XtParent(XtParent(XtParent(XtParent(pagvis[is].w)))));
	pagvis[is].attiva=0;
	XSync(display,False); 
	pagvis[is].w=0;
	}
}

static void staz_del_callback(w,info,str)
Widget w;
caddr_t info;
XmSelectionBoxCallbackStruct *str;
{

/* sgancio code messaggi */
msg_close_fam();
exit(0);
}

int cnew_staz(ip3)
int ip3;
{
Widget wTopLev;
Widget wmenu,wbutton,wpopup,wshell,wform,wscroll,wclip;
Widget wRow;
char app[LUN_DES_PAG+1];
int x,y,lform,hform;
int ipag;
int i;
int flag,it,is;


if (tipo_staz == 0)
{
/* lato legograf */
	tra_nom_log_s(app,10,"X_STAZ","EASE$LNM");
	sscanf(app,"%d",&x);
	x=x*6;

	tra_nom_log_s(app,10,"Y_STAZ","EASE$LNM");
	sscanf(app,"%d",&y);
	y=y*6;

	tra_nom_log_s(app,15,"VAR_STAZ","EASE$LNM");
	printf("\n PAGINA : %s \n",app);
}
else
{
/* lato schedulatore */

        x=ip3 * 20;
        y=ip3 * 10;
        strcpy(app,richiesta_staz.nome_pagina);
}


/*
 ricerca la pagina nella lista delle pagine ricavandone l'indice
 */
for(i=0;i<header.tot_pagine;i++)
	{
	if(memcmp(pagina[i].nome,app,strlen(app))==0)
		{
		ipag=i;
		break;
		}
	}
if(i==header.tot_pagine) return(1);
/*
 verifica che la pagina non sia gia' presente
*/
for(i=0;i<MAX3_PAG;i++)
	{
	if(pagvis[i].attiva && pagvis[i].pagina==ipag) return(2);
			  /* ritorno per pagina gia' presente */
	}

/*
 riempimento struttura descrittiva della pagina visualizzata e
 creazione della window di rappresentazione della pagina
*/
pagvis[ip3].attiva=1;
pagvis[ip3].pagina=ipag;
pagvis[ip3].x=x;
pagvis[ip3].y=y;
pagvis[ip3].width=pagina[ipag].posmx*(DIM_UNITSTAZ);
pagvis[ip3].height=pagina[ipag].posmy*(DIM_UNITSTAZ);

if (draw_width < pagvis[ip3].width + 10) lform =draw_width;
else                                 lform = pagvis[ip3].width + 10;
if (draw_height < pagvis[ip3].height + 10) hform =draw_height;
else                                 hform = pagvis[ip3].height + 10;
i=0;
XtSetArg(args[i],XmNx,x); i++;
XtSetArg(args[i],XmNy,y); i++;
XtSetArg(args[i],XmNdefaultPosition,False); i++;
XtSetArg(args[i],XmNheight,hform);i++;
XtSetArg(args[i],XmNwidth,lform);i++;
XtSetArg(args[i],XmNallowShellResize,True); i++;
/*XtSetArg(args[i],XmNresizable,False);i++;*/
XtSetArg(args[i],XmNbackground,sfondo_window.pixel);i++;
memcpy(app,pagina[ipag].descrizione,LUN_DES_PAG);
app[LUN_DES_PAG]='\0';
/*XtSetArg(args[i],XmNdialogTitle,XmStringCreateLtoR(pagina[ipag].descrizione,XmSTRING_DEFAULT_CHARSET)); i++;
*/
XtSetArg(args[i],XmNiconName,pagina[ipag].descrizione);
XtSetArg(args[i],XmNtitle,pagina[ipag].descrizione); i++;
wTopLev=XtCreateManagedWidget(pagina[ipag].descrizione,topLevelShellWidgetClass,topLevel,args,i);
/* wform=XmCreateFormDialog(topLevel,"form_staz",args,i); */
i=0;
wform=XmCreateForm(wTopLev,"form_staz",args,i);   
XtManageChild(wform);

i=0;
XtSetArg(args[i],XmNscrollingPolicy,XmAUTOMATIC); i++;
XtSetArg(args[i],XmNscrollBarDisplayPolicy,XmAS_NEEDED); i++;
XtSetArg(args[i],XmNvisualPolicy,XmCONSTANT); i++;
XtSetArg(args[i],XmNbackground,sfondo_label.pixel);i++;
XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM); i++;
XtSetArg(args[i],XmNleftOffset,0); i++;
XtSetArg(args[i],XmNtopAttachment,XmATTACH_FORM); i++;
XtSetArg(args[i],XmNtopOffset,0); i++;
XtSetArg(args[i],XmNrightAttachment,XmATTACH_FORM); i++;
XtSetArg(args[i],XmNrightOffset,0); i++;
XtSetArg(args[i],XmNbottomAttachment,XmATTACH_FORM); i++;
XtSetArg(args[i],XmNbottomOffset,0); i++;
XtSetArg(args[i],XmNspacing,0); i++;
XtSetArg(args[i],XmNborderWidth,0); i++;
XtSetArg(args[i],XmNheight,hform);i++;
XtSetArg(args[i],XmNwidth,lform);i++;
wscroll=XmCreateScrolledWindow(wform,"scroll_staz",args,i);   

XtManageChild(wscroll);

/* la clip window e' creata automaticamente dalla scrolled window 
   con il colore di default */

XtSetArg(args[0],XmNclipWindow,&wclip);
XtGetValues(wscroll,args,1);
XtSetArg(args[0],XmNbackground,sfondo_window.pixel);
XtSetValues(wclip,args,1);

i=0;
XtSetArg(args[i],XmNx,2); i++;
XtSetArg(args[i],XmNy,0); i++;
XtSetArg(args[i],XmNdialogStyle,XmDIALOG_WORK_AREA); i++;  
XtSetArg(args[i],XmNautoUnmanage,True);i++;
XtSetArg(args[i],XmNmarginHeight,0);i++;
XtSetArg(args[i],XmNmarginWidth,0);i++;
XtSetArg(args[i],XmNlabelFontList,fontlist);i++;
XtSetArg(args[i],XmNbackground,sfondo_window.pixel);i++;
pagvis[ip3].w=XmCreateBulletinBoard(wscroll,"stazione",args,i);   
XtManageChild(pagvis[ip3].w);

XtSetArg(args[0],XmNworkWindow,pagvis[ip3].w);
XtSetValues(wscroll,args,1);
/*
 Creazione delle stazioni che compongono la pagina
*/
flag=1;
for(i=0;i<pagina[ipag].num_staz;i++)
	{
	is=offset_staz[pagina[ipag].offset_staz +i ];
	if(is==0) break;
/*	it=pagina[ipag].tipo[i]; */
        it=stazione[is-1].tipo;
	if (it >= MINIMO_INDICE_NEW_STAZ) crea_new_staz(flag,it,is,ip3);
	else 		staz_gen_r(&flag,&it,&is,&ip3);
/*       printf(" creata stazione tipo %d indice %d \n",it,is);*/
	}

i=0;
XtSetArg(args[i],XmNwhichButton,Button3); i++;
wpopup=(Widget) XmCreatePopupMenu(pagvis[ip3].w ,"pulldown",args,i);
i=0;
XtSetArg(args[i],XmNlabelString,XmStringCreateLtoR("Quit ",XmSTRING_DEFAULT_CHARSET)); i++;
wbutton=(Widget) XmCreatePushButton(wpopup,"bottone",args,i);
XtManageChild(wbutton);

XtAddCallback(wbutton,XmNactivateCallback,pag_del_callback,(XtPointer)ip3);
XtAddEventHandler(pagvis[ip3].w,ButtonPressMask,False,PostIt,wpopup);

return(0); /* ritorno corretto */
}


static void PostIt(w,num,event)
Widget w;
int *num;
XButtonEvent *event;
{
Widget wpop=(Widget)num;
if(event->button == Button3)
	{
		XmMenuPosition(wpop,event);
           /*  Visualizza il popup menu */
		XtManageChild(wpop);
	}
}





void init_staz()
{
int i;
for(i=0;i<header.tot_staz;i++)
	{
	stazione[i].incorso= (-1);
	}
for(i=0;i<header.tot_staz * MAX_WIDGET_STAZ;i++)
	{
	t_call[i].closure= NULL;                   /*(-1);*/
	t_call[i].callback=NULL;
	t_call_redraw[i].closure=NULL;
	t_call_redraw[i].callback=NULL;
	}
}

/* 
 add_refresh 
    permette di associare al timeout una nuova funzione di refresh;
    verra' generalmente associata una routine di refresh per ogni
    stazione creata
 */
int add_refresh(proc,closure)
XtCallbackProc proc;   /* in Dec era caddr_t */
caddr_t closure;
{
int i;
for(i=0;i<header.tot_staz*MAX_WIDGET_STAZ;i++)
	{
	if(t_call[i].callback == NULL)
		{

		t_call[i].callback=((XtCallbackProc)proc);
		t_call[i].closure=closure;

		return(0);
		}
	}
return(-1);   /* mancanza di memoria per aggiunta routine di refresh */
}


/*
  elimina la funzione di refresh associata ad una stazione
 */

int del_refresh(closure)
caddr_t closure;
{
int i;
for(i=0;i<header.tot_staz*MAX_WIDGET_STAZ;i++)
	{
	if(t_call[i].closure==closure)
		{
		t_call[i].callback=NULL;
		t_call[i].closure= NULL;
		return(0);
		}
	}
return(-1);   /* routine di callback non trovata */
}


/* 
 add_redraw 
    permette di associare una funzione di redraw ad un oggetto
 */
int add_redraw(proc,closure)
XtCallbackProc proc;   /* in Dec era caddr_t */
caddr_t closure;
{
int i;
for(i=0;i<header.tot_staz*MAX_WIDGET_STAZ;i++)
	{
	if(t_call_redraw[i].callback== NULL)
		{
		t_call_redraw[i].callback=((XtCallbackProc)proc);
		t_call_redraw[i].closure=closure;
		return(0);
		}
	}
return(-1);   /* mancanza di memoria per aggiunta routine di redraw */
}


/*
  elimina la funzione di redraw associata ad un oggetto
 */

int del_redraw(closure)
caddr_t closure;
{
int i;
for(i=0;i<header.tot_staz*MAX_WIDGET_STAZ;i++)
	{
	if(t_call_redraw[i].closure==closure)
		{
		t_call_redraw[i].callback=NULL;
		t_call_redraw[i].closure= NULL;
		return(0);
		}
	}
return(-1);   /* routine di callback non trovata */
}

void init_gc()
{
XGCValues values;
XColor color;
Colormap default_cmap;
Pixel pixel;

unsigned int line_width = 0;
static int line_style[] = {LineSolid,LineSolid,LineSolid,LineSolid,LineSolid };
int cap_style = CapRound;
int join_style = JoinRound;
static int dash_offset[] = {0,0,0,5};
static char dash_list[4][2] ={ {0,0},{0,0},{5,5},{5,5}};
int list_lenght = 2;
int i;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCCapStyle | GCJoinStyle |
                           GCFunction;



#if defined VMS
char *font_name = "fixed";
char *font_name_big = "-Adobe-Times-Bold-R-Normal--*-180-*-*-P-*-ISO8859-1";
#else
char *font_name = "fixed";
char *font_name_big = "-adobe-times-bold-r-normal--25-180-100-100-p-132-iso8859-1";
#endif

load_font(&font_info,(XmFontList *)&fontlist,(char*)font_name);
load_font(&font_info_big,(XmFontList *)&fontlist_big,(char*)font_name_big);
font_ascent= font_info->ascent;
font_height= font_info->ascent + font_info->descent;
font_width= font_info->max_bounds.width;
values.function=GXcopy;
values.line_width = line_width;
values.cap_style = cap_style;
values.join_style = join_style;
values.background = WhitePixel(display,screen_num);
default_cmap=DefaultColormap(display,screen_num);

/* vecchie staz */
for(i=0;i<5;i++)
   {
   XAllocNamedColor(display,
            default_cmap,
            colori[i],&color,&excolor[i]);
   }

for(i=0;i<5;i++)
   {
   values.line_style = line_style[i];
   values.foreground = excolor[i].pixel;
   values.background = WhitePixel(display,screen_num);
                                            
   gc[i]= XCreateGC(display,RootWindow(display,screen_num), valuemask, 
&values);
   XSetFont(display,gc[i],font_info->fid);
   }
}


void load_font(font_info,flist,fontname)
XFontStruct **font_info;
XmFontList *flist;
char fontname[];
{
/* Carica il font ottenendo la descrizione del font stesso */

char **pf;
int i,nf;

if((*font_info = XLoadQueryFont(display,fontname)) == NULL)
        {
        fprintf(stderr,"\nCannot open font");
        }
	else
	{
	*flist = XmFontListCreate (*font_info, charset);
	if (*flist == NULL) fprintf(stderr,"FONTLIST NULL");
	}
}
          
int crea_sfondoP(stip,bits,width,height)
Pixmap *stip;
char *bits;
int width,height;
{
long cnero,cbianco;

if((*stip=XCreatePixmapFromBitmapData(display,
            RootWindow(display,screen_num),bits,
            width,height,BlackPixel(display,screen_num),
	    sfondo_staz.pixel,
/*            WhitePixel(display,screen_num),*/
            DefaultDepth(display,screen_num)))== False)
        {
        fprintf(stderr,"\n Errore creazione pixmap per sfondo");
        exit(1);
        }
}

/*
 * estr_sh
 *    funzione utilizzata per estrarre un valore dall'area
 *    shared di memoria; e' necessaria in quanto in tale area
 *    dichiarata come array di interi i valori sono memorizzati
 *    come floating
 */
float estr_sh(indice,funct)
int indice;  /* se indice == -1 ritorna 0 perche' variabile non definita
*/
int funct;  /* se >0 converte in logical  */
{

float val; /* puntatore di appoggio */

if (indice == -1) return(0.);

if(tipo_staz==1)
        {
	RtDbPGetValue(dbpunti,indice,&val);
        }
else
        {
	RtDbPGetValue(dbpunti,indice,&val);
        }


if(funct)
        {
        val=((int)val)%2;
        if(val<0)
                val=(-val);
        if(funct == 2) /* esamina se e' richiesta la negazione */
                val=(!val);
        }
return(val);
}
        
/*
	g_perturba 
	chiama la routine perturba di scrittura in area shared
*/

int g_perturba(int nul1,int tipo,float valfin,float vnul2,int nul3,int imodel,int ipunti)
// int  nul1,nul3;	   /* valore nullo */
// int tipo;	   /* tipo operazione (step o rampa )*/
// float valfin;      /* valore variabile */
// float vnul2;	   /* valore nullo */
// int imodel,ipunti; /* indice modello e indice di punto */
{
int iret;


   switch (tipo)
   	{

	case STEP:
          
         iret=pert_scalino(id_msg_pert,ipunti,valfin,0.);
	 break;

        case  IMPULSO:

         iret=pert_impulso(id_msg_pert,ipunti,1.,0.,0.);
	 break;

        case  NEGAZIONE:

         iret=pert_nega(id_msg_pert,ipunti,0.);
	 break;

	case UP_DOWN:

        /* se valfin == 0 e' UP se valfin == -1 e' DOWN */

	if (valfin == 0.)
         iret=pert_up(id_msg_pert,ipunti,0.);
	else
         iret=pert_down(id_msg_pert,ipunti,0.);
	}

return(iret);
}

void pr_pagina()
{
int i,j;
for (i=0; i< header.tot_pagine; i++)
{
printf("\n nome pag= %.8s  ",pagina[i].nome);
printf("\n posmx= %d  posmy= %d",pagina[i].posmx,pagina[i].posmy);
printf (" pag. %d  offset %d  num.staz %d\n",i,pagina[i].offset_staz,pagina[i].num_staz);
for (j=0; j<pagina[i].num_staz; j++)
         printf(" indice staz %d \n",offset_staz[pagina[i].offset_staz+j]);
}

}

void pr_stazione()
{
int i,j;
for (i=0; i< header.tot_staz; i++)
{
printf("\n nome staz= %.8s indice %d tipo %d  ",stazione[i].nome,i,stazione[i].tipo);
printf("\n posmx= %d %d  posmy= %d %d%",stazione[i].posix0,stazione[i].posix1,stazione[i].posiy0,stazione[i].posiy1);
}
}




void LoadColor(w)
Widget w;
{
Colormap default_cmap;
XColor color;
int i;
default_cmap=DefaultColormap(XtDisplay(w),DefaultScreen(XtDisplay(w)));

for(i=0;i<NUM_COLORI;i++)
   {
   if (!XAllocColor(XtDisplay(w),default_cmap,&excolor_new[i]))
             {fprintf(stderr,"\n colore %s ",colori_new[i]);perror("load col");}
   }

for(i=0;i<NUM_COLORI;i++)
   {
   if (!XAllocColor(XtDisplay(w), default_cmap,&excolor_blink[i]))
        {fprintf(stderr,"\n colore %s ",colori_blink[i]);perror("load col");}
   }

if (!XAllocColor(XtDisplay(w), default_cmap,&sfondo_label))
        {perror("load col");}

if (!XAllocColor(XtDisplay(w), default_cmap,&sfondo_window))
        {perror("load col");}

if (!XAllocColor(XtDisplay(w), default_cmap,&sfondo_scroll))
        {perror("load col");}
if (!XAllocColor(XtDisplay(w), default_cmap,&sfondo_staz))
        {perror("load col");}
if (!XAllocColor(XtDisplay(w), default_cmap,&sfondo_bianco))
        {perror("load col");}
}

