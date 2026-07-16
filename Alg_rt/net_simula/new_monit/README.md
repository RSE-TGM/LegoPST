# Alg_rt/net_simula — Master Monitor (`new_monit`)

Il monitor principale della sessione ([Alg_rt/net_simula/new_monit/](.)) espone i menù *Programs*, *Options*, ecc. Le opzioni di sessione sono in una struttura `OPTIONS_FLAGS` ([option.h](option.h)) persistita nel file binario **`.bi_options`** (`OPTION_FILE`) nella dir di lavoro del simulatore (`FILES_PATH`), letto all'avvio da `read_options()` (`SD_optload`).

## User Programs — comandi utente lanciabili dal monitor

Meccanismo per lanciare comandi shell arbitrari dal monitor, in due parti:

- **Esecuzione** — menù *Programs → User programs …* apre il pannello `programLauncher` ([programLauncher.c](programLauncher.c)) con fino a **8 pulsanti radio**, uno per comando configurato non vuoto ([`loadPrograms`](options.c#L960)). Selezione → `selectedCommand`; **Execute** → [`system(selectedCommand)`](programLauncher.c#L121). L'etichetta del pulsante è la stringa del comando.
- **Configurazione** — menù *Options → Edit* apre `optionSet`; nel selettore *Current Selection:* scegliere **User Programs** → pannello con **8 campi di testo** ([`add_opt_userprog`](options.c#L490), `optionUserprogText[i]`) precompilati con i comandi correnti. Si scrive la riga di comando nello slot.

**Vincoli**: max **8** programmi, max **100** caratteri per comando ([`MAX_USERPROG` / `MAX_USERPROG_LUN`](option.h#L21)). `system()` è **bloccante** e eredita cwd/ambiente di `new_monit` (dir simulatore): per una GUI/script terminare con **`&`** (es. `xterm &`). Slot vuoto = pulsante non mostrato (per rimuovere: svuota il campo e salva). Config **per-directory** (ogni sim ha il suo `.bi_options`).

## `optionSet` — Save / Load (editor opzioni)

Nell'editor *Options → Edit* i due comandi persistono/ripristinano l'intera struttura opzioni (non solo User Programs):

- **Save** ([`activateCB_optionSetMenuSavepb`](optionSet.c#L183)): `aggiorna_opzioni` copia i widget → struttura `options`, poi `SD_optsave3` scrive `.bi_options`.
- **Load** ([`activateCB_optionSetMenuLoadpb`](optionSet.c#L192)): `read_options()` **rilegge `.bi_options`** (ultimo stato salvato) nella struttura in memoria → **scarta le modifiche non salvate**, poi chiude l'editor. In precedenza dopo `DistruggiInterfaccia` veniva richiamato `aggiorna_opzioni(&options)`, che ricopiava i widget della pagina corrente dentro `options` vanificando il reload sulla pagina visualizzata: **richiamo rimosso** (fix), ora il Load ripristina l'intera struttura in modo completo.

Il copia widget→struttura avviene invece nel bottone *Apply* ([`activateCB_pushButton6`](optionSet.c#L165) → `aggiorna_opzioni`): **Save non lo richiama**, quindi il flusso è *modifica campi → Apply → Save*.
