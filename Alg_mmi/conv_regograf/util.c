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
   modulo util.c
   tipo 
   release 1.18
   data 11/10/95
   reserved @(#)util.c	1.18
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)util.c	1.18\t11/10/95";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <Rt/RtMemory.h>

#include "util.h"


extern char file_icone[255];
extern char nome_file_input[100];
extern char nome_file_lib[255];
extern char path_ico[255];
extern char path_pag[255];
extern int modo_icona;
extern int modo_lib;

void TagPagCalcNext();
void read_solid(FILE *,FILE *);
void read_circle(FILE *,FILE *);
void sistema_file(char *);
void insert_icona(FILE *,FILE *);
void insert_text(FILE *,FILE *);
int to_snap(int );
void insert_icona_lib(char *);
void sistema_connessioni(FILE *);
static int dim_x_icona(char*);
static int dim_y_icona(char*);
static int min_x_icona(char*);
static int min_y_icona(char*);
void insert_figli_icona(char *,int ,FILE *,char *);
static int to_snap_porte(int );


int num_obj=0;
char elenco_wid[8000];

extern TAB_PORTE tab_porte[4000];
extern int num_porte;

TAB_ICONE tab_icone[1000];
int num_icone=0;

ELENCO_PORTE elenco_porte[4000];
int tot_porte=0;

CONNESSIONI conn[3000];
int num_conn=0;

int numTagReg=0;
char maxtag[3];
char tag_schema[3];

char *cerca_tag();
char *cerca_id_i();

void skip_header(FILE *);
void read_polyline(FILE *,FILE *);
void minuscolo(char *);
static void sistema_posizione_porte(char *,int *x,int *);


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
	/*
		sistema il minimo e il masssimo nel file del disegno
	*/
	sistema_file(nome_file);
	}
if(modo_lib==1)
	insert_icona_lib(nome);
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
		x[num_punti]=to_snap(x[num_punti]);
		legge_riga(fp_in,riga);
		legge_riga(fp_in,riga);
		y[num_punti]= -(int)(atof(riga)*FATTORE_SCALA);
		y[num_punti]=to_snap(y[num_punti]);
		++num_punti;
		}
	legge_riga(fp_in,riga);
	}
fprintf(fp_out,"wi_c 0\n");
fprintf(fp_out,"st_c 0\n");
fprintf(fp_out,"fg_c %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"bg_c %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"fg_f %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"fill 0\n");

if(modo_icona==0)
{	
fprintf(fp_out,"Conn %d",num_punti);
for(i=0;i<num_punti;i++)
	fprintf(fp_out," %d %d",x[i],y[i]);
}
else
{	
fprintf(fp_out,"poli %d",num_punti+1);
for(i=0;i<num_punti;i++)
	fprintf(fp_out," %d %d",x[i],y[i]);
fprintf(fp_out," %d %d",x[0],y[0]);
}

/*
	chiusura della polyline
*/
fprintf(fp_out,"\n");
}

void sistema_file(nome)
char *nome;
{
FILE *fp;
FILE *fp_app;
int num=0;
char riga[MAXRIGA];
int min_x=9999999;
int min_y=9999999;
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
                case 'C':
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

x1=to_snap(x1);
x2=to_snap(x2);
x3=to_snap(x3);
x4=to_snap(x4);
y1=to_snap(y1);
y2=to_snap(y2);
y3=to_snap(y3);
y4=to_snap(y4);

if((x1==x2)&&(y1==y2)&&(x2==x3)&&(y2==y3)&&(x3==x4)&&(y3==y4))
	return;

fprintf(fp_out,"wi_c 0\n");
fprintf(fp_out,"st_c 0\n");
fprintf(fp_out,"fg_c %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"bg_c %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"fg_f %s\n",tab_colori[codice_colore].colore);
fprintf(fp_out,"fill 1\n");
fprintf(fp_out,"poli 4 %d %d %d %d %d %d %d %d\n",
		x1,y1, x2,y2, x4,y4, x3,y3);
/**
printf("SOLID col %d (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n",codice_colore,
	x1,y1, x2,y2, x3,y3, x4,y4);
**/
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

x1=to_snap(x1);
y1=to_snap(y1);
r=to_snap(r);

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
int i;
char newmaxtag[3];

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
fprintf(fp_pag,"*top_tipo:     Regolazione\n");
fprintf(fp_pag,"*top_descrizione:  Schema di regolazione prov. da Regograf\n");
fprintf(fp_pag,"*drawing_width: 1500\n");
fprintf(fp_pag,"*drawing_height: 1500\n");
fprintf(fp_pag,"*drawing_background: %s\n",BACKGROUND);
fprintf(fp_pag,"*nomePag:  %s\n",nome);
fprintf(fp_pag,"*refresh_freq: 10\n");

elenco_wid[0]=0;

strcpy(maxtag,"00");

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
fprintf(fp_pag,"*tagPag:   %s\n",tag_schema);
fprintf(fp_pag,"*numTagReg:   %d\n",numTagReg);
TagPagCalcNext(maxtag,maxtag);
fprintf(fp_pag,"*nextTagReg:   %s\n",maxtag);
sistema_connessioni(fp_pag);
fclose(fp_pag);
/******
scrivi_connessioni(nome_file);
sistema_file(nome_file);
*********/
/**
for(i=0;i<tot_porte;i++)
	printf("icona [%s] porta [%s] id_i [%s] id_w [%s]\n",
		elenco_porte[i].nome_icona,elenco_porte[i].nome_porta,
		elenco_porte[i].id_i,elenco_porte[i].id_w);
**/
}

