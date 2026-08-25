# Development Environment

## Required Dependencies (Ubuntu/Debian)
```bash
# Motif and Tcl/Tk
sudo apt install libmrm4 tcl tk tix libmotif-dev

# X11 libraries  
sudo apt install libxmu-dev freeglut3-dev libxext-dev libxi-dev

# Other essentials
sudo apt install libbsd-dev libsqlite3-dev libgdbm-compat-dev
```

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
- Uses DCE threads compatibility layer
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
