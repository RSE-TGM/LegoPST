/**********************************************************************
*
*       C Source:               monit_malf.c
*       Subsystem:              1
*       Description:
*       %created_by:    mauric %
*       %date_created:  Wed Apr  8 14:31:48 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: monit_malf.c-2 %  (%full_filespec: monit_malf.c-2:csrc:1 %)";
#endif
/*
   modulo monit_malf.c
   tipo 
   release 5.2
   data 1/24/96
   reserved @(#)monit_malf.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)monit_malf.c	5.2\t1/24/96";
/*
        Fine sezione per SCCS
*/
#ifdef MFFR
# include <stdio.h>
# include <math.h>
# include <string.h>
#if defined UNIX
# include <sys/types.h>
# include <ctype.h>
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
# include <Xm/ScrollBar.h>
# include "monit.h"
# include "monituil.h"
# include "libutilx.h"
# include "sked.h"
# include "dispatcher.h"
# include "agg_manovra.h"
# include "malf_somm.bmp"

#include  "monit_mffr.h"
#include <Rt/RtMemory.h>

extern struct timeb tempo;      /* tempo di orologio            */
extern Widget widget_array[MAX_WIDGETS];
extern Widget main_window_widget;
extern Widget toplevel_widget; 

extern Widget tasto_ok;
extern VALORI_AGG val_agg;
int riga_selezionata[MAX_MF_SOMM];

/* ****************  procedure inserite  ************************ */

void malf_proc(); /* procedura che attiva il colloquio malfunzioni    */
static void refresh_malf();
static void gestione_malf();
extern int aggiorna_valori_sommari();
extern int aggiorna_valori_mf();
extern int aggiorna_valori_fr();

int attiva_timer (Widget );
int crea_sommario_malf();
int crea_top_malf();
int crea_bottoni_comando();
int attiva_timer_gestione_mf (Widget w);
int stop_pert_riga_sommario_mf ();
int attivazione_mf();
int disattivazione_mf();
int cancellazione_mf();
int fine_sommario_mf();
int cancella_riga_sommario (int riga);
int impacca_sommario();
int sposta_riga_sommario (int , int );
int copia_riga_sommario (int , int );
int malf_management();
int inserisci_riga_sommario(int );
int disinserisci_riga_sommario(int );
int perturba_riga_sommario_mf (int, float, float);
int attiva_riga_sommario (int );
int aggiorna_delay (int );
int aggiorna_durata (int riga);
int seleziona_riga_sommario(int );

/********************************************************************/
/*
 * definizioni malfunzioni
 */
SOMMARIO_MF sommario_mf;
extern SOMMARIO_FR sommario_fr;

Widget form_label_top;
Widget form_dialog_malf;
Widget form_scroll_malf;
Widget malf_shell;
Widget dialog_malf;
Widget scroll_malf;
Widget main_malf;
Widget form_mf;
Widget toggle_w[MAX_MF_SOMM];
Widget label_w[MAX_MF_SOMM];
Widget descrizione_w[MAX_MF_SOMM];
Widget val_default_w[MAX_MF_SOMM];
Widget delay_w[MAX_MF_SOMM];
Widget durata_w[MAX_MF_SOMM];
Widget rampa_w[MAX_MF_SOMM];
Widget val_fin_w[MAX_MF_SOMM];
Widget val_att_w[MAX_MF_SOMM];
Widget stato_mf_w[MAX_MF_SOMM];
char   nriga[MAX_MF_SOMM][5];
/********************************************************************/
static  int attivato = 0;
static XtIntervalId timer_mf;
static XtIntervalId timer_gestione_mf;
/***************************************************************************/
/***************************************************************************/
/*
 * specifiche fonts
 */
extern XmFontList  fontlist_mffr;
/****************************************************************************/

