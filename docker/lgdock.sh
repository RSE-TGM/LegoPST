#!/bin/bash
#
# lgdock versione lgdock_unified.sh
#
# Lancia il container LegoPST ed apre un terminale bash
# Crea dinamicamente l'utente dell'host nel container
#
# Supporta due modalità di X11 forwarding:
# - Standard: X11 forwarding diretto (default)
# - Socat: Usa socat per creare socket bridge (utile per SSH X11 forwarding)
#

# =============================================================================
# Gestione Parametri
# =============================================================================
VERSION="1.1"

#  L'immagine da lanciare. Ce n'e' una sola (fino al 2026-09-27 erano due,
#  legopst_multi e legopst_slim, con l'opzione -l per scegliere la seconda);
#  con LG_DOCKER_IMAGE si punta a un'altra, per esempio una build di prova:
#      LG_DOCKER_IMAGE=aguagliardi/legopst:2.1-prova lgdock
IMAGE_NAME="${LG_DOCKER_IMAGE:-aguagliardi/legopst:2.0}"

# Coordinate del repository da cui questo lanciatore e' stato installato.
# Le stampiglia install_legopst_dock.sh al momento dell'installazione, come fa
# gia' con VERSION: qui restano i valori di default, che valgono solo se si usa
# questo file cosi' com'e' dal repository.
#
# Servono a "-update": l'installer va riscaricato DALLO STESSO branch da cui e'
# arrivata questa copia, altrimenti chi ha installato da un branch di prova si
# ritroverebbe su master senza che nulla glielo dica.
REPO_HOST="github.com"
REPO_SLUG="RSE-TGM/LegoPST"
REPO_BRANCH="master"

#  La release di GitHub che porta i pacchetti delle demo, uno per demo:
#  legopst_<nome>.tgz. Li pubblica demo/publish_demo.sh. Quella standard
#  (userstd) sta anche dentro l'immagine; le altre si scaricano da qui.
DEMO_RELEASE="${LG_DEMO_RELEASE:-demo-2.0}"

url_demo() {    # $1 = nome della demo
    echo "https://github.com/${REPO_SLUG}/releases/download/${DEMO_RELEASE}/legopst_$1.tgz"
}

url_installer() {
    if [ "$REPO_HOST" = "github.com" ]; then
        echo "https://raw.githubusercontent.com/${REPO_SLUG}/${REPO_BRANCH}/docker/install_legopst_dock.sh"
    else
        echo "https://${REPO_HOST}/${REPO_SLUG}/-/raw/${REPO_BRANCH}/docker/install_legopst_dock.sh"
    fi
}

#  update e uninstall RILANCIANO L'INSTALLER, invece di rifare qui quello che
#  lui sa gia' fare. Cosi' installazione e aggiornamento restano un solo
#  percorso di codice, che non puo' divergere.
#
#  exec, e non una semplice chiamata: l'installer riscrive questo stesso file
#  (curl -o, che TRONCA lo stesso inode invece di scrivere altrove e
#  rinominare). Bash legge uno script a pezzi tenendo aperto il descrittore e
#  riposizionandosi dopo ogni comando: se il contenuto cambia sotto, prosegue
#  al vecchio offset dentro il NUOVO testo ed esegue spazzatura. Con exec il
#  processo viene sostituito e il descrittore chiuso PRIMA che l'installer
#  scriva, quindi non c'e' piu' nessuno a cui segare il ramo.
#  NON sostituire questa exec con una chiamata normale.
rilancia_installer() {
    local modo="$1" url tmp
    url="$(url_installer)"
    if ! tmp="$(mktemp)"; then
        echo "ERRORE: non riesco a creare un file temporaneo." >&2
        exit 1
    fi
    echo "Scarico l'installer:"
    echo "  $url"
    if ! curl -fsSL "$url" -o "$tmp"; then
        rm -f "$tmp"
        echo "" >&2
        echo "ERRORE: non riesco a scaricare l'installer." >&2
        echo "        Controlla la connessione e riprova. Niente e' stato" >&2
        echo "        modificato: il comando che hai adesso continua a funzionare." >&2
        exit 1
    fi
    #  Uno scaricamento troncato, o una pagina di errore HTML, non devono
    #  finire dentro bash: il controllo costa niente, il danno di saltarlo e'
    #  un'installazione rotta a meta'.
    if [ ! -s "$tmp" ] || ! head -1 "$tmp" | grep -q '^#!'; then
        rm -f "$tmp"
        echo "" >&2
        echo "ERRORE: quello che e' arrivato non e' uno script (scaricamento" >&2
        echo "        incompleto, o l'URL risponde con altro)." >&2
        echo "        Niente e' stato modificato." >&2
        exit 1
    fi
    echo ""
    if [ -n "$modo" ]; then
        exec bash "$tmp" "$modo"
    else
        exec bash "$tmp"
    fi
}


show_help() {
    local CMD_NAME=$(basename "$0")
    cat << EOF
Uso: $CMD_NAME [OPZIONI] [COMANDO [ARGOMENTI...]]

Senza COMANDO apre una shell nel container. Con un COMANDO esegue solo
quello: vedi "Modo applicazione" piu' sotto.

Opzioni:
  -h, --help          Mostra questo help
  -v, --version       Mostra la versione di $CMD_NAME, l'immagine Docker e la
                      versione di LegoPST che contiene
  -d, --demo [NOME]   Installa una demo e lancia il container con essa. Senza
                      NOME e' quella standard, legopst_userstd, che sta dentro
                      l'immagine; con NOME e' legopst_NOME, scaricata dalla
                      release delle demo su GitHub. Una demo gia' installata
                      nella home non viene toccata. NOME vale come nome di demo
                      solo se quella demo esiste (installata o pubblicata):
                      altrimenti e' il COMANDO da eseguire, come in
                      "$CMD_NAME -d lghmi". Per non lasciare dubbi: --demo=NOME.
  -s, --socat         Usa socat per X11 forwarding (utile per SSH con MobaXterm)
  -p, --pull          Esegue docker pull dell'immagine prima di avviare il container
  -dbg, --debug       Mostra tutti i messaggi dell'avvio: quelli di $CMD_NAME,
                      gli avvisi del runtime, la preparazione del container e
                      il profilo LegoPST. Senza, si vede una riga sola che dice
                      che il container e' partito (e, se l'avvio fallisce,
                      tutto quello che era stato detto fino a li').
  -e, --exec PROG     Appena il container e' pronto esegue PROG al suo interno,
                      con l'ambiente LegoPST gia' caricato, in background: il
                      terminale resta una shell del container. PROG puo' avere
                      argomenti, tra virgolette: -e "lghmi -staz".
                      L'output va in /tmp/lgdock_exec.log, dentro il container.

Modo applicazione:
  $CMD_NAME [OPZIONI] COMANDO [ARGOMENTI...]
                      Il container esegue solo COMANDO, senza aprire una shell
                      nel terminale, e VIVE QUANTO LUI. Quando COMANDO finisce -
                      chiuso o andato in crash - il container termina, con
                      tutto quello che COMANDO ha lanciato. $CMD_NAME resta in
                      attesa e restituisce il codice di uscita di COMANDO;
                      Ctrl-C lo ferma. Le opzioni di $CMD_NAME vanno PRIMA del
                      comando: quello che segue e' suo.

Manutenzione:
  -update             Reinstalla $CMD_NAME all'ultima versione e aggiorna
                      l'immagine Docker. Riscarica ed esegue l'installer dallo
                      stesso repository da cui e' stato installato.
  -uninstall          Disinstalla $CMD_NAME. Non tocca l'immagine Docker ne' i
                      tuoi dati (~/legocad, ~/sked): dice come rimuoverli.

Esempi:
  $CMD_NAME                  # Lancia container LegoPST (modalità standard)
  $CMD_NAME --demo           # Installa modello demo (legocad e sked) e lancia container
  $CMD_NAME --socat          # Lancia container con socat per X11 (per SSH/MobaXterm)
  $CMD_NAME -d -s            # Demo + socat
  $CMD_NAME -e lghmi         # Lancia il container e apre subito lghmi
  $CMD_NAME lghmi            # Solo lghmi: il container si chiude con lui
  $CMD_NAME lghmi -staz      # ... con i suoi argomenti
  $CMD_NAME -d lghmi         # ... con la demo standard
  $CMD_NAME -d nucleare      # Installa la demo legopst_nucleare (dalla release)
  $CMD_NAME -update          # aggiorna comando e immagine all'ultima versione
  $CMD_NAME -uninstall       # rimuove il comando

Modalità X11:
  - Standard (default): X11 forwarding diretto, adatto per uso locale
  - Socat (--socat): Crea socket bridge, necessario per SSH X11 forwarding

EOF
}

