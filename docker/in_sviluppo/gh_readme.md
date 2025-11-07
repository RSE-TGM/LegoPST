Ottima domanda! `gh` è un comando che sta diventando sempre più indispensabile per chi lavora con Git e GitHub.

In poche parole, **`gh` è il client ufficiale a riga di comando di GitHub (GitHub CLI)**.

Non sostituisce `git`, ma lo **affianca**. Pensa a loro in questo modo:
*   `git` gestisce tutto ciò che riguarda il tuo **repository locale**: commit, branch, merge, cronologia, ecc.
*   `gh` gestisce tutto ciò che riguarda l'interazione con la **piattaforma GitHub.com**: Pull Request, Issue, Actions, Gist, Repository remoti, ecc.

In pratica, `gh` porta l'intera esperienza di GitHub (quella che normalmente faresti tramite il sito web) direttamente nel tuo terminale, rendendo il tuo flusso di lavoro molto più veloce e automatizzabile.

### Differenza Chiave: `git` vs `gh`

| Comando | Scopo Principale | Esempi di Azioni |
| :--- | :--- | :--- |
| **`git`** | **Version Control (Locale)** | `git commit`, `git branch`, `git merge`, `git push` (per inviare i dati), `git pull` (per ricevere i dati) |
| **`gh`** | **Interazione con GitHub (Remoto)** | Creare/listare/unire Pull Request, gestire Issue, vedere i log delle Actions, clonare/creare repository su GitHub |

Lavorano insieme. Ad esempio, useresti `git` per creare i tuoi commit e poi `gh` per creare una Pull Request basata sul tuo branch.

### Cosa puoi fare con `gh`? (Esempi Pratici)

Ecco alcune delle cose più potenti che puoi fare con `gh`, che ti evitano di dover aprire il browser.

#### 1. Gestire le Pull Request (PR)

Questa è forse la funzionalità più usata.

*   **Creare una PR dal tuo branch attuale:**
    ```bash
    # Spinge il branch corrente e avvia una procedura guidata per creare la PR
    gh pr create
    ```
    Ti chiederà titolo, corpo, revisori, etichette, tutto interattivamente.

*   **Listare le PR aperte:**
    ```bash
    gh pr list
    ```

*   **Fare il checkout di una PR di qualcun altro per testarla localmente:**
    Questo è un enorme risparmio di tempo! Invece di complicati comandi `git fetch`, basta fare:
    ```bash
    # Scarica e passa al branch della PR numero 123
    gh pr checkout 123
    ```

*   **Vedere lo stato e le modifiche di una PR:**
    ```bash
    gh pr status
    gh pr diff 123
    ```

*   **Fare la merge di una PR:**
    ```bash
    gh pr merge 123 --squash --delete-branch
    ```

#### 2. Gestire i Repository

*   **Clonare un repository in modo più intelligente:**
    Non devi copiare l'URL completo.
    ```bash
    gh repo clone aguagliardi/legopst
    ```

*   **Creare un nuovo repository su GitHub e clonarlo localmente:**
    ```bash
    # Crea un repo pubblico chiamato 'mio-nuovo-progetto' su GitHub
    gh repo create mio-nuovo-progetto --public --clone
    ```

#### 3. Gestire le Issue

*   **Creare una nuova issue:**
    ```bash
    gh issue create --title "Problema con il login" --body "L'utente non riesce ad accedere."
    ```

*   **Listare le issue:**
    ```bash
    gh issue list --assignee @me  # Mostra solo quelle assegnate a te
    ```

#### 4. Interagire con GitHub Actions

*   **Vedere l'elenco delle esecuzioni dei workflow:**
    ```bash
    gh run list
    ```

*   **Vedere i log di una specifica esecuzione fallita:**
    ```bash
    gh run view <RUN_ID> --log-failed
    ```

### Come si installa?

La maggior parte delle distribuzioni Linux lo rende molto semplice.

**Per Debian/Ubuntu:**
```bash
(type -p wget >/dev/null || (sudo apt update && sudo apt install wget -y)) && \
sudo mkdir -p -m 755 /etc/apt/keyrings && \
wget -qO- https://cli.github.com/packages/githubcli-archive-keyring.gpg | sudo tee /etc/apt/keyrings/githubcli-archive-keyring.gpg > /dev/null && \
sudo chmod go+r /etc/apt/keyrings/githubcli-archive-keyring.gpg && \
echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | sudo tee /etc/apt/sources.list.d/github-cli.list > /dev/null && \
sudo apt update && \
sudo apt install gh -y
```

**Per Fedora/RHEL/CentOS:**
```bash
sudo dnf install 'dnf-command(config-manager)'
sudo dnf config-manager --add-repo https://cli.github.com/packages/rpm/gh-cli.repo
sudo dnf install gh
```

**Per Arch Linux:**
```bash
sudo pacman -S github-cli
```

### Primo Utilizzo: Autenticazione

La prima volta che usi `gh`, dovrai autenticarti. È un processo guidato e sicuro:
```bash
gh auth login
```
Ti chiederà di accedere a GitHub.com, con quale protocollo (HTTPS o SSH), e poi aprirà il tuo browser per completare l'autenticazione tramite OAuth. È molto semplice e sicuro.

In conclusione, `gh` è uno strumento quasi indispensabile per chiunque utilizzi GitHub seriamente dal terminale. Riduce il cambio di contesto (terminale ↔ browser) e rende i flussi di lavoro molto più fluidi ed efficienti.
