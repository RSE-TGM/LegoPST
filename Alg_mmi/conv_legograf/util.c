/**********************************************************************
*
*       C Source:               util.c
*       Subsystem:              3
*       Description:
*       %created_by:    famgr %
*       %date_created:  Wed Apr 10 10:34:45 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: util.c,2 %  (%full_filespec: 3,csrc,util.c,2 %)";
#endif
/*
   modulo util.c
   tipo 
   release 1.3
   data 3/20/95
   reserved @(#)util.c	1.3
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)util.c	1.3\t3/20/95";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"


extern char nome_file_input[100];
extern char path_ico[255];
extern char path_pag[255];

int num_obj=0;
char elenco_wid[8000];

void read_polyline(FILE *,FILE *);
void read_solid(FILE *,FILE *);
void read_circle(FILE *,FILE *);
void sistema_file(char *);
void legge_sfondo(FILE *fp, char *nome);
void insert_icona(FILE *,FILE *);
void insert_text(FILE *,FILE *);
static int dim_x_icona(char*);
static int dim_y_icona(char*);



int legge_riga(fp,riga)
 char riga [MAXRIGA];
 FILE *fp;
{
 int c, k;

 for (k=0; k<MAXRIGA; k++)
   riga[k] = (char)NULL;

 for (k=0; ((c = fgetc( fp)) != '\n')&&(c>0); k++)
  riga[k]=c;
 if (c<0)
	{
 	for (k=0; k<MAXRIGA; k++)
   		riga[k] = (char)NULL;
	return(-1);
	}

 riga[k]=(char)NULL;

return(1);
}

void skip_header(fp)
FILE *fp;
{
char riga[MAXRIGA];

legge_riga(fp,riga);
while(strcmp(riga,"ENDSEC")!=0)
	legge_riga(fp,riga);

legge_riga(fp,riga);
while(strcmp(riga,"ENDSEC")!=0)
	legge_riga(fp,riga);
}


void legge_icona(fp,nome)
FILE *fp;
char *nome;
{
char riga[MAXRIGA];
FILE *fp_icon;
char nome_file[50];
int esiste_icona=0;


sprintf(nome_file,"%s/%s.ico",path_ico,nome);
fp_icon=fopen(nome_file,"r");
if(fp_icon>0)
	{
	fclose(fp_icon);
	esiste_icona=1;
	}
else
	fp_icon=fopen(nome_file,"w");

if(esiste_icona==0)
	printf("Scrittura icona %s.ico\n",nome);

legge_riga(fp,riga);
while(strcmp(riga,"ENDBLK")!=0)
	{
	if(esiste_icona==0)
		{
		if(strcmp(riga,"POLYLINE")==0)
	        	read_polyline(fp,fp_icon);
		if(strcmp(riga,"SOLID")==0)
	        	read_solid(fp,fp_icon);
		if(strcmp(riga,"CIRCLE")==0)
	        	read_circle(fp,fp_icon);
		}
	legge_riga(fp,riga);
	}
if(esiste_icona==0)
	{
	fclose(fp_icon);
	sistema_file(nome_file);
	}
}


void read_polyline(fp_in,fp_out)
FILE *fp_in;
FILE *fp_out;
{
char riga[MAXRIGA];
int codice_colore;
int num_punti=0;
int x[200],y[200];
int i;
int min_x=0;
int min_y=0;

/*
	skip 5 righe
*/
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);

/*
	legge il colore
*/
legge_riga(fp_in,riga);
codice_colore=atoi(riga);

fprintf(fp_out,"wi_c 0\n");
fprintf(fp_out,"st_c 0\n");
fprintf(fp_out,"fg_c %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"bg_c %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"fg_f %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"fill 0\n");

/*
	skip 7 righe
*/
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);

