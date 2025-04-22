/*
   modulo creaf22.c
   tipo 
   release 1.3
   data 7/21/94
   reserved @(#)creaf22.c	1.3
*/
#include <stdio.h> 
#include <string.h> 
#include <memory.h>
#include <math.h> 
#include "f22.h"
#include "sim_types.h"

S_HEAD1 header1;
S_HEAD2 header2;
char nomi_misure[NUM_VAR+1][LUN_NOME+1]; 
char simboli[LUN_SIMB+1];

main(argc,argv)
int argc;
char **argv;
{
double dstep;
float step;
int num_camp,i,j;
int lung,intervallo;
int numvar=150;
int nbyte;
float tempo=0.0;

S_DATI dato;  /* dato da memorizzare */

FILE *fpDAT;

if(argc != 3)
	{
	printf("\nComando corretto: %s step num_campioni \n step e' un valore floating; num_campioni un intero\n",argv[0]);
	exit(1);
	}
/*
dstep= atof(argv[1]);
step=dstep;
*/
sscanf(argv[1],"%f",&step);
num_camp= atoi(argv[2]);

printf("\n Creazione file F22.dat:\n - step: %f\n - numero campioni: %d\n",
		step,num_camp);

fpDAT=fopen("f22","w+");
if(fpDAT==NULL)
	printf("errore apertura file ");

memset (header1.titolo,0,sizeof (S_HEAD1));
strcpy(header1.titolo,"FILE DI PROVA CREATO PRESSO S.D.I.  4/6/91 ");
header1.nvar=numvar;
if(!fwrite(&header1.titolo[0],sizeof(S_HEAD1),1,fpDAT)) exit(0);

for (i=0; i<numvar; i++)
{
	strcpy(&nomi_misure[i][0],"MISUR");
	sprintf(&nomi_misure[i][5],"%d",i);
  if(!fwrite(nomi_misure[i],LUN_NOME,1,fpDAT)) exit(0);
}

header2.ncasi=numvar;
strncpy(header2.nome_mod,"MODELLO0",LUN_NOME);
if(!fwrite(&header2.nome_mod[0],sizeof(S_HEAD2),1,fpDAT)) exit(0);

for (i=0; i<numvar; i++)
{
	memset(simboli,' ',LUN_SIMB);
	simboli[LUN_SIMB]=0;
	lung=strlen(nomi_misure[i]);
	strncpy(simboli,nomi_misure[i],lung);
	memset(&simboli[12],'*',LUN_SIMB-12);
   if(!fwrite(simboli,LUN_SIMB,1,fpDAT)) exit(0);
}

intervallo=(int)step;

for(i=0;i<num_camp;i++)
	{
sleep(intervallo);

	dato.t=tempo;
	for(j=0;j<numvar;j++)
		{
        dato.mis[j]=10*(j)*sin(tempo/20)+tempo/10;
/*		dato.mis[j]=10*j+10*(j)*sin(tempo/20)+tempo/10; */
		}
	nbyte=sizeof (float ) * (numvar +1);
	fwrite(&dato,nbyte,1,fpDAT);
fflush(fpDAT);

	tempo+=step;
	}
fclose(fpDAT);
}





