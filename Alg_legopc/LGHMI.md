# `lghmi` — selettore grafico delle task che lancia la HMI draw2gr

`lghmi` apre una finestra con l'elenco delle task LegoPST; selezionandone una
lancia la HMI di supervisione/plot **draw2gr** per quella task. Sostituisce il
comando manuale:

```bash
cd $HOME/legocad/<task> ; wish $LG_TIX/draw2gr.tcl 1 f22circ
```

File: helper [`Alg_rt/bin/lghmi`](../Alg_rt/bin/lghmi) (nel PATH via profilo) +
selettore Tk [`Alg_legopc/src/tix/lghmi.tcl`](src/tix/lghmi.tcl) (deployato in
`Alg_legopc/bin`).

## Uso

```bash
lghmi              # apre il selettore (pre-imposta il Set Sim path: vedi sotto)
lghmi -loc         # esplicito, identico al default
lghmi -loc DIR     # usa DIR come dir della simulazione
lghmi -noloc       # NON pre-imposta alcun sim path
lghmi -h           # aiuto
```

Nella finestra:

- **Doppio-click** su una task (oppure selezione + **Launch HMI**, oppure
  **Invio**) → lancia la HMI per quella task.
- **Refresh** → rilegge l'elenco delle task.
- **Quit** (o **Esc**) → chiude **solo** il selettore. Le HMI già aperte
  **restano vive** (le chiudi tu dalla loro finestra).

## Quali task compaiono — due modalità

La sorgente della lista dipende dalla **directory da cui lanci `lghmi`**:

**1. dir-scan (default)** — le sottodirectory di `$LG_TASKROOT` (default
**`$HOME/legocad`**) che contengono almeno un file **`*.tom`**. Di conseguenza:

- le directory di **libreria** (`libgraph`, `libut*`, …) sono escluse (niente
  `.tom`);
- le task di **regolazione** `r_*` sono escluse (usano `config`/`Connessioni.reg`,
  non hanno `.tom`) — per quelle si usa l'applicazione `config`, non draw2gr.

Per usare una directory diversa:

```bash
LG_TASKROOT=/altro/percorso lghmi
```

