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
     e lo installa come ~/.local/lib/legopst/lgdock, FUORI dal PATH: e' il
     motore, non un comando da digitare;
  3. crea ~/.local/bin/lgrun come SYMLINK a lgdock. L'utente finale ha cosi'
     un solo comando nel PATH, e l'aiuto porta il nome giusto da se': bash
     mette in $0 il percorso con cui lo script e' stato invocato, quindi
     attraverso il link "basename $0" vale gia' "lgrun".
  4. se ~/.local/bin non e' nel PATH, aggiunge una riga al tuo .bashrc.
  Non installa LegoPST, non compila niente, non tocca il sistema: scrive in
  ~/.local/bin e, al piu', quella riga nel .bashrc.

DOPO L'INSTALLAZIONE
  lgrun              avvia il container e apre un terminale bash dentro
  lgrun --demo       ci mette anche un modello di esempio (legocad e sked)
  lgrun --socat      X11 attraverso un socket bridge, per SSH/MobaXterm
  lgrun --pull       aggiorna l'immagine prima di partire
  lgrun -e lghmi     appena il container e' pronto apre lghmi (o un altro programma)
  lgrun lghmi        solo lghmi, senza shell: il container si chiude con lui
  lgrun -v           versione di lgrun, dell'immagine e di LegoPST
  lgrun -dbg         mostra tutti i messaggi dell'avvio (di norma: una riga)
  lgrun -update      reinstalla all'ultima versione e aggiorna l'immagine
  lgrun -uninstall   disinstalla (come "install_legopst_dock.sh -u")
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
  ne' i tuoi dati - ~/legocad, ~/sked, ~/defaults, ~/risorse sono tuoi; stampa il
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
    echo "  Disinstallazione LegoPST (i comandi lgrun e lgdock)"
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
            echo "✓ rimosso $LGRUN_LINK"
        else
            echo "⚠ $LGRUN_LINK non sembra installato da qui: lo lascio dov'e'"
        fi
    else
        echo "- lgrun: non installato in $INSTALL_DIR"
    fi

    #  Il motore, nella posizione nuova e in quella vecchia.
    for f in "$LIB_DIR/lgdock" "$INSTALL_DIR/lgdock"; do
        [ -f "$f" ] || continue
        if ! grep -q "LegoPST" "$f" 2>/dev/null; then
            echo "⚠ $f non sembra installato da qui: lo lascio dov'e'"
            continue
        fi
        rm -f "$f"
        echo "✓ rimosso $f"
    done
    #  La dir di libreria si toglie solo se e' rimasta vuota: e' nostra, ma non
    #  e' detto che non ci abbia messo altro qualcun altro.
    if [ -d "$LIB_DIR" ] && [ -z "$(ls -A "$LIB_DIR" 2>/dev/null)" ]; then
        rmdir "$LIB_DIR" 2>/dev/null && echo "✓ rimossa $LIB_DIR"
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
    echo "        ${RUNTIME:-docker} rmi $IMMAGINE"
    echo "  - i tuoi dati: ~/legocad, ~/sked, ~/defaults, ~/risorse e i modelli che"
    echo "    contengono. Sono il tuo lavoro, non li cancella nessuno script."
    echo ""
    echo "Disinstallazione completata."
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

#  Il runtime: docker se c'e' come ESEGUIBILE, altrimenti podman.
#
#  Non basta cercare "docker": su alcune installazioni (Arch, tipicamente) e' un
#  ALIAS di shell a podman, e gli alias NON esistono nelle shell non
#  interattive. Questo script si lancia con "bash -c \"$(curl ...)\"", quindi
#  non lo vedrebbe mai: diceva "Docker non trovato" e si fermava, mentre Podman
#  era installato e funzionante.
if [ -z "$RUNTIME" ]; then
    echo "ERRORE: nessun runtime per container trovato (docker o podman)!"
    echo ""
    echo "Per installarne uno:"
    echo "  Ubuntu/Debian: sudo apt-get install docker.io   (oppure podman)"
    echo "  Fedora/RHEL:   sudo dnf install docker          (oppure podman)"
    echo "  Arch:          sudo pacman -S docker            (oppure podman)"
    echo ""
    echo "Con Docker, aggiungi il tuo utente al gruppo docker:"
    echo "  sudo usermod -aG docker \$USER"
    echo "  newgrp docker"
    echo ""
    echo "Se 'docker' funziona nel tuo terminale ma non qui, e' un ALIAS: gli"
    echo "alias non esistono nelle shell non interattive. Installa"
    echo "podman-docker, oppure usa podman - ora lo trovo da solo."
    exit 1
fi
echo "✓ Runtime trovato: $RUNTIME"

