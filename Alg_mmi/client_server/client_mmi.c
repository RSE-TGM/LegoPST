/**********************************************************************
*
*       C Source:               client_mmi.c
*       Subsystem:              2
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Mon Aug  4 17:21:30 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: client_mmi.c-8 %  (%full_filespec: client_mmi.c-8:csrc:2 %)";
#endif

/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)client_mmi.c	1.17\t4/12/96";
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#if defined UNIX
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#else
#include <iodef.h>
#include <types.h>
#include <socket.h>
#include <in.h>
#include <netdb.h>
#include <inet.h>
#include <ucx$inetdef.h>
#include "vmsipc.h"
#endif



#include "sim_param.h"
#include "sim_ipc.h"
#include "sim_types.h"
#include "comandi.h"
#include "demone_mmi.h"
#include "libnet.h"
#include <Cs/Cs.h>


#if defined VMS
#define pid_t int
#endif

#define errore(stringa)         exit(fprintf(stderr,"Attention !!!\n      Error in process client_mmi[%d]: %s\n",getpid(),stringa))

static int invia_richiesta_connessione(char *, int);
static void main_mmi_client_command();
static int main_mmi_client_data();
static void main_mmi_client_pert();
static int socketclient_mmi();
static pid_t genera_client_mmi();
static void handler_client_mmi();
extern int tab_piena(TAB_SHARED *);
extern int send_mmi_ret_ack(int ,int );
extern int cerca_shm(TAB_SHARED *, int );
static int verify_sim(int,char *);
int tipo_client;
int port_daemon;      /* specifica No di porta ove trovare demone_mmi */

static int abilita_richiesta_connessione(char *);
static int write_shm(TAB_SHARED, float *);

char *crea_shrmem(int,int,int *);
extern int sgancia_shrmem(char *);
extern void elimina_shrmem(int,char*,int);
extern char* io_sono();
int CsRcvMsgDel(int );
int CsRcvMsgChangePoint(int ,MSG_CHANGE_POINT_MMI *);
int CsRcvMsgF22(int , char **);

pid_t client_command = 0,
      client_data = 0,
      client_pert = 0;





