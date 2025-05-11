/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)c_selet.c	1.2\t6/16/93";
/*
   modulo c_selet.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)c_selet.c	1.2
*/
/*
	Compilazione dell' oggetto tipo  SELETTORE
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
extern float get_valore();

extern  STRIN_ST string[];

extern FILE *fo;

extern  char riga [80];
extern  int nriga;

extern	char *stipo_perturb[];


int c_selet(p_r02,sottotipo,pagina,num_w,cont_f,elenco,x,y)
TIPO_SELETTORE *p_r02;
int pagina,num_w,x,y,sottotipo;
int *cont_f;
char *elenco;
{
int i,lun,nstr,j,imu,index;
char *px, saveriga[80];
char output[50];
char input_pos[50];


  p_r02->tipo_oggetto=SELETTORE;

/*
	legge le  descrizioni associate al selettore
*/
  legge_riga( riga, &lun, &nriga);
  strcpy(saveriga,riga); 
  separa_str( riga, lun, nstr=10, string);
  if (strncmp(string[0].stringa,s_etic,strlen(s_etic))) errore(ERR_ETIC,riga);
  memset(p_r02->etic_0,0,LUNG_ETICHETTA+1);
  if (strlen(string[1].stringa)) 
  {
  	px=strstr(saveriga,string[1].stringa);
fprintf(fo,"\n px %s \n",px);
	strncpy(p_r02->etic_0,px,LUNG_ETICHETTA);
  }
  fprintf(fo,"\n etichetta 0 %s ",p_r02->etic_0);
  legge_riga( riga, &lun, &nriga);
  strcpy(saveriga,riga);
  separa_str( riga, lun, nstr=10, string);
  if (strncmp(string[0].stringa,s_etic,strlen(s_etic))) errore(ERR_ETIC,riga);
  memset(p_r02->etic_1,0,LUNG_ETICHETTA+1);
  if (strlen(string[1].stringa))
  {
        px=strstr(saveriga,string[1].stringa);
        strncpy(p_r02->etic_1,px,LUNG_ETICHETTA);
  }
  fprintf(fo,"\n etichetta 1 %s ",p_r02->etic_1);

/*
        legge la  variabile di output associata al pulsante, il modello
        di appartenenza e il tipo di perturbazione
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=5, string);
  if (strncmp(string[0].stringa,s_out,strlen(s_out))) errore(ERR_OUTPUT,riga);
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
  strcpy(output,CostruisciRigaOutput(string[1].stringa,string[2].stringa,
                string[3].stringa,string[4].stringa));
  }
  fprintf(fo,"\n output modello %d indice %d perturb %d",p_r02->out.nmod,p_r02->out.
indice,p_r02->out.tipo_pert);

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
        input_pos[0]=0;
  }
  else
  {
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  p_r02->input=index;
  strcpy(input_pos,CostruisciRigaInput(string[1].stringa,string[2].stringa,
                p_r02->input));
  }
  fprintf(fo,"\n variabile input %d",p_r02->input);
/*
        inserisce il selettore nel file delle risorse
*/
fprintf(fp_staz[pagina],"*%dw%dc.x0: %d\n",num_w,*cont_f,x);
fprintf(fp_staz[pagina],"*%dw%dc.y0: %d\n",num_w,*cont_f,y);
fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,W_SELETTORE);
fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,H_SELETTORE);
fprintf(fp_staz[pagina],"*%dw%dc.borderWidth: 0\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.background: %s\n",num_w,*cont_f,SFONDO_STAZ);
fprintf(fp_staz[pagina],"*%dw%dc.normBg: %s\n",num_w,*cont_f,SFONDO_STAZ);
if(sottotipo==SELET_TIPO_A)
  fprintf(fp_staz[pagina],"*%dw%dc.tipoSel: 0\n",num_w,*cont_f);
else
  fprintf(fp_staz[pagina],"*%dw%dc.tipoSel: 1\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.SelettoreFg: %s\n",num_w,*cont_f,
                RetColoreBlink("GRIGIO"));
if(output!=NULL)
        fprintf(fp_staz[pagina],"*%dw%dc.varOutput: %s\n",
                num_w,*cont_f,output);
if(input_pos!=NULL)
	fprintf(fp_staz[pagina],"*%dw%dc.varInput: %s\n",
                num_w,*cont_f,input_pos);
sprintf(elenco,"%s %dw%dc Selettore",elenco,num_w,*cont_f);
*cont_f +=1;
/*
        inserisce le label nel file delle risorse
*/
if(strlen(p_r02->etic_0)>0)
  {
  fprintf(fp_staz[pagina],"*%dw%dc.x0: %d\n",num_w,*cont_f,x-35);
  fprintf(fp_staz[pagina],"*%dw%dc.y0: %d\n",num_w,*cont_f,y-10);
  fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,10);
  fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,10);
  fprintf(fp_staz[pagina],"*%dw%dc.tipoLabel: 0\n",num_w,*cont_f);
  fprintf(fp_staz[pagina],"*%dw%dc.borderWidth: 0\n",num_w,*cont_f);
  fprintf(fp_staz[pagina],"*%dw%dc.normalFont: fixed\n",num_w,*cont_f);
  fprintf(fp_staz[pagina],"*%dw%dc.labelText: %s\n",num_w,*cont_f,p_r02->etic_0);
  fprintf(fp_staz[pagina],"*%dw%dc.background: %s\n",num_w,*cont_f,SFONDO_STAZ);
  sprintf(elenco,"%s %dw%dc Label",elenco,num_w,*cont_f);
  *cont_f +=1;
  }
if(strlen(p_r02->etic_1)>0)
  {
  fprintf(fp_staz[pagina],"*%dw%dc.x0: %d\n",num_w,*cont_f,x+25);
  fprintf(fp_staz[pagina],"*%dw%dc.y0: %d\n",num_w,*cont_f,y-10);
  fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,10);
  fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,10);
  fprintf(fp_staz[pagina],"*%dw%dc.tipoLabel: 0\n",num_w,*cont_f);
  fprintf(fp_staz[pagina],"*%dw%dc.borderWidth: 0\n",num_w,*cont_f);
  fprintf(fp_staz[pagina],"*%dw%dc.normalFont: fixed\n",num_w,*cont_f);
  fprintf(fp_staz[pagina],"*%dw%dc.labelText: %s\n",num_w,*cont_f,p_r02->etic_0);
  fprintf(fp_staz[pagina],"*%dw%dc.background: %s\n",num_w,*cont_f,SFONDO_STAZ);
  sprintf(elenco,"%s %dw%dc Label",elenco,num_w,*cont_f);
  *cont_f +=1;
  }
}

