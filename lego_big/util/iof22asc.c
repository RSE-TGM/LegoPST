/*
   modulo iof22asc.c
   tipo 
   release 1.8
   data 7/21/94
   reserved @(#)iof22asc.c	1.8
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct common_variabili {
                         char nomi_misure[1000][8];
                         char simboli[1000][100];
                         float misure[1000];
                         float tempo;
                         char titolo[80];
                         int numvar;
                         int numsimb;
                         char model[8];
                        };

typedef struct common_variabili VARIABILE;

VARIABILE variabili_;

FILE  *f22;

//float atof();

void leggi_testa_asc()
   {
   int i;
   int j;
   char stringa[101];
      f22 = fopen("f22.asc","r");
      fgets( variabili_.titolo, 101, f22);
      fgets( stringa, 101, f22);
      variabili_.numvar = atoi(stringa);
      for (i=0; i<variabili_.numvar; i++)
      {
          fgets(variabili_.nomi_misure[i], 80, f22);
      }
      fgets( variabili_.model, 101, f22);
      fgets( stringa, 101, f22);
      variabili_.numsimb = atoi(stringa);
      for (i=0; i<variabili_.numsimb; i++)
      {
          fgets(variabili_.simboli[i], 120, f22);
          j = strlen(variabili_.simboli[i]);
          variabili_.simboli[i][j-1] = '\0';
      }
   }



int leggi_corpo_asc()
   {
   int i;
   char stringa[101];
      fgets( stringa, 101, f22);
      stringa[strlen(stringa)-1] = '\0';
      variabili_.tempo = atof(stringa);
      for (i=0; i<variabili_.numvar; i++)
      {
          if( ! fgets( stringa, 101, f22) )
             return(0);
          stringa[strlen(stringa)-1] = '\0';
          variabili_.misure[i] = atof(stringa);
      }
   }

void scrivi_testa_asc()
   {
   int i;
   int j;
   char stringa[101];
      f22 = fopen("f22.asc","w+");
      fprintf(f22, "%s\n", variabili_.titolo);
      fprintf(f22, "%i\n", variabili_.numvar);
      for (i=0; i<variabili_.numvar; i++)
      {
          strncpy(stringa,variabili_.nomi_misure[i],8);
          stringa[8] = '\0';
          fprintf(f22, "%s\n", stringa);
      }
      fprintf(f22, "%s\n", variabili_.model);
      fprintf(f22, "%i\n", variabili_.numsimb);
      for (i=0; i<variabili_.numsimb; i++)
      {
          strncpy(stringa,variabili_.simboli[i],8);
          stringa[100] = '\0';
          fprintf(f22, "%s\n", stringa);
      }
   }


void scrivi_corpo_asc()

   {
   int i;
            fprintf(f22, "%f\n", variabili_.tempo);
            for (i=0; i<variabili_.numvar; i++)
                fprintf(f22, "%f\n", variabili_.misure[i]);

   }

