/**********************************************************************
*
*       C Source:               compstaz.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Mar 31 11:33:49 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: compstaz.c-2 %  (%full_filespec: compstaz.c-2:csrc:1 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)compstaz.c	1.4\t3/23/95";
/*
   modulo compstaz.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)compstaz.c	1.4
*/
/**********************************************************
	programma compstaz
Compila il file descrittore delle stazioni r01.dat e produce
	il file r02.dat
*********************************************************/

#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>

#include "sim_param.h"
#include "sim_types.h"
#include "xstaz.h"
#include "compstaz.inc"
/*
	Include la tabella di definizione delle nuove stazioni 
*/

#include "newstaz.h"
#include <sqlite3.h>

int read_staz2(int);
typedef struct s_tipostaz {
	char s[4]; 
	} TIPOSTAZ;

// void legge_riga( char *riga, int *lun, int *nriga );
// void separa_str( char *riga, int lun, int nstr, STRIN_ST strin[]);
int fill_pagina(S_COMP_PAGINA *);
int check_pagina(S_COMP_PAGINA *);


FILE *fp_s01,*fperr;
STRIN_ST string[10];

S_PAGINE   pag;
S_STAZIONI staz;
char riga [80];
int nriga;


int	numero_pag=0;			/* indice ricorsivo pagine */
int	numero_staz=0;			/* indice ricorsivo stazioni */

int	svincolo_indici_pag[MAX_PAG];	/* svincolo tra indice pagina vero
					calcolato progessivamente e queelo
					definito nel file r01.dat */
int	offset_staz[MAX_STAZ];
/*
        Variabili per l'area shared delle variabili */

char *ind_sh_top;
int id_sh;
VARIABILI *variabili;
int tot_variabili;
int tot_modelli;

FILE *fo;

sqlite3 *db;

