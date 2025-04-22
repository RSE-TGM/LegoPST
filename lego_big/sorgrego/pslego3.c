/*								*/
/* pslego3.c							*/
/*								*/
/* v.0.b.2 (per macchine Alpha DEC)				*/
/*								*/
/* creato:	21 luglio 1995					*/
/* modificato:	 3 agosto 1995					*/
/*								*/
/* autore:	Marco Bonasoro (CISE)				*/
/*								*/
/*								*/
/* Questa applicazione acquisisce un file F01 e il relativo	*/
/* F14 creati da REGOMMI; produce un file F04 standard.		*/
/*								*/
/*
   modulo pslego3.c
   tipo 
   release 1.6
   data 2/20/96
   reserved @(#)pslego3.c	1.6
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)pslego3.c	1.6\t2/20/96";
/*
        Fine sezione per SCCS
*/
/* inclusione dei file header					*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* definizione delle dimensioni delle varie strutture           */

#define MULT            1000            /* moltiplicatore                                       */
#define NMODULI         10*MULT         /* numero di moduli                                     */
#define NBLOCCHI        25*MULT         /* numero di blocchi                                    */
#define NSTATI_ALG      50*MULT         /* ordine max sistema algebrico                         */
#define NINGRESSI       50*MULT         /* numero di ingressi                                   */
#define NVARIABILI      100*MULT        /* numero di variabili                                  */
#define NREGBLOCVAR     NSTATI_ALG+NINGRESSI    /* num. di var. di un blocco (regolazione)      */
#define NDATI           500*MULT        /* numero di dati                                       */
#define NTOTINGRESSI    NVARIABILI-NSTATI_ALG   /* numero totale ingressi                       */
#define NBLOCCHI1       NBLOCCHI+1      /* numero di blocchi +1                                 */
#define NSTATI_ALG1     NSTATI_ALG+1    /* numero ingressi = uscite + 1                         */
#define NVARIABILI1     NVARIABILI+1    /* numero di variabili + 1                              */


/* ridefinizione chiamate esterne a FORTRAN a causa della	*/
/* opzione -qextname usata di default su Alpha			*/

#define legbl1		legbl1_
#define modi2		modi2_
#define modc1		modc1_
#define leggiline	leggiline_
#define scrivif04	scrivif04_


/* dichiarazione di funzioni FORTRAN */

extern void legbl1 (int *, int [], int [], float [], float []);
// legbl1(&nu, ipi, ipvri, uu, xyu);
extern void modi2 (int *, int *, int *, int *, int [][NVARIABILI], int *, int *, int *,
                   float [], float [], int *, int *, int *, float [], float []);
extern void modc1 (int *, int *, float [][1], int *, int *, float [], int *,
                   float [], float [], int *, int *);
extern void leggiline (int *, int *, char [], char [], int *);


extern void scrivif04(char [], int*, int*, int*, int*, int*, int*, int*,
  char [], int[], int(*)[],int[], int[],int[], int[],char(*)[], int[],char(*)[], int[],
  int[], int[],int[], int[], float[],float[],float[],int*,  int[], float*, char (*)[], char (*)[]);
/*
            scrivif04(char [] f04name, &fn002, &nbl, &nvart, &neqsis, &npvrt, &nu, &nvri,
  FORTsigla, nosub, noblc, nusta, nusci, ningr, islb, nmbloc, ip, var, ipvrs,
  ips, ipvrt, ipi   , ipvri, xy,      uu,     xyu,   &ndati,ipdati, dati, nmsiva, nmvari);
*/
int essit(char *);
void initsm_();
int scrivi_f14();

/* definizione di alcune costanti				*/

#define SEPALINE	"****"
#define SEPALINELEN	4
#define USCLABEL	"US"
#define USCID		1
#define USC2LABEL	"UA"
#define USC2ID		2
#define INGLABEL	"IN"
#define INGID		3
#define US_IN_LEN	2
#define US_IN_POS	12
#define MAGICHAR_SUBVAR	'#'
#define MAGICPOS_SUBVAR	17
#define MAGICPOS_F14PLANT	19
#define MAGICPOS_F14VAR		4
#define MAGICPOS_F14VARVAL	14

