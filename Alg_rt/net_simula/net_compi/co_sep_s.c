/**********************************************************************
*
*       C Source:               co_sep_s.c
*       Subsystem:              1
*       Description:
*       %created_by:    mauric %
*       %date_created:  Tue Mar 10 10:59:23 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: co_sep_s.c-2 %  (%full_filespec: co_sep_s.c-2:csrc:1 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)co_sep_s.c	5.1\t11/7/95";
/*
   modulo co_sep_s.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)co_sep_s.c	5.1
*/

/*    SUBROUTINE SEPARA_STR(RIGA,LUN,NSTR,STRIN)
  Conversione f-c:        1991-05-09      Autore: C.Lusso (CISE)
*/
	/* rimuovere se inserita in compi.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
 char *stringa;
 int lun_stringa;
 } STRIN_ST;

int separa_str(
 char riga[],
 int lun,
 int nstr,
 STRIN_ST strin[])
{
char *s;
int i;
for (s=riga, i=0; i<nstr; i++) {
  s = strtok( s, " \t");
  if(s == (char*)NULL)
     {
/*
     printf("separa_str: realloc(3) indice = %d\n",i);
*/
     strin[i].stringa=(char *)realloc(strin[i].stringa,3);
     sprintf(strin[i].stringa," ");
     strin[i].lun_stringa = strlen(strin[i].stringa);
     }
  else
     {
     strin[i].lun_stringa = ( s ? strlen( s) : 0 );
     strin[i].stringa=(char*)realloc(strin[i].stringa,
                                     (strin[i].lun_stringa + 1));
     strcpy(strin[i].stringa, s);
/*
printf("separa_str: realloc  stringa = %s indice = %d\n",strin[i].stringa,i);
*/
     }
  s = (char*)NULL; }

} /* FINE "co_separa_str.c" */
