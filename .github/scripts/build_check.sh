#!/bin/bash
#
# build_check.sh - compilazione DA ZERO e controllo dei warning.
#
# Lo usa il workflow .github/workflows/build.yml, dentro l'immagine
# aguagliardi/legopst:2.0 (che ha gcc, gfortran e gli header di sviluppo).
# Si puo' lanciare uguale a mano, ma SU UNA COPIA: cancella i .o.
#
#   docker run --rm -v <copia>:/src -w /src aguagliardi/legopst:2.0 \
#       bash .github/scripts/build_check.sh
#
# Cosa controlla
#   1. che il build dalla radice (make -k -f Makefile.mk) finisca senza errori;
#   2. che i warning non siano PIU' di quelli registrati in
#      .github/build_warnings_baseline. Il numero puo' solo scendere: quando
#      scende, si aggiorna il file con il nuovo valore (lo script lo dice).
#
# Da zero vuol dire senza i .o: in una copia gia' compilata ci sono tutti, e con quelli make
# non ricompilerebbe niente (e non direbbe niente).
#
# Uscita: 0 tutto bene, 1 errori di compilazione, 2 warning oltre la soglia.

set -o pipefail
cd "$(dirname "$0")/../.." || exit 1
RADICE="$(pwd)"
LOG="${BUILD_LOG:-$RADICE/build.log}"
BASE_FILE="$RADICE/.github/build_warnings_baseline"

echo "Radice:  $RADICE"
echo "Log:     $LOG"

# Tutti i file con la STESSA data. Il checkout di GitHub scrive i file in un
# ordine qualunque, con la data del momento: un prodotto tracciato in git
# (regopatch.jar, un eseguibile costruito direttamente da un .c) puo' cosi'
# risultare piu' vecchio del suo sorgente, e make prova a rifarlo. Il build
# cambiava da una run all'altra: una volta si fermava su javac (che
# nell'immagine non c'e'), un'altra contava 19 warning in piu'. Con le date
# uguali si compila quello che dipende dai .o - cancellati qui sotto, quindi
# tutto il codice C e Fortran delle librerie e dei programmi - e il risultato
# e' sempre lo stesso, come sulle macchine di sviluppo.
ORA=$(date +%s)
find . -path ./.git -prune -o -print0 | xargs -0 touch -h -d "@$ORA"
find . -path ./.git -prune -o -name '*.o' -type f -print0 | xargs -0 rm -f
# shellcheck disable=SC1091
source ./.profile_legoroot >/dev/null 2>&1

INIZIO=$(date +%s)
make -k -f Makefile.mk > "$LOG" 2>&1
RC=$?
echo "make: uscita $RC in $(( $(date +%s) - INIZIO )) s"

ERRORI=$(grep -ac ' error:' "$LOG")
WARNING=$(grep -ac 'warning:' "$LOG")
BASE=$(tr -dc '0-9' < "$BASE_FILE" 2>/dev/null)
BASE=${BASE:-0}

echo ""
echo "errori:   $ERRORI"
echo "warning:  $WARNING   (soglia: $BASE)"
echo ""
echo "--- warning per tipo ---"
grep -aoE '\[-W[a-z0-9=-]+\]' "$LOG" | sort | uniq -c | sort -rn | head -20

ESITO=0
if [ "$RC" -ne 0 ] || [ "$ERRORI" -ne 0 ]; then
    echo ""
    echo "ERRORE: la compilazione non e' andata a buon fine. Primi errori:"
    grep -a -B2 ' error:\|\*\*\* ' "$LOG" | head -40
    # Su GitHub i primi errori diventano annotazioni della run: si vedono in
    # cima alla pagina, e si leggono anche dall'API pubblica, senza dover
    # scaricare il log (che richiede di essere autenticati).
    if [ -n "${GITHUB_ACTIONS:-}" ]; then
        grep -a ' error:\|\*\*\* ' "$LOG" | head -10 | while IFS= read -r riga; do
            echo "::error::${riga//%/%25}"
        done
    fi
    ESITO=1
elif [ "$WARNING" -gt "$BASE" ]; then
    echo ""
    echo "ERRORE: i warning sono $WARNING, la soglia e' $BASE: ne sono stati"
    echo "        aggiunti. Correggerli, non alzare la soglia."
    ESITO=2
elif [ "$WARNING" -lt "$BASE" ]; then
    echo ""
    echo "I warning sono scesi da $BASE a $WARNING: aggiornare la soglia con"
    echo "    echo $WARNING > .github/build_warnings_baseline"
fi
# Il riepilogo nella pagina della run (GitHub Actions)
if [ -n "${GITHUB_STEP_SUMMARY:-}" ]; then
    {
        echo "### Build da zero"
        echo ""
        echo "| make | errori | warning | soglia |"
        echo "|---|---|---|---|"
        echo "| uscita $RC | $ERRORI | $WARNING | $BASE |"
        echo ""
        echo '```'
        grep -aoE '\[-W[a-z0-9=-]+\]' "$LOG" | sort | uniq -c | sort -rn | head -12
        if [ "$ESITO" -eq 1 ]; then
            echo ""
            grep -a -B2 ' error:\|\*\*\* ' "$LOG" | head -40
        fi
        echo '```'
    } >> "$GITHUB_STEP_SUMMARY"
fi
exit $ESITO
