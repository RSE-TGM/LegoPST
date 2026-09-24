#!/usr/bin/env wish
#
# cattura_pagine.tcl - rifa' le catture pag_*.png aprendo davvero le pagine
# del catalogo con xstaz e fotografandole.
#
#   wish cattura_pagine.tcl [opzioni]
#       -rtf <variabili.rtf>   quale topologia usare (default: la cerca)
#       -schermo               usa il display vero invece di Xvfb
#       -no-ritaglia           non rifare gli sprite dopo la cattura
#
# Serve quando cambia il DISEGNO delle stazioni (i g*.c di xstaz, i widget di
# AlgLib/libwidget) o la tabella new_staz[]: gli sprite di lgmkstaz sono
# fotografie di quel codice, e senza ricattura restano quelli vecchi senza che
# niente protesti.
#
# Cosa fa, in ordine:
#   1. prepara una directory scratch con il r01.dat del catalogo e un
#      variabili.rtf vero (compstaz ne vuole uno, anche se il catalogo non
#      cita nessuna variabile);
#   2. compila con compstaz VERO, con una chiave SHM/IPC isolata
#      (50000000+pid), lontana dal banco operatore (uid*10000) - compstaz
#      tocca la memoria condivisa in ogni caso;
#   3. apre un display virtuale Xvfb, cosi' non compaiono finestre sullo
#      schermo e la cattura e' ripetibile (con -schermo si usa il display
#      vero, e allora le finestre si vedono);
#   4. per ogni pagina lancia xstaz, chiede la pagina con stazpag, aspetta che
#      la finestra sia della dimensione giusta e la fotografa con "import";
#   5. ritaglia la fotografia al contenuto piu' 5 px per lato, che e'
#      l'inquadratura che ritaglia_sprite.tcl si aspetta;
#   6. rimuove SOLO la chiave IPC isolata (mai killsim, che su Linux
#      cancellerebbe tutte le SHM dell'utente) e cancella lo scratch;
#   7. rilancia ritaglia_sprite.tcl.
#
# Un xstaz PER PAGINA, non uno solo, per due motivi: sul display virtuale non
# c'e' un window manager e le finestre si sovrapporrebbero tutte nello stesso
# punto; e con xstaz appena avviato la pagina e' la prima (ip3=0), quindi cade
# sempre in (0,0) - "x = ip3*20" in xstaz.c. Sapendo dov'e' si fotografa la
# ROOT e si ritaglia il rettangolo noto, senza cercare la finestra per nome:
# "import -window <nome>" che non trova il nome si mette ad aspettare un clic,
# e su un display virtuale quel clic non arriva mai.

package require Tk
wm withdraw .

set ::qui [file dirname [file normalize [info script]]]
set ::opt(rtf) ""
set ::opt(schermo) 0
set ::opt(ritaglia) 1
for {set i 0} {$i < [llength $::argv]} {incr i} {
    switch -- [lindex $::argv $i] {
        -rtf          { incr i; set ::opt(rtf) [lindex $::argv $i] }
        -schermo      { set ::opt(schermo) 1 }
        -no-ritaglia  { set ::opt(ritaglia) 0 }
        default {
            puts stderr "cattura_pagine: opzione sconosciuta [lindex $::argv $i]"
            exit 1
        }
    }
}

proc avviso {testo} { puts $testo ; flush stdout ; update idletasks }

#  Quanto e' grande una pagina, in celle: NON il massimo di posx+larg, ma
#  l'ESTENSIONE del contenuto (massimo meno minimo).
#
#  compstaz normalizza: per ogni pagina trova il minimo posix0/posiy0 e lo
#  sottrae a tutte le stazioni (compstaz.c, "ipx0"/"ipy0"), poi posmx e posmy
#  sono i massimi sulle coordinate GIA' normalizzate. Quindi una pagina le cui
#  stazioni cominciano alla cella 5 viene disegnata da xstaz come se
#  cominciassero a 0: contano solo le posizioni relative fra le stazioni.
#
#  La finestra che xstaz apre e' poi posmx*62+10 per posmy*62+10 (xstaz.c:
#  lform/hform) - contenuto piu' 5 px di margine per lato, che e' esattamente
#  l'inquadratura di cui ha bisogno ritaglia_sprite.tcl.
proc dim_pagina_celle {modello numero} {
    set x0 ""; set y0 ""; set x1 0; set y1 0
    foreach s [dict get $modello stazioni] {
        if {[dict get $s pagina] != $numero} continue
        set px [dict get $s posx]; set py [dict get $s posy]
        if {$x0 eq "" || $px < $x0} { set x0 $px }
        if {$y0 eq "" || $py < $y0} { set y0 $py }
        if {$px + [dict get $s larg] > $x1}    { set x1 [expr {$px + [dict get $s larg]}] }
        if {$py + [dict get $s altezza] > $y1} { set y1 [expr {$py + [dict get $s altezza]}] }
    }
    if {$x0 eq ""} { return [list 0 0] }
    return [list [expr {$x1 - $x0}] [expr {$y1 - $y0}]]
}
proc muori {testo}  { puts stderr "cattura_pagine: $testo" ; flush stderr ; pulisci ; exit 1 }

