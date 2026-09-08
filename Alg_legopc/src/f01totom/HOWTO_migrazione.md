# HOWTO — migrare una task legocad in una task con `.tom`

Procedura per portare una task **legocad** d'epoca — quella la cui topologia
grafica sta in `macroblocks.dat` — a una task **legopc.tix**, che disegna a
partire da un file `.tom` e si apre con `lgpc`.

Vale per i pochi modelli vecchi che ha senso recuperare. Non è un passo della
build: è un'operazione manuale, da fare una volta per modello, il cui risultato
si rifinisce comunque in `lgpc`.

Per il perché delle scelte e per la diagnosi dei difetti dello strumento, vedi
[README.md](README.md). Qui c'è solo la sequenza da eseguire.

---

## Quello che bisogna sapere prima di cominciare

**La conversione non può essere fedele al 100%, e non è un difetto del
convertitore.** legocad collega **variabile su variabile**, il `.tom` collega
**porta con porta**: il primo è più espressivo, il secondo più rigido ma più
fisico. Una porta del `.tom` si lega a un solo altro blocco e il legame vale per
tutte le sue variabili, mentre il `f01.dat` d'epoca può prendere la pressione da
un blocco e la temperatura da un altro sulla stessa porta.

Su GTS, delle 165 connessioni in ingresso: 83 esprimibili in un `.tom` (e lo
strumento ne ricostruisce 81), 53 su porte con ingressi da blocchi diversi, 29
su variabili che il modulo di libreria oggi non ha più. Quindi:

- una parte delle connessioni va **ridisegnata a mano in `lgpc`**;
- i due `f01.dat` restano diversi, quindi il `f14.dat` d'epoca non si adatta al
  modello ricostruito e **il travaso dei dati è un passo obbligato**, non un
  ripiego.

Aspettarsi il contrario porta a credere che qualcosa sia rotto quando non lo è.

---

## Passo 0 — Ambiente

```sh
cd ~/LegoPST           # la radice del repo, dove sta il profilo
source .profile_legoroot
```

Dopo il `source`, `$LG_BASE` punta ad `Alg_legopc` (non alla radice del repo) e
`$LG_TOOLS` alla directory degli eseguibili.

Serve una `libgraph` nella radice utente (`$LG_ENTRY`, cioè `$LG_MODELS`):
`connect.dat`, `files_i5`, `help`, `libraries`, `pixmaps`, `xbm`. Le
installazioni legocad d'epoca non l'hanno. Verifica:

```sh
ls $LG_FILESI5 | wc -l          # devono essere centinaia di .i5
ls $LG_LIBRARIES                # le librerie dei moduli
```

Se è vuoto, clonane una (vedi la fine del [README.md](README.md) per il
confronto fra le candidate).

Compila lo strumento, se serve:

```sh
cd $LG_BASE/src/f01totom && make -f makefile
```

---

## Passo 1 — Convertire la topologia

Nella directory della task d'epoca servono `f01.dat` e `macroblocks.dat`.
**Non lavorare in quella directory**: conviene una copia di lavoro, così
l'originale resta intatto.

```sh
mkdir -p /tmp/migra && cd /tmp/migra
cp $LG_MODELS/GTS/f01.dat $LG_MODELS/GTS/macroblocks.dat .
$LG_TOOLS/f01totom
```

Non fa domande: le istanze dei moduli le sceglie da sé, preferendo quelle
provviste di elemento grafico, e rispetta le scelte già registrate in
`f01totom.inp`. Con **`-i`** le chiede una per una e salva le risposte in
`f01totom.inp`, che si può anche modificare a mano per imporre altre istanze.

Il `macroblocks.dat` viene cercato **accanto al `f01.dat`**, quindi va bene
anche indicare il file per percorso, da qualunque directory:

```sh
$LG_TOOLS/f01totom -o GTS_conv.tom $LG_MODELS/GTS/f01.dat
```

Le opzioni utili: `-o <file.tom>` per il nome dell'uscita, `-i` per scegliere le
istanze a mano, `-noremark` per non convertire le annotazioni, `-h` per
l'aiuto. `-a` è accettato per compatibilità e non fa niente (una volta serviva a
*non* essere interattivi, che ora è il comportamento normale).

