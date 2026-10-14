/**********************************************************************
*
*       C Source:               sked_s02.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed Aug 23 15:37:18 2006 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: sked_s02.c-10 %  (%full_filespec: sked_s02.c-10:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)sked_s02.c	5.2\t2/7/96";
/*
   modulo sked_s02.c
   tipo 
   release 5.2
   data 2/7/96
   reserved @(#)sked_s02.c	5.2
*/
# include <stdio.h>
# include <unistd.h>
# include <netdb.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <sys/shm.h>
# include <sys/stat.h>
#endif
#if defined VMS
# include"vmsipc.h"
# include <stat.h>
#endif
# include <Rt/RtDbPunti.h>
# include <Rt/RtMemory.h>
# include "sim_param.h"
# include "sim_types.h"
# include "sim_ipc.h"
# include "comandi.h"
# include "sked.h"
# include "sked_fun.h"

#if defined SCO_UNIX
#define vfork() fork()
#endif

#define     MAXHOSTNAMELEN 256


extern char     host[MAX_LUN_HOST];

extern int      id_msg_sked;	/* id coda messaggi  da processi read */

extern int      id_msg_leg;	/* id coda messaggi  da processi aus */

extern char     nodo_master[MAX_LUN_HOST];	/* nome del nodo MASTER   */

extern int      tipo_sked;	/* tipo di schedulatore */

extern int      nmod;


extern S02      s02_;		/* struttura modelli e var */

int             fp_slave[MAX_MODEL];

int             fp_read_master;

int             fp_legograf_master;

int             fp_legograf[MAX_MODEL];

char            host_locale[30];

extern int      shr_usr_key;	/* chiave utente per shared  */

extern int      indice_macchina;

extern int    _MAX_SNAP_SHOT;
extern int    _MAX_BACK_TRACK;
extern int    _MAX_CAMPIONI;
extern int    _NUM_VAR;
extern int    _MAX_PERTUR;
extern int    _SPARE_SNAP;
extern int    _PERT_CLEAR;
extern char   _SCADA_NAME[MAX_LUN_HOST];

#define PARARGV 21
#define DEBUG_STAMPE_HOST 21

