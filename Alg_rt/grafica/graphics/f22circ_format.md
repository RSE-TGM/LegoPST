# Formato binario `f22circ.dat` — guida al programma `graphics`

`f22circ.dat` è un **buffer circolare a finestra scorrevole** scritto da
`net_prepf22` ad ogni ciclo di simulazione e letto dal viewer `graphics`
(e da `XlCurve` in `Alg_mmi`).

Il nome passato a `f22_open_file()` è `f22circ` (senza estensione);
la funzione aggiunge `.dat` in apertura. Il file è binario, formato C
nativo (little-endian su x86_64, `sizeof(int)=4`, `sizeof(float)=4`).

---

## Layout complessivo del file

```
Byte 0
┌─────────────────────────────────────────────────────┐
│  HEADER_REGISTRAZIONI                               │  scritto da net_sked all'avvio
│    int  size_area_dati                              │
│    SIMULATOR {                                      │
│      int max_snap_shot    ← -num_snap      (=60)   │
│      int max_back_track   ← -num_bktk      (=30)   │
│      int max_campioni     ← -num_camp_cr   (=7200) │  ← capacità buffer f22
│      int num_var          ← -num_var_cr    (=1000) │  ← variabili totali in SHM
│      int max_pertur       ← -num_pert_active (=50) │
│      int spare_snap       ← -num_spare_forsnap (=1)│
│      int pert_clear       ← -clear_pert_bktk   (=0)│
│      int spare1, spare2, spare3                     │
│    }                                                │
│    NUMERI_MODELLI num_modelli[MAX_MODEL]            │
│    char area_spare[512]                             │
├─────────────────────────────────────────────────────┤  offset = sizeof(HEADER_REGISTRAZIONI)
│  F22CIRC_HD  (header circolare, aggiornato live)    │
│    int p_iniz       ← indice 1-based primo campione valido  │
│    int p_fine       ← indice 1-based ultimo campione scritto│
│    int num_campioni ← copia di max_campioni         │
│    int num_var_graf ← variabili grafiche selezionate│
│    int ore, minuti, secondi, giorno, mese, anno     │  timestamp apertura file
├─────────────────────────────────────────────────────┤
│  F22CIRC_VAR × num_var_graf   (dizionario variabili)│
│    char nomevar[9]   ← nome breve (es. "PCOLMANI") │
│    char descvar[71]  ← descrizione lunga            │
│  (ripetuto num_var_graf volte)                      │
├─────────────────────────────────────────────────────┤  ← "offheader"
│  CAMPIONE 1                                         │
│  CAMPIONE 2                                         │
│  ...                                                │
│  CAMPIONE num_campioni                              │
└─────────────────────────────────────────────────────┘
```

`offheader` è calcolato ovunque (writer e reader) come:

```c
offheader = sizeof(HEADER_REGISTRAZIONI)
          + sizeof(F22CIRC_HD)
          + sizeof(F22CIRC_VAR) * num_var_graf;
```

---

## Struttura di ogni campione

```c
// layout fisico di ogni slot nel buffer:
float tempo;               // F22CIRC_T — tempo simulato in secondi
float mis[num_var_graf];   // valori delle variabili grafiche
```

Dimensione:

```
size_campione = sizeof(F22CIRC_T) + num_var_graf × sizeof(float)
              = (num_var_graf + 1) × 4 byte
```

Offset del campione `i` (indice **1-based**):

```
offset(i) = offheader + (i - 1) × size_campione
```

---

## Semantica del buffer circolare

```
p_iniz == p_fine   →  buffer vuoto (nessun campione valido)
p_iniz <  p_fine   →  campioni contigui in [p_iniz .. p_fine], nessun wrap
p_iniz >  p_fine   →  buffer pieno, campioni wrappati:
                       validi: [p_iniz .. num_campioni] ++ [1 .. p_fine]
```

`net_prepf22` ad ogni ciclo:
1. incrementa `p_fine`
2. se `p_fine > num_campioni` → `p_fine = 1`, `file_full = 1`
3. se `file_full` → incrementa anche `p_iniz` (sovrascrive il campione più vecchio)
4. scrive il campione corrente (tempo + valori) alla posizione `p_fine`
5. aggiorna `F22CIRC_HD` a inizio file (`fseek` + `fwrite`)

`graphics` (in `f22_leggi_campioni`) **legge all'indietro** partendo da
`p_fine` verso `p_iniz`, raccoglie i campioni con `tempo > t_ultimo_letto`,
poi li riordina per tempo crescente con `qsort`.

---

## Parametri di dimensionamento

