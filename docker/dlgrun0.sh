#!/bin/bash
#
# Lancia il container LegoPST ed apre un terminale bash
# Crea dinamicamente l'utente dell'host nel container
#

# Ottieni le informazioni dell'utente host
HOST_USERNAME=$(whoami)
HOST_USER_ID=$(id -u)
HOST_GROUP_ID=$(id -g)
HOST_USER_HOME="/home/$HOST_USERNAME"

echo "Avviando container per utente: $HOST_USERNAME (UID: $HOST_USER_ID, GID: $HOST_GROUP_ID)"
echo "Directory home host: $HOST_USER_HOME"

# Crea l'utente dinamicamente nel container e avvia la shell
docker run --rm -it \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v "$HOST_USER_HOME:/host_home" \
    aguagliardi/legopst:2.0 \
    bash -c "
        set -e
        echo '=== Configurazione utente dinamica ==='
        
        # Crea il gruppo se non esiste
        if ! getent group $HOST_GROUP_ID >/dev/null 2>&1; then
            echo 'Creando gruppo $HOST_USERNAME (GID: $HOST_GROUP_ID)'
            groupadd -g $HOST_GROUP_ID $HOST_USERNAME || echo 'Gruppo già esistente'
        fi
        
        # Crea l'utente se non esiste
        if ! getent passwd $HOST_USER_ID >/dev/null 2>&1; then
            echo 'Creando utente $HOST_USERNAME (UID: $HOST_USER_ID)'
            useradd -u $HOST_USER_ID -g $HOST_GROUP_ID -m -s /bin/bash $HOST_USERNAME || echo 'Utente già esistente'
        fi
        
        # Aggiungi l'utente ai gruppi necessari
        usermod -aG wheel,sudo $HOST_USERNAME 2>/dev/null || echo 'Gruppi wheel/sudo non disponibili'
        
        # Configura sudo senza password
        echo '$HOST_USERNAME ALL=(ALL) NOPASSWD:ALL' > /etc/sudoers.d/$HOST_USERNAME
        
        # Configura la home directory
        mkdir -p /home/$HOST_USERNAME
        chown $HOST_USER_ID:$HOST_GROUP_ID /home/$HOST_USERNAME
        
        # Crea link alla home dell'host
        su - $HOST_USERNAME -c \"
            cd /home/$HOST_USERNAME
            ln -sf /host_home host_data
            ln -sf /host_home/legocad legocad
            ln -sf /host_home/sked sked
            echo 'Link alla home host creato: /home/$HOST_USERNAME/host_data'
        \"
        
        echo '=== Avvio sessione utente ==='
        echo 'Container configurato per utente: $HOST_USERNAME'
        echo 'UID: $HOST_USER_ID, GID: $HOST_GROUP_ID'
        echo ''
        
        # Avvio shell come utente specifico
        su - $HOST_USERNAME -c \"
            echo 'Sessione utente avviata come: \$(whoami) (UID: \$(id -u), GID: \$(id -g))'
            ls -la 
            # Source del profile LegoPST se esiste
            if [ -f /home/legoroot_fedora41/.profile_legoroot ]; then
                echo 'Sourcing profile legoroot...'
                source /home/legoroot_fedora41/.profile_legoroot
            fi
            
            # Configura l'ambiente
            export HOME=/home/$HOST_USERNAME
            export USER=$HOST_USERNAME
            
            echo ''
            echo '========================================='
            echo 'Container LegoPST pronto!'
            echo 'Utente: \$(whoami)'
            echo 'Home: \$(pwd)'
            echo 'Home host: ~/host_data'
            echo '========================================='
            echo ''
            export DISPLAY=$DISPLAY 
#            cat echo source /home/legoroot_fedora41/.profile_legoroot >> .bashrc
#            exec bash -l -c source "/home/legoroot_fedora41/.profile_legoroot"
            exec bash -l 
        \"
    "

