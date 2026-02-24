/**************************************************************

 pag2f01.c   v. 1.5

 Creato(MB):  3 Febbraio 1995
 PC porting: 18 Dicembre 1997
 Modif.(MR):  6 Novembre 1998
 Modif.(GM): 14 Maggio   1999
 Modif.(GM): 25 Maggio   2000
 Modif.(GM): 09 Ottobre  2000

 descrizione:
 File sorgente dell'applicazione pag2f01, un prototipo
 di convertitore che accetta in input un file .pag
 o un .top, e crea il file f01.dat corrispondente.

 L'applicazione si compone di sette fasi distinte, eseguite
 in maniera sequenziale:

 I.a) se e` attiva l'opzione -p, viene letto il file .pag
   In questa fase vengono allocate le strutture dati per i
   moduli e le porte fisiche (comprese le variabili fisiche).
   Le strutture vengono riempite con le informazioni contenute
   nel .pag, che sono il nome dei moduli, il tipo di porte che
   essi contengono, e le CONNESSIONI tra le diverse porte.
   Al termine viene scritto un file .top con le informazioni
   finora recuperate.

 I.b) senza opzioni, viene letto il file .top
   In questa fase vengono allocate le strutture dati per i
   moduli e le porte fisiche (comprese le variabili fisiche).
   Le strutture vengono riempite con le informazioni contenute
   nel .top, che sono il nome dei moduli, il tipo di porte che
   essi contengono, e le CONNESSIONI tra le diverse porte.

 II) vengono letti i files .I5
   In questa fase si completa il riempimento delle strutture
   dati. Le informazioni ricavate sono quelle relative alla
   parte matematica del modulo (variabili e loro tipo, eventuali
   tipi di configurazione).
   Se il modulo ha piu` di una configurazione possibile, si
   chiede all'utente se desidera sceglierne una, o se lascia
   fare al sistema (configurazione automatica).

 III) vengono analizzate le connessioni tra le singole variabili

   III.a) se si trovano collegamenti tra variabili di tipo opposto
     (-1 -> 1 o viceversa, -1 -> 2 ->2... -> 1 o viceversa)
     l'applicazione da` un messaggio di errore.
   III.b) se si identifica un collegamento tra un 3 e una var.
     di valore fissato (-1 o 1), si verifica se il 3 puo` assumere quel
     valore.
   III.c) si inseguono le catene di 2 e se possibile si sostituiscono
     un po' di 3.

 IV) vengono assegnate le configurazioni matematiche a quei
     moduli non ancora configurati

   IV.a) il vettore conftree viene riempito con gli ID dei moduli
     non ancora configurati (cioe` quelli con ancora qualche
     variabile di valore 3). Se un modulo ha dei 3, ma solo una
     delle possibili configurazioni si adatta a quella corrente,
     viene configurato e rimosso da conftree; se un modulo ha
     dei 3, ma nessuna delle possibili configurazioni si adatta
     a quella corrente, l'applicazione da` un messaggio di errore.

   IV.b) vengono ricercate sottoreti indipendenti (anche collegate
     attraverso catene di 2) e si cerca di configurarle con una
     configurazione definitiva.
     Se una di esse non e` configurabile, errore.

   IV.c) vengono analizzate tutte le possibili combinazioni di
     configurazioni per i moduli non ancora configurati: nel
     momento in cui una configurazione risulta accettabile, viene
     scelta come definitiva. In realta`, se la fase IV.b viene
     eseguita correttamente, a questo punto non dovrebbero
     esserci piu` moduli non configurati, per cui questa fase
     viene mantenuta per sicurezza.

 V) vengono sostituiti i nomi di variabili collegate attraverso
   variabili di passaggio
   In questa fase si effettuano le sostituzioni tra variabili
   matematiche collegate attraverso serie di 2. Il procedimento
   viene iterato finche' tutti i 2 sono stati trattati.

 VI) ultime sostituzioni
   In questa fase si sostituiscono i nomi delle variabili
   matematiche collegate attraverso un link diretto.

   VI.b) controllo su variabili di ingresso duplicate ed eventuale
     aggiunta di moduli DISN.

 VII) costruzione del file f01.dat
   In questa fase si effettua il riversamento dei dati di modlist
   sul file di output. Vengono modificati i primi caratteri dei
   blocchi con configurazioni particolari (es. EVAP -> +EVA)
   tranne che per i moduli NODO.

 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef Linux
#include <ctype.h>
#endif

#ifndef Linux
#include <windows.h>
#endif

#include "pag2f01.h"

#define MAXLINELENGTH 512

/* global var */

char *I5PATH;                               /* path della directory contenente i files I5   */
int modnum;                                 /* numero di moduli nello schema                */
int fol2buf[MAXMULTLINK];                   /* buffer contenente i valori dei limiti della catena di 2 corrente */
int fol2vtbuf[MAXMULTLINK];                 /* buffer con il tipo di limite */
fpdesc *fol2pbuf[MAXMULTLINK];              /* buffer contenente le porte dei limiti della catena di 2 */
char fol2vnbuf[MAXMULTLINK][MVNAMENCH + 1]; /* buffer con i nomi completi delle var. limite */
int fol2num3, fol2last3;                    /* numero di limiti d'uscita di valore 3 e posizione dell'ultimo 3 */
int fol2midbuf;                             /* contatore del numero di 2 della catena di 2 corrente */
int nummatCrossed4nodo;                     /* contatore del numero di limiti matematici della catena di 2 */
fpdesc *fol2midpbuf[MAXMULTLINK];           /* buffer contenente le porte dei 2 della catena di 2 */
moddesc *modlist[MAXNMODULE];               /* lista globale dei moduli                     */
int fol2Lbuf[MAXMULTLINK];                  /* buffer contenente gli ID dei moduli dei limiti della catena di 2 Lcorrente */
fpdesc *fol2Lpbuf[MAXMULTLINK];             /* buffer contenente le porte dei limiti della Lcatena di 2 */
char tempstr[MAXLINELENGTH*2];
int cftreecount;
int conftree[CFTREE_MAXMODUNASS][MAXNMATCFG + 1]; /* albero di check combinazioni matematiche */
int confnet[CFTREE_MAXMODUNASS], autocfgOK;
int cfLtreecount;
int confLtree[CFTREE_MAXMODUNASS][MAXNMATCFG]; /* albero di check combinazioni mat. locali */
int confLnet[CFTREE_MAXMODUNASS], autocfgLOK;
int zerosumLnet[CFTREE_MAXMODUNASS], foundaZerosum, localZerosum;
int chain2isLocal; /* flag che segnala se la catena di 2 influisce sulla sottorete in verifica */
int disnnum;       /* numero di moduli DISN aggiunti automaticamente */

int LMFMANAGE(void);
void read_conn(void);

int aggiornadescrf01(), rettx;
// #define MAXLIN 2000                //massimo linee del file f01.dat
// char *f01old[MAXLIN];

/* essit - routine di gestione errori fatali                    */

void essit(char *mesg)

{
  printf(mesg);
  exit(2);
}

void essitW(char *mesg)

{
  printf(mesg);
}

/* warn - routine di gestione avvertimenti non fatali           */

void warn(char *mesg)

{
  printf(mesg);
}

/* banner - routine per segnalazioni "vistose"                  */

void banner(char *mesg)

{
  printf("\n********************************************************\n");
  printf("* %s\n", mesg);
  printf("********************************************************\n\n");
}

/* findmod - accetta in input una linea di un file .pag         */
/* che descrive un attributo di modulo o di porta               */
/* Se il modulo (o il modulo a cui la porta appartiene)         */
/* esiste gia', restituisce l'indice dello stesso in            */
/* modlist, altrimenti ritorna -1                               */
/*                                                              */
/* viene utilizzato nella fase di scansione del .pag,           */
/* perche' un modulo puo` essere definito prima o dopo le       */
/* porte che gli appartengono, e bisogna sapere se              */
/* allocarlo o no                                               */

int findmod(char *line)

{
  char nomemod[MODPAGNAMENCH + 1];
  int ret, i, ml;
  char *wstr;

  wstr = strchr(line, 'w');
  ml = strlen(line) - strlen(wstr);
  strncpy(nomemod, line + 1, ml);
  nomemod[ml] = '\0';

  ret = -1;

  for (i = 0; i < modnum; i++)
    if (strcmp(nomemod, modlist[i]->modpname) == 0)
      ret = i;

  if (ret == -1) /* modulo non trovato: e` nuovo */
    ret = modnum;

  return (ret);
}

/* findmod4mvname - accetta in input un nome completo           */
/* di variabile matematica.                                     */
/* Restituisce l'indice del modulo di provenienza               */
/* originale della stessa (-1 se non viene trovato)             */
/*                                                              */
/* viene usata nella fase di compilazione del f01.dat,          */
/* per sapere da quale modulo provengono le variabili           */
/* che hanno sostituito quelle originali                        */

int findmod4mvname(char *mvfullname)

{
  int ret, m;

  ret = -1;

  for (m = 0; m < modnum; m++)
    if (strcmp(mvfullname + FULLMODNAMEPREFLEN,
               modlist[m]->fullmodname + FULLMODNAMEPREFLEN) == 0)
      ret = m;

  return (ret);
}

/* findm4p - accetta in input un descrittore di porta fisica    */
/* Restituisce l'indice in modlist del modulo a cui la porta    */
/* appartiene (-1 se non lo trova)                              */
/*                                                              */
/* viene usata ogni qualvolta si abbia l'informazione sulla     */
/* porta, e da questa si voglia risalire al suo modulo; infatti */
/* la topologia delle connessioni viene mantenuta non a livello */
/* di moduli, ma a livello di connessioni tra le singole porte  */

int findm4p(fpdesc *fpd)

{
  int m, p, ret;

  ret = -1;

  for (m = 0; m < modnum; m++)
    for (p = 0; p < modlist[m]->fpnum; p++)
      if (fpd == modlist[m]->fplist[p])
        ret = m;

  return (ret);
}

/* fpconnect - accetta in  input un descrittore di porta fisica */
/* e due nomi di porta di pagina.                               */
/* il campo connectport della prima porta viene riempito con il */
/* descrittore della seconda porta (corrisponde a effettuare    */
/* una connessione tra le due, con la prima come porta di       */
/* uscita e la seconda come porta di ingresso                   */
/*                                                              */
/* viene usata nella scansione del .pag, per memorizzare nella  */
/* struttura modlist la topologia delle connessioni del modello */

void fpconnect(fpdesc *fpptr, char *fp2name)

{
  fpdesc *fp2ptr;
  int m, p;

  fp2ptr = NULL;

  /* percorri modlist e fplist */

  for (m = 0; m < modnum; m++)
    for (p = 0; p < modlist[m]->fpnum; p++)
      if (strcmp(modlist[m]->fplist[p]->fppname, fp2name) == 0)
        fp2ptr = modlist[m]->fplist[p];

  if (fp2ptr != NULL) /* port2 esiste */
  {
    fp2ptr->connectport = fpptr;
    fpptr->connectport = fp2ptr;
    /*printf("port %s and %s connected\n", fpptr->fppname, fp2ptr->fppname);*/
  }
}

/* fpconnectTop - accetta in  input un descrittore di porta     */
/* fisica di ingresso, un nome di porta fisica di uscita e il   */
/* codice del blocco della porta di uscita.                     */
/* Il campo connectport della seconda porta viene riempito con  */
/* il descrittore della prima porta (corrisponde a effettuare   */
/* una connessione tra le due.                                  */
/*                                                              */
/* viene usata nella scansione del .top, per memorizzare nella  */
/* struttura modlist la topologia delle connessioni del modello */

void fpconnectTop(fpdesc *fping, char *fpoutname, char *modcod)

{
  int m, p;
  fpdesc *fpout;

  fpout = NULL;

  /* percorri modlist e fplist */

  for (m = 0; m < modnum; m++)
    for (p = 0; p < modlist[m]->fpnum; p++)
      if (strcmp(modlist[m]->fplist[p]->fpname, fpoutname) == 0 &&
          strcmp(modcod, modlist[m]->fullmodname + FULLMODNAMEPREFLEN) == 0)
        fpout = modlist[m]->fplist[p];

  if (fpout != NULL) /* portout esiste - sempre vero poiche' .top e` gia` stato letto */
  {
    fpout->connectport = fping;
    fping->connectport = fpout;
    printf("port %s and %s (%s) connected\n", fping->fpname, fpout->fpname, modcod);
  }
}

/* whatport - in input richiede un'indice di modulo e un nome   */
/* fisico di porta.                                             */
/* Restituisce il descrittore di porta associato ai due campi   */
/* sopra elencati (NULL se non lo trova)                        */
/*                                                              */
/* viene usata nella scansione degli I5, per verificare la      */
/* corrispondenza tra i moduli descritti dal .pag e quelli      */
/* descritti negli I5 (poiche' ambedue, per il momento, sono     */
/* scritti a mano)                                              */

fpdesc *whatport(int module, char *fpname)

{
  int p;
  fpdesc *ret;

  ret = NULL;
  for (p = 0; p < modlist[module]->fpnum; p++)
    if (strcmp(modlist[module]->fplist[p]->fpname, fpname) == 0)
      ret = modlist[module]->fplist[p];

  return (ret);
}

/* leftconnectport - restituisce la porta collegata ad una      */
/* porta di ingresso (NULL se non esiste)                       */
/*                                                              */
/* viene richiamata dalla follow2() e da isport3connected()     */

fpdesc *leftconnectport(fpdesc *fpd)

{
  int m, p;
  fpdesc *leftfpd;

  leftfpd = NULL;

  for (m = 0; m < modnum; m++)
    for (p = 0; p < modlist[m]->fpnum; p++)
      if (fpd == modlist[m]->fplist[p]->connectport)
        leftfpd = modlist[m]->fplist[p];

  return (leftfpd);
}

/* findmv4fv - questa funzione prende come input                */
/* un identificatore di modulo e un nome di variabile           */
/* fisica: restituisce l'id della variabile matematica          */
/* corrispondente                                               */
/*                                                              */
/* viene usata dalla replaceout2() per identificare la var.     */
/* matematica che deve essere sostituita nel modulo             */

int findmv4fv(int modid, char *fvname)

{
  char tmvn[MVNAMENCH + 1];
  int mv, ret, mst;

  ret = -1;

  strcpy(tmvn, fvname);

  mst = modlist[modid]->status;
  for (mv = 0; mv < modlist[modid]->mvnum; mv++)
    if (strcmp(tmvn, modlist[modid]->mvlist[mst][mv]->fullmvname) == 0)
      ret = mv;

  return (ret);
}

/* findmv4fvRelaxed - questa funzione prende come input         */
/* un identificatore di modulo e un nome di variabile           */
/* fisica: restituisce 1 se esiste una variabile matematica     */
/* con lo stesso nome in una qualsiasi configurazione           */
/*                                                              */
/* viene usata dalla follow2() per contare il numero di var.    */
/* matematiche attraversate (ed eventualmente suggerire l'uso   */
/* di nodo)                                                     */

int findmv4fvRelaxed(int modid, char *fvname)

{
  char tmvn[MVNAMENCH + 1];
  int mv, ret;

  ret = -1;

  strcpy(tmvn, fvname);

  for (mv = 0; mv < modlist[modid]->mvnum; mv++)
    if (strcmp(tmvn, modlist[modid]->mvlist[1][mv]->fullmvname) == 0)
      ret = mv;

  return (ret);
}

