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
static char SccsID[] = "@(#)c_tasto.c	1.3\t3/23/95";
/*
   modulo c_tasto.c
   tipo 
   release 1.3
   data 3/23/95
   reserved @(#)c_tasto.c	1.3
*/
/*
	Compilazione dell' oggetto di tipo  TASTO
*/
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>
#include "sim_param.h"
#include "sim_types.h"
#include "xstaz.h"
#include "compstaz.inc"

extern float get_valore();
// void legge_riga( char *riga, int *lun, int *nriga );
// void separa_str( char *riga, int lun, int nstr, STRIN_ST strin[]);

extern  STRIN_ST string[];

extern FILE *fo;

extern  char riga [80];
extern  int nriga;

extern	char *stipo_perturb[];
extern	char *scolori_oggetti[];


int c_tasto(p_r02)
TIPO_TASTO *p_r02;
{
int i,lun,nstr,j,imu,index;


  p_r02->tipo_oggetto=TASTO;
/*
	legge il colore di sfondo del tasto
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,s_colore,strlen(s_colore)) ||
    		 (string[1].stringa == NULL )) errore(ERR_COLORE,riga);
  for (i=0; scolori_oggetti[i]!=NULL; i++)
    if (!strncmp(string[1].stringa,scolori_oggetti[i],strlen(scolori_oggetti[i])
)) break;
  if (scolori_oggetti[i] == NULL) errore(ERR_COLORE,riga);
  p_r02->colore=i;
  fprintf(fo,"\n colore %s ",scolori_oggetti[i]);

/*
	legge la  variabile di output associata al tasto, il modello
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
}