void sked_s02()
{
   PACCHETTO_NET   messaggio;
   MSG_LEGOGRAF    messaggio_legograf;
   HEADER_NET      header_net;
   char           *appoggio;
   int             i, j, k;
   char           *argv[PARARGV];
   char           *comando;
   char            appoggio_1[80];
   char            appoggio_2[80];
   struct stat    *stat_buf;
   FILE           *fp_s02;
   int             formato;
   int             formato_upd = 0;
   char            nome_S02[10];
   int             app_int;
   HEADER_REGISTRAZIONI h_reg;
   char            ptr[MAXHOSTNAMELEN];

   if (tipo_sked == MASTER)
   {
      read_S02_net();

//      printf("_SCADA_NAME = %s\n",_SCADA_NAME);

/*
   Si controlla il nome della macchina SCADA specificato nel file
   Simulator. Se nessuna macchina e' specificata vale quello che
   e' indicato nel file S01.
*/
      if(strcmp(_SCADA_NAME,"")!=0) {
         if(strcmp(_SCADA_NAME,"NULL")==0) {
/*
   E' stato specificato il valore 'NULL'. Il simulatore parte senza
   agganciare lo SCADA.
*/
            strcpy(s02_.host_scada,"");
         }
         else {
/*
   E' stata specificata una macchina per lo SCADA. Il simulatore
   sostituisce quella specificata nel file S01 con quest'ultima.
*/
            strcpy(s02_.host_scada,_SCADA_NAME);
         }
      }
         
      nmod = s02_.nmod;
/*
   Si controlla se il nome specificato nel file /etc/hosts e':
   loopback oppure localhost.
*/ 
      strcpy(host_locale,"loopback");
      if(gethostbyname(host_locale) == NULL)
         strcpy(host_locale,"localhost");
/*
printf("local_host = %s\n",host_locale);
*/
      for(i=0; i<nmod; i++) {
/*
   Si aggiorna la struttura s02_ in base all'host_locale trovato.
*/

// printf("s02_.model[%d] = %s\n",i,s02_.model[i].host);


         if(strcmp(s02_.model[i].host,"loopback")==0) {
            if(strcmp(host_locale,s02_.model[i].host) != 0)
               strcpy(s02_.model[i].host,host_locale);
         }
      }

      for (i = 0; i < nmod; i++)
      {
	 for (j = 0; j <= i; j++)
	    if (conf_host(s02_.model[i].host, s02_.model[j].host) == 0)
	       break;
	 if (
              (i == j) && 
              (conf_host(s02_.model[i].host, nodo_master)!= 0) &&
              (strcmp(s02_.model[i].host, host_locale)!= 0) 
            )
	 {
	    /* lancio lo sked SLAVE (tramite do_demone) su una nuova macchina */
	    if (strcmp(s02_.model[i].sistema_operativo, "AIX") == 0)
	    {
	       formato = 1;
	       formato_upd = 0;
	    }
	    if (strcmp(s02_.model[i].sistema_operativo, "ULTRIX") == 0)
	    {
	       formato = 4;
	       formato_upd = 1;
	    }
	    if (strcmp(s02_.model[i].sistema_operativo, "OSF1") == 0)
	    {
	       formato = 4;
	       formato_upd = 1;
	    }
	    if (strcmp(s02_.model[i].sistema_operativo, "VMS") == 0)
	    {
	       formato = 7;
	       formato_upd = 2;
	    }
	    if (strcmp(s02_.model[i].sistema_operativo, "LINUX") == 0)
	    {
	       formato = 4;
	       formato_upd = 1;
	    }
#if defined AIX
	    formato += 0;
	    formato_upd += 1;
#endif
#if defined ULTRIX || defined OSF1 || defined LINUX
	    formato += 1;
	    formato_upd += 4;
#endif
#if defined VMS
	    formato += 2;
	    formato_upd += 7;
#endif
	    sprintf(appoggio_1, "%d", formato);
	    sprintf(appoggio_2, " ");
	    printf("sked_s02:  msg datagram verso demone macch. %s (mod=%d)\n",
		   s02_.model[i].host,j);
            sked_prepara_header_reg(&h_reg);
            printf("sked_s02:do_demone con nodo_master=%s [%s] [%s]\n",
                   nodo_master,appoggio_2,appoggio_1);
	    do_demone(SLAVE, s02_.model[i].host, nodo_master, 1,
		      appoggio_2, appoggio_1, &h_reg);

	    /* apro il socket in scrittura */
printf("\t Chiamo socketscrittura con host=[%s]\n",s02_.model[i].host);
	    fp_slave[i] = socketscrittura(s02_.model[i].host, 0);
	    header_net.sorg = (-1);	/* momentaneamente a -1 (era 12345, che in un char diventa 57) */
	    header_net.dest = (-1);	/* momentaneamente a -1 */
	    header_net.tasksorg = (-1);
	    header_net.taskdest = (-1);
	    header_net.tipo = DATISTARTUP;
	    header_net.flag = (-1);
	    header_net.codice = (-1);
	    /* calcolo le dimensione del file S02 */
	    if (strcmp(s02_.model[i].sistema_operativo, "AIX") == 0)
	       sprintf(nome_S02, "S02_AIX");
	    if (strcmp(s02_.model[i].sistema_operativo, "ULTRIX") == 0)
	       sprintf(nome_S02, "S02_ULTRIX");
	    if (strcmp(s02_.model[i].sistema_operativo, "OSF1") == 0)
	       sprintf(nome_S02, "S02_OSF1");
	    if (strcmp(s02_.model[i].sistema_operativo, "VMS") == 0)
	       sprintf(nome_S02, "S02_VMS");
	    if (strcmp(s02_.model[i].sistema_operativo, "LINUX") == 0)
	       sprintf(nome_S02, "S02_LINUX");
	    stat_buf = (struct stat *) malloc(sizeof(struct stat));
	    stat(nome_S02, stat_buf);
	    header_net.lun = stat_buf->st_size;
	    free(stat_buf);
	    appoggio = (char *) malloc(sizeof(HEADER_NET) +
				       header_net.lun);
	    memcpy(appoggio, &header_net, sizeof(HEADER_NET));
	    fp_s02 = fopen(nome_S02, "r");
	    fread(&appoggio[sizeof(HEADER_NET)],
		  header_net.lun, 1, fp_s02);
	    printf("sked_s02: invio dati S02 verso %s (%d)\n",
                    s02_.model[i].host, header_net.lun);
	    writen(fp_slave[i], appoggio, sizeof(HEADER_NET) +
		   header_net.lun);
	    free(appoggio);

	    /* lancio i processi di legograf (net_updbase) di write */
	    argv[0] = (char *) malloc(FILENAME_MAX);
	    argv[1] = (char *) malloc(FILENAME_MAX);
	    argv[2] = (char *) malloc(FILENAME_MAX);
	    comando = (char *) malloc(FILENAME_MAX);
#if defined UNIX
	    sprintf(comando, "%s/net_updbase\00", getenv("LEGORT_BIN"));
#endif
#if defined VMS
	    sprintf(comando, "%snet_updbase\00", getenv("LEGORT_BIN"));
#endif
	    sprintf(argv[0], "net_updbase\00");
	    sprintf(argv[1], "%s\00", s02_.model[i].host);
	    sprintf(argv[2], "%d\00", formato_upd);
	    fp_legograf[i] = vfork();
	    if (fp_legograf[i] == 0)
	    {
#ifndef SCO_UNIX
	       sigsetmask(0);	/* sblocca tutti i segnali */
#endif
	       execl(comando, argv[0], argv[1], argv[2], (char *) 0);
	       exit(0);
	    }
	    msg_rcv(id_msg_sked, &messaggio, sizeof(HEADER_NET),
		    (long)ACKOWNLEDGE, !IPC_NOWAIT, TIMEOUT_AUS);
	    /*
	     * calcola le sezioni e le dimensioni che devono essere ricevute 
	     */
	    for (k = 0; k < nmod; k++)
	    {
	       if (conf_host(s02_.model[i].host, s02_.model[k].host) == 0)
               {
                  messaggio_legograf.dati.sezione[k] = 1;
                  messaggio_legograf.dati.dimensione[k] =
                     (s02_.model[k].punt_ider - s02_.model[k].punt_ivus)
                     * sizeof(float);
                  printf("host=%s mod k=%d con (%d %d) %d\n",
                         host,
                         k,
                         s02_.model[k].punt_ider,
                         s02_.model[k].punt_ivus,
                         messaggio_legograf.dati.dimensione[k]);
               } else
               {
                  messaggio_legograf.dati.sezione[k] = 0;
                  messaggio_legograf.dati.dimensione[k] = 0;
               }
            }
            messaggio_legograf.mtype = LEGOGRAF;
            msg_snd(id_msg_leg, &messaggio_legograf, sizeof(TIPO_LEGOGRAF),
                    !IPC_NOWAIT);
         } 
      }
   } else
   {
      /* lancio il processo di read com il master */
      for(i=0;i<PARARGV;i++)
         argv[i] = (char *) malloc(FILENAME_MAX);

      comando = (char *) malloc(FILENAME_MAX);
      sprintf(argv[0], "net_read\00");
      sprintf(argv[1], "%s\00", nodo_master);
      sprintf(argv[2], "%d\00", indice_macchina);
      app_int = 0;
      sprintf(argv[3], "%d\00", app_int);
      sprintf(argv[4], "%d\00", 1);

      i = 5;
      sprintf(argv[i], "%s\00", ARGV_N_SNAP);i++;
      sprintf(argv[i], "%d\00",_MAX_SNAP_SHOT);i++;
      sprintf(argv[i], "%s\00", ARGV_N_BKTK);i++;
      sprintf(argv[i], "%d\00",_MAX_BACK_TRACK);i++;
      sprintf(argv[i], "%s\00", ARGV_N_CAMP_CR);i++;
      sprintf(argv[i], "%d\00",_MAX_CAMPIONI);i++;
      sprintf(argv[i], "%s\00", ARGV_N_VAR_CR);i++;
      sprintf(argv[i], "%d\00",_NUM_VAR);i++;
      sprintf(argv[i], "%s\00", ARGV_N_PERT_ACTIVE);i++;
      sprintf(argv[i], "%d\00",_MAX_PERTUR);i++;
      sprintf(argv[i], "%s\00", ARGV_N_SPARE);i++;
      sprintf(argv[i], "%d\00",_SPARE_SNAP);i++;
      sprintf(argv[i], "%s\00", ARGV_N_PERTCL);i++;
      sprintf(argv[i], "%d\00",_PERT_CLEAR);i++;
      argv[i] = NULL;

#if defined UNIX
      sprintf(comando, "%s/net_read\00", getenv("LEGORT_BIN"));
#endif
#if defined VMS
      sprintf(comando, "%snet_read\00", getenv("LEGORT_BIN"));
#endif
      fp_read_master = vfork();
      if (fp_read_master == 0)
      {
         execv(comando, argv);
         exit(0);
      }
      msg_rcv(id_msg_sked, &messaggio, sizeof(HEADER_NET),
              (long)ACKOWNLEDGE, !IPC_NOWAIT, TIMEOUT_AUS);
      msg_rcv(id_msg_sked, &messaggio, sizeof(HEADER_NET),
              (long)DATISTARTUP, !IPC_NOWAIT, TIMEOUT_AUS * 2);
      read_S02_net();

      sked_prolog();

      /* lancio i processi di legograf di write */
      sprintf(argv[0], "net_updbase\00");
      sprintf(argv[1], "%s\00", nodo_master);
      sprintf(argv[2], "%d\00", 0);
#if defined UNIX
      sprintf(comando, "%s/net_updbase\00", getenv("LEGORT_BIN"));
#endif
#if defined VMS
      sprintf(comando, "%snet_updbase\00", getenv("LEGORT_BIN"));
#endif
      fp_legograf_master = vfork();
      if (fp_legograf_master == 0)
      {
         execl(comando, argv[0], argv[1], argv[2], (char *) 0);
         exit(0);
      }
      msg_rcv(id_msg_sked, &messaggio, sizeof(HEADER_NET),
              (long)ACKOWNLEDGE, !IPC_NOWAIT, TIMEOUT_AUS);
      /*
       * calcola le sezioni e le dimensioni che devono essere spedite
       */
      strcpy(host, io_sono());
      nmod = s02_.nmod;
      for (k = 0; k < s02_.nmod; k++)
      {
         if (conf_host(s02_.model[k].host, host) == 0)
         {
            messaggio_legograf.dati.sezione[k] = 1;
            messaggio_legograf.dati.dimensione[k] =
               (s02_.model[k].punt_ider - s02_.model[k].punt_ivus)
               * sizeof(float);
         printf("host=%s mod k=%d con (%d %d) %d\n",
                 host,
                 k,
                 s02_.model[k].punt_ider,
                 s02_.model[k].punt_ivus,
                 messaggio_legograf.dati.dimensione[k]);
         } else
         {
            messaggio_legograf.dati.sezione[k] = 0;
            messaggio_legograf.dati.dimensione[k] = 0;
         }
      }
      messaggio_legograf.mtype = LEGOGRAF;
      msg_snd(id_msg_leg, &messaggio_legograf, sizeof(TIPO_LEGOGRAF),
              !IPC_NOWAIT);
   for(i=0;i<PARARGV;i++)
      free(argv[i]);
   }
   /* apertura delle code relativa alla sincronizzazione delle TASK */
   msg_create_fam_sim(shr_usr_key, nmod, 1);
}


