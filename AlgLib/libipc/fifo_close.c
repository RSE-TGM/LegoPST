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
   modulo fifo_close.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)fifo_close.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)fifo_close.c	5.1\t11/7/95";
/*
        Fine sezione per SCCS
*/
#if defined UNIX
#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <errno.h>

#define errore(stringa)         fprintf(stderr,"Errore pid(%d)[%s]: %s\n",getpid(),strerror(errno),stringa)

int fifo_close(char *nome_fifo)
{
int ret;

	ret=unlink(nome_fifo);
	if(ret<0)
		errore("chiusura fifo");
	if(ret==0)
		return(1);
	else
		return(-1);	
}
#endif
