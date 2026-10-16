# Piano di intervento sulla codebase

Revisione del 2026-10-14: coerenza della codebase, parti non compilate,
possibili bug, sviluppi. Il piano è a fasi indipendenti: quali e quando
mettere in produzione è una decisione a parte, fase per fase.

Il codice del **prodotto Windows** che sta in `Alg_legopc` (`.bat`,
`compall*`, `lgsincroaccshm`…) è fuori da questa revisione: è un altro
prodotto, divergente da quello Linux, e non c'è l'intenzione di unificarli.

## Come è stata fatta

- build dalla radice (`make -k -f Makefile.mk`): incrementale sull'albero di
  lavoro, e **da zero** su una copia senza `.git` e senza `.o` (943 sorgenti
  ricompilati);
- raggiungibilità dei makefile, seguendo i `cd <dir>; $(MAKE)` dalla radice e
  risolvendo i link simbolici (`Alg_rt/AlgLib`, `legocad/lego_big`);
- classificazione dei warning della compilazione da zero;
- per il bug più serio, verifica sui **binari** (`objdump`), non solo sul
  sorgente.

## Stato in sintesi

La base è coerente e si compila **senza errori**. I punti deboli: 2.458
warning, con una classe di bug latenti nella libreria grafica dell'MMI; 44
makefile che il build non raggiunge; artefatti di compilazione tracciati in
git.

## 1. Coerenza

| Cosa | Stato |
|---|---|
| Build dalla radice | ✅ 0 errori, incrementale e da zero; raggiunge 118 makefile su 163 |
| Link locali di `DOCUMENTATION_INDEX.html` | ✅ tutti validi |
| Artefatti in git | ❌ 821 `.o`, 26 `.a`, 28 `.so`, 129 eseguibili in `bin/`/`kbin/`; pacchetto git di 329 MB. Ogni build modifica file tracciati |
| Flag di compilazione storici | ⚠️ `-Dmmap=_mmap_32_` in 151 makefile (innocuo: nessuno usa `mmap`); `-I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include` in 22; un `-I-I.`; `-fpermissive` in 6, che nasconde errori veri |
| Nome dei makefile | ⚠️ `Makefile.mk` ovunque tranne `Alg_legopc/src/*`, che usa `makefile` |
| Albero delle directory nel README | ⚠️ mancavano `Alg_help/`, `kdefaults/`, `scada/` |
| Codice morto tracciato | ⚠️ `AlgLib/dcethreads_lib_NO` (711 file, 15 MB), `Alg_legopc/src/tix_old` + `bin_old` (231 file, vecchia UI Linux fuori build da agosto 2026), `kutil/LeeF22.c`, `kutil/kReadDB.c`, `kbin/kReadDB` (eseguibile Alpha) |
| CI e test | ⚠️ nessun build automatico; test solo in `Alg_rt/lg_fmu/tests` e `lgmkstaz_test.tcl` |

## 2. Parti non compilate

| Gruppo | Directory | Stato |
|---|---|---|
| A. Tool di simulazione rimasti fuori | `Alg_rt/net_simula/{agg_scada, dump_shr, monitor, net_tast, agg_manovra}`, `Alg_rt/snaf14` | Non compilano con GCC 14/15 (`implicit-int`, funzioni non dichiarate, firme cambiate: `sna2f14` chiama `ControlParam` con gli argomenti vecchi). In `bin/` restano eseguibili di aprile 2025 |
| B. Compilabili ma non agganciati | `Alg_mmi/client_scada`, `legocad/menugraf`, `Alg_mmi/resource/VARIABILI`, `Alg_mmi/starter`, `legocad/legoserver` (tranne `creamodi`) | `make -n` pulito su `client_scada` e `menugraf`; da verificare gli altri |
| C. `scada/` | 37 makefile | Commentato nel makefile della radice: sottosistema intero, con librerie per OSF1 |

Approccio: per ogni directory, prima il **triage** (a cosa serve, chi la
chiama, c'è un sostituto: `monitor` → `net_monit`/`new_monit`? `dump_shr` utile
per il debug della SHM?), poi uno di tre esiti: **rimuovere**, **recuperare**
(correggere fino a compilare con i flag degli altri e aggiungere al build),
**archiviare** come `docs/legacy`. `scada/` è una decisione a sé: serve ancora
l'integrazione SCADA?

