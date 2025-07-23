/**********************************************************************
 *
 *       C Source:               OlForm_regol.c
 *       Subsystem:              1
 *       Description:
 *       %created_by:    famgr %
 *       %date_created:  Mon Sep 21 17:38:47 1998 %
 *
 **********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: OlForm_regol.c-6 %  (%full_filespec: OlForm_regol.c-6:csrc:1 %)";
#endif
/*
   modulo OlForm_regol.c
   tipo
   release 5.3
   data 2/5/96
   reserved @(#)OlForm_regol.c	5.3
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)OlForm_regol.c	5.3\t2/5/96";
/*
        Fine sezione per SCCS
*/
/*
 *  OlForm_regol.c
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <Ol/OlFormP.h>

#if defined UNIX
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/stat.h>
#endif
#if defined VMS
#include <stat.h>
#include "vmsipc.h"
#endif
#include <forme.h>
#include <Ol/OlForm_regol.h>

extern int path_rel_to_abs(char *, char *);
/*
    Strutture di riferimento per il riconoscimento dei dati. - 15/9/94 -
    Vengono usate sia da OlFiltraTags che da OlNumTipiVar che effettua solo
    un conteggio.
    Qualora si introducesse un nuovo carattere va riportato anche nell'assegnamento
    statico fatto per effettuare il conteggio globale delle variabili che evita
    di conteggiare variabili ripetute.
*/
ACCETTAZIONI *acc;
static ACCETTAZIONI in_analog = {{2, 'I', 'S'}, {4, 'A', 'B', 'C', 'I', ' '}};
static ACCETTAZIONI in_digit = {{1, 'J'}, {4, 'L', 'M', 'N', 'J', ' '}};
static ACCETTAZIONI in_dati = {{0, 'J'}, {2, 'H', 'K', ' ', ' ', ' '}};
static ACCETTAZIONI out_analog = {{2, 'U', 'T'}, {4, 'X', 'Y', 'Z', 'U', ' '}};
static ACCETTAZIONI out_digit = {{2, 'J', 'V'}, {5, 'L', 'N', 'M', 'J', 'V'}};

/*
 OlFiltraTags
 Restituisce  con il medesimo formato usato in WriteStrInp un buffer
 creato a partire da un ' altro buffer di Tags.
 In funzione dei parametri direzione (UA e IN), tipo (analogico, digitale,
 dati), collegamento (variabile collegata) scrive sul buffer il numero di
 queste Tags riconosciute e di seguito le Tags stesse.
*/
char *OlFiltraTags(char *buftags, int direzione, int tipo, int collegamento)
{
   int i, j;
   int size;
   int nmax;
   int num_tags;
   int numero = 0;
   char *ret;
   char *ret_app;
   char *punt;
   char *punt_ret;
   char *punt_app;
   char first_car;
   char str_confronto[MAX_L_TAGS + 1];
   char tag[MAX_L_TAGS + 1];
   char tag_tipo[MAX_L_TAGS + 1];

   /*    Controllo primo parametro */
   if ((direzione != VAR_IN) && (direzione != VAR_OUT))
   {
      printf("Parametro direzione non previsto\n");
      return (NULL);
   }

   /*    Scelta della struttura di riferimento per le variabili accettabili */
   if ((direzione == VAR_IN) && (tipo == VAR_ANALOG))
   {
      acc = &in_analog;
      strcpy(str_confronto, DEF_IN);
      /*printf("in_analog\n");*/
   }
   if ((direzione == VAR_IN) && (tipo == VAR_DIGIT))
   {
      acc = &in_digit;
      strcpy(str_confronto, DEF_IN);
      /*printf("in_digit\n");*/
   }
   if ((direzione == VAR_IN) && (tipo == VAR_DATI))
   {
      acc = &in_dati;
      strcpy(str_confronto, DEF_IN);
      /*printf("in_dati\n");*/
   }
   if ((direzione == VAR_OUT) && (tipo == VAR_ANALOG))
   {
      acc = &out_analog;
      strcpy(str_confronto, DEF_OUT);
      /*printf("out_analog\n");*/
   }
   if ((direzione == VAR_OUT) && (tipo == VAR_DIGIT))
   {
      acc = &out_digit;
      strcpy(str_confronto, DEF_OUT);
      /*printf("out_digit\n");*/
   }

   /*    Numero delle tags accettabili su cui testare (il primo carattere) */
   if (collegamento == VAR_COLLEG)
      num_tags = acc->colleg.num_tags;
   else
      num_tags = acc->all.num_tags;

   /*    Lettura del numero massimo di variabili riconoscibili */
   punt = buftags;
   memcpy(&nmax, punt, sizeof(int));
   punt += sizeof(int);

   /*    Alloco il  buffer di appoggio in grado di contenere tutte le variabili */
   size = sizeof(int) + nmax * MAX_L_TAGS;
   ret_app = (char *)malloc(size);
   punt_ret = ret_app;
   punt_ret += sizeof(int);

   /*    Ciclo su tutte le tags in ingresso */
   for (i = 0; i < nmax; i++)
   {
      strcpy(tag, punt);
      punt += MAX_L_TAGS;
      strncpy(tag_tipo, &tag[OFF_INI_TIPO], LUN_TIPO + 1);
      tag_tipo[LUN_TIPO] = '\0';
      first_car = tag[0];
      if (!strcmp(tag_tipo, str_confronto))
      {
         /*          Ciclo sul numero delle tags riconoscibili in base agli ingressi */
         for (j = 0; j < num_tags; j++)
         {
            if (collegamento == VAR_COLLEG)
            {
               if (first_car == acc->colleg.first_ch_tag[j])
               {
                  memcpy(punt_ret, tag, MAX_L_TAGS);
                  punt_ret += MAX_L_TAGS;
                  numero++;
               }
            }
            else
            {
               if (first_car == acc->all.first_ch_tag[j])
               {
                  memcpy(punt_ret, tag, MAX_L_TAGS);
                  punt_ret += MAX_L_TAGS;
                  numero++;
               }
            }
         }
      }
   }
   /*      Si copia in uscita in un buffer delle dimensioni necessarie */
   size = sizeof(int) + numero * MAX_L_TAGS;
   ret = (char *)malloc(size);
   punt_ret = ret;
   memcpy(punt_ret, &numero, sizeof(int));
   punt_ret += sizeof(int);
   punt_app = ret_app;
   punt_app += sizeof(int);
   memcpy(punt_ret, punt_app, (size - sizeof(int)));
   free(ret_app);

   return (ret);
}

