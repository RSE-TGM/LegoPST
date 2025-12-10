/**********************************************************************
*
*       C Source:               cerca.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed Jun  7 12:22:55 2006 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: cerca.c-10 %  (%full_filespec: cerca.c-10:csrc:1 %)";
#endif

/* ***************************************************************** *
 * ***  ROUTINE PER IL CALCOLO DELLE PROPRIETA' TERMODINAMICHE   *** *
 * ***************************************************************** */

# include <stdio.h>
# include <errno.h>
# include <math.h>
# include "tavpar.h"           /* strutture common delle tavole      */

void newt_(int* ,float *);
void cerca_(float* ,float* ,int* );
int diagn_io_();
int diagn_(int*,float*,float*);

/* ************* DEFINIZIONE VARIABILI GLOBALI ********************* */

   extern float (*valda)[];

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

   extern struct L
    {
     int ipha;
     int in;
     float titre;
     int idumm;
    } ther_ ;


   /* ************  DEFINIZIONE DELLA SUBROUTINE  ****************** */

 void cerca_(float* hd,float* s0,int* non)
   {
    int i,icic,j,k,jj,kj,is,incr4;
    float isfl,idfl,infl,nnpfl,incr4fl;
    int new,n;
    int aca=0,acb=0,acc=0,acd=0;
    float hint;
    int nnp[5];
    float slim[5]={0,0,0,0,0},hlim[6]={0,0,0,0,0,0};
        float pippo;

    *non=0;
    ther_.titre=0.;
    ther_.ipha=0;

   /* Calcola H limite sottoregioni 1,4
      e distingue sottoregioni 1,2 o 3 o 4,5                        */

    if ( (init_.i0-init_.kk) >= 0 )
      {
   /* REGIONI SOPRACRITICHE                                         */
       inte_.id=inte_.incr+9;
       new=0;
       newt_(&new,&hlim[0]);
       hlim[3]=hlim[0];
       if ( (*hd-hlim[0]) < 0. )   aca=100;
       else                        aca=200;
      }
    else
      {
   /* REGIONI SOTTOCRITICHE                                         */
       for (i=1; i<=4; i+=3)
         {
          inte_.id=inte_.incr-6+i+1;
          new=0;
          newt_(&new,&hlim[i-1]);
          if ( (*hd-hlim[i-1]) <= 0. )
            {
             acb=1;
             break;
            }
         }
       if (acb!=1)  aca=200;
      }
    if (aca==0)
      {
        if (i!=1)
           {
   /* SATURO: calcola in e titolo                                   */
              ther_.ipha=1;
              ther_.titre=(*hd-hlim[0])/(hlim[3]-hlim[0]);
              isfl=(*valda)[inte_.incr]+(*valda)[inte_.incr+1];
              is=isfl;
              ther_.in=inte_.incr+(is+1)*6;
              return;
           }
        else  aca=100;
      }

    if (aca==100)
      {
   /* SOTTOREGIONI 1,2
      Calcola H min sottoregione 1,2 ed individua sottoreg.: i      */
       inte_.id=inte_.incr+3;
       for (icic=1; icic<=2; ++icic)
          {
           i=icic;
           idfl=inte_.id+6*(*valda)[inte_.incr+i-1];
           inte_.id=idfl;
           new=0;
           newt_(&new,&hlim[i]);
           if ( (*hd-hlim[i]) >= 0. )
             {
              acc=1;
              break;
             }
          }
   /* Entalpia troppo piccola                                       */
       n=6;
       if (acc!=1)   
                 {
                  diagn_(&n,&(*hd),&cont_.pd);
                  pippo=*hd+1;
/* guag 19-11-91 */
                  *hd=hlim[i];
                  *s0=slim[i-1];
                  *non=1;
/* 
		  fprintf(2,"diagn_ hd= %f \n",*hd);
		  fprintf(2,"diagn_ s0= %f \n",*s0);
*/
                  pippo=*hd+1;
                  pippo=*s0+1;
                  return;
/* guag  */
                  }

       infl=inte_.incr+6*(*valda)[inte_.incr+i-2]*(i-1)+3.;
       ther_.in=infl;
       nnpfl=(*valda)[inte_.incr+i-1]-2.;
       nnp[i-1]=nnpfl;

   /* Se la sottoregione ha solo 3 punti is=1                       */
       k=2;
       if (nnp[i-1]==1)
         {
          is=k-1;
   /* Calcola s max delle sottoregioni 1,2 per il calcolo di s0     */
          if ( (init_.i0-init_.kk) >= 0 )
             slim[0]=4442.86472;
          else
            {
             inte_.id=inte_.incr-6+1;
             new=0;
             newt_(&new,&slim[0]);
            }
          if (i==2)
             slim[1]=slim[0]+init_.s[0]*((*valda)[inte_.incr]-1);
   /* Sottoregioni 1,2,4,5
      Calcola passo in s, in, s0                                    */
          inte_.pae=1./init_.s[i-1];
          ther_.in=ther_.in+6*is-3;
          *s0=slim[i-1]+init_.s[i-1]*(is-1);
          return;
         }

   /* Cerca H(P0) < HD                                              */
       for (j=2; j<=nnp[i-1]; ++j)
         {
          if ( (*hd-(*valda)[ther_.in+6*j-1]) >= 0. )
            {
             for (k=j; k<=nnp[i-1]; ++k)
               {
                inte_.id=ther_.in+6*k;
                new=0;
                newt_(&new,&hint);
                if ( (*hd-hint) >= 0. )
                  {
                   is=k-1;
   /* Calcola s max delle sottoregioni 1,2 per il calcolo di s0     */
                   if ( (init_.i0-init_.kk) >= 0 )
                      slim[0]=4442.86472;
                   else
                     {
                      inte_.id=inte_.incr-6+1;
                      new=0;
                      newt_(&new,&slim[0]);
                     }
                   if (i==2)
                      slim[1]=
                        slim[0]+init_.s[0]*((*valda)[inte_.incr]-1.);
   /* Sottoregioni 1,2,4,5
      Calcola passo in s, in, s0                                    */
                   inte_.pae=1./init_.s[i-1];
                   ther_.in=ther_.in+6*is-3;
                   *s0=slim[i-1]+init_.s[i-1]*(is-1);
                   return;
                  }
               }
   /* L'isoclina e' la terz'ultima is=nnp[i-1]                      */
             k=nnp[i-1]+1;
             is=k-1;
   /* Calcola s max delle sottoregioni 1,2 per il calcolo di s0     */
             if ( (init_.i0-init_.kk) >= 0 )
                      slim[0]=4442.86472;
             else
                     {
                      inte_.id=inte_.incr-6+1;
                      new=0;
                      newt_(&new,&slim[0]);
                     }
             if (i==2)
                      slim[1]=
                        slim[0]+init_.s[0]*((*valda)[inte_.incr]-1.);
   /* Sottoregioni 1,2,4,5
      Calcola passo in s, in, s0                                    */
             inte_.pae=1./init_.s[i-1];
             ther_.in=ther_.in+6*is-3;
             *s0=slim[i-1]+init_.s[i-1]*(is-1);
             return;
            }
         }
   /* L'isoclina e' la terz'ultima is=nnp[i-1]                      */
       k=nnp[i-1]+1;
       is=k-1;
   /* Calcola s max delle sottoregioni 1,2 per il calcolo di s0     */
       if ( (init_.i0-init_.kk) >= 0 )  slim[0]=4442.86472;
       else                            {
                                        inte_.id=inte_.incr-6+1;
                                        new=0;
                                        newt_(&new,&slim[0]);
                                       }
       if (i==2) slim[1]=slim[0]+init_.s[0]*((*valda)[inte_.incr]-1.);
   /* Sottoregioni 1,2,4,5
      Calcola passo in s, in, s0                                    */
       inte_.pae=1./init_.s[i-1];
       ther_.in=ther_.in+6*is-3;
       *s0=slim[i-1]+init_.s[i-1]*(is-1);
       return;
      }

    if (aca==200)
      {
   /* SOTTOREGIONI 4,5                                              */
       ther_.titre=1.;
       incr4=inte_.incr;
       for (i=1; i<=3; ++i)
         {
          incr4fl=incr4+6*(*valda)[inte_.incr+i-1];
          incr4=incr4fl;
         }
       inte_.id=incr4+3;
   /* Calcola H max sottoregione 4,5 ed individua sottoreg.: i      */
       for (icic=4; icic<=5; ++icic)
         {
          i=icic;
          idfl=inte_.id+6*(*valda)[inte_.incr+i-1];
          inte_.id=idfl;
          new=0;
          newt_(&new,&hlim[i]);
          if ( (hlim[i]-*hd) >= 0. )
            {
             acc=1;
             break;
            }
         }
   /* Entalpia troppo elevata                                       */
       n=6;
       if (acc!=1)
                  {
                    diagn_(&n,&(*hd),&cont_.pd);
                  pippo=*hd+1;
/* guag 19-11-91 */
                  *hd=hlim[i];
                  *s0=slim[i-1];
                  *non=1;
                  pippo=*hd+1;
                  pippo=*s0+1;
                  return;
/* guag  */
                  }
   
       infl=incr4+6*(*valda)[inte_.incr+i-2]*(i-4)+3.;
       ther_.in=infl;
       nnpfl=(*valda)[inte_.incr+i-1]-2.;
       nnp[i-1]=nnpfl;

   /* Se la sottoregione ha solo 3 punti is=1                       */
       k=1;
       if (nnp[i-1]==1)
         {
          is=k;
   /* Calcola s min delle sottoregioni 4,5 per il calcolo di s0     */
          if ( (init_.i0-init_.kk) >= 0 )
             slim[3]=4442.86472;
          else
            {
             inte_.id=inte_.incr-6+4;
             new=0;
             newt_(&new,&slim[3]);
            }
          if (i==5)
             slim[4]=slim[3]+init_.s[3]*((*valda)[inte_.incr+3]-1.);
   /* Sottoregioni 1,2,4,5
      Calcola passo in s, in, s0                                    */
          inte_.pae=1./init_.s[i-1];
          ther_.in=ther_.in+6*is-3;
          *s0=slim[i-1]+init_.s[i-1]*(is-1);
          return;
         }

   /* Cerca H(P0) > HD                                              */
       for (j=2; j<=nnp[i-1]; ++j)
         {
          if ( ((*valda)[ther_.in+6*j-1]-*hd) >= 0. )
            {
             acd=1;
             break;
            }
         }
       if (acd!=1)   j=nnp[i-1];
       jj=j-1;

   /* Cerca H(P) < HD                                               */
       for (kj=1; kj<=jj; ++kj)
         {
          k=j-kj+1;
          inte_.id=ther_.in+6*k;
          new=0;
          newt_(&new,&hint);
          if ( (*hd-hint) >= 0. )
            {
             is=k;
   /* Calcola s min delle sottoregioni 4,5 per il calcolo di s0     */
             if ( (init_.i0-init_.kk) >= 0 ) 
                slim[3]=4442.86472;
             else
               {
                inte_.id=inte_.incr-6+4;
                new=0;
                newt_(&new,&slim[3]);
               }
             if (i==5)
               slim[4]=slim[3]+init_.s[3]*((*valda)[inte_.incr+3]-1.);
   /* Sottoregioni 1,2,4,5
      Calcola passo in s, in, s0                                    */
             inte_.pae=1./init_.s[i-1];
             ther_.in=ther_.in+6*is-3;
             *s0=slim[i-1]+init_.s[i-1]*(is-1);
             return;
            }
         }
   /* L'isoclina e' la prima is=1                                   */
       k=1;
       is=k;
   /* Calcola s min delle sottoregioni 4,5 per il calcolo di s0     */
       if ( (init_.i0-init_.kk) >= 0 )
          slim[3]=4442.86472;
       else
         {
          inte_.id=inte_.incr-6+4;
          new=0;
          newt_(&new,&slim[3]);
         }
       if (i==5)
          slim[4]=slim[3]+init_.s[3]*((*valda)[inte_.incr+3]-1.);
   /* Sottoregioni 1,2,4,5
      Calcola passo in s, in, s0                                    */
       inte_.pae=1./init_.s[i-1];
       ther_.in=ther_.in+6*is-3;
       *s0=slim[i-1]+init_.s[i-1]*(is-1);
       return;
      }

   }   /* FINE DELLA cerca_ */


   /* --------------- FINE del sorgente --------------------------- */
