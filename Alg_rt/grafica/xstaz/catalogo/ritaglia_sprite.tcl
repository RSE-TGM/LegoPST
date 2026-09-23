#!/usr/bin/env wish
#
# ritaglia_sprite.tcl - ricava uno sprite PNG per ciascun tipo di stazione
# ritagliandolo dalle catture reali di xstaz gia' presenti in questa directory
# (pag_*.png). Gli sprite servono a lgmkstaz, che li disegna al posto dei
# segnaposto rettangolari: quello che si vede nell'editor e' allora la vera
# immagine prodotta da xstaz, non un disegno approssimato.
#
#   wish ritaglia_sprite.tcl            # -> staz/<TIPO>.png
#   wish ritaglia_sprite.tcl -prova     # dice cosa farebbe, senza scrivere
#
# Perche' Tcl/Tk e non Python: su questa macchina non ci sono ne' Pillow ne'
# ImageMagick, mentre Tk 8.6 legge e scrive PNG da solo e "photo copy -from"
# ritaglia. Nessuna dipendenza esterna, quindi lo script gira dove gira
# lgmkstaz.
#
# NON ricattura le pagine: lavora sui PNG gia' committati. Si ricatturano solo
# se cambia la tabella new_staz[] di AlgLib/libinclude/newstaz.h (allora si
# rifa' prima r01.dat con genera_catalogo.py, poi si ricattura, poi si rilancia
# questo script). Vedi README.md.
#
# --- Come si calcola il ritaglio ------------------------------------------
#
# Ogni PNG e' la cattura di una pagina di xstaz. La finestra della pagina e'
# grande posmx*62 x posmy*62 (xstaz.c:589), ma la cattura committata NON la
# comprende tutta: il bordo vuoto attorno al contenuto e' stato tolto, e ne
# resta una cornice uniforme di pochi pixel. Lo script non da' per buono un
# numero: ricava lo scarto dalla differenza fra la dimensione del PNG e quella
# del contenuto (il rettangolo che racchiude le stazioni della pagina, che il
# r01.dat di questa directory descrive per intero), e PRETENDE che lo scarto
# sia identico in orizzontale e in verticale.
#
# Quel conto pero' non basta da solo: una stazione spostata DENTRO la pagina
# lascia il rettangolo complessivo identico, e i ritagli uscirebbero sbagliati
# senza che niente se ne accorga. Per questo si controlla anche, cella per
# cella, che la cattura sia occupata esattamente dove il r01.dat dice
# (verifica_occupazione): il centro di una cella libera ha il colore di
# sfondo della pagina (sfondo_window, xstaz.c), quello di una cella coperta da
# una stazione no. E' una verifica indipendente dal conto dello scarto, e
# insieme i due coprono sia lo spostamento del bordo sia quello interno.
#
# L'asse Y e' capovolto (cnewstaz.c: ydraw = height - posiy0*62 - htot): la
# cella y minore sta in BASSO. Da qui il (y1 - posy - altezza) nel conto.
#
# --- Quale stazione e' il campione di un tipo -----------------------------
#
# Il catalogo contiene, per ogni tipo, DUE stazioni: il campione e una stazione
# TESTO che ne scrive il nome sotto (genera_catalogo.py). Si distinguono dalla
# descrizione: il campione ha DESCRIZIONE uguale al proprio TIPO, l'etichetta ha
# DESCRIZIONE "etichetta". Lo sprite si ritaglia dal campione.

package require Tk
wm withdraw .

set ::solo_prova [expr {[lindex $::argv 0] eq "-prova"}]
set ::qui [file dirname [file normalize [info script]]]

#  I moduli di lgmkstaz (catalogo dei tipi e parser del r01.dat) si riusano
#  invece di riscriverli: il parser e' lo stesso che lgmkstaz usa sui file veri,
#  gia' coperto dal suo test di round-trip. Si cerca prima il sorgente e poi la
#  copia distribuita, risalendo le directory finche' non si trova - non si conta
#  un numero fisso di livelli, cosi' vale da qualunque punto lo si lanci.
proc trova_moduli {partenza} {
    if {[info exists ::env(LEGOROOT)] && $::env(LEGOROOT) ne ""} {
        foreach sotto {{Alg_legopc src tix} {Alg_legopc bin}} {
            set c [file join $::env(LEGOROOT) {*}$sotto]
            if {[file readable [file join $c lgmkstaz_dati.tcl]]} { return $c }
        }
    }
    set dir $partenza
    for {set i 0} {$i < 8} {incr i} {
        foreach sotto {{Alg_legopc src tix} {Alg_legopc bin}} {
            set c [file join $dir {*}$sotto]
            if {[file readable [file join $c lgmkstaz_dati.tcl]]} { return $c }
        }
        set su [file dirname $dir]
        if {$su eq $dir} break
        set dir $su
    }
    return ""
}

