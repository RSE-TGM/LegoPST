# lghmi.tcl - selettore grafico delle task LegoPST -> lancia la HMI draw2gr.
# Si avvia con l'helper "lghmi" (Alg_rt/bin/lghmi), che sorgia il profilo e fa
# `wish $LG_TIX/lghmi.tcl`.
#
# Due modalita', scelte automaticamente in base alla cwd di lancio:
#
#  * Modalita' S01 (se nella cwd esiste un file "S01"): la lista delle task
#    viene letta dal file S01, che descrive un simulatore complessivo composto
#    da piu' task lego. Vengono elencate SOLO le task di PROCESSO (tipo P) -
#    le uniche ad avere un file di topologia .tom. Ogni task viene lanciata
#    dalla propria directory modello (path relativo risolto rispetto al S01).
#
#  * Modalita' dir-scan (default, nessun S01): elenca le sottodir di
#    $LG_TASKROOT (default $HOME/legocad) che contengono almeno un *.tom (le
#    dir di libreria come libgraph/libut, prive di .tom, sono quindi escluse).
#
# Con l'opzione -staz il selettore cambia oggetto: invece delle task elenca le
# PAGINE DI FACEPLATE (stazioni di comando) compilate in r02.dat, e apre quella
# scelta con xstaz. Serve a chi gestisce la simulazione con net_startup, che
# monta il banco (new_monit) e non ha il dialogo delle stazioni di net_monit.
#
# La barra in basso ha anche un pulsante "mmi" che lancia l'applicazione MMI
# (Alg_mmi): non dipende dalle liste, sceglie la directory di lavoro fra $KPAGES,
# $LG_SIM_PATH/globpages, $KPAGES, ./globpages e la cwd, perche' mmi legge il
# Context.ctx della dir da cui parte.
#
# Accanto c'e' il pulsante "net_startup", che lancia la simulazione della
# directory corrente. E' abilitato solo dove esiste variabili.rtf, il file che
# net_startup controlla per primo, e chiede sempre conferma perche' comincia con
# killsim (che cancella tutte le SHM dell'utente). La simulazione parte in una
# SESSIONE PROPRIA e il suo output va in una finestra di log del selettore, non
# in un terminale: chiudere quella finestra non ferma piu' la simulazione (in un
# terminale la ammazzava - vedi lancia_net_startup), e da li' la si puo' anche
# fermare per davvero.
#
# La barra dei menu ha File -> Open loc path, che cambia a runtime la directory
# di lavoro del selettore: equivale a rilanciare lghmi da quella directory, e
# aggiorna modalita', liste, Set Sim path e stato del pulsante di lancio.
#
# In entrambe, selezionando una task la HMI viene lanciata in un processo
# INDIPENDENTE (detached):
#     cd <task> ; wish $LG_TIX/draw2gr.tcl 1 f22circ
# Il selettore resta aperto per altre scelte. "Quit" chiude SOLO il selettore:
# le HMI gia' aperte restano vive (le chiude l'utente), e cosi' la simulazione -
# con Quit se ne va anche la finestra di log, ma il log resta in /tmp.

package require Tk

# openhelp.tcl porta open_hlp (il manuale dei moduli, come nel ? di legopc) e
# browser_disponibile, che sceglie il browser partendo da $LG_BROWSER. Se non
# c'e' si perde solo il menu "?": il selettore funziona comunque.
catch {source [file join $env(LG_TIX) openhelp.tcl]}

# md2html.tcl converte i .md in HTML in Tcl puro, senza dipendere da pacchetti
# esterni: e' cosi' che la documentazione si vede formattata su OGNI
# installazione di LegoPST, non solo dove qualcuno ha installato pandoc.
catch {source [file join $env(LG_TIX) md2html.tcl]}

set TASKROOT [expr {[info exists env(LG_TASKROOT)] && $env(LG_TASKROOT) ne "" \
                    ? $env(LG_TASKROOT) : [file join $env(HOME) legocad]}]

#  Tre modalita':
#    -proc   solo le pagine di processo (task -> HMI draw2gr)
#    -staz   solo i faceplate (pagine di r02.dat -> xstaz)
#    nessuna delle due: entrambe, in due liste affiancate
set stazmode [expr {[lsearch -exact $argv "-staz"] >= 0}]
set procmode [expr {[lsearch -exact $argv "-proc"] >= 0}]

# -insim: il selettore e' stato lanciato DA DENTRO una simulazione in corso -
# lo fa il banco (new_monit, attiva_lghmi in cont_rec.c) dal suo menu, e il
# banco gira nella dir del simulatore. In quel caso il selettore appartiene a
# quella simulazione e due comandi non hanno senso, o sono dannosi:
#   * Open loc path  porterebbe il selettore su un'ALTRA directory, scollegandolo
#                    dalla simulazione che l'ha aperto;
#   * net_startup    farebbe killsim, cioe' ammazzerebbe proprio la simulazione
#                    da cui e' stato lanciato (e il banco con lei).
# Entrambi vengono quindi disabilitati.
set insim [expr {[lsearch -exact $argv "-insim"] >= 0}]

# Directory usate di recente, per riaprirle dal menu File senza passare dal
# dialogo di selezione. Stanno in un file nella home e non in
# $LG_ENTRY/legopc_prefs.tcl perche' la lista attraversa le installazioni: la
# radice utente cambia proprio quando si cambia directory.
set RECENTIFILE [file join $env(HOME) .lghmi_recent]
set MAXRECENTI  3
set RECENTI     {}
set mostra_proc [expr {$procmode || !$stazmode}]
set mostra_staz [expr {$stazmode || !$procmode}]
set doppia      [expr {$mostra_proc && $mostra_staz}]
set LGTIX    [expr {[info exists env(LG_TIX)] ? $env(LG_TIX) : ""}]

# --- Modalita' S01: file "S01" nella cwd di lancio ----------------------
# La cwd e' la dir da cui l'helper ha fatto exec di wish (nessun cd), quindi
# tipicamente la dir del simulatore in esecuzione.
set S01FILE [file join [pwd] S01]
set s01mode [expr {[file exists $S01FILE] && ![file isdirectory $S01FILE]}]
set s01_name ""
set s01_desc ""

# Set Sim path (helper -loc): LG_SIM_PATH = dir del simulatore in esecuzione,
# ereditata dal processo draw2gr. In modalita' S01 e' la dir del file S01 (la
# cwd di lancio): li' gira il simulatore COMPOSTO (dati live, SHM, f22circ.dat,
# variabili.rtf). Le task vengono invece lanciate dalla loro dir MODELLO (solo
# per caricare lo schema .tom): animazione/Plot/Command devono comunque puntare
# alla dir del simulatore, non a quella modello. Qui la mostriamo soltanto.
set SIMPATH  [expr {[info exists env(LG_SIM_PATH)] && $env(LG_SIM_PATH) ne "" ? $env(LG_SIM_PATH) : ""}]

# ITEMS = lista parallela alla listbox: {label dir name} per ogni voce.
set ITEMS {}

# --- Parsing S01 --------------------------------------------------------
# Il file S01 e' diviso in sezioni separate da righe che iniziano con '****'
# (quattro asterischi a partire dalla prima colonna):
#   sez.1  nome simulatore + descrizione (una riga)
#   sez.2  una riga per task: nome + descrizione
#   sez.3  una riga per task (associazione posizionale con la sez.2):
#          path relativo <tab/spazi> lettera tipo (P=processo, R=regolazione)
# Ritorna la lista {name desc dir} delle sole task di PROCESSO (P), con il
# path risolto in assoluto rispetto alla directory del file S01.
proc parse_s01 {path {tipi P}} {
    global s01_name s01_desc
    set s01dir [file dirname $path]
    if {[catch {open $path r} fh]} { return {} }
    set data [read $fh]
    close $fh

    # Suddividi in sezioni sui separatori '****' in colonna 1.
    set sec 0
    array set S {}
    foreach line [split $data "\n"] {
        if {[string range $line 0 3] eq "****"} { incr sec; continue }
        lappend S($sec) $line
    }

    # sez.1: prima riga non vuota = nome + descrizione simulatore.
    if {[info exists S(1)]} {
        foreach l $S(1) {
            if {[string trim $l] eq ""} continue
            if {[regexp {^(\S+)\s*(.*)$} $l -> nm ds]} {
                set s01_name $nm
                set s01_desc [string trim $ds]
            }
            break
        }
    }

    # sez.2: nome + descrizione di ogni task.
    set names {}
    if {[info exists S(2)]} {
        foreach l $S(2) {
            if {[string trim $l] eq ""} continue
            if {[regexp {^(\S+)\s*(.*)$} $l -> nm ds]} {
                lappend names [list $nm [string trim $ds]]
            }
        }
    }

    # sez.3: path relativo + tipo (P/R) di ogni task.
    set paths {}
    if {[info exists S(3)]} {
        foreach l $S(3) {
            if {[string trim $l] eq ""} continue
            if {[regexp {^(\S+)\s+(\S+)} $l -> rp tp]} {
                lappend paths [list $rp $tp]
            }
        }
    }

    # Associazione posizionale sez.2 <-> sez.3; tieni solo il tipo P.
    set out {}
    set n [expr {min([llength $names], [llength $paths])}]
    for {set i 0} {$i < $n} {incr i} {
        lassign [lindex $names $i] name desc
        lassign [lindex $paths $i] relpath tipo
        #  In modalita' faceplate servono anche le task di REGOLAZIONE (R):
        #  r01.dat/r02.dat vivono li', non nelle task di processo.
        set tenere 0
        foreach t $tipi { if {[string equal -nocase $tipo $t]} { set tenere 1 } }
        if {!$tenere} continue
        set dir [file normalize [file join $s01dir $relpath]]
        lappend out [list $name $desc $dir]
    }
    return $out
}

# --- Task = sottodir di $root con almeno un *.tom (modalita' dir-scan) ---
proc scan_tasks {root} {
    set out {}
    foreach d [lsort [glob -nocomplain -type d [file join $root *]]] {
        if {[llength [glob -nocomplain [file join $d *.tom]]] > 0} {
            lappend out [file tail $d]
        }
    }
    return $out
}

#  --- Modalita' -staz: pagine di faceplate --------------------------------

