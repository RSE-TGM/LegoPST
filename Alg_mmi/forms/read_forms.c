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
   modulo read_forms.c
   tipo 
   release 1.4
   data 3/20/95
   reserved @(#)read_forms.c	1.4
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)read_forms.c	1.4\t3/20/95";
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
#include <sys/wait.h>
#include <forme.h>

#define errore(stringa)         exit(fprintf(stderr,"Attention !!!\n      Error in process read_forms[%d]: %s\n",getpid(),stringa))
#define stampas(s1,s2,s3)        fprintf(stderr,"%s|%s|%s|\n",s1,s2,s3)
#define stampai(i1,i2,i3)        fprintf(stderr,"%d|%d|%d|\n",i1,i2,i3)


/*
 tipi di linea (per line_type)
*/
#define LINE_INP        0
#define LINE_DESCR      1
#define MAX_L_DESCR   100
#define MAX_L_TAGS     15

typedef struct _linea_input {
                char tag[MAX_L_TAGS];
                char format[MAX_L_TAGS];
                char value[MAX_L_TAGS];
                char descr[MAX_L_DESCR];
                } LINEA_INPUT;

typedef struct _linea_descr {
                char descr[MAX_L_DESCR];
                } LINEA_DESCR;

typedef union {  LINEA_INPUT input;
                 LINEA_DESCR descr;
                } UNION_LINE;

typedef struct _descr_line {
                int line_type;
                UNION_LINE u;
                } DESCR_LINE;

typedef struct _descr_forms {
                int num_lines; /* numero di linee costituenti la form */
                DESCR_LINE *pdescr_line; /* descrittori di linea */
                } DESCR_FORMS;

typedef struct {
        /* risorse  */
        char *path_name;      /* pathname del file di forms             */
        int num_forms;        /* numero delle forms */
        DESCR_FORMS *pdescr_forms;  /* vettore dei descrittori delle singole
                                       forms  */
        } OlFormPart;

/*
 Dichiarazione completa del record per instanziazione
 */
typedef struct _OlFormRec {
        OlFormPart olform;
        } OlFormRec;
typedef struct _OlFormRec *OlFormObject;

int legge_form(OlFormObject, int , int );
int r_line_d(OlFormObject, int , int );
int r_line_i(OlFormObject, int , int );

FILE *ff;
OlFormPart Frm;
OlFormRec *form;
OlFormObject newform=NULL;

static int GetNumForms();
static int GetNumLines();
static int GetNumTags();
static char * GetTags();
static char *WriteStrInp();
static int Read_666();

int main(argc,argv)
int argc;
char **argv;
{
char *buffer=NULL;
char nome_file_form[10];
int i,j,k;
int num_forms;
char ch;

/*  Apertura del file .frm */
    strcpy(nome_file_form,"som4.frm");
    if( !(ff=fopen(nome_file_form,"r")) )
      errore("Non si puo' aprire il file");


/*  Allocazione della form */
    newform=(OlFormRec*) malloc (sizeof (OlFormRec));
    if(newform==NULL)
        {
        printf("OlForm OlCreateForm Impossibile allocare form in memoria\n");
        return(0);
        }

/*   Allocazione e riempimento risorsa path-name della form */
     newform->olform.path_name=(char*)malloc (strlen(nome_file_form)+1);
     strcpy(newform->olform.path_name,nome_file_form);

/*   Lettura del file .FRM e cariacamento in memoria */
     Read_666(newform);

/*   Lettura del file .FRM dalla memoria */
     num_forms = GetNumForms(newform);
     printf("GetNumForms=%d\n",num_forms);

     for(i=0;i<num_forms;i++)
        {
        printf("******************forma=%d**********************\n",i);
        j = GetNumTags(newform, i);
        printf("GetNumLines=%d GetNumTags=%d\n",GetNumLines(newform,i),j);
        buffer = (char*)GetTags(newform,i);
        printf("**************************\n");
        memcpy(&k,buffer,sizeof(int));
        printf("---->%d|\n",k);
        buffer += sizeof(int);
        for(k=0;k<j;k++)
           {
           printf("%s|\n",buffer);
           buffer += MAX_L_TAGS;
           }
        if(buffer!=NULL)
          free(buffer);
        printf("**************************\n");
        }
     for(i=0;i<num_forms;i++)
        {
        WriteStrInp(newform,buffer,i);
        printf("%s|\n",buffer);
        if(buffer!=NULL)
          free(buffer);
        }
}