/* follow2 - accetta in input un descrittore di modulo, un      */
/* descrittore di porta fisica e una posizione di variabile     */
/* in un vettore di variabili fisiche                           */
/* Insegue le variabili della porta suddetta connesse a         */
/* dei 2, e riempie i vettori fol2buf e fol2pbuf con i valori   */
/* delle variabili trovate e con i descrittori delle porte      */
/* fisiche corrispondenti                                       */
/*                                                              */
/* viene usata nella fase III per l'analisi delle connessioni   */

void follow2(fpdesc *fromfp, int vpos)

{
  int p, m, v, val, valn, fromfptype, varispresent;
  moddesc *modnext, *recmod;
  fpdesc *fpcur, *fpnext, *recfp;
  char vnom[FVNAMENCH + 1];

  fromfptype = fromfp->fptype;

  if (fromfptype % 2 == 0)
    recfp = leftconnectport(fromfp);
  else
    recfp = fromfp->connectport;

  if (recfp == NULL)
    return;

  fol2midpbuf[fol2midbuf++] = recfp;

  m = findm4p(recfp);
  recmod = modlist[m];
  /*printf("follow2ing %s\n", recmod->fullmodname);*/
  strcpy(vnom, recfp->fvar[vpos]);

  if (findmv4fvRelaxed(m, vnom) != -1) /* la variabile e` matematica */
    nummatCrossed4nodo++;

  for (p = 0; p < recmod->fpnum; p++)
  {
    fpcur = recmod->fplist[p];
    if (fpcur->fptype % 2 == 0)
      fpnext = leftconnectport(fpcur);
    else
      fpnext = fpcur->connectport;

    if (fpcur != recfp &&
        (strcmp(fpcur->fvar[vpos], vnom) == 0))
      fol2midpbuf[fol2midbuf++] = fpcur;

    if (fpcur != recfp &&
        (strcmp(fpcur->fvar[vpos], vnom) == 0) &&
        fpnext != NULL) /* la porta e` connessa */
    {
      val = fpcur->cfnum[recmod->status][vpos];

      if (val == FVARTRANSINMAT) /* dovrebbe essere sempre vero */
      {
        m = findm4p(fpnext);
        modnext = modlist[m];

        valn = fpnext->cfnum[modnext->status][vpos];

        if (valn == FVARTRANSINMAT) /* la var. deve essere ritrasmessa */
        {
          follow2(fpcur, vpos);
        }
        else
        {
          fol2buf[fol2buf[0] + 1] = valn;
          fol2pbuf[fol2buf[0] + 1] = fpnext;
          strcpy(fol2vnbuf[fol2buf[0] + 1], fpnext->fvar[vpos]);
          fol2vtbuf[fol2buf[0] + 1] = (fpnext->fptype % 2 == 0) ? LIMIT2OUT : LIMIT2IN;

          if (findmv4fvRelaxed(m, fpnext->fvar[vpos]) != -1) /* la variabile e` matematica */
            nummatCrossed4nodo++;

          varispresent = 0;
          for (v = 1; v <= fol2buf[0]; v++)
            if (strcmp(fol2vnbuf[v], fol2vnbuf[fol2buf[0] + 1]) == 0) /* var gia presente */
              varispresent = 1;

          if (!varispresent)
          {
            printf("var is %d %s  -  ", fol2buf[fol2buf[0] + 1], fol2vnbuf[fol2buf[0] + 1]);
            fol2buf[0]++;

            fol2num3 += valn == FVARUNDMATROLE ? 1 : 0;
            fol2last3 = valn == FVARUNDMATROLE ? fol2buf[0] : fol2last3;
          }
        }
      }
      else
      {
        sprintf(tempstr, "broken pipe of 2 (%s-%s-%s should be 2)\n",
                recmod->fullmodname, fpcur->fpname, fpcur->fvar[vpos]);
        essit(tempstr);
      }
    }
  }
}

/* followL2 - accetta in input un descrittore di modulo, un     */
/* descrittore di porta fisica e una posizione di variabile     */
/* in un vettore di variabili fisiche                           */
/* Insegue le variabili della porta suddetta connesse a         */
/* dei 2, e riempie i vettori folL2buf e folL2pbuf con gli ID   */
/* dei moduli trovati (contenenti i limiti) e con i descrittori */
/* delle porte fisiche corrispondenti                           */
/*                                                              */
/* viene usata nella fase IV.b per la creazione della sottorete */
/* quando le sottoreti sono disgiunte da catene di 2            */

void followL2(fpdesc *fromfp, int vpos)

{
  int p, m, v, val, valn, fromfptype, varispresent;
  moddesc *modnext, *recmod;
  fpdesc *fpcur, *fpnext, *recfp;
  char vnom[FVNAMENCH + 1];

  fromfptype = fromfp->fptype;

  if (fromfptype % 2 == 0)
    recfp = leftconnectport(fromfp);
  else
    recfp = fromfp->connectport;

  if (recfp == NULL)
    return;

  m = findm4p(recfp);
  recmod = modlist[m];
  printf("followL2ing %s\n", recmod->fullmodname);
  strcpy(vnom, recfp->fvar[vpos]);

  for (p = 0; p < recmod->fpnum; p++)
  {
    fpcur = recmod->fplist[p];
    if (fpcur->fptype % 2 == 0)
      fpnext = leftconnectport(fpcur);
    else
      fpnext = fpcur->connectport;

    if (fpcur != recfp &&
        (strcmp(fpcur->fvar[vpos], vnom) == 0) &&
        fpnext != NULL) /* la porta e` connessa */
    {
      val = fpcur->cfnum[recmod->status][vpos];

      if (val == FVARTRANSINMAT) /* dovrebbe essere sempre vero */
      {
        m = findm4p(fpnext);
        modnext = modlist[m];

        valn = fpnext->cfnum[modnext->status][vpos];

        if (valn == FVARTRANSINMAT) /* la var. deve essere ritrasmessa */
        {
          followL2(fpcur, vpos);
        }
        else if (valn == FVARUNDMATROLE)
        {
          fol2Lbuf[fol2Lbuf[0] + 1] = m;
          fol2Lpbuf[fol2Lbuf[0] + 1] = fpnext;
          strcpy(fol2vnbuf[fol2Lbuf[0] + 1], fpnext->fvar[vpos]);

          varispresent = 0;
          for (v = 1; v <= fol2Lbuf[0]; v++)
            if (strcmp(fol2vnbuf[v], fol2vnbuf[fol2Lbuf[0] + 1]) == 0) /* var gia presente */
              varispresent = 1;

          if (!varispresent)
          {
            /*printf("Lvar is %d ....................%s\n", fol2Lbuf[fol2Lbuf[0]+1], fol2vnbuf[fol2Lbuf[0]+1]);*/
            fol2Lbuf[0]++;
          }
        }
      }
      else
      {
        sprintf(tempstr, "broken pipe of 2 (%s-%s-%s should be 2)\n",
                recmod->fullmodname, fpcur->fpname, fpcur->fvar[vpos]);
        essit(tempstr);
      }
    }
  }
}

/* followLC2 - accetta in input un descrittore di modulo, un    */
/* descrittore di porta fisica e una posizione di variabile     */
/* in un vettore di variabili fisiche                           */
/* Insegue le variabili della porta suddetta connesse a         */
/* dei 2, e riempie i vettori fol2buf e fol2pbuf con i valori   */
/* delle variabili trovate e con i descrittori delle porte      */
/* fisiche corrispondenti                                       */
/*                                                              */
/* viene usata nella fase IV.b per il controllo delle catene    */
/* di 2 durante la checkLconf()                                 */

void followLC2(fpdesc *fromfp, int vpos)

{
  int p, m, v, val, valn, fromfptype, varispresent;
  moddesc *modnext, *recmod;
  fpdesc *fpcur, *fpnext, *recfp;
  char vnom[FVNAMENCH + 1];

  fromfptype = fromfp->fptype;

  if (fromfptype % 2 == 0)
    recfp = leftconnectport(fromfp);
  else
    recfp = fromfp->connectport;

  if (recfp == NULL)
    return;

  fol2midpbuf[fol2midbuf++] = recfp;

  m = findm4p(recfp);
  recmod = modlist[m];
  /*printf("followLC2ing %s\n", recmod->fullmodname);*/
  strcpy(vnom, recfp->fvar[vpos]);

  for (p = 0; p < recmod->fpnum; p++)
  {
    fpcur = recmod->fplist[p];
    if (fpcur->fptype % 2 == 0)
      fpnext = leftconnectport(fpcur);
    else
      fpnext = fpcur->connectport;

    if (fpcur != recfp &&
        (strcmp(fpcur->fvar[vpos], vnom) == 0))
      fol2midpbuf[fol2midbuf++] = fpcur;

    if (fpcur != recfp &&
        (strcmp(fpcur->fvar[vpos], vnom) == 0) &&
        fpnext != NULL) /* la porta e` connessa */
    {
      if (recmod->cfLtreeslot == ISNOTINSUBNET)
        val = fpcur->cfnum[recmod->status][vpos];
      else
        val = fpcur->cfnum[confLnet[recmod->cfLtreeslot]][vpos];

      if (val == FVARTRANSINMAT) /* dovrebbe essere sempre vero */
      {
        m = findm4p(fpnext);
        modnext = modlist[m];

        if (modnext->cfLtreeslot == ISNOTINSUBNET)
          valn = fpnext->cfnum[modnext->status][vpos];
        else
          valn = fpnext->cfnum[confLnet[modnext->cfLtreeslot]][vpos];

        if (valn == FVARTRANSINMAT) /* la var. deve essere ritrasmessa */
        {
          followLC2(fpcur, vpos);
        }
        else
        {
          fol2buf[fol2buf[0] + 1] = valn;
          fol2pbuf[fol2buf[0] + 1] = fpnext;
          strcpy(fol2vnbuf[fol2buf[0] + 1], fpnext->fvar[vpos]);
          fol2vtbuf[fol2buf[0] + 1] = (fpnext->fptype % 2 == 0) ? LIMIT2OUT : LIMIT2IN;

          varispresent = 0;
          for (v = 1; v <= fol2buf[0]; v++)
            if (strcmp(fol2vnbuf[v], fol2vnbuf[fol2buf[0] + 1]) == 0) /* var gia presente */
              varispresent = 1;

          if (!varispresent)
          {
            printf("var is %d %s  -  ", fol2buf[fol2buf[0] + 1], fol2vnbuf[fol2buf[0] + 1]);
            fol2buf[0]++;

            fol2num3 += valn == FVARUNDMATROLE ? 1 : 0;
            fol2last3 = valn == FVARUNDMATROLE ? fol2buf[0] : fol2last3;
          }
        }
      }
      else
      {
        sprintf(tempstr, "broken pipe of 2 (%s-%s-%s should be 2)\n",
                recmod->fullmodname, fpcur->fpname, fpcur->fvar[vpos]);
        essit(tempstr);
      }
    }
  }
}

/* checksinglecfg - controlla che il contenuto di curcfnum per  */
/* il modulo passato sia compatibile con la configurazione cfg  */
/* Restituisce CFTREE_CFG2TEST se e' compatibile (significa che */
/* la configurazione sara' testata nella fase di checkconf() ), */
/* CFTREE_CFGREJECTED altrimenti                                */
/*                                                              */
/* viene usata nella fase IVa per scartare le configurazioni di */
/* blocco non valide                                            */

int checksinglecfg(int modid, int cfg)

{
  int v, p, fpt, val, cval, ret;

  ret = CFTREE_CFG2TEST;

  for (p = 0; p < modlist[modid]->fpnum; p++)
  {
    fpt = modlist[modid]->fplist[p]->fptype;

    for (v = 0; v < fpvnum[fpt]; v++)
    {
      cval = modlist[modid]->fplist[p]->curcfnum[v];
      val = modlist[modid]->fplist[p]->cfnum[cfg][v];

      if ((cval == FVAREQMATROLE && val == FVAROPPMATROLE) ||
          (cval == FVAROPPMATROLE && val == FVAREQMATROLE))
        ret = CFTREE_CFGREJECTED;
    }
  }

  return (ret);
}

/* checkconf - verifica che le configurazioni dei moduli da     */
/* configurare matematicamente in maniera automatica, contenute */
/* nel vettore confnet, formino una "rete" di moduli valida     */
/* (senza connessioni proibite)                                 */
/* Restituisce CFG_GOOD se la config. corrente e` valida,        */
/* CFG_NOGOOD altrimenti                                        */
/*                                                              */
/* viene richiamata dalla checktree()                           */

int checkconf()

{
  int cret;
  int m, p, fpt, mI, v, tv;
  int fpsO, fpsI, numO, numI;
  fpdesc *fpdO, *fpdI;

  cret = CFG_GOOD;

  for (m = 0; m < modnum; m++)
  {
    for (p = 0; p < modlist[m]->fpnum; p++)
    {
      fpdO = modlist[m]->fplist[p];
      fpdI = modlist[m]->fplist[p]->connectport;
      fpt = modlist[m]->fplist[p]->fptype;

      if ((fpt % 2 == 1) && fpdI != NULL)
      {
        mI = findm4p(fpdI);
        if (modlist[m]->status == SYSTEM_CONF)
          fpsO = confnet[modlist[m]->cftreeslot];
        else
          fpsO = modlist[m]->status;

        if (modlist[mI]->status == SYSTEM_CONF)
          fpsI = confnet[modlist[mI]->cftreeslot];
        else
          fpsI = modlist[mI]->status;

        /*printf("outing module-port-status: %s-%s %d   inning port-status: %s-%s %d\n",
        modlist[m]->modpname,fpdO->fpname,fpsO,modlist[mI]->modpname,fpdI->fpname,fpsI);*/

        for (v = 0; v < fpvnum[fpt]; v++)
        {
          numO = fpdO->cfnum[fpsO][v];
          numI = fpdI->cfnum[fpsI][v];
          /*printf("Ovv: %s %d  Ivv: %s %d\n",
          fpdO->fvar[v],numO,fpdI->fvar[v],numI);*/

          if ((numO == FVAREQMATROLE && numI == FVAROPPMATROLE) ||
              (numO == FVAROPPMATROLE && numI == FVAREQMATROLE))
          {
            cret = CFG_NOGOOD;
          }

          if (numI == FVARTRANSINMAT)
          {
            fol2buf[0] = 0;
            follow2(fpdO, v);

            /* ora in fol2buf c'e` la lista delle variabili collegate via 2 */
            /*printf("found %d trans.ble vars\n", fol2buf[0]);*/
            for (tv = 1; tv < fol2buf[0] + 1; tv++)
            {
              if ((numO == FVAREQMATROLE && fol2buf[tv] == FVAROPPMATROLE) ||
                  (numO == FVAROPPMATROLE && fol2buf[tv] == FVAREQMATROLE))
              {
                cret = CFG_NOGOOD;
              }
            }
          }
        }
      }
    }

    if (cret == CFG_NOGOOD)
      break;
  }

  return (cret);
}

/* checktree - "esplode" le possibili combinazioni ottenibili   */
/* dalle configurazioni dei moduli da configurare in automatico */
/* percorrendo l'albero formato dalle stesse in maniera         */
/* ricorsiva.                                                   */
/* Ogni volta che arriva al "fondo" (riempiendo il vettore      */
/* confnet) richiama la checkconf() per il controllo            */
/*                                                              */
/* viene usata nella fase IV.c                                  */

void checktree(int depth)

