#!/bin/bash

# ==========================================
# CONFIGURAZIONE
# ==========================================
DOCKER_IMAGE="python:3.11-slim"
X11_SOCK_DIR="/tmp/.X11-unix"
CONTAINER_WORK="/tmp"   # working dir dentro il container (fmu mount, extract, script)
STOP_TIME=10            # secondi di default

# ==========================================
# GESTIONE PARAMETRI (-t e lista file)
# ==========================================
if [ "$1" == "-t" ]; then
    if [ -z "$2" ]; then
        echo "Errore: devi specificare i secondi dopo -t"
        exit 1
    fi
    STOP_TIME="$2"
    shift 2
fi

if [ "$#" -lt 1 ] || [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
    echo "Lancia una o più FMU bundle in un container Docker effimero ($DOCKER_IMAGE),"
    echo "simulando in parallelo e aprendo la grafica post-run se disponibile."
    echo ""
    echo "Uso: $0 [-t secondi] <fmu_1> [fmu_2 ...]"
    echo ""
    echo "  -t secondi   durata simulazione (default: $STOP_TIME)"
    echo ""
    echo "Esempi:"
    echo "  $0 modello.fmu"
    echo "  $0 -t 25 modello.fmu"
    echo "  $0 -t 5 mod_A.fmu mod_B.fmu"
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
    DOCKER_VOLUMES="$DOCKER_VOLUMES -v $ABS_PATH:$CONTAINER_WORK/fmu_$INDEX.fmu:ro"
    CONTAINER_FILES="$CONTAINER_FILES $CONTAINER_WORK/fmu_$INDEX.fmu"

    INDEX=$((INDEX + 1))
done

# ==========================================
# ABILITAZIONE GRAFICA X11
# ==========================================
xhost +local:docker > /dev/null 2>&1

# ==========================================
# ESECUZIONE DOCKER
# ==========================================
docker run --rm -it \
  --net host \
  -e DISPLAY="$DISPLAY" \
  -e STOP_TIME="$STOP_TIME" \
  -e FILES="$CONTAINER_FILES" \
  -e CONTAINER_WORK="$CONTAINER_WORK" \
  -v "$X11_SOCK_DIR:$X11_SOCK_DIR:rw" \
  $DOCKER_VOLUMES \
  "$DOCKER_IMAGE" bash -c '

echo ">>> [Container] Installazione fmpy..."
pip install --quiet fmpy

cat << "EOF" > "$CONTAINER_WORK/simula.py"
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

for f in $FILES; do
    (
        BASENAME=$(basename "$f" .fmu)
        UNZ_DIR="$CONTAINER_WORK/dir_$BASENAME"

        python "$CONTAINER_WORK/simula.py" "$f" "$UNZ_DIR"

        cd "$UNZ_DIR"
        if [ -f "./resources/bundle/run_graphics.sh" ]; then
            chmod +x ./resources/bundle/run_graphics.sh
            ./resources/bundle/run_graphics.sh
        else
            echo "[$f] ATTENZIONE: Script grafico non trovato."
        fi
    ) &
done

wait

echo ">>> [Container] Tutte le simulazioni completate. Uscita."
'
