#!/bin/bash
#
# Script di installazione LegoPST
#
# Scarica lgdock.sh e crea il comando "lgrun" per eseguire LegoPST via Docker
# Non richiede installazione di LegoPST - tutto funziona tramite container Docker
#

set -e

VERSION="1.0"

# ---------------------------------------------------------------------------
#  -h prima di tutto: chi si trova davanti questo script deve poter capire
#  cosa fa PRIMA di eseguirlo. Va sopra il rilevamento del repository, che
#  stampa e legge file, e sopra qualunque cosa tocchi la home.
# ---------------------------------------------------------------------------
mostra_aiuto() {
    cat <<'AIUTO'
install_legopst_dock.sh - installs LegoPST as the "lgrun" command, via Docker

WHAT IT IS FOR
  Using LegoPST on a machine where LegoPST is NOT installed and will not be
  compiled. The full environment - compilers, Motif, Tcl/Tk/Tix, X11 - lives
  inside a Docker image; this script installs only the commands that launch
  it. Your files stay yours, in your home: the container mounts the host home
  and creates your same user inside, so whatever it writes is owned by your
  UID, not by root.

WHAT IT DOES
  1. checks that Docker (or Podman) is running and that curl is available;
  2. downloads docker/lgdock.sh FROM THE REPOSITORY - from the branch, not
     from a copy - and installs it as ~/.local/lib/legopst/lgdock, OUTSIDE
     the PATH: it is the engine, not a command to type;
  3. creates ~/.local/bin/lgrun as a SYMLINK to lgdock. The end user has a
     single command in the PATH, and its help shows the right name by
     itself: through the link, "basename $0" is already "lgrun".
  4. if ~/.local/bin is not in the PATH, adds a line to your .bashrc.
  It does not install LegoPST, compiles nothing, does not touch the system:
  it writes to ~/.local/bin and ~/.local/lib/legopst and, at most, that one
  line in .bashrc.

AFTER INSTALLATION
  lgrun              starts the container and opens a bash terminal inside
  lgrun --demo       also puts a sample model there (legocad and sked)
  lgrun -d NAME      another demo: legopst_NAME, downloaded from the release
  lgrun --socat      X11 through a socket bridge, for SSH/MobaXterm
  lgrun --pull       updates the image before starting
  lgrun -e lghmi     opens lghmi (or another program) once the container is up
  lgrun lghmi        only lghmi, no shell: the container exits with it
  lgrun -v           versions of lgrun, of the image and of LegoPST
  lgrun -dbg         shows every startup message (normally: one line)
  lgrun -update      reinstalls the latest version and updates the image
  lgrun -uninstall   uninstalls (same as "install_legopst_dock.sh -u")
  lgrun --help       all the options
  The first time, Docker downloads the image (a few GB): a few minutes, once.

UPDATING
  Run this script again (or "lgrun -update"): it downloads lgdock.sh from the
  branch again and overwrites the commands. That is how you get fixes - the
  installed copies do not update themselves.

UNINSTALLING
  install_legopst_dock.sh -u
  Removes from ~/.local/bin the commands it installed (and only those: a file
  with the same name belonging to someone else is left alone). It does NOT
  delete the Docker image or your data - ~/legocad, ~/sked, ~/defaults,
  ~/risorse are yours; it prints the command to remove the image, if you want
  that gone too. The PATH line in .bashrc is removed only if ~/.local/bin
  ends up empty, because other commands often live there.

OPTIONS
  -h, --help         this message
  -u, --uninstall    uninstall (see above)

WHERE IT DOWNLOADS FROM
  Host, repository and branch are read from docker/repo_info.conf (generated
  by the Makefile) or from the git remote - only when the script is run as a
  file from a checkout. Run through "curl ... | bash" it uses RSE-TGM/LegoPST,
  branch master. The environment variables REPO_HOST, REPO_SLUG, REPO_BRANCH
  override everything. The image is the one written in lgdock.sh
  (aguagliardi/legopst:2.0), replaceable with LG_DOCKER_IMAGE.
AIUTO
}

