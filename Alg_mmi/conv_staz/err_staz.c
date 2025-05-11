/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)err_staz.c	1.2\t6/16/93";
/*
   modulo err_staz.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)err_staz.c	1.2
*/
/*
	gestione errori di compilazione stazioni 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *tab_err[]={	 
				   "DESCRIZIONE aaaaaaaa... ",
               "PAGINA  nnn				 ",
               "POSIZIONE  nn nn			 ",
               "LABELS  aaaa  bbbb  cccc",
               "U_MISURA  aaaaaa			 ",
               "SCALAMENTO  nnnn.nn		 ",
               "SCALAM_ERR  nnnn.nn		 ",
               "MINMAX  nnn nnn			 ",
               "MINMAX_ERR  nnn nnn		 ",
               "STAZ_ASSOC  nnn			 ",
               "LOGICA_ASS  0 o 1		 ",
               "STATO  vvvvvv  mmmmmm	 ",
               "ESAM_RICH  vvvvvv  mmmmmm",
               "VALORE  vvvvvv  mmmmmm	  ",
               "VAL_ERR  vvvvvv  mmmmmm  ",
               "RICH_STATO  vvvvvv  mmmmmm",
               "RICH_PIU  vvvvvv  mmmmmm	",
               "RICH_MENO  vvvvvv  mmmmmm	",
               "RICH_TARGET  vvvvvv  mmmmmm",
               "TARGET  vvvvvv  mmmmmm		 ",
				   "DESCRIZIONE aaaaaaaa... ",
               "GRUPPO1   vvvvvv  mmmmmm  ",
               "GRUPPO2   vvvvvv  mmmmmm  ",
"LED, PULSANTE, PULS_LUCE, LAMPADA, SELETTORE, INDICATORE, STRINGA,TASTO, LUCE" ,
		"COLORE	cccccc (BIANCO,GIALLO,VERDE,ROSSO,GRIGIO)",
		"ETIC	eeeeeeeeee ( max 40 caratteri )   ",
		"INPUT	vvvvvvvv  mmmmmm                  ",
		"INPUT_BLINK	vvvvvvvv  mmmmmm          ",
		"OUTPUT		vvvvvvvv  mmmmmm pppppp   ",
		"STEP, IMPULSO, NEGAZIONE, UP_DOWN        ",
		"INPUT_ERR	vvvvvvvv  mmmmmm          ",
		"OFFSET	nnnn.mmmm        ",
		"INIBIZIONE	vvvvvvvv  mmmmmm                  ",
		};
	


extern	int nriga;
extern	FILE *fo;

void errore(ind,riga)
int ind;				/* indice errori */
char *riga;			
{
printf("\n ATTENZIONE: la riga %d : \n %s \n  non e' del tipo: \n",nriga,riga);
printf("\n %s",tab_err[ind]);
fprintf(fo,"\n ATTENZIONE: la riga %d : \n %s \n  non e' del tipo: \n",nriga,riga);
fprintf(fo,"\n %s",tab_err[ind]);
exit(puts("\n il programma COMPSTAZ termina per errore "));
}