#define MAXLINELENGTH	255
#define SIVARILINELEN	100
#define NMBLOCLINELEN	80
#define NO_DEBUG	0
#define ALL_DEBUG	99
#define INT_ITEM	1
#define STR_ITEM	2
#define FLOAT_ITEM	3
#define STR8		9
#define STR132		133


/* dichiarazione delle strutture dati				*/

int nosub[NBLOCCHI];
int noblc[2][NBLOCCHI];
int nusta[NBLOCCHI], nusci[NBLOCCHI], ningr[NBLOCCHI], islb[NBLOCCHI];
int ipvrt[NVARIABILI], ips[NSTATI_ALG1];
int ipvri[NTOTINGRESSI], ipi[NVARIABILI1];
float xy[NSTATI_ALG], uu[NINGRESSI], xyu[NVARIABILI];
float dati[NDATI];
int ndati, ipdati[NBLOCCHI1];
char var[NVARIABILI][STR8-1],vari[NINGRESSI][STR8], sivar[NSTATI_ALG][STR8];
int rvar[2][NVARIABILI];
char nmsiva[NSTATI_ALG][SIVARILINELEN];
char nmvari[NINGRESSI][SIVARILINELEN];
int nvart, nbl, nbl1, nu, nu1, neqsis, neqs1, neqal, npvrt, nvri;
char nmbloc[NBLOCCHI][NMBLOCLINELEN];
char riga[MAXLINELENGTH];
float ajac[1][1], rni[NSTATI_ALG];


/* dichiarazione delle strutture dati "dummy" (non usate ma	*/
/* inserite per garantire la compatibilita` del F04)		*/

char sigla[STR8], sig12[STR8], si12[STR8];
int ier, issis, nbtri, nst, nosl[NMODULI];
int ip[NBLOCCHI1], ipvrs[NVARIABILI];
int iousiv[NSTATI_ALG], iouvar[NINGRESSI];
int p0, h0, t0, q0, r0, al0, v0, dp0;
float cnxyu[NVARIABILI], toll[NSTATI_ALG], tol[NREGBLOCVAR];
float xn[NBLOCCHI], xin[NSTATI_ALG];


/* dichiarazione ed inizializzazione delle COMMON FORTRAN	*/

struct common_regime { unsigned kregim; };
struct common_regime regime_;

struct common_parpar { int nul[7];
                       int itert; };
struct common_parpar parpar_;

struct common_nequaz { int neqmod; };
struct common_nequaz nequaz_;

struct common_integr { float tstop, tempo, dtint;
                       int npas;
                       float cdt, alfadt; };
struct common_integr integr_;




/* dichiarazione dei parametri per l'utilizzo della leggiline()	*/

int inizpar, unitpar, errpar;
char nomef14[32], llline[STR132];

/* dichiarazione delle variabili globali			*/

FILE *finf01, *foutf04,*fp_old,*fp_s;
char tempstr[255];
int debuglevel;


/* subroutines 							*/


/*								*/
/* essit - routine di gestione errori fatali                    */
/*								*/

int essit(char *mesg)
{
  printf(mesg);
  exit(-1);
}




/*								*/
/* debugStr - invia alla console un messaggio diagnostico	*/
/*								*/

void debugStr(int igroup, char *mesg, int itemtype, int ival, char *istr, float fval)

{
  char safestr[MAXLINELENGTH];

  if (debuglevel == NO_DEBUG)	/* no debug */
    return;

  switch (itemtype) {
    case INT_ITEM: if (igroup == debuglevel || debuglevel == ALL_DEBUG)
		     printf("%s %d\n", mesg, ival);
		   break;
    case STR_ITEM: if (igroup == debuglevel || debuglevel == ALL_DEBUG)
		   {
		     strcpy(safestr, istr);
		     printf("%s %s\n", mesg, istr);
		   }
		   break;
    case FLOAT_ITEM: if (igroup == debuglevel || debuglevel == ALL_DEBUG)
		       printf("%s %f\n", mesg, fval);
		     break;
    default: printf("unrecognized item type for debug %s\n", mesg);
  }
}




