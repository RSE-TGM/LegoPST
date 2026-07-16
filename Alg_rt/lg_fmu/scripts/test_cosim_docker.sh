#!/bin/bash
#
# test_cosim_docker.sh — co-simulazione ACCOPPIATA di N FMU bundle LegoPST in un
# container Docker EFFIMERO e PULITO (nessun LegoPST a bordo), via lg_cosim.py.
#
# Prova di portabilita': dimostra che per far girare piu' FMU accoppiate
# (uscite di una -> ingressi di un'altra) bastano le .fmu bundle self-contained
# + fmpy; LegoPST NON serve installato sul target.
#
# A differenza di test_fmu_docker.sh (che lancia le FMU INDIPENDENTI in
# parallelo, senza connessioni), qui gira il MASTER lg_cosim.py che legge il
# lg_cosim.json e scambia le variabili collegate a ogni passo.
#
# Uso:
#   test_cosim_docker.sh [opzioni] [<lg_cosim.json>]
#
#   <lg_cosim.json>  configurazione lg_cosim (default: <lg_cosim>/lg_cosim.json).
#                  I path delle FMU e del log sono risolti dalla dir del config
#                  (cosi' vengono montati tutti insieme nel container).
#
# Opzioni:
#   -t, --stop-time SEC  override di stop_time del config
#   -s, --speedup K      pacing: 1.0 = tempo reale, 5.0 = 5x, assente = massimo
#   -i, --image IMG      immagine Docker (default: python:3.11-slim)
#   -d, --debug          LG_FMU_DEBUG=1 + lg_cosim --debug (diagnostica FMU)
#   -x, --x11            abilita X11 (se una FMU apre grafica/HMI)
#   -k, --keep           non usa --rm (per ispezionare il container dopo)
#   -h, --help
#
# Esempi:
#   test_cosim_docker.sh                              # config di default, max velocita'
#   test_cosim_docker.sh -s 1.0 -t 60                 # tempo reale, 60 s
#   test_cosim_docker.sh /path/mio_sistema/lg_cosim.json
#   test_cosim_docker.sh -i debian:12 -d lg_cosim.json  # altra immagine + debug
#
# Uscita: 0 co-sim ok, 1 args, 2 config non trovato, 3 docker non disponibile,
#         !=0 = codice di lg_cosim (co-sim fallita).
#
# Nota: le FMU devono essere generate con il codice corrente (la .so del bundle
# ripristina da sola i bit +x dopo l'estrazione via fmpy). FMU vecchie potrebbero
# non essere self-contained: rigenerarle con `dolgfmu -b <task>`.

set -u

# ---- default -------------------------------------------------------------
IMAGE="python:3.11-slim"
STOP_TIME=""
SPEEDUP=""
DEBUG=0
X11=0
RM_FLAG="--rm"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
LG_COSIM_DIR="$(cd "$SCRIPT_DIR/../lg_cosim" 2>/dev/null && pwd || true)"
LG_COSIM_PY="$LG_COSIM_DIR/lg_cosim.py"
CONFIG=""

# ---- parsing opzioni -----------------------------------------------------
while [ $# -gt 0 ]; do
    case "$1" in
        -t|--stop-time) STOP_TIME="${2:-}"; shift 2 ;;
        -s|--speedup)   SPEEDUP="${2:-}";   shift 2 ;;
        -i|--image)     IMAGE="${2:-}";     shift 2 ;;
        -d|--debug)     DEBUG=1;            shift ;;
        -x|--x11)       X11=1;              shift ;;
        -k|--keep)      RM_FLAG="";         shift ;;
        -h|--help)      sed -n '2,45p' "$0"; exit 1 ;;
        -*)             echo "ERRORE: opzione sconosciuta '$1'." >&2; exit 1 ;;
        *)              if [ -n "$CONFIG" ]; then
                            echo "ERRORE: config specificato piu' volte." >&2; exit 1
                        fi
                        CONFIG="$1"; shift ;;
    esac
done

[ -n "$CONFIG" ] || CONFIG="$LG_COSIM_DIR/lg_cosim.json"

# ---- validazioni ---------------------------------------------------------
if [ ! -f "$LG_COSIM_PY" ]; then
    echo "ERRORE: lg_cosim.py non trovato in $LG_COSIM_DIR" >&2; exit 2
fi
if [ ! -f "$CONFIG" ]; then
    echo "ERRORE: config '$CONFIG' non trovato." >&2; exit 2
fi
if ! command -v docker >/dev/null 2>&1; then
    echo "ERRORE: docker non trovato nel PATH." >&2; exit 3
fi
if ! docker info >/dev/null 2>&1; then
    echo "ERRORE: daemon Docker non raggiungibile (permessi? servizio attivo?)." >&2; exit 3
