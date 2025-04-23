/**********************************************************************
*
*	C Source:		msgesist.c
*	Instance:		1
*	Description:	
*	%created_by:	famgr %
*	%date_created:	Tue Feb 25 12:59:20 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: msgesist.c-1 %  (%full_filespec: msgesist.c-1:csrc:1 %)";
#endif

# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <math.h>
# include <errno.h>
# include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
 
#include <sys/ipc.h>
 
#include <sys/msg.h>

int     msgesist (id)
int     id;
{
char *ind;
int i;

    if ((i = msgget (id, 0777 | IPC_CREAT | IPC_EXCL)) > 0)
    {
        /* la coda non esiste */
        if (msgctl (i, IPC_RMID, (struct msqid_ds *)0) < 0)
        {
            fprintf (stderr,"msgesist: codice errore %d\n", errno);
            perror ("shmctl");
        }
        return (0);
    }

    else
        return (1);
}

