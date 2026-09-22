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
| `LG_TIX` | `$LG_BIN` | Script Tcl/Tix dell'applicazione |
| `LG_LIBGRAPH` | `$LG_ENTRY/libgraph` | Root delle risorse grafiche utente |
| `LG_LIBRARIES` | `$LG_LIBGRAPH/libraries` | Librerie di moduli (organizzate per sottodirectory) |
| `LG_PIXMAPS` | `$LG_LIBGRAPH/pixmaps` | Icone connettori (`.ppm`, `.gif`) |
| `LG_FILESI5` | `$LG_LIBGRAPH/files_i5` | Directory legacy dei file `.i5` (vedi sotto) |
| `LG_HELP` | `$LG_LIBGRAPH/help` | File `.tch` di aiuto per ogni modulo |
| `LG_HTML` | `$LG_BASE/Alg_legopc_help` | Manuale HTML aperto dal menu `?` → Help |
| `LG_BROWSER` | primo installato | Browser per l'help HTML |
| `LG_TEXTEDITOR` | primo installato | Editor di testo (file `.inp`, log, sorgenti) |
| `LG_ICOEDITOR` | primo installato | Editor delle icone dei moduli |
| `LG_PDFVIEWER` | primo installato | Viewer PDF/PNG |
| `LG_XTERM` | primo installato | Emulatore di terminale |

### Quale installazione è attiva — il titolo della finestra

`LG_ENTRY` è la radice utente, e spesso è un **link simbolico** che punta
all'installazione in uso:

```
~/legocad -> legopst_nuclear/legocad
```

Il titolo della finestra di `legopc` mostra quella radice, e **se è un link
scrive il path a cui punta** invece del nome del link, che sarebbe sempre
`legocad` qualunque installazione fosse attiva:

| radice utente | titolo |
|---|---|
| directory reale | `LegoPC-legocad - <modello>` |
| link | `LegoPC-> legopst_nuclear/legocad - <modello>` |

Lo decide `etichettaAmbiente` in [src/tix/legopc.tix](src/tix/legopc.tix), usata
sia all'avvio sia quando `applyUserFromTom` cambia radice aprendo un `.tom` di
un'altra installazione. Con un link **assoluto** compare il path assoluto.

## Help in linea (menu `?` → Help)

La voce chiama `open_hlp index` ([src/tix/openhelp.tcl](src/tix/openhelp.tcl)), che
lancia `$LG_BROWSER $LG_HTML/index.htm`. Due trappole, entrambe risolte:

- **`LG_BROWSER` non è più hardcodato** (vedi *Programmi esterni* qui sotto).
  Valeva `/usr/bin/mozilla`, che su Linux moderno non esiste: il menu si fermava
  su *"HTML browser not found"*. `open_hlp` applica la stessa catena di fallback
  dell'ambiente e ora segnala l'errore anche quando è `exec` a fallire (browser
  installato ma non avviabile). Su WSL i browser Qt/Chromium (Falkon) tendono a
  crashare per la GPU non accelerata: `firefox` è la scelta affidabile.
- **Il menu di navigazione era vuoto.** Le pagine sono generate con FrontPage 4.0
  e i pulsanti erano **applet Java** (`fphover.class`, gli "Hover Button"):
  nessun browser esegue più le applet, quindi la prima pagina appariva ma non
  aveva link cliccabili. I 33 applet delle 7 pagine `menu *.htm` sono stati
  convertiti in normali `<a href>` con lo stesso testo/destinazione/colori
  (classe CSS `fphover`, colore di hover nella variabile inline `--hc`).
- **Il frame di destinazione va dichiarato sul singolo link.** Il
  `<base target="rtop">` delle pagine di menu manda tutto nella striscia del
  titolo, alta il 20%: le pagine di secondo livello (che sono a loro volta dei
  frameset) finivano annidate dentro quel riquadro, con logo e menu duplicati.
  Ogni link ora porta il proprio `target`: **`_top`** se la destinazione è un
  frameset (13 link: le voci del menu principale e i "Home"), **`rbottom`** se è
  una pagina di contenuto (20 link). Regola valida per chiunque aggiunga voci:
  se il file di destinazione contiene `<frameset>` serve `_top`, altrimenti
  `rbottom`.

### Programmi esterni (browser, editor, viewer)

Cinque variabili dicono a legopc quali programmi lanciare. Ordine di precedenza:

1. la variabile **esportata a mano prima** di sorgiare `.profile_legoroot` vince;
2. altrimenti [Alg_env.sh](../Alg_env.sh) (funzione `lg_pick`) prende il **primo
   installato** di una lista di candidati — nessun nome hardcodato;
3. all'avvio di legopc, la scelta salvata in **`$LG_ENTRY/legopc_prefs.tcl`**
   (`::pref_browser`, `::pref_texteditor`, …) **sovrascrive** la variabile: è lì
   che finisce quello che si imposta da *File → Settings*, ed è lì che va
   cambiata la preferenza di un utente.

