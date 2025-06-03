/**********************************************************************
*
*       C Source:               comuni.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Nov  8 17:49:25 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: comuni.c-11 %  (%full_filespec: comuni.c-11:csrc:1 %)";
#endif

/*
 * comuni.c
 *				funzioni comuni all' applicativo 
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <X11/Xlib.h>
#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>
#include <Xm/SeparatoG.h>
#include <Xm/Separator.h>
#include <Xm/Xm.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include "UxXt.h"

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "libdispatcher.h"

#include "sked.h"

#include "parametri.h"
#include "messaggi.h"
#include "master_monit.h"
#include "bistrutt.h"

#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"

#include "cursore.h"

static int last_message=0;
extern Widget areaMessaggi;
extern Widget masterMenu;
extern int inizializzazione;
extern swidget create_sessionName();
extern int stato_sim;
extern int last_backtrack;
extern int prec_sel;   /* memorizza il record BT precedentemente selezionato */
extern float intervallo_backtrack;
extern RICHIESTA_STAT richiesta_stat;
extern VARIABILI *variabili;
extern int tot_variabili;

extern int nuovoSnapCaricato;
extern int nuovoSnapCaricato2;

extern char *FILES_PATH;

#ifndef OLD_STAT
extern STATISTICHE statistiche;   /* ricezione statistiche da sked */
#else
/*
 * provvisoriamente si usa la vecchia struttura statistiche
 */
extern STATISTICA_SKED statistiche;
#endif
char *get_pwd();
char *read_sim_name();
int getCampoString();
int     SD_clear (int);
int update_sessionName (Widget);
int not_executable (Widget);
int SD_sessione (int, char*);
int update_titolo_sessione (Widget);



