# `xstaz` / `compstaz` — faceplate di comando (stazioni)

Una **stazione** è un faceplate: un gruppetto di LED, pulsanti, display e
impostatori che comanda e sorveglia una porzione della regolazione. Le stazioni
sono raggruppate in **pagine**. Due programmi:

| Programma | Dove | Ruolo |
|---|---|---|
| **`compstaz`** | `Alg_rt/bin` ([sorgente](../compstaz/compstaz.c) in `Alg_rt/grafica/compstaz/`) | compila il file di configurazione `r01.dat` e produce `r02.dat` |
| **`xstaz`** | `Alg_rt/bin` ([xstaz.c](xstaz.c)) | applicazione Motif che legge `r02.dat` e disegna le pagine di stazioni |
| **`stazpag`** | `Alg_rt/bin` ([stazpag.c](stazpag.c)) | elenca le pagine di `r02.dat` e ne chiede la visualizzazione a `xstaz` |

Il nome del compilatore è **`compstaz`** (non *xcompstz*). Esiste anche
`Alg_mmi/bin/convstaz`: stesso `r01.dat` in ingresso, ma genera le pagine per
LEGOMMI invece del binario per `xstaz`.

## Il ciclo di lavoro

```sh
cd <dir della regolazione>      # deve contenere r01.dat e variabili.rtf
compstaz                        # -> r02.dat + compstaz.log
xstaz 1                         # visualizzatore (di norma lo lancia net_monit)
```

In una catena di aggiornamento conviene usare la procedura
[`kCompStaz`](../../../kprocedure/kCompStaz.sh) invece del binario nudo: fa la
stessa cosa in `$KSIM`, ma restituisce un **exit status utilizzabile** (vedi
*Trappole*) e controlla che `variabili.rtf` ci sia.

`compstaz` **non prende argomenti** e lavora interamente nella directory
corrente. Prima di leggere `r01.dat` aggancia la **shared memory della topologia**
(chiave `SHR_USR_KEY + 5`) dimensionandola su `variabili.rtf`: serve perché ogni
riga `INPUT`/`OUTPUT` cita una variabile e un modello, che vengono risolti contro
la topologia del simulatore. Se `variabili.rtf` manca si ferma subito con
*"Errore il file variabili.rtf non esiste"*.

Gli errori di compilazione finiscono in **`compstaz.log`** oltre che a video, e
sono fatali: alla prima riga malformata il programma esce.

### Il gemello `convstaz`, per l'MMI

Lo **stesso `r01.dat`** può essere compilato anche da
[`convstaz`](../../../Alg_mmi/conv_staz/), che appartiene al mondo `Alg_mmi`: è un
fork di `compstaz` — stesso parser, stessi moduli per tipo di oggetto — che invece
del binario produce **una pagina `<NOME>.pag` per ogni blocco `PAGINA`**, cioè lo
stesso faceplate reso dall'MMI anziché da `xstaz`.

Le due strade sono indipendenti e non vanno confuse:

| | `compstaz` | `convstaz` |
|---|---|---|
| uscita | `r02.dat` (binario) | `<NOME>.pag` (risorse X, `*top_tipo: Stazioni`) |
| la legge | `xstaz`, `net_monit` | l'MMI, dopo compilazione in `.rtf` |
| serve `variabili.rtf` | **sì**, risolve gli indici delle variabili | no: i suoi `check_*` sono stub |

`convstaz` **non** scrive `r02.dat` (la scrittura, ereditata dal progenitore e
per giunta con indici sempre a 1, è stata rimossa nel settembre 2026): il file
binario lo produce solo `compstaz`.

