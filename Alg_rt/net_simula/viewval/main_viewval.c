/*
 *  Main program viewval - Extended with interactive selector, units, save/load
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "viewshr.h"
#include "sim_types.h"
#include "sked.h"
#include <sqlite3.h>
#include "uni_mis.h"
#include <math.h>
#include <time.h>
#include <sys/ioctl.h>

#define TIMELOOP 2
#define TIMEMIN 0.01
#define TIMEMAX 30

extern int cerca_umis(char *, int);
extern void init_umis();

// Forward declarations
int usage();
void SetUp(int, char **);
void chdefaults();
void effetto();
void sospendi(unsigned int);
void costruisci_var(char **, VARIABILI **, int *);
int viewshr(int, char *, int *, float *, int *, float *, int*, float );


// Global variables for viewval
double timeloop, timeprint, conta;
int passo;
unsigned int timemilli;
static char *progname;
static char nomevar[MAX_LUN_NOME_VAR], FormatoStampa[10], pathloc[256];
int modo, indir, stato, num_var, forza, server, kston, interactive_mode; // Added interactive_mode
float valore, tempo, valprec, forzval;
char *save_selection_file = NULL; // Per -S
char *load_selection_file = NULL; // Per -L
char *log_file = NULL;            // Per -l: traccia delle scritture in SHM
static FILE *log_fp = NULL;

extern S_UNI_MIS uni_mis[];
extern int tot_variabili;
extern VARIABILI *variabili;

sqlite3 *db;

// =============================================================================
// Interactive Selector Code Section (integrated from selector_final.c)
// =============================================================================
#ifdef _WIN32
    #include <conio.h>
    #define KEY_UP 72
    #define KEY_DOWN 80
    #define KEY_HOME 71
    #define KEY_END 79
    #define KEY_PGUP 73
    #define KEY_PGDN 81
    #define KEY_ENTER 13
    #define KEY_BACKSPACE 8
    #define KEY_ESCAPE 27
    #define KEY_SPACE ' '
#else
    #include <termios.h>
    #include <sys/time.h>
    #define KEY_ENTER 10
    #define KEY_BACKSPACE 127
    #define KEY_ESCAPE 27
    #define KEY_SPACE ' '
    #define KEY_UP 256
    #define KEY_DOWN 257
    #define KEY_HOME 258
    #define KEY_END 259
    #define KEY_PGUP 260
    #define KEY_PGDN 261
#endif
#define CLEAR_SCREEN "clear"
#define ANSI_REVERSE "\033[7m"
#define ANSI_RESET   "\033[0m"
#define WINDOW_HEIGHT 25

typedef struct {
    char** nomi;
    int count;
} SelezioneMultipla;

typedef enum { MODE_NAVIGATION, MODE_GOTO, MODE_SEARCH } UIMode;
typedef enum { SEARCH_FORWARD, SEARCH_BACKWARD } SearchDirection;

// Forward declarations for selector functions
SelezioneMultipla choose_from_file(const char* nome_file, int multi_select, int show_numbers);
int get_key();
void adjust_window(int s_idx, int* w_top, int l_count);
int find_pattern(FILE* f, long* off, int c, const char* p, int s, SearchDirection d);
#ifndef _WIN32
int getch_unix(void);
void enable_raw_mode(void);
void disable_raw_mode(void);
#endif
int check_keyboard_hit(void);
void free_selection(SelezioneMultipla* s);
// =============================================================================


// viewval print MACROS
#define stampa(str, val1, val2) printf("\t%-10s\t%f\t%f\n", str, val1, val2)
#define stampa_kst(str, val1, val2) printf("\t%s\t%f\t%f\n", str, val1, val2); fflush(stdout);
#define stampaeff(str1, str2, val) printf(" %s\t%s\t\t\t\t%f\r", str1, str2, val)
#define stampa_server_gen(str, val1, val2) printf(FormatoStampa, val1, str, val2)

int usage() {
    fprintf(stderr, "\nuso:  %s [nome_variabile | -i] [-opzioni ... ]\n", progname);
    fprintf(stderr, "le opzioni sono:\n");
    fprintf(stderr, "    -i                 avvia in modalita' interattiva.\n");
    fprintf(stderr, "    -L <file>          carica le variabili da <file> e avvia la visualizzazione (-i richiesto).\n");
    fprintf(stderr, "    -S <file>          salva le variabili selezionate in <file> (-i richiesto).\n\n");
    fprintf(stderr, "    -l <file>          registra in <file> ogni valore scritto in SHM.\n\n");
    fprintf(stderr, "    -t tempoloop       tempo di scansione (default %.1f s).\n", (double)TIMELOOP);
    fprintf(stderr, "    -p tempoprint      tempo di stampa forzata.\n");
    fprintf(stderr, "    -f valore_forzato  forza un valore (float) e termina.\n");
    fprintf(stderr, "    -s [formato]       modo server, attende nomi variabili su stdin.\n");
    fprintf(stderr, "    -k                 output per KST.\n");
    fprintf(stderr, "\n");
    exit(1);
}

// NUOVE funzioni per salvare e caricare la selezione
void save_selection_to_file(const char* filename, char** nomi, int count) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("Errore salvataggio selezione");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s\n", nomi[i]);
    }
    fclose(f);
    printf("Selezione salvata in '%s'.\n", filename);
}

void load_selection_from_file(const char* filename, SelezioneMultipla* sel) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("Errore caricamento selezione");
        sel->count = 0;
        sel->nomi = NULL;
        return;
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), f)) {
        buffer[strcspn(buffer, "\n\r")] = 0; // Rimuove newline
        sel->count++;
        sel->nomi = realloc(sel->nomi, sel->count * sizeof(char*));
        sel->nomi[sel->count - 1] = strdup(buffer);
    }
    fclose(f);
    printf("Caricate %d variabili da '%s'.\n", sel->count, filename);
}


// =============================================================================
// Tabella interattiva: visualizzazione + scrittura dei valori in SHM
// =============================================================================
//  Il loop originale dormiva timeloop secondi e poi ridisegnava, leggendo la
//  tastiera con un getchar() nudo: bastava per 'i'/'q' ma non per le frecce
//  (che arrivano come sequenza ESC [ A, spalmata su tre cicli) ne' per
//  scrivere dentro una cella. Qui refresh e tastiera sono disaccoppiati: si
//  attende un tasto per al massimo POLL_MS e si ridisegna quando serve.
//
//  La scrittura usa la stessa primitiva di "viewval TAG -f valore":
//  viewshr(PUTVAR) -> RtDbPPutValue(), cioe' un float depositato nel DB punti.
//  I valori si digitano nelle UNITA' VISUALIZZATE e vengono riconvertiti in
//  unita' interne con l'inversa della tabella uni_mis.
// =============================================================================

#define POLL_MS         50    /* granularita' di attesa sulla tastiera        */
#define ESC_TIMEOUT_MS  30    /* oltre, un ESC e' un ESC vero, non una sequenza */
#define RIGA_PRIMA       5    /* prima riga di tabella sullo schermo (1-based) */
#define COL_VALORE      59    /* colonna d'inizio della cella "Valore"        */
#define CHROME           7    /* righe non-tabella (titolo, righelli, prompt) */
#define MAX_INPUT       32

