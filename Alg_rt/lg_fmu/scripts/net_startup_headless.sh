#!/bin/sh
#
# net_startup_headless.sh — minimal sim startup per la FMU autonoma.
#
# Differenze rispetto a Alg_rt/procedure/net_startup.sh:
#   - NIENTE check xhost  (la FMU gira in fmpy/Simulink, no X)
#   - NIENTE check_license algrt (lasciato al setup utente)
#   - NIENTE banco SUPERUSER (operator UI Motif: non serve)
#   - lancia solo `dispatcher` + `net_sked`, che è quanto serve a
#     RtCreateDbPunti + libdispatcher per dialogare via SD_goup.
#
# Lo script si aggiusta da solo il PATH: dato che vive in
#   $LEGOROOT/Alg_rt/lg_fmu/scripts/
# risale a $LEGOROOT e aggiunge $LEGOROOT/Alg_rt/bin al PATH se manca.
# Cosi' il caller (la FMU caricata da fmpy/Simulink) non deve aver
# preventivamente sourceato .profile_legoroot.
#
# Uso:
#   net_startup_headless.sh <task_dir>
#
# Exit code:
#   0 ok, sim avviata (dispatcher + net_sked in background)
#   1 task_dir mancante o no variabili.rtf
#   2 dispatcher non trovato in PATH
#   3 net_sked non trovato in PATH
#
# Nota: lo script ESCE subito dopo aver lanciato i background process.
# Sara' la FMU che pollera' RtCreateDbPunti finche' la SHM non e' pronta.

# NOTA: niente "set -u". .profile_legoroot referenzia variabili non sempre
# definite (HOME e' OK, ma alcuni alias/check fanno test su -z) e set -u
# fa abortire lo script su scoperta del primo unset.

# --- Risolve LEGOROOT dalla propria posizione --------------------------
SCRIPT_DIR=$(cd "$(dirname "$0")" 2>/dev/null && pwd)
# scripts/ -> lg_fmu -> Alg_rt -> $LEGOROOT
LEGOROOT=$(cd "$SCRIPT_DIR/../../.." 2>/dev/null && pwd)
export LEGOROOT

# Source .profile_legoroot e' OBBLIGATORIO: net_sked legge env tipo
# N001..N008/M001..M010/NR00/NP00 (parametri di dimensionamento) e
# LEGORT_INCLUDE/LEGOMMI_BIN/EXTENSION (via Alg_env.sh sourceato dentro
# il profile). Senza queste net_sked muore subito dopo il banner.
# Passiamo LEGOROOT come $1 cosi' il profile non prova a dedurlo da $0.
. "$LEGOROOT/.profile_legoroot" "$LEGOROOT" >/dev/null 2>&1 || true

# Glibc env utili (vedi net_startup.sh)
export MALLOCTYPE=3.1
export OS=Linux

# --- Cwd al task ------------------------------------------------------
if [ "$#" -lt 1 ]; then
    echo "ERR: usage: $0 <task_dir>" >&2
    exit 1
fi
cd "$1" || { echo "ERR: cd $1 fallito" >&2; exit 1; }

if [ ! -f variabili.rtf ]; then
    echo "ERR: $1 non e' una task LegoPST (manca variabili.rtf)" >&2
    exit 1
fi

# --- Pulizia eventuale sim precedente ---------------------------------
killsim 2>/dev/null || true

# --- Ripristino tavole acqua/vapore -----------------------------------
# killsim su Linux cancella TUTTE le SHM (vedi killsim.c:402 "test fittizio
# per LINUX"), inclusa SHR_TAV_KEY=999 che initav popola via .profile_legoroot.
# Senza questo step lg5sk muore con SIGFPE in trova_(tavole/trova.c:77)
# perche' le tavole acqua/vapore sono vuote.
# initav e' idempotente (ism01.c:62-65: se la SHM esiste -> iret=97 "GIA'
# PRESENTI", altrimenti la crea+popola).
command -v initav >/dev/null 2>&1 && initav 2>/dev/null || true

# --- Check prerequisiti -----------------------------------------------
command -v dispatcher >/dev/null 2>&1 || { echo "ERR: dispatcher non in PATH" >&2; exit 2; }
command -v net_sked   >/dev/null 2>&1 || { echo "ERR: net_sked non in PATH"   >&2; exit 3; }

# --- Parametri (default Simulator.tpl) --------------------------------
SNAP_S=60
BACK_T=30
CAMPIO=7200
NUM_VA=1000
PERTUR=50
SPARE_=1
PERTCL=0
TIME_BACK_T=120.0

# --- Lancio dispatcher + net_sked in background -----------------------
# Stdout/err ridiretti a file nel cwd; setsid stacca dal controlling tty
# cosi' i processi sopravvivono se la FMU/host muore senza Terminate.
LOG_DIR="$1"
exec </dev/null
nohup dispatcher \
    -num_snap $SNAP_S \
    -num_bktk $BACK_T \
    -num_camp_cr $CAMPIO \
    -num_var_cr $NUM_VA \
    -num_pert_active $PERTUR \
    -num_spare_forsnap $SPARE_ \
    -clear_pert_bktk $PERTCL \
    -time_bktk $TIME_BACK_T \
    >"$LOG_DIR/dispatcher.fmu.log" 2>&1 &

sleep 1

nohup net_sked 2 \
    -num_snap $SNAP_S \
    -num_bktk $BACK_T \
    -num_camp_cr $CAMPIO \
    -num_var_cr $NUM_VA \
    -num_pert_active $PERTUR \
    -num_spare_forsnap $SPARE_ \
    -clear_pert_bktk $PERTCL \
    -time_bktk $TIME_BACK_T \
    -no_priolo "" \
    >"$LOG_DIR/net_sked.fmu.log" 2>&1 &

# Lasciamo loop rapido per dar tempo ai processi di registrarsi nelle
# tabelle SHM prima che il caller riprenda il polling
sleep 1
exit 0
