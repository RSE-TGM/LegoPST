# Installazione LegoPST via Docker

## L'immagine: `aguagliardi/legopst:2.0`

Si costruisce dalla radice del repository:

```sh
make -f Makefile.mk docker        # la costruisce
make -f Makefile.mk docker-push   # la costruisce e la pubblica
```

2,27 GB su disco, 447 pacchetti rpm, e un layer da 462 MB per la copia del
repository. Dentro ci sono `gcc`, `gfortran`, Motif, Tcl/Tk/Tix,
`ghostscript`, ImageMagick, `evince`, `firefox` e tutti i `-devel`: si compila
e si lavora come sulla macchina di sviluppo.

### Perché una sola

Fino al **2026-09-27** ce n'erano due, `legopst_multi` (tutti i pacchetti) e
`legopst_slim`. Misurando che cosa le distingueva davvero — 213 pacchetti e
529 MB — è venuto fuori che non c'era niente su cui LegoPST si appoggi:

| | |
|---|---|
| `gimp` | 32 pacchetti, **355 MB** (`suitesparse` 140, gimp 105, `openblas-openmp` 39). Sostituito con `mtpaint`: `LG_ICOEDITOR` in `Alg_env.sh` è una catena di ripieghi, serve *un* editor di icone, non quello |
| dipendenze deboli | circa 120 pacchetti: `systemd-udev`, `NetworkManager-libnm`, `pipewire`, `pulseaudio`, `tracker-miners`, `appstream`, la catena di perl. Un desktop, in un container che fa X11 e Motif |
| `git` → `git-core` | 70 pacchetti per 73 MB, di cui `git-core-doc` da solo 18 MB. Restano i comandi, si perdono i sottocomandi in perl (`git send-email`, `git svn`) |

### Perché il browser è `firefox` e non uno più leggero

Il menu **?** di `lghmi` e `legopc` apre la documentazione HTML con
`exec $browser $file &` (`openhelp.tcl`). Senza browser la catena di
`lg_pick` cade su `xdg-open`, che a sua volta non trova nulla: si vedevano venti
righe di *command not found* e nessun documento.

Costo misurato dentro l'immagine con `dnf install --assumeno`:

| | | |
|---|---|---|
| `lynx` | 2 MiB | solo testo |
| `links` | 3 MiB | solo testo |
| `w3m` | 9 MiB | solo testo |
| `epiphany` | 69 MiB | grafico — **non funziona qui** |
| **`firefox`** | **98 MiB** | grafico — la scelta |
| `falkon` | 157 MiB | grafico, porta l'intero stack Qt |

**`epiphany` costerebbe 29 MiB in meno ed è già nella lista dei candidati, ma
muore**: usa *bubblewrap* per isolare i processi web, e creare namespace
annidati dentro un container non è permesso — sotto Podman rootless a maggior
ragione, perché si è già dentro uno user namespace.

```
bwrap: Creating new namespace failed: Operation not permitted
Failed to fully launch dbus-proxy
```

Servirebbe `--privileged` o `SYS_ADMIN`: uno scambio pessimo per un lettore di
documentazione. `firefox` incontra **la stessa restrizione** —
`Sandbox: CanCreateUserNamespace() clone() failure: EPERM` — ma la degrada con
un avviso e prosegue. Verificato rendendo una pagina dentro il container; non
gli serve nemmeno `dbus`.

I browser testuali costerebbero una manciata di MiB, ma `openhelp.tcl` li lancia
**staccati e senza terminale**: partirebbero su un terminale che non c'è. Per
usarli servirebbe avvolgerli in `lgterm` e aggiungerli alle liste di candidati
in `Alg_env.sh` e `openhelp.tcl`, e resterebbe una resa scadente per
`DOCUMENTATION_INDEX.html`, che è impaginato con CSS.

`firefox` è già il **primo** della lista dei candidati, quindi non si tocca una
riga di codice.

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
   copia: è per questo che rilanciarlo è il modo di prendere le correzioni. Lo
   installa in `~/.local/lib/legopst/lgdock`, **fuori dal `PATH`**: è il motore,
   non un comando da digitare
3. **Crea il comando `lgrun`**: un **symlink** a quel `lgdock`. L'utente finale
   ha così un solo comando nel `PATH`, e l'aiuto porta il nome giusto da sé —
   bash mette in `$0` il percorso con cui lo script è stato invocato, quindi
   attraverso il link `basename "$0"` vale già `lgrun`
4. **Stampiglia dentro `lgdock`** la versione e le coordinate del repository da
   cui l'installazione è venuta, che servono a `lgrun update`
5. **Configura il PATH**: aggiunge ~/.local/bin al PATH se necessario
6. **Verifica l'installazione**: testa che tutto funzioni

Non installa LegoPST, non compila niente e non tocca il sistema: scrive in
`~/.local/bin`, in `~/.local/lib/legopst` e, se serve, una riga nel `.bashrc`.

