/**********************************************************************
*
*       C Source:               conv_l.c
*       Subsystem:              2
*       Description:
*       %created_by:    famgr %
*       %date_created:  Wed Apr 10 10:34:14 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: conv_l.c,2 %  (%full_filespec: 2,csrc,conv_l.c,2 %)";
#endif
/*
   modulo conv_l.c
   tipo 
   release 1.3
   data 3/20/95
   reserved @(#)conv_l.c	1.3
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)conv_l.c	1.3\t3/20/95";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "util.h"

void skip_header(FILE *);
int legge_riga(FILE *, char *);
void legge_icona(FILE *, char *);
void legge_sfondo(FILE *, char *);


char nome_file_input[100];
char path_ico[255];
char path_pag[255];

FILE *fp_input;

int main(argc,argv)
int argc;
char **argv;
{
char riga[MAXRIGA];


if(argc!=4)
	exit(fprintf(stderr,"usa: conv_l <nome_file> <path_pag> <path_ico>\n"));
sprintf(nome_file_input,"%s.dxf",argv[1]);
strcpy (path_pag,argv[2]);
strcpy (path_ico,argv[3]);
printf("Converto il file <%s>\n",nome_file_input);

if((fp_input=fopen(nome_file_input,"r"))<=0)
	exit(fprintf(stderr,"il file %s.dxf non esiste\n"));

skip_header(fp_input);

/*
	skip 4 righe
*/
legge_riga(fp_input,riga);
legge_riga(fp_input,riga);
legge_riga(fp_input,riga);
legge_riga(fp_input,riga);

/*
	lettura icone
*/
while (legge_riga(fp_input,riga)==1)
      {
      if(strcmp(riga,"ICON")==0)
	{
	legge_riga(fp_input,riga);
	legge_riga(fp_input,riga);
	legge_icona(fp_input,riga);
	}
      if(strcmp(riga,"ENDSEC")==0)
	break;
      }

legge_sfondo(fp_input,argv[1]);

/*
	lettura entities
*/
/*
	MOMENTANEAMENTE


while (legge_riga(fp_input,riga)==1)
      {
	printf("%s\n",riga);
      }
*********************************************/

fclose(fp_input);
}