/*
 OlNumTipiVar
*/
int OlNumTipiVar(char *buftags, int *nvin, int *nvout, int *nvdat)
{
   /*  Strutture di riferimento per conteggio dei dati. - 16/9/94 -
       Qualora si introducesse un nuovo carattere va riportato anche nell'assegnamento
       statico fatto per effettuare il filtraggio delle variabili separato per
       ingressi e uscite digitali/analogiche (OlFiltraTags)
   ACCETTAZIONI *acc;
       ELENCO_ACCETTAZIONI[0] = Sono tutti gli ingressi
       ELENCO_ACCETTAZIONI[1] = Sono tutte le uscite
       ELENCO_ACCETTAZIONI[2] = Sono le variabili dati (che sono di tipo --IN--)
   */
   ELENCO_ACCETTAZIONI *count;
   static ELENCO_ACCETTAZIONI var[3] =
       {{8, 'A', 'B', 'C', 'I', 'J', 'L', 'M', 'N'},
        {9, 'X', 'Y', 'Z', 'U', 'J', 'V', 'L', 'M', 'N'},
        {2, 'H', 'K', ' ', ' ', ' ', ' ', ' ', ' '}};
   int numeri_var[3] = {0, 0, 0};
   int i, j, k;
   int nmax;
   int num_tags;
   char *punt;
   char *punt_mem;
   char first_car;
   char tag[MAX_L_TAGS + 1];
   char tag_tipo[MAX_L_TAGS + 1];

   /*    Lettura del numero massimo di variabili riconoscibili */
   punt = buftags;
   memcpy(&nmax, punt, sizeof(int));
   punt += sizeof(int);
   punt_mem = punt;

   /*    Ciclo per testare i 3 casi di variabili accettabili */
   for (i = 0; i < 3; i++)
   {
      punt = punt_mem;
      count = &var[i];
      num_tags = count->num_tags;
      for (j = 0; j < nmax; j++)
      {
         strcpy(tag, punt);
         punt += MAX_L_TAGS;
         strncpy(tag_tipo, &tag[OFF_INI_TIPO], LUN_TIPO + 1);
         tag_tipo[LUN_TIPO] = '\0';
         first_car = tag[0];
         /*          Si controlla se la variabile puo' corrispondere nel tipo */
         if (((i != 1) && (strcmp(tag_tipo, DEF_IN) == 0)) ||
             ((i == 1) && (strcmp(tag_tipo, DEF_OUT) == 0)))
         {
            /*             Ciclo sul numero di tags riconoscibili secondo gli ingressi */
            for (k = 0; k < num_tags; k++)
               if (first_car == count->first_ch_tag[k])
                  numeri_var[i]++;
         }
      }
   }
   (*nvin) = numeri_var[0];
   (*nvout) = numeri_var[1];
   (*nvdat) = numeri_var[2];

   return (1);
}