int main(argc,argv)
int argc;
char **argv;
{
int port;
int server_port;
int i;

/*
Inizializzazione port_daemon a porta base (8888)
*/
printf("PROCESSO CLIENT_MMI\n\n\n");
if(argc<3)
	{
	printf("client_mmi: Si e' verificato un errore\n");
	errore("use: client_mmi  <hostname> <port_daemon_mmi>");
	}
if(argc==3)
	{
	tipo_client=CLIENT_MMI_FIRST;
        port_daemon=atoi(argv[2]);
	printf("tipo client_mmi first\n");
	}
else
	{
	tipo_client=atoi(argv[2]);
	server_port=atoi(argv[3]);
        port_daemon=atoi(argv[4]);
	}
if(port_daemon<0)
   port_daemon=DEMONE_PORT;

printf("\n");
switch(tipo_client)
        {
	case CLIENT_MMI_FIRST:
		{
		printf("COMMAND=%d PERT=%d DATA=%d\n",
                        SERVER_MMI_COMMAND,SERVER_MMI_PERT,SERVER_MMI_DATA);
		printf("CLIENT_MMI_FIRST  \n");
                if(abilita_richiesta_connessione(argv[1]))
                {   
                   printf("\nclient_mmi:abilita_richiesta_connessione Ok\n");
		   if( (port = invia_richiesta_connessione(argv[1],
                                                        SERVER_MMI_COMMAND))<0)
			errore("Connection impossible");
		   printf("PORTA di connessione = %d\n",port);
		   client_command = genera_client_mmi(argv[1],
                                                   CLIENT_MMI_COMMAND,port);

		   if( (port = invia_richiesta_connessione(argv[1],
			                                SERVER_MMI_PERT))<0)
			errore("Connection impossible");
		   printf("PORTA di connessione = %d\n",port);
		   client_pert = genera_client_mmi(argv[1],
                                                CLIENT_MMI_PERT,port);

		   if( ( port = invia_richiesta_connessione(argv[1],
                                                         SERVER_MMI_DATA))<0)
			errore("Connection impossible");
		   printf("PORTA di connessione = %d\n",port);
		   client_data=genera_client_mmi(argv[1],CLIENT_MMI_DATA,port);
		/*
			si sospende sino al ricevimento del
			segnale SIGALRM
		*/
printf("Valore di client_data=%d client_pert=%d client_command=%d\n",client_data,client_pert,client_command);

// GUAG2025 Eliminazione di sigpause non funziona con versione attuale de SO e che oltretutto é anacronistico e deprecato.
//		   signal(SIGALRM,handler_client_mmi); 
//		   sigpause(0);
           sigset_t mask_vuota, mask_originale;
           signal(SIGALRM,handler_client_mmi);
           // Prepara una maschera vuota (nessun segnale bloccato)
           sigemptyset(&mask_vuota);

           // Blocca SIGALRM prima di entrare in attesa per evitare race condition
           sigprocmask(SIG_BLOCK, NULL, &mask_originale); // Ottieni maschera corrente
           sigset_t mask_blocco_alrm = mask_originale;
           sigaddset(&mask_blocco_alrm, SIGALRM);
           sigprocmask(SIG_SETMASK, &mask_blocco_alrm, NULL); // Blocca SIGALRM

           printf("Client MMI principale in attesa di segnale...\n");

           // Attendi un segnale, sbloccando atomicamente tutti i segnali (usando mask_vuota)
           // Quando sigsuspend ritorna (dopo che l'handler è stato eseguito),
           // la maschera originale viene ripristinata automaticamente.
           sigsuspend(&mask_vuota);

           // A questo punto, il segnale (probabilmente SIGALRM) è arrivato
           // e l'handler è stato eseguito.
           // La maschera originale dovrebbe essere già ripristinata da sigsuspend,
           // ma per sicurezza potresti ripristinarla esplicitamente se non sei sicuro
           // del comportamento esatto del kernel/libc.
           // sigprocmask(SIG_SETMASK, &mask_originale, NULL);

           printf("Client MMI principale risvegliato.\n");         
//  fine modifica sigpause         

                 } /* if abilita */
              else
                 {
		 printf("client_mmi: connessione no abilitata\n");
		 errore("connection refused");
                 }

		/*
			termina tutti i processi client
		*/
                printf("DEBUG: Sto per killare tutti i client_mmi\n");
		if(client_command>0)
			kill(client_command,SIGTERM);
		if(client_data>0)
			kill(client_data,SIGTERM);
		if(client_pert>0)
			kill(client_pert,SIGTERM);
		break;
		}
	case CLIENT_MMI_COMMAND:
		{
		printf("CLIENT_MMI_COMMAND\n");
                main_mmi_client_command(argv[1],server_port);
		break;
		}
	case CLIENT_MMI_DATA:
		{
		printf("CLIENT_MMI_DATA\n");
                main_mmi_client_data(argv[1],server_port);
		break;
		}
	case CLIENT_MMI_PERT:
		{
		printf("CLIENT_MMI_PERT\n");
                main_mmi_client_pert(argv[1],server_port);
		break;
		}
	default:
		{
		printf("client_mmi: sono nel caso di default\n");
		errore("type error");
		}
	}
}


/*
Funzione per richiesta connessione con un server del simulatore
Parametri:
	char *host : macchina del server
        int richiesta : tipo di server (1 command 2 data 3 pert)
Return:
	int : No di porta su cui il server e' in attesa .
*/

static int invia_richiesta_connessione(host, richiesta)
char *host;
int richiesta;
{
int ret = -1;
MSG_DEMONE_MMI messaggio;
struct sockaddr_in client, server;
struct hostent *hp;
int size_info_client=sizeof(client);
int socd;
char path_pwd[FILENAME_MAX];
int id_msg_client;   /* id coda msg FROM_MMI_MSG */
int appo_shr_usr_key;


/* 
	preparazione messaggio
*/
printf("invia_richiesta_connessione:host=%s richiesta=%d\n",host,richiesta);
strcpy(messaggio.host,io_sono());
messaggio.richiesta=richiesta;
CONVERTI_INT_T(messaggio.richiesta);
/*
Scrivo stringa dummy in path_sim
*/
strcpy(messaggio.path_sim,RICHIESTA_SERVER);

/*
	trasmissione messaggio
*/

if((hp = gethostbyname(host))==0)
	errore("unknown host");
memset(&server,0,sizeof(server));
memcpy((char *)&server.sin_addr,(char *)hp->h_addr,hp->h_length);
server.sin_family = AF_INET;
server.sin_port = htons(port_daemon);


/*
 Connessione iniziale tra client e demone per scambiarsi
 il numero di porta della connessione definitiva
*/
if((socd = socket( AF_INET, SOCK_STREAM, 0))<0)
	errore("opening datagram socket");

if(connect(socd, (const struct sockaddr *)&server, sizeof(server)) == -1 )
	{
	perror("client_mmi: invia_richiesta_connessione");
	errore("connect iniziale tra client e demone");
	}

printf("=== invio ====\n");
messaggio.port = 66;
printf("    socd=%d sizeof(MSG_DEMONE_MMI)=%d sizeof(server)=%d\n",
            socd,sizeof(MSG_DEMONE_MMI),sizeof (server));
printf("    host=%s richiesta=%d port=%d\n",
            messaggio.host,messaggio.richiesta,messaggio.port);
/*
Invia al demone il messaggio di richiesta connessione
*/
write(socd,&messaggio, sizeof(MSG_DEMONE_MMI));

/*
	leggo il messaggio di ritorno con il numero di porta
	da utilizzarsi per la connessione definitiva
*/
read(socd,&messaggio, sizeof(MSG_DEMONE_MMI));
if(shutdown(socd,0) == -1)
	errore("shutdown socket connessione clent-demone");
CONVERTI_INT_R(messaggio.port);
ret = messaggio.port;

/*
 chiusura del socket per richiesta connessione
*/
close(socd);

/*
  ritorna il numero di porta per la connessione
*/
return (ret);

}