/*								*/
/* checkInit - controlla la validita dei parametri passati come	*/
/* argomento e apre i files di input/output			*/
/*								*/

void checkInit (int argc, char **argv)

{
  if (argc != 3 && argc != 4)
    essit("Usage: pslego3 <F01 file> <F14 file>\n"); 
  else if (argc == 4)
    debuglevel = atoi(argv[3]);
  else
    debuglevel = NO_DEBUG;

  if ((finf01 = fopen(argv[1], "r")) == NULL)
  {
    sprintf(tempstr, "Fatal error: cannot open %s file\n", argv[1]);
    essit(tempstr);
  }

  strcpy(nomef14, argv[2]);
  inizpar = 1;
  unitpar = 14;

  leggiline(&inizpar, &unitpar, nomef14, llline, &errpar);
  if (errpar != 0)
  {
    sprintf(tempstr, "Fatal error: cannot open %s file\n", argv[2]);
    essit(tempstr);
  }

  inizpar = 0;
}




/*								*/
/* squeezeF01 - recupera dal file F01 tutte le informazioni	*/
/* sui blocchi e le loro variabili, oltre ai collegamenti tra	*/
/* queste, caricando i vettori globali.				*/
/*								*/

void  squeezeF01 ()

{
  char *priga;
  int endsection, bl, varuitype;
  int tnu, tnoblc;

  priga = fgets(riga, MAXLINELENGTH-1, finf01);	/* salta la prima riga separatrice */

/* inizializzazione globali */

  nbl = 0;
  nu = neqsis = tnu = 0;
  nvart = 0;

  endsection = 0;
  while ((priga = fgets(riga, MAXLINELENGTH-1, finf01)) != NULL  &&  !endsection)
  {
    if (strncmp (priga, SEPALINE, SEPALINELEN) == 0)	/* fine prima sezione */
      endsection = 1;
    else
      nbl++;
  }
  nbl1 = nbl+1;
  debugStr(1, "nbl value is", 1, nbl, "", .0);

  sscanf(riga, "%s", sigla);				/* leggi sigla impianto */
  sigla[STR8] = '\0';
  debugStr(1, "impianto is", 2, 0, sigla, .0);

  priga = fgets(riga, MAXLINELENGTH-1, finf01);		/* salta la prima riga separatrice */

  for (bl = 0; bl < nbl; bl++)
  {
    priga = fgets(riga, MAXLINELENGTH-1, finf01);	/* leggi riga di descrizione blocco	*/
    riga[strlen(riga)-1] = '\0';
    strncpy (nmbloc[bl], riga, NMBLOCLINELEN);
    sscanf(riga, "%s", tempstr);
    debugStr(10, "found block", 2, 0, riga, .0);

    memcpy(&tnoblc, tempstr, 4);			/* inserisci nome modulo,	*/
    nosub[bl] = tnoblc;
    memcpy(&tnoblc, tempstr, 4);			/* nome blocco (1.parte),	*/
    noblc[0][bl] = tnoblc;
    memcpy(&tnoblc, tempstr+4, 4);			/* nome blocco (2.parte).	*/
    noblc[1][bl] = tnoblc;
    islb[bl] = bl+1;
    debugStr(10, "block 1st half is", 1, noblc[0][bl], "", .0);
    debugStr(10, "block 2nd half is", 1, noblc[1][bl], "", .0);

    nusta[bl] = nusci[bl] = ningr[bl] = 0;

    endsection = 0;
    while ((priga = fgets(riga, MAXLINELENGTH-1, finf01)) != NULL  &&  !endsection)
    {
      riga[strlen(riga)-1] = '\0';

      if (strncmp (priga, SEPALINE, SEPALINELEN) == 0)	/* fine sezione blocco */
        endsection = 1;
      else
      {
	if (strncmp (priga+US_IN_POS, USCLABEL, US_IN_LEN) == 0)	/* controlla tipo var. */
	  varuitype = USCID;
	else if (strncmp (priga+US_IN_POS, USC2LABEL, US_IN_LEN) == 0)
	  varuitype = USC2ID;
	else if (strncmp (priga+US_IN_POS, INGLABEL, US_IN_LEN) == 0)
	  varuitype = INGID;

        sscanf (riga, "%s", tempstr);

        memcpy(var[nvart], tempstr, 8);			/* inserisci nome variabile,	*/
	/*var[nvart][STR8] = '\0';*/
        memcpy(&(rvar[0][nvart]), tempstr, 4);			/* nome variabile (1.parte),	*/
        memcpy(&(rvar[1][nvart]), tempstr+4, 4);		/* nome variabile (2.parte).	*/
        /*debugStr(2, "var 1st half is", 1, rvar[0][nvart], "", .0);
        debugStr(2, "var 2nd half is", 1, rvar[1][nvart], "", .0);*/

	if (varuitype != INGID)
	{
          strncpy (nmsiva[neqsis], riga, SIVARILINELEN);
          sscanf(riga, "%s", sivar[neqsis]);
          debugStr(2, "found var UA", 2, 0, sivar[neqsis], .0);

	  nusci[bl]++;
	  neqsis++;
	}
	else
	{
	  if (riga[MAGICPOS_SUBVAR] != MAGICHAR_SUBVAR)	/* l'ingresso non e` stato sostituito */
	  {
	    ipvri[tnu] = nvart+1;
	    ipi[tnu] = tnu+1;
	    tnu++;
	  }

          ningr[bl]++;
	}

	nvart++;
      }
    }
    debugStr(2, "blocco has IN", 1, ningr[bl], "", .0);
    debugStr(2, "blocco has UA", 1, nusci[bl], "", .0);
    debugStr(2, "\nfine blocco", 2, 0, "\n", .0);
  }

  endsection = 0;
  while ((priga = fgets(riga, MAXLINELENGTH-1, finf01)) != NULL  &&  !endsection)
  {
    riga[strlen(riga)-1] = '\0';

    if (strncmp (priga, SEPALINE, SEPALINELEN) == 0)	/* fine sezione INGRESSI */
      endsection = 1;
    else
    {
      strncpy (nmvari[nu], riga, SIVARILINELEN);
      sscanf(riga, "%s", vari[nu]);
      debugStr(2, "found var IN", 2, 0, vari[nu], .0);

      nu++;
    }
  }
  nvri = nu;
  ipi[tnu] = tnu+1;

  nu1 = nu+1;
  neqs1 = neqsis+1;
  neqal = neqsis;

  debugStr(1, "\nfine FILE", 2, 0, "\n", .0);
  debugStr(1, "file has IN", 1, nu, "", .0);
  debugStr(1, "(t-check) file has IN", 1, tnu, "", .0);
  debugStr(1, "file has UA", 1, neqsis, "", .0);

  fclose(finf01);
}




