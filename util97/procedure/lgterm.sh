#!/bin/bash
#
# lgterm - apre un terminale per LegoPST con il terminale scelto dall'utente.
#
# Chi apriva una finestra con un comando dentro chiamava xterm per nome, con
# opzioni che esistono solo in xterm (-bg, -bd, -bw, -ls): con un altro
# terminale non funzionava, e kStat/kLeeF22 cercavano /usr/bin/X11/xterm, che
# su Fedora non c'e'. lgterm usa il terminale scelto dall'utente e traduce
# titolo, geometria, colore e comando nella sintassi di quel terminale.
#
# Quale terminale, in quest'ordine:
#   1. LGTERM, se esportata: per forzarne uno in una shell;
#   2. la preferenza scelta in File -> Settings -> Terminal (di legopc o di
#      lghmi: e' lo stesso dialogo), riletta A OGNI LANCIO da
#      $LG_ENTRY/legopc_prefs.tcl: una scelta appena fatta vale subito anche
#      nelle shell e nei programmi gia' aperti, che hanno in LG_XTERM il
#      valore di quando e' stato sorgiato il profilo;
#   3. LG_XTERM;
#   4. il primo terminale noto installato.
# Un terminale non installato si salta.
#
# Uso:
#   lgterm [-t TITOLO] [-g GEOMETRIA] [-bg COLORE] [-H] [-- COMANDO [ARG...]]
#   lgterm [opzioni] -c "RIGA DI COMANDO"      (eseguita con sh -c)
#   lgterm --list                              terminali noti e quello scelto
#   lgterm --which                             solo il nome di quello scelto
#
#   -t, -title       titolo della finestra
#   -g, -geometry    COLONNExRIGHE[+X+Y], come in xterm (80x24+100+100)
#   -bg              colore di sfondo (xterm, xfce4-terminal; gli altri lo
#                    ignorano)
#   -H, --hold       a comando finito la finestra resta aperta finche' non si
#                    preme Invio (uguale per tutti i terminali)
#   senza comando    un terminale interattivo nella directory corrente
#
# Terminali noti: xterm, xfce4-terminal, tilix, konsole, gnome-terminal,
# lxterminal. Un altro si prova con la convenzione "-e comando".
#
# L'ambiente conta: i comandi di LegoPST vogliono SHR_USR_KEY, KSIM, LG_* di
# chi li lancia. xfce4-terminal e tilix per default aprono la finestra in
# un'istanza gia' in esecuzione, con l'AMBIENTE DI QUELLA: qui si lanciano
# come processo nuovo (--disable-server, --new-process).

TERMINALI="xterm xfce4-terminal tilix konsole gnome-terminal lxterminal"

uso() {
    sed -n '/^# Uso:/,/^# Terminali noti/p' "$0" | sed 's/^# \{0,1\}//'
}

PREFS="${LG_ENTRY:-$HOME/legocad}/legopc_prefs.tcl"

#  La preferenza salvata da legopc (set ::pref_xterm {nome}), "" se non c'e'.
preferenza() {
    [ -f "$PREFS" ] || return
    sed -n 's/^set ::pref_xterm *{\(.*\)}.*$/\1/p' "$PREFS" | head -1
}

installato() { [ -n "$1" ] && command -v "$1" >/dev/null 2>&1; }

#  Il terminale da usare e da dove viene la scelta: stampa "terminale origine"
#  (origine: LGTERM, preferenza, LG_XTERM, primo), niente se non ce n'e'.
scegli() {
    local p
    if installato "${LGTERM:-}"; then echo "$LGTERM LGTERM"; return; fi
    p=$(preferenza)
    if installato "$p"; then echo "$p preferenza"; return; fi
    if installato "${LG_XTERM:-}"; then echo "$LG_XTERM LG_XTERM"; return; fi
    for t in $TERMINALI; do
        if installato "$t"; then echo "$t primo"; return; fi
    done
}

scegli_terminale() { set -- $(scegli); echo "$1"; }

#  Una parola tra apici singoli, pronta per sh -c.
q() {
    local s=$1
    printf "'%s'" "${s//\'/\'\\\'\'}"
}