/*
Funzione preliminare che mi consente di verificare che il simulatore 
a cui mi aggancio sia quello giusto

Parameter:
	char *host : nome host dove demone_mmi e' in attesa
Return: 
	int : se positivo connessione abilitata
        
*/

static int abilita_richiesta_connessione(host)
char *host;
{
int ret = -1;
MSG_DEMONE_MMI messaggio;
struct sockaddr_in client, server;
struct hostent *hp;
int size_info_client=sizeof(client);
int socd;
char path_pwd[FILENAME_MAX];
int id_msg_client;   /* id coda msg FROM_MMI_MSG */
int appo_shr_usr_key;


/* 
	preparazione messaggio
*/

strcpy(messaggio.host,io_sono());
messaggio.richiesta=REQ_ABILITAZIONE;
CONVERTI_INT_T(messaggio.richiesta);

/*
Scrivo nome dir dove c'e' il variabili.rtf del simulatore
*/
strcpy(messaggio.path_sim,ABILITAZIONE);


/*
	trasmissione messaggio
*/

if((hp = gethostbyname(host))==0)
	errore("unknown host");
memset(&server,0,sizeof(server));
memcpy((char *)&server.sin_addr,(char *)hp->h_addr,hp->h_length);
server.sin_family = AF_INET;
server.sin_port = htons(port_daemon);
/*
 Connessione iniziale tra client e demone per scambiarsi
 il numero di porta della connessione definitiva
*/
if((socd = socket( AF_INET, SOCK_STREAM, 0))<0)
	errore("opening datagram socket");

if(connect(socd, (const struct sockaddr *)&server, sizeof(server)) == -1 )
	{
	perror("client_mmi: invia_richiesta_connessione");
	errore("connect iniziale tra client e demone");
	}

printf("=== invio ====\n");
messaggio.port = 66;
printf("    socd=%d sizeof(MSG_DEMONE_MMI)=%d sizeof(server)=%d\n",
            socd,sizeof(MSG_DEMONE_MMI),sizeof (server));
printf("    host=%s richiesta=%d port=%d\n",
            messaggio.host,messaggio.richiesta,messaggio.port);
/*
Invia al demone il messaggio di richiesta connessione
*/
write(socd,&messaggio, sizeof(MSG_DEMONE_MMI));


/*
Aspetta da demone mmi msg in cui viene e' inviato il
path che identifica (per startup) il simulatore
*/
read(socd,&messaggio, sizeof(MSG_DEMONE_MMI));
if(shutdown(socd,0) == -1)
	errore("shutdown socket connessione clent-demone");


/*
 chiusura del socket per richiesta abilitazione
*/
close(socd);

/*
Verifica che il simulatore sia quello corretto
trasmettendo il path ( appena comunicato da demone_mmi) all' mmi
*/

/*
        apertura coda ricevimento comandi
        La KEY associata alla coda e' funzione della SHR_USR_KEYS
        per poter avere + mmi sulla stessa macchina
*/

if(atoi(getenv("SHR_USR_KEY"))==NULL)
     {
     printf("client_mmi: ATTENZIONE SHR_USR_KEY non settata !!!\n");
     exit(0);
     }
appo_shr_usr_key=atoi(getenv("SHR_USR_KEY"));

   /*
   aggancio alla coda creata in OlDbPunti in attiva_client
   */
id_msg_client=msg_create(appo_shr_usr_key+FROM_MMI_MSG,0);

/*
Verifico che il simulatore sia identico a quello settato nella risorsa 
simulator del Context.ctx.
*/ 

if(verify_sim(id_msg_client,messaggio.path_sim)<=0)
   {
    printf("client_mmi: ERROR Simulator not correct. Exiting !!!\n");
    ret=0;
   }
else
   ret=1;

/*
  ritorna 1 solo se mmi ha accettato il path comunicato
  da skedulatore
*/
return (ret);
}


