# `lg*` — riferimento dei comandi d'ambiente LegoPST

I comandi che cominciano per **`lg`** sono il punto d'ingresso di LegoPST: entrare
nell'ambiente, lanciare il CAD e le HMI, riallineare un simulatore, scegliere su
quale area di lavoro operare. Sono l'equivalente "utente" dei 192 comandi `k*`
documentati in [../kbin/README.md](../kbin/README.md), che invece coprono il
dettaglio della manutenzione di un simulatore.

Questa è la mappa dello **scopo di ciascuno**. Dove esiste un documento
specializzato, la voce lo indica: qui resta comunque una descrizione breve, così
il riferimento si legge tutto di seguito senza rimbalzare.

> Ricostruito ispezionando gli script, gli alias in `Alg_env.sh` e
> `.profile_legoroot`, e i makefile. I comandi non sono tutti dello stesso genere:
> alcuni sono alias di shell che esistono solo col profilo sorgiato, altri
> eseguibili nel `PATH`, altri ancora script Python da lanciare a mano.

## Convenzioni

| Marca | Significato |
|---|---|
| **alias** | alias di shell definito in `Alg_env.sh` o `.profile_legoroot`: esiste **solo** in una shell col profilo sorgiato, non è un file lanciabile |
| **func** | funzione di shell, come sopra |
| **script** | script eseguibile in una directory del `PATH` |
| **binario** | eseguibile compilato |
| **tool** | script Python da lanciare a mano (`python3 …`), non nel `PATH` |
| **legacy** | resto di epoca VMS/OSF, non funzionante o non usato su Linux |

---

## Prerequisito: il profilo

**Senza profilo sorgiato nessuno di questi comandi esiste**, e gli alias non sono
nemmeno definiti:

```sh
cd $LEGOROOT      # la radice del repository
source .profile_legoroot
```

È lo stesso prerequisito del build (vedi [BUILD.md](BUILD.md)): senza, gli include
path restano vuoti e la compilazione fallisce in modo poco chiaro. Il profilo
costruisce il `PATH` aggiungendo, nell'ordine, `LEGOCAD_BIN`, `LEGO_BIN`,
`LEGORT_BIN`, `LEGOMMI_BIN`, `$UTIL97/bin`, `$UTIL2007/bin` e `$KBIN` — è da lì
che arrivano gli eseguibili elencati sotto.

> **Attenzione**: il profilo aliasa anche `test` a `cd $HOME; pwd`. In una shell
> interattiva col profilo sorgiato, `test -f x` non fa quello che sembra: usare
> `[ -f x ]`.

---

## 0. Entrare e restare nell'ambiente

| Comando | Tipo | Scopo |
|---|---|---|
| `lgini` | alias | Ri-sorgia `$LEGOROOT/.profile_legoroot` nella shell corrente e imposta il prompt `[LEGOROOT:utente@host dir]$`. Serve a **rientrare** nell'ambiente dopo averlo sporcato, o dopo aver modificato il profilo. Richiede che `LEGOROOT` sia già impostata: non è il comando con cui si parte da zero. |
| `lggo` | alias | Come `lgini`, ma prima fa `cd $LEGOROOT`. È il modo spiccio di tornare alla radice del progetto e riallineare l'ambiente in un colpo solo. |

Il prompt `[LEGOROOT:…]` non è un vezzo: è il segnale visivo che la shell ha il
profilo caricato. Se non c'è, i comandi che seguono non funzioneranno.

---

## 1. Container Docker

Il modo raccomandato di usare LegoPST su una macchina che non ha l'ambiente
installato. Il container porta compilatori, Motif, Tcl/Tk/Tix e X11 già a posto.

| Comando | Tipo | Scopo |
|---|---|---|
| `lgdock` | script | **Il comando da usare.** Lancia il container LegoPST e apre un terminale bash dentro, creando dinamicamente l'utente dell'host nel container così che i file scritti restino tuoi. Opzioni: `-d`/`--demo` installa la demo (legocad e sked) nella home e parte con quella; `-s`/`--socat` usa un socket bridge per l'X11, necessario via SSH/MobaXterm; `-p`/`--pull` aggiorna l'immagine prima di partire; `-h` e `-v` per aiuto e versione. Si combinano: `lgdock -d -s`. |
| `lgdock_multi` | script | Variante **multipiattaforma**: stessa interfaccia (`-h`, `-v`, `-d`, `-s`) ma senza `--pull`, e con il rilevamento dei permessi — se `/var/run/docker.sock` esiste ma non è scrivibile dall'utente, ricade su `sudo docker` (chiedendo subito la password, invece di fallire a metà). Da usare dove `lgdock` non parte per questioni di permessi. |
| `lgdock_socat` | script | Versione **v1.0**, solo X11 via `socat`. Superata da `lgdock --socat`, che fa lo stesso restando un comando solo. Conservata per compatibilità. |

