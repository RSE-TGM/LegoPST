/**********************************************************************
*
*       C Source:               socketscrittura.c
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Fri Aug 29 16:03:04 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: socketscrittura.c-3 %  (%full_filespec: socketscrittura.c-3:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)socketscrittura.c	5.1\t11/7/95";
/*
   modulo socketscrittura.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)socketscrittura.c	5.1
*/
#include <stdio.h>
#include <stdlib.h>>
#include<unistd.h>
#include <string.h>
#include <errno.h>
#if defined UNIX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
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

char *io_sono();
extern int h_errno;


int socketscrittura(macchina,offset)
char *macchina;
int offset;
{
int fp;
struct sockaddr_in client;
struct hostent *hp; 
#if defined UNIX
struct hostent *gethostbyname();
#endif
char io[256];
short porta;
int tentativi=0;

/*
   verifica che l'host specificato in macchina sia conosciuto
*/
if(test_host(macchina)==0)
   {
   fprintf(stderr,"socketlettura: host <%s> sconosciuto\n",macchina);
   return(-1);
   }


// printf("Inizio apertura socket in scrittura verso %s\n",macchina);
while(1)
{
/* apertura socket */
fp = socket( AF_INET, SOCK_STREAM, 0);
        if (fp < 0)
        {
                perror("opening stream socket");
                exit(0);
        }

client.sin_family = AF_INET;
client.sin_addr.s_addr=INADDR_ANY;

hp = gethostbyname(macchina);
if (hp == 0)
    {
    printf("%s: unknown host h_errno= [%d]\n", macchina,h_errno);
    exit(0);
    }
strcpy(io,io_sono());
memcpy ((char *)&client.sin_addr,(char *)hp->h_addr, hp->h_length);
printf("\n getindbyname(%s)= %d", macchina,getindbyname(macchina));
printf("\n getindbyname(%s)= %d", io,getindbyname(io));
printf("\n offset = %d",offset);
printf("\n SOCKETSCRITTURA USA LA PORTA %d",(unsigned short)(READ_PORT+getindbyname(macchina)+offset+getindbyname(io)));
client.sin_port = htons((unsigned short)(READ_PORT+getindbyname(macchina)+offset+
		getindbyname(io)));
if(connect(fp,(struct sockaddr *) & client, sizeof(client))<0)
    {
    sleep(1);
    if(++tentativi==180)
	exit(printf("Connessione con %s fallita\n",macchina));
    }
else
	{
	if(writen(fp,(char*)&fp,sizeof(int))<0)
        {
        perror("socketscrittura : -->");
        exit(0);
        }
	printf("Fine apertura socket in scrittura verso %s porta %d id %d \n",
	   macchina,READ_PORT+getindbyname(macchina)+offset,fp);
	setta_dim_buffer(fp,5000);
	return(fp);
	}
}
}
