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
static char SccsID[] = "@(#)uni_mis.c	5.1\t11/10/95";
/*
   modulo uni_mis.c
   tipo 
   release 5.1
   data 11/10/95
   reserved @(#)uni_mis.c	5.1
*/
/*
 *  uni_mis.c
 *      contiene le routines per default unita' di
 *      misura
 */
#include <stdio.h>      /* For printf and so on. */
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <uni_mis_val.h>
#include <ctype.h>

//#include <sim_types.h>
static void init_umis();
static int cerca_num_umis();
static void agg_umis();
static int cerca_umis(char*);


/*
   apertura file uni_misc.dat e lettura dati nella tabella uni_mis;
   se non esiste lo crea inizializzato con i valori di default definiti 
   nella tabella uni_mis (vedi uni_mis_val.h>)
   
*/ 

void init_umis()
{
int i,num_umis;
FILE *fpUMIS;

num_umis=cerca_num_umis();
fpUMIS=fopen("uni_misc.dat","r");
if(fpUMIS==NULL)
        {
/*
  se il file non era apribile in lettura lo inizializza con i default
*/
fpUMIS=fopen("uni_misc.dat","w");
        if(fpUMIS == NULL)
                {
                printf("\n errore apertura file unita' di misura\n");
                exit(0);
                }
        for(i=0;i<num_umis;i++)
                fwrite(&uni_mis[i],sizeof(S_UNI_MIS),1,fpUMIS);
        }
else
        {
        for(i=0;i<num_umis;i++)
                fread(&uni_mis[i],sizeof(S_UNI_MIS),1,fpUMIS);
        }
fclose(fpUMIS);
}

/* agg_umis()
    aggiorna il file delle unita' di misura scrivendo i valori correnti
*/
void agg_umis()
{
int i,num_umis;
FILE *fpUMIS;

num_umis=cerca_num_umis();
fpUMIS=fopen("uni_misc.dat","w");

for(i=0;i<num_umis;i++)
        fwrite(&uni_mis[i],sizeof(S_UNI_MIS),1,fpUMIS);
fclose(fpUMIS);
}
/*
 * cerca_umis
 *      trova l'indice nella tabella delle unita' di misura in base
 *      al primo carattere della sigla.
 */

int cerca_umis(descr)
char *descr;
{
int i,num_umis;
num_umis=cerca_num_umis();
for(i=0;i<num_umis;i++)
	{
	if(toupper(descr[0]) == (int) uni_mis[i].type)
		return(i);
	}
return(num_umis-1); /* se non e' stata trovata l'unita' di misura
                       assume come umis l'ultima in tabella che 
                       corrisponde al caso di undefined */  
}

/*
   ricerca quante unita' di misura sono previste
*/

int cerca_num_umis()
{
int i;

i= sizeof (uni_mis) / sizeof (S_UNI_MIS);
return(i);
}
