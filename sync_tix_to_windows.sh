#!/bin/bash
# sync_tix_to_windows.sh
# Sincronizza Alg_legopc/bin/tix_new/ → repo Windows \tix
# Copia/aggiorna solo i file provenienti da Linux.
# I file Windows-only (.bat, makefile, .txt, ecc.) non vengono toccati.
# Nota: file cancellati da tix_new vanno rimossi manualmente da \tix se necessario.

SRC="/home/antonio/LegoPST/Alg_legopc/bin/tix_new/"
DST="/mnt/c/Disco_D/git_wa/legopc_prj/src/tix/"

if [ ! -d "$DST" ]; then
    echo "ERRORE: destinazione non trovata: $DST"
    exit 1
fi

echo "Sorgente: $SRC"
echo "Destinazione: $DST"
read -r -p "Confermi la sincronizzazione? [s/N] " risposta
if [[ ! "$risposta" =~ ^[sS]$ ]]; then
    echo "Annullato."
    exit 0
fi

echo "Sincronizzazione: $SRC → $DST"
rsync -av "$SRC" "$DST"

echo "Conversione LF→CRLF per tutti i file di testo ..."
find "$DST" -type f -exec unix2dos -q {} \;

echo "Fatto."