> `lgdock` e `lgdock_multi` sono alla v2.0, `lgdock_socat` alla v1.0: se ne dubiti,
> usa `lgdock`.

Dettagli su immagine, build e installazione: [../docker/README_INSTALLER.md](../docker/README_INSTALLER.md).

---

## 2. CAD grafico (legopc)

| Comando | Tipo | Scopo |
|---|---|---|
| `lgpc` | alias | **Lancia il CAD grafico** `legopc.tix`: disegno degli schemi, librerie di moduli, generazione dei `.i5`/`.tom`. Imposta `LG_TIX=$LG_BIN` prima di partire, quindi usa sempre la versione corrente in `Alg_legopc/bin`. È il comando normale. |
| `lgpcu` | alias | Lo stesso CAD ma con il **wish "ultimo"** (`$LG_WISH`, in `tcltktix-8.3.5b/`) invece di quello di sistema. Serve solo quando il wish di sistema dà problemi con Tix. |
| `lgpc2` | func | Lancia `wish $LG_TIX/legopc.tix` **senza reimpostare `LG_TIX`**: rispetta un `LG_TIX` già esportato a mano. L'uso previsto non è documentato da nessuna parte, e differisce da `lgpc` solo per questo. |

> `lgpc0` **non esiste più** (rimosso il 2026-08-02): lanciava la legopc originale
> da `$LG_BASE/bin_old`, generata da `src/tix_old`, sorgente ormai escluso dal
> build e quindi non più aggiornato. Usare `lgpc`.

Il CAD si raggiunge anche **da dentro `lghmi`**, con `Tools → Edit model
(legopc)`, che lo apre direttamente sul modello della task selezionata — vedi la
sezione 3 — e dal menu **Edit** delle HMI `draw2gr` lanciate da `lghmi`, che lo
apre sulla task di quella HMI. Queste strade fanno alcuni controlli che la riga
di comando non fa (area di lavoro, simulazione in corso, `legopc` già aperto
sulla stessa task).

Riferimento completo del CAD — librerie moduli, `.i5`/`.tom`/`.remap`/`.lstyle`,
remark e background, elementi operatore (bottoni faceplate e invio di valori
dagli schemi), Command Mode, Set Sim path, unità di misura:
[../Alg_legopc/README.md](../Alg_legopc/README.md).

---

## 3. HMI di supervisione e faceplate

| Comando | Tipo | Scopo |
|---|---|---|
| `lghmi` | script | **Selettore grafico delle task**: apre una finestra con l'elenco delle task e, scegliendone una, lancia la HMI `draw2gr` in un processo indipendente. Ha anche una **modalità faceplate** (`-staz`) che elenca le stazioni di comando compilate in `r02.dat` e apre quella scelta con `xstaz`, avviandolo se serve — utile con `net_startup`, che monta il banco e non ha il dialogo delle stazioni di `net_monit`. Dalla stessa finestra si lancia `net_startup` e si segue il log della simulazione, e si cambia area di lavoro (*File → Work area*, che usa `lgswitch`). |

Opzioni principali:

| Opzione | Effetto |
|---|---|
| `-staz` | modalità faceplate invece che task (vedi sopra) |
| `-loc [DIR]` | pre-imposta il *Set Sim path* delle HMI lanciate (via `LG_SIM_PATH`). Senza `DIR` usa la directory corrente. **È il comportamento di default**: animazione, Plot e Command puntano subito alla simulazione giusta senza doverlo fare a mano in ogni HMI |
| `-noloc` | non pre-imposta alcun sim path: ogni HMI parte "nuda" |
| `-insim` | dichiara che il selettore è lanciato **da dentro** una simulazione in corso. Lo passa il banco (`new_monit`). Disabilita *File → Open Simulator path* e il pulsante *net_startup*, che con `killsim` ammazzerebbe proprio la simulazione che ha aperto il selettore, e *File → Work area*; lancia le HMI senza menu *Edit* |
| `-noedit` | le HMI `draw2gr` lanciate non hanno il menu *Edit*, che apre il modello della task in `legopc`. Senza, il menu c'è per le task dell'area corrente (mai per quelle dei bundle FMU) |

