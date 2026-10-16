#!/bin/bash
#
# lgdemo - downloads a LegoPST demo and installs it among the work areas.
#
# The demos are ready-made work areas (a legopst_<name> directory with
# legocad and sked inside), published as legopst_<name>.tgz assets of the
# "Demo LegoPST" release of the LegoPST GitHub repository. They are packaged
# and published by demo/publish_demo.sh; "lgrun -d <name>" installs the same
# demos inside the container, this command does it on a native installation.
#
#   usage:  lgdemo [options] [NAME]
#
#     lgdemo            lists the published demos and asks which to install
#     lgdemo NAME       installs demo NAME in <base>/legopst_NAME
#                       (legopst_NAME or legopst_NAME.tgz work too)
#
#     -l, --list        lists the published demos and exits, asking nothing
#     -y, --yes         no confirmation if the demo is already installed
#         --result FILE on success, writes the area name (legopst_NAME) into
#                       FILE. For programs that run lgdemo: lghmi uses it to
#                       switch to the demo. --esito is the same option
#         --base DIR    the work-area directory to install into. Without it,
#                       it is derived from the links (see WHERE IT INSTALLS)
#         --machine     output for a program (lghmi), see MACHINE MODE
#     -h, --help        this help
#
# WHERE IT INSTALLS
#
#   In the WORK-AREA directory, the one holding the legopst_* areas and the
#   legocad and sked links that lgswitch moves. On a native installation it
#   is the home. Not in the lgrun container: the container home is ephemeral
#   (it disappears on exit) and the areas live in the host home, mounted as
#   /host_home, reached through ~/legocad -> /host_home/legocad ->
#   legopst_<area>/legocad. Installing in $HOME meant losing the demo on exit,
#   and lghmi could not find it among the areas.
#
#   Without --base it is derived like this: follow the links starting from
#   ~/legocad up to the LAST link (the one pointing to the real directory);
#   the base is the directory that contains it. It is lghmi's rule
#   (base_aree), which passes it with --base. If ~/legocad is not a link, the
#   base is the home.
#
# THE DESCRIPTION
#
#   Each demo may have a one-line description: the label of the release
#   asset, which publish_demo.sh writes from the DESCRIZIONE= line of
#   demo_<name>.conf. lgdemo shows it in the list.
#
# MACHINE MODE (--machine)
#
#   For programs that run lgdemo and show its progress (lghmi's Install demo
#   window). No colours and no questions: the demo must be given by name,
#   and if it is already installed -y is needed (the program asks for the
#   confirmation). On stdout, besides the usual messages, lines starting
#   with "LGDEMO ":
#
#     with --list, one line per demo:
#       LGDEMO DEMO <name>|<bytes>|<sha256>|<state>|<description>
#       state: current (the area in use), installed, absent
#     during the installation:
#       LGDEMO BASE <dir>                 where it installs
#       LGDEMO PHASE <phase>              download, check, extract, align, install
#       LGDEMO PROGRESS <received> <total>   twice a second; total 0 when
#                                         unknown
#       LGDEMO DONE <legopst_name> <directory>   on success
#       LGDEMO ERROR <reason>             before exiting with an error
#
# IF THE DEMO IS ALREADY THERE
#
#   lgdemo says so and asks for confirmation. If confirmed, the existing
#   directory is NOT deleted nor overwritten: it is set aside as
#   old.legopst_<name>.<date>-<time>, and the new demo takes its place. It
#   may contain someone's work; deleting it is the user's decision, not this
#   script's. The name deliberately does not start with "legopst_": that is
#   the pattern lgswitch uses to find areas, and a copy set aside must not
#   show up in its menu.
#
#   The new demo is downloaded, checked and extracted into a temporary
#   directory BEFORE touching the existing one: if anything goes wrong
#   (network, sha256, bad package) the home stays as it was.
#
# AFTER THE INSTALLATION
#
#   lgdemo does not touch the legocad and sked links. To work on the demo:
#       cd <base> ; lgswitch legopst_<name>
#
# ENVIRONMENT VARIABLES
#
#   LG_DEMO_RELEASE   tag of the release to take the demos from (default:
#                     demo-2.0, the same as lgrun)
#   LG_DEMO_REPO      GitHub repository, as owner/name (default:
#                     RSE-TGM/LegoPST)
#   GITHUB_TOKEN      optional: GitHub allows 60 anonymous queries an hour,
#                     and with a token the limit goes away
#   LG_DEMO_API       for a mirror, or for tests: the address to read the
#                     release from (default: the GitHub API)
#   LG_DEMO_DOWNLOAD  likewise: the address holding the legopst_<name>.tgz
#
# (The comments in the code below are in Italian, like the rest of LegoPST.)
set -o pipefail

