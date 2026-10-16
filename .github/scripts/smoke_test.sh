#!/bin/bash
#
# smoke_test.sh - prove di FUNZIONAMENTO, dopo il build (fase 8 del piano).
#
# build_check.sh verifica che LegoPST compili; questo verifica che i
# programmi appena compilati FACCIANO il loro lavoro, su una demo vera e senza
# display. Lo usa il workflow .github/workflows/build.yml, nello stesso
# container del build e subito dopo. A mano, su una copia gia' compilata:
#
#   docker run --rm -v <copia>:/src -w /src aguagliardi/legopst:2.0 \
#       bash .github/scripts/smoke_test.sh
#
# Le prove, in ordine:
#   1. lgdemo     scarica e installa la demo "nuclear" dalla release (sha256
#                 controllato, Context.ctx allineati): e' l'unica pubblicata
#                 con le proc/ delle task, quindi simulabile senza legopc
#   2. lgmkstaz   round-trip del parser/writer dei faceplate sul catalogo e sul
#                 r01.dat della demo (lgmkstaz_test.tcl)
#   3. kUpSim     riallinea la configurazione del simulatore SLaurent_0
#                 (kConnex, kNetCompi, kCompStaz, kCollect; -nommi: niente
#                 pagine MMI, che vogliono un display)
#   4. simulazione  net_startup_headless.sh sulla task PWRN1PSS, probe_init,
#                 10 passi con probe_step --goup: il tempo simulato deve
#                 avanzare di 10 s
#   5. variabili  test_var_mapping sulla simulazione viva
#   6. topologia  uno snapshot.dat salvato con un'altra topologia (intestazione
#                 alterata apposta) va messo da parte all'avvio, e la
#                 simulazione deve partire lo stesso (ControlParam, fase 9)
#   7. task       le task della demo si ricostruiscono con i comandi di LegoPC,
#                 e i .o/.a dell'installazione che usano sono tutti in git
#                 (chi non compila prende quelli committati)
#
# Tutto avviene in una HOME provvisoria: la demo, i link legocad e sked, la
# scelta del simulatore. killsim fra una prova e l'altra: nel container le IPC
# sono solo sue, e non c'e' nessun altro da disturbare.
#
# Ogni passo ha un limite di tempo (timeout): una simulazione che non
# risponde non deve tenere fermo il runner. Era successo in prova: con
# net_sked che non partiva, probe_init aspettava la risposta per sempre.
#
# Uscita: 0 se tutte le prove passano, 1 altrimenti. Una prova fallita non
# ferma le successive, salvo quelle che dipendono da lei (senza demo non si
# prova niente).

cd "$(dirname "$0")/../.." || exit 1
RADICE="$(pwd)"
LOGDIR="${SMOKE_LOG_DIR:-$RADICE/smoke-logs}"
mkdir -p "$LOGDIR"
export HOME
HOME="$(mktemp -d /tmp/smoke_home.XXXXXX)"

# lancia <secondi> <comando...>: il comando con un limite di tempo; 124 se
# scade (timeout manda TERM, e KILL 10 s dopo se non basta)
lancia() { local t="$1"; shift; timeout -k 10 "$t" "$@"; }
scaduto() { [ "$1" -eq 124 ] || [ "$1" -eq 137 ]; }

RISULTATI=()      # "nome|esito|nota"
FALLITE=0
esito() {         # $1 nome  $2 0/1  $3 nota
    local e="PASS"
    if [ "$2" -ne 0 ]; then e="FAIL"; FALLITE=$((FALLITE + 1)); fi
    set -- "$1" "$2" "$(printf '%s' "$3" | tr -s ' ' | sed 's/ *$//')"
    RISULTATI+=("$1|$e|$3")
    printf '%-12s %s  %s\n' "$1" "$e" "$3"
    if [ "$e" = FAIL ] && [ -n "${GITHUB_ACTIONS:-}" ]; then
        echo "::error::smoke test $1: ${3//%/%25}"
    fi
}
riepilogo() {
    if [ -n "${GITHUB_STEP_SUMMARY:-}" ]; then
        {
            echo "### Prove di funzionamento"
            echo ""
            echo "| prova | esito | nota |"
            echo "|---|---|---|"
            for r in "${RISULTATI[@]}"; do
                IFS='|' read -r n e t <<< "$r"
                echo "| $n | $e | $t |"
            done
        } >> "$GITHUB_STEP_SUMMARY"
    fi
}

