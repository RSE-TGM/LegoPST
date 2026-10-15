# Componenti rimossi dal repository

Registro di quello che è stato tolto da `master`: **cosa**, **perché** e
**come recuperarlo**. Niente di ciò che è elencato qui è perso: resta nella
storia di git, e si riporta indietro con un comando.

Le rimozioni vengono dal [piano di intervento](PIANO_INTERVENTO.md) (fase 4,
triage delle directory fuori dal build) e da pulizie fatte prima.

## Come si recupera

Ogni voce indica il **commit di riferimento**: l'ultimo che contiene il
componente. Da lì:

```bash
# riportare una directory (o un file) nell'albero di lavoro, com'era
git checkout <commit> -- <percorso>

# solo guardarla, senza toccare l'albero di lavoro
git show <commit>:<percorso/file>
git ls-tree -r --name-only <commit> -- <percorso>

# estrarla in una directory a parte, per consultarla
git archive <commit> <percorso> | tar -x -C /tmp/recupero
```

Riportata la directory, va rimessa la riga nel makefile che la compila (è
indicata per ogni voce, quando c'era) e, se il componente aveva un comando
installato, ricompilato o ricopiato quello.

Per non doversi ricordare l'hash, conviene un tag sul commit di riferimento:

```bash
git tag prima-fase4 d9b500a2      # una volta sola
git checkout prima-fase4 -- Alg_rt/net_simula/monitor
```

## Fase 4, ottobre 2026

Commit di riferimento: **`d9b500a2`** (*ci: prove di funzionamento dopo il
build (fase 8)*, 2026-10-14), l'ultimo che contiene tutto quello che segue.

Nessuno di questi componenti era nel build da tempo: le directory non erano
raggiunte dal makefile della radice e non compilavano con GCC 14/15, o non
avevano più i sorgenti. Prima di rimuoverle se ne è cercato ogni riferimento
in makefile, script, Tcl, documentazione, e le si è compilate su una copia per
misurare gli errori.

| Rimosso | Cos'era | Perché è stato tolto |
|---|---|---|
| `Alg_rt/net_simula/agg_manovra/` (1 file: `Makefile.mk`) | collegamento al **banco manovra** fisico: i programmi `agg_manovra`, `agg_manovra_stato`, `agg_manovra_pert`, `bm` | Era rimasto **solo il makefile**: i sorgenti (`agg_manovra.c`, `agg_manovra_prolog.c`, `agg_manovra_task.c`, `scoda_dispatcher.c`, `send_manovra.c`, `agg_manovra_stato.c`, `agg_manovra_pert.c`) non ci sono, nemmeno nella storia di git, e in `bin/` nessun eseguibile. Non recuperabile da qui. `net_sked` lo lancerebbe solo se l'`S01` dichiarasse un host di banco manovra; l'header `AlgLib/libinclude/agg_manovra.h`, usato da `net_monit` e `dispatcher`, **resta** |
| `Alg_mmi/starter/` (2 file: `Makefile`, `Makefile.mk`) | avvio della versione stand-alone di SCADA/MMI | Solo i makefile: `starter.c` non c'è. Nessuno lo usava. (`scada/starter`, che produce `starter_tpl`, è un'altra directory e non è toccata) |
| `Alg_rt/net_simula/monitor/` (8 file: `monitor.c`, `monitorInterface.c/.h/.i`, `monitor.prj`, `UxXt.c/.h`, `Makefile.mk`) | il vecchio **monitor** della simulazione, interfaccia Motif generata con AIC | Sostituito da `net_monit`/`new_monit`; nessuno lo lanciava; 9 errori con GCC 14. **Attenzione**: `net_monit` prendeva da qui `UxXt.h` (con un `-I` nel suo makefile); quel file ora sta in `Alg_rt/net_simula/net_monit/`, identico. Lo ha scoperto la CI, non la ricerca dei riferimenti |
| `legocad/menugraf/` (35 file: `menu_graf.c`, `Menu_graf.c`, `CalcWindow.c`, `FilterWindow.c`, `f22io.c`, `clip.c`… e i `.i` di AIC) | il vecchio **menu dei grafici** di legocad | Sostituito da `graphics`/`grafics`; nessun riferimento; 143 errori con GCC 14 |
| `Alg_rt/net_simula/net_tast/` (2 file: `net_tast.c`, `Makefile.mk`) | **comandi alla simulazione da tastiera** (start, freeze, stop, backtrack) | Lo usava solo `tsimula.sh`, uno script di prova; lo stesso lo fanno il banco e `probe_step`; 33 errori con GCC 14 |
| `Alg_rt/procedure/tsimula.sh`, e la sua regola in `Alg_rt/procedure/Makefile.mk` | script di prova: `dispatcher` + `net_sked` + `net_tast` | Senza `net_tast` non ha più senso |
| `Alg_rt/bin/tsimula`, `Alg_rt/bin/net_tast` | i due comandi installati | Eseguibili vecchi (aprile 2025) dei due sopra |
| riga `cd ./net_tast` (commentata) in `Alg_rt/net_simula/Makefile.mk` | — | Puntava a una directory che non c'è più |

