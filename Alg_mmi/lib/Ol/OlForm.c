/**********************************************************************
*
*       C Source:               OlForm.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Wed Oct 23 15:25:05 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: OlForm.c-3 %  (%full_filespec: OlForm.c-3:csrc:1 %)";
#endif
/*
   modulo OlForm.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)OlForm.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)OlForm.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*
 *  OlForm.c 
 *  Oggetto per la memorizzazione di strutture del tipo Form.
 *  Tali strutture vengono utilizzate per memorizzare le informazioni 
 *  relative a forms di input per l'utente.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Ol/OlFormP.h>

#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <sys/shm.h>
# include <sys/stat.h>
#endif
#if defined VMS
# include <stat.h>
# include "vmsipc.h"
#endif
# include <forme.h>
#include "utile.h"

/* dichiarazioni funzioni varie */
static Boolean IsFormAllocated(); /* verifica se la form e' allocata */
static Boolean IsIndexValid();    /* verifica se la form e' allocata e se
				    l'indice della form corrisponde ad una
				    form esistente */
int legge_form(OlFormObject, FILE * , int , int );
int r_line_d(OlFormObject, FILE * , int , int );
int r_line_i(OlFormObject, FILE * , int , int );
int indice(OlFormObject , int , int );
int  leggi_campo(FILE *, char *, int, int *, char *, int);
int lettura_forms(OlFormObject , int, FILE *, int , int , int, int *, char * );
         
/* dichiarazione dei metodi */
static Boolean Initialize();
static Boolean Read_File();
static int Write();
static int GetNumForms();
static int GetNumLines();
static int GetNumTags();
static char * GetTags();
static char *GetValForm();
static Boolean SetValForm();
static Boolean ReadStrInp();
static char *WriteStrInp();
static char *GetTitle();
static DESCR_LINE *GetLine();
static char *GetDescr();

/* Inizializzazione del class record */
OlFormClassRec olFormClassRec = {
  { /* form fields */
    /* initialize               */      Initialize,
    /* read                     */      Read_File,
    /* write                    */      Write,
    /* get_num_forms            */      GetNumForms,
    /* get_num_lines            */      GetNumLines,
    /* get_num_tags             */      GetNumTags,
    /* get_tags                 */      GetTags,
    /* read_str_inp             */      ReadStrInp,
    /* write_str_inp            */      WriteStrInp,
    /* get_val_form		*/      GetValForm,
    /* set_val_form             */      SetValForm,
    /* get_title                */      GetTitle,
    /* get_descr                */      GetDescr,
    /* get_line                 */      GetLine
  }
};


ObjectClass olFormObjectClass = (ObjectClass) &olFormClassRec;

/*
	Metodo di inizializzazioene dell'oggetto form
*/
static Boolean Initialize(OlFormObject form) 
{
return(True);
}



/*
 Write (per ora non serve)
 Scrive il file .FRM in base al contenuto delle strutture dell'oggetto
 form. Restituisce il numero di bytes scritti.
*/
static int Write(OlFormObject form)
{
}


/*
 OlCreateForm
 Creazione oggetto form
*/
OlFormObject OlCreateForm(char *nome_file_form) 
{
OlFormObject newform=NULL;

   newform=(OlFormRec*) XtMalloc (sizeof (OlFormRec));
   if(newform==NULL)
	{
	XlWarning("OlForm","OlCreateForm",
		"Impossibile allocare form in memoria\n");
	return(NULL);
	}

/* Allocazione e riempimento risorsa path-name della form */
   newform->olform.path_name=(char*)XtMalloc (strlen(nome_file_form)+1);
   strcpy(newform->olform.path_name,nome_file_form);

/* Lettura del file .FRM e cariacamento in memoria */
   if((olFormClassRec.olform_class.read)(newform))
	{
/*      Inizializzazione del database */
	if((olFormClassRec.olform_class.initialize)(newform))
		return(newform);
	else
		{
		XlWarning("OlForm","OlDestroyForm",
			"form in memoria non allocato");
		XtFree((char *)newform);
		return(NULL);
		}
	}
else
   return(NULL);
}




