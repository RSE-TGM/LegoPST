#!/usr/bin/env bash
#
# test_selfcontained_docker.sh — prova di self-containment di un bundle FMU
# LegoCliSINC in un container Docker EFFIMERO e PULITO (nessun LegoPST a bordo).
#
# A cosa serve
# -----------
# La variante "bundle" della FMU (prodotta da dolgfmu.sh -b) deve girare su una
# macchina che NON ha LegoPST installato: porta dentro lo zip il runtime LegoPST
# (dispatcher, net_sked, killsim, net_prepf22, initav, tavole del vapore, le
# shared lib non di sistema) e la task. Questo script lo verifica nel modo piu'
# onesto possibile: monta SOLO il file .fmu (read-only) in un container con
# nient'altro che Python, installa fmpy, ed esegue extract + simulate. Se la sim
# produce sample con valori fisici sensati, il bundle e' davvero autoconsistente.
#
# A differenza di test_fmu_docker.sh (interattivo, X11, apre la grafica
# 'graphics' post-run), questo e' BATCH e HEADLESS: nessun DISPLAY, nessun mount
# del socket X11. E' lo strumento giusto per CI / verifica di portabilita'.
#
# Uso:
#   test_selfcontained_docker.sh [opzioni] [<fmu_path|task_dir>]
#
# Argomento posizionale (come run_fmu.sh):
#   <fmu_path>   path a un .fmu bundle da testare
#   <task_dir>   dir di una task: usa <task_dir>/legoclix_<task>_bundle.fmu,
#                altrimenti il primo *_bundle.fmu trovato nella dir
#   (se omesso) cerca il primo *_bundle.fmu nella cwd
#
# Opzioni:
#   -i, --image IMG    immagine Docker (default: python:3.11-slim).
#                      Provate note: python:3.11-slim (debian, pip pronto),
#                      fedora:41, ubuntu:24.04 (per queste lo script installa
#                      pip da solo via dnf/apt). NB: il baseline glibc dei
#                      binari del bundle e' ~2.38 -> immagini piu' vecchie
#                      (Ubuntu 20.04/22.04) falliscono: serve un rebuild.
#   -t, --stop-time T  secondi di simulazione (default: 15)
#   -q, --quiet        filtra il rumore di debug 'to_dispatcher: ... GUAG ...'
#                      del dispatcher dall'output del container
#   -k, --keep         non passare --rm: lascia il container per ispezione
#                      (utile per debug; rimuovilo poi con `docker rm`)
#   -h, --help
#
# Exit code:
#   0  self-containment OK (sim conclusa con sample)
#   1  errore argomenti
#   2  .fmu non trovata
#   3  docker non disponibile
#   4  simulazione fallita nel container
#
# Esempi:
#   # bundle nella cwd, default debian, 15 s
#   test_selfcontained_docker.sh
#
#   # bundle esplicito, output pulito
#   test_selfcontained_docker.sh -q /home/antonio/legocad/collet/legoclix_collet_bundle.fmu
#
#   # stessa prova ma su Fedora 41 (pip installato dallo script via dnf)
#   test_selfcontained_docker.sh -i fedora:41 -t 30 /home/antonio/legocad/collet
#
#   # risolvi il bundle dalla dir della task
#   test_selfcontained_docker.sh /home/antonio/legocad/collet

set -u

usage() { sed -n '2,61p' "$0" | sed 's/^# \{0,1\}//'; exit 0; }

# ---- default ------------------------------------------------------------
IMAGE="python:3.11-slim"
STOP_TIME=15
QUIET=0
RM_FLAG="--rm"
FMU_ARG=""

# ---- arg parsing --------------------------------------------------------
while [ $# -gt 0 ]; do
    case "$1" in
        -i|--image)     IMAGE="${2:?manca IMG dopo $1}"; shift 2 ;;
        -t|--stop-time) STOP_TIME="${2:?manca T dopo $1}"; shift 2 ;;
        -q|--quiet)     QUIET=1; shift ;;
        -k|--keep)      RM_FLAG=""; shift ;;
        -h|--help)      usage ;;
        -*)             echo "ERRORE: opzione sconosciuta '$1' (vedi -h)" >&2; exit 1 ;;
        *)
            if [ -n "$FMU_ARG" ]; then
                echo "ERRORE: argomento posizionale specificato piu' volte." >&2
                exit 1
            fi
            FMU_ARG="$1"; shift ;;
    esac
done

# ---- docker disponibile? ------------------------------------------------
if ! command -v docker >/dev/null 2>&1; then
    echo "ERRORE: docker non trovato nel PATH." >&2; exit 3
fi
if ! docker info >/dev/null 2>&1; then
    echo "ERRORE: il daemon Docker non e' raggiungibile (permessi? servizio attivo?)." >&2
    exit 3
fi

