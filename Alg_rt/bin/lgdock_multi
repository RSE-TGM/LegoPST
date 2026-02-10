#!/bin/bash
#
# lgdock versione Multipiattaforma e lgdock_unified.sh
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
IMAGE_NAME = aguagliardi/legopst_multi:2.0

show_help() {
    cat << EOF
Uso: $0 [OPZIONI]

Opzioni:
  -h, --help          Mostra questo help
  -v, --version       Mostra versione
  -d, --demo          Installa una demo di legopst e lancia il container con essa
  -s, --socat         Usa socat per X11 forwarding (utile per SSH con MobaXterm)

Esempi:
  $0                  # Lancia container LegoPST (modalità standard)
  $0 --demo           # Installa modello demo (legocad e sked) e lancia container
  $0 --socat          # Lancia container con socat per X11 (per SSH/MobaXterm)
  $0 -d -s            # Demo + socat

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
echo "Demo mode: $RUN_DEMO"
echo "Utente: $HOST_USERNAME (UID: $HOST_USER_ID, GID: $HOST_GROUP_ID)"
echo "Home directory: $HOST_USER_HOME"
echo "DISPLAY: $DISPLAY"
echo ""

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

# Copia xauth per l'utente (solo in modalità socat)
if [[ "USE_SOCAT_VAR" == "true" ]]; then
    mkdir -p "$USER_HOME_IN_CONTAINER"
    cp /tmp/.Xauthority "$USER_HOME_IN_CONTAINER/.Xauthority" 2>/dev/null || touch "$USER_HOME_IN_CONTAINER/.Xauthority"
    chown "HOST_USER_ID_VAR:HOST_GROUP_ID_VAR" "$USER_HOME_IN_CONTAINER/.Xauthority"
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
        tar xvfz /home/legoroot_fedora41/demo/legopst_userstd.tgz -C "/host_home/"
        chown -R "HOST_USER_ID_VAR:HOST_GROUP_ID_VAR" "/host_home/legopst_userstd"
        
        echo ""
        echo "Contenuto directory demo:"
        echo "- legocad:"
        ls -la /host_home/legopst_userstd/legocad | head -10
        echo "- sked:"
        ls -la /host_home/legopst_userstd/sked | head -10
        
        # Crea link simbolici
        ln -sf /host_home/legopst_userstd/legocad /host_home/legocad 2>/dev/null || true
        ln -sf /host_home/legopst_userstd/sked /host_home/sked 2>/dev/null || true
        chown "HOST_USER_ID_VAR:HOST_GROUP_ID_VAR" /host_home/legocad 2>/dev/null || true
        chown "HOST_USER_ID_VAR:HOST_GROUP_ID_VAR" /host_home/sked 2>/dev/null || true
        
        echo "=========================================="
        echo "  Demo installata in: HOST_USER_HOME_VAR/legopst_userstd"
        echo "=========================================="
        echo ""
    else
        echo "Demo già installata in /host_home/legopst_userstd"
    fi
fi

# Link simbolici finali
ln -sf /host_home/legocad "$USER_HOME_IN_CONTAINER/legocad" 2>/dev/null || true
ln -sf /host_home/sked "$USER_HOME_IN_CONTAINER/sked" 2>/dev/null || true
ln -sf /host_home/defaults "$USER_HOME_IN_CONTAINER/defaults" 2>/dev/null || true
chown -h "HOST_USER_ID_VAR:HOST_GROUP_ID_VAR" "$USER_HOME_IN_CONTAINER/host_data"

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
    echo "# Sorgente del profilo custom LegoPST"
    echo "if [ -f \"$PROFILE_LEGOROOT_PATH\" ]; then"
    echo "    source \"$PROFILE_LEGOROOT_PATH\""
    echo "fi"
} > "$BASH_PROFILE_PATH"

chown "HOST_USER_ID_VAR:HOST_GROUP_ID_VAR" "$BASH_PROFILE_PATH"

echo '======================================================================='
echo '  Container Pronto'
echo '======================================================================='
echo "Utente: HOST_USERNAME_VAR"
echo "UID: HOST_USER_ID_VAR, GID: HOST_GROUP_ID_VAR"
echo "DISPLAY: $DISPLAY_VALUE"
[[ "USE_SOCAT_VAR" == "true" ]] && echo "X11 Mode: socat bridge" || echo "X11 Mode: standard"
echo '======================================================================='
echo ''

exec su - "HOST_USERNAME_VAR"
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
        -e DISPLAY="$DISPLAY" \
        -v /tmp/.X11-unix:/tmp/.X11-unix \
        -v "$HOST_USER_HOME:/host_home" \
        --network=host \
        $IMAGE_NAME \
        bash -c "$CONTAINER_SCRIPT"
fi