/*
 OlGetNumForms
 Restituisce il numero delle form dell'oggetto.
*/
int OlGetNumForms(OlFormObject form)
{
return(olFormClassRec.olform_class.get_num_forms(form));
}


/*
 OlGetNumLines
 Restituisce il numero delle linee della form -ind- 
 dell'oggetto.
*/
int OlGetNumLines(OlFormObject form, int ind)
{
return(olFormClassRec.olform_class.get_num_lines(form,ind));
}



/*
 OlGetNumTags
 Restituisce il numero delle tags della form -ind- 
 dell'oggetto.
*/
int OlGetNumTags(OlFormObject form, int ind)
{
return(olFormClassRec.olform_class.get_num_tags(form,ind));
}


/*
 OlGetTags
 Restituisce le tags della form -ind- dell'oggetto.
*/
char *OlGetTags(OlFormObject form, int ind)
{
return(olFormClassRec.olform_class.get_tags(form,ind));
}


/*
 OlGetLineForm
 Restituisce le tags della form -ind- dell'oggetto.
*/
DESCR_LINE *OlGetLineForm(OlFormObject form, int ind, int num_line)
{
return(olFormClassRec.olform_class.get_line(form,ind,num_line));
}

/*
 OlWriteStrInp
 Ritorna in una stringa valori e descrizioni (fra apici) consecutivamente.
*/
char *OlWriteStrInp(OlFormObject form, char **str, int ind)
{
  return(olFormClassRec.olform_class.write_str_inp(form,str,ind));
}


/*
 OlReadStrInp
 Legge una stringa in input nel formato 'valore' 'descrizione' e la
 utilizza per assegnare i valori ai campi di input utente della form.
*/
char *OlReadStrInp(OlFormObject form, char *str, int ind)
{
/*return(olFormClassRec.olform_class.write_str_inp(form,str,ind));*/
  olFormClassRec.olform_class.read_str_inp(form,str,ind);
  return(str);
}


/*
 OlGetValForm
 Ritorna il campo valore della tag specificata della form scelta.
*/
char *OlGetValForm(OlFormObject form, char *tag, int ind, int num_car)
{
  return(olFormClassRec.olform_class.get_val_form(form,ind,tag,num_car));
}

/*
 OlSetValForm
 Assegna il campo valore della tag specificata nella form scelta.
*/
Boolean OlSetValForm(OlFormObject form, char *tag, int ind, int num_car, 
                     char *val)
{
  return(olFormClassRec.olform_class.set_val_form(form,ind,tag,num_car,val));
}


/*
 OlGetTitle
 Ritorna il titolo della form specificata.
*/
char *OlGetTitle(OlFormObject form, int ind)
{
char *punt;

  return(olFormClassRec.olform_class.get_title(form,ind));
}


/*
 OlGetDescr
 Ritorna un intero seguito da tutte le descrizioni delle tags
*/
char *OlGetDescr(OlFormObject form, int ind)
{
char *punt;

  return(olFormClassRec.olform_class.get_descr(form,ind));
}



/*
	Ritorna il numero di forms 
        descritte nel file .FRM
	Ritorna 0 se errore o nessuna form presente.
*/
static int GetNumForms(OlFormObject form)
{

if(IsFormAllocated(form))
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

/* Verifica la correttezza dei parametri passati */
if(IsIndexValid(form,ind))
	return(form->olform.pdescr_forms[ind].num_lines);
else
	return(0);
}



/*
 Ricava il numero di tags presenti nella lista
*/
static int GetNumTags(OlFormObject form,int ind)
{
int num_lines,num_tags,i;

num_tags=0;
num_lines=GetNumLines(form,ind);

if(IsIndexValid(form,ind))
   {    
   for(i=0; i< num_lines; i++)
	{
	if(form->olform.pdescr_forms[ind].pdescr_line[i].line_type == LINE_INP)
		num_tags++;
	}
   return(num_tags);
   }
else
   return(0);
}