/********************************************************************/
int init_message_list()
{
	add_message (areaMessaggi,"INIZIALIZZAZIONE",0);
	return(0);
}
/********************************************************************/
int add_item_list (w,s)
Widget w;
char *s;
{
char *str;
XmString c_str;

	str = (char*)XtMalloc(sizeof(char)*(strlen(s)+20));
	strcpy (str,s);
	c_str = XmStringCreate (str, XmSTRING_DEFAULT_CHARSET);
	XmListAddItemUnselected (w, c_str, 0);
	XmStringFree(c_str);
	XtFree (str);
	return (0);
}
/********************************************************************/
int add_message (lista, s, l)
Widget lista;
char *s;			/* messaggio    */
int l;         /* livello      */
{
char *str;
XmString c_str;
static char path[400];
static int primo_giro = 1;
FILE *fp;
char *app;
char tempo_c[20];

#ifndef OLD_STAT
	sprintf (tempo_c,"%8.1f",statistiche.stato_tempo.tempo_sim);
#else
	sprintf (tempo_c,"%8.1f",statistiche.tempo_sim);
#endif
/*
 * codifica i messaggi per livelli 
 * li scrive su file
 * provvedere alla gestione filtrata sulla lista
 */
/*
 * aggiorna la lista
 */
		if (exist_Widget(lista))
			{
			last_message++;
      	str = (char*)XtMalloc(sizeof(char)*((strlen(s)+strlen(tempo_c))+20));
			strcpy (str,tempo_c);
			strcat (str," : ");
      	strcat (str, s);
      	c_str = XmStringCreate (str, XmSTRING_DEFAULT_CHARSET);
      	XmListAddItemUnselected (lista, c_str, 0);
      	XtVaSetValues (lista, XmNitemCount, last_message, NULL);  
      	XmListSetBottomPos(lista,last_message);  
      	XmStringFree(c_str);
        if(str)
           XtFree(str);
			}

/*
 * aggiorna il file 
 */
		if (primo_giro)
			{
			strcpy (path, FILES_PATH);
   		strcat (path,"/");
   		strcat (path,MESS_FILE);
			primo_giro = 0;
			fp = fopen (path,"w");
			}
		else
			fp = fopen (path,"a");
			
		app = (char *)malloc (strlen(tempo_c) + strlen(s) + sizeof(int) +10);
		sprintf (app,"%d%s : %s\n",l,tempo_c,s);
		fwrite (app,strlen(app)+1, 1, fp);
		free(app);
		fclose(fp);
		

		return (0);
}
/****************************************************************/
void Popup_Menu (w, dati, event)
Widget w;
XtPointer dati;
XButtonEvent *event;
{
extern Widget areaMessaggiPopupMenu;
Widget menu = (Widget)dati;

	if (event->button != Button3)
		return;

	XmMenuPosition (menu, event);
	XtManageChild (menu);
	
}
/****************************************************************/
int clear_message_list (lista)
Widget lista;
{
	XmListDeleteAllItems (lista);
	last_message = 0;
}
/****************************************************************/
int exist_Widget (w)
Widget w;
{
int ret = 0;

	if (w == (Widget)NULL)
		return (ret);

	if ( XtIsRealized (w))
		{
//		printf ("widget realized\n");
		if ( XtIsManaged (w))
			{
//			printf ("widget managed\n");
			ret = 1;
			}
		}
	return (ret);
}
/****************************************************************/
int DistruggiInterfaccia (Widget w)
{
	XtDestroyWidget (w);
	XSync(XtDisplay(w),False);
	return(0);
}
/******************************************************************/
int recording_reset(w)
Widget w;
{
char *messaggio;
int esito;

   esito = 0;
   messaggio = (char *)malloc (strlen(CLEAR_SIM)+strlen(OPER_FALLITA) + 20);
	sprintf (messaggio,"%s",CLEAR_SIM);
	if (!create_attenzioneDialog (w, messaggio,DIALOG_QUESTION))
      {
      free (messaggio);
      return(-1);
      }

   if (SD_clear(BANCO) > 0)
      {
      sprintf (messaggio,"%s",CLEAR_SIM);
		last_backtrack = 0;
		intervallo_backtrack = -1.;  /* registra bt al prossimo step   */
		prec_sel = 0;						/* clear del puntatore precedente */
      }
   else
      {
      sprintf (messaggio,"%s %s",CLEAR_SIM,OPER_FALLITA);
      attenzione (masterMenu,messaggio);
      esito = -1;
      }
   add_message (areaMessaggi,messaggio,LIVELLO_1);
   free (messaggio);

   return (esito);
}
/******************************************************************/
int simulatorShutdown (Widget w)
{
char *messaggio;
int esito;

   esito = 0;
   messaggio = (char *)malloc (strlen(SHUTDOWN_SIM)+strlen(OPER_FALLITA) + 20);
   sprintf (messaggio,"%s",SHUTDOWN_SIM);
   if (!create_attenzioneDialog (w, messaggio,DIALOG_QUESTION))
      {
      free (messaggio);
      return(-1);
      }
	SD_stop (BANCO);
	system ("killsim");
	exit (0);
}
/******************************************************************/
int run_sim()
{
char *messaggio;
int esito;

   esito = 0;
   messaggio = (char *)malloc (strlen(RUN_SIM)+strlen(OPER_FALLITA) + 20);
   if (SD_run(BANCO) > 0)
		{
      sprintf (messaggio,"%s",RUN_SIM);
/*		stato_sim = STATO_RUN;   */
		}
   else
      {
      sprintf (messaggio,"%s %s",RUN_SIM,OPER_FALLITA);
		attenzione (masterMenu,messaggio);
      esito = -1;
      }
   add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);

   return (esito);
}
/******************************************************************/
int freeze_sim()
{
char *messaggio;
int esito;
 
	esito = 0;
	messaggio = (char *)malloc (strlen(FREEZE_SIM)+strlen(OPER_FALLITA) + 20);
	if (SD_freeze(BANCO) > 0)
		{
		sprintf (messaggio,"%s",FREEZE_SIM);
/*		stato_sim = STATO_FREEZE;     */
		}
	else
		{
		sprintf (messaggio,"%s %s",FREEZE_SIM,OPER_FALLITA);
		attenzione (masterMenu,messaggio);
		esito = -1;
		}
	add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);

	return (esito);
}
/******************************************************************/
int backtrack_sim()
{
char *messaggio;
int esito;

   esito = 0;
   messaggio = (char *)malloc (strlen(BACKTRACK_SIM)+strlen(OPER_FALLITA) + 20);
   if (SD_backtrack(BANCO) > 0)
		{
      sprintf (messaggio,"%s",BACKTRACK_SIM);
/*		stato_sim = STATO_BACKTRACK;    */
		}
   else
      {
      sprintf (messaggio,"%s %s",BACKTRACK_SIM,OPER_FALLITA);
      esito = -1;
		attenzione (masterMenu,messaggio);
      }
   add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);
   return (esito);
}
/******************************************************************/
int step_sim()
{
char *messaggio;
int esito;

   esito = 0;
   messaggio = (char *)malloc (strlen(STEP_SIM)+strlen(OPER_FALLITA) + 20);
   if (SD_goup(BANCO) > 0)
      {
      sprintf (messaggio,"%s",STEP_SIM);
		nuovoSnapCaricato = 0;		/* reset flag di caricamento avvenuto */
		nuovoSnapCaricato2 = 0;		/* reset flag di caricamento avvenuto */
      }
   else
      {
      sprintf (messaggio,"%s %s",STEP_SIM,OPER_FALLITA);
      esito = -1;
		attenzione (masterMenu,messaggio);
      }
   add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);
   return (esito);

}
/******************************************************************/
int switch_to_run(w)
Widget w;
{
swidget sn;

	switch (stato_sim)
		{
		case STATO_RUN:
			break;
		case STATO_FREEZE:
			if (inizializzazione)
   			{
   			sn = create_sessionName(titolo_sessione);
   			printf ("titolo sessione = :%s:\n",titolo_sessione);
   			inizializzazione = 0;
   			UxDestroyInterface (sn);
				update_sessionName(w);
   			}
			run_sim();
			break;
		default:
			not_executable (w);
         return (-1);
		}
	nuovoSnapCaricato = 0;		/* reset flag di caricamento avvenuto */
	nuovoSnapCaricato2 = 0;		/* reset flag di caricamento avvenuto */
	return (0);
}
/******************************************************************/
int stop_simulatore (w)
Widget w;
{
	system ("killsim");
}
/******************************************************************/
int simulator_error (w)
Widget w;
{
char *messaggio;

   messaggio = (char *)malloc (strlen(ERROR_SIM) + strlen(REINIT) + 10);
   sprintf (messaggio,"%s \n\n %s",ERROR_SIM,REINIT);
   create_attenzioneDialog(w,messaggio,DIALOG_ERROR);
   free(messaggio);
   return (0);
}
/******************************************************************/
int not_executable (w)
Widget w;
{
char *messaggio;

	messaggio = (char *)malloc (strlen(NOT_EXECUTABLE) + strlen(BAD_STATE) + 10);
   sprintf (messaggio,"%s \n\n %s",NOT_EXECUTABLE,BAD_STATE);
   create_attenzioneDialog(w,messaggio,DIALOG_WARNING);
   free(messaggio);
	return (0);
}
/******************************************************************/
int not_initialized (w,s)
Widget w;
char *s;
{
char *messaggio;

   messaggio = (char *)malloc (strlen(ERRORE_INIT)+
							strlen(s)+strlen(ABORTING)+100);
   sprintf (messaggio,"%s\n%s\n\n%s",s,ERRORE_INIT,ABORTING);  
   create_attenzioneDialog(w,messaggio,DIALOG_ERROR);
   free(messaggio);
   return (0);
}
/******************************************************************/
int attenzione (w,s)
Widget w;
char *s;
{
	create_attenzioneDialog(w,s,DIALOG_WARNING);
	return (0);
}
/******************************************************************/
int switch_to_freeze(w)
Widget w;
{
/*
fprintf (stderr,"switch_to_freeze: att. %d\n",stato_sim);
*/
   switch (stato_sim)
      {
      case STATO_RUN:
			freeze_sim();
         break;
/**** 
	Forzo in ogni caso lo stato di freeze
   per il caso della sequenza load bt + init 
   loadbt deve portare in bt, mentre init porta in freeze
   se le statistiche non arrivano in tempo con lo stato bt
   la successiva commutazione a freeze non ha luogo e il simulatore
	rimane in bt.
**/
      case STATO_FREEZE:
			freeze_sim(); 
         break;
		case STATO_BACKTRACK: /* uscita per validazione database */
			freeze_sim();
         break;
      default:
			not_executable (w);
         return (-1);
      }
   return (0);
}
/******************************************************************/
int execute_step (w)
Widget w;
{

	switch (stato_sim)
		{
		case STATO_FREEZE:
			step_sim();
			break;
		default:
			not_executable (w);
         return (-1);
		}
	return (0);
}
/******************************************************************/
int execute_freeze_button (w)
Widget w;
{

	switch (stato_sim)
		{
		case STATO_FREEZE:
			execute_step (w);
			break;
		case STATO_RUN:
			switch_to_freeze (w);
			break;
		default:
			not_executable (w);
         return (-1);
		}
	return (0);
}
/******************************************************************/
int update_sessionName (w)
Widget w;
{
char *messaggio;
int esito;

   esito = 0;
   messaggio=(char *)malloc(strlen(NEW_SESSION_NAME)+
						strlen(titolo_sessione)+strlen(OPER_FALLITA)+20);
   if (SD_sessione(BANCO,titolo_sessione) > 0)
      {
      sprintf (messaggio,"%s:%s",NEW_SESSION_NAME,titolo_sessione);
		update_titolo_sessione (w);
      }
   else
      {
      sprintf (messaggio,"%s:%s %s",
                 NEW_SESSION_NAME,titolo_sessione,OPER_FALLITA);
      esito = -1;
      attenzione (masterMenu,messaggio);
      }
   add_message (areaMessaggi,messaggio,LIVELLO_1);
   free (messaggio);
   return (esito);

}
/******************************************************************/
int update_titolo_sessione (w)
Widget w;
{
char *s;

	s = (char *)malloc (strlen(titolo_sessione) + strlen(init_from) + 10);
	strcpy (s, titolo_sessione);
	strcat (s," : ");
	strcat (s, init_from);
	XtVaSetValues (XtParent(w), XmNtitle, s, XmNiconName, s, NULL);     
	free(s);
	return (0);
}
/******************************************************************/
int update_title (w,s)
Widget w;
char *s;
{
   XtVaSetValues (XtParent(w), XmNtitle, s, XmNiconName, s, NULL);
   return (0);
}
/******************************************************************/
/*
 * ritorna l' indirizzo in memoria
 */