while(strcmp(riga,"SEQEND")!=0)
	{
	if(strcmp(riga,"VERTEX")==0)
		{
		legge_riga(fp_in,riga);
		legge_riga(fp_in,riga);
		legge_riga(fp_in,riga);
		legge_riga(fp_in,riga);
		x[num_punti]=(int)(atof(riga)*FATTORE_SCALA);
		legge_riga(fp_in,riga);
		legge_riga(fp_in,riga);
		y[num_punti]= -(int)(atof(riga)*FATTORE_SCALA);
		++num_punti;
		}
	legge_riga(fp_in,riga);
	}
fprintf(fp_out,"poli %d",num_punti+1);
for(i=0;i<num_punti;i++)
	fprintf(fp_out," %d %d",x[i],y[i]);

/*
	chiusura della polyline
*/
fprintf(fp_out," %d %d",x[0],y[0]);
fprintf(fp_out,"\n");
}

void sistema_file(nome)
char *nome;
{
FILE *fp;
FILE *fp_app;
int num=0;
char riga[MAXRIGA];
int min_x=0;
int min_y=0;
int max_x=0;
int max_y=0;
int np,i;
int x1,y1,x2,y2,a,b;
char comando[100];


fp=fopen(nome,"r");

while(legge_riga(fp,riga)>0)
	{
	legge_riga(fp,riga);
	legge_riga(fp,riga);
	legge_riga(fp,riga);
	legge_riga(fp,riga);
	legge_riga(fp,riga);
        fscanf(fp,"%s",riga);
        switch(riga[0])
                {
                case 'p':
			{
			fscanf(fp,"%d",&np);
			for(i=0;i<np;i++)
                                {
                                fscanf(fp,"%d %d",&x1,&y1);
				if(x1<min_x) min_x=x1;
				if(y1<min_y) min_y=y1;
				if(x1>max_x) max_x=x1;
				if(y1>max_y) max_y=y1;
				}
			break;
			}
                case 'c':
			{
                        fscanf(fp,"%d %d %d %d %d %d",
				&x1,&y1,&x2,&y2,&a,&b);
				if(x1<min_x) min_x=x1;
				if(y1<min_y) min_y=y1;
				if(x1>max_x) max_x=x1;
				if(y1>max_y) max_y=y1;
				if(x2<min_x) min_x=x2;
				if(y2<min_y) min_y=y2;
				if(x2>max_x) max_x=x2;
				if(y2>max_y) max_y=y2;
			break;
			}
		}
	++num;
        fscanf(fp,"\n");
	}
fclose(fp);
sprintf(comando,"mv %s _xxx.ico",nome);
system(comando);
fp_app=fopen("_xxx.ico","r");
fp=fopen(nome,"w");
fprintf(fp,"x_min_d %d\n",min_x);
fprintf(fp,"y_min_d %d\n",min_y);
fprintf(fp,"x_max_d %d\n",max_x+1);
fprintf(fp,"y_max_d %d\n",max_y+1);
fprintf(fp,"num_d %d\n",num);
while(legge_riga(fp_app,riga)>0)
	fprintf(fp,"%s\n",riga);
fclose(fp);
fclose(fp_app);
sprintf(comando,"rm _xxx.ico");
system(comando);
}

void read_solid(fp_in,fp_out)
FILE *fp_in;
FILE *fp_out;
{
char riga[MAXRIGA];
int codice_colore;
int x1,y1,x2,y2,x3,y3,x4,y4;

/*
	skip 3 righe
*/
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);

/*
	legge il colore
*/
legge_riga(fp_in,riga);
codice_colore=atoi(riga);

fprintf(fp_out,"wi_c 0\n");
fprintf(fp_out,"st_c 0\n");
fprintf(fp_out,"fg_c %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"bg_c %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"fg_f %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"fill 1\n");

/*
	legge i campi numerici
*/
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
x1=(int)(atof(riga)*FATTORE_SCALA);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
y1= -(int)(atof(riga)*FATTORE_SCALA);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
x2=(int)(atof(riga)*FATTORE_SCALA);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
y2= -(int)(atof(riga)*FATTORE_SCALA);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
x3=(int)(atof(riga)*FATTORE_SCALA);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
y3= -(int)(atof(riga)*FATTORE_SCALA);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
x4=(int)(atof(riga)*FATTORE_SCALA);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
y4= -(int)(atof(riga)*FATTORE_SCALA);

fprintf(fp_out,"poli 4 %d %d %d %d %d %d %d %d\n",
		x1,y1, x2,y2, x4,y4, x3,y3);
/*******
printf("SOLID  (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n",x1,y1, x2,y2, x3,y3, x4,y4);
********/
}

