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
#                (o legoclix_<task>_bundle.fmu con --bundle)
#   (se omesso) cerca *.fmu nella cwd
#
# Opzioni:
#   -b, --bundle           usa la variante bundle (legoclix_<task>_bundle.fmu)
#   -i, --info             metadati + lista variabili (no simulate)
#   -V, --validate         lancia fmpy.validation e poi esce
#   -t, --stop-time T      tempo finale [s] (default: 30)
#   -d, --step-size DT     passo comunicazione [s] (default: dal modelDescription)
#   -r, --realtime K       pacing a tempo reale (coeff. velocita'): K=1 reale,
#                          K>1 accelerato (2=2x), K<1 rallentato (0.5=2x lento),
#                          0/assente = batch (default). Per perturbare live.
#   -s, --set VAR=VAL      valore iniziale (ripetibile)
#   -o, --csv FILE         output CSV (default: results.csv accanto alla FMU)
#   --hmi                  apre la HMI interattiva draw2gr (Command Mode) a
#                          inizio simulazione: la FMU lancia run_draw2gr.sh con
#                          la SHR_USR_KEY corretta. Richiede -b (bundle) e DISPLAY.
#   -h, --help
#
# Variabili d'ambiente:
#   LG_FMU_DEBUG=1              abilita log diagnostico su stderr (dt_sked, n_goup, ...)
#   LG_FMU_STEP_TIMEOUT_S=N     timeout in secondi per ciascun SD_goup (default 10).
#                               Aumentare per task grandi/lente (es. =120 per TCon-r1).
#
# Exit code:
#   0 ok, 1 args, 2 fmu non trovata, 3 fmpy non disponibile, 4 simulate fallito
#
# Note: la variante bundle (-b) e' self-contained (include runtime LegoPST +
# HMI). La variante base richiede LegoPST installato. La sim viene avviata
# dalla FMU stessa (attach_or_launch).
#
# Esempi:
#   # ispeziona metadati e variabili della FMU bundle di una task
#   run_fmu.sh -b -i /home/antonio/legocad/collet
#
#   # simulazione batch (default, piu' veloce possibile), CSV accanto alla FMU
#   run_fmu.sh -b -t 30 /home/antonio/legocad/collet
#
#   # perturbazione LIVE: HMI draw2gr (Command Mode) + tempo reale, 180 s.
#   # In draw2gr: Mode->Command (rosso), click su una IN libera, gradino in xaing.
#   run_fmu.sh -b --hmi -r 1 -t 180 /home/antonio/legocad/collet
#
#   # accelerata 2x con valore iniziale forzato su un ingresso
#   run_fmu.sh -b -r 2 -s ALZAVING=0.3 -t 120 /home/antonio/legocad/collet
#
#   # task grande/lenta: alza il timeout per ogni passo dello scheduler
#   LG_FMU_STEP_TIMEOUT_S=120 run_fmu.sh -b -t 60 /home/antonio/legocad/TCon-r1

set -u

usage() { sed -n '2,60p' "$0" | sed 's/^# \{0,1\}//'; exit 0; }

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

# ---- Slot SHM: attach vs launch -----------------------------------------
# Il profile setta SHR_USR_KEY=uid*10000 (slot 0, "banco operatore"). Se ora
# c'e' davvero un banco vivo lo SHM con offset ID_SHM_VAR=5 esiste in ipcs:
# manteniamo l'env per attach. Altrimenti unset, cosi' lg_fmi2.c sceglie da
# se' uno slot libero in 1..8 (logica P5) e non collide con eventuali residui
# di altri modelli a slot 0 (es. SHM dimensionate per N variabili diverse,
# che fanno fallire crea_shrmem con "esiste ed ha dim X superiori a Y").
USER_UID=$(id -u)
SLOT0_VAR_HEX=$(printf "0x%08x" $(( USER_UID * 10000 + 5 )))
if ipcs -m | awk 'NR>3 {print $1}' | grep -qiF "$SLOT0_VAR_HEX"; then
    echo "[run_fmu] sim attiva su slot 0 ($((USER_UID*10000))) -> attach mode" >&2
else
    unset SHR_USR_KEY SHR_USR_KEYS
fi

# ---- Args ---------------------------------------------------------------
MODE="simulate"
STOP_TIME="30"
STEP_SIZE=""
REALTIME="0"
OPEN_HMI=0
CSV_FILE=""
SET_KV=()
TARGET=""
BUNDLE=0

while [ $# -gt 0 ]; do
    case "$1" in
        -h|--help)        usage ;;
        -b|--bundle)      BUNDLE=1; shift ;;
        -i|--info)        MODE="info"; shift ;;
        -V|--validate)    MODE="validate"; shift ;;
        -t|--stop-time)   STOP_TIME="$2"; shift 2 ;;
        -d|--step-size)   STEP_SIZE="$2"; shift 2 ;;
        -r|--realtime)    REALTIME="$2"; shift 2 ;;
        --hmi)            OPEN_HMI=1; shift ;;
        -s|--set)         SET_KV+=("$2"); shift 2 ;;
        -o|--csv)         CSV_FILE="$2"; shift 2 ;;
        -*)               echo "ERR: opzione sconosciuta: $1" >&2; exit 1 ;;
        *)                TARGET="$1"; shift ;;
    esac
done

# ---- Risolvi FMU --------------------------------------------------------
SUFFIX=""
[ "$BUNDLE" = 1 ] && SUFFIX="_bundle"

