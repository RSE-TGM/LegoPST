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
# In entrambe, selezionando una task la HMI viene lanciata in un processo
# INDIPENDENTE (detached):
#     cd <task> ; wish $LG_TIX/draw2gr.tcl 1 f22circ
# Il selettore resta aperto per altre scelte. "Quit" chiude SOLO il selettore:
# le HMI gia' aperte restano vive (le chiude l'utente).

package require Tk

set TASKROOT [expr {[info exists env(LG_TASKROOT)] && $env(LG_TASKROOT) ne "" \
                    ? $env(LG_TASKROOT) : [file join $env(HOME) legocad]}]

#  Tre modalita':
#    -proc   solo le pagine di processo (task -> HMI draw2gr)
#    -staz   solo i faceplate (pagine di r02.dat -> xstaz)
#    nessuna delle due: entrambe, in due liste affiancate
set stazmode [expr {[lsearch -exact $argv "-staz"] >= 0}]
set procmode [expr {[lsearch -exact $argv "-proc"] >= 0}]
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
        if {[info exists ::s01hdr]} {
            .s01hdr configure -text "Simulatore: $::s01_name  $::s01_desc"
        }
        if {[llength $ITEMS_PROC] == 0} {
            return "Nessuna task di processo (P) nel file S01"
        }
        $LB_PROC selection clear 0 end
        $LB_PROC selection set 0
        return "[llength $ITEMS_PROC] task di processo (S01: $::s01_name)"
    }

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

# --- Interfaccia ---------------------------------------------------------
if {$doppia} {
    wm title . "LegoPST - HMI e faceplate"
    wm minsize . 860 320
} elseif {$stazmode} {
    wm title . "LegoPST - Faceplate launcher (xstaz)"
    wm minsize . 520 280
} else {
    wm title . "LegoPST - HMI launcher"
    wm minsize . 340 280
}

label .head -anchor w -padx 6 -pady 4 -text [expr {
        $doppia   ? "A sinistra le pagine di processo (draw2gr), a destra i faceplate di comando (xstaz)" :
        $stazmode ? "Seleziona una pagina di faceplate e aprila (xstaz)" :
                    "Seleziona una task e lancia la HMI (draw2gr)"}]
pack .head -side top -fill x

if {$s01mode} {
    # Intestazione simulatore (testo impostato da riempi_proc dopo il parsing).
    label .s01hdr -text "" -anchor w -padx 6 -foreground "#006400"
    pack .s01hdr -side top -fill x
    set ::s01hdr 1
}

# Riga informativa sul Set Sim path pre-impostato (dir del simulatore attivo).
if {$SIMPATH ne ""} {
    label .loc -text "Set Sim path pre-impostato (-loc): $SIMPATH" \
               -anchor w -padx 6 -foreground blue
    pack .loc -side top -fill x
}

# Barra in basso: Refresh e Quit sono comuni; il pulsante d'azione sta qui solo
# quando la lista e' una sola (in modalita' doppia ogni riquadro ha il suo).
frame .btn
if {!$doppia} {
    button .btn.launch -text [expr {$stazmode ? "Apri faceplate" : "Launch HMI"}] \
                       -command [expr {$stazmode ? "apri_faceplate" : "launch_hmi"}]
    pack .btn.launch -side left -padx 4 -pady 6
}
button .btn.refresh -text "Refresh" -command refresh_list
button .btn.quit    -text "Quit"    -command exit
pack .btn.refresh -side left  -padx 4 -pady 6
pack .btn.quit    -side right -padx 4 -pady 6
pack .btn -side bottom -fill x

label .status -text "" -anchor w -relief sunken -bd 1 -padx 4
pack .status -side bottom -fill x

#  Costruisce un riquadro "intestazione + lista + pulsante". Ritorna il path
#  della listbox.
proc crea_riquadro {parent titolo larghezza testo_bottone azione} {
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
    if {$testo_bottone ne ""} {
        button $parent.b -text $testo_bottone -command $azione
        pack   $parent.b -side bottom -fill x -padx 4 -pady 4
    }
    return $parent.f.lb
}

if {$doppia} {
    # Due liste affiancate, divisorio trascinabile: i faceplate hanno etichette
    # piu' lunghe, quindi partono con piu' spazio.
    panedwindow .pw -orient horizontal -sashrelief raised -sashwidth 6
    pack .pw -side top -fill both -expand 1 -padx 6 -pady 2
    set LB_PROC [crea_riquadro .pw.proc "Pagine di processo" 30 "Launch HMI"     launch_hmi]
    set LB_STAZ [crea_riquadro .pw.staz "Faceplate xstaz"    68 "Apri faceplate" apri_faceplate]
    .pw add .pw.proc -minsize 180
    .pw add .pw.staz -minsize 260
    bind $LB_PROC <Double-1> { launch_hmi }
    bind $LB_PROC <Return>   { launch_hmi }
    bind $LB_STAZ <Double-1> { apri_faceplate }
    bind $LB_STAZ <Return>   { apri_faceplate }
} elseif {$stazmode} {
    set LB_STAZ [crea_riquadro .f "" 68 "" ""]
    pack .f -side top -fill both -expand 1 -padx 6 -pady 2
    bind $LB_STAZ <Double-1> { apri_faceplate }
    bind . <Return>          { apri_faceplate }
} else {
    set LB_PROC [crea_riquadro .f "" 34 "" ""]
    pack .f -side top -fill both -expand 1 -padx 6 -pady 2
    bind $LB_PROC <Double-1> { launch_hmi }
    bind . <Return>          { launch_hmi }
}

bind . <Escape> { exit }

refresh_list
