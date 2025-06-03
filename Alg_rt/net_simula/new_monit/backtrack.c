/**********************************************************************
*
*       C Source:               backtrack.c
*       Subsystem:              1
*       Description:
*       %created_by:    zanna %
*       %date_created:  Mon Jun 16 17:37:46 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: backtrack.c-5 %  (%full_filespec: backtrack.c-5:csrc:1 %)";
#endif

/*
 * backtrack.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>

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

/* #include "interf_attive.h"  */
extern int bt_interface_active;
#include "parametri.h"
#include "messaggi.h"
#include "bistrutt.h"
#include "cursore.h"

#include "backtrack.h"
#include "backtrackManagement.h"

#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"

#include "refresh.h"
extern BKTAB *bt_header;
int numeroBtLista;			/* contatore numero bt in lista				*/
extern   VAL val;

extern int create_richiestaDati();
extern STATISTICHE statistiche;
extern Widget areaMessaggi;
extern XtIntervalId timer_replayOn;
extern int stato_sim;  /* stato simulatore come aggiornato da statistiche */
extern int inizializzazione;  /* flag per eventuale cambio sessione       */

extern int _MAX_BACK_TRACK;

int SD_editbt (int,BKTAB *,int);
int crea_cambio_sessione (Widget,char*);
int crea_backtrack_entry(Widget,int);
int displayDatiRecordBt (BKTAB *, int );
int ricarica_default_ci (Widget);
int SD_btrecstep (int, float*);
int     SD_validatebt (int);
int prossimo_step();
int mess_replay (Widget, Boolean);
int tasti_replay(Widget, Boolean);
int attiva_timer_replayOn (Widget);
int lastBtLoad(Widget);
int     SD_lbtload (int);


/**********************************************************/
/*
 * funzione introdotta per allocare aree dinamiche invece che 
 * statiche dopo le modifiche allo schedulatore
 */
int AllocaAreeBacktrack()
{
static int primo_giro = 1;

	if (primo_giro)
		{
		BtScrollForm = (Widget *)XtMalloc (sizeof(Widget)*_MAX_BACK_TRACK);
		BtNumber = (Widget *)XtMalloc (sizeof(Widget)*_MAX_BACK_TRACK);
		BtDate = (Widget *)XtMalloc (sizeof(Widget)*_MAX_BACK_TRACK);
		BtSimtime = (Widget *)XtMalloc (sizeof(Widget)*_MAX_BACK_TRACK);
	
		n_entry = (char *)malloc(sizeof(char)*_MAX_BACK_TRACK*4);
		punt_rec_backtrack = (int *)malloc(sizeof(int)*_MAX_BACK_TRACK);
		punt_rec_bt = (int *)malloc(sizeof(int)*_MAX_BACK_TRACK);
		primo_giro = 0;
		}
}
/**********************************************************/
int read_lista_bt (BKTAB *bthead)
{

	if (SD_editbt(BANCO, bthead, _MAX_BACK_TRACK) > 0)
		return(0);
	return (-1);
}
/**********************************************************/
int display_lista_bt (lista, p)
Widget lista;
BKTAB *p;
{
int i;
char stringa[80];
char *str;
XmString c_str;
int pos;
BKTAB *dati;
char flag[2];
char *messaggio;

printf ("display bt : last = %d\n",last_backtrack);
	if (!last_backtrack)
		{
		messaggio = (char *)malloc (strlen(NO_BT_LIST) + 
									strlen (NOT_EXECUTABLE) +20);
		sprintf (messaggio,"%s \n %s",NOT_EXECUTABLE,NO_BT_LIST);
		attenzione (masterMenu,messaggio);
		free (messaggio);
		return (-1);
		}

	strcpy (flag,"*");
	contatore_sessioni = 0;
	pos = 0;
	for (i=last_backtrack; ;i++)
		{
		if (i >= _MAX_BACK_TRACK) 
			i = 0;
		dati = p+i;
		if (dati->stat == 1)
         {
			pos++;			
			punt_rec_backtrack[pos] = i; /* corrispondenza item lista e file */ 
			sprintf(stringa,"%3d - %s", pos, flag);
			sprintf(stringa,"%s - %s - %s - %f",
            stringa,dati->datasn,dati->temposn, dati->tempo);
			str = (char*)XtMalloc(sizeof(char)*((strlen(stringa))+20));
      	strcpy (str, stringa);
      	c_str = XmStringCreate (str, XmSTRING_DEFAULT_CHARSET);
      	XmListAddItemUnselected (lista, c_str, 0);
			XmStringFree(c_str);
      	XtVaSetValues (lista, XmNitemCount, pos, NULL);
			if (dati->forzato != 0)
				{
				contatore_sessioni++;
				if (!strcmp(flag,"*"))
					strcpy (flag," ");
				else
					strcpy (flag,"*");
				}
			if (i == (last_backtrack-1))
				{
				XmListSetBottomPos(lista,pos);
				break;     /* termine del loop */
				}
			}
		}
	return(0);
}
/**********************************************************/
int display_header_bt (parent,p)
Widget parent;
BKTAB *p;
{
int i;
char stringa[100];
BKTAB *dati;
int pos;
int cambio_sessione;


	numero_step = 0;
	contatore_sessioni = 0;
	cambio_sessione = 1;
	pos = 0;
	numeroBtLista = 0;			/* numero item visualizzati		*/
   for (i=last_backtrack; ;i++)
      {
      if (i >= _MAX_BACK_TRACK)
         i = 0;
      dati = p+i;
      if (dati->stat == 1)
         {
			pos++;
			numero_step++;
			if (cambio_sessione == 1)
				{
				strcpy (stringa,dati->descr);
				crea_cambio_sessione(parent,stringa);
				cambio_sessione = 0;
				}
			punt_rec_bt[pos] = i;/* corrispondenza item lista e file */
			crea_backtrack_entry (parent,pos-1);
			displayDatiRecordBt (dati, pos-1);
			numeroBtLista ++;
/*********
			XtVaSetValues (BtDate[pos-1], XmNvalue, dati->temposn, NULL);
         sprintf (stringa,"%f",dati->tempo);
         XtVaSetValues (BtSimtime[pos-1], XmNvalue, stringa, NULL);
***************/
			if (dati->forzato != 0)
            {
            contatore_sessioni++;
				cambio_sessione=1;
				}
			if (i == (last_backtrack-1))
            break;     /* termine del loop */
			}
		}
   return(0);
}
/**********************************************************/
/*
 * visualizza i dati del record di bt
 */
