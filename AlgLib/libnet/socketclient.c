/**********************************************************************
*
*       C Source:               socketclient.c
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Fri Aug 29 16:00:59 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: socketclient.c-2 %  (%full_filespec: socketclient.c-2:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)socketclient.c	5.1\t11/7/95";
/*
   modulo socketclient.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)socketclient.c	5.1
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include<unistd.h>
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

char   *io_sono ();


int     socketclient (macchina, server_port)
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

//    printf ("Inizio apertura socket in scrittura verso %s\n", macchina);
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
	    printf ("Fine apertura socket in scrittura verso %s porta %d id %d \n",
		    macchina, server_port, fp);
	    return (fp);
	}
    }
}
