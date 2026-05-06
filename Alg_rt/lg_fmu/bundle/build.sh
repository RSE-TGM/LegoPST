#!/usr/bin/env bash
#
# build.sh — produce un .fmu (FMI 2.0 Co-Simulation, target Linux x86_64)
# per la task LegoPST attualmente attiva (deve esistere una simulazione
# in stato RUN o FREEZE: net_startup gia' eseguito, SHR_USR_KEY in env).
#
# Layout del bundle prodotto:
#   <output>.fmu  (zip)
#   ├── modelDescription.xml
#   ├── binaries/linux64/
#   │   └── <model_name>.so
#   └── resources/
#       └── README.txt        (placeholder)
#
# Pre-requisiti:
#   * source $LEGOROOT/.profile_legoroot
#   * cwd dentro la task target (variabili.rtf + S04/S05 raggiungibili)
#   * sim attiva: net_startup gia' lanciato, SHR_USR_KEY definita
#   * /usr/bin/zip
#
# Uso:
#   cd /home/antonio/legocad/MDC_GV
#   $LEGOROOT/Alg_rt/lg_fmu/bundle/build.sh [opzioni]
#
# Opzioni:
#   -o, --output PATH      output .fmu (default: ./<model_name>.fmu)
#   -n, --name NAME        modelName/modelIdentifier (default: LegoCliSINC)
#   -d, --description STR  descrizione human-readable
#   -v, --version V        version stringa (default: 1.0.0)
#   -k, --keep-staging     non rimuovere la dir di staging (debug)
#   -b, --bundle           include runtime LegoPST self-contained (P7)
#   -h, --help

set -eu

usage() {
    sed -n '2,28p' "$0" | sed 's/^# \{0,1\}//'
    exit 0
}

# ---- Defaults / args -----------------------------------------------------
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
LG_FMU_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

MODEL_NAME="LegoCliSINC"
DESCRIPTION="LEGO thermo-hydraulic Co-Simulation FMU (Linux)"
VERSION="1.0.0"
OUTPUT=""
KEEP_STAGING=0
BUNDLE=0

while [[ $# -gt 0 ]]; do
    case "$1" in
        -o|--output)        OUTPUT="$2"; shift 2 ;;
        -n|--name)          MODEL_NAME="$2"; shift 2 ;;
        -d|--description)   DESCRIPTION="$2"; shift 2 ;;
        -v|--version)       VERSION="$2"; shift 2 ;;
        -k|--keep-staging)  KEEP_STAGING=1; shift ;;
        -b|--bundle)        BUNDLE=1; shift ;;
        -h|--help)          usage ;;
        *)                  echo "ERRORE: opzione sconosciuta: $1" >&2; exit 1 ;;
    esac
done

[[ -z "$OUTPUT" ]] && OUTPUT="$(pwd)/${MODEL_NAME}.fmu"

# ---- Pre-check -----------------------------------------------------------
[[ -z "${SHR_USR_KEY:-}" ]] && {
    echo "ERRORE: SHR_USR_KEY non definita. Lancia net_startup prima." >&2
    exit 2
}

[[ -f variabili.rtf ]] || {
    echo "ERRORE: cwd '$(pwd)' non sembra una task LegoPST" >&2
    echo "        (manca variabili.rtf)." >&2
    exit 2
}

GEN_MD="$LG_FMU_DIR/tools/gen_modeldescription"
TEMPLATE="$LG_FMU_DIR/bundle/modelDescription.xml.in"
SO_FILE="$LG_FMU_DIR/src/LegoCliSINC.so"

[[ -x "$GEN_MD"   ]] || { echo "ERRORE: $GEN_MD non trovato. Build i tools prima." >&2; exit 3; }
[[ -f "$TEMPLATE" ]] || { echo "ERRORE: $TEMPLATE non trovato." >&2; exit 3; }
[[ -f "$SO_FILE"  ]] || { echo "ERRORE: $SO_FILE non trovato. Build src prima." >&2; exit 3; }
command -v zip >/dev/null 2>&1 || { echo "ERRORE: 'zip' non installato." >&2; exit 3; }