Con l'opzione `-d <dir>` `convstaz` deposita pagina e sfondo dove serve, e la
procedura [`kStazPages`](../../../kprocedure/kStazPages.sh) porta i faceplate
descritti nei `r01.dat` fin dentro l'MMI. Il dettaglio è in
[Alg_mmi/README.md](../../../Alg_mmi/README.md#5-da-dove-vengono-le-pagine-i-tre-generatori).

La pagina MMI riusa **la stessa tavolozza di `xstaz`** (`sfondo_window`,
`sfondo_staz`, `sfondo_label` di [xstaz.c:139-143](xstaz.c#L139-L143)), con una
sola eccezione: la finestrella del display numerico è **scura**. Qui il valore è
una `XmLabel` con foreground nero fisso, là è un `XlIndic` che colora le cifre
secondo lo stato del punto, con una tavolozza satura pensata per fondo scuro.
Dettaglio e tabelle in
[Alg_mmi/README.md](../../../Alg_mmi/README.md#i-colori-della-pagina-generata).

`xstaz` prende **un solo argomento**, `tipo_staz`:

- **`xstaz 1`** — dialogo con **MONIT**: aggancia il DB punti condiviso
  (`DB_PUNTI_SHARED`) e resta in attesa di richieste sulla coda di messaggi.
- `xstaz 0` — dialogo con **LEGOGRAF**: percorso storico VMS (attende gli event
  flag, fa `chdir` su `S04_PATH`), non usato su Linux.

Legge `r02.dat` **dalla directory corrente**; se non c'è esce con *"file r02.dat
non esistente"*. Ha bisogno di `DISPLAY`, `SHR_USR_KEY`, `SHR_TAV_KEY` e
`LEGORT_BIN`.

## Chi lo pilota

**Attenzione: dipende da come è stata avviata la simulazione.** Gli script di
avvio non lanciano tutti la stessa interfaccia:

| Script | Schedulatore | Interfaccia | Ha il dialogo delle stazioni? |
|---|---|---|---|
| `net_startup` | `net_sked 2` | `banco SUPERUSER` (da `new_monit`) | **no** |
| `net_simula` | `net_sked 2` | `net_monit` | sì |
| `simula` | `net_sked 1` | `net_monit` | sì |
| `cad_simula` | `cad_sked` | `cad_monit` | — |

Il pulsante che lancia `xstaz` esiste **solo in `net_monit`**: chi avvia con
`net_startup` ha il banco, che non lo ha.

### Il pulsante delle stazioni è spento finché non si inizializza

Le voci del menu *Control* di `net_monit` seguono lo stato dello schedulatore
([monit_menu.h](../../net_simula/net_monit/monit_menu.h)), e appena avviato lo
stato è **STOP**, dove è abilitata **solo `Initialize`**:

| Stato | Voci abilitate |
|---|---|
| **STOP** (all'avvio) | `Initialize`, `Quit`, i tre `Config` |
| **FREEZE** (dopo Initialize) | `Run`, `Backtrack`, `Clear`, `Stop`, `Load`/`Save`/`Edit`, `Statistics`, `Input`, **stazioni**, `Graphics` |
| **RUN** | solo `Freeze` |
| **ERROR** | solo `Quit`, `Save`, `Edit`, `Graphics` |

Quindi `Run` **e il pulsante delle stazioni** risultano grigi finché non si fa
*Control → Initialize*, che manda `SD_inizializza(MONIT)` allo schedulatore. Lo
stato corrente si legge nell'etichetta in alto (`STOP`/`RUN`/`FREEZE`/
`BACKTRACK`/`REPLAY`/`ERROR`). Non è un malfunzionamento: è la sequenza prevista.

**I colori sembravano invertiti.** Le voci di `monit.uil` impostavano solo
`XmNbackground` (MediumSpringGreen) e mai `XmNforeground`: in quel caso Motif ne
sceglie uno suo, che con quello sfondo è **bianco** (verificato interrogando il
widget: `foreground = 255,255,255`). Le voci **abilitate** risultavano quindi
quasi illeggibili, mentre quelle **disabilitate** — disegnate con le ombre grigio
scuro, `127,127,127` e `76,76,76` — si vedevano benissimo: l'esatto contrario di
quel che ci si aspetta. Dalla revisione di agosto 2026 **tutti** i widget di menu hanno un
`XmNforeground = testo_menu` esplicito (nero): le 27 voci dei pulldown e i 16
`XmCascadeButton`, cioè le voci della barra in alto (`Control`, `Configure`,
`Initial Condition`, `Statistics`, `Input`, `Graf`, `Mf & Fr`, `About`) e i
sotto-menu. Le voci dei pulldown usano inoltre il font `grassetto`
(`-adobe-helvetica-bold-r-normal--17-120-100-100-p-92-iso8859-1`, stessa misura
del precedente `normal`), così l'abilitato stacca nettamente dal disabilitato,
che Motif disegna sbiadito. Il colore vive nel `.uid`, che si rigenera con
`uil` (`make -f Makefile.mk` in `Alg_rt/net_simula/net_monit`): perché la
modifica si veda basta **riavviare `net_monit`**, non serve ricompilarlo.

Nota: `stazpag` non passa dal menu e quindi non ha questo vincolo, ma i valori
mostrati dai faceplate restano fermi finché la simulazione non è inizializzata.

### Aprire una pagina senza `net_monit`

Il modo più comodo è **`lghmi`**: apre un selettore grafico con, a destra,
l'elenco delle pagine (nome, descrizione, numero di stazioni); avvia `xstaz` se
non è già attivo e gli manda la richiesta. A sinistra ci sono le pagine di
processo, così le due interfacce della simulazione si aprono dalla stessa
finestra; con `-staz` si limita ai faceplate, con `-proc` al processo. Vedi
[Alg_legopc/LGHMI.md](../../../Alg_legopc/LGHMI.md). Sotto, il meccanismo a riga
di comando su cui si appoggia.

`xstaz` da solo non serve a niente: all'avvio crea unicamente una finestrella
**iconificata** con il tasto *Quit* e poi aspetta un messaggio sulla coda
`SHR_USR_KEY + ID_MSG_STAZ` (= `+12`), tipo `RIC_STAZ`, con il nome della
pagina. Non esiste un elenco interno da cui scegliere.

`stazpag` manda quel messaggio:

```sh
cd <dir con r02.dat>
stazpag                 # elenca le pagine: NOME, DESCRIZIONE, n. stazioni
xstaz 1 &               # avvia il visualizzatore (resta iconificato)
stazpag RISCBP          # chiede la pagina: si apre la finestra del faceplate
```

`stazpag` verifica che il nome esista in `r02.dat` prima di spedire, e se la
coda non c'è dice che la simulazione non è avviata invece di fallire in
silenzio. In alternativa si può lanciare `net_monit` a mano — usa una coda diversa da
quella del banco (`ID_MSG_MONIT` 3 contro `ID_MSG_BANCO` 4) — ma **solo con la
simulazione già avviata**: `net_monit` non è autosufficiente, all'avvio si
aggancia al `dispatcher` e ne aspetta l'ACK. Senza `dispatcher` e `net_sked`
resta fermo dopo aver stampato

```
to_dispatcher: 2 GUAG id_sem = 38 id=131105
```

per **675 secondi** (`TIMEOUT_DISP` = `TIMEOUT_BASE` 45 x 15), poi stampa
`TIMEOUT SCADUTO` e `fallita msgrcv (ACK)`. Se serve interromperlo prima:
`net_monit` **blocca SIGTERM** ([monit.c:615](../../net_simula/net_monit/monit.c#L615)),
quindi `kill` non basta e ci vuole `kill -9`. Per avviare tutto in ordine si usa
`net_simula`, che lancia `dispatcher`, `net_sked` e `net_monit` con le pause
giuste.

Nota: chiudere `net_monit` con `kill -9` salta `quit_proc()`, quindi niente
`SD_sgancio(MONIT)`: la registrazione presso il dispatcher resta appesa. Meglio
usare il Quit della finestra.
Attenzione: fino ad agosto 2026 `net_monit` moriva di SIGSEGV all'avvio, prima
di mostrare la finestra, per una chiamata K&R senza argomenti a `malf_proc()`
(vedi [docs/KR_PROTOTYPE_AUDIT.md](../../../docs/KR_PROTOTYPE_AUDIT.md)); se
succede ancora, il binario è vecchio e va ricompilato.


Normalmente non si lancia a mano: lo fa **`net_monit`**
([monit_staz.c](../../net_simula/net_monit/monit_staz.c)), che mostra l'elenco
delle pagine (nome + descrizione, letti da `r02.dat`), e alla conferma:

1. se `xstaz` non è già vivo lo lancia come `$LEGORT_BIN/xstaz 1`, passandogli un
   ambiente ristretto (`DISPLAY`, `SHR_USR_KEY`, `SHR_TAV_KEY`, `LEGORT_BIN`,
   `HOME`, `DEBUG`, `N000`–`N007`, `M001`–`M005`);
2. gli manda sulla coda un messaggio `RICHIESTA_STAZ` (`mtype = RIC_STAZ`) con il
   **nome della pagina**.

`xstaz` scoda la richiesta nel proprio timer e apre la pagina, fino a **20 pagine
contemporanee** (`MAX3_PAG`). L'MMI ha inoltre un pulsante dedicato
(`drawnButtonXstaz` in `Alg_mmi/run_time/topLevelShell1.c`).

## Formato di `r01.dat`

> Questa è la sintesi. Il formato completo — tutti i tipi di stazione con il
> loro template, il significato di ogni parametro, un `r01.dat` di esempio e la
> checklist operativa — sta nel **[HOWTO faceplate](HOWTO_faceplate.md)**.


File di testo a record separati da una riga di soli `****`, chiuso da
`END_OF_FILE`. Due tipi di record: `PAGINA` e `STAZIONE`.

```
****
PAGINA
NUMERO       1
NOME         RISCBP
DESCRIZIONE  Livello RISCALDATORI BP
```

`NUMERO` è l'identificatore citato dalle stazioni (1..500, non serve che sia
contiguo: `compstaz` mantiene la tabella di svincolo fra numero dichiarato e
indice reale); `NOME` è quello che `net_monit` invia per aprire la pagina.

```
****
STAZIONE
NUMERO       1
TIPO         DISPSCAL
DESCRIZIONE
PAGINA       1
POSIZIONE    1   11
STRINGA
ETICHETTA    SET LIV R3BP (norm)
DISPLAY_SCALATO
INPUT        US2S00CI   CICA
SCALAMENTO      100.
OFFSET          0.
```

L'ordine è obbligatorio: `NUMERO`, `TIPO`, `DESCRIZIONE`, `PAGINA`, `POSIZIONE`,
poi **un blocco per ogni oggetto previsto dal tipo**, nella sequenza esatta in cui
il tipo li dichiara (la tabella `new_staz[]` in
[newstaz.h](../../../AlgLib/libinclude/newstaz.h)); se ne manca uno o è fuori
ordine, `compstaz` stampa *"oggetto previsto: ..."* ed esce. `NUMERO` per le
stazioni è ignorato: l'indice reale è la posizione progressiva nel file.
`POSIZIONE x y` è in celle, non pixel: larghezza e altezza le mette il tipo.

### Righe degli oggetti

| Riga | Significato |
|---|---|
| `COLORE <nome>` | `NERO BIANCO GIALLO VERDE ROSSO GRIGIO BLU` |
| `ETICHETTA <testo>` | testo mostrato |
| `INPUT <variabile> <modello>` | grandezza letta; il modello è risolto sulla topologia |
| `INPUT_ERR`, `INPUT_BLINK` | ingresso di errore / lampeggio (riga anche vuota) |
| `OUTPUT <variabile> <modello> <tipo>` | **comando**: cosa scrive il faceplate |
| `SCALAMENTO`, `OFFSET` | `visualizzato = SCALAMENTO * valore + OFFSET` |
| `MINMAX`, `MINMAX_ERR`, `SCALAMENTO_ERR` | fondoscala e varianti per l'ingresso di errore |
| `INIBIZIONE`, `NOT` | condizione di inibizione; negazione logica |

Il `<tipo>` di `OUTPUT` è la **modalità di perturbazione**: `STEP` (impone il
valore), `IMPULSO` (impulso), `NEGAZIONE` (commuta), `UP_DOWN` (incrementa/
decrementa). Una riga `OUTPUT` senza campi lascia il comando scollegato.

Esempio di stazione **di comando** completa (impostatore con indicatore):

```
STAZIONE
NUMERO       5
TIPO         AGERSETV
DESCRIZIONE  SCAR A RBP2
PAGINA       1
POSIZIONE    5   12
SET_VALORE   1.
ETICHETTA    [%]
OUTPUT       IV2500CI   CICA   STEP
OUTPUT       J22500CI   CICA   IMPULSO
INPUT        UP2500CI   CICA
SCALAMENTO      100.
OFFSET          0.
INIBIZIONE
INDICATORE
...
```

### Tipi di stazione

13 tipi "storici" (`tipi_old_staz[]`): `SA1 SP1 SPD ID1 BR1 TR1 MR1 LU1 AM1 AM2
AM3 AMD SD1`, più 54 tipi "nuovi" definiti in `new_staz[]`, fra cui i più usati:

- **testo/display**: `TESTO`, `TESTOBIS`, `DISPLAY`, `DISPSET`, `DISPSCAL`, `DISSETSC`
- **segnalazioni**: `LEDS2 LEDS3 LEDS4 LEDS6 LEDR2 LEDR3 LEDR4 LEDR6 …`, `LAMP1`, `LAMP2`
- **comando**: `P1L0 P1L1 P1L2 P1L3 P2L0 P2L2 P2L3 P3L3 PL1 PL2L2 PL3L3 …`
  (pulsanti con 0–3 spie), `SELET_A`, `SELET_B`, `TASTO`
- **impostatori/indicatori**: `IAGO`, `IAGOERR`, `AGOSETV`, `AGERSETV`, `IBARRA1`,
  `IBARRA2`, `SINCRONO`, `MIXER`

Gli oggetti elementari che compongono un tipo sono `LED`, `PULSANTE`,
`PULS_LUCE`, `LAMPADA`, `SELETTORE`, `INDICATORE`, `STRINGA`, `DISPLAY`, `LUCE`,
`TASTO`, `SET_VALORE`, `DISPLAY_SCALATO`.

### Limiti

`MAX_PAG` 500 pagine, `MAX_STAZ` 2000 stazioni, `MAX_OGG` 200 stazioni per
pagina, `MAX3_PAG` 20 pagine aperte insieme.

Nomi: `LUN_NOM_PAG` e `LUN_NOM_STAZ` valgono **8 caratteri**, `LUN_DES_PAG` 50.
Il `NOME` di una pagina piu' lungo di 8 caratteri sforava nel campo descrizione
che segue nella struttura; da settembre 2026 `compstaz` e `convstaz` lo rifiutano
con un messaggio.

## Trappole

- **`compstaz` restituisce 24 anche quando va bene.** Termina con
  `exit(puts("\nFine corretta COMPSTAZ"))`, e `puts` restituisce il numero di
  caratteri stampati: 24 in caso di successo, 42 in caso di errore di sintassi.
  Due conseguenze: in una catena `cmd1 && compstaz && cmd2` la catena **si ferma
  dopo `compstaz`** anche se è andato tutto bene, e l'exit status non distingue
  il successo dall'errore. L'unico segnale affidabile è l'output (`Fine corretta`
  contro `termina per errore`): è così che lo verifica
  [`kCompStaz`](../../../kprocedure/kCompStaz.sh), che espone 0 o 1 come si deve.
  Lo stesso vale per `convstaz`, che condivide il codice.
- **`r02.dat` va rigenerato dopo aver ricompilato le task.** `net_compi` riscrive
  `variabili.rtf`, e `compstaz` risolve gli indici delle variabili contro quel
  file: se le task vengono ricompilate e i faceplate no, gli indici possono non
  corrispondere più e le stazioni leggono punti sbagliati. Il sintomo è muto —
  nessun errore, solo valori inattesi. Un `ls -l r02.dat variabili.rtf` dice
  subito se il primo è più vecchio del secondo.
- **Topologia di un altro modello in memoria.** Se alla chiave
  `SHR_USR_KEY + 5` c'è già un segmento che non corrisponde al modello da
  compilare, `compstaz` non può proseguire. Fino ad agosto 2026 il sintomo era un
  **SIGSEGV muto**: `crea_shrmem()` restituiva `NULL` ma nessuno lo controllava e
  `costruisci_var()` lo dereferenziava subito
  ([var_sh.c](../../../AlgLib/libsim/var_sh.c)). Ora
  [shrmem.c](../../../AlgLib/libsim/shrmem.c) stampa chiave, dimensione
  richiesta, errno in chiaro e lo stato del segmento che occupa la chiave
  (dimensione, processi agganciati, pid del creatore), e il chiamante esce con
  un messaggio invece di schiantarsi. Dettagli e rimedi nel capitolo 11 del
  [HOWTO](HOWTO_faceplate.md).
- **I valori non si aggiornavano affatto** (corretto in agosto 2026): display
  fermi sulla loro etichetta iniziale `----`, led spenti, indicatori immobili.
  Erano due difetti in fila.
  1. In `timer_proc()` ([xstaz.c](xstaz.c)) le due chiamate alle funzioni di
     refresh erano **commentate** e sostituite da `t_call[i].callback;`, cioè
     un'espressione senza effetto che il compilatore non segnala. Le funzioni
     registrate da `add_refresh()` prendono un solo argomento pur essendo
     conservate in un `XtCallbackRec`, quindi vanno chiamate con il cast alla
     loro vera firma: `((void (*)(caddr_t))t_call[i].callback)(t_call[i].closure)`.
  2. Ripristinate le chiamate, `xstaz` andava in **SIGSEGV** dentro
     `XmStringDraw` → `_XmStringIsCurrentCharset`. Causa: sei punti fra
     `gdisplayscal.c`, `gdisplay.c`, `sd1_r.c` e `amd_r.c` liberavano le
     `XmString` con `XtFree()` invece di `XmStringFree()`. `XtFree` rilascia
     solo il blocco esterno e lascia in giro le strutture interne di Motif,
     corrompendone le tabelle di rendition. Con il refresh disattivato quel
     codice non girava mai: disabilitando l'aggiornamento si era tolto il
     sintomo insieme alla funzione.
- **Chiudere una pagina con la X del window manager** non la rendeva più
  riapribile fino al riavvio di `xstaz` (correzione di agosto 2026). Le pagine
  aperte sono registrate in `pagvis[]`, e solo il *quit* del menu contestuale
  passava da `pag_del_callback`, che azzera `attiva` e il puntatore al widget.
  La X del window manager seguiva invece il default Motif
  (`XmNdeleteResponse = XmDESTROY`): distruggeva la finestra senza toccare la
  registrazione, così la pagina restava marcata come aperta — e `cnew_staz` la
  scartava — con in più un puntatore penzolante in `pagvis[].w`. Ora la shell
  della pagina è creata con `XmDO_NOTHING` e un `XmAddWMProtocolCallback` su
  `WM_DELETE_WINDOW` che richiama la stessa callback del *quit*: le due chiusure
  sono equivalenti.
- **`compstaz` non ha un codice di uscita affidabile** su tutti i percorsi: si
  controlla `compstaz.log`.
- `xstaz` e `compstaz` lavorano **entrambi nella directory corrente**: nessuna
  opzione per indicare i file altrove.
