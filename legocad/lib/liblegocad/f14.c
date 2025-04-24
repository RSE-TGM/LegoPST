/**********************************************************************
*
*       C Source:               f14.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Feb 24 16:28:00 2004 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: f14.c-2.1.1 %  (%full_filespec: f14.c-2.1.1:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)f14.c	2.14\t1/17/96";
/*
        Fine sezione per SCCS
*/
/*
 f14.c
    routine di lettura del file f14 per ricavare i valori di input -
    output e dati in formato binario per il modello in esame.

    Vengono creati per la lettura due vettori valinp e valout di
    strutture che descrivono per ogni variabile: 
		- valore 
		- se la variabile e' nota
		- indice unita' di misura
		- unita' di misura selezionata da utente.
    (valinp e valout sono paralleli a nom_sivar e nom_vari).

    Per i dati viene creato un vettore parallelo a IP (ipd) che per ogni
    blocco punta ad una sezione del vettore dei dati (dati).
    Il vettore dati e' formato da records consecutivi che possoo
    contenere una stringa di descrizione di una sotto-sezione dati.
*/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <f03.h>
#include <f14.h>
#include <uni_mis.h>
extern S_UNI_MIS uni_mis[];
#ifndef OSF1
extern int nbl;
#else
extern _INTEGER nbl;
#endif

char riga_intesta[L_RIGA_F14+1];
char riga_norm[2][L_RIGA_F14+1];
char dati_norm[MAX_DATI_NORM][7];
char descr_norm[MAX_DATI_NORM][5];
VAL_VAR *valout;
VAL_VAR *valinp;
int tot_righe_dati,tot_descr;
DATO_RIGA *array_dati;
DATO_DESCR *array_descr;
int num_note; /* numero delle variabili definite come note */
HEADER_DATI *header_dati; /* tabella parallela a nom_bloc che per ogni
                             blocco contiene il puntatore all'array
                             dei dati e all'array delle descrizioni
			   */

extern char **nom_bloc;
extern int cerca_umis(char*);

read_file_f14(neqsis,nu)
int neqsis;    /* numero eq del sistema (= no.uscite) */
int nu;        /* numero ingressi */
{
FILE *fp_f14;
if ( (fp_f14 = fopen( FILE_F14, "r")) == NULL)
	{
	return(1);
	}
crea_array_valori_f14(neqsis,nu);
read_valori_f14(fp_f14,neqsis,nu);
crea_array_dati_f14(fp_f14);
read_dati_f14(fp_f14);
#ifdef DEBUG
printf("\n RIGA DI INTESTAZIONE LETTA (DOPO DATI) = %s LUNGH.=%d",riga_intesta,strlen(riga_intesta));
#endif
fclose(fp_f14);
return(0);
}

