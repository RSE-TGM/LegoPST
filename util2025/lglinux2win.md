# lglinux2win.py

Script Python per convertire applicazioni Legopc da formato **Linux** a formato **Windows**.

## Descrizione

Questo script converte le directory `libut`, `libgraph` e i modelli di un'applicazione Legopc dal formato Linux al formato Windows, gestendo automaticamente:

- Conversione dei line-ending (LF → CRLF)
- Rinomina delle estensioni Fortran (`.f` → `.for`)
- Rinomina dei file di configurazione
- Copia dei file binari senza modifiche
- **Creazione della directory `models` con i modelli**
- **Chiede conferma prima di procedere**
- **Produce un archivio zip della struttura convertita**

## Struttura Sorgente

```
legocad/
├── libut/        → <dest>/libut
├── libgraph/     → <dest>/libgraph
├── collet/       → <dest>/models/collet (modello)
└── pum/          → <dest>/models/pum (modello)
```

## Uso

```bash
python lglinux2win.py [sorgente] [destinazione] [-y]
```

### Opzioni

| Opzione | Descrizione |
|---------|-------------|
| `-h`, `--help` | Mostra il messaggio di aiuto ed esce |
| `-y`, `--yes` | Esegui senza chiedere conferma |

### Parametri

| Parametro | Obbligatorio | Default | Descrizione |
|-----------|--------------|---------|-------------|
| `sorgente` | No | `legocad` | Directory sorgente |
| `destinazione` | No | `<sorgente>_user_win` | Directory destinazione Windows |

### Esempi

```bash
# Mostra help
python lglinux2win.py -h

# Conversione con valori default (legocad -> legocad_user_win)
python lglinux2win.py
# Output: legocad_user_win/ + legocad_user_win.zip

# Conversione con input personalizzato (pippo -> pippo_user_win)
python lglinux2win.py pippo
# Output: pippo_user_win/ + pippo_user_win.zip

# Conversione con input e output personalizzati
python lglinux2win.py pippo pippo_win
# Output: pippo_win/ + pippo_win.zip

# Esegui senza conferma
python lglinux2win.py -y
python lglinux2win.py pippo -y
```

## Conferma Interattiva

Prima di eseguire la conversione, lo script mostra il comando con i nomi di input e output e chiede conferma:

```
Comando: python lglinux2win.py legocad legocad_user_win
  Input:  legocad/
  Output: legocad_user_win/

Procedere? [s/N]:
```

Usare `-y` per saltare la conferma.

## Regole di Conversione

### Directory `libut`

| Regola | Descrizione |
|--------|-------------|
| Estensione Fortran | `*.f` → `*.for` |
| File moduli | `lista_moduli.dat` → `l_moduli.dat` |
| File ausiliari | `lista_complementari.dat` → `l_aux.dat` |
| File C/H | Copiati con stesso nome |
| Line-ending | LF → CRLF |
| File ignorati | `.o`, `.so`, `.a`, `.sh` |

### Directory `libgraph`

| Regola | Descrizione |
|--------|-------------|
| Struttura | Copia ricorsiva di tutte le sottodirectory |
| File di testo | Conversione line-ending LF → CRLF |
| File binari | Copiati intatti (`.gif`, `.jpeg`, `.bmp`, `.ppm` raw) |
| File moduli | `lista_moduli.dat` → `l_moduli.dat` |
| File ausiliari | `lista_complementari.dat` → `l_aux.dat` |

### Directory modelli → `models/<modello>`

| Regola | Descrizione |
|--------|-------------|
| Sorgente | Ogni directory in sorgente (esclusi `libut`, `libgraph`) |
| Destinazione | `<dest>/models/<modello>` |
| File trasferiti | Solo file di testo |
| Topologia | `*.tom` |
| Dati | `f01.dat`, `f14.dat` |
| Fortran | `*.f` → `*.for` |
| Directory escluse | `proc/` (mai trasferita) |
| Line-ending | LF → CRLF |

## Struttura Output

```
legocad_user_win/
├── libut/
│   ├── *.for            (da *.f)
│   ├── *.c
│   ├── *.h
│   ├── l_moduli.dat     (da lista_moduli.dat)
│   └── l_aux.dat        (da lista_complementari.dat)
├── libgraph/
│   ├── [sottodirectory...]
│   └── [file testo/binari...]
└── models/
    ├── collet/
    │   ├── collet.tom
    │   ├── f01.dat
    │   ├── f14.dat
    │   └── foraus.for   (da foraus.f)
    └── pum/
        └── ...
```

## Archivio Output

Al termine della conversione viene creato automaticamente un archivio **zip** della directory prodotta:

```
legocad_user_win.zip
```

L'archivio contiene l'intera struttura della directory destinazione, pronto per essere utilizzato su sistema Windows.

## Codici di Uscita

| Codice | Significato |
|--------|-------------|
| 0 | Conversione completata senza errori |
| 1 | Errori durante la conversione |

## Differenze rispetto a lgwin2linux.py

| Aspetto | lgwin2linux.py | lglinux2win.py |
|---------|----------------|----------------|
| Direzione | Windows → Linux | Linux → Windows |
| Line-ending | CRLF → LF | LF → CRLF |
| Fortran | `.for` → `.f` | `.f` → `.for` |
| Input default | `user` | `legocad` |
| Output default | `<input>_legocad_linux` | `<input>_user_win` |
| Sorgente modelli | `<src>/models/*` | `<src>/*` (esclusi libut, libgraph) |
| Destinazione modelli | `<dest>/*` (root) | `<dest>/models/*` |
| File ignorati libut | `.bat`, `.lib`, `.obj`, `.exe`, `.dll` | `.o`, `.so`, `.a`, `.sh` |

## Note

- Il rilevamento file binario/testo è automatico (basato sulla presenza di byte nulli)
- I file nella directory `proc/` non vengono mai trasferiti
- Le directory vuote vengono create ma senza contenuto
- La conversione LF → CRLF evita la doppia conversione normalizzando prima a LF
