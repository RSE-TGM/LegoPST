# lgconn_test.tcl - prove di lgconn_dati.tcl, senza grafica.
#
#   tclsh lgconn_test.tcl
#
# 1. Una task sintetica, fabbricata qui in una directory provvisoria, con un
#    caso per ogni controllo di ::lgconn::verifica: deve trovare esattamente
#    quei problemi.
# 2. Le task di regolazione vere che trova in $HOME/legopst_*/legocad/r_*: per
#    ogni connessione il cui collegamento interno non e' cambiato, la riga che
#    lgconn ricostruisce dalle pagine deve essere quella che config ha scritto
#    in Connessioni.reg, campo per campo, salvo il valore e la descrizione
#    (copie che config non aggiorna, e che nessuno legge).
#
# Esce con 0 se tutto passa, 1 altrimenti.

source [file join [file dirname [file normalize [info script]]] lgconn_dati.tcl]

set ::ok 0
set ::falliti 0
proc esito {nome cond {nota ""}} {
    if {$cond} {
        incr ::ok
        puts "OK       $nome"
    } else {
        incr ::falliti
        puts "FALLITO  $nome $nota"
    }
}

# ---------------------------------------------------------------------------
# 1. task sintetica
# ---------------------------------------------------------------------------

proc scrivi {file testo} {
    set f [open $file w]
    puts -nonewline $f $testo
    close $f
}

#  Un'icona di interfaccia: $w con porta interna ${w}0c, collegata a
#  $porta_blocco (vuoto = non collegata), e porta esterna ${w}1c di tipo $tipo.
proc icona_int {w tipo porta_blocco} {
    return "*$w.iconRegType:\t1
*$w.listChildren:\t${w}0c Port ${w}1c Port
*${w}0c.interfacePort:\t0
*${w}0c.portNameConnect:\t$porta_blocco
*${w}1c.interfacePort:\t1
*${w}1c.tipoPort:\t$tipo
*${w}1c.portNameConnect:\t
"
}
#  Un blocco $w con modulo prov, tag $tag e una porta ${w}0c che si chiama $var.
proc blocco {w tag var {descr ""}} {
    return "*$w.iconRegType:\t0
*$w.modulName:\tprov
*$w.tagName:\t$tag
*$w.iconDescr:\t$descr
*$w.listChildren:\t${w}0c Port
*$w.inputValue:\t|1.5000    | |PRIMA| |2.5000    | |SECONDA|
*${w}0c.portName:\t$var
*${w}0c.interfacePort:\t0
"
}

set d [file join [expr {[info exists ::env(TMPDIR)] ? $::env(TMPDIR) : "/tmp"}] lgconn_test_[pid]]
file mkdir [file join $d lib]
scrivi [file join $d Context.ctx] "*pages:\t./\n*objectLibraries:\tlib\n*page_list:\t\\ PAGU PAGI\n"
scrivi [file join $d lib prov.frm] "NumForms: 1\n1.1.type: input\n1.1.label: U1  --UA--\n1.2.type: input\n1.2.label: U2  --UA--\n"
#  PAGU: uscite. 1w->2w (U2, valore 2.5), 3w digitale->4w, 5w non collegata
#  dentro, 6w->7w
scrivi [file join $d PAGU.pag] "[icona_int 1w 1 2w0c][blocco 2w T001 U2 {un blocco}][icona_int 3w 3 4w0c][blocco 4w T002 U1][icona_int 5w 1 {}][icona_int 6w 1 7w0c][blocco 7w T003 U1]"
#  PAGI: ingressi. 1w<-2w, 3w analogico<-4w, 5w<-6w, 8w, 10w scollegato
scrivi [file join $d PAGI.pag] "[icona_int 1w 0 2w0c][blocco 2w T101 U1][icona_int 3w 0 4w0c][blocco 4w T102 U1][icona_int 5w 0 6w0c][blocco 6w T103 U1][icona_int 8w 0 9w0c][blocco 9w T104 U1][icona_int 10w 0 11w0c][blocco 11w T105 U1]"

