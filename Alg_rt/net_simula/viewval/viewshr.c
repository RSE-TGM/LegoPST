/*
 *  ----------------    viewshr   -------------------------
 *  Funzione di acceeso alla shared memory del simulatore.
 *  Consente di leggere il valore di una variabile del simulatore
 *  e di forzarne il valore.
 *
 *  #include "viewshr.h"
 *
 *  int viewshr(
 *        int 	modo,
 *	char 	*nomevar,
 *	int 	*indir,
 *	float 	*valore,
 *	int 	*stato,
 *	float 	*tempo,
 *	int	*num_var,
 *	float	forzval );
 *
 *  Le funzioni vengono attivate con la variabile "modo":
 *   INIZIALIZZA 	aggancio alla shared memory e lettura del db delle variabili
 *                       input: modo
 *                       output: num_var, stato
 *
 *   GETIND		decodifica indirizzo della variabile
 *			input: modo, nomevar
 *			output: indir
 *			return TRUE: variabile trovata, FALSE: non trovata
 *
 *   GETVAR		lettura del valore della variabile
 *			input: modo, indir
 *			output: valore, tempo
 *
 *   PUTVAR		forzamento valore
 *			input: modo, indir, forzval
 *                       output:
 *
 *   CHECK		lettura stato del simulatore
 *			input: modo
 *                       output: stato, tempo
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "sim_param.h" /* parametri generali LEGO	    */
#include "sim_types.h" /* tipi e costanti LEGO	    */
#include "sked.h"

#include "viewshr.h"

#include <Rt/RtDbPunti.h>
#include <Rt/RtErrore.h>
#include <Rt/RtMemory.h>

#define errore(stringa) exit(fprintf(stderr, "Attenzione !\nERRORE comtidea: %s\n", stringa) != 0)
#define errore1(stringa, num) exit(fprintf(stderr, "Attenzione !\nERRORE comtidea: %s %d\n", stringa, num) != 0)

RtDbPuntiOggetto dbpunti;
RtErroreOggetto errore;

int tot_variabili;    /* numero totale variabili dei modelli   */
VARIABILI *variabili; /* database variabili di tutti i modelli */
char *ind_sh_top;     /* puntatore inizio shared memory sh_var */
int id_sh;            /* identificatore sh_var */

int cerca_indirizzo();
void costruisci_var(char **, VARIABILI **, int *);

double gettim();

#define FILEOUT "VAR.LOG"
#define FILEIN "LISTA_VIEWSHR"
FILE *fplog;

#define INIT_STAMPA 1
#define STAMPA 2
#define MAXRIGA 85

#define MAX_LISTA_VAR 300
#define MAX_LUN_DESCR 80

float puntonow;
FILE *fpsave, *fpout;

int viewshr(modo, nomevar, indir, valore, stato, tempo, num_var, forzval)
int modo, *indir, *stato, *num_var;
float *valore, *tempo, forzval;
char *nomevar;
{
  int i, j, indirizzo, lun_valore;
  float time[1];
  Boolean retlog;
  char *get_work_directory(), *newdir;
  SIMULATOR *simpar;

  simpar = (SIMULATOR *)calloc(1, sizeof(SIMULATOR));
  simpar->max_snap_shot = 1;
  simpar->max_back_track = 1;
  simpar->max_campioni = 1;
  simpar->num_var = 1;
  simpar->max_pertur = 1;
  simpar->spare_snap = 1;
  simpar->pert_clear = 1;

  switch (modo)
  {
  case INIZIALIZZA:
    // DEBUG   fprintf(stderr,"cambio directory: %s \n", ((chdir ( get_work_directory()) == 0 ) ? "OK" : "NO") ) ;
    newdir = get_work_directory();
    chdir(newdir);
    // DEBUG	   fprintf(stderr,"cambio directory: %s   %s \n", newdir,  ( ((chdir (newdir)) == 0) ? "OK" : "NO") ) ;
    //	   leggi_file_lista_log( lista_var, &num_lista_var);

    /* aggancio sh_var variabili del simulatore */

    /* ridirezione momentanea dello stdout su file */
    fpsave = (FILE *)malloc(sizeof(FILE));
    //	    memcpy(fpsave,&_iob[1],sizeof( FILE ));
    memcpy(fpsave, stdout, sizeof(FILE));
    fpout = fopen("viewval.out", "w");
    //	    _iob[1] = *fpout;
    *(stdout) = *fpout;
    // DEBUG	    fprintf(stderr,"err---------------->\n");
    // DEBUG	    fprintf(stdout,"out---------------->\n");
    errore = RtCreateErrore(RT_ERRORE_TERMINALE, "viewshr");
    dbpunti = RtCreateDbPunti(errore, NULL, DB_PUNTI_INT, simpar);
    if (dbpunti == NULL)
      exit(printf("viewshr: simulazione non attiva\n"));

    /* costruzione o aggancio sh_var nomi var e descrizioni*/
    costruisci_var(&ind_sh_top, &variabili, &id_sh);
    /* rimetto a posto stdout */
    fflush(stdout);
    //	    _iob[1] = *fpsave;
    *(stdout) = *fpsave;
    // DEBUG            fprintf(stdout,"out-out---------------->\n");
    // DEBUG            fprintf(stderr,"err-err---------------->\n\n\n\n");

    /* ricerca tot_variabili */
    tot_variabili = numero_variabili(ind_sh_top);
    // DEBUG fprintf(stderr,"tot_variabili (simulatore )=%d\n",tot_variabili);

    /*
                {
                  char nome1[MAX_LUN_NOME_VAR]={"IT0H0BPA"};
                  indirizzo=cerca_indirizzo (nome1,variabili,tot_variabili);
                  if (indirizzo != -1) {
                         printf("trovato indirizzo %d\n",indirizzo);
                         printf("valore=%f\n",RtDbPGetValueD(dbpunti,indirizzo));
                  }
                  else {
                         printf("variabile %s non trovata\n",nome1);
                  }

                }
    */
    retlog = RtDbPGetStato(dbpunti, stato);
    return (TRUE);
  case CHECK:
    retlog = RtDbPGetStato(dbpunti, stato);
    if (!retlog)
    {
      fprintf(stderr, "Attenzione dppunti NULL\n");
      return (FALSE);
    }
    // DEBUG                 printf("Stato del simulatore: %d\n", *stato);
    RtDbPGetTime(dbpunti, time);
    *tempo = *time;
    return (TRUE);
  case GETIND:
    indirizzo = cerca_indirizzo(nomevar, variabili, tot_variabili);
    *indir = indirizzo;
    if (indirizzo <= -1)
      return (FALSE);
    return (TRUE);
  case GETVAR:
    /*
                      for (i=0;i<num_var_ok;i++)
                      {
    // DEBUG fprintf(stderr, " LOOP vettpunti[%d]=%f num_var_ok=%d\n",i,vettpunti[i],num_var_ok);
                        puntonow=RtDbPGetValueD(dbpunti,lista_var_ok[i].ind);
                        if( (vettpunti[i] != puntonow ) || forzaprint )
                        {
                          RtDbPGetTime(dbpunti,time);
                          fprintf(stderr, "tempo %f punto %s (%d) indirizzo %d cambiato: old=%f now=%f\n", *time, lista_var_ok[i].nome,i,lista_var_ok[i].ind,vettpunti[i],puntonow);
                          stampa(STAMPA, i, lista_var_ok, *time );
                          vettpunti[i]=puntonow;
                        }
                      }
    */

    indirizzo = *indir;
    puntonow = RtDbPGetValueD(dbpunti, indirizzo);
    *valore = puntonow;
    RtDbPGetTime(dbpunti, time);
    *tempo = *time;
    // DEBUG		  fprintf(stdout, "tempo %f punto %s indirizzo %d valore=%f\n", *time, nomevar,indirizzo,puntonow);
    return (TRUE);
  case PUTVAR:
    indirizzo = *indir;
    RtDbPPutValue(dbpunti, indirizzo, forzval);
    return (TRUE);
  }
  return (FALSE);
}

