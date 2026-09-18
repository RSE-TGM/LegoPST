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

> **L'interfaccia è in inglese** — etichette, menù, messaggi, conferme e riga di
> stato. Qui sotto le voci sono citate con il testo che si legge a schermo. I
> commenti del sorgente e questo documento restano in italiano.

## Uso

```bash
lghmi              # processo + faceplate affiancati, regolazioni sotto
lghmi -proc        # solo le pagine di processo (task -> draw2gr)
lghmi -staz        # solo i faceplate (pagine di r02.dat -> xstaz)
lghmi -reg         # riaccende le regolazioni insieme a -proc/-staz
lghmi -noreg       # nasconde il riquadro delle task di regolazione
lghmi -loc         # esplicito, identico al default
lghmi -loc DIR     # usa DIR come dir della simulazione
lghmi -noloc       # NON pre-imposta alcun sim path
lghmi -insim       # lanciato da dentro una simulazione (lo passa il banco)
lghmi -noedit      # le HMI lanciate non hanno il menu Edit (legopc)
lghmi -h           # aiuto
```

Nella finestra:

- **Per aprire una pagina** ci sono tre strade, tutte sulla voce della lista:
  **doppio-click**, **Invio** sulla voce selezionata, oppure **tasto destro**, che
  apre un popup minuscolo con il solo pulsante *Open page*. Non ci sono pulsanti
  di apertura nella finestra: l'azione sta dove sta l'oggetto, e nel modo a due
  liste questo toglie ogni ambiguità su cosa si sta aprendo.
- **Come si apre lo dice un balloon**, non una riga di testo: fermandosi sul
  titolo di un riquadro (*Process pages*, *xstaz faceplates*) compare
  *«Double-click an entry to open it, or right-click → Open page»*. Il riquadro
  *Regulation tasks* ha il **suo** testo — *«Double-click a task to edit its
  regulation with config»* — e il suo popup dice *Edit regulation*, non *Open
  page*: lì il doppio clic non apre una pagina, lancia l'editor della
  regolazione. Prima erano
  due righe fisse in cima alla finestra — una descriveva la disposizione delle
  liste, l'altra il modo di aprire una voce — che occupavano spazio a ogni
  avvio per dire una cosa che serve una volta sola. Il suggerimento lo dà
  `set_balloon` di [src/tix/balloon.tcl](src/tix/balloon.tcl), lo stesso di
  `draw2gr`, sorgiato dentro un `catch`: se manca si perdono i suggerimenti, non
  il selettore.
- Il **tasto destro seleziona prima la voce sotto il cursore**, quindi il popup
  agisce su quella che hai puntato e non sulla selezione precedente. Si chiude
  con **Esc** o con un click fuori dal pulsante; finché è aperto tiene un grab
  locale, così un click altrove lo congeda invece di finire sulla lista.
- **Ogni riquadro ha il suo titolo** — *Process pages* e *xstaz faceplates* —
  anche in modalità a lista singola, dove prima non c'era perché ci pensavano le
  due righe di intestazione. Nel modo a due liste il titolo porta anche il numero
  di voci; se una delle due liste non ha nulla da mostrare resta vuota, e la riga
  di stato lo dice per entrambe.
  Il divisorio fra i due riquadri si trascina per dare più spazio all'uno o
  all'altro.
- La finestra parte **680x328**, la stessa larghezza del **banco** (`new_monit`),
  con le due liste di pari larghezza: le due finestre si usano insieme, una sopra
  l'altra, e allineate stanno meglio. Il divisorio resta trascinabile e la
  finestra ridimensionabile (minimo 560x300).
- **L'area di lavoro corrente** (`legopst_<nome>`) sta nel **titolo** e nella
  **prima riga** in alto, rossa se i link `~/legocad` e `~/sked` non indicano
  un'area sola.
