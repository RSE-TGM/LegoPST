#!/usr/bin/env bash
#
# lgversion - la versione di LegoPST: vederla, controllarla, cambiarla.
#
# Uso:
#   lgversion                  la versione installata in $LEGOROOT (versione
#                              git, numero di build, data) e, in un checkout
#                              git, il rilascio (file VERSION), l'ultimo tag,
#                              l'immagine Docker e se tutti i riferimenti sono
#                              coerenti.
#   lgversion --define         stampa solo le tre righe "#define" nella forma
#                              di version.h: e' quello che leggono legopc,
#                              lghmi e l'indice della documentazione.
#   lgversion --check          solo il controllo di coerenza: esce con 1 se
#                              un file nomina un'immagine diversa da quella di
#                              VERSION, o se un lanciatore lgdock e' rimasto
#                              alla versione vecchia. Lo usa la CI.
#   lgversion AAAA-N.M [-y] [--docker]
#                              battezza una nuova versione, per esempio
#                              "lgversion 2027-2.1". Fa, nell'ordine:
#                              1. controlla che non ci siano modifiche non
#                                 committate e che il tag non esista gia';
#                              2. scrive VERSION e, se le cifre dopo il
#                                 trattino cambiano, il nome dell'immagine
#                                 Docker in tutti i file (da :2.0 a :2.1);
#                              3. rigenera i lanciatori lgdock (make in
#                                 docker/: il resto non cambia);
#                              4. commit e tag vAAAA-N.M (chiede conferma);
#                              5. costruisce e pubblica l'immagine Docker, se
#                                 il suo nome e' cambiato o con --docker
#                                 (chiede conferma). PRIMA del push: la CI di
#                                 GitHub gira dentro quell'immagine;
#                              6. push del branch e del tag (chiede conferma).
#                              A ogni "no" si ferma e stampa i comandi che
#                              restano da dare, nell'ordine.
#   -y                         risponde si' a tutte le conferme.
#   --docker                   costruisce e pubblica l'immagine anche se il
#                              suo nome non cambia (per aggiornarne il
#                              contenuto).
#   lgversion --docker [-y]    senza versione: non cambia niente, ricostruisce
#                              e ripubblica l'immagine della versione attuale
#                              (make -f Makefile.mk docker-push), dopo il
#                              controllo di coerenza e una conferma.
#
# Fino a novembre 2026 erano due comandi: lgversion (vedere) e lgversione
# (controllare e cambiare). Ora sono questo.
#
# Da dove viene la versione installata:
#  - se $LEGOROOT e' un checkout git, da git, calcolata adesso. Eseguibili e
#    librerie stanno in git (docs/BUILD.md, "Compiled files in git"): dopo un
#    "git pull" quelli in uso sono quelli del commit scaricato, senza
#    compilare niente. version.h invece e' escluso da git e si riscrive solo
#    con make: dopo un pull restava quello dell'ultima compilazione fatta in
#    quella copia, e due copie allineate mostravano versioni diverse.
#  - dove git non c'e' (immagine Docker, bundle FMU, tarball), da version.h,
#    scritto da chi ha costruito quella copia.
# Versione e numero sono calcolati come nella regola version.h di Makefile.mk:
# se si cambia il "git describe" la', va cambiato anche qui. La data invece e'
# quella dell'ultimo commit: la data del make non direbbe niente di una copia
# aggiornata con git pull.
#
# Il file VERSION non si modifica a mano: lo cambia "lgversion AAAA-N.M", che
# si lancia solo quando si decide una nuova versione (o "lgversion --docker"
# per rifare l'immagine). Nel lavoro normale non serve: version.h si aggiorna
# da solo a ogni make (v2026-2.0-25-g1a2b3c4d = 25 commit dopo la 2026-2.0), e
# il controllo --check lo fa la CI a ogni push.
#
# La versione si scrive AAAA-N.M: l'anno, poi il numero dell'immagine Docker
# (2026-2.0 va con l'immagine :2.0). Guida: docs/BUILD.md, "Versioni e
# rilasci".
#
set -u

