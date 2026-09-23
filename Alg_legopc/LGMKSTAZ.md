# `lgmkstaz` — builder grafico delle pagine di faceplate (`r01.dat`)

`lgmkstaz` costruisce e modifica a video le pagine di faceplate di comando,
cioè il file **`r01.dat`** che `compstaz` compila in `r02.dat` e che `xstaz`
mostra all'operatore. Sostituisce la scrittura a mano di un formato rigido
(una parola chiave per riga, 54 tipi di stazione con ingombri diversi,
riferimenti alle variabili da azzeccare esattamente) con una vista a griglia
in cui le stazioni si piazzano, si spostano e si collegano al modello.

Per il **formato del file** e il catalogo dei tipi vedi
[HOWTO_faceplate.md](../Alg_rt/grafica/xstaz/HOWTO_faceplate.md); per
`compstaz`/`xstaz` vedi [il README di xstaz](../Alg_rt/grafica/xstaz/README.md).

## Come si lancia

```sh
lgmkstaz                 # r01.dat della directory corrente (come compstaz)
lgmkstaz <r01.dat>
lgmkstaz <directory>     # il r01.dat che sta lì dentro
lgmkstaz -h
```

Oppure da `lghmi`, voce **`Tools → lgmkstaz`** (accanto a *Edit model*: sono i
due editor — `legopc` disegna il modello, `lgmkstaz` le pagine di faceplate).
Lanciato da lì lavora sulla directory corrente del selettore, di solito quella
della task.

Alla prima apertura di un file in una sessione viene creata una copia
**`r01.dat.bak`**, che non viene più sovrascritta: è la rete di sicurezza
prima di qualunque modifica.

## La finestra

| Zona | Cosa c'è |
|---|---|
| In alto a sinistra | **Pagine** del file: si clicca per passare da una all'altra |
| In basso a sinistra | **Libreria (clic per piazzare)**: i 54 tipi di stazione, con una casella di ricerca |
| A destra | Il **canvas**: la pagina disegnata a griglia, una cella = 62 pixel come in `xstaz` |
| In basso | La **riga di stato**: informazioni sulla stazione sotto il mouse, o sul tipo armato |

### L'origine è in basso a sinistra

`POSIZIONE x y` si legge come in un piano cartesiano: **x cresce verso destra,
y verso l'alto**, e la cella di y minore sta in basso. Non è una scelta di
`lgmkstaz`: è come disegna `xstaz`
(`cnewstaz.c`, `ydraw = altezza_pagina*62 - posy*62 - altezza*62`).

Le coordinate negative non esistono, quindi il canvas le mostra come una
**striscia grigia** — una colonna a sinistra e una riga sotto l'origine — per
rendere visibile il confine invece di lasciare un'area bianca ambigua. La
vista si apre sempre sull'origine, con un margine di crescita verso l'alto e
verso destra.

## Vedere le stazioni

Di default ogni stazione è disegnata con la **sua immagine reale**, ritagliata
da catture di `xstaz` (vedi *Le immagini* più sotto): la pagina in costruzione
somiglia a quella che vedrà l'operatore.

L'immagine è però il *campione del catalogo*: mostra la forma vera del tipo,
non i contenuti di quella stazione. Etichette, colori e scale degli indicatori
restano quelli generici (`-`, giallo, `0`/`50`/`100`).

Per questo il menu **`Visualizza → Immagini reali di xstaz`** permette di
spegnerle e tornare allo **schema** a forme semplici, che invece scrive le
`ETICHETTA` vere della stazione. I due modi sono complementari: l'immagine per
la forma e l'ingombro, lo schema per il contenuto.

Le 13 stazioni **storiche** (`SA1`, `SP1`, `AM1`, …) non hanno immagine e
compaiono come blocchi opachi: si possono spostare e cancellare, non
modificare — il catalogo dei tipi non le copre.

## Costruire una pagina

**Piazzare.** Si sceglie un tipo dalla libreria: il cursore diventa un mirino e
la riga di stato mostra nome, **ingombro in celle** (`2x1`, `12x4`, …) e la
miniatura dell'elemento. Un clic sul canvas piazza la stazione *centrata sul
punto*. Il tipo resta armato — si piazzano più copie di seguito — finché non
si preme **`Esc`** o non si sceglie un altro tipo.

