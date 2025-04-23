/**********************************************************************
*
*       C Source:               do_demone.c
*       Subsystem:              3
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed Jan 22 11:36:03 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: do_demone.c-3 %  (%full_filespec: do_demone.c-3:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)do_demone.c	5.1\t11/7/95";
/*
   modulo do_demone.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)do_demone.c	5.1
*/
#if defined UNIX
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#if defined VMS
#include <types.h>
#include <socket.h>
#include <in.h>
#include <ucx$inetdef.h>
#endif
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include "sim_param.h"
#include "sim_types.h"
#include "sim_param.h"
#include "demone.h"
#include "libnet.h"

      

void do_demone(int tipo_demone, char *server_host,  char *client_host, 
          int tipo, char *riga_comando_1, char *riga_comando_2, 
          HEADER_REGISTRAZIONI *par)
{
int socd;
struct sockaddr_in client, server;
struct hostent *hp;
#if defined UNIX
struct hostent *gethostbyname();
#endif
short port;
int ordine;
MSG_DEMONE messaggio;
int i;
	

	memcpy(&messaggio.tipo,converti_int(&tipo,TRASMISSIONE),sizeof(int));
	printf("DO_DEMONE --> messaggio tipo %d\n",tipo);
	strcpy(messaggio.comando_1,riga_comando_1);
	strcpy(messaggio.comando_2,riga_comando_2);
	strcpy(messaggio.host,client_host);

/*         Preparazione dei dati da inviare all'header */
           memcpy(&messaggio.simul.max_snap_shot,
                  converti_int(&par->simulator.max_snap_shot,TRASMISSIONE),
                  sizeof(int));
           memcpy(&messaggio.simul.max_back_track,
                  converti_int(&par->simulator.max_back_track,TRASMISSIONE),
                  sizeof(int));
           memcpy(&messaggio.simul.max_campioni,
                  converti_int(&par->simulator.max_campioni,TRASMISSIONE),
                  sizeof(int));
           memcpy(&messaggio.simul.num_var,
                  converti_int(&par->simulator.num_var,TRASMISSIONE),
                  sizeof(int));
           memcpy(&messaggio.simul.max_pertur,
                  converti_int(&par->simulator.max_pertur,TRASMISSIONE),
                  sizeof(int));
           memcpy(&messaggio.simul.spare_snap,
                  converti_int(&par->simulator.spare_snap,TRASMISSIONE),
                  sizeof(int));
           memcpy(&messaggio.simul.pert_clear,
                  converti_int(&par->simulator.pert_clear,TRASMISSIONE),
                  sizeof(int));

	memset((char*)&server,0,sizeof(server));
	hp = gethostbyname(server_host);
	if (hp == 0)
		{
		fprintf(stderr, "%s: unknown host\n", server_host);
	        exit(2);
		}
	memcpy((char *)&server.sin_addr,(char *)hp->h_addr,hp->h_length);
	server.sin_family = AF_INET;
	port = SERVER_PORT+tipo_demone;
	server.sin_port = htons(port);

	socd = socket( AF_INET, SOCK_DGRAM, 0);
	if (socd < 0)
	{
		perror("opening datagram socket");
		exit(1);
	}


	
	memset((char*)&client,0,sizeof(client));
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = htonl(INADDR_ANY);
	client.sin_port = htons(0);

	if (bind(socd, (struct sockaddr*)&client, sizeof(client)) < 0)
	{

		perror("binding datagram socket");
		exit(1);
        }


        if (sendto(socd, &messaggio, sizeof(MSG_DEMONE),0, 
				(const struct sockaddr *)&server, sizeof (server))<0)
		perror("writing on datagram socket");
}
