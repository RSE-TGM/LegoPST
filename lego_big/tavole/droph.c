/**********************************************************************
*
*       C Source:               droph.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed Jun  7 12:26:16 2006 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: droph.c-10 %  (%full_filespec: droph.c-10:csrc:1 %)";
#endif

/* **************************************************************** *
 * ***  ROUTINE PER IL CALCOLO DELLE PROPRIETA' TERMODINAMICHE  *** *
 * **************************************************************** */

# include <stdio.h>
# include <errno.h>
# include <math.h>
# include "tavpar.h"           /* strutture common delle tavole     */

void  table_();
void  diagn_(int*,float*,float*);

/* ************* DEFINIZIONE VARIABILI GLOBALI ******************** */

   extern float (*valda)[];

    extern CCO cont_ ;

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


   /* ************  DEFINIZIONE DELLA SUBROUTINE  ***************** */

 void droph_(float *p,float *ncal,float *res1,float *res2,float *res3)
   {
    int i,in;
    int n;
    int aga=0;
    float infl;
    float t,a2,b,fx1,fx2;
    float aw,bw,vw;
    float drp[3],drh[3],ro[3];

    if ( (*p) > 221.2E5 )
      {
       n=5;
       diagn_(&n,&cont_.sd,p);
/* guag 19-11-91 */
       (*p)= 221.2E5;
/* guag */
       return;
      }
    if ( ((*p)-cont_.pd) == 0. )   aga=1;
    if (aga==0)
      {
       n=3;
       if ((*p)<610.) 
                  {
                   diagn_(&n,&cont_.sd,p);
/* guag 19-11-91 */
       (*p)= 610.;
/* guag */
                  }
       cont_.pd=(*p);
       cont_.sd=(-.1);
       table_();
      }
    if ( (*ncal-1) <= 0 )
      {
       in=inte_.incr+6;
       for (i=1; i<=3; ++i)
         {
          ro[i-1]=(*valda)[in+inte_.ip*(i-1)];
          t=(*valda)[in+inte_.ip*(i-1)+1];
          a2=(*valda)[in+inte_.ip*(i-1)+3];
          b=(*valda)[in+inte_.ip*(i-1)+4];
          drh[i-1]=b/t;
          drp[i-1]=1./a2-drh[i-1]/ro[i-1];
         }
      }
    if ( (*ncal-1) > 0 )
      {
       infl=inte_.incr+((*valda)[inte_.incr]+
                        (*valda)[inte_.incr+1]+1)*6;
       in=infl;
       for (i=1; i<=3; ++i)
         {
          vw=(*valda)[in+inte_.ip*(i-1)];
          t=(*valda)[in+inte_.ip*(i-1)+1];
          aw=(*valda)[in+inte_.ip*(i-1)+3];
          bw=(*valda)[in+inte_.ip*(i-1)+4];
          ro[i-1]=1./vw;
          drh[i-1]=bw/(vw*vw*t);
          drp[i-1]=aw/(vw*vw)-drh[i-1]*vw;
         }
      }
    fx1=(drp[1]-drp[0])*inte_.x[0]*inte_.pai;
    fx2=(drp[2]-drp[1])*inte_.x[0]*inte_.pai;
    *res1=drp[0]+fx1+(fx2-fx1)*inte_.x[1]*.5*inte_.pai;
    fx1=(drh[1]-drh[0])*inte_.x[0]*inte_.pai;
    fx2=(drh[2]-drh[1])*inte_.x[0]*inte_.pai;
    *res2=drh[0]+fx1+(fx2-fx1)*inte_.x[1]*.5*inte_.pai;
    fx1=(ro[1]-ro[0])*inte_.x[0]*inte_.pai;
    fx2=(ro[2]-ro[1])*inte_.x[0]*inte_.pai;
    *res3=ro[0]+fx1+(fx2-fx1)*inte_.x[1]*.5*inte_.pai;

   }   /* FINE DELLA droph_ */


   /* --------------- FINE del sorgente --------------------------- */