int get_label_addr (s,ind_st,ind_end)
char *s;              /* label da cercare */
int  ind_st;
int  ind_end;
{
int i;
/*
printf("get_label_addr nome_var = %s\n",s);
*/
   for (i=ind_st; i<ind_end; i++)
      {
      if ((variabili[i].nome[0] == s[0]) && (variabili[i].nome[1] == s[1]))
         if (!strcmp(variabili[i].nome,s)) {
/*
            printf("get_label_addr variabile %s trovata!!!\n",s);
*/
            return (variabili[i].addr);
         }
      }
   return (-1);
}
/******************************************************************/
/*
 * ritorna l' indirizzo in memoria
 */
int get_label_addr_ind(s,ind_in_var,ind_st,ind_end)
char *s;              /* label da cercare */
int  *ind_in_var;
int  ind_st;
int  ind_end;
{
int i;
/*
printf("get_label_addr_ind nome_var = %s\n",s);
*/
   for (i=ind_st; i<ind_end; i++)
      {
      if ((variabili[i].nome[0] == s[0]) && (variabili[i].nome[1] == s[1]))
         if (!strcmp(variabili[i].nome,s)) {
/*
            printf("get_label_addr_ind variabile %s trovata!!!\n",s);
*/
            *ind_in_var = i;
            return (variabili[i].addr);
         }
      }
      *ind_in_var = -1;
   return (-1);
}
/******************************************************************/
/*
 * ritorna l' indice in variabili.rtf
 */
