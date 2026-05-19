#!/usr/bin/env tclsh
#
# migrate_i5.tcl
#
# Script one-shot di migrazione da modalità flat (files_i5/) a modalità
# libreria (.i5 accanto al .pi4 nella directory della libreria).
#
# Uso:
#   tclsh migrate_i5.tcl
#
# Lo script:
#   1. Verifica che LG_FILESI5 e LG_LIBRARIES siano definiti
#   2. Per ogni .pi4 trovato in LG_LIBRARIES/*/, copia il .i5
#      corrispondente da LG_FILESI5/ nella directory della libreria.
#      Se lo stesso modulo esiste in più librerie, il .i5 viene copiato
#      in ciascuna (la sorgente viene cancellata solo dopo tutte le copie).
#   3. Rimuove i .i5 da LG_FILESI5/ che sono stati copiati con successo.
#   4. Se LG_FILESI5/ è rimasta vuota, la cancella (attivando la nuova modalità).
#   5. Stampa un report di cosa è stato fatto e cosa non è stato trovato.
#
# IMPORTANTE: dopo la migrazione, riaprire e risalvare ogni .tom in legopc
# per rigenerare il .top nel nuovo formato con i library path.
#

proc main {} {
    global env

    set i5dir  [expr {[info exists env(LG_FILESI5)]  ? $env(LG_FILESI5)  : ""}]
    set libsdir [expr {[info exists env(LG_LIBRARIES)] ? $env(LG_LIBRARIES) : ""}]

    if {$i5dir == "" || $libsdir == ""} {
        puts "ERRORE: variabili LG_FILESI5 e/o LG_LIBRARIES non definite."
        puts "Eseguire prima: source .profile_legoroot"
        exit 1
    }

    if {![file isdirectory $i5dir]} {
        puts "NOTA: $i5dir non esiste — sistema già in modalità libreria, niente da fare."
        exit 0
    }

    # Pass 1: raccoglie per ogni base-name le librerie destinazione
    # copied_to($base) = lista di libdir dove copiare
    # missing = lista di .pi4 senza .i5 sorgente
    array set copied_to {}
    set missing {}

    foreach libdir [glob -nocomplain $libsdir/*] {
        if {![file isdirectory $libdir]} continue
        foreach pi4 [glob -nocomplain $libdir/*.pi4] {
            set base [file rootname [file tail $pi4]]
            set src  $i5dir/${base}.i5
            if {[file exists $src]} {
                lappend copied_to($base) $libdir
            } else {
                lappend missing "$base  (atteso in $i5dir, pi4 in $libdir)"
            }
        }
    }

    # Pass 2: esegui le copie, poi cancella la sorgente
    set moved {}
    foreach base [array names copied_to] {
        set src $i5dir/${base}.i5
        foreach libdir $copied_to($base) {
            file copy -force $src $libdir/${base}.i5
            lappend moved "$base  → $libdir"
        }
        file delete $src
    }

    puts "\n=== migrate_i5: report ==="

    if {[llength $moved] > 0} {
        puts "\nSpostati ([llength $moved] copie, [array size copied_to] moduli distinti):"
        foreach m $moved { puts "  OK  $m" }
    }

    if {[llength $missing] > 0} {
        puts "\nNon trovati in $i5dir ([llength $missing]):"
        foreach m $missing { puts "  ??  $m" }
    }

    # controlla se restano file in files_i5/
    set residui [glob -nocomplain $i5dir/*.i5]
    if {[llength $residui] > 0} {
        puts "\nATTENZIONE: $i5dir non è vuota — file rimasti:"
        foreach r $residui { puts "  !   [file tail $r]" }
        puts "La directory NON è stata rimossa; la nuova modalità NON è attiva."
        puts "Verificare manualmente i file rimasti (moduli senza .pi4 in nessuna libreria)."
    } else {
        file delete $i5dir
        puts "\nDirectory $i5dir rimossa."
        puts "Nuova modalità libreria ATTIVA."
        puts "\nRicordarsi di riaprire e risalvare ogni .tom in legopc"
        puts "per rigenerare il .top con i library path."
    }
}

main
