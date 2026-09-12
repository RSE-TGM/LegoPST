#!/bin/bash
#
# make_demo_tgz.sh - confeziona la demo di LegoPST (legopst_userstd.tgz)
#
# Il tarball viene estratto dentro il container da lgdock.sh nella home
# dell'utente (vedi docker/lgdock.sh, opzione -d/--demo). Prima era confezionato
# a mano, e a mano ci finiva dentro roba che non deve viaggiare: questo script
# esiste per rendere la selezione riproducibile.
#
#   uso:  ./make_demo_tgz.sh [DIR_SORGENTE] [TGZ_DESTINAZIONE]
#   dflt: ~/legopst_userstd  ->  <repo>/demo/legopst_userstd.tgz
#
# COME SI DECIDE COSA ENTRA
#
#   Gli elenchi AMMESSE_* in testa allo script SELEZIONANO: quello che vi
#   compare entra nel pacchetto, tutto il resto resta fuori e viene solo
#   elencato a schermo. La sorgente di default (~/legopst_userstd) e' una
#   directory di LAVORO: ci si accumulano simulatori, copie di salvataggio,
#   prove e tarball intermedi. Pretendere che sia pulita per poter confezionare
#   la demo bloccava il lavoro senza motivo - il pacchetto lo definisce questo
#   elenco, non lo stato della directory di lavoro.
#
#   Quando la demo cambia davvero, si aggiorna QUESTO elenco: e' la definizione
#   di cosa la demo contiene, non un filtro di comodo.
#
# COSA VIENE ESCLUSO DENTRO A CIO' CHE ENTRA, E PERCHE'
#
#   */proc   la directory di BUILD di ogni task: contiene l'eseguibile della
#            task (lg2), gli oggetti compilati (foraus.o) e i .dat generati.
#            Non deve viaggiare per tre motivi:
#
#            1. sono binari compilati sulla macchina di confezionamento, contro
#               le SUE librerie: su un'altra macchina non valgono niente. Chi
#               usa la demo rifa' la task con i propri eseguibili e librerie, e
#               proc/ viene ricreata li';
#            2. e' il grosso del pacchetto;
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
#   */out    la directory di OUTPUT della corsa: ci finiscono lg5.out, lg5c.out
#            e f21.dat, piu' il symlink proc di cui sopra. E' roba prodotta da
#            una corsa sulla macchina di confezionamento, non materiale di
#            partenza: sked_start.c la ricrea da se' a ogni avvio, con mkdir()
#            sia di ./out sia di ./out/<modello> per ogni modello del
#            simulatore, e tollera che esistano gia' (la exit(1) dopo la
#            perror e' commentata apposta). Confezionarla significa spedire i
#            risultati di una corsa altrui e i symlink rotti che ci stanno
#            dentro.
#
# Le due esclusioni valgono a QUALUNQUE profondita': i bundle FMU annidati
# (legoclix_<task>_bundle/resources/bundle/task/<task>/) hanno le loro proc/ e
# out/, e vanno via anche quelle.

set -e

# Le due soglie di GitHub, che sono cose diverse:
#   50 MB  soglia CONSIGLIATA. Il push passa, ma il server stampa un warning
#          ("larger than GitHub's recommended maximum file size of 50.00 MB")
#          e suggerisce Git LFS. Fastidio, non errore.
#  100 MB  limite DURO. Il push viene respinto e il commit non entra.
# Nessuna delle due impedisce di produrre il pacchetto: qui si segnala e basta.
AVVISO_MB=50
AVVISO_BYTE=$((AVVISO_MB * 1024 * 1024))
LIMITE_MB=100
LIMITE_BYTE=$((LIMITE_MB * 1024 * 1024))

PROGRAMMA="$(basename "$0")"