void insert_icona(fp_in,fp_out)
FILE *fp_in;
FILE *fp_out;
{
char riga[MAXRIGA];
char nome_icona[50];
int x,y;
int old_x,old_y;
int rotate,ass_rotate;
int dim_y;
int dim_x;
int min_y;
int min_x;
char *tag;
char nome_tag[5];
char nome_modulo[5];
char id_i[5];

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
old_x=(int)(atof(riga));
legge_riga(fp_in,riga);
legge_riga(fp_in,riga);
y= -(int)(atof(riga)*FATTORE_SCALA);
old_y= -(int)(atof(riga));

x=to_snap(x);
y=to_snap(y);
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
   (strcmp(nome_icona,"CAD")==0)||
   (strcmp(nome_icona,"C")==0)||
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

dim_x=dim_x_icona(nome_icona);
dim_y=dim_y_icona(nome_icona);

fprintf(fp_out,"*%dw.width0: %d\n",num_obj,dim_x+1);
fprintf(fp_out,"*%dw.height0: %d\n",num_obj,dim_y+1);
fprintf(fp_out,"*%dw.nomeFile: %s\n",num_obj,nome_icona);
strcpy(nome_modulo,nome_icona);
nome_modulo[4]=0;
if(strcmp(nome_modulo,"INTE")==0)
	{
	fprintf(fp_out,"*%dw.modulName: %s\n",num_obj,nome_modulo);
	fprintf(fp_out,"*%dw.iconRegType: 1\n",num_obj);
	}
else
	{
	minuscolo(nome_modulo);
	fprintf(fp_out,"*%dw.iconRegType: 0\n",num_obj);
	fprintf(fp_out,"*%dw.modulName: %s\n",num_obj,nome_modulo);
	}


min_x=min_x_icona(nome_icona);
min_y=min_y_icona(nome_icona);

/*****
printf("icona [%s] min(%d,%d)  dim(%d,%d)\n",nome_icona,min_x,min_y,dim_x,dim_y);
******/

strcpy(id_i,cerca_id_i(nome_icona,old_x,old_y));
tag=cerca_tag(nome_icona,old_x,old_y);
strcpy(nome_tag,tag);
nome_tag[4]=0;
fprintf(fp_out,"*%dw.tagName: %s\n",num_obj,nome_tag);

/*
	incrementa il numero delle icone
*/
if(strcmp(nome_tag,"none")!=0)
	{
	++numTagReg;
        strcpy(tag_schema,&nome_tag[2]);
	nome_tag[2]=0;
	if(strcmp(nome_tag,maxtag)>0)
		strcpy(maxtag,nome_tag);
	}

if((rotate==90)&&(ass_rotate==1))
	{
	fprintf(fp_out,"*%dw.x0: %d\n",num_obj,x);
	fprintf(fp_out,"*%dw.y0: %d\n",num_obj,y-dim_x-min_x);
    	fprintf(fp_out,"*%dw.rotate: 3\n",num_obj);
/******** prova ********/
	fprintf(fp_out,"*%dw.assRotate: 0\n",num_obj);
	}
if((rotate==0)&&(ass_rotate==0))
	{
	fprintf(fp_out,"*%dw.x0: %d\n",num_obj,x+min_x);
	fprintf(fp_out,"*%dw.y0: %d\n",num_obj,y+min_y);
    	fprintf(fp_out,"*%dw.rotate: 0\n",num_obj);
	fprintf(fp_out,"*%dw.assRotate: 0\n",num_obj);
	}
if((rotate== -90)&&(ass_rotate==1))
	{
	fprintf(fp_out,"*%dw.x0: %d\n",num_obj,x);
	fprintf(fp_out,"*%dw.y0: %d\n",num_obj,y+min_x);
    	fprintf(fp_out,"*%dw.rotate: 1\n",num_obj);
	fprintf(fp_out,"*%dw.assRotate: 1\n",num_obj);
	}
if((rotate== 0)&&(ass_rotate== -1))
	{
	fprintf(fp_out,"*%dw.x0: %d\n",num_obj,x-dim_x);
	fprintf(fp_out,"*%dw.y0: %d\n",num_obj,y);
    	fprintf(fp_out,"*%dw.rotate: 0\n",num_obj);
	fprintf(fp_out,"*%dw.assRotate: 1\n",num_obj);
	}
if((rotate== 0)&&(ass_rotate== 1))
	{
	fprintf(fp_out,"*%dw.x0: %d\n",num_obj,x+min_x);
	fprintf(fp_out,"*%dw.y0: %d\n",num_obj,y+min_y);
    	fprintf(fp_out,"*%dw.rotate: 0\n",num_obj);
	fprintf(fp_out,"*%dw.assRotate: 0\n",num_obj);
	}
sprintf(elenco_wid,"%s %dw IconReg",elenco_wid,num_obj);
insert_figli_icona(nome_icona,num_obj,fp_out,id_i);
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

fclose(fp);
return(max_y-min_y);
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

fclose(fp);
return(max_x-min_x);
}

