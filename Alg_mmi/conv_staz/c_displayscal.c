/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)c_displayscal.c	1.2\t6/16/93";
/*
   modulo c_displayscal.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)c_displayscal.c	1.2
*/
/*
	Compilazione dell' oggetto DISPLAY_SCALATO
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

extern	char *scolori_oggetti[];

extern	FILE *fo;

int c_displayscal(p_r02,sottotipo,pagina,num_w,cont_f,elenco,x,y)
TIPO_DISPLAY_SCALATO *p_r02;
int pagina,num_w,x,y,sottotipo;
int *cont_f;
char *elenco;
{
int lun, nstr, imu, index;
float valore,offset;
char *px, saveriga[80];
char input_ago[50];



  p_r02->tipo_oggetto=DISPLAY_SCALATO;

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
   p_r02->input=index;
   strcpy(input_ago,CostruisciRigaInput(string[1].stringa,string[2].stringa,
                     0));
  }
  fprintf(fo,"\n variabile input %d",p_r02->input );
/*
 legge lo scalamento per la variabile da visualizzare
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,s_scalam,strlen (s_scalam)) ||
                          (string[1].stringa == NULL )) errore(ERR_SCAL,riga);
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
fprintf(fp_staz[pagina],"*%dw%dc.x0: %d\n",num_w,*cont_f,x);
fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,100);
fprintf(fp_staz[pagina],"*%dw%dc.y0: %d\n",num_w,*cont_f,y);
fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,35);
fprintf(fp_staz[pagina],"*%dw%dc.tipoInd: 2\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.assRotate: 0\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.borderWidth: 1\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.normFg: black\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.background: %s\n",num_w,*cont_f,SFONDO_STAZ);
fprintf(fp_staz[pagina],"*%dw%dc.normalFont: %s\n",num_w,*cont_f,FONT_GRANDE);
fprintf(fp_staz[pagina],"*%dw%dc.agoFg: black\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.scalamento: %f\n",num_w,*cont_f,
			p_r02->scalam);
fprintf(fp_staz[pagina],"*%dw%dc.valoreMinimo: -10000\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.valoreMassimo: 10000\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.offset: %f\n",num_w,*cont_f,
			p_r02->offset);
fprintf(fp_staz[pagina],"*%dw%dc.numeroInt: 2\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.numeroDec: 1\n",num_w,*cont_f);
if(input_ago!=NULL)
  fprintf(fp_staz[pagina],"*%dw%dc.varInputCambioColore1: %s\n",
        num_w,*cont_f,input_ago);
sprintf(elenco,"%s %dw%dc Indic",elenco,num_w,*cont_f);
*cont_f +=1;

}
