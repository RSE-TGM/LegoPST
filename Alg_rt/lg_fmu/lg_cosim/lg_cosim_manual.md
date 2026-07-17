# lg_cosim — Manuale Utente

Co-simulation master FMI 2.0.  
Gestisce N FMU in co-simulazione: scambio variabili, sincronizzazione real-time / tempo accelerato.

Accetta **sia FMU LegoPST sia FMU standard di terze parti**, anche mescolate nello
stesso `lg_cosim.json`. Le funzioni specifiche LegoPST (slot SHM, HMI `draw2gr`,
ripristino permessi del bundle) si attivano **solo** sulle FMU LegoPST, riconosciute
dalla presenza di `resources/task_info.env` nel `.fmu`; sulle altre sono no-op
silenziosi. Vedi [FMU non LegoPST](#fmu-non-legopst-e-limiti).

---

## Prerequisiti

```bash
source ~/fmpy_venv/bin/activate   # venv fmpy già usato per run_fmu
```

Nessuna dipendenza aggiuntiva: usa `fmpy` (già validato con le FMU bundle LegoPST).

---

## Struttura della directory

```
lg_cosim/
  lg_cosim.py          # master di co-simulazione
  lg_cosim.json        # configurazione della sessione
  lg_cosim_manual.md   # questo file
  example/
    legoclix_collet_bundle.fmu
    legoclix_ctrcoll_bundle.fmu
```

---

## Avvio rapido

`lg_cosim.py` richiede `fmpy` — usare il venv già configurato per `run_fmu.sh`:

```bash
cd /home/antonio/LegoPST/Alg_rt/lg_fmu/lg_cosim

# run base (massima velocità)
/home/antonio/fmpy_venv/bin/python3 lg_cosim.py lg_cosim.json

# con output diagnostico FMU (LG_FMU_DEBUG=1)
/home/antonio/fmpy_venv/bin/python3 lg_cosim.py lg_cosim.json --debug

# real-time
/home/antonio/fmpy_venv/bin/python3 lg_cosim.py lg_cosim.json --speedup 1.0

# tempo accelerato 5×
/home/antonio/fmpy_venv/bin/python3 lg_cosim.py lg_cosim.json --speedup 5.0

# override parametri al volo
/home/antonio/fmpy_venv/bin/python3 lg_cosim.py lg_cosim.json --stop-time 120 --step-size 0.5 --speedup 2.0
```

In alternativa, attivare il venv prima:

```bash
source ~/fmpy_venv/bin/activate
python3 lg_cosim.py lg_cosim.json
```

---

## Il file `lg_cosim.json`

### Schema completo

```json
{
  "model_name": "NomeSistema",
  "settings": {
    "start_time":  0.0,
    "stop_time":   30.0,
    "step_size":   1.0,
    "speedup":     null,
    "hmi":         false,
    "log_file":    "log_cosim.csv",
    "log_vars":    ["FMU_A.VARNAME", "FMU_B.VARNAME"]
  },
  "fmus": {
    "FMU_A": {"path": "path/al/modello_a.fmu", "desc": "descrizione per lghmi"},
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
| `hmi`         | bool    | `false`            | Apre il selettore `lghmi` da cui scegliere le pagine `draw2gr`. Richiede `DISPLAY`. Vedi [HMI](#hmi-draw2gr-in-co-simulazione) |
| `log_file`    | string  | `"log_cosim.csv"`  | Path del CSV di output; se relativo, risolto rispetto alla dir di `lg_cosim.json` (non al CWD del processo, che `lg_fmi2.c` sposta nella task dir) |
| `log_vars`    | lista   | `[]`               | Variabili da loggare nel CSV, formato `"FMU.VARNAME"`    |

### Campo `fmus`

Mappa **nome istanza** → **path FMU**. I path relativi sono risolti rispetto alla directory di `lg_cosim.json`.

```json
"fmus": {
    "COLLET":     "example/legoclix_collet_bundle.fmu",
    "CONTROLLER": "/home/antonio/legocad/ctrcoll/legoclix_ctrcoll_bundle.fmu"
}
```

L'ordine di dichiarazione determina l'ordine di esecuzione dei `doStep` (schema Gauss-Seidel).

Forma estesa, per dare una descrizione leggibile alla task nell'elenco di `lghmi`
(default: `task <nome> (<file.fmu>)`):

```json
"fmus": {
    "COLLET":     {"path": "example/legoclix_collet_bundle.fmu", "desc": "Collettore"},
    "CONTROLLER": "example/legoclix_ctrcoll_bundle.fmu"
}
```

### Campo `start_values`

Valori imposti prima di `exitInitializationMode` (fase di setup FMI).  
Formato: `"NOME_FMU.NOME_VARIABILE"`.

```json
"start_values": {
    "COLLET.ALZAVAU1":     0.5,
    "CONTROLLER.IN_1PMIS": 3.10E5
}
```

I tipi (`Real`, `Integer`, `Boolean`) vengono rilevati automaticamente dal `modelDescription.xml`.

### Campo `connections`

Lista di connessioni output→input tra FMU. Aggiornate ad ogni passo dopo i `doStep`.

```json
"connections": [
    {"from": "COLLET.PCOLMANI",     "to": "CONTROLLER.IN_1PMIS"},
    {"from": "CONTROLLER.USOMUCTR", "to": "COLLET.ALZAVING"}
]
```

Connessione 1-a-molti: ripetere il campo `from` in entry separate.

```json
"connections": [
    {"from": "CONTROLLER.USOMUCTR", "to": "COLLET.ALZAVAU1"},
    {"from": "CONTROLLER.USOMUCTR", "to": "COLLET.ALZAVAU2"}
]
```

---

## Come trovare i nomi delle variabili

Usare `run_fmu.sh --info` (già presente in `Alg_rt/lg_fmu/scripts/`):

```bash
source ~/.profile_legoroot /home/antonio/LegoPST
run_fmu.sh --info example/legoclix_collet_bundle.fmu
```

Output (colonne: nome, causality, tipo, start):
```
ALZAVAU1    input   Real    0.4124
ALZAVING    input   Real    0.4124
PCOLMANI    output  Real    2979659.25
...
```

In alternativa, estrarre direttamente il `modelDescription.xml`:

```bash
unzip -p example/legoclix_collet_bundle.fmu modelDescription.xml | grep 'name='
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
time,COLLET.PCOLMANI,CONTROLLER.USOMUCTR
1.0,2979659.25,3.2496666
2.0,3534810.5,1.0
3.0,4038576.25,1.0
...
```

---

## Logica Python nel loop — `step_hook`

Per iniettare calcoli Python tra una FMU e l'altra (controllore esterno, limitatori, logica di supervisione), creare uno script separato che estende `LgCosim`:

```python
from lg_cosim import LgCosim

class SistemaConControllore(LgCosim):
    def step_hook(self, t):
        # leggo un output dalla FMU COLLET
        pressione = self.fmus["COLLET"].get("PCOLMANI")

        # calcolo il setpoint con logica Python
        apertura = 0.3 if pressione > 4.0e6 else 0.6

        # scrivo l'input sulla FMU COLLET
        self.fmus["COLLET"].set("ALZAVAU1", apertura)

sim = SistemaConControllore("lg_cosim.json")
sim.run()
```

`step_hook(t)` viene chiamato **dopo** i `doStep` e **dopo** lo scambio delle `connections`,
con `t` = tempo simulato corrente (già avanzato del passo).

---

## Argomenti da riga di comando

```
usage: lg_cosim.py [config] [--stop-time T] [--step-size H] [--speedup S] [--debug]

  config        path al file JSON (default: lg_cosim.json)
  --stop-time T sovrascrive settings.stop_time
  --step-size H sovrascrive settings.step_size
  --speedup S   sovrascrive settings.speedup (0 = max)
  --debug       setta LG_FMU_DEBUG=1 — output [lg_fmu DBG] su stderr per ogni FMU
```

Exit code: `0` se nessun overrun, `1` se ci sono stati overrun real-time.

---

## Variabili nel config: nome breve vs nome completo

I nomi delle variabili nel `modelDescription.xml` hanno la forma `taskname.Bnn.NOMEVARIABILE`
(es. `ctrcoll.B5.IN_1PMIS`). Nel `lg_cosim.json` puoi usare indifferentemente:

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
| CSV finisce in una directory inattesa | `lg_fmi2.c` chiama `chdir(task_path)` durante l'instantiate | `log_file` relativo è ora risolto rispetto alla dir di `lg_cosim.json`, non al CWD. Aggiornare a `lg_cosim.py` ≥ 2026-05-11 |
| Overrun real-time sistematici | `step_size` troppo piccolo o macchina sovraccarica | Aumentare `step_size` oppure ridurre `speedup` |
| FMU non parte (timeout init) | `net_sked` non raggiunge `STATO_FREEZE` | Lanciare con `--debug` per output `[lg_fmu DBG]` su stderr |
| Slot IPC esauriti (> 8 istanze) | Massimo 8 FMU concorrenti per utente | Terminare le istanze in eccesso con `killsim`; `ipcs -m` mostra i slot occupati |
| `to_dispatcher: ...` su stdout durante la sim | Output normale del dispatcher C (non un errore) | Redirigere stderr a `/dev/null` o a file se disturbano |

---

## HMI (draw2gr) in co-simulazione

Un **unico interruttore** per l'intera sessione, in `settings`:

```json
"settings": { "stop_time": 30.0, "step_size": 1.0, "speedup": 1.0, "hmi": true }
```

```bash
DISPLAY=:0 python3 lg_cosim.py --speedup 1.0
```

Non si apre una HMI per FMU: si apre **il selettore `lghmi`**, e da lì scegli a
run-time quali pagine `draw2gr` aprire — quante vuoi e quando vuoi — invece di
doverlo decidere nel config prima di partire. Il selettore resta aperto per tutta
la sessione e **sopravvive** alla fine del run, così puoi guardare lo stato finale.

Nella pagina hai schema animato, *View → Show Value*, Plot e **Command Mode**
(perturbazione via `xaing`): in co-simulazione perturbi il modello mentre il
master scambia le variabili. Usa **`--speedup 1.0`** o simile — a velocità massima
la sim vola e non c'è tempo di osservare né di perturbare.

Serve un **`DISPLAY` X11**: senza, `lg_cosim` lo dice e prosegue headless.

### Come funziona (e perché così)

`lg_cosim.json` è concettualmente un **S01**: entrambi descrivono un simulatore
composto da N task con le interconnessioni. Quando `settings.hmi` è attivo, il
master lo traduce con [`lg_cosim2s01.py`](lg_cosim2s01.py) e lancia `lghmi` da
quella directory: `lghmi` entra nella sua **modalità S01 già esistente**, che
elenca le task. Nessun formato di lista inventato per l'occasione.

La traduzione avviene **dopo** l'instantiate — sim vive e slot SHM assegnati —
quindi ogni pagina aperta trova la propria sim per costruzione.

**Ogni FMU è un simulatore a sé, con il proprio `net_sked` e la propria
`SHR_USR_KEY`** (mentre un S01 nativo ha un solo scheduler per tutte le task).
La chiave non è dichiarata da nessuna parte: `run_draw2gr.sh` cerca il `net_sked`
la cui *cwd* è la task dir che sta aprendo e ne legge `SHR_USR_KEY` da
`/proc/<pid>/environ`. Perciò ogni pagina punta alla sim giusta anche con più
`net_sked` attivi, e `lghmi` delega al `run_draw2gr.sh` **del bundle di quella
task** — che conosce il proprio ambiente e funziona anche tra bundle diversi.

Il traduttore è usabile anche da solo, senza avviare nulla — e genera un S01
**completo**, quindi serve oltre a `lghmi`:

```bash
cd <dir con lg_cosim.json>
python3 lg_cosim2s01.py        # genera ./S01
lghmi                          # elenca le task (legge sez. 1-3)
kDiffS01                       # verifica coerenza delle variabili di interconnessione
```

L'S01 prodotto ha tutte le sezioni di un S01 nativo: simulatore, elenco task,
path + tipo, **distribuzione** (`OS <host> guest <path>`, host da `settings.host`
o per-FMU), **passo di integrazione** per task (`settings.step_size`, override
per-FMU con `dt`), **connessioni** (un blocco per task destinazione, direzione
`<var_in> <task_sorgente> <var_out>`) e i processi di sessione `BM/SCADA/BI` che
`net_compi` pretende. I nomi di variabile non sono troncati a 8 caratteri.

> **Non è eseguibile** da `net_sked`: descrive la co-simulazione (per `lghmi`,
> `kDiffS01`, ispezione), non è un simulatore composto nativo — un S01 nativo ha
> UNO scheduler per N task, una co-simulazione ha N scheduler indipendenti. Le
> FMU non LegoPST sono saltate (nessuna task dir), e con esse le connessioni
> da/verso di loro.

### Se la HMI non compare

- **`[lg_cosim] hmi: nessun selettore trovato`** → i bundle sono anteriori a
  `run_lghmi.sh`: rigenerali con `bundle/build.sh`. L'S01 resta comunque generato,
  quindi puoi lanciare `lghmi` a mano da quella directory.
- **La pagina non si apre** → guarda **`<task>/draw2gr.fmu.log`** dentro il bundle
  estratto (`<dir del .fmu>/<nome>/resources/bundle/task/<nome>/`): la HMI gira in
  background e i suoi errori finiscono lì. Causa storica: `run_draw2gr.sh` non
  esportava `LG_MODELS` e `draw2gr` moriva in avvio — i bundle costruiti prima di
  quel fix vanno rigenerati.
- **`ERR: SHR_USR_KEY non impostata e nessun net_sked trovato`** → la sim di quella
  task non è viva (run finito, o FMU non ancora istanziata).

---

## FMU non LegoPST, e limiti

`lg_cosim` esegue **normali FMU FMI 2.0 Co-Simulation** di terze parti, da sole o
insieme a FMU LegoPST. Non serve configurare nulla: il riconoscimento è automatico
(presenza di `resources/task_info.env` nel `.fmu`).

Su una FMU non LegoPST il master **salta**: il probe dello slot SHM e
`SHR_USR_KEY`/`SHR_USR_KEYS` (una FMU standard non crea SHM LegoPST: il probe la
vedrebbe sempre "libera" e assegnerebbe lo stesso slot a tutte) e il ripristino
dei permessi del bundle. Restano attivi `connections`, `start_values`, `log_vars`
e il pacing `--speedup`, identici.

Con `settings.hmi` le FMU non LegoPST semplicemente **non compaiono nell'elenco**
del selettore (`lg_cosim2s01.py` le salta con un avviso): non hanno una task dir
né uno schema `draw2gr` da aprire. Le FMU LegoPST della stessa sessione ci sono
regolarmente.

Limiti del master (validi per tutte le FMU):

| Aspetto | Supporto |
|---|---|
| Versione FMI | **2.0** soltanto (usa `fmpy.fmi2.FMU2Slave`); non FMI 3.0 |
| Tipo | **Co-Simulation** soltanto (serve `coSimulation.modelIdentifier`); non Model Exchange |
| Tipi variabile | `Real`, `Integer`, `Boolean`. **`String` non gestito** in `get`/`set` |
| Accoppiamento | Gauss-Seidel a **passo fisso**, nell'ordine di dichiarazione in `lg_cosim.json`; nessuna iterazione sui loop algebrici, nessun rollback (`fmi2GetFMUstate`) |

Esempio con sole FMU standard:

```json
{
  "settings": {"stop_time": 5.0, "step_size": 1.0, "speedup": null,
               "log_file": "log_std.csv", "log_vars": ["A.y", "B.y"]},
  "fmus": { "A": "StdA.fmu", "B": "StdB.fmu" },
  "start_values": { "A.u": 1.0 },
  "connections": [ {"from": "A.y", "to": "B.u"} ]
}
```

## Test in Docker (prova di portabilità, senza LegoPST)

Per verificare che la co-simulazione **accoppiata** giri su una macchina *senza
LegoPST installato*, c'è lo script
[`../scripts/test_cosim_docker.sh`](../scripts/test_cosim_docker.sh): lancia
`lg_cosim.py` + le FMU del config in un container Docker effimero
(`python:3.11-slim`), installando solo `fmpy`.

```bash
../scripts/test_cosim_docker.sh                       # config di default, max velocità
../scripts/test_cosim_docker.sh -s 1.0 -t 60          # tempo reale, 60 s
../scripts/test_cosim_docker.sh -d /path/lg_cosim.json  # altra config + debug FMU
```

- Monta la dir del config **read-only**; copia tutto in una dir effimera del
  container ed estrae lì (l'host non viene sporcato da file di root).
- La FMU (`lg_fmi2.c`) ripristina da sola i bit `+x` dopo l'estrazione via fmpy,
  quindi **non serve LegoPST né chmod**: è il senso del test.
- Ogni FMU prende uno **slot `SHR_USR_KEY`** diverso → SHM separate, nessuna
  collisione. Stampa in coda le ultime righe del log CSV.

A differenza di `test_fmu_docker.sh` (FMU **indipendenti** in parallelo), questo
esegue il **master accoppiato** con lo scambio uscite→ingressi.

---

*Versione: lg_cosim 1.1 — 2026-05-11*  
*Aggiornamenti: slot probe Python-side (ID_SHM_VAR), `LG_COSIM_NO_KILLSIM`, `_patch_killsim_guard`, log_file assoluto.*
