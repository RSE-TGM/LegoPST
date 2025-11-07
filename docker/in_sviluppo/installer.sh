#!/bin/bash
# =============================================================================
# LGDock Installer Script
# =============================================================================
# Descrizione: Script di installazione per lgdock e lgdock_socat
# Autore: [Il Tuo Nome]
# Versione: 1.2.0
# =============================================================================

set -e

# Colori per output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color
BOLD='\033[1m'

# Configurazione
GITHUB_USER="TUO_USERNAME"  # <-- MODIFICA QUI
LGDOCK_GIST_ID="d7c030f939f69b07784a309889b8510a"
LGDOCK_SOCAT_GIST_ID="83c887ef78610842508b9f972130d3e1"
INSTALL_DIR="/usr/local/bin"
USER_INSTALL_DIR="$HOME/.local/bin"
VERSION="1.2.0"

# URLs
LGDOCK_URL="https://gist.githubusercontent.com/${GITHUB_USER}/${LGDOCK_GIST_ID}/raw/lgdock.sh"
LGDOCK_SOCAT_URL="https://gist.githubusercontent.com/${GITHUB_USER}/${LGDOCK_SOCAT_GIST_ID}/raw/lgdock_socat.sh"

# Flags
QUIET=false
AUTO_YES=false
USER_INSTALL=false
UPDATE_MODE=false
UNINSTALL_MODE=false
CHECK_ONLY=false

# =============================================================================
# Funzioni Helper
# =============================================================================

print_banner() {
    echo -e "${CYAN}"
    echo "╔════════════════════════════════════════╗"
    echo "║        ${BOLD}LGDock Installer v${VERSION}${NC}${CYAN}         ║"
    echo "║    Docker Management Tools Suite       ║"
    echo "╚════════════════════════════════════════╝"
    echo -e "${NC}"
}

log_info() {
    [[ "$QUIET" == true ]] && return
    echo -e "${BLUE}ℹ${NC} $1"
}

log_success() {
    echo -e "${GREEN}✅${NC} $1"
}

log_error() {
    echo -e "${RED}❌${NC} $1" >&2
}

log_warning() {
    echo -e "${YELLOW}⚠️${NC} $1"
}

spinner() {
    local pid=$1
    local delay=0.1
    local spinstr='⣾⣽⣻⢿⡿⣟⣯⣷'
    while [ "$(ps a | awk '{print $1}' | grep $pid)" ]; do
        local temp=${spinstr#?}
        printf " [%c]  " "$spinstr"
        local spinstr=$temp${spinstr%"$temp"}
        sleep $delay
        printf "\b\b\b\b\b\b"
    done
    printf "    \b\b\b\b"
}

check_command() {
    command -v "$1" >/dev/null 2>&1
}

get_os() {
    case "$(uname -s)" in
        Linux*)     echo "Linux";;
        Darwin*)    echo "Mac";;
        CYGWIN*)    echo "Cygwin";;
        MINGW*)     echo "MinGw";;
        *)          echo "UNKNOWN";;
    esac
}

confirm() {
    [[ "$AUTO_YES" == true ]] && return 0
    
    local prompt="${1:-Continuare?}"
    local response
    
    echo -ne "${BOLD}$prompt [y/N]:${NC} "
    read -r response
    
    [[ "$response" =~ ^[Yy]$ ]]
}

# =============================================================================
# Funzioni di Check
# =============================================================================

check_dependencies() {
    log_info "Controllo dipendenze..."
    
    local deps_ok=true
    
    # Check curl o wget
    if ! check_command curl && ! check_command wget; then
        log_error "curl o wget richiesto ma non trovato"
        deps_ok=false
    fi
    
    # Check Docker
    if ! check_command docker; then
        log_warning "Docker non trovato (opzionale ma consigliato)"
        echo -e "   Installa Docker: ${CYAN}https://docs.docker.com/get-docker/${NC}"
    fi
    
    # Check socat per lgdock_socat
    if ! check_command socat; then
        log_warning "socat non trovato (richiesto per lgdock_socat)"
        
        local os=$(get_os)
        case "$os" in
            Linux)
                if check_command apt-get; then
                    echo "   Installa con: sudo apt-get install socat"
                elif check_command yum; then
                    echo "   Installa con: sudo yum install socat"
                elif check_command pacman; then
                    echo "   Installa con: sudo pacman -S socat"
                fi
                ;;
            Mac)
                echo "   Installa con: brew install socat"
                ;;
        esac
    fi
    
    [[ "$deps_ok" == true ]]
}

check_existing_installation() {
    log_info "Controllo installazioni esistenti..."
    
    local found=false
    
    for dir in "$INSTALL_DIR" "$USER_INSTALL_DIR" "/usr/bin" "/bin"; do
        if [[ -f "$dir/lgdock" ]]; then
            echo -e "   ${GREEN}↳${NC} lgdock trovato in: $dir"
            found=true
        fi
        if [[ -f "$dir/lgdock_socat" ]]; then
            echo -e "   ${GREEN}↳${NC} lgdock_socat trovato in: $dir"
            found=true
        fi
    done
    
    if [[ "$found" == true ]]; then
        log_warning "Installazione esistente trovata"
        if [[ "$UPDATE_MODE" == false ]] && [[ "$UNINSTALL_MODE" == false ]]; then
            confirm "Vuoi aggiornare l'installazione esistente?" || return 1
        fi
    fi
    
    return 0
}

# =============================================================================
# Funzioni di Installazione
# =============================================================================

download_file() {
    local url="$1"
    local output="$2"
    
    if check_command curl; then
        curl -sSL "$url" -o "$output"
    elif check_command wget; then
        wget -qO "$output" "$url"
    else
        log_error "Né curl né wget disponibili"
        return 1
    fi
}

install_script() {
    local script_name="$1"
    local url="$2"
    local target_dir="$3"
    
    log_info "Installazione ${BOLD}$script_name${NC}..."
    
    # Crea directory se non esiste
    if [[ ! -d "$target_dir" ]]; then
        if [[ "$target_dir" == "$USER_INSTALL_DIR" ]]; then
            mkdir -p "$target_dir"
        else
            sudo mkdir -p "$target_dir"
        fi
    fi
    
    # Download temporaneo
    local temp_file=$(mktemp)
    
    if ! download_file "$url" "$temp_file"; then
        log_error "Download di $script_name fallito"
        rm -f "$temp_file"
        return 1
    fi
    
    # Verifica che il file sia uno script valido
    if ! head -n1 "$temp_file" | grep -q "^#!/"; then
        log_error "Il file scaricato non sembra essere uno script valido"
        rm -f "$temp_file"
        return 1
    fi
    
    # Installa
    local target="$target_dir/${script_name%.*}"  # Rimuove .sh
    
    if [[ "$target_dir" == "$USER_INSTALL_DIR" ]] || [[ -w "$target_dir" ]]; then
        mv "$temp_file" "$target"
        chmod +x "$target"
    else
        sudo mv "$temp_file" "$target"
        sudo chmod +x "$target"
    fi
    
    log_success "$script_name installato in: $target"
    
    return 0
}

setup_shell_functions() {
    log_info "Configurazione funzioni shell..."
    
    local shell_rc=""
    
    # Determina file RC
    if [[ -n "$ZSH_VERSION" ]]; then
        shell_rc="$HOME/.zshrc"
    elif [[ -n "$BASH_VERSION" ]]; then
        shell_rc="$HOME/.bashrc"
    else
        log_warning "Shell non riconosciuta, skip configurazione automatica"
        return
    fi
    
    # Controlla se già configurato
    if grep -q "# LGDock Functions" "$shell_rc" 2>/dev/null; then
        log_info "Funzioni shell già configurate in $shell_rc"
        return
    fi
    
    if confirm "Vuoi aggiungere le funzioni LGDock al tuo $shell_rc?"; then
        cat >> "$shell_rc" << 'EOF'

# LGDock Functions
lgdock() {
    if command -v lgdock >/dev/null 2>&1; then
        lgdock "$@"
    else
        bash <(curl -sSL https://gist.githubusercontent.com/TUO_USERNAME/d7c030f939f69b07784a309889b8510a/raw/lgdock.sh) "$@"
    fi
}

lgdock_socat() {
    if command -v lgdock_socat >/dev/null 2>&1; then
        lgdock_socat "$@"
    else
        bash <(curl -sSL https://gist.githubusercontent.com/TUO_USERNAME/83c887ef78610842508b9f972130d3e1/raw/lgdock_socat.sh) "$@"
    fi
}

# Alias
alias lgd='lgdock'
alias lgds='lgdock_socat'
EOF
        
        # Sostituisci USERNAME
        sed -i.bak "s/TUO_USERNAME/$GITHUB_USER/g" "$shell_rc"
        rm -f "${shell_rc}.bak"
        
        log_success "Funzioni aggiunte a $shell_rc"
        echo -e "   ${CYAN}↳${NC} Ricarica con: source $shell_rc"
    fi
}

# =============================================================================
# Modalità di Esecuzione
# =============================================================================

do_install() {
    print_banner
    
    # Check dipendenze
    check_dependencies || {
        log_error "Dipendenze mancanti"
        exit 1
    }
    
    # Check installazioni esistenti
    check_existing_installation || {
        log_info "Installazione annullata"
        exit 0
    }
    
    # Determina directory di installazione
    local install_path="$INSTALL_DIR"
    if [[ "$USER_INSTALL" == true ]]; then
        install_path="$USER_INSTALL_DIR"
        log_info "Installazione locale in: $install_path"
    else
        log_info "Installazione di sistema in: $install_path"
        
        # Check permessi
        if [[ ! -w "$install_path" ]] && ! sudo -n true 2>/dev/null; then
            log_warning "Richiesti permessi sudo per installazione di sistema"
            if ! sudo true; then
                log_error "Permessi sudo negati"
                exit 1
            fi
        fi
    fi
    
    # Menu selezione
    echo
    echo -e "${BOLD}Cosa vuoi installare?${NC}"
    echo "  1) lgdock (script principale)"
    echo "  2) lgdock_socat (con supporto socat)"
    echo "  3) Entrambi gli script"
    echo "  4) Annulla"
    echo
    
    local choice
    read -p "Scelta [1-4]: " choice
    
    case "$choice" in
        1)
            install_script "lgdock.sh" "$LGDOCK_URL" "$install_path" || exit 1
            ;;
        2)
            install_script "lgdock_socat.sh" "$LGDOCK_SOCAT_URL" "$install_path" || exit 1
            ;;
        3)
            install_script "lgdock.sh" "$LGDOCK_URL" "$install_path" || exit 1
            install_script "lgdock_socat.sh" "$LGDOCK_SOCAT_URL" "$install_path" || exit 1
            ;;
        4)
            log_info "Installazione annullata"
            exit 0
            ;;
        *)
            log_error "Scelta non valida"
            exit 1
            ;;
    esac
    
    # Setup opzionale delle funzioni shell
    setup_shell_functions
    
    # Verifica PATH
    if [[ ":$PATH:" != *":$install_path:"* ]]; then
        log_warning "$install_path non è nel PATH"
        echo -e "   ${CYAN}↳${NC} Aggiungi al PATH con: export PATH=\"$install_path:\$PATH\""
    fi
    
    echo
    log_success "Installazione completata!"
    echo
    echo -e "${BOLD}Prossimi passi:${NC}"
    echo -e "  ${CYAN}•${NC} Verifica installazione: lgdock --version"
    echo -e "  ${CYAN}•${NC} Mostra help: lgdock --help"
    echo -e "  ${CYAN}•${NC} Prima esecuzione: lgdock ps"
}

do_update() {
    print_banner
    log_info "Aggiornamento LGDock..."
    
    UPDATE_MODE=true
    check_existing_installation
    
    # Trova dove sono installati
    local lgdock_path=""
    local lgdock_socat_path=""
    
    for dir in "$INSTALL_DIR" "$USER_INSTALL_DIR" "/usr/bin" "/bin"; do
        [[ -f "$dir/lgdock" ]] && lgdock_path="$dir"
        [[ -f "$dir/lgdock_socat" ]] && lgdock_socat_path="$dir"
    done
    
    if [[ -n "$lgdock_path" ]]; then
        log_info "Aggiornamento lgdock..."
        install_script "lgdock.sh" "$LGDOCK_URL" "$lgdock_path" || log_error "Aggiornamento lgdock fallito"
    fi
    
    if [[ -n "$lgdock_socat_path" ]]; then
        log_info "Aggiornamento lgdock_socat..."
        install_script "lgdock_socat.sh" "$LGDOCK_SOCAT_URL" "$lgdock_socat_path" || log_error "Aggiornamento lgdock_socat fallito"
    fi
    
    log_success "Aggiornamento completato!"
}

do_uninstall() {
    print_banner
    log_warning "Disinstallazione LGDock"
    
    confirm "Sei sicuro di voler disinstallare LGDock?" || exit 0
    
    local removed=false
    
    for dir in "$INSTALL_DIR" "$USER_INSTALL_DIR" "/usr/bin" "/bin"; do
        if [[ -f "$dir/lgdock" ]]; then
            log_info "Rimozione $dir/lgdock..."
            if [[ -w "$dir" ]]; then
                rm -f "$dir/lgdock"
            else
                sudo rm -f "$dir/lgdock"
            fi
            removed=true
        fi
        
        if [[ -f "$dir/lgdock_socat" ]]; then
            log_info "Rimozione $dir/lgdock_socat..."
            if [[ -w "$dir" ]]; then
                rm -f "$dir/lgdock_socat"
            else
                sudo rm -f "$dir/lgdock_socat"
            fi
            removed=true
        fi
    done
    
    if [[ "$removed" == true ]]; then
        log_success "LGDock disinstallato"
        echo
        echo "Per rimuovere le funzioni shell, elimina manualmente"
        echo "la sezione '# LGDock Functions' dal tuo .bashrc/.zshrc"
    else
        log_warning "Nessuna installazione trovata"
    fi
}

