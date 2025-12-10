/**********************************************************************
*
*       C Source:               table.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed Jun  7 12:28:28 2006 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: table.c-10 %  (%full_filespec: table.c-10:csrc:1 %)";
#endif

/* ***************************************************************** *
 * ***  ROUTINE PER IL CALCOLO DELLE PROPRIETA' TERMODINAMICHE   *** *
 * ***************************************************************** */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# include <errno.h>
# include <math.h>
# include "tavpar.h"           /* strutture common delle tavole      */

/* ************* DEFINIZIONE VARIABILI GLOBALI ********************* */

   extern struct B tabl_ ;
   extern CCO cont_ ;
   extern struct D init_ ;

   extern struct K
    {
     int id;
     int ip;
     float x[2];
     float y[2];
     int incr;
     float pae;
     float pai;
    } inte_ ;

  int diagn_io_();
int diagn_(int*,float*,float*);

   /* ************  DEFINIZIONE DELLA SUBROUTINE  ****************** */

void  table_()

   {
    int i,n;
    int ia,ifo,ireg,i1;
    float i1fl;
    int aaa;

    if ( (cont_.pd-tabl_.prt[20]) <= 0 )
      {
       ia=1;
       ifo=init_.kk-1;
      }
    else
      {
       ia=init_.kk;
       ifo=init_.klim;
      }
    aaa=0;
    for (ireg=ia; ireg<=ifo; ++ireg)
      {
       if ( (cont_.pd-tabl_.prt[ireg]) > 0 )   continue;
       inte_.pai=1./tabl_.pap[ireg-1];
       i1fl=((cont_.pd)-(tabl_.prt[ireg-1]))*inte_.pai;
       i1=i1fl+1;
       if ( (i1-init_.ii+2) >= 0 )  i1=init_.ii-2;
       inte_.x[0]=
           cont_.pd-tabl_.prt[ireg-1]-(i1-1)*tabl_.pap[ireg-1];
       inte_.x[1]=inte_.x[0]-tabl_.pap[ireg-1];
       aaa=1;
       break;
      }
    n=4;
    if (aaa==0)  
              {
                diagn_(&n,&cont_.sd,&cont_.pd);
/* guag 19-11-91 */
                cont_.pd=tabl_.prt[ireg-1];
/* guag */
              }
    init_.i0=ireg;
    inte_.ip=tabl_.ic[ireg-1]*6;
    i=0;
    if (ireg<init_.kk)  i=1;
    inte_.incr=(i1-1)*inte_.ip+6*i+tabl_.ioffset[ireg-1];

   }   /* FINE DELLA table_ */


   /* --------------- FINE del sorgente --------------------------- */