Restano fuori da questa tabella, per scelta:

- **SCADA**: rimosso subito dopo, vedi la sezione seguente.
- **`legocad/legoserver/`** e **`Alg_mmi/resource/VARIABILI/`**: sembravano
  orfane, ma non lo sono. Il primo lo compila `cad_crealibut.sh` a richiesta,
  quando si crea una libreria di moduli; i sorgenti del secondo li compilano i
  makefile di `config` e di `mmi`.

### Recuperati, invece, nella stessa fase

| Componente | Cosa è stato corretto |
|---|---|
| `Alg_rt/snaf14/` → **`sna2f14`** (comando `snaf14`): da uno snapshot a un `f14.s` con le condizioni iniziali, per ogni modello | `ControlParam` vuole ora il codice del processo, non il nome: nuovo codice `SNAF14` in `dispatcher.h`, con il suo nome in `NomeProcesso`. Il numero mostrato nell'elenco degli snapshot è quello da scegliere, e la scelta accetta da 1 all'ultimo (prima l'ultimo era escluso, e 0 leggeva fuori dall'array). Etichette `MAX_CAMPIONI`/`NUM_VAR` stampate scambiate. **Uno snapshot salvato prima di una modifica del modello viene rifiutato** con una spiegazione, confrontando `size_area_dati` dell'intestazione del file con la topologia attuale: prima si scriveva un `f14.s` di valori senza senso, annunciando *successfull*. Rientrato nel build (`Alg_rt/Makefile.mk`) |
| `Alg_rt/net_simula/dump_shr/` → **`dump_shr`**: dump del database della simulazione in SHM. Uso: `dump_shr <n. task> <file>` (`term` per lo schermo), o senza argomenti per sceglierli | Tre errori (firma di `RtCreateDbPunti`, `main` senza tipo, `costruisci_var` non dichiarata), `db`/`-lsqlite3` come negli strumenti di `lg_fmu/tools`, e un `-I$(LEGOROOT_INCLUDE)` vuoto nel makefile che si mangiava il `-c`. Rientrato nel build (`Alg_rt/net_simula/Makefile.mk`) |

Provati tutti e due nel container, sulla demo `nuclear`: `dump_shr` sulla
simulazione viva (3977 righe, valori coerenti con lo stato), `snaf14` su uno
snapshot salvato dalla simulazione attuale (valori dello stato nel `f14.s`) e
su quello vecchio della demo (rifiutato).

## SCADA, ottobre 2026

Commit di riferimento: **`d9b500a2`** anche qui (l'ultimo che contiene tutto).

**Perché.** Il sottosistema non era più nel build (la riga `cd ./scada` era
commentata nel makefile della radice) e non si poteva rimettere in piedi con
una ricompilazione. Provato su una copia, il 15 ottobre 2026:

- le sue librerie di base (`scada/libOSF1`: Net, Util, Marte, InterPol, Xscada)
  davano 326 errori con GCC 14, 48 ammorbidendo le regole (`-fpermissive`);
- Net e Marte usano l'API dei thread **DCE** (`pthread_addr_t`,
  `pthread_startroutine_t`: 19 punti), lo strato che il progetto non costruisce
  più;
- `integr` (import/export dei punti, file `.ec`) usava il database
  **Informix**, e in `lib_informix` c'erano solo eseguibili e librerie per
  **Alpha** (Digital Unix): su Linux non c'era niente da collegare;
- le librerie si costruivano con `ccm_make` di Telelogic CM Synergy, che non
  esiste più;
- `agg_scada`, il ponte simulazione-SCADA, parte solo con `net_sked` e
  `dispatcher` compilati con `-DSCADA`, flag che il profilo ha disabilitato da
  tempo; il suo eseguibile era a 32 bit, inutilizzabile.

Rimetterlo in funzione voleva dire un porting vero (thread POSIX, un database
al posto di Informix, makefile nuovi) e dei dati su cui provarlo, che non
c'erano. Nessun uso attuale: deciso dall'utente il 15 ottobre 2026.