#  Tre versioni diverse, e prima se ne vedeva una sola:
#    - quella di questo lanciatore (la stampiglia l'installer dal file VERSION
#      del repository da cui lo ha scaricato);
#    - l'immagine Docker che lancia;
#    - il LegoPST che c'e' DENTRO l'immagine, che e' quello che gira davvero:
#      il file VERSION e version.h (git describe, numero e data di build)
#      scritti quando l'immagine e' stata costruita.
#  Lanciatore e immagine si aggiornano separatamente, quindi possono non
#  coincidere: e' proprio per questo che si mostrano entrambe.
#  L'ultima si legge dall'immagine con un container di un attimo - ma solo se
#  l'immagine c'e' gia': "docker run" su un'immagine assente la SCARICHEREBBE,
#  qualche GB per rispondere a -v.
show_version() {
    local CMD_NAME creata info
    CMD_NAME=$(basename "$0")
    echo "$CMD_NAME v${VERSION} - lanciatore del container LegoPST"
    if ! $DOCKER_CMD image inspect "$IMAGE_NAME" >/dev/null 2>&1; then
        echo "Immagine:  $IMAGE_NAME (non ancora scaricata)"
        echo "LegoPST:   si vede dopo il primo avvio, o dopo '$CMD_NAME -p'"
        return 0
    fi
    creata=$($DOCKER_CMD image inspect --format '{{.Created}}' "$IMAGE_NAME" 2>/dev/null | cut -c1-10)
    echo "Immagine:  $IMAGE_NAME${creata:+ (creata il $creata)}"
    #  Il container restituisce i quattro valori grezzi, ciascuno con la sua
    #  etichetta; la frase si compone qui.
    info=$($DOCKER_CMD run --rm --platform linux/amd64 --entrypoint sh "$IMAGE_NAME" -c '
        R=/home/legoroot_fedora41
        echo "V=$(tr -d "[:space:]" < $R/VERSION 2>/dev/null)"
        echo "G=$(grep "define GIT_VERSION_STRING" $R/version.h 2>/dev/null | cut -d\" -f2)"
        echo "N=$(grep "define BUILD_NUMBER" $R/version.h 2>/dev/null | tr -dc "0-9")"
        echo "D=$(grep "define BUILD_DATE_STRING" $R/version.h 2>/dev/null | cut -d\" -f2)"' 2>/dev/null) || info=""
    if [ -z "$info" ]; then
        echo "LegoPST:   versione non leggibile da questa immagine"
        return 0
    fi
    local v g n d riga
    v=$(sed -n 's/^V=//p' <<< "$info")
    g=$(sed -n 's/^G=//p' <<< "$info")
    n=$(sed -n 's/^N=//p' <<< "$info")
    d=$(sed -n 's/^D=//p' <<< "$info")
    # la data arriva come AAAAMMGG
    [[ "$d" =~ ^([0-9]{4})([0-9]{2})([0-9]{2})$ ]] && d="${BASH_REMATCH[1]}-${BASH_REMATCH[2]}-${BASH_REMATCH[3]}"
    riga="${v:-?}"
    [ -n "$g" ] && riga="$riga - $g"
    [ -n "$n" ] && riga="$riga, build $n${d:+ del $d}"
    echo "LegoPST:   $riga"
}

# =============================================================================
# Controllo Docker
# =============================================================================
#  Il runtime: docker se c'e' come ESEGUIBILE, altrimenti podman.
#
#  Non basta cercare "docker". Ci sono tre situazioni diverse, e solo la prima
#  e' quella ovvia:
#    1. Docker vero              -> /usr/bin/docker
#    2. podman-docker (Fedora,   -> /usr/bin/docker, ma sotto e' Podman
#       Ubuntu): uno shim
#    3. alias di shell su Podman -> NIENTE nel PATH
#  Il terzo caso e' quello che rompeva: un "alias docker=podman" nel .bashrc
#  non esiste nelle shell NON INTERATTIVE, e questo script ci gira sempre
#  (l'installer lo lancia con "bash -c"). Si cercava docker, non lo si trovava,
#  e si diceva all'utente di installare Docker mentre Podman era li'.
if command -v docker >/dev/null 2>&1; then
    RUNTIME="docker"
elif command -v podman >/dev/null 2>&1; then
    RUNTIME="podman"
else
    echo "---------------------------------------------------------------------"
    echo "WARNING: no container runtime found in your PATH (docker or podman)."
    echo "         Install one of them to create the LegoPST container."
    echo ""
    echo "         If 'docker' works in your terminal but not here, it is an"
    echo "         ALIAS: aliases do not exist in non-interactive shells."
    echo "         Install podman-docker, or use podman directly - this script"
    echo "         now finds it by itself."
    echo "---------------------------------------------------------------------"
    exit 1
fi

# 2. Determina se è necessario usare 'sudo'
DOCKER_CMD="$RUNTIME"

#    Il percorso standard del socket di Docker su Linux.
DOCKER_SOCKET="/var/run/docker.sock"

#    Verifichiamo se il socket esiste e se l'utente corrente NON ha permessi di scrittura (-w).
#    Se entrambe le condizioni sono vere, significa che serve 'sudo'.
if [ -S "$DOCKER_SOCKET" ] && ! [ -w "$DOCKER_SOCKET" ]; then
  echo "INFO: L'utente corrente non ha i permessi per accedere al socket di Docker."
  echo "      Verrà usato 'sudo' per eseguire i comandi Docker."
  DOCKER_CMD="sudo $RUNTIME"

  # Aggiungiamo un piccolo test per vedere se 'sudo' funziona senza password
  # o per forzare l'utente a inserirla subito, prima che lo script faccia altro.
  if ! sudo -n true 2>/dev/null; then
    echo "      Potrebbe essere richiesta la password di sudo."
    sudo -v # Chiede la password ora e la tiene in cache per un po'.
  fi
  echo "------------------------------------------------------------"
fi

# --- FINE BLOCCO DI VERIFICA DOCKER ---

# =============================================================================
# Parsing Parametri
# =============================================================================
RUN_DEMO=false
DEMO_NOME="userstd"
USE_SOCAT=false
DO_PULL=false
EXEC_PROG=""
APP_PROG=""
DEBUG=false

#  Il comando del modo applicazione, dagli argomenti rimasti. Un argomento solo
#  si prende com'e': puo' essere una riga di shell tra virgolette
#  ("cd ~/sked/X && lghmi"). Piu' argomenti si quotano uno per uno, cosi'
#  arrivano al programma come sono stati scritti, spazi compresi.
comando_da() {
    if [[ $# -eq 1 ]]; then
        APP_PROG="$1"
    else
        APP_PROG=$(printf '%q ' "$@")
        APP_PROG="${APP_PROG% }"
    fi
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        -h|--help)
            show_help
            exit 0
            ;;
        -v|--version)
            show_version
            exit 0
            ;;
        -d|--demo)
            RUN_DEMO=true
            shift
            #  La parola che segue puo' essere il nome della demo oppure il
            #  comando da eseguire ("lgrun -d lghmi"). E' un nome di demo se
            #  quella demo ESISTE: gia' installata nella home, oppure
            #  pubblicata nella release. Altrimenti resta dov'e', e il giro
            #  successivo la prende come comando.
            if [[ $# -gt 0 && "$1" =~ ^[A-Za-z0-9_][A-Za-z0-9_-]*$ ]]; then
                if [[ -d "$HOME/legopst_$1" ]] || \
                   curl -fsIL --max-time 8 -o /dev/null "$(url_demo "$1")" 2>/dev/null; then
                    DEMO_NOME="$1"
                    shift
                fi
            fi
            ;;
        --demo=*)
            RUN_DEMO=true
            DEMO_NOME="${1#--demo=}"
            if [[ ! "$DEMO_NOME" =~ ^[A-Za-z0-9_][A-Za-z0-9_-]*$ ]]; then
                echo "Nome di demo non valido: '$DEMO_NOME' (lettere, cifre, _ e -)"
                exit 1
            fi
            shift
            ;;
        -s|--socat)
            USE_SOCAT=true
            shift
            ;;
        -p|--pull)
            DO_PULL=true
            shift
            ;;
        -dbg|--debug)
            DEBUG=true
            shift
            ;;
        -e|--exec)
            if [[ $# -lt 2 || -z "$2" ]]; then
                echo "L'opzione $1 vuole il programma da eseguire: $1 <prog>"
                exit 1
            fi
            EXEC_PROG="$2"
            shift 2
            ;;
        -update|--update)
            #  --pull-image: l'immagine si scarica DOPO aver installato gli
            #  script, non prima. Il nome dell'immagine sta dentro lgdock, e
            #  un aggiornamento puo' cambiarlo: scaricandola prima si tirerebbe
            #  giu' quella vecchia.
            rilancia_installer --pull-image
            ;;
        -uninstall|--uninstall)
            rilancia_installer -u
            ;;
        --)
            shift
            [[ $# -gt 0 ]] && comando_da "$@"
            break
            ;;
        -*)
            echo "Opzione sconosciuta: $1"
            echo "Usa --help per vedere le opzioni disponibili"
            exit 1
            ;;
        *)
            #  La prima parola che non e' un'opzione e' il comando, e tutto
            #  quello che segue e' suo: come sudo, o "docker run immagine cmd".
            comando_da "$@"
            break
            ;;
    esac
