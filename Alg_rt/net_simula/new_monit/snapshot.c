/**********************************************************************
*
*       C Source:               snapshot.c
*       Subsystem:              1
*       Description:
*       %created_by:    mauric %
*       %date_created:  Thu Sep  3 11:44:57 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: snapshot.c-9 %  (%full_filespec: snapshot.c-9:csrc:1 %)";
#endif

/*
 * snapshot.c
 * 
 * 
 */
#include <stdio.h>
#include <malloc.h>

#include <X11/Xlib.h>
#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/Xm.h>

#include "UxXt.h"

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

#include "sked.h"

#include "parametri.h"
#include "init_ci.h"
#include "messaggi.h"
#include "bistrutt.h"
#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"
#include "mainOptions.h"
#include "option.h"

#include "Lucchetto.bmp"
#include "OldTopology.bmp"
#include "nullSpace.bmp"

//#include "libutilx.h"

extern int init_bt_interface_active;
extern int bt_interface_active;

extern int inizializzazione;
extern int backtrack_caricato;
extern int stato_sim;
extern int tasto_initic_attivabile;

extern int _MAX_SNAP_SHOT;

extern Widget areaMessaggi;
extern Widget masterMenu;
extern int isSuperuser;
extern OPTIONS_FLAGS options;
extern Boolean *IcProt;    /* punta alla struttura protezione IC */
extern Boolean cutCopyCiBuffer;

extern int nuovoSnapCaricato;
extern int nuovoSnapCaricato2;

int read_background_color (Widget);
int SD_saveic (int,int*,char*,char*);
int display_header_snap (SNTAB *);
int carica_lista_bt ();
int backtrack_sim();
int salva_ci(Widget, int);
int not_executable (Widget);
int SD_loadbt (int,int*,int);
int stampa (char *, int );
int SD_cancellaic (int,int*);
int SD_checkci (int,int*);





/**********************************************************/
/* 
 * alloca dinamicamente strutture prima allocate staticamente
 * (modifica allo sked) 8apr97 fm
 */
