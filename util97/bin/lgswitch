#!/bin/bash

# ==============================================================================
# Script per gestire link simbolici a directory "legocad" e "sked".
# Le directory candidate al link devono essere denominate "legopst_*".
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
    echo "Script per gestire link simbolici chiamati 'legocad' e 'sked'."
    echo ""
    echo "Opzioni:"
    echo "  (nessuna opzione)      Modalità interattiva: cerca, elenca e permette di scegliere una"
    echo "                         directory 'legopst_*'. Crea link simbolici per 'legocad' e 'sked'"
    echo "                         se le sottodirectory corrispondenti esistono."
    echo ""
    echo "  <dir>                  Cerca all'interno della directory <dir> la presenza di 'legocad'"
    echo "                         e 'sked' e crea i link simbolici ./legocad e ./sked come al solito."
    echo ""
    echo "  -f <dir>               Come <dir>, ma non chiede conferma per la creazione dei link."
    echo ""
    echo "  -s <sorgente> <link>   Crea <link> che punta a <sorgente>. Fallisce se <sorgente> non"
    echo "                         è una directory o se <link> esiste e non è un link simbolico."
    echo "                         Chiede conferma per sovrascrivere un link esistente."
    echo "  -s -f <sorgente> <link> Come -s, ma forza la sovrascrittura del link senza conferma."
    echo "  -h, --help             Mostra questo messaggio di aiuto."
}

# Funzione per gestire backup e creazione link per un singolo target
handle_single_link() {
    local source_dir="$1"
    local target_link="$2"
    local force_mode="${3:-false}"
    
    if [ -e "$target_link" ]; then
        if [ -d "$target_link" ] && [ ! -L "$target_link" ]; then
            local unique_id
            if command -v uuidgen &> /dev/null; then
                unique_id=$(uuidgen)
            else
                unique_id=$(date +%s%N)
            fi
            local backup_name="${target_link}_${unique_id}"

            if [ "$force_mode" = true ]; then
                echo -e "${YELLOW}Modalità -f: rinomino automaticamente '${target_link}' in '${backup_name}'${NC}"
                mv "$target_link" "$backup_name" || { echo -e "${RED}Errore durante la rinomina di ${target_link}. Link non creato.${NC}"; return 1; }
            else
                echo -e "${YELLOW}ATTENZIONE: '${target_link}' è una directory esistente.${NC}"
                read -p "Vuoi rinominarla in '${backup_name}' per procedere? (S/n): " confirm
                confirm=${confirm,,}
                if [[ "$confirm" == "s" || "$confirm" == "si" || "$confirm" == "" ]]; then
                    echo -e "Rinominando la directory esistente in -> ${GREEN}${backup_name}${NC}"
                    mv "$target_link" "$backup_name" || { echo -e "${RED}Errore durante la rinomina di ${target_link}. Link non creato.${NC}"; return 1; }
                else
                    echo "Rinomina di '$target_link' annullata dall'utente. Link non creato."
                    return 2
                fi
            fi
        elif [ -L "$target_link" ]; then
            if [ "$force_mode" = true ]; then
                echo -e "${YELLOW}Modalità -f: rimuovo automaticamente il link esistente '${target_link}'${NC}"
                rm -f "$target_link"
            else
                echo -e "${YELLOW}ATTENZIONE: Il link '${target_link}' esiste già.${NC}"
                read -p "Vuoi sovrascriverlo? (S/n): " confirm
                confirm=${confirm,,}
                if [[ "$confirm" == "s" || "$confirm" == "si" || "$confirm" == "" ]]; then
                    rm -f "$target_link"
                else
                    echo "Sovrascrittura di '$target_link' annullata dall'utente. Link non creato."
                    return 2
                fi
            fi
        fi
    fi
    
    echo -e "Creazione del link simbolico: ${GREEN}${target_link}${NC} -> ${GREEN}${source_dir}${NC}"
    ln -s "$source_dir" "$target_link"
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Link '${target_link}' creato/aggiornato con successo!${NC}"
        return 0
    else
        echo -e "${RED}Errore durante la creazione del link '${target_link}'.${NC}"
        return 1
    fi
}