#  Directory in cui cercare r02.dat, in ordine di preferenza:
#   1. la cwd di lancio, se contiene gia' un r02.dat (caso tipico: lghmi -staz
#      lanciato dalla dir della regolazione);
#   2. in modalita' S01, tutte le task del simulatore, REGOLAZIONE COMPRESA;
#   3. altrimenti le sottodir di $TASKROOT.
proc dirs_con_r02 {} {
    global TASKROOT s01mode S01FILE
    set out {}
    if {[file exists [file join [pwd] r02.dat]]} { lappend out [pwd] }
    if {$s01mode} {
        foreach e [parse_s01 $S01FILE {P R}] {
            lassign $e name desc dir
            if {[file exists [file join $dir r02.dat]] && [lsearch -exact $out $dir] < 0} {
                lappend out $dir
            }
        }
    } else {
        foreach d [lsort [glob -nocomplain -type d [file join $TASKROOT *]]] {
            if {[file exists [file join $d r02.dat]] && [lsearch -exact $out $d] < 0} {
                lappend out $d
            }
        }
    }
    return $out
}

#  Pagine definite in <dir>/r02.dat. La lettura la fa 'stazpag -m', che conosce
#  il formato binario: qui non si reimplementa il layout delle strutture.
#  Ritorna una lista di {nome descrizione num_stazioni}.
proc pagine_di {dir} {
    set old [pwd]
    if {[catch {cd $dir}]} { return {} }
    set rc [catch {exec stazpag -m} out]
    cd $old
    if {$rc} { return {} }
    set res {}
    foreach riga [split $out "\n"] {
        if {[string trim $riga] eq ""} continue
        set campi [split $riga "|"]
        if {[llength $campi] < 3} continue
        lappend res [list [lindex $campi 0] [lindex $campi 1] [lindex $campi 2]]
    }
    return $res
}

#  xstaz gia' in esecuzione? Ritorna {pid cwd}, oppure {} se non c'e'.
proc xstaz_attivo {} {
    if {[catch {exec pgrep -x xstaz} out]} { return {} }
    set pid [lindex [split [string trim $out]] 0]
    if {$pid eq ""} { return {} }
    set cwd ""
    catch {set cwd [file readlink /proc/$pid/cwd]}
    return [list $pid $cwd]
}

#  Apre la pagina selezionata: avvia xstaz se serve, poi gli manda la richiesta.
proc apri_faceplate {} {
    global ITEMS_STAZ LB_STAZ
    set sel [$LB_STAZ curselection]
    if {[llength $sel] == 0} {
        .status configure -text "Seleziona una pagina di faceplate dalla lista."
        return
    }
    lassign [lindex $ITEMS_STAZ [lindex $sel 0]] label dir nome

    #  La coda delle richieste (SHR_USR_KEY + ID_MSG_STAZ) e' UNA per
    #  simulazione: due xstaz avviati su r02.dat diversi si ruberebbero i
    #  messaggi a vicenda, quindi non se ne lancia mai un secondo.
    set attivo [xstaz_attivo]
    if {[llength $attivo]} {
        lassign $attivo pid cwd
        if {$cwd ne "" && [file normalize $cwd] ne [file normalize $dir]} {
            tk_messageBox -icon warning -title "xstaz" -parent . -message \
                "xstaz e' gia' in esecuzione (pid $pid) nella directory:\n$cwd\n\nLa coda delle richieste e' unica per simulazione: chiudi quel xstaz prima di aprire pagine di:\n$dir"
            return
        }
    } else {
        set log [file join /tmp "lghmi_xstaz.log"]
        set old [pwd]
        if {[catch {cd $dir}]} {
            tk_messageBox -icon error -title "xstaz" -parent . \
                -message "Directory non accessibile:\n$dir"
            return
        }
        #  xstaz parte ICONIFICATO (una finestrella con il solo tasto Quit) e
        #  apre le pagine su richiesta. setsid lo stacca dal selettore.
        set errore ""
        if {[catch {exec setsid xstaz 1 > $log 2>@1 &} errore]} {
            catch {exec xstaz 1 > $log 2>@1 &} errore
        }
        cd $old
        if {![llength [xstaz_attivo]]} {
            after 700
        }
    }

    #  La richiesta resta in coda finche' xstaz non la scoda: nessuna corsa.
    set old [pwd]
    cd $dir
    set rc [catch {exec stazpag $nome} out]
    cd $old
    if {$rc} {
        tk_messageBox -icon error -title "Faceplate" -parent . -message \
            "Impossibile richiedere la pagina '$nome':\n$out"
        return
    }
    .status configure -text "Pagina '$nome' richiesta a xstaz  ($dir)"
}

#  Riempie la lista delle pagine di PROCESSO (task con .tom). Ritorna il testo
#  di stato da mostrare.
proc riempi_proc {} {
    global TASKROOT s01mode S01FILE ITEMS_PROC LB_PROC
    $LB_PROC delete 0 end
    set ITEMS_PROC {}

    if {$s01mode} {
        foreach e [parse_s01 $S01FILE] {
            lassign $e name desc dir
            set label [expr {$desc ne "" ? "$name  $desc" : $name}]
            $LB_PROC insert end $label
            lappend ITEMS_PROC [list $label $dir $name]
        }
        catch {.hdr.s01 configure -text "Simulatore: $::s01_name  $::s01_desc"}
        if {[llength $ITEMS_PROC] == 0} {
            return "Nessuna task di processo (P) nel file S01"
        }
        $LB_PROC selection clear 0 end
        $LB_PROC selection set 0
        return "[llength $ITEMS_PROC] task di processo (S01: $::s01_name)"
    }

    # fuori dalla modalita' S01 l'intestazione del simulatore non ha senso:
    # va svuotata, altrimenti resta quella della directory precedente
    catch {.hdr.s01 configure -text ""}

    if {![file isdirectory $TASKROOT]} {
        return "Directory task non trovata: $TASKROOT"
    }
    set tasks [scan_tasks $TASKROOT]
    foreach t $tasks {
        $LB_PROC insert end $t
        lappend ITEMS_PROC [list $t [file join $TASKROOT $t] $t]
    }
    if {[llength $tasks] == 0} {
        return "Nessuna task (*.tom) in $TASKROOT"
    }
    $LB_PROC selection clear 0 end
    $LB_PROC selection set 0
    return "[llength $tasks] task in $TASKROOT"
}

#  Riempie la lista dei FACEPLATE (pagine dei vari r02.dat).
proc riempi_staz {} {
    global ITEMS_STAZ LB_STAZ
    $LB_STAZ delete 0 end
    set ITEMS_STAZ {}

    set dirs [dirs_con_r02]
    if {[llength $dirs] == 0} {
        return "Nessun r02.dat trovato (compilare r01.dat con compstaz)"
    }
    set piu_dir [expr {[llength $dirs] > 1}]
    foreach d $dirs {
        foreach pg [pagine_di $d] {
            lassign $pg nome descr nstaz
            set label [format "%-10s %-42s %3s staz" $nome $descr $nstaz]
            if {$piu_dir} { append label "   \[[file tail $d]\]" }
            $LB_STAZ insert end $label
            lappend ITEMS_STAZ [list $label $d $nome]
        }
    }
    if {[llength $ITEMS_STAZ] == 0} {
        return "r02.dat presente ma senza pagine leggibili"
    }
    $LB_STAZ selection clear 0 end
    $LB_STAZ selection set 0
    return "[llength $ITEMS_STAZ] pagine di faceplate in [llength $dirs] directory"
}

proc refresh_list {} {
    global mostra_proc mostra_staz doppia
    set msg {}
    if {$mostra_proc} { lappend msg [riempi_proc] }
    if {$mostra_staz} { lappend msg [riempi_staz] }
    set nota [aggiorna_stato_mmi]
    if {$nota ne ""} { lappend msg $nota }
    set nota [aggiorna_stato_startup]
    if {$nota ne ""} { lappend msg $nota }
    aggiorna_etichette_loc
    # un simulatore creato dopo l'avvio compare nel sottomenu al primo Refresh
    catch {aggiorna_menu_tools}
    .status configure -text [join $msg "   |   "]
    if {$doppia} { aggiorna_intestazioni }
}

#  In modalita' doppia il conteggio va anche sulle intestazioni dei due riquadri.
proc aggiorna_intestazioni {} {
    global ITEMS_PROC ITEMS_STAZ
    catch {.pw.proc.h configure -text "Pagine di processo ([llength $ITEMS_PROC])"}
    catch {.pw.staz.h configure -text "Faceplate xstaz ([llength $ITEMS_STAZ])"}
}

# --- Lancio della HMI in un processo indipendente ------------------------
proc launch_hmi {} {
    global LGTIX ITEMS_PROC LB_PROC
    set sel [$LB_PROC curselection]
    if {[llength $sel] == 0} {
        .status configure -text "Seleziona una task di processo dalla lista."
        return
    }
    lassign [lindex $ITEMS_PROC [lindex $sel 0]] label dir name
    if {![file isdirectory $dir]} {
        tk_messageBox -icon error -title "Task" -parent . -message \
            "Directory della task non trovata:\n$dir"
        return
    }
    # Task dentro un bundle FMU? (<bundle>/task/<nome> -> <bundle>/run_draw2gr.sh)
    # Allora la HMI va lanciata col run_draw2gr.sh di QUEL bundle, non col
    # draw2gr.tcl dell'installazione LegoPST: solo lui conosce l'ambiente del
    # proprio bundle (wish, LG_TIX, runtime Tcl/Tk/Tix, LG_MODELS) e si ricava da
    # solo SHR_USR_KEY/LG_SIM_PATH dal net_sked di QUESTA task. Serve in
    # co-simulazione (lg_cosim), dove ogni FMU e' un simulatore a se' con la
    # propria chiave. Sulla macchina target del bundle, LegoPST non c'e' affatto.
    set launcher [file normalize [file join $dir .. .. run_draw2gr.sh]]
    set d2g [file join $LGTIX draw2gr.tcl]
    if {![file exists $launcher] && ($LGTIX eq "" || ![file exists $d2g])} {
        tk_messageBox -icon error -title "LG_TIX" -parent . -message \
            "draw2gr.tcl non trovato (LG_TIX='$LGTIX').\nAvvia lghmi da un ambiente LegoPST (profilo sorgiato)."
        return
    }
    # LG_SIM_PATH (Set Sim path) e' ereditato invariato dall'helper: la dir del
    # simulatore in esecuzione (in S01 = dir del file S01). NON va reimpostato
    # alla dir modello della task, altrimenti animazione/Plot/Command leggono
    # una dir senza dati live/SHM del simulatore attivo.
    # Processo INDIPENDENTE: cd nella task ed exec della HMI. `setsid` la mette
    # in una nuova sessione -> sopravvive al Quit del selettore. L'output va in
    # un log in /tmp (per non sporcare la task) utile per debug.
    set log [file join /tmp "lghmi_${name}.log"]
    if {[file exists $launcher]} {
        # Bundle: `env -u` toglie LG_SIM_PATH/SHR_USR_KEY ereditate dal selettore.
        # Sono quelle del simulatore da cui e' partito lghmi (per S01/-loc) e qui
        # sarebbero SBAGLIATE: in co-simulazione ogni task ha la sua sim e la sua
        # chiave. Tolte, run_draw2gr.sh le ricava dal net_sked di questa task.
        set sh "exec env -u LG_SIM_PATH -u SHR_USR_KEY bash [list $launcher] [list $dir] >[list $log] 2>&1"
    } else {
        set sh "cd [list $dir] && exec wish [list $d2g] 1 f22circ >[list $log] 2>&1"
    }
    if {[catch {exec setsid sh -c $sh &} err]} {
        # fallback senza setsid: resta comunque orfano (sopravvive) alla chiusura
        if {[catch {exec sh -c $sh &} err2]} {
            tk_messageBox -icon error -title "Lancio HMI" -parent . \
                -message "Impossibile lanciare la HMI per '$name':\n$err2"
            return
        }
    }
    .status configure -text "HMI avviata per '$name'  (log: $log)"
}

