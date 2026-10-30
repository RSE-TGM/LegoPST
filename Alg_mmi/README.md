# Alg_mmi — l'interfaccia uomo-macchina LegoPST

`Alg_mmi` è l'HMI "SCADA-like" di LegoPST: pagine sinottiche animate, disegnate con
un editor grafico e visualizzate a run-time collegandosi ai punti del simulatore.
È un mondo separato da `legopc` (il CAD di modellazione) e da `xstaz`/`draw2gr`
(i faceplate e le pagine di processo di `Alg_rt`): usa file, directory e
convenzioni tutte sue.

| eseguibile | sorgente | ruolo |
|---|---|---|
| `mmi` | [run_time/](run_time/) | il run-time: mostra le pagine e le anima |
| `config` | [config/](config/) | l'editor/configuratore delle pagine (pagedit) |
| `compreg` | [config/](config/) | compilatore delle pagine di regolazione |
| `conv_l`, `conv_r` | [conv_legograf/](conv_legograf/), [conv_regograf/](conv_regograf/) | conversione da schemi LEGO/regolazione a pagine |
| `convstaz` | [conv_staz/](conv_staz/) | conversione stazioni di comando |
| `demone_mmi`, `client_mmi`, `server_mmi` | [client_server/](client_server/) | collegamento MMI ↔ simulatore |

Gli eseguibili finiscono in [bin/](bin/) (`$LEGOMMI_BIN`).

---

## 1. Il file Context: l'unico punto di verità

Tutto ciò che l'MMI deve trovare su disco è dichiarato in un **file Context**
(`Context.ctx`), che è un normale **file di risorse X** letto con
`XrmGetFileDatabase`. Non esiste nessun altro meccanismo di ricerca: niente
variabili di ambiente da impostare a mano, niente lista di directory da provare
in sequenza, nessun default nel codice.

Le risorse che descrivono le directory ([include/pagresdef.h](include/pagresdef.h)):

