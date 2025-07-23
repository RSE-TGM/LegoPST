/**********************************************************************
*
*	C Source:		client_scada_func.c
*	Subsystem:		4
*	Description:	
*	%created_by:	lomgr %
*	%date_created:	Thu Jul  1 17:07:51 2004 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: client_scada_func.c-31 %  (%full_filespec: client_scada_func.c-31:csrc:4 %)";
#endif

#include <stdio.h>
#include <errno.h>

/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)client_scada_func.c	1.20\t5/23/95";
/*
        Fine sezione per SCCS
*/
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <libnet.h>
#include <Cs/Cs.h>

#include <client_scada.h>
#include <client_s.h>
#include <sim_param.h>
#include <sim_types.h>

/************************************
Funzioni per gestione lock miniASD
************************************/
/*
Ogni volta che viene fatta una richiesta si setta lock_miniASD ad un valore !=0
e viene settata a 0 non appena la richiesta e' stata soddisfatta.
Fino a quel momento nessun miniASD puo' fare un' altra richiesta.
All' interno della struttura di lock e' gestito anche il timeout.
*/
static int unset_lock_miniASD(void);
static int set_lock_miniASD(int);
static int lock_enabled(void);
static int status_lock_miniASD(void);
extern int sgancia_shrmem(char *);
extern void elimina_shrmem(int,char*,int);

typedef struct flag_digitale_st{
                unsigned short sl : 1;   /* stato logico corrente */
                unsigned short ri : 1;
                unsigned short au : 1;
                unsigned short in : 1;
                unsigned short st : 1;
                unsigned short fz : 1;   /* forzato */
                unsigned short fa : 1;   /* fuori attendibilita' */
                unsigned short fs : 1;   /* fuori scansione */
                unsigned short a0 : 1;
                unsigned short a1 : 1;
                unsigned short a2 : 1;
                unsigned short a3 : 1;
                unsigned short a4 : 1;
                unsigned short a5 : 1;
                unsigned short a6 : 1;
                unsigned short a7 : 1;
                } FLAG_DIGITALE;

extern ZONE zone[MAX_ZONE];
extern int fp; /* canale comunicaz. */
extern pthread_mutex_t fp_mutex;
extern pthread_mutex_t canale_mutex;
extern int numero_mmi;
extern LOCK_MINIASD lock_miniASD;
extern int sem_mmi, sem_scada, sem_scada_key, sem_mmi_key;

/*
Identificatori della shm asociate ad una OW
*/
int  id_allarm_data, id_manual_data;
int modo_trend=0;
/*
Variabili per riconoscimento miniASD.

Viene inviata da mmi la richiesta di riconoscimento per un certo No di
allarmi. client_scada deve,per ogni allarme, inviare msg a SCADA per ric.
ed aspettare la risposta. Durante questo periodo, ogni altro riconosci=
mento e' impedito da next_ric=0. Quando num_allar_miniASD==prog_allar_miniASD
si e' esaudita la richiesta di riconoscimento dell' mmi.
*/
int num_allar_miniASD=0;  /* num allar da ric per miniASD */
int prog_allar_miniASD=0; /* progressivo allar da ric per miniASD */
int next_ric=1;           /* abilita riconoscimento next alarm */

/*
Passata come variabile globale (prima era di ins_dati_allarmi LOCALE) 
perche' altrimenti si hanno problemi sullo stack
*/
S_VDEAL allarmi;
S_VDEAL allarmi_miniASD;
/*
Messaggio per aggiornamento stack 
( tolto da main_data per stack troppo grosso)
*/
S_VDASI m_vdasi;

/*
	variabili per la gestione degli allarmi
*/
extern int id_shm_allarmi;
extern int shmvid_allarmi;
extern char *punt_shm_allarmi;   /* shm principale (db 1) */




int db_allarmi;

int allarmi_a,allarmi_da,operazione_allarmi,zone_impianto[32];
int modo=0;   /* modo = 0 -> allarmi    modo = 1 -> archivi  */


static int estrai_campioni(char  *,int,int,float,int);
extern int tab_piena(TAB_SHARED *);
extern int send_mmi_ret_ack(int ,int );
extern int cerca_shm(TAB_SHARED *, int );
TAB_SHARED tabella[MAX_OPERAZIONI];  /* tabella che tiene conto degli id 
                                        delle shm */
char *crea_shrmem(int,int,int *);
int invio_richiesta_nuova_pagina(int fp,pthread_mutex_t *fp_mutex,
                char *nome_pagina,int zona,int video);
int CsRcvMsgDel(int coda);
int cerca_zona(int id_shm);
int invio_richiesta_canc_pagina(int fp,pthread_mutex_t *fp_mutex,
                int zona,int video);
int CsRcvMsgChangePoint(int coda,MSG_CHANGE_POINT_MMI *msg);
int invio_riconoscimento_allarmi_miniASD(int fp,pthread_mutex_t *fp_mutex,
                int video,char* pagina,int db,unsigned char *gerarchia,
                DATI_ALLARMI_SHM *data_all,int modo,
                int filtro,int tipo_all,int indice_all);
int invio_richiesta_allarmi_miniASD(int fp,pthread_mutex_t *fp_mutex,int video,
                char* pagina,int db, unsigned char *gerarchia,int modo,
                int filtro,int tipo_allarme);

int destroy_shm_allarmi_miniASD(int id_allarm,int id_manual);
int invio_richiesta_allarmi_zone(int fp,pthread_mutex_t *fp_mutex,int video,
                char* pagina,int db, int *zone_imp,int modo);
extern int CsRcvMsgAllCommand(int coda,MSG_ALL_COMMAND_MMI *msg);
int invio_comando_allarmi(int fp,pthread_mutex_t *fp_mutex,int video,
                char* pagina,int db, int comando);
