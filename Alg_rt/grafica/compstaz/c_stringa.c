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
static char SccsID[] = "@(#)c_stringa.c	1.3\t3/23/95";
/*
   modulo c_stringa.c
   tipo 
   release 1.3
   data 3/23/95
   reserved @(#)c_stringa.c	1.3
*/
/*
	Compilazione dell' oggetto STRINGA_DESCR
*/

#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>
#include "sim_param.h"
#include "sim_types.h"
#include "xstaz.h"
#include "compstaz.inc"

// void legge_riga( char *riga, int *lun, int *nriga );
// void separa_str( char *riga, int lun, int nstr, STRIN_ST strin[]);

extern  STRIN_ST string[];

extern  char riga [80];
extern  int nriga;

extern	char *scolori_oggetti[];


extern FILE *fo;

int c_stringa(p_r02)
TIPO_STRINGA_DESCR *p_r02;
{
int lun, nstr;
char *px, saveriga[80];


  p_r02->tipo_oggetto=STRINGA_DESCR;

/*
	legge la descrizione della stringa
*/
  legge_riga( riga, &lun, &nriga);
  strcpy(saveriga,riga); 
  separa_str( riga, lun, nstr=10, string);
  if (strncmp(string[0].stringa,s_etic,strlen(s_etic))) errore(ERR_ETIC,riga);
  memset(p_r02->etic,0,LUNG_ETICHETTA+1);
  if (string[1].lun_stringa) 
  {
  	px=strstr(saveriga,string[1].stringa);
	strncpy(p_r02->etic,px,LUNG_ETICHETTA);
  }
  fprintf(fo,"\n etichetta %s ",p_r02->etic);
}

