# Alg_legopc — CAD Grafico per la Costruzione di Modelli

## Componente legopc

L'applicazione principale è `tix/legopc.tix`, lanciata tramite:
```bash
tixwish8.x $LG_TIX/legopc.tix
```
È un'interfaccia grafica Tcl/Tix per la composizione di schemi impiantistici: l'utente assembla moduli da librerie su un canvas, definisce connessioni tra porte, e genera i file di input per il simulatore (F01/F14).

## Variabili d'ambiente chiave (Alg_legopc)

| Variabile | Path tipico | Contenuto |
|---|---|---|
| `LG_TIX` | `$LG_BIN/tix` | Script Tcl/Tix dell'applicazione |
| `LG_LIBGRAPH` | `$LG_ENTRY/libgraph` | Root delle risorse grafiche utente |
| `LG_LIBRARIES` | `$LG_LIBGRAPH/libraries` | Librerie di moduli (organizzate per sottodirectory) |
| `LG_PIXMAPS` | `$LG_LIBGRAPH/pixmaps` | Icone connettori (`.ppm`, `.gif`) |
| `LG_FILESI5` | `$LG_LIBGRAPH/files_i5` | Directory legacy dei file `.i5` (vedi sotto) |
| `LG_HELP` | `$LG_LIBGRAPH/help` | File `.tch` di aiuto per ogni modulo |

## Struttura delle librerie di moduli

Ogni libreria è una sottodirectory di `LG_LIBRARIES/`. Per ogni tipo di modulo (es. `valv_0`) esistono i file:

```
LG_LIBRARIES/
  h2Ocav/
    valv_0.pi3      ← struttura porte (formato testo)
    valv_0.pi4      ← definizione matematica (sorgente)
    valv_0.tcl      ← script di posizionamento sul canvas
    valv_0n.gif     ← icona orientazione Nord
    valv_0.i5       ← interfaccia compilata (generata da i32i5, vedi sotto)
  elettra/
    sble_0.pi4
    ...
```

## File `.i5` — interfaccia compilata del modulo

Il file `.i5` è generato automaticamente dal tool `i32i5` a partire dal `.pi4` ogni volta che si salva un modulo (`presave.tcl`). Descrive le porte, le variabili matematiche e le configurazioni possibili del tipo di modulo. Viene letto da `pag2f01` nella Phase II per costruire il file `f01.dat`.

### Modalità operativa — dual mode (flat vs libreria)

`pag2f01` e i script Tcl supportano due modalità, selezionate automaticamente dall'**esistenza fisica della directory `LG_FILESI5`**:

| Condizione | Modalità | Comportamento |
|---|---|---|
| `files_i5/` **esiste** | **flat (legacy)** | `.i5` in `LG_FILESI5/`, `.top` con riga vuota per modulo |
| `files_i5/` **assente** | **libreria (nuova)** | `.i5` in `LG_LIBRARIES/<lib>/`, `.top` con nome libreria per modulo |

In modalità libreria, quando `legopc` salva il `.top` (`fileio.tcl`), scrive il **nome** della libreria di provenienza (es. `h2Ocav`) al posto della riga vuota. `pag2f01` ricostruisce il path completo come `LG_LIBRARIES/<libname>/<modname>.i5`.

### Migrazione da flat a libreria

La sequenza corretta è:

```bash
# 1. Genera i .i5 mancanti direttamente nelle librerie (da .pi4)
#    Salta i moduli che hanno già il .i5 nella directory della libreria.
bash $LG_TIX/gen_missing_i5.sh

# 2. Rimuovi gli eventuali .i5 orfani rimasti in files_i5/
#    (moduli eliminati o rinominati, senza .pi4 in nessuna libreria)
rm $LG_FILESI5/*.i5        # solo se ci sono orfani segnalati
rmdir $LG_FILESI5          # rimuove la dir → modalità libreria ATTIVA

# 3. Riaprire e risalvare ogni .tom in legopc
#    per rigenerare il .top nel nuovo formato con i nomi libreria
```

