# Icone di LegoPST

Un'icona per ciascuno dei cinque programmi principali, disegnate come una
famiglia: stesso riquadro, stesso tratto, simboli presi dagli schemi
d'impianto (tubi, valvole, turbine, strumenti). Ogni programma ha il suo
colore, per riconoscerlo a colpo d'occhio nella barra delle applicazioni.

| Cartella | Programma | Colore | Disegno |
|---|---|---|---|
| `lghmi/` | `lghmi`, il selettore da cui si lancia la simulazione e si aprono le modifiche | blu `#1F4E79` | serbatoio, valvola e turbina chiusi in un ciclo |
| `lgpc/` | `lgpc` / legopc, i modelli di processo | verde `#2E7D5B` | moduli collegati da tubi, con le porte |
| `config/` | `config`, i modelli di regolazione | arancio `#B85C1E` | anello di retroazione: nodo somma, blocco, ritorno |
| `net_startup/` | `net_startup`, l'avvio della simulazione | rosso `#A93434` | il tasto di avvio inserito in una condotta |
| `xstaz/` | `xstaz`, i faceplate di comando | viola `#5B4B8A` | pannello con due barre di livello e due tasti |

## I file

In ogni cartella, `<nome>` è il nome della cartella:

| File | A cosa serve |
|---|---|
| `<nome>.svg` | il disegno originale, 64×64: **è la fonte**, gli altri si ricavano da lui |
| `<nome>_16.png` … `<nome>_256.png` | 16, 24, 32, 48, 64, 128 e 256 pixel, per finestre e menu |
| `<nome>_32.xpm`, `<nome>_48.xpm` | per i programmi Motif (`config`, `xstaz`), che usano pixmap X11 |

## Chi le usa

- **`lghmi`** ([lghmi.tcl](../Alg_legopc/src/tix/lghmi.tcl), `imposta_icona`):
  l'icona delle sue finestre è `lghmi/`, o `xstaz/` con `-staz`.
- **legopc** ([legopc.tix](../Alg_legopc/src/tix/legopc.tix)): l'icona delle sue
  finestre è `lgpc/`.
- **`lgicone`** ([lgicone.sh](../util97/procedure/lgicone.sh)): le voci del menu
  delle applicazioni (sotto WSL il menu Start di Windows) usano
  `lghmi_256.png`, `lgpc_256.png` e `xstaz_256.png`. Guida:
  [docs/COMANDI_LG.md](../docs/COMANDI_LG.md), *Le icone e il menu delle
  applicazioni*.

`lghmi` e legopc caricano i PNG da 16, 32, 48, 64 e 128 pixel, e se la
cartella manca vanno avanti senza icona.

Non usati ancora: le icone di `config` e `net_startup`, e gli XPM. `config` e
`xstaz` impostano già un'icona nel codice C (`XmNiconPixmap`, in
`Alg_mmi/run_time/topLevelShellMain.c` e `topLevelShellStaz.c`): per usare
quelle di qui bisogna cambiare il codice e ricompilare. `net_startup` non ha
una finestra sua, e non ha una voce nel menu perché lancia `killsim`.

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
  colore del programma;
- disegno bianco, tratto `stroke-width="3"` (2,5 per i dettagli piccoli), estremi
  e angoli arrotondati, senza riempimento, tranne le valvole e i simboli pieni;
- simboli degli schemi d'impianto: la valvola è la farfalla piena (due triangoli
  uniti al vertice), la turbina un trapezio che si allarga verso l'uscita;
- niente testo: a 16 pixel non si legge. Prima di adottarla, guardarla a 16 e a
  32 pixel.