int read_messaggio_scada(int fp,pthread_mutex_t *fp_mutex, char* buf);






                
//void *main_client_scada_command(pthread_addr_t arg)
void *main_client_scada_command(void *arg)
{
float a;
int id_msg_client;
char *buffer=NULL;
int size;
TOKEN *app_token;
HEADER_NEW_PAGE_MMI *app_header;
HEADER_NEW_PAGE_MMI header;
MSG_CHANGE_POINT_MMI msg_point;
MSG_TREND_REQ_MMI msg_trend;
MSG_ALL_REQ_MMI msg_all;
MSG_ALL_MINIASD_REQ_MMI msg_all_ASD;
MSG_ALL_MINIASD_DES_MMI msg_des_ASD;
MSG_ALL_MINIASD_RIC_MMI msg_ric_ASD;
MSG_ALL_COMMAND_MMI msg_all_command;
MSG_F22_MMI msg_f22;
DATI_ALLARMI_SHM dati_all;
int id_shm;
int i;
char *ind;
int zona_libera;
int sblocco_canale=1;
char nome_pagina[l_pagina];
int mess_ricevuto=0;
int comando;
int appo_shr_usr_keyS;

static int prima_volta=1;
/*
Variabili per potere agganciare la shm per riconoscimento 
allarmi
*/
DATI_ALLARMI_SHM *punt_data_all;
int shmvid;
int ind_tab=0;


/* Inizializzo tabella id_shm */
if(prima_volta)
   {
   prima_volta=0;
   for(i=0;i<MAX_OPERAZIONI;i++)
      {
      tabella[i].id_shm=-1;
      tabella[i].ind=NULL;
      tabella[i].num_punti=0;
      }

   db_allarmi=1;
   }


if(getenv("SHR_USR_KEYS")==NULL)
        {
        printf("client_mmiS: ATTENZIONE SHR_USR_KEYS non settata !!!\n");
        exit(0);
        }
appo_shr_usr_keyS=atoi(getenv("SHR_USR_KEYS"));

/*
        apertura coda ricevimento comandi
	La KEY associata alla coda e' funzione della SHR_USR_KEYS
	per poter avere + mmi sulla stessa macchina
*/

// printf("CLIENT_SCADA: id coda =%d\n",appo_shr_usr_keyS+FROM_MMI_MSGS);
id_msg_client=msg_create(appo_shr_usr_keyS+FROM_MMI_MSGS,0);
// printf("CLIENT_SCADA: id_msg_client=%d\n",id_msg_client);


unset_lock_miniASD(); /* richiesta unlocked */
while(1)
        {
/*      Verifica la presenza di messaggi in coda */
	if(mess_ricevuto ==0)
             pthread_mutex_lock(&canale_mutex);
	mess_ricevuto = 0;
        if((id_shm=CsRcvTokenS(id_msg_client,&buffer))>0)
                {
		mess_ricevuto = 1;
                memcpy(&header,buffer,sizeof(HEADER_NEW_PAGE_MMI));
                size = sizeof(HEADER_NEW_PAGE_MMI) + header.size;
// printf("CLIENT_SCADA_COMMAND (CsRcvToken) id_shm=%d size=%d buffer=%s\n", id_shm, size, buffer);

                /* Accedo all' indice in tabella libero */
                 if( (ind_tab=tab_piena(tabella))==-1 )
                        {
                        printf("ERROR: Reached maximun number of page\n");
                        break;
                        }

                /*
                Creazione della shm
                */
                tabella[ind_tab].id_shm=appo_shr_usr_keyS+OFFSETMMIPAGE+ind_tab+1;
                id_shm=tabella[ind_tab].id_shm;
                tabella[ind_tab].num_punti = header.num_punti;
                tabella[ind_tab].ind=(char*) crea_shrmem(id_shm,
                            (header.num_punti*2+1)*sizeof(float),&shmvid);

	       if(tabella[ind_tab].ind ==NULL)
                  {
                  printf("client_scada:ERROR impossible to open SHM with key=%d\n",tabella[ind_tab].id_shm);
                  break;
                  }
                 

                if(sgancia_shrmem(tabella[ind_tab].ind)<0)
                   {
                   printf("client_mmi:ERRORE command: sgancia_shrmem ind=%d\n",tabella[i].ind);
                        break;
                   }

                /*
                Invio ad mmi msg di ritorno 
                */ 

                send_mmi_ret_ack(id_msg_client,id_shm);


                app_token  = (TOKEN*)&buffer[sizeof(HEADER_NEW_PAGE_MMI)];
		
		/*
                for(i=0;i<header.num_punti;i++)
                        {
			printf("Tipo %d ind = %d\n",
				app_token[i].tipo,app_token[i].puntdb);
                        }
		*/
		

		/*
			verifico che se la richiesta riguarda
			l'aggiornamento del tempo.
			Modifica del 1/8/96: test con REQ_TIME invece di 1
		*/
		if((header.num_punti==1)&&
			(app_token[0].puntdb==REQ_TIME)) 
		   {
	           zone[ZONA_TEMPO-1].id_shm=id_shm;
		   zone[ZONA_TEMPO-1].stato=1;
		   zone[ZONA_TEMPO-1].size=
				(header.num_punti*2+1)*sizeof(float);
		   zone[ZONA_TEMPO-1].ind=
				(char*)crea_shrmem(id_shm,
                                           zone[ZONA_TEMPO-1].size,
                                           &(zone[ZONA_TEMPO-1].shmvid));
                   printf("client scada : allocata SHMS per zona Tempo\n");
		   }
		else
   		   {
	           /* 
			cerco la prima zona libera
 		   */
		   zona_libera=cerca_zona_libera();
		   if(zona_libera > 0)
			{
			zone[zona_libera-1].id_shm=id_shm;
			zone[zona_libera-1].stato=1;
			zone[zona_libera-1].t_agg=header.t_agg/10;
			if(zone[zona_libera-1].t_agg<1)
				zone[zona_libera-1].t_agg=1;
			zone[zona_libera-1].size=
                                (header.num_punti*2+1)*sizeof(float);
			zone[zona_libera-1].ind=
				(char*)crea_shrmem(id_shm,
                                           zone[zona_libera-1].size,
                                           &(zone[zona_libera-1].shmvid));
			printf("zone[zona_libera-1].ind = %d\n",
				zone[zona_libera-1].ind);
			printf("Inserisco la pagina nella zona %d\n",zona_libera);
			sprintf(nome_pagina,"pag_%d",zona_libera);
			ins_token_zona(zona_libera,header.num_punti,app_token);
			if(invio_richiesta_nuova_pagina(fp,&fp_mutex,
                			nome_pagina,zona_libera,numero_mmi)<0)
			  warning("invio_richiesta_nuova_pagina");
			}
		   else
			warning("Raggiunto il massimo numero di zone occupate"); 
                   }
                if(buffer!=NULL)
                   free(buffer);
                }
        /*else*/
           if((header.id_shm=CsRcvMsgDel(id_msg_client))>0)
                {
		mess_ricevuto = 1;
                header.num_punti = 0;
                header.t_agg = 0;
                header.size  = 0;
                header.richiesta_mmi = DEL_PAGE_MMI;
                CONVERTI_INT_T(header.num_punti);
                CONVERTI_INT_T(header.t_agg);
                CONVERTI_INT_T(header.size);
                CONVERTI_INT_T(header.id_shm);
                CONVERTI_INT_T(header.richiesta_mmi);
                

                if( (ind_tab=cerca_shm(tabella,header.id_shm))==-1)
                       {
                       printf("client_scada:SHM to delete not found !!! \n");
                       break;
                       }


		zona_libera=cerca_zona(header.id_shm);
		if(zona_libera>0)
			{
	        	invio_richiesta_canc_pagina(fp,&fp_mutex,
                		zona_libera,numero_mmi);
			/*
				elimino la shared memory
			*/
			sgancia_shrmem(zone[zona_libera-1].ind);
			elimina_shrmem(zone[zona_libera-1].shmvid,
					zone[zona_libera-1].ind,
                                       zone[zona_libera-1].size);
			/*
				libero la zona
			*/
			zona_libera=libera_zona(header.id_shm);		
                        if(!shresist(tabella[ind_tab].id_shm))
                           tabella[ind_tab].id_shm = -1;
                        else
                           printf("ERROR:client_mmi_command: shm=%d not destroyed\n",tabella[ind_tab].id_shm);
			}
                }
           if(CsRcvMsgChangePoint(id_msg_client, &msg_point)>0)
                {
		mess_ricevuto = 1;
		printf("Ricevuto messaggio change point\n");
                header.num_punti = 3;
                header.t_agg = 0;
                header.size  = 0;
                header.richiesta_mmi = CHANGE_POINT_MMI;
                header.id_shm = msg_point.id_shm;
		zona_libera=cerca_zona(header.id_shm);
		if(zona_libera>0)
			{
	        	invio_richiesta_canc_pagina(fp,&fp_mutex,
                		zona_libera,numero_mmi);
			change_token_zona(zona_libera,
				msg_point.posizione,msg_point.indirizzo,
				msg_point.tipo);
			if(invio_richiesta_nuova_pagina(fp,&fp_mutex,
                			nome_pagina,zona_libera,numero_mmi)<0)
			  warning("invio_richiesta_nuova_pagina");
			}
                }
/*
Distinguo due casi possibili:
1) Riconoscimento di un solo allarme (indice_all>=0)
2) Riconoscimento di tutti gli allarmi presenti nella shm (indice_all=OPERAZIONERICALL)
*/
        if(!lock_enabled() &&(CsRcvMsgMiniASDRic(id_msg_client, &msg_ric_ASD)>0))
                {
                /* riconoscimento multiplo allarme da miniASD */
                mess_ricevuto = 1;
                if(msg_ric_ASD.indice_all==OPERAZIONERICALL)
                   {
                   printf("client_scada:Ricevuto msg di Ric tot\n"); 
                   set_lock_miniASD(LOCKRICALL);
                   num_allar_miniASD=msg_ric_ASD.n_all;
                   next_ric=1; /* abilito ric prossimo all */
                   fprintf(stderr,"client_scada:n_all=%d\n",num_allar_miniASD);
                   }
                else
                   {
                   fprintf(stderr,"client_scada:Received msg of Ric sing\n"); 
                   /* lock di ulteriori richieste */
                   operazione_allarmi=ALL_RIC_MINIASD;
                   set_lock_miniASD(LOCKRIC);
                   
                   punt_data_all=(DATI_ALLARMI_SHM *)
                               crea_shrmem(msg_ric_ASD.id_data_all,
                                      sizeof(DATI_ALLARMI_SHM),&shmvid);
                   /* snd msg di riconosc. a SCADA */
                   if(invio_riconoscimento_allarmi_miniASD(fp,&fp_mutex,
                      numero_mmi,"$MINIASD",1,msg_ric_ASD.gerarchia,
                      punt_data_all,
                      MRICSMINIASD,msg_ric_ASD.filtro,msg_ric_ASD.tipo_allarme,
                      msg_ric_ASD.indice_all)<0)
                         {
                         unset_lock_miniASD();
                         fprintf(stderr,"ERROR: Impossible to send msg \
                                 for ack miniASD\n");
                         }
                    sgancia_shrmem((char*)punt_data_all);
                    }
                }
/*
Parte per il riconoscimento di tutti gli allarmi
Se ho ancora allarmi da riconoscere (num_allar_miniASD>0)
e se sono in riconoscimento totale miniASD status_lock_miniASD()== LOCKRICALL
e se la precedente operazione di riconoscimento e' stata OK (next_ric)
esegui un nuovo riconoscimento 
*/
        if( (num_allar_miniASD>0) && 
           (status_lock_miniASD()== LOCKRICALL) && next_ric)
           {
           fprintf(stderr,"client_scada: ack alarm :prog_all_miniASD=%d\n",
                 prog_allar_miniASD);
           next_ric=0; /* disabilito ric prossimo all */
           /*
            Fase di aggancio alla shm solo quando esamino il 1o all.
           */
           if(prog_allar_miniASD==0)
               punt_data_all=(DATI_ALLARMI_SHM *)
                        crea_shrmem(msg_ric_ASD.id_data_all,
                                         sizeof(DATI_ALLARMI_SHM),&shmvid);

           if(invio_riconoscimento_allarmi_miniASD(fp,&fp_mutex,
              numero_mmi,"$MINIASD",1,msg_ric_ASD.gerarchia,
              punt_data_all,
              MRICSMINIASD,msg_ric_ASD.filtro,msg_ric_ASD.tipo_allarme,
              prog_allar_miniASD)<0)
              {
              /*
              Gestisco errore riabilitando i lock
              */
              next_ric=1;
              unset_lock_miniASD();
              prog_allar_miniASD=0;
              fprintf(stderr,"ERROR global ackn. miniASD\n");
              }
           else
              {
              /*printf("DEBUG: Ho inviato ricon.all. prog_allar=%d  num_allar=%d\n",prog_allar_miniASD,num_allar_miniASD);*/
              prog_allar_miniASD++;
              num_allar_miniASD--;
              if(! num_allar_miniASD)
                 {
                 prog_allar_miniASD=0;
                 unset_lock_miniASD();
                 sgancia_shrmem((char*)punt_data_all);
                 }
              }
           }

        if(!lock_enabled() &&(CsRcvMsgMiniASDReq(id_msg_client, &msg_all_ASD)>0))
                {
printf("client_scada: In lock_enabled() &&(CsRcvMsgMiniASDReq \n");
                mess_ricevuto = 1;
                /* lock di ulteriori richieste in base al filtro */
                if(msg_all_ASD.filtro==ALARMREQ)
                    set_lock_miniASD(LOCKALARMREQ);
                else if(msg_all_ASD.filtro==MANUALREQ)
                    set_lock_miniASD(LOCKMANUALREQ);
                operazione_allarmi=ALL_REQ_MINIASD;
                if(invio_richiesta_allarmi_miniASD(fp,&fp_mutex,
                   numero_mmi,"$MINIASD",1,msg_all_ASD.gerarchia,
                   MPALMINIASD,msg_all_ASD.filtro,msg_all_ASD.tipo_allarme)<0)
                        {
                        unset_lock_miniASD();
                        printf("Errore richiesta allarmi mini ASD\n");
                        }
                else
                   {
                   id_allarm_data=msg_all_ASD.id_allarm_data;
                   id_manual_data=msg_all_ASD.id_manual_data;
                   /*printf("client_scada:id_allarm_data=%d id_manual_data=%ld\n",id_allarm_data,id_manual_data);*/
                   }
printf("client_scada: Dopo invio_richiesta_allarmi_miniASD \n");

                }


        if(!lock_enabled() &&(CsRcvMsgMiniASDDes(id_msg_client, &msg_des_ASD)>0))
                {
printf("client_scada: In lock_enabled() &&(CsRcvMsgMiniASDDes \n");
                mess_ricevuto = 1;
                /* lock di ulteriori richieste in base al filtro */
                    set_lock_miniASD(LOCKALARMDES);
                if(destroy_shm_allarmi_miniASD(msg_des_ASD.id_allarm,
                                          msg_des_ASD.id_manual)<0)
                        {
                        unset_lock_miniASD();
                        printf("Errore  distruzione shm  allarmi mini ASD\n");
                        }
                else
                   {
printf("client_scada: In lock_enabled() &&(CsRcvMsgMiniASDDes else di if\n");
                   unset_lock_miniASD();
                   }
               }


//printf("client_scada: Prima di CsRcvMsgAllReq(id_msg_client\n");

        if(CsRcvMsgAllReq(id_msg_client, &msg_all)>0)
                {
		memset(punt_shm_allarmi,0,sizeof(DATI_ALLARMI_SHM));
		mess_ricevuto = 1;
		zone[1].id_shm=1;
		zone[1].stato=1;
		zone[1].t_agg=1;
		zone[1].size=0;
		zone[1].ind = (char *)-1; // Assegna un valore puntatore non-NULL ma riconoscibile come fittizio
		zone[1].num_punti=0;
		allarmi_da=1;
   		memcpy(&dati_all,punt_shm_allarmi,sizeof(DATI_ALLARMI_SHM));
   		dati_all.dati_validi=DATI_ALLARMI_NON_VALIDI;
   		memcpy(punt_shm_allarmi,&dati_all,sizeof(DATI_ALLARMI_SHM));
		operazione_allarmi=ALL_REQ_ALLARMI;
                invio_richiesta_canc_pagina(fp,&fp_mutex,
                        (int)2,(int)numero_mmi);

// printf("CLIENT_SCADA: prima di invio_richiesta_allarmi_zone\n");

		if(invio_richiesta_nuova_pagina(fp,&fp_mutex,
                	"$ALLARMI",2,numero_mmi)<0)
			  warning("invio_richiesta_nuova_pagina");

// printf("CLIENT_SCADA: Ricevuto messaggio richiesta allarmi db=%d\n",msg_all.db);

                db_allarmi = msg_all.db;

                if(invio_richiesta_allarmi_zone(fp,&fp_mutex,
                        numero_mmi,"$ALLARMI",msg_all.db,msg_all.zone,0)<0)
                        printf("Errore richiesta allarmi\n");
// printf("CLIENT_SCADA: invio_richiesta_allarmi_zone:eseguita\n");

		memcpy(zone_impianto,msg_all.zone,32*sizeof(int));
		}

        if(CsRcvMsgAllCommand(id_msg_client, &msg_all_command)>0)
                {
		mess_ricevuto = 1;
		printf("Ricevuto messaggio comando allarmi [%d]\n",
				msg_all_command.comando);
		if(msg_all_command.comando==ALL_REQ_ARCHIVI)
		   {
		   printf("Passaggio in modo ARCHIVI\n");
		   allarmi_da=1;
		   modo =1;
		   operazione_allarmi=ALL_REQ_ARCHIVI;
                   if(invio_richiesta_allarmi_zone(fp,&fp_mutex,
                        numero_mmi,"$ALLARMI",1,zone_impianto,1)<0)
                        printf("Errore richiesta archivi\n");
		   }
		if(msg_all_command.comando==ALL_REQ_ALLARMI)
		   {
		   allarmi_da=1;
		   modo=0;
		   operazione_allarmi=ALL_REQ_ALLARMI;
                   if(invio_richiesta_allarmi_zone(fp,&fp_mutex,
                        numero_mmi,"$ALLARMI",msg_all_command.db,zone_impianto,0)<0)
                        printf("Errore richiesta allarmi\n");
		   printf("Passaggio in modo ALLARMI\n");
		   }
		if(msg_all_command.comando==ALL_REQ_AVANTI)
		   {
		   printf("Passaggio alla pagina successiva\n");
		   comando=3;
		   operazione_allarmi=ALL_REQ_AVANTI;
                   if(invio_comando_allarmi(fp,&fp_mutex,
                        numero_mmi,"$ALLARMI",msg_all_command.db,comando)<0)
                        printf("Errore comando allarmi\n");
		   }
		if(msg_all_command.comando==ALL_REQ_INDIETRO)
		   {
		   printf("Passaggio alla pagina precedente\n");
		   comando=4;
		   operazione_allarmi=ALL_REQ_INDIETRO;
                   if(invio_comando_allarmi(fp,&fp_mutex,
                        numero_mmi,"$ALLARMI",msg_all_command.db,comando)<0)
                        printf("Errore comando allarmi\n");
		   }
		if(msg_all_command.comando==ALL_REQ_CHIUDI)
		   {
		   printf("Chiusura pagina allarmi\n");
		   operazione_allarmi=ALL_REQ_CHIUDI;
                   invio_richiesta_canc_pagina(fp,&fp_mutex,
                        2,numero_mmi);
		   }
		if(msg_all_command.comando==ALL_REQ_RIC)
		   {
		   comando=2;
		   operazione_allarmi=ALL_REQ_RIC;
                   if(invio_comando_allarmi(fp,&fp_mutex,
                        numero_mmi,"$ALLARMI",msg_all_command.db,comando)<0)
                        printf("Errore comando allarmi\n");
		   }
		}
	if(mess_ricevuto == 0)
	{
        if(sblocco_canale == 1)
        	pthread_mutex_unlock(&canale_mutex);
	}
	sblocco_canale = 1;
        p_sospendi(100);
        }
return NULL; 
}