int displayDatiRecordBt (BKTAB *dati, int indx)
{
char stringa[100];
	sprintf (stringa,"%d",indx+1);
	XtVaSetValues (BtNumber[indx], XmNvalue, stringa, NULL);
	XtVaSetValues (BtDate[indx], XmNvalue, dati->temposn, NULL);
	sprintf (stringa,"%f",dati->tempo);
	XtVaSetValues (BtSimtime[indx], XmNvalue, stringa, NULL);
	return (0);
}
/**********************************************************/
/*
 * aggiorna in linea l' interfaccia BT a seguito di avvenuto 
 *	snapshot di BT
 */
int updateHeaderBt ()
{
_UxCbacktrackManagement *Ctx;
int i;
int pos;

	Ctx = (_UxCbacktrackManagement *)UxGetContext(backtrackManagement);

/* lettura header BT per aggiornamento */
	read_lista_bt (bt_header);
/* controllo se il file non e' ancora completamente usato 
		 il controllo viene effettuato sul record successivo a quello 
 	    ultimo scritto																	*/

	if	((bt_header[last_backtrack].stat != 1) && /* circolarita' non usata */
		 (numeroBtLista < _MAX_BACK_TRACK))       /* se lista incompleta		*/
		{
fprintf (stderr,"updateHeaderBt: nuovo record lastbt %d \n",last_backtrack);
		punt_rec_bt[last_backtrack] = last_backtrack;
		crea_backtrack_entry (Ctx->UxBtRc,last_backtrack-1);	
		displayDatiRecordBt (bt_header+(last_backtrack-1),last_backtrack-1);
		numeroBtLista ++;
		}
	else
		{
/* eliminazione del primo record visualizzato */
		DistruggiInterfaccia (BtScrollForm[0]);	/* elimina la entry 0	*/
/* shift degli oggetti indietro di un passo */
		for (i=1; i<_MAX_BACK_TRACK; i++)
			{
			BtScrollForm[i-1] 	= BtScrollForm[i];
			BtNumber[i-1] 			= BtNumber[i];
			BtDate[i-1] 			= BtDate[i];
			BtSimtime[i-1] 		= BtSimtime[i];
			}
/* si aggiunge l' ultimo elemento della lista */
      crea_backtrack_entry (Ctx->UxBtRc,_MAX_BACK_TRACK-1);  
      displayDatiRecordBt (bt_header+(last_backtrack-1),_MAX_BACK_TRACK-1);
/* rimozione callbacks */
      for (i=0; i<_MAX_BACK_TRACK; i++)
         {
         XtRemoveAllCallbacks (*(BtNumber+i),XmNfocusCallback);
         XtRemoveAllCallbacks (*(BtNumber+i),XmNlosingFocusCallback);
         XtRemoveAllCallbacks (*(BtDate+i),XmNfocusCallback);
         XtRemoveAllCallbacks (*(BtDate+i),XmNlosingFocusCallback);
         XtRemoveAllCallbacks (*(BtSimtime+i),XmNfocusCallback);
         XtRemoveAllCallbacks (*(BtSimtime+i),XmNlosingFocusCallback);
         }
/* ricalcolo puntatori lista/file e associazione callbacks */
		pos = 0; /* indice posizione nella lista					*/
   	for (i=last_backtrack; ;i++)
      	{
      	if (i >= _MAX_BACK_TRACK)
         	i = 0;
			if (bt_header[i].stat == 1)		/* controllo superfluo		*/
				{
				pos++;
				punt_rec_bt[pos] = i;			/* associazione lista/file */
				sprintf (n_entry+(i*4),"%d", pos);
				XtAddCallback (*(BtNumber+pos-1),
								XmNfocusCallback,BtSelActivate,n_entry+i*4);	
				XtAddCallback (*(BtNumber+pos-1),
								XmNlosingFocusCallback,BtSelActivate,n_entry+i*4);

				XtAddCallback (*(BtDate+pos-1),
                        XmNfocusCallback,BtSelActivate,n_entry+i*4);
            XtAddCallback (*(BtDate+pos-1),
                        XmNlosingFocusCallback,BtSelActivate,n_entry+i*4);

				XtAddCallback (*(BtSimtime+pos-1),
                        XmNfocusCallback,BtSelActivate,n_entry+i*4);
            XtAddCallback (*(BtSimtime+pos-1),
                        XmNlosingFocusCallback,BtSelActivate,n_entry+i*4);						
				if (i == (last_backtrack-1))
            	break;     /* termine del loop */
				}
			}
		}
}
/**********************************************************/
int crea_cambio_sessione (parent,testo)
Widget parent;
char *testo;
{
Widget BtChangeSession, BtChangeSessionLabel;
XmString c_str;

	c_str = XmStringCreate (testo, XmSTRING_DEFAULT_CHARSET);

	BtChangeSession = XtVaCreateManagedWidget( "BtChangeSession",
         xmFormWidgetClass, parent,
			XmNwidth,    330,
         NULL );
	BtChangeSessionLabel = XtVaCreateManagedWidget( "BtChangeSessionLabel",
         xmLabelWidgetClass, BtChangeSession,
         XmNleftAttachment, XmATTACH_FORM,
         XmNrightAttachment, XmATTACH_FORM,
         XmNtopAttachment, XmATTACH_FORM,
         XmNbottomAttachment, XmATTACH_FORM,
			XmNbackground,   colore_app[1],
         XmNheight, 30,
         XmNlabelString,  c_str,
         NULL );

	XmStringFree(c_str);
	return(0);
}
/**********************************************************/
int crea_backtrack_entry(parent,i)
Widget parent;
int i; /* indice dei widget */
{

printf ("creazione BtScrollForm indice %d\n",i);
   *(BtScrollForm+i) = XtVaCreateManagedWidget( "BtScrollFormn",
         xmFormWidgetClass, parent,
			XmNwidth,    330,
         NULL );

	sprintf (n_entry+(i*4),"%d", i+1);

   *(BtNumber+i) = XtVaCreateManagedWidget( "BtNumber1",
         xmTextWidgetClass, *(BtScrollForm+i),
         XmNleftAttachment, XmATTACH_FORM,
         XmNtopAttachment, XmATTACH_FORM,
         XmNleftOffset, 0,
         XmNbottomAttachment, XmATTACH_FORM,
         XmNheight, 30,
         XmNwidth, 50,
         XmNy, 0,
         XmNx, 5,
			XmNvalue,	n_entry+i*4,
			XmNeditable     ,    False,
			XmNcursorPositionVisible, False,
         NULL );


   *(BtDate+i) = XtVaCreateManagedWidget( "BtDate1",
         xmTextWidgetClass, *(BtScrollForm+i),
         XmNtopAttachment, XmATTACH_FORM,
         XmNleftWidget, *(BtNumber+i),
         XmNleftAttachment, XmATTACH_WIDGET,
         XmNbottomAttachment, XmATTACH_FORM,
         XmNheight, 30,
         XmNwidth, 80,
			XmNeditable     ,    False,
			XmNcursorPositionVisible, False,
         NULL );


   *(BtSimtime+i) = XtVaCreateManagedWidget( "BtSimtime1",
         xmTextWidgetClass, *(BtScrollForm+i),
         XmNresizeWidth, TRUE,
         XmNmarginWidth, 5,
         XmNmarginHeight, 5,
         XmNhighlightThickness, 2,
         XmNtopAttachment, XmATTACH_FORM,
         XmNleftWidget, *(BtDate+i),
         XmNleftAttachment, XmATTACH_WIDGET,
         XmNbottomAttachment, XmATTACH_FORM,
         XmNheight, 30,
         XmNwidth, 200,
			XmNeditable     ,    False,
			XmNcursorPositionVisible, False,
         NULL );

	XtAddCallback (*(BtNumber+i),XmNfocusCallback,BtSelActivate,n_entry+i*4);
	XtAddCallback (*(BtNumber+i),XmNlosingFocusCallback,BtSelActivate,n_entry+i*4);

	XtAddCallback (*(BtDate+i),XmNfocusCallback,BtSelActivate,n_entry+i*4);
	XtAddCallback (*(BtDate+i),XmNlosingFocusCallback,BtSelActivate,n_entry+i*4);

	XtAddCallback (*(BtSimtime+i),XmNfocusCallback,BtSelActivate,n_entry+i*4);
	XtAddCallback (*(BtSimtime+i),XmNlosingFocusCallback,BtSelActivate,n_entry+i*4);

	return(0);
}
/******************************************************************/
int set_colore_riga_bt (n, c)
int n;   /* numero di riga */
int c;   /* colore         */
{
printf ("set colore riga %d\n", n);


	XtVaSetValues (*(BtNumber+n), XmNbackground, c, NULL);
   XtVaSetValues (*(BtDate+n), XmNbackground, c, NULL);
   XtVaSetValues (*(BtSimtime+n), XmNbackground, c, NULL);
   return(0);
}
/******************************************************************/
int set_bt_sensitivity (n,w1,w2)
int n;   /* numero di bt selezionato */
Widget w1,w2;
{
extern BKTAB *bt_header;

   if ((bt_header+n)->stat == 1) /* snap occupato  */
      {
      XtVaSetValues (w1, XmNsensitive, True, NULL);
      XtVaSetValues (w2, XmNsensitive, True, NULL);
      }
   else
      {
      /* printf ("record %d non occupato\n",n); */
      XtVaSetValues (w1, XmNsensitive, False, NULL);
      XtVaSetValues (w2, XmNsensitive, False, NULL);
      }
   return(0);
}
/******************************************************************/
void uscita_backtrack (w)
Widget w;
{
extern int backtrack_caricato;
extern BKTAB *bt_header;
extern SNTAB *snap_header;
/*
 * uscita per annullamento backtrack
 */
set_cursor (w, CLOCK);
if (backtrack_caricato)
   {
   if (!ricarica_default_ci(w))  /* ha caricato la ci default */
      {
/*** azioni eseguite da destroyCallback
      bt_interface_active = 0;
      free (snap_header);
      free (bt_header);
***************************************/
		backtrack_caricato = 0;
		inizializzazione = 0;
      DistruggiInterfaccia (XtParent(w));
		set_cursor (w, NORMALE);
   	return;
      }
	else
		{
		set_cursor (w, NORMALE);
   	return;
		}
   }
else
   {
   DistruggiInterfaccia (XtParent(w));
/*** azioni eseguite da destroyCallback
   bt_interface_active = 0;
   free (snap_header);
   free (bt_header);
***************************************/
	set_cursor (w, NORMALE);
   return;
   }
}
/**************************************************************/
int richiesta_timer_bt (w)
Widget w;
{
char risp [10];
char old_val[10];
float new_val;
char *messaggio;
int esito;

#ifndef OLD_STAT
	sprintf (old_val,"%4.1f",statistiche.stato_parametri.passo_reg_bt);
#else
	sprintf (old_val,"%4.1f",val.actual.passo_reg_bt);
#endif
	create_richiestaDati (w, DELTAT_REC_BT, old_val, risp);
	new_val = (float)atof(risp);
	printf ("risp = %s: %f\n",risp,new_val);

	esito = 0;
	messaggio = (char *)malloc (strlen(NEW_DELTAT_BT)+strlen(OPER_FALLITA)+20);

	if (SD_btrecstep(BANCO, &new_val) >0)
      sprintf (messaggio,"%s :%4.1f",NEW_DELTAT_BT,new_val);
   else
      {
      sprintf (messaggio,"%s :%4.1f %s",NEW_DELTAT_BT,new_val,OPER_FALLITA);
      esito = -1;
      attenzione(w,messaggio);
      }

   add_message (areaMessaggi,messaggio,LIVELLO_1);
   free (messaggio);
   return(esito);
}
/**************************************************************/
int btload_ok (w)
Widget w;
{
char *messaggio;
int esito;

   esito = 0;
   messaggio = (char *)malloc (strlen(VALIDATE_BT_OK)+strlen(OPER_FALLITA)+20);

   if (SD_validatebt(BANCO) >0)
      sprintf (messaggio,"%s",VALIDATE_BT_OK);
   else
      {
      sprintf (messaggio,"%s",VALIDATE_BT_OK,OPER_FALLITA);
      esito = -1;
      attenzione(w,messaggio);
      }

   add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);
   return(esito);
}
/********************************************************************/
int bt_indietro()
{
int stato;
printf ("direzione indietro\n");
   direzione_bt=BT_REVERSE;
   prossimo_step();
   return(0);
}
/********************************************************************/
int bt_avanti()
{
int stato;
printf ("direzione avanti\n");
   direzione_bt=BT_FORWARD;
   prossimo_step();
   return(0);
}
/********************************************************************/
int bt_in_manuale()
{
int stato;
   modo_bt=BT_MANUAL;
   return(0);
}
/********************************************************************/
int bt_in_automatico()
{
int stato;
   modo_bt=BT_AUTO;
   return(0);
}
/********************************************************************/
int prossimo_step()
{
   if (numero_step == 1)
         return(0);           /* un solo step visualizzato */

   switch (direzione_bt)
         {
         case BT_REVERSE:
            if (bt_sel == 1)
               bt_avanti();
            else
               bt_sel--;
            break;

         case BT_FORWARD:
            if (bt_sel == numero_step)
               bt_indietro();
            else
               bt_sel++;

            break;
         }
     return(0);
}
/********************************************************************/
int replay_mode (w, stato)
Widget w;                   /* chiamante                            */
Boolean stato;              /* set del tasto                        */
{
char *messaggio;
int esito;
int n;
int old_n;
int first;

   first = 1;

	esito = 0;
	messaggio = (char *)malloc (strlen(REPLAY_ACTIVE)+
									    strlen(REPLAY_STOP)+
										 strlen(OPER_FALLITA) + 20);
	switch (stato)
		{
		case False:
			if (SD_backtrack (BANCO) > 0)
				{
				sprintf (messaggio,"%s",REPLAY_STOP);
				stop_timer (timer_replayOn);
				mess_replay (w,False);
				tasti_replay (w,False);
				}
         else
            {
				sprintf (messaggio,"%s \n %s",REPLAY_STOP,OPER_FALLITA);
				esito = -1;
            attenzione (w,messaggio);
            }
			break;

		case True:
			if (SD_replay (BANCO) > 0)
				{
				sprintf (messaggio,"%s",REPLAY_ACTIVE);
				reload_lastBtLoad (w,&n,&old_n,first);
				mess_replay (w,True);
				attiva_timer_replayOn (w);
				tasti_replay (w,True);
				}
			else
				{
				sprintf (messaggio,"%s \n %s",REPLAY_ACTIVE,OPER_FALLITA);
				esito = -1;
				attenzione (w,messaggio);
				}
			break;

		default:
			break;
		
		}
	add_message (areaMessaggi, messaggio, LIVELLO_1);
	free (messaggio);
	return (esito);
}
/********************************************************************/
int abilita_replay_mode (w)
Widget w;               /* chiamante                                */
{
_UxCbacktrackManagement *Ctx;

	Ctx = (_UxCbacktrackManagement *)UxGetContext(w);

	XtVaSetValues (Ctx->UxreplaySelectionbt,XmNsensitive,True,NULL);

	return (0);
}
/********************************************************************/
/*
 * il simulatore e' tornato in BT alla fine della sequenza
 * di replay
 */
