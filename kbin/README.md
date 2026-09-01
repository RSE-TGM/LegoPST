# `kbin/` — riferimento dei comandi (LegoPST / kprocedure)

I 189 script Korn-shell dell'ambiente **k** di LegoPST: build, esecuzione, SCADA/MMI e
manutenzione di un simulatore d'impianto. Questa è la mappa dello **scopo di ciascuno**,
raggruppato per funzione.

> Ricostruito ispezionando header, stringhe `kAddScreen`, operazioni e tool invocati dei
> singoli script. Alcune voci minori (`kDiffusion`, `kReadDB`, `kPlace`) sono inferite dal
> nome e dal contesto.

## Convenzioni di nome

| Marca | Significato |
|---|---|
| **main** | comando principale (CamelCase), es. `kExport`, `kCompile`, `kMmi` |
| **bg** | wrapper minuscolo di 3 righe che lancia il main **in background** (`. $KBIN/kExport $* &`) |
| **slave** | sotto-passo interno di un main (es. `kDiffS01Slave1..6`) |
| **helper** | primitiva usata da altri script (`kAddLog`, `kAddScreen`, …) |
| **util** | utility a sé |
| **index** | indicizzatore pagine: scandisce i `????.pag` e costruisce le liste in `$KINFOTAG` |
| **deprecato** | non più da usare, rimanda ad altro comando |

Ambiente: le variabili `K*` derivano da `KSIM` (vedi `ksetsim` qui sotto).

---

## 0. Ambiente — scelta del simulatore corrente

> Queste **non** sono script in `kbin/`: sono **funzioni shell** definite in
> [`Alg_env.sh`](../Alg_env.sh) (sorgiato dal profilo). Le elenco qui perché sono il punto
> d'ingresso dell'ambiente k: molti comandi kbin fanno `cd $KSIM` e ne dipendono.

| Comando | Tipo | Scopo |
|---|---|---|
| `ksetsim <nome\|/path>` | func | Imposta il **simulatore corrente** (`KSIM`) sotto `$KSKED` e **ri-deriva tutte le `K*`** (`KLOG`, `KSTATUS`, `KSCADA`, `KDATABASES`, `KPAGES`, `KGRAF`, …). Crea `status/`/`log/` mancanti, sorgia l'override per-sim `$KSIM/ksim.conf`, e memorizza la scelta in `~/.legosim` (sticky tra shell). Completion bash sui nomi disponibili. |
| `ksims` | func | Elenca i simulatori disponibili (solo directory sotto `$KSKED`, default `$HOME/sked`). |
| `ksetsim_default` | func | Chiamata dal profilo all'avvio: sceglie il simulatore in cascata `~/.legosim` → `cassano0` → **primo disponibile** (`ksims`); se `$KSKED` è vuoto avvisa e lascia `KSIM` non impostata. |

```bash
ksetsim SLaurentB1     # passa a $KSKED/SLaurentB1 e riallinea tutte le K*
ksims                  # elenca i simulatori
kDiffS01               # ora cd $KSIM trova l'S01
```

---

## 1. Costruzione & compilazione del simulatore