// GUAG2025 porting da dcethread a pthread  
//void *main_client_scada_data(pthread_addr_t arg)
void *main_client_scada_data(void *arg)
{
S_VTIME time;
S_HEAHOST hea;
S_VCLRV m_vclrv;
S_VDEAL m_vdeal;
S_VPAGV m_vpagv;
int ret;
//int my_number;
char buf[MAX_DIM_MESSAGGIO_SCADA+1000];
short giorno,mese,anno,ore,minuti,secondi;
float tempo_sim;
short nodo;
char indice;
short size;
float dato[255];
int i;
DATI_ALLARMI_SHM dati_all;
int kk;
short buf_len;
int my_number = (int)(intptr_t)arg; // Modo più sicuro per castare

while(1)
        {
	p_sospendi(10);
        pthread_mutex_lock(&canale_mutex);
        ret=read_messaggio_scada(fp, &fp_mutex, buf);
        memcpy(&buf_len, buf, sizeof(short));
// printf("main_client_scada_data ---> ricevuto messaggio da SCADA: ret=%i buf=%hd\n", ret, buf_len);
	if(modo_trend==1)
		ret = 0;

        if(ret == vpagv)
                {
                memcpy(&m_vpagv,buf,sizeof(S_VPAGV));
//                printf("S_VPAGV: zona %d video %d pagina %s elenco %s\n",
//                        m_vpagv.zona,m_vpagv.video,m_vpagv.page,m_vpagv.elen);
/*****
		if(strcmp(m_vpagv.page,"$ALLARMI")==0)
		   {
   		   memcpy(&dati_all,punt_shm_allarmi,sizeof(DATI_ALLARMI_SHM));
   		   dati_all.dati_validi=DATI_ALLARMI_VALIDI;
   		   memcpy(punt_shm_allarmi,&dati_all,sizeof(DATI_ALLARMI_SHM));
		   }
********/
                }
        if(ret == vpaal)
                {
                memcpy(&m_vdeal,buf,sizeof(S_VDEAL));
/*
                printf("S_VDEAL: aggiornamento parziale\n");
                printf("     video=%d pagina=%s ndb=%d codice=%d arch=%d\n", m_vdeal.video,m_vdeal.pag,m_vdeal.n_dataBase, m_vdeal.n_codice,m_vdeal.n_arch);
                printf("     all da ric=%d all arch=%d num=%d\n", m_vdeal.n_pall,m_vdeal.n_parch,m_vdeal.n_all);
		printf("Situazione all PARZIALE x zone\n");

		for(kk=0;kk<5;kk++)
			{
			printf("ZONE[%d] allpen=%d allarc=%d\n",kk,m_vdeal.zone_all.n_all_pen[kk],m_vdeal.zone_all.n_all_arc[kk]);
			}
*/

                if( m_vdeal.n_dataBase == db_allarmi )
			ins_parziale_dati_allarmi(m_vdeal);

                /*
                Permetto una nuova richiesta per miniASD
                */
                if(status_lock_miniASD() == LOCKRIC)
                     unset_lock_miniASD();
                else if((status_lock_miniASD()==LOCKRICALL) &&
                          (next_ric==0))
                         next_ric=1;
                }

        if(ret == vdeal)
                {
//                printf("S_VDEAL: aggiornamento totale\n");
                memcpy(&m_vdeal,buf,sizeof(S_VDEAL));
/*
                printf("     video=%d pagina=%s ndb=%d codice=%d arch=%d\n", m_vdeal.video,m_vdeal.pag,m_vdeal.n_dataBase, m_vdeal.n_codice,m_vdeal.n_arch);
                printf("     all da ric=%d all arch=%d num=%d\n", m_vdeal.n_pall,m_vdeal.n_parch,m_vdeal.n_all);
*/

                if(!strncmp( m_vdeal.pag,"$MINIASD",8))
                   {
                   /* Disinnesco timeout */
                   printf("Ricevuto msg per miniASD\n");
                   /*
                   in base al messaggio decido se inserire in
                   allarm_data o in manual_data
                   */
                   if(status_lock_miniASD()==LOCKALARMREQ)
                      {
                      /* Acqusisco controllo sem.*/
                      sem_wait(sem_scada);
                      ins_dati_miniASD(&m_vdeal,id_allarm_data);
                      }
                   else if(status_lock_miniASD()==LOCKMANUALREQ)
                      {
                      ins_dati_miniASD(&m_vdeal,id_manual_data);
                      /*
                      Abilito semaforo per lettura da mmi
                      */
// printf("CLIENT_SCADA: prima di sem_signal(sem_mmi)\n");
                      sem_signal(sem_mmi);
// printf("CLIENT_SCADA: dopo sem_signal(sem_mmi)\n");
                      }
                   /*
                   rendo disponibile un' altra richiesta
                   */
                   unset_lock_miniASD();
                   }
                else   
                    {
                    
                if( m_vdeal.n_dataBase == db_allarmi )
		    ins_dati_allarmi(&m_vdeal);
                    /*
                    Sgancio lock miniASD per ricon. se settato:
                    se lock_miniASD servizio ric completato 
                    */
                    if(status_lock_miniASD()==LOCKRIC)
                       unset_lock_miniASD();
                    else if((status_lock_miniASD()==LOCKRICALL) &&
                            (next_ric==0))
                            next_ric=1;

                    }
                }
        if(ret == vdasi)
                {
                memcpy(&m_vdasi,buf,sizeof(S_VDASI));


/*
printf("messaggio S_VDASI\n");
printf("S_VDASI: zona=%d video=%d pagina=%s num_byte=%d\n",
        m_vdasi.zona,m_vdasi.video,m_vdasi.pag, m_vdasi.num_byte);			
*/


/*
	prima era 2
*/
                if(m_vdasi.zona!=999)
		  {
		  if(m_vdasi.zona!=2)
			{
		  	if(ins_dati_shm(m_vdasi.zona,m_vdasi.dati)<0)
			     warning("ins_dati_shm");
			}
		   else
			{
                        if(ins_dati_shm_all(m_vdasi.dati)<0)
                             warning("ins_dati_shm_all");
                        }
/************
                  for(i=0;i<m_vdasi.num_byte/6;i++)
                        {
                        memcpy(&dato[i],&(m_vdasi.dati[i*6]),sizeof(float));
                        utov_float(&dato[i],&dato[i]);
                        printf("     dato[%d]=%f\n",i,dato[i]);
                        }
***********/
		  }
                else
                   invio_richiesta_canc_pagina(fp,&fp_mutex,
                        (int)m_vdasi.zona,(int)m_vdasi.video);
                }
        if(ret == vclrv)
                {

                memcpy(&m_vclrv,buf,sizeof(S_VCLRV));
                printf("S_VCLRV: zona =%d video = %d (%d,%d,%d,%d,%d)\n",
                        m_vclrv.zona,m_vclrv.video,m_vclrv.funzione[0],
                        m_vclrv.funzione[1],m_vclrv.funzione[2],
                        m_vclrv.funzione[3],m_vclrv.funzione[4]);
                }
        if(ret == hostmaster)
                {
               printf("HOSTMASTER\n");
                }

        if(ret == vtime)
                {
                memcpy(&time,buf,sizeof(S_VTIME));
                nodo=time.hea.nodo;
                indice=time.hea.indice;
                ore=time.ore;
                minuti=time.minuti;
                secondi=time.secondi;
                giorno=time.giorno;
		tempo_sim=conv_t_sim(giorno,ore,minuti,secondi);
// printf("tempo_sim=%f\n",tempo_sim);
		if(ins_dati_shm(ZONA_TEMPO,&tempo_sim)<0)
			warning("ins_dati_shm");
                }
	pthread_mutex_unlock(&canale_mutex);
        }
    printf("Terminato thread leggi\n");
    pthread_exit((void *)(intptr_t)my_number); // Modo più sicuro per ritornare un valore

// printf("Terminato thread leggi\n");
// pthread_exit(my_number);

return NULL;

}
                       

                                                           
int cerca_zona_libera()
{
int i;
for(i=1;i<MAX_ZONE+1;i++)
	{
	if(i==ZONA_ALLARMI) continue; /* zona riservata alla pagina allarmi */
	if(zone[i-1].stato==0)
		return(i);
	}
return(-1);
}                                                                   

int libera_tutte_le_zone()
{
int i,j;

for(i=0;i<MAX_ZONE;i++)
	{
	zone[i].stato=0;
	zone[i].id_shm=0;
	zone[i].shmvid=-1;
	zone[i].ind=NULL;
	for(j=0;j<MAX_PUNTI_SCADA;j++)
		{
		zone[i].tipo[j]=0;
		zone[i].pos[j]= -1;
		zone[i].puntdb[j]= -1;
		}
	}
}

int libera_zona(int id_shm)
{
int j,i;

for(i=0;i<MAX_ZONE;i++)
	if(zone[i].id_shm == id_shm)
		{
		zone[i].stato=0;
		zone[i].id_shm=0;
		zone[i].shmvid=-1;
		zone[i].ind=NULL;
		for(j=0;j<MAX_PUNTI_SCADA;j++)
			{
			zone[i].tipo[j]=0;
			zone[i].pos[j]= -1;
			zone[i].puntdb[j]= -1;
			}
		return(i+1);
		}
warning("libera_zona: zona non trovata"); 
return(-1);
}



int cerca_zona(int id_shm)
{
int j,i;

for(i=0;i<MAX_ZONE;i++)
	if(zone[i].id_shm == id_shm)
		return(i+1);
warning("cerca_zona: zona non trovata"); 
return(-1);
}


int change_token_zona(int zona,int pos, int puntdb, int tipo)
{
int i;

// printf("change_token_zona pos = %d puntdb = %d tipo = %d\n", pos,puntdb,tipo);
for(i=0;i<zone[zona-1].num_punti;i++)
	if(zone[zona-1].pos[i] == pos)
		{
		zone[zona-1].tipo[i]=tipo;
		zone[zona-1].puntdb[i]=puntdb;
		return(1);
		}
return(-1);
}

int ins_token_zona(int zona,int num_punti,TOKEN *token)
{
int i,j;
// printf("Inserisco i Token nella zona\n");

j=0;
for(i=0;i<num_punti;i++)
	if(token[i].puntdb >= 0)
		{
		zone[zona-1].tipo[j]=token[i].tipo;
		zone[zona-1].puntdb[j]=token[i].puntdb;
		zone[zona-1].pos[j]=i;
		j++;
		}
zone[zona-1].num_punti = j;
// printf("la zona %d contiene %d punti\n",zona,zone[zona-1].num_punti);
}

//GUAG2025 migrazione a pthreads
// NUOVA VERSIONE STANDARD POSIX (DA USARE)
void p_sospendi(int millisecondi)
{
    struct timespec interval;

    if (millisecondi <= 0) {
        return;
    }

    interval.tv_sec = millisecondi / 1000;
    interval.tv_nsec = (millisecondi % 1000) * 1000000L;
    
    // nanosleep è la funzione standard POSIX per questo scopo.
    nanosleep(&interval, NULL);
}

