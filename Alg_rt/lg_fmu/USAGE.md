# LegoCliSINC FMU — guida utente e developer

FMU FMI 2.0 Co-Simulation per il runtime di simulazione LegoPST su Linux.

Esistono **due varianti** della stessa FMU, con stesso nome modello (`LegoCliSINC`)
e stessa interfaccia FMI ma diversa modalità di deployment:

| Variante | File | Self-contained | Richiede LegoPST installato sul target | Dimensione tipica |
|----------|------|----------------|----------------------------------------|-------------------|
| **base**   | `legoclix_<task>.fmu`         | no  | sì | ~80 KB |
| **bundle** | `legoclix_<task>_bundle.fmu`  | sì  | no | ~12 MB |

Entrambe sono "bound al task": una FMU vale per la singola task LegoPST sulla
quale è stata generata (`TASK_PATH` embedded in `resources/task_info.env`).
Per ogni task serve quindi un build dedicato.

Output di entrambe le varianti: file `.fmu` nella **directory della task**
(es. `/home/antonio/legocad/collet/`), non in `/tmp`. Anche la dir di unzip
del bundle (`legoclix_<task>_bundle/`) deve stare accanto alla `.fmu`.

---

## Sezione utente

### Quando usare quale variante

- **base**: la macchina su cui giri la FMU ha LegoPST installato (stesso utente, stesso `LEGOROOT`). Tipicamente: dev workstation. Più piccola e veloce da rigenerare.
- **bundle**: la macchina target NON ha LegoPST. Tipicamente: container / collega / shipping. Include tutto il runtime serve: `dispatcher`, `net_sked`, `killsim`, `initav`, tabelle acqua/vapore, shared libs, e una copia della task stessa.

In entrambi i casi la FMU è **Linux x86_64**. Per Simulink Windows serve la
versione Windows separata (`/home/antonio/legopc_prj/src/legosim/LegoCliSINC_fmu`).

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

Manualmente, con sim attiva sulla task:
```bash
source $LEGOROOT/.profile_legoroot
cd /home/antonio/legocad/collet
# se la sim non è attiva, lanciala prima:
$LEGOROOT/Alg_rt/lg_fmu/scripts/net_startup_headless.sh /home/antonio/legocad/collet

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
3. `setup_legopst_env`: `SHR_USR_KEY` (= `getuid()*10000`), `SHR_USR_KEYS`, `OS=Linux`, `PATH`, `LD_LIBRARY_PATH` (bundle), `SHR_TAV_KEY=999` (bundle)
4. `chdir(TASK_PATH)`
5. `try_attach_db` (RtCreateDbPunti). Se OK → attach mode (`we_started_sim=0`). Se NO → `launch_sim_and_wait` (`we_started_sim=1`)
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

### File modificati (stato 2026-05-01, NON committati)

Vedi `~/.claude/projects/-home-antonio-LegoPST/memory/project_lg_fmu_linux.md`
per la lista completa per fase (P4 → P7.5).

### Architettura di riferimento

- Dispatcher path è quello usato da `net_operator`. Sender = `BI` (= 1, "banco istruttore")
- ValueReference FMI = SHM `addr` (1:1 con cella di memoria, stabile nel tempo)
- Una cella SHM può apparire in più entry di `VARIABILI[]` (alias): la FMU espone una `ScalarVariable` per `addr` unico (`lg_var_unique_*`)
- `dt_sked` letto live via `RtDbPGetDt(db, mod=0, &dt)` durante `fmi2Instantiate`
- Cleanup in `fmi2FreeInstance`: `system("killsim")` solo se `we_started_sim==1`
