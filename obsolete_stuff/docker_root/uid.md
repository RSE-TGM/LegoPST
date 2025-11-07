Hai centrato perfettamente il problema comune della proprietà dei file quando si montano volumi da host a container Docker, specialmente quando il container esegue processi come utente `root`.

**Il Problema: UID/GID Mismatch**

*   All'interno del container, per impostazione predefinita, molti processi (inclusa la shell che avvii) vengono eseguiti come utente `root` (UID 0, GID 0).
*   Quando questo utente `root` del container crea o modifica file nel volume montato (che punta a `/home/antonio` sul tuo host), i file vengono creati con proprietario UID 0 e GID 0 *dal punto di vista del kernel Linux condiviso*.
*   Sul tuo sistema host, l'utente `antonio` ha un UID e GID specifici (es. 1000:1000).
*   Quindi, i file creati da `root` nel container appaiono come appartenenti a `root:root` sull'host, anche se si trovano nella tua directory `/home/antonio`.

**Soluzioni:**

Non esiste una singola opzione "magica" in `docker run` o `docker-compose` che risolva questo problema in modo completamente automatico e trasparente per tutti i casi, ma ci sono diverse strategie efficaci:

**1. Eseguire il Container con l'UID/GID dell'Utente Host (Opzione `-u` o `user`):**

Questo è spesso l'approccio più pulito e diretto.

*   **Con `docker run`:**
    Puoi usare l'opzione `-u` (o `--user`) per specificare l'UID e il GID con cui eseguire il comando principale nel container.
    ```bash
    docker run --rm -it \
      -e DISPLAY=$DISPLAY \
      -v /tmp/.X11-unix:/tmp/.X11-unix \
      -v /home/antonio:/home/antonio_container \  # Mappa in una dir diversa da /root
      -u "$(id -u):$(id -g)" \                   # Esegui come utente host
      --workdir /home/antonio_container \        # Imposta la workdir
      aguagliardi/legopst:latest \
      bash -c "source /home/legoroot_fedora41/.profile_legoroot && exec bash -l"
    ```
    **Spiegazione:**
    *   **`-v /home/antonio:/home/antonio_container`**: Ho cambiato il punto di montaggio all'interno del container da `/root` a `/home/antonio_container` (o qualsiasi altro nome che preferisci, come `/app` o `/workspace`). Questo perché la directory `/root` è la home directory dell'utente root e potrebbe avere permessi restrittivi o essere usata per altri scopi dal sistema base del container. È meglio mappare la tua home in una directory dedicata.
    *   **`-u "$(id -u):$(id -g)"`**:
        *   `id -u`: Ottiene l'UID del tuo utente corrente sull'host.
        *   `id -g`: Ottiene il GID del tuo utente corrente sull'host.
        *   L'opzione `-u UID:GID` dice a Docker di eseguire il processo principale nel container con questo UID e GID.
    *   **`--workdir /home/antonio_container`**: Imposta la directory di lavoro all'avvio della shell nella directory che hai montato. Quindi il `cd /root` non è più necessario nel comando bash.
    *   Il comando bash ora potrebbe fare riferimento ai file nel nuovo punto di montaggio se necessario.