fi

CFG_ABS="$(realpath "$CONFIG")"
CFG_DIR="$(dirname "$CFG_ABS")"
CFG_BASE="$(basename "$CFG_ABS")"

# flag da passare a lg_cosim.py
COSIM_FLAGS=""
[ -n "$STOP_TIME" ] && COSIM_FLAGS="$COSIM_FLAGS --stop-time $STOP_TIME"
[ -n "$SPEEDUP" ]   && COSIM_FLAGS="$COSIM_FLAGS --speedup $SPEEDUP"
[ "$DEBUG" -eq 1 ]  && COSIM_FLAGS="$COSIM_FLAGS --debug"

# X11 (opzionale): la co-sim di default logga soltanto, ma una FMU potrebbe
# aprire grafica. --net host + socket X + DISPLAY.
X11_ARGS=""
if [ "$X11" -eq 1 ]; then
    xhost +local:docker >/dev/null 2>&1 || true
    X11_ARGS="--net host -e DISPLAY=${DISPLAY:-} -v /tmp/.X11-unix:/tmp/.X11-unix:rw"
fi

echo ">>> lg_cosim in Docker ($IMAGE) — nessun LegoPST nel container"
echo "    config : $CFG_ABS"
echo "    FMU dir: $CFG_DIR   (montata READ-ONLY: estrazioni/log restano nel container)"
echo "    flags  : ${COSIM_FLAGS:-<da config>}"
echo

# ---- bootstrap dentro il container --------------------------------------
# 1) prova onesta: LegoPST NON deve esserci.  2) installa fmpy (con fallback
# PEP 668 per Python "externally-managed").  3) lancia il master lg_cosim.
BOOTSTRAP='
set -e
echo "[container] distro: $(. /etc/os-release 2>/dev/null; echo "${PRETTY_NAME:-?}")"
echo "[container] LegoPST installato? $(command -v net_sked 2>/dev/null || echo NO)"
if ! command -v pip3 >/dev/null 2>&1 && ! command -v pip >/dev/null 2>&1; then
    if   command -v apt-get  >/dev/null 2>&1; then apt-get update -qq && apt-get install -y -qq python3-pip
    elif command -v dnf      >/dev/null 2>&1; then dnf install -y -q python3-pip
    elif command -v microdnf >/dev/null 2>&1; then microdnf install -y python3-pip
    elif command -v apk      >/dev/null 2>&1; then apk add --no-cache py3-pip
    fi
fi
PIP="$(command -v pip3 || command -v pip)"
echo "[container] installo fmpy..."
"$PIP" install --quiet --no-input fmpy 2>/dev/null \
    || "$PIP" install --quiet --no-input --break-system-packages fmpy
# Copio in una dir effimera del container: lg_cosim estrae le FMU accanto al
# .fmu -> cosi le estrazioni restano nel container (host montato read-only,
# niente file di root che sporcano la dir del config).
echo "[container] copio in /run/cosim (host read-only)..."
mkdir -p /run/cosim && cp -a /src/. /run/cosim/
cd /run/cosim
echo "[container] avvio lg_cosim..."
python3 /opt/lg_cosim.py "'"$CFG_BASE"'" '"$COSIM_FLAGS"'
_LOG=$(ls -1 *.csv 2>/dev/null | head -1)
[ -n "$_LOG" ] && { echo; echo "--- log $_LOG (ultime righe) ---"; tail -n 6 "$_LOG"; }
'

# ---- esecuzione ----------------------------------------------------------
# /work = dir del config (rw: FMU, log, estrazioni).  /opt/lg_cosim.py = master.
# La FMU (lg_fmi2.c) ripristina da sola i +x dopo l'estrazione via fmpy, quindi
# non serve montare LegoPST ne' fare chmod: e' il senso del test self-contained.
# -t solo se siamo su un terminale (altrimenti "the input device is not a TTY").
TTY_FLAG=""
[ -t 1 ] && TTY_FLAG="-t"
docker run $RM_FLAG $TTY_FLAG \
    $X11_ARGS \
    -e LG_FMU_DEBUG="$DEBUG" \
    -v "$CFG_DIR":/src:ro \
    -v "$LG_COSIM_PY":/opt/lg_cosim.py:ro \
    "$IMAGE" bash -c "$BOOTSTRAP"
RC=$?

echo
if [ "$RC" -eq 0 ]; then
    echo ">>> CO-SIM OK (exit 0): le FMU accoppiate girano in container pulito (senza LegoPST)."
    echo "    (log ed estrazioni sono restati nel container; con -k puoi ispezionarlo)"
else
    echo ">>> FALLITO (exit $RC): vedi l'output sopra." >&2
fi
exit $RC