aiuto() {
cat <<FINE_AIUTO
$PROGRAMMA - confeziona la demo di LegoPST (legopst_userstd.tgz)

USO
  $PROGRAMMA [DIR_SORGENTE] [TGZ_DESTINAZIONE]
  $PROGRAMMA -h | --help

  DIR_SORGENTE       default: \$HOME/legopst_userstd
  TGZ_DESTINAZIONE   default: <dir di questo script>/legopst_userstd.tgz

COSA ENTRA NEL PACCHETTO
  Lo decidono gli elenchi AMMESSE_* in testa allo script, che SELEZIONANO:

    AMMESSE_RADICE    le directory di primo livello
    AMMESSE_LEGOCAD   cosa si prende dentro legocad/
    AMMESSE_SKED      cosa si prende dentro sked/

  Quello che c'e' nella sorgente ma non compare negli elenchi resta fuori e
  viene solo elencato a schermo: la sorgente e' una directory di lavoro, non
  deve essere pulita perche' il confezionamento funzioni. Quando la demo
  cambia davvero si aggiornano quegli elenchi.

  Dentro a cio' che entra, a qualunque profondita', si scartano sempre:
    */proc   directory di build della task (eseguibili e oggetti compilati
             sulla macchina di confezionamento, inutili altrove)
    */out    directory di output della corsa (lg5.out, f21.dat e il symlink
             proc con path assoluto). net_sked le ricrea da se' a ogni avvio.

COSA STAMPA
  - le directory ammesse, una per riga, con la loro dimensione
  - il totale non compresso
  - le voci lasciate fuori e quelle ammesse ma assenti
  - la verifica del tarball (proc, out, symlink, binari, proprietari)
  - il pacchetto prodotto con la sua dimensione

  Il pacchetto viene prodotto SEMPRE, e non viene mai cancellato. Sulla sua
  dimensione ci sono due segnalazioni, che sono le due soglie di GitHub:

    oltre ${AVVISO_MB} MB    soglia consigliata: il push passa, il server avvisa
    oltre ${LIMITE_MB} MB   limite duro: il push verrebbe respinto

  In entrambi i casi si segnala soltanto: ritagliare gli elenchi AMMESSE_* e
  rifare il pacchetto e' una decisione di chi confeziona.

CODICI DI USCITA
  0  fatto (anche quando il pacchetto e' sopra le soglie: sono avvisi)
  1  sorgente inesistente o vuota di ammesse, oppure tarball venuto male
     (in quest'ultimo caso il file c'e' lo stesso, ma e' da rifare)

ESEMPI
  $PROGRAMMA
  $PROGRAMMA ~/legopst_userstd_pulita
  $PROGRAMMA ~/legopst_userstd /tmp/prova.tgz
FINE_AIUTO
}

case "$1" in
    -h|--help|-help|--aiuto)
        aiuto
        exit 0
        ;;
    -*)
        echo "ERRORE: opzione sconosciuta: $1" >&2
        echo "        \"$PROGRAMMA --help\" per l'uso." >&2
        exit 1
        ;;
esac

SORGENTE="${1:-$HOME/legopst_userstd}"
DEST="${2:-$(cd "$(dirname "$0")" && pwd)/legopst_userstd.tgz}"

if [ ! -d "$SORGENTE" ]; then
    echo "ERRORE: directory sorgente non trovata: $SORGENTE" >&2
    exit 1
fi

SORGENTE="$(cd "$SORGENTE" && pwd)"
RADICE="$(dirname "$SORGENTE")"
NOME="$(basename "$SORGENTE")"

echo "Sorgente:     $SORGENTE"
echo "Destinazione: $DEST"
echo ""

# ---------------------------------------------------------------------------
# COSA FA PARTE DELLA DEMO
# ---------------------------------------------------------------------------
AMMESSE_RADICE="legocad sked"
AMMESSE_LEGOCAD="libgraph libut libut_reg MDC_GV collet ctrcoll r_MDC0"
AMMESSE_SKED="duetask"

# Un livello e' un CONTENITORE se ha un elenco di ammesse proprio: di lui si
# confeziona solo la directory in se', e poi si scende. Altrimenti e' una
# FOGLIA e si confeziona tutto il sottoalbero (meno proc/ e out/).
# La chiave e' il path relativo, non il nome: due directory omonime a livelli
# diversi restano distinte.
figli_di() {
    case "$1" in
        "")        printf '%s' "$AMMESSE_RADICE" ;;
        legocad)   printf '%s' "$AMMESSE_LEGOCAD" ;;
        sked)      printf '%s' "$AMMESSE_SKED" ;;
        *)         printf '%s' "" ;;
    esac
}