static int     socketclient_mmi (macchina, server_port)
char   *macchina;
int     server_port;
{
int     fp;
struct sockaddr_in  client;
struct hostent *hp;
#if defined UNIX
struct hostent *gethostbyname ();
#endif
char    io[255];
int     tentativi = 0;


    while (1)
    {
/* apertura socket */
	fp = socket (AF_INET, SOCK_STREAM, 0);
	if (fp < 0)
	{
	    perror ("opening stream socket");
	    exit (0);
	}

	client.sin_family = AF_INET;
	client.sin_addr.s_addr = INADDR_ANY;

	hp = gethostbyname (macchina);
	if (hp == 0)
	{
	    printf ("%s: unknown host\n", macchina);
	    exit (0);
	}
	strcpy (io, io_sono ());
	memcpy ((char *) & client.sin_addr, (char *) hp -> h_addr, hp -> h_length);
	client.sin_port = htons (server_port);
	if (connect (fp, (struct sockaddr  *) & client, sizeof (client)) < 0)
	{
	                                        sleep (1);
	    if (++tentativi == 20)
		exit (printf ("Connessione con %s fallita\n", macchina));
	}
	else
	{
	    return (fp);
	}
    }
}

static pid_t genera_client_mmi(char *host, int tipo, int port)
{
char *path_bin;
char task_name[256];
char *task_argv[6];
char *task_envp[4];
pid_t pid_client;

task_argv[0]=(char*)malloc(256);
task_argv[1]=(char*)malloc(256);
task_argv[2]=(char*)malloc(256);
task_argv[3]=(char*)malloc(256);
task_argv[4]=(char*)malloc(256);
task_argv[5]=(char*)malloc(256);
task_envp[0]=(char*)malloc(256);
task_envp[1]=(char*)malloc(256);
task_envp[2]=(char*)malloc(256);
task_envp[3]=(char*)malloc(256);

printf("genera_client_mmi %s tipo=%d porta=%d\n",host,tipo,port);

path_bin = getenv("LEGOMMI_BIN");
#if defined UNIX
sprintf(task_name,"%s/client_mmi\00",path_bin);
#else
sprintf(task_name,"%sclient_mmi\00",path_bin);
#endif

sprintf(task_argv[0],"client_mmi\00");
sprintf(task_argv[1],"%s\00",host);
sprintf(task_argv[2],"%d\00",tipo);
sprintf(task_argv[3],"%d\00",port);
sprintf(task_argv[4],"%d\00",port_daemon);
task_argv[5]=NULL;
sprintf(task_envp[0],"LEGOMMI_BIN=%s\n",path_bin);
sprintf(task_envp[1],"SHR_USR_KEY=%s\n",getenv("SHR_USR_KEY"));
sprintf(task_envp[2],"FORMATO_DATI=%s\n",getenv("FORMATO_DATI"));
task_envp[3]=NULL;

pid_client=vfork();

if(pid_client==0)
       {
#if defined SCO_UNIX
      setpgrp();
      sigrelse(0);
#else
#if defined UNIX
       setpgrp();
       sigsetmask(0);  /* sblocca tutti i segnali  */
#endif
#endif
       execve(task_name,task_argv,task_envp);
       exit(0);
       }


return(pid_client);
}


