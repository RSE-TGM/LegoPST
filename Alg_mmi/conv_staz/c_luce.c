/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)c_luce.c	1.2\t6/16/93";
/*
   modulo c_luce.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)c_luce.c	1.2
*/
/*
	Compilazione tipo oggetto LUCE
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


extern FILE *fo;


int c_luce(p_r02,sottotipo,pagina,num_w,cont_f,elenco,x,y)
TIPO_LUCE *p_r02;
int pagina,num_w,x,y,sottotipo;
int *cont_f;
char *elenco;
{
int i,lun,nstr,j,imu,index;
char *px, saveriga[80];
char colore_led[50];
char input_colore[50];
char input_blink[50];


/*
	legge il colore associato  alla luce
*/

  p_r02->tipo_oggetto=LUCE;
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,s_colore,strlen(s_colore)) || (string[1].stringa == NULL ))
					  errore(ERR_COLORE,riga);
  for (i=0; scolori_oggetti[i]!=NULL; i++)
    if (!strncmp(string[1].stringa,scolori_oggetti[i],strlen(scolori_oggetti[i]))) break;
  if (scolori_oggetti[i] == NULL) errore(ERR_COLORE,riga);
  p_r02->colore=i;
  strcpy(colore_led,scolori_oggetti[i]);
  fprintf(fo,"\n colore %s ",scolori_oggetti[i]);

/*
	legge il nome della variabile e il modello della variabile INPUT
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=4, string);
  if (strncmp(string[0].stringa,s_inp,strlen(s_inp)))
						errore(ERR_INPUT,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ))
  {
        p_r02->input_colore = -1;
        input_colore[0]=0;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input_colore=index;
  p_r02->neg_colore= is_neg(string[3].stringa);
  strcpy(input_colore,CostruisciRigaInput(string[1].stringa,string[2].stringa,
                p_r02->neg_colore));
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
        input_blink[0]=0;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input_blink=index;
  p_r02->neg_blink= is_neg(string[3].stringa);
  strcpy(input_blink,CostruisciRigaInput(string[1].stringa,string[2].stringa,
                p_r02->neg_blink));
  }
  fprintf(fo,"\n variabile input_blink %d",p_r02->input_blink);
  /*
        inserisce il led nel file delle risorse
*/
fprintf(fp_staz[pagina],"*%dw%dc.x0: %d\n",num_w,*cont_f,x);
fprintf(fp_staz[pagina],"*%dw%dc.y0: %d\n",num_w,*cont_f,y);
fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,W_LUCE);
fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,H_LUCE);
fprintf(fp_staz[pagina],"*%dw%dc.borderWidth: 1\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.tipoLed: 0\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.colorNorm: %s\n",num_w,*cont_f,
                RetColore(colore_led));
fprintf(fp_staz[pagina],"*%dw%dc.colorBlink: %s\n",num_w,*cont_f,
                RetColoreBlink(colore_led));
if(input_colore!=NULL)
   fprintf(fp_staz[pagina],"*%dw%dc.varInputColore: %s\n",
        num_w,*cont_f,input_colore);
if(input_blink!=NULL)
   fprintf(fp_staz[pagina],"*%dw%dc.varInputBlink: %s\n",
        num_w,*cont_f,input_blink);
sprintf(elenco,"%s %dw%dc Led",elenco,num_w,*cont_f);
*cont_f +=1;
}

