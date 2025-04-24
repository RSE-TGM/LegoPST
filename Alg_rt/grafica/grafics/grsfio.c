/**********************************************************************
*
*       C Source:               grsfio.c
*       Subsystem:              2
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Apr 20 14:00:58 2004 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: grsfio.c-3 %  (%full_filespec: grsfio.c-3:csrc:2 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)grsfio.c	1.8\t11/6/95";
/*
   modulo grsfio.c
   tipo 
   release 1.8
   data 11/6/95
   reserved @(#)grsfio.c	1.8
*/
/*
 * grsfio.c
 *    contiene tutte le routine di lettura-scrittura su file relative
 *    all'applicazione graf.
 */

#include <stdio.h>
#include <stdlib.h>
#if defined UNIX
#include <sys/types.h>
#include <sys/stat.h>
#endif
#if defined VMS
#include <types.h>
#include <stat.h>
#endif
#include <X11/Xlib.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Mrm/MrmPublic.h> 
#if defined VMS
#include <unixio.h>
#include <file.h>
#endif

#include "libutilx.h"
#include "uni_mis.h"
#include "param_f22.h"
#include "grafics.h"

                 
/*
 variabili di uso comune alle routines di I/O
 */
extern float t_iniziale;
extern float t_finale;
extern S_DATI dato;
extern S_SEL_DATI bufdati[];
extern S_GRAFICO sg;

extern S_MIN_MAX min_max[NUM_VAR];   /* valori minimi e massimi per ogni
					variabile  */
extern S_HEAD1_C header1_C;
extern S_HEAD1_FORTRAN header1_FORTRAN;
extern S_HEAD2 header2;
extern char nomi_misure[NUM_VAR+1][LUN_NOME+1]; /* tabella dei nomi delle misure
                                          con terminatore 0 aggiunto */
                 
extern char **simboli;

extern int n_last;     /* posizione dell'ultimo campione nel buffer circolare */

extern XmString x_gruppi[NUM_GRUPPI+1]; /* elenco dei gruppi */
extern REC_GRUPPO gruppi[NUM_GRUPPI];
extern char lista_gruppi[NUM_GRUPPI][LUN_SIMB+1];

int inizio_dati;
fpos_t posizione_iniziale;
int fine_dati; /* posizione ultimo dato */
#if defined (VMS_FORTRAN)
int fpDAT;
#else
FILE *fpDAT;
#endif
FILE *fpGR;   /* file per memorizzazione gruppi */
extern char *path_22dat;
char path_name[101];
/*
 open_22dat
   apre il file f22.dat
*/

extern int linguaggio_eseguibile;

static int read_nomi(FILE*,int *);
static int read_multi(char*);
static int set_min_max(S_DATI *);
static void close_path();


open_22dat()
{
int i;
int intero_iniziale;

i=strlen(path_22dat)-1;
strcpy(path_name,path_22dat);
#if defined VMS || VMS_FORTRAN
while(path_22dat[i]!=']' && path_22dat[i]!=':' ) i--;
#else
while(path_22dat[i]!='/' ) i--;
#endif
i++;
if(i>0) path_name[i]=0;
#if defined (VMS_FORTRAN)
fpDAT=open(path_22dat,O_RDWR,0,"ctx=rec","mrs=0","tmo=100",
            "rop=tmo","rfm=var","shr=upd");
if(fpDAT==-1)
        {
        return(1);
        }
#elif defined VMS 
i=open(path_22dat,O_RDONLY,0004|0002,
                "ctx=rec","rfm=var","shr=put");
printf("Apertura VMS file %s fp=%d\n",path_22dat,i);
fpDAT=fdopen(i,"r");
fseek(fpDAT,0,0);
if(fpDAT==NULL)
        {
        return(1);
        }
#endif
#if defined UNIX
fpDAT=fopen(path_22dat,"r");
if(fpDAT==NULL)
        {
        return(1);
        }
#endif

if ( fread( &intero_iniziale, sizeof(intero_iniziale), 1, fpDAT) != 1)
     {
     return(2);
     }
if (intero_iniziale==80)
     {
     linguaggio_eseguibile=F22_FORTRAN;
     printf("File generato da FORTRAN \n");
     }
else
     {
     linguaggio_eseguibile=F22_C;
     printf("File generato da C \n");
     }
rewind(fpDAT);
return(0);
}

