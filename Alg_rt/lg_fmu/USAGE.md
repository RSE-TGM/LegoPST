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
| `fmpy.simulate_fmu` (Python diretto) | base, bundle | `pip install fmpy` (Python ≥ 3.8) | scripting, integrazione test, debug fine-grained con `LG_FMU_DEBUG=1` | ✅ supportato |
| Container Linux pulito (`docker run python:3.11-slim` + fmpy) | **solo bundle** | `pip install fmpy` nel container | deployment, CI esterna, demo | ✅ validato 2026-05-02 |
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

Dalla UI tix_new (consigliato): menu **Tools → Build FMU**. Apre un xterm con
l'output di `dolgfmu.sh` sulla task corrente (`$DIRMODEL`).

Da CLI:
```bash
source $LEGOROOT/.profile_legoroot
$LEGOROOT/Alg_rt/lg_fmu/scripts/dolgfmu.sh /home/antonio/legocad/collet
# Output: /home/antonio/legocad/collet/legoclix_collet.fmu
```

`dolgfmu.sh` rileva se la sim sulla task è già viva (attach) o no
(headless launch + probe_init + build + killsim finale).

### Generare una FMU (variante bundle)

Dalla UI tix_new (consigliato): menu **Tools → Build FMU (bundle)**.

Da CLI tramite `dolgfmu.sh -b`:
```bash
source $LEGOROOT/.profile_legoroot
$LEGOROOT/Alg_rt/lg_fmu/scripts/dolgfmu.sh -b /home/antonio/legocad/collet
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
$LEGOROOT/Alg_rt/lg_fmu/scripts/run_fmu.sh /home/antonio/legocad/collet/legoclix_collet.fmu --stop-time 30
# CSV in /home/antonio/legocad/collet/results.csv
```

Subcommand:
- (default): simulate → CSV
- `--info`: lista variabili (no sim)
- `--validate`: passa attraverso `fmpy.validation`

Opzioni:
- `--stop-time T` (default 30 s)
- `--step-size DT` (default da modelDescription.xml)
- `--set VAR=VAL` (ripetibile, override input)
- `--csv FILE` (default `<fmu_dir>/results.csv`)

### Eseguire la FMU bundle in env pulito (verifica self-containment)

```bash
cd /home/antonio/legocad/collet
env -i HOME=$HOME USER=$USER PATH=/usr/bin:/usr/local/bin \
  /home/antonio/fmpy_venv/bin/python3 -c "
from fmpy import simulate_fmu, extract
fmu = '/home/antonio/legocad/collet/legoclix_collet_bundle.fmu'
unzip = '/home/antonio/legocad/collet/legoclix_collet_bundle'
extract(fmu, unzipdir=unzip)
result = simulate_fmu(unzip, stop_time=10)
print(f'{len(result)} sample. Ultimo: {result[-1]}')
"
```

Note:
- `env -i` rimuove tutto l'environment ereditato → testa che il bundle sia davvero self-contained.
- `unzipdir` accanto al `.fmu` (non `/tmp/fmpy_*`) per coerenza con la regola di output path.
- `LG_FMU_DEBUG=1` (env var aggiuntiva) attiva il dump diagnostico `[lg_fmu DBG] ...` su stderr.

### Eseguire la FMU bundle in container Linux pulito (deployment)

Validato su `python:3.11-slim` (debian-trixie, glibc 2.41, no LegoPST installato):

