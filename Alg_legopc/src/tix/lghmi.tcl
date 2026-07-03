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
# In entrambe, selezionando una task la HMI viene lanciata in un processo
# INDIPENDENTE (detached):
#     cd <task> ; wish $LG_TIX/draw2gr.tcl 1 f22circ
# Il selettore resta aperto per altre scelte. "Quit" chiude SOLO il selettore:
# le HMI gia' aperte restano vive (le chiude l'utente).

package require Tk

set TASKROOT [expr {[info exists env(LG_TASKROOT)] && $env(LG_TASKROOT) ne "" \
                    ? $env(LG_TASKROOT) : [file join $env(HOME) legocad]}]
set LGTIX    [expr {[info exists env(LG_TIX)] ? $env(LG_TIX) : ""}]

# --- Modalita' S01: file "S01" nella cwd di lancio ----------------------
# La cwd e' la dir da cui l'helper ha fatto exec di wish (nessun cd), quindi
# tipicamente la dir del simulatore in esecuzione.
set S01FILE [file join [pwd] S01]
set s01mode [expr {[file exists $S01FILE] && ![file isdirectory $S01FILE]}]
set s01_name ""
set s01_desc ""

# -loc automatico (helper: LG_SIM_AUTO): il Set Sim path va deciso qui in base
# alla task selezionata (in S01 = dir della task). -loc DIR esplicito fissa
# invece LG_SIM_PATH nell'helper; -noloc lo lascia assente.
set SIMAUTO  [expr {[info exists env(LG_SIM_AUTO)] && $env(LG_SIM_AUTO) ne ""}]
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
proc parse_s01 {path} {
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
        if {![string equal -nocase $tipo P]} continue
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

proc refresh_list {} {
    global TASKROOT s01mode S01FILE ITEMS
    .f.lb delete 0 end
    set ITEMS {}

    if {$s01mode} {
        # Modalita' S01: lista dalle task di processo del simulatore.
        set entries [parse_s01 $S01FILE]
        foreach e $entries {
            lassign $e name desc dir
            set label [expr {$desc ne "" ? "$name  $desc" : $name}]
            .f.lb insert end $label
            lappend ITEMS [list $label $dir $name]
        }
        if {[info exists ::s01hdr]} {
            .s01hdr configure -text "Simulatore: $::s01_name  $::s01_desc"
        }
        if {[llength $ITEMS] == 0} {
            .status configure -text "Nessuna task di processo (P) nel file S01"
        } else {
            .f.lb selection clear 0 end
            .f.lb selection set 0
            .status configure -text "[llength $ITEMS] task di processo (S01: $::s01_name)"
        }
        return
    }

    # Modalita' dir-scan: sottodir di $TASKROOT con *.tom.
    if {![file isdirectory $TASKROOT]} {
        .status configure -text "Directory task non trovata: $TASKROOT"
        return
    }
    set tasks [scan_tasks $TASKROOT]
    foreach t $tasks {
        .f.lb insert end $t
        lappend ITEMS [list $t [file join $TASKROOT $t] $t]
    }
    if {[llength $tasks] == 0} {
        .status configure -text "Nessuna task (*.tom) in $TASKROOT"
    } else {
        .f.lb selection clear 0 end
        .f.lb selection set 0
        .status configure -text "[llength $tasks] task in $TASKROOT"
    }
}

# --- Set Sim path (LG_SIM_PATH) per la HMI in lancio --------------------
# -loc automatico (default, SIMAUTO): in S01 il sim path = dir della task
# selezionata; in dir-scan resta la cwd ereditata dall'helper. -loc DIR
# esplicito (SIMPATH senza SIMAUTO): fisso per tutte. -noloc: LG_SIM_PATH
# assente -> non tocchiamo l'ambiente.
proc apply_sim_path {taskdir} {
    global s01mode SIMAUTO env
    if {$s01mode && $SIMAUTO} {
        set env(LG_SIM_PATH) $taskdir
    }
}

# --- Lancio della HMI in un processo indipendente ------------------------
proc launch_hmi {} {
    global LGTIX ITEMS
    set sel [.f.lb curselection]
    if {[llength $sel] == 0} {
        .status configure -text "Seleziona una task dalla lista."
        return
    }
    lassign [lindex $ITEMS [lindex $sel 0]] label dir name
    set d2g [file join $LGTIX draw2gr.tcl]
    if {$LGTIX eq "" || ![file exists $d2g]} {
        tk_messageBox -icon error -title "LG_TIX" -parent . -message \
            "draw2gr.tcl non trovato (LG_TIX='$LGTIX').\nAvvia lghmi da un ambiente LegoPST (profilo sorgiato)."
        return
    }
    if {![file isdirectory $dir]} {
        tk_messageBox -icon error -title "Task" -parent . -message \
            "Directory della task non trovata:\n$dir"
        return
    }
    # Set Sim path per questa HMI (deve essere impostato PRIMA dell'exec, cosi'
    # il processo draw2gr eredita LG_SIM_PATH).
    apply_sim_path $dir
    # Processo INDIPENDENTE: cd nella task ed exec della HMI. `setsid` la mette
    # in una nuova sessione -> sopravvive al Quit del selettore. L'output va in
    # un log in /tmp (per non sporcare la task) utile per debug.
    set log [file join /tmp "lghmi_${name}.log"]
    set sh  "cd [list $dir] && exec wish [list $d2g] 1 f22circ >[list $log] 2>&1"
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
wm title . "LegoPST - HMI launcher"
wm minsize . 340 280

label .head -text "Seleziona una task e lancia la HMI (draw2gr)" -anchor w -padx 6 -pady 4
pack .head -side top -fill x

if {$s01mode} {
    # Intestazione simulatore (testo impostato da refresh_list dopo il parsing).
    label .s01hdr -text "" -anchor w -padx 6 -foreground "#006400"
    pack .s01hdr -side top -fill x
    set ::s01hdr 1
}

# Riga informativa sul Set Sim path pre-impostato.
if {$s01mode && $SIMAUTO} {
    label .loc -text "Set Sim path: directory della task selezionata" \
               -anchor w -padx 6 -foreground blue
    pack .loc -side top -fill x
} elseif {$SIMPATH ne ""} {
    label .loc -text "Set Sim path pre-impostato (-loc): $SIMPATH" \
               -anchor w -padx 6 -foreground blue
    pack .loc -side top -fill x
}

frame .btn
button .btn.launch  -text "Launch HMI" -command launch_hmi
button .btn.refresh -text "Refresh"    -command refresh_list
button .btn.quit    -text "Quit"       -command exit
pack .btn.launch .btn.refresh -side left  -padx 4 -pady 6
pack .btn.quit               -side right -padx 4 -pady 6
pack .btn -side bottom -fill x

label .status -text "" -anchor w -relief sunken -bd 1 -padx 4
pack .status -side bottom -fill x

frame .f
listbox .f.lb -yscrollcommand ".f.sb set" -height 12 -width 34 \
              -activestyle dotbox -exportselection 0
scrollbar .f.sb -orient vertical -command ".f.lb yview"
pack .f.sb -side right -fill y
pack .f.lb -side left -fill both -expand 1
pack .f -side top -fill both -expand 1 -padx 6 -pady 2

bind .f.lb <Double-1> { launch_hmi }
bind . <Return>       { launch_hmi }
bind . <Escape>       { exit }

refresh_list