#  Quante istanze di mmi sono vive adesso. Serve per capire se il lancio e'
#  riuscito: mmi parte in background, quindi l'esito non torna da exec.
proc conta_mmi {} {
    if {[catch {exec pgrep -x mmi} out]} { return 0 }
    return [llength [split [string trim $out] "\n"]]
}

#  Lancia l'applicazione MMI (LegoMMI, Alg_mmi/run_time).
#  La directory di lavoro e' tutto: mmi legge ./Context.ctx e da li' ricava dove
#  stanno le pagine, senza cercare altrove (vedi Alg_mmi/README.md). Ordine:
#     1) $LG_SIM_PATH/globpages  il Set Sim path (-loc) vince: e' il simulatore
#                                che l'utente ha indicato a QUESTO selettore, e
#                                puo' non essere quello scelto con ksetsim
#     2) $KPAGES                 (di norma $KSIM/globpages, dal profilo)
#     3) ./globpages             (simulatore sotto la cwd, profilo non sorgiato)
#     4) nessuna                 (si lancia dalla cwd, sara' mmi a lamentarsi
#                                 del Context mancante)
proc dir_mmi {} {
    global env SIMPATH
    if {$SIMPATH ne "" && [file isdirectory [file join $SIMPATH globpages]]} {
        return [list [file join $SIMPATH globpages] "Set Sim path"]
    }
    if {[info exists env(KPAGES)] && $env(KPAGES) ne "" \
        && [file isdirectory $env(KPAGES)]} {
        return [list $env(KPAGES) "KPAGES"]
    }
    if {[file isdirectory globpages]} {
        return [list [file normalize globpages] "./globpages"]
    }
    return [list "" ""]
}

#  Quante pagine mmi potrebbe davvero aprire partendo da $dir, applicando le sue
#  stesse regole: il Context.ctx della dir di lancio dichiara in *pages DOVE
#  stanno le pagine compilate e in *page_list QUALI sono; mmi apre poi
#  <pages>/<NOME>.rtf (vedi Alg_mmi/README.md). Non basta contare i *.rtf della
#  directory: la dir di un simulatore ne contiene altri che pagine non sono
#  (variabili.rtf, recorder.rtf...), e il Context puo' puntare le pagine altrove.
#  Ritorna -1 se manca il Context (mmi uscirebbe subito), altrimenti il numero di
#  pagine elencate che hanno il .rtf al suo posto.
proc pagine_mmi {dir} {
    set ctx [file join $dir Context.ctx]
    if {![file exists $ctx]} { return -1 }
    if {[catch {open $ctx r} fp]} { return -1 }
    set testo [read $fp] ; close $fp
    set pagdir $dir
    set elenco {}
    foreach riga [split $testo "\n"] {
        if {[regexp {^\*pages:[ \t]*(.*)$} $riga -> v]} {
            set v [string trim $v]
            if {$v ne ""} {
                set pagdir [expr {[string index $v 0] eq "/" ? $v : [file join $dir $v]}]
            }
        } elseif {[regexp {^\*page_list:[ \t]*(.*)$} $riga -> v]} {
            # Il valore inizia con "\ " (continuazione delle risorse X): via il
            # backslash, poi i nomi separati da spazi.
            set elenco [regexp -all -inline {\S+} [string map {"\\" " "} $v]]
        }
    }
    set n 0
    foreach nome $elenco {
        if {[file exists [file join $pagdir $nome.rtf]]} { incr n }
    }
    return $n
}

#  Abilita/disabilita il pulsante mmi: senza pagine apribili non ha senso.
#  Ritorna il motivo (stringa vuota se tutto a posto) da mostrare nella riga di
#  stato insieme ai conteggi delle liste.
proc aggiorna_stato_mmi {} {
    lassign [dir_mmi] dir via
    set d [expr {$dir eq "" ? [pwd] : $dir}]
    set n [pagine_mmi $d]
    if {$n > 0} {
        .btn.mmi configure -state normal -background "#50a050"
        return ""
    }
    .btn.mmi configure -state disabled -background "#9ab89a"
    if {$n < 0} { return "mmi: nessun Context.ctx in $d" }
    return "mmi: nessuna pagina compilata (.rtf) in $d"
}

proc launch_mmi {} {
    lassign [dir_mmi] dir via
    # Senza profilo LegoPST sorgiato l'eseguibile non e' raggiungibile.
    if {[auto_execok mmi] eq ""} {
        tk_messageBox -icon error -title "mmi" -parent . -message \
            "Eseguibile 'mmi' non trovato nel PATH.\nAvvia lghmi da un ambiente LegoPST (profilo sorgiato)."
        .status configure -text "mmi non trovato nel PATH."
        return
    }
    set log [file join /tmp "lghmi_mmi.log"]
    if {$dir ne ""} {
        set sh "cd [list $dir] && exec mmi >[list $log] 2>&1"
    } else {
        set sh "exec mmi >[list $log] 2>&1"
    }
    set prima [conta_mmi]
    # Processo INDIPENDENTE come per la HMI: setsid lo mette in una nuova
    # sessione, cosi' sopravvive al Quit del selettore.
    if {[catch {exec setsid sh -c $sh &} err]} {
        if {[catch {exec sh -c $sh &} err2]} {
            tk_messageBox -icon error -title "Lancio mmi" -parent . \
                -message "Impossibile lanciare mmi:\n$err2"
            .status configure -text "mmi NON avviato."
            return
        }
    }
    .status configure -text [expr {$dir eq "" ? "mmi in avvio dalla cwd..." \
                                              : "mmi in avvio da $dir ($via)..."}]
    after 3000 [list verifica_mmi $prima $dir $log]
}

#  Controllo differito dell'esito: se non e' comparsa una nuova istanza, mmi e'
#  morto subito e il motivo sta nelle ultime righe del log.
proc verifica_mmi {prima dir log} {
    if {[conta_mmi] > $prima} {
        .status configure -text [expr {$dir eq "" ? "mmi avviato (log: $log)" \
                                                  : "mmi avviato da $dir (log: $log)"}]
        return
    }
    set coda ""
    if {[file exists $log]} {
        catch {
            set fp [open $log r] ; set testo [read $fp] ; close $fp
            set righe [split [string trimright $testo "\n"] "\n"]
            if {[llength $righe] > 12} { set righe [lrange $righe end-11 end] }
            set coda [join $righe "\n"]
        }
    }
    tk_messageBox -icon error -title "Lancio mmi" -parent . -message \
        "mmi non e' partito.\n\nDirectory: [expr {$dir eq "" ? "(cwd)" : $dir}]\nLog: $log\n\n$coda"
    .status configure -text "mmi NON avviato - vedi $log"
}

#  Popup minimo del tasto destro: una finestrella senza decorazioni con il solo
#  pulsante "Open page". Il tasto destro prima SELEZIONA la voce sotto il
#  cursore, cosi' il popup agisce su quella puntata e non sulla selezione
#  precedente; poi la apre con la stessa azione del doppio click.
proc chiudi_popup {} {
    if {[winfo exists .popup_open]} {
        catch {grab release .popup_open}
        destroy .popup_open
    }
}

#  Un click fuori dal pulsante chiude il popup. Il grab e' LOCALE: i figli di
#  .popup_open ricevono i loro eventi normalmente (il pulsante funziona), tutto
#  il resto arriva qui.
proc popup_fuori {X Y} {
    if {[winfo containing $X $Y] ne ".popup_open.b"} { chiudi_popup }
}

proc esegui_popup {azione} {
    chiudi_popup
    uplevel #0 $azione
}

proc popup_open_page {lb azione y X Y} {
    chiudi_popup
    if {[$lb size] == 0} { return }
    set i [$lb nearest $y]
    if {$i < 0} { return }
    $lb selection clear 0 end
    $lb selection set $i
    $lb activate $i
    toplevel .popup_open -bd 1 -relief solid
    wm overrideredirect .popup_open 1
    wm geometry .popup_open +[expr {$X + 2}]+[expr {$Y + 2}]
    button .popup_open.b -text "Open page" -padx 6 -pady 2 \
                         -command [list esegui_popup $azione]
    pack .popup_open.b
    bind .popup_open <Escape>      { chiudi_popup }
    bind .popup_open <ButtonPress> { popup_fuori %X %Y }
    update idletasks
    raise .popup_open
    focus .popup_open
    grab set .popup_open
}

# --- Directory corrente: "File -> Open loc path" -------------------------
#
# Cambiare directory a runtime equivale a rilanciare lghmi da quella dir: da
# essa dipendono la modalita' (l'S01 si cerca nella cwd), la lista dei
# faceplate (dirs_con_r02 guarda [pwd]), la dir di lavoro dell'MMI e il Set Sim
# path che le HMI ereditano.
#
# Nota: NON coincide con l'opzione "-loc DIR" dell'helper, che imposta soltanto
# LG_SIM_PATH e lascia la cwd dov'era. Qui si fa entrambe le cose, cioe' quello
# che si otterrebbe lanciando lghmi da quella directory.