main()
{
char aux[80];
char file [50];

int  i, j,  i1, i2, ncoll, lun, ier;
FILE *fp;
S_COMP_PAGINA *scomp;
HEAD_R02 header;


/* apre file di output compstaz.log */

fo = fopen("compstaz.log","w");

/* lettura file s02.dat */

/*    reads02(&nmod,model); */


/* si alloca l' area shared delle variabili */


        costruisci_var(&ind_sh_top,&variabili,&id_sh);
        tot_modelli=numero_modelli(ind_sh_top);
        tot_variabili=numero_variabili(ind_sh_top);
	for (i=0; i<tot_variabili; i++) 
		fprintf(fo," \n indice %d  variab %s  tipo %d  addr  %d %d",i,variabili[i].nome,variabili[i].tipo,variabili[i].addr,variabili[i].mod);


/* inizializza buffer di svicolo indici pagine */

for (i=0; i< MAX_PAG; i++) svincolo_indici_pag[i]= -1;

/*	apre ro1 */

if (!(fp_s01 = fopen( "r01.dat", "r")))
{
 distruggi_var(id_sh);
 exit( puts( "ATTENZIONE non esiste il file R01.DAT"));
}
 nriga=0;

/*	lettura prima riga di r01.DAT */

for (;;)
{
	legge_riga( riga, &lun, &nriga);
	if (lun != 4 || strncmp( riga, "****", 4))
	{
		printf(" ATTENZIONE: la riga seguente non e' del tipo **** \n %s");
		fprintf(fo," ATTENZIONE: la riga seguente non e' del tipo **** \n %s");
 		distruggi_var(id_sh);
		exit (puts("\n Il programma COMPSTAZ termina per errore"));
	}
			
/* legge il codice */

	legge_riga( riga, &lun, &nriga);
	if (lun == 6 && !strncmp( riga, "PAGINA", 6)) read_pagina();
	else
	{
/* non esegue il test sulla lunghezza perche' puo' essere inserito un campo
   descrittivo della stazione  */

		if (!strncmp( riga, "STAZIONE", 8))	read_staz2(tot_modelli);
		else
		{
			 if (lun != 11 || strncmp( riga, "END_OF_FILE", 11))
			 {
	 	printf("\n ATTENZIONE: la riga %d non e' del tipo ",nriga);
		printf("\n PAGINA o STAZIONE o END_OF_FILE \n %s",riga);
	 	fprintf(fo,"\n ATTENZIONE: la riga %d non e' del tipo ",nriga);
		fprintf(fo,"\n PAGINA o STAZIONE o END_OF_FILE \n %s",riga);
 			distruggi_var(id_sh);
			exit (puts("\nIl programma COMPSTAZ termina per errore"));
			 }
			 else
			 {
      		 	/*  si alloca la struttura dati necessaria per 
                           completare la definizione  delle pagine */

	         		scomp=(S_COMP_PAGINA *) calloc (numero_pag, sizeof (S_COMP_PAGINA));
 				if (scomp == NULL)
				{
					printf("malloc fallita \n");
                	                 distruggi_var(id_sh);
                        	       exit (1);
	                        }
				fill_pagina(scomp);
				check_pagina(scomp);

				/* scrive il file r02.dat */

				fp=fopen("r02.dat","w");
				memset(header.data,0,sizeof (HEAD_R02));
				header.tot_pagine=numero_pag;
				header.tot_staz=numero_staz;
				fwrite(header.data,sizeof (HEAD_R02), 1, fp);
				
				/* scrive la struttura S_PAGINA per tutte le
				pagine definite */

				lun=sizeof (S_PAGINA);
				for (i=0; i< numero_pag; i++)
                                {
/*		printf (" pag. %d  offset %d  num.staz %d\n",i,pag.p[i].offset_staz,pag.p[i].num_staz);*/
				
				fwrite(&pag.p[i].attiva,sizeof (S_PAGINA), 1, fp);
				}

				/* scrive l'array degli indici delle
				stazioni di ogni pagina */

				lun=sizeof (int) * numero_staz;
				fwrite(offset_staz, lun , 1, fp);
				
				/* scrive la struttura stazione per tutte
				quelle definite */

				lun=sizeof (S_STAZIONE);
				for (i=0; i< numero_staz; i++)
				fwrite((char *) staz.s[i].nome,sizeof (S_STAZIONE), 1, fp);
				fclose(fp);
				fclose(fp_s01);
				distruggi_var(id_sh);
			 	exit(puts("\nFine corretta COMPSTAZ"));
			 }
		}
	}
}	/* ciclo lettura file */
}

/**************************
	gestione pagine
***************************/
int read_pagina()
{
int i, lun, nstr, ipag, numbyte;
char aux[80],*px;

/* legge il numero della pagina */

  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,"NUMERO",6)) 
  {
	strcpy(aux,"la riga seguente non  e' del tipo: NUMERO nnn \n");
  		goto errore;
  }
numbyte=sscanf(string[1].stringa,"%3d",&ipag);
  if (!sscanf(string[1].stringa,"%3d",&ipag)) goto errore;
  if (numero_pag > MAX_PAG) 
  {
  		strcpy(aux,"il numero di pagine eccede il massimo ammesso \n");
		goto errore;
	}
	ipag--;
        if (svincolo_indici_pag[ipag] != -1)
   	{
  		strcpy(aux,"numero pagina gia' definito \n");
		goto errore;
	}
	pag.p[numero_pag].attiva=1;
        svincolo_indici_pag[ipag]=numero_pag;

/* cerca il nome della pagina */
	  
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,"NOME",4) || (string[1].stringa == NULL )) 
  {
	 strcpy(aux,"la riga seguente non e' del tipo: NOME aaaaaaaa");
  		goto errore;
	}
  strcpy(pag.p[numero_pag].nome,string[1].stringa);