echo "HOME provvisoria: $HOME"
echo "log:              $LOGDIR"
echo ""

# --- 1. demo -------------------------------------------------------------
lancia 600 util97/bin/lgdemo --base "$HOME" -y nuclear > "$LOGDIR/1_lgdemo.log" 2>&1
RC=$?
DEMO="$HOME/legopst_nuclear"
if [ $RC -eq 0 ] && [ -d "$DEMO/legocad" ] && [ -d "$DEMO/sked/SLaurent_0" ]; then
    esito lgdemo 0 "demo nuclear installata ($(du -sh "$DEMO" | cut -f1))"
else
    esito lgdemo 1 "installazione fallita (uscita $RC): $(grep -m1 -i 'errore' "$LOGDIR/1_lgdemo.log")"
    riepilogo
    exit 1
fi
ln -s legopst_nuclear/legocad "$HOME/legocad"
ln -s legopst_nuclear/sked "$HOME/sked"
echo SLaurent_0 > "$HOME/.legosim"

# Il profilo ricava tutto da HOME (LG_ENTRY, KSKED, KSIM...). La chiave IPC la
# fissiamo noi: il profilo la calcola come uid*10000, e nel container l'uid
# e' 0.
# shellcheck disable=SC1091
. ./.profile_legoroot "$RADICE" >/dev/null 2>&1
export SHR_USR_KEY=61000000 SHR_USR_KEYS=61001000
initav >/dev/null 2>&1

# --- 2. lgmkstaz ---------------------------------------------------------
lancia 120 tclsh Alg_legopc/src/tix/lgmkstaz_test.tcl </dev/null > "$LOGDIR/2_lgmkstaz.log" 2>&1
RC=$?
esito lgmkstaz $RC "$(grep -m1 '^== ' "$LOGDIR/2_lgmkstaz.log" | tr -d '=' | sed 's/^ *//')"

# --- 3. kUpSim -----------------------------------------------------------
( cd /tmp && lancia 600 kUpSim -nommi ) > "$LOGDIR/3_kUpSim.log" 2>&1
RC=$?
if scaduto $RC; then
    esito kUpSim 1 "timeout: non ha finito in 600 s"
elif [ $RC -eq 0 ] && ! grep -q "NON AGGIORNATA" "$LOGDIR/3_kUpSim.log"; then
    esito kUpSim 0 "SLaurent_0 riallineato ($(grep -c '| End |' "$LOGDIR/3_kUpSim.log") passi)"
else
    esito kUpSim 1 "$(grep -m1 'NON AGGIORNATA\|ERROR' "$LOGDIR/3_kUpSim.log")"
fi
killsim >/dev/null 2>&1

# --- 4. simulazione senza grafica ----------------------------------------
TASK="$HOME/legocad/PWRN1PSS"
T_FIN=""
lancia 120 Alg_rt/lg_fmu/scripts/net_startup_headless.sh "$TASK" > "$LOGDIR/4_avvio.log" 2>&1
RC=$?
sleep 2
# lo script esce appena ha lanciato i processi in background: che siano vivi
# lo si controlla qui, prima di mandare comandi a una simulazione che non c'e'
VIVI=""
for p in dispatcher net_sked; do pgrep -x "$p" >/dev/null && VIVI="$VIVI $p"; done
if [ $RC -ne 0 ]; then
    esito simulazione 1 "net_startup_headless.sh uscito con $RC"
