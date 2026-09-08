#!/usr/bin/env bash
#
# verifica_tom.sh
#
# Controlla che un .tom sia leggibile da topRead (../tix/fileio.tcl) senza
# doverlo aprire in lgpc:
#   - struttura: intestazione, dimensioni, cinque righe per blocco fino a ****,
#     poi le sezioni delle porte chiuse da ++++ e il **** finale;
#   - per ogni blocco esistono i file che topRead cerca davvero, cioe'
#     $LG_LIBRARIES/<libreria>/<classe>.tcl  (elementScript) e
#     $LG_LIBRARIES/<libreria>/<classe>n.gif (checkImage).
#
# Uso:  bash verifica_tom.sh <file.tom> [altri.tom ...]
# Richiede LG_LIBRARIES (source .profile_legoroot).

set -uo pipefail

if [[ -z "${LG_LIBRARIES:-}" ]]; then
    echo "ERRORE: LG_LIBRARIES non definita. Eseguire: source .profile_legoroot"
    exit 1
fi
[[ $# -ge 1 ]] || { echo "Uso: bash verifica_tom.sh <file.tom> ..."; exit 2; }

esito_globale=0

for tom in "$@"; do
    echo "=== $tom"
    if [[ ! -f "$tom" ]]; then echo "   file assente"; esito_globale=1; continue; fi

    # struttura + estrazione delle coppie "classe libreria"
    coppie=$(awk '
        NR==1 { if ($0 !~ /^#/) { print "STRUTTURA: la riga 1 non e\x27 un commento" > "/dev/stderr"; err=1 } ; next }
        NR==2 { if (NF!=2 || $1+0<=0 || $2+0<=0) { print "STRUTTURA: riga 2 non sono due dimensioni: " $0 > "/dev/stderr"; err=1 }
                sez=1; k=0; next }
        sez==1 {
            if ($0=="****") { sez=2; if (k%5!=0) { print "STRUTTURA: prima sezione con " k " righe, non multiplo di 5" > "/dev/stderr"; err=1 } ; k=0; next }
            r = k%5
            if (r==0) cls=$0
            if (r==3 && $0 !~ /^-?[0-9]+\.[0-9]+ -?[0-9]+\.[0-9]+$/) { print "STRUTTURA: coordinate malformate per " cls ": " $0 > "/dev/stderr"; err=1 }
            if (r==4) print cls, $0
            k++
            next
        }
        sez==2 {
            if ($0=="****") { sez=3; next }
            next
        }
        END {
            if (sez!=3) { print "STRUTTURA: manca il **** finale" > "/dev/stderr"; err=1 }
            if (err) exit 3
        }' "$tom" 2>/tmp/verifica_err.$$)
    rc=$?
    if [[ -s /tmp/verifica_err.$$ ]]; then cat /tmp/verifica_err.$$ | sed 's/^/   /'; esito_globale=1; fi
    rm -f /tmp/verifica_err.$$

    nblo=0; mancanti=0
    while read -r cls lib; do
        [[ -z "$cls" ]] && continue
        nblo=$((nblo+1))
        d="$LG_LIBRARIES/$(basename "$lib")"
        [[ -f "$d/$cls.tcl"  ]] || { echo "   manca $d/$cls.tcl";  mancanti=$((mancanti+1)); }
        [[ -f "$d/${cls}n.gif" ]] || { echo "   manca $d/${cls}n.gif"; mancanti=$((mancanti+1)); }
    done <<< "$coppie"

    if [[ $rc -eq 0 && $mancanti -eq 0 ]]; then
        echo "   OK - $nblo blocchi, struttura valida, tutti i file di libreria presenti"
    else
        echo "   PROBLEMI - $nblo blocchi, $mancanti file di libreria mancanti"
        esito_globale=1
    fi
done

exit $esito_globale