// void p_sospendi(int time)
// {
// struct timespec interval;
// float ftime;

// ftime = (float)time * 1000000;

// interval.tv_sec = ftime/1000000000;
// interval.tv_nsec = ftime - interval.tv_sec*1000000000;
// pthread_delay_np(&interval);
// }

int readn_i(fd,fd_mutex,ptr,nbytes)
int fd;
pthread_mutex_t *fd_mutex;
char *ptr;
int nbytes;
{
int nleft,nread;
nleft=nbytes;
while(nleft>0)
        {
        if(nleft>MAX_DIM_BUFFER_RETE)
                {
                pthread_mutex_lock(fd_mutex);
                nread=read(fd,ptr,MAX_DIM_BUFFER_RETE);
		if(nread<0)
			errore("connection width scada");
                pthread_mutex_unlock(fd_mutex);
                p_sospendi(1);
                }
        else
                {
                pthread_mutex_lock(fd_mutex);
                nread=read(fd,ptr,nleft);
		if(nread<0)
			errore("connection width scada");
                pthread_mutex_unlock(fd_mutex);
                p_sospendi(1);
                }
        if(nread<0)
                return(nread);
        else if (nread==0)
                break;
        nleft-=nread;
        ptr+=nread;
        }
return(nbytes-nleft);
}

int writen_i(fd,fd_mutex,ptr,nbytes)
int fd;
pthread_mutex_t *fd_mutex;
char *ptr;
int nbytes;
{
int nleft,nwrite;
nleft=nbytes;
if(pthread_mutex_lock(fd_mutex)<0)
	{
	printf("ERRORE nella writen_i\n");
	printf("nbytes=%d\n",nbytes);
	}

while(nleft>0)
        {
        if(nleft>MAX_DIM_BUFFER_RETE)
                nwrite=write(fd,ptr,MAX_DIM_BUFFER_RETE);
        else
                nwrite=write(fd,ptr,nleft);
	if(nwrite<0)
		errore("connection width scada");
        if(nwrite<=0)
                {
                pthread_mutex_unlock(fd_mutex);
                return(nwrite);
                }
        nleft-=nwrite;
        ptr+=nwrite;
        }
if(pthread_mutex_unlock(fd_mutex)<0)
	{
	printf("ERRORE:pthread_mutex_unlock fallita\n");
	printf("nbytes=%d\n",nbytes);
	}
return(nbytes-nleft);
}

int read_messaggio_scada(int fp,pthread_mutex_t *fp_mutex, char* buf)
{
short size;
S_HEAHOST hea;

        if(readn_i(fp,fp_mutex,&buf[0],sizeof(short))!=sizeof(short))
                return(-1);
        memcpy(&size,&buf[0],sizeof(short));
        size -= sizeof(short);
        if(size<0) return(-1);
        if(readn_i(fp,fp_mutex,&buf[2],size)!=(size))
                return(-1);
        memcpy(&hea,&buf[0],sizeof(S_HEAHOST));

//        printf("Letto messaggio %d size %d\n",hea.indice,size+sizeof(short));
        return((int)(hea.indice));
}

int invio_richiesta_nuova_pagina(int fp,pthread_mutex_t *fp_mutex,
                char *nome_pagina,int zona,int video)
{
S_PAV m_s_pav;
int i;

/*
 printf("CLIENT_SCADA: Invio richiesta aggiornamento pagina <%s>  zona %d num %d - %d\n",
	nome_pagina,zona,zone[zona-1].num_punti,zone[zona-1].t_agg);
*/

        /* size del pacchetto */
        m_s_pav.hea.nodo=sizeof(S_PAV);
        /* tipo messaggio */
        m_s_pav.hea.indice=mpav;
        /* classe messaggio */
        m_s_pav.hea.classe=0;
        /* zona video */
        m_s_pav.zona=zona;
        /* video */
        m_s_pav.video = video;
        /* nome */
/*
 printf("CLIENT_SCADA: l_pagina=%d - nome_pagina=%s \n", l_pagina, nome_pagina);
        nome_pagina[l_pagina-1]=0;
 printf("CLIENT_SCADA: nome_pagina=%s\n", nome_pagina);
*/
        sprintf(m_s_pav.nome,"%s",nome_pagina);
	m_s_pav.nome[l_pagina-1]=0;
        /* tipo input */
        m_s_pav.tin=inp_db;
        /* numero punti */
        m_s_pav.num_punti=zone[zona-1].num_punti;
        /* tipo di pagina */
        m_s_pav.tipo = 0;
        /* t aggiornamento */
        m_s_pav.per_sca = zone[zona-1].t_agg;
        /* formato */
        m_s_pav.formato = 0;
        /* puntatori */
        for(i=0;i<zone[zona-1].num_punti;i++)
                {
/*
		printf("puntdb = %d tipo = %d\n",zone[zona-1].puntdb[i],
				zone[zona-1].tipo[i]);
*/
                m_s_pav.point[i].tipo=zone[zona-1].tipo[i];
                m_s_pav.point[i].ext=0;
                m_s_pav.point[i].indice=zone[zona-1].puntdb[i];
                }
        if(writen_i(fp,fp_mutex,&m_s_pav,sizeof(S_PAV))!=sizeof(S_PAV))
                return(-1);
        else
                return(1);

                                                  
}

int invio_richiesta_allarmi(int fp,pthread_mutex_t *fp_mutex,int video,
                char* pagina,int db)
{
S_SEND_ALLARMI send;
        printf("Invio richiesta pagina allarmi  video %d pag %s db %d\n",
                video,pagina,db);

        send.hea.nodo=sizeof(S_SEND_ALLARMI);
        send.hea.indice=msen;
        send.hea.classe=0;
        send.zona=2;
        send.video=video;
        strcpy(send.pag,pagina);
        send.pag[8]=0;
        send.pag[9]=0;
        send.spare1=0;
#if defined OSF1_40 || defined LINUX
        send.spare2=0;
#endif
        send.tipo_arg_1=8;
        memcpy(send.stringa,pagina,8);
        send.tipo_arg_2= -2;
        send.ind_db=db;
        send.tipo_arg_3= -2;
        send.criterio=0;
        send.tipo_arg_3= -2;
        send.tipo_comando=0;
        send.tappo = -1;

        if(writen_i(fp,fp_mutex,&send,sizeof(S_SEND_ALLARMI))!=
                        sizeof(S_SEND_ALLARMI))
                return(-1);
        else
                return(1);
}

int invio_richiesta_canc_pagina(int fp,pthread_mutex_t *fp_mutex,
                int zona,int video)
{
S_VCLRV m_vclrv;

        printf("Invio richiesta cancellazione pagina  zona %d video %d\n",
                        zona,video);
        m_vclrv.hea.nodo=sizeof(S_VCLRV);
        m_vclrv.hea.indice=vclrv;
        m_vclrv.hea.classe=0;
        m_vclrv.zona=zona;
        m_vclrv.video=video;
        if(writen_i(fp,fp_mutex,&m_vclrv,sizeof(S_VCLRV))!=
                        sizeof(S_VCLRV))
                return(-1);
        else
                return(1);
}


int ins_dati_shm(int zona,char* dati)
{
float uno=1;
int i;
char *punt, *dest, *punt_flag;
FLAG_DIGITALE flag;
float valore;

punt=dati;

if(zone[zona-1].ind == NULL)
	return(1);

if(zona==ZONA_TEMPO)
	{
	dest = zone[zona-1].ind;
	memcpy(dest,punt,sizeof(float));
	memcpy(&valore,punt,sizeof(float));
	punt=zone[zona-1].ind+zone[zona-1].size-sizeof(float);
	memcpy(punt,&uno,sizeof(float));
	return(1);
	}


for(i=0;i<zone[zona-1].num_punti;i++)
  {
  switch(zone[zona-1].tipo[i])
	{
	case P_AA:
	case P_AS:
	case P_AC:
	case P_AD:
	case P_AO:
		{
#if defined VMS
		utov_float(punt,punt);
#endif
		dest = zone[zona-1].ind + zone[zona-1].pos[i]*2*sizeof(float);
		memcpy(dest,punt,sizeof(float));
		memcpy(&valore,punt,sizeof(float));
		dest = zone[zona-1].ind + zone[zona-1].pos[i]*2*sizeof(float)
			+sizeof(float);
		punt_flag = punt + SIZE_ANA - SIZE_FLAG;
		memcpy(dest,punt_flag,SIZE_FLAG);
		punt += SIZE_ANA;
		break;
		}
	case P_DA:
	case P_DS:
	case P_DC:
	case P_DD:
	case P_DO:
		{
		dest = zone[zona-1].ind + zone[zona-1].pos[i]*2*sizeof(float)
			+sizeof(float);
		memcpy(dest,punt,SIZE_FLAG);
		memcpy(&flag,punt,SIZE_FLAG);
		valore = (float)flag.sl;
		dest = zone[zona-1].ind + zone[zona-1].pos[i]*2*sizeof(float);
		memcpy(dest,&valore,sizeof(float));

/*************************************************************
memcpy(&flag,punt,SIZE_FLAG);
printf("flag dig a0=%d a1=%d a2=%d a3=%d a4=%d a5=%d a6=%d a7=%d\n",
        flag.a0,flag.a1,flag.a2,flag.a3,flag.a4,flag.a5,
                flag.a6,flag.a7);
printf("flag dig fs=%d fa=%d fz=%d st=%d in=%d au=%d ri=%d sl=%d\n",
        flag.fs,flag.fa,flag.fz,flag.st,flag.in,flag.au,
                flag.ri,flag.sl);
**************************************/
		punt += SIZE_DIG;
		break;
		}
	case P_OR:
		{
                printf("Punto organo: non ancora previsto\n");
                return(-1);
                break;
                }
	case P_ST:
		{
                printf("Punto stringa: non ancora previsto\n");
                return(-1);
                break;
                }
	default:
		{
                printf("Punto sconosciuto\n");
                return(-1);
                break;
                }
	}
  }

punt=zone[zona-1].ind+zone[zona-1].size-sizeof(float);
memcpy(punt,&uno,sizeof(float));

return(1);
}
                          
float conv_t_sim(short giorno,short ore,short minuti,short secondi)
{
static short giorno_save = -1;
static int count=0;
float t;

if(giorno_save == -1)
	giorno_save = giorno;

if(giorno_save != giorno)
	{
	giorno_save = giorno;
	count +=1 ;
	printf("INCREMENTO COUNT %d\n",count);
	}

t = (float)ore * 3600.0 + (float)minuti * 60.0 + (float)secondi + (float)count * 86400.0;
/*
printf("giorno = %d ore=%d minuti=%d secondi=%d tempo=%f\n",
	giorno,ore,minuti,secondi,t);
*/
return(t);
}                                                 

