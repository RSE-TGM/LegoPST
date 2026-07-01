# Copia/incolla nei campi di testo di `config`

L'applicazione `config` (costruzione/modifica degli schemi di regolazione,
tipicamente sulle task `r_*` della struttura `legocad` dell'utente) è una GUI
Motif/Xlib legacy. I suoi campi di input sono widget Motif standard
(`XmTextField` monoriga, `XmText` multiriga): la clipboard di sistema è quindi
già supportata, ma di default Motif la lega solo ai **tasti legacy**.

Per comodità sono state aggiunte le scorciatoie moderne **Ctrl+C / Ctrl+X /
Ctrl+V** senza toccare il codice C dell'applicazione: la modifica è puramente
nel file di risorse X (app-defaults) `Config`.

## Come si usa

In qualsiasi campo di testo di `config` (nomi, path, descrizioni, area
multiriga, dialog di contesto, ecc.):

| Azione | Scorciatoia (aggiunta da questa modifica) |
|---|---|
| **Copia** | `Ctrl+C` |
| **Taglia** | `Ctrl+X` |
| **Incolla** | `Ctrl+V` |

In più (comportamento nativo X11, sempre disponibile):

- **Primary selection**: seleziona il testo trascinando col mouse, poi
  **incolla col tasto centrale** dove serve. Non richiede Ctrl+C.
- **Cross-applicazione**: `Ctrl+C`/`Ctrl+V` usano la selezione ICCCM `CLIPBOARD`,
  quindi puoi copiare da/verso browser, editor e altre app moderne.

Nota: su un campo non editabile, `Ctrl+V`/`Ctrl+X` non fanno nulla (comportamento
sicuro); `Ctrl+C` copia comunque il testo selezionato.

## Qual è il default Motif (senza questa modifica)

Utile per capire *perché* serviva l'intervento e cosa aspettarsi su altre
postazioni.

I widget `XmText`/`XmTextField` **hanno già** le azioni clipboard
`copy-clipboard()` / `cut-clipboard()` / `paste-clipboard()` nella loro
translation table di default, ma legate a **virtual key** Motif, non a tasti
fisici:

```
:<Key>osfCopy:   copy-clipboard()
:<Key>osfCut:    cut-clipboard()
:<Key>osfPaste:  paste-clipboard()
```

Quali tasti fisici corrispondano a `osfCopy`/`osfCut`/`osfPaste` dipende dalle
**"virtual bindings"** di Motif del sistema:

- **Storicamente** (molte installazioni Unix/Motif) erano mappati su
  **Ctrl+Ins** (copia), **Shift+Canc** (taglia), **Shift+Ins** (incolla).
- **Su questa postazione** (open-motif 2.3.8, Fedora) il fallback interno lascia
  `osfCopy`/`osfCut`/`osfPaste` **unbound** (verificato: `_MOTIF_BINDINGS` non
  impostata sul server X, nessun `~/.motifbind`, e il default in
  `man VirtualBindings` li dà "unbound"). Di conseguenza **da tastiera non
  funziona alcun copia/incolla di serie**: l'unico meccanismo nativo è la
  **primary selection col mouse** (evidenzia + tasto centrale). È esattamente
  il motivo per cui l'operazione sembrava non supportata.

Perciò questa modifica lega `Ctrl+C/X/V` **direttamente ai tasti fisici** (non
ai virtual key): funziona indipendentemente dalle virtual bindings del sistema.
In alternativa si sarebbero potuti definire i virtual key con `~/.motifbind` o
`xmbind`, ma sarebbe stato più fragile e per-utente.

## Dove vive la modifica

La modifica è nel file di risorse `Config` (app-class `Config`, caricata via
`XAPPLRESDIR`). Righe aggiunte:

```
Config*XmText.translations: #override \n\
	Ctrl<Key>c: copy-clipboard()\n\
	Ctrl<Key>x: cut-clipboard()\n\
	Ctrl<Key>v: paste-clipboard()
Config*XmTextField.translations: #override \n\
	Ctrl<Key>c: copy-clipboard()\n\
	Ctrl<Key>x: cut-clipboard()\n\
	Ctrl<Key>v: paste-clipboard()
```

Copie del file (da tenere allineate):

- **Runtime** (quella effettivamente letta): `~/risorse/Config`
  — perché `.profile_legoroot` imposta `XAPPLRESDIR=~/risorse` (dopo aver
  sorgiato `Alg_env.sh` che invece punterebbe a `$LEGOCAD/risorse`).
- **Sorgente versionato**: `legocad/risorse/Config`
- **Copia versionata**: `util2025/risorse/Config`

Per farla valere su un'altra postazione, copiare il `Config` aggiornato nella
directory indicata da `XAPPLRESDIR` di quell'utente. Non serve ricompilare
`config`: basta **riavviare** l'applicazione (le risorse X si leggono all'avvio).

## Perché è a basso rischio

- **Nessun codice C toccato, nessuna ricompilazione**: l'app legacy resta
  invariata. `copy/cut/paste-clipboard()` sono azioni **built-in** di
  `XmText`/`XmTextField`.
- **`#override` (non `#replace`)**: aggiunge solo Ctrl+C/X/V, lasciando intatte
  tutte le translation di default (tasti legacy, navigazione, `Return` →
  callback di attivazione, ecc.).
- **Ambito circoscritto**: il selettore `Config*Xm…` riguarda solo i widget di
  testo dell'app `config`; le scorciatoie di **disegno** dello schema (F5, F11,
  frecce, connessioni) sono su altri widget e restano invariate.
- Verificato con un test Motif isolato: la tabella parsa e si applica su
  `XmText`/`XmTextField` reali con **0 warning**.

## Disattivare / rollback

Rimuovere i due blocchi `translations` dal file `Config` e riavviare `config`.
Nessun binario coinvolto.
