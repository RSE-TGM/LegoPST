# Installazione LegoPST via Docker

Questo installer consente di eseguire LegoPST senza installare nulla localmente - tutto funziona tramite container Docker.

## Requisiti

- Docker installato e funzionante
- Sistema Linux (Ubuntu, Fedora, Debian, etc.) o WSL2
- Connessione internet per il download

## Installazione Rapida

### Metodo 1: Download e esecuzione diretta

```bash
curl -fsSL https://raw.githubusercontent.com/TUOREPO/install_legopst_dock.sh | bash
```

### Metodo 2: Download manuale

```bash
# Scarica lo script
curl -fsSL https://raw.githubusercontent.com/TUOREPO/install_legopst_dock.sh -o install_legopst_dock.sh

# Rendi eseguibile
chmod +x install_legopst_dock.sh

# Esegui
./install_legopst_dock.sh
```

### Metodo 3: Clone del repository

```bash
git clone https://github.com/TUOREPO/LegoPST2010A.git
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
curl -fsSL https://raw.githubusercontent.com/TUOREPO/install_legopst_dock.sh | bash

# Oppure aggiorna l'immagine Docker
docker pull aguagliardi/legopst_multi:2.0
```

## Supporto

Per problemi o domande:
- Issue tracker: https://github.com/TUOREPO/issues
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