# ---- Staging dir ---------------------------------------------------------
STAGING="$(mktemp -d -t lg_fmu_bundle.XXXXXX)"
cleanup() { [[ $KEEP_STAGING -eq 0 ]] && rm -rf "$STAGING"; }
trap cleanup EXIT

mkdir -p "$STAGING/binaries/linux64"
mkdir -p "$STAGING/resources"

# ---- 1. modelDescription.xml --------------------------------------------
echo "[1/4] gen_modeldescription"
"$GEN_MD" \
    --template    "$TEMPLATE" \
    --out         "$STAGING/modelDescription.xml" \
    --model-name  "$MODEL_NAME" \
    --description "$DESCRIPTION" \
    --version     "$VERSION"

# ---- 2. binary -----------------------------------------------------------
echo "[2/4] copia binario"
# Il modelIdentifier dichiarato dentro modelDescription.xml deve combaciare
# col nome del .so. gen_modeldescription mette modelIdentifier=$MODEL_NAME.
cp "$SO_FILE" "$STAGING/binaries/linux64/${MODEL_NAME}.so"
chmod 0755    "$STAGING/binaries/linux64/${MODEL_NAME}.so"

# ---- 3. resources --------------------------------------------------------
echo "[3/4] resources placeholder + task_info.env"

# Per la modalita' autonoma (Punto 4): la FMU legge da resources/task_info.env
# il path della task e LEGOROOT, cosi' fmi2Instantiate puo' lanciare lui
# net_startup_headless.sh se la sim non e' gia' attiva.
TASK_PATH_ABS="$(pwd)"
LEGOROOT_ABS="$(cd "$LG_FMU_DIR/../.." && pwd)"

TASK_NAME="$(basename "$TASK_PATH_ABS")"

# In bundle mode i path assoluti del builder non sono validi sul target:
# TASK_PATH diventa relativo (task/<name>, sotto resources/bundle/) e LEGOROOT
# viene derivato a runtime dalla resource URI (vedi lg_fmi2.c::attach_or_launch).
if [[ $BUNDLE -eq 1 ]]; then
    TASK_PATH_FIELD="task/$TASK_NAME"
    LEGOROOT_FIELD=""
else
    TASK_PATH_FIELD="$TASK_PATH_ABS"
    LEGOROOT_FIELD="$LEGOROOT_ABS"
fi

cat > "$STAGING/resources/task_info.env" <<EOF
# Generato da lg_fmu/bundle/build.sh il $(date -u +%Y-%m-%dT%H:%M:%SZ)
# Contesto del task LegoPST a cui la FMU si attacca / lancia.
TASK_PATH=$TASK_PATH_FIELD
LEGOROOT=$LEGOROOT_FIELD
BUNDLE_MODE=$BUNDLE
TASK_NAME=$TASK_NAME
EOF

# Parametri di dimensionamento LEGO (N000..N007, M001..M005): GetParLego()
# in libsim/par_lego.c li legge da env. Senza, le strutture interne usano i
# DEF_* che non combaciano con la SHM creata dalla sim attiva (mismatch ->
# fmi2DoStep status 3). Dump dall'env del builder (.profile_legoroot deve
# essere stato sourceato): se assenti scrive "" (lg_fmi2.c salta la setenv).
{
    echo
    echo "# Parametri dimensionamento LEGO (Alg_env.sh)"
    for v in N000 N001 N002 N003 N004 N005 N007 M001 M002 M003 M004 M005; do
        echo "${v}=${!v:-}"
    done
} >> "$STAGING/resources/task_info.env"

