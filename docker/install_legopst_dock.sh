#!/bin/bash
#
# Script di installazione LegoPST
#
# Scarica lgdock.sh e crea il comando "lgrun" per eseguire LegoPST via Docker
# Non richiede installazione di LegoPST - tutto funziona tramite container Docker
#

set -e

VERSION="1.0"

# Rileva host e owner/repo dal file repo_info.conf (generato da Makefile.mk)
# oppure dal git remote come fallback
DEFAULT_HOST="github.com"
DEFAULT_REPO="RSE-TGM/LegoPST"
DEFAULT_BRANCH="master"
SCRIPT_DIR_INST="$(cd "$(dirname "$0")" 2>/dev/null && pwd)"
REPO_INFO_FILE="$SCRIPT_DIR_INST/repo_info.conf"
REPO_HOST=""
REPO_SLUG=""
REPO_BRANCH=""

if [ -f "$REPO_INFO_FILE" ]; then
    # Leggi le coordinate dal file generato dal Makefile
    . "$REPO_INFO_FILE"
    echo "Repository rilevato da repo_info.conf: $REPO_HOST / $REPO_SLUG (branch: $REPO_BRANCH)"
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

REPO_HOST="${REPO_HOST:-$DEFAULT_HOST}"
REPO_SLUG="${REPO_SLUG:-$DEFAULT_REPO}"
REPO_BRANCH="${REPO_BRANCH:-$DEFAULT_BRANCH}"

# Costruisci URL raw in base al tipo di hosting
if [ "$REPO_HOST" = "github.com" ]; then
    LGDOCK_URL="https://raw.githubusercontent.com/${REPO_SLUG}/${REPO_BRANCH}/docker/lgdock.sh"
else
    LGDOCK_URL="https://${REPO_HOST}/${REPO_SLUG}/-/raw/${REPO_BRANCH}/docker/lgdock.sh"
fi
INSTALL_DIR="$HOME/.local/bin"
LGDOCK_SCRIPT="$INSTALL_DIR/lgdock"
LGRUN_SCRIPT="$INSTALL_DIR/lgrun"

echo "======================================================================="
echo "  Installazione LegoPST - v${VERSION}"
echo "======================================================================="
echo ""
echo "Questo script installerà LegoPST come comando 'lgrun'"
echo "Non è necessario installare LegoPST localmente - tutto funziona via Docker"
echo ""

# =============================================================================
# Verifica prerequisiti
# =============================================================================
echo "--- Verifica prerequisiti ---"

# Controlla Docker
if ! command -v docker >/dev/null 2>&1; then
    echo "ERRORE: Docker non trovato!"
    echo ""
    echo "Per installare Docker:"
    echo "  Ubuntu/Debian: sudo apt-get install docker.io"
    echo "  Fedora/RHEL:   sudo dnf install docker"
    echo "  Arch:          sudo pacman -S docker"
    echo ""
    echo "Dopo l'installazione, aggiungi il tuo utente al gruppo docker:"
    echo "  sudo usermod -aG docker \$USER"
    echo "  newgrp docker"
    exit 1
fi
echo "✓ Docker installato"

# Controlla curl
if ! command -v curl >/dev/null 2>&1; then
    echo "ERRORE: curl non trovato!"
    echo "Installa curl con: sudo apt-get install curl"
    exit 1
fi
echo "✓ curl installato"

# Verifica che Docker sia accessibile
if ! docker ps >/dev/null 2>&1; then
    if [ -w "/var/run/docker.sock" ]; then
        echo "✓ Docker accessibile"
    else
        echo "⚠ Docker richiede sudo (normale, lo script gestirà automaticamente)"
    fi
else
    echo "✓ Docker accessibile"
fi

echo ""

# =============================================================================
# Creazione directory di installazione
# =============================================================================
echo "--- Preparazione installazione ---"

if [ ! -d "$INSTALL_DIR" ]; then
    echo "Creazione directory $INSTALL_DIR..."
    mkdir -p "$INSTALL_DIR"
else
    echo "✓ Directory di installazione esiste"
