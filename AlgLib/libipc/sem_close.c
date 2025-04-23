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
static char SccsID[] = "@(#)sem_close.c	5.1\t11/7/95";
/*
   modulo sem_close.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)sem_close.c	5.1
*/
#include <stdlib.h>

# include "libipc.h"

void sem_close(id)
int id;
{
int semval;

#if defined UNIX
if(semop(id,&op_close[0],3)<0)
	perror("semop impossibile");
if((semval=semctl(id,1,GETVAL,0))<0)
	perror("GETVAL impossibile");
if(semval>BIGCOUNT)
	{
	printf("Errore BIGCOUNT\n");
	exit(0);
	}
else if(semval==BIGCOUNT)
	sem_rm(id);
else if(semop(id,&op_unlock[0],1)<0)
	perror("unlock impossibile");
#endif
}

	