Dal menu `Tools` si riallinea la configurazione del simulatore (`kUpSim`, cioè
`lgupsim`), si cambia simulatore corrente, e con **`Edit model (legopc)`** si apre
il CAD sul modello della task selezionata — rifiutando se una simulazione è in
corso o se la task appartiene a un'altra area di lavoro.

Dal menu `File → Work area` si **cambia area di lavoro** senza uscire: è
`lgswitch` (sezione 5), con in più il rifiuto finché qualcosa lavora sull'area
corrente e, dopo lo switch, il riallineamento di liste e simulatore corrente.
L'area corrente sta nel titolo della finestra e nella prima riga in alto.

Un terzo riquadro elenca le **task di regolazione** (`r_*`), che prima non
comparivano affatto perché non hanno un `.tom`: da lì si apre **`config`**,
l'editor della regolazione, e da `Tools` si lanciano i tre passi
di costruzione — `kCompile Regolation`, `Task`, `Page` — sulla sola task
selezionata e in quest'ordine. Si nasconde con `-noreg`.
L'output delle compilazioni — comprese quelle di `kUpSim` — finisce nel visore
di log di `lghmi`, riapribile da `File → Logs`.

Variabili d'ambiente: `LG_TASKROOT` (directory delle task, default `$HOME/legocad`),
`LG_SIM_PATH` (la imposta `-loc`), `LG_TIX`.

Riferimento completo, formato `S01` e finestra di log: [../Alg_legopc/LGHMI.md](../Alg_legopc/LGHMI.md).

---

## 4. Riallineamento del simulatore

| Comando | Tipo | Scopo |
|---|---|---|
| `lgupsim` | alias | **Riallinea tutta la configurazione del simulatore corrente** dopo una modifica a modelli, schemi o faceplate. È un alias di `kUpSim`, che orchestra in quest'ordine: `kConnex` (topologia e connessioni → `S01`) → `kNetCompi` (compilazione task → `variabili.rtf`) → `kCompStaz` (faceplate → `r02.dat`) → `kStazPages` → `kWinContext` → `kCompileSim` (pagine MMI) → `kCollect`. Si ferma al primo passo fallito dicendo quale. |
| `lgupsimx` | alias | Come sopra ma `kUpSim -nommi`: **salta le pagine MMI** dei faceplate. Più rapido quando si sta lavorando solo sui modelli. |

> Il simulatore su cui agiscono è quello **corrente**, scelto con `ksetsim <nome>`
> (`ksims` li elenca). I tool `k*` fanno `cd $KSIM` e ignorano la directory da cui
> li lanci: sceglilo **prima**, o lavorerai su quello sbagliato senza accorgertene.

Dettaglio dei singoli passi: [../kbin/README.md](../kbin/README.md).

---

## 5. Scelta dell'area di lavoro

| Comando | Tipo | Scopo |
|---|---|---|
| `lgswitch` | script | **Commuta i link `legocad` e `sked`** fra più aree di lavoro. Nella directory corrente cerca le directory `legopst_*`, le elenca segnalando quali contengono `legocad` e/o `sked`, e crea i due link simbolici verso quella scelta. È il modo di tenere più installazioni affiancate (una demo, un impianto reale, una prova) e passare dall'una all'altra senza spostare file. |

Modi d'uso:

| Invocazione | Effetto |
|---|---|
| `lgswitch` | **interattivo**: mostra lo stato attuale dei link, elenca le `legopst_*` disponibili e chiede quale usare |
| `lgswitch <dir>` | prende `legocad` e `sked` da `<dir>`, chiedendo conferma |
| `lgswitch -f <dir>` | come sopra, **senza chiedere conferma** |
| `lgswitch -s <sorgente> <link>` | crea un singolo link `<link>` → `<sorgente>`. Rifiuta di procedere se `<link>` esiste e **non** è un link simbolico |
| `lgswitch -s -f <sorgente> <link>` | come sopra, forzando la sovrascrittura |
| `lgswitch -l` / `--list` | **non cambia niente**: elenca aree, stato dei link e copie `.prelink-*` in una forma pensata per un programma (vedi sotto). La usa `lghmi` |
| `lgswitch -h` | aiuto |

