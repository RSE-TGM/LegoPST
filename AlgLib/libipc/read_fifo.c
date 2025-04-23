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
   modulo read_fifo.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)read_fifo.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)read_fifo.c	5.1\t11/7/95";
/*
        Fine sezione per SCCS
*/
#if defined UNIX
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>


#define MAX_SIZE_FIFO  		4096
#define FINE_MSG		13

#define errore(stringa)         fprintf(stderr,"Errore read_fifo pid(%d)[%s]: %s\n",getpid(),strerror(errno),stringa)

struct msg_fifo_st
        {
        int size;
        char dato[MAX_SIZE_FIFO-sizeof(int)-1];
        };
typedef struct msg_fifo_st MSG_FIFO;

MSG_FIFO msg_fifo;

int readn(int,char*,int);


int read_fifo(int fd, char *dato, int size)
{
int size_tot;
int ret = -1;

	/* controllo lamassima dimensione del messaggio */
	if((size+sizeof(int)+1)>MAX_SIZE_FIFO)
		{
		errore("size maggiore del massimo consentito");
		return(-1);
		}

	/* calcolo size totale del messaggio, 
		header + messaggio + terminatore */
	size_tot=size+sizeof(int)+1;

	/* lettura messaggio  */
	ret=readn(fd,(char*)&msg_fifo,size_tot);
	if(ret!=size_tot)
		{
		errore(" chiamata read sulla FIFO");
		return(-1);
		}

	/* test terminatore */
	if(msg_fifo.dato[msg_fifo.size]!=FINE_MSG)
		{
		errore(" terminatore non trovato");
		return(1);
		}

	/* copia il campo dato */
	memcpy(dato,&msg_fifo.dato[0],msg_fifo.size);

	return(msg_fifo.size);

}
#endif
