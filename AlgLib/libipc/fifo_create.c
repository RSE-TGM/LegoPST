/**********************************************************************
*
*       C Source:               fifo_create.c
*       Subsystem:              3
*       Description:
*       %created_by:    ciccotel %
*       %date_created:  Mon Oct 21 12:13:34 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: fifo_create.c-1.1.1 %  (%full_filespec: fifo_create.c-1.1.1:csrc:3 %)";
#endif
/*
   modulo fifo_create.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)fifo_create.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)fifo_create.c	5.1\t11/7/95";
/*
        Fine sezione per SCCS
*/
#if defined UNIX
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <fcntl.h> 

int fifo_create(char *nome_fifo,int modo)
{
int ret;
int fd;
printf("apertura fifo %s\n",nome_fifo);

#ifdef LINUX
	ret=mkfifo(nome_fifo, S_IFIFO | 0777);
#else
	ret=mknod(nome_fifo, S_IFIFO | 0777);
#endif
	if((ret<0)&&(errno!=EEXIST))
		{
#ifdef LINUX
		perror("mkfifo");
#else
		perror("mknod");
#endif
		return(-1);
		}
	fd=open(nome_fifo,modo);
	if(fd<0)
		{
                perror("open");
                return(-1);
                }

printf("fine apertura fifo %s\n",nome_fifo);
	return(fd);
}
#endif
