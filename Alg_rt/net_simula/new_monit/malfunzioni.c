/*
 * malfunzioni.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <X11/Xlib.h>
#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/ToggleB.h>
#include <Xm/Xm.h>

#include "Rt/RtDbPunti.h"
#include "Rt/RtErrore.h"
#include "Rt/RtMemory.h"

#include "UxXt.h"
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "malfunzioni.h"
#include "parametri.h"
#include "bistrutt.h"
#include "tabelle_malf.h"
#include "refresh.h"
#include "cont_rec.h"

extern VAL val;
extern VARIABILI *variabili;
extern IND_VARIABILI *ind_variabili;

extern int sommario_malf_active;
extern int sommario_frem_active;
extern int lista_scenari_malf_active;
extern int lista_scenari_frem_active;

extern XtIntervalId timer_listaScenMalf;
extern XtIntervalId timer_listaScenFrem;

#include "messaggi.h"
#include "cursore.h"
#include "file_selection.h"
#include "filtri.h"
#include "listaScenari.h"
#include "selVar.h"
#include "scenarioMalf.h"
#include "malfConfig.h"

#include "option.h"
#include "bistrutt.h"
#include "tabelle_malf.h"
#include "banco_globals.h"


extern int tot_variabili;
extern char *FILES_PATH;

extern Widget areaMessaggi;
extern Widget masterMenu;
extern Widget scenarioMalf;
extern Widget scenarioFrem;
extern Widget sommarioMalf;
extern Widget sommarioFrem;
extern Widget listaScenariMalf;
extern Widget listaScenariFrem;

extern SCENARI scenari_malf;
extern SCENARI scenari_frem;

extern int _MAX_PERTUR;
extern TIPO_PERT *pertMalf;         /* area pert per malfunzioni/funz. remote */
extern TIPO_PERT *pertMalfOld;         /* area pert per malfunzioni/funz. remote */

M_COMP_TYPE *m_comp_type;
MALF_SET *malf_set;
M_COMPONENT *m_component;

M_COMP_TYPE *fr_comp_type;
MALF_SET *frem_set;
M_COMPONENT *fr_component;

/* flag alzata dal caricamento ci o bt e resettata da passaggio a run */
/* serve a malf/frem per gestione valori target var. sec.				 */
int nuovoSnapCaricato = 0;        
int nuovoSnapCaricato2 = 0;        

extern int nuovoScenarioMalf;
extern int nuovoScenarioFrem;

extern RtDbPuntiOggetto dbpunti;

extern char titolo_sessione[MAX_LUN_COMMENTO];

extern int id_msg_pert;
extern char *ind_sh_top;
char *creaTestoMalfList();

void act_malf_scenario ();
void riga_scen_malf_sel ();
void sel_riga_scenario ();

char *sintesiDescr ();
void openMalfConfig ();
extern Widget create_listaScenari();
extern Widget create_listaNuova();

int startMalfunzione (int a, float v, REC_SCENARIO s);

void refresh_scenarioMalf ();
static   void  varSecInModifica();
int SD_listascenari (int,int, SCENARI *);
int add_item_list (Widget,char*);
int caricaTipologiaMalf (Widget,int);
int carica_conf_malf(Widget,int);
int completa_conf_malf (Widget, int, M_COMPONENT *, MALF_SET *, M_COMP_TYPE *);


struct riga_val_st
	{
	int nriga;
	int valore;
	};
typedef struct riga_val_st RIGA_VAL;
RIGA_VAL riga_val[MAX_VAR_SCENARIO][MAX_MALF_SET_VALUE];
/***************************************************************/
int attiva_sommario (w,tipo)
Widget w;
int tipo;
{
switch (tipo)
	{
	case MALFUNZIONI:
	case SOMMARIO_MALF:
		if (sommario_malf_active)
   		{
   		if (!exist_Widget(sommarioMalf))
      		create_scenarioMalf(masterMenu,tipo,-1);
   		}
		else
   		create_scenarioMalf(masterMenu,tipo,-1);
	break;
	case FUNZIONI_REMOTE:
   case SOMMARIO_FREM:
      if (sommario_frem_active)
         {
         if (!exist_Widget(sommarioFrem))
            create_scenarioMalf(masterMenu,tipo,-1);
         }
      else
         create_scenarioMalf(masterMenu,tipo,-1);
   break;
	}
return (0);
}
/************************************************************/
int attiva_lista_scenari(w,tipo)
Widget w;
int tipo;
{
switch (tipo)
	{
	case MALFUNZIONI:
		if (lista_scenari_malf_active)
   		{
   		if (!exist_Widget(listaScenariMalf))
      		   if(create_listaScenari(masterMenu,tipo) == NULL)
                      return(-1);
   		}
		else
   		   if(create_listaScenari(masterMenu,tipo) == NULL)
                      return(-1);
		break;

	case FUNZIONI_REMOTE:
      if (lista_scenari_frem_active)
         {
         if (!exist_Widget(listaScenariFrem))
            if(create_listaScenari(masterMenu,tipo) == NULL)
               return(-1);
         }
      else
         if(create_listaScenari(masterMenu,tipo) == NULL)
            return(-1);
		break;
	}
return (0);
}
/************************************************************/
int readScenari (w, tipo, s)
Widget w;
int tipo;
SCENARI *s;
{
   if (SD_listascenari (BANCO,tipo,s) >= 0) {
      return (0);
   }
   fprintf (stderr,"read_scenari_malf: errore lettura ");
   perror ("read_scenari_malf: errore lettura --> ");
   return (-1);
}
/************************************************************/
int displayScenari (w, s)
Widget w;
SCENARI *s;
{
_UxClistaScenari *Ctx;
int i;
char *stringa;
int Validi = 0;

	Ctx = (_UxClistaScenari *)UxGetContext(w);

	clear_lista (Ctx->UxscrollListaScen);

	for (i=0; i<MAX_SCENARI; i++)
		{
		if (s->valido[i] >= 0)
			{
			Validi ++;
			stringa=(char *)malloc(strlen(s->s[i].scen_header.commento)+1);
			strcpy (stringa,s->s[i].scen_header.commento);
			}
		else
			{
			stringa=(char *)malloc(strlen(EMPTY_IC)+1);
			strcpy (stringa,EMPTY_IC);
			}
      add_item_list (Ctx->UxscrollListaScen,stringa);
      free (stringa);
		}
	return (Validi);
}
/************************************************************/
int countScenariValidi (SCENARI *scenari)
{
int i;
int n = 0;

	for (i=0; i<MAX_SCENARI; i++)
      if (scenari->valido[i] >= 0)
			n ++;
	return (n);
}
/************************************************************/
/*
 * legge e carica la configurazione malf e frem
 */
int readConfMalf (Widget w, int tipo)
{
int ret = 0;
int i;
static int primo_giro_malf = 1;
static int primo_giro_frem = 1;

   switch (tipo)
      {
      case MALFUNZIONI:
      case SOMMARIO_MALF:
         if (primo_giro_malf > 0)  
            {
            if ((ret = caricaTipologiaMalf (w,tipo)) >= 0)
               {
               if ((ret = carica_conf_malf (w,tipo)) >= 0)
                  {
                  completa_conf_malf (w, tipo, m_component, malf_set, m_comp_type);	  
                  }
               }	
            if(ret >= 0)
               primo_giro_malf = 0;
            else
               return(-1);
            }
         break;
      case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         if (primo_giro_frem > 0)
            {
            if ((ret = caricaTipologiaMalf (w,tipo)) >= 0)
               {
               if ((ret = carica_conf_malf (w,tipo)) >= 0)
                  completa_conf_malf (w, tipo, fr_component, frem_set, fr_comp_type);
               }
            if(ret >= 0)
               primo_giro_malf = 0;
            else
               return(-1);
            }
         break;
      }
   return(0);
}


/************************************************************/
int caricaTipologiaMalf (w,tipo)
Widget w;
int tipo; 			/* malf o frem */
{
FILE *fp;
char path[400];
char *messaggio;
#define ALLOC_MIN  100     /* minimo 100 elementi */
int num_alloc;
int n_malf_type;
int n_malf_set;
int ret;
int i;


/*
 * carica la tipologia della configurazione
 */
   strcpy (path, FILES_PATH);
   strcat (path,"/");
   switch (tipo)
      {
      case MALFUNZIONI:
      case SOMMARIO_MALF:
         strcat (path,FILE_MALF_COMP_TYPE);
         break;
      case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         strcat (path,FILE_FREM_COMP_TYPE);
         break;
      }
      if ((fp = fopen (path,"r")) == NULL)
         {
         messaggio = (char *)malloc(strlen(MALF_CONF_READ)+
                                    strlen(FILE_MALF_COMP_TYPE)+10+
                                    strlen(NOT_EXISTENT));
         sprintf (messaggio,"%s \n %s : \n %s",
                             MALF_CONF_READ,FILE_MALF_COMP_TYPE,NOT_EXISTENT);
         attenzione (masterMenu,  messaggio);
         free (messaggio);
         return (-1);
         }
   switch (tipo)
      {
      case MALFUNZIONI:
      case SOMMARIO_MALF:
         num_alloc = 1;
         m_comp_type = (M_COMP_TYPE *)malloc(sizeof(M_COMP_TYPE)*
                                       ALLOC_MIN*num_alloc);
         n_malf_type = 0;
         while ((ret = getMalfType(w,fp,n_malf_type,m_comp_type)) >= 0)  /* record letto (mette il tappo alla fine) */
            {
            n_malf_type++;
            if (((n_malf_type/ALLOC_MIN)* ALLOC_MIN) == n_malf_type)
               {
               num_alloc++;
               m_comp_type = (M_COMP_TYPE *)realloc (m_comp_type,
                                                     sizeof(M_COMP_TYPE)*
                                                     ALLOC_MIN*num_alloc);
               printf ("MALFUNZ: realloc n. %d var %d\n",
                     		num_alloc,n_malf_type);
               }
            }
            i = 0;
            while (m_comp_type[i].m_c_t_code >= 0)
               {
               printf ("%d) tipo %d\n",i,m_comp_type[i].m_c_t_code);
               i++;
               }
         break;
      case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         num_alloc = 1;
         fr_comp_type = (M_COMP_TYPE *)malloc(sizeof(M_COMP_TYPE)*
                                       ALLOC_MIN*num_alloc);
         n_malf_type = 0;
         while ((ret = getMalfType(w,fp,n_malf_type,fr_comp_type)) >= 0)  /* record letto (mette il tappo alla fine) */
            {
            n_malf_type++;
            if (((n_malf_type/ALLOC_MIN)* ALLOC_MIN) == n_malf_type)
               {
               num_alloc++;
               fr_comp_type = (M_COMP_TYPE *)realloc (fr_comp_type,
                                       sizeof(M_COMP_TYPE)*
                                       ALLOC_MIN*num_alloc);
               printf ("MALFUNZ: realloc n. %d var %d\n",
                           num_alloc,n_malf_type);
               }
            }
         i = 0;
         while (fr_comp_type[i].m_c_t_code >= 0)
            {
            printf ("%d) tipo %d\n",i,fr_comp_type[i].m_c_t_code);
            i++;
            }
      break;
   }
/*****************/
   strcpy (path, FILES_PATH);
   strcat (path,"/");
   switch (tipo)
      {
      case MALFUNZIONI:
      case SOMMARIO_MALF:
         strcat (path,FILE_MALF_SET);
         break;
      case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         strcat (path,FILE_FREM_SET);
         break;
      }

   if ((fp = fopen (path,"r")) == NULL)
      {
      messaggio = (char *)malloc(strlen(MALF_CONF_READ)+
                                 strlen(path)+10+
                                 strlen(NOT_EXISTENT));
      sprintf (messaggio,"%s \n %s : \n %s",
                  MALF_CONF_READ,FILE_MALF_SET,NOT_EXISTENT);
      attenzione(masterMenu, messaggio);
      free (messaggio);
      return (-1);
      }
   switch (tipo)
      {
      case MALFUNZIONI:
      case SOMMARIO_MALF:
         num_alloc = 1;
         malf_set = (MALF_SET *)malloc(sizeof(MALF_SET)*
                                       ALLOC_MIN*num_alloc);
         n_malf_set = 0;
         while ((ret = getMalfSet(w,fp,n_malf_set,malf_set)) >= 0)  /* record letto (mette il tappo alla fine) */
            {
            n_malf_set++;
            if (((n_malf_set/ALLOC_MIN)* ALLOC_MIN) == n_malf_set)
               {
               num_alloc++;
               malf_set = (MALF_SET *)realloc (malf_set,
                                               sizeof(MALF_SET)*
                                               ALLOC_MIN*num_alloc);
               printf ("MALFUNZ: realloc n. %d var %d\n",
                       num_alloc,n_malf_set);
               }
            }
         break;
      case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         num_alloc = 1;
         frem_set = (MALF_SET *)malloc(sizeof(MALF_SET)*
                                       ALLOC_MIN*num_alloc);
         n_malf_set = 0;
         while ((ret = getMalfSet(w,fp,n_malf_set,frem_set)) >= 0)  /* record letto (mette il tappo alla fine) */
            {
            n_malf_set++;
            if (((n_malf_set/ALLOC_MIN)* ALLOC_MIN) == n_malf_set)
               {
               num_alloc++;
               frem_set = (MALF_SET *)realloc (frem_set,
                                       sizeof(MALF_SET)*
                                       ALLOC_MIN*num_alloc);
               printf ("MALFUNZ: realloc n. %d var %d\n",
                     num_alloc,n_malf_set);
               }
            }
         break;
      }
   return (0);
}
/************************************************************/
/*
 * carica un record alla volta e mette un tappo all' EOF 
 */
int getMalfType (w,fp,indx,m_c_t)
Widget w;
FILE *fp;
int indx;
M_COMP_TYPE *m_c_t;
{
#define MAX_STRINGA (MAX_LUN_NOME_VAR*4)+(MAX_LUN_DESCR_VAR*4)+ \
							MAX_LUN_NOME_BLOCCO+LUN_DESCR_TIPO_LUNGA+ \
							LUN_DESCR_TIPO_BREVE+LUN_NOME_TIPO+10+ \
							20
char riga[MAX_STRINGA];
char delimiter[2];
char campo[MAX_STRINGA];
char *messaggio;
int count;
int errore = 0;
int next_pos;
char pertSec[MAX_LUN_DESCR_VAR];
	
	strcpy (delimiter,";");

	if (fgets(riga,MAX_STRINGA,fp) != NULL)
		{
		riga[strlen(riga)-1] = '\0';   /* toglie new line */
/*		printf ("riga =%s=\n",riga);  */

		count = -1;
		next_pos = 0;
		while (next_pos >= 0)
			{
			count ++;
			getCampoStringa(riga, campo, &next_pos, delimiter);
				switch (count)
					{
					case 0:
						m_c_t[indx].m_c_t_code = atoi(campo);
/*				printf ("C_T_CODE :%s: %d\n",campo,m_c_t[indx].m_c_t_code); */
						break;
					case 1:
						strcpy (m_c_t[indx].m_c_type,campo);
						break;
					case 2:
						strcpy (m_c_t[indx].m_c_t_desc,campo);
						break;
					case 3:
						strcpy (m_c_t[indx].m_c_t_note,campo);
						break;
					case 4:
						strcpy (m_c_t[indx].module,campo);
						break;
					case 5:
						strcpy (m_c_t[indx].main_var,campo);
						break;	

					case 6:
						strcpy (m_c_t[indx].sec_var_1,campo);
						break;	
					case 7:
						strcpy (m_c_t[indx].sec_des_1,campo);
						break;	
					case 8:
						strcpy (m_c_t[indx].pertSec1,campo);
						break;	
					case 9:
						if (!validaStrNumber(campo,pertSec))
							strcpy (m_c_t[indx].valPertSec1,pertSec);
						else
							strcpy (m_c_t[indx].valPertSec1,"0.0");
						break;	

               case 10:
                  strcpy (m_c_t[indx].sec_var_2,campo);
                  break;
               case 11:
                  strcpy (m_c_t[indx].sec_des_2,campo);
                  break;
               case 12:
                  strcpy (m_c_t[indx].pertSec2,campo);
                  break;
               case 13:
						if (!validaStrNumber(campo,pertSec))
                     strcpy (m_c_t[indx].valPertSec2,pertSec);
                  else
                     strcpy (m_c_t[indx].valPertSec2,"0.0");
                  break;
                                      
               case 14:
                  strcpy (m_c_t[indx].sec_var_3,campo);
                  break;
               case 15:
                  strcpy (m_c_t[indx].sec_des_3,campo);
                  break;
               case 16:
                  strcpy (m_c_t[indx].pertSec3,campo);
                  break;
               case 17:
						if (!validaStrNumber(campo,pertSec))
                     strcpy (m_c_t[indx].valPertSec3,pertSec);
                  else
                     strcpy (m_c_t[indx].valPertSec3,"0.0");
                  break;
                                     
               case 18:
                  strcpy (m_c_t[indx].sec_var_4,campo);
                  break;
               case 19:
                  strcpy (m_c_t[indx].sec_des_4,campo);
                  break;
               case 20:
                  strcpy (m_c_t[indx].pertSec4,campo);
                  break;
               case 21:
						if (!validaStrNumber(campo,pertSec))
                     strcpy (m_c_t[indx].valPertSec4,pertSec);
                  else
                     strcpy (m_c_t[indx].valPertSec4,"0.0");
                  break;
				}
			}
		return (0);
		}
	else    /* raggiunto eof */
		{
		m_c_t[indx].m_c_t_code = -1;  /* tappo finale   */
		printf ("validati %d records tipo\n",indx);
		return (-1);
		}
}
/************************************************************/
/*
 * carica un record alla volta e mette un tappo all' EOF 
 */
int getMalfSet (w,fp,indx,m_set)
Widget w;
FILE *fp;
int indx;
MALF_SET *m_set;
{
#define MAX_STRINGA LUN_DESCR_MALF_BREVE+ 20

char riga[MAX_STRINGA];
char delimiter[2];
char campo[MAX_STRINGA];
char *messaggio;
int count;
int errore = 0;
int next_pos;

	strcpy (delimiter,";");

   if (fgets(riga,MAX_STRINGA,fp) != NULL)
      {
      riga[strlen(riga)-1] = '\0';   /* toglie new line */
/*      printf ("riga =%s\n",riga); */
      count = -1;
		next_pos = 0;
		while (next_pos >= 0)
			{
			count ++;
         getCampoStringa (riga,campo,&next_pos,delimiter);
            switch (count)
               {
               case 0:
                  m_set[indx].m_s_value = atoi(campo);
                  break;
               case 1:
                  m_set[indx].m_c_t_code = atoi(campo);
                  break;
               case 2:
                  strcpy (m_set[indx].m_s_descr,campo);
                  break;
               }
         }
      return (0);
      }
   else    /* raggiunto eof */
      {
      m_set[indx].m_s_value = -1;  /* tappo finale   */
		m_set[indx].m_c_t_code = -1;  /* tappo finale   */
      printf ("validati %d records malf_set\n",indx);
      return (-1);
      }
}
/************************************************************/
/** int carica_conf_malf (w,tipo,m_c_t,m_set,m_comp)  **/
int carica_conf_malf (w,tipo)
Widget w;
int tipo;         /* malf o frem */
/*****
M_COMP_TYPE *m_c_t;
MALF_SET *m_set;
M_COMPONENT *m_comp;
*******/
{
FILE *fp;
char path[400];
char *messaggio;
#define ALLOC_MIN  100     /* minimo 100 elementi */
int num_alloc;
int n_malf;
int ret;
/*
 * carica la tipologia della configurazione
 */
   strcpy (path, FILES_PATH);
   strcat (path,"/");
   switch (tipo)
      {
      case MALFUNZIONI:
      case SOMMARIO_MALF:
         strcat (path,FILE_MALF_CONFIG);
         break;
      case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         strcat (path,FILE_FREM_CONFIG);
         break;
      }

   if ((fp = fopen (path,"r")) == NULL)
      {
      messaggio = (char *)malloc(strlen(MALF_CONF_READ)+
                              strlen(path)+10+
                              strlen(NOT_EXISTENT));
      sprintf (messaggio,"%s \n %s : \n %s",
                  MALF_CONF_READ,path,NOT_EXISTENT);
      attenzione(masterMenu, messaggio);
      free (messaggio);
      return (-1);
      }
   switch (tipo)
      {
      case MALFUNZIONI:
      case SOMMARIO_MALF:
         num_alloc = 1;
         m_component = (M_COMPONENT *)malloc(sizeof(M_COMPONENT)*
                                             ALLOC_MIN*num_alloc);
         n_malf = 0;
         while ((ret = getMalfConf(w,fp,n_malf,m_component)) >= 0)/* record letto (mette il tappo alla fine) */
            {
            n_malf++;
            if (((n_malf/ALLOC_MIN)* ALLOC_MIN) == n_malf)
               {
               num_alloc++;
               m_component = (M_COMPONENT *)realloc (m_component,
                                       	             sizeof(M_COMPONENT)*
                                                     ALLOC_MIN*num_alloc);
               printf ("MALFUNZ: realloc n. %d var %d\n", num_alloc,n_malf);
               }
            }
         break;
      case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         num_alloc = 1;
         fr_component = (M_COMPONENT *)malloc(sizeof(M_COMPONENT)*
                                       ALLOC_MIN*num_alloc);
         n_malf = 0;
         while ((ret = getMalfConf(w,fp,n_malf,fr_component)) >= 0)/* record letto (mette il tappo alla fine) */
            {
            n_malf++;
            if (((n_malf/ALLOC_MIN)* ALLOC_MIN) == n_malf)
               {
               num_alloc++;
               fr_component = (M_COMPONENT *)realloc (fr_component,
                                          sizeof(M_COMPONENT)*
                                       ALLOC_MIN*num_alloc);
               printf ("MALFUNZ: realloc n. %d var %d\n", num_alloc,n_malf);
               }
            }
         break;
      }
   return (0);
}
/************************************************************/
/*
 * carica un record alla volta e mette un tappo all' EOF
 */