elif [ "$VIVI" != " dispatcher net_sked" ]; then
    esito simulazione 1 "dopo l'avvio sono vivi solo:${VIVI:- nessuno} (servono dispatcher e net_sked)"
else
    ( cd "$TASK" && lancia 120 "$RADICE/Alg_rt/lg_fmu/tools/probe_init" ) > "$LOGDIR/4_init.log" 2>&1
    RC_INIT=$?
    ( cd "$TASK" && lancia 120 "$RADICE/Alg_rt/lg_fmu/tools/probe_step" --goup 10 ) > "$LOGDIR/4_step.log" 2>&1
    RC_STEP=$?
    T_INI=$(sed -n 's/.*tempo iniziale *: *\([0-9.]*\).*/\1/p' "$LOGDIR/4_step.log")
    T_FIN=$(sed -n 's/.*tempo finale *: *\([0-9.]*\).*/\1/p' "$LOGDIR/4_step.log")
    AVANZ=$(awk -v a="${T_INI:-0}" -v b="${T_FIN:-0}" 'BEGIN{printf "%.3f", b-a}')
    if [ $RC_INIT -eq 0 ] && [ $RC_STEP -eq 0 ] && [ "$AVANZ" = "10.000" ]; then
        esito simulazione 0 "PWRN1PSS: 10 passi, t da ${T_INI} a ${T_FIN} s"
    else
        NOTA="init=$RC_INIT step=$RC_STEP, avanzamento ${AVANZ} s (atteso 10.000)"
        if scaduto $RC_INIT || scaduto $RC_STEP; then NOTA="timeout: la simulazione non risponde ($NOTA)"; fi
        esito simulazione 1 "$NOTA"
    fi
fi

# --- 5. mappa delle variabili --------------------------------------------
if [ -n "$T_FIN" ]; then
    ( cd "$TASK" && lancia 120 "$RADICE/Alg_rt/lg_fmu/tests/test_var_mapping" ) > "$LOGDIR/5_variabili.log" 2>&1
    RC=$?
    esito variabili $RC "$(grep -m1 'outputs uniche' "$LOGDIR/5_variabili.log" | sed 's/^ *//'), $(grep -m1 'inputs uniche' "$LOGDIR/5_variabili.log" | sed 's/^ *//')"
else
    esito variabili 1 "saltata: la simulazione non e' partita"