I valori vengono passati a `net_sked` sulla riga di comando e scritti in
`SIMULATOR` dentro `HEADER_REGISTRAZIONI`. I valori di default usati nel
bundle FMU (da `net_startup_headless.sh`) sono:

| Parametro CLI       | Campo `SIMULATOR`  | Default bundle | Significato                                      |
|---------------------|--------------------|----------------|--------------------------------------------------|
| `-num_camp_cr N`    | `max_campioni`     | 7200           | Slot nel buffer circolare (profondità f22circ)   |
| `-num_var_cr N`     | `num_var`          | 1000           | Variabili totali in SHM (non influisce su f22)   |
| `-num_snap N`       | `max_snap_shot`    | 60             | Numero massimo di snapshot                       |
| `-num_bktk N`       | `max_back_track`   | 30             | Posizioni di backtrack                           |
| `-num_pert_active N`| `max_pertur`       | 50             | Perturbazioni attive simultanee                  |
| `-num_spare_forsnap N`| `spare_snap`     | 1              | Slot spare per snapshot                          |
| `-clear_pert_bktk N`| `pert_clear`       | 0              | Reset perturbazioni al backtrack                 |

`num_var_graf` **non** è un parametro di avvio: viene determinato dalla
configurazione grafica della task (variabili selezionate dall'utente per
la visualizzazione, persistite in `f22_fgraf.edf`). È il numero effettivo
di variabili che `net_prepf22` campiona ad ogni ciclo e che occupa spazio
nel file.

### Dimensione fisica del file

```
sizeof(file) = offheader + num_campioni × size_campione
             = offheader + num_campioni × (num_var_graf + 1) × 4 byte

Esempio (default bundle, 50 variabili grafiche):
  size_campione = (50 + 1) × 4 = 204 byte
  area dati     = 7200 × 204   = ~1.4 MB
```

---

## Frequenza di campionamento

`net_prepf22` viene svegliato da `net_sked` ad ogni ciclo di simulazione
(`dt_sked`, tipicamente 1 s). `graphics.h` definisce `INC_SEC 2` come
intervallo atteso tra campioni **visualizzati** (decimazione lato viewer).

Con `num_campioni = 7200` e campionamento ogni 1 s la finestra copre
**7200 s = 2 ore** di simulazione in tempo reale.

---

## Flag `DF22_APPEND` — motivo del `rm -f` pre-avvio

Il dispatcher è compilato con `-DDF22_APPEND`: apre `f22circ.dat` in
modalità append (`O_WRONLY | O_CREAT | O_APPEND`) invece di troncare.
Questo permette di continuare una sessione esistente dopo un riavvio
del banco operatore interattivo.

**Conseguenza per la FMU headless**: se `f22circ.dat` sopravvive da un
run precedente nella `unzipdir`, all'avvio `net_sked` trova
`p_fine = N > 0` e interpreta il file come uno stato "da riprendere"
(restored). `SD_goup` non avanza il tempo → `fmi2DoStep` va in timeout
(status 3).

Per questo `net_startup_headless.sh` esegue:
```bash
rm -f f22circ.dat backtrack.dat 2>/dev/null || true
```
**prima** di avviare `dispatcher` + `net_sked`, azzerando la storia del
run precedente. Vedi `USAGE.md` §"Ciclo di vita di `f22circ.dat`".

---

## Sorgenti rilevanti

| File | Ruolo |
|------|-------|
| `AlgLib/libinclude/f22_circ.h` | Strutture `F22CIRC_HD`, `F22CIRC_VAR`, `F22CIRC_T`, prototipi API |
| `AlgLib/libinclude/sim_types.h` | `HEADER_REGISTRAZIONI`, `SIMULATOR`, `NUMERI_MODELLI` |
| `AlgLib/libsim/f22_func.c` | Implementazione API lettura: `f22_open_file`, `f22_leggo_header`, `f22_leggi_campioni`, `f22_leggo_nomi_var` |
| `Alg_rt/net_simula/net_prepf22_circ/pf_wrif22circ.c` | Writer: campionamento SHM → scrittura campione su file |
| `Alg_rt/grafica/graphics/graphics_io.c` | Integrazione API in `graphics`: `read_22dat_circGR`, `alloca_bufdati`, `read_nomi_circ` |
| `Alg_rt/grafica/graphics/graphics.h` | `INC_SEC`, `NUM_CAMP_Z*`, `S_GRAFICO` |
| `AlgLib/libinclude/param_f22.h` | `NUM_VAR`, `S_HEAD1_C`, `S_DATI`, `S_GRUPPO` (formato f22 lineare, non circolare) |

---

*Redatto 2026-05-12 — basato su analisi di `f22_func.c` e `pf_wrif22circ.c`.*