int getMalfConf (w,fp,indx,m)
Widget w;
FILE *fp;
int indx;
M_COMPONENT *m;
{
#define MAX_STRINGA 		LUN_DESCR_MALF_BREVE+MAX_LUN_KKS+ \
								(MAX_LUN_MODU_CODE*5)+ 20

char riga[MAX_STRINGA];
char delimiter[2];
char campo[MAX_STRINGA];
char *messaggio;
int count;
int errore = 0;
int next_pos;

	strcpy (delimiter,";");
   if (fgets(riga,MAX_STRINGA,fp) != NULL)
      {
      riga[strlen(riga)-1] = '\0';   /* toglie new line */
/*      printf ("riga =%s=\n",riga); */
      count = -1;
		next_pos = 0;
		while (next_pos >= 0)
			{
			count ++;
			getCampoStringa(riga, campo, &next_pos, delimiter);
            switch (count)
               {
               case 0:
                  m[indx].m_c_t_code = atoi(campo);
                  break;
               case 1:
                  strcpy (m[indx].m_c_desc,campo);
                  break;
               case 2:
                  strcpy (m[indx].comp_code,campo);
                  break;
					case 3:
                  strcpy (m[indx].modu_code,campo);
                  break;
					case 4:
                  strcpy (m[indx].modu_code1,campo);
                  break;
					case 5:
                  strcpy (m[indx].modu_code2,campo);
                  break;
					case 6:
                  strcpy (m[indx].modu_code3,campo);
                  break;
					case 7:
                  strcpy (m[indx].modu_code4,campo);
                  break;
               }
         }
      return (0);
      }
   else    /* raggiunto eof */
      {
      m[indx].m_c_t_code = -1;  /* tappo finale   */
      printf ("validati %d records componente\n",indx);
      return (-1);
      }
}
/************************************************************/
/*
 * completa i dati della struttura m_component
 */
int completa_conf_malf (w, tipo, m, m_s, m_t)   
Widget w;
int tipo;
M_COMPONENT *m;
MALF_SET *m_s;
M_COMP_TYPE *m_t;
{
int i,k,n,ind,j;
int m_t_found;
int ret;
char *messaggio;
char *app;
char nome_model[30],nome_model_low[30];
int  nmod;
int  ind_start,ind_end;

   messaggio = (char *)malloc (strlen(MALF_CONF_READ)+
                               strlen(TYPE_NOT_EXIST) +
                               MAX_LUN_NOME_VAR+10+
                               strlen(VAR_NOT_EXIST));

   i = 0;
   nmod = numero_modelli(ind_sh_top);
   while (m[i].m_c_t_code >= 0)
      {
/*
   Soi ricava il nome del modello a cui appartiene il componente corrente.
*/ 
      app = strrchr(m[i].modu_code,'-');
      strcpy(nome_model,(app+1));
      strncpy (m[i].cod_rego,m[i].modu_code,LUN_COD_REGO);
      m[i].cod_rego[LUN_COD_REGO-1] = '\0';
      m_t_found = 0;
      k=0;
      while (m_t[k].m_c_t_code >= 0)
         { 
         if (m_t[k].m_c_t_code == m[i].m_c_t_code)
            {
            strcpy (m[i].label_main_var,m_t[k].main_var);
            strcat (m[i].label_main_var,m[i].cod_rego);
/* allineamento a 8 caratteri */
            if (strlen(m[i].label_main_var) < (MAX_LUN_NOME_VAR-1))
               {
               for (n=strlen(m[i].label_main_var); n<(MAX_LUN_NOME_VAR-1); n++)
                  m[i].label_main_var[n] = ' ';
               m[i].label_main_var[n] = '\0';
               }
/** printf ("label principale :%s:\n",m[i].label_main_var); **/

/*
   Si ricerca il modello corrente nella tabella 'ind_variabili' e si
   ricava l'indice della prima variabile del modello nella struttura
   'variabili'.
*/
               j = 0;
               while(nome_model[j]!='\0') {
                  nome_model_low[j] = tolower(nome_model[j]);
                  j++;
               }
               for(ind=0;ind<nmod;ind++) {
                  if(strcmp(nome_model,ind_variabili[ind].nome_mod)==0)
                     break;
                  if(strcmp(nome_model_low,ind_variabili[ind].nome_mod)==0)
                     break;
               }
               if(ind==nmod) {
                  printf("MODELLO: %s  NON TROVATO!!!! var = %s\n",nome_model,
                         m[i].label_main_var);
                  m[i].p_main_var = -1;
                  m[i].ind_main_var = -1; 
               }
               else { 
                  ind_start = ind_variabili[ind].ind_var;
                  if(ind == (nmod-1))
                     ind_end = tot_variabili;
                  else
                     ind_end = ind_variabili[ind+1].ind_var;
                  if ((m[i].p_main_var = get_label_addr_ind(m[i].label_main_var,&m[i].ind_main_var,ind_start,ind_end)) < 0)
                     {
                     sprintf (messaggio,"%s \n %s : %s",
			      MALF_CONF_READ,m[i].label_main_var,VAR_NOT_EXIST);
                     attenzione (w, messaggio);             
                     }
               }
/*
 * prima variabile secondaria
 */
            if (strcmp (m_t[k].sec_var_1, ""))
               {
               app = strrchr(m[i].modu_code1,'-');
               strcpy(nome_model,(app+1));
               j = 0;
               while(nome_model[j]!='\0') {
                  nome_model_low[j] = tolower(nome_model[j]);
                  j++;
               }
	       strncpy (m[i].cod_rego,m[i].modu_code1,LUN_COD_REGO);
               m[i].cod_rego[LUN_COD_REGO-1] = '\0';
               strcpy (m[i].label_sec_var_1,m_t[k].sec_var_1);
               strcat (m[i].label_sec_var_1,m[i].cod_rego);
               for(ind=0;ind<nmod;ind++) {
                  if(strcmp(nome_model,ind_variabili[ind].nome_mod)==0)
                     break;
                  if(strcmp(nome_model_low,ind_variabili[ind].nome_mod)==0)
                     break;
               }
               if(ind==nmod) {
                  printf("MODELLO: %s  NON TROVATO!!!! var = %s\n",nome_model,
                         m[i].label_sec_var_1);
                  m[i].p_sec_var_1 = -1;
                  m[i].ind_sec_var_1 = -1; 
               }
               else { 
                  ind_start = ind_variabili[ind].ind_var;
                  if(ind == (nmod-1))
                     ind_end = tot_variabili;
                  else
                     ind_end = ind_variabili[ind+1].ind_var;
                  if ((m[i].p_sec_var_1=get_label_addr_ind (m[i].label_sec_var_1,&m[i].ind_sec_var_1,ind_start,ind_end)) < 0)
                     {
                     sprintf (messaggio,"%s \n %s : %s",
                              MALF_CONF_READ,m[i].label_sec_var_1,VAR_NOT_EXIST);
                     attenzione (w, messaggio);          
                     }
                  }
               }
            else
               m[i].p_sec_var_1=-1;
/*
 * seconda variabile secondaria
 */
            if (strcmp (m_t[k].sec_var_2, ""))
               {
               app = strrchr(m[i].modu_code2,'-');
               strcpy(nome_model,(app+1));
               j = 0;
               while(nome_model[j]!='\0') {
                  nome_model_low[j] = tolower(nome_model[j]);
                  j++;
               }
	       strncpy (m[i].cod_rego,m[i].modu_code2,LUN_COD_REGO);
	       m[i].cod_rego[LUN_COD_REGO-1] = '\0';
               strcpy (m[i].label_sec_var_2,m_t[k].sec_var_2);
               strcat (m[i].label_sec_var_2,m[i].cod_rego);
               for(ind=0;ind<nmod;ind++) {
                  if(strcmp(nome_model,ind_variabili[ind].nome_mod)==0)
                     break;
                  if(strcmp(nome_model_low,ind_variabili[ind].nome_mod)==0)
                     break;
               }
               if(ind==nmod) {
                  printf("MODELLO: %s  NON TROVATO!!!! var = %s\n",nome_model,
                         m[i].label_sec_var_2);
                  m[i].p_sec_var_2 = -1;
                  m[i].ind_sec_var_2 = -1; 
               }
               else { 
                  ind_start = ind_variabili[ind].ind_var;
                  if(ind == (nmod-1))
                     ind_end = tot_variabili;
                  else
                     ind_end = ind_variabili[ind+1].ind_var;
                  if ((m[i].p_sec_var_2=get_label_addr_ind (m[i].label_sec_var_2,&m[i].ind_sec_var_2,ind_start,ind_end)) < 0)
                     {
                     sprintf (messaggio,"%s \n %s : %s",
                              MALF_CONF_READ,m[i].label_sec_var_2,VAR_NOT_EXIST);
                     attenzione (w, messaggio); 
                     }
                  }
               }
            else
               m[i].p_sec_var_2=-1;
/*
 * terza variabile secondaria
 */
            if (strcmp (m_t[k].sec_var_3, ""))
               {
               app = strrchr(m[i].modu_code3,'-');
               strcpy(nome_model,(app+1));
               j = 0;
               while(nome_model[j]!='\0') {
                  nome_model_low[j] = tolower(nome_model[j]);
                  j++;
               }
	       strncpy (m[i].cod_rego,m[i].modu_code3,LUN_COD_REGO);
	       m[i].cod_rego[LUN_COD_REGO-1] = '\0';
               strcpy (m[i].label_sec_var_3,m_t[k].sec_var_3);
               strcat (m[i].label_sec_var_3,m[i].cod_rego);
               for(ind=0;ind<nmod;ind++) {
                  if(strcmp(nome_model,ind_variabili[ind].nome_mod)==0)
                     break;
                  if(strcmp(nome_model_low,ind_variabili[ind].nome_mod)==0)
                     break;
               }
               if(ind==nmod) {
                  printf("MODELLO: %s  NON TROVATO!!!! var = %s\n",nome_model,
                         m[i].label_sec_var_3);
                  m[i].p_sec_var_3 = -1;
                  m[i].ind_sec_var_3 = -1; 
               }
               else { 
                  ind_start = ind_variabili[ind].ind_var;
                  if(ind == (nmod-1))
                     ind_end = tot_variabili;
                  else
                     ind_end = ind_variabili[ind+1].ind_var;
                  if ((m[i].p_sec_var_3=get_label_addr_ind (m[i].label_sec_var_3,&m[i].ind_sec_var_3,ind_start,ind_end)) < 0)
                     {
                     sprintf (messaggio,"%s \n %s : %s",
                              MALF_CONF_READ,m[i].label_sec_var_3,VAR_NOT_EXIST);
                     attenzione (w, messaggio); 
                     }
                  }
               }
            else
               m[i].p_sec_var_3=-1;

/*
 * quarta variabile secondaria
 */
            if (strcmp (m_t[k].sec_var_4, ""))
               {
               app = strrchr(m[i].modu_code4,'-');
               strcpy(nome_model,(app+1));
               j = 0;
               while(nome_model[j]!='\0') {
                  nome_model_low[j] = tolower(nome_model[j]);
                  j++;
               }
	       strncpy (m[i].cod_rego,m[i].modu_code4,LUN_COD_REGO);
	       m[i].cod_rego[LUN_COD_REGO-1] = '\0';
               strcpy (m[i].label_sec_var_4,m_t[k].sec_var_4);
               strcat (m[i].label_sec_var_4,m[i].cod_rego);
               for(ind=0;ind<nmod;ind++) {
                  if(strcmp(nome_model,ind_variabili[ind].nome_mod)==0)
                     break;
                  if(strcmp(nome_model_low,ind_variabili[ind].nome_mod)==0)
                     break;
               }
               if(ind==nmod) {
                  printf("MODELLO: %s  NON TROVATO!!!! var = %s\n",nome_model,
                         m[i].label_sec_var_4);
                  m[i].p_sec_var_4 = -1;
                  m[i].ind_sec_var_4 = -1; 
               }
               else { 
                  ind_start = ind_variabili[ind].ind_var;
                  if(ind == (nmod-1))
                     ind_end = tot_variabili;
                  else
                     ind_end = ind_variabili[ind+1].ind_var;
                  if ((m[i].p_sec_var_4=get_label_addr_ind (m[i].label_sec_var_4,&m[i].ind_sec_var_4,ind_start,ind_end)) < 0)
                     {
                     sprintf (messaggio,"%s \n %s : %s",
                              MALF_CONF_READ,m[i].label_sec_var_4,VAR_NOT_EXIST);
                     attenzione (w, messaggio);           
                     }
                  }
               }
            else
               m[i].p_sec_var_4=-1;

            m_t_found = 1;
            break;
            }
         k++;
         }
      if (!m_t_found)
         {
         sprintf (messaggio,"%s \n %d : %s",
                  MALF_CONF_READ,m[i].m_c_t_code,TYPE_NOT_EXIST);
         attenzione (w, messaggio);        
         }
         i++;
      }
      free (messaggio);

/*********
printf ("Controllo completamento struttura\n");
	i = 0;
	while (m[i].m_c_t_code >= 0)
		{
		printf ("%d) %s %d\n",i,m[i].label_main_var,m[i].p_main_var);
		i++;
		}
************/
   return (0);
}
/********************************************************************/
/*
 * esegue la chiamata a display_conf_malf
 *
 *	wgt			Widget dell' interfaccia chiamante (selVar)
 *	tipo 			tipo di filtro (FILTRO_MALF o FILTRO_FREM))
 *	mod			modello selezionato
 * block			blocco selezionato
 *	filtrokks   flag filtro attivo
 *	filtrotipo  flag filtro attivo
 *
 */ 
int visualizzaConfMalf (Widget wgt,int tipo,int mod, 
							int block, int filtrokks,int filtrotipo)
{
Widget lista;
_UxCselVar *Ctx = (_UxCselVar *)UxGetContext(wgt);

	lista = Ctx->UxselVarToSelect1;

	switch (tipo)
		{
		case FILTRO_MALF:
			display_conf_malf (lista,m_component,malf_set,m_comp_type,
									mod, block, filtrokks, filtrotipo);
			break;
		case FILTRO_FREM:
			display_conf_malf (lista,fr_component,frem_set,fr_comp_type,
									mod, block, filtrokks, filtrotipo);
			break;
		}
}
/********************************************************************/
/*
 * visualizza nella lista le malfunzioni dopo l' applicazione dei filtri
 *
 * 6/6/97 eliminata selezMalf e integrato in selVar
 */