case "${1:-}" in
    -h|--help) sed -n '3,/^[^#]/p' "$0" | sed -n 's/^# \{0,1\}//p'; exit 0 ;;
esac

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
IMMAGINE="aguagliardi/legopst"
#  documenti che raccontano la storia: i nomi vecchi li' sono giusti
STORIA=(':!docs/PIANO_INTERVENTO.md' ':!docs/COMPONENTI_RIMOSSI.md')

# =============================================================================
#  La versione installata in $LEGOROOT (il vecchio lgversion)
# =============================================================================

#  Mette in DEFINES le tre righe "#define" e in FONTE da dove vengono.
#  $1 = 1 se chiamata per --define: allora niente make, e se version.h manca
#  esce con 1 (lghmi la chiama anche dove il repository non e' scrivibile).
leggi_installata() {
    local solo_define="$1" vfile="$LEGOROOT/version.h" gv gn gd
    DEFINES=""
    FONTE=""
    #  Solo se il .git e' proprio in $LEGOROOT: una copia senza .git dentro un
    #  altro repository non deve prendere la versione di quello.
    if [ -e "$LEGOROOT/.git" ] && command -v git >/dev/null 2>&1; then
        gv=$(git -C "$LEGOROOT" describe --tags --match 'v[0-9]*' --always --long --dirty 2>/dev/null)
        gn=$(git -C "$LEGOROOT" rev-list --count HEAD 2>/dev/null)
        gd=$(git -C "$LEGOROOT" log -1 --format=%cd --date=format:%Y%m%d 2>/dev/null)
        if [ -n "$gv" ] && [ -n "$gn" ] && [ -n "$gd" ]; then
            DEFINES="#define GIT_VERSION_STRING \"$gv\"
#define BUILD_NUMBER $gn
#define BUILD_DATE_STRING \"$gd\""
            FONTE="git (sorgenti in \$LEGOROOT)"
            return 0
        fi
    fi
    #  Ripiego: version.h
    if [ "$solo_define" = 1 ] && [ ! -f "$vfile" ]; then
        exit 1
    fi
    if [ ! -f "$vfile" ]; then
        echo "Info: Il file di versione '$vfile' non è stato trovato." >&2
        echo "Info: Tento di generarlo ora eseguendo 'make'..." >&2
        if ! make -s -C "$LEGOROOT" -f Makefile.mk version.h >&2; then
            echo "Errore: Impossibile generare '$vfile'." >&2
            echo "Assicurati che 'git' e 'make' siano installati e che '$LEGOROOT' sia la directory corretta." >&2
            exit 1
        fi
        echo "Info: File '$vfile' generato con successo." >&2
    fi
    if [ ! -f "$vfile" ]; then
        echo "Errore: Non è stato possibile trovare o creare '$vfile'." >&2
        exit 1
    fi
    DEFINES=$(grep '^#define ' "$vfile")
    FONTE="$vfile (git non disponibile)"
}

richiedi_legoroot() {
    if [ -z "${LEGOROOT:-}" ]; then
        echo "Errore: La variabile d'ambiente LEGOROOT non è impostata." >&2
        echo "Impostala alla directory radice del progetto prima di eseguire questo script." >&2
        exit 1
    fi
}

# =============================================================================
#  Rilasci: VERSION, tag, immagine Docker (il vecchio lgversione)
# =============================================================================

leggi_versione() { tr -d '[:space:]' < "$R/VERSION" 2>/dev/null; }
valida() { [[ "$1" =~ ^[0-9]{4}-[0-9]+\.[0-9]+$ ]]; }
tag_immagine() { echo "${1#*-}"; }

ultimo_tag() {
    git -C "$R" describe --tags --match 'v[0-9]*' --abbrev=0 2>/dev/null || echo "(nessuno)"
}

