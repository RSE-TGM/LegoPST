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
   modulo write_fifo.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)write_fifo.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)write_fifo.c	5.1\t11/7/95";
/*
        Fine sezione per SCCS
*/
#if defined UNIX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>


#define errore(stringa)         exit(fprintf(stderr,"Errore write_fifo pid(%d)[%s]: %s\n",getpid(),strerror(errno),stringa))


#define MAX_SIZE_FIFO  		4096
#define FINE_MSG		13

struct msg_fifo_st
        {
        int size;
        char dato[MAX_SIZE_FIFO-sizeof(int)-1];
        };
typedef struct msg_fifo_st MSG_FIFO;

MSG_FIFO msg_fifo;

int  writen(int,char*,int);

int write_fifo(int fd, char *dato, int size)
{
int size_tot;
int ret = -1;

	/* controllo lamassima dimensione del messaggio */
	if((size+sizeof(int)+1)>MAX_SIZE_FIFO)
		{
		errore("size maggiore del massimo consentito");
		return(-1);
		}

	/* riempimento struttura msg_fifo */
	msg_fifo.size=size;
	memcpy(&msg_fifo.dato[0],dato,size);
	msg_fifo.dato[size]=FINE_MSG;
	
	/* calcolo size totale del messaggio, 
		header + messaggio + terminatore */
	size_tot=size+sizeof(int)+1;

	/* invio messaggio  */
	ret=writen(fd,(char *)&msg_fifo,size_tot);
	if(ret!=size_tot)
		{
		errore(" chiamata write FIFO");
		return(-1);
		}
	else
		return(size);
	
}
#endif
