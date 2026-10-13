#!/bin/bash
#
# lgdemo - scarica e installa nella home una demo di LegoPST.
#
# Le demo sono aree di lavoro gia' pronte (una directory legopst_<nome> con
# dentro legocad e sked) pubblicate come allegati legopst_<nome>.tgz della
# release "Demo LegoPST" del repository GitHub di LegoPST. Le confeziona e le
# pubblica demo/publish_demo.sh; "lgrun -d <nome>" installa le stesse demo
# dentro il container, questo comando lo fa su una installazione nativa.
#
#   uso:  lgdemo [opzioni] [NOME]
#
#     lgdemo            elenca le demo pubblicate e chiede quale installare
#     lgdemo NOME       installa la demo NOME in ~/legopst_NOME
#                       (va bene anche legopst_NOME o legopst_NOME.tgz)
#
#     -l, --list        elenca le demo pubblicate ed esce, senza chiedere niente
#     -y, --yes         non chiede conferma se la demo e' gia' installata
#         --esito FILE  a installazione riuscita scrive in FILE il nome
#                       dell'area (legopst_NOME). Serve a chi lancia lgdemo
#                       da un programma: lghmi lo usa per passare alla demo
#     -h, --help        questo aiuto
#
# SE LA DEMO C'E' GIA'
#
#   Lo si segnala e si chiede conferma. Se si conferma, la directory che c'e'
#   NON viene cancellata ne' sovrascritta: viene messa da parte con il nome
#   old.legopst_<nome>.<data>-<ora>, e la demo nuova prende il suo posto.
#   Dentro potrebbe esserci del lavoro; cancellarla e' una decisione di chi
#   lavora, non di questo script. Il nome non comincia per "legopst_" apposta:
#   e' il modello con cui lgswitch cerca le aree, e una copia messa da parte
#   non deve comparire nel suo menu.
#
#   La demo nuova viene scaricata, controllata ed estratta in una directory
#   provvisoria PRIMA di toccare quella che c'e': se qualcosa va storto (rete,
#   sha256, pacchetto fatto male) la home resta com'era.
#
# DOPO L'INSTALLAZIONE
#
#   lgdemo non tocca i link ~/legocad e ~/sked. Per lavorare sulla demo:
#       cd ~ ; lgswitch legopst_<nome>
#
# VARIABILI D'AMBIENTE
#
#   LG_DEMO_RELEASE   tag della release da cui prendere le demo (default:
#                     demo-2.0, lo stesso di lgrun)
#   LG_DEMO_REPO      repository GitHub, come proprietario/nome (default:
#                     RSE-TGM/LegoPST)
#   GITHUB_TOKEN      facoltativo: GitHub concede 60 interrogazioni l'ora a chi
#                     non si presenta, e con un token il limite sparisce

set -o pipefail

PROGRAMMA="$(basename "$0")"
REPO_SLUG="${LG_DEMO_REPO:-RSE-TGM/LegoPST}"
DEMO_RELEASE="${LG_DEMO_RELEASE:-demo-2.0}"
API="https://api.github.com/repos/$REPO_SLUG/releases/tags/$DEMO_RELEASE"

if [ -t 1 ]; then
    #  Colori che si leggono su fondo chiaro come su fondo scuro: il terminale
    #  in cui lghmi apre lgdemo e' spesso un xterm bianco, e li' il giallo
    #  non si vede. Gli avvisi sono in magenta, non in giallo.
    GREEN='\033[0;32m' ; YELLOW='\033[0;35m' ; RED='\033[0;31m' ; NC='\033[0m'
else
    GREEN='' ; YELLOW='' ; RED='' ; NC=''
fi

muori() { echo -e "${RED}ERRORE:${NC} $*" >&2; exit 1; }
avviso() { echo -e "${YELLOW}ATTENZIONE:${NC} $*" >&2; }

SOLO_LISTA=false
SENZA_CONFERMA=false
NOME=""
FILE_ESITO=""

while [ $# -gt 0 ]; do
    case "$1" in
        -l|--list)  SOLO_LISTA=true ;;
        -y|--yes)   SENZA_CONFERMA=true ;;
        --esito)    FILE_ESITO="${2:?$1 vuole il nome del file}"; shift ;;
        --esito=*)  FILE_ESITO="${1#--esito=}" ;;
        -h|--help)  sed -n '3,/^[^#]/p' "$0" | sed -n 's/^# \{0,1\}//p'; exit 0 ;;
        -*)         echo "$PROGRAMMA: opzione non riconosciuta: $1 (vedi -h)" >&2; exit 1 ;;
        *)          [ -z "$NOME" ] || { echo "$PROGRAMMA: una demo alla volta (vedi -h)" >&2; exit 1; }
                    NOME="$1" ;;
    esac
    shift
