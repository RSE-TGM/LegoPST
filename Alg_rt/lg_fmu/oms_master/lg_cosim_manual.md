# lg_cosim — Manuale Utente

Co-simulation master FMI 2.0 per FMU LegoPST.  
Gestisce N FMU in co-simulazione: scambio variabili, sincronizzazione real-time / tempo accelerato.

---

## Prerequisiti

```bash
source ~/fmpy_venv/bin/activate   # venv fmpy già usato per run_fmu
```

Nessuna dipendenza aggiuntiva: usa `fmpy` (già validato con le FMU bundle LegoPST).

---

## Struttura della directory

```
oms_master/
  lg_cosim.py          # master di co-simulazione
  config.json          # configurazione della sessione
  lg_cosim_manual.md   # questo file
  example/
    legoclix_MDC_GV_bundle.fmu
    legoclix_collet_bundle.fmu
    legoclix_ctrcoll_bundle.fmu
```

---

## Avvio rapido

`lg_cosim.py` richiede `fmpy` — usare il venv già configurato per `run_fmu.sh`:

```bash
cd /home/antonio/LegoPST/Alg_rt/lg_fmu/oms_master

# run base (massima velocità)
/home/antonio/fmpy_venv/bin/python3 lg_cosim.py config.json

# con output diagnostico FMU (LG_FMU_DEBUG=1)
/home/antonio/fmpy_venv/bin/python3 lg_cosim.py config.json --debug

# real-time
/home/antonio/fmpy_venv/bin/python3 lg_cosim.py config.json --speedup 1.0

# tempo accelerato 5×
/home/antonio/fmpy_venv/bin/python3 lg_cosim.py config.json --speedup 5.0

# override parametri al volo
/home/antonio/fmpy_venv/bin/python3 lg_cosim.py config.json --stop-time 120 --step-size 0.5 --speedup 2.0
```

In alternativa, attivare il venv prima:

```bash
source ~/fmpy_venv/bin/activate
python3 lg_cosim.py config.json
```

---

## Il file `config.json`

### Schema completo

```json
{
  "model_name": "NomeSistema",
  "settings": {
    "start_time":  0.0,
    "stop_time":   30.0,
    "step_size":   1.0,
    "speedup":     null,
    "log_file":    "log_cosim.csv",
    "log_vars":    ["FMU_A.VARNAME", "FMU_B.VARNAME"]
  },
  "fmus": {
    "FMU_A": "path/al/modello_a.fmu",
    "FMU_B": "path/al/modello_b.fmu"
  },
  "start_values": {
    "FMU_A.INPUT1": 0.5,
    "FMU_B.PARAM1": 1
  },
  "connections": [
    {"from": "FMU_A.OUTPUT1", "to": "FMU_B.INPUT1"},
    {"from": "FMU_B.OUTPUT2", "to": "FMU_A.INPUT2"}
  ]
}
```

### Campi `settings`

