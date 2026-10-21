# lgconn.tcl - le connessioni fra gli schemi di una task di regolazione.
#
#   lgconn -c [-v] [DIR]  controllo: elenca i problemi di Connessioni.reg
#                     rispetto alle pagine, senza grafica. Con -v elenca anche
#                     le note (gli ingressi di interfaccia non collegati ad
#                     altri schemi), altrimenti ne dice solo il numero. Esce
#                     con 0 se non ci sono errori, 1 se ce ne sono, 2 se la
#                     task non si legge.
#
# DIR e' la directory della task (quella con Context.ctx); senza, la corrente.
# Il formato dei file e le regole sono in lgconn_dati.tcl e in
# Alg_mmi/README.md.

namespace eval ::lgconn {}
set ::lgconn::_qui [file dirname [file normalize [info script]]]
source [file join $::lgconn::_qui lgconn_dati.tcl]

#  Il rapporto del controllo, sullo standard output. Ritorna il codice di
#  uscita.
proc ::lgconn::rapporto {dir {note 0}} {
    if {[catch {carica_task $dir} task]} {
        puts stderr "lgconn: $task"
        return 2
    }
    set probl [verifica $task]
    set n [dict create errore 0 avviso 0 nota 0]
    foreach p $probl { dict incr n [dict get $p gravita] }
    set n_conn 0
    foreach c [dict get $task conns] { if {![dict exists $c errore]} { incr n_conn } }
    puts "task:          [dict get $task dir]"
    puts "connessioni:   $n_conn in [dict get $task file_reg]"
    puts "pagine:        [llength [dict get $task pagine]], porte di interfaccia [dict size [dict get $task porte]]"
    puts ""
    set gravita_mostrate {errore avviso}
    if {$note} { lappend gravita_mostrate nota }
    foreach gravita $gravita_mostrate titolo {ERRORI AVVISI NOTE} {
        set righe {}
        foreach p $probl {
            if {[dict get $p gravita] ne $gravita} continue
            set r [dict get $p riga]
            lappend righe [expr {$r > 0 ? [format "riga %4d  " $r] : "           "}][dict get $p testo]
        }
        if {$righe eq {}} continue
        puts "$titolo ([llength $righe])"
        foreach r $righe { puts "  $r" }
        puts ""
    }
    puts "esito: [dict get $n errore] errori, [dict get $n avviso] avvisi, [dict get $n nota] note[expr {$note || ![dict get $n nota] ? "" : " (con -v le note sono elencate)"}]"
    return [expr {[dict get $n errore] > 0 ? 1 : 0}]
}

if {[lindex $::argv 0] eq "-c"} {
    #  un'uscita chiusa a meta' (lgconn -c | head) non e' un errore da mostrare
    set argomenti [lrange $::argv 1 end]
    set note 0
    if {[lindex $argomenti 0] eq "-v"} { set note 1; set argomenti [lrange $argomenti 1 end] }
    if {[catch {::lgconn::rapporto [expr {$argomenti ne {} ? [lindex $argomenti 0] : "."}] $note} esito]} {
        exit 2
    }
    exit $esito
}
