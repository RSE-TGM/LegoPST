#!/usr/bin/env bash
#
# dolgfmu.sh — costruisce la FMU LegoCliSINC per una task LegoPST.
#
# Equivalente Linux di dolgfmu.bat/dolgfmu.py della versione Windows.
# Differenze rispetto a bundle/build.sh "raw":
#   - sourcea automaticamente .profile_legoroot se LEGOROOT non e' in env.
#   - se la sim non e' viva, la avvia via net_startup_headless.sh +
#     probe_init (SD_inizializza), e poi a fine la chiude con killsim.
#   - se la sim era gia' viva (utente la sta usando dalla UI), si limita
#     ad attaccarsi e produrre la .fmu, lasciandola attiva.
#   - output di default: <TASK>/legoclix_<basename(TASK)>.fmu
#
# Uso:
#   dolgfmu.sh [TASK_DIR]
#       TASK_DIR: dir della task (default: $PWD).
#
# Exit code:
#   0  ok
#   1  task non valida (manca variabili.rtf)
#   2  errore di build (bundle/build.sh ha fallito)
#   3  fallito avvio sim headless
#

set -u

# ---- 0. LEGOROOT --------------------------------------------------------
SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
# scripts/ -> lg_fmu -> Alg_rt -> $LEGOROOT
LEGOROOT_GUESS=$(cd "$SCRIPT_DIR/../../.." && pwd)
: "${LEGOROOT:=$LEGOROOT_GUESS}"
export LEGOROOT

# Se l'env LegoPST non e' settato (PATH, N001..N008, ALTERLEGO_*), sourcea
# il profile. Lo capiamo da N001: se manca, non e' stato sourceato.
# NB: il profile referenzia variabili opzionali (vedi commento in
# net_startup_headless.sh:30-32); va sourceato senza `set -u`, altrimenti
# la prima dereferenza di una var unset fa exit dello script.
if [ -z "${N001:-}" ]; then
    set +u
    # shellcheck source=/dev/null
    . "$LEGOROOT/.profile_legoroot" "$LEGOROOT" >/dev/null 2>&1 || true
    set -u
fi

# ---- 1. TASK_DIR --------------------------------------------------------
TASK_DIR="${1:-$PWD}"
if [ ! -d "$TASK_DIR" ]; then
    echo "ERRORE: TASK_DIR '$TASK_DIR' non esiste." >&2
    exit 1
fi
TASK_DIR=$(cd "$TASK_DIR" && pwd)
TASK_NAME=$(basename "$TASK_DIR")

if [ ! -f "$TASK_DIR/variabili.rtf" ]; then
    echo "ERRORE: '$TASK_DIR' non e' una task LegoPST (manca variabili.rtf)." >&2
    exit 1
fi

OUTPUT="$TASK_DIR/legoclix_${TASK_NAME}.fmu"

echo "============================================================"
echo "  dolgfmu — build FMU per task LegoPST"
echo "============================================================"
echo "  LEGOROOT  : $LEGOROOT"
echo "  TASK_DIR  : $TASK_DIR"
echo "  OUTPUT    : $OUTPUT"
echo

# ---- 2. SHR_USR_KEY default (uid*10000, vedi Alg_env.sh:235) -----------
if [ -z "${SHR_USR_KEY:-}" ]; then
    SHR_USR_KEY=$(( $(id -u) * 10000 ))
    export SHR_USR_KEY
fi
# SHR_USR_KEYS richiesta da killsim (vedi P4.5 della memory).
if [ -z "${SHR_USR_KEYS:-}" ]; then
    SHR_USR_KEYS=$(( SHR_USR_KEY + 1000 ))
    export SHR_USR_KEYS
fi

# ---- 3. Detect sim viva -------------------------------------------------
# Cerca SHM con key prefisso SHR_USR_KEY nel range [SHR_USR_KEY, SHR_USR_KEY+99].
# `ipcs -m` stampa key in esadecimale; convertiamo SHR_USR_KEY in hex per il
# pattern. Esempio: SHR_USR_KEY=10000000 → 0x00989680, range fino 0x009896E3.
sim_alive() {
    local key_hex
    key_hex=$(printf '0x%08x' "$SHR_USR_KEY")
    # I primi 6 hex chars sono comuni a tutto il range +0..+255 → match prefisso.
    local prefix="${key_hex:0:8}"
    ipcs -m 2>/dev/null | awk -v pfx="$prefix" '$1 ~ pfx { found=1 } END { exit !found }'
}

SIM_STARTED_BY_US=0
if sim_alive; then
    echo "[detect] simulazione GIA' viva su SHR_USR_KEY=$SHR_USR_KEY → attach mode"
else
    echo "[detect] simulazione spenta → la avvio in modo headless"
    if ! "$LEGOROOT/Alg_rt/lg_fmu/scripts/net_startup_headless.sh" "$TASK_DIR"; then
        echo "ERRORE: net_startup_headless.sh ha fallito." >&2
        exit 3
    fi
    sleep 2
    if ! "$LEGOROOT/Alg_rt/lg_fmu/tools/probe_init" >/dev/null 2>&1; then
        echo "ERRORE: probe_init (SD_inizializza) ha fallito." >&2
        exit 3
    fi
    # Attesa che lg5sk spawni e popoli SHM_VAR. 3s e' empiricamente sufficiente
    # per la task collet; per task piu' grandi il polling lo fa la FMU stessa.
    sleep 3
    SIM_STARTED_BY_US=1
    echo "[detect] sim avviata."
fi

# ---- 4. Build .fmu -------------------------------------------------------
echo
echo "[build] $LEGOROOT/Alg_rt/lg_fmu/bundle/build.sh -o $OUTPUT"
( cd "$TASK_DIR" && "$LEGOROOT/Alg_rt/lg_fmu/bundle/build.sh" -o "$OUTPUT" )
BUILD_RC=$?

# ---- 5. Cleanup sim se l'avevamo avviata noi -----------------------------
if [ "$SIM_STARTED_BY_US" -eq 1 ]; then
    echo
    echo "[cleanup] killsim (la sim era stata avviata da dolgfmu)"
    killsim >/dev/null 2>&1 || true
fi

if [ $BUILD_RC -ne 0 ] && [ ! -f "$OUTPUT" ]; then
    echo "ERRORE: build fallito (rc=$BUILD_RC)." >&2
    exit 2
fi

echo
echo "============================================================"
echo "  ✓ FMU pronta: $OUTPUT"
ls -lh "$OUTPUT" 2>/dev/null
echo "============================================================"
exit 0