# ---- 3.5 BUNDLE (--bundle): include tutto il runtime LegoPST ------------
if [[ $BUNDLE -eq 1 ]]; then
    echo "[3.5/4] bundling self-contained (--bundle)"

    BD="$STAGING/resources/bundle"
    mkdir -p "$BD/Alg_rt/bin" \
             "$BD/Alg_rt/lg_fmu/scripts" \
             "$BD/Alg_rt/lg_fmu/tools" \
             "$BD/lego_big/bin" \
             "$BD/task" \
             "$BD/lib"

    # Binari principali (Alg_rt/bin). net_prepf22 e' OBBLIGATORIO: net_sked
    # MASTER lo spawna in sked_start.c:1039 e attende un ack con timeout
    # TIMEOUT_AUS*10 = 1350s. Senza net_prepf22 nel PATH, sked_start blocca
    # ~22 minuti prima di proseguire e la sim non arriva mai a STATO_FREEZE
    # entro il timeout della FMU.
    for b in dispatcher net_sked killsim net_prepf22; do
        cp -p "$LEGOROOT_ABS/Alg_rt/bin/$b" "$BD/Alg_rt/bin/" \
            || { echo "ERR: $b non trovato in $LEGOROOT_ABS/Alg_rt/bin" >&2; exit 4; }
    done

    # Loader tabelle acqua/vapore
    cp -p "$LEGOROOT_ABS/lego_big/bin/initav"     "$BD/lego_big/bin/"
    cp -p "$LEGOROOT_ABS/lego_big/bin/TAVOLE.DAT" "$BD/lego_big/bin/"

    # Symlink atteso da Alg_env.sh:118: LEGO=$LEGOROOT/legocad/lego_big.
    # Senza questo initav non trova TAVOLE.DAT, lg5sk fa SIGFPE in trova_().
    mkdir -p "$BD/legocad"
    ln -sf "../lego_big" "$BD/legocad/lego_big"

    # Scripts/tools FMU
    cp -p "$LG_FMU_DIR/scripts/net_startup_headless.sh" "$BD/Alg_rt/lg_fmu/scripts/"
    cp -p "$LG_FMU_DIR/tools/probe_init"                "$BD/Alg_rt/lg_fmu/tools/"

    # Graphics viewer (Motif GUI, richiede DISPLAY): binary + MRM resource + unita' di misura
    cp -p "$LEGOROOT_ABS/Alg_rt/bin/graphics"       "$BD/Alg_rt/bin/" \
        || { echo "ERR: graphics non trovato in $LEGOROOT_ABS/Alg_rt/bin" >&2; exit 4; }
    mkdir -p "$BD/Alg_rt/uid"
    cp -p "$LEGOROOT_ABS/Alg_rt/uid/graphics.uid"   "$BD/Alg_rt/uid/" \
        || { echo "ERR: graphics.uid non trovato in $LEGOROOT_ABS/Alg_rt/uid" >&2; exit 4; }
    # chdefaults() cerca uni_misc.dat in $HOME/defaults/: forniamo uno stub
    mkdir -p "$BD/home_stub/defaults"
    if [[ -f "$HOME/defaults/uni_misc.dat" ]]; then
        cp -p "$HOME/defaults/uni_misc.dat" "$BD/home_stub/defaults/"
    else
        echo "AVVISO: $HOME/defaults/uni_misc.dat non trovato, graphics partira' senza unita' di misura" >&2
    fi

    # Profile + Alg_env (eseguiti con LEGOROOT=<bundle> a runtime)
    cp -p "$LEGOROOT_ABS/.profile_legoroot" "$BD/.profile_legoroot"
    cp -p "$LEGOROOT_ABS/Alg_env.sh"        "$BD/Alg_env.sh"

    # Task corrente: copia escludendo log/trend rigenerabili e binari Windows.
    # legoclix_*_bundle/ e legoclix_*/ sono dir di unzip residue di run di
    # fmpy precedenti: vanno escluse sennò il bundle si include ricorsivamente.
    rsync -a \
        --exclude='*.fmu' \
        --exclude='legoclix_*_bundle' \
        --exclude='legoclix_*_bundle/' \
        --exclude='*.log' \
        --exclude='dispatcher.out' \
        --exclude='banco.log' \
        --exclude='lgerr.out' \
        --exclude='*.last' \
        --exclude='results.csv' \
        --exclude='lgser.exe' \
        --exclude='*.slx' \
        --exclude='*.slx.*' \
        --exclude='*.autosave' \
        --exclude='f22circ.dat' \
        --exclude='*.fmu.log' \
        --exclude='S02_ULTRIX' \
        --exclude='S02_OSF1' \
        --exclude='out/' \
        --exclude='backtrack.dat' \
        "$TASK_PATH_ABS/" "$BD/task/$TASK_NAME/"

    # Shared libs: ldd di tutti i binari, copia non-system .so in lib/
    SO_LIST="$(mktemp)"
    for b in "$BD/Alg_rt/bin/"* "$BD/lego_big/bin/initav" \
             "$BD/Alg_rt/lg_fmu/tools/probe_init" \
             "$BD/task/$TASK_NAME/proc/lg5sk" \
             "$LG_FMU_DIR/src/LegoCliSINC.so"; do
        [[ -f "$b" ]] && ldd "$b" 2>/dev/null
    done | awk '/=>/ {print $3}' \
         | grep -v '^$' \
         | grep -v '/libc\.so' \
         | grep -v '/libm\.so' \
         | grep -v '/libpthread\.so' \
         | grep -v '/libdl\.so' \
         | grep -v '/librt\.so' \
         | grep -v '/ld-linux' \
         | sort -u > "$SO_LIST"

    while IFS= read -r so; do
        [[ -f "$so" ]] && cp -p "$so" "$BD/lib/"
    done < "$SO_LIST"
    SO_COUNT=$(wc -l < "$SO_LIST")
    rm -f "$SO_LIST"

    # launch_sim.sh: wrapper che lg_fmi2.c chiama in bundle mode.
    # Setta LEGOROOT alla dir del bundle, LD_LIBRARY_PATH a lib/, sourcea il
    # profile (con set +u, vedi commento in net_startup_headless.sh) e poi
    # delega a net_startup_headless.sh col task_dir interno al bundle.
    cat > "$BD/launch_sim.sh" <<'LAUNCH'
