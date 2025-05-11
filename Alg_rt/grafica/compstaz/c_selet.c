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
static char SccsID[] = "@(#)c_selet.c	1.3\t3/23/95";
/*
   modulo c_selet.c
   tipo 
   release 1.3
   data 3/23/95
   reserved @(#)c_selet.c	1.3
*/
/*
	Compilazione dell' oggetto tipo  SELETTORE
*/

#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>
#include "sim_param.h"
#include "sim_types.h"
#include "xstaz.h"
#include "compstaz.inc"

// void legge_riga( char *riga, int *lun, int *nriga );
// void separa_str( char *riga, int lun, int nstr, STRIN_ST strin[]);
extern float get_valore();

extern  STRIN_ST string[];

extern FILE *fo;

extern  char riga [80];
extern  int nriga;

extern	char *stipo_perturb[];


c_selet(p_r02)
TIPO_SELETTORE *p_r02;
{
int i,lun,nstr,j,imu,index;
char *px, saveriga[80];


  p_r02->tipo_oggetto=SELETTORE;

/*
	legge le  descrizioni associate al selettore
*/
  legge_riga( riga, &lun, &nriga);
  strcpy(saveriga,riga); 
  separa_str( riga, lun, nstr=10, string);
  if (strncmp(string[0].stringa,s_etic,strlen(s_etic))) errore(ERR_ETIC,riga);
  memset(p_r02->etic_0,0,LUNG_ETICHETTA+1);
  if (strlen(string[1].stringa)) 
  {
  	px=strstr(saveriga,string[1].stringa);
fprintf(fo,"\n px %s \n",px);
	strncpy(p_r02->etic_0,px,LUNG_ETICHETTA);
  }
  fprintf(fo,"\n etichetta 0 %s ",p_r02->etic_0);
  legge_riga( riga, &lun, &nriga);
  strcpy(saveriga,riga);
  separa_str( riga, lun, nstr=10, string);
  if (strncmp(string[0].stringa,s_etic,strlen(s_etic))) errore(ERR_ETIC,riga);
  memset(p_r02->etic_1,0,LUNG_ETICHETTA+1);
  if (strlen(string[1].stringa))
  {
        px=strstr(saveriga,string[1].stringa);
        strncpy(p_r02->etic_1,px,LUNG_ETICHETTA);
  }
  fprintf(fo,"\n etichetta 1 %s ",p_r02->etic_1);

/*
        legge la  variabile di output associata al pulsante, il modello
        di appartenenza e il tipo di perturbazione
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=5, string);
  if (strncmp(string[0].stringa,s_out,strlen(s_out))) errore(ERR_OUTPUT,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ) &&
                        (string[3].stringa == NULL ))
  {
        p_r02->out.nmod= -1;
        p_r02->out.indice= -1;
        p_r02->out.tipo_pert= -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_input(string[1].stringa,imu,&index);
 if (string[3].stringa == NULL ) errore(ERR_PERTURB,riga);
  for (i=1; stipo_perturb[i]!=NULL; i++)
 if (!strcmp(string[3].stringa,stipo_perturb[i])) break;
  if (stipo_perturb[i] == NULL) errore(ERR_PERTURB,riga);
  p_r02->out.nmod=imu;
  p_r02->out.indice=index;
  p_r02->out.tipo_pert=i;
  p_r02->out.value=get_valore(string[4].stringa);
  }
  fprintf(fo,"\n output modello %d indice %d perturb %d",p_r02->out.nmod,p_r02->out.
indice,p_r02->out.tipo_pert);

/*
	legge il nome della variabile e il modello della variabile INPUT
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,s_inp,strlen(s_inp)))
						errore(ERR_INPUT,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ))
  {
        p_r02->input = -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input=index;
  }
  fprintf(fo,"\n variabile input %d",p_r02->input);
}