# --- dove stanno le cose ---------------------------------------------------

proc risali_per {partenza sotto} {
    if {[info exists ::env(LEGOROOT)] && $::env(LEGOROOT) ne ""} {
        set c [file join $::env(LEGOROOT) {*}$sotto]
        if {[file exists $c]} { return $c }
    }
    set dir $partenza
    for {set i 0} {$i < 8} {incr i} {
        set c [file join $dir {*}$sotto]
        if {[file exists $c]} { return $c }
        set su [file dirname $dir]
        if {$su eq $dir} break
        set dir $su
    }
    return ""
}

set ::bin(compstaz) [risali_per $::qui {Alg_rt bin compstaz}]
set ::bin(xstaz)    [risali_per $::qui {Alg_rt bin xstaz}]
set ::bin(stazpag)  [risali_per $::qui {Alg_rt bin stazpag}]
foreach k {compstaz xstaz stazpag} {
    if {$::bin($k) eq "" || ![file executable $::bin($k)]} {
        puts stderr "cattura_pagine: $k non trovato (compila Alg_rt/grafica/xstaz e compstaz)"
        exit 1
    }
}
foreach prog {import Xvfb} {
    if {[auto_execok $prog] eq ""} {
        puts stderr "cattura_pagine: manca '$prog'.\
                     Installa:  sudo dnf install ImageMagick xorg-x11-server-Xvfb"
        exit 1
    }
}

set moduli [risali_per $::qui {Alg_legopc src tix lgmkstaz_dati.tcl}]
if {$moduli eq ""} { set moduli [risali_per $::qui {Alg_legopc bin lgmkstaz_dati.tcl}] }
if {$moduli eq ""} { puts stderr "cattura_pagine: moduli di lgmkstaz non trovati"; exit 1 }
set moduli [file dirname $moduli]
namespace eval ::lgmkstaz {}
set ::lgmkstaz::_qui $moduli
source [file join $moduli lgmkstaz_dati.tcl]
source [file join $moduli lgmkstaz_leggi.tcl]

#  Un variabili.rtf vero: compstaz lo vuole comunque, anche se il catalogo non
#  cita nessuna variabile (tutti i riferimenti sono vuoti).
proc trova_rtf {} {
    if {$::opt(rtf) ne ""} { return $::opt(rtf) }
    set cerca {}
    if {[info exists ::env(KSIM)] && $::env(KSIM) ne ""} {
        lappend cerca [file join $::env(KSIM) variabili.rtf]
        foreach d [glob -nocomplain -directory $::env(KSIM) -type d *] {
            lappend cerca [file join $d variabili.rtf]
        }
    }
    foreach base [list [file join $::env(HOME) sked] [file join $::env(HOME) legocad]] {
        foreach d [glob -nocomplain -directory $base -type d *] {
            lappend cerca [file join $d variabili.rtf]
            foreach d2 [glob -nocomplain -directory $d -type d *] {
                lappend cerca [file join $d2 variabili.rtf]
            }
        }
    }
    foreach f $cerca { if {[file readable $f]} { return $f } }
    return ""
}

# --- pulizia: si chiama da ogni uscita -------------------------------------

set ::stato(xstaz_pid) ""
set ::stato(xvfb_pid)  ""
set ::stato(scratch)   ""
set ::stato(chiave)    [expr {50000000 + [pid]}]