#!/usr/bin/env bash
# launch_sim.sh - bundle entry point. cwd / arg = task dir interna al bundle.
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
export LEGOROOT="$SCRIPT_DIR"
export LD_LIBRARY_PATH="$LEGOROOT/lib:${LD_LIBRARY_PATH:-}"
export PATH="$LEGOROOT/Alg_rt/bin:$LEGOROOT/lego_big/bin:$PATH"
export BUNDLE_MODE=1
exec "$LEGOROOT/Alg_rt/lg_fmu/scripts/net_startup_headless.sh" "$@"
LAUNCH
    chmod 0755 "$BD/launch_sim.sh"

    # restore_perms.sh: fmpy estrae il bundle via Python zipfile, che NON
    # preserva il bit +x. lg_fmi2.c (bundle_mode) lo invoca via `bash` (che
    # non richiede exec bit sul file) prima di lanciare launch_sim.sh.
    cat > "$BD/restore_perms.sh" <<'PERMS'
#!/usr/bin/env bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
chmod +x "$SCRIPT_DIR/launch_sim.sh" \
         "$SCRIPT_DIR/run_graphics.sh" \
         "$SCRIPT_DIR/Alg_rt/bin/dispatcher" \
         "$SCRIPT_DIR/Alg_rt/bin/net_sked" \
         "$SCRIPT_DIR/Alg_rt/bin/killsim" \
         "$SCRIPT_DIR/Alg_rt/bin/net_prepf22" \
         "$SCRIPT_DIR/Alg_rt/bin/graphics" \
         "$SCRIPT_DIR/Alg_rt/lg_fmu/scripts/net_startup_headless.sh" \
         "$SCRIPT_DIR/Alg_rt/lg_fmu/tools/probe_init" \
         "$SCRIPT_DIR/lego_big/bin/initav" 2>/dev/null || true