int get_label_indx (s)
char *s;              /* label da cercare */
{
int i;

   for (i=0; i<tot_variabili; i++)
      {
      if ((variabili[i].nome[0] == s[0]) && (variabili[i].nome[1] == s[1]))
         if (!strcmp(variabili[i].nome,s))
            return (i);
      }
   return (-1);	
}
/******************************************************************/
/*
 * ritorna l' indice in variabili.rtf dato l' indirizzo in memoria
 */
int get_addr_indx (int addr)
{
int i;

   for (i=0; i<tot_variabili; i++)
      {
      if (addr == variabili[i].addr)
         return (i);
      }
   return (-1);
}
/******************************************************************/
/*
 * Ritorna la stringa corrispondente alla compound string
 * Alloca internamente la stringa. Al chiamante liberare la memoria.
 */
char *XmStringToChar (xmstring)
XmString xmstring;
{
  XmStringContext context;
  XmStringCharSet charset;
  XmStringDirection direction;
  Boolean separator;
  static char *primitive_string;

  XmStringInitContext (&context,xmstring);
  XmStringGetNextSegment (context,&primitive_string,
                          &charset,&direction,&separator);
  XmStringFreeContext (context);
  primitive_string = (char *)malloc (strlen(primitive_string)+1);
  return ((char *) primitive_string);
}
/**********************************************************/
char *get_pwd()
{
char path[MAX_PATH_LEN];
char *punt;

	if ((punt = (char *)getcwd(NULL, MAX_PATH_LEN)) != 0)
		return (punt);
	return ("BAD DIRECTORY");
}
/**********************************************************/
char *read_sim_name (p)
char *p;               /* directory di ricerca         */
{
char filename[]="S01";
char path[MAX_PATH_LEN];
FILE *fp;
char riga[MAX_PATH_LEN];
// GUAG2025 ggiunto static
static char riga2[MAX_PATH_LEN];
int i,k;
int inizio = 1;
int dopo_nome = 1;

	sprintf (path,"%s/%s",p,filename);
	printf ("path = :%s: \n",path);

	if ((fp = fopen (path,"r")) == NULL)
      {	
		return ("NO_FILE");
		}
	fgets (riga,MAX_PATH_LEN,fp);      /* lettura dummy       */
	fgets (riga,MAX_PATH_LEN,fp);      /* nome simulatore + commenti */
	
	fclose (fp);
/*
 * riduce gli spazi 
 */
	for (i=0,k=0; riga[i] != '\0' ;i++)
		{
		if ((riga[i] == ' ') && (inizio == 1)) /* taglia spazi iniziali */
			{
			inizio = 0;
			continue;	
			}
		if (riga[i] != ' ')
			{
			riga2[k] = riga[i];
			k++;
			continue;
			}
		if (riga[i] == ' ')       /* per chiarezza */
			{
			if (dopo_nome == 1)
				{
				riga2[k] = '-';
				k++;
				dopo_nome = 0;
				continue;
				}	
			continue;            /* sostituisce solo il primo blank  */
			}
		}
	riga2[k] =  '\0';
	return (riga2);	
}
/********************************************************************/
int clear_lista (w)
Widget w;
{
int totale;
int k;

   XtVaGetValues (w,XmNitemCount,&totale,NULL);
   if (totale)
      for (k = 1; k <= totale; k++, XmListDeletePos(w, 1));

   return (0);
}
/********************************************************************/
/*
 * mette in campo la parte di r a partire da *next fino al prossimo d
 */