int display_conf_malf (lista, m, m_s, m_t, mod, block, kks, tipo)
Widget lista;
M_COMPONENT *m;
MALF_SET *m_s;
M_COMP_TYPE *m_t;
int mod;       /* modello   */
int block;       /* blocco    */
int kks;		/* filtro kks attivo/disattivo */
int tipo;   /* filtro tipo attivo/disattivo */
{
int i;
#define ALLOC_MIN 50          /* minimo 50 malf   */
int size;
int n_reconf;
int nn;

	_UxCselVar *Ctx = (_UxCselVar *)UxGetContext(lista);

	size = ALLOC_MIN;
   if (!Ctx->Uxprimo_giro)
      {
      free (Ctx->Uxpunt_conf_malf);
      free (Ctx->Uxpunt_malf_kks);
      free (Ctx->Uxpunt_malf_tipo);
      free (Ctx->Uxpunt_malf_lista);  
      }
   Ctx->Uxprimo_giro = 0;

   Ctx->Uxsel_mod = 1;  /* per funzioni find   */
/*
 * pulisce le lista
 */
   XtUnmanageChild (lista);
   clear_lista (lista);
/************************************************************************
 * FILTRI:
 *    sono previsti i seguenti filtri:
 *          -  filtro lego (modulo/blocco)
 *          -  filtro KKS  (tag nella descrizione della variabile)=misure
 *          -  filtro tipo di grandezza (primo carattere della label )
 *          -  filtro ingresso/uscita
 *          -  filtro var. aingabili
 *    Al momento attuale la relazione tra i filtri e' di tipo AND
 *       nell' ordine specificato sopra.
 *                                           30/1/96 fm
 ************************************************************************/
/*
 * allocazione memoria per indici puntatori
 */
	n_reconf = 1;
	Ctx->Uxpunt_conf_malf = (int *)malloc (sizeof(int)* ALLOC_MIN*n_reconf);

/* FILTRO PRINCIPALE
 */
/* 
 * selezione delle malfunzioni da m_component
 * Carico in punt_conf_malf i puntatori in M_COMPONENT
 *		di tutte le malf. configurate filtrati per mod / block
 */

	Ctx->Uxn_conf_malf = 0;
	listaMalfModBloc (m,mod,block,
                          &Ctx->Uxpunt_conf_malf,&Ctx->Uxn_conf_malf,
                          &n_reconf, size);
/*
 * alloco memoria per i restanti puntatori (al max mi serve 
 *		lo stesso numero di elementi di listaMalfModBloc)
 *
 */
	Ctx->Uxpunt_malf_kks = (int *)malloc (sizeof(int)* ALLOC_MIN*n_reconf);
	Ctx->Uxpunt_malf_tipo = (int *)malloc (sizeof(int)* ALLOC_MIN*n_reconf);
	Ctx->Uxpunt_malf_lista = (int *)malloc (sizeof(int)* ALLOC_MIN*n_reconf);
/*****************************
 * FILTRO KKS
 */

   if (kks)
		{
		Ctx->Uxpunt_malf_kks = (int *)malloc (sizeof(int)*
                                                      ALLOC_MIN*Ctx->Uxn_conf_malf);
printf ("*** FILTRO KKS ATTIVO  ***\n");
		lista_kks_malf_conf (Ctx->Uxpunt_conf_malf,Ctx->Uxn_conf_malf,
                                     &Ctx->Uxn_conf_malf_kks,Ctx->Uxpunt_malf_kks,
                                     Ctx->Uxstringa_kks, m);
		}
/*****************************
 * FILTRO TIPO
 */
   if (tipo && !kks)
      {
printf ("*** FILTRO TIPO ATTIVO  ***\n");
		lista_tipo_malf_conf (Ctx->Uxpunt_conf_malf,Ctx->Uxn_conf_malf,
                                      &Ctx->Uxn_conf_malf_tipo, Ctx->Uxpunt_malf_tipo,
                                      Ctx->Uxfiltro_tipo,m);
      }

   if (tipo && kks)
      {
printf ("*** FILTRO TIPO ATTIVO  ***\n");
		lista_tipo_malf_conf (Ctx->Uxpunt_malf_kks,Ctx->Uxn_conf_malf_kks,
                                      &Ctx->Uxn_conf_malf_tipo, Ctx->Uxpunt_malf_tipo,
                                      Ctx->Uxfiltro_tipo,m);
      }
/*****************************
 * DISPLAY DELLE LISTE
 * (N.B.: la relazione tra i filtri e AND)
 */
	if (kks && !tipo)
      {
		Ctx->Uxpunt_malf_lista=(int *)malloc (sizeof(int)*Ctx->Uxn_conf_malf_kks);
		display_item_malf_conf (lista,Ctx->Uxpunt_malf_kks,Ctx->Uxn_conf_malf_kks,
								&Ctx->Uxpunt_malf_lista,&Ctx->Uxn_conf_malf_lista,size,
								m,m_s,m_t);

/*** 	for (nn=0;nn<Ctx->Uxn_conf_malf_lista;nn++)
			printf ("%d) %d\n",nn,*((Ctx->Uxpunt_malf_lista)+nn));  ***/

		}
	else if (tipo)
      {
		Ctx->Uxpunt_malf_lista = (int *)malloc (sizeof(int)*Ctx->Uxn_conf_malf_tipo);
		display_item_malf_conf (lista,Ctx->Uxpunt_malf_tipo,Ctx->Uxn_conf_malf_tipo,
								&Ctx->Uxpunt_malf_lista,&Ctx->Uxn_conf_malf_lista,size,
								m,m_s,m_t);
		}
	else
		{
		Ctx->Uxpunt_malf_lista = (int *)malloc (sizeof(int)*Ctx->Uxn_conf_malf);
		display_item_malf_conf (lista,Ctx->Uxpunt_conf_malf,Ctx->Uxn_conf_malf,
								&Ctx->Uxpunt_malf_lista,&Ctx->Uxn_conf_malf_lista,size,
								m,m_s,m_t);
		}
        XtVaSetValues (Ctx->UxselOkpb,XmNsensitive,False,NULL);
        XtVaSetValues (Ctx->UxselVarMenuPopupSelect,XmNsensitive,False,NULL);

	XtManageChild (lista);
	return (0);
}
/********************************************************************/
int listaMalfModBloc (m,mod,block,p_sel,n_sel,n_alloc_sel,dim)
M_COMPONENT *m;		/* database malfunzioni 							*/
int mod;              /* modello selezionato                        */
int block;              /* blocco selezionato                         */
int **p_sel;        /* punt. lista elementi selez. matching       */
int *n_sel;         /* n. elementi selez. matching                */
int *n_alloc_sel;   /* numero realloc selez.                      */
int dim; 			  /* elementi preallocati                       */
{
int i,k;
int n_s;
int punt;

   n_s = *n_sel;

/* nessun filtro modulo / blocco (prendo tutte le var. configurate )*/
   if ((mod == NO_MOD) && (block == NO_BLOCK))
      {
      i = 0;
      while (m[i].m_c_t_code >= 0)
         {
         rialloca_puntatore (p_sel,n_s,n_alloc_sel,dim);
         (*p_sel)[i] = i;
         n_s++;
      	 i++;
      	 }
      }
/* filtro di solo modello */
   else if (( mod >= 0) && (block == NO_BLOCK))
      {
      i = 0;
      while (m[i].m_c_t_code >= 0)
         {
         rialloca_puntatore (p_sel,n_s,n_alloc_sel,dim);
         if((punt = m[i].ind_main_var) >= 0 )
            {
            if (variabili[punt].mod == mod)
               {
               (*p_sel)[n_s] = i;
               n_s++;			
               }
            }
         i++;
         }
      }
/* filtro per modulo / blocco   */
   else if (( mod >= 0) && (block  >= 0))
      {
      i = 0;
      while (m[i].m_c_t_code >= 0)
         {
         rialloca_puntatore (p_sel,n_s,n_alloc_sel,dim);
         if((punt = m[i].ind_main_var) >= 0 )
            {
            if ( variabili[punt].mod == mod && 
                (block == numero_blocchi(ind_sh_top, mod)+1 ||
                variabili[punt].blocco == block))
               {
               (*p_sel)[n_s] = i;
               n_s++;
               }
            }
         i++;
         }
      }

   *n_sel = n_s;
   printf ("listaMalfModBloc: %d elementi conf malf in lista\n",*n_sel);
   return (0);
}
/********************************************************************/
int add_lista_conf_malf (w, m, n, m_s, m_t)
Widget w;        /* lista                         */
M_COMPONENT *m; 
int n;           /* elemento in m                 */
MALF_SET *m_s;
M_COMP_TYPE *m_t;
{
char      testo[500];
char descr[LUN_DESCR_TIPO_BREVE];
char comp[LUN_NOME_TIPO];
char     *str;
XmString  c_str;
int i;
int found;
	
	 i=0;
	 found = 0;

	 strcpy (testo,creaTestoMalfList(m,n,m_t));
/*************************
	 while (m_t[i].m_c_t_code >= 0)
		{
		if (m_t[i].m_c_t_code == m[n].m_c_t_code)
			{
			strcpy (comp,m_t[i].m_c_type);
			strcpy (descr,m_t[i].m_c_t_desc);
			found = 1;
			break;
			}
		i++;
		} 
	 if (!found)
		{
		printf ("add_lista_conf_malf : code %d non trovato\n",m[n].m_c_t_code);
		}
    sprintf( testo, "%s - %s - %s - %s", 
					m[n].label_main_var,comp,descr,m[n].comp_code);
*******************************************************************/
    str = (char *)XtMalloc(sizeof(char)*((strlen(testo)) + 20));
    strcpy(str, testo);
    c_str = XmStringCreate (str, XmSTRING_DEFAULT_CHARSET);
    XmListAddItemUnselected(w, c_str, 0);
    XSync(UxDisplay, False);
    XtFree(str);
    XmStringFree(c_str);

   return (0);
}
/**********************************************************/
int lista_kks_malf_conf (p_in,n_in,n_sel,p_sel,filtro,m)
int *p_in;        /* punt. lista elementi per check     */
int n_in;         /* n. elementi per check              */
int *n_sel;       /* n. elementi matching               */
int *p_sel;       /* punt. lista elementi matching      */
char *filtro;     /* filtro kks                         */
M_COMPONENT *m;   /* struttura mf o fr  					  */
{
int i,k;
int n;

      n = *n_sel;
      for (i=0,k=0,n=0; i<n_in; i++)
         {
         if (check_kks_malf_conf(p_in[i],filtro,m)>0)
            {
            p_sel[k] = p_in[i];
            n++;
            k++;
            }
         }
      *n_sel = n;
      return (0);
}
/**********************************************************/
int check_kks_malf_conf (indx, filtro,m)
int indx;         /* indice in m_component      */
char *filtro;
M_COMPONENT *m;   /* struttura mf o fr                  */
{
int i,k;
char stringa[MAX_LUN_DESCR_VAR];
char *s;
int match = 1;

   strcpy (stringa,m[indx].comp_code);
   s = (char *)malloc(strlen(filtro)+1);
   strcpy (s,filtro);

      for (k=0; k<strlen(filtro); k++)
         {
         if (filtro[k] == ' ')  /* any character      */
            continue;
         if (filtro[k] == stringa[k]) /* matching   */
            continue;
         match = -1;
         }
   if (i == strlen(stringa))
      match = -1;

   free (s);
   return (match);
}
/**********************************************************/
int lista_tipo_malf_conf (p_in,n_in,n_sel,p_sel,filtro,m)
int *p_in;           /* punt. lista elementi per check     */
int n_in;            /* n. elementi per check              */
int *n_sel;          /* n. elementi matching               */
int *p_sel;          /* punt. lista elementi matching      */
FILTRO_TIPI filtro;  /* filtro tipo                        */
M_COMPONENT *m;		/* struttura MF o FR						  */
{
int i,k;
int n;
      n = *n_sel;

      for (i=0,k=0,n=0; i<n_in; i++)
         {
         if (check_tipo_malf_conf(p_in[i],filtro,m)>0)
            {
            p_sel[k] = p_in[i];
            n++;
            k++;
            }
         }
      *n_sel = n;
      return (0);
}
/**********************************************************/
int check_tipo_malf_conf (indx, f, m)
int indx;         /* indice in database      */
FILTRO_TIPI f;
M_COMPONENT *m;      /* struttura MF o FR                  */
{
int i,k;
char label[MAX_LUN_NOME_VAR];
 
   strcpy (label,m[indx].label_main_var);
   return (test_tipo(label,f,indx));
}
/**********************************************************/
int display_item_malf_conf (w, p_in, n_in, p_disp, n_disp, dim, m, m_s, m_t)
Widget w;         /* lista destinazione            */
int *p_in;        /* punt. lista elementi per display   */
int n_in;         /* numero elementi da analizzare      */
int **p_disp;      /* punt. lista elementi in display    */
int *n_disp;      /* numero elementi in lista           */
int dim;          /* dimensione dei realloc             */
M_COMPONENT *m;      /* struttura MF o FR                  */
MALF_SET *m_s;
M_COMP_TYPE *m_t;
{
int i;
int n;

      for (i=0; i<n_in; i++)
         {
			add_lista_conf_malf (w, m, p_in[i], m_s, m_t);
         (*p_disp)[i] = p_in[i];
/*       printf ("Set p_disp : %d) %d \n",i,(*p_disp)[i]);      */
         }
      *n_disp = n_in;
      return (0);
}
/**********************************************************/
int add_item_scenario_malf (w, sMalf, n, scenario, t)
Widget w;           /* interfaccia chiamante                     */
Widget sMalf;       /* interfaccia selezMalf	(selVar)				  */
int n;              /* posizione in lista dell' item selezionato */
SCENARIO *scenario; /* scenario dell' interfaccia chiamante      */
int t;				  /* tipo malf o frem									*/
{
int indx;           /* indice in m_component                     */
int n_scen;         /* numero variabili in scenario              */
int i;
M_COMPONENT *m;
MALF_SET *m_s;
M_COMP_TYPE *m_c_t;

/*	_UxCselezMalf *Ctx = (_UxCselezMalf *)UxGetContext(sMalf);  */
	_UxCselVar *Ctx = (_UxCselVar *)UxGetContext(sMalf);

	switch (t)
		{
		case MALFUNZIONI:
		case SOMMARIO_MALF:
			m = m_component;
			m_s = malf_set;
			m_c_t = m_comp_type;
			break;
		case FUNZIONI_REMOTE:
		case SOMMARIO_FREM:
			m = fr_component;
			m_s = frem_set;
			m_c_t = fr_comp_type;
			break;
		default:
			break;
		}
/*
 * default = n_scen; si cerca comunque il primo slot libero
 */
	indx = *((Ctx->Uxpunt_malf_lista)+n-1);
/*
 * controllo per duplicazioni
 */
	if (varGiaInScenario (w,scenario,indx,m))
		return (scenario->scen_header.numvar);

	if ((indx = addMalfScenEntry (w,scenario,indx,m)) >= 0)
		{
/*
 * per default lo stato viene stabilito dalla funzione di refresh:
 *		se la var e' perturbata = ACTIVE
 *		se la var non e' perturbata = SELECTED
 */
		scenario->rec[indx].stato = -1;
		scenario->rec[indx].stato_prec = -1;
      crea_scen_malf_entry (w, indx, scenario, m, m_s, m_c_t);
		updateRigaScenario (w, indx);
		}

	return (scenario->scen_header.numvar);
}
/**********************************************************/
/*
 * controlla se la variabile e' gia in sommario
 * torna 1 se e' gia' in scenario
 * torna 0 altrimenti
 */
int varGiaInScenario (Widget w,SCENARIO *s, int p, M_COMPONENT *m)
{
int i;
char *messaggio;

	for (i=0; i<MAX_VAR_SCENARIO; i++)
      {
		if (s->rec[i].valido > 0)		/* se lo slot e' occupato */
			{
			if (!strcmp(m[p].label_main_var,s->rec[i].label))
				{
				messaggio = (char *)malloc (strlen(MALF)+
								MAX_LUN_NOME_VAR+strlen(ALREADYIN)+20);
				sprintf (messaggio,"%s: %s\n %s",
								MALF,m[p].label_main_var,ALREADYIN);
				attenzione (w,messaggio);
				free (messaggio);
				return (1);
				}
			}
		}
	return (0);
}
/**********************************************************/
/*
 * resetta la flag di malfConfig aperto
 */
int reset_MoreDataFlag (Widget w, int n)
{
_UxCscenarioMalf *Ctx = (_UxCscenarioMalf*)UxGetContext(w);

	Ctx->UxmoreDataAperto[n] = 0;
	return (0);
}
/**********************************************************/
int displayScenario (w, scenario, m, m_s, m_c_t)
Widget w;
SCENARIO *scenario; /* scenario dell' interfaccia chiamante      */
M_COMPONENT *m;
MALF_SET *m_s;
M_COMP_TYPE *m_c_t;
{
int n;
char *messaggio;

	_UxCscenarioMalf *Ctx = (_UxCscenarioMalf*)UxGetContext(w);

	for (n=0; n<MAX_VAR_SCENARIO; n++)
		{
		if (scenario->rec[n].valido > 0)
			{
			scenario->rec[n].punt_comp = checkMalfConf (m, scenario->rec[n].label);
			if (scenario->rec[n].punt_comp < 0) /* non e' piu' configurata */
				{
				scenario->rec[n].valido = -1;   /* invalida il record */
				messaggio = (char *)malloc(strlen(MALF)+
                              MAX_LUN_NOME_VAR + 20+
                              strlen(NOT_CONFIGURED));
				sprintf (messaggio,"%s : %s\n%s", 
									MALF,scenario->rec[n].label,NOT_CONFIGURED);
      		attenzione (w, messaggio);
      		free (messaggio);
				continue;
				}
/*******
			printf ("scenario %d) %s %d\n",
						n,scenario->rec[n].label,scenario->rec[n].punt_comp);
**********/
			scenario->rec[n].stato = -1;
			scenario->rec[n].stato_prec = -1;
			crea_scen_malf_entry(w,n,scenario,m,m_s,m_c_t);
			updateRigaScenario (w,n);
			}
		}
	return (0);
}
/**********************************************************/
int checkMalfConf (M_COMPONENT *m, char *label)
{
int i;
	i = 0;
	while (m[i].m_c_t_code >= 0)
		{
		if (!strcmp(label,m[i].label_main_var))   /* trovata in configurazione */
			return (i);
		i++;
		}
	return (-1);
}
/**********************************************************/
/*
 * crea e inizializza gli oggetti di una riga di scenario (o sommario)
 * 	se il tipo e' sommario non crea il tasto MORE DATA 
 *				(le info non sarebbero reperibili)
 */