**Lo stesso dialogo è nel menu *File* di `lghmi`** (`settings.tcl` lo sorgiano
entrambi): i programmi di base sono quelli di tutto LegoPST, e il file delle
preferenze è uno solo, quindi la scelta fatta in un programma vale nell'altro.
Salvando da `lghmi` si riscrivono **solo** le cinque righe `set ::pref_*`: i
colori dei canvas, che legopc tiene in memoria e `lghmi` non ha, restano dove
sono. Vedi [LGHMI.md](LGHMI.md#file--settings--le-applicazioni-di-base).

**Il terminale** fa un passo in più: *File → Settings* ha accanto al campo
*Terminal* il menu *Installed* (i terminali che `lgterm` sa usare, fra quelli
presenti), e la scelta vale subito ovunque: `lgterm` rilegge `::pref_xterm` da
`legopc_prefs.tcl` a ogni lancio, e il profilo a ogni sorgiata (la preferenza
vince su `LG_XTERM`, come in legopc; `LGTERM` la forza in una shell). Le finestre di
terminale con un comando dentro (*Export as → FMU*, *Tools → Terminal*, che prima
si chiamava *Xterm*, e `kStat`/`kLeeF22`) le apre `lgterm`: vedi
[docs/COMANDI_LG.md](../docs/COMANDI_LG.md), *Il terminale*.

Prima erano hardcodate su programmi non sempre installati (`/usr/bin/mozilla`,
`kwrite`, più un `export LG_TEXTEDITOR=leafpad` in `.profile_legoroot` che
arrivava *dopo* Alg_env.sh e ne annullava la scelta), mentre `LG_ICOEDITOR`,
`LG_PDFVIEWER` e `LG_XTERM` non erano definite affatto — e
[src/tix/libraria.tix](src/tix/libraria.tix) fa `exec $env(LG_ICOEDITOR)` senza
guardia, quindi partiva l'errore Tcl sulla variabile inesistente.

Le pagine sono in `windows-1252`: se vanno modificate, non salvarle in UTF-8.
Le directory `_vti_cnf/`, `_derived/`, `_private/`, `_borders/` sono metadati
FrontPage, non pagine servite.

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

## La lista dei moduli della `libut` — `lista_moduli.dat`

Sotto `LG_LIBUT` c'è l'elenco dei moduli dichiarati nella libreria utente, una
riga per modulo: **quattro caratteri di nome**, poi eventuali marcatori e la
descrizione.

```
ATTU  Actuator
BRTT !bruciatore TG
CLAV* Non thermodynamic equilibrium cavity containing gas and water
```

**Il nome del file dipende dalla piattaforma**, e su Linux è
`lista_moduli.dat`. Il riscontro sta in `lg1fil`, che esiste nelle due varianti
e definisce il percorso della lista:

| | `LMODUL` |
|---|---|
| Linux | `lego_big/sorglego/sub/lg1fil.f` → `'../libut/lista_moduli.dat'` |
| Windows | `src/libs_dir/legolib/lg1fil.for` → `'..\..\libut\l_moduli.dat'` |

Su Linux `lista_moduli.dat` è anche l'**unico** che la catena di build legge
(`cad_maketask.sh` come dipendenza di `modulilib.a`, `cad_lism2lis.sh` per
ricavare gli oggetti da compilare) e l'unico che `cad_environment.sh` crea
quando prepara una radice utente. Lo stesso vale per la libreria di
regolazione, dove c'è solo `lista_schemi.dat` e nessun `l_schemi.dat`.

Chi legge la lista fa quindi il test sulla piattaforma —
[src/tix/foraus.tix](src/tix/foraus.tix) lo fa da sempre e
[src/tix/libraria.tix](src/tix/libraria.tix) dal settembre 2026:

```tcl
if { $::tcl_platform(os) != "Linux" } {
	set ::listamodfile "l_moduli.dat"
} else {
	set ::listamodfile "lista_moduli.dat"
}
```

> **Attenzione alle liste divergenti.** Finché `libraria` lavorava su
> `l_moduli.dat` anche su Linux, aprirlo in una `libut` appena creata dava
> errore (il file non c'è: `cad_environment.sh` crea l'altro), e dove i due
> file c'erano entrambi le modifiche fatte da `libraria` non arrivavano alla
> build, che continuava a leggere `lista_moduli.dat`. Le due liste divergevano
> in silenzio: succede in otto installazioni su nove di questa macchina, fino a
> 17 righe di differenza. Se in una `libut` esistono entrambi i file, **quello
> buono è `lista_moduli.dat`** e l'altro è un residuo da ignorare.

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

## Elementi della libreria `remark` — testo, display ed elementi operatore

La libreria **`$LG_TIX/remark/`** contiene elementi di annotazione (non moduli di
simulazione). **Sta con legopc, non con le librerie grafiche dell'utente**:
sorgente in [src/tix/remark/](src/tix/remark/), deployata in `Alg_legopc/bin/remark`
dal makefile di `src/tix`. Prima viveva in `LG_LIBRARIES/remark`, cioè dentro
`libgraph/libraries` del singolo impianto, il che legava elementi generali del CAD
all'assetto delle librerie di un modello.

**Compatibilità con i modelli già salvati**: il percorso della libreria finisce
nei tag del canvas (indice 5, `<path>.lpath`) e quindi **dentro il `.tom`** come
percorso assoluto. `elementScript` ([src/tix/fileio.tcl](src/tix/fileio.tcl))
intercetta il caso: se allo `lpath` salvato l'elemento non c'è, lo cerca in
`[remarkLibPath]` **prima** di ripiegare su `bgelement.tcl` — senza quel controllo
un `@com_0`/`@val_0` di un modello vecchio diventerebbe un decoro di sfondo,
perdendo testo e animazione. Non serve quindi toccare i modelli esistenti. Sul canvas hanno il tag `remarkdescr` e vengono salvati nel `.tom` come testo + font (nessuna porta). Tipi:

| Elemento | Classe | Inserimento | Comportamento |
|---|---|---|---|
| `@com_0` | `@com` | popup tasto-destro → **Add elements ▸ Text** (`AddRemark`) | Testo statico. Se inizia con `#tag`, anima la variabile `tag` (read-only: la variabile **non** è modificabile a run-time) |
| `@val_0` | `@val` | popup tasto-destro → **Add elements ▸ Display** (`AddDisplay`) | **Display dinamico**: casella valore senza testo statico (placeholder `--?--`). La variabile si sceglie **all'inserimento**, dallo stesso elenco filtrabile degli elementi operatore, ed è **sempre ridefinibile**: doppio-click in *Show Value*, o la voce del popup che per i testi è *Modify Text* |
| `@stz_0` | `@stz` | popup tasto-destro → **Add elements ▸ Faceplate (xstaz)** (`AddFaceplate`) | **Bottone faceplate**: apre una pagina di `r02.dat` con `xstaz`. Vedi [Elementi operatore](#elementi-operatore-delle-pagine-faceplate-e-set-value) |
| `@set_0` | `@set` | popup tasto-destro → **Add elements ▸ Set value** (`AddSetValue`) | **Invio di valori** a una variabile di ingresso durante la simulazione. Vedi [Elementi operatore](#elementi-operatore-delle-pagine-faceplate-e-set-value) |

**Vincolo sull'ordine dei tag (`@val_0.tcl`, `@stz_0.tcl`, `@set_0.tcl`)**: i tag 0..7 devono restare identici a `@com_0` (`0=id, 2=cls, 3=ori, 5=lpath, 7=font`) perché `leggi_font` e altro codice in `legopc.tix` usano **indici posizionali fissi**. Il tag distintivo (`freeval`, `hmistaz`, `hmiset`) va quindi aggiunto **per ultimo** (indice 8).

**Comportamento del display `@val_0`** (logica in `animate.tcl`, condivisa da `legopc.tix` tab *Data assignment & Simulation* / canvas `$c2` e da `draw2gr.tcl` *HMI & Plot*):
- In *View → Show Value*, doppio-click sul campo apre un dialogo che chiede il nome variabile, **validato** contro `tipVarMod` (rifiuta variabili non presenti nel modello). Sotto il campo c'è l'**elenco filtrabile** di tutte le variabili del modello, con tipo (`IN`/`US`/`UA`) e descrizione: scrivendo nel campo l'elenco si restringe ai nomi che contengono quel testo, un clic mette il nome nel campo, il doppio clic conferma. È lo stesso componente del dialogo *Variable to set* degli elementi operatore e di *File → Open Model* (`hmi_lista`, con la variante `hmi_lista_variabili`, in [src/tix/hmielem.tcl](src/tix/hmielem.tcl)); il filtro non distingue maiuscole e minuscole.
- Una checkbox sceglie la **modalità di visualizzazione**: *solo valore* (`1.55E08 Pa`) oppure *etichetta* (`PCOL 1.55E08 Pa`).
- Stile casella come i blocchi: senza bordo, **giallo** in simulazione (valore live via pipe), **azzurro** (`cyan`) nei valori di stazionario (F14). La casella copre il placeholder.
- **Il popup di Model Topology** raccoglie gli elementi in **Add elements ▸** (come *File → Export as ▸*), abilitato su sfondo e porte. Lo stato delle voci lo imposta `topol_pop_stati` **per etichetta**, non per indice: prima gli indici erano cablati in cinque rami, e ogni voce aggiunta li spostava tutti. Le due voci che cambiano etichetta — *Modify Text* (che sugli elementi operatore diventa *Assign page...* / *Assign variable...*) e *Delete* (*Delete link* sui collegamenti) — si indirizzano con l'indice ricordato alla costruzione (`::topol_pop_modifica`, `::topol_pop_delete`).

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
F001=RISCBP;F         ← bottone faceplate @stz_0: pagina RISCBP di r02.dat
S001=WEST;S           ← set value @set_0: variabile di ingresso WEST
```

- Chiave = nome istanza (univoco sul canvas, garantito da `inputModName`).
- Valore = nome variabile; token opzionale **`;L`** (solo elementi `@val_0`) = modalità etichetta; **`;F`** = pagina di faceplate (`@stz_0`); **`;S`** = variabile di un set value (`@set_0`).
- Al caricamento le righe la cui variabile non è più nel modello (`tipVarMod`) vengono **scartate** e il file riscritto ripulito. Le righe **`;F`** non si validano: il valore è una pagina, non una variabile.
- **Una voce alla volta con `anim_remap_set`**: rilegge il file, cambia la voce e lo riscrive, aggiornando anche la memoria. La usano le assegnazioni (display, elementi operatore, remap dei blocchi in legopc e draw2gr). `anim_save_remap` invece scrive **tutta la memoria**, che fuori da *Show Value* può mancare o essere di un altro modello, e cancellerebbe le voci scritte nel frattempo da un'altra applicazione (legopc e draw2gr lavorano sullo stesso file).
- **Compatibilità**: una versione di legopc/draw2gr precedente a settembre 2026 non conosce `;F` e, riscrivendo il file ripulito, **cancella** le pagine dei bottoni faceplate (le prende per variabili inesistenti).

### Remap di un blocco dal doppio clic (legopc e draw2gr)

In *View → Show Value*, un **doppio clic sul campo** sotto l'icona di un blocco
(giallo dal vivo, azzurro a simulazione ferma; non i remark) sceglie quale
**variabile del blocco** mostrare, e la scelta va nel `.remap` (`TURB=T02TURBO1`).
Il legame è uno solo, `anim_field_remap` in [animate.tcl](src/tix/animate.tcl);
cambia solo come si sceglie:

| | draw2gr | legopc (tab *Data Assignment & Simulation*) |
|---|---|---|
| doppio clic | il campo diventa verde e si apre l'elenco delle variabili del blocco nel **pannello del Plot** (`anim_field_select` → `showIt`) | il campo diventa verde e si apre il dialogo ***Variable to show***, con il nome del blocco nel titolo (`anim_field_dialog`) |
| scelta | clic su una variabile del pannello (`setSlot`) | elenco filtrabile delle variabili del blocco, con tipo e descrizione, e quella attuale selezionata; doppio clic, Invio o *OK* |
| annullare | secondo doppio clic sullo stesso campo | *Cancel*, Escape o chiusura |

Le variabili sono quelle del blocco nel F01 caricato (`blocNvar`/`blocVars`, le
stesse di `loadVariables`); legopc le legge direttamente, perché
`loadVariables` riscrive le globali del pannello dei dati. Il dialogo mostra la
variabile attuale e quella di default (`<prefisso .anim><istanza>`); nel campo
si può scrivere il nome anche in minuscolo.

Il salvataggio è comune (`anim_apply_remap`, spostata da `draw2gr.tcl` ad
`animate.tcl`): `anim_remap_set` per la voce del `.remap`, tag
`<variabile>.nome_anim` sul modulo (lo legge il ciclo dal vivo), testo della
casella aggiornato subito — a simulazione ferma con il valore di stazionario,
come il modo 3 — e campo rimesso del **suo** colore (prima tornava sempre
giallo, anche se era azzurro).

**Tag dei campi ripuliti a ogni avvio di Show Value** (`anim_togli_campi_vecchi`,
modi 1 e 3): prima il modo 3 non toglieva i `*.visual` dei campi precedenti e
il modo 1 solo il primo, così un modulo poteva portarne due e chi cercava il
primo trovava una casella già cancellata; allo stesso modo un `*.nome_anim`
vecchio restava davanti a quello nuovo quando il `.remap` cambiava da un'altra
applicazione, e il ciclo dal vivo mostrava la variabile vecchia.

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
- **Applicazione**: al load, dopo `topRead`, `linkstyle_reload $c` (in `raisetopol` e `apri_modello`). `showLinks` (View→Links) riapplica gli override quando una categoria torna visibile ("override vince"). `linkDelete` rimuove l'eventuale override del tratto cancellato. `writeFiles` chiama `linkstyle_save`; *File → Save As* copia `<modello>.lstyle` nella copia, rinominato (vedi *File → Open Model, Save Model, Save As, Include model, Delete Model*).
- Deployato in `$LG_TIX` via makefile; sorgiato da `legopc.tix`.

## Elementi operatore delle pagine: faceplate e set value

Due elementi della libreria `remark` trasformano uno schema in un **pannello per
l'operatore**: in *View → Show Value* (tab *Data Assignment & Simulation* di
legopc e HMI draw2gr) non si limitano a mostrare valori, ma aprono faceplate e
mandano valori alla simulazione in corso. Codice in
[src/tix/hmielem.tcl](src/tix/hmielem.tcl) (sorgiato da `animate.tcl`), elementi
[@stz_0.tcl](src/tix/remark/@stz_0.tcl) e [@set_0.tcl](src/tix/remark/@set_0.tcl).

| | Faceplate (`@stz_0`, tag `hmistaz`) | Set value (`@set_0`, tag `hmiset`) |
|---|---|---|
| cosa si assegna | una **pagina** di `r02.dat` | una **variabile di ingresso** (`tipVarMod` = `IN`) |
| nel `.remap` | `F001=RISCBP;F` | `S001=WEST;S` |
| in *Model Topology* | bottone grigio con il nome della pagina | casella grigia `WEST` + bottone grigio **Set** |
| in *Show Value* | **bottone disegnato** con il nome della pagina | casella `WEST 12.5 bar` (gialla dal vivo, azzurra fuori) + bottone **Set** |
| **clic sinistro** (al rilascio) | apre la pagina con `xstaz` | apre il dialogo di invio |
| **tasto destro** | menu: pagina assegnata, *Open page*, *Assign page...* | menu: variabile assegnata, *Set value...*, *Assign variable...* |

**In legopc si vedono come in *Show Value* a simulazione ferma**: casella per
il display, bottoni grigi per faceplate e set value, con dentro la variabile o
la pagina assegnata invece del testo dell'elemento. Nel tab *Model
Topology* si vedono **sempre** (lì *Show Value* non legge la simulazione, vedi
*Menu View* più sotto); nel tab *Data Assignment & Simulation* si
vedono finché *Show Value* è spento — ma quel tab ci entra già acceso, quindi
lì di norma si trovano subito le caselle vive, e spegnendolo tornano i
segnaposti.

Quando l'assegnazione non c'è ancora, la casella o il bottone ci sono lo
stesso, con il posto del nome occupato da **`--?--`** per il display (un solo
`?` darebbe una casella minuscola) e da **`xstaz: ?`** / **`set: ?`** per gli
altri due: si vede dov'è l'elemento e cosa gli manca.

Il disegno sta **sopra** l'elemento ma ha `-state disabled`: Tk lo disegna e non
lo considera nella scelta dell'oggetto sotto il puntatore, così il clic arriva
sempre all'elemento e trascinamento, selezione e menù del tasto destro
funzionano come prima. Non porta il tag `module` né quello dell'istanza: chi
conta i moduli (`writeFiles`, il `.top`) e chi legge i tag per posizione non lo
vede.

Il disegno non si sposta da solo: si rifà quando qualcosa cambia (caricamento
del modello, inserimento, incolla, assegnazione, fine trascinamento,
cancellazione, cambio di sovrapposizione), e durante il trascinamento si toglie
per non restare indietro; lo zoom invece lo scala da sé, come le caselle di
*Show Value*. Nel `.tom` resta un'etichetta di testo con il **solo nome** (la
pagina, la variabile, o `xstaz: ?` / `set: ?`): è quello che si vede dove il
disegno non c'è — una versione più vecchia, un altro programma — ed è corta
apposta, così la casella che le sta sopra la copre esatta senza allargarsi.

**Assegnazione.** Da *Model Topology* il dialogo si apre subito dopo l'inserimento
(si può annullare) e poi con la voce del popup che per i testi è *Modify Text*; in
*Show Value* dal menu del tasto destro. In entrambi i casi il valore va nel
`.remap` (`anim_remap_set`): nel `.tom` resta solo l'etichetta del segnaposto,
che `hmi_aggiorna_etichette` riallinea al file dopo ogni caricamento.
- **Pagina**: il dialogo elenca le pagine degli `r02.dat` trovati (`staz_dirs`, in
  [src/tix/lgstaz.tcl](src/tix/lgstaz.tcl)): la directory della simulazione
  (*Set Sim path*), il simulatore corrente (`$KSIM`) e le task dell'area del
  modello, con l'`S01` per le task di regolazione. Un nome che non c'è si può
  usare lo stesso, dopo conferma.
- **Variabile**: elenco filtrabile degli ingressi del modello con la loro
  descrizione (lo stesso componente del dialogo dei display, `hmi_lista_variabili`).
  In *Model Topology* le variabili non sono caricate — le carica il tab dei dati —
  quindi se accanto al modello c'è già un `f01.dat` lo si **legge** (`readF01`,
  `hmi_assicura_variabili`), senza ricostruire niente: `cad_crealg1` riscrive i
  file della task e ci mette secondi. Se quel file non c'è, all'inserimento non
  si chiede niente: l'elemento resta con `?` e la variabile si assegna più
  tardi; una variabile calcolata (`US`/`UA`) è rifiutata, perché il modello
  la riscriverebbe al passo successivo. Se il F01 non è caricato (Model
  Topology) il nome non si può verificare, e il dialogo lo dice.

**Apertura della pagina.** È la stessa di `lghmi` (`staz_apri`, in `lgstaz.tcl`):
si cerca l'`r02.dat` che definisce la pagina, si avvia `xstaz 1` in quella
directory se non gira già, e si manda `stazpag <pagina>`. Se `xstaz` gira su
un'altra directory la richiesta si rifiuta con un dialogo: la coda è una sola
per simulazione.

Il clic agisce **al rilascio** del tasto, come un bottone vero, e un secondo
clic entro mezzo secondo si ignora: con i display si è abituati al doppio clic,
e il secondo clic arrivava allo schema, che il window manager portava sopra il
dialogo appena aperto.

**Invio di un valore.** Il dialogo (uno per elemento, non bloccante) mostra il
valore attuale aggiornato ogni secondo e ha due strade:
- **Send**: scrittura diretta con `viewval VAR -f <valore> -l <registro>`. Il
  valore si digita nelle **unità mostrate** e si converte in quelle interne con
  l'inversa della tabella unità (`interno = (visuale - B) / A`), perché `-f` non
  converte. `viewval` esce con 0 anche senza simulazione, quindi lo stato si
  controlla prima.
- **Perturbation...**: il pannello `xaing` (gradino, rampa...), applicato dallo
  scheduler, come il Command Mode di draw2gr.

Il dialogo è **transient** della finestra dello schema: resta sopra di lei
quando si clicca lo schema e non ha un'icona sua. Se è già aperto, un nuovo clic
lo **ritira e lo rimostra** vicino al puntatore, perché su WSLg/XWayland `raise`
e `deiconify` di una finestra già mappata spesso non hanno effetto. Prima di
questa correzione, dopo qualche apertura e chiusura il dialogo poteva restare
nascosto dietro lo schema, o iconizzato senza che si riuscisse a ripristinarlo,
e sembrava non aprirsi più finché non si chiudeva draw2gr. Il suo aggiornamento
periodico è uno solo e si ferma con la finestra (prima, chiudere e riaprire
entro un secondo lasciava vivo quello vecchio).

I numeri si scrivono **con il punto** decimale: `0,5` viene rifiutato, e il
dialogo lo dice.

Ogni invio va nel **registro delle scritture** `hmi_setvalue.log`, nella
directory della simulazione (in `/tmp` se non è scrivibile): una riga di
commento con valore e unità mostrate, seguita dalla riga che `viewval -l`
scrive con i valori interni prima e dopo.

**Senza simulazione non si disturba.** "Dal vivo" vuol dire pipe di *Show Value*
aperta e `net_sked` vivo (`hmi_live`, controllo di `net_sked` al massimo ogni
3 s). Altrimenti:
- il **set value** si disegna **spento** (bottone grigio, casella azzurra con il
  valore di stazionario) e un clic mostra un **fumetto breve** (*No simulation
  running*), senza dialoghi: non c'è niente a cui mandare il valore;
- il **faceplate** invece resta **attivo** e apre la pagina lo stesso, con i
  valori fermi (fumetto *Page … requested. No simulation running: the values
  are not live.*): serve a costruire e configurare le stazioni senza avviare
  la simulazione;
- se la simulazione si ferma mentre *Show Value* è attivo, al ciclo successivo
  gli elementi si spengono, e il dialogo di invio disattiva i suoi bottoni;
- ogni comando esterno gira in un `catch`, con l'output in `/tmp/legopc_hmi.log`;
  nello stesso log vanno le aperture del dialogo di invio e **ogni tentativo di
  invio**, anche quelli che non partono, con il motivo (simulazione assente,
  valore non numerico): è la prima cosa da guardare se "non succede niente";
- `xstaz` parte anche senza `net_sked`: la coda delle richieste la crea lui
  (vedi `staz_apri` in [lgstaz.tcl](src/tix/lgstaz.tcl) e
  [LGHMI.md](LGHMI.md)); se manca `SHR_USR_KEY` non lo si lancia, perché
  andrebbe in crash;
- se manca `viewval`, *Show Value* lo dice con il dialogo di sempre **e** nella
  riga di stato del tab *Data Assignment* (`hmi_stato`).

Fuori da Linux gli elementi si vedono ma restano spenti.

## File → Export as (PDF, PNG) — in legopc e in draw2gr

Il canvas in vista si esporta in PDF o PNG da *File → Export as ▸*: in legopc
(il tab corrente, *Model Topology* o *Data Assignment*) e in draw2gr (lo schema
della HMI, compresi i valori di *Show Value*). Il codice è uno solo,
[src/tix/esporta.tcl](src/tix/esporta.tcl), sorgiato da `legopc.tix` e da
`draw2gr.tcl`; prima stava in `legopc.tix`, e draw2gr non l'aveva.

**La strada normale** passa per Ghostscript: il canvas diventa PostScript con
il comando di Tk (`$c postscript`, `plotPS_internal`, pagina A4 con l'orientamento
che fa venire il disegno più grande) e `gs` lo converte in PDF (`pdfwrite`) o in
PNG (`png16m`, a 96/150/300 dpi scelti in un dialogo). Il file va accanto al
`.tom`, con il nome del modello (`<modello>.pdf`, `<modello>.png`); se quella
directory non è scrivibile — un bundle FMU installato in sola lettura — si
chiede dove salvarlo. Poi il PDF si apre con il viewer (`LG_PDFVIEWER`, o il
primo tra evince/okular/...), il PNG con `LG_ICOEDITOR` se c'è, altrimenti con
il viewer.

**Senza Ghostscript l'esportazione è impossibile**, e il dialogo lo dice per
prima cosa, con il comando per installarlo. Poi **propone** un ripiego, che si
fa solo rispondendo *Sì* (default *No*: nessun file che non si è chiesto):

| Formato | Ripiego proposto |
|---|---|
| PDF | salvare il **PostScript** (`<modello>.ps`: vettoriale, stampabile), da convertire poi con `ps2pdf` |
| PNG | **fotografare la finestra** con il pacchetto Tcl `Img` (formato `window`) o con `import` di ImageMagick: solo la parte del disegno visibile, alla risoluzione dello schermo |
| PNG, senza nemmeno quelli | salvare il PostScript, come per il PDF, con il comando `gs` per il PNG |

Se Ghostscript c'è ma fallisce, il messaggio dice *Esportazione non riuscita*
con l'errore, e il PostScript resta al posto del risultato.

Nel **bundle FMU** `esporta.tcl` c'è (`build.sh` lo copia con gli altri script
di draw2gr), Ghostscript no: sulla macchina target l'export funziona se `gs` è
installato, altrimenti valgono i ripieghi. Un bundle generato prima di
`esporta.tcl` non mostra la voce, senza errori.

In legopc *Export as* si accende anche subito dopo *Open Model*/*Save* (come
*Include model*, `modelli_menu_modello`): prima solo il cambio di tab la
accendeva.

## Menu Edit in `draw2gr.tcl` (Linux) — opzione `-edit`

`draw2gr.tcl` ha un menu **Edit** (*Edit model (legopc)...*) che apre nel CAD il modello della task della HMI. È **assente di default** e compare solo con l'opzione con nome `-edit` (`draw2gr.tcl 1 f22circ -edit`), che lo script toglie da `argv`/`argc` come prima cosa: gli argomenti posizionali restano quelli di sempre, su Linux e su Windows.

- **Chi la passa**: solo `lghmi`, per le task dell'area corrente e non con `-noedit`/`-insim`. Non la passano `legopc` (`watchtrends`), i bundle FMU (`run_draw2gr.sh`), `run_fmu --hmi`, `lg_cosim`.
- **Quando è ignorata anche se passata**: fuori da Linux, con `LG_FMU_BUNDLE`, se `lgedit.tcl` non si legge, e se `draw2gr` discende da un processo `legopc.tix` (catena dei padri in `/proc`).
- **Controlli al clic**: quelli di `lghmi` *Tools → Edit model*, dallo stesso file [src/tix/lgedit.tcl](src/tix/lgedit.tcl) (`modifica_task`): simulazione in corso, task di un'altra area, `.tom` mancante, `legopc` già aperto sulla task. `lgedit.tcl` è sorgiato da `draw2gr` **solo** con `-edit`.

Dettagli e tabella dei chiamanti: [LGHMI.md](LGHMI.md#il-menu-edit-delle-hmi-draw2gr--edit).

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

## File → Open Model, Save Model, Save As, Include model, Delete Model — l'elenco dei modelli dell'area

*File → Open Model...* non apre più il selettore di file: mostra l'**elenco dei
modelli** che stanno in `$LG_MODELS` (su Linux `~/legocad`, cioè l'area scelta
con `lgswitch`), con l'area nel titolo (*Open Model - legopst_nuclear*). Codice
in [src/tix/modelli.tcl](src/tix/modelli.tcl), sorgiato da `legopc.tix`.

```
Model:  [slb_______________]
Model          Modified          In use
SLB1_NI2       2026-09-15 16:23
Test_x1        2026-09-18 16:11  open in legopc (pid 21372)
Beta           2026-09-18 16:11  simulation running
        [ Open ]  [ Browse... ]  [ Cancel ]
```

- **Cosa è un modello**: una directory con il `.tom` **omonimo**
  (`<dir>/<dir>.tom`) — quello che legopc apre per nome (`topRead`) e che lghmi
  considera il modello della task (`tom_della_task`). Le directory con soli
  `.tom` di altro nome (copie di backup come `MDC_NI0_bad/MDC_NI0.tom`) non
  compaiono; una directory con più `.tom` compare una volta, per quello
  omonimo. Le task di regolazione `r_*` non hanno `.tom` e non compaiono.
- **Colonne**: nome, data dell'ultima modifica del `.tom`, e chi lo sta usando.
  Ordine alfabetico (`lsort -dictionary`).
- **Ricerca**: è l'elenco filtrabile di *Variable to set* (`hmi_lista`):
  scrivendo nel campo le righe si restringono, un clic mette il nome nel campo,
  doppio clic o *Open* (o Invio) apre. Nel campo basta un pezzo del nome se
  lascia una riga sola, e le maiuscole non contano.
- **Browse...** apre il selettore di file di prima, per un `.tom` fuori
  dall'area o con un nome diverso dalla sua directory.
- **Modifiche non salvate**: prima dell'elenco c'è la stessa domanda del cambio
  di tab (`avverti`: salvare topologia, f01, f14?). Come lì, rispondere *No*
  **annulla** l'apertura: non esiste un "scarta e continua".

**"In use"** — calcolato all'apertura dell'elenco e **ricontrollato al
momento di aprire**, perché nel frattempo può cambiare:

| Segno | Come si vede |
|---|---|
| `open in legopc (pid N)` | un **altro** legopc ha la directory corrente nella directory del modello: legopc ci si porta quando apre un modello. Stesso criterio di `legopc_aperto_su` (lgedit.tcl), ma in un giro solo per tutti i modelli |
| `simulation running` | un `net_sked` dell'utente gira nella directory di un simulatore il cui `S01` elenca il modello tra le task di processo (`net_startup` fa `cd` nella directory del simulatore). Nei bundle FMU conta anche la directory del `net_sked` stessa |

In entrambi i casi aprire si può, dopo un avviso con *Yes/No* (default *No*).
È più preciso del controllo di *Edit model* di lghmi (`sim_attiva`), che rifiuta
se gira **una** simulazione qualsiasi: qui conta quella del modello. Le
directory si confrontano per identità (`stessa_directory`, device+inode),
perché `~/legocad` e `~/sked` sono link e la stessa directory arriva con grafie
diverse; per il titolo, `modelli_fisica` risolve anche il link dell'ultimo
componente, che `file normalize` lascia com'è. Su Windows la colonna *In use*
resta vuota (niente `/proc` né `pgrep`).

`lgpc` lanciato senza argomento parte vuoto come prima; con il nome di un
modello lo apre direttamente (vedi [docs/COMANDI_LG.md](../docs/COMANDI_LG.md)).

### In scrittura: Save Model e Save As

Lo stesso elenco, con le stesse colonne, serve a scegliere il **nome di un
modello nuovo** (`modelli_chiedi_nome`). Mostra i nomi già presi, e sotto il
campo una riga dice in tempo reale se il nome va bene; *Save* resta spento
finché non va bene.

- ***Save Model*** con un modello aperto **salva subito**, senza dialogo, come
  prima. Il dialogo compare solo per un modello **mai salvato** (il ramo
  "untitled" di `topWrite` in [fileio.tcl](src/tix/fileio.tcl), che chiama
  `modelli_salva_nuovo`), al posto del campo *ModelName Selection*. Resta
  **sincrono**: `topWrite` aspetta la scelta e ritorna 0/1 come prima, quindi
  funziona anche dalla domanda "Save...?" del cambio di tab. Gli altri
  programmi che sorgiano `fileio.tcl` non caricano `modelli.tcl` e tengono il
  campo di prima.
- ***Save As...*** (`modelli_salva_come`, al posto di `DupModel`): prima la
  domanda sulle modifiche non salvate (`avverti`, *No* annulla), perché la
  copia si fa **dai file su disco** e deve partire dall'ultima versione del
  disegno. Copia `.tom` (rinominato), `f14.dat`, `f01.dat`, `foraus.for`,
  `tasks.dat`, `simul.dat` — come prima — **più `<modello>.remap` e
  `<modello>.lstyle` rinominati**, che prima si perdevano: assegnazioni di
  display, faceplate e set value, e stili delle connessioni. `proc/` e i file
  di build no: la copia si ricompila all'apertura. Poi si passa alla copia.
  Se la copia fallisce, la directory appena creata si cancella.

**Regole del nome nuovo** (`modelli_nome_nuovo`): quelle di prima — non
esistente, al massimo 8 caratteri — più solo lettere, cifre, `_` e `-` (un `/`
creava sottodirectory) e nessun omonimo **anche con maiuscole diverse**
(`slb1_ni2` contro `SLB1_NI2`: due directory distinte solo su Linux). Contano
tutte le voci dell'area, non solo i modelli: `libgraph`, le task `r_*`, i
backup. Un nome esistente si rifiuta sempre: in scrittura l'elenco non
sovrascrive.

Attenzione a un comportamento di `writeFiles` che resta com'era: riscrive il
`.tom` **solo se il modello risulta modificato** (`modified`), mentre il `.top`
lo riscrive sempre. Un modello nuovo salvato con il canvas vuoto crea quindi la
directory e il `.top`, ma non il `.tom`.

*Import f01* ha ancora il suo campo *New model* (scelta dell'utente).

**La voce *Include model...*** si accende anche dopo *Open Model*, *Browse*,
`lgpc <modello>`, *Save Model* e *Save As* (`modelli_menu_modello`): prima la
accendeva solo il cambio di tab (`raisetopol`, per indice 4), e dopo un'apertura
restava spenta finché non si passava a un altro tab e si tornava.

### Include model

Lo stesso elenco, **senza il modello corrente**, e sotto la posizione
dell'incluso: *above* (default), *below*, *on the left*, *on the right*
(`modelli_includi`). La fusione la fa ancora `inhoud`
([src/inhoud/inhoud.c](src/inhoud/inhoud.c)): nella directory del modello
corrente, `inhoud <corrente> <incluso> <N|S|W|E>` legge i due `.tom` **dal
disco** e scrive `inhoud.tom`, che prende il posto di `<corrente>.tom`; poi il
modello si ricarica e si ricompila. Intorno:

- prima, la domanda sulle modifiche non salvate (`avverti`, *No* annulla),
  perché `inhoud` legge il file;
- se il modello corrente è aperto in un altro legopc o la sua simulazione è in
  corso, un avviso *Yes/No*: è lui che si riscrive e si ricompila (l'incluso si
  legge soltanto);
- **backup** `<corrente>.tom.bak`, riscritto a ogni inclusione;
- se `inhoud` fallisce ("ties to nonexisting blocks", "no symbols more left",
  troppi blocchi) il suo messaggio si mostra e il `.tom` corrente **non si
  tocca**;
- i blocchi dell'incluso con un nome già usato vengono rinominati da `inhoud`
  (quarto carattere: `0`–`9`, poi `$`, `?`, `!`) e le coppie vanno in
  `changed.out`: una finestra le mostra;
- **`.remap` e `.lstyle` dell'incluso si fondono nel corrente**, con i nomi
  nuovi. Nel `.remap` si rinominano la chiave (l'istanza) e il valore quando è
  una variabile — una variabile LEGO porta nei suoi ultimi 4 caratteri il nome
  del blocco (`WALIPGCD` è del blocco `PGCD`), e la ricompilazione le dà il
  nome nuovo — ma non la pagina di un faceplate (`;F`). Del `.lstyle` entrano
  solo gli stili dei singoli tratti (`LINK`): gli override di categoria (`CAT`)
  valgono per tutto il modello e ricolorerebbero anche i tratti del corrente;
- l'incluso deve stare nella stessa area del corrente (`inhoud` lo cerca in
  `../<nome>`): se il modello aperto non è in `$LG_MODELS` l'inclusione si
  rifiuta.

I dati (`f14`) del modello incluso restano fuori, come prima: la ricompilazione
parte dall'`f14` del corrente, e i blocchi inclusi si completano in *Data
Assignment*.

### Delete Model

Lo stesso elenco (`modelli_cancella`), con il bottone *Delete* rosso. Il modello
**non si cancella**: la sua directory si **sposta nel cestino dell'area**,
`$LG_MODELS/.deleted/<nome>_<AAAAMMGG_hhmmss>`. Il punto la nasconde a tutti gli
elenchi (`glob *` non vede le directory nascoste: né quelli di legopc né il
dir-scan di lghmi), e il cestino segue l'area quando `lgswitch` la sposta.

- **Rifiuto** se il modello è in uso: è quello aperto in questo legopc, è
  aperto in un altro legopc, o la sua simulazione è in corso. Un avviso non
  basterebbe: si toglierebbe la directory a chi la sta usando.
- **Conferma** *Yes/No* (default *No*) con path, dimensione (`du -sk`) e
  destinazione, più due avvisi quando servono:
  - il modello è una task di un **simulatore dell'area**: un `S01` sotto
    `$KSKED` (`~/sked`) lo elenca. Quel simulatore non si aggiorna (`kUpSim`) e
    non parte finché il modello non torna o non si toglie dal suo `S01`, che
    *Delete Model* **non modifica**;
  - **altri processi** lavorano nella sua directory (una HMI `draw2gr`, una
    shell, un editor): spostarla non toglie loro niente, ma quello che salvano
    da lì in poi finisce nel cestino.
- Invio **non** cancella: servono il bottone o il doppio clic, e poi la conferma.
- Alla fine un messaggio dice dov'è finito e dà il comando per ripristinarlo.

**Ripristino e svuotamento, a mano** (scelta dell'utente):

```sh
ls $LG_MODELS/.deleted                                   # cosa c'e' nel cestino
mv $LG_MODELS/.deleted/SLB1_NI2_20260918_170821 $LG_MODELS/SLB1_NI2   # ripristino
rm -rf $LG_MODELS/.deleted/SLB1_NI2_20260918_170821      # cancellazione definitiva
```

Il ripristino funziona solo se nel frattempo non è stato creato un modello con
lo stesso nome. Un modello pesa: `SLB1_NI2` occupa 130 MB, tra `proc/` e i file
di build.

**Il File menu si indirizza per etichetta.** `raisetopol`, `raisedata`,
`raisetaskconf`, *Import f01*, *New Model* e il `topRead` di `muovi.tcl`
accendevano e spegnevano le voci per indice (0–11): *Delete Model*, inserito
dopo *Include model*, le avrebbe spostate tutte. Ora usano le etichette
(`entryconfigure "Save As..."`), come il menu View.

**`inhoud` corretto (settembre 2026).** Tre difetti del C:

- le righe si leggevano a 79 caratteri (`char line[81]`), mentre nei `.tom`
  reali arrivano a 107: ora si leggono intere (`LINE_MAX_INH` 1024);
- **perdeva la parola `busy`** nelle connessioni del modello incluso su porte a
  due cifre: `busy port17 DEGA` diventava ` port17 DEGA`, perché `char b[6]`
  riceveva `port17` più il terminatore e il byte in più azzerava `a`. Ogni
  inclusione rovinava così le connessioni dalla porta 10 in su (13 su un
  modello come `PWRN1PSS`);
- `changed.out` usciva spezzato su due righe per coppia (il nome vecchio
  veniva copiato con il suo `\n` in un campo di 5 byte).

Verifica: su tre coppie di modelli reali il nuovo `inhoud` produce lo stesso
file del vecchio **tranne** le righe `busy portNN`, che ora sono giuste.

## Menu View — ordine delle voci e modo di visualizzazione di partenza

In **legopc** l'ordine delle voci (`legopc.tix`, blocco `set m .menu.view`) è:
prima i **modi di visualizzazione**, poi gli strumenti del disegno.

```
Show Value          <- radio, e' il default del tab Data Assignment
--------
Show OFF
Show Names
Show Classes
Show Connections...
--------
Links...
Info...
Set Sim path      >  (solo Linux)
Units...             (solo Linux)
StileAnim            (su Windows: Show infoitemname)
--------
Zoom              >
Find name
```

**Ogni tab ha il suo modo di default, e ci torna a ogni ingresso.** La globale
`showon` (a cui sono legati i radiobutton) vale per il tab in vista; i default
stanno nell'array `::showon_tab` (`1` Model Topology, `2` Data Assignment &
Simulation, `3` Task Configuration) e li rimette `cambia_tab_showon`, chiamata
da `raisetopol`/`raisedata`/`raisetaskconf`. Dentro un tab il modo si cambia a
mano quanto si vuole, ma uscendo e rientrando si riparte dal default:

| Tab | Modo a ogni ingresso | Perché |
|---|---|---|
| *Model Topology* | **Show Names** | è il tab del disegno: serve sapere come si chiama ogni blocco |
| *Data Assignment & Simulation* | **Show Value** | è il tab della simulazione: si entra per vedere i valori |
| *Task Configuration* | **Show Names** | — |

Uscendo da *Show Value* `cambia_tab_showon` fa anche `anima chiudi`: se no la
pipe di `viewval` resterebbe a girare su un canvas non più in vista.

**Show Value vale solo nel tab dei dati.** Negli altri tab vuol dire soltanto
"pagina senza etichette": `ShowNamesfilt` esce subito se `modalita != 2`, quindi
**non apre la pipe di `viewval`** e non legge la simulazione (coerente con i
segnaposti disegnati, che sono statici). Due punti da non toccare:

- `raisetopol` con `showon == 4` (succede solo se si sceglie *Show Value* a mano
  restando nel disegno) fa `ShowNames $c 1`, non `ShowNames $c 4`: con `4` la
  proc disegna per ogni modulo un testo vuoto **su rettangolo giallo**, cioè
  caselle vuote sparse sul disegno;
- `raisedata`, entrando nel tab dei dati, richiama `ShowNamesfilt $c $c2 4`
  (mette in moto la lettura dei valori) invece di `ShowNames $c2 4`, che
  darebbe le stesse caselle vuote.

**Anche in `draw2gr.tcl`** (menu `.menu.vmgr`) *Show Value* è la **prima voce**
e il **modo di partenza** (`set showon 4`): la pagina è un'interfaccia
operatore, si apre per vedere i valori. In coda allo startup, dopo
`topRead`/`loadF01`, il modo si applica da solo — `ShowNamesfilt $c $c 4` se
vale 4, `ShowNames $c $showon` altrimenti — dove prima c'era un `ShowNames $c 2`
fisso. Le altre voci del menu (*Graf sequential/circular*, *Find name*, *Zoom*,
*Set Sim path*, *Units...*) sono rimaste dov'erano.

**Zoom con Ctrl+rotella** (un passo per scatto nei livelli del menu *Zoom*):
attenzione, su Linux/X11 la rotella arriva come **`Button-4`/`Button-5`**, e Tk
8.6 su X11 `<MouseWheel>` non lo genera affatto. Serve quindi la coppia di bind
`<Control-Button-4>`/`<Control-Button-5>`, con `<Control-MouseWheel>` tenuta
solo per Windows: draw2gr aveva la sola `<Control-MouseWheel>` e il Ctrl+rotella
non faceva nulla (`d2g_zoomWheel` in draw2gr.tcl, `addcanvas` in legopc.tix).

**Gotcha indici menu**: le proc `raisetopol`/`raisedata`/`raisetaskconf` fanno
`entryconfigure` sul menu View (e sul File menu, vedi *Delete Model*); **tutte** le voci sono indirizzate **per label**
(`"Links..."`, `"Info..."`, `"Show OFF"`, `"Set Sim path"`, `"Units..."`, …)
perché gli indici numerici cambiano tra piattaforme e a ogni voce aggiunta o
spostata (l'inserimento di *Units...* aveva rotto `entryconfigure 4` → errore
`unknown option "-state"` sul separatore; il riordino ha spostato *Links...*
dall'indice 0 al 7).

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