read_valori_f14(fp_f14,neqsis,nu)
FILE *fp_f14;  /* file pointer al file f14 in lettura) */
int neqsis;    /* numero eq del sistema (= no.uscite) */
int nu;        /* numero ingressi */
{
int i,numval;
char temp[L_RIGA_F14+1]; /* buffer per lettura f14 (riga per riga:
                             ogni riga e' di 132+1 caratteri */
/*
 legge la riga di intestazione del file 14
*/
while(fgets(temp,L_RIGA_F14+1,fp_f14) != NULL &&
         strncmp(temp,HEADER_F14,strlen(HEADER_F14)) );
elim_newline(temp);
strcpy(riga_intesta,temp);
#ifdef DEBUG
printf("\n RIGA DI INTESTAZIONE LETTA = %s",riga_intesta);
printf("\n NUMERO BLOCCHI LETTI = %d",nbl);
#endif
/*
 legge i dati di normalizzazione
*/
while(fgets(temp,L_RIGA_F14+1,fp_f14) != NULL &&
         strncmp(temp,HEADER_NORM_F14,strlen(HEADER_NORM_F14)) );
strcpy(riga_norm[0],temp);
fgets(temp,L_RIGA_F14+1,fp_f14);
strcpy(riga_norm[1],temp);
sscanf(riga_norm[0],
"%*28c%2c%*c%6c%*c%2c%*c%6c%*c%2c%*c%6c%*c%2c%*c%6c%*c%2c%*c%6c%*c"
     ,descr_norm[0],dati_norm[0],
      descr_norm[1],dati_norm[1],
      descr_norm[2],dati_norm[2],
      descr_norm[3],dati_norm[3],
      descr_norm[4],dati_norm[4]
      );
sscanf(riga_norm[1],
"%*28c%2c%*c%6c%*c%2c%*c%6c%*c%3c%*c%6c%*c",
      descr_norm[5],dati_norm[5],
      descr_norm[6],dati_norm[6],
      descr_norm[7],dati_norm[7]);
#ifdef DEBUG
printf("\n RIGA DI INTESTAZIONE LETTA 2= %s",riga_intesta);
for(i=0;i<8;i++)
	printf("\ndati[%d] = %s",i,dati_norm[i]);
#endif
/*
 nel caso non siano stati specificati nel file 14 inserisce i valori di defaults
 per i  dati di normalizzazione
*/
set_defaults_norm(); 
/*
 cerca l'inizio delle condizioni iniziali per i dati di
 output
*/
while(fgets(temp,L_RIGA_F14+1,fp_f14) != NULL &&
         strncmp(temp,HEADER_VAR_OUT,strlen(HEADER_VAR_OUT)) );
/* 
 inserisce nei vettori dei dati i valori dati di uscita
*/
i=0;
num_note=0; /* inizializza il contatore delle variabili note */
numval=neqsis;
while(numval && fgets(temp,L_RIGA_F14+1,fp_f14) != NULL)
	{
	temp[L_RIGA_F14-1]=0;
	estr_dati(temp,&valout[i]);
	if(valout[i].noto) num_note++;
	i++;
	numval--;
	}
if(numval)
	printf("\nnumero di uscite incongruente: non trovate %d",numval);
/*
 cerca l'inizio delle condizioni iniziali per i dati di
 input
*/
while(fgets(temp,L_RIGA_F14+1,fp_f14) != NULL &&
         strncmp(temp,HEADER_VAR_INP,strlen(HEADER_VAR_INP)) );
/*
 inserisce nei vettori dei dati i valori dati di ingresso
*/
i=0;
numval=nu;
while(numval && fgets(temp,L_RIGA_F14+1,fp_f14) != NULL)
        {
	elim_newline(temp);
/*	temp[L_RIGA_F14-1]=0; */
	estr_dati(temp,&valinp[i]);
	if(valinp[i].noto) num_note++;
        i++;
        numval--;
        }
if(numval)
	printf("\nnumero di ingressi incongruente: non trovate %d",numval);
/* stampa_dati(neqsis,nu); */
#ifdef DEBUG
printf("\n RIGA DI INTESTAZIONE LETTA 3= %s",riga_intesta);
#endif
}

estr_dati(temp,val)
char *temp;
VAL_VAR *val;
{
char strapp[2]; /* stringa di appoggio */
strapp[1]=0;
/* esamina in base alla prima lettera della sigla quale e'
   l'unita' di misura utilizzata (nel caso di indice = 0)
   si tratta di digitale */
        val->ind_umis=cerca_umis(&temp[OFF_SIGLA]);

/* estrae il valore */
        sscanf(&temp[OFF_VAL],"%10f",&val->val);
/* esamina se variabile definita come nota */
        if(!strncmp(&temp[OFF_NOTO],VAR_NOTA,strlen(VAR_NOTA)))
                val->noto=1;
        else
                val->noto=0;
/*
 viene utilizzata la prima unita' di misura nell'elenco
*/
        val->sel_umis=0;
}
   
