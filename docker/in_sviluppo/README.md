# 🐳 LGDock Scripts Suite

> Strumenti Docker avanzati per la gestione di container e servizi

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Bash](https://img.shields.io/badge/bash-%3E%3D4.0-green.svg)](https://www.gnu.org/software/bash/)
[![Docker](https://img.shields.io/badge/docker-%3E%3D20.10-blue.svg)](https://www.docker.com/)

## 📋 Descrizione

**LGDock** è una suite di script bash per semplificare la gestione di Docker con funzionalità avanzate:

- **`lgdock.sh`** - Script principale per gestione container Docker
- **`lgdock_socat.sh`** - Versione estesa con supporto socat per proxy e tunneling

## 🚀 Installazione Rapida

### Opzione 1: One-Line Install (Consigliato)

```bash
# Installer interattivo
bash <(curl -sSL https://gist.githubusercontent.com/TUO_USERNAME/ID_INSTALLER/raw/installer.sh)
```

### Opzione 2: Installazione Diretta

#### Installa lgdock
```bash
sudo curl -sSL https://gist.githubusercontent.com/TUO_USERNAME/d7c030f939f69b07784a309889b8510a/raw/lgdock.sh \
  -o /usr/local/bin/lgdock && sudo chmod +x /usr/local/bin/lgdock
```

#### Installa lgdock_socat
```bash
sudo curl -sSL https://gist.githubusercontent.com/TUO_USERNAME/83c887ef78610842508b9f972130d3e1/raw/lgdock_socat.sh \
  -o /usr/local/bin/lgdock_socat && sudo chmod +x /usr/local/bin/lgdock_socat
```

### Opzione 3: Esecuzione Senza Installazione

```bash
# Esegui lgdock direttamente
bash <(curl -sSL https://gist.githubusercontent.com/TUO_USERNAME/d7c030f939f69b07784a309889b8510a/raw/lgdock.sh) [parametri]

# Esegui lgdock_socat direttamente
bash <(curl -sSL https://gist.githubusercontent.com/TUO_USERNAME/83c887ef78610842508b9f972130d3e1/raw/lgdock_socat.sh) [parametri]
```

### Opzione 4: Installazione Manuale

```bash
# 1. Scarica gli script
wget https://gist.githubusercontent.com/TUO_USERNAME/d7c030f939f69b07784a309889b8510a/raw/lgdock.sh
wget https://gist.githubusercontent.com/TUO_USERNAME/83c887ef78610842508b9f972130d3e1/raw/lgdock_socat.sh

# 2. Rendi eseguibili
chmod +x lgdock.sh lgdock_socat.sh

# 3. Sposta in PATH (opzionale)
sudo mv lgdock.sh /usr/local/bin/lgdock
sudo mv lgdock_socat.sh /usr/local/bin/lgdock_socat
```

## 🔧 Configurazione

### Funzioni per Shell (.bashrc/.zshrc)

Aggiungi al tuo `~/.bashrc` o `~/.zshrc`:

```bash
# LGDock Functions
lgdock() {
    bash <(curl -sSL https://gist.githubusercontent.com/TUO_USERNAME/d7c030f939f69b07784a309889b8510a/raw/lgdock.sh) "$@"
}

lgdock_socat() {
    bash <(curl -sSL https://gist.githubusercontent.com/TUO_USERNAME/83c887ef78610842508b9f972130d3e1/raw/lgdock_socat.sh) "$@"
}

# Alias utili
alias lgd='lgdock'
alias lgds='lgdock_socat'
```

## 📖 Utilizzo

### lgdock.sh - Comandi Base

```bash
# Avvia container
lgdock start [container-name]

# Ferma container
lgdock stop [container-name]

# Lista container attivi
lgdock ps

# Mostra log
lgdock logs [container-name]

# Shell interattiva
lgdock shell [container-name]

# Rimuovi container
lgdock rm [container-name]
```

### lgdock_socat.sh - Comandi Avanzati

```bash
# Crea tunnel TCP
lgdock_socat tunnel [source-port] [dest-host:port]

# Proxy HTTP
lgdock_socat proxy [port]

# Forward porta container
lgdock_socat forward [container:port] [local-port]

# Monitor connessioni
lgdock_socat monitor [port]
```

## 🐳 Uso con Docker

### Container con lgdock pre-installato

```dockerfile
FROM ubuntu:latest
RUN apt-get update && apt-get install -y curl bash
RUN curl -sSL https://gist.githubusercontent.com/TUO_USERNAME/ID_INSTALLER/raw/installer.sh | bash -s -- --all --quiet
ENTRYPOINT ["lgdock"]
```

### Docker Run

```bash
# Esegui lgdock in container temporaneo
docker run --rm -it \
  -v /var/run/docker.sock:/var/run/docker.sock \
  ubuntu:latest \
  bash -c "curl -sSL https://gist.github.../lgdock.sh | bash"
```

## 🔄 Aggiornamento

### Aggiornamento Automatico

```bash
# Con installer
bash <(curl -sSL https://gist.github.../installer.sh) --update

# Manuale
sudo curl -sSL https://gist.github.../lgdock.sh -o /usr/local/bin/lgdock
sudo curl -sSL https://gist.github.../lgdock_socat.sh -o /usr/local/bin/lgdock_socat
```

## 🛠️ Troubleshooting

### Problemi Comuni

#### Permission Denied
```bash
# Soluzione: Usa sudo o cambia permessi
sudo chmod +x /usr/local/bin/lgdock*
```

#### Docker Socket Non Accessibile
```bash
# Aggiungi utente al gruppo docker
sudo usermod -aG docker $USER
# Logout e login per applicare
```

#### Socat Non Installato
```bash
# Ubuntu/Debian
sudo apt-get install socat

# CentOS/RHEL
sudo yum install socat

# macOS
brew install socat
```

### Debug

```bash
# Modalità verbose
LGDOCK_DEBUG=1 lgdock [comando]

# Test connessione Docker
docker ps >/dev/null 2>&1 && echo "Docker OK" || echo "Docker Error"

# Verifica installazione
which lgdock lgdock_socat
lgdock --version
```

## ⚡ Performance Tips

1. **Cache delle immagini**: Pre-scarica immagini comuni
   ```bash
   docker pull alpine:latest ubuntu:latest
   ```

2. **Alias personalizzati**: Crea shortcut per comandi frequenti
   ```bash
   alias lgd-clean='lgdock rm $(lgdock ps -aq)'
   ```

3. **Autocompletamento**: Installa bash-completion
   ```bash
   # Ubuntu/Debian
   sudo apt-get install bash-completion
   ```

## 🔒 Sicurezza

⚠️ **IMPORTANTE**: Prima di eseguire script da internet:

1. **Verifica il codice sorgente**
   ```bash
   # Visualizza prima di eseguire
   curl -sSL https://gist.github.../lgdock.sh | less
   ```

2. **Controlla l'hash SHA256**
   ```bash
   curl -sSL https://gist.github.../lgdock.sh | sha256sum
   ```

3. **Usa ambiente isolato per test**
   ```bash
   # Test in container
   docker run --rm -it ubuntu:latest bash
   ```

4. **Limita i permessi**
   ```bash
   # Installa solo per utente corrente
   mkdir -p ~/.local/bin
   curl -sSL ... -o ~/.local/bin/lgdock
   ```

## 📊 Requisiti di Sistema

- **OS**: Linux, macOS, WSL2
- **Bash**: >= 4.0
- **Docker**: >= 20.10
- **Opzionale**: socat (per lgdock_socat)
- **RAM**: Minimo 512MB
- **Disk**: 100MB liberi

## 🤝 Contribuire

1. Fork del repository
2. Crea un branch (`git checkout -b feature/AmazingFeature`)
3. Commit modifiche (`git commit -m 'Add AmazingFeature'`)
4. Push al branch (`git push origin feature/AmazingFeature`)
5. Apri una Pull Request

## 📝 Changelog

### v1.2.0 (2025-01-XX)
- ✨ Aggiunto supporto socat
- 🐛 Fix gestione errori
- 📚 Documentazione migliorata

### v1.1.0 (2024-XX-XX)
- 🚀 Prima release pubblica
- 📦 Installer automatico

## 📄 Licenza

Distribuito sotto licenza MIT. Vedi `LICENSE` per maggiori informazioni.

## 👤 Autore

**[Il Tuo Nome]**
- GitHub: [@tuousername](https://github.com/tuousername)
- Gist: [LGDock Scripts](https://gist.github.com/tuousername)

## 🙏 Ringraziamenti

- Docker Community
- Contributors
- Tutti gli utenti che hanno fornito feedback

## 📞 Supporto

- 📧 Email: tuo@email.com
- 💬 Issues: [GitHub Issues](https://github.com/tuousername/repo/issues)
- 📖 Wiki: [Documentation](https://github.com/tuousername/repo/wiki)

---

<div align="center">
  
⭐ **Se trovi utile questo progetto, considera di aggiungere una stella!** ⭐

</div>