done

command -v curl >/dev/null 2>&1 || muori "serve 'curl' per scaricare le demo."
[ -n "${HOME:-}" ] && [ -d "$HOME" ] && [ -w "$HOME" ] || muori "la home ('${HOME:-}') non c'e' o non e' scrivibile."

TMP=""          # file provvisori, fuori dalla home
PROVV=""        # directory provvisoria di estrazione, DENTRO la home
pulisci() {
    [ -n "$TMP" ] && rm -rf "$TMP"
    [ -n "$PROVV" ] && rm -rf "$PROVV"
}
trap pulisci EXIT
trap 'echo; echo "$PROGRAMMA: interrotto." >&2; exit 130' INT TERM
TMP="$(mktemp -d "${TMPDIR:-/tmp}/lgdemo.XXXXXX")" || muori "non riesco a creare una directory provvisoria."

#  Le demo pubblicate, una per riga:  <nome>|<byte>|<sha256>
#
#  Si leggono dalla descrizione della release che da' GitHub. E' JSON, ma qui
#  non lo si interpreta con python o jq, che su una installazione ridotta
#  possono mancare: GitHub lo scrive una chiave per riga, e di ogni allegato
#  servono tre chiavi che arrivano sempre nello stesso ordine (name, size,
#  digest, e in fondo browser_download_url che chiude l'allegato).
#  Lo sha256 puo' mancare (allegati vecchi): in quel caso il campo resta vuoto.
#
#  Ritorna 1 se la release non si riesce a leggere; il perche' lo dice su stderr.
ELENCO=""
leggi_elenco() {
    local http riga nome="" byte="" sha=""
    local -a intest=(-H "Accept: application/vnd.github+json")
    [ -n "${GITHUB_TOKEN:-}" ] && intest+=(-H "Authorization: Bearer $GITHUB_TOKEN")
    http="$(curl -sS --connect-timeout 15 --max-time 60 "${intest[@]}" \
                 -o "$TMP/rel.json" -w '%{http_code}' "$API" 2>"$TMP/curl.err")" || http="000"
    case "$http" in
        200) ;;
        404) echo "la release '$DEMO_RELEASE' di $REPO_SLUG non esiste." >&2; return 1 ;;
        403|429) echo "GitHub ha rifiutato l'interrogazione (troppe richieste da questo indirizzo: riprova fra un'ora, o definisci GITHUB_TOKEN)." >&2; return 1 ;;
        000) echo "GitHub non risponde ($(head -1 "$TMP/curl.err" 2>/dev/null))." >&2; return 1 ;;
        *)   echo "GitHub ha risposto $http leggendo la release '$DEMO_RELEASE'." >&2; return 1 ;;
    esac
    ELENCO=""
    while IFS= read -r riga; do
        case "$riga" in
            *'"name": "legopst_'*'.tgz"'*)
                nome="${riga#*\"name\": \"legopst_}"; nome="${nome%%.tgz\"*}"; byte=""; sha="" ;;
            *'"size": '*)
                [ -n "$nome" ] && { byte="${riga#*\"size\": }"; byte="${byte%%[!0-9]*}"; } ;;
            *'"digest": "sha256:'*)
                [ -n "$nome" ] && { sha="${riga#*\"digest\": \"sha256:}"; sha="${sha%%\"*}"; } ;;
            *'"browser_download_url": '*)
                [ -n "$nome" ] && ELENCO+="$nome|$byte|$sha"$'\n'
                nome="" ;;
        esac
    done < "$TMP/rel.json"
    ELENCO="$(printf '%s' "$ELENCO" | sort)"
    return 0
}

dimensione() {      # $1 = byte -> "54M"
    if [ -n "$1" ] && command -v numfmt >/dev/null 2>&1; then
        numfmt --to=iec "$1" 2>/dev/null || echo "$1"
    else
        echo "${1:-?}"
    fi
}

#  Stampa l'elenco numerato. Accanto a ogni demo dice se e' gia' nella home.
mostra_elenco() {
    local n=0 nome byte sha nota
    echo "Demo pubblicate (release '$DEMO_RELEASE' di $REPO_SLUG):"
    echo ""
    while IFS='|' read -r nome byte sha; do
        [ -n "$nome" ] || continue
        n=$((n + 1))
        nota=""
        [ -e "$HOME/legopst_$nome" ] && nota="${YELLOW}gia' installata in ~/legopst_$nome${NC}"
        printf '  %2d) %-20s %6s   ' "$n" "$nome" "$(dimensione "$byte")"
        echo -e "$nota"
    done <<< "$ELENCO"
    echo ""
}

