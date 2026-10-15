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

- **SCADA** (`scada/`, `Alg_rt/net_simula/agg_scada/`, `Alg_mmi/client_scada/`):
  da decidere a parte, vedi il piano.
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

## Prima, ottobre 2026

| Rimosso | Commit di riferimento | Cos'era, e perché |
|---|---|---|
| `tcltktix-8.3.5b/` (311 file, 14 MB) | `0572018d^` (il commit prima di *via tcltktix-8.3.5b*) | Tcl/Tk 8.3 e Tix 8.1 compilati per i386 (a 32 bit), con i sorgenti. Non eseguibili sui sistemi di oggi: LegoPST usa il Tcl/Tk 8.6 di sistema. Con lui sono stati tolti l'alias `lgpcu`, la variabile `LG_WISH` e due righe di `PATH`/`LD_LIBRARY_PATH` verso una directory che non esisteva |
| `gdbm-install/` (13 file, RPM compresi) | `a81b0477^` | Installava in `/usr/lib64` la vecchia `libgdbm.so.2`, da un RPM di Fedora 12, per eseguibili precompilati di allora. Nessun eseguibile la usa più: si compila contro la gdbm di sistema. L'elenco dei componenti che usano gdbm sta ora in [Environment_setup.md](../Environment_setup.md) |
| `doc/`, `doc_devel/` | — (spostate, non rimosse) | I documenti storici sono in [docs/legacy/](legacy/), con la loro storia (`git mv`) |