/*
     Determina il numero d'ordine (posizione) di una Tag tra le
     possibili Tags delle diverse form.
*/
int PosTag(OlFormObject form, char *tag_cercata, int num_car,
           int indice, int *linea)
{
   int i;
   int ind;
   int num_lines;
   int num_forms;
   int numero_tag = 0;
   int trovata = 0;

   num_forms = olFormClassRec.olform_class.get_num_forms(form);
   numero_tag = 0;
   for (ind = 0; ind < num_forms; ind++)
   {

      /*     Cerco tra le linee il numero d'ordine della Tag */
      num_lines = form->olform.pdescr_forms[ind].num_lines;
      for (i = 0; i < num_lines; i++)
      {
         (*linea) = i;
         if (form->olform.pdescr_forms[ind].pdescr_line[i].line_type ==
             LINE_INP)
         {
            numero_tag++;
            if (!strncmp(
                    form->olform.pdescr_forms[ind].pdescr_line[i].u.input.tag,
                    tag_cercata, num_car) &&
                (indice == ind))
            {
               trovata = 1;
               break;
            }
         }
      }
      if (trovata == 1)
         break;
   } /* Fine ciclo su ind */
   if (trovata == 0)
   {
      numero_tag = -1;
      fprintf(stderr, "ATTENZIONE, Tag <%s> non trovata nel file \n",
              tag_cercata);
   }

   return (numero_tag);
}

/*
    Data la stringa prodotta da WriteStrInp "|VALORE| |DESCRIZIONE|"
    e la Tag interessata restituisce il valore recuperandolo
    dalla stringa.
    La posizione nella stringa viene determinata cercando la Tag
    nella struttura dell'oggetto.
    Controlli sul buffer in ingresso
    Restituisce la stringa contenente il valore letto:
    La stringa viene allocata internamente a questa funzione e quindi
    deve essere successivamente liberata (free).
*/
char *EstrTagVal(OlFormObject form, int ind,
                 char *tag_cercata, int lun_ricerca, char *buffer_inp)
{
   int i, k;
   int num_lines;
   int num_forms;
   int numero_tag = 0;
   int pos;
   int pos_strutt;
   char *punt_val;
   char *punt_des;
   char *punt_blk;
   char *punt_val_prec;
   char *ret;
   char *buffer = NULL;
   char punt_app[MAX_L_TAGS + 1];
   int stringa_anomala = 0;
   int errore_strtok = 0;

   /*      Controlli sulla stringa in ingresso */
   if (strstr(buffer_inp, "no value") != NULL)
   {
      fprintf(stderr,
              "EstrTagValue fallita:stringa contenente -no value-%s|\n",
              buffer_inp);
      return (NULL);
   }

   if ((buffer_inp == NULL) || (strlen(buffer_inp) == 0))
   {
      stringa_anomala = 1;
      printf("\n caso di stringa anomala \n");
   }
   /*
           Alloca un buffer di appoggio con la copia del buffer in input
   */
   if (buffer_inp)
      buffer = XtNewString(buffer_inp);
   else
      buffer = NULL;

   /*      Identifico l'indice del vettore in cui sta la stringa cercata */
   if ((pos = PosTag(form, tag_cercata, lun_ricerca, ind, &pos_strutt)) < 0)
   {
      if (buffer)
         XtFree(buffer);
      return (NULL);
   }

   /*      Allocazione */
   ret = (char *)malloc(MAX_L_TAGS * sizeof(char));

   /*      Con stringa di input vuota viene letto il valore della Tag in memoria */
   if (stringa_anomala == 1)
   {
      fprintf(stderr, "EstrTagValue: stringa ingresso NULL o lunga 0\n");
      fprintf(stderr, "          --> si legge il valore da memoria \n");
      strcpy(ret,
             form->olform.pdescr_forms[ind].pdescr_line[pos_strutt].u.input.value);
      if (buffer)
         XtFree(buffer);
      return (ret);
   }

   /*      Lettura primo valore */
   punt_val = strtok(buffer, "|"); /* Lettura primo valore */

   /*      Se la tags di interesse e' la prima e' gia' stato letto il valore */
   if (pos == 1)
   {
      strcpy(punt_app, punt_val);
      memcpy(ret, punt_app, MAX_L_TAGS);
      /* MAURIC
                 memcpy(ret,punt_val,MAX_L_TAGS);
      */
      if (buffer)
         XtFree(buffer);
      return (ret);
   }

   /*
           Prosecuzione lettura e memorizzazione del valore precedente.
           Il valore effettivo e' quello in punt_val_prec dato che il valore
           precede la descrizione che viene letta comunque.
           Il controllo sullo sforamento nella strtok nello stringone viene
           effettuato per questo motivo alle prime due strtok.
   */
   for (k = 0; k < pos; k++)
   {
      punt_val_prec = punt_val;
      /*printf("(%d) punt_val_prec=%s|\n",errore_strtok,punt_val_prec);*/
      if (punt_val_prec == NULL)
      {
         errore_strtok = 1;
         break;
      }

      punt_blk = strtok(NULL, "|"); /* Lettura blank di separazione */
      /*printf("(%d) punt_blk=%s|\n",errore_strtok,punt_blk);*/
      if (punt_blk == NULL)
      {
         errore_strtok = 2;
         break;
      }

      punt_des = strtok(NULL, "|"); /* Lettura campo descrizione */
      /*printf("(%d) punt_des=%s|\n",errore_strtok,punt_des);*/
      if (punt_des == NULL)
      {
         errore_strtok = 3;
         break;
      }

      punt_blk = strtok(NULL, "|"); /* Lettura blank di separazione */
      /*printf("(%d) punt_blk=%s|\n",errore_strtok,punt_blk);*/
      if (punt_blk == NULL)
         errore_strtok = -4;

      punt_val = strtok(NULL, "|"); /* Lettura campo valore */
      /*printf("(%d) punt_val=%s|\n",errore_strtok,punt_val);*/
      if (punt_val == NULL)
         errore_strtok = -5;

      /*printf("(%d)  _______________________\n",errore_strtok);*/
   }

   if (errore_strtok > 0)
   {
      fprintf(stderr, "EstrTagVal: Errore in lettura stringone\n");
      return (NULL);
   }
   strcpy(punt_app, punt_val_prec);
   memcpy(ret, punt_app, MAX_L_TAGS);
   /* MAURIC
           memcpy(ret,punt_val_prec,MAX_L_TAGS);
   */
   if (buffer)
      XtFree(buffer); /* libera il buffer di appoggio */
   return (ret);
}

