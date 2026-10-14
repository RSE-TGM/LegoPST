# Development Environment

## Required Dependencies (Ubuntu/Debian)
```bash
# Motif and Tcl/Tk
sudo apt install libmrm4 tcl tk tix libmotif-dev

# X11 libraries  
sudo apt install libxmu-dev freeglut3-dev libxext-dev libxi-dev

# Other essentials
sudo apt install libbsd-dev libsqlite3-dev libgdbm-compat-dev

# Only to REGENERATE the station images of the faceplate builder
# (they are committed, so normal use does not need these)
sudo apt install imagemagick xvfb
```

The Fedora/WSL list, which is the one kept up to date, is in
[README.md](README.md); `imagemagick` also gives `legopc` and
`draw2gr` their PNG export.

## gdbm: solo la libreria di sistema

I componenti che usano gdbm passano dalla sua interfaccia `ndbm` e si
compilano contro la gdbm **di sistema**, `-lgdbm_compat -lgdbm` (cioè
`libgdbm.so.6`): basta il pacchetto di sviluppo, `gdbm-devel` su Fedora,
`libgdbm-compat-dev` su Debian/Ubuntu.

| Componente | Cosa fa con gdbm |
|---|---|
| `util97/pagmod/pagcompiler.c` | compilatore di risorse (`.pag` → `.rtf`) della pipeline MMI: apre il database `edfdb` e legge con `dbm_fetch` gli attributi dei widget |
| `kutil/kReadDB.c` | legge un valore dati il nome del database e la chiave, con i tempi in millisecondi |
| `util97/dbutil/dbmrea.c` | stampa tutto il database, coppia per coppia |
| `util97/dbutil/dbmins.c` | inserisce un record |
| `util97/dbutil/dbmins_mul.c` | inserisce più record, letti come `chiave valore` da stdin |
| `util97/dbutil/dbmftc.c` | legge un valore dati database e chiave |
| `util97/dbutil/dbmftc2.c` | come `dbmftc`, con i tempi |
| `util97/dbutil/dbm2sql.c` | converte un database gdbm in SQLite3 (`-lsqlite3`) |

Funzioni usate: `dbm_open`, `dbm_close`, `dbm_fetch`, `dbm_store`,
`dbm_firstkey`, `dbm_nextkey`, `dbm_error`, `dbm_clearerr`.

Il database `CAI_VAR_DB` viene creato e riletto a runtime dalla stessa libreria
(`leggi_cai_var.sh`, `elab_cai_var.sh`), quindi non c'è un formato su disco da
rispettare.

**La vecchia `libgdbm.so.2` non serve più.** Fino al 2026 il repository aveva
una directory `gdbm-install/` che la installava in `/usr/lib64` da un RPM di
Fedora 12, per binari precompilati collegati a quella versione; anche il
Dockerfile la eseguiva. Ricompilati dai sorgenti, nessun eseguibile la usa più
(verificato con `ldd` su tutte le directory `bin` il 2026-10-14): directory e
passo del Dockerfile sono stati tolti. Servirebbe soltanto per far girare un
binario vecchio importato già compilato, invece di ricompilarlo. Su una
macchina dove era stata installata a mano può restare: nessuno la carica.

## Compiler Configuration
- **C Compiler**: gcc with `-fcommon` flag for multiple definitions
- **Fortran Compiler**: gfortran with legacy support
- **Key Flags**: `-fno-second-underscore -std=legacy -finit-local-zero`

## Directory Structure Conventions
- All makefiles use `Makefile.mk` naming
- Libraries built as static archives (`.a` files)
- Source organization by functional area
- Shared includes in `libinclude/` directories

## Threading and IPC
- Uses POSIX threads (pthreads); the old DCE threads layer is no longer built
- Extensive shared memory usage
- Message queues for inter-process communication
- Semaphores for synchronization

## WSLg: puntatore che sparisce, e come recuperarlo senza chiudere WSL

Su WSLg il server X (Xwayland) e il compositor **non girano nella tua distro**:
stanno nella distro di sistema `wslg`, e da qui se ne vede solo il socket
(`/tmp/.X11-unix/X0`, montato da `/mnt/wslg`). Per questo `pgrep Xwayland` non
trova nulla e non c'è modo di riavviare "solo la grafica" dall'interno.

Quando il puntatore smette di rispondere, però, quasi mai il problema è il
server: è un **client X che ha lasciato attivo un grab**. È il meccanismo con
cui X consegna tutti gli eventi di mouse/tastiera a una sola finestra, e lo
usano i **menu Motif** — `xstaz`, `net_monit`, `banco`, `config` aprono popup
che afferrano il puntatore. Se quel client muore male, si blocca o resta con il
menu aperto, il grab non viene mai rilasciato e il mouse sembra sparito.

**La via d'uscita non richiede di chiudere WSL**: il terminale è una finestra
Windows e continua a funzionare anche a puntatore bloccato. Da lì:

```sh
xsblocca                  # dice se puntatore/tastiera sono bloccati
xsblocca -l               # elenca le finestre di primo livello
xsblocca -k <titolo>      # chiude i client di quelle col titolo indicato
```

`xsblocca -k` chiude la connessione X del client (come `xkill`): il server
rilascia i suoi grab e il puntatore torna. Non serve conoscere il PID — utile
perché le applicazioni Motif di LegoPST non pubblicano `_NET_WM_PID`.

Nell'elenco, i sospetti principali sono le finestre di menu, che hanno nomi tipo
`#menu#vmgr`, `#menu#vmgr#zoom`: sono popup aperti, ed è lì che vive il grab.

Se le finestre bloccate sono applicazioni LegoPST, la scorciatoia è **`killsim`**,
che le termina tutte in blocco (attenzione: su Linux cancella anche tutte le SHM
dell'utente, quindi non usarlo se hai altre sessioni di simulazione da salvare).

Sorgente: [util2025/xsblocca.c](util2025/xsblocca.c), binario in `util97/bin`
(già nel PATH del profilo). Si ricompila con:

```sh
cc -O2 -o util97/bin/xsblocca util2025/xsblocca.c -lX11
```