int crea_scen_malf_entry (w,n,scenario,m,m_s,m_t)
Widget w;           /* nella chiamata specificare Parent(scenarioMalf)*/
int n;              /* indice in scenario (da 0 a ...)           */
SCENARIO *scenario; /* scenario dell' interfaccia chiamante      */
M_COMPONENT *m;
MALF_SET *m_s;
M_COMP_TYPE *m_t;
{
Widget BaseWidget;
int indx_m_type;
int p_malf_set[MAX_MALF_SET_VALUE];
int i;
int n_set;
float valore;
char valStr[LUN_DESCR_MALF_BREVE+20];
int tipo;
char appStr[MAX_LUN_NOME_VAR+MAX_LUN_DESCR_VAR+20];
int indx_mcomp;	/* indice in M_COMPONENT */
char tipoPert[MAX_LUN_DESCR_VAR];
int pertToggle;
float valoreScen;
int pt;


   _UxCscenarioMalf *Ctx = (_UxCscenarioMalf*)UxGetContext(w);
   BaseWidget = Ctx->UxscenMalfRc;

   tipo = Ctx->Uxtipo;

   Ctx->Uxmalf_select[n] = 0;

   Ctx->UxscenMalfEntryForm[n] = XtVaCreateManagedWidget( "form47",
         xmFormWidgetClass, BaseWidget,
         XmNresizePolicy, XmRESIZE_NONE,
         XmNx, 3,
         XmNy, 3,
         XmNwidth, 750,
         XmNheight, 35,
         NULL );
   UxPutContext( Ctx->UxscenMalfEntryForm[n], (char *) Ctx );

/* 
	3/7/97 il valore target e' 0. (NORMAL WORKING) non il valore salvato
   (l' option menu e' settato per default a 0.)
 */
   scenario->rec[n].target = 0.0;
/*
 * LABELS IN DISPLAY
 */
/*
 * Toggle buttons per selezione
 */
   Ctx->UxscenMalfTb[n] = XtVaCreateManagedWidget( "",
         xmToggleButtonWidgetClass, Ctx->UxscenMalfEntryForm[n],
         XmNtopAttachment, XmATTACH_FORM,
         XmNleftOffset, 0,
         XmNbottomAttachment, XmATTACH_FORM,
         XmNindicatorSize,20,  
         XmNmarginHeight, 0,
         XmNmarginBottom, 2,
         XmNmarginTop, 2,
         XmNalignment, XmALIGNMENT_BEGINNING,
         XmNwidth, 340,
         XmNheight, 35,
         XmNrecomputeSize, False,  
         RES_CONVERT( XmNlabelString," "),
         XmNy, 0,
         XmNx, 0,
         XmNfontList, UxConvertFontList( "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1"),
         NULL );
   UxPutContext(Ctx->UxscenMalfTb[n], (char *)Ctx);

   XtAddCallback(Ctx->UxscenMalfTb[n],
                 XmNvalueChangedCallback, sel_riga_scenario,
                 (XtPointer) &(riga_val[n]));

/* completa con la label e la descrizione     */
   indx_mcomp = scenario->rec[n].punt_comp;
/* se la var. e' configurata nel database mf  */
   if (scenario->rec[n].non_configurata == 0)
   {
      if ((indx_m_type=get_m_type_from_m_comp(indx_mcomp,m,m_t)) >=0)
/*
			sprintf (appStr,"%s %s",scenario->rec[n].label,
			m_t[indx_m_type].m_c_t_desc);
*/
         sprintf (appStr,"%s-%s-%s-%s:", scenario->rec[n].label,
                  m[indx_mcomp].comp_code,m_t[indx_m_type].m_c_type,
                  m[indx_mcomp].m_c_desc);
      else 
         sprintf (appStr,"%s %s",scenario->rec[n].label,TYPE_NOT_EXIST);
   }
   else
   {
      sprintf (appStr,"%s %s",scenario->rec[n].label,"-NOT CONFIG -");
      XtVaSetValues (Ctx->UxscenMalfTb[n],XmNsensitive,False,NULL);
   }
fprintf (stderr,"Descr [%s]\n",appStr);
	XtVaSetValues (Ctx->UxscenMalfTb[n],RES_CONVERT( XmNlabelString,appStr),NULL);

/* valore attuale : se la variabile non esiste non si da disponibile il bottone  */
/* puntatore settato in addMalfScenEntry		*/
/** 	scenario->rec[n].p_main_var = m[scenario->rec[n].punt_comp].p_main_var;   **/

   if (scenario->rec[n].p_main_var >= 0)
   {
      valore = RtDbPGetValueD (dbpunti,scenario->rec[n].p_main_var);
      sprintf (valStr,"%10.3f",valore);
   }
   else
   {
      sprintf (valStr,"-----");
      XtVaSetValues (Ctx->UxscenMalfTb[n],XmNsensitive,False,NULL);
   }

   Ctx->UxscenMalfValAtt[n] = XtVaCreateManagedWidget( "label25",
         xmLabelWidgetClass,
         Ctx->UxscenMalfEntryForm[n],
         XmNleftAttachment, XmATTACH_WIDGET,
         XmNleftWidget, Ctx->UxscenMalfTb[n],
         XmNy, 0,
       /*  XmNwidth, 80,    */
         XmNwidth, 150,
         XmNborderWidth, 0,
         XmNmarginHeight, 1,
         XmNresizeHeight, True,
         XmNbottomAttachment, XmATTACH_FORM,
         XmNtopAttachment, XmATTACH_FORM,
         XmNrecomputeSize, False,
         RES_CONVERT( XmNlabelString, valStr),
         XmNalignment, XmALIGNMENT_CENTER,
         XmNfontList, UxConvertFontList( "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1"),
         NULL );
   UxPutContext( Ctx->UxscenMalfValAtt[n], (char *) Ctx );

   Ctx->Uxmenu6_p2_shell[n] = XtVaCreatePopupShell ("menu6_p2_shell",
         xmMenuShellWidgetClass, Ctx->UxscenMalfEntryForm[n],
         XmNwidth, 1,
         XmNheight, 1,
         XmNallowShellResize, FALSE,
         XmNoverrideRedirect, TRUE,
         NULL );

   Ctx->Uxmenu6_p2[n] = XtVaCreateWidget( "menu6_p2",
         xmRowColumnWidgetClass,
         Ctx->Uxmenu6_p2_shell[n],
         XmNrowColumnType, XmMENU_PULLDOWN,
         XmNwidth, 160,
         XmNleftAttachment, XmATTACH_WIDGET,
         XmNleftWidget, Ctx->UxscenMalfValAtt[n], 
         XmNresizable, FALSE,
         XmNresizeWidth, FALSE,
/*
         XmNrightAttachment, XmATTACH_FORM,
*/
         NULL );
   UxPutContext( Ctx->Uxmenu6_p2[n], (char *) Ctx );

/*
 * OPTION MENU
 */ 
   if (scenario->rec[n].non_configurata == 0) /* se la var. e' configurata*/
      {
      if ((n_set = get_m_set_from_m_comp (scenario->rec[n].punt_comp,
		&p_malf_set[0], m,m_s)) >= 0) 
      {
         i = 0;
         while (p_malf_set[i] >= 0)
         {
            strcpy(valStr,m_s[p_malf_set[i]].m_s_descr);
            valStr[strlen(valStr)-1]='\0';
            Ctx->UxscenMalfMenuTarget[i][n]=
                  XtVaCreateManagedWidget( "menu6_p2_b1",
                  xmPushButtonWidgetClass,
         	  Ctx->Uxmenu6_p2[n],
                  XmNrecomputeSize, FALSE,
         	  RES_CONVERT( XmNlabelString,valStr  ),
                  XmNfontList, UxConvertFontList( "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1"),
                  NULL );
            UxPutContext( Ctx->UxscenMalfMenuTarget[i][n], (char *) Ctx );

            riga_val[n][i].nriga = n;
            riga_val[n][i].valore = m_s[p_malf_set[i]].m_s_value;
	
            XtAddCallback(Ctx->UxscenMalfMenuTarget[i][n],
                          XmNactivateCallback, act_malf_scenario, 
                          (XtPointer) &(riga_val[n][i]));

            i++;
         }
      }
   }
   else
   {
      Ctx->UxscenMalfMenuTarget[0][n]=
            XtVaCreateManagedWidget( "menu6_p2_b1",
            xmPushButtonWidgetClass,
            Ctx->Uxmenu6_p2[n],
            RES_CONVERT( XmNlabelString,"NOT AVAIL."),
            NULL );
      UxPutContext( Ctx->UxscenMalfMenuTarget[i][n], (char *) Ctx );
   }

   Ctx->Uxmenu6[n] = XtVaCreateManagedWidget( "menu6",
         xmRowColumnWidgetClass,
         Ctx->UxscenMalfEntryForm[n],
         XmNrowColumnType, XmMENU_OPTION,
         XmNsubMenuId, Ctx->Uxmenu6_p2[n],
         XmNleftAttachment, XmATTACH_WIDGET,
         XmNleftWidget, Ctx->UxscenMalfValAtt[n],
         XmNwidth, 160,
         XmNmarginHeight, 1,
         XmNresizable, TRUE,
         XmNresizeWidth, FALSE,
         NULL );
   UxPutContext( Ctx->Uxmenu6[n], (char *) Ctx );

/* se e' configurata nel database mf e la var princ. esiste */
   if ((scenario->rec[n].non_configurata == 0) && 
       (scenario->rec[n].p_main_var >= 0))
   { 
/* bottone per espansione colloquio in caso di variabili secondarie */
      Ctx->UxscenMalfMoreData[n] = XtVaCreateManagedWidget( "MoreDataPb",
            xmPushButtonWidgetClass,
            Ctx->UxscenMalfEntryForm[n],
/*
         XmNrightAttachment, XmATTACH_FORM,
         XmNrightOffset, 5,
*/
            XmNleftAttachment  , XmATTACH_WIDGET,
            XmNleftWidget,Ctx->Uxmenu6[n],
            XmNleftOffset, 5,
            XmNwidth, 40,
            XmNmarginHeight, 1,
            RES_CONVERT( XmNlabelString, "..." ),
            XmNsensitive, True,
            NULL );

      UxPutContext( Ctx->UxscenMalfMoreData[n], (char *) Ctx );
                    Ctx->UxitemExpand[n] = n;
      XtAddCallback(Ctx->UxscenMalfMoreData[n],
                    XmNactivateCallback, openMalfConfig,
                    (XtPointer) n);
            /*   (XtPointer) &(Ctx->UxitemExpand[n]));   */
		} 
/* carica in scenario i puntatori delle var secondarie */
	scenario->rec[n].p_sec1_var = m[indx_mcomp].p_sec_var_1;
	scenario->rec[n].p_sec2_var = m[indx_mcomp].p_sec_var_2;
	scenario->rec[n].p_sec3_var = m[indx_mcomp].p_sec_var_3;
	scenario->rec[n].p_sec4_var = m[indx_mcomp].p_sec_var_4;

/* carica i tipi pert (i valori target sono i defaults)		*/
/* 
	modifica del 3/7/97 il valore target di default e' il valore in database
   al momento dell' istanziazione ( per evitare emissioni di pert sec. non
   volute). Pertanto la memorizzazione dei valori di default in scenario
   non e' efficace.
	Il concetto e' stato esteso anche al momento del caricamento CI o BT:
	in seguito a reload database vengono ricalcolate le variabili secondarie
   delle malf in display (scen e somm) (vedere updateRigaSommario/Scenario).
	Vengono inoltre azzerati i delay e la durata.
   Tutto cio' per far si che l' istruttore sia protetto dalla possibilita'
	di mandare perturbazioni indesiderate o non espressamente richieste
	tramite le pert secondarie. Fatto dopo le prove con DM il 3-4/7/97. fm
 */
	pt = getCompType (m_t,m[indx_mcomp].m_c_t_code);
	if (scenario->rec[n].p_sec1_var >= 0)
		{
		strcpy (tipoPert,m_t[pt].pertSec1);
		scenario->rec[n].tipoPert1 = assegnaTipoPert (tipoPert);		
/*		scenario->rec[n].valSec1 = (float)atof(m_t[pt].valPertSec1);  */
		scenario->rec[n].valSec1 = RtDbPGetValueD (dbpunti,scenario->rec[n].p_sec1_var);
		if (scenario->rec[n].tipoPert1 < 0)  /* tipo pert non esiste   */
			{
			fprintf (stderr,"Tipo Pert %s non esiste (%s)\n",tipoPert,m[indx_mcomp].label_sec_var_1);
			scenario->rec[n].p_sec1_var = -1;
			}
		}
   if (scenario->rec[n].p_sec2_var >= 0)
      {
      strcpy (tipoPert,m_t[pt].pertSec2);
      scenario->rec[n].tipoPert2 = assegnaTipoPert (tipoPert);
/*		scenario->rec[n].valSec2 = (float)atof(m_t[pt].valPertSec2);*/
		scenario->rec[n].valSec2 = RtDbPGetValueD (dbpunti,scenario->rec[n].p_sec2_var);
      if (scenario->rec[n].tipoPert2 < 0)  /* tipo pert non esiste   */
         {
         fprintf (stderr,"Tipo Pert %s non esiste (%s)\n",tipoPert,m[indx_mcomp].label_sec_var_2);
         scenario->rec[n].p_sec2_var = -1;
         }
      }
   if (scenario->rec[n].p_sec3_var >= 0)
      {
      strcpy (tipoPert,m_t[pt].pertSec3);
      scenario->rec[n].tipoPert3 = assegnaTipoPert (tipoPert);
/*		scenario->rec[n].valSec3 = (float)atof(m_t[pt].valPertSec3);*/
		scenario->rec[n].valSec3 = RtDbPGetValueD (dbpunti,scenario->rec[n].p_sec3_var);
      if (scenario->rec[n].tipoPert3 < 0)  /* tipo pert non esiste   */
         {
         fprintf (stderr,"Tipo Pert %s non esiste (%s)\n",tipoPert,m[indx_mcomp].label_sec_var_3);
         scenario->rec[n].p_sec3_var = -1;
         }
      }
   if (scenario->rec[n].p_sec4_var >= 0)
      {
      strcpy (tipoPert,m_t[pt].pertSec4);
      scenario->rec[n].tipoPert4 = assegnaTipoPert (tipoPert);
/*		scenario->rec[n].valSec4 = (float)atof(m_t[pt].valPertSec4);    */
		scenario->rec[n].valSec4 = RtDbPGetValueD (dbpunti,scenario->rec[n].p_sec4_var);
      if (scenario->rec[n].tipoPert4 < 0)  /* tipo pert non esiste   */
         {
         fprintf (stderr,"Tipo Pert %s non esiste (%s)\n",tipoPert,m[indx_mcomp].label_sec_var_4);
         scenario->rec[n].p_sec4_var = -1;
         }
      }
/* 
 *	azzera delay e durata
 */
	scenario->rec[n].delay = 0.;
	scenario->rec[n].durata = 0.;
	scenario->rec[n].durataInf = 0; /* default durata infinita   */
	scenario->rec[n].time_to = 0.;
			
	return (0);
}
/*********************************************/
void openMalfConfig (w, data, reason)
Widget w;
/* char *data;   */
int *data;
XmPushButtonCallbackStruct *reason;
{
int n;
_UxCscenarioMalf *Ctx;
   Ctx = (_UxCscenarioMalf *)UxGetContext (w);
	n = (int)data;
   if (Ctx->UxmoreDataAperto[n] == 0)
      {
      create_malfConfig (Ctx->UxscenarioMalf,n,Ctx->Uxtipo);
      Ctx->UxmoreDataAperto[n] = 1;
      }
}
/********************************************************************/
/*
 * ritorna il testo da inserire nella lista malf
 * funzione usata anche per find
 *
 *	m		struttura delle istanze M_COMPONENT
 *	im		indice della istanza in M_COMPONENT
 *	m_t	struttura delle classi M_COMP_TYPE
 */
char *creaTestoMalfList (M_COMPONENT *m,int im,M_COMP_TYPE *m_t)
{
int comp_type_i;		/*indice in m_t  */
char testo[1000];

	if ((comp_type_i = get_m_type_from_m_comp(im,m,m_t)) < 0)
		{
		sprintf( testo, "%s - UNKNOWN MALF. CLASS - %s",
               m[im].label_main_var,
               m[im].comp_code);
		}
	else
		sprintf( testo, "%s - %s - %s - %s",
               m[im].label_main_var,
					m_t[comp_type_i].m_c_type, 
					m_t[comp_type_i].m_c_t_desc,
					m[im].comp_code);

	return (testo);
}
/********************************************************************/
int get_m_type_from_m_comp (n,m,m_t)
int n;   /* puntatore in m_component          */
M_COMPONENT *m;
M_COMP_TYPE *m_t;
{
int i;

	i = 0;
	while (m_t[i].m_c_t_code >= 0)
      {
		if (m_t[i].m_c_t_code == m[n].m_c_t_code)
			return (i);
		i++;
		}
	return (-1);
}
/********************************************************************/
int get_m_set_from_m_comp (n,p,m,m_s)
int n;   /* puntatore in m_component          */
int *p;  /* array ai puntatori in malf_set    */
M_COMPONENT *m;
MALF_SET *m_s;
{
int i,k;

   i = 0;
	k = 0;
   while (m_s[i].m_c_t_code >= 0)
      {
      if (m_s[i].m_c_t_code == m[n].m_c_t_code)
			{
			p[k] = i;
			k++;
			}
      i++;
      }
	p[k] = -1;  /* tappo finale  */

   if (k == 0)
		return (-1);
	return (k);
}
/********************************************************************/
/*
 * gestione malfunzione da scenario 
 */
void act_malf_scenario (w, data, reason)
Widget w;
char *data;
XmPushButtonCallbackStruct *reason;
{
int valore;
int riga;
int riga_select;
int sfondo;
RIGA_VAL *r_v;

	_UxCscenarioMalf *Ctx = (_UxCscenarioMalf*)UxGetContext(w);
	riga_select = (int )Ctx->Uxscen_malf_select;
	sfondo   = (int)Ctx->Uxcol_sfondo_scenMalf;
	
	r_v = (RIGA_VAL *)data;
	switch (reason->reason)
      {
      case XmCR_ACTIVATE:
         valore  = r_v->valore;
         riga  = r_v->nriga;
         printf ("nriga %d target = %d\n",riga, valore);
			Ctx->Uxscenario.rec[riga].target = (float)valore;
         break;
		}
	return ;
}
/********************************************************************/
/*
 * gestione malfunzione da scenario
 */
void sel_riga_scenario (w, data, reason)
Widget w;
char *data;
XmToggleButtonCallbackStruct *reason;
{
int riga;
int riga_select;
int sfondo;
SCENARIO *scenario;
RIGA_VAL *r_v;
Boolean stato;


   _UxCscenarioMalf *Ctx = (_UxCscenarioMalf*)UxGetContext(w);
   scenario = (SCENARIO *)&Ctx->Uxscenario;
	riga_select = (int )Ctx->Uxscen_malf_select;
	sfondo   = (int)Ctx->Uxcol_sfondo_scenMalf;

	stato = reason->set;   /* stato del toggle */

	r_v = (RIGA_VAL *)data;

   riga  = r_v->nriga;
/*   printf ("selez. riga %d\n",riga);  */

	if (stato)
		Ctx->Uxmalf_select[riga] = 1;
	else
      Ctx->Uxmalf_select[riga] = 0;

	set_sensitive_scenario (w);
   return ;
}
/******************************************************************/
int set_sensitive_scenario (w)
Widget w;
{
   _UxCscenarioMalf *Ctx = (_UxCscenarioMalf*)UxGetContext(w);
int i;

	for (i=0; i<MAX_VAR_SCENARIO; i++)
		{
		if (Ctx->Uxmalf_select[i] > 0) /* almeno 1 selezionato */
			{
			XtVaSetValues (Ctx->UxscenMalfEditDel,XmNsensitive,True,NULL);
			XtVaSetValues (Ctx->UxscenMalfPopupDel,XmNsensitive,True,NULL);
			return (0);
			}
		}
   XtVaSetValues (Ctx->UxscenMalfEditDel,XmNsensitive,False,NULL);
   XtVaSetValues (Ctx->UxscenMalfPopupDel,XmNsensitive,False,NULL);

	return (0);
}
/******************************************************************/
int set_colore_riga_scenario (w, n, c)
Widget w;
int n;   /* numero di riga */
int c;   /* colore         */
{
	_UxCscenarioMalf *Ctx = (_UxCscenarioMalf*)UxGetContext(w);
	
/*   XtVaSetValues (Ctx->UxscenMalfMainLabel[n], XmNbackground, c, NULL); */
/*   XtVaSetValues (Ctx->UxscenMalfTipoComp[n], XmNbackground, c, NULL); */
/*   XtVaSetValues (Ctx->UxscenMalfDescr[n], XmNbackground, c, NULL); */
	XtVaSetValues (Ctx->UxscenMalfTb[n], XmNbackground, c, NULL);
   return(0);
}
/******************************************************************/
int delete_item_scen_malf (w)
Widget w;
{
int n;

	_UxCscenarioMalf *Ctx = (_UxCscenarioMalf*)UxGetContext(w);

for (n=0; n<MAX_VAR_SCENARIO;n++)
	{
	if (Ctx->Uxscenario.rec[n].valido > 0)
      {
		printf ("%d valido (%d)\n",n,Ctx->Uxscenario.rec[n].valido);
		if (XmToggleButtonGetState(Ctx->UxscenMalfTb[n]))
			{
			printf ("stato = %d\n",XmToggleButtonGetState(Ctx->UxscenMalfTb[n]));
			eliminaItemScenario (w, n);   /* elimina la entry dallo scenario */
			removeScenMalfEntry (w, n);   /* elimina la entry dalla grafica  */
			set_sensitive_scenario (w);
			}
		}
	else
		printf ("item %d non occupato\n",n);
	}
	return (0);
}
/*************************************************************/
int eliminaItemScenario (Widget w, int item)
{
_UxCscenarioMalf *Ctx = (_UxCscenarioMalf*)UxGetContext(w);

	Ctx->Uxscenario.rec[item].valido = -1;
	Ctx->Uxscenario.rec[item].non_configurata = 0;
	Ctx->Uxmalf_select[item] = 0;
	Ctx->Uxscenario.scen_header.numvar--;
	
	return (Ctx->Uxscenario.scen_header.numvar);
}
/*************************************************************/
int removeScenMalfEntry (Widget w, int item)
{
_UxCscenarioMalf *Ctx = (_UxCscenarioMalf*)UxGetContext(w);
	DistruggiInterfaccia (Ctx->UxscenMalfEntryForm[item]);
}
/*************************************************************/
/*
 * salva lo scenario composto dall' interfaccia scenarioMalf/Frem
 */
int save_scenmalf (w, tipo, n)
Widget w;         /* chiamante        						*/
int tipo;  			/* interfaccia chiamante      		*/
int n;            /* indice in lista dello scenario 	*/
{
char *messaggio;
SCENARI *s;


	_UxCscenarioMalf *Ctx = (_UxCscenarioMalf*)UxGetContext(w);

   messaggio = (char *)malloc(strlen(SAVE_FILE_SCEN)+
                              20+
                              strlen(NOT_ACCESSIBLE));
	
   set_cursor (w,CLOCK);

	switch (tipo)
		{
		case MALFUNZIONI:
		case SOMMARIO_MALF:
			s = &scenari_malf;
			break;
		case FUNZIONI_REMOTE:
		case SOMMARIO_FREM:
			s = &scenari_frem;
			break;
		}
	strcpy (&Ctx->Uxscenario.scen_header.commento[0],
					create_dataRequest (Ctx->UxscenarioMalf,
												MALF_SCEN_TITLE,
												&Ctx->Uxscenario.scen_header.commento[0]));
	strcpy (&Ctx->Uxscenario.scen_header.sessione[0], titolo_sessione);
	copia_scenario (w,s,&(Ctx->Uxscenario),n);

	if (SD_savescenari(BANCO,tipo,s) < 0)
		{
		printf ("save_scenmalf scrittura fallita\n");
   	sprintf (messaggio,"%s %s", SAVE_FILE_SCEN,NOT_ACCESSIBLE);
      attenzione (w, messaggio);
      free (messaggio);
   	set_cursor (w,NORMALE);
      return (-1);
		}

	update_title (Ctx->UxscenarioMalf,Ctx->Uxscenario.scen_header.commento);

   sprintf (messaggio,"%s ", SAVE_FILE_SCEN);
   set_cursor (w,NORMALE);
   add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);
   return (0);
}
/*************************************************************/
/*
 * salva gli scenari dopo editing dall' interfaccia listaScenari
 */
