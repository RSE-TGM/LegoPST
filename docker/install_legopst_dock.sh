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
install_legopst_dock.sh - installa LegoPST come comando "lgrun", via Docker

A COSA SERVE
  A usare LegoPST su una macchina dove LegoPST NON e' installato e non verra'
  compilato. L'ambiente completo - compilatori, Motif, Tcl/Tk/Tix, X11 - sta
  dentro un'immagine Docker; qui si installano solo i due comandi che la
  lanciano. I file su cui lavori restano tuoi, nella tua home: il container
  monta la home dell'host e dentro ci crea il tuo stesso utente, cosi' quello
  che scrive esce con il tuo UID e non di root.

COSA FA, IN CONCRETO
  1. controlla che ci siano Docker (in esecuzione) e curl;
  2. scarica docker/lgdock.sh DAL REPOSITORY - dal branch, non da una copia -
     e lo installa come ~/.local/bin/lgdock;
  3. crea ~/.local/bin/lgrun, un involucro che passa tutto a lgdock;
  4. se ~/.local/bin non e' nel PATH, aggiunge una riga al tuo .bashrc.
  Non installa LegoPST, non compila niente, non tocca il sistema: scrive in
  ~/.local/bin e, al piu', quella riga nel .bashrc.

DOPO L'INSTALLAZIONE
  lgrun              avvia il container e apre un terminale bash dentro
  lgrun --demo       ci mette anche un modello di esempio (legocad e sked)
  lgrun --socat      X11 attraverso un socket bridge, per SSH/MobaXterm
  lgrun --pull       aggiorna l'immagine prima di partire
  lgrun --help       tutte le opzioni
  La prima volta Docker scarica l'immagine (qualche GB): qualche minuto, una
  volta sola.

PER AGGIORNARSI
  Rilancia questo script: riscarica lgdock.sh dal branch e sovrascrive i due
  comandi. E' il modo di prendere le correzioni - quelle copie non si
  aggiornano da se'.

PER DISINSTALLARE
  install_legopst_dock.sh -u
  Toglie da ~/.local/bin i due comandi che ha messo (e solo quelli: un file
  omonimo di qualcun altro lo lascia dov'e'). NON cancella l'immagine Docker
  ne' i tuoi dati - ~/legocad, ~/sked, ~/defaults sono il tuo lavoro; stampa il
  comando per l'immagine, se la vuoi togliere anche quella. La riga del PATH
  nel .bashrc la toglie solo se ~/.local/bin resta vuota, perche' la' dentro
  vivono spesso altri comandi.

OPZIONI
  -h, --help         questo messaggio
  -u, --uninstall    disinstalla (vedi sopra)

DOVE VA A PESCARE
  Host, repository e branch li legge da docker/repo_info.conf (lo genera il
  Makefile) oppure dal git remote; si forzano con le variabili d'ambiente
  REPO_HOST, REPO_SLUG, REPO_BRANCH. L'immagine e' quella scritta in lgdock.sh
  (aguagliardi/legopst:2.0), sostituibile con LG_DOCKER_IMAGE.
AIUTO
}

#  Disinstallazione: l'inverso esatto dell'installazione, e nulla di piu'.
#  Il criterio e' "si tocca solo cio' che ha messo questo script": i due
#  comandi in ~/.local/bin, e la riga del PATH soltanto quando non serve piu' a
#  nessuno. L'immagine Docker e i dati dell'utente non sono roba
#  dell'installatore: si dice come fare e si lascia decidere a lui.
disinstalla() {
    echo "======================================================================="
    echo "  Disinstallazione LegoPST (i comandi lgrun e lgdock)"
    echo "======================================================================="
    echo ""

    #  il nome dell'immagine si legge dal lanciatore PRIMA di cancellarlo:
    #  dopo non ci sarebbe piu' modo di saperlo
    IMMAGINE=$(grep -m1 -o 'aguagliardi/legopst[^"}[:space:]]*' "$INSTALL_DIR/lgdock" 2>/dev/null || true)
    IMMAGINE="${IMMAGINE:-aguagliardi/legopst:2.0}"

    for comando in lgrun lgdock; do
        f="$INSTALL_DIR/$comando"
        if [ ! -e "$f" ]; then
            echo "- $comando: non installato in $INSTALL_DIR"
            continue
        fi
        #  un file con lo stesso nome ma di qualcun altro non si tocca
        if ! grep -q "LegoPST" "$f" 2>/dev/null; then
            echo "⚠ $f non sembra installato da qui: lo lascio dov'e'"
            continue
        fi
        rm -f "$f"
        echo "✓ rimosso $f"
    done

    #  La riga del PATH. In ~/.local/bin ci vivono spesso altri comandi (pipx,
    #  uv, cmake...): togliere quella riga li farebbe sparire dal PATH senza un
    #  errore che lo spieghi. Quindi la si tocca solo a directory vuota.
    RESTANTI=$(ls -A "$INSTALL_DIR" 2>/dev/null | wc -l)
    for cfg in "$HOME/.bashrc" "$HOME/.bash_profile"; do
        [ -f "$cfg" ] || continue
        grep -q "# Added by LegoPST installer" "$cfg" 2>/dev/null || continue
        if [ "$RESTANTI" -eq 0 ]; then
            sed -i '/# Added by LegoPST installer/,+1d' "$cfg"
            echo "✓ rimossa da $cfg la riga del PATH ($INSTALL_DIR e' rimasta vuota)"
        else
            echo ""
            echo "- In $cfg resta la riga aggiunta dall'installazione:"
            echo "      # Added by LegoPST installer"
            echo "      export PATH=\"\$HOME/.local/bin:\$PATH\""
            echo "  La lascio: in $INSTALL_DIR ci sono ancora $RESTANTI file e altri"
            echo "  comandi possono dipendere da quella riga. Toglila a mano se sei"
            echo "  sicuro che non serva."
        fi
    done

    echo ""
    echo "Non ho toccato:"
    echo "  - l'immagine Docker. Per togliere anche quella (qualche GB):"
    echo "        docker rmi $IMMAGINE"
    echo "  - i tuoi dati: ~/legocad, ~/sked, ~/defaults e i modelli che"
    echo "    contengono. Sono il tuo lavoro, non li cancella nessuno script."
    echo ""
    echo "Disinstallazione completata."
}

case "${1:-}" in
    -h|--help)      mostra_aiuto; exit 0 ;;
    -u|--uninstall) INSTALL_DIR="$HOME/.local/bin"; disinstalla; exit 0 ;;
esac

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
    echo "✓ Versione impostata a: $PROJECT_VER"
else
    echo "⚠ File VERSION non trovato, versione placeholder mantenuta"
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
