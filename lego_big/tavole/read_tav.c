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
 * read_tav.c
 * legge il file TAVOLE.DAT e lo trasferisce nella memoria
 * corrispondente al vettore valda. 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tav_vap.h"
char *getenv(const char *name);
int dim_rec[NUM_REGIONI]={41,44,48,47,48,49,49,51,52,53,54,57,59,62,66,
			  69,69,69,71,73,70,70,70,70,70,70,70,70,69,69,68,
                          68};
#define K_DIM_REG (6*11)  /* costante per cui moltiplicare le sopra riportate 
                             dimensioni delle regioni */
extern float (*valda)[];

void read_tav()
{
int i;
FILE *fp;
float *ptav;  /* puntatore all'area di memoria utilizzata
                 per le tavole  */
char app[200];
char *path_tavole;
ptav=(float *)valda;  /* punta all'inizio delle tavole */
path_tavole=getenv("TAVOLE");
strcpy(app,path_tavole);
strcat(app,"/TAVOLE.DAT");
/* printf("\n apertura %s",app); */
fp=fopen(app,"rb");
if(fp==NULL) perror("Errore aperura file TAVOLE.DAT");
for(i=0;i<NUM_REGIONI;i++)
	{
	fread(ptav,sizeof(float),dim_rec[i]*K_DIM_REG,fp);
	ptav+=(dim_rec[i]*K_DIM_REG);
/*	printf("\n%d) dim_rec = %d    ptav= %d",i,dim_rec[i],ptav); */
	fseek(fp,(DIM_REGIONE*(i+1)*sizeof(float)),0);
	}
fclose(fp);
}