> Prima l'installer creava `lgrun` come piccolo involucro che faceva `exec` di
> `lgdock`, e metteva **entrambi** in `~/.local/bin`. Due comandi identici nella
> stessa directory, e l'aiuto di `lgrun` diceva *«Uso: lgdock»*: si digitava un
> nome e se ne leggeva un altro. Un'installazione fatta così viene ripulita al
> primo aggiornamento — il vecchio `~/.local/bin/lgdock` viene rimosso.

### Aggiornare

```sh
lgrun update
```

Riscarica l'installer **dallo stesso branch** da cui è venuta l'installazione e
lo esegue, poi aggiorna l'immagine Docker. Così installazione e aggiornamento
restano un solo percorso di codice, che non può divergere.

L'ordine non è casuale: l'immagine si scarica **dopo** aver installato gli
script, perché il nome dell'immagine sta dentro `lgdock` e un aggiornamento può
cambiarlo — scaricandola prima si tirerebbe giù quella vecchia.

Se il download dell'installer fallisce, o quel che arriva non è uno script,
`update` **si ferma prima di toccare qualcosa**: il comando che hai continua a
funzionare. Se fallisce solo il `docker pull`, l'installazione resta valida e lo
dice.

> **Dettaglio da non "semplificare".** `update` sostituisce il proprio processo
> con l'installer (`exec`) invece di chiamarlo. L'installer riscrive `lgdock`
> con `curl -o`, che **tronca lo stesso inode**; bash legge uno script a pezzi
> tenendo aperto il descrittore, quindi se il contenuto cambia sotto prosegue al
> vecchio offset dentro il nuovo testo ed esegue spazzatura. Con `exec` il
> descrittore è già chiuso quando l'installer scrive.

Rilanciare l'installer a mano continua a funzionare, e resta la strada per chi
non ha ancora un `lgrun` che conosce `update`.

### Disinstallare

```sh
lgrun uninstall                     # oppure, a mano:
install_legopst_dock.sh -u          # ...che e' esattamente la stessa cosa
```

Fa l'inverso esatto dell'installazione, e **nulla di più**:

- toglie `~/.local/bin/lgrun` e `~/.local/lib/legopst/lgdock` (e la directory,
  se resta vuota), e **solo quelli**: se un file con quel nome non è stato messo
  da qui, lo lascia dov'è e lo dice. Ripulisce anche il vecchio
  `~/.local/bin/lgdock`, per chi viene da un'installazione precedente;
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

# Avvia e apre subito un programma nel container (qui il selettore lghmi)
lgrun -e lghmi

# Solo lghmi, senza shell: il container si chiude quando lo chiudi
lgrun -a lghmi

# Mostra help
lgrun --help

