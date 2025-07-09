/*
 *  Main program
 */

#include <stdio.h>

#define TIMELOOP 2
#define TIMEMIN 0.01
#define TIMEMAX 30
#include "viewshr.h"
#include "sim_types.h"
#include "sked.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include "uni_mis.h"
// #include "libutilx.h"

extern int cerca_umis(char *, int);
extern void init_umis();

int usage();
void SetUp(int, char **);
void chdefaults();
void effetto();
void sospendi(unsigned int);
void costruisci_var(char **, VARIABILI **, int *);

double timeloop, timeprint, conta;
int passo;
unsigned int timemilli;
static char *progname; /* nome di questo programma */
static char nomevar[10], FormatoStampa[10], retstr[10], pathloc[256];
int modo, indir, stato, num_var, viewshr(), forza, server, kston;
float valore, tempo, valprec, forzval;

extern S_UNI_MIS uni_mis[]; /* tabella che descrive tutte le unita'
                                di misura  */

sqlite3 *db;

#define stampa(str, val1, val2) printf("\t%s\t%f\t%f\n", str, val1, val2);
#define stampa_kst(str, val1, val2)          \
  printf("\t%s\t%f\t%f\n", str, val1, val2); \
  fflush(stdout);
// #define stampaeff(str1,str2,val) printf("\t%s\t\t%s\t\t%f\r",str1,str2,val)
#define stampaeff(str1, str2, val) printf(" %s\t%s\t\t\t\t%f\r", str1, str2, val)
#define stampa_server(str, val1, val2) printf("%f  %s  %f\n", val1, str, val2)
#define stampa_server_gen(str, val1, val2) printf(FormatoStampa, val1, str, val2)

int usage()
{

  fprintf(stderr,
          "\nuso:  %s nome_variabile [-opzioni ... ]\n",
          progname);

  fprintf(stderr,
          "le opzioni sono:\n");
  fprintf(stderr, "    -t tempoloop       tempo di scansione. \n");
  fprintf(stderr, "                       Passo di tempo di controllo di una\n");
  fprintf(stderr, "                       eventuale variazione nel valore della\n");
  fprintf(stderr, "                       variabile.\n\n");
  fprintf(stderr, "    -p tempoprint      tempo di stampa forzata.\n");
  fprintf(stderr, "                       Allo scadere di tempoprint viene forzata\n");
  fprintf(stderr, "                       la stampa della variabile, anche se \n");
  fprintf(stderr, "                       non e` variata.\n\n");
  fprintf(stderr, "    -f valore_forzato  valore (float) da assegnare alla variabile\n");
  fprintf(stderr, "                       Si ignorano le opzioni -t e -p\n\n");
  fprintf(stderr, "    -s %%f              modo server: resta in attesa del nome della variabile\n");
  fprintf(stderr, "                       facoltativo è il formato di stampa %%f (default %%.4g)\n\n");
  fprintf(stderr, "                       %%STOP%% termina l esecuzione\n");

  /*
      fprintf (stderr, "\n\nSi ricorda che %s legge e scrive i seguenti file.\n", progname);
      fprintf (stderr, "file di  input: LISTA_VARLOG     lista delle variabili (nome + descrizione)\n");
      fprintf (stderr, "file di output: VAR.LOG          listato degli eventi\n");
  */
  fprintf(stderr, "\n");
  exit(1);
}

