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
   modulo rw_net.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)rw_net.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)rw_net.c	5.1\t11/7/95";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <unistd.h>
#if defined HELIOS
#include <posix.h>
#include <unistd.h>
#endif
#include "libnet.h"

int readn(fd,ptr,nbytes)
int fd;
char *ptr;
int nbytes;
{
int nleft,nread;
nleft=nbytes;
while(nleft>0)
	{
	if(nleft>MAX_DIM_BUFFER_RETE)
		nread=read(fd,ptr,MAX_DIM_BUFFER_RETE);
	else
		nread=read(fd,ptr,nleft);
	if(nread<0)
		return(nread);
	else if (nread==0)
		break;
	nleft-=nread;
	ptr+=nread;
	}
return(nbytes-nleft);
}

int writen(fd,ptr,nbytes)
int fd;
char *ptr;
int nbytes;
{
int nleft,nwrite;
nleft=nbytes;
while(nleft>0)
        {
	if(nleft>MAX_DIM_BUFFER_RETE)
        	nwrite=write(fd,ptr,MAX_DIM_BUFFER_RETE);
	else
        	nwrite=write(fd,ptr,nleft);
        if(nwrite<=0)
                return(nwrite);
        nleft-=nwrite;
        ptr+=nwrite;
        }
return(nbytes-nleft);
}
