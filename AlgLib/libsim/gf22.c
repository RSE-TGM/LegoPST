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
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)gf22.c	5.1\t11/7/95";
/*
   modulo gf22.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)gf22.c	5.1
*/
/*
 * gf22.c
 *    contiene le routine di accesso al file disco f22 ( su macchine ULTRIX) 
      all' area shared che simula il file (su macchine AIX / VMS)
 */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>                /* Motif Toolkit and MRM */

#include "sim_param.h"
#include "sim_types.h"
#include "sim_ipc.h"

#include "f22.h"
// #define Boolean anotherBOOLtype
#include <Rt/RtMemory.h>

#if defined AIX || VMS
DATI_GRAF *pshrf22;             /* puntatore buffer circolare */
                 
int id_sh_f22;                 /* identificatore buffer circolare  */
extern S_MIS *bufdati;  	/* buffer circolare contenente i campioni letti 				   da file   */
#endif

#if defined UNIX && !defined AIX
extern S_DATI bufdati[];
S_HEAD1 header1;
S_HEAD2 header2;
FILE *fpDAT;
#endif

/*
 variabili di uso comune alle routines di I/O
 */
extern S_MIN_MAX min_max[NUM_VAR];
			   /* valori minimi e massimi per ogni variabile  */

extern int num_var_graf;   /* numero di variabili in registrazione*/ 
extern int n_last;         /* posizione dell'ultimo campione nel buffer circolare */

/*
 open_22dat
   apre il file f22.dat 
*/

void open_22dat()
{
int shr_usr_key;

#if defined AIX || VMS
shr_usr_key = atoi((char *)getenv("SHR_USR_KEY"));
pshrf22 = (DATI_GRAF *) crea_shrmem(shr_usr_key+ID_SHM_GRAF,sizeof(DATI_GRAF),&id_sh_f22);
bufdati=(S_MIS *) &pshrf22->ord[0][0];
#endif

#if defined UNIX && !defined AIX
fpDAT=fopen("f22","r");
if(fpDAT==NULL)
        {
        printf("\n apertura non riuscita file f22");
        exit(1);
        }
#endif
}

/*
 close_22dat
   chiude il file f22.dat
*/
void close_22dat()
{
#if defined AIX || VMS
   distruggi_shrmem(id_sh_f22);
#endif

#if defined UNIX && !defined AIX
	fclose(fpDAT);
#endif
}


/*
 read_22dat
    legge i dati relativi a tutte le misure;
	- flag : AGGIORNA    aggiorna l'area dati con gli ultimi dati acquisiti
	         TUTTI       trasferisce nell'area dati tutti i campioni presenti
*/

#if defined AIX || VMS

float tsec;   /* tempo in secondi da memorizzare nel buffer dati; 
         	 viene incrementato di INC_SEC secondi per ogni campione*/
int read_22dat(flag,dim_bufcirc,num_camp_z0,inc_sec)
char flag;
int dim_bufcirc;	/* dimensione massima buffer circolare DIM_BUFCIRC*/
int num_camp_z0;	/* utili solo versione ULTRIX*/
int inc_sec;		/*   "     "     "       "   */
{
static S_DATI buf;   /* buffer di appoggio per la lettura del record dati */
float step,correz;
int ultimo;
int i,lun;
int ind;      /* indice che scorre il buffer di memorizzazione dati
				 bufval   */
int pieno;   /* indica se il buffer e' stato completamente riempito 
				(variabile utilizzata in fase di prima lettura) */

int kk;

if(flag==TUTTI)
	{
	tsec=0.0;
	n_last=pshrf22->npu;
	if(n_last<0)
		n_last=dim_bufcirc-1;
	ind=0;
	pieno=0;
/*
  inizializza il buffer dei minimi e massimi a valori estremi
*/
	set_min_max(NULL);

/*
 calcola i valori di minimo e massimo su tutto il buffer circolare
*/
	ultimo=n_last-1;
	if(ultimo<0) ultimo=dim_bufcirc-1;
	while(n_last!=ultimo)
		{
		memcpy(&buf.mis[0],&pshrf22->ord[n_last][0],sizeof(S_MIS));
		buf.t=pshrf22->asc[n_last];
		if(!(buf.t==0 && n_last!=0))
			set_min_max(&buf);
		if(n_last<dim_bufcirc-1)
			n_last++;
		else
			n_last=0;
		}
	}
else  /* caso di lettura per aggiornamento  */
	{
/* esamina nel buffer circolare condiviso tutte le nuove variabili
   acquisite aggiornando i valori di massimo e minimo
*/
	ultimo=pshrf22->npu-1;
	if(ultimo<0)  /*  modifica 26-8-91 da : ultimo=dim_bufcirc; */
		ultimo=dim_bufcirc-1;
	while(n_last!=ultimo)
		{
		memcpy(&buf.mis[0],&pshrf22->ord[n_last][0],sizeof(S_MIS));
		buf.t=pshrf22->asc[n_last];
		if(!(buf.t==0 && n_last!=0))
			set_min_max(&buf);
		if(n_last<dim_bufcirc-1)
			n_last++;
		else
			n_last=0;
		}
	}
return(0);
}
#endif

#if defined UNIX && !defined AIX

float tsec;   /* tempo in secondi da memorizzare nel buffer dati; 
         	 viene incrementato di INC_SEC secondi per ogni campione*/
