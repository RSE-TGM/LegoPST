# `al_sim.conf` — come si compila

`al_sim.conf` è il file di **composizione** di un simulatore: sta in `$KSIM` e
dichiara di quali task è fatto, con che passo di integrazione girano, come si
chiamano le loro pagine MMI e con quali regole vanno collegate fra loro.

Non contiene dimensionamenti: quelli stanno nel file `Simulator`
(`MAX_CAMPIONI`, `NUM_VAR`, snapshot, backtrack). Vedi
[BUILD.md](BUILD.md#i-due-file-di-configurazione-da-non-confondere).

Lo installa `creasim` copiando [`util97/bin/al_sim.conf.example`](../util97/bin/al_sim.conf.example),
che va poi adattato a mano.

## Chi lo legge, e cosa ne ricava

| programma | comando utente | cosa ne ricava |
|---|---|---|
| `connex2` | `kConnex` | il file **`S01`**: elenco e tipo delle task, path, passi, e le **connessioni** ingresso/uscita fra task |
| `kMakeGlobpages` | `kMakeGlobpages` | le **pagine MMI**: riloca i `.rtf` delle task di regolazione e costruisce `globpages` + `Context.ctx` |
| `kCompile`, `kCheckRegoTask`, `kUpDateNavigation` | — | l'elenco delle **directory** delle task (terza colonna) |
| `kUpDateHostName`, `kWinContext` | — | gli host MMI/SCADA |

`connex2` legge `al_sim.conf` **dalla directory corrente** e scrive `S01.new` e
`connex2.out`: va lanciato da `$KSIM`, cosa di cui si occupa `kConnex`.

## Struttura del file

Tre parti, nell'ordine:

```
CHIAVE=valore          <- opzioni, una per riga, nessuno spazio attorno all'"="
                       <- riga vuota (facoltativa, per leggibilità)
#tipo nome  directory  filespec  tempo   descrizione
P     NPS   SLB1_NI2   -         0.100000 Nuclear Island System - proc.
```

Le righe che iniziano con `#` sono commenti — compresa la riga di intestazione
delle colonne, che è lì solo per chi legge.

## Le opzioni di composizione (le usa `connex2`)

| chiave | default | a cosa serve |
|---|---|---|
| `TITLE=` | vuoto | titolo del simulatore; finisce in testa all'`S01` ed è quello che vedi in `lghmi` e nell'MMI. Ammette spazi |
| `BASEPATH=` | `../../legocad` | radice da cui si risolvono le directory delle task, **relativa a `$KSIM`**. Con i simulatori in `$HOME/sked/<nome>` e i modelli in `$HOME/legocad`, il default è già giusto |
| `KEY=` | `@#K@` | marcatore che identifica le **tag di interconnessione** dentro i `f01.dat`. Da cambiare solo se il progetto usa un'altra convenzione |
| `HOST0=` | `OS host guest ` | stringa di stazione, ripetuta nell'`S01` per ogni task |
| `AUTOM_TO_PROC_CONN=` | `ENABLE` | consente le connessioni automazione (`R`) ↔ processo (`P`). Con `DISABLE` le task di regolazione restano scollegate dal processo |
| `PROC_TO_PROC_CONN=` | `ENABLE` | consente le connessioni processo ↔ processo. Con `DISABLE` due task `P` non si parlano |
| `INTERACTIVE=` | `YES` | messaggi di avanzamento a video durante l'elaborazione |
| `IGNORE_NUMBERSIGN=` | `NO` | con `YES` toglie il `#` finale dalla tag prima di confrontarla: serve quando le tag sono scritte `@#K@XYZ#` e vanno abbinate a `@#K@XYZ` |
| `S01_BM=` | vuoto | riga `BM` in coda all'`S01` |
| `S01_BI=` | vuoto | riga `BI` in coda all'`S01` |

## Le opzioni MMI (le usano gli script `k*`)

| chiave | default | a cosa serve |
|---|---|---|
| `MMI_GLBDIR=` | `globpages` | directory in cui `kMakeGlobpages` raccoglie le pagine |
| `MMI_WINDIR=` | `o_win` | directory delle operating window |
| `MMI_HOSTNAME=` | `localhost` | host del server MMI |
| `MMI_HOSTNAME_SCADA=` | `localhost` | host SCADA — **vedi l'avvertenza qui sotto** |
| `MMI_DISPLAY_LIST=` | vuoto | elenco dei display su cui l'MMI può aprire pagine |

> **`MMI_HOSTNAME_SCADA` va lasciata vuota su un simulatore locale.** È l'unica
> `MMI_*` che legge anche `connex2`, e finisce nel `Context.ctx` come
> `*hostNameS`. Se valorizzata, `mmi` conclude che deve collegarsi a uno SCADA,
> lancia `client_scada` e si blocca in timeout aspettando un server che non
> esiste. Il meccanismo è spiegato in
> [Alg_mmi/README.md](../Alg_mmi/README.md#7-due-configurazioni-mmi-locale-o-mmi-clientscada).

`kMakeGlobpages` fa `eval` di **ogni** riga che comincia per `MMI`, quindi da
`al_sim.conf` si possono sovrascrivere anche i suoi default interni:
`MMI_VARFIL=` (`variabili.edf`), `MMI_CTXFIL=` (`Context.ctx`), `MMI_PAGE=`
(`YES`; con `NO` salta del tutto l'elaborazione delle pagine).

## La tabella delle task

Sei campi separati da spazi o tabulazioni. L'ultimo, la descrizione, può
contenere spazi perché arriva fino a fine riga.

```
#tipo nome      directory    filespec time     description
P     HPS000NG  HPS000NG     -        0.500000 High Pressure System - NG15.000
R     HPSREG0   r_hpsreg0    H???     1.000000 High Pressure System - Regolation Task
N     GASI      n_gasi       -        1.000000 Prova N04
```

| # | campo | regole |
|---|---|---|
| 1 | **tipo** | `P` processo, `R` regolazione, `N` (variabili da `proc/n04.dat`). Dev'essere la prima cosa sulla riga, seguita da spazio |
| 2 | **nome** | nome della task nell'`S01`: è quello che compare in `lghmi`, nei log dello scheduler e nel Context |
| 3 | **directory** | directory del modello, **relativa a `BASEPATH`** (non a `$KSIM`) |
| 4 | **filespec** | pattern delle pagine MMI: `-` per `P` e `N`, un glob per le `R` — vedi la sezione dedicata |
| 5 | **tempo** | passo di integrazione in secondi (`0.100000`, `0.1`, `1.6`…) |
| 6 | **descrizione** | testo libero; finisce nell'`S01` accanto al nome |

Da dove `connex2` prende le variabili di interconnessione, secondo il tipo:

| tipo | file letto | ingressi | uscite |
|---|---|---|---|
| `P` | `<dir>/f01.dat` | righe con `--IN--` | righe con `--UA--` o `--US--` |
| `R` | `<dir>/f01.dat` | idem | idem |
| `N` | `<dir>/proc/n04.dat` | idem | idem |

In tutti i casi la riga conta solo se contiene la stringa di `KEY=`: la tag è ciò
che segue, e l'abbinamento fra ingresso di una task e uscita di un'altra si fa
per **tag uguale**.

## `filespec`: la colonna che sembra inutile e non lo è

Serve **solo alle righe `R`** ed è il **glob dei nomi delle pagine MMI** che
appartengono a quella task di regolazione.

Lo usa `kMakeGlobpages`: per ogni task `R` entra nella directory del modello e fa

```sh
ls -1 $filespec.rtf        # es. H???.rtf, oppure ????.rtf
```

e per ciascuna pagina trovata:

1. ricava da `variabili.edf` la **posizione** del modello di quella task e, con
   `al_punt_mod`, il suo **offset**;
2. riscrive gli indirizzi interni della pagina con `pagmod`;
3. controlla che esista anche il `.bkg` (se manca, cancella entrambi con un
   warning);
4. deposita la pagina corretta in `globpages` e ci collega il `.bkg`.

Le pagine MMI hanno nomi di **quattro caratteri**, e la convenzione del template
è che la prima lettera identifichi la task: `r_hpsreg0` → `H???`, `r_ipsreg0` →
`I???`, `r_csreg0` → `C???`.

**Regole pratiche**

- con **una sola** task di regolazione va bene `????`: prende tutte le pagine a
  quattro lettere e lascia fuori file come `variabili.rtf` o `stato_cr.rtf`, che
  pagine non sono;
- con **più** task `R`, ognuna deve avere un pattern che seleziona solo le sue
  pagine. Un pattern troppo largo fa rilocare la stessa pagina con l'indirizzo
  del modello sbagliato — la pagina si apre, ma mostra i valori di un'altra task;
- un pattern che non prende niente non dà errore: semplicemente quelle pagine non
  arrivano in `globpages` e nell'MMI non si vedono;
- sulle righe `P` e `N` si scrive `-`: è un segnaposto che tiene in colonna i
  campi successivi. `connex2` lo legge e lo butta via — nessun altro lo guarda.

Esempio verificato su `SLaurent_0`, che ha una sola task `R` con `????`: la
directory `r_SLB1_0` contiene 22 `.rtf`, di cui `variabili.rtf` e `stato_cr.rtf`
non hanno nome di quattro caratteri; il glob ne seleziona 20, e 20 sono le pagine
in `globpages`.

## Esempio completo

```
TITLE=SLaurent_0 - PWR 900 MWe Saint Laurent B1
BASEPATH=../../legocad
KEY=@#K@
HOST0=OS host guest
AUTOM_TO_PROC_CONN=ENABLE
PROC_TO_PROC_CONN=ENABLE
INTERACTIVE=YES
IGNORE_NUMBERSIGN=YES

MMI_GLBDIR=globpages
MMI_WINDIR=o_win
MMI_HOSTNAME=
MMI_HOSTNAME_SCADA=
MMI_DISPLAY_LIST=

#type name      directory    filespec: time     description
P     NPS       SLB1_NI2         -     0.100000  Nuclear Island System           - proc.
P     SSS       PWRN1PSS         -     0.100000  Steam Supply System & feedwater - proc.
R     R_PCS     r_SLB1_0       ????    0.1       Plant Contro System             - reg.
```

Due task di processo a 100 ms e una di regolazione a 100 ms; i modelli stanno in
`$HOME/legocad/SLB1_NI2`, `PWRN1PSS`, `r_SLB1_0`; l'MMI è locale (host vuoti).

## Dopo aver modificato il file

| hai cambiato | rilancia |
|---|---|
| task, directory, tempi, opzioni di connessione | **`kConnex`** — salva l'`S01` in `S01.kold`, esegue `connex2` e rigenera `$KSIM/S01`; gli errori finiscono in `$KLOG/kConnex.log` |
| `filespec`, `MMI_*` | **`kMakeGlobpages`** — ricostruisce `globpages` e il `Context.ctx` |

Il `Context.ctx` delle pagine può essere rigenerato anche da `kGlobContext`, ma
quello parte dai `.pag` presenti in `globpages`: su un simulatore consegnato con
le sole pagine compilate azzera l'elenco. Vedi
[Alg_mmi/README.md](../Alg_mmi/README.md).

## Errori tipici

- **`connex2: Errore apertura file ../../legocad/<dir>/f01.dat`** — `BASEPATH` o
  la terza colonna non puntano dove stanno i modelli, oppure la task non è stata
  compilata. Per le task `R` il `f01.dat` serve anche per scrivere l'intestazione
  dell'`S01`, quindi la mancanza è fatale.
- **Una task compare nell'`S01` ma non si collega a nessuno** — le tag non
  coincidono, oppure hai `AUTOM_TO_PROC_CONN=DISABLE` /
  `PROC_TO_PROC_CONN=DISABLE`, oppure serve `IGNORE_NUMBERSIGN=YES` perché le tag
  hanno il `#` finale. Il dettaglio degli abbinamenti sta in `connex2.out`
  (copiato in `$KLOG/kConnex.log`).
- **Le pagine non compaiono nell'MMI** — `filespec` non le seleziona, oppure
  manca il `.bkg` accanto all'`.rtf` e `kMakeGlobpages` le ha rimosse (il warning
  è nel suo log).
- **L'MMI si blocca in timeout su `client_scada`** — `MMI_HOSTNAME_SCADA`
  valorizzata su una macchina sola: va lasciata vuota.
- **Campi fuori colonna** — la tabella è posizionale: i primi cinque campi non
  possono contenere spazi. Solo la descrizione può.
