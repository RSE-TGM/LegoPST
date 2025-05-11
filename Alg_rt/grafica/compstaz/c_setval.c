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
static char SccsID[] = "@(#)c_setval.c	1.3\t3/23/95";
/*
   modulo c_setval.c
   tipo 
   release 1.3
   data 3/23/95
   reserved @(#)c_setval.c	1.3
*/
/*
	Compilazione dell' oggetto di tipo  SET_VALORE
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

extern  char riga [80];
extern  int nriga;

extern	char *stipo_perturb[];
extern	char *scolori_oggetti[];


extern FILE *fo;

int c_setval(p_r02)
TIPO_SET_VALORE *p_r02;
{
int i,lun,nstr,j,imu,index;
char *px,saveriga[80];
float valore,offset;

  p_r02->tipo_oggetto=SET_VALORE;
/*
        legge la descrizione associata 
*/
  legge_riga( riga, &lun, &nriga);
  strcpy(saveriga,riga);
  separa_str( riga, lun, nstr=10, string);
  if (strncmp(string[0].stringa,s_etic,strlen(s_etic))) errore(ERR_ETIC,riga);
  memset(p_r02->etic,0,LUNG_ETICHETTA+1);
  if (string[1].lun_stringa)
  {
        px=strstr(saveriga,string[1].stringa);
        strncpy(p_r02->etic,px,LUNG_ETICHETTA);
  }
  fprintf(fo,"\n etichetta %s ",p_r02->etic);
/*
	legge la  variabile di output associata , il modello
	di appartenenza e il tipo di perturbazione 
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=5, string);
  if (strncmp(string[0].stringa,s_out,strlen(s_out)))
						errore(ERR_OUTPUT,riga);
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
  fprintf(fo,"\n output modello %d indice %d perturb%d",p_r02->out.nmod,p_r02->out.
indice,p_r02->out.tipo_pert);
/*
        legge la  variabile di output associata a rltarget , il modello
        di appartenenza e il tipo di perturbazione
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=4, string);
  if (strncmp(string[0].stringa,s_out,strlen(s_out)))
                                                errore(ERR_OUTPUT,riga);
 if ((string[1].stringa == NULL) && (string[2].stringa == NULL ) &&
                        (string[3].stringa == NULL ))
  {
        p_r02->rltarget.nmod= -1;
        p_r02->rltarget.indice= -1;
        p_r02->rltarget.tipo_pert= -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_input(string[1].stringa,imu,&index);
  for (i=1; stipo_perturb[i]!=NULL; i++)
 if (!strcmp(string[3].stringa,stipo_perturb[i])) break;
  if (stipo_perturb[i] == NULL) errore(ERR_PERTURB,riga);
  p_r02->rltarget.nmod=imu;
  p_r02->rltarget.indice=index;
  p_r02->rltarget.tipo_pert=i;
  }
  fprintf(fo,"\n rltarget modello %d indice %d perturb%d",p_r02->rltarget.nmod,p_r02->rltarget.indice,p_r02->rltarget.tipo_pert);
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
/*
  legge il valore di scalamento 
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,s_scalam,strlen (s_scalam)) ||
                          (string[1].stringa == NULL ))
  					errore(ERR_SCAL,riga);
  if (co_float(13,6,&string[1],&valore)) errore(ERR_SCAL,riga);
  p_r02->scalam=valore;
        fprintf(fo,"\n valore %f ",valore);

/*
        legge il valore dell' offset
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,s_offset,strlen (s_offset)) ||
            (string[1].stringa == NULL )) errore(ERR_OFFSET,riga);
  if (co_float(13,6,&string[1],&offset)) errore(ERR_OFFSET,riga);
  p_r02->offset=offset;

/*
	INIBIZ
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,s_inib,strlen(s_inib)))
          					 errore(ERR_INIBIZ,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ))
  {
        p_r02->inibiz = -1;
  }
  else
  {
  	check_model(string[2].stringa,&imu);
	check_output(string[1].stringa,imu,&index);
	p_r02->inibiz=index;
  }
  fprintf(fo,"\n variabile inibiz %d",p_r02->inibiz);

}