static const char *RIGHELLO =
    "----------------------------------------------------------------------------------------";

typedef enum { MODO_VISTA, MODO_EDIT } ModoTabella;

static long ora_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000L + tv.tv_usec / 1000L;
}

/*  1 se entro ms millisecondi c'e' almeno un byte leggibile su stdin.  */
static int tastiera_pronta(int ms) {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec  = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

/*  Come get_key(), ma non si blocca sul secondo byte: get_key() dopo un ESC
    chiama getch_unix() in lettura bloccante, quindi un ESC isolato -- che qui
    serve per annullare l'edit -- resterebbe appeso fino al tasto successivo.
    Richiede stdin non bufferizzato (setvbuf piu' sotto), altrimenti i byte
    gia' assorbiti da stdio sono invisibili a select().  */
static int leggi_tasto(void) {
    int c = getchar();
    if (c == EOF) return EOF;      /* stdin chiuso (non e' un terminale): si esce */
    if (c != KEY_ESCAPE) return c;
    if (!tastiera_pronta(ESC_TIMEOUT_MS)) return KEY_ESCAPE;
    if (getchar() != '[') return KEY_ESCAPE;
    switch (getchar()) {
        case 'A': return KEY_UP;
        case 'B': return KEY_DOWN;
        case 'H': return KEY_HOME;
        case 'F': return KEY_END;
        case '1': return (getchar() == '~') ? KEY_HOME : KEY_ESCAPE;
        case '4': return (getchar() == '~') ? KEY_END  : KEY_ESCAPE;
        case '5': return (getchar() == '~') ? KEY_PGUP : KEY_ESCAPE;
        case '6': return (getchar() == '~') ? KEY_PGDN : KEY_ESCAPE;
    }
    return KEY_ESCAPE;
}

/*  Righe utili del terminale; WINDOW_HEIGHT resta il ripiego se la ioctl non
    risponde (pipe, terminale non riconosciuto).  */
static int righe_terminale(void) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_row > CHROME + 1)
        return ws.ws_row;
    return WINDOW_HEIGHT + CHROME;
}

static void apri_log(void) {
    time_t adesso;
    char quando[64];

    if (!log_file) return;
    log_fp = fopen(log_file, "a");
    if (!log_fp) {
        fprintf(stderr, "%s: non riesco ad aprire il log '%s'\n", progname, log_file);
        return;
    }
    adesso = time(NULL);
    strftime(quando, sizeof(quando), "%Y-%m-%d %H:%M:%S", localtime(&adesso));
    fprintf(log_fp, "# viewval - sessione avviata %s\n", quando);
    fflush(log_fp);
}

static void traccia_scrittura(const char *nome, float vecchio_int, float nuovo_int,
                              float vecchio_vis, float nuovo_vis,
                              const char *umis, float tsim) {
    time_t adesso;
    char quando[64];

    if (!log_fp) return;
    adesso = time(NULL);
    strftime(quando, sizeof(quando), "%Y-%m-%d %H:%M:%S", localtime(&adesso));
    fprintf(log_fp, "%s  tsim=%10.3f  %-12s  interno %g -> %g   visual %g -> %g %s\n",
            quando, tsim, nome, vecchio_int, nuovo_int, vecchio_vis, nuovo_vis, umis);
    fflush(log_fp);
}