**2. S01** — se nella cwd esiste un file **`S01`** (descrittore di un simulatore
composto), la lista è letta da lì. Compaiono **solo le task di PROCESSO**
(tipo `P`), le uniche con un `.tom`; l'etichetta è **nome + descrizione**. Ogni
HMI viene lanciata dalla propria **dir modello** (path relativo risolto rispetto
alla dir del `S01`). Vedi [Formato `S01`](#formato-s01) sotto.

## Set Sim path (comportamento di default)

Ogni HMI ha nel menu **View → Set Sim path** la directory della *simulazione in
corso* verso cui puntano animazione (viewval), Plot (graphics) e Command Mode
(xaing). `lghmi` la **pre-imposta da solo**, alla **directory da cui lo lanci**:

```bash
cd /home/antonio/sked/SLaurentB1     # dir della simulazione attiva
lghmi                                # scegli la task: la HMI punta gia' a questa sim
```

Così animazione/Plot/Command funzionano subito, senza *Set Sim path* manuale.
Il selettore mostra in alto (in blu) il path pre-impostato.

| Opzione | Effetto |
|---|---|
| *(nessuna)* / `-loc` | `LG_SIM_PATH=$PWD` — la dir di lancio |
| `-loc DIR` | usa `DIR` (relativo → normalizzato ad assoluto; se non esiste: errore ed exit) |
| `-noloc` | non pre-imposta nulla: ogni HMI parte senza sim path, da impostare a mano |

**In modalità S01 questo è essenziale**, e la distinzione è sottile: la cwd di
lancio è la dir del **simulatore composto in esecuzione** (dati live, SHM,
`f22circ.dat`, `variabili.rtf`), mentre ogni HMI è lanciata dalla sua **dir
modello** (serve il `cd` per caricare lo schema `.tom`). Il Set Sim path deve
restare la dir del **simulatore**, non quella del modello: puntarlo alla dir
modello romperebbe animazione/Plot/Command, che lì non trovano nessun dato live.

Meccanismo: l'helper esporta `LG_SIM_PATH`; `animate.tcl` e `draw2gr.tcl`
inizializzano la variabile `::anima_sim_path` da lì se è una directory valida
(altrimenti resta il default *"click to change"*). Il valore lo puoi comunque
cambiare a mano dal menu.

## Formato `S01`

File testuale (nella dir del simulatore) che descrive un simulatore composto da
più task lego. È diviso in **sezioni separate da righe che iniziano con `****`**
(quattro asterischi in **colonna 1**). Le prime tre sono le uniche usate da
`lghmi` (proc `parse_s01`):

1. **Sez. 1** — una riga: `<nome_simulatore> <descrizione>`.
2. **Sez. 2** — una riga per task: `<nome> <descrizione>`. Numero di task
   **indefinito**.
3. **Sez. 3** — una riga per task, in **associazione posizionale** con la sez. 2:
   `<path_relativo> <tipo>`, dove `tipo` = `P` (Processo) o `R` (Regolazione).
   Le righe `R` possono avere campi aggiuntivi (nome regolazione ecc.), ignorati.

`lghmi` tiene **solo le task `P`**: abbina sez. 2[i] ↔ sez. 3[i], risolve il path
relativo in assoluto (`file normalize` rispetto alla dir del `S01`) e mostra
`nome + descrizione`. Le sezioni successive non sono usate.

## Task dentro un bundle FMU (co-simulazione)

`lghmi` riconosce le task che stanno dentro un **bundle FMU** (`<bundle>/task/<nome>`,
riconosciute dalla presenza di `<bundle>/run_draw2gr.sh`) e per quelle **delega al
`run_draw2gr.sh` di quel bundle** invece di lanciare il `draw2gr.tcl`
dell'installazione LegoPST. Serve perché:

- ogni bundle porta il **proprio** ambiente (wish, `LG_TIX`, runtime Tcl/Tk/Tix,
  `LG_MODELS`), e sulla macchina target LegoPST **non c'è affatto**;
- in co-simulazione ogni FMU è un **simulatore a sé**, con il proprio `net_sked` e
  la propria `SHR_USR_KEY`: `run_draw2gr.sh` la ricava dal `net_sked` di *quella*
  task, quindi ogni pagina punta alla sim giusta.

Per questo il lancio toglie `LG_SIM_PATH` e `SHR_USR_KEY` dall'ambiente
(`env -u`): sono quelle del simulatore da cui è partito `lghmi` e qui sarebbero
sbagliate. Le task LegoPST normali non sono toccate — restano sul percorso
classico.

In co-simulazione l'elenco arriva da un `S01` generato da `lg_cosim2s01.py` a
partire dal `lg_cosim.json`; è `lg_cosim` stesso ad aprire il selettore quando
`settings.hmi` è attivo. Vedi
[Alg_rt/lg_fmu/lg_cosim/lg_cosim_manual.md](../Alg_rt/lg_fmu/lg_cosim/lg_cosim_manual.md).

## Comportamento dei processi

- Ogni HMI è lanciata in un **processo indipendente** (`setsid`), quindi
  sopravvive alla chiusura del selettore.
- L'output di ciascuna HMI va in **`/tmp/lghmi_<task>.log`** (per debug; non
  sporca la directory della task).
- L'helper **sorgia `.profile_legoroot`** da solo se `LG_TIX` non è
  nell'ambiente → `lghmi` funziona da qualsiasi shell. Gli passa `"$LEGOROOT"`
  come `$1`: senza, il profilo erediterebbe i parametri posizionali dell'helper
  (es. `-loc`) e farebbe `export LEGOROOT=$1`.

## Variabili d'ambiente

| Variabile | Effetto |
|---|---|
| `LG_TASKROOT` | directory delle task in modalità dir-scan (default `$HOME/legocad`) |
| `LG_SIM_PATH` | dir sim pre-impostata per *Set Sim path* (la imposta `lghmi`; `-noloc` la omette) |
| `LG_TIX` | dir di `draw2gr.tcl`/`lghmi.tcl` (dal profilo LegoPST) |

## Troubleshooting

- **"LG_TIX non definito … profilo non sorgiato"**: l'ambiente LegoPST non è
  disponibile e l'auto-source è fallito. Lancia da una shell in cui hai sorgiato
  `.profile_legoroot`.
- **Nessuna task in lista**: in dir-scan, nessuna sottodir di `$LG_TASKROOT` ha
  un `*.tom` (controlla `LG_TASKROOT`, che può essere un symlink); in modalità
  S01, il file non ha task di tipo `P`.
- **La HMI si apre ma Plot/Command non trovano i dati**: la simulazione gira in
  un'altra directory → lancia `lghmi` dalla dir della sim, oppure usa *View → Set
  Sim path* nella HMI. Vedi la sezione *Set Sim path* in
  [README.md](README.md#set-sim-path--animazioneplotcommand-su-una-simulazione-in-unaltra-directory).
- **Caratteri strani nelle scritte**: usare solo ASCII negli script Tk lanciati
  così: con `LANG=POSIX` Tcl non decodifica i file come UTF-8 (un em-dash
  comparirebbe come `â` + riquadri).
