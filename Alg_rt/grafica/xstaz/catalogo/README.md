# Catalogo visivo dei tipi di stazione

`r01.dat` di sola consultazione: contiene **tutti i 54 tipi** di stazione, ognuno
con il proprio nome scritto **sotto**. Serve come aiuto alla configurazione —
si guarda che aspetto ha un tipo prima di usarlo in una pagina vera.

```sh
cd <una dir con variabili.rtf>      # p.es. la task di regolazione
cp <questa dir>/r01.dat .
compstaz                            # -> r02.dat
xstaz 1 &                           # oppure: lghmi -staz
stazpag                             # elenca le 8 pagine
stazpag LED                         # ne apre una
```

Le pagine sono divise per famiglia: `LED`, `LED2`, `PULS`, `PULS2`, `INDIC`,
`INDIC2`, `DISP`, `VARIE`. Tutti i riferimenti a variabili sono lasciati
scollegati (righe vuote), quindi **non serve alcun modello**: il catalogo
compila in qualsiasi task.

## Le pagine

| Pagina | Contenuto | Immagine |
|---|---|---|
| `LED` / `LED2` | 13 tipi di segnalazione a led | [LED](pag_LED.png), [LED2](pag_LED2.png) |
| `PULS` / `PULS2` | 18 pulsantiere con spie | [PULS](pag_PULS.png), [PULS2](pag_PULS2.png) |
| `INDIC` / `INDIC2` | barre, indicatori a indice, sincronoscopio | [INDIC](pag_INDIC.png), [INDIC2](pag_INDIC2.png) |
| `DISP` | display, impostatori numerici, testi | [DISP](pag_DISP.png) |
| `VARIE` | selettori, lampade, comandi elementari, mixer | [VARIE](pag_VARIE.png) |

Le immagini sono catture reali di `xstaz`, non disegni.

## Gli sprite dei tipi (`staz/`)

`staz/<TIPO>.png` contiene, per ciascuno dei 54 tipi, **l'immagine reale della
stazione** ritagliata dalle catture qui sopra. Li usa `lgmkstaz` per disegnare
le stazioni come appaiono davvero in `xstaz`, invece dei segnaposto
rettangolari. Ogni sprite misura esattamente `larg * 62` x `altezza * 62`
pixel, cioè occupa le stesse celle della stazione vera.

```sh
wish ritaglia_sprite.tcl            # -> staz/<TIPO>.png
wish ritaglia_sprite.tcl -prova     # dice cosa farebbe, senza scrivere
```

Lo sprite mostra il **campione del catalogo**, quindi con i contenuti generici
di questo `r01.dat`: etichette `-`, colori tutti gialli, scale degli
indicatori `0`/`50`/`100`. Le parti che nella pagina vera dipendono
dall'istanza (etichette, colori, `MINMAX`) non ci sono: vanno sovrapposte da
chi disegna, non cercate nell'immagine.

`ritaglia_sprite.tcl` non dà per buono nessun offset: ricava il bordo della
cattura dalla differenza fra le dimensioni del PNG e quelle del contenuto
descritto da `r01.dat`, e si ferma con un errore se i due assi non concordano
— cioè se la cattura non corrisponde più a questo `r01.dat`. Riusa il parser e
il catalogo dei tipi di `lgmkstaz` invece di rifarli, e non ha bisogno di
ImageMagick né di Pillow: Tk 8.6 legge e scrive PNG da solo.

## Rigenerarlo

`genera_catalogo.py` legge la tabella `new_staz[]` di
[newstaz.h](../../../../AlgLib/libinclude/newstaz.h), quindi si mantiene
allineato da solo se vengono aggiunti tipi di stazione:

```sh
python3 genera_catalogo.py > r01.dat
```

Due vincoli che il generatore rispetta e che vale la pena conoscere se si
ritocca la disposizione:

- una pagina non può superare **50x50 celle** (`MAX_CEL`); qui si sta entro
  28x14 così la finestra ci sta sullo schermo (una cella = `DIM_UNITSTAZ` = 62
  pixel);
- **l'asse Y è invertito** nel disegno (`cnewstaz.c`: `ydraw = height - ydraw -
  htot`): per far comparire il nome *sotto* al widget, l'etichetta va messa alla
  cella di y **minore**.

Se si rigenera `r01.dat` la catena va percorsa tutta, perché le immagini
restano indietro:

1. `python3 genera_catalogo.py > r01.dat`
2. **ricatturare** le pagine (`pag_*.png`): aprirle con `xstaz` e fotografare
   la finestra di ciascuna. Serve uno strumento di cattura (`import` di
   ImageMagick, come fa l'export PNG di `legopc`); su un display virtuale
   `Xvfb` le finestre non compaiono sullo schermo e la cattura è ripetibile.
3. `wish ritaglia_sprite.tcl` per rifare gli sprite.

Il passo 3 si accorge da solo se si è saltato il 2: il controllo sul bordo
fallisce, oppure un tipo nuovo resta senza sprite e viene elencato.
