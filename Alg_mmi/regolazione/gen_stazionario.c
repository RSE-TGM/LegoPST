/**********************************************************************
*
*       C Source:               gen_stazionario.c
*       Subsystem:              2
*       Description:
*       %created_by:    zanna %
*       %date_created:  Thu Jul  4 09:17:12 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: gen_stazionario.c,2 %  (%full_filespec: 2,csrc,gen_stazionario.c,2 %)";
#endif
/*
   modulo gen_stazionario.c
   tipo 
   release 5.2
   data 11/30/95
   reserved @(#)gen_stazionario.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)gen_stazionario.c	5.2\t11/30/95";
/*
        Fine sezione per SCCS
*/
#ifndef VMS
/*
	gen_stazionario 
	Programma di generazione dei files schema_f14.dat
	e schema_04.dat con i valori di inizializzazione
        Solo per ambiente UNIX !!
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined VMS
#include <stat.h>
#endif
#if defined UNIX
#include <memory.h>
#include <sys/stat.h>
#endif


#define VAPO "$LEGO_LIB/vapo.a"
#define LEGOLIB "$LEGO_LIB/legolib.a"
#if defined AIX
 char *compila = "xlf -c -qextname";
 char *linka= "xlf -o";
#else
 char *compila = "f77 -O -c ";
 char *linka= "f77 -o";
#endif

void XlOpenFileErrComp(char *nomeFile);
void XlErrComp(char* ,char *,char *,char *);
void XlSetFileErrComp(char *);

int main(argc, argv)
int argc;
char *argv[];
{
int  ret;       /* Codice tipi errore; config gestisce solo da 1 a 4.  */
int  rtd,i, lun;
int  in_use;
char nl[2];
char schema[5];
char schemao[10];
char schema_f14[10], schema_f04[10];
char buf[1500];
char stringa[FILENAME_MAX];
char file_err[FILENAME_MAX];
char libschemi[FILENAME_MAX];
char libreg[FILENAME_MAX];
char libmacro[FILENAME_MAX];
char path_rlib[FILENAME_MAX];
char work_path[FILENAME_MAX];
struct stat st;


	if (argc != 6)
           {
           fprintf(stderr,
"USO CORRETTO: gen_stazionario <schema> <lib_path> <work_path> <file_err> <in_use>\n");
           exit(2);
           }
        strcpy(schema,argv[1]);
        strcpy(path_rlib,argv[2]);
        strcpy(work_path,argv[3]);
        strcpy(file_err,argv[4]);
        in_use = atoi(argv[5]);

        strcpy(schemao,schema);
        strcat(schemao,".o");

/*      Setto variabili di path */
	sprintf(libschemi,"%s/schemi_reg.a",work_path);
	sprintf(libreg,"%s/libreg/reglib.a",path_rlib);
	sprintf(libmacro,"%s/macro_reg.a", work_path);
        if( stat(libmacro,&st)!=0 )
          strcpy(libmacro,"");

/*      Compilazione di lg3reg.f */
   	sprintf(buf,"%s -g -o %s/lg3reg.o %s/lg3reg.f", 
                compila,work_path,work_path);
	/*
        printf("\t Frase di compilazione: [%s] (%d)\n\n",buf,strlen(buf));
	fflush(stdout);
        */
	rtd = system(buf);
	if ( rtd != 0)
                {
                strcpy(stringa,"Compilazione fallita di lg3reg.f");
                ret=3;
		goto FINE;
                }

/*      Generazione eseguibile lg3reg */ 
        if ( in_use==1 )
           {
           sprintf(buf,"%s %s/lg3reg %s/lg3reg.o %s %s %s %s %s", linka,
                   work_path,work_path,libschemi,libmacro,libreg,LEGOLIB,VAPO);
           /*printf("\t Frase di link=[%s] (%d)\n\n",buf,strlen(buf));*/
           }
        else
           if ( in_use==0 )
		{
/*
                Viene testata l'esistenza di schemi_reg.a che in questo
                caso potrebbe non essere mai stata creata.
*/
                if( stat(libschemi,&st)!=0 )
                   strcpy(libschemi,"");
                sprintf(buf,"%s %s/lg3reg %s/lg3reg.o %s %s %s %s %s %s", linka,
                      work_path,work_path,schemao,
                      libschemi,libmacro,libreg,LEGOLIB,VAPO);
                /*
                printf("\t Frase di link caso not in use =[%s] (%d)\n\n",
                          buf,strlen(buf));
                */
		}
           else
              {
              strcpy(stringa,"Schema senza flag uso/non uso\n");
              ret=5;
	      goto FINE;
              }
        /*printf("\t Frase di link=[%s] (%d)\n\n",buf,strlen(buf));*/
	rtd = system(buf);
	if (rtd != 0 )
                {
                strcpy(stringa,"Generazione eseguibile lg3reg fallita");
                ret=6;
		goto FINE;
                }

/*      Esecuzione lg3reg nel work_dir e ritorno a quello corrente */
	sprintf(buf,"lg3reg");
        /* 
	printf("\t Esecuzione : [%s] (%d)\n\n",buf,strlen(buf));
	fflush(stdout);
        */
	rtd = system(buf);
	if ( rtd!=0 )
                {
                strcpy(stringa,"Calcolo regime schema (lg3reg fallito)");
                ret=4;
		goto FINE;
                }

/*      Cancellazione di lg3reg, lg3reg.f lg3reg.o */
	sprintf(buf,"rm lg3reg*");
        /*
	printf("\t Cancellazione [%s]\n\n",buf);
	fflush(stdout);
        */
	rtd = system(buf);
	if ( rtd!=0 )
                {
                strcpy(stringa,"Fallita cancellazione lg3reg* nel direttorio");
                strcat(stringa,work_path);
                ret=7;
		goto FINE;
                }

        exit(0);

FINE:	
        XlOpenFileErrComp(file_err);
        XlErrComp(NULL,"gen_stazionario","ERRORE",stringa);
        XlErrComp(NULL,"gen_stazionario","ERRORE",
                  "I files lg3reg.f lg3reg.o lg3reg non vengono rimossi");
        XlSetFileErrComp(NULL);

        exit(ret);

}
/* Fine file */
#endif
