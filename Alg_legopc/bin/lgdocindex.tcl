# lgdocindex.tcl - copia NAVIGABILE della documentazione di LegoPST.
#
# PERCHE' ESISTE
#   L'indice ragionato (DOCUMENTATION_INDEX.html) e' una pagina HTML i cui
#   collegamenti puntano ai .md, e i .md fra loro. Un browser i .md non li
#   rende: ne mostra il sorgente, a meno che non abbia un'estensione apposta -
#   che nel container Docker non c'e', e su una macchina qualunque nemmeno.
#   md2html.tcl convertiva gia' i documenti aperti dal menu "?" di lghmi, ma
#   uno alla volta: seguendo un collegamento dalla pagina convertita, o
#   dall'indice, si tornava al sorgente.
#
# COSA FA
#   Parte dall'indice e segue i collegamenti ai .md, ricorsivamente. Ogni .md
#   raggiunto diventa una pagina HTML in una directory di cache dell'utente, e
#   nell'indice e nelle pagine i collegamenti ai .md vengono riscritti verso le
#   pagine convertite. Tutto il resto - immagini, .txt, i manuali storici in
#   HTML, le directory - continua a puntare agli ORIGINALI in $LEGOROOT, che
#   resta com'e' e puo' essere in sola lettura (nel container lo e').
#
#   Una pagina si riconverte solo se il suo .md e' piu' recente (o se e'
#   cambiato il convertitore); l'indice si riscrive sempre, costa niente.
#
# USO
#   tclsh lgdocindex.tcl            stampa il percorso dell'indice navigabile
#   source lgdocindex.tcl           poi: lgdocindex::costruisci $env(LEGOROOT)
#
#   costruisci ritorna il percorso dell'indice navigabile, o "" se non si puo'
#   fare (manca l'indice, o md2html.tcl): chi chiama apre allora l'originale.

package require Tcl 8.5

namespace eval lgdocindex {
    variable qui [file dirname [file normalize [info script]]]
}

#  La directory di cache. Nella home (XDG), e non in /tmp, perche' sia
#  dell'utente: in /tmp un nome fisso potrebbe essere di un altro. Se la home
#  non e' scrivibile si ripiega su /tmp con il nome dell'utente.
proc lgdocindex::cache {} {
    global env
    set candidati {}
    if {[info exists env(XDG_CACHE_HOME)] && $env(XDG_CACHE_HOME) ne ""} {
        lappend candidati [file join $env(XDG_CACHE_HOME) legopst doc]
    }
    if {[info exists env(HOME)] && $env(HOME) ne ""} {
        lappend candidati [file join $env(HOME) .cache legopst doc]
    }
    set chi [expr {[info exists env(USER)] && $env(USER) ne "" ? $env(USER) : [pid]}]
    lappend candidati [file join /tmp "lgdoc_$chi"]
    foreach d $candidati {
        if {![catch {file mkdir $d}] && [file writable $d]} { return $d }
    }
    return ""
}

proc lgdocindex::leggi {f} {
    set fd [open $f r]
    fconfigure $fd -encoding utf-8
    set t [read $fd]
    close $fd
    return $t
}

proc lgdocindex::scrivi {f testo} {
    file mkdir [file dirname $f]
    set fd [open $f w]
    fconfigure $fd -encoding utf-8
    puts -nonewline $fd $testo
    close $fd
}

#  La pagina convertita di un .md: stesso percorso relativo a $root, sotto la
#  cache, con .html in coda (README.md -> README.md.html: il nome dice da dove
#  viene). Un .md fuori da $root non si converte.
proc lgdocindex::pagina_di {md root cache} {
    set rel [string range $md [expr {[string length $root] + 1}] end]
    return [file join $cache "$rel.html"]
}

#  Dove porta un href scritto in un documento che sta in $dir. Ritorna il
#  percorso assoluto del .md se e' un collegamento a un .md di $root che esiste,
#  altrimenti "". L'eventuale #frammento va tolto prima.
proc lgdocindex::md_puntato {percorso dir root} {
    if {$percorso eq ""} { return "" }
    if {[regexp {^[a-zA-Z][a-zA-Z0-9+.-]*:} $percorso]} { return "" }   ;# http:, mailto:, file:
    if {[string tolower [file extension $percorso]] ne ".md"} { return "" }
    set abs [file normalize [file join $dir $percorso]]
    if {![file isfile $abs]} { return "" }
    if {[string first "$root/" $abs] != 0} { return "" }
    return $abs
}

