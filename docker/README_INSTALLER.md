# Installazione LegoPST via Docker

## Due immagini: completa e snella

Lo stesso ambiente, due pesi. Si scelgono dalla radice del repository:

```sh
make -f Makefile.mk docker         # aguagliardi/legopst_multi:2.0   (completa)
make -f Makefile.mk docker_small   # aguagliardi/legopst:2.0         (snella)
```

| | completa | snella |
|---|---|---|
| contenuto | 1,52 GB | **704 MB** |
| su disco | 4,51 GB | 2,55 GB |
| pacchetti rpm | 657 | 447 |

Dentro si compila e si lavora allo stesso modo: ci sono `gcc`, `gfortran`,
Motif, Tcl/Tk/Tix, `ghostscript`, ImageMagick e tutti i `-devel`. Cosa cambia
e perché è scritto in testa a
[`Dockerfile_LegoPST_small`](Dockerfile_LegoPST_small); in due righe:

- **le dipendenze deboli non si installano** (`install_weak_deps=False`);
- **`gimp` sostituito da `mtpaint`** — `LG_ICOEDITOR` è una catena di ripieghi
  (`Alg_env.sh`), serve *un* editor di icone e con gimp se ne andavano 50
  pacchetti, `suitesparse` e `openblas` compresi;
- **il `.git` del repository non entra nella copia**: 631 MB che là dentro
  nessuno consulterebbe. Restano invece i `.o` e i `.a`: sembravano residui di
  compilazione, ma `legocad/lego_big/lib/*.a` sono le librerie con cui
  `cad_crealg1` linka le task quando si apre una HMI — senza, la HMI si apre
  col disegno cancellato. Lo tiene fuori
  [`Dockerfile_LegoPST_small.dockerignore`](Dockerfile_LegoPST_small.dockerignore),
  che vale solo per quel Dockerfile — la completa resta identica a prima.

**`evince` è rimasto** anche nella snella, benché si porti dietro
`mesa-dri-drivers` e `llvm-libs` (285 MB): `esporta.tcl` apre con
`LG_PDFVIEWER` sia i PDF sia i **PNG**, e un visualizzatore di soli PDF
lascerebbe monco l'export PNG di `legopc`. Rinunciando a quella funzione si
scenderebbe di altri ~285 MB.

### Lanciare la snella

`lgdock` usa la completa per default. Per la snella basta un'opzione, come per
la demo o per socat:

```sh
lgdock              # immagine completa (come sempre)
lgdock --small      # immagine snella
lgdock -S -d        # snella, con la demo
```

Vale per `lgdock`, `lgdock_multi` e `lgdock_socat`. Per un'immagine diversa da
queste due — una build di prova, un tag personale — c'è la variabile
d'ambiente, che l'opzione sovrascrive:

```sh
LG_DOCKER_IMAGE=aguagliardi/legopst:3.0-test lgdock
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

1. **Verifica i prerequisiti**: controlla che Docker e curl siano installati
2. **Scarica lgdock.sh**: dal repository ufficiale
3. **Crea il comando `lgrun`**: wrapper semplice per lanciare LegoPST
4. **Configura il PATH**: aggiunge ~/.local/bin al PATH se necessario
5. **Verifica l'installazione**: testa che tutto funzioni

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
docker run --rm -v "$HOME:/host_home" aguagliardi/legopst_multi:2.0 \
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
docker rmi aguagliardi/legopst_multi:2.0

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
docker pull aguagliardi/legopst_multi:2.0
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
