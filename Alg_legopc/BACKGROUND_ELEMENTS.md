# Elementi grafici di background in legopc

Guida pratica per **creare** e usare elementi grafici di *background* negli schemi
di un modello LegoPST (`legopc.tix`).

## Cos'è un elemento di background

Un'**icona/disegno decorativo**: un'immagine statica posizionata sul canvas che
**decora** lo schema ma **non fa parte della topologia** del modello.

- **Non** ha porte, **non** genera F01, **non** è animabile.
- Viene salvato nel `.tom` (per ricaricare lo schema) ma **escluso** dal `.top`
  (che alimenta `pag2f01` → F01).
- Lo **zoom lo scala** insieme allo schema; la **rotazione è disabilitata**.

È il "terzo tipo" della famiglia non-topologica, accanto agli elementi della
libreria `remark`: testo statico (`@com_0`, *Add Text*) e display dinamico
(`@val_0`, *Add Display*).

---

## Ricetta: creare un nuovo elemento di background

Esempio di riferimento già presente: **`@alb_0`** in
`$LG_LIBRARIES/background/`.

> **Il punto chiave**: tutti gli elementi di background condividono **un solo**
> script — `$LG_TIX/bgelement.tcl` (generico, basato su `$idclass`) — che vive
> tra i file della UI, **non** tra le risorse grafiche. Per aggiungere un nuovo
> decoro **basta mettere la sua GIF** nella libreria: nessun `.tcl`. Vedi
> §"Uno script per tutti" per il perché.

### A) Preparare la libreria — una volta sola

Gli elementi di background vivono in una **libreria dedicata** sotto
`$LG_LIBRARIES`, separati dai moduli. Serve **una tantum** un solo file, il
marker `.lib`:

```bash
mkdir -p "$LG_LIBRARIES/background"
# marker .lib: rende la libreria selezionabile nel browser (contenuto irrilevante)
cp "$LG_LIBRARIES/remark/OldLibH20.lib" "$LG_LIBRARIES/background/background.lib"
```

Un file `*.lib` qualsiasi nella directory basta: il browser di legopc
(`Open library`) elenca le dir che contengono un `.lib`. **Nessuno script
`.tcl`** va messo nella libreria: quello condiviso è già `$LG_TIX/bgelement.tcl`.

### B) Aggiungere un decoro — per ogni nuovo elemento

**Un solo file**: la GIF.

- Formato **GIF**.
- Nome file: **`@<nome>_0n.gif`** — la `@` iniziale e il suffisso **`n`** sono
  obbligatori (vedi §Convenzioni). Es. `@alb_0n.gif`.
- Serve **solo** l'orientamento `n` (nord): niente `e/s/w`.

```bash
cp mio_disegno.gif "$LG_LIBRARIES/background/@logo_0n.gif"
```

Fatto: `@logo_0` compare nel browser e si istanzia dallo `_default.tcl` condiviso.

> Suggerimento qualità: lo zoom rimpicciolisce meglio di quanto ingrandisca
> (scaling a interi, nearest-neighbor). Disegna la GIF a risoluzione un po'
> generosa e lasciala ridurre.

### C) (Opzionale) Descrizione nel browser

Crea `$LG_HELP/@<nome>_0.tch`: riga 1 = titolo, **riga 2** = descrizione breve
mostrata nella status-bar della palette quando selezioni l'icona.

### D) Verifica

Apri legopc, poi *Open library* → `background.lib`: l'icona deve comparire nella
palette. Selezionala e fai **Ctrl+Left** sul canvas per inserirla.

## Uno script per tutti: `$LG_TIX/bgelement.tcl`

Lo script è **unico per tutte le librerie** e vive tra i file della UI
(`Alg_legopc/src/tix/bgelement.tcl`, deployato in `$LG_TIX`), **non** tra le
risorse grafiche. È **generico** perché non nomina mai un elemento specifico:
crea l'immagine con `-image $idclass$GIForient`, dove `$idclass` è la classe
dell'elemento (es. `@alb_0`) impostata dal chiamante. Quindi lo stesso script
produce l'immagine giusta per **qualunque** decoro.

Quando istanzia un elemento, legopc (helper `elementScript`, usato da `itemAdd`,
`itemAddFromfile`, `topRead`):

