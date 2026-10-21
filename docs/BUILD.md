# Build System and Commands

## Main Build Command
```bash
make -f Makefile.mk
```

This builds all subprojects in sequence:
- kprocedure utilities
- kutil tools
- Alg_mmi (MMI components and AlgLib)
- Alg_rt (runtime components)
- legocad (CAD tools with lego_big)
- util97 and util2007 utilities
- docker launchers (`lgdock`, `lgdock_socat`, `lgdock_multi`) and `repo_info.conf`

It does **not** build the Docker image, so it runs on a machine without Docker.

## Docker Image
One image, `aguagliardi/legopst:2.0` (~2.3 GB on disk):
```bash
make -f Makefile.mk docker        # builds it
make -f Makefile.mk docker-push   # builds it and pushes it to the registry
```

Both run `all` and then `docker/BuildImage -y`, which needs Docker installed and running. Rebuild the
image whenever something that ends up inside it changes - the repository is copied in,
the demo tarball included. Until 2026-09-27 there were two images, `legopst_multi` and
`legopst_slim`; they differed only in packages nothing requires, so they were unified on
the leaner recipe. What the image deliberately leaves out, and why, is in
[docker/README_INSTALLER.md](../docker/README_INSTALLER.md).

**The image is built from the working copy on disk, not from git.**
`BuildImage` runs `docker build` on the repository directory and compiles
nothing: whatever is on disk ends up in the image, compiled files included.

For this reason, since 2026-10-16 **`docker` and `docker-push` first run
`all`**. Then:
- after a `clean`, the image no longer lacks the nine objects that LegoPC links
  into the tasks, nor all the libraries (`libsim.a`, `legolib.a`, `vapo.a`...);
- `version.h` is always current;
- if the build fails, no image is built.

When nothing has changed, `all` just walks the directories and recompiles
nothing; after a `clean` it takes a couple of minutes. So the whole sequence
is one command:

```bash
source .profile_legoroot          # required: the targets check it
make -f Makefile.mk docker-push
```

**Things to know:**
- **The profile.** Both targets refuse to start if the profile is not loaded,
  or if `LEGOROOT` points to another copy of the tree. Without the profile,
  `all` would fail with unhelpful `-I -I` errors.
- **Uncommitted files.** If `all` recompiles something, the executables on disk
  change and need committing. The image contains them anyway.
- **Without recompiling.** For an image of the current state on disk, run
  `cd docker && ./BuildImage -y [--push]` by hand. Then you must make sure the
  tree is built; never do it right after a `clean`.

Checked on 2026-10-16: a tree with no `.o` and no `.a` at all, built from
scratch in the container, came out with every library and object that git
tracks, and no errors. Before that date only the `.o` had been tested this
way (the CI deletes `.o` files, not `.a` ones).

