
/*
   modulo chgcompiled1.c
   tipo 
   release 1.2
   data 2/23/96
   reserved @(#)chgcompiled1.c	1.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)modifica.c	1.1\t1/8/96";
/*
        Fine sezione per SCCS
*/
/*             modifica.c     F.Lodigiani

 * sono contenute tutte le routine necessarie per modpag.c e
 * modallpag.c
 */



#include<stdio.h>
#include<stdlib.h>

#include<string.h>

#define DIM 2000

#define NUMTOKEN0 0
#define NUMTOKEN6 6
#define NUMTOKEN8 8
#define TERMINATOR '|'

/*
 cambia : sostituisce nel file stdin primo par di ogni campo
somma secondo parametro
In questa Release vengono modificate solo quelle variabili delimitate da un fine
campo '---' 
 se l' operazione � riuscita ritorna 1
*/
extern FILE *fp;
unsigned int n_linea;

/***********************/
char *scorri_campo(int *fine,char *car)
/**********************/
/*
Questa funzione scorre il campo verificando nel frattempo il numero di token 
trovati.
Come sottocampo che fa da terminatore si sono presi in considerazione i caratteri
ASCII scrivibili cioe' >65 il carattere / (47) e sequenze di - (45)
*/
{
int end; /*fine campo:'---' */
unsigned int num_token,end_campo,tokenOk;
end=0;
num_token=0;
end_campo=0;


if(!(*fine)) /*trova seq. di carattteri terminatori */	
	while(!end&&!(*fine)){
		/*printf("Linea%s\n",car);
		getc(stdin);*/
		if(strlen(car)==1)
			*fine=1;

		if(((*car)==' ')) /*conta i token*/
			{
			num_token+=1;			
			}	
/*		end_campo=((*car)=='-')&&(*(car+1)=='-')||(((int)(*car))>65);*/	
		end_campo=((*car)==TERMINATOR);
		if(end_campo)
			end=1;	
		car++;
		}

/*
Una volta che ha raggiunto la fine del campo controlla quanti token ho letto 
*/
	tokenOk=(num_token==0)||(num_token==6)||(num_token==8);
	if(!tokenOk)
		fprintf(fp,"Alla linea %d trovato campo con %d sottocampi\n",n_linea,num_token);
	
/*inizio di un nuovo campo*/
return(car);

}
/************************************************************/
int cambiac(char *off1,char *off2)
/************************************************************/


{
char linea[DIM],stringa[DIM],*car,*car1,*car2;
char comando[50],buf1[50],buf2[50];/*di appoggio*/
char *tmppag="filetmp";
int int1,int2,ris;	/*valori interi letti*/


/*linea =linea letta,*/
/*fine = nessun campo trovato             */


int i,set,fine;



FILE *fp1,*fp2;
/*
 apri in lettura file da convertire
*/
set=0;

/*apri file temporaneo (copia modificata)*/


/*if((fp1=fopen("LIVA.rtf","r"))==NULL)
	{
	printf("\n\tImpossibile aprire file temporaneo\n");
	return(3);	
	}
if((fp2=fopen("LIVA.fin","w"))==NULL)
	{
	printf("\n\tImpossibile aprire file temporaneo\n");
	return(3);	
	}*/



/*printf("Sono nella cambiacompiled\n");*/
/*Legge tutte le stringhe del file */
n_linea=0;		
while ((fgets(linea,DIM-1,stdin))!=NULL) {

n_linea+=1;	/*contatore linee lette*/


/*Verifica se trova una parola delle parole chiave */
/*la parola chiave deve essere preceduta da * o da . */

		car=strstr(linea,"compiled");

/*Se la stringa trovata verifica che sia il nome di una risorsa */
		
		if(car &&(*(car+strlen("compiled"))==':')&&( ((*(car-1))=='.')||((*(car-1))=='*') )){

	/*Parola chiave trovata*/		

	
		 /*Vai alla fine della parola chiave */	
			set=1;		

		 	while((*car)!=':'){
		 		car++;
		 		}
		 		car++;

	/* punta all' inizio del valore daella risorsa da modificare */
	/*salta i caratteri tab o blank */

		 	while(((*car)==9)||((*car)==' ')){
		 		car++;
		 		}
	/*
	Se non 1� car non cancelletto -> errore
	*/	
			if(((*car)!='#')&&((*car)!='n')){
				
				return(4);	 			 		
				}
			else car++;
	fine=0;
					
/*
Fin qui resta tutto = alla prima release 
*/
	while(((*car)!='\n')&& ((*car)!='o')&&!fine)
		{
/*printf("Da elaborare %s\n",car);
getc(stdin);*/
	/* Inizia operazione di modifica*/
			while(((*car)==' '))
		 		car++;
		/*arrivato inizio primo valore*/
			if(((*car)!='-')&&(*(car+1)!='1'))
				{
			
				/* procedura di conv.*/
				car1=car;
				car2=car;
				if(strlen(car)==1){
					/*strcpy(car,off1);
					
					strcat(linea," \n");*/
					fine=1;
					
					}		
				else{
/* car punta alla fine del primo valore */
					car=strstr(car1," ");
					/*in buf1 2 valore da somm.*/
					sscanf(car,"%s",buf1);
					car2=strstr((car+1)," ");
/* Copio il resto della linea in stringa */	
					strcpy(stringa,car2);
/* copio offset1 nel posto puntato da car1 */
					strcpy(car1,off1);
					
					int2=atoi(off2);
					
					int1=atoi(buf1);
					/*printf("Valore del due%s\n",buf1);*/
					ris=int1+int2;
					
					sprintf(buf2,"%d",ris);
/*
Copio un blank il risultato somma del 2o campo ed il resto della stringa
*/
					/*printf("Ris%s\n",buf2);
					getc(stdin);*/
					strcat(linea," ");
					strcat(linea,buf2);
					strcat(linea,stringa);
					
			/*cambiato 1� car*/	
			/*cambiato 2o car */
						
					}
				}/* end if 1o valore !=-1*/
						
		car=scorri_campo(&fine,car);			
									
		
		}/* end while not newline*/
	/* inserimento controllo dei token */	
	
	}/*end if global*/	
	fputs(linea,stdout);	


}/*end while*/
/*fclose(fp1);
fclose(fp2);*/
return(0);
}
/****************************************************************/

