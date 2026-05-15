/**************************************************************

 i32i5.c   v. 1.2

 Creato:        27 Febbraio 1995
 scritto da:    Marco Bonasoro
 
 Modificato:    25 Maggio 2000 (GM)

 descrizione:
 File sorgente dell'applicazione i32i5, un
 convertitore che accetta in input un file .pi4, e scrive
 automaticamente il file .I5 corrispondente.

 **************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef Linux
#include <ctype.h>
#endif

#include "pag2f01.h"
#include "i32i5.h"



#define MAXLINELENGTH   512
#define I5NAMENCH	4
#define MATMODNAMENCH	4


extern void read_conn(void);

/* global var */

  i3mvdesc mvlist[MAXNMATCFG][MAXNMATVAR];
  i3fpdesc fplist[MAXNFPORT];
  int nstati, nuscite, ningressi;
  int nfport, nconfig;
  char ftrue2[MAXNFISVAR*5][GENERICVARNAMENCH+1];
  int ftrue2count;
  




/* essit - routine di gestione errori fatali                    */

void essit(char *mesg)

{
  printf(mesg);
  exit(-1);
}




/* findmvtype4fvname - accetta in input un nome di var. fisica	*/
/* e restituisce:						*/
/*	 0 - la var. non compare tra quelle matematiche		*/
/*	 1 - la var. � una var. matematica di ingresso		*/
/*	-1 - la var. � una var. matematica di uscita		*/

int findmvtype4fvname(int cfg, char *fvname)

{
  int mv, nmv, ret;
  char c;

  ret = 0;
  nmv = nstati + nuscite + ningressi;

  for (mv=0; mv<nmv; mv++)
    if (strcmp(fvname, mvlist[cfg][mv].mvname) == 0)
    {
      c = mvlist[cfg][mv].mvtype[0];

      ret = c == 'I' ? 1 : -1;
    }

  return(ret);
}




/* fvisinftrue - accetta in input un nome di var. fisica	*/
/* restituisce 0 se la variabile non compare in ftrue2,	1 se	*/
/* compare							*/
/*								*/
/* viene usata per stabilire se una precedente assegnazione di	*/
/* un valore 1 ad un ingresso di una porta di ingresso deve	*/
/* essere modificata in 2					*/

int fvisinftrue(char *fvname)

{
  int fv, ret;

  ret = 0;

  for (fv=0; fv<ftrue2count; fv++)
    if (strcmp(ftrue2[fv], fvname) == 0)
      ret = 1;

  return(ret);
}




/* put2infnum - richiede in input un nome di var. fisica e un	*/
/* valore di configurazione					*/
/* sostituisce nel fnum appropriato il valore 2 alla variabile	*/
/* indicata							*/

void put2infnum(int cfg, char *fvname)

{
  int fv, nfv, fp;

  for (fp=0; fp<nfport; fp++)
    if (fplist[fp].fptype % 2 == 0)		/* la porta � di ingresso */
    {
      nfv = fpvnum[fplist[fp].fptype];

      for (fv=0; fv<nfv; fv++)
        if (strcmp(fvname, fplist[fp].fvar[fv]) == 0)	/* variabile da modificare */
	  fplist[fp].fnum[cfg][fv] = 2;
    }
}




/*                                                              */
/*                      MAIN program                            */
/*                                                              */

int main (int argc, char **argv)