#  Mostra o nasconde le due intestazioni che dipendono dalla directory: il
#  simulatore S01 e il Set Sim path. Il testo dell'S01 lo scrive riempi_proc,
#  che ha i valori del parsing; qui si decide solo cosa si vede.
proc aggiorna_etichette_loc {} {
    global SIMPATH
    if {[.hdr.s01 cget -text] ne ""} {
        pack .hdr.s01 -side top -fill x
    } else {
        pack forget .hdr.s01
    }
    if {$SIMPATH ne ""} {
        .hdr.loc configure -text "Set Sim path: $SIMPATH"
        pack .hdr.loc -side top -fill x
    } else {
        .hdr.loc configure -text ""
        pack forget .hdr.loc
    }
}

#  Porta il selettore nella directory <dir>: cwd, Set Sim path (anche
#  nell'ambiente, perche' le HMI lanciate lo ereditano), rilevamento dell'S01 e
#  aggiornamento delle liste. Ritorna un messaggio d'errore, o "" se e' andata.
proc imposta_loc {dir} {
    global S01FILE s01mode SIMPATH s01_name s01_desc
    if {![file isdirectory $dir]} { return "Directory non trovata: $dir" }
    if {[catch {cd $dir} err]}    { return "Impossibile entrare in $dir: $err" }

    set SIMPATH [pwd]
    set ::env(LG_SIM_PATH) $SIMPATH
    set S01FILE [file join $SIMPATH S01]
    set s01mode [expr {[file exists $S01FILE] && ![file isdirectory $S01FILE]}]
    if {!$s01mode} { set s01_name ""; set s01_desc "" }
    refresh_list
    return ""
}

#  Legge le directory recenti, scartando quelle che non esistono piu' (una sim
#  cancellata, un disco smontato): restano nel file ma non nel menu.
proc carica_recenti {} {
    global RECENTI RECENTIFILE MAXRECENTI
    set RECENTI {}
    if {[catch {open $RECENTIFILE r} fd]} return
    while {[gets $fd riga] >= 0} {
        set riga [string trim $riga]
        if {$riga eq "" || ![file isdirectory $riga]} continue
        if {[lsearch -exact $RECENTI $riga] < 0} { lappend RECENTI $riga }
        if {[llength $RECENTI] >= $MAXRECENTI} break
    }
    close $fd
}

proc salva_recenti {} {
    global RECENTI RECENTIFILE
    # Se la home non e' scrivibile si perde solo la memoria dei recenti: non e'
    # un motivo per fermare il selettore.
    catch {
        set fd [open $RECENTIFILE w]
        foreach d $RECENTI { puts $fd $d }
        close $fd
    }
}

#  Mette <dir> in testa ai recenti (senza doppioni), tronca e salva.
proc ricorda_recente {dir} {
    global RECENTI MAXRECENTI
    set dir [file normalize $dir]
    set pos [lsearch -exact $RECENTI $dir]
    if {$pos >= 0} { set RECENTI [lreplace $RECENTI $pos $pos] }
    set RECENTI [linsert $RECENTI 0 $dir]
    if {[llength $RECENTI] > $MAXRECENTI} {
        set RECENTI [lrange $RECENTI 0 [expr {$MAXRECENTI-1}]]
    }
    salva_recenti
    aggiorna_menu_file
}

#  Ricostruisce il menu File PER INTERO a ogni cambiamento dei recenti.
#  Non si toccano le singole voci: gli indici cambierebbero a ogni path in piu'
#  o in meno, ed e' proprio il tipo di indirizzamento da evitare in un menu Tk.
proc aggiorna_menu_file {} {
    global RECENTI insim env
    if {![winfo exists .mb.file]} return
    set stato [expr {$insim ? "disabled" : "normal"}]

    .mb.file delete 0 end
    .mb.file add command -label "Open loc path..." -command apri_loc_path -state $stato
    if {[llength $RECENTI] > 0} {
        .mb.file add separator
        foreach d $RECENTI {
            # ~ al posto della home: i path delle simulazioni sono lunghi e la
            # parte utile e' la coda
            set etichetta $d
            if {[string first $env(HOME)/ $d] == 0} {
                set etichetta "~[string range $d [string length $env(HOME)] end]"
            }
            .mb.file add command -label $etichetta -state $stato \
                                 -command [list vai_a_loc $d]
        }
    }
    .mb.file add separator
    .mb.file add command -label "Refresh" -command refresh_list
    .mb.file add separator
    .mb.file add command -label "Quit" -command exit
}

#  Va nella directory <dir> e la promuove in testa ai recenti. E' la strada sia
#  del dialogo di selezione sia delle voci di menu dei path recenti.
proc vai_a_loc {dir} {
    global insim
    if {$insim} {
        .status configure -text \
            "Directory fissa: il selettore e' stato lanciato dal banco della simulazione in corso."
        return
    }
    set err [imposta_loc $dir]
    if {$err ne ""} {
        tk_messageBox -icon error -title "Open loc path" -parent . -message $err
        return
    }
    ricorda_recente [pwd]
    # refresh_list ha gia' scritto i conteggi: la directory si aggiunge davanti,
    # non li sostituisce
    .status configure -text "Directory: [pwd]   |   [.status cget -text]"
}

#  Voce di menu: scegli la directory e vacci.
proc apri_loc_path {} {
    global insim
    if {$insim} {
        # la voce di menu e' disabilitata, ma la proc resta raggiungibile
        .status configure -text \
            "Directory fissa: il selettore e' stato lanciato dal banco della simulazione in corso."
        return
    }
    set dir [tk_chooseDirectory -parent . -mustexist 1 -initialdir [pwd] \
                 -title "Directory della simulazione (loc path)"]
    if {$dir eq ""} return
    vai_a_loc $dir
}

# --- Lancio della simulazione (net_startup) ------------------------------

#  Processi di simulazione vivi adesso. Ritorna la lista dei nomi trovati.
proc sim_attiva {} {
    set vivi {}
    foreach p {dispatcher net_sked banco} {
        if {![catch {exec pgrep -x $p}]} { lappend vivi $p }
    }
    return $vivi
}

#  Il pulsante di lancio si abilita solo dove net_startup puo' funzionare.
#  Il file che lo script controlla e' variabili.rtf: senza quello si fermano
#  tutti i suoi passi successivi. Sta anche nelle dir delle task singole, che
#  sono lanciabili come i simulatori composti.
proc aggiorna_stato_startup {} {
    global insim
    if {$insim} {
        # lanciato dal banco: la simulazione gira gia', e net_startup la
        # fermerebbe con killsim
        .btn.start configure -state disabled
        return "lanciato dal banco: directory fissa, simulazione gia' in corso"
    }
    if {[file exists [file join [pwd] variabili.rtf]]} {
        .btn.start configure -state normal
        return "simulazione lanciabile da qui"
    }
    .btn.start configure -state disabled
    return ""
}

#  Comando per eseguire uno script in un terminale X. L'opzione con cui si
#  passa il comando cambia col terminale (gnome-terminal vuole "--", gli altri
#  "-e"), e la finestra la si tiene aperta con una pausa finale invece di
#  -hold, che e' solo di xterm: l'output dei controlli va letto.
proc comando_in_terminale {comando {shell sh}} {
    set xt [expr {[info exists ::env(LG_XTERM)] && $::env(LG_XTERM) ne "" \
                  ? $::env(LG_XTERM) : "xterm"}]
    set opz [expr {[string match "gnome-terminal*" [file tail $xt]] ? "--" : "-e"}]
    set sh "$comando; echo; echo '--- finito: premi Invio per chiudere ---'; read _"
    return [list $xt $opz $shell -c $sh]
}

#  Dove finisce l'output di net_startup: un file, non un terminale. La finestra
#  di log lo segue da li' (mostra_log_sim) e resta leggibile anche dopo che la
#  finestra e' stata chiusa. In /tmp, come gli altri lanci di questo file, per
#  non sporcare la directory della simulazione.
set SIMLOG [file join /tmp "lghmi_net_startup.log"]

#  Lancia net_startup nella directory corrente e ne mostra l'output.
#
#  La simulazione parte in una SESSIONE PROPRIA (setsid), NON dentro la finestra
#  che la mostra, e non e' un dettaglio: net_startup lancia dispatcher, net_sked
#  e banco con `&` da una ksh non interattiva, e senza job control restano tutti
#  nel process group di chi li ha lanciati. Dentro un terminale quel process
#  group prende un SIGHUP ogni volta che il terminale se ne va - chiudendo la
#  finestra con la X, ma anche premendo Invio al prompt finale, perche' uscendo
#  il session leader il kernel manda SIGHUP al foreground process group - e
#  nessuno dei tre binari ignora SIGHUP: morivano tutti e tre, e con loro le HMI
#  aperte. Con setsid la finestra e' soltanto un visore del log: chiuderla non
#  tocca la simulazione, e la conferma della X lo dice.
#
#  Chiede comunque conferma PRIMA di partire, perche' la prima cosa che
#  net_startup fa e' `killsim`, che su Linux cancella TUTTE le SHM, le code e i
#  semafori dell'utente senza filtrare per chiave: se c'e' una simulazione in
#  corso la ferma, e con essa le HMI che le stanno sopra. Se dei processi di
#  simulazione sono vivi lo si dice esplicitamente nel testo della conferma.
proc lancia_net_startup {} {
    global SIMLOG
    set dir [pwd]
    if {![file exists [file join $dir variabili.rtf]]} {
        tk_messageBox -icon error -title "Start simulation" -parent . -message \
            "In questa directory non c'e' variabili.rtf: net_startup non puo' partire.\n\n$dir"
        return
    }

    set vivi [sim_attiva]
    set avviso "Lanciare la simulazione in\n$dir\n\n"
    append avviso "net_startup esegue killsim, che cancella tutte le SHM, le code\n"
    append avviso "e i semafori di questo utente (nessun filtro per chiave)."
    if {[llength $vivi] > 0} {
        append avviso "\n\nATTENZIONE: c'e' gia' una simulazione in esecuzione\n"
        append avviso "([join $vivi ", "]): verra' fermata, e con essa le HMI aperte."
    }
    append avviso "\n\nProcedere?"
    if {[tk_messageBox -icon warning -type yesno -default no -parent . \
             -title "Start simulation" -message $avviso] ne "yes"} {
        .status configure -text "Lancio della simulazione annullato."
        return
    }

    # Il log e' quello della simulazione corrente: riparte da zero a ogni lancio.
    # Aprirlo qui non e' solo per troncarlo: e' anche la prova che si puo'
    # scrivere. Il nome in /tmp e' fisso, quindi il file puo' essere di un altro
    # utente della stessa macchina; senza questo controllo la redirezione di
    # net_startup fallirebbe e la finestra di log resterebbe vuota, senza dire
    # perche'.
    if {[catch {open $SIMLOG w} fd]} {
        tk_messageBox -icon error -title "Start simulation" -parent . -message \
            "Non riesco a scrivere il log della simulazione:\n$SIMLOG\n\n$fd"
        return
    }
    close $fd

    # Processo indipendente, come per le HMI: la simulazione sopravvive al Quit
    # del selettore. La dir si passa a net_startup, che fa il cd da se'.
    set sh "exec net_startup [list $dir] > [list $SIMLOG] 2>&1"
    if {[catch {exec setsid sh -c $sh &} err]} {
        # Senza setsid la simulazione resta nella sessione del selettore:
        # sopravvive comunque alla finestra di log, ma non alla fine della
        # sessione da cui lghmi e' partito.
        if {[catch {exec sh -c $sh &} err2]} {
            tk_messageBox -icon error -title "Start simulation" -parent . -message \
                "Impossibile lanciare net_startup:\n$err2"
            return
        }
    }
    .status configure -text "net_startup avviato in $dir  (log: $SIMLOG)"
    mostra_log_sim $dir
}


