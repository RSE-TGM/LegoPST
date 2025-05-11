/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)c_setval.c	1.2\t6/16/93";
/*
   modulo c_setval.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)c_setval.c	1.2
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
#include "conv_mmi.h"


extern FILE *fp_staz[MAX_PAG];
extern int numero_el[MAX_PAG];
extern char nomi_ogg_pag[MAX_PAG][MAX_LUN_RIGA_ELENCO_WID];


extern float get_valore();
// void legge_riga( char *riga, int *lun, int *nriga );
// void separa_str( char *riga, int lun, int nstr, STRIN_ST strin[]);

extern  STRIN_ST string[];

extern  char riga [80];
extern  int nriga;

extern	char *stipo_perturb[];
extern	char *scolori_oggetti[];


extern FILE *fo;

int c_setval(p_r02,sottotipo,pagina,num_w,cont_f,elenco,x,y)
TIPO_SET_VALORE *p_r02;
int pagina,num_w,x,y,sottotipo;
int *cont_f;
char *elenco;
{
int i,lun,nstr,j,imu,index;
char *px,saveriga[100];
float valore,offset;
char output[100];
char output_save[100];
char rltarget[100];
char input[100];
char inibiz[100];

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
printf("\nriga_output=%s\n",riga);
  separa_str( riga, lun, nstr=5, string);
  if (strncmp(string[0].stringa,s_out,strlen(s_out)))
						errore(ERR_OUTPUT,riga);
  if ((string[1].stringa == NULL) && (string[2].stringa == NULL ) &&
                        (string[3].stringa == NULL ))
  {
        p_r02->out.nmod= -1;
        p_r02->out.indice= -1;
        p_r02->out.tipo_pert= -1;
        output[0]=0;
  }
  else
  {
printf("\nriga_output=OK\n");
  check_model(string[2].stringa,&imu);
  check_input(string[1].stringa,imu,&index);
 if (string[3].stringa == NULL ) errore(ERR_PERTURB,riga);
  for (i=1; stipo_perturb[i]!=NULL; i++)
 if (!strcmp(string[3].stringa,stipo_perturb[i])) break;
  if (stipo_perturb[i] == NULL) errore(ERR_PERTURB,riga);
  p_r02->out.nmod=imu;
  p_r02->out.indice=index;
  p_r02->out.tipo_pert=i;
  if (string[4].stringa == NULL ) 
  {
  p_r02->out.value=1.0;
  strcpy(output,CostruisciRigaOutput(string[1].stringa,string[2].stringa,
                string[3].stringa,"1.0"));
  }
 else
  {
  p_r02->out.value=get_valore(string[4].stringa);
  strcpy(output,CostruisciRigaOutput(string[1].stringa,string[2].stringa,
                string[3].stringa,string[4].stringa));
  }
strcpy(output_save,output);
printf("\nriga_output:output=%s\n",output);

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
        rltarget[0]=0;
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
  strcpy(rltarget,CostruisciRigaOutput(string[1].stringa,string[2].stringa,
                string[3].stringa,string[4].stringa));
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
        input[0];
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input=index;
  strcpy(input,CostruisciRigaInput(string[1].stringa,string[2].stringa,0));
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
        inibiz[0]=0;
  }
  else
  {
  	check_model(string[2].stringa,&imu);
	check_output(string[1].stringa,imu,&index);
	p_r02->inibiz=index;
        strcpy(inibiz,CostruisciRigaInput(string[1].stringa,string[2].stringa,
                p_r02->inibiz));
  }
  fprintf(fo,"\n variabile inibiz %d",p_r02->inibiz);
/*
        inserisce il SetValore nel file delle risorse
*/
fprintf(fp_staz[pagina],"*%dw%dc.x0: %d\n",num_w,*cont_f,x);
fprintf(fp_staz[pagina],"*%dw%dc.y0: %d\n",num_w,*cont_f,y);
fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,70);
fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,25);
fprintf(fp_staz[pagina],"*%dw%dc.borderWidth: 0\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.numeroInt: 2\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.numeroDec: 1\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.background: %s\n",num_w,*cont_f,SFONDO_STAZ);
fprintf(fp_staz[pagina],"*%dw%dc.normalFont: %s\n",num_w,*cont_f,FONT_PICCOLO);
fprintf(fp_staz[pagina],"*%dw%dc.normBg: %s\n",num_w,*cont_f,SFONDO_STAZ);
fprintf(fp_staz[pagina],"*%dw%dc.scalamento: %f\n",num_w,*cont_f,p_r02->scalam);
fprintf(fp_staz[pagina],"*%dw%dc.offset: %f\n",num_w,*cont_f,p_r02->offset);
strcpy(output,output_save);
if(output!=NULL)
printf("\nriga_output:output_finale=%s\n",output);
        fprintf(fp_staz[pagina],"*%dw%dc.varOutput: %s\n",
                num_w,*cont_f,output);
if(input!=NULL)
        fprintf(fp_staz[pagina],"*%dw%dc.varInput: %s\n",
                num_w,*cont_f,input);
if(inibiz!=NULL)
        fprintf(fp_staz[pagina],"*%dw%dc.varInibitSetV: %s\n",
                num_w,*cont_f,inibiz);
if(rltarget!=NULL)
        fprintf(fp_staz[pagina],"*%dw%dc.varRltarget: %s\n",
                num_w,*cont_f,rltarget);
sprintf(elenco,"%s %dw%dc SetValore",elenco,num_w,*cont_f);
*cont_f +=1;
/*
        inserisce la label nel file delle risorse
*/
if(strlen(p_r02->etic)>0)
  {
  fprintf(fp_staz[pagina],"*%dw%dc.x0: %d\n",num_w,*cont_f,x+70);
  fprintf(fp_staz[pagina],"*%dw%dc.y0: %d\n",num_w,*cont_f,y+3);
  fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,W_LED);
  fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,H_LED);
  fprintf(fp_staz[pagina],"*%dw%dc.tipoLabel: 0\n",num_w,*cont_f);
  fprintf(fp_staz[pagina],"*%dw%dc.borderWidth: 0\n",num_w,*cont_f);
  fprintf(fp_staz[pagina],"*%dw%dc.normalFont: %s\n",num_w,*cont_f,FONT_PICCOLO);
  fprintf(fp_staz[pagina],"*%dw%dc.labelText: %s\n",num_w,*cont_f,p_r02->etic);
  fprintf(fp_staz[pagina],"*%dw%dc.background: %s\n",num_w,*cont_f,SFONDO_STAZ);
  sprintf(elenco,"%s %dw%dc Label",elenco,num_w,*cont_f);
  *cont_f +=1;
  }

}