/* cerca la descrizione della pagina  */
	  
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=10, string);
  if (strncmp(string[0].stringa,"DESCRIZIONE",11) || (string[1].stringa == NULL ))
  {
	 strcpy(aux,"la riga seguente non e' del tipo: DESCRIZIONE aaaa....");
  	 goto errore;
	}
  memset(pag.p[numero_pag].descrizione,' ',LUN_DES_PAG-1);
  i=1;
  numbyte=0;
  px=pag.p[numero_pag].descrizione;
  while (string[i].lun_stringa && (numbyte+string[i].lun_stringa+1) < LUN_DES_PAG)
  {
   strncpy(px,string[i].stringa,string[i].lun_stringa);
	px += string[i].lun_stringa + 1 ;  /* lascia un blank */
	numbyte+= string[i].lun_stringa +1;
	i++;
	}
  numero_pag++;
  return(0);

errore:
 fprintf(fo,"\n ATTENZIONE:  %s",aux);
 fprintf(fo,"\n ATTENZIONE:  %s",aux);
 printf("\n riga : %d - %s \t",nriga,riga);
 printf("\n riga : %d - %s \t",nriga,riga);
 distruggi_var(id_sh);
 exit (puts("\n Il programma COMPSTAZ termina per errore"));
 }
	

/**************************
	gestione stazioni
***************************/
int read_staz2(nmod)
int nmod;   /*numero modelli */
{
int i, lun, nstr, istaz, itipo;
char aux[80];
char nomestaz[LUN_NOME_STAZ+1];

 fprintf(fo,"\n riga : %d %s \n",nriga,riga);

/*
	separa il nome della stazione definito in riga e lo salva
*/
  memset(nomestaz,0,sizeof (nomestaz));
  separa_str( riga, lun, nstr=2, string);
  if (string[1].lun_stringa) strcpy(nomestaz,string[1].stringa);
  fprintf(fo," nome staz %s \n",nomestaz);

/* legge il numero della stazione */

  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,"NUMERO",6)) 
  {
	  	strcpy(aux,"la riga seguente non e' del tipo: NUMERO nnn");
  		goto errore;
  }
  
/*   nella nuova versione istaz  non e' dato dal campo NUMERO ma dal
	numero pregressivo di stazioni definite in r01.dat */

  istaz= numero_staz;
  if (istaz == MAX_STAZ) 
  {
  		strcpy(aux,"il numero della stazione eccede il massimo ");
		goto errore;
  }
  numero_staz++;
   if (staz.s[istaz].pagina) 
   {
  		strcpy(aux,"la stazione e' definita piu' volte ");
		goto errore;
   }
   fprintf(fo,"\n numero: %d",istaz+1);
   printf("\nCompilazione stazione numero : %d",istaz+1);
/* legge il tipo di stazione e verifica se compreso nella lista */

   memset(staz.s[istaz].nome,0,sizeof (S_STAZIONE));
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
fprintf(fo," \nstazione  tipo %s  ",string[1].stringa);
  if (strncmp(string[0].stringa,"TIPO",4) || (string[1].stringa == NULL )) 
  {
  	strcpy(aux,"la riga seguente non e' del tipo : TIPO aaa "); 
	goto errore;
	}

	itipo=0;
	while (tipi_old_staz[itipo] != NULL)
	{
            if (!strncmp(tipi_old_staz[itipo],string[1].stringa,3))
	    {
		itipo++;			/* parte da 1*/
		staz.s[istaz].tipo=itipo;
		staz_gen_c(istaz,itipo,nmod);
		goto FINE_READ_STAZ;
	    }
	itipo++;
	}
/*
	verifica se la stazione e' del nuovo tipo 
*/
        for (itipo=0; itipo< NUM_NEW_STAZ; itipo++)
        {
          if (!strncmp(new_staz[itipo].nomestaz,string[1].stringa,LUN_NOME_STAZ)) 
	  {
	        staz.s[istaz].tipo=itipo+ MINIMO_INDICE_NEW_STAZ;
		compila_new_staz(istaz,itipo,nmod);
		goto FINE_READ_STAZ;
	  }
        }

  	strcpy(aux,"tipo stazione non esistente ");
errore:
 printf("\n ATTENZIONE: %s",aux);
 printf("\nriga : %d - %s ",nriga,riga);
 fprintf(fo,"\n ATTENZIONE: %s",aux);
 fprintf(fo,"\nriga : %d - %s ",nriga,riga);
 distruggi_var(id_sh);
 exit (puts("\nIl programma COMPSTAZ termina per errore"));