| Comando | Tipo | Scopo |
|---|---|---|
| `kCompile` | main | Compila il simulatore (invoca `net_compi`). |
| `kCompileSim` | main | Compila l'intero simulatore. |
| `kNetCompi` | main | Compila il simulatore di rete (`net_compi`). |
| `knetcompi` | bg | Avvia kNetCompi in background. |
| `knet_compi` | bg | Avvia kNetCompi in background. |
| `kConnex` | main | Costruisce il database delle connessioni (interconnessioni tra task). |
| `kconnex` | bg | Avvia kConnex in background. |
| `kConnexSlave1` | slave | Sotto-passo di kConnex. |
| `LimpiaConnexCiclo` | util | Ripulisce i cicli nel DB connessioni (filtra `connex2.out`). |
| `kMakeConnDB` | main | Costruisce il DB delle connessioni. |
| `kmakeconndb` | bg | Avvia kMakeConnDB in background. |
| `kMakeConnDBSlave1` | slave | Sotto-passo di kMakeConnDB. |
| `k_crea_simulatore` | main | Crea la struttura di un nuovo simulatore. |
| `kcreastaz` | main | Crea la «stazione» (station) del simulatore. |
| `kCheckSimulator` | main | Verifica coerenza e completezza del simulatore. |
| `kCheckSimulatorSlave1` | slave | Sotto-passo di kCheckSimulator. |
| `kCheckRegoTask` | main | Verifica le task di regolazione. |
| `kDiffS01` | main | Coerenza dei valori di stazionario delle variabili di interconnessione tra le task (via `S01`). |
| `kDiffS01Slave1` | slave | Costruisce `kDiffS01.DB` dai `f24.dat` delle task (LEGO & REGO). |
| `kDiffS01Slave2` | slave | Costruisce il DB per le task SID. |
| `kDiffS01Slave4` | slave | `PROGRAM DIFFS01`: legge S01+DB, produce `diffs01.out` (il motore). |
| `kDiffS01Slave5` | slave | Sotto-passo di kDiffS01. |
| `kDiffS01Slave6` | slave | Costruisce il DB per le task GIPS. |
| `koldlg5` | util | Compila `proc/lg5` legacy (`cad_maketask`). |
| `kCompStaz` | main | Compila i faceplate (`r01.dat` -> `r02.dat`) per `xstaz`, con un exit status utilizzabile: `compstaz` da solo esce con 24 anche quando riesce. |

## 2. Esecuzione, avvio & lancio strumenti

| Comando | Tipo | Scopo |
|---|---|---|
| `kStart` | main | Avvia il simulatore. |
| `kstart` | bg | Avvia kStart in background. |
| `kRun` | main | Avvia una sessione del simulatore (sorgia `~/.profile`). |
| `kMmi` | main | Avvia la MMI (`mmi`/`client_mmi`). |
| `kmmi` | bg | Avvia kMmi in background. |
| `kXlego` | main | Avvia `xlego` (ambiente grafico legocad) su `$KSIM`. |
| `kxlego` | bg | Avvia kXlego in background. |
| `kControlM` | main | Elaborazione/lancio via ControlM (scheduling). |
| `kc` | util | Scorciatoia: lancia `config` sulla task legocad indicata (con `f01.dat`). |
| `kconfig` | util | Lancia il tool `config` con gli argomenti dati. |
| `kgr` | util | Lancia `graphics` su `$KSIM/f22circ` (trend/plot). |
| `kmandb` | util | Lancia `mandb` (database manovre) su `$KSIM`. |
| `kdxstart` | util | Apre un terminale `dtterm` personalizzato. |
| `kdxterm` | util | Apre un terminale `dxterm` personalizzato. |

## 3. SCADA / MMI — pagine, finestre, curve, navigazione

