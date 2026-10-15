# Alg_rt/net_simula — Master Monitor (`new_monit`)

Il monitor principale della sessione ([Alg_rt/net_simula/new_monit/](.)) espone i menù *Programs*, *Options*, ecc. Le opzioni di sessione sono in una struttura `OPTIONS_FLAGS` ([option.h](option.h)) persistita nel file binario **`.bi_options`** (`OPTION_FILE`) nella dir di lavoro del simulatore (`FILES_PATH`), letto all'avvio da `read_options()` (`SD_optload`).

## Il selettore HMI (`lghmi`) dal menù del monitor

Oltre agli *User Programs* configurabili, il monitor ha una voce che apre il
**selettore delle HMI**: [`attiva_lghmi`](cont_rec.c) (chiamata da due punti di
[`masterMenu.c`](masterMenu.c)) fa

```c
system("$LEGORT_BIN/lghmi -insim &");     /* HMI_PROGRAM + HMI_PROGRAM_OPT */
```

Nessun dialogo di display: parte sul `DISPLAY` corrente, e ogni pressione apre
una nuova istanza.

**`-insim`** dice al selettore che è stato lanciato da dentro una simulazione in
corso, e gli fa disabilitare i comandi che sarebbero dannosi in quel contesto:
*File → Open Simulator path* (lo scollegherebbe da questa simulazione), il
pulsante *net_startup* (che con `killsim` ammazzerebbe proprio questa
simulazione, e il banco con lei), *File → Work area* (cambierebbe l'area di
lavoro sotto la simulazione) e il menu *Edit* delle HMI che lancia (aprirebbe
nel CAD i modelli in esecuzione). Dettagli in
[Alg_legopc/LGHMI.md](../../../Alg_legopc/LGHMI.md#lanciato-dal-banco--lopzione--insim).

## User Programs — comandi utente lanciabili dal monitor

Meccanismo per lanciare comandi shell arbitrari dal monitor, in due parti:

- **Esecuzione** — menù *Programs → User programs …* apre il pannello `programLauncher` ([programLauncher.c](programLauncher.c)) con fino a **8 pulsanti radio**, uno per comando configurato non vuoto ([`loadPrograms`](options.c#L960)). Selezione → `selectedCommand`; **Execute** → [`system(selectedCommand)`](programLauncher.c#L121). L'etichetta del pulsante è la stringa del comando.
- **Configurazione** — menù *Options → Edit* apre `optionSet`; nel selettore *Current Selection:* scegliere **User Programs** → pannello con **8 campi di testo** ([`add_opt_userprog`](options.c#L490), `optionUserprogText[i]`) precompilati con i comandi correnti. Si scrive la riga di comando nello slot.

**Vincoli**: max **8** programmi, max **100** caratteri per comando ([`MAX_USERPROG` / `MAX_USERPROG_LUN`](option.h#L21)). `system()` è **bloccante** e eredita cwd/ambiente di `new_monit` (dir simulatore): per una GUI/script terminare con **`&`** (es. `xterm &`). Slot vuoto = pulsante non mostrato (per rimuovere: svuota il campo e salva). Config **per-directory** (ogni sim ha il suo `.bi_options`).

## `optionSet` — Save / Load (editor opzioni)

Nell'editor *Options → Edit* i due comandi persistono/ripristinano l'intera struttura opzioni (non solo User Programs):

- **Save** ([`activateCB_optionSetMenuSavepb`](optionSet.c#L183)): `aggiorna_opzioni` copia i widget → struttura `options`, poi `SD_optsave3` scrive `.bi_options`.
- **Load** ([`activateCB_optionSetMenuLoadpb`](optionSet.c#L192)): `read_options()` **rilegge `.bi_options`** (ultimo stato salvato) nella struttura in memoria → **scarta le modifiche non salvate**, poi chiude l'editor. In precedenza dopo `DistruggiInterfaccia` veniva richiamato `aggiorna_opzioni(&options)`, che ricopiava i widget della pagina corrente dentro `options` vanificando il reload sulla pagina visualizzata: **richiamo rimosso** (fix), ora il Load ripristina l'intera struttura in modo completo.

Il copia widget→struttura avviene invece nel bottone *Apply* ([`activateCB_pushButton6`](optionSet.c#L165) → `aggiorna_opzioni`): **Save non lo richiama**, quindi il flusso è *modifica campi → Apply → Save*.

## Avvio: i file delle registrazioni e i parametri del `Simulator`

All'avvio `dispatcher`, `net_sked`, il banco e `net_prepf22` passano tutti da
`ControlParam` ([AlgLib/libsim/simulator.c](../../../AlgLib/libsim/simulator.c)),
che confronta i parametri in uso — quelli del file `Simulator`: numero di
snapshot, di backtrack, di campioni, di variabili… — con quelli scritti
nell'intestazione di `snapshot.dat`, `backtrack.dat` e `f22circ.dat`. Le
differenze finiscono in **`parametri.out`**, nella directory del simulatore:
`WARNING` quelle tollerate, `SEVERE` quelle che contano. È il primo file da
guardare quando il banco stampa `Errori in fase di Startup (sk=N disp=N
monit=N shm=N)`: `N` vale 1 per `snapshot.dat`, 2 per `backtrack.dat`, 4 per
`f22circ.dat`, sommati.

`parametri.out` **ricomincia a ogni avvio** (da ottobre 2026): lo apre da capo
il `dispatcher`, il primo processo a partire, con la riga "Avvio del <data>", e
quello dell'avvio precedente resta come `parametri.out.prec`. Prima si scriveva
sempre in coda e cresceva per sempre: righe `SEVERE` vecchie di giorni
sembravano dell'avvio appena fatto.

**`f22circ.dat` incompatibile non ferma più l'avvio** (da ottobre 2026). È solo
la registrazione circolare per i grafici, non uno stato da cui si riparte: il
primo processo che lo trova con parametri diversi lo rinomina in
`f22circ.dat.incompatibile` e prosegue, e `net_prepf22` ne crea uno nuovo. Lo
dice sullo stdout e in `parametri.out` (`RIMEDIO: ...`). Prima la simulazione
non partiva più finché qualcuno non cancellava il file a mano — capitava dopo
una corsa FMU della stessa task, o cambiando il `Simulator`.

Per `snapshot.dat` e `backtrack.dat` con **parametri** diversi l'errore
**resta**: contengono gli stati salvati, e scartarli è una decisione di chi usa
il simulatore.

**Salvo un caso: un'altra topologia** (da ottobre 2026). L'intestazione di
`snapshot.dat` e `backtrack.dat` registra anche la dimensione dell'area dati e,
task per task, il numero di stati, ingressi, derivate e dati. Se non
corrispondono alla topologia attuale (letta da `S02_LINUX`), il file è stato
salvato prima di una modifica del modello e caricarlo vorrebbe dire ripartire
da valori sfalsati, senza che niente lo dica. Non si può usare in nessun caso:
il primo processo che se ne accorge lo rinomina in `snapshot.dat.incompatibile`
(o `backtrack.dat.incompatibile`), lo scrive sullo stdout e in `parametri.out`
(`RIMEDIO: ... salvato con un'altra topologia ...`), e la simulazione parte con
un file nuovo. Lo si è scoperto con `snaf14` su una demo, con uno snapshot di
settembre e 56 variabili in più. Codice: `ScartaSeAltraTopologia` in
`AlgLib/libsim/simulator.c`; la CI lo prova a ogni push.

## Due strumenti sugli stati della simulazione

### Da uno snapshot alle condizioni iniziali: `snaf14`

Nella directory del simulatore (o di una task), `snaf14` elenca gli snapshot di
`snapshot.dat` e, scelto il numero, scrive accanto al `f14.dat` di ogni modello
un **`f14.s`** con i valori di quello snapshot. Il `f14.dat` non viene toccato:
per usarli si rinomina `f14.s` a mano. I parametri li legge dal `Simulator`.

```
cd $KSIM            # o la directory della task
snaf14              # invio, poi il numero dello snapshot (quello dell'elenco)
```

Uno snapshot salvato **con un'altra topologia** (prima di una modifica del
modello) viene rifiutato con una spiegazione e senza scrivere niente: i valori
sarebbero sfalsati. Va rifatto lo snapshot con la simulazione attuale. Il
programma (`Alg_rt/snaf14/sna2f14.c`) è tornato nel build a ottobre 2026, con
alcune correzioni: vedi [docs/COMPONENTI_RIMOSSI.md](../../../docs/COMPONENTI_RIMOSSI.md).

### Il database della simulazione in memoria: `dump_shr`

Con una simulazione in corso, `dump_shr` scrive in un file tutte le variabili
di una task, con il loro indirizzo e il valore attuale nella memoria
condivisa: ingressi e uscite, poi derivate e dati.

```
dump_shr 1 dump.txt     # task numero 1 (nell'ordine di S01) nel file dump.txt
dump_shr 1 term         # sullo schermo
dump_shr                # senza argomenti: elenca le task e chiede
```

Serve a guardare lo stato completo di una task quando qualcosa non torna (per
una variabile sola basta `viewval`, o `probe_attach` di `lg_fmu/tools`). Tornato
nel build a ottobre 2026 (`Alg_rt/net_simula/dump_shr`).