#  Disinstallazione: l'inverso esatto dell'installazione, e nulla di piu'.
#  Il criterio e' "si tocca solo cio' che ha messo questo script": i due
#  comandi in ~/.local/bin, e la riga del PATH soltanto quando non serve piu' a
#  nessuno. L'immagine Docker e i dati dell'utente non sono roba
#  dell'installatore: si dice come fare e si lascia decidere a lui.
#  Il runtime dei container: docker se c'e' come ESEGUIBILE, altrimenti podman.
#  Silenzioso: il messaggio lo stampa la verifica dei prerequisiti. Sta qui, in
#  cima, perche' serve anche alla disinstallazione, che gira prima di quella.
#
#  Non basta cercare "docker": su alcune installazioni (Arch, tipicamente) e' un
#  ALIAS di shell a podman, e gli alias NON esistono nelle shell non
#  interattive. Questo script si lancia con bash -c "$(curl ...)", quindi non lo
#  vedrebbe mai.
rileva_runtime() {
    if command -v docker >/dev/null 2>&1; then
        echo docker
    elif command -v podman >/dev/null 2>&1; then
        echo podman
    fi
}
RUNTIME="$(rileva_runtime)"

#  Il nome dell'immagine, letto da un lgdock.
#
#  Solo la riga dell'ASSEGNAZIONE: appena sopra c'e' un commento che mostra
#  un'immagine di ESEMPIO (LG_DOCKER_IMAGE=...:2.1-prova), e un grep non
#  ancorato pesca quella. E' successo due volte - in disinstallazione e
#  nell'aggiornamento dell'immagine - percio' la lettura sta qui, in un posto
#  solo, e non si riscrive a mano.
immagine_da() {
    local f="$1" img=""
    if [ -f "$f" ]; then
        img=$(grep -m1 '^IMAGE_NAME=' "$f" 2>/dev/null \
              | grep -o 'aguagliardi/legopst[^"}[:space:]]*' || true)
    fi
    echo "${img:-aguagliardi/legopst:2.0}"
}

disinstalla() {
    LGRUN_LINK="$INSTALL_DIR/lgrun"
    echo "======================================================================="
    echo "  Uninstalling LegoPST (the lgrun and lgdock commands)"
    echo "======================================================================="
    echo ""

    #  il nome dell'immagine si legge dal lanciatore PRIMA di cancellarlo:
    #  dopo non ci sarebbe piu' modo di saperlo
    #  Il nome dell'immagine si legge dal motore PRIMA di cancellarlo: dopo non
    #  ci sarebbe piu' modo di saperlo. Si guarda nella posizione nuova e in
    #  quella vecchia, perche' chi disinstalla puo' avere ancora l'assetto di
    #  prima (lgdock dentro ~/.local/bin).
    IMMAGINE=$(immagine_da "$LIB_DIR/lgdock")
    if [ ! -f "$LIB_DIR/lgdock" ]; then
        IMMAGINE=$(immagine_da "$INSTALL_DIR/lgdock")   # assetto precedente
    fi

    #  lgrun e' un symlink: -e lo segue, e su un link penzolante direbbe che non
    #  c'e' pur essendoci. Percio' si controlla anche -L.
    if [ -L "$LGRUN_LINK" ] || [ -e "$LGRUN_LINK" ]; then
        #  Un omonimo di qualcun altro non si tocca: o punta al nostro motore,
        #  o e' un file che nomina LegoPST (il vecchio involucro).
        BERSAGLIO=$(readlink "$LGRUN_LINK" 2>/dev/null || true)
        if [ "$BERSAGLIO" = "$LIB_DIR/lgdock" ] || grep -q "LegoPST" "$LGRUN_LINK" 2>/dev/null; then
            rm -f "$LGRUN_LINK"
            echo "✓ removed $LGRUN_LINK"
        else
            echo "⚠ $LGRUN_LINK does not look installed by this script: leaving it alone"
        fi
    else
        echo "- lgrun: not installed in $INSTALL_DIR"
    fi

    #  Il motore, nella posizione nuova e in quella vecchia.
    for f in "$LIB_DIR/lgdock" "$INSTALL_DIR/lgdock"; do
        [ -f "$f" ] || continue
        if ! grep -q "LegoPST" "$f" 2>/dev/null; then
            echo "⚠ $f does not look installed by this script: leaving it alone"
            continue
        fi
        rm -f "$f"
        echo "✓ removed $f"
    done
    #  La dir di libreria si toglie solo se e' rimasta vuota: e' nostra, ma non
    #  e' detto che non ci abbia messo altro qualcun altro.
    if [ -d "$LIB_DIR" ] && [ -z "$(ls -A "$LIB_DIR" 2>/dev/null)" ]; then
        rmdir "$LIB_DIR" 2>/dev/null && echo "✓ removed $LIB_DIR"
    fi

    #  La riga del PATH. In ~/.local/bin ci vivono spesso altri comandi (pipx,
    #  uv, cmake...): togliere quella riga li farebbe sparire dal PATH senza un
    #  errore che lo spieghi. Quindi la si tocca solo a directory vuota.
    RESTANTI=$(ls -A "$INSTALL_DIR" 2>/dev/null | wc -l)
    for cfg in "$HOME/.bashrc" "$HOME/.bash_profile"; do
        [ -f "$cfg" ] || continue
        grep -q "# Added by LegoPST installer" "$cfg" 2>/dev/null || continue
        if [ "$RESTANTI" -eq 0 ]; then
            sed -i '/# Added by LegoPST installer/,+1d' "$cfg"
            echo "✓ removed the PATH line from $cfg ($INSTALL_DIR is now empty)"
        else
            echo ""
            echo "- $cfg still has the line added by the installer:"
            echo "      # Added by LegoPST installer"
            echo "      export PATH=\"\$HOME/.local/bin:\$PATH\""
            echo "  Keeping it: $INSTALL_DIR still holds $RESTANTI files, and other"
            echo "  commands may depend on that line. Remove it by hand if you are"
            echo "  sure it is not needed."
        fi
    done

    echo ""
    echo "Left untouched:"
    echo "  - the Docker image. To remove it too (a few GB):"
    echo "        ${RUNTIME:-docker} rmi $IMMAGINE"
    echo "  - your data: ~/legocad, ~/sked, ~/defaults, ~/risorse and the models they"
    echo "    contain. That is your work: no script deletes it."
    echo ""
    echo "Uninstall complete."
}