void read_circle(fp_in,fp_out)
FILE *fp_in;
FILE *fp_out;
{
char riga[MAXRIGA];
int codice_colore;
int x1,y1,r;

/*
	skip 3 righe
*/
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);

/*
	legge il colore
*/
legge_riga(fp_in,riga);
codice_colore=atoi(riga);

fprintf(fp_out,"wi_c 0\n");
fprintf(fp_out,"st_c 0\n");
fprintf(fp_out,"fg_c %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"bg_c %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"fg_f %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"fill 0\n");

/*
	legge i campi numerici
*/
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
x1=(int)(atof(riga)*FATTORE_SCALA);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
y1= -(int)(atof(riga)*FATTORE_SCALA);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
r=(int)(atof(riga)*FATTORE_SCALA);

fprintf(fp_out,"circ %d %d %d %d 0 23040\n",
	x1-r,y1-r,x1+r,y1+r);
}


void legge_sfondo(fp,nome)
FILE *fp;
char *nome;
{
char riga[MAXRIGA];
FILE *fp_sfondo;
FILE *fp_pag;
char nome_file[50];

printf("SFONDO %s\n",nome);

sprintf(nome_file,"%s/%s.pag",path_pag,nome);
fp_pag=fopen(nome_file,"w");
/*
	inserisce le righe iniziali
*/
fprintf(fp_pag,"*top_x: 0\n");
fprintf(fp_pag,"*top_y: 0\n");
fprintf(fp_pag,"*top_width:     1500\n");
fprintf(fp_pag,"*top_height:     1500\n");
fprintf(fp_pag,"*top_tipo:     Sinottico\n");
fprintf(fp_pag,"*top_descrizione:     Sinottico prov. da Legograf\n");
fprintf(fp_pag,"*drawing_width: 1500\n");
fprintf(fp_pag,"*drawing_height: 1500\n");
fprintf(fp_pag,"*drawing_background: %s\n",BACKGROUND);
fprintf(fp_pag,"*nomePag:  Sinottico prov. da Regograf\n");
fprintf(fp_pag,"*tagPag: 00\n");
fprintf(fp_pag,"*numTagReg:  00\n");
fprintf(fp_pag,"*nextTagReg: 00\n");
fprintf(fp_pag,"*refresh_freq: 10\n");

elenco_wid[0]=0;

sprintf(nome_file,"%s/%s.bkg",path_pag,nome);
fp_sfondo=fopen(nome_file,"w");

legge_riga(fp,riga);
while(strcmp(riga,"EOF")!=0)
	{
	if(strcmp(riga,"POLYLINE")==0)
	        read_polyline(fp,fp_sfondo);
	if(strcmp(riga,"SOLID")==0)
	        read_solid(fp,fp_sfondo);
	if(strcmp(riga,"CIRCLE")==0)
	        read_circle(fp,fp_sfondo);
	if(strcmp(riga,"INSERT")==0)
	        insert_icona(fp,fp_pag);
	if(strcmp(riga,"TEXT")==0)
	        insert_text(fp,fp_pag);
	legge_riga(fp,riga);
	}
fclose(fp_sfondo);
fprintf(fp_pag,"*elenco_wid0: %s\n",elenco_wid);
fprintf(fp_pag,"*num_widget:  %d\n",num_obj);
fprintf(fp_pag,"*nextnum:   %d\n",num_obj+1);
fclose(fp_pag);
sistema_file(nome_file);
}

void insert_icona(fp_in,fp_out)
FILE *fp_in;
FILE *fp_out;
{
char riga[MAXRIGA];
char nome_icona[50];
int x,y;
int rotate,ass_rotate;
int dim_y;
int dim_x;

/*
	skippa 3 righe
*/
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);