# --- 1. quale demo ------------------------------------------------------------

ELENCO_LETTO=false
if leggi_elenco 2>"$TMP/elenco.err"; then
    ELENCO_LETTO=true
fi

if [ -z "$NOME" ] || $SOLO_LISTA; then
    $ELENCO_LETTO || muori "non riesco a leggere l'elenco delle demo: $(cat "$TMP/elenco.err")"
    [ -n "$ELENCO" ] || muori "la release '$DEMO_RELEASE' non ha nessun allegato legopst_<nome>.tgz."
    mostra_elenco
    $SOLO_LISTA && exit 0
    [ -t 0 ] || muori "nessuna demo indicata e nessun terminale a cui chiederla: usa '$PROGRAMMA NOME'."
    QUANTE="$(printf '%s\n' "$ELENCO" | grep -c .)"
    while :; do
        read -r -p "Quale installo? [1-$QUANTE, oppure il nome; invio = nessuna]: " SCELTA || { echo; exit 0; }
        [ -n "$SCELTA" ] || { echo "Niente da fare."; exit 0; }
        if [[ "$SCELTA" =~ ^[0-9]+$ ]]; then
            if [ "$SCELTA" -ge 1 ] && [ "$SCELTA" -le "$QUANTE" ]; then
                NOME="$(printf '%s\n' "$ELENCO" | sed -n "${SCELTA}p" | cut -d'|' -f1)"
                break
            fi
        elif [[ "$SCELTA" =~ ^[A-Za-z0-9_-]+$ ]] && printf '%s\n' "$ELENCO" | grep -q "^$SCELTA|"; then
            NOME="$SCELTA"
            break
        fi
        echo "Scelta non valida: '$SCELTA'."
    done
fi

#  Si accetta anche il nome della directory o quello del pacchetto.
NOME="${NOME%.tgz}"
NOME="${NOME#legopst_}"
[[ "$NOME" =~ ^[A-Za-z0-9_][A-Za-z0-9_-]*$ ]] || muori "nome di demo non valido: '$NOME' (lettere, cifre, _ e -)."

DIR_DEMO="legopst_$NOME"
DEST="$HOME/$DIR_DEMO"
URL="https://github.com/$REPO_SLUG/releases/download/$DEMO_RELEASE/$DIR_DEMO.tgz"
BYTE="" ; SHA=""

if $ELENCO_LETTO; then
    RIGA="$(printf '%s\n' "$ELENCO" | grep "^$NOME|" || true)"
    if [ -z "$RIGA" ]; then
        echo -e "${RED}ERRORE:${NC} la demo '$NOME' non c'e' nella release '$DEMO_RELEASE'." >&2
        [ -n "$ELENCO" ] && { echo "" >&2; mostra_elenco >&2; }
        exit 1
    fi
    BYTE="$(echo "$RIGA" | cut -d'|' -f2)"
    SHA="$(echo "$RIGA" | cut -d'|' -f3)"
else
    #  L'elenco non si legge (di solito: troppe interrogazioni), ma il
    #  pacchetto si scarica da un altro indirizzo, che non ha quel limite.
    avviso "non riesco a leggere l'elenco delle demo: $(cat "$TMP/elenco.err")"
    echo "            Provo lo stesso a scaricare $DIR_DEMO.tgz, senza poterne controllare lo sha256." >&2
fi

# --- 2. c'e' gia'? ------------------------------------------------------------

if [ -e "$DEST" ] || [ -L "$DEST" ]; then
    echo -e "${YELLOW}La demo '$NOME' e' gia' installata:${NC} $DEST"
    if [ -d "$DEST" ]; then
        echo "  occupa $(du -sh "$DEST" 2>/dev/null | cut -f1), ultima modifica $(date -r "$DEST" '+%Y-%m-%d %H:%M' 2>/dev/null)"
    fi
    for l in legocad sked; do
        if [ -L "$HOME/$l" ] && [ "$(readlink "$HOME/$l")" = "$DIR_DEMO/$l" -o "$(readlink "$HOME/$l")" = "$DEST/$l" ]; then
            echo "  ~/$l punta li': e' l'area di lavoro in uso."
        fi
    done
    echo "  Se la installo di nuovo, quella che c'e' viene messa da parte (non cancellata)"
    echo "  come ~/old.$DIR_DEMO.<data>-<ora>, e la demo appena scaricata prende il suo posto."
    if ! $SENZA_CONFERMA; then
        [ -t 0 ] || muori "la demo c'e' gia' e non c'e' un terminale a cui chiedere conferma: usa -y per installarla comunque."
        read -r -p "La installo comunque? [s/N]: " RISPOSTA || { echo; exit 0; }
        case "$RISPOSTA" in
            s|S|si|SI|Si|y|Y|yes) ;;
            *) echo "Lasciata com'e'."; exit 0 ;;
        esac
    fi
