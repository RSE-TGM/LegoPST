/**********************************************************************
*
*       C Source:               connessioni.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Fri Aug 29 14:04:00 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: connessioni.c-10 %  (%full_filespec: connessioni.c-10:csrc:1 %)";
#endif

/*
 * connessioni.c
 *
 * funzioni relative alla inizializzazione e aggancio al simulatore
 */
#include <stdio.h>
#include <string.h>

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

#include "UxXt.h"

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

#include "sked.h"
//#include "sked_fun.h"
#include "bistrutt.h"

#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"

#include "risorse.h"

#include "Rt/RtDbPunti.h"
#include "Rt/RtErrore.h"
#include "Rt/RtMemory.h"

#include "parametri.h"
#include "messaggi.h"
#include "messages.h"
#include "master_monit.h"
#include "cont_rec.h"
#include "mainOptions.h"
#include "libdispatcher.h"

VARIABILI *variabili;
IND_VARIABILI *ind_variabili;
char *ind_sh_top;
int id_sh;
int nmod;
int tot_variabili;

RtDbPuntiOggetto dbpunti;
RtDbPuntiOggetto dbpunti_ext;
RtErroreOggetto errore;

int size_area_dati;
int size_area_spare;
int size_blocco_snap;
int size_header_snap;
int size_parte1_bktk;
int size_blocco_bktk;

int shr_usr_key;
int isSuperuser;        /* True = superuser         */

extern int stato_sim;
extern RICHIESTA_STAT richiesta_stat;
extern Widget areaMessaggi;
extern Widget masterMenu;

extern XrmDatabase GetExtResDb ();
extern XrmDatabase GetApplResDb ();
extern int    _MAX_SNAP_SHOT;
extern int    _MAX_BACK_TRACK;
extern int    _MAX_CAMPIONI;
extern int    _NUM_VAR;
extern int    _MAX_PERTUR;
extern int    _SPARE_SNAP;
extern int    _PERT_CLEAR;

extern char *FILES_PATH;

extern XrmDatabase Xdb;

int errsked=0,errdisp=0,errshm=0,errmonit=0,errtotali=0;

char   *nome_modello (char *, int);
void legge_sel(char *);
int get_colors();
int caricaMessages (int );
int aggancia_dispatcher (Widget, char*);
int ControlParam(int );
int init_message_list();
int not_initialized (Widget,char*);
int     SD_lbtreg (int);
 




