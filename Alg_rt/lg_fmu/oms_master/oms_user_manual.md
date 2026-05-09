Ecco un manuale d'uso completo per l'orchestratore Python sviluppato, formattato in Markdown. Puoi salvarlo come `OMuser_manual.md`.

---

# Manuale Utente: OMSimulator Python Orchestrator (v4.0)

## 1. Introduzione
L'Orchestratore Python per **OMSimulator 3.0** è un sistema "Master" progettato per gestire co-simulazioni complesse basate sullo standard **FMI (Functional Mock-up Interface)**. 
Il sistema permette di connettere più unità di simulazione (FMU), configurare parametri, gestire loop di controllo interattivi (Something-in-the-loop) e recuperare automaticamente i file generati dalle FMU.

### Caratteristiche principali:
*   **Architettura Data-Driven**: Configurazione completa tramite file JSON.
*   **Supporto Multi-FMU**: Caricamento e connessione di N modelli.
*   **Gestione Risorse**: Recupero automatico dei file dalla cartella `resources` di ogni FMU.
*   **Logging Ibrido**: Un unico file CSV con dati FMU e calcoli effettuati in Python.
*   **Failsafe**: Arresto immediato in caso di errore di inizializzazione di una FMU.

---

## 2. Struttura dei File
Il sistema si compone di tre elementi principali:
1.  **`OMSMaster_v4.py`**: Il motore dell'orchestratore.
2.  **`config.json`**: Il file di configurazione del sistema.
3.  **FMU Files (`.fmu`)**: I modelli compilati (es. `legoclix_MDC_GV_bundle.fmu`).

---

## 3. Guida alle Variabili (Naming Convention)
OMSimulator segue regole rigide per identificare variabili e parametri.

### 3.1 Anatomia del Percorso (Path)
Ogni variabile deve essere richiamata tramite il suo **Full Path** testuale:
`[NomeModello].root.[NomeIstanza].[NomeVariabile]`

*   **NomeModello**: Definito nel JSON (`model_name`).
*   **root**: Livello gerarchico obbligatorio in OMSimulator.
*   **NomeIstanza**: Il nome breve assegnato alla FMU nel JSON (es. `MDC`).
*   **NomeVariabile**: Il nome reale della variabile definito all'interno della FMU.

### 3.2 Regole di Causalità (Causality)
A seconda della natura della variabile (definita nello standard FMI), l'orchestratore si comporta in modo diverso:

| Causalità | Tipo FMI | Funzione Python | Descrizione |
| :--- | :--- | :--- | :--- |
| **Input** | `input` | `set_value()` | Segnali che la FMU riceve dall'esterno. Scrivibili in ogni momento. |
| **Output** | `output` | `get_value()` | Segnali calcolati dalla FMU. Sola lettura. |
| **Parameter** | `parameter` | `set_value()` | Valori di configurazione (es. masse, costanti). Solitamente scrivibili solo prima dell'inizializzazione. |
| **Local** | `local` | `get_value()` | Variabili interne di stato. Sola lettura. |


---

## 3.3 Identificazione e Costruzione delle Variabili

Per interagire con una FMU, è necessario conoscere i nomi esatti delle variabili definiti dal fornitore del modello.

### A. Come viene costruito il nome (Hierarchy)
OMSimulator utilizza una struttura a "punti" per navigare nella gerarchia del sistema. Il nome completo (Full Path) che Python deve usare è composto da 4 segmenti:

1.  **Model Name**: Il nome del progetto (definito in `config.json` -> `model_name`).
2.  **root**: La radice del sistema (obbligatoria in OMSimulator).
3.  **Instance Name**: Il nome che hai dato alla FMU nel JSON (es. `MDC`).
4.  **Variable Name**: Il nome interno della variabile nell'FMU.

**Esempio:** `SistemaMDC.root.MDC.sensore_temperatura`

---

### B. Dove trovare i nomi (Sorgenti di Verità)

Esistono tre modi per identificare i nomi delle variabili disponibili:

#### 1. Il file `modelDescription.xml` (Metodo più preciso)
Ogni file `.fmu` è in realtà un archivio compresso (ZIP).
1.  Apri il file `.fmu` con un software di archiviazione (es. 7-Zip, WinRAR o `unzip`).
2.  Estrai il file **`modelDescription.xml`**.
3.  Cerca il tag `<ScalarVariable>`. Ogni variabile è definita così:
    ```xml
    <ScalarVariable name="motore.velocita" valueReference="16" causality="output">
       <Real unit="rad/s"/>
    </ScalarVariable>
    ```
    *   Il valore dell'attributo **`name`** (`motore.velocita`) è quello da usare nello script.
    *   L'attributo **`causality`** indica come usarla (vedi tabella sotto).