- **File → Work area ▸** → cambia l'**area di lavoro** (i link `~/legocad` e
  `~/sked`) con `lgswitch`, dopo aver controllato che niente ci lavori ancora:
  [vedi sotto](#cambiare-area-di-lavoro-menu-file-work-area).
- **File → Open Simulator path…** → cambia la **directory di lavoro** del selettore,
  e sotto la voce ci sono le ultime directory usate nell'area corrente: vedi
  sotto.
- **File → Simulation log** → **riapre** la finestra di log di `net_startup`, che
  altrimenti la X chiude per sempre: vedi sotto.
- **File → Logs ▸** → gli **altri log** che `lghmi` scrive in `/tmp`: uno per ogni
  HMI lanciata, più `mmi` e `xstaz`: vedi sotto.
- **Tools** → aggiorna la configurazione del **simulatore corrente** con
  `kUpSim`, permette di cambiare simulatore e di aprire il modello della task
  selezionata nel CAD (`legopc`): vedi sotto.
- **?** → versione di LegoPST e documentazione dell'ambiente: vedi sotto.
- **Refresh** → rilegge l'elenco delle task e l'area di lavoro (un `lgswitch`
  fatto da terminale si vede qui).
- **net_startup** → lancia la **simulazione** nella directory corrente e ne
  mostra l'output in una finestra di log. È abilitato solo dove si può: vedi
  sotto.
- **mmi** → lancia l'**applicazione MMI** (`Alg_mmi`), indipendente dalle due
  liste: vedi sotto.
- **Quit** (o **Esc**) → chiude **solo** il selettore. Le HMI già aperte
  **restano vive** (le chiudi tu dalla loro finestra), e così la simulazione: con
  *Quit* se ne va anche la finestra di log, ma il log resta in `/tmp`.

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

**Anche senza simulazione** (`net_sked` assente) la pagina si apre, con i
valori fermi, e la riga di stato lo dice (*No simulation running: the values
are not live.*): è quello che serve mentre si costruiscono e si configurano le
stazioni. Funziona perché:

- `xstaz` non trova il DB punti condiviso (`RtCreateDbPunti` ritorna NULL: la
  chiave dell'header non c'è) e va avanti lo stesso;
- la coda delle richieste la **crea `xstaz`** (`msg_create_fam`), non solo
  `net_sked`. L'unica attesa è quella: appena lanciato, `xstaz` la crea qualche
  istante dopo, e `stazpag` che arriva prima non la trova (esce con 5).
  `staz_apri` allora riprova per al massimo 3 secondi, solo se `xstaz` l'ha
  appena avviato lui;
- un `xstaz` avviato così **non si aggancia più** alla simulazione, ma non ci
  arriva: `net_startup`, `net_simula` e `simula` cominciano con `killsim`, che
  lo chiude. (Eccezione: in co-simulazione con `LG_COSIM_NO_KILLSIM=1` il
  `killsim` non c'è, e un `xstaz` aperto prima va chiuso a mano.)

`xstaz` legge `SHR_USR_KEY` con `atoi(getenv(...))` senza controllarla: se la
variabile manca `staz_apri` non lo lancia e dice di sorgiare il profilo.

L'apertura non è scritta in `lghmi.tcl` ma in
[src/tix/lgstaz.tcl](src/tix/lgstaz.tcl) (`staz_apri`, con `parse_s01`,
`pagine_di` e `xstaz_attivo`), perché la usano anche i **bottoni faceplate**
delle pagine di legopc e draw2gr: vedi
[README.md](README.md#elementi-operatore-delle-pagine-faceplate-e-set-value).

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

## Lanciato dal banco — l'opzione `-insim`

`lghmi` non si lancia solo a mano: lo apre anche il **banco** (`new_monit`) da
una voce del suo menù — [`attiva_lghmi`](../Alg_rt/net_simula/new_monit/cont_rec.c)
fa `system("$LEGORT_BIN/lghmi -insim &")` — e il banco gira nella directory del
simulatore, perché lo avvia `net_startup`.

In quel caso il selettore **appartiene a quella simulazione**, e quattro comandi
vengono **disabilitati**:

| comando | perché |
|---|---|
| *File → Open Simulator path* | lo porterebbe su un'altra directory, scollegandolo dalla simulazione che l'ha aperto |
| pulsante *net_startup* | comincia con `killsim`: ammazzerebbe proprio la simulazione da cui è stato lanciato, e il banco con lei |
| menu *Edit* delle HMI | le HMI lanciate da qui partono **senza** `-edit`: la simulazione è in corso, e il modello non va toccato (vedi [il menu Edit delle HMI](#il-menu-edit-delle-hmi-draw2gr--edit)) |
| *File → Work area* | cambierebbe l'area sotto la simulazione in corso (vedi [Work area](#cambiare-area-di-lavoro-menu-file-work-area)) |

Le voci di menù nascono disabilitate e il pulsante resta spento; la riga di
stato dice *"started from the desk: fixed directory, simulation already
running"* e il titolo della finestra porta `(from the desk)`, così si capisce da
dove viene.

Lanciando `lghmi` a mano l'opzione non serve: i comandi restano tutti
disponibili, `net_startup` chiede comunque conferma, e *Work area* si rifiuta da
sé se trova una simulazione in corso.

## `File → Open Simulator path…` — cambiare simulazione senza riavviare

Apre un selettore di directory e **porta lì il selettore**: da quella directory
dipendono la modalità (l'`S01` si cerca nella directory corrente), la lista dei
faceplate (`r02.dat` della directory), la directory di lavoro dell'`mmi` e il
**Set Sim path** che le HMI ereditano. Le liste si aggiornano subito.

È l'equivalente di **rilanciare `lghmi` da quella directory**, e serve quando si
passa da una simulazione a un'altra: prima bisognava chiudere il selettore,
`cd`, e riaprirlo.

Il nome dice cosa si sceglie: la directory del **simulatore**, che diventa anche
il *Set Sim path* mostrato in alto. Fino a settembre 2026 la voce si chiamava
*Open loc path*, dall'opzione `-loc` a cui somiglia (ma non è la stessa cosa:
vedi sotto).

### I path recenti

Sotto *Open Simulator path…* il menù File porta le **ultime 3 directory usate
nell'area di lavoro corrente**, così per tornare su una simulazione già visitata
non serve riaprire il dialogo di selezione: si clicca la voce.

- La lista sta in **`~/.lghmi_recent`**, una riga per path. Non in
  `$LG_ENTRY/legopc_prefs.tcl` come le preferenze di `legopc`, perché
  attraversa le installazioni: la radice utente cambia proprio quando si cambia
  directory.
- **Il menù mostra solo i path dell'area corrente** (vedi
  [*Work area*](#cambiare-area-di-lavoro-menu-file-work-area)): un path che sta in
  un'altra `legopst_*` non compare, mentre quelli fuori da qualunque area si
  vedono sempre. Il file ne tiene fino a 30 (`MAXRECENTIFILE`), di tutte le
  aree: tornando a un'area si ritrovano i suoi.
- Le voci mostrano il path con **`~`** al posto della home, e la più recente
  sta in cima. Riaprire una directory già in lista la **promuove** senza
  duplicarla.
- Le directory che non esistono più (una simulazione cancellata, un disco
  smontato) **scompaiono dal menù** al primo avvio successivo: restano nel file
  ma non vengono mostrate.
- **La directory di lancio entra in lista da sé**, ma solo se è una directory di
  simulazione (c'è un `S01` o `variabili.rtf`): lanciando `lghmi` da casa, in
  dir-scan, non ha senso ricordarsela. Così il menù è utile dalla prima volta,
  senza dover passare almeno una volta dal dialogo.
- Il numero di path mostrati è la costante `MAXRECENTI` in `lghmi.tcl`.

Con `-insim` la voce *Open Simulator path* **e tutti i path recenti** sono
disabilitati: vedi sopra.

> **Non è la stessa cosa di `-loc DIR`.** L'opzione della riga di comando
> imposta soltanto `LG_SIM_PATH` e lascia la directory di lavoro dov'era, quindi
> l'`S01` e i faceplate continuano a essere cercati nella directory di lancio.
> La voce di menu fa entrambe le cose.

Le due intestazioni in alto — il simulatore `S01` (verde) e il *Set Sim path*
(blu) — compaiono e spariscono da sé secondo quello che c'è nella nuova
directory. Per questo esistono sempre come widget, anche vuote: creandole solo
all'avvio, aprendo una directory con `S01` da una sessione partita in dir-scan
non ci sarebbe niente da riempire.

## Cambiare area di lavoro (menu File, Work area)

È [`lgswitch`](../docs/COMANDI_LG.md#5-scelta-dellarea-di-lavoro) dentro il
selettore. Un'**area di lavoro** è una directory `legopst_<nome>` con dentro
`legocad` e `sked`; quella corrente la scelgono i due link `~/legocad` e
`~/sked`. Il sottomenù elenca le aree, con la corrente spuntata; quelle a cui
manca `legocad` o `sked` si vedono ma sono **spente**, con il motivo accanto al
nome. L'area corrente sta anche **nel titolo** (`[legopst_nuclear]`) e nella
**prima riga dell'intestazione**, che diventa rossa quando i link non indicano
un'area sola (link misti, directory vere, link mancanti).

Il sottomenù si ricostruisce a ogni apertura, e l'intestazione a ogni *Refresh*:
un `lgswitch` fatto in un terminale si vede senza riavviare. Ma solo lì: il
simulatore corrente e la directory di lavoro del selettore restano quelli di
prima, quindi dopo un `lgswitch` da terminale conviene riaprire `lghmi`.

### Perché lo switch è così prudente

Tutto il profilo raggiunge l'area **attraverso i link**: `LG_ENTRY=$HOME/legocad`,
`LG_LIBGRAPH`, `LG_MODELS`, `KSKED=$HOME/sked`, `KSIM=$KSKED/<nome>`, `KPAGES`,
e il `PATH` con `$HOME/legocad/libut_bin`. Cambiare i link cambia quindi l'area
anche ai **processi già aperti**, che però restano con la directory corrente
nella vecchia: un `legopc` continuerebbe a editare un modello della vecchia area
risolvendone i blocchi contro il `libgraph` della nuova. Nessun errore, solo
risultati sbagliati.

Per questo lo switch **si rifiuta**, elencando pid, nome e directory, se trova:

| cosa | dove |
|---|---|
| un processo qualsiasi dell'utente con la directory corrente **dentro l'area corrente** (HMI `draw2gr`, `config`, `mmi`, `xstaz`, compilazioni, editor…) | nell'area |
| la simulazione (`dispatcher`, `net_sked`, `banco`) | ovunque |
| un `legopc` (anche aperto vuoto, legge `libgraph` attraverso `~/legocad`) | ovunque |
| un altro `lghmi` (resterebbe con l'area vecchia in memoria) | ovunque |

Le **shell interattive** con la directory corrente nell'area (`bash`, `ksh`… con
soli argomenti-opzione, senza `-c` né uno script) **non bloccano**: la conferma le
elenca e avvisa che da lì in poi i loro path portano alla nuova area. Una shell
con `-c` sta eseguendo qualcosa, e blocca. La conferma ricorda anche che **le
shell già aperte** tengono il loro `KSIM`, che ora nomina una directory della
nuova area: meglio aprirne di nuove.

Prima di procedere la conferma dice anche se `~/legocad` o `~/sked` sono
**directory vere**: in quel caso `lgswitch` le **rinomina** in
`<nome>.prelink-<data>-<ora>`, senza cancellare niente. Se uno dei due esiste e
non è né un link né una directory, lo switch si rifiuta e lo si sistema a mano.

### Come avviene

Lo switch non è riscritto in Tcl: `lghmi` chiede a `lgswitch --list` cosa c'è
(aree, stato dei link) e poi esegue **`lgswitch -f <area>`** nella directory dei
link. L'output va in `/tmp/lghmi_lgswitch.log`, riapribile da *File → Logs*; se
`lgswitch` fallisce, o se alla fine l'area non è quella scelta, un dialogo mostra
la coda del log. La regola su cosa è un'area, le copie `.prelink-*` e il rifiuto
delle aree incomplete restano scritti in un posto solo, `lgswitch`.

La voce è **spenta** — con il motivo nel sottomenù — se:

- `LG_ENTRY` e `KSKED` non finiscono in `legocad` e `sked`, o non stanno nella
  **stessa directory** (`lgswitch` crea entrambi i link nella directory
  corrente);
- `lgswitch` non si trova (si cerca in `$UTIL97/bin`, poi in
  `$LEGOROOT/util97/bin`, poi nel `PATH`);
- `lghmi` è stato lanciato con `-insim`: la simulazione gira.

### Dopo lo switch

Il selettore riparte **in dir-scan**, dalla directory dei link (`~`) e **senza
Set Sim path**: la directory da cui lavorava, il suo `S01` e il suo sim path
erano della vecchia area. Le liste mostrano le task della nuova area.

Il **simulatore corrente** diventa l'ultimo usato **in quella area**, se esiste
ancora; altrimenti vale la cascata del profilo (`~/.legosim`, `cassano0`, il
primo di `ksims`). La scelta si scrive in `~/.legosim`, perché le shell future
trovino un simulatore che esiste. La memoria per area sta in
**`~/.lghmi_areas`** (una riga `<directory fisica dell'area>|<simulatore>`), e
si aggiorna quando si sceglie un simulatore da `Tools → Current simulator` e
quando si lascia un'area. Sta nella home, come `~/.legosim`, e non dentro le
aree, che si copiano e si impacchettano. Se la nuova area non ha simulatori, le
voci di `Tools` si spengono e la riga di stato lo dice.

## Il pulsante `net_startup` — lanciare la simulazione

Lancia `net_startup` nella **directory corrente** e ne mostra l'output in una
**finestra di log** del selettore, così si vedono scorrere i suoi controlli e si
legge il motivo di un eventuale fallimento: `net_startup` verifica
`variabili.rtf`, la connessione all'X server (`xhost`) e la licenza
(`check_license algrt`), e su ognuno può fermarsi. Il log è anche un file,
`/tmp/lghmi_net_startup.log`, che resta leggibile dopo che la finestra è stata
chiusa.

**Il pulsante è abilitato solo se nella directory corrente esiste
`variabili.rtf`**, che è il file che `net_startup` controlla per primo e senza
il quale non fa nulla. Sta anche nelle directory delle task singole
(`legocad/GTS`), che sono lanciabili come i simulatori composti — per questo la
condizione non è la presenza di `S01`. Lo stato si aggiorna a ogni *Refresh* e
a ogni cambio di directory.

> **Chiede sempre conferma, e c'è un buon motivo.** La prima cosa che
> `net_startup` fa è **`killsim`**, che su Linux cancella *tutte* le SHM, le
> code e i semafori dell'utente, senza filtrare per chiave: se una simulazione è
> in corso la ferma, e con essa le HMI che le stanno sopra. Se il selettore
> trova `dispatcher`, `net_sked` o `banco` già in esecuzione lo dice
> esplicitamente nel testo della conferma.

### Perché il log non è un terminale

La simulazione parte in una **sessione propria** (`setsid`), staccata dalla
finestra che la mostra. Non è un dettaglio di stile: `net_startup` lancia
`dispatcher`, `net_sked` e `banco` con `&` da una `ksh` non interattiva, quindi
**senza job control restano tutti nel process group di chi li ha lanciati**.
Dentro un terminale quel process group prende un `SIGHUP` ogni volta che il
terminale se ne va, e nessuno dei tre binari lo ignora: morivano tutti e tre, e
con loro le HMI aperte. Succedeva in **due** modi, non uno:

| gesto | cosa succedeva |
|---|---|
| **X** della finestra del terminale | `xterm` manda `SIGHUP` al process group del figlio |
| **Invio** al prompt *«press Enter to close»* | esce il session leader, e il kernel manda `SIGHUP` al foreground process group |

Il secondo è il peggiore: era il messaggio stesso a invitare a farlo. Un
terminale non poteva nemmeno chiedere conferma — `xterm` **non ha alcun hook**
sulla richiesta di chiusura del window manager (`WM_DELETE_WINDOW`). La finestra
di log invece è del selettore, quindi la X passa da Tk e si può avvisare prima di
chiudere.

> **Conseguenza da ricordare**: la simulazione lanciata da qui **non ha un
> terminale di controllo**, quindi non compare in `ps -a`, che elenca solo i
> processi legati a un tty. Chi deve sapere se un processo della simulazione è
> vivo usa `ps -e`/`ps -A` o `pgrep`. Su questo scoglio si era arenato *Show
> Value*: `viewval` cercava `net_sked` con `ps -ao ucomm` e dichiarava la
> simulazione spenta (corretto il 2026-09-10, vedi
> [viewval/README.md](../Alg_rt/net_simula/viewval/README.md)).

### La finestra di log

- **X**, **Close** o **Esc** → se la simulazione è in corso, chiedono conferma
  ricordando che **chiudere la finestra NON la ferma**, elencando i processi che
  restano vivi, dicendo come fermarli — prima la via normale (`Simulator Shutdown
  ...` dal banco), poi l'emergenza — e **come riaprire questa finestra**. Se non
  c'è nulla in esecuzione, si chiude senza domande.
- **Si riapre da `File → Simulation log`.** Chiudere la finestra non ferma la
  simulazione — è il punto di tutto il meccanismo — ma fino a prima la chiudeva
  *per sempre*: il log restava solo nel file in `/tmp` e, con la finestra, se ne
  andava l'unico modo grafico di fermare la simulazione **in emergenza**, cioè
  il pulsante *Kill simulation*. Alla riapertura il log **ricompare per intero**, perché il
  visore rilegge il file da capo (`SIMLOG_POS` a 0), e i due cicli di
  aggiornamento ripartono in una nuova generazione.

  Il banner in testa **non** dice *«Simulation started»*, che su una riapertura
  sarebbe falso: dice che il log è stato riaperto e, se lo sappiamo, da quale
  directory la simulazione era partita.

  | Stato | Voce di menù |
  |---|---|
  | c'è un log in `/tmp`, o una simulazione viva | **attiva** |
  | né log né simulazione | spenta |
  | `-insim` | spenta, come il resto del menù File |

  La voce è accesa anche **senza log** quando una simulazione gira, perché la
  finestra serve comunque: è da lì che si preme *Kill simulation*. Con `-insim`
  resta spenta perché il selettore appartiene a una simulazione che non ha
  lanciato lui: il log in `/tmp` non è il suo, e *Kill simulation* ammazzerebbe
  proprio la simulazione da cui `lghmi` è stato aperto.

  Lo stato è ricalcolato a ogni apertura del menù (`-postcommand`), non alla sua
  costruzione: il menù File si ricostruisce di rado — solo quando cambiano i path
  recenti — mentre il log compare e la simulazione parte e si ferma in qualsiasi
  momento.
- **Kill simulation** → **è l'uscita di emergenza, non lo stop normale.** Forza
  la fine della simulazione in modo brutale, ed esiste per quando la via ordinata
  non è più percorribile: banco morto o piantato, finestra persa, processi
  rimasti appesi.

  > **Lo stop normale non si dà da qui.** Si dà dalla finestra che `net_startup`
  > apre — il **banco** — con la voce **`Simulator Shutdown ...`** del suo *Master
  > Menu*, che chiude la simulazione in modo ordinato. (`new_monit/messaggi.h`,
  > `ShutdownLabel`; il banco è l'eseguibile prodotto da `new_monit`.)

  Il pulsante esegue `killsim`, cioè lo stesso comando con cui `net_startup`
  comincia. Chiede conferma ricordando qual è la via normale, e che ammazza anche
  le HMI e i faceplate aperti e cancella *tutte* le SHM, le code e i semafori
  dell'utente — su Linux `killsim` non filtra per chiave. È acceso solo quando
  c'è qualcosa da fermare.

  Il nome dice il mestiere: si chiamava *Stop simulation*, che lo faceva sembrare
  lo spegnimento previsto e invitava a usarlo al posto di `Simulator Shutdown ...`.
- In basso a sinistra lo **stato**: quali fra `dispatcher`, `net_sked` e `banco`
  sono vivi, riletto ogni 3 secondi.
- Il visore è **uno solo**: un secondo `net_startup` riparte da capo nella stessa
  finestra, come il log.
- La **directory** dell'ultimo lancio sta in `SIMLOG_DIR`, e serve solo al titolo
  della finestra: il file di log non la contiene (`net_startup` comincia
  direttamente con i suoi controlli). Se `lghmi` è stato riavviato nel frattempo,
  il log in `/tmp` c'è ancora ma la directory no, e il titolo lo dice invece di
  inventarsela: *`Simulation log - /tmp/lghmi_net_startup.log`*.

Con `-insim` il pulsante è sempre spento, qualunque cosa ci sia nella
directory: vedi sopra.

## `File → Logs ▸` — gli altri log di lghmi

Tutto quello che `lghmi` lancia parte **in background e staccato** (`setsid`),
quindi il suo output non ha nessun terminale dove finire: va in un file in
`/tmp`, che fino a prima era l'unico posto dove leggerlo — sapendo che esisteva e
andandoselo a cercare a mano.

| File | Chi lo scrive | Cosa c'è dentro |
|---|---|---|
| `lghmi_net_startup.log` | `lancia_net_startup` | i controlli di `net_startup` — ha la **voce sua** |
| `lghmi_<task>.log` | `launch_hmi`, uno per HMI | `loadf01`, caricamento di `.tom`/F01/F14 |
| `lghmi_mmi.log` | `launch_mmi` | font mancanti, apertura del `Context.ctx` |
| `lghmi_xstaz.log` | `staz_apri` (`lgstaz.tcl`): la lista faceplate e i bottoni faceplate delle pagine | banner e versione di `xstaz` |
| `lghmi_lgswitch.log` | *File → Work area* | l'output di `lgswitch -f`, con le copie `.prelink-*` |

Tutti sono aperti con `>`, quindi ognuno è sempre **l'ultima esecuzione** di
quella cosa, non uno storico.

`net_startup` **non** compare nel sottomenù: la sua finestra non è solo un
visore — ha la riga di stato dei processi e il pulsante che li ferma — e resta
una voce di primo livello, sopra.

**L'elenco si fa con una `glob` su `/tmp/lghmi_*.log`**, non con un registro
popolato nei quattro punti di lancio. Così si vedono anche i log di una sessione
**precedente** di `lghmi` (lanci una HMI, esci, riapri), non c'è stato da tenere
sincronizzato con chi lancia cosa, e l'ordine — dal più recente — lo dà il
`mtime`. L'etichetta è una **funzione pura del nome del file** (`etichetta_log`),
quindi non serve ricordarsi chi ha lanciato cosa:

```
Logs ▸
  HMI: NPS              (286 B, 30 min ago)
  xstaz (faceplates)    (10 KB, 34 min ago)
  HMI: SSS              (286 B, 23 h ago)
  mmi                   (1 KB, 1 d ago)
```

Dimensione ed età distinguono a colpo d'occhio il log di adesso da quello di
ieri, e un log vuoto da uno che ha qualcosa da dire.

> **Il filtro sul proprietario non è pedanteria.** I nomi in `/tmp` sono
> **fissi**, quindi su una macchina con più utenti un `lghmi_mmi.log` può essere
> di un altro: `elenco_log` scarta con `file owned` quello che non è nostro. È la
> stessa ragione per cui `lancia_net_startup` apre il suo log in scrittura prima
> di partire, invece di darlo per suo.

Con `-insim` il sottomenù **resta acceso**, a differenza del resto del menù File:
leggere il log di una HMI non tocca niente, e queste finestre non hanno nessun
pulsante che ferma nulla.

### Il visore, uno per log

Il visore non è più un singleton. Lo stato — posizione già letta, generazione dei
cicli `after`, file seguito, presenza dei controlli della simulazione — sta in
array indicizzati **per finestra** (`LOGPOS`, `LOGGEN`, `LOGFILE`, `LOGCONSIM`),
così più log restano aperti insieme senza pestarsi i piedi.

- **Una finestra per file**: riaprire lo stesso log riporta davanti la sua,
  invece di accumularne due sullo stesso contenuto (`LOGWIN`).
- Il path del toplevel è un **progressivo** (`.log1`, `.log2`) e non il nome della
  task: quello può contenere punti e spazi, che Tk non accetta nei path dei
  widget. La finestra della simulazione resta `.simlog`.
- **Due sapori di finestra**, decisi dal flag `consim`: solo quella della
  simulazione ha la riga di stato dei processi e *Kill simulation*, e solo lei
  chiede conferma quando la chiudi. Il log di una HMI si chiude e basta — non
  lascia acceso niente — e non deve offrire un pulsante che fa `killsim` su una
  simulazione che non è la sua.
- Per lo stesso motivo il ciclo dello **stato dei processi** (tre `pgrep` ogni 3
  secondi) gira **solo** su `.simlog`. Quello del log, che è un `file size` ogni
  mezzo secondo, gira su tutte.
- **Log molto grandi**: riaprendo si rilegge da capo, quindi oltre `LOGMAX`
  (512 KB) si parte dalla coda e lo si dice in testa — *`--- showing the last
  512 KB of 2.3 MB ---`*. La prima riga può risultare tagliata a metà: è il
  prezzo di non dover leggere il file due volte per trovare un a capo. La soglia
  è larga apposta (i log veri stanno sotto i 100 KB): serve come protezione, non
  come politica.

## Menù `Tools` — configurazione del simulatore, e modifica dei modelli

Le prime tre voci lanciano **`kUpSim`** sul **simulatore corrente** (`$KSIM`):

| voce | cosa fa |
|---|---|
| `kUpSim - realign the configuration of <nome>` | la catena completa |
| `kUpSim -nommi - without the MMI faceplate pages` | salta `kStazPages`, `kWinContext`, `kCompileSim` |
| `kUpSim -n - preview: show the steps without running them` | prova a vuoto |

Il nome del simulatore sta **nell'etichetta della prima voce**, così si sa su
cosa si sta per agire senza aprire nulla. Se `$KSIM` non è definita o non è una
directory, le tre voci sono disabilitate.

> `lgupsim` è un **alias** di `kUpSim` in `Alg_env.sh` (e `lgupsimx` di
> `kUpSim -nommi`). Gli alias non esistono nelle shell non interattive: qui si
> chiama `kUpSim`.

**La conferma dice cosa succede e su quale simulatore**: nome, path e i sette
passi in sequenza (`kConnex` → `kNetCompi` → `kCompStaz` → `kStazPages` →
`kWinContext` → `kCompileSim` → `kCollect`). Con `-nommi` l'elenco mostra che i
tre passi MMI vengono saltati. Se `dispatcher`, `net_sked` o `banco` sono in
esecuzione, la conferma avverte che la simulazione **sta usando**
`variabili.rtf`, `r02.dat` e le pagine, e che le troverebbe cambiate sotto.
L'anteprima `-n` non chiede conferma: non esegue niente.

**L'output va nel visore di log, non in un terminale.** Il terminale sembrava la
scelta ovvia per un comando batch, ma **perde l'output**: chiusa la finestra non
resta niente, e di una compilazione si vogliono poter rileggere gli errori. Il
visore tiene il file in `/tmp/lghmi_kupsim_<simulatore>.log`, lo segue dal vivo,
si riapre da *File → Logs* e non dipende da `$LG_XTERM` — che su una macchina
senza `xterm` non c'è. Lo stesso vale per `config -c compreg` e `-c creatask`.

> Il nome del file **deve** stare nella forma `lghmi_*.log`: `elenco_log` fa la
> glob su quel modello, e così la voce compare da sola nel sottomenu *Logs*,
> senza una riga di codice in più.

### `Tools → Edit model (legopc)`

Apre il **CAD** sul modello della task selezionata, oppure vuoto se non c'è
selezione. I controlli e il lancio stanno in
[`src/tix/lgedit.tcl`](src/tix/lgedit.tcl) (`modifica_task`), condivisi con il
[menu *Edit* delle HMI](#il-menu-edit-delle-hmi-draw2gr--edit): le due strade
rifiutano negli stessi casi e con le stesse parole. `lghmi` lo sorgia sempre,
dalla propria directory, e senza non parte — contiene anche `sim_attiva` e
`stessa_directory`, che il selettore usa altrove. È la stessa cosa che fa l'alias `lgpc`, ma **l'alias non si può
lanciare**: gli alias non esistono nelle shell non interattive, e dietro `lgpc`
non c'è nemmeno un eseguibile — è `export LG_TIX=$LG_BIN; wish
$LG_TIX/legopc.tix`. Qui si lancia `wish` su `legopc.tix` **ereditando
`LG_TIX`**, così il CAD e le HMI vengono dalla stessa installazione: quella con
cui `lghmi` è stato avviato.

Il lancio è un **processo indipendente** (`setsid`), come per le HMI: chiudere
il selettore non porta via il CAD con dentro il lavoro non salvato. L'output va
in `/tmp/lghmi_legopc_<task>.log`.

`legopc.tix` del suo argomento tiene **solo il basename** e lo risolve sulla
directory corrente: per questo si fa `cd` nella task e si passa il nome nudo,
esattamente come per `draw2gr`.

**Il modello di una task è uno solo, e porta il nome della sua directory**
(`<task>/<task>.tom`). Altri `.tom` nella stessa directory non sono alternative:
sono un'anomalia, e la voce lo dice invece di sceglierne uno a caso.

La voce è **sempre attiva**. I rifiuti avvengono al momento del clic, perché una
voce spenta non può spiegarsi:

| situazione | cosa succede |
|---|---|
| nessuna selezione | apre `legopc` vuoto — **anche a simulazione in corso**: non sta editando niente |
| task selezionata, simulazione in corso | **rifiuta**, e offre di aprire `legopc` vuoto |
| task di un'altra area di lavoro | **rifiuta**, nominando le due aree e indicando *File → Work area* (o `lgswitch` da terminale) |
| task di regolazione (nessun `.tom`) | lo dice: si costruiscono dai `.sed`/`.dxf`, non si aprono nel CAD |
| manca il `.tom` omonimo, ma ce ne sono altri | lo dice, elencandoli come anomalia da correggere |
| `legopc` già aperto sulla task | **rifiuta**, indicando pid e directory del CAD già aperto |

> **Perché il blocco della doppia apertura.** Due CAD sullo stesso modello si
> sovrascriverebbero i salvataggi a vicenda, senza che nessuno dei due lo sappia.
> Un `legopc` è "sulla task" se la sua **directory corrente** è quella della task
> (confronto per identità, come per l'area): chi lo lancia su una task ci fa `cd`
> prima — `lghmi`, `draw2gr`, `lgpc` da un terminale — e `legopc` stesso ci si
> porta quando apre un modello (`apri_modello`). Così si trova anche il `legopc`
> partito vuoto che ha poi fatto *Open Model* sulla task. Contano solo i processi
> che **eseguono** `legopc.tix` (un `wish` con quel file fra gli argomenti), non
> un editor o una shell che lo nominano. Per i pochi istanti fra il lancio e la
> comparsa del processo vale un blocco interno: un secondo clic subito dopo il
> primo è rifiutato lo stesso.

> **Perché il blocco a simulazione viva.** Salvare da `legopc` riscrive `.tom` e
> `.i5` mentre la task gira: l'eseguibile in `proc/` e il layout della SHM non
> corrisponderebbero più a quel che è disegnato, e le HMI `draw2gr` già aperte
> leggerebbero file che cambiano sotto. Il blocco però **non impedisce** di
> modificare: impedisce a `lghmi` di *porgere* la task già aperta. Da `legopc`
> vuoto ci si arriva lo stesso con *File → Open Model*, che però **avvisa**
> (senza bloccare) se la simulazione di quel modello è in corso o se un altro
> `legopc` ce l'ha già aperto: vedi *File → Open Model* in
> [README.md](README.md).

> **Perché il controllo sull'area.** `lghmi` elenca anche task che non stanno
> sotto `$LG_ENTRY`: in modalità `S01` i path del file sono arbitrari, e i bundle
> FMU hanno le task in `<bundle>/task/<nome>`. Su Linux il contesto lo fissa solo
> il profilo — `LG_ENTRY` e le derivate `LG_LIBGRAPH`/`LG_LIBUT`/`LG_LIBRARIES` —
> perché `applyUserFromTom` di `legopc.tix` si aspetta
> `<LG_ENTRY>/models/<n>/<n>.tom`, che è il **layout Windows**: su Linux non c'è
> il livello `models` (il profilo pone `LG_MODELS=LG_ENTRY`), quindi quella
> funzione non scatta mai e non corregge niente. Aprire la task di un'altra area
> significherebbe risolverne i blocchi contro il `libgraph` **sbagliato**, senza
> che niente lo dica. Una conseguenza voluta: **le task dentro un bundle FMU non
> sono modificabili da qui** — un bundle è un artefatto confezionato.

> **Il confronto fra le aree è per identità, non per nome** (`device`+`inode` via
> `file stat`). Confrontare le stringhe non funziona: `$HOME/legocad` è un
> **symlink** — lo gestisce `lgswitch` — e `file normalize` di Tcl **non risolve i
> symlink**, rende assoluto e toglie `.` e `..`, nient'altro. `LG_ENTRY` arriva dal
> profilo nella grafia col link, mentre in modalità `S01` il path della task nasce
> da `[file normalize [file join $s01dir $relpath]]`: lì il `..` costringe a
> risolvere il link e viene fuori il path **reale**
> (`…/legopst_<area>/legocad/<task>`). Sono due grafie della stessa directory, e
> un confronto testuale rifiuterebbe **sistematicamente** le task del simulatore
> su cui si sta lavorando — cioè il caso normale.

Alla chiusura di `legopc` la barra di stato ricorda che, se il modello è
cambiato, va riallineato con `Tools → kUpSim`. L'attesa non usa un PID — il
lancio passa per `setsid`, che può forkare — ma un `pgrep` sul nome del `.tom`,
come già fa `conta_mmi`, tenendo solo i processi che eseguono davvero
`legopc.tix`: prima anche una shell che nominava `legopc.tix` e la task nella
sua riga di comando passava per un CAD aperto, e la chiusura non veniva mai
segnalata. Se il processo non compare entro 10 secondi si rinuncia in silenzio.

### Il menu `Edit` delle HMI (`draw2gr -edit`)

Le HMI di processo lanciate da `lghmi` hanno un menu **Edit**, fra *File* e
*View*, con la voce *Edit model (legopc)...*: apre nel CAD il modello della
task di **quella** HMI, cioè della directory da cui è partita. I controlli sono
quelli di `Tools → Edit model` (stesso codice, `modifica_task`), con una
differenza: a simulazione in corso la HMI **rifiuta e basta**, senza proporre
`legopc` vuoto — da lì si chiede proprio quella task. Alla chiusura di `legopc`
un avviso ricorda di riallineare con `kUpSim` e che la HMI mostra ancora lo
schema com'era all'apertura: per vedere le modifiche va chiusa e riaperta.

**Il menu è spento di default, e spento vuol dire assente.** `draw2gr` è
lanciato da molti chiamanti, e la HMI serve a guardare e perturbare la
simulazione, non a cambiarne il modello. Il menu compare solo con l'opzione
`-edit`:

```bash
cd <task> ; wish $LG_TIX/draw2gr.tcl 1 f22circ -edit
```

`-edit` è un'opzione **con nome**, non un argomento posizionale: quelli di
`draw2gr` hanno già significati diversi fra Linux e Windows (su Windows il terzo
è il `clientNum` di lgser, il quarto `command` attiva il Command Mode), e `$argc`
è letto in più punti. `draw2gr` la toglie da `argv`/`argc` come prima cosa, così
per il resto dello script è come se non ci fosse.

Chi la passa, e chi no:

| chiamante | `-edit` |
|---|---|
| `lghmi`, task dell'area corrente (`$LG_ENTRY`) | **sì** |
| `lghmi -noedit` | no |
| `lghmi -insim` (dal banco: simulazione in corso) | no |
| `lghmi`, task di un'altra area | no — `modifica_task` la rifiuterebbe sempre |
| `lghmi`, installazione senza `legopc.tix` | no |
| `lghmi`, task di un bundle FMU (`run_draw2gr.sh`) | no — sulla macchina target `legopc` non c'è |
| `legopc` (*HMI & Plot*, `watchtrends`) | no |
| FMU (`run_fmu --hmi`), `lg_cosim` | no |

La simulazione in corso **non** toglie il menu: può fermarsi mentre la HMI è
aperta, e il controllo si rifà al momento del clic.

Anche con `-edit`, `draw2gr` **non** mostra il menu, e lo scrive nel log:

- fuori da Linux (`LINUXPLAT`);
- dentro un bundle FMU (`LG_FMU_BUNDLE`);
- se non riesce a leggere `lgedit.tcl` (lo sorgia **solo** con `-edit`, così
  gli altri usi non ne dipendono);
- se discende da un `legopc`: risale la catena dei processi padre in `/proc`, e
  se uno di loro esegue `legopc.tix` ignora l'opzione. `watchtrends` non passa
  `-edit`, ma è il caso da escludere per primo — un secondo CAD aperto dalla HMI
  del primo — e il controllo lo copre anche se qualcuno aggiungesse l'opzione a
  quel lancio.

### Il riquadro `Regulation tasks` e il tool `config`

Le task di **regolazione** (`r_*`) finora erano **invisibili** in `lghmi`:
`scan_tasks` tiene solo le directory che contengono un `*.tom`, e una
regolazione il `.tom` non ce l'ha — ha i `.sed` del suo editor. Per il selettore
erano nella stessa categoria di `libgraph` e `libut`.

Ora hanno un riquadro loro, in un layout **2+1**: **processo e regolazione
affiancate in alto** — sono le due liste su cui si lavora di più e che si
confrontano fra loro — e i **faceplate `xstaz` sotto**, a tutta larghezza. **La
finestra non si allarga**: resta 680 px, la larghezza del banco — che è la
ragione per cui quel numero è quello — e cresce solo in altezza. Tre liste
affiancate avrebbero sfondato la larghezza o ridotto ogni colonna a una ventina
di caratteri.

L'altezza della finestra **non è cablata**: si prende quella *richiesta* dal
contenuto dopo aver costruito i riquadri. Le tre liste chiedono tutte 12 righe,
e una `panedwindow` alla prima apertura dà a ogni pannello la sua dimensione
naturale: così le tre partono **alla stessa altezza**. Con un numero fisso il
pannello di sotto si prendeva quel che avanzava e si apriva schiacciato.

Non c'è un pulsante: la task si apre come nelle altre liste — doppio clic,
`Invio`, o tasto destro.

Si spegne con **`-noreg`**; **`-reg`** lo riaccende nelle modalità a lista
singola (`-proc`, `-staz`), dove altrimenti non comparirebbe.

**Come vengono riconosciute**: in modalità `S01` dal **tipo `R`** scritto nel
file, che è il dato autorevole (`parse_s01` lo sa già leggere, lo fa per i
faceplate); fuori da `S01` dal **prefisso `r_`**, la stessa convenzione che usa
`kCompile`, che «entra in ogni `r_*` sotto legocad».

Tre azioni, tutte sulla task **selezionata in quel riquadro**:

| dove | azione | come gira |
|---|---|---|
| doppio clic, `Invio`, tasto destro | `config` — l'editor | processo indipendente (`setsid`), log in `/tmp`: è una GUI Motif, non un batch |
| `Tools` | `1. kCompile Regolation` | nel **visore di log** |
| `Tools` | `2. kCompile Task` | nel **visore di log** |
| `Tools` | `3. kCompile Page` | nel **visore di log** |

Le tre voci in `Tools` sono **spente** con `-noreg`: agiscono su una selezione
che senza quel riquadro non esiste.

> **L'ordine conta, ed è il motivo per cui le etichette sono numerate.**
> Produrre una task di regolazione vuol dire, in quest'ordine:
>
> 1. **`Regolation`** — compila tutti gli schemi (`config -c compreg`);
> 2. **`Task`** — produce la task come eseguibile (`config -c creatask`);
> 3. **`Page`** — compila le pagine che `mmi` animerà (`config -c compall`).
>
> **Il secondo passo NON fa il terzo**: sono tre tipi distinti. Eseguirli in
> disordine non dà errore — dà una task incoerente, che è peggio.

> **Perché `kCompile` e non `config -c` nudo.** Sarebbe più corto, ma `kCompile`
> ([kbin/kCompile](../kbin/kCompile)) prima di compilare **cancella i vecchi
> `*err*` e `net_compi.out`**, e senza quella pulizia i conteggi dopo la
> compilazione sono falsi: un `.reg_err` rimasto dalla corsa precedente fa
> leggere errori che non ci sono più. In più fa `kTest` sull'ambiente, tiene un
> log suo in `$KLOG` e conta gli errori.
>
> Il prezzo è che `kCompile` vuole un **simulatore corrente**: `kTest` esce NOK
> senza `KSIMNAME`. Per questo le tre voci sono **spente senza simulatore**,
> come già quelle di `kUpSim`. Il comando gira in una shell che sorgia il
> profilo e chiama `ksetsim`, con un `||` che **ferma la catena** se la
> selezione fallisce: senza, si compilerebbe contro il simulatore precedente
> senza che niente lo dica.
>
> `Local` prende la task da `pwd`, quindi il `cd` nella task viene dopo
> `ksetsim`. Attenzione alla grafia: è **`Regolation`**, non "Regulation" —
> scritta in inglese corretto lo script cade nell'`else` e stampa solo la riga
> d'uso.
>
> Gli errori restano nella directory della task: `<pagina>.reg_err` per i primi
> due passi, `<pagina>.rtf_err` per il terzo, e l'esito in `net_compi.out`.

> **`config` non prende argomenti**: lavora sulla directory corrente, quindi si
> fa `cd` nella task — identico a `legopc`. Il precedente è `kc`. Ma **`kc` non
> va imitato fino in fondo**: verifica l'esistenza della task cercando
> `f01.dat`, e `f01.dat` ce l'hanno *tutte* le task, anche quelle di processo.

> **Il controllo sull'area vale anche qui**, con lo stesso confronto per
> identità: `config` risolve `libut_reg/libreg` e `libut_mmi` a partire da
> `LEGOCAD_USER`, che il profilo pone a `~`, quindi punta allo stesso
> `$HOME/legocad` di `LG_ENTRY`. Una regolazione di un'altra area verrebbe
> compilata contro la libreria sbagliata.

> **A simulazione in corso si avverte e si consente** — diversamente da
> `legopc`, che invece **blocca**. La differenza è voluta: un modello salvato
> cambia la topologia sotto la task che gira, mentre sulla regolazione si lavora
> anche a simulazione viva. `creatask` ha un avviso più esplicito degli altri
> due, perché non modifica file ma **rigenera la task**: l'eseguibile in `proc/`
> viene ricostruito sotto la simulazione, che continuerebbe a usare il vecchio
> fino al riavvio.

### Quando le voci di `Tools` sono spente

Tutte le voci che agiscono sul simulatore — le tre di `kUpSim` e le tre di
`kCompile` — sono accese solo se c'è un **simulatore corrente**, cioè se `$KSIM`
esiste ed è una directory. `kCompile` in particolare comincia con `kTest`, che
senza `KSIMNAME` esce NOK.

Normalmente il simulatore lo fissa il profilo all'avvio (`ksetsim_default`) e
`lghmi` lo eredita. Ma **l'eredità non è garantita**: il wrapper risorgia il
profilo solo se `LG_TIX` è vuota, quindi un lancio da un ambiente che ha
`LG_TIX` ma non `KSIM` arrivava qui senza simulatore, e trovava **tutte le voci
spente senza che nulla dicesse perché**. Bastava passare da *Tools → Current
simulator* per vederle accendersi — quella voce imposta `env(KSIM)` dentro il
processo Tcl — e la cosa sembrava un capriccio dell'interfaccia.

Ora, se `KSIM` manca o punta a una directory che non c'è, `lghmi` **rifà da sé
la cascata del profilo**: `~/.legosim`, poi `cassano0`, poi il primo di
`$KSKED`. Solo in memoria: `~/.legosim` non viene riscritto, perché aprire il
selettore non è una scelta dell'utente e non deve cambiare il default delle
shell future. Delle variabili derivate aggiorna solo `KPAGES`, che serve al
pulsante *mmi*; alle altre (`KWIN`, `KLOG`…) non pensa: i comandi girano in una
shell che chiama `ksetsim` per conto suo, ed è quella a derivarle.

Quando il ripiego scatta, la barra di stato lo dice; se non c'è proprio nessun
simulatore, dice quello e indica dove sceglierne uno. Succede anche dopo
*File → Work area* verso un'area il cui `sked` non ha simulatori.

> **`Open Simulator path` non c'entra con il simulatore corrente.** Cambia la
> directory su cui lavora il selettore — quale elenco di task si vede e quale
> *Set Sim path* ereditano le HMI — non `$KSIM`. Sono due cose distinte, e
> sceglierne una non tocca l'altra.

### `Tools → Current simulator` e la variabile `KSIM`

Il sottomenù elenca i simulatori di `$KSKED` (le stesse directory della
funzione `ksims`) con quello corrente marcato. Scegliendone uno:

1. lghmi scrive il nome in **`~/.legosim`**, che è il file già letto da
   `ksetsim_default` all'avvio di ogni shell (poi `cassano0`, poi il primo di
   `ksims`). La scelta vale quindi anche per **le shell future** e per gli altri
   comandi della toolchain;
2. i comandi lanciati da `Tools` girano in una shell che **sorgia il profilo e
   chiama `ksetsim <nome>`**;
3. il nome si ricorda come ultimo simulatore **dell'area** di `~/sked`
   (`~/.lghmi_areas`), per ritrovarlo dopo *File → Work area*;
4. nell'ambiente di `lghmi` si aggiornano `KSIM`, `KSIMNAME` e **`KPAGES`**, che
   serve al pulsante *mmi*. `KPAGES` la calcola il `ksetsim` vero, in una
   subshell che sorgia solo `Alg_env.sh` con `HOME` spostata (così non tocca
   `~/.legosim`): di norma è `$KSIM/globpages`, ma `$KSIM/ksim.conf` la può
   ridefinire. Prima restava quella del simulatore con cui `lghmi` era partito,
   e *mmi* apriva le pagine di quello.

Il punto 2 non è pignoleria: `ksetsim` non imposta solo `KSIM`, ne **deriva una
ventina di variabili** (`KWIN`, `KPAGES`, `KSTATUS`, `KCASSAFORTE`, `KGRAF`…) e
sorgia `$KSIM/ksim.conf`. Cambiare solo `KSIM` lascerebbe le derivate puntate al
simulatore precedente, e `kUpSim` lavorerebbe su un miscuglio senza dirlo. Quella
logica non è riscritta in Tcl: a derivare è il codice del profilo, che esiste
già.

> **Quello che una GUI non può fare**: cambiare l'ambiente della *shell che l'ha
> lanciata*. La `$KSIM` del tuo terminale resta quella di prima; per allinearla
> basta un `ksetsim <nome>`, oppure una shell nuova, che rilegge `~/.legosim`.

## Menù `?` — versione e documentazione

Stesso nome che usa `legopc`, per coerenza fra le due finestre.

### `About LegoPST`

Legge **`$LEGOROOT/version.h`**, la stessa fonte del dialogo *LegoPc Release
Info* di `legopc`: `GIT_VERSION_STRING`, `BUILD_NUMBER`, `BUILD_DATE_STRING`.
Il file lo genera il Makefile (`make -f Makefile.mk version.h`) a partire da
git.

Sotto la versione ci sono le quattro righe che dicono **dove si sta
lavorando** — `LEGOROOT`, simulatore corrente, radice utente, directory — che in
questo ambiente è la domanda subito successiva a "che versione è". Il pulsante
*Release Notes* apre `$LG_INSTALL/relnotes.txt` nell'editor, e compare solo se
quel file c'è.

> A differenza di `legopc`, **se `version.h` manca non viene generato**: lghmi
> può girare dove il repository non è scrivibile — per esempio dentro un bundle
> FMU — e un selettore non deve mettersi a invocare `make`. Il dialogo dice come
> ottenerlo.

### Documentazione

```
?
  About LegoPST
  ──────────────────────────────────────────
  LegoPST - project overview (README)         README.md          ← in grassetto
  ──────────────────────────────────────────
  Annotated documentation index               INDICE_DOCUMENTAZIONE.html
  kbin commands (the 192 kprocedure)          kbin/kbin-riferimento-comandi-LegoPST.html
  Modules help (legacy manual)                $LG_HTML/index.htm, via open_hlp
  ──────────────────────────────────────────
  This window: lghmi                          Alg_legopc/LGHMI.md
  Configuring a simulator: al_sim.conf        docs/AL_SIM_CONF.md
  Command faceplates (xstaz)                  Alg_rt/grafica/xstaz/HOWTO_faceplate.md
```

Il **README** è la prima voce, in un gruppo suo e **in grassetto**: è il
documento che dice *che cos'è* LegoPST — quello che si legge su GitHub — e viene
prima di sapere dove sta tutto il resto. Il risalto lo dà un font derivato da
`TkMenuFont` con `-weight bold`, così segue tema e dimensione del sistema
invece di essere scritto a mano.

Il criterio per le altre: la documentazione di LegoPST è molta — una trentina di `.md`, due
HTML e il manuale storico dei moduli in 218 pagine `.htm` — e **il menù non ne è
il catalogo**. C'è l'**indice ragionato**, che è l'hub di tutto il resto con le
sue 12 sezioni, e accanto i documenti che rispondono alle domande di chi sta
usando *questa* finestra: lghmi stesso, la configurazione del simulatore che il
menù `Tools` riallinea, e i faceplate che la lista di destra apre. Il resto
resta a un click dentro l'indice.

Dettagli d'implementazione:

- l'ordine delle voci è **dichiarativo**, nella proc `documenti_aiuto`: `--` è un
  separatore e `MODULI` è il manuale storico, che non è un file del repository
  ma una collezione sotto `$LG_HTML`;
- i `.md` si aprono **nel browser**, come i 36 rimandi ai `.md` dentro l'indice
  ragionato, così il meccanismo resta uno solo. Il browser lo sceglie
  `browser_disponibile` di [src/tix/openhelp.tcl](src/tix/openhelp.tcl), la
  stessa di `legopc`, partendo da `$LG_BROWSER`. Sui `.md` passa prima un
  convertitore, **se c'è**: vedi sotto;
- il manuale dei moduli si apre con `open_hlp`, la stessa proc della voce *Help*
  di `legopc`. `openhelp.tcl` viene sorgiato dentro un `catch`: se manca si perde
  solo il menù `?`, non il selettore;
- una voce che punta a un file assente **nasce disabilitata** invece di sparire:
  si vede che il documento è previsto e che manca.

### Perché i `.md` si vedono formattati

Un browser **non sa rendere il Markdown**: il sistema classifica i `.md` come
`text/plain`, quindi Firefox ne mostra il *sorgente*. Prima di aprirli, `lghmi`
li converte in HTML in una directory temporanea (`$TMPDIR`), con un foglio di
stile incorporato — tabelle con i bordi, blocchi di codice su fondo grigio,
citazioni con la barra a sinistra, titoli con l'ancora per i rimandi interni.

**La conversione la fa LegoPST, non un pacchetto di sistema:**
[src/tix/md2html.tcl](src/tix/md2html.tcl) è un convertitore Markdown→HTML in
**Tcl puro**. Tcl c'è per definizione — tutta l'interfaccia di LegoPST è
Tcl/Tk — quindi la documentazione si vede formattata su **ogni installazione**,
senza installare niente e con la stessa resa dappertutto.

Il perimetro è il sottoinsieme che la documentazione di LegoPST usa davvero,
misurato su 34 documenti e 10 073 righe: titoli, recinti di codice, tabelle,
citazioni, elenchi puntati e numerati, righe orizzontali, link, immagini,
codice inline, grassetto, corsivo. Non è un parser Markdown generico e non prova
a esserlo: nella nostra documentazione non ci sono note a piè di pagina, tabelle
annidate né HTML inline oltre sei righe.

Sui nostri documenti **rende meglio di `markdown_py`**, che sbaglia i recinti di
codice rientrati dentro una voce di elenco — li trasforma in un `<code>`
malformato che si mangia il resto del blocco. Verificato confrontando i due
output su `LGHMI.md`, `HOWTO_faceplate.md` e `AL_SIM_CONF.md`: elementi
identici, tranne quel caso, dove il conteggio dei `<pre>` differisce perché il
nostro è corretto.

Il degrado, se qualcosa mancasse:

| situazione | cosa si vede |
|---|---|
| normale | HTML formattato da `md2html.tcl` |
| `md2html.tcl` assente (deploy parziale, `bin` vecchia) | HTML da `markdown_py`, `pandoc` o `cmark`, se installati |
| nessuno dei due | il `.md` grezzo, con la riga di stato che lo dice |

**Dell'HTML si riconoscono solo `<details>` e `<summary>`**, che nel README
fanno la sezione richiudibile dell'installazione Docker. Tutto il resto degli
angolari resta escapato, ed è la scelta giusta, non una semplificazione: nella
nostra documentazione `<nome>`, `<task>`, `<modello>`, `<dir>`, `<path>` e
simili sono **segnaposto in prosa, a centinaia**. Passandoli come HTML il
browser li tratterebbe da tag sconosciuti e li **cancellerebbe dalla pagina**,
cambiando il senso di quello che c'è scritto.

Un limite noto: un recinto di codice **rientrato dentro una voce di elenco**
chiude l'elenco e lo riapre dopo, invece di annidarsi nella voce. L'HTML resta
valido e si legge bene, solo la spaziatura cambia; annidarlo per davvero
vorrebbe dire tenere aperto il `<li>`, e per quattro occorrenze in 34 documenti
non vale la complicazione.

Un dettaglio che è facile sbagliare: l'HTML generato porta un
`<base href="file://<directory del documento>/">`. Senza quello i **rimandi
relativi** agli altri documenti — 236 in tutta la documentazione — si
risolverebbero dentro la directory temporanea e non porterebbero da nessuna
parte.

**Le ancore dei titoli seguono la regola di GitHub**, perché i rimandi interni
(`[…](#sezione)`) sono scritti per GitHub: minuscolo, via tutto ciò che non è
lettera, cifra, spazio, trattino o sottolineatura, e **ogni** spazio diventa un
trattino, senza fonderli (`File → Logs` dà `file--logs`). Fino a settembre 2026
i separatori venivano fusi, e i rimandi ai titoli con frecce o lineette non
funzionavano nel browser: 3 su 24.

**Il `.md` si legge in UTF-8 e l'HTML si scrive in UTF-8**, qualunque sia la
codifica di sistema. Con `LANG=POSIX`, che il profilo imposta, la lettura con la
codifica di sistema spezzava ogni carattere non ASCII in due o tre byte: le
ancore con accenti, frecce o emoji venivano storpiate, e un `à` a fine riga
perdeva il secondo byte (`0xA0`, che in Latin-1 è uno spazio e cadeva col
`trim`).

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
   (*"mmi: no Context.ctx in DIR"*);
2. il Context c'è: da esso si leggono `*pages` (dove stanno le pagine, anche
   altrove) e `*page_list` (quali sono), e si contano i `<NOME>.rtf` presenti. Se
   nessuno esiste → *"mmi: no compiled page (.rtf) in DIR"*.

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
- `lghmi.tcl` sorgia **`lgedit.tcl`** e **`lgstaz.tcl`** (faceplate) dalla
  propria directory; senza uno dei due non parte. `lgedit.tcl` contiene i
  controlli di *Edit model* e altri che il selettore usa ovunque
  (`sim_attiva`, `stessa_directory`).

### File che `lghmi` scrive nella home

| File | Contenuto | Chi lo scrive |
|---|---|---|
| `~/.lghmi_recent` | le directory usate di recente, fino a 30, di tutte le aree | *Open Simulator path*, i recenti, l'avvio da una directory di simulazione |
| `~/.lghmi_areas` | l'ultimo simulatore usato in ogni area: una riga per area, con directory fisica e nome separati da una barra verticale | *Tools → Current simulator*, *File → Work area* |
| `~/.legosim` | il simulatore corrente per le shell future (lo legge `ksetsim_default`) | *Tools → Current simulator*, *File → Work area* |

Se la home non è scrivibile si perde solo la memoria: il selettore funziona lo
stesso. Nei log in `/tmp` (`lghmi_*.log`) finiscono invece gli output dei
comandi lanciati, compreso `lghmi_lgswitch.log`.

## Variabili d'ambiente

| Variabile | Effetto |
|---|---|
| `LG_TASKROOT` | directory delle task in modalità dir-scan (default `$HOME/legocad`) |
| `LG_SIM_PATH` | dir sim pre-impostata per *Set Sim path* (la imposta `lghmi`; `-noloc` la omette); il pulsante *mmi* ne prova per primo il `globpages` |
| `LG_TIX` | dir di `draw2gr.tcl`/`lghmi.tcl` (dal profilo LegoPST) |
| `LG_ENTRY`, `KSKED` | i link `~/legocad` e `~/sked` (dal profilo): *Work area* funziona solo se finiscono in `legocad` e `sked` e stanno nella stessa directory; `LG_ENTRY` decide anche quali task hanno il menu *Edit* |
| `KSIM`, `KSIMNAME` | simulatore corrente (dal profilo, o scelto da *Tools*); se mancano `lghmi` rifà la cascata del profilo |
| `KPAGES` | dir delle pagine MMI usata dal pulsante *mmi* (di norma `$KSIM/globpages`); `lghmi` la ricalcola quando cambia simulatore |
| `UTIL97`, `LEGOROOT` | dove cercare `lgswitch` (`$UTIL97/bin`, poi `$LEGOROOT/util97/bin`, poi il `PATH`) e `Alg_env.sh` per ricalcolare `KPAGES` |

## Troubleshooting

- **"LG_TIX non definito … profilo non sorgiato"**: l'ambiente LegoPST non è
  disponibile e l'auto-source è fallito. Lancia da una shell in cui hai sorgiato
  `.profile_legoroot`.
- **Nessuna task in lista**: in dir-scan, nessuna sottodir di `$LG_TASKROOT` ha
  un `*.tom` (controlla `LG_TASKROOT`, che può essere un symlink); in modalità
  S01, il file non ha task di tipo `P`. Le task di **regolazione** non stanno
  lì: non hanno un `.tom`, e hanno un riquadro loro.
- **Le voci di `Tools` sono spente**: agiscono sul simulatore corrente, e non ce
  n'è uno valido. La barra di stato lo dice. Sceglilo da *Tools → Current
  simulator*, o con `ksetsim <nome>` prima di lanciare. Non confonderlo con
  *Open Simulator path*, che cambia la directory di lavoro e **non** il simulatore.
- **Le voci `kCompile` sono spente ma `kUpSim` no**: manca il riquadro delle
  regolazioni (`-noreg`), e quelle voci agiscono su una selezione che lì dentro
  non esiste.
- **File → Work area è spenta, o dice "Not available"**: il motivo è scritto
  nel sottomenù. Di solito `LG_ENTRY` e `KSKED` mancano o non sono i link
  `…/legocad` e `…/sked` della stessa directory — un profilo non standard, o il
  `run_lghmi.sh` di un bundle FMU sulla macchina di destinazione, dove il
  profilo non c'è — oppure `lgswitch` non si trova. Con `-insim` è spenta di
  proposito.
- **Lo switch si rifiuta elencando dei processi**: stanno ancora lavorando
  sull'area corrente, e dopo lo switch la vedrebbero cambiare sotto di loro.
  Chiudili — la simulazione con *Simulator Shutdown* dal banco — e riprova. La
  colonna di destra dice perché ciascuno conta: la directory in cui lavora
  (relativa all'area), oppure *simulation running*, *legopc (CAD)*, *another
  lghmi*.
- **La riga dell'area è rossa**: i link non indicano un'area sola. *MIXED*
  vuol dire `legocad` e `sked` di aree diverse; *none* che uno dei due è una
  directory vera o manca. Scegliere un'area da *Work area* rimette le cose a
  posto (una directory vera viene rinominata in `.prelink-*`, previa conferma).
- **Dopo un `lgswitch` da terminale** il selettore mostra la nuova area al primo
  *Refresh*, ma simulatore corrente e directory di lavoro restano quelli di
  prima: riaprire `lghmi`, oppure fare lo switch da *Work area*.
- **"lgswitch did not complete the switch"**: il dialogo mostra la fine di
  `/tmp/lghmi_lgswitch.log` (anche da *File → Logs*). Il caso tipico è la
  directory dei link non scrivibile: `lgswitch` se ne accorge prima di toccare
  qualunque cosa, e i link restano com'erano.
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