# Eseguibili dentro task/<name>/proc/ (lg5sk e altri lg*).
shopt -s nullglob
for f in "$SCRIPT_DIR"/task/*/proc/*; do
    [[ -f "$f" ]] && chmod +x "$f"
done
shopt -u nullglob
exit 0
PERMS
    chmod 0755 "$BD/restore_perms.sh"

    # run_graphics.sh: wrapper per aprire graphics sul f22circ.dat prodotto
    # dalla sim. Il programma graphics (per ragioni storiche) vuole il path
    # SENZA estensione .dat: es. ".../task/collet/f22circ" non "...f22circ.dat".
    cat > "$BD/run_graphics.sh" <<'GRAFICS'
#!/usr/bin/env bash
#
# run_graphics.sh — visualizza f22circ prodotto dalla simulazione FMU bundle.
#
# Uso:
#   run_graphics.sh                    # cerca f22circ in task/*/
#   run_graphics.sh /path/f22circ      # path esplicito (senza .dat)
#   run_graphics.sh /path/f22circ.dat  # .dat accettato (rimosso automaticamente)
#   run_graphics.sh <task_dir>         # usa task_dir/f22circ
#
# Richiede: DISPLAY impostato (sessione X11).
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
export LEGORT_UID="$SCRIPT_DIR/Alg_rt/uid/"
export HOME="$SCRIPT_DIR/home_stub"
export LD_LIBRARY_PATH="$SCRIPT_DIR/lib:${LD_LIBRARY_PATH:-}"

GRAPHICS_BIN="$SCRIPT_DIR/Alg_rt/bin/graphics"
[[ -x "$GRAPHICS_BIN" ]] || { echo "ERR: $GRAPHICS_BIN non trovato o non eseguibile" >&2; exit 1; }
[[ -n "${DISPLAY:-}" ]] || { echo "ERR: DISPLAY non impostato (serve sessione X11)" >&2; exit 1; }

if [[ $# -eq 0 ]]; then
    # Cerca f22circ (file senza estensione) nella prima task bundled
    F22BASE=$(find "$SCRIPT_DIR/task" -maxdepth 2 -name "f22circ.dat" 2>/dev/null | head -1)
    [[ -z "$F22BASE" ]] && {
        echo "ERR: nessun f22circ.dat in $SCRIPT_DIR/task/." >&2
        echo "     Esegui prima la simulazione con fmpy o run_fmu." >&2
        exit 2
    }
    # Rimuovi .dat: graphics vuole il path senza estensione
    F22BASE="${F22BASE%.dat}"
elif [[ -d "$1" ]]; then
    F22BASE="$1/f22circ"
else
    # Accetta sia "f22circ" che "f22circ.dat"
    F22BASE="${1%.dat}"
fi

[[ -f "${F22BASE}.dat" ]] || { echo "ERR: ${F22BASE}.dat non trovato" >&2; exit 2; }
echo "[run_graphics] $F22BASE"
exec "$GRAPHICS_BIN" "$F22BASE"
GRAFICS
    chmod 0755 "$BD/run_graphics.sh"

    # Sommario bundle
    BUNDLE_SIZE=$(du -sh "$BD" | cut -f1)
    echo "  bundle: $BUNDLE_SIZE  (binari + task + $SO_COUNT shared libs)"
fi


cat > "$STAGING/resources/README.txt" <<EOF
Linux LegoCliSINC FMU - resources directory.
This bundle was produced by lg_fmu/bundle/build.sh.

Files:
  task_info.env   TASK_PATH + LEGOROOT, used by fmi2Instantiate to
                  auto-attach or auto-launch the simulation.

Auto-launch behavior:
  fmi2Instantiate tries first to attach to an already-running sim
  (RtCreateDbPunti). If that fails, it execs the headless launcher
  \$LEGOROOT/Alg_rt/lg_fmu/scripts/net_startup_headless.sh on
  TASK_PATH and polls until the sim is up.

  fmi2Terminate runs killsim only if the FMU was the one that
  started the sim (i.e. attach-only sessions leave the sim alone).

Manual mode (legacy):
  If you prefer to manage the sim yourself, simply launch
  net_startup on the task and set SHR_USR_KEY in the FMU host
  process env: fmi2Instantiate will detect the live sim and attach.
EOF

# ---- 4. zip --------------------------------------------------------------
echo "[4/4] zip -> $OUTPUT"
rm -f "$OUTPUT"
( cd "$STAGING" && zip -r -q "$OUTPUT" . )

echo
echo "FATTO: $OUTPUT"
ls -lh "$OUTPUT"
if [[ $KEEP_STAGING -eq 1 ]]; then
    echo "staging: $STAGING (mantenuto, --keep-staging)"
fi