done

#  -e e il comando rispondono a due idee opposte: con -e il programma parte in
#  background e il terminale resta una shell del container; con un comando non
#  c'e' nessuna shell e il container vive quanto lui.
if [[ -n "$EXEC_PROG" && -n "$APP_PROG" ]]; then
    echo "-e e un comando non si usano insieme: -e lascia una shell nel"
    echo "terminale, il comando no. Scegline uno."
    exit 1
fi

# =============================================================================
# Modo silenzioso (il default) / -dbg
# =============================================================================
#  L'avvio parla molto: i riquadri di questo script, gli avvisi del runtime, la
#  preparazione nel container, il profilo LegoPST. Serve quando qualcosa non
#  va; tutte le altre volte e' una pagina di testo prima del prompt, o prima
#  dell'output del comando che si e' chiesto. Senza -dbg se ne vede una riga.
#
#  I messaggi pero' non si buttano: finiscono in un file, e se lo script esce
#  con un errore PRIMA di aver avviato il container li si mostra tutti. Un
#  avvio fallito in silenzio sarebbe peggio del rumore.
#
#  Solo lo stdout: gli errori veri (stderr) restano a vista, e le domande di
#  sudo passano dal terminale. FD_VISTA e' il descrittore su cui scrivere
#  quello che si deve vedere comunque - l'avanzamento di un pull chiesto con -p.
FD_VISTA=1
LG_AVVIATO=""
HOST_LOG=""
fine_host() {
    local rc=$?
    if [[ -n "$HOST_LOG" ]]; then
        if [[ $rc -ne 0 && -z "$LG_AVVIATO" ]]; then
            cat "$HOST_LOG" >&5
        fi
        rm -f "$HOST_LOG"
    fi
    return $rc
}
if [[ "$DEBUG" != true ]] && HOST_LOG=$(mktemp 2>/dev/null); then
    exec 5>&1 >"$HOST_LOG"
    FD_VISTA=5
    trap fine_host EXIT
else
    HOST_LOG=""
fi

# =============================================================================
# Informazioni Host
# =============================================================================
HOST_USERNAME=$(whoami)
HOST_USER_ID=$(id -u)
HOST_GROUP_ID=$(id -g)
HOST_USER_HOME="$HOME"

# Modalità di esecuzione
MODE="standard"
[[ "$USE_SOCAT" == true ]] && MODE="socat"

echo "======================================================================="
echo "  Avvio LegoPST Docker - Modalità: $MODE"
echo "======================================================================="
echo "Immagine: $IMAGE_NAME"
echo "Demo mode: $RUN_DEMO$([[ "$RUN_DEMO" == true ]] && echo " (legopst_$DEMO_NOME)")"
echo "Utente: $HOST_USERNAME (UID: $HOST_USER_ID, GID: $HOST_GROUP_ID)"
echo "Home directory: $HOST_USER_HOME"
echo "DISPLAY: $DISPLAY"
echo ""

# =============================================================================
# Docker Pull (se richiesto)
# =============================================================================
if [[ "$DO_PULL" == true ]]; then
    echo "--- Aggiornamento immagine Docker ---"
    echo "Esecuzione: $DOCKER_CMD pull $IMAGE_NAME"
    $DOCKER_CMD pull $IMAGE_NAME >&$FD_VISTA
    echo ""