/*
    Data la stringa prodotta da WriteStrInp e la Tag interessata
    restituisce la descrizione della Tag recuperandola dalla stringa o
    in memoria se la stringa e' NULL.
    La posizione nella stringa viene determinata cercando la Tag
    nella struttura dell'oggetto.
*/
char *EstrTagDescr(OlFormObject form, int ind, char *tag_cercata,
                   int lun_ricerca, char *buffer_inp)
{
   int i, k;
   int num_lines;
   int num_forms;
   int numero_tag = 0;
   int pos;
   int pos_strutt;
   char *punt_val;
   char *punt_des;
   char *punt_blk;
   char *punt_des_prec;
   char *ret;
   char *buffer = NULL;
   char punt_app[MAX_L_DESCR + 1];

   /*
           Alloca un buffer di appoggio con la copia del buffer in input
   */
   if (buffer_inp)
      buffer = XtNewString(buffer_inp);

   /*      Identifico l'indice del vettore in cui sta la stringa cercata */
   if ((pos = PosTag(form, tag_cercata, lun_ricerca, ind, &pos_strutt)) < 0)
   {
      if (buffer)
         XtFree(buffer);
      return (NULL);
   }

   /*      Allocazione */
   ret = (char *)malloc((MAX_L_DESCR + 1) * sizeof(char));

   /*      Con stringa di input vuota viene letta la descr della Tag in memoria */
   if (buffer == NULL)
   {
      fprintf(stderr, "Si legge la descrizione da memoria e non da stringa\n");
      strcpy(ret,
             form->olform.pdescr_forms[ind].pdescr_line[pos_strutt].u.input.descr);
      return (ret);
   }

   /*      Lettura prima descrizione */
   punt_val = strtok(buffer, "|"); /* Lettura primo valore */
   punt_blk = strtok(NULL, "|");   /* Lettura blank di separazione */
   punt_des = strtok(NULL, "|");   /* Lettura campo descrizione */

   /*      Se la tags di interesse e' la prima e' gia' stata letta */
   if (pos == 1)
   {
      strcpy(punt_app, punt_des);
      memcpy(ret, punt_app, MAX_L_DESCR);
      /* MAURIC
                 memcpy(ret,punt_des,MAX_L_DESCR);
      */
      if (buffer)
         XtFree(buffer);
      return (ret);
   }

   /*      Prosecuzione lettura e memorizzazione del valore precedente */
   for (k = 0; k < pos; k++)
   {
      punt_des_prec = punt_des;
      punt_blk = strtok(NULL, "|"); /* Lettura blank di separazione */
      punt_val = strtok(NULL, "|"); /* Lettura campo descrizione */
      punt_blk = strtok(NULL, "|"); /* Lettura blank di separazione */
      punt_des = strtok(NULL, "|"); /* Lettura campo descrizione */
   }
   strcpy(punt_app, punt_des_prec);
   memcpy(ret, punt_app, MAX_L_DESCR);
   /* MAURIC
           memcpy(ret,punt_des_prec,MAX_L_DESCR);
   */
   if (buffer)
      XtFree(buffer); /* libera il buffer di appoggio */

   return (ret);
}