*   **Con `docker-compose.yml`:**
    ```yaml
    version: '3.8' # Obsoleto, ma ok se presente

    services:
      legopst_interactive:
        image: aguagliardi/legopst:latest
        container_name: legopst_shell_container
        stdin_open: true
        tty: true
        user: "${UID_HOST}:${GID_HOST}" # Usa variabili d'ambiente per UID/GID
        working_dir: /home/antonio_container # O il nome che preferisci
        environment:
          - DISPLAY=${DISPLAY}
          # HOME: /home/antonio_container # Potrebbe essere utile impostare HOME
        volumes:
          - "/tmp/.X11-unix:/tmp/.X11-unix"
          - "/home/antonio:/home/antonio_container" # Mappa alla nuova directory
        command: >
          bash -c "
          if [ -f /home/legoroot_fedora41/.profile_legoroot ]; then
            echo 'Sourcing /home/legoroot_fedora41/.profile_legoroot...';
            source /home/legoroot_fedora41/.profile_legoroot;
          else
            echo 'Attenzione: /home/legoroot_fedora41/.profile_legoroot non trovato nel container.';
          fi;
          echo 'Avvio della shell di login interattiva...';
          exec bash -l
          "
    ```
    **Come usare con `docker-compose` e variabili d'ambiente:**
    Prima di eseguire `docker compose ...`, definisci le variabili d'ambiente nella tua shell host o in un file `.env` nella stessa directory del `docker-compose.yml`:
    **Nella shell host:**
    ```bash
    export UID_HOST=$(id -u)
    export GID_HOST=$(id -g)
    docker compose up legopst_interactive # o docker compose run ...
    ```
    **Oppure, crea un file `.env`:**
    ```
    UID_HOST=1000 # Sostituisci con il tuo UID effettivo
    GID_HOST=1000 # Sostituisci con il tuo GID effettivo
    ```
    (Puoi anche mettere `UID_HOST=$(id -u)` direttamente, ma non tutti gli interpreti di `.env` lo gestiscono).
    Docker Compose leggerà automaticamente il file `.env`.

**Prerequisiti e Considerazioni per l'Opzione `-u`:**

*   **Utente nel Container:** L'UID/GID specificato con `-u` non deve necessariamente corrispondere a un utente con un nome definito nel file `/etc/passwd` del container. Il kernel Linux si preoccupa solo degli UID/GID numerici per i permessi.
*   **Permessi sui File dell'Immagine:** Se il processo nel container (ora in esecuzione come tuo utente host) ha bisogno di scrivere in directory che sono parte dell'immagine Docker e che appartengono a `root` con permessi restrittivi (es. `drwxr-x--- root root`), allora l'operazione di scrittura fallirà. Le immagini sono spesso costruite in modo che molte directory siano scrivibili da `root`.
    *   Questo è meno un problema per il volume che hai montato (`/home/antonio_container`), perché i permessi di quella directory sono determinati dall'host.
*   **Software che si aspetta di girare come root:** Alcuni software potrebbero essere progettati per avviarsi come `root` (es. per legarsi a porte privilegiate < 1024, anche se questo è meno rilevante per una shell interattiva).
*   **File di Profilo e `HOME`:** Quando esegui come un UID arbitrario, la variabile d'ambiente `HOME` potrebbe non essere impostata correttamente o potrebbe puntare a `/` o a una directory non scrivibile. Potrebbe essere necessario impostare `HOME` esplicitamente (es. `environment: - HOME=/home/antonio_container` in `docker-compose.yml` o `-e HOME=/home/antonio_container` con `docker run`) se i tuoi script di profilo o applicazioni ne dipendono. La shell di login (`bash -l`) proverà a leggere i file di profilo relativi alla home dell'utente (come `~/.bash_profile`). Se `HOME` è `/home/antonio_container`, cercherà `/home/antonio_container/.bash_profile`.

