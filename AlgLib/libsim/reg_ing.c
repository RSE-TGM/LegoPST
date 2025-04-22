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
static char SccsID[] = "@(#)reg_ing.c	5.1\t11/7/95";
/*
   modulo reg_ing.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)reg_ing.c	5.1
*/
# include <stdio.h>
# include <string.h>
# include <Rt/RtDbPunti.h>
#include <Rt/RtMemory.h>

/* **************** Definizione variabili globali ****************** */

extern int kini;                            /* puntatore intero             */
extern RtDbPuntiOggetto dbpunti;

void reg_ing(uu,neqsis,nu,cnuu)

 int *neqsis,*nu;
 float (*uu)[],(*cnuu)[];

 {
  int i,kap;                      /* variabile spare                 */

  kap = kini + *neqsis;
  for( i = 1 ; i <= *nu ; i++ )
      (*uu)[i-1] = RtDbPGetValueD(dbpunti,kap+i-1) / (*cnuu)[i-1];

 }

/* FINE di reg_ing  */