FINE_READ_STAZ:

if (strlen(nomestaz))
{
	strcpy(staz.s[istaz].nome,nomestaz);
	if (cerca_staz(nomestaz,istaz)) 
	{
  sprintf(aux," Stazione nome %s non congruente con le precedenti definizioni ",nomestaz);
        riga[0]=0;
	  goto errore;
	}
}
return(0);
}	


/***********************************************************************
	fill_pagina()
	Completa la descrione delle pagine
***********************************************************************/

fill_pagina(scomp)
S_COMP_PAGINA *scomp;
{
int i, j, ipag, istaz;

	for (i=0; i< numero_staz; i++)
	{
		if (staz.s[i].pagina)
		{
			if (svincolo_indici_pag[staz.s[i].pagina -1] == -1 )
			{
			printf("\n ATTENZIONE: la pagina %d evocata dalla stazione %d",staz.s[i].pagina,i+1);
			printf("\n non  e' stata definita ");
			fprintf(fo,"\n ATTENZIONE: la pagina %d evocata dalla stazione %d",staz.s[i].pagina,i+1);
			fprintf(fo,"\n non  e' stata definita ");
			distruggi_var(id_sh);
			exit (puts("\nIl programma COMPSTAZ termina per errore"));
			}
                        staz.s[i].pagina=svincolo_indici_pag[staz.s[i].pagina -1];
			ipag = staz.s[i].pagina ;
			if (pag.p[ipag].num_staz >= MAX_OGG ) 
                        {
printf(" indice stazione %d \n",i);
			printf("\n ATTENZIONE: nella pagina %d non possono essere",ipag+1);
			printf("\n inserite piu' %d stazioni",pag.p[ipag].num_staz );
			fprintf(fo,"\n ATTENZIONE: nella pagina %d non possono essere",ipag+1);
			fprintf(fo,"\n inserite piu' %d stazioni",pag.p[ipag].num_staz );
			distruggi_var(id_sh);
			exit (puts("\nIl programma COMPSTAZ termina per errore"));
			}	
			else
			{
                        istaz=pag.p[ipag].num_staz;
			scomp[ipag].posix0[istaz]=staz.s[i].posix0;
			scomp[ipag].posiy0[istaz]=staz.s[i].posiy0;
			scomp[ipag].posix1[istaz]=staz.s[i].posix1;
			scomp[ipag].posiy1[istaz]=staz.s[i].posiy1;
			scomp[ipag].staz[istaz]=i+1;
			pag.p[ipag].num_staz++;
			}
		}
	}

/*
	compila l' arrey degli indici stazioni relativi alle pagine
*/

pag.p[0].offset_staz=0;
for (i=0; i< numero_pag; i++)
{
        if (i>0) pag.p[i].offset_staz= pag.p[i-1].offset_staz + pag.p[i-1].num_staz;
	for (j=0; j<  pag.p[i].num_staz; j++)
	offset_staz[pag.p[i].offset_staz+j]=scomp[i].staz[j];
}
	

	

return(0);
}


/*******************************************************************
	check_pagina()
********************************************************************/