static void main_mmi_client_command(char *host,int server_port)
{
int fp;
float a;
char *buffer=NULL;
int size;
TOKEN *app_token;
HEADER_NEW_PAGE_MMI *app_header;
HEADER_NEW_PAGE_MMI header;
MSG_NEW_PAGE_MMI_RET msg_ret;
MSG_CHANGE_POINT_MMI msg_point;
MSG_F22_MMI msg_f22;
int id_shm;
int i;
char *ind;
int appo_shr_usr_key;
int shmvid;
static int prima_volta=1;
/*
tabella contenente l' elenco degli ID di shm associate alle 
pagine di mmi aperte
*/
TAB_SHARED tabella[MAX_OPERAZIONI];
int ind_tab=0;    /* indice tabella */
int id_msg_client;



if(prima_volta)
   {
   prima_volta=0;
   for(i=0;i<MAX_OPERAZIONI;i++)
      {
      tabella[i].id_shm=-1;
      tabella[i].ind=NULL;
      tabella[i].num_punti=0;
      }
   }


if(atoi(getenv("SHR_USR_KEY"))==NULL)
        {
        printf("client_mmi: ATTENZIONE SHR_USR_KEY non settata !!!\n");
        exit(0);
        }
appo_shr_usr_key=atoi(getenv("SHR_USR_KEY"));


printf("main_mmi_client_command -> %s\n",host);
fp=socketclient_mmi(host,server_port);

/*
        apertura coda ricevimento comandi
        La KEY associata alla coda e' funzione della SHR_USR_KEYS
        per poter avere + mmi sulla stessa macchina
*/

printf("client_mmi_command:id coda =%d\n",appo_shr_usr_key+FROM_MMI_MSG);
id_msg_client=msg_create(appo_shr_usr_key+FROM_MMI_MSG,0);

while(1)
        {
/*      Verifica la presenza di messaggi in coda */
	if((id_shm=CsRcvToken(id_msg_client,&buffer))>0)
		{
printf("client_mmi:ricevuto msg su id_msg_client=%d\n",id_msg_client);
                /************
                parte per gestione che evita di incrementare 
                all' infinito l' id della shm associata alle pagine
                *************/
                memcpy(&header,buffer,sizeof(HEADER_NEW_PAGE_MMI));
                size = sizeof(HEADER_NEW_PAGE_MMI) + header.size;
                /* Accedo all' indice in tabella libero */
                 if( (ind_tab=tab_piena(tabella))==-1 )
                        {
                        printf("ERROR: Reached maximun number of page\n");
                        break;
                        }

                tabella[ind_tab].id_shm=appo_shr_usr_key+OFFSETMMIPAGE+ind_tab+1;
                tabella[ind_tab].num_punti = header.num_punti;

                tabella[ind_tab].ind = (char*) crea_shrmem(appo_shr_usr_key+
                            OFFSETMMIPAGE+ind_tab+1,
                            (header.num_punti+1)*sizeof(float),&shmvid);

/*
                   printf("DEBUG: ho creato pag con id_shm=%d ind_tab=%d\n",tabella[ind_tab].id_shm,ind_tab);
*/
                if(sgancia_shrmem(tabella[ind_tab].ind)<0)
                         {
                        printf("client_mmi:ERRORE command: sgancia_shrmem ind=%d\n",tabella[i].ind);
                        break;
                        }
                /*
                Manda acknowledge MMI
                */
                send_mmi_ret_ack(id_msg_client,tabella[ind_tab].id_shm);

                /* sistemo header e id_shm */
                header.id_shm=tabella[ind_tab].id_shm;
/****
                printf("\n     CLIENT_COMMAND (CsRcvToken) id_shm=%d size=%d \
                         header.size=%d num_punti=%d\n",
                         tabella[ind_tab].id_shm,
                         (header.num_punti+1)*sizeof(float),header.size,
                          tabella[ind_tab].num_punti );
****/
               
		/* converto il messaggio */
		app_header = (HEADER_NEW_PAGE_MMI*)buffer;
		app_token  = (TOKEN*)&buffer[sizeof(HEADER_NEW_PAGE_MMI)];
                app_header->id_shm=tabella[ind_tab].id_shm;
		/* converto prima i token e dopo (!!!) l'header */
		for(i=0;i<(header.size/sizeof(TOKEN));i++)
			{
			CONVERTI_INT_T(app_token[i].tipo);
			CONVERTI_INT_T(app_token[i].puntdb);
			}
		CONVERTI_INT_T(app_header->id_shm);
		CONVERTI_INT_T(app_header->t_agg);
		CONVERTI_INT_T(app_header->num_punti);
		CONVERTI_INT_T(app_header->size);
		CONVERTI_INT_T(app_header->richiesta_mmi);
                writen(fp,&buffer[0],size);
                if(buffer!=NULL)
                   free(buffer);
		}
        /*else*/
	   if((header.id_shm=CsRcvMsgDel(id_msg_client))>0)
		{
                header.num_punti = 0;
                header.t_agg = 0;
                header.size  = 0;
                header.richiesta_mmi = DEL_PAGE_MMI;
		CONVERTI_INT_T(header.num_punti);
		CONVERTI_INT_T(header.t_agg);
		CONVERTI_INT_T(header.size);
		CONVERTI_INT_T(header.id_shm);
		CONVERTI_INT_T(header.richiesta_mmi);
                /* Eliminazione SHM dopo controllo della sua presenza 
                indipendentemente da i processi agganciati
                */
                if( (ind_tab=cerca_shm(tabella,header.id_shm))==-1)
                       {
                       printf("SHM DA CANCELLARE NON TROVATA \n");
                       break;
                       }
/****
                printf("CLIENT_COMM:SHM DA CANCELLARE=%d ind_tab=%d size=%d\n",
      tabella[ind_tab].id_shm,ind_tab,(tabella[ind_tab].num_punti+1)*sizeof(float));
                printf("CLIENT_COMM: tabella[ind_tab].id_shm=%d\n",
                         tabella[ind_tab].id_shm);
****/

                    tabella[ind_tab].ind = (char *)crea_shrmem(tabella[ind_tab].id_shm,
                                        (tabella[ind_tab].num_punti+1)*sizeof(float),
                                                        &tabella[ind_tab].shmvid);

                if(( tabella[ind_tab].ind)==NULL)
                        {
                        printf("client_mmi:ERRORE command: CLT_CMDT_DELPAGE;SHMnon agganciata\n");

                        break;
                        }
                if(sgancia_shrmem(tabella[ind_tab].ind)<0)
                         {
                        printf("client_mmi:ERRORE command: sgancia_shrmem ind=%d\n",tabella[i].ind);
                        break;
                        }

                elimina_shrmem(tabella[ind_tab].shmvid,tabella[ind_tab].ind,
                                (tabella[ind_tab].num_punti+1)*sizeof(float));

                /*
                Controllo che eliminazione sia andata bene
                Se non e' stata eliminata evito di bloccare l' mmi
                */
                if(!shresist(tabella[ind_tab].id_shm))
                   {
/*
                   printf("DEBUG: elimino pag con id_shm=%d ind_tab=%d\n",tabella[ind_tab].id_shm,ind_tab);
*/
                   tabella[ind_tab].id_shm = -1;
                   }
                else
                   printf("ERROR:client_mmi_command: shm=%d not destroyed\n",
                           tabella[ind_tab].id_shm);

                writen(fp,(char*)&header,sizeof(HEADER_NEW_PAGE_MMI));

                }


           if(CsRcvMsgChangePoint(id_msg_client, &msg_point)>0)
		{
                header.num_punti = 3;
                header.t_agg = 0;
                header.size  = 0;
                header.richiesta_mmi = CHANGE_POINT_MMI;
		header.id_shm = msg_point.id_shm;
		CONVERTI_INT_T(header.num_punti);
		CONVERTI_INT_T(header.t_agg);
		CONVERTI_INT_T(header.size);
		CONVERTI_INT_T(header.richiesta_mmi);
		CONVERTI_INT_T(header.id_shm);
		CONVERTI_INT_T(msg_point.posizione);
		CONVERTI_INT_T(msg_point.indirizzo);
		CONVERTI_INT_T(msg_point.tipo);
                writen(fp,(char *)&header,sizeof(HEADER_NEW_PAGE_MMI));
                writen(fp,(char *)&(msg_point.posizione),sizeof(int));
                writen(fp,(char *)&(msg_point.indirizzo),sizeof(int));
                writen(fp,(char *)&(msg_point.tipo),sizeof(int));
		}
	   if( (size=CsRcvMsgF22(id_msg_client,&buffer))>=0 )
		{
                writen(fp,buffer,size);
                if(buffer!=NULL)
                   free(buffer);
                }
	sospendi(100);
        }
}