# Controlla curl
if ! command -v curl >/dev/null 2>&1; then
    echo "ERRORE: curl non trovato!"
    echo "Installa curl con: sudo apt-get install curl"
    exit 1
fi
echo "✓ curl installato"

# Verifica che il runtime sia accessibile
if $RUNTIME ps >/dev/null 2>&1; then
    echo "✓ $RUNTIME accessibile"
elif [ -w "/var/run/docker.sock" ]; then
    echo "✓ $RUNTIME accessibile"
else
    echo "⚠ $RUNTIME richiede sudo (normale, lo script gestirà automaticamente)"
fi

echo ""

# =============================================================================
# Creazione directory di installazione
# =============================================================================
echo "--- Preparazione installazione ---"

mkdir -p "$LIB_DIR"
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

#  Da dove e' arrivata questa copia. Serve a "lgrun -update", che riscarica
#  l'installer: senza, dovrebbe cablare master e chi installa da un branch di
#  prova ci finirebbe sopra senza accorgersene.
sed -i -e "s|^REPO_HOST=\".*\"|REPO_HOST=\"${REPO_HOST}\"|" \
       -e "s|^REPO_SLUG=\".*\"|REPO_SLUG=\"${REPO_SLUG}\"|" \
       -e "s|^REPO_BRANCH=\".*\"|REPO_BRANCH=\"${REPO_BRANCH}\"|" "$LGDOCK_SCRIPT"
echo "✓ Aggiornamenti da: $REPO_SLUG ($REPO_BRANCH)"

# =============================================================================
# Comando lgrun (symlink a lgdock)
# =============================================================================
echo ""
echo "--- Creazione comando 'lgrun' ---"

#  Installazione PRECEDENTE: lgdock stava in ~/.local/bin, accanto a lgrun.
#  Va tolto, altrimenti resta nel PATH un secondo comando identico e ormai
#  stantio, che nessuno aggiornera' piu'.
VECCHIO="$INSTALL_DIR/lgdock"
if [ -f "$VECCHIO" ] && [ ! -L "$VECCHIO" ] && grep -q "LegoPST" "$VECCHIO" 2>/dev/null; then
    rm -f "$VECCHIO"
    echo "✓ rimosso il vecchio $VECCHIO (ora lgdock sta in $LIB_DIR)"
fi

#  -f: se lgrun c'e' gia' - da un'installazione precedente, involucro o link -
#  si sostituisce. ln non tocca l'inode del target, quindi questo e' sicuro
#  anche mentre lgrun sta girando (e' il caso di "lgrun -update").
if ln -sfn "$LGDOCK_SCRIPT" "$LGRUN_SCRIPT"; then
    echo "✓ Comando 'lgrun' creato ($LGRUN_SCRIPT -> $LGDOCK_SCRIPT)"
else
    echo "ERRORE: impossibile creare $LGRUN_SCRIPT"
    exit 1
fi

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

IMMAGINE_PRONTA=false
#  Aggiornamento dell'immagine, chiesto da "lgrun -update". Si fa QUI, dopo
#  l'installazione, e leggendo il nome dall'lgdock APPENA installato: se un
#  aggiornamento cambia immagine, e' quella nuova che va scaricata.
if [ "$PULL_IMAGE" = true ]; then
    IMG=$(immagine_da "$LGDOCK_SCRIPT")
    echo "--- Aggiornamento immagine Docker ---"
    echo "Immagine: $IMG"
    echo "(qualche GB: puo' volerci parecchio)"
    echo ""
    if $RUNTIME pull "$IMG"; then
        echo "✓ Immagine aggiornata"
        IMMAGINE_PRONTA=true
    else
        #  Non e' un fallimento dell'installazione: i comandi ci sono e
        #  funzionano con l'immagine che c'e' gia'.
        echo ""
        echo "⚠ Non sono riuscito ad aggiornare l'immagine."
        echo "  I comandi sono installati e funzionano con quella che hai."
        echo "  Puoi riprovare piu' tardi con:  $RUNTIME pull $IMG"
    fi
    echo ""
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
echo "  lgrun lghmi        # Solo lghmi: il container si chiude con lui"
echo "  lgrun -update      # Aggiorna comando e immagine all'ultima versione"
echo "  lgrun -uninstall   # Disinstalla"
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
if [ "$IMMAGINE_PRONTA" = true ]; then
    echo "L'immagine è aggiornata: il prossimo avvio parte subito."
else
    echo "Nota: Al primo avvio, Docker scaricherà l'immagine LegoPST"
    echo "      (circa 2-3 GB, potrebbe richiedere alcuni minuti)"
fi
echo ""
echo "======================================================================="