check_pagina(scomp)
S_COMP_PAGINA *scomp;
{
int ipag, istaz,  j, m, k, ipx0, ipy0, ipx1, ipy1;

	for (ipag=0; ipag < numero_pag; ipag++)
	{

		/* calcola le coordinate di origine della window */

		ipx0=MAX_CEL;
		ipy0=MAX_CEL;
		for (j=0; j< pag.p[ipag].num_staz; j++)
		{
			if (scomp[ipag].posix0[j] < ipx0) ipx0=scomp[ipag].posix0[j];
			if (scomp[ipag].posiy0[j] < ipy0) ipy0=scomp[ipag].posiy0[j];
		}

		/* ridefinisce le posizioni x0 y0 x1 y1 di ogni stazione della
			pagina in funzione di ipx0, ipy0 */

		for (j=0; j< pag.p[ipag].num_staz; j++)
		{
				istaz=scomp[ipag].staz[j]-1;
				scomp[ipag].posix0[j] -=ipx0;
				scomp[ipag].posiy0[j] -=ipy0;
				scomp[ipag].posix1[j] -=ipx0;
				scomp[ipag].posiy1[j] -=ipy0;
				staz.s[istaz].posix0 -=ipx0;
				staz.s[istaz].posiy0 -=ipy0;
				staz.s[istaz].posix1 -=ipx0;
				staz.s[istaz].posiy1 -=ipy0;
		}

		 /* test per evitare overlap */

		for (j=0; j< pag.p[ipag].num_staz; j++)
		{
			ipx0 =scomp[ipag].posix0[j];
			ipy0 =scomp[ipag].posiy0[j];
			ipx1 =scomp[ipag].posix1[j];
			ipy1 =scomp[ipag].posiy1[j];
			for (k=ipx0; k<ipx1; k++)
			{
				for (m=ipy0; m<ipy1; m++)
				{
					if (scomp[ipag].fill_pag[k][m])
					{
						printf("\n ATTENZIONE : sovrapposizione di stazioni ");
						printf("\n nella pagina %d ",ipag+1);
						printf("\n nella staz %d ",scomp[ipag].staz[j]);
						fprintf(fo,"\n ATTENZIONE : sovrapposizione di stazioni ");
						fprintf(fo,"\n nella pagina %d ",ipag+1);
						fprintf(fo,"\n nella staz %d ",scomp[ipag].staz[j]);
						distruggi_var(id_sh);
						exit (puts("\nIl programma COMPSTAZ termina per errore"));
					}
				}
			}
			
			/* riempio la matrice con il numero della stazione */

			for (m=ipy0; m<ipy1; m++)
			{
				for (k=ipx0; k<ipx1; k++)
				{
					scomp[ipag].fill_pag[k][m] =scomp[ipag].staz[j];
				}
			}
		}

		/* calcola le dimensioni della windows */

		pag.p[ipag].posmx=0;
		pag.p[ipag].posmy=0;
		for (j=0; j< pag.p[ipag].num_staz; j++)
		{
			if (pag.p[ipag].posmx < scomp[ipag].posix1[j]) pag.p[ipag].posmx = scomp[ipag].posix1[j];
			if (pag.p[ipag].posmy < scomp[ipag].posiy1[j]) pag.p[ipag].posmy = scomp[ipag].posiy1[j];
		}
	}
	return(0);
}

/*
	cerca_staz: verifica che le stazioni aventi lo stesso nome
	abbiano anche gli stessi parametri
*/

int cerca_staz(nomestaz,istaz)
char *nomestaz;		/* nome stazione */
int istaz;		/* indice stazione */
{
int i, k, lun;
char *p1,*p2;

for (i=0; i< numero_staz; i++)
{
	if (i == istaz ) continue;
	if (!strcmp(staz.s[i].nome,nomestaz))
	{
	/* dell' header della stazione controlla solo il tipo e poi
	   tutti gli altri campi */
	
	if (staz.s[i].tipo != staz.s[istaz].tipo)  return(1);
        p1 = &staz.s[i].nome[0] + sizeof (HEADER_STAZ);
        p2 = &staz.s[istaz].nome[0] + sizeof (HEADER_STAZ);
/*
        lun = sizeof ( S_STAZIONE) - sizeof (HEADER_STAZ);
	for (k=0; k< lun; k++, p1++, p2++)
        printf( "num  %d  byte: %d  %x   %x  \n",lun,k,*p1,*p2);
        p1 = &staz.s[i].nome[0] + sizeof (HEADER_STAZ);
        p2 = &staz.s[istaz].nome[0] + sizeof (HEADER_STAZ);
*/
	if (memcmp(p1,p2,sizeof ( S_STAZIONE) - sizeof (HEADER_STAZ)))
							 return(1);
	}
}
return(0);
}