int min_y_icona(nome)
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

fclose(fp);
return(min_y);
}

int min_x_icona(nome)
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

fclose(fp);
return(min_x);
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

/*
 Lettura del file P-CSRPR.OUT dal quale si ricavano per ogni porta l'icona
 di appartenenza la posizione ed il colore delle porte medesime.
 I dati letti vengono memorizzati nel vettore di strutture tab_porte
*/ 
void leggi_porte(char *nome_file)
{
FILE *fp;
char riga[MAXRIGA];
char nome_icona[20];
char *app_char;

printf("leggi_porte: <%s>\n",nome_file);
if((fp=fopen(nome_file,"r"))<=0)
        exit(fprintf(stderr,"il file %s non esiste\n",nome_file));

while (legge_riga(fp,riga)==1)
      {
/*
 Copia il nome dell'icona
*/
      if(riga[0]=='-')
	{
	app_char=strstr(riga," ");
	++app_char;
	strcpy(nome_icona,app_char);
	}
/*
 Legge Nome della porta, colore, posizione ed ID della porta
 salva in tab_porte i dati letti insieme al nome della icona
 che contiene la porta stessa.
*/
      if(riga[0]=='P')
	{
	strcpy(tab_porte[num_porte].nome_icona,nome_icona);
	strcpy(tab_porte[num_porte].nome_porta,&riga[6]);
	legge_riga(fp,riga);
	strcpy(tab_porte[num_porte].id_porta,&riga[6]);
	legge_riga(fp,riga);
	tab_porte[num_porte].colore_porta=atoi(&riga[6]);
	legge_riga(fp,riga);
	tab_porte[num_porte].pos_x=atof(&riga[6]);
	legge_riga(fp,riga);
	tab_porte[num_porte].pos_y=atof(&riga[6]);
	/*********
	printf("icona -> [%s] porta [%s] id=[%s] colore=[%d] (%f,%f)\n",
		tab_porte[num_porte].nome_icona,
		tab_porte[num_porte].nome_porta,
		tab_porte[num_porte].id_porta,
		tab_porte[num_porte].colore_porta,
		tab_porte[num_porte].pos_x,
		tab_porte[num_porte].pos_y);
	*************/
	++num_porte;
	}
      }
fclose(fp);

}

