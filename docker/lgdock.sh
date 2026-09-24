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

#  L'immagine da lanciare. Il default e' quella completa, come e' sempre
#  stato; con LG_DOCKER_IMAGE si sceglie un'altra, per esempio la variante
#  snella prodotta da "make -f Makefile.mk docker_slim":
#      LG_DOCKER_IMAGE=aguagliardi/legopst_slim:2.0 lgdock
IMAGE_NAME="${LG_DOCKER_IMAGE:-aguagliardi/legopst_multi:2.0}"


show_help() {
    local CMD_NAME=$(basename "$0")
    cat << EOF
Uso: $CMD_NAME [OPZIONI]

Opzioni:
  -h, --help          Mostra questo help
  -v, --version       Mostra versione
  -d, --demo          Installa una demo di legopst e lancia il container con essa
  -s, --socat         Usa socat per X11 forwarding (utile per SSH con MobaXterm)
  -p, --pull          Esegue docker pull dell'immagine prima di avviare il container
  -l, --slim          Usa l'immagine SNELLA, leggera (aguagliardi/legopst_slim:2.0)
                      invece di quella completa: stesso ambiente, meta' del peso

Esempi:
  $CMD_NAME                  # Lancia container LegoPST (modalità standard)
  $CMD_NAME --demo           # Installa modello demo (legocad e sked) e lancia container
  $CMD_NAME --socat          # Lancia container con socat per X11 (per SSH/MobaXterm)
  $CMD_NAME -d -s            # Demo + socat
  $CMD_NAME --slim           # Usa l'immagine snella

Modalità X11:
  - Standard (default): X11 forwarding diretto, adatto per uso locale
  - Socat (--socat): Crea socket bridge, necessario per SSH X11 forwarding

EOF
}

show_version() {
    echo "lgdock - Docker container per LegoPST v${VERSION}"
}

# =============================================================================
# Controllo Docker
# =============================================================================
if ! command -v docker >/dev/null 2>&1; then
    echo "---------------------------------------------------------------------"
    echo "WARNING: Docker appears to be missing or is not in your PATH."
    echo "         Please install Docker to create the LegoPST container."
    echo "---------------------------------------------------------------------"
    exit 1
fi

# 2. Determina se è necessario usare 'sudo'
#    Inizializziamo il comando base come 'docker'.
DOCKER_CMD="docker"

#    Il percorso standard del socket di Docker su Linux.
DOCKER_SOCKET="/var/run/docker.sock"

#    Verifichiamo se il socket esiste e se l'utente corrente NON ha permessi di scrittura (-w).
#    Se entrambe le condizioni sono vere, significa che serve 'sudo'.
if [ -S "$DOCKER_SOCKET" ] && ! [ -w "$DOCKER_SOCKET" ]; then
  echo "INFO: L'utente corrente non ha i permessi per accedere al socket di Docker."
  echo "      Verrà usato 'sudo' per eseguire i comandi Docker."
  DOCKER_CMD="sudo docker"

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
USE_SOCAT=false
DO_PULL=false

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
            ;;
        -s|--socat)
            USE_SOCAT=true
            shift
            ;;
        -p|--pull)
            DO_PULL=true
            shift
            ;;
        -l|--slim)
            IMAGE_NAME="aguagliardi/legopst_slim:2.0"
            shift
        ;;
        *)
            echo "Opzione sconosciuta: $1"
            echo "Usa --help per vedere le opzioni disponibili"
            exit 1
            ;;
    esac
done

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
echo "Demo mode: $RUN_DEMO"
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
    $DOCKER_CMD pull $IMAGE_NAME
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
            echo "Terminazione socat (PID: $SOCAT_PID)..."
            kill $SOCAT_PID 2>/dev/null
        fi
        if [ "$SOCKET_CREATED" = true ]; then
            rm -f "$SOCKET_PATH" 2>/dev/null
        fi
        rm -f "$TEMP_XAUTH" 2>/dev/null
    }
    trap cleanup EXIT
    
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
echo "Creazione link simbolici in $USER_HOME_IN_CONTAINER..."
mkdir -p "$USER_HOME_IN_CONTAINER"
ln -sf /host_home "$USER_HOME_IN_CONTAINER/host_data"