fi

# =============================================================================
# Creazione directory defaults se non esiste
# =============================================================================
HOST_DEFAULTS_DIR="$HOME/defaults"
if [[ ! -d "$HOST_DEFAULTS_DIR" ]]; then
    echo "Creazione directory $HOST_DEFAULTS_DIR..."
    mkdir -p "$HOST_DEFAULTS_DIR"
else
    echo "Directory defaults: OK"
fi

# =============================================================================
# Setup X11 - Diverso per modalità standard vs socat
# =============================================================================
if [[ "$USE_SOCAT" == true ]]; then
    echo ""
    echo "--- Setup X11 con socat ---"
    
    # Estrai numero display
    DISPLAY_NUM=$(echo $DISPLAY | sed 's/.*:\([0-9]*\).*/\1/')
    SOCKET_PATH="/tmp/.X11-unix/X${DISPLAY_NUM}"
    
    # Flag per tracciare se abbiamo creato il socket
    SOCKET_CREATED=false
    SOCAT_PID=""
    
    # Crea socket bridge se non esiste
    if [ ! -S "$SOCKET_PATH" ]; then
        if ! command -v socat >/dev/null 2>&1; then
            echo "ERRORE: socat non trovato ma richiesto con --socat"
            echo "Installalo con: sudo dnf install socat"
            exit 1
        fi
        
        echo "Creando socket bridge X11 su $SOCKET_PATH..."
        socat UNIX-LISTEN:$SOCKET_PATH,fork,mode=777 TCP:localhost:$((6000 + DISPLAY_NUM)) &
        SOCAT_PID=$!
        sleep 1
        chmod 777 $SOCKET_PATH
        SOCKET_CREATED=true
        echo "Socket bridge creato (PID: $SOCAT_PID)"
    else
        echo "Socket X11 $SOCKET_PATH esiste già"
    fi
    
    # Estrai cookie X11 e crea file xauth temporaneo
    TEMP_XAUTH="/tmp/.docker_xauth_$$"
    touch "$TEMP_XAUTH"
    chmod 600 "$TEMP_XAUTH"
    
    # Controlla se xauth è disponibile
    if command -v xauth >/dev/null 2>&1; then
        XAUTH_COOKIE=$(xauth list $DISPLAY | head -n1 | awk '{print $3}')
        if [ -n "$XAUTH_COOKIE" ]; then
            xauth -f "$TEMP_XAUTH" add ":${DISPLAY_NUM}" . "$XAUTH_COOKIE"
            echo "X11 authentication configurata per display :${DISPLAY_NUM}"
        fi
    else
        echo "WARNING: xauth non trovato, X11 authentication potrebbe non funzionare"
        echo "         Installa con: sudo dnf install xorg-x11-xauth"
    fi
    
    # Permetti connessioni locali
    xhost +local:all 2>/dev/null
    
    # Cleanup alla fine - rimuovi solo ciò che abbiamo creato
    cleanup() {
        if [ -n "$SOCAT_PID" ]; then
            [[ "$DEBUG" == true ]] && echo "Terminazione socat (PID: $SOCAT_PID)..."
            kill $SOCAT_PID 2>/dev/null
        fi
        if [ "$SOCKET_CREATED" = true ]; then
            rm -f "$SOCKET_PATH" 2>/dev/null
        fi
        rm -f "$TEMP_XAUTH" 2>/dev/null
    }
    #  una sola trap EXIT per script: questa prende anche il posto di quella
    #  del modo silenzioso, e la richiama
    trap 'cleanup; fine_host' EXIT
    
else
    echo ""
    echo "--- Setup X11 standard ---"
    DISPLAY_NUM=$(echo $DISPLAY | sed 's/.*:\([0-9]*\).*/\1/')
    echo "Modalità standard: X11 forwarding diretto"
fi

echo ""

# =============================================================================
# Script Container (comune a entrambe le modalità)
# =============================================================================
read -r -d '' CONTAINER_SCRIPT << 'SCRIPT_EOF'
set -e

# Modo silenzioso (LGDOCK_DEBUG diverso da 1): la preparazione non si vede. Va
# in un file, e se questo script esce con un errore prima di essere arrivato
# in fondo lo si stampa tutto: senza, un "UID gia' usato" o un tar fallito
# chiuderebbero il container senza una parola. I descrittori 3 e 4 tengono da
# parte lo stdout e lo stderr veri, per ridarli al programma alla fine.
LGDOCK_PRONTO=""
LGDOCK_LOG=""
if [ "${LGDOCK_DEBUG:-0}" != 1 ]; then
    LGDOCK_LOG=$(mktemp 2>/dev/null || echo /tmp/lgdock_avvio.log)
    exec 3>&1 4>&2 >"$LGDOCK_LOG" 2>&1
    trap 'rc=$?; if [ "$rc" -ne 0 ] && [ -z "$LGDOCK_PRONTO" ]; then cat "$LGDOCK_LOG" >&4; fi' EXIT
fi

USER_HOME_IN_CONTAINER="/home/HOST_USERNAME_VAR"

# Cattura DISPLAY prima di su - perché non sarà più disponibile dopo
if [[ "USE_SOCAT_VAR" == "true" ]]; then
    # In modalità socat, usiamo il numero display hardcoded
    DISPLAY_VALUE=":DISPLAY_NUM_VAR"
else
    # In modalità standard, catturiamo DISPLAY dall'environment
    DISPLAY_VALUE="$DISPLAY"
fi

# =============================================================================
# Chi e' l'utente dell'host, visto da dentro il container
# =============================================================================
# Con Docker "classico" il root del container e' il root dell'host: un file
# scritto nel bind mount esce sull'host con lo stesso UID numerico che ha qui
# dentro, e per intestare la demo all'utente basta un chown al suo UID.
#
# In modalita' ROOTLESS no. Vale per Docker rootless (rootlesskit) e per Podman
# rootless, che spesso si presenta proprio come "docker" tramite lo shim
# podman-docker: il comando e' lo stesso, il runtime sotto no. Demone/container
# finiscono in uno user namespace dove l'UID dell'host diventa 0 e i subuid di
# /etc/subuid (tipicamente da 100000 in su) diventano 1..65536:
#
#     scritto qui dentro come...    ...esce sull'host come
#     root (UID 0)                  l'utente dell'host       <- quello che serve
#     UID 1000                      99999 + 1000 = 100999    <- nessun utente
#
# Cioe' il "chown all'UID dell'host" fa esattamente il danno che dovrebbe
# evitare: la demo finisce a 100999, che sull'host non e' nessuno, e con i modi
# 0700 che si porta dietro il tarball l'utente non riesce nemmeno a entrarci.
#
# Il rilevamento non indovina niente: guarda di chi risulta /host_home - che
# sull'host e' la home dell'utente - visto da qui dentro. Quel numero E'
# l'utente dell'host in coordinate container, qualunque sia la mappatura.
HOST_HOME_UID=$(stat -c %u /host_home 2>/dev/null || true)
HOST_HOME_GID=$(stat -c %g /host_home 2>/dev/null || true)

