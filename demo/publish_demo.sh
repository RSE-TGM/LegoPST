#!/bin/bash
#
# publish_demo.sh - aggiorna la release "Demo LegoPST" su GitHub.
#
# Fa in un colpo solo tutta la procedura, che a mano non si ricorda mai:
#
#   1. confeziona il pacchetto         (demo/make_demo_tgz.sh)
#   2. crea la release, se non c'e'    (tag demo-2.0, titolo "Demo LegoPST 2.0")
#   3. ci carica legopst_<nome>.tgz    (sostituendo l'allegato precedente)
#   4. lo riscarica e ne confronta lo sha256 con quello locale
#   5. per la demo standard: aggiorna lo sha256 nel Dockerfile
#
# La release e' una sola e porta un allegato per demo. "lgrun -d" installa
# userstd, che viaggia anche dentro l'immagine Docker (per questo, e solo per
# lei, c'e' il passo 5); "lgrun -d <nome>" scarica legopst_<nome>.tgz da qui.
#
#   uso:  ./publish_demo.sh [opzioni]
#
#     -d, --demo NOME   quale demo pubblicare (default: userstd)
#     -n, --dry-run     dice cosa farebbe, senza confezionare ne' pubblicare
#     -y, --yes         non chiede conferma prima di pubblicare
#         --no-build    non rifa' il pacchetto: pubblica il .tgz che c'e' gia'
#     -t, --tag TAG     tag della release       (default: demo-2.0)
#         --title TXT   titolo della release    (default: Demo LegoPST 2.0)
#     -h, --help        questo aiuto
#
# COME SI AUTENTICA
#
#   Per scrivere su GitHub serve un'identita'. Nell'ordine:
#
#   a) "gh", il client di GitHub, se e' installato e ha fatto il login. E' la
#      strada comoda, e si prepara una volta sola:
#          sudo dnf install gh      (Arch: sudo pacman -S github-cli)
#          gh auth login
#
#   b) un token, se gh non c'e'. Lo si cerca in $GITHUB_TOKEN, in $GH_TOKEN, nel
#      file ~/.config/legopst/github_token; se non c'e' da nessuna parte lo si
#      chiede a terminale (non viene mostrato ne' salvato).
#      Il token si crea su github.com -> Settings -> Developer settings ->
#      Personal access tokens: "classic" con il permesso "repo", oppure
#      "fine-grained" sul repository con "Contents: Read and write".
#      Per non doverlo ridare ogni volta:
#          mkdir -p ~/.config/legopst
#          echo 'ghp_...' > ~/.config/legopst/github_token
#          chmod 600 ~/.config/legopst/github_token
#
# Il repository e' quello di "origin". Leggere la release non richiede niente:
# con -n si puo' provare lo script anche senza credenziali.

set -e
set -o pipefail

PROGRAMMA="$(basename "$0")"
QUI="$(cd "$(dirname "$0")" && pwd)"
RADICE_REPO="$(cd "$QUI/.." && pwd)"
DEMO="userstd"
DOCKERFILE="$RADICE_REPO/docker/Dockerfile_LegoPST"

TAG="demo-2.0"
TITOLO="Demo LegoPST 2.0"
PROVA=false
SENZA_CONFERMA=false
CONFEZIONA=true

while [ $# -gt 0 ]; do
    case "$1" in
        -n|--dry-run)  PROVA=true ;;
        -y|--yes)      SENZA_CONFERMA=true ;;
        --no-build)    CONFEZIONA=false ;;
        -d|--demo)     DEMO="${2:?$1 vuole il nome della demo}"; shift ;;
        --demo=*)      DEMO="${1#--demo=}" ;;
        -t|--tag)      TAG="${2:?$1 vuole il tag}"; shift ;;
        --title)       TITOLO="${2:?$1 vuole il titolo}"; shift ;;
        -h|--help)     sed -n '3,/^[^#]/p' "$0" | sed -n 's/^# \{0,1\}//p'; exit 0 ;;
        *) echo "$PROGRAMMA: opzione non riconosciuta: $1 (vedi -h)" >&2; exit 1 ;;
    esac
    shift