L'output di `--list`, una riga per voce con i campi separati dalla barra
verticale:

```
link|legocad|link|legopst_nuclear/legocad     # nome, stato (link, dir, altro, assente), destinazione
link|sked|link|legopst_nuclear/sked
area|legopst_elsy|1|1                         # nome, ha legocad, ha sked
area|legopst_2i-retegas_modificato|1|0
backup|legocad.prelink-20260916-180429        # copie di sicurezza
```

> **Da `lghmi`.** Lo stesso switch si fa da *File → Work area* del selettore,
> che prima di chiamare `lgswitch -f` si rifiuta se qualcosa lavora ancora
> sull'area corrente (simulazione, `legopc`, HMI…) e poi riallinea simulatore
> corrente e liste. Vedi
> [LGHMI.md](../Alg_legopc/LGHMI.md#cambiare-area-di-lavoro-menu-file-work-area).
>
> I **colori** escono solo se l'output è un terminale: rediretto in un file (è
> così che lo usa `lghmi`) lo script scrive testo semplice.

> **Cosa succede a ciò che c'è già.** Se `legocad` (o `sked`) è un **link**, viene
> sostituito. Se è una **directory vera** non viene mai cancellata: viene
> rinominata in `legocad.prelink-AAAAMMGG-HHMMSS`, e lo script stampa il comando
> esatto per tornare indietro. Queste copie vengono **elencate con la loro
> dimensione a ogni esecuzione**, finché non le cancelli: sono alberi interi, e
> senza quell'elenco resterebbero lì per sempre senza che niente lo dica.
>
> La modalità `-s` è più prudente di proposito: su una directory vera **si ferma**
> invece di rinominarla, e chiede di spostarla a mano.
>
> **Se un link non si può togliere** (directory non scrivibile) lo script si
> ferma con stato 1 e non tocca niente; con `<dir>`/`-f <dir>` lo controlla
> prima di cambiare il primo dei due link. Fino a settembre 2026 andava avanti:
> il link vecchio restava, e `ln -s` — trovando un link a una directory — creava
> quello nuovo **dentro la vecchia area** (`legopst_x/legocad/legocad`),
> dichiarando successo. Ora i link si creano con `ln -sn`.

> **L'area scelta deve avere entrambe.** Se contiene `legocad` ma non `sked` (o
> viceversa), il link mancante non viene creato e quello vecchio continuerebbe a
> puntare all'area precedente: si finirebbe a lavorare con `legocad` di un
> impianto e `sked` di un altro. `lgswitch` lo **segnala ed esce con stato 1**,
> suggerendo come rimediare. È il caso più insidioso, perché tutto sembra
> funzionare.

---

## 6. Versione e diagnosi dell'ambiente

| Comando | Tipo | Scopo |
|---|---|---|
| `lgversion` | script | Stampa **quale LegoPST stai usando**: `LEGOROOT`, il file `version.h` letto, e da lì versione Git, numero di build e data di build. Se `version.h` manca lo **genera** al volo lanciando `make -f Makefile.mk version.h` in `$LEGOROOT` (servono `git` e `make`). Fallisce subito, con un messaggio esplicito, se `LEGOROOT` non è impostata — il che lo rende anche un controllo veloce del fatto che il profilo sia stato sorgiato. |

È la prima cosa da eseguire quando si segnala un problema: senza versione e numero
di build, una segnalazione non è verificabile.

---

## 7. Conversione fra Linux e Windows

Due convertitori di applicazioni legopc. Non sono nel `PATH`: si lanciano a mano
con `python3` dalla directory `util2025/`.

| Comando | Tipo | Scopo |
|---|---|---|
| `lglinux2win.py` | tool | Converte un'applicazione legopc dal formato **Linux a Windows**: directory `libut`, `libgraph` e i modelli. Documentazione propria: [../util2025/lglinux2win.md](../util2025/lglinux2win.md). |
| `lgwin2linux.py` | tool | Il verso opposto, **Windows a Linux**: `libut`, `libgraph` e `models`. Documentazione propria: [../util2025/lgwin2linux.md](../util2025/lgwin2linux.md). |

---

## 8. Co-simulazione FMU