# --- Finestra di log della simulazione -----------------------------------
#
# Prende il posto del terminale, e la differenza che conta non e' estetica: la
# finestra e' di lghmi, quindi la X passa da WM_DELETE_WINDOW e prima di
# chiudere si puo' dire all'utente cosa succede - e cosa non succede - alla
# simulazione. Un xterm non lo consente: non ha nessun hook sulla richiesta di
# chiusura del window manager.
#
# Il testo si aggiorna leggendo la CODA del file di log: si tiene la posizione
# gia' mostrata e a ogni giro si legge soltanto quello che e' arrivato dopo.

set SIMLOG_POS 0   ;# byte del log gia' mostrati
set SIMLOG_GEN 0   ;# generazione della finestra: i cicli `after` di una
                   ;# finestra chiusa non devono lavorare per la successiva

#  Aggiunge testo al visore. Autoscroll SOLO se si sta guardando il fondo: chi
#  e' risalito a rileggere un errore non se lo vede scappare via.
proc log_sim_scrivi {testo {tag ""}} {
    set t .simlog.f.t
    if {![winfo exists $t] || $testo eq ""} return
    set infondo [expr {[lindex [$t yview] 1] >= 0.999}]
    $t configure -state normal
    if {$tag eq ""} { $t insert end $testo } else { $t insert end $testo $tag }
    $t configure -state disabled
    if {$infondo} { $t see end }
}

#  Un giro di lettura del log. Si richiama da solo finche' la finestra esiste
#  ed e' quella per cui il ciclo era partito.
proc segui_log_sim {gen} {
    global SIMLOG SIMLOG_POS SIMLOG_GEN
    if {![winfo exists .simlog] || $gen != $SIMLOG_GEN} return
    if {[file exists $SIMLOG]} {
        set dim [file size $SIMLOG]
        if {$dim < $SIMLOG_POS} { set SIMLOG_POS 0 }   ;# log rifatto da capo
        if {$dim > $SIMLOG_POS} {
            if {![catch {open $SIMLOG r} fd]} {
                seek $fd $SIMLOG_POS
                set nuovo [read $fd]
                set SIMLOG_POS [tell $fd]
                close $fd
                log_sim_scrivi $nuovo
            }
        }
    }
    after 500 [list segui_log_sim $gen]
}

#  Stato dei processi di simulazione. Ciclo separato e piu' lento di quello del
#  log: sim_attiva costa tre `pgrep`, e lo stato cambia raramente.
proc stato_sim_loop {gen} {
    global SIMLOG_GEN
    if {![winfo exists .simlog] || $gen != $SIMLOG_GEN} return
    aggiorna_stato_sim
    after 3000 [list stato_sim_loop $gen]
}

proc aggiorna_stato_sim {} {
    if {![winfo exists .simlog]} return
    set vivi [sim_attiva]
    if {[llength $vivi] > 0} {
        .simlog.b.stato configure -foreground "#006400" \
            -text "Simulazione in corso: [join $vivi ", "]"
        .simlog.b.stop configure -state normal
    } else {
        .simlog.b.stato configure -foreground "#707070" \
            -text "Nessun processo di simulazione attivo"
        .simlog.b.stop configure -state disabled
    }
}

#  Chiusura del visore: e' qui che finisce la X della finestra.
#
#  Con la simulazione in sessione propria chiudere non la ferma piu', ma la
#  domanda resta - la finestra e' l'unica cosa che dice che una simulazione sta
#  girando, e chi la chiude deve sapere che cosa lascia acceso e come spegnerlo.
proc chiudi_log_sim {} {
    global SIMLOG
    set vivi [sim_attiva]
    if {[llength $vivi] > 0} {
        set msg "Chiudere questa finestra NON ferma la simulazione.\n\n"
        append msg "Restano in esecuzione: [join $vivi ", "].\n"
        append msg "Girano in una sessione propria, e con loro restano vive le HMI\n"
        append msg "e i faceplate gia' aperti.\n\n"
        append msg "Per fermarla davvero: il pulsante \"Ferma la simulazione\" di\n"
        append msg "questa finestra, il comando killsim, o Quit dal banco.\n\n"
        append msg "Il log resta comunque leggibile in\n$SIMLOG\n\n"
        append msg "Chiudo la finestra di log?"
        if {[tk_messageBox -icon question -type yesno -default yes -parent .simlog \
                 -title "Log della simulazione" -message $msg] ne "yes"} return
    }
    destroy .simlog
}

#  Ferma davvero la simulazione. Lo fa killsim, cioe' lo stesso comando con cui
#  net_startup comincia: e' il modo previsto di ripulire l'ambiente (vedi
#  CLAUDE.md / docs), non un kill a mano dei processi.
proc ferma_simulazione {} {
    set vivi [sim_attiva]
    if {[llength $vivi] == 0} {
        aggiorna_stato_sim
        return
    }
    if {[auto_execok killsim] eq ""} {
        tk_messageBox -icon error -title "Ferma la simulazione" -parent .simlog \
            -message "Eseguibile 'killsim' non trovato nel PATH.\nAvvia lghmi da un ambiente LegoPST (profilo sorgiato)."
        return
    }
    set msg "Fermare la simulazione in corso?\n\n"
    append msg "Verranno terminati i processi ([join $vivi ", "]), e con loro\n"
    append msg "le HMI e i faceplate che ci stanno sopra.\n\n"
    append msg "Lo fa killsim, che su Linux cancella TUTTE le SHM, le code e i\n"
    append msg "semafori di questo utente, senza filtrare per chiave.\n\n"
    append msg "Procedere?"
    if {[tk_messageBox -icon warning -type yesno -default no -parent .simlog \
             -title "Ferma la simulazione" -message $msg] ne "yes"} return

    log_sim_scrivi "\n--- killsim ---\n" lghmi
    .simlog configure -cursor watch
    update idletasks
    set rc [catch {exec killsim} out]
    catch {.simlog configure -cursor ""}
    log_sim_scrivi "[string trim $out]\n" [expr {$rc ? "errore" : ""}]
    set vivi [sim_attiva]
    if {[llength $vivi] > 0} {
        log_sim_scrivi "Ancora attivi: [join $vivi ", "]\n" errore
    } else {
        log_sim_scrivi "Simulazione fermata.\n" lghmi
    }
    aggiorna_stato_sim
    catch {.status configure -text "Simulazione fermata con killsim."}
}

#  Apre (o riusa) il visore del log e fa ripartire i due cicli di
#  aggiornamento. Il visore e' UNO: un secondo net_startup riparte da capo
#  nella stessa finestra, come il log.
proc mostra_log_sim {dir} {
    global SIMLOG SIMLOG_POS SIMLOG_GEN
    set w .simlog
    if {![winfo exists $w]} {
        toplevel $w
        #  Un filo piu' alta della dimensione naturale del contenuto (~437 px):
        #  all'apertura si vede tutto senza che nulla parta compresso.
        wm geometry $w 720x460
        wm minsize  $w 480 240
        #  La X della finestra: il motivo per cui il log sta qui e non in un
        #  terminale.
        wm protocol $w WM_DELETE_WINDOW chiudi_log_sim

        #  Le barre in basso si impacchettano PRIMA del visore, anche se stanno
        #  sotto: pack assegna lo spazio nell'ordine di impacchettamento, e chi
        #  arriva dopo si prende quel che resta. Mettendo per primo il testo, che
        #  ha -expand 1, pulsanti e scritte finivano fuori dalla finestra.
        #  Stessa scelta della finestra principale, dove .btn e .status sono
        #  impacchettati prima delle liste.
        frame  $w.b
        label  $w.b.stato -anchor w -text ""
        button $w.b.chiudi -text "Chiudi" -width 10 -command chiudi_log_sim
        button $w.b.stop -text "Ferma la simulazione" -state disabled \
               -command ferma_simulazione
        pack $w.b.chiudi -side right -padx 4 -pady 6
        pack $w.b.stop   -side right -padx 4 -pady 6
        pack $w.b.stato  -side left  -padx 6
        pack $w.b -side bottom -fill x

        label $w.log -anchor w -padx 6 -foreground "#505050" -text "Log: $SIMLOG"
        pack  $w.log -side bottom -fill x

        #  -width/-height del testo tengono la dimensione NATURALE della finestra
        #  sotto quella imposta da wm geometry: cosi' quello che si vede
        #  all'apertura e' tutto, non solo il visore.
        frame $w.f
        text  $w.f.t -wrap none -font TkFixedFont -state disabled -bd 1 \
              -width 80 -height 18 -relief sunken -background white \
              -yscrollcommand "$w.f.sy set" -xscrollcommand "$w.f.sx set"
        scrollbar $w.f.sy -orient vertical   -command "$w.f.t yview"
        scrollbar $w.f.sx -orient horizontal -command "$w.f.t xview"
        grid $w.f.t  $w.f.sy -sticky nsew
        grid $w.f.sx -sticky ew
        grid rowconfigure    $w.f 0 -weight 1
        grid columnconfigure $w.f 0 -weight 1
        pack $w.f -side top -fill both -expand 1 -padx 4 -pady 4
        #  Le righe scritte da lghmi si distinguono da quelle di net_startup.
        $w.f.t tag configure lghmi  -foreground "#000080"
        $w.f.t tag configure errore -foreground "#a00000"

        bind $w <Escape> chiudi_log_sim
    }
    wm title $w "net_startup - $dir"

    #  Nuovo lancio: log da capo e cicli `after` di una nuova generazione (i
    #  vecchi si spengono da soli al primo giro).
    incr SIMLOG_GEN
    set SIMLOG_POS 0
    $w.f.t configure -state normal
    $w.f.t delete 1.0 end
    $w.f.t configure -state disabled
    log_sim_scrivi "Simulazione avviata in $dir\n" lghmi
    log_sim_scrivi "Gira in una sessione propria: chiudere questa finestra NON la ferma.\n\n" lghmi

    wm deiconify $w
    raise $w
    segui_log_sim $SIMLOG_GEN
    stato_sim_loop $SIMLOG_GEN
}