{
  int cfpunt, cfts, nret, i;
  moddesc *mod;

  cfpunt = 0;

  if (autocfgOK == CFG_GOOD)
    return;

  if (depth < cftreecount - 1) /* la rete e` incompleta: vai piu` a fondo */
  {
    mod = modlist[conftree[depth][CFTREE_MODID]];

    for (cfpunt = 1; cfpunt < mod->numconfig + 1; cfpunt++)
    {
      if (conftree[depth][cfpunt] == CFTREE_CFG2TEST) /* la config. non e` stata rifiutata */
      {
        confnet[depth] = cfpunt;
        checktree(depth + 1);
      }

      if (autocfgOK == CFG_GOOD)
        break;
    }
  }
  else
  {
    mod = modlist[conftree[depth][CFTREE_MODID]];

    for (cfts = 1; cfts < mod->numconfig + 1; cfts++)
    {
      if (conftree[depth][cfts] == CFTREE_CFG2TEST) /* la config. non e` stata rifiutata */
      {
        confnet[depth] = cfts;
        nret = checkconf();
        printf("checked aConf - ");
        for (i = 0; i < cftreecount; i++)
          printf(" %d", confnet[i]);
        printf("\n");
      }

      if (nret == CFG_GOOD)
      {
        autocfgOK = CFG_GOOD;
        break;
      }
    }
  }
}

/* isport3connected - se la porta passata come input ha un      */
/* collegamento 3-3 con la porta connessa, ritorna 1,           */
/* altrimenti 0                                                 */
/*                                                              */
/* viene richiamata dalla fillLtree()                           */

int isport3connected(fpdesc *fpd)

{
  int ret, fv, v, mod, modst, cmod, cmodst;
  int val1, val2;
  fpdesc *cfpd;

  ret = 0;
  fv = fpvnum[fpd->fptype];
  mod = findm4p(fpd);
  modst = modlist[mod]->status;

  cfpd = (fpd->fptype % 2 == 0) ? leftconnectport(fpd) : fpd->connectport;

  if (cfpd != NULL)
  {
    cmod = findm4p(cfpd);
    cmodst = modlist[cmod]->status;

    for (v = 0; v < fv; v++)
    {
      if (modst == SYSTEM_CONF)
        val1 = fpd->curcfnum[v];
      else
        val1 = fpd->cfnum[modst][v];

      if (cmodst == SYSTEM_CONF)
        val2 = cfpd->curcfnum[v];
      else
        val2 = cfpd->cfnum[cmodst][v];

      if (val1 == FVARUNDMATROLE && val2 == FVARUNDMATROLE)
        ret = 1;
    }
  }

  return (ret);
}

/* findmodconnwithport - ritorna l'id del modulo connesso con   */
/* la porta data come input, -1 se la porta non e` connessa      */
/*                                                              */
/* chiamata dalla fillLtree()                                   */

int findmodconnwithport(fpdesc *fpd)

{
  int ret, fpt, m, p, leftmod;
  fpdesc *leftfpd;

  ret = -1;

  fpt = fpd->fptype;

  if (fpt % 2 == 0)
  {
    leftfpd = NULL;
    leftmod = -1;

    for (m = 0; m < modnum; m++)
      for (p = 0; p < modlist[m]->fpnum; p++)
        if (fpd == modlist[m]->fplist[p]->connectport)
        {
          leftmod = m;
          leftfpd = modlist[m]->fplist[p];
        }

    if (leftfpd == NULL)
      return (ret);
    else
      return (leftmod);
  }
  else if (fpd->connectport == NULL)
    return (ret);
  else
    return (findm4p(fpd->connectport));
}

/* fillLtree - riempie confLtree con gli identificativi dei     */
/* moduli connessi in una sottorete, seguendo le varie catene   */
/* di porte non configurate in modo ricorsivo                   */
/* Il parametro level serve a gestire la ricorsione della       */
/* procedura: le catene di due vengono seguite al termine delle */
/* chiamate a livello zero                                      */
/*                                                              */
/* usata nella fase IV.b                                        */

void fillLtree(int modid, int level)

{
  int nextmod, p, v, k, mod1, mod2;
  int freezecfL2, mod, ms1, ms2, num1, num2;
  int mod2nd, m2nd1, m2nd2;
  fpdesc *fromfpd, *fpd1, *fpd2;
  int myfol2Lbuf[MAXMULTLINK]; /* buffer contenente gli ID dei moduli dei limiti della catena di 2 Lcorrente */

  for (p = 0; p < modlist[modid]->fpnum; p++)
  {
    fromfpd = modlist[modid]->fplist[p];

    if (isport3connected(fromfpd))
    {
      nextmod = findmodconnwithport(fromfpd);

      if (modlist[nextmod]->cfLtreeslot == ISNOTINSUBNET)
      {
        printf("Popping %s - Lslot %d\n", modlist[nextmod]->fullmodname, cfLtreecount);
        modlist[nextmod]->cfLtreeslot = cfLtreecount;
        for (k = 0; k < MAXNMATCFG; k++) /* copia le info di conftree in confLtree */
          confLtree[cfLtreecount][k] = conftree[modlist[nextmod]->cftreeslot][k];
        cfLtreecount++;

        fillLtree(nextmod, LEVELNOTTOP);
      }
    }
  }

  if (level == LEVELTOP)
  {
    freezecfL2 = cfLtreecount;

    for (mod = 0; mod < freezecfL2; mod++)
    {
      mod1 = confLtree[mod][CFTREE_MODID];

      for (p = 0; p < modlist[mod1]->fpnum; p++)
      {
        fpd1 = modlist[mod1]->fplist[p];
        fpd2 = (fpd1->fptype % 2 == 0) ? leftconnectport(fpd1) : fpd1->connectport;

        if (fpd2 != NULL)
        {
          mod2 = findm4p(fpd2);

          for (v = 0; v < fpvnum[fpd1->fptype]; v++)
          {
            ms1 = modlist[mod1]->status;
            if (ms1 == SYSTEM_CONF)
              num1 = fpd1->curcfnum[v];
            else
              num1 = fpd1->cfnum[ms1][v];

            ms2 = modlist[mod2]->status;
            if (ms2 == SYSTEM_CONF)
              num2 = fpd2->curcfnum[v];
            else
              num2 = fpd2->cfnum[ms2][v];

            if (num1 == FVARUNDMATROLE && num2 == FVARTRANSINMAT)
            {
              printf("Local chain beginning in %s %s\n", fpd1->fpname, fpd1->fvar[v]);
              /* follow2 */
              fol2Lbuf[0] = 0;
              followL2(fpd1, v);

              for (mod2nd = 0; mod2nd < fol2Lbuf[0] + 1; mod2nd++)
                myfol2Lbuf[mod2nd] = fol2Lbuf[mod2nd];

              printf("found %d Ltrans.ble vars\n", fol2Lbuf[0]);

              for (mod2nd = 0; mod2nd < myfol2Lbuf[0]; mod2nd++)
              {
                m2nd1 = myfol2Lbuf[mod2nd + 1];

                if (modlist[m2nd1]->cfLtreeslot == ISNOTINSUBNET)
                {
                  modlist[m2nd1]->cfLtreeslot = cfLtreecount;
                  m2nd2 = modlist[m2nd1]->cftreeslot;
                  printf("Adding %s - Lslot %d - slot %d\n", modlist[m2nd1]->fullmodname, cfLtreecount, m2nd2);

                  for (k = 0; k < MAXNMATCFG; k++) /* copia le info di conftree in confLtree */
                    confLtree[cfLtreecount][k] = conftree[m2nd2][k];
                  cfLtreecount++;

                  fillLtree(m2nd1, LEVELTOP);
                }
              }
            }
          }
        }
      }
    }
  }
}

/* fillWas3 - assegna 1 al campo was3 di ciascuna variabile     */
/* fisica se c'e` un 3 nella configurazione di sistema ed il     */
/* modulo deve essere ancora configurato, 0 altrimenti. Serve   */
/* alla checkLtree per vedere se la catena di 2 va              */
/* sumzero-verificata o no                                      */
/*                                                              */
/* chiamata nella fase IV.b                                     */

void fillWas3()

{
  int M, m, p, v;

  for (m = 0; m < modnum; m++)
    for (p = 0; p < modlist[m]->fpnum; p++)
      for (v = 0; v < fpvnum[modlist[m]->fplist[p]->fptype]; v++)
        modlist[m]->fplist[p]->was3[v] = 0;

  for (M = 0; M < cfLtreecount; M++)
  {
    m = confLtree[M][CFTREE_MODID];

    for (p = 0; p < modlist[m]->fpnum; p++)
      for (v = 0; v < fpvnum[modlist[m]->fplist[p]->fptype]; v++)
        if (modlist[m]->status == SYSTEM_CONF &&
            modlist[m]->fplist[p]->curcfnum[v] == FVARUNDMATROLE)
        {
          modlist[m]->fplist[p]->was3[v] = 1;
          /*printf("ping! %s was3\n", modlist[m]->fplist[p]->fvar[v]);*/
        }
  }
}

/* checkLconf - verifica che le configurazioni dei moduli da    */
/* configurare matematicamente in maniera automatica, contenute */
/* nel vettore confLnet, formino una "sottorete" di moduli      */
/* valida (senza connessioni proibite)                          */
/* Restituisce CFG_GOOD se la config. corrente e` valida,        */
/* CFG_NOGOOD altrimenti                                        */
/*                                                              */
/* viene richiamata dalla checkLtree()                          */

int checkLconf()

{
  int cret, ingusc2sum;
  int m, p, fpt, mI, v, tv;
  int fpsO, fpsI, numO, numI;
  fpdesc *fpdO, *fpdI;
  int mZ;

  cret = CFG_GOOD;

  for (m = 0; m < modnum; m++)
  {
    for (p = 0; p < modlist[m]->fpnum; p++)
    {
      fpdO = modlist[m]->fplist[p];
      fpdI = modlist[m]->fplist[p]->connectport;
      fpt = modlist[m]->fplist[p]->fptype;

      if ((fpt % 2 == 1) && fpdI != NULL)
      {
        mI = findm4p(fpdI);
        if (modlist[m]->cfLtreeslot != ISNOTINSUBNET)
          fpsO = confLnet[modlist[m]->cfLtreeslot];
        else
          fpsO = modlist[m]->status;

        if (modlist[mI]->cfLtreeslot != ISNOTINSUBNET)
          fpsI = confLnet[modlist[mI]->cfLtreeslot];
        else
          fpsI = modlist[mI]->status;

        printf("outing module-port-status: %s-%s %d   inning port-status: %s-%s %d\n",
               modlist[m]->modpname, fpdO->fpname, fpsO, modlist[mI]->modpname, fpdI->fpname, fpsI);

        for (v = 0; v < fpvnum[fpt]; v++)
        {
          numO = fpdO->cfnum[fpsO][v];
          numI = fpdI->cfnum[fpsI][v];
          printf("Ovv: %s %d  Ivv: %s %d\n",
                 fpdO->fvar[v], numO, fpdI->fvar[v], numI);

          if ((numO == FVAREQMATROLE && numI == FVAROPPMATROLE) ||
              (numO == FVAROPPMATROLE && numI == FVAREQMATROLE))
          {
            cret = CFG_NOGOOD;
          }
        }
      }
    }

    if (cret == CFG_NOGOOD)
      break;
  }

  if (cret != CFG_NOGOOD)
    for (m = 0; m < modnum; m++)
    {
      for (p = 0; p < modlist[m]->fpnum; p++)
      {
        fpt = modlist[m]->fplist[p]->fptype;
        fpdO = modlist[m]->fplist[p];

        if (fpt % 2 == 1)
          fpdI = modlist[m]->fplist[p]->connectport;
        else
          fpdI = leftconnectport(modlist[m]->fplist[p]);

        if (fpdI != NULL)
        {
          mI = findm4p(fpdI);

          if (modlist[m]->cfLtreeslot == ISNOTINSUBNET)
            fpsO = modlist[m]->status;
          else
            fpsO = confLnet[modlist[m]->cfLtreeslot];

          if (modlist[mI]->cfLtreeslot == ISNOTINSUBNET)
            fpsI = modlist[mI]->status;
          else
            fpsI = confLnet[modlist[mI]->cfLtreeslot];

          /*printf("outing? module-port-status: %s-%s %d   inning? module-port-status: %s-%s %d\n",
          modlist[m]->modpname,fpdO->fpname,fpsO,modlist[mI]->modpname,fpdI->fpname,fpsI);*/

          for (v = 0; v < fpvnum[fpt]; v++)
          {
            numO = fpdO->cfnum[fpsO][v];
            numI = fpdI->cfnum[fpsI][v];

            if (numO != FVARTRANSINMAT && numI == FVARTRANSINMAT)
            {
              chain2isLocal = 0;

              if (fpdO->fptype % 2 == 0)
                fol2vtbuf[1] = LIMIT2OUT;
              else
                fol2vtbuf[1] = LIMIT2IN;

              fol2num3 = numO == FVARUNDMATROLE ? 1 : 0;
              fol2last3 = numO == FVARUNDMATROLE ? 1 : -1;

              fol2midbuf = 0;
              fol2buf[1] = numO;
              fol2pbuf[1] = fpdO;
              strcpy(fol2vnbuf[1], fpdO->fvar[v]);
              printf("\nVar is %d %s  -  ", fol2buf[1], fol2vnbuf[1]);

              if (fpdO->was3[v])
                chain2isLocal = 1;

              fol2buf[0] = 1;
              followLC2(fpdO, v);
              /* now in fol2buf is the list of transmittable variables */

              printf("found %d trans.ble vars\n", fol2buf[0]);

              ingusc2sum = 0;
              for (tv = 1; tv < fol2buf[0] + 1; tv++)
              {
                if ((fol2buf[tv] == FVAREQMATROLE && fol2vtbuf[tv] == LIMIT2IN) ||
                    (fol2buf[tv] == FVAROPPMATROLE && fol2vtbuf[tv] == LIMIT2OUT))
                  ingusc2sum++;

                if (fol2pbuf[tv]->was3[v])
                  chain2isLocal = 1;
              }
              printf("ingusc2sum = %d  fol2midpn=%d", ingusc2sum, fol2midbuf);

              for (tv = 0; tv < fol2midbuf; tv++)
              {
                printf(" Ex3- %s  ", fol2midpbuf[tv]->fvar[v]);
                if (fol2midpbuf[tv]->was3[v])
                  chain2isLocal = 1;
              }

              if (ingusc2sum > 1)
                cret = CFG_NOGOOD;
              else if (ingusc2sum == 0 && chain2isLocal)
              {
                cret = CFG_NOGOOD;
                localZerosum = 1;
                printf("This chain has sum 0!\n");

                /* copy confLnet in zerosumLnet */

                for (mZ = 0; mZ < cfLtreecount; mZ++)
                  zerosumLnet[mZ] = confLnet[mZ];
              }
            }
          }
        }
      }
    }

  return (cret);
}

/* checkLtree - "esplode" le possibili combinazioni ottenibili  */
/* dalle configurazioni dei moduli da configurare in automatico */
/* percorrendo l'albero formato dalle stesse in maniera         */
/* ricorsiva.                                                   */
/* Ogni volta che arriva al "fondo" (riempiendo il vettore      */
/* confLnet) richiama la checkLconf() per il controllo          */
/*                                                              */
/* viene usata nella fase IV.b                                  */

void checkLtree(int depth)

