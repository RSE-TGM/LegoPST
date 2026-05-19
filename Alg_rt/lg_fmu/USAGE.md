# LegoCliSINC FMU — guida utente e developer

FMU FMI 2.0 Co-Simulation per il runtime di simulazione LegoPST su Linux.

Esistono **due varianti** della stessa FMU, con stesso nome modello (`LegoCliSINC`)
e stessa interfaccia FMI ma diversa modalità di deployment.

Entrambe sono "bound al task": una FMU vale per la singola task LegoPST sulla
quale è stata generata (`TASK_PATH` embedded in `resources/task_info.env`).
Per ogni task serve quindi un build dedicato.

Output di entrambe le varianti: file `.fmu` nella **directory della task**
(es. `/home/antonio/legocad/collet/`), non in `/tmp`. Anche la dir di unzip
del bundle (`legoclix_<task>_bundle/`) deve stare accanto alla `.fmu`.

---

## Sezione utente

### Differenze e limiti tra le varianti

| Aspetto | base | bundle |
|---------|------|--------|
| File | `legoclix_<task>.fmu` | `legoclix_<task>_bundle.fmu` |
| Dimensione tipica | ~80 KB | ~3.8 MB |
| Self-contained | ❌ | ✅ |
| Richiede LegoPST installato sul target | ✅ stesso `LEGOROOT`, stesso utente | ❌ |
| Vincolo glibc target | quella del target locale (di solito ok) | ≥ 2.38 (baseline `net_sked` del build host) |
| Architettura | x86_64 Linux | x86_64 Linux |
| Gira in container Linux pulito (es. `python:3.11-slim`) | ❌ | ✅ validato 2026-05-02 |
| Modalità attach (sim già viva) | ✅ | ✅ |
| Modalità launch (FMU lancia la sim da sola) | ✅ | ✅ |
| Multi-istanza concorrente stesso uid | ✅ max 8 (slot 1..8) | ✅ max 8 (slot 1..8) |
| Coesistenza con banco operatore | ✅ slot 0 = `uid*10000` riservato | ✅ slot 0 = `uid*10000` riservato |
| Tempo di build | rapido (~1-2 s) | ~5-15 s (rsync runtime + task) |
| Aggiornamento LegoPST sul target | basta aggiornare LegoPST | richiede rebuild della FMU |
| Use case tipico | dev workstation, CI con LegoPST installato | shipping a colleghi, container, deployment senza LegoPST |
| Compatibilità Simulink Windows | ❌ serve FMU Windows separata | ❌ serve FMU Windows separata |
| Compatibilità ARM / non-x86_64 | ❌ rebuild dedicato | ❌ rebuild dedicato |

**Quando scegliere base**: il target ha LegoPST già installato (dev box, collega con setup completo, CI con immagine LegoPST). Più piccola, build rapido, eredita automaticamente upgrade di LegoPST sul target.

**Quando scegliere bundle**: il target non ha LegoPST (container minimal, collega senza setup, demo). Self-contained ma vincolato alla glibc del build host (≥ 2.38 → ok per Ubuntu 24.04 / Debian 13 / Fedora 39+; per target più vecchi serve rebuild su container con baseline glibc più bassa, vedi `Compatibilità glibc` più sotto).

In entrambi i casi la FMU è **Linux x86_64**. Per Simulink Windows serve la versione Windows separata (`/home/antonio/legopc_prj/src/legosim/LegoCliSINC_fmu`).

### Metodi di esecuzione (FMI master)

Il file `.fmu` è uno standard FMI 2.0 e può essere caricato da qualunque master FMI conforme. Tabella dei metodi testati / supportati:

| Metodo | Variante FMU | Setup richiesto | Use case | Stato |
|--------|--------------|-----------------|----------|-------|
| `run_fmu.sh` (wrapper bash su fmpy) | base, bundle | `source .profile_legoroot` + `/home/antonio/fmpy_venv` | smoke test rapido in CLI con CSV out | ✅ supportato |
| `lg_cosim.py` (co-simulazione N FMU) | bundle | `/home/antonio/fmpy_venv`; `config.json` con fmus/connections | co-simulazione Gauss-Seidel di più FMU con scambio variabili, log CSV, sync RT; vedi `oms_master/lg_cosim_manual.md` | ✅ validato 2026-05-11 (collet + ctrcoll) |
| `fmpy.simulate_fmu` (Python diretto) | base, bundle | `pip install fmpy` (Python ≥ 3.8) | scripting, integrazione test, debug fine-grained con `LG_FMU_DEBUG=1` | ✅ supportato |
| Container Linux pulito (`docker run python:3.11-slim` + fmpy) | **solo bundle** | `pip install fmpy` nel container | deployment, CI esterna, demo | ✅ validato 2026-05-02 |
| `test_fmu_docker` (wrapper bash su docker + fmpy) | **solo bundle** | Docker installato e avviato; `DISPLAY` per grafica post-sim | smoke test parallelo di più FMU in container isolato ed effimero | ✅ supportato |
| Simulink R2023a (Linux master) | base, bundle | Simulink Linux + FMI Toolbox | integrazione modelli misti | ⏸ non testato (utente usa Simulink Windows) |
| Simulink R2023a (Windows host) | nessuna | — | — | ❌ serve FMU Windows separata, non Linux |
| OpenModelica | base, bundle | `dnf/apt install openmodelica` | validazione cross-tool open-source | ⏸ non testato (non installato) |
| FMI Compliance Checker (Modelica Association) | base, bundle | jar standalone | validazione `modelDescription.xml` + .so contro spec FMI 2.0 | ✅ 0 issue 2026-04-30 |

**Limitazioni comuni a tutti i metodi:**
- Get/Set per Integer / Boolean / String → `fmi2Discard` (solo Real esposto).
- FMUstate / Serialize / DeSerialize → `fmi2Error` (`canGetAndSetFMUstate=false`).
- DirectionalDerivative / RealOutputDerivatives / RealInputDerivatives / CancelStep → `fmi2Error`.
- `communicationStepSize` < `dt_sked` → warning (la FMU avanza comunque di 1 `dt_sked`, il tempo "gonfia"); non `fmi2Discard` perché fmpy non lo gestisce e va in loop.
- 9ª FMU concorrente sullo stesso uid → fallisce con `[lg_fmu] tutti gli 8 slot...` (vedi sezione multi-istanza più sotto).

### Generare una FMU (variante base)

Dalla UI tix (consigliato): menu **Tools → Build FMU**. Apre un xterm con
l'output di `dolgfmu.sh` sulla task corrente (`$DIRMODEL`).

Da CLI (gli script sono installati in `Alg_rt/bin/` come gli altri eseguibili
LegoPST, quindi chiamabili direttamente dopo `source .profile_legoroot`):
```bash
source $LEGOROOT/.profile_legoroot
dolgfmu /home/antonio/legocad/collet
# Output: /home/antonio/legocad/collet/legoclix_collet.fmu
```

`dolgfmu.sh` rileva se la sim sulla task è già viva (attach) o no
(headless launch + probe_init + build + killsim finale). La sim viva è
necessaria perché il `modelDescription.xml` viene generato leggendo
nomi/indirizzi/start values dalla SHM live (vedi sezione developer
"Perché serve una sim viva durante il build della FMU").