#  Riscrive gli href di una pagina HTML. $dir e' la directory rispetto a cui i
#  suoi collegamenti relativi vanno letti (quella del documento ORIGINALE);
#  $se_stessa il percorso della pagina che si sta scrivendo.
#    - a un .md di $root        -> la sua pagina convertita (e lo si accoda)
#    - "#ancora"                -> la pagina stessa: con un <base> in testa un
#                                  "#x" nudo porterebbe alla directory base
#    - tutto il resto           -> com'e' (lo risolve il <base>, sull'originale)
proc lgdocindex::riscrivi {html dir se_stessa root cache codaVar} {
    upvar 1 $codaVar coda
    set fuori ""
    set pos 0
    while {[regexp -indices -start $pos {href="([^"]*)"} $html tutto dentro]} {
        lassign $tutto t0 t1
        lassign $dentro d0 d1
        set href [string range $html $d0 $d1]
        append fuori [string range $html $pos [expr {$t0 - 1}]]
        set nuovo $href
        if {[string index $href 0] eq "#"} {
            set nuovo "file://$se_stessa$href"
        } else {
            set fram ""
            set percorso $href
            set i [string first "#" $href]
            if {$i >= 0} {
                set fram [string range $href $i end]
                set percorso [string range $href 0 [expr {$i - 1}]]
            }
            set md [md_puntato $percorso $dir $root]
            if {$md ne ""} {
                set nuovo "file://[pagina_di $md $root $cache]$fram"
                lappend coda $md
            }
        }
        append fuori "href=\"$nuovo\""
        set pos [expr {$t1 + 1}]
    }
    append fuori [string range $html $pos end]
    return $fuori
}

#  Costruisce (o aggiorna) la copia navigabile. Ritorna il percorso dell'indice.
proc lgdocindex::costruisci {root} {
    variable qui
    set root [file normalize $root]
    set indice [file join $root DOCUMENTATION_INDEX.html]
    if {![file isfile $indice]} { return "" }
    if {[llength [info procs ::md2html::documento]] == 0} {
        set conv [file join $qui md2html.tcl]
        if {[catch {source $conv}]} { return "" }
    }
    set cache [cache]
    if {$cache eq ""} { return "" }

    #  Una pagina e' buona se e' piu' recente del suo .md e di chi l'ha fatta.
    set attrezzi 0
    foreach f [list [file join $qui md2html.tcl] [file join $qui lgdocindex.tcl]] {
        if {![catch {file mtime $f} m] && $m > $attrezzi} { set attrezzi $m }
    }

    # --- l'indice: <base> sull'originale, i .md verso le pagine ------------
    set coda {}
    set out [file join $cache DOCUMENTATION_INDEX.html]
    set html [riscrivi [leggi $indice] $root $out $root $cache coda]
    if {![regsub -nocase {<head[^>]*>} $html "&\n<base href=\"file://$root/\">" html]} {
        set html "<base href=\"file://$root/\">\n$html"
    }
    scrivi $out $html

    # --- i documenti, seguendo i collegamenti -------------------------------
    array set visto {}
    while {[llength $coda] > 0} {
        set md [lindex $coda 0]
        set coda [lrange $coda 1 end]
        if {[info exists visto($md)]} continue
        set visto($md) 1
        set pagina [pagina_di $md $root $cache]
        set fresca [expr {[file isfile $pagina] \
                          && [file mtime $pagina] >= [file mtime $md] \
                          && [file mtime $pagina] >= $attrezzi}]
        if {$fresca} {
            #  niente da convertire, ma i suoi collegamenti vanno seguiti lo
            #  stesso: si leggono dal sorgente, che costa poco
            foreach {- href} [regexp -all -inline {\]\(([^)\s]+)\)} [leggi $md]] {
                set i [string first "#" $href]
                if {$i >= 0} { set href [string range $href 0 [expr {$i - 1}]] }
                set altro [md_puntato $href [file dirname $md] $root]
                if {$altro ne ""} { lappend coda $altro }
            }
            continue
        }
        if {[catch {md2html::documento $md} html]} continue
        if {[catch {scrivi $pagina [riscrivi $html [file dirname $md] $pagina $root $cache coda]}]} continue
    }
    return $out
}

#  Eseguito come script: stampa il percorso dell'indice navigabile.
if {[info exists argv0] && [file tail $argv0] eq "lgdocindex.tcl"} {
    if {![info exists env(LEGOROOT)] || $env(LEGOROOT) eq ""} {
        puts stderr "lgdocindex: LEGOROOT non definito"
        exit 1
    }
    set p [lgdocindex::costruisci $env(LEGOROOT)]
    if {$p eq ""} {
        puts stderr "lgdocindex: non riesco a costruire la copia navigabile"
        exit 1
    }
    puts $p
}