void insert_figli_icona(char *nome_icona,int num_obj,FILE *fp_out,char *id_i)
{
int i;
int num_figli=0;
char elenco[1000];
int x,y;

elenco[0]=0;
sprintf(elenco,"*%dw.listChildren: ",num_obj);
for(i==0;i<num_porte;i++)
	{
	if(strcmp(nome_icona,tab_porte[i].nome_icona)==0)
		{
		/******
		printf("ins porta [%s] id_i = [%s]\n",tab_porte[i].nome_porta,id_i);
		*****/
		strcpy(elenco_porte[tot_porte].nome_icona,nome_icona);
		strcpy(elenco_porte[tot_porte].nome_porta,
			tab_porte[i].nome_porta);
		strcpy(elenco_porte[tot_porte].id_i,id_i);
		sprintf(elenco_porte[tot_porte].id_w,"%dw%dc",
				num_obj,num_figli);
		++tot_porte;

		
		sprintf(elenco,"%s %dw%dc Port",elenco,num_obj,num_figli);
		if(modo_lib == 1)
		   {
		   x=(tab_porte[i].pos_x*(float)FATTORE_SCALA);
		   y=(tab_porte[i].pos_y*(float)FATTORE_SCALA);
		   sistema_posizione_porte(nome_icona,&x,&y);
		fprintf(fp_out,"*%dw%dc.layoutPort: 1\n",num_obj,num_figli);
                   }
		else
		   {
		   x=tab_porte[i].pos_x;
		   y=tab_porte[i].pos_y;
		fprintf(fp_out,"*%dw%dc.layoutPort: %d\n",num_obj,num_figli,
				tab_porte[i].layout);
		fprintf(fp_out,"*%dw%dc.rotate: %d\n",num_obj,num_figli,
				tab_porte[i].rotate);
		fprintf(fp_out,"*%dw%dc.assRotate: %d\n",num_obj,num_figli,
				tab_porte[i].ass_rotate);
                   }
		fprintf(fp_out,"*%dw%dc.x0: %d\n",num_obj,num_figli,x);
		fprintf(fp_out,"*%dw%dc.y0: %d\n",num_obj,num_figli,y);
		fprintf(fp_out,"*%dw%dc.labelColor: gray\n",
				num_obj,num_figli);
		fprintf(fp_out,"*%dw%dc.portName: %s\n",num_obj,num_figli,
				tab_porte[i].nome_porta);
		if((tab_porte[i].id_porta[1]=='A')&&
			(tab_porte[i].id_porta[2]=='I'))
		     		fprintf(fp_out,"*%dw%dc.tipoPort: 0\n",
					num_obj,num_figli);
		if((tab_porte[i].id_porta[1]=='A')&&
			(tab_porte[i].id_porta[2]=='O'))
		     		fprintf(fp_out,"*%dw%dc.tipoPort: 1\n",
					num_obj,num_figli);
		if((tab_porte[i].id_porta[1]=='L')&&
			(tab_porte[i].id_porta[2]=='I'))
		     		fprintf(fp_out,"*%dw%dc.tipoPort: 2\n",
					num_obj,num_figli);
		if((tab_porte[i].id_porta[1]=='L')&&
			(tab_porte[i].id_porta[2]=='O'))
		     		fprintf(fp_out,"*%dw%dc.tipoPort: 3\n",
					num_obj,num_figli);
		num_figli++;
		}
	}
if(num_figli>0)
	{
	fprintf(fp_out,"%s\n",elenco);
	fprintf(fp_out,"*%dw.numFigli: %d\n",num_obj,num_figli);
	}
}

