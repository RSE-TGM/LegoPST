/**********************************************************************
*
*       C Source:               monit_frem.c
*       Subsystem:              1
*       Description:
*       %created_by:    mauric %
*       %date_created:  Wed Apr  8 15:10:39 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: monit_frem.c-2 %  (%full_filespec: monit_frem.c-2:csrc:1 %)";
#endif
/*
   modulo monit_frem.c
   tipo 
   release 5.2
   data 1/24/96
   reserved @(#)monit_frem.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)monit_frem.c	5.2\t1/24/96";
/*
        Fine sezione per SCCS
*/
#ifdef MFFR
# include <stdio.h>
# include <math.h>
# include <string.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <sys/timeb.h>
#endif
#if defined VMS
# include <stdlib.h>
# include "vmsipc.h"
#endif
# include "sim_param.h"
# include "sim_types.h"       /* tipi di varibili LEGO               */
# include "sim_ipc.h"
# include <X11/Xlib.h>
# include <Mrm/MrmAppl.h>
# include <Xm/ScrolledW.h>
# include <Xm/List.h>
# include <Xm/Label.h>
# include <Xm/Text.h>
# include <Xm/Frame.h>
# include <Xm/ToggleB.h>
# include <Xm/PushB.h>
# include <Xm/RowColumn.h>
# include <Xm/Form.h>
# include <Xm/Xm.h>
# include "monit.h"
# include "monituil.h"
# include "libutilx.h"
# include "sked.h"
# include "dispatcher.h"
# include "agg_manovra.h"
# include "frem_somm.bmp"

#include  "monit_mffr.h"
#include <Rt/RtMemory.h>

extern Widget tasto_fr_ok;

int riga_selezionata[MAX_MF_SOMM];

extern struct timeb tempo;      /* tempo di orologio            */
extern Widget widget_array[MAX_WIDGETS];
extern Widget main_window_widget;
extern Widget toplevel_widget; 

extern VALORI_AGG val_agg;
/* ****************  procedure inserite  ************************ */

void frem_proc(); /* procedura che attiva il colloquio funzioni remote    */

static void refresh_frem();
static void gestione_frem();
extern int aggiorna_valori_sommari();
extern int aggiorna_valori_mf();
extern int aggiorna_valori_fr();
int attiva_timer_fr (Widget );
int crea_sommario_frem();
int crea_top_frem();
int crea_bottoni_comando_frem();
int attiva_timer_gestione_fr (Widget );
int attivazione_fr();
int disattivazione_fr();
int cancellazione_fr();
int fine_sommario_fr();
int cancella_riga_sommario_fr (int );
int impacca_sommario_fr();
int sposta_riga_sommario_fr (int , int );
int copia_riga_sommario_fr (int , int );
int frem_management();
int inserisci_riga_sommario_fr(int );
int disinserisci_riga_sommario_fr(int );
int attiva_riga_sommario_fr(int );
int seleziona_riga_sommario_fr(int );
int perturba_riga_sommario_fr (int, float, float);



/********************************************************************/
/*
 * definizioni funzioni remote
 */
SOMMARIO_FR sommario_fr;

Widget form_dialog_frem;
Widget form_scroll_frem;
Widget frem_shell;
Widget dialog_frem;
Widget scroll_frem;
Widget main_frem;
Widget form_fr;

Widget toggle_fr_w[MAX_FR_SOMM];
Widget label_fr_w[MAX_FR_SOMM];
Widget descrizione_fr_w[MAX_FR_SOMM];
Widget val_default_fr[MAX_FR_SOMM];
Widget rampa_fr_w[MAX_FR_SOMM];
Widget val_fin_fr_w[MAX_FR_SOMM];
Widget val_att_fr_w[MAX_FR_SOMM];
Widget stato_fr_w[MAX_FR_SOMM];
char   nriga_fr[MAX_FR_SOMM][5];
/********************************************************************/
static  int attivato = 0;
static XtIntervalId timer_fr;
static XtIntervalId timer_gestione_fr;
/***************************************************************************/
/*
 * specifiche fonts
 */
extern XmFontList  fontlist_mffr;
/****************************************************************************/