/*
 Alloca e restituisce l'elenco delle tags della form di indice ind
 secondo il formato specificato.
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

        if( IsIndexValid(form,ind)==False )
           {
           printf("GetTags : indice non valido\n");
           return(NULL);
           }
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
/*
        memcpy(punt,'\00',sizeof(char));
*/
        (*punt) = '\0';
        return(ret);
}

		
/*
 ReadStrInp
 Legge una stringa in input e la utilizza per assegnare i valori ai campi
 di input utente della form.
 La stringa in input deve riportare ordinatamente tutti i valori da assegnare 
 alle form considerate; ogni singolo valore compare incluso tra virgolette
 singole. Ogni valore e' separato da uno o piu' spazi dal successivo.
 Es:
 '123.45' 'descrizione editabile primo valore' '156.78' 'desc. secondo val.' ...
 Parametri:
 form     oggetto form.
 strinp   stringa fornita dal chiamante che contiene i valori da leggere nel
          formato sopra descritto.
 ind      indice della form.

*/
static Boolean ReadStrInp(OlFormObject form, char *strinp_inp, int ind)
{
char *punt;
int k;        /* Indice degli elementi della stringa da leggere, blank compresi                  es. 123.45 (k=0),   descrizione ... (k=1) */
int i=0;      /* Indice della coppia valore-descrizione da leggere */
int j=0;      /* Posizione della coppia i nel vettore di num_lines elementi */
char stringa[10];
char *strinp;


/*
 alloca un buffer di appoggio con la copia del buffer
 in input
*/
        strinp=XtNewString(strinp_inp);

        if(IsIndexValid(form,ind)==False)
           {
           printf("ReadStrInp : indice non valido\n",ind);
           return(False);
           }
/*      Lettura della prima coppia di elementi valore-descrizione */
        punt = strtok(strinp,"|");
        j = indice(form,ind,i);
        strcpy(form->olform.pdescr_forms[ind].pdescr_line[j].u.input.value,
               "666"/*punt*/);
        punt = strtok(NULL,"|");
        punt = strtok(NULL,"|");
        strcpy(form->olform.pdescr_forms[ind].pdescr_line[j].u.input.descr,
               punt);
/*
        Letto il primo elemento (i=1) e il blank successivo (k=3) si leggono
        i successivi campi valore (per i quali k e' divisibile per 4) e
        descrizione
*/
        k=3;
        i=1;
        j = indice(form,ind,i);
        while(punt!=NULL)
           {
           punt = strtok(NULL,"|");
           if( (k%2)==0 )
             {
             if( (k%4)==0 )
               {
               sprintf(stringa,"%d\00",k);
               strcpy(
                  form->olform.pdescr_forms[ind].pdescr_line[j].u.input.value,
                  stringa/*punt*/);
               }
             else
               {
               strcpy(
                  form->olform.pdescr_forms[ind].pdescr_line[j].u.input.descr,
                   punt);
               j = indice(form,ind,++i);
               }
             }
           k++;
           }
        XtFree((char *)strinp);     /* libera il buffer di appoggio */

        return(True);
}




/*
    In funzione del numero della riga di input restituisce la posizione
    nel vettore lungo num_lines occupata nella struttura (tipo UNION_LINE) 
    relatuva alla form ind dell'oggetto form.
*/
int indice(OlFormObject form, int ind, int num)
{
int ret;
int k=0;
int i=0;

/*
      while(i<num)
         {
         while(form->olform.pdescr_forms[ind].pdescr_line[k].line_type ==
               LINE_DESCR)
               {
               k++;
               }
         i++;
         } 
      return(i+k);
*/
      while(1)
         {
         while(form->olform.pdescr_forms[ind].pdescr_line[k].line_type ==
               LINE_DESCR)
               {
               k++;
               }
         if(i==num)
            break;
         i++;
         } 
      return(i+k);


}