/*
        Ritorna il numero di forms
        descritte nel file .FRM
        Ritorna 0 se errore o nessuna form presente.
*/
static int GetNumForms(OlFormObject form)
{
if( form!=NULL )
        return(form->olform.num_forms);
else
        return(0);
}



/*
        Ritorna il numero di linee di una determinata form
        ritorna 0 se errore o nessuna linea nella form.
*/
static int GetNumLines(OlFormObject form,int ind)
{
/*
 Verifica la correttezza dei parametri passati NOO!!
*/
        return(form->olform.pdescr_forms[ind].num_lines);
}


/*
 Ricava il numero di tags presenti nella lista
*/
static int GetNumTags(OlFormObject form,int ind)
{
int num_lines,num_tags,i;

num_tags=0;
num_lines=GetNumLines(form,ind);

for(i=0; i< num_lines; i++)
        {
        if(form->olform.pdescr_forms[ind].pdescr_line[i].line_type ==
            LINE_INP)
                num_tags++;
        }
return(num_tags);
}



/*
 Ricava l'elenco delle tags della form di indice ind
 Valore di ritorno       --> Un elenco contenente in testa un intero
                                        con il numero delle tags presenti,
                                        quindi una lista di nomi di tags
                                        terminata da un elemento a NULL;
                                        Ciascuna tag e' dimensionata a
                                        MAX_LUN_FORMTAG ( 80 char. )
*/
static char * GetTags(OlFormObject form,int ind)
{
#define MAX_LUN_FORMTAG 80
int i;
int j;
int size;
int ntags;
int nline;
char *ret =NULL;
char *punt=NULL;

        nline = GetNumLines(form,ind);
        ntags = GetNumTags(form, ind);
        size = sizeof(int) + ntags*MAX_L_TAGS*sizeof(char) + sizeof(char);
        ret = (char*)malloc(size);
        punt = ret;

        memcpy(punt,&ntags,sizeof(int));
        punt += sizeof(int);
        for(i=0;i<nline;i++)
           {
           if(form->olform.pdescr_forms[ind].pdescr_line[i].line_type ==
              LINE_INP)
              {
              memcpy(punt,
                form->olform.pdescr_forms[ind].pdescr_line[i].u.input.tag,
                MAX_L_TAGS);
              punt = punt + MAX_L_TAGS;
              }
           }
        memcpy(punt,'\00',sizeof(char));
        return(ret);
}


/*
 WriteStrInp
 Produce una stringa ottenuta riportando in forma ascii, ordinatamente, inclusi
 tra virgolette singole e tra loro separati da uno o piu' spazi i valori contenu
 ti nella form.
 Parametri:
 form    oggetto form
 istrinp stringa dei valori in input; viene allocata internamente alla
         funzione (dovra' essere liberata dall'utente).
 ind     indice della form.
*/
static char *WriteStrInp(OlFormObject form, char *strinp, int ind)
{
/* ATTENZIONE, puntatore di puntatore */
int i;
int j;
int size;
int ntags;
int nline;
char *ret =NULL;
char *punt=NULL;
char *str[200];


        nline = GetNumLines(form,ind);
        ntags = GetNumTags(form, ind);
        size = ntags*(MAX_L_TAGS + MAX_L_DESCR);
        strinp = (char*)malloc(size);

        strinp[0] = '\x27';
        for(i=0;i<nline;i++)
           {
           if(form->olform.pdescr_forms[ind].pdescr_line[i].line_type ==
              LINE_INP)
              {
              strcat(strinp,
                form->olform.pdescr_forms[ind].pdescr_line[i].u.input.value);
              strcat(strinp,APICI);
              strcat(strinp,
                form->olform.pdescr_forms[ind].pdescr_line[i].u.input.descr);
              strcat(strinp,APICI);
              }
           }
         strinp[strlen(strinp)-2] = '\00';
}