## 3. Possibili bug

### Gravità alta

1. **Puntatori troncati a 32 bit nella libreria grafica dell'MMI**
   (`Alg_mmi/lib/Xl`). Una quarantina di chiamate a funzioni che restituiscono
   **puntatori** avviene senza prototipo: `XmTextGetString`, `XmCreateText`,
   `XmCreatePushButton`, `XmCreateLabel`, `XmCreateToggleButton`, `XlGetFont`
   (15 chiamate), `UxNewContext`, `create_xlcurve`, `create_legend`,
   `create_top_allarmi`, `OlGetNameVar`… in `XlSetValore.c`, `XlChangePage.c`,
   `XlCurve*.c`, `XlAllarmi.c`. Verificato nei binari `mmi`, `config`, `draw`:
   dopo ogni chiamata il compilatore estende il segno di un `int` (`cltq`).
   Oggi funziona per caso: eseguibili non-PIE (il default del gcc di Fedora) e
   piccole allocazioni in basso nella memoria. Si rompe compilando su
   Ubuntu/Debian (eseguibili PIE: heap sopra i 4 GB), o con allocazioni grandi
   o da thread (via `mmap`, in indirizzi alti).
2. **Ritorno dell'indirizzo di una variabile locale** (10 casi):
   `new_monit/file_select.c` (4), `malfunzioni.c` (2), `messaggi.c` (3),
   `AlgLib/libnet/io_sono.c`. Comportamento indefinito.

### Gravità media

3. Scritture oltre i limiti: `libsim/letture_dati.c:249`,
   `net_monit/util_monit/monit.c:2351`, `new_monit/preview.c:350`,
   `legocad/edicole/edicole.c:579`; lettura oltre i limiti:
   `mandb/mandb.c:1587`.
4. `free` su memoria non allocata (`legocad/filtri/applicationShell1.c:540`);
   un `char` che riceve `12345` (`net_sked/sked_s02.c:233`).
5. Script con `] & [` invece di `] && [`: `kMmi`, `kMmiConfig`, `kClean`,
   `kCleanLoc`. Il primo test va in background: decide solo l'utente.
6. `lgdock_socat.sh`: riga di debug `chown -R … /home/antonio`, cablata.

### Gravità bassa

7. `kStart` → `kLeeF22` → `LeeF22` inesistente; `gets()` in
   `Alg_mmi/utilities/modallpag.c`.
8. Percorsi cablati: `-I/usr/users/` in `new_monit/Makefile.mk`,
   `/usr/users/legoroot/...` in `legocad/filtri/dialogShell2.c`, esempi
   `/home/antonio` nell'aiuto di `graphics`, `kdxstart.sh`.
9. Da classificare: 261 cast puntatore→`int` e 342 `int`→puntatore. Molti sono
   il passaggio di un intero come `client_data` di Xt, innocuo; altri potrebbero
   essere troncamenti come il punto 1.

### Lavori recenti non ancora verificati sul campo

Podman reale (filtro dei warning, `--pull=never`); Command Mode di draw2gr con
`net_sked` vivo; elementi operatore (`@stz_0`, `@set_0`) con una simulazione
vera; `bundle/build.sh` con draw2gr; bundle FMU già costruiti da ricostruire.

## 4. Sviluppi e miglioramenti

**Manutenibilità**: CI di build nell'immagine Docker con conteggio dei warning
(il numero non deve crescere), poi `-Werror=implicit-function-declaration` una
libreria alla volta; un frammento di makefile comune al posto dei 150 blocchi
copiati; test di regressione con i pezzi che già esistono
(`net_startup_headless` e `kUpSim` sulla demo, `probe_attach` FMU, round-trip
`lgmkstaz`); pulizia del codice morto.

**Funzioni**: draw2gr nel bundle FMU (piano in 4 fasi) e fase 3 del Command
Mode in FMU; `lgmkstaz`: colori e scale MINMAX; `f01totom` fase 2; configuratore
dei simulatori (opzione A con Access); messaggi di `lgdemo` in inglese; se
serve, un esportatore in testo di `f22circ.dat` per il formato attuale al posto
di `LeeF22`.