PULL_IMAGE=false
case "${1:-}" in
    -h|--help)      mostra_aiuto; exit 0 ;;
    -u|--uninstall)
        INSTALL_DIR="$HOME/.local/bin"
        LIB_DIR="$HOME/.local/lib/legopst"
        disinstalla; exit 0 ;;
    #  Lo passa "lgrun -update": installa e POI scarica l'immagine. L'ordine
    #  conta - il nome dell'immagine sta dentro lgdock, e un aggiornamento puo'
    #  cambiarlo, quindi scaricarla prima vorrebbe dire tirare giu' la vecchia.
    --pull-image)   PULL_IMAGE=true ;;
esac

# Rileva host e owner/repo dal file repo_info.conf (generato da Makefile.mk)
# oppure dal git remote come fallback
DEFAULT_HOST="github.com"
DEFAULT_REPO="RSE-TGM/LegoPST"
DEFAULT_BRANCH="master"
#  Le variabili d'ambiente vincono su tutto, come promette l'aiuto: si mettono
#  da parte prima che il rilevamento le sovrascriva.
ENV_HOST="${REPO_HOST:-}"
ENV_SLUG="${REPO_SLUG:-}"
ENV_BRANCH="${REPO_BRANCH:-}"
REPO_HOST=""
REPO_SLUG=""
REPO_BRANCH=""

#  Il rilevamento ha senso solo se lo script e' un FILE dentro un checkout.
#  Lanciato con "curl ... | bash" o bash -c "$(curl ...)" non lo e':
#  BASH_SOURCE e' vuoto e $0 vale "bash", e dirname "bash" e' la directory
#  corrente - dentro un altro repository git si sarebbe preso il suo remote.
SCRIPT_DIR_INST=""
if [ -n "${BASH_SOURCE[0]:-}" ] && [ -f "${BASH_SOURCE[0]}" ]; then
    SCRIPT_DIR_INST="$(cd "$(dirname "${BASH_SOURCE[0]}")" 2>/dev/null && pwd)"
fi
REPO_INFO_FILE="$SCRIPT_DIR_INST/repo_info.conf"

if [ -z "$SCRIPT_DIR_INST" ]; then
    :   # scaricato al volo: valgono i default qui sotto
elif [ -f "$REPO_INFO_FILE" ]; then
    # Leggi le coordinate dal file generato dal Makefile
    . "$REPO_INFO_FILE"
    echo "Repository from repo_info.conf: $REPO_HOST / $REPO_SLUG (branch: $REPO_BRANCH)"
