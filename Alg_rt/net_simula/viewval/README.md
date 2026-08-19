# `viewval` — lettura e scrittura dei punti in memoria condivisa

`viewval` legge (e forza) i valori delle variabili nel DB punti di una
simulazione LegoPST attiva. È il motore dietro *View → Show Value* di legopc
(che lo lancia in modalità server) ed è utilizzabile a mano da terminale.

Sorgente: [main_viewval.c](main_viewval.c); l'accesso alla SHM è in
[viewshr.c](viewshr.c). Il binario si costruisce con `make -f Makefile.mk` e
finisce in `Alg_rt/bin/`; lo stesso makefile produce `viewval_legacy` (versione
precedente, senza selettore) e `umis`.

## Modi d'uso

| Comando | Cosa fa |
|---|---|
| `viewval TAG` | stampa valore e tempo, poi esce |
| `viewval TAG -t 1` | rilettura continua ogni secondo |
| `viewval TAG -f 10.1` | **scrive** 10.1 nel punto e termina |
| `viewval -i` | selettore a schermo pieno su tutte le variabili, poi tabella |
| `viewval -L file.val` | salta il selettore, legge i nomi dal file |
| `viewval -i -S file.val` | salva su file i nomi selezionati |
| `viewval -s [formato]` | modalità server: nomi su stdin, valori su stdout |
| `viewval -k <dt>` | output nel formato di KST |
| `viewval -l file.log` | registra ogni scrittura in SHM (vale per `-f` e per l'editing) |

Del file passato a `-L` viene usata **solo la prima colonna** (il tag): il resto
della riga è ignorato, quindi vanno bene sia i file prodotti da `-S` sia il
copia-incolla di listati con descrizione e unità.

## Modalità interattiva

**Selettore** (`-i` senza `-L`): frecce, `PgUp`/`PgDn`, `Home`/`End`, cifre
seguite da `g` per saltare a una riga, `/` per cercare con `n`/`N` per ripetere,
`Spazio` per selezionare/deselezionare, `Invio` per confermare, `q`/`ESC` per
uscire. La ricerca lavora sull'intera riga, quindi anche sulle descrizioni.

**Tabella** (`Variabile | Descrizione | Valore | Unita'`): i valori sono
mostrati **convertiti** nelle unità di misura correnti (`uni_mis`, file
`uni_misc.cfg` per-simulazione). `viewval` esce da solo se il simulatore va in
STOP o in ERRORE.

| Tasto | Azione |
|---|---|
| ↑ ↓ / PgUp PgDn / Home End | sposta il cursore (la finestra segue, altezza presa dal terminale) |
| `f` o `Invio` | apre la modifica del valore sulla riga selezionata |
| `i` | torna al selettore |
| `q` | esce |

**Modifica di un valore.** Con `f` il cursore si porta nella cella del valore,
che diventa un campo vuoto (il valore attuale resta scritto nel prompt in
basso); l'aggiornamento della tabella è **sospeso** finché si digita, così le
righe non si muovono sotto le dita. Si accettano cifre e `+ - . e E`,
`Backspace` corregge, **`Invio` scrive in memoria condivisa**, **`ESC` annulla**.
Con il campo lasciato vuoto o con un testo non numerico non viene scritto nulla.

Il valore si digita nelle **unità visualizzate** e viene riconvertito in unità
interne con l'inversa della tabella `uni_mis` (`interno = (visuale - B) / A`);
la riga di stato riporta entrambi. Attenzione: `-f` invece **non** converte,
lavora direttamente in unità interne.

Sotto c'è la stessa primitiva usata da `-f`: `viewshr(PUTVAR)` →
`RtDbPPutValue()`, cioè un float depositato nel DB punti, senza semafori e
senza coordinamento con lo scheduler.

## Se il valore forzato "non resta"

Scrivere ha effetto duraturo solo sui punti che il modello **non ricalcola**
(gli ingressi). Su una variabile calcolata il passo di integrazione successivo
ci passa sopra e il valore torna quello del modello: `viewval` se ne accorge e
lo dice in riga di stato invece di lasciar credere a una scrittura fallita.

## Trappole

- **`viewval` cerca `net_sked` con `ps -ao ucomm`**, che elenca solo i processi
  legati a un terminale: una simulazione avviata senza tty (`nohup`/`setsid` da
  uno script, come fa la FMU) risulta invisibile e viewval risponde
  *"Simulazione non attiva! manca net_sked"* anche se il simulatore sta girando.
- La modalità interattiva **richiede un terminale vero**: con stdin rediretto
  esce subito (guardia su EOF).
- Il file di log di `-l` è aperto in *append* e riceve un'intestazione di
  sessione a ogni avvio.