Escono `f01totom.tom` (o quello indicato con `-o`) e `f01totom.inp`.

Codici di uscita: **0** tutto convertito, **1** convertito ma con blocchi
esclusi, **2** errore d'uso.

**Leggi il riepilogo finale.** Dice quanti blocchi sono stati convertiti, quanti
esclusi e perché, quante posizioni recuperate, quante porte collegate e quante
lasciate libere perché non esprimibili. È lì che si vede cosa aspettarsi.

Un blocco viene **escluso** quando il suo modulo non è in libreria o non ha un
`.i5`: quei blocchi non finiscono nel `.tom` e vanno aggiunti a mano in `lgpc`.
Su GTS sono 5 su 49 (`FCTT`, quattro `MITN`).

Le **annotazioni di testo** del disegno d'epoca (i record `*REMARK*`) vengono
convertite in elementi `@com_0` della libreria `remark`: su LPS sono 95, su IPS
59. I `*SYMBOL*` e i `*GLINES*` — simboli e polilinee decorative — restano
fuori: il riepilogo dice quanti erano, così si sa cosa manca.

---

## Passo 2 — Verificare il `.tom` prima di aprirlo

```sh
bash $LG_BASE/src/f01totom/verifica_tom.sh f01totom.tom
```

Controlla la struttura e che per ogni blocco esistano i file che `topRead`
andrà a cercare (`$LG_LIBRARIES/<libreria>/<classe>.tcl` e `<classe>n.gif`).
Se qui è OK, il file si apre.

Quanto è stata buona la ricostruzione delle connessioni:

```sh
python3 $LG_BASE/src/f01totom/misura_connessioni.py \
        $LG_MODELS/GTS/f01.dat f01totom.tom f01totom.inp
```

---

## Passo 3 — Installare la task

Due strade, e la scelta cambia tutto.

### 3a. Solo il disegno, sopra un modello che gira

Il `.tom` è autosufficiente per la grafica: `topRead` legge soltanto quello.
Basta metterlo accanto alla task originale, col nome della task:

```sh
cp f01totom.tom $LG_MODELS/GTS/GTS.tom
```

Si apre lo schema sopra un modello che funziona già, con il suo `f14.dat`
intatto. **Non premere Build**: la ricostruzione rigenera `f01.dat` dal `.tom` e
sostituisce l'originale con la versione ricostruita, che ha meno connessioni.

È la strada per *guardare* il modello d'epoca in `lgpc`, o per usarlo come
sfondo di un'animazione.

### 3b. Task convertita, da ricostruire

La task convertita **è la task originale più il `.tom`**: si parte da una task
che gira, non da una directory vuota.

```sh
cp -a $LG_MODELS/GTS $LG_MODELS/GTS_conv
cp f01totom.tom $LG_MODELS/GTS_conv/GTS_conv.tom
cp f01totom.inp $LG_MODELS/GTS_conv/            # per rifare la conversione
```

> **Il nome della directory e quello del `.tom` devono coincidere.** `topRead`
> risolve il modello come `$LG_MODELS/<nome>/<nome>.tom`. Un `.tom` fuori da
> `$LG_MODELS`, o con un nome diverso dalla directory, fa uscire
> *"TopRead: 1 - File ... not found"*, che sembra un difetto del convertito e
> non lo è.

---

## Passo 4 — Aprire in `lgpc`

```sh
export LG_TIX=$LG_BIN
wish $LG_TIX/legopc.tix $LG_MODELS/GTS_conv/GTS_conv.tom
```

(È quello che fa l'alias `lgpc`, che però non prende argomenti: da lì il modello
si apre con File → Open.)

Cosa guardare:

- le icone sono dove erano in legocad? Le coordinate vengono da
  `macroblocks.dat` e sono riportate tali e quali;
- i blocchi che il convertitore non ha potuto posizionare stanno nella **zona di
  raccolta in basso**, in fila: trascinali al loro posto;
- i blocchi esclusi (moduli assenti) **non ci sono**: vanno aggiunti dalla
  libreria e collegati;
- le annotazioni di testo ci sono, come elementi di tipo remark: si spostano e
  si modificano come qualunque altro elemento;
- le connessioni mancanti vanno disegnate. Il riepilogo del Passo 1 dice quali
  porte sono state lasciate libere e perché.