int AllocaAreeSnapshot()
{
static int primo_giro = 1;

	if (primo_giro)
		{
		IcScrollForm = (Widget *)XtMalloc (sizeof(Widget)*_MAX_SNAP_SHOT);
		IcNumber = (Widget *)XtMalloc (sizeof(Widget)*_MAX_SNAP_SHOT);
		IcProtect = (Widget *)XtMalloc (sizeof(Widget)*_MAX_SNAP_SHOT);
		IcDate = (Widget *)XtMalloc (sizeof(Widget)*_MAX_SNAP_SHOT);
		IcDescription = (Widget *)XtMalloc (sizeof(Widget)*_MAX_SNAP_SHOT);
		IcModified = (Widget *)XtMalloc (sizeof(Widget)*_MAX_SNAP_SHOT);

		nriga = (char *)malloc(sizeof(char)*_MAX_SNAP_SHOT*3);

		primo_giro = 0;
		}
}
/**********************************************************/
int esegui_snap (n,s)
int n;           /* numero della ci da 1 a ... */
char *s;         /* commento alla ci           */
{
/****************************
 * per sommari mffr snappati (tipo Priolo)
 */
SNAP_SKED old_mffr;
/**************************/
int ic_num;
char *messaggio;
int esito;
extern SNTAB *snap_header;

	ic_num = n;
	esito = 0;
	messaggio = (char *)malloc (strlen(SAVE_CI) + strlen(s) + 
							strlen(OPER_FALLITA) + 20);
	if (SD_saveic(BANCO, &ic_num, s,(char*)&old_mffr) >0)
		{
		sprintf (messaggio,"%s %d: %s",SAVE_CI,ic_num,s);
		read_lista_snap (snap_header);
      display_header_snap (snap_header);
		}
	else
		{
		sprintf (messaggio,"%s %d: %s %s",SAVE_CI,ic_num,s,OPER_FALLITA);
		esito = -1;
		attenzione(masterMenu,messaggio);
		}
		
	if (ic_num == _MAX_SNAP_SHOT)
		add_message (areaMessaggi,messaggio,LIVELLO_3);
	else
		add_message (areaMessaggi,messaggio,LIVELLO_1);

	free (messaggio);
	return(esito);
}
/**********************************************************/
int esegui_snap_bt (n,s)
int n;           /* numero del bt da 1 a ...   */
char *s;         
{
/****************************
 * per sommari mffr snappati (tipo Priolo)
 */
SNAP_SKED old_mffr;
/**************************/
int bt_num;
char *messaggio;
int esito;

   bt_num = n;
   esito = 0;
   messaggio = (char *)malloc (strlen(SAVE_BT) + strlen(s) + 
							strlen(OPER_FALLITA) + 20);
   if (SD_savebt(BANCO, &bt_num, s,(char*)&old_mffr) >0)
      {
      sprintf (messaggio,"%s %d: %s",SAVE_BT,bt_num,s);
		if (init_bt_interface_active)
			carica_lista_bt();
/***** su backtrackManagement impasta la grafica
		if (bt_interface_active)
			carica_header_bt();
**********************************/
/* se e' attiva BT aggiorna l' interfaccia */
		if (bt_interface_active)
			{
/*			updateHeaderBt ();   Funziona da completare */
			}
      }
   else
      {
      sprintf (messaggio,"%s %d: %s %s",SAVE_BT,bt_num,s,OPER_FALLITA);
      esito = -1;
      attenzione(masterMenu,messaggio);
      }

   add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);
   return(esito);
}
/**********************************************************/
int default_snap (n,s)
int n;           /* numero della ci da 1 a ... */
char *s;         /* commento alla ci           */
{
/****************************
 * per sommari mffr snappati (tipo Priolo)
 */
SNAP_SKED old_mffr;
/**************************/
int ic_num;
char *messaggio;
int esito;
extern SNTAB *snap_header;

   ic_num = n;
   esito = 0;
   messaggio = (char *)malloc (strlen(SAVE_CI) + strlen(s) + 
										strlen(OPER_FALLITA) + 20);
   if (SD_saveic(BANCO, &ic_num, s,(char*)&old_mffr) >0)
      {
      sprintf (messaggio,"%s %d: %s",SAVE_CI,ic_num,s);
      read_lista_snap (snap_header);
      }
   else
      {
      sprintf (messaggio,"%s %d: %s %s",SAVE_CI,ic_num,s,OPER_FALLITA);
      esito = -1;
		attenzione(masterMenu,messaggio);
      }

   add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);
   return(esito);
}
/**********************************************************/
int salva_ci_def (w,n)
Widget w;
int n;           /* numero della ci da 1 a ... */
{
/****************************
 * per sommari mffr snappati (tipo Priolo)
 */
SNAP_SKED old_mffr;
/**************************/
int ic_num;
char *messaggio;
int esito;
extern SNTAB *snap_list;
char *s;

   ic_num = n;
   esito = 0;

	s = (char *)malloc (strlen(DEFAULT_SNAP) + 10);
	sprintf (s,"%s",DEFAULT_SNAP);
   messaggio = (char *)malloc (strlen(SAVE_CI) + strlen(s) + 
								strlen(OPER_FALLITA) + 20);
   if (SD_saveic(BANCO, &ic_num, s,(char*)&old_mffr) >0)
      {
      sprintf (messaggio,"%s %d: %s",SAVE_CI,ic_num,s);
      read_lista_snap (snap_list);
      }
   else
      {
      sprintf (messaggio,"%s %d: %s %s",SAVE_CI,ic_num,s,OPER_FALLITA);
      esito = -1;
		attenzione(masterMenu,messaggio);
      }

   add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);
	free (s);
   return(esito);
}
/**********************************************************/
int carica_bt (w,n)
Widget w;        /* widget richiedente       */
int n;           /* numero del bt da 1 a ... */
{
/****************************
 * per sommari mffr snappati (tipo Priolo)
 */
SNAP_SKED old_mffr;
/**************************/
int bt_num;
char *messaggio;
int esito;

   bt_num = n;
	esito = 0;

	switch (stato_sim)
		{
		case STATO_BACKTRACK:
			break;
		case STATO_RUN:
			freeze_sim();
		case STATO_FREEZE:
         backtrack_sim();
			salva_ci (w,_MAX_SNAP_SHOT); /* salva database su ci di default  */
			break;
		case STATO_REPLAY:
			backtrack_sim();
			break;
		default:
			not_executable (w);
         return (-1);
		}

	messaggio = (char *)malloc (strlen(LOAD_BT)+strlen(OPER_FALLITA) + 20);
printf ("carica_bt passa %d\n",bt_num);
  if (SD_loadbt (BANCO,&bt_num, (int)&old_mffr) > 0)
		{
		sprintf (messaggio,"%s %d",LOAD_BT,bt_num);
		inizializzazione = 1;
		backtrack_caricato = 1;
		strcpy (init_from, messaggio);
		nuovoSnapCaricato = 1;   /* memoria caricamento avvenuto	*/
		nuovoSnapCaricato2 ++;   /* contatore snap caricati		*/
      }
	else
      {
      sprintf (messaggio,"%s %d %s",LOAD_BT,bt_num,OPER_FALLITA);
      esito = -1;
		attenzione(masterMenu,messaggio);
      }
	add_message (areaMessaggi,messaggio,LIVELLO_1);
   free (messaggio);
   return(esito);
}
/**********************************************************/
int init_from_bt (w,n)
Widget w;        /* widget richiedente       */
int n;           /* numero del bt da 1 a ... */
{
/****************************
 * per sommari mffr snappati (tipo Priolo)
 */
SNAP_SKED old_mffr;
/**************************/
/*
 * solo dalla funzione omonima, non salva database e disponibile
 * solo in FREEZE 
 * revisione 5jan96 : esegue la sequenza canonica per evitare errori 
 * da parte di sked nella gestione del tempo di simulazione
 */
int bt_num;
char *messaggio;
int esito;

   bt_num = n;
   esito = 0;

	switch (stato_sim)
		{
		case STATO_BACKTRACK:
			break;
		case STATO_RUN:
			freeze_sim();
		case STATO_FREEZE:
         backtrack_sim();
			break;
		case STATO_REPLAY:
			backtrack_sim();
			break;
		default:
			not_executable (w);
         return (-1);
		}

   messaggio = (char *)malloc (strlen(LOAD_BT)+strlen(OPER_FALLITA) + 20);
printf ("init_from_bt passa %d\n",bt_num);
  if (SD_loadbt (BANCO,&bt_num, (int)&old_mffr) > 0)
      {
      sprintf (messaggio,"%s %d",LOAD_BT,bt_num);
      inizializzazione = 1;
		strcpy (init_from, messaggio);
      }
   else
      {
      sprintf (messaggio,"%s %d %s",LOAD_BT,bt_num,OPER_FALLITA);
      esito = -1;
		attenzione(masterMenu,messaggio);
      }
   add_message (areaMessaggi,messaggio,LIVELLO_1);
   free (messaggio);

	btload_ok (w);
	freeze_sim (w);

	return (esito);
}