void malf_proc(w, tag, reason)
    Widget w;
    int *tag;
    XmListCallbackStruct *reason;
{
int     widget_num = *tag;
int i;
static Screen *screen;
static Pixmap pixmap;
static int prima_volta = 1;
Window window;
Widget vert_scroll_w;

XmString label_dial;


      def_cursore (toplevel_widget, OROLOGIO);
      vis_messaggio("Attivazione colloquio malf",0);
                                                   
		if (prima_volta)
			{
			setta_colori();
			screen = XtScreen (toplevel_widget);
/*
			pixmap = XmGetPixmap (screen, "malf_somm.bmp",
										pix_val[0], pix_val[1]);
*/
			if ((pixmap = XCreatePixmapFromBitmapData (XtDisplay(toplevel_widget),
					RootWindowOfScreen(screen), malf_somm_bits,
					(unsigned int)50, (unsigned int)50, 
					colore_tabelle, pix_val[10],
					DefaultDepthOfScreen(XtScreen(toplevel_widget)))) <= 0) 
				{
				perror ("errore pixmap -->");
				exit(-1);
				}
			prima_volta = 0;
			}

		if (attivato)
			{
			if (!XtIsManaged (malf_shell))
				{
				XtManageChild (malf_shell);
				attiva_timer(malf_shell);
				}
			undef_cursore (toplevel_widget);
			return;
			}


		window = XtWindow (main_window_widget);

      malf_shell = XtVaCreateManagedWidget (NULL,
      			topLevelShellWidgetClass,toplevel_widget, 
			XmNallowShellResize       , True,
                        XmNbackground             , colore_sfondo,
			XmNiconName               , "MALFUNZIONI",
			XmNiconic                 , True,          
			XmNmaxWidth               , 980,
/*			XmNminWidth               , 930,  */
			XmNminHeight              , 500,
			XmNiconPixmap             , pixmap,
                        XmNbackground             , colore_sfondo,
			XmNfontList               , fontlist_mffr,
			NULL);
						
      form_dialog_malf = XtVaCreateManagedWidget (NULL,
                xmFormWidgetClass, malf_shell,
                XmNbackground             , colore_sfondo,
		XmNfontList               , fontlist_mffr,
                NULL);

      label_dial = XmStringCreateLtoR ("Sommario Malfunzioni",
                                        XmSTRING_DEFAULT_CHARSET);

      dialog_malf = XtVaCreateManagedWidget (NULL,
                xmBulletinBoardWidgetClass, form_dialog_malf,
                XmNallowOverlap           , True,
                XmNautoUnmanage           , False,
                XmNdialogTitle            , label_dial,
/*              XmNwidth                  , 930,            */
                XmNdefaultPosition        , False,
                XmNdialogStyle            , XmDIALOG_MODELESS,
                XmNbackground             , colore_sfondo,
		XmNbottomAttachment       , XmATTACH_FORM,
		XmNleftAttachment       , XmATTACH_FORM,
		XmNfontList               , fontlist_mffr,
                NULL);

                XmStringFree(label_dial);

/*		scroll_malf = XtVaCreateManagedWidget (NULL,     */
		scroll_malf = XtVaCreateWidget (NULL,
				xmScrolledWindowWidgetClass , form_dialog_malf,
				XmNx                      , 0,
/*				XmNwidth                  , 950,      */
				XmNscrollingPolicy        , XmAUTOMATIC,
				XmNvisualPolicy           , XmVARIABLE,
				XmNtopAttachment          , XmATTACH_FORM,
				XmNtopOffset              , 50,
				XmNbottomAttachment       , XmATTACH_FORM,
				XmNbottomOffset           , 100,
				XmNrightAttachment        , XmATTACH_FORM,
				XmNleftAttachment         , XmATTACH_FORM,
				XmNbackground             , colore_tabelle,
				XmNfontList               , fontlist_mffr,
				NULL);
						
		XtVaGetValues (scroll_malf, XmNverticalScrollBar, &vert_scroll_w,NULL);
		XtVaSetValues(vert_scroll_w, XmNinitialDelay, 1000, NULL);

		crea_sommario_malf();
		XtManageChild (scroll_malf);

		crea_top_malf();
		crea_bottoni_comando(); 

		undef_cursore (toplevel_widget);
		attiva_timer(malf_shell);
		attiva_timer_gestione_mf(malf_shell);

		attivato = 1;

}
/******************************************************************/
void delay_new(w, tag, reason)
    Widget w;
    char *tag;
    XmAnyCallbackStruct *reason;
{
/*	printf ("delay cambiato in riga %s\n",tag); */
	if (sommario_mf.delay_in_modifica[atoi(tag)] == 0)
		sommario_mf.delay_in_modifica[atoi(tag)] = 1;
	return;
}
/******************************************************************/
void durata_new(w, tag, reason)
    Widget w;
    char *tag;
    XmAnyCallbackStruct *reason;
{
/* printf ("durata cambiato in riga %s\n",tag); */
	if (sommario_mf.durata_in_modifica[atoi(tag)] == 0)
		sommario_mf.durata_in_modifica[atoi(tag)] = 1;
	return;
}
/******************************************************************/
void target_new(w, tag, reason)
    Widget w;
    char *tag;
    XmAnyCallbackStruct *reason;
{
/* printf ("target cambiato in riga %s\n",tag); */
}
/******************************************************************/
void target_fr_new(w, tag, reason)
    Widget w;
    char *tag;
    XmAnyCallbackStruct *reason;
{
/* printf ("target fr cambiato in riga %s\n",tag); */
}
/******************************************************************/
void rampa_act(w, tag, reason)
    Widget w;
    char *tag;
    XmTextVerifyCallbackStruct *reason;
{
int riga;
int i;
char *stringa;
char app[10];
float valore;
Boolean flag;
int npunti;

   riga = atoi(tag);
   if ((reason->reason == XmCR_ACTIVATE) ||
         (reason->reason == XmCR_LOSING_FOCUS))
      {
		XtVaGetValues (w, XmNeditable, &flag, NULL);
		if (!flag)
			{
			strcpy(app,"");
			XmTextSetString (w,app);
			return;
			}
/*      printf ("rampa activate su riga %s ******\n",tag);  */
      stringa = XmTextGetString (w);
/*      printf ("testo :%s:\n",stringa);  */

		npunti = 0;
      for (i=0; i<strlen(stringa); i++)
         {
         if (stringa[i] == '.')
            npunti++;
         if (((!isdigit(stringa[i])) && (stringa[i] != '.')) ||
            (npunti > 1))
            {
            sprintf (app,"%7.3f",sommario_mf.rampa[riga]);
            XmTextSetString (w,app);
            XtFree (stringa);
            return;
            }
         }
      if (strlen(stringa) == 0)
         {
         valore = 0.0;
        	sprintf (app,"%7.3f",valore);
         XmTextSetString (w,app);
         }
      else
         {
         valore = (float)atof(stringa);
			sprintf (app,"%7.3f",valore);
         XmTextSetString (w,app);
         }
      XtFree (stringa);
		if (valore == sommario_mf.rampa[riga])
      	return;

		if ((!strcmp(sommario_mf.stato[riga], ATTIVATO)) ||
			 (!strcmp(sommario_mf.stato[riga], DISATTIVATO)))
			{
			sommario_mf.rampa[riga] = valore;
			if (sommario_mf.attuale[riga] == sommario_mf.target[riga])
				return;
			else
				{
				stop_pert_riga_sommario_mf(riga);
				sommario_mf.da_riattivare[riga] = DELAY_ALLA_RIATTIVAZIONE;
            return;
				}
			}
		sommario_mf.rampa[riga] = valore;
      }
	return;
}
/******************************************************************/
void rampa_fr_act(w, tag, reason)
    Widget w;
    char *tag;
    XmTextVerifyCallbackStruct *reason;
{
int riga;
int i;
char *stringa;
char app[10];
float valore;
Boolean flag;
int npunti;

   riga = atoi(tag);
   if ((reason->reason == XmCR_ACTIVATE) ||
         (reason->reason == XmCR_LOSING_FOCUS))
      {
      XtVaGetValues (w, XmNeditable, &flag, NULL);
      if (!flag)
         {
         strcpy(app,"");
         XmTextSetString (w,app);
         return;
         }
/*      printf ("rampa fr activate su riga %s ******\n",tag);  */
      stringa = XmTextGetString (w);
/*      printf ("testo :%s:\n",stringa);  */

      npunti = 0;
      for (i=0; i<strlen(stringa); i++)
         {
         if (stringa[i] == '.')
            npunti++;
         if (((!isdigit(stringa[i])) && (stringa[i] != '.')) ||
            (npunti > 1))
            {
            sprintf (app,"%7.3f",sommario_fr.rampa[riga]);
            XmTextSetString (w,app);
            XtFree (stringa);
            return;
            }
         }
      if (strlen(stringa) == 0)
         {
         valore = 0.0;
         sprintf (app,"%7.3f",valore);
         XmTextSetString (w,app);
         }
      else
         {
         valore = (float)atof(stringa);
			sprintf (app,"%7.3f",valore);
         XmTextSetString (w,app);
         }
      XtFree (stringa);
      if (valore == sommario_fr.rampa[riga])
         return;

      if ((!strcmp(sommario_fr.stato[riga], ATTIVATO)) ||
			 (!strcmp(sommario_fr.stato[riga], DISATTIVATO)))
         {
         sommario_fr.rampa[riga] = valore;
         if (sommario_fr.attuale[riga] == sommario_fr.target[riga])
            return;
         else
            {
            stop_pert_riga_sommario_fr(riga);
            sommario_fr.da_riattivare[riga] = DELAY_ALLA_RIATTIVAZIONE;
            return;
            }
         }
      sommario_fr.rampa[riga] = valore;
      }
   return;
}
/******************************************************************/
void delay_act(w, tag, reason)
    Widget w;
    char *tag;
    XmTextVerifyCallbackStruct *reason;
{
int riga;
int i;
char *stringa;
char app[10];

	riga = atoi(tag);
	if ((reason->reason == XmCR_ACTIVATE) || 
			(reason->reason == XmCR_LOSING_FOCUS))
		{
/*   	printf ("delay activate su riga %s ******\n",tag);  */
		stringa = XmTextGetString (w);
/*		printf ("testo :%s:\n",stringa);   */
		sommario_mf.delay_in_modifica[riga] = 0;
		for (i=0; i<strlen(stringa); i++)
			{
			if (!isdigit(stringa[i]))
				{
				sprintf (app,"%4.0f",sommario_mf.delay[riga]);
				XmTextSetString (w,app);
				XtFree (stringa);
				return;	
				}
			}
		if (strlen(stringa) == 0)
			{
			sommario_mf.delay[riga] = 0.0;
			sprintf (app,"%4.0f",sommario_mf.delay[riga]);
			XmTextSetString (w,app);
			}
		else
			{
			sommario_mf.delay[riga] = (float)atof(stringa);
			XmTextSetString (w,stringa);
			}
		XtFree (stringa);
		return;
		}
/******** non funziona
printf ("curr %d, new %d, start %d, end %d\n",
			reason->currInsert,
			reason->newInsert,
			reason->startPos,
			reason->endPos);

	if (reason->text->length > 1)
		{
		printf ("lenght = %d\n",reason->text->length);
		reason->doit = False;
      return;
		}
	if ((reason->text->ptr == NULL) ||
		 (isdigit(reason->text->ptr))) 
		{
		printf ("carattere = :%c: \n",reason->text->ptr);
		reason->doit = True;
		return;
		}
	printf ("lenght = %d\n",reason->text->length);
	printf ("carattere = :%4x: \n",reason->text->ptr[reason->currInsert]);
	reason->doit = False;
****************/
   return;
	
}
/******************************************************************/
void durata_act(w, tag, reason)
    Widget w;
    char *tag;
    XmTextVerifyCallbackStruct *reason;
{
int riga;
int i;
char *stringa;
char app[10];

   riga = atoi(tag);
   if ((reason->reason == XmCR_ACTIVATE) ||
		         (reason->reason == XmCR_LOSING_FOCUS))
      {
/*      printf ("durata activate su riga %s ******\n",tag);  */
      stringa = XmTextGetString (w);
/*      printf ("testo :%s:\n",stringa);  */
		sommario_mf.durata_in_modifica[riga] = 0;
      for (i=0; i<strlen(stringa); i++)
         {
         if (!isdigit(stringa[i]))
            {
            sprintf (app,"%4.0f",sommario_mf.durata[riga]);
            XmTextSetString (w,app);
            XtFree (stringa);
            return;
            }
         }
      if (strlen(stringa) == 0)
         {
         sommario_mf.durata[riga] = 0.0;
			sommario_mf.durata_non_inf[riga] = 0;
         sprintf (app,"%4.0f",sommario_mf.durata[riga]);
         XmTextSetString (w,app);
         }
      else
         {
         sommario_mf.durata[riga] = (float)atof(stringa);
			if (sommario_mf.durata[riga] == 0.0)
				sommario_mf.durata_non_inf[riga] = 0;
			else 
				sommario_mf.durata_non_inf[riga] = 1;
         XmTextSetString (w,stringa);
         }
/*printf ("durata: settato flag = %d\n",sommario_mf.durata_non_inf[riga]);*/
      XtFree (stringa);
      return;
      }
	return;	
}
/******************************************************************/
void target_act(w, tag, reason)
    Widget w;
    char *tag;
	 XmTextVerifyCallbackStruct *reason;
{
int riga;
char *stringa;
int i,npunti;
char app[10];
float valore;
Boolean flag;

	riga = atoi(tag);
	if ((reason->reason == XmCR_ACTIVATE) ||
               (reason->reason == XmCR_LOSING_FOCUS))
      {
      XtVaGetValues (w, XmNeditable, &flag, NULL);
      if (!flag)
         return;
/*   	printf ("target activate su riga %s ******\n",tag);*/
   	stringa = XmTextGetString (w);
/*   	printf ("testo :%s:\n",stringa); */
		npunti = 0;
      for (i=0; i<strlen(stringa); i++)
         {
			if (stringa[i] == '.')
				npunti++;
         if (((!isdigit(stringa[i])) && (stringa[i] != '.')) ||
				(npunti > 1))
            {
            sprintf (app,"%f",sommario_mf.target[riga]);
            XmTextSetString (w,app);
            XtFree (stringa);
            return;
            }
			}
      if (strlen(stringa) == 0)
        	{
        	valore = 0.0;
        	sprintf (app,"%f",valore);
        	XmTextSetString (w,app);
        	}
      else
        	{
        	valore = (float)atof(stringa);
			sprintf (app,"%f",valore);
         XmTextSetString (w,app);
        	}			
		XtFree (stringa);
      if (valore == sommario_mf.target[riga])
         return;
      if ((!strcmp(sommario_mf.stato[riga], ATTIVATO)) ||
			 (!strcmp(sommario_mf.stato[riga], DISATTIVATO)))
         {
         if (sommario_mf.attuale[riga] != sommario_mf.target[riga])
            stop_pert_riga_sommario_mf(riga);
			sommario_mf.target[riga] = valore;
			sommario_mf.da_riattivare[riga] = DELAY_ALLA_RIATTIVAZIONE;
			return;
         }
      sommario_mf.target[riga] = valore;
      }
   return;
}
/******************************************************************/
void target_fr_act(w, tag, reason)
    Widget w;
    char *tag;
    XmAnyCallbackStruct *reason;
{
int riga;
char *stringa;
int i,npunti;
char app[10];
float valore;
Boolean flag;

   riga = atoi(tag);
   if ((reason->reason == XmCR_ACTIVATE) ||
               (reason->reason == XmCR_LOSING_FOCUS))
      {
      XtVaGetValues (w, XmNeditable, &flag, NULL);
      if (!flag)
         return;
/*      printf ("target fr activate su riga %s ******\n",tag); */
      stringa = XmTextGetString (w);
/*      printf ("testo :%s:\n",stringa); */
      npunti = 0;
      for (i=0; i<strlen(stringa); i++)
         {
         if (stringa[i] == '.')
            npunti++;
         if (((!isdigit(stringa[i])) && (stringa[i] != '.')) ||
            (npunti > 1))
            {
            sprintf (app,"%f",sommario_fr.target[riga]);
            XmTextSetString (w,app);
            XtFree (stringa);
            return;
            }
			}
      if (strlen(stringa) == 0)
         {
         valore = 0.0;
         sprintf (app,"%f",valore);
         XmTextSetString (w,app);
         }
      else
         {
         valore = (float)atof(stringa);
			sprintf (app,"%f",valore);
         XmTextSetString (w,app);
         }
      XtFree (stringa);
      if (valore == sommario_fr.target[riga])
         return;
      if ((!strcmp(sommario_fr.stato[riga], ATTIVATO)) ||
			 (!strcmp(sommario_fr.stato[riga], DISATTIVATO)))
         {
         if (sommario_fr.attuale[riga] != sommario_fr.target[riga])
            stop_pert_riga_sommario_fr(riga);
         sommario_fr.target[riga] = valore;
         sommario_fr.da_riattivare[riga] = DELAY_ALLA_RIATTIVAZIONE;
         return;
         }
      sommario_fr.target[riga] = valore;
      }
   return;
}
/******************************************************************/
void rampa_new(w, tag, reason)
    Widget w;
    char *tag;
    XmAnyCallbackStruct *reason;
{
}
/******************************************************************/
void rampa_fr_new(w, tag, reason)
    Widget w;
    char *tag;
    XmAnyCallbackStruct *reason;
{
}
/******************************************************************/
void label_focus(w, tag, reason)
    Widget w;
    char *tag;
    XmAnyCallbackStruct *reason;
{
int riga;
char *stringa;

/*   printf ("label focus su riga %s ******\n",tag); */
   stringa = XmTextGetString (w);
/*   printf ("testo :%s:\n",stringa); */
   XtFree (stringa);
}
/******************************************************************/
void label_losing_focus(w, tag, reason)
    Widget w;
    char *tag;
    XmAnyCallbackStruct *reason;
{
int riga;
char *stringa;

/*   printf ("label terminato focus su riga %s ******\n",tag); */
   stringa = XmTextGetString (w);
/*   printf ("testo :%s:\n",stringa); */
   XtFree (stringa);
}
/******************************************************************/
/*******
int reset_toggle (int n)
{
char riga[4];
XmToggleButtonCallbackStruct reason;

	sprintf (riga,"%d",n);
	reason.set = 0;
	toggled (toggle_w[n], riga, &reason);
	XtVaSetValues (toggle_w[n],
		XmNset,     False,
		NULL);
	return(0);
}
*************/
/******************************************************************/
void toggled(w, tag, reason)
    Widget w;
    char *tag;
    XmToggleButtonCallbackStruct *reason;
{
/*   printf ("cliccato toggle button %s : %d *******\n",
                    tag,reason->set);  */
	if (reason->set)
		{
		riga_selezionata[atoi(tag)] = 1;
		sommario_mf.marcato[atoi(tag)] = MARCATO;
/*		printf (" marcato %d\n",atoi(tag)); */
		}
	else
		{
		riga_selezionata[atoi(tag)] = 0;
		sommario_mf.marcato[atoi(tag)] = NON_MARCATO;
/*		printf (" smarcato %d\n",atoi(tag)); */
		}
}
/******************************************************************/
int cambia_colori_mf (Pixel p, int riga)
{
/*
 * cambia il colore dell' intera riga delle malfunzioni
 * a seguito di selezione o cambio di stato
 */

	XtVaSetValues (label_w[riga], XmNbackground,   p, NULL);
	XtVaSetValues (descrizione_w[riga], XmNbackground,   p, NULL);
	XtVaSetValues (val_default_w[riga], XmNbackground,   p, NULL);
	XtVaSetValues (delay_w[riga], XmNbackground,   p, NULL);
	XtVaSetValues (durata_w[riga], XmNbackground,   p, NULL);
	XtVaSetValues (rampa_w[riga], XmNbackground,   p, NULL);
	XtVaSetValues (val_fin_w[riga], XmNbackground,   p, NULL);
	XtVaSetValues (val_att_w[riga], XmNbackground,   p, NULL);
	XtVaSetValues (stato_mf_w[riga], XmNbackground,   p, NULL);

	return(0);
}
/******************************************************************/
int crea_top_malf()
{
int i;
Position ascisse[8];
char *top_label[] = { /*"Label",*/ "Descrizione", "Val. Default", "Delay",
		   "Durata", "Rampa", "Target", "Attuale", /*"Stato",*/ "NULL"};
XmString label_top;

   i=0;
/*		XtVaGetValues (label_w[0], XmNx, &ascisse[i], NULL); i++;**/
   XtVaGetValues (descrizione_w[0], XmNx, &ascisse[i], NULL); i++;
   XtVaGetValues (val_default_w[0], XmNx, &ascisse[i], NULL); i++;
   XtVaGetValues (delay_w[0], XmNx, &ascisse[i], NULL); i++;
   XtVaGetValues (durata_w[0], XmNx, &ascisse[i], NULL); i++;
   XtVaGetValues (rampa_w[0], XmNx, &ascisse[i], NULL); i++;
   XtVaGetValues (val_fin_w[0], XmNx, &ascisse[i], NULL); i++;
   XtVaGetValues (val_att_w[0], XmNx, &ascisse[i], NULL); i++;
/*      XtVaGetValues (stato_mf_w[0], XmNx, &ascisse[i], NULL); i++;  **/

   form_label_top = XtVaCreateManagedWidget (NULL,
                            xmFormWidgetClass,   form_dialog_malf,
                            XmNtopAttachment , XmATTACH_FORM,
                            XmNleftAttachment, XmATTACH_FORM,
                            XmNrightAttachment, XmATTACH_FORM,
                            XmNbackground, colore_sfondo,
                            XmNfontList, fontlist_mffr,
                            NULL);
   i=0;
   while (strcmp(top_label[i],"NULL"))
      {

      label_top = XmStringCreateLtoR (top_label[i],
                                      XmSTRING_DEFAULT_CHARSET);
      XtVaCreateManagedWidget (NULL,
                  xmLabelWidgetClass, form_label_top,
                  XmNlabelString    , label_top,
                  XmNy              , y_init_top_label,
                  XmNx              , ascisse[i],
                  XmNbackground     , colore_sfondo,
                  XmNfontList               , fontlist_mffr,
                  NULL);
      i++;
      XmStringFree(label_top);
      }
   return(0);
}
/******************************************************************/
int crea_sommario_malf()
{
int i;
char temp[100];

      main_malf = XtVaCreateWidget ("main_malf",
                  xmRowColumnWidgetClass, scroll_malf,
                  XmNbackground      , colore_tabelle,
                  XmNforeground      , colore_tabelle,
						XmNrightAttachment , XmATTACH_WIDGET,
						XmNrightWidget     ,	form_dialog_malf,
                  NULL);

      for (i=0; i<MAX_MF_SOMM; i++)
         {
      form_mf = XtVaCreateWidget (NULL,
                  xmFormWidgetClass, main_malf,  
                  XmNbackground      , colore_tabelle,
                  XmNforeground      , colore_tabelle,
						XmNrightAttachment , XmATTACH_WIDGET,
                  XmNrightWidget     , form_dialog_malf,
                  NULL);
      toggle_w[i] = XtVaCreateManagedWidget (NULL,
                  xmToggleButtonWidgetClass, form_mf,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_FORM,
                  XmNbackground      , colore_tabelle,
                  XmNselectColor     , pix_val[0],
                  NULL);

      label_w[i] = XtVaCreateWidget ("label",
                  xmTextWidgetClass,   form_mf,
                  XmNcolumns      ,    8,
                  XmNeditable     ,    False,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNleftAttachment,    XmATTACH_WIDGET,
                  XmNleftWidget      , toggle_w[i],
                  XmNcursorPositionVisible, False,
                  XmNbackground        , colore_tabelle,
						XmNhighlightThickness, 2,
						XmNshadowThickness   , 2,
						XmNfontList               , fontlist_mffr,
                  NULL);
/*		XtManageChild (label_w[i]);       */

      descrizione_w[i] = XtVaCreateManagedWidget ("descrizione",
                  xmTextWidgetClass,   form_mf,
                  XmNcolumns      ,    50,
                  XmNeditable     ,    False,
                  XmNleftAttachment  , XmATTACH_WIDGET,
 /*                 XmNleftWidget      , label_w[i],     */
                  XmNleftWidget      , toggle_w[i],
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNcursorPositionVisible, False,
                  XmNbackground        , colore_tabelle,
						XmNfontList               , fontlist_mffr,
                  NULL);

      val_default_w[i] = XtVaCreateManagedWidget ("default",
                  xmTextWidgetClass,   form_mf,
                  XmNcolumns      ,    10,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_WIDGET,
                  XmNleftWidget      , descrizione_w[i],
                  XmNcursorPositionVisible, False,
                  XmNeditable     ,    False,
                  XmNbackground        , colore_tabelle,
						XmNfontList               , fontlist_mffr,
                  NULL);

      delay_w[i] = XtVaCreateManagedWidget ("delay",
                  xmTextWidgetClass,   form_mf,
                  XmNcolumns      ,    4,
						XmNmaxLength       , 4,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_WIDGET,
                  XmNleftWidget      , val_default_w[i],
                  XmNcursorPositionVisible, True,
                  XmNbackground        , colore_tabelle,
						XmNeditable          , False,
						XmNfontList               , fontlist_mffr,
                  NULL);

      durata_w[i] = XtVaCreateManagedWidget ("durata",
                  xmTextWidgetClass,   form_mf,
                  XmNcolumns      ,    4,
						XmNmaxLength       , 4,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_WIDGET,
                  XmNleftWidget      , delay_w[i],
                  XmNcursorPositionVisible, True,
                  XmNbackground        , colore_tabelle,
						XmNeditable          , False,
						XmNfontList               , fontlist_mffr,
                  NULL);

      rampa_w[i] = XtVaCreateManagedWidget ("rampa",
                  xmTextWidgetClass,   form_mf,
                  XmNcolumns      ,    8,
						XmNmaxLength       , 8,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_WIDGET,
                  XmNleftWidget      , durata_w[i],
                  XmNcursorPositionVisible, True,
                  XmNbackground        , colore_tabelle,
						XmNeditable          , True,
						XmNfontList               , fontlist_mffr,
                  NULL);

      val_fin_w[i] = XtVaCreateManagedWidget ("val_fin",
                  xmTextWidgetClass,   form_mf,
                  XmNcolumns      ,    10,
						XmNmaxLength       , 10,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_WIDGET,
                  XmNleftWidget      , rampa_w[i],
                  XmNcursorPositionVisible, True,
                  XmNbackground        , colore_tabelle,
						XmNfontList               , fontlist_mffr,
                  NULL);

      val_att_w[i] = XtVaCreateManagedWidget ("val_att",
                  xmTextWidgetClass,   form_mf,
                  XmNcolumns      ,    10,
                  XmNeditable     ,    False,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_WIDGET,
                  XmNleftWidget      , val_fin_w[i],
						XmNrightAttachment , XmATTACH_FORM,
                  XmNcursorPositionVisible, False,
                  XmNbackground        , colore_tabelle,
						XmNfontList               , fontlist_mffr,
                  NULL);

      stato_mf_w[i] = XtVaCreateWidget ("stato_mf",
                  xmTextWidgetClass,   form_mf,
                  XmNcolumns      ,    10,
                  XmNeditable     ,    False,
                  XmNtopAttachment,    XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  XmNleftAttachment  , XmATTACH_WIDGET,
                  XmNleftWidget      , val_att_w[i],
						XmNrightAttachment , XmATTACH_FORM,
                  XmNcursorPositionVisible, False,
                  XmNbackground        , colore_tabelle,
						XmNfontList               , fontlist_mffr,
                  NULL);
/*		XtManageChild (stato_mf_w[i]);        */

		aggiorna_riga_sommario(i);
/*
 * callbacks relative ai campi
 */
      sprintf (nriga[i],"%d",i);
      XtAddCallback (toggle_w[i],XmNvalueChangedCallback,toggled,nriga[i]);

      XtAddCallback (delay_w[i],XmNfocusCallback,delay_act,nriga[i]);
      XtAddCallback (delay_w[i],XmNvalueChangedCallback,delay_act,nriga[i]); 
      XtAddCallback (delay_w[i],XmNactivateCallback,delay_act,nriga[i]);
      XtAddCallback (delay_w[i],XmNlosingFocusCallback,delay_act,nriga[i]);

/*		XtAddCallback (durata_w[i],XmNmodifyVerifyCallback,durata_act,nriga[i]);*/
      XtAddCallback (durata_w[i],XmNvalueChangedCallback,durata_new,nriga[i]);
      XtAddCallback (durata_w[i],XmNactivateCallback,durata_act,nriga[i]);
		XtAddCallback (durata_w[i],XmNlosingFocusCallback,durata_act,nriga[i]);

/*    XtAddCallback (rampa_w[i],XmNvalueChangedCallback,rampa_new,nriga[i]);*/
      XtAddCallback (rampa_w[i],XmNactivateCallback,rampa_act,nriga[i]);
		XtAddCallback (rampa_w[i],XmNlosingFocusCallback,rampa_act,nriga[i]);

/*    XtAddCallback(val_fin_w[i],XmNvalueChangedCallback,target_new,nriga[i]);*/
      XtAddCallback (val_fin_w[i],XmNactivateCallback,target_act,nriga[i]);
		XtAddCallback (val_fin_w[i],XmNlosingFocusCallback,target_act,nriga[i]);

/*****************

      XtAddCallback (delay_w[i],XmNfocusCallback,label_focus,nriga[i]);
      XtAddCallback (durata_w[i],XmNfocusCallback,label_focus,nriga[i]);
      XtAddCallback (durata_w[i],XmNlosingFocusCallback,
                                 label_losing_focus,nriga[i]);
      XtAddCallback (rampa_w[i],XmNfocusCallback,label_focus,nriga[i]);
      XtAddCallback (rampa_w[i],XmNlosingFocusCallback,
                                 label_losing_focus,nriga[i]);
      XtAddCallback (val_fin_w[i],XmNfocusCallback,label_focus,nriga[i]);
      XtAddCallback (val_fin_w[i],XmNlosingFocusCallback,
                                 label_losing_focus,nriga[i]);
****************/
		riga_selezionata[i] = 0;   /* non selezionata per default        */

      XtManageChild (form_mf);
         }
      XtManageChild (main_malf);

		return(0);
}
/******************************************************************/
int crea_bottoni_comando()
{
Widget form_tasti;
int i;
char str[10];
#define NUM_CMD_MF       5  /* numero MAX tasti comandi in calce al sommario */
Widget  frame_cmd[NUM_CMD_MF];
Widget  tasto[NUM_CMD_MF];
XmString label_tasto;
struct cmd_mf_st
	{
	char *str;
	}cmd_mf[]={tasto_att, tasto_dis, tasto_del, tasto_add, tasto_exit, "NULL"};

      form_tasti = XtVaCreateManagedWidget (NULL,
                  xmFormWidgetClass, dialog_malf,
                  XmNbackground      , colore_sfondo,
                  XmNforeground      , colore_sfondo,
                  XmNleftAttachment      ,  XmATTACH_FORM,
                  XmNrightAttachment     ,  XmATTACH_FORM,
                  XmNbottomAttachment    ,  XmATTACH_FORM,
                  XmNx                   ,  0,
                  XmNy                   ,  y_init_tasti,  
                  XmNfontList               , fontlist_mffr,
                  NULL);

      XtVaCreateManagedWidget (NULL,
                  xmFrameWidgetClass     ,  form_tasti,
                  XmNwidth               ,  945,   
                  XmNheight              ,  70,      
                  XmNbottomAttachment    ,  XmATTACH_FORM,
                  XmNleftAttachment      ,  XmATTACH_FORM,
                  XmNbackground      , colore_sfondo,
                  XmNfontList               , fontlist_mffr,
                  NULL);
	
      i=0;
      while (strcmp(cmd_mf[i].str,"NULL"))
         {
         label_tasto = XmStringCreateLtoR (cmd_mf[i].str,
                                           XmSTRING_DEFAULT_CHARSET);
         frame_cmd[i] = XtVaCreateWidget (NULL,
                            xmFrameWidgetClass     ,  form_tasti,
                            XmNwidth               ,  120,
                            XmNheight              ,  30,
                            XmNbottomAttachment    ,  XmATTACH_FORM,
                            XmNbottomOffset        ,  10,
                            XmNbackground      , colore_tasti,
						NULL);
         tasto[i] = XtVaCreateWidget (NULL,
			xmPushButtonWidgetClass, frame_cmd[i],
			XmNlabelString, label_tasto,
			XmNwidth               , 120,
	          	XmNheight              , 30,   
			XmNdefaultButtonShadowThickness ,  1,
          		XmNbackground      , colore_tasti,
			XmNfontList               , fontlist_mffr,
			NULL);
         sprintf (str,"%d",i);
         XtAddCallback (tasto[i], XmNactivateCallback, tasto_mf, cmd_mf[i].str);
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

   for (i=0; i<NUM_CMD_MF; i++)
      {
      XtManageChild (frame_cmd[i]);
      XtManageChild (tasto[i]);
      }
/*********
	XtVaSetValues (tasto[0], XmNshowAsDefault , 1, NULL);
	XtVaSetValues (dialog_malf, XmNdefaultButton, tasto[0], NULL);
*******************************************************/
	return(0);
}
/***************************************************************/
void tasto_mf(w, data, cbs)
Widget w;
XtPointer data;
XmPushButtonCallbackStruct *cbs;
{
AGGIORNA_VALORI aggiornamento;
int i;
int n;
int size;

/*	printf ("premuto bottone %s\n",data); */
	
	if (!strcmp(data,tasto_att))
		{
		attivazione_mf();
		}
	else if (!strcmp(data,tasto_dis))
		{
		disattivazione_mf();
      }
	else if (!strcmp(data,tasto_del))
		{
		cancellazione_mf();
      }
	else if (!strcmp(data,tasto_add))
      {
		select_malf_proc();
      }
	else if (!strcmp(data,tasto_exit))
      {
		fine_sommario_mf();
      }
	else
		printf ("richiesta non riconosciuta\n");
}
/******************************************************************/
int cancellazione_mf()
{
int i,n;

		n = sommario_mf.numero;
      for (i=0; i<n; i++)
         {
         if ((sommario_mf.marcato[i] == MARCATO) &&
             (sommario_mf.occupato[i] == OCCUPATO) &&
             (!strcmp(sommario_mf.stato[i], SELEZIONATO)))
            {
            cancella_riga_sommario(i);
            aggiorna_riga_sommario (i);
            XmToggleButtonSetState (toggle_w[i],False,True);
            if (sommario_mf.numero == MAX_MF_SOMM)
               tasto_sbloccato(tasto_ok);
            sommario_mf.numero--;
            }
         }
      impacca_sommario();
		return(0);
}
/******************************************************************/
int fine_sommario_mf()
{
/*		XtDestroyWidget (malf_shell);     *********/
		XtRemoveTimeOut (timer_mf);
		XtUnmanageChild (malf_shell);     
}
/******************************************************************/
int clear_sommario_mf()
{
int i;

	for (i=0; i<MAX_MF_SOMM; i++)
		{
		strcpy (sommario_mf.label[i], "");
		strcpy (sommario_mf.descrizione[i], "");
		sommario_mf.address[i] = -1;
		sommario_mf.delay[i] = 0.0;
		sommario_mf.durata[i] = 0.0;
		sommario_mf.durata_non_inf[i] = 0;
		sommario_mf.rampa[i] = 0.0;
		sommario_mf.target[i] = 0.0;
		sommario_mf.attuale[i] = 0.0;
		strcpy (sommario_mf.stato[i],  "");
		sommario_mf.marcato[i] = NON_MARCATO;
		sommario_mf.occupato[i] = NON_OCCUPATO;
		sommario_mf.da_riattivare[i] = 0;
		sommario_mf.delay_in_modifica[i] = 0;
		sommario_mf.durata_in_modifica[i] = 0;
		}
	sommario_mf.numero = 0;

	return(0);
}
/******************************************************************/
int cancella_riga_sommario (int riga)
{

/*  printf ("cancella riga %d\n",riga);  */

	strcpy (sommario_mf.label[riga], "");
	strcpy (sommario_mf.descrizione[riga], "");
	sommario_mf.address[riga] = -1;
	sommario_mf.delay[riga] = 0.0;
	sommario_mf.durata[riga] = 0.0;
	sommario_mf.durata_non_inf[riga] = 0;
	sommario_mf.rampa[riga] = 0.0;
	sommario_mf.target[riga] = 0.0;
   sommario_mf.attuale[riga] = 0.0;
   strcpy (sommario_mf.stato[riga],  "");
   sommario_mf.marcato[riga] = NON_MARCATO;
	sommario_mf.occupato[riga] = NON_OCCUPATO;
	sommario_mf.da_riattivare[riga] = 0;
	sommario_mf.delay_in_modifica[riga] = 0;
   sommario_mf.durata_in_modifica[riga] = 0;

/*	cambia_colori_mf (colore_tabelle, riga);  */

	return (0);
}
/******************************************************************/
int impacca_sommario()
{
int i;
int primo_vuoto, primo_pieno, ultimo_vuoto;

for (;;)
	{
	for (i=0; i<MAX_MF_SOMM; i++)
		if (sommario_mf.occupato[i] == NON_OCCUPATO)
			break;
	if (i == MAX_MF_SOMM)
		return(0);
	primo_vuoto = i;

	for (i=i+1; i<MAX_MF_SOMM; i++)
		if (sommario_mf.occupato[i] == OCCUPATO)
			break;
	if (i == MAX_MF_SOMM)
      return(0);
	ultimo_vuoto = i-1;
	primo_pieno  = i;

	while (sommario_mf.occupato[i] == OCCUPATO)
		{
		sposta_riga_sommario (primo_vuoto,primo_pieno);
		primo_vuoto++;
		primo_pieno++;
		i++;
		}
	}
}
/******************************************************************/
int sposta_riga_sommario (int d, int s)
{
	copia_riga_sommario (d,s);
	cancella_riga_sommario (s);
	aggiorna_riga_sommario (d);
	aggiorna_riga_sommario (s);  

	return(0);
}
/******************************************************************/
int copia_riga_sommario (int d, int s)
{
Boolean stato_toggle;
Pixel colore;

	strcpy (sommario_mf.label[d], sommario_mf.label[s]);
	strcpy (sommario_mf.descrizione[d], sommario_mf.descrizione[s]);
	sommario_mf.address[d] = sommario_mf.address[s];
   sommario_mf.delay[d] = sommario_mf.delay[s];
   sommario_mf.durata[d] = sommario_mf.durata[s];
	sommario_mf.durata_non_inf[d] = sommario_mf.durata_non_inf[s];
   sommario_mf.rampa[d] = sommario_mf.rampa[s];
   sommario_mf.target[d] = sommario_mf.target[s];
   sommario_mf.attuale[d] = sommario_mf.attuale[s];
   strcpy (sommario_mf.stato[d], sommario_mf.stato[s]);
   sommario_mf.marcato[d] =  sommario_mf.marcato[s];
   sommario_mf.occupato[d] = sommario_mf.occupato[s];
	sommario_mf.da_riattivare[d] = sommario_mf.da_riattivare[s];
	sommario_mf.delay_in_modifica[d] = sommario_mf.delay_in_modifica[s];
	sommario_mf.durata_in_modifica[d] = sommario_mf.durata_in_modifica[s];

	XtVaGetValues (toggle_w[s], XmNset,  &stato_toggle, NULL);
	XtVaSetValues (toggle_w[d], XmNset,  stato_toggle, NULL);
	
	XtVaGetValues (descrizione_w[s], XmNbackground, &colore, NULL);
	cambia_colori_mf (colore, d);
   return(0);
}
/***************************************************************/
int stampa_somm()
{
	int i;
	for (i=0; i<40; i++)
		printf ("%d) :%s:\n",i,sommario_mf.descrizione[i]);
	return(0);
}
/***************************************************************/
int attiva_timer (Widget w)
{
	timer_mf = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
						(unsigned long)5000, refresh_malf, NULL);
	return(0);
}
/***************************************************************/
int attiva_timer_gestione_mf (Widget w)
{
   timer_gestione_mf = XtAppAddTimeOut (XtWidgetToApplicationContext(w),
                  (unsigned long)2000, gestione_malf, NULL);
   return(0);
}
/***************************************************************/
static  void refresh_malf (client_data, id)
caddr_t client_data;
XtIntervalId * id;
{
	aggiorna_valori_mf();
	attiva_timer (malf_shell);	
}
/***************************************************************/
static  void gestione_malf (client_data, id)
caddr_t client_data;
XtIntervalId * id;
{
	if (val_agg.stato_sked == STATO_RUN)
   	malf_management();

   attiva_timer_gestione_mf (malf_shell);
}
/***************************************************************/
int attivazione_mf()
{
int n,i;

		n = sommario_mf.numero;
      for (i=0; i<n; i++)
         {
         if ((sommario_mf.marcato[i] == MARCATO) &&
             (sommario_mf.occupato[i] == OCCUPATO) &&
             (!strcmp(sommario_mf.stato[i], SELEZIONATO)))
            {
            inserisci_riga_sommario(i);
            XmToggleButtonSetState (toggle_w[i],False,True);
            }
         }
		return(0);
}
/***************************************************************/
int inserisci_riga_sommario(int n)
{
/*		printf ("INSERITA riga %d\n",n);  */
		strcpy (sommario_mf.stato[n], INSERITO);
		cambia_colori_mf (colore_inserito, n);		
		return(0);
}
/***************************************************************/
int disattivazione_mf()
{
int n,i;

		n = sommario_mf.numero;
      for (i=0; i<n; i++)
         {
         if ((sommario_mf.marcato[i] == MARCATO) &&
             (sommario_mf.occupato[i] == OCCUPATO))
				{
             if (!strcmp(sommario_mf.stato[i], INSERITO))
            	{
            	disinserisci_riga_sommario(i);
            	XmToggleButtonSetState (toggle_w[i],False,True);
            	}
				 else if (!strcmp(sommario_mf.stato[i], ATTIVATO))
					{
					if (sommario_mf.attuale[i] == sommario_mf.target[i])
						{
						printf ("*** Disattivazione riga %d\n",n);
						strcpy (sommario_mf.stato[i], DISATTIVATO);
						perturba_riga_sommario_mf(i, sommario_mf.val_def[i],0.0);
						}
					else
						{
						stop_pert_riga_sommario_mf(i);
						strcpy (sommario_mf.stato[i], DISATTIVATO);
						sommario_mf.da_riattivare[i] = DELAY_ALLA_RIATTIVAZIONE;
						}
					XmToggleButtonSetState (toggle_w[i],False,True);
					}
				}
         }
      return(0);
}
/***************************************************************/
int disinserisci_riga_sommario(int n)
{
/*		printf ("DISINSERITA riga %d\n",n); */
      strcpy (sommario_mf.stato[n], SELEZIONATO);
      cambia_colori_mf (colore_selezionato, n);
		return(0);
}
/***************************************************************/
int malf_management()
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
	n = sommario_mf.numero;
      for (i=0; i<n; i++)
         {
         if (sommario_mf.occupato[i] == OCCUPATO) 
				{
             if (!strcmp(sommario_mf.stato[i], INSERITO))
            	{
					sommario_mf.delay[i] -= deltat;
					if (sommario_mf.delay[i] <= 0.0)
						{
						sommario_mf.delay[i] = 0.0;
						attiva_riga_sommario(i);
						}
					aggiorna_delay(i);
					}
				 else if (!strcmp(sommario_mf.stato[i], ATTIVATO))
					{
					if (sommario_mf.da_riattivare[i] > 0)
						{
						sommario_mf.da_riattivare[i] -= (int)deltat;
						if (sommario_mf.da_riattivare[i] <= 0)
							{
							sommario_mf.da_riattivare[i] = 0;
							perturba_riga_sommario_mf(i, sommario_mf.target[i],0.0);
							}
						}
					if (sommario_mf.durata_non_inf[i] != 0)
						{
						sommario_mf.durata[i] -= deltat;
						if (sommario_mf.durata[i] <= 0.0)
                  	{
							sommario_mf.durata[i] = 0;
							if (sommario_mf.attuale[i] == sommario_mf.target[i])
								{
								printf ("*** Disattivazione riga %d\n",i);
								strcpy (sommario_mf.stato[i], DISATTIVATO);
								perturba_riga_sommario_mf(i,sommario_mf.val_def[i],0.0);
								}
							else
								{
								printf ("*** Disattivazione riga %d\n",i);
   							strcpy (sommario_mf.stato[i], DISATTIVATO);
								stop_pert_riga_sommario_mf(i);
								sommario_mf.da_riattivare[i]=DELAY_ALLA_RIATTIVAZIONE;
								}
							}
						aggiorna_durata(i);
						}
					}
				 else if (!strcmp(sommario_mf.stato[i], DISATTIVATO))
					{
					if (sommario_mf.da_riattivare[i] > 0)
                  {
                  sommario_mf.da_riattivare[i] -= (int)deltat;
                  if (sommario_mf.da_riattivare[i] <= 0)
                     {
                     sommario_mf.da_riattivare[i] = 0;
                     perturba_riga_sommario_mf(i, sommario_mf.val_def[i],0.0);
                     }
                  }
					if (sommario_mf.attuale[i] == sommario_mf.val_def[i])
						seleziona_riga_sommario(i);
					}
				}
			}
	tempo_old = tempo;
	return(0);	
}
/***************************************************************/
int aggiorna_delay (int riga)
{
char temp[20];
	sprintf (temp,"%4.0f",sommario_mf.delay[riga]);
	XtVaSetValues (delay_w[riga],XmNvalue,temp,NULL);
	return(0);
}
/***************************************************************/
int aggiorna_durata (int riga)
{
char temp[20];
   sprintf (temp,"%4.0f",sommario_mf.durata[riga]);
   XtVaSetValues (durata_w[riga],XmNvalue,temp,NULL);
   return(0);
}
/***************************************************************/
int attiva_riga_sommario (int n)
{
int tipo_pert;
float rateo;

/*	printf ("*** Attivazione riga %d\n",n);  */
	perturba_riga_sommario_mf(n, sommario_mf.target[n],0.0);
	strcpy (sommario_mf.stato[n], ATTIVATO);
   cambia_colori_mf (colore_attivato, n);
	return(0);
}
/***************************************************************/
/* Stile prototipo, NON K&R: come perturba_riga_sommario_fr in monit_frem.c
   (prototipo a riga 104). Vedi docs/KR_PROTOTYPE_AUDIT.md. */
