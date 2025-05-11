/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)sa1_c.c	1.2\t6/16/93";
/*
   modulo sa1_c.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)sa1_c.c	1.2
*/
/*
	compilazione stazione di tipo sa1
*/

#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>

#include "sim_param.h"
#include "sim_types.h"
#include "xstaz.h"
#include "compstaz.inc"

// void legge_riga( char *riga, int *lun, int *nriga );
// void separa_str( char *riga, int lun, int nstr, STRIN_ST strin[]);

extern	FILE *fp_s01;
extern  FILE *fo;
extern	STRIN_ST string[];

extern	S_PAGINE   pag;
extern	S_STAZIONI staz;
extern	char riga [80];
extern	int nriga;

int staz_sa1_c(istaz,itipo,nmod)
int istaz;
int itipo;
int nmod;
{
int  i, lun, ier, nstr, ipag, index, imi, imu, ipx, ipy, numbyte;
float valore, valmin, valmax;
char *px;

/* legge la descrizione della stazione */

  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=10, string);
  if (strncmp(string[0].stringa,"DESCRIZIONE",11) ||
      			  					   (string[1].stringa == NULL ))  errore(ERR_DES,riga);
  i=1;
  numbyte=0;
  px=(char *)staz.s[istaz].descrizione;
  memset(staz.s[istaz].descrizione,' ',sizeof (staz.s[istaz].descrizione));
  while (string[i].lun_stringa && numbyte+string[i].lun_stringa + 1 < sizeof (staz.s[istaz].descrizione))
  {
   strncpy(px,string[i].stringa,string[i].lun_stringa);
	px+= string[i].lun_stringa + 1 ;  /* lascia un blank */
	numbyte+= string[i].lun_stringa +1;
	i++;
	}
  fprintf(fo,"\n %s",(char *)staz.s[istaz].descrizione);

/*
	legge la pagina associata alla stazione
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,"PAGINA",6) ||
      									   (string[1].stringa == NULL ))  errore(ERR_PAG,riga);
  if (!sscanf(string[1].stringa,"%3d",&ipag))  errore(ERR_PAG,riga);
  if (ipag < 1 || ipag >MAX_PAG)  errore(ERR_PAG,riga);
  staz.s[istaz].pagina=ipag;
  fprintf(fo,"\n pagina %d",ipag);

/*
	legge la posizione della  stazione
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,"POSIZIONE",9) ||
			   (string[1].stringa == NULL ) || (string[2].stringa == NULL ))  errore(ERR_POS,riga);
  if (!sscanf(string[1].stringa,"%2d",&ipx))  errore(ERR_POS,riga);
  if (!sscanf(string[2].stringa,"%2d",&ipy))  errore(ERR_POS,riga);
	staz.s[istaz].posix0=ipx;
	staz.s[istaz].posiy0=ipy;
	staz.s[istaz].posix1=ipx+2;
	staz.s[istaz].posiy1=ipy+1;
  fprintf(fo,"\n pos x %d",ipx);
  fprintf(fo,"\n pos y %d",ipy);
/*
	legge l'unit di misura 
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,"U_MISURA",8))  errore(ERR_UMIS,riga);
  if (string[1].stringa == NULL)
 	memset(&staz.s[istaz].umis[0],' ',sizeof(staz.s[istaz].umis));
   else	
 	memcpy(&staz.s[istaz].umis[0],string[1].stringa,string[1].lun_stringa);
  	fprintf(fo,"\n u.mis %s ",staz.s[istaz].umis);
/*
	legge lo scalamento della variabile
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,"SCALAMENTO",10) ||
						  (string[1].stringa == NULL ))  errore(ERR_SCAL,riga);
  if (co_float(13,6,&string[1],&valore)) errore(ERR_SCAL,riga);
  staz.s[istaz].scalam=valore;
  	fprintf(fo,"\n valore %f ",valore);
/*
	legge i valori di fondo scala 
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,"MINMAX",6) ||
			  (string[1].stringa == NULL ) || (string[2].stringa == NULL ))  errore(ERR_MINMAX,riga);
  if (co_float(13,6,&string[1],&valmin)) errore(ERR_MINMAX,riga);
  if (co_float(13,6,&string[2],&valmax)) errore(ERR_MINMAX,riga);
  staz.s[istaz].fs[0]=valmin;
  staz.s[istaz].fs[1]=valmax;
  	fprintf(fo,"\n min %f ",valmin);
  	fprintf(fo,"\n max %f ",valmax);
/*
	legge la variabile di valore e verifica se appartiene al modello indicato
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,"VALORE",6) ||
		  (string[1].stringa == NULL) || (string[2].stringa == NULL ))  errore(ERR_VALORE,riga);
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  staz.s[istaz].valore=index;
  	fprintf(fo,"\n valore :%d ",index);
  return(0);

}


	
	

	