### Generare una FMU (variante bundle)

Dalla UI tix (consigliato): menu **Tools → Build FMU (bundle)**.

Da CLI tramite `dolgfmu -b`:
```bash
source $LEGOROOT/.profile_legoroot
dolgfmu -b /home/antonio/legocad/collet
# Output: /home/antonio/legocad/collet/legoclix_collet_bundle.fmu
```

`dolgfmu.sh -b` riusa la stessa logica detect/start/cleanup della variante
base (attach se sim viva, altrimenti headless launch + probe_init + killsim
finale), e passa `-b` a `bundle/build.sh`.

Da CLI tramite `bundle/build.sh` direttamente (senza orchestrazione sim):
```bash
source $LEGOROOT/.profile_legoroot
cd /home/antonio/legocad/collet
# la sim deve essere gia' attiva sulla task
$LEGOROOT/Alg_rt/lg_fmu/bundle/build.sh -b \
    -o /home/antonio/legocad/collet/legoclix_collet_bundle.fmu
```

Opzioni utili di `build.sh`:
- `-b` / `--bundle`: include runtime self-contained (questa è la differenza chiave)
- `-o PATH`: output path (default `<cwd>/<MODEL_NAME>.fmu`; convenzione: `<task>/legoclix_<task>_bundle.fmu`)
- `-n NAME`: model name / identifier (default `LegoCliSINC`)
- `-k`: keep staging dir (debug del contenuto del bundle prima dello zip)

### Eseguire la FMU con `run_fmu.sh` (wrapper utente, sim auto-gestita)

```bash
source $LEGOROOT/.profile_legoroot
# variante base
run_fmu /home/antonio/legocad/collet --stop-time 30
# variante bundle
run_fmu -b /home/antonio/legocad/collet --stop-time 30
# CSV in /home/antonio/legocad/collet/results.csv
```

L'argomento posizionale può essere il path al `.fmu`, una task dir LegoPST, oppure
omesso (cerca `*.fmu` o `*_bundle.fmu` nella cwd). Con `-b`/`--bundle` cerca
`legoclix_<task>_bundle.fmu`.

La FMU viene estratta in una directory **accanto al `.fmu`** (es.
`<task>/legoclix_<task>_bundle/`), non in `/tmp/fmpy_*`. La dir di unzip
persiste tra run, conforme alla regola di output path del progetto. Il CSV
finisce in `<fmu_dir>/results.csv` di default.

Subcommand:
- (default): simulate → CSV
- `--info`: lista variabili (no sim)
- `--validate`: passa attraverso `fmpy.validation`

Opzioni:
- `-b`, `--bundle`: usa la variante bundle (`legoclix_<task>_bundle.fmu`)
- `--stop-time T` (default 30 s)
- `--step-size DT` (default da modelDescription.xml)
- `--set VAR=VAL` (ripetibile, override input)
- `--csv FILE` (default `<fmu_dir>/results.csv`)

### Visualizzare i grafici di simulazione con `run_graphics.sh`

Il bundle include il programma `graphics` (viewer Motif/X11 per file `f22circ.dat`)
e il wrapper `run_graphics.sh` che imposta l'ambiente corretto. Il file `f22circ.dat`
viene prodotto dalla simulazione durante ogni `fmi2DoStep` (via `net_prepf22`) e
si trova nella task bundled estratta.

**Nota storica**: `graphics` vuole il path **senza l'estensione `.dat`** (es.
`.../task/collet/f22circ`). Il wrapper gestisce questo automaticamente.

```bash
# Dopo aver estratto il bundle ed eseguito la simulazione:
cd /home/antonio/legocad/collet/legoclix_collet_bundle/resources/bundle

# senza argomento: trova f22circ in task/*/
./run_graphics.sh

# con path esplicito (accetta con o senza .dat)
./run_graphics.sh task/collet/f22circ
./run_graphics.sh task/collet/f22circ.dat   # .dat rimosso automaticamente

# oppure dalla task dir
./run_graphics.sh task/collet/
```

Requisiti:
- `DISPLAY` impostato (sessione X11 o WSL con server X11, es. VcXsrv / X410)
- Il bundle è self-contained per Motif (`libXm.so.4`, `libMrm.so.4`) e le altre
  dipendenze X11 non standard (`libXp.so.6`): vengono bundlate dal ldd scan
  automatico in `build.sh`

Il wrapper setta:
- `LEGORT_UID=$BUNDLE/Alg_rt/uid/` → `graphics.uid` (file MRM compilato)
- `HOME=$BUNDLE/home_stub/` → `chdefaults()` trova `defaults/uni_misc.dat`
  (unità di misura: portata, pressione, temperatura, …)
- `LD_LIBRARY_PATH=$BUNDLE/lib/` → Motif + altre lib non standard

### Eseguire la FMU bundle in env pulito (verifica self-containment)

```bash
cd /home/antonio/legocad/collet
env -i HOME=$HOME USER=$USER PATH=/usr/bin:/usr/local/bin \
  /home/antonio/fmpy_venv/bin/python3 -c "
import os
from fmpy import simulate_fmu, extract
unz = os.path.abspath('legoclix_collet_bundle')
extract('legoclix_collet_bundle.fmu', unzipdir=unz)
result = simulate_fmu(unz, stop_time=10)
print(f'{len(result)} sample. Ultimo: {result[-1]}')
"
```

Note:
- `env -i` rimuove tutto l'environment ereditato → testa che il bundle sia davvero self-contained.
- `unzipdir` accanto al `.fmu` (non `/tmp/fmpy_*`) per coerenza con la regola di output path.
- `os.path.abspath` su Python ≥ 3.13: `pathlib.Path(...).as_uri()` chiamato da `fmpy.fmi2.instantiate` rifiuta path relativi (`ValueError: relative path can't be expressed as a file URI`).
- `LG_FMU_DEBUG=1` (env var aggiuntiva) attiva il dump diagnostico `[lg_fmu DBG] ...` su stderr.

### Eseguire la FMU bundle in container Linux pulito (deployment)

Validato su `python:3.11-slim` (debian-trixie, glibc 2.41, no LegoPST installato):

```bash
docker run --rm \
  -v /home/antonio/legocad/collet/legoclix_collet_bundle.fmu:/tmp/bundle.fmu:ro \
  python:3.11-slim bash -c '
pip install --quiet fmpy
python -c "
import os
from fmpy import simulate_fmu, extract
unz = os.path.abspath(\"/tmp/b\")
extract(\"/tmp/bundle.fmu\", unzipdir=unz)
result = simulate_fmu(unz, stop_time=10)
print(f\"OK {len(result)} sample\")"'
```

**Compatibilità glibc**: la baseline dei binari del bundle è **GLIBC_2.38** (limite imposto da `net_sked`). Distribuzioni compatibili:

| Distro | glibc | Compatibile |
|--------|-------|-------------|
| Ubuntu 22.04 | 2.35 | ❌ no |
| Ubuntu 24.04 | 2.39 | ✅ sì |
| Debian 12 (bookworm) | 2.36 | ❌ no |
| Debian 13 (trixie) | 2.41 | ✅ sì |
| Fedora 39+ | 2.38+ | ✅ sì |

