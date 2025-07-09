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
    fprintf(stderr, "    -t tempoloop       tempo di scansione (default %.1f s).\n", TIMELOOP);
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

int main(int argc, char **argv) {
    char str_app[256];
    int iumis, defumis;
    float valore_conv;

    SetUp(argc, argv);
    getcwd(pathloc, 256);
    chdefaults();
    init_umis();
    chdir(pathloc);

    viewshr(INIZIALIZZA, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);

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
            char** descrizioni_valide = malloc(scelta.count * sizeof(char*)); // Nuovo array per le descrizioni
            int valid_vars_count = 0;
            
            char nome_temp[MAX_LUN_NOME_VAR + 1];

            for (int i = 0; i < scelta.count; i++) {
                // Estrae solo il nome dalla riga selezionata (es. "NOME   DESCRIZIONE")
                sscanf(scelta.nomi[i], "%s", nome_temp);

                if (viewshr(GETIND, nome_temp, &indirizzi[valid_vars_count], NULL, NULL, NULL, NULL, 0)) {
                    nomi_validi[valid_vars_count] = strdup(nome_temp);
                    
                    // Trova la descrizione corrispondente nell'array globale 'variabili'
                    for (int j = 0; j < tot_variabili; j++) {
                        if (strncmp(nome_temp, variabili[j].nome, strlen(nome_temp)) == 0) {
                             descrizioni_valide[valid_vars_count] = strdup(variabili[j].descr);
                             break;
                        }
                    }
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
            
            // 3. Loop di visualizzazione
            // *** ATTIVAZIONE MODALITA' RAW PRIMA DEL LOOP DI VISUALIZZAZIONE ***
            #ifndef _WIN32
            enable_raw_mode();
            #endif

            while (1) {
                viewshr(CHECK, NULL, NULL, NULL, &stato, &tempo, NULL, 0);
                if (stato == STATO_STOP || stato == STATO_ERRORE) {
                    fprintf(stderr, "\n%s termina. Simulatore in STOP/ERRORE.\n", progname);
                    return_to_select = 0; // Uscita definitiva
                    break;
                }
                if (check_keyboard_hit()) {
                    int key_pressed = getchar();
                    if (key_pressed == 'i') {
                        return_to_select = 1; // Ritorna alla selezione
                        break;
                    } else if (key_pressed == 'q') {
                        return_to_select = 0; // Esce dal programma
                        break;
                    }
                }                
                // // Controlla se l'utente vuole tornare alla selezione
                // if (check_keyboard_hit()) {
                //     // Usiamo getchar() perché il terminale è già in modalità raw
                //     if (getchar() == 'i') { 
                //         return_to_select = 1;
                //         break;
                //     }
                // }

                system(CLEAR_SCREEN);
                printf("Visualizzazione Multipla (premi 'i' per riselezionare, 'q' per uscire) - Tempo Sim: %.2f\n", tempo);
                // *** MODIFICA 3: Header della tabella aggiornato ***
                printf("----------------------------------------------------------------------------------------\n");
                printf("%-12s | %-40s | %-15s | %s\n", "Variabile", "Descrizione", "Valore", "Unita'");
                printf("----------------------------------------------------------------------------------------\n");

                for (int i = 0; i < valid_vars_count; i++) {
                    viewshr(GETVAR, nomi_validi[i], &indirizzi[i], &valore, &stato, &tempo, &num_var, 0);
                    // Logica per le unità di misura                    
                    iumis = cerca_umis(nomi_validi[i], 1);
                    defumis = uni_mis[iumis].sel;
                    valore_conv = uni_mis[iumis].A[defumis] * valore + uni_mis[iumis].B[defumis];

                    // *** MODIFICA 4: Stampa della tabella con la descrizione ***
                    printf("%-12s | %-40.40s | %-15.4f | %s\n", 
                           nomi_validi[i], 
                           descrizioni_valide[i], 
                           valore_conv, 
                           uni_mis[iumis].codm[defumis]);
                }
                
                fflush(stdout);
                sospendi(timemilli);
            }
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
        viewshr(PUTVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
        viewshr(GETVAR, nomevar, &indir, &valore, &stato, &tempo, &num_var, forzval);
        printf("-->\b\b\b");
        stampa(nomevar, valore, tempo);
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
    int i;
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