void frem_proc(w, tag, reason)
    Widget w;
    int *tag;
    XmListCallbackStruct *reason;
{
/*  La variabile non era usata da nessuna parte: l'unico effetto della riga
    era dereferenziare 'tag'. Con la chiamata di apertura fatta da monit.c
    (che non passava argomenti) il puntatore conteneva spazzatura e net_monit
    moriva di SIGSEGV appena avviato, prima di mostrare la finestra.
    int     widget_num = *tag;   */
int i;
static Screen *screen;
static Pixmap pixmap;
static int prima_volta = 1;
Window window;
Widget vert_scroll_w;
XmString label_title;

      def_cursore (toplevel_widget, OROLOGIO);
      vis_messaggio("Attivazione colloquio frem",0);
                                                   
      if (prima_volta)
         {
			setta_colori();
         screen = XtScreen (toplevel_widget);
/*
         pixmap = XmGetPixmap (screen, "frem_somm.bmp",
         pix_val[9], pix_val[7]);
*/
	if ((pixmap = XCreatePixmapFromBitmapData (XtDisplay(toplevel_widget),
               RootWindowOfScreen(screen), frem_somm_bits,
               (unsigned int)50, (unsigned int)50,
               colore_tabelle_fr, pix_val[6],
               DefaultDepthOfScreen(XtScreen(toplevel_widget)))) <= 0)
            {
            perror ("errore pixmap -->");
            exit(-1);
            }
         prima_volta = 0;
         }

		if (attivato)
			{
			if (!XtIsManaged (frem_shell))
            {
				XtManageChild (frem_shell);
				attiva_timer_fr (frem_shell);
				}
			undef_cursore (toplevel_widget);
			return;
			}

      frem_shell = XtVaCreateManagedWidget ("fr_shell",
      			topLevelShellWidgetClass,toplevel_widget, 
			XmNallowShellResize       , True,
                        XmNbackground             , colore_sfondo_fr,
			XmNiconName               , "FUNZIONI REMOTE",
			XmNiconic                 , True,        
			XmNmaxWidth               , 880,
/*			XmNminWidth               , 830,    */
			XmNallowShellResize       , True,
			XmNminWidth               , 500,    
			XmNminHeight              , 500,
                        XmNiconPixmap             , pixmap,
			XmNbackground             , colore_tabelle_fr,
			XmNfontList               , fontlist_mffr,
			NULL);
						
      form_dialog_frem = XtVaCreateManagedWidget ("form_dialog_frem",
                xmFormWidgetClass, frem_shell,
                XmNbackground             , colore_sfondo_fr,
	   	XmNfontList               , fontlist_mffr,
                NULL);

      label_title = XmStringCreateLtoR("Sommario Funzioni Remote",
                                       XmSTRING_DEFAULT_CHARSET); 

      dialog_frem = XtVaCreateManagedWidget ("dialog_frem",
                xmBulletinBoardWidgetClass, form_dialog_frem,
	   	XmNfontList               , fontlist_mffr,
                XmNallowOverlap           , True,
                XmNautoUnmanage           , False,
                XmNdialogTitle            , label_title,
                XmNdefaultPosition        , False,
                XmNdialogStyle            , XmDIALOG_MODELESS,
                XmNbackground             , colore_sfondo_fr,
/*		XmNwidth                  , 830,               */
		XmNbottomAttachment       , XmATTACH_FORM,
		XmNleftAttachment       , XmATTACH_FORM,
                NULL);

      XmStringFree(label_title);       

/*		scroll_frem = XtVaCreateManagedWidget ("scroll_frem",       */
      scroll_frem = XtVaCreateWidget ("scroll_frem",
		xmScrolledWindowWidgetClass , form_dialog_frem,
		XmNfontList               , fontlist_mffr,
		XmNx                      , 0,
/*		XmNwidth                  , 830,     */    
		XmNscrollingPolicy        , XmAUTOMATIC,
		XmNvisualPolicy           , XmVARIABLE,
		XmNtopAttachment          , XmATTACH_FORM,
		XmNtopOffset              , 50,
		XmNbottomAttachment       , XmATTACH_FORM,
		XmNbottomOffset           , 100,
		XmNrightAttachment        , XmATTACH_FORM,
		XmNleftAttachment         , XmATTACH_FORM,
		XmNbackground             , colore_tabelle_fr,
		NULL);
						

      XtVaGetValues (scroll_frem, XmNverticalScrollBar, &vert_scroll_w,NULL);
      XtVaSetValues(vert_scroll_w, XmNinitialDelay, 1000, NULL);

		crea_sommario_frem();
		XtManageChild (scroll_frem);

		crea_top_frem(); 
		crea_bottoni_comando_frem();


		undef_cursore (toplevel_widget);
		attiva_timer_fr(frem_shell);
		attiva_timer_gestione_fr(frem_shell);
		attivato = 1;

}
/******************************************************************/
void toggled_fr(w, tag, reason)
    Widget w;
    char *tag;
    XmToggleButtonCallbackStruct *reason;
{
/*   printf ("cliccato toggle button %s : %d *******\n",
            tag,reason->set); */
   if (reason->set)
      {
      riga_selezionata[atoi(tag)] = 1;
		sommario_fr.marcato[atoi(tag)] = MARCATO;
/*      printf (" marcato %d\n",atoi(tag));  */
      }
   else
      {
      riga_selezionata[atoi(tag)] = 0;
		sommario_fr.marcato[atoi(tag)] = NON_MARCATO;
/*      printf (" smarcato %d\n",atoi(tag));  */
      }
}
/******************************************************************/
int cambia_colori_fr (Pixel p, int riga)
{
/*
 * cambia il colore dell' intera riga delle funzioni remote
 * a seguito di selezione o cambio di stato
 */

	XtVaSetValues (label_fr_w[riga], XmNbackground,   p, NULL);
	XtVaSetValues (descrizione_fr_w[riga], XmNbackground,   p, NULL);
	XtVaSetValues (val_default_fr[riga], XmNbackground,   p, NULL);
	XtVaSetValues (rampa_fr_w[riga], XmNbackground,   p, NULL);
	XtVaSetValues (val_fin_fr_w[riga], XmNbackground,   p, NULL);
	XtVaSetValues (val_att_fr_w[riga], XmNbackground,   p, NULL);
	XtVaSetValues (stato_fr_w[riga], XmNbackground,   p, NULL);

	return(0);
}
/******************************************************************/
int crea_top_frem()
{
Widget form_label_top;
int i;
Position ascisse[7];
char *top_label[]= {/* "Label",*/ "Descrizione", "Default", "Rampa",
         "Target", "Attuale", /*"Stato",*/ "NULL"};

XmString label_top;

   i=0;
/*		XtVaGetValues (label_fr_w[0], XmNx, &ascisse[i], NULL); i++;    **/
		XtVaGetValues (descrizione_fr_w[0], XmNx, &ascisse[i], NULL); i++;
		XtVaGetValues (val_default_fr[0], XmNx, &ascisse[i], NULL); i++;
		XtVaGetValues (rampa_fr_w[0], XmNx, &ascisse[i], NULL); i++;
		XtVaGetValues (val_fin_fr_w[0], XmNx, &ascisse[i], NULL); i++;
		XtVaGetValues (val_att_fr_w[0], XmNx, &ascisse[i], NULL); i++;
/*		XtVaGetValues (stato_fr_w[0], XmNx, &ascisse[i], NULL); i++;    **/

   form_label_top = XtVaCreateManagedWidget (NULL,
                xmFormWidgetClass,   form_dialog_frem,
		XmNtopAttachment          , XmATTACH_FORM,
		XmNleftAttachment          , XmATTACH_FORM,
		XmNrightAttachment          , XmATTACH_FORM,
                XmNbackground     , colore_sfondo_fr,
		XmNfontList               , fontlist_mffr,
                NULL);
   i=0;
   while (strcmp(top_label[i],"NULL"))
      {
      label_top = XmStringCreateLtoR (top_label[i],
				XmSTRING_DEFAULT_CHARSET);
      XtVaCreateManagedWidget ("fr_header",
                xmLabelWidgetClass, form_label_top,
                XmNlabelString    , label_top,
		XmNy              , y_init_top_label,
                XmNx              , ascisse[i],
                XmNbackground     , colore_sfondo_fr,
		XmNfontList               , fontlist_mffr,
                NULL);
      i++;
      XmStringFree(label_top);
   }
   return(0);
}
/******************************************************************/
int crea_sommario_frem()
{
int i;
char temp[100];

      main_frem = XtVaCreateWidget ("main_frem",
                  xmRowColumnWidgetClass, scroll_frem,
                  XmNbackground      , colore_tabelle_fr,
                  XmNforeground      , colore_tabelle_fr,
						XmNrightAttachment , XmATTACH_WIDGET,
						XmNrightWidget     ,	form_dialog_frem,
                  NULL);

      for (i=0; i<MAX_FR_SOMM; i++)
         {
      form_fr = XtVaCreateWidget (NULL,
                  xmFormWidgetClass, main_frem,  
                  XmNbackground      , colore_tabelle_fr,
                  XmNforeground      , colore_tabelle_fr,
						XmNrightAttachment , XmATTACH_WIDGET,
                  XmNrightWidget     , form_dialog_frem,
                  NULL);
      toggle_fr_w[i] = XtVaCreateManagedWidget (NULL,
                  xmToggleButtonWidgetClass, form_fr,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_FORM,
                  XmNbackground      , colore_tabelle_fr,
                  XmNselectColor     , pix_val[0],
                  NULL);

      label_fr_w[i] = XtVaCreateWidget ("label",
                  xmTextWidgetClass,   form_fr,
                  XmNcolumns      ,    8,
                  XmNeditable     ,    False,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNleftAttachment,    XmATTACH_WIDGET,
                  XmNleftWidget      , toggle_fr_w[i],
                  XmNcursorPositionVisible, False,
                  XmNbackground        , colore_tabelle_fr,
						XmNfontList               , fontlist_mffr,
                  NULL);
/*		XtManageChild (label_fr_w[i]);       ******/

      descrizione_fr_w[i] = XtVaCreateManagedWidget ("descrizione",
                  xmTextWidgetClass,   form_fr,
                  XmNcolumns      ,    50,
                  XmNeditable     ,    False,
                  XmNleftAttachment  , XmATTACH_WIDGET,
 /*                 XmNleftWidget      , label_fr_w[i],      */
						XmNleftWidget      , toggle_fr_w[i],
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNcursorPositionVisible, False,
                  XmNbackground        , colore_tabelle_fr,
						XmNfontList               , fontlist_mffr,
                  NULL);

      val_default_fr[i] = XtVaCreateManagedWidget ("default",
                  xmTextWidgetClass,   form_fr,
                  XmNcolumns      ,    10,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_WIDGET,
                  XmNleftWidget      , descrizione_fr_w[i],
                  XmNcursorPositionVisible, False,
                  XmNeditable     ,    False,
                  XmNbackground        , colore_tabelle_fr,
						XmNfontList               , fontlist_mffr,
                  NULL);

      rampa_fr_w[i] = XtVaCreateManagedWidget ("rampa",
                  xmTextWidgetClass,   form_fr,
                  XmNcolumns      ,    8,
						XmNmaxLength       , 8,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_WIDGET,
                  XmNleftWidget      , val_default_fr[i],
                  XmNcursorPositionVisible, True,
                  XmNbackground        , colore_tabelle_fr,
						XmNfontList               , fontlist_mffr,
                  NULL);

      val_fin_fr_w[i] = XtVaCreateManagedWidget ("val_fin",
                  xmTextWidgetClass,   form_fr,
                  XmNcolumns      ,    10,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_WIDGET,
                  XmNleftWidget      , rampa_fr_w[i],
                  XmNcursorPositionVisible, True,
                  XmNbackground        , colore_tabelle_fr,
						XmNfontList               , fontlist_mffr,
                  NULL);

      val_att_fr_w[i] = XtVaCreateManagedWidget ("val_att",
                  xmTextWidgetClass,   form_fr,
                  XmNcolumns      ,    10,
                  XmNeditable     ,    False,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_WIDGET,
                  XmNleftWidget      , val_fin_fr_w[i],
						XmNrightAttachment , XmATTACH_FORM,
                  XmNcursorPositionVisible, False,
                  XmNbackground        , colore_tabelle_fr,
						XmNfontList               , fontlist_mffr,
                  NULL);

      stato_fr_w[i] = XtVaCreateWidget ("stato",
                  xmTextWidgetClass,   form_fr,
                  XmNcolumns      ,    10,
                  XmNeditable     ,    False,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_WIDGET,
                  XmNleftWidget      , val_att_fr_w[i],
                  XmNcursorPositionVisible, False,
                  XmNbackground        , colore_tabelle_fr,
						XmNfontList               , fontlist_mffr,
                  NULL);
/*		XtManageChild (stato_fr_w[i]);        *******/

		aggiorna_riga_sommario_fr(i);
/*
 * callbacks relative ai campi
 */
 sprintf (nriga_fr[i],"%d",i);
 XtAddCallback (toggle_fr_w[i],XmNvalueChangedCallback,toggled_fr,nriga_fr[i]);

/* XtAddCallback (rampa_fr_w[i],XmNvalueChangedCallback,rampa_fr_new,nriga_fr[i]);*/
 XtAddCallback (rampa_fr_w[i],XmNactivateCallback,rampa_fr_act,nriga_fr[i]);
 XtAddCallback (rampa_fr_w[i],XmNlosingFocusCallback,
                                 rampa_fr_act,nriga_fr[i]);

/* XtAddCallback (val_fin_fr_w[i],XmNvalueChangedCallback,target_fr_new,nriga_fr[i]);*/
 XtAddCallback (val_fin_fr_w[i],XmNactivateCallback,target_fr_act,nriga_fr[i]);
XtAddCallback (val_fin_fr_w[i],XmNlosingFocusCallback,
                                 target_fr_act,nriga_fr[i]);

/**********
 XtAddCallback (rampa_fr_w[i],XmNfocusCallback,label_focus,nriga_fr[i]);
 XtAddCallback (rampa_fr_w[i],XmNlosingFocusCallback,
                                 label_losing_focus,nriga_fr[i]);
 XtAddCallback (val_fin_fr_w[i],XmNfocusCallback,label_focus,nriga_fr[i]);
 XtAddCallback (val_fin_fr_w[i],XmNlosingFocusCallback,
                                 label_losing_focus,nriga_fr[i]);
************/
riga_selezionata[i] = 0;   /* non selezionata per default        */

      XtManageChild (form_fr);
         }
      XtManageChild (main_frem);

		return(0);
}
/******************************************************************/
int crea_bottoni_comando_frem()
{
Widget form_tasti;
int i;
char str[10];
#define NUM_CMD_FR       5  /* numero MAX tasti comandi in calce al sommario */
Widget  frame_cmd[NUM_CMD_FR];
Widget  tasto[NUM_CMD_FR];
XmString label_tasto;
struct cmd_fr_st
	{
	char *str;
	}cmd_fr[]={tasto_att, tasto_dis, tasto_del, tasto_add, tasto_exit, "NULL"};

      form_tasti = XtVaCreateManagedWidget (NULL,
                xmFormWidgetClass, dialog_frem,
                XmNbackground      , colore_sfondo_fr,
                XmNforeground      , colore_sfondo_fr,
		XmNleftAttachment      ,  XmATTACH_FORM,
		XmNrightAttachment     ,  XmATTACH_FORM,
		XmNbottomAttachment    ,  XmATTACH_FORM,
		XmNx                   ,  0,
                XmNy                   ,  y_init_tasti,  
		XmNfontList               , fontlist_mffr,
                NULL);

      XtVaCreateManagedWidget (NULL,
                xmFrameWidgetClass     ,  form_tasti,
                XmNheight              ,  70,      
		XmNwidth               ,  845,
		XmNbottomAttachment    ,  XmATTACH_FORM,
		XmNleftAttachment      ,  XmATTACH_FORM,
                XmNbackground      , colore_sfondo_fr,
		XmNfontList               , fontlist_mffr,
                NULL);
	
      i=0;
      while (strcmp(cmd_fr[i].str,"NULL"))
         {
         label_tasto = XmStringCreateLtoR (cmd_fr[i].str,
                                           XmSTRING_DEFAULT_CHARSET);
         frame_cmd[i] = XtVaCreateWidget (NULL,
			xmFrameWidgetClass     ,  form_tasti,
			XmNwidth               ,  120,
			XmNheight              ,  30,
			XmNbottomAttachment    ,  XmATTACH_FORM,
			XmNbottomOffset        ,  10,
                        XmNbackground          , colore_tasti_fr,
			XmNfontList            , fontlist_mffr,
	        	NULL);
         tasto[i] = XtVaCreateWidget (NULL,
			xmPushButtonWidgetClass, frame_cmd[i],
			XmNlabelString         , label_tasto, 
			XmNwidth               , 120,
			XmNheight              , 30,     
			XmNdefaultButtonShadowThickness ,  1,
			XmNbackground          , colore_tasti_fr,
			XmNfontList            , fontlist_mffr,
			NULL);
         sprintf (str,"%d",i);
         XtAddCallback (tasto[i], XmNactivateCallback, tasto_fr, cmd_fr[i].str);
         i++;

         XmStringFree(label_tasto);
         }
	XtVaSetValues (frame_cmd[0], XmNleftAttachment, XmATTACH_FORM, 
										  XmNleftOffset    , 10, NULL);
	XtVaSetValues (frame_cmd[1], XmNleftAttachment, XmATTACH_WIDGET,
										  XmNleftWidget    , frame_cmd[0], 
										  XmNleftOffset    , 10, NULL);
	XtVaSetValues (frame_cmd[2], XmNleftAttachment, XmATTACH_WIDGET,
                                XmNleftWidget    , frame_cmd[1], 
										  XmNleftOffset    , 10, NULL);
	XtVaSetValues (frame_cmd[3], XmNleftAttachment, XmATTACH_WIDGET,
                                XmNleftWidget    , frame_cmd[2], 
										  XmNleftOffset    , 10, NULL);
	XtVaSetValues (frame_cmd[4], XmNrightAttachment, XmATTACH_FORM, 
										  XmNrightOffset   , 10, NULL);

	for (i=0; i<NUM_CMD_FR; i++)
		{
		XtManageChild (frame_cmd[i]);
		XtManageChild (tasto[i]);
		}
/*********
	XtVaSetValues (tasto[0], XmNshowAsDefault , 1, NULL);
	XtVaSetValues (dialog_frem, XmNdefaultButton, tasto[0], NULL);
*******************************************************/
	return(0);
}
/***************************************************************/
void tasto_fr(w, data, cbs)
Widget w;
XtPointer data;
XmPushButtonCallbackStruct *cbs;
{
int i,n;

/*	printf ("premuto bottone %s\n",data); */
	
	if (!strcmp(data,tasto_att))
		{
		attivazione_fr();
		}
	else if (!strcmp(data,tasto_dis))
		{
		disattivazione_fr();
      }
	else if (!strcmp(data,tasto_del))
		{
		cancellazione_fr();
      }
	else if (!strcmp(data,tasto_add))
      {
		select_frem_proc();
      }
	else if (!strcmp(data,tasto_exit))
      {
		fine_sommario_fr();
      }
	else
		printf ("richiesta non riconosciuta\n");
}
/******************************************************************/
int cancellazione_fr()
{
int i,n;
      n = sommario_fr.numero;
      for (i=0; i<n; i++)
         {
         if ((sommario_fr.marcato[i] == MARCATO) &&
             (sommario_fr.occupato[i] == OCCUPATO) &&
             (!strcmp(sommario_fr.stato[i], SELEZIONATO)))
            {
            cancella_riga_sommario_fr(i);
            aggiorna_riga_sommario_fr (i);
            XmToggleButtonSetState (toggle_fr_w[i],False,True);
            if (sommario_fr.numero == MAX_FR_SOMM)
               tasto_sbloccato(tasto_fr_ok);
            sommario_fr.numero--;
            }
         }
      impacca_sommario_fr();
		return(0);
}
/******************************************************************/
int fine_sommario_fr()
{
/*		XtDestroyWidget (frem_shell);     ********/
		XtRemoveTimeOut (timer_fr);
		XtUnmanageChild (frem_shell);
}
/******************************************************************/
int clear_sommario_fr()
{
int i;

   for (i=0; i<MAX_FR_SOMM; i++)
      {
      strcpy (sommario_fr.label[i], "");
      strcpy (sommario_fr.descrizione[i], "");
      sommario_fr.address[i] = -1;
      sommario_fr.val_def[i] = 0.0;
      sommario_fr.rampa[i] = RAMPA_DEFAULT;
      sommario_fr.target[i] = TARGET_DEFAULT;
      sommario_fr.attuale[i] = 0.0;
      strcpy ((char*)sommario_fr.stato,  "");
      sommario_fr.marcato[i] = NON_MARCATO;
		sommario_fr.occupato[i] = NON_OCCUPATO;
		sommario_fr.da_riattivare[i] = 0;
      }
   sommario_fr.numero = 0;

   return(0);
}
/******************************************************************/
int cancella_riga_sommario_fr (int riga)
{

/*  printf ("cancella riga %d\n",riga);  */

   strcpy (sommario_fr.label[riga], "");
   strcpy (sommario_fr.descrizione[riga], "");
   sommario_fr.address[riga] = -1;
   sommario_fr.val_def[riga] = 0.0;
   sommario_fr.rampa[riga] = 0.0;
   sommario_fr.target[riga] = 0.0;
   sommario_fr.attuale[riga] = 0.0;
   strcpy (sommario_fr.stato[riga],  "");
   sommario_fr.marcato[riga] = NON_MARCATO;
   sommario_fr.occupato[riga] = NON_OCCUPATO;
	sommario_fr.da_riattivare[riga] = 0;

/*   cambia_colori_fr (colore_tabelle_fr, riga);  */

   return (0);
}
/******************************************************************/
int impacca_sommario_fr()
{
int i;
int primo_vuoto, primo_pieno, ultimo_vuoto;

for (;;)
   {
   for (i=0; i<MAX_FR_SOMM; i++)
      if (sommario_fr.occupato[i] == NON_OCCUPATO)
         break;
   if (i == MAX_FR_SOMM)
      return(0);
   primo_vuoto = i;

   for (i=i+1; i<MAX_FR_SOMM; i++)
      if (sommario_fr.occupato[i] == OCCUPATO)
         break;
   if (i == MAX_FR_SOMM)
      return(0);
   ultimo_vuoto = i-1;
   primo_pieno  = i;

   while (sommario_fr.occupato[i] == OCCUPATO)
      {
      sposta_riga_sommario_fr (primo_vuoto,primo_pieno);
      primo_vuoto++;
      primo_pieno++;
      i++;
      }
   }
}
/******************************************************************/
int sposta_riga_sommario_fr (int d, int s)
{
   copia_riga_sommario_fr (d,s);
   cancella_riga_sommario_fr (s);
   aggiorna_riga_sommario_fr (d);
   aggiorna_riga_sommario_fr (s);

   return(0);
}
/******************************************************************/
int copia_riga_sommario_fr (int d, int s)
{
Boolean stato_toggle;
Pixel colore;

   strcpy (sommario_fr.label[d], sommario_fr.label[s]);
   strcpy (sommario_fr.descrizione[d], sommario_fr.descrizione[s]);
   sommario_fr.address[d] = sommario_fr.address[s];
   sommario_fr.val_def[d] = sommario_fr.val_def[s];
   sommario_fr.rampa[d] = sommario_fr.rampa[s];
   sommario_fr.target[d] = sommario_fr.target[s];
   sommario_fr.attuale[d] = sommario_fr.attuale[s];
   strcpy (sommario_fr.stato[d], sommario_fr.stato[s]);
   sommario_fr.marcato[d] =  sommario_fr.marcato[s];
   sommario_fr.occupato[d] = sommario_fr.occupato[s];
	sommario_fr.da_riattivare[d] = sommario_fr.da_riattivare[s];

   XtVaGetValues (toggle_fr_w[s], XmNset,  &stato_toggle, NULL);
   XtVaSetValues (toggle_fr_w[d], XmNset,  stato_toggle, NULL);

   XtVaGetValues (descrizione_fr_w[s], XmNbackground, &colore, NULL);
   cambia_colori_fr (colore, d);
   return(0);
}
/***************************************************************/
int stampa_somm_fr()
{
   int i;
   for (i=0; i<40; i++)
      printf ("%d) :%s:\n",i,sommario_fr.descrizione[i]);
   return(0);
}
/***************************************************************/
int attiva_timer_fr (Widget w)
{
   timer_fr = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)5000, refresh_frem, NULL);
   return(0);
}
/***************************************************************/
int attiva_timer_gestione_fr (Widget w)
{
   timer_gestione_fr = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)2000, gestione_frem, NULL);
   return(0);
}
/***************************************************************/
static  void refresh_frem (client_data, id)
caddr_t client_data;
XtIntervalId * id;
{
   aggiorna_valori_fr();
   attiva_timer_fr (frem_shell);
}
/***************************************************************/
static  void gestione_frem (client_data, id)
caddr_t client_data;
XtIntervalId * id;
{
	if (val_agg.stato_sked == STATO_RUN)
   	frem_management();

   attiva_timer_gestione_fr (frem_shell);
}
/***************************************************************/
int attivazione_fr()
{
int n,i;

      n = sommario_fr.numero;
      for (i=0; i<n; i++)
         {
         if ((sommario_fr.marcato[i] == MARCATO) &&
             (sommario_fr.occupato[i] == OCCUPATO) &&
             (!strcmp(sommario_fr.stato[i], SELEZIONATO)))
            {
            inserisci_riga_sommario_fr(i);
            XmToggleButtonSetState (toggle_fr_w[i],False,True);
            }
         }
      return(0);
}
/***************************************************************/
int inserisci_riga_sommario_fr(int n)
{
/*      printf ("INSERITA FR riga %d\n",n);  */
      strcpy (sommario_fr.stato[n], INSERITO);
      cambia_colori_fr (colore_inserito, n);
      return(0);
}
/***************************************************************/
int disattivazione_fr()
{
int n,i;

      n = sommario_fr.numero;
      for (i=0; i<n; i++)
         {
         if ((sommario_fr.marcato[i] == MARCATO) &&
             (sommario_fr.occupato[i] == OCCUPATO))
            {
             if (!strcmp(sommario_fr.stato[i], INSERITO))
               {
               disinserisci_riga_sommario_fr(i);
               XmToggleButtonSetState (toggle_fr_w[i],False,True);
               }
             else if (!strcmp(sommario_fr.stato[i], ATTIVATO))
               {
               if (sommario_fr.attuale[i] == sommario_fr.target[i])
                  {
                  printf ("*** Disattivazione fr riga %d\n",n);
                  strcpy (sommario_fr.stato[i], DISATTIVATO);
                  perturba_riga_sommario_fr(i, sommario_fr.val_def[i],0.0);
                  }
               else
                  {
                  stop_pert_riga_sommario_fr(i);
                  strcpy (sommario_fr.stato[i], DISATTIVATO);
                  sommario_fr.da_riattivare[i] = DELAY_ALLA_RIATTIVAZIONE;
                  }
               XmToggleButtonSetState (toggle_fr_w[i],False,True);
               }
            }
         }
      return(0);
}
/***************************************************************/
int disinserisci_riga_sommario_fr(int n)
{
/*      printf ("DISINSERITA FR riga %d\n",n);  */
      strcpy (sommario_fr.stato[n], SELEZIONATO);
      cambia_colori_fr (colore_selezionato_fr, n);
}
/***************************************************************/
int frem_management()
{
int i,n;
static float tempo_old = 0.0;
float tempo;
float deltat;
static int primo_giro = 1;

   tempo = val_agg.tempo_sim;
   if (primo_giro)
      {
      primo_giro = 0;
      tempo_old = tempo;
      return(0);
      }
   deltat = tempo - tempo_old;

	n = sommario_fr.numero;
		for (i=0; i<n; i++)
         {
         if (sommario_fr.occupato[i] == OCCUPATO)
            {
             if (!strcmp(sommario_fr.stato[i], INSERITO))
                  attiva_riga_sommario_fr(i);
				 else if (!strcmp(sommario_fr.stato[i], ATTIVATO))
               {
               if (sommario_fr.da_riattivare[i] > 0)
                  {
                  sommario_fr.da_riattivare[i] -= (int)deltat;
                  if (sommario_fr.da_riattivare[i] <= 0)
                     {
                     sommario_fr.da_riattivare[i] = 0;
                     perturba_riga_sommario_fr(i, sommario_fr.target[i],0.0);
                     }
                  }
					}
				 else if (!strcmp(sommario_fr.stato[i], DISATTIVATO))
					{
					if (sommario_fr.da_riattivare[i] > 0)
                  {
                  sommario_fr.da_riattivare[i] -= (int)deltat;
                  if (sommario_fr.da_riattivare[i] <= 0)
                     {
                     sommario_fr.da_riattivare[i] = 0;
                     perturba_riga_sommario_fr(i, sommario_fr.val_def[i],0.0);
                     }
                  }
					if (sommario_fr.attuale[i] == sommario_fr.val_def[i])
                  seleziona_riga_sommario_fr(i);
					}
            }
         }
   tempo_old = tempo;
   return(0);
}
/***************************************************************/
int attiva_riga_sommario_fr(int n)
{
/*	printf ("attivazione fr %d\n",n); */
	perturba_riga_sommario_fr(n, sommario_fr.target[n],0.0);
	strcpy (sommario_fr.stato[n], ATTIVATO);
	cambia_colori_fr (colore_attivato, n);
	return(0);
}
/***************************************************************/
/* Stile prototipo, NON K&R: il prototipo (riga 104) dichiara (int,float,float),
   il K&R promuoveva i float a double -> valore/t arrivavano corrotti.
   Vedi docs/KR_PROTOTYPE_AUDIT.md. */
int perturba_riga_sommario_fr (int n, float valore, float t)
{
extern int id_msg_pert;
int tipo_pert;
float rateo;

   if (sommario_fr.rampa[n] == 0)
      {
      tipo_pert = PERT_SCALINO;
      rateo = 0.0;
      }
   else
      {
      tipo_pert = PERT_RAMPA;
      rateo = sommario_fr.rampa[n]/60.0;
      }

   pert_snd(id_msg_pert,
               tipo_pert,
               sommario_fr.address[n],
               valore,
               rateo,
					0.0, 0.0, 0.0, 0.0, 0.0,
               0.0);
   return(0);
}
/***************************************************************/
int seleziona_riga_sommario_fr(int n)
{
   printf ("*** Riga fr %d torna selezionata ***\n",n);
	strcpy (sommario_fr.stato[n], SELEZIONATO);
   cambia_colori_fr (colore_selezionato_fr, n);
   return(0);
}
#endif