#  Il controllo di coerenza, sul repository $R. Stampa i problemi, torna 1 se
#  ce ne sono.
controlla() {
    local ver tag errori=0 riga file token t
    ver="$(leggi_versione)"
    if ! valida "$ver"; then
        echo "VERSION: \"$ver\" non e' nella forma AAAA-N.M (es. 2026-2.0)"
        return 1
    fi
    tag="$(tag_immagine "$ver")"
    #  ogni "aguagliardi/legopst:N.M" nei file in git; quelli con un suffisso
    #  (":2.1-prova", gli esempi di LG_DOCKER_IMAGE) non sono l'immagine vera
    while IFS= read -r riga; do
        file="${riga%%:*}"
        token="${riga#*:}"
        t="${token#*legopst:}"
        t="${t%.}"                 # il punto di fine frase non e' del nome
        [[ "$t" == *-* ]] && continue
        if [ "$t" != "$tag" ]; then
            echo "  $file: nomina $IMMAGINE:$t, VERSION dice :$tag"
            errori=1
        fi
    done < <(cd "$R" && git grep -oIE "$IMMAGINE:[0-9]+\.[0-9]+[A-Za-z0-9._-]*" -- . "${STORIA[@]}" 2>/dev/null)
    for file in Alg_rt/bin/lgdock Alg_rt/bin/lgdock_multi Alg_rt/bin/lgdock_socat; do
        [ -f "$R/$file" ] || continue
        t="$(sed -n 's/^VERSION="\{0,1\}\([^"]*\)"\{0,1\}$/\1/p' "$R/$file" | head -1)"
        if [ "$t" != "$ver" ]; then
            echo "  $file: VERSION=\"$t\", il file VERSION dice $ver (manca un make?)"
            errori=1
        fi
    done
    return $errori
}

#  Il repository su cui si lavora per --check e per i rilasci: quello che
#  contiene questo script (come faceva lgversione).
repo_dello_script() {
    R="$(git -C "$SCRIPT_DIR" rev-parse --show-toplevel 2>/dev/null)" || {
        echo "lgversion: $SCRIPT_DIR non e' dentro un repository git di LegoPST." >&2
        exit 2
    }
    cd "$R" || exit 2
}

chiedi() {
    local r
    if [ "$SI" = 1 ]; then
        echo "$1 [s/N] s (-y)"
        return 0
    fi
    read -r -p "$1 [s/N] " r
    [[ "$r" =~ ^[sSyY] ]]
}

#  I comandi che restano, quando ci si ferma a meta'.
resta() {
    echo ""
    echo "Fermato qui. Per finire, nell'ordine:"
    for c in "$@"; do echo "    $c"; done
    exit 0
}

# =============================================================================
#  --define
# =============================================================================
if [ "${1:-}" = "--define" ]; then
    [ $# -eq 1 ] || { echo "Uso: lgversion --define" >&2; exit 2; }
    richiedi_legoroot
    leggi_installata 1
    echo "$DEFINES"
    exit 0
fi

# =============================================================================
#  Senza argomenti: la versione installata, e il rilascio se c'e' git
# =============================================================================
if [ $# -eq 0 ]; then
    richiedi_legoroot
    leggi_installata 0
    echo "------- Ambiente LegoPST---------"
    echo "LEGOROOT: $LEGOROOT"
    echo "Fonte: $FONTE"
    echo "--------------------------------"
    echo "$DEFINES" | awk '
      /^#define / {
        nome_macro = $2
        valore = ""
        for (i = 3; i <= NF; i++) {
          valore = valore (i == 3 ? "" : " ") $i
        }
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
    #  Il rilascio: il file VERSION c'e' anche senza git (nell'immagine
    #  Docker); tag e controllo di coerenza solo in un checkout.
    R="$LEGOROOT"
    VER="$(leggi_versione)"
    if [ -n "$VER" ]; then
        printf "%-20s: %s\n" "Rilascio (VERSION)" "$VER"
        valida "$VER" && printf "%-20s: %s\n" "Immagine Docker" "$IMMAGINE:$(tag_immagine "$VER")"
        if [ -e "$LEGOROOT/.git" ] && command -v git >/dev/null 2>&1; then
            printf "%-20s: %s\n" "Ultimo tag" "$(ultimo_tag)"
            if out="$(controlla)"; then
                printf "%-20s: %s\n" "Riferimenti" "coerenti con VERSION"
            else
                printf "%-20s: %s\n" "Riferimenti" "NON coerenti con VERSION:"
                echo "$out"
                echo "(lgversion AAAA-N.M li riallinea; un lanciatore vecchio si rigenera con make)"
            fi
        fi
        echo "--------------------------------"
    fi
    exit 0
fi

# =============================================================================
#  --check
# =============================================================================
if [ "$1" = "--check" ]; then
    repo_dello_script
    if out="$(controlla)"; then
        echo "Riferimenti: coerenti con VERSION."
        exit 0
    fi
    echo "Riferimenti NON coerenti con VERSION:"
    echo "$out"
    echo "(lgversion AAAA-N.M li riallinea; un lanciatore vecchio si rigenera con make)"
    exit 1
fi

# =============================================================================
#  Nuova versione, o --docker
# =============================================================================
repo_dello_script
NUOVA=""; SI=0; DOCKER=0
for a in "$@"; do
    case "$a" in
        -y) SI=1 ;;
        --docker) DOCKER=1 ;;
        -*) echo "lgversion: opzione sconosciuta $a (lgversion -h)" >&2; exit 2 ;;
        *) NUOVA="$a" ;;
    esac