/*								*/
/* fillInvMatrix - calcola i vettori di collegamento inverso	*/
/* IPVRS, IPVRT e IPS.						*/
/*								*/

void fillInvMatrix()

{
  int i, v, w, b, stck, vispres;
  int ingrcount;

/* inizializzazione di una variabile globale */

  npvrt = 0;

/* riempimento IPVRS e IP */

  stck = 0;
  ingrcount = 1;
  for (b=0; b<nbl; b++)
  {
    ip[b] = stck + 1;

    for (v=stck; v<stck+nusci[b]; v++)
      for (w=0; w<neqsis; w++)
	if (strncmp (sivar[w], var[v], STR8-1) == 0)	/* la var. e' gia' presente */
	  ipvrs[v] = w+1;

    stck += nusci[b];

    for (v=stck; v<stck+ningr[b]; v++)
    {
      vispres = 0;

      for (w=0; w<neqsis; w++)
	if (strncmp (sivar[w], var[v], STR8-1) == 0)	/* la var. e' gia' presente */
	{
	  ipvrs[v] = w+1;
	  vispres = 1;
        }

      if (! vispres)
      {
        ipvrs[v] = -ingrcount;
        ingrcount++;
      }
    }
    stck += ningr[b];
  }
  ip[nbl] = stck + 1;

/* riempimento IPVRT e IPS */

  for (i=0; i<neqsis; i++)
  {
    ips[i] = npvrt+1;
    for (v=0; v<nvart; v++)
      if (strncmp (sivar[i], var[v], STR8-1) == 0)
      {
	ipvrt[npvrt] = v+1;
	npvrt++;
      }
  }
  ips[neqsis] = npvrt+1;
  
}




