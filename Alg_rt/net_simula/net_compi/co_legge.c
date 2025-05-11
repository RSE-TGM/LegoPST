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
static char SccsID[] = "@(#)co_legge.c	5.1\t11/7/95";
/*
   modulo co_legge.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)co_legge.c	5.1
*/

/*    SUBROUTINE LEGGE_RIGA(RIGA,LUN)
      SUBROUTINE LUNGH(RIGA,LUN,NCHAR)
  Conversione f-c:	1991-05-09	Autore: C.Lusso (CISE)
  modif. riga_s01:	1991-07-24	Autore: C.Lusso (CISE)
*/
	/* rimuovere se inserita in compi.c */
#include <stdio.h>
#include <stdlib.h>

 extern FILE *fp_s01;
 extern int riga_s01;

#define MAXRIGA 85

void legge_riga(
 char riga [MAXRIGA],
 int *lun)
{
 int c, k;

 riga_s01++;
 for (k=0; k<MAXRIGA; k++) 
   riga[k] = (char)NULL;

 for (k=0; (c = fgetc( fp_s01)) != '\n'; k++) {
  if (c == EOF) {
   fprintf(stderr, "ATTENZIONE end_of_file inatteso del file S01\n");
   exit( 1);
  }
  if (k == MAXRIGA-1) {
   fprintf(stderr, "ATTENZIONE riga %d supera %d caratteri.\n", riga_s01, MAXRIGA);
   exit( 1);
  }
  riga[k]=c;
 }
 riga[k]=(char)NULL;

 if (k == 0) {
  fprintf( stderr,"ATTENZIONE nel file S01 la riga %d e' vuota\n", riga_s01);
  exit( 1);
 }
 if (riga[0] == ' ') {
  fprintf(stderr, "ATTENZIONE nel file S01 la riga %d inizia con blank\n", riga_s01);
  fprintf(stderr, "L'elaborazione continua!\n");
 }

 /*   CALL LUNGH(RIGA,LUN,NCHAR) superflua: */
 *lun = k;
} /* FINE "co_legge_riga.c" */
