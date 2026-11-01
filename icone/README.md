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

- **`config`, `xstaz` e `mmi`** (programmi C con Motif): ogni loro finestra
  ha la sua icona sulla barra delle applicazioni, `config/`, `xstaz/` e
  `mmi/`, a 16, 32 e 48 pixel. Per `xstaz` vuol dire la finestrella con
  *Quit* e le pagine dei faceplate; per `mmi` la finestra principale, le
  pagine sinottiche, le pagine delle stazioni (`topLevelShellStaz.c`: le apre
  la MMI stessa, non xstaz, quindi hanno l'icona della MMI) e i dialoghi.

  Le barre di oggi (WSLg, GNOME, KDE) leggono la proprietà `_NET_WM_ICON`,
  pixel ARGB a colori: la vecchia bitmap a un bit `XmNiconPixmap` (in
  `topLevelShellMain.c` e `topLevelShellStaz.c`) è rimasta, ma non la usa
  nessuno. La scrive `IconaFinestreApp`
  ([iconafinestra.c](../AlgLib/libutilx/iconafinestra.c), in `libutilx`),
  chiamata una volta nel `main` dopo `XtAppInitialize`: si aggancia al
  *changeHook* di Xt e mette l'icona su ogni shell che il programma realizza,
  senza toccare i punti in cui le finestre vengono create. I pixel sono
  **compilati dentro il programma** (`AlgLib/libutilx/icona_<nome>.c`): non
  si legge niente all'avvio, e non servono `$LEGOROOT`, libpng o libXpm. Se
  cambia un PNG, vanno rigenerati e ricompilati (vedi sotto).

Sotto WSLg l'icona si vede sulla **barra di Windows** e in Alt+Tab, non a
sinistra nella barra del titolo: quel quadratino lo disegna la cornice di
WSLg (Weston 9), ed è lo stesso per tutte le finestre X11. Su Linux
installato sulla macchina dipende dal window manager (KDE la mostra anche
nel titolo, GNOME no).

Non ancora usati: gli XPM. `net_startup` non ha una finestra sua, e non ha
una voce nel menu perché lancia `killsim`.

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

Per `config`, `xstaz` e `mmi` i PNG da 16, 32 e 48 pixel finiscono anche
**dentro gli eseguibili**. Dopo averli rigenerati, dalla radice del
repository:

```sh
python3 icone/icone_c.py            # riscrive AlgLib/libutilx/icona_{config,xstaz,mmi}.c
source .profile_legoroot
(cd AlgLib/libutilx && make -f Makefile.mk)
(cd Alg_mmi/config && make -f Makefile.mk)        # config (e compreg)
(cd Alg_mmi/run_time && make -f Makefile.mk)      # mmi
(cd Alg_rt/grafica/xstaz && make -f Makefile.mk)  # xstaz
```

`icone_c.py` è Python puro (legge i PNG con `zlib`): non servono
ImageMagick o PIL. Con un nome (`python3 icone/icone_c.py mmi`) rifà solo
quello. I makefile non ricollegano un eseguibile quando cambia una libreria:
se `make` dice che non c'è niente da fare, cancella l'eseguibile e rilancialo.

Per controllare, senza aprire finestre sullo schermo: con un `Xvfb` avviato,
`xprop -id <finestra> _NET_WM_ICON` deve elencare `Icon (16 x 16)`,
`(32 x 32)` e `(48 x 48)` e disegnarle.

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