int perturba_riga_sommario_mf (int n, float valore, float t)
{
extern int id_msg_pert;
int tipo_pert;
float rateo;

   if (sommario_mf.rampa[n] == 0)
      {
      tipo_pert = PERT_SCALINO;
      rateo = 0.0;
      }
   else
      {
      tipo_pert = PERT_RAMPA;
      rateo = sommario_mf.rampa[n]/60.0;
      }

   pert_snd(id_msg_pert,
               tipo_pert,
               sommario_mf.address[n],
               valore,
               rateo,
					0.0, 0.0, 0.0, 0.0, 0.0,
               t);
	return(0);
}
/***************************************************************/
int stop_pert_riga_sommario_mf (n)
int n;
{
extern int id_msg_pert;

	pert_snd(id_msg_pert,
				PERT_RAMPA_STOP,
				sommario_mf.address[n],
				0.,
				0.,
				0.0, 0.0, 0.0, 0.0, 0.0,
				0.);
	return(0);
}
/***************************************************************/
int stop_pert_riga_sommario_fr (n)
int n;
{
extern int id_msg_pert;

   pert_snd(id_msg_pert,
            PERT_RAMPA_STOP,
            sommario_fr.address[n],
            0.,
            0.,
				0.0, 0.0, 0.0, 0.0, 0.0,
            0.);
   return(0);
}
/***************************************************************/
int seleziona_riga_sommario(int n)
{
/*	printf ("*** Riga %d torna selezionata ***\n",n);  */
	strcpy (sommario_mf.stato[n], SELEZIONATO);
	sommario_mf.durata_non_inf[n] = 0;
	cambia_colori_mf (colore_selezionato, n);
	return(0);
}
/***************************************************************/
int setta_colori()
{
	colore_sfondo = pix_val[5];
	colore_tabelle = colore_sfondo; 
	colore_tasti =  colore_sfondo;
	colore_selezionato = colore_tabelle;
	colore_scrollbar  = pix_val[10];

	colore_sfondo_fr = pix_val[7];
	colore_tabelle_fr = colore_sfondo_fr; 
	colore_tasti_fr = colore_sfondo_fr;
	colore_selezionato_fr = colore_tabelle_fr;
	colore_scrollbar_fr  = pix_val[11];

	colore_inserito = pix_val[2];
	colore_attivato = pix_val[3];
}
#endif