else
    # Fallback: rileva dal git remote
    if command -v git >/dev/null 2>&1 && git -C "$SCRIPT_DIR_INST" rev-parse --is-inside-work-tree >/dev/null 2>&1; then
        REMOTE_URL=$(git -C "$SCRIPT_DIR_INST" remote get-url origin 2>/dev/null || true)
        if [ -n "$REMOTE_URL" ]; then
            REPO_HOST=$(echo "$REMOTE_URL" | sed -E 's#https?://([^/]+)/.*#\1#; s#git@([^:]+):.*#\1#')
            REPO_SLUG=$(echo "$REMOTE_URL" | sed -E 's#(https?://[^/]+/|git@[^:]+:)##; s/\.git$//')
            REPO_BRANCH=$(git -C "$SCRIPT_DIR_INST" symbolic-ref --short HEAD 2>/dev/null || true)
        fi
    fi
fi

REPO_HOST="${ENV_HOST:-${REPO_HOST:-$DEFAULT_HOST}}"
REPO_SLUG="${ENV_SLUG:-${REPO_SLUG:-$DEFAULT_REPO}}"
REPO_BRANCH="${ENV_BRANCH:-${REPO_BRANCH:-$DEFAULT_BRANCH}}"

# Costruisci URL raw in base al tipo di hosting
if [ "$REPO_HOST" = "github.com" ]; then
    LGDOCK_URL="https://raw.githubusercontent.com/${REPO_SLUG}/${REPO_BRANCH}/docker/lgdock.sh"
else
    LGDOCK_URL="https://${REPO_HOST}/${REPO_SLUG}/-/raw/${REPO_BRANCH}/docker/lgdock.sh"
fi
#  L'utente finale ha UN SOLO comando nel PATH: lgrun. lgdock e' il motore, e
#  vive fuori dal PATH - due nomi per la stessa cosa nella stessa directory
#  facevano scegliere a caso, e l'aiuto ne indicava uno mentre se ne digitava
#  un altro.
#
#  lgrun e' un SYMLINK a lgdock, non un involucro: bash imposta $0 al percorso
#  con cui lo script e' stato invocato, quindi attraverso il link "basename $0"
#  vale gia' "lgrun" e l'aiuto porta il nome giusto da se'. L'involucro
#  precedente non poteva farlo (exec -a non funziona sugli script: per uno
#  shebang e' il kernel a passare il path all'interprete) e avrebbe richiesto a
#  lgdock di sapere di poter essere frontato.
INSTALL_DIR="$HOME/.local/bin"
LIB_DIR="$HOME/.local/lib/legopst"
LGDOCK_SCRIPT="$LIB_DIR/lgdock"
LGRUN_SCRIPT="$INSTALL_DIR/lgrun"

echo "======================================================================="
echo "  LegoPST installation - v${VERSION}"
echo "======================================================================="
echo ""
echo "This script installs LegoPST as the 'lgrun' command"
echo "LegoPST itself is not installed locally - everything runs through Docker"
echo ""

# =============================================================================
# Verifica prerequisiti
# =============================================================================
echo "--- Checking prerequisites ---"

#  Il runtime: docker se c'e' come ESEGUIBILE, altrimenti podman.
#
#  Non basta cercare "docker": su alcune installazioni (Arch, tipicamente) e' un
#  ALIAS di shell a podman, e gli alias NON esistono nelle shell non
#  interattive. Questo script si lancia con "bash -c \"$(curl ...)\"", quindi
#  non lo vedrebbe mai: diceva "Docker non trovato" e si fermava, mentre Podman
#  era installato e funzionante.
if [ -z "$RUNTIME" ]; then
    echo "ERROR: no container runtime found (docker or podman)!"
    echo ""
    echo "To install one:"
    echo "  Ubuntu/Debian: sudo apt-get install docker.io   (or podman)"
    echo "  Fedora/RHEL:   sudo dnf install docker          (or podman)"
    echo "  Arch:          sudo pacman -S docker            (or podman)"
    echo ""
    echo "With Docker, add your user to the docker group:"
    echo "  sudo usermod -aG docker \$USER"
    echo "  newgrp docker"
    echo ""
    echo "If 'docker' works in your terminal but not here, it is an ALIAS:"
    echo "aliases do not exist in non-interactive shells. Install"
    echo "podman-docker, or just use podman - it is detected automatically."
    exit 1
fi
echo "✓ Runtime found: $RUNTIME"

# Controlla curl
if ! command -v curl >/dev/null 2>&1; then
    echo "ERROR: curl not found!"
    echo "Install curl with: sudo apt-get install curl"
    exit 1
fi
echo "✓ curl installed"

# Verifica che il runtime sia accessibile
if $RUNTIME ps >/dev/null 2>&1; then
    echo "✓ $RUNTIME accessible"
elif [ -w "/var/run/docker.sock" ]; then
    echo "✓ $RUNTIME accessible"