| Rimosso | Cos'era |
|---|---|
| `scada/` (845 file, 19 MB, più 70 `.o` non tracciati) | il sottosistema SCADA: acquisizione, archivi, allarmi, stampe, la sua interfaccia `Xscada`, le librerie per OSF1, `integr` con Informix, i documenti in `scada/docs/` |
| `Alg_rt/net_simula/agg_scada/` (5 file) e `Alg_rt/bin/agg_scada` | il ponte simulazione-SCADA |
| `Alg_mmi/client_scada/` (5 file) e `Alg_mmi/bin/client_scada` | il client MMI verso SCADA |
| `kdefaults/` (9 file: `scada/start_txt`) | i dati di avvio delle tabelle SCADA, che `kScadaInit` copiava nel simulatore |
| `kprocedure/kScadaInit.sh`, `kscadainit.sh` e le copie in `kbin/` | inizializzazione del database SCADA del simulatore (`PointInst`, `ExpRtf`, `ToRtf`, e il link `fnomi.rtf`) |
| `kprocedure/kScd.sh`, `kscd.sh` e le copie in `kbin/` | avvio del programma `scada` (che già prima non c'era in `scada/bin`) |
| `util97/procedure/creascada.sh`, `lanciascada.sh` e le copie in `util97/bin/` | creazione e lancio dello SCADA di un simulatore |
| le righe commentate in `Makefile.mk`, `Alg_mmi/Makefile.mk`, `Alg_rt/net_simula/Makefile.mk`; i target in `kprocedure/Makefile.mk` e `util97/procedure/Makefile.mk`; le righe `scada/` di `.gitignore` | — |
| in `Alg_env.sh`: `SCADA`, `SCADA_BIN` e la voce `$SCADA_BIN` nel `PATH`; in `.profile_legoroot`: `SCADA_DEBUG` | variabili che puntavano al sottosistema |

**Modificato di conseguenza.** `kMakeUser` creava `~/defaults` copiando
`kdefaults`, e se la directory esisteva già la cancellava per ricopiarla,
perdendo quello che l'utente ci aveva messo: ora la crea se manca e altrimenti
la lascia com'è.

**Lasciato apposta**, perché non è il prodotto SCADA o non dà fastidio:

- **l'"id SCADA" delle istanze MMI** (`kMmi.cfg`, `kMmi`, `kMmiConfig`,
  `kuser 77<id>`, `*hostNameS` nei `Context.ctx`): è il canale client/server
  dell'MMI (`server_mmi`, `demone_mmi`), non il sottosistema rimosso. Senza
  `kScadaInit` però non si genera più `fnomi.rtf`: in quella configurazione
  `mmi` lo segnala e resta senza albero gerarchico, come già documentato in
  [Alg_mmi/README.md](../Alg_mmi/README.md);
- **le directory del simulatore** `$KSIM/scada` (`KSCADA`, `KFILEOP`,
  `KSTART_TABLES`, `KEDIT_TABLES`, `KOPER_TABLES`, alias `kscada`), che
  `k_crea_simulatore`, `kExport`, `k_crea_cassaforte` e `kMakeRemoteSim`
  ancora creano o spostano;
- **il codice `#if defined SCADA`** di `net_sked` e `dispatcher` e l'opzione
  `-host_scada_name` degli script di avvio: senza il flag non viene compilato, e
  toglierlo vorrebbe dire toccare il motore della simulazione;
- i nomi `client_scada`, `scada`, `agg_scada` negli elenchi di processi da
  terminare di `kClean`, `kPulirSim` e `killsim`: non trovano niente, e non
  fanno danni;
- `Alg_help/` (l'aiuto storico) e i documenti in `docs/legacy`.

**Sistemati subito dopo** (fase 10 del piano): `kRun` lanciava `kScd` e
aspettava senza limite che SCADA risultasse avviato; `kStart` si rifiutava di
partire senza `kScd`, aspettava per sempre un messaggio che il banco non stampa
più, e alla fine lanciava `kLeeF22`; `kOk` segnava a mano `kScd` come avviato.
`kStart` è stato riscritto (avvia con `net_startup`, aspetta i processi con un
limite di tempo, esce con il motivo se non partono), `kRun` non lancia più
`kScd`, `kOk` non lo nomina più.

**Recupero.** Come per il resto: `git checkout d9b500a2 -- scada
Alg_rt/net_simula/agg_scada Alg_mmi/client_scada kdefaults ...`, poi le righe
nei makefile e in `Alg_env.sh`. Per farlo funzionare, però, resta tutto il
porting descritto sopra.

## Prima, ottobre 2026

| Rimosso | Commit di riferimento | Cos'era, e perché |
|---|---|---|
| `tcltktix-8.3.5b/` (311 file, 14 MB) | `0572018d^` (il commit prima di *via tcltktix-8.3.5b*) | Tcl/Tk 8.3 e Tix 8.1 compilati per i386 (a 32 bit), con i sorgenti. Non eseguibili sui sistemi di oggi: LegoPST usa il Tcl/Tk 8.6 di sistema. Con lui sono stati tolti l'alias `lgpcu`, la variabile `LG_WISH` e due righe di `PATH`/`LD_LIBRARY_PATH` verso una directory che non esisteva |
| `gdbm-install/` (13 file, RPM compresi) | `a81b0477^` | Installava in `/usr/lib64` la vecchia `libgdbm.so.2`, da un RPM di Fedora 12, per eseguibili precompilati di allora. Nessun eseguibile la usa più: si compila contro la gdbm di sistema. L'elenco dei componenti che usano gdbm sta ora in [Environment_setup.md](../Environment_setup.md) |
| `doc/`, `doc_devel/` | — (spostate, non rimosse) | I documenti storici sono in [docs/legacy/](legacy/), con la loro storia (`git mv`) |
