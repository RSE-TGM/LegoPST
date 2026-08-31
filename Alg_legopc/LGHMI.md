# `lghmi` — selettore grafico di HMI di processo e faceplate di comando

`lghmi` apre una finestra da cui si aprono le due interfacce di una simulazione:

- le **pagine di processo**, cioè le task LegoPST, ognuna con la sua HMI di
  supervisione/plot **draw2gr**;
- i **faceplate di comando**, cioè le pagine di stazioni descritte in `r01.dat` e
  compilate in `r02.dat`, visualizzate da **xstaz**.

Senza opzioni mostra **entrambe le liste affiancate**; con `-proc` o `-staz` si
limita a una sola. Sostituisce i comandi manuali:

```bash
cd $HOME/legocad/<task> ; wish $LG_TIX/draw2gr.tcl 1 f22circ   # HMI di processo
cd <dir con r02.dat>   ; xstaz 1 & ; stazpag <PAGINA>          # faceplate
```

File: helper [`Alg_rt/bin/lghmi`](../Alg_rt/bin/lghmi) (nel PATH via profilo) +
selettore Tk [`Alg_legopc/src/tix/lghmi.tcl`](src/tix/lghmi.tcl) (deployato in
`Alg_legopc/bin`).

## Uso

```bash
lghmi              # due liste affiancate: processo + faceplate
lghmi -proc        # solo le pagine di processo (task -> draw2gr)
lghmi -loc         # esplicito, identico al default
lghmi -loc DIR     # usa DIR come dir della simulazione
lghmi -noloc       # NON pre-imposta alcun sim path
lghmi -staz        # solo i faceplate (pagine di r02.dat -> xstaz)
lghmi -h           # aiuto
```

Nella finestra:

- **Per aprire una pagina** ci sono tre strade, tutte sulla voce della lista:
  **doppio-click**, **Invio** sulla voce selezionata, oppure **tasto destro**, che
  apre un popup minuscolo con il solo pulsante *Open page*. Non ci sono pulsanti
  di apertura nella finestra: l'azione sta dove sta l'oggetto, e nel modo a due
  liste questo toglie ogni ambiguità su cosa si sta aprendo.
- Il **tasto destro seleziona prima la voce sotto il cursore**, quindi il popup
  agisce su quella che hai puntato e non sulla selezione precedente. Si chiude
  con **Esc** o con un click fuori dal pulsante; finché è aperto tiene un grab
  locale, così un click altrove lo congeda invece di finire sulla lista.
- L'intestazione di ogni riquadro porta il numero di voci; se una delle due liste
  non ha nulla da mostrare resta vuota, e la riga di stato lo dice per entrambe.
  Il divisorio fra i due riquadri si trascina per dare più spazio all'uno o
  all'altro.
- La finestra parte **680x328**, la stessa larghezza del **banco** (`new_monit`),
  con le due liste di pari larghezza: le due finestre si usano insieme, una sopra
  l'altra, e allineate stanno meglio. Il divisorio resta trascinabile e la
  finestra ridimensionabile (minimo 560x300).
- **Refresh** → rilegge l'elenco delle task.
- **mmi** → lancia l'**applicazione MMI** (`Alg_mmi`), indipendente dalle due
  liste: vedi sotto.
- **Quit** (o **Esc**) → chiude **solo** il selettore. Le HMI già aperte
  **restano vive** (le chiudi tu dalla loro finestra).

## Faceplate di comando (lista di destra, o `-staz`)