/*								*/
/* readF14 - recupera dal file F14 tutte le informazioni	*/
/* sui dati dei blocchi.					*/
/*								*/

void readF14()

{
  int v;

  leggiline(&inizpar, &unitpar, nomef14, llline, &errpar);	/* leggi sigla impianto */
  llline[STR132]='\0';
  strcpy(riga, llline);

  sscanf(riga+MAGICPOS_F14PLANT, "%s", sig12);
  if (strcmp(sigla, sig12) != 0)
  {
    sprintf (tempstr, "F14 data belong to %s plant, which is different from %s plant (in F01)\n",
	     sig12, sigla);
    essit(tempstr);
  }

  leggiline(&inizpar, &unitpar, nomef14, llline, &errpar);	/* salta dati normalizzazione */
  leggiline(&inizpar, &unitpar, nomef14, llline, &errpar);	/* salta dati normalizzazione */
  leggiline(&inizpar, &unitpar, nomef14, llline, &errpar);	/* salta riga "CONDIZIONI INIZIALI..." */

  for (v=0; v < neqsis; v++)
  {
    leggiline(&inizpar, &unitpar, nomef14, llline, &errpar);
    llline[STR132]='\0';
    strcpy(riga, llline);
    sscanf(riga+MAGICPOS_F14VAR, "%s", si12);
    sscanf(riga+MAGICPOS_F14VARVAL, "%f", &(xy[v]));
    if (strcmp(si12, sivar[v]) != 0)
    {
      sprintf (tempstr, "var %s (F14) doesn't match with var %s (F01)\n", si12, sivar[v]);
      essit(tempstr);
    }
    else
    {
      debugStr(4, "found var", 2, 0, si12, .0);
      debugStr(4, "val is", 3, 0, "", xy[v]);
    }
  }

  leggiline(&inizpar, &unitpar, nomef14, llline, &errpar);	/* salta riga "CONDIZIONI INIZIALI..." */

  for (v=0; v < nu; v++)
  {
    leggiline(&inizpar, &unitpar, nomef14, llline, &errpar);
    llline[STR132]='\0';
    strcpy(riga, llline);
    sscanf(riga+MAGICPOS_F14VAR, "%s", si12);
    sscanf(riga+MAGICPOS_F14VARVAL, "%f", &(uu[v]));
    if (strcmp(si12, vari[v]) != 0)
    {
      sprintf (tempstr, "var %s (F14) doesn't match with var %s (F01)\n", si12, vari[v]);
      essit(tempstr);
    }
    else
    {
      debugStr(4, "found var", 2, 0, si12, .0);
      debugStr(4, "val is", 3, 0, "", uu[v]);
    }
  }
}




/*								*/
/* doModi2Loop - esegue un loop su tutti i blocchi del modello	*/
/* chiamando la subroutine MODI2 opportunamente inizializzata	*/
/*								*/

void doModi2Loop()

{
  int ne, j, bl, ifun, k1, k2, i1, i2, tint;
  int nstati, nuscit, ningre, ibloc1, ibloc2, FORTbl;

  k1 = k2 = 1;
  ifun = 2;
    leggiline(&inizpar, &unitpar, nomef14, llline, &errpar);
  for (bl=0; bl < nbl; bl++)
  {
    FORTbl=bl+1;
    i1 = ip[bl];
    i2 = ip[bl+1]-1;
    nstati = nusta[bl];
    nuscit = nusci[bl];
    ningre = ningr[bl];
    ne = nusta[bl]+nusci[bl];

    for (j=0; j<ne; j++)
      tol[j] = 0.;

    ibloc1 = noblc[0][bl];
    ibloc2 = noblc[1][bl];

    tint = NVARIABILI;
    debugStr(13, "modI2ing block", 1, FORTbl, "", .0);
    modi2(&FORTbl, &ifun, &ibloc1, &ibloc2, rvar, &tint, &i1, &i2, xyu, dati,
	  &k1, &k2, &ier, cnxyu, tol);

    ipdati[bl] = k1;
    k2++;
    if (k2 <= NDATI)
    {
      ipdati[bl+1] = k2;
      k1 = k2;
    }
    else
      essit("data vector overflow\n");
  }
  ndati = ipdati[nbl]-1;
  debugStr(13, "NDATI val is", 1, ndati, "", .0);
}