int getCampoStringa (char *r, char *f, int *next, char *d)
{
int i;
int k;

	for (i=0,k=*next; ;i++,k++)
		{
		f[i] = r[k];
		if (f[i] == d[0])
			{
			f[i] = '\0';
			*next = k+1;
			return (1);
			}
		if (f[i] == '\0')
			{
			*next = -1;
			return (1);
			}
		}
}
/**********************************************************/
/*
 * legge un campo Text e restituisce la stringa relativa ad
 * un valore numerico (int o float), pulito di eventuali blank
 * Check del numero di punti e di caratteri non digit.
 * Ritorna: 0 se OK
 *          1 se errore
 */
int read_txt(w,s)
Widget w;
char *s;    /* testo letto    */
{
char *stringa;
char *stringa2;
int npunti;
int i,k;

   stringa = XmTextGetString (w);
   strcpy (s,stringa);

   stringa2 = (char *)malloc(strlen(stringa)+1);

   if (strlen(stringa) <= 0)
      {
      XtFree(stringa);
      free (stringa2);
      return(1);
      }
/*
 * toglie i blank
 */
   for (i=0,k=0; i<strlen(stringa); i++)
      {
      if (stringa[i] != ' ')
         {
         stringa2[k] = stringa[i];
         k++;
         }
      }
   stringa2[k] = '\0';

printf (" letto :%s:  :%s:\n",s,stringa2);
   npunti = 0;
   for (i=0; i<strlen(stringa2); i++)
      {
      if (stringa2[i] == '.')
         npunti++;
      if (((!isdigit(stringa2[i])) && (stringa2[i] != '.')) ||
         (npunti > 1))
         {
         XtFree(stringa);
         free (stringa2);
         return(1);
         }
      }
   XtFree (stringa);
	strcpy (s,stringa2);
   free (stringa2);
   return(0);
}
/**********************************************************/
/*
 * verifica se la stringa sorg contiene un numero int o float
 *		e ritorna in dest la stringa pulita da blank
 *	ritorna 0 se ok
 *	ritorna 1 se non corretto
 */