1. usa `<lib>/<nome>.tcl` **se esiste** (moduli veri e `@com_0`/`@val_0` ce
   l'hanno);
2. altrimenti, **solo se il nome inizia con `@`**, usa `$LG_TIX/bgelement.tcl`.

Il **gate `@`** è la rete di sicurezza: un **modulo vero** (nome senza `@`) con
il `.tcl` mancante **non** ripiega — `source` dà errore come prima, così un bug
di libreria resta visibile. Solo gli elementi non-topologici (`@…`) senza script
dedicato usano il condiviso.

Vuoi un decoro con comportamento speciale? Dagli il suo `@<nome>_0.tcl` dedicato
nella libreria: avrà la precedenza sul condiviso.

Template di `bgelement.tcl` (per riferimento; è già in `$LG_TIX`):

```tcl
	set mymodId [$c create image $x $y -image $idclass$GIForient]

	$c addtag id$mymodId     withtag $mymodId
	$c addtag module         withtag $mymodId
	$c addtag $idclass.cls   withtag $mymodId
	$c addtag $GIForient.ori withtag $mymodId
	$c addtag remarkdescr    withtag $mymodId
	if {$fromfile == "yes"} {
		$c addtag $mlpath.lpath   withtag $mymodId
	} else {
		$c addtag $curLibPath.lpath withtag $mymodId
	}
	if {$fromfile == "yes"} then {set progName $ff_progNumb} else {inputModName $c $x $y}
	$c addtag $progName.name withtag $mymodId
	incr progNumb
	$c addtag bgimage withtag $mymodId
```

Cosa NON toccare nel template, e perché:

| Riga | Perché è così |
|---|---|
| `$c create image ... -image $idclass$GIForient` | crea un'**immagine** (non testo). `$idclass$GIForient` = `@<nome>_0` + `n` = nome del photo GIF caricato dal browser |
| ordine dei `addtag` 0..6 | vari punti del codice usano **indici posizionali fissi** (cls=2, ori=3, lpath=5) |
| `remarkdescr` | blocca rinomina e rotazione, e instrada il save/load nel ramo non-topologico |
| `module` | fa sì che lo **zoom** scali l'immagine (loop `find withtag module` in `doZoom`) |
| `bgimage` per **ultimo** | tag distintivo che separa i BG dal testo/display; deve stare all'indice 7 per non spostare gli altri |

---

## Convenzioni di nome (importanti)

- **`@` iniziale — OBBLIGATORIA.** È ciò che **esclude l'elemento dalla
  topologia**: `writeFiles` (fileio.tcl) salta dal `.top`/F01 ogni elemento il
  cui nome-classe contiene `@` (`set nonmodulo [regexp {@} ...]`). Senza `@`
  l'elemento finirebbe erroneamente in F01. *Non è il tag `remarkdescr` a
  escludere dalla topologia, ma la `@`.*
- **Suffisso `_0`** — convenzione moduli (variante 0). Consigliato per coerenza.
- **Evita nomi che finiscono per `n`** prima dell'orientamento (es. `@fann_0`):
  il browser ricava il nome-tool con `string trimright` togliendo la `n`, e
  toglierebbe anche quella del nome. `@alb_0`, `@logo_0`, `@cornice_0` sono ok.

## Cosa NON serve (a differenza dei moduli veri)

| File | Moduli | Background |
|---|---|---|
| `.pi3` (struttura porte) | sì | **no** |
| `.pi4` (matematica) / `.i5` | sì | **no** |
| gif `e/s/w` (orientamenti) | sì | **no** (orientamento singolo) |
| `.tch` (help) | opz. | opz. |

I file `.pi3`/`.pi4`/`.i5` servono all'authoring di un modulo (porte, `i32i5`,
`pag2f01`): un decoro non ha porte e non passa mai da quella pipeline.

## Come si usa nello schema

- **Inserimento**: *Open library* → `background.lib` → seleziona icona →
  **Ctrl+Left** sul canvas (come un modulo qualsiasi).
- **Z-order**: l'elemento resta nell'ordine di inserimento; usa *Put Bottom* /
  *Put Top* dal menu per mandarlo dietro/davanti ai moduli.
- **Zoom**: scala automaticamente con lo schema.
- **Rotazione**: disabilitata (messaggio *"This is a Text..."*); l'orientamento
  è unico.
- **Doppio-click**: nessun effetto (non ha testo da modificare).
- **Copia/incolla**: supportato (l'immagine viene ricreata).

## Persistenza `.tom` (per curiosità/diagnostica)

Un BG viene salvato nel `.tom` con un blocco minimale (nessun testo/font):

```
# --- pass 1 ---
@alb_0            ← classe (.cls)
n                 ← orientamento
ALB1              ← nome istanza
120.0 340.0       ← coordinate
<path libreria>
...
****
# --- pass 2 ---
@alb_0
ALB1
++++              ← nessuna riga testo/font
****
```

In lettura (`topRead`) l'immagine è ricreata dal `.tcl` nel primo passaggio; il
secondo passaggio riconosce il tag `bgimage` e consuma il blocco fino a `++++`
senza applicare testo/font.

## Dettaglio implementativo (dove tocca il codice)

Modifiche in `Alg_legopc/src/tix/` (deployate in `bin` con `make -f makefile`),
tutte condizionate al tag **`bgimage`**:

| File / punto | Ruolo |
|---|---|
| `legopc.tix` — `elementScript` | sceglie `<nome>.tcl` o, se manca ed è un nome `@…`, `$LG_TIX/bgelement.tcl` (usato da `itemAdd`/`itemAddFromfile`/`topRead`) |
| `src/tix/bgelement.tcl` | script condiviso che crea l'item `image` con i tag + `bgimage` (deployato in `$LG_TIX`) |
| `fileio.tcl` — `writeFiles` (save `.tom`) | non scrive testo/font per i BG |
| `fileio.tcl` — `topRead` (load `.tom`) | per i BG consuma il blocco fino a `++++` |
| `legopc.tix` — `IncollaItem` (paste) | non legge `-text` sui BG |
| `legopc.tix` — `modifica_remark` (doppio-click) | esce subito sui BG |

Non serve toccare zoom (ereditato dal tag `module`), esclusione topologia
(già via `@`), blocco rinomina/rotazione (già via `remarkdescr`).

---

Vedi anche la sezione *"Libreria `background`"* in [CLAUDE.md](../CLAUDE.md) per
il contesto architetturale, e la libreria `remark` per gli elementi testo
(`@com_0`) e display (`@val_0`).
