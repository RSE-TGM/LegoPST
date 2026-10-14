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
| 3. Classificazione dei cast | i ~600 cast puntatore/`int` | medio-grande | medio | da decidere |
| 4. Directory orfane | triage dei gruppi A, B, C | medio | basso se si archivia | da decidere |
| 5. Artefatti in git | `.o`, `.a`, eseguibili fuori dall'indice, `.gitignore` | piccolo da fare, grande da decidere | alto | da decidere |
| 6. Makefile comune | frammento condiviso, via i flag storici | grande | medio | da decidere |
| 7. Funzioni | §4, una per volta | variabile | variabile | da decidere |
| 8. CI: test di funzionamento | dopo il build, nello stesso container: simulazione senza grafica sulla demo (`net_startup_headless`) che avanza qualche secondo, `kUpSim` sulla demo, i test esistenti (`lg_fmu/tests`, round-trip di `lgmkstaz`). Vedi §6 | piccolo-medio | nullo (non tocca il prodotto) | **fatta** (2026-10-14), vedi §7 |

**Prima della fase 5**: il Dockerfile non compila, copia gli eseguibili
dell'albero di lavoro, quindi l'immagine non dipende da git. Ma le macchine
native che fanno `git pull` per avere gli eseguibili senza compilare sì; e le
`legocad/lego_big/lib/*.a`, che non sono in git, vanno comunque prodotte dal
build. Va chiarito chi si affida agli eseguibili presi da git.

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
