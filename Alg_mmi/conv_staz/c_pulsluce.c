/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)c_pulsluce.c	1.2\t6/16/93";
/*
   modulo c_pulsluce.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)c_pulsluce.c	1.2
*/
/*
	Compilazione dell' oggetto di tipo PULS_LUCE
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

extern  char riga [80];
extern  int nriga;

extern	char *scolori_oggetti[];
extern	char *stipo_perturb[];


extern FILE *fo;

int c_pulsluce(p_r02,sottotipo,pagina,num_w,cont_f,elenco,x,y)
TIPO_PULS_LUCE *p_r02;
int pagina,num_w,x,y,sottotipo;
int *cont_f;
char *elenco;
{
int i,lun,nstr,j,imu,index;
char colore_bottone[50];
char output[50];
char input_colore[50];
char input_blink[50];




/*
	legge il colore associato al  pulsante con luce
*/

printf("Ogetto PULS_LUCE\n\n");

  p_r02->tipo_oggetto=PULS_LUCE;
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,s_colore,strlen(s_colore)) || (string[1].stringa == NULL ))
					  errore(ERR_COLORE,riga);
  for (i=0; scolori_oggetti[i]!=NULL; i++)
    if (!strncmp(string[1].stringa,scolori_oggetti[i],strlen(scolori_oggetti[i]))) break;
  if (scolori_oggetti[i] == NULL) errore(ERR_COLORE,riga);
  p_r02->colore=i;
  strcpy(colore_bottone,scolori_oggetti[i]);
  fprintf(fo,"\n colore %s ",scolori_oggetti[i]);

/*
	legge la  variabile di output associata al pulsante, il modello
	di appartenenza e il tipo di perturbazione 
*/
  legge_riga( riga, &lun, &nriga);
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
  fprintf(fo,"\n output modello %d indice %d perturb %d",p_r02->out.nmod,p_r02->out.indice,p_r02->out.tipo_pert);

/*
   legge il nome della variabile e il modello della variabile INPUT_COLORE
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
        inserisce il pulsante nel file delle risorse
*/
fprintf(fp_staz[pagina],"*%dw%dc.x0: %d\n",num_w,*cont_f,x);
fprintf(fp_staz[pagina],"*%dw%dc.y0: %d\n",num_w,*cont_f,y);
fprintf(fp_staz[pagina],"*%dw%dc.width0: %d\n",num_w,*cont_f,W_PULSANTE);
fprintf(fp_staz[pagina],"*%dw%dc.height0: %d\n",num_w,*cont_f,H_PULSANTE);
fprintf(fp_staz[pagina],"*%dw%dc.borderWidth: 0\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.background: %s\n",num_w,*cont_f,SFONDO_STAZ);
fprintf(fp_staz[pagina],"*%dw%dc.tipoBt: 1\n",num_w,*cont_f);
fprintf(fp_staz[pagina],"*%dw%dc.colorLamp: %s\n",num_w,*cont_f,
                RetColore(colore_bottone));
fprintf(fp_staz[pagina],"*%dw%dc.actBg: %s\n",num_w,*cont_f,
                RetColoreBlink(colore_bottone));
fprintf(fp_staz[pagina],"*%dw%dc.normFg: black\n",num_w,*cont_f);
if(output!=NULL)
        fprintf(fp_staz[pagina],"*%dw%dc.varOutput: %s\n",
                num_w,*cont_f,output);

if(input_colore!=NULL)
	fprintf(fp_staz[pagina],"*%dw%dc.varInputColore: %s\n",
		num_w,*cont_f,input_colore);
if(input_blink!=NULL)
	fprintf(fp_staz[pagina],"*%dw%dc.varInputBlink: %s\n",
		num_w,*cont_f,input_blink);
sprintf(elenco,"%s %dw%dc Bottone",elenco,num_w,*cont_f);
*cont_f +=1;

}