set task [::lgconn::carica_task $d]
set p [dict get $task porte]
esito "sintetica: porte di interfaccia trovate" [expr {[dict size $p] == 9}] "([dict size $p])"
esito "sintetica: direzione e tipo" [expr {[dict get $p "PAGU 3w1c" direzione] eq "uscita" &&
     ![dict get $p "PAGU 3w1c" analogica] && [dict get $p "PAGI 3w1c" analogica]}]
esito "sintetica: blocco interno" [expr {[dict get $p "PAGU 1w1c" var] eq "U2" &&
     [dict get $p "PAGU 1w1c" tag] eq "T001" && [dict get $p "PAGU 1w1c" descr] eq "un blocco" &&
     [dict get $p "PAGI 1w1c" descr] eq "-"}]
esito "sintetica: valore dalla form" [expr {[dict get $p "PAGU 1w1c" valore] == 2.5}] \
    "([dict get $p {PAGU 1w1c} valore])"

#  la connessione buona, come la scriverebbe config (moduli scambiati)
set buona [::lgconn::riga_reg [::lgconn::nuova_connessione [dict get $p "PAGU 1w1c"] [dict get $p "PAGI 1w1c"]]]
esito "sintetica: riga come config" [expr {$buona eq "uscita|PAGU|1w1c|1w|2w0c|2w|U2|prov|T001|un blocco|ingresso|PAGI|1w1c|1w|2w0c|2w|U1|prov|T101|-|valore|2.500000"}] $buona

#  il file: 1 buona, 2 tag vecchio, 3 analogico<-digitale, 4 pagina assente,
#  5 porta sparita, 6 icona non collegata dentro, 7 direzione sbagliata,
#  8 porta gia' usata (PAGI 1w1c, come la 1), 9 riga rotta
set righe [list $buona \
    [string map {|T003| |T999|} [::lgconn::riga_reg [::lgconn::nuova_connessione [dict get $p "PAGU 6w1c"] [dict get $p "PAGI 5w1c"]]]] \
    [::lgconn::riga_reg [::lgconn::nuova_connessione [dict get $p "PAGU 3w1c"] [dict get $p "PAGI 3w1c"]]] \
    "uscita|ALTRA|1w1c|1w|2w0c|2w|U1|prov|T001|-|ingresso|PAGI|3w1c|3w|4w0c|4w|U1|prov|T102|-|valore|0.000000" \
    "uscita|PAGU|99w1c|99w|2w0c|2w|U1|prov|T001|-|ingresso|PAGI|8w1c|8w|9w0c|9w|U1|prov|T104|-|valore|0.000000" \
    "uscita|PAGU|5w1c|5w|||||||ingresso|PAGI|5w1c|5w|6w0c|6w|U1|prov|T103|-|valore|0.000000" \
    "uscita|PAGI|5w1c|5w|6w0c|6w|U1|prov|T103|-|ingresso|PAGU|6w1c|6w|7w0c|7w|U1|prov|T003|-|valore|0.000000" \
    "uscita|PAGU|1w1c|1w|2w0c|2w|U2|prov|T001|un blocco|ingresso|PAGI|1w1c|1w|2w0c|2w|U1|prov|T101|-|valore|2.500000" \
    "solo|tre|campi"]
scrivi [file join $d Connessioni.reg] "[join $righe \n]\n"
set task [::lgconn::carica_task $d]
set probl [::lgconn::verifica $task]