/*
 WriteStrInp 
 Alloca e ritorna tra i parametri una stringa ottenuta riportando in forma
 ascii, ordinatamente, inclusi tra virgolette singole e tra loro separati da
 uno o piu' spazi i valori contenuti nella form.
 Lo stesso lo ritorna.
 Parametri:
 form    oggetto form
 strinp  stringa dei valori in input; viene allocata internamente alla
         funzione (dovra' essere liberata dall'utente).
 ind     indice della form.
*/
static char *WriteStrInp(OlFormObject form, char **strinp, int ind)
{
int i;
int j;
int size;
int ntags;
int nline;
char *ret =NULL;
char *punt=NULL;

        if(IsIndexValid(form,ind)==False)
            {
            printf("WriteStrInp : indice non valido=%d\n",ind);
            return(NULL);
            }
        nline = GetNumLines(form,ind);
        ntags = GetNumTags(form, ind);
        if( ntags==0 )
           return(NULL);
        size = ntags*(MAX_L_TAGS + MAX_L_DESCR + 2*strlen(SEPARATORE) ) + 3;
        (*strinp) = (char*)calloc(size,sizeof(char));

        (*strinp)[0] = '|';
        for(i=0;i<nline;i++)
           {
           if(form->olform.pdescr_forms[ind].pdescr_line[i].line_type ==
              LINE_INP)
              {
              strcat((*strinp),
                form->olform.pdescr_forms[ind].pdescr_line[i].u.input.value);
              strcat((*strinp),SEPARATORE);
              strcat((*strinp),
                form->olform.pdescr_forms[ind].pdescr_line[i].u.input.descr);
              strcat((*strinp),SEPARATORE);
              }
           }
         (*strinp)[strlen((*strinp))-2] = '\0';

         ret = (*strinp);
         return(ret);
}






/*
 GetValForm
 Restituisce il puntatore al campo valore corrispondente ad una 
 determinata tag.
 La etichetta (tag_cercata) passata come parametro alla funzione viene
 utilizzata come chiave di ricerca tra le tag appartenenti alla form.
 Viene ricercata tra le tag della form una tag avente i primi N caratteri
 uguali alla tag_cercata, dove N e' un parametro passato. Questo per 
 prevedere la ricerca in stringhe lunghe di un numero di caratteri inferiore
 a strlen.
*/
static char *GetValForm(OlFormObject form, int ind, char *tag_cercata, 
                        int num_car)
{
int i;
int num_lines;


    if(IsIndexValid(form,ind)== False)
        {
        printf("GetValForm : indice non valido=%d\n",ind);
        return(NULL);
        }
    num_lines = form->olform.pdescr_forms[ind].num_lines;
    for(i=0;i<num_lines;i++)
       {
       if( form->olform.pdescr_forms[ind].pdescr_line[i].line_type ==
           LINE_INP )
          {
          if( !strncmp(
                form->olform.pdescr_forms[ind].pdescr_line[i].u.input.tag,
                tag_cercata,num_car) )
             {
             return(
                 form->olform.pdescr_forms[ind].pdescr_line[i].u.input.value);
             }
          }
       }
    return(NULL);
}




/*
 SetValForm
 Assegna un valore al campo di input corrispondente ad una determinata tag 
 (tag_cercata). 
 La funzione ritorna True se e' stato possibile l'assegnamento ( la tag
 e' stata trovata nella form di indice richiesto ). Il confronto tra le tag
 viene effettuato considerando i primi N caratteri delle tags.
*/
static Boolean SetValForm(OlFormObject form, int ind, char *tag_cercata,
                          int num_car, char *val)
{
int i;
int num_lines;


    if(IsIndexValid(form,ind)== False)
        {
        printf("SetValForm : indice non valido=%d\n",ind);
        return(False);
        }
    num_lines = form->olform.pdescr_forms[ind].num_lines;
    for(i=0;i<num_lines;i++)
       {
       if( form->olform.pdescr_forms[ind].pdescr_line[i].line_type ==
           LINE_INP )
          {
          if( !strncmp(
                form->olform.pdescr_forms[ind].pdescr_line[i].u.input.tag,
                tag_cercata,num_car) )
             {
             strcpy(form->olform.pdescr_forms[ind].pdescr_line[i].u.input.value,
                    val);
             return(True);
             }
          }
       }
    return(False);
}