int invio_richiesta_dati_trend(int fp,pthread_mutex_t *fp_mutex,
                int zona,int video,char * pagina, int archivio,
                char * sigla_1,char * sigla_2,char * sigla_3,char * sigla_4,
                int num_sigle,char *out,pthread_mutex_t *canale_mutex)
{
S_MAGGR trend;
int i,ret;
char buf[MAX_DIM_MESSAGGIO_SCADA];
S_VPRGR m_vprgr;
S_VDSGR m_vdsgr;
S_HEAHOST ack;
float t_iniz;
char titolo[80];
/*
      per registrazione trend su file tipo f22
*/
char nome_var[4][MAX_LUN_NOME_VAR];
char descr_var[4][MAX_LUN_DESCR_VAR];
char buffer_nome[MAX_LUN_NOME_VAR*4];
char buffer_descr[MAX_LUN_DESCR_VAR*4];
float buffer_valori[4];

        if(num_sigle == 0)
                return(-1);

        if(num_sigle == 1)
                {
                if(sigla_1==NULL)
                        return(-1);
                if(strlen(sigla_1)==0)
                        return(-1);
                }

        if(num_sigle == 2)
                {
                if((sigla_1==NULL)||(sigla_2==NULL))
                        return(-1);
                if((strlen(sigla_1)==0)||(strlen(sigla_2)==0))
                        return(-1);
                }

        if(num_sigle == 3)
                {
                if((sigla_1==NULL)||(sigla_2==NULL)||
                        (sigla_3==NULL))
                        return(-1);
                }

        if(num_sigle == 4)

                {
                if((sigla_1==NULL)||(sigla_2==NULL)||
                        (sigla_3==NULL)||(sigla_4==NULL))
                        return(-1);
                }


printf("Richiedo dati trend zona = %d video = %d pagina =%s archivio = %d num_sigle = %d\n",
        zona,video,pagina,archivio,num_sigle);
printf("sigla_1=%s sigla_2=%s sigla_3=%s sigla_4=%s\n",
                sigla_1,sigla_2,sigla_3,sigla_4);


        trend.hea.nodo=sizeof(S_MAGGR);
        trend.hea.indice=maggr_new;
        trend.hea.zona=zona;
        trend.hea.video=video;
        strcpy(trend.hea.pagina,pagina);
                                              
        trend.hea.pagina[9]=0;
        trend.hea.indqua=1;
        trend.hea.n_mis=num_sigle;
        trend.archivio=archivio;
        /* settaggio delle sigle */
        memset(trend.sigla,0,max_sigqua*l_sigqua);
        strcpy(&(trend.sigla[0]),sigla_1);
        strcpy(&(trend.sigla[l_sigqua]),sigla_2);
        strcpy(&(trend.sigla[l_sigqua*2]),sigla_3);
        strcpy(&(trend.sigla[l_sigqua*3]),sigla_4);
        if(writen_i(fp,fp_mutex,&trend,sizeof(S_MAGGR))!=
                        sizeof(S_MAGGR))
                return(-1);
        /*  attende i dati */
        printf("Attendo i dati dei trend\n");
        while(1)
        {
        ret=read_messaggio_scada(fp,fp_mutex,buf);

        if(ret == vprgr)
                {
                memcpy(&m_vprgr,buf,sizeof(S_VPRGR));
#if defined VMS
		utov_float(&(m_vprgr.t_iniz),&(m_vprgr.t_iniz));
		utov_float(&(m_vprgr.durata),&(m_vprgr.durata));
		utov_float(&(m_vprgr.interv),&(m_vprgr.interv));
#endif	
                printf("S_VPRGR: t_iniz = %f durate = %f interv = %f\n",
                        m_vprgr.t_iniz,m_vprgr.durata,m_vprgr.interv);
                printf("giorno = %d mese = %d anno = %d n_mis= %c\n",
                        m_vprgr.giorno,m_vprgr.mese,m_vprgr.anno,m_vprgr.hea.n_mis);
		sprintf(titolo,"[start: %d/%d/%d]",
			m_vprgr.giorno,m_vprgr.mese,m_vprgr.anno);
                t_iniz=m_vprgr.t_iniz;
                for(i=0;i<num_sigle;i++)
                        {
                        m_vprgr.md[i].descr[l_desqua-1]=0;
                        if(strncmp(m_vprgr.md[i].descr,"****",4)==0)
				{
				printf("Errore nel ritorno delle sigle\n");
                                return(-1);
				}
                        }
                /* apertura file e scrittura header */
                memset(buffer_nome,' ',MAX_LUN_NOME_VAR*4);
                memset(buffer_descr,' ',MAX_LUN_DESCR_VAR*4);
                strcpy(nome_var[0],sigla_1);
                strcpy(nome_var[1],sigla_2);
                strcpy(nome_var[2],sigla_3);
                strcpy(nome_var[3],sigla_4);
                fill_space(nome_var[0],MAX_LUN_NOME_VAR-1);
                fill_space(nome_var[1],MAX_LUN_NOME_VAR-1);
                fill_space(nome_var[2],MAX_LUN_NOME_VAR-1);
                fill_space(nome_var[3],MAX_LUN_NOME_VAR-1);

                strcpy(descr_var[0],m_vprgr.md[0].descr);
                strcpy(descr_var[1],m_vprgr.md[1].descr);
                strcpy(descr_var[2],m_vprgr.md[2].descr);
                strcpy(descr_var[3],m_vprgr.md[3].descr);
                fill_space(descr_var[0],MAX_LUN_DESCR_VAR-1);
                fill_space(descr_var[1],MAX_LUN_DESCR_VAR-1);
                fill_space(descr_var[2],MAX_LUN_DESCR_VAR-1);
                fill_space(descr_var[3],MAX_LUN_DESCR_VAR-1);

                i=0;
                memcpy(&buffer_nome[i],&nome_var[0][0],MAX_LUN_NOME_VAR-1);
                i=0;
                memcpy(&buffer_descr[i],&descr_var[0][0],MAX_LUN_DESCR_VAR-1);

                i=MAX_LUN_NOME_VAR-1;
                memcpy(&buffer_nome[i],&nome_var[1][0],MAX_LUN_NOME_VAR-1);
                i=MAX_LUN_DESCR_VAR-1;
                memcpy(&buffer_descr[i],&descr_var[1][0],MAX_LUN_DESCR_VAR-1);

                i=(MAX_LUN_NOME_VAR-1)*2;
                memcpy(&buffer_nome[i],&nome_var[2][0],MAX_LUN_NOME_VAR-1);
                i=(MAX_LUN_DESCR_VAR-1)*2;
                memcpy(&buffer_descr[i],&descr_var[2][0],MAX_LUN_DESCR_VAR-1);

                i=(MAX_LUN_NOME_VAR-1)*3;
                memcpy(&buffer_nome[i],&nome_var[3][0],MAX_LUN_NOME_VAR-1);
                i=(MAX_LUN_DESCR_VAR-1)*3;
                memcpy(&buffer_descr[i],&descr_var[3][0],MAX_LUN_DESCR_VAR-1);
/*
                f22_open(out,"titolo della simulazione",
                        &buffer_nome[0],&buffer_descr[0],num_sigle);
*/
                break;
                }
        }

        while(1)
        {
        ret=read_messaggio_scada(fp,fp_mutex,buf);
        if(ret == vdsgr)
                {
		ack.nodo=sizeof(S_HEAHOST);
		ack.indice=vdsgr_ack;
        	if(writen_i(fp,fp_mutex,&ack,sizeof(S_HEAHOST))!=
                        sizeof(S_HEAHOST))
                		return(-1);
                memcpy(&m_vdsgr,buf,sizeof(S_VDSGR));
                printf(
           "S_VDSGR: n_campioni = %d  flag = %d sizeof(QCAMP)=%d l_vdsgr=%d\n",
                        m_vdsgr.n_camp,m_vdsgr.flag,sizeof(QCAMP),l_vdsgr);

		/* sblocca il canale prima di scrivere i dati su disco */
		if((m_vdsgr.flag == HT_ENDTRASM)||(m_vdsgr.flag == 3))
			{
			modo_trend=1;
        		pthread_mutex_unlock(canale_mutex);
                	// if(f22_open(out,titolo,
                        //     &buffer_nome[0],&buffer_descr[0],num_sigle)<0)
			// 	{
			// 	printf("Errore ritorno f22_open\n");
			// 	return(-1);
			// 	}
                        f22_open(out,
                            &buffer_nome[0],&buffer_descr[0],num_sigle);                     
			}
                if(estrai_campioni(&buf[l_cra_vdsgr],num_sigle,
                        (int)m_vdsgr.n_camp,t_iniz,(int)(m_vdsgr.flag))<0)
				{
				printf("Errore ritorno estrai_campioni\n");
				return(-1);
				}
                if((m_vdsgr.flag == HT_ENDTRASM)||(m_vdsgr.flag == 3))
                        {
                        // if(f22_close()<0)
			// 	{
			// 	printf("Errore ritorno f22_close\n");
			// 	return(-1);
			// 	}
                        f22_close();
printf("trend: blocco canale mutex\n");
        		pthread_mutex_lock(canale_mutex);
			modo_trend=0;
                        printf("Fine ricevimento dati trend\n");
                        return(1);
                        }
                }
        }

}

static int estrai_campioni(char   *b,int num,int num_c,float t_iniz,int flag)
{
char *app, *app_flag;
float t,tempo;
int i,j;
float dati[max_sigqua];
short tipo;
static BUFFER_TREND *buffer_trend;
static int tot_campioni;

typedef struct flag_digitale_st{
                unsigned short sl : 1;   /* stato logico corrente */
                unsigned short ri : 1;
                unsigned short au : 1;
                unsigned short in : 1;
                unsigned short st : 1;
                unsigned short fz : 1;   /* forzato */
                unsigned short fa : 1;   /* fuori attendibilita' */
                unsigned short fs : 1;   /* fuori scansione */
                unsigned short trattamento : 4;
                unsigned short spare : 2;
                unsigned short ls : 1;
                unsigned short as : 1;
                } FLAG_DIGITALE;
FLAG_DIGITALE flag_digitale;

	if((flag == 2)||(flag==3))
		{
		buffer_trend=malloc(num_c*sizeof(BUFFER_TREND));
		if(buffer_trend == NULL)
			exit(fprintf(stderr,"Errore allocazione buffer trend\n"));
		tot_campioni=num_c;
		}

	if((flag == 0)||(flag == 1))
		{
		tot_campioni += num_c;
		buffer_trend=realloc(buffer_trend,
				tot_campioni*sizeof(BUFFER_TREND));
		if(buffer_trend == NULL)
			exit(fprintf(stderr,"Errore allocazione buffer trend\n"));
		}

        app=b;
        for(i=0;i<num_c;i++)
                {
#if defined VMS
		utov_float(app,app);
#endif	
                memcpy(&t,app,sizeof(float));
                app += sizeof(float); /*+l_cra_qcamp*num);*/
                memset(dati,0,sizeof(float)*max_sigqua);
                for(j=0;j<num;j++)
                  {
                  memcpy(&tipo,app,sizeof(short));
                  if(tipo == -2)
                        { /* punto digitale */
                        app_flag = app+sizeof(short)+1;
                        memcpy(&flag_digitale,app_flag,sizeof(FLAG_DIGITALE));
                        dati[j]=flag_digitale.sl;
                        }
                  else
                        { /* punto analogico */
#if defined VMS
			utov_float(app,app);
#endif	
                        memcpy(&dati[j],app,sizeof(float));

                        }
                  app += l_cra_qcamp;
                  }


           buffer_trend[tot_campioni-num_c+i].tempo=t+t_iniz; 
	   memcpy(&(buffer_trend[tot_campioni-num_c+i].dati[0]),&(dati[0]),
		4*sizeof(float));

/*         prova
                f22_write(t+t_iniz,dati,num);
*/
                                       
/*****************************
                printf("Campione[%d] t = %f - %f %f %f %f\n",i,t,
                        dati[0],dati[1],dati[2],dati[3]);
*****************************/
                }

	if((flag == HT_ENDTRASM) || (flag == 3) )
		{
		printf("Totale campioni ricevuti = %d\n",tot_campioni);
		for(i=0;i<tot_campioni;i++)
		 {

		// if(f22_write(buffer_trend[i].tempo,buffer_trend[i].dati,num)<0)
		// 	{
		// 	printf("Errore ritorno f22_write\n");
		// 	return(-1);
		// 	}
                f22_write(buffer_trend[i].tempo,buffer_trend[i].dati,num);
		if(i==tot_campioni-1)
                printf("Ultimo campione[%d] t = %f - %f %f %f %f\n",i,buffer_trend[i].tempo,
                        buffer_trend[i].dati[0],
			buffer_trend[i].dati[1],
			buffer_trend[i].dati[2],
			buffer_trend[i].dati[3]);
		 }
		tot_campioni = 0;
		free(buffer_trend);
		}
return(1);
}

void ins_parziale_dati_allarmi(S_VDEAL allarmi)
{
int i,inizio;
DATI_ALLARMI_SHM dati_all;
char *app_char;
int size_riga_all=17+all_data+all_desc+6;  /* prova */
char col_str,col_ast;
int num_allarmi,kk;

   memcpy(&dati_all,punt_shm_allarmi,sizeof(DATI_ALLARMI_SHM));
   dati_all.num_all_arc=allarmi.n_parch;
   dati_all.num_all_da_ric=allarmi.n_pall;
/*
Inserisco anche il numero di allarmi per zone di impianto 
*/
   memcpy(dati_all.n_all_arc,allarmi.zone_all.n_all_arc,sizeof(short) * max_zone);
   memcpy(dati_all.n_all_pen,allarmi.zone_all.n_all_pen,sizeof(short) * max_zone);
/*
        printf("Situazione all x zone nella SHM\n");
                for(kk=0;kk<4;kk++)
                        {
                        printf("ZONE[%d] allpen=%d allarc=%d\n",kk,dati_all.n_all_pen[kk],dati_all.n_all_arc[kk]);
                        }
*/

   printf("ins_parziale_dati_allarmi: dati_validi = %d\n",dati_all.dati_validi);
   memcpy(punt_shm_allarmi,&dati_all,sizeof(DATI_ALLARMI_SHM));
}