/*
 close_22dat
   chiude il file f22.dat
*/
close_22dat()
{
#if defined VMS_FORTRAN
close(fpDAT);
#else
fclose(fpDAT);
#endif
}


/*
 read_22dat
    legge i dati relativi a tutte le misure;
	- flag : AGGIORNA    aggiorna l'area dati con gli ultimi dati acquisiti
	         TUTTI       trasferisce nell'area dati tutti i campioni 
			     presenti nel file.
*/

float tsec;   /* tempo in secondi da memorizzare nel buffer dati; 
				 viene incrementato di INC_SEC secondi per ogni
				 dato che viene inserito nel buffer */
int off_f22; 
int lun_rec_dati;  /* lunghezza record dati */
int lun_parziale;  /* lunghezza lettura parziale dati */
int num_complete;  /* numero di letture complete */
int b_wait;        /* segnala che e' stato settato il cursore di wait */

read_22dat(flag)
char flag;
{
static S_DATI buf;   /* buffer di appoggio per la lettura del record dati */
char *pb;
int iret;
float step,correz;
int parziali;
int i,lun;
char sep[2];
int ind;      /* indice che scorre il buffer di memorizzazione dati
				 bufval   */
int pieno;   /* indica se il buffer e' stato completamente riempito 
		(variabile utilizzata in fase di prima lettura) */
if(flag==TUTTI)
	{
	b_wait=1;
	set_cur_wait(); /* setta il cursore di wait */
	tsec=0.0;
	n_last=0;
	ind=0;
	pieno=0;
	off_f22=0;
//	printf("READ f22 TUTTI\n");
	if(read_nomi(fpDAT,&off_f22)==1) /* legge le tabelle dei nomi e simboli */
		{
		printf("\n fpDAT = %d",fpDAT);
		printf("\n errore in read nomi");
		return(1);
		}
/* lettura da file: si posiziona sul primo dato */
#if defined VMS_FORTRAN
	inizio_dati=lseek(fpDAT,0,1);
#else
	inizio_dati=off_f22;
//	printf("READ TUTTI %d\n",inizio_dati);
#endif
/* lunghezza totale dati da leggere */
        if (linguaggio_eseguibile == F22_C)
            {
	    lun_rec_dati=(header1_C.nvar+1)*sizeof(float);
	    }
        else
            {
	    lun_rec_dati=(header1_FORTRAN.nvar+1)*sizeof(float);
	    }
#if defined VMS_FORTRAN
/* numero di letture complete da effettuare */
        num_complete=lun_rec_dati/2042;
        if(lun_rec_dati%2042) parziali=1;
        else parziali=0;
/* lunghezza reale dei dati (compresi i separatori) */
        lun_rec_dati+=(2*(num_complete+parziali));
        lun_parziale=(lun_rec_dati)%2044-2;
#endif
/*
  inizializza il buffer dei minimi e massimi a valori estremi
*/
	set_min_max(NULL);
/*
  lettura del primo campione: ad esso dovra' corrispondere un tempo
  pari a 0.0 secondi.
*/	
	if((iret=read_multi(&buf.t))==0)
              {
              printf("\n dati non presenti in F22.DAT (read_multi)");
              exit(1);
              }
	off_f22+=iret;
/*
  aggiorna se necessario i valori di minimo e di massimo
*/
	t_iniziale=buf.t;
	while(read_multi(&buf.t)==lun_rec_dati)
		{
		if(ind >= DIM_BUFDATI)
			{
			if(b_wait)
				{
				b_wait=0;
				clr_cur_wait();
				}
			n_last=ind-1;
			fine_dati=n_last;
			return(3);
			}
		bufdati[ind].t=buf.t; 
		set_min_max(&buf); 
		ind++;
		}

	}
else  /* caso di lettura per aggiornamento  */
	{
	ind=n_last;
	if(n_last==0) 
		{
		b_wait=1;
		set_cur_wait();
		}
	while(read_multi(&buf.t)==lun_rec_dati)
		{
/*
 testa il caso in cui si e' riempito il buffer dati
*/
		if(ind >= DIM_BUFDATI)
			{
			if(b_wait)
				{
				b_wait=0;
				clr_cur_wait();
				}
			n_last=ind-1;
			fine_dati=n_last;
			return(3);
			}
                off_f22+=lun_rec_dati;
                set_min_max(&buf);
		for(i=0;i<4;i++)
			{
			if(sg.ind_mis[i]!=-1)
				bufdati[ind].mis[i]=buf.mis[sg.ind_mis[i]];
			}
		bufdati[ind].t=buf.t;
		ind++;
		}

	}

/*
 e' terminata la scansione del file f22.dat; ind punta alla posizione suc-
 cessiva all'ultima riempita.
*/
FINE:
if(b_wait)
	{
	b_wait=0;
	clr_cur_wait();
	}
n_last=ind;
fine_dati=n_last;
return(0);
}



