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
   modulo forme.c
   tipo 
   release 1.4
   data 3/20/95
   reserved @(#)forme.c	1.4
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)forme.c	1.4\t3/20/95";
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

#define errore(stringa)         exit(fprintf(stderr,"Attention !!!\n      Error in process forme[%d]: %s\n",getpid(),stringa))


char *name_file_form( FILE * );
int read_appo_file( FILE *, char **, int * );
int write_file_form( int, int, char *, char *, char * );
int write_var(FILE *, char *, char *, char *, char *, int );
int intestazioni(FILE *, int , char *);



int main(argc,argv)
int argc;
char **argv;
{
char *buffer_var=NULL;
char *buffer_dat=NULL;
char file_var[20];
char file_dat[20];
char file_frm[20];
int ret;
int linee_var;
int linee_dat;
FILE *fv;
FILE *fd;



    strcpy(file_var,"APPOVAR");
    strcpy(file_dat,"APPODATI");

    if( !(fv = fopen(file_var,"r")) )
       errore("Non si puo' aprire il file APPOVAR");
    if( !(fd = fopen(file_dat,"r")) )
       errore("Non si puo' aprire il file APPODATI");

    strcpy(file_frm,name_file_form(fv));

/*  Lettura dei file */
    ret = read_appo_file(fv,&buffer_var,&linee_var);
    ret = read_appo_file(fd,&buffer_dat,&linee_dat);

    fclose(fv);
    fclose(fd);

/*  Costruzione del file delle forms */
    ret = write_file_form(linee_var,linee_dat,buffer_var,
                          buffer_dat,file_frm);

    if(buffer_var!=NULL)
       free(buffer_var);
    if(buffer_dat!=NULL)
       free(buffer_dat);

    exit(ret);
}




/*
    Determina il nome del file delle forms a partire
    dal file APPOVAR. 
*/
char *name_file_form(FILE *fp)
{
static char nome[20];
int ret;
int i;

    ret=fscanf(fp,"%s",nome);
    if (ret<=0)
       errore("Non si puo' determinare il nome del file delle forms");
    sprintf(&nome[strlen(nome)],"%s\00",ESTENSIONE);
    for(i=0;i<strlen(nome);i++)
       nome[i] = tolower(nome[i]);
    fseek(fp,0,0);
    return(nome);
}




/*
    Legge un il file (APPOVAR o APPODATI) carattere per 
    carattere inserendo le linee costruite in un buffer.
*/
int read_appo_file(FILE *fp, char **buf, int *nl)
{
char linea[300];
char car;
char *punt=NULL;
int size=0;
int size_wr=0;
int size_buf=1000;
int new_size=size_buf;
int num_linee=0;
int riallocazioni=0;
int ret;
int i,j;
int lun;


    i=0;
    j=0;
    (*buf) = (char*)calloc(size_buf,sizeof(char));
    punt = (*buf);
/*  Lettura intero file carattere per carattere con eliminazione dei NULL */
    while( (ret=fscanf(fp,"%c",&car))!= -1 )
       {
       if( car != NULL )
          {
          linea[j] = car;
          j++;
          }
       i++;
/*     Riconoscimento di una riga */
       if( car == '\n')
          {
          num_linee++;
          linea[j+1] = '\0';
          lun = j;
          /*printf("#%d %d#%s|",strlen(linea),j,linea);*/
          i=0;
          j=0;
          size += sizeof(int) + lun;
/*        Eventuali riallocazioni del buffer */
          if( size>new_size )
            {
            riallocazioni++;
            new_size = (size/size_buf + 1) * size_buf;
            (*buf) = realloc((*buf),new_size);
            punt = (*buf) + size_wr;
            }
          size_wr = size;
/*        Scrittura su file della linea costruita e della sua lunghezza */
          memcpy(punt,&lun,sizeof(int));
          punt += sizeof(int);
          memcpy(punt,linea,lun);
          /*printf("#%d#%s",lun,linea);*/
          punt += lun;
          memset(linea,'\000',300);
          }
       }
       if (riallocazioni != 0)
          printf("riallocazioni=%d num_linee=%d\n",riallocazioni,num_linee);
       (*nl) = num_linee;
       return(1);
}




