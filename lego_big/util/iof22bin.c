/*
   modulo iof22bin.c
   tipo 
   release 1.3
   data 7/21/94
   reserved @(#)iof22bin.c	1.3
*/
#include <stdio.h>
#include <string.h>

struct common_variabili {
                         char nomi_misure[1000][8];
                         char simboli[1000][80];
                         float misure[1000];
                         float tempo;
                         char titolo[80];
                         int numvar;
                         int numsimb;
                         char model[8];
                         char nomefile[80];
                         int lun_nome;
                        };

typedef struct common_variabili VARIABILE;

VARIABILE variabili_;

FILE  *file22, *file22out;


void leggi_testa_bin()
   {
   int i;
   char nomi[8][1000];
      file22 = fopen("f22","r");
      strcpy(variabili_.nomefile,"f22.out");
      variabili_.lun_nome=7;
      fread(variabili_.titolo,80,1,file22);
      fread(&variabili_.numvar,sizeof(variabili_.numvar),1,file22);
      fread(variabili_.nomi_misure,8,variabili_.numvar,file22);
      fread(variabili_.model,8,1,file22);
      fread(&variabili_.numsimb,sizeof(variabili_.numsimb),1,file22);
      fread(variabili_.simboli,80,variabili_.numsimb,file22);
   }


int leggi_corpo_bin()
   {
   int i;
            if ( ! fread(&variabili_.tempo,sizeof(variabili_.tempo),1,file22))
               return(0);
            fread(variabili_.misure,sizeof(float),variabili_.numvar,file22);

   }

void scrivi_testa_bin_()
   {
   int i;
   char nomi[8][1000];
      variabili_.nomefile[variabili_.lun_nome]='\0';
      file22out = fopen( variabili_.nomefile,"w+");
      fwrite(variabili_.titolo,80,1,file22out);
      fwrite(&variabili_.numvar,sizeof(variabili_.numvar),1,file22out);
      fwrite(variabili_.nomi_misure,8,variabili_.numvar,file22out);
      fwrite(variabili_.model,8,1,file22out);
      fwrite(&variabili_.numsimb,sizeof(variabili_.numsimb),1,file22out);
      fwrite(variabili_.simboli,80,variabili_.numsimb,file22out);
   }


void scrivi_corpo_bin_()
   {
   int i;
            fwrite(&variabili_.tempo,sizeof(variabili_.tempo),1,file22out);
            fwrite(variabili_.misure,sizeof(float),variabili_.numvar,file22out);

   }

     