| risorsa | significato |
|---|---|
| `*pages` | directory delle pagine (`.pag`, `.rtf`, `.bkg`) |
| `*simulator` | directory del simulatore, dove sta `variabili.rtf` (il db dei punti) |
| `*objectLibraries` | librerie di oggetti grafici |
| `*animatedIconLibraries` | librerie di icone animate |
| `*description` | descrizione mostrata dall'MMI |
| `*hostName`, `*hostNameS` | host del server e dello SCADA |
| `*numDisplay`, `*displayList` | display su cui l'MMI può aprire pagine |
| `*pag_num`, `*page_list` | **quante** e **quali** pagine esistono |
| `*iconlib_num`, `*iconlib_list`, `*iconlib_label` | librerie di icone (solo per l'editor) |

Più, per ogni pagina, un blocco di attributi:

```
*ACSP.top_tipo:         Sinottico
*ACSP.top_descrizione:
*ACSP.refresh_freq:     10
*ACSP.gerarchia:        -1,-1,-1,-1,-1,-1
*ACSP.tagPag:
*ACSP.schemeInUse:      0
```

Esempio reale (`$KSIM/globpages/Context.ctx` di SLaurent_0):

```
*description:            SLaurent_0 - PWR 900 MWe Saint Laurent B1
*simulator:              /home/antonio/sked/SLaurent_0
*pages:                  /home/antonio/sked/SLaurent_0/globpages
*objectLibraries:        /home/antonio/legocad/libut_reg/libreg
*animatedIconLibraries:  /home/antonio/legocad/libut_reg/libreg
*pag_num:                20
*page_list:              \ ACSP ACTR ATTT LTTV MVAR RHPC ... RVTV
```

> **L'elenco delle pagine non viene dal filesystem.** `mmi` non fa `ls` della
> directory: legge `*pag_num` e `*page_list`
> ([topLevelShellMain.c:1244](run_time/topLevelShellMain.c#L1244)). Una pagina il
> cui `.rtf` esiste ma che non compare in `*page_list` per l'MMI **non esiste**.

---

## 2. Come `mmi` trova le pagine (run-time)

```
cwd ──► Context.ctx ──► risorsa *pages ──► LEGOMMI_PAGINE ──► $LEGOMMI_PAGINE/<NOME>.rtf
```

1. **Quale Context** — il default è la stringa `"Context.ctx"`, quindi *relativa
   al cwd* ([run_time.c:193](run_time/run_time.c#L193)); si sovrascrive con
   l'opzione `-Context <file>` ([other_mmi.c:2053](run_time/other_mmi.c#L2053)).
2. **Lettura** — `LoadCtxResDb()` carica il file e lo fonde nel database risorse
   del display ([topLevelShellMain.c:416](run_time/topLevelShellMain.c#L416)).
3. **Estrazione** — `*pages` → `path_pagine`, `*animatedIconLibraries` →
   `path_ico`, `*simulator` → `path_simulator`.
4. **Esportazione** — `XlPutenv("LEGOMMI_PAGINE", path_pagine)` e
   `XlPutenv("LEGOMMI_ICO", path_ico)`
   ([topLevelShellMain.c:1909](run_time/topLevelShellMain.c#L1909)).
   `XlGetenv`/`XlPutenv` sono `getenv`/`putenv` nudi, senza fallback
   ([XlEnv.c:55](lib/Xl/XlEnv.c#L55)).
5. **Composizione del nome** — sempre nella stessa forma:

   | file | dove viene composto |
   |---|---|
   | `$LEGOMMI_PAGINE/<NOME>.rtf` | [other_mmi.c:193](run_time/other_mmi.c#L193), [teleperm.c:3486](run_time/teleperm.c#L3486), [OperatingWindow.c:2887](run_time/OperatingWindow.c#L2887) |
   | `$LEGOMMI_PAGINE/<NOME>.bkg` | [other_mmi.c:1404](run_time/other_mmi.c#L1404) |

**Il run-time non fa nessuna `chdir` e nessuna conversione relativo→assoluto.**
Un `*pages: ./` significa letteralmente "la directory da cui è stato lanciato
`mmi`". Per questo `kMmi` fa una `cd` prima di lanciarlo (§6).

### Opzioni di lancio ([run_time/other.h](run_time/other.h#L30))

| opzione | effetto |
|---|---|
| `-Context <file>` | usa un Context diverso da `./Context.ctx` |
| `-pag <NOME>` | pagina da aprire all'avvio (ripetibile, max `MAX_STARTUP_PAGES`) |
| `-pagback <NOME>` | pagina di background |
| `-alldisp` | apre la pagina di startup su tutti i display configurati |
| `-noclose` | inibisce la chiusura |
| `-Topologia`, `-DemonePort` | collegamento al demone |

---

## 3. Come `config` trova le pagine (editor)

Il configuratore usa una strada **diversa** dal run-time: risolve i path
**rispetto alla directory del file Context**, non al cwd.

1. `Context_Path = dirname(<file context>)`, o il cwd se il nome è nudo
   ([pagedit.c:119](config/pagedit.c#L119));
2. `XlChDir(Context_Path)`, poi `path_rel_to_abs` su ogni voce;
3. esporta le variabili ([pagedit.c:160-245](config/pagedit.c#L160)):

| variabile | da | contiene |
|---|---|---|
| `LEGOMMI_CTX` | directory del Context | radice del progetto |
| `LEGOMMI_PAG` | `*pages` | `.pag`, `.rtf`, `.bkg` |
| `LEGOMMI_LIB` | `*objectLibraries` | librerie oggetti |
| `LEGOMMI_ICO` | `*animatedIconLibraries` | icone animate |
| `LEGOMMI_RTF` | `*simulator` | `variabili.rtf` |
| `LEGOMMI_WORK` | `$LEGOMMI_CTX/proc` | uscite di regolazione ([EstrWorkFile](config/comp_pag_util.c#L592)) |

I nomi dei file si compongono con due sole funzioni:

- [`FileNameLoc(pagina, tag)`](config/comp_all.c#L1032) → `$LEGOMMI_PAG` + nome + tag
  (`.pag`, `.rtf`)
- [`FileNameInproc(pagina, tag)`](config/regol_util.c#L2846) → `$LEGOMMI_WORK` +
  nome **in minuscolo** + tag (`_04.dat`, `_14.dat`, `.reg`)

> **Attenzione ai due nomi diversi.** Il run-time usa `LEGOMMI_PAGINE`,
> l'editor usa `LEGOMMI_PAG`, per la stessa directory
> ([config.h:145](include/config.h#L145)). Impostarne una a mano non ha effetto
> sull'altro programma.

### Lo zoom di una pagina

Due modi, equivalenti:

- *Option → Zoom* nella pagina: 100%, 200%, 300%, 400%;
- **Ctrl + rotella del mouse** sulla pagina, sopra un'icona o sullo sfondo:
  la rotella in su passa al gradino successivo (fino a 400%), in giù al
  precedente (fino a 100%). Il punto sotto il puntatore resta fermo, e il
  menu *Zoom* segna sempre il gradino attuale
  ([PagZoomRotella](config/PagShell.c)).

I gradini sono solo quelli interi: le icone sono state fatte per fattori
interi. La rotella **senza** Ctrl non fa scorrere la pagina (non lo ha mai
fatto): si scorre con le barre.

### Spostare e ridimensionare le icone

Un'icona si sposta **premendo e trascinando**, con il tasto sinistro o con
quello centrale, a qualunque zoom
([move_or_resize](config/othercnf.c)):

- non serve selezionarla prima: se non lo è, viene selezionata alla
  pressione (con il tasto sinistro basta il clic stesso);
- se si preme su un'icona che fa parte di una selezione multipla, si
  spostano tutte le icone selezionate (e gli elementi di sfondo
  selezionati);
- l'icona si prende in qualunque punto, anche sulla scritta interna
  ("UA"), sul disegno interno di un'icona di `LIB_pictograph` (un
  `XlComposite`: il clic seleziona solo quel pezzo, ma trascinando si
  sposta l'icona intera) o sul bordo; col tasto sinistro **non** su una
  porta, che serve alle connessioni. I pezzi interni si spostano uno per
  uno solo nelle pagine di libreria, dove le icone si disegnano;
- trascinata oltre il bordo della pagina, si ferma al bordo;
- lo spostamento segue la griglia (*snap*) moltiplicata per lo zoom.

**Ridimensionare**: solo gli oggetti che lo ammettono (non le icone di
regolazione, non le porte) e uno alla volta. Le maniglie sono una fascia
di 8 pixel lungo il bordo (`MANIGLIA_RESIZE` in
[config.h](include/config.h)), un quarto del lato sugli oggetti piccoli;
l'interno sposta.

Prima di ottobre 2026 si spostava solo con il tasto centrale e solo
un'icona già selezionata; il move partiva solo dal quadrato centrale
(metà larghezza per metà altezza), il resto era maniglia di resize anche
per le icone che non si ridimensionano, e un clic al centro di
un'icona di regolazione ne selezionava la scritta interna: in tutti
questi casi la pressione non faceva niente, e sembrava che il
trascinamento si "inceppasse". In più il primo tratto del movimento
andava perso, e un'icona portata oltre il bordo tornava al suo posto
senza messaggi. Trascinare il pezzo interno di un `XlComposite` in una
pagina di regolazione faceva crashare `config`: `select_connections`
passava il composite a `manage_all_conn`, che vuole la drawing della
pagina. Il tasto centrale per il trascinamento è la convenzione
Motif degli anni '90 (sinistro seleziona, centrale trascina, destro menu).

### Copiare un'icona da una libreria (Add Icon)

Dal menu **Add Icon** della pagina si apre la palette di una libreria
(`IconShell`). Un'icona si copia nella pagina **premendo e trascinando**
col tasto sinistro (o col centrale, come prima)
([do_rubber](config/othercnf.c)):

- durante il trascinamento il contorno dell'icona si vede **nella pagina**
  sotto il puntatore, grande come sarà a quello zoom e agganciato alla
  griglia; il punto per cui l'icona è stata presa resta sotto il
  puntatore, e l'icona viene posata esattamente dove si vede il contorno;
- un clic semplice nella palette seleziona soltanto l'icona.

Prima di ottobre 2026 il contorno si disegnava sulla root window dello
schermo: sotto WSLg (XWayland senza desktop X) la root non si vede e il
contorno non compariva; e l'icona veniva posata con l'angolo in alto a
sinistra sul puntatore.

**Selezione nella palette.** Cliccando un'icona si deselezionano le altre.
Prima, nella palette di REGOLIB le icone restavano tutte selezionate,
mentre in LIB_pictograph no: la `Select` della libreria Xl deseleziona
solo i fratelli `XlCore`, e le icone di regolazione (`XlIconReg`) derivano
da `XlManager`. Nelle pagine non si notava perché `config` deseleziona
tutto da sé ([proc_deselect](config/othercnf.c)), che ora lo fa anche
nella palette.

### Chiudere una pagina: modifiche non salvate

*File → Close* e la **X** della finestra fanno la stessa cosa
([PagChiudiRichiesta](config/PagShell.c)):

- se la pagina non è cambiata dall'apertura (o dall'ultimo *Save*) si
  chiude subito, senza domande;
- se è cambiata compare *Page … has unsaved changes*, con **Save**
  (salva come *File → Save* e chiude), **Discard** (chiude senza salvare)
  e **Cancel** (la pagina resta aperta). In *Interface Mode*, dove il
  salvataggio è disattivato, *Save* è grigio.

Come si sa che è cambiata: non c'è un flag di modifica (le modifiche
passano da troppi punti), si confronta un'**impronta** di quello che il
*Save* scriverebbe: database delle risorse (righe ordinate), dati della
pagina, elenco degli oggetti, disegno di sfondo. La si prende
all'apertura e dopo ogni salvataggio (anche quello fatto dalla
compilazione, che salva il `.pag` ma non il `.bkg`). Lo zoom non conta:
dall'impronta del `.bkg` si toglie il riquadro `x_min_d … y_max_d`, che è
in pixel dello zoom corrente. Per vedere cosa differisce, lanciare
`config` con `CONFIG_IMPRONTE=<dir>`: alla chiusura di una pagina
modificata scrive in `<dir>` le due impronte (`<PAG>_<parte>_prima.txt`
e `_ora.txt`; parte 0 = `.pag`, 1 = `.bkg`).

Fino a ottobre 2026 la X lasciava il default Motif (`XmDESTROY`): la
finestra spariva ma la pagina restava nella lista delle pagine aperte, e
riaprendola `config` usciva con `BadWindow` (portava in primo piano una
finestra che non c'era più). *File → Close* chiedeva solo *Do you really
want close …?*, senza dire se c'erano modifiche.

**Uscita da `config`** (*File → Exit* o la X della finestra principale)
([PagUscitaControllo](config/PagShell.c)): se ci sono pagine aperte con
modifiche non salvate compare il loro elenco, con **Save all** (salva
quelle modificate ed esce; se un salvataggio fallisce non esce),
**Discard** (esce senza salvarle) e **Cancel**. Dopo Save all o Discard
non si chiede di nuovo *Do you really want to Exit?*; la domanda sul
Context modificato (*Context is not Saved*) resta. Senza pagine
modificate l'uscita è quella di sempre.

**Riaprire una libreria.** La lista delle librerie è `XmSINGLE_SELECT`,
dove un clic su una voce già selezionata la deseleziona: dopo aver chiuso
una libreria la sua voce restava selezionata, il doppio clic la toglieva
e il primo tentativo di riaprirla andava a vuoto. Ora il doppio clic
seleziona la voce su cui è fatto
([defaultActionCB_LibraryList](config/topLevelShell.c)).

### Il nome completo delle variabili sulle porte (balloon)

In *Option → Set visual mode* la casella **Variable balloon** accende, per
quella pagina, un riquadro giallo che compare quando il mouse si ferma
vicino a una porta di un'icona ([balloon.c](config/balloon.c)). Per il
sommatore 44 della pagina RSL2 di `r_SLB1_0`:

```
I14441SL   INGRESSO 1 SOMMATORE A 2 INGRESSI
<- U13I41SL
```

- **Il nome completo** è nome della porta + tag del blocco + sigla della
  task: `I1` + `4441` + `SL`. La sigla sono le due lettere dopo `r_` nel nome
  della directory, come in compilazione. Si calcola dalla pagina com'è
  adesso.
- **Il collegamento** (`<-` per un ingresso, `->` per un'uscita) viene da due
  fonti:
  - il filo nella pagina, quando arriva a un altro blocco o a un'icona di
    interfaccia (per l'altro schema si legge `Connessioni.reg`:
    `<- MVAR:U33X3QSL`). È lo stato attuale.
  - `proc/<pagina>_01.dat`, che risolve anche i fili che passano per le
    diramazioni e dà la descrizione della variabile, ma è **scritto solo
    dalla compilazione dello schema**. Quando le due fonti non coincidono il
    balloon le mostra entrambe (`(last compile: ...)`); quando il `.pag` è
    stato salvato dopo l'ultima compilazione lo dice (`page saved after the
    last compile`); per uno schema mai compilato dice che i collegamenti
    vengono solo dalla pagina.
- *not connected (model input)*: la compilazione ha lasciato l'ingresso
  libero, cioè è un dato del modello.
- Sopra un'icona di interfaccia: la variabile e l'altra pagina, da
  `Connessioni.reg`.

La zona sensibile è di qualche pixel attorno alla porta, che da sola è
minuscola. La modalità vale finché la pagina resta aperta e non si salva nel
`.pag`.

Righe del `_01.dat` usate, per chi deve metterci le mani:

| Riga | Significato |
|---|---|
| `UA4441SL --UA-- descrizione` | uscita del blocco |
| `U13I41SL --IN-- #I14441SL#...<===3I41` | ingresso `I14441SL` alimentato da `U13I41SL` |
| `I14441SL --CO-- descrizione` | ingresso collegato |
| `IP4N41SL --IN-- descrizione` | ingresso libero, dato del modello |

### Collegare gli schemi di una task di regolazione

In una task di regolazione si porta una variabile d'uscita di uno schema
(una pagina) a una variabile d'ingresso di un altro schema attraverso le
**icone di interfaccia**. Ognuna ha due porte:

- una **interna**, che dentro lo schema si collega con un filo normale alla
  porta di un blocco;
- una **di interfaccia**, rivolta verso l'esterno, che si collega alla porta
  di interfaccia dell'altro schema.

Le connessioni fra schemi non stanno nelle pagine: stanno in
**`$LEGOMMI_PAG/Connessioni.reg`**. Le pagine se ne ricordano solo per
disegnare le porte occupate.

**Prima di tutto, dentro ciascuno dei due schemi**, si collega la porta
interna dell'icona di interfaccia al blocco: nello schema di partenza
l'icona d'uscita al blocco che produce la variabile, nell'altro l'icona
d'ingresso al blocco che la riceve. Si fa come ogni collegamento, con
*Option → Connect Mode* nella pagina. Senza questo passo la connessione fra
schemi viene rifiutata con *Cannot connect Interface Port That Is Not
Internally Connected* ([CreoNuovaInterfaccia](config/othercnf.c#L4118)).

Poi le due porte di interfaccia si collegano in due modi, che scrivono lo
stesso file e si possono alternare: direttamente nelle pagine, oppure con
l'editor `lgconn` (anche da `config`, menu *Scheme Conn.*). Il terzo modo
storico, *Interface Mode*, è disattivato dall'ottobre 2026 (vedi sotto).

#### Collegare direttamente nelle pagine

Si lavora come per un filo dentro lo schema, senza cambiare modalità e senza
chiudere le pagine.

1. Aprire le due pagine e attivare in entrambe *Option → Connect Mode*.
2. Fare clic con il tasto **sinistro** sulla **porta di interfaccia** della
   variabile d'uscita in uno schema: è il quadratino sulla punta esterna del
   triangolo, non il triangolo. La riga di stato in fondo alla pagina
   diventa verde e dice *Output selected: now click the interface INPUT port
   in the other page*.
3. Clic sinistro sulla **porta di interfaccia** della variabile d'ingresso
   nell'altra pagina. L'ordine dei due clic non conta: `config` riconosce
   l'ingresso dal tipo della porta.
4. La connessione è **già salvata** in `Connessioni.reg`. Tutte e due le
   pagine lo confermano nella riga di stato (*Connected ACSP <-> ACTR, saved
   in Connessioni.reg*).

**La riga di stato.** Ogni pagina di `config` ha in fondo una riga che
dice com'è andato l'ultimo clic, senza aprire finestre
([PagStatusMsg](config/PagShell.c)):

- **verde**: fatto, oppure il passo successivo;
- **gialla, con un bip**: clic rifiutato, e il perché;
- dopo qualche secondo il colore torna normale, e il testo resta fino alla
  notifica successiva.

I rifiuti più comuni:

| Riga di stato | Cosa fare |
|---|---|
| *Port already connected: Edit -> Disconnect interface frees it* | la porta è già collegata a un altro schema: scollegarla prima, o sceglierne una libera (`lgconn` mostra quelle libere) |
| *Wrong port: an output goes to an input of the same kind (analog/digital)* | due ingressi, due uscite, o analogico con digitale: cliccare un'altra porta, il primo clic resta valido |
| *Not connected: an interface port goes to another page* | i due clic sono nella stessa pagina: si ricomincia dal primo |
| *Not connected: an interface port goes to an interface port of another page* | il secondo clic è finito sulla porta di un blocco |
| *Not connected: in this page the interface icon has no wire to a block, draw it first* | manca il filo interno allo schema (vedi sopra) |
| *Not connected: Connessioni.reg already has one of the two ports (changed by lgconn?): reopen the pages* | nel frattempo `lgconn` o un altro `config` ha collegato una delle due porte: riaprire le pagine |

Fino all'ottobre 2026 questi rifiuti finivano solo sul terminale da cui era
partito `config`, come `XlWarning`, e a video il clic sembrava non fare
niente. Sul terminale ci sono ancora.

Una porta di interfaccia scelta con il primo clic non blocca il resto: un
secondo clic sulla stessa porta annulla la scelta (*Selection cancelled*), e
un clic sulla porta di un blocco la abbandona e comincia un filo normale.
Un filo normale lasciato a metà si annulla con il tasto destro.

Prima di scrivere, `config` rilegge il file
([CollegaInterfacceSuFile](config/othercnf.c#L1860)), per non sovrascrivere
le connessioni fatte nel frattempo da `lgconn` o da un altro `config`.

**Scollegare**

1. Fuori dal *Connect Mode*, selezionare l'icona di interfaccia collegata,
   oppure la sua porta. Si possono selezionare più icone insieme.
2. **Disconnect interface**, dal menu *Edit* della pagina o dal menu del
   tasto destro ([scollega_interfacce](config/othercnf.c#L1909)).
   La connessione viene tolta e salvata subito, e la porta torna libera
   anche nell'altra pagina, se è aperta. La riga di stato conferma con
   *Disconnected N connection(s) to other pages, saved in Connessioni.reg*.

Con *Connect Mode* attivo un clic sulla porta non seleziona l'icona: per
scollegare, prima si esce da *Connect Mode*.

L'icona collegata non si cancella con il Delete: la riga di stato risponde
*Not deleted: the icon is connected to another page, Edit -> Disconnect
interface first*. Prima si scollega, poi si cancella.

Se `Connessioni.reg` è stato cambiato da un altro programma fra la lettura e
la scrittura, `config` non lo sovrascrive. Lascia il risultato in
`Connessioni.reg.config` e lo dice (*Connessioni.reg changed elsewhere*).

#### Collegare in *Interface Mode* (disattivato)

**Dal 26 ottobre 2026 la spunta *Interface Mode* è disattivata**: resta,
in grigio, nel menu *Scheme Conn.* della finestra principale, dopo la voce
di `lgconn`. Gli stessi collegamenti si fanno nelle pagine, salvati subito,
o con `lgconn`. Il codice della modalità è intatto: per riattivarla basta
togliere `XmNsensitive, FALSE` dalla creazione di `Mode` in
[topLevelShell.c](config/topLevelShell.c) e ricompilare. Quel che segue
descrive come funzionava.

Il modo storico. Il file si legge quando si entra in *Interface Mode* e si
**scrive solo quando se ne esce**
([set_interface](config/othercnf.c#L4074)).

1. **Chiudere tutte le pagine.** Con una pagina aperta la modalità non cambia,
   e compare *Close all the page first.*
2. Nella finestra principale di `config`, menu **Scheme Conn.** (fino
   all'ottobre 2026 si chiamava *Interface Mode*), attivare la voce a spunta
   **Interface Mode**. Buona parte dei menu principali si spegne,
   e nelle pagine aperte da qui il *Connect Mode* è già attivo.
3. Aprire le due pagine.
4. Fare clic sulle due **porte di interfaccia**, come sopra. In questa
   modalità rispondono **solo** le porte di interfaccia
   ([proc_SelPort](config/othercnf.c#L3459)).
5. Chiudere le pagine e **togliere la spunta** da *Interface Mode*: è qui che la
   connessione viene salvata in `Connessioni.reg`. Finché si resta nella
   modalità, non è scritta su disco.

**Cancellare la connessione**

1. Con le pagine chiuse, entrare in *Interface Mode* e aprire la pagina con
   l'icona di interfaccia collegata.
2. Selezionare l'icona di interfaccia.
3. **Delete**, oppure *Disconnect interface*, dal menu *Edit* della pagina o
   dal menu del tasto destro. In *Interface Mode* il Delete **non cancella
   l'icona**: toglie solo la sua connessione verso l'altro schema e libera la
   porta ([delete_interface](config/othercnf.c#L2017)).
4. Chiudere le pagine e uscire da *Interface Mode* per salvare.

Per cancellare l'icona stessa, o il filo interno allo schema, si lavora fuori
da *Interface Mode*, con il Delete normale della pagina.

#### Collegare e scollegare con l'editor `lgconn` (consigliato)

`lgconn` fa lo stesso lavoro senza aprire `config` e senza aprire le
pagine, e mostra tutte le connessioni della task in una tabella. Si lancia in tre modi:
- da **config**, nella finestra principale: *Scheme Conn. → lgconn - Manage
  sch. connections*, sulla task aperta in `config`
  ([activateCB_Lgconn](config/topLevelShell.c)). Non parte se `config` è in
  *Interface Mode*, che tiene le connessioni in memoria e le scrive
  all'uscita. Le pagine già aperte in `config` vedono le modifiche di
  `lgconn` solo dopo averle riaperte;
- da **lghmi**, selezionando la task di regolazione e scegliendo *Tools →
  lgconn - connections between the schemes of the selected regulation task*;
- da terminale, con `lgconn <directory della task>`.

La finestra ha due tabelle:

- **In alto, tutti gli ingressi di interfaccia** degli schemi della task.
  Per ciascuno si vedono: pagina, icona, variabile, blocco e descrizione,
  l'uscita a cui è collegato e lo **stato**. Lo stato è *collegato*,
  *libero* (in grigio), oppure un problema: in giallo gli avvisi, in rosso
  gli errori, gli stessi di `lgconn -c`. In fondo alla tabella stanno le
  connessioni verso pagine che non sono più nella task.
- **In basso, le uscite collegabili** all'ingresso selezionato. Sono solo le
  uscite che vanno bene: libere, collegate a un blocco dentro il loro
  schema, dello stesso tipo (analogico o digitale).

Le caselle *Cerca* filtrano le due tabelle per pagina, variabile, blocco o
descrizione. Il menu **?** apre una guida rapida (cos'è `lgconn` e come si
usa, in una finestra di `lgconn` stesso) e, nel browser, questa guida e gli
altri documenti; mentre il browser parte c'è la finestrella di attesa
(*Starting firefox ...*), come in `lghelp`.

**Collegare.** Selezionare in alto l'ingresso, poi in basso l'uscita. Si
conferma con doppio clic o con **Collega**.

**Scollegare.** Selezionare in alto l'ingresso collegato e premere
**Scollega**.

**Salvare.** Con **Salva** o `Ctrl+S`. Finché non si salva non viene scritto
niente, e la barra in basso dice *MODIFICATO, non salvato*. Il primo
salvataggio lascia il file com'era in `Connessioni.reg.bak`. Uscendo con
modifiche non salvate, `lgconn` chiede cosa fare.

Dal menu *Modifica* ci sono anche due pulizie:

- **Aggiorna i dati copiati dalle pagine.** Riscrive variabile, tag,
  descrizione e valore di ogni connessione con quelli attuali dello schema:
  corregge gli errori "la compilazione userebbe il valore vecchio" di
  `lgconn -c`.
- **Togli le connessioni verso pagine assenti.**

Cosa resta da fare in `config`: le icone di interfaccia e il loro filo
interno al blocco si mettono nello schema, come sempre. `lgconn` collega fra
loro icone già esistenti. Se un'icona non è collegata dentro lo schema,
`lgconn` lo dice e non la propone.

Le righe scritte da `lgconn` sono identiche a quelle che scriverebbe
`config`. Quelle che non si toccano restano byte per byte come erano, e il
file si legge con il `config` di sempre.

**`config` e `lgconn` aperti insieme.** Entrambi tengono in memoria le
connessioni lette, e nessuno dei due riscrive il file se nel frattempo
l'altro lo ha cambiato:
- `lgconn` lo segnala al salvataggio e propone di mettere le sue connessioni
  in `Connessioni.reg.lgconn`;
- `config`, all'uscita da *Interface Mode*, mette le sue in
  `Connessioni.reg.config` e lo dice ([OlConn.c](lib/Ol/OlConn.c),
  `file_cambiato`).

Collegando direttamente nelle pagine il rischio è minore: `config` rilegge
il file a ogni collegamento e a ogni *Disconnect interface*. Resta il fatto
che le porte disegnate nelle pagine aperte non si aggiornano da sole: dopo un
salvataggio di `lgconn` conviene riaprirle.

Fino all'ottobre 2026 `config` riscriveva il file in ogni caso, e cancellava
senza avvisare le modifiche fatte nel frattempo da altri.

#### Il file `Connessioni.reg`

Una connessione per riga, campi separati da `|`
([WriteConn](lib/Ol/OlConn.c#L197)):

```
uscita|<lato uscita: 9 campi>|ingresso|<lato ingresso: 9 campi>|valore|<valore>
```

I 9 campi di ciascun lato:

| # | Campo | Da dove lo prende `config` | Chi lo usa |
|---|---|---|---|
| 1 | pagina | la pagina dell'icona di interfaccia | compilazione, apertura delle pagine |
| 2 | porta di interfaccia | la porta esterna dell'icona (`interfacePort: 1`) | apertura delle pagine |
| 3 | icona di interfaccia | l'icona (`iconRegType` 1 o 2) | apertura; la compilazione usa quella del lato ingresso |
| 4 | porta interna | la porta del blocco a cui l'icona è collegata dentro lo schema | nessuno |
| 5 | blocco | quel blocco | nessuno |
| 6 | variabile | `portName` di quella porta | **compilazione** |
| 7 | modulo | `modulName` del blocco **dell'altro lato** (vedi sotto) | nessuno |
| 8 | tag | `tagName` del blocco | **compilazione** |
| 9 | descrizione | `iconDescr` del blocco, `-` se vuota | nessuno |

Il *valore*, l'ultimo campo, è il valore iniziale della variabile d'uscita. Si
legge in `inputValue` del blocco, nella posizione che la variabile ha fra le
righe `input` della form del modulo (`<modulo>.frm` in `*objectLibraries`).
Non lo usa nessuno.

Cose da sapere:

- **Sono copie, e `config` non le aggiorna.** Variabile, tag, descrizione e
  valore sono fotografati quando si fa la connessione. Se poi lo schema cambia
  (un altro blocco collegato all'icona, un tag rinominato), il file resta com'era
  e la compilazione usa **variabile e tag vecchi**, senza avvisare. `lgconn -c`
  lo trova.
- **Il modulo è scambiato.** `CreoNuovaInterfaccia` scrive nel lato uscita il
  modulo del blocco d'ingresso e viceversa ([othercnf.c](config/othercnf.c#L4049)).
  Non ha conseguenze, perché il campo non lo legge nessuno.
- **I nomi `43w`, `43w0c` sono nomi di widget**, assegnati in ordine di
  creazione. Copia/incolla e duplicazioni li cambiano, e `config` cerca di
  rimappare le connessioni ([pagina.c](config/pagina.c#L1404)).
- **Nei `.pag`** la porta di interfaccia ha una risorsa `portNameConnect` con
  l'altro capo della connessione. È una copia che `PostOpenPage2` rifà da
  `Connessioni.reg` ogni volta che apre la pagina: comanda il file.

#### Controllare le connessioni: `lgconn -c`

```
lgconn -c [-v] [DIR]
```

Legge `Connessioni.reg` e le pagine della task (`DIR`, quella con
`Context.ctx`; senza, la directory corrente), senza aprire niente e senza
display, e scrive un rapporto:

| Gravità | Cosa |
|---|---|
| **errore** | variabile o tag nel file diversi da quelli del blocco collegato oggi (la compilazione userebbe i vecchi); porta di interfaccia sparita da una pagina della task; icona di interfaccia non collegata a nessun blocco dentro lo schema; porta d'uscita usata come ingresso o viceversa; analogico collegato a digitale; una porta in più connessioni; riga illeggibile |
| **avviso** | connessioni con pagine che non sono più nella task (riassunte per pagina: non servono a niente, si possono togliere); icona collegata dentro lo schema a un blocco diverso da quello del file |
| **nota** | ingressi di interfaccia non collegati ad altri schemi. Non sono per forza un errore, perché la variabile resta un ingresso della task e può arrivare da un'altra task. Con `-v` sono elencati, altrimenti se ne dà il numero |

Esce con 0 senza errori, 1 con errori, 2 se la task non si legge. Non modifica
niente.

La lettura delle pagine è verificata contro i file veri: sulle 26 task di
regolazione presenti sulla macchina di sviluppo (circa 2.600 connessioni confrontabili),
per ogni connessione il cui collegamento interno non è cambiato, la riga
ricostruita dalle pagine è identica a quella scritta da `config`. Fanno
eccezione valore e descrizione, che sono copie invecchiate. La prova è
`Alg_legopc/src/tix/lgconn_test.tcl`, ed è anche nella CI (prova `lgconn`).
Sulla task di regolazione della demo nuclear, `r_SLB1_0`, il controllo trova
2 errori veri (riga 131: variabile e tag cambiati dopo il collegamento) e 76
connessioni verso pagine che non esistono più.

---

## 4. Ciclo di vita dei file di una pagina

| estensione | dove | prodotto da | letto da |
|---|---|---|---|
| `<NOME>.pag` | `$LEGOMMI_PAG` | editor `config` | editor, `kGlobContext` |
| `<NOME>.rtf` | `$LEGOMMI_PAG` | compilazione | **`mmi`** |
| `<NOME>.bkg` | `$LEGOMMI_PAG` | editor | `mmi` (sfondo) |
| `variabili.rtf` | `$LEGOMMI_RTF` (= `*simulator`) | il simulatore | compilazione |
| `<nome>_04.dat`, `_14.dat`, `.reg` | `$LEGOMMI_CTX/proc` | `compreg` | generazione task |

La compilazione è di due tipi ([config/comp_all.c](config/comp_all.c)):

- **sinottici** — compilati in-process da `config`, che istanzia i widget e
  scrive il `.rtf`;
- **pagine di regolazione** — delegate all'eseguibile esterno:
  `compreg <pagina> $LEGOMMI_PAG $LEGOMMI_RTF` ([comp_all.c:236](config/comp_all.c#L236)).

`IsPaginaCompiled` decide se ricompilare confrontando le date: `.rtf` contro
`.pag` per i sinottici, `_04.dat`/`_14.dat`/`.reg` contro `.pag` per le
regolazioni ([comp_all.c:715](config/comp_all.c#L715)).

---

## 5. Da dove vengono le pagine: i tre generatori

I `.pag` non nascono in un modo solo. Tre strade, che convergono tutte sulla
stessa compilazione descritta sopra:

| generatore | ingresso | uscita | a cosa serve |
|---|---|---|---|
| `config` (pagedit) | disegno interattivo | `<NOME>.pag` + `.bkg` | sinottici e pagine di regolazione disegnate a mano |
| `convstaz` | `r01.dat` | una `<NOME>.pag` per blocco `PAGINA` | portare nell'MMI i faceplate di comando gia' descritti per `xstaz` |
| `mkstaz`, via `kcreastaz` o `kOw` | `<base>.templ` + `<base>.list` | una `.pag` + `.bkg` per riga della lista | finestre di malfunzione e operating window, generate in serie dal database d'impianto |

### `convstaz` — dallo stesso `r01.dat` dei faceplate

Sorgenti in [conv_staz/](conv_staz/), binario `bin/convstaz`. È un **fork di
`compstaz`** ([Alg_rt/grafica/xstaz](../Alg_rt/grafica/xstaz/README.md)): stesso
parser, stessi moduli per tipo di oggetto, con innestata la scrittura delle
pagine MMI. Si vede ancora dalla firma, che è rimasta quella del progenitore
(*"Fine corretta COMPSTAZ"*).

Si lancia dalla directory che contiene il file:

```sh
cd <dir con r01.dat> && convstaz            # scrive nella dir corrente
cd <dir con r01.dat> && convstaz -d $KWIN   # scrive direttamente in o_win
```

| | file |
|---|---|
| ingresso | `r01.dat` (l'unico) |
| uscite | `convstaz.log` + per ogni blocco `PAGINA` una **`<NOME>.pag`** (risorse X, `*top_tipo: Stazioni`) e una **`<NOME>.bkg`** |

L'opzione **`-d <dir>`** dice dove depositare pagina e sfondo; senza, restano
nella directory corrente come è sempre stato.

Lo sfondo generato è "vuoto" (bounding box degenere, zero oggetti disegnati) ed è
quello che serve a un faceplate, il cui fondo è il colore di
`*drawing_background`. Ma **deve esistere**: la raccolta collega sempre il `.bkg`
accanto al `.rtf` con un `ln -sf` che non verifica il target, e senza il file
l'MMI segnala *"Errore lettura file di background"*.

Il nome del file viene dal campo `NOME` del blocco `PAGINA` e **non può superare
gli 8 caratteri** (`LUN_NOM_PAG` in [xstaz.h](../AlgLib/libinclude/xstaz.h#L44)):
oltre, la `strcpy` sfora nel campo descrizione che segue nella struttura. Dal
2026-09 il programma se ne accorge e si ferma con un messaggio, invece di
produrre un file dal nome assurdo.

Due cose da sapere, che lo distinguono dal gemello:

- **non produce `r02.dat`.** Quel file serve solo a `xstaz` e a `net_monit`, e lo
  fa `compstaz`. `convstaz` lo scriveva per eredità, per giunta sbagliato (vedi
  il punto seguente): la scrittura è stata rimossa, così lanciarlo nella
  directory del simulatore non può più sovrascrivere quello buono.
- **non risolve le variabili, e non è un problema.** `check_model`,
  `check_input` e `check_output` sono stub che ritornano sempre indice 1 — le
  implementazioni vere stanno sotto `#ifdef ORIGINALE`, che il makefile non
  definisce ([checkvar.c:176](conv_staz/checkvar.c#L176)). Nelle pagine però
  `convstaz` scrive riferimenti **simbolici**, non indici:

  ```
  *1w1c.varInputCambioColore1: U1094FSL BLOCCO R_PCS NOP 1.0 0.0 ---
  ```

  che vengono risolti dopo, in compilazione, contro `variabili.rtf`, esattamente
  come per una pagina disegnata a mano. Gli indici fasulli finivano solo nel
  `r02.dat`, che infatti non viene più scritto. Conseguenza pratica: `convstaz`
  gira ovunque col solo `r01.dat`, mentre `compstaz` pretende `variabili.rtf`
  nella directory e senza si ferma.

#### I colori della pagina generata

I tre sfondi sono quelli del faceplate `xstaz`
([xstaz.c:139-143](../Alg_rt/grafica/xstaz/xstaz.c#L139-L143)), perché la pagina
MMI deve somigliare al pannello che l'operatore conosce:

| `conv_mmi.h` | valore | in `xstaz` |
|---|---|---|
| `SFONDO_WINDOW` | `#cce5e5` verdino pallido | `sfondo_window` |
| `SFONDO_STAZ` | `#d5e5d5` verdino grigio | `sfondo_staz` |
| `SFONDO_LABEL` | `#cce5cb` verdino chiaro | `sfondo_label` |
| `SFONDO_DISPLAY` | `#101c2c` blu notte | *non esiste* |

`SFONDO_DISPLAY` è l'unico che si stacca dall'originale, e serve perché le due
viste **colorano le cifre in modo diverso**. In `xstaz` il display è una
`XmLabel` con foreground fisso: nero, sempre
([gdisplay.c:111-120](../Alg_rt/grafica/xstaz/gdisplay.c#L111-L120)). Nell'MMI è
un `XlIndic`, e il colore delle cifre lo sceglie
[`XlFlagToGC`](lib/Xl/XlWidgetUtil.c#L855) in base ai flag del punto — fuori
scansione, automatico, fuori attendibilità, stimato, e le quattro soglie
`hs`/`ls`/`hc`/`lc`. Il colore **è** l'informazione, e quella tavolozza nasce
satura, per fondo scuro: su un fondo chiaro il giallo "stimato" dà un rapporto di
contrasto di 1,2, cioè è invisibile. Da qui la finestrella scura, che per giunta
somiglia al display dello strumento vero.

Le risorse `colore*1` le scrive `ScriviCambioColore` in due tarature, scelte in
base allo sfondo dell'oggetto:

| stato | risorsa | su fondo scuro (display) | su fondo chiaro (barra) |
|---|---|---|---|
| stimato | `coloreStimato1` | `#ffff00` | `#7a5c00` |
| in conduzione | `coloreBassoAlto1` | `#00ff00` | `#006400` |
| automatico | `coloreAutomatico1` | `#00ffff` | `#006a6a` |
| fuori scansione | `coloreFuoriScansione1` | `#6699ff` | `#0000b3` |
| non attendibile | `coloreFuoriAttendib1` | `#ff66ff` | `#8b008b` |
| fuori sicurezza | `coloreBassissimo1`, `coloreAltissimo1` | `#ff5555` | `#b30000` |

I due valori corretti rispetto ai default di Xl sono il blu e il rosso: `blue` su
fondo scuro darebbe 2,0 di contrasto e `red` 4,3.

#### `inheritBackground: 0`, senza il quale niente di tutto questo si vede

**Dichiarare `background` in una pagina MMI non basta.** La risorsa
`inheritBackground` vale `True` per default
([XlCore.c:154-161](lib/Xl/XlCore.c#L154-L161), idem
[XlManager.c:159-167](lib/Xl/XlManager.c#L159-L167)) e la `Initialize` del widget
copia il background del **padre** sopra quello letto dal file:

```c
/* se inhertiBackground == 1 setta il background del padre */
if(new->xlcore.inheritBackground == 1)
    {
    XtSetArg(arg[i],XmNbackground,&back_parent);i++;
    XtGetValues(XtParent(tnew),arg,i);
    new->core.background_pixel=back_parent;
    }
```

L'ereditarietà risale tutta la gerarchia — figli → composite → drawing area — e
la pagina finisce **uniformemente al colore di `drawing_background`**. Fino al
settembre 2026 `convstaz` non scriveva mai quella riga: dei suoi tre colori se ne
vedeva uno solo, e per la via indiretta dello sfondo pagina. Ora ogni sfondo lo
scrive `ScriviSfondoFiglio` ([conv_staz/conv_mmi.c](conv_staz/conv_mmi.c)), che
emette sempre le due righe insieme. Le pagine disegnate con `legopc` non hanno
mai avuto il problema perché il CAD scrive `inheritBackground` per ogni oggetto.

### `mkstaz` — da un template e una lista

[Alg_mmi/procedure/mkstaz.sh](procedure/mkstaz.sh) genera pagine **in serie**
sostituendo segnaposto dentro un modello. Servono tre file:

| file | dove |
|---|---|
| `<base>.list` | directory di lavoro — l'elenco delle istanze |
| `<base>.templ` | `$HOME/legocad/libut_mmi/` — la pagina modello |
| `<base>.tebkg` | `$HOME/legocad/libut_mmi/` — lo sfondo modello |

La lista è posizionale: **la prima riga contiene i segnaposto**, le righe
successive i valori. Esempio reale da `malDES.list`:

```
@#L@TITLE       @#L@REFSTA              @#V@SACT
F_0LCA20AA001   O_0LCA20AA001           0208M_-XREG08M_-LEGOCAD
```

Per ogni riga di dati `mkstaz` fa una `sed` dei segnaposto sul template (fino a
20 colonne) e scrive **`<primo valore>.pag`**, più una copia del `.tebkg` come
`.bkg`. Nei valori i trattini diventano spazi, così si scrivono stringhe con
spazi in un campo separato da spazi.

Il primo valore, che dà il nome al file, deve rispettare i **prefissi** che
`kMakeGlobpages` cerca in `$MMI_WINDIR`: `O_` operating window, `F_` finestra di
malfunzione, `M_` plant display ([kMakeGlobpages.sh:522](../kprocedure/kMakeGlobpages.sh#L522)).

Due procedure lo pilotano:

- [`kcreastaz`](../kprocedure/kcreastaz.sh) — la versione spiccia: `mkstaz` su
  tutti i `mal*.list` e `N_win*.list` della directory corrente, log locale,
  nessun controllo;
- [`kOw`](../kprocedure/kOw.sh) — quella completa: verifica che le liste non
  contengano `/` (romperebbe i nomi di file), poi `mkstaz` per le operating
  window, `mkdtw` per le finestre di dettaglio e `mkstaz2 mgw` per la finestra
  di gruppo delle malfunzioni, scrivendo nei log standard.

Si lavora in `$KWIN`, cioè `$KSIM/o_win`, la stessa directory che `al_sim.conf`
dichiara come `MMI_WINDIR`.

---

### Portare i faceplate nell'MMI: `kStazPages`

`convstaz` produce le pagine, ma per anni nessuno andava a prenderle: la raccolta
guarda solo dentro `$MMI_WINDIR` (`o_win`) i file con prefisso `O_`, `F_`, `M_`, e
le directory delle task `R` tramite `filespec`. Una `POWCTRL.pag` nella directory
del `r01.dat` non rientrava in nessuno dei due criteri.

Il ponte è [`kStazPages`](../kprocedure/kStazPages.sh): cerca un `r01.dat` in
`$KSIM` e nelle directory delle task `R` dichiarate in `al_sim.conf` (risolte via
`BASEPATH`), lancia `convstaz -d` in una directory temporanea e deposita pagina e
sfondo in `$KWIN` **anteponendo `O_`** al nome — lasciandolo com'è se comincia già
per `O_`, `F_` o `M_`.

Il prefisso non consuma gli 8 caratteri del campo `NOME`: a valle i limiti sono
molto più larghi (`MAXCHAR_PAGNAME` 60, `L_NOME_PAGINE` 200), e in un impianto
reale i nomi di pagina arrivano a 24 caratteri.

Da lì in poi vale la catena che già esisteva per le operating window:

```
kStazPages           →  $KWIN/O_<nome>.pag + O_<nome>.bkg
kWinContext          →  $KWIN/Context.ctx           (gemello di kGlobContext, per o_win)
kconfig -c compall   →  O_<nome>.rtf                (risolve le variabili contro variabili.rtf)
kCollect             →  kMakeGlobpages collega gli O_*.rtf in globpages
```

Esempio verificato su `SLaurent_0`: da un `r01.dat` con una pagina `POWCTRL` si
ottiene `o_win/O_POWCTRL.pag` + `.bkg`, un `Context.ctx` con descrizione,
`refresh_freq` e `top_tipo: Stazioni`, un `.rtf` di 18 KB compilato senza errori
con i riferimenti risolti (`compiled: # … | 2 11977 1 0 4 …`), e infine i symlink
in `globpages`.

> **`$?` non dice se `convstaz` è andato bene.** Il programma esce con
> `exit(puts("\nFine corretta COMPSTAZ"))`, cioè con il numero di caratteri
> stampati (24). `kStazPages` verifica l'esito guardando se ha davvero prodotto
> pagine, e lo fa con `ls`, non con `[ -f <glob> ]`: senza corrispondenze il
> pattern resterebbe letterale.

---

## 6. La catena operativa in LegoPST (kprocedure)

Nell'uso reale non si tocca il Context a mano: lo generano le procedure.

```
$KSIM/globpages/          ← KPAGES: le pagine, condivise  (.pag .rtf .bkg + Context.ctx)
$KSIM/globpages_a/        ← una directory per istanza MMI: solo il suo Context.ctx
$KSIM/globpages_b/
$KSIM/globpages_c/
$KSIM/globpages_d/
$KSIM/kMmi.cfg            ← quali istanze esistono, su quale host e per quale utente
```

| passo | procedura | cosa fa |
|---|---|---|
| 0 | [Alg_env.sh:336](../Alg_env.sh#L336) | definisce `KPAGES=$KSIM/globpages` |
| 1 | [kMakeGlobpages](../kprocedure/kMakeGlobpages.sh) | popola `$KPAGES` a partire dalle task di regolazione dichiarate in `al_sim.conf`: riloca i `.rtf` con posizione e offset del modello e scrive un primo `Context.ctx`. Quali pagine appartengono a quale task lo dice la colonna `filespec` ([docs/AL_SIM_CONF.md](../docs/AL_SIM_CONF.md)) |
| 2 | [kGlobContext](../kprocedure/kGlobContext.sh) | `cd $KPAGES`, elenca i `*.pag` con `ls \| grep pag$`, estrae da ciascuno gli attributi e **rigenera** `Context.ctx` (con `*pages: $KPAGES`, `*simulator: $KSIM`, le librerie in `$HOME/legocad/libut_reg/libreg`). Il vecchio finisce in `Context.ctx.kold` |
| 3 | [kMmiConfig](../kprocedure/kMmiConfig.sh) | `rm -rf ${KPAGES}_*`; per ogni riga di `kMmi.cfg` relativa a questo host/utente crea `${KPAGES}_<id>/Context.ctx`, copia del globale con il solo `*hostNameS` completato con l'id SCADA |
| 4 | [kMmi](../kprocedure/kMmi.sh) `[id]` | mostra le istanze disponibili, poi `cd ${KPAGES}_<id>`, imposta `MMI_ULEVEL`, `. kuser 77<idScada>` e lancia `mmi &` |

Formato di `$KSIM/kMmi.cfg` — un'istanza per riga, campi separati da `;`:

```
a;1;I;RIC326282;antonio
b;2;I;RIC326282;antonio
c;3;O;RIC326282;antonio
d;4;O;RIC326282;antonio
 │ │ │     │        └── utente
 │ │ │     └─────────── host
 │ │ └───────────────── tipo: I=Instructor  O=Operator  S=Only Scada  X=Super instructor
 │ └─────────────────── id SCADA (1..4): diventa SHR_USR_KEY=77<id>0000 via kuser
 └───────────────────── identificatore dell'istanza, è il nome della directory globpages_<id>
```

Il tipo determina `MMI_ULEVEL` (0 operatore, 1 istruttore, 2 super), che finisce
nella classe dei widget `XlCore`/`XlManager` come livello di privilegio
([XlCore.c:783](lib/Xl/XlCore.c#L783)).

Conseguenza importante dello schema: **le pagine sono condivise** (una sola copia
in `$KPAGES`), mentre ogni istanza MMI ha una directory propria che contiene solo
il suo Context — e dove finiscono i file che `mmi` scrive nel cwd.

### File che `mmi` crea nella directory da cui parte

| file | a cosa serve |
|---|---|
| `Context.ctx_rtf` | cache binaria del Context, per non rileggere tutto ad ogni avvio |
| `ApplDb.res` | database risorse dell'applicazione ([topLevelShellMain.c:411](run_time/topLevelShellMain.c#L411)) |
| `LegoMMI.log` | log; `kMmi` aspetta che compaia per dichiarare l'MMI avviato |

La cache viene rifatta se `Context.ctx_rtf` è più vecchio di `Context.ctx`
([other_mmi.c:1376](run_time/other_mmi.c#L1376)). `NOCONTEXTRTF=YES` la fa usare
comunque, anche se scaduta.

---

## 7. Due configurazioni: MMI locale o MMI client/SCADA

`mmi` decide da solo come collegarsi ai punti, guardando **`*hostName` e
`*hostNameS`** del Context ([OlDatabasePunti.c:208](lib/Ol/OlDatabasePunti.c#L208)):

| `*hostName` | `*hostNameS` | tipo | come si collega |
|---|---|---|---|
| vuoto | vuoto | `DB_XLSIMUL` | **locale**: si attacca direttamente alla SHM del simulatore con `SHR_USR_KEY` |
| `<host> [cod]` | vuoto | `DB_XLSIMUL_CLIENT` | lancia `client_mmi` verso `<host>` |
| vuoto | `<host> [cod]` | `DB_XLSCADA_CLIENT` | lancia `client_scada` verso `<host>` |
| valorizzati entrambi | | `DB_XLCLIENTS` | entrambi i client |

Il commento nel codice è esplicito: *"Se non scrivo niente nel Simulator on e
Scada on del Context allora mmi in locale con simulatore"*.

### Se il simulatore gira sulla stessa macchina

È il caso normale in sviluppo, ed è la configurazione dei Context di produzione
(`hostName` e `hostNameS` vuoti). Si lancia così, con l'ambiente standard del
simulatore:

```sh
cd $KPAGES && mmi &
```

Lo stesso lancio, con la scelta automatica della directory, e' il pulsante **mmi**
del selettore [`lghmi`](../Alg_legopc/LGHMI.md#il-pulsante-mmi).

**`kMmi` non serve e anzi non funziona**, per due motivi indipendenti:

1. **`kMmiConfig` riempie `*hostNameS`.** La sua `sed` aggiunge l'id SCADA al
   campo: partendo da un `*hostNameS:` vuoto produce `*hostNameS: 2`. `mmi` lo
   interpreta come nome di host, `gethostbyname("2")` non fallisce (lo risolve
   come indirizzo numerico), quindi passa a `DB_XLSCADA_CLIENT` e lancia
   `client_scada`. Non essendoci nessun server SCADA, si blocca:

   ```
   Process client_scada started [2] [1]
   TIMEOUT SCADUTO pid= 138246
   ATTIVA CLIENT: Errore ricevimento msg ACK su richiesta pag.TEMPO per client_scada
   ```

2. **`kMmi` esegue `. kuser 77<idScada>`**, che riscrive `SHR_USR_KEY`
   (`kuser 772` → `7720000`). Il simulatore locale usa la chiave del suo
   ambiente (es. `10000000`, visibile con `ipcs -m`), quindi anche svuotando
   `*hostNameS` l'MMI cercherebbe segmenti inesistenti.

`kMmi`/`kMmiConfig` implementano la strada client/SCADA: hanno senso quando
esiste davvero un server (`server_mmi`/`demone_mmi`) e più postazioni operatore
con id SCADA distinti.

### Cosa serve in più per la strada SCADA

- `*hostName`/`*hostNameS` con il **nome host** (più l'eventuale codice):
  `*hostNameS: RIC326282 2`;
- `fnomi.rtf` nella directory `*simulator`, il database "light" della topologia:
  era un link creato da `kScadaInit` a partire da `$KFILEOP/fnomi.rtf`, con i
  programmi del sottosistema `scada/` (`PointInst`, `ExpRtf`, `ToRtf`). Il
  sottosistema e `kScadaInit` sono stati **rimossi** a ottobre 2026 (vedi
  [docs/COMPONENTI_RIMOSSI.md](../docs/COMPONENTI_RIMOSSI.md)): oggi quel file non
  si genera più. Se manca, `mmi` non si ferma ma segnala

  ```
  XlWarning [OlDatabaseTopologia]: Impossibile aprire il file fnomi.rtf ...
  ERRORE nella costruzione dell'oggetto OlTree!!!
  ```

  e resta senza albero gerarchico (in locale è innocuo).

---

## 8. Diagnostica

### `mmi` parte e si blocca su `TIMEOUT SCADUTO`

```
Process client_scada started [2] [1]
TIMEOUT SCADUTO pid= ...
ATTIVA CLIENT: Errore ricevimento msg ACK su richiesta pag.TEMPO per client_scada
```

L'MMI sta cercando un server SCADA che non esiste, perché `*hostNameS` non è
vuoto. Su un simulatore locale va lanciato senza `kMmi`, con `cd $KPAGES && mmi &`
(§7).

### Nella directory del simulatore compare un file chiamato `*mf`

Un symlink penzolante con l'asterisco nel nome, che punta a `$KWIN/*mf`. Lo
creava [`kCollect`](../kprocedure/kCollect.sh), che collega in `$KSIM` i file
dell'interfaccia malfunzioni con `ln -fs $KWIN/*mf $KSIM`: se in `o_win` non c'è
nessun `*mf` — interfaccia malfunzioni non configurata — il glob resta
**letterale** e `ln` crea il link con quel nome.

Si toglie con le virgolette, altrimenti la shell espande il pattern:

```sh
cd $KSIM && rm -- '*mf'
```

Da settembre 2026 la riga è protetta da un `if ls … > /dev/null 2>&1` e al suo
posto compare un warning nel log. I file veri sono `component.mf`,
`malf_set.mf` e `tipo_comp_malf.mf`, che porta in `$KWIN` la procedura
`kExport`.

### `kMmi` mostra una lista vuota

```
Mmi available on RIC326282 :
Selection :
```

`kMmi` filtra le righe di `$KSIM/kMmi.cfg` confrontandole con `$USER`
([kMmi.sh:42](../kprocedure/kMmi.sh#L42)). Se il simulatore arriva da un'altra
macchina, le righe portano l'utente di quella e vengono scartate tutte. Si
corregge riscrivendo `kMmi.cfg` con l'utente corrente e rilanciando
`kMmiConfig` per ricreare le `globpages_*`; `lgrun -d` e `lgdemo` lo fanno da
sé per le demo.

Da sapere: **decide solo l'utente**, l'host non conta. Fino a ottobre 2026 il
confronto era scritto `[ "$MmiHost" = "$HOST" ] & [ "$MmiUser" = "$USER" ]`, con
una sola `&` che mandava in background il primo test: contava già solo
l'utente, ma per sbaglio. Ora lo dice il codice (`kMmi`, `kMmiConfig`, `kClean`,
`kCleanLoc`). Non si è passati a `&&` di proposito: l'host di `kMmi.cfg` è quello
della macchina su cui è stato scritto, e nel container (hostname nuovo a ogni
avvio) o in una demo confezionata altrove non coinciderebbe mai.

### `SORRY : You have to answer with an available mmi identifier`

L'id scelto non è in `kMmi.cfg`, oppure manca la directory `${KPAGES}_<id>`:
va rilanciato `kMmiConfig`.

### Una pagina nuova non compare nell'elenco

L'elenco viene da `*page_list` nel Context, non dal filesystem: dopo aver aggiunto
un `.pag` servono `kGlobContext` e poi `kMmiConfig`.

### La pagina si apre ma è quella vecchia

Cache: cancellare `Context.ctx_rtf` nella directory dell'istanza, o verificare che
`NOCONTEXTRTF` non sia impostata a `YES`.

### `Error on GetFileDatabase Context.ctx - Exit.`

`mmi` è stato lanciato da una directory che non contiene il Context. Va lanciato
da `${KPAGES}_<id>` (lo fa `kMmi`) o con `-Context <file>`.

---

## 9. Tranelli

- **Non esiste un search path.** Una sola directory per categoria, presa dal
  Context. Se il path è sbagliato, l'MMI non prova nessuna alternativa.
- **`path_rel_to_abs` decide "è relativo?" cercando un punto qualsiasi nella
  stringa** ([utile.c:488](../AlgLib/libutil/utile.c#L488)). Un path relativo
  *senza* punto (`pagine`) resta relativo; va scritto `./pagine`. Un path
  assoluto che contiene un punto (`/home/x/sim.v2/pag`) prende il ramo
  "relativo": funziona, ma per caso.
- **`mmi` dipende dal cwd**, il configuratore no: il primo risolve `*pages: ./`
  rispetto alla directory da cui parte, il secondo rispetto alla directory del
  Context.
- **`kGlobContext` rigenera il Context dai soli `.pag` presenti.** In un
  simulatore consegnato con le sole pagine compilate (`.rtf`/`.bkg`) e senza
  sorgenti, produce `pag_num: 0` e cancella l'elenco: il Context precedente si
  recupera da `Context.ctx.kold`. In quel caso il Context non va rigenerato.
- **I Context ereditati portano path assoluti della macchina d'origine**
  (es. `/usr/users/milost64/...`): su un'altra macchina vanno rigenerati, non
  corretti voce per voce.
- **`kMmiConfig` scrive sempre l'id SCADA in `*hostNameS`**, anche quando il
  campo era vuoto: basta questo a far passare `mmi` dalla modalità locale a
  quella client/SCADA, con blocco su timeout (§7).
- **`background` da solo non colora niente**: `inheritBackground` vale 1 di
  default e la `Initialize` di `XlCore`/`XlManager` ci copia sopra lo sfondo del
  padre. Vanno scritte sempre le due righe insieme, altrimenti la pagina esce
  tutta del colore di `drawing_background` senza un solo messaggio d'errore (§5).
- **Solo l'editor conosce `iconlib_list`**, la lista di librerie di icone: è per
  la palette, non è un path di ricerca a run-time. Per le pagine una lista
  analoga non esiste.
- **Chiudere `config` dalla X della finestra lasciava il processo vivo**
  (corretto in ottobre 2026): la finestra principale è una `topLevelShell`
  figlia dell'`applicationShell` nascosta `configMom`, e con il default Motif
  (`XmNdeleteResponse = XmDESTROY`) la X distruggeva solo la finestra, mentre
  `XtAppMainLoop` continuava a girare. In `ps` restava un `config` in stato
  `Ss` per ogni chiusura. Ora la shell è `XmDO_NOTHING` e `WM_DELETE_WINDOW`
  richiama la callback di *File → Exit* (`chiusura_wm` in
  [topLevelShell.c](config/topLevelShell.c)), conferma e salvataggio del
  Context compresi. Vale per ogni shell Motif che non sia quella
  dell'applicazione: se la X deve chiudere il programma, va detto esplicitamente.
  La modifica è nel `.c`, non nel `topLevelShell.i` di UIM/X, che va tenuto
  allineato a mano se mai lo si rigenera.
