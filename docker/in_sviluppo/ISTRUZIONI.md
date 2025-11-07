# 📚 ISTRUZIONI DI IMPLEMENTAZIONE

## 🎯 Cosa hai ricevuto

1. **README.md** - Documentazione completa per gli utenti
2. **installer.sh** - Script di installazione automatica
3. **sync-gist.yml** - GitHub Action per sincronizzazione automatica
4. **ISTRUZIONI.md** - Questo file (guida per te)

## 📝 Passaggi da seguire

### 1️⃣ Prepara i Gist

Se non hai ancora creato i Gist per i tuoi script:

```bash
# Crea Gist per lgdock.sh
gh gist create docker/lgdock.sh --public --desc "LGDock - Docker management script"

# Crea Gist per lgdock_socat.sh  
gh gist create docker/lgdock_socat.sh --public --desc "LGDock Socat - Docker management with socat support"

# Crea Gist per l'installer
gh gist create installer.sh --public --desc "LGDock Installer Script"
```

Annota i Gist ID che vengono restituiti (es: d7c030f939f69b07784a309889b8510a)

### 2️⃣ Configura i file

#### Nel file `installer.sh`:
- **Riga 23**: Sostituisci `TUO_USERNAME` con il tuo username GitHub
- **Riga 24-25**: Verifica/sostituisci i GIST_ID con i tuoi

```bash
GITHUB_USER="tuousername"  # <-- MODIFICA QUI
LGDOCK_GIST_ID="d7c030f939f69b07784a309889b8510a"  # <-- TUO GIST ID
LGDOCK_SOCAT_GIST_ID="83c887ef78610842508b9f972130d3e1"  # <-- TUO GIST ID
```

#### Nel file `README.md`:
Sostituisci tutti i `TUO_USERNAME` con il tuo username GitHub reale.

Cerca e sostituisci:
```bash
sed -i 's/TUO_USERNAME/tuousername/g' README.md
```

#### Nel file `sync-gist.yml`:
I Gist ID sono già corretti basandomi sui tuoi ID forniti.

### 3️⃣ Carica l'installer su Gist

```bash
# Crea un nuovo Gist per l'installer
gh gist create installer.sh --public --desc "LGDock Installer"

# O se hai già un Gist, aggiornalo
gh gist edit [INSTALLER_GIST_ID] installer.sh
```

### 4️⃣ Configura GitHub Actions

1. Nel tuo repository GitHub, crea la directory:
   ```bash
   mkdir -p .github/workflows
   ```

2. Copia il file `sync-gist.yml`:
   ```bash
   cp sync-gist.yml .github/workflows/
   ```

3. Crea il secret `GIST_SECRET`:
   - Vai su GitHub → Settings → Developer settings → Personal access tokens → Tokens (classic)
   - Clicca "Generate new token"
   - Nome: "GIST_SECRET"
   - Seleziona permesso: `gist` ✅
   - Clicca "Generate token"
   - Copia il token
   
4. Aggiungi il token al repository:
   - Vai nel tuo repo → Settings → Secrets and variables → Actions
   - Clicca "New repository secret"
   - Name: `GIST_SECRET`
   - Secret: incolla il token
   - Clicca "Add secret"

### 5️⃣ Crea il README principale su Gist

```bash
# Carica il README come Gist separato
gh gist create README.md --public --desc "LGDock Documentation"
```

### 6️⃣ Testa il sistema

#### Test manuale dell'installer:
```bash
# Test locale
bash installer.sh --check

# Test da remoto
bash <(curl -sSL https://gist.github.com/tuousername/[INSTALLER_GIST_ID]/raw) --check
```

#### Test GitHub Action:
1. Vai su GitHub → tuo repo → Actions
2. Seleziona "Sync Scripts to Gist"
3. Clicca "Run workflow" → "Run workflow"
4. Abilita debug per vedere più dettagli

### 7️⃣ Condividi con gli utenti

Crea una pagina/post con queste istruzioni semplificate:

```markdown
## 🚀 Installazione LGDock

### Metodo 1: Installer automatico (consigliato)
```bash
bash <(curl -sSL https://gist.github.com/USERNAME/INSTALLER_ID/raw)
```

### Metodo 2: Installazione rapida
```bash
# Installa entrambi gli script
curl -sSL https://gist.github.com/USERNAME/ID1/raw -o /tmp/lgdock && \
curl -sSL https://gist.github.com/USERNAME/ID2/raw -o /tmp/lgdock_socat && \
sudo mv /tmp/lgdock* /usr/local/bin/ && \
sudo chmod +x /usr/local/bin/lgdock*
```

### Documentazione completa
https://gist.github.com/USERNAME/README_GIST_ID
```

## 🔍 Verifica finale

Checklist per assicurarti che tutto funzioni:

- [ ] I file `lgdock.sh` e `lgdock_socat.sh` sono nella directory `docker/`
- [ ] I Gist sono stati creati e sono pubblici
- [ ] L'installer.sh ha il tuo username e i Gist ID corretti
- [ ] Il secret GIST_SECRET è configurato nel repository
- [ ] La GitHub Action appare nella tab Actions
- [ ] Il workflow si attiva quando modifichi i file in `docker/`
- [ ] Gli utenti possono installare con il comando one-liner

## 🆘 Troubleshooting

### La GitHub Action non si attiva
- Verifica che il branch sia `main` o `master` (controlla nel workflow)
- Assicurati che il file sia in `.github/workflows/` (con la 's')
- Controlla che il file sia `.yml` non `.yaml`

### Errore "repository not found" nella Action
- Il secret GIST_SECRET potrebbe mancare o essere errato
- Verifica i permessi del token (deve avere `gist`)

### Gli script non si aggiornano su Gist
- Controlla i log della GitHub Action
- Verifica che i Gist ID siano corretti
- Prova ad eseguire manualmente con "Run workflow"

### L'installer non funziona
- Verifica che l'username GitHub sia corretto
- Controlla che i Gist siano pubblici
- Testa con `bash installer.sh --check` localmente

## 📬 Supporto

Se hai problemi:
1. Abilita il debug nella GitHub Action
2. Esegui `installer.sh --check` per diagnostica
3. Controlla i log in GitHub Actions → tuo workflow → ultimo run

## ✅ Fatto!

Una volta completati questi passaggi, i tuoi utenti potranno installare facilmente i tuoi script con un singolo comando!

---

**Nota**: Ricorda di aggiornare periodicamente la documentazione e di monitorare i feedback degli utenti per migliorare l'esperienza di installazione.