CONT_UID="HOST_USER_ID_VAR"
CONT_GID="HOST_GROUP_ID_VAR"
LOGIN_USER="HOST_USERNAME_VAR"
ROOTLESS=false

if [ "$HOST_HOME_UID" = "HOST_USER_ID_VAR" ]; then
    : # Runtime classico (o Podman --userns=keep-id): l'UID dell'host vale
      # anche qui dentro. Niente da fare.
elif [ "$HOST_HOME_UID" = "0" ]; then
    ROOTLESS=true
    CONT_UID=0
    CONT_GID="${HOST_HOME_GID:-0}"
    LOGIN_USER=root
    USER_HOME_IN_CONTAINER="/root"
    echo "=== Modalita' rootless rilevata (Podman o Docker) ==="
    echo "/host_home risulta di root: qui dentro l'utente dell'host E' root."
    echo "Si lavora percio' come root del container. Altrimenti tutto quello che"
    echo "si scrive nella home uscirebbe sull'host intestato a un UID mappato"
    echo "(100999 e simili), che HOST_USERNAME_VAR non puo' nemmeno leggere."
    echo ""
elif [ -z "$HOST_HOME_UID" ]; then
    echo "ATTENZIONE: /host_home non e' leggibile (stat fallito)."
    echo "Proseguo come se fosse un runtime classico, UID HOST_USER_ID_VAR."
    echo ""
else
    echo "ATTENZIONE: /host_home risulta dell'UID $HOST_HOME_UID, che non e'"
    echo "ne' HOST_USER_ID_VAR (runtime classico) ne' 0 (rootless). Il bind mount"
    echo "e' rimappato in un modo che non so tradurre: userns-remap, oppure un"
    echo "filesystem che non tiene le proprieta' Unix. Uso $HOST_HOME_UID come"
    echo "proprietario; se i chown falliscono, i file restano come sono."
    echo ""
    CONT_UID="$HOST_HOME_UID"
    CONT_GID="${HOST_HOME_GID:-HOST_GROUP_ID_VAR}"
fi

if [ "$ROOTLESS" = true ]; then
    # Niente utente da creare: si usa root, che qui dentro E' l'utente
    # dell'host. E niente sudoers: root non ne ha bisogno.
    echo "=== Utente nel container: root (sull'host risulta HOST_USERNAME_VAR) ==="
    mkdir -p "$USER_HOME_IN_CONTAINER"
else
    echo '=== Configurazione utente dinamica (UID: HOST_USER_ID_VAR, GID: HOST_GROUP_ID_VAR, Utente: HOST_USERNAME_VAR) ==='

    # Crea il gruppo se non esiste con il GID dell'host
    if ! getent group "HOST_GROUP_ID_VAR" >/dev/null 2>&1; then
        echo "Creando gruppo 'HOST_USERNAME_VAR' (GID: HOST_GROUP_ID_VAR)"
        groupadd -g "HOST_GROUP_ID_VAR" "HOST_USERNAME_VAR"
    else
        EXISTING_GROUP_NAME=$(getent group "HOST_GROUP_ID_VAR" | cut -d: -f1)
        if [ "$EXISTING_GROUP_NAME" != "HOST_USERNAME_VAR" ]; then
            echo "Attenzione: GID HOST_GROUP_ID_VAR è già usato dal gruppo '$EXISTING_GROUP_NAME'"
            if getent group "HOST_USERNAME_VAR" >/dev/null 2>&1; then 
                groupmod -g "HOST_GROUP_ID_VAR" "HOST_USERNAME_VAR"
            else 
                groupadd -g "HOST_GROUP_ID_VAR" "HOST_USERNAME_VAR"
            fi
        fi
    fi

    # Crea l'utente se non esiste con l'UID dell'host
    if ! getent passwd "HOST_USER_ID_VAR" >/dev/null 2>&1; then
        echo "Creando utente 'HOST_USERNAME_VAR' (UID: HOST_USER_ID_VAR, GID: HOST_GROUP_ID_VAR)"
        useradd -u "HOST_USER_ID_VAR" -g "HOST_GROUP_ID_VAR" -m -d "$USER_HOME_IN_CONTAINER" -s /bin/bash "HOST_USERNAME_VAR"
    else
        EXISTING_USER_WITH_UID=$(getent passwd "HOST_USER_ID_VAR" | cut -d: -f1)
        if [ "$EXISTING_USER_WITH_UID" != "HOST_USERNAME_VAR" ]; then
            echo "ERRORE: UID HOST_USER_ID_VAR è già usato dall'utente '$EXISTING_USER_WITH_UID'"
            exit 1
        else
            echo "Utente 'HOST_USERNAME_VAR' (UID HOST_USER_ID_VAR) trovato. Aggiorno configurazione."
            usermod -g "HOST_GROUP_ID_VAR" -d "$USER_HOME_IN_CONTAINER" -s /bin/bash "HOST_USERNAME_VAR"
        fi
    fi

    # Configurazione sudo
    SUDOERS_FILE_PATH="/etc/sudoers.d/HOST_USERNAME_VAR"
    echo "HOST_USERNAME_VAR ALL=(ALL) NOPASSWD:ALL" > "${SUDOERS_FILE_PATH}"
    chmod 0440 "${SUDOERS_FILE_PATH}"
fi

# Copia xauth per l'utente (solo in modalità socat)
if [[ "USE_SOCAT_VAR" == "true" ]]; then
    mkdir -p "$USER_HOME_IN_CONTAINER"
    cp /tmp/.Xauthority "$USER_HOME_IN_CONTAINER/.Xauthority" 2>/dev/null || touch "$USER_HOME_IN_CONTAINER/.Xauthority"
    chown "$CONT_UID:$CONT_GID" "$USER_HOME_IN_CONTAINER/.Xauthority"
    chmod 600 "$USER_HOME_IN_CONTAINER/.Xauthority"
fi

# Link simbolici
#
# -n (--no-dereference) su tutte, per allinearle alle due del demo che lo
# avevano gia'. Serve quando il bersaglio esiste ed e' un LINK A DIRECTORY:
# senza -n, ln lo segue e crea il collegamento DENTRO la directory puntata -
# viene ~/sked/sked invece di ~/sked, e si continua a vedere il contenuto
# vecchio senza un errore che lo dica. Con -n il link viene sostituito.
#
# Attenzione a non aspettarsi di piu': se il bersaglio e' una DIRECTORY VERA
# nemmeno -n la sostituisce (ln non rimpiazza una directory con un link), e il
# collegamento finisce dentro comunque. Quel caso resta scoperto: qui va bene
# perche' non si verifica - con --rm la home del container e' sempre nuova,
# appena creata da useradd o il /root dell'immagine, che non contiene ne' sked
# ne' legocad - ma se un domani l'immagine nascesse con quelle directory il
# problema tornerebbe, e -n non basterebbe.
echo "Creazione link simbolici in $USER_HOME_IN_CONTAINER..."
mkdir -p "$USER_HOME_IN_CONTAINER"
ln -sfn /host_home "$USER_HOME_IN_CONTAINER/host_data"

