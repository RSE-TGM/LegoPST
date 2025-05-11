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
static char SccsID[] = "@(#)cnewstaz.c	1.4\t3/23/95";
/*
   modulo cnewstaz.c
   tipo 
   release 1.4
   data 3/23/95
   reserved @(#)cnewstaz.c	1.4
*/
/*
	Compila le nuove stazioni
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

extern  FILE *fp_s01;
extern  FILE *fo;
extern  STRIN_ST string[];

extern  char *ind_sh_top;

extern  S_PAGINE   pag;
extern  S_STAZIONI staz;
extern  char riga [80];
extern  int nriga;

extern	TIPI_NEWSTAZ new_staz[];

extern 	char *stipi_oggetti[];
extern char  *scolori_oggetti[];
extern char  *stipo_perturb[];

		
int compila_new_staz(istaz,itipo,nmod)
int	istaz;		/* indice stazione */
int	itipo;		/* indice nuovo tipo in tabella new_staz */
int	nmod;		/* numero modelli */
{

int i,lun,ipag,nstr,ipx,ipy,oggetto;
char *p_r02;
TIPO_LED *p_r;

/*
	legge la riga di descrizione ma non la memorizza 
	e se non c'e' non da errore 
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=1, string);
  if (strncmp(string[0].stringa,"DESCRIZIONE",11)) errore(ERR_DES,riga);
/*
        legge la pagina associata alla stazione
*/
  legge_riga( riga, &lun, &nriga);
  separa_str( riga, lun, nstr=2, string);
  if (strncmp(string[0].stringa,"PAGINA",6) || (string[1].stringa == NULL ))
							  errore(ERR_PAG,riga);
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
                           (string[1].stringa == NULL ) || (string[2].stringa ==
 NULL ))  errore(ERR_POS,riga);
  if (!sscanf(string[1].stringa,"%2d",&ipx))  errore(ERR_POS,riga);
  if (!sscanf(string[2].stringa,"%2d",&ipy))  errore(ERR_POS,riga);
        staz.s[istaz].posix0=ipx;
        staz.s[istaz].posiy0=ipy;
        staz.s[istaz].posix1=ipx+ new_staz[itipo].larg;
        staz.s[istaz].posiy1=ipy+ new_staz[itipo].altezza;
  fprintf(fo,"\n pos x %d",ipx);
  fprintf(fo,"\n pos y %d",ipy);

/*
   legge l' identificativo del tipo di oggetto 
*/
  p_r02 = (char*) staz.s[istaz].descrizione;
  for (i=0; i<new_staz[itipo].num_oggetti;i++)
  {
	legge_riga( riga, &lun, &nriga);
	separa_str( riga, lun, nstr=1, string);
        if (strcmp(string[0].stringa,stipi_oggetti[new_staz[itipo].ogg[i].tipo]))
	{
	      printf("\n oggetto previsto : %s ",stipi_oggetti[new_staz[itipo].ogg[i].tipo]);
	      fprintf(fo,"\n oggetto previsto : %s ",stipi_oggetti[new_staz[itipo].ogg[i].tipo]);
             errore(ERR_OGGETTO,riga);
        }

/* gestisce a seconda del tipo di oggetto */

         switch (new_staz[itipo].ogg[i].tipo)
	  {
		case LED:
		c_led(p_r02);
		p_r02+= sizeof (TIPO_LED);
		break;

		case PULSANTE:
		c_puls(p_r02);
		p_r02+=sizeof (TIPO_PULS);
		break;

		case PULS_LUCE:
                c_pulsluce(p_r02);
                p_r02+=sizeof (TIPO_PULS_LUCE);
		break;

		case LAMPADA:
		c_lamp(p_r02);
                p_r02+=sizeof (TIPO_LAMPADA);
                break;

		case SELETTORE:
		c_selet(p_r02);
                p_r02+=sizeof (TIPO_SELETTORE);
                break;

                case INDICATORE:
                c_indic(p_r02,new_staz[itipo].ogg[i].sottotipo);
                p_r02+=sizeof (TIPO_INDICATORE);
                break;

                case STRINGA_DESCR:
                c_stringa(p_r02);
                p_r02+=sizeof (TIPO_STRINGA_DESCR);
                break;

                case DISPLAY:
                c_display(p_r02);
                p_r02+=sizeof (TIPO_DISPLAY);
                break;

                case LUCE:
                c_luce(p_r02);
                p_r02+=sizeof (TIPO_LUCE);
                break;

                case TASTO:
                c_tasto(p_r02);
                p_r02+=sizeof (TIPO_TASTO);
                break;

                case SET_VALORE:
                c_setval(p_r02);
                p_r02+=sizeof (TIPO_SET_VALORE);
                break;

		case DISPLAY_SCALATO:
                c_displayscal(p_r02);
                p_r02+=sizeof (TIPO_DISPLAY_SCALATO);
                break;

		case INDICATORE_SINCRO:
                c_sincro(p_r02);
                p_r02+=sizeof (TIPO_INDICATORE_SINCRO);
                break;
    	  }
   }
}


/*
 is_neg
 riconosce se si desidera una variabile negata in ingresso
 (parola chiave NOT)
*/
is_neg(str)
char *str;
{
if(str==NULL) return(0);
if(strcmp(str,s_not)==0)
	return(1);
else
	return(0);
}
/*
 Estrae il valore da assegnare ad una perturbazione: se non e' definito
 o non definito correttamente assegna per default il valore 1.0
*/
float get_valore(str)
char *str;
{
float value;
if(str==NULL) return(1.0);
if(sscanf(str,"%f",&value)!=1)
        {
        printf("\n WARNING : valore per la perturbazione non accettabile");
        value=1.0;
        }
return(value);
}