/*
     Ritorna il titolo della form.
*/
static char *GetTitle(OlFormObject form, int ind)
{

/* Verifica la correttezza dei parametri passati */
if(IsIndexValid(form,ind))
	return(form->olform.pdescr_forms[ind].title);
else
	return(NULL);
}




/*
     Alloca e ritorna un buffer costituito da un intero,numero delle 
     descrizioni della form seguito dall'elenco delle descrizioni.
     Scrive tutte le descrizioni, anche quelle delle variabili di tipo
     tag
*/
static char *GetDescr(OlFormObject form, int ind)
{
int i;
int nline;
int ntags;
int size; 
int ndescr=0;
char *ret;
char *punt;

/* Verifica la correttezza dei parametri passati */
if(IsIndexValid(form,ind)==False)
	return(NULL);

        nline = GetNumLines(form,ind);
        ntags = GetNumTags(form, ind);
        size = sizeof(int) + ntags*MAX_L_DESCR*sizeof(char) + sizeof(char);
        ret = (char*)calloc(size,sizeof(char));
        punt = ret;
/*
        memcpy(punt,&nline,sizeof(int));
*/
        memcpy(punt,&ntags,sizeof(int));
        punt += sizeof(int);
/*      Ciclo di lettura delle descrizioni per tutte le righe della form */
        for(i=0;i<nline;i++)
           {
           if(form->olform.pdescr_forms[ind].pdescr_line[i].line_type ==
              LINE_INP)
              {
              memcpy(punt,
                form->olform.pdescr_forms[ind].pdescr_line[i].u.input.descr,
                MAX_L_DESCR);
              punt += MAX_L_DESCR;
              ndescr++;
              }
           else
              {
              if(form->olform.pdescr_forms[ind].pdescr_line[i].line_type ==
                 LINE_DESCR)
                 {
/*
                 memcpy(punt,
                   form->olform.pdescr_forms[ind].pdescr_line[i].u.descr.descr,
                   MAX_L_DESCR);
                 punt += MAX_L_DESCR;
                 ndescr++;
*/
                 }
              else
                 {
                 fprintf(stderr,
                         "GetDescr:campo line_type della form non corretto\n");
                 return(NULL);
                 }
              }
           }
        if( ntags!=ndescr )
            fprintf(stderr,"GetDescr: numero di linee (%d) diverso dal numero delle descrizioni riconosciute (%d)\n",nline,ndescr);
/*
        memcpy(punt,'\00',sizeof(char));
        punt = ret;
        memcpy(&nline,punt,sizeof(int));
        printf("          #############\n");
        printf("          nline=%d\n",nline);
        punt += sizeof(int);
        for(i=0;i<nline;i++)
            {
            printf("          |%s|\n",punt);
            punt += MAX_L_DESCR;
            }
        printf("          #############\n");
*/
        return(ret);
}


static DESCR_LINE *GetLine(OlFormObject form, int ind, int num_line)
{
if(IsIndexValid(form,ind)== False)
	return(NULL);
if(form->olform.pdescr_forms[ind].num_lines< num_line)
	{
        XlWarning("OlForm","GetLine","num line not valid !");
	return(NULL);
	}
return(&form->olform.pdescr_forms[ind].pdescr_line[num_line]);
}


/*
 Distruzione dell'oggetto form 
*/
Boolean OlDestroyForm(OlFormObject form)
{
if(IsFormAllocated(form))
	{
/*
 Aggiungere la free corrispondente alle allocazioni interne
 eseguite durante il caricamento delle descr_form.
*/
	XtFree((char *)form);
	return(True);
	}
return(False);
}




