#!/bin/bash
#
# lgicone: collegamenti di LegoPST nel menu delle applicazioni, con le icone
# di $LEGOROOT/icone. Sotto WSL li raccoglie WSLg, che li mette nel menu Start
# di Windows (cartella della distribuzione, per esempio "Fedora 44").
#
#   lgicone                     installa i collegamenti per l'utente
#                               (~/.local/share/applications)
#   sudo $LEGOROOT/util97/bin/lgicone --sistema
#                               li installa per tutti (/usr/share/applications):
#                               serve se WSLg non legge la cartella dell'utente.
#                               Con sudo il PATH di LegoPST non c'e': percorso intero.
#   lgicone --rimuovi [--sistema]   li toglie
#   lgicone --avvia VOCE        quello che lancia un collegamento: carica il
#                               profilo LegoPST e avvia il programma
#
# Le voci:
#   lghmi   LegoPST             lghmi, nella dir del simulatore corrente
#   lgpc    LegoPST lgpc        lgpc (legopc va da se' in $LG_MODELS)
#   xstaz   LegoPST Faceplate   lghmi -staz, il lanciatore dei faceplate xstaz
#
# config e net_startup non hanno un collegamento: config si apre su una task
# di regolazione scelta in lghmi, e net_startup lancia killsim, che ferma ogni
# simulazione dell'utente; lghmi chiede conferma prima, un collegamento no.
#
# Un collegamento parte senza la shell dell'utente, quindi senza il profilo
# sorgiato da ~/.bashrc: --avvia lo sorgia da se'. L'uscita del programma va
# in /tmp/lgicone_<voce>_<utente>.log, perche' WSLg non la mostra da nessuna
# parte.

# $LEGOROOT si ricava dalla posizione dello script ($LEGOROOT/util97/bin):
# con sudo e da un collegamento la variabile non c'e'.
LEGOROOT_QUI=$(realpath "$(dirname "$(realpath "$0")")/../..")
LEGOROOT=${LEGOROOT:-$LEGOROOT_QUI}

VOCI="lghmi lgpc xstaz"

nome_voce() {
  case "$1" in
    lghmi) echo "LegoPST" ;;
    lgpc)  echo "LegoPST lgpc" ;;
    xstaz) echo "LegoPST Faceplate" ;;
  esac
}

# La classe X della finestra principale (WM_CLASS): Tk la ricava dal nome dello
# script, "Lghmi.tcl" per lghmi.tcl. Con StartupWMClass i desktop Linux
# (GNOME, KDE) associano la finestra aperta al suo collegamento: icona giusta
# nel dock e "aggiungi ai preferiti". Faceplate e' lghmi -staz, stessa classe
# di lghmi: le sue finestre finiscono sotto una delle due voci.
classe_voce() {
  case "$1" in
    lghmi|xstaz) echo "Lghmi.tcl" ;;
    lgpc)        echo "Legopc.tix" ;;
  esac
}

commento_voce() {
  case "$1" in
    lghmi) echo "Simulazione, HMI e modifiche del simulatore corrente" ;;
    lgpc)  echo "Modifica dei modelli di processo" ;;
    xstaz) echo "Faceplate di comando (xstaz)" ;;
  esac
}

avvia() {
  local voce="$1"
  case "$voce" in
    lghmi|lgpc|xstaz) ;;
    *) echo "lgicone: voce sconosciuta '$voce' (voci: $VOCI)" >&2; exit 2 ;;
  esac
  local log="/tmp/lgicone_${voce}_${USER:-$(id -un)}.log"
  exec >"$log" 2>&1
  echo "lgicone --avvia $voce, $(date)"
  # Il profilo prende il primo argomento come LEGOROOT: qui, dentro una
  # funzione, senza argomento vedrebbe il $1 della funzione (la voce). Glielo
  # si passa esplicito.
  # shellcheck disable=SC1091
  source "$LEGOROOT/.profile_legoroot" "$LEGOROOT"
  case "$voce" in
    lghmi) cd "${KSIM:-$HOME}" 2>/dev/null || cd "$HOME"; exec lghmi ;;
    lgpc)  cd "$HOME"; exec lgpc ;;
    xstaz) cd "${KSIM:-$HOME}" 2>/dev/null || cd "$HOME"; exec lghmi -staz ;;
  esac
}

# Nel campo Exec di un .desktop, dentro le virgolette, vanno protetti
# " ` $ e \ (specifica Desktop Entry).
exec_quote() {
  printf '"%s"' "$(printf '%s' "$1" | sed 's/[\\"`$]/\\&/g')"
}

installa() {
  local dir="$1"
  local lgicone="$LEGOROOT/util97/bin/lgicone"
  mkdir -p "$dir" || exit 1
  for voce in $VOCI; do
    local icona="$LEGOROOT/icone/$voce/${voce}_256.png"
    if [ ! -f "$icona" ]; then
      echo "lgicone: manca l'icona $icona" >&2
      exit 1
    fi
    cat > "$dir/legopst-$voce.desktop" <<EOF
[Desktop Entry]
Type=Application
Name=$(nome_voce "$voce")
Comment=$(commento_voce "$voce")
Exec=$(exec_quote "$lgicone") --avvia $voce
Icon=$icona
Terminal=false
StartupWMClass=$(classe_voce "$voce")
Categories=Science;Engineering;
EOF
    echo "installato: $dir/legopst-$voce.desktop"
  done
  if grep -qi microsoft /proc/version 2>/dev/null; then
    echo "Se le voci non compaiono nel menu Start di Windows:"
    echo "  - prova 'sudo $lgicone --sistema';"
    echo "  - da Windows, 'wsl --shutdown' e riapri la distribuzione."
  else
    echo "Le voci compaiono nel menu delle applicazioni (con KDE puo' servire"
    echo "rifare l'accesso, o lanciare kbuildsycoca6)."
  fi
}

rimuovi() {
  local dir="$1"
  for voce in $VOCI; do
    rm -f "$dir/legopst-$voce.desktop" && echo "rimosso: $dir/legopst-$voce.desktop"
  done
}

DIR_UTENTE="${XDG_DATA_HOME:-$HOME/.local/share}/applications"
DIR_SISTEMA="/usr/share/applications"

case "$1" in
  "")                installa "$DIR_UTENTE" ;;
  --sistema)         installa "$DIR_SISTEMA" ;;
  --rimuovi)
    if [ "$2" = "--sistema" ]; then rimuovi "$DIR_SISTEMA"; else rimuovi "$DIR_UTENTE"; fi ;;
  --avvia)           avvia "$2" ;;
  *)
    echo "Uso: lgicone [--sistema] | --rimuovi [--sistema] | --avvia VOCE" >&2
    exit 2 ;;
esac