/*								*/
/* doModc1Loop - esegue un loop su tutti i blocchi del modello	*/
/* chiamando la subroutine MODC1 opportunamente inizializzata	*/
/*								*/

void doModc1Loop()

{
  int bl, bl2, ipd, ixyu, ibloc1, ibloc2, ifun, FORTbl;
  int ne, j, jj, l, i1, ki, tint;
  int m, nus, kj1, kj2, kj, kkj;

  ifun = 2;

 for (bl2=0; bl2 < 5; bl2++)
 {
  m=0;
  for (bl=0; bl < nbl; bl++)
  {
    FORTbl = bl+1;
    ipd = ipdati[bl];
    ixyu = ip[bl];
    nus = nusci[bl];
    ibloc1 = noblc[0][bl];
    ibloc2 = noblc[1][bl];

    tint = 1;
    debugStr(13, "modC1ing block", 1, FORTbl, "", .0);
    modc1 (&FORTbl, &ifun, ajac, &tint, &ixyu, xyu, &ipd, dati, rni, &ibloc1, &ibloc2);

/* trasferimento di xyu (valori delle uscite di un blocco)
   nel vettore xy (variabili residui eq. del sistema)		*/

    for (j = 0; j<nus; j++)
    {
      m++;
      xy[m-1] = xyu[ixyu+j-1];

      kj1 = ips[m-1];
      kj2 = ips[m]-1;

      if (kj2 >= kj1)
        for (kj = kj1; kj <= kj2; kj++)
        {
          kkj = ipvrt[kj-1];
          xyu[kkj-1] = xy[m-1];
        }
    }
  }
 }
}




/*								*/
/* writeF04 - scrive sul file F04 le informazioni ricavate	*/
/* dall'esecuzione.						*/
/*								*/

void writeF04()