static void main_mmi_client_pert(char *host,int server_port)
{
int fp;
float a;
int id_msg_pert;
MSG_PERT messaggio_pert;
int shr_usr_key;


printf("main_mmi_client_pert -> %s\n",host);
fp=socketclient_mmi(host,server_port);
shr_usr_key = atoi((char *)getenv("SHR_USR_KEY"));
/*
Coda su cui oggetto OlPert spedisce perturbazioni
*/
id_msg_pert = msg_create(shr_usr_key+ID_MSG_PERT_MMI,1);

while(1)
        {
	if(msg_rcv(id_msg_pert,&messaggio_pert,sizeof(TIPO_PERT),(long)(-(OFFSET_PERT)),
        		!IPC_NOWAIT,TIMEOUT_INF)>=0)
           {
      	   CONVERTI_INT_T(messaggio_pert.perturbazione.posizione);
	   CONVERTI_INT_T(messaggio_pert.perturbazione.tipo);
	   CONVERTI_INT_T(messaggio_pert.perturbazione.indirizzo);
	   CONVERTI_INT_T(messaggio_pert.perturbazione.esaminata);
	   CONVERTI_FLOAT_T(messaggio_pert.perturbazione.valore_delta);
	   CONVERTI_FLOAT_T(messaggio_pert.perturbazione.rateo_durata);
	   CONVERTI_FLOAT_T(messaggio_pert.perturbazione.t);
	   CONVERTI_FLOAT_T(messaggio_pert.perturbazione.period);
	   CONVERTI_FLOAT_T(messaggio_pert.perturbazione.phase);
	   CONVERTI_FLOAT_T(messaggio_pert.perturbazione.meanvalue);
	   CONVERTI_FLOAT_T(messaggio_pert.perturbazione.wide);
	   CONVERTI_FLOAT_T(messaggio_pert.perturbazione.t_null_var);
	   if(writen(fp,(char *)&(messaggio_pert.perturbazione),sizeof(TIPO_PERT))!=
		sizeof(TIPO_PERT))
		errore("Close connection [write pert]\n");
           } /* end if msg_rcv >=0 */
        else
           break;
        }
       printf("WARNING:client_mmmi_pert: queue =%d not available any longer . Exiting !!!\n",
               shr_usr_key+ID_MSG_PERT_MMI);
}