#ifdef PER_COPIARLA
/********************************************************
	Routine che crea una lista contenente la lista dei modelli
	presenti nel database topologico passato come parametro:

char *OlElencoModelli
	(
	database		--> Database della topologia del simulatore;
	)

	Valore di ritorno	--> Un elenco contenente in testa un intero
					con il numero dei modelli presenti,
					quindi una lista di nomi di modelli 
					dimensionate a MAX_LUN_NOME_MODELLO
					terminata da un elemento a NULL;
**********************************************************/
char *OlElencoModelli (OlDatabaseTopologiaObject database)
{
char *appoggio,*p,*ind,*modelloappoggio;
int numeroModelli;
int i;

/*  Ricavo il numero totale di modelli  */
numeroModelli = NumModelli (database);

/*  Ricavo l'indirizzo di inizio della shared memory  */
ind = database->oldatabaseTopologia.id;

/*  Alloco memoria per contenere la lista dei modelli presenti con in testa
	l'intero contenente il numero di modelli presenti  */
appoggio = (char *)XtCalloc (sizeof(int)+(numeroModelli*MAX_LUN_NOME_MODELLO),
                                sizeof(char));
p = appoggio;

/*  Copio in testa alla lista il numero degli elementi   */
memcpy ( p, &numeroModelli, sizeof(int) );
p += sizeof(int);

for (i=1;i<numeroModelli+1;i++)
  {
  modelloappoggio = XtNewString ( nome_modello (ind,i) );
  strncpy ( p, modelloappoggio, strlen ( modelloappoggio ));
  p += MAX_LUN_NOME_MODELLO;
  XtFree ((char *)modelloappoggio);
  }
return (appoggio);
}
#endif

/*
 Verifica che la form sia allocata 
*/
static Boolean IsFormAllocated(OlFormObject form)
{
if(form==NULL)
        {
        XlWarning("OlForm","IsIndexValid","form in mem. non allocato");
        return(False);
        }
return(True);
}


/*
 Verifica che la forms sia allocata e l'indice di form valido
*/
static Boolean IsIndexValid(OlFormObject form,int index)
{
/*
 Verifica la correttezza dei parametri passati
*/
if(form==NULL)
	{
	XlWarning("OlForm","IsIndexValid","form in mem. non allocato");
	return(False);
	}
/*
 Le form contenute nel file .FRM vengono numerate a partire da 0.
 Si verifica che all'indice richiesto corrisponda una form.
*/
if(index<0 || index >= form->olform.num_forms)
	{
        printf("INDICE=%d num_form=%d\n",index,form->olform.num_forms);
	XlWarning("OlForm","IsIndexValid","Indice form richiesta non esistente");
	return(False);
	}
return(True);
}





/* ============================================ */

Boolean out_close(char *stringa,FILE *file_point,int num)\
                  {fprintf(stderr,\
                   "La linea del file non riconosciuta corrisponde a:\
                    \n<%s>\nLinea numero <%d>\n",stringa,num);\
                    fclose(file_point);\
                    return(False);}

