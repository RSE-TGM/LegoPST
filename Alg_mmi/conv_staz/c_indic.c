/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)c_indic.c	1.2\t6/16/93";
/*
   modulo c_indic.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)c_indic.c	1.2
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
#include "conv_mmi.h"


extern FILE *fp_staz[MAX_PAG];
extern int numero_el[MAX_PAG];
extern char nomi_ogg_pag[MAX_PAG][MAX_LUN_RIGA_ELENCO_WID];


// void legge_riga( char *riga, int *lun, int *nriga );
// void separa_str( char *riga, int lun, int nstr, STRIN_ST strin[]);

extern  STRIN_ST string[];


extern  char riga [80];
extern  int nriga;


extern FILE *fo;

int c_indic(p_r02,sottotipo,pagina,num_w,cont_f,elenco,x,y)
TIPO_INDICATORE *p_r02;
int pagina,num_w,x,y,sottotipo;
int *cont_f;
char *elenco;
{
int i,lun,nstr,j,imu,index;
float valore,valmin,valmax,offset;
char input_ago[50];
char input_ago_err[50];

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
        input_ago[0]=0;
  } 
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
/*
  p_r02->input=index;
*/
  p_r02->input=0;
  strcpy(input_ago,CostruisciRigaInput(string[1].stringa,string[2].stringa,
                p_r02->input));
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
        input_ago_err[0]=0;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
/*
  p_r02->input_err=index;
*/
  p_r02->input_err=0;
printf("\n INDIC: string[1]=%s\n",string[1].stringa);
printf("\n INDIC: string[2]=%s\n",string[2].stringa);
printf("\n INDIC: index=%d\n", index);
  strcpy(input_ago_err,CostruisciRigaInput(string[1].stringa,string[2].stringa,
                p_r02->input_err));
  }
  fprintf(fo,"\n variabile input %d",p_r02->input_err);
  }
/*
        inserisce l'indicatore nel file delle risorse
*/
if(sottotipo==INDIC_AGO)
   {
   fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,100);
   fprintf(fp_staz[pagina],"*%dw%dc.x0: %d\n",num_w,*cont_f,x);
   fprintf(fp_staz[pagina],"*%dw%dc.y0: %d\n",num_w,*cont_f,y);
   fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,100);
   fprintf(fp_staz[pagina],"*%dw%dc.tipoInd: 0\n",num_w,*cont_f);
   fprintf(fp_staz[pagina],"*%dw%dc.assRotate: 1\n",num_w,*cont_f);
   fprintf(fp_staz[pagina],"*%dw%dc.borderWidth: 1\n",num_w,*cont_f);
   sprintf(elenco,"%s %dw%dc Indic",elenco,num_w,*cont_f);
   }
else
if(sottotipo==INDIC_AGO_ERR)
   {
   fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,100);
   fprintf(fp_staz[pagina],"*%dw%dc.x0: %d\n",num_w,*cont_f,x);
   fprintf(fp_staz[pagina],"*%dw%dc.y0: %d\n",num_w,*cont_f,y);
   fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,100);
   fprintf(fp_staz[pagina],"*%dw%dc.assRotate: 1\n",num_w,*cont_f);
   fprintf(fp_staz[pagina],"*%dw%dc.borderWidth: 1\n",num_w,*cont_f);
   fprintf(fp_staz[pagina],"*%dw%dc.scalamentoErr: %f\n",num_w,*cont_f,
			p_r02->scalam_err);
   fprintf(fp_staz[pagina],"*%dw%dc.valoreMinimoErr: %f\n",
			num_w,*cont_f,p_r02->basso_err);
   fprintf(fp_staz[pagina],"*%dw%dc.valoreMassimoErr: %f\n",
			num_w,*cont_f,p_r02->alto_err);
   fprintf(fp_staz[pagina],"*%dw%dc.offsetErr: 0\n",
			num_w,*cont_f);
   if(input_ago!=NULL)
     fprintf(fp_staz[pagina],"*%dw%dc.varInputErr: %s\n",
        num_w,*cont_f,input_ago_err);
   sprintf(elenco,"%s %dw%dc IndicErr",elenco,num_w,*cont_f);
   }
else
/*
        Indicatore di tipo IBARRA1 
*/
   {
   fprintf(fp_staz[pagina],"*%dw%dc.x0: %d\n",num_w,*cont_f,x-10);
   fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,120);
   fprintf(fp_staz[pagina],"*%dw%dc.y0: %d\n",num_w,*cont_f,y-25);
   fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,50);
   fprintf(fp_staz[pagina],"*%dw%dc.tipoInd: 1\n",num_w,*cont_f);
   fprintf(fp_staz[pagina],"*%dw%dc.assRotate: 0\n",num_w,*cont_f);
   fprintf(fp_staz[pagina],"*%dw%dc.borderWidth: 0\n",num_w,*cont_f);
   sprintf(elenco,"%s %dw%dc Indic",elenco,num_w,*cont_f);
   }
fprintf(fp_staz[pagina],"*%dw%dc.normFg: black\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.background: %s\n",num_w,*cont_f,SFONDO_STAZ);
fprintf(fp_staz[pagina],"*%dw%dc.normalFont: %s\n",num_w,*cont_f,FONT_PICCOLO);
fprintf(fp_staz[pagina],"*%dw%dc.agoFg: red\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.scalamento: %f\n",num_w,*cont_f,
			p_r02->scalam);
fprintf(fp_staz[pagina],"*%dw%dc.valoreMinimo: %f\n",
			num_w,*cont_f,p_r02->basso_norm);
fprintf(fp_staz[pagina],"*%dw%dc.valoreMassimo: %f\n",
			num_w,*cont_f,p_r02->alto_norm);
fprintf(fp_staz[pagina],"*%dw%dc.offset: %f\n",
			num_w,*cont_f,p_r02->offset);
fprintf(fp_staz[pagina],"*%dw%dc.numeroInt: 2\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.numeroDec: 0\n",num_w,*cont_f);
 if(input_ago!=NULL)
   {
   if ( (sottotipo==INDIC_AGO_ERR) || (sottotipo==INDIC_AGO) )
      {
      fprintf(fp_staz[pagina],"*%dw%dc.varInput: %s\n",
              num_w,*cont_f,input_ago);
      }
   else
      {
      fprintf(fp_staz[pagina],"*%dw%dc.varInputCambioColore1: %s\n",
              num_w,*cont_f,input_ago);
      }
   }
*cont_f +=1;
}