fi
killsim >/dev/null 2>&1
# i log delle task restano nella demo: li si porta con gli altri
cp "$TASK"/*.fmu.log "$LOGDIR"/ 2>/dev/null

# --- 6. snapshot di un'altra topologia ------------------------------------
# Lo snapshot.dat della task, scritto dalla simulazione della prova 4, e' della
# topologia attuale. Gli si cambia nell'intestazione la dimensione dell'area
# dati (il primo int): per ControlParam diventa uno snapshot di un altro
# modello, e all'avvio va messo da parte come snapshot.dat.incompatibile.
SNAP="$TASK/snapshot.dat"
if [ -z "$T_FIN" ] || [ ! -f "$SNAP" ]; then
    esito topologia 1 "saltata: manca la simulazione della prova 4 o il suo snapshot.dat"
else
    rm -f "$TASK/snapshot.dat.incompatibile" "$TASK/parametri.out"
    VERO=$(od -An -t d4 -N4 "$SNAP" | tr -d ' ')
    FALSO=$((VERO + 4))
    printf "$(printf '\\%03o\\%03o\\%03o\\%03o' $((FALSO & 255)) $(((FALSO >> 8) & 255)) $(((FALSO >> 16) & 255)) $(((FALSO >> 24) & 255)))" \
        | dd of="$SNAP" bs=1 count=4 conv=notrunc 2>/dev/null
    lancia 120 Alg_rt/lg_fmu/scripts/net_startup_headless.sh "$TASK" > "$LOGDIR/6_avvio.log" 2>&1
    sleep 2
    VIVI=""
    for p in dispatcher net_sked; do pgrep -x "$p" >/dev/null && VIVI="$VIVI $p"; done
    if [ ! -f "$TASK/snapshot.dat.incompatibile" ]; then
        esito topologia 1 "lo snapshot con area dati $FALSO byte (vera: $VERO) non e' stato messo da parte"
    elif ! grep -q "altra topologia" "$TASK/parametri.out" 2>/dev/null; then
        esito topologia 1 "messo da parte, ma parametri.out non dice perche'"
    elif [ "$VIVI" != " dispatcher net_sked" ]; then
        esito topologia 1 "messo da parte, ma la simulazione non e' partita (vivi:${VIVI:- nessuno})"
    else
        esito topologia 0 "snapshot da $FALSO byte messo da parte, simulazione partita"
    fi
    cp "$TASK/parametri.out" "$LOGDIR/6_parametri.out" 2>/dev/null
    killsim >/dev/null 2>&1
fi

# --- 7. ricostruzione delle task -----------------------------------------
# Le task si collegano sulla macchina dell'utente, con oggetti e librerie
# dell'installazione (main_lg*.o, rg5sim.o, trapfpe.o, legolib.a...), e chi
# non compila prende quelli committati: se uno non e' in git, il link fallisce
# da lui e non qui, dove il build li ha appena rifatti tutti. Quindi si
# ricostruiscono le task della demo con i comandi di LegoPC (cad_crealg1,
# cad_crealg3 e crealg5 per il processo, cad_crealg5sk, cad_marcotask per la
# regolazione semplificata), e ogni .o/.a dell'installazione usato nel link
# deve essere tracciato da git. L'elenco ragionato e' in docs/BUILD.md.
OGGETTI="$LOGDIR/7_oggetti.txt"
: > "$OGGETTI"
NOTA=""
costruisci() {    # $1 task  $2 log  $3... comando
    local t="$1" log="$LOGDIR/7_$2.log"; shift 2
    ( cd "$HOME/legocad/$t" && lancia 300 "$@" ) > "$log" 2>&1 || { NOTA="$NOTA $t:$*"; return; }
    grep -oE "(^|[[:space:]])$RADICE/[^[:space:]]+\.(o|a)\b" "$log" | sed 's/^[[:space:]]//' >> "$OGGETTI"
}
rm -f "$HOME"/legocad/PWRN1PSS/proc/{lg2,lg3,lg5} "$HOME"/legocad/SLB1_NI2/proc/lg5sk \
      "$HOME"/legocad/r_SLB1_0/proc/{lg5sk,pslego3}
costruisci PWRN1PSS lg1   cad_crealg1 p
costruisci PWRN1PSS lg3   cad_crealg3 p
costruisci PWRN1PSS lg5   crealg5
costruisci SLB1_NI2 lg5sk cad_crealg5sk p
costruisci r_SLB1_0 rg5sk make -f "$LEGO_BIN/cad_marcotask" rg5sk
for f in PWRN1PSS/proc/lg2 PWRN1PSS/proc/lg3 PWRN1PSS/proc/lg5 SLB1_NI2/proc/lg5sk r_SLB1_0/proc/lg5sk; do
    [ -x "$HOME/legocad/$f" ] || NOTA="$NOTA manca:$f"
done
FUORI=""
for o in $(sort -u "$OGGETTI"); do
    r=$(realpath --relative-to="$RADICE" "$o")
    git -c safe.directory='*' ls-files --error-unmatch "$r" >/dev/null 2>&1 || FUORI="$FUORI $r"
done
if [ -n "$NOTA" ]; then
    esito task 1 "costruzione fallita:$NOTA"
elif [ -n "$FUORI" ]; then
    esito task 1 "collegate con file dell'installazione che non sono in git:$FUORI"
else
    esito task 0 "5 task ricostruite, $(sort -u "$OGGETTI" | wc -l) oggetti e librerie dell'installazione, tutti in git"
fi

echo ""
echo "prove fallite: $FALLITE su ${#RISULTATI[@]}"
riepilogo
[ "$FALLITE" -eq 0 ]