int salvaScenariMalf (Widget w, int tipo)
{
char *messaggio;
SCENARI *s;


   messaggio = (char *)malloc(strlen(SAVE_FILE_SCEN)+
                              20+
                              strlen(NOT_ACCESSIBLE));
   set_cursor (w,CLOCK);
	
	switch (tipo)
		{
		case MALFUNZIONI:
			s = &scenari_malf;
			break;
		case FUNZIONI_REMOTE:
			s = &scenari_frem;
			break;
		}
	if (SD_savescenari(BANCO,tipo,s) < 0)
      {
      sprintf (messaggio,"%s %s", SAVE_FILE_SCEN,NOT_ACCESSIBLE);
      attenzione (w, messaggio);
      free (messaggio);
      set_cursor (w,NORMALE);
      return (-1);
      }
   set_cursor (w,NORMALE);
   sprintf (messaggio,"%s ", SAVE_FILE_SCEN);
   add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);
   return (0);
}
/*************************************************************/
int copia_scenario (w,scenari,scenario,n)
Widget w;
SCENARI *scenari;
SCENARIO *scenario;
int n;
{
int i;
int indx;

	printf ("numero di scenari registrati = %d\n",scenari->n);
	if (n < 0) /* il primo libero */
		{
		for (i=0; i<MAX_SCENARI; i++)
			{
			if (scenari->valido[i] < 0) /* record libero */
				{
				printf ("copia scenario in record %d\n",i);
				memcpy (&(scenari->s[i]),scenario,sizeof(SCENARIO));
				scenari->valido[i] = 1;
				scenari->n++;
				break;
				}
			}
		if (i == MAX_SCENARI)
			{
			printf ("ERRORE nessuno scenario libero\n");
			}
		}
	else
		{
		if (scenari->valido[n-1] < 0) /* record libero */
         {
         printf ("copia scenario in record %d\n",n-1);
         memcpy (&(scenari->s[n-1]),scenario,sizeof(SCENARIO));
         scenari->valido[n-1] = 1;
         scenari->n++;
         }
		else    /* ricopre scenario esistente  */
			{
			printf ("copia scenario in record occupato %d\n",n-1);
         memcpy (&(scenari->s[n-1]),scenario,sizeof(SCENARIO));
         scenari->valido[n-1] = 1;
			}
		}
/**
for (i=0; i<MAX_VAR_SCENARIO; i++)
	{
	if (scenario->rec[i].valido > 0)
		printf ("copia_scenario: %d)  %s delay = %f valSec1 %f valSec2 %f\n",
					i,scenario->rec[i].label,scenario->rec[i].delay,
						scenario->rec[i].valSec1,scenario->rec[i].valSec2); 
		printf ("copia_scenario: %d)  %s delay = %f valSec1 %f valSec2 %f\n",
					i,scenari->s[n-1].rec[i].label,scenari->s[n-1].rec[i].delay,
						scenari->s[n-1].rec[i].valSec1,scenari->s[n-1].rec[i].valSec2);
	}
**/
	return (0);
}
/*************************************************************/
/*
 * rinfresca il SOMMARIO malf in funzione delle pert attive
 *	o che sono terminate
 */
int refreshSommarioMalf (Widget w, SCENARIO *s, TIPO_PERT *p)
{
/*
 * scansione delle pert attive
 * identificazione delle malf.
 * controllo esistenza in sommario ed eventuale aggiunta
 * eliminazione delle entries prima attive  e non piu' nella lista pert
 */
_UxCscenarioMalf *Ctx;
int i,k;
int punt;
int trovata;
char *messaggio;
M_COMP_TYPE *m_c_t;
MALF_SET *m_set;
M_COMPONENT *m;

   Ctx = (_UxCscenarioMalf *)UxGetContext(w);
   switch (Ctx->Uxtipo)
      {
      case MALFUNZIONI:
      case SOMMARIO_MALF:
         m_c_t = m_comp_type;
         m_set = malf_set;
         m = m_component;
         break;
      case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         m_c_t = fr_comp_type;
         m_set = frem_set;
         m = fr_component;
         break;
      }
/*
 * controllo per aggiunta di entries
 */
   for (i=0; i<_MAX_PERTUR*2 ; i++ )
      {
      if ((p[i].tipo == PERT_MALFUNCTION) ||
          (p[i].tipo == PERT_MALFUNCTION_STOP))
         {
/*	printf ("refreshSommarioMalf: pert %d = tipo %d ******MALF*****\n",
		i,p[i].tipo); **/
/*
 * solo se e' in configurazione
 */
/* se punt < 0 la variabile non e' configurata nel database malfunzioni */

         punt = malfCurrentlyActive(p, i, m);

/** printf ("\t\t trovata in database indice %d\n",punt); **/
/* controlla se era gia' in elenco (controllo sui puntatori) */
         trovata = 0;
         for (k=0; k<MAX_VAR_SCENARIO; k++)
            {
            if (s->rec[k].valido > 0)
               {
               if (s->rec[k].p_main_var == p[i].indirizzo)
                  {
/*  fprintf (stderr,"PERT [%s] gia in elenco \n", s->rec[k].label);  */
                  trovata = 1;
                  break;
                  }
               }
            }
         if (!trovata)	/* e' valida e non in display	*/
            {
            if (punt >= 0)	/* la variabile e' configurata	*/
               {
               if ((punt = addMalfScenEntry (w,s,punt,m)) < 0)
                  {
                  messaggio = (char *)malloc(strlen(NO_MORE_ENTRIES)+
                                             20+ MAX_LUN_NOME_VAR);
                  sprintf (messaggio,"%s %s",
                           NO_MORE_ENTRIES,m[punt].label_main_var);
                  attenzione (w, messaggio);
                  free (messaggio);
                  continue;
                  } 
               }
            else   /* non e' configurata */
               {
               if ((punt = addMalfScenEntryDummy (w,s,p[i].indirizzo)) < 0)
                  { 
                  messaggio = (char *)malloc(strlen(NO_MORE_ENTRIES)+
                                                20+ MAX_LUN_NOME_VAR);
                  sprintf (messaggio,"%s %s",
                           NO_MORE_ENTRIES,m[punt].label_main_var);
                  attenzione (w, messaggio);
                  free (messaggio);
                  continue;
                  }
               }
/* per default le new entries in sommario sono attive */
               s->rec[punt].stato = ACTIVE;
/* fprintf (stderr,"refreshSommarioMalf: crea entry in item %d\n",punt); */
               crea_scen_malf_entry (w,punt,s,m,m_set,m_c_t);
               updateRigaSommario (w, punt);  
            }
         }
      }
/*
 * controllo per eliminazione di entries
 */
	for (k=0; k<MAX_VAR_SCENARIO; k++)
   	{
		if (s->rec[k].valido > 0)
			{
			if ((malfPertInCorso (s->rec[k].p_main_var,p,PERT_MALFUNCTION) < 0) &&
             (malfPertInCorso (s->rec[k].p_main_var,p,PERT_MALFUNCTION_STOP) < 0))
				{     /* e' in display ma non piu in lista, da eliminare */
                  /* fprintf (stderr,"PERT [%s] gia in elenco \n", s->rec[k].label); */
				eliminaItemScenario (w, k);   /* elimina la entry dallo scenario */
         	removeScenMalfEntry (w, k);   /* elimina la entry dalla grafica  */
				}
			}
		}
}
/*************************************************************/
/*
 * ritorna l' indice in struttura TIPO_PERT di un indirizzo se perturbato
 *		-1 in caso non sia perturbato
 */
int malfPertInCorso (int addr, TIPO_PERT *p, int tipo)
{
int i;
int punt;
	punt = -1;
	for (i=0; i<_MAX_PERTUR*2 ; i++ )
		{
		if ((p[i].tipo == PERT_MALFUNCTION) ||
                     (p[i].tipo == PERT_MALFUNCTION_STOP))
      	{
         if (addr == p[i].indirizzo)
            {
            punt = i;
            break;
            }
         }
      }
	return (punt);
}
/*************************************************************/
/*
 * se la richiesta proviene da uno scenario carica da file (scenari)
 * 	se invece proviene da sommario carica dalle perturbazioni
 */
int caricaScenario (w,s,scenario,tipo,n)
Widget w;					/* interfaccia chiamante				*/
SCENARI *s;             /* scenari in memoria					*/
SCENARIO *scenario;
int tipo;					 /* indica la richiesta					*/
int n;                   /* numero dello scenario in lista */
{
int i;
int count;
int esito = 0;
int punt;
char *messaggio;

printf ("carica_scenario tipo = %d nscen = %d \n",tipo,n);
	switch (tipo)
		{
		case MALFUNZIONI:
		case FUNZIONI_REMOTE:
			if (s->valido[n-1] >= 0)
				{
				memcpy (scenario, &(s->s[n-1]),sizeof(SCENARIO));
      		printf ("richiesto scenario n. %d\n",n);
				}
			else
				esito = -1;
			break;
		case SOMMARIO_MALF:
                   for (i=0; i<_MAX_PERTUR*2 ; i++ )
      		      {
                         if ((pertMalf[i].tipo == PERT_MALFUNCTION) || 
                             (pertMalf[i].tipo == PERT_MALFUNCTION_STOP))
         		    {
printf ("CARICA_SCENARIO: pert %d = tipo %d ******MALF*****\n",i,pertMalf[i].tipo);
                               if ((punt = malfCurrentlyActive(pertMalf,i, m_component)) >= 0)
						{
printf ("\t\t trovata in database indice %d\n",punt);
                                  if ((punt = addMalfScenEntry (w,scenario,punt,m_component)) < 0)
                                     {
                                        messaggio = (char *)malloc(strlen(NO_MORE_ENTRIES)+
                              20+ MAX_LUN_NOME_VAR);
					sprintf (messaggio,"%s %s", 
											NO_MORE_ENTRIES,m_component[punt].label_main_var);
      					attenzione (w, messaggio);
      					free (messaggio);
                                     }
                                  }
                              }
                          }
			break;
		case SOMMARIO_FREM:
			for (i=0; i<_MAX_PERTUR*2 ; i++ )
            {
            if ((pertMalf[i].tipo == PERT_MALFUNCTION) ||
                     (pertMalf[i].tipo == PERT_MALFUNCTION_STOP))
               {
printf ("CARICA_SCENARIO: pert %d = tipo %d ******MALF*****\n",i,pertMalf[i].tipo);
               if ((punt = malfCurrentlyActive(pertMalf,i, fr_component)) >= 0)
                  {
printf ("\t\t trovata in database indice %d\n",punt);
                  if ((punt = addMalfScenEntry (w,scenario,punt,fr_component)) < 0)
                     {
                     messaggio = (char *)malloc(strlen(NO_MORE_ENTRIES)+
                              20+ MAX_LUN_NOME_VAR);
                     sprintf (messaggio,"%s %s",
                                 NO_MORE_ENTRIES,fr_component[punt].label_main_var);
                     attenzione (w, messaggio);
                     free (messaggio);
                     }
                  }
               }
            }
			break;
		}
/*******
for (i=0; i<MAX_VAR_SCENARIO; i++)
   {
   if (scenario->rec[i].valido > 0)
      printf ("caricaScenario: %d)  %s delay = %f valSec1 %f valSec2 %f\n",
               i,scenario->rec[i].label,scenario->rec[i].delay,
                  scenario->rec[i].valSec1,scenario->rec[i].valSec2); 
   }
**************/
	return (esito);
}
/******************************************************************/
/*
 * analizza l' immagine delle perturbazioni attive e controlla
 * se ce ne sono di tipo malfunzione
 *	ritorna il puntatore alla struttura M_COMPONENT o -1
 */
int malfCurrentlyActive (TIPO_PERT *p,int n, M_COMPONENT *m)
{
int k;

   k = 0;
   while (m[k].m_c_t_code >= 0)  /* check sulla sola var. principale */
      {
      if (p[n].indirizzo == m[k].p_main_var) /* trovata */
         return (k);
      k++;
      }
printf ("malfCurrentlyActive: var %d non configurata\n",p[n].indirizzo);
	return (-1);
}
/******************************************************************/
/*
 * aggiunge un item alla struttura scenario
 *	la entry e' relativa all' indirizzo in memoria e non alla configurazione
 *		(che non c'e')
 * p = puntatore in memoria della variabile
 * ritorna l' indice in scenario
 */
int addMalfScenEntryDummy (Widget w,SCENARIO *scenario, int p)
{
int n_scen;
   if ((n_scen = primoScenarioLibero (scenario)) >= 0)
      {
      scenario->rec[n_scen].punt_comp = p;
      strcpy (scenario->rec[n_scen].label, variabili[get_addr_indx(p)].nome);
      scenario->rec[n_scen].p_main_var = p;	/* puntatore in memoria */
      scenario->rec[n_scen].delay = 0.;
      scenario->rec[n_scen].durata = 0.;
      scenario->rec[n_scen].durataInf = 0; /* default durata infinita   */
      scenario->rec[n_scen].time_to = 0.;
      scenario->rec[n_scen].target = 0.;
      scenario->rec[n_scen].valido = 1;
   	scenario->rec[n_scen].non_configurata = 1; /* non e' configurata */
      scenario->rec[n_scen].stato = SELECTED;
      scenario->rec[n_scen].stato_prec = SELECTED;

      scenario->scen_header.numvar++;
printf ("Aggiunta malf non config. [%s] in slot %d (p=%d)\n",
					scenario->rec[n_scen].label,n_scen,p);
      }
   return (n_scen);
}
/******************************************************************/
/*
 * aggiunge un item alla struttura scenario
 *	p = puntatore alla struttura M_COMPONENT
 * ritorna l' indice in scenario
 */
int addMalfScenEntry (Widget w,SCENARIO *scenario, int p, M_COMPONENT *m)
{
int n_scen;
	if ((n_scen = primoScenarioLibero (scenario)) >= 0)
		{
		scenario->rec[n_scen].punt_comp = p;
   	strcpy (scenario->rec[n_scen].label,m[p].label_main_var);
		scenario->rec[n_scen].p_main_var = m[p].p_main_var;
   	scenario->rec[n_scen].delay = 0.;
   	scenario->rec[n_scen].durata = 0.;
   	scenario->rec[n_scen].durataInf = 0; /* default durata infinita   */
   	scenario->rec[n_scen].time_to = 0.;
   	scenario->rec[n_scen].target = 0.;
   	scenario->rec[n_scen].valido = 1;
   	scenario->rec[n_scen].non_configurata = 0; /* e' configurata */
   	scenario->rec[n_scen].stato = SELECTED;
   	scenario->rec[n_scen].stato_prec = SELECTED;
	
   	scenario->scen_header.numvar++;
printf ("Aggiunta malf [%s] in slot %d\n",scenario->rec[n_scen].label,n_scen);
		}
	return (n_scen);
}
/******************************************************************/
/* 
 * ritorna il primo slot libero dello scenario
 */
int primoScenarioLibero (SCENARIO *s)
{
int n_scen;
int i;
/*
 * default = n_scen; si cerca comunque il primo slot libero
 */
   n_scen = s->scen_header.numvar;
   for (i=0; i<n_scen; i++)
      {
      if (s->rec[i].valido <= 0) /* record libero   */
         {
         n_scen = i;
         break;
         }
      }
	if (n_scen >= MAX_VAR_SCENARIO)
		n_scen = -1;						/* tutti gli slots occupati	*/
	return (n_scen);
}
/******************************************************************/
int uscita_listaScenari (w,t)
Widget w;
int t;
{
	switch (t)
		{
		case MALFUNZIONI:
			DistruggiInterfaccia (XtParent(w));
/****** azioni svolte dalla destroyCallback
			lista_scenari_malf_active = 0;
			stop_timer (timer_listaScenMalf);
			remove_timer_editPertMalf ();
********/
			break;
		case FUNZIONI_REMOTE:
			DistruggiInterfaccia (XtParent(w));
/********
			lista_scenari_frem_active = 0;
			stop_timer (timer_listaScenFrem);
			remove_timer_editPertMalf ();
**********/
			break;
		}
}
/******************************************************************/
/*
 * genera i campi relativi alle variabili secondarie
 */
