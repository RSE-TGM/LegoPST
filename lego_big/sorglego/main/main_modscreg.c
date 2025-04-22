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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#define DICHIARA "CHARACTER*80 DESC_BLOC"
#define COMMON "C BUFDATI:VETTORE CONTENENTE I DATI RICALCOLATI E, A \
PARTIRE DA\nC INIZOFF, I  RISPETTIVI OFFSET NEL VETTORE DATI. IL VETTORE E'\n\
C CHIUSO CON -1.\n\
      REAL BUFDATI(1)\n\
      COMMON /BUFFER/ IDBUFREG,NSKIP,INIZOFF,BUFDATI\n"
#define ASSNEQ "NEQMOD = 0"
#define ASSKIP "      NSKIP = 0\n      IDBUFREG = 1\n"
#define ASSI2  "ID2=IDP"
#define TAPPO  "      BUFDATI(INIZOFF+IDBUFREG) = -1\n"
#define BUFFER "COMMON /BUFFER/"

int ncaratteri(char *);

char riga[200];

int main(argc,argv)
int argc;
char **argv;
{
char nomefile[150];
char newfile[150];
char riganew[400];
FILE *fp;
FILE *fp_copia;
int size=0,nrighe=0;
int newline=0,una_sostituzione=1;
char *app;
char *punt;
char *puntf;
char *ret;


     if(argc!=2)
	exit(fprintf(stderr,"usa: riscrivi <path file da modificare> \n"));
     strcpy(nomefile,argv[1]);
     fp = fopen(nomefile,"r");
     if ( fp==NULL )
        exit(fprintf(stderr,"errore apertura file da modificare\n"));
/*   Nome del nuovo file */
     strcpy(newfile,nomefile);
     puntf = strstr(newfile,".f");
     if (puntf==NULL)
        exit(fprintf(stderr,"Il file trattato non e' del tipo *.f\n"));
     strcpy(puntf,".new");
     fp_copia = fopen(newfile,"w");
     if ( fp_copia==NULL )
        exit(fprintf(stderr,"errore apertura file copia.\n"));
     while ( (fgets(riga,sizeof(riga),fp))!=NULL )
        nrighe++;
     rewind(fp);
     app = (char*)calloc(nrighe,200);
     punt = app;
     while ( (fgets(riga,sizeof(riga),fp))!=NULL )
        {
/*      Controllo che il file non sia gia' stato modificato */
        if ( NULL != (ret=strstr(riga,BUFFER)) )
           {
           free(app);
           fclose(fp);
           fclose(fp_copia);
           exit(fprintf(stderr,"Il file %s e' gia' modificato\n",nomefile));
           }
        if ( NULL != (ret=strstr(riga,DICHIARA)) )
           {
           newline = 1;
           strcpy(riganew,COMMON);
           }
        if ( (NULL != (ret=strstr(riga,ASSNEQ))) && (una_sostituzione) )
           {
           newline = 1;
           una_sostituzione = 0;
           strcpy(riganew,ASSKIP);
           }
        if ( NULL != (ret=strstr(riga,ASSI2)) )
           {
           newline = 1;
           strcpy(riganew,TAPPO);
           }
        memcpy(app,riga,strlen(riga));
        size += strlen(riga);
        app += strlen(riga);
        if (newline) 
           {
           newline = 0;
           memcpy(app,riganew,strlen(riganew));
           size += strlen(riganew);
           app += strlen(riganew);
           }
        }
     fwrite(punt,size,1,fp_copia);
     free(app);
     fclose(fp);
     fclose(fp_copia);
     printf("\n\n     Nuovo file prodotto=%s\n\n",newfile);
}



/*
   Sostituisce a partire da ret il carattere newstr.
*/
int sostituisci(char *ret, char *newstr, int loldstr, int lnewstr)
{
char resto_riga[100];
int kk;

strcpy(resto_riga,ret+loldstr);
strcpy(ret,newstr);
strcat(ret+lnewstr,resto_riga);
kk = ncaratteri(riga);
riga[kk+1] = '\n';
riga[kk+2] = '\0';
}


/*
   Decrementa la lunghezza di una stringa finche' trova 
   un carattere non blank.
*/
int ncaratteri(char *parola)
{
int i;

i =  strlen(parola);
while( (parola[i-2] == ' ')&&(i>0) )
   i--;
return(i-1);
}
