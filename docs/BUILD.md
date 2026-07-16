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
- docker components

## Clean Build
```bash
make -f Makefile.mk clean
```
Removes all object files (*.o), libraries (*.a), and generated version.h

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
- **Scelta "sticky"**: `ksetsim` scrive il nome in **`~/.legosim`**; una nuova shell riparte sull'ultimo scelto.
- **All'avvio** il profilo chiama `ksetsim_default`, che sceglie in cascata: `~/.legosim` → `cassano0` → **primo simulatore disponibile** (`ksims`); solo se `$KSKED` è vuoto stampa un avviso e lascia `KSIM` non impostata (nessun crash).
- `ksetsim` crea le dir minime mancanti (`status/`, `log/`) e stampa `Simulatore corrente: <nome>`.
- **Override per-simulatore**: un file **`$KSIM/ksim.conf`** (sorgiato da `ksetsim`) tiene le variabili specifiche di quel simulatore, che così **vivono col simulatore** e non nel profilo generale. Es. per `cassano0` (nome cassaforte irregolare):
  ```sh
  # sked/cassano0/ksim.conf
  export KCASSAFORTE=$KSIM/cassanosafe
  ```
- **`KCASSAFORTE`** (= "cassaforte" grafica) è la dir delle risorse **curve/trend** (`$KCASSAFORTE/curve` → `KGRAF`) e **plant display** (`$KCASSAFORTE/plant_display` → `KDIRPD`), usata **solo** dalle procedure di generazione grafica/SCADA (`kMakeCurve*`, `kMakePdList`, `k_crea_cassaforte`, …). Non serve a `kDiffS01`/legopc/core; il default `$KSIM/<nome>safe` va bene anche se la dir non esiste.

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