# Eventuale installazione di una demo nella home dell'utente.
#
# La demo e' una directory legopst_<nome> con dentro legocad e sked, cioe'
# un'area di lavoro come le altre. Il pacchetto legopst_<nome>.tgz si prende
# dall'immagine se c'e' (quella standard, userstd, ci viaggia dentro: si
# installa anche senza rete); altrimenti si scarica dalla release delle demo.
DEMO_NOME="DEMO_NOME_VAR"
DEMO_DIR="legopst_$DEMO_NOME"
if [[ "RUN_DEMO_FLAG" == "true" ]]; then
    if [ ! -d "/host_home/$DEMO_DIR" ]; then
        # l'estrazione dura: in modo silenzioso almeno si dica che c'e'
        [ -n "$LGDOCK_LOG" ] && echo "LegoPST: installazione della demo $DEMO_DIR in corso..." >&4
        echo "=========================================="
        echo "  Installazione Demo LegoPST: $DEMO_DIR"
        echo "=========================================="
        DEMO_TGZ="/home/legoroot_fedora41/demo/$DEMO_DIR.tgz"
        if [ ! -f "$DEMO_TGZ" ]; then
            DEMO_TGZ="/tmp/$DEMO_DIR.tgz"
            echo "Non e' nell'immagine: la scarico da"
            echo "  DEMO_URL_VAR"
            [ -n "$LGDOCK_LOG" ] && echo "LegoPST: la scarico dalla release delle demo..." >&4
            if ! curl -fL --retry 3 -o "$DEMO_TGZ" "DEMO_URL_VAR"; then
                echo ""
                echo "ERRORE: non riesco a scaricare la demo '$DEMO_NOME'."
                echo "        Non e' nell'immagine e la release non la porta (o manca"
                echo "        la rete). Le demo si pubblicano con demo/publish_demo.sh."
                rm -f "$DEMO_TGZ"
                exit 1
            fi
        fi
        # --no-same-owner: NON ripristinare l'UID registrato nel tarball (che e'
        # 1000, l'UID di chi ha confezionato la demo). Estraendo da root, senza
        # questa opzione tar intesta ogni file a 1000 - che sotto rootless e'
        # gia' l'UID sbagliato (esce sull'host come 100999) - e solo dopo ci
        # applica il modo. Con --no-same-owner i file restano di chi estrae e a
        # decidere il proprietario e' il solo chown qui sotto, che sa qual e'
        # quello giusto. Probabile che sia anche la fine dei "Cannot change
        # mode": il chmod cadeva su file appena passati a un altro UID.
        #
        # L'estrazione resta comunque dentro un "if": su filesystem che non
        # implementano i permessi Unix (cartella condivisa di VM, NTFS/exFAT,
        # share di rete) tar puo' lamentarsi lo stesso. I file vengono estratti:
        # quello che non riesce e' solo il ripristino del modo. Senza l'"if" il
        # `set -e` in testa allo script farebbe morire tutto QUI, e non
        # girerebbero il chown e i link a legocad/sked che stanno subito sotto:
        # la demo resterebbe senza proprietario giusto e senza link, cioe'
        # inutilizzabile, senza che nulla lo dica.
        if ! tar --no-same-owner -xvzf "$DEMO_TGZ" -C "/host_home/"; then
            echo ""
            echo "ATTENZIONE: tar ha segnalato errori durante l'estrazione."
            echo "Se sono del tipo \"Cannot change mode\" sono innocui: i file ci"
            echo "sono, ma il filesystem dell'host non accetta i permessi."
            echo "Proseguo con l'installazione."
            echo ""
        fi
        # Il pacchetto deve contenere proprio legopst_<nome>: e' da li' che si
        # fanno i link qui sotto. Uno confezionato da una directory con un
        # altro nome si estrarrebbe altrove, lasciando link rotti in silenzio.
        if [ ! -d "/host_home/$DEMO_DIR/legocad" ] || [ ! -d "/host_home/$DEMO_DIR/sked" ]; then
            echo ""
            echo "ERRORE: il pacchetto non contiene $DEMO_DIR/legocad e $DEMO_DIR/sked."
            echo "        Va rifatto con demo/make_demo_tgz.sh -d $DEMO_NOME."
            exit 1
        fi
        # Stessa ragione dell'"if" sopra: anche il chown puo' fallire su quei
        # filesystem, e nudo sotto `set -e` ammazzerebbe l'installazione un
        # attimo prima dei link.
        if ! chown -R "$CONT_UID:$CONT_GID" "/host_home/$DEMO_DIR"; then
            echo ""
            echo "ATTENZIONE: chown -R fallito su /host_home/$DEMO_DIR."
            echo "I file restano come li ha scritti tar. Proseguo."
            echo ""
        fi
        
        echo ""
        echo "Contenuto directory demo:"
        echo "- legocad:"
        ls -la /host_home/$DEMO_DIR/legocad | head -10
        echo "- sked:"
        ls -la /host_home/$DEMO_DIR/sked | head -10
        
        # Link simbolici, RELATIVI. Con il target assoluto (/host_home/...)
        # funzionerebbero solo qui dentro: /host_home e' il nome che la home ha
        # nel container, sull'host non esiste e i due link resterebbero
        # penzolanti. Relativi si risolvono giusti in tutti e due i mondi:
        # ~/legopst_<nome>/legocad sull'host, /host_home/legopst_<nome>/legocad
        # qui. Ed e' anche la convenzione delle installazioni native.
        ln -sfn "$DEMO_DIR/legocad" /host_home/legocad 2>/dev/null || true
        ln -sfn "$DEMO_DIR/sked" /host_home/sked 2>/dev/null || true
        chown -h "$CONT_UID:$CONT_GID" /host_home/legocad 2>/dev/null || true
        chown -h "$CONT_UID:$CONT_GID" /host_home/sked 2>/dev/null || true
        
        echo "=========================================="
        echo "  Demo installata in: HOST_USER_HOME_VAR/$DEMO_DIR"
        echo "=========================================="
        echo ""
    else
        echo "Demo già installata in /host_home/$DEMO_DIR"
        # Se e' stata installata da una lgdock precedente sotto rootless, e'
        # intestata all'UID sbagliato e sull'host non si apre nemmeno.
        DEMO_UID=$(stat -c %u /host_home/$DEMO_DIR/legocad 2>/dev/null || true)
        if [ -n "$DEMO_UID" ] && [ "$DEMO_UID" != "$CONT_UID" ]; then
            echo ""
            echo "ATTENZIONE: la demo presente risulta dell'UID $DEMO_UID, non $CONT_UID."
            echo "L'ha installata una versione di lgdock che non riconosceva questa"
            echo "mappatura: sull'host non e' utilizzabile. Per rifarla, da una"
            echo "shell dell'host:"
            echo "    rm -rf ~/$DEMO_DIR ~/legocad ~/sked"
            echo "    lgrun --demo=$DEMO_NOME"
            echo ""
        fi
    fi
fi