/*
 Read
 Legge il file .FRM e inserisce il contenuto nelle opportune strutture
 dell'oggetto form eseguendo le allocazioni di memoria necessarie.
 Controlla che le tag lette appartengano ai campi opportuni.
*/
static Boolean   Read_File(OlFormObject form)
{
FILE *ff;
int i;
int ret;
char *campo;
char str[100];
char str1[100];
char err[100];
/* contatore del numero di righe lette nella scansione del file */
int line_read=0;    
int num;
int cont;
int ind;

/*  Apertura del file .frm */
    if( !(ff=fopen(form->olform.path_name,"r")) )
       {
       fprintf(stderr,"Non si puo' aprire il file %s\n",
                       form->olform.path_name);
       return(False);
       }

/*  Lettura numero di forms */
    line_read = 2;
    ret = leggi_campo(ff,NUME,INT,&form->olform.num_forms,str,LETT_STRIN);
    if( ret==-1 )
       out_close(NUME,ff,line_read);

/*  Allocazione della struttura descrittiva delle forms */
    form->olform.pdescr_forms = (DESCR_FORMS*)
                         calloc(form->olform.num_forms,sizeof(DESCR_FORMS));

/*  Lettura titolo della form */
    line_read = 3;
    ret = leggi_campo(ff,TITLE,STR,&num,
                      form->olform.pdescr_forms[LINE_INP].title,LETT_INPUT);
    if( ret==-1 )
       out_close(NUMEL,ff,line_read);

/*  Lettura numero di linee prima form e allocazione struttura DESCR_LINE */
    line_read = 4;
    ret = leggi_campo(ff,NUMEL,INT,&cont,str,LETT_STRIN);
    if( ret==-1 )
       out_close(NUMEL,ff,line_read);
    form->olform.pdescr_forms[LINE_INP].num_lines   = cont;
    form->olform.pdescr_forms[LINE_INP].pdescr_line =
               (DESCR_LINE*)calloc(cont,sizeof(DESCR_LINE));

/*  Lettura campi della prima form */
    ret = lettura_forms(form,LINE_INP,ff,
                        form->olform.pdescr_forms[LINE_INP].num_lines,
                        STR,LETT_INPUT,&line_read,err);
    if( ret==-1 )
       out_close(err,ff,line_read);

/*  Lettura titolo della form */
    line_read++;
    ret = leggi_campo(ff,TITLE,STR,&num,
                      form->olform.pdescr_forms[LINE_DESCR].title,LETT_INPUT);

/*  Lettura numero di linee seconda form */
    line_read ++;
    ret = leggi_campo(ff,NUMEL,INT,&cont,str,LETT_STRIN);
    if( ret==-1 )
       out_close(NUMEL,ff,line_read);
    form->olform.pdescr_forms[LINE_DESCR].num_lines = cont;
    form->olform.pdescr_forms[LINE_DESCR].pdescr_line =
               (DESCR_LINE*)calloc(cont,sizeof(DESCR_LINE));

/*  Lettura campi della seconda form */
    ret = lettura_forms(form,LINE_DESCR,ff,
                        form->olform.pdescr_forms[LINE_DESCR].num_lines,
                        STR,LETT_INPUT,&line_read,err);
    if( ret==-1 )
       out_close(err,ff,line_read);
    fclose(ff);
    return(ret);
}


/*
    In funzione del campo passato in ingresso viene letta da file
    (con modalita' opportuna) l'informazione corrispondente.
    Quando la stringa cercata non si trova ritorna -1.
*/
int leggi_campo(FILE *fp, char *campo, int tipo_dato, int *intero, 
                char *stringa, int tipo_lettura)
{
int eofile;
int i=0;
char str[100],app_str[100];
char *ret;


    eofile = fscanf(fp,"%s",str);
    /*printf("     str=%s| campo=%s|",str,campo);*/
    while( strcmp(str,campo)!=0 )
       {
       if( strstr(str,campo) )
           break;
       if(eofile==EOF)
          {
          fclose(fp);
          return(eofile);
          }
       eofile = fscanf(fp,"%s",str);
/*       printf("        letto=%s|",str);*/
       }
    /*printf("  --->Trovato str=%s|\n",str);*/
    if ( tipo_dato==INT ) 
       {
       eofile = fscanf(fp,"%d",intero);
       return(eofile);
       }
    else
       if ( tipo_dato==STR )
          {
          if(tipo_lettura==LETT_STRIN)
              eofile = fscanf(fp,"%s",str);
          else
              if(tipo_lettura==LETT_INPUT)
                 {
                 /* vecchia versione
                 eofile = fscanf(fp,"%*[ ]%[^\n]",str);
		 */
                 /* come fscanf: 1 se ha letto, EOF a fine file (prima il
                    puntatore di fgets troncato a int) */
                 if (fgets(app_str,100,fp) == NULL)
                    {
                    stringa[0]=0;
                    return(EOF);
                    }
                 eofile=1;
                 app_str[strcspn(app_str,"\n")]=0;
                 for(i=0;app_str[i]==' ';i++);
                 strcpy(str,&app_str[i]);
                 }
              else
                 {
                 fprintf(stderr,"leggi_campo tipo_lettura non riconosciuto\n");
                 return(-1);
                 }
          memcpy(stringa,str,strlen(str)+1);
          return(eofile);
          }
       else
          {
          fprintf(stderr,"leggi_campo tipo_dato non riconosciuto\n");
          return(-1);
          }
}