fi

# --- 3. scarico e controllo ---------------------------------------------------

TGZ="$TMP/$DIR_DEMO.tgz"
echo "Scarico $DIR_DEMO.tgz$([ -n "$BYTE" ] && echo " ($(dimensione "$BYTE"))")"
echo "  $URL"
BARRA="-sS" ; [ -t 2 ] && BARRA="-#"
curl -fL $BARRA --retry 3 --connect-timeout 15 -o "$TGZ" "$URL" \
    || muori "non riesco a scaricare la demo '$NOME' ($URL)."

if [ -n "$SHA" ] && command -v sha256sum >/dev/null 2>&1; then
    SHA_SCARICATO="$(sha256sum "$TGZ" | cut -d' ' -f1)"
    [ "$SHA_SCARICATO" = "$SHA" ] || muori "lo sha256 del pacchetto scaricato non e' quello pubblicato.
        pubblicato: $SHA
        scaricato:  $SHA_SCARICATO
        Il file e' arrivato rovinato, o e' stato sostituito mentre lo scaricavo: riprova."
    echo "  sha256 corretto"
elif $ELENCO_LETTO; then
    avviso "sha256 non controllato (GitHub non lo pubblica per questo allegato, o manca sha256sum)."
fi

# --- 4. estraggo in una directory provvisoria ---------------------------------

#  Nella home, non in /tmp: cosi' metterla al suo posto e' una rinomina sullo
#  stesso disco, che o riesce tutta o non riesce.
PROVV="$(mktemp -d "$HOME/.lgdemo.XXXXXX")" || muori "non riesco a creare una directory provvisoria nella home."
echo "Estraggo..."
#  --no-same-owner: i file sono di chi installa, non di chi ha confezionato.
tar --no-same-owner -xzf "$TGZ" -C "$PROVV" || muori "il pacchetto non si estrae (disco pieno? file rovinato?)."

#  Il pacchetto deve contenere proprio legopst_<nome>, con legocad e sked: e'
#  un'area di lavoro, e senza quelle due directory lgswitch non la userebbe.
if [ ! -d "$PROVV/$DIR_DEMO/legocad" ] || [ ! -d "$PROVV/$DIR_DEMO/sked" ]; then
    muori "il pacchetto non contiene $DIR_DEMO/legocad e $DIR_DEMO/sked.
        Va rifatto con demo/make_demo_tgz.sh -d $NOME."
fi
ALTRO="$(ls -A "$PROVV" | grep -v -x "$DIR_DEMO" || true)"
[ -z "$ALTRO" ] || avviso "il pacchetto contiene anche altro, che non installo: $(echo $ALTRO)"