proc trovato {probl gravita riga modello} {
    foreach q $probl {
        if {[dict get $q gravita] eq $gravita && [dict get $q riga] == $riga &&
            [string match $modello [dict get $q testo]]} { return 1 }
    }
    return 0
}
esito "controllo: tag vecchio"           [trovato $probl errore 2 "*ha tag 'T003', nel file c'e' 'T999'*"]
esito "controllo: analogico/digitale"    [trovato $probl errore 3 "*porta digitale a una analogica*"]
esito "controllo: pagina assente"        [trovato $probl avviso 4 "*pagina ALTRA*si possono togliere*"]
esito "controllo: porta sparita"         [trovato $probl errore 5 "*non c'e' piu' la porta di interfaccia 99w1c*"]
esito "controllo: non collegata dentro"  [trovato $probl errore 6 "*icona di interfaccia 5w non e' collegata a nessun blocco*"]
esito "controllo: direzione"             [trovato $probl errore 7 "*e' una porta di ingresso, non di uscita*"]
esito "controllo: porta usata due volte" [trovato $probl errore 1 "*PAGI 1w1c compare in 2 connessioni*"]
esito "controllo: riga rotta"            [trovato $probl errore 9 "riga non nel formato atteso"]
esito "controllo: ingresso scollegato"   [trovato $probl nota 0 "*interfaccia 10w (variabile U1 del blocco T105) non e' collegato ad altri schemi*"]
set su_buona 0
foreach q $probl {
    if {[dict get $q riga] == 1 && ![string match "*due volte*" [dict get $q testo]] &&
        ![string match "*compare in 2*" [dict get $q testo]]} { incr su_buona }
}
esito "controllo: la connessione buona non ha altri problemi" [expr {$su_buona == 0}]

# ---------------------------------------------------------------------------
# 1b. modifiche, sulla stessa task
# ---------------------------------------------------------------------------

proc rifiuta {task ku ki modello} {
    if {![catch {::lgconn::collega $task $ku $ki} msg]} { return 0 }
    return [string match $modello $msg]
}
esito "collega: rifiuta un ingresso gia' collegato" [rifiuta $task "PAGU 6w1c" "PAGI 1w1c" "*gia' collegata*"]
esito "collega: rifiuta la direzione sbagliata"     [rifiuta $task "PAGI 10w1c" "PAGU 6w1c" "*porta di ingresso, non di uscita*"]
esito "collega: rifiuta un'icona non collegata dentro" [rifiuta $task "PAGU 5w1c" "PAGI 10w1c" "*non e' collegata a nessun blocco*"]
#  per l'analogico/digitale serve un'uscita digitale libera: 3w e' usata nella
#  riga 3, la si scollega prima
set t2 [::lgconn::scollega $task 2]
esito "scollega: toglie la riga" [expr {[llength [dict get $t2 conns]] == [llength [dict get $task conns]] - 1}]
esito "collega: rifiuta analogico/digitale" [rifiuta $t2 "PAGU 3w1c" "PAGI 10w1c" "*analogica a una digitale*"]
esito "uscite_per: solo uscite libere dello stesso tipo" [expr {[::lgconn::uscite_per $t2 "PAGI 10w1c"] eq {}}] \
    [::lgconn::uscite_per $t2 "PAGI 10w1c"]

#  un collegamento valido: tolgo le righe 7, 2 e 1, che usano 6w e 1w di
#  PAGU, e collego 6w -> 10w
set t3 [::lgconn::scollega [::lgconn::scollega [::lgconn::scollega $task 6] 1] 0]
esito "uscite_per: l'uscita liberata e' disponibile" [expr {"PAGU 6w1c" in [::lgconn::uscite_per $t3 "PAGI 10w1c"]}]
set t3 [::lgconn::collega $t3 "PAGU 6w1c" "PAGI 10w1c"]
set ultima [lindex [dict get $t3 conns] end]
esito "collega: la nuova riga e' come la scriverebbe config" \
    [expr {[::lgconn::riga_reg $ultima] eq "uscita|PAGU|6w1c|6w|7w0c|7w|U1|prov|T003|-|ingresso|PAGI|10w1c|10w|11w0c|11w|U1|prov|T105|-|valore|1.500000"}] \
    [::lgconn::riga_reg $ultima]

lassign [::lgconn::togli_orfane $task] t4 tolte
esito "togli_orfane: toglie solo la riga con la pagina assente" [expr {$tolte == 1 && [llength [dict get $t4 conns]] == [llength [dict get $task conns]] - 1}]
lassign [::lgconn::aggiorna_copie $task] t5 aggiornate
set probl5 [::lgconn::verifica $t5]
esito "aggiorna_copie: corregge il tag invecchiato" [expr {$aggiornate >= 1 && ![trovato $probl5 errore 2 "*T999*"]}] "(aggiornate $aggiornate)"

