/*
 * f22exp - esporta in testo (CSV) le variabili registrate in f22circ.dat.
 *
 * f22circ.dat e' il file circolare in cui net_prepf22 registra, durante la
 * simulazione, l'andamento delle variabili grafiche; lo leggono graphics e
 * draw2gr per i grafici. Questo programma lo scrive in colonne, per un
 * foglio di calcolo o per un altro programma. Prende il posto di LeeF22, che
 * leggeva il formato sequenziale di prima (f22.dat) e non si compilava piu'.
 *
 * Uso:  f22exp [opzioni] [FILE]
 *
 *   FILE  f22circ.dat, oppure f22circ (come lo chiama graphics), oppure la
 *         directory che lo contiene. Senza, f22circ.dat della directory
 *         corrente.
 *
 *   -l          elenca le variabili registrate (indice, nome, descrizione) e
 *               i dati del file, senza esportare i campioni
 *   -v NOMI     solo queste variabili, separate da virgole (anche ripetuta)
 *   -a          anche gli slot liberi ("libera") e senza nome
 *   -t DA:A     solo i campioni con tempo fra DA e A secondi (uno dei due
 *               puo' mancare: "100:" o ":250")
 *   -p N        un campione ogni N
 *   -d SEP      separatore delle colonne (default ","); "-d ';'" per i fogli
 *               di calcolo in italiano, "-d tab" per il tabulatore
 *   -D          una seconda riga di intestazione con le descrizioni
 *   -o FILE     scrive in FILE invece che sullo standard output
 *   -h          questo aiuto
 *
 * Il file si legge con le stesse strutture con cui lo scrive net_prepf22
 * (sim_types.h, f22_circ.h):
 *
 *   HEADER_REGISTRAZIONI | F22CIRC_HD | num_var_graf x F22CIRC_VAR |
 *   num_campioni record da (tempo, num_var_graf float)
 *
 * Il buffer e' circolare, con indici da 1: i campioni vanno da p_iniz a
 * p_fine, ripartendo da 1 dopo num_campioni. p_fine = 0 vuol dire file
 * vuoto. Anche lo slot num_campioni contiene un campione: f22_leggi_tutti_camp
 * di libsim lo salta, qui no.
 *
 * Esce con 0 se ha esportato (anche zero campioni), 1 per un errore.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>

#include "sim_param.h"
#include "sim_types.h"
#include "f22_circ.h"

static void aiuto(FILE *f)
{
    fputs(
"uso: f22exp [opzioni] [FILE]\n"
"  Esporta in CSV le variabili registrate in f22circ.dat.\n"
"  FILE: f22circ.dat, f22circ o la sua directory (default: f22circ.dat qui)\n"
"  -l        elenca le variabili e i dati del file, senza campioni\n"
"  -v NOMI   solo queste variabili (separate da virgole, anche ripetuta)\n"
"  -a        anche gli slot liberi e senza nome\n"
"  -t DA:A   solo i campioni con tempo fra DA e A secondi\n"
"  -p N      un campione ogni N\n"
"  -d SEP    separatore (default \",\"; \"tab\" per il tabulatore)\n"
"  -D        seconda riga di intestazione con le descrizioni\n"
"  -o FILE   scrive in FILE invece che sullo standard output\n", f);
}

/* Nome o descrizione senza spazi in coda, terminato anche se il campo e'
   pieno fino all'ultimo byte. */
static void copia_campo(char *dst, const char *src, size_t n)
{
    size_t l;
    memcpy(dst, src, n);
    dst[n] = '\0';
    l = strlen(dst);
    while (l > 0 && (dst[l - 1] == ' ' || dst[l - 1] == '\t'))
        dst[--l] = '\0';
}

/* Il percorso del file: accetta il nome con o senza .dat, o la directory. */
static char *trova_file(const char *arg)
{
    struct stat st;
    char *p;
    size_t n;

    if (arg == NULL)
        arg = ".";
    n = strlen(arg);
    p = malloc(n + 20);
    if (p == NULL)
        return NULL;
    if (stat(arg, &st) == 0 && S_ISDIR(st.st_mode)) {
        sprintf(p, "%s/f22circ.dat", arg);
    } else if (n > 4 && strcmp(arg + n - 4, ".dat") == 0) {
        strcpy(p, arg);
    } else if (stat(arg, &st) == 0) {
        strcpy(p, arg);
    } else {
        sprintf(p, "%s.dat", arg);
    }
    return p;
}

/* Il campo a cui cercare un nome nella lista -v (confronto senza maiuscole). */
static int in_lista(const char *nome, char **lista, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (strcasecmp(nome, lista[i]) == 0)
            return i;
    return -1;
}

