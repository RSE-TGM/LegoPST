#!/usr/bin/env tclsh
#
# genera_geometria.tcl - ricava da AlgLib/libinclude/newstaz.h la posizione di
# ogni oggetto dentro la sua stazione, e la scrive in lgmkstaz_geometria.tcl.
#
#   tclsh genera_geometria.tcl [<newstaz.h>] > lgmkstaz_geometria.tcl
#
# Serve al disegno "con i parametri" di lgmkstaz: lo sprite di un tipo mostra
# la forma vera della stazione ma con i contenuti generici del catalogo, e per
# scriverci sopra l'etichetta di QUESTA stazione bisogna sapere dove xstaz la
# mette - cioe' la x/y dell'oggetto nella tabella new_staz[], piu' il
# sottotipo (che decide allineamento e font) e il flag.
#
# Si genera invece di trascriverlo a mano perche' la tabella e' lunga 888
# righe: la trascrizione a mano del solo elenco dei tipi, in
# lgmkstaz_dati.tcl, aveva gia' prodotto un errore (un INDICATORE di troppo in
# SINCRONO) trovato solo dal test di round-trip.
#
# Il file generato NON sostituisce il catalogo di lgmkstaz_dati.tcl: quello
# porta la grammatica (quali campi ha ogni oggetto), questo le coordinate. I
# due elencano gli stessi oggetti nello stesso ordine, e lgmkstaz_test.tcl lo
# verifica.

set qui [file dirname [file normalize [info script]]]

proc trova_newstaz {qui} {
    if {[llength $::argv] > 0} { return [lindex $::argv 0] }
    if {[info exists ::env(LEGOROOT)] && $::env(LEGOROOT) ne ""} {
        set c [file join $::env(LEGOROOT) AlgLib libinclude newstaz.h]
        if {[file readable $c]} { return $c }
    }
    set dir $qui
    for {set i 0} {$i < 8} {incr i} {
        set c [file join $dir AlgLib libinclude newstaz.h]
        if {[file readable $c]} { return $c }
        set su [file dirname $dir]
        if {$su eq $dir} break
        set dir $su
    }
    return ""
}

set percorso [trova_newstaz $qui]
if {$percorso eq "" || ![file readable $percorso]} {
    puts stderr "genera_geometria: newstaz.h non trovato"
    exit 1
}

set ch [open $percorso r]
fconfigure $ch -encoding iso8859-1
set src [read $ch]
close $ch

set inizio [string first "TIPI_NEWSTAZ  new_staz\[\]" $src]
if {$inizio < 0} {
    puts stderr "genera_geometria: tabella new_staz\[\] non trovata in $percorso"
    exit 1
}
set tab [string range $src $inizio end]
set tab [string range $tab 0 [expr {[string first "\n\};" $tab] - 1}]]
#  via i commenti: dentro la tabella sono note, non dati
regsub -all {/\*.*?\*/} $tab "" tab

#  Il nome di grammatica che usa lgmkstaz_dati.tcl per un oggetto della
#  tabella: STRINGA_DESCR e' la STRINGA della grammatica, e un INDICATORE con
#  sottotipo INDIC_AGO_ERR e' INDICATORE_ERR (ha i campi in piu' per l'errore).
proc nome_grammatica {tipo sottotipo} {
    if {$tipo eq "STRINGA_DESCR"} { return STRINGA }
    if {$tipo eq "INDICATORE" && $sottotipo eq "INDIC_AGO_ERR"} { return INDICATORE_ERR }
    return $tipo
}

set righe {}
set ntipi 0
set noggetti 0
foreach {tutto nome resto} [regexp -all -inline {\{\s*"([A-Z0-9_]+)"\s*,([^\}]*)\}} $tab] {
    set tok {}
    foreach t [split [string map {"\n" " "} $resto] ,] {
        set t [string trim $t]
        if {$t ne ""} { lappend tok $t }
    }
    set n [lindex $tok 0]
    set corpo [lrange $tok 3 end]
    set oggetti {}
    for {set i 0} {$i < $n} {incr i} {
        lassign [lrange $corpo [expr {5*$i}] [expr {5*$i+4}]] tipo sottotipo x y flag
        if {$tipo eq ""} break
        lappend oggetti [list [nome_grammatica $tipo $sottotipo] $sottotipo $x $y $flag]
        incr noggetti
    }
    lappend righe [format "    %-10s {%s}" $nome $oggetti]
    incr ntipi
}

puts "#  GENERATO da genera_geometria.tcl - non modificare a mano."
puts "#"
puts "#  Per ogni tipo di stazione, gli oggetti nell'ordine in cui compaiono in"
puts "#  new_staz\[\] (AlgLib/libinclude/newstaz.h), ciascuno come"
puts "#"
puts "#      {<tipo di grammatica> <sottotipo> <x> <y> <flag>}"
puts "#"
puts "#  x e y sono in PIXEL, relativi all'angolo alto-sinistra della stazione"
puts "#  (il riquadro che xstaz crea in cnewstaz.c), non alla pagina. Il"
puts "#  sottotipo decide come l'oggetto viene disegnato - per una STRINGA"
puts "#  l'allineamento e il font, per un LED da che parte sta l'etichetta."
puts "#"
puts "#  Rigenerare con:  tclsh genera_geometria.tcl > lgmkstaz_geometria.tcl"
puts ""
puts "array set ::lgmkstaz::geometria {"
puts [join $righe "\n"]
puts "}"
puts ""
puts "#  $ntipi tipi, $noggetti oggetti."

puts stderr "genera_geometria: $ntipi tipi, $noggetti oggetti (da $percorso)"
