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
static char SccsID[] = "@(#)c_display.c	1.3\t3/23/95";
/*
   modulo c_display.c
   tipo 
   release 1.3
   data 3/23/95
   reserved @(#)c_display.c	1.3
*/
/*
	Compilazione dell' oggetto DISPLAY
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

extern	char *scolori_oggetti[];

extern	FILE *fo;

int c_display(p_r02)
TIPO_DISPLAY *p_r02;
{
int lun, nstr, imu, index;
char *px, saveriga[80];


  p_r02->tipo_oggetto=DISPLAY;

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
  fprintf(fo,"\n variabile input %d",p_r02->input );

}