/*
	legge il nome dell'icona
*/
legge_riga(fp_in,riga);
strcpy(nome_icona,riga);

/*
	legge la posizione
*/
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
x=(int)(atof(riga)*FATTORE_SCALA);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
y= -(int)(atof(riga)*FATTORE_SCALA);
/*
	legge le eventuali rotazioni
*/
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
ass_rotate=(int)(atof(riga));
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
rotate=(int)(atof(riga));

/*
          verifico se l'icona e' fra quelle da eliminare
*/
if((strcmp(nome_icona,"TESSER")==0)||
   (strcmp(nome_icona,"TESSE1")==0)||
   (strcmp(nome_icona,"BEEP")==0)||
   (strcmp(nome_icona,"RIG")==0)||
   (strcmp(nome_icona,"SEP")==0)||
   (strcmp(nome_icona,"BOTT")==0)||
   (strcmp(nome_icona,"GRAF")==0)||
   (strcmp(nome_icona,"LISTPP")==0)||
   (strcmp(nome_icona,"LISTPR")==0)||
   (strcmp(nome_icona,"AF")==0)||
   (strcmp(nome_icona,"COL")==0)||
   (strcmp(nome_icona,"LEG")==0)||
   (strcmp(nome_icona,"OGR")==0)||
   (strcmp(nome_icona,"STAZ")==0))
	return;


/*
	verifica il tipo di icona
*/
if((strcmp(nome_icona,"DISP1")==0)||
    (strcmp(nome_icona,"DISPMM")==0)||
    (strcmp(nome_icona,"DISPMW")==0)||
    (strcmp(nome_icona,"DISP2")==0) ||
    (strcmp(nome_icona,"DISP3")==0))
	{
	fprintf(fp_out,"*%dw.x0: %d\n",num_obj,x);
	fprintf(fp_out,"*%dw.y0: %d\n",num_obj,y);
	fprintf(fp_out,"*%dw.background: %s\n",num_obj,BACKGROUND);
	fprintf(fp_out,"*%dw.agoFg: white\n",num_obj);
	fprintf(fp_out,"*%dw.width0: %d\n",num_obj,dim_x_icona(nome_icona));
	fprintf(fp_out,"*%dw.height0: %d\n",num_obj,dim_y_icona(nome_icona));
	fprintf(fp_out,"*%dw.tipoInd: 2\n",num_obj);
        sprintf(elenco_wid,"%s %dw Indic",elenco_wid,num_obj);
	++num_obj;
	return;
	}
if((strcmp(nome_icona,"CPAG")==0))
	{
	fprintf(fp_out,"*%dw.x0: %d\n",num_obj,x);
	fprintf(fp_out,"*%dw.y0: %d\n",num_obj,y);
	fprintf(fp_out,"*%dw.background: %s\n",num_obj,BACKGROUND);
	fprintf(fp_out,"*%dw.normFg: white\n",num_obj);
	fprintf(fp_out,"*%dw.width0: 10\n",num_obj);
	fprintf(fp_out,"*%dw.height0: 10\n",num_obj);
	fprintf(fp_out,"*%dw.changeType: 0\n",num_obj);
        sprintf(elenco_wid,"%s %dw ChangePage",elenco_wid,num_obj);
	++num_obj;
	return;
	}

fprintf(fp_out,"*%dw.width0: 10\n",num_obj);
fprintf(fp_out,"*%dw.height0: 10\n",num_obj);
fprintf(fp_out,"*%dw.nomeFile: %s\n",num_obj,nome_icona);

if(ass_rotate== -1)
	{
	fprintf(fp_out,"*%dw.assRotate: 1\n",num_obj);
	dim_x=dim_x_icona(nome_icona);
	fprintf(fp_out,"*%dw.x0: %d\n",num_obj,x-dim_x);
	}
