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
lghmi           # apre il selettore
lghmi -loc      # come sopra, ma pre-imposta il "Set Sim path" (vedi sotto)
lghmi -h        # aiuto
```

Nella finestra:

- **Doppio-click** su una task (oppure selezione + **Launch HMI**, oppure
  **Invio**) → lancia la HMI per quella task.
- **Refresh** → rilegge l'elenco delle task.
- **Quit** (o **Esc**) → chiude **solo** il selettore. Le HMI già aperte
  **restano vive** (le chiudi tu dalla loro finestra).

## Quali task compaiono

Le sottodirectory di `$LG_TASKROOT` (default **`$HOME/legocad`**) che contengono
almeno un file **`*.tom`**. Di conseguenza:

- le directory di **libreria** (`libgraph`, `libut*`, …) sono escluse (niente
  `.tom`);
- le task di **regolazione** `r_*` sono escluse (usano `config`/`Connessioni.reg`,
  non hanno `.tom`) — per quelle si usa l'applicazione `config`, non draw2gr.

Per usare una directory diversa:

```bash
LG_TASKROOT=/altro/percorso lghmi
```

## L'opzione `-loc` (Set Sim path automatico)

Ogni HMI ha nel menu **View → Set Sim path** la directory della *simulazione in
corso* verso cui puntano animazione (viewval), Plot (graphics) e Command Mode
(xaing). Normalmente la si sceglie a mano in ogni HMI.

Con **`-loc`** questo path viene pre-impostato alla **directory da cui lanci
`lghmi`**:

```bash
cd /home/antonio/sked/SLaurentB1     # dir della simulazione attiva
lghmi -loc                           # scegli la task: la HMI punta gia' a questa sim
```

Così animazione/Plot/Command funzionano subito, senza *Set Sim path* manuale.
Il selettore mostra in alto (in blu) il path pre-impostato.

Meccanismo: l'helper esporta `LG_SIM_PATH=$PWD`; `animate.tcl` e `draw2gr.tcl`
inizializzano la variabile `::anima_sim_path` da `LG_SIM_PATH` se è una directory
valida (altrimenti resta il default *"click to change"*). Il valore lo puoi
comunque cambiare a mano dal menu.

> Nota: `-loc` usa la dir **da cui lanci `lghmi`**, non quella delle task. Se la
> lanci da una directory senza simulazione, le HMI ripiegano sul default (nessun
> danno, ma dovrai impostare *Set Sim path* a mano).

## Comportamento dei processi

- Ogni HMI è lanciata in un **processo indipendente** (`setsid`), quindi
  sopravvive alla chiusura del selettore.
- L'output di ciascuna HMI va in **`/tmp/lghmi_<task>.log`** (per debug; non
  sporca la directory della task).
- L'helper **sorgia `.profile_legoroot`** da solo se `LG_TIX` non è
  nell'ambiente → `lghmi` funziona da qualsiasi shell.

## Variabili d'ambiente

| Variabile | Effetto |
|---|---|
| `LG_TASKROOT` | directory delle task (default `$HOME/legocad`) |
| `LG_SIM_PATH` | dir sim pre-impostata per *Set Sim path* (la imposta `-loc`) |
| `LG_TIX` | dir di `draw2gr.tcl`/`lghmi.tcl` (dal profilo LegoPST) |

## Troubleshooting

- **"LG_TIX non definito … profilo non sorgiato"**: l'ambiente LegoPST non è
  disponibile e l'auto-source è fallito. Lancia da una shell in cui hai sorgiato
  `.profile_legoroot`.
- **Nessuna task in lista**: nessuna sottodir di `$LG_TASKROOT` ha un `*.tom`.
  Controlla `LG_TASKROOT` (default `$HOME/legocad`, che può essere un symlink).
- **La HMI si apre ma Plot/Command non trovano i dati**: la simulazione gira in
  un'altra directory → usa `lghmi -loc` dalla dir della sim, oppure *View → Set
  Sim path* nella HMI. Vedi la sezione *Set Sim path* in `CLAUDE.md`.
- **Caratteri strani nelle scritte**: usare solo ASCII negli script Tk lanciati
  così: con `LANG=POSIX` Tcl non decodifica i file come UTF-8.
