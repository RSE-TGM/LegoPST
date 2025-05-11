/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)co_lungh.c	1.2\t6/16/93";
/*
   modulo co_lungh.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)co_lungh.c	1.2
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