static int Read_666(OlFormObject form)
{
int i;
int ret;
char str1[100];
char str2[100];
char str3[100];
int cont;

/*  Lettura numero di forms */
    fscanf(ff,"%*s%*s%*s%d",&form->olform.num_forms);

    form->olform.pdescr_forms = (DESCR_FORMS*)
                         calloc(form->olform.num_forms,sizeof(DESCR_FORMS));

/*  Lettura del numero di linee della prima form */
    fscanf(ff,"%*s%*s%*s%d",&cont);
    form->olform.pdescr_forms[LINE_INP].num_lines = cont;
    form->olform.pdescr_forms[LINE_INP].pdescr_line = 
               (DESCR_LINE*)calloc(cont,sizeof(DESCR_LINE));

/*  Legge le righe della prima forms */
    printf("Read=%s\n",form->olform.path_name);
    legge_form(form, cont, LINE_INP);

/*  Lettura del numero di linee della seconda form */
    fscanf(ff,"%*[\n]  %*[^\n]%*[\n] %*s %d",&cont);
    form->olform.pdescr_forms[LINE_DESCR].num_lines = cont;
    form->olform.pdescr_forms[LINE_DESCR].pdescr_line = 
               (DESCR_LINE*)calloc(cont,sizeof(DESCR_LINE));

/*  Legge le righe della prima forms */
    legge_form(form, cont, LINE_DESCR);

    fclose(ff);
}




/*
    In funzione del numero di righe della form identificata
    da tipo form legge le specifiche dell'intera form.
*/
int legge_form(OlFormObject form, int num_lines, int tipo_form)
{
int i;
char s1[100];
char s2[100];

/*  Ciclo sulle linee della prima forms */
    for(i=0;i<num_lines;i++)
       {
       fscanf(ff,"%s%s",s1,s2);
       if( strstr(s2,DESCRIZIONE) )
           {
           form->olform.pdescr_forms[tipo_form].pdescr_line[i].line_type =
                                                                 LINE_DESCR;
           r_line_d(form, tipo_form,i);
           }
       else
          if ( strstr(s2,INPUT) )
              {
              form->olform.pdescr_forms[tipo_form].pdescr_line[i].line_type =
                                                                 LINE_INP;
              r_line_i(form, tipo_form,i);
              }
           else
              errore("Riga non riconosciuta");
       }
}





/*
    Legge i campi label, format, value, descr della riga 
    di tipo input della form.
*/
int r_line_i(OlFormObject form, int n_frm, int n_lns)
{

     fscanf(ff,"%*s%*[ ]%[^\n]  %*s%s  %*s%s  %*s%*[ ]%[^\n]",
            form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.input.tag,
            form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.input.format,
            form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.input.value,
            form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.input.descr);

     if ( !strcmp(BLANK,
           form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.input.value) )
         {
         strcpy(
           form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.input.value,
                " \00");
         }
     if ( !strcmp("\"\"",
           form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.input.descr) )
         {
         strcpy(
           form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.input.descr,
                "\00");
         }
     printf("%s|\n%s|\n%s|\n%s|\n",
            form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.input.tag,
            form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.input.format,
            form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.input.value,
            form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.input.descr);
     printf("=================\n");
}




/*
    Legge il campo descr della riga di tipo descr della form.
*/
int r_line_d(OlFormObject form, int n_frm, int n_lns)
{
int i;
char s1[100];
char s2[100];
char s3[100];

     fscanf(ff,"%*s%*[ ]%[^\n]",
            form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.descr.descr);
     printf("%s|\n",
            form->olform.pdescr_forms[n_frm].pdescr_line[n_lns].u.descr.descr);
}
/* Fine file */
