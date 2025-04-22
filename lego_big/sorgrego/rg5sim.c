/*
   modulo rg5sim.c
   tipo 
   release 1.1
   data 11/15/95
   reserved @(#)rg5sim.c	1.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)rg5sim.c	1.1\t11/15/95";
/*
        Fine sezione per SCCS
*/
/* inclusione dei file header                                   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

/* definizione delle COMMON Fortran e subroutines esterne	*/

#define regime		regime_
#define parint		parint_
#define integr		integr_
#define parpar		parpar_
#define inpar1          inpar1_

#define scanf04		scanf04_
#define fetch04		fetch04_
#define rg5sim		rg5sim_
#define initsm		initsm_
#define legbl1          legbl1_
#define modc1           modc1_

extern void legbl1_ (int *, int [], int [], float [], float []);

extern void modc1_ (int *, int *, float [][1], int *, int *, float [], int *,
  float [], float [], int *, int *);

extern void initsm_();



/*
extern void reg000_(int*, float*[], float*[], float*, float*[])
reg000_(&ipr, xy, uu, &px, dati, &(inpar1.neqsis), &(inpar1.nu),
&neqdif, &(inpar1.ndati), cnxy, cnuu, &(parint.tstep), &ifine,
&t_proc, &jac, &(parpar.itert));
*/


struct common_regime {
			short kregim;
		     } regime;

struct common_parint {
			int ititol[20];
			float tfin, tstep, fatoll;
                        int interr;
                        float tinter;
		     } parint;

struct common_integr {
                        float tstop, tempo, dtint;
                        int npas;
                        float cdt, alfadt;
                     } integr;

struct common_parpar {
			float nul[7];
			int itert;
		     } parpar;

struct common_inpar1 {
			int nbl;
			int neqal;
			int neqsis;
			int nu;
			int nst;
			int nvart;
			int npvrt;
			int nvri;
			int ndati;
		     } inpar1;


/* strutture "dummy" mantenute per non modificare le liste di	*/
/* parametri delle funzioni chiamate				*/

int nosl[1], nosub[1], noblc[2][1], nusta[1], ningr[1], islb[1], ipvrs[1];
float cnxyu[1], ajac[1][1];

/* puntatori ai valori delle variabili e ai dati allocati	*/
/* dinamicamente						*/

int *nusci, *ip, *ips, *ipvrt, *ipi, *ipvri, *ipdati;
float *xy, *uu, *xyu, *dati, *rni, *cnxy, *cnuu;




/* fprot - stampa i valori contenuti nell'area puntata da ppp */

void fprot(char *vett, float *ppp, int nitem)

{
  int i;

  for (i=0; i<nitem; i++)
  {
    printf("%s - %d = %f", vett, i, ppp[i]);
    if ((i+1)%5 == 0) printf("\n");
  }
}





/* iprot - stampa i valori contenuti nell'area puntata da ppp */

void iprot(char *vett, int *ppp, int nitem)

{
  int i;

  for (i=0; i<nitem; i++)
  {
    printf("%s - %d = %d", vett, i, ppp[i]);
    if ((i+1)%5 == 0) printf("\n");
  }
}





/*								*/
/* myIalloc - alloca il numero di byte indicati da msize e li	*/
/* ritorna al chiamante						*/
/*								*/

int *myIalloc(size_t msize)

{
  int *ret, i, *tptr;

  if ((ret = (int *)malloc(msize)) == NULL && (int)msize != 0)
    {printf("sorry: cannot i-alloc requested memory\n"); exit(-1);}
/*
  else
  {
    tptr = ret;
    for (i=0; i<msize/sizeof(int); i++)
      tptr[i]=0;
    return(ret);
  }
*/
    return(ret);
}




/*								*/
/* myFalloc - alloca il numero di byte indicati da msize e li	*/
/* ritorna al chiamante						*/
/*								*/

float *myFalloc(size_t msize)

{
  int i;
  float *ret, *tptr;

  if ((ret = (float *)malloc(msize)) == NULL)
    {printf("sorry: cannot f-alloc requested memory\n"); exit(-1);}
/*
  else
  {
    tptr = ret;
    for (i=0; i<msize/sizeof(float); i++)
      tptr[i]=0.0;
    return(ret);
  }
*/
    return(ret);
}




void rgdyns()