**Perché non serve `migrate_i5.tcl`**: quello script copiava i `.i5` da `files_i5/` nelle librerie, ma `gen_missing_i5.sh` ottiene lo stesso risultato rigenerandoli da `.pi4` (i `.i5` sono file generati, quindi il risultato è identico). `migrate_i5.tcl` è utile solo se si vogliono preservare `.i5` modificati a mano.

`gen_missing_i5.sh` stampa un riepilogo per ogni modulo (GEN / SKIP / ERRORE) con contatori finali. Gli orfani in `files_i5/` vanno eliminati manualmente: non hanno `.pi4` in nessuna libreria quindi non vengono toccati dallo script.

## File `.tom` — topologia del modello

File di topologia salvato da legopc (`fileio.tcl`). Formato testuale, contiene per ogni istanza: tipo modulo, nome istanza, posizione canvas, path della libreria di appartenenza. Viene usato dall'applicazione per ricaricare lo schema; genera in parallelo il `.top` per `pag2f01`.

## Elementi della libreria `remark` — testo e display dinamici

La libreria `LG_LIBRARIES/remark/` contiene elementi di annotazione (non moduli di simulazione). Sul canvas hanno il tag `remarkdescr` e vengono salvati nel `.tom` come testo + font (nessuna porta). Tipi:

| Elemento | Classe | Inserimento | Comportamento |
|---|---|---|---|
| `@com_0` | `@com` | popup tasto-destro → **Add Text** (`AddRemark`) | Testo statico. Se inizia con `#tag`, anima la variabile `tag` (read-only: la variabile **non** è modificabile a run-time) |
| `@val_0` | `@val` | popup tasto-destro → **Add Display** (`AddDisplay`) | **Display dinamico**: casella valore senza testo statico (placeholder `--?--`); variabile **sempre ridefinibile a run-time** con doppio-click in *Show Value* |

**Vincolo sull'ordine dei tag (`@val_0.tcl`)**: i tag 0..7 devono restare identici a `@com_0` (`0=id, 2=cls, 3=ori, 5=lpath, 7=font`) perché `leggi_font` e altro codice in `legopc.tix` usano **indici posizionali fissi**. Il tag distintivo `freeval` va quindi aggiunto **per ultimo** (indice 8).

**Comportamento del display `@val_0`** (logica in `animate.tcl`, condivisa da `legopc.tix` tab *Data assignment & Simulation* / canvas `$c2` e da `draw2gr.tcl` *HMI & Plot*):
- In *View → Show Value*, doppio-click sul campo apre un dialogo che chiede il nome variabile, **validato** contro `tipVarMod` (rifiuta variabili non presenti nel modello).
- Una checkbox sceglie la **modalità di visualizzazione**: *solo valore* (`1.55E08 Pa`) oppure *etichetta* (`PCOL 1.55E08 Pa`).
- Stile casella come i blocchi: senza bordo, **giallo** in simulazione (valore live via pipe), **azzurro** (`cyan`) nei valori di stazionario (F14). La casella copre il placeholder.
- Il menu popup voce **Add Display** è abilitato negli stessi contesti di **Add Text** (sfondo / porta); la logica `entryconfigure` del popup usa **indici posizionali** che vanno riallineati in tutti i rami se si aggiungono/tolgono voci.

## Libreria `background` — icone/disegni decorativi (`bgimage`)

Libreria `LG_LIBRARIES/background/` per **elementi grafici di sfondo**: icone/disegni statici che **decorano** lo schema ma **non fanno parte della topologia** del modello (niente porte, niente F01, non animabili). Terzo tipo della famiglia non-topologica accanto a testo (`@com_0`) e display (`@val_0`).

> **Guida pratica per crearne di nuovi**: [Alg_legopc/BACKGROUND_ELEMENTS.md](BACKGROUND_ELEMENTS.md) (ricetta passo-passo + template `.tcl`).

**Formato elemento** (`@<nome>_0`, es. `@alb_0`):