else
    echo "⚠ $RUNTIME requires sudo (normal: lgrun handles it automatically)"
fi

echo ""

# =============================================================================
# Creazione directory di installazione
# =============================================================================
echo "--- Preparing installation ---"

mkdir -p "$LIB_DIR"
if [ ! -d "$INSTALL_DIR" ]; then
    echo "Creating directory $INSTALL_DIR..."
    mkdir -p "$INSTALL_DIR"
else
    echo "✓ Installation directory exists"
fi

# =============================================================================
# Download lgdock.sh
# =============================================================================
echo ""
echo "--- Download lgdock.sh ---"
echo "URL: $LGDOCK_URL"
echo "Destination: $LGDOCK_SCRIPT"

if curl -fsSL "$LGDOCK_URL" -o "$LGDOCK_SCRIPT"; then
    chmod +x "$LGDOCK_SCRIPT"
    echo "✓ lgdock.sh downloaded and made executable"
else
    echo "ERROR: cannot download lgdock.sh"
    echo "Check your internet connection and the URL"
    exit 1
fi

# Iniezione versione dal file VERSION
LOCAL_VERSION_FILE="$SCRIPT_DIR_INST/../VERSION"
PROJECT_VER=""
if [ -f "$LOCAL_VERSION_FILE" ]; then
    # File VERSION locale (esecuzione da repo)
    PROJECT_VER=$(tr -d '[:space:]' < "$LOCAL_VERSION_FILE")
else
    # Scarica VERSION dal repo remoto
    if [ "$REPO_HOST" = "github.com" ]; then
        VER_URL="https://raw.githubusercontent.com/${REPO_SLUG}/${REPO_BRANCH}/VERSION"
    else
        VER_URL="https://${REPO_HOST}/${REPO_SLUG}/-/raw/${REPO_BRANCH}/VERSION"
    fi
    PROJECT_VER=$(curl -fsSL "$VER_URL" 2>/dev/null | tr -d '[:space:]' || true)
fi

if [ -n "$PROJECT_VER" ]; then
    sed -i "s/^VERSION=\".*\"/VERSION=\"${PROJECT_VER}\"/" "$LGDOCK_SCRIPT"
    echo "✓ Version set to: $PROJECT_VER"
else
    echo "⚠ VERSION file not found, keeping the placeholder version"
fi

#  Da dove e' arrivata questa copia. Serve a "lgrun -update", che riscarica
#  l'installer: senza, dovrebbe cablare master e chi installa da un branch di
#  prova ci finirebbe sopra senza accorgersene.
sed -i -e "s|^REPO_HOST=\".*\"|REPO_HOST=\"${REPO_HOST}\"|" \
       -e "s|^REPO_SLUG=\".*\"|REPO_SLUG=\"${REPO_SLUG}\"|" \
       -e "s|^REPO_BRANCH=\".*\"|REPO_BRANCH=\"${REPO_BRANCH}\"|" "$LGDOCK_SCRIPT"
echo "✓ Updates from: $REPO_SLUG ($REPO_BRANCH)"

# =============================================================================
# Comando lgrun (symlink a lgdock)
# =============================================================================
echo ""
echo "--- Creating the 'lgrun' command ---"

#  Installazione PRECEDENTE: lgdock stava in ~/.local/bin, accanto a lgrun.
#  Va tolto, altrimenti resta nel PATH un secondo comando identico e ormai
#  stantio, che nessuno aggiornera' piu'.
VECCHIO="$INSTALL_DIR/lgdock"
if [ -f "$VECCHIO" ] && [ ! -L "$VECCHIO" ] && grep -q "LegoPST" "$VECCHIO" 2>/dev/null; then
    rm -f "$VECCHIO"
    echo "✓ removed the old $VECCHIO (lgdock now lives in $LIB_DIR)"
fi

#  -f: se lgrun c'e' gia' - da un'installazione precedente, involucro o link -
#  si sostituisce. ln non tocca l'inode del target, quindi questo e' sicuro
#  anche mentre lgrun sta girando (e' il caso di "lgrun -update").
if ln -sfn "$LGDOCK_SCRIPT" "$LGRUN_SCRIPT"; then
    echo "✓ Command 'lgrun' created ($LGRUN_SCRIPT -> $LGDOCK_SCRIPT)"
else
    echo "ERROR: cannot create $LGRUN_SCRIPT"
    exit 1
fi

# =============================================================================
# Verifica PATH
# =============================================================================
echo ""
echo "--- Checking PATH ---"