## 5. Fasi

| Fase | Contenuto | Impegno | Rischio | Stato |
|---|---|---|---|---|
| 0. Correzioni puntuali | bug 2–6, `gets()`, README | piccolo | basso | **fatta** (2026-10-14), vedi §7 |
| 1. Prototipi nell'MMI | bug 1; poi `-Werror=implicit-function-declaration` su `Alg_mmi/lib/Xl`; prova di `mmi`, `config`, `draw` | medio | basso nel codice, va provato con pagine vere | **fatta** (2026-10-14), vedi §7; provata dall'utente con la simulazione viva |
| 2. CI di build | workflow nell'immagine, base dei warning di oggi | piccolo | nullo | **fatta** (2026-10-14), vedi §7; verde su GitHub dal commit `1367c68e` |
| 3. Classificazione dei cast | i cast puntatore/`int` | medio-grande | medio | **fatta** (2026-10-16): 5 difetti corretti, 4 sospetti chiusi, eseguibili ricollegati, soglia warning 2212 → 2183, vedi §7 |
| 4. Directory orfane | triage dei gruppi A, B, C | medio | basso se si archivia | **fatta** (2026-10-15), SCADA compreso: vedi §7 e [COMPONENTI_RIMOSSI.md](COMPONENTI_RIMOSSI.md) |
| 5. Artefatti in git | ~~togliere `.o`, `.a`, eseguibili dall'indice~~ | — | — | **non si fa** (decisione dell'utente, 2026-10-15): tutti prendono gli eseguibili già compilati da git, il prodotto deve funzionare senza essere compilato. **Attenzione (utente, 2026-10-15): anche alcuni `.o` vanno tenuti**, perché servono a compilare gli eseguibili che il motore LEGO produce a runtime (le task). **Indagine fatta (2026-10-16)**: i `.o` indispensabili sono 9 (`main_lg2/lg3/lg5/cf04/lg5sk/rg5sk.o`, `rg5sim.o`, `pslego3.o`, `trapfpe.o`), più 10 `.a`; gli altri ~810 sono intermedi del build. Elenco e motivi in [BUILD.md, *Compiled files in git*](BUILD.md#compiled-files-in-git); la prova `task` della CI ricostruisce le task della demo e fallisce se uno di quei file non è in git |
| 6. Makefile comune | frammento condiviso, via i flag storici | grande | medio | da decidere |
| 7. Funzioni | §4, una per volta | variabile | variabile | da decidere |
| 9. Snapshot e topologia | `net_sked` carica uno snapshot (e un backtrack) senza controllare che sia stato salvato con la topologia attuale: dopo una modifica del modello legge uno stato sfalsato senza avvisare. `ConfrontoPar` confronta i parametri del `Simulator` ma non `size_area_dati`, che pure sta nell'intestazione del file. Correzione: confrontarlo in `ControlParam` e mettere da parte il file incompatibile, come già per `f22circ.dat` | piccolo | basso | **fatta** (2026-10-15), vedi §7 |
| 10. Catena di avvio "k" | `kRun`, `kStart`, `kOk` (e `kLeeF22`, lanciato da `kStart`) dipendono da `kScd`, rimosso con SCADA: già prima inservibili. Rimuoverli, o riscrivere `kStart` sopra `net_startup` | piccolo | basso | **fatta** (2026-10-15): `kStart` riscritto, vedi §7 |
| 11. `parametri.out` che non si azzera | Sembrava che su SLaurent_0 `f22circ.dat` nascesse con 7200/1000 contro i 14400/10000 del `Simulator`. Non era così: `parametri.out` si scriveva sempre in coda, e quello della demo conteneva 131 avvii della macchina di chi l'ha confezionata; le righe SEVERE erano vecchie. Corretto: il dispatcher lo ricomincia a ogni avvio (il precedente in `.prec`), e le demo non lo includono | piccolo | basso | **fatta** (2026-10-15), vedi §7 |
| 8. CI: test di funzionamento | dopo il build, nello stesso container: simulazione senza grafica sulla demo (`net_startup_headless`) che avanza qualche secondo, `kUpSim` sulla demo, i test esistenti (`lg_fmu/tests`, round-trip di `lgmkstaz`). Vedi §6 | piccolo-medio | nullo (non tocca il prodotto) | **fatta** (2026-10-14), vedi §7 |

**Prima della fase 5**: il Dockerfile non compila, copia gli eseguibili
dell'albero di lavoro, quindi l'immagine non dipende da git. Ma le macchine
native che fanno `git pull` per avere gli eseguibili senza compilare sì; e le
`legocad/lego_big/lib/*.a`, che non sono in git, vanno comunque prodotte dal
build. Va chiarito chi si affida agli eseguibili presi da git.

## 2bis. Fase 4: scheda di triage (2026-10-15)

Per ogni directory fuori dal build: cosa è, chi la usa, quanti errori dà oggi
compilandola (su una copia, con i flag del suo makefile), e la proposta.
"Rimuovere" vuol dire togliere da `master`: resta tutto nella storia di git, e
prima si mette un tag per ritrovarlo in un comando.

| # | Directory | Cos'è | Chi la usa | Compila? | Proposta |
|---|---|---|---|---|---|
| 1 | `net_simula/agg_manovra` | collegamento al banco manovra fisico (`agg_manovra`, `_stato`, `_pert`, `bm`) | `net_sked` lo lancerebbe solo se `S01` dichiara un host di banco manovra | **no: c'è solo il makefile**, i sorgenti non esistono più (nemmeno nella storia di git); in `bin/` nessun eseguibile | **rimuovere** |
| 2 | `Alg_mmi/starter` | avvio della versione stand-alone di SCADA/MMI | nessuno | **no: solo i makefile**, `starter.c` non c'è | **rimuovere** |
| 3 | `net_simula/monitor` | vecchio monitor della simulazione (Motif, generato con AIC) | nessuno: sostituito da `net_monit`/`new_monit` | 9 errori | **rimuovere** |
| 4 | `legocad/menugraf` | vecchio menu dei grafici di legocad | nessuno: sostituito da `graphics`/`grafics` | 143 errori | **rimuovere** |
| 5 | `net_simula/net_tast` | comandi alla simulazione da tastiera (start, freeze, stop, backtrack) | solo `tsimula.sh`, script di prova | 33 errori; in `bin/` un eseguibile del 2025 | **rimuovere**, con `tsimula` e l'eseguibile: lo stesso lo fanno il banco e `probe_step` |
| 6 | `net_simula/dump_shr` | dump del database della simulazione in SHM | nessuno (è uno strumento di debug) | 3 errori | **recuperare**: costa poco, e per i problemi di SHM serve (`probe_attach` legge una variabile alla volta) |
| 7 | `Alg_rt/snaf14` | `sna2f14`: da uno snapshot a un `f14.dat` (condizioni iniziali) | il comando `snaf14` (installato in `Alg_rt/bin`) | 1 errore: `ControlParam` ha cambiato firma | **recuperare**, se `snaf14` ti serve; altrimenti rimuovere insieme al comando |
| 8 | `net_simula/agg_scada`, `Alg_mmi/client_scada`, `scada/` | l'integrazione SCADA: ponte simulazione-SCADA, client MMI dello SCADA, il sottosistema intero (845 file, 19 MB, librerie OSF1) | `agg_scada` lo lancerebbe `net_sked` solo se compilato con `-DSCADA`, che non c'è; `Alg_env.sh` esporta `SCADA`, `KSCADA` e l'alias `kscada` | `agg_scada` 49 errori, eseguibile a 32 bit inutilizzabile; `client_scada` compila; `scada/` fuori dal build | **una decisione sola**: se SCADA non serve più, rimuovere tutto insieme e pulire i riferimenti nel profilo |
| — | `legocad/legoserver` | il server dei moduli di una libreria utente | `cad_crealibut.sh` lo compila **a richiesta**, quando si crea una libreria di moduli | non si compila col build apposta | **non è orfana**: lasciare |
| — | `Alg_mmi/resource/VARIABILI` | l'editor delle variabili dell'MMI | `config` e `mmi`: i loro makefile compilano questi sorgenti | — | **non è orfana**: lasciare |

## 6. Cosa si può costruire sulla CI

La CI della fase 2 verifica che il codice **compili**, da zero e in un
ambiente pulito, e che i warning non crescano. Non verifica che **funzioni**,
non tocca l'immagine Docker, non copre il codice fuori dal build. Gli sviluppi
possibili, in ordine di utilità:

1. **Test di funzionamento** (fase 8): trasformano "compila" in "funziona". I
   pezzi esistono quasi tutti: `net_startup_headless.sh` (simulazione senza
   grafica, gia' usato dalle FMU), la demo `userstd`, `kUpSim`, i test di
   `lg_fmu` e di `lgmkstaz`.
2. **Protezione del ramo `master`**: su GitHub, unire a `master` solo con il
   build verde, lavorando con le pull request. Serve se sul repository lavora
   anche qualcun altro.
3. **Immagine Docker costruita dalla CI** a ogni rilascio, da sorgenti
   verificati, invece che dalla macchina di sviluppo con i suoi binari. È la
   strada che rende fattibile la fase 5: gli eseguibili li produce la CI, non
   il repository.
4. **Soglia dei warning che scende**: a ogni pulizia si aggiorna
   `.github/build_warnings_baseline`.

## 7. Fatto

### Fase 0 (2026-10-14)

- **Ritorno di indirizzi locali** (bug 2): buffer `static` in `io_sono`,
  `fileSelectionCb`, `creaTestoMalfList`, `sintesiDescr`, `getMessageString`,
  `getVar`, `getVarMessage`; tutti i chiamanti copiano subito il risultato.
  Nelle stesse funzioni di `new_monit/messaggi.c`: `getVar` andava in ciclo
  infinito su uno spazio nel nome (`continue` senza `i++`); `getVar` e
  `getVarMessage` ritornano `""` e non `NULL` su una riga malformata (i
  chiamanti facevano `strcpy` del risultato); copie limitate alla dimensione
  del buffer.
- **Scritture oltre i limiti** (bug 3): `letture_dati.c` (comando `cp` di due
  path in 120 byte: ora `2*FILENAME_MAX+8` e `snprintf`), `monit.c`
  (`app_str[10]` con `"Automatico"`), `edicole.c` (`nome_modulo[5]` con
  `"STDNP"`, piu' `strncpy` dell'opzione). `mandb.c`: si leggeva un byte oltre
  una costante. Falsi allarmi tolti comunque: lo `strncat` di `MOM`, il ciclo
  morto di `preview.c`.
- **Memoria** (bug 4): in `legocad/filtri/applicationShell1.c` si liberavano
  un array sullo stack (`free(par)`) e la struttura del callback di Motif
  (`free(callb)`): via tutti e due. `sked_s02.c`: `sorg = -1` come negli altri
  punti (era `12345`, che nel `char` diventa 57).
- **`] & [`** (bug 5): non e' diventato `&&`. L'host di `kMmi.cfg` e' quello di
  chi l'ha scritto, e nel container o in una demo non coinciderebbe mai: i
  quattro script ora confrontano **solo l'utente**, che e' quello che gia'
  succedeva per sbaglio. Documentato in `Alg_mmi/README.md`.
- `lgdock_socat.sh`: tolte le righe di debug (`chown -R … /home/antonio`).
  `modallpag.c`: `fgets` al posto di `gets`. README: albero delle directory.

### Fase 1 (2026-10-14)

- `Alg_mmi/lib/Xl/XlProto.h`: le funzioni che i sorgenti di Xl chiamavano senza
  dichiarazione, con il tipo di ritorno vero (puntatori, `Boolean`, `Pixel`).
  Incluso dai 43 sorgenti che ne avevano bisogno, non da `XlP.h`. Header Motif
  mancanti (`Xm/Text.h`, `Xm/ToggleB.h`, `Xm/PushB.h`, `Xm/Label.h`,
  `Xm/RowColumn.h`), `ctype.h`, `unistd.h`. In `draw`: `UxNewContext`,
  `PrintSetup` e le altre.
- Dichiarazioni implicite: da 196 a **0**. Il makefile di Xl ora ha
  `-Werror=implicit-function-declaration`.
- **Verificato nei binari**: dopo le chiamate a rischio non resta nessun
  troncamento (`cltq`) in `mmi`, `config`, `draw` (le 4 estensioni rimaste
  riguardano l'indice di un array). Ricollegati gli eseguibili che contengono
  `libXl`, `libsim`, `libnet`.
- **Prova**: `config`, `draw` e `mmi` partono su un display virtuale con le
  pagine vere di SLaurent_0, senza crash. Provati poi dall'utente con la
  simulazione viva: nessun problema.

### Fase 2 (2026-10-14)

- `.github/workflows/build.yml` + `.github/scripts/build_check.sh`: build da
  zero nell'immagine `aguagliardi/legopst:2.0`, a ogni push su `master` e
  pull request, con il log come artefatto. Fallisce su errori o se i warning
  superano `.github/build_warnings_baseline`.
- Provato in locale nel container: 49 s, 0 errori, **2212 warning** (erano
  2458), che e' la soglia. Con soglia 2211 il controllo fallisce (uscita 2),
  con 2212 passa.
- Prima run su GitHub (commit `d62718a4`) fallita: il checkout da' ai file
  date in ordine casuale, e prodotti tracciati in git risultavano piu' vecchi
  dei sorgenti. `make` rifaceva `regopatch.jar` (senza `javac` nell'immagine)
  e, a caso, `f01totom` e `inhoud` (costruiti direttamente dal `.c`, con
  `-Wall`: 19 warning in piu'). Corretto in `build_check.sh` dando a tutti i
  file la stessa data prima di cancellare i `.o`; i primi errori diventano
  annotazioni della run e c'e' un riepilogo nella pagina. Verde su GitHub dal
  commit `1367c68e`.

### Fase 8 (2026-10-14)

- `.github/scripts/smoke_test.sh`, nel workflow dopo il build e nello stesso
  container: `lgdemo` installa la demo `nuclear` (l'unica pubblicata con le
  `proc/`, quindi simulabile senza legopc), poi round-trip di `lgmkstaz`,
  `kUpSim -nommi` su SLaurent_0, simulazione senza grafica su PWRN1PSS con
  `net_startup_headless.sh` + `probe_init` + 10 passi di `probe_step --goup`
  (il tempo deve avanzare di 10 s), `test_var_mapping`. HOME provvisoria;
  esiti come annotazioni e riepilogo nella pagina della run; log come
  artefatto.
- Provato in locale nel container su una copia pulita di HEAD: build e prove
  in 75 s, 5 su 5.
- **Prova in negativo** (con `net_sked` non eseguibile): la prima versione
  restava appesa, perche' `probe_init` aspetta la risposta per sempre e
  `net_startup_headless.sh` esce con 0 appena ha lanciato i processi in
  background. Ora ogni passo ha un `timeout`, lo script controlla che
  `dispatcher` e `net_sked` siano vivi prima di mandare comandi, e il job ha
  un tetto di 45 minuti: la stessa rottura da' "FAIL" in 22 s.
- Da sapere: le prove dipendono dalla release delle demo (rete, e la demo
  `nuclear` deve restare pubblicata con le `proc/`); il token della run evita
  il limite di 60 interrogazioni l'ora dell'API di GitHub.

### Fase 4 (2026-10-15)

- Scheda di triage in §2bis; decisioni dell'utente: rimuovere 1-5,
  recuperare `dump_shr` e `snaf14`, SCADA a parte.
- Rimossi `agg_manovra` (solo makefile), `Alg_mmi/starter` (solo makefile),
  `monitor`, `legocad/menugraf`, `net_tast` con `tsimula`. Tutto documentato,
  con le istruzioni per recuperarlo, in
  [COMPONENTI_RIMOSSI.md](COMPONENTI_RIMOSSI.md) (insieme alle rimozioni
  precedenti: `tcltktix-8.3.5b`, `gdbm-install`).
- La ricerca dei riferimenti non aveva visto che `net_monit` prendeva
  `UxXt.h` da `monitor/` (un `-I` nel makefile): l'ha trovato la CI, in
  locale, prima del commit. Il file ora sta in `net_monit/`.
- Recuperati e rientrati nel build: `dump_shr` (provato su una simulazione
  viva) e `snaf14`. In `snaf14` corretti anche due bug di selezione dello
  snapshot, e aggiunto il rifiuto di uno snapshot salvato con un'altra
  topologia: sulla demo `nuclear` lo snapshot di settembre, con 56 variabili
  in più, produceva un `f14.s` di valori senza senso annunciando successo.
  Lo stesso difetto riguarda `net_sked`: nuova fase 9.
- CI in locale dopo le modifiche: 0 errori, 2212 warning (invariati), 5 prove
  su 5.
- SCADA rimosso il 2026-10-15 su decisione dell'utente, dopo l'analisi (non
  si ricompila: thread DCE, Informix solo per Alpha, `ccm_make`). Rimossi anche
  `kScadaInit`, `kScd`, `creascada`, `lanciascada`, `kdefaults`; `kMakeUser`
  ora non cancella più `~/defaults`. Lasciati l'"id SCADA" dell'MMI e le
  directory `$KSIM/scada`. Dettagli in COMPONENTI_RIMOSSI.md; nuova fase 10
  (catena di avvio "k").

### Fase 9 (2026-10-15)

- `ControlParam` (`AlgLib/libsim/simulator.c`): `snapshot.dat` e
  `backtrack.dat` salvati con un'altra topologia - confronto della dimensione
  dell'area dati e, task per task, di stati, ingressi, derivate e dati con
  quelli di `S02_LINUX` - vengono rinominati in `.incompatibile` dal primo
  processo che se ne accorge, con il motivo in `parametri.out`; la simulazione
  parte con un file nuovo. Per i parametri diversi l'errore resta com'era.
- Provato sulla demo `nuclear`: lo snapshot di settembre (14968 byte contro
  14744) messo da parte al primo avvio, simulazione partita, niente al
  secondo. Nuova prova `topologia` nella CI (intestazione alterata apposta).
- Ricollegati dispatcher, net_sked, cad_sked, net_monit, cad_monit, banco,
  net_prepf22, sna2f14.

### Fase 10 (2026-10-15)

- `kStart` riscritto: avvia con `net_startup` (come prima), poi aspetta che
  `dispatcher`, `net_sked` e `banco` siano vivi e restino vivi per 5 s, con un
  limite di tempo (`KSTART_TIMEOUT`, 120 s); esce con 1 e il motivo se
  `net_startup` segnala errori di startup, se un processo muore o se scade il
  tempo. Tolti il controllo su `kScd`, l'attesa senza limite di un messaggio
  che il banco non stampa più, e `kLeeF22`. L'output va in
  `$KLOG/kStart.log`. `kRun` non lancia più `kScd` e si ferma se `kStart`
  fallisce; `kOk` non nomina più `kScd`.
- Provato nel container (con Xvfb installato al volo: nell'immagine non
  c'è): PWRN1PSS avviata in 8 s, SLaurent_0 (3 task) in 9 s; un secondo
  `kStart` risponde "already started"; senza display esce con NOK in 16 s
  invece di restare appeso. Non in CI: servirebbe Xvfb nell'immagine.
- Notato durante le prove: fase 11 (che poi si è rivelata un'altra cosa).

### Fase 11 (2026-10-15)

- L'indagine ha smentito l'ipotesi: su SLaurent_0 i processi partono con i
  parametri del `Simulator` (14400/10000) e `f22circ.dat` nasce con quelli. Le
  righe SEVERE con 7200/1000 venivano da `parametri.out`, che si scriveva
  sempre in coda: nella demo pubblicata ne era arrivato uno di 3103 righe, 131
  avvii fatti sulla macchina di chi l'ha confezionata.
- `ControlParam`: il dispatcher, primo processo a partire, mette da parte il
  `parametri.out` dell'avvio precedente come `parametri.out.prec` e ne apre uno
  nuovo che comincia con "Avvio del <data>". `make_demo_tgz.sh` esclude
  `parametri.out` e `.prec`.
- Verificato nel container con `net_startup` (Xvfb installato al volo): un
  `f22circ.dat` con l'intestazione alterata (999 campioni) viene segnalato,
  rinominato in `f22circ.dat.incompatibile` e ricreato con i parametri del
  `Simulator`; la simulazione parte. Con `net_startup_headless.sh` quel
  controllo non scatta, perché lo script cancella sempre `f22circ.dat` prima
  di avviare.
- **Da fare alla prossima pubblicazione delle demo**: rifarle con
  `make_demo_tgz.sh` (oggi `nuclear` contiene ancora il `parametri.out` lungo).

### Fase 3 (fatta, 2026-10-16)

Dal build da zero: **417** punti distinti (240 `int-to-pointer-cast`, 177
`pointer-to-int-cast`). Classificati leggendo la riga di sorgente:

- **343 innocui**: 182 interi passati a Xt come `client_data`/risorsa
  (`(XtPointer)*is-1`, `set_something(...,(char *)width)`) e 130 riletti
  come intero nel callback (`int is=(int)info`): il valore va e torna intero.
  29 `(char)NULL` (vale zero), 2 confronti `(int)ptr == -1` dopo `shmat`.
- **74 guardati a mano**; quasi tutti lo stesso idioma su più righe. Difetti
  **veri**, da correggere:
  1. `Alg_rt/net_simula/net_disp/dispatcher.c:940`: `memcpy((void*)app_varupd[k*MAX_LUN_COMANDI_DISPATCHER], ...)`
     usa come destinazione il VALORE di un carattere, non l'indirizzo
     (deve essere `app_varupd + k*MAX_LUN_COMANDI_DISPATCHER`, come alla riga
     961). Un aggiornamento di variabili (VARUPD) di più blocchi scrive in un
     indirizzo a caso.
  2. `AlgLib/libsim/var_sh.c` (`numero_blocco`, righe 294-299):
     `init = (int) ret; punt = (char*)(inizio + init)`: indirizzo troncato a
     32 bit. Usare un `char *` base.
  3. `Alg_mmi/lib/Ol/OlForm.c:1026`: `eofile=(int)fgets(...)`: usare
     `fgets(...) != NULL`.
  4. `Alg_mmi/run_time/topLevelShellMain.c:1996`: `(int)(fpLog=fopen(...))<0`
     non rileva mai il fallimento: `== NULL`.
  5. `Alg_rt/grafica/xstaz/xstaz.c:374,381`: `(int)(t_call[i].closure)>1000`
     distingue un codice da un puntatore troncando: confrontare come
     `uintptr_t`.
- **Da guardare ancora**: `Alg_mmi/config/PagShell.c:750` e
  `Alg_mmi/draw/applicationShell1.c:389` (`return((int)ret)`, ret
  puntatore?); `net_sked/sked_start.c:1068,1103`
  (`signal(SIGKILL,(__sighandler_t)pid)`: insensato, probabilmente voleva
  `kill`); `net_monit/util_monit/monit_variabili.c:897,937,989,1029` (non
  risolti dallo script).
- **Non è un difetto su Linux**: `resource/UtilRes.c` mette il pid restituito
  da `ActiveProgWithParameter` in un `PROG_DATA *`; lo si dereferenzia solo in
  `AnimateIconSelection.c` (`uccidi_processo`), che è sotto `#ifdef VMS`.
- **Fatto (2026-10-16)**: corretti i 5 difetti, come indicato sopra. In `OlForm.c`
  la lettura a fine file ora restituisce `EOF` invece di elaborare un buffer
  vecchio. I 4 sospetti:
  - `gs_draw_ris` (PagShell.c, applicationShell1.c): il valore restituito non
    lo usa nessuno; ora è `ret != NULL`.
  - `sked_start.c`: `signal(SIGKILL, pid)` non uccideva niente; ora è
    `kill(pid, SIGKILL)` se il pid è positivo. **Cambia il comportamento**:
    se `net_prepf22` o `agg_manovra` non risponde entro il timeout di avvio,
    adesso viene terminato. Prima restava vivo, ma `net_sked` lo dava già per
    perso (`pid = -1`).
  - `monit_variabili.c`: `!= (int)NULL` diventa `!= NULL`. Era già corretto,
    perché `Utstrstr` ha il prototipo.

  Ricollegati tutti gli eseguibili e le `.so` tracciati che contenevano il
  codice vecchio di `libsim`/`libOl`. Sono stati trovati con `nm`; i makefile
  non ricollegavano da soli `viewval`, `viewval_legacy`, `DataServer`,
  `LegoCliSINC.so` e gli strumenti di `lg_fmu/tools` e `tests`. CI locale:
  0 errori, 7 prove su 7, warning da 2212 a 2183 (nuova soglia).
