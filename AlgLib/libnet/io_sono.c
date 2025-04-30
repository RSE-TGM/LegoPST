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
static char SccsID[] = "@(#)io_sono.c	5.1\t11/7/95";
/*
   modulo io_sono.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)io_sono.c	5.1
*/
# include <stdio.h>
# include <string.h>
#include <unistd.h>
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


#define MAXHOSTNAMELEN 256

char *io_sono(void)
{
char ptr[MAXHOSTNAMELEN];

gethostname(ptr,MAXHOSTNAMELEN);

return(ptr);
}