stampa_dati(neqsis,nu)
int neqsis;
int nu;
{
int i;

for(i=0;i<neqsis;i++)
	{
printf("\n VARIABILI DI USCITA ");
	pr_float(stdout,valout[i].val);
	if(valout[i].noto)
		printf(VAR_NOTA);
        printf("     %d",valout[i].sel_umis+1);
	printf(" %s\n",uni_mis[valout[i].ind_umis].codm[valout[i].sel_umis]);
	}

for(i=0;i<nu;i++)
        {
printf("\n VARIABILI DI INGRESSO ");
	printf("\n");
        pr_float(stdout,valinp[i].val);
	printf("     ");
        if(valinp[i].noto)
                printf(VAR_NOTA);
	printf("     %d",valinp[i].sel_umis+1);
	printf(" %s\n",uni_mis[valinp[i].ind_umis].codm[valinp[i].sel_umis]);
        }
}

pr_float(fp,val)
FILE *fp;
float val;
{
if(val<999.99 && val>-99.99)
	fprintf(fp,"%-7.3f",val);
else
	fprintf(fp,"%-.4E",val);
}

crea_array_valori_f14(neqsis,nu)
int neqsis;   /* numero eq del sistema (= no.uscite) */
int nu;	      /* numero ingressi */
{
valout=(VAL_VAR *) calloc(neqsis,sizeof(VAL_VAR));
valinp=(VAL_VAR *) calloc(nu,sizeof(VAL_VAR));
}

crea_array_dati_f14(fp_f14)
FILE *fp_f14;
{
char temp[L_RIGA_F14+1];  /* buffer per lettura f14 (riga per riga:
                             ogni riga e' di 132 caratteri */
int i,f_posiz,pos,pos_descr;
int flag_descr;
/*
 Alloca per ogni blocco una struttura  per contenere la descrizione ed il
 puntatore all'inizio dei corrispondenti dati
*/
header_dati= (HEADER_DATI *) calloc(nbl,sizeof(HEADER_DATI));
/*
 Cerca il primo blocco dati
*/
while(fgets(temp,L_RIGA_F14+1,fp_f14) != NULL &&
         strncmp(temp,HEADER_BLOCCO_F14,strlen(HEADER_BLOCCO_F14)) );
/*
 Salva la posizione del primo blocco dati (da utilizzarsi per la successiva
 lettura dati).
*/
f_posiz=ftell(fp_f14);
/*
 Per ogni blocco verifica se sono presenti dati ad esso relativi ed inserisce
 il valore nel campo num_dati. Calcola l'inizio del blocco dati nell'array
 dei dati che sara' successivamente allocato.
*/
pos=0; /* posizione nel vettore dei dati */
pos_descr=0; /* posizione delle descrizioni */

for(i=0;i<nbl;i++)
	{
	flag_descr=0;
	temp[strlen(temp)-1]=0; /* sostituisce al newline il 
                                   fine stringa */
	if(strncmp(&temp[OFF_NOME_BLO_DAT],nom_bloc[i],8))
		{
		printf("\n error in section dati of f14.dat");
		printf("\n riga letta %s",temp);
		printf("\n nome cercato %.8s",nom_bloc[i]);
		return (-1);
		}
	strcpy(header_dati[i].titolo,temp);
/*
 Conta il numero di dati presenti in base agli asterischi ed esamina se vi sono
 descrizioni 
*/
	header_dati[i].inizio_descr=pos_descr;
	header_dati[i].inizio=pos;
	while(fgets(temp,L_RIGA_F14+1,fp_f14) != NULL &&
	strncmp(temp,HEADER_BLOCCO_F14,strlen(HEADER_BLOCCO_F14)) )
		{
		elim_newline(temp); /* sostituisce il newline con NULL */
	/*	temp[L_RIGA_F14-1]=0; */
	 	if(strncmp(temp,DESCRIZIONE_IN_DATI,
                           strlen(DESCRIZIONE_IN_DATI))==0) 
/* si tratta di una riga di commento*/
			{
			header_dati[i].num_descr++;
			pos_descr++;
			}	
		else
			{
			if(temp[OFF_END_DATO1]=='*' && 
				strlen(temp)>= (OFF_END_DATO1+1))
				{
				header_dati[i].num_righe_dati++;
				pos++;
				}
			}
		}
	}
/*
 calcola le dimensioni globali dell'area dati e dell'area descrizioni
 e le alloca 
*/
tot_righe_dati=0;
tot_descr=0;
for(i=0;i<nbl;i++)
	{
	tot_righe_dati+=header_dati[i].num_righe_dati;
	tot_descr+=header_dati[i].num_descr;
	}

#ifdef DEBUG
printf("tot_descr=%d\n",tot_descr);
printf("tot_righe_dati=%d\n",tot_righe_dati);
#endif

array_dati= (DATO_RIGA *)calloc(tot_righe_dati+1,sizeof(DATO_RIGA));
array_descr= (DATO_DESCR *)calloc(tot_descr+1,sizeof(DATO_DESCR)); 
/*
 si riposiziona all'inizio della parte dati
*/
fseek(fp_f14,f_posiz,0);
}