done
#  --docker senza versione: solo l'immagine, della versione attuale
if [ -z "$NUOVA" ] && [ "$DOCKER" = 1 ]; then
    VER="$(leggi_versione)"
    if ! out="$(controlla)"; then
        echo "lgversion: i riferimenti non sono coerenti con VERSION, non costruisco:" >&2
        echo "$out" >&2
        exit 1
    fi
    if [ -z "${LEGOROOT:-}" ] || [ "$(cd "$LEGOROOT" 2>/dev/null && pwd -P)" != "$(pwd -P)" ]; then
        echo "lgversion: prima  source $R/.profile_legoroot" >&2
        exit 1
    fi
    echo "Versione:  $VER (non cambia)"
    echo "Immagine:  $IMMAGINE:$(tag_immagine "$VER"), costruita da questa copia:"
    echo "           $(git describe --tags --match 'v[0-9]*' --always --long --dirty 2>/dev/null)"
    echo "           e pubblicata su Docker Hub al posto di quella che c'e'."
    [ -n "$(git status --porcelain --untracked-files=no)" ] && \
        echo "ATTENZIONE: ci sono modifiche non committate, e finiscono nell'immagine."
    chiedi "Costruisco e pubblico l'immagine (alcuni minuti)?" || exit 0
    exec make -f Makefile.mk docker-push
fi
if ! valida "$NUOVA"; then
    if [ -z "$NUOVA" ]; then
        echo "lgversion: manca la versione (es. lgversion 2027-2.1); per rifare" >&2
        echo "           solo l'immagine Docker: lgversion --docker" >&2
    else
        echo "lgversion: \"$NUOVA\" non e' una versione AAAA-N.M (es. 2027-2.1)." >&2
    fi
    exit 2
fi
VECCHIA="$(leggi_versione)"
TV="$(tag_immagine "$VECCHIA")"
TN="$(tag_immagine "$NUOVA")"
BRANCH="$(git symbolic-ref --short HEAD 2>/dev/null || echo HEAD)"

if [ "$NUOVA" = "$VECCHIA" ]; then
    echo "lgversion: la versione e' gia' $NUOVA." >&2; exit 1
fi
if git rev-parse -q --verify "refs/tags/v$NUOVA" >/dev/null; then
    echo "lgversion: il tag v$NUOVA esiste gia'." >&2; exit 1
fi
if [ -n "$(git status --porcelain --untracked-files=no)" ]; then
    echo "lgversion: ci sono modifiche non committate. Il commit della versione" >&2
    echo "           deve contenere solo il cambio di versione: prima committa." >&2
    git status --short --untracked-files=no >&2
    exit 1
fi
#  make vuole il profilo di QUESTO albero (vedi controlla_profilo nel Makefile)
if [ -z "${LEGOROOT:-}" ] || [ "$(cd "$LEGOROOT" 2>/dev/null && pwd -P)" != "$(pwd -P)" ]; then
    echo "lgversion: prima  source $R/.profile_legoroot" >&2
    exit 1