static int main_mmi_client_data(char *host,int server_port)
{
int fp;
int i;
int size;
float *val=NULL;
float uno=1;
float tappo;
char *punt;
static int prima_volta=1;
HEADER_DATA_DATA hd;
char *buf_f22;
int ret;
int nbyte_read;
TAB_SHARED tabella;

if(prima_volta=1)
   {
   prima_volta=0;
   tabella.id_shm = -1;
   }
/*printf("main_mmi_client_data -> %s\n",host);*/
fp=socketclient_mmi(host,server_port);
while(1)
        {
        /*printf("client data : prima di readn\n");*/
        if( (ret=readn(fp,(char *)&hd,sizeof(HEADER_DATA_DATA)))<=0 )
           {
	   printf("ERRORE:ricezione dati da simula KO\n");
	   errore("connection close [read header<0]");
           }
	else if(ret!=sizeof(HEADER_DATA_DATA))
	   {
	   printf("ERRORE:ricezione dati da simula KO\n");
	   errore("connection close [read header>0]");
	   }
        else
           {
	   /* converto la struttura HEADER_DATA_DATA */
	   CONVERTI_INT_R(hd.tipo_operazione);
	   CONVERTI_INT_R(hd.id_shm);
	   CONVERTI_INT_R(hd.num_punti);
           switch(hd.tipo_operazione)
                 {
                 case CLT_CMDT_NEWPAGE:
                    {
/*                  Aggancio e inserimento nuova SHM in tabella */
                    size = hd.num_punti * sizeof(float);
                    val = (float*)calloc(hd.num_punti,sizeof(float));
                    if((nbyte_read=readn(fp,(char *)val,size))<=0)
                             errore("connection close [read data NEWPAGE]");
		    if(nbyte_read!=size)
				errore("connection close [read data NEWPAGE >0]");


                    tabella.num_punti = hd.num_punti;
                    tabella.id_shm    = hd.id_shm;
                    printf("AGGANCIO con : id_shm=%d size=%d shmvid=%d\n",
                            tabella.id_shm,(hd.num_punti+1)*sizeof(float),
                            tabella.shmvid);
                    if(shresist(tabella.id_shm))
                       tabella.ind  = (char*)crea_shrmem(tabella.id_shm,
                                                (hd.num_punti+1)*sizeof(float),
                                                &tabella.shmvid);
                    else
                       {
/*
                       printf("DEBUG: client_mmi_data: shm not exist\n");
*/
                       break;
                       }

		      if ((tabella.ind)==NULL)
                       {
                       printf("ERRORE client_data ind <%d>\n",tabella.ind);
                       break;
                       }
                    write_shm(tabella,val);
                    if(sgancia_shrmem(tabella.ind)<0)
			{
			printf("ERRORE:client_data:sgancia_shrmem con ind=%d KO\n",
				tabella.ind);
			break;
			}
                    if(val!=NULL)
                      free(val);
                    break;
                    } 
                 case CLT_CMDT_REFPAGE:
                    {
/*                  Refresh SHM dopo controllo della sua presenza  */

                    size = hd.num_punti * sizeof(float);
                    val = (float*)calloc(hd.num_punti,sizeof(float));
                    if((nbyte_read=readn(fp,(char *)val,size))<=0)
                             errore("connection close [read data REFPAGE]");
		    if(nbyte_read!=size)
			     errore("connection close [read data REFPAGE>0]");

                    tabella.id_shm=hd.id_shm;
                    tabella.num_punti=hd.num_punti;

                    /*
                    Verifico che la shm esista
                    */
                    if(shresist(tabella.id_shm))
                       tabella.ind = (char *)crea_shrmem(tabella.id_shm,
                                        (tabella.num_punti+1)*sizeof(float),
                                                         &tabella.shmvid);
                    else 
                       break;

		    if((tabella.ind)==NULL)
			{
			printf("Client_mmi_data:WARNING: command:CLT_CMDT_REFPAGE  SHM_ID=%d size=%d non hooked\n",tabella.id_shm,(tabella.num_punti+1)*sizeof(float));	
			break;
			}

                    punt = tabella.ind + tabella.num_punti*sizeof(float);
                    memcpy(&tappo,punt,sizeof(float));
                    if(tappo!=uno)
                       {
                       printf("SHM IN TABELLA MA NON INIZIALIZZATA\n");
                       if(sgancia_shrmem(tabella.ind)<0)
				{
				printf("Client_mmi_data:ERRORE sgancia_shrmem ind=%d KO\n",tabella.ind);
				break;
				}
                       if(val!=NULL)
                          free(val);
                       break;
                       }
                    else
                       {
/*
                       printf("REFRESH  id_shm=%d ind=%d shmvid=%d\n",
                               hd.id_shm,tabella.ind,tabella.shmvid);
*/
                       write_shm(tabella,val);
                       if(sgancia_shrmem(tabella.ind)<0)
				{
                                printf("Client_mmi:ERRORE sgancia_shrmem ind=%dKO\n",tabella.ind);
                                break;
                                }

                       if(val!=NULL)
                          free(val);
                       break;
                       }
                    }
                 case CLT_CMDT_DELPAGE:
                    {
                    break;
                    }
                 case CLT_CMDT_F22PAGE:
                    {
                    printf("Client_mmi ha ricevuto il file f22\n");
                    size = hd.num_punti;
                    buf_f22 = (char *)malloc(size);
                    nbyte_read=readn(fp,buf_f22,size);
		    if(nbyte_read!=size)
			errore("Problemi con CLT_CMDT_F22PAGE\n");
		    
/*
                    ADESSO CHE HO RICEVUTO I DATI FACCIO QUELLO CHE VOGLIO
*/
                    if(buf_f22!=NULL)
                        free(buf_f22);
                    break;
                    }
                 case CLT_CMDT_WATCHDOG:
                    {
                    break;
                    }
                 default :
                    {
                    printf("client_mmi_data,tipo_operazione non contemplata\n");
                    break;
                    }
                 }      /* Fine switch */
           } 
        }
printf("\n\nDEBUG: esco da client_command\n\n");
}



