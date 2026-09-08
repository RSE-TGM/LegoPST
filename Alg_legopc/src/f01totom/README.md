# f01totom — da legocad a legopc.tix

Converte la topologia di una task **legocad** d'epoca nel formato **`.tom`** di
`legopc.tix` (l'alias `lgpc`), per non ridisegnare a mano i pochi modelli vecchi
che vale ancora la pena recuperare.

Non è uno strumento della catena di build: è un convertitore una-tantum, da
lanciare a mano, il cui risultato si finisce comunque in `lgpc`. Per questo non
è integrato in `legopc.tix` e non deve esserlo.

## I tre file in gioco

### `f01.dat` (input) — la topologia funzionale
Prodotto dalla catena legocad. Contiene, per ogni blocco, il modulo, il nome,
la descrizione e l'elenco delle variabili con la provenienza delle connessioni
(`--IN--`, `<===`). È la fonte per **cosa** c'è nel modello e **come è
connesso**.

### `macroblocks.dat` (input) — il disegno
Sta accanto al `f01.dat`. Dopo cinque righe di intestazione
(`****` / nome modello / `****` / descrizione / `****`), ogni riga è un record
la cui **prima colonna è il tipo, non la pagina**:

| Tipo | Formato | Significato |
|---|---|---|
| `0` | `0 <MOD><BLO> <MOD> <variante> <x> <y> 0` | istanza di blocco: la chiave è modulo+blocco concatenati (`BRTT`+`GMB1` → `BRTTGMB1`) |
| `1` | `1 *REMARK* <x> <y> <testo>` | annotazione di testo |
| `2` | `2 *SYMBOL* <id> <x> <y>` | simbolo grafico decorativo |
| `3` | `3 *GLINES* <parametri>` | polilinea decorativa |

I record **non sono raggruppati per tipo**: blocchi, remark e simboli sono
intercalati. C'è una sola pagina; il concetto di pagina in questo formato non
esiste.

Le coordinate sono in unità direttamente utilizzabili come pixel di tavolozza,
ma **l'estensione varia molto** da modello a modello (vedi la tabella sotto):
non si può assumere una tavolozza fissa.

### `.tom` (output) — il disegno di legopc
Lo legge `topRead` in [../tix/fileio.tcl](../tix/fileio.tcl). Struttura:
intestazione, dimensioni tavolozza, poi per ogni blocco cinque righe
(nome `.i5` senza estensione, orientamento `n`/`s`, nome blocco, `x.0 y.0`,
path libreria), `****`, poi per ogni blocco le porte
(`portN` + `busy portM BLOC` oppure `free`) chiuse da `++++`, e `****` finale.

Del path libreria `topRead` usa **solo il basename**, ricostruendo
`$LG_LIBRARIES/<nome>`: scrivere il nome secco della libreria va bene.

Gli elementi `@com_0` del `.tom` sono esattamente l'equivalente dei `*REMARK*`
di legocad — la strada per non perdere le annotazioni c'è.

## Stato

### Prima della revisione (misurato l'8 settembre 2026)

Provato su tutte le task di `/home/antonio/legocad`. Nessuna arrivava a
produrre un `.tom` apribile:

| task | blocchi | posizioni recuperate | esito |
|---|---|---|---|
| `provldch` | 1 | 1 | **ciclo infinito** |
| `STS` | 14 | 11 | `.tom` prodotto, 40 errori sulle porte |
| `GTS` | 49 | 49 | **abortiva**: `FCTT`, `MITN` assenti |
| `VCT` | 86 | 18 | abortiva |
| `HPS` | 103 | 22 | abortiva |
| `IPS` | 111 | 22 | abortiva |
| `LPS` | 134 | 12 | **segmentation fault** |

### Dopo la Fase 1

Tutte convertono, e i `.tom` prodotti **si aprono in `lgpc`** (verificato
lanciando `wish $LG_TIX/legopc.tix <file>.tom` su ognuno: nessun errore Tcl):

| task | blocchi | nel `.tom` | posizioni | porte connesse | esclusi |
|---|---|---|---|---|---|
| `provldch` | 1 | 1 | 1 | 0 | — |
| `STS` | 14 | 14 | 11 | 6 | — |
| `GTS` | 49 | 44 | 49 | 46 | `FCTT`, `MITN` |
| `HPS` | 103 | 102 | 22 | 70 | `CLAV` |
| `IPS` | 111 | 110 | 22 | 68 | `CLAV` |
| `LPS` | 134 | 128 | 12 | 100 | `AXCH CLAV VACT WARD` |
| `VCT` | 86 | 0 | 18 | 0 | `VACT` (tutti i blocchi) |

Le posizioni restano quelle di prima: **è la Fase 2 a sbloccarle**, e si vede
bene su LPS (12 su 134). Le porte connesse sono *scese* rispetto al primo
tentativo (GTS da 80 a 46) perché le connessioni che non si riescono a
risolvere fino in fondo ora diventano porte libere invece di righe malformate
che impedivano al file di aprirsi.

### Estensione delle coordinate

Smentisce la tavolozza fissa 1200×800 che il programma usava:

| task | x | y |
|---|---|---|
| `GTS` | 35–950 | 25–570 |
| `STS` | 75–870 | 25–690 |
| `VCT` | 4–1068 | 22–773 |
| `HPS` | 4–**1640** | 21–**1565** |
| `IPS` | 41–**1729** | 33–1393 |
| `LPS` | 27–**2191** | 21–1333 |

### Moduli che non si possono convertire

Richiesti dalle task legocad e senza `.i5` in nessuna installazione:
`axch`, `clav`, `fctt`, `mitn`, `vact`, `ward`. Non sono tutti nella stessa
condizione:

| modulo | cosa esiste | cosa manca |
|---|---|---|
| `fctt` | `Control/fctt_0.pi3` + `fctt_0n.gif` | il `.pi4`, da cui `i32i5` genera il `.i5` |
| `vact` | quattro gif (`vact_paral`, `vact_serie`, `vact_single`, `vact_tre_vie`) | `.pi4` e `.pi3` |
| `axch`, `clav`, `mitn`, `ward` | nulla | tutto |

**Nessuno ha un `.pi4` in nessuna installazione.** `fctt` e `vact` sono quindi
in parte recuperabili (c'è la grafica e, per `fctt`, l'interfaccia vecchia), ma
resta lavoro di modellazione, non di conversione.

### Attrezzi del banco

- `prova.sh` — converte le task e riporta blocchi, posizioni, porte, mancanti,
  esito. È la misura di ogni fase.
- `verifica_tom.sh` — dice se un `.tom` è leggibile da `topRead` **senza
  aprire la GUI**: controlla la struttura e che per ogni blocco esistano
  `$LG_LIBRARIES/<libreria>/<classe>.tcl` e `<classe>n.gif`.


## I difetti, in ordine di gravità

> I numeri di riga citati sono quelli del codice **prima** della revisione
> (commit di partenza): servono a ritrovare il punto nella storia, non nel
> file di oggi.

**1. [RISOLTO in Fase 1] Si pianta in un ciclo infinito.** [f01totom.c:534](f01totom.c#L534)

```c
while (buff[0] != 't') fgets(buff,MAXL,fpi5);
```

Nessun controllo di EOF: quando la sezione porte del `.i5` non ha tanti record
`t<n>` quanti dice il contatore, `fgets` restituisce NULL, `buff` resta com'è e
il ciclo gira per sempre. Su GTS succede leggendo `catt_0.i5`.

**2. [RISOLTO in Fase 1] Un modulo mancante annulla tutta la conversione.**
[f01totom.c:271](f01totom.c#L271) — `essit("Fine del programma\n")`. Su GTS due
blocchi su 49 (`FCTT`, `MITN`) impediscono di produrre qualunque cosa, mentre
gli altri 47 sarebbero convertibili.

**3. [aperto] La prima colonna di `macroblocks.dat` è letta come numero di pagina.**
[f01totom.c:801](f01totom.c#L801) e seguenti: il ciclo dei blocchi termina al
primo record con prima colonna diversa da `0`, e poi salta al prossimo `****`,
cioè alla fine del file. **Tutti i blocchi che seguono il primo `*REMARK*` o
`*SYMBOL*` perdono la posizione.** È l'origine delle icone sovrapposte: su LPS
122 blocchi su 134, su IPS 89 su 111.

**4. [RISOLTO in Fase 1] Le posizioni non recuperate non sono nemmeno azzerate.**
[f01totom.c:141](f01totom.c#L141) usa `malloc` e `posx`/`posy`/`pag` vengono
scritti solo se la ricerca in `macroblocks.dat` va a buon fine. Con `pag`=0 il
calcolo `posy + dimy*(pag-1)` porta le icone a `y = -800`, **fuori tavolozza**:
in `lgpc` non si vedono affatto. Verificato sui 3 blocchi persi di STS.

**5. [RISOLTO in Fase 1] Tavolozza di dimensione fissa** ([f01totom.c:593](f01totom.c#L593),
`dimx=1200, dimy=800`) e `totpag` sbagliato di uno
([f01totom.c:835](f01totom.c#L835): parte da 1 e ci somma le pagine contate).
Su STS la tavolozza esce 1200×1600 invece di quanto serve; su HPS e LPS le
coordinate reali arrivano oltre 1600 e 2100 e vengono tagliate.

**6. [RISOLTO in Fase 1] Il ripiego automatico impila le icone.**
[f01totom.c:626](f01totom.c#L626): `if(posy>=dimy-step) posy=dimy-step;`
satura l'ordinata, così dal 67° blocco in poi tutto finisce sulla stessa riga.

**7. [aperto] `macroblocks.dat` è cercato nella directory corrente.**
[f01totom.c:786](f01totom.c#L786) fa `fopen("macroblocks.dat", ...)` senza
path, mentre il `f01.dat` può essere indicato con un percorso qualsiasi:
convertire da un'altra directory perde silenziosamente tutte le posizioni.

**8. [RISOLTO in Fase 1] Tipo di porta preso da un indice fuori posto.**
[f01totom.c:580](f01totom.c#L580): quando la variabile della porta non viene
trovata fra quelle del blocco, `ii` vale `numvar` e il tipo viene letto oltre
le variabili valide. Ne derivano porte marcate `busy`/`free` a caso — parte
degli "errori" nel disegno convertito.

**9. [aperto] Funziona solo in modalità flat.** [f01totom.c:284](f01totom.c#L284) cerca
i `.i5` solo in `$LG_FILESI5`. Nelle installazioni migrate a modalità libreria
(vedi [../tix/migrate_i5.tcl](../tix/migrate_i5.tcl)), dove i `.i5` stanno
accanto ai `.pi4`, non trova niente. `pag2f01` gestisce entrambe le modalità,
questo no.

**10. [RISOLTO in Fase 1] Sciatterie di memoria.** [f01totom.c:222](f01totom.c#L222):
`malloc(strlen(x))` senza `+1` per il terminatore, su ogni nome di libreria.
Diverse `sscanf("%s")` senza limite di larghezza su buffer da 2, 3 e 5 byte.
`conta` in `cercai5` può essere usata non inizializzata se `glob` fallisce con
un codice diverso da `GLOB_NOMATCH`.

**11. [aperto] Deriva di versione fra i moduli.** I 40 errori `ERR3` su STS non sono un
bug del programma: i `.i5` di oggi dichiarano variabili di porta (`WVAL`,
`RPM1`, `HCOL`, `WMIX`) che i blocchi del `f01.dat` d'epoca non hanno. Va
deciso *cosa fare*, non *come ripararlo*.

**12. [RISOLTO in Fase 1] Segmentation fault oltre i 100 blocchi.** `MAXBLO`
valeva 100 e nessun ciclo lo controllava: LPS (134 blocchi) e IPS (111)
scrivevano oltre la fine degli array di `HEADF01`. Portato a 500, con
controllo e messaggio se anche quello non basta.

**13. [RISOLTO in Fase 1] La libreria veniva scelta guardando un file
qualsiasi.** La ricerca usava il glob `<modulo>*.*`, così bastava un `.gif`
orfano per vincere: `Airgas` contiene un `mixn_0n.gif` mentre l'elemento vero
sta in `LibH20`, e il `.tom` che punta ad `Airgas` non si apre. Ora si cerca
`<istanza>.tcl`, che è il file che `topRead` sorgia davvero. Per lo stesso
motivo la scelta dell'istanza `.i5` preferisce quelle provviste di elemento
grafico: `ldch` ha `ldch_0..ldch_3` come `.i5` ma solo `ldch_2` e `ldch_3`
hanno il `.tcl`, e prendere la prima in ordine alfabetico dava un file
inservibile.

**14. [RISOLTO in Fase 1] Connessioni malformate che impedivano l'apertura.**
Quando la porta remota non veniva individuata, `idbloconn` restava vuoto e la
riga usciva come `busy por GMG1`: `topRead` chiama `ffconnect`, che non trova
nessuna porta con tag `por` e muore con *can't read "ff2current"*. Era **il**
motivo per cui il file convertito non si apriva. Ora quelle porte restano
libere e vengono contate nel riepilogo, da ricollegare in `lgpc`.

## Piano di revisione

L'obiettivo è modesto e preciso: **dato un modello legocad, ottenere un `.tom`
che si apra in `lgpc` e assomigli al disegno originale**, con un rapporto
onesto di quello che non si è potuto convertire. Non serve altro.

### Fase 0 — Banco di prova ✔ FATTA

`/home/antonio/legocad` è l'installazione legocad d'epoca e **non ha
`libgraph`**: le variabili `LG_FILESI5`/`LG_LIBRARIES` puntano nel vuoto e
qualunque prova fallisce prima di cominciare. Va clonato un `libgraph`
completo (vedi la sezione finale).

Script `prova.sh` che converte `provldch`, `STS`, `GTS` e `LPS` e per ognuno
riporta: blocchi totali, posizioni recuperate, porte connesse, moduli mancanti,
esito. Serve come rete per tutte le fasi successive.

### Fase 1 — Non morire ✔ FATTA (difetti 1, 2, 4, 5, 6, 8, 10, 12, 13, 14)

Il minimo per cui lo strumento produca sempre qualcosa:

- controllo di EOF nel ciclo delle porte;
- modulo mancante → blocco segnaposto e nota nel rapporto, non `essit`;
- `calloc` al posto di `malloc` per `BLOF01`, e `posx`/`posy`/`pag`
  inizializzati a un valore riconoscibile;
- `+1` sulle `malloc` di stringa, larghezze nelle `sscanf`.

Verifica: GTS arriva in fondo e produce un `.tom` con 47 blocchi buoni e 2
segnaposto; nessuna task va in loop.

### Fase 2 — Leggere `macroblocks.dat` per quello che è (difetti 3, 7)

È la fase che risolve il problema che rende oggi il risultato inservibile.

- riconoscere i quattro tipi di record e leggere **tutti** quelli di tipo `0`,
  qualunque cosa ci sia intercalato;
- eliminare la finta logica delle pagine e l'off-by-one di `totpag`;
- cercare `macroblocks.dat` accanto al `f01.dat`, non nella directory corrente;
- chiamare la lettura **una volta** (oggi avviene sia in `main` che in
  `scrivi_tom`);
- riportare quante posizioni sono state recuperate su quante attese.

Verifica: LPS passa da 12 a 134 posizioni recuperate, IPS da 22 a 111.

### Fase 3 — Tavolozza e ripiego (difetti 5, 6)

- dimensioni della tavolozza calcolate dall'estensione reale delle coordinate,
  con un margine;
- ripiego automatico a griglia vera, dimensionata sul numero di blocchi, senza
  saturazione;
- i blocchi senza posizione **non** vanno fuori tavolozza: si mettono in una
  zona di raccolta a lato, così in `lgpc` si vedono e si trascinano al loro
  posto.

Verifica visiva in `lgpc`: aprire il `.tom` di GTS e confrontarlo con il
disegno legocad originale. È anche l'occasione per stabilire se l'ordinata va
capovolta — legocad e il canvas Tk potrebbero avere origini opposte, e una
sola occhiata lo dice.

### Fase 4 — Porte e connessioni (difetti 8, 11)

- indice del tipo di porta corretto;
- politica dichiarata per le variabili non abbinate: porta lasciata `free` e
  registrata nel rapporto, **mai** una connessione inventata;
- capire, su STS che è coperta al 100%, se la deriva `WVAL`/`RPM1`/`HCOL`/`WMIX`
  sia sistematica (rinomino di convenzione, rimediabile con una tabella di
  corrispondenza) o vera divergenza di modello (e allora è il rapporto a dover
  dire all'utente quali porte ricollegare a mano).

### Fase 5 — Usabilità come strumento a sé (difetto 9)

- riga di comando esplicita: file di ingresso, `-o` per l'uscita, opzioni per
  libreria e `.i5`, non interattivo per default;
- ricerca dei `.i5` in entrambe le modalità, flat e libreria, come fa
  `pag2f01`;
- rapporto finale conclusivo: quanti blocchi, quante posizioni, quante porte
  connesse, cosa manca, cosa va rifinito a mano;
- codici di uscita sensati.

### Fase 6 — Facoltativa: i `*REMARK*`

Tradurre le annotazioni di testo in elementi `@com_0`. È la differenza fra uno
schema anonimo e un disegno leggibile: LPS ne ha 95, IPS 59. I `*SYMBOL*` e i
`*GLINES*` invece conviene dichiararli fuori perimetro e dirlo nel rapporto.

## Fuori perimetro

- Integrazione con `legopc.tix`: resta uno strumento a riga di comando.
- Bellezza del layout oltre il rispetto delle posizioni originali.
- Conversione di simboli e polilinee decorative.
- Creazione dei moduli mancanti (`axch`, `clav`, `fctt`, `mitn`, `vact`,
  `ward`): è modellazione, non conversione.

## Il `libgraph` da clonare in `/home/antonio/legocad`

Tutti i candidati coprono le stesse **23 delle 29** istanze richieste dalle
task legocad, quindi la scelta si gioca su completezza e attualità:

| candidato | `.i5` | librerie | `remark` | `xbm` | note |
|---|---|---|---|---|---|
| `legopst_userstd_save_con_dir_i5/legocad/libgraph` | 394 | 20 | sì | **no** | recente (2026-05) |
| `legopst_milost64/legocad/libgraph` | 394 | 19 | sì | sì | completo, ma del 2011 |
| `legopst_nuclear/legocad/libgraph` | 391 | 22 | **rinominata** | sì | `remark` spostata in LegoPST |
| `Alg_legopc/user_default/libgraph` (repo) | 124 | 8 | no | sì | set ridotto, copre solo 19/29 |

Scelta consigliata: **`legopst_userstd_save_con_dir_i5`**, aggiungendoci `xbm`
preso da un altro (serve a `legopc.tix` per le icone di contorno):

```sh
cp -a /home/antonio/legopst_userstd_save_con_dir_i5/legocad/libgraph /home/antonio/legocad/
cp -a /home/antonio/legopst_milost64/legocad/libgraph/xbm           /home/antonio/legocad/libgraph/
```

`libgraph` deve contenere `connect.dat`, `files_i5`, `help`, `libraries`,
`pixmaps`, `xbm`; `Alg_env.sh` ne deriva `LG_FILESI5`, `LG_LIBRARIES`,
`LG_PIXMAPS`, `LG_HELP`, `LG_XBM` a partire da `LG_ENTRY`.