unsigned long off_f22;
int lun_rec_dati;  /* lunghezza record dati */

void read_22dat(flag,dim_bufcirc,num_camp_z0,inc_sec)
char flag;
int dim_bufcirc;        /* dimensione massima buffer circolare DIM_BUFCIRC*/
int num_camp_z0;        /* utili solo versione ULTRIX*/
int inc_sec;            /*   "     "     "       "   */
{
S_DATI buf;   /* buffer di appoggio per la lettura del record dati */
static S_DATI bufprec;  /* conserva i dati letti all'iterazione
precedente */
float step,correz;
int i;
int ind;      /* indice che scorre il buffer di memorizzazione dati
                                 bufval   */
int pieno;   /* indica se il buffer e' stato completamente riempito
                        (variabile utilizzata in fase di prima lettura)
*/
if(flag==TUTTI)
        {
        tsec=0.0;
        n_last=0;
        ind=0;
        pieno=0;
        off_f22=0L;
        read_nomi(fpDAT,&off_f22); /* legge le tabelle dei nomi e
simboli */
        fseek(fpDAT,off_f22,0);
        lun_rec_dati=(header1.nvar+1)*sizeof(float);
/*
  inizializza il buffer dei minimi e massimi a valori estremi
*/
        set_min_max(NULL);
/*
  lettura del primo campione: ad esso dovra' corrispondere un tempo
  pari a 0.0 secondi.
*/
        if((fread(&bufprec,lun_rec_dati,1,fpDAT))==NULL)
                {
                printf("\n dati non presenti in F22.DAT");
                exit(1);
                }
        off_f22+=lun_rec_dati;
/*
  aggiorna se necessario i valori di minimo e di massimo
*/
        set_min_max(&bufprec);
/*
  se il primo dato presente non e' stato raccolto all'istante zero
  riempie gli istanti precedenti di valori uguali a quelli presenti
  nel primo campione;
  memorizza comunque il dato di tempo 0.0
*/
        while(tsec <= bufprec.t)
                {
                bufdati[ind++]=bufprec;
                tsec+=inc_sec;
                }
        }
else  /* caso di lettura per aggiornamento  */
        {
        fseek(fpDAT,off_f22,0);
        ind=n_last;
        }

  
while(fread(&buf,lun_rec_dati,1,fpDAT))
                {
                off_f22+=lun_rec_dati;
                set_min_max(&buf);
                while(buf.t<tsec)
                        {
                        bufprec=buf;
                        fseek(fpDAT,off_f22,0);
                        if(fread(&buf,lun_rec_dati,1,fpDAT)==0)
                                goto FINE;
                        off_f22+=lun_rec_dati;
                        set_min_max(&buf);
                        }
                bufdati[ind].t=tsec;
                if(buf.t==tsec)
                        bufdati[ind]=buf;
                else     /* esegue una interpolazione lineare  */
                        {
                        step=buf.t-bufprec.t;
                        correz=tsec-bufprec.t;
                        for(i=0;i<header1.nvar;i++)
                                {
                                bufdati[ind].mis[i]=bufprec.mis[i]+

((buf.mis[i]-bufprec.mis[i])/step)*correz;
                                }
                        }
                tsec+=inc_sec;
                if(tsec>buf.t)
                        bufprec=buf;
                if(ind<(num_camp_z0-1))
                        ind++;
                else if (ind<(dim_bufcirc-1))
                        {ind++;  pieno=1;}
                else ind=0;
                fseek(fpDAT,off_f22,0);
                }
/*
 e' terminata la scansione del file f22.dat; ind punta alla posizione
suc- cessiva all'ultima riempita.
*/
FINE:
n_last=ind;
/*
 se e' nel caso di lettura globale del file e non e' stato riempito
 completamente il buffer, lo completa utilizzando come valore il primo
 acquisito che andra a riempire istanti di tempo precedenti al primo
*/
if(flag==TUTTI && !pieno)
        {
        while(ind<dim_bufcirc)
                {
                bufdati[ind]=bufdati[0];
                bufdati[ind].t=(float)(-(dim_bufcirc-ind));
                ind++;
                }
        }
}
    
void read_nomi(fp,offset)
FILE *fp;
unsigned long *offset;
{
int i,k;

fseek(fp,*offset,0);
/*
  lettura del primo header
*/
if((fread(&header1,sizeof(S_HEAD1),1,fp))==NULL)
        {
        printf("\n dati non presenti in F22.DAT");
        exit(1);
        }
(*offset)+=sizeof(S_HEAD1) + (LUN_NOME*header1.nvar);
fseek(fpDAT,*offset,0);

if((fread(&header2,sizeof(S_HEAD2),1,fp))==NULL)
        {
        printf("\n dati non presenti in F22.DAT");
        exit(1);
        }
(*offset)+=sizeof(S_HEAD2) + (LUN_SIMB*header2.ncasi);
}
#endif                        


/*
 *  set_min_max
 *      aggiorna i valori di minimo e massimo per ogni variabile 
 *      appartenente al record.
 *		se il parametro passato come argomento e' =NULL inizializza
 *      a valori estremi i valori di minimo e massimo.
 */

void set_min_max(rec)
S_DATI *rec;   /* record dati */
{
short i;
float delta;
int variato;
for(i=0;i<num_var_graf;i++)
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
	