# ---- risoluzione del file .fmu (stile run_fmu.sh) -----------------------
# Regole: file esplicito -> usato; dir -> legoclix_<dir>_bundle.fmu o primo
# *_bundle.fmu nella dir; nessun arg -> primo *_bundle.fmu nella cwd.
resolve_fmu() {
    local arg="$1"
    if [ -z "$arg" ]; then
        ls -1 ./*_bundle.fmu 2>/dev/null | head -1
    elif [ -f "$arg" ]; then
        printf '%s\n' "$arg"
    elif [ -d "$arg" ]; then
        local cand="$arg/legoclix_$(basename "$arg")_bundle.fmu"
        if [ -f "$cand" ]; then
            printf '%s\n' "$cand"
        else
            ls -1 "$arg"/*_bundle.fmu 2>/dev/null | head -1
        fi
    fi
}

FMU="$(resolve_fmu "$FMU_ARG")"
if [ -z "${FMU:-}" ] || [ ! -f "$FMU" ]; then
    echo "ERRORE: bundle .fmu non trovato (arg='${FMU_ARG:-<vuoto>}')." >&2
    echo "        Indica un file .fmu, una task dir, o lancia dalla dir che lo contiene." >&2
    exit 2
fi
FMU_ABS="$(cd "$(dirname "$FMU")" && pwd)/$(basename "$FMU")"

echo ">>> Bundle    : $FMU_ABS  ($(du -h "$FMU_ABS" | cut -f1))"
echo ">>> Immagine  : $IMAGE"
echo ">>> Stop time : ${STOP_TIME}s"
echo ">>> Container : pulito, nessun LegoPST installato"
echo

# ---- codice Python eseguito dentro il container -------------------------
# Passato via env var per evitare l'inferno del quoting annidato (bash->python).
read -r -d '' PYCODE <<'PY'
import os
from fmpy import simulate_fmu, extract

extract("/work/bundle.fmu", unzipdir="/work/b")
st = float(os.environ.get("STOP_TIME", "15"))
r = simulate_fmu("/work/b", stop_time=st)

names = r.dtype.names
print(f"[container] OK: {len(r)} sample (stop_time={st}s)")
print("[container] colonne:", ", ".join(names))
print("[container] t0:", {k: float(r[0][k])  for k in names[:6]})
print("[container] tN:", {k: float(r[-1][k]) for k in names[:6]})
PY

# ---- bootstrap eseguito dentro il container -----------------------------
# 1) stampa distro/glibc e verifica che LegoPST NON ci sia (prova onesta).
# 2) garantisce pip (debian/ubuntu via apt, fedora via dnf/microdnf, alpine apk).
# 3) installa fmpy (con fallback --break-system-packages per i Python
#    "externally-managed", PEP 668: Debian 12+/Ubuntu 24.04).
# 4) esegue il codice Python di simulazione.
BOOTSTRAP='
set -e
echo "[container] distro: $(. /etc/os-release 2>/dev/null; echo "${PRETTY_NAME:-?}")"
echo "[container] glibc : $(ldd --version 2>/dev/null | head -1)"
echo "[container] LegoPST installato? $(command -v net_sked 2>/dev/null || echo NO)"

if ! command -v pip3 >/dev/null 2>&1 && ! command -v pip >/dev/null 2>&1; then
    echo "[container] pip assente -> installo..."
    if   command -v apt-get  >/dev/null 2>&1; then apt-get update -qq && apt-get install -y -qq python3-pip
    elif command -v dnf      >/dev/null 2>&1; then dnf install -y -q python3-pip
    elif command -v microdnf >/dev/null 2>&1; then microdnf install -y python3-pip
    elif command -v apk      >/dev/null 2>&1; then apk add --no-cache py3-pip
    else echo "[container] ERRORE: nessun gestore pacchetti noto per installare pip" >&2; exit 4
    fi
fi
PIP="$(command -v pip3 || command -v pip)"

echo "[container] installo fmpy..."
"$PIP" install --quiet --no-input fmpy 2>/dev/null \
    || "$PIP" install --quiet --no-input --break-system-packages fmpy

python3 -c "$PYCODE"
'

# ---- esecuzione ---------------------------------------------------------
set +e
if [ "$QUIET" -eq 1 ]; then
    docker run $RM_FLAG \
        -v "$FMU_ABS:/work/bundle.fmu:ro" \
        -e STOP_TIME="$STOP_TIME" \
        -e PYCODE="$PYCODE" \
        "$IMAGE" bash -c "$BOOTSTRAP" 2>&1 | grep -vE 'to_dispatcher:.*GUAG'
    RC=${PIPESTATUS[0]}
else
    docker run $RM_FLAG \
        -v "$FMU_ABS:/work/bundle.fmu:ro" \
        -e STOP_TIME="$STOP_TIME" \
        -e PYCODE="$PYCODE" \
        "$IMAGE" bash -c "$BOOTSTRAP"
    RC=$?
fi
set -e

echo
if [ "$RC" -eq 0 ]; then
    echo ">>> SELF-CONTAINMENT OK (exit 0): il bundle gira in container pulito."
else
    echo ">>> FALLITO (exit $RC): vedi l'output sopra." >&2
    [ "$RC" -ne 0 ] && exit 4
fi