Per target con glibc < 2.38 servirebbe rebuild dei binari LegoPST in un container con baseline più bassa (es. `aguagliardi/legopst_multi:2.0` su Ubuntu 20.04 = glibc 2.31).

**Note di implementazione (non rilevanti per l'utente, lette dalla FMU automaticamente):**
- fmpy estrae lo zip via Python `zipfile` che NON preserva il bit `+x` → la FMU invoca `bash restore_perms.sh` (generato da `bundle/build.sh`) prima del launch.
- Container con utente root (uid=0): SysV SHM key=0 = `IPC_PRIVATE`, non condivisibile. La FMU ricalcola `SHR_USR_KEY = getpid()*10 + 10000` come fallback.
- Multi-istanza (uid≠0, sim non viva): la FMU sceglie automaticamente uno degli 8 slot `uid*10000 + slot*1100` (slot 1..8) disponibili. Lo slot 0 (= `uid*10000`) è riservato alla sessione LegoPST normale dell'utente: aprendo il banco operatore mentre una FMU gira, le SHM non collidono. Limite: max 8 FMU concorrenti per uid. In co-simulazione tramite `lg_cosim.py`, il probe degli slot è fatto lato Python (con `ID_SHM_VAR=5`) anziché dentro `lg_fmi2.c` (che usa `ID_SHM_SIM=0`, mai creato in headless → tutti i slot sembrano liberi → colliderebbero); `lg_cosim.py` pre-assegna `SHR_USR_KEY` via `ctypes.setenv` prima di ogni `fmi2Instantiate`.
- Co-simulazione e `killsim`: su Linux `killsim` cancella **tutte** le SHM dell'utente (non solo il proprio slot). In co-simulazione, il `killsim` chiamato da `net_startup_headless.sh` di ogni FMU distruggerebbe le SHM delle FMU già avviate. `lg_cosim.py` setta `LG_COSIM_NO_KILLSIM=1` nell'environment C prima dell'instantiate, e `net_startup_headless.sh` (sia quello installato in `Alg_rt/bin/` sia quelli estratti dai bundle, patchati on-the-fly da `_patch_killsim_guard()`) salta il `killsim` quando questa variabile è impostata.
- `net_startup_headless.sh` ha shebang `bash` (non `sh`): in debian/ubuntu `/bin/sh = dash` non digerisce i costrutti bash di `.profile_legoroot` (`set -o emacs`, `[[ ]]`).

### Testare una o più FMU bundle in container con `test_fmu_docker`

`test_fmu_docker` (installato in `Alg_rt/bin/` dal Makefile) automatizza il
lancio di uno o più bundle FMU in un singolo container `python:3.11-slim`
effimero. Le simulazioni vengono eseguite **in parallelo** (una per FMU,
subshell bash in background + `wait`), ciascuna con la propria directory di
estrazione.

**Sintassi**

```bash
test_fmu_docker [-t SECONDI] <fmu_1.fmu> [fmu_2.fmu ...]
```

- `-t SECONDI`: durata della simulazione (default: 10 s)
- Senza argomenti o con `-h`/`--help`: mostra l'usage

**Esempi**

```bash
# smoke test singolo (10s default)
test_fmu_docker legoclix_collet_bundle.fmu

# simulazione di 25 secondi
test_fmu_docker -t 25 legoclix_TCon-r1_bundle.fmu

# due FMU in parallelo per 5 secondi
test_fmu_docker -t 5 legoclix_collet_bundle.fmu legoclix_TCon-r1_bundle.fmu
```

**Cosa fa**

1. Verifica che tutti i file `.fmu` esistano e calcola i path assoluti.
2. `xhost +local:docker` per abilitare X11 dal container.
3. `docker run --rm -it --net host python:3.11-slim` con:
   - ogni `.fmu` montato in sola lettura come `/tmp/fmu_N.fmu`
   - `STOP_TIME` e `FILES` passati come variabili d'ambiente
   - `/tmp/.X11-unix` e `DISPLAY` per la grafica
4. Nel container: `pip install fmpy`, poi per ogni FMU in parallelo:
   - `fmpy.extract` nella dir `/tmp/dir_<basename>`
   - `fmpy.simulate_fmu` per `STOP_TIME` secondi
   - se `resources/bundle/run_graphics.sh` è presente, lo esegue (apre il viewer Motif `graphics` su `f22circ.dat`)
5. `wait` — aspetta il completamento di tutte le simulazioni prima di uscire.
6. Il container si autodistrugge (`--rm`).

**Prerequisiti**

- Docker installato e il demone in esecuzione (`systemctl start docker` o Docker Desktop)
- `DISPLAY` impostato (sessione X11 o WSL con X server, es. VcXsrv/X410) — necessario solo se si vuole la grafica post-sim
- Solo bundle FMU (la variante base richiede LegoPST installato sul target)

**Limitazioni**

| Aspetto | Dettaglio |
|---------|-----------|
| Dati effimeri | Container con `--rm`: `f22circ.dat`, log di dispatcher/net_sked e risultati CSV vivono solo dentro il container; non sono accessibili dall'host dopo l'uscita |
| `fmpy` installato ad ogni run | Nessuna cache Docker per fmpy → ~30–60 s di overhead al primo avvio. Per uso frequente, costruire un'immagine con fmpy pre-installato |
| Grafica | `run_graphics.sh` apre una finestra X11 sul desktop dell'host; se il server X non è raggiungibile, il viewer fallisce silenziosamente (la simulazione è comunque completata) |
| glibc | Come per qualunque bundle FMU: baseline GLIBC_2.38. `python:3.11-slim` (Debian trixie, glibc 2.41) è compatibile ✅ |
| IPC per FMU multiple | Ogni FMU sceglie uno slot `SHR_USR_KEY` indipendente (P5). In container con uid=0 il fallback è `getpid()*10+10000` — pid diversi → chiavi diverse → nessuna collisione |

### Cleanup dopo i test

```bash
source $LEGOROOT/.profile_legoroot
killsim                              # uccide dispatcher/net_sked/lg5sk + pulisce SHM
ipcs -m                              # verifica nessun segmento residuo
```

`killsim` su Linux cancella **tutte** le SHM utente, inclusa `SHR_TAV_KEY=999`
(tabelle acqua/vapore). `net_startup_headless.sh` la ripopola via `initav`
al prossimo lancio (idempotente). Vedi `reference_shm_keys_lego.md`.

---

## Sezione developer

### Layout sorgenti `Alg_rt/lg_fmu/`

```
include/
  fmi/headers/        # header standard FMI 2.0
  lg_fmi2_state.h     # struct istanza Linux
  lg_var_mapping.h    # API var mapping (open/close/by_name/by_addr)
src/
  lg_var_mapping.c    # wrapper su costruisci_var (libsim)
  lg_fmi2.c           # 33 entry point FMI 2.0 + attach_or_launch + init_sim_if_stopped
  Makefile.mk         # liblg_fmu.a + LegoCliSINC.so
bundle/
  modelDescription.xml.in   # template con placeholder @KEY@
  build.sh                  # builder .fmu (con/senza --bundle)
tools/
  gen_modeldescription      # genera modelDescription.xml dalla sim attiva
  probe_init                # SD_inizializza(BI) standalone (diagnostico)
  probe_step                # SD_goup(BI) N volte (diagnostico cadenza)
  probe_attach              # attach + read smoke test
scripts/
  dolgfmu.sh                # entry point UI: build FMU base / bundle sulla task
  run_fmu.sh                # wrapper fmpy per eseguire una FMU
  net_startup_headless.sh   # dispatcher + net_sked headless (no banco, no X)
  test_fmu_docker.sh        # smoke test FMU bundle in container Docker (anche in parallelo)
tests/
  test_var_mapping          # test offline su lg_var_mapping
Makefile.mk                 # installa gli script in Alg_rt/bin/ (no .sh nel nome)
USAGE.md                    # questo file
```

Il `Makefile.mk` di `Alg_rt/lg_fmu` e' agganciato a `Alg_rt/Makefile.mk` (`make`
globale lo richiama dopo `procedure/`, `util/` e `algrt_db/`). Installa i 3
script in `Alg_rt/bin/` rinominandoli senza estensione, cosi' sono nel PATH
dopo `source .profile_legoroot`. Pattern coerente con
`Alg_rt/procedure/Makefile.mk` (cp + chmod 755).

