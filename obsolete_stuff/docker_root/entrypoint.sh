#!/bin/bash
#set -e # Esce immediatamente se un comando fallisce
#set -x

echo "Ora eseguo entrypoint"
cat /home/contuser/.bashrc
#echo "source /home/legoroot_fedora41/.profile_legoroot" >> /home/contuser/.bashrc
echo "altro cat"
cat /home/contuser/.bashrc
echo "fine cat /home/contuser/.bashrc"

# Valori di default se PUID/PGID non sono passati
DEFAULT_UID=1000
DEFAULT_GID=1000

# Prendi PUID/PGID dalle variabili d'ambiente o usa valori di default
# Se il volume è montato in /data, controlla il proprietario di /data
# Altrimenti, l'utente deve passare PUID/PGID
TARGET_UID=${PUID:-$(stat -c "%u" /home/contuser 2>/dev/null || echo $DEFAULT_UID)}
TARGET_GID=${PGID:-$(stat -c "%g" /home/contuser 2>/dev/null || echo $DEFAULT_GID)}
USERNAME="contuser"
USER_HOME="/home/$USERNAME"

# Controlla se il gruppo esiste con il GID desiderato, altrimenti crealo
if ! getent group "$TARGET_GID" > /dev/null 2>&1; then
    if getent group "$USERNAME" > /dev/null 2>&1; then
        echo "Gruppo '$USERNAME' esiste con GID diverso. Lo modifico in $TARGET_GID."
        groupmod -g "$TARGET_GID" "$USERNAME"
    else
        echo "Creazione gruppo '$USERNAME' con GID $TARGET_GID."
        groupadd -g "$TARGET_GID" "$USERNAME"
    fi
else
    # Il GID esiste, controlliamo se è già il nostro USERNAME
    EXISTING_GROUP_NAME=$(getent group "$TARGET_GID" | cut -d: -f1)
    if [ "$EXISTING_GROUP_NAME" != "$USERNAME" ]; then
        echo "Attenzione: GID $TARGET_GID è già usato dal gruppo '$EXISTING_GROUP_NAME'."
        echo "Procedo comunque a creare/modificare l'utente '$USERNAME' con questo GID."
        # Potrebbe essere necessario creare un gruppo con un nome diverso se questo è un problema
        # o assicurarsi che l'utente sia aggiunto al gruppo esistente se appropriato.
        # Per semplicità, qui procediamo.
        if getent group "$USERNAME" > /dev/null 2>&1; then
             groupmod -g "$TARGET_GID" "$USERNAME" # Modifica GID del gruppo 'devuser'
        else
             groupadd -g "$TARGET_GID" "$USERNAME" # Crea gruppo 'devuser' con GID target
        fi
    fi
fi


# Controlla se l'utente esiste con l'UID desiderato, altrimenti crealo/modificalo
if ! getent passwd "$TARGET_UID" > /dev/null 2>&1; then
    if getent passwd "$USERNAME" > /dev/null 2>&1; then
        echo "Utente '$USERNAME' esiste con UID diverso. Lo modifico in $TARGET_UID e GID $TARGET_GID."
        usermod -u "$TARGET_UID" -g "$TARGET_GID" -d "$USER_HOME" -md "$USERNAME"
    else
        echo "Creazione utente '$USERNAME' con UID $TARGET_UID, GID $TARGET_GID e home $USER_HOME."
        useradd -u "$TARGET_UID" -g "$TARGET_GID" -ms /bin/bash -d "$USER_HOME" "$USERNAME"
    fi
else
    # L'UID esiste. Verifichiamo se è il nostro USERNAME.
    EXISTING_USER_NAME=$(getent passwd "$TARGET_UID" | cut -d: -f1)
    if [ "$EXISTING_USER_NAME" != "$USERNAME" ]; then
        echo "Attenzione: UID $TARGET_UID è già usato dall'utente '$EXISTING_USER_NAME'."
        echo "Questo potrebbe causare conflitti. Considera di usare un UID diverso per l'host o il container."
        # Per ora, per semplicità, non facciamo nulla e lasciamo che runuser usi l'UID esistente
        # ma questo non è ideale. La soluzione migliore sarebbe fallire o avere una logica più complessa.
        # Oppure, se siamo sicuri, potremmo provare a *modificare* l'utente 'devuser' perché abbia questo UID,
        # ma solo se 'devuser' è l'unico utente che ci interessa con quell'UID.
        if getent passwd "$USERNAME" > /dev/null 2>&1; then
            echo "Modifico l'utente '$USERNAME' per usare UID $TARGET_UID e GID $TARGET_GID."
            usermod -u "$TARGET_UID" -g "$TARGET_GID" "$USERNAME"
        else
            # Se 'devuser' non esiste ma l'UID sì, potremmo avere un problema
            echo "ERRORE: UID $TARGET_UID esiste ma non è $USERNAME, e $USERNAME non esiste per essere modificato."
            exit 1
        fi
    else
        # L'UID esiste ed è già il nostro USERNAME, assicuriamoci che il GID e la home siano corretti
        echo "Utente '$USERNAME' (UID $TARGET_UID) trovato. Verifico GID e home."
        usermod -g "$TARGET_GID" -d "$USER_HOME" "$USERNAME"
    fi
fi

# Assicura che la directory home esista e abbia i permessi corretti
# (useradd -md dovrebbe già farlo, ma una doppia verifica non fa male)
mkdir -p "$USER_HOME"
chown "$TARGET_UID:$TARGET_GID" "$USER_HOME"

# Esegui il comando passato (dalla direttiva CMD del Dockerfile o da docker run)
# come l'utente specificato.
# runuser -l <utente> -c '<comando>' esegue il comando in una shell di login.
# Per eseguire direttamente l'eseguibile senza una shell di login completa, ma con l'ambiente
# dell'utente, si può usare:
# exec runuser -u "$USERNAME" -- "$@"
# Se "$@" è "bash -c 'source ... && exec bash -l'", questo potrebbe creare una doppia shell.
# È più semplice se CMD è solo ["bash", "-l"] e il sourcing del profilo avviene qui o nel .bashrc dell'utente.

echo "Esecuzione del comando come utente '$USERNAME' (UID $TARGET_UID, GID $TARGET_GID): $@"

# Se il comando passato è già una shell o un comando che gestisce il proprio ambiente:
# exec runuser -u "$USERNAME" -- "$@"
# Se vuoi forzare una shell di login che esegua il comando passato ($@):
# Questo è più vicino a `gosu user command` se il comando è semplice.
# Se "$@" è complesso (es. "bash -c '... && exec bash -l'"), questo lo avvolge.

### exec runuser -l "$USERNAME" -s /bin/bash -P -- "$@"
exec runuser -l "$USERNAME" -s /bin/bash -P -l


###  exec runuser -l "$USERNAME" -c "exec \"\$@\"" -- "$@"
# La parte ` -- "$@"` dopo il comando -c è per passare gli argomenti in modo sicuro a "$@"
# all'interno della shell avviata da -c.