int ins_dati_miniASD(S_VDEAL *allarmiPunt,int id_shm_miniASD)
{
char *punt_shm_miniASD;
int shmvid;
int i,inizio,j;
DATI_ALLARMI_SHM dati_all;
char *app_char; 
int size_riga_all=17+all_data+all_desc+n_gerarchie+dim_emission;
short appo_short;
char appo_char;
int num_allarmi;
/*S_VDEAL allarmi;*/    /* divenuta globale per problemi di stack */
char asterisco;
char gerarchia[n_gerarchie];
int kk; /* prova */
int k;

printf("CLIENT_SCADA: sono entrato in ins_dati_miniASD\n");
printf("CLIENT_SCADA: id_shm_miniASD=%ld\n",id_shm_miniASD);
        if(!id_shm_miniASD)
             {
             fprintf(stderr,"WARNING shm not created\n");
             }
	if(operazione_allarmi==ALL_REQ_MINIASD)
              printf("client_scada: operazione ALL_REQ_MINIASD finita \n");
        else
              printf("client_scada: operazione non consentita\n");
	inizio=0;
	memcpy(&allarmi_miniASD,allarmiPunt,sizeof(S_VDEAL));
	memset(&dati_all,0,sizeof(DATI_ALLARMI_SHM));
	inizio=0;

	num_allarmi=allarmi_miniASD.n_all;
/*
Parte comune a tutte le operazioni:
N.B.
Gli offset sono differenti da quelli del messaggio Scada
(Compattamento dei byte )
*/
	
	dati_all.n_all=allarmi_miniASD.n_all;

        printf("client_scada:insASD: num_all=%d\n",dati_all.n_all);
        for(i=0;i<dati_all.n_all;i++)
		{
#if defined OSF1_40 || LINUX
                /* Codice valido per versioni di OSF1 precedenti la 4.0 */

		app_char=(char*)&(allarmi_miniASD.stringall[0])+i*size_riga_all;
                /* Vado a cercare campo indice di S_PDBS nel msg */
		app_char+= 8;

		memcpy(&appo_short,app_char,sizeof(short));
		dati_all.indice[i+inizio]=appo_short;
                printf("client_scada punt allarme[%d]=%d\n",i,
                        dati_all.indice[i+inizio]);
                /*
		memcpy(&asterisco,&app_char[1],1);
                */
		app_char=(char*)&(allarmi_miniASD.stringall[0])+i*size_riga_all;

                /* Vado a cercare campo ext di S_PDBS nel msg */
		app_char+= 7;

		memcpy(&appo_char,app_char,1);
		dati_all.ext[i+inizio]=appo_char;
                printf("client_scada ext allarme[%d]=%d\n",i,
                        dati_all.ext[i+inizio]);
		app_char=(char*)&(allarmi_miniASD.stringall[0])+i*size_riga_all;

                /* Vado a cercare campo ertr di S_PDBS nel msg */
		app_char+= 6;

		memcpy(&appo_char,app_char,1);
		dati_all.tipo[i+inizio]=appo_char;
                printf("client_scada ertr allarme[%d]=%d\n",i,
                        dati_all.tipo[i+inizio]);

                /* Data allarme */
		app_char=(char*)&(allarmi_miniASD.stringall[0])+i*size_riga_all;

		app_char +=15;
		memcpy(dati_all.data[i+inizio],app_char,all_data);
		app_char+=all_data;
		memcpy(dati_all.descr[i+inizio],app_char,all_desc);
		app_char+=all_desc;

		memcpy(dati_all.hierarchy[i+inizio],app_char,n_gerarchie);
/*
Acquisisco lo stato di ogni allarme emissione o rientro
*/
		app_char+=n_gerarchie;
		dati_all.emission[i+inizio]=*app_char;
/*		printf("emission=%d\n",dati_all.emission[i+inizio]);*/
#else 
                /* Codice valido per OSF1 ver. 4.0 */

		dati_all.indice[i+inizio]=allarmi_miniASD.stringall[i].p.indice;

                printf("client_scada punt allarme[%d]=%d\n",i,
                        dati_all.indice[i+inizio]);

		dati_all.ext[i+inizio]=allarmi_miniASD.stringall[i].p.ext;

                printf("client_scada ext allarme[%d]=%d\n",i,
                        dati_all.ext[i+inizio]);

		dati_all.tipo[i+inizio]=allarmi_miniASD.stringall[i].p.tipo;

		memcpy(dati_all.data[i+inizio],allarmi_miniASD.stringall[i].data,all_data);
		memcpy(dati_all.descr[i+inizio],allarmi_miniASD.stringall[i].desc,all_desc);
		memcpy(dati_all.hierarchy[i+inizio],allarmi_miniASD.stringall[i].hierarchy,n_gerarchie);
		dati_all.emission[i+inizio]=allarmi_miniASD.stringall[i].emission;
#endif

/*
Inserisco terminatore di stringa alla fine di ogni descrizione per evitare
sporcamento quando tratto le stringhe del campo descr delle variabili di
tipo DATI_ALLARMI_SHM
by Fabio 12/7/96
N.B.
Dal lato SCADA le stringhe sono trattate tutte senza \0
*/
//		dati_all.descr[i+inizio][ALL_DESC-1]=NULL;
                dati_all.descr[i+inizio][ALL_DESC-1] = '\0';

printf("allarmi[%d]=>   |%d/%d| |%s|\n",i+inizio,
			dati_all.col_ast[i+inizio],dati_all.col_str[i+inizio],
			dati_all.descr[i+inizio]);
/*
Ultima modifica di Marcello del Marzo 96
*/
printf("allarmi[%d]=>    |desc=%s|data=%s| em=%d\n",i+inizio,
                        dati_all.descr[i+inizio], dati_all.data[i+inizio],
                        dati_all.emission[i+inizio]);

		}/*end for*/
/*
Inserisco situazione allarmi per zone di impianto
*/
/******
		printf("Situazione all x zone\n");
		for(kk=0;kk<4;kk++)
			{
			printf("ZONE[%d] allpen=%d allarc=%d\n",kk,allarmi_miniASD.zone_all.n_all_pen[kk],allarmi_miniASD.zone_all.n_all_arc[kk]);
			}
*****/
	memcpy(dati_all.n_all_arc,allarmi_miniASD.zone_all.n_all_arc,sizeof(short) * max_zone);
	memcpy(dati_all.n_all_pen,allarmi_miniASD.zone_all.n_all_pen,sizeof(short) * max_zone);
/*****
	printf("Situazione all x zone nella SHM\n");
                for(kk=0;kk<4;kk++)
                        {
                        printf("ZONE[%d] allpen=%d allarc=%d\n",kk,dati_all.n_all_pen[kk],dati_all.n_all_arc[kk]);
                        }	
*****/
	dati_all.num_all_arc=allarmi_miniASD.n_parch;
	dati_all.num_all_da_ric=allarmi_miniASD.n_pall;
/*
printf("CLIENT_SCADA ---> num_all_da_ric = %d\n",dati_all.num_all_da_ric);
*/
	if(dati_all.num_all_da_ric<0)
		dati_all.num_all_da_ric=0;
/*
for(i=0;i<dati_all.n_all;i++)
	printf("allarmi2[%d]=>   Descriz=%s--Data=%s\n",i,dati_all.descr[i],dati_all.data[i]);
*/

/*        for(i=0;i<dati_all.n_all;i++)
                   printf("all[%d] - |%d/%d|-|%s|-|%s|\n",
			i,dati_all.col_ast[i],dati_all.col_str[i],
			dati_all.descr[i],dati_all.data[i]);*/
   	dati_all.dati_validi=DATI_ALLARMI_VALIDI;
printf("client_scada: dati_validi = %d\n",dati_all.dati_validi);
/*
Copio i dati nella shm della OW
*/
punt_shm_miniASD=(char*)crea_shrmem(id_shm_miniASD,sizeof(DATI_ALLARMI_SHM),
        &shmvid);
if(punt_shm_miniASD==NULL)
   {
   fprintf(stderr,"client_scada:WARNING:Access to shm with id=%d impossible\n",
          id_shm_miniASD);
   }
else
   {
   printf("client_scada: punt_shm_miniASD = %ld\n",punt_shm_miniASD);
   memcpy(punt_shm_miniASD,&dati_all,sizeof(DATI_ALLARMI_SHM)); 
   }
if(sgancia_shrmem(punt_shm_miniASD)<0)
   {
   printf("client_scada:WARNING:Impossibile sgancia_shrmem on miniASD\n");
   }
}





