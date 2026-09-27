# Installazione LegoPST via Docker

## L'immagine: `aguagliardi/legopst:2.0`

Si costruisce dalla radice del repository:

```sh
make -f Makefile.mk docker        # la costruisce
make -f Makefile.mk docker-push   # la costruisce e la pubblica
```

2,27 GB su disco, 447 pacchetti rpm, e un layer da 462 MB per la copia del
repository. Dentro ci sono `gcc`, `gfortran`, Motif, Tcl/Tk/Tix,
`ghostscript`, ImageMagick, `evince` e tutti i `-devel`: si compila e si lavora
come sulla macchina di sviluppo.

### Perché una sola

Fino al **2026-09-27** ce n'erano due, `legopst_multi` (tutti i pacchetti) e
`legopst_slim`. Misurando che cosa le distingueva davvero — 213 pacchetti e
529 MB — è venuto fuori che non c'era niente su cui LegoPST si appoggi:

| | |
|---|---|
| `gimp` | 32 pacchetti, **355 MB** (`suitesparse` 140, gimp 105, `openblas-openmp` 39). Sostituito con `mtpaint`: `LG_ICOEDITOR` in `Alg_env.sh` è una catena di ripieghi, serve *un* editor di icone, non quello |
| dipendenze deboli | circa 120 pacchetti: `systemd-udev`, `NetworkManager-libnm`, `pipewire`, `pulseaudio`, `tracker-miners`, `appstream`, la catena di perl. Un desktop, in un container che fa X11 e Motif |
| `git` → `git-core` | 70 pacchetti per 73 MB, di cui `git-core-doc` da solo 18 MB. Restano i comandi, si perdono i sottocomandi in perl (`git send-email`, `git svn`) |

Tenerle separate costava due `.dockerignore` da allineare, due bersagli nel
makefile, un'opzione in tre lanciatori — e una volta si sono disallineate per
davvero: una correzione provata sulla snella non era nella completa, che era
quella che si lanciava. Un'immagine sola non può avere quel problema.

Prima di rinunciare alla completa si è verificato dentro la snella che
`legopc`, `lghmi`, `xstaz` e `viewval` non abbiano **nessuna libreria
irrisolta** (`ldd`), che `compstaz` compili un `r01.dat` vero e che `legopc`,
`lghmi` e `xstaz` aprano davvero la finestra, in modalità standard **e** con
`--socat`.

**`evince` è rimasto**, benché si porti dietro `mesa-dri-drivers` e
`llvm-libs` (285 MB): `esporta.tcl` apre con `LG_PDFVIEWER` sia i PDF sia i
**PNG**, e un visualizzatore di soli PDF lascerebbe monco l'export PNG di
`legopc`. Rinunciando a quella funzione si scenderebbe di altri ~285 MB.

I `.o` e i `.a` **restano**: sembravano residui di compilazione, ma
`legocad/lego_big/lib/*.a` sono le librerie con cui `cad_crealg1` linka le task
quando si apre una HMI — senza, la HMI si apre col disegno cancellato.

### Cosa non entra nella copia, e perché conta per il push

La `COPY` del repository è **un solo layer**, e un layer è indivisibile: cambia
un file e `docker push` lo rispedisce intero, `lgdock --pull` lo riscarica
intero. Perciò si tiene fuori ciò che là dentro non serve a nessuno
([`Dockerfile_LegoPST.dockerignore`](Dockerfile_LegoPST.dockerignore)):

| | |
|---|---|
| `.git` | **631 MB** di storia che nessuno consulta dentro un container |
| `.aider*` | **41 MB** di cache di un assistente di codice |
| `.claude`, `.ai-docs`, `.vscode`, `.github` | strumenti dell'ambiente di sviluppo |
| `Alg_rt/lg_fmu/lg_cosim/example` | **155 MB** di bundle FMU di esempio già costruiti: materiale di riferimento del manuale, nessun makefile li usa |

Il layer della copia è passato da **1,33 GB a 462 MB**.

Una conseguenza del `.git` mancante: `make` non può più ricavare la versione da
`git describe`. Il bersaglio `version.h` del `Makefile.mk` della radice se ne
accorge e **tiene il `version.h` che la copia si porta dietro** invece di
riscriverlo vuoto — è quel file che `lghmi` e `legopc` leggono per mostrare la
versione di LegoPST, e `legopc` lo rigenera da sé se manca.

### Lanciare un'immagine diversa

`lgdock`, `lgdock_multi` e `lgdock_socat` usano `aguagliardi/legopst:2.0`. Per
una build di prova o un tag personale c'è la variabile d'ambiente:

```sh
LG_DOCKER_IMAGE=aguagliardi/legopst:2.1-prova lgdock
```

Questo installer consente di eseguire LegoPST senza installare nulla localmente - tutto funziona tramite container Docker.

## Requisiti

- Docker installato e funzionante
- Sistema Linux (Ubuntu, Fedora, Debian, etc.) o WSL2
- Connessione internet per il download

## Installazione Rapida

### Metodo 1: Download e esecuzione diretta

```bash
curl -fsSL https://raw.githubusercontent.com/RSE-TGM/LegoPST/master/docker/install_legopst_dock.sh | bash
```

### Metodo 2: Download manuale

```bash
# Scarica lo script
curl -fsSL https://raw.githubusercontent.com/RSE-TGM/LegoPST/master/docker/install_legopst_dock.sh -o install_legopst_dock.sh

# Rendi eseguibile
chmod +x install_legopst_dock.sh

# Esegui
./install_legopst_dock.sh
```

### Metodo 3: Clone del repository

```bash
git clone https://github.com/RSE-TGM/LegoPST.git
cd LegoPST2010A/docker
./install_legopst_dock.sh
```

## Cosa fa lo script di installazione

Lo spiega da sé: **`install_legopst_dock.sh -h`** dice a cosa serve, cosa
scrive, cosa fare dopo e come disinstallare — prima di eseguirlo, non dopo.

1. **Verifica i prerequisiti**: controlla che Docker e curl siano installati
2. **Scarica lgdock.sh**: dal repository ufficiale, **dal branch** — non da una
   copia: è per questo che rilanciarlo è il modo di prendere le correzioni
3. **Crea il comando `lgrun`**: wrapper semplice per lanciare LegoPST
4. **Configura il PATH**: aggiunge ~/.local/bin al PATH se necessario
5. **Verifica l'installazione**: testa che tutto funzioni

Non installa LegoPST, non compila niente e non tocca il sistema: scrive in
`~/.local/bin` e, se serve, una riga nel `.bashrc`.

### Aggiornare

Rilancia lo script. Riscarica `lgdock.sh` dal branch e sovrascrive i due
comandi — quelle copie non si aggiornano da sé, e un `lgdock` vecchio continua
a cercare l'immagine che conosceva lui.

### Disinstallare

```sh
install_legopst_dock.sh -u          # oppure --uninstall
```

Fa l'inverso esatto dell'installazione, e **nulla di più**:

- toglie `~/.local/bin/lgrun` e `~/.local/bin/lgdock`, e **solo quelli**: se un
  file con quel nome non è stato messo da qui, lo lascia dov'è e lo dice;
- la riga del `PATH` nel `.bashrc` la toglie **solo se `~/.local/bin` resta
  vuota**. Se dentro c'è ancora qualcosa la lascia e ti dice quale riga
  guardare: là vivono spesso altri comandi (`pipx`, `uv`, `cmake`…) e
  togliergliela di sotto li farebbe sparire dal `PATH` senza un errore che lo
  spieghi;
- **non cancella l'immagine Docker** — stampa il comando, `docker rmi
  aguagliardi/legopst:2.0`, e lascia decidere a te;
- **non cancella i tuoi dati**: `~/legocad`, `~/sked`, `~/defaults` e i modelli
  che contengono sono il tuo lavoro.

## Utilizzo

Dopo l'installazione, usa il comando `lgrun`:

```bash
# Avvia LegoPST (modalità standard)
lgrun

# Avvia con modello demo
lgrun --demo

# Avvia con X11 via socat (utile per SSH/MobaXterm)
lgrun --socat

# Combina opzioni
lgrun --demo --socat

# Mostra help
lgrun --help

# Mostra versione
lgrun --version
```

## Primo Avvio

Al primo avvio, Docker scaricherà automaticamente l'immagine LegoPST (circa 2-3 GB).
Questo richiederà alcuni minuti, ma avverrà solo una volta.

```bash
lgrun --demo
```

Attendi il download dell'immagine, poi il container si avvierà automaticamente.

## File Installati

L'installer crea:
- `~/.local/bin/lgdock` - Script principale scaricato da GitHub
- `~/.local/bin/lgrun` - Wrapper comodo per eseguire lgdock

e, se `~/.local/bin` non era nel `PATH`, una riga nel `.bashrc` marcata
`# Added by LegoPST installer`. Sono esattamente le tre cose che
`install_legopst_dock.sh -u` sa disfare.

## Risoluzione Problemi

### Docker non accessibile

Se ricevi errori di permessi Docker:

```bash
# Aggiungi il tuo utente al gruppo docker
sudo usermod -aG docker $USER

# Ricarica i gruppi (o fai logout/login)
newgrp docker
```

### Comando lgrun non trovato

Dopo l'installazione, potresti dover ricaricare il PATH:

```bash
# Ricarica la configurazione bash
source ~/.bashrc

# Oppure riapri il terminale
```

### Problemi X11

Se le finestre grafiche non si aprono:

```bash
# Verifica DISPLAY
echo $DISPLAY

# Se usi SSH, prova con --socat
lgrun --socat
```

### `lgrun -d`: demo di un utente inesistente (100999), o "Cannot change mode"

```
tar: ./legopst_userstd/legocad/r_MDC0/proc: Cannot change mode to rwxr-xr-x: Operation not permitted
$ ls -ln ~/legopst_userstd
drwx------ 9 100999 100999 4096 Oct  1  2025 legocad
drwx------ 3 100999 100999 4096 Oct  1  2025 sked
```

I due sintomi hanno **una causa sola: il runtime è in modalità rootless**, e il
numero `100999` ne è la firma. Vale per Docker rootless *e per Podman*, che è il
caso più insidioso: con il pacchetto `podman-docker` il comando si chiama
`docker`, si comporta come `docker`, ma sotto è Podman rootless. Verifica:

```bash
docker --version                  # "podman version ..." se è lo shim
docker info 2>/dev/null | grep -iE "rootless|podman"
grep "^$USER:" /etc/subuid        # es. "antonio:100000:65536"
```

Ma la prova che chiude la questione, senza interpretare niente, è chiedere al
container di chi gli risulta la home montata:

```bash
docker run --rm -v "$HOME:/host_home" aguagliardi/legopst:2.0 \
       stat -c '%u %g' /host_home
```

`0 0` = rootless. `1000 1000` = runtime classico.

`100999` è `99999 + 1000`: il runtime mette demone e container in uno user
namespace dove **l'UID dell'host diventa 0** e i subuid di `/etc/subuid`
(tipicamente da 100000) diventano `1..65536`. Sul bind mount della home:

| scritto nel container come | esce sull'host come |
|---|---|
| root (UID 0) | l'utente dell'host (1000) — **quello che serve** |
| UID 1000 | `99999 + 1000` = 100999 — nessun utente |

Con un runtime "classico" (Docker rootful, o Podman con `--userns=keep-id`) vale
il contrario, e l'UID dell'host è anche l'UID da usare nel container. `lgdock` era
scritto **solo** per quel caso: creava un utente con l'UID dell'host e faceva
`chown -R` a quell'UID. Su rootless quel `chown` è
esattamente il guasto — intesta la demo a 100999, che sull'host non è nessuno — e
con i modi `0700` rimasti l'utente non riesce nemmeno a entrare nelle directory.
Non è una regressione: era un ramo che non era mai stato scritto, e finché le
macchine sono state rootful non si è visto. Il primo sintomo ad arrivare è che
`tar` muore (`set -e`) **prima** del `chown` e dei link, quindi il `chown` non
fa nemmeno in tempo a sbagliare: il `100999` lo lascia `tar`, ripristinando
l'UID registrato nel tarball.