elenco() {
    local t o p
    set -- $(scegli)
    t=${1:-} o=${2:-}
    p=$(preferenza)
    echo "Terminali noti a lgterm (* = installato):"
    for x in $TERMINALI; do
        if installato "$x"; then echo "  * $x"; else echo "    $x"; fi
    done
    echo
    echo "LGTERM      = ${LGTERM:-(non definita)}"
    echo "preferenza  = ${p:-(nessuna)}   ($PREFS)"
    echo "LG_XTERM    = ${LG_XTERM:-(non definita)}"
    echo
    case $o in
        LGTERM)     echo "Si usa $t: lo forza LGTERM." ;;
        preferenza) echo "Si usa $t: la preferenza scelta in File -> Settings -> Terminal." ;;
        LG_XTERM)   echo "Si usa $t: da LG_XTERM (nessuna preferenza installata)." ;;
        primo)      echo "Si usa $t: il primo installato." ;;
        *)          echo "Nessun terminale installato." ;;
    esac
    echo "Per sceglierlo: legopc o lghmi, File -> Settings, campo Terminal:"
    echo "vale subito ovunque. Per forzarne uno: export LGTERM=<terminale>."
}

TITOLO="" GEOM="" SFONDO="" TIENI=0 RIGA="" HA_RIGA=0
while [ $# -gt 0 ]; do
    case $1 in
        -t|-title)     TITOLO=$2; shift 2 ;;
        -g|-geometry)  GEOM=$2; shift 2 ;;
        -bg)           SFONDO=$2; shift 2 ;;
        -H|--hold)     TIENI=1; shift ;;
        -c)            RIGA=$2; HA_RIGA=1; shift 2 ;;
        --list)        elenco; exit 0 ;;
        --which)       scegli_terminale; exit 0 ;;
        -h|--help)     uso; exit 0 ;;
        --)            shift; break ;;
        -*)            echo "lgterm: opzione sconosciuta: $1" >&2; uso >&2; exit 2 ;;
        *)             break ;;
    esac
done

#  la riga da eseguire: -c "..." oppure il comando con i suoi argomenti
if [ $HA_RIGA -eq 0 ] && [ $# -gt 0 ]; then
    RIGA=""
    for a in "$@"; do RIGA="$RIGA $(q "$a")"; done
    RIGA=${RIGA# }
fi
if [ -n "$RIGA" ] && [ $TIENI -eq 1 ]; then
    RIGA="$RIGA; echo; printf 'Premi Invio per chiudere... '; read _lgterm_invio"
fi

T=$(scegli_terminale)
if [ -z "$T" ]; then
    echo "lgterm: nessun terminale trovato (installarne uno: sudo dnf install xfce4-terminal)." >&2
    exit 1
fi

A=("$T")
case $(basename "$T") in
    xterm|uxterm)
        [ -n "$TITOLO" ] && A+=(-title "$TITOLO")
        [ -n "$GEOM" ]   && A+=(-geometry "$GEOM")
        [ -n "$SFONDO" ] && A+=(-bg "$SFONDO")
        [ -n "$RIGA" ]   && A+=(-e sh -c "$RIGA")
        ;;
    xfce4-terminal)
        A+=(--disable-server)
        [ -n "$TITOLO" ] && A+=(-T "$TITOLO")
        [ -n "$GEOM" ]   && A+=("--geometry=$GEOM")
        [ -n "$SFONDO" ] && A+=("--color-bg=$SFONDO")
        [ -n "$RIGA" ]   && A+=(-x sh -c "$RIGA")
        ;;
    tilix)
        A+=(--new-process)
        [ -n "$TITOLO" ] && A+=(-t "$TITOLO")
        [ -n "$GEOM" ]   && A+=("--geometry=$GEOM")
        #  -e vuole UNA stringa, che tilix divide come farebbe la shell
        [ -n "$RIGA" ]   && A+=(-e "sh -c $(q "$RIGA")")
        ;;
    konsole)
        [ -n "$TITOLO" ] && A+=(-p "tabtitle=$TITOLO")
        [ -n "$RIGA" ]   && A+=(-e sh -c "$RIGA")
        ;;
    gnome-terminal)
        [ -n "$TITOLO" ] && A+=("--title=$TITOLO")
        [ -n "$GEOM" ]   && A+=("--geometry=$GEOM")
        [ -n "$RIGA" ]   && A+=(-- sh -c "$RIGA")
        ;;
    lxterminal)
        [ -n "$TITOLO" ] && A+=(-t "$TITOLO")
        [ -n "$GEOM" ]   && A+=("--geometry=$GEOM")
        [ -n "$RIGA" ]   && A+=(-e "sh -c $(q "$RIGA")")
        ;;
    *)
        [ -n "$RIGA" ]   && A+=(-e sh -c "$RIGA")
        ;;
esac
exec "${A[@]}"
