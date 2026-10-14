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
# Da zero vuol dire senza i .o: in git ce ne sono centinaia, e con quelli make
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
exit $ESITO