/**********************************************************/
int carica_ci (w,n)
Widget w;
int n;           /* numero della ci da 1 a 60 */
{
/****************************
 * per sommari mffr snappati (tipo Priolo)
 */
SNAP_SKED old_mffr;
/**************************/
int ic_num;
char *messaggio;
int esito;

	ic_num = n;
	esito = 0;
/*
printf("carica_ci DEBUG: stato_sim = %d ic_num = %d\n",stato_sim, ic_num);
*/
   switch (stato_sim)
     	{
     	case STATO_FREEZE:
        	break;
	case STATO_BACKTRACK:
		if (ic_num != _MAX_SNAP_SHOT)
			{
			not_executable (w);
   	      return (-1);
			}
			break;
     	default:
        	not_executable (w);
        	return (-1);
     	}

	messaggio = (char *)malloc (strlen(LOAD_CI)+strlen(OPER_FALLITA) + 20);
	if (SD_loadic (BANCO,&ic_num, (int*)&old_mffr) > 0)
		{
      sprintf (messaggio,"%s %d",LOAD_CI,ic_num);
		inizializzazione = 1;
		strcpy (init_from, messaggio);
      nuovoSnapCaricato = 1;   /* memoria caricamento avvenuto */
      nuovoSnapCaricato2 ++;	 /* contatore snap caricati		*/
		}
   else
      {
      sprintf (messaggio,"%s %d %s",LOAD_CI,ic_num,OPER_FALLITA);
      esito = -1;
		attenzione(masterMenu,messaggio);
      }
   add_message (areaMessaggi,messaggio,LIVELLO_1);
   free (messaggio);
   return(esito);

}
/**********************************************************/
int read_lista_snap (SNTAB *dati)
{

	if (SD_editic(BANCO, dati, 0, _MAX_SNAP_SHOT) > 0)
		return(0);

	printf ("ERRORE LETTURA editic\n");
	return (-1);
}
/**********************************************************/
int display_header_snap (p)
SNTAB *p;
{
int i;
char stringa[100];
SNTAB *dati;
static int first_time = 1;
static int bgcolor;

   dati = p;

	if (first_time)
		{
                bgcolor=read_background_color(IcDate[0]);
		if ((LockedIc = XCreatePixmapFromBitmapData (UxDisplay,
                        RootWindowOfScreen(XtScreen(UxTopLevel)),Lucchetto_bits,
                        Lucchetto_width, Lucchetto_height,
                        BlackPixelOfScreen(XtScreen(UxTopLevel)),
                        bgcolor,  // era cosi   read_background_color(IcDate[0]),
                        DefaultDepthOfScreen(XtScreen(UxTopLevel)))) <= 0)
            printf ("ERRORE : pixmap 1 non allocata\n");

      if ((ModifiedIc = XCreatePixmapFromBitmapData (UxDisplay,
                        RootWindowOfScreen(XtScreen(UxTopLevel)),
								OldTopology_bits,
                        OldTopology_width, OldTopology_height,
                        BlackPixelOfScreen(XtScreen(UxTopLevel)),
                        bgcolor,  // era cosi   read_background_color(IcDate[0]), 
                        DefaultDepthOfScreen(XtScreen(UxTopLevel)))) <= 0)
            printf ("ERRORE : pixmap 2 non allocata\n");

      if ((nullSpace = XCreatePixmapFromBitmapData (UxDisplay,
                        RootWindowOfScreen(XtScreen(UxTopLevel)),
                        nullSpace_bits,
                        nullSpace_width, nullSpace_height,
                        BlackPixelOfScreen(XtScreen(UxTopLevel)),
                        bgcolor,  // era cosi   read_background_color(IcDate[0]), 
                        DefaultDepthOfScreen(XtScreen(UxTopLevel)))) <= 0)
            printf ("ERRORE : pixmap 3 non allocata\n");

		first_time = 0;
		}

	for (i=0; i<_MAX_SNAP_SHOT; i++)
		{
		if (dati->stat == 1)   /* se occupato */
			{
			XtVaSetValues (*(IcDate+i), XmNvalue, dati->temposn, NULL);
			XtVaSetValues (*(IcDescription+i), XmNvalue, dati->descr, NULL);
			if (dati->mod == 1)      /* modificato da net_compi  */
				{
				XtVaSetValues (*(IcModified+i),
							XmNlabelType, XmPIXMAP,
							XmNlabelPixmap,ModifiedIc,NULL);
				}
			else
				{
            XtVaSetValues (*(IcModified+i),
                     XmNlabelType, XmPIXMAP,
                     XmNlabelPixmap,nullSpace,NULL);
            }
			}
		else 
			{
			XtVaSetValues (*(IcDate+i), XmNvalue,"" , NULL);
			XtVaSetValues (*(IcDescription+i),XmNvalue, EMPTY_IC , NULL);
			XtVaSetValues (*(IcModified+i),
                     XmNlabelType, XmPIXMAP,
                     XmNlabelPixmap,nullSpace,NULL);
			}
		if (*(IcProt+i))
				XtVaSetValues (*(IcProtect+i), 
							XmNlabelType, XmPIXMAP,
							XmNlabelPixmap,LockedIc,NULL);
		dati++;
		}
	return(0);
}
/**********************************************************/
int display_lista_snap (lista, p)
Widget lista;
SNTAB *p;
{
int i;
char stringa[80];
char *str;
XmString c_str;
int pos;
SNTAB *dati;

   dati = p;
	for (i=0,pos=1; i<_MAX_SNAP_SHOT; i++,pos++,dati++)
		{
		sprintf(stringa,"%3d  -  %s", dati->prog ,dati->descr);
      if (dati->stat == 1)
      	{
         sprintf(stringa,"%s - %s - %s",
         	stringa,dati->datasn,dati->temposn);
      	}
		str = (char*)XtMalloc(sizeof(char)*((strlen(stringa))+20));
		strcpy (str, stringa);
		c_str = XmStringCreate (str, XmSTRING_DEFAULT_CHARSET);
		XmListAddItemUnselected (lista, c_str, 0);
		XtVaSetValues (lista, XmNitemCount, pos, NULL);
		XmStringFree(c_str);
		}
	return(0);
}
/**********************************************************/
int crea_snapshot_entry(parent)
Widget parent;
{
int i;

for (i=0; i<_MAX_SNAP_SHOT; i++)
	{
   *(IcScrollForm+i) = XtVaCreateManagedWidget( "IcScrollFormn",
         xmFormWidgetClass, parent,
         XmNresizePolicy, XmRESIZE_NONE,
			XmNwidth,  600,
         NULL );

	sprintf (nriga+i*3,"%d",i+1);

   *(IcNumber+i) = XtVaCreateManagedWidget( "IcNumber1",
         xmTextWidgetClass, *(IcScrollForm+i),
         XmNleftAttachment, XmATTACH_FORM,
         XmNtopAttachment, XmATTACH_FORM,
         XmNleftOffset, 0,
         XmNbottomAttachment, XmATTACH_FORM,
         XmNheight, 30,
         XmNwidth, 35,
			XmNmarginHeight, 3,
         XmNy, 0,
         XmNx, 5,
			XmNvalue,	nriga+i*3,
			XmNeditable     ,    False,
			XmNcursorPositionVisible, False,
         NULL );


   *(IcProtect+i) = XtVaCreateManagedWidget( " ",
         xmLabelWidgetClass, *(IcScrollForm+i),
         XmNtopAttachment, XmATTACH_FORM,
         XmNbottomAttachment, XmATTACH_FORM,
         XmNleftWidget, *(IcNumber+i),
         XmNleftAttachment, XmATTACH_WIDGET,
         XmNtopOffset, 0,
         XmNheight, 20,
         XmNwidth, 20,
         XmNy, 5,
         XmNx, 45,
         NULL );


   *(IcDate+i) = XtVaCreateManagedWidget( "IcDate1",
         xmTextWidgetClass, *(IcScrollForm+i),
         XmNtopAttachment, XmATTACH_FORM,
         XmNleftWidget, *(IcProtect+i),
         XmNleftAttachment, XmATTACH_WIDGET,
         XmNbottomAttachment, XmATTACH_FORM,
         XmNheight, 30,
         XmNwidth, 85,
			XmNmarginHeight, 3,
         XmNy, 0,
         XmNx, 82,
			XmNeditable     ,    False,
			XmNcursorPositionVisible, False,
         NULL );


   *(IcDescription+i) = XtVaCreateManagedWidget( "IcDescrizione1",
         xmTextWidgetClass, *(IcScrollForm+i),
         XmNresizeWidth, TRUE,
         XmNmarginWidth, 5,
         XmNmarginHeight, 5,
         XmNhighlightThickness, 2,
         XmNtopAttachment, XmATTACH_FORM,
         XmNleftWidget, *(IcDate+i),
         XmNleftAttachment, XmATTACH_WIDGET,
         XmNbottomAttachment, XmATTACH_FORM,
         XmNheight, 20,
         XmNwidth, 430,
			XmNmarginHeight, 3,
			XmNeditable     ,    False,
			XmNcursorPositionVisible, False,
         NULL );

   *(IcModified+i) = XtVaCreateManagedWidget( " ",
         xmLabelWidgetClass, *(IcScrollForm+i),
         XmNtopAttachment, XmATTACH_FORM,
         XmNbottomAttachment, XmATTACH_FORM,
         XmNleftWidget, *(IcDescription+i),
         XmNleftAttachment, XmATTACH_WIDGET,
         XmNtopOffset, 0,
         XmNheight, 20,
         XmNwidth, 20,
         NULL );

	XtAddCallback (*(IcNumber+i),XmNfocusCallback,IcSelActivate,nriga+i*3);
	XtAddCallback (*(IcNumber+i),XmNlosingFocusCallback,IcSelActivate,nriga+i*3);

	XtAddCallback (*(IcDate+i),XmNfocusCallback,IcSelActivate,nriga+i*3);
	XtAddCallback (*(IcDate+i),XmNlosingFocusCallback,IcSelActivate,nriga+i*3);

	XtAddCallback (*(IcDescription+i),XmNfocusCallback,IcSelActivate,nriga+i*3);
	XtAddCallback (*(IcDescription+i),XmNlosingFocusCallback,IcSelActivate,nriga+i*3);
	}

}
/******************************************************************/
int set_colore_riga_snap (n, c)
int n;   /* numero di riga */
int c;   /* colore         */
{
printf ("set colore riga %d\n",n);
	XtVaSetValues (*(IcNumber+n), XmNbackground, c, NULL);
	XtVaSetValues (*(IcDate+n), XmNbackground, c, NULL);
	XtVaSetValues (*(IcDescription+n), XmNbackground, c, NULL);
	XtVaSetValues (*(IcProtect+n), XmNbackground, c, NULL);  
	return(0);
}
/******************************************************************/
int set_ci_sensitivity (n,w1,w2,w3)
int n;   /* numero di ci selezionata */
Widget w1,w2,w3;
{
extern SNTAB *snap_header;

	if ((snap_header+n)->stat == 1) /* snap occupato  */
		{
/*		XtVaSetValues (w1, XmNsensitive, True, NULL);  */
		tasto_initic_attivabile = True;
   	XtVaSetValues (w2, XmNsensitive, True, NULL);
   	XtVaSetValues (w3, XmNsensitive, True, NULL);
		}
	else
		{
		printf ("record %d non occupato\n",n);
/*		XtVaSetValues (w1, XmNsensitive, False, NULL);  */
		tasto_initic_attivabile = False;
		XtVaSetValues (w2, XmNsensitive, True, NULL);
		XtVaSetValues (w3, XmNsensitive, False, NULL);
		}
	return(0);
}
/******************************************************************/
/*
 * funzione di stampa della lista delle Condizioni iniziali 
 */
