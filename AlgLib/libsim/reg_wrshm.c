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
static char SccsID[] = "@(#)reg_wrshm.c	5.1\t11/7/95";
/*
   modulo reg_wrshm.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)reg_wrshm.c	5.1
*/
# include <stdio.h>
# include <string.h>
# include <Rt/RtDbPunti.h>
#include <Rt/RtMemory.h>

/* **************** Definizione variabili globali ****************** */

extern int kini;                            /* puntatore intero             */
extern RtDbPuntiOggetto dbpunti;

void reg_wrshm(xy,uu,px,dati,neqsis,nu,neqdif,ndati,cnxy,cnuu)

 int *neqsis,*nu,*neqdif,*ndati;
 float (*xy)[],(*uu)[],(*px)[],(*dati)[],(*cnxy)[],(*cnuu)[];

 {
  int i,kap;                      /* variabile spare                 */

  kap = kini;
  for( i = 1 ; i <= *neqsis ; i++ )
      RtDbPPutValue(dbpunti,kap+i-1,(*xy)[i-1] * (*cnxy)[i-1]);

  kap = kap + *neqsis;
  for( i = 1 ; i <= *nu ; i++ )
      RtDbPPutValue(dbpunti,kap+i-1,(*uu)[i-1] * (*cnuu)[i-1]);

  kap = kap + *nu;
  for( i = 1 ; i <= *neqdif ; i++ )
      RtDbPPutValue(dbpunti,kap+i-1,(*px)[i-1]);

  kap = kap + *neqdif ;
  for( i = 1 ; i <= *ndati  ; i++ )
      RtDbPPutValue(dbpunti,kap+i-1,(*dati)[i-1]);
 }

/* FINE di reg_wrshm */