CONTENITORI=()   # dir da registrare nel tar senza ricorsione
MEMBRI=()        # sottoalberi da confezionare per intero
MANCANTI=()      # elencate fra le ammesse ma assenti nella sorgente
ESCLUSI=()       # presenti nella sorgente ma fuori dall'elenco

# $1 = path relativo dentro la sorgente ("" = radice)
raccogli() {
    local rel="$1" ammesse nome sub figli
    ammesse="$(figli_di "$rel")"

    # find, non il glob: cosi' entrano anche i nomi che cominciano con un punto
    while IFS= read -r nome; do
        case " $ammesse " in
            *" $nome "*) ;;
            *) ESCLUSI+=("${rel:+$rel/}$nome") ;;
        esac
    done < <(find "$SORGENTE${rel:+/$rel}" -mindepth 1 -maxdepth 1 -printf '%f\n' | sort)

    for nome in $ammesse; do
        sub="${rel:+$rel/}$nome"
        if [ ! -e "$SORGENTE/$sub" ]; then
            MANCANTI+=("$sub")
            continue
        fi
        figli="$(figli_di "$sub")"
        if [ -n "$figli" ]; then
            CONTENITORI+=("$sub")
            raccogli "$sub"
        else
            MEMBRI+=("$sub")
        fi
    done
}

raccogli ""

