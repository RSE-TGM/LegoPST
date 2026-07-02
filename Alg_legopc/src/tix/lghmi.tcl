# lghmi.tcl - selettore grafico delle task LegoPST -> lancia la HMI draw2gr.
# Si avvia con l'helper "lghmi" (Alg_rt/bin/lghmi), che sorgia il profilo e fa
# `wish $LG_TIX/lghmi.tcl`.
#
# Elenca le task in $LG_TASKROOT (default $HOME/legocad): le sottodirectory che
# contengono almeno un file *.tom (le dir di libreria come libgraph/libut, prive
# di .tom, sono quindi escluse). Selezionando una task, la HMI viene lanciata in
# un processo INDIPENDENTE (detached):
#     cd <task> ; wish $LG_TIX/draw2gr.tcl 1 f22circ
# Il selettore resta aperto per altre scelte. "Quit" chiude SOLO il selettore:
# le HMI gia' aperte restano vive (le chiude l'utente).

package require Tk

set TASKROOT [expr {[info exists env(LG_TASKROOT)] && $env(LG_TASKROOT) ne "" \
                    ? $env(LG_TASKROOT) : [file join $env(HOME) legocad]}]
set LGTIX    [expr {[info exists env(LG_TIX)] ? $env(LG_TIX) : ""}]
# -loc (helper): LG_SIM_PATH = dir di lancio, pre-impostata come Set Sim path
# nelle HMI (ereditata dal processo draw2gr). Qui la mostriamo soltanto.
set SIMLOC   [expr {[info exists env(LG_SIM_PATH)] && $env(LG_SIM_PATH) ne "" ? $env(LG_SIM_PATH) : ""}]

# --- Task = sottodir di $root con almeno un *.tom ------------------------
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
    global TASKROOT
    .f.lb delete 0 end
    if {![file isdirectory $TASKROOT]} {
        .status configure -text "Directory task non trovata: $TASKROOT"
        return
    }
    set tasks [scan_tasks $TASKROOT]
    foreach t $tasks { .f.lb insert end $t }
    if {[llength $tasks] == 0} {
        .status configure -text "Nessuna task (*.tom) in $TASKROOT"
    } else {
        .f.lb selection clear 0 end
        .f.lb selection set 0
        .status configure -text "[llength $tasks] task in $TASKROOT"
    }
}

# --- Lancio della HMI in un processo indipendente ------------------------
proc launch_hmi {} {
    global TASKROOT LGTIX
    set sel [.f.lb curselection]
    if {[llength $sel] == 0} {
        .status configure -text "Seleziona una task dalla lista."
        return
    }
    set name [.f.lb get [lindex $sel 0]]
    set dir  [file join $TASKROOT $name]
    set d2g  [file join $LGTIX draw2gr.tcl]
    if {$LGTIX eq "" || ![file exists $d2g]} {
        tk_messageBox -icon error -title "LG_TIX" -parent . -message \
            "draw2gr.tcl non trovato (LG_TIX='$LGTIX').\nAvvia lghmi da un ambiente LegoPST (profilo sorgiato)."
        return
    }
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

if {$SIMLOC ne ""} {
    label .loc -text "Set Sim path pre-impostato (-loc): $SIMLOC" \
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