else
	{
	fprintf(fp_out,"*%dw.assRotate: 0\n",num_obj);
	fprintf(fp_out,"*%dw.x0: %d\n",num_obj,x);
	}
	
if(rotate==0)
	{
	fprintf(fp_out,"*%dw.rotate: 0\n",num_obj);
	fprintf(fp_out,"*%dw.y0: %d\n",num_obj,y);
	}
 else
  if(rotate==90)
	{
	fprintf(fp_out,"*%dw.rotate: 3\n",num_obj);
	dim_y=dim_y_icona(nome_icona);
	fprintf(fp_out,"*%dw.y0: %d\n",num_obj,y-dim_y);
	}
   else
    if(rotate== -90)
	{
    	fprintf(fp_out,"*%dw.rotate: 1\n",num_obj);
	fprintf(fp_out,"*%dw.y0: %d\n",num_obj,y);
	}
sprintf(elenco_wid,"%s %dw Icon",elenco_wid,num_obj);
++num_obj;
}

int dim_y_icona(nome)
char  *nome;
{
FILE *fp;
int min_x,min_y,max_x,max_y;
char riga[50];
char nome_file[50];

sprintf(nome_file,"%s/%s.ico",path_ico,nome);
fp=fopen(nome_file,"r");
/*
        legge le coordinate massime e minime
*/
fscanf(fp,"%s %d",riga,&min_x);
fscanf(fp,"%s %d",riga,&min_y);
fscanf(fp,"%s %d",riga,&max_x);
fscanf(fp,"%s %d",riga,&max_y);

return(max_y-min_y);
fclose(fp);
}


int dim_x_icona(nome)
char  *nome;
{
FILE *fp;
int min_x,min_y,max_x,max_y;
char riga[50];
char nome_file[50];

sprintf(nome_file,"%s/%s.ico",path_ico,nome);
fp=fopen(nome_file,"r");
/*
        legge le coordinate massime e minime
*/
fscanf(fp,"%s %d",riga,&min_x);
fscanf(fp,"%s %d",riga,&min_y);
fscanf(fp,"%s %d",riga,&max_x);
fscanf(fp,"%s %d",riga,&max_y);

return(max_x-min_x);
fclose(fp);
}

void insert_text(fp_in,fp_out)
FILE *fp_in;
FILE *fp_out;
{
char riga[MAXRIGA];
int x,y;
int codice_colore;

/*
	skippa 3 righe
*/
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);

/*
	legge il colore
*/
legge_riga(fp_in,riga);
codice_colore=atoi(riga);

/*
	legge la posizione
*/
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
x=(int)(atof(riga)*FATTORE_SCALA);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
y= -(int)(atof(riga)*FATTORE_SCALA);

/*
	skippa 13 righe
*/
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);

/*
	legge il testo
*/
legge_riga(fp_in,riga);

/*
	verifica che il testo non sia fra i testi da eliminare
*/
if(strcmp(riga,"PLOT")==0 ||
   strcmp(riga,"STAZ")==0 ||
   strcmp(riga,"INFO")==0 ||
   strcmp(riga,"AING")==0 ||
   strcmp(riga,"DISP")==0 ||
	strncmp(riga,"_",1)==0 )
	return;

if(strlen(riga)>0)
	{
	fprintf(fp_out,"*%dw.x0: %d\n",num_obj,x);
	fprintf(fp_out,"*%dw.y0: %d\n",num_obj,y);
	fprintf(fp_out,"*%dw.width0: 10\n",num_obj);
	fprintf(fp_out,"*%dw.height0: 10\n",num_obj);
	fprintf(fp_out,"*%dw.background: %s\n",num_obj,BACKGROUND);
	fprintf(fp_out,"*%dw.normFg: %s\n",num_obj,
			tab_colori[codice_colore].colore);
	fprintf(fp_out,"*%dw.tipoLabel: 0\n",num_obj);
	fprintf(fp_out,"*%dw.labelText: %s\n",num_obj,riga);
	sprintf(elenco_wid,"%s %dw Label",elenco_wid,num_obj);
	++num_obj;
	}
}