| Comando | Tipo | Scopo |
|---|---|---|
| `kScd` | main | Build/verifica del sottosistema SCADA. |
| `kscd` | bg | Avvia kScd in background. |
| `kScadaInit` | main | Inizializza il database SCADA (previo `kclean`). |
| `kscadainit` | bg | Avvia kScadaInit in background. |
| `kMmiConfig` | main | Configurazione della MMI. |
| `kCollect` | main | Configurazione multi-MMI (raccolta pagine/config). |
| `kcollect` | bg | Avvia kCollect in background. |
| `kChangeMmiColor` | main | Cambia le definizioni di colore della MMI. |
| `kAddWidget` | helper | Helper MMI: aggiunge un widget nella configurazione. |
| `kAddChildrenWidget` | helper | Helper MMI: aggiunge i widget «figli» nella configurazione. |
| `kMakeGlobpages` | main | Costruisce le «global pages» (pagine globali MMI). |
| `kGlobContext` | main | Costruisce il context globale della MMI (`Context.ctx`). |
| `kWinContext` | main | Costruisce il context delle finestre MMI. |
| `kMakeWin` | main | Costruisce le liste finestre MMI (in `$KWIN`). |
| `kOw` | main | Costruisce le Operating Window (finestre operative MMI). |
| `kStazPages` | main | Porta in `$KWIN` i faceplate descritti nei `r01.dat` (via `convstaz -d`), col prefisso `O_`, pronti per `kWinContext` + compilazione + `kCollect`. |
| `kPlace` | main | Piazzamento (placement) di pagine/widget MMI. |
| `kpag` | util | Costruisce la lista pagine (`Context.ctx`) dai `????.pag`. |
| `kRenamePage` | main | Rinomina una pagina MMI. |
| `kMakeCurve` | main | Costruisce le definizioni di curve/trend (in `$KGRAF`). |
| `kMakeCurveSlave0` | slave | Sotto-passo di kMakeCurve. |
| `kMakeCurveSlave1` | slave | Sotto-passo di kMakeCurve. |
| `kMakeCurveSlave2` | slave | Sotto-passo di kMakeCurve. |
| `kMakeRecorder` | main | Costruisce la configurazione del recorder (registratore variabili). |
| `kMakeRecorderSlave1` | slave | Estrazione variabili da `recorder.edf`. |
| `kMakeRecorderSlave2` | slave | Genera `kRecorder_KKS.txt`. |
| `kMakeRecorderSlave3` | slave | Sotto-passo di kMakeRecorder. |
| `kMakeRecorderSlave4` | slave | Genera `kRecorder_VAR.txt`. |
| `kMakeRecorderSlave5` | slave | Elaborazione `Recorder.edf.new`. |
| `kMakeRecorderSlave6` | slave | Sotto-passo di kMakeRecorder. |
| `kMakePdList` | main | Costruisce la lista dei plant display. |
| `kcreabasic` | main | Costruisce la «superlist» plant-display dai `win*.list` (`creasuperlist2`). |
| `k_crea_cassaforte` | main | Crea la «cassaforte» grafica (`$KCASSAFORTE`: curve, plant_display). |
| `kCheckCaiVarPD` | main | Verifica le variabili CAI nel plant display. |
| `kMakeCaiVar` | main | Costruisce il database CaiVar (variabili CAI). |
| `kmakecaivar` | bg | Avvia kMakeCaiVar in background. |
| `kUpDateNavigation` | main | Costruisce/aggiorna il DB di navigazione MMI. |
| `kupdatenavigation` | bg | Avvia kUpDateNavigation in background. |
| `kUpDateNavigationSlave1` | slave | Aggiornamento navigazione di ingresso. |
| `kUpDateNavigationSlave2` | slave | Individua le pagine «non in uso». |
| `kNavigation` | main | Capacità di navigazione per una task. |
| `kNavigationAll` | main | Capacità di navigazione per tutte le task. |
| `kUpDateCaiHierarchy` | main | Aggiorna la gerarchia CAI (navigazione + plant display). |
| `kUpDateCaiHierarchySlave1` | slave | Aggiorna la Cai Navigation. |
| `kUpDateCaiHierarchySlave2` | slave | Aggiorna la gerarchia del PlantDisplay. |
| `kUpDatePD` | main | Aggiorna il plant display (`Context.ctx`, `mmi`). |
| `kupdatepd` | bg | Avvia kUpDatePD in background. |
| `kDirect` | main | Costruisce i collegamenti diretti (merge `DirLinks*.list`). |
| `kCheckDirect` | main | Verifica i collegamenti diretti (direct links). |
| `kModRtf` | main | Modifica i file RTF (tabelle operatore). |
| `kmodrtf` | bg | Avvia kModRtf in background. |
| `kLeeF22` | main | Legge/verifica il file grafico F22 (variabili di grafica). |
| `kLeeF22Slave1` | slave | Verifica (sotto-passo di kLeeF22). |
| `kMalfunctionGroupWindow` | main | Costruisce la finestra dei gruppi di malfunzioni. |
| `kMalfunctionGroupWindowSlave2` | slave | Aggiorna l'altezza della finestra. |
| `kMalfunctionGroupWindowSlave3` | slave | Sotto-passo di kMalfunctionGroupWindow. |
| `kMalfunctionGroupWindowSlave4` | slave | Compone tag/descrizioni delle malfunzioni. |
| `kSinottico` | main | Converte l'attributo «Sinottico» in «Teleperm» nelle pagine. |
| `kTeleperm` | main | Converte l'attributo «Sinottico» in «Teleperm» (come kSinottico). |
| `unistaz` | main | Aggiorna i `win*.list`/`mal*.list` globali dalle task di regolazione. |