{
  FILE *foutf04;
  char FORTsigla[8], f04name[32];
  int i, fn002;

  if ((foutf04 = fopen("F04.asc", "w")) == NULL)
  {
    sprintf(tempstr, "Fatal error: cannot open %s file\n", "F04.asc");
    essit(tempstr);
  }

/* pseudo-F04 in formato ASCII */

  fprintf(foutf04, "NBL = %d  NEQAL = %d  NBL1 = %d  NVART = %d\n",
	            nbl, neqal, nbl1, nvart);

  fprintf(foutf04, "NEQSIS = %d  NEQS1 = %d  NPVRT = %d  NU = %d  NU1 = %d  NVRI = %d\n",
	            neqsis, neqs1, npvrt, nu, nu1, nvri);

  fprintf(foutf04, "ISSIS = %d  NBTRI = %d  NST = %d  SIGLA = %s\n",
		    issis, nbtri, nst, sigla);

  for (i=0; i<nst; i++)
  {
    if ((i+1)%10 == 0)
      fprintf(foutf04, "\n");
    fprintf(foutf04, "NOSL%d = %d  ", i+1, nosl[i]);
  }

  fprintf(foutf04, "\n         NUSTA NUSCI NINGR\n");
  for (i=0; i<nbl; i++)
    fprintf(foutf04, "Bloc %d   %d %d %d\n", i+1, nusta[i], nusci[i], ningr[i]);

/*
  for (i=0; i<nvart; i++)
    fprintf(foutf04, "VAR(%d)   %s\n", i+1, var[i]);
*/

  fprintf(foutf04, "\n         SIVAR IPS NMSIVA\n");
  /*for (i=0; i<neqsis; i++)
    fprintf(foutf04, "Out %d   %s %d %s\n", i+1, sivar[i], ips[i], nmsiva[i]);*/
  fprintf(foutf04, "\nIPVRT\n");
  for (i=0; i<npvrt; i++)
  {
    if ((i+1)%10 == 0)
      fprintf(foutf04, "\n");
    fprintf(foutf04, "v%d = %d  ", i+1, ipvrt[i]);
  }

  fprintf(foutf04, "\n         VARI IPI NMVARI\n");
  /*for (i=0; i<nu; i++)
    fprintf(foutf04, "In %d   %s %d %s\n", i+1, vari[i], ipi[i], nmvari[i]);*/
  fprintf(foutf04, "\nIPVRI\n");
  for (i=0; i<nvri; i++)
  {
    if ((i+1)%10 == 0)
      fprintf(foutf04, "\n");
    fprintf(foutf04, "v%d = %d  ", i+1, ipvri[i]);
  }

  fprintf(foutf04, "\nXY\n");
  for (i=0; i<neqsis; i++)
    fprintf(foutf04, "XY(%d) = %f\n", i+1, xy[i]);

  fprintf(foutf04, "\nUU\n");
  for (i=0; i<nu; i++)
    fprintf(foutf04, "UU(%d) = %f\n", i+1, uu[i]);

  fprintf(foutf04, "\nXYU\n");
  for (i=0; i<nvart; i++)
    fprintf(foutf04, "XYU(%d) = %f\n", i+1, xyu[i]);

  fprintf(foutf04, "\nNDATI = %d\n", ndati);
  for (i=0; i<nbl1; i++)
    fprintf(foutf04, "IPDATI(%d) = %d\n", i+1, ipdati[i]);
  for (i=0; i<ndati; i++)
    fprintf(foutf04, "DATI(%d) = %f\n", i+1, dati[i]);

  fclose(foutf04);

/* scrittura del file F04 binario */

  fn002 = NBLOCCHI;
  strncpy(FORTsigla, sigla, 8);
  strcpy(f04name, "F04.dat");

  scrivif04(f04name, &fn002, &nbl, &nvart, &neqsis, &npvrt, &nu, &nvri,
            FORTsigla, nosub, noblc, nusta, nusci, ningr, islb, nmbloc, ip, var, ipvrs,
            ips, ipvrt, ipi, ipvri, xy, uu, xyu, &ndati, ipdati, dati, nmsiva, nmvari);

}




/*								*/
/* writeN04 - scrive sul file N04 le informazioni ricavate	*/
/* dall'esecuzione.						*/
/*								*/

void writeN04()

{
  FILE *foutn04;
  int bl, ing, usc, usc_offset=0, ing_offset=0;

  if ((foutn04 = fopen("n04.dat", "w")) == NULL)
  {
    sprintf(tempstr, "Fatal error: cannot open %s file\n", "n04.dat");
    essit(tempstr);
  }

  fprintf(foutn04, "$%s\n%d\n", sigla, nbl);

  for (bl=0; bl<nbl; bl++)
  {
    nmbloc[bl][NMBLOCLINELEN] = '\0';	/* sneak hack */
    fprintf(foutn04, "%s\n%d\n", nmbloc[bl], nusci[bl]);

    for (usc=0; usc<nusci[bl]; usc++)
      fprintf(foutn04, "%s\n", sivar[usc + usc_offset]);
    usc_offset += nusci[bl];

    fprintf(foutn04, "%d\n", ningr[bl]);
    for (ing=0; ing<ningr[bl]; ing++)
      fprintf(foutn04, "%s\n", vari[ing + ing_offset]);
    ing_offset += ningr[bl];
  }

  fprintf(foutn04, "****\n1\n%d\n", ndati);

  fclose(foutn04);
}




/*								*/
/* main - programma principale					*/
/*								*/

int main (int argc, char **argv)

