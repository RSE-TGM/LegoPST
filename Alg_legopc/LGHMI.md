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
lghmi -insim       # lanciato da dentro una simulazione (lo passa il banco)
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
- **File → Open loc path…** → cambia la **directory di lavoro** del selettore,
  e sotto la voce ci sono le ultime directory usate: vedi sotto.
- **Tools** → aggiorna la configurazione del **simulatore corrente** con
  `kUpSim`, e permette di cambiare simulatore: vedi sotto.
- **?** → versione di LegoPST e documentazione dell'ambiente: vedi sotto.
- **Refresh** → rilegge l'elenco delle task.
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

In quel caso il selettore **appartiene a quella simulazione**, e due comandi
vengono **disabilitati**:

| comando | perché |
|---|---|
| *File → Open loc path* | lo porterebbe su un'altra directory, scollegandolo dalla simulazione che l'ha aperto |
| pulsante *net_startup* | comincia con `killsim`: ammazzerebbe proprio la simulazione da cui è stato lanciato, e il banco con lei |

La voce di menù nasce disabilitata e il pulsante resta spento; la riga di stato
dice *"lanciato dal banco: directory fissa, simulazione già in corso"* e il
titolo della finestra porta `(dal banco)`, così si capisce da dove viene.

Lanciando `lghmi` a mano l'opzione non serve: i due comandi restano
disponibili, e `net_startup` chiede comunque conferma.

## `File → Open loc path…` — cambiare simulazione senza riavviare

Apre un selettore di directory e **porta lì il selettore**: da quella directory
dipendono la modalità (l'`S01` si cerca nella directory corrente), la lista dei
faceplate (`r02.dat` della directory), la directory di lavoro dell'`mmi` e il
**Set Sim path** che le HMI ereditano. Le liste si aggiornano subito.

È l'equivalente di **rilanciare `lghmi` da quella directory**, e serve quando si
passa da una simulazione a un'altra: prima bisognava chiudere il selettore,
`cd`, e riaprirlo.

### I path recenti

Sotto *Open loc path…* il menù File porta le **ultime 3 directory usate**, così
per tornare su una simulazione già visitata non serve riaprire il dialogo di
selezione: si clicca la voce.

- La lista sta in **`~/.lghmi_recent`**, una riga per path. Non in
  `$LG_ENTRY/legopc_prefs.tcl` come le preferenze di `legopc`, perché
  attraversa le installazioni: la radice utente cambia proprio quando si cambia
  directory.
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
- Il numero di path ricordati è la costante `MAXRECENTI` in `lghmi.tcl`.

Con `-insim` la voce *Open loc path* **e tutti i path recenti** sono
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
| **Invio** al prompt *«premi Invio per chiudere»* | esce il session leader, e il kernel manda `SIGHUP` al foreground process group |

Il secondo è il peggiore: era il messaggio stesso a invitare a farlo. Un
terminale non poteva nemmeno chiedere conferma — `xterm` **non ha alcun hook**
sulla richiesta di chiusura del window manager (`WM_DELETE_WINDOW`). La finestra
di log invece è del selettore, quindi la X passa da Tk e si può avvisare prima di
chiudere.

### La finestra di log

- **X**, **Chiudi** o **Esc** → se la simulazione è in corso, chiedono conferma
  ricordando che **chiudere la finestra NON la ferma**, elencando i processi che
  restano vivi e dicendo come fermarli. Se non c'è nulla in esecuzione, si chiude
  senza domande.
- **Ferma la simulazione** → esegue `killsim`, cioè lo stesso comando con cui
  `net_startup` comincia. Chiede conferma ricordando che ammazza anche le HMI e i
  faceplate aperti, e che cancella *tutte* le SHM, le code e i semafori
  dell'utente. È acceso solo quando c'è qualcosa da fermare.
- In basso a sinistra lo **stato**: quali fra `dispatcher`, `net_sked` e `banco`
  sono vivi, riletto ogni 3 secondi.
- Il visore è **uno solo**: un secondo `net_startup` riparte da capo nella stessa
  finestra, come il log.

Con `-insim` il pulsante è sempre spento, qualunque cosa ci sia nella
directory: vedi sopra.

## Menù `Tools` — aggiornare la configurazione del simulatore

Tre voci, che lanciano **`kUpSim`** sul **simulatore corrente** (`$KSIM`) in un
terminale:

| voce | cosa fa |
|---|---|
| `kUpSim - riallinea la configurazione di <nome>` | la catena completa |
| `kUpSim -nommi - senza le pagine MMI dei faceplate` | salta `kStazPages`, `kWinContext`, `kCompileSim` |
| `kUpSim -n - anteprima: mostra i passi senza eseguirli` | prova a vuoto |

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

### `Tools → Simulatore corrente` e la variabile `KSIM`

Il sottomenù elenca i simulatori di `$KSKED` (le stesse directory della
funzione `ksims`) con quello corrente marcato. Scegliendone uno:

1. lghmi scrive il nome in **`~/.legosim`**, che è il file già letto da
   `ksetsim_default` all'avvio di ogni shell (poi `cassano0`, poi il primo di
   `ksims`). La scelta vale quindi anche per **le shell future** e per gli altri
   comandi della toolchain;
2. i comandi lanciati da `Tools` girano in una shell che **sorgia il profilo e
   chiama `ksetsim <nome>`**.

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
  LegoPST - panoramica del progetto (README)  README.md          ← in grassetto
  ──────────────────────────────────────────
  Indice ragionato della documentazione       INDICE_DOCUMENTAZIONE.html
  Comandi kbin (i 192 kprocedure)             kbin/kbin-riferimento-comandi-LegoPST.html
  Help dei moduli (manuale storico)           $LG_HTML/index.htm, via open_hlp
  ──────────────────────────────────────────
  Questa finestra: lghmi                      Alg_legopc/LGHMI.md
  Configurare un simulatore: al_sim.conf      docs/AL_SIM_CONF.md
  Faceplate di comando (xstaz)                Alg_rt/grafica/xstaz/HOWTO_faceplate.md
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