{
  int cfpunt, cfts, nret, i;
  moddesc *mod;

  cfpunt = 0;
  nret = CFG_NOGOOD;

  if (autocfgLOK == CFG_GOOD)
    return;

  if (depth < cfLtreecount - 1) /* la rete e` incompleta: vai piu` a fondo */
  {
    mod = modlist[confLtree[depth][CFTREE_MODID]];

    for (cfpunt = 1; cfpunt < mod->numconfig + 1; cfpunt++)
    {
      if (confLtree[depth][cfpunt] == CFTREE_CFG2TEST) /* la config. non e` stata rifiutata */
      {
        confLnet[depth] = cfpunt;
        checkLtree(depth + 1);
      }

      if (autocfgLOK == CFG_GOOD)
        break;
    }
  }
  else
  {
    mod = modlist[confLtree[depth][CFTREE_MODID]];

    for (cfts = 1; cfts < mod->numconfig + 1; cfts++)
    {
      if (confLtree[depth][cfts] == CFTREE_CFG2TEST) /* la config. non e` stata rifiutata */
      {
        localZerosum = 0;
        confLnet[depth] = cfts;
        nret = checkLconf();
        if (localZerosum && !foundaZerosum)
          foundaZerosum = 1;
        printf("\n*****\nchecked aLConf - ");
        for (i = 0; i < cfLtreecount; i++)
          printf(" %d", confLnet[i]);
        printf("\n");
        printf("*****\n\n");
      }

      if (nret == CFG_GOOD)
      {
        autocfgLOK = CFG_GOOD;
        break;
      }
    }
  }
}

/* replaceout2 - sostituisce nel modulo mod le variabili        */
/* in uscita di tipo 2 indicate da oldname con un valore che    */
/* dipende dal tipo di porta e con newname (viene da modfrom)   */
/* Restituisce il numero di 2 obliterati                        */
/*                                                              */
/* viene usata nella fase V                                     */

int replaceout2(int mod, char *newname, char *oldname)

{
  int p, pt, v, st, mv, ret;
  fpdesc *fpd;

  ret = 0;
  st = modlist[mod]->status;
  mv = findmv4fv(mod, oldname);

  for (p = 0; p < modlist[mod]->fpnum; p++)
  {
    fpd = modlist[mod]->fplist[p];
    pt = fpd->fptype;

    for (v = 0; v < fpvnum[pt]; v++)
      if (strcmp(oldname, fpd->fvar[v]) == 0 &&
          fpd->cfnum[st][v] == FVARTRANSINMAT)
      {
        strcpy(fpd->fvar[v], newname);

        if (fpd->fptype % 2 == 0)
          fpd->cfnum[st][v] = FVAROPPMATROLE;
        else
          fpd->cfnum[st][v] = FVAREQMATROLE;

        if (mv != -1) /* var. matematica trovata */
        {
          ret++;

          if (modlist[mod]->mvlist[st][mv]->status == MVSTATUS_ORIG)
            strcpy(modlist[mod]->mvlist[st][mv]->subconn, modlist[mod]->mvlist[st][mv]->fullmvname);
          sprintf(modlist[mod]->mvlist[st][mv]->fullmvname,
                  "%s", newname);
          modlist[mod]->mvlist[st][mv]->status = MVSTATUS_SUBST;

          printf("rep2out: %s in %s %s substituted with %s %d\n", oldname,
                 modlist[mod]->fullmodname, fpd->fpname, modlist[mod]->mvlist[st][mv]->fullmvname, fpd->cfnum[st][v]);
        }
        else
        {
          ret++;
          printf("repl2outNOMV: %s in %s substituted with %s %d\n", oldname,
                 modlist[mod]->fullmodname, fpd->fvar[v], fpd->cfnum[st][v]);
        }
      }
  }

  return (ret);
}

/* replaceout1 - sostituisce in tutti i moduli la variabile     */
/* indicata da oldname (nome completo matematico) con newname,  */
/* che proviene dal modulo modfrom                              */
/* Restituisce il numero di sostituzioni                        */
/*                                                              */
/* viene usata nella fase VI                                    */

int replaceout1(char *newname, char *oldname)

{
  int ret, m, mv, mst, p, fv, nfv;
  fpdesc *fpd;
  mvdesc *mvd;

  ret = 0;

  for (m = 0; m < modnum; m++)
  {
    mst = modlist[m]->status;

    for (p = 0; p < modlist[m]->fpnum; p++)
    {
      fpd = modlist[m]->fplist[p];
      nfv = fpvnum[fpd->fptype];

      for (fv = 0; fv < nfv; fv++)
        if (strcmp(fpd->fvar[fv], oldname) == 0)
        {
          printf("repl1NOMV: %s - %s <- %s\n", oldname, modlist[m]->fullmodname, newname);
          strcpy(fpd->fvar[fv], newname);
        }
    }

    for (mv = 0; mv < modlist[m]->mvnum; mv++)
    {
      mvd = modlist[m]->mvlist[mst][mv];

      if (strcmp(mvd->fullmvname, oldname) == 0)
      {
        printf("repl1: %s - %s <- %s\n", oldname, modlist[m]->fullmodname, newname);
        if (mvd->status == MVSTATUS_ORIG)
          strcpy(mvd->subconn, mvd->fullmvname);
        strcpy(mvd->fullmvname, newname);
        mvd->status = MVSTATUS_SUBST;
        ret++;
      }
    }
  }
  return (ret);
}

/* subst3 - verifica se un 3 puo` venire sostituito, ed esegue   */
/* l'operazione, modificando anche i 2 corrispondenti di ogni   */
/* configurazione.                                              */
/*                                                              */
/* viene usata nelle fasi III.b e III.c                         */

void subst3(moddesc *mod, fpdesc *fpd, int vpos, int newval)

{
  int ncfg, nfp, fpt, fpt_in, fpt2, thereis2, cf, p;
  fpdesc *fpd2;

  ncfg = mod->numconfig;
  nfp = mod->fpnum;
  fpt = fpd->fptype;
  fpt_in = fpt - (fpt % 2); /*numero pari che definisce l'identificativo
                              della porta fisica d'ingresso del tipo di
                              connettore del modulo puntato da fpd */

  thereis2 = 0;

  for (cf = 1; cf < ncfg + 1; cf++)
    if (fpd->cfnum[cf][vpos] == FVARTRANSINMAT)
      thereis2 = 1;

  if (!thereis2)
  {
    fpd->curcfnum[vpos] = newval;
    return;
  }

  for (cf = 1; cf < ncfg + 1; cf++)
    if (fpd->cfnum[cf][vpos] == FVARTRANSINMAT)
    {
      if ((fpt % 2 == 0 && newval == FVAREQMATROLE) ||
          (fpt % 2 == 1 && newval == FVAROPPMATROLE))
      {
        fpd->curcfnum[vpos] = newval;
        fpd->cfnum[cf][vpos] = newval;
        for (p = 0; p < nfp; p++)
          if (mod->fplist[p] != fpd)
          {
            fpd2 = mod->fplist[p];
            fpt2 = fpd2->fptype;
            if (fpt2 == fpt_in)
            {
              fpd2->cfnum[cf][vpos] = FVAROPPMATROLE;
            }
            else if (fpt2 == fpt_in + 1)
            {
              fpd2->cfnum[cf][vpos] = FVAREQMATROLE;
            }
          }
      }
      else if (nfp == 2)
      {
        if (fpt % 2 == 0 && newval == FVAROPPMATROLE)
        {
          fpd->curcfnum[vpos] = newval;
          fpd->cfnum[cf][vpos] = newval;
          fpd2 = (mod->fplist[0] == fpd) ? mod->fplist[1] : mod->fplist[0];
          fpt2 = fpd2->fptype;
          if (fpt2 == fpt_in || fpt2 == fpt_in + 1) /*stesso tipo di connettore*/
          {
            fpd2->cfnum[cf][vpos] = newval;
          }
        }
      }
      else if (fpt % 2 == 1 && newval == FVAREQMATROLE)
      {
        fpd->curcfnum[vpos] = newval;
        fpd->cfnum[cf][vpos] = newval;
        fpd2 = (mod->fplist[0] == fpd) ? mod->fplist[1] : mod->fplist[0];
        fpt2 = fpd2->fptype;
        if (fpt2 == fpt_in || fpt2 == fpt_in + 1) /*stesso tipo di connettore*/
        {
          fpd2->cfnum[cf][vpos] = newval;
        }
      }
    }
}

/* addDISN - nel caso che un modulo, dopo tutte le sostituzioni */
/* abbia due o piu` variabili di ingresso con lo stesso nome,    */
/* questa funzione aggiunge un modulo non fisico DISN, e lo     */
/* configura nel modo seguente:                                 */
/*   il suo unico ingresso viene sostituito dalla variabile     */
/*   multi-presente;                                            */
/*   le sue uscite vengono poste nel modulo incriminato al      */
/*   degli ingressi originali.                                  */
/*                                                              */
/* viene usata nella fase VI.b                                  */

void addDISN(int modid, char *mvname, int numocc)

{
  char handin_name[MVNAMENCH + 2];  /* +2: snprintf "U_%1d_M%03d" = 8 chars + null, 1 byte spare per GCC */
  int mv, mvv, mst;

  mst = modlist[modid]->status;

  if ((modlist[modnum] = (moddesc *)malloc(sizeof(moddesc))) == NULL)
    essit("malloc failed in DISN allocation\n");

  sprintf(tempstr, "DISNM%03d", disnnum);
  strcpy(modlist[modnum]->fullmodname, tempstr);

  strcpy(modlist[modnum]->modcomment, "non-physical module");
  modlist[modnum]->status = SYSTEM_CONF;

  mvv = 0;
  for (mv = 0; mv < numocc; mv++)
  {
    if ((modlist[modnum]->mvlist[SYSTEM_CONF][mv] = (mvdesc *)malloc(sizeof(mvdesc))) == NULL)
      essit("malloc failed in mvdesc-DISN allocation\n");

    snprintf(handin_name, sizeof(handin_name), "U_%1d_M%03d", (mv + 1) % 10, (disnnum) % 1000);

    strcpy(modlist[modnum]->mvlist[SYSTEM_CONF][mv]->fullmvname, handin_name);
    strcpy(modlist[modnum]->mvlist[SYSTEM_CONF][mv]->mvtype, "UA");
    strcpy(modlist[modnum]->mvlist[SYSTEM_CONF][mv]->mvcomment, "dummy output variable");
    modlist[modnum]->mvlist[SYSTEM_CONF][mv]->status = MVSTATUS_ORIG;

    while (strcmp(modlist[modid]->mvlist[mst][mvv++]->fullmvname, mvname) != 0 && mvv < MAXNMATVAR)
      ;

    if (modlist[modid]->mvlist[mst][mvv - 1]->status == MVSTATUS_ORIG)
      strcpy(modlist[modid]->mvlist[mst][mvv - 1]->subconn, modlist[modid]->mvlist[mst][mvv - 1]->fullmvname);

    strcpy(modlist[modid]->mvlist[mst][mvv - 1]->fullmvname, handin_name);
    modlist[modid]->mvlist[mst][mvv - 1]->status = MVSTATUS_SUBST;
  }

  if ((modlist[modnum]->mvlist[SYSTEM_CONF][numocc] = (mvdesc *)malloc(sizeof(mvdesc))) == NULL)
    essit("malloc failed in mvdesc-DISN allocation\n");

  strcpy(modlist[modnum]->mvlist[SYSTEM_CONF][numocc]->fullmvname, mvname);
  sprintf(modlist[modnum]->mvlist[SYSTEM_CONF][numocc]->subconn, "AINGM%03d", disnnum);
  strcpy(modlist[modnum]->mvlist[SYSTEM_CONF][numocc]->mvtype, "IN");
  strcpy(modlist[modnum]->mvlist[SYSTEM_CONF][numocc]->mvcomment, "dummy input variable");
  modlist[modnum]->mvlist[SYSTEM_CONF][numocc]->status = MVSTATUS_SUBST;

  modlist[modnum]->mvnum = numocc + 1;

  disnnum++;
  modnum++;
}

/* buildTop - nel caso che il programma sia eseguito con        */
/* l'opzione -p (cioe` leggendo un file .pag), dopo la lettura  */
/* del file viene scritto un file equivalente in formato .top.  */
/* Viene usata nella fase I.a                                   */

void buildTop(char *pagname)

{
  FILE *topf;
  fpdesc *fpd, *fpdcon;
  char topname[COMMENTNCH];
  int m, p, ln, mcon;

  /* apri .top */

  strcpy(topname, pagname);
  ln = strlen(topname);
  topname[ln - 3] = 't';
  topname[ln - 2] = 'o';
  topname[ln - 1] = 'p';

  printf("creating %s...\n", topname);
  if ((topf = fopen(topname, "w")) == NULL)
  {
    sprintf(tempstr, "cannot open %s file\n", topname);
    warn(tempstr);
    return;
  }

  /* scarica prima parte informativa sui moduli */

  for (m = 0; m < modnum; m++)
  {
    fprintf(topf, "%s\n%s\n\n",
            modlist[m]->modname,
            modlist[m]->fullmodname + FULLMODNAMEPREFLEN);
  }
  fprintf(topf, "****\n");

  /* scarica informazioni su moduli e porte */

  for (m = 0; m < modnum; m++)
  {
    fprintf(topf, "%s\n%s\n",
            modlist[m]->modname,
            modlist[m]->fullmodname + FULLMODNAMEPREFLEN);

    /* scarica info su porte d'ingresso */

    for (p = 0; p < modlist[m]->fpnum; p++)
    {
      fpd = modlist[m]->fplist[p];

      if (fpd->fptype % 2 == 1) /* porta uscita */
        fprintf(topf, "%s %d\n", fpd->fpname, fpd->fptype);
    }

    /* scarica info su porte d'uscita */

    for (p = 0; p < modlist[m]->fpnum; p++)
    {
      fpd = modlist[m]->fplist[p];

      if (fpd->fptype % 2 == 0) /* porta ingresso */
      {
        fpdcon = leftconnectport(fpd);

        if (fpdcon == NULL)
          fprintf(topf, "%s %d __\n", fpd->fpname, fpd->fptype);
        else
        {
          mcon = findm4p(fpdcon);
          fprintf(topf, "%s %d %s %s\n", fpd->fpname, fpd->fptype,
                  fpdcon->fpname, modlist[mcon]->fullmodname + FULLMODNAMEPREFLEN);
        }
      }
    }

    fprintf(topf, "****\n");
  }

  fclose(topf);
}

/*                                                              */
/*                      MAIN program                            */
/*                                                              */

int main(int argc, char **argv)

