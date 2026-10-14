/**********************************************************************
*
*       C Source:               file_select.c
*       Subsystem:              1
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed May 21 17:36:41 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: file_select.c-4 %  (%full_filespec: file_select.c-4:csrc:1 %)";
#endif

/*
 * file_select.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
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
#include "messaggi.h"
#include "bistrutt.h"
#include "cursore.h"

#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"
#include "selVarCr.h"
#include "cont_rec.h"
#include "file_selection.h"
#include "files_riservati.h"

char *full_path();

char *fileSelectionCb();

int check_file_select (Widget,int, int, char*);



extern Widget areaMessaggi;
extern int modello_selezionato;
extern int blocco_selezionato;
/**********************************************************/
char *fileSelectionCb (w, cbs, modo, app)
Widget w;      /* interfaccia fileSelection       */
XmFileSelectionBoxCallbackStruct *cbs; /* callback structure              */
int modo;      /* read/write                      */
int app;       /* applicazione chiamante          */
{
char *messaggio;
int esito = 0;
char *risp;
/*  static: si ritorna il suo indirizzo, e una variabile locale non sopravvive
    alla funzione. I chiamanti (fileSelection.c) copiano subito il risultato. */
static char nome[MAX_PATH_LEN];

	switch (cbs->reason)
		{
		case XmCR_CANCEL:
			strcpy (nome, FILE_ERRORE);
			return (nome);
			break;
		case XmCR_OK:
		default:
			break;
		}
/*
 * handling case XmCR_OK 
 */
   if (!XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &risp))
      {
      printf ("stringa non letta\n");
		strcpy (nome, FILE_ERRORE);
      return (nome);
      }
	strncpy (nome,risp,MAX_PATH_LEN-1);
	nome[MAX_PATH_LEN-1] = '\0';
	XtFree (risp);
	printf ("letto :%s:\n",nome);

	if (check_file_select (w, app, modo, nome) < 0)
		{
		strcpy (nome, FILE_ERRORE);
      return (nome);
      }
	return (nome);
}
/**********************************************************/
int file_esiste (s)
char *s;
{
struct stat buf;

	if (stat (s, &buf) == -1)              /* non accessibile   */
		{
		printf ("errore 1\n");
		return (0);
		}
	if ((buf.st_mode & S_IFMT) == S_IFDIR)  /* directory         */
		{
		printf ("errore 2\n");
		return (0);
		}
	if ((buf.st_mode & S_IFMT) == S_IFREG)  /* regular file      */
		return (1);                          /* ok                */
	return (0);
}
/**********************************************************/
int not_reserved (s, from)
char *s;
int from;
{
char filename[MAX_PATH_LEN];
int i,k;

	for (i=0,k=0; s[i]!='\0'; i++)
		{
		if (s[i] == '/')
			{
			k = 0;
			continue;
			}
		filename[k] = s[i];
		k++;
		}	
	filename[k] = '\0';
	if (k == 0) /* non c'e' il filename */
		return (0);
	
	printf ("compare con file :%s:\n",filename);
	
	switch (from)
		{
		case VAR_FOR_CR:
			i = 0;
   		while (strcmp(reserved_for_cr_files[i],"NULL"))
      		{
      		if (!strcmp(filename,reserved_for_cr_files[i]))
         		return (0);
      		i++;
      		}
			break;
		case PREVIEW_FILE:
			i = 0;
         while (strcmp(reserved_for_preview_files[i],"NULL"))
            {
            if (!strcmp(filename,reserved_for_preview_files[i]))
               return (0);
            i++;
            }
			break;
		default:
			break;
		}
	return (1);
}
/**********************************************************/
int in_path (s)
char *s;
{
char path[MAX_PATH_LEN];

	strcpy (path,getenv("HOME"));
printf ("compare per path :%s: :%s: %d\n",path,s,strlen(path));
	if (!strncmp(path,s,strlen(path)))
		return(1);
	return (0);
}
/**********************************************************/
int scrivibile (s)
char *s;
{
	if (!access (s, W_OK))
   	return (1); 
	return (0);
}
/**********************************************************/
int leggibile (s)
char *s;
{
   if (!access (s, R_OK))
      return (1); 
   return (0);
}
/**********************************************************/
int check_file_select (w,app, modo, s)
Widget w;
int app;
int modo;
char *s;
{
char *messaggio;
int esito = 0;

   messaggio = (char *)malloc (strlen(SAVE_FILE_CR)+
							  strlen(LOAD_FILE_CR)+
							  strlen(LOAD_FILE_PREV)+
							  strlen(SAVE_FILE_PREV)+
							  strlen(s)+
                       strlen(NOT_PERMITTED)+
                       strlen(OUT_OF_PATH)+
                       strlen(NAME_RESERVED)+
                       strlen(FALLITA)+20);
	switch (app)
		{
		case VAR_FOR_CR:
			switch (modo)
				{
				case CARICA:
					sprintf (messaggio,"%s \n %s ",LOAD_FILE_CR,s);
					break;
				case SALVA:
					sprintf (messaggio,"%s \n %s ",SAVE_FILE_CR,s);
					break;
				}
			break;
		case PREVIEW_FILE:
         switch (modo)
            {
            case CARICA:
               sprintf (messaggio,"%s \n %s ",LOAD_FILE_PREV,s);
               break;
            case SALVA:
               sprintf (messaggio,"%s \n %s ",SAVE_FILE_PREV,s);
               break;
            }
			break;
		}

	s = full_path (s);
	printf ("full path :%s:\n",s);

#ifndef AIX /* ibm linka /u con /home  */  
   if (!in_path (s))   /* punta directory fuori dall' utente */
      {
      esito = -1;
      sprintf (messaggio,"%s\n %s \n %s",
             messaggio,OUT_OF_PATH,FALLITA);
      add_message (areaMessaggi,messaggio,LIVELLO_1);
      attenzione (w,messaggio);
      free (messaggio);
      return (esito);
      }
#endif
   if (!not_reserved (s,app)) /* file riservato   */
      {
      esito = -1;
      sprintf (messaggio,"%s\n %s \n %s",
            messaggio,NAME_RESERVED,FALLITA);
      add_message (areaMessaggi,messaggio,LIVELLO_1);
      attenzione (w,messaggio);
      free (messaggio);
      return (esito);
      }
	switch (modo)
		{
		case CARICA:

			if (!file_esiste(s))
      		{
      		esito = -1;
      		sprintf (messaggio,"%s\n %s \n %s",
              		messaggio,NOT_ACCESSIBLE,FALLITA);
      		add_message (areaMessaggi,messaggio,LIVELLO_1);
      		attenzione (w,messaggio);
      		free (messaggio);
      		free (s);
      		return (esito);
      		}

   		if (!leggibile (s)) /* non ha permessi lettura       */
      		{
      		esito = -1;
      		sprintf (messaggio,"%s \n %s \n %s",
              		messaggio,NOT_PERMITTED,FALLITA);
      		add_message (areaMessaggi,messaggio,LIVELLO_1);
      		attenzione (w,messaggio);
      		free (messaggio);
      		free (s);
      		return (esito);
      		}
				break;

		case SALVA:
   		if (file_esiste(s))
      		{
      		if (!scrivibile (s)) /* non ha i permessi in scrittura */
         		{
         		esito = -1;
         		sprintf (messaggio,"%s \n %s \n %s",
                 		messaggio,NOT_PERMITTED,FALLITA);
         		add_message (areaMessaggi,messaggio,LIVELLO_1);
         		attenzione (w,messaggio);
         		free (messaggio);
         		return (esito);
         		}
      		}
			break;
		}
	return (esito);
}
/**********************************************************/
char *full_path (s)
char *s;
{
char *app;
char path[MAX_PATH_LEN];

	if (s[0] != '/') /* specificato  file name ,mette directory corrente  */
      {
      app = (char *)malloc(strlen(s)+1);
      strcpy (app,s);
      strcpy (path, getenv("PWD"));
      sprintf (s,"%s/%s",path,app);
      printf ("path completo :%s:\n",s);
      free(app);
      }
	return (s);
}
/***********************************************************/