fi
[ "$TV" != "$TN" ] && DOCKER=1

echo "Versione:  $VECCHIA  ->  $NUOVA   (tag v$NUOVA, branch $BRANCH)"
if [ "$TV" != "$TN" ]; then
    echo "Immagine:  $IMMAGINE:$TV  ->  $IMMAGINE:$TN"
else
    echo "Immagine:  $IMMAGINE:$TN (non cambia)"
fi
chiedi "Procedo?" || exit 0

# 2. VERSION e nome dell'immagine
echo "$NUOVA" > VERSION
if [ "$TV" != "$TN" ]; then
    TV_RE="${TV//./\\.}"
    mapfile -t FILES < <(git grep -lI "$IMMAGINE:$TV" -- . "${STORIA[@]}")
    for f in "${FILES[@]}"; do
        #  "...:2.0." a fine frase si', "...:2.0.1" o "...:2.0-prova" no
        sed -i -E "s#$IMMAGINE:$TV_RE(\\.?([^0-9A-Za-z._-]|\$))#$IMMAGINE:$TN\\1#g" "$f"
    done
    echo "Immagine rinominata in ${#FILES[@]} file."
fi

# 3. i lanciatori lgdock, con la versione e l'immagine nuove. Solo quelli: un
#    make completo qui ricompilerebbe (dopo un clean, o in una copia nuova)
#    binari che finirebbero nel commit della versione, che deve contenere solo
#    la versione. Il make completo lo fa docker-push, quando serve.
LOG="$(mktemp -t lgversion_make.XXXXXX)"
echo "make in docker/ (i lanciatori; log: $LOG)"
if ! (cd docker && make -f Makefile.mk all) > "$LOG" 2>&1; then
    echo "lgversion: il build e' fallito, vedi $LOG. Le modifiche sono rimaste" >&2
    echo "           sul disco (git diff); per tornare indietro: git checkout ." >&2
    exit 1
fi
if ! out="$(controlla)"; then
    echo "lgversion: dopo il build i riferimenti non sono coerenti:" >&2
    echo "$out" >&2
    exit 1
fi
git status --short --untracked-files=no

# 4. commit e tag
MSG="versione $NUOVA"
[ "$TV" != "$TN" ] && MSG="$MSG, immagine Docker $IMMAGINE:$TN"
RESTO=("git commit -a -m \"$MSG\""
       "git tag -a v$NUOVA -m \"LegoPST $NUOVA\""
       "make -f Makefile.mk version.h")
[ "$DOCKER" = 1 ] && RESTO+=("make -f Makefile.mk docker-push")
RESTO+=("git push origin $BRANCH v$NUOVA")
chiedi "Commit \"$MSG\" e tag v$NUOVA?" || resta "${RESTO[@]}"
git commit -a -q -m "$MSG" || exit 1
git tag -a "v$NUOVA" -m "LegoPST $NUOVA" || exit 1
make -f Makefile.mk version.h >/dev/null 2>&1
echo "Fatto: commit $(git rev-parse --short HEAD), tag v$NUOVA, version.h $(sed -n 's/.*GIT_VERSION_STRING "\(.*\)"/\1/p' version.h)"

# 5. immagine Docker, prima del push
if [ "$DOCKER" = 1 ]; then
    if chiedi "Costruisco e pubblico $IMMAGINE:$TN (alcuni minuti, ~2 GB da caricare)?"; then
        make -f Makefile.mk docker-push || {
            echo "lgversion: docker-push fallito. NON fare il push di git prima" >&2
            echo "           che l'immagine sia pubblicata: la CI non la troverebbe." >&2
            exit 1
        }
    else
        resta "make -f Makefile.mk docker-push" "git push origin $BRANCH v$NUOVA"
    fi
fi

# 6. push
chiedi "Push di $BRANCH e v$NUOVA su origin?" || resta "git push origin $BRANCH v$NUOVA"
git push origin "$BRANCH" "v$NUOVA"
