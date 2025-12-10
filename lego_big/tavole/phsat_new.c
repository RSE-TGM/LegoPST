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

/* **************************************************************** *
 * ***  ROUTINE PER IL CALCOLO DELLE PROPRIETA' TERMODINAMICHE  *** *
 * **************************************************************** */

# include <stdio.h>
#include <stdlib.h>
# include <errno.h>
# include <math.h>
# include "tavpar.h"           /* strutture common delle tavole     */


/* ************* DEFINIZIONE VARIABILI GLOBALI ******************** */

   extern float (*valda)[];

   extern struct B tabl_ ;
   extern struct D init_ ;


   /* ************  DEFINIZIONE DELLA SUBROUTINE  ***************** */

#if defined HELIOS

 float PHSAT (void ** params)

   {

   float *hnow = *((float*) params);
#else
 void phsat_new_(float *hnow,float *phs)
   {
#endif
    int afa=0;
    int ia,ifo,ireg,ip0,is0,is1,ip;
    float pasp,p0;
    float aa,bb,cc,csi,hw0,hw1,hw2;
    float sgn,qdr,rdc;
    double rdcdb;
    FILE *fp;

   /* Ricerca regione
      test su HWS: ultima isobara = prima isobara regione superiore */

    ia=1;
    ifo=init_.kk-1;                  /* ultima regione sottocritica */
    for (ireg=ia; ireg<=ifo; ++ireg)
      {
       ip=tabl_.ic[ireg-1]*6;        /* occupazione di una isobara  */
       if (*hnow <= (*valda)[tabl_.ioffset[ireg]-ip+1])
         {
          afa=1;
          break;
         }
      }
    if (afa==0)                      /* fine regioni sottocritiche  */
      {
       fp=fopen("lg5c.out","a");
       fprintf(fp,".....H = %f",*hnow);
       fprintf(fp," non esiste PSAT(H)\n");
       fprintf(fp,".....STOP in PHSAT\n");
       fclose(fp);
       exit(0);
      }

   /* Ricerca isobara, dalla seconda all'ultima                     */
    if (ireg==1 && *hnow<(*valda)[1])            /* errore          */
      {
       fp=fopen("lg5c.out","a");
       fprintf(fp,".....H = %f",*hnow);
       fprintf(fp," non esiste PSAT(H)\n");
       fprintf(fp,".....STOP in PHSAT\n");
       fclose(fp);
       exit(0);
      }
    is1=1;
    is1++;
    if (is1>init_.ii)                            /* assurdo         */
      {
       fp=fopen("lg5c.out","a");
       fprintf(fp,".....STOP in PHSAT ?????\n");
       fclose(fp);
       exit(0);
      }
    while (*hnow > (*valda)[tabl_.ioffset[ireg-1]+ip*(is1-1)+1])
      {
       is1++;
       if (is1>init_.ii)
         {
          fp=fopen("lg5c.out","a");
          fprintf(fp,".....STOP in PHSAT ?????\n");
          fclose(fp);
          exit(0);
         }
      }
    is0=is1-1;                       /* n. isobara di P0            */
    if (is1==init_.ii)  is0=is0-1;   /* limite superiore di regione */

    pasp=tabl_.pap[ireg-1];          /* passo in pressione          */
    p0=tabl_.prt[ireg-1]+(is0-1)*pasp;
    ip0=tabl_.ioffset[ireg-1]+ip*(is0-1)+2;
                                     /* puntatore ad HWS(P0)        */
    hw0=(*valda)[ip0-1];
    hw1=(*valda)[ip0+ip-1];
    hw2=(*valda)[ip0+ip+ip-1];

   /* Inversione interpolazione in P                                */
    aa=hw2-2.*hw1+hw0;
    bb=(-aa)+2.*hw1-2.*hw0;
    cc=(-2.)*((*hnow)-hw0);
    if (bb < 0.)    sgn=(-1.);
    else            sgn=1.;
    qdr=bb*bb-4.*aa*cc;
    rdcdb=sqrt((double) qdr);
    rdc=rdcdb;
    csi=(-2.)*cc/(bb+sgn*rdc);

    *phs=p0+csi*pasp;
    return;
//    return(p0+csi*pasp);

   }   /* FINE DELLA phsat_ */


   /* --------------- FINE del sorgente --------------------------- */
