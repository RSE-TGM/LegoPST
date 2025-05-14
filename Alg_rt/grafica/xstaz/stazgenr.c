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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/Separator.h>
#include "sim_param.h"
#include "xstaz.h"
#include "compstaz.h"

/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)stazgenr.c	1.4\t3/23/95";
/*
   modulo stazgenr.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)stazgenr.c	1.4
*/
/* stagenr.c :  esegue  stazione relativa al tipo */
void staz_gen_r(flag,itipo,istaz,indice)
int *flag, *itipo, *istaz, *indice;
{ 
switch (*itipo)
{ 
   case    1: 
      staz_sa1_r(flag,istaz,indice);
   break;
   case    2: 
      staz_sp1_r(flag,istaz,indice);
   break;
   case    3: 
      staz_spd_r(flag,istaz,indice);
   break;
   case    4: 
      staz_id1_r(flag,istaz,indice);
   break;
   case    5: 
      staz_br1_r(flag,istaz,indice);
   break;
   case    6: 
      staz_tr1_r(flag,istaz,indice);
   break;
   case    7: 
      staz_mr1_r(flag,istaz,indice);
   break;
   case    8: 
      staz_lu1_r(flag,istaz,indice);
   break;
   case    9: 
      staz_am1_r(flag,istaz,indice);
   break;
   case   10: 
      staz_am2_r(flag,istaz,indice);
   break;
   case   11: 
      staz_am3_r(flag,istaz,indice);
   break;
   case   12: 
      staz_amd_r(flag,istaz,indice);
   break;
   case   13: 
      staz_sd1_r(flag,istaz,indice);
   break;
}
}