/*
    Scrive il file delle forms a partire dai buffer
    dei file APPODATI e APPOVAR letti.
*/
int write_file_form(int nl_var,int nl_dat, char *buf_var, 
                    char *buf_dat, char *filefrm)
{
FILE *ff;
int i,j;
int lun;
int ndati;
char linea[300];
char descr[300];
char label[20];
char value[20];
char num_lines[6];
char *punt;
int pos_ndat;
int pos_fine;



    if( !(ff = fopen(filefrm,"w")) )
       errore("Non si puo' aprire il file delle forms");

/*  Scrive la parte iniziale del file */
    intestazioni(ff,nl_var,filefrm);

/*  Scrive la parte di file delle forms relativa al file APPOVAR */
    punt = buf_var;
    for(i=0;i<nl_var;i++)
       {
       memset(linea,'\000',300);
       memcpy(&lun,punt,sizeof(int));
       punt += sizeof(int);
       memcpy(linea,punt,lun);
       punt += lun;
       /* printf("%s",linea); */
       if(i==0)
         {
         fprintf(ff,"%s%d%s     %s\n",UNOP,i+1,TYPE,DESCRIZIONE);
         fprintf(ff,"%s%d%s    %s",UNOP,i+1,DESCR,linea);
         }
       else
         {
         strncpy(label,linea,10);
         label[10] = '\0';
         strcpy(descr,&linea[11]);
         write_var(ff,UNOP,label,ZERO,descr,i);
         }
       }

/*  Scrive la parte di file delle forms relativa al file APPODATI */
    punt = buf_dat;
    fprintf(ff,"%s\n",TITLD);
    fprintf(ff,"%s%s   ",DUEP,NUMEL);
    pos_ndat = ftell(ff);
    strcpy(num_lines,"    ");
    fprintf(ff,"%s\n",num_lines);

    strcpy(descr,"\"\"\n");

    ndati = 0;
    j = 0;
    for(i=0;i<nl_dat;i++)
       {
       memset(linea,'\000',300);
       memcpy(&lun,punt,sizeof(int));
       punt += sizeof(int);
       memcpy(linea,punt,lun);
       punt += lun;
       /* printf("#%d#%s",lun,linea); */
       if( !strncmp(linea,DESCDATI,strlen(DESCDATI)) )
           {
/*         Riga di commento */
           fprintf(ff,"%s%d%s     %s\n",DUEP,j+1,TYPE,DESCRIZIONE);
           fprintf(ff,"%s%d%s    %s",DUEP,j+1,DESCR,linea);
           j++;
           }
       else
           {
/*         Riga con uno, due o tre dati */
           if( linea[OFF_END_DATO1] == '*' &&
               strlen(linea) >= (OFF_END_DATO1+1) )
               {
               strncpy(value,&linea[OFF_INI_DATO1],10);
               strncpy(label,&linea[OFF_INI_DESC1],8);
               label[8]   = '\0';
               value[10] = '\0';
               write_var(ff,DUEP,label,value,descr,j);
               ndati++;
               j++;
               }
           if( linea[OFF_END_DATO2] == '*' &&
               strlen(linea) >= (OFF_END_DATO2+1) )
               {
               strncpy(value,&linea[OFF_INI_DATO2],10);
               strncpy(label,&linea[OFF_INI_DESC2],8);
               label[8]   = '\0';
               value[10] = '\0';
               write_var(ff,DUEP,label,value,descr,j);
               ndati++;
               j++;
               }
           if( linea[OFF_END_DATO3] == '*' &&
               strlen(linea) >= (OFF_END_DATO3+1) )
               {
               strncpy(value,&linea[OFF_INI_DATO3],10);
               strncpy(label,&linea[OFF_INI_DESC3],8);
               label[8]   = '\0';
               value[10] = '\0';
               write_var(ff,DUEP,label,value,descr,j);
               ndati++;
               j++;
               }
           }
       }
/*  Scrittura del numero delle righe per la form dei dati */
    pos_fine = ftell(ff);
    fseek(ff,pos_ndat,0);
    sprintf(num_lines,"%d",j);
    fprintf(ff,"%s",num_lines);

    fclose(ff);
    printf("Scritto il file <%s> : %d linee form variabili e %d dati\n",
            filefrm,nl_var,ndati);
    return(1);
}




/*
    Scrive sul file delle forms i quattro campi descrittivi
    delle variabili.
*/
int write_var(FILE *fp, char *form, char *label_var, char *value_var, 
              char *descr_var, int num)
{
int i;

    for(i=0;i<strlen(value_var);i++)
       {
       if(value_var[i] != ' ')
           break;
       }
    if ( i==strlen(value_var) )
       strcpy(value_var,BLANK);
    fprintf(fp,"%s%d%s     %s\n",form,num+1,TYPE,INPUT);
    fprintf(fp,"%s%d%s    %s\n",form,num+1,LABEL,label_var);
    fprintf(fp,"%s%d%s   %s\n",form,num+1,FORMAT,FLOAT);
    fprintf(fp,"%s%d%s    %s\n",form,num+1,VALUE,value_var);
    fprintf(fp,"%s%d%s    %s",form,num+1,DESCR,descr_var);
}




/*
    Scrive le prime righe del file delle forms.
*/
int intestazioni(FILE *fp, int n_line_var, char *file)
{
     fprintf(fp,"%s     %s\n",NAME,file);
     fprintf(fp,"%s     %s\n",NUME,"2");
     fprintf(fp,"%s\n",TITLV);
     fprintf(fp,"%s%s   %d\n",UNOP,NUMEL,n_line_var);
}
/* Fine file */