done

muori() { echo "ERRORE: $*" >&2; exit 1; }

case "$DEMO" in
    ""|-*|*[!A-Za-z0-9_-]*) muori "nome di demo non valido: '$DEMO' (lettere, cifre, _ e -)." ;;
esac
NOME_ASSET="legopst_$DEMO.tgz"
TGZ="$QUI/$NOME_ASSET"

command -v curl >/dev/null 2>&1    || muori "curl non trovato."
command -v python3 >/dev/null 2>&1 || muori "python3 non trovato (serve a leggere le risposte di GitHub)."

# ---------------------------------------------------------------------------
# Il repository: quello di origin, come proprietario/nome
# ---------------------------------------------------------------------------
URL_ORIGIN="$(git -C "$RADICE_REPO" remote get-url origin 2>/dev/null || true)"
SLUG="$(printf '%s' "$URL_ORIGIN" | sed -E 's#^(https?://[^/]+/|git@[^:]+:)##; s#\.git$##')"
case "$URL_ORIGIN" in
    *github.com*) ;;
    *) muori "origin non e' su github.com ($URL_ORIGIN): questo script pubblica solo li'." ;;
esac
[ -n "$SLUG" ] || muori "non riesco a ricavare il repository da origin ($URL_ORIGIN)."

API="https://api.github.com/repos/$SLUG"
URL_ASSET="https://github.com/$SLUG/releases/download/$TAG/$NOME_ASSET"

echo "Repository:  $SLUG"
echo "Release:     $TAG  (\"$TITOLO\")"
echo "Demo:        $DEMO"
echo "Pacchetto:   $TGZ"
$PROVA && echo "Modo:        PROVA (-n): non si confeziona e non si pubblica niente"
echo ""

# ---------------------------------------------------------------------------
# 1. il pacchetto
# ---------------------------------------------------------------------------
if $PROVA; then
    $CONFEZIONA && echo "[prova] rifarei il pacchetto con make_demo_tgz.sh"
elif $CONFEZIONA; then
    echo "--- 1. confeziono il pacchetto ---"
    "$QUI/make_demo_tgz.sh" -d "$DEMO"
    echo ""
fi
[ -f "$TGZ" ] || muori "$TGZ non c'e'. Lancia senza --no-build, o prima make_demo_tgz.sh -d $DEMO."
SHA_LOCALE="$(sha256sum "$TGZ" | cut -d' ' -f1)"
DIM="$(stat -c%s "$TGZ")"
echo "sha256:      $SHA_LOCALE"
echo "dimensione:  $((DIM / 1024 / 1024)) MB"
echo ""

# ---------------------------------------------------------------------------
# Lo stato della release (lettura: non serve autenticarsi)
# ---------------------------------------------------------------------------
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT
GIA_PUBBLICATO=false

# campo <file json> <espressione python su d>
campo() { python3 -c 'import sys,json
d=json.load(open(sys.argv[1]))
print(eval(sys.argv[2]))' "$1" "$2" 2>/dev/null; }

HTTP="$(curl -sS -o "$TMP/rel.json" -w '%{http_code}' "$API/releases/tags/$TAG" || true)"
case "$HTTP" in
    200) ESISTE=true
         ID_REL="$(campo "$TMP/rel.json" 'd["id"]')"
         ID_ASSET="$(python3 -c 'import sys,json
d=json.load(open(sys.argv[1]))
print(([a["id"] for a in d["assets"] if a["name"]==sys.argv[2]] or [""])[0])' "$TMP/rel.json" "$NOME_ASSET")"
         echo "La release $TAG esiste gia'$([ -n "$ID_ASSET" ] && echo ", con un allegato $NOME_ASSET che verra' sostituito" || echo ", senza allegato")." ;;
    404) ESISTE=false; ID_REL=""; ID_ASSET=""
         echo "La release $TAG non esiste ancora: verra' creata." ;;
    *)   muori "GitHub ha risposto $HTTP leggendo la release (rete? limite di richieste?)." ;;
