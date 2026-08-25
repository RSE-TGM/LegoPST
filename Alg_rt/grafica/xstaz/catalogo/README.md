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