/*
    Data la stringa prodotta da WriteStrInp restituisce tutte le descrizioni
    delle Tags recuperandole dalla stringa (in memoria se la stringa e' NULL) .
    Il formato e' analogo a quello della stringa in ingresso ma contiene le
    sole descrizioni.
*/
char *EstrDescr(OlFormObject form, int ind, char *buffer_inp)
{
   int i, k;
   int num_tags;
   int num_lines;
   char *punt_val;
   char *punt_des;
   char *punt_blk;
   char *ret;
   char *punt;
   char *buffer;
   char *buffer_originale = NULL;
   char punt_app[MAX_L_DESCR + 1];
   int size;

   /*
    alloca un buffer di appoggio con la copia del buffer
    in input
   */
   if (buffer_inp)
      buffer_originale = XtNewString(buffer_inp);
   num_tags = OlGetNumTags(form, ind);
   if (num_tags == 0)
   {
      ret = malloc(sizeof(int));
      memcpy(ret, &num_tags, sizeof(int));
      if (buffer_originale)
         XtFree(buffer_originale);
      return (ret);
   }

   /* Posizionamento nella posizione dei dati (quando ind=1) */
   if (ind != 0)
      buffer = PosStringForm(form, ind, buffer_originale);
   else
      buffer = buffer_originale;

   /*      Allocazione */
   size = sizeof(int) + MAX_L_DESCR * num_tags + sizeof(char);
   ret = (char *)malloc(size);
   punt = ret;
   memcpy(punt, &num_tags, sizeof(int));
   punt += sizeof(int);
   /*
           sprintf(punt,"%s\00","|");
           punt += sizeof(char);
   */

   /*      Con stringa di input vuota viene letta la descr della Tag in memoria */
   if (buffer == NULL)
   {
      fprintf(stderr,
              "Si leggono tutte le descrizioni dalla memoria \n");
      num_lines = OlGetNumLines(form, ind);
      for (k = 0; k < num_lines; k++)
      {
         if (form->olform.pdescr_forms[ind].pdescr_line[k].line_type ==
             LINE_INP)
         {
            memcpy(punt,
                   form->olform.pdescr_forms[ind].pdescr_line[k].u.input.descr,
                   MAX_L_DESCR);
            punt += MAX_L_DESCR;
         }
      }
      memcpy(punt, '\00', sizeof(char));
      return (ret);
   }

   /*      Lettura descrizioni */
   punt_val = strtok(buffer, "|"); /* Lettura primo valore */
   for (k = 0; k < num_tags; k++)
   {
      punt_blk = strtok(NULL, "|"); /* Lettura blank di separazione */
      punt_des = strtok(NULL, "|"); /* Lettura campo descrizione */
      punt_blk = strtok(NULL, "|"); /* Lettura blank di separazione */
      punt_val = strtok(NULL, "|"); /* Lettura campo descrizione */
                                    /*
                                               strcat(punt,punt_des);
                                               punt += strlen(punt);
                                               strcat(punt,SEPARATORE);
                                               punt += strlen(SEPARATORE);
                                    */
      strcpy(punt_app, punt_des);
      memcpy(punt, punt_app, MAX_L_DESCR);
      /* MAURIC
                 memcpy(punt,punt_des,MAX_L_DESCR);
      */
      punt += MAX_L_DESCR;
   }
   /*
           memcpy(punt,'\00',sizeof(char));
   */
   (*punt) = '\0';
   if (buffer_originale)
      XtFree(buffer_originale); /* libera il buffer di appoggio */
   return (ret);
}