/*  Inversa della conversione usata per la stampa (visuale = A*interno + B).  */
static float da_visuale_a_interno(int iumis, int defumis, float visuale) {
    float A = uni_mis[iumis].A[defumis];
    float B = uni_mis[iumis].B[defumis];
    if (A == 0.0f) return visuale;      /* tabella malformata: nessuna conversione */
    return (visuale - B) / A;
}

/*  Ritorna 1 se l'utente vuole tornare al selettore ('i'), 0 per uscire.  */
static int tabella_interattiva(char **nomi, char **descr, int *indirizzi, int n) {
    ModoTabella modo = MODO_VISTA;
    int   cursore = 0, finestra = 0, riseleziona = 0, esci = 0;
    long  prossimo = 0;
    char  input[MAX_INPUT] = "";
    char  msg[200] = "";
    float *val;
    float tempo_vis = 0.0f;
    /*  Riga da sorvegliare dopo una scrittura: se il punto non e' un ingresso
        il modello lo ricalcola al passo dopo e il valore forzato sparisce.  */
    int   sorveglia = -1;
    float sorveglia_val = 0.0f, sorveglia_tempo = 0.0f;

    val = calloc(n, sizeof(float));
    if (!val) return 0;

    setvbuf(stdin, NULL, _IONBF, 0);
    printf("\033[2J");

    while (!esci && !riseleziona) {
        int righe = righe_terminale() - CHROME;
        long adesso = ora_ms();
        int i, key, iu, du;

        if (righe < 1) righe = 1;

        /*  Stato del simulatore: letto sempre, anche a tabella congelata,
            altrimenti uno STOP durante l'edit non verrebbe notato.  */
        viewshr(CHECK, NULL, NULL, NULL, &stato, &tempo, NULL, 0);
        if (stato == STATO_STOP || stato == STATO_ERRORE) {
            printf("\033[?25h\n");
            fflush(stdout);
            fprintf(stderr, "\n%s termina. Simulatore in STOP/ERRORE.\n", progname);
            break;
        }

        /*  Rilettura dei valori: sospesa durante l'edit, cosi' la tabella non
            si muove sotto le dita mentre si digita.  */
        if (modo == MODO_VISTA && adesso >= prossimo) {
            for (i = 0; i < n; i++)
                viewshr(GETVAR, nomi[i], &indirizzi[i], &val[i], &stato, &tempo, &num_var, 0);
            tempo_vis = tempo;
            prossimo  = adesso + (long)timemilli;

            if (sorveglia >= 0 && tempo_vis > sorveglia_tempo) {
                if (fabsf(val[sorveglia] - sorveglia_val) >
                    1e-6f * (fabsf(sorveglia_val) + 1.0f))
                    snprintf(msg, sizeof(msg),
                             "%s ricalcolata dal modello: non e' un ingresso, "
                             "il valore forzato non resta.", nomi[sorveglia]);
                sorveglia = -1;
            }
        }

        /* ---- finestra visibile ---- */
        if (cursore < finestra) finestra = cursore;
        if (cursore >= finestra + righe) finestra = cursore - righe + 1;
        if (finestra > n - righe) finestra = n - righe;
        if (finestra < 0) finestra = 0;

        /* ---- disegno (niente system("clear"): niente fork e niente sfarfallio) ---- */
        printf("\033[?25l\033[H");
        printf("viewval - Tempo Sim: %.2f   %s   [%d/%d]\033[K\n",
               tempo_vis, (stato == STATO_FREEZE) ? "FREEZE" : "RUN   ",
               cursore + 1, n);
        printf("%s\033[K\n", RIGHELLO);
        printf("%-12s | %-40s | %-15s | %s\033[K\n",
               "Variabile", "Descrizione", "Valore", "Unita'");
        printf("%s\033[K\n", RIGHELLO);

        for (i = 0; i < righe; i++) {
            int r = finestra + i;
            char cella[MAX_INPUT + 16];

            if (r >= n) { printf("\033[K\n"); continue; }
            iu = cerca_umis(nomi[r], 1);
            du = uni_mis[iu].sel;
            if (modo == MODO_EDIT && r == cursore)
                snprintf(cella, sizeof(cella), "%-15s", input);
            else
                snprintf(cella, sizeof(cella), "%-15.4f",
                         uni_mis[iu].A[du] * val[r] + uni_mis[iu].B[du]);

            if (r == cursore) printf("%s", ANSI_REVERSE);
            printf("%-12s | %-40.40s | %s | %s",
                   nomi[r], descr[r] ? descr[r] : "", cella, uni_mis[iu].codm[du]);
            if (r == cursore) printf("%s", ANSI_RESET);
            printf("\033[K\n");
        }

        printf("%s\033[K\n", RIGHELLO);
        iu = cerca_umis(nomi[cursore], 1);
        du = uni_mis[iu].sel;
        if (modo == MODO_EDIT)
            printf("Nuovo valore per %s (attuale %.4f %s) - Invio scrive, ESC annulla\033[K\n",
                   nomi[cursore],
                   uni_mis[iu].A[du] * val[cursore] + uni_mis[iu].B[du],
                   uni_mis[iu].codm[du]);
        else
            printf("Frecce/PgUp/PgDn/Home/End: scorri | f o Invio: modifica | "
                   "i: riseleziona | q: esci\033[K\n");
        printf("%s\033[K\033[J", msg);

        /*  In edit il cursore vero del terminale va dove si sta scrivendo,
            cioe' nella cella del valore della riga selezionata.  */
        if (modo == MODO_EDIT)
            printf("\033[%d;%dH\033[?25h",
                   RIGA_PRIMA + (cursore - finestra), COL_VALORE + (int)strlen(input));
        fflush(stdout);

        /* ---- tastiera ---- */
        if (!tastiera_pronta(POLL_MS)) continue;
        key = leggi_tasto();
        if (key == EOF) { esci = 1; continue; }

        if (modo == MODO_VISTA) {
            switch (key) {
                case KEY_UP:   if (cursore > 0) cursore--;     break;
                case KEY_DOWN: if (cursore < n - 1) cursore++; break;
                case KEY_PGUP: cursore -= righe; if (cursore < 0) cursore = 0;          break;
                case KEY_PGDN: cursore += righe; if (cursore > n - 1) cursore = n - 1;  break;
                case KEY_HOME: cursore = 0;     break;
                case KEY_END:  cursore = n - 1; break;
                case 'f': case KEY_ENTER: case 13:
                    modo = MODO_EDIT;
                    input[0] = '\0';
                    msg[0] = '\0';
                    break;
                case 'i': riseleziona = 1; break;
                case 'q': esci = 1;        break;
            }
        } else {
            size_t l = strlen(input);

            if (key == KEY_ESCAPE) {
                modo = MODO_VISTA;
                input[0] = '\0';
                snprintf(msg, sizeof(msg), "Modifica annullata: niente scritto.");
            } else if (key == KEY_ENTER || key == 13) {
                modo = MODO_VISTA;
                if (l == 0) {
                    snprintf(msg, sizeof(msg), "Nessun valore digitato: niente scritto.");
                } else {
                    char *fine;
                    double v = strtod(input, &fine);
                    while (*fine == ' ') fine++;
                    if (*fine != '\0') {
                        snprintf(msg, sizeof(msg),
                                 "'%s' non e' un numero: niente scritto.", input);
                    } else {
                        float vecchio_int = val[cursore];
                        float vecchio_vis, nuovo_int;

                        iu = cerca_umis(nomi[cursore], 1);
                        du = uni_mis[iu].sel;
                        vecchio_vis = uni_mis[iu].A[du] * vecchio_int + uni_mis[iu].B[du];
                        nuovo_int   = da_visuale_a_interno(iu, du, (float)v);

                        viewshr(PUTVAR, nomi[cursore], &indirizzi[cursore],
                                &valore, &stato, &tempo, &num_var, nuovo_int);
                        viewshr(GETVAR, nomi[cursore], &indirizzi[cursore],
                                &val[cursore], &stato, &tempo, &num_var, 0);

                        snprintf(msg, sizeof(msg),
                                 "%s scritta: %.4f -> %.4f %s   (interno %g)",
                                 nomi[cursore], vecchio_vis, (float)v,
                                 uni_mis[iu].codm[du], nuovo_int);
                        traccia_scrittura(nomi[cursore], vecchio_int, nuovo_int,
                                          vecchio_vis, (float)v,
                                          uni_mis[iu].codm[du], tempo);
                        sorveglia       = cursore;
                        sorveglia_val   = nuovo_int;
                        sorveglia_tempo = tempo;
                    }
                }
                input[0] = '\0';
            } else if (key == KEY_BACKSPACE || key == 8) {
                if (l) input[l - 1] = '\0';
            } else if (l < MAX_INPUT - 1 && key > 0 && key < 256 &&
                       (isdigit(key) || strchr("+-.eE", key))) {
                input[l]     = (char)key;
                input[l + 1] = '\0';
            }
        }
    }

    printf("\033[?25h");
    fflush(stdout);
    free(val);
    return riseleziona;
}