PATH_CONFIGURED=false
if [[ ":$PATH:" == *":$INSTALL_DIR:"* ]]; then
    echo "✓ $INSTALL_DIR is already in the PATH"
    PATH_CONFIGURED=true
else
    echo "⚠ $INSTALL_DIR is NOT in the PATH"
    echo ""
    echo "Adding $INSTALL_DIR to the PATH in your .bashrc..."

    # Determina quale file di configurazione shell usare
    SHELL_CONFIG="$HOME/.bashrc"
    if [ -f "$HOME/.bash_profile" ]; then
        # Controlla se .bash_profile carica già .bashrc
        if ! grep -q ".bashrc" "$HOME/.bash_profile" 2>/dev/null; then
            SHELL_CONFIG="$HOME/.bash_profile"
        fi
    fi

    # Aggiungi al PATH se non già presente
    if ! grep -q "$INSTALL_DIR" "$SHELL_CONFIG" 2>/dev/null; then
        echo "" >> "$SHELL_CONFIG"
        echo "# Added by LegoPST installer" >> "$SHELL_CONFIG"
        echo "export PATH=\"\$HOME/.local/bin:\$PATH\"" >> "$SHELL_CONFIG"
        echo "✓ PATH updated in $SHELL_CONFIG"
        echo ""
        echo "⚠ IMPORTANT: run 'source $SHELL_CONFIG' or open a new terminal"
    else
        echo "✓ PATH already configured in $SHELL_CONFIG"
        PATH_CONFIGURED=true
    fi
fi

# =============================================================================
# Test installazione
# =============================================================================
echo ""
echo "--- Testing the installation ---"

if [ "$PATH_CONFIGURED" = true ]; then
    if command -v lgrun >/dev/null 2>&1; then
        echo "✓ Command 'lgrun' available"
    else
        echo "⚠ Command 'lgrun' not available yet (open a new terminal)"
    fi
else
    echo "⚠ Open a new terminal to use 'lgrun'"
fi

IMMAGINE_PRONTA=false
#  Aggiornamento dell'immagine, chiesto da "lgrun -update". Si fa QUI, dopo
#  l'installazione, e leggendo il nome dall'lgdock APPENA installato: se un
#  aggiornamento cambia immagine, e' quella nuova che va scaricata.
if [ "$PULL_IMAGE" = true ]; then
    IMG=$(immagine_da "$LGDOCK_SCRIPT")
    echo "--- Updating the Docker image ---"
    echo "Image: $IMG"
    echo "(a few GB: this can take a while)"
    echo ""
    if $RUNTIME pull "$IMG"; then
        echo "✓ Image updated"
        IMMAGINE_PRONTA=true
    else
        #  Non e' un fallimento dell'installazione: i comandi ci sono e
        #  funzionano con l'immagine che c'e' gia'.
        echo ""
        echo "⚠ Could not update the image."
        echo "  The commands are installed and work with the image you already have."
        echo "  You can retry later with:  $RUNTIME pull $IMG"
    fi
    echo ""
fi

# =============================================================================
# Riepilogo finale
# =============================================================================
echo ""
echo "======================================================================="
echo "  Installation complete!"
echo "======================================================================="
echo ""
echo "Installed files:"
echo "  - $LGDOCK_SCRIPT"
echo "  - $LGRUN_SCRIPT"
echo ""
echo "Usage:"
echo "  lgrun              # Start the LegoPST container"
echo "  lgrun --demo       # Start with the demo model"
echo "  lgrun --socat      # Start with X11 through socat (for SSH)"
echo "  lgrun lghmi        # Only lghmi: the container exits with it"
echo "  lgrun -update      # Update command and image to the latest version"
echo "  lgrun -uninstall   # Uninstall"
echo "  lgrun --help       # Show all the options"
echo ""


if [ "$PATH_CONFIGURED" = false ]; then
    echo "⚠ ACTION REQUIRED:"
    echo "   Run: source ~/.bashrc"
    echo "   or open a new terminal"
    echo ""
fi

echo "To test the installation:"
echo "  lgrun --version"
echo ""
echo "To start LegoPST:"
echo "  lgrun"
echo ""
if [ "$IMMAGINE_PRONTA" = true ]; then
    echo "The image is up to date: the next start is immediate."
else
    echo "Note: on the first start, Docker downloads the LegoPST image"
    echo "      (about 2-3 GB, it may take a few minutes)"
fi
echo ""
echo "======================================================================="