**Spostare.** Si trascina. Al rilascio la stazione si aggancia alla griglia. Se
finisce sopra un'altra la riga di stato lo **segnala**, ma non lo impedisce:
`compstaz` non sembra vietarlo, e un divieto inventato qui sarebbe un vincolo
in più rispetto al formato vero.

**Cancellare.** `Canc` o `BackSpace` sulla stazione selezionata.

**Copiare e incollare.** `Ctrl+C` e `Ctrl+V` **sul canvas** (che prende il
fuoco al primo clic; nelle caselle di testo `Ctrl+C`/`Ctrl+V` restano copia e
incolla del testo). Si copia la stazione intera — tipo, descrizione e *tutti i
valori degli oggetti*: colori, riferimenti alle variabili, scalamenti — che è
poi il lavoro che si vuole evitare di rifare a mano su una stazione gemella.

Della copia cambia solo ciò che la identifica nella pagina: id nuovo, e
**`NUMERO` azzerato** (un numero duplicato sarebbe un errore vero per
`compstaz`; vuoto, la stazione incollata si comporta come una appena
piazzata). Si incolla sotto il puntatore se è sul canvas, altrimenti una cella
in diagonale rispetto all'originale. Gli appunti restano: si incolla più
volte, anche **in un'altra pagina** del file.

**Nuova pagina.** `File → Nuova pagina...` chiede numero, nome e descrizione,
con le stesse convalide del parser.

## Le proprietà di una stazione

Doppio clic su una stazione apre il pannello delle proprietà. Non è scritto a
mano per ognuno dei 54 tipi: è **generato dalla grammatica** dei 12 oggetti
elementari, quindi un colore diventa un menu, un riferimento due caselle
(variabile e modello) con l'eventuale negazione, un `OUTPUT` anche il modo di
perturbazione e il valore, e così via.

### Il collegamento al modello

Se accanto al `r01.dat` c'è un **`variabili.edf`** — il dump ASCII che
`compstaz` scrive insieme a `variabili.rtf` — viene caricato all'apertura, e
ogni campo `INPUT`/`INPUT_ERR`/`INPUT_BLINK`/`INIBIZIONE`/`OUTPUT` ha un
pulsante **`...`** che apre l'elenco filtrabile delle variabili vere del
modello (`VAR MODELLO descrizione`; si scrive per filtrare, doppio clic per
confermare).

L'elenco rispetta le regole di `checkvar.c`: per un `INPUT` si offrono le
**uscite** (tipo 0), per un `OUTPUT` gli **ingressi liberi** (tipo 1) — un
ingresso già connesso non si può perturbare da fuori.

Anche un nome scritto a mano viene verificato al salvataggio, con le stesse
regole: vuoto+vuoto va sempre bene (scollegato), un nome che comincia per
**`#`** pure (scollegato ma leggibile, per prepararlo prima che la variabile
esista), e `variabil` resta il segnaposto storico. Senza `variabili.edf` non
si verifica niente e si accetta tutto.

## `Verifica → Compila e verifica...`

Lancia **`compstaz` per davvero** sul modello che hai sotto mano, anche non
salvato, e mostra il suo esito e il suo log. È la prova che il file compila,
non una simulazione delle sue regole.

Come lo fa, e perché così:

- lavora su una **copia scratch** in una directory temporanea, mai
  sull'originale (ci copia il `variabili.rtf` che trova accanto al file);
- gira con una **chiave SHM/IPC isolata** (`50000000 + pid`), lontana dal banco
  operatore (`uid*10000`), perché `compstaz` tocca la memoria condivisa in
  ogni caso — anche quando `variabili.rtf` esiste già;
- all'uscita rimuove **solo** quella chiave (shared memory, le 11 code di
  messaggi e il semaforo che `msg_create_fam` crea), con `ipcrm` mirato. Mai
  `killsim`, che su Linux cancellerebbe tutte le SHM dell'utente;
