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
   modulo trypag.c
   tipo 
   release 1.3
   data 3/23/95
   reserved @(#)trypag.c	1.3
*/
 /*   programma di invio messaggi a xstaa
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> // Necessario per strlen e strcspn
#include <math.h>

#include <X11/Xlib.h> // Non usate direttamente in main, ma incluse
#include <Xm/Xm.h>    // Non usate direttamente in main, ma incluse

#include "sim_param.h" // Presumibilmente definito altrove
#include "xstaz.h"     // Presumibilmente definito altrove
#include "compstaz.h"  // Presumibilmente definito altrove

// Dichiarazioni di funzioni esterne o del file stesso
// Assumo che queste siano definite altrove o siano prototipi mancanti
// se fanno parte di questo file.
extern int ef_cluster();
extern int set_nom_log_s(char *, int, char *, char *);
extern int set_ef(int, int);
extern int clr_ef(int, int);
// extern void end_nom_log_s(); // Commentata nel codice originale

// Firma di main modernizzata (opzionale ma consigliato)
int main(int argc, char *argv[]) {
    // int ix, iy; // Non usate
    // char nome[10]; // Non usata
    char app[50];
    char file[50];
    // int shr_usr_key; // Non usata

    // Inizializzazione del cluster event flags
    ef_cluster();
    file[0] = '.';
    file[1] = '\0'; // Assicurati che la stringa sia terminata correttamente
    set_nom_log_s(file, strlen(file), "S04_PATH", "EASE$LNM");
    set_ef(7, 1);
    printf("\n premi un tasto per invio dati  ");
    getchar(); // Legge un singolo carattere (e il newline se l'utente preme invio)

INIZIO:

    printf("\n X: ");
    // Sostituzione di gets(app)
    if (fgets(app, sizeof(app), stdin) == NULL) {
        // Errore o EOF (End Of File)
        fprintf(stderr, "Errore durante la lettura di X o EOF.\n");
        goto fine; // O gestisci l'errore diversamente
    }
    // Rimuovi il newline se presente alla fine della stringa letta da fgets
    app[strcspn(app, "\n")] = '\0';
    if (strlen(app) == 0) goto fine;
    set_nom_log_s(app, strlen(app), "X_STAZ", "EASE$LNM");

    printf("\n Y: ");
    // Sostituzione di gets(app)
    if (fgets(app, sizeof(app), stdin) == NULL) {
        fprintf(stderr, "Errore durante la lettura di Y o EOF.\n");
        goto fine;
    }
    app[strcspn(app, "\n")] = '\0';
    if (strlen(app) == 0) goto fine;
    set_nom_log_s(app, strlen(app), "Y_STAZ", "EASE$LNM");

    printf("\n NOME:    ");
    // Sostituzione di gets(app)
    if (fgets(app, sizeof(app), stdin) == NULL) {
        fprintf(stderr, "Errore durante la lettura di NOME o EOF.\n");
        goto fine;
    }
    app[strcspn(app, "\n")] = '\0';
    if (strlen(app) == 0) goto fine;
    set_nom_log_s(app, strlen(app), "VAR_STAZ", "EASE$LNM");

    set_ef(7, 1);
    set_ef(5, 1);

    goto INIZIO;

fine:
    clr_ef(7, 1);
    clr_ef(5, 1);
    // end_nom_log_s();

    return 0; // Buona pratica restituire 0 da main se tutto ok
}
