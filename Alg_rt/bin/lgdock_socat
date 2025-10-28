#!/bin/bash
#
# Lancia il container LegoPST ed apre un terminale bash
# Crea dinamicamente l'utente dell'host nel container
# Versione con socat

# Ottieni le informazioni dell'utente host
HOST_USERNAME=$(whoami)
HOST_USER_ID=$(id -u)
HOST_GROUP_ID=$(id -g)
HOST_USER_HOME="$HOME" 

#eventuale creazione di /defaults
HOST_DEFAULTS_DIR="$HOME/defaults"
echo "=== Controllo della directory: $HOST_DEFAULTS_DIR ==="
if [[ -d "$HOST_DEFAULTS_DIR" ]]; then
    echo "La directory '$HOST_DEFAULTS_DIR' esiste già."
else
    echo "La directory '$HOST_DEFAULTS_DIR' NON esiste. La creo"
    mkdir -p "$HOST_DEFAULTS_DIR"
fi



echo "Avviando container per utente: $HOST_USERNAME (UID: $HOST_USER_ID, GID: $HOST_GROUP_ID)"
echo "Directory home host: $HOST_USER_HOME"
echo "DISPLAY dell'host: $DISPLAY"

# Estrai numero display
DISPLAY_NUM=$(echo $DISPLAY | sed 's/.*:\([0-9]*\).*/\1/')
SOCKET_PATH="/tmp/.X11-unix/X${DISPLAY_NUM}"

# Crea socket bridge se non esiste
if [ ! -S "$SOCKET_PATH" ]; then
    echo "Creando socket bridge X11 su $SOCKET_PATH..."
    socat UNIX-LISTEN:$SOCKET_PATH,fork,mode=777 TCP:localhost:$((6000 + DISPLAY_NUM)) &
    SOCAT_PID=$!
    sleep 1
    chmod 777 $SOCKET_PATH
fi

# Estrai cookie X11 e crea file xauth temporaneo
XAUTH_COOKIE=$(xauth list $DISPLAY | head -n1 | awk '{print $3}')
TEMP_XAUTH="/tmp/.docker_xauth_$$"
touch "$TEMP_XAUTH"
chmod 600 "$TEMP_XAUTH"

if [ -n "$XAUTH_COOKIE" ]; then
    xauth -f "$TEMP_XAUTH" add ":${DISPLAY_NUM}" . "$XAUTH_COOKIE"
    echo "X11 authentication configurata per display :${DISPLAY_NUM}"
fi

# Permetti connessioni locali
xhost +local:all 2>/dev/null

# Cleanup alla fine
trap "kill $SOCAT_PID 2>/dev/null; rm -f $SOCKET_PATH $TEMP_XAUTH" EXIT