- **chiede conferma ogni volta**, non solo la prima;
- se la compilazione fallisce la directory scratch **non** viene cancellata e
  il suo percorso è scritto in finestra, così si può guardarci dentro.

### `Anteprima con xstaz`

Se la compilazione è riuscita, la finestra di esito offre un selettore di
pagina e il pulsante **`Anteprima con xstaz`**: apre la pagina scelta nel vero
`xstaz`, sulla copia appena compilata. È l'unico modo di vedere il risultato
esatto, immagini dell'editor comprese.

Il pulsante **`Chiudi`** è consapevole dell'anteprima: se `xstaz` è ancora
aperto su quella directory si rifiuta di chiudere e ripulire, per non
cancellare i file sotto una finestra che li sta ancora leggendo.

## Le immagini delle stazioni

Stanno in [`Alg_rt/grafica/xstaz/catalogo/staz/`](../Alg_rt/grafica/xstaz/catalogo/staz/),
un PNG per tipo, e sono ritagliate dalle **catture reali** delle pagine di
catalogo: non sono disegni. `lgmkstaz` le cerca da `LEGOROOT`, poi risalendo
le directory — non ne tiene una copia propria.

Se mancano (o per un tipo che non ne ha) si ripiega sullo schema a forme
semplici, senza errori.

Si rigenerano con
[`ritaglia_sprite.tcl`](../Alg_rt/grafica/xstaz/catalogo/README.md), che serve
solo se cambia la tabella `new_staz[]` di `newstaz.h`; il README lì accanto
spiega la catena completa (rigenerare il `r01.dat` di catalogo, ricatturare le
pagine, ritagliare).

## Salvare

`File → Salva` e `Salva con nome...` riscrivono il `r01.dat` con lo stesso
formato che il parser legge — il ciclo legge/scrive è coperto da un test di
regressione su tutti i `r01.dat` reali del parco macchine
(`lgmkstaz_test.tcl`). Prima di scartare modifiche non salvate (apertura di un
altro file, `Aggiorna`, uscita) viene chiesta conferma.

Dopo aver modificato un `r01.dat` di un simulatore vero ricordati che la
configurazione va riallineata: `kCompStaz`, o `kUpSim` per rifare tutto (vedi
[BUILD.md](../docs/BUILD.md)).

## Il menu `?`

Apre la documentazione nel browser, con lo stesso meccanismo del `?` di
[`lghmi`](LGHMI.md) — condiviso in `openhelp.tcl`: i `.md` passano prima dal
convertitore `md2html.tcl` (Tcl puro, niente da installare), perché un browser
da solo mostrerebbe il sorgente Markdown.

| Voce | Documento |
|---|---|
| **lgmkstaz: how to use it** | questa guida |
| Command faceplates: the r01.dat format | [HOWTO_faceplate.md](../Alg_rt/grafica/xstaz/HOWTO_faceplate.md) |
| Station catalogue and sprites | [catalogo/README.md](../Alg_rt/grafica/xstaz/catalogo/README.md) |
| xstaz and compstaz | [README di xstaz](../Alg_rt/grafica/xstaz/README.md) |
| LegoPST - project overview | [README.md](../README.md) |
| Annotated documentation index | [DOCUMENTATION_INDEX.html](../DOCUMENTATION_INDEX.html) |

Una voce che punta a un file assente nasce **spenta** invece di sparire: si
vede che il documento è previsto e che manca.

## Variabili d'ambiente

| Variabile | A cosa serve |
|---|---|
| `LG_TIX` | dove sta `lgmkstaz.tcl` (la imposta il profilo) |
| `LEGOROOT` | da cui si trovano `compstaz` e le immagini delle stazioni; senza, si risale dalle directory |

## Limiti noti

- Le immagini mostrano il campione del catalogo: etichette, colori e scale
  della singola stazione non ci sono (c'è lo schema per quelli).
- Le 13 stazioni storiche non sono modificabili.
- La sovrapposizione fra stazioni è segnalata, non impedita.
- Lo zoom non c'è: la vista è sempre a 62 pixel per cella.