int creaCampoVarSec (Widget Wconf, Widget Wscen,
      M_COMPONENT *m,int pm,
      M_COMP_TYPE *m_t,int pt,
      int n,             /* indice di scenario		*/
      int numVar)
{
_UxCscenarioMalf *CtxScenMalf;
_UxCmalfConfig *CtxMalfConf;
char label[MAX_LUN_NOME_VAR];
char descrizione[MAX_LUN_DESCR_VAR];
int addr;
Widget Wbase;
Widget Separator;
Widget *Wlabel;
Widget *Wmod;
Widget *Wdescr;
Widget *Wval;
Widget *Wtoggle;
Widget *Wform;
Widget *Wvalact;
Widget *Wvalactlabel;
float valore;
char valStr[20];
Boolean editable;
Boolean valToggle;
char tipoPert[MAX_LUN_DESCR_VAR];
float valoreScen;
int pertToggle;


   CtxScenMalf = (_UxCscenarioMalf *) UxGetContext (Wscen);
   CtxMalfConf = (_UxCmalfConfig *) UxGetContext (Wconf);
   Wbase = CtxMalfConf->UxsecVarRc;	/* RowColumn alla quale si aggiungono le entries */
	
/*
 * assegnazione variabili
 */
   switch (numVar)      /* indice della variabile secondaria         */
      {
      case 1:
         strcpy (label,m[pm].label_sec_var_1);
         strcpy (descrizione,sintesiDescr(variabili[m[pm].ind_sec_var_1].descr));
         strcpy (tipoPert,m_t[pt].pertSec1);
         addr = CtxScenMalf->Uxscenario.rec[n].p_sec1_var;
         if (!strcmp(tipoPert,"PERT_NOT"))   /* e' una variabile toggle	*/
            CtxMalfConf->UxpertToggle1 = 1;
         CtxScenMalf->Uxscenario.rec[n].tipoPert1 = assegnaTipoPert (tipoPert);
         if (CtxScenMalf->Uxscenario.rec[n].tipoPert1 < 0)
            {
            fprintf (stderr,"Tipo Pert %s non esiste (%s)\n",tipoPert,label);
            addr = -1;
            }
         pertToggle = CtxMalfConf->UxpertToggle1;
         Wform = &CtxMalfConf->UxSecVar1;
         Wlabel= &CtxMalfConf->UxsecVar1Label;
         Wmod  = &CtxMalfConf->UxsecVar1Mod;
         Wdescr= &CtxMalfConf->UxsecVar1Descr;
         Wval  = &CtxMalfConf->UxsecVar1ValTf;
         Wtoggle = &CtxMalfConf->UxsecVar1Tb;
         Wvalact = &CtxMalfConf->UxsecVar1ValAct;
         Wvalactlabel = &CtxMalfConf->UxsecVar1ValActLab;
         valoreScen = CtxScenMalf->Uxscenario.rec[n].valSec1;
         if (strcmp(label,""))   /* se esiste */
            CtxMalfConf->UxvarSecExist1 = 1;
         break;
      case 2:
         strcpy (label,m[pm].label_sec_var_2);
         strcpy (descrizione,sintesiDescr(variabili[m[pm].ind_sec_var_2].descr));
         strcpy (tipoPert,m_t[pt].pertSec2);
         addr = CtxScenMalf->Uxscenario.rec[n].p_sec2_var;
         if (!strcmp(tipoPert,"PERT_NOT"))   /* e' una variabile toggle	*/
            CtxMalfConf->UxpertToggle2 = 1;
         CtxScenMalf->Uxscenario.rec[n].tipoPert2 = assegnaTipoPert (tipoPert);
         if (CtxScenMalf->Uxscenario.rec[n].tipoPert2 < 0)
            {
            fprintf (stderr,"Tipo Pert %s non esiste (%s)\n",tipoPert,label);
            addr = -1;
            }
         pertToggle = CtxMalfConf->UxpertToggle2;
         Wform = &CtxMalfConf->UxSecVar2;
         Wlabel= &CtxMalfConf->UxsecVar2Label;
         Wmod  = &CtxMalfConf->UxsecVar2Mod;
         Wdescr= &CtxMalfConf->UxsecVar2Descr;
         Wval  = &CtxMalfConf->UxsecVar2ValTf;
         Wtoggle = &CtxMalfConf->UxsecVar2Tb;
         Wvalact = &CtxMalfConf->UxsecVar2ValAct;
         Wvalactlabel = &CtxMalfConf->UxsecVar2ValActLab;
         valoreScen = CtxScenMalf->Uxscenario.rec[n].valSec2;
         if (strcmp(label,""))   /* se esiste */
            CtxMalfConf->UxvarSecExist2 = 1;
         break;
      case 3:
         strcpy (label,m[pm].label_sec_var_3);
         strcpy (descrizione,sintesiDescr(variabili[m[pm].ind_sec_var_3].descr));
         strcpy (tipoPert,m_t[pt].pertSec3);
         addr = CtxScenMalf->Uxscenario.rec[n].p_sec3_var;
         if (!strcmp(tipoPert,"PERT_NOT"))   /* e' una variabile toggle	*/
            CtxMalfConf->UxpertToggle3 = 1;
         CtxScenMalf->Uxscenario.rec[n].tipoPert3 = assegnaTipoPert (tipoPert);
         if (CtxScenMalf->Uxscenario.rec[n].tipoPert3 < 0)
            {
            fprintf (stderr,"Tipo Pert %s non esiste (%s)\n",tipoPert,label);
            addr = -1;
            }
         pertToggle = CtxMalfConf->UxpertToggle3;
         Wform = &CtxMalfConf->UxSecVar3;
         Wlabel= &CtxMalfConf->UxsecVar3Label;
         Wmod  = &CtxMalfConf->UxsecVar3Mod;
         Wdescr= &CtxMalfConf->UxsecVar3Descr;
         Wval  = &CtxMalfConf->UxsecVar3ValTf;
         Wtoggle = &CtxMalfConf->UxsecVar3Tb;
         Wvalact = &CtxMalfConf->UxsecVar3ValAct;
         Wvalactlabel = &CtxMalfConf->UxsecVar3ValActLab;
         valoreScen = CtxScenMalf->Uxscenario.rec[n].valSec3;
         if (strcmp(label,""))   /* se esiste */
            CtxMalfConf->UxvarSecExist3 = 1;
         break;
      case 4:
         strcpy (label,m[pm].label_sec_var_4);
         strcpy (descrizione,sintesiDescr(variabili[m[pm].ind_sec_var_4].descr));
         strcpy (tipoPert,m_t[pt].pertSec4);
         addr = CtxScenMalf->Uxscenario.rec[n].p_sec4_var;
         if (!strcmp(tipoPert,"PERT_NOT"))   /* e' una variabile toggle	*/
            CtxMalfConf->UxpertToggle4 = 1;
         CtxScenMalf->Uxscenario.rec[n].tipoPert4 = assegnaTipoPert (tipoPert);
         if (CtxScenMalf->Uxscenario.rec[n].tipoPert4 < 0)
            {
            fprintf (stderr,"Tipo Pert %s non esiste (%s)\n",tipoPert,label);
            addr = -1;
            }
         pertToggle = CtxMalfConf->UxpertToggle4;
         Wform = &CtxMalfConf->UxSecVar4;
         Wlabel= &CtxMalfConf->UxsecVar4Label;
         Wmod  = &CtxMalfConf->UxsecVar4Mod;
         Wdescr= &CtxMalfConf->UxsecVar4Descr;
         Wval  = &CtxMalfConf->UxsecVar4ValTf;
         Wtoggle = &CtxMalfConf->UxsecVar4Tb;
         Wvalact = &CtxMalfConf->UxsecVar4ValAct;
         Wvalactlabel = &CtxMalfConf->UxsecVar4ValActLab;
         valoreScen = CtxScenMalf->Uxscenario.rec[n].valSec4;
         if (strcmp(label,""))   /* se esiste */
            CtxMalfConf->UxvarSecExist4 = 1;
         break;
      }

printf ("Numero %d valore scenario %f\n", numVar,valoreScen); 

		if (strcmp (label,""))   /* se esiste */
            {
				*Wform = XtVaCreateManagedWidget( "SecVar1",
         				xmFormWidgetClass, Wbase,
         				XmNresizePolicy, XmRESIZE_NONE,
         				XmNx, 0, XmNy, 0, XmNwidth, 260, XmNheight, 80,
         				XmNrightAttachment, XmATTACH_FORM,
         				XmNleftAttachment, XmATTACH_FORM,
         				NULL );	

printf("dopo la creazione di wform!!!!!!!!!!\n"); 

				Separator = XtVaCreateManagedWidget( "separator4",
         				xmSeparatorWidgetClass, *Wform,
         				XmNx, 0, XmNy, 0, XmNwidth, 260, XmNheight, 10,
         				XmNtopOffset, 0,
         				XmNbottomAttachment, XmATTACH_FORM,
         				XmNtopAttachment, XmATTACH_NONE,
         				NULL );
printf("dopo la creazione di separator!!!!!!!!!!\n"); 
				*Wdescr = XtVaCreateManagedWidget( "secVar1Descr",
         				xmLabelWidgetClass, *Wform,
         				XmNx, 10, XmNy, 53, XmNwidth, 240, XmNheight, 20,
         				XmNborderWidth, 0,
         				XmNalignment, XmALIGNMENT_BEGINNING,
                                        XmNfontList, UxConvertFontList(
                                        "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1"),
         				RES_CONVERT( XmNlabelString, descrizione ),
         				NULL );
printf("dopo la creazione di Wdescr!!!!!!!!!!\n"); 
				if (!pertToggle) /* se non e' toggle crea il campo testo */
      			{
      			*Wlabel = XtVaCreateManagedWidget( "secVar1Label",
         				xmLabelWidgetClass, *Wform,
         				XmNx, 10, XmNy, 26, XmNwidth, 100, XmNheight, 25,
         				XmNborderWidth, 0,
         				XmNalignment, XmALIGNMENT_BEGINNING,
                                        XmNfontList, UxConvertFontList(
                                        "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1"),
         				RES_CONVERT( XmNlabelString, label ),
         				NULL );
					*Wval = XtVaCreateManagedWidget( "secVar1ValTf",
         				xmTextWidgetClass, *Wform,
         				XmNx, 140, XmNy, 26, XmNwidth, 110, XmNheight, 25,
         				XmNmarginHeight, 2,
                                        XmNfontList, UxConvertFontList(
                                        "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1"),
         				NULL );
      			UxPutContext( *Wval, (char *) CtxMalfConf );

					/* flag entry modificata   */
      			*Wmod = XtVaCreateManagedWidget( "secVar1mod",
         				xmLabelWidgetClass, *Wform,
         				XmNwidth, 5, XmNheight, 20,
							XmNx, 130, XmNy, 26,
         				XmNborderWidth, 0,
         				XmNalignment, XmALIGNMENT_BEGINNING,
                                        XmNfontList, UxConvertFontList(
                                        "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1"),
         				RES_CONVERT( XmNlabelString, "" ),
         				NULL );
      			UxPutContext( *Wmod, (char *) CtxMalfConf );

					XtAddCallback( *Wval, XmNgainPrimaryCallback,
         					(XtCallbackProc) varSecInModifica,
         					(XtPointer)numVar);
/* controllo tipo e validita' della variabile */
      			if (addr >= 0)    /* se indirizzo valido  */
         			{
/* controllo se la variabile e' di input */
         			if (!string_exist (variabili[get_label_indx(label)].descr,
                                 FLAG_TIPO_INPUT))
            			editable = True;
         			else
            			editable = False;
/* se e' editabile e' di input, ricavo il valore da scenario */
         			if (editable == True)
            			valore = valoreScen;
         			else
            			valore = RtDbPGetValueD (dbpunti,addr);
         			sprintf (valStr,"%10.3f",valore);
         			XtVaSetValues (*Wval, XmNeditable, editable,
                              XmNcursorPositionVisible, editable,
                              XmNvalue, valStr, NULL);
         			}
      			else
         			{
         			sprintf (valStr,"xxxxx");
         			XtVaSetValues (*Wval, XmNeditable, False,
                              XmNcursorPositionVisible, False,
                              XmNvalue, valStr, NULL);
         			}
                       
            	}
				else /* se e' pert toggle (PERT_NOT) crea il toggle Button  */
      			{

      			if (valoreScen == 0.0)
         			valToggle = False;
      			else
         			valToggle = True;
		
/* printf ("Indice %d recuperato valore %d (%f)\n",n,valToggle,valoreScen); */

      			*Wtoggle = XtVaCreateManagedWidget( "toggle",
         			xmToggleButtonWidgetClass, *Wform,
         			XmNx, 10, XmNy, 26, XmNwidth, 240, XmNheight, 25,
         			XmNmarginHeight, 2,
         			XmNalignment, XmALIGNMENT_BEGINNING,
                                XmNfontList, UxConvertFontList(
                                "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1"),
         			RES_CONVERT( XmNlabelString, label ),
         			XmNindicatorSize, 20,
         			XmNrecomputeSize, False,
         			NULL );
      			XmToggleButtonSetState (*Wtoggle,valToggle,False);
      			UxPutContext( *Wtoggle, (char *) CtxMalfConf );
			
      			XtAddCallback (*Wtoggle,
                     			XmNvalueChangedCallback,
                     	varSecInModifica,(XtPointer)numVar);
		/* flag entry modificata   */
      			*Wmod = XtVaCreateManagedWidget( "secVar1mod",
         			xmLabelWidgetClass, *Wform,
         			XmNwidth, 5, XmNheight, 20,
						XmNx, 5, XmNy, 26,
         			XmNborderWidth, 0,
         			XmNalignment, XmALIGNMENT_BEGINNING,
                                XmNfontList, UxConvertFontList(
                                "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1"),
         			RES_CONVERT( XmNlabelString, "" ),
         			NULL );
			
      			UxPutContext( *Wmod, (char *) CtxMalfConf );
      			}

			*Wvalactlabel = XtVaCreateManagedWidget( "secVar1mod",
                  xmLabelWidgetClass, *Wform,
                  XmNwidth, 100, XmNheight, 20,
                  XmNx, 10, XmNy, 2,
                  XmNborderWidth, 0,
                  XmNalignment, XmALIGNMENT_BEGINNING,
                  XmNfontList, UxConvertFontList(
                  "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1"),
                  RES_CONVERT( XmNlabelString, VAL_ACT_LABEL ),
                  NULL );

			valore = RtDbPGetValueD (dbpunti,addr);
			sprintf (valStr,"%10.3f",valore);

			*Wvalact = XtVaCreateManagedWidget( "secVar1mod",
                  xmLabelWidgetClass, *Wform,
                  XmNwidth, 110, XmNheight, 20,
						XmNx, 140, XmNy, 2,
                  XmNborderWidth, 0,
                  XmNalignment, XmALIGNMENT_BEGINNING,
                  XmNfontList, UxConvertFontList(
                  "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1"),
                  RES_CONVERT( XmNlabelString, valStr ),
                  NULL );
 printf ("creaCampoVarSec: var [%s] n. %d\n",label,numVar); 

			}
}
/******************************************************************/
/*
 * elimina la perte non significativa delle descrizioni
 * Ritorna:		la parte a destra dei ":" ( se ci sono)
 *					oppure la parte a destra di "--xx--"
 *					oppure tutta la stringa
 */
char *sintesiDescr (char *s)
{
char stringa[MAX_LUN_DESCR_VAR];
int i;
int k;
int duepunti;

	duepunti = 0;
	for (i=0; ; i++)
		{
		if (s[i] == '\0')		/* se non ci sono i ":"  	*/
			break;
		if (s[i] == ':')		/* : trovati					*/
			{
			duepunti = 1;
			break;
			}
		}
	if (duepunti)
		{
		for (i++,k=0; s[i] != '\0'; i++,k++)
			stringa[k] = s[i];
		stringa[k] = '\0';
		}
	else
		strcpy (stringa,s);

	return (stringa);
}
/******************************************************************/
void  varSecInModifica(
         Widget wgt,
         int * data,
			XtPointer cb)
         
{
_UxCmalfConfig *Ctx;
int n_entry;
	Ctx = (_UxCmalfConfig *)UxGetContext (wgt);
	n_entry = (int)data;
	
	printf ("azione per entry %d\n",n_entry);
	switch (n_entry)
		{
		case 1:
			Ctx->UxvalSec1Modified = 1;
			XtVaSetValues (Ctx->UxsecVar1Mod, 
						XmNbackground, colore_app[COLORE_MODIFY],NULL);
			break;
		case 2:
         Ctx->UxvalSec2Modified = 1;
			XtVaSetValues (Ctx->UxsecVar2Mod, 
						XmNbackground, colore_app[COLORE_MODIFY],NULL);
         break;
		case 3:
         Ctx->UxvalSec3Modified = 1;
			XtVaSetValues (Ctx->UxsecVar3Mod, 
						XmNbackground, colore_app[COLORE_MODIFY],NULL);
         break;
		case 4:
         Ctx->UxvalSec4Modified = 1;
			XtVaSetValues (Ctx->UxsecVar4Mod, 
						XmNbackground, colore_app[COLORE_MODIFY],NULL);
         break;
		}
}
/******************************************************************/
/*
 * effettua il refresh della entry della variabile secondaria
 *	Ha funzioni di aggiornamento dati
 */
int displayVarSec (Widget Wconf, Widget Wscen,
		M_COMPONENT *m,int pm, 
		M_COMP_TYPE *m_t,int pt, 
		int n, 
		int numVar)
{
_UxCscenarioMalf *CtxScenMalf;
_UxCmalfConfig *CtxMalfConf;
char label[MAX_LUN_NOME_VAR];
char descrizione[MAX_LUN_DESCR_VAR];
int addr;
Widget Wval;
Widget Wactval;
float valore;
float valPrec;
char valStr[20];
int editable;
int exist;
int toggle;
int modify;


	CtxScenMalf = (_UxCscenarioMalf *) UxGetContext (Wscen);
   CtxMalfConf = (_UxCmalfConfig *) UxGetContext (Wconf);
/*
 * assegnazione variabili
 */
	modify = 0;				/* flag per refresh display						*/
	switch (numVar)		/* indice della variabile secondaria			*/
		{
		case 1:
			Wval = CtxMalfConf->UxsecVar1ValTf;
			Wactval = CtxMalfConf->UxsecVar1ValAct;
			addr = CtxScenMalf->Uxscenario.rec[n].p_sec1_var;
			exist = CtxMalfConf->UxvarSecExist1;
			toggle = CtxMalfConf->UxpertToggle1;
			valPrec = CtxMalfConf->UxvalPrec1;
         valore = RtDbPGetValueD (dbpunti,addr);
			if (valore != valPrec)		/* si modifica		*/
				{
				modify = 1;
				CtxMalfConf->UxvalPrec1 = valore;
				}
			break;
		case 2:
         Wval = CtxMalfConf->UxsecVar2ValTf;
			Wactval = CtxMalfConf->UxsecVar2ValAct;
         addr = CtxScenMalf->Uxscenario.rec[n].p_sec2_var;
			exist = CtxMalfConf->UxvarSecExist2;
			toggle = CtxMalfConf->UxpertToggle2;
			valPrec = CtxMalfConf->UxvalPrec2;
			valore = RtDbPGetValueD (dbpunti,addr);
         if (valore != valPrec)     /* si modifica    */
            {
            modify = 1;
            CtxMalfConf->UxvalPrec2 = valore;
            }
			break;
		case 3:
         Wval = CtxMalfConf->UxsecVar3ValTf;
			Wactval = CtxMalfConf->UxsecVar3ValAct;
         addr = CtxScenMalf->Uxscenario.rec[n].p_sec3_var;
			exist = CtxMalfConf->UxvarSecExist3;
			toggle = CtxMalfConf->UxpertToggle3;
			valPrec = CtxMalfConf->UxvalPrec3;
			valore = RtDbPGetValueD (dbpunti,addr);
         if (valore != valPrec)     /* si modifica    */
            {
            modify = 1;
            CtxMalfConf->UxvalPrec3 = valore;
            }
			break;
		case 4:
         Wval = CtxMalfConf->UxsecVar4ValTf;
			Wactval = CtxMalfConf->UxsecVar4ValAct;
         addr = CtxScenMalf->Uxscenario.rec[n].p_sec4_var;
			exist = CtxMalfConf->UxvarSecExist4;
			toggle = CtxMalfConf->UxpertToggle4;
			valPrec = CtxMalfConf->UxvalPrec4;
			valore = RtDbPGetValueD (dbpunti,addr);
         if (valore != valPrec)     /* si modifica    */
            {
            modify = 1;
            CtxMalfConf->UxvalPrec4 = valore;
            }
			break;
		}
/*  printf ("displayVarSec: var [%s] n. %d\n",label,numVar);  */

	if ((exist == 1) &&  	/* se esiste la entry 	*/
       (addr >= 0))       	/* se indirizzo valido  */
         {
			if (!toggle)		/* se non e' toggle c'e' il campo testo */
				{
				XtVaGetValues (Wval, XmNeditable , &editable, NULL);
				
				if ((!editable) && (modify == 1))/* e' di output o inesistente, refresh	*/
					{
         		sprintf (valStr,"%10.3f",valore);
         		XtVaSetValues (Wval, XmNvalue, valStr, NULL);
					}
				}
/* eseguo il refresh del valore attuale */
			
			if (modify == 1)
				{
         	sprintf (valStr,"%10.3f",valore);
				XtVaSetValues (Wactval, RES_CONVERT( XmNlabelString,valStr), NULL);
				}
         }
}
/******************************************************************/
/*
 * carica nell' interfaccia malfConfig i dati della malfunzione selezionata
 * Wdest = widget interfaccia malfConfig
 * Wparent = widget interfaccia scenarioMalf
 *	Ha funzione di aggiornamento dati
 */
int displayDatiMalfConfig (Widget Wdest, Widget Wparent, 
			M_COMP_TYPE *m_type, MALF_SET *m_set, M_COMPONENT *m)
{
_UxCscenarioMalf *CtxScenMalf;
_UxCmalfConfig *CtxMalfConf;
int n;
int punt;	/* puntatore alla struttura M_COMPONENT */
char descrizione[MAX_LUN_DESCR_VAR];
int indx;   /* indice del tipo in M_COMP_TYPE */
char valStr[20];
float valore;
char *messaggio;
Boolean editable;

	CtxScenMalf = (_UxCscenarioMalf *) UxGetContext (Wparent);
	CtxMalfConf = (_UxCmalfConfig *) UxGetContext (Wdest);
/*
 * recupera l'indice di scenario
 */
	n = CtxMalfConf->Uxindx;

/*
 * recupera le precedenti impostazioni dei parametri configurabili 
 *		Il primo giro vengono da file, durante gli aggiornamenti 
 * 		sono calcolati.
 */
	CtxMalfConf->Uxdelay = CtxScenMalf->Uxscenario.rec[n].delay;
	CtxMalfConf->Uxduration = CtxScenMalf->Uxscenario.rec[n].durata;
	CtxMalfConf->Uxramp = CtxScenMalf->Uxscenario.rec[n].time_to;

	if (!CtxMalfConf->UxdelayModified)		/* se il campo non e' in modifica	*/
		{
		sprintf (valStr,"%8.0f",CtxMalfConf->Uxdelay);
		write_txt (CtxMalfConf->UxdelayTf,valStr);
		}

	if (!CtxMalfConf->UxdurataModified)
		{
		if (CtxMalfConf->Uxduration == 0.0)
			strcpy (valStr,"");
		else
			sprintf (valStr,"%8.0f",CtxMalfConf->Uxduration);
		XtVaSetValues (CtxMalfConf->UxdurationTf, XmNvalue, valStr, NULL);
		}

	if (!CtxMalfConf->UxrampModified)
      {
		sprintf (valStr,"%8.0f",CtxMalfConf->Uxramp);
		XtVaSetValues (CtxMalfConf->UxrampTf, XmNvalue, valStr, NULL);
		}
/*
 * carica il puntatore a M_COMPONENT
 */
	punt = CtxScenMalf->Uxscenario.rec[n].punt_comp;
/*
 * verifica l' esistenza della classe (consolidare il dato )
 */
   indx = getCompType (m_type,m[punt].m_c_t_code);
   if (indx < 0)
      {
      messaggio = (char *)malloc (strlen(MALF)+strlen(TYPE_NOT_EXIST)+20);
      sprintf (messaggio,"%s:\n%s %d\n",MALF,TYPE_NOT_EXIST,m[punt].m_c_t_code);
      attenzione (CtxMalfConf->UxmalfConfig,messaggio);
      sprintf (messaggio,"%s:%s %d\n",MALF,TYPE_NOT_EXIST,m[punt].m_c_t_code);
      add_message (areaMessaggi,messaggio,LIVELLO_3);
      free (messaggio);
      return (-1);
      }
/*
 * aggiorna le variabili secondarie
 */
/*	if ((!CtxMalfConf->UxvalSec1Modified) &&  */
		if	(CtxScenMalf->Uxscenario.rec[n].p_sec1_var >= 0) /* la var. sec. esiste */
			displayVarSec (Wdest,Wparent,m,punt,m_type,indx,n,1);

/*	if ((!CtxMalfConf->UxvalSec2Modified) && */
      if  (CtxScenMalf->Uxscenario.rec[n].p_sec2_var >= 0) /* la var. sec. esiste */
			displayVarSec (Wdest,Wparent,m,punt,m_type,indx,n,2);

/*	if ((!CtxMalfConf->UxvalSec3Modified) && */
      if  (CtxScenMalf->Uxscenario.rec[n].p_sec3_var >= 0) /* la var. sec. esiste */
			displayVarSec (Wdest,Wparent,m,punt,m_type,indx,n,3);

/*	if ((!CtxMalfConf->UxvalSec4Modified) && */
      if  (CtxScenMalf->Uxscenario.rec[n].p_sec4_var >= 0) /* la var. sec. esiste */
			displayVarSec (Wdest,Wparent,m,punt,m_type,indx,n,4);

	return (0);
}
/*********************************************************************/
/*
 * ha il compito diindividuare le variabili secondarie e creare le relative entries
 */