#  salva: le righe non toccate restano identiche, .bak, rifiuto se cambiato
set prima [::lgconn::contenuto_file [file join $d Connessioni.reg]]
set t6 [::lgconn::salva [::lgconn::collega [::lgconn::scollega [::lgconn::scollega [::lgconn::scollega $task 6] 1] 0] "PAGU 6w1c" "PAGI 10w1c"]]
set dopo [split [::lgconn::contenuto_file [file join $d Connessioni.reg]] \n]
set righe_prima [split $prima \n]
esito "salva: le righe non toccate restano identiche" \
    [expr {[lrange $dopo 0 5] eq [concat [lrange $righe_prima 2 5] [lrange $righe_prima 7 8]]}]
esito "salva: lascia Connessioni.reg.bak com'era" \
    [expr {[::lgconn::contenuto_file [file join $d Connessioni.reg.bak]] eq $prima}]
esito "salva: la task salvata si rilegge uguale" \
    [expr {[llength [dict get [::lgconn::carica_task $d] conns]] == [llength [dict get $t6 conns]]}]
set f [open [file join $d Connessioni.reg] a]
puts $f "uscita|X|1w1c|1w|||||||ingresso|Y|1w1c|1w|||||||valore|0.000000"
close $f
set ok_rifiuto [expr {[catch {::lgconn::salva $t6} msg] && $::errorCode eq "LGCONN_CAMBIATO"}]
esito "salva: rifiuta se un altro programma ha cambiato il file" $ok_rifiuto $msg
file delete -force $d

# ---------------------------------------------------------------------------
# 2. task vere
# ---------------------------------------------------------------------------

set campi {- pagina porta_int icona_int porta icona var modulo tag descr}
set task_trovate 0
foreach dir [lsort [glob -nocomplain -types d [file join $::env(HOME) legopst_* legocad r_*]]] {
    if {![file exists [file join $dir Connessioni.reg]] || ![file exists [file join $dir Context.ctx]]} continue
    if {[catch {::lgconn::carica_task $dir} task]} {
        esito "task vera [file tail [file dirname [file dirname $dir]]]/[file tail $dir]" 0 $task
        continue
    }
    incr task_trovate
    set porte [dict get $task porte]
    set confrontate 0
    set diverse {}
    foreach c [dict get $task conns] {
        if {[dict exists $c errore]} continue
        set ku "[dict get $c uscita pagina] [dict get $c uscita porta_int]"
        set ki "[dict get $c ingresso pagina] [dict get $c ingresso porta_int]"
        if {![dict exists $porte $ku] || ![dict exists $porte $ki]} continue
        #  collegamento interno cambiato dopo la connessione: lo segnala il
        #  controllo, qui non si confronta
        if {[dict get $porte $ku porta] ne [dict get $c uscita porta] ||
            [dict get $porte $ki porta] ne [dict get $c ingresso porta]} continue
        set a [split [::lgconn::riga_reg [::lgconn::nuova_connessione [dict get $porte $ku] [dict get $porte $ki]]] |]
        set b [split [dict get $c testo] |]
        incr confrontate
        for {set j 1} {$j < 20} {incr j} {
            if {$j == 10 || [lindex $campi [expr {$j % 10}]] eq "descr"} continue
            if {[lindex $a $j] ne [lindex $b $j]} {
                lappend diverse "riga [dict get $c riga] [lindex $campi [expr {$j % 10}]]: '[lindex $b $j]' / '[lindex $a $j]'"
            }
        }
    }
    esito "task vera [file tail [file dirname [file dirname $dir]]]/[file tail $dir] ($confrontate connessioni)" \
        [expr {$diverse eq {}}] [join [lrange $diverse 0 2] {; }]
}
if {$task_trovate == 0} { puts "(nessuna task di regolazione con Connessioni.reg in \$HOME/legopst_*: parte 2 saltata)" }

puts ""
puts "== $::ok riusciti, $::falliti falliti =="
exit [expr {$::falliti > 0}]