int stampa_ci (Widget w)
{
extern SNTAB *snap_header;
extern char *FILES_PATH;
int i;
char *messaggio;
FILE *fp;
char path[MAX_PATH_LEN];
long  giorno, mese, anno;

	sprintf (path,"%s/%s",FILES_PATH,TMP_FILE_SNAP);
   messaggio = (char *)malloc(strlen(path)+ 100+
                              strlen(NOT_ACCESSIBLE));

	if ((fp = fopen (path,"w")) == NULL)
		{
		sprintf (messaggio,"%s \n %s",path,NOT_ACCESSIBLE);
		attenzione (w, messaggio);
      free (messaggio);
		fprintf (stderr,"Impossibile creare file %s <<<<<<<<\n",path);
      return (-1);
		}
	read_lista_snap (snap_header);
	
/* prepara heade */
	giorno  = 1;
   mese    = 1;
   anno    = 2000;
	data((int *)(&giorno), (int *)(&mese), (int *)(&anno));
	fprintf (fp,"-------------------------------------------------------\n");
	fprintf (fp," Snapshot list on date %2d/%2d/%2d \n",giorno, mese, anno);
	fprintf (fp,"-------------------------------------------------------\n");
	for (i=0; i<_MAX_SNAP_SHOT; i++)
		{
		if (snap_header[i].stat == 1)		/* occupato	*/
			{
			fprintf (fp,"%d) - %s - %s\n",
						i+1,snap_header[i].temposn,snap_header[i].descr);
			}
		else
			{
			fprintf (fp,"%d) - %s\n",
						i+1,snap_header[i].descr);
			}
		}
	fclose (fp);

	stampa (path,0);
	free (messaggio);
	messaggio = (char *) malloc (100);		/* messaggio da inserire in .h */
	sprintf (messaggio,"Snap list on file %s\nsent to printer\n", path);
	create_attenzioneDialog(w,messaggio,DIALOG_INFORMATION);
/****
	remove (path);
******/
	free (messaggio);
	return (0);
}
/******************************************************************/
int cancella_ci (w,n)
Widget w;
int n;
{
extern SNTAB *snap_header;
char *mess;
int esito;

	mess = (char *)malloc(strlen(WARN_CUT_CI)+10);
	sprintf (mess,"%s %d",WARN_CUT_CI,n);
	if (!create_attenzioneDialog (w, mess,DIALOG_QUESTION))
		{
		free (mess);
		return(-1);
		}
		
	free (mess);
	mess = (char *)malloc(strlen(CUT_CI)+strlen(OPER_FALLITA) + 10);

	sprintf (mess,"%s %d",CUT_CI,n);
/*	if (SD_removeic(BANCO,&n) > 0)  distruttiva      *******/
	if (SD_cancellaic(BANCO,&n) > 0)  /* permette cut & paste */
   	{
		cutCopyCiBuffer = True;
   	read_lista_snap (snap_header);
   	display_header_snap (snap_header);
		esito = 0;
   	}
	else
		{
		sprintf (mess,"%s %s",mess,OPER_FALLITA);
		attenzione(w,mess);
     	esito = -1;
     	}

	add_message (areaMessaggi,mess,LIVELLO_1);
	free(mess);
	
	return(esito);
}
/******************************************************************/
/*
 * accetta la ci marcata come modificata dall' esecuzione di un net_compi
 */