int creaCampiVarSec (Widget Wdest, Widget Wparent,
         M_COMP_TYPE *m_type, MALF_SET *m_set, M_COMPONENT *m)
{
_UxCscenarioMalf *CtxScenMalf;
_UxCmalfConfig *CtxMalfConf;
int n;
int punt;   /* puntatore alla struttura M_COMPONENT */
char descrizione[MAX_LUN_DESCR_VAR];
int indx;   /* indice del tipo in M_COMP_TYPE */
char valStr[20];
float valore;
char *messaggio;
Boolean editable;
int numeroScenario;


   CtxScenMalf = (_UxCscenarioMalf *) UxGetContext (Wparent);
   CtxMalfConf = (_UxCmalfConfig *) UxGetContext (Wdest);
/*
 * aggiorna la titlebar
 */
   n = CtxMalfConf->Uxindx;      /* indice della variabile  */
   update_title (Wdest, CtxScenMalf->Uxscenario.rec[n].label);

printf("dopo la update_title!!!!!!!!!!!\n"); 


/*
 * inserisce le variabili secondarie
 */
/* punt_comp viene aggiornato alla selezione della variabile ed al caricamento
   da file dello scenario
 */
   punt = CtxScenMalf->Uxscenario.rec[n].punt_comp;

/*
 * carica i puntatori in memoria
 */
/* caricato da scenarioMalf */
/* CtxScenMalf->Uxscenario.rec[n].p_main_var = m[punt].p_main_var; */

   CtxScenMalf->Uxscenario.rec[n].p_sec1_var = m[punt].p_sec_var_1;
   CtxScenMalf->Uxscenario.rec[n].p_sec2_var = m[punt].p_sec_var_2;
   CtxScenMalf->Uxscenario.rec[n].p_sec3_var = m[punt].p_sec_var_3;
   CtxScenMalf->Uxscenario.rec[n].p_sec4_var = m[punt].p_sec_var_4;

   indx = getCompType (m_type,m[punt].m_c_t_code);
   if (indx < 0)
      {
      messaggio = (char *)malloc (strlen(MALF)+strlen(TYPE_NOT_EXIST)+20);
      sprintf (messaggio,"%s:\n%s %d\n",MALF,TYPE_NOT_EXIST,m[punt].m_c_t_code);
      attenzione (CtxMalfConf->UxmalfConfig,messaggio);
      sprintf (messaggio,"%s:%s %d\n",MALF,TYPE_NOT_EXIST,m[punt].m_c_t_code);
      add_message (areaMessaggi,messaggio,LIVELLO_3);
      free (messaggio);
      return (-1);
      }
/* se lo scenario e' valido (gia' registrato) carica i valori registrati		*/
/* altrimenti recupera i dati di default													*/

	numeroScenario = CtxScenMalf->Uxnum_scen;

/*
   modifica del 3/7/97 il valore target di default e' il valore in database
   al momento dell' istanziazione ( per evitare emissioni di pert sec. non
   volute). Pertanto la memorizzazione dei valori di default in scenario
   non e' efficace.
 */
	if (CtxScenMalf->Uxscenario.rec[n].p_sec1_var >= 0)
		CtxScenMalf->Uxscenario.rec[n].valSec1 = 
									RtDbPGetValueD (dbpunti,CtxScenMalf->Uxscenario.rec[n].p_sec1_var);
	if (CtxScenMalf->Uxscenario.rec[n].p_sec2_var >= 0)
		CtxScenMalf->Uxscenario.rec[n].valSec2 = 
									RtDbPGetValueD (dbpunti,CtxScenMalf->Uxscenario.rec[n].p_sec2_var);
	if (CtxScenMalf->Uxscenario.rec[n].p_sec3_var >= 0)
		CtxScenMalf->Uxscenario.rec[n].valSec3 = 
									RtDbPGetValueD (dbpunti,CtxScenMalf->Uxscenario.rec[n].p_sec3_var);
	if (CtxScenMalf->Uxscenario.rec[n].p_sec4_var >= 0)
		CtxScenMalf->Uxscenario.rec[n].valSec4 = 
									RtDbPGetValueD (dbpunti,CtxScenMalf->Uxscenario.rec[n].p_sec4_var);
	/* se scenario valido			*/
/******* modifica del 3/7/97
	if (scenari_malf.valido[numeroScenario - 1] > 0) 				
		{
		CtxScenMalf->Uxscenario.rec[n].valSec1 =  CtxScenMalf->Uxscenario.rec[n].valSec1;
		CtxScenMalf->Uxscenario.rec[n].valSec2 =  CtxScenMalf->Uxscenario.rec[n].valSec2;
		CtxScenMalf->Uxscenario.rec[n].valSec3 =  CtxScenMalf->Uxscenario.rec[n].valSec3;
		CtxScenMalf->Uxscenario.rec[n].valSec4 =  CtxScenMalf->Uxscenario.rec[n].valSec4;
		}
	else
		{
		CtxScenMalf->Uxscenario.rec[n].valSec1 =  (float)atof(m_type[indx].valPertSec1);
		CtxScenMalf->Uxscenario.rec[n].valSec2 =  (float)atof(m_type[indx].valPertSec2);
		CtxScenMalf->Uxscenario.rec[n].valSec3 =  (float)atof(m_type[indx].valPertSec3);
		CtxScenMalf->Uxscenario.rec[n].valSec4 =  (float)atof(m_type[indx].valPertSec4);
		}
*************************************************************************/

printf("prima della creazione delle entries!!!!!!!!!!!\n"); 

/* crea le entries */
   if (CtxScenMalf->Uxscenario.rec[n].p_sec1_var >= 0) /* la var. sec. esiste */
      creaCampoVarSec (Wdest,Wparent,m,punt,m_type,indx,n,1);
   if (CtxScenMalf->Uxscenario.rec[n].p_sec2_var >= 0) /* la var. sec. esiste */
      creaCampoVarSec (Wdest,Wparent,m,punt,m_type,indx,n,2);
   if (CtxScenMalf->Uxscenario.rec[n].p_sec3_var >= 0) /* la var. sec. esiste */
      creaCampoVarSec (Wdest,Wparent,m,punt,m_type,indx,n,3);
   if (CtxScenMalf->Uxscenario.rec[n].p_sec4_var >= 0) /* la var. sec. esiste */
      creaCampoVarSec (Wdest,Wparent,m,punt,m_type,indx,n,4);

printf("dopo della creazione delle entries!!!!!!!!!!!\n"); 

   return (0);
}
/*********************************************************************/
/*
 * verifica l' esistenza in configurazione della classe prevista
 */
int getCompType (M_COMP_TYPE *m_t, int n)
{
int i;
   i = 0;
   while (m_t[i].m_c_t_code >= 0)
      {
      if (m_t[i].m_c_t_code == n) /* trovato il codice classe */
         return (i);
      i++;
      }
   return (-1);
}
/*********************************************************************/
int applyMalfConfig (Widget mConf, Widget scenMalf)
{
_UxCmalfConfig *Ctx;
_UxCscenarioMalf *CtxScenMalf;


   Ctx = (_UxCmalfConfig *) UxGetContext (mConf);
   CtxScenMalf = (_UxCscenarioMalf *) UxGetContext (scenMalf);

/* delay */
	mfConfToScenMalf (Ctx->UxdelayTf,Ctx->Uxindx,&CtxScenMalf->Uxscenario,1);
	Ctx->UxdelayModified = 0;

/* duration */
	mfConfToScenMalf (Ctx->UxdurationTf,Ctx->Uxindx,&CtxScenMalf->Uxscenario,2);
	Ctx->UxdurataModified = 0;

/* rampa */
	mfConfToScenMalf (Ctx->UxrampTf,Ctx->Uxindx,&CtxScenMalf->Uxscenario,3);
	Ctx->UxrampModified = 0;

/* sec var 1 (se esiste e se non e' toggle) */
	if ((Ctx->UxvarSecExist1 == 1) && (Ctx->UxpertToggle1 != 1))
		mfConfToScenMalf(Ctx->UxsecVar1ValTf,Ctx->Uxindx,&CtxScenMalf->Uxscenario,4);
	else if ((Ctx->UxvarSecExist1 == 1) && (Ctx->UxpertToggle1 == 1))
		applyTbState (Ctx->UxsecVar1Tb,Ctx->Uxindx,&CtxScenMalf->Uxscenario,4);
	if (Ctx->UxvalSec1Modified)
		{
		Ctx->UxvalSec1Modified = 0; 
		XtVaSetValues (Ctx->UxsecVar1Mod, XmNbackground, Ctx->Uxsfondo, NULL);
		}

/* sec var 2 (se esiste e se non e' toggle) */
	if ((Ctx->UxvarSecExist2 == 1) && (Ctx->UxpertToggle2 != 1))
		mfConfToScenMalf(Ctx->UxsecVar2ValTf,Ctx->Uxindx,&CtxScenMalf->Uxscenario,5);
	else if ((Ctx->UxvarSecExist2 == 1) && (Ctx->UxpertToggle2 == 1))
      applyTbState (Ctx->UxsecVar2Tb,Ctx->Uxindx,&CtxScenMalf->Uxscenario,5);
	if (Ctx->UxvalSec2Modified)
		{
		Ctx->UxvalSec2Modified = 0; 
		XtVaSetValues (Ctx->UxsecVar2Mod, XmNbackground, Ctx->Uxsfondo, NULL);
		}

/* sec var 3 (se esiste e se non e' toggle) */
	if ((Ctx->UxvarSecExist3 == 1) && (Ctx->UxpertToggle3 != 1))
		mfConfToScenMalf(Ctx->UxsecVar3ValTf,Ctx->Uxindx,&CtxScenMalf->Uxscenario,6);
	else if ((Ctx->UxvarSecExist3 == 1) && (Ctx->UxpertToggle3 == 1))
      applyTbState (Ctx->UxsecVar3Tb,Ctx->Uxindx,&CtxScenMalf->Uxscenario,6);
	if (Ctx->UxvalSec3Modified)
		{
		Ctx->UxvalSec3Modified = 0; 
		XtVaSetValues (Ctx->UxsecVar3Mod, XmNbackground, Ctx->Uxsfondo, NULL);
		}

/* sec var 4 (se esiste e se non e' toggle) */
	if ((Ctx->UxvarSecExist4 == 1) && (Ctx->UxpertToggle4 != 1))
		mfConfToScenMalf(Ctx->UxsecVar4ValTf,Ctx->Uxindx,&CtxScenMalf->Uxscenario,7);
	else if ((Ctx->UxvarSecExist4 == 1) && (Ctx->UxpertToggle4 == 1))
      applyTbState (Ctx->UxsecVar4Tb,Ctx->Uxindx,&CtxScenMalf->Uxscenario,7);
	if (Ctx->UxvalSec4Modified)
		{
		Ctx->UxvalSec4Modified = 0; 
		XtVaSetValues (Ctx->UxsecVar4Mod, XmNbackground, Ctx->Uxsfondo, NULL);
		}
}
/*********************************************************************/
int mfConfToScenMalf (Widget Wdest, int n, SCENARIO *scen, int item)
{
char testo[20];

	switch (item)
		{

		case 1:														/* delay	*/
   		if (!read_txt(Wdest,testo))
				{
      		scen->rec[n].delay = (float)atof(testo);
				sprintf (testo,"%10.0f",scen->rec[n].delay);
				write_txt (Wdest, testo);
      		}
   		else
      		{
      		scen->rec[n].delay = 0.0;
      		write_txt (Wdest, "0");
      		}
			break;

		case 2:                                         /* durata */
      	if (!read_txt(Wdest,testo))
         	{
         	scen->rec[n].durata = (float)atof(testo);
         	sprintf (testo,"%10.0f",scen->rec[n].durata);
         	write_txt (Wdest, testo);
				if (scen->rec[n].durata != 0.0)
					scen->rec[n].durataInf = 1;	/* flag di durata non infinita */
				else
					scen->rec[n].durataInf = 0;
         	}
      	else
         	{
         	scen->rec[n].durata = 0.0;
         	write_txt (Wdest, "");
         	}
			break;

		case 3:                                         /* rampa  */
      	if (!read_txt(Wdest,testo))
         	{
         	scen->rec[n].time_to = (float)atof(testo);
         	sprintf (testo,"%10.3f",scen->rec[n].time_to);
         	write_txt (Wdest, testo);
         	}
      	else
         	{
         	scen->rec[n].time_to = 0.0;
         	write_txt (Wdest, "");
         	}
			break;

      case 4:                                         /* sec var 1  */
         if (!read_txt(Wdest,testo))
            {
            scen->rec[n].valSec1 = (float)atof(testo);
            sprintf (testo,"%10.3f",scen->rec[n].valSec1);
            write_txt (Wdest, testo);
            }
         else
            {
            scen->rec[n].valSec1 = 0.0;
            write_txt (Wdest, "");
            }
         break;

      case 5:                                         /* sec var 2  */
         if (!read_txt(Wdest,testo))
            {
            scen->rec[n].valSec2 = (float)atof(testo);
            sprintf (testo,"%10.3f",scen->rec[n].valSec2);
            write_txt (Wdest, testo);
            }
         else
            {
            scen->rec[n].valSec2 = 0.0;
            write_txt (Wdest, "");
            }
         break;

      case 6:                                         /* sec var 3  */
         if (!read_txt(Wdest,testo))
            {
            scen->rec[n].valSec3 = (float)atof(testo);
            sprintf (testo,"%10.3f",scen->rec[n].valSec3);
            write_txt (Wdest, testo);
            }
         else
            {
            scen->rec[n].valSec3 = 0.0;
            write_txt (Wdest, "");
            }
         break;

      case 7:                                         /* sec var 4  */
         if (!read_txt(Wdest,testo))
            {
            scen->rec[n].valSec4 = (float)atof(testo);
            sprintf (testo,"%10.3f",scen->rec[n].valSec4);
            write_txt (Wdest, testo);
            }
         else
            {
            scen->rec[n].valSec4 = 0.0;
            write_txt (Wdest, "");
            }
         break;
		}
	return (0);
}
/*********************************************************************/
/*
 * memorizza nello scenario lo stato del toggle
 */
int applyTbState (Widget Wdest, int n, SCENARIO *scen, int item)
{
Boolean stato;

	stato = XmToggleButtonGetState (Wdest);
printf ("Toggle %d stato %d\n",item-3,stato);
	switch (item)
		{
		case 4:
			scen->rec[n].valSec1 = (float)stato;
			break;
		case 5:
			scen->rec[n].valSec2 = (float)stato;
			break;
		case 6:
			scen->rec[n].valSec3 = (float)stato;
			break;
		case 7:
			scen->rec[n].valSec4 = (float)stato;
			break;
		}
	return (0);
}
/************************************************************************/
/*
 * attiva (azione = 0) o disattiva (azione = 1) le malfunzioni selezionate
 */
int attivaMalfSelect (Widget w, int azione)
{
int i;
_UxCscenarioMalf *Ctx;

	Ctx = (_UxCscenarioMalf *) UxGetContext (w);

	for (i=0; i<MAX_VAR_SCENARIO; i++)
		{
		if (Ctx->Uxscenario.rec[i].valido > 0) /* se l' indirizzo e' valido */
			{
			if (Ctx->Uxmalf_select[i] > 0) 			/* toggle button selezionato */
				{
					{
					switch (azione)
						{
						case ACTIVATION:
							Ctx->Uxscenario.rec[i].stato = INSERTED;
							XmToggleButtonSetState (Ctx->UxscenMalfTb[i],False,True);
							break;
						case DEACTIVATION:
							Ctx->Uxscenario.rec[i].stato = REMOVED;
							XmToggleButtonSetState (Ctx->UxscenMalfTb[i],False,True);
							break;
						}
					}
				}
			}
		}
	return (0);
}
/*****************************************************************************/
void refresh_scenarioMalf (XtPointer data)
{
_UxCscenarioMalf *Ctx;
int i;
Widget w = (Widget)data;

	Ctx = (_UxCscenarioMalf*)UxGetContext(w);

	switch (Ctx->Uxtipo)
		{
		case SOMMARIO_MALF:
		case SOMMARIO_FREM:
			for (i=0; i<MAX_VAR_SCENARIO; i++)
            {
            if (Ctx->Uxscenario.rec[i].valido > 0) /* slot dello scenario occupato  */
               updateRigaSommario (w, i);  
            }
   		break;
		case MALFUNZIONI:
		case FUNZIONI_REMOTE:
			for (i=0; i<MAX_VAR_SCENARIO; i++)
				{
				if (Ctx->Uxscenario.rec[i].valido > 0)	/* slot dello scenario occupato	*/
					updateRigaScenario (w, i);
				}
			break;
		}
	
	attiva_timer_scenarioMalf (w);
	return;
}
/*****************************************************************************/
/*
 * aggiorna le entry di sommario in display 
 */