fi

# =============================================================================
# Download lgdock.sh
# =============================================================================
echo ""
echo "--- Download lgdock.sh ---"
echo "URL: $LGDOCK_URL"
echo "Destinazione: $LGDOCK_SCRIPT"

if curl -fsSL "$LGDOCK_URL" -o "$LGDOCK_SCRIPT"; then
    chmod +x "$LGDOCK_SCRIPT"
    echo "✓ lgdock.sh scaricato e reso eseguibile"
else
    echo "ERRORE: Impossibile scaricare lgdock.sh"
    echo "Verifica la connessione internet e l'URL"
    exit 1
fi

# =============================================================================
# Creazione comando lgrun
# =============================================================================
echo ""
echo "--- Creazione comando 'lgrun' ---"

cat > "$LGRUN_SCRIPT" << 'EOF'
#!/bin/bash
#
# lgrun - Launcher per LegoPST via Docker
#
# Wrapper per lgdock che fornisce un comando semplice per eseguire LegoPST
#

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
LGDOCK="$SCRIPT_DIR/lgdock"

if [ ! -f "$LGDOCK" ]; then
    echo "ERRORE: lgdock non trovato in $LGDOCK"
    echo "Esegui nuovamente lo script di installazione"
    exit 1
fi

# Passa tutti i parametri a lgdock
exec "$LGDOCK" "$@"
EOF

chmod +x "$LGRUN_SCRIPT"
echo "✓ Comando 'lgrun' creato"

# =============================================================================
# Verifica PATH
# =============================================================================
echo ""
echo "--- Verifica PATH ---"

PATH_CONFIGURED=false
if [[ ":$PATH:" == *":$INSTALL_DIR:"* ]]; then
    echo "✓ $INSTALL_DIR è già nel PATH"
    PATH_CONFIGURED=true
else
    echo "⚠ $INSTALL_DIR NON è nel PATH"
    echo ""
    echo "Aggiungo $INSTALL_DIR al PATH nel tuo .bashrc..."

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
        echo "✓ PATH aggiornato in $SHELL_CONFIG"
        echo ""
        echo "⚠ IMPORTANTE: Esegui 'source $SHELL_CONFIG' o riapri il terminale"
    else
        echo "✓ PATH già configurato in $SHELL_CONFIG"
        PATH_CONFIGURED=true
    fi
fi

# =============================================================================
# Test installazione
# =============================================================================
echo ""
echo "--- Test installazione ---"

if [ "$PATH_CONFIGURED" = true ]; then
    if command -v lgrun >/dev/null 2>&1; then
        echo "✓ Comando 'lgrun' disponibile"
    else
        echo "⚠ Comando 'lgrun' non ancora disponibile (riavvia il terminale)"
    fi
else
    echo "⚠ Riavvia il terminale per usare 'lgrun'"
fi

# =============================================================================
# Riepilogo finale
# =============================================================================
echo ""
echo "======================================================================="
echo "  Installazione completata!"
echo "======================================================================="
echo ""
echo "File installati:"
echo "  - $LGDOCK_SCRIPT"
echo "  - $LGRUN_SCRIPT"
echo ""
echo "Uso:"
echo "  lgrun              # Avvia LegoPST container"
echo "  lgrun --demo       # Avvia con modello demo"
echo "  lgrun --socat      # Avvia con X11 via socat (per SSH)"
echo "  lgrun --help       # Mostra tutte le opzioni"
echo ""

if [ "$PATH_CONFIGURED" = false ]; then
    echo "⚠ AZIONE RICHIESTA:"
    echo "   Esegui: source ~/.bashrc"
    echo "   oppure riapri il terminale"
    echo ""
fi

echo "Per testare l'installazione:"
echo "  lgrun --version"
echo ""
echo "Per avviare LegoPST:"
echo "  lgrun"
echo ""
echo "Nota: Al primo avvio, Docker scaricherà l'immagine LegoPST"
echo "      (circa 2-3 GB, potrebbe richiedere alcuni minuti)"
echo ""
echo "======================================================================="
