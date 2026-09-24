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
| In alto a sinistra | **Pagine** del file: si clicca per passare da una all'altra, col tasto destro si rinomina o si elimina |
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

Quelle caselle grigie fanno anche da **righelli**: portano scritto il numero
della colonna (sotto) e della riga (a sinistra), così la `POSIZIONE` di una
cella si legge invece di contarla. L'angolo in cui le due strisce si
incrociano non ha numero, perché non è né una colonna né una riga valida. Si
spengono da `Visualizza → Numeri di riga e colonna`, acceso all'avvio.

## Vedere le stazioni

Di default ogni stazione è disegnata con la **sua immagine reale**, ritagliata
da catture di `xstaz` (vedi *Le immagini* più sotto): la pagina in costruzione
somiglia a quella che vedrà l'operatore.

L'immagine è il *campione del catalogo* — mostra la forma vera del tipo, non i
contenuti di quella stazione — e sopra ci vengono scritti i **parametri di
questa stazione**: tutte le `ETICHETTA`, cioè il titolo della pagina, le
descrizioni delle stazioni e le scritte dei pulsanti, dei selettori e degli
impostatori. Si vedono man mano che si configurano: cambiata una stringa nel
pannello delle proprietà, compare nel disegno.

Le posizioni non sono stimate: vengono dalla stessa tabella `new_staz[]` che
usa `xstaz` (vedi *Da dove vengono le posizioni*), e i testi sono scritti con
gli stessi font X. Confrontando con le catture reali, i 54 testi del catalogo
cadono entro **2 pixel** in orizzontale e **4** sulla linea di base.

Restano generici i parametri che l'immagine porta con sé e che non sono testo:
i **colori** dei LED e delle spie e le **scale** degli indicatori (`0`/`50`/
`100`, che nella pagina vera vengono dal `MINMAX` della stazione). Quelli che
dipendono dalla simulazione in corso — valori, stati accesi/spenti, posizione
dell'indice — non ci sono per definizione: quelli si vedono solo in `xstaz`
con una simulazione viva.

Il menu **`Visualizza → Immagini reali di xstaz`** spegne le immagini e torna
allo **schema** a forme semplici, utile per vedere a colpo d'occhio quali
oggetti compongono una stazione.

Le 13 stazioni **storiche** (`SA1`, `SP1`, `AM1`, …) non hanno immagine e
compaiono come blocchi opachi: si possono spostare e cancellare, non
modificare — il catalogo dei tipi non le copre.

## Costruire una pagina

**Piazzare.** Si sceglie un tipo dalla libreria: il cursore diventa un mirino e
la riga di stato mostra nome, **ingombro in celle** (`2x1`, `12x4`, …) e la
miniatura dell'elemento. Un clic sul canvas piazza la stazione *centrata sul
punto*. Il tipo resta armato — si piazzano più copie di seguito — finché non
si preme **`Esc`** o non si sceglie un altro tipo.

**Vedere un tipo prima di usarlo.** La miniatura della riga di stato è alta 31
pixel e serve a riconoscere il tipo, non a guardarlo. Tenendo premuto il
**tasto destro** su un nome nella libreria compare lo sprite a **grandezza
naturale** — quanto occuperà davvero sulla pagina — con nome e ingombro sotto;
si richiude appena si rilascia il tasto. La finestrella resta dentro lo
schermo anche per i tipi larghi (`SINCRONO` è 744 pixel).

**Selezionare.** Un clic su una stazione la seleziona. Premendo e trascinando
**sullo sfondo** del canvas si traccia un rettangolo e si selezionano tutte le
stazioni che tocca, come in `legopc`. `Ctrl+A` seleziona l'intera pagina, `Esc`
deseleziona. Ogni stazione selezionata è cerchiata da un riquadro tratteggiato.

Tutto quello che segue vale per **una stazione o per molte**: la selezione si
sposta, si copia, si incolla e si cancella come un blocco solo.

**Spostare.** Si trascina. Al rilascio le stazioni si agganciano alla griglia.
Trascinando una stazione che fa parte della selezione si muove **tutto il
gruppo**, dello stesso scostamento, quindi le posizioni relative si
conservano; nessuna può finire a coordinate negative (il limite si applica allo
scostamento, non alla singola stazione, altrimenti il gruppo si deformerebbe).
Se qualcosa finisce sopra un'altra stazione la riga di stato lo **segnala**, ma
non lo impedisce: `compstaz` non sembra vietarlo, e un divieto inventato qui
sarebbe un vincolo in più rispetto al formato vero.

**Cancellare.** `Canc` o `BackSpace`.

**Copiare e incollare.** `Ctrl+C` e `Ctrl+V` **sul canvas** (che prende il
fuoco al primo clic; nelle caselle di testo `Ctrl+C`/`Ctrl+V` restano copia e
incolla del testo). Si copia la stazione intera — tipo, descrizione e *tutti i
valori degli oggetti*: colori, riferimenti alle variabili, scalamenti — che è
poi il lavoro che si vuole evitare di rifare a mano su una stazione gemella.
Con più stazioni selezionate si copia il gruppo, e incollandolo le posizioni
relative restano quelle.

Della copia cambia solo ciò che la identifica nella pagina: id nuovo, e
**`NUMERO` azzerato** (un numero duplicato sarebbe un errore vero per
`compstaz`; vuoto, la stazione incollata si comporta come una appena
piazzata). Si incolla sotto il puntatore se è sul canvas, altrimenti una cella
in diagonale rispetto all'originale. Gli appunti restano: si incolla più
volte, anche **in un'altra pagina** del file.

**Annullare e rifare.** `Ctrl+Z` annulla l'ultima operazione, `Ctrl+Y` la
rifà. Sono coperte solo le operazioni **sulle stazioni**: piazzare, spostare,
incollare e cancellare — anche di gruppo, che si annullano in un colpo solo.