int read_multi(buf)
char *buf;
{
char sep[2];
char *pb;
int tot_letti=0; /* lunghezza totale bytes letti */
int lun,i;
int ret;
float tempo;

pb=buf;
#if defined VMS_FORTRAN
for(i=0;i<num_complete;i++)
        {
        lun=read(fpDAT,sep,2);
        if(lun!=2) return(0);
        tot_letti+=2;
        if((lun=read(fpDAT,pb,2042))!=2042)
                {
                printf("\n lun_2042 = %d",lun);
                return(0);
                }
        pb+=2042;
        tot_letti+=lun;
        }
if(lun_parziale)
        {
        lun=read(fpDAT,sep,2);  /* legge il separatore */
        if(lun!=2) return(0);
        tot_letti+=2;
        if((lun=read(fpDAT,pb,lun_parziale))!=lun_parziale)
                {
                printf("\n lun_parziale %d = %d",lun_parziale,lun);
                return(0);
                }
        tot_letti+=lun;
        }
#else
if (linguaggio_eseguibile == F22_FORTRAN)
        {
        fseek(fpDAT, 4, SEEK_CUR);
        }
tot_letti=fread(pb,1,lun_rec_dati,fpDAT);
if ((tot_letti!=lun_rec_dati)&&(tot_letti!=0))
	{
	while(tot_letti<lun_rec_dati)
		tot_letti+=fread(&pb[tot_letti],1,
			(lun_rec_dati-tot_letti),fpDAT);
	}
if (linguaggio_eseguibile == F22_FORTRAN)
        {
        fseek(fpDAT, 4, SEEK_CUR);
        }
if(tot_letti==lun_rec_dati)
	{
	memcpy(&tempo,&pb[0],sizeof(float));
	}
#endif
return(tot_letti);
}




void rew_dati(void)
{
int ret;

n_last=0;
#if defined VMS_FORTRAN
lseek(fpDAT,inizio_dati,0);
#else
/*
fseek(fpDAT,inizio_dati,0);
*/
printf("rew_dati -> inizio_dati=%d\n",inizio_dati);
ret=fseek(fpDAT,0,0);
ret=fseek(fpDAT,inizio_dati,0);
fsetpos(fpDAT,&posizione_iniziale);
if(ret!=0)
	{
	printf("errore rew_dati\n");
	}
#endif
}


/*
 *  set_min_max
 *      aggiorna i valori di minimo e massimo per ogni variabile 
 *      appartenente al record.
 *		se il parametro passato come argomento e' =NULL inizializza
 *      a valori estremi i valori di minimo e massimo.
 */