## 4. Configurazione, installazione, archivio, utenti

| Comando | Tipo | Scopo |
|---|---|---|
| `kInstall` | main | Installazione del simulatore. |
| `kExport` | main | Esporta il simulatore in `$KEXPORT`. |
| `kexport` | bg | Avvia kExport in background. |
| `kExport_new` | main | Variante nuova dell'export. |
| `kArchive` | main | Archivia un simulatore in `$KARCHIVE`. |
| `kArchiveDisponibility` | slave | Verifica spazio/disponibilità per l'archiviazione. |
| `kBackupSim` | main | Backup di un simulatore «ready to start» (sked, modelli, …). |
| `kMakeRemoteSim` | main | Prepara un simulatore remoto. |
| `kSimMove` | util | Sposta/rinomina un simulatore. |
| `kUpDateHostName` | main | Aggiorna l'hostname nel simulatore (`S01`). |
| `kUpDateLibrary` | main | Aggiorna le librerie legocad (`libut…`). |
| `kMakeLicense` | main | Genera la licenza (`config`). |
| `kDiffReport` | main | Report di confronto/differenze del simulatore. |
| `kProcessReport` | main | Report delle task di processo. |
| `kMakeUser` | main | Crea/inizializza un utente (copia `.dt`, …). |
| `kuser` | util | Imposta la chiave utente di sessione (`USR_KEY`/`SHR_USR_KEY`). |
| `kUpDateUserName` | main | Aggiorna il nome utente. |
| `kChangeUserLevel` | main | Cambia il livello utente («Make User Level»). |

## 5. Ricerca & interrogazione (KKS, tag, indici pagine)

| Comando | Tipo | Scopo |
|---|---|---|
| `Kks` | main | Cerca un tag KKS nel simulatore (`-h` per la guida). |
| `kks` | bg | Alias/wrapper di Kks. |
| `KKS` | bg | Alias/wrapper di Kks. |
| `kko` | util | Variante di kks (ricerca KKS in legocad). |
| `kFindKks` | main | Trova un KKS (tag) nel simulatore. |
| `kFindDuplicatedTag` | main | Trova i tag duplicati. |
| `Key` | util | Cerca una parola-chiave nei file topologici legocad. |
| `tag` | util | Cerca un tag in `TAG.list`. |
| `S01` | util | Cerca una key nel file `S01`. |
| `li` | util | Cerca pagine plant_display «li» per KKS. |
| `ow` | util | Cerca le Operating Window per KKS nel plant_display. |
| `rp` | util | Cerca le Report Page (`F_`) per KKS nel plant_display. |
| `grepr` | util | grep ricorsivo: cerca una stringa in tutti i file. |
| `xFindKks` | index | Indicizza i KKS dai `????.pag` → `KKS.list`. |
| `xFindTag` | index | Indicizza i tag → `TAG.list`. |
| `xFindTask` | index | Indicizza le pagine per task → `TASK.list`. |
| `xFindDescr` | index | Indicizza le descrizioni → `DESCR.list`. |
| `xFindPdDescr` | index | Indicizza le descrizioni plant display → `PDDESCR.list`. |
| `xFindLi` | index | Indicizza le info «li» → `LI.list`. |
| `xFindOw` | index | Indicizza le Operating Window → `OW.list`. |
| `xFindRp` | index | Indicizza le Report Page → `RP.list`. |
| `xFindRev` | index | Indicizza le revisioni → `REV.list`. |
| `ktaskinfo` | util | Raccoglie i file `task.info` in una lista. |

## 6. Pulizia & manutenzione