int main(int argc, char **argv)
{
    const char *sep = ",";
    const char *uscita = NULL;
    int solo_elenco = 0, tutti = 0, descrizioni = 0, passo = 1;
    int c_da = 0, c_a = 0;
    double t_da = 0.0, t_a = 0.0;
    char **richieste = NULL;
    int n_richieste = 0;
    char *file;
    FILE *f, *out;
    struct stat st;
    F22CIRC_HD hd;
    F22CIRC_VAR *var;
    long off_var, off_dati, size_rec, attesa;
    int n_camp, i, k, primo, scritti;
    int *col, n_col;
    float *rec;
    char nome[MAX_LUN_NOME_VAR + 1], descr[sizeof(var->descvar) + 1];

    for (i = 1; i < argc && argv[i][0] == '-' && argv[i][1] != '\0'; i++) {
        const char *o = argv[i];
        const char *val = NULL;
        if (strchr("vtpdo", o[1]) != NULL && o[2] == '\0') {
            if (i + 1 >= argc) {
                fprintf(stderr, "f22exp: %s vuole un argomento (vedi -h)\n", o);
                return 1;
            }
            val = argv[++i];
        }
        switch (o[1]) {
        case 'h': aiuto(stdout); return 0;
        case 'l': solo_elenco = 1; break;
        case 'a': tutti = 1; break;
        case 'D': descrizioni = 1; break;
        case 'o': uscita = val; break;
        case 'd': sep = strcmp(val, "tab") == 0 ? "\t" : val; break;
        case 'p':
            passo = atoi(val);
            if (passo < 1) {
                fprintf(stderr, "f22exp: -p vuole un numero >= 1\n");
                return 1;
            }
            break;
        case 't': {
            const char *due = strchr(val, ':');
            char *fine;
            if (due == NULL) {
                fprintf(stderr, "f22exp: -t vuole DA:A (es. 100:250, 100:, :250)\n");
                return 1;
            }
            if (due != val) {
                t_da = strtod(val, &fine);
                if (fine != due) { fprintf(stderr, "f22exp: -t: '%s' non e' un tempo\n", val); return 1; }
                c_da = 1;
            }
            if (due[1] != '\0') {
                t_a = strtod(due + 1, &fine);
                if (*fine != '\0') { fprintf(stderr, "f22exp: -t: '%s' non e' un tempo\n", due + 1); return 1; }
                c_a = 1;
            }
            break;
        }
        case 'v': {
            char *copia = strdup(val), *tok, *salva = NULL;
            for (tok = strtok_r(copia, ",", &salva); tok; tok = strtok_r(NULL, ",", &salva)) {
                richieste = realloc(richieste, (n_richieste + 1) * sizeof(char *));
                richieste[n_richieste++] = tok;
            }
            break;
        }
        default:
            fprintf(stderr, "f22exp: opzione sconosciuta: %s (vedi -h)\n", o);
            return 1;
        }
    }
    if (i < argc - 1) {
        fprintf(stderr, "f22exp: un file alla volta (vedi -h)\n");
        return 1;
    }

    file = trova_file(i < argc ? argv[i] : NULL);
    f = file ? fopen(file, "rb") : NULL;
    if (f == NULL) {
        fprintf(stderr, "f22exp: non riesco ad aprire %s\n", file ? file : "f22circ.dat");
        return 1;
    }

    /* intestazione */
    if (fseek(f, (long) sizeof(HEADER_REGISTRAZIONI), SEEK_SET) != 0 ||
        fread(&hd, sizeof(hd), 1, f) != 1) {
        fprintf(stderr, "f22exp: %s e' troppo corto per essere un f22circ.dat\n", file);
        return 1;
    }
    if (hd.num_var_graf <= 0 || hd.num_var_graf > 100000 ||
        hd.num_campioni <= 0 || hd.num_campioni > 100000000 ||
        hd.p_fine < 0 || hd.p_fine > hd.num_campioni ||
        hd.p_iniz < 0 || hd.p_iniz > hd.num_campioni) {
        fprintf(stderr, "f22exp: %s: intestazione non valida (variabili %d, campioni %d, "
                "inizio %d, fine %d): non e' un f22circ.dat?\n",
                file, hd.num_var_graf, hd.num_campioni, hd.p_iniz, hd.p_fine);
        return 1;
    }
    off_var  = (long) sizeof(HEADER_REGISTRAZIONI) + (long) sizeof(F22CIRC_HD);
    off_dati = off_var + (long) sizeof(F22CIRC_VAR) * hd.num_var_graf;
    size_rec = (long) sizeof(F22CIRC_T) + (long) sizeof(float) * hd.num_var_graf;

    var = malloc(sizeof(F22CIRC_VAR) * hd.num_var_graf);
    rec = malloc(size_rec);
    col = malloc(sizeof(int) * hd.num_var_graf);
    if (var == NULL || rec == NULL || col == NULL) {
        fprintf(stderr, "f22exp: memoria insufficiente\n");
        return 1;
    }
    if (fread(var, sizeof(F22CIRC_VAR), hd.num_var_graf, f) != (size_t) hd.num_var_graf) {
        fprintf(stderr, "f22exp: %s: elenco delle variabili troncato\n", file);
        return 1;
    }

    n_camp = hd.p_fine == 0 ? 0 : (hd.p_iniz <= 1 ? hd.p_fine : hd.num_campioni);
    primo = hd.p_iniz < 1 ? 1 : hd.p_iniz;
    attesa = off_dati + size_rec * hd.num_campioni;
    if (stat(file, &st) == 0 && st.st_size < off_dati + size_rec * (long) n_camp) {
        fprintf(stderr, "f22exp: %s: %ld byte, ne servirebbero almeno %ld: file troncato\n",
                file, (long) st.st_size, off_dati + size_rec * (long) n_camp);
        return 1;
    }

    if (solo_elenco) {
        printf("file:       %s\n", file);
        printf("creato:     %02d/%02d/%04d %02d:%02d:%02d\n",
               hd.giorno, hd.mese, hd.anno, hd.ore, hd.minuti, hd.secondi);
        printf("campioni:   %d registrati su %d (inizio %d, fine %d)\n",
               n_camp, hd.num_campioni, hd.p_iniz, hd.p_fine);
        printf("variabili:  %d slot\n", hd.num_var_graf);
        /* il file cresce mentre si registra: piu' corto va bene, piu'
           lungo no */
        if (stat(file, &st) == 0 && st.st_size > attesa)
            printf("attenzione: il file ha %ld byte, il formato ne prevede al massimo %ld\n",
                   (long) st.st_size, attesa);
        for (k = 0; k < hd.num_var_graf; k++) {
            copia_campo(nome, var[k].nomevar, sizeof(var[k].nomevar));
            copia_campo(descr, var[k].descvar, sizeof(var[k].descvar));
            if (!tutti && (nome[0] == '\0' || strcmp(nome, "libera") == 0))
                continue;
            printf("%5d  %-8s  %s\n", k + 1, nome, descr);
        }
        return 0;
    }

    /* colonne da esportare */
    n_col = 0;
    if (n_richieste > 0) {
        int *trovata = calloc(n_richieste, sizeof(int)), mancanti = 0;
        for (k = 0; k < hd.num_var_graf; k++) {
            int r;
            copia_campo(nome, var[k].nomevar, sizeof(var[k].nomevar));
            r = in_lista(nome, richieste, n_richieste);
            if (r >= 0 && !trovata[r]) {
                trovata[r] = 1;
                col[n_col++] = k;
            }
        }
        for (k = 0; k < n_richieste; k++)
            if (!trovata[k]) {
                fprintf(stderr, "f22exp: la variabile %s non e' registrata in %s (vedi -l)\n",
                        richieste[k], file);
                mancanti++;
            }
        if (mancanti)
            return 1;
    } else {
        for (k = 0; k < hd.num_var_graf; k++) {
            copia_campo(nome, var[k].nomevar, sizeof(var[k].nomevar));
            if (tutti || (nome[0] != '\0' && strcmp(nome, "libera") != 0))
                col[n_col++] = k;
        }
    }

    out = stdout;
    if (uscita != NULL && (out = fopen(uscita, "w")) == NULL) {
        fprintf(stderr, "f22exp: non riesco a scrivere %s\n", uscita);
        return 1;
    }

    fprintf(out, "tempo");
    for (k = 0; k < n_col; k++) {
        copia_campo(nome, var[col[k]].nomevar, sizeof(var[col[k]].nomevar));
        fprintf(out, "%s%s", sep, nome);
    }
    fputc('\n', out);
    if (descrizioni) {
        fprintf(out, "s");
        for (k = 0; k < n_col; k++) {
            copia_campo(descr, var[col[k]].descvar, sizeof(var[col[k]].descvar));
            /* il separatore dentro una descrizione romperebbe le colonne */
            if (strlen(sep) == 1) {
                char *s;
                for (s = descr; *s; s++)
                    if (*s == sep[0]) *s = ' ';
            }
            fprintf(out, "%s%s", sep, descr);
        }
        fputc('\n', out);
    }

    /* campioni, dal piu' vecchio */
    scritti = 0;
    for (i = 0; i < n_camp; i++) {
        int slot = (primo - 1 + i) % hd.num_campioni;   /* da 0 */
        float tempo;
        if (fseek(f, off_dati + size_rec * slot, SEEK_SET) != 0 ||
            fread(rec, size_rec, 1, f) != 1) {
            fprintf(stderr, "f22exp: %s: errore leggendo il campione %d\n", file, slot + 1);
            return 1;
        }
        tempo = rec[0];
        if (c_da && tempo < t_da) continue;
        if (c_a && tempo > t_a) continue;
        if (i % passo != 0) continue;
        fprintf(out, "%.7g", tempo);
        for (k = 0; k < n_col; k++)
            fprintf(out, "%s%.7g", sep, rec[1 + col[k]]);
        fputc('\n', out);
        scritti++;
    }
    if (out != stdout && fclose(out) != 0) {
        fprintf(stderr, "f22exp: errore scrivendo %s\n", uscita);
        return 1;
    }
    fprintf(stderr, "f22exp: %d campioni, %d variabili%s%s\n", scritti, n_col,
            uscita ? " -> " : "", uscita ? uscita : "");
    fclose(f);
    return 0;
}