read_dati_f14(fp_f14)
FILE *fp_f14;
{
int i;
char temp[L_RIGA_F14+2];  /* buffer per lettura f14 (riga per riga:
                             ogni riga e' di 132 caratteri */
/* era L_RIGA_F14+2 */
int ind_riga_dato,ind_descr;  /* posizioni all'interno degli array dei dati
                            e delle descrizioni */
int flag_descr; /* indica se e' stata trovata una descrizione */
/*
 E' gia' posizionato sul primo blocco dati
*/
for(i=0;i<nbl;i++)
	{
	flag_descr=0;
	ind_riga_dato=header_dati[i].inizio;
	ind_descr=header_dati[i].inizio_descr;
	while(fgets(temp,L_RIGA_F14+2,fp_f14) != NULL &&
	strncmp(temp,HEADER_BLOCCO_F14,strlen(HEADER_BLOCCO_F14)) )
		{
		elim_newline(temp);
				 /* sostituisce al newline il fine
                                           stringa */
	 	if(strncmp(temp,DESCRIZIONE_IN_DATI,
                           strlen(DESCRIZIONE_IN_DATI))==0) 
/* si tratta di una riga di commento*/
			{
			if(flag_descr)
				array_descr[ind_descr-1].continua=1;
			strcpy(array_descr[ind_descr].descr,temp);
			array_descr[ind_descr].dato_seguente=ind_riga_dato;
			ind_descr++;
			flag_descr=1;
			}	
		else
			{
			if(temp[OFF_END_DATO1]=='*' && 
                           strlen(temp) >= (OFF_END_DATO1+1))
				{
			/* estrae il valore */
				strncpy(array_dati[ind_riga_dato].dato[0].val,
					&temp[OFF_INI_DATO1],10);
				strncpy(array_dati[ind_riga_dato].dato[0].descr,
                                         &temp[OFF_INI_DESC1],8);
				if(flag_descr)
					{
/* 3-5-95 Micheletti
					array_descr[ind_descr].dato_seguente=ind_riga_dato;
					array_descr[ind_descr].continua=0;
*/
					flag_descr=0;
					}
				array_dati[ind_riga_dato].num_dati++;
				}
			if(temp[OFF_END_DATO2]=='*' &&
                           strlen(temp) >= (OFF_END_DATO2+1))
                        	{
			/* estrae il valore */
				strncpy(array_dati[ind_riga_dato].dato[1].val,
					&temp[OFF_INI_DATO2],10);
				strncpy(array_dati[ind_riga_dato].dato[1].descr,
                                         &temp[OFF_INI_DESC2],8);
				array_dati[ind_riga_dato].num_dati++;
                        	}
			if(temp[OFF_END_DATO3]=='*' &&
                           strlen(temp) >= (OFF_END_DATO3+1))
                        	{
			/* estrae il valore */
				strncpy(array_dati[ind_riga_dato].dato[2].val,
					&temp[OFF_INI_DATO3],10);
				strncpy(array_dati[ind_riga_dato].dato[2].descr,
                                         &temp[OFF_INI_DESC3],8);
				array_dati[ind_riga_dato].num_dati++;
                        	}
/* 7-4-95 Micheletti */
if( tot_righe_dati )
{
			if(array_dati[ind_riga_dato].num_dati)
				ind_riga_dato++;
}
			}
		}
	}
}