PROGRAMMA="$(basename "$0")"
REPO_SLUG="${LG_DEMO_REPO:-RSE-TGM/LegoPST}"
DEMO_RELEASE="${LG_DEMO_RELEASE:-demo-2.0}"
API="${LG_DEMO_API:-https://api.github.com/repos/$REPO_SLUG/releases/tags/$DEMO_RELEASE}"
SCARICA_DA="${LG_DEMO_DOWNLOAD:-https://github.com/$REPO_SLUG/releases/download/$DEMO_RELEASE}"
MACCHINA=false
for a in "$@"; do [ "$a" = "--machine" ] && MACCHINA=true; done

if [ -t 1 ] && ! $MACCHINA; then
    #  Colori che si leggono su fondo chiaro come su fondo scuro: il terminale
    #  in cui lghmi apre lgdemo e' spesso un xterm bianco, e li' il giallo
    #  non si vede. Gli avvisi sono in magenta, non in giallo.
    GREEN='\033[0;32m' ; YELLOW='\033[0;35m' ; RED='\033[0;31m' ; NC='\033[0m'
else
    GREEN='' ; YELLOW='' ; RED='' ; NC=''
fi

#  Le righe per il programma che ci ha lanciato (MODO MACCHINA in testa).
proto() { $MACCHINA && echo "LGDEMO $*"; return 0; }
muori() {
    $MACCHINA && echo "LGDEMO ERROR $(printf '%s' "$*" | head -1)"
    echo -e "${RED}ERROR:${NC} $*" >&2
    exit 1
}
avviso() { echo -e "${YELLOW}WARNING:${NC} $*" >&2; }

SOLO_LISTA=false
SENZA_CONFERMA=false
NOME=""
FILE_ESITO=""
BASE=""

while [ $# -gt 0 ]; do
    case "$1" in
        -l|--list)  SOLO_LISTA=true ;;
        -y|--yes)   SENZA_CONFERMA=true ;;
        --esito|--result)  FILE_ESITO="${2:?$1 needs a file name}"; shift ;;
        --esito=*)  FILE_ESITO="${1#--esito=}" ;;
        --result=*) FILE_ESITO="${1#--result=}" ;;
        --base)     BASE="${2:?$1 needs a directory}"; shift ;;
        --base=*)   BASE="${1#--base=}" ;;
        --machine)  ;;      # gia' letta sopra: serve prima dei colori
        -h|--help)  sed -n '3,/^[^#]/p' "$0" | sed -n 's/^# \{0,1\}//p'; exit 0 ;;
        -*)         muori "unknown option: $1 (see -h)" ;;
        *)          [ -z "$NOME" ] || muori "one demo at a time (see -h)"
                    NOME="$1" ;;
    esac
    shift
done

command -v curl >/dev/null 2>&1 || muori "'curl' is needed to download the demos."
[ -n "${HOME:-}" ] && [ -d "$HOME" ] || muori "the home directory ('${HOME:-}') does not exist."