{
  int neqdif, t_proc, ifun, ipr, ifine, ipd;
  float jac, px, dum_tstep;
  int i, j, k, m, nus, ixyu, kj1, kj2, kj, kkj;
  int uno = 1, dummy = 0;

  for (i=0; i<inpar1.neqsis; i++)
    cnxy[i] = 1.0;

  for (i=0; i<inpar1.nu; i++)
    cnuu[i] = 1.0;

   
// printf("dopo i for\n");

  neqdif = 0;

  parpar.itert = 0;
  jac = 0.0;

  t_proc = 0;
  ifun = 2;
  integr.npas = 0;
  integr.tempo = 0.0;
  ipr = 1;

   
// printf("prima di reg000 inizializzazione \n");

/*sleep(5);*/
  reg000_(&ipr, xy, uu, &px, dati, &(inpar1.neqsis), &(inpar1.nu),
	 &neqdif, &(inpar1.ndati), cnxy, cnuu, &(parint.tstep), &ifine,
	 &t_proc, &jac, &(parpar.itert));
   
// printf("dopo reg000 inizializzazione \n");


  while (ifine != 2)	/* loop di integrazione */
  {
    integr.dtint = parint.tstep;
    integr.npas++;
    integr.tempo = integr.tempo + integr.dtint;
    legbl1(&(inpar1.neqsis), ips, ipvrt, xy, xyu);
    legbl1(&(inpar1.nu), ipi, ipvri, uu, xyu);

    /* calcolo della risposta della regolazione */

    m = 0;
    for (i=0; i<inpar1.nbl; i++)
    {
	k = i+1;
        ipd = ipdati[i];
	nus = nusci[i];

	ixyu = ip[i];

	modc1(&k, &ifun, ajac, &uno, &ixyu, xyu, &ipd, dati, rni, &dummy, &dummy);

      /* trasferimento di xyu (valori delle uscite di un blocco) 	*/
      /* nel vettore xy (variabili residui eq. del sistema)		*/

      for (j=0; j<nus; j++)
	{
	  m++;
	  xy[m-1] = rni[j];
	  /*xy[m-1] = xyu[ixyu+j-1];*/

	  /* trasferimento delle uscite verso gli ingressi degli altri	*/
	  /* schemi connessi (non esisteva in Regograf)			*/

	  kj1 = ips[m-1];
	  kj2 = ips[m] - 1;

	  if (kj2 >= kj1)
	  for (kj=kj1; kj<=kj2; kj++)
	  {
	    kkj = ipvrt[kj-1];
	    xyu[kkj-1] = xy[m-1];
	  }
	}
    }		/* fine aggiunta trasferimento uscite	*/

    ipr = 2;

    reg000_(&ipr, xy, uu, &px, dati, &(inpar1.neqsis), &(inpar1.nu),
	   &neqdif, &(inpar1.ndati), cnxy, cnuu, &(parint.tstep), &ifine,
	   &t_proc, &jac, &(parpar.itert));
  }
/*
fprot("xy", xy, 10);
fprot("dati", dati, 10);
iprot("ipdati", ipdati, 10);
*/
}




void rg5sim()

{
  regime.kregim = 0;

  initsm();
   
// printf("dopo initsm\n");

  fetch04();

// printf("ndims are %d %d %d %d %d\n", inpar1.nbl,
//         inpar1.neqsis, inpar1.nvart, inpar1.nu, inpar1.ndati);

/* allocazione vettori per il calcolo */

  nusci = myIalloc(sizeof(int) * inpar1.nbl);
  ip = myIalloc(sizeof(int) * (inpar1.nbl + 1));
  ips = myIalloc(sizeof(int) * (inpar1.neqsis + 1));
  ipvrt = myIalloc(sizeof(int) * inpar1.npvrt);
  ipi = myIalloc(sizeof(int) * (inpar1.nu + 1));
  ipvri = myIalloc(sizeof(int) * inpar1.nvri);
  xy = myFalloc(sizeof(float) * inpar1.neqsis);
  uu = myFalloc(sizeof(float) * inpar1.nu);
  xyu = myFalloc(sizeof(float) * inpar1.nvart);
  cnxy = myFalloc(sizeof(float) * inpar1.neqsis);
  cnuu = myFalloc(sizeof(float) * inpar1.nu);
  dati = myFalloc(sizeof(float) * inpar1.ndati);
  ipdati = myIalloc(sizeof(int) * (inpar1.nbl + 1));
  rni = myFalloc(sizeof(float) * inpar1.neqsis);

   
// printf("dopo allocazioni\n");

  scanf04(nusci, ip, ips, ipvrt, ipi, ipvri, xy, uu, xyu, dati, ipdati);

   
// printf("dopo scanf04\n");

  rgdyns();
   
// printf("dopo rgdyns\n");

}