{
  FILE *fppag, *fptop, *fpI5, *fpf01;
  char linebuf[MAXLINELENGTH], linehead[MAXLINELENGTH], oldline[MAXLINELENGTH];
  //  char answer[MAXLINELENGTH]
  char *lbuf, i5fname[MAXLINELENGTH * 2], partmodname[MODNAMENCH + 1];
  char *tpstr, fpcandname[FPPAGNAMENCH + 1], fpnam[FPNAMENCH + 1], tagpag[TAGPAGNCH + 1];
  char tmvname[MVNAMENCH + 1], tmvtype[MVTYPENCH + 1];
  char i5path_bounded[MAXLINELENGTH]; /* copia di I5PATH con dimensione nota per analisi GCC */
  char mvname1[MVNAMENCH + 1], mvname2[MVNAMENCH + 1];
  int modcur, typel, itlen, itmlen, itplen, ownermod, k;
  fpdesc *fpcurptr, *fpd, *fpdI, *fpdO;
  int fpsO, fpsI, numO, numI, ingusc2sum;
  mvdesc *mvcur;
  int c, m, mI, p, nfp, ncfg, cf, v, mv, tv, fpt, c1, c2, cc, imm;
  int addm, num3, neqv, chkcfg, cfg2test, goodcfg;
  char tempmvname[MAXLINELENGTH], stafulmodname[MODNAMENCH + 1];
  int some2ispresent, some1ispresent;
  int mst, om;
  int mod, mm, cmod, erasable, erased;
  int cz, a, trmod;
  char curmvname[MVNAMENCH + 1], mvname[MVNAMENCH + 1];
  int curmvocc, mvv;

  /* controlla file lmf.dat con licenza */
#ifndef Linux
  if (LMFMANAGE())
    exit(1);
#endif
  /* legge il file dei connettori */

  read_conn();

  /* controlla il numero di argomenti */

  if (argc != 2 && argc != 3)
    essit("usage: pag2f01 [-p] <inputfile>\n");

  /* alcune inizializzazioni */

  modnum = 0;
  modcur = 0;

  /*                                              */
  /* Fase I.a                                     */
  /* leggi file .pag e riempi modlist (in parte)  */
  /*                                              */

  if (argc == 3) /* il file di input deve essere un .pag */
  {

    /* cerca opzione -p */

    if (strcmp(argv[1], "-p") != 0)
    {
      sprintf(tempstr, "option %s is not supported\n", argv[1]);
      essit(tempstr);
    }

    /* apri il file .pag */

    if ((fppag = fopen(argv[2], "r")) == NULL)
    {
      sprintf(tempstr, "cannot open %s file\n", argv[2]);
      essit(tempstr);
    }

    banner("Phase I - scan .pag");

    oldline[0] = '\0';
    while ((lbuf = fgets(linebuf, MAXLINELENGTH - 1, fppag)) != NULL)
    {
      tpstr = strchr(linebuf, '.');

      if (tpstr == NULL)
        typel = PAGLINE; /* attributi di pagina */
      else
      {
        itlen = strlen(linebuf) - strlen(tpstr);
        strncpy(linehead, linebuf, itlen);
        linehead[itlen] = '\0';

        if (strchr(linehead, 'c') != NULL) /* porta */
          typel = PORTLINE;
        else /* modulo */
          typel = MODLINE;
      }

      if (typel != PAGLINE)
      {
        if (strncmp(linebuf, oldline, itlen + 1) != 0)

        /* new item: alloc slot in portList or modList */
        {
          if (typel == MODLINE) /* modulo */
          {
            modcur = findmod(linebuf);

            if (modcur >= modnum) /* NUOVO modulo: allocalo */
            {
              if ((modlist[modcur] = (moddesc *)malloc(sizeof(moddesc))) == NULL)
                essit("malloc failed in moddesc allocation\n");

              tpstr = strchr(linebuf, 'w');
              itmlen = strlen(linebuf) - strlen(tpstr);
              strncpy(modlist[modcur]->modpname, linebuf + 1, itmlen);
              modlist[modcur]->modpname[itmlen] = '\0';

              modlist[modcur]->fpnum = 0;
              modlist[modcur]->mvnum = 0;
              modnum++;
              /*printf("allocated module %s (%d) from %s",modlist[modcur]->modpname, modcur, linebuf);*/
            }
          }
          else if (typel == PORTLINE) /* porta */
          {
            ownermod = findmod(linebuf);

            if (ownermod >= modnum) /* NUOVO modulo: allocalo */
            {
              if ((modlist[ownermod] = (moddesc *)malloc(sizeof(moddesc))) == NULL)
                essit("malloc failed in moddesc allocation\n");

              tpstr = strchr(linebuf, 'w');
              itmlen = strlen(linebuf) - strlen(tpstr);
              strncpy(modlist[ownermod]->modpname, linebuf + 1, itmlen);
              modlist[ownermod]->modpname[itmlen] = '\0';

              modlist[ownermod]->fpnum = 0;
              modlist[ownermod]->mvnum = 0;
              modnum++;
              /*printf("allocated module %s (%d) from %s",modlist[ownermod]->modpname, ownermod, linebuf);*/
            }

            /* alloca la struct fpdesc */

            if ((fpcurptr = (fpdesc *)malloc(sizeof(fpdesc))) == NULL)
              essit("malloc failed in fpdesc allocation\n");

            tpstr = strchr(linebuf, 'c');
            itplen = strlen(linebuf) - strlen(tpstr);
            strncpy(fpcurptr->fppname, linebuf + 1, itplen);
            fpcurptr->fppname[itplen] = '\0';
            /*printf("allocated port %s from %s",fpcurptr->fppname,linebuf);*/

            /* inserisci fpcurptr nella fplist del modulo di appartenenza */

            modlist[ownermod]->fplist[modlist[ownermod]->fpnum] = fpcurptr;
            modlist[ownermod]->fpnum++;
            /*printf("inserted port %s with %s module\n",fpcurptr->fppname,modlist[ownermod]->modpname);*/
          }
        }

        /* leggi gli attributi */

        if (typel == MODLINE) /* modulo */
        {
          if (strncmp(linebuf + itlen + 1, "modulName:", 10) == 0)
          {
            sscanf(linebuf, "%s %s", tempstr, modlist[modcur]->modname);
            strcpy(partmodname, modlist[modcur]->modname);

            for (k = 0; k < 6 - itlen; k++)
              strcat(partmodname, "0");

            strcat(partmodname, modlist[modcur]->modpname);
            partmodname[strlen(partmodname) - 1] = '\0';
            strcpy(modlist[modcur]->fullmodname, partmodname);
            printf("%s %s\n", modlist[modcur]->modname, modlist[modcur]->fullmodname + FULLMODNAMEPREFLEN);
          }
        }
        else if (typel == PORTLINE) /* porta */
        {
          if (strncmp(linebuf + itlen + 1, "portName:", 9) == 0)
          {
            sscanf(linebuf, "%s %s", tempstr, fpcurptr->fpname);
            /*printf("name of %s is %s\n",fpcurptr->fppname,fpcurptr->fpname);*/
          }

          if (strncmp(linebuf + itlen + 1, "portNameConnect:", 16) == 0)
          {
            fpcandname[0] = '\0';
            sscanf(linebuf, "%s %s", tempstr, fpcandname);
            if (strlen(fpcandname) > 0) /*trucco per evitare le porte nulle */
              fpconnect(fpcurptr, fpcandname);
          }

          if (strncmp(linebuf + itlen + 1, "tipoPort:", 9) == 0)
          {
            sscanf(linebuf, "%s %d", tempstr, &(fpcurptr->fptype));
            /*printf("tipoport of %s is %d\n", fpcurptr->fppname, fpcurptr->fptype);*/
          }
        }
      }
      else if (strncmp(linebuf, "*tagPag:", 8) == 0) /* tagpag */
      {
        sscanf(linebuf, "%s %s", tempstr, tagpag);
        /*printf("%s\n", tagpag);*/
      }

      strcpy(oldline, linebuf);
    }

    fclose(fppag);

    buildTop(argv[2]);
  }

  else

  /*                                              */
  /* Fase I.b                                     */
  /* leggi file .top e riempi modlist (in parte)  */
  /*                                              */

  {
    /* apri il file .top */

    if ((fptop = fopen(argv[1], "r")) == NULL)
    {
      sprintf(tempstr, "cannot open %s file\n", argv[1]);
      essit(tempstr);
    }

    banner("Phase I - scan .top");

    /* prima scansione e allocazione dei soli moduli */

    fscanf(fptop, "%s", tempstr);
    while (strcmp(tempstr, "****") != 0)
    {
      if ((modlist[modcur] = (moddesc *)malloc(sizeof(moddesc))) == NULL)
        essit("malloc failed in moddesc allocation\n");

      strcpy(modlist[modcur]->modname, tempstr);
      fscanf(fptop, "%s", partmodname);
      sprintf(tempstr, "%-4.4s%-4.4s", modlist[modcur]->modname, partmodname);
      strcpy(modlist[modcur]->fullmodname, tempstr);
      strcpy(modlist[modcur]->modpname, partmodname);
      modlist[modcur]->fpnum = 0;
      modlist[modcur]->mvnum = 0;

      fscanf(fptop, "%s", tempstr);
      modcur++;
      printf("%s\n", tempstr);
    }

    modnum = modcur;

    /* seconda scansione e allocazione delle porte */

    for (m = 0; m < modnum; m++)
    {
      fscanf(fptop, "%s", tempstr);
      fscanf(fptop, "%s", tempstr);

      fscanf(fptop, "%s", tempstr);
      while (strcmp(tempstr, "****") != 0)
      {
        if ((fpcurptr = (fpdesc *)malloc(sizeof(fpdesc))) == NULL)
          essit("malloc failed in fpdesc allocation\n");

        strcpy(fpcurptr->fpname, tempstr);
        strcpy(fpcurptr->fppname, "");
        modlist[m]->fplist[modlist[m]->fpnum] = fpcurptr;
        modlist[m]->fpnum++;
        fscanf(fptop, "%d", &(fpcurptr->fptype));

        if (fpcurptr->fptype % 2 == 0)
        {
          fscanf(fptop, "%s", tempstr);

          strcpy(fpcurptr->fpcpname, tempstr);
          if (strcmp(tempstr, "__") != 0)
          {
            fscanf(fptop, "%s", fpcurptr->fpcmodpn);
          }
        }

        fscanf(fptop, "%s", tempstr);
        printf("%s\n", tempstr);
      }
    }

    /* connessione delle porte */

    /* clean memory */
    for (m = 0; m < modnum; m++)
      for (p = 0; p < modlist[m]->fpnum; p++)
        modlist[m]->fplist[p]->connectport = NULL;

    for (m = 0; m < modnum; m++)
      for (p = 0; p < modlist[m]->fpnum; p++)
      {
        fpcurptr = modlist[m]->fplist[p];

        if (fpcurptr->fptype % 2 == 0 && strcmp(fpcurptr->fpcpname, "__") != 0)
          fpconnectTop(fpcurptr, fpcurptr->fpcpname, fpcurptr->fpcmodpn);
      }
  }

  /*                                              */
  /* Fase II                                      */
  /* leggi files I5 e finisci di riempire modlist */
  /*                                              */

  banner("Phase II - scan *.I5");

  //  I5PATH = getenv("FILESI5_PATH");
  I5PATH = getenv("LG_FILESI5");
  strncpy(i5path_bounded, I5PATH ? I5PATH : "", sizeof(i5path_bounded) - 1);
  i5path_bounded[sizeof(i5path_bounded) - 1] = '\0';

  for (m = 0; m < modnum; m++)
  {
    sprintf(tempstr, "%s", modlist[m]->modname);
    /* conversione in minuscolo */
    for (imm = 0; imm < strlen(tempstr); imm++)
      tempstr[imm] = (char)tolower(tempstr[imm]);

    snprintf(i5fname, sizeof(i5fname), "%s/%.*s.i5", i5path_bounded, MODNAMENCH, tempstr);

    /*printf("opening file %s\n", i5fname);*/
    if ((fpI5 = fopen(i5fname, "r")) == NULL)
    {
      snprintf(tempstr, sizeof(tempstr), "cannot open %s file\n", i5fname);
      essit(tempstr);
    }

    /* leggi .I5 */

    fscanf(fpI5, "%s\n", partmodname);

    lbuf = fgets(modlist[m]->modcomment, COMMENTNCH - 1, fpI5); /* leggi il commento del  modulo */
    modlist[m]->modcomment[strlen(modlist[m]->modcomment) - 1] = '\0';

    // Guag feb 2008 - test per i moduli di tipo REGOL.
    modlist[m]->tiporegol = 0;
    if (modlist[m]->modcomment[0] == '#')
    {
      modlist[m]->tiporegol = 1;
      modlist[m]->modcomment[0] = ' ';
    }

    /* costruisci fullmodname */

    if (argc == 3)
    {
      for (k = 0; k < 5 - strlen(modlist[m]->modpname); k++)
        strcat(partmodname, "0");

      strcat(partmodname, modlist[m]->modpname);
      partmodname[strlen(partmodname) - 1] = '\0';
      strcpy(modlist[m]->fullmodname, partmodname);
    }
    else
    {
      strcpy(tempstr, modlist[m]->fullmodname + FULLMODNAMEPREFLEN);
      strcpy(modlist[m]->fullmodname, partmodname);
      strcat(modlist[m]->fullmodname, tempstr);
    }
    printf("%s\n", modlist[m]->fullmodname);

    fscanf(fpI5, "%d %d", &nfp, &ncfg); /* leggi numphport e nummatconfig */
    modlist[m]->numconfig = ncfg;

    if (nfp != modlist[m]->fpnum) /* controlla la consistenza con il .pag */
    {
      snprintf(tempstr, sizeof(tempstr),
               "number of physical ports doesn't correspond in .pag/.top (%d) and %s (%d):\nproceeding with %d\n",
               modlist[m]->fpnum, i5fname, nfp, nfp);
      modlist[m]->fpnum = nfp;
      warn(tempstr);
    }
    /*printf("%s %d %d\n", modlist[m]->modcomment, nfp, ncfg);*/

    for (p = 0; p < modlist[m]->fpnum; p++) /* per ogni phport... */
    {
      fscanf(fpI5, "%s", fpnam); /* ...leggi portname... */
                                 /*printf("%s\n", fpnam);*/

      fpd = whatport(m, fpnam); /* ...prendi il vero ID... */
      if (fpd == NULL)
      {
        sprintf(tempstr, "cannot find port %s of module %s in .pag\n",
                fpnam, modlist[m]->fullmodname);
        essit(tempstr);
      }

      for (v = 0; v < fpvnum[fpd->fptype]; v++) /* ...leggi variabili fisiche... */
      {
        fscanf(fpI5, "%s", fpd->fvar[v]);
        strcat(fpd->fvar[v], modlist[m]->fullmodname + FULLMODNAMEPREFLEN);
      }

      if (ncfg > 1)
        for (v = 0; v < fpvnum[fpd->fptype]; v++) /* ...leggi NUMbase (opzionale)... */
          fscanf(fpI5, "%d", &(fpd->cfnum[0][v]));

      for (cf = 1; cf < ncfg + 1; cf++) /* per ogni matconfig... */
      {
        for (v = 0; v < fpvnum[fpd->fptype]; v++) /* ...leggi NUMj */
          fscanf(fpI5, "%d", &(fpd->cfnum[cf][v]));
      }
    }

    fscanf(fpI5, "%d %d %d\n", &(modlist[m]->cnstat), /* leggi nstati, nout, nin */
           &(modlist[m]->cnout), &(modlist[m]->cnin));

    if (ncfg > 1)
      for (cf = 1; cf < ncfg + 1; cf++) /* leggi il commento di config. */
      {
        lbuf = fgets(modlist[m]->cfgcomment[cf], COMMENTNCH, fpI5);
        /*printf("%d %d %d %s", modlist[m]->cnstat[cf], modlist[m]->cnout[cf],
        modlist[m]->cnin[cf], modlist[m]->cfgcomment[cf]);*/
      }

    modlist[m]->mvnum = modlist[m]->cnin + modlist[m]->cnout + modlist[m]->cnstat;

    for (cf = 1; cf < ncfg + 1; cf++)
      for (mv = 0; mv < modlist[m]->mvnum; mv++)
      {
        if ((mvcur = (mvdesc *)malloc(sizeof(mvdesc))) == NULL)
          essit("malloc failed in mvdesc allocation\n");

        fscanf(fpI5, "%s %s\n", tmvname, tmvtype); /* leggi var. mat. */
        modlist[m]->mvlist[cf][mv] = mvcur;
        mvcur->status = MVSTATUS_ORIG;

        strcpy(mvcur->mvname, tmvname);
        strcpy(mvcur->fullmvname, mvcur->mvname);
        strcat(mvcur->fullmvname, modlist[m]->fullmodname + FULLMODNAMEPREFLEN);
        strcpy(mvcur->mvtype, tmvtype);
        lbuf = fgets(mvcur->mvcomment, COMMENTNCH, fpI5);
        mvcur->mvcomment[strlen(mvcur->mvcomment) - 1] = '\0';
        /*printf("%s %s %s %s\n", mvcur->mvname, mvcur->fullmvname, mvcur->mvtype, mvcur->mvcomment);*/
      }

    if (ncfg > 1) /* chiedi all'utente la config. voluta */
    {
      printf("Module %s has more than one config\n", modlist[m]->fullmodname);
      printf("please select your choice:\n");
      printf(" 0): CONFIGURAZIONE AUTOMATICA\n");
      for (cf = 1; cf < ncfg + 1; cf++)
      {
        sprintf(tempstr, "%d", cf);
        printf(" %s): %s", tempstr, modlist[m]->cfgcomment[cf]);
      }
      printf("? ");
      /*      fscanf(stdin, "%s", answer);
            switch (answer[0]){
        case '1': modlist[m]->status = BASE_MAT_CONF; break;
        case '2': modlist[m]->status = PLUS_MAT_CONF; break;
        case '3': modlist[m]->status = MINUS_MAT_CONF; break;
        default: modlist[m]->status = SYSTEM_CONF; break;
            }*/
      modlist[m]->status = SYSTEM_CONF;
      //
      // GUAG Aprile 2007 - Con questa modalità non sono gestibili i moduli con
      // una sola configurazione e con inziale + oppure - metre funziona correttamente con blank!!!
      // Per esempio con un TUBO che calcola la pressione uscita, che dovrebbe avere
      // il nome del blocco  -TUBXXXX, si ottiene un file f01.dat con il nome del blocco
      // sbagliato TUBOXXXX !!!! Questo comporta un assegnamento dei residui delle equaz. in fase di run completamente sbagliato.
      //
      // Per riparare si deve:
      //    1) Cambiare il formato del file pi3 inserendo, magari vicino al numero
      //       delle configurazioni (solo quando è 1) il + oppure il - oppure nulla se è blank.
      //    2) Cambiare il programma i32i5.c che produce il file i5 dal pi3, propagando l'informazione del carattere + e -
      //    3) Infine cambiare questo programma che legge il file i5, caricando nella
      //       variabile modlist[m]->status il valore opportuno, magari ristabilendo lo switch sopra
      // Questa modifica è stata rimandata per l'impatto che ha sulla compatibilità con le versioni precedenti.
      // Quindi in questo momento si accetta questo limite.
      //

      printf("selected %d\n\n", modlist[m]->status);

      if (modlist[m]->status == SYSTEM_CONF)    /* copia in curcfnum la configurazione del      */
        for (p = 0; p < modlist[m]->fpnum; p++) /* sistema per le successive configurazioni     */
        {
          fpt = modlist[m]->fplist[p]->fptype;

          for (v = 0; v < fpvnum[fpt]; v++)
            modlist[m]->fplist[p]->curcfnum[v] = modlist[m]->fplist[p]->cfnum[SYSTEM_CONF][v];
        }
    }
    else
      modlist[m]->status = BASE_MAT_CONF;

    fclose(fpI5);
  }

  printf("\n%d moduli found\n\n", modnum);
  for (m = 0; m < modnum; m++)
  {
    printf("%s has %d ports: ", modlist[m]->fullmodname, modlist[m]->fpnum);
    for (p = 0; p < modlist[m]->fpnum; p++)
      printf("%s -> %s    ", modlist[m]->fplist[p]->fpname, modlist[m]->fplist[p]->connectport == NULL ? "??" : modlist[m]->fplist[p]->connectport->fpname);
    printf("\n");
  }
  printf("\n\n");

  /*                      */
  /* Fase III.a           */
  /* primo controllo link */
  /*                                                                      */
  /* In questa fase controlliamo alcune connessioni proibite.             */
  /* La classe ed il senso delle porte sono controllati nativamente da    */
  /* RegoMMI.                                                             */
  /*                                                                      */
  /* La prima verifica e` sulla presenza di collegamenti del tipo:        */
  /*   1 -> -1    -1 -> 1                                                 */
  /* se si trovano, errore fatale.                                        */
  /* Contemporanemante, si verifica la presenza di collegamenti degradati:*/
  /*   1 <-> 0    3 <-> 0                                                 */
  /* se si trovano, warning.                                              */
  /*                                                                      */

  banner("Phase III.a - links check");

  for (m = 0; m < modnum; m++)
  {
    for (p = 0; p < modlist[m]->fpnum; p++)
    {
      fpdO = modlist[m]->fplist[p];
      fpdI = modlist[m]->fplist[p]->connectport;
      fpt = modlist[m]->fplist[p]->fptype;

      if ((fpt % 2 == 1) && fpdI != NULL)
      {
        mI = findm4p(fpdI);
        fpsO = modlist[m]->status;
        fpsI = modlist[mI]->status;
        printf("outing module-port-status: %s-%s %d   inning port-status: %s-%s %d\n",
               modlist[m]->modpname, fpdO->fpname, fpsO, modlist[mI]->modpname, fpdI->fpname, fpsI);
        fflush(stdout);

        for (v = 0; v < fpvnum[fpt]; v++)
        {
          numO = fpdO->cfnum[fpsO][v];
          numI = fpdI->cfnum[fpsI][v];
          /*printf("Ovv: %s %d  Ivv: %s %d\n",
          fpdO->fvar[v],numO,fpdI->fvar[v],numI);*/

          if ((numO == FVAREQMATROLE && numI == FVAROPPMATROLE) ||
              (numO == FVAROPPMATROLE && numI == FVAREQMATROLE))
          {
            sprintf(tempstr, "1 <-> -1 error: %s-%s-%s %d is connected with %s-%s-%s %d\n",
                    modlist[m]->modpname,
                    fpdO->fpname,
                    fpdO->fvar[v],
                    numO,
                    modlist[mI]->modpname,
                    fpdI->fpname,
                    fpdI->fvar[v],
                    numI);

            essit(tempstr);
          }

          if ((numO == FVAREQMATROLE && numI == FVARNOTINMAT) ||
              (numO == FVARNOTINMAT && numI == FVAREQMATROLE))
          {
            sprintf(tempstr, "1 <-> 0 warning: %s-%s-%s %d is connected with %s-%s-%s %d\n",
                    modlist[m]->modpname,
                    fpdO->fpname,
                    fpdO->fvar[v],
                    numO,
                    modlist[mI]->modpname,
                    fpdI->fpname,
                    fpdI->fvar[v],
                    numI);

            warn(tempstr);
          }

          if ((numO == FVAROPPMATROLE && numI == FVARNOTINMAT) ||
              (numO == FVARNOTINMAT && numI == FVAROPPMATROLE))
          {
            sprintf(tempstr, "-1 <-> 0 warning: %s-%s-%s %d is connected with %s-%s-%s %d\n",
                    modlist[m]->modpname,
                    fpdO->fpname,
                    fpdO->fvar[v],
                    numO,
                    modlist[mI]->modpname,
                    fpdI->fpname,
                    fpdI->fvar[v],
                    numI);

            warn(tempstr);
          }

          if ((numO == FVARUNDMATROLE && numI == FVARNOTINMAT) ||
              (numO == FVARNOTINMAT && numI == FVARUNDMATROLE))
          {
            sprintf(tempstr, "3 <-> 0 warning: %s-%s-%s %d is connected with %s %d\n",
                    modlist[m]->modpname,
                    fpdO->fpname,
                    fpdO->fvar[v],
                    numO,
                    fpdI->fvar[v], numI);

            warn(tempstr);
          }
        }
      }
    }
  }

  /*                              */
  /* Fase III.b                   */
  /* secondo controllo link       */
  /*                                                                      */
  /* In questa fase controlliamo altri tipi di connessioni proibite.      */
  /* Il vettore curcfnum contiene la configurazione corrente di ogni porta*/
  /* passibile di modifiche (quindi sono esclusi quei moduli con una sola */
  /* configurazione, e quelli configurati dall'utente). Il vettore viene  */
  /* riempito ed utilizzato solo per quelle porte contenenti dei 3.       */
  /*                                                                      */
  /* La verifica e` sulla presenza di collegamenti del tipo:              */
  /*   1 -> 3    3 -> 1    -1 -> 3    3 -> -1                             */
  /* se si trovano, si verifica se il 3 puo` assumere il valore + o - 1    */
  /* richiamando la subst3()                                              */
  /*                                                                      */

  banner("Phase III.b - second links check");

  for (m = 0; m < modnum; m++)
  {
    for (p = 0; p < modlist[m]->fpnum; p++)
    {
      fpdO = modlist[m]->fplist[p];
      fpdI = modlist[m]->fplist[p]->connectport;
      fpt = modlist[m]->fplist[p]->fptype;

      if ((fpt % 2 == 1) && fpdI != NULL)
      {
        mI = findm4p(fpdI);
        fpsO = modlist[m]->status;
        fpsI = modlist[mI]->status;
        printf("outing module-port-status: %s-%s %d   inning module-port-status: %s-%s %d\n",
               modlist[m]->modpname, fpdO->fpname, fpsO, modlist[mI]->modpname, fpdI->fpname, fpsI);

        for (v = 0; v < fpvnum[fpt]; v++)
        {
          numO = fpdO->cfnum[fpsO][v];
          numI = fpdI->cfnum[fpsI][v];
          /*printf("Ovv: %s %d  Ivv: %s %d\n",
          fpdO->fvar[v],numO,fpdI->fvar[v],numI);*/

          if ((numO == FVAREQMATROLE || numO == FVAROPPMATROLE) &&
              numI == FVARUNDMATROLE)
          {
            subst3(modlist[mI], fpdI, v, numO);
          }

          if ((numI == FVAREQMATROLE || numI == FVAROPPMATROLE) &&
              numO == FVARUNDMATROLE)
          {
            subst3(modlist[m], fpdO, v, numI);
          }
        }
      }
    }
  }

  /*                              */
  /* Fase III.c                   */
  /* terzo controllo link */
  /*                                                                      */
  /* In questa fase controlliamo le catene di 2                           */
  /* Il vettore curcfnum contiene la configurazione corrente di ogni porta*/
  /* passibile di modifiche (quindi sono esclusi quei moduli con una sola */
  /* configurazione, e quelli configurati dall'utente). Il vettore viene  */
  /* riempito ed utilizzato solo per quelle porte contenenti dei 3.       */
  /*                                                                      */
  /* La verifica e` sulla presenza di collegamenti del tipo:              */
  /*   1 -> 2 -> 2... -> 3    3 -> 2 -> 2... -> 1                         */
  /*   -1 -> 2 -> 2... -> 3    3 -> 2 -> 2... -> -1                       */
  /* se si trovano, si segue la catena con tutte le ramificazioni: alla   */
  /* fine, si verifica la somma degli ingressi di catena a 1 e dei limiti */
  /* di catena a -1.                                                      */
  /*   se somma > 1 => il modello e` sbagliato                             */
  /*   se somma = 1 => i 3 vengono eventualmente sostituiti (1 o -1)      */
  /*   se somma = 0 => si controlla il numero dei limiti a 3:             */
  /*      se num3 = 0 e numero limiti matematici > 1 si suggerisce l'uso  */
  /*         di un modulo NODO                                            */
  /*      se num3 = 1 il 3 viene eventualmente sostituito (1 o -1)        */
  /* La eventuale sostituzione dei 3 viene effettuata con la subst3()     */
  /*                                                                      */

  banner("Phase III.c - third links check");

  for (m = 0; m < modnum; m++)
  {
    for (p = 0; p < modlist[m]->fpnum; p++)
    {
      fpt = modlist[m]->fplist[p]->fptype;
      fpdO = modlist[m]->fplist[p];

      if (fpt % 2 == 1)
        fpdI = modlist[m]->fplist[p]->connectport;
      else
        fpdI = leftconnectport(modlist[m]->fplist[p]);

      if (fpdI != NULL)
      {
        mI = findm4p(fpdI);
        fpsO = modlist[m]->status;
        fpsI = modlist[mI]->status;
        printf("outing? module-port-status: %s-%s %d   inning? module-port-status: %s-%s %d\n",
               modlist[m]->modpname, fpdO->fpname, fpsO, modlist[mI]->modpname, fpdI->fpname, fpsI);

        for (v = 0; v < fpvnum[fpt]; v++)
        {
          numO = fpdO->cfnum[fpsO][v];
          numI = fpdI->cfnum[fpsI][v];

          if (numO != FVARTRANSINMAT && numI == FVARTRANSINMAT)
          {
            if (fpdO->fptype % 2 == 0)
              fol2vtbuf[1] = LIMIT2OUT;
            else
              fol2vtbuf[1] = LIMIT2IN;

            fol2num3 = numO == FVARUNDMATROLE ? 1 : 0;
            fol2last3 = numO == FVARUNDMATROLE ? 1 : -1;

            nummatCrossed4nodo = (findmv4fvRelaxed(m, fpdO->fvar[v]) != -1) ? 1 : 0;

            fol2midbuf = 0;
            fol2buf[1] = numO;
            fol2pbuf[1] = fpdO;
            strcpy(fol2vnbuf[1], fpdO->fvar[v]);
            printf("\nvar is %d ....................%s\n", fol2buf[1], fol2vnbuf[1]);

            fol2buf[0] = 1;
            follow2(fpdO, v);
            /* now in fol2buf is the list of transmittable variables */

            printf("found %d trans.ble vars\n", fol2buf[0]);

            ingusc2sum = 0;
            for (tv = 1; tv < fol2buf[0] + 1; tv++)
              if ((fol2buf[tv] == FVAREQMATROLE && fol2vtbuf[tv] == LIMIT2IN) ||
                  (fol2buf[tv] == FVAROPPMATROLE && fol2vtbuf[tv] == LIMIT2OUT))
                ingusc2sum++;

            if (ingusc2sum > 1)
              essit("malconditioned 2 chain - sum > 1\n");
            else if (ingusc2sum == 0)
            {
              if (fol2num3 == 0)
              {
                warn("unused 2 chain - sum 0 num3 0 - 2s zeroed");
                for (tv = 0; tv < fol2midbuf; tv++)
                {
                  printf(" - %s ", fol2midpbuf[tv]->fvar[v]);
                  fol2midpbuf[tv]->curcfnum[v] = FVARNOTINMAT;
                  for (cz = 0; cz <= modlist[findm4p(fol2midpbuf[tv])]->numconfig; cz++)
                    fol2midpbuf[tv]->cfnum[cz][v] = FVARNOTINMAT;
                }

                if (nummatCrossed4nodo > 1)
                {
                  sprintf(tempstr, "you could use a NODO, couldn't you? %d math lim\n", nummatCrossed4nodo);
                  warn(tempstr);
                }
              }
              else if (fol2num3 == 1)
              {
                trmod = findm4p(fol2pbuf[fol2last3]);

                subst3(modlist[trmod], fol2pbuf[fol2last3], v,
                       (fol2vtbuf[fol2last3] == LIMIT2IN) ? FVAREQMATROLE : FVAROPPMATROLE);
              }
            }
            else if (ingusc2sum == 1)
            {
              for (tv = 1; tv < fol2buf[0] + 1; tv++)
                if (fol2buf[tv] == FVARUNDMATROLE)
                {
                  trmod = findm4p(fol2pbuf[tv]);
                  subst3(modlist[trmod], fol2pbuf[tv], v,
                         (fol2vtbuf[tv] == LIMIT2IN) ? FVAROPPMATROLE : FVAREQMATROLE);
                }
            }
          }
        }
      }
    }
  }

  /*                              */
  /* Fase IV                      */
  /* controllo configurazioni     */
  /*                                                              */
  /* In questa fase vengono assegnate le configurazioni degli     */
  /* ultimi moduli la cui config. matematica resta ancora non     */
  /* stabilita.                                                   */
  /*                                                              */
  /* Vengono considerate tutte le possibili combinazioni per tutti*/
  /* i moduli con ancora qualche 3. Se vi sono piu` configurazioni */
  /* matematicamente accettabili, si sceglie la prima trovata     */
  /*                                                              */

  banner("Phase IV - configurations check");

  /* subphase IV.a        */
  /* riempi conftree con i moduli ancora da configurare           */
  /* se modulo->status vale SYSTEM_CONF allora se ha qualche 3    */
  /* viene aggiunto a conftree, se non ne ha, si controlla la sua */
  /* configurazione corrente: se e` corretta, viene modificato il  */
  /* suo status, se non lo e`, si esce con messaggio di errore     */

  cftreecount = 0;

  for (m = 0; m < modnum; m++)
  {
    addm = 0;

    if (modlist[m]->status == SYSTEM_CONF)
    {
      for (p = 0; p < modlist[m]->fpnum; p++)
      {
        fpd = modlist[m]->fplist[p];
        num3 = 0;
        for (v = 0; v < fpvnum[fpd->fptype]; v++)
          if (fpd->curcfnum[v] == FVARUNDMATROLE)
            num3++;

        if (num3 > 0) /* aggiungi modulo a conftree (dopo) */
          addm = 1;
      }

      if (addm == 1)
      {
        conftree[cftreecount][CFTREE_MODID] = m;
        modlist[m]->cftreeslot = cftreecount;
        cfg2test = 0;
        for (cf = 1; cf < modlist[m]->numconfig + 1; cf++)
        {
          conftree[cftreecount][cf] = checksinglecfg(m, cf);

          if (conftree[cftreecount][cf] == CFTREE_CFG2TEST)
          {
            cfg2test++;
            goodcfg = cf;
          }
          printf("%s %d == %d\n", modlist[m]->fullmodname,
                 cf, conftree[cftreecount][cf]);
        }

        if (cfg2test == 0) /* nessuna configurazione possibile: errore */
        {
          sprintf(tempstr, "sorry: module %s is not compatible with this scheme\n",
                  modlist[m]->fullmodname);
          essitW(tempstr);
        }
        else if (cfg2test == 1) /* una sola possibile: selezionala */
        {
          modlist[m]->status = goodcfg;
          printf("%s default-configured (inspite of some 3s) with %s",
                 modlist[m]->fullmodname, modlist[m]->cfgcomment[goodcfg]);
        }
        else /* piu' di una: lascialo in conftree */
        {
          cftreecount++;
          printf("%s will be mathematically configured\n", modlist[m]->fullmodname);
        }
      }
      else
      {
        chkcfg = -1;

        for (cf = 1; cf < modlist[m]->numconfig + 1; cf++)
        {
          neqv = 0;

          for (p = 0; p < modlist[m]->fpnum; p++)
          {
            fpd = modlist[m]->fplist[p];

            for (v = 0; v < fpvnum[fpd->fptype]; v++)
              if ((fpd->curcfnum[v] != fpd->cfnum[cf][v]) &&
                  fpd->curcfnum[v] != 2 &&
                  fpd->cfnum[cf][v] != 2)
                neqv = 1;
          }

          if (neqv == 0) /* trovata una configurazione */
            chkcfg = cf;
        }

        if (chkcfg == -1) /* nessuna configurazione possibile: esci */
        {
          sprintf(tempstr, "math configuration not found for module %s\n",
                  modlist[m]->fullmodname);
          essit(tempstr);
        }
        else
        {
          modlist[m]->status = chkcfg;
          printf("%s default-configured with %s",
                 modlist[m]->fullmodname, modlist[m]->cfgcomment[chkcfg]);
        }
      }
    }
  }

  /* sottofase IV.b       */
  /* Adesso conftree contiene le informazioni sui moduli che      */
  /* devono essere ancora configurati.                            */
  /* Cerchiamo blocchi lineari indipendenti dal resto della rete  */
  /* di moduli (anche connessi attraverso catene di 2) che        */
  /* possiamo trattare come sottoreti a se' stanti                 */

  banner("Phase IV.b - local configurations check");

  for (mod = 0; mod < cftreecount; mod++)
  {
    conftree[mod][CFTREE_ERASEME] = 0;
  }
  erasable = 0;

  for (mod = 0; mod < cftreecount; mod++)
  {
    m = conftree[mod][CFTREE_MODID];

    if (modlist[m]->status == SYSTEM_CONF)
    {
      printf("\nmodule %s is a head-chain\n", modlist[m]->fullmodname);

      for (mm = 0; mm < modnum; mm++) /* pulisci cfLtreeslot in ogni modulo */
        modlist[mm]->cfLtreeslot = ISNOTINSUBNET;

      cfLtreecount = 0;

      modlist[m]->cfLtreeslot = cfLtreecount;

      for (k = 0; k < MAXNMATCFG; k++) /* copia le info di conftree in confLtree */
        confLtree[cfLtreecount][k] = conftree[mod][k];
      cfLtreecount++;

      fillLtree(m, LEVELTOP);
      printf("\nHo trovato una sottorete da %d moduli\n", cfLtreecount);
      for (a = 0; a < cfLtreecount; a++)
        printf("=> %s\n", modlist[confLtree[a][CFTREE_MODID]]->fullmodname);

      fillWas3();

      autocfgLOK = CFG_NOGOOD;
      foundaZerosum = 0;
      checkLtree(0);

      if (autocfgLOK == CFG_GOOD) /* segna in conftree[] che questa sottorete     */
      {                           /* deve essere tolta da conftree                */
        for (mm = 0; mm < cfLtreecount; mm++)
        {
          conftree[modlist[confLtree[mm][CFTREE_MODID]]->cftreeslot][CFTREE_ERASEME] = 1;
          printf("erasable %s  ", modlist[confLtree[mm][CFTREE_MODID]]->fullmodname);

          modlist[confLtree[mm][CFTREE_MODID]]->status = confLnet[mm];
          modlist[confLtree[mm][CFTREE_MODID]]->cfLtreeslot = ISNOTINSUBNET;
          erasable++;
        }
        printf("\n");
      }
      else if (foundaZerosum) /* segna in conftree[] che questa sottorete     */
      {                       /* deve essere tolta da conftree                */
        warn("SUMZERO group!\n");

        for (mm = 0; mm < cfLtreecount; mm++)
        {
          conftree[modlist[confLtree[mm][CFTREE_MODID]]->cftreeslot][CFTREE_ERASEME] = 1;
          printf("erasable %s  ", modlist[confLtree[mm][CFTREE_MODID]]->fullmodname);

          modlist[confLtree[mm][CFTREE_MODID]]->status = zerosumLnet[mm];
          modlist[confLtree[mm][CFTREE_MODID]]->cfLtreeslot = ISNOTINSUBNET;
          erasable++;
        }
      }
      else
      {
        warn("WRONG group!\n");
        for (mm = 0; mm < cfLtreecount; mm++)
        {
          sprintf(tempstr, "%s\n", modlist[confLtree[mm][CFTREE_MODID]]->fullmodname);
          warn(tempstr);
        }
        essit("no Lconfiguration has been found!\n");
      }
    }
  }

  /*
  printf("erasable? = %d\n", erasable);
  for(k=0; k<cftreecount; k++) printf("%s   %d\n",modlist[conftree[k][CFTREE_MODID]]->fullmodname,conftree[k][CFTREE_ERASEME]);
  */
  printf("\n");

  erased = 0;

  printf("\ncompacting...\n");
  for (mod = 0; mod < cftreecount - erased; mod++) /* compatta il conftree rimuovendo      */
  {                                                /* quelli configurati in una sottorete  */
    printf("testing %s cftc = %d\n", modlist[conftree[mod][CFTREE_MODID]]->fullmodname,
           modlist[conftree[mod][CFTREE_MODID]]->cftreeslot);
    while (conftree[mod][CFTREE_ERASEME] == 1 && erased != erasable)
    {
      printf("erasing %s   %d  %d\n", modlist[conftree[mod][CFTREE_MODID]]->fullmodname,
             mod, cftreecount - erased);
      for (cmod = mod; cmod < cftreecount - erased - 1; cmod++)
      {
        for (k = 0; k < MAXNMATCFG + 1; k++) /* copia le info di cmod+1 in cmod */
          conftree[cmod][k] = conftree[cmod + 1][k];

        modlist[conftree[cmod][CFTREE_MODID]]->cftreeslot = cmod; /* aggiorna cftreeslot */
      }

      erased++;
    }
  }

  printf("cftcount = %d  erased = %d\n", cftreecount, erased);
  cftreecount -= erased;

  /* sottofase IV.c       */
  /* Adesso conftree contiene le informazioni sui moduli che      */
  /* devono essere ancora configurati. Se cftreecount > 0,        */
  /* scorriamo tutte le combinazioni e teniamo traccia di quelle  */
  /* accettabili. In realta`, cftreecount dovrebbe essere sempre 0 */

  if (cftreecount > 0)
  {
    autocfgOK = CFG_NOGOOD;

    checktree(0);

    if (autocfgOK == CFG_GOOD)
      for (m = 0; m < cftreecount; m++)
      {
        modlist[conftree[m][CFTREE_MODID]]->status = confnet[m];

        printf("%s auto-configured with %s",
               modlist[conftree[m][CFTREE_MODID]]->fullmodname,
               modlist[conftree[m][CFTREE_MODID]]->cfgcomment[confnet[m]]);
      }
    else
      essit("sorry: no configuration has been found!\n");
  }

  /*                      */
  /* Fase V               */
  /* rimozione dei 2      */
  /*                                                              */
  /* In questa fase vengono effettuate le sostituzioni dei nomi   */
  /* delle variabili collegate attraverso links di tipo 2         */
  /* La fase viene ripetuta finche' tutti i due sono scomparsi    */
  /* Le sostituzioni dei 2 e dei nomi variabili sono fatte        */
  /* direttamente su cfnum[status] e su mvlist->fullmvname        */
  /*                                                              */

  banner("Phase V - 2s obliteration");

  do
  {
    some2ispresent = 0; /* quando, alla fine del ciclo, questa  */
    /* variabile vale 0, i 2 sono finiti    */
    for (m = 0; m < modnum; m++)
    {
      for (p = 0; p < modlist[m]->fpnum; p++)
      {
        fpdO = modlist[m]->fplist[p];
        fpdI = modlist[m]->fplist[p]->connectport;
        fpt = modlist[m]->fplist[p]->fptype;

        if ((fpt % 2 == 1) && fpdI != NULL)
        {
          mI = findm4p(fpdI);
          fpsO = modlist[m]->status;
          fpsI = modlist[mI]->status;
          /*printf("p4-outing module-port-status: %s-%s %d   inning module-port-status: %s-%s %d\n",
          modlist[m]->modpname,fpdO->fpname,fpsO,modlist[mI]->modpname,fpdI->fpname,fpsI);*/

          for (v = 0; v < fpvnum[fpt]; v++)
          {
            numO = fpdO->cfnum[fpsO][v];
            numI = fpdI->cfnum[fpsI][v];
            /*printf("p4-Ovv: %s %d  Ivv: %s %d\n",
            fpdO->fvar[v],numO,fpdI->fvar[v],numI);*/

            if (numO == FVAREQMATROLE && numI == FVARTRANSINMAT)
            {
              some2ispresent++;

              printf("p4: %s 2 in %s %s substituted with %s %d\n",
                     fpdI->fvar[v], modlist[mI]->fullmodname, fpdI->fpname,
                     fpdO->fvar[v], fpdO->cfnum[fpsO][v]);

              fpdO->cfnum[fpsO][v] = FVARNOTINMAT;
              fpdI->cfnum[fpsI][v] = FVARNOTINMAT;

              some2ispresent += replaceout2(mI, fpdO->fvar[v], fpdI->fvar[v]);
            }

            if (numO == FVARTRANSINMAT && numI == FVAROPPMATROLE)
            {
              some2ispresent++;

              printf("p4: %s 2 in %s %s substituted with %s %d\n",
                     fpdO->fvar[v], modlist[m]->fullmodname, fpdO->fpname,
                     fpdI->fvar[v], fpdI->cfnum[fpsI][v]);

              fpdO->cfnum[fpsO][v] = FVARNOTINMAT;
              fpdI->cfnum[fpsI][v] = FVARNOTINMAT;

              some2ispresent += replaceout2(m, fpdI->fvar[v], fpdO->fvar[v]);
            }
          }
        }
      }
    }
    printf("looping: obliterated %d 2s\n", some2ispresent);
  } while (some2ispresent != 0);

  /*                      */
  /* Fase VI              */
  /* ultime sostituzioni  */
  /*                                                              */
  /* In questa fase vengono effettuate le sostituzioni dei nomi   */
  /* delle variabili collegate attraverso links di tipo +-1       */
  /* La fase viene ripetuta finche' tutte sono state trattate     */
  /* Le sostituzioni dei nomi variabili sono propagate lungo      */
  /* tutti i moduli->mvlist->fullmvname                           */
  /* I valori numerici sono modificati su cfnum[status]           */
  /*                                                              */

  banner("Phase VI - last substitutions");

  do
  {
    some1ispresent = 0; /* quando, alla fine del ciclo, questa  */
    /* variabile vale 0, gli 1 sono finiti  */
    for (m = 0; m < modnum; m++)
    {
      for (p = 0; p < modlist[m]->fpnum; p++)
      {
        fpdO = modlist[m]->fplist[p];
        fpdI = modlist[m]->fplist[p]->connectport;
        fpt = modlist[m]->fplist[p]->fptype;

        if ((fpt % 2 == 1) && fpdI != NULL)
        {
          mI = findm4p(fpdI);
          fpsO = modlist[m]->status;
          fpsI = modlist[mI]->status;
          /*printf("p5-outing module-port-status: %s-%s %d   inning module-port-status: %s-%s %d\n",
          modlist[m]->modpname,fpdO->fpname,fpsO,modlist[mI]->modpname,fpdI->fpname,fpsI);*/

          for (v = 0; v < fpvnum[fpt]; v++)
          {
            numO = fpdO->cfnum[fpsO][v];
            numI = fpdI->cfnum[fpsI][v];
            /*printf("p5-Ovv: %s %d  Ivv: %s %d\n",
            fpdO->fvar[v],numO,fpdI->fvar[v],numI);*/

            if (numO == FVAREQMATROLE && numI == FVAREQMATROLE)
            {
              fpdO->cfnum[fpsO][v] = FVARNOTINMAT;
              fpdI->cfnum[fpsI][v] = FVARNOTINMAT;

              strcpy(mvname1, fpdO->fvar[v]);
              strcpy(mvname2, fpdI->fvar[v]);
              some1ispresent += replaceout1(mvname1, mvname2);
            }

            if (numO == FVAROPPMATROLE && numI == FVAROPPMATROLE)
            {
              fpdO->cfnum[fpsO][v] = FVARNOTINMAT;
              fpdI->cfnum[fpsI][v] = FVARNOTINMAT;

              strcpy(mvname1, fpdO->fvar[v]);
              strcpy(mvname2, fpdI->fvar[v]);
              some1ispresent += replaceout1(mvname2, mvname1);
            }
          }
        }
      }
    }
    printf("looping: obliterated %d 1s\n", some1ispresent);
  } while (some1ispresent != 0);

  banner("Final Configuration");
  /*
  for (m=0; m<modnum; m++)
  {
    mst = modlist[m]->status;
    printf("** %s **\n", modlist[m]->fullmodname);
    for (mv=0; mv <modlist[m]->mvnum; mv++)
    {
      printf("%s\n",modlist[m]->mvlist[mst][mv]->fullmvname);
    }
    printf("\n");
  }
  */

  /*                                              */
  /* Fase VI.b                                    */
  /* controllo su variabili di ingresso duplicate */
  /* ed eventuale aggiunta di moduli DISN.        */
  /*                                              */

  banner("Phase VI.b - check duplicate input vars");

  disnnum = 0;

  for (m = 0; m < modnum; m++)
  {
    mst = modlist[m]->status;

    for (mv = 0; mv < modlist[m]->mvnum; mv++)
    {
      strcpy(curmvname, modlist[m]->mvlist[mst][mv]->fullmvname);
      if (strcmp(modlist[m]->mvlist[mst][mv]->mvtype, "IN") == 0)
        curmvocc = 1;
      else
        curmvocc = 0;

      for (mvv = mv + 1; mvv < modlist[m]->mvnum; mvv++)
      {
        strcpy(mvname, modlist[m]->mvlist[mst][mvv]->fullmvname);

        if (strcmp(curmvname, mvname) == 0 && strcmp(modlist[m]->mvlist[mst][mvv]->mvtype, "IN") == 0)
          curmvocc++;
      }

      if (curmvocc > 1)
      {
        sprintf(tempstr, "%s in %s has %d occurrence(s). Inserting DISN.\n",
                curmvname, modlist[m]->fullmodname, curmvocc);
        warn(tempstr);

        addDISN(m, curmvname, curmvocc);
      }
    }
  }

  /*                                              */
  /* Fase VII                                     */
  /* costruisci f01.dat                           */
  /*                                              */

  banner("Phase VII - build f01.dat");

  /* apri file f01.dat */
  // if(MoveFileEx("f01.dat", "f01old.dat", MOVEFILE_WRITE_THROUGH) == 0) printf("aggiornadescr FALLITA copia \n");

  if ((fpf01 = fopen("f01_pag.dat", "w")) == NULL)
  {
    sprintf(tempstr, "cannot open f01_pag.dat\n");
    essit(tempstr);
  }

  fprintf(fpf01, "****\n"); /* header */
  printf("****\n");
  for (m = 0; m < modnum; m++)
  {
    strcpy(partmodname, modlist[m]->fullmodname);
    partmodname[FULLMODNAMEPREFLEN] = '\0';

    sprintf(tempstr, "%s      BLOCCO (%s) - %s\n",
            partmodname,
            modlist[m]->fullmodname + FULLMODNAMEPREFLEN,
            modlist[m]->modcomment);
    tempstr[76] = '\0';
    tempstr[75] = '\n';
    fprintf(fpf01, tempstr);
    printf(tempstr);
  }

  //  strcpy(tempstr, argv[argc-1]);                        /* tronca il nome file al carattere '.' */
  cc = strlen(argv[argc - 1]);
  c1 = 0;
  c2 = cc - 1;
  for (c = 0; c < cc; c++)
    if (argv[argc - 1][c] == '/' || argv[argc - 1][c] == '\\')
      c1 = c;
    else if (argv[argc - 1][c] == '.')
      c2 = c;
  if (c1 > 0 && c1 < cc - 1)
    c1++;
  if (c2 < c1)
    c2 = c1;
  for (c = c1; c < c2; c++)
  {
    cc = (unsigned)argv[argc - 1][c];
    tempstr[c - c1] = (cc >= 'a' && cc <= 'z') ? cc - 32 : cc;
  }
  tempstr[c2 - c1] = '\0';

  fprintf(fpf01, "****\n%s\n", tempstr);
  printf("****\n%s\n", tempstr);

  for (m = 0; m < modnum; m++)
  {
    strcpy(partmodname, modlist[m]->fullmodname);
    partmodname[FULLMODNAMEPREFLEN] = '\0';

    mst = modlist[m]->status;
    fprintf(fpf01, "****\n");
    printf("****\n");

    strcpy(stafulmodname, modlist[m]->fullmodname);
    if (modlist[m]->status > BASE_MAT_CONF &&
        (strcmp(partmodname, "NODO") != 0)) /* assegna il primo carattere +/- */
    {
      for (c = FULLMODNAMEPREFLEN - 2; c >= 0; c--)
        stafulmodname[c + 1] = stafulmodname[c];
      if (modlist[m]->status == PLUS_MAT_CONF)
        stafulmodname[0] = '+';
      else if (modlist[m]->status == MINUS_MAT_CONF)
        stafulmodname[0] = '-';
    }

    // guag feb 2008 - correzione per moduli di tipo REGOL
    if (modlist[m]->tiporegol == 1)
      sprintf(tempstr, "%s  BL.-%s- **** REGOL. %s - %s\n",
              stafulmodname,
              modlist[m]->fullmodname + FULLMODNAMEPREFLEN,
              partmodname,
              modlist[m]->modcomment);
    else
      sprintf(tempstr, "%s  BL.-%s- **** MODULO %s - %s\n",
              stafulmodname,
              modlist[m]->fullmodname + FULLMODNAMEPREFLEN,
              partmodname,
              modlist[m]->modcomment);

    tempstr[76] = '\0';
    tempstr[75] = '\n';
    fprintf(fpf01, tempstr);
    printf(tempstr);

    for (mv = 0; mv < modlist[m]->mvnum; mv++)
      if (modlist[m]->mvlist[mst][mv]->status == MVSTATUS_ORIG)
      {
        sprintf(tempstr, "  --%s-- %s\n",
                modlist[m]->mvlist[mst][mv]->mvtype,
                modlist[m]->mvlist[mst][mv]->mvcomment);
        strcpy(tempmvname, modlist[m]->mvlist[mst][mv]->fullmvname);
        strcat(tempmvname, tempstr);
        tempmvname[76] = '\0';
        tempmvname[75] = '\n';
        for (c = 0; c < strlen(tempmvname); c++)
        {
          fputc(tempmvname[c], fpf01);
          putchar(tempmvname[c]);
        }
      }
      else
      {
        sprintf(tempstr, "  --%s-- #",
                modlist[m]->mvlist[mst][mv]->mvtype);
        strcpy(tempmvname, modlist[m]->mvlist[mst][mv]->fullmvname);
        strcat(tempmvname, tempstr);
        strcat(tempmvname, modlist[m]->mvlist[mst][mv]->subconn);
        strcat(tempmvname, "#....................<===");

        om = findmod4mvname(modlist[m]->mvlist[mst][mv]->fullmvname);
        strcpy(stafulmodname, modlist[om]->fullmodname);
        strcpy(partmodname, modlist[om]->fullmodname);
        partmodname[FULLMODNAMEPREFLEN] = '\0';
        if (modlist[om]->status > BASE_MAT_CONF &&
            (strcmp(partmodname, "NODO") != 0)) /* assegna il primo carattere +/- */
        {
          for (c = FULLMODNAMEPREFLEN - 2; c >= 0; c--)
            stafulmodname[c + 1] = stafulmodname[c];
          if (modlist[om]->status == PLUS_MAT_CONF)
            stafulmodname[0] = '+';
          else if (modlist[om]->status == MINUS_MAT_CONF)
            stafulmodname[0] = '-';
        }

        strcat(tempmvname, stafulmodname);
        strcat(tempmvname, "  BL.-");
        strcat(tempmvname, stafulmodname + FULLMODNAMEPREFLEN);
        strcat(tempmvname, "-\n");

        for (c = 0; c < strlen(tempmvname); c++)
        {
          fputc(tempmvname[c], fpf01);
          putchar(tempmvname[c]);
        }
      }

    fprintf(fpf01, "****\n");
    printf("****\n");
  }

  fprintf(fpf01, "****      >>>>>>INGRESSI DI TUTTI I MODULI<<<<<<\n");
  printf("****      >>>>>>INGRESSI DI TUTTI I MODULI<<<<<<\n");

  for (m = 0; m < modnum; m++)
  {
    mst = modlist[m]->status;

    for (mv = 0; mv < modlist[m]->mvnum; mv++)
      if (modlist[m]->mvlist[mst][mv]->status == MVSTATUS_ORIG &&
          (strcmp(modlist[m]->mvlist[mst][mv]->mvtype, "IN") == 0))
      {
        sprintf(tempstr, "  --%s-- %s\n",
                modlist[m]->mvlist[mst][mv]->mvtype,
                modlist[m]->mvlist[mst][mv]->mvcomment);
        strcpy(tempmvname, modlist[m]->mvlist[mst][mv]->fullmvname);
        strcat(tempmvname, tempstr);
        tempmvname[76] = '\0';
        tempmvname[75] = '\n';
        for (c = 0; c < strlen(tempmvname); c++)
        {
          fputc(tempmvname[c], fpf01);
          putchar(tempmvname[c]);
        }
      }
  }
  fprintf(fpf01, "****\n");
  printf("****\n");

  for (m = 0; m < modnum; m++)
  {
    mst = modlist[m]->status;

    for (mv = 0; mv < modlist[m]->mvnum; mv++)
      if (modlist[m]->mvlist[mst][mv]->status != MVSTATUS_ORIG)
      {
        sprintf(tempstr, "  --CO-- %s\n",
                modlist[m]->mvlist[mst][mv]->mvcomment);
        strcpy(tempmvname, modlist[m]->mvlist[mst][mv]->subconn);
        strcat(tempmvname, tempstr);
        tempmvname[76] = '\0';
        tempmvname[75] = '\n';
        for (c = 0; c < strlen(tempmvname); c++)
        {
          fputc(tempmvname[c], fpf01);
          putchar(tempmvname[c]);
        }
      }
  }

  fflush(fpf01);
  fclose(fpf01);

  rettx = (int)aggiornadescrf01();
  printf(" Fine pag2f01: %d\n", rettx);
  // #ifndef Linux
  return (rettx);
  // #endif
}