/*
   Scrive in SHM identificata da una tabella un vettore 
   di float ed un tappo conclusivo.
*/
static int write_shm(TAB_SHARED tab, float *valori)
{
char *punt;
float uno=1;
int i;
     
      punt = tab.ind;
      for(i=0;i<tab.num_punti;i++)
	{
	  CONVERTI_FLOAT_R(valori[i]);
	}
      memcpy(punt,valori,tab.num_punti*sizeof(float));
      punt += tab.num_punti*sizeof(float);
      memcpy(punt,&uno,sizeof(float));
}


static void handler_client_mmi(signum,code,scp)
   int signum, code;
   struct sigcontext *scp;
  {
   if( signum == SIGALRM )
     {
      signal(SIGALRM,handler_client_mmi);  /*   installazione sig_handler */
      return;
     }
  }

/*
Funzione che comunica all' MMI il path del simulatore 
ricevuto da demone_mmi con una msg_snd ed aspetta la
risposta da mmi (da utente)
Parametri:
	int id_coda: coda fra mmi e client_mmi
        char *path_sim: path trasmesso da simulatore
Return:
        int risultato della verifica
*/

static int verify_sim(int id_coda,char *path_sim)
{
int ris;  /* Risposta dell' mmi */

if(!CsPrepMsgSimID(id_coda,path_sim))
   {
   printf("ERROR:client_mmi:verify_sim: Impossible to send a msg to mmi \n");
   return 0;
   }


if(CsRcvMsgSimIDAck(id_coda,&ris)<0)
   {  
   printf("ERROR:client_mmi:verify_sim: Error waiting OK from SIMID from MMI\n");
   return 0; 
   }   
/*
Valuto la risposta dell' MMI elaborata con interfaccia utente 
*/
return ris;
}



