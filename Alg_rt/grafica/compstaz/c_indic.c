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
static char SccsID[] = "@(#)c_indic.c	1.3\t3/23/95";
/*
   modulo c_indic.c
   tipo 
   release 1.3
   data 3/23/95
   reserved @(#)c_indic.c	1.3
*/
/*
	Compilazione oggetto tipo indicatore
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


extern FILE *fo;

int c_indic(p_r02,sottotipo)
TIPO_INDICATORE *p_r02;
int	sottotipo;
{
int i,lun,nstr,j,imu,index;
float valore,valmin,valmax,offset;

  p_r02->tipo_oggetto=INDICATORE;
/*
        legge lo scalamento della variabile
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,s_scalam,strlen (s_scalam)) ||
                          (string[1].stringa == NULL )) errore(ERR_SCAL,riga);
  if (co_float(13,6,&string[1],&valore)) errore(ERR_SCAL,riga);
  p_r02->scalam=valore;
        fprintf(fo,"\n valore %f ",valore);
/*
        legge i valori di fondo scala
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,s_minmax,strlen (s_minmax)) ||
    (string[1].stringa == NULL ) || (string[2].stringa == NULL ))  errore(ERR_MINMAX,riga);
  if (co_float(13,6,&string[1],&valmin)) errore(ERR_MINMAX,riga);
  if (co_float(13,6,&string[2],&valmax)) errore(ERR_MINMAX,riga);
  p_r02->basso_norm=valmin;
  p_r02->alto_norm=valmax;

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
	se l' indicatore e' di tipo ad ago con errore
	legge i valori di scalamento e i range per l'errore
*/
  if ( sottotipo == INDIC_AGO_ERR)
  {
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,s_scalam_err,strlen (s_scalam_err)) ||
                          (string[1].stringa == NULL )) errore(ERR_SCAL,riga);
  if (co_float(13,6,&string[1],&valore)) errore(ERR_SCALERR,riga);
  p_r02->scalam_err=valore;
  fprintf(fo,"\n valore_err %f ",valore);
/*
        legge i valori di fondo scala
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,s_minmax_err,strlen (s_minmax_err)) ||
    (string[1].stringa == NULL ) || (string[2].stringa == NULL )) 
					 errore(ERR_MINMAXERR,riga);
  if (co_float(13,6,&string[1],&valmin)) errore(ERR_MINMAXERR,riga);
  if (co_float(13,6,&string[2],&valmax)) errore(ERR_MINMAXERR,riga);
  p_r02->basso_err=valmin;
  p_r02->alto_err=valmax;
  }

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
        legge il nome della variabile e il modello della variabile INPUT_ERR
*/

  if (sottotipo == INDIC_AGO_ERR)
  {
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,s_inperr,strlen(s_inperr))) 
                                                errore(ERR_INPUT_ERR,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ))
  {
        p_r02->input_err = -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input_err=index;
  }
  fprintf(fo,"\n variabile input %d",p_r02->input_err);
  }

}

