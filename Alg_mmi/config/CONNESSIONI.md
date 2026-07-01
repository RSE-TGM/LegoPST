# Collegare porte/variabili tra schemi in `config`

Promemoria della **modalità** e dei **tasti mouse/tastiera** per tracciare le
connessioni tra porte (e quindi variabili) negli schemi di regolazione, sia
all'interno di un singolo schema sia **tra più schemi** (pagine).

Ricostruito dai sorgenti:
[proc_SelPort / regPoliTable](othercnf.c) in `othercnf.c`,
[toggle Interface Mode](topLevelShell.c) in `topLevelShell.c`,
[translations delle porte](../lib/Xl/XlPort.c) in `Alg_mmi/lib/Xl/XlPort.c`.

## Due contesti: dentro uno schema vs tra schemi

| | Modalità | Cosa si connette |
|---|---|---|
| **Dentro un singolo schema** | normale (Interface Mode **OFF**) | solo le porte **interne** allo schema |
| **Tra schemi diversi** | **Interface Mode ON** | solo le porte **di interfaccia** (ponte tra pagine) |

La regola è imposta dal codice: una porta di interfaccia è connettibile **solo**
se Interface Mode è attivo, e in Interface Mode si connettono **solo** porte di
interfaccia ([othercnf.c `proc_SelPort`](othercnf.c)).

### Attivare/disattivare Interface Mode

- Voce di menù **"Interface Mode"** nel top-level di `config` (toggle
  `StateInterfaceMode`).
- **Richiede tutte le pagine chiuse**: se ci sono schemi aperti, `config`
  rifiuta con il messaggio *"Close all the page first."*.
- Flusso tipico per collegare tra schemi: **chiudi tutti gli schemi → attiva
  Interface Mode → riapri le pagine da collegare → traccia le connessioni tra le
  porte di interfaccia**.

## I tasti per tracciare una connessione

**1. Avvio** — **clic sinistro** (Btn1, **senza** Shift) sulla **prima porta**.
Seleziona il punto di partenza e attiva il disegno della connessione.
(`Seleziona()` → `PortSelect()` → `proc_SelPort` modo 1.)

**2. Durante il tracciamento** — la linea segue il puntatore (*rubber-band*):

| Tasto / mouse | Azione interna | Effetto |
|---|---|---|
| **Clic sinistro (Btn1)** o **Invio** | `second_point_conn()` | Fissa un **punto intermedio** (gomito); cliccando la **seconda porta** **completa** la connessione |
| **Clic centrale (Btn2)** o **clic destro (Btn3)** o **F11** | `end_draw_conn()` | **Termina / chiude** il tracciamento in corso |
| **F5** | `draw_delete()` | **Cancella** la connessione |
| **Frecce** ← → ↑ ↓ | `move_tasti()` | **Sposta** il punto corrente da tastiera (posizionamento fine) |
| **Movimento mouse** | `draw_draget()` | La linea insegue il puntatore |

**3. Completamento** — **clic sinistro sulla seconda porta**: registra la
connessione (`proc_SelPort` modo 2). In Interface Mode viene prima verificata la
compatibilità dell'interfaccia (`CreoNuovaInterfaccia`).

### In sintesi

- **Sinistro** = fai/prosegui la connessione: porta iniziale → eventuali gomiti
  → porta finale.
- **Centrale / destro / F11** = chiudi o annulla il tracciamento.
- **F5** = cancella.
- **Frecce** = rifinisci la posizione del punto.

## Altri tasti utili sulle porte (non connessione)

Dalle translation di default delle porte
([XlPort.c](../lib/Xl/XlPort.c)):

- **Clic sinistro** (senza Shift) = seleziona la porta (`Seleziona`) — è anche
  ciò che avvia/completa la connessione.
- **Shift + clic sinistro** = **aggiungi alla selezione** (`AddSelez`,
  multi-selezione) — **non** avvia una connessione.
- **Frecce** = sposta la porta/selezione.

## Note

- Le connessioni dello schema sono persistite nel file `Connessioni.reg`
  (contesto/task; le task di regolazione sono per convenzione i nomi `r_*` nella
  struttura `legocad` dell'utente).
- Per il copia/incolla del **testo** nei campi delle form vedi
  [COPIA_INCOLLA.md](COPIA_INCOLLA.md).
