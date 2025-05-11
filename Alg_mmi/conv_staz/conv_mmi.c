#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>

#include "sim_param.h"
#include "sim_types.h"
#include "xstaz.h"
#include "conv_mmi.h"
#include "compstaz.inc"


extern FILE *fp_staz[MAX_PAG];
extern int numero_el[MAX_PAG];
extern int last_num[MAX_PAG];
extern int max_x[MAX_PAG];
extern int max_y[MAX_PAG];
extern char nomi_ogg_pag[MAX_PAG][MAX_LUN_RIGA_ELENCO_WID];

int ApriFileMMI(int num,char* nome,char *descrizione)
{
char nome_file[50];

	printf("Apro il file %s.pag numero %d\n",nome,num+1);
	sprintf(nome_file,"%s.pag",nome);
	
	fp_staz[num]=fopen(nome_file,"w");
	numero_el[num]=0;
	nomi_ogg_pag[num][0]=0;
	fprintf(fp_staz[num],"*top_destrizione:    %s\n",descrizione);
	fprintf(fp_staz[num],"*top_x: 0\n");
	fprintf(fp_staz[num],"*top_y: 0\n");
	fprintf(fp_staz[num],"*top_width: %d\n",
			(max_x[num]+3)*WIDTH_COMPOSITE);
	fprintf(fp_staz[num],"*top_height: %d\n",
			(max_y[num]+2)*HEIGHT_COMPOSITE);
	fprintf(fp_staz[num],"*top_tipo: Stazioni\n");
	fprintf(fp_staz[num],"*drawing_width: %d\n",
			(max_x[num]+3)*WIDTH_COMPOSITE);
	fprintf(fp_staz[num],"*drawing_height: %d\n",
			(max_y[num]+2)*HEIGHT_COMPOSITE);
	fprintf(fp_staz[num],"*drawing_background: %s\n",SFONDO_WINDOW);
}

void ChiudiFileMMI()
{
int i;

for(i=0;i<MAX_PAG;i++)
	if(fp_staz[i]!=NULL)
		{
		fprintf(fp_staz[i],
			"*elenco_wid0: %s\n",nomi_ogg_pag[i]);
		fprintf(fp_staz[i],
			"*num_widget: %d\n",numero_el[i]);
		fprintf(fp_staz[i],
			"*nextnum: %d\n",last_num[i]+1);
		fclose(fp_staz[i]);
		}
}

void AggiungiOggetto(int pagina,int num)
{
sprintf(nomi_ogg_pag[pagina],"%s %dw XlComposite",nomi_ogg_pag[pagina],num);
last_num[pagina]=num;
++numero_el[pagina];
}


int ScriviComposite(int pagina, int num, int x, int y, int width, int height)
{

y=max_y[pagina]-y-height+2;

fprintf(fp_staz[pagina],"*%dw.x0:    %d\n",num,x*WIDTH_COMPOSITE);
fprintf(fp_staz[pagina],"*%dw.y0:    %d\n",num,y*HEIGHT_COMPOSITE);
fprintf(fp_staz[pagina],"*%dw.width0:    %d\n",num,
		width*WIDTH_COMPOSITE-2*BORDER_COMPOSITE);
fprintf(fp_staz[pagina],"*%dw.height0:    %d\n",num,
		height*HEIGHT_COMPOSITE-2*BORDER_COMPOSITE);
fprintf(fp_staz[pagina],"*%dw.borderWidth:    %d\n",num,BORDER_COMPOSITE);
fprintf(fp_staz[pagina],"*%dw.background: %s\n",num,SFONDO_STAZ);


}

int RegistraElencoFigliComposite(int pagina,  int num, int num_figli,
	char *elenco_figli)
{
fprintf(fp_staz[pagina],"*%dw.numFigli:   %d\n",num,num_figli);
fprintf(fp_staz[pagina],"*%dw.listChildren:   %s\n",num,elenco_figli);
}

char *RetColore(char *old)
{
int j;
   for (j=0; conv_colori[j].old_colore!=NULL; j++)
        if (!strcmp(old,conv_colori[j].old_colore))
                break;
return(conv_colori[j].colore);
}

char *RetColoreBlink(char *old)
{
int j;
   for (j=0; conv_colori[j].old_colore!=NULL; j++)
        if (!strcmp(old,conv_colori[j].old_colore))
                break;
return(conv_colori[j].colore_blink);
}

char *CostruisciRigaInput(char *var, char *mod,int neg)
{
char ret[100];
	
if(neg==0)
	sprintf(ret,"%s BLOCCO %s NOP 1.0 0.0 ---",var,mod);
else
	sprintf(ret,"%s BLOCCO %s NOT 1.0 0.0 ---",var,mod);


return(ret);
}

char *CostruisciRigaOutput(char *var, char *mod,char *pert,char *val)
{
char ret[100];
float app_float;

if(val==NULL)
	app_float=0;
else
	app_float=atof(val);
	
if(strcmp(pert,"STEP")==0)
	sprintf(ret,"%s BLOCCO %s PERT_SCALINO %f 0.0 1.0 0.0 ---",
		var,mod,app_float);
if(strcmp(pert,"IMPULSO")==0)
	sprintf(ret,"%s BLOCCO %s PERT_IMPULSO 1.0 0.0 1.0 0.0 ---",var,mod);
if(strcmp(pert,"NEGAZIONE")==0)
	sprintf(ret,"%s BLOCCO %s PERT_NOT 0.0 0.0 1.0 0.0 ---",var,mod);
if(strcmp(pert,"UP_DOWN")==0)
	sprintf(ret,"%s BLOCCO %s PERT_UP_DOWN 0.0 0.0 1.0 0.0 ---",var,mod);


return(ret);
}

void CercoPosMax(FILE *fp)
{
char riga [80];
int lun;
int nriga=0;
STRIN_ST string[10];
int pagina;
int x,y;
int nstr;

for (;;)
  {
  legge_riga( riga, &lun, &nriga);
  if(strncmp(riga,"END_OF_FILE",11)==0)
	break;
  if(strncmp(riga,"PAGINA",6)==0)
	{
        separa_str( riga, lun, nstr=2, string);
	if(string[1].stringa == NULL)
		continue;
	pagina = atoi (string[1].stringa);
        legge_riga( riga, &lun, &nriga);
        separa_str( riga, lun, nstr=3, string);
	x=atoi(string[1].stringa);
	y=atoi(string[2].stringa);
	printf("PAGINA = %d   pos_x=%d pos_y=%d\n",pagina,x,y);
        if(max_x[pagina-1]<x)
		max_x[pagina-1]=x;
        if(max_y[pagina-1]<y)
		max_y[pagina-1]=y;
	}
  }
rewind(fp);
}