# Eventuale copia del legocad demo nella home dell'utente
if [[ "RUN_DEMO_FLAG" == "true" ]]; then
    if [ ! -d "/host_home/legopst_userstd" ]; then
        echo "=========================================="
        echo "  Installazione Demo LegoPST"
        echo "=========================================="
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
        if ! tar --no-same-owner -xvzf /home/legoroot_fedora41/demo/legopst_userstd.tgz -C "/host_home/"; then
            echo ""
            echo "ATTENZIONE: tar ha segnalato errori durante l'estrazione."
            echo "Se sono del tipo \"Cannot change mode\" sono innocui: i file ci"
            echo "sono, ma il filesystem dell'host non accetta i permessi."
            echo "Proseguo con l'installazione."
            echo ""
        fi
        # Stessa ragione dell'"if" sopra: anche il chown puo' fallire su quei
        # filesystem, e nudo sotto `set -e` ammazzerebbe l'installazione un
        # attimo prima dei link.
        if ! chown -R "$CONT_UID:$CONT_GID" "/host_home/legopst_userstd"; then
            echo ""
            echo "ATTENZIONE: chown -R fallito su /host_home/legopst_userstd."
            echo "I file restano come li ha scritti tar. Proseguo."
            echo ""
        fi
        
        echo ""
        echo "Contenuto directory demo:"
        echo "- legocad:"
        ls -la /host_home/legopst_userstd/legocad | head -10
        echo "- sked:"
        ls -la /host_home/legopst_userstd/sked | head -10
        
        # Link simbolici, RELATIVI. Con il target assoluto (/host_home/...)
        # funzionerebbero solo qui dentro: /host_home e' il nome che la home ha
        # nel container, sull'host non esiste e i due link resterebbero
        # penzolanti. Relativi si risolvono giusti in tutti e due i mondi:
        # ~/legopst_userstd/legocad sull'host, /host_home/legopst_userstd/legocad
        # qui. Ed e' anche la convenzione delle installazioni native.
        ln -sfn legopst_userstd/legocad /host_home/legocad 2>/dev/null || true
        ln -sfn legopst_userstd/sked /host_home/sked 2>/dev/null || true
        chown -h "$CONT_UID:$CONT_GID" /host_home/legocad 2>/dev/null || true
        chown -h "$CONT_UID:$CONT_GID" /host_home/sked 2>/dev/null || true
        
        echo "=========================================="
        echo "  Demo installata in: HOST_USER_HOME_VAR/legopst_userstd"
        echo "=========================================="
        echo ""
    else
        echo "Demo già installata in /host_home/legopst_userstd"
        # Se e' stata installata da una lgdock precedente sotto rootless, e'
        # intestata all'UID sbagliato e sull'host non si apre nemmeno.
        DEMO_UID=$(stat -c %u /host_home/legopst_userstd/legocad 2>/dev/null || true)
        if [ -n "$DEMO_UID" ] && [ "$DEMO_UID" != "$CONT_UID" ]; then
            echo ""
            echo "ATTENZIONE: la demo presente risulta dell'UID $DEMO_UID, non $CONT_UID."
            echo "L'ha installata una versione di lgdock che non riconosceva questa"
            echo "mappatura: sull'host non e' utilizzabile. Per rifarla, da una"
            echo "shell dell'host:"
            echo "    rm -rf ~/legopst_userstd ~/legocad ~/sked"
            echo "    lgrun -d"
            echo ""
        fi
    fi
fi

# Link simbolici finali
ln -sf /host_home/legocad "$USER_HOME_IN_CONTAINER/legocad" 2>/dev/null || true
ln -sf /host_home/sked "$USER_HOME_IN_CONTAINER/sked" 2>/dev/null || true
ln -sf /host_home/defaults "$USER_HOME_IN_CONTAINER/defaults" 2>/dev/null || true
chown -h "$CONT_UID:$CONT_GID" "$USER_HOME_IN_CONTAINER/host_data"

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
    echo "echo \"DISPLAY impostato a: \$DISPLAY\""
    echo ""
    echo "# Prompt personalizzato LegoPST"
    echo "export PS1='LegoPST@\w \$ '"
    echo ""
    echo "# Sorgente del profilo custom LegoPST"
    echo "if [ -f \"$PROFILE_LEGOROOT_PATH\" ]; then"
    echo "    source \"$PROFILE_LEGOROOT_PATH\""
    echo "fi"
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
echo '======================================================================='
echo ''

exec su - "$LOGIN_USER"
SCRIPT_EOF

# =============================================================================
# Sostituzioni placeholder
# =============================================================================
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//HOST_USERNAME_VAR/$HOST_USERNAME}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//HOST_USER_ID_VAR/$HOST_USER_ID}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//HOST_GROUP_ID_VAR/$HOST_GROUP_ID}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//RUN_DEMO_FLAG/$RUN_DEMO}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//HOST_USER_HOME_VAR/$HOST_USER_HOME}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//USE_SOCAT_VAR/$USE_SOCAT}"
CONTAINER_SCRIPT="${CONTAINER_SCRIPT//DISPLAY_NUM_VAR/$DISPLAY_NUM}"

# =============================================================================
# Docker Run - Diverso per modalità standard vs socat
# =============================================================================
if [[ "$USE_SOCAT" == true ]]; then
    # Modalità socat: con xauth e socket bridge
#    sudo docker run --rm -it \

    $DOCKER_CMD run --rm -it \
        --platform linux/amd64 \
        -e DISPLAY=":${DISPLAY_NUM}" \
        -e XAUTHORITY="/tmp/.Xauthority" \
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

     $DOCKER_CMD run --rm -it \
        --platform linux/amd64 \
        -e DISPLAY="$DISPLAY" \
        -v /tmp/.X11-unix:/tmp/.X11-unix \
        -v "$HOST_USER_HOME:/host_home" \
        --network=host \
        $IMAGE_NAME \
        bash -c "$CONTAINER_SCRIPT"
fi