int updateRigaSommario (Widget w, int n)
{
_UxCscenarioMalf *Ctx;
float deltat;
float val_att;
char valStr[20];
int p_malf_set[MAX_MALF_SET_VALUE];
int i;
int n_set;
int stato;
int sfondo;
int stato_prec;
int addr;
float valore;
int t;
M_COMP_TYPE *m_c_t;
MALF_SET *m_set;
M_COMPONENT *m;

	Ctx = (_UxCscenarioMalf*)UxGetContext(w);
	sfondo = (int)Ctx->Uxcol_sfondo_scenMalf;
	t = Ctx->Uxtipo;

	switch (t)
		{
      case MALFUNZIONI:
      case SOMMARIO_MALF:
         m = m_component;
         m_set = malf_set;
         m_c_t = m_comp_type;
         break;
      case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         m = fr_component;
         m_set = frem_set;
         m_c_t = fr_comp_type;
         break;
		}
   if (Ctx->Uxprimo_giro[n] == 1)
      {
      Ctx->Uxprimo_giro[n] = 0;
      Ctx->Uxtempo_old[n]  = val.actual.tempo_sim;
      deltat = 0.;
      }
   else
      {
      deltat = val.actual.tempo_sim - Ctx->Uxtempo_old[n];
/* printf ("tsim %f told %f delta %f\n",val.actual.tempo_sim,Ctx->Uxtempo_old[n],deltat);  */
      Ctx->Uxtempo_old[n] = val.actual.tempo_sim;
      }
/*
 * aggiorna il valore attuale
 */
/** printf ("Entry %d punt %d\n",n,Ctx->Uxscenario.rec[n].p_main_var); **/
	val_att = RtDbPGetValueD(dbpunti,Ctx->Uxscenario.rec[n].p_main_var);
/*
 *  aggiornamento var. sec. dopo caricamento di ogni CI o BT 
 */
   if (nuovoSnapCaricato == 1)      /* database aggiornato   */
      {
/*      if (Ctx->UxnuovoSnapAggiornamento[n] == 0)     */
      if (Ctx->UxnuovoSnapAggiornamento[n] != nuovoSnapCaricato2)   
         { 
fprintf (stderr,"Refresh var. sec. dopo LOAD CI\n");
         if (Ctx->Uxscenario.rec[n].p_sec1_var >= 0)
            Ctx->Uxscenario.rec[n].valSec1 =
                        RtDbPGetValueD (dbpunti,Ctx->Uxscenario.rec[n].p_sec1_var);
         if (Ctx->Uxscenario.rec[n].p_sec2_var >= 0)
            Ctx->Uxscenario.rec[n].valSec2 =
                        RtDbPGetValueD (dbpunti,Ctx->Uxscenario.rec[n].p_sec2_var);
         if (Ctx->Uxscenario.rec[n].p_sec3_var >= 0)
            Ctx->Uxscenario.rec[n].valSec3 =
                        RtDbPGetValueD (dbpunti,Ctx->Uxscenario.rec[n].p_sec3_var);
         if (Ctx->Uxscenario.rec[n].p_sec4_var >= 0)
            Ctx->Uxscenario.rec[n].valSec4 =
                        RtDbPGetValueD (dbpunti,Ctx->Uxscenario.rec[n].p_sec4_var);
/*         Ctx->UxnuovoSnapAggiornamento[n] = 1;   */
/*
 * azzera delay e durata
 */
   		Ctx->Uxscenario.rec[n].delay = 0.;
   		Ctx->Uxscenario.rec[n].durata = 0.;
   		Ctx->Uxscenario.rec[n].durataInf = 0; /* default durata infinita   */
   		Ctx->Uxscenario.rec[n].time_to = 0.;

         Ctx->UxnuovoSnapAggiornamento[n] = nuovoSnapCaricato2;
         }
      }
   else                             /* terminato aggiornamento    */
      Ctx->UxnuovoSnapAggiornamento[n] = 0;  /* reset condizione        */
/*
 * se non e' nel database visualizza il valore e ritorna 
 */
	if (Ctx->Uxscenario.rec[n].non_configurata > 0)
		{
		sprintf (valStr,"%10.3f",val_att);
		XtVaSetValues (Ctx->UxscenMalfValAtt[n],
               RES_CONVERT (XmNlabelString,valStr),NULL);
		return (-1);
		}

	if (Ctx->UxvalorePrec[n] != val_att)   /* se c' e' stata variazione   */
		{
   	if ((n_set = get_m_set_from_m_comp (Ctx->Uxscenario.rec[n].punt_comp,
                                          &p_malf_set[0], m, m_set)) >= 0)
      	{
      	sprintf (valStr,"%10.3f",val_att);  /* default visualizza il valore  */
      	i = 0;
      	while (p_malf_set[i] >= 0)
         	{
         	if (m_set[p_malf_set[i]].m_s_value == (int)val_att)
            	{     /* se trova il corrispondente visualizza la stringa   */
            	strcpy(valStr,m_set[p_malf_set[i]].m_s_descr);
                valStr[strlen(valStr)-1]='\0';
            	break;
            	}
         	i++;
         	}
      	}
   	XtVaSetValues (Ctx->UxscenMalfValAtt[n],
               RES_CONVERT (XmNlabelString,valStr),NULL);
		Ctx->UxvalorePrec[n] = val_att;
		}
 	
/*
 * gestione colore:
 *		se la variabile e' in sommario significa che la perturbazione e' stata emessa.
 *		La pert viene emessa a delay = 0 (il delay viene calcolato dallo scenario), quindi
 * 	la malfunzione e' comunque attiva
 */
	stato = Ctx->Uxscenario.rec[n].stato;
	stato_prec = Ctx->Uxscenario.rec[n].stato_prec;
	switch (stato)
      {
      case SELECTED:    /* in sommario il caso non dovrebbe esistere */
			if (stato_prec != stato)  /* e' stata richiesta l' attivazione  */
            {
            set_colore_riga_scenario (w,n,sfondo);
            Ctx->Uxscenario.rec[n].stato_prec = stato;
            }
			break;
		case INSERTED:
/*         deltat = val.actual.tempo_sim - Ctx->Uxtempo_old[n];   ***/
         Ctx->Uxscenario.rec[n].delay -= deltat;
/*         Ctx->Uxtempo_old[n] = val.actual.tempo_sim;   ***/
/* cambio colore */
         if (stato_prec != stato)  /* e' stata richiesta l' attivazione  */
               {
               set_colore_riga_scenario (w,n,colore_app[COLORE_INSERTED]);
               Ctx->Uxscenario.rec[n].stato_prec = stato;
               }
/* emissione di pert se delay inesistente o scaduto   */
         if (Ctx->Uxscenario.rec[n].delay <= 0)    /* delay scaduto  */
            {
            Ctx->Uxscenario.rec[n].delay = 0.0;
            addr = Ctx->Uxscenario.rec[n].p_main_var;
            valore = Ctx->Uxscenario.rec[n].target;
            fprintf (stderr,"Azionamento Variabile [%s]\n", Ctx->Uxscenario.rec[n].label);
				startMalfunzione (addr,valore,Ctx->Uxscenario.rec[n]);
/* cambio di stato   */
            Ctx->Uxscenario.rec[n].stato = ACTIVE;
            set_colore_riga_scenario (w,n,colore_app[COLORE_ACTIVE]);
            }
			break;
		case REMOVED:
			if (stato_prec != stato)  /* e' stata richiesta l' attivazione  */
               {
               set_colore_riga_scenario (w,n,colore_app[COLORE_REMOVED]);
               Ctx->Uxscenario.rec[n].stato_prec = stato;
               addr = Ctx->Uxscenario.rec[n].p_main_var;
					stopMalfunzione (addr);
					}
			break;
		case ACTIVE:
         set_colore_riga_scenario (w,n,colore_app[COLORE_ACTIVE]);
			if (stato_prec != stato)  /* e' stata richiesta l' attivazione  */
            {
            Ctx->Uxscenario.rec[n].stato_prec = stato;
            }
/* cambio di stato se durata scaduta   */
         if (Ctx->Uxscenario.rec[n].durataInf > 0)
            {
            Ctx->Uxscenario.rec[n].durata -= deltat;
            if (Ctx->Uxscenario.rec[n].durata <= 0.0)    /* delay scaduto  */
               {
/* cambio di stato   */
               Ctx->Uxscenario.rec[n].durata = 0.0;
               Ctx->Uxscenario.rec[n].durataInf = 0;
               Ctx->Uxscenario.rec[n].stato = REMOVED;
          /*     set_colore_riga_scenario (w,n,colore_app[COLORE_REMOVED]); */
               }
            }
			break;
		}
	
}
/*****************************************************************************/
int updateRigaScenario (Widget w, int n)
{
int stato;
int stato_prec;
int sfondo;
float val_att;
_UxCscenarioMalf *Ctx;
int n_set;
int p_malf_set[MAX_MALF_SET_VALUE];
int i;
char valStr[20];
float deltat;
int addr;
float valore;
float durata;
float valPert;
int indx;
int t;
M_COMP_TYPE *m_c_t;
MALF_SET *m_set;
M_COMPONENT *m;

	Ctx = (_UxCscenarioMalf*)UxGetContext(w);
	sfondo = (int)Ctx->Uxcol_sfondo_scenMalf;
	t = Ctx->Uxtipo;
	
	switch (t)
		{
		case MALFUNZIONI:
		case SOMMARIO_MALF:
			m = m_component;
			m_set = malf_set;
			m_c_t = m_comp_type;
			break;
		case FUNZIONI_REMOTE:
		case SOMMARIO_FREM:
			m = fr_component;
			m_set = frem_set;
			m_c_t = fr_comp_type;
			break;
		}

	if (Ctx->Uxprimo_giro[n] == 1)
		{
		Ctx->Uxprimo_giro[n] = 0;
		Ctx->Uxtempo_old[n]	= val.actual.tempo_sim;
		deltat = 0.;
		}
	else
		{
      deltat = val.actual.tempo_sim - Ctx->Uxtempo_old[n];
/* printf ("tsim %f told %f delta %f\n",val.actual.tempo_sim,Ctx->Uxtempo_old[n],deltat);  */
      Ctx->Uxtempo_old[n] = val.actual.tempo_sim;
		}
/*
 * aggiornamento valore attuale
 */
	if (Ctx->Uxscenario.rec[n].p_main_var >= 0)
		val_att = RtDbPGetValueD(dbpunti,Ctx->Uxscenario.rec[n].p_main_var);
	else 	
		val_att = -999.999;	/* la variabile non esiste		*/

	if (val_att != Ctx->UxvalorePrec[n])		/* se c'e' stata variazione  */
		{
		if ((n_set = get_m_set_from_m_comp (Ctx->Uxscenario.rec[n].punt_comp,
                                          &p_malf_set[0], m, m_set)) >= 0)
			{
			sprintf (valStr,"%10.3f",val_att);	/* default visualizza il valore	*/
			i = 0;
      	while (p_malf_set[i] >= 0)
         	{
				if (m_set[p_malf_set[i]].m_s_value == (int)val_att)
					{		/* se trova il corrispondente visualizza la stringa	*/
					strcpy(valStr,m_set[p_malf_set[i]].m_s_descr);	
                                        valStr[strlen(valStr)-1]='\0';
					break;
					}
				i++;
				}
			}
   	XtVaSetValues (Ctx->UxscenMalfValAtt[n],
					RES_CONVERT (XmNlabelString,valStr),NULL);

		Ctx->UxvalorePrec[n] = val_att;
		}
/*
 *  aggiornamento var. sec. dopo caricamento CI o BT (1 sola volta)
 */
	if (nuovoSnapCaricato == 1) 		/* database aggiornato   */
		{
/*		if (Ctx->UxnuovoSnapAggiornamento[n] == 0)*/	/* prima volta, rinfresco	*/
		if (Ctx->UxnuovoSnapAggiornamento[n] != nuovoSnapCaricato2)
			{
fprintf (stderr,"Refresh var. sec. dopo LOAD CI\n");
			if (Ctx->Uxscenario.rec[n].p_sec1_var >= 0)
				Ctx->Uxscenario.rec[n].valSec1 = 
								RtDbPGetValueD (dbpunti,Ctx->Uxscenario.rec[n].p_sec1_var);
			if (Ctx->Uxscenario.rec[n].p_sec2_var >= 0)
            Ctx->Uxscenario.rec[n].valSec2 =
                        RtDbPGetValueD (dbpunti,Ctx->Uxscenario.rec[n].p_sec2_var);
			if (Ctx->Uxscenario.rec[n].p_sec3_var >= 0)
            Ctx->Uxscenario.rec[n].valSec3 =
                        RtDbPGetValueD (dbpunti,Ctx->Uxscenario.rec[n].p_sec3_var);
         if (Ctx->Uxscenario.rec[n].p_sec4_var >= 0)
            Ctx->Uxscenario.rec[n].valSec4 =
                        RtDbPGetValueD (dbpunti,Ctx->Uxscenario.rec[n].p_sec4_var);
/*			Ctx->UxnuovoSnapAggiornamento[n] = 1;   */
/*
 * azzera delay e durata
 */
         Ctx->Uxscenario.rec[n].delay = 0.;
         Ctx->Uxscenario.rec[n].durata = 0.;
         Ctx->Uxscenario.rec[n].durataInf = 0; /* default durata infinita   */
         Ctx->Uxscenario.rec[n].time_to = 0.;

			Ctx->UxnuovoSnapAggiornamento[n] = nuovoSnapCaricato2;
			}
		}
	else										/* terminato aggiornamento		*/
		Ctx->UxnuovoSnapAggiornamento[n] = 0;	/* reset condizione			*/

/*
 * controlla l' area perturbazioni per verificare se la malf in questione
 *	e' stata attivata da agenti esterni
 */
	stato = Ctx->Uxscenario.rec[n].stato;	/* stato attuale della malf in elenco	*/
	stato_prec = Ctx->Uxscenario.rec[n].stato_prec;/* stato precedente malf in elenco	*/

	if ((indx = malfPertInCorso(Ctx->Uxscenario.rec[n].p_main_var,
											pertMalf,PERT_MALFUNCTION)) >= 0) 
		{
/* la malf e' perturbata: 
		si esamina lo stato attuale e si stabilisce se puo' essere congruente */
/***
printf ("updataRigaScenario: Malf [%s] perturbata\n",Ctx->Uxscenario.rec[n].label);
printf ("\t\t	stato = %d\n",stato);
***/
		switch (stato)
			{
			case SELECTED:   /* e' stata attivata da altri agenti o caricata da CI 	*/
			case -1:         /* nuova creazione													*/
				Ctx->Uxscenario.rec[n].stato = ACTIVE;
				stato = ACTIVE;
            set_colore_riga_scenario (w,n,colore_app[COLORE_ACTIVE]);
/*  printf ("\t\t	nuovo stato = %d\n",stato);  *******/
				break;
				
			}
		}
/*
 * cambi di stato ed attuazioni
 */
	switch (stato)
		{
		case SELECTED:     /* nessuna azione necessaria	*/
/* cambio colore */
         if (val_att == 0.0)	/* se e' gia' resettata		*/
				{
         	if (stato_prec != stato)  /* e' stata richiesta l' attivazione  */
               {
               set_colore_riga_scenario (w,n,sfondo);
               Ctx->Uxscenario.rec[n].stato_prec = stato;
               }
				}
			break;
		case INSERTED:
/*
 * la variabile e' stata inserita (e' stata richiesta l' attivazione)
 *		- si controlla il delay allo start
 *		- si manda la perturbazione con delay = 0
 *		- si predispone per lo stato ACTIVE
 */
      	Ctx->Uxscenario.rec[n].delay -= deltat;
/**
printf ("delay = %f\n",Ctx->Uxscenario.rec[n].delay);  **/
/* cambio colore */
			if (stato_prec != stato)  /* e' stata richiesta l' attivazione  */
               {
               set_colore_riga_scenario (w,n,colore_app[COLORE_INSERTED]);
               Ctx->Uxscenario.rec[n].stato_prec = stato;
               }
/* emissione di pert se delay inesistente o scaduto	*/
			if (Ctx->Uxscenario.rec[n].delay <= 0)    /* delay scaduto	*/
				{
				Ctx->Uxscenario.rec[n].delay = 0.0;
				addr = Ctx->Uxscenario.rec[n].p_main_var;
            valore = Ctx->Uxscenario.rec[n].target;
            printf ("Azionamento Variabile [%s]\n", Ctx->Uxscenario.rec[n].label);
				startMalfunzione (addr, valore,Ctx->Uxscenario.rec[n]);
/* cambio di stato	*/
				Ctx->Uxscenario.rec[n].stato = ACTIVE;
				}
			break;

		case REMOVED:
/* cambio colore e emissione perturbazione */
         if (stato_prec != stato)  /* e' stata richiesta l' attivazione  */
               {
               set_colore_riga_scenario (w,n,colore_app[COLORE_REMOVED]);  
               Ctx->Uxscenario.rec[n].stato_prec = stato;
					addr = Ctx->Uxscenario.rec[n].p_main_var;
					stopMalfunzione (addr);
               fprintf (stderr,"Stop Variabile [%s]\n", Ctx->Uxscenario.rec[n].label);
/* cambio di stato   */
		         Ctx->Uxscenario.rec[n].stato = SELECTED;
      			set_colore_riga_scenario (w,n,sfondo);
               }
         break;
		case ACTIVE:
/* cambio colore */
			if (val_att == Ctx->Uxscenario.rec[n].target)
				{
         	if (stato_prec != stato)  /* e' stata richiesta l' attivazione  */
               {
               Ctx->Uxscenario.rec[n].stato_prec = stato;
               }
            set_colore_riga_scenario (w,n,colore_app[COLORE_ACTIVE]);
				}
/* cambio di stato se durata scaduta	*/
			if (Ctx->Uxscenario.rec[n].durataInf > 0)
				{
         	Ctx->Uxscenario.rec[n].durata -= deltat;
         	if (Ctx->Uxscenario.rec[n].durata <= 0.0)    /* delay scaduto  */
            	{
/* cambio di stato   */
					Ctx->Uxscenario.rec[n].durata = 0.0;
					Ctx->Uxscenario.rec[n].durataInf = 0;
            	Ctx->Uxscenario.rec[n].stato = REMOVED;
            	}
				}
/* verifica se e' sparita la pert (ad opera di altro agente). In tal caso 
 *	cambia stato a SELECTED*/
		if ((indx = malfPertInCorso(Ctx->Uxscenario.rec[n].p_main_var,
                                 pertMalf,PERT_MALFUNCTION)) < 0)
      	{
/* essendo lo stato = ACTIVE mi aspetto che esista la pert in memoria.
   Se la pert non c'e' e' stata rimossa da altro utente (AING, o altro)
	In questo caso forzo lo stato SELECTED */

			Ctx->Uxscenario.rec[n].stato = SELECTED;
			}
			break;
		}
}
/***************************************************************/
int nuovoScenarioNotify (Widget scen, Widget listScen)
{
_UxCscenarioMalf *Ctx;

	Ctx = (_UxCscenarioMalf *)UxGetContext(scen);
	switch (Ctx->Uxtipo)
		{
		case MALFUNZIONI:
		case SOMMARIO_MALF:
			nuovoScenarioMalf = 1;
			break;
		case FUNZIONI_REMOTE:
      case SOMMARIO_FREM:
         nuovoScenarioFrem = 1;
         break;
		}
	return (0);
}
/***************************************************************/
/*
 * emette la malfunzione: variabile principale e secondarie
 */
int startMalfunzione (int addr, float valore, REC_SCENARIO scen)
{
int tipo;
float tempo;
float rateo_durata = 0.0;
float period = 0.0;
float phase = 0.0;
float meanvalue = 0.0;
float wide = 0.0;
float rateo = 0.0;
float t_null_var = 0.0;
float t = 0.0;
float valAtt;

/* variabile secondaria n. 1  */
	if (scen.p_sec1_var >= 0)
		{
		valAtt = RtDbPGetValueD (dbpunti,scen.p_sec1_var);
		if (valAtt != scen.valSec1)   /* filtro per pert_not */
			{
			pert_snd(id_msg_pert,     /* indice coda  */
               scen.tipoPert1,  /* tipo pert		*/
               scen.p_sec1_var, /* indirizzo		*/
               scen.valSec1,    /* valore target*/
               rateo,           /* roc				*/
               period,
					phase,
					meanvalue,
					wide,
					t_null_var,
               t);
			}
		}
/* variabile secondaria n. 2  */
   if (scen.p_sec2_var >= 0)
		{
		valAtt = RtDbPGetValueD (dbpunti,scen.p_sec2_var);
		if (valAtt != scen.valSec2)   /* filtro per pert_not */
      	{
      	pert_snd(id_msg_pert,     /* indice coda  */
               scen.tipoPert2,  /* tipo pert    */
               scen.p_sec2_var, /* indirizzo    */
               scen.valSec2,    /* valore target*/
               rateo,           /* roc          */
               period,
               phase,
               meanvalue,
               wide,
               t_null_var,
               t);
      	}
		}
/* variabile secondaria n. 3  */
   if (scen.p_sec3_var >= 0)
		{
		valAtt = RtDbPGetValueD (dbpunti,scen.p_sec3_var);
		if (valAtt != scen.valSec3)   /* filtro per pert_not */
      	{
      	pert_snd(id_msg_pert,     /* indice coda  */
               scen.tipoPert3,  /* tipo pert    */
               scen.p_sec3_var, /* indirizzo    */
               scen.valSec3,    /* valore target*/
               rateo,           /* roc          */
               period,
               phase,
               meanvalue,
               wide,
               t_null_var,
               t);
      	}
		}
/* variabile secondaria n. 4  */
   if (scen.p_sec4_var >= 0)
		{
		valAtt = RtDbPGetValueD (dbpunti,scen.p_sec4_var);
		if (valAtt != scen.valSec4)   /* filtro per pert_not */
      	{
      	pert_snd(id_msg_pert,     /* indice coda  */
               scen.tipoPert4,  /* tipo pert    */
               scen.p_sec4_var, /* indirizzo    */
               scen.valSec4,    /* valore target*/
               rateo,           /* roc          */
               period,
               phase,
               meanvalue,
               wide,
               t_null_var,
               t);
      	}
		}
/* variabile principale */
	tipo = 0;
	tempo = 0.0;
	pert_malfunction (id_msg_pert, addr, valore, tipo, tempo);

	return (0);
}
/***************************************************************/
/*
 * elimina la malfunzione: e' sufficiente portare a 0 la principale 
 *			le secondarie non influiscono
 */
int stopMalfunzione (int addr)
{
int tipo;
float tempo;
float val;

/* variabile principale */
   tipo = 0;
   tempo = 0.0;
	val = 0.0;
   pert_malfunction (id_msg_pert, addr, val, tipo, tempo);
}