esac

# Se quello pubblicato e' gia' questo pacchetto non c'e' niente da fare.
if [ -n "$ID_ASSET" ]; then
    SHA_REMOTO="$(python3 -c 'import sys,json
d=json.load(open(sys.argv[1]))
for a in d["assets"]:
    if a["name"]==sys.argv[2]:
        print((a.get("digest") or "").replace("sha256:",""))' "$TMP/rel.json" "$NOME_ASSET")"
    if [ -n "$SHA_REMOTO" ] && [ "$SHA_REMOTO" = "$SHA_LOCALE" ]; then
        echo ""
        echo "L'allegato pubblicato ha gia' questo sha256: non c'e' niente da caricare."
        GIA_PUBBLICATO=true
    fi
fi
echo ""

#  Se e' gia' pubblicato si salta tutta la parte che scrive su GitHub (e quindi
#  non serve nemmeno autenticarsi), ma il resto si fa lo stesso: il Dockerfile
#  puo' non essere ancora allineato a quello che sta nella release.
if ! $GIA_PUBBLICATO; then

# ---------------------------------------------------------------------------
# Con chi ci si presenta
# ---------------------------------------------------------------------------
USA_GH=false
TOKEN=""
if command -v gh >/dev/null 2>&1 && gh auth status >/dev/null 2>&1; then
    USA_GH=true
    echo "Autenticazione: gh ($(gh api user -q .login 2>/dev/null || echo '?'))"
else
    TOKEN="${GITHUB_TOKEN:-${GH_TOKEN:-}}"
    FILE_TOKEN="$HOME/.config/legopst/github_token"
    if [ -z "$TOKEN" ] && [ -r "$FILE_TOKEN" ]; then
        TOKEN="$(tr -d '[:space:]' < "$FILE_TOKEN")"
    fi
    if [ -n "$TOKEN" ]; then
        echo "Autenticazione: token"
    elif $PROVA; then
        echo "Autenticazione: nessuna trovata (ne' gh ne' un token). Per pubblicare"
        echo "                davvero ne servira' una: vedi $PROGRAMMA -h."
    elif [ -t 0 ]; then
        echo "Non trovo ne' gh autenticato ne' un token (vedi $PROGRAMMA -h)."
        printf "Token di GitHub (non viene mostrato ne' salvato): "
        read -r -s TOKEN; echo ""
        [ -n "$TOKEN" ] || muori "nessun token: non posso pubblicare."
    else
        muori "ne' gh autenticato ne' un token (GITHUB_TOKEN, o $FILE_TOKEN). Vedi $PROGRAMMA -h."
    fi
fi
echo ""

if $PROVA; then
    $ESISTE || echo "[prova] creerei la release $TAG (\"$TITOLO\")"
    [ -n "$ID_ASSET" ] && echo "[prova] cancellerei l'allegato attuale"
    echo "[prova] caricherei $NOME_ASSET ($((DIM / 1024 / 1024)) MB)"
    echo "[prova] lo riscaricherei da $URL_ASSET per confrontare lo sha256"
    if [ "$DEMO" = userstd ] && grep -q '^ARG DEMO_SHA256=' "$DOCKERFILE" 2>/dev/null; then
        echo "[prova] aggiornerei ARG DEMO_SHA256 in docker/Dockerfile_LegoPST"
    fi
    exit 0
fi

if ! $SENZA_CONFERMA; then
    [ -t 0 ] || muori "serve una conferma: rilancia da terminale, o con -y."
    printf "Pubblico %s sulla release %s di %s? [s/N] " "$NOME_ASSET" "$TAG" "$SLUG"
    read -r risposta
    case "$risposta" in s|S|si|SI|y|Y) ;; *) echo "Annullato."; exit 1 ;; esac
    echo ""
fi