# Avvia container
docker run --rm -it \
    --platform linux/amd64 \
    -e DISPLAY=":${DISPLAY_NUM}" \
    -e XAUTHORITY="/tmp/.Xauthority" \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    -v "$TEMP_XAUTH:/tmp/.Xauthority:ro" \
    -v "$HOST_USER_HOME:/host_home" \
    --network=host \
    --ipc=host \
    aguagliardi/legopst:2.0 \
    bash -c "
        set -e

        USER_HOME_IN_CONTAINER=\"/home/$HOST_USERNAME\"

        echo '=== Configurazione utente dinamica (UID: $HOST_USER_ID, GID: $HOST_GROUP_ID, Utente: $HOST_USERNAME) ==='

        # Crea gruppo
        if ! getent group \"$HOST_GROUP_ID\" >/dev/null 2>&1; then
            groupadd -g \"$HOST_GROUP_ID\" \"$HOST_USERNAME\"
        else
            EXISTING_GROUP_NAME=\$(getent group \"$HOST_GROUP_ID\" | cut -d: -f1)
            if [ \"\$EXISTING_GROUP_NAME\" != \"$HOST_USERNAME\" ]; then
                if getent group \"$HOST_USERNAME\" >/dev/null 2>&1; then 
                    groupmod -g \"$HOST_GROUP_ID\" \"$HOST_USERNAME\"
                else 
                    groupadd -g \"$HOST_GROUP_ID\" \"$HOST_USERNAME\"
                fi
            fi
        fi

        # Crea utente
        if ! getent passwd \"$HOST_USER_ID\" >/dev/null 2>&1; then
            useradd -u \"$HOST_USER_ID\" -g \"$HOST_GROUP_ID\" -m -d \"\$USER_HOME_IN_CONTAINER\" -s /bin/bash \"$HOST_USERNAME\"
        else
            EXISTING_USER_WITH_UID=\$(getent passwd \"$HOST_USER_ID\" | cut -d: -f1)
            if [ \"\$EXISTING_USER_WITH_UID\" != \"$HOST_USERNAME\" ]; then
                echo \"ERRORE: UID $HOST_USER_ID già usato dall'utente '\$EXISTING_USER_WITH_UID'\"
                exit 1
            else
                usermod -g \"$HOST_GROUP_ID\" -d \"\$USER_HOME_IN_CONTAINER\" -s /bin/bash -md \"$HOST_USERNAME\"
            fi
        fi

        # Configurazione sudo
        SUDOERS_FILE_PATH=\"/etc/sudoers.d/${HOST_USERNAME}\"
        echo \"${HOST_USERNAME} ALL=(ALL) NOPASSWD:ALL\" > \"\${SUDOERS_FILE_PATH}\"
        chmod 0440 \"\${SUDOERS_FILE_PATH}\"

        # Copia xauth per l'utente
        mkdir -p \"\$USER_HOME_IN_CONTAINER\"
        cp /tmp/.Xauthority \"\$USER_HOME_IN_CONTAINER/.Xauthority\" 2>/dev/null || touch \"\$USER_HOME_IN_CONTAINER/.Xauthority\"
        chown \"$HOST_USER_ID:$HOST_GROUP_ID\" \"\$USER_HOME_IN_CONTAINER/.Xauthority\"
        chmod 600 \"\$USER_HOME_IN_CONTAINER/.Xauthority\"

        # Link simbolici
        echo \"Creazione link simbolici in \$USER_HOME_IN_CONTAINER...\"
        ln -sf /host_home \"\$USER_HOME_IN_CONTAINER/host_data\"
        ln -sf /host_home/legocad \"\$USER_HOME_IN_CONTAINER/legocad\" 2>/dev/null || true
        ln -sf /host_home/sked \"\$USER_HOME_IN_CONTAINER/sked\" 2>/dev/null || true
        ln -sf /host_home/defaults \"\$USER_HOME_IN_CONTAINER/defaults\" 2>/dev/null || true
        chown -h \"$HOST_USER_ID:$HOST_GROUP_ID\" \"\$USER_HOME_IN_CONTAINER/host_data\"

        # Configurazione .bash_profile
        BASH_PROFILE_PATH=\"\$USER_HOME_IN_CONTAINER/.bash_profile\"
        PROFILE_LEGOROOT_PATH=\"/home/legoroot_fedora41/.profile_legoroot\"

        {
            echo \"# File .bash_profile per $HOST_USERNAME\";
            echo \"if [ -f ~/.bashrc ]; then . ~/.bashrc; fi\";
            echo \"\";
            echo \"export DISPLAY=:${DISPLAY_NUM}\";
            echo \"export XAUTHORITY=\\\"\\\$HOME/.Xauthority\\\"\";
            echo \"echo \\\"DISPLAY impostato a: \\\$DISPLAY\\\"\";
            echo \"\";
            echo \"# Sorgente del profilo custom LegoPST\";
            echo \"if [ -f \\\"\$PROFILE_LEGOROOT_PATH\\\" ]; then\";
            echo \"    echo 'Sourcing profile legoroot da .bash_profile...'\";
            echo \"    source \\\"\$PROFILE_LEGOROOT_PATH\\\"\";
            echo \"fi\";
        } > \"\$BASH_PROFILE_PATH\"

        chown \"$HOST_USER_ID:$HOST_GROUP_ID\" \"\$BASH_PROFILE_PATH\"

        echo '=== Avvio sessione utente ==='
        echo \"Container configurato per utente: $HOST_USERNAME\"
        echo \"UID: $HOST_USER_ID, GID: $HOST_GROUP_ID\"
        echo \"X11 forwarding: abilitato su display :${DISPLAY_NUM}\"
        echo ''

        exec su - \"$HOST_USERNAME\"
    "