int aggiornadescrf01()
{
  // GUAG2025 uso allocazione dinamica dei puntatori alle linee salvate in memoria di f01old
//#define MAXLIN 20000 // massimo linee del file f01.dat

  FILE *fpf01pag, *fpf01old, *fpf01new;
  char **f01old=NULL;
  size_t f01old_size = 0;
  size_t f01old_capacity = 0;
  #define MAXLENBUFFER 101
  char buffer[MAXLENBUFFER];
  
  int i, linconfr;

  printf("aggiornadescr Inizio programma\n");
  //   if(MoveFileEx("f01.dat", "f01old.dat", MOVEFILE_WRITE_THROUGH) == 0) printf("aggiornadescr FALLITA copia \n");
  //   if((fpf01old=fopen("f01old.dat", "r"))== NULL){
  if ((fpf01old = fopen("f01.dat", "r")) == NULL)
  {
    printf("aggiornadescr - f01.dat non esistente, procedo alla copia\n");
//        MoveFileEx("f01_pag.dat", "f01.dat", MOVEFILE_WRITE_THROUGH);
#ifndef Linux
    if (MoveFile("f01_pag.dat", "f01.dat") == 0)
    {
      printf("aggiornadescr- FALLITA copia di f01_pag.dat su f01.dat\n");
      return (1);
    }
#else
    if (rename("f01_pag.dat", "f01.dat") != 0)
    {
      printf("aggiornadescr- FALLITA copia di f01_pag.dat su f01.dat\n");
      return (1);
    }

    if ((fpf01old = fopen("f01.dat", "r")) == NULL)
    {
      printf("aggiornadescr- FALLITA apertura di f01.dat anche dopo la copia.\n");
      return (2);
    }
#endif
    return (0);
  }
  // memorizzo il file con le descrizioni da salvare
  

while (fgets(buffer, sizeof(buffer), fpf01old)) {
    if (f01old_size == f01old_capacity) {
        size_t new_capacity = f01old_capacity == 0 ? 128 : f01old_capacity * 2;
        char **tmp = realloc(f01old, new_capacity * sizeof(char *));
        if (!tmp) {
            perror("pag2f01, aggiornadescrf01: realloc fallita");
            exit(1);
        }
        f01old = tmp;
        f01old_capacity = new_capacity;
    }
    f01old[f01old_size] = malloc(strlen(buffer) + 1);
    if (!f01old[f01old_size]) {
        perror("pag2f01, aggiornadescrf01: malloc fallita");
        exit(1);
    }
    strcpy(f01old[f01old_size], buffer);
    f01old_size++;
}
fclose(fpf01old);


  // while (!feof(fpf01old))
  // {

  //   fgets(buff, 100, fpf01old);
  //   f01old[nlin] = (char *)malloc(strlen(buff) + 1);
  //   strcpy(f01old[nlin], buff);
  //   nlin++;
  //   if (nlin > MAXLIN)
  //   {
  //     printf("pag2f01, aggiornadescrf01: Superato il numero massimo di linee per f01.dat (max=%d)\n",
  //            MAXLIN);
  //     printf("pag2f01, aggiornadescrf01: pag2f01 termina per errore\n");
  //     exit(1);
  //   }
  // }
  // fflush(fpf01old);
  // fclose(fpf01old);

  if ((fpf01pag = fopen("f01_pag.dat", "r")) == NULL)
  {
    printf("aggiornadescr - Apertura del file f01_pag.dat FALLITA\n");
    return (3);
  }

  if ((fpf01new = fopen("f01_new.dat", "w")) == NULL)
  {
    printf("aggiornadescr - Creazione del file f01_new.dat FALLITA\n");
    return (3);
  }

  fgets(buffer, sizeof(buffer), fpf01pag);

  fputs(buffer, fpf01new);

  strcpy(buffer, " ");
  while (strncmp(buffer, "****", 4) != 0)
  {
    fgets(buffer, sizeof(buffer), fpf01pag);

    for (i = 0; i < (int)f01old_size; i++)
    {
      if (strncmp(buffer, f01old[i], 30) == 0)
      {
        strcpy(buffer, f01old[i]);
        break;
      }
    }
    fputs(buffer, fpf01new);
  }

  while (!feof(fpf01pag))
  {
    strcpy(buffer, "");
    fgets(buffer, sizeof(buffer), fpf01pag);

    if (buffer[17] != '#')
    {
      linconfr = 16;
      if (buffer[10] == 'B' && buffer[11] == 'L' && buffer[12] == '.')
        linconfr = 36;
      for (i = 0; i < (int)f01old_size; i++)
      {
        if (strncmp(buffer, f01old[i], linconfr) == 0)
        {
          if (f01old[i][17] != '#')
            strcpy(buffer, f01old[i]);
          break;
        }
      }
    }
    fputs(buffer, fpf01new);
    printf("buffer= %s\n", buffer);
  }
  fflush(fpf01pag);
  fclose(fpf01pag);

  fflush(fpf01new);
  fclose(fpf01new);

// Alla fine libero la memoria
for (size_t i = 0; i < f01old_size; ++i) {
    free(f01old[i]);
}
free(f01old);


  //   WinExec("copy f01_new.dat f01.dat",SW_MINIMIZE);
  //   execl( "copy", "copy", "f01_new.dat", "f01.dat" , NULL );
  //   if(CopyFile("f01_new.dat", "f01.dat",FALSE)== 0) printf("aggiornadescr FALLITA copia f01_new.dat \n");

#ifndef Linux
  printf("rinomino f01_new.dat in f01.dat \n");
  if (MoveFileEx("f01_new.dat", "f01.dat", MOVEFILE_REPLACE_EXISTING) == 0)
  {
    printf("aggiornadescr FALLITA rename f01_new.dat \n");
    return (4);
  }
#else
  if (rename("f01.dat", "f01_scrach.dat") != 0)
  {
    printf("aggiornadescr- FALLITA rename di f01.dat su f01_scarch.dat\n");
    return (4);
  }
  if (rename("f01_new.dat", "f01.dat") != 0)
  {
    printf("aggiornadescr- FALLITA rename di f01_new.dat su f01.dat\n");
    return (4);
  }
#endif

  printf("aggiornadescr Fine programma\n");
  return (0);
}
