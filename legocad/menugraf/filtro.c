/*
   modulo filtro.c
   tipo 
   release 1.6
   data 3/28/95
   reserved @(#)filtro.c	1.6
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)filtro.c	1.6\t3/28/95";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <math.h>
#include <Xm/Xm.h>
#include <grsf22.h>
#include "menu_graf.h"

/***************************************************************************/
/*                           FILTRO DIGITALE                               */
/***************************************************************************/
typedef double DTYPE;

 /* dati relativi al filtro Bessel o Butterworth del 4� ordine             */
typedef struct fildat
	{
	DTYPE alfa;
	DTYPE delta0;
	DTYPE delta1;
	DTYPE delta2;
	DTYPE delta3;
	DTYPE delta4;
	}FILDAT;

FILDAT tab;

DTYPE yvect[4],yfvect[4];

extern void rew_dati(void);

DTYPE power(base,esponente)
DTYPE base,esponente;
{
int i;
DTYPE risult;
risult=base;
for (i=1;i<esponente;i++)
risult=risult*base;
return risult;
}
   /*****************************/
   /*  inizializzo il FILTRO    */
   /*****************************/
filt_init(fcut,tsam,fi_type,val_init)
DTYPE fcut,tsam;
int fi_type;
DTYPE val_init;
{
DTYPE aux1,aux2,beta;          /* variabili ausiliarie */
DTYPE Fomega1,Fomega2,Fcsi1,Fcsi2,Fbeta0,Fbeta1,Fbeta2,Fbeta3;
                                   /* per l'inizializzazione del filtro */

yvect [0] = yfvect[0] = val_init;
yvect [1] = yfvect[1] = val_init;
yvect [2] = yfvect[2] = val_init;
yvect [3] = yfvect[3] = val_init;

#ifndef GIUSEPPE_GARIBALDI
/* versione non funzionante commentata
switch (fi_type)
	{
	case BESSEL:*/ /* filtro BESSEL 4� ordine */ /*
		Fomega1=(1.59*fcut);
		Fomega2=(1.42*fcut);
		Fcsi1=0.62;
		Fcsi2=0.96;
	break;
	case BUTTERWORTH:*/ /* filtro BUTTERWORTH 4� ordine */ /*
		Fomega1=fcut;
		Fomega2=fcut;
		Fcsi1=0.38;
		Fcsi2=0.92;
	break;
	}
beta=2.0/tsam;*/ /* discretizzazione Tustin senza prewarping */ /*

tab.alfa=(pow(Fomega1,2)*pow(Fomega2,2));
Fbeta0=tab.alfa;
aux1=(2*Fcsi1*Fomega1);
aux2=(2*Fcsi2*Fomega2);
Fbeta1=(aux1*pow(Fomega2,2)+aux2*pow(Fomega1,2));
Fbeta2=(pow(Fomega1,2)+pow(Fomega2,2)+4*Fcsi1*Fcsi2*Fomega1*Fomega2);
Fbeta3=(aux1+aux2);

tab.delta0=(pow(beta,4)-Fbeta3*pow(beta,3)+Fbeta2*pow(beta,2)-
		Fbeta1*beta+Fbeta0);
tab.delta1=(-4*pow(beta,4)+2*Fbeta3*pow(beta,3)-2*beta*Fbeta1+
		4*Fbeta0);
tab.delta2=(6*pow(beta,4)-2*Fbeta2*pow(beta,2)+6*Fbeta0);
tab.delta3=(-4*pow(beta,4)-2*Fbeta3*pow(beta,3)+2*beta*Fbeta1+
		4*Fbeta0);
tab.delta4=(pow(beta,4)+Fbeta3*pow(beta,3)+Fbeta2*pow(beta,2)+
		Fbeta1*beta+Fbeta0);

if (tab.delta4==0.0)             */ /* controllo sul delta 4 del filtro */ /*
	{ 
	printf("INIT_Error: tab.delta4=0");
	return (ERRORE);
	}
 		fine versione non funzionante commentata */
#endif /* GIUSEPPE_GARIBALDI */

	/* per il momento non si fa distinzione tra i due tipi 
	di filtro comunque l'implementazione riguarda un filtro 
	Butterworth del 4 ordine */

        beta = sin(M_PI/4-M_PI*fcut*tsam)/sin(M_PI/4+M_PI*fcut*tsam);
        tab.alfa=47*(power(beta,4)-4*power(beta,3)+6*power(beta,2)-4*beta+1);
        tab.delta0=(500*power(beta,4)+243*power(beta,2)+9);
        tab.delta1=-2*beta*(1243*power(beta,2)+261);
        tab.delta2=3*(81*power(beta,4)+1342*power(beta,2)+81);
        tab.delta3=-2*beta*(261*power(beta,2)+1243);
/*        tab.delta4=(9*power(beta,4)+243*power(beta,2)+500); */
	tab.delta4=16*tab.alfa-tab.delta3-tab.delta2 -tab.delta1-tab.delta0;
}

/***************************************************************************/
/*                            routine FILTER                               */
/* riceve in ingresso il segnale y  restituisce il corrispondente          */
/* segnali filtrati in *yfpoint;                                           */
/* l'utente pu� scegliere tra un filtro del 4� ordine di Bessel e uno di   */
/* Butterworth (sempre del 4� ordine); il filtro viene inizializzato nella */
/* routine di initcycle e i dati conservati nella tabella filtab, passata  */
/* di conseguenza alla funzione di filtraggio.                              */
/***************************************************************************/

 /*****************************/
 /*  sottofunzioni di FILTER  */
 /*****************************/

