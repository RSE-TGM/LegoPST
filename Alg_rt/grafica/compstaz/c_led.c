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
static char SccsID[] = "@(#)c_led.c	1.3\t3/23/95";
/*
   modulo c_led.c
   tipo 
   release 1.3
   data 3/23/95
   reserved @(#)c_led.c	1.3
*/
/*
	Compilazione tipo oggetto LED
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

extern FILE *fo;

int c_led(p_r02)
TIPO_LED *p_r02;
{
int i,lun,nstr,j,imu,index;
char *px, saveriga[80];

/*
	legge il colore associato al led
*/

  p_r02->tipo_oggetto=LED;
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,s_colore,strlen(s_colore)) || (string[1].stringa == NULL ))
					  errore(ERR_COLORE,riga);
  for (i=0; scolori_oggetti[i]!=NULL; i++)
    if (!strncmp(string[1].stringa,scolori_oggetti[i],strlen(scolori_oggetti[i]))) break;
  if (scolori_oggetti[i] == NULL) errore(ERR_COLORE,riga);
  p_r02->colore=i;
  fprintf(fo,"\n colore %s ",scolori_oggetti[i]);

/*
	legge la descrizione associata al led 
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
	legge il nome della variabile e il modello della variabile INPUT
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=4, string);
  if (strncmp(string[0].stringa,s_inp,strlen(s_inp)) )
						errore(ERR_INPUT,riga);
  if (string[1].stringa == NULL && string[2].stringa == NULL )
  {
        p_r02->input_colore = -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input_colore=index;
  p_r02->neg_colore= is_neg(string[3].stringa); 
  }
  fprintf(fo,"\n variabile input %d",p_r02->input_colore);
/*
    legge il nome della variabile e il modello della variabile INPUT_BLINK
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=4, string);
  if (strncmp(string[0].stringa,s_inpb,strlen(s_inpb)))
                                                errore(ERR_INPUT_BLINK,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ))
  {
        p_r02->input_blink = -1;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input_blink=index;
  p_r02->neg_blink= is_neg(string[3].stringa); 
  }
  fprintf(fo,"\n variabile input_blink %d",p_r02->input_blink);
}

