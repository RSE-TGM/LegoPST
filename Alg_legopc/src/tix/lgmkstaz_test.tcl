# lgmkstaz_test.tcl - prova di fedelta' (round-trip) del parser/writer di
# lgmkstaz: legge un r01.dat -> modello1, lo riscrive -> testo, lo rilegge ->
# modello2, e verifica che modello1 e modello2 siano lo stesso. E' un test di
# REGRESSIONE, non un'esecuzione di lgmkstaz: non produce nessuna GUI, non
# tocca NESSUNO dei file del corpus (solo lettura), e non serve un display.
#
# Uso:
#   tclsh lgmkstaz_test.tcl </dev/null
#
# Il corpus e' il r01.dat del catalogo di consultazione (committato nel
# repository, sempre disponibile) PIU', se la macchina li ha, tutti i
# r01.dat veri che si trovano sotto $HOME/legopst_*/legocad/ - il parco
# macchine di chi esegue il test. Nessun percorso personale e' scritto qui:
# la lista si scopre a runtime, quindi il test resta portabile da una
# macchina all'altra e vale anche su un checkout pulito (dove trova solo il
# file del catalogo).
#
# Uscita: 0 se tutti i file del corpus tornano fedeli, 1 altrimenti.

set qui [file dirname [file normalize [info script]]]
source [file join $qui lgmkstaz_dati.tcl]
source [file join $qui lgmkstaz_leggi.tcl]
source [file join $qui lgmkstaz_scrivi.tcl]

# Il catalogo di consultazione: e' nel repository, quindi sempre nel corpus.
# Path risolto rispetto a questo file (Alg_legopc/src/tix/), non alla cwd.
set legoroot [file normalize [file join $qui .. .. ..]]
set corpus [list [file join $legoroot Alg_rt grafica xstaz catalogo r01.dat]]

# I r01.dat veri delle aree di lavoro presenti sulla macchina, se ce ne sono.
if {[info exists ::env(HOME)]} {
    foreach area [glob -nocomplain -directory $::env(HOME) -type d legopst_*] {
        set legocad [file join $area legocad]
        if {![file isdirectory $legocad]} { continue }
        foreach f [glob -nocomplain -directory $legocad -join * r01.dat] {
            lappend corpus $f
        }
    }
}
set corpus [lsort -unique $corpus]

set ok 0
set falliti 0

foreach percorso $corpus {
    set etichetta [regsub "^$legoroot/" $percorso ""]
    if {[info exists ::env(HOME)]} {
        set etichetta [regsub "^$::env(HOME)/" $etichetta "~/"]
    }

    if {[catch {::lgmkstaz::leggi_file $percorso} modello1]} {
        puts "LETTURA FALLITA  $etichetta"
        puts "    $modello1"
        incr falliti
        continue
    }
    set npag [llength [dict get $modello1 pagine]]
    set nstaz [llength [dict get $modello1 stazioni]]
    set ngrezze 0
    foreach s [dict get $modello1 stazioni] {
        if {[dict get $s grezzo]} { incr ngrezze }
    }

    if {[catch {::lgmkstaz::scrivi_testo $modello1} testo2]} {
        puts "SCRITTURA FALLITA  $etichetta"
        puts "    $testo2"
        incr falliti
        continue
    }

    if {[catch {::lgmkstaz::leggi_testo $testo2} modello2]} {
        puts "RILETTURA FALLITA  $etichetta"
        puts "    $modello2"
        incr falliti
        continue
    }

    if {$modello1 eq $modello2} {
        puts "OK       $etichetta  ($npag pagine, $nstaz stazioni, $ngrezze grezze)"
        incr ok
    } else {
        puts "DIFFERISCE  $etichetta"
        set p1 [dict get $modello1 pagine]; set p2 [dict get $modello2 pagine]
        foreach a $p1 b $p2 {
            if {$a ne $b} { puts "    pagina:\n      prima: $a\n      dopo:  $b" }
        }
        set s1 [dict get $modello1 stazioni]; set s2 [dict get $modello2 stazioni]
        foreach a $s1 b $s2 {
            if {$a ne $b} {
                puts "    stazione (tipo [dict get $a tipo]):\n      prima: $a\n      dopo:  $b"
            }
        }
        incr falliti
    }
}

puts ""
puts "== $ok riusciti, $falliti falliti, su [llength $corpus] file =="
exit [expr {$falliti > 0 ? 1 : 0}]