# Mostra versione
lgrun --version
```

### Lanciare un programma all'avvio: `-e`

`lgrun -e <prog>` (anche `--exec`) esegue `<prog>` dentro il container appena è
pronto, così con un solo comando si apre, per esempio, `lghmi`:

```bash
lgrun -e lghmi
lgrun -d -e lghmi              # con la demo
lgrun -e "lghmi -staz"         # con argomenti: tra virgolette
```

- Parte **dopo il profilo LegoPST**, quindi con `DISPLAY`, `PATH` e le variabili
  del simulatore corrente già impostate, come se lo si scrivesse al prompt.
- Gira **in background**: il terminale resta la solita shell del container, da
  cui si può continuare a lavorare. Il container vive finché vive quella shell,
  e con lui `lghmi` e quello che apre (HMI, faceplate, la simulazione): è il
  motivo per cui il container non si chiude al *Quit* del programma, che si
  porterebbe via tutto il resto. Per uscire si chiude la shell (`exit`).
- **Una volta sola** per container: le shell di login aperte dopo (un `bash -l`,
  un terminale aperto da `lghmi`) non lo rilanciano.
- L'output del programma va in **`/tmp/lgdock_exec.log`**, dentro il container,
  per non mescolarsi con il prompt. Se il comando non esiste nel container lo
  dice al posto dell'avvio.

Il comando arriva al container in una variabile d'ambiente (`LGDOCK_EXEC`), non
sostituito nel testo dello script: spazi, argomenti e virgolette arrivano
intatti.

### Modo applicazione: `-a`

`lgrun -a <prog>` (anche `--app`) esegue **solo** `<prog>` e fa vivere il
container **quanto lui**:

```bash
lgrun -a lghmi
```

- **Nessuna shell del container** nel terminale: il container si prepara come
  sempre, esegue il programma con il profilo LegoPST caricato, e basta.
- Finché il programma è aperto il container esiste, con tutto quello che il
  programma lancia: da `lghmi`, la simulazione, le HMI, `legopc`, i faceplate.
- Quando il programma **finisce — chiuso o andato in crash — il container
  termina** e porta via anche quei processi. È la differenza con `-e`, dove il
  container resta finché non si chiude la shell.
- `lgrun` resta in attesa, come un qualsiasi programma grafico lanciato da
  terminale, mostra l'output del programma e alla fine **restituisce il suo
  codice di uscita**. Non vuole un terminale: si può lanciare da un menu, da uno
  script o con `&`.
- **Ctrl-C** (o un `kill` di `lgrun`) arriva al programma e chiude il container.

> **Chiudere il programma chiude tutto, di colpo.** Se da `lghmi` è partita una
> simulazione e si esce da `lghmi`, il container termina e la simulazione viene
> ammazzata con lui, senza lo *Simulator Shutdown* ordinato del banco. Prima di
> uscire conviene fermarla da lì.

`-a` e `-e` non si usano insieme. Quale scegliere:

| | `lgrun -e <prog>` | `lgrun -a <prog>` |
|---|---|---|
| nel terminale | la shell del container | niente shell: l'output del programma |
| il programma | parte in background, una volta | è l'unica cosa che gira |
| il container finisce | all'`exit` della shell | quando il programma finisce o va in crash |
| serve un terminale | sì | no: va bene da menu, script, `&` |
| codice di uscita di `lgrun` | quello della shell | quello del programma |

In entrambi i modi `lghmi` ricorda l'ultimo Simulator path fra un avvio e
l'altro: nel container tiene la sua memoria in `~/defaults`, che sta sull'host
(vedi [LGHMI.md](../Alg_legopc/LGHMI.md), *I path recenti*).

Come è fatto: senza `-it` nel `docker run`, e in fondo allo script del container
`su - <utente> -c <prog>` al posto della shell interattiva. Lo script è il
processo 1 del container: quando esce, il runtime termina tutto il resto. `su`
gira in background con una `trap` e una `wait`, perché il processo 1 ignora i
segnali per cui non ha un gestore e il Ctrl-C dell'host andrebbe perso.

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

### `WARN ... "/" is not a shared mount` (Podman rootless, WSL)

```
WARN[0001] "/" is not a shared mount, this could cause issues or missing mounts with rootless containers
```

È un **avviso di Podman**, non un errore di LegoPST: dopo il messaggio
LegoPST parte normalmente. Compare con Podman rootless (anche quando il comando
si chiama `docker`, vedi lo shim `podman-docker` più sotto) se la radice `/`
dell'host non è montata come *shared*, cioè senza la propagazione dei mount.

**Per `lgrun` è innocuo.** La propagazione serve solo ai container che chiedono
di vedere i mount fatti *dopo* l'avvio (`:rshared`, `:rslave`).
[`lgdock.sh`](lgdock.sh) monta solo il socket X11, il file `.Xauthority` e la
home su `/host_home`, con dei `-v` semplici.

Si vede tipicamente in una **WSL senza systemd** (es. Arch): è systemd che
all'avvio rende `/` shared, e senza di lui resta *private*. Su una distribuzione
con systemd attivo (Fedora, Ubuntu recenti) l'avviso non compare.

Per toglierlo, fino al prossimo riavvio della WSL:

```bash
sudo mount --make-rshared /
```

Per sempre, in `/etc/wsl.conf`, poi `wsl --shutdown` da Windows:

```ini
[boot]
command = mount --make-rshared /
```

Se `/etc/wsl.conf` ha già `systemd=true` e l'avviso compare lo stesso, systemd
non sta partendo: il problema è quello, non LegoPST.

L'**attesa** al primo `lgrun` dopo un riavvio della WSL non dipende da questo
avviso: Podman rootless prepara lo spazio utente e i layer dell'immagine, e i
lanci successivi sono più rapidi.

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

### Cosa si vede: la home dentro il container cambia

È la conseguenza visibile del punto qui sopra, e sorprende perché sembra un
errore. Sulla **stessa** macchina, con lo **stesso** comando:

| | runtime classico | rootless (Podman, Docker rootless) |
|---|---|---|
| utente nel container | `antonio` (UID 1000) | `root` (UID 0) |
| `$HOME` | `/home/antonio` | **`/root`** |
| `LEGOCAD_USER` | `/home/antonio` | **`/root`** |
| prompt | `LegoPST@.../antonio` | `LegoPST@.../root` |

**Non cambia quali dati vedi.** La home dell'host è montata in `/host_home` —
è l'unico punto in cui entra nel container — e `lgdock` crea in entrambi i casi
gli stessi collegamenti:

```
~/legocad   ->  /host_home/legocad
~/sked      ->  /host_home/sked
~/defaults  ->  /host_home/defaults
~/host_data ->  /host_home
```

Quindi `/root/sked` e `/home/antonio/sked` sono lo stesso `~/sked` dell'host. Se
due esecuzioni mostrano **simulatori diversi** non è il container: sono due home
diverse. Succede tipicamente su WSL, dove ogni distribuzione ha il suo
`/home/<utente>` e `lgdock` monta quella da cui lo lanci.

> **La home del container è effimera, in entrambi i casi.** `/home/antonio` e
> `/root` vivono dentro il container e con `--rm` spariscono all'uscita.
> Sopravvive solo ciò che sta sotto i quattro collegamenti qui sopra, perché
> puntano all'host. Quello che salvi in `~/altro` lo perdi — e non c'entra il
> rootless.
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