/*
    Data la stringa prodotta da WriteStrInp e la Tag interessata
    inserisce nella stringa la nuova descrizione nella posizione opportuna.
    La posizione nella stringa viene determinata cercando la Tag
    nella struttura dell'oggetto.
    La stringa inizia sempre con un carattere '|'.
    Ritorna -1 se fallisce.
*/
int InsTagDescr(OlFormObject form, int ind,
                char *tag_cercata, int lun_ricerca,
                char **buffer_inp, char *val_inp)
{
   int i, k;
   int numero_tag = 0;
   int pos;
   int pos_strutt;
   int space;
   int lun;
   int lun_str;
   int diff;
   char *punt;
   char *punt_app;
   char *buffer;
   char *appoggio = NULL;
   char val[4];
   int ultimo_campo = 0;

   if ((*buffer_inp) == NULL)
   {
      fprintf(stderr, "InsTagDescr: Stringa di ingresso vuota\n");
      return (-1);
   }
   if (strlen((*buffer_inp)) == 0)
   {
      fprintf(stderr, "InsTagDescr: Stringa lunga 0\n");
      return (-1);
   }
   /*      Identifico l'indice del vettore in cui sta la stringa cercata */
   if ((pos = PosTag(form, tag_cercata, lun_ricerca, ind, &pos_strutt)) < 0)
   {
      printf("Ricerca fallita di tag_cercata=%s| \n",
             tag_cercata);
      return (-1);
   }
   printf("InsTagDescr:POS=%d\n",pos);

   punt = (*buffer_inp) + 1;
   /*
           Crecare un valore in posizione <pos> nella stringa di input significa
           trovare 2*(pos-1)+1 stringhe di tipo SEPARATORE.
      (Una tra val(i) e des(i) ed una tra des(i) e val(i+1) ).
   */
   for (i = 0; i < (2 * (pos - 1) + 1); i++)
   {
      punt_app = strstr(punt, SEPARATORE);
      punt = punt_app + strlen(SEPARATORE);
   }

   /*      Spazio disponibile per la nuova stringa al posto della vecchia */
   punt_app = punt;
   space = 0;
   while (punt_app[space] != '|')
      space++;
   /* il campo da sostituire e' l'ultimo campo della riga,
      va quindi trattato in maniera diversa  */
   if (punt_app[space + 1] == 0)
   {
      printf("ULTIMO CAMPO\n");
      ultimo_campo = 1;
   }
   punt_app += space + strlen(SEPARATORE);

   /* printf("punt <%d>   punt_app <%d>\n",
              strlen(punt),strlen(punt_app));*/

   lun = strlen(val_inp);
   lun_str = strlen((*buffer_inp));

   diff = (int)fabs((float)(lun - space));

   /*printf("lun=%d lun_str=%d diff=%d space=%d\n",
             lun,lun_str,diff,space);*/
   if (diff != 0)
   {
      punt[0] = '\00';
      appoggio = XtMalloc(lun + strlen(SEPARATORE) + strlen(punt_app) + 1);
      if (ultimo_campo == 1) /* ultimo campo della riga */
         sprintf(appoggio, "%s|\0", val_inp);
      else
         sprintf(appoggio, "%s%s%s\0", val_inp, SEPARATORE, punt_app);

      /* printf("buffer_input lungo=%d  appoggio lungo=%d\n",
                 strlen((*buffer_inp)),strlen(appoggio));*/
      /*printf("Io alloco di %d  Potrei allocare=%d\n",
              lun_str+diff+1,strlen((*buffer_inp))+strlen(appoggio));*/

      (*buffer_inp) = (char *)XtRealloc((*buffer_inp), lun_str + diff + 1);

      strcat((*buffer_inp), appoggio);
      if (appoggio != NULL)
         XtFree(appoggio);
   }
   else
      memcpy(punt, val_inp, strlen(val_inp));
   /*
   printf("BUFFER_INPUT=%s- (lun=%d)\n",
           (*buffer_inp),strlen((*buffer_inp)));
   */

   return (1);
}

