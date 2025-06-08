#!/bin/bash

# Nome del file da leggere
VERSION_FILE="$LEGOROOT/version.h"

# Verifica se il file esiste
if [ ! -f "$VERSION_FILE" ]; then
  echo "Errore: Il file '$VERSION_FILE' non è stato trovato."
  exit 1
fi

echo "------- Ambiente LegoPST---------"
echo "LEGOROOT $LEGOROOT"
#echo "File $VERSION_FILE"
echo "--------------------------------"
# Usa awk per processare il file
awk '
  # /^#define / è il pattern: esegui il blocco solo per le righe che iniziano con "#define "
  /^#define / {
    # $1 è il primo campo (es. "#define")
    # $2 è il secondo campo (es. "GIT_VERSION_STRING")
    # $3 è il terzo campo (es. "\"638b80d-dirty\"" o "47")

    # Inizializza la variabile del valore
    valore = ""

    # Concatena i campi dal terzo in poi per formare il valore completo
    # Questo gestisce valori che potrebbero contenere spazi se non fossero tra virgolette
    # (anche se nel tuo esempio non è il caso, è una buona pratica)
    for (i = 3; i <= NF; i++) {
      valore = valore (i == 3 ? "" : " ") $i
    }
    
    # Stampa il secondo campo (nome della macro) e il valore ricostruito
    print $2, valore
  }
' "$VERSION_FILE"
echo "--------------------------------"
exit 0