# --- Tools: configurazione del simulatore (kUpSim) -----------------------
#
# kUpSim riallinea tutta la configurazione del simulatore CORRENTE, quello
# puntato da $KSIM: kConnex, kNetCompi, kCompStaz, kStazPages, kWinContext,
# kCompileSim, kCollect. L'ordine non e' arbitrario (i faceplate si risolvono
# contro gli indici di variabili.rtf, quindi vanno dopo le task).
#
# Nota: "lgupsim" e' un alias di kUpSim in Alg_env.sh, e gli alias non esistono
# nelle shell non interattive: qui si chiama kUpSim.

#  I simulatori disponibili: le sottodirectory di $KSKED, come la funzione
#  ksims del profilo.
proc lista_simulatori {} {
    global env
    if {![info exists env(KSKED)] || $env(KSKED) eq ""} { return {} }
    set out {}
    foreach d [lsort [glob -nocomplain -type d [file join $env(KSKED) *]]] {
        lappend out [file tail $d]
    }
    return $out
}

#  Nome del simulatore corrente, o "" se non ce n'e' uno valido.
proc simulatore_corrente {} {
    global env
    if {![info exists env(KSIM)] || ![file isdirectory $env(KSIM)]} { return "" }
    if {[info exists env(KSIMNAME)] && $env(KSIMNAME) ne ""} { return $env(KSIMNAME) }
    return [file tail $env(KSIM)]
}

#  Cambia il simulatore corrente.
#
#  Una GUI non puo' cambiare l'ambiente della shell che l'ha lanciata, ma il
#  profilo LegoPST ha gia' il posto giusto dove registrare la scelta:
#  ksetsim_default legge ~/.legosim a ogni avvio di shell (poi cassano0, poi il
#  primo di ksims). Scrivendo il nome la', la scelta vale per lghmi, per i
#  comandi che lancia e per le shell future.
#
#  KSIM da sola non basterebbe: ksetsim ne deriva una ventina di variabili
#  (KWIN, KPAGES, KSTATUS, KCASSAFORTE, KGRAF...) e sorgia $KSIM/ksim.conf.
#  Quella logica NON si riscrive qui: i comandi si lanciano in una shell che
#  sorgia il profilo e chiama ksetsim, cosi' a derivare e' il codice che esiste
#  gia'. Le due variabili aggiornate qui sotto servono solo a quello che si
#  mostra nei menu e nei dialoghi.
proc scegli_simulatore {nome} {
    global env
    set dir [file join $env(KSKED) $nome]
    if {![file isdirectory $dir]} {
        tk_messageBox -icon error -title "Simulatore" -parent . -message \
            "Directory del simulatore non trovata:\n$dir"
        return
    }
    set scritto 1
    if {[catch {
        set fd [open [file join $env(HOME) .legosim] w]
        puts $fd $nome
        close $fd
    }]} { set scritto 0 }

    set env(KSIM)     $dir
    set env(KSIMNAME) $nome
    set ::KSIMSCELTO  $nome
    aggiorna_menu_tools
    if {$scritto} {
        .status configure -text \
            "Simulatore corrente: $nome   |   scritto in ~/.legosim: vale anche per le shell future"
    } else {
        .status configure -text \
            "Simulatore corrente: $nome   |   ~/.legosim non scrivibile: vale solo per questa sessione"
    }
}

#  Ricostruisce il menu Tools per intero, come per il menu File: le etichette
#  portano il nome del simulatore corrente, che cambia.
proc aggiorna_menu_tools {} {
    global env
    if {![winfo exists .mb.tools]} return

    set nome  [simulatore_corrente]
    set stato [expr {$nome ne "" ? "normal" : "disabled"}]
    set quale [expr {$nome ne "" ? $nome : "nessun simulatore"}]

    .mb.tools delete 0 end
    .mb.tools add command -state $stato -command [list lancia_kupsim {}] \
        -label "kUpSim - riallinea la configurazione di $quale"
    .mb.tools add command -state $stato -command [list lancia_kupsim -nommi] \
        -label "kUpSim -nommi - senza le pagine MMI dei faceplate"
    .mb.tools add command -state $stato -command [list lancia_kupsim -n] \
        -label "kUpSim -n - anteprima: mostra i passi senza eseguirli"
    .mb.tools add separator

    if {![winfo exists .mb.tools.sim]} { menu .mb.tools.sim -tearoff 0 }
    .mb.tools.sim delete 0 end
    set sims [lista_simulatori]
    if {[llength $sims] == 0} {
        .mb.tools.sim add command -state disabled \
            -label "(nessun simulatore in \$KSKED)"
    } else {
        foreach sim $sims {
            .mb.tools.sim add radiobutton -label $sim -value $sim \
                -variable ::KSIMSCELTO -command [list scegli_simulatore $sim]
        }
    }
    .mb.tools add cascade -label "Simulatore corrente" -menu .mb.tools.sim
}

#  Lancia kUpSim sul simulatore corrente, in un terminale.
#
#  La shell del terminale sorgia il profilo e chiama ksetsim: e' la sola strada
#  per avere KSIM E tutte le sue derivate coerenti, ed e' anche il motivo per
#  cui serve bash (il profilo e' pensato per quella) e non la sh usata per
#  net_startup.
#
#  Chiede conferma dicendo cosa succede e su quale simulatore, tranne per
#  l'anteprima -n, che non esegue niente. Se una simulazione e' in corso lo
#  segnala: kUpSim riscrive variabili.rtf, r02.dat e le pagine, che quella
#  simulazione sta usando.
proc lancia_kupsim {opzioni} {
    global env
    set nome [simulatore_corrente]
    if {$nome eq ""} {
        tk_messageBox -icon error -title "kUpSim" -parent . -message \
            "Nessun simulatore corrente: KSIM non e' definita o non e' una directory.\nSceglilo da Tools -> Simulatore corrente, o con 'ksetsim <nome>'."
        return
    }
    set anteprima [expr {[lsearch -exact $opzioni "-n"] >= 0}]

    if {!$anteprima} {
        set msg "Aggiornare la configurazione del simulatore\n\n"
        append msg "    $nome\n    $env(KSIM)\n\n"
        append msg "kUpSim rifa' in sequenza:\n"
        append msg "    kConnex       topologia fra le task      -> S01\n"
        append msg "    kNetCompi     compilazione delle task    -> variabili.rtf\n"
        append msg "    kCompStaz     faceplate per xstaz        -> r02.dat\n"
        if {[lsearch -exact $opzioni "-nommi"] < 0} {
            append msg "    kStazPages    faceplate come pagine MMI\n"
            append msg "    kWinContext   Context.ctx di \$KWIN\n"
            append msg "    kCompileSim   compila le pagine          -> .rtf\n"
        } else {
            append msg "    (i tre passi delle pagine MMI vengono saltati)\n"
        }
        append msg "    kCollect      raccolta in globpages + kMmiConfig\n"
        set vivi [sim_attiva]
        if {[llength $vivi] > 0} {
            append msg "\nATTENZIONE: c'e' una simulazione in esecuzione ([join $vivi ", "]).\n"
            append msg "Sta usando variabili.rtf, r02.dat e le pagine, e le troverebbe\n"
            append msg "cambiate sotto: conviene fermarla prima."
        }
        append msg "\n\nProcedere?"
        if {[tk_messageBox -icon warning -type yesno -default no -parent . \
                 -title "kUpSim" -message $msg] ne "yes"} {
            .status configure -text "kUpSim annullato."
            return
        }
    }

    set radice [expr {[info exists env(LEGOROOT)] ? $env(LEGOROOT) : ""}]
    if {$radice eq "" || ![file exists [file join $radice .profile_legoroot]]} {
        tk_messageBox -icon error -title "kUpSim" -parent . -message \
            "LEGOROOT non definita o profilo non trovato: non posso preparare l'ambiente di kUpSim."
        return
    }
    set prof [file join $radice .profile_legoroot]
    set kup [string trim "kUpSim $opzioni"]
    set riga ". [list $prof] [list $radice] >/dev/null 2>&1; ksetsim [list $nome] >/dev/null; $kup"
    set cmd [comando_in_terminale $riga bash]
    if {[catch {exec setsid {*}$cmd &} err]} {
        if {[catch {exec {*}$cmd &} err2]} {
            tk_messageBox -icon error -title "kUpSim" -parent . -message \
                "Impossibile lanciare kUpSim:\n$err2"
            return
        }
    }
    if {$anteprima} {
        .status configure -text "kUpSim -n: anteprima dei passi su $nome, nel terminale."
    } else {
        .status configure -text "kUpSim avviato su $nome - i passi e gli errori sono nel terminale."
    }
}

# --- Menu "?" : documentazione e versione --------------------------------
#
# La documentazione di LegoPST e' molta - una trentina di .md, due HTML e il
# manuale storico dei moduli in 218 pagine .htm - e il menu non deve diventarne
# il catalogo. Qui ci sono l'INDICE RAGIONATO, che e' l'hub di tutto il resto,
# il riferimento dei comandi kbin, il manuale dei moduli, e i tre documenti che
# rispondono alle domande di chi sta usando PROPRIO questa finestra: lghmi, la
# configurazione del simulatore che il menu Tools riallinea, e i faceplate che
# la lista di destra apre.
#
# I .md si aprono nel browser, dove si vedono come testo: e' quello che succede
# comunque ai 36 rimandi ai .md dentro l'indice ragionato, quindi il
# meccanismo resta uno solo.

