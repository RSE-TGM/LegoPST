#!/usr/bin/env bash
#
# travasa_f14.sh
#
# Porta i dati iniziali e i parametri dal f14.dat di un modello legocad
# d'epoca nel f14.dat del modello convertito con f01totom.
#
# PERCHE' SERVE
#   f01totom produce solo il .tom. Aprendolo in lgpc e costruendo il modello,
#   la catena genera un f14.dat con la struttura giusta ma tutti i valori in
#   bianco: condizioni iniziali, variabili di ingresso e dati fisici dei
#   blocchi. Il modello non parte finche' non si riempiono.
#
#   La copia secca del vecchio f14.dat NON va bene: i moduli di libreria di
#   oggi hanno variabili e porte diverse da quelle dell'epoca, quindi i due
#   file hanno insiemi di nomi diversi (su GTS: 179 variabili contro 248). Il
#   travaso va fatto per nome, ed e' esattamente quello che sa fare edi14,
#   lo strumento storico di LegoPST (src/main_lego/edi14.for), lo stesso che
#   legopc usa nella proc autoedi14.
#
#   Questo script aggiunge a edi14 le due cose che gli mancano: il travaso
#   della riga dei dati di normalizzazione (P0, H0, W0, T0, R0, L0, V0, DP0),
#   che edi14 lascia in bianco, e un conteggio di cosa e' stato riempito.
#
# USO
#   bash travasa_f14.sh <dir del modello convertito> <f14.dat del modello vecchio>
#
#   es. bash travasa_f14.sh $LG_MODELS/GTS_conv $LG_MODELS/GTS/f14.dat
#
# Il f14.dat di partenza viene salvato come f14.dat.pre_travaso.
# Richiede LEGO_BIN (source .profile_legoroot).

set -uo pipefail

if [[ $# -ne 2 ]]; then
    echo "Uso: bash travasa_f14.sh <dir modello convertito> <f14.dat vecchio>"
    exit 2
fi

NUOVO_DIR="$1"
VECCHIO="$2"
EDI14="${LEGO_BIN:-}/edi14c"

[[ -n "${LEGO_BIN:-}" ]] || { echo "ERRORE: LEGO_BIN non definita. Eseguire: source .profile_legoroot"; exit 1; }
[[ -x "$EDI14"        ]] || { echo "ERRORE: $EDI14 non eseguibile"; exit 1; }
[[ -d "$NUOVO_DIR"    ]] || { echo "ERRORE: directory inesistente: $NUOVO_DIR"; exit 1; }
[[ -f "$NUOVO_DIR/f14.dat" ]] || { echo "ERRORE: manca $NUOVO_DIR/f14.dat (il modello va prima costruito in lgpc)"; exit 1; }
[[ -f "$VECCHIO"      ]] || { echo "ERRORE: file inesistente: $VECCHIO"; exit 1; }

VECCHIO_ASS=$(readlink -f "$VECCHIO")

# conta i valori riempiti e quelli in bianco, per sezione
conta() {
    awk '
        /^\*LG\*CONDIZIONI INIZIALI VARIABILI DEL SISTEMA/ { sez="sistema";  next }
        /^\*LG\*CONDIZIONI INIZIALI VARIABILI DI INGRESSO/ { sez="ingresso"; next }
        /^\*LG\*DATI FISICI/                               { sez="blocchi";  next }
        /^\*LG\*/ { next }
        sez=="" || $0 ~ /^[ \t]*$/ { next }
        sez=="blocchi" {
            # righe a gruppi: "    NOME  N =valore    *"
            n=split($0, pezzi, "*")
            for (i=1; i<=n; i++) {
                p=index(pezzi[i], "=")
                if (p>0) { v=substr(pezzi[i], p+1); gsub(/[ \t]/,"",v)
                           if (v=="") vuoti[sez]++; else pieni[sez]++ }
            }
            next
        }
        {
            v=substr($0, 15, 10); gsub(/[ \t]/,"",v)
            if (v=="") vuoti[sez]++; else pieni[sez]++
        }
        END {
            for (s in pieni) tot[s]=1
            for (s in vuoti) tot[s]=1
            for (s in tot) printf "   %-9s valorizzati=%4d  vuoti=%4d\n", s, pieni[s]+0, vuoti[s]+0
        }' "$1" | sort
}

echo "PRIMA:"
conta "$NUOVO_DIR/f14.dat"

cp -p "$NUOVO_DIR/f14.dat" "$NUOVO_DIR/f14.dat.pre_travaso"

# edi14 vuole f14.dat nella directory corrente, legge da stdin il nome del
# file vecchio e scrive edi14.out
( cd "$NUOVO_DIR" && echo "$VECCHIO_ASS" | "$EDI14" > edi14_travaso.log 2>&1 )
rc=$?
if [[ $rc -ne 0 || ! -s "$NUOVO_DIR/edi14.out" ]]; then
    echo "ERRORE: edi14c non ha prodotto edi14.out (exit=$rc). Log: $NUOVO_DIR/edi14_travaso.log"
    exit 1
fi

mv "$NUOVO_DIR/edi14.out" "$NUOVO_DIR/f14.dat"

# edi14 non tocca i dati di normalizzazione: si prendono dal file vecchio.
# Sono le righe che cominciano per "*LG*DATI DI NORMALIZZAZIONE" e la sua
# continuazione, e valgono per l'impianto, non per il modello.
awk '
    NR==FNR {
        if ($0 ~ /^\*LG\*DATI DI NORMALIZZAZIONE/) { norm1=$0; attesa=1; next }
        if (attesa==1)                             { norm2=$0; attesa=0 }
        next
    }
    /^\*LG\*DATI DI NORMALIZZAZIONE/ { print norm1; presa=1; next }
    presa==1                         { print norm2; presa=0; next }
    { print }
' "$VECCHIO_ASS" "$NUOVO_DIR/f14.dat" > "$NUOVO_DIR/f14.dat.tmp" \
    && mv "$NUOVO_DIR/f14.dat.tmp" "$NUOVO_DIR/f14.dat"

echo "DOPO:"
conta "$NUOVO_DIR/f14.dat"
echo
echo "dati di normalizzazione:"
grep -m1 '^\*LG\*DATI DI NORMALIZZAZIONE' "$NUOVO_DIR/f14.dat" | sed 's/^/   /' | cut -c1-90
echo
echo "I valori rimasti in bianco vanno messi a mano: sono variabili che nel"
echo "modello d'epoca non esistevano (moduli di libreria cambiati) oppure"
echo "ingressi diventati liberi perche' la connessione non e' stata ricostruita."
echo "Dettaglio di cosa edi14 non ha trovato: $NUOVO_DIR/edi14_travaso.log"
echo "Copia di sicurezza: $NUOVO_DIR/f14.dat.pre_travaso"
