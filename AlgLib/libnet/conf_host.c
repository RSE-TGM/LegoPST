/**********************************************************************
*
*       C Source:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: %  (%full_filespec: %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)conf_host.c	5.1\t11/7/95";
/*
   modulo conf_host.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)conf_host.c	5.1
*/
#include <stdio.h>
# include <string.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <arpa/inet.h>
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

extern int getindbyname(char*);

int conf_host(host1,host2)
char *host1,*host2;
{

if(getindbyname(host1)==getindbyname(host2))
		return(0);
	else
		return(1);
}