#  Colore di sfondo della pagina fuori dalle stazioni: sfondo_window di
#  xstaz.c (0xcc,0xe5,0xe5). Le stazioni disegnano sul proprio sfondo
#  (sfondo_staz / sfondo_label) o in nero, mai su questo.
set ::sfondo_pagina {204 229 229}

#  Confronta, cella per cella, le stazioni dichiarate dal r01.dat con quelle
#  che si vedono davvero nella cattura. Ritorna la lista delle celle discordi
#  (vuota se tutto torna). E' il controllo che si accorge di una stazione
#  spostata dentro la pagina, che non altera il rettangolo complessivo.
proc verifica_occupazione {img stazioni x0 y1 bordo cella} {
    array set occupata {}
    foreach s $stazioni {
        set px [dict get $s posx]; set py [dict get $s posy]
        for {set i 0} {$i < [dict get $s larg]} {incr i} {
            for {set j 0} {$j < [dict get $s altezza]} {incr j} {
                set occupata([expr {$px+$i}],[expr {$py+$j}]) 1
            }
        }
    }
    set larg_img [image width $img]; set alt_img [image height $img]
    set discordi {}
    foreach chiave [array names occupata] {
        lassign [split $chiave ,] cx cy
        set sx [expr {$bordo + ($cx - $x0)*$cella + $cella/2}]
        set sy [expr {$bordo + ($y1 - $cy - 1)*$cella + $cella/2}]
        if {$sx < 0 || $sy < 0 || $sx >= $larg_img || $sy >= $alt_img} {
            lappend discordi "($cx,$cy) fuori dalla cattura"
            continue
        }
        if {[$img get $sx $sy] eq $::sfondo_pagina} {
            lappend discordi "($cx,$cy) dichiarata piena, nella cattura e' sfondo"
        }
    }
    #  e il contrario: qualcosa disegnato dove il r01.dat non dichiara niente
    for {set cx $x0} {$cx < $x0 + ($larg_img - 2*$bordo)/$cella} {incr cx} {
        for {set cy [expr {$y1 - ($alt_img - 2*$bordo)/$cella}]} {$cy < $y1} {incr cy} {
            if {[info exists occupata($cx,$cy)]} { continue }
            set sx [expr {$bordo + ($cx - $x0)*$cella + $cella/2}]
            set sy [expr {$bordo + ($y1 - $cy - 1)*$cella + $cella/2}]
            if {$sx < 0 || $sy < 0 || $sx >= $larg_img || $sy >= $alt_img} { continue }
            if {[$img get $sx $sy] ne $::sfondo_pagina} {
                lappend discordi "($cx,$cy) dichiarata vuota, nella cattura e' disegnata"
            }
        }
    }
    return $discordi
}

set moduli [trova_moduli $::qui]
if {$moduli eq ""} {
    puts stderr "ritaglia_sprite: lgmkstaz_dati.tcl non trovato risalendo da $::qui"
    exit 1
}
namespace eval ::lgmkstaz {}
set ::lgmkstaz::_qui $moduli
foreach f {lgmkstaz_dati.tcl lgmkstaz_leggi.tcl} {
    if {[catch {source [file join $moduli $f]} err]} {
        puts stderr "ritaglia_sprite: non riesco a caricare $f: $err"
        exit 1
    }
}

set cella $::lgmkstaz::dim_cella_px
set dir_out [file join $::qui staz]

set r01 [file join $::qui r01.dat]
if {![file readable $r01]} {
    puts stderr "ritaglia_sprite: $r01 non leggibile"
    exit 1
}
if {[catch {::lgmkstaz::leggi_file $r01} modello]} {
    puts stderr "ritaglia_sprite: r01.dat non si legge: $modello"
    exit 1
}

#  nome della pagina per numero, e stazioni raggruppate per pagina
array set nome_pagina {}
foreach p [dict get $modello pagine] {
    set nome_pagina([dict get $p numero]) [dict get $p nome]
}
array set per_pagina {}
foreach s [dict get $modello stazioni] {
    lappend per_pagina([dict get $s pagina]) $s
}

set errori 0
set scritti 0
array set fatto {}