#  Allinea la demo a QUESTA home.
#
#  I file dell'MMI portano scritta per esteso la home di chi ha confezionato la
#  demo: kGlobContext mette nel Context.ctx "*simulator: $KSIM", "*pages:
#  $KPAGES" e le librerie in "$HOME/legocad/...", cioe' percorsi assoluti. Se
#  quella home qui non esiste mmi non trova ne' pagine ne' librerie, e lghmi
#  lascia spento il pulsante "mmi". Si sostituisce quel prefisso con la home
#  corrente: testo in pochi file, niente da ricompilare. La home d'origine non
#  si indovina, si legge dalla riga "*simulator: <home>/sked/<simulatore>".
#  E' lo stesso lavoro che fa lgdock (allinea_demo_alla_home) nel container.
#
#  kMmi.cfg: ogni riga porta al quinto campo l'utente di chi l'ha scritta, e
#  kMmi mostra solo le righe dell'utente corrente. Si riscrive l'utente.
#
#   $1 = directory della demo
allinea_demo_alla_home() {
    local demo="$1" sim ctx vecchio_sim vecchia nuova="$HOME" f riga utente
    utente="$(id -un)"
    for sim in "$demo"/sked/*/; do
        sim="${sim%/}"
        ctx="$sim/globpages/Context.ctx"
        [ -f "$ctx" ] || continue
        vecchio_sim="$(sed -n 's/^\*simulator:[[:space:]]*//p' "$ctx" | head -1 | tr -d '\r' | sed 's/[[:space:]]*$//')"
        case "$vecchio_sim" in
            /*/sked/"$(basename "$sim")") ;;
            *) continue ;;      # non e' <home>/sked/<questo simulatore>: non si tocca
        esac
        vecchia="${vecchio_sim%/sked/*}"
        [ "$vecchia" != "$nuova" ] || continue
        # il prefisso finisce in un'espressione di sed: se contiene altro che
        # caratteri da nome di directory si lascia perdere
        case "$vecchia$nuova" in *[!A-Za-z0-9_./-]*) continue ;; esac
        while IFS= read -r f; do
            if grep -q "$vecchia/" "$f" 2>/dev/null; then
                sed -i "s|$vecchia/|$nuova/|g" "$f" || true
            fi
        done < <(find "$sim" -maxdepth 2 -type f \( -name Context.ctx -o -name ApplDb.res \))
        if [ -f "$sim/kMmi.cfg" ]; then
            while IFS= read -r riga || [ -n "$riga" ]; do
                if [[ "$riga" =~ ^([^\;]*\;[^\;]*\;[^\;]*\;[^\;]*\;)[^\;]*(.*)$ ]]; then
                    printf '%s\n' "${BASH_REMATCH[1]}$utente${BASH_REMATCH[2]}"
                else
                    printf '%s\n' "$riga"
                fi
            done < "$sim/kMmi.cfg" > "$sim/kMmi.cfg.lgdemo" && cat "$sim/kMmi.cfg.lgdemo" > "$sim/kMmi.cfg"
            rm -f "$sim/kMmi.cfg.lgdemo"
        fi
        echo "  pagine MMI di $(basename "$sim") allineate a questa home ($vecchia -> $nuova)"
    done
    return 0
}
#  LC_ALL=C: quei file non sono UTF-8, e vanno trattati byte per byte.
( export LC_ALL=C; allinea_demo_alla_home "$PROVV/$DIR_DEMO" )

# --- 5. la metto al suo posto -------------------------------------------------

if [ -e "$DEST" ] || [ -L "$DEST" ]; then
    DA_PARTE="$HOME/old.$DIR_DEMO.$(date +%Y%m%d-%H%M%S)"
    n=2
    while [ -e "$DA_PARTE" ]; do
        DA_PARTE="$HOME/old.$DIR_DEMO.$(date +%Y%m%d-%H%M%S)_$n"; n=$((n + 1))
    done
    mv "$DEST" "$DA_PARTE" || muori "non riesco a mettere da parte $DEST: non ho toccato niente."
    echo "Quella che c'era e' stata messa da parte: $DA_PARTE"
    if ! mv "$PROVV/$DIR_DEMO" "$DEST"; then
        mv "$DA_PARTE" "$DEST" 2>/dev/null
        muori "non riesco a mettere la demo nuova in $DEST: ho rimesso al suo posto quella che c'era."
    fi
else
    mv "$PROVV/$DIR_DEMO" "$DEST" || muori "non riesco a creare $DEST."
fi

echo ""
echo -e "${GREEN}Demo '$NOME' installata in $DEST${NC} ($(du -sh "$DEST" 2>/dev/null | cut -f1))"
echo "  modelli:     $(ls "$DEST/legocad" 2>/dev/null | wc -l) voci in legocad/"
echo "  simulatori:  $(ls "$DEST/sked" 2>/dev/null | tr '\n' ' ')"
echo ""
if [ -L "$HOME/legocad" ] && [ "$(readlink -f "$HOME/legocad")" = "$(readlink -f "$DEST/legocad")" ]; then
    echo "~/legocad e ~/sked puntano gia' a questa area: e' pronta."
elif [ -n "$FILE_ESITO" ]; then
    echo "Il programma che ha lanciato lgdemo passa ora a questa area."
else
    echo "Per lavorarci, punta ~/legocad e ~/sked a questa area:"
    echo "    cd ~ ; lgswitch $DIR_DEMO"
fi
[ -n "${DA_PARTE:-}" ] && echo "Quando sei sicuro di non volerla piu':  rm -rf $DA_PARTE"
#  Per ultimo, a cose fatte: chi aspetta questo file lo prende per "installata".
[ -n "$FILE_ESITO" ] && { printf '%s\n' "$DIR_DEMO" > "$FILE_ESITO" || true; }
exit 0