{
  int i;

/* inizializzazione COMMON fortran */

  regime_.kregim = 1;
  parpar_.itert = -1;
  integr_.tempo = 0.;
  integr_.dtint = 1.;
  nequaz_.neqmod = -1;

  initsm_();
printf("dopo initsm \n");

  checkInit(argc, argv);
printf("dopo checkInit \n");

  squeezeF01();
printf("dopo squeeze \n");

  fillInvMatrix();
printf("dopo fillInv \n");

  readF14();
printf("dopo readF14 \n");

  legbl1(&neqsis, ips, ipvrt, xy, xyu);
printf("dopo legbl1 1 \n");

  legbl1(&nu, ipi, ipvri, uu, xyu);
printf("dopo legbl1 2 \n");

debugStr(5, "num.var is", 1, nvart, "", .0);
printf("dopo debugStr \n");

  for (i=0; i < nvart; i++)
    cnxyu[i] = 1.;

  doModi2Loop();
printf("dopo doModi2Loop \n");

  doModc1Loop();
printf("dopo doModc1Loop \n");

  writeF04();
printf("dopo writeF04 \n");

  writeN04();
printf("dopo writeN04 \n");

  scrivi_f14();
printf("dopo scrivi_f14 \n");

exit(0);
}
 /*
routines aggiunte per la scrittura di un f24.dat (Villa)
*/

/**************** function carica_riga_intera da f14.dat a f14.s ****/

int carica_riga_intera(riga, lun)

char riga [MAXLINELENGTH];
int *lun;
{
 int c, k;

 for (k=0; (c = fgetc( fp_old)) != '\n'; k++)
 {
  if (c == EOF)
   {
   /*printf( "\n leggi_riga_intera: trovato fine del file \n ");*/
   return(1);
   }
  riga[k]=c;
 }
 riga[k]=0;
 *lun = k;
 return(0);
}

          
/****************  function di scrittura del f14.s  ***************/


int  scrivi_f14()
{
 int p,k,lun,iret;
 char riga[MAXLINELENGTH];
 char f14_old[80];
 char f14_s[80];
 int  carica_riga_intera();
 int   correggi_valore();

 strcpy(f14_old,"f14.dat");
 fp_old=fopen(f14_old,"r");
 if (fp_old==NULL)
    {
     printf("\n scrivi_f14: ERRORE IN APERTURA DEL FILE %s",f14_old );
     fflush(stdout);
     exit(11);
    }

  strcpy(f14_s,"proc/f24.dat");
 fp_s=fopen(f14_s,"w");
 if (fp_s==NULL)
    {
     printf("\n scrivi_f14: ERRORE IN APERTURA DEL FILE %s",f14_s );
     fflush(stdout);
     exit(12);
    }

 /* scrivo le prime 4 righe che non cambiano */
 for(k=0;k<4;k++)
    {
     carica_riga_intera(riga,&lun);
     fprintf(fp_s,"%s\n",riga);
    }
       
 /* scrivo le righe corrette delle variabili di uscita */
 for(k=0;k<neqsis;k++)
    {
     carica_riga_intera(riga,&lun);
     iret=correggi_valore(riga,xy[k]);
     fprintf(fp_s,"%s\n",riga);
    }


 /* salto la riga di commento "CONDIZIONI INIZIALI"  */

 carica_riga_intera(riga,&lun);
 fprintf(fp_s,"%s\n",riga);
 
 /* scrivo le righe corrette delle variabili di ingresso */

 for(k=0;k<nu;k++)
    {
     carica_riga_intera(riga,&lun);
     iret=correggi_valore(riga,uu[k]);
     fprintf(fp_s,"%s\n",riga);
    }


 /* scrivo tutto il resto del file uguale  */

 while(carica_riga_intera(riga,&lun)==0)
    {
     fprintf(fp_s,"%s\n",riga);
    }

 fclose(fp_old);
 fclose(fp_s);

 return(1);
}



/****** function correggi_valore per la sostituzione dei valori ****/

 int correggi_valore(riga,valore)

char riga[MAXLINELENGTH];
float valore;

{
 char val_char[15];
 char *priga;
 char *lun_num;

 priga=riga;
 priga=priga+14;


 /* se il numero e' positivo aumento la precisione  */
 if(valore<0)
    sprintf(val_char,"%-10.4G",valore);
 else
    sprintf(val_char,"%-10.5G",valore);


 if((strchr(val_char,'.')==NULL)&&(strchr(val_char,'E')==NULL))
   {
    lun_num =strchr(val_char,' ');
    memcpy(lun_num,".",1);
   }

 memcpy(priga,val_char,10);
}
               
