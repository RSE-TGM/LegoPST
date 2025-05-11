/*
   modulo chgcompiled.c
   tipo 
   release 1.2
   data 2/23/96
   reserved @(#)chgcompiled.c	1.2
*/
/*
        Variabile per identificazione della versione
*/
 


#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>


extern int cambiac(char *,char *);
extern int found;
#define DIM 200        /*dimensione max della linea da leggere da file*/
#define NEWL 30
#define MAXARG 5

/* CAMBIO TAG */

char wordkeymodify;                         /*var per nome record da modificare */
char newrecord[NEWL];                             /*var che contiene la nuova stringa */
char offset1[80],offset2[80];    /*in salvapagina .bak */

FILE *fp,*fp1,*fp2;



int main (argc,argv)
int argc;
char **argv;
{ 

int off1=0;
int ritm2=0;

int risultato=0;
int i;

/*
Errore 1 file msg not open
Errore 2 sintassi non corretta
Errore 3 errore generico
Errore 4 errore su inizio di campo
Con uscita 0 tutto Ok*/
/* file per messaggi */
if((fp=fopen("chgcompiled.msg","w"))==NULL)
	{
	
	exit(1);	
	}

if (argc<2) {
 
  fprintf(fp,"\n \t\tSintassi di chgcompiled:\n");
  fprintf(fp,"chgcompiled  offset1 offset2 < Inputfile_name.rtf >Outputfile_name.rtf ");
  fgetc(stdin);
  fprintf(fp,"\n offset1: primo parametro ");
  fprintf(fp,"\n offset2 valore intero che verr� sommato al secondo parametro");
  fprintf(fp,"\n Verranno modificati cosi' i primi 2 parametri di tutti i campi compiled");
  fclose(fp);	
  exit(2);

  }
 	
else
  {

   /* in argv[1] parametro di offset1 
    * in argv[2] parametro di offset2
    
    */



/*copio in offset valori di argv*/      
  
	
   strcpy(offset1,argv[1]);			
   strcpy(offset2,argv[2]); 	
   
   
   
   
/*Azzera tabella parole chiave da trovare */
   
   

/*Chiama la routine di sostituzione*/

risultato=cambiac(offset1,offset2);

/*Valutazione degli errori */
 
 	   switch(risultato){
   		case 3:
   		/*segnalazione di errore generico*/
   		fprintf(fp,"\n\t errore\n ");
   		getc(stdin);
   		fclose(fp);
   		exit(3);
   		break;
   		
   		case 0:
   		fprintf(fp,"\n\t Operazione conclusa positivamente\n");
   		break;
   		
   		case 4:
   		fprintf(fp,"\n\t errore sul campo(manca # o not compiled)\n ");
   		getc(stdin);
   		fclose(fp);
   		exit(4);
   		break;
   		
   		default:
   		break;
   			
   		} 	/* end switch */
  } 	/*end if ... else*/
fclose(fp); /*chiudi file errori*/
exit(0);
} 	/* end main */









