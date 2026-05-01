#!/usr/bin/env bash
#
# run_fmu.sh — wrapper bash semplificato per simulare/ispezionare una FMU
# LegoPST Linux. Su Linux il pendant di run_fmu.py Windows e' fmpy: questo
# script fa source di .profile_legoroot, sceglie l'interprete (preferendo
# il venv fmpy dedicato), risolve la FMU e invoca uno script Python inline.
#
# Uso:
#   run_fmu.sh [opzioni] [<fmu_path|task_dir>]
#
# Argomento posizionale:
#   <fmu_path>   path al .fmu da simulare/ispezionare
#   <task_dir>   dir di una task LegoPST: usa <task_dir>/legoclix_<task>.fmu
#   (se omesso) cerca *.fmu nella cwd
#
# Opzioni:
#   -i, --info             metadati + lista variabili (no simulate)
#   -V, --validate         lancia fmpy.validation e poi esce
#   -t, --stop-time T      tempo finale [s] (default: 30)
#   -d, --step-size DT     passo comunicazione [s] (default: dal modelDescription)
#   -s, --set VAR=VAL      valore iniziale (ripetibile)
#   -o, --csv FILE         output CSV (default: results.csv accanto alla FMU)
#   -h, --help
#
# Exit code:
#   0 ok, 1 args, 2 fmu non trovata, 3 fmpy non disponibile, 4 simulate fallito
#
# Note: la FMU Linux NON e' self-contained (P7 lo risolvera'). Questo wrapper
# richiede che sulla macchina sia installato LegoPST e che la sim possa essere
# avviata (la FMU stessa lo fa via attach_or_launch).

set -u

usage() { sed -n '2,28p' "$0" | sed 's/^# \{0,1\}//'; exit 0; }

# ---- LEGOROOT + profile -------------------------------------------------
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
LEGOROOT_GUESS="$(cd "$SCRIPT_DIR/../../.." && pwd)"
: "${LEGOROOT:=$LEGOROOT_GUESS}"
export LEGOROOT

if [ -z "${N001:-}" ]; then
    set +u
    . "$LEGOROOT/.profile_legoroot" "$LEGOROOT" >/dev/null 2>&1 || true
    set -u
fi

# ---- Args ---------------------------------------------------------------
MODE="simulate"
STOP_TIME="30"
STEP_SIZE=""
CSV_FILE=""
SET_KV=()
TARGET=""

while [ $# -gt 0 ]; do
    case "$1" in
        -h|--help)        usage ;;
        -i|--info)        MODE="info"; shift ;;
        -V|--validate)    MODE="validate"; shift ;;
        -t|--stop-time)   STOP_TIME="$2"; shift 2 ;;
        -d|--step-size)   STEP_SIZE="$2"; shift 2 ;;
        -s|--set)         SET_KV+=("$2"); shift 2 ;;
        -o|--csv)         CSV_FILE="$2"; shift 2 ;;
        -*)               echo "ERR: opzione sconosciuta: $1" >&2; exit 1 ;;
        *)                TARGET="$1"; shift ;;
    esac
done

# ---- Risolvi FMU --------------------------------------------------------
if [ -z "$TARGET" ]; then
    FMU="$(ls -1 *.fmu 2>/dev/null | head -1 || true)"
    [ -n "$FMU" ] && FMU="$(pwd)/$FMU"
elif [ -d "$TARGET" ]; then
    TASK_NAME="$(basename "$(cd "$TARGET" && pwd)")"
    FMU="$(cd "$TARGET" && pwd)/legoclix_${TASK_NAME}.fmu"
elif [ -f "$TARGET" ]; then
    FMU="$(cd "$(dirname "$TARGET")" && pwd)/$(basename "$TARGET")"
else
    echo "ERR: target '$TARGET' non e' un file ne' una directory" >&2; exit 1
fi

[ -n "$FMU" ] && [ -f "$FMU" ] || {
    echo "ERR: FMU non trovata (target=$TARGET, FMU=$FMU)" >&2; exit 2; }

[ -z "$CSV_FILE" ] && CSV_FILE="$(dirname "$FMU")/results.csv"

