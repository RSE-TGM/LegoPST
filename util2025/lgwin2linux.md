# lgwin2linux.py

Script Python per convertire applicazioni Legopc da formato **Windows** a formato **Linux**.

## Descrizione

Questo script converte le directory `libut`, `libgraph` e `models` di un'applicazione Legopc dal formato Windows al formato Linux, gestendo automaticamente:

- Conversione dei line-ending (CRLF → LF)
- Rinomina delle estensioni Fortran (`.for` → `.f`)
- Rinomina dei file di configurazione
- Copia dei file binari senza modifiche
- **Chiede conferma prima di procedere**
- **Produce un archivio tar.gz della struttura convertita**

## Uso

```bash
python lgwin2linux.py [sorgente] [destinazione] [-y]
```

### Opzioni

| Opzione | Descrizione |
|---------|-------------|
| `-h`, `--help` | Mostra il messaggio di aiuto ed esce |
| `-y`, `--yes` | Esegui senza chiedere conferma |

### Parametri

| Parametro | Obbligatorio | Default | Descrizione |
|-----------|--------------|---------|-------------|
| `sorgente` | No | `user` | Directory sorgente Windows |
| `destinazione` | No | `<sorgente>_legocad_linux` | Directory destinazione Linux |

### Esempi

```bash
# Mostra help
python lgwin2linux.py -h

# Conversione con valori default (user -> user_legocad_linux)
python lgwin2linux.py
# Output: user_legocad_linux/ + user_legocad_linux.tar.gz

# Conversione con input personalizzato (pippo -> pippo_legocad_linux)
python lgwin2linux.py pippo
# Output: pippo_legocad_linux/ + pippo_legocad_linux.tar.gz

# Conversione con input e output personalizzati
python lgwin2linux.py pippo pippo_linux
# Output: pippo_linux/ + pippo_linux.tar.gz

# Esegui senza conferma
python lgwin2linux.py -y
python lgwin2linux.py pippo -y
```

## Conferma Interattiva

Prima di eseguire la conversione, lo script mostra il comando con i nomi di input e output e chiede conferma:

```
Comando: python lgwin2linux.py user user_legocad_linux
  Input:  user/
  Output: user_legocad_linux/

Procedere? [s/N]:
```

Usare `-y` per saltare la conferma.

## Regole di Conversione

### Directory `libut`

| Regola | Descrizione |
|--------|-------------|
| Estensione Fortran | `*.for` → `*.f` |
| File moduli | `l_moduli.dat` → `lista_moduli.dat` |
| File ausiliari | `l_aux.dat` → `lista_complementari.dat` |
| File C/H | Copiati con stesso nome |
| Line-ending | CRLF → LF |
| File ignorati | `.bat`, `.lib`, `.obj`, `.exe`, `.dll` |

### Directory `libgraph`

| Regola | Descrizione |
|--------|-------------|
| Struttura | Copia ricorsiva di tutte le sottodirectory |
| File di testo | Conversione line-ending CRLF → LF |
| File binari | Copiati intatti (`.gif`, `.jpeg`, `.bmp`, `.ppm` raw) |
| File moduli | `l_moduli.dat` → `lista_moduli.dat` |
| File ausiliari | `l_aux.dat` → `lista_complementari.dat` |

### Directory `models` → Root destinazione

| Regola | Descrizione |
|--------|-------------|
| **Struttura** | `models/*` → `<dest>/*` (modelli direttamente nella root) |
| File trasferiti | Solo file di testo |
| Topologia | `*.tom` |
| Dati | `f01.dat`, `f14.dat` |
| Fortran | `*.for` → `*.f` |
| Directory escluse | `proc/` (mai trasferita) |
| Line-ending | CRLF → LF |

## Struttura Output

```
user_legocad_linux/
├── libut/
│   ├── *.f              (da *.for)
│   ├── *.c
│   ├── *.h
│   ├── lista_moduli.dat (da l_moduli.dat)
│   └── lista_complementari.dat (da l_aux.dat)
├── libgraph/
│   ├── [sottodirectory...]
│   └── [file testo/binari...]
├── collet/              ← NOTA: da models/collet (direttamente in root)
│   ├── collet.tom
│   ├── f01.dat
│   ├── f14.dat
│   └── foraus.f         (da foraus.for)
└── pum/                 ← NOTA: da models/pum (direttamente in root)
    └── ...
```

## Archivio Output

Al termine della conversione viene creato automaticamente un archivio **tar.gz** della directory prodotta:

```
user_legocad_linux.tar.gz
```

L'archivio contiene l'intera struttura della directory destinazione, pronto per essere trasferito su sistema Linux.

## Codici di Uscita

| Codice | Significato |
|--------|-------------|
| 0 | Conversione completata senza errori |
| 1 | Errori durante la conversione |

## Note

- Il rilevamento file binario/testo è automatico (basato sulla presenza di byte nulli)
- I file nella directory `proc/` non vengono mai trasferiti
- Le directory vuote vengono create ma senza contenuto