if [ -z "$TARGET" ]; then
    FMU="$(ls -1 *${SUFFIX}.fmu 2>/dev/null | head -1 || true)"
    [ -n "$FMU" ] && FMU="$(pwd)/$FMU"
elif [ -d "$TARGET" ]; then
    TASK_NAME="$(basename "$(cd "$TARGET" && pwd)")"
    FMU="$(cd "$TARGET" && pwd)/legoclix_${TASK_NAME}${SUFFIX}.fmu"
elif [ -f "$TARGET" ]; then
    FMU="$(cd "$(dirname "$TARGET")" && pwd)/$(basename "$TARGET")"
else
    echo "ERR: target '$TARGET' non e' un file ne' una directory" >&2; exit 1
fi

[ -n "$FMU" ] && [ -f "$FMU" ] || {
    if [ "$BUNDLE" = 1 ]; then
        echo "ERR: FMU bundle non trovata (cercavo *_bundle.fmu nella cwd o legoclix_<task>_bundle.fmu nella task)." >&2
        echo "     Genera la FMU bundle con: dolgfmu.sh -b <task>" >&2
    else
        echo "ERR: FMU non trovata (target='$TARGET', cercavo *.fmu nella cwd o legoclix_<task>.fmu nella task)." >&2
        echo "     Genera la FMU con: dolgfmu.sh <task>" >&2
    fi
    exit 2
}

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
echo "[run_fmu] bundle   : $BUNDLE"

# ---- Esegui Python inline ----------------------------------------------
# Passiamo gli argomenti via env per evitare quoting hell con --set VAR=VAL.
export RF_FMU="$FMU"
export RF_MODE="$MODE"
export RF_STOP="$STOP_TIME"
export RF_STEP="$STEP_SIZE"
export RF_REALTIME="$REALTIME"
export RF_CSV="$CSV_FILE"
export RF_SET="${SET_KV[*]:-}"

# open_hmi: la FMU (lg_fmi2.c::maybe_open_hmi) legge LG_FMU_OPEN_HMI dall'env e,
# in bundle mode, lancia run_draw2gr.sh con la SHR_USR_KEY corretta.
export LG_FMU_OPEN_HMI="$OPEN_HMI"
if [ "$OPEN_HMI" = 1 ]; then
    [ "$BUNDLE" = 1 ] || echo "[run_fmu] AVVISO: --hmi richiede -b (bundle); sara' ignorato" >&2
    [ -n "${DISPLAY:-}" ] || echo "[run_fmu] AVVISO: --hmi senza DISPLAY: la HMI non si aprira'" >&2
fi

"$PYBIN" - <<'PYEOF'
import os, sys, csv

fmu_path  = os.environ["RF_FMU"]
mode      = os.environ["RF_MODE"]
stop_time = float(os.environ["RF_STOP"])
step_size = os.environ["RF_STEP"]
step_size = float(step_size) if step_size else None
realtime  = float(os.environ.get("RF_REALTIME", "0") or "0")
csv_file  = os.environ["RF_CSV"]
set_raw   = os.environ["RF_SET"].strip()

# Pacing a tempo reale: callback chiamato da fmpy dopo ogni step di
# comunicazione. Ancora lo scheduling al primo step e dorme finche' il wall
# clock non raggiunge (t_sim - t0)/coeff. coeff>1 accelera, <1 rallenta.
import time as _wt
_pace = {}
def _step_finished(t_sim, recorder):
    if realtime > 0:
        now = _wt.perf_counter()
        if "w0" not in _pace:
            _pace["w0"] = now
            _pace["s0"] = t_sim
        else:
            target = _pace["w0"] + (t_sim - _pace["s0"]) / realtime
            d = target - now
            if d > 0:
                _wt.sleep(d)
    return True

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

from fmpy import read_model_description, simulate_fmu, extract

# Estraiamo la FMU in una dir accanto al .fmu (regola progetto: niente
# /tmp/fmpy_*). La dir di unzip e' <fmu_dir>/<basename_no_ext>/, persistente
# tra run (utile per debug + conforme a feedback_fmu_output_path.md).
unzip_dir = os.path.splitext(fmu_path)[0]
unzip_dir = os.path.abspath(unzip_dir)
extract(fmu_path, unzipdir=unzip_dir)
print(f"[run_fmu] unzipped : {unzip_dir}")

# fmpy estrae via zipfile Python, che NON preserva il bit +x: senza questo il
# wish/net_sked/xaing/graphics del bundle restano non-eseguibili (es. la HMI
# --hmi in attach mode non si apriva). Il bundle porta restore_perms.sh: lo
# eseguiamo qui subito dopo l'estrazione. No-op per FMU non-bundle.
import subprocess as _sp
_rp = os.path.join(unzip_dir, "resources", "bundle", "restore_perms.sh")
if os.path.isfile(_rp):
    _sp.run(["bash", _rp], check=False)
    print("[run_fmu] restore_perms: bundle executables +x")

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
# Usiamo la dir gia' estratta invece di passare il .fmu (evita extract
# duplicato in /tmp/fmpy_*; serve anche su Python 3.13 dove fmpy.fmi2
# rifiuta path relativi via pathlib.as_uri()).
if realtime > 0:
    print(f"[simulate] pacing tempo reale: coeff={realtime} "
          f"({'reale' if realtime==1 else (str(realtime)+'x')})")
result = simulate_fmu(
    unzip_dir,
    stop_time=stop_time,
    step_size=step_size,
    start_values=start_values,
    output=[v.name for v in outputs] or None,
    step_finished=(_step_finished if realtime > 0 else None),
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