/*
    Legge tutte le righe di una form.
    Ritorna -1 in caso di errore nella lettura del file restituendo la 
    stringa che si sarebbe dovuta riconoscere e la riga del file sulla
    quale e' avvenuto l'errore.   
*/
int lettura_forms(OlFormObject form, int indice, FILE *fp, int nl, 
                  int tipo_dato, int tipo_lettura, int *linee_lette, 
                  char *error)
{
int ret;
int dummy;
int i;
char str1[100];
char numero[10];
char prefisso[10];
char linea[10];
char etichetta[100];

    sprintf(numero,"%d%s\00",indice+1,".");
    for(i=0;i<nl;i++)
       {
       strcpy(prefisso,numero);
       sprintf(linea,"%d\00",i+1);
       strcat(prefisso,linea);
       sprintf(etichetta,"%s%s\00",prefisso,TYPE);
       (*linee_lette)++;
/*     Lettura di riconoscimento della linea (input o descr) */
       ret = leggi_campo(fp,etichetta,tipo_dato,&dummy,str1,tipo_lettura);
       if( !strcmp(str1,DESCRIZIONE) )
          {
          form->olform.pdescr_forms[indice].pdescr_line[i].line_type =
                                                                 LINE_DESCR;
/*        Lettura del campo descrizione */
          sprintf(etichetta,"%s%s\00",prefisso,DESCR);
          (*linee_lette)++;
          ret = leggi_campo(fp,etichetta,tipo_dato,&dummy,
                form->olform.pdescr_forms[indice].pdescr_line[i].u.descr.descr,
                            tipo_lettura);
          if( ret==-1 )
             {
             strcpy(error,etichetta);
             return(ret);
             }
          }
       else
          {
/*
          if( strcmp(str1,INPUT) )
             {
             strcpy(error,etichetta);
             return(-1);
             }
*/
          form->olform.pdescr_forms[indice].pdescr_line[i].line_type =
                                                                 LINE_INP;
/*        Lettura del campo tag */
          sprintf(etichetta,"%s%s\00",prefisso,LABEL);
          (*linee_lette)++;
          ret = leggi_campo(fp,etichetta,tipo_dato,&dummy,
                form->olform.pdescr_forms[indice].pdescr_line[i].u.input.tag,
                            tipo_lettura);
          if( ret==-1 )
             {
             strcpy(error,etichetta);
             return(ret);
             }
/*        Lettura del campo format */
          sprintf(etichetta,"%s%s\00",prefisso,FORMAT);
          (*linee_lette)++;
          ret = leggi_campo(fp,etichetta,tipo_dato,&dummy,
                form->olform.pdescr_forms[indice].pdescr_line[i].u.input.format,
                            tipo_lettura);
          if( ret==-1 )
             {
             strcpy(error,etichetta);
             return(ret);
             }
/*        Lettura del campo valore */
          sprintf(etichetta,"%s%s\00",prefisso,VALUE);
          (*linee_lette)++;
          ret = leggi_campo(fp,etichetta,tipo_dato,&dummy,
                form->olform.pdescr_forms[indice].pdescr_line[i].u.input.value,
                            tipo_lettura);
          if( ret==-1 )
             {
             strcpy(error,etichetta);
             return(ret);
             }
          if ( !strcmp(BLANK,
               form->olform.pdescr_forms[indice].pdescr_line[i].u.input.value) )
            {
            strcpy(
              form->olform.pdescr_forms[indice].pdescr_line[i].u.input.value,
              " \00");
            }
/*        Lettura del campo descrizione */
          sprintf(etichetta,"%s%s\00",prefisso,DESCR);
          (*linee_lette)++;
          ret = leggi_campo(fp,etichetta,tipo_dato,&dummy,
                form->olform.pdescr_forms[indice].pdescr_line[i].u.input.descr,
                            tipo_lettura);
          if( ret==-1 )
             {
             strcpy(error,etichetta);
             return(ret);
             }
          if ( !strcmp("\x22\x22",
               form->olform.pdescr_forms[indice].pdescr_line[i].u.input.descr) )
              {
              strcpy(
                form->olform.pdescr_forms[indice].pdescr_line[i].u.input.descr,
                " \00");
              }
          }
       }
       strcpy(error,"");
       return(1);
}
/* Fine file */