#  Le note della release elencano le demo con il loro sha256. Si aggiorna la
#  riga di QUESTA demo e si lasciano le altre com'erano.
NOTE="$(python3 - "$TMP/rel.json" "$NOME_ASSET" "$SHA_LOCALE" "$DEMO" <<'PY'
import sys, json, re
rel, asset, sha, demo = sys.argv[1:5]
testa = ("Pacchetti demo di LegoPST, confezionati da demo/make_demo_tgz.sh.\n"
         "`lgrun -d` installa legopst_userstd, `lgrun -d <nome>` installa legopst_<nome>.\n")
righe = {}
try:
    corpo = json.load(open(rel)).get("body") or ""
    for m in re.finditer(r"^- (legopst_[A-Za-z0-9_-]+\.tgz)  sha256: ([0-9a-f]{64})", corpo, re.M):
        righe[m.group(1)] = m.group(2)
except Exception:
    pass
righe[asset] = sha
print(testa + "\n" + "\n".join("- %s  sha256: %s" % (k, righe[k]) for k in sorted(righe)))
PY
)"

# ---------------------------------------------------------------------------
# 2-3. release e allegato
# ---------------------------------------------------------------------------
echo "--- 2. pubblico ---"
if $USA_GH; then
    if $ESISTE; then
        gh release edit "$TAG" --repo "$SLUG" --title "$TITOLO" --notes "$NOTE" >/dev/null
    else
        gh release create "$TAG" --repo "$SLUG" --title "$TITOLO" --notes "$NOTE" >/dev/null
        echo "release $TAG creata"
    fi
    gh release upload "$TAG" "$TGZ" --repo "$SLUG" --clobber