#### 2. Ispezione tramite OMEdit (Interfaccia Grafica)
Se hai installato OpenModelica:
1.  Apri **OMEdit**.
2.  Trascina l'FMU nell'area di lavoro.
3.  Nel browser dei componenti a sinistra, espandi l'FMU: vedrai l'elenco completo di ingressi (frecce verso l'interno), uscite (frecce verso l'esterno) e parametri (icona chiave inglese).

#### 3. Ispezione tramite CLI (Riga di comando)
Puoi usare il terminale per interrogare rapidamente l'FMU senza scompattarla:
```bash
OMSimulator --list-variables legoclix_MDC_GV_bundle.fmu
```

---

### C. Mappatura Causalità -> Funzione Python
Una volta trovato il nome, devi usare la funzione corretta in base alla sua `causality`:

| Causalità nell'XML | Ruolo | Funzione Master Pro |
| :--- | :--- | :--- |
| `input` | Ingressi dinamici (es. voltaggio, setpoint) | `set_value()` |
| `output` | Risultati calcolati (es. velocità, pressione) | `get_value()` |
| `parameter` | Costanti fisiche o setup (es. massa, guadagno) | `set_value()` (solo in fase di setup) |
| `calculatedParameter` | Parametri derivati da altri parametri | `get_value()` |
| `local` | Variabili di stato interne o di debug | `get_value()` |

---

### D. Note speciali sui nomi generati da Modelica
Se l'FMU è stata generata da **OpenModelica**, i nomi seguono la struttura dei blocchi:
*   Se hai un componente `pompa1` che ha un parametro `V_flow`, il nome sarà `pompa1.V_flow`.
*   Se la variabile fa parte di un connettore (es. un porto meccanico o elettrico), il nome potrebbe includere il nome del connettore: `porto_a.v` (velocità al porto A).

> **Attenzione:** I nomi delle variabili sono **Case Sensitive** (distinguono tra maiuscole e minuscole). `Temperatura` e `temperatura` sono considerate variabili diverse.

---
---

## 4. Configurazione (`config.json`)
Il file JSON permette di definire la simulazione senza modificare il codice.

```json
{
  "model_name": "Sistema_Test",
  "settings": {
    "stop_time": 30.0,
    "step_size": 1.0,
    "harvest_interval": 5.0
  },
  "fmus": {
    "MDC": "modello_fisico.fmu",
    "CTRL": "controller.fmu"
  },
  "parameters": {
    "MDC.guadagno": 1.5
  },
  "connections": {
    "CTRL.segnale_u": ["MDC.ingresso_v", "MDC.ingresso_w"]
  }
}
```
*   **`fmus`**: Mappa i nomi istanza ai file fisici.
*   **`connections`**: Definisce i collegamenti. Supporta connessioni **1-a-molti** (un output a più input).
*   **`harvest_interval`**: Ogni quanti secondi di simulazione l'orchestratore deve copiare i file prodotti dalle FMU.

---

## 5. Funzionalità Avanzate

### 5.1 Loop Interattivo (Something-in-the-loop)
L'orchestratore esegue un ciclo `while` basato sulla funzione `stepUntil`. Ad ogni iterazione:
1.  Le FMU avanzano nel tempo.
2.  Python legge i dati tramite `get_value`.
3.  Python esegue calcoli personalizzati.
4.  Python può iniettare nuovi valori tramite `set_value`.
5.  I dati vengono salvati nel log unificato.

### 5.2 Recupero Risorse (Harvesting)
OMSimulator 3.0 estrae le FMU in directory temporanee con nomi casuali (es. `MDC_Simulation-400zcmwg`). 
L'orchestratore risolve questo problema mappando automaticamente:
1.  L'indice di caricamento (`0001`, `0002`).
2.  Il percorso `temp/000X_NomeIstanza/resources`.
3.  Copia i file in una cartella permanente `RISULTATI_SISTEMA/[NomeIstanza]`.

---

## 6. Esecuzione
Per avviare il sistema, assicurarsi che l'ambiente Python abbia accesso alle librerie di OMSimulator e lanciare:

```bash
python OMSMaster_v4.py
```

### Output generati:
*   **`log_ibrido.csv`**: File contenente i segnali temporali di tutte le FMU e i calcoli Python.
*   **`RISULTATI_SISTEMA/`**: Cartelle divise per istanza contenenti i file prodotti (log interni, database, etc.).
*   **`log_simulazione.csv`**: Log nativo di OMSimulator (opzionale).

---

## 7. Troubleshooting e Ottimizzazione
*   **Errore Status > 1**: L'orchestratore si ferma se provi a scrivere su una variabile di `output` o se il percorso della variabile è errato.
*   **Lentezza**: Se la simulazione è lenta, aumentare lo `step_size` nel JSON o attivare le tecniche di "Buffered Logging" descritte nei commenti del codice (sezione Optimization).
*   **File non trovati**: Verificare che la FMU sia compilata per il sistema operativo in uso (Linux/GCC).

---
*Manuale aggiornato al: 09/05/2026*
*Versione Orchestratore: 4.0 (Stable)*
