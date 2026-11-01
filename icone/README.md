# Icone di LegoPST

Un'icona per ciascuno dei sei programmi principali, disegnate come una
famiglia (la famiglia A, «Impianto»): in tutte lo **stesso serbatoio a
sinistra e lo stesso tubo di ritorno in basso**, e cambia solo l'elemento
sulla linea, che dice cosa fa il programma. Ogni programma ha il suo colore,
per riconoscerlo a colpo d'occhio nella barra delle applicazioni.

| Cartella | Programma | Colore | Sulla linea |
|---|---|---|---|
| `lghmi/` | `lghmi`, il selettore da cui si lancia la simulazione e si aprono le modifiche | blu `#1F4E79` | valvola e turbina: l'impianto intero |
| `lgpc/` | `lgpc` / legopc, i modelli di processo | verde `#2E7D5B` | valvola e matita al posto della turbina: il processo si disegna |
| `config/` | `config`, i modelli di regolazione | arancio `#B85C1E` | regolatore sulla valvola, con il segnale misurato a valle (tratteggiato) |
| `net_startup/` | `net_startup`, l'avvio della simulazione | rosso `#A93434` | il tasto di avvio dentro il ciclo |
| `xstaz/` | `xstaz`, i faceplate di comando | viola `#5B4B8A` | serbatoio con il livello, valvola e faceplate con i tasti |
| `mmi/` | `mmi`, le pagine sinottiche di supervisione | verde acqua `#1D6E78` | un occhio: si guarda l'impianto |

## I file

In ogni cartella, `<nome>` è il nome della cartella:

| File | A cosa serve |
|---|---|
| `<nome>.svg` | il disegno originale, 64×64: **è la fonte**, gli altri si ricavano da lui |
| `<nome>_16.png` … `<nome>_256.png` | 16, 24, 32, 48, 64, 128 e 256 pixel, per finestre e menu |
| `<nome>_32.xpm`, `<nome>_48.xpm` | per i programmi Motif (`config`, `xstaz`), che usano pixmap X11 |

## Chi le usa

- **`lghmi`** ([lghmi.tcl](../Alg_legopc/src/tix/lghmi.tcl)): l'icona delle sue
  finestre è `lghmi/`, o `xstaz/` con `-staz` (`imposta_icona`). Dentro la
  finestra, i programmi che lancia hanno la loro icona dove li lancia
  (`icona_programma`, 16 e 24 pixel): `net_startup` sul pulsante *start sim*,
  `mmi` sul pulsante *mmi*,
  `lgpc` sul titolo *Process pages*, `config` sul titolo *Regulation tasks* e su *Edit regulation* del tasto
  destro, `xstaz` sul titolo *xstaz faceplates* e sul suo *Open page*.
- **legopc** ([legopc.tix](../Alg_legopc/src/tix/legopc.tix)): l'icona delle sue
  finestre è `lgpc/`.
- **`lgicone`** ([lgicone.sh](../util97/procedure/lgicone.sh)): le voci del menu
  delle applicazioni (sotto WSL il menu Start di Windows) usano
  `lghmi_256.png`, `lgpc_256.png` e `xstaz_256.png`. Guida:
  [docs/COMANDI_LG.md](../docs/COMANDI_LG.md), *Le icone e il menu delle
  applicazioni*.

Per le finestre `lghmi` e legopc caricano i PNG da 16, 32, 48, 64 e 128 pixel.
Se la cartella manca vanno avanti senza icone.

Non ancora usati: gli XPM, e le icone sulle **finestre** di `config`, `xstaz`
e `mmi`.
Sono programmi C con Motif, e mettere un'icona a colori sulla barra delle
applicazioni vuol dire scrivere `_NET_WM_ICON` dal codice e ricompilare (oggi
la finestra principale della MMI e le pagine dei faceplate impostano solo una
vecchia bitmap a un bit, `XmNiconPixmap`, in
`Alg_mmi/run_time/topLevelShellMain.c` e `topLevelShellStaz.c`; `config` non
ne imposta nessuna). `net_startup` non ha una finestra sua, e non ha una voce
nel menu perché lancia `killsim`.

## Cambiare un'icona

Si modifica l'SVG e si rigenerano gli altri file da lui. Con ImageMagick, dalla
cartella dell'icona:

```sh
n=lghmi
for s in 16 24 32 48 64 128 256; do
  magick -background none -density 1536 $n.svg -resize ${s}x$s ${n}_$s.png
done
for s in 32 48; do
  magick -background none -density 1536 $n.svg -resize ${s}x$s ${n}_$s.xpm
done
```

`-density 1536` fa disegnare l'SVG già grande (64 × 24 = 1536 pixel), e il
ridimensionamento lo riduce: senza, i PNG grandi vengono sfocati. Dopo la
modifica non serve rilanciare `lgicone`: le voci puntano ai file, non li
copiano.

Per un'icona nuova, le regole che tengono insieme la famiglia:

- `viewBox="0 0 64 64"`; riquadro `x=2 y=2`, 60×60, angoli `rx=13`, pieno del
  colore del programma, diverso da quelli già usati;
- il serbatoio `<rect x="10" y="16" width="12" height="24" rx="3">` e il
  ritorno che parte dal suo fondo (`M16 40 V48 H…`) restano uguali; cambia
  l'elemento sulla linea, che il ritorno raggiunge da sotto;
- disegno bianco, tratto `stroke-width="3"` (2,5 per i dettagli piccoli), estremi
  e angoli arrotondati, senza riempimento, tranne le valvole e i simboli pieni;
- simboli degli schemi d'impianto: la valvola è la farfalla piena (due triangoli
  uniti al vertice), la turbina un trapezio che si allarga verso l'uscita;
- niente testo: a 16 pixel non si legge. Prima di adottarla, guardarla a 16 e a
  32 pixel.