/*                         */
/*      stampa ()          */
/*                         */

/*
stampaf(modo,punt,lista_var, time)
int modo, punt;
VARSC *lista_var;
float time;
{


switch (modo)
{
  case INIT_STAMPA :
        fplog=fopen( FILEOUT , "w");
        break;
  case STAMPA :
        fprintf(fplog,"\nTempo: %.0f \nPunto: %s %s \nda\t%.1f \ta\t%.1f\n",time, lista_var[punt].nome,lista_var[punt].descr,vettpunti[punt],puntonow);
        fflush(fplog);
        break;
}

}
*/

/*                         */
/*      cerca_indirizzo () */
/*                         */
int cerca_indirizzo(sigla, vari, tot_var)
char *sigla;
VARIABILI *vari;
int tot_var;
{

  int k;
  char app_sigla[MAX_LUN_NOME_VAR];

  memcpy(&app_sigla[0], "        ", 8);
  memcpy(&app_sigla[0], sigla, strlen(sigla));
  app_sigla[MAX_LUN_NOME_VAR - 1] = 0;

  for (k = 0; k < tot_var; k++)
  {
    if (strncmp(app_sigla, vari[k].nome, MAX_LUN_NOME_VAR) == 0)
      break;
  }

  // DEBUG    printf ("cerca indirizzo %s -> %d/%d %d \n", app_sigla, k, tot_var, vari[k].addr);
  if (k < tot_var)
    return (vari[k].addr);
  else
    return (-1);
}

char *get_work_directory()
{
#ifdef LINUX
#define COMANDO "ps -aeww | gawk '/net_sked/ { qq=index($0,\"PWD\"); print qq; print(substr($0, qq, 100))}' | gawk '/PWD=/{ print $1}'"
#else
#define COMANDO "ps Aeww | gawk '/net_sked/ { qq=index($0,\"PWD\"); print qq; print(substr($0, qq, 100))}' | gawk '/PWD=/{ print $1}'"
#endif

#define DIMDIRBUFF 100

  static char dirbuff[DIMDIRBUFF];
  FILE *fpop;
  int retval;

#ifdef LINUX
  fpop = popen("ps -ao ucomm", "r");
#else
  fpop = popen("ps -o ucomm", "r");
#endif

  while (fgets(dirbuff, DIMDIRBUFF, fpop) != NULL)
  {
    if (!strncmp(dirbuff, "net_sked", 8))
    {
      // DEBUG	   printf("---------------------> trovato net_sked %s \n",dirbuff);
      retval = pclose(fpop);
      fpop = popen(COMANDO, "r");
      while (fgets(dirbuff, DIMDIRBUFF, fpop) != NULL)
      {
        // DEBUG                  printf("---------------------> %s \n",dirbuff);
        if (!strncmp(dirbuff, "PWD=", 4))
        {
          // DEBUG				   fprintf(stderr, "PATH = %s\n",dirbuff+4);
          retval = pclose(fpop);
          dirbuff[strlen(dirbuff) - 1] = 0;
          return (dirbuff + 4);
        }
      }
      return (".");
    }
  }
  fprintf(stderr, "Simulazione non attiva! manca net_sked\n");
  exit(0);
  //
  // DEBUG printf("\n FINE COMANDO retval=%d direct=%s\n",retval, get_work_directory);
}