set_min_max(rec)
S_DATI *rec;   /* record dati */
{
register int i;
float delta;
int variato;
int nvar;
        if (linguaggio_eseguibile == F22_C)
            {
	    nvar=header1_C.nvar;
	    }
        else
            {
	    nvar=header1_FORTRAN.nvar;
	    }
for(i=0;i<nvar;i++)
        {
        if(rec==NULL)
                {
                min_max[i].max=(-1.0E-37);
                min_max[i].min=1.0E+38;
                }
        else
                {
                variato=0;
                if(min_max[i].min>rec->mis[i])
                        {
                        min_max[i].min=rec->mis[i];
                        variato=1;
                        }
                if(min_max[i].max<rec->mis[i])
                        {
                        min_max[i].max=rec->mis[i];
                        variato=1;
                        }
                if(variato && (min_max[i].max>=min_max[i].min)
                    && (min_max[i].max-min_max[i].min)<=(0.001*min_max[i].max))
                        {
                        delta=min_max[i].max * 0.001; 
                        if(delta <0.001) delta=0.001;
                        min_max[i].max+=delta;
                        min_max[i].min-=delta;
                        }
                }
	}
}

#if defined VMS_FORTRAN
read_nomi(fp,offset)
int *offset;
int fp;
{
int row,col,dimens;
char *pdata;
int max_dim;  /* massima dimensione del blocco fortran */
int sizebuf,sizeprec;  /* dimensione del blocco allocato */
short sep[2]; /* separatore di blocchi (scrittura fortran) */
char *pbuf;   /* puntatore buffer di appoggio  */
char *papp;
int pos,newpos;
int lun;
int ultimo;
int nvar;

register int i,k;
/*
  lettura del primo header
*/
max_dim=2042;
sizebuf=2044;
pbuf=XtCalloc(sizebuf,sizeof(char));
if(pbuf==NULL) printf("\n manca memoria per calloc");

pos=(*offset);
if (linguaggio_eseguibile == F22_C)
    {
    if((lun=read(fp,pbuf,sizeof(S_HEAD1_C)-6))==0)
        {
        printf("\n dati non presenti in F22.DAT");
        exit(1);
        }
    }
else
    {
    if((lun=read(fp,pbuf,sizeof(S_HEAD1_FORTRAN)-6))==0)
        {
        printf("\n dati non presenti in F22.DAT");
        exit(1);
        }
    }
if(lun==-1)
        {
        printf("\n errore in lettura");
        exit(1);
        }
pos+=lun;
if (linguaggio_eseguibile == F22_C)
    {
    memcpy(&header1_C,pbuf,sizeof(S_HEAD1_C)-6);
    }
else
    {
    memcpy(&header1_FORTRAN,pbuf,sizeof(S_HEAD1_FORTRAN)-6);
    }
ultimo=0;
pos=lseek(fp,0,1);
newpos=lseek(fp,pos,0);
if(newpos==-1) {perror("\ngraf:");XtFree(pbuf); return(1);}
lun=read(fp,pbuf,sizebuf); /* legge il primo blocco */
if(lun==-1)
        {
        printf("\n errore in lettura");
        XtFree(pbuf);
        perror("\n graf:");
        return(1);
        }
if(lun<max_dim) ultimo=1;
pos+=lun;
if (linguaggio_eseguibile == F22_C)
    {
    memcpy(&header1_C.void2[0],pbuf,6);  /* copia spare e numero variabili */
    }
else
    {
    memcpy(&header1_FORTRAN.void2[0],pbuf,6);  /* copia spare e numero variabili */
    }
sizeprec=sizebuf;
sizebuf=max_dim;
/*
   legge tutta la parte variabili del file
*/
while(ultimo==0)
        {
        if(read(fp,sep,2)!=2) break;  /* legge il separatore */
        pos+=2;
        sizebuf+=max_dim;
        pbuf=XtRealloc(pbuf,sizebuf);
        if(pbuf==NULL) printf("\n manca memoria per realloc");
        if((lun=read(fp,&pbuf[sizeprec],max_dim))!=max_dim)
                {pos+=lun; ultimo=1; break;}
        (*offset)+=(max_dim);
        sizeprec+=max_dim;
        pos+=lun;
        }
papp=pbuf+6;
if (linguaggio_eseguibile == F22_C)
    {
    nvar = header1_C.nvar; 
    }
else
    {
    nvar = header1_FORTRAN.nvar; 
    }
for(i=0;i<nvar;i++)
        {
        memcpy(nomi_misure[i],papp,LUN_NOME);
        papp+=LUN_NOME;
        nomi_misure[i][LUN_NOME]=0;
        }
nomi_misure[i][0]=0;  /* tappo */
memcpy(&header2,papp,sizeof(S_HEAD2));
papp+=sizeof(S_HEAD2);
/*
        alloca l'array bidimensionale che conterra' la lista delle
        descrizioni.
*/
row=header2.ncasi+1;
col=LUN_SIMB+1;
pdata = (char *) XtCalloc(row * col,  sizeof(char));
if(pdata==(char *)NULL) {
                printf("\n mancanza di spazio per allocazione");
                exit(1);
                }
dimens=row * (sizeof(char *));
simboli = (char **) XtCalloc(row,sizeof(char *));
if(simboli==(char **)NULL) {
                printf("\n mancanza di spazio per allocazione");
                exit(1);
                }
for(i=0;i<row;i++)
                {
                simboli[i]=pdata;
                pdata+=col;
                }
for(i=0;i<header2.ncasi;i++)
        {
        memcpy(simboli[i],papp,LUN_SIMB);
        simboli[i][LUN_SIMB]=(char)0;
        papp+=LUN_SIMB;
        (*offset)+=LUN_SIMB;
        }
simboli[i][0]=(char)0;
XtFree(pbuf);
(*offset)=pos;
return(0);
}
#else
read_nomi(fp,offset)
int *offset;
FILE *fp;
{
int row,col,dimens;
char *pdata;
int i,k;
int nvar;
/*
  lettura del primo header
*/
if (linguaggio_eseguibile == F22_C)
    {
    if((fread(&header1_C,sizeof(S_HEAD1_C),1,fp))==(int)NULL)
        {
        printf("\n dati non presenti in F22.DAT (lettura S_HEAD1_C)");
        exit(1);
        }
    (*offset)+=sizeof(S_HEAD1_C);
    nvar=header1_C.nvar;
    }
else
    {
    if((fread(&header1_FORTRAN,sizeof(S_HEAD1_FORTRAN),1,fp))==(int)NULL)
        {
        printf("\n dati non presenti in F22.DAT (lettura S_HEAD1_FORTRAN)");
        exit(1);
        }
    (*offset)+=sizeof(S_HEAD1_FORTRAN);
    nvar=header1_FORTRAN.nvar;
    }


for(i=0;i<nvar;i++)
        {
        fread(nomi_misure[i],LUN_NOME,1,fp);
        for(k=0;k<LUN_NOME;k++)
                if(nomi_misure[i][k]==' ') nomi_misure[i][k]=0;
        nomi_misure[i][LUN_NOME]=0;
        (*offset)+=LUN_NOME;
        }
nomi_misure[i][0]=0;  /* tappo */

#if defined LINUX
fread(&header2,sizeof(S_HEAD2),1,fp);
(*offset)+=sizeof(S_HEAD2);
#else
fread(header2.nome_mod,LUN_NOME,1,fp);
fread(&header2.ncasi,sizeof(int),1,fp);
(*offset)+= (LUN_NOME+sizeof(int));
#endif
/*
        alloca l'array bidimensionale che conterra' la lista delle
        descrizioni.
*/
row=header2.ncasi+1;
col=LUN_SIMB+1;
pdata = (char *) XtCalloc(row * col,  sizeof(char));
if(pdata==(char *)NULL) {
                printf("\n mancanza di spazio per allocazione");
                exit(1);
                }
dimens=row * (sizeof(char *));
simboli = (char **) XtCalloc(row,sizeof(char *));
if(simboli==(char **)NULL) {
                printf("\n mancanza di spazio per allocazione");
                exit(1);
                }
for(i=0;i<row;i++)
                {
                simboli[i]=pdata;
                pdata+=col;
                }

for(i=0;i<header2.ncasi;i++)
        {
        fread(simboli[i],LUN_SIMB,1,fp);
        simboli[i][LUN_SIMB]=(char)0;
        (*offset)+=LUN_SIMB;
        }
simboli[i][0]=(char)0;

if (linguaggio_eseguibile == F22_FORTRAN)
    {
    fseek(fp, 4, SEEK_CUR);
    }

fgetpos(fp,&posizione_iniziale);
return(0);
}
#endif
/*
 * open_gruppi()
 *   routine di apertura in lettura-scrittura del file contenente le 
 *   informazioni relative ai gruppi: se l'apertura in lettura non 
 *   riesce viene creato un nuovo file per gruppi inizializzato a 0.
 */