```bash
docker run --rm \
  -v /home/antonio/legocad/collet/legoclix_collet_bundle.fmu:/tmp/bundle.fmu:ro \
  python:3.11-slim bash -c '
pip install --quiet fmpy
python -c "
from fmpy import simulate_fmu, extract
extract(\"/tmp/bundle.fmu\", unzipdir=\"/tmp/b\")
result = simulate_fmu(\"/tmp/b\", stop_time=10)
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
- Multi-istanza (uid≠0, sim non viva): la FMU sceglie automaticamente uno degli 8 slot `uid*10000 + slot*1100` (slot 1..8) disponibili. Lo slot 0 (= `uid*10000`) è riservato alla sessione LegoPST normale dell'utente: aprendo il banco operatore mentre una FMU gira, le SHM non collidono. Limite: max 8 FMU concorrenti per uid.
- `net_startup_headless.sh` ha shebang `bash` (non `sh`): in debian/ubuntu `/bin/sh = dash` non digerisce i costrutti bash di `.profile_legoroot` (`set -o emacs`, `[[ ]]`).

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
  dolgfmu.sh                # entry point UI: build FMU base sulla task
  run_fmu.sh                # wrapper fmpy per eseguire una FMU
  net_startup_headless.sh   # dispatcher + net_sked headless (no banco, no X)
tests/
  test_var_mapping          # test offline su lg_var_mapping
USAGE.md                    # questo file
```

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
5. `try_attach_db` (RtCreateDbPunti). Se OK → attach mode (`we_started_sim=0`). Se NO → `launch_sim_and_wait` (`we_started_sim=1`)
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
      Alg_rt/bin/{dispatcher,net_sked,killsim}
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
| Bundle gira con env normale ma fallisce con `env -i` | Eredità di env vars LegoPST dalla shell del builder | Verifica con `unzip -p <fmu> resources/task_info.env` che le 12 N/M siano presenti |
| Container: `[lg_fmu LAUNCH] sh: ... Permission denied` | `fmpy.extract` non preserva il bit `+x` | Fixato in P7-bis: `restore_perms.sh` invocato pre-launch. Verifica che il bundle contenga `resources/bundle/restore_perms.sh` |
| Container root: polling `try_attach_db` infinito, sim non parte | `SHR_USR_KEY=0` (uid root) = `IPC_PRIVATE` | Fixato in P7-bis: fallback a `pid*10+10000`. Verifica con `LG_FMU_DEBUG=1` la riga `SHR_USR_KEY=...` post-setup |
| Container debian/ubuntu: launcher exit 2 "dispatcher non in PATH" | `/bin/sh = dash`, source `.profile_legoroot` (bash) fallisce silenziosamente | Fixato in P7-bis: shebang `bash` in `net_startup_headless.sh`. Bash deve essere disponibile nel target (lo è in tutte le distro Linux mainstream) |
| Glibc del target < 2.38 → `version 'GLIBC_2.38' not found` | Bundle compilato su host con glibc recente; `net_sked` link a 2.38 | Rebuild dei binari LegoPST in container con glibc baseline più bassa (es. ubuntu:20.04) |
| `[lg_fmu] tutti gli 8 slot SHR_USR_KEY per uid=N occupati` | 8 FMU concorrenti già attive sullo stesso uid | Rilascia istanze precedenti (`killsim` da una shell con `SHR_USR_KEY=<slot>` settata) o attendi la chiusura. `ipcs -m` mostra le chiavi occupate |
| `TIMEOUT init: stato=0`, `out/lg5c.out` arriva a `reg_prolog - uscita` ma `lg5.out` resta a `FILE06= lg5.out`, in `net_sked.fmu.log` `SIGCHILD_HANDLER: CLD_DUMPED ... process child pid = <pid>` seguito da `restart_task: creazione path per files .out: File exists` in loop | `proc/lg5sk` della task source obsoleto/inconsistente con i N/M del runtime: lg5sk segfaulta dopo `reg_prolog`, net_sked entra in `restart_task` infinito, STATO_STOP permane | Ricompila `lg5sk` per la task (in legopc: `Make` sulla task, oppure manualmente `make -f Makefile.mk` in `<task>/`) e rigenera il bundle (`dolgfmu.sh -b <task>`) |
| `ValueError: relative path can't be expressed as a file URI` da `fmpy.fmi2.instantiate` | Python ≥ 3.13: `pathlib.Path(...).as_uri()` rifiuta path relativi | Passa un path assoluto a `simulate_fmu` / `extract`: `unz = os.path.abspath('legoclix_<task>_bundle')` |

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

Aprire il banco normale su `collet` (`net_startup` o tix_new "Run"), poi lanciare una FMU come T1. Verificare che il banco occupi slot 0 (`10000000`) e la FMU slot 1 (`10001100`). Le SHMs in `ipcs -m` sono due insiemi disgiunti.

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

### File modificati (stato 2026-05-02)

Vedi `~/.claude/projects/-home-antonio-LegoPST/memory/project_lg_fmu_linux.md`
per la lista completa per fase (P4 → P7-bis). P7-bis (container support)
committato 2026-05-02.

### Architettura di riferimento

- Dispatcher path è quello usato da `net_operator`. Sender = `BI` (= 1, "banco istruttore")
- ValueReference FMI = SHM `addr` (1:1 con cella di memoria, stabile nel tempo)
- Una cella SHM può apparire in più entry di `VARIABILI[]` (alias): la FMU espone una `ScalarVariable` per `addr` unico (`lg_var_unique_*`)
- `dt_sked` letto live via `RtDbPGetDt(db, mod=0, &dt)` durante `fmi2Instantiate`
- Cleanup in `fmi2FreeInstance`: `system("killsim")` solo se `we_started_sim==1`
