#!/usr/bin/env bash
#
# gen_missing_i5.sh
#
# Per ogni .pi4 nelle librerie di LG_LIBRARIES che non ha il corrispondente
# .i5 nella stessa directory, genera il .i5 con i32i5 e lo copia in loco.
#
# Uso:
#   bash $LG_TIX/gen_missing_i5.sh
#
# Richiede LG_LIBRARIES e LG_TOOLS definiti (source .profile_legoroot).

set -euo pipefail

# Librerie da ignorare (moduli grafici/annotazioni senza interfaccia matematica)
SKIP_LIBS="remark"

if [[ -z "${LG_LIBRARIES:-}" || -z "${LG_TOOLS:-}" ]]; then
    echo "ERRORE: LG_LIBRARIES e/o LG_TOOLS non definiti."
    echo "Eseguire prima: source .profile_legoroot"
    exit 1
fi

if [[ ! -x "$LG_TOOLS/i32i5" ]]; then
    echo "ERRORE: $LG_TOOLS/i32i5 non trovato o non eseguibile."
    exit 1
fi

tmpdir=$(mktemp -d)
trap "rm -rf $tmpdir" EXIT

ok=0
skip=0
skip_lib=0
fail=0

for pi4 in "$LG_LIBRARIES"/*/*.pi4; do
    [[ -f "$pi4" ]] || continue
    libdir=$(dirname "$pi4")
    libname=$(basename "$libdir")
    base=$(basename "${pi4%.pi4}")
    i5target="$libdir/${base}.i5"

    if [[ " $SKIP_LIBS " == *" $libname "* ]]; then
        echo "  SKIP  $base  (libreria $libname esclusa)"
        (( skip_lib++ )) || true
        continue
    fi

    if [[ -f "$i5target" ]]; then
        echo "  SKIP  $base  (${i5target##*/LG_LIBRARIES/} già presente)"
        (( skip++ )) || true
        continue
    fi

    echo -n "  GEN   $base  → $libname/ ... "
    ( cd "$tmpdir" && "$LG_TOOLS/i32i5" "$pi4" ) 2>/tmp/i32i5_err || true
    if [[ -f "$tmpdir/${base}.i5" ]]; then
        mv "$tmpdir/${base}.i5" "$i5target"
        echo "OK"
        (( ok++ )) || true
    else
        echo "ERRORE"
        cat /tmp/i32i5_err
        (( fail++ )) || true
    fi
done

echo ""
echo "=== gen_missing_i5: riepilogo ==="
echo "  Generati          : $ok"
echo "  Già presenti      : $skip"
echo "  Librerie escluse  : $skip_lib"
echo "  Errori            : $fail"
