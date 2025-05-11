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
static char SccsID[] = "@(#)co_lungh.c	1.3\t3/23/95";
/*
   modulo co_lungh.c
   tipo 
   release 1.3
   data 3/23/95
   reserved @(#)co_lungh.c	1.3
*/

/*    SUBROUTINE LUNGH(RIGA,LUN,NCHAR)
  conversione f-c	1991-05-09	autore C.Lusso (CISE)
*/
        /* rimuovere se inserita in compi.c */
#include <string.h>

void lungh(
 char riga[],
 int *lun,
 int nchar)
{
*lun = strlen( riga);
riga[--*lun] = 0;
} /* FINE "co_lungh.c" */
