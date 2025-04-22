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
static char SccsID[] = "@(#)list_vargraf.c	5.1\t11/7/95";
/*
   modulo list_vargraf.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)list_vargraf.c	5.1
*/
/*
	routine di creazione della lista delle variabili selezionate
	per la grafica
*/
#include <stdio.h>
#include <string.h>
# include "sim_param.h"         /* parametri generali LEGO          */
# include "sim_types.h"         /* tipi e costanti LEGO             */
#include  "f22.h"
#include <Rt/RtMemory.h>

extern char *ind_sh_top;	/* puntatore iniziale area shared variabili */
extern VARIABILI *variabili;
extern int tot_variabili;         /* numero totale delle variabili */
extern int tot_righe_simboli;    /* numero totale elementi della lista simboli*/
extern int num_var_graf;		  /* nuemro variabili selezionate per la
				     grafica */

extern char **simboli;		/* lista di tutte le variabili */
extern char nomi_misure[NUM_VAR][MAX_LUN_NOME_VAR];

char   *nome_modello (char *, int);
char   *nome_blocco(char *,int,int);

void list_vargraf()
{
int k, col, indvar, ind_mod, ind_bloc, ilista;
char *pdata;


col=LUN_SIMB+1;
pdata = (char *) calloc(tot_righe_simboli * col,  sizeof(char));
if(pdata==(char *)NULL) {
                printf("\n mancanza di spazio per allocazione");
                exit(1);
                }
simboli = (char **) calloc(tot_righe_simboli, sizeof(char *));
if(simboli==(char **)NULL) {
                printf("\n mancanza di spazio per allocazione");
                exit(1);
                }
printf("\n tot_righe %d \n",tot_righe_simboli);
for(k=0; k<tot_righe_simboli ;k++)
                {
                simboli[k]=pdata;
                pdata+=col;
                }

indvar=0;
ind_mod=0;
ind_bloc=0;
ilista=0;
for( k =0 ; k < tot_variabili ; k++ )
{
   if (variabili[k].typ==NO_GRAF_SEL) continue;
   if (variabili[k].mod != ind_mod)
        {
        sprintf(simboli[ilista]," ****  MODELLO %s",
                            nome_modello(ind_sh_top,variabili[k].mod)); 
	ilista++;
	ind_mod=variabili[k].mod;
        ind_bloc= -1;
        }
   if (variabili[k].blocco != ind_bloc)
        {
        strcpy(simboli[ilista]," ****  ");
        strncat(simboli[ilista],nome_blocco(ind_sh_top,variabili[k].mod,
                                        variabili[k].blocco),LUN_SIMB-7);
	simboli[ilista][LUN_SIMB]=0;
        ilista++;
        ind_bloc=variabili[k].blocco;
        }
   strcpy(&nomi_misure[indvar][0],variabili[k].nome);
   strcpy(simboli[ilista],variabili[k].nome);
   strcat(simboli[ilista]," - ");
   strncat(simboli[ilista],variabili[k].descr,LUN_SIMB-15);
   indvar++;
   ilista++;
}

}

