/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)stazgenc.c	1.2\t6/16/93";
/*
   modulo stazgenc.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)stazgenc.c	1.2
*/
#include "sim_param.h"
#include "sim_types.h"
#include "compstaz.inc"

// int staz_am1_c(int,int,int);
// int staz_am2_c(int,int,int);


/* stagenc.c : compila la stazione relativa al tipo */
int staz_gen_c(istaz,itipo,nmod)
int istaz, itipo, nmod;
{ 
switch (itipo)
{ 
   case    1: 
      staz_sa1_c(istaz,itipo,nmod);
   break;
   case    2: 
      staz_sp1_c(istaz,itipo,nmod);
   break;
   case    3: 
      staz_spd_c(istaz,itipo,nmod);
   break;
   case    4: 
      staz_id1_c(istaz,itipo,nmod);
   break;
   case    5: 
      staz_br1_c(istaz,itipo,nmod);
   break;
   case    6: 
      staz_tr1_c(istaz,itipo,nmod);
   break;
   case    7: 
      staz_mr1_c(istaz,itipo,nmod);
   break;
   case    8: 
      staz_lu1_c(istaz,itipo,nmod);
   break;
   case    9: 
      staz_am1_c(istaz,itipo,nmod);
   break;
   case   10: 
      staz_am2_c(istaz,itipo,nmod);
   break;
   case   11: 
      staz_am3_c(istaz,itipo,nmod);
   break;
   case   12: 
      staz_amd_c(istaz,itipo,nmod);
   break;
   case   13: 
      staz_sd1_c(istaz,itipo,nmod);
   break;
}
}
