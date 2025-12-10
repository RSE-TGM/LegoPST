/**********************************************************************
*
*       C Source:               drsev.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed Jun  7 12:27:13 2006 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: drsev.c-10 %  (%full_filespec: drsev.c-10:csrc:1 %)";
#endif

/* **************************************************************** *
 * ***  ROUTINE PER IL CALCOLO DELLE PROPRIETA' TERMODINAMICHE  *** *
 * **************************************************************** */

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

# include <errno.h>
# include <math.h>
# include "tavpar.h"           /* strutture common delle tavole     */


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


    int diagn_(int*,float*,float*);
    void  table_();
    void newt_(int*,float*);

   /* ************  DEFINIZIONE DELLA SUBROUTINE  ***************** */

 void drsev_(float * p,int* ncal,float* res1,float* res2)
   {
    int i,in;
    int n,new;
    int ada=0;
    float infl;
    float t,a2,b,fx1,fx2;
    float drp[3],r[3];

    if ( (*p) > 221.2E5 )
      {
       n=5;
       diagn_(&n,&cont_.sd,p);
/* guag 19-11-91 */
                     (*p)= 221.2E5;
/* guag */
       return;
      }
    if ( ((*p)-cont_.pd) == 0. )   ada=1;
    if (ada==0)
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
    if ( (*ncal-2) < 0 )
      {
       infl=inte_.incr+((*valda)[inte_.incr]+
                            (*valda)[inte_.incr+1]+1)*6;
       in=infl;
       inte_.id=in+4;
       new=0;
       newt_(&new,res1);
       inte_.id=inte_.id+6;
       newt_(&new,res2);
       return;
      }
    if ( (*ncal-2) == 0 )
       in=inte_.incr+6;
    if ( (*ncal-2) > 0 )
      {
       infl=inte_.incr+((*valda)[inte_.incr]+
                            (*valda)[inte_.incr+1]+3)*6;
       in=infl;
      }
    for (i=1; i<=3; i++)
      {
       r[i-1]=(*valda)[in+inte_.ip*(i-1)];
       t=(*valda)[in+inte_.ip*(i-1)+1];
       a2=(*valda)[in+inte_.ip*(i-1)+3];
       b=(*valda)[in+inte_.ip*(i-1)+4];
       drp[i-1]=1./a2-b/(r[i-1]*t);
      }
    fx1=(drp[1]-drp[0])*inte_.x[0]*inte_.pai;
    fx2=(drp[2]-drp[1])*inte_.x[0]*inte_.pai;
    *res1=drp[0]+fx1+(fx2-fx1)*inte_.x[1]*.5*inte_.pai;
    fx1=(r[1]-r[0])*inte_.x[0]*inte_.pai;
    fx2=(r[2]-r[1])*inte_.x[0]*inte_.pai;
    *res2=r[0]+fx1+(fx2-fx1)*inte_.x[1]*.5*inte_.pai;

   }   /* FINE DELLA drsev_ */


   /* --------------- FINE del sorgente --------------------------- */