| File | Necessario | Scopo |
|---|---|---|
| `<lib>.lib` | sì (per libreria) | Marker vuoto → rende la libreria selezionabile nel browser (`createPal`) |
| `@<nome>_0n.gif` | sì (per elemento) | Immagine (unico orientamento; suffisso `n` richiesto: il browser globba `*n.gif`) |
| `@<nome>_0.tcl` | **no** | Non serve: si usa lo script condiviso `$LG_TIX/bgelement.tcl`. Crearlo solo per un decoro con comportamento speciale (ha precedenza) |
| `.pi3` `.pi4` `.i5`, gif `e/s/w` | **no** | Non servono (niente porte/authoring/rotazione) |

**Aggiungere un decoro = mettere una GIF** nella libreria. Nessun `.tcl` (né per-elemento né per-libreria) tra le risorse grafiche.

**Convenzioni chiave**:
- **`@` iniziale obbligatoria** → esclusione automatica dal `.top`/F01 ([fileio.tcl](src/tix/fileio.tcl) `writeFiles`, `set nonmodulo [regexp {@} ...]`). È **questo** (non il tag `remarkdescr`) a escludere dalla topologia. Il `@` fa anche da **gate** in `elementScript` (vedi sotto).
- Tag distintivo **`bgimage`** aggiunto **per ultimo** (idx 7), tag 0..6 identici a `@com_0` (0=id,2=cls,3=ori,4=remarkdescr,5=lpath,6=name) per gli indici posizionali fissi.
- L'item è tipo **`image`** e ha tag **`module`** → lo **zoom lo scala già** (loop `find withtag module` in `doZoom`); la **rotazione è bloccata** (guard `remarkdescr` in `itemRotate` + assenza gif e/s/w).

**Script condiviso — `elementScript`** (definita in [fileio.tcl](src/tix/fileio.tcl), **non** in legopc.tix: `topRead` la usa e `fileio.tcl` è sorgiato anche da draw2gr/legodat/edit_simulx/select — metterla in legopc.tix rompeva `lghmi`/HMI perché `elementScript` era indefinita lì): i tre punti che istanziano un elemento (`itemAdd`, `itemAddFromfile`, `topRead`) sorgiano `<cls>.tcl` **se esiste**; altrimenti, **solo per elementi con nome `@…`**, ripiegano sull'unico script UI **`$LG_TIX/bgelement.tcl`** (uguale per tutte le librerie; sorgiato nello scope del chiamante → usa `$idclass` e imposta `mymodId`). Il gate `@` preserva la rete di sicurezza: un **modulo vero** (nome senza `@`) con `.tcl` mancante ritorna il path originale → `source` dà errore, come prima. `@com_0`/`@val_0` hanno il loro `.tcl` e non toccano il fallback.

**Inserimento**: via browser di libreria come un modulo (apri `background.lib` → seleziona icona → Ctrl+Left). Nessun codice di inserimento dedicato (`itemAdd`→`source [elementScript ...]`).

**Save/Load `.tom`** (rami keyati su `bgimage`, simmetrici): in save ([fileio.tcl](src/tix/fileio.tcl) `writeFiles`) il ramo `remarkdescr` **non** scrive testo/font se `bgimage` → blocco pass-2 = `classe\nnome\n++++`; in load (`topRead` pass-2, ramo `@`) se l'item è `bgimage` **consuma il blocco fino a `++++`** senza applicare testo/font (l'immagine è già ricreata nel pass-1 via `source .tcl`). Anche **paste** (`IncollaItem`) e **doppio-click** (`modifica_remark`) escono presto sui `bgimage` (nessun `-text`).

## File `.remap` — variabili animate persistenti

Side-file di runtime accanto al `.tom` (`<modello>.remap`), gestito da `animate.tcl` (`anim_load_remap` / `anim_save_remap`). Memorizza, per nome istanza, la variabile da animare scelta dall'utente — sia il **remap** di un campo di un blocco convenzionale, sia la variabile assegnata a un display `@val_0`. Formato testuale, una riga per istanza:

