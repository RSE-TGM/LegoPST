/**********************************************************************
*
*       C Source:               calsr.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed Jun  7 12:21:46 2006 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: calsr.c-10 %  (%full_filespec: calsr.c-10:csrc:1 %)";
#endif
/*
   modulo calsr.c
   tipo
   release 1.3
   data 2/8/96
   reserved @(#)calsr.c	1.3
*/
/* ***************************************************************** *
 * ***  ROUTINE PER IL CALCOLO DELLE PROPRIETA' TERMODINAMICHE   *** *
 * ***************************************************************** */

# include <stdio.h>
# include <errno.h>
# include <math.h>
# include "tavpar.h"           /* strutture common delle tavole      */


void newt_(int*,float*);
int diagn_io_();
int diagn_(int*,float*,float*);

/* ************* DEFINIZIONE VARIABILI GLOBALI ********************* */

   float (*valda)[];

   extern CCO cont_ ;        // inizializzato nel fortran
   extern struct D init_ ;   // inizializzato nel fortran

   struct K
    {
     int id;
     int ip;
     float x[2];
     float y[2];
     int incr;
     float pae;
     float pai;
    } inte_ ;

   struct L
    {
     int ipha;
     int in;
     float titre;
     int idumm;
    } ther_ ;


   /* ************  DEFINIZIONE DELLA SUBROUTINE  ****************** */

 void calsr_()

   {
    int i,j,n,new;
    int jz;
    float jfl,jzfl;
    double jzdb;
    int aab=0,aac=0;
    float slim[5]={0,0,0,0,0};

    ther_.titre=0.;
    ther_.ipha=0;
    for (i=1; i<=4; i+=3)
       {
         if ( (init_.i0-init_.kk) < 0 )
           {
               inte_.id=inte_.incr-6+i;
               new=0;
               newt_(&new,&slim[i-1]);
           }
         else  slim[i-1]=4442.86472;
         if ( (cont_.sd-slim[i-1]) <= 0 )
           {
             aab=1;
             break;
           }
       }
    if (aab==0)
      {
        if (cont_.sd > (*valda)[inte_.incr+6+2*inte_.ip-1])
           {
            n=2;
            diagn_(&n,&cont_.sd,&cont_.pd);
/* guag 19-11-91 */
            cont_.sd=(*valda)[inte_.incr+6+2*inte_.ip-1];
/* guag */
           }
        i=4;
        ther_.titre=1.;
      }
    if (i==4 && aab==1)
      {
        ther_.ipha=1;
        ther_.titre=(cont_.sd-slim[0])/(slim[3]-slim[0]);
        jz=2;
        j=2;
        for (i=1; i<=j; ++i)
           {
            jzfl=jz+(*valda)[inte_.incr+i-1];
            jz=jzfl;
           }
        ther_.in=inte_.incr+(jz-1)*6;
        return;
      }
    slim[i]=slim[i-1]+init_.s[i-1]*( (*valda)[inte_.incr+i-1]-1 );
    if ( (cont_.sd-slim[i]) < 0 )    jfl=2+0.67*(i-1);
    else                             jfl=1+1.34*(i-1);
    j=jfl;
    inte_.pae=1./init_.s[j-1];
    jzdb=fabs((double) ((cont_.sd-slim[j-1])*inte_.pae));
    jz=jzdb;
    aac=(*valda)[inte_.incr+j-1];
    if ( (jz+3) > aac )  jz--;
    inte_.y[0]=cont_.sd-slim[j-1]-jz*init_.s[j-1];
    inte_.y[1]=inte_.y[0]-init_.s[j-1];
    jz=jz+2;
    j--;
    if (j==0)
      {
        ther_.in=inte_.incr+(jz-1)*6;
        return;
      }
    for (i=1; i<=j; ++i)
      {
        jzfl=jz+(*valda)[inte_.incr+i-1];
        jz=jzfl;
      }
    ther_.in=inte_.incr+(jz-1)*6;

   }   /* FINE DELLA calsr_ */


   /* --------------- FINE del sorgente --------------------------- */