/*
    Data la stringa prodotta da WriteStrInp e la Tag interessata
    inserisce nella stringa un nuovo valore nellla posizione opportuna.
    La posizione nella stringa viene determinata cercando la Tag
    nella struttura dell'oggetto.
    La stringa inizia sempre con un carattere '|'.
    Ritorna -1 se fallisce.
*/
int InsTagVal(OlFormObject form, int ind,
              char *tag_cercata, int lun_ricerca,
              char **buffer_inp, char *val_inp)
{
   int i, k;
   int numero_tag = 0;
   int pos;
   int pos_strutt;
   int space;
   int lun;
   int lun_str;
   int diff;
   char *punt;
   char *punt_app;
   char *buffer;
   char *appoggio = NULL;
   char val[4];

   // Debug
   // printf("InsTagVal: Entering with buffer at %p, content: %.50s\n",
   //        *buffer_inp, *buffer_inp);

   if ((*buffer_inp) == NULL)
   {
      //         fprintf(stderr,"InsTagVal: Stringa di ingresso vuota\n");
      return (-1);
   }
   if (strlen((*buffer_inp)) == 0)
   {
      //         fprintf(stderr,"InsTagVal: Stringa lunga 0\n");
      return (-1);
   }
   /*      Identifico l'indice del vettore in cui sta la stringa cercata */
   if ((pos = PosTag(form, tag_cercata, lun_ricerca, ind, &pos_strutt)) < 0)
   {
      printf("Ricerca fallita di tag_cercata=%s| \n",
             tag_cercata);
      return (-1);
   }
   /**
           printf("InsTagVal:POS=%d|\n",pos);
   **/

   punt = (*buffer_inp) + 1;
   /*
           Crecare un valore in posizione <pos> nella stringa di input significa
           trovare 2*(pos-1) stringhe di tipo SEPARATORE. (Una tra val(i) e des(i)
           ed una tra des(i) e val(i+1) ).
   */
   for (i = 0; i < 2 * (pos - 1); i++)
   {
      punt_app = strstr(punt, SEPARATORE);
      punt = punt_app + strlen(SEPARATORE);
   }

   /*      Spazio disponibile per la nuova stringa al posto della vecchia */
   punt_app = punt;
   space = 0;
   while (punt_app[space] != '|')
      space++;
   punt_app += space + strlen(SEPARATORE);

   /* printf("punt <%d>   punt_app <%d>\n",
              strlen(punt),strlen(punt_app));*/

   lun = strlen(val_inp);
   lun_str = strlen((*buffer_inp));

   diff = (int)fabs((float)(lun - space));

   if (diff != 0)
   {
      // Non liberare memoria per ora, solo alloca nuova
      char *new_buffer = XtMalloc(lun_str + diff + 1);

      // Copia il contenuto
      strcpy(new_buffer, *buffer_inp);

      // Modifica nel nuovo buffer
      char *new_punt = new_buffer + (punt - *buffer_inp);
      strcpy(new_punt, val_inp);
      strcat(new_punt, SEPARATORE);
      strcat(new_punt, punt_app);

      // Aggiorna il puntatore
      *buffer_inp = new_buffer;

      // NON liberare il vecchio buffer per evitare crash
      //        printf("InsTagVal: Created new buffer at %p\n", new_buffer);
   }
   else
   {
      memcpy(punt, val_inp, strlen(val_inp));
      return (1); // Ritorna 1 per indicare che NON è stata allocata nuova memoria
   }

   // GUAG2025 crash stringone - attenzione vedi anche InsTagDescr !
   /*printf("lun=%d lun_str=%d diff=%d space=%d\n",
             lun,lun_str,diff,space);*/
   //         if( diff!=0 )
   //             {
   //             punt[0] = '\00';
   // printf("InsTagVal: buffer_inp=%p *buffer_inp=%p\n", buffer_inp, *buffer_inp);
   //             appoggio = XtMalloc(lun+strlen(SEPARATORE)+strlen(punt_app)+1);
   //             sprintf(appoggio,"%s%s%s",val_inp,SEPARATORE,punt_app);
   // printf("InsTagVal: lun_str=%d diff=%d totale=%d\n", lun_str, diff, lun_str+diff+1);
   // fflush(stdout);
   //             /* printf("buffer_input lungo=%d  appoggio lungo=%d\n",
   //                        strlen((*buffer_inp)),strlen(appoggio));*/
   //             /*printf("Io alloco di %d  Potrei allocare=%d\n",
   //                     lun_str+diff+1,strlen((*buffer_inp))+strlen(appoggio));*/
   // // XtFree(*buffer_inp);
   // // *buffer_inp = XtMalloc(lun_str+diff+1);
   //             (*buffer_inp) = (char*)XtRealloc((*buffer_inp),lun_str+diff+1);

   //             strcat((*buffer_inp),appoggio);
   //             if(appoggio!=NULL)
   //                XtFree(appoggio);
   //             }
   //          else
   //             memcpy(punt,val_inp,strlen(val_inp));
   //          /*
   //          printf("BUFFER_INPUT=%s- (lun=%d)\n",
   //                  (*buffer_inp),strlen((*buffer_inp)));
   //          */

   return (2); // Ritorna 2 per indicare che è stata allocata nuova memoria
}