| Comando | Tipo | Scopo |
|---|---|---|
| `kClean` | main | Pulizia del simulatore (status, temporanei). |
| `kclean` | bg | Avvia kClean in background. |
| `kCleanLoc` | main | Pulizia «locale» (temporanei nella dir corrente). |
| `kcleanloc` | bg | Avvia kCleanLoc in background. |
| `kPulirSim` | main | Pulisce l'intero simulatore. |
| `kPulirProc` | main | Pulisce le task di processo. |
| `kPulirProcAll` | main | Pulisce tutte le task di processo. |
| `kPulirProcSid` | main | Pulisce le task di processo SID (`rm -rf proc`). |
| `kPulirProcSidAll` | main | Pulisce tutte le task di processo SID. |
| `kPulirRego` | main | Pulisce le task di regolazione (rimuove `ow`, …). |
| `kPulirRegoAll` | main | Pulisce tutte le task di regolazione. |
| `kSuperPipo` | util | Pulisce tutte le task di regolazione `r_*`. |
| `kDangerPipo` | util | Pulizia task di regolazione, protetta da chiave `7777`. |
| `mkill` | util | Pulizia di una simulazione andata male (`killsim` + `kclean`). |
| `pu` | util | Conta/pulisce i file di backup (`*bak`, `*kold`, `*tmp`). |
| `FindCore` | util | Trova i file `core` (dump) sotto la home. |
| `RemoveCore` | util | Rimuove i file `core` sotto la home. |
| `ksosti` | deprecato | Rimanda a kCheckAlarm (sostituzioni liste win). |

## 7. Infrastruttura interna & helper

| Comando | Tipo | Scopo |
|---|---|---|
| `kHelp` | main | Menu di aiuto interattivo delle procedure k. |
| `kTest` | main | Verifica l'ambiente k (scrive `kTest.status`). |
| `ktest` | bg | Avvia kTest in background. |
| `kpresentation` | helper | Stampa il banner/intestazione delle procedure. |
| `kAddLog` | helper | Accoda una riga al log della procedura (`$KLOG/<proc>.log`). |
| `kAddScreen` | helper | Stampa a schermo una riga di avanzamento della procedura. |
| `kAddStatus` | helper | Scrive lo stato di uno step in `$KSTATUS/<x>.status`. |
| `kAddStatistic` | helper | Registra l'uso di un comando (statistiche). |
| `kAddInfo` | helper | Registra informazioni di stato/uso. |
| `kSpace` | helper | Ritorna l'indentazione (spazi) per la formattazione. |
| `kOk` | util | Resetta/imposta gli stati degli step (`kAddStatus … Reset/OK`). |
| `kStat` | main | Statistiche d'uso dei comandi. |
| `kStatSlave1` | slave | Sotto-passo di kStat. |
| `astat` | deprecato | Rimanda a kStat. |
| `kCheckAlarm` | main | Verifica la configurazione allarmi (7 controlli via Slave0..7). |
| `kcheckalarm` | bg | Avvia kCheckAlarm in background. |
| `kCheckAlarm_new` | main | Variante nuova del check allarmi (`MMI_K_FA_LIST`). |
| `kCheckAlarmSlave0` | slave | Verifica allarmi n.0. |
| `kCheckAlarmSlave1` | slave | Verifica allarmi n.1. |
| `kCheckAlarmSlave2` | slave | Verifica allarmi n.2. |
| `kCheckAlarmSlave3` | slave | Verifica allarmi n.3. |
| `kCheckAlarmSlave4` | slave | Verifica allarmi n.4. |
| `kCheckAlarmSlave5` | slave | Verifica allarmi n.5. |
| `kCheckAlarmSlave6` | slave | Verifica allarmi n.6. |
| `kCheckAlarmSlave7` | slave | Verifica allarmi n.7. |
| `kReadDB` | util | Legge un database k (binario, da `kReadDB.c`). |
| `kDiffusion` | util | Utility minore (richiama `kks -h`). |
| `kdbx` | util | Lancia il debugger (`lg5debug`). |
| `kdu` | util | Uso disco per sottodirectory (`du -s` formattato). |
| `ksortlist` | util | Ordina in-place tutti i file `*.list`. |
| `lr` | util | «list relevant»: `ls` filtrato (esclude pag/bkg/rtf/reg_err). |
| `mye` | util | «my editor»: apre `asedit`/`dtpad` su un file. |
| `slash` | util | Stampa un backslash (utility banale). |

---

*Totale: 191 file — comandi principali (CamelCase), wrapper background (minuscoli),
sotto-passi (`*SlaveN`), helper e utility. Versione navigabile con ricerca disponibile
come Artifact.*