if [ ${#MEMBRI[@]} -eq 0 ]; then
    echo "ERRORE: nessuna delle directory ammesse esiste sotto $SORGENTE." >&2
    echo "        Controlla la sorgente, o gli elenchi AMMESSE_* in testa a" >&2
    echo "        questo script." >&2
    exit 1
fi

# ---------------------------------------------------------------------------
# COSA ENTRA, CON LE DIMENSIONI
# ---------------------------------------------------------------------------
# du con le stesse esclusioni del tar, altrimenti i numeri non corrispondono a
# quello che finisce davvero nel pacchetto.
misura() { du -sb --exclude=proc --exclude=out "$1" | cut -f1; }
umana()  { numfmt --to=iec --format='%.1f' "$1"; }

echo "--- contenuto della demo (proc/ e out/ escluse) ---"
TOTALE=0
for m in "${MEMBRI[@]}"; do
    dim="$(misura "$SORGENTE/$m")"
    TOTALE=$((TOTALE + dim))
    printf '  %-40s %10s\n' "$m" "$(umana "$dim")"
done
printf '  %-40s %10s\n' "$(printf '%.0s-' {1..40})" "----------"
printf '  %-40s %10s\n' "totale non compresso" "$(umana "$TOTALE")"
echo ""

if [ -n "${ESCLUSI[*]}" ]; then
    echo "Lasciati fuori (non in elenco, $((${#ESCLUSI[@]})) voci):"
    printf '  %s\n' "${ESCLUSI[@]}"
    echo ""
fi

# Non fatale: una demo incompleta e' un guaio diverso, e chi confeziona una
# demo ridotta apposta deve poterlo fare.
if [ -n "${MANCANTI[*]}" ]; then
    echo "ATTENZIONE: elencate fra le ammesse ma assenti nella sorgente:"
    printf '  %s\n' "${MANCANTI[@]}"
    echo ""
fi

if [ "$TOTALE" -gt "$LIMITE_BYTE" ]; then
    echo "ATTENZIONE: il contenuto non compresso supera i $LIMITE_MB MB."
    echo "            Il pacchetto compresso potrebbe superare il limite di"
    echo "            GitHub: si vede fra poco."
    echo ""
fi

# ---------------------------------------------------------------------------
# CONFEZIONAMENTO
# ---------------------------------------------------------------------------
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
#
# --no-recursion prima dei contenitori: di ~/legopst_userstd e di legocad/ e
# sked/ si registra la sola directory, senza tirarsi dietro quello che ci sta
# accanto e non e' ammesso. Poi --recursion riapre la ricorsione per i
# sottoalberi veri. Le opzioni di tar valgono per i nomi che le seguono:
# l'ordine qui sotto e' voluto.
echo "Confeziono..."
tar czf "$DEST" -C "$RADICE" \
    --owner=0 --group=0 --numeric-owner \
    --exclude='*/proc' --exclude='*/out' \
    --no-recursion "./$NOME" "${CONTENITORI[@]/#/./$NOME/}" \
    --recursion "${MEMBRI[@]/#/./$NOME/}"

# ---------------------------------------------------------------------------
# VERIFICA
# ---------------------------------------------------------------------------
NOMI="$(mktemp)"; DETT="$(mktemp)"
trap 'rm -f "$NOMI" "$DETT"' EXIT
tar tzf "$DEST" > "$NOMI"
tar --numeric-owner -tvzf "$DEST" > "$DETT"

RES_PROC=$(grep -cE '/proc(/|$)' "$NOMI" || true)
RES_OUT=$(grep -cE '/out(/|$)' "$NOMI" || true)
LINK=$(grep -c '^l' "$DETT" || true)
BINARI=$(grep -cE '/(lg2|foraus\.o)$' "$NOMI" || true)
PROPRIETARI=$(awk '{print $2}' "$DETT" | sort -u | grep -cv '^0/0$' || true)
DIM_TGZ=$(stat -c%s "$DEST")

echo ""
echo "--- verifica ---"
printf '  %-40s %10s\n' "membri totali" "$(wc -l < "$NOMI")"
printf '  %-40s %10s  (atteso 0)\n' "membri 'proc'" "$RES_PROC"
printf '  %-40s %10s  (atteso 0)\n' "membri 'out'" "$RES_OUT"
printf '  %-40s %10s  (atteso 0)\n' "symlink" "$LINK"
printf '  %-40s %10s  (atteso 0)\n' "eseguibili di build" "$BINARI"
printf '  %-40s %10s  (atteso 0)\n' "proprietari != 0/0" "$PROPRIETARI"
echo ""
echo "--- pacchetto ---"
printf '  %-40s %10s\n' "$(basename "$DEST")" "$(umana "$DIM_TGZ")"
echo ""

# Il pacchetto e' gia' scritto e resta dov'e': qui non si blocca niente, si
# dice soltanto che e' venuto male. Se uno dei contatori sopra non e' zero il
# tarball si porta dietro roba che non deve viaggiare, e va rifatto - ma la
# decisione e' di chi confeziona. Exit 1 solo perche' il difetto si veda anche
# da uno script che chiami questo.
ESITO=0
if [ "$RES_PROC" -ne 0 ] || [ "$RES_OUT" -ne 0 ] || [ "$LINK" -ne 0 ] \
   || [ "$BINARI" -ne 0 ] || [ "$PROPRIETARI" -ne 0 ]; then
    echo "ATTENZIONE: il tarball e' stato prodotto, ma contiene roba che non"
    echo "            deve viaggiare (vedi i contatori sopra diversi da zero)."
    echo ""
    ESITO=1
fi

if [ "$DIM_TGZ" -gt "$LIMITE_BYTE" ]; then
    echo "ATTENZIONE: il pacchetto supera i $LIMITE_MB MB ($(umana "$DIM_TGZ"))."
    echo "            E' il limite DURO di GitHub: il push verrebbe respinto e"
    echo "            il commit non entrerebbe. Ritaglia gli elenchi AMMESSE_*"
    echo "            in testa a questo script e rifa' il pacchetto."
    echo ""
    echo "Il pacchetto resta dov'e', ma cosi' non si puo' committare."
    exit "$ESITO"
elif [ "$DIM_TGZ" -gt "$AVVISO_BYTE" ]; then
    echo "NOTA: il pacchetto supera i $AVVISO_MB MB ($(umana "$DIM_TGZ")), la soglia"
    echo "      CONSIGLIATA da GitHub: il push passa, ma il server stampa un"
    echo "      warning. Il limite duro e' a $LIMITE_MB MB, e siamo sotto."
    echo ""
    echo "      Tieni conto che ogni versione del pacchetto resta nella storia"
    echo "      del repository per sempre, anche quando la sostituisci."
    echo ""
fi

if [ "$ESITO" -eq 0 ]; then
    echo "OK."
fi
exit "$ESITO"