# Link simbolici finali
ln -sfn /host_home/legocad "$USER_HOME_IN_CONTAINER/legocad" 2>/dev/null || true
ln -sfn /host_home/sked "$USER_HOME_IN_CONTAINER/sked" 2>/dev/null || true
ln -sfn /host_home/defaults "$USER_HOME_IN_CONTAINER/defaults" 2>/dev/null || true
chown -h "$CONT_UID:$CONT_GID" "$USER_HOME_IN_CONTAINER/host_data"

# Risorse X delle applicazioni Motif (config, mmi, graphics, legocad...):
# colori, font, scorciatoie - anche il Ctrl+C/X/V nei campi di config. Il
# profilo le cerca in ~/risorse (XAPPLRESDIR, .profile_legoroot), e nella home
# del container, che nasce nuova a ogni avvio, quella directory non c'era:
# i programmi partivano con l'aspetto di default, senza un errore che lo dicesse.
#
# Come per defaults, stanno sull'host e qui si collegano: cosi' una modifica
# fatta dall'utente sopravvive al container. Se l'host non le ha ancora si
# creano ADESSO, dalla copia ufficiale dell'immagine (util2025/risorse, quella
# che il suo Readme dice di copiare nella home): a container chiuso restano
# come ~/risorse. Se ci sono gia' non si toccano - sono personalizzabili, e
# l'utente puo' averle cambiate.
RISORSE_UFFICIALI="/home/legoroot_fedora41/util2025/risorse"
if [ ! -e /host_home/risorse ] && [ -d "$RISORSE_UFFICIALI" ]; then
    echo "Creazione di ~/risorse (risorse X delle applicazioni) da util2025/risorse..."
    # Gli stessi riguardi della demo: su un filesystem che non accetta i
    # permessi Unix cp e chown possono lamentarsi, e nudi sotto set -e
    # fermerebbero l'avvio del container per delle risorse grafiche.
    if mkdir -p /host_home/risorse && cp -R "$RISORSE_UFFICIALI/." /host_home/risorse/; then
        chown -R "$CONT_UID:$CONT_GID" /host_home/risorse 2>/dev/null || \
            echo "ATTENZIONE: chown fallito su /host_home/risorse. Proseguo."
    else
        echo "ATTENZIONE: non riesco a creare /host_home/risorse. Proseguo senza:"
        echo "le applicazioni useranno l'aspetto di default."
    fi
fi
ln -sfn /host_home/risorse "$USER_HOME_IN_CONTAINER/risorse" 2>/dev/null || true
chown -h "$CONT_UID:$CONT_GID" "$USER_HOME_IN_CONTAINER/risorse" 2>/dev/null || true

# Configurazione .bash_profile
BASH_PROFILE_PATH="$USER_HOME_IN_CONTAINER/.bash_profile"
PROFILE_LEGOROOT_PATH="/home/legoroot_fedora41/.profile_legoroot"

{
    echo "# File .bash_profile per HOST_USERNAME_VAR"
    echo "# Generato automaticamente da lgdock"
    echo ""
    echo "# Carica .bashrc se esiste"
    echo "if [ -f ~/.bashrc ]; then"
    echo "    . ~/.bashrc"
    echo "fi"
    echo ""
    echo "# Esporta la variabile DISPLAY"
    echo "export DISPLAY=\"$DISPLAY_VALUE\""
    if [[ "USE_SOCAT_VAR" == "true" ]]; then
        echo "export XAUTHORITY=\"\$HOME/.Xauthority\""
    fi
    [ "${LGDOCK_DEBUG:-0}" = 1 ] && echo "echo \"DISPLAY impostato a: \$DISPLAY\""
    echo ""
    echo "# Prompt personalizzato LegoPST"
    echo "export PS1='LegoPST@\w \$ '"
    echo ""
    echo "# Sorgente del profilo custom LegoPST"
    # In modo silenzioso il profilo si carica senza i suoi messaggi (la
    # piattaforma, le tavole del vapore, il simulatore corrente...): sono
    # quelli che riempivano lo schermo prima del prompt, e prima dell'output
    # di un comando lanciato con "lgrun <comando>". Con -dbg si vedono.
    echo "if [ -f \"$PROFILE_LEGOROOT_PATH\" ]; then"
    if [ "${LGDOCK_DEBUG:-0}" = 1 ]; then
        echo "    source \"$PROFILE_LEGOROOT_PATH\""
    else
        echo "    source \"$PROFILE_LEGOROOT_PATH\" >/dev/null 2>&1"
    fi
    echo "fi"
    # lgrun -e <prog>: il programma arriva dall'host nella variabile
    # LGDOCK_EXEC (docker run -e), non sostituito nel testo di questo script,
    # cosi' spazi e virgolette arrivano intatti; qui si scrive quotato con %q.
    # Parte DOPO il profilo, che da' DISPLAY, PATH e le variabili LegoPST.
    # Una volta sola per container: il .bash_profile lo rileggono anche le
    # shell di login aperte dopo (un "bash -l", un terminale da lghmi), e
    # quelle non devono rilanciarlo - da qui il segnaposto in /tmp, che con
    # --rm muore con il container.
    # In background, perche' il terminale resti una shell: lghmi e quello che
    # apre (HMI, xstaz, la simulazione) vivono finche' vive il container, e
    # legare il container alla vita di lghmi li porterebbe via tutti al Quit.
    if [ -n "${LGDOCK_EXEC:-}" ]; then
        echo ""
        echo "# lgrun -e: eseguito una volta, appena il container e' pronto"
        printf 'LGDOCK_EXEC=%q\n' "$LGDOCK_EXEC"
        echo 'if [ ! -e /tmp/.lgdock_exec_fatto ]; then'
        echo '    touch /tmp/.lgdock_exec_fatto'
        echo '    LGDOCK_PRIMO=${LGDOCK_EXEC%% *}'
        echo '    if command -v "$LGDOCK_PRIMO" >/dev/null 2>&1; then'
        [ "${LGDOCK_DEBUG:-0}" = 1 ] && \
            echo '        echo "Avvio di: $LGDOCK_EXEC   (output in /tmp/lgdock_exec.log)"'
        echo '        ( eval "$LGDOCK_EXEC" >/tmp/lgdock_exec.log 2>&1 & )'
        echo '    else'
        echo '        echo "lgrun -e: comando non trovato nel container: $LGDOCK_PRIMO"'
        echo '    fi'
        echo '    unset LGDOCK_PRIMO'
        echo 'fi'
    fi
} > "$BASH_PROFILE_PATH"

chown "$CONT_UID:$CONT_GID" "$BASH_PROFILE_PATH"

echo '======================================================================='
echo '  Container Pronto'
echo '======================================================================='
echo "Utente: $LOGIN_USER (UID $CONT_UID, GID $CONT_GID)"
if [ "$ROOTLESS" = true ]; then
    echo "Runtime: rootless - sull'host i file risultano di HOST_USERNAME_VAR"
fi
echo "DISPLAY: $DISPLAY_VALUE"
[[ "USE_SOCAT_VAR" == "true" ]] && echo "X11 Mode: socat bridge" || echo "X11 Mode: standard"
[ -n "${LGDOCK_EXEC:-}" ] && echo "Da eseguire all'avvio: $LGDOCK_EXEC"
echo '======================================================================='
echo ''