int validaStrNumber (char *sorg, char *dest)
{
char *s;
int i,k, npunti;

	if (strlen(sorg) <= 0) /* stringa nulla */
		return (1);
	s = (char *)malloc(strlen(sorg)+1);
/*
 * toglie i blank
 */
   for (i=0,k=0; i<strlen(sorg); i++)
      {
      if (sorg[i] != ' ')
         {
         s[k] = sorg[i];
         k++;
         }
      }
   s[k] = '\0';
/*
 * controllo punti e caratteri
 */
   npunti = 0;
   for (i=0; i<strlen(s); i++)
      {
      if (s[i] == '.')
         npunti++;
      if (((!isdigit(s[i])) && (s[i] != '.')) ||
         (npunti > 1))
         {
			free (s);
         return(1);
         }
      }	
/*
 * tutto ok
 */
	strcpy (dest,s);
	free (s);
	return (0);
}
/**********************************************************/
/*
 * scrive il testo s nel campo Text w
 */
int write_txt (w,s)
Widget w;
char *s;
{
   XmTextSetString (w,s);
}
/**********************************************************/
int existFile (char *n)
{
struct stat buffer;

	if (stat(n, &buffer) < 0)		/* non esiste */
		return (0);
	return (1);
}
/******************************************************************/
/*
 * ritorna il codice numerico del tipo di pert indicato da s
 */
int assegnaTipoPert (char *s)
{

   if (!strcmp(s,"PERT_NOT"))
      return (PERT_NOT);

   if (!strcmp(s,"PERT_SCALINO"))
      return (PERT_SCALINO);

   if (!strcmp(s,"PERT_IMPULSO"))
      return (PERT_IMPULSO);

   if (!strcmp(s,"PERT_RAMPA"))
      return (PERT_RAMPA);

   if (!strcmp(s,"PERT_RAMPA_STOP"))
      return (PERT_RAMPA_STOP);

   if (!strcmp(s,"PERT_IMPULSO_STOP"))
      return (PERT_IMPULSO_STOP);

   if (!strcmp(s,"PERT_UP"))
      return (PERT_UP);

   if (!strcmp(s,"PERT_DOWN"))
      return (PERT_DOWN);

   if (!strcmp(s,"PERT_TO"))
      return (PERT_TO);

   if (!strcmp(s,"PERT_TO_STOP"))
      return (PERT_TO_STOP);

   if (!strcmp(s,"PERT_PERIODIC"))
      return (PERT_PERIODIC);

   if (!strcmp(s,"PERT_PERIODIC_STOP"))
      return ( PERT_PERIODIC_STOP);

   if (!strcmp(s,"PERT_WHITENOISE"))
      return (PERT_WHITENOISE);

   if (!strcmp(s,"PERT_WHITENOISE_STOP"))
      return (PERT_WHITENOISE_STOP);

   if (!strcmp(s,"PERT_UP_REPLAY"))
      return (PERT_UP_REPLAY);

   if (!strcmp(s,"PERT_MALFUNCTION"))
      return (PERT_MALFUNCTION);

   if (!strcmp(s,"PERT_MALFUNCTION_STOP"))
      return (PERT_MALFUNCTION_STOP);

   return (-1);
}
/**************************************************/
int stampa (char *f, int modo)
{
char comando[200];

	sprintf (comando,"lpr %s",f);
	system (comando);

	return (0);
}