proc ferma_xstaz {} {
    if {$::stato(xstaz_pid) eq ""} return
    catch { exec kill $::stato(xstaz_pid) }
    after 300
    catch { exec kill -9 $::stato(xstaz_pid) }
    set ::stato(xstaz_pid) ""
}

#  Rimuove TUTTO cio' che sta nella fascia di questa esecuzione: la chiave
#  isolata e i 100 numeri che la seguono. Una lista fissa di offset non basta -
#  oltre alla SHM di compstaz (+5) e alla famiglia di code di msg_create_fam,
#  xstaz ne crea altre (vista una a +8), e restavano in giro. La fascia invece
#  e' nostra per costruzione (50000000 + pid, con pid < 100000), quindi
#  spazzarla non puo' toccare ne' il banco dell'utente (uid*10000) ne' la
#  tavola acqua/vapore (999). Mai killsim, che non filtra per chiave.
proc pulisci_fascia {chiave} {
    foreach {opzione elenco} {-M m -Q q -S s} {
        if {[catch {exec ipcs -$elenco} righe]} continue
        foreach riga [split $righe \n] {
            set k [lindex $riga 0]
            if {![string match "0x*" $k]} continue
            if {[catch {expr {$k + 0}} n]} continue
            if {$n >= $chiave && $n <= $chiave + 100} {
                catch { exec ipcrm $opzione $n }
            }
        }
    }
}

proc pulisci {} {
    ferma_xstaz
    if {$::stato(xvfb_pid) ne ""} {
        catch { exec kill $::stato(xvfb_pid) }
        set ::stato(xvfb_pid) ""
    }
    pulisci_fascia $::stato(chiave)
    if {$::stato(scratch) ne "" && [file isdirectory $::stato(scratch)]} {
        catch { file delete -force $::stato(scratch) }
    }
}

# --- 1. scratch + 2. compstaz ----------------------------------------------

set r01 [file join $::qui r01.dat]
if {![file readable $r01]} { muori "$r01 non leggibile" }
if {[catch {::lgmkstaz::leggi_file $r01} modello]} { muori "r01.dat non si legge: $modello" }

set rtf [trova_rtf]
if {$rtf eq ""} {
    muori "nessun variabili.rtf trovato: passalo con -rtf <file>"
}
avviso "topologia: $rtf"

set ::stato(scratch) [file join /tmp "cattura_pagine_[pid]_[clock clicks]"]
file mkdir $::stato(scratch)
file copy $r01 [file join $::stato(scratch) r01.dat]
file copy $rtf [file join $::stato(scratch) variabili.rtf]

avviso "compilo il catalogo con compstaz (chiave isolata $::stato(chiave))..."
set vecchia [pwd]
cd $::stato(scratch)
set rc [catch {exec env -i HOME=$::env(HOME) PATH=/usr/bin:/bin LANG=POSIX \
                   SHR_USR_KEY=$::stato(chiave) $::bin(compstaz) 2>@1} esito]
cd $vecchia
if {[string first "Fine corretta COMPSTAZ" $esito] < 0} {
    puts stderr $esito
    muori "compstaz non ha compilato il catalogo"
}

# --- 3. display ------------------------------------------------------------

if {$::opt(schermo)} {
    if {![info exists ::env(DISPLAY)] || $::env(DISPLAY) eq ""} { muori "DISPLAY non impostato" }
    set ::stato(display) $::env(DISPLAY)
    avviso "uso il display vero $::stato(display): le finestre si vedranno"
} else {
    #  serve piu' largo della pagina piu' grande (posmx*62+10)
    set largo 0; set alto 0
    foreach p [dict get $modello pagine] {
        lassign [dim_pagina_celle $modello [dict get $p numero]] pw ph
        if {$pw*62+10 > $largo} { set largo [expr {$pw*62+10}] }
        if {$ph*62+10 > $alto}  { set alto  [expr {$ph*62+10}] }
    }
    set largo [expr {$largo + 100}]; set alto [expr {$alto + 100}]
    set n 90
    while {$n < 120 && [file exists "/tmp/.X${n}-lock"]} { incr n }
    set ::stato(display) ":$n"
    avviso "display virtuale $::stato(display) ($largo x $alto)"
    set ::stato(xvfb_pid) [exec Xvfb $::stato(display) -screen 0 ${largo}x${alto}x24 \
                               -nolisten tcp >/dev/null 2>@1 &]
    after 800
    if {![file exists "/tmp/.X${n}-lock"]} { muori "Xvfb non e' partito su $::stato(display)" }
}

