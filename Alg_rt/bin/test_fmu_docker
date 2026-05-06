#!/bin/bash

# ==========================================
# GESTIONE PARAMETRI (-t e lista file)
# ==========================================
STOP_TIME=10 # Valore di default

# Controlla se il primo parametro è "-t"
if [ "$1" == "-t" ]; then
    if [ -z "$2" ]; then
        echo "Errore: devi specificare i secondi dopo -t"
        exit 1
    fi
    STOP_TIME="$2"
    shift 2 # Rimuove "-t" e il "numero" dalla lista degli argomenti
fi

# Se dopo aver tolto il tempo non ci sono file, mostra l'aiuto
if [ "$#" -lt 1 ] || [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
    echo "Uso: $0 [-t secondi_transitorio] <file_fmu_1> [file_fmu_2] ..."
    echo "Esempi:"
    echo "  $0 modello.fmu                    (usa 10 sec di default)"
    echo "  $0 -t 25 modello.fmu              (simula per 25 secondi)"
    echo "  $0 -t 5 mod_A.fmu mod_B.fmu       (simula A e B in parallelo per 5 sec)"
    exit 1
fi

# ==========================================
# PREPARAZIONE VOLUMI DOCKER DINAMICI
# ==========================================
DOCKER_VOLUMES=""
CONTAINER_FILES=""
INDEX=1

echo ">>> Preparazione di $# file FMU..."

for FMU_FILE in "$@"; do
    if [ ! -f "$FMU_FILE" ]; then
        echo "Errore: Il file '$FMU_FILE' non esiste. Interruzione."
        exit 1
    fi
    
    ABS_PATH=$(realpath "$FMU_FILE")
    # Crea la stringa dei volumi es: -v /mio/path.fmu:/tmp/fmu_1.fmu:ro
    DOCKER_VOLUMES="$DOCKER_VOLUMES -v $ABS_PATH:/tmp/fmu_$INDEX.fmu:ro"
    # Salva la lista dei percorsi interni al container
    CONTAINER_FILES="$CONTAINER_FILES /tmp/fmu_$INDEX.fmu"
    
    INDEX=$((INDEX + 1))
done

# ==========================================
# ABILITAZIONE GRAFICA X11/WAYLAND
# ==========================================
xhost +local:docker > /dev/null 2>&1

# ==========================================
# ESECUZIONE DOCKER
# ==========================================
# Nota: Passiamo le variabili DOCKER_VOLUMES (senza virgolette per farle espandere)
# Passiamo STOP_TIME e FILES come variabili d'ambiente dentro il container.

docker run --rm -it \
  --net host \
  -e DISPLAY=$DISPLAY \
  -e STOP_TIME="$STOP_TIME" \
  -e FILES="$CONTAINER_FILES" \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  $DOCKER_VOLUMES \
  python:3.11-slim bash -c '

echo ">>> [Container] Installazione fmpy..."
pip install --quiet fmpy

# Creiamo un file python temporaneo dentro il container. 
# È molto più pulito che fare tutto su una sola riga!
cat << "EOF" > /tmp/simula.py
import sys, os
from fmpy import simulate_fmu, extract

fmu_path = sys.argv[1]
extract_dir = sys.argv[2]
stop_time = float(os.environ.get("STOP_TIME", 10))

print(f"[{fmu_path}] Estrazione in {extract_dir}...")
extract(fmu_path, unzipdir=extract_dir)

try:
    print(f"[{fmu_path}] Simulazione per {stop_time} secondi...")
    result = simulate_fmu(extract_dir, stop_time=stop_time)
    print(f"[{fmu_path}] OK: {len(result)} sample generati.")
except Exception as e:
    print(f"[{fmu_path}] ERRORE: {e}")
EOF

echo ">>> [Container] Avvio simulazioni in parallelo..."

# Ciclo su tutti i file passati nella variabile d ambiente $FILES
for f in $FILES; do
    # Crea un blocco ( ... ) & per eseguire ogni file in background (parallelo)
    (
        # Estraiamo il nome file senza estensione per creare cartelle separate
        BASENAME=$(basename $f .fmu)
        UNZ_DIR="/tmp/dir_$BASENAME"
        
        # 1. Lancia lo script python per questo file
        python /tmp/simula.py "$f" "$UNZ_DIR"
        
        # 2. Avvia la grafica
        cd "$UNZ_DIR"
        if [ -f "./resources/bundle/run_graphics.sh" ]; then
            chmod +x ./resources/bundle/run_graphics.sh
            ./resources/bundle/run_graphics.sh
        else
            echo "[$f] ATTENZIONE: Script grafico non trovato."
        fi
    ) & 
done

# Comando cruciale: aspetta che tutti i processi in background (&) finiscano
# prima di distruggere il container Docker.
wait

echo ">>> [Container] Tutte le simulazioni completate. Uscita."
'