int main(argc, argv)
int argc;
char **argv;
{
  int num_var_log[1];
  char str_app[256], str2[20];
  int iumis, defumis;
  float valore_conv;

  SetUp(argc, argv);
  // leggo le unità di misura di default
  getcwd(pathloc, 256);
  chdefaults();
  init_umis();
  chdir(pathloc);

  // printf("nomevar=%s\n", nomevar);

  viewshr(INIZIALIZZA, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
  if (server)
  {
    //        printf("server ready...\n");
    fflush(stdout);
  ciclo:
    scanf("%s", nomevar);
    if (strcmp(nomevar, "%STOP%") == 0)
      exit(0);
    if (!viewshr(GETIND, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval))
    {
      printf("ERRORE server viewval - VARIABILE %s NON ESISTENTE\n", nomevar);
      fflush(stdout);
    }
    else
    {
      viewshr(GETVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
      iumis = cerca_umis(nomevar, 1); // la chiamo con 1 per evitare di stampare il messaggio di errore se non trova l'unità di misura
      defumis = uni_mis[iumis].sel;
      strcpy(str_app, uni_mis[iumis].codm[defumis]);
      strcat(str_app, " ");
      strcat(str_app, nomevar);
      valore_conv = uni_mis[iumis].A[defumis] * valore + uni_mis[iumis].B[defumis];
      stampa_server_gen(str_app, valore_conv, tempo);
      fflush(stdout);
    }
    goto ciclo;
  };

  if (!viewshr(GETIND, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval))
  {
    printf("variabile %s non trovata\n", nomevar);
    exit(0);
  }
  viewshr(GETVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
  printf("\n\tVariabile\tValore\t\tTempo\n");
  stampa(nomevar, valore, tempo);

  if (forza)
  {
    viewshr(PUTVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
    viewshr(GETVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
    printf("-->\b\b\b");
    stampa(nomevar, valore, tempo);
    exit(0);
  }

  if ((timeloop <= 0) && (timeprint <= 0))
    exit(0);
  valprec = valore;

  if (timeloop > 0)
    timemilli = (unsigned int)(timeloop * 1000.);
  else
    timemilli = (unsigned int)(timeprint * 1000.);

  while (1)
  {
    viewshr(CHECK, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
    if (stato == STATO_STOP)
    {
      fprintf(stderr, "\n---------\n%s termina. Simulatore in STOP \n---------\n", argv[0]);
      exit(0);
    }
    else if (stato == STATO_ERRORE)
    {
      fprintf(stderr, "\n---------\n%s termina. Simulatore in ERRORE \n---------\n", argv[0]);
      exit(0);
    }

    if (!kston)
      effetto();
    //         sleep ( (unsigned int)timeloop );
    sospendi(timemilli);
    // DEBUG fprintf( stderr, "\n---------> %d %d %d\n",++passo,timemilli,timeloop );
    if (timeprint >= 0)
    {
      conta = conta - timeloop;
      if ((conta <= 0) || (timeloop <= 0))
      {
        viewshr(GETVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
        stampa(nomevar, valore, tempo);
        conta = timeprint;
      }
    }
    if (stato == STATO_FREEZE)
    {
      continue;
      // DEBUG        fprintf( stderr, "Simulatore in FREEZE \n");
    }
    else
    {
      viewshr(GETVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
      if (kston)
      {
        stampa_kst(nomevar, valore, tempo);
      }
      else if (valprec != valore)
      {
        stampa(nomevar, valore, tempo);
      }
      valprec = valore;
    }
  }
}

void SetUp(argc, argv) int argc;
char **argv;
{
  int i;

  progname = argv[0];

  timeprint = -1;
  timeloop = -1;
  forza = FALSE;
  server = FALSE;
  kston = FALSE;

  /* Se non ci sono parametri chiama usage */
  if (argc < 2)
    usage();

  /* Parse command line */
  for (i = 1; i < argc; i++)
  {
    char *arg = argv[i];

    if (arg[0] == '-')
    {

      switch (arg[1])
      {
      case 's': /* -s stampa server */
                //		   if (++i >= argc) usage ();
        server = TRUE;
        // Mi aspetto una stringa con il formato di stampa del valore, ad esempio %.3f, il default è %.4g
        strcpy(FormatoStampa, "%.4g");
        if (++i < argc)
        {
          if (*argv[i] == '%')
          {
            strcpy(FormatoStampa, argv[i]);
            //		   		   printf("Trovato il percento argc=%d argv[%d]=%s  , server con formato %s ", argc,i, argv[i],  FormatoStampa);
          }
        }
        printf("server con formato %s.  ", FormatoStampa);
        strcat(FormatoStampa, "  %s  %f\n");
        //		   printf("server con formato %s.  ", FormatoStampa);
        continue;
      case 't': /* -t tempo di loop */
        if (++i >= argc)
          usage();
        timeloop = atof(argv[i]);
        fprintf(stderr, "Valore timeloop=%lf\n", timeloop);
        if (timeloop <= TIMEMIN)
        {
          fprintf(stderr, "Valore non corretto del tempo di loop\n Si imposta il valore minimo %lf\n", TIMEMIN);
          timeloop = TIMEMIN;
        }
        else if (timeloop >= TIMEMAX)
        {
          fprintf(stderr, "Valore non corretto del tempo di loop\n Si imposta il valore massimo %lf\n", TIMEMAX);
          timeloop = TIMEMAX;
        }
        continue;
      case 'k': /* -k tempo di loop per kst*/
        if (++i >= argc)
          usage();
        kston = TRUE;
        timeloop = atof(argv[i]);
        fprintf(stderr, "Valore timeloop=%lf\n", timeloop);
        if (timeloop <= TIMEMIN)
        {
          fprintf(stderr, "Valore non corretto del tempo di loop\n Si imposta il valore minimo %lf\n", TIMEMIN);
          timeloop = TIMEMIN;
        }
        else if (timeloop >= TIMEMAX)
        {
          fprintf(stderr, "Valore non corretto del tempo di loop\n Si imposta il valore massimo %lf\n", TIMEMAX);
          timeloop = TIMEMAX;
        }
        continue;
      case 'p': /* -p tempo di stampa */
        if (++i >= argc)
          usage();
        timeprint = atof(argv[i]);
        conta = timeprint;
        fprintf(stderr, "Valore timeprint=%lf\n", timeprint);
        continue;
      case 'f': /* -p tempo di stampa */
        if (++i >= argc)
          usage();
        forzval = atof(argv[i]);
        fprintf(stderr, "Valore forzato=%lf\n", forzval);
        forza = TRUE;
        continue;
      default:
        usage();
      }
    }
    else
      strcpy(nomevar, argv[1]);
  }
}

void effetto()
{
#define NUMEFF 4
  const char *tabeff[] = {"\|", "\\", "\-", "\/"};
  static int ieff;

  ieff++;
  //    fprintf(stderr,"        %s %s\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b",tabeff[ieff%NUMEFF],( (stato == STATO_FREEZE ) ? "FREEZE" : "RUN   ") );
  stampaeff(((stato == STATO_FREEZE) ? "FREEZE" : "RUN   "), tabeff[ieff % NUMEFF], tempo);
  fflush(stdout);
}