**Cosa fa adesso `lgdock`.** Non prova a riconoscere il runtime — cosa che con lo
shim `podman-docker` fallirebbe: guarda **di chi risulta `/host_home`** (la home
dell'host) visto da dentro il container. Quel numero *è* l'utente dell'host in
coordinate container, qualunque sia la mappatura, e vale identico per Docker,
Podman e per `--userns=keep-id`:

- risulta l'UID dell'host → runtime classico, tutto come prima;
- risulta **0** → rootless: si lavora come root del container (niente utente
  creato, niente sudoers) e si fa `chown` a `0:0`, così sull'host i file
  risultano dell'utente. Lo dice con un banner
  `=== Modalita' rootless rilevata (Podman o Docker) ===`;
- risulta altro (`65534`, userns-remap, filesystem senza proprietà Unix) → non è
  traducibile: avvisa e prosegue con quel valore.

Inoltre l'estrazione usa ora `tar --no-same-owner` — il proprietario lo decide il
solo `chown`, non quello che è registrato nel tarball — e **sia il `tar` sia il
`chown` sono intercettati**: entrambi possono fallire su filesystem che non
implementano i permessi Unix (cartella condivisa di VM — vboxsf, virtiofs, 9p —
NTFS/exFAT, share di rete), e lo script comincia con `set -e`. Nudi, ammazzavano
l'installazione **prima** dei link `~/legocad` e `~/sked`, lasciando una demo
inutilizzabile senza che nulla lo dicesse.

**Se hai già una demo installata da una versione precedente** non viene toccata:
`lgrun -d` si ferma a "Demo già installata". Adesso però controlla di chi è e
avvisa. Per rifarla (il `rm` funziona anche se i file sono di 100999: conta il
permesso sulla home, non sui file):

```bash
rm -rf ~/legopst_userstd ~/legocad ~/sked
lgrun -d
```

## Confezionamento della demo

Il tarball `demo/legopst_userstd.tgz` si costruisce con
[`demo/make_demo_tgz.sh`](../demo/make_demo_tgz.sh), non a mano: lo script esiste
proprio perché l'esclusione qui sotto non si perda al prossimo repack.

**Dalla demo si esclude `*/proc`**, cioè la directory di *build* di ogni task:

- contiene l'**eseguibile della task** (`lg2`) e gli oggetti compilati
  (`foraus.o`), binari costruiti sulla macchina di confezionamento contro le
  *sue* librerie: su un'altra macchina non valgono niente. Chi usa la demo rifà
  la task con i propri eseguibili e librerie, e `proc/` viene ricreata lì;
- è il grosso del pacchetto: 47 MB non compressi, da 20 MB a 17 MB compressi;
- sotto `out/` il `proc` non è nemmeno una directory ma un **symlink**, che
  `net_sked` ricrea da solo a ogni avvio di task — e prima lo cancella apposta,
  per non lasciarne uno stantìo (`sked_start.c`, `unlink()` poi `symlink()`).
  Quelli confezionati erano per giunta **assoluti e cablati sulla home di chi
  aveva fatto il pacchetto**, quindi rotti su qualunque altra macchina.

> **Non "aggiustare" quei symlink creando una directory vera al loro posto.**
> `net_sked` fa `unlink()` e poi `symlink()`: su una directory l'`unlink`
> fallisce, il `symlink` fallisce con `EEXIST` e si finisce su `exit(1)` — la
> task non parte. **Assente** è lo stato giusto.

Resta invece `out/` con `f21.dat`, `lg5.out`, `lg5c.out`: `f21.dat` viene letto a
runtime (`sked_start.c`, `sked_fine.c`, `lg5sim.for`), quindi si esclude
`*/proc`, non `*/out`.

> Dopo aver rigenerato il tarball **va ricostruita l'immagine Docker**: il
> `Dockerfile_LegoPST` copia l'intero repository (`COPY /LegoPST
> /home/legoroot_fedora41`) e la demo viaggia lì dentro. Senza rebuild, `lgrun -d`
> continua a estrarre il tarball vecchio. L'immagine si costruisce con
> `make -f Makefile.mk docker` dalla radice del repository (o `docker/BuildImage -y`):
> il `make` normale non la tocca.

## Disinstallazione

Per rimuovere LegoPST:

```bash
# Rimuovi i comandi installati
rm ~/.local/bin/lgdock
rm ~/.local/bin/lgrun

# Rimuovi l'immagine Docker (opzionale)
docker rmi aguagliardi/legopst:2.0

# Rimuovi i dati utente (opzionale - ATTENZIONE: cancella i tuoi modelli!)
rm -rf ~/legopst_userstd
rm -rf ~/defaults
```

## Aggiornamento

Per aggiornare all'ultima versione:

```bash
# Riesegui l'installer
curl -fsSL https://raw.githubusercontent.com/RSE-TGM/LegoPST/master/docker/install_legopst_dock.sh | bash

# Oppure aggiorna l'immagine Docker
docker pull aguagliardi/legopst:2.0
```

## Supporto

Per problemi o domande:
- Issue tracker: https://github.com/RSE-TGM/LegoPST/issues
- Email: your.email@example.com

## Note Tecniche

### Directory Condivise

Il container monta automaticamente:
- `$HOME` → `/host_home` nel container
- I tuoi file sono accessibili in entrambi gli ambienti
- I link simbolici permettono accesso comodo a legocad e sked

### Utente nel Container

Lo script crea automaticamente un utente nel container con:
- Stesso username dell'host
- Stesso UID e GID dell'host
- Permessi sudo senza password
- Home directory mappata

### Persistenza Dati

Tutti i dati utente (modelli, configurazioni) sono salvati nell'home dell'host:
- `~/legopst_userstd/` - Modelli e progetti
- `~/defaults/` - Configurazioni predefinite
- I dati sopravvivono alla chiusura del container

## Licenza

[Specificare la licenza del progetto]