**2. Creare un Utente nel Dockerfile con lo Stesso UID/GID (Più lavoro di preparazione nell'immagine):**

Puoi modificare il tuo `Dockerfile_LegoPST` per creare un utente `antonio` (o un nome generico come `devuser`) con UID e GID che possono essere passati come argomenti durante la build, o impostati a valori comuni.

**Esempio `Dockerfile_LegoPST`:**
```dockerfile
# ... (FROM fedora:41 e installazioni) ...

ARG USER_UID=1000 # Valore di default, può essere sovrascritto con --build-arg
ARG USER_GID=1000 # Valore di default

# Crea il gruppo e l'utente
RUN groupadd -g $USER_GID devuser || true # || true per ignorare l'errore se il GID esiste già
RUN useradd -u $USER_UID -g $USER_GID -ms /bin/bash -d /home/devuser devuser
RUN mkdir -p /home/devuser && chown devuser:devuser /home/devuser

# Copia i file necessari per l'utente, imposta permessi se necessario
COPY --chown=devuser:devuser /LegoPST2010A_WSL_DARHE6_64 /home/legoroot_fedora41_img # Rinomino per chiarezza
# ... (copia il profilo o altri file in /home/devuser)

# Imposta l'utente per i comandi successivi e per il runtime
USER devuser
WORKDIR /home/devuser # O il tuo workdir preferito

# Il tuo profilo potrebbe dover essere copiato nella home del nuovo utente
# e sorgente dal .bashrc di devuser
# RUN echo "source /home/legoroot_fedora41_img/.profile_legoroot" >> /home/devuser/.bashrc

# CMD ["bash", "-l"] # CMD di default per avviare una shell per questo utente
CMD ["bash", "-c", "source /home/legoroot_fedora41_img/.profile_legoroot && exec bash -l"]
```

**Come buildare e eseguire:**
```bash
# Build (passando UID/GID del tuo host)
docker build \
  --build-arg USER_UID=$(id -u) \
  --build-arg USER_GID=$(id -g) \
  -t aguagliardi/legopst:latest \
  --file LegoPST2010A_WSL_DARHE6_64/docker/Dockerfile_LegoPST .

# Run (non hai più bisogno dell'opzione -u perché l'utente è impostato nell'immagine)
docker run --rm -it \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -v /home/antonio:/home/devuser \ # Mappa alla home dell'utente nel container
  aguagliardi/legopst:latest
  # Il CMD del Dockerfile verrà eseguito
```

**Con `docker-compose.yml` (usando l'immagine buildata con utente):**
```yaml
version: '3.8'

services:
  legopst_interactive:
    image: aguagliardi/legopst:latest # L'immagine ora ha l'utente 'devuser'
    # build: # Se vuoi che compose buildi l'immagine
    #   context: . # Percorso al contesto di build
    #   dockerfile: ./percorso/a/Dockerfile_LegoPST
    #   args:
    #     USER_UID: ${UID_HOST:-1000} # Usa var d'ambiente o default
    #     USER_GID: ${GID_HOST:-1000}
    container_name: legopst_shell_container
    stdin_open: true
    tty: true
    # user: "${UID_HOST}:${GID_HOST}" # Non più necessario se USER nel Dockerfile è corretto
    working_dir: /home/devuser # O dove hai montato i volumi
    environment:
      - DISPLAY=${DISPLAY}
    volumes:
      - "/tmp/.X11-unix:/tmp/.X11-unix"
      - "/home/antonio:/home/devuser" # Mappa alla home dell'utente nel container
    # Il CMD dell'immagine verrà usato, o puoi sovrascriverlo qui se necessario
    # command: bash -l # se il CMD dell'immagine non è già una shell interattiva
```
**Vantaggi dell'Opzione 2:**
*   L'ambiente all'interno del container è più "pulito" con un utente dedicato.
*   Minore necessità di specificare `-u` al runtime.
**Svantaggi:**
*   Il Dockerfile diventa più complesso.
*   Devi passare gli argomenti UID/GID durante la build se vuoi che corrispondano dinamicamente all'utente host. Se gli UID/GID cambiano sull'host, devi ricostruire l'immagine.

**3. Modificare i Permessi all'Avvio del Container (Entrypoint Script):**

Puoi usare uno script entrypoint nel tuo container che, all'avvio:
1.  Controlla l'UID/GID del volume montato (o riceve UID/GID desiderati tramite variabili d'ambiente).
2.  Crea un utente nel container con quell'UID/GID (se non esiste).
3.  Usa `gosu` o `su-exec` (utility leggere simili a `sudo`) per eseguire il comando principale come quell'utente.

Questa è una tecnica comune per immagini Docker generiche che devono adattarsi all'UID/GID dell'host.
Esempio di script entrypoint (molto semplificato):
```bash
#!/bin/bash
set -e

HOST_UID=${PUID:-$(stat -c "%u" /punto_di_montaggio)} # Prendi PUID da env o dal file montato
HOST_GID=${PGID:-$(stat -c "%g" /punto_di_montaggio)} # Prendi PGID da env o dal file montato
USERNAME="devuser"

if ! getent passwd "$USERNAME" > /dev/null 2>&1; then
    echo "Creating user $USERNAME with UID $HOST_UID GID $HOST_GID"
    groupadd -g "$HOST_GID" "$USERNAME" || true
    useradd -u "$HOST_UID" -g "$HOST_GID" -ms /bin/bash -d "/home/$USERNAME" "$USERNAME"
    # ... configura la home dell'utente, copia profili, ecc. ...
fi

# Esegui il comando passato come CMD, come l'utente specificato
exec gosu "$USERNAME" "$@" # o su-exec "$USERNAME" "$@"
```
Dovresti installare `gosu` o `su-exec` nel tuo Dockerfile.
**Dockerfile:**
```dockerfile
# ...
RUN dnf install -y gosu # o scarica e installa su-exec
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
CMD ["bash", "-c", "source /home/legoroot_fedora41/.profile_legoroot && exec bash -l"]
```
**docker-compose.yml:**
```yaml
# ...
    environment:
      - PUID=${UID_HOST} # Passa UID/GID dell'host
      - PGID=${GID_HOST}
    volumes:
      - "/home/antonio:/punto_di_montaggio" # L'entrypoint controllerà i permessi qui
# ...
```
Questa è la soluzione più flessibile ma anche la più complessa da implementare correttamente.

**Quale soluzione scegliere?**

*   Per il tuo caso d'uso (una shell interattiva con accesso alla tua home), **l'Opzione 1 (usare `-u $(id -u):$(id -g)` o `user: "${UID_HOST}:${GID_HOST}"`) è spesso la più semplice ed efficace.** Ricorda di mappare il volume a una directory come `/home/antonio_container` (o simile) invece di `/root` per evitare conflitti e per chiarezza.
*   L'Opzione 2 è valida se vuoi un utente predefinito non-root nell'immagine.
*   L'Opzione 3 è potente per immagini generiche, ma potrebbe essere eccessiva per questo scenario.

**Consiglio pratico per l'Opzione 1 (la più semplice):**

**Modifica `docker-compose.yml`:**

```yaml
version: '3.8'

services:
  legopst_interactive:
    image: aguagliardi/legopst:latest
    container_name: legopst_shell_user_antonio # Nome container più descrittivo
    stdin_open: true
    tty: true
    user: "${UID_HOST:-1000}:${GID_HOST:-1000}" # Usa var d'ambiente o default 1000
    working_dir: /home/antonio_workspace # Nuovo punto di montaggio e workdir
    environment:
      - DISPLAY=${DISPLAY}
      - HOME=/home/antonio_workspace # Imposta HOME per la shell di login
    volumes:
      - "/tmp/.X11-unix:/tmp/.X11-unix"
      - "/home/antonio:/home/antonio_workspace" # Mappa qui
    command: >
      bash -c "
      # Il profilo è nell'immagine, non nel volume montato da host (a meno che non sia in /home/antonio)
      # Se il profilo è parte di ciò che copi in /home/legoroot_fedora41 nell'immagine:
      PROFILE_PATH_IN_IMAGE=/home/legoroot_fedora41/.profile_legoroot

      # Se il profilo si trova in /home/antonio (host) e quindi in /home/antonio_workspace (container):
      # PROFILE_PATH_IN_CONTAINER=/home/antonio_workspace/LegoPST2010A_WSL_DARHE6_64/.profile_legoroot # Adatta il percorso

      if [ -f \"$PROFILE_PATH_IN_IMAGE\" ]; then
        echo \"Sourcing $PROFILE_PATH_IN_IMAGE...\";
        source \"$PROFILE_PATH_IN_IMAGE\";
      else
        echo \"Attenzione: $PROFILE_PATH_IN_IMAGE non trovato nel container.\";
      fi;
      echo 'Avvio della shell di login interattiva...';
      exec bash -l
      "
```

**Prima di eseguire `docker compose up`:**

```bash
export UID_HOST=$(id -u)
export GID_HOST=$(id -g)
docker compose up legopst_interactive
```

Questo dovrebbe darti una shell come tuo utente host, e i file creati/modificati in `/home/antonio_workspace` (che è `/home/antonio` sull'host) avranno la proprietà corretta. Assicurati che il percorso a `.profile_legoroot` nel `command:` sia corretto rispetto a dove si trova quel file *all'interno dell'immagine* o nel volume montato.