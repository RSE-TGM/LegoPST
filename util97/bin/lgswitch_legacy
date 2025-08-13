#!/bin/bash

# ==============================================================================
# Script per gestire un link simbolico a una directory "legocad".
# Le directory candidate al link devono essere denominate "legocad_*".
# ==============================================================================

# Definiamo alcuni colori per un output più leggibile
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# --- FUNZIONI ---

show_help() {
    echo "Uso: $0 [OPZIONE]"
    echo ""
    echo "Script per gestire un link simbolico chiamato 'legocad'."
    echo ""
    echo "Opzioni:"
    echo "  (nessuna opzione)      Modalità interattiva: cerca, elenca e permette di scegliere una"
    echo "                         directory 'legocad_*'. Se 'legocad' esiste come directory,"
    echo "                         propone di rinominarla in un backup con UID."
    echo ""
    echo "  -s <sorgente> <link>   Crea <link> che punta a <sorgente>. Fallisce se <sorgente> non"
    echo "                         è una directory o se <link> esiste e non è un link simbolico."
    echo "                         Chiede conferma per sovrascrivere un link esistente."
    echo "  -s -f <sorgente> <link> Come -s, ma forza la sovrascrittura del link senza conferma."
    echo "  -h, --help             Mostra questo messaggio di aiuto."
}

# Funzione per la modalità interattiva, che ha una logica di backup separata
interactive_backup_and_link() {
    local source_dir="$1"
    local target_link="$2"

    if [ -e "$target_link" ]; then
        if [ -d "$target_link" ] && [ ! -L "$target_link" ]; then
            local unique_id
            if command -v uuidgen &> /dev/null; then
                unique_id=$(uuidgen)
            else
                unique_id=$(date +%s%N)
            fi
            local backup_name="${target_link}_${unique_id}"

            echo -e "${YELLOW}ATTENZIONE: '${target_link}' è una directory esistente.${NC}"
            read -p "Vuoi rinominarla in '${backup_name}' per procedere? (s/n): " confirm
            confirm=${confirm,,}
            if [[ "$confirm" == "s" || "$confirm" == "si" ]]; then
                echo -e "Rinominando la directory esistente in -> ${GREEN}${backup_name}${NC}"
                mv "$target_link" "$backup_name" || { echo -e "${RED}Errore durante la rinomina. Operazione annullata.${NC}"; return 1; }
            else
                echo "Operazione annullata dall'utente."
                return 2
            fi
        elif [ -L "$target_link" ]; then
             echo -e "${YELLOW}ATTENZIONE: Il link '${target_link}' esiste già.${NC}"
             read -p "Vuoi sovrascriverlo? (s/n): " confirm
             confirm=${confirm,,}
             if [[ "$confirm" == "s" || "$confirm" == "si" ]]; then
                 rm -f "$target_link"
             else
                echo "Operazione annullata dall'utente."
                return 2
             fi
        fi
    fi
    
    echo -e "Creazione del link simbolico: ${GREEN}${target_link}${NC} -> ${GREEN}${source_dir}${NC}"
    ln -s "$source_dir" "$target_link"
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Link '${target_link}' creato/aggiornato con successo!${NC}"
        return 0
    else
        echo -e "${RED}Errore durante la creazione del link.${NC}"
        return 1
    fi
}

# Funzione per la modalità con parametri -s (logica più restrittiva)
set_link_strict() {
    local source_dir="$1"
    local target_link="$2"
    local force_overwrite="$3"

    # 1. Controlla che la sorgente sia una directory
    if [ ! -d "$source_dir" ]; then
        echo -e "${RED}Errore: La sorgente specificata '${source_dir}' non è una directory.${NC}"
        return 1
    fi

    # 2. Controlla la destinazione
    if [ -e "$target_link" ]; then
        # 2a. Se esiste e NON è un link simbolico, rifiuta l'operazione.
        if [ ! -L "$target_link" ]; then
            echo -e "${RED}Errore: La destinazione '${target_link}' esiste già ma non è un link simbolico.${NC}"
            echo -e "${RED}Per sicurezza, l'operazione è stata annullata. Rimuovi o sposta '${target_link}' manualmente.${NC}"
            return 1
        fi
        
        # 2b. Se è un link simbolico, gestisci la sovrascrittura.
        if [ "$force_overwrite" = true ]; then
            echo "Opzione '-f' specificata: rimuovo il vecchio link '${target_link}'."
        else
            echo -e "${YELLOW}ATTENZIONE: Il link simbolico '${target_link}' esiste già.${NC}"
            read -p "Vuoi sovrascriverlo? (s/n): " confirm
            confirm=${confirm,,}
            if ! [[ "$confirm" == "s" || "$confirm" == "si" ]]; then
                echo "Operazione annullata dall'utente."
                return 2
            fi
            echo "Rimozione del vecchio link '${target_link}'..."
        fi
        rm -f "$target_link"
    fi

    # 3. Se tutti i controlli sono passati, crea il link.
    echo -e "Creazione del link simbolico: ${GREEN}${target_link}${NC} -> ${GREEN}${source_dir}${NC}"
    ln -s "$source_dir" "$target_link"

    if [ -L "$target_link" ]; then
        echo -e "${GREEN}Link '${target_link}' creato/aggiornato con successo!${NC}"
        return 0
    else
        echo -e "${RED}Errore durante la creazione del link.${NC}"
        return 1
    fi
}

interactive_mode() {
    mapfile -t dirs < <(find . -maxdepth 1 -type d -name "legocad_*")
    dirs=("${dirs[@]#./}")

    if [ ${#dirs[@]} -eq 0 ]; then
        echo -e "${RED}Nessuna directory che inizia con 'legocad_' trovata.${NC}"
        exit 1
    fi

# Controlla se il link simbolico 'legocad' esiste e se è una directory
    NOME="legocad"

# Controlla se il nome esiste ED è un link simbolico (-L)
    if [ -L "$NOME" ]; then
      # Se è un link, stampa il suo nome e la destinazione
      echo -e "${RED}Attenzione: ${YELLOW}${NOME} -> $(readlink "${NOME}")\n"
# Altrimenti, controlla se è una directory (-d)
    elif [ -d "$NOME" ]; then
      # Se è una directory, stampa l'avviso
      echo -e "${RED}Attenzione: legocad è una directory\n"
# Se non è nessuna delle precedenti (non esiste, è un file normale, etc.), non si stampa nulla
    fi

    echo -e "${YELLOW}Scegli una delle seguenti opzioni:${NC}"
    for i in "${!dirs[@]}"; do
        echo "  $((i+1))) ${dirs[$i]}"
    done
    local exit_option_num=$(( ${#dirs[@]} + 1 ))
    echo "  ${exit_option_num}) Esci (nessuna azione)"
    echo ""

    read -p "Inserisci il numero della scelta [1-${exit_option_num}]: " choice
    if ! [[ "$choice" =~ ^[0-9]+$ ]] || [ "$choice" -lt 1 ] || [ "$choice" -gt "$exit_option_num" ]; then
        echo -e "${RED}Scelta non valida.${NC}"
        exit 1
    fi

    if [ "$choice" -eq "$exit_option_num" ]; then
        echo "Operazione annullata."
        exit 0
    fi
    
    selected_dir="${dirs[$((choice-1))]}"
    target_link="legocad"

    echo -e "Hai scelto: ${GREEN}${selected_dir}${NC}"
    echo ""
    
    interactive_backup_and_link "$selected_dir" "$target_link"
    exit $?
}

# --- LOGICA PRINCIPALE ---

if [ $# -eq 0 ]; then
    interactive_mode
fi

case "$1" in
    -h|--help)
        show_help; exit 0 ;;
    -s|--set)
        shift 
        force=false
        if [ "$1" == "-f" ]; then
            force=true; shift
        fi
        if [ $# -ne 2 ]; then
            echo -e "${RED}Errore: numero di argomenti non corretto.${NC}" >&2
            show_help; exit 1
        fi
        set_link_strict "$1" "$2" "$force"
        exit $?
        ;;
    *)
        echo -e "${RED}Errore: opzione non riconosciuta '$1'.${NC}" >&2
        show_help; exit 1
        ;;
esac
