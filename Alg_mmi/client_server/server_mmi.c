/**********************************************************************
*
*       C Source:               server_mmi.c
*       Subsystem:              2
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Mon Nov  7 14:33:25 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: server_mmi.c-2.8 %  (%full_filespec: server_mmi.c-2.8:csrc:2 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)server_mmi.c	1.18\t4/12/96";
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>



#include "sim_param.h"
#include "sim_types.h"
#include "sim_ipc.h"
#include "comandi.h"
#include "demone_mmi.h"
#include "libipc.h"
#include "libnet.h"
#include <Cs/Cs.h>
#include <Rt/RtDbPunti.h>
#define MAXLUNHOST	256

#define errore(stringa)         exit(fprintf(stderr,"Attention !!!\n      Error in process server_mmi[%d]: %s\n",getpid(),stringa))
/*
#define close_connection()    exit(fprintf(stderr,"Message !!!\n      Process server_mmi[%d]: close connection\n",getpid()))
*/

static int  socketserver_mmi ();
static void main_mmi_server_command(int ,int );
static void main_mmi_server_data(int ,int );
static void main_mmi_server_pert(int );
static int scoda_nuova_operazione(int , ELENCO_OPERAZIONI *, int *,
                                  MSG_F22_MMI *);
static void refresh_pagina(ELENCO_OPERAZIONI * , int,int);
static void invio_wachdog(int,int);
static void close_connection(void);
static void finePIPE();
int tipo_server;
int prog_server;
int msg_daemon;

RtDbPuntiOggetto dbpunti;

static void close_connection(void)
{

printf("Message !!!\n      Process server_mmi[%d]: close connection\n",getpid());
exit(1);
}

int main(argc,argv)
int argc;
char **argv;
{
int fp;
SIMULATOR *simpar;
int i;
if(argc!=5)
        {   
        for(i=0;i,argc;i++)
           printf("ERROR argv[%d]=%s\n",i,argv[i]);
        errore("use: server_mmi  <hostname> <type> <prog_server> <msg key daemon>");
        }

tipo_server=atoi(argv[2]);
prog_server=atoi(argv[3]);
msg_daemon=atoi(argv[4]);
printf("server_mmi: num msg queue=%d \n",msg_daemon);
printf("server_mmi %s %d %d\n",argv[1],tipo_server,prog_server);
fp=socketserver_mmi(argv[1]);
if(fp<=0)
	errore("open connection");

simpar  = (SIMULATOR*)calloc(1,sizeof(SIMULATOR));
dbpunti = RtCreateDbPunti(NULL,NULL,DB_PUNTI_SHARED,simpar);
free(simpar);

switch(tipo_server){
        case SERVER_MMI_COMMAND:
                {
		printf("SERVER_MMI_COMMAND\n");
		main_mmi_server_command(fp,prog_server);
		break;
		}
        case SERVER_MMI_DATA:
                {
		printf("SERVER_MMI_DATA\n");
		main_mmi_server_data(fp,prog_server);
		break;
		}
        case SERVER_MMI_PERT:
                {
		printf("SERVER_MMI_PERT\n");
		main_mmi_server_pert(fp);
		break;
		}
	default:
		errore("type error");
	}

}