Le modifiche ai **parametri** di una stazione restano fuori, e non per
semplificare: la storia registra le *operazioni inverse*, non istantanee del
modello. Se salvasse istantanee, annullare uno spostamento riporterebbe
indietro anche i colori e le variabili cambiati nel frattempo — che è
esattamente ciò che non deve succedere. Per la stessa ragione, annullare una
cancellazione restituisce la stazione **con i suoi parametri** come erano.

Aprendo un altro file la storia si azzera: è la storia di *quel* modello.

Le stesse azioni stanno nel menu **`Edit`** e nel **popup del tasto destro**
sul canvas, con la scorciatoia scritta accanto, per non doverle ricordare a
memoria. Le due liste sono la stessa cosa: sono costruite da un unico elenco,
così non possono divergere.

Le voci che in quel momento non avrebbero effetto nascono **spente** invece di
sparire — *Copia* ed *Elimina* senza selezione, *Incolla* senza appunti — così
il menu non cambia forma sotto le dita. Il tasto destro su una stazione che
non è nella selezione la seleziona (si agisce su ciò che si indica); su una
già selezionata la selezione di gruppo resta intatta.

**Nuova pagina.** `File → Nuova pagina...` chiede numero, nome e descrizione,
con le stesse convalide del parser.

**Rinominare, duplicare ed eliminare una pagina.** Tasto destro sul suo nome
nell'elenco:

- *Rinomina* cambia **nome e descrizione**, con le stesse convalide (nome di
  al più 8 caratteri, senza spazi, descrizione obbligatoria). Il **numero non
  si tocca**: è quello che le stazioni citano in `PAGINA`, e cambiarlo le
  lascerebbe orfane.
- *Duplica* crea una pagina nuova **con tutte le sue stazioni**, nelle stesse
  posizioni e con gli stessi valori: è il modo di partire da una pagina che
  funziona invece che da un foglio bianco. Il numero proposto è il primo
  libero, e il **nome dev'essere diverso** da quelli già in uso — non è
  pignoleria: `stazpag` cerca la pagina per nome e si ferma alla **prima** che
  combacia, quindi due omonime ne renderebbero una irraggiungibile da riga di
  comando e dai bottoni faceplate degli schemi. Le stazioni copiate hanno id
  nuovi e `NUMERO` azzerato, come quelle incollate.
- *Elimina* chiede conferma, e se la pagina ha delle stazioni dice **quante ne
  spariscono con lei**. Le due cose vanno insieme per forza: una stazione che
  cita una pagina inesistente è un file che il parser rifiuta, quindi
  lasciarle orfane produrrebbe un `r01.dat` che `lgmkstaz` stesso non
  rileggerebbe. Finché non salvi, il file su disco è intatto.

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

L'elenco rispetta le regole di `checkvar.c`, che **non sono simmetriche**:

- per un `OUTPUT` si offrono i soli **ingressi liberi** (`INGRESSO_NC`), perché
  un ingresso già connesso dentro il modello non si può perturbare da fuori:
  `check_input` lo rifiuta e ferma la compilazione;
- per un `INPUT` si offre **tutto il modello**, con le uscite per prime. Il
  controllo di compstaz si chiama `check_output` ma non guarda il tipo: un
  `INPUT` che cita un ingresso compila e funziona — è normale, per esempio,
  far cambiare colore a un led in base a un ingresso.

Anche un nome scritto a mano viene verificato al salvataggio, con le stesse
regole: vuoto+vuoto va sempre bene (scollegato), un nome che comincia per
**`#`** pure (scollegato ma leggibile, per prepararlo prima che la variabile
esista), e `variabil` resta il segnaposto storico. Senza `variabili.edf` non
si verifica niente e si accetta tutto.

Il controllo è tarato su quello che compstaz accetta davvero, non su quello
che sembrerebbe sensato: rifiutare qui una pagina che compila e gira sarebbe
il difetto peggiore.

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

## Da dove vengono le posizioni dei parametri

`lgmkstaz_geometria.tcl` contiene, per ciascuno dei 54 tipi, la posizione in
pixel di ogni oggetto dentro la stazione, con il suo sottotipo e flag: sono le
stesse che `xstaz` passa ai propri disegnatori. È **generato** da
`new_staz[]`, non trascritto:

```sh
cd Alg_legopc/src/tix
tclsh genera_geometria.tcl > lgmkstaz_geometria.tcl
```

La generazione non è un vezzo: la tabella è lunga 888 righe, e la
trascrizione a mano del solo elenco dei tipi in `lgmkstaz_dati.tcl` aveva già
prodotto un errore (un `INDICATORE` di troppo in `SINCRONO`). Il test
`lgmkstaz_test.tcl` verifica che geometria e catalogo elenchino gli stessi
oggetti nello stesso ordine — è anche il controllo che segnala una geometria
non rigenerata dopo un cambio di `newstaz.h`.

Sulla verticale, `xstaz` mette i testi in `XmLabel`, che dal proprio `y`
scende di `marginHeight` (2) più l'*ascent* del font prima di appoggiarci la
linea di base. Gli ascent sono quelli dei font X veri di `xstaz` (`fixed` e
`-adobe-times-bold…25`), misurati sulle catture del catalogo: Tk non li può
dire, perché per quei nomi sostituisce font propri.

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

- Dei parametri della singola stazione si vedono i **testi**; colori e scale
  degli indicatori restano quelli generici dell'immagine.
- Le 13 stazioni storiche non sono modificabili.
- La sovrapposizione fra stazioni è segnalata, non impedita.
- Lo zoom non c'è: la vista è sempre a 62 pixel per cella.
