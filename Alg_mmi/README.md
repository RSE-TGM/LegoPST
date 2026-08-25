# Alg_mmi — l'interfaccia uomo-macchina LegoPST

`Alg_mmi` è l'HMI "SCADA-like" di LegoPST: pagine sinottiche animate, disegnate con
un editor grafico e visualizzate a run-time collegandosi ai punti del simulatore.
È un mondo separato da `legopc` (il CAD di modellazione) e da `xstaz`/`draw2gr`
(i faceplate e le pagine di processo di `Alg_rt`): usa file, directory e
convenzioni tutte sue.

| eseguibile | sorgente | ruolo |
|---|---|---|
| `mmi` | [run_time/](run_time/) | il run-time: mostra le pagine e le anima |
| `config` | [config/](config/) | l'editor/configuratore delle pagine (pagedit) |
| `compreg` | [config/](config/) | compilatore delle pagine di regolazione |
| `conv_l`, `conv_r` | [conv_legograf/](conv_legograf/), [conv_regograf/](conv_regograf/) | conversione da schemi LEGO/regolazione a pagine |
| `convstaz` | [conv_staz/](conv_staz/) | conversione stazioni di comando |
| `demone_mmi`, `client_mmi`, `server_mmi` | [client_server/](client_server/) | collegamento MMI ↔ simulatore |

Gli eseguibili finiscono in [bin/](bin/) (`$LEGOMMI_BIN`).

---

## 1. Il file Context: l'unico punto di verità

Tutto ciò che l'MMI deve trovare su disco è dichiarato in un **file Context**
(`Context.ctx`), che è un normale **file di risorse X** letto con
`XrmGetFileDatabase`. Non esiste nessun altro meccanismo di ricerca: niente
variabili di ambiente da impostare a mano, niente lista di directory da provare
in sequenza, nessun default nel codice.

Le risorse che descrivono le directory ([include/pagresdef.h](include/pagresdef.h)):