static int     socketserver_mmi (macchina)
char   *macchina;
{
int     fp;
int     prova;
int     qq;
struct sockaddr_in  server;
struct sockaddr_in sock_info;
int size_sockaddr=sizeof(sock_info);
int server_port;
int id_msg;
char nomeHost[MAXLUNHOST];
MSG_DEMONE_PORT messaggio_porta;


/* apertura socket */
    fp = socket (AF_INET, SOCK_STREAM, 0);
    if (fp < 0)
    {
	perror ("opening stream socket");
	exit (0);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    gethostname(nomeHost,MAXLUNHOST);
    server.sin_port = htons (0);
    /* attraverso la "getsockname" si viene a conoscenza del numero di porta
       che il sistema ha assegnato al socket */

    if (bind (fp, (struct sockaddr *) & server, sizeof (server)) < 0)
    {
        perror ("binding datagram socket");
	exit (0);
    }
if(getsockname(fp,(struct sockaddr * restrict)&sock_info,&size_sockaddr)!=0)
	errore("getsockname");

 /* si memorizza il numero di porta nella struttura del server */

    server_port=htons(sock_info.sin_port);
    
    /*
	invio al demone il numero di porta ricavato
    */
    id_msg=msg_create(msg_daemon,0);
    messaggio_porta.mtype=MSG_PORT_SERVER;
    messaggio_porta.port=server_port;

 /* invio al processo demone del numero di porta ricavato */

    msg_snd(id_msg,&messaggio_porta,sizeof(MSG_DEMONE_PORT)-sizeof(long),
		!IPC_NOWAIT);

    listen (fp, 5);

   /*  accetta la connessione da parte di un processo client */

    if ((fp = accept (fp, (struct sockaddr * restrict)&server, &qq)) < 0)
    {
	perror ("Errore accept");
	exit (0);
    }

    return (fp);
}

static void main_mmi_server_command(int fp,int prog)
{
char *buffer;
int id_msg;
int size;
int ret;
int i,j;
int npacch;
TOKEN *tok;
HEADER_NEW_PAGE_MMI hd;
MSG_HDR msg_hdr;
MSG_TOK msg_tok;
MSG_F22_MMI msg_f22;
MSG_CHANGE_POINT_MMI msg_point;



id_msg = msg_create(SERVER_MMI_MSG+prog,0);

while(1)
	{
/*      
        Lettura dell'header del messaggio inviato in rete
        dal client_command al server_command e switch sulla 
        richiesta ricevuta. 
*/
	ret = readn(fp,(char *)&hd,sizeof(HEADER_NEW_PAGE_MMI));
	/* converto l'header  */
	CONVERTI_INT_R(hd.id_shm);
	CONVERTI_INT_R(hd.t_agg);
	CONVERTI_INT_R(hd.num_punti);
	CONVERTI_INT_R(hd.size);
	CONVERTI_INT_R(hd.richiesta_mmi);
printf("server_command: id_shm=%d t_agg=%d num_punti=%d size=%d ric=%d\n",
	hd.id_shm,hd.t_agg,hd.num_punti,hd.size,hd.richiesta_mmi);
        if(ret<=0)
		{
		msg_close(id_msg);
		close_connection();
		}
/*
Se read restituisce 0 cioe' se il numero di byte letti e' inferiore a quelli
da leggere, chiudi connessione
*/
        if(ret!=sizeof(HEADER_NEW_PAGE_MMI))
		{
		msg_close(id_msg);
		close_connection();
		}
        msg_hdr.mtype = SERVER_COMMAND_DATA;
        memcpy(&msg_hdr.header,&hd,sizeof(HEADER_NEW_PAGE_MMI));
        size = hd.size;
        switch(hd.richiesta_mmi)
           {
           case NEW_PAGE_MMI:
                {
/*              Vengono spediti al server_data un messaggio con un header
                ed un messaggio con i token ricevuti dal client */
                msg_hdr.operazione = NEW_PAGE_MMI;
                if(msg_snd(id_msg,&msg_hdr,sizeof(MSG_HDR)-sizeof(long),
                        !IPC_NOWAIT)<0)
				errore("msg_snd: messaggio NEW_PAGE_MMI");
                if(size>0)
                   {
/*                 Lettura pacchetti e controllo sulle dimensioni */
                   tok = (TOKEN *)malloc(size);
                   ret = readn(fp,(char *)tok,size);
        	   if(ret<=0)
		           {		
			   msg_close(id_msg);
			   close_connection();
			   }
		   if(ret!=size)  /* read bytes must be = size */
			   {
                           msg_close(id_msg);
                           close_connection();
                           }
		   for(i=0;i<(size/sizeof(TOKEN));i++)
			{
			CONVERTI_INT_R(tok[i].tipo);
			CONVERTI_INT_R(tok[i].puntdb);
			}
                   npacch = size/(MAX_TOKEN*sizeof(TOKEN));
                   if( ( size%(MAX_TOKEN*sizeof(TOKEN)) )>0 )
                      printf("ERRORE  size=%d  sizeof(TOKEN)=%d  npacch=%d\n",
                             size,sizeof(TOKEN),npacch); 
                   msg_tok.mtype = SERVER_COMMAND_DATA;
                   for(i=0;i<npacch;i++)
                      {
                      memcpy(msg_tok.token,&tok[i*MAX_TOKEN],MAX_TOKEN*sizeof(TOKEN));
                      if(msg_snd(id_msg,&msg_tok,MAX_TOKEN*sizeof(TOKEN),
                              !IPC_NOWAIT)<0)
				errore("msg_snd: messaggio dati NEW_PAGE_MMI");
                      }
                   }
                break;
                }
           case DEL_PAGE_MMI:
                {
                printf("           SERVER COMMAND Cancellazione pagina\n");
                msg_hdr.operazione = DEL_PAGE_MMI;
                msg_snd(id_msg,&msg_hdr,sizeof(MSG_HDR)-sizeof(long),
                        !IPC_NOWAIT);
                break;
                }
           case CHANGE_POINT_MMI:
                {
                ret = readn(fp,(char *)&(msg_point.posizione),sizeof(int));
                ret = readn(fp,(char *)&(msg_point.indirizzo),sizeof(int));
                ret = readn(fp,(char *)&(msg_point.tipo),sizeof(int));
		CONVERTI_INT_R(msg_point.posizione);
		CONVERTI_INT_R(msg_point.indirizzo);
		CONVERTI_INT_R(msg_point.tipo);
		msg_point.mtype=SERVER_COMMAND_DATA;
		msg_point.id_shm=hd.id_shm;
                msg_hdr.operazione = CHANGE_POINT_MMI;
                msg_snd(id_msg,&msg_hdr,sizeof(MSG_HDR)-sizeof(long),
                        !IPC_NOWAIT);
                msg_snd(id_msg,&msg_point,sizeof(MSG_CHANGE_POINT_MMI)-sizeof(long),
                        !IPC_NOWAIT);
                break;
                }
           case F22_MMI:
                {
                ret = readn(fp,(char *)&msg_f22,sizeof(MSG_F22_MMI));
        	if(ret<=0)
		   {
		   msg_close(id_msg);
		   close_connection();
		   }
		if(ret!=sizeof(MSG_F22_MMI))
		  {
		  msg_close(id_msg);
                  close_connection();
                  }

                msg_hdr.operazione = F22_MMI;
                msg_snd(id_msg,&msg_hdr,sizeof(MSG_HDR)-sizeof(long),
                        !IPC_NOWAIT);
                msg_f22.mtype = SERVER_COMMAND_DATA;
                msg_snd(id_msg,&msg_f22,sizeof(MSG_F22_MMI)-sizeof(long),
                        !IPC_NOWAIT);
                break;
                }
           default:
		printf("\n\tserver_command: Commando non riconosciuto\n");	
		break;
		
           }/* Fine switch */        
	}  
}

void finePIPE(dummy)
int dummy;
{
 pid_t pid_def=0;   /* id del processo defunct */
 int coderr;        /* codice d'uscita */

    printf("SEGNALE SIGPIPE nel server  !!!\n");
    exit(1);
}


static void main_mmi_server_data(int fp, int prog)
{
ELENCO_OPERAZIONI operazioni[MAX_OPERAZIONI];
int i;
int id_msg;
int ind_del_shm;
int shr_usr_key;
int comando;
/***
extern float (*var)[];
***/
HEADER_DATA_DATA hd_data;
static int prima_volta=1;
struct stat *info_file;
int size_f22= -1;
FILE *fp22;
char *buf_f22;
MSG_F22_MMI msg_f22;
int invio_dati = 0;


float *val;

if(signal(SIGPIPE,finePIPE)==SIG_ERR)
	{
	printf("\n\tERRORE nel SIGPIPE\n");
	}
id_msg = msg_create(SERVER_MMI_MSG + prog,0);
shr_usr_key = atoi((char *)getenv("SHR_USR_KEY"));
/***
sim_shraddr(shr_usr_key);
***/


for(i=0;i<MAX_OPERAZIONI;i++)
	operazioni[i].stato = -1;
while(1)
    {
    invio_dati = 0;
    comando = scoda_nuova_operazione(id_msg,&operazioni[0],
                                     &ind_del_shm,&msg_f22);
    switch(comando)
       {
       case REF_PAGE_MMI:
          {
	  for(i=0;i<MAX_OPERAZIONI;i++)
	     if(operazioni[i].stato!=-1)
                {
                -- operazioni[i].cont;
                if(operazioni[i].cont==0)
                   {
                   operazioni[i].cont = operazioni[i].t_agg;
                   refresh_pagina(&operazioni[i],fp,id_msg);
		   invio_dati = 1;
                   }
                }
          sospendi(100);
          break;
          }
       case NEW_PAGE_MMI:
          {
	  for(i=0;i<MAX_OPERAZIONI;i++)
	     if(operazioni[i].stato!=-1)
                {
                -- operazioni[i].cont;
                if(operazioni[i].cont==0)
                   {
                   operazioni[i].cont = operazioni[i].t_agg;
                   refresh_pagina(&operazioni[i],fp,id_msg);
		   invio_dati = 1;
                   }
                }
          sospendi(100);
          break;
          }
       case DEL_PAGE_MMI:
          {
          hd_data.tipo_operazione = CLT_CMDT_DELPAGE;
          hd_data.id_shm    = ind_del_shm;
          hd_data.num_punti = 0;
	  CONVERTI_INT_T(hd_data.tipo_operazione);
	  CONVERTI_INT_T(hd_data.id_shm);
	  CONVERTI_INT_T(hd_data.num_punti);
          if(writen(fp,(char *)&hd_data,sizeof(HEADER_DATA_DATA))<=0)
		{
		msg_close(id_msg);
		close_connection();
		}
          break;
          }
       case F22_MMI:
          {
/*        Leggo le dimensioni del file path_server */
          info_file = (struct stat *) malloc (sizeof (struct stat));
          stat(msg_f22.path_server,info_file);
          size_f22 = (int)info_file->st_size;
          free(info_file);
/*        Leggo e trasmetto il file */
          hd_data.tipo_operazione = CLT_CMDT_F22PAGE;
          hd_data.id_shm    = 0;
          hd_data.num_punti = size_f22;
          buf_f22 = (char *)malloc(size_f22);
          fp22 = fopen("f22","r");
          fread(buf_f22,size_f22,1,fp22);
          fclose(fp22);
          printf("*********************\n");
          printf("size_f22=%d\n",size_f22);
          printf("*********************\n");
          if(writen(fp,(char *)&hd_data,sizeof(HEADER_DATA_DATA))<=0)
		{
		msg_close(id_msg);
		close_connection();
		}
          if(writen(fp,buf_f22,size_f22)<=0)
		{
		msg_close(id_msg);
		close_connection();
		}
          if(buf_f22!=NULL)
              free(buf_f22);
          break;
          }
       default :
          {
          printf("mmi_server_data , caso non contemplato=%d\n",
                 comando);
          break;
          }
       }  /* Fine switch */
/*  Test lettura tabella */
/*
    printf("================================================\n");
    printf("                   ELENCO OPERAZIONI\n");
    for(i=0;i<MAX_OPERAZIONI;i++)
       {
       if(operazioni[i].stato!=-1)
          printf("%d) stato=%d   id_shm=%d\n",i,
                 operazioni[i].stato,operazioni[i].id_shm);
       }
    printf("================================================\n\n");
*/
    /*
	se non sono stati inviati dati invio un segnale di wachdog
	per verificare la connessione  
    */
    if(invio_dati == 0)
	invio_wachdog(fp,id_msg);
    }
}

static void main_mmi_server_pert(int fp)
{
float a=5.12;
MSG_PERT messaggio_pert;
int shr_usr_key,id_msg_pert;
int ret;

shr_usr_key = atoi((char *)getenv("SHR_USR_KEY"));
id_msg_pert = msg_create(shr_usr_key+ID_MSG_PERT,0);

while(1)
	{
	/* riceve le perturbazioni dalla rete  */
	ret=readn(fp,(char *)&(messaggio_pert.perturbazione),sizeof(TIPO_PERT));
	if(ret!=sizeof(TIPO_PERT))
		close_connection();

        CONVERTI_INT_R(messaggio_pert.perturbazione.posizione);
        CONVERTI_INT_R(messaggio_pert.perturbazione.tipo);
        CONVERTI_INT_R(messaggio_pert.perturbazione.indirizzo);
        CONVERTI_INT_R(messaggio_pert.perturbazione.esaminata);
        CONVERTI_FLOAT_R(messaggio_pert.perturbazione.valore_delta);
        CONVERTI_FLOAT_R(messaggio_pert.perturbazione.rateo_durata);
        CONVERTI_FLOAT_R(messaggio_pert.perturbazione.t);
        CONVERTI_FLOAT_R(messaggio_pert.perturbazione.period);
        CONVERTI_FLOAT_R(messaggio_pert.perturbazione.phase);
        CONVERTI_FLOAT_R(messaggio_pert.perturbazione.meanvalue);
        CONVERTI_FLOAT_R(messaggio_pert.perturbazione.wide);
        CONVERTI_FLOAT_R(messaggio_pert.perturbazione.t_null_var);

	messaggio_pert.mtype=messaggio_pert.perturbazione.tipo;
	msg_snd(id_msg_pert,&messaggio_pert,sizeof(TIPO_PERT),!IPC_NOWAIT);

	}
}

static int scoda_nuova_operazione(int id_msg, ELENCO_OPERAZIONI *operazione,
                                  int *id_del_shm, 
                                  MSG_F22_MMI *msg_f22)
{
int ret;
int i;
int nattese;
int nop=0;
MSG_HDR pacch_hdr;
MSG_TOK pacch_tok;
MSG_CHANGE_POINT_MMI msg_point;


/*  Attesa header */
    ret = msg_rcv(id_msg,&pacch_hdr,sizeof(MSG_HDR)-sizeof(long),
                  (long)SERVER_COMMAND_DATA,IPC_NOWAIT,TIMEOUT_INFINITO);

    if(ret<=0)
      {
      pacch_hdr.operazione = REF_PAGE_MMI;
      return(pacch_hdr.operazione);
      }
    else
      {
      nattese = pacch_hdr.header.size/(MAX_TOKEN*sizeof(TOKEN));
      if( ( pacch_hdr.header.size%(MAX_TOKEN*sizeof(TOKEN)) )>0 )
         printf("ERRORE (scoda_nuova_operazione) size non multipla MAX_TOKEN\n"); 
      }

    switch(pacch_hdr.operazione)
       {
       case NEW_PAGE_MMI:
          {
printf("server_mmi: NEW_PAGE_MMI nattese=%d id_shm=%d\n", nattese, pacch_hdr.header.id_shm);
/*        Controllo del numero della operazione */
          while( (operazione[nop].stato!=-1) )
             {
             nop++;
             if (nop==MAX_OPERAZIONI)
                {
                printf("ERRORE : superato MAX_OPERAZIONI\n");
                break;
                }
             }
/*        Caricamento struttura operazione */
          operazione[nop].stato  = 2;
          operazione[nop].id_shm = pacch_hdr.header.id_shm;
          operazione[nop].t_agg  = pacch_hdr.header.t_agg;
          operazione[nop].cont   = pacch_hdr.header.t_agg;
          operazione[nop].num_punti = pacch_hdr.header.num_punti;
/*        Attesa dei token */
          operazione[nop].token = (TOKEN *)calloc(nattese,
                                                  MAX_TOKEN*sizeof(TOKEN));
          for(i=0;i<nattese;i++)
             {
             msg_rcv(id_msg,&pacch_tok,MAX_TOKEN*sizeof(TOKEN),
                     (long)SERVER_COMMAND_DATA,!IPC_NOWAIT,TIMEOUT_INFINITO);
             memcpy(&operazione[nop].token[i*MAX_TOKEN],
                    pacch_tok.token,MAX_TOKEN*sizeof(TOKEN));
             printf("Ricevuto pacchetto n.%d  di  %d totali operazione=%d (token[0]=%d)\n",
                     i,nattese, nop, operazione[nop].token[i*MAX_TOKEN+0].puntdb);
             }
          break;
          }
       case DEL_PAGE_MMI:
          {
          while( (operazione[nop].id_shm != pacch_hdr.header.id_shm)/*&&
                 (operazione[nop].stato != -1)*/ )
             {
             nop++;
             if (nop==MAX_OPERAZIONI)
                {
                printf("ERRORE : manca la SHM <%d> in tabella\n",
                        pacch_hdr.header.id_shm);
                break;
                }
             }
/*        Elimino l'operazione dalla tabella */
printf("server_mmi: DEL_PAGE_MMI id_shm=%d\n",  pacch_hdr.header.id_shm);
          operazione[nop].stato = -1;
          operazione[nop].id_shm = 0;
          (*id_del_shm) = pacch_hdr.header.id_shm;
          break;
          }
       case CHANGE_POINT_MMI:
          {
          while( (operazione[nop].id_shm != pacch_hdr.header.id_shm))
             {
             nop++;
             if (nop==MAX_OPERAZIONI)
                {
                printf("ERRORE : manca la SHM <%d> in tabella\n",
                        pacch_hdr.header.id_shm);
                break;
                }
             }
	  printf("server_mmi_data: CHANGE_POINT_MMI attendo 2^ mess\n");
          ret = msg_rcv(id_msg,&msg_point,sizeof(MSG_CHANGE_POINT_MMI)
		-sizeof(long),
                        (long)SERVER_COMMAND_DATA,!IPC_NOWAIT,TIMEOUT_INFINITO);
	  printf("server_mmi_data trovata tabelle %d pos %d ind %d\n",
			nop,msg_point.posizione,msg_point.indirizzo);
	  pacch_hdr.operazione=REF_PAGE_MMI;
	  /*
		modifico l'indirizzo del token selezionato
	  */
	  operazione[nop].token[msg_point.posizione].puntdb=msg_point.indirizzo;
	  operazione[nop].token[msg_point.posizione].tipo=msg_point.tipo;
          break;
	  }
       case F22_MMI:
          {
/*        Lettura del file f22 */
          ret = msg_rcv(id_msg,msg_f22,sizeof(MSG_F22_MMI)-sizeof(long),
                        (long)SERVER_COMMAND_DATA,!IPC_NOWAIT,TIMEOUT_INFINITO);
          break;
          }
       default :
          {
          printf("scoda_nuova operazione, caso non contemplato=%d\n",
                  pacch_hdr.operazione);
          break;
          }
       }    /*  Fine switch */
    return(pacch_hdr.operazione);
}



/*
    Leggo dalla SHM i float e li invio sulla rete
*/
static void refresh_pagina(ELENCO_OPERAZIONI *oprz, int fprete, int id_msg)
{
/**
extern float (*var)[];
**/
int i;
int size;
int nbyte_written;
float zero=0;
char *punt;
char *buff=NULL;
float *app_float;
HEADER_DATA_DATA hd_data;



/* Allocazione buffer di speizione */
   size = sizeof(HEADER_DATA_DATA) + oprz->num_punti*sizeof(float);
   buff = (char *)malloc(size);
   punt = buff;

/* Scrittura in rete dell'header trasmissione fra processi DATA */
   if(oprz->stato == 2)
      {
      oprz->stato = 1;
      hd_data.tipo_operazione = CLT_CMDT_NEWPAGE;
      }
   else
       hd_data.tipo_operazione = CLT_CMDT_REFPAGE;
   hd_data.id_shm    = oprz->id_shm;
   hd_data.num_punti = oprz->num_punti;
   CONVERTI_INT_T(hd_data.tipo_operazione);
   CONVERTI_INT_T(hd_data.id_shm);
   CONVERTI_INT_T(hd_data.num_punti);
   memcpy(punt,&hd_data,sizeof(HEADER_DATA_DATA));
   punt += sizeof(HEADER_DATA_DATA);

/* Scrittura in rete dell'elenco di float letti da SHM */
   for(i=0;i< oprz->num_punti;i++)
      {
      if(oprz->token[i].puntdb!=-1)
         {
/**** MODIFICA_QQ
         memcpy(punt, &(*var)[oprz->token[i].puntdb], sizeof(float));
****/
	if(oprz->token[i].puntdb >= 0)
		{
	 	RtDbPGetValue(dbpunti,oprz->token[i].puntdb,(float*)punt);
		}
	else
		{
		switch(oprz->token[i].puntdb){
        		case REQ_TIME:
                		{
               			memset(punt,0,sizeof(float));
                		RtDbPGetTime(dbpunti,(float*)punt);
                		break;
                		}
        		case REQ_STATUS:
                		{
                		memset(punt,0,sizeof(int));
                		RtDbPGetStato(dbpunti,(int*)punt);
                		break;
                		}
        		case -99999:
                		{
               			memset(punt,0,sizeof(float));
                		RtDbPGetTime(dbpunti,(float*)punt);
                		break;
                		}
        		case -88888:
                		{
               			memset(punt,0,sizeof(float));
                		RtDbPGetTime(dbpunti,(float*)punt);
                		break;
                		}
        		default:
                		errore("type error");
        		}

		}
	 
	 app_float = (float*)punt;
	 CONVERTI_FLOAT_T(*app_float);
/*
         printf("Il TOKEN n.%d (ind=%d) vale=%f\n",
                 i,oprz->token[i].puntdb,(*var)[oprz->token[i].puntdb]);
*/
         }
      else
         {
         memcpy(punt, &zero, sizeof(float));
	 app_float = (float*)punt;
	 CONVERTI_FLOAT_T(*app_float);
/*
         printf("Il TOKEN n.%d (ind=%d) vale zero\n",
                 i,oprz->token[i].puntdb);
*/
         }
      punt += sizeof(float);
      }
   if((nbyte_written=writen(fprete,buff,size))<=0)
	{
	printf("server_mmi: ERRORE in scrittura !!!<0\n");
	msg_close(id_msg);
	close_connection();
	}
   if(nbyte_written!=size)
	{
	printf("server_mmi: ERRORE in scrittura !!!>0\n");
	msg_close(id_msg);
	close_connection();
	}

   if (buff!=NULL)
      free(buff);

   /*printf("Fine refresh_pagina \n");*/
}

static void  invio_wachdog(fp,id_msg)
int fp;
int id_msg;
{
HEADER_DATA_DATA hd_data;

   hd_data.tipo_operazione = CLT_CMDT_WATCHDOG;
   hd_data.id_shm    = -1;
   hd_data.num_punti = 0;
  
   CONVERTI_INT_T(hd_data.tipo_operazione);
   CONVERTI_INT_T(hd_data.id_shm);
   CONVERTI_INT_T(hd_data.num_punti);

   if(writen(fp,(char *)&hd_data,sizeof(HEADER_DATA_DATA))<=0)
	{
        msg_close(id_msg);
printf("\n\nCLOSE CONN 4 \n\n ");
	close_connection();
	}
}