int open_gruppi()
{
int i,k;
char appoggio[150];
appoggio[0]=0;
strcpy(appoggio,path_name);
strcat(appoggio,"f22_fgraf.dat");
fpGR=fopen(appoggio,"r");
if(fpGR==NULL)
	{
/* 
  se il file non era apribile in lettura lo inizializza a zero
*/

	fpGR=fopen(appoggio,"w");

	if(fpGR == NULL)
		{
		printf("\n errore apertura file gruppi\n");
		return(1);
		}
	for(i=0;i<NUM_GRUPPI;i++)
		{
		for(k=0;k<4;k++)
			{
			gruppi[i].gr.ind_mis[k]=(int)-1;
			gruppi[i].gr.autoscaling[k]=1;
			}
		fwrite(&gruppi[i],sizeof(REC_GRUPPO),1,fpGR);
		}
	}
fclose(fpGR);
fpGR=fopen(appoggio,"r+");
if(fpGR==NULL)
	{
	fpGR=fopen(appoggio,"r");
	}
return(0);
}


close_gruppi()
{
int i;
for(i=0;i<NUM_GRUPPI;i++)
	{
	XmStringFree(x_gruppi[i]);
	}
fclose(fpGR);
}


read_gruppi(flag)
int flag;  /*  flag == 1 se si desidera la lista per fase di inserimento
               gruppi  */
{
int i;
int ngr;
ngr=0;
for(i=0;i<NUM_GRUPPI;i++)
	{
	fread(&gruppi[i],sizeof(REC_GRUPPO),1,fpGR);
	if(gruppi[i].pieno)
		{
		ngr++;
		sprintf(lista_gruppi[i],"%.2d - %s",i+1,gruppi[i].descr);
		}
	else if (flag==1)
	sprintf(lista_gruppi[i],"%.2d - disponibile -           ",
                i+1);
	x_gruppi[i]=XmStringGenerate(lista_gruppi[i], NULL, XmCHARSET_TEXT, NULL);
	}
x_gruppi[i]=NULL;
return(ngr);
}

