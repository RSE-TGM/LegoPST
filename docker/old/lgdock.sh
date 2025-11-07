#!/bin/bash
#
# Lancia il container LegoPST ed apre un terminale bash
# Crea dinamicamente l'utente dell'host nel container
#

# Controlla se Docker è installato
if ! command -v docker >/dev/null 2>&1; then
    echo "---------------------------------------------------------------------"
    echo "WARNING: Docker appears to be missing or is not in your PATH."
    echo "         Please install Docker to create the LegoPST container. "
    echo "---------------------------------------------------------------------"
    exit 0
fi


# Ottieni le informazioni dell'utente host
HOST_USERNAME=$(whoami)
HOST_USER_ID=$(id -u)
HOST_GROUP_ID=$(id -g)
#HOST_USER_HOME="/home/$HOST_USERNAME" # Home dell'utente SULL'HOST
HOST_USER_HOME="$HOME" # Home dell'utente SULL'HOST
DESIRED_USER_PASSWORD=$HOST_USERNAME
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
echo "DISPLAY dell'host che verr passato: $DISPLAY"

# Crea l'utente dinamicamente nel container e avvia la shell
docker run --rm -it \
    --platform linux/amd64 \
    -e DISPLAY="$DISPLAY" \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v "$HOST_USER_HOME:/host_home" \
    --network=host \
    aguagliardi/legopst:2.0 \
    bash -c "
        set -e # Esce immediatamente se un comando fallisce
        
        # Cattura il valore di DISPLAY passato con -e all'inizio dello script.
        # Le variabili HOST_* sono espanse dalla shell host prima di 'docker run'.
        # DISPLAY_FROM_ENV è il valore che Docker ha impostato per questo script.
        DISPLAY_FROM_ENV=\"\$DISPLAY\" 

        echo '=== Configurazione utente dinamica (UID: $HOST_USER_ID, GID: $HOST_GROUP_ID, Utente: $HOST_USERNAME) ==='
        
        USER_HOME_IN_CONTAINER=\"/home/$HOST_USERNAME\"

        # Crea il gruppo se non esiste con il GID dell'host
        if ! getent group \"$HOST_GROUP_ID\" >/dev/null 2>&1; then
            echo \"Creando gruppo '$HOST_USERNAME' (GID: $HOST_GROUP_ID)\"
            groupadd -g \"$HOST_GROUP_ID\" \"$HOST_USERNAME\"
        else
            EXISTING_GROUP_NAME=\$(getent group \"$HOST_GROUP_ID\" | cut -d: -f1)
            if [ \"\$EXISTING_GROUP_NAME\" != \"$HOST_USERNAME\" ]; then
                echo \"Attenzione: GID $HOST_GROUP_ID è già usato dal gruppo '\$EXISTING_GROUP_NAME'. Modifico il gruppo '$HOST_USERNAME' se esiste, altrimenti lo creo.\"
                if getent group \"$HOST_USERNAME\" >/dev/null 2>&1; then groupmod -g \"$HOST_GROUP_ID\" \"$HOST_USERNAME\"; else groupadd -g \"$HOST_GROUP_ID\" \"$HOST_USERNAME\"; fi
            else
                 echo \"Gruppo '$HOST_USERNAME' con GID $HOST_GROUP_ID già esistente o GID $HOST_GROUP_ID già associato a '$HOST_USERNAME'.\"
            fi
        fi
        
        # Crea l'utente se non esiste con l'UID dell'host
        if ! getent passwd \"$HOST_USER_ID\" >/dev/null 2>&1; then
            echo \"Creando utente '$HOST_USERNAME' (UID: $HOST_USER_ID, GID: $HOST_GROUP_ID)\"
            useradd -u \"$HOST_USER_ID\" -g \"$HOST_GROUP_ID\" -m -d \"\$USER_HOME_IN_CONTAINER\" -s /bin/bash \"$HOST_USERNAME\"
            # # Imposta la password per il nuovo utente
            # echo \"Impostazione password per l'utente '$HOST_USERNAME'...\"
            # echo \"$HOST_USERNAME:\$DESIRED_PASSWORD\" | chpasswd
            # echo \"Password impostata: $DESIRED_PASSWORD\"
        else
            EXISTING_USER_WITH_UID=\$(getent passwd \"$HOST_USER_ID\" | cut -d: -f1)
            if [ \"\$EXISTING_USER_WITH_UID\" != \"$HOST_USERNAME\" ]; then
                echo \"ERRORE: UID $HOST_USER_ID è già usato dall'utente '\$EXISTING_USER_WITH_UID'. Impossibile procedere.\"
                exit 1
            else
                echo \"Utente '$HOST_USERNAME' (UID $HOST_USER_ID) trovato. Aggiorno GID, home e shell.\"
                usermod -g \"$HOST_GROUP_ID\" -d \"\$USER_HOME_IN_CONTAINER\" -s /bin/bash -md \"$HOST_USERNAME\"
            fi
        fi

        # --- INIZIO BLOCCO SUDO ---
        SUDOERS_FILE_CONTENT=\"${HOST_USERNAME} ALL=(ALL) NOPASSWD:ALL\"
        SUDOERS_FILE_PATH=\"/etc/sudoers.d/${HOST_USERNAME}\"

        echo \"Configurazione sudo per ${HOST_USERNAME} in \${SUDOERS_FILE_PATH}...\"
        printf \"%s\\n\" \"\${SUDOERS_FILE_CONTENT}\" > \"\${SUDOERS_FILE_PATH}\"
        chmod 0440 \"\${SUDOERS_FILE_PATH}\"
        echo \"Contenuto di \${SUDOERS_FILE_PATH}:\"
        cat \"\${SUDOERS_FILE_PATH}\"
        echo \"Permessi di \${SUDOERS_FILE_PATH}:\"
        ls -l \"\${SUDOERS_FILE_PATH}\"
        # --- FINE BLOCCO SUDO ---

        echo \"Creazione link simbolici in \$USER_HOME_IN_CONTAINER...\"
        mkdir -p \"\$USER_HOME_IN_CONTAINER\" # Assicurati che esista
        ln -sf /host_home \"\$USER_HOME_IN_CONTAINER/host_data\"
        ln -sf /host_home/legocad  \"\$USER_HOME_IN_CONTAINER/legocad\"   # se esiste viene creato un link per legocad       
        ln -sf /host_home/sked  \"\$USER_HOME_IN_CONTAINER/sked\"  # se esiste viene creato un link per sked
        ln -sf /host_home/defaults  \"\$USER_HOME_IN_CONTAINER/defaults\"  # se esiste viene creato un link per defaults
        chown -h \"$HOST_USER_ID:$HOST_GROUP_ID\" \"\$USER_HOME_IN_CONTAINER/host_data\"
        
        # Aggiungi il comando source e l'esportazione di DISPLAY a .bash_profile dell'utente
        BASH_PROFILE_PATH=\"\$USER_HOME_IN_CONTAINER/.bash_profile\"
        PROFILE_LEGOROOT_PATH=\"/home/legoroot_fedora41/.profile_legoroot\"

        echo \"Configurazione di \$BASH_PROFILE_PATH...\"
        {
            echo \"# File .bash_profile per $HOST_USERNAME\";
            echo \"# Carica .bashrc se esiste\";
            echo \"if [ -f ~/.bashrc ]; then\";
            echo \"    . ~/.bashrc\";
            echo \"fi\";
            echo \"\";
            echo \"# Esporta la variabile DISPLAY passata al container\";
            echo \"if [ -n \\\"\$DISPLAY_FROM_ENV\\\" ]; then\"; # Usa la variabile catturata
            echo \"  export DISPLAY=\\\"\$DISPLAY_FROM_ENV\\\"\"; # Doppio escape per le virgolette del valore
            echo \"  echo \\\"DISPLAY impostato a: \$DISPLAY (da .bash_profile)\\\"\";
            echo \"else\";
            echo \"  echo \\\"Attenzione: DISPLAY_FROM_ENV non era impostato per .bash_profile.\\\"\";
            echo \"fi\";
            echo \"\";
            echo \"# Sorgente del profilo custom LegoPST\";
            echo \"if [ -f \\\"\$PROFILE_LEGOROOT_PATH\\\" ]; then\";
            echo \"    echo 'Sourcing profile legoroot da .bash_profile...'\" ;
            echo \"    source \\\"\$PROFILE_LEGOROOT_PATH\\\"\";
            echo \"fi\";
        } > \"\$BASH_PROFILE_PATH\" # Sovrascrive o crea il file
        
        chown \"$HOST_USER_ID:$HOST_GROUP_ID\" \"\$BASH_PROFILE_PATH\"
        
        echo '=== Avvio sessione utente ==='
        echo \"Container configurato per utente: $HOST_USERNAME\"
        echo \"UID: $HOST_USER_ID, GID: $HOST_GROUP_ID\"
        echo \"DISPLAY che dovrebbe essere usato: \$DISPLAY_FROM_ENV\"
        echo ''
            # echo \"Impostazione password per l'utente '$HOST_USERNAME'...\"
            # echo \"$HOST_USERNAME:\$DESIRED_USER_PASSWORD\" | chpasswd
            # echo \"Password impostata: $DESIRED_USER_PASSWORD\"
        
        exec su - \"$HOST_USERNAME\"
    "


