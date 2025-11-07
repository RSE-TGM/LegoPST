Certamente! L'equivalente ufficiale e diretto di `gh` per GitLab è **`glab`**.

**`glab` è la CLI ufficiale di GitLab.** Proprio come `gh` affianca `git` per le operazioni su GitHub, `glab` affianca `git` per portare l'intera esperienza di GitLab nel tuo terminale.

La filosofia è esattamente la stessa:
*   **`git`** gestisce il tuo repository locale (commit, branch, merge).
*   **`glab`** gestisce l'interazione con la tua istanza GitLab (gitlab.com o self-hosted): Merge Request, Issue, CI/CD, Snippet, ecc.

### Cosa puoi fare con `glab`? (Esempi Pratici)

Le funzionalità sono molto simili a quelle di `gh`, ma adattate alla terminologia e alle feature di GitLab.

#### 1. Gestire le Merge Request (MR)

Le Merge Request sono l'equivalente delle Pull Request.

*   **Creare una MR dal tuo branch attuale:**
    ```bash
    # Fa il push del branch e avvia una procedura guidata per creare la MR
    glab mr create --fill
    ```
    L'opzione `--fill` usa intelligentemente l'ultimo commit per compilare titolo e descrizione, velocizzando il processo.

*   **Listare le MR aperte:**
    ```bash
    glab mr list
    ```

*   **Fare il checkout di una MR per testarla localmente:**
    Anche qui, è un enorme risparmio di tempo.
    ```bash
    # Scarica e passa al branch della MR numero 456
    glab mr checkout 456
    ```

*   **Approvare una MR:**
    ```bash
    glab mr approve 456
    ```

*   **Fare la merge di una MR:**
    ```bash
    # Esegue la merge, fa lo squash in un unico commit e cancella il branch sorgente
    glab mr merge 456 --squash --remove-source-branch
    ```

#### 2. Gestire i Repository

*   **Clonare un repository:**
    Non serve l'URL completo, basta il percorso.
    ```bash
    glab repo clone group/my-project
    ```

*   **Creare un nuovo repository su GitLab:**
    ```bash
    glab repo create my-new-project --public
    ```

#### 3. Gestire le Issue

*   **Creare una nuova issue:**
    ```bash
    glab issue create -t "Fix API endpoint" -d "The /users endpoint returns 500." -l "bug,backend"
    ```
    (`-t` per il titolo, `-d` per la descrizione, `-l` per le etichette)

*   **Listare le issue assegnate a te:**
    ```bash
    glab issue list --assignee @me
    ```

#### 4. Interagire con GitLab CI/CD (Funzionalità potentissima)

Questa è un'area in cui `glab` brilla, dato che la CI/CD è una feature centrale di GitLab.

*   **Vedere lo stato dell'ultima pipeline del branch corrente:**
    ```bash
    glab ci status
    ```

*   **Vedere i log di un job specifico:**
    ```bash
    glab ci view --logs
    ```

*   **Rilanciare un job fallito:**
    ```bash
    glab ci retry <JOB_ID>
    ```

### Come si installa?

L'installazione è molto simile a quella di `gh`.

**Per Debian/Ubuntu:**
```bash
curl -sL https://raw.githubusercontent.com/profclems/glab/main/scripts/install.sh | sudo bash
```

**Per Fedora/RHEL/CentOS:**
```bash
sudo dnf config-manager --add-repo https://cli.rpm.gitlab.com/gitlab-cli.repo
sudo dnf install glab
```

**Per Arch Linux:**
```bash
sudo pacman -S gitlab-cli
```

**Per macOS (con Homebrew):**
```bash
brew install glab
```

### Primo Utilizzo: Autenticazione

La prima volta che lo usi, dovrai autenticarti con la tua istanza GitLab.

```bash
glab auth login
```
Il processo è guidato:
1.  Ti chiederà se vuoi autenticarti su **gitlab.com** o su un'istanza **self-hosted** (molto comune nelle aziende).
2.  Ti chiederà di creare un **Personal Access Token** dal tuo profilo GitLab.
3.  **Importante:** Quando crei il token sul sito di GitLab, assicurati di dargli i permessi (`scopes`) necessari. Per un uso completo, seleziona `api`, `read_repository` e `write_repository`.
4.  Incolla il token nel terminale e sarai pronto a partire.

### Tabella di Confronto: `gh` vs `glab`

| Azione | GitHub CLI (`gh`) | GitLab CLI (`glab`) | Note |
| :--- | :--- | :--- | :--- |
| Autenticazione | `gh auth login` | `glab auth login` | `glab` richiede un token manuale. |
| Creare PR/MR | `gh pr create` | `glab mr create` | |
| Listare PR/MR | `gh pr list` | `glab mr list` | |
| Checkout PR/MR | `gh pr checkout <id>` | `glab mr checkout <id>` | |
| Merge PR/MR | `gh pr merge <id>` | `glab mr merge <id>` | |
| Clonare Repo | `gh repo clone <org/repo>` | `glab repo clone <group/repo>`| |
| Creare Repo | `gh repo create <nome>` | `glab repo create <nome>` | |
| Vedere CI/Actions | `gh run list` | `glab ci status` / `glab ci list` | `glab` è molto più ricco di feature per la CI/CD. |

In sintesi, **`glab` è l'equivalente diretto e potentissimo di `gh` per l'ecosistema GitLab.** Se lavori con GitLab, è uno strumento che ti farà risparmiare moltissimo tempo.