| Campo         | Tipo    | Default            | Descrizione                                              |
|---------------|---------|--------------------|----------------------------------------------------------|
| `start_time`  | float   | `0.0`              | Tempo iniziale simulazione                               |
| `stop_time`   | float   | —                  | Tempo finale (obbligatorio)                              |
| `step_size`   | float   | —                  | Passo di comunicazione FMI (obbligatorio)                |
| `speedup`     | float\|null | `null`         | Vedi sezione [Speedup](#speedup-e-real-time) sotto       |
| `log_file`    | string  | `"log_cosim.csv"`  | Path del CSV di output; se relativo, risolto rispetto alla dir di `config.json` (non al CWD del processo, che `lg_fmi2.c` sposta nella task dir) |
| `log_vars`    | lista   | `[]`               | Variabili da loggare nel CSV, formato `"FMU.VARNAME"`    |

### Campo `fmus`

Mappa **nome istanza** → **path FMU**. I path relativi sono risolti rispetto alla directory di `config.json`.

```json
"fmus": {
    "MDC":  "example/legoclix_MDC_GV_bundle.fmu",
    "CTRL": "/home/antonio/legocad/collet/legoclix_collet_bundle.fmu"
}
```

L'ordine di dichiarazione determina l'ordine di esecuzione dei `doStep` (schema Gauss-Seidel).

### Campo `start_values`

Valori imposti prima di `exitInitializationMode` (fase di setup FMI).  
Formato: `"NOME_FMU.NOME_VARIABILE"`.

```json
"start_values": {
    "MDC.ALZAVAU1":        0.5,
    "CTRL.ITYPTMIS":       1,
    "CTRL.AFLGTMIS":       1.0
}
```

I tipi (`Real`, `Integer`, `Boolean`) vengono rilevati automaticamente dal `modelDescription.xml`.

### Campo `connections`

Lista di connessioni output→input tra FMU. Aggiornate ad ogni passo dopo i `doStep`.

```json
"connections": [
    {"from": "CTRL.ALZAVING",  "to": "MDC.ALZAVAU1"},
    {"from": "MDC.TFLDTMIS",   "to": "CTRL.TESTMANI"}
]
```

Connessione 1-a-molti: ripetere il campo `from` in entry separate.

```json
"connections": [
    {"from": "CTRL.ALZAVING", "to": "MDC.ALZAVAU1"},
    {"from": "CTRL.ALZAVING", "to": "MDC.ALZAVAU2"}
]
```

---

## Come trovare i nomi delle variabili

Usare `run_fmu.sh --info` (già presente in `Alg_rt/lg_fmu/scripts/`):

```bash
source ~/.profile_legoroot /home/antonio/LegoPST
run_fmu.sh --info example/legoclix_MDC_GV_bundle.fmu
```

Output (colonne: nome, causality, tipo, start):
```
ALZAVAU1    input   Real    0.4124
ALZAVAU2    input   Real    0.4124
TFLDTMIS    output  Real    574.97
WVALVAU1    output  Real    12.3
...
```

In alternativa, estrarre direttamente il `modelDescription.xml`:

```bash
unzip -p example/legoclix_MDC_GV_bundle.fmu modelDescription.xml | grep 'name='
```

---

## Speedup e real-time

| Valore `speedup` | Comportamento                          |
|------------------|----------------------------------------|
| `null` o `0`     | Massima velocità (nessun sleep)        |
| `1.0`            | Real-time (1s simulato = 1s wall)      |
| `2.0`            | 2× real-time (1s simulato = 0.5s wall) |
| `0.5`            | Metà velocità (1s simulato = 2s wall)  |

Se la macchina non regge il passo richiesto, lo script segnala gli **overrun** a fine run:

```
[lg_cosim] attenzione: 5 overrun real-time (16.7% dei passi)
```

Nota: `step_size` deve essere ≥ `dt_sked` della FMU (tipicamente 0.1 s per le task LegoPST).  
Valori più piccoli vengono arrotondati internamente dalla FMU senza errore ma senza effetto.

---

## Output CSV

Il file `log_cosim.csv` contiene una colonna `time` e una colonna per ogni variabile in `log_vars`:

```
time,MDC.TFLDTMIS,CTRL.ALZAVING
1.0,574.97,0.4124
2.0,574.85,0.4100
...
```

---

## Logica Python nel loop — `step_hook`

Per iniettare calcoli Python tra una FMU e l'altra (controllore esterno, limitatori, logica di supervisione), creare uno script separato che estende `LgCosim`:

```python
from lg_cosim import LgCosim

class SistemaConControllore(LgCosim):
    def step_hook(self, t):
        # leggo un output dalla FMU MDC
        portata = self.fmus["MDC"].get("WVALVAU1")

        # calcolo il setpoint con logica Python
        apertura = 0.3 if portata > 15.0 else 0.6

        # scrivo l'input sulla FMU CTRL
        self.fmus["CTRL"].set("ALZAVAU1", apertura)

sim = SistemaConControllore("config.json")
sim.run()
```

`step_hook(t)` viene chiamato **dopo** i `doStep` e **dopo** lo scambio delle `connections`,
con `t` = tempo simulato corrente (già avanzato del passo).

---

## Argomenti da riga di comando

```
usage: lg_cosim.py [config] [--stop-time T] [--step-size H] [--speedup S] [--debug]

  config        path al file JSON (default: config.json)
  --stop-time T sovrascrive settings.stop_time
  --step-size H sovrascrive settings.step_size
  --speedup S   sovrascrive settings.speedup (0 = max)
  --debug       setta LG_FMU_DEBUG=1 — output [lg_fmu DBG] su stderr per ogni FMU
```

Exit code: `0` se nessun overrun, `1` se ci sono stati overrun real-time.

---

## Variabili nel config: nome breve vs nome completo

I nomi delle variabili nel `modelDescription.xml` hanno la forma `taskname.Bnn.NOMEVARIABILE`
(es. `ctrcoll.B5.IN_1PMIS`). Nel `config.json` puoi usare indifferentemente:

- **Nome breve**: `CONTROLLER.IN_1PMIS` — sufficiente se il nome è univoco all'interno della FMU
- **Nome completo**: `CONTROLLER.ctrcoll.B5.IN_1PMIS` — necessario solo in caso di ambiguità

Il nome simbolico dell'istanza (`CONTROLLER`, `COLLET`, ...) serve come disambiguatore
di istanza, non di variabile: permette di avere due copie della stessa FMU con nomi distinti
(es. `COLLET_A` e `COLLET_B`).

---

## Note su co-simulazione multi-istanza (stesso processo)

Ogni bundle FMU lancia il proprio `dispatcher` + `net_sked` usando un **slot SHM**
(chiave SysV IPC). Il meccanismo di slot LegoPST:

- Slot disponibili: `uid*10000 + slot*1100`, con `slot` ∈ {1..8}
- Slot 0 (`uid*10000`) riservato alla sessione LegoPST interattiva dell'utente
- Ogni istanza occupa ~1030 chiavi consecutive

### Selezione slot Python-side (`_find_free_slot`)

`lg_cosim.py` effettua il probe degli slot lato Python via `shmget(key + ID_SHM_VAR, 0, 0)`.
`ID_SHM_VAR=5` è la SHM effettivamente creata in modalità headless. Il probe interno
a `lg_fmi2.c` usa invece `ID_SHM_SIM=0`, che in modalità headless non viene mai
creato → tutti gli slot sembrano liberi → colliderebbero. Per questo il probe
Python pre-assegna `SHR_USR_KEY` via `ctypes.setenv` prima di ogni
`fmi2Instantiate`, garantendo slot isolati per ciascuna FMU.

Il log di avvio mostra il slot assegnato:
```
[lg_cosim] COLLET:     slot SHM SHR_USR_KEY=10001100
[lg_cosim] CONTROLLER: slot SHM SHR_USR_KEY=10002200
```

### `LG_COSIM_NO_KILLSIM` — protezione da killsim distruttivo

`killsim` su Linux cancella **tutte** le SHM dell'utente (non solo il proprio
slot), come documentato in `killsim.c:402` ("test fittizio per LINUX"). In
co-simulazione, il `killsim` chiamato da `net_startup_headless.sh` della
seconda FMU distruggerebbe le SHM della prima FMU già avviata.