/********************************************************************/
int main_init(n,parm)
int n;                /* numero parametri passati       */
char *parm[];          /* stringhe                       */
{
int i;
char *nome;
XrmDatabase NewXdb;
char *str_type[20];
XrmValue valore;
char stringa[200];

/*****
	for (i=0; i<n; i++)
		printf ("%d] [%s] \n",i,parm[i]);
*******/
	isSuperuser = 0;
	if (n > 1)
		{
		if ((n-1) > MAX_MAINOPTIONS)
			n = MAX_MAINOPTIONS;
		printf ("parametro [%s]\n",parm[USER_POSITION]);
		if (!strcmp(parm[USER_POSITION],SUPERUSER_KEY))
			{
			isSuperuser = 1;
//			printf ("Utente superuser ***********\n");
			}
		}

	masterMenu = (Widget)NULL;

	if (getenv("BANCO_FILES") == NULL)
		{
		FILES_PATH = malloc (strlen(".")+1);
		strcpy (FILES_PATH,".");
		}
	else
		{
		FILES_PATH = malloc (strlen(getenv("BANCO_FILES"))+1);
		strcpy (FILES_PATH,getenv("BANCO_FILES"));
		}

	if (get_colors() < 0)
      {
      fprintf (stderr,"#####  BANCO: get_colors error!!!\n");
      fprintf (stderr,"#####  BANCO: Problems in colours allocation!!!\n");
      return(-1);
      }
   read_options();
   stato_sim = STATO_STOP; /* preset a stop */
	tempo_finale_impostato = 0;	/* preset simulazione senza limiti	*/	
/*
 * preparazione stringhe
 */

	caricaMessages (DEFAULT_TEXT);
/*
 * acquisizione risorse
 */
	nome = (char *)malloc(strlen(FILES_PATH)+strlen(RESOURCE_FILE)+2);
	sprintf (nome,"%s/%s",FILES_PATH,RESOURCE_FILE);
	if (!existFile(nome))			/* file locale non esiste	*/
		{
		Xdb = GetApplResDb (UxDisplay); /* acquisisce il database attuale	*/
		XrmPutFileDatabase (Xdb, nome); /* lo salva nel file					*/
		}
	NewXdb = GetExtResDb (nome);				/* ricarica dal file locale */
	Xdb = GetApplResDb (UxDisplay);
	ApplyResDb (NewXdb, &Xdb, UxDisplay, MERGE);

	free (nome);	

	if (XrmGetResource (Xdb, "banco*Background", "Banco*Background",
						str_type, &valore) == True)
		{
		strncpy (stringa, valore.addr, (int)valore.size);
//		printf ("Risorsa banco*background [%s]\n",stringa);
		}
	else
		printf ("Risorsa banco*background non restituita\n");

   if (XrmGetResource (Xdb, "banco*listaScenari*Background",
										"Banco*ListaScenari*Background",
                  str_type, &valore) == True)
      {
      strncpy (stringa, valore.addr, (int)valore.size);
//      printf ("Risorsa banco*initialCondition*background [%s]\n",stringa);
      }
   else
      printf ("Risorsa banco*initialCondition*background non restituita\n");

	return(0);
}
/********************************************************************/
int richiesta_stat_default ()
{
   richiesta_stat.tempo       = 1;
   richiesta_stat.parametri   = 1;
   richiesta_stat.tempi       = 1;
   richiesta_stat.stato       = 1;
   richiesta_stat.operazione  = 1;
   richiesta_stat.statistiche = 1;
   return(0);
}
/********************************************************************/
int set_richiesta_stat(w)
Widget w;
{
char *mess;

	if (SD_stato(BANCO,&richiesta_stat) > 0)
		{
		if (exist_Widget(areaMessaggi))
			add_message (areaMessaggi,NEW_STAT_REQ,LIVELLO_1);
		}
	else
		{
		mess = malloc (sizeof(NEW_STAT_REQ)+sizeof(OPER_FALLITA)+10);
		if (exist_Widget(areaMessaggi))
			{
			sprintf (mess,"%s %s",NEW_STAT_REQ,OPER_FALLITA);
			add_message (areaMessaggi,mess,LIVELLO_1);
			}
		attenzione (w,mess);
		free(mess);
		}
	return (0);
}
/********************************************************************/
int connessioni(w)
Widget w;
{
char *s;
SIMULATOR *simpar;
char *errori;
char *messaggio;
int err_startup = 0;
int i,k;

	simpar = (SIMULATOR*)calloc(1,sizeof(SIMULATOR));
   	simpar->max_snap_shot  = _MAX_SNAP_SHOT;
   	simpar->max_back_track = _MAX_BACK_TRACK;
   	simpar->max_campioni   = _MAX_CAMPIONI;
   	simpar->num_var        = _NUM_VAR;
   	simpar->max_pertur     = _MAX_PERTUR;
   	simpar->spare_snap     = _SPARE_SNAP;
   	simpar->pert_clear     = _PERT_CLEAR;
/*
 * aggancia le code di messaggi con dispatcher
 */
	shr_usr_key = atoi(getenv("SHR_USR_KEY"));
	msg_create_fam(shr_usr_key,0);
/*
 * crea o aggancia il database delle variabili
 */	
	costruisci_var(&ind_sh_top, &variabili, &id_sh);
	nmod = numero_modelli(ind_sh_top);
	tot_variabili = numero_variabili(ind_sh_top);
	printf ("*** numero modelli = %d numero variabili = %d\n",
							nmod,tot_variabili);
/*
 * caricamento della struttura 'ind_variabili' a partire da 'variabili'
 */
        ind_variabili = (IND_VARIABILI *)malloc(sizeof(IND_VARIABILI)*nmod);
        k = 0;
        ind_variabili[0].ind_mod = variabili[0].mod;
        strcpy(ind_variabili[0].nome_mod,nome_modello(ind_sh_top, k + 1)); 
        ind_variabili[0].ind_var = 0;

printf("nome_mod = %s   ind_var = %d\n",ind_variabili[0].nome_mod,
                                       ind_variabili[0].ind_var);
 
        for(i=1;i<tot_variabili;i++) {
           if(variabili[i].mod != ind_variabili[k].ind_mod) {
              k++;
              ind_variabili[k].ind_mod = variabili[i].mod;
              strcpy(ind_variabili[k].nome_mod,nome_modello(ind_sh_top, k + 1)); 
              ind_variabili[k].ind_var = i;

printf("nome_mod = %s   ind_var = %d\n",ind_variabili[k].nome_mod,
                                       ind_variabili[k].ind_var);

           }
        }

   /* aggancio al database dei punti */
   errore = RtCreateErrore(RT_ERRORE_TERMINALE,"net_sked");
#if defined SCO_UNIX
   dbpunti = RtCreateDbPunti(errore,"S02_SCO",DB_PUNTI_INT,simpar);
   dbpunti_ext = RtCreateDbPunti(errore,"S02_SCO",DB_PUNTI_SHARED,simpar);
#endif
#if defined OSF1
   dbpunti = RtCreateDbPunti(errore,"S02_OSF1",DB_PUNTI_INT,simpar);
   dbpunti_ext = RtCreateDbPunti(errore,"S02_OSF1",DB_PUNTI_SHARED,simpar);
#endif
#if defined LINUX
   dbpunti = RtCreateDbPunti(errore,"S02_LINUX",DB_PUNTI_INT,simpar);
   dbpunti_ext = RtCreateDbPunti(errore,"S02_LINUX",DB_PUNTI_SHARED,simpar);
#endif
#if defined AIX
   dbpunti = RtCreateDbPunti(errore,"S02_AIX",DB_PUNTI_INT,simpar);
   dbpunti_ext = RtCreateDbPunti(errore,"S02_AIX",DB_PUNTI_SHARED,simpar);
#endif
#if defined VMS
   dbpunti = RtCreateDbPunti(errore,"S02_VMS",DB_PUNTI_INT,simpar);
   dbpunti_ext = RtCreateDbPunti(errore,"S02_VMS",DB_PUNTI_SHARED,simpar);
#endif

/*
 * calcola ed acquisisce i parametri relativi ai files snap e BT
 */
	size_area_dati = RtDbPSizeData(dbpunti);
	size_header_snap = _MAX_SNAP_SHOT*sizeof(SNTAB)+sizeof(HEADER_REGISTRAZIONI);
	size_area_spare = _SPARE_SNAP*AREA_SPARE;
	size_blocco_snap = size_area_dati + sizeof(float) +
#if defined MFFR
								sizeof(SNAP_SKED) +
#endif
                       	DIM_SNAP_AUS      +
                       	size_area_spare * sizeof(char)         +
                       	2*_MAX_PERTUR * sizeof(TIPO_PERT);

	size_parte1_bktk =  _MAX_BACK_TRACK * sizeof(BKTAB) +
                       sizeof(HEADER_REGISTRAZIONI);
	size_blocco_bktk =  size_area_dati    +  sizeof(float)        +
#if defined MFFR
                       sizeof(SNAP_SKED) + 
#endif
							  DIM_SNAP_AUS         +
                       size_area_spare  * sizeof(char)           +
                       2*_MAX_PERTUR * sizeof(TIPO_PERT);
/*
 * legge o inizializza il file della lista delle variabili da registrare
 */
	flag_sel = SEL_REG;
	s = malloc (strlen(FILE_REC_DEFAULT)+1);
	strcpy (s,FILE_REC_DEFAULT);
	legge_sel (s);
	free (s);
	printf ("*** Numero variabili selezionate = %d\n",nu_var_sel);

	errori = (char*)calloc(ERRSTARTUP*sizeof(int),sizeof(char));
	if (aggancia_dispatcher(w,errori) < 0)
		exit (-1);
	errmonit = ControlParam(MONIT);
   i=0;
   memcpy(&errsked,&errori[i*sizeof(int)],sizeof(int));i++;
   memcpy(&errdisp,&errori[i*sizeof(int)],sizeof(int));i++;
   memcpy(&errshm, &errori[i*sizeof(int)],sizeof(int));i++;
   errtotali = errsked+errdisp+errshm+errmonit;
   if ( errtotali!=0 )
      fprintf(stderr,
				"Errori in fase di Startup (sk=%d disp=%d monit=%d shm=%d)\n",
              errsked,errdisp,errmonit,errshm);
   if( errtotali > 0 )
        err_startup = 1;
   free(errori);
/* test degli errori di startup */
    if ( err_startup == 1 )
            {
				messaggio = (char *)malloc(strlen(ABORTING)			+
													strlen(STARTUP_ERR) + 200);
				sprintf (messaggio,"%s\n %s\n (SK %d DISP %d BANCO %d SHM %d) ",
															STARTUP_ERR,
															ABORTING,
															errsked,errdisp,errmonit,errshm);
				create_attenzioneDialog(masterMenu,messaggio,DIALOG_ERROR);
   			free(messaggio);
				simulatorShutdown (masterMenu);
				}

	printf ("*** BANCO: dispatcher agganciato \n");

   richiesta_stat_default();
	set_richiesta_stat(w);
	init_message_list();
	fprintf (stderr,"*** BANCO: connessioni completate\n");
	return (0);
}
/********************************************************************/
int aggancia_dispatcher (w, err)
Widget w;
char *err;
{
char *stringa;
printf ("*** aggancia_dispatcher: GUAG BANCO=%d \n",BANCO);

	if (SD_aggancio(BANCO,err) < 0)
		{
printf ("*** aggancia_dispatcher: GUAG ERRORE AGGANCIO -BANCO=%d \n",BANCO);
		stringa = (char *)malloc (strlen(ERRORE_AGGANCIO) + 1);
		strcpy (stringa,ERRORE_AGGANCIO);
		not_initialized (w,stringa);
		free (stringa);
		return (-1);	
		}
	return (0);
}
/********************************************************************/
int read_last_backtrack (w)
Widget w;
{
char *messaggio;
int esito;

   esito = 0;
   messaggio = malloc (strlen(LAST_BT_READ) + strlen(OPER_FALLITA) + 20);
   if ((last_backtrack = SD_lbtreg(BANCO)) >= 0)
      {
      sprintf (messaggio,"%s : %d",LAST_BT_READ,last_backtrack);
      }
   else
      {
      sprintf (messaggio,"%s : %d",LAST_BT_READ,last_backtrack);
      esito = -1;
      attenzione(masterMenu,messaggio);
      }

   add_message (areaMessaggi,messaggio,LIVELLO_3);

   free (messaggio);
   return(esito);
}
/********************************************************************/
int startup_simulatore (w)
Widget w;
{
char *messaggio;
int esito;

   esito = 0;
   messaggio = malloc (strlen(INIT_SIM) + strlen(OPER_FALLITA) + 20);
   if ((SD_inizializza(BANCO)) >= 0)
      {
      sprintf (messaggio,"%s",INIT_SIM);
      }
   else
      {
      sprintf (messaggio,"%s \n %s",INIT_SIM,OPER_FALLITA);
      esito = -1;
      attenzione(masterMenu,messaggio);
      }

   add_message (areaMessaggi,messaggio,LIVELLO_1);

   free (messaggio);
   return(esito);
}