#  La directory delle aree di lavoro (vedi DOVE SI INSTALLA in testa).
base_aree() {
    local p="$HOME/legocad" t d i
    for i in 1 2 3 4 5 6 7 8; do
        [ -L "$p" ] || break
        t="$(readlink "$p")" || break
        case "$t" in /*) d="$t" ;; *) d="$(dirname "$p")/$t" ;; esac
        [ -L "$d" ] || break
        p="$d"
    done
    (cd "$(dirname "$p")" 2>/dev/null && pwd) || echo "$HOME"
}
[ -n "$BASE" ] || BASE="$(base_aree)"
[ -d "$BASE" ] || muori "the work-area directory ('$BASE') does not exist."
BASE="$(cd "$BASE" && pwd)"
[ -w "$BASE" ] || muori "the work-area directory ('$BASE') is not writable."
#  Come la si mostra: "~" se e' la home, se no per intero
if [ "$BASE" = "$(cd "$HOME" && pwd)" ]; then MOSTRA_BASE="~"; else MOSTRA_BASE="$BASE"; fi

TMP=""          # file provvisori, fuori dalla base
PROVV=""        # directory provvisoria di estrazione, DENTRO la base
CURL_PID=""     # lo scaricamento in corso, nel modo macchina
pulisci() {
    [ -n "$CURL_PID" ] && kill "$CURL_PID" 2>/dev/null
    [ -n "$TMP" ] && rm -rf "$TMP"
    [ -n "$PROVV" ] && rm -rf "$PROVV"
}
trap pulisci EXIT
trap 'echo; proto ERROR "interrupted"; echo "$PROGRAMMA: interrupted." >&2; exit 130' INT TERM
TMP="$(mktemp -d "${TMPDIR:-/tmp}/lgdemo.XXXXXX")" || muori "cannot create a temporary directory."

#  Le demo pubblicate, una per riga:  <nome>|<byte>|<sha256>|<descrizione>
#
#  Si leggono dalla descrizione della release che da' GitHub. E' JSON, ma qui
#  non lo si interpreta con python o jq, che su una installazione ridotta
#  possono mancare: GitHub lo scrive una chiave per riga, e di ogni allegato
#  servono tre chiavi che arrivano sempre nello stesso ordine (name, size,
#  digest, e in fondo browser_download_url che chiude l'allegato).
#  Lo sha256 puo' mancare (allegati vecchi): in quel caso il campo resta vuoto.
#  La descrizione e' l'etichetta (label) dell'allegato, che viene subito dopo
#  il nome; senza etichetta GitHub scrive null, e il campo resta vuoto.
#
#  Ritorna 1 se la release non si riesce a leggere; il perche' lo dice su stderr.
ELENCO=""
leggi_elenco() {
    local http riga nome="" byte="" sha="" eti=""
    local -a intest=(-H "Accept: application/vnd.github+json")
    [ -n "${GITHUB_TOKEN:-}" ] && intest+=(-H "Authorization: Bearer $GITHUB_TOKEN")
    http="$(curl -sS --connect-timeout 15 --max-time 60 "${intest[@]}" \
                 -o "$TMP/rel.json" -w '%{http_code}' "$API" 2>"$TMP/curl.err")" || http="000"
    case "$http" in
        200) ;;
        404) echo "release '$DEMO_RELEASE' of $REPO_SLUG does not exist." >&2; return 1 ;;
        403|429) echo "GitHub refused the query (too many requests from this address: try again in an hour, or set GITHUB_TOKEN)." >&2; return 1 ;;
        000) echo "GitHub does not answer ($(head -1 "$TMP/curl.err" 2>/dev/null))." >&2; return 1 ;;
        *)   echo "GitHub answered $http reading release '$DEMO_RELEASE'." >&2; return 1 ;;
    esac
    ELENCO=""
    while IFS= read -r riga; do
        case "$riga" in
            *'"name": "legopst_'*'.tgz"'*)
                nome="${riga#*\"name\": \"legopst_}"; nome="${nome%%.tgz\"*}"; byte=""; sha=""; eti="" ;;
            *'"label": "'*)
                if [ -n "$nome" ]; then
                    eti="${riga#*\"label\": \"}"; eti="${eti%\"*}"
                    eti="${eti//\\\"/\"}"; eti="${eti//\\\\/\\}"; eti="${eti//|//}"
                fi ;;
            *'"size": '*)
                [ -n "$nome" ] && { byte="${riga#*\"size\": }"; byte="${byte%%[!0-9]*}"; } ;;
            *'"digest": "sha256:'*)
                [ -n "$nome" ] && { sha="${riga#*\"digest\": \"sha256:}"; sha="${sha%%\"*}"; } ;;
            *'"browser_download_url": '*)
                [ -n "$nome" ] && ELENCO+="$nome|$byte|$sha|$eti"$'\n'
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

#  Lo stato di una demo nella base: current, installed o absent.
stato_demo() {
    local d="$BASE/legopst_$1"
    if [ ! -e "$d" ]; then echo absent; return; fi
    if [ -L "$BASE/legocad" ] && [ "$(readlink -f "$BASE/legocad")" = "$(readlink -f "$d/legocad")" ]; then
        echo current
    else
        echo installed
    fi
}

#  Stampa l'elenco numerato. Accanto a ogni demo dice se e' gia' installata,
#  e sotto la sua descrizione.
mostra_elenco() {
    local n=0 nome byte sha eti nota
    echo "Published demos (release '$DEMO_RELEASE' of $REPO_SLUG):"
    echo ""
    while IFS='|' read -r nome byte sha eti; do
        [ -n "$nome" ] || continue
        n=$((n + 1))
        nota=""
        [ -e "$BASE/legopst_$nome" ] && nota="${YELLOW}already installed in $MOSTRA_BASE/legopst_$nome${NC}"
        printf '  %2d) %-20s %6s   ' "$n" "$nome" "$(dimensione "$byte")"
        echo -e "$nota"
        [ -n "$eti" ] && printf '      %s\n' "$eti"
    done <<< "$ELENCO"
    echo ""
}

# --- 1. quale demo ------------------------------------------------------------

ELENCO_LETTO=false
if leggi_elenco 2>"$TMP/elenco.err"; then
    ELENCO_LETTO=true
fi

if [ -z "$NOME" ] || $SOLO_LISTA; then
    $ELENCO_LETTO || muori "cannot read the list of demos: $(cat "$TMP/elenco.err")"
    [ -n "$ELENCO" ] || muori "release '$DEMO_RELEASE' has no legopst_<name>.tgz asset."
    if $MACCHINA; then
        $SOLO_LISTA || muori "in machine mode the demo must be given by name."
        proto BASE "$BASE"
        while IFS='|' read -r nome byte sha eti; do
            [ -n "$nome" ] && proto DEMO "$nome|$byte|$sha|$(stato_demo "$nome")|$eti"
        done <<< "$ELENCO"
        exit 0
    fi
    mostra_elenco
    $SOLO_LISTA && exit 0
    [ -t 0 ] || muori "no demo given and no terminal to ask on: use '$PROGRAMMA NAME'."
    QUANTE="$(printf '%s\n' "$ELENCO" | grep -c .)"
    while :; do
        read -r -p "Which one should I install? [1-$QUANTE, or the name; Enter = none]: " SCELTA || { echo; exit 0; }
        [ -n "$SCELTA" ] || { echo "Nothing to do."; exit 0; }
        if [[ "$SCELTA" =~ ^[0-9]+$ ]]; then
            if [ "$SCELTA" -ge 1 ] && [ "$SCELTA" -le "$QUANTE" ]; then
                NOME="$(printf '%s\n' "$ELENCO" | sed -n "${SCELTA}p" | cut -d'|' -f1)"
                break
            fi
        elif [[ "$SCELTA" =~ ^[A-Za-z0-9_-]+$ ]] && printf '%s\n' "$ELENCO" | grep -q "^$SCELTA|"; then
            NOME="$SCELTA"
            break
        fi
        echo "Invalid choice: '$SCELTA'."
    done
fi

#  Si accetta anche il nome della directory o quello del pacchetto.
NOME="${NOME%.tgz}"
NOME="${NOME#legopst_}"
[[ "$NOME" =~ ^[A-Za-z0-9_][A-Za-z0-9_-]*$ ]] || muori "invalid demo name: '$NOME' (letters, digits, _ and -)."

DIR_DEMO="legopst_$NOME"
DEST="$BASE/$DIR_DEMO"
URL="$SCARICA_DA/$DIR_DEMO.tgz"
BYTE="" ; SHA=""

if $ELENCO_LETTO; then
    RIGA="$(printf '%s\n' "$ELENCO" | grep "^$NOME|" || true)"
    if [ -z "$RIGA" ]; then
        proto ERROR "demo '$NOME' is not in release '$DEMO_RELEASE'."
        echo -e "${RED}ERROR:${NC} demo '$NOME' is not in release '$DEMO_RELEASE'." >&2
        [ -n "$ELENCO" ] && { echo "" >&2; mostra_elenco >&2; }
        exit 1
    fi
    BYTE="$(echo "$RIGA" | cut -d'|' -f2)"
    SHA="$(echo "$RIGA" | cut -d'|' -f3)"
else
    #  L'elenco non si legge (di solito: troppe interrogazioni), ma il
    #  pacchetto si scarica da un altro indirizzo, che non ha quel limite.
    avviso "cannot read the list of demos: $(cat "$TMP/elenco.err")"
    echo "         Trying to download $DIR_DEMO.tgz anyway, without checking its sha256." >&2
fi

# --- 2. c'e' gia'? ------------------------------------------------------------

if [ -e "$DEST" ] || [ -L "$DEST" ]; then
    echo -e "${YELLOW}Demo '$NOME' is already installed:${NC} $DEST"
    if [ -d "$DEST" ]; then
        echo "  size $(du -sh "$DEST" 2>/dev/null | cut -f1), last modified $(date -r "$DEST" '+%Y-%m-%d %H:%M' 2>/dev/null)"
    fi
    for l in legocad sked; do
        if [ -L "$BASE/$l" ] && [ "$(readlink "$BASE/$l")" = "$DIR_DEMO/$l" -o "$(readlink "$BASE/$l")" = "$DEST/$l" ]; then
            echo "  $MOSTRA_BASE/$l points there: it is the work area in use."
        fi
    done
    echo "  If I install it again, the existing one is set aside (not deleted)"
    echo "  as $MOSTRA_BASE/old.$DIR_DEMO.<date>-<time>, and the downloaded demo takes its place."
    if ! $SENZA_CONFERMA; then
        [ -t 0 ] || muori "the demo is already there and there is no terminal to ask for confirmation: use -y to install it anyway."
        read -r -p "Install it anyway? [y/N]: " RISPOSTA || { echo; exit 0; }
        case "$RISPOSTA" in
            s|S|si|SI|Si|y|Y|yes) ;;
            *) echo "Left as it is."; exit 0 ;;
        esac
    fi
fi

# --- 3. scarico e controllo ---------------------------------------------------

TGZ="$TMP/$DIR_DEMO.tgz"
proto BASE "$BASE"
proto PHASE download
echo "Downloading $DIR_DEMO.tgz$([ -n "$BYTE" ] && echo " ($(dimensione "$BYTE"))")"
echo "  $URL"
if $MACCHINA; then
    #  Nessuna barra di curl: l'avanzamento lo si misura dalla dimensione del
    #  file, e lo si dice con le righe PROGRESS.
    curl -fL -sS --retry 3 --connect-timeout 15 -o "$TGZ" "$URL" 2>"$TMP/curl.err" &
    CURL_PID=$!
    while kill -0 "$CURL_PID" 2>/dev/null; do
        proto PROGRESS "$(stat -c%s "$TGZ" 2>/dev/null || echo 0)" "${BYTE:-0}"
        sleep 0.5
    done
    wait "$CURL_PID"; esito_curl=$?
    CURL_PID=""
    [ "$esito_curl" -eq 0 ] || muori "cannot download demo '$NOME' ($URL): $(head -1 "$TMP/curl.err" 2>/dev/null)"
    proto PROGRESS "$(stat -c%s "$TGZ")" "${BYTE:-0}"
else
    BARRA="-sS" ; [ -t 2 ] && BARRA="-#"
    curl -fL $BARRA --retry 3 --connect-timeout 15 -o "$TGZ" "$URL" \
        || muori "cannot download demo '$NOME' ($URL)."
fi
proto PHASE check

if [ -n "$SHA" ] && command -v sha256sum >/dev/null 2>&1; then
    SHA_SCARICATO="$(sha256sum "$TGZ" | cut -d' ' -f1)"
    [ "$SHA_SCARICATO" = "$SHA" ] || muori "the sha256 of the downloaded package is not the published one.
        published:  $SHA
        downloaded: $SHA_SCARICATO
        The file arrived damaged, or was replaced while downloading: try again."
    echo "  sha256 correct"
elif $ELENCO_LETTO; then
    avviso "sha256 not checked (GitHub does not publish it for this asset, or sha256sum is missing)."
fi

# --- 4. estraggo in una directory provvisoria ---------------------------------

#  Nella base, non in /tmp: cosi' metterla al suo posto e' una rinomina sullo
#  stesso disco, che o riesce tutta o non riesce.
PROVV="$(mktemp -d "$BASE/.lgdemo.XXXXXX")" || muori "cannot create a temporary directory in $BASE."
proto PHASE extract
echo "Extracting..."
#  --no-same-owner: i file sono di chi installa, non di chi ha confezionato.
tar --no-same-owner -xzf "$TGZ" -C "$PROVV" || muori "the package does not extract (disk full? damaged file?)."

#  Il pacchetto deve contenere proprio legopst_<nome>, con legocad e sked: e'
#  un'area di lavoro, e senza quelle due directory lgswitch non la userebbe.
if [ ! -d "$PROVV/$DIR_DEMO/legocad" ] || [ ! -d "$PROVV/$DIR_DEMO/sked" ]; then
    muori "the package does not contain $DIR_DEMO/legocad and $DIR_DEMO/sked.
        It must be rebuilt with demo/make_demo_tgz.sh -d $NOME."
fi
ALTRO="$(ls -A "$PROVV" | grep -v -x "$DIR_DEMO" || true)"
[ -z "$ALTRO" ] || avviso "the package also contains other things, which I do not install: $(echo $ALTRO)"

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
#  La home nuova e' $HOME anche quando la base e' un'altra (nel container,
#  /host_home): kGlobContext scrive $HOME/sked/..., e $HOME/sked porta all'area
#  corrente attraverso i link, li' come in una installazione nativa.
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
        echo "  MMI pages of $(basename "$sim") aligned to this home ($vecchia -> $nuova)"
    done
    return 0
}
#  LC_ALL=C: quei file non sono UTF-8, e vanno trattati byte per byte.
proto PHASE align
( export LC_ALL=C; allinea_demo_alla_home "$PROVV/$DIR_DEMO" )
proto PHASE install

# --- 5. la metto al suo posto -------------------------------------------------

if [ -e "$DEST" ] || [ -L "$DEST" ]; then
    DA_PARTE="$BASE/old.$DIR_DEMO.$(date +%Y%m%d-%H%M%S)"
    n=2
    while [ -e "$DA_PARTE" ]; do
        DA_PARTE="$BASE/old.$DIR_DEMO.$(date +%Y%m%d-%H%M%S)_$n"; n=$((n + 1))
    done
    mv "$DEST" "$DA_PARTE" || muori "cannot set aside $DEST: nothing was touched."
    echo "The existing one was set aside: $DA_PARTE"
    if ! mv "$PROVV/$DIR_DEMO" "$DEST"; then
        mv "$DA_PARTE" "$DEST" 2>/dev/null
        muori "cannot put the new demo in $DEST: the existing one was put back."
    fi
else
    mv "$PROVV/$DIR_DEMO" "$DEST" || muori "cannot create $DEST."
fi

echo ""
echo -e "${GREEN}Demo '$NOME' installed in $DEST${NC} ($(du -sh "$DEST" 2>/dev/null | cut -f1))"
echo "  models:      $(ls "$DEST/legocad" 2>/dev/null | wc -l) entries in legocad/"
echo "  simulators:  $(ls "$DEST/sked" 2>/dev/null | tr '\n' ' ')"
echo ""
if [ -L "$BASE/legocad" ] && [ "$(readlink -f "$BASE/legocad")" = "$(readlink -f "$DEST/legocad")" ]; then
    echo "$MOSTRA_BASE/legocad and $MOSTRA_BASE/sked already point to this area: it is ready."
elif [ -n "$FILE_ESITO" ] || $MACCHINA; then
    echo "The program that ran lgdemo now switches to this area."
else
    echo "To work on it, point legocad and sked to this area:"
    echo "    cd $MOSTRA_BASE ; lgswitch $DIR_DEMO"
fi
[ -n "${DA_PARTE:-}" ] && echo "When you are sure you no longer need it:  rm -rf $DA_PARTE"
#  Per ultimo, a cose fatte: chi aspetta questo file lo prende per "installata".
[ -n "$FILE_ESITO" ] && { printf '%s\n' "$DIR_DEMO" > "$FILE_ESITO" || true; }
proto DONE "$DIR_DEMO" "$DEST"
exit 0