I sorgenti C/headers (`src/`, `tools/`, `tests/`, `include/`) e il builder
`bundle/build.sh` hanno Makefile locali e build manuale, NON agganciati al
build globale (eviterebbe di forzare `-fPIC` su tutta `AlgLib` ad ogni `make`
di root).

### Build da zero

Pre-requisito: `source $LEGOROOT/.profile_legoroot`.

```bash
# 1. AlgLib con -fPIC (già fatto in P2.5; rifare solo dopo modifica .c di libsim/Rt/...)
cd $LEGOROOT/AlgLib && make -f Makefile.mk

# 2. liblg_fmu.a + LegoCliSINC.so
cd $LEGOROOT/Alg_rt/lg_fmu/src && make -f Makefile.mk

# 3. tools (gen_modeldescription, probe_*)
cd $LEGOROOT/Alg_rt/lg_fmu/tools && make -f Makefile.mk

# 4. tests (opzionale)
cd $LEGOROOT/Alg_rt/lg_fmu/tests && make -f Makefile.mk
```

### Flusso `fmi2Instantiate` (sintesi)

1. `parse_resource_uri` — `file:///abs/path` → `/abs/path` (resources dir)
2. `read_env_value` su `resources/task_info.env`:
   - `TASK_PATH`, `LEGOROOT`, `BUNDLE_MODE`
   - **12 var LEGO** (`N000..N007`, `M001..M005`) → `setenv` (overwrite=0)
