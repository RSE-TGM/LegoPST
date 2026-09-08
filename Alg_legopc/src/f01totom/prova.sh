#!/usr/bin/env bash
#
# prova.sh
#
# Banco di prova di f01totom: converte le task legocad indicate e per ognuna
# riporta blocchi, posizioni recuperate, porte connesse, moduli mancanti ed
# esito. Serve a misurare i progressi della revisione: ogni fase deve
# migliorare una colonna senza peggiorarne altre.
#
# Uso:
#   bash prova.sh                  # le quattro task di riferimento
#   bash prova.sh GTS LPS          # solo quelle indicate
#   bash prova.sh -v GTS           # lascia in piedi la dir di lavoro e mostra il log
#
# Richiede LG_FILESI5, LG_LIBRARIES e LG_TOOLS (source .profile_legoroot).
# Non tocca i modelli originali: lavora su copie in una dir temporanea.

set -uo pipefail

TASKROOT="${LG_MODELS:-$HOME/legocad}"
TOOL="${LG_TOOLS:-}/f01totom"
WORKROOT="${TMPDIR:-/tmp}/f01totom_prova"
TIMEOUT=60
VERBOSE=0

# provldch: 1 blocco, smoke test.  STS: coperta al 100%, riferimento.
# GTS: due moduli mancanti.        LPS: il caso peggiore per le posizioni.
TASK_DEFAULT="provldch STS GTS LPS"

while [[ $# -gt 0 && "$1" == -* ]]; do
    case "$1" in
        -v) VERBOSE=1; shift ;;
        -t) TIMEOUT="$2"; shift 2 ;;
        *)  echo "opzione sconosciuta: $1"; exit 2 ;;
    esac
done
TASKS="${*:-$TASK_DEFAULT}"

if [[ -z "${LG_FILESI5:-}" || -z "${LG_LIBRARIES:-}" || -z "${LG_TOOLS:-}" ]]; then
    echo "ERRORE: LG_FILESI5, LG_LIBRARIES o LG_TOOLS non definiti."
    echo "Eseguire prima: source .profile_legoroot"
    exit 1
fi
if [[ ! -x "$TOOL" ]]; then
    echo "ERRORE: $TOOL non eseguibile. Compilare con: make -f makefile"
    exit 1
fi

# Conta i record di tipo blocco in macroblocks.dat: prima colonna '0',
# saltate le cinque righe di intestazione.
conta_blocchi() { awk 'NR>5 && $1=="0" && NF==7' "$1" | wc -l; }

printf '%-10s %7s %10s %7s %6s %6s %8s  %s\n' \
       TASK BLOCCHI POSIZIONI PORTE LIBERE ESITO TEMPO MANCANTI
printf '%.0s-' {1..96}; echo

for t in $TASKS; do
    src="$TASKROOT/$t"
    if [[ ! -f "$src/f01.dat" || ! -f "$src/macroblocks.dat" ]]; then
        printf '%-10s %s\n' "$t" "salto: manca f01.dat o macroblocks.dat in $src"
        continue
    fi

    work="$WORKROOT/$t"
    rm -rf "$work"; mkdir -p "$work"
    cp "$src/f01.dat" "$src/macroblocks.dat" "$work/"

    attesi=$(conta_blocchi "$src/macroblocks.dat")

    inizio=$SECONDS
    ( cd "$work" && timeout "$TIMEOUT" stdbuf -oL "$TOOL" -a </dev/null >run.log 2>&1 )
    rc=$?
    durata=$(( SECONDS - inizio ))

    case $rc in
        0)   esito="ok" ;;
        124) esito="LOOP" ;;
        *)   esito="err$rc" ;;
    esac

    posizioni=$(grep -cE ' --> x=' "$work/run.log" 2>/dev/null || true)
    mancanti=$(grep 'non presente in libreria' "$work/run.log" 2>/dev/null \
               | sed 's/.*Modulo: \([A-Z0-9]*\) .*/\1/' | sort -u | tr '\n' ' ')
    if [[ -f "$work/f01totom.tom" ]]; then
        porte=$(grep -c '^busy' "$work/f01totom.tom")
        libere=$(grep -c '^free' "$work/f01totom.tom")
    else
        porte="-"; libere="-"; [[ $esito == ok ]] && esito="NO.tom"
    fi

    printf '%-10s %7s %10s %7s %6s %6s %7ss  %s\n' \
           "$t" "$attesi" "$posizioni" "$porte" "$libere" "$esito" "$durata" "${mancanti:-—}"

    [[ $VERBOSE -eq 1 ]] && { echo "--- log: $work/run.log ---"; tail -20 "$work/run.log"; }
done

echo
echo "dir di lavoro: $WORKROOT"
