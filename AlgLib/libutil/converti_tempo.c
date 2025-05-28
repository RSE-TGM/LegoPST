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
static char SccsID[] = "@(#)converti_tempo.c	5.3\t2/21/96";
/*
   modulo converti_tempo.c
   tipo 
   release 5.3
   data 2/21/96
   reserved @(#)converti_tempo.c	5.3
*/
#include <stdio.h>

int AnnoBis(long );

/*
    converti_tempo

      Converte il tempo di simulazione espresso come valore floating 
      sempre crescente, come data e ora a partire dalle 
      ora:min:sec del gior/mes/anno.
             
   Parametri

      flot temposim: tempo di simulazione in secondi (valore multiplo del
                    passo di simulazione di 1.2 sec)
      long *ora:    ora di partenza da cui calcolare il tempo trascorso;
      long *min:    minuto di partenza da cui calcolare il tempo trascorso;
      long *sec:    secondo di partenza da cui calcolare il tempo trascorso;
      long *gior:   giorno di partenza da cui calcolare il tempo trascorso;
      long *mes:    mese di partenza da cui calcolare il tempo trascorso;
      long *anno:   anno di partenza da cui calcolare il tempo trascorso;

*/

int converti_tempo(temposim,ora,min,sec,gior,mes,anno)
float temposim;
long  *ora,*min,*sec,*gior,*mes,*anno;
{
float tsim;
short i,incr; 
short giorni;
short lastgiorno;
static short giomese[]={31,28,31,30,31,30,31,31,30,31,30,31};
long appoggio;


   giorni=(short)(temposim/86400.);           /* giorni trascorsi */
   tsim=temposim-giorni*86400.; 		
   incr=(short)(tsim/3600.);              /* ore trascorse */
   (*ora)=(*ora)+(long)incr;
   tsim=tsim-incr*3600.; 		
   incr=(short)(tsim/60.);                /* minuti trascorsi */
   (*min)=(*min)+(long)incr;
   incr=(short)(tsim-incr*60.); 		
   (*sec)=(*sec)+(long)incr;                           /* secondi trascorsi */
   
/*
   test overflow
*/
   if((*sec)>=60)          /* secondi */
   {
      (*sec)=0;
      (*min)++;
   }
   if((*min)>=60)           /* minuti */
   {
      (*min)=0;
      (*ora)++;
   }
   if((*ora)>=24)              /* ore */
   {
      (*ora)=(*ora)-24;
      giorni++;
   }

/*
   data
*/   
   appoggio = (*mes) - 1;

   lastgiorno=giomese[(short)appoggio];

   if(AnnoBis((*anno)) && (*mes)==2) lastgiorno++;
   for (i=0;i<giorni;i++)
   {
      (*gior)++;
      if((*gior)>lastgiorno)        /* giorno */
      {
         (*gior)=1;
         (*mes)++;
         if((*mes)>12)                          /* mese */
         {
            (*mes)=1;
            (*anno)++;
         }
         lastgiorno=giomese[(short)(*mes)-1];
         if(AnnoBis((*anno)) && (*mes)==2) lastgiorno++;
      }
   }
}


/*
   AnnoBis()
      Dato un anno verifica se e' bisestile.

   Parametri:
      short anno: anno da analizzare

   Ritorno:
      0:     anno non bisestile
      1:     anno bisestile

   9 Dicembre 1992
*/
int AnnoBis(long anno)
{
   if(!(anno % 4) && (anno % 100) || !(anno % 400)) return(1);
   return(0);
}