/*
 get_size_icone
 Ricava nomi e posizione icone di regolazione e
 descrizione delle connessioni
*/
void get_size_icone(char *nome_file)
{
char riga[MAXRIGA];
char riga_dir[MAXRIGA];
FILE *fp;
char id_i[5],id_m[5];
int punti,i;
int off_x,off_y;

printf("get_size_icone\n");
/*
 Apertura file I-SCHM.OUT
*/
if((fp=fopen(nome_file,"r"))<=0)
        exit(fprintf(stderr,"il file %s non esiste\n",nome_file));
while (legge_riga(fp,riga)==1)
      {
/*
 Se incontra CONN significa che e' terminata la sezione descrittiva delle icone
 -> segue lalettura delle connessioni
*/
      if(strstr(riga,"--------CONN")!=NULL)
	break;
      if(riga[0]=='I')
	{
/*
 Copia Id interno della icona e l'Id modellistico
 della medesima
*/
	strcpy(id_i,&riga[6]);
	legge_riga(fp,riga);
	strcpy(id_m,&riga[6]);
	}
      if(riga[0]=='T')
	{
/*
 Copia il nome del blocco lego associato all'icona ed 
 introduce nella struttuta tab_icone gli Id letti in precedenza
 seguono dimensioni rotazione posizione.
*/
   	off_x=0;
   	off_y=0;
	strcpy(tab_icone[num_icone].nome_icona,&riga[6]);
	strcpy(tab_icone[num_icone].id_i,id_i);
	strcpy(tab_icone[num_icone].id_m,id_m);
	legge_riga(fp,riga);
	strcpy(riga_dir,riga);
	legge_riga(fp,riga);
	tab_icone[num_icone].dim_x=atoi(&riga[6]);
	legge_riga(fp,riga);
	tab_icone[num_icone].dim_y=atoi(&riga[6]);
	legge_riga(fp,riga);
	/*
		corregge le eventuali rotazioni
	*/
	if(riga_dir[6]== 'L')
		{
		off_x = tab_icone[num_icone].dim_x;
		}
	if(riga_dir[6]== 'U')
		{
		off_y = tab_icone[num_icone].dim_x;
		}
		
	tab_icone[num_icone].pos_x=atoi(&riga[6]) + off_x;
	legge_riga(fp,riga);
	tab_icone[num_icone].pos_y=atoi(&riga[6]) + off_y;
	printf("icona [%s]  xs=%d ys=%d x=%d y=%d id_i=%s id_m=%s\n",
		tab_icone[num_icone].nome_icona,
		tab_icone[num_icone].dim_x,
		tab_icone[num_icone].dim_y,
		tab_icone[num_icone].pos_x,
                tab_icone[num_icone].pos_y,
		tab_icone[num_icone].id_i,
		tab_icone[num_icone].id_m);
	num_icone++;
	}
      }

/*
	legge le connessioni cioe' sorgente e destinazione
        della connessione
*/
while (legge_riga(fp,riga)==1)
      {
/*
 Copia Id interno icona sorgente
       Nome blocco lego associato a icona sorgente
       Nome porta sorgente (nome variabile LEGO)
*/
	strcpy(conn[num_conn].id_i_1,&riga[6]);
	legge_riga(fp,riga);
	strcpy(conn[num_conn].nome_icona_1,&riga[6]);
	legge_riga(fp,riga);
	strcpy(conn[num_conn].nome_porta_1,&riga[6]);
	legge_riga(fp,riga);
/*
 Copia Id interno icona destinazione
       Nome blocco lego associato a icona destinazione
       Nome porta destinazione (nome variabile LEGO)
*/
	strcpy(conn[num_conn].id_i_2,&riga[6]);
	legge_riga(fp,riga);
	strcpy(conn[num_conn].nome_icona_2,&riga[6]);
	legge_riga(fp,riga);
	strcpy(conn[num_conn].nome_porta_2,&riga[6]);
	legge_riga(fp,riga);
	conn[num_conn].colore=atoi(&riga[6]);
	legge_riga(fp,riga);
/*
 Ricava quanti sono i punti che compongono la connessione
 e li memorizza
*/
	conn[num_conn].num=atoi(&riga[6]);
	for(i=0;i<conn[num_conn].num;i++)
		{
		legge_riga(fp,riga);
		conn[num_conn].x[i]=(atof(&riga[2])*(float)FATTORE_SCALA);
		legge_riga(fp,riga);
		conn[num_conn].y[i]=(atof(&riga[2])*(float)FATTORE_SCALA);
		}
	legge_riga(fp,riga);
	num_conn++;
      }
}