{
  FILE *fpI3, *fpI5;
  char tempstr[MAXLINELENGTH], dustr1[16], dustr2[16];
  char i5name[I5NAMENCH+4], modname[MATMODNAMENCH+2];
  char modcomment[MAXLINELENGTH];
  char cfgcomment[MAXNMATCFG][MAXLINELENGTH];
  char dummyline[MAXLINELENGTH];
  int cf, nstinus, mv, fp, nfv, fv, mvt, allequal, imm;

  /* legge il file dei connettori */
  
  read_conn();

  /* controlla il numero di argomenti */

  if (argc != 2)
  {
	  char cucu;
 	printf ("vuoi uscire? ");
	fflush(stdout);
	scanf ("%c", &cucu);
	printf("\n Hai detto %c\n", cucu);
   essit("usage: i32i5 <file.pi4>\n");
  }

  /* apri il file .I3 */

  if ((fpI3 = fopen(argv[1], "r")) == NULL)
  {
    sprintf(tempstr, "cannot open %s file\n", argv[1]);
    essit(tempstr);
  }


  /* leggi le informazioni fino a numconfig */

  fscanf(fpI3, "%s %s\n", i5name, modname);
/* conversione in minuscolo */
//    for (imm=0; imm< strlen(modname); imm++) modname[imm]=(char)tolower(modname[imm]);
    for (imm=0; imm< strlen(i5name); imm++) i5name[imm]=(char)tolower(i5name[imm]);

printf("i5name=%s modname=%s\n",i5name,modname);
  fgets(modcomment, MAXLINELENGTH-1, fpI3);
  modcomment[strlen(modcomment)-1] = '\0';
printf("0 - i5name=%s\n",i5name);
  fscanf(fpI3, "%s %c %d\n", dustr1, dustr2, &nstati);
  fscanf(fpI3, "%s %c %d\n", dustr1, dustr2, &nuscite);
  fscanf(fpI3, "%s %c %d\n", dustr1, dustr2, &ningressi);
  fscanf(fpI3, "%s %c %d\n", dustr1, dustr2, &nconfig);
  nstinus = nstati+nuscite+ningressi;
printf("1 - i5name=%s\n",i5name);

  /* leggi gli eventuali commenti di configurazione */

  if (nconfig>1)
    for (cf=0; cf<nconfig; cf++)
      strcpy(cfgcomment[cf], "no comment\n");


  /* leggi le variabili matematiche */

  for (cf=0; cf<nconfig; cf++)
  {
    for (mv=0; mv<nstinus; mv++)
    {
      fgets(dummyline, MAXLINELENGTH-1, fpI3);
      strncpy(mvlist[cf][mv].mvcomment, dummyline+17, strlen(dummyline)-16);
      sscanf(dummyline, "%s --%c%c",	/* W the readability!! */
		   mvlist[cf][mv].mvname,
		   &(mvlist[cf][mv].mvtype[0]), &(mvlist[cf][mv].mvtype[1]));
      mvlist[cf][mv].mvtype[2] = '\0';
    }
    fgets(dummyline, MAXLINELENGTH-1, fpI3);
  }
  fgets(dummyline, MAXLINELENGTH-1, fpI3);


  /* leggi le informazioni sulle porte fisiche */

  fscanf(fpI3, "%d", &nfport);

  for (fp=0; fp<nfport; fp++)
  {
    fscanf(fpI3, "%s %d\n",
		  fplist[fp].fpname,
		  &(fplist[fp].fptype));
    fgets(fplist[fp].fpcomment, COMMENTNCH-1, fpI3);

    nfv = fpvnum[fplist[fp].fptype];

    for (fv=0; fv<nfv; fv++)
      fscanf(fpI3, "%s", fplist[fp].fvar[fv]);
  }


  fclose(fpI3);



  /* apri il file .I5 */

  strcpy(tempstr, i5name);
  strcat(tempstr, ".i5");
printf("i5name=%s tempstr=%s\n",i5name,tempstr);

  if ((fpI5 = fopen(tempstr, "w")) == NULL)
  {
    sprintf(tempstr, "-> cannot open %s.i5\n", i5name);
    essit(tempstr);
  }


  /* scarica informazioni generali su I5 */

  fprintf(fpI5, "%s\n\n%s\n%d\n%d\n\n",
          modname,
          modcomment,
          nfport, nconfig);


  /* calcola configurazioni numeriche per le variabili fisiche */

  for (cf=0; cf<nconfig; cf++)			/* per ogni configurazione... */
  {

    ftrue2count = 0;			/* azzera ftrue2 */
    for (fv=0; fv<MAXNFISVAR*5; fv++)
      strcpy(ftrue2[fv], "");

    /* tratta porte fisiche di ingresso */

    for (fp=0; fp<nfport; fp++)
      if (fplist[fp].fptype % 2 == 0)		/* la porta � di ingresso */
      {
        nfv = fpvnum[fplist[fp].fptype];

        for (fv=0; fv<nfv; fv++)
        {
  	  if (strcmp(NULLFVAR, fplist[fp].fvar[fv]) == 0)	/* variabile nulla */
	    fplist[fp].fnum[cf][fv] = 0;
          else
          {
	    mvt = findmvtype4fvname(cf, fplist[fp].fvar[fv]);

            if (mvt == 0)					/* variabile non matematica */
	      fplist[fp].fnum[cf][fv] = 2;
	    else
	    {
	      fplist[fp].fnum[cf][fv] = mvt;

	      if (mvt == 1)					/* ingr. matematico: segna in ftrue2 */
	        strcpy(ftrue2[ftrue2count++], fplist[fp].fvar[fv]);
	    }
	  }
        }
      }


    /* tratta porte fisiche di uscita */

    for (fp=0; fp<nfport; fp++)
      if (fplist[fp].fptype % 2 != 0)		/* la porta � di uscita */
      {
        nfv = fpvnum[fplist[fp].fptype];

        for (fv=0; fv<nfv; fv++)
        {
  	  if (strcmp(NULLFVAR, fplist[fp].fvar[fv]) == 0)	/* variabile nulla */
	    fplist[fp].fnum[cf][fv] = 0;
	  else
	  {
	    mvt = findmvtype4fvname(cf, fplist[fp].fvar[fv]);

            if (mvt == 0)					/* variabile non matematica */
	      fplist[fp].fnum[cf][fv] = 2;
	    else
	    {
	      if (mvt == -1)					/* uscita matematica */
	        fplist[fp].fnum[cf][fv] = 1;
	      else						/* ingr. matematico: controlla ftrue2 */
	      {
	        if (fvisinftrue(fplist[fp].fvar[fv]) == 1)	/* era segnata in ftrue2 */
	        {
		  fplist[fp].fnum[cf][fv] = 2;
		  put2infnum(cf, fplist[fp].fvar[fv]);
	        }
	        else
		  fplist[fp].fnum[cf][fv] = -1;
	      }
	    }
	  }
        }
      }

  }

  if (nconfig>1)				/* devi calcolare numbase */
    for (fp=0; fp<nfport; fp++)
    {
      nfv = fpvnum[fplist[fp].fptype];

      for (fv=0; fv<nfv; fv++)
      {
	allequal = 1;

        for (cf=0; cf<nconfig-1; cf++)
	  if (fplist[fp].fnum[cf][fv] != fplist[fp].fnum[cf+1][fv])
	    allequal = 0;

	if (allequal == 1)			/* in numbase mettiamo il valore delle tre config. */
	  fplist[fp].fnum[CFGBASESLOT][fv] = fplist[fp].fnum[0][fv];
	else					/* la var. � indeterminata: mettiamo 3 */
	  fplist[fp].fnum[CFGBASESLOT][fv] = 3;
      }
    }



  /* scarica informazioni sulle porte fisiche */

  for (fp=0; fp<nfport; fp++)
  {
    fprintf(fpI5, "%s\n", fplist[fp].fpname);

    nfv = fpvnum[fplist[fp].fptype];

    for (fv=0; fv<nfv; fv++)
      fprintf(fpI5, "%s ", fplist[fp].fvar[fv]);
    fprintf(fpI5, "\n");

    if (nconfig>1)
    {
      for (fv=0; fv<nfv; fv++)
	fprintf(fpI5, "%d ", fplist[fp].fnum[CFGBASESLOT][fv]);
      fprintf(fpI5, "\n");
    }

    for (cf=0; cf<nconfig; cf++)
    {
      for (fv=0; fv<nfv; fv++)
	fprintf(fpI5, "%d ", fplist[fp].fnum[cf][fv]);
      fprintf(fpI5, "\n");
    }
  }
  fprintf(fpI5, "\n");


  /* scarica informazioni sulle configurazioni matematiche */

  fprintf(fpI5, "%d\n%d\n%d\n\n",
		nstati, nuscite, ningressi);

  if (nconfig>1)
  {
    for (cf=0; cf<nconfig; cf++)
      fprintf(fpI5, "%s", cfgcomment[cf]);
    fprintf(fpI5, "\n");
  }

  for (cf=0; cf<nconfig; cf++)
  {
    for (mv=0; mv<nstinus; mv++)
      fprintf(fpI5, "%s %s\n%s",
		    mvlist[cf][mv].mvname,
		    mvlist[cf][mv].mvtype,
                    mvlist[cf][mv].mvcomment);
    fprintf(fpI5, "\n");
  }


  fclose(fpI5);
}