/*
 scrittura del file f14.dat
*/
write_file_f14(fp)
FILE *fp;
{
char app[12];
int i,j,k;
float val_appo; /* valore di appoggio per conversione in unita' di misura standard */
int ind_riga_dato,ind_descr;
int num_descr;
int ind_umis; /* indice dell'unita' di misura tra quelle disponibili */
int sel_umis; /* indice della unita' scelta */
int counter;  /* indice riga */

#ifdef DEBUG
printf("\n RIGA DI INTESTAZIONE SCRITTA = %s",riga_intesta);
printf("\n NUMERO BLOCCHI LETTI = %d",nbl);
#endif
fprintf(fp,"%-132.132s",riga_intesta);
sprintf(&riga_norm[0][28],
"%2.2s=%-6.6s*%2.2s=%-6.6s*%2.2s=%-6.6s*%2.2s=%-6.6s*%2.2s=%-6.6s*"
     ,descr_norm[0],dati_norm[0],
      descr_norm[1],dati_norm[1],
      descr_norm[2],dati_norm[2],
      descr_norm[3],dati_norm[3],
      descr_norm[4],dati_norm[4]
      );
sprintf(&riga_norm[1][28],
"%2.2s=%-6.6s*%2.2s=%-6.6s*%3.3s=%-6.6s*",
      descr_norm[5],dati_norm[5],
      descr_norm[6],dati_norm[6],
      descr_norm[7],dati_norm[7]);
fprintf(fp,"\n%-132.132s",riga_norm[0]);
fprintf(fp,"\n%-132.132s",riga_norm[1]);

fprintf(fp,"\n%-132.132s",HEADER_VAR_OUT);
for(i=0,counter=1;i<neqsis;i++,counter++)
        {
           /* reset counter ogni 100 */
        if( counter && !(counter % 100) )
           counter -= 100;
/*
 Effettua la conversione tra l'unita' di misura correntemente selezionata
 e l'unita' di misura M.K.S.
*/
        ind_umis=valout[i].ind_umis;
	sel_umis=valout[i].sel_umis;
/*
        valout[i].val=(valout[i].val-uni_mis[ind_umis].B[sel_umis])
                  /uni_mis[ind_umis].A[sel_umis];
        fprintf(fp,"\n%3d %.8s =",i+1,nom_sivar[i]);
        spr_float(app,valout[i].val);
*/
        val_appo=(valout[i].val-uni_mis[ind_umis].B[sel_umis])
                  /uni_mis[ind_umis].A[sel_umis];
        fprintf(fp,"\n%3d %.8s =",counter,nom_sivar[i]);
        spr_float(app,val_appo);

        fprintf(fp,"%s*",app);
        fprintf(fp,"%.13s=",&nom_sivar[i][16]);
        if(valout[i].noto)
                fprintf(fp,VAR_NOTA);
        else
                fprintf(fp,"    ");
        fprintf(fp,"* %.70s",&nom_sivar[i][30]);
        fprintf(fp,"                 ");
        }
fprintf(fp,"\n%-132.132s",HEADER_VAR_INP);
for(i=0,counter=1;i<nu;i++,counter++)
         {
           /* reset counter ogni 100 */
         if( counter && !(counter % 100) )
           counter -= 100;
/*
 Effettua la conversione tra l'unita' di misura correntemente selezionata
 e l'unita' di misura M.K.S.
*/
        ind_umis=valinp[i].ind_umis;
	sel_umis=valinp[i].sel_umis;
/*
        valinp[i].val=(valinp[i].val-uni_mis[ind_umis].B[sel_umis])
                  /uni_mis[ind_umis].A[sel_umis];
        fprintf(fp,"\n%3d %.8s =",i+1,nom_vari[i]);
        spr_float(app,valinp[i].val);
*/
        val_appo=(valinp[i].val-uni_mis[ind_umis].B[sel_umis])
                  /uni_mis[ind_umis].A[sel_umis];
        fprintf(fp,"\n%3d %.8s =",counter,nom_vari[i]);
        spr_float(app,val_appo);

        fprintf(fp,"%s*",app);
        fprintf(fp,"%.13s=",&nom_vari[i][16]);
        if(valinp[i].noto)
                fprintf(fp,VAR_NOTA);
        else
                fprintf(fp,"    ");
/* (Micheletti) 
        fprintf(fp," %.70s",&nom_vari[i][30]);
*/
        fprintf(fp,"* %.70s",&nom_vari[i][30]);
        fprintf(fp,"                 ");
        }


fprintf(fp,"\n%-132.132s",HEADER_SEZ_DATI);
for(i=0;i<nbl;i++)
        {
        ind_descr=header_dati[i].inizio_descr;
        ind_riga_dato=header_dati[i].inizio;
        num_descr=header_dati[i].num_descr;
        fprintf(fp,"\n%-132.132s",header_dati[i].titolo);
/*
 gestione del caso in cui non vi sono dati ma solo descrizioni
*/
/****
	if(header_dati[i].num_righe_dati == 0 && num_descr)
		{
		for(j=0; j<num_descr; j++)
			{	
                        fprintf(fp,"\n%s",
                                       array_descr[ind_descr].descr);
                        ind_descr++;
			}
		}
	else
****/
/*
 caso normale
*/
		{


	        for(j=0;j<header_dati[i].num_righe_dati; j++)
                {

                while(num_descr && array_descr[ind_descr].dato_seguente==ind_riga_dato &&
                       ind_descr<tot_descr)
                                {
                                fprintf(fp,"\n%s",
                                       array_descr[ind_descr].descr);
                                ind_descr++;
                                }
                fprintf(fp,"\n");
                for(k=0;k<array_dati[ind_riga_dato].num_dati;k++)
                        {
                        fprintf(fp,"    %s =%.10s*",
                                      array_dati[ind_riga_dato].dato[k].descr,
                                      array_dati[ind_riga_dato].dato[k].val);
                        }
                if(array_dati[ind_riga_dato].num_dati)
                        ind_riga_dato++;
                }
/*
 potrebbero esserci ancora delle descrizioni da riportare
*/
        	while(ind_descr < num_descr)
                	{
                        fprintf(fp,"\n%s",
                                       array_descr[ind_descr].descr);
                        ind_descr++;
                	}
		}
        }
fprintf(fp,"\n");
fprintf(fp,EOF_F14);
fprintf(fp,"\n");
}

spr_float(string,val)
char *string;
float val;
{
if(val<9999.99999 && val>0.1)
        sprintf(string,"%-10.5f",val);
else if (val >0)
        sprintf(string,"%-10.4E",val);
else  /* valore negativo -> deve lasciare spazio per il segno */
	sprintf(string,"%-10.3E",val);
}

elim_newline(str)
char *str;
{
char *pnewline;
pnewline=strchr(str,'\n');
if(pnewline) *pnewline='\0';
}

set_defaults_norm()
{
int i;
static char *str_defaults[]={ DATI_NORM_P0,
			      DATI_NORM_H0,
			      DATI_NORM_W0,
			      DATI_NORM_T0,
			      DATI_NORM_R0,
			      DATI_NORM_L0,
			      DATI_NORM_V0,
			      DATI_NORM_DP0 };
for(i=0;i<8;i++)
	{
	if(strcmp(dati_norm[i],DATI_NORM_BLANK)==0)
		{
		strcpy(dati_norm[i],str_defaults[i]);
		}
	}
}
		
	