# --- 4-5. una pagina alla volta --------------------------------------------

proc cattura_pagina {modello numero nome descrizione} {
    lassign [dim_pagina_celle $modello $numero] pw ph
    set fin_w [expr {$pw * 62 + 10}]
    set fin_h [expr {$ph * 62 + 10}]

    ferma_xstaz
    set vecchia [pwd]
    cd $::stato(scratch)
    set ::stato(xstaz_pid) [exec env HOME=$::env(HOME) PATH=/usr/bin:/bin LANG=POSIX \
                                DISPLAY=$::stato(display) SHR_USR_KEY=$::stato(chiave) \
                                $::bin(xstaz) 1 >/dev/null 2>@1 &]
    after 900
    #  stazpag esce 5 finche' xstaz non ha creato la coda. Sotto "timeout"
    #  perche' un exec che non torna bloccherebbe tutto lo script.
    set aperto 0
    for {set i 0} {$i < 20} {incr i} {
        set rc [catch {exec timeout 10 env HOME=$::env(HOME) PATH=/usr/bin:/bin LANG=POSIX \
                           DISPLAY=$::stato(display) SHR_USR_KEY=$::stato(chiave) \
                           $::bin(stazpag) $nome 2>@1} out]
        if {!$rc} { set aperto 1; break }
        after 250
    }
    cd $vecchia
    if {!$aperto} { return [list "" "stazpag non ha aperto '$nome': $out"] }

    #  Si fotografa la root e si ritaglia (0,0)-(fin_w,fin_h): la finestra e'
    #  li' perche' xstaz e' appena partito. Si riprova finche' il contenuto non
    #  ha la dimensione attesa - e' anche il modo di sapere che e' disegnata.
    set tmp [file join $::stato(scratch) "cattura.png"]
    set vista ""
    for {set i 0} {$i < 40} {incr i} {
        after 250
        catch { file delete -force $tmp }
        if {[catch {exec timeout 20 env DISPLAY=$::stato(display) \
                        import -silent -window root $tmp 2>@1} err]} { continue }
        if {![file readable $tmp]} { continue }
        if {[catch {image create photo ::schermo -file $tmp}]} { continue }
        if {[image width ::schermo] < $fin_w || [image height ::schermo] < $fin_h} {
            set vista "schermo troppo piccolo ([image width ::schermo]x[image height ::schermo])"
            image delete ::schermo
            continue
        }
        image create photo ::scatto -width $fin_w -height $fin_h
        ::scatto copy ::schermo -from 0 0 $fin_w $fin_h -to 0 0
        image delete ::schermo
        #  il bordo della finestra dev'essere lo sfondo pagina: se la finestra
        #  non fosse li', qui si vede subito invece di salvare una fotografia
        #  del nulla
        if {[::scatto get 2 2] eq {204 229 229}} { return [list ::scatto ""] }
        set vista "in (2,2) c'e' [::scatto get 2 2], non lo sfondo pagina"
        image delete ::scatto
    }
    set nota ""
    if {$vista ne ""} { set nota " ($vista)" }
    return [list "" "la finestra di '$nome' non e' comparsa in (0,0)$nota"]
}

set errori 0
set fatte 0
foreach p [dict get $modello pagine] {
    set nome [dict get $p nome]
    set descrizione [dict get $p descrizione]
    avviso "pagina $nome ($descrizione)..."
    lassign [cattura_pagina $modello [dict get $p numero] $nome $descrizione] img err
    if {$img eq ""} {
        puts stderr "  $err"
        incr errori
        continue
    }
    set uscita [file join $::qui "pag_$nome.png"]
    $img write $uscita -format png
    avviso "  -> [file tail $uscita]  [image width $img]x[image height $img]"
    image delete $img
    incr fatte
}

ferma_xstaz
pulisci
avviso "$fatte pagine catturate, $errori errori"
if {$errori} { exit 1 }

# --- 7. gli sprite ---------------------------------------------------------

if {$::opt(ritaglia)} {
    avviso "rifaccio gli sprite..."
    set rc [catch {exec [info nameofexecutable] [file join $::qui ritaglia_sprite.tcl] 2>@1} out]
    puts $out
    if {$rc} { exit 1 }
}
exit 0