# ---- Interprete Python --------------------------------------------------
# Preferisci venv dedicato (l'utente ha /home/antonio/fmpy_venv da Punto 5).
# Override possibile via env FMPY_VENV.
PYBIN=""
for cand in "${FMPY_VENV:-/home/antonio/fmpy_venv}/bin/python3" python3; do
    if [ -x "$cand" ] || command -v "$cand" >/dev/null 2>&1; then
        if "$cand" -c "import fmpy" >/dev/null 2>&1; then
            PYBIN="$cand"; break
        fi
    fi
done
[ -n "$PYBIN" ] || {
    echo "ERR: fmpy non disponibile. Installa con: pip install fmpy" >&2
    echo "     oppure imposta FMPY_VENV=/path/al/venv" >&2
    exit 3; }

echo "[run_fmu] FMU      : $FMU"
echo "[run_fmu] python   : $PYBIN"
echo "[run_fmu] mode     : $MODE"

# ---- Esegui Python inline ----------------------------------------------
# Passiamo gli argomenti via env per evitare quoting hell con --set VAR=VAL.
export RF_FMU="$FMU"
export RF_MODE="$MODE"
export RF_STOP="$STOP_TIME"
export RF_STEP="$STEP_SIZE"
export RF_CSV="$CSV_FILE"
export RF_SET="${SET_KV[*]:-}"

"$PYBIN" - <<'PYEOF'
import os, sys, csv

fmu_path  = os.environ["RF_FMU"]
mode      = os.environ["RF_MODE"]
stop_time = float(os.environ["RF_STOP"])
step_size = os.environ["RF_STEP"]
step_size = float(step_size) if step_size else None
csv_file  = os.environ["RF_CSV"]
set_raw   = os.environ["RF_SET"].strip()

start_values = {}
if set_raw:
    for kv in set_raw.split():
        if "=" not in kv:
            print(f"ERR: --set richiede VAR=VAL, ricevuto {kv!r}", file=sys.stderr)
            sys.exit(1)
        k, v = kv.split("=", 1)
        try:
            start_values[k] = float(v)
        except ValueError:
            start_values[k] = v

from fmpy import read_model_description, simulate_fmu

if mode == "validate":
    from fmpy.validation import validate_fmu
    issues = validate_fmu(fmu_path)
    if issues:
        print(f"  [validate] {len(issues)} issue:")
        for it in issues:
            print(f"   - {it}")
        sys.exit(1)
    print("  [validate] OK (0 issue)")
    sys.exit(0)

md = read_model_description(fmu_path)
print(f"  modelName      : {md.modelName}")
print(f"  guid           : {md.guid}")
print(f"  fmiVersion     : {md.fmiVersion}")
print(f"  generationTool : {md.generationTool}")
if md.defaultExperiment:
    print(f"  defaultStep    : {md.defaultExperiment.stepSize}")

inputs  = [v for v in md.modelVariables if v.causality == "input"]
outputs = [v for v in md.modelVariables if v.causality == "output"]
print(f"\n  Inputs  ({len(inputs)}):")
for v in inputs:
    s = f"  start={v.start}" if v.start is not None else ""
    print(f"    VR={v.valueReference:<6} {v.name}{s}")
print(f"\n  Outputs ({len(outputs)}):")
for v in outputs:
    print(f"    VR={v.valueReference:<6} {v.name}")

if mode == "info":
    sys.exit(0)

# --- simulate ---
print(f"\n[simulate] stop_time={stop_time}s step={step_size}s start_values={start_values}")
result = simulate_fmu(
    fmu_path,
    stop_time=stop_time,
    step_size=step_size,
    start_values=start_values,
    output=[v.name for v in outputs] or None,
)

# Dump CSV
with open(csv_file, "w", newline="") as f:
    w = csv.writer(f)
    w.writerow(result.dtype.names)
    for row in result:
        w.writerow(row)

print(f"\n[simulate] {len(result)} sample(s) -> {csv_file}")

# Summary t finale
print(f"\nUltimo sample (t={result['time'][-1]}):")
for name in result.dtype.names:
    if name == "time": continue
    print(f"  {name:14} = {result[name][-1]}")
PYEOF

RC=$?
[ $RC -eq 0 ] || { echo "[run_fmu] FAIL (rc=$RC)" >&2; exit 4; }
exit 0