3. `setup_legopst_env`: `SHR_USR_KEY` (se non in env: probe slot 1..8 di `uid*10000+slot*1100` via `shmget(ID_SHM_SIM)` finché ENOENT — slot 0 riservato alla sessione LegoPST normale; uid=0 fallback `getpid()*10+10000`), `SHR_USR_KEYS = SHR_USR_KEY+1000`, `OS=Linux`, `PATH`, `LD_LIBRARY_PATH` (bundle), `SHR_TAV_KEY=999` (bundle)
4. `chdir(TASK_PATH)`
5. `try_attach_db` (`RtCreateDbPunti(errore, "TEST", ...)`). `file_top="TEST"` (anziché `NULL`) seleziona `condizione=2` in `InitializeDbPunti` ([RtDbPunti.c:121-127](AlgLib/Rt/RtDbPunti.c#L121-L127)), che in caso di magic mismatch sgancia silenziosamente la SHM invece di emettere `[error shared-memory not attached]`: per noi il fail è atteso (probing pre-launch). Se OK → attach mode (`we_started_sim=0`). Se NO → `launch_sim_and_wait` (`we_started_sim=1`)
   - In bundle mode: pre-launch invoca `bash restore_perms.sh` (riapplica `chmod +x` post-fmpy.extract)
6. **`init_sim_if_stopped`**: se `RtDbPGetStato == STATO_STOP`, manda `SD_inizializza(BI)` + poll `STATO != STOP` (timeout 30s). Idempotente (no-op se già FREEZE/RUN)
7. `lg_var_open` + `build_var_index` su `variabili.rtf`

### Flusso `fmi2DoStep` (sintesi)

```c
n_goup = ceil(communicationStepSize / dt_sked);
for (i = 0; i < n_goup; i++) {
    SD_goup(BI);                            /* mappa su SKDIS_GO_UP in net_sked */
    poll RtDbPGetTime fino a t_post > t_before     /* timeout 10s per step */
}
```

`canHandleVariableCommunicationStepSize=false` in modelDescription.xml. Se il
master chiede `stepSize < dt_sked`, log warning e avanziamo comunque di un
`dt_sked` intero (fmpy non gestisce `fmi2Discard` → loop infinito).

#### Relazione tra `-d` / `communicationStepSize` e `dt_sked`

`dt_sked` è il passo di integrazione fisso della simulazione LegoPST: ogni
`SD_goup` avanza il tempo simulato di esattamente `dt_sked` e non esiste
granularità inferiore. Il valore è riportato in `DefaultExperiment.stepSize`
nel `modelDescription.xml` (scritto da `gen_modeldescription` via
`RtDbPGetDt` al momento del build della FMU).

`communicationStepSize` (opzione `-d` di `run_fmu.sh`) è invece il
*passo di comunicazione* del master FMI: con quale frequenza il master chiama
`fmi2DoStep` e legge gli output. I due passi sono indipendenti ma vincolati:

**Regola**: `-d` deve essere un **multiplo intero di `dt_sked`** per avere
comportamento corretto. `n_goup = ceil(communicationStepSize / dt_sked)` è
l'arrotondamento al multiplo superiore; se il rapporto non è intero si
accumula drift tra il tempo interno della sim e quello dichiarato al master.

| `dt_sked` (da modelDescription) | `-d` passato | `n_goup` | Comportamento |
|---|---|---|---|
| 1.0 | 1.0 | 1 | ✅ naturale, 1 SD_goup per step, 1 campione/s |
| 1.0 | 2.0 | 2 | ✅ corretto, sim avanza 2s per step, meno campioni |
| 1.0 | 5.0 | 5 | ✅ corretto, 1 campione ogni 5s sim |
| 1.0 | 0.5 | 1 | ⚠️ warning: sim avanza 1s ma master pensa 0.5s → drift |
| 0.5 | 0.5 | 1 | ✅ naturale per una task con dt_sked=0.5s |
| 0.5 | 1.0 | 2 | ✅ corretto |
| 0.5 | 0.25 | 1 | ⚠️ warning: sim avanza 0.5s ma master pensa 0.25s → drift |
| 0.5 | 0.7 | 2 | ⚠️ nessun warning ma sim avanza 1.0s per step (non 0.7s) → drift |

**Drift temporale**: quando `communicationStepSize` non è multiplo esatto di
`dt_sked`, `inst->current_time` (tempo dichiarato al master) e il tempo reale
della sim divergono. Dopo K step il tempo reale è `K × n_goup × dt_sked`
mentre il master vede `K × communicationStepSize`. I valori letti sono
comunque coerenti con il tempo reale della sim, non con il timestamp fmpy.

**Caso d'uso tipico**: non specificare `-d` (oppure `-d <valore uguale a
DefaultExperiment.stepSize>`). Senza `-d`, `run_fmu.sh` passa `step_size=None`
a fmpy che usa `defaultExperiment.stepSize` dal modelDescription — cioè
esattamente `dt_sked` — ed è sempre corretto.

**Ridurre la frequenza di campionamento** (es. output ogni 5s su una task
con dt_sked=1s): usare `-d 5.0`. La sim gira comunque a passo 1s ma fmpy
legge gli output solo ogni 5 passi. Riduce dimensione del CSV e overhead del
master senza cambiare la fisica della simulazione.

**Verificare il comportamento con `LG_FMU_DEBUG=1`**: ogni `fmi2DoStep`
stampa su stderr `[lg_fmu DBG] DoStep t=%.3f h=%.3f dt_sked=%.3f n_goup=%d`.
Da lì si vede immediatamente se `n_goup > 1` o se c'è disallineamento.

### Layout di un bundle estratto

```
<fmu>/
  binaries/linux64/LegoCliSINC.so
  modelDescription.xml
  resources/
    task_info.env             # TASK_PATH, LEGOROOT, BUNDLE_MODE, N000..M005
    bundle/                   # SOLO in --bundle mode
      .profile_legoroot
      Alg_env.sh
      launch_sim.sh           # entry point: setta env + exec net_startup_headless.sh
      restore_perms.sh        # chmod +x post-fmpy.extract (zipfile non preserva mode)
      Alg_rt/bin/{dispatcher,net_sked,killsim,net_prepf22}
      Alg_rt/lg_fmu/scripts/net_startup_headless.sh
      Alg_rt/lg_fmu/tools/probe_init
      lego_big/bin/{initav,TAVOLE.DAT}
      legocad/lego_big -> ../lego_big        # symlink atteso da Alg_env.sh:118
      lib/{libgfortran.so.5,libgcc_s.so.1,libsqlite3.so.0,libz.so.1}
      task/<name>/            # copia della task (filtrata via rsync exclude)
    README.txt
```

In bundle mode `TASK_PATH=task/<name>` (relativo); `LEGOROOT` ricalcolato a
runtime come `<resources>/bundle`.

### Adattamenti runtime per container Linux (P7-bis)

Quando la FMU bundle viene caricata da un host che NON è la macchina di build,
serve adattare l'environment in modi che il flusso "host con LegoPST già
installato" non vede:

| Problema (in container) | Causa | Fix in lg_fmu |
|-------------------------|-------|---------------|
| File del bundle senza bit `+x` dopo `fmpy.extract` | Python `zipfile` non preserva i permessi unix dello zip | `bundle/restore_perms.sh` (generato da `bundle/build.sh`) chmoda i file noti; `lg_fmi2.c::launch_sim_and_wait` lo invoca via `bash` (non richiede exec bit sul file invocato) prima di `launch_sim.sh` |
| `SHR_USR_KEY=0` in container root (uid=0) → `IPC_PRIVATE`, SHM non condivisibile | `Alg_env.sh:234`: `USR_KEY=$(id -u)`, poi `SHR_USR_KEY=USR_KEY*10000` | `lg_fmi2.c::setup_legopst_env`: se `getuid()==0` fallback a `getpid()*10 + 10000`; `net_startup_headless.sh` preserva `SHR_USR_KEY` del caller dopo source profile |
| `dispatcher non in PATH` exit 2 dal launcher | `/bin/sh = dash` su debian/ubuntu, non digerisce sintassi bash di `.profile_legoroot` (`set -o emacs`, `[[ ]]`) → source silente fallisce → `PATH` non esteso | `net_startup_headless.sh` ora ha shebang `#!/usr/bin/env bash` |

### Perché serve una sim viva durante il build della FMU

Per produrre una FMU (sia base sia bundle) `bundle/build.sh` richiede che la
sim della task sia attiva. `dolgfmu.sh` se ne occupa automaticamente: rileva
con `ipcs -m` se la sim e' gia' viva (attach mode, lasciata viva dopo il
build), altrimenti la avvia con `net_startup_headless.sh + probe_init`,
builda la FMU, poi `killsim`. La domanda e': perche' non si puo' generare
la FMU "a freddo" partendo solo dai file della task?

Il `.fmu` contiene un solo file specifico alla task: `modelDescription.xml`.
Il resto del bundle (binari LegoPST, libs, runtime, `.so`) e' identico tra
task. Quindi tutta la dipendenza e' nella generazione di
`modelDescription.xml`.

[`tools/gen_modeldescription.c`](tools/gen_modeldescription.c) si attacca
alla sim viva via `RtCreateDbPunti` e usa `costruisci_var` di libsim
([AlgLib/libsim/var_sh.c:527](AlgLib/libsim/var_sh.c#L527)) che riempie
l'array `VARIABILI vars[]` partendo dalla SHM `ID_SHM_VAR` (offset 5 da
`SHR_USR_KEY`). Questa SHM viene popolata da **`lg5sk` durante `reg_prolog`
+ il primo ciclo**: prima ci sono solo dimensioni vuote, dopo c'e' la lista
completa di variabili con i loro indirizzi e i loro valori correnti.

Cosa serve per `modelDescription.xml` e da dove viene preso:

| Informazione | Sorgente | Disponibile a freddo? |
|--------------|----------|----------------------|
| Nomi qualificati `<modello>.B<blocco>.<nome>` | `costruisci_var` da SHM `ID_SHM_VAR` | ❌ richiede sim viva |
| `valueReference` = addr SHM | `vars[i].addr` post-`costruisci_var` | ❌ idem |
| `causality` (input/output/local) | `vars[i].tipo` (`LG_VAR_INPUT_NC`, `LG_VAR_OUTPUT`, ...) | ❌ idem |
| `start` value degli input | `RtDbPGetValue(addr)` live | ❌ richiede sim viva (e SHM popolata da lg5sk) |
| `dt_sked`, `timescaling` per `defaultExperiment.stepSize` | `RtDbPGetDt`, `RtDbPGetTimeScaling` su DB live | ❌ vivono solo a runtime |
| GUID univoco | `/proc/sys/kernel/random/uuid` | ✅ no dipendenza |

Alternativa teorica — parsare a freddo i file `s04`/`s05` (NOMI_MODELLI/
NOMI_BLOCCHI/VARIABILI binari) + `variabili.rtf` (causality):

- Richiederebbe un parser separato del formato binario `s04`/`s05`, che
  oggi e' incapsulato in `costruisci_var` (libsim). Il formato e' interno
  a lego_big e cambia tra release.
- Niente start value live → solo default statici, meno informativi per il
  master FMI che importa la FMU (specie con flag `provideAllStartValues`).
- Niente `dt_sked` / `timescaling` (non sono persistiti nei file di task,
  vengono dal config runtime di sked).
- Si duplicherebbe la logica gia' implementata in libsim, mantenuta
  insieme al runtime.

Scelta architetturale: piggyback su `costruisci_var` come "lettore canonico"
del layout SHM. Costo: serve sim viva durante il build (gestito in modo
trasparente da `dolgfmu.sh`).

**Sequenza temporale critica per gli `start` values degli input**:

I valori di stazionario per gli `INPUT_FREE` sono calcolati da `lg3` e
salvati come parte di `UU` in [`<task>/proc/f04.dat`](AlgLib/libsim/reg_wrshm.c).
Quando `lg5sk` parte:

1. `reg_prolog`: registra al dispatcher e crea code IPC. SHM `ID_SHM_VAR` e' creata vuota (zeri).
2. `LG5SIM` apre `f04.dat` e legge `XY/UU/XYU` in memoria locale Fortran ([lg5sim.f:124](legocad/lego_big/sorglego/sub/lg5sim.f#L124)).
3. `LGTOPS` costruisce la topologia + factorization (lavoro non banale per modelli grandi).
4. `LGDYNS` primo passo → chiama `reg_000(ipr=1)` → **`reg_wrshm` scrive XY/UU in SHM** ([reg_wrshm.c:48](AlgLib/libsim/reg_wrshm.c#L48)).
5. `lg5sk` `msg_snd` primo ack → `sked_start` torna → `sked.c:330` setta `STATO_FREEZE`.

**Il punto critico**: se `gen_modeldescription` legge la SHM **prima** del
passo 4, gli input free sono ancora a 0. Per questo `dolgfmu.sh` headless
chiama `probe_init` che ora **polla `RtDbPGetStato` fino a uscire da
`STATO_STOP`** (timeout 30s), garantendo che `reg_wrshm` sia stato chiamato
prima del build. Senza il polling (`sleep` cieco), la latenza dipende dal
modello (su `collet` ~4-5s reali) ed e' impossibile fissare un valore
sicuro a priori.

### Ciclo di vita di `f22circ.dat` e `backtrack.dat` (FMU headless)

`f22circ.dat` e `backtrack.dat` sono file di stato che il runtime LegoPST
scrive nella directory della task. La FMU headless li tratta diversamente
dal banco operatore interattivo.

**Cosa contengono**

| File | Scritto da | Contenuto |
|------|-----------|-----------|
| `f22circ.dat` | `net_prepf22` (processo figlio di net_sked) | Buffer circolare dei campioni di simulazione: header + N campioni × M variabili grafiche. Usato da `run_graphics.sh` per visualizzare i risultati post-run |
| `backtrack.dat` | `net_sked` / `dispatcher` (snapshot periodici) | Stato completo della simulazione a istanti passati, usato per la funzione "torna indietro nel tempo" |

**Quando vengono cancellati — operazione distruttiva**

`net_startup_headless.sh` cancella **entrambi** subito dopo `killsim`, prima
di avviare il nuovo `dispatcher` + `net_sked`:

```bash
killsim 2>/dev/null || true
rm -f f22circ.dat backtrack.dat 2>/dev/null || true   # ← QUI
nohup dispatcher ... &
nohup net_sked 1 ... &
```

Questo avviene **all'inizio di ogni nuovo lancio FMU** (ogni chiamata a
`fmi2Instantiate` in launch mode che avvia la sim). I dati del run
precedente sono irrecuperabili dopo questo punto.

**Perché è necessario**

Il `dispatcher` è compilato con il flag `DF22_APPEND` (vedi `dispatcher.c`):
apre `f22circ.dat` in append mode (`O_WRONLY | O_CREAT | O_APPEND`) anziché
troncare. Se il file esiste da un run precedente, `net_sked` al riavvio
legge l'header e trova `p_fine=N` (campioni del run precedente) → la sim
entra in uno stato "restored" incompatibile con `SD_goup` headless → il
secondo `fmi2DoStep` va in timeout e ritorna status 3 (fmi2Error).

**Conseguenze pratiche per l'utente**

> ⚠️ Se si esegue la FMU bundle più volte sulla stessa directory di
> estrazione (`unzipdir`), il `f22circ.dat` del run precedente viene
> **cancellato prima del nuovo run**. I grafici di quel run non sono
> più visualizzabili.

Se si vogliono conservare i dati grafici di un run, copiare `f22circ.dat`
**prima** di eseguire nuovamente la FMU:

```bash
# Dopo il primo run — prima del secondo
cp legoclix_<task>_bundle/resources/bundle/task/<task>/f22circ.dat \
   /tmp/f22circ_run1.dat
run_fmu -b legoclix_<task>_bundle.fmu --stop-time 30  # run 2 → cancella e ricrea
./run_graphics.sh /tmp/f22circ_run1.dat               # grafici del run 1
```

Questo non si applica alla **FMU base** (non bundle): in quel caso la task
vive nella sua directory originale (`/home/antonio/legocad/<task>`), non
nella dir di estrazione fmpy, e il `f22circ.dat` è il file di task normale
(quello che il banco operatore usa già).

**Comportamento di fmpy.extract (ulteriore dettaglio)**

`fmpy.extract(fmu_path, unzipdir=...)` usa `zipfile.ZipFile.extractall()`
che **non cancella** la directory di destinazione prima di estrarre. File
generati a runtime (`f22circ.dat`, `backtrack.dat`, `*.fmu.log`) sopravvivono
tra run successivi sulla stessa `unzipdir`. Il `rm` in `net_startup_headless.sh`
è il punto di pulizia designato; non esiste un altro meccanismo di reset.

### Diagnostica

Variabile env `LG_FMU_DEBUG=1` (si setta nel processo che carica la `.so`):
attiva `[lg_fmu DBG] ...` su stderr in:
- `attach_or_launch`: res_dir, bundle_mode, legoroot, task_path, env post-setup, errno chdir, stato pre-init, esito SD_inizializza
- `launch_sim_and_wait`: cmd system, rc, polling try_attach
- `fmi2Instantiate`: ptr lg_var_open, rc build_var_index
- `[lg_fmu LAUNCH]`: stdout/stderr di `launch_sim.sh` (NON ridiretto a /dev/null in debug)

Tools standalone (richiedono sim attiva sulla task corrente):
- `probe_init` — `SD_inizializza(BI)`. Sblocca sim ferma in `STATO_STOP` post-headless
- `probe_step --goup N` — N×`SD_goup(BI)` (avanza N×`dt_sked`)
- `probe_step DT` — wallclock RUN/FREEZE per DT secondi
- `probe_attach` — solo attach + read smoke test

### Troubleshooting comune

| Sintomo | Causa | Fix |
|---------|-------|-----|
| `fmi2Instantiate` ritorna NULL | `task_info.env` non leggibile / TASK_PATH inesistente | Ricostruisci la FMU sulla task corretta |
| **`fmi2DoStep` status 3 (error)** + warning **"parametri del LEGO non sono definiti"** | `N000..N007/M001..M005` mancanti nell'env del processo host | Già fixato in P7.5. Verifica che `task_info.env` contenga le righe `N000=...` ecc. (richiede `.profile_legoroot` sourceato al build) |
| **`fmi2DoStep` status 3** + log "timeout aspettando avanzamento tempo" | Sim in `STATO_STOP` (manca `SD_inizializza`) | Già fixato in P7.5. Per debug: `probe_step --goup 1` mostra `ERRORE: simulazione in stato STOP`; `probe_init` la sblocca |
| `chdir(...) fallito` in DBG | TASK_PATH errato; in bundle, dir non estratta correttamente | Cancella e re-estrai il bundle (`rm -rf <fmu_dir>/<task>_bundle/` poi `fmpy.extract`) |
| `lg5sk` SIGFPE in `trova_(tavole/trova.c:77)` | `SHR_TAV_KEY=999` cancellata da `killsim`, non ripopolata | `net_startup_headless.sh` chiama `initav` post-killsim (idempotente). Se ancora rotto, controlla che `lego_big/bin/{initav,TAVOLE.DAT}` siano presenti |
| `to_dispatcher` + `msg_ack.ret=1` ma tempo non avanza | Sim attiva ma in stato sporco da test precedente | `killsim` + `net_startup_headless.sh` |
| **`fmi2DoStep` status 3** al secondo run, `net_sked.fmu.log` mostra `f22_leggo_header header: p_fine=N` con N>0 | `f22circ.dat` del run precedente sopravvive a `fmpy.extract` (che non cancella la dir); il dispatcher lo riapre in append mode e net_sked lo legge come stato restored → `SD_goup` non avanza il tempo | Fixato 2026-05-06: `net_startup_headless.sh` esegue `rm -f f22circ.dat backtrack.dat` dopo `killsim`. Se ancora presente: cancella manualmente `<unzipdir>/resources/bundle/task/<task>/f22circ.dat` e `backtrack.dat`. Vedi sezione "Ciclo di vita di `f22circ.dat`" |
| Bundle gira con env normale ma fallisce con `env -i` | Eredità di env vars LegoPST dalla shell del builder | Verifica con `unzip -p <fmu> resources/task_info.env` che le 12 N/M siano presenti |
| Container: `[lg_fmu LAUNCH] sh: ... Permission denied` | `fmpy.extract` non preserva il bit `+x` | Fixato in P7-bis: `restore_perms.sh` invocato pre-launch. Verifica che il bundle contenga `resources/bundle/restore_perms.sh` |
| Container root: polling `try_attach_db` infinito, sim non parte | `SHR_USR_KEY=0` (uid root) = `IPC_PRIVATE` | Fixato in P7-bis: fallback a `pid*10+10000`. Verifica con `LG_FMU_DEBUG=1` la riga `SHR_USR_KEY=...` post-setup |
| Container debian/ubuntu: launcher exit 2 "dispatcher non in PATH" | `/bin/sh = dash`, source `.profile_legoroot` (bash) fallisce silenziosamente | Fixato in P7-bis: shebang `bash` in `net_startup_headless.sh`. Bash deve essere disponibile nel target (lo è in tutte le distro Linux mainstream) |
| Glibc del target < 2.38 → `version 'GLIBC_2.38' not found` | Bundle compilato su host con glibc recente; `net_sked` link a 2.38 | Rebuild dei binari LegoPST in container con glibc baseline più bassa (es. ubuntu:20.04) |
| `[lg_fmu] tutti gli 8 slot SHR_USR_KEY per uid=N occupati` | 8 FMU concorrenti già attive sullo stesso uid | Rilascia istanze precedenti (`killsim` da una shell con `SHR_USR_KEY=<slot>` settata) o attendi la chiusura. `ipcs -m` mostra le chiavi occupate |
| In `lg_cosim.py`: la prima FMU scompare durante l'init della seconda (timeout o `STATO_STOP`) | `killsim` nella seconda FMU ha distrutto le SHM della prima; oppure `LG_COSIM_NO_KILLSIM` non visibile al launcher C | Usare `lg_cosim.py` ≥ 2026-05-11 (setta `LG_COSIM_NO_KILLSIM` via `ctypes.setenv` + `_patch_killsim_guard` su bundle estratti) |
| `TIMEOUT init: stato=0`, `out/lg5c.out` arriva a `reg_prolog - uscita` ma `lg5.out` resta a `FILE06= lg5.out`, `net_sked.fmu.log` con `SIGCHILD_HANDLER: CLD_DUMPED ... process child pid = <pid>` seguito da `restart_task: creazione path per files .out: File exists` in loop | `proc/lg5sk` della task source obsoleto/inconsistente con i N/M del runtime: lg5sk segfaulta dopo `reg_prolog`, net_sked entra in `restart_task` infinito | Ricompila `lg5sk` per la task (in legopc: `Make` sulla task, oppure manualmente `make -f Makefile.mk` in `<task>/`) e rigenera il bundle (`dolgfmu.sh -b <task>`) |
| `TIMEOUT init: stato=0` (lg5sk vivo, niente CLD_DUMPED), `lg5sk` e `net_sked` entrambi in `do_msgrcv` (deadlock visibile via `cat /proc/<pid>/wchan`) | `net_prepf22` mancante dal bundle: `sked_start` (con `net_sked 2` = MASTER+demone) lo spawna a [sked_start.c:1039](Alg_rt/net_simula/net_sked/sked_start.c#L1039) e attende il suo ack con timeout `TIMEOUT_AUS*10 = 1350s`. La FMU polla solo 30s → TIMEOUT prima dell'ack di prep_f22, sim mai a STATO_FREEZE | Fixato 2026-05-04: `bundle/build.sh` include `net_prepf22` nel bundle, `net_startup_headless.sh` usa `net_sked 1` (MASTER senza demone_attivo, evita anche lo spawn di `demone_mmi` che richiederebbe Alg_mmi nel bundle). `net_prepf22` resta indispensabile perché serve al salvataggio risultati in `f22circ.dat` |
| `ValueError: relative path can't be expressed as a file URI` da `fmpy.fmi2.instantiate` | Python ≥ 3.13: `pathlib.Path(...).as_uri()` rifiuta path relativi | Passa un path assoluto a `simulate_fmu` / `extract`: `unz = os.path.abspath('legoclix_<task>_bundle')` |
| `modelDescription.xml` con tutti gli input `<Real start="0"/>` (anche per pressioni/entalpie/lift che in stazionario non sono zero) | Build fatto con `dolgfmu.sh` headless prima che `reg_wrshm` (in `LGDYNS::reg_000`) scrivesse `UU` da F04 in SHM. La sim era ancora in `STATO_STOP` quando `gen_modeldescription` ha letto i valori | Fixato 2026-05-04: `probe_init` ora polla `RtDbPGetStato` fino a uscire da `STATO_STOP` (timeout 30s) prima di ritornare. `dolgfmu.sh` headless rigenera bundle con start corretti. Workaround se non si puo' rebuildare la FMU: usare modalita' attach (banco vivo + `dolgfmu -b` mentre il banco e' attivo) |
| `OSError: Error reading file 'modelDescription.xml': Invalid bytes in character encoding` da `lxml`/`fmpy.read_model_description` | Le `description` LegoPST sono Latin-1/CP1252 (es. `m³`/`m²` con `³`=`0xB3`, `²`=`0xB2` da soli) e finivano grezze nell'XML che dichiara `encoding="UTF-8"` | Fixato 2026-05-04: `gen_modeldescription.c::xml_escape` ora converte byte ≥ 0x80 nella sequenza UTF-8 a 2 byte (`0xC0|h>>6`, `0x80|h&0x3F`). Rebuild gen_modeldescription + ricostruisci la FMU. Validazione: `iconv -f UTF-8 -t UTF-8 modelDescription.xml > /dev/null` deve passare |
| `ERRORE: SHM <id> esiste ed ha dim X superiori a Y` + segfault (rc=139) durante `simulate_fmu` | `run_fmu.sh` source `.profile_legoroot` che setta `SHR_USR_KEY=uid*10000` (slot 0). Senza un banco operatore vivo a quello slot, la FMU lancia la sim su slot 0 e collide con residui SHM di altre task lasciati da run precedenti (es. SHM dimensionata per N variabili diverse) | Fixato 2026-05-04: `run_fmu.sh` rileva via `ipcs -m` se c'e' una sim viva a slot 0 (chiave `uid*10000+5` = ID_SHM_VAR). Se sì, mantiene `SHR_USR_KEY` per attach mode. Se no, fa `unset SHR_USR_KEY/SHR_USR_KEYS` e lascia che `lg_fmi2.c::setup_legopst_env` scelga slot 1..8 libero (logica P5) |

### Procedura di test — multi-istanza FMU (P5)

Valida che la FMU in launch mode scelga uno slot `SHR_USR_KEY` libero (1..8) senza collidere con la sessione LegoPST normale (slot 0) né con altre FMU concorrenti dello stesso uid. Riferimento codice: `setup_legopst_env` in `src/lg_fmi2.c`.

Per uid=1000 (utente `antonio`) gli slot attesi sono:
- slot 0 = `10000000` → riservato al banco operatore (mai scelto dalla FMU)
- slot 1 = `10001100`, slot 2 = `10002200`, ..., slot 8 = `10008800`

#### Pre-requisito — rigenerare il bundle con la `.so` post-P5

Il `.fmu` deve includere la nuova `LegoCliSINC.so`. Se il bundle è stato generato prima della patch, rigenerare:
```bash
source $LEGOROOT/.profile_legoroot
$LEGOROOT/Alg_rt/lg_fmu/scripts/dolgfmu.sh -b /home/antonio/legocad/collet
stat -c '%y %n' /home/antonio/legocad/collet/legoclix_collet_bundle.fmu \
                $LEGOROOT/Alg_rt/lg_fmu/src/LegoCliSINC.so
# .fmu deve essere PIU' RECENTE del .so
```

Pulizia IPC residui prima di iniziare:
```bash
killsim 2>/dev/null; ipcs -m
# atteso: solo SHR_TAV_KEY=0x000003e7 (acqua/vapore)
```

#### T1 — singola istanza, verifica che NON usi slot 0

```bash
cd /home/antonio/legocad/collet
env -i HOME=$HOME USER=$USER PATH=/usr/bin LG_FMU_DEBUG=1 \
  /home/antonio/fmpy_venv/bin/python3 -c "
import os
from fmpy import simulate_fmu, extract
unz = os.path.abspath('legoclix_collet_bundle')
extract('legoclix_collet_bundle.fmu', unzipdir=unz)
result = simulate_fmu(unz, stop_time=5)
print(f'OK {len(result)} sample')" 2>&1 | grep -E "SHR_USR_KEY|sample"
```

`os.path.abspath` è necessario su Python ≥ 3.13: `pathlib.Path(...).as_uri()` (chiamata da `fmpy.fmi2.instantiate`) rifiuta path relativi con `ValueError: relative path can't be expressed as a file URI`.

Atteso:
```
[lg_fmu DBG] post setup_legopst_env: SHR_USR_KEY=10001100 ...
OK 6 sample
```
Se `SHR_USR_KEY=10000000`, il `.so` nel bundle non è quello post-P5 (vedi pre-requisito).

#### T2 — due istanze concorrenti, verifica slot diversi

In due terminali separati lanciare lo stesso comando di T1. Da un terzo terminale:
```bash
ipcs -m | awk 'NR>3 && $1 ~ /^0x/ { printf "%s = %d\n", $1, strtonum($1) }'
```

Atteso: due gruppi di SHM su slot 1 e slot 2:
```
0x00989acc = 10001100   ← FMU 1, ID_SHM_SIM
0x00989ad1 = 10001105   ← FMU 1, ID_SHM_VAR
...
0x00989f18 = 10002200   ← FMU 2, ID_SHM_SIM
0x00989f1d = 10002205   ← FMU 2, ID_SHM_VAR
...
```
più `0x000003e7 = 999` (SHR_TAV_KEY condivisa). Nessuna collisione tra le 2 FMU.

#### T3 (opzionale) — coesistenza FMU + banco operatore

Aprire il banco normale su `collet` (`net_startup` o tix "Run"), poi lanciare una FMU come T1. Verificare che il banco occupi slot 0 (`10000000`) e la FMU slot 1 (`10001100`). Le SHMs in `ipcs -m` sono due insiemi disgiunti.

#### T4 (stress, opzionale) — 9ª istanza fallisce in modo pulito

Pre-requisito: T1 già eseguito una volta nella cwd, così `legoclix_collet_bundle/` è già estratta.

```bash
cd /home/antonio/legocad/collet
for i in 1 2 3 4 5 6 7 8; do
  (env -i HOME=$HOME USER=$USER PATH=/usr/bin \
    /home/antonio/fmpy_venv/bin/python3 -c "
import os
from fmpy import simulate_fmu
unz = os.path.abspath('legoclix_collet_bundle')
simulate_fmu(unz, stop_time=20)" &)
  sleep 1
done
sleep 5
env -i HOME=$HOME USER=$USER PATH=/usr/bin LG_FMU_DEBUG=1 \
  /home/antonio/fmpy_venv/bin/python3 -c "
import os
from fmpy import simulate_fmu
unz = os.path.abspath('legoclix_collet_bundle')
simulate_fmu(unz, stop_time=5)" 2>&1 | tail -5
```

Atteso (sulla 9ª): messaggio `[lg_fmu] tutti gli 8 slot SHR_USR_KEY per uid=1000 sono occupati: rilascia istanze precedenti (killsim) o attendi.` + fallimento `fmi2Instantiate`.

#### Cleanup finale

```bash
killsim
ipcs -m   # atteso: solo SHR_TAV_KEY=0x000003e7
```

### File modificati (stato 2026-05-04)

Vedi `~/.claude/projects/-home-antonio-LegoPST/memory/project_lg_fmu_linux.md`
per la lista completa per fase (P4 → P7-bis → P5). Il bundle FMU funziona
end-to-end native + container, T1 multi-istanza PASS dal 2026-05-04 (fix
`net_prepf22` mancante + `net_sked 1` + `try_attach_db("TEST")`).

### Architettura di riferimento

- Dispatcher path è quello usato da `net_operator`. Sender = `BI` (= 1, "banco istruttore")
- ValueReference FMI = SHM `addr` (1:1 con cella di memoria, stabile nel tempo)
- Una cella SHM può apparire in più entry di `VARIABILI[]` (alias): la FMU espone una `ScalarVariable` per `addr` unico (`lg_var_unique_*`)
- `dt_sked` letto live via `RtDbPGetDt(db, mod=0, &dt)` durante `fmi2Instantiate`
- Cleanup in `fmi2FreeInstance` ([lg_fmi2.c:707](Alg_rt/lg_fmu/src/lg_fmi2.c#L707)):
  - **launch mode** (`we_started_sim==1`): `system("killsim")` distrugge SHM/code/proc; **NO** `RtDestroyDbPunti` perché le SHM sono già sparite (eviterebbe `CloseDbPunti->elimina_shrmem` su puntatori invalidi e il `[error shared-memory not attached]` cosmetico)
  - **attach mode** (`we_started_sim==0`): solo `RtDestroyDbPunti` (sgancio pulito dell'handle, la sim utente continua a girare); niente `killsim`
