/**********************************************************************
*
*       C Source:               socketlettura.c
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Fri Aug 29 16:01:34 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: socketlettura.c-3 %  (%full_filespec: socketlettura.c-3:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)socketlettura.c	5.2\t3/8/96";
/*
   modulo socketlettura.c
   tipo 
   release 5.2
   data 3/8/96
   reserved @(#)socketlettura.c	5.2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#if defined UNIX
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h> 
#endif

#if defined VMS
# include <iodef.h>
#include <types.h>
#include <socket.h>
#include <in.h>
#include <netdb.h>
#include <inet.h>
#include <ucx$inetdef.h>
#endif

#include "sim_param.h"
#include "sim_types.h"
#include "comandi.h"

char asdfghj[50];

int connessione_fallita()
{
	exit(printf("Connessione con %s fallita \n",asdfghj));
}


char *io_sono();

int socketlettura(macchina,offset)
char *macchina;
int offset;
{
int fp;
int prova;
int qq;
struct sockaddr_in server;
char io[256];

/*
	verifica che l'host specificato in macchina sia conosciuto
*/
if(test_host(macchina)==0)
	{
	fprintf(stderr,"socketlettura: host <%s> sconosciuto\n",macchina);
	return(-1);
	}

strcpy(asdfghj,macchina);

// printf("Inizio apertura socket in lettura da %s\n",macchina);

/* apertura socket */
fp = socket( AF_INET, SOCK_STREAM, 0);
        if (fp < 0)
        {
                perror("opening stream socket");
                exit(0);
        }

server.sin_family = AF_INET;
server.sin_addr.s_addr = INADDR_ANY;
strcpy(io,io_sono());
server.sin_port = htons((unsigned short)(getindbyname(io)+READ_PORT+offset)+
		getindbyname(macchina));

if (bind(fp, (struct sockaddr*)&server, sizeof(server)) < 0)
        {
                perror("binding datagram socket");
                exit(0);
        }

#if defined UNIX
signal(SIGALRM,(__sighandler_t)connessione_fallita);
alarm(180);
#endif
listen(fp,5);

/*if((fp=accept(fp,(struct sockaddr *) 0,(int *) 0))<0)*/
qq=sizeof(server);
if((fp=accept(fp,(struct sockaddr * restrict)&server,&qq))<0)
        {
        perror("Errore accept");
        exit(0);
        }

#if defined UNIX
alarm(0);
#endif


if(readn(fp,(char*)&prova,sizeof(int))<0)
	{
	perror("socketlettura : -->");
	exit(0);
	}

printf("Fine apertura socket in lettura da %s porta %d id %d\n",macchina,
	getindbyname(io)+READ_PORT+offset,fp);
setta_dim_buffer(fp,5000);
return(fp);
}