void sistema_posizione_porte(char *nome_icona,int *x,int *y)
{
int i;
int min_x,min_y;
int dim_x,dim_y;
int app_int;

if(modo_lib != 1)
	{
	for(i=0;i<num_icone;i++)
		if(strcmp(nome_icona,tab_icone[i].nome_icona)==0)
			break;
	if(i==num_icone)
	    exit(fprintf(stderr,
	     "sistema_posizione_porte: icone [%s] non trovata\n", nome_icona));
	}

min_x=min_x_icona(nome_icona);
min_y=min_y_icona(nome_icona);
dim_x=dim_x_icona(nome_icona);
dim_y=dim_y_icona(nome_icona);


if(min_x>0)
	{
	*x -= min_x;
	}
if(min_y>0)
	{
	*y -= min_y;
	}

if(dim_y - (*y) <4)
	*y=dim_y-4;
if(dim_x - (*x) <4)
	*x=dim_x-4;

if(*x<0)
	*x=0;
if(*y<0)
	*y=0;

*x = to_snap_porte( *x );
*y = to_snap_porte( *y );
}

char *cerca_tag(char *nome_icona,int x,int y)
{
int i;

for(i=0;i<num_icone;i++)
	{
	if(strcmp(tab_icone[i].nome_icona,nome_icona)==0)
		{
		if((fabs((float)tab_icone[i].pos_x-(float)x)<2.)&&
			(fabs((float)tab_icone[i].pos_y-(float)y)<2.))
			return(tab_icone[i].id_m);
		}
	}
printf("Errore non trovata tag per [%s] (%d,%d)\n",
		nome_icona,x,y);
return(tab_icone[i].id_m);
}

char *cerca_id_i(char *nome_icona,int x,int y)
{
int i;

for(i=0;i<num_icone;i++)
	{
	if(strcmp(tab_icone[i].nome_icona,nome_icona)==0)
		{
		if((fabs((float)tab_icone[i].pos_x-(float)x)<2.)&&
			(fabs((float)tab_icone[i].pos_y-(float)y)<2.))
			return(tab_icone[i].id_i);
		}
	}
printf("Errore non trovata tag per [%s] (%d,%d)\n",
		nome_icona,x,y);
return(tab_icone[i].id_i);
}

/*
 Inserisce il nome della porta connessa nelle due porte
 interessate dalla connessione medesima
*/
void sistema_connessioni(FILE *fp_pag)
{
int i,j;
char porta_1[10];
char porta_2[10];
porta_1[0]=0;
porta_2[0]=0;

for(i=0;i<num_conn;i++)
	{
	for(j=0;j<tot_porte;j++)
		{
/*******
printf("%s - %s ------------- %s - %s\n",
		conn[i].id_i_1,elenco_porte[j].id_i,
		conn[i].nome_icona_1,elenco_porte[j].nome_icona);
*******/
	        if((strcmp(conn[i].id_i_1,elenco_porte[j].id_i)==0)&&
		 (strcmp(conn[i].nome_icona_1,elenco_porte[j].nome_icona)==0)&&
		 (strcmp(conn[i].nome_porta_1,elenco_porte[j].nome_porta)==0))
			{
			strcpy(porta_1,elenco_porte[j].id_w);
			break;
			}
		}
	if(j==tot_porte)
		{
		printf("\n Porta non trovata\n");
printf("%s - %s ------------- %s - %s\n",
		conn[i].id_i_1,elenco_porte[j].id_i,
		conn[i].nome_icona_1,elenco_porte[j].nome_icona);
		continue;
		}
	for(j=0;j<tot_porte;j++)
	        if((strcmp(conn[i].id_i_2,elenco_porte[j].id_i)==0)&&
		 (strcmp(conn[i].nome_icona_2,elenco_porte[j].nome_icona)==0)&&
		 (strcmp(conn[i].nome_porta_2,elenco_porte[j].nome_porta)==0))
			{
			strcpy(porta_2,elenco_porte[j].id_w);
			break;
			}
	if(j==tot_porte)
		{
		printf("\n Porta non trovata\n");
printf("%s - %s ------------- %s - %s\n",
		conn[i].id_i_1,elenco_porte[j].id_i,
		conn[i].nome_icona_1,elenco_porte[j].nome_icona);
		continue;
		}
	strcpy(conn[i].nomep1,porta_1);
	strcpy(conn[i].nomep2,porta_2);
	fprintf(fp_pag,"*%s.portNameConnect: %s\n",porta_1,porta_2);
	fprintf(fp_pag,"*%s.portNameConnect: %s\n",porta_2,porta_1);
	}
}

void minuscolo(char *testo)
{
int i;
for(i=0;i<strlen(testo);i++)
        testo[i]=tolower(testo[i]);
}