/*
    In una stringa composta da stringhe valori-descrizioni,
    una per ogni form (ad esempio due : valori, dati)  si
    posiziona all'inizio della stringa corrispondente alla
    form scelta.
*/
char *PosStringForm(OlFormObject form, int ind, char *stringone)
{
   int num_forms;
   int num_tags;
   int i, j;
   char *ret;
   char *punt;

   /*  Se si tratta della prima form (0) sono gia' collocato */
   if (ind == 0)
      return (stringone);
   /*  Controlli sul numero delle forms e delle tags della form */
   num_forms = OlGetNumForms(form);
   if (ind > num_forms)
   {
      fprintf(stderr, "Richiesto posizionamento su form non esistente (%d)\n",
              ind);
      return (NULL);
   }
   num_tags = OlGetNumTags(form, ind);
   if (num_tags == 0)
      return (NULL);

   /*   Ricerca delle tags : ho 4 caratteri "|" per ciascuna
        coppia valore-descrizione */
   punt = stringone;
   for (i = 0; i < ind; i++)
   {
      num_tags = OlGetNumTags(form, i);
      for (j = 0; j < 4 * num_tags; j++)
      {
         punt = strstr(punt, "|");
         punt++;
      }
   }
   /*    Ritorno a partite dal primo separatore */
   punt++;
   ret = punt;
   /*printf("num_tags=%d RITORNO=%s=\n",num_tags,ret);*/
   return (ret);
}

/*
     Estrae da un assegnato pathname di regolazione del tipo standard
     xxxxxx/r_tasknumero1 i primi due caratteri che seguono r_ e ritorna
     fra i suoi parametri il nome della task, in questo caso nome_out
     ma limita ad 8 caratteri.
*/
char *OlEstrRegTaskName(char *nome_path, char *nome_out)
{
#define FILTRO_REG "r_"
#define DIM_RET 2
   int i;
   int old_directory = 0;
   int poscpy;
   char *ret;
   char appoggio[200] = {'\0'};
   char appoggio1[200] = {'\0'};
   char nome_path_abs[FILENAME_MAX];

   printf("\n    NOMEIN=%s\n", nome_path);
   strcpy(nome_out, "");
   if (path_rel_to_abs(nome_path, nome_path_abs) < 0)
   {
      printf("OlEstrRegTaskName: error retrieving abs path of=%s\n", nome_path);
      return (NULL);
   }

   for (i = strlen(nome_path_abs); i > 0; i--)
   {
      if (nome_path_abs[i] == '/')
      {
         strcpy(appoggio, &nome_path_abs[i + 1]);

         /*        Controllo che il direttorio sia del tipo regolazione */
         if (strncmp(appoggio, FILTRO_REG, 2) != 0)
         {
            fprintf(stderr, "Directory senza la chiave %s\n", FILTRO_REG);
            old_directory = 1;
            /* return(NULL);*/
         }
         if (strlen(appoggio) <= strlen(FILTRO_REG))
         {
            fprintf(stderr, "Directory task non standard\n%s\n", appoggio);
            return (NULL);
         }

         /*        Se il direttorio e' vecchio copio da 0 */
         if (old_directory == 1)
         {
            poscpy = 0;
         }
         else
         {
            poscpy = strlen(FILTRO_REG);
         }

         strncpy(appoggio1, &appoggio[poscpy], 8);

         if (strlen(appoggio1) > 8)
            appoggio1[8] = '\0';

         strcpy(nome_out, appoggio1);

#if defined VMS
         nome_out[strlen(nome_out) - 1] = '\0';
#endif
#if defined UNIX
         nome_out[strlen(nome_out)] = '\0';
#endif

         /*        Determinato il nome si esce */
         break;
      }
   }

   /* Test */
   if (i == 0)
   {
      fprintf(stderr, "Estrazione nome task scorretta ");
      return (NULL);
   }

   /* Ritorno: i primi DIM_RET (2) caratteri */
   ret = (char *)malloc(sizeof(char) * (DIM_RET + 1));
   strncpy(ret, nome_out, DIM_RET);
   ret[DIM_RET] = '\0';

   return (ret);
}
/* Fine file */
