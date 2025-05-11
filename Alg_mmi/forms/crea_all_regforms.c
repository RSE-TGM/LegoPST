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
   modulo crea_all_regforms.c
   tipo 
   release 1.4
   data 3/20/95
   reserved @(#)crea_all_regforms.c	1.4
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)crea_all_regforms.c	1.4\t3/20/95";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <forme.h>


#define errore(stringa)         exit(fprintf(stderr,"Attention !!!\n      Error in process crea_all_regforms[%d]: %s\n",getpid(),stringa))


int numero_moduli=0;
int numero_files=0;


int main(argc,argv)
int argc;
char **argv;
{
FILE *fl;
int ret;
int forms;
int j,i,lun;
char nome[20];
char linea[200];
char comando[200];
char regod[200];
char regov[200];
char car;


    printf("CREA_ALL_REGFORMS\n");

/*  Lettura della lista_moduli_reg.dat */
    if( !(fl = fopen("lista_moduli_reg.dat","r")) )
       errore("Non si puo' aprire il file lista_moduli_reg.dat");

    i=0;
    j=0;
    while( (ret=fscanf(fl,"%c",&car)) != -1 )
       {
       if( car != NULL )
          {
          linea[j] = car;
          j++;
          }
       i++;
       if( car == '\n')
          {
          numero_moduli++;
          linea[j+1] = '\0';
          lun = j;
          strncpy(nome,linea,4);
          nome[5] = '\0';
          j = 0;
          memset(linea,'\x00',200);
/*        Si lancia regoserver per creare i file APPODATI e APPOVAR */
          sprintf(regod,"%s %d %s\00","regoserver",numero_moduli,"D");
          sprintf(regov,"%s %d %s\00","regoserver",numero_moduli,"V");
          ret = 0;
          ret = system(regod);
/*        Controllo dell'esecuzione di regoserver */
          if( W_EXITSTATUS(ret) != 2 )
             {
             printf("RET=%d RET=%d\n",ret,W_EXITSTATUS(ret));
             errore("crea_all_regforms : Non si puo' lanciare regoserver");
             }
          system(regov);
/*        Si lancia regoserver per creare il file delle forms */
          forms = system("forme");
          if ( W_EXITSTATUS(forms)==1 )
             numero_files++;
          else
             printf("forms=%d\n",W_EXITSTATUS(forms));
          }
       }
    fclose(fl);
    if (numero_files != numero_moduli )
       printf("ATTENZIONE File in lista moduli=%d  files generati=%d\n",
               numero_moduli,numero_files);
    else
       printf("File generati=%d (quanti i  moduletti) \n",numero_moduli);
}
/* Fine file */