`lg_cosim.py` setta `LG_COSIM_NO_KILLSIM=1` nell'environment C (via
`ctypes.setenv`) prima di qualunque `fmi2Instantiate`. `net_startup_headless.sh`
(sia la versione in `Alg_rt/lg_fmu/scripts/` sia quelle estratte dai bundle)
rispetta questa variabile:

```bash
if [ -z "${LG_COSIM_NO_KILLSIM:-}" ]; then
    killsim 2>/dev/null || true
fi
```

I `net_startup_headless.sh` estratti dai bundle vengono patchati on-the-fly
da `_patch_killsim_guard()` in `_FMUInstance.__init__()` subito dopo
`fmpy.extract`, in modo che il guard sia attivo anche nei bundle già generati
prima dell'aggiornamento degli script.

**Limite**: massimo 8 FMU concorrenti per utente (slot 1..8). Se tutti gli slot
sono occupati da run precedenti, usare `killsim` per liberarli (con una sola
FMU alla volta).

---

## Troubleshooting

| Sintomo | Causa probabile | Soluzione |
|---------|----------------|-----------|
| `KeyError: 'NOMEVARIABILE'` | Variabile non presente nel modelDescription | Verificare con `run_fmu.sh --info` o `unzip -p *.fmu modelDescription.xml \| grep name=` |
| `fmi2DoStep failed` al secondo run | File `f22circ.dat`/`backtrack.dat` residui nella dir estratta | `net_startup_headless.sh` li cancella automaticamente ad ogni lancio. Se persiste, cancellare manualmente `<bundle_dir>/resources/bundle/task/<task>/f22circ.dat` e `backtrack.dat` |
| FMU avviata scompare durante init della seconda FMU | `killsim` della seconda FMU ha distrutto le SHM della prima | Verificare che `LG_COSIM_NO_KILLSIM` sia visibile a `net_startup_headless.sh`; su bundle vecchi (precedenti alla patch) ri-estrarre e rilanciar: `_patch_killsim_guard()` provvede automaticamente |
| `COLLET.VAR` / `CONTROLLER.VAR` → `KeyError` durante log | Nome variabile sbagliato in `log_vars` | Usare `unzip -p <fmu> modelDescription.xml \| grep 'name='` per vedere i nomi effettivi; i nomi brevi (ultima componente) funzionano se univoci |
| CSV finisce in una directory inattesa | `lg_fmi2.c` chiama `chdir(task_path)` durante l'instantiate | `log_file` relativo è ora risolto rispetto alla dir di `config.json`, non al CWD. Aggiornare a `lg_cosim.py` ≥ 2026-05-11 |
| Overrun real-time sistematici | `step_size` troppo piccolo o macchina sovraccarica | Aumentare `step_size` oppure ridurre `speedup` |
| FMU non parte (timeout init) | `net_sked` non raggiunge `STATO_FREEZE` | Lanciare con `--debug` per output `[lg_fmu DBG]` su stderr |
| Slot IPC esauriti (> 8 istanze) | Massimo 8 FMU concorrenti per utente | Terminare le istanze in eccesso con `killsim`; `ipcs -m` mostra i slot occupati |
| `to_dispatcher: ...` su stdout durante la sim | Output normale del dispatcher C (non un errore) | Redirigere stderr a `/dev/null` o a file se disturbano |

---

*Versione: lg_cosim 1.1 — 2026-05-11*  
*Aggiornamenti: slot probe Python-side (ID_SHM_VAR), `LG_COSIM_NO_KILLSIM`, `_patch_killsim_guard`, log_file assoluto.*
