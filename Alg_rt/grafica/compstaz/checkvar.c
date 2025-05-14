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
static char SccsID[] = "@(#)checkvar.c	1.3\t3/23/95";
/*
   modulo checkvar.c
   tipo 
   release 1.3
   data 3/23/95
   reserved @(#)checkvar.c	1.3
*/
/*
	CHECK_MODEL
	Routine che ricerca nell' area shared delle variabili il modello
	specificato in input e ritorna l'indice ; se non esiste termina
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>
#include "sim_param.h"
#include "sim_types.h"
#include "xstaz.h"
#include "compstaz.h"
//#include "conv_mmi.h"

extern	int	nriga;
extern	int  tot_modelli,tot_variabili;

extern	char *ind_sh_top;
extern  int id_sh;
extern VARIABILI *variabili;


int check_model(
 char *stringa,
 int *imu)
{
int i;
NOMI_MODELLI *p_modelli;

if (strcmp(stringa,"modello"))  
{
        p_modelli = (NOMI_MODELLI *) (ind_sh_top + sizeof(int));
	for (i=0; i<tot_modelli; i++, p_modelli++)
  	if (! strncmp( stringa, p_modelli->nome, strlen(stringa))) 
	{
		*imu=i+1;    
		return(0);
	}
/*
	il modello non esiste, termina la compilazione 
*/
	distruggi_var(id_sh);
	printf("\n IL MODELLO %s CITATO ALLA RIGA %d  NON ESISTE\n ",stringa,nriga);
	exit(puts("COMPILAZIONE TERMINATA CON ERRORE "));
}
else *imu=0;
}

/*
	CHECK_OUTPUT
	Verifica se la variabile definita appartiene al modello
        specificato
*/

int check_output(
 char *nomevar,
 int imu,
 int *iu)

{
int i;
NOMI_MODELLI *p_modelli;
int ier;
char nvar[MAX_LUN_NOME_VAR];

if (strcmp(nomevar,"variabil"))
{
	memset(nvar,' ',MAX_LUN_NOME_VAR-1);
	nvar[MAX_LUN_NOME_VAR-1]=0;
	memcpy(nvar,nomevar,strlen(nomevar));
        for (i=0; i<tot_variabili; i++)
	{
		if (variabili[i].mod != imu ) continue;
                ier=strcmp(nvar,variabili[i].nome);
		if (!ier)
		{
			*iu=variabili[i].addr;
			return(0);
		}
        }
        p_modelli = (NOMI_MODELLI *) (ind_sh_top + sizeof(int) + (imu-1)*sizeof(NOMI_MODELLI));
        printf("\n LA VARIABILE %s NON E' USCITA DEL MODELLO %s (RIGA: %d ) ",nomevar,p_modelli->nome,nriga);
        distruggi_var(id_sh);
        exit(puts("COMPILAZIONE TERMINATA CON ERRORE "));

}
else *iu=0;
}


/*
        CHECK_input
        Verifica se la variabile definita appartiene al modello
        specificato e se e' un ingresso non connesso
*/

int check_input(
 char *nomevar,
 int imu,
 int *iu)

{
int i;
NOMI_MODELLI *p_modelli;
char nvar[MAX_LUN_NOME_VAR];


if (strcmp(nomevar,"variabil"))
{
        memset(nvar,' ',MAX_LUN_NOME_VAR-1);
        nvar[MAX_LUN_NOME_VAR-1]=0;
        memcpy(nvar,nomevar,strlen(nomevar));
        for (i=0; i<tot_variabili; i++)
        {
                if (variabili[i].mod != imu ) continue;
                if (!strcmp(nvar,variabili[i].nome))
                {
	        	if (variabili[i].tipo != INGRESSO_NC ) break;
                        *iu=variabili[i].addr;
                        return(0);
                }
        }
        p_modelli = (NOMI_MODELLI *) (ind_sh_top + sizeof(int) + (imu-1)*sizeof(NOMI_MODELLI));
        printf("\n LA VARIABILE %s NON E' UN INGRESSO  DEL MODELLO %s (RIGA : %d) ",nomevar,p_modelli->nome,nriga);
        distruggi_var(id_sh);
        exit(puts("COMPILAZIONE TERMINATA CON ERRORE "));

}
else *iu=0;
}

