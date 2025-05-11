/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)br1_c.c	1.2\t6/16/93";
/*
   modulo br1_c.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)br1_c.c	1.2
*/
/*
	compilazione stazione di tipo br1
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
extern	FILE *fo;
extern	STRIN_ST string[];


extern  char *ind_sh_top;
extern	S_PAGINE   pag;
extern	S_STAZIONI staz;
extern	char riga [80];
extern	int nriga;

int staz_br1_c(istaz,itipo,nmod)
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
  while (string[i].lun_stringa && numbyte +string[i].lun_stringa + 1< sizeof (staz.s[istaz].descrizione))
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
	lettura delle labels della stazione 
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,"LABELS",6) ||
  (string[1].stringa == NULL ) || (string[2].stringa == NULL ))  errore(ERR_LAB,riga);
        memset(&staz.s[istaz].etic[0][0],' ',4);
        memset(&staz.s[istaz].etic[1][0],' ',4);
  	memcpy(&staz.s[istaz].etic[0][0],string[1].stringa,string[1].lun_stringa);
  	memcpy(&staz.s[istaz].etic[1][0],string[2].stringa,string[2].lun_stringa);
        fprintf(fo,"\n labels %4s ",&staz.s[istaz].etic[0][0]);
        fprintf(fo,"\n labels %4s ",&staz.s[istaz].etic[1][0]);

/*
	legge la variabile di richiesta gruppo 1 e 2
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,"GRUPPO1",7) ||
       (string[1].stringa == NULL) || (string[2].stringa == NULL )) errore(ERR_GRUP1,riga);
  check_model(string[2].stringa,&imu);
  check_input(string[1].stringa,imu,&index);
  staz.s[istaz].rlaum[0]=index;
  staz.s[istaz].rlaum[1]=imu;
        fprintf(fo,"\n gruppo 1 :%d %d",index,imu);

  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,"GRUPPO2",7) ||
        (string[1].stringa == NULL) || (string[2].stringa == NULL )) errore(ERR_GRUP2,riga);
  check_model(string[2].stringa,&imu);
  check_input(string[1].stringa,imu,&index);
  staz.s[istaz].rldim[0]=index;
  staz.s[istaz].rldim[1]=imu;
        fprintf(fo,"\n gruppo 2 :%d %d",index,imu);
	return(0);
}
	
	

	



