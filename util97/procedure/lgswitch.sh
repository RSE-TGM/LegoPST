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

#  Nome della copia di sicurezza per una directory VERA trovata al posto del
#  link. Datato e ordinabile, non un uuid: deve dire QUANDO e' stata messa da
#  parte, perche' e' l'unico modo di capire piu' tardi se serve ancora.
#
#  Il suffisso e' ".prelink-", non "_<qualcosa>": "legopst_*" e "legocad_*" sono
#  i modelli con cui lgswitch e lgswitch_legacy CERCANO le aree di lavoro, e una
#  copia di sicurezza che finisce nel loro menu come se fosse un'area e' peggio
#  che inutile.
nome_backup() {
    local base="$1" nome n
    nome="${base}.prelink-$(date +%Y%m%d-%H%M%S)"
    n=2
    while [ -e "$nome" ]; do        # due switch nello stesso secondo
        nome="${base}.prelink-$(date +%Y%m%d-%H%M%S)_${n}"
        n=$((n + 1))
    done
    printf '%s' "$nome"
}

#  Elenca le copie di sicurezza lasciate dalle esecuzioni precedenti. Sono
#  directory INTERE, non link: se nessuno le guarda restano li' per sempre a
#  occupare disco senza che niente lo dica. Vengono solo elencate - cancellarle
#  e' una decisione di chi lavora, non di questo script.
mostra_backup() {
    local trovati=0 d
    for d in ./*.prelink-*; do
        [ -d "$d" ] || continue
        if [ $trovati -eq 0 ]; then
            echo ""
            echo -e "${YELLOW}Copie di sicurezza lasciate da esecuzioni precedenti:${NC}"
            trovati=1
        fi
        printf '  %-46s %s\n' "${d#./}" "$(du -sh "$d" 2>/dev/null | cut -f1)"
    done
    if [ $trovati -eq 1 ]; then
        echo "  lgswitch non le usa: cancellale quando sei sicuro di non volerle piu'."
    fi
}

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
    echo ""
    echo "Cosa succede a quello che c'e' gia':"
    echo "  - un LINK viene sostituito (con conferma, salvo -f);"
    echo "  - una DIRECTORY VERA non viene mai cancellata: viene rinominata in"
    echo "    <nome>.prelink-AAAAMMGG-HHMMSS e lo script stampa il comando per"
    echo "    tornare indietro. Le copie cosi' create vengono elencate a ogni"
    echo "    esecuzione, con la loro dimensione, finche' non le cancelli."
    echo ""
    echo "  Se l'area scelta ha 'legocad' ma non 'sked' (o viceversa), il link"
    echo "  mancante NON viene creato e quello vecchio resterebbe puntato"
    echo "  all'area precedente: lo script lo segnala e esce con stato 1, perche'"
    echo "  lavorare con legocad e sked di aree diverse e' un errore silenzioso."
}

# Funzione per gestire backup e creazione link per un singolo target
handle_single_link() {
    local source_dir="$1"
    local target_link="$2"
    local force_mode="${3:-false}"
    if [ -e "$target_link" ] || [ -L "$target_link" ]; then
        if [ -d "$target_link" ] && [ ! -L "$target_link" ]; then
            local backup_name
            backup_name=$(nome_backup "$target_link")

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
            echo -e "${YELLOW}'${target_link}' era una directory vera: e' stata messa da parte,${NC}"
            echo -e "${YELLOW}non cancellata. Per tornare indietro:${NC}"
            echo "    rm -f '${target_link}' && mv '${backup_name}' '${target_link}'"
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
        else
            #  Esiste, ma non e' ne' una directory ne' un link: un file normale,
            #  o un link rotto che -d non vede. Senza questo ramo si finiva su
            #  ln -s, che falliva con "File exists" senza spiegare perche'.
            echo -e "${RED}Errore: '${target_link}' esiste e non e' ne' una directory ne' un link.${NC}"
            echo -e "${RED}Per sicurezza non viene toccato: spostalo o cancellalo a mano.${NC}"
            return 1
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
    local saltati=""

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
        saltati="$saltati legocad"
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
        saltati="$saltati sked"
    fi

    #  Un link rimasto indietro e' PEGGIO di un link mancante: se l'area scelta
    #  ha legocad ma non sked, il vecchio sked continua a puntare all'area
    #  precedente e si finisce a lavorare con meta' configurazione di un
    #  impianto e meta' di un altro, senza che niente lo dica. Prima di questo
    #  controllo lo script usciva con 0, cioe' dichiarando che era andato bene.
    local nome
    for nome in $saltati; do
        if [ -L "$nome" ]; then
            echo ""
            echo -e "${RED}ATTENZIONE: '${nome}' punta ancora a '$(readlink "$nome")'.${NC}"
            echo -e "${RED}'${selected_dir}' non ha '${nome}', quindi il link vecchio e' rimasto:${NC}"
            echo -e "${RED}legocad e sked verrebbero da AREE DIVERSE.${NC}"
            echo "Come rimediare, a seconda di cosa vuoi:"
            echo "  - lavorare senza '${nome}':"
            echo "        rm -f '${nome}'"
            echo "  - puntarlo di proposito a un'altra area:"
            echo "        lgswitch -s <area>/${nome} ${nome}"
            success=1
        elif [ -d "$nome" ]; then
            echo ""
            echo -e "${YELLOW}Nota: '${nome}' e' una directory vera e resta dov'e'.${NC}"
            echo -e "${YELLOW}'${selected_dir}' non ha '${nome}', quindi non e' stata toccata.${NC}"
        fi
    done

    mostra_backup

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