foreach numero [lsort -integer [array names per_pagina]] {
    set nome $nome_pagina($numero)
    set png [file join $::qui "pag_$nome.png"]
    if {![file readable $png]} {
        puts stderr "pagina $nome: manca la cattura [file tail $png]"
        incr errori
        continue
    }
    if {[catch {image create photo ::pag -file $png} err]} {
        puts stderr "pagina $nome: [file tail $png] non si legge: $err"
        incr errori
        continue
    }

    #  rettangolo che racchiude il contenuto della pagina, in celle
    set x0 ""; set x1 0; set y0 ""; set y1 0
    foreach s $per_pagina($numero) {
        set px [dict get $s posx]; set py [dict get $s posy]
        set pw [dict get $s larg]; set ph [dict get $s altezza]
        if {$x0 eq "" || $px < $x0} { set x0 $px }
        if {$y0 eq "" || $py < $y0} { set y0 $py }
        if {$px + $pw > $x1} { set x1 [expr {$px + $pw}] }
        if {$py + $ph > $y1} { set y1 [expr {$py + $ph}] }
    }
    set cont_w [expr {($x1 - $x0) * $cella}]
    set cont_h [expr {($y1 - $y0) * $cella}]
    set larg_png [image width ::pag]
    set alt_png  [image height ::pag]

    #  lo scarto deve essere lo stesso sui due assi e non negativo: e' la prova
    #  che questa cattura e' ancora quella di questo r01.dat
    set doppio_x [expr {$larg_png - $cont_w}]
    set doppio_y [expr {$alt_png - $cont_h}]
    if {$doppio_x != $doppio_y || $doppio_x < 0 || $doppio_x % 2 != 0} {
        puts stderr "pagina $nome: la cattura ([set larg_png]x[set alt_png]) non\
                     corrisponde al contenuto del r01.dat ([set cont_w]x[set cont_h]):\
                     scarto $doppio_x x $doppio_y. Ricattura la pagina."
        incr errori
        image delete ::pag
        continue
    }
    set bordo [expr {$doppio_x / 2}]

    set discordi [verifica_occupazione ::pag $per_pagina($numero) $x0 $y1 $bordo $cella]
    if {[llength $discordi]} {
        puts stderr "pagina $nome: la cattura non corrisponde al r01.dat in\
                     [llength $discordi] celle - ricattura la pagina."
        foreach d [lrange $discordi 0 4] { puts stderr "    $d" }
        if {[llength $discordi] > 5} { puts stderr "    ... e altre [expr {[llength $discordi]-5}]" }
        incr errori
        image delete ::pag
        continue
    }

    foreach s $per_pagina($numero) {
        set tipo [dict get $s tipo]
        #  solo il campione, non la stazione TESTO che ne scrive il nome sotto
        if {[dict get $s descrizione] ne $tipo} { continue }
        if {[info exists fatto($tipo)]} {
            puts stderr "tipo $tipo: due campioni (pagine $fatto($tipo) e $nome)"
            incr errori
            continue
        }
        set pw [dict get $s larg]; set ph [dict get $s altezza]
        set sx [expr {$bordo + ([dict get $s posx] - $x0) * $cella}]
        set sy [expr {$bordo + ($y1 - [dict get $s posy] - $ph) * $cella}]
        set sw [expr {$pw * $cella}]
        set sh [expr {$ph * $cella}]

        if {$sx < 0 || $sy < 0 || $sx + $sw > $larg_png || $sy + $sh > $alt_png} {
            puts stderr "tipo $tipo: il ritaglio ($sx,$sy ${sw}x${sh}) esce dalla\
                         cattura di $nome ([set larg_png]x[set alt_png])"
            incr errori
            continue
        }

        set fatto($tipo) $nome
        if {$::solo_prova} {
            puts [format "%-10s %-7s (%2d,%2d) %2dx%-2d celle -> %4d,%-4d %4dx%-4d px" \
                      $tipo $nome [dict get $s posx] [dict get $s posy] $pw $ph $sx $sy $sw $sh]
            continue
        }
        if {![file isdirectory $dir_out]} { file mkdir $dir_out }
        image create photo ::sprite -width $sw -height $sh
        ::sprite copy ::pag -from $sx $sy [expr {$sx + $sw}] [expr {$sy + $sh}] -to 0 0
        ::sprite write [file join $dir_out "$tipo.png"] -format png
        image delete ::sprite
        incr scritti
    }
    image delete ::pag
}

#  ogni tipo del catalogo deve avere il suo sprite: se newstaz.h cresce e il
#  catalogo non e' stato rigenerato, qui si vede subito
set mancanti {}
foreach tipo [lsort [array names ::lgmkstaz::catalogo]] {
    if {![info exists fatto($tipo)]} { lappend mancanti $tipo }
}
if {[llength $mancanti]} {
    puts stderr "senza sprite ([llength $mancanti]): [join $mancanti { }]"
    puts stderr "  rigenera r01.dat con genera_catalogo.py, ricattura le pagine, riprova."
    incr errori
}

if {$::solo_prova} {
    puts "\n[array size fatto] tipi, nessun file scritto (-prova)."
} else {
    puts "[array size ::lgmkstaz::catalogo] tipi nel catalogo, $scritti sprite scritti in [file tail $dir_out]/"
}
if {$errori} {
    puts stderr "\n$errori problemi."
    exit 1
}
exit 0
