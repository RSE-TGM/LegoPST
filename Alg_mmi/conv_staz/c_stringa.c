/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)c_stringa.c	1.2\t6/16/93";
/*
   modulo c_stringa.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)c_stringa.c	1.2
*/
/*
	Compilazione dell' oggetto STRINGA_DESCR
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

int c_stringa(p_r02,sottotipo,pagina,num_w,cont_f,elenco,x,y)
TIPO_STRINGA_DESCR *p_r02;
int pagina,num_w,x,y,sottotipo;
int *cont_f;
char *elenco;
{
int lun, nstr;
char *px, saveriga[80];


  p_r02->tipo_oggetto=STRINGA_DESCR;

/*
	legge la descrizione della stringa
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
if(strlen(p_r02->etic)==0)
	return(0);
fprintf(fp_staz[pagina],"*%dw%dc.x0: %d\n",num_w,*cont_f,x);
fprintf(fp_staz[pagina],"*%dw%dc.y0: %d\n",num_w,*cont_f,y);
fprintf(fp_staz[pagina],"*%dw%dc.borderWidth: 0\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.labelText: %s\n",num_w,*cont_f,p_r02->etic);
if(sottotipo==0)
   {
   fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,20);
   fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,20);
   fprintf(fp_staz[pagina],"*%dw%dc.tipoLabel: 0\n",num_w,*cont_f);
   fprintf(fp_staz[pagina],"*%dw%dc.normalFont: %s\n",
		num_w,*cont_f,FONT_PICCOLO);
   fprintf(fp_staz[pagina],"*%dw%dc.background: %s\n",
		num_w,*cont_f,SFONDO_LABEL);
   }
else
if(sottotipo==STRINGA_TESTO)
   {
   fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,20);
   fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,20);
   fprintf(fp_staz[pagina],"*%dw%dc.tipoLabel: 0\n",num_w,*cont_f);
   fprintf(fp_staz[pagina],"*%dw%dc.normalFont: %s\n",
		num_w,*cont_f,FONT_GRANDE);
   fprintf(fp_staz[pagina],"*%dw%dc.background: %s\n",
		num_w,*cont_f,SFONDO_STAZ);
   }
else
if(sottotipo==STRINGA_ETIC)
   {
   fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,20);
   fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,20);
   fprintf(fp_staz[pagina],"*%dw%dc.tipoLabel: 0\n",num_w,*cont_f);
   fprintf(fp_staz[pagina],"*%dw%dc.normalFont: %s\n",
		num_w,*cont_f,FONT_PICCOLO);
   fprintf(fp_staz[pagina],"*%dw%dc.background: %s\n",
		num_w,*cont_f,SFONDO_LABEL);
   }
else
if(sottotipo==STRINGA_ETIC_BIG)
   {
   fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,20);
   fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,20);
   fprintf(fp_staz[pagina],"*%dw%dc.tipoLabel: 0\n",num_w,*cont_f);
   fprintf(fp_staz[pagina],"*%dw%dc.normalFont: %s\n",
		num_w,*cont_f,FONT_GRANDE);
   fprintf(fp_staz[pagina],"*%dw%dc.background: %s\n",
		num_w,*cont_f,SFONDO_STAZ);
   }
else
   fprintf(stderr,"LABEL: sottotipo [%d] inesistente\n",sottotipo);
sprintf(elenco,"%s %dw%dc Label",elenco,num_w,*cont_f);
*cont_f +=1;
}