| Comando | Tipo | Scopo |
|---|---|---|
| `lg_cosim.py` | tool | **Master di co-simulazione FMU**: orchestra più FMU LegoPST secondo un file di configurazione, gestendo passo di integrazione e scambio delle variabili. Manuale dedicato: [../Alg_rt/lg_fmu/lg_cosim/lg_cosim_manual.md](../Alg_rt/lg_fmu/lg_cosim/lg_cosim_manual.md). |
| `lg_cosim2s01.py` | tool | Genera il file **`S01`** a partire dalla configurazione di `lg_cosim`, cioè traduce lo schema di co-simulazione nel formato che il resto di LegoPST già capisce. |

Build dei bundle FMU, HMI e diagnostica: [../Alg_rt/lg_fmu/USAGE.md](../Alg_rt/lg_fmu/USAGE.md).

> **Testare un bundle FMU da una shell col profilo sorgiato non prova nulla**: le
> variabili `LG_*` trapelano e mascherano ciò che manca nel bundle. Usare `env -i`
> per emulare una macchina senza LegoPST.

---

## 9. Motori di calcolo e debug

Raramente si lanciano a mano: normalmente li invocano il CAD, `net_sked` o i
comandi `k*`. Elencati perché compaiono nei log e nei messaggi d'errore.

| Comando | Tipo | Scopo |
|---|---|---|
| `lg1` | binario | Eseguibile della **topologia** legocad (interfaccia Motif), in `LEGOCAD_BIN`. Costruito da `legocad/topologia/`. |
| `lg1a_exe`, `lg4_exe` | binario | Motori di `lego_big`, in `LEGO_BIN` (`legocad/lego_big` è un symlink a `lego_big/`). |
| `lg3debug`, `lg5debug` | legacy | Lanciano **`dbx`** su `proc/lg3` o `proc/lg5sk` con il `core`, con gli `-I` dei sorgenti già impostati. Richiamati da `kdbx`. **Su Linux non funzionano così com'è**: `dbx` è il debugger DEC/OSF e non è installato — l'equivalente è `gdb`. Sono resti dell'epoca VMS/OSF, datati 1997. |

---

## Cosa NON è un comando

Due nomi che si incontrano leggendo il codice e che è facile scambiare per comandi:

| Nome | Cos'è davvero |
|---|---|
| `lg_pick` | **Helper interno** del profilo, non un comando. Sceglie il primo programma disponibile fra una lista di candidati per browser, editor di testo, editor di icone, visualizzatore PDF e terminale (`LG_BROWSER`, `LG_TEXTEDITOR`, `LG_ICOEDITOR`, `LG_PDFVIEWER`, `LG_XTERM`). Viene **distrutto subito dopo l'uso** (`unset -f lg_pick`), quindi digitarlo non produce nulla. Per forzare un programma, esportare la variabile **prima** di sorgiare il profilo. |
| `lgser` | Il **server LEGO lato Windows**. In `Alg_legopc/src/legosim/lgser/` ci sono solo sorgenti C e file `.bat`, e gli eseguibili `lgser.exe` stanno nelle `user_default`. **Su Linux non c'è**: il ruolo corrispondente lo coprono i bundle FMU (sezione 8). |

---

## Mappa rapida

| Voglio… | Comando |
|---|---|
| entrare nell'ambiente | `source .profile_legoroot`, poi `lggo` per tornarci |
| lavorare senza installare niente | `lgdock` (`-d` per la demo) |
| disegnare schemi e modelli | `lgpc` |
| far girare una simulazione e vederla | `lghmi` |
| aprire i faceplate di comando | `lghmi -staz` |
| comandare la simulazione da uno schema (faceplate, valori) | `lgpc` → *Add elements ▸ Faceplate / Set value*, poi *Show Value* in `lghmi`/draw2gr |
| ricompilare tutto dopo una modifica | `lgupsim` (`lgupsimx` senza MMI) |
| cambiare area di lavoro | `lgswitch`, oppure `lghmi` → *File → Work area* |
| sapere che versione sto usando | `lgversion` |
| portare un'applicazione su Windows | `python3 util2025/lglinux2win.py` |
| far dialogare più modelli come FMU | `python3 .../lg_cosim.py` |

---

*Comandi `lg*` dell'ambiente LegoPST: alias di shell, script nel `PATH`, tool
Python e motori di calcolo. Per la manutenzione fine di un simulatore vedi i 192
comandi `k*` in [../kbin/README.md](../kbin/README.md).*