#  {etichetta  path relativo a $LEGOROOT ?rilievo?}, con "--" per un separatore
#  e "MODULI" per il manuale storico dei moduli, che non e' un file del
#  repository ma una collezione sotto $LG_HTML e si apre con open_hlp. Il terzo
#  campo, se c'e', mette la voce in grassetto. Cosi' l'ordine e il risalto delle
#  voci stanno tutti qui.
#
#  Il README e' il primo, e in grassetto: e' il documento che dice CHE COS'E'
#  LegoPST - quello che si legge su GitHub - e viene prima di sapere dove sta
#  tutto il resto.
proc documenti_aiuto {} {
    return {
        {"LegoPST - panoramica del progetto (README)"  README.md  rilievo}
        --
        {"Indice ragionato della documentazione"   INDICE_DOCUMENTAZIONE.html}
        {"Comandi kbin (i 192 kprocedure)"         kbin/kbin-riferimento-comandi-LegoPST.html}
        MODULI
        --
        {"Questa finestra: lghmi"                  Alg_legopc/LGHMI.md}
        {"Configurare un simulatore: al_sim.conf"  docs/AL_SIM_CONF.md}
        {"Faceplate di comando (xstaz)"            Alg_rt/grafica/xstaz/HOWTO_faceplate.md}
    }
}

#  Converte un .md in HTML dentro una directory temporanea e ne ritorna il
#  path, oppure "" se non c'e' un convertitore.
#
#  Serve perche' i browser NON sanno rendere il Markdown: il sistema classifica
#  i .md come text/plain, quindi Firefox mostra il sorgente. Con un
#  convertitore, tabelle, blocchi di codice e citazioni si vedono per quello che
#  sono.
#
#  Il convertitore e' quello di LegoPST (md2html.tcl, Tcl puro): niente da
#  installare, e la resa e' la stessa su ogni installazione. Se quel file
#  mancasse si prova un convertitore esterno, e solo se non c'e' nemmeno quello
#  si apre il .md grezzo.
#
#  Il <base href> punta alla directory del documento ORIGINALE: senza quello i
#  rimandi relativi agli altri documenti - che i nostri .md usano molto - si
#  romperebbero, risolvendosi dentro la directory temporanea.
proc md_in_html {doc} {
    global env

    set tmp [expr {[info exists env(TMPDIR)] && $env(TMPDIR) ne "" ? $env(TMPDIR) : "/tmp"}]
    if {![file isdirectory $tmp] && [catch {file mkdir $tmp}]} { set tmp "/tmp" }
    set out [file join $tmp "lghmi_doc_[file rootname [file tail $doc]].html"]

    #  1. Il convertitore di LegoPST (md2html.tcl): in Tcl puro, quindi
    #     disponibile su ogni installazione senza installare niente. E' il
    #     primario, non il ripiego, per due ragioni: la resa e' identica
    #     dappertutto, e sui nostri documenti e' piu' fedele di markdown_py, che
    #     sbaglia i recinti di codice rientrati dentro una voce di elenco (li
    #     trasforma in un <code> malformato).
    if {[llength [info procs ::md2html::documento]] > 0} {
        if {![catch {md2html::documento $doc} pagina] && $pagina ne ""} {
            if {![catch {open $out w} fd]} {
                puts $fd $pagina
                close $fd
                return $out
            }
        }
    }

    #  2. Ripiego, se md2html.tcl non c'e' (deploy parziale, una bin vecchia):
    #     un convertitore esterno, se c'e'.
    set conv ""
    foreach c {markdown_py pandoc cmark-gfm cmark} {
        if {[auto_execok $c] ne ""} { set conv $c; break }
    }
    if {$conv eq ""} { return "" }
    set corpo ""
    switch -- $conv {
        markdown_py { set rc [catch {exec markdown_py -x tables -x fenced_code -x toc $doc} corpo] }
        pandoc      { set rc [catch {exec pandoc -f gfm -t html $doc} corpo] }
        default     { set rc [catch {exec $conv $doc} corpo] }
    }
    if {$rc || $corpo eq ""} { return "" }
    if {[catch {open $out w} fd]} { return "" }
    puts $fd "<!DOCTYPE html>\n<html><head><meta charset=\"utf-8\">"
    puts $fd "<title>[file tail $doc]</title>"
    puts $fd "<base href=\"file://[file dirname $doc]/\">"
    if {[llength [info procs ::md2html::stile]] > 0} {
        puts $fd "<style>[md2html::stile]</style>"
    }
    puts $fd "</head><body>"
    puts $fd $corpo
    puts $fd "</body></html>"
    close $fd
    return $out
}

#  Apre un documento del repository nel browser. Il browser lo sceglie
#  browser_disponibile di openhelp.tcl, la stessa di legopc, che scorre una
#  lista di candidati partendo da $LG_BROWSER.
proc apri_documento {relativo} {
    global env
    if {![info exists env(LEGOROOT)] || $env(LEGOROOT) eq ""} {
        tk_messageBox -icon error -title "Documentazione" -parent . -message \
            "LEGOROOT non definita: non trovo la documentazione."
        return
    }
    set doc [file join $env(LEGOROOT) $relativo]
    if {![file exists $doc]} {
        tk_messageBox -icon error -title "Documentazione" -parent . -message \
            "Documento non trovato:\n$doc"
        return
    }
    set preferito [expr {[info exists env(LG_BROWSER)] ? $env(LG_BROWSER) : ""}]
    set browser ""
    catch {set browser [browser_disponibile $preferito]}
    if {$browser eq ""} {
        tk_messageBox -icon error -title "Documentazione" -parent . -message \
            "Nessun browser disponibile.\nControlla LG_BROWSER (adesso: '$preferito')."
        return
    }
    # I .md passano per il convertitore, se c'e' uno.
    set nota ""
    if {[string tolower [file extension $doc]] eq ".md"} {
        set html [md_in_html $doc]
        if {$html ne ""} {
            set doc $html
        } else {
            set nota "  (md2html.tcl non trovato: testo non formattato)"
        }
    }
    if {[catch {exec $browser $doc &} err]} {
        tk_messageBox -icon error -title "Documentazione" -parent . -message \
            "Non riesco ad avviare il browser:\n$browser $doc\n\n$err"
        return
    }
    .status configure -text "Aperto nel browser: $relativo$nota"
}

# --- Menu "?" : versione dell'ambiente -----------------------------------