void TagPagCalcNext(char *Tag,char *nextTag)
{
   if( (Tag[1] >= ASCII0 && Tag[1] < ASCII9) ||
       (Tag[1] >= ASCIIA && Tag[1] < ASCIIZ) )
      nextTag[1]++;
   else
   {
      if(Tag[1] == ASCII9) /* tag == '9'  passo ad 'A' */
         nextTag[1] = ASCIIA;
      else if(Tag[1] == ASCIIZ) /* tag == 'Z' riparto incrementando 
					il numero a sin */
      {
         nextTag[0]++;
         nextTag[1] = ASCII0;  /* '0' */
      }
   }

   nextTag[2]=0; /* chiusura stringa */
}

void insert_icona_lib(char *nome_icona)
{
static int nwlib=0;
static int py=0;
static int px=20;
FILE *fp;
char riga[MAXRIGA];
int x,y;
int old_x,old_y;
int rotate,ass_rotate;
int dim_y;
int dim_x;
int min_y;
int min_x;
char *tag;
char nome_tag[5];
char nome_modulo[5];
char id_i[5];

if(nome_icona!=NULL)
{

/*
          verifico se l'icona e' fra quelle da eliminare
*/
if((strcmp(nome_icona,"TESSER")==0)||
   (strcmp(nome_icona,"MINTTX")==0)||
   (strcmp(nome_icona,"INTETX")==0)||
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
   (strcmp(nome_icona,"CAD")==0)||
   (strcmp(nome_icona,"C")==0)||
   (strcmp(nome_icona,"OGR")==0)||
   (strcmp(nome_icona,"DISP1")==0)||
    (strcmp(nome_icona,"DISPMM")==0)||
    (strcmp(nome_icona,"DISPMW")==0)||
    (strcmp(nome_icona,"DISP2")==0) ||
    (strcmp(nome_icona,"DISP3")==0) ||
    (strcmp(nome_icona,"CPAG")==0) ||
   (strcmp(nome_icona,"STAZ")==0))
        return;



py+=100;
if(py>900)
	{
	py=100;
	px+=80;
	}
printf("insert_icona_file: [%s] %d %d\n",nome_icona,px,py);
fp=fopen(nome_file_lib,"a");
dim_x=dim_x_icona(nome_icona);
dim_y=dim_y_icona(nome_icona);

fprintf(fp,"*%dw.width0: %d\n",nwlib,dim_x+1);
fprintf(fp,"*%dw.height0: %d\n",nwlib,dim_y+1);
fprintf(fp,"*%dw.nomeFile: %s\n",nwlib,nome_icona);
strcpy(nome_modulo,nome_icona);
nome_modulo[4]=0;
if(strcmp(nome_modulo,"INTE")==0)
        {
        fprintf(fp,"*%dw.modulName: %s\n",nwlib,nome_modulo);
        fprintf(fp,"*%dw.iconRegType: 1\n",nwlib);
        }
else
        {
        minuscolo(nome_modulo);
        fprintf(fp,"*%dw.iconRegType: 0\n",nwlib);
        fprintf(fp,"*%dw.modulName: %s\n",nwlib,nome_modulo);
        }



fprintf(fp,"*%dw.x0: %d\n",nwlib,px);
fprintf(fp,"*%dw.y0: %d\n",nwlib,py);
fprintf(fp,"*%dw.rotate: 0\n",nwlib);
fprintf(fp,"*%dw.assRotate: 0\n",nwlib);
sprintf(elenco_wid,"%s %dw IconReg",elenco_wid,nwlib);
insert_figli_icona(nome_icona,nwlib,fp,"");
++nwlib;
fclose(fp);
}
else
{
fp=fopen(nome_file_lib,"a");
fprintf(fp,"*elenco_wid0: %s\n",elenco_wid);
fprintf(fp,"*num_widget:  %d\n",nwlib);
fprintf(fp,"*nextnum:   %d\n",nwlib+1);
fclose(fp);
}
}


int to_snap(int p)
{
static int ret;
int snap=4;

ret= (p/snap)*snap;

if(ret!=p)
	ret+=snap;
if(ret-p>snap/2)
	ret -= snap;

return(ret);
}

int to_snap_porte(int p)
{
static int ret;
int snap=4;

ret= (p/snap)*snap;

if(ret!=p)
	ret+=snap;
if(ret-p>snap/2)
	ret -= snap;

return(ret);
}