# Funzione per la modalità interattiva con gestione di legocad e sked
interactive_backup_and_link() {
    local selected_dir="$1"
    local force_mode="${2:-false}"
    local legocad_path="${selected_dir}/legocad"
    local sked_path="${selected_dir}/sked"
    local success=0
    
    echo -e "${YELLOW}Controllo delle sottodirectory disponibili in '${selected_dir}':${NC}"
    
    # Gestisci legocad
    if [ -d "$legocad_path" ]; then
        echo -e "Trovata sottodirectory: ${GREEN}${legocad_path}${NC}"
        handle_single_link "$legocad_path" "legocad" "$force_mode"
        if [ $? -ne 0 ]; then
            success=1
        fi
    else
        echo -e "Sottodirectory '${legocad_path}' non trovata. Link per 'legocad' non creato."
    fi
    
    echo ""
    
    # Gestisci sked
    if [ -d "$sked_path" ]; then
        echo -e "Trovata sottodirectory: ${GREEN}${sked_path}${NC}"
        handle_single_link "$sked_path" "sked" "$force_mode"
        if [ $? -ne 0 ]; then
            success=1
        fi
    else
        echo -e "Sottodirectory '${sked_path}' non trovata. Link per 'sked' non creato."
    fi
    
    return $success
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

# Funzione per modalità directory (nuovo)
directory_mode() {
    local target_dir="$1"
    local force_mode="${2:-false}"
    
    # Controlla che la directory specificata esista
    if [ ! -d "$target_dir" ]; then
        echo -e "${RED}Errore: La directory specificata '${target_dir}' non esiste.${NC}"
        exit 1
    fi
    
    if [ "$force_mode" = true ]; then
        echo -e "${YELLOW}Modalità directory con -f: cercando 'legocad' e 'sked' in '${target_dir}' (nessuna conferma richiesta)${NC}"
    else
        echo -e "${YELLOW}Modalità directory: cercando 'legocad' e 'sked' in '${target_dir}'${NC}"
    fi
    
    # Usa la stessa logica di interactive_backup_and_link
    interactive_backup_and_link "$target_dir" "$force_mode"
    exit $?
}

interactive_mode() {
    mapfile -t dirs < <(find . -maxdepth 1 -type d -name "legopst_*")
    dirs=("${dirs[@]#./}")

    if [ ${#dirs[@]} -eq 0 ]; then
        echo -e "${RED}Nessuna directory che inizia con 'legopst_' trovata.${NC}"
        exit 1
    fi

# Controlla se i link simbolici 'legocad' e 'sked' esistono
    echo -e "${YELLOW}Stato attuale dei link:${NC}"
    
    # Controlla legocad
    if [ -L "legocad" ]; then
        echo -e "${RED}Attenzione: ${YELLOW}legocad -> $(readlink "legocad")${NC}"
    elif [ -d "legocad" ]; then
        echo -e "${RED}Attenzione: legocad è una directory${NC}"
    else
        echo -e "legocad: non esiste"
    fi
    
    # Controlla sked
    if [ -L "sked" ]; then
        echo -e "${RED}Attenzione: ${YELLOW}sked -> $(readlink "sked")${NC}"
    elif [ -d "sked" ]; then
        echo -e "${RED}Attenzione: sked è una directory${NC}"
    else
        echo -e "sked: non esiste"
    fi
    
    echo ""

    echo -e "${YELLOW}Scegli una delle seguenti opzioni:${NC}"
    for i in "${!dirs[@]}"; do
        local dir="${dirs[$i]}"
        local legocad_status=""
        local sked_status=""
        
        if [ -d "$dir/legocad" ]; then
            legocad_status=" [legocad✓]"
        fi
        if [ -d "$dir/sked" ]; then
            sked_status=" [sked✓]"
        fi
        
        echo "  $((i+1))) ${dir}${legocad_status}${sked_status}"
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

    echo -e "Hai scelto: ${GREEN}${selected_dir}${NC}"
    echo ""
    
    interactive_backup_and_link "$selected_dir"
    exit $?
}

# --- LOGICA PRINCIPALE ---

if [ $# -eq 0 ]; then
    interactive_mode
fi

case "$1" in
    -h|--help)
        show_help; exit 0 ;;
    -f)
        # Modalità force per directory
        if [ $# -ne 2 ]; then
            echo -e "${RED}Errore: -f richiede esattamente una directory come argomento.${NC}" >&2
            show_help; exit 1
        fi
        directory_mode "$2" true
        ;;
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
    -*)
        echo -e "${RED}Errore: opzione non riconosciuta '$1'.${NC}" >&2
        show_help; exit 1
        ;;
    *)
        # Se non inizia con '-', assumiamo sia una directory
        if [ $# -eq 1 ]; then
            directory_mode "$1"
        else
            echo -e "${RED}Errore: troppi argomenti. Aspetto una sola directory.${NC}" >&2
            show_help; exit 1
        fi
        ;;
esac
