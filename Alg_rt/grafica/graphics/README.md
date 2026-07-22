# `graphics` — visualizzatore grafici (file circolare f22)

Applicazione Motif/X11 che visualizza gli andamenti temporali registrati da una
simulazione LegoPST nel **file circolare f22** (`f22circ.dat`). È il viewer che
apre `run_graphics.sh` nel bundle FMU e che la HMI `draw2gr` invoca su *Plot*.

## Uso da riga di comando

```
graphics [-scala] <file_f22> [var1 var2 ... varN]
graphics -h | --help
```

Per l'help sintetico: `graphics -h` (funziona anche senza `DISPLAY` né ambiente
LegoPST).

### Argomenti

| Argomento | Descrizione |
|---|---|
| `-scala` | Opzionale, **come primo argomento**. Usa una scala verticale **unica** condivisa da tutte le tracce, invece di una scala per traccia. |
| `<file_f22>` | Path del file circolare f22 **senza** estensione `.dat`: `graphics` la aggiunge (`f22_open_file` fa `"%s.dat"`). Es. `.../f22circ` apre `f22circ.dat`. |
| `var1 … varN` | Opzionali. Nomi di variabili da tracciare **subito** all'apertura. Senza, la finestra si apre vuota e le variabili si scelgono dal menu. |

**Limite argomenti**: al massimo 6 token dopo `graphics` (il codice rifiuta
`argc > 7`). Quindi con il solo file restano fino a **5** nomi di variabile, con
`-scala` + file fino a **4**.

### Esempi

```bash
graphics /home/antonio/sked/duetask/f22circ           # apre f22circ.dat, finestra vuota
graphics -scala /home/antonio/sked/duetask/f22circ    # scala verticale unica
graphics .../f22circ PORTATA PRESSIONE                # pre-traccia due variabili
```

## Ambiente

| Variabile | Ruolo |
|---|---|
| `DISPLAY` | Server X11 (obbligatoria per la GUI; non serve solo per `-h`). |
| `LEGORT_UID` | Directory dei file `.uid` Motif compilati. Dal profilo: `$LEGORT/uid`. Senza, `MrmOpenHierarchy` fallisce con *can't open hierarchy*. |
| `$HOME/defaults` | Directory dei default, creata da `chdefaults()` se manca. Contiene `f22_files.edf` (ultimi path usati) e i file delle unità di misura. |

Sorgiare `.profile_legoroot` imposta `LEGORT_UID` e il resto; lanciare `graphics`
da una shell senza profilo richiede almeno `DISPLAY` e `LEGORT_UID`.

## Note

- Il file f22 è **circolare**: `p_iniz`/`p_fine` nell'header delimitano la
  finestra di campioni valida; i messaggi `DEBUG: f22_leggo_header …` in avvio
  sono diagnostica normale, non errori.
- Alla chiusura (*Quit*) `graphics` salva i path correnti in
  `f22_files.edf` nella cwd (`close_path()`). Se la directory non contiene ancora
  quel file, viene creato.