# Fine della preparazione. In modo silenzioso si ridanno stdout e stderr veri
# a quello che viene dopo - la shell, o il comando - e si dice in UNA riga che
# questo e' un container LegoPST. Sullo stderr: chi fa "lgrun ls | wc -l" deve
# contare i file, non anche questa riga.
if [ -n "$LGDOCK_LOG" ]; then
    LGDOCK_PRONTO=1
    exec 1>&3 2>&4 3>&- 4>&-
    LGDOCK_VER=$(tr -d '[:space:]' < /home/legoroot_fedora41/VERSION 2>/dev/null || true)
    LGDOCK_RIGA="LegoPST container avviato (${LGDOCK_IMAGE:-immagine sconosciuta}${LGDOCK_VER:+, LegoPST $LGDOCK_VER})"
    [ -z "${LGDOCK_APP:-}" ] && LGDOCK_RIGA="$LGDOCK_RIGA - 'exit' per uscire"
    echo "$LGDOCK_RIGA" >&2
fi

# lgrun <comando>: modo "applicazione". Niente shell interattiva:
# si esegue solo il programma, come farebbe la shell di login (su - ... -c
# carica il .bash_profile, quindi DISPLAY e il profilo LegoPST), e si esce con
# il suo codice. Questo script e' il processo 1 del container: quando esce, il
# container finisce e il runtime termina tutto quello che il programma aveva
# lanciato - simulazione, HMI, legopc - anche se in sessioni proprie.
#
# su in background + wait, e non una semplice chiamata, per i segnali: il
# processo 1 ignora quelli per cui non ha un gestore, e un Ctrl-C dall'host
# (che il runtime inoltra qui come SIGINT) andrebbe perso. Con la trap lo si
# passa al programma, e wait - a differenza di un comando in primo piano - si
# interrompe per farla scattare.
if [ -n "${LGDOCK_APP:-}" ]; then
    if [ "${LGDOCK_DEBUG:-0}" = 1 ]; then
        echo "Modo applicazione: $LGDOCK_APP"
        echo "Il container si chiude quando il programma finisce."
        echo ''
    fi
    su - "$LOGIN_USER" -c "$LGDOCK_APP" &
    APP_PID=$!
    trap 'kill -TERM "$APP_PID" 2>/dev/null' INT TERM HUP
    APP_RC=0
    wait "$APP_PID" || APP_RC=$?
    # interrotto da un segnale: il programma sta chiudendo, lo si aspetta
    if kill -0 "$APP_PID" 2>/dev/null; then
        wait "$APP_PID" || APP_RC=$?
    fi
    if [ "${LGDOCK_DEBUG:-0}" = 1 ]; then
        echo ''
        echo "Programma terminato (codice $APP_RC): chiudo il container."
    fi
    exit "$APP_RC"
fi

exec su - "$LOGIN_USER"
SCRIPT_EOF

# =============================================================================
# Sostituzioni placeholder
# =============================================================================
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//HOST_USERNAME_VAR/$HOST_USERNAME}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//HOST_USER_ID_VAR/$HOST_USER_ID}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//HOST_GROUP_ID_VAR/$HOST_GROUP_ID}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//RUN_DEMO_FLAG/$RUN_DEMO}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//DEMO_NOME_VAR/$DEMO_NOME}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//DEMO_URL_VAR/$(url_demo "$DEMO_NOME")}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//HOST_USER_HOME_VAR/$HOST_USER_HOME}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//USE_SOCAT_VAR/$USE_SOCAT}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//DISPLAY_NUM_VAR/$DISPLAY_NUM}"

# =============================================================================
# Docker Run - Diverso per modalità standard vs socat
# =============================================================================
#  -it serve alla shell interattiva. In modo applicazione non c'e' nessuna
#  shell da pilotare: senza -t il runtime non pretende un terminale (si puo'
#  lanciare da un menu, da uno script, con &) e inoltra i segnali al container.
DOCKER_TTY="-it"
[[ -n "$APP_PROG" ]] && DOCKER_TTY=""

#  Da qui il container parte: lo stdout torna a vista, e un errore non fa piu'
#  stampare il file dei messaggi (a quel punto parla il container).
LG_AVVIATO=1
LGDOCK_DEBUG=0
[[ "$DEBUG" == true ]] && LGDOCK_DEBUG=1
FILTRO_PID=""
if [[ -n "$HOST_LOG" ]]; then
    exec 1>&5
    #  Gli avvisi del runtime (le righe "WARN[0000] ..." di Podman: cgroup,
    #  mount non condiviso) escono sullo stderr di questo comando. Si tolgono
    #  quelle e solo quelle: tutto il resto dello stderr - gli errori del
    #  runtime, l'avanzamento se l'immagine va scaricata, lo stderr del comando
    #  in modo applicazione - passa com'e'.
    exec 6>&2
    exec 2> >(grep --line-buffered -v '^WARN\[' >&6)
    FILTRO_PID=$!
fi

if [[ "$USE_SOCAT" == true ]]; then
    # Modalità socat: con xauth e socket bridge
#    sudo docker run --rm -it \

    $DOCKER_CMD run --rm $DOCKER_TTY \
        --platform linux/amd64 \
        -e DISPLAY=":${DISPLAY_NUM}" \
        -e XAUTHORITY="/tmp/.Xauthority" \
        -e LGDOCK_EXEC="$EXEC_PROG" \
        -e LGDOCK_APP="$APP_PROG" \
        -e LGDOCK_DEBUG="$LGDOCK_DEBUG" \
        -e LGDOCK_IMAGE="$IMAGE_NAME" \
        -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
        -v "$TEMP_XAUTH:/tmp/.Xauthority:ro" \
        -v "$HOST_USER_HOME:/host_home" \
        --network=host \
        --ipc=host \
        $IMAGE_NAME \
        bash -c "$CONTAINER_SCRIPT"
else
    # Modalità standard: X11 forwarding diretto
#    sudo docker run --rm -it \

     $DOCKER_CMD run --rm $DOCKER_TTY \
        --platform linux/amd64 \
        -e DISPLAY="$DISPLAY" \
        -e LGDOCK_EXEC="$EXEC_PROG" \
        -e LGDOCK_APP="$APP_PROG" \
        -e LGDOCK_DEBUG="$LGDOCK_DEBUG" \
        -e LGDOCK_IMAGE="$IMAGE_NAME" \
        -v /tmp/.X11-unix:/tmp/.X11-unix \
        -v "$HOST_USER_HOME:/host_home" \
        --network=host \
        $IMAGE_NAME \
        bash -c "$CONTAINER_SCRIPT"
fi
LG_RC=$?

#  Si aspetta il filtro dello stderr, altrimenti le sue ultime righe potrebbero
#  arrivare dopo il prompt.
if [[ -n "$FILTRO_PID" ]]; then
    exec 2>&6 6>&-
    wait "$FILTRO_PID" 2>/dev/null
fi
exit $LG_RC