DTYPE filsign(sign,signf,new,fildata)         /* esegue il filtraggio del */
DTYPE sign[4],signf[4];
DTYPE new;
FILDAT fildata;
{
DTYPE aux1,aux2,out;

aux1 = fildata.delta3*signf[0]+
	fildata.delta2*signf[1]+
	fildata.delta1*signf[2]+
	fildata.delta0*signf[3] ;
aux2 = fildata.alfa*(new+4*sign[0]+6*sign[1]+4*sign[2]+sign[3]) ;

out=((aux2-aux1)/fildata.delta4);

return (out);
}

shiftvect(vect,x)  /* shift verso dx di vect e aggiunta in testa di x */
DTYPE vect[4],x;
{
int j;

for (j=3;j>0;j--)
	vect[j]=vect[j-1];
vect[0]=x;
}

 /*****************************/
 /*      corpo di FILTER      */
 /*****************************/

DTYPE filter(y,tabella)
DTYPE y;
FILDAT tabella;

{

DTYPE yfpoint;

yfpoint=filsign(yvect,yfvect,y,tabella);
shiftvect(yvect,y);
shiftvect(yfvect,yfpoint);
printf ("Valore filtrato: %f Valore y: %f\n",yfpoint,y);
return (yfpoint);
}
/**************************  FINE ROUTINE FILTER  **************************/


EseguiFiltro (Indice,ftipo,NVar,NDescr,ftaglio)
int Indice,ftipo;
char *NVar,*NDescr;
float ftaglio;
{
extern Widget Message;
extern float delta_nuovo;
extern int numero_var;
extern int numero_campioni;
extern char nomi_var[NUM_VAR+1][LUN_NOME+1];
extern char **simboli;
extern S_HEAD1 header1;
extern S_HEAD2 header2;
extern FILE *fpAPPO,*fpDAT;

int j,i,uguali,k,lung;
char Mess[100],*Destinazione,**simb_appo;
XmString Messaggio;
float tempo,valore,ValSav,NuovoV;
Boolean prima;

Destinazione = (char *)XtCalloc (LUN_SIMB+1,sizeof(char));

/*  Aggiungo alla struttura nomi_var il nuovo nome della variabile
	e compongo la stringa Destinazione con il nome della 
	nuova variabile e la sua descrizione  */
strcpy (nomi_var[numero_var],NVar);
for (i=(strlen(NVar));i<LUN_NOME+1;i++)
	NVar [i] = ' ';
strcpy (Destinazione,NVar);
lung = strlen (NDescr);
k = 0;
for (i=LUN_NOME;i<(LUN_NOME+lung+1);i++)
	{
	Destinazione[i] = NDescr[k];
	k++;
	}

/*  Alloco una struttura di appoggio dove aggiungo in coda
	la nuova variabile  */
simb_appo = AllocaStruttura (numero_var+2,LUN_SIMB+1);
for (i=0;i<numero_var;i++)
	strcpy (simb_appo[i],simboli[i]);
strcpy (simb_appo[i],Destinazione);
d2free (simboli);
XtFree (Destinazione);

/*  Riscrivo la struttura simboli con la nuova variabile  */
simboli = AllocaStruttura (numero_var+1,LUN_SIMB+1);
for (i=0;i<numero_var+1;i++)
	strcpy (simboli[i],simb_appo[i]);

header1.nvar++;
header2.ncasi++;
Apri_f22_temp ();
rew_dati ();
Scrivi_header1();
Scrivi_header2();
prima = True;
for (i=0;i<numero_campioni+1;i++)
	{
	ValSav = 0;
	fread (&tempo,1,sizeof(float),fpDAT);
	fwrite (&tempo,sizeof(float),1,fpAPPO);
	for (j=0;j<numero_var;j++)
		{
		fread (&valore,1,sizeof(float),fpDAT);
		fwrite (&valore,sizeof(float),1,fpAPPO);
		if (j==Indice) 
			{
			ValSav = valore;
			if (prima)
			   {
			      /* Inizializzo il filtro con il primo valore */
			      filt_init ((DTYPE)ftaglio,(DTYPE)delta_nuovo,
					ftipo,(DTYPE)valore);
			      prima = False;
			   }
			}
		}
	NuovoV = (float)filter ((DTYPE)ValSav,tab);
	fwrite (&NuovoV,sizeof(float),1,fpAPPO);
	}
Uscita ();
return (GIUSTO);
}

char *EseguiControlliInput (But1,But2,Freq,Nomevar)
Boolean But1,But2;
char *Freq,*Nomevar;
{
extern int numero_var;
extern int campioni;
extern char nomi_var[NUM_VAR+1][LUN_NOME+1];

char *stringa;
int errore,i,uguali;

stringa = (char *)XtCalloc (80, sizeof(char) );
errore = 0;
if ( (!But1) && (!But2) )
	errore = 1;
if ( !NumFloVerifica(Freq) )
	errore = 2;
i=0;
uguali =1;
while ( (i<numero_var) && (uguali!=0) )
        {
        uguali = strcmp (nomi_var[i],Nomevar);
        i++;
        }
if (uguali == 0) errore = 3;
if ( (numero_var + 1) > NUM_VAR)
	errore = 4;
if (campioni == CAMP_DISUGUALI)
	errore = 5;

switch (errore)
	{
	case 1:
		strcpy (stringa,"Must be selected a filter type!\0");
	break;
	case 2:
		strcpy (stringa,"Cut frequency value not valid!\0");
	break;
	case 3:
		strcpy (stringa,"New variable name already exist!\0");
	break;
	case 4:
		strcpy (stringa,"Max variable number superated!\0");
	break;
	case 5:
		strcpy (stringa,"Delta must be the same!\0");
	break;
	default:
		stringa = NULL;
	break;
	}
return (stringa);
}