void ins_dati_allarmi(S_VDEAL *allarmiPunt)
{
int i,inizio,j;
DATI_ALLARMI_SHM dati_all;
char *app_char; 
int size_riga_all=17+all_data+all_desc+n_gerarchie+dim_emission;
char col_str,col_ast;
int num_allarmi;
/*S_VDEAL allarmi;*/    /* divenuta globale per problemi di stack */
char asterisco;
char gerarchia[n_gerarchie];
int kk; /* prova */
int k;

	inizio=0;
	memcpy(&allarmi,allarmiPunt,sizeof(S_VDEAL));
	memset(&dati_all,0,sizeof(DATI_ALLARMI_SHM));
	if((operazione_allarmi==ALL_REQ_ALLARMI)||
		(operazione_allarmi==ALL_REQ_ARCHIVI)||
		(operazione_allarmi==ALL_REQ_RIC))
			inizio=0;
	num_allarmi=allarmi.n_all;
/*
Verifico se e' richiesta operazione AVANTI od INDIETRO 
Ricopio i dati degli allarmi che rimaranno visualizzati nelle giuste posizioni
*/

	if(operazione_allarmi==ALL_REQ_AVANTI)
	    {
	    printf("ALL_REQ_AVANTI-num_allarmi = %d\n",num_allarmi);
            memcpy(&dati_all,punt_shm_allarmi,sizeof(DATI_ALLARMI_SHM));
	    inizio=16-num_allarmi;
	    for(i=0;i<16-num_allarmi;i++)
		{
		memcpy(dati_all.data[i],
			dati_all.data[i+num_allarmi],all_data);
		memcpy(dati_all.descr[i],
			dati_all.descr[i+num_allarmi],all_desc);
		memcpy(dati_all.hierarchy[i],
			dati_all.hierarchy[i+num_allarmi],n_gerarchie);
		dati_all.emission[i]=dati_all.emission[i+num_allarmi];
		dati_all.col_ast[i]=dati_all.col_ast[i+num_allarmi];
		dati_all.col_str[i]=dati_all.col_str[i+num_allarmi];
		}
	     for(i=16-num_allarmi;i<16;i++)
		{
		memset(dati_all.data[i],0,all_data);
		memset(dati_all.descr[i],0,all_desc);
		dati_all.col_ast[i]=0;
		dati_all.col_str[i]=0;
		} 
	    allarmi_a=dati_all.allarmi_a;
	    allarmi_da=dati_all.allarmi_da;
	    allarmi_a +=allarmi.n_all;
	    allarmi_da +=allarmi.n_all;
	    }/* end if ALL_REQ_AVANTI */

	if(operazione_allarmi==ALL_REQ_INDIETRO)
	    {
            memcpy(&dati_all,punt_shm_allarmi,sizeof(DATI_ALLARMI_SHM));
	    inizio=0;
	    for(i=15;i>=num_allarmi;i--)
		{
		memcpy(dati_all.data[i],
			dati_all.data[i-num_allarmi],all_data);
		memcpy(dati_all.descr[i],
			dati_all.descr[i-num_allarmi],all_desc);
		memcpy(dati_all.hierarchy[i],
			dati_all.hierarchy[i-num_allarmi],n_gerarchie);
		dati_all.emission[i]=dati_all.emission[i-num_allarmi];
		dati_all.col_ast[i]=dati_all.col_ast[i-num_allarmi];
		dati_all.col_str[i]=dati_all.col_str[i-num_allarmi];
		}
	    for(i=0;i<num_allarmi;i++)
		{
		memset(dati_all.data[i],0,all_data);
		memset(dati_all.descr[i],0,all_desc);
		dati_all.col_ast[i]=0;
		dati_all.col_str[i]=0;
		}
	    allarmi_a=dati_all.allarmi_a;
	    allarmi_da=dati_all.allarmi_da;
	    allarmi_a -=allarmi.n_all;
	    allarmi_da -=allarmi.n_all;
	    } /*end if ALL_REQ_INDIETRO */
/*
Parte comune a tutte le operazioni:
N.B.
Gli offset sono differenti da quelli del messaggio Scada
(Compattamento dei byte )
*/
	
	dati_all.n_all=allarmi.n_all;

        for(i=0;i<dati_all.n_all;i++)
		{
#if !defined OSF1_40 &&  !defined LINUX
                /* Codice valido per OSF1 ver. precedente la 4.0 */

		app_char=(char*)&(allarmi.stringall[0])+i*size_riga_all;
		app_char+= 14;
		memcpy(&col_str,app_char,1);
		dati_all.col_str[i+inizio]=col_str;
		memcpy(&asterisco,&app_char[1],1);
		app_char=(char*)&(allarmi.stringall[0])+i*size_riga_all;
		app_char+= 10;
		memcpy(&col_ast,app_char,1);
		dati_all.col_ast[i+inizio]=col_ast;
		app_char=(char*)&(allarmi.stringall[0])+i*size_riga_all;
		app_char +=15;
		memcpy(dati_all.data[i+inizio],app_char,all_data);
		app_char+=all_data;
		memcpy(dati_all.descr[i+inizio],app_char,all_desc);
		app_char+=all_desc;
/*
Inizio modifiche by Fabio
*/
		memcpy(dati_all.hierarchy[i+inizio],app_char,n_gerarchie);
/*
		printf("client_scada:allarm hierarchy no %d inizio=%d\n",i,inizio);
                for(kk=0;kk<6;kk++)
                        {
                        printf("%d",dati_all.hierarchy[i+inizio][kk]);
                        }
                printf("\n");
*/
/*
Acquisisco lo stato di ogni allarme emissione o rientro
*/
		app_char+=n_gerarchie;
		dati_all.emission[i+inizio]=*app_char;
/*		printf("emission=%d\n",dati_all.emission[i+inizio]);*/
#else
                /* Codice valido per OSF1 ver. 4.0  */

                dati_all.col_str[i+inizio]=allarmi.stringall[i].colorstr;
                dati_all.col_ast[i+inizio]=allarmi.stringall[i].colorast;
		memcpy(dati_all.data[i+inizio],allarmi.stringall[i].data,all_data);
		memcpy(dati_all.descr[i+inizio],allarmi.stringall[i].desc,all_desc);
		memcpy(dati_all.hierarchy[i+inizio],allarmi.stringall[i].hierarchy,n_gerarchie);
                
/*
Acquisisco lo stato di ogni allarme emissione o rientro
*/
		dati_all.emission[i+inizio]=allarmi.stringall[i].emission;
/*
                printf("emission=%d\n",dati_all.emission[i+inizio]);
                printf("INS_DATI_ALLARMI --> dati = %s\tdesc=%s\n",
                       allarmi.stringall[i].data, allarmi.stringall[i].desc);
*/
#endif

/*
Inserisco terminatore di stringa alla fine di ogni descrizione per evitare
sporcamento quando tratto le stringhe del campo descr delle variabili di
tipo DATI_ALLARMI_SHM
by Fabio 12/7/96
N.B.
Dal lato SCADA le stringhe sono trattate tutte senza \0
*/
//		dati_all.descr[i+inizio][ALL_DESC-1]=NULL;
		dati_all.descr[i+inizio][ALL_DESC-1]= '\0';

printf("allarmi[%d]=>   |%d/%d| |%s|\n",i+inizio,
			dati_all.col_ast[i+inizio],dati_all.col_str[i+inizio],
			dati_all.descr[i+inizio]);
/*
Ultima modifica di Marcello del Marzo 96
*/
printf("allarmi[%d]=>   |%d[%c]/%d| |%s|\n",i+inizio,
                        dati_all.col_ast[i+inizio],asterisco,dati_all.col_str[i+
inizio],
                        dati_all.descr[i+inizio]);

		}/*end for*/
/*
Inserisco situazione allarmi per zone di impianto
*/

/*
		printf("Situazione all x zone\n");
		for(kk=0;kk<4;kk++)
			{
			printf("ZONE[%d] allpen=%d allarc=%d\n",kk,allarmi.zone_all.n_all_pen[kk],allarmi.zone_all.n_all_arc[kk]);
			}
*/

	memcpy(dati_all.n_all_arc,allarmi.zone_all.n_all_arc,sizeof(short) * max_zone);
	memcpy(dati_all.n_all_pen,allarmi.zone_all.n_all_pen,sizeof(short) * max_zone);

/*
	printf("Situazione all x zone nella SHM\n");
                for(kk=0;kk<4;kk++)
                        {
                        printf("ZONE[%d] allpen=%d allarc=%d\n",kk,dati_all.n_all_pen[kk],dati_all.n_all_arc[kk]);
                        }	
*/

	dati_all.num_all_arc=allarmi.n_parch;
	dati_all.num_all_da_ric=allarmi.n_pall;
	if(dati_all.num_all_da_ric<0)
		dati_all.num_all_da_ric=0;

for(i=0;i<dati_all.n_all;i++)
	printf("allarmi2[%d]=>   Descriz=%s--Data=%s\n",i,dati_all.descr[i],dati_all.data[i]);

	if((operazione_allarmi==ALL_REQ_ALLARMI)||
		(operazione_allarmi==ALL_REQ_RIC) ||
		(operazione_allarmi==ALL_RIC_MINIASD))
		{
		if(allarmi.n_all>0)
			{
			if(allarmi.n_all<16)
				allarmi_a=allarmi.n_all;
			else
				allarmi_a=16;
			allarmi_da=1;
			}
		else
			{
			allarmi_a=0;
			allarmi_da=0;
			}
		}

	if(operazione_allarmi==ALL_REQ_ARCHIVI)
		{
		if(allarmi.n_parch>0)
			{
			if(allarmi.n_parch<16)
				{
				allarmi_a=allarmi.n_parch;
				allarmi_da=1;
				}
			else
				{
				allarmi_a=allarmi.n_parch;
				allarmi_da=allarmi.n_parch-15;
				}
			}
		else
			{
			allarmi_a=0;
			allarmi_da=0;
			}
		}

	dati_all.allarmi_a=allarmi_a;
	dati_all.allarmi_da=allarmi_da;

/*        for(i=0;i<dati_all.n_all;i++)
                   printf("all[%d] - |%d/%d|-|%s|-|%s|\n",
			i,dati_all.col_ast[i],dati_all.col_str[i],
			dati_all.descr[i],dati_all.data[i]);*/
printf("client_scada: dati_validi = %d\n",dati_all.dati_validi);
   	dati_all.dati_validi=DATI_ALLARMI_VALIDI;
        memcpy(punt_shm_allarmi,&dati_all,sizeof(DATI_ALLARMI_SHM));
	/*	printf("client_scada:FINAL hierarchy final %d \n",i);*/
}


int destroy_shm_allarmi_miniASD(int id_allarm,int id_manual)
{
int shmvid;
char *data_all;
if(id_allarm<0 || id_manual<0)
    return(-1);
printf("client_scada= id_allarm=%d id_manual=%d\n",id_allarm,id_manual);
/*
Aggancio la shm e la distruggo
*/
if(!(data_all=(char *)crea_shrmem(id_allarm,sizeof(DATI_ALLARMI_SHM),
               &shmvid)))
   {
   fprintf(stderr,"client_scada: impossible to get shm with id=%d\n",
           id_allarm);
   return(-1);
   }
sgancia_shrmem(data_all);

distruggi_shrmem(shmvid);


/*
Aggancio la shm e la distruggo per i manual
*/
if(!(data_all=(char *)crea_shrmem(id_manual,sizeof(DATI_ALLARMI_SHM),
               &shmvid)))
   {
   fprintf(stderr,"client_scada: impossible to get shm with id=%d\n",
           id_manual);
   return(-1);
   }
sgancia_shrmem(data_all);

distruggi_shrmem(shmvid);
return(0);
}