else
    #  Il token viaggia in un file di intestazioni, non sulla riga di comando,
    #  dove chiunque sulla macchina lo leggerebbe con ps.
    INTEST="$TMP/h"
    ( umask 077; printf 'Authorization: Bearer %s\nAccept: application/vnd.github+json\nX-GitHub-Api-Version: 2022-11-28\n' "$TOKEN" > "$INTEST" )

    # chiama <metodo> <url> [opzioni curl...]: risposta in $TMP/r.json, stato in $STATO
    chiama() {
        local metodo="$1" url="$2"; shift 2
        STATO="$(curl -sS -X "$metodo" -H @"$INTEST" -o "$TMP/r.json" -w '%{http_code}' "$@" "$url")"
    }
    spiega() { python3 -c 'import sys,json
try: print(json.load(open(sys.argv[1])).get("message",""))
except Exception: pass' "$TMP/r.json"; }

    CORPO="$TMP/corpo.json"
    python3 -c 'import sys,json
json.dump({"tag_name":sys.argv[1],"name":sys.argv[2],"body":sys.argv[3]}, open(sys.argv[4],"w"))' \
        "$TAG" "$TITOLO" "$NOTE" "$CORPO"

    if $ESISTE; then
        chiama PATCH "$API/releases/$ID_REL" -H 'Content-Type: application/json' --data-binary @"$CORPO"
        [ "$STATO" = 200 ] || muori "aggiornamento della release fallito ($STATO): $(spiega)"
    else
        chiama POST "$API/releases" -H 'Content-Type: application/json' --data-binary @"$CORPO"
        [ "$STATO" = 201 ] || muori "creazione della release fallita ($STATO): $(spiega)
        (401/403: il token non ha il permesso di scrivere su $SLUG)"
        ID_REL="$(campo "$TMP/r.json" 'd["id"]')"
        echo "release $TAG creata"
    fi

    if [ -n "$ID_ASSET" ]; then
        chiama DELETE "$API/releases/assets/$ID_ASSET"
        [ "$STATO" = 204 ] || muori "non riesco a togliere l'allegato precedente ($STATO): $(spiega)"
    fi

    echo "carico $NOME_ASSET ($((DIM / 1024 / 1024)) MB)..."
    STATO="$(curl -sS -X POST -H @"$INTEST" -H 'Content-Type: application/gzip' \
                  --data-binary @"$TGZ" -o "$TMP/r.json" -w '%{http_code}' \
                  "https://uploads.github.com/repos/$SLUG/releases/$ID_REL/assets?name=$NOME_ASSET")"
    [ "$STATO" = 201 ] || muori "caricamento fallito ($STATO): $(spiega)"
fi
echo "pubblicato."
echo ""

# ---------------------------------------------------------------------------
# 4. quello che si scarica e' quello che si e' caricato?
# ---------------------------------------------------------------------------
echo "--- 3. verifico scaricandolo ---"
SHA_SCARICATO=""
for tentativo in 1 2 3 4 5; do
    if curl -fsSL --retry 2 -o "$TMP/scaricato.tgz" "$URL_ASSET"; then
        SHA_SCARICATO="$(sha256sum "$TMP/scaricato.tgz" | cut -d' ' -f1)"
        [ "$SHA_SCARICATO" = "$SHA_LOCALE" ] && break
    fi
    sleep 4    # la distribuzione dell'allegato puo' metterci qualche secondo
done
[ "$SHA_SCARICATO" = "$SHA_LOCALE" ] || muori "lo sha256 di quello che si scarica (${SHA_SCARICATO:-niente}) non e' quello locale.
        URL: $URL_ASSET"
echo "identico: $URL_ASSET"
echo ""

fi   # ! GIA_PUBBLICATO

# ---------------------------------------------------------------------------
# 5. il Dockerfile: solo per la demo standard, l'unica che sta nell'immagine
# ---------------------------------------------------------------------------
if $PROVA; then
    if [ "$DEMO" = userstd ] && grep -q '^ARG DEMO_SHA256=' "$DOCKERFILE" 2>/dev/null \
       && [ "$(sed -n 's/^ARG DEMO_SHA256=//p' "$DOCKERFILE")" != "$SHA_LOCALE" ]; then
        echo "[prova] scriverei tag e sha256 in docker/Dockerfile_LegoPST"
    else
        echo "[prova] niente da fare."
    fi
    exit 0
fi

n=1
echo "COSA RESTA DA FARE"
#  Il pacchetto non deve stare in git: viaggia nella release. Toglierlo
#  dall'indice lo lascia su disco, dove serve per rifarlo.
if git -C "$RADICE_REPO" ls-files --error-unmatch "demo/$NOME_ASSET" >/dev/null 2>&1; then
    echo "  $n. togliere il pacchetto da git (resta su disco):"
    echo "         git rm --cached demo/$NOME_ASSET"
    n=$((n + 1))
fi
if [ "$DEMO" != userstd ]; then
    echo "  $n. niente da ricostruire: \"lgrun -d $DEMO\" la scarica da questa release."
    echo "     Chi l'ha gia' installata la tiene: per avere la nuova deve prima"
    echo "     cancellare ~/legopst_$DEMO."
elif grep -q '^ARG DEMO_SHA256=' "$DOCKERFILE" 2>/dev/null; then
    VECCHIO="$(sed -n 's/^ARG DEMO_SHA256=//p' "$DOCKERFILE")"
    if [ "$VECCHIO" != "$SHA_LOCALE" ]; then
        sed -i "s/^ARG DEMO_SHA256=.*/ARG DEMO_SHA256=$SHA_LOCALE/" "$DOCKERFILE"
        sed -i "s/^ARG DEMO_RELEASE=.*/ARG DEMO_RELEASE=$TAG/" "$DOCKERFILE"
        echo "  (docker/Dockerfile_LegoPST aggiornato con il nuovo sha256)"
    fi
    echo "  $n. committare (docker/Dockerfile_LegoPST porta lo sha256 della demo nuova) e fare push"
    echo "  $((n + 1)). ricostruire e pubblicare l'immagine:  cd docker && ./BuildImage -y --push"
    echo "  $((n + 2)). sulle altre macchine:                 lgrun -update"
else
    echo "  $n. l'immagine Docker prende ancora la demo dalla copia nel repository:"
    echo "     il Dockerfile non ha le righe ARG DEMO_RELEASE / ARG DEMO_SHA256."
fi
