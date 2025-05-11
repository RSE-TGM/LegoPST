/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)amd_c.c	1.2\t6/16/93";
/*
   modulo amd_c.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)amd_c.c	1.2
*/
/*
	compilazione stazione di tipo amd
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

int staz_amd_c(istaz,itipo,nmod)
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
  if (ipag < 1 || ipag > MAX_PAG) errore(ERR_PAG,riga);
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
	staz.s[istaz].posiy1=ipy+2;
  fprintf(fo,"\n pos x %d",ipx);
  fprintf(fo,"\n pos y %d",ipy);
/*
	lettura delle labels della stazione 
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=4, string);
  if (strncmp(string[0].stringa,"LABELS",6) ||
  (string[1].stringa == NULL ) || (string[2].stringa == NULL ) ||
											  (string[3].stringa == NULL ))  errore(ERR_LAB,riga);
        memset(&staz.s[istaz].etic[0][0],' ',4);
        memset(&staz.s[istaz].etic[1][0],' ',4);
        memset(&staz.s[istaz].etic[2][0],' ',4);

  	memcpy(&staz.s[istaz].etic[0][0],string[1].stringa,string[1].lun_stringa);
  	memcpy(&staz.s[istaz].etic[1][0],string[2].stringa,string[2].lun_stringa);
  	memcpy(&staz.s[istaz].etic[2][0],string[3].stringa,string[3].lun_stringa);
  	fprintf(fo,"\n labels %s ",&staz.s[istaz].etic[0][0]);
  	fprintf(fo,"\n labels %s ",&staz.s[istaz].etic[1][0]);
  	fprintf(fo,"\n labels %s ",&staz.s[istaz].etic[2][0]);

/* legge la descrizione del valore della stazione */

  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=10, string);
  if (strncmp(string[0].stringa,"DESCRIZIONE_VALORE",18) ||
		(string[1].stringa == NULL ))  errore(ERR_DESVAL,riga);
  i=1;
  numbyte=0;
  px=(char *)staz.s[istaz].mod;
  memset(staz.s[istaz].mod,' ',sizeof (staz.s[istaz].mod));
  while (string[i].lun_stringa && numbyte+string[i].lun_stringa + 1 < sizeof (staz.s[istaz].mod))
  {
   strncpy(px,string[i].stringa,string[i].lun_stringa);
        px+= string[i].lun_stringa + 1 ;  /* lascia un blank */
        numbyte+= string[i].lun_stringa +1;
        i++;
        }
  	fprintf(fo,"\n des val %s ",staz.s[istaz].mod);
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
	legge l'indice della stazione associata
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,"STAZ_ASSOC",10) ||
				  (string[1].stringa == NULL ) )  errore(ERR_ASSOC,riga);
  if (!sscanf(string[1].stringa,"%3d",&index))  errore(ERR_ASSOC,riga);
  staz.s[istaz].stassoc[0]=index;
  	fprintf(fo,"\n staz assoc %d ",index);
/*
	legge la logica  della stazione associata
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,"LOGICA_ASS",10) ||
				  (string[1].stringa == NULL ) )  errore(ERR_LOGICA,riga);
  if (!sscanf(string[1].stringa,"%3d",&index))  errore(ERR_LOGICA,riga);
  if (index == 0 || index == 1 ) staz.s[istaz].stassoc[1]=index;
  else	 errore(ERR_LOGICA,riga);
  	fprintf(fo,"\n logic assoc %d ",index);
/*
	legge la variabile di stato e verifica se appartiene al modello indicato
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,"STATO",5) ||
		  (string[1].stringa == NULL) || (string[2].stringa == NULL ))  errore(ERR_STATO,riga);
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  staz.s[istaz].slstaz=index;
  	fprintf(fo,"\n stato :%d ",index);
/*
	legge la variabile di esamina richiesta  e verifica se appartiene 
	al modello indicato
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,"ESAM_RICH",9) ||
		  (string[1].stringa == NULL) || (string[2].stringa == NULL ))  errore(ERR_ESAMR,riga);
  check_model(string[2].stringa,&imu);
  check_output(string[1].stringa,imu,&index);
  staz.s[istaz].slesamr=index;
  	fprintf(fo,"\n esam_rich :%d ",index);
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
/*
	legge la variabile di richiesta stato  e verifica se appartiene al
	modello indicato
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,"RICH_STATO",10) ||
		  (string[1].stringa == NULL) || (string[2].stringa == NULL ))  errore(ERR_RSTATO,riga);
  check_model(string[2].stringa,&imu);
  check_input(string[1].stringa,imu,&index);
  staz.s[istaz].rlstaz[0]=index;
  staz.s[istaz].rlstaz[1]=imu;
  	fprintf(fo,"\n rich stato :%d %d",index,imu);
/*
	legge la variabile di richiesta aumenta  e verifica se appartiene al
	modello indicato
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,"RICH_PIU",8) ||
		  (string[1].stringa == NULL) || (string[2].stringa == NULL ))  errore(ERR_RPIU,riga);
  check_model(string[2].stringa,&imu);
  check_input(string[1].stringa,imu,&index);
  staz.s[istaz].rlaum[0]=index;
  staz.s[istaz].rlaum[1]=imu;
  	fprintf(fo,"\n rich piu :%d %d",index,imu);
/*
	legge la variabile di richiesta diminuisci  e verifica se appartiene al
	modello indicato
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,"RICH_MENO",9) ||
		  (string[1].stringa == NULL) || (string[2].stringa == NULL ))  errore(ERR_RMENO,riga);
  check_model(string[2].stringa,&imu);
  check_input(string[1].stringa,imu,&index);
  staz.s[istaz].rldim[0]=index;
  staz.s[istaz].rldim[1]=imu;
  	fprintf(fo,"\n rich meno :%d %d",index,imu);
/*
/*
	legge la variabile di richiesta target e verifica se appartiene al
	modello indicato
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,"RICH_TARGET",11) ||
		  (string[1].stringa == NULL) || (string[2].stringa == NULL ))  errore(ERR_RTARGET,riga);
  check_model(string[2].stringa,&imu);
  check_input(string[1].stringa,imu,&index);
  staz.s[istaz].rltarget[0]=index;
  staz.s[istaz].rltarget[1]=imu;
  	fprintf(fo,"\n rich target :%d %d",index,imu);
/*
	legge la variabile di target e verifica se appartiene al	modello indicato
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=3, string);
  if (strncmp(string[0].stringa,"TARGET",6) ||
		  (string[1].stringa == NULL) || (string[2].stringa == NULL ))  errore(ERR_TARGET,riga);
  check_model(string[2].stringa,&imu);
  check_input(string[1].stringa,imu,&index);
  staz.s[istaz].target[0]=index;
  staz.s[istaz].target[1]=imu;
  	fprintf(fo,"\n target :%d %d",index,imu);
  return(0);

}


	
	

	