La lista di destra (o l'intera finestra con `-staz`) elenca le **pagine di
faceplate** — le stazioni di comando descritte in `r01.dat` e compilate in
`r02.dat` da `compstaz` — e apre quella scelta con **`xstaz`**.

```bash
cd <dir della regolazione>   # quella con r02.dat
lghmi                        # oppure lghmi -staz per la sola lista faceplate
```

Serve soprattutto a chi gestisce la simulazione con **`net_startup`**: quello
script monta il `banco` (da `new_monit`), che **non ha** il dialogo delle
stazioni: quel dialogo esiste solo in `net_monit`, avviato da `net_simula` e
`simula`. Prima di `-staz` le pagine si potevano aprire solo da lì o a mano da
riga di comando con `stazpag`.

**Dove cerca le pagine**, in quest'ordine:

1. la directory corrente, se contiene già un `r02.dat`;
2. in modalità S01, tutte le task del simulatore — **regolazione compresa**
   (`r01.dat`/`r02.dat` vivono nelle task `R`, non nelle `P`);
3. altrimenti le sottodirectory di `$LG_TASKROOT`.

Se le pagine provengono da più directory, il nome della directory compare in
coda a ogni voce. Ogni riga mostra nome pagina, descrizione e numero di
stazioni; l'elenco è letto da `stazpag -m`, che conosce il formato binario di
`r02.dat`.

**Cosa fa all'apertura**: se `xstaz` non è in esecuzione lo avvia
(`xstaz 1`, processo indipendente, cwd = la directory del `r02.dat`; parte
iconificato, con la sola finestrella *Quit*), poi gli manda la richiesta della
pagina con `stazpag`. La richiesta resta in coda finché `xstaz` non la scoda,
quindi non ci sono corse di avvio.

**Un solo `xstaz` per simulazione.** La coda delle richieste
(`SHR_USR_KEY + ID_MSG_STAZ`) è unica: due `xstaz` avviati su `r02.dat` diversi
si ruberebbero i messaggi a vicenda. Se ne trova già uno attivo su un'altra
directory, `lghmi -staz` non ne avvia un secondo e lo segnala, chiedendo di
chiudere quello.

Perché i faceplate mostrino valori veri la simulazione deve essere avviata **e
inizializzata**; il dettaglio del formato `r01.dat` e del ciclo di vita sta in
[Alg_rt/grafica/xstaz/HOWTO_faceplate.md](../Alg_rt/grafica/xstaz/HOWTO_faceplate.md).

## Quali task di processo compaiono — S01 o dir-scan

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

## Il pulsante `mmi`

**Al centro** della barra in basso, largo il doppio degli altri e con lo sfondo
**verde `#50a050`**, lo stesso della finestra dell'MMI: non agisce sulle liste,
lancia l'**MMI** (`Alg_mmi/run_time`, le pagine sinottiche SCADA-like), non una
HMI di task. È centrato con `place -relx 0.5 -anchor center`, quindi sul centro
della finestra e non su quello dello spazio lasciato libero da *Refresh* e *Quit*,
che hanno larghezze diverse.

`mmi` non ha opzioni per dire dove stanno le pagine: legge `Context.ctx` **nella
directory da cui parte** e da lì ricava tutto (vedi
[Alg_mmi/README.md](../Alg_mmi/README.md)). Il pulsante quindi sceglie la
directory di lavoro in quest'ordine:

| # | condizione | comando |
|---|---|---|
| 1 | *Set Sim path* attivo e `$LG_SIM_PATH/globpages` esiste | `cd $LG_SIM_PATH/globpages && mmi &` |
| 2 | altrimenti `$KPAGES` definita ed esistente | `cd $KPAGES && mmi &` |
| 3 | altrimenti, esiste `./globpages` sotto la cwd | `cd ./globpages && mmi &` |
| 4 | altrimenti | `mmi &` (dalla cwd; sarà `mmi` a dire che manca il Context) |

L'ordine mette il **Set Sim path davanti a `$KPAGES`**: se hai lanciato
`lghmi -loc DIR` (o `lghmi` dalla directory di una simulazione) è quello il
simulatore che ti interessa, e può non essere quello selezionato con `ksetsim`.
Con `-noloc` il caso 1 non si applica.

`$KPAGES` è di norma `$KSIM/globpages` e la definisce il profilo LegoPST: il caso
2 è quello normale con un simulatore selezionato (`ksetsim`), il caso 3 serve
quando `lghmi` parte dalla directory di un simulatore senza profilo sorgiato.

**Quando è disabilitato.** Se nella directory scelta non c'è niente da aprire il
pulsante è **grigio-verde e inattivo**, e la riga di stato dice perché. Il
controllo replica le regole di `mmi` invece di limitarsi a contare i file:

1. manca `Context.ctx` nella directory → `mmi` uscirebbe subito
   (*"mmi: nessun Context.ctx in DIR"*);
2. il Context c'è: da esso si leggono `*pages` (dove stanno le pagine, anche
   altrove) e `*page_list` (quali sono), e si contano i `<NOME>.rtf` presenti. Se
   nessuno esiste → *"mmi: nessuna pagina compilata (.rtf) in DIR"*.

Contare i `*.rtf` della directory non basterebbe: la directory di un simulatore
ne contiene altri che pagine non sono (`variabili.rtf`, `recorder.rtf`,
`stato_cr.rtf`) e il Context può dichiarare le pagine in un'altra directory — è
il caso delle `globpages_<id>` generate da `kMmiConfig`, che contengono il solo
Context e puntano alle pagine condivise.

Lo stato è ricalcolato a ogni **Refresh**: se compili le pagine mentre il
selettore è aperto, un Refresh riabilita il pulsante.

**Segnalazione degli errori.** Il lancio è in background, quindi l'esito non torna
da `exec`: il selettore controlla che `mmi` non sia trovabile nel `PATH` (errore
immediato) e, tre secondi dopo il lancio, che sia comparsa davvero una nuova
istanza. Se non c'è, apre un dialogo con la directory usata, il percorso del log e
le **ultime righe del log** con la causa reale — tipicamente:

```
Error on GetFileDatabase Context.ctx - Exit.
```

L'output va in **`/tmp/lghmi_mmi.log`**. Come le HMI, `mmi` parte con `setsid` e
sopravvive al *Quit* del selettore.

> Su un simulatore locale l'MMI va lanciato così (o a mano con `cd $KPAGES && mmi &`),
> **non** con `kMmi`: quella procedura configura la strada client/SCADA e su una
> macchina sola si blocca in timeout. Il perché è in
> [Alg_mmi/README.md](../Alg_mmi/README.md).

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
| `LG_SIM_PATH` | dir sim pre-impostata per *Set Sim path* (la imposta `lghmi`; `-noloc` la omette); il pulsante *mmi* ne prova per primo il `globpages` |
| `LG_TIX` | dir di `draw2gr.tcl`/`lghmi.tcl` (dal profilo LegoPST) |
| `KPAGES` | dir delle pagine MMI usata dal pulsante *mmi* (di norma `$KSIM/globpages`) |

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
- **Il pulsante *mmi* dice "mmi non e' partito"**: le ultime righe del log nel
  dialogo dicono il motivo. `Error on GetFileDatabase Context.ctx` significa che
  la directory scelta non contiene un `Context.ctx`: seleziona un simulatore con
  `ksetsim` (così `$KPAGES` punta alle sue pagine) oppure lancia `lghmi` dalla
  directory del simulatore, che contiene `globpages`.
- **Caratteri strani nelle scritte**: usare solo ASCII negli script Tk lanciati
  così: con `LANG=POSIX` Tcl non decodifica i file come UTF-8 (un em-dash
  comparirebbe come `â` + riquadri).