int accetta_ci (w,n)
Widget w;
int n;
{
extern SNTAB *snap_header;
char *mess;
int esito;

   mess = (char *)malloc(strlen(CHECK_CI)+10);
   sprintf (mess,"%s %d",CHECK_CI,n);
   if (!create_attenzioneDialog (w, mess,DIALOG_QUESTION))
      {
      free (mess);
      return(-1);
      }

   free (mess);
   mess = (char *)malloc(strlen(CHECK_CI)+strlen(OPER_FALLITA) + 10);

   sprintf (mess,"%s %d",CHECK_CI,n);
   if (SD_checkci(BANCO,&n) > 0)  /* permette cut & paste */
      {
      read_lista_snap (snap_header);
      display_header_snap (snap_header);
      esito = 0;
      }
   else
      {
      sprintf (mess,"%s %s",mess,OPER_FALLITA);
      attenzione(w,mess);
      esito = -1;
      }

   add_message (areaMessaggi,mess,LIVELLO_1);
   free(mess);

   return(esito);
}
/******************************************************************/
int copia_ci (wp,w, n)
Widget wp;   /* padre       */
Widget w;    /* tasto paste */
int n;       /* numero ci   */
{
char *mess;

   mess = (char *)malloc(strlen(COPY_CI)+strlen(OPER_FALLITA)+10);
   sprintf (mess,"%s %d",COPY_CI,n);

	if (SD_snapcopy(BANCO,&n) > 0)
		{
		cutCopyCiBuffer = True;
/*
 * il tasto paste viene gestito dalla nuova selezione 
 */
/*   	XtVaSetValues (w, XmNsensitive, True, NULL); */
		}
	else
		{
		sprintf (mess,"%s %s",mess,OPER_FALLITA);
		attenzione(wp,mess);
		add_message (areaMessaggi,mess,LIVELLO_1);
		free(mess);
		return(-1);
		}

	add_message (areaMessaggi,mess,LIVELLO_1);
   free(mess);

	return(0);
}
/******************************************************************/
int paste_ci (w,n)
Widget w;
int n;
{
extern SNTAB *snap_header;
char *mess;
int esito;

	esito = 0;
	mess = (char *)malloc(strlen(WARN_REPLACE_CI)+10);
	if ((snap_header+n-1)->stat == 1) /* se e' occupato     */
		{
   	sprintf (mess,"%s %d",WARN_REPLACE_CI,n);
   	if (!create_attenzioneDialog (w, mess,DIALOG_QUESTION))
      	{
      	free (mess);
      	return(-1);
      	}
		}
	free (mess);
	mess = (char *)malloc(strlen(PASTE_CI)+strlen(OPER_FALLITA)+10);
   sprintf (mess,"%s %d",PASTE_CI,n);

	if (SD_snappaste(BANCO, &n) > 0)
   	{
/*
 * modifica del 5apr96: il buffer viene mantenuto occupato
 */
		/*cutCopyCiBuffer = False;*/ /* buffer liberato      */
		/*XtVaSetValues (w, XmNsensitive, False, NULL);*//* tasto paste reset */
   	read_lista_snap (snap_header);
   	display_header_snap (snap_header);
   	}
	else
   	{
		sprintf (mess,"%s %s",mess,OPER_FALLITA);
		attenzione(w,mess);
      esito = -1;
   	}
	add_message (areaMessaggi,mess,LIVELLO_1);
   free(mess);
	return(esito);
}
/******************************************************************/
int salva_ci (w,n)
Widget w;
int n;   /* ci da 1 a ... */
{
extern SNTAB *snap_header;
char *mess;
char *stringa;

	if (n != _MAX_SNAP_SHOT)  /* se == e' snap di default, no conferma */
		{
   	mess = (char *)malloc(strlen(WARN_REPLACE_CI)+10);
   	if ((snap_header+n-1)->stat == 1) /* se e' occupato     */
      	{
      	sprintf (mess,"%s %d",WARN_REPLACE_CI,n);
      	if (!create_attenzioneDialog (w, mess,DIALOG_QUESTION))
         	{
         	free (mess);
         	return(-1);
         	}
      	}
   	free (mess);
/* colloquio per gestione commento e conferma */
		create_icCommentReqform(n,(snap_header+n-1)->descr); 
		}
	else
		{
		stringa =  (char *)malloc(strlen(DEFAULT_SNAP) + 10);
		sprintf (stringa, "%s", DEFAULT_SNAP);
		default_snap (_MAX_SNAP_SHOT,stringa);
		free (stringa);
		}

	return(0);
}
/******************************************************************/
int ricarica_default_ci (w)
Widget w;
{
char *mess;
      mess = (char *)malloc(strlen(RESTORE_DEF_CI)+10);
      sprintf (mess,"%s",RESTORE_DEF_CI);
      if (!create_attenzioneDialog (w, mess,DIALOG_QUESTION))
         {
         free (mess);
         return(-1);
         }
   if (carica_ci (w,_MAX_SNAP_SHOT))
      return(-1);
		
   switch_to_freeze(w);
   return (0);

}