int reset_replay_mode (w)
Widget w;
{
_UxCbacktrackManagement *Ctx;

   Ctx = (_UxCbacktrackManagement *)UxGetContext(w);
	XmToggleButtonSetState (Ctx->UxreplaySelectionbt,False,True);
	stop_timer (timer_replayOn);
	mess_replay (w,False);

	return(0);
}
/********************************************************************/
int update_btload (w,n)
Widget w;               /* chiamante                                */
int n;                  /* record caricato                          */
{
char str[10];

_UxCbacktrackManagement *Ctx;

   Ctx = (_UxCbacktrackManagement *)UxGetContext(w);
	sprintf (str,"%d",n);
	XtVaSetValues (Ctx->UxbtLoadNumber,RES_CONVERT( XmNlabelString,str),NULL);
	return (0);
}
/********************************************************************/
int mess_replay (w, stato)
Widget w;
Boolean stato;
{
_UxCbacktrackManagement *Ctx;

   Ctx = (_UxCbacktrackManagement *)UxGetContext(w);
	switch (stato)
		{
		case True:
			XtVaSetValues (Ctx->UxreplaySelectionLabel,
										RES_CONVERT( XmNlabelString,REPLAY_ACTIVE),
										XmNbackground,colore_app[1],NULL);
			break;
		case False:
			XtVaSetValues (Ctx->UxreplaySelectionLabel,
										RES_CONVERT( XmNlabelString,REPLAY_STOP),
										XmNbackground,background_backtrack,NULL);
			break;
		}
	return (0);
}
/********************************************************************/
int reload_lastBtLoad (w, n, old, flag)
Widget w;               /* chiamante                                */
int *n;                 /* record caricato                          */
int *old;               /* precedente record caricato               */
int flag;               /* =1 primo giro , altrimenti continuazione */
{
static int old_BtLoad = -1;
int BtLoad;
int changed = -1;
	
	BtLoad = lastBtLoad (w);
	*n = BtLoad;

	if (flag == 1)
		old_BtLoad = -1;

	if (old_BtLoad < 0) /* primi giri */
		old_BtLoad = bt_sel;

	*old = old_BtLoad;

	if (BtLoad != old_BtLoad)
		{
		old_BtLoad = BtLoad;
		bt_sel = BtLoad;
		changed = 1;
		}
	return (changed);
}
/********************************************************************/
int lastBtLoad (w)
Widget w;
{
	return (SD_lbtload (BANCO));
}
/********************************************************************/
int refresh_colors_bt (w, n, old)
Widget w;            /* chiamante                                */
int n;               /* record caricato                          */
int old;             /* precedente record caricato               */
{

	if ((old < 0 )  ||  /*  primo giro     */
		 (old == n))    /* nessuna variazione  */
		return (0);
printf ("refresh colori %d e %d \n",n,old);
	set_colore_riga_bt (old-1, colore_app[3]);
	set_colore_riga_bt (n-1,colore_app[0]);
	return (0);
}
/********************************************************************/
int tasti_replay (w, stato_replay)
Widget w;         /* chiamante                          */
Boolean stato_replay;
{
_UxCbacktrackManagement *Ctx;

   Ctx = (_UxCbacktrackManagement *)UxGetContext(w);

	switch (stato_replay)
		{
		case True:
			XtVaSetValues (Ctx->UxbtLoadpb,XmNsensitive,False,NULL);
			XtVaSetValues (Ctx->UxBtRcPopupLoad,XmNsensitive,False,NULL);
			XtVaSetValues (Ctx->UxbtInitpb,XmNsensitive,False,NULL);
			XtVaSetValues (Ctx->UxBtRcPopupInit,XmNsensitive,False,NULL);
			break;
		case False:
			XtVaSetValues (Ctx->UxbtLoadpb,XmNsensitive,True,NULL);
			XtVaSetValues (Ctx->UxBtRcPopupLoad,XmNsensitive,True,NULL);
			XtVaSetValues (Ctx->UxbtInitpb,XmNsensitive,True,NULL);
			XtVaSetValues (Ctx->UxBtRcPopupInit,XmNsensitive,True,NULL);
			break;
		}
	return (0);
}