/*
Funzione che richiede gli allarmi per una certa gerarchia di O.W.
per miniASD
*/
int invio_richiesta_allarmi_miniASD(int fp,pthread_mutex_t *fp_mutex,int video,
                char* pagina,int db, unsigned char *gerarchia,int modo,
                int filtro,int tipo_allarme)
{
int i;
short tappo=-1;
short app_short;
S_SEND_ALLARMI_ZONE send;

// printf("CLIENT_SCADA:Invio richiesta allarmi miniASD  video %d pag %s db %d\n", video,pagina,db);
/*
Setto le dim del messaggio
*/

send.hea.nodo=sizeof(S_SEND_ALLARMI_ZONE)+sizeof(short)+
              sizeof(short)*2*N_GERARCHIE+sizeof(short)*2;
send.hea.indice=msen;
send.hea.classe=0;
send.zona=2;
send.video=video;
strcpy(send.pag,pagina);
send.pag[8]=0;
send.pag[9]=0;
send.spare1=0;
#if defined OSF1_40 || defined LINUX
send.spare2=0;
#endif
send.tipo_arg_1=8;
memcpy(send.stringa,pagina,8);
send.tipo_arg_2= -2;
send.ind_db=db;
send.tipo_arg_3= -2;
send.criterio=ALL_M;    /* tipo di allarme */
send.tipo_arg_3= -2;
send.tipo_comando=(short)modo;

/*
Termine dell' header del messaggio 
*/
if(writen_i(fp,fp_mutex,&send,sizeof(S_SEND_ALLARMI_ZONE))!=
                sizeof(S_SEND_ALLARMI_ZONE))
        return(-1);
for( i=0;i<N_GERARCHIE;i++)
	{
	tappo=-2;
       	if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
       		 return(-1);
	app_short=(short)gerarchia[i];
        fprintf(stderr,"client_scada: hierarchy requested level[%d]=%d\n",i,app_short);
       	if(writen_i(fp,fp_mutex,&app_short,sizeof(short))!=
			sizeof(short))
                 return(-1);
		
	}
tappo=-2;
if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
       	{
        printf("client_scada:write_i fallita \n");
        return(-1);
        }
/*
Spedisco il filtro con cui selezione gli allarmi da visualizzare
*/
printf("client_scada: filtro=%d tipo=%d\n",filtro,tipo_allarme);
app_short=(short)filtro;
printf("client_scada: Sto per inviare filtro=%d\n",filtro);
if(writen_i(fp,fp_mutex,&app_short,sizeof(short))!=
     sizeof(short))
       	 return(-1);
/*
Invio tappo finale
*/
tappo=-1;
printf("client_scada: Sto per inviare tappo=%d\n",tappo);
if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
       {
       printf("client_scada: errore nell' iviare tappo\n");
       return(-1);
       }
return(1);
}
/*
Funzione che spedisce a SCADA messaggio contenente le informazioni che 
distinguono quell' allarme da riconoscere.
data_all e' un puntatore alla shm in cui si trova l' allarme di indice
indice_all da riconoscere. 

Ritorno:
	-1 se incontro quqlche pb in trasmissione
*/
int invio_riconoscimento_allarmi_miniASD(int fp,pthread_mutex_t *fp_mutex,
                int video,char* pagina,int db,unsigned char *gerarchia,
                DATI_ALLARMI_SHM *data_all,int modo,
                int filtro,int tipo_all,int indice_all)
{

int i;
short tappo=-1;
short app_short;
S_SEND_ALLARMI_ZONE send;
char appo_data[ALL_DATA],*str;
short ora,minuti,secondi;
int shmvid;

// printf("CLIENT_SCADA:RICON Invio richiesta miniASD  video %d pag %s db %d\n", video,pagina,db);
/*
 Setto le dim del messaggio 7 sta per data( giorno ore ...) + ext ertr e 
 punt dell' allarme
*/

send.hea.nodo=sizeof(S_SEND_ALLARMI_ZONE)+sizeof(short)+
              sizeof(short)*2*N_GERARCHIE+(2*7*sizeof(short));
send.hea.indice=msen;
send.hea.classe=0;
send.zona=2;
send.video=video;
strcpy(send.pag,pagina);
send.pag[8]=0;
send.pag[9]=0;
send.spare1=0;
#if defined OSF1_40 || defined LINUX
send.spare2=0;
#endif
send.tipo_arg_1=8;
memcpy(send.stringa,pagina,8);
send.tipo_arg_2= -2;
send.ind_db=db;
send.tipo_arg_3= -2;
send.criterio=ALL_M;    /* tipo di allarme */
send.tipo_arg_3= -2;
send.tipo_comando=(short)modo;

/*
Controlli 
*/
// printf("CLIENT_SCADA: RICON data_all=%ld\n",data_all);
if(data_all==NULL)
    {
    fprintf(stderr,"client_scada:WARNING: Impossible to open shm \n");
    return(-1);
    }
if(data_all->n_all<indice_all)
   {
   fprintf(stderr,"ERROR: alarm index is greater than n_all in shm O.W\n");
   fprintf(stderr,"data_all->n_all=%d indice_all=%d\n",data_all->n_all,indice_all);
   fprintf(stderr,"data_all->dati_validi=%d \n",data_all->dati_validi);
   return(-1);
   }
if(data_all->emission[indice_all]==RICONOSCIUTO)
   {
   printf("client_scada:allarme gia' riconosciuto \n");
   next_ric=1;
   return(1);
   }
/*
Eleborazione data
*/
memcpy(appo_data,data_all->data[indice_all],all_data);
appo_data[all_data-1]=0;  /*terminatore di stringa */
// printf("CLIENT_SCADA: RICON appo_data=%s\n",appo_data);
str=strtok(appo_data,":");
if(!str)
    {
    printf("client_scada: impossibile trovare ora\n");
    return(-1);
    }
ora=(short)atoi(str);

// printf("CLIENT_SCADA: RICON ora=%d\n",ora);
str=strtok((char *)NULL,":");
if(!str)
    {
    printf("client_scada: impossibile trovare minuti\n");
    return(-1);
    }
minuti=(short)atoi(str);
printf("client_scada:minuti=%d\n",minuti);
str=strtok((char *)NULL,":");
if(!str)
    {
    printf("client_scada: impossibile trovare secondi\n");
    return(-1);
    }
secondi=(short)atoi(str);
printf("client_scada:secondi=%d\n",secondi);

/*
Termine dell' header del messaggio 
*/
if(writen_i(fp,fp_mutex,&send,sizeof(S_SEND_ALLARMI_ZONE))!=
                sizeof(S_SEND_ALLARMI_ZONE))
        return(-1);
/* 
Spedisco gerarchia allarme 
*/
for( i=0;i<N_GERARCHIE;i++)
	{
	tappo=-2;
       	if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
       		 return(-1);
	app_short=(short)gerarchia[i];
        fprintf(stderr,"client_scada: hierarchy requested level[%d]=%d\n",i,
               app_short);
       	if(writen_i(fp,fp_mutex,&app_short,sizeof(short))!=
			sizeof(short))
                 return(-1);
		
	}

tappo=-2;
if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
       	{
        printf("client_scada:write_i fallita \n");
        return(-1);
        }
/*
Spedisco il filtro con cui seleziono l' allarme da riconoscere
*/
printf("client_scada: filtro=%d tipo=%d\n",filtro,tipo_all);
app_short=(short)filtro;
printf("client_scada: Sto per inviare filtro=%d\n",filtro);
if(writen_i(fp,fp_mutex,&app_short,sizeof(short))!=sizeof(short))
       	 return(-1);
/*
Invio punt dell' allarme 
*/

tappo=-2;
if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
       	{
        printf("client_scada:write_i fallita \n");
        return(-1);
        }

app_short=(short)data_all->indice[indice_all];
printf("client_scada: Sto per inviare punt=%d\n",
       (short)data_all->indice[indice_all]);
if(writen_i(fp,fp_mutex,&app_short,sizeof(short))!=sizeof(short))
       	 return(-1);
/*
Invio ext allarme 
*/
tappo=-2;
if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
       	{
        printf("client_scada:write_i fallita \n");
        return(-1);
        }

app_short=(short)data_all->ext[indice_all];
printf("client_scada: Sto per inviare ext=%d\n",
       (short)data_all->ext[indice_all]);
if(writen_i(fp,fp_mutex,&app_short,sizeof(short))!=sizeof(short))
       	 return(-1);

/*
Invio ertr allarme 
*/
tappo=-2;
if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
       	{
        printf("client_scada:write_i fallita \n");
        return(-1);
        }

app_short=(short)data_all->tipo[indice_all];
printf("client_scada: Sto per inviare ext=%d\n",
       (short)data_all->tipo[indice_all]);
if(writen_i(fp,fp_mutex,&app_short,sizeof(short))!=sizeof(short))
         {
       	 return(-1);
         }
/*
Invio ora
*/
tappo=-2;
if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
       	{
        printf("client_scada:write_i fallita \n");
        return(-1);
        }

app_short=ora;
printf("client_scada: Sto per inviare ora=%d\n",ora);
if(writen_i(fp,fp_mutex,&app_short,sizeof(short))!=sizeof(short))
         {
       	 return(-1);
         }
/*
Invio minuti
*/
tappo=-2;
if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
       	{
        printf("client_scada:write_i fallita \n");
        return(-1);
        }

app_short=minuti;
printf("client_scada: Sto per inviare minuti=%d\n",minuti);
if(writen_i(fp,fp_mutex,&app_short,sizeof(short))!=sizeof(short))
         {
       	 return(-1);
         }
/*
Invio secondi
*/
tappo=-2;
if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
       	{
        printf("client_scada:write_i fallita \n");
        return(-1);
        }

app_short=secondi;
printf("client_scada: Sto per inviare secondi=%d\n",secondi);
if(writen_i(fp,fp_mutex,&app_short,sizeof(short))!=sizeof(short))
         {
       	 return(-1);
         }

/*
Invio tappo finale
*/
tappo=-1;
if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
       {
       printf("client_scada: errore nell' iviare tappo\n");
       return(-1);
       }
return(1);

}
int invio_richiesta_allarmi_zone(int fp,pthread_mutex_t *fp_mutex,int video,
                char* pagina,int db, int *zone_imp,int modo)
{
S_SEND_ALLARMI_ZONE send;
int i;
short tappo=-1;
int num_zone=0;
short app_short;

        printf("Invio richiesta pagina allarmi  video %d pag %s db %d\n",
                video,pagina,db);

	for(i=0;i<32;i++)
		if(zone_imp[i]==1)
			num_zone++;

        send.hea.nodo=sizeof(S_SEND_ALLARMI_ZONE)+sizeof(short)+
			sizeof(short)*2*num_zone;
printf("DEBUG: client_scada ---> nodo = %d\n",send.hea.nodo);
        send.hea.indice=msen;
        send.hea.classe=0;
        send.zona=2;
        send.video=video;
        strcpy(send.pag,pagina);
printf("DEBUG: client_scada ---> pagina = %s\n",send.pag);
        send.pag[8]=0;
        send.pag[9]=0;
        send.spare1=0;
#ifdef OSf1_40
        send.spare2=0;
#endif
        send.tipo_arg_1=8;
        memcpy(send.stringa,pagina,8);
        send.tipo_arg_2= -2;
printf("DEBUG: client_scada ---> stringa = %s\n",send.stringa);
        send.ind_db=db;
        send.tipo_arg_3= -2;
        send.criterio=1234;
        send.tipo_arg_3= -2;
        send.tipo_comando=(short)modo;
printf("DEBUG: client_scada ---> dim_S_SEND_ALLARMI_ZONE = %d\n",
       sizeof(S_SEND_ALLARMI_ZONE));
        if(writen_i(fp,fp_mutex,&send,sizeof(S_SEND_ALLARMI_ZONE))!=
                        sizeof(S_SEND_ALLARMI_ZONE))
                return(-1);
	for(i=0;i<32;i++)
	  if(zone_imp[i]==1)
		{
                fprintf(stderr,"client_scada: requested zone no=%d\n",i);
		tappo=-2;
        	if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
               		 return(-1);
		app_short=i;
// printf("CLIENT_SCADA ---> ZONA = %d\n",app_short);
        	if(writen_i(fp,fp_mutex,&app_short,sizeof(short))!=
				sizeof(short))
               		 return(-1);
		
		}
	tappo=-1;
        if(writen_i(fp,fp_mutex,&tappo,sizeof(short))!=sizeof(short))
                return(-1);
return(1);
}

int invio_comando_allarmi(int fp,pthread_mutex_t *fp_mutex,int video,
                char* pagina,int db, int comando)
{
S_SEND_ALLARMI send;
int i;

        printf("Invio comando pagina allarmi  video %d pag %s db %d [%d]\n",
                video,pagina,db,comando);

        send.hea.nodo=sizeof(S_SEND_ALLARMI);
        send.hea.indice=msen;
        send.hea.classe=0;
        send.zona=2;
        send.video=video;
        strcpy(send.pag,pagina);
        send.pag[8]=0;
        send.pag[9]=0;
        send.spare1=0;
#if defined OSF1_40 || defined LINUX
        send.spare2=0;
#endif
        send.tipo_arg_1=8;
        memcpy(send.stringa,pagina,8);
        send.tipo_arg_2= -2;
        send.ind_db=db;
        send.tipo_arg_3= -2;
        send.criterio=1234;
        send.tipo_arg_3= -2;
        send.tipo_comando=comando;
        send.tappo = -1;

        if(writen_i(fp,fp_mutex,&send,sizeof(S_SEND_ALLARMI))!=
                        sizeof(S_SEND_ALLARMI))
                return(-1);
        else
                return(1);
}

int ins_dati_shm_all(char* dati)
{
float uno=1;
int i;
char *punt;
FLAG_DIGITALE flag;
float valore;
char test_char[4];
DATI_ALLARMI_SHM dati_all;



memcpy(&dati_all,punt_shm_allarmi,sizeof(DATI_ALLARMI_SHM));

punt=dati;

for(i=0;i<16;i++)
  {
#if defined VMS
                utov_float(punt,punt);
#endif
                memcpy(test_char,punt,sizeof(float));
		if((test_char[0] == -1)&&(test_char[1] == -1)&&
			(test_char[2] == -1)&&(test_char[3] == -1))
				{
				dati_all.esiste_val[i]=0;
				}
		else
			{
                	memcpy(&valore,punt,sizeof(float));
			dati_all.esiste_val[i]=1;
			dati_all.val[i]=valore;
			/***************
			printf("ins_dati_shm_all [%d] = %f\n",i,valore);
			***************/
			}
               	punt += SIZE_ANA;
  }
memcpy(punt_shm_allarmi,&dati_all,sizeof(DATI_ALLARMI_SHM));
}
/***
Funzioni di gestione lockminiASD
***/
/*
Funzione che restituisce lo stato sel lock per miniASD
*/
int status_lock_miniASD()
{
return(lock_miniASD.type_lock);
}
/*
Funzione di settaggio del lock miniASD
*/
int  set_lock_miniASD(int value)
{
time_t tempo;
        lock_miniASD.type_lock=value;
        /*
        Setto valore timeout
        */
        time(&tempo);
        lock_miniASD.time_start=tempo;
}
/*
Funzione di sblocco per lock miniASD
*/
int  unset_lock_miniASD()
{
        lock_miniASD.type_lock=UNLOCK;
        lock_miniASD.time_start=0;
}

int lock_enabled()
{
time_t tempo_attuale;
time_t diff_time;
/*
Se mini ASD non lockato lock disabled
*/
if(lock_miniASD.type_lock==UNLOCK)
   {
//printf("CLIENT SCADA: lock_enabled = UNLOCK \n");
   return(0);
   }
time(&tempo_attuale);
diff_time=tempo_attuale-lock_miniASD.time_start; 
if(lock_miniASD.type_lock==LOCKRICALL && diff_time > TIMEOUT_MINIASD_RICALL)
   {
   fprintf(stderr,"WARNING: last operation with miniASD failed !!!\n");
   fprintf(stderr,"WARNING: TIMEOUT miniASD RICALL elapsed !!!\n");
   unset_lock_miniASD();
   return(0);
   }
else if(diff_time>TIMEOUT_MINIASD)
   {
   fprintf(stderr,"WARNING: last operation with miniASD failed !!!\n");
   fprintf(stderr,"WARNING: TIMEOUT miniASD elapsed !!!\n");
   unset_lock_miniASD();
   return(0);
   }
else           /* TIMEOUT scaduto */
   {
//printf("CLIENT SCADA: lock_enabled = LOCKRICALL and not TIMOUT \n");
   return(1);
   }
}