## Clean Build
```bash
make -f Makefile.mk clean
```
Removes all object files (*.o), libraries (*.a), and generated version.h - including
the ones committed for users that do not compile: see
[Compiled files in git](#compiled-files-in-git). Follow it with
`make -f Makefile.mk` before committing. The Docker targets rebuild by
themselves: see [Docker Image](#docker-image).

## Build check (CI)

Every push to `master` and every pull request is compiled **from scratch**,
and then tested (see *Functional tests* below), by GitHub Actions ([.github/workflows/build.yml](../.github/workflows/build.yml)),
inside the `aguagliardi/legopst:2.0` image, which already has the compilers
and the development headers. The check fails if:

- the build from the root ends with errors;
- the warnings are **more** than the number in
  [.github/build_warnings_baseline](../.github/build_warnings_baseline). The
  number may only go down: when it does, the script prints the new value to
  write there.

"From scratch" means without any `.o` file: on a working copy that has been
built they are all there, and `make` would recompile nothing. The same script runs by hand,
**on a copy** of the tree, since it deletes the `.o` files:

```bash
docker run --rm -v <copy>:/src -w /src aguagliardi/legopst:2.0 \
    bash .github/scripts/build_check.sh
```

### Functional tests

After a successful build, in the same container,
[.github/scripts/smoke_test.sh](../.github/scripts/smoke_test.sh) checks that
the freshly built programs **work**, on a real demo and without a display:

| Test | What it does |
|---|---|
| `lgdemo` | downloads and installs the `nuclear` demo from the release (sha256 checked). It is the only published demo with the tasks' `proc/`, so it can be simulated without legopc |
| `lgmkstaz` | round-trip of the faceplate parser/writer (`lgmkstaz_test.tcl`) on the catalogue and on the demo's `r01.dat` |
| `kUpSim` | `kUpSim -nommi` on the `SLaurent_0` simulator (no MMI pages: they need a display) |
| simulation | `net_startup_headless.sh` on the `PWRN1PSS` task, `probe_init`, then 10 steps with `probe_step --goup`: simulated time must advance by 10 s |
| variables | `test_var_mapping` on the live simulation |
| topology | the task's `snapshot.dat`, with its header altered to look like another model's, must be set aside as `snapshot.dat.incompatibile` at the next start, and the simulation must start anyway (`ControlParam`, see [new_monit/README.md](../Alg_rt/net_simula/new_monit/README.md)) |
| task | the demo's tasks are rebuilt with LegoPC's commands (`cad_crealg1`, `cad_crealg3`, `crealg5`, `cad_crealg5sk`, `cad_marcotask`), and every installation `.o`/`.a` they link must be tracked by git (see [Compiled files in git](#compiled-files-in-git)) |
| lgconn | `lgconn_test.tcl`: a synthetic task with one case for each check of `lgconn -c`, and the `Connessioni.reg` rows of the demo's regulation task rebuilt from its pages, which must match what `config` wrote; then `lgconn -c` on that task must produce its report (its findings are not judged: the demo has real ones) |

Everything happens in a temporary `HOME`. Every step has a time limit: a
simulation that does not answer must not hold the runner (in testing, with
`net_sked` not starting, `probe_init` waited forever). The whole job has a
45-minute ceiling. The whole run (build and tests) takes about a minute and a
half; the logs of both are in the run's `build-log` artifact.

By hand, on an already built **copy**:

```bash
docker run --rm -v <copy>:/src -w /src aguagliardi/legopst:2.0 \
    bash .github/scripts/smoke_test.sh
```

The `Alg_mmi/lib/Xl` library is also compiled with
`-Werror=implicit-function-declaration`: a function called without a
declaration returns `int`, and on 64 bits a pointer returned that way is
truncated. Its declarations are in `Alg_mmi/lib/Xl/XlProto.h`.

No makefile uses `-fpermissive` any more: with GCC 14 it turns every new C
error into a warning. `Xl` alone downgrades exactly three categories
(`XL_PERMISSIVE` in its makefile: `incompatible-pointer-types`,
`int-conversion`, `implicit-int`), about 1300 lines of Xt idiom. Any other
GCC 14 error stops the build there too.

## Compiled files in git

Executables, libraries and **nine** objects are committed **on purpose**: users
get them with `git pull`, and the product must work without compiling anything.
All the other `.o` files are ignored (`.gitignore`: `*.o`, with an exception
for each of the nine).
After changing a source, rebuild **and relink** what depends on it (the
makefiles do not relink an executable when a library changes), and commit the
results.

Every other `.o` is a build intermediate, and has not been in git since
October 2026. Until then 824 were tracked, so every build left hundreds of
modified files to commit. The nine are different: they are linked **on the
user's machine**. LegoPC builds each task's
executables (`lg2`, `lg3`, `lg5`, `lg5sk`, `pslego3`) through
`lego_big/bin/cad_maketask`, `maketask` and `cad_marcotask`, and those link
installation objects together with the user's module libraries. Without them a
user who does not compile cannot build a model. Checked on 2026-10-16 by
rebuilding the `nuclear` demo's tasks on a fresh clone with nothing compiled:

| Objects | Linked by |
|---|---|
| `lego_big/sorglego/main/main_lg2.o` | `lg2` (`cad_crealg1`), `lg5sk` |
| `lego_big/sorglego/main/main_lg3.o` | `lg3` (`cad_crealg3`), `lg5`, `lg5sk` |
| `lego_big/sorglego/main/main_lg5.o`, `main_cf04.o` | `lg5` (`crealg5`; `main_cf04.o` only when `proc/f04.dat` is rebuilt) |
| `lego_big/sorglego/main/main_lg5sk.o` | `lg5sk` (`cad_crealg5sk`) |
| `lego_big/sorglego/main/main_rg5sk.o` | `lg5sk` of the simplified-regulation tasks (`cad_marcotask`) |
| `lego_big/lib/rg5sim.o`, `pslego3.o` | `lg5sk`, `pslego3` (`cad_marcotask`) |
| `AlgLib/trapfpe.o` | every task (`ALG_FPE_OBJ`, from `Alg_env.sh`) |

| Libraries | |
|---|---|
| `lego_big/lib/` | `clegolib.a`, `legolib.a`, `vapo.a`, `regonew.a` |
| `AlgLib/` | `libsim.a`, `libnet.a`, `libipc.a`, `libutil.a`, `libRt.a` |
| `legocad/lib/` | `liblegocad.a` |

The `task` functional test keeps this list honest: it fails, naming the file,
if a task links an installation file that git does not track.

Not on the list, on purpose:

- `lego_big/lib/cmain_lg1.o` is only an intermediate of `clg1a_exe`.
- `main_refresh.o` (for `refresh_dati`, which `net_sked` runs, if it exists,
  when the monitor changes a block's data) is neither committed nor built: only
  the old `crealibut` would link it, compiling `main_refresh.c` on the fly
  inside the installation, and that link fails anyway (`modidat_` missing from
  the `modulilib.a` built by LegoPC). LegoPC uses `cad_crealibut`, which does
  not build `refresh_dati`. A dormant feature, not a missing file.

Before the other `.o` files were untracked, the change was tested on a copy
from which they had been deleted, as `git pull` deletes them on the users'
machines. Every functional test passed without compiling anything. That copy,
though, had been filled from the working tree with `rsync`, and it missed one
thing: **git does not record empty directories**. `net_monit/cad_monit_o`,
`net_monit/net_monit_o`, `net_sked/cad_sked_o` and `net_sked/net_sked_o`
contained only `.o` files, so a fresh checkout no longer had them, and the
first GitHub build failed with `can't create cad_monit_o/monit.o`. Their
makefiles now create them (`$(shell mkdir -p ...)`). The fix was checked on a
real `git clone` of the commit: build from scratch and all seven tests green.
**Test changes like this on a real checkout, never on a copy of the working
tree.**

**Adding a new one.** Suppose a runtime script starts linking another
installation object. Then:
1. add it with a `!` line in `.gitignore`;
2. commit it;
3. add a row to the table above.

If you forget, the `task` test fails and names the file.

`make -f Makefile.mk clean` deletes **all** `.o` and `.a` files, these
included. After it, rebuild everything (`make -f Makefile.mk`) before
committing, or restore them with `git checkout`. `docker` and `docker-push`
rebuild by themselves.

## Version Management
The build system automatically generates `version.h` with:
- Git version string from `git describe`
- Build number from commit count
- Build date

## Environment Setup
Before building, source the environment script:
```bash
source .profile_legoroot
```

This sets up:
- LEGOROOT path detection
- All required environment variables
- Compiler flags for gfortran/gcc
- Library paths for threading and SQLite

## Simulatore corrente — `ksetsim` / `ksims`

`.profile_legoroot` è **general-purpose**: **non** contiene più un simulatore hardcodato. Il simulatore corrente (`KSIM` e tutte le K* derivate) si sceglie a runtime con la funzione **`ksetsim`** (definita in [Alg_env.sh](../Alg_env.sh)). Molti tool *kprocedure* (es. `kDiffS01`) fanno `cd $KSIM` e **ignorano la cwd**: vanno usati dopo aver impostato il simulatore giusto.

```bash
ksetsim SLaurentB1     # passa a $KSKED/SLaurentB1 e RI-DERIVA tutte le K* (KLOG, KSTATUS, KSCADA, KDATABASES, KPAGES, KGRAF, ...)
ksetsim /path/assoluto # accetta anche un path assoluto
ksims                  # elenca i simulatori disponibili (solo dir sotto $KSKED)
ksetsim <TAB>          # completion bash sui nomi disponibili
```

Dettagli:
- **`KSKED`** = root dei simulatori (default `$HOME/sked`), variabile *generale*.
- **Scelta "sticky"**: `ksetsim` scrive il nome in **`~/.legosim`**; una nuova shell riparte sull'ultimo scelto. Lo scrive anche `lghmi`, da *File → Current simulator*, entrando con *File → Open Simulator path* (o una recente) nella directory di un simulatore dell'area, e dopo *File → Work area*: in `lghmi` il simulatore corrente segue la directory che si guarda. Lanciato dalla directory di un simulatore, `lghmi` lo prende come corrente solo in memoria, senza scrivere `~/.legosim` (vedi [LGHMI.md](../Alg_legopc/LGHMI.md#file--current-simulator-e-la-variabile-ksim)).
- **All'avvio** il profilo chiama `ksetsim_default`, che sceglie in cascata: `~/.legosim` → `cassano0` → **primo simulatore disponibile** (`ksims`); solo se `$KSKED` è vuoto stampa un avviso e lascia `KSIM` non impostata (nessun crash).
- `ksetsim` crea le dir minime mancanti (`status/`, `log/`) e stampa `Simulatore corrente: <nome>`.
- **Override per-simulatore**: un file **`$KSIM/ksim.conf`** (sorgiato da `ksetsim`) tiene le variabili specifiche di quel simulatore, che così **vivono col simulatore** e non nel profilo generale. Es. per `cassano0` (nome cassaforte irregolare):
  ```sh
  # sked/cassano0/ksim.conf
  export KCASSAFORTE=$KSIM/cassanosafe
  ```
- **`KCASSAFORTE`** (= "cassaforte" grafica) è la dir delle risorse **curve/trend** (`$KCASSAFORTE/curve` → `KGRAF`) e **plant display** (`$KCASSAFORTE/plant_display` → `KDIRPD`), usata **solo** dalle procedure di generazione grafica/SCADA (`kMakeCurve*`, `kMakePdList`, `k_crea_cassaforte`, …). Non serve a `kDiffS01`/legopc/core; il default `$KSIM/<nome>safe` va bene anche se la dir non esiste.

## Creare un simulatore nuovo — `creasim`

`ksetsim` **pretende che la directory esista già**, e `k_crea_simulatore` lavora su
`$KSIM`: da zero mancava quindi il primo passo. Lo fa **`creasim`**:

```sh
creasim <nome-simulatore>      # crea $KSKED/<nome> pronto da configurare
```

Cosa fa, in ordine:

1. crea `$KSKED/<nome>` (esce con errore se esiste già);
2. delega a **`k_crea_simulatore`** le 18 sottodirectory (`databases/`, `export/`,
   `globpages*/`, `log/`, `o_win/`, `plant_display/`, `scada/*`, `statistic/`,
   `status/`, `tmp/`) passandogli `KSIM`: **la lista sta in un posto solo**;
3. installa **`al_sim.conf`** da `util97/bin/al_sim.conf.example`, togliendo le
   righe di README del template;
4. installa **`Simulator`** da [`lego_big/procedure/Simulator.tpl`](../lego_big/procedure/Simulator.tpl);
5. stampa i passi successivi.

Poi: `ksetsim <nome>`, adattare `al_sim.conf` (`TITLE`, `BASEPATH`,
`MMI_HOSTNAME` e l'elenco delle task `P`/`R`) e verificare i dimensionamenti in
`Simulator`. `creasim` **non** compila e **non** genera l'`S01`.

### I due file di configurazione, da non confondere

| File | Contiene | Template |
|---|---|---|
| `al_sim.conf` | **composizione**: titolo, path, host, config MMI, elenco delle task (`P`=processo, `R`=regolazione) con dt e descrizione. È l'input di `connex2`. Come si compila: [AL_SIM_CONF.md](AL_SIM_CONF.md). | `util97/bin/al_sim.conf.example` |
| `Simulator` | **dimensionamento** runtime: `MAX_CAMPIONI`, `NUM_VAR`, snapshot, backtrack | `lego_big/procedure/Simulator.tpl` |

I default di `Simulator.tpl` sono `MAX_CAMPIONI: 14400` e `NUM_VAR: 10000`. Gli
stessi valori sono **ripetuti come fallback** in tutti gli script di avvio
(`startup`, `net_startup`, `simula`, `net_startup_headless`, …), usati quando il
file `Simulator` manca: cambiando i default vanno allineati anche lì.

## Aggiornare la configurazione di un simulatore

Dopo aver modificato modelli, schemi o faceplate, la configurazione si riallinea
con **`kUpSim`**, che orchestra i sette passi e si ferma al primo che fallisce
dicendo quale:

```sh
kUpSim              # catena completa
kUpSim -nommi       # senza le pagine MMI dei faceplate
kUpSim -n           # dry run: mostra i passi senza eseguirli
```

In [Alg_env.sh](../Alg_env.sh) ci sono le due scorciatoie:
`lgupsim` per la catena completa e `lgupsimx` per quella senza pagine MMI.

I sette passi, per esteso:

```sh
cd $KSIM && kConnex && kNetCompi && kCompStaz && kStazPages && kWinContext && kCompileSim && kCollect
```

| passo | cosa rifà |
|---|---|
| `kConnex` | topologia e connessioni fra task → `S01` |
| `kNetCompi` | compilazione delle task → `variabili.rtf` |
| `kCompStaz` | faceplate per `xstaz` → `r02.dat` |
| `kStazPages` | gli stessi faceplate come pagine MMI → `$KWIN/O_*.pag` |
| `kWinContext` | `Context.ctx` di `$KWIN` |
| `kCompileSim` | compila le pagine di `$KWIN` → `.rtf` |
| `kCollect` | raccolta in `globpages` + `kMmiConfig` |

Chi non porta i faceplate nell'MMI usa `kUpSim -nommi` (alias `lgupsimx`), che
esegue solo `kConnex`, `kNetCompi`, `kCompStaz` e `kCollect`.

`kUpSim` non fa niente di diverso dalla catena a mano, ma scrive in
`$KLOG/kUpSim.log`, aggiorna schermata e stato come le altre procedure, e dopo
`kConnex` e `kNetCompi` verifica che `S01` e `variabili.rtf` siano stati davvero
riscritti — un controllo che serve perché quasi tutte le `kprocedure` terminano
con una `print` e restituiscono 0 comunque.

Due cose da sapere, entrambe imparate sul campo:

- **I comandi vanno scritti in CamelCase.** Le versioni minuscole (`kconnex`,
  `kcollect`, `knetcompi`…) sono wrapper di tre righe che lanciano il comando in
  **background** (`. $KBIN/kConnex $* &`): ritornano subito, quindi in una catena
  `&&` non sequenziano niente e l'exit status è sempre 0.
- **`kCompStaz` esiste per rimediare a un exit status inutilizzabile.**
  `compstaz` termina con `exit(puts(...))`, cioè restituisce il numero di
  caratteri stampati: 24 quando riesce, 42 quando fallisce. Messo in una catena
  `&&` la interrompe anche dopo un successo. La procedura ricava l'esito
  dall'output e restituisce 0 o 1. Vale lo stesso per `convstaz`, che infatti
  viene lanciato da `kStazPages`.

L'ordine non è arbitrario: `net_compi` riscrive `variabili.rtf`, e `kCompStaz`
risolve gli indici delle variabili contro quel file — ricompilare le task senza
ricompilare i faceplate lascia `r02.dat` con indici che possono non corrispondere
più, senza alcun messaggio d'errore.

## `kDiffS01` / `diffs01` — coerenza delle variabili di interconnessione

Strumento *kprocedure* che, per un simulatore **composto** (descritto da un file `S01`, vedi la sezione `lghmi`/`S01` più sotto), stampa i **valori di stazionario delle variabili di interconnessione** tra le task dello scheduler: serve a verificare che l'uscita di una task e l'ingresso connesso su un'altra abbiano **valori coerenti**.

**Catena** (orchestratore `kbin/kDiffS01`, sorgente `kprocedure/kDiffS01.sh`):
1. `ktest` → verifica ambiente; `cd $KSIM`; richiede il file **`S01`** lì;
2. `kDiffS01Slave1` → costruisce `kDiffS01.DB` dai `proc/f24.dat` (condizioni iniziali) di ogni task elencata in `S01`;
3. `kDiffS01Slave2` (se `SID_ENV=SID`) / `kDiffS01Slave6` (se `GIPS`) → DB aggiuntivo (senza `SID_ENV` solo un warning);
4. **`kDiffS01Slave4`** = `PROGRAM DIFFS01` ([kutil/kDiffS01Slave4.f](../kutil/kDiffS01Slave4.f), ELF in `kbin`): legge `S01` + i `kDiffS01.DB` → produce `diffs01.out`/`diffs01.err`;
5. l'orchestratore accoda `diffs01.out`/`.err` in **`$KLOG/kDiffS01.log`** (= `$KSIM/log/kDiffS01.log`) e ne stampa il path.

**Uso**:
```bash
ksetsim SLaurentB1      # IMPRESCINDIBILE: kDiffS01 fa cd $KSIM, ignora la cwd
kDiffS01
cat "$KLOG/kDiffS01.log"
```
**Prerequisiti**: `$KSIM/S01` presente; le task con `proc/f24.dat`; dir `status/`/`log/` (le crea `ksetsim`). `SID_ENV` non settata → solo warning, `Slave4` gira comunque.

**`util97/bin/diffs01`**: è lo **stesso** motore (`PROGRAM DIFFS01`), ricompilato da `kutil/kDiffS01Slave4.f` (prima era uno stub corrotto di 3 byte → *command not found*; la regola in [util97/procedure/Makefile.mk](../util97/procedure/Makefile.mk) ora ricompila dal Fortran invece di copiare il `.sh`). **Non è standalone**: legge anch'esso i `kDiffS01.DB` creati da `Slave1`, quindi per un output sensato va usata la procedura `kDiffS01` completa, non il binario isolato.