int main(int argc, char **argv) {
    char str_app[256];
    int iumis, defumis;
    float valore_conv;

    SetUp(argc, argv);
    /* init_umis e' self-contained: cerca ./uni_misc.cfg (per-simulazione),
       poi ./uni_misc.dat, poi $HOME/defaults/uni_misc.dat; ripristina la cwd */
    init_umis();

    viewshr(INIZIALIZZA, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
    apri_log();

    // ================== LOGICA INTERATTIVA COMPLETAMENTE RISTRUTTURATA ==================
    if (interactive_mode) {
        int return_to_select = 1;

        while(return_to_select) {
            return_to_select = 0; // Default: non tornare alla selezione
            SelezioneMultipla scelta = {NULL, 0};

            // 1. Carica la selezione da file o avvia il selettore interattivo
            if (load_selection_file) {
                load_selection_from_file(load_selection_file, &scelta);
                load_selection_file = NULL; // Carica solo una volta
            } else {
                const char* tmp_filename = "viewval_vars.tmp";
                FILE* tmp_file = fopen(tmp_filename, "w");
                if (tmp_file) {
                    // *** MODIFICA 1: Scrive nome E descrizione nel file temporaneo ***
                    for (int i = 0; i < tot_variabili; i++) {
                        // Allinea il nome a sinistra e poi aggiunge la descrizione
                        fprintf(tmp_file, "%-12.*s %s\n", 
                                MAX_LUN_NOME_VAR, variabili[i].nome, 
                                variabili[i].descr);
                    }
                    fclose(tmp_file);
                    scelta = choose_from_file(tmp_filename, 1, 1);
                    remove(tmp_filename);
                }
            }

            if (scelta.count == 0) {
                printf("Nessuna variabile da visualizzare. Uscita.\n");
                break; // Esce dal loop while(return_to_select)
            }
            
            // 2. Prepara le variabili valide per la visualizzazione
            int* indirizzi = malloc(scelta.count * sizeof(int));
            char** nomi_validi = malloc(scelta.count * sizeof(char*));
            char** descrizioni_valide = calloc(scelta.count, sizeof(char*));
            int valid_vars_count = 0;
            
            char nome_temp[MAX_LUN_NOME_VAR + 1];

            for (int i = 0; i < scelta.count; i++) {
                // Estrae solo il nome dalla riga selezionata (es. "NOME   DESCRIZIONE")
                sscanf(scelta.nomi[i], "%s", nome_temp);

                if (viewshr(GETIND, nome_temp, &indirizzi[valid_vars_count], NULL, NULL, NULL, NULL, 0)) {
                    nomi_validi[valid_vars_count] = strdup(nome_temp);
                    
                    // Descrizione dall'array globale 'variabili'. Il confronto e'
                    // esatto: con strncmp(.., strlen(nome)) un tag corto
                    // agganciava la descrizione di uno piu' lungo che iniziava
                    // allo stesso modo.
                    for (int j = 0; j < tot_variabili; j++) {
                        if (strncmp(nome_temp, variabili[j].nome, MAX_LUN_NOME_VAR) == 0) {
                             descrizioni_valide[valid_vars_count] = strdup(variabili[j].descr);
                             break;
                        }
                    }
                    if (!descrizioni_valide[valid_vars_count])
                        descrizioni_valide[valid_vars_count] = strdup("");
                    valid_vars_count++;
                }
            }
            if (save_selection_file) { // Salva dopo aver validato
                save_selection_to_file(save_selection_file, nomi_validi, valid_vars_count);
                save_selection_file = NULL; // Salva solo una volta
            }
            free_selection(&scelta);

            if (valid_vars_count == 0) { printf("Nessuna variabile valida trovata. Uscita.\n"); break; }

            if (timeloop <= 0) timeloop = TIMELOOP;
            timemilli = (unsigned int)(timeloop * 1000.);
            
            // 3. Loop di visualizzazione + editing (vedi tabella_interattiva)
            #ifndef _WIN32
            enable_raw_mode();
            #endif

            return_to_select = tabella_interattiva(nomi_validi, descrizioni_valide,
                                                   indirizzi, valid_vars_count);
            // Pulizia prima di un eventuale nuovo ciclo di selezione
            // *** RIPRISTINO MODALITA' NORMALE DEL TERMINALE ***
            #ifndef _WIN32
            disable_raw_mode();
            #endif

            for(int i = 0; i < valid_vars_count; i++) {
                free(nomi_validi[i]);
                free(descrizioni_valide[i]); // Pulisce la memoria della descrizione
            }
            free(nomi_validi);
            free(descrizioni_valide);
            free(indirizzi);
        }
        exit(0);
    }

    // ================== FINE LOGICA INTERATTIVA RISTRUTTURATA ==================
    

    // Original viewval logic continues here...
    if (server) {
        /* Handshake di avvio: emette UNA riga su stdout appena la SHM e'
           stata agganciata con successo (viewshr(INIZIALIZZA) e' gia'
           ritornato qui sopra). Il lato Tcl (animate.tcl) fa un gets
           BLOCCANTE subito dopo aver aperto la pipe "| viewval -s": senza
           questa riga l'event loop Tcl si blocca all'infinito (freeze
           recuperabile solo con kill -9). Storicamente la riga era fornita
           "per caso" dal printf("direttorio def...") di chdefaults(),
           rimosso con il refactor umis_chdefaults() in uni_mis.c. */
        printf("VIEWVAL READY\n");
        fflush(stdout);
    ciclo:
        scanf("%s", nomevar);
        if (strcmp(nomevar, "%STOP%") == 0) exit(0);
        if (!viewshr(GETIND, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval)) {
            printf("ERRORE server viewval - VARIABILE %s NON ESISTENTE\n", nomevar);
            fflush(stdout);
        } else {
            viewshr(GETVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
            iumis = cerca_umis(nomevar, 1);
            defumis = uni_mis[iumis].sel;
            strcpy(str_app, uni_mis[iumis].codm[defumis]);
            strcat(str_app, " ");
            strcat(str_app, nomevar);
            valore_conv = uni_mis[iumis].A[defumis] * valore + uni_mis[iumis].B[defumis];
            stampa_server_gen(str_app, valore_conv, tempo);
            fflush(stdout);
        }
        goto ciclo;
    }

    if (!viewshr(GETIND, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval)) {
        printf("variabile %s non trovata\n", nomevar);
        exit(0);
    }
    viewshr(GETVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
    printf("\n\tVariabile\tValore\t\tTempo\n");
    stampa(nomevar, valore, tempo);

    if (forza) {
        float vecchio = valore;   /* letto dalla GETVAR qui sopra */
        viewshr(PUTVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
        viewshr(GETVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
        printf("-->\b\b\b");
        stampa(nomevar, valore, tempo);
        /*  -f lavora in unita' interne, non converte: nel log le due coppie
            coincidono e l'unita' e' segnata come tale.  */
        traccia_scrittura(nomevar, vecchio, forzval, vecchio, forzval, "(-f, interne)", tempo);
        exit(0);
    }

    if ((timeloop <= 0) && (timeprint <= 0)) exit(0);
    valprec = valore;

    if (timeloop > 0) timemilli = (unsigned int)(timeloop * 1000.);
    else if (timeprint > 0) timemilli = (unsigned int)(timeprint * 1000.);
    else exit(0);

    while (1) {
        viewshr(CHECK, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
        if (stato == STATO_STOP || stato == STATO_ERRORE) {
            fprintf(stderr, "\n---------\n%s termina. Simulatore in STOP/ERRORE \n---------\n", progname);
            exit(0);
        }

        if (!kston) effetto();
        sospendi(timemilli);
        
        if (timeprint > 0) {
            conta -= timeloop > 0 ? timeloop : timeprint;
            if (conta <= 0) {
                viewshr(GETVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
                stampa(nomevar, valore, tempo);
                conta = timeprint;
            }
        }
        if (stato == STATO_FREEZE) {
            continue;
        } else {
            viewshr(GETVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
            if (kston) {
                stampa_kst(nomevar, valore, tempo);
            } else if (valprec != valore) {
                stampa(nomevar, valore, tempo);
            }
            valprec = valore;
        }
    }
}


void SetUp(int argc, char **argv) {
    progname = argv[0];

    timeprint = -1;
    timeloop = -1;
    forza = FALSE;
    server = FALSE;
    kston = FALSE;
    interactive_mode = FALSE;
    nomevar[0] = '\0';
    load_selection_file = NULL;
    save_selection_file = NULL;
    // ...

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (arg[0] == '-') {
            switch (arg[1]) {
                case 'i': interactive_mode = TRUE;
               continue;
                case 'L': 
                    if (++i >= argc) usage();
                    load_selection_file = argv[i];
                    interactive_mode = TRUE; // -L implica -i
                    continue;
                case 'S':
                    if (++i >= argc) usage();
                    save_selection_file = argv[i];
                    interactive_mode = TRUE; // -S implica -i
                    continue;
                case 'l':
                    if (++i >= argc) usage();
                    log_file = argv[i];
                    continue;
                case 's':
                    server = TRUE;
                    strcpy(FormatoStampa, "%.4g");
                    if (i + 1 < argc && argv[i + 1][0] == '%') {
                        strcpy(FormatoStampa, argv[++i]);
                    }
                    strcat(FormatoStampa, "  %s  %f\n");
                    continue;
                case 't':
                    if (++i >= argc) usage();
                    timeloop = atof(argv[i]);
                    if (timeloop <= TIMEMIN) timeloop = TIMEMIN;
                    if (timeloop >= TIMEMAX) timeloop = TIMEMAX;
                    continue;
                case 'k':
                    if (++i >= argc) usage();
                    kston = TRUE;
                    timeloop = atof(argv[i]);
                    if (timeloop <= TIMEMIN) timeloop = TIMEMIN;
                    if (timeloop >= TIMEMAX) timeloop = TIMEMAX;
                    continue;
                case 'p':
                    if (++i >= argc) usage();
                    timeprint = atof(argv[i]);
                    conta = timeprint;
                    continue;
                case 'f':
                    if (++i >= argc) usage();
                    forzval = atof(argv[i]);
                    forza = TRUE;
                    continue;
                default:
                    usage();
            }
        } else {
            if (!interactive_mode) {
                strncpy(nomevar, arg, MAX_LUN_NOME_VAR - 1);
                nomevar[MAX_LUN_NOME_VAR - 1] = '\0';
            }
        }
    }

    if (!interactive_mode && !server && nomevar[0] == '\0') {
         fprintf(stderr, "Errore: specificare un nome di variabile o usare -i per la modalita' interattiva.\n");
         usage();
    }
}

void effetto() {
    #define NUMEFF 4
    const char *tabeff[] = {"|", "\\", "-", "/"};
    static int ieff = 0;
    ieff = (ieff + 1) % NUMEFF;
    stampaeff(((stato == STATO_FREEZE) ? "FREEZE" : "RUN   "), tabeff[ieff], tempo);
    fflush(stdout);
}

// =============================================================================
// Implementazione delle Funzioni Aggiuntive e del Selettore
// =============================================================================
#ifndef _WIN32

int getch_unix(void) { struct termios o, n; int c; tcgetattr(0,&o); n=o; n.c_lflag&=~(ICANON|ECHO); tcsetattr(0,TCSANOW,&n); c=getchar(); tcsetattr(0,TCSANOW,&o); return c; }

static struct termios orig_termios;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode); // Assicura il ripristino anche in caso di exit()
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
#endif
int get_key() {
#ifdef _WIN32
    int ch = _getch(); if (ch == 0 || ch == 224) return _getch(); return ch;
#else
    int ch = getch_unix(); if (ch == 27) { int ch2 = getch_unix(); if (ch2 == '[') { int ch3 = getch_unix(); switch (ch3) { case 'A': return KEY_UP; case 'B': return KEY_DOWN; case 'H': return KEY_HOME; case 'F': return KEY_END; case '1': if (getch_unix() == '~') return KEY_HOME; break; case '4': if (getch_unix() == '~') return KEY_END; break; case '5': if (getch_unix() == '~') return KEY_PGUP; break; case '6': if (getch_unix() == '~') return KEY_PGDN; break; default: return ch3; } } ungetc(ch2, stdin); return KEY_ESCAPE; } return ch;
#endif
}
void free_selection(SelezioneMultipla* s) {
    if (!s) return;
    for (int i = 0; i < s->count; i++) {
        free(s->nomi[i]);
    }
    free(s->nomi);
    s->nomi = NULL;
    s->count = 0;
}

int check_keyboard_hit(void) {
#ifdef _WIN32
    return _kbhit();
#else
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds); // 0 è lo standard input
    return select(1, &fds, NULL, NULL, &tv) > 0;
#endif
}
void adjust_window(int s_idx, int* w_top, int l_count) { if (s_idx < *w_top) *w_top = s_idx; else if (s_idx >= *w_top + WINDOW_HEIGHT) *w_top = s_idx - WINDOW_HEIGHT + 1; if (*w_top > l_count - WINDOW_HEIGHT && l_count > WINDOW_HEIGHT) *w_top = l_count - WINDOW_HEIGHT; if (*w_top < 0) *w_top = 0; }
int find_pattern(FILE* f, long* off, int c, const char* p, int s, SearchDirection d) { if (strlen(p)==0) return -1; char b[4096]; int inc=(d==SEARCH_FORWARD)?1:-1; for (int i=1;i<c;i++){ int cur=(s+(i*inc)+c)%c; fseek(f,off[cur],SEEK_SET); if(fgets(b,sizeof(b),f)){if(strstr(b,p)){return cur;}}} return -1; }

SelezioneMultipla choose_from_file(const char* nome_file, int multi_select, int show_numbers) {
    SelezioneMultipla risultato = {NULL, 0};
    FILE* file = fopen(nome_file, "rb");
    if (!file) { perror("Errore apertura file selettore"); return risultato; }

    long* line_offsets=NULL; int line_count=0, capacity=0; char buffer[4096]; long current_pos=ftell(file);
    while (fgets(buffer,sizeof(buffer),file)){ if(line_count>=capacity){capacity=(capacity==0)?1024:capacity*2; long* temp=realloc(line_offsets,capacity*sizeof(long)); if(!temp){perror("Allocazione indice");free(line_offsets);fclose(file);return risultato;}line_offsets=temp;}line_offsets[line_count++]=current_pos;current_pos=ftell(file);}
    if (line_count == 0) { fclose(file); free(line_offsets); return risultato; }

    UIMode mode = MODE_NAVIGATION; int selection_idx=0, window_top_idx=0, key=0;
    char* selection_map = NULL; char input_buffer[256]={0}, last_search_pattern[256]={0}, status_message[256]={0};

    if (multi_select) {
        selection_map = calloc(line_count, sizeof(char));
        if (!selection_map) { perror("Allocazione mappa selezione"); fclose(file); free(line_offsets); return risultato; }
    }

    while (1) {
        adjust_window(selection_idx, &window_top_idx, line_count);
        system(CLEAR_SCREEN);
        printf("Comandi: Frecce, PgUp/Dn, Home/End, G, /, n/N | Spazio (Sel/Desel), q (Esci), Invio (OK)\n");
        printf("----------------------------------------------------------------------------------------\n");

        for (int i = 0; i < WINDOW_HEIGHT && (window_top_idx + i) < line_count; i++) {
            int current_line_idx = window_top_idx + i;
            fseek(file, line_offsets[current_line_idx], SEEK_SET);
            fgets(buffer, sizeof(buffer), file); buffer[strcspn(buffer, "\n\r")] = 0;
            if (show_numbers) printf("%-6d ", current_line_idx + 1);
            int is_selected = multi_select && selection_map[current_line_idx];
            int is_cursor = current_line_idx == selection_idx;
            if (is_cursor) printf("%s", ANSI_REVERSE);
            printf("%c %s", is_cursor ? '>' : (is_selected ? '*' : ' '), buffer);
            if (is_cursor) printf("%s", ANSI_RESET);
            printf("\n");
        }
        printf("----------------------------------------------------------------------------------------\n");
        
        if (mode==MODE_GOTO) printf("Vai a riga: %s", input_buffer); else if (mode==MODE_SEARCH) printf("/%s", input_buffer); else printf("Riga %d/%d %s", selection_idx+1, line_count, status_message);
        fflush(stdout); strcpy(status_message, "");

        key = get_key();

        if (mode == MODE_NAVIGATION) {
            switch (key) {
                case KEY_SPACE: if (multi_select) { selection_map[selection_idx] = !selection_map[selection_idx]; if (selection_idx < line_count - 1) selection_idx++; } break;
                case KEY_UP:   if (selection_idx > 0) selection_idx--; break;
                case KEY_DOWN: if (selection_idx < line_count - 1) selection_idx++; break;
                case KEY_PGUP: selection_idx -= WINDOW_HEIGHT; if (selection_idx < 0) selection_idx = 0; break;
                case KEY_PGDN: selection_idx += WINDOW_HEIGHT; if (selection_idx >= line_count) selection_idx = line_count - 1; break;
                case KEY_HOME: selection_idx = 0; break;
                case KEY_END:  selection_idx = line_count - 1; break;
                case 'G':      selection_idx = line_count - 1; break;
                case '/':      mode = MODE_SEARCH; input_buffer[0] = '\0'; break;
                case 'n':      if (strlen(last_search_pattern)>0){int f=find_pattern(file,line_offsets,line_count,last_search_pattern,selection_idx,SEARCH_FORWARD);if(f!=-1)selection_idx=f;else snprintf(status_message,sizeof(status_message),"| Fine ricerca");}else snprintf(status_message,sizeof(status_message),"| Nessuna ricerca precedente"); break;
                case 'N':      if (strlen(last_search_pattern)>0){int f=find_pattern(file,line_offsets,line_count,last_search_pattern,selection_idx,SEARCH_BACKWARD);if(f!=-1)selection_idx=f;else snprintf(status_message,sizeof(status_message),"| Inizio ricerca");}else snprintf(status_message,sizeof(status_message),"| Nessuna ricerca precedente"); break;
                case KEY_ENTER:goto end_loop;
                case 'q': case KEY_ESCAPE: goto end_loop;
                default: if (isdigit(key)){mode=MODE_GOTO;input_buffer[0]=key;input_buffer[1]='\0';} break;
            }
        } else if (mode == MODE_GOTO) {
            if (isdigit(key)&&strlen(input_buffer)<10){char t[2]={(char)key,'\0'};strcat(input_buffer,t);}else if((key=='g'||key=='G')&&strlen(input_buffer)>0){int l=atoi(input_buffer);input_buffer[0]='\0';if(l>0&&l<=line_count)selection_idx=l-1;else snprintf(status_message,sizeof(status_message),"| Riga non valida");mode=MODE_NAVIGATION;}else if(key==KEY_BACKSPACE&&strlen(input_buffer)>0)input_buffer[strlen(input_buffer)-1]='\0';else if(key==KEY_ESCAPE){mode=MODE_NAVIGATION;input_buffer[0]='\0';}
        } else if (mode == MODE_SEARCH) {
            if (key==KEY_ENTER){if(strlen(input_buffer)>0){strcpy(last_search_pattern,input_buffer);int f=find_pattern(file,line_offsets,line_count,last_search_pattern,selection_idx,SEARCH_FORWARD);if(f!=-1)selection_idx=f;else snprintf(status_message,sizeof(status_message),"| Pattern non trovato");}mode=MODE_NAVIGATION;input_buffer[0]='\0';}else if(key==KEY_BACKSPACE&&strlen(input_buffer)>0)input_buffer[strlen(input_buffer)-1]='\0';else if(key==KEY_ESCAPE){mode=MODE_NAVIGATION;input_buffer[0]='\0';}else if(isprint(key)&&strlen(input_buffer)<sizeof(input_buffer)-1){char t[2]={(char)key,'\0'};strcat(input_buffer,t);}
        }
    }

end_loop:
    if (key == KEY_ENTER) {
        if (multi_select) {
            for (int i=0;i<line_count;i++) if (selection_map[i]) risultato.count++;
            if(risultato.count>0){risultato.nomi=malloc(risultato.count*sizeof(char*));if(!risultato.nomi){goto cleanup;}int c=0;for(int i=0;i<line_count;i++){if(selection_map[i]){fseek(file,line_offsets[i],SEEK_SET);fgets(buffer,sizeof(buffer),file);buffer[strcspn(buffer,"\n\r")]=0;risultato.nomi[c++]=strdup(buffer);}}}
        } else {
            risultato.count=1;risultato.nomi=malloc(sizeof(char*));if(!risultato.nomi){goto cleanup;}fseek(file,line_offsets[selection_idx],SEEK_SET);fgets(buffer,sizeof(buffer),file);buffer[strcspn(buffer,"\n\r")]=0;risultato.nomi[0]=strdup(buffer);
        }
    }
cleanup:
    free(line_offsets); if (selection_map) free(selection_map); fclose(file); return risultato;
}