| risorsa | significato |
|---|---|
| `*pages` | directory delle pagine (`.pag`, `.rtf`, `.bkg`) |
| `*simulator` | directory del simulatore, dove sta `variabili.rtf` (il db dei punti) |
| `*objectLibraries` | librerie di oggetti grafici |
| `*animatedIconLibraries` | librerie di icone animate |
| `*description` | descrizione mostrata dall'MMI |
| `*hostName`, `*hostNameS` | host del server e dello SCADA |
| `*numDisplay`, `*displayList` | display su cui l'MMI può aprire pagine |
| `*pag_num`, `*page_list` | **quante** e **quali** pagine esistono |
| `*iconlib_num`, `*iconlib_list`, `*iconlib_label` | librerie di icone (solo per l'editor) |

Più, per ogni pagina, un blocco di attributi:

```
*ACSP.top_tipo:         Sinottico
*ACSP.top_descrizione:
*ACSP.refresh_freq:     10
*ACSP.gerarchia:        -1,-1,-1,-1,-1,-1
*ACSP.tagPag:
*ACSP.schemeInUse:      0
```

Esempio reale (`$KSIM/globpages/Context.ctx` di SLaurent_0):

```
*description:            SLaurent_0 - PWR 900 MWe Saint Laurent B1
*simulator:              /home/antonio/sked/SLaurent_0
*pages:                  /home/antonio/sked/SLaurent_0/globpages
*objectLibraries:        /home/antonio/legocad/libut_reg/libreg
*animatedIconLibraries:  /home/antonio/legocad/libut_reg/libreg
*pag_num:                20
*page_list:              \ ACSP ACTR ATTT LTTV MVAR RHPC ... RVTV
```

> **L'elenco delle pagine non viene dal filesystem.** `mmi` non fa `ls` della
> directory: legge `*pag_num` e `*page_list`
> ([topLevelShellMain.c:1244](run_time/topLevelShellMain.c#L1244)). Una pagina il
> cui `.rtf` esiste ma che non compare in `*page_list` per l'MMI **non esiste**.

---

## 2. Come `mmi` trova le pagine (run-time)

```
cwd ──► Context.ctx ──► risorsa *pages ──► LEGOMMI_PAGINE ──► $LEGOMMI_PAGINE/<NOME>.rtf
```

1. **Quale Context** — il default è la stringa `"Context.ctx"`, quindi *relativa
   al cwd* ([run_time.c:193](run_time/run_time.c#L193)); si sovrascrive con
   l'opzione `-Context <file>` ([other_mmi.c:2053](run_time/other_mmi.c#L2053)).
2. **Lettura** — `LoadCtxResDb()` carica il file e lo fonde nel database risorse
   del display ([topLevelShellMain.c:416](run_time/topLevelShellMain.c#L416)).
3. **Estrazione** — `*pages` → `path_pagine`, `*animatedIconLibraries` →
   `path_ico`, `*simulator` → `path_simulator`.
4. **Esportazione** — `XlPutenv("LEGOMMI_PAGINE", path_pagine)` e
   `XlPutenv("LEGOMMI_ICO", path_ico)`
   ([topLevelShellMain.c:1909](run_time/topLevelShellMain.c#L1909)).
   `XlGetenv`/`XlPutenv` sono `getenv`/`putenv` nudi, senza fallback
   ([XlEnv.c:55](lib/Xl/XlEnv.c#L55)).
5. **Composizione del nome** — sempre nella stessa forma:

   | file | dove viene composto |
   |---|---|
   | `$LEGOMMI_PAGINE/<NOME>.rtf` | [other_mmi.c:193](run_time/other_mmi.c#L193), [teleperm.c:3486](run_time/teleperm.c#L3486), [OperatingWindow.c:2887](run_time/OperatingWindow.c#L2887) |
   | `$LEGOMMI_PAGINE/<NOME>.bkg` | [other_mmi.c:1404](run_time/other_mmi.c#L1404) |

**Il run-time non fa nessuna `chdir` e nessuna conversione relativo→assoluto.**
Un `*pages: ./` significa letteralmente "la directory da cui è stato lanciato
`mmi`". Per questo `kMmi` fa una `cd` prima di lanciarlo (§5).

### Opzioni di lancio ([run_time/other.h](run_time/other.h#L30))

| opzione | effetto |
|---|---|
| `-Context <file>` | usa un Context diverso da `./Context.ctx` |
| `-pag <NOME>` | pagina da aprire all'avvio (ripetibile, max `MAX_STARTUP_PAGES`) |
| `-pagback <NOME>` | pagina di background |
| `-alldisp` | apre la pagina di startup su tutti i display configurati |
| `-noclose` | inibisce la chiusura |
| `-Topologia`, `-DemonePort` | collegamento al demone |

---

## 3. Come `config` trova le pagine (editor)

Il configuratore usa una strada **diversa** dal run-time: risolve i path
**rispetto alla directory del file Context**, non al cwd.

1. `Context_Path = dirname(<file context>)`, o il cwd se il nome è nudo
   ([pagedit.c:119](config/pagedit.c#L119));
2. `XlChDir(Context_Path)`, poi `path_rel_to_abs` su ogni voce;
3. esporta le variabili ([pagedit.c:160-245](config/pagedit.c#L160)):

| variabile | da | contiene |
|---|---|---|
| `LEGOMMI_CTX` | directory del Context | radice del progetto |
| `LEGOMMI_PAG` | `*pages` | `.pag`, `.rtf`, `.bkg` |
| `LEGOMMI_LIB` | `*objectLibraries` | librerie oggetti |
| `LEGOMMI_ICO` | `*animatedIconLibraries` | icone animate |
| `LEGOMMI_RTF` | `*simulator` | `variabili.rtf` |
| `LEGOMMI_WORK` | `$LEGOMMI_CTX/proc` | uscite di regolazione ([EstrWorkFile](config/comp_pag_util.c#L592)) |

I nomi dei file si compongono con due sole funzioni:

- [`FileNameLoc(pagina, tag)`](config/comp_all.c#L1032) → `$LEGOMMI_PAG` + nome + tag
  (`.pag`, `.rtf`)
- [`FileNameInproc(pagina, tag)`](config/regol_util.c#L2846) → `$LEGOMMI_WORK` +
  nome **in minuscolo** + tag (`_04.dat`, `_14.dat`, `.reg`)

> **Attenzione ai due nomi diversi.** Il run-time usa `LEGOMMI_PAGINE`,
> l'editor usa `LEGOMMI_PAG`, per la stessa directory
> ([config.h:145](include/config.h#L145)). Impostarne una a mano non ha effetto
> sull'altro programma.

---

## 4. Ciclo di vita dei file di una pagina

| estensione | dove | prodotto da | letto da |
|---|---|---|---|
| `<NOME>.pag` | `$LEGOMMI_PAG` | editor `config` | editor, `kGlobContext` |
| `<NOME>.rtf` | `$LEGOMMI_PAG` | compilazione | **`mmi`** |
| `<NOME>.bkg` | `$LEGOMMI_PAG` | editor | `mmi` (sfondo) |
| `variabili.rtf` | `$LEGOMMI_RTF` (= `*simulator`) | il simulatore | compilazione |
| `<nome>_04.dat`, `_14.dat`, `.reg` | `$LEGOMMI_CTX/proc` | `compreg` | generazione task |

La compilazione è di due tipi ([config/comp_all.c](config/comp_all.c)):

- **sinottici** — compilati in-process da `config`, che istanzia i widget e
  scrive il `.rtf`;
- **pagine di regolazione** — delegate all'eseguibile esterno:
  `compreg <pagina> $LEGOMMI_PAG $LEGOMMI_RTF` ([comp_all.c:236](config/comp_all.c#L236)).

`IsPaginaCompiled` decide se ricompilare confrontando le date: `.rtf` contro
`.pag` per i sinottici, `_04.dat`/`_14.dat`/`.reg` contro `.pag` per le
regolazioni ([comp_all.c:715](config/comp_all.c#L715)).

---

## 5. La catena operativa in LegoPST (kprocedure)

Nell'uso reale non si tocca il Context a mano: lo generano le procedure.

```
$KSIM/globpages/          ← KPAGES: le pagine, condivise  (.pag .rtf .bkg + Context.ctx)
$KSIM/globpages_a/        ← una directory per istanza MMI: solo il suo Context.ctx
$KSIM/globpages_b/
$KSIM/globpages_c/
$KSIM/globpages_d/
$KSIM/kMmi.cfg            ← quali istanze esistono, su quale host e per quale utente
```

| passo | procedura | cosa fa |
|---|---|---|
| 0 | [Alg_env.sh:336](../Alg_env.sh#L336) | definisce `KPAGES=$KSIM/globpages` |
| 1 | [kGlobContext](../kprocedure/kGlobContext.sh) | `cd $KPAGES`, elenca i `*.pag` con `ls \| grep pag$`, estrae da ciascuno gli attributi e **rigenera** `Context.ctx` (con `*pages: $KPAGES`, `*simulator: $KSIM`, le librerie in `$HOME/legocad/libut_reg/libreg`). Il vecchio finisce in `Context.ctx.kold` |
| 2 | [kMmiConfig](../kprocedure/kMmiConfig.sh) | `rm -rf ${KPAGES}_*`; per ogni riga di `kMmi.cfg` relativa a questo host/utente crea `${KPAGES}_<id>/Context.ctx`, copia del globale con il solo `*hostNameS` completato con l'id SCADA |
| 3 | [kMmi](../kprocedure/kMmi.sh) `[id]` | mostra le istanze disponibili, poi `cd ${KPAGES}_<id>`, imposta `MMI_ULEVEL`, `. kuser 77<idScada>` e lancia `mmi &` |

Formato di `$KSIM/kMmi.cfg` — un'istanza per riga, campi separati da `;`:

```
a;1;I;RIC326282;antonio
b;2;I;RIC326282;antonio
c;3;O;RIC326282;antonio
d;4;O;RIC326282;antonio
 │ │ │     │        └── utente
 │ │ │     └─────────── host
 │ │ └───────────────── tipo: I=Instructor  O=Operator  S=Only Scada  X=Super instructor
 │ └─────────────────── id SCADA (1..4): diventa SHR_USR_KEY=77<id>0000 via kuser
 └───────────────────── identificatore dell'istanza, è il nome della directory globpages_<id>
```

Il tipo determina `MMI_ULEVEL` (0 operatore, 1 istruttore, 2 super), che finisce
nella classe dei widget `XlCore`/`XlManager` come livello di privilegio
([XlCore.c:783](lib/Xl/XlCore.c#L783)).

Conseguenza importante dello schema: **le pagine sono condivise** (una sola copia
in `$KPAGES`), mentre ogni istanza MMI ha una directory propria che contiene solo
il suo Context — e dove finiscono i file che `mmi` scrive nel cwd.

### File che `mmi` crea nella directory da cui parte

| file | a cosa serve |
|---|---|
| `Context.ctx_rtf` | cache binaria del Context, per non rileggere tutto ad ogni avvio |
| `ApplDb.res` | database risorse dell'applicazione ([topLevelShellMain.c:411](run_time/topLevelShellMain.c#L411)) |
| `LegoMMI.log` | log; `kMmi` aspetta che compaia per dichiarare l'MMI avviato |

La cache viene rifatta se `Context.ctx_rtf` è più vecchio di `Context.ctx`
([other_mmi.c:1376](run_time/other_mmi.c#L1376)). `NOCONTEXTRTF=YES` la fa usare
comunque, anche se scaduta.

---

## 6. Due configurazioni: MMI locale o MMI client/SCADA

`mmi` decide da solo come collegarsi ai punti, guardando **`*hostName` e
`*hostNameS`** del Context ([OlDatabasePunti.c:208](lib/Ol/OlDatabasePunti.c#L208)):

| `*hostName` | `*hostNameS` | tipo | come si collega |
|---|---|---|---|
| vuoto | vuoto | `DB_XLSIMUL` | **locale**: si attacca direttamente alla SHM del simulatore con `SHR_USR_KEY` |
| `<host> [cod]` | vuoto | `DB_XLSIMUL_CLIENT` | lancia `client_mmi` verso `<host>` |
| vuoto | `<host> [cod]` | `DB_XLSCADA_CLIENT` | lancia `client_scada` verso `<host>` |
| valorizzati entrambi | | `DB_XLCLIENTS` | entrambi i client |

Il commento nel codice è esplicito: *"Se non scrivo niente nel Simulator on e
Scada on del Context allora mmi in locale con simulatore"*.

### Se il simulatore gira sulla stessa macchina

È il caso normale in sviluppo, ed è la configurazione dei Context di produzione
(`hostName` e `hostNameS` vuoti). Si lancia così, con l'ambiente standard del
simulatore:

```sh
cd $KPAGES && mmi &
```

Lo stesso lancio, con la scelta automatica della directory, e' il pulsante **mmi**
del selettore [`lghmi`](../Alg_legopc/LGHMI.md#il-pulsante-mmi).

**`kMmi` non serve e anzi non funziona**, per due motivi indipendenti:

1. **`kMmiConfig` riempie `*hostNameS`.** La sua `sed` aggiunge l'id SCADA al
   campo: partendo da un `*hostNameS:` vuoto produce `*hostNameS: 2`. `mmi` lo
   interpreta come nome di host, `gethostbyname("2")` non fallisce (lo risolve
   come indirizzo numerico), quindi passa a `DB_XLSCADA_CLIENT` e lancia
   `client_scada`. Non essendoci nessun server SCADA, si blocca:

   ```
   Process client_scada started [2] [1]
   TIMEOUT SCADUTO pid= 138246
   ATTIVA CLIENT: Errore ricevimento msg ACK su richiesta pag.TEMPO per client_scada
   ```

2. **`kMmi` esegue `. kuser 77<idScada>`**, che riscrive `SHR_USR_KEY`
   (`kuser 772` → `7720000`). Il simulatore locale usa la chiave del suo
   ambiente (es. `10000000`, visibile con `ipcs -m`), quindi anche svuotando
   `*hostNameS` l'MMI cercherebbe segmenti inesistenti.

`kMmi`/`kMmiConfig` implementano la strada client/SCADA: hanno senso quando
esiste davvero un server (`server_mmi`/`demone_mmi`) e più postazioni operatore
con id SCADA distinti.

### Cosa serve in più per la strada SCADA

- `*hostName`/`*hostNameS` con il **nome host** (più l'eventuale codice):
  `*hostNameS: RIC326282 2`;
- `fnomi.rtf` nella directory `*simulator`, il database "light" della topologia:
  è un link creato da [kScadaInit](../kprocedure/kScadaInit.sh#L73) a partire da
  `$KFILEOP/fnomi.rtf`. Se manca, `mmi` non si ferma ma segnala

  ```
  XlWarning [OlDatabaseTopologia]: Impossibile aprire il file fnomi.rtf ...
  ERRORE nella costruzione dell'oggetto OlTree!!!
  ```

  e resta senza albero gerarchico (in locale è innocuo).

---

## 7. Diagnostica

### `mmi` parte e si blocca su `TIMEOUT SCADUTO`

```
Process client_scada started [2] [1]
TIMEOUT SCADUTO pid= ...
ATTIVA CLIENT: Errore ricevimento msg ACK su richiesta pag.TEMPO per client_scada
```

L'MMI sta cercando un server SCADA che non esiste, perché `*hostNameS` non è
vuoto. Su un simulatore locale va lanciato senza `kMmi`, con `cd $KPAGES && mmi &`
(§6).

### `kMmi` mostra una lista vuota

```
Mmi available on RIC326282 :
Selection :
```

`kMmi` filtra le righe di `$KSIM/kMmi.cfg` confrontandole con `$HOST` e `$USER`
([kMmi.sh:38](../kprocedure/kMmi.sh#L38)). Se il simulatore arriva da un'altra
macchina, le righe portano host e utente di quella e vengono scartate tutte.
Si corregge riscrivendo `kMmi.cfg` con host e utente correnti e rilanciando
`kMmiConfig` per ricreare le `globpages_*`.

Da sapere: il confronto è scritto `[ "$MmiHost" = "$HOST" ] & [ "$MmiUser" = "$USER" ]`
con una sola `&`, che manda in background il primo test — di fatto **decide solo
l'utente**. Una riga con l'host sbagliato ma l'utente giusto compare lo stesso.

### `SORRY : You have to answer with an available mmi identifier`

L'id scelto non è in `kMmi.cfg`, oppure manca la directory `${KPAGES}_<id>`:
va rilanciato `kMmiConfig`.

### Una pagina nuova non compare nell'elenco

L'elenco viene da `*page_list` nel Context, non dal filesystem: dopo aver aggiunto
un `.pag` servono `kGlobContext` e poi `kMmiConfig`.

### La pagina si apre ma è quella vecchia

Cache: cancellare `Context.ctx_rtf` nella directory dell'istanza, o verificare che
`NOCONTEXTRTF` non sia impostata a `YES`.

### `Error on GetFileDatabase Context.ctx - Exit.`

`mmi` è stato lanciato da una directory che non contiene il Context. Va lanciato
da `${KPAGES}_<id>` (lo fa `kMmi`) o con `-Context <file>`.

---

## 8. Tranelli

- **Non esiste un search path.** Una sola directory per categoria, presa dal
  Context. Se il path è sbagliato, l'MMI non prova nessuna alternativa.
- **`path_rel_to_abs` decide "è relativo?" cercando un punto qualsiasi nella
  stringa** ([utile.c:488](../AlgLib/libutil/utile.c#L488)). Un path relativo
  *senza* punto (`pagine`) resta relativo; va scritto `./pagine`. Un path
  assoluto che contiene un punto (`/home/x/sim.v2/pag`) prende il ramo
  "relativo": funziona, ma per caso.
- **`mmi` dipende dal cwd**, il configuratore no: il primo risolve `*pages: ./`
  rispetto alla directory da cui parte, il secondo rispetto alla directory del
  Context.
- **`kGlobContext` rigenera il Context dai soli `.pag` presenti.** In un
  simulatore consegnato con le sole pagine compilate (`.rtf`/`.bkg`) e senza
  sorgenti, produce `pag_num: 0` e cancella l'elenco: il Context precedente si
  recupera da `Context.ctx.kold`. In quel caso il Context non va rigenerato.
- **I Context ereditati portano path assoluti della macchina d'origine**
  (es. `/usr/users/milost64/...`): su un'altra macchina vanno rigenerati, non
  corretti voce per voce.
- **`kMmiConfig` scrive sempre l'id SCADA in `*hostNameS`**, anche quando il
  campo era vuoto: basta questo a far passare `mmi` dalla modalità locale a
  quella client/SCADA, con blocco su timeout (§6).
- **Solo l'editor conosce `iconlib_list`**, la lista di librerie di icone: è per
  la palette, non è un path di ricerca a run-time. Per le pagine una lista
  analoga non esiste.