```
# LegoPC animation remap - generato automaticamente
TURB=T02TURBO1        ← blocco: campo rimappato sulla variabile T02TURBO1
VAL1=PCOL;L           ← display @val_0: variabile PCOL, modalità etichetta (token ;L)
VAL2=TCOL             ← display @val_0: variabile TCOL, modalità solo-valore
```

- Chiave = nome istanza (univoco sul canvas, garantito da `inputModName`).
- Valore = nome variabile; token opzionale **`;L`** (solo elementi `@val_0`) = modalità etichetta.
- Al caricamento le righe la cui variabile non è più nel modello (`tipVarMod`) vengono **scartate** e il file riscritto ripulito.

## File `.lstyle` — override per-modello dello stile delle connessioni

Side-file `<modello>.lstyle` accanto al `.tom` ([linkstyle.tcl](src/tix/linkstyle.tcl)), che memorizza **eccezioni per-modello** a colore/spessore/tratteggio delle linee di connessione (il default è per-categoria in `connect.dat`, `clines($tycon,...)`). Solo canvas topologia.

Due livelli, con priorità **default `connect.dat` < `CAT` < `LINK`**:

```
# LegoPC per-connection/per-category line style (per-model, auto-generato)
CAT  hydr color=#00aa00 width=2                 ← tutta la categoria hydr (solo questo modello)
LINK TURB.port1|COND.port0 color=red width=3 dash=1   ← singolo tratto (vince sulla categoria)
```

