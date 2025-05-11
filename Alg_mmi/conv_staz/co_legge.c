/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)co_legge.c	1.2\t6/16/93";
/*
   modulo co_legge.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)co_legge.c	1.2
*/

/*    SUBROUTINE LEGGE_RIGA(RIGA,LUN)
  Conversione f-c:	1991-05-09	Autore: C.Lusso (CISE)
*/
	/* rimuovere se inserita in compi.c */
#include <stdio.h>
#include <stdlib.h>
void lungh(char[],int *,int );

void legge_riga(
 char riga [],
 int *lun,
 int *nriga)
{
extern FILE *fp_s01;

if (! fgets( riga, 80, fp_s01)) exit( puts(
 "ATTENZIONE end_of_file inatteso del file R01.DAT"));
if (riga[0] == ' ') exit( puts(
 "ATTENZIONE il file S01 contiene una riga che inizia con blank"));
lungh( riga, lun, 80);
(*nriga)++;
} /* FINE "co_legge_riga.c" */
