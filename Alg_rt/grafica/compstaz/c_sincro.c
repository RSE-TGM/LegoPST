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
static char SccsID[] = "@(#)c_sincro.c	1.3\t3/23/95";
/*
   modulo c_sincro.c
   tipo 
   release 1.3
   data 3/23/95
   reserved @(#)c_sincro.c	1.3
*/
/*
	Compilazione oggetto tipo indicatore_sincro
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

extern  STRIN_ST string[];


extern  char riga [80];
extern  int nriga;

extern  char *stipo_perturb[];
extern  char *scolori_oggetti[];


extern FILE *fo;

int c_sincro(p_r02,sottotipo)
TIPO_INDICATORE_SINCRO *p_r02;
int     sottotipo;
{
int i,lun,nstr,j,imu,index;
float valore,valmin,valmax,offset;

  p_r02->tipo_oggetto=INDICATORE_SINCRO;
/*
        legge il nome della prima variabile e il modello della variabile INPUT
*/

  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,s_inp,strlen(s_inp)))
                                                errore(ERR_INPUT,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ))
  {
        p_r02->input_1 = -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input_1=index;
  }
  fprintf(fo,"\n variabile input %d",p_r02->input_1);
  
/*
        legge il nome della seconda variabile e il modello della variabile INPUT
*/

  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,s_inp,strlen(s_inp)))
                                                errore(ERR_INPUT,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ))
  {
        p_r02->input_2 = -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input_2=index;
  }
  fprintf(fo,"\n variabile input %d",p_r02->input_2);

/*
        legge il nome della terza variabile e il modello della variabile INPUT
*/

  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,s_inp,strlen(s_inp)))
                                                errore(ERR_INPUT,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ))
  {
        p_r02->input_3 = -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input_3=index;
  }
  fprintf(fo,"\n variabile input %d",p_r02->input_3);

/*
        legge il nome della quarta variabile e il modello della variabile INPUT
*/

  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,s_inp,strlen(s_inp)))
                                                errore(ERR_INPUT,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ))
  {
        p_r02->input_4 = -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input_4=index;
  }
  fprintf(fo,"\n variabile input %d",p_r02->input_4);

/*
        legge il nome della quinta variabile e il modello della variabile INPUT
*/

  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,s_inp,strlen(s_inp)))
                                                errore(ERR_INPUT,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ))
  {
        p_r02->input_5 = -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input_5=index;
  }
  fprintf(fo,"\n variabile input %d",p_r02->input_5);

/*
        legge il nome della sesta variabile e il modello della variabile INPUT
*/

  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,s_inp,strlen(s_inp)))
                                                errore(ERR_INPUT,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ))
  {
        p_r02->input_6 = -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input_6=index;
  }
  fprintf(fo,"\n variabile input %d",p_r02->input_6);


/*
        legge la  prima variabile di output associata all sincronoscopio, 
	il modello di appartenenza e il tipo di perturbazione
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=5, string);
  if (strncmp(string[0].stringa,s_out,strlen(s_out)))
                                                errore(ERR_OUTPUT,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ) &&
                        (string[3].stringa == NULL ))
  {
        p_r02->out_1.nmod= -1;
        p_r02->out_1.indice= -1;
        p_r02->out_1.tipo_pert= -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_input(string[1].stringa,imu,&index);
  if (string[3].stringa == NULL ) errore(ERR_PERTURB,riga);
  for (i=1; stipo_perturb[i]!=NULL; i++)
 if (!strcmp(string[3].stringa,stipo_perturb[i])) break;
  if (stipo_perturb[i] == NULL) errore(ERR_PERTURB,riga);
  p_r02->out_1.nmod=imu;
  p_r02->out_1.indice=index;
  p_r02->out_1.tipo_pert=i;
  p_r02->out_1.value=get_valore(string[4].stringa);
  }
  fprintf(fo,
	"\n output modello %d indice %d perturb %d",p_r02->out_1.nmod,p_r02-> out_1.indice,p_r02->out_1.tipo_pert);

/*
        legge la  seconda variabile di output associata all sincronoscopio, 
	il modello di appartenenza e il tipo di perturbazione
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=5, string);
  if (strncmp(string[0].stringa,s_out,strlen(s_out)))
                                                errore(ERR_OUTPUT,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ) &&
                        (string[3].stringa == NULL ))
  {
        p_r02->out_2.nmod= -1;
        p_r02->out_2.indice= -1;
        p_r02->out_2.tipo_pert= -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_input(string[1].stringa,imu,&index);
  if (string[3].stringa == NULL ) errore(ERR_PERTURB,riga);
  for (i=1; stipo_perturb[i]!=NULL; i++)
 if (!strcmp(string[3].stringa,stipo_perturb[i])) break;
  if (stipo_perturb[i] == NULL) errore(ERR_PERTURB,riga);
  p_r02->out_2.nmod=imu;
  p_r02->out_2.indice=index;
  p_r02->out_2.tipo_pert=i;
  p_r02->out_2.value=get_valore(string[4].stringa);
  }
  fprintf(fo,
	"\n output modello %d indice %d perturb %d",p_r02->out_2.nmod,p_r02-> out_2.indice,p_r02->out_2.tipo_pert);


}