#  Versione di LegoPST, dalla stessa fonte che usa legopc: $LEGOROOT/version.h,
#  generato dal Makefile (bersaglio "version.h") a partire da git.
#
#  A differenza di legopc, qui il file NON viene generato se manca: lghmi puo'
#  girare dove il repository non e' scrivibile - per esempio dentro un bundle
#  FMU - e un selettore non deve mettersi a invocare make. Se manca si dice come
#  ottenerlo.
proc versione_legopst {} {
    global env
    set fuori [dict create versione "" build "" data "" nota ""]
    if {![info exists env(LEGOROOT)] || $env(LEGOROOT) eq ""} {
        dict set fuori nota "LEGOROOT non definita: profilo LegoPST non sorgiato."
        return $fuori
    }
    set vf [file join $env(LEGOROOT) version.h]
    if {![file exists $vf]} {
        dict set fuori nota "version.h assente. Si genera con:\n    make -C $env(LEGOROOT) -f Makefile.mk version.h"
        return $fuori
    }
    if {[catch {open $vf r} fd]} {
        dict set fuori nota "version.h non leggibile: $vf"
        return $fuori
    }
    while {[gets $fd riga] >= 0} {
        if {[regexp {^#define\s+GIT_VERSION_STRING\s+"([^"]+)"} $riga -> v]} {
            dict set fuori versione $v
        } elseif {[regexp {^#define\s+BUILD_NUMBER\s+(\d+)} $riga -> v]} {
            dict set fuori build $v
        } elseif {[regexp {^#define\s+BUILD_DATE_STRING\s+"([^"]+)"} $riga -> v]} {
            dict set fuori data $v
        }
    }
    close $fd
    return $fuori
}

proc about_legopst {} {
    global env

    if {[winfo exists .about]} { raise .about; focus .about; return }

    set v [versione_legopst]
    set testo "LegoPST\n"
    append testo "LegoPowerSystemTechnology\n\n"
    if {[dict get $v versione] ne ""} {
        append testo "Versione:\t[dict get $v versione]\n"
        append testo "Build:\t\t[dict get $v build]\n"
        append testo "Data:\t\t[dict get $v data]\n"
    } else {
        append testo "Versione:\tnon disponibile\n"
    }
    # Dove si sta lavorando: in questo ambiente e' la domanda che viene subito
    # dopo "che versione e'".
    append testo "\nLEGOROOT:\t[expr {[info exists env(LEGOROOT)] ? $env(LEGOROOT) : "-"}]\n"
    append testo "Simulatore:\t[expr {[simulatore_corrente] ne "" ? [simulatore_corrente] : "-"}]\n"
    append testo "Radice utente:\t[expr {[info exists env(LG_ENTRY)] ? $env(LG_ENTRY) : "-"}]\n"
    append testo "Directory:\t[pwd]"
    if {[dict get $v nota] ne ""} { append testo "\n\n[dict get $v nota]" }

    toplevel .about
    wm title .about "About LegoPST"
    wm resizable .about 0 0

    frame .about.f
    # Lo stesso logo di legopc, se c'e'.
    set logo [file join $env(LG_TIX) img lego.gif]
    if {[file exists $logo] && ![catch {image create photo imgabout -file $logo}]} {
        label .about.f.logo -image imgabout -bd 1 -relief sunken
        pack  .about.f.logo -side left -padx 3m -pady 2m
    }
    label .about.f.txt -justify left -padx 3m -pady 3m -text $testo
    pack  .about.f.txt -side left
    pack  .about.f -side top

    frame .about.b
    # Le note di rilascio stanno in $LG_INSTALL/relnotes.txt, come per legopc:
    # il pulsante compare solo se il file c'e' davvero.
    set rn ""
    if {[info exists env(LG_INSTALL)]} { set rn [file join $env(LG_INSTALL) relnotes.txt] }
    if {$rn ne "" && [file exists $rn]} {
        set ed [expr {[info exists env(LG_TEXTEDITOR)] && $env(LG_TEXTEDITOR) ne "" \
                      ? $env(LG_TEXTEDITOR) : "xdg-open"}]
        button .about.b.rn -text "Release Notes" -width 14 \
               -command [list catch [list exec $ed $rn &]]
        pack   .about.b.rn -side left -padx 3 -pady 6
    }
    button .about.b.ok -text "OK" -width 10 -command {destroy .about}
    pack   .about.b.ok -side left -padx 3 -pady 6
    pack   .about.b -side bottom
    bind .about <Escape> {destroy .about}
    focus .about.b.ok
}

# --- Interfaccia ---------------------------------------------------------
if {$doppia} {
    wm title . "LegoPST - HMI e faceplate"
    # Stessa larghezza del banco (new_monit, 680 px): le due finestre si usano
    # insieme, una sopra l'altra, e allineate stanno meglio. L'altezza e' quella
    # che serve a 12 righe di lista.
    wm geometry . 680x328
    wm minsize . 560 300
} elseif {$stazmode} {
    wm title . "LegoPST - Faceplate launcher (xstaz)"
    wm minsize . 520 280
} else {
    wm title . "LegoPST - HMI launcher"
    wm minsize . 340 280
}

# Barra dei menu. Finora non c'era: nasce per "Open loc path", che cambia la
# directory di lavoro del selettore e non e' un'azione sulle liste come i
# pulsanti in basso. Refresh e Quit ci stanno per comodita', ma restano anche
# in basso, dove si usano.
menu .mb -tearoff 0
. configure -menu .mb
menu .mb.file -tearoff 0
.mb add cascade -label "File" -menu .mb.file
# Le voci del menu File le costruisce aggiorna_menu_file, che lo rifa' da capo
# ogni volta che i path recenti cambiano. Con -insim nascono tutte disabilitate,
# recenti compresi: cambiare directory scollegherebbe il selettore dalla
# simulazione che l'ha lanciato.
menu .mb.tools -tearoff 0
.mb add cascade -label "Tools" -menu .mb.tools

# Menu "?" con lo stesso nome che usa legopc, per coerenza fra le due finestre.
# Font per le voci in rilievo: quello dei menu, in grassetto. Derivato invece
# che scritto a mano, cosi' segue il tema e la dimensione del sistema.
catch {
    font create fontMenuRilievo {*}[font actual TkMenuFont]
    font configure fontMenuRilievo -weight bold
}

menu .mb.aiuto -tearoff 0
.mb add cascade -label "?" -menu .mb.aiuto
.mb.aiuto add command -label "About LegoPST" -command about_legopst
.mb.aiuto add separator

# Le voci che puntano a un file assente nascono disabilitate invece di sparire:
# si vede che il documento e' previsto e che manca.
foreach _voce [documenti_aiuto] {
    if {$_voce eq "--"} { .mb.aiuto add separator; continue }
    if {$_voce eq "MODULI"} {
        # il manuale storico dei moduli: lo apre open_hlp di openhelp.tcl, la
        # stessa proc della voce Help di legopc
        if {[info exists env(LG_HTML)] \
            && [file exists [file join $env(LG_HTML) index.htm]] \
            && [llength [info procs open_hlp]] > 0} {
            .mb.aiuto add command -label "Help dei moduli (manuale storico)" \
                                  -command {open_hlp index}
        }
        continue
    }
    lassign $_voce _etichetta _rel _rilievo
    set _ok [expr {[info exists env(LEGOROOT)] \
                   && [file exists [file join $env(LEGOROOT) $_rel]]}]
    if {$_rilievo ne ""} {
        .mb.aiuto add command -label $_etichetta -command [list apri_documento $_rel] \
                              -state [expr {$_ok ? "normal" : "disabled"}] \
                              -font fontMenuRilievo
    } else {
        .mb.aiuto add command -label $_etichetta -command [list apri_documento $_rel] \
                              -state [expr {$_ok ? "normal" : "disabled"}]
    }
}
unset -nocomplain _voce _etichetta _rel _rilievo _ok

# Il simulatore corrente e' quello che il profilo ha scelto all'avvio
# (ksetsim_default: ~/.legosim, poi cassano0, poi il primo di ksims). La
# variabile tiene il radiobutton del sottomenu allineato.
set ::KSIMSCELTO [simulatore_corrente]

carica_recenti
aggiorna_menu_file
aggiorna_menu_tools

# La directory di lancio entra fra i recenti solo se e' una directory di
# simulazione (c'e' un S01 o variabili.rtf): lanciando lghmi da casa, in
# dir-scan, non ha senso ricordarsela. Cosi' il menu serve dalla prima volta,
# senza dover passare almeno una volta dal dialogo di selezione.
if {[file exists [file join [pwd] S01]] || \
    [file exists [file join [pwd] variabili.rtf]]} {
    ricorda_recente [pwd]
}

if {$insim} { wm title . "[wm title .]  (dal banco)" }

label .head -anchor w -padx 6 -pady 4 -text [expr {
        $doppia   ? "A sinistra le pagine di processo (draw2gr), a destra i faceplate di comando (xstaz)" :
        $stazmode ? "Le pagine di faceplate di comando (xstaz)" :
                    "Le task di processo e la loro HMI (draw2gr)"}]
label .hint -anchor w -padx 6 -foreground "#505050" -text \
    "Per aprire: doppio click sulla voce, oppure tasto destro -> Open page"
pack .head -side top -fill x
pack .hint -side top -fill x

# Intestazioni che dipendono dalla directory corrente: il simulatore S01 e il
# Set Sim path. Vengono create SEMPRE, anche quando non servono, perche'
# File -> Open loc path puo' cambiare directory a runtime e con essa la
# modalita': creandole solo all'avvio, aprendo una dir con S01 da una sessione
# partita in dir-scan non ci sarebbe nessun widget da riempire. Stanno in un
# frame perche' l'ordine fra le due resti stabile quando si mostrano e si
# nascondono (pack/pack forget dentro il frame, non sulla toplevel).
frame .hdr
pack  .hdr -side top -fill x
label .hdr.s01 -text "" -anchor w -padx 6 -foreground "#006400"
label .hdr.loc -text "" -anchor w -padx 6 -foreground blue

# Barra in basso: Refresh, mmi e Quit. Le pagine si aprono dalla lista (doppio
# click o tasto destro), non da un pulsante.
frame .btn
button .btn.refresh -text "Refresh" -command refresh_list
button .btn.quit    -text "Quit"    -command exit
# Lancia la simulazione nella directory corrente. Nome del comando come
# etichetta, come per il pulsante "mmi": si riconosce cosa fa. Parte disabilitato
# ed e' aggiorna_stato_startup a deciderne lo stato, in base alla presenza di
# variabili.rtf nella dir corrente.
button .btn.start -text "net_startup" -width 11 -state disabled \
                  -command lancia_net_startup
# Lancio di un'altra applicazione, non un'azione sulla lista: sta al centro
# della barra, largo il doppio e con il verde della finestra dell'MMI, cosi' si
# riconosce a colpo d'occhio. Centratura con `place` (non pack -expand): il
# centro e' quello della finestra, non della porzione lasciata libera da
# Refresh e Quit, che hanno larghezze diverse.
button .btn.mmi -text "mmi" -width 11 -command launch_mmi \
                -background "#50a050" -activebackground "#60c060" \
                -foreground black -activeforeground black
pack  .btn.refresh -side left  -padx 4 -pady 6
pack  .btn.start   -side left  -padx 4 -pady 6
pack  .btn.quit    -side right -padx 4 -pady 6
place .btn.mmi -relx 0.5 -rely 0.5 -anchor center
pack .btn -side bottom -fill x

label .status -text "" -anchor w -relief sunken -bd 1 -padx 4
pack .status -side bottom -fill x

#  Costruisce un riquadro "intestazione + lista + pulsante". Ritorna il path
#  della listbox.
proc crea_riquadro {parent titolo larghezza} {
    frame $parent
    if {$titolo ne ""} {
        label $parent.h -text $titolo -anchor w -padx 4 -pady 2 -foreground "#000080"
        pack  $parent.h -side top -fill x
    }
    frame $parent.f
    listbox $parent.f.lb -yscrollcommand "$parent.f.sb set" -height 12 \
                         -width $larghezza -activestyle dotbox -exportselection 0
    scrollbar $parent.f.sb -orient vertical -command "$parent.f.lb yview"
    pack $parent.f.sb -side right -fill y
    pack $parent.f.lb -side left -fill both -expand 1
    pack $parent.f -side top -fill both -expand 1
    return $parent.f.lb
}

if {$doppia} {
    # Due liste affiancate a meta' schermo ciascuna (39 caratteri -> ~328 px):
    # e' la ripartizione che sta in 680 px, la larghezza del banco. I faceplate
    # hanno etichette piu' lunghe, ma il divisorio si trascina.
    panedwindow .pw -orient horizontal -sashrelief raised -sashwidth 6
    pack .pw -side top -fill both -expand 1 -padx 6 -pady 2
    set LB_PROC [crea_riquadro .pw.proc "Pagine di processo" 39]
    set LB_STAZ [crea_riquadro .pw.staz "Faceplate xstaz"    39]
    .pw add .pw.proc -minsize 180
    .pw add .pw.staz -minsize 180
    bind $LB_PROC <Double-1> { launch_hmi }
    bind $LB_PROC <Return>   { launch_hmi }
    bind $LB_STAZ <Double-1> { apri_faceplate }
    bind $LB_STAZ <Return>   { apri_faceplate }
    bind $LB_PROC <Button-3> [list popup_open_page $LB_PROC launch_hmi     %y %X %Y]
    bind $LB_STAZ <Button-3> [list popup_open_page $LB_STAZ apri_faceplate %y %X %Y]
} elseif {$stazmode} {
    set LB_STAZ [crea_riquadro .f "" 68]
    pack .f -side top -fill both -expand 1 -padx 6 -pady 2
    bind $LB_STAZ <Double-1> { apri_faceplate }
    bind . <Return>          { apri_faceplate }
    bind $LB_STAZ <Button-3> [list popup_open_page $LB_STAZ apri_faceplate %y %X %Y]
} else {
    set LB_PROC [crea_riquadro .f "" 34]
    pack .f -side top -fill both -expand 1 -padx 6 -pady 2
    bind $LB_PROC <Double-1> { launch_hmi }
    bind . <Return>          { launch_hmi }
    bind $LB_PROC <Button-3> [list popup_open_page $LB_PROC launch_hmi %y %X %Y]
}

bind . <Escape> { exit }

refresh_list