- **Chiave `LINK`** = coppia porte **normalizzata** `min|max` di `mod.port` (indipendente dall'ordine, stabile tra sessioni — a differenza del tag `link<sId>.<eId>` che usa ID item volatili). Risolta da `linkstyle_key_from_line` (linea→link tag→porte→moduli via `*.name`).
- **Chiave `CAT`** = nome categoria (`tycon`, prima 4 lettere del `*_ptype`).
- **UI**: tasto destro su una connessione → voce **"Line style…"** nel popup (abilitata solo sui link, indirizzata **per label** per non dipendere dagli indici del menu). Dialogo con scelta *This connection only* / *All "<cat>" connections*, color picker, spessore, tratteggio, **Apply** / **Reset to default**. Salvataggio immediato (come `.remap`).
- **Applicazione**: al load, dopo `topRead`, `linkstyle_reload $c` (in `raisetopol` e `apri_modello`). `showLinks` (View→Links) riapplica gli override quando una categoria torna visibile ("override vince"). `linkDelete` rimuove l'eventuale override del tratto cancellato. `writeFiles` chiama `linkstyle_save` (persiste anche su Save As).
- Deployato in `$LG_TIX` via makefile; sorgiato da `legopc.tix`.

## Command Mode in `draw2gr.tcl` (Linux) — perturbazione real-time via xaing

`draw2gr.tcl` (tab *HMI & Plot*) ha due modalità, commutate dal tasto a destra del pannello *Selected Set* (`.varch.buttMode.mode`), uguale alla versione Windows:

| Modalità | Colore tasto | Lista variabili del blocco | Click su variabile |
|---|---|---|---|
| **Plot** (default) | verde | tutte (`numVars`) | aggiunge la variabile al set da plottare |
| **Command** | rosso | solo ingressi indipendenti (`numVarsINDIP`) | invia una richiesta di **perturbazione** in tempo reale |

**Catena di perturbazione su Linux** (analogo del servizio `lgsincro`/`LgSincroAccShM.exe` di Windows): `draw2gr` → `xaing` (modalità send) → messaggio IPC `RIC_AING` → **pannello xaing** → `g_perturba()` → coda `id_msg_pert` → modello **net_simula** (scheduler `net_sked`).

- **`d2g_cmdmode_available`** (draw2gr.tcl): abilita l'ingresso in Command Mode solo se gira `net_sked` (rilevato via `ps -A -o comm`). Il path FMU/lgser è demandato alla Fase 3 (vedi memoria progetto).
- **`d2g_send_aing {name}`** (draw2gr.tcl): in Command Mode `setSlot` chiama questa proc che esegue `exec $LEGORT_BIN/xaing 3 <nome_var> &`.
- **`xaing 3 <nome_var>`** — nuova modalità `tipo_aing==3` ([xaing.c](../Alg_rt/grafica/xaing/xaing.c)): **non apre finestre X**; legge `SHR_USR_KEY`, fa `msg_create_fam`→`id_msg_aing`, lancia il pannello xaing (`xaing 1`) se assente (`xaing_panel_attivo()` scandisce `/proc`, `lancia_pannello_xaing()` fa `fork`+`execl`), costruisce `RICHIESTA_AING` con `nome_variabile` e fa `msg_snd(RIC_AING)`, poi esce. Il pannello xaing mostra il dialogo nativo (valore + tipo perturbazione: step/rampa/impulso) e applica via `g_perturba`.
- **Variabili di ingresso indipendenti** (`numVarsINDIP`/`nomeVarsINDIP`): popolate in `loadVariables` ([read_f01.tcl](src/tix/read_f01.tcl)) filtrando le variabili del blocco con **`tipo == "IN"`** e descrizione non commentata con `#` (stessa condizione di `listVblo(IN)`).

Riferimento sender C originale: [monit_perturba.c](../Alg_rt/net_simula/net_monit/monit_perturba.c) (`vfork`/`execve` di xaing + `msg_snd` di `RIC_AING`). Struttura messaggio: [ric_aing.h](../AlgLib/libinclude/ric_aing.h).

## Set Sim path — animazione/Plot/Command su una simulazione in un'altra directory

Voce **View → Set Sim path** (in `legopc.tix` tab *Data Assignment* e in `draw2gr.tcl`): imposta la globale `::anima_sim_path` (definita in `animate.tcl`) = directory della simulazione in corso. Serve quando la HMI è stata avviata in una dir diversa da quella della sim attiva. Tre consumatori, tutti da allineare a quella dir:

- **Animazione (viewval)**: `animate.tcl` fa `cd $::anima_sim_path` prima di `viewval -s` (e ripristina la cwd). Funziona già.
- **Plot (graphics)**: `f22name` è **assoluto e ancorato alla cwd di avvio** di draw2gr (`[file join $curdir argv1]`), quindi `ShowGraf_lin` — oltre al `cd` — **ripunta `args(0)`** a `<simdir>/<basename f22>`, altrimenti graphics legge un f22 sbagliato → grafico vuoto/crash.
- **Command (xaing)**: il pannello `xaing 1` (via `costruisci_var`) legge **`variabili.rtf` dalla cwd** e con esso aggancia la SHM; `d2g_send_aing` fa quindi `cd $::anima_sim_path` prima di `exec xaing 3` (altrimenti `[error shared-memory not attached]` → il pannello muore, "non compare nulla").

Helper condiviso `d2g_sim_dir` (draw2gr.tcl): ritorna `::anima_sim_path` se è una dir valida, altrimenti `""` (nessun `cd`/repoint → comportamento originale, es. bundle FMU dove la cwd è già la task).

## Unità di misura — tabella `uni_mis`, file per-simulazione, dialogo Units

La selezione dell'unità è **per tipo di grandezza** (prima lettera del nome variabile: `P`=pressione, `T`=temperatura, `W`=portata, …), non per variabile. Tabella `S_UNI_MIS uni_mis[]` ([uni_mis.h](../AlgLib/libinclude/uni_mis.h), default compilati in [uni_mis_val.h](../AlgLib/libinclude/uni_mis_val.h), variante `PIACENZA` da `$VERSIONE`): per ogni tipo fino a 5 unità con conversione lineare `val_vis = A[sel]*val_MKS + B[sel]`; `sel` = unità selezionata (0 = MKS).

**Ricerca del file unità** — `init_umis()` ([uni_mis.c](../AlgLib/libsim/uni_mis.c), self-contained: ripristina la cwd) cerca in ordine:
1. `./uni_misc.cfg` — **testo per-simulazione** (righe `TIPO=unità`, es. `PRESSION=bar`), applicato sopra i default compilati; righe non valide segnalate su stderr e ignorate;
2. `./uni_misc.dat` — binario legacy per-directory (storico Alg_mmi);
3. `$HOME/defaults/uni_misc.dat` — binario globale per-utente (creato coi default se assente).

`agg_umis()` salva **sempre dove init_umis ha letto** (stesso file e formato) → il dialogo *Defaults → Unità di misura* di graphics resta coerente. `crea_umis_cfg_locale()` crea/promuove il `.cfg` nella cwd. I chiamanti (viewval, graphics, grafics, xaing) chiamano `init_umis()` **prima** di `chdefaults()` con la cwd ancora sulla dir della sim.

**Tool `umis`** ([main_umis.c](../Alg_rt/net_simula/viewval/main_umis.c) → `$LEGORT_BIN/umis`, incluso nel bundle FMU): `umis -l` lista allineata e parsabile (`TIPO lettera sel u0|[u1]|... A B`: l'unità **selezionata è tra `[ ]`**, A/B sono i coefficienti **dell'unità selezionata**, `%.6g`; righe `#` = intestazioni, saltate da `umis_load` che rimuove anche le quadre); `umis <tipo|lettera> <unità>` imposta e salva nel `.cfg` per-sim della cwd (creato se assente); `-g` salva invece dove ha letto. Va lanciato dalla dir della simulazione. NB: solo `-l`/`-g`/`-h` sono opzioni (l'unità `---` inizia con `-`).

**Show Value e unità**: il path **live** (pipe `viewval -s`) converte già in viewval (`cerca_umis`→`sel`→`A*val+B`, [main_viewval.c](../Alg_rt/net_simula/viewval/main_viewval.c) modo server). Il path **statico F14** (caselle azzurre) converte lato Tcl: `umis_load` (animate.tcl) carica la tabella via `umis -l` dalla dir sim (`umis_sim_dir` = `::anima_sim_path` o cwd), `conv_umis` converte i valori `matrVf14`, `ret_umis` usa la tabella caricata (fallback: vecchia switch MKS hardcoded). `umis_load` è richiamata all'attivazione (modi 1 e 3 di `anima_aggiorna`).

**Dialogo View → Units…** (`umis_dialog` in animate.tcl; voci menu in draw2gr.tcl e legopc.tix, solo Linux): radiobutton per tipo (tipi a unità singola nascosti), OK → una `exec umis TIPO unità` per tipo cambiato → `refresh_cmd` (`umis_refresh_showvalue[_pc]`) rilancia Show Value se attivo, così **viewval riparte e rilegge le unità** (viewval legge il file solo all'avvio). Windows (sincview) non gestito.

**Gotcha indici menu**: in legopc.tix le proc `raisetopol`/`raisedata`/`raisetaskconf` fanno `entryconfigure` sul menu View; le voci dopo *Info...* sono ora indirizzate **per label** (`"Show OFF"`, `"Set Sim path"`, `"Units..."`, …) perché gli indici numerici cambiano tra piattaforme e a ogni voce aggiunta (l'inserimento di *Units...* aveva rotto `entryconfigure 4` → errore `unknown option "-state"` sul separatore).

## Tool C in `Alg_legopc/src/c_files/`

| Tool | Funzione |
|---|---|
| `pag2f01.c` | Legge `.top` + `.i5`, genera `f01.dat` per il simulatore |
| `i32i5.c` | Converte `.pi4` → `.i5` (lanciato da `presave.tcl`) |

## Risorse grafiche in `LG_PIXMAPS`

| File | Usato da | Scopo |
|---|---|---|
| `????_[news].ppm` | `legopc.tix` | Icone connettori sul canvas |
| `????[news].gif` | `preinit.tcl` | Bottoni toolbox connettori |
| `actconn.ppm` | `preinst.tcl` | Icona connettore attivo/selezionato |
| `zrotatel/r.gif` | `preinit.tcl` | Bottoni rotazione nella toolbox |
| `zxdelete.gif` | `preinit.tcl` | Bottone cancellazione nella toolbox |
| `broken.bmp` | `itemjoin.tcl` | Stipple pattern per linee tratteggiate |