Quando il disegno è a posto, **salva**: è il salvataggio che genera il `.top`,
da cui la catena ricostruisce il modello.

---

## Passo 5 — Ricostruire il modello

Dalla GUI, con la funzione di build di `lgpc` (che passa per `cad_crealg1` →
`pag2f01` e produce `f01.dat`, `foraus.f`, `proc/` e un `f14.dat` **vuoto**).

A questo punto il modello è coerente col disegno, ma senza dati.

---

## Passo 6 — Travasare i dati iniziali e i parametri

```sh
bash $LG_BASE/src/f01totom/travasa_f14.sh \
     $LG_MODELS/GTS_conv $LG_MODELS/GTS/f14.dat
```

Usa `edi14` (`src/main_lego/edi14.for`, lo strumento storico di LegoPST per
questo, lo stesso che `legopc` chiama in `autoedi14`), che travasa **per nome**
condizioni iniziali, variabili di ingresso e dati fisici dei blocchi. Aggiunge
la riga dei dati di normalizzazione (`P0`, `H0`, `W0`, `T0`, `R0`, `L0`, `V0`,
`DP0`), che `edi14` da solo lascia in bianco.

Su GTS recupera 411 valori su 548. Lo script stampa il conteggio prima e dopo,
tiene `f14.dat.pre_travaso` come copia di sicurezza e lascia in
`edi14_travaso.log` l'elenco di cosa non ha trovato.

> **Da rifare dopo ogni ricostruzione.** La build rigenera `f14.dat`, e la riga
> di normalizzazione torna vuota. `legopc` ha un travaso automatico
> (`autoedi14`), ma gli serve un `proc/f14.dat` che non sempre c'è: dopo aver
> ricostruito, controlla il `f14.dat` e se è vuoto rilancia lo script.

---

## Passo 7 — Completare a mano

Quello che resta in bianco nel `f14.dat` sono due famiglie sole:

- **variabili di moduli cambiati**: il modulo di libreria di oggi non ha più
  quella variabile. Esempio: `NODO` chiamava la sua uscita `ASOM`/`QSOM`, oggi
  si chiama `USOM`; l'`ATTU` d'epoca aveva quattro canali (`TV_1..TV_4`,
  `TS_1..TS_4`), quello di oggi ne ha uno (`AS_1`, `AV_1`);
- **ingressi diventati liberi** perché la connessione non è stata ricostruita:
  se ridisegni la connessione in `lgpc` e ricostruisci, il valore non serve più.

Per avere l'elenco di cosa manca basta cercare gli slot vuoti:

```sh
awk 'substr($0,15,10) ~ /^ *$/ && /^ *[0-9]+ /' $LG_MODELS/GTS_conv/f14.dat
```

---

## Riepilogo della catena

```
task legocad                     f01.dat + macroblocks.dat
      |
      |  1.  f01totom -a                        -> f01totom.tom + .inp
      |  2.  verifica_tom.sh / misura_connessioni.py
      |
      +-- 3a. .tom accanto alla task originale  -> si guarda, NON si ricostruisce
      |
      +-- 3b. cp -a task task_conv + .tom
              4.  lgpc: sistema il disegno e salva  -> .top
              5.  lgpc: build                       -> f01.dat, foraus.f, f14.dat vuoto
              6.  travasa_f14.sh                    -> f14.dat con i dati d'epoca
              7.  a mano: blocchi esclusi, connessioni non esprimibili, valori residui
```

## Trappole, in ordine di quanto fanno perdere tempo

1. **`.tom` fuori da `$LG_MODELS` o con nome diverso dalla directory** →
   *"File not found"* all'apertura. Non è un difetto del convertito.
2. **`libgraph` assente nella radice utente** → nessun modulo trovato, la
   conversione esclude tutto. Le installazioni legocad d'epoca non l'hanno.
3. **Ricostruire dopo aver messo il `.tom` accanto alla task originale** (3a) →
   il `f01.dat` d'epoca viene sostituito da quello ricostruito, con meno
   connessioni. Fare una copia prima.
4. **Dimenticare il travaso dopo una ricostruzione** → modello senza condizioni
   iniziali, non parte.
5. **Aspettarsi che le connessioni tornino tutte** → non è possibile, vedi
   l'introduzione.