do_check() {
    print_banner
    log_info "Controllo sistema..."
    
    echo
    echo -e "${BOLD}Sistema:${NC}"
    echo -e "  OS: $(get_os)"
    echo -e "  Shell: ${SHELL##*/}"
    echo -e "  User: $USER"
    echo
    
    echo -e "${BOLD}Dipendenze:${NC}"
    
    # Check curl
    if check_command curl; then
        echo -e "  ${GREEN}✓${NC} curl: $(curl --version | head -n1)"
    else
        echo -e "  ${RED}✗${NC} curl: non trovato"
    fi
    
    # Check wget
    if check_command wget; then
        echo -e "  ${GREEN}✓${NC} wget: $(wget --version | head -n1)"
    else
        echo -e "  ${YELLOW}○${NC} wget: non trovato (opzionale)"
    fi
    
    # Check Docker
    if check_command docker; then
        echo -e "  ${GREEN}✓${NC} docker: $(docker --version)"
    else
        echo -e "  ${YELLOW}○${NC} docker: non trovato (consigliato)"
    fi
    
    # Check socat
    if check_command socat; then
        echo -e "  ${GREEN}✓${NC} socat: $(socat -V 2>&1 | head -n1)"
    else
        echo -e "  ${YELLOW}○${NC} socat: non trovato (richiesto per lgdock_socat)"
    fi
    
    echo
    echo -e "${BOLD}Installazioni LGDock:${NC}"
    
    local found=false
    for dir in "$INSTALL_DIR" "$USER_INSTALL_DIR" "/usr/bin" "/bin" "$HOME/bin"; do
        if [[ -f "$dir/lgdock" ]]; then
            echo -e "  ${GREEN}✓${NC} lgdock: $dir/lgdock"
            if [[ -x "$dir/lgdock" ]]; then
                if "$dir/lgdock" --version 2>/dev/null | grep -q "version"; then
                    echo -e "     Version: $("$dir/lgdock" --version 2>/dev/null | grep -oP 'version \K[0-9.]+' || echo "unknown")"
                fi
            fi
            found=true
        fi
        
        if [[ -f "$dir/lgdock_socat" ]]; then
            echo -e "  ${GREEN}✓${NC} lgdock_socat: $dir/lgdock_socat"
            if [[ -x "$dir/lgdock_socat" ]]; then
                if "$dir/lgdock_socat" --version 2>/dev/null | grep -q "version"; then
                    echo -e "     Version: $("$dir/lgdock_socat" --version 2>/dev/null | grep -oP 'version \K[0-9.]+' || echo "unknown")"
                fi
            fi
            found=true
        fi
    done
    
    if [[ "$found" == false ]]; then
        echo -e "  ${YELLOW}○${NC} Nessuna installazione trovata"
    fi
    
    echo
    echo -e "${BOLD}PATH:${NC}"
    echo -e "  Directories in PATH:"
    echo "$PATH" | tr ':' '\n' | while read -r dir; do
        if [[ "$dir" == "$INSTALL_DIR" ]] || [[ "$dir" == "$USER_INSTALL_DIR" ]]; then
            echo -e "  ${GREEN}→${NC} $dir"
        else
            echo -e "  ${CYAN}•${NC} $dir"
        fi
    done | head -10
    
    echo
    
    # Test connettività
    echo -e "${BOLD}Test Connettività:${NC}"
    
    # Test GitHub
    if curl -sSf https://api.github.com >/dev/null 2>&1; then
        echo -e "  ${GREEN}✓${NC} GitHub API raggiungibile"
    else
        echo -e "  ${RED}✗${NC} GitHub API non raggiungibile"
    fi
    
    # Test Gist
    if curl -sSf "https://gist.githubusercontent.com/$GITHUB_USER" >/dev/null 2>&1; then
        echo -e "  ${GREEN}✓${NC} GitHub Gist raggiungibile"
    else
        echo -e "  ${YELLOW}○${NC} GitHub Gist utente non verificabile"
    fi
    
    echo
}

# =============================================================================
# Gestione Parametri
# =============================================================================

show_help() {
    echo "Uso: $0 [OPZIONI]"
    echo
    echo "Opzioni:"
    echo "  -h, --help          Mostra questo help"
    echo "  -v, --version       Mostra versione"
    echo "  -q, --quiet         Modalità silenziosa"
    echo "  -y, --yes           Accetta automaticamente tutte le conferme"
    echo "  -u, --user          Installa nella home directory dell'utente"
    echo "  --update            Aggiorna installazione esistente"
    echo "  --uninstall         Disinstalla LGDock"
    echo "  --check             Controlla sistema e dipendenze"
    echo "  --all               Installa tutti gli script (con -y)"
    echo
    echo "Esempi:"
    echo "  $0                  # Installazione interattiva"
    echo "  $0 --user           # Installa in ~/.local/bin"
    echo "  $0 -y --all         # Installa tutto automaticamente"
    echo "  $0 --update         # Aggiorna installazione esistente"
    echo "  $0 --check          # Verifica sistema"
    echo
}

show_version() {
    echo "LGDock Installer v${VERSION}"
}

# =============================================================================
# Main
# =============================================================================

main() {
    # Parse parametri
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
            -q|--quiet)
                QUIET=true
                shift
                ;;
            -y|--yes)
                AUTO_YES=true
                shift
                ;;
            -u|--user)
                USER_INSTALL=true
                shift
                ;;
            --update)
                do_update
                exit 0
                ;;
            --uninstall)
                do_uninstall
                exit 0
                ;;
            --check)
                do_check
                exit 0
                ;;
            --all)
                AUTO_YES=true
                shift
                ;;
            *)
                log_error "Opzione sconosciuta: $1"
                echo "Usa -h per vedere l'help"
                exit 1
                ;;
        esac
    done
    
    # Esecuzione principale
    do_install
}

# Trap per pulizia
trap 'echo -e "\n${YELLOW}Installazione interrotta${NC}"; exit 130' INT TERM

# Avvia
main "$@"
