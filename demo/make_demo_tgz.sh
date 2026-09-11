#!/bin/bash
#
# make_demo_tgz.sh - confeziona la demo di LegoPST (legopst_userstd.tgz)
#
# Il tarball viene estratto dentro il container da lgdock.sh nella home
# dell'utente (vedi docker/lgdock.sh, opzione -d/--demo). Prima era confezionato
# a mano, e a mano ci finiva dentro roba che non deve viaggiare: questo script
# esiste per rendere l'esclusione riproducibile.
#
#   uso:  ./make_demo_tgz.sh [DIR_SORGENTE] [TGZ_DESTINAZIONE]
#   dflt: ~/legopst_userstd  ->  <repo>/demo/legopst_userstd.tgz
#
# COSA VIENE ESCLUSO, E PERCHE'
#
#   */proc   la directory di BUILD di ogni task: contiene l'eseguibile della
#            task (lg2), gli oggetti compilati (foraus.o) e i .dat generati.
#            Non deve viaggiare per tre motivi:
#
#            1. sono binari compilati sulla macchina di confezionamento, contro
#               le SUE librerie: su un'altra macchina non valgono niente. Chi
#               usa la demo rifa' la task con i propri eseguibili e librerie, e
#               proc/ viene ricreata li';
#            2. e' il grosso del pacchetto (47 MB non compressi);
#            3. sotto out/ il "proc" non e' nemmeno una directory ma un SYMLINK,
#               che net_sked ricrea da solo a ogni avvio di task - e prima lo
#               cancella apposta, per non lasciarne uno stantio (sked_start.c,
#               unlink() + symlink()). Quelli confezionati erano per giunta
#               assoluti e cablati sulla home di chi aveva fatto il pacchetto,
#               quindi rotti su qualunque altra macchina.
#
#            Attenzione: NON "aggiustarli" creando una directory vera al posto
#            del symlink. net_sked fa unlink() e poi symlink(): su una directory
#            l'unlink fallisce, il symlink fallisce con EEXIST e si va su
#            exit(1) - la task non parte. Assente e' lo stato giusto.
#
# Quello che invece RESTA: out/ con f21.dat, lg5.out, lg5c.out. f21.dat viene
# letto a runtime (sked_start.c, sked_fine.c, lg5sim.for), quindi si esclude
# "*/proc", non "*/out".

set -e

SORGENTE="${1:-$HOME/legopst_userstd}"
DEST="${2:-$(cd "$(dirname "$0")" && pwd)/legopst_userstd.tgz}"

if [ ! -d "$SORGENTE" ]; then
    echo "ERRORE: directory sorgente non trovata: $SORGENTE" >&2
    exit 1
fi

RADICE="$(dirname "$SORGENTE")"
NOME="$(basename "$SORGENTE")"

echo "Sorgente:    $SORGENTE"
echo "Destinazione: $DEST"
echo ""

# --owner/--group=0 --numeric-owner: NON registrare nel tarball l'UID di chi
# confeziona. Quell'UID e' una proprieta' della macchina di confezionamento e
# altrove non significa niente. Estraendo da root sotto un runtime rootless
# (Podman, Docker rootless) l'UID 1000 registrato qui esce sull'host come
# 99999+1000 = 100999, un utente che non esiste, e la demo diventa
# inaccessibile. lgdock.sh si difende gia' con --no-same-owner, ma il tarball
# viene anche aperto a mano: meglio che sia neutro all'origine.
#
# Niente -p: e' un'opzione di ESTRAZIONE. In creazione tar registra i modi del
# filesystem cosi' come sono, e l'umask non c'entra (verificato: con e senza -p
# il tarball esce identico anche con umask 077).
tar czf "$DEST" -C "$RADICE" \
    --owner=0 --group=0 --numeric-owner \
    --exclude='*/proc' "./$NOME"

echo ""
echo "--- verifica ---"
RESIDUI=$(tar tzf "$DEST" | grep -cE "/proc(/|$)" || true)
LINK=$(tar tvzf "$DEST" | grep -c '^l' || true)
BINARI=$(tar tzf "$DEST" | grep -cE '/(lg2|foraus\.o)$' || true)
PROPRIETARI=$(tar --numeric-owner -tvzf "$DEST" | awk '{print $2}' | sort -u | grep -cv '^0/0$' || true)
printf "  membri totali        : %s\n" "$(tar tzf "$DEST" | wc -l)"
printf "  membri 'proc'        : %s  (atteso 0)\n" "$RESIDUI"
printf "  symlink              : %s  (atteso 0)\n" "$LINK"
printf "  eseguibili di build  : %s  (atteso 0)\n" "$BINARI"
printf "  proprietari != 0/0   : %s  (atteso 0)\n" "$PROPRIETARI"
printf "  dimensione           : %s\n" "$(du -h "$DEST" | cut -f1)"

if [ "$RESIDUI" -ne 0 ] || [ "$LINK" -ne 0 ] || [ "$BINARI" -ne 0 ] \
   || [ "$PROPRIETARI" -ne 0 ]; then
    echo ""
    echo "ERRORE: il tarball contiene ancora roba che non deve viaggiare." >&2
    exit 1
fi
echo ""
echo "OK."
