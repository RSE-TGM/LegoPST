#!/bin/bash
#
# lgversion: la versione di LegoPST installata in $LEGOROOT.
#
#   lgversion            la mostra
#   lgversion --define   stampa solo le tre righe "#define" nella forma di
#                        version.h: e' quello che leggono legopc e lghmi
#
# Da dove viene la versione:
#  - se $LEGOROOT e' un checkout git, da git, calcolata adesso. Eseguibili e
#    librerie stanno in git (docs/BUILD.md, "Compiled files in git"): dopo un
#    "git pull" quelli in uso sono quelli del commit scaricato, senza
#    compilare niente. version.h invece e' escluso da git e si riscrive solo
#    con make: dopo un pull restava quello dell'ultima compilazione fatta in
#    quella copia, e due copie allineate mostravano versioni diverse.
#  - dove git non c'e' (immagine Docker, bundle FMU, tarball), da version.h,
#    scritto da chi ha costruito quella copia.
#
# Versione e numero sono calcolati come nella regola version.h di Makefile.mk:
# se si cambia il "git describe" la', va cambiato anche qui. La data invece e'
# quella dell'ultimo commit: la data del make non direbbe niente di una copia
# aggiornata con git pull.

DEFINE=0
case "$1" in
  --define) DEFINE=1 ;;
  "") ;;
  *) echo "Uso: lgversion [--define]" >&2; exit 2 ;;
esac

# Controlla se la variabile d'ambiente LEGOROOT è impostata
if [ -z "$LEGOROOT" ]; then
  echo "Errore: La variabile d'ambiente LEGOROOT non è impostata." >&2
  echo "Impostala alla directory radice del progetto prima di eseguire questo script." >&2
  exit 1
fi

# Nome del file da leggere, costruito a partire da LEGOROOT
VERSION_FILE="$LEGOROOT/version.h"

# --- Versione da git ---
#  Solo se il .git e' proprio in $LEGOROOT: una copia senza .git dentro un
#  altro repository non deve prendere la versione di quello.
DEFINES=""
if [ -e "$LEGOROOT/.git" ] && command -v git >/dev/null 2>&1; then
  GV=$(git -C "$LEGOROOT" describe --tags --match 'v[0-9]*' --always --long --dirty 2>/dev/null)
  GN=$(git -C "$LEGOROOT" rev-list --count HEAD 2>/dev/null)
  GD=$(git -C "$LEGOROOT" log -1 --format=%cd --date=format:%Y%m%d 2>/dev/null)
  if [ -n "$GV" ] && [ -n "$GN" ] && [ -n "$GD" ]; then
    DEFINES="#define GIT_VERSION_STRING \"$GV\"
#define BUILD_NUMBER $GN
#define BUILD_DATE_STRING \"$GD\""
    FONTE="git (sorgenti in \$LEGOROOT)"
  fi
fi

# --- Ripiego: version.h ---
if [ -z "$DEFINES" ]; then
  # Con --define niente make: lghmi lo chiama anche dove il repository non e'
  # scrivibile. Esce con 1 e chi chiama decide.
  if [ "$DEFINE" = 1 ] && [ ! -f "$VERSION_FILE" ]; then
    exit 1
  fi
  # Se il file non c'e', si prova a generarlo con make
  if [ ! -f "$VERSION_FILE" ]; then
    echo "Info: Il file di versione '$VERSION_FILE' non è stato trovato." >&2
    echo "Info: Tento di generarlo ora eseguendo 'make'..." >&2
    if ! make -s -C "$LEGOROOT" -f Makefile.mk version.h >&2; then
      echo "Errore: Impossibile generare '$VERSION_FILE'." >&2
      echo "Assicurati che 'git' e 'make' siano installati e che '$LEGOROOT' sia la directory corretta." >&2
      exit 1
    fi
    echo "Info: File '$VERSION_FILE' generato con successo." >&2
  fi
  if [ ! -f "$VERSION_FILE" ]; then
    echo "Errore: Non è stato possibile trovare o creare '$VERSION_FILE'." >&2
    exit 1
  fi
  DEFINES=$(grep '^#define ' "$VERSION_FILE")
  FONTE="$VERSION_FILE (git non disponibile)"
fi

if [ "$DEFINE" = 1 ]; then
  echo "$DEFINES"
  exit 0
fi

echo "------- Ambiente LegoPST---------"
echo "LEGOROOT: $LEGOROOT"
echo "Fonte: $FONTE"
echo "--------------------------------"

# Usa awk per mostrare le informazioni in forma leggibile
echo "$DEFINES" | awk '
  /^#define / {
    nome_macro = $2
    valore = ""
    for (i = 3; i <= NF; i++) {
      valore = valore (i == 3 ? "" : " ") $i
    }

    # Rimuovi le virgolette dal valore per una visualizzazione più pulita
    gsub(/"/, "", valore)

    if (nome_macro == "GIT_VERSION_STRING") {
        printf "%-20s: %s\n", "Versione Git", valore
    } else if (nome_macro == "BUILD_NUMBER") {
        printf "%-20s: %s\n", "Numero Build", valore
    } else if (nome_macro == "BUILD_DATE_STRING") {
        printf "%-20s: %s\n", "Data", valore
    }
  }
'

echo "--------------------------------"
exit 0