write_gruppo(indice)
int indice;
{
unsigned long offset;
offset=(long)indice*(long)sizeof(REC_GRUPPO);
fseek(fpGR,offset,0);
if(fwrite(&gruppi[indice],sizeof(REC_GRUPPO),1,fpGR)!=1)
	return(1); /* ritorno con errore in scrittura */
else
	return(0);
}

/*
 * open_path()
 *	legge i path names memorizzati sul file F22_FILES.DAT 
 */
FILE *fpPATH;	        
extern char path[NUM_PATH_FILES][LUN_PATH_FILES];
open_path()
{
int i;
fpPATH=fopen("f22_files.dat","r");
if(fpPATH==NULL)
        {
/* 
  se il file non era apribile in lettura lo inizializza a zero
*/
	fpPATH=fopen("f22_files.dat","w");

	if(fpPATH == NULL)
		{
		printf("\n errore apertura file path\n");
		exit(0);
		}
	for(i=0;i<NUM_PATH_FILES;i++)
		fwrite(path[i],LUN_PATH_FILES,1,fpPATH);
	}
fclose(fpPATH);
fpPATH=fopen("f22_files.dat","r+");
for(i=0;i<NUM_PATH_FILES;i++)
	fread(path[i],LUN_PATH_FILES,1,fpPATH);
fclose(fpPATH);
}
             
/*
 * close_path()
 *    salva i valori attuali dei path names e chiude il file 
 *    F22_FILES.DAT
 */
void close_path()
{
int i;
fpPATH=fopen("f22_files.dat","r+");
fseek(fpPATH,0,0);
for(i=0;i<NUM_PATH_FILES;i++)
	fwrite(path[i],LUN_PATH_FILES,1,fpPATH);
fclose(fpPATH);
}
                                                  


d2free(prow) 
char **prow;
{
                             
XtFree(*prow);
XtFree(prow);
}
