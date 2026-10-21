# lgconn_dati.tcl - le connessioni fra gli schemi di una task di regolazione.
#
# Tcl puro, senza Tk: la usano "lgconn -c" (il controllo, anche senza display)
# e l'editor lgconn.
#
# Cosa legge, e dove:
#   Context.ctx          *pages (la directory delle pagine), *page_list,
#                        *objectLibraries (le form dei moduli)
#   <pagina>.pag         database di risorse X: "*<widget>.<risorsa>:\t<valore>"
#   <modulo>.frm         le righe "input" della form, nell'ordine in cui il
#                        blocco ne tiene i valori in "inputValue"
#   Connessioni.reg      una connessione per riga, scritta da config
#                        (Alg_mmi/lib/Ol/OlConn.c, WriteConn)
#
# Il formato di Connessioni.reg e come config ne riempie i campi sono descritti
# in Alg_mmi/README.md, "Il file Connessioni.reg". In breve, per ciascuno dei
# due lati (uscita, ingresso):
#   pagina | porta di interfaccia | icona di interfaccia | porta del blocco
#   interno | blocco interno | variabile | modulo | tag | descrizione
# e in fondo "valore|<valore iniziale della variabile d'uscita>".
#
# Due particolarita' di config che qui si riproducono apposta, perche' le righe
# scritte da lgconn siano uguali alle sue:
#   - il campo "modulo" di un lato contiene il modulo del blocco dell'ALTRO
#     lato (CreoNuovaInterfaccia passa nome_mod_ing al posto di nome_mod_usc e
#     viceversa). Nessuno lo legge;
#   - una descrizione vuota si scrive "-".
# La compilazione (iconinterf.c) usa solo pagina, icona di interfaccia,
# variabile e tag; l'apertura delle pagine (PostOpenPage2) pagina, porta e
# icona di interfaccia.

namespace eval ::lgconn {
    #  campi di una riga, nell'ordine del file
    variable campi_lato {pagina porta_int icona_int porta icona var modulo tag descr}
    #  tipoPort (Alg_mmi/lib/Xl/XlPort.h)
    variable tipo_porta {0 {ingresso analogico} 1 {uscita analogica}
                         2 {ingresso digitale} 3 {uscita digitale}}
}

# ---------------------------------------------------------------------------
# Lettura dei file
# ---------------------------------------------------------------------------

#  Un .pag (o un Context.ctx) come dict: widget -> dict risorsa -> valore.
#  Le risorse globali ("*pages:") stanno sotto il widget "".
#  Le righe che finiscono con "\" continuano sulla successiva.
proc ::lgconn::leggi_risorse {file} {
    set f [open $file r]
    fconfigure $f -encoding iso8859-1
    set testo [read $f]
    close $f
    set testo [string map [list "\\\n" ""] $testo]
    set db [dict create]
    foreach riga [split $testo \n] {
        if {![regexp {^\*([^:]*):\s?(.*)$} $riga -> chiave valore]} continue
        set i [string last . $chiave]
        if {$i < 0} {
            dict set db "" $chiave $valore
        } else {
            dict set db [string range $chiave 0 [expr {$i - 1}]] \
                [string range $chiave [expr {$i + 1}] end] $valore
        }
    }
    return $db
}

proc ::lgconn::risorsa {db widget nome {default ""}} {
    if {[dict exists $db $widget $nome]} { return [dict get $db $widget $nome] }
    return $default
}

#  Le variabili d'ingresso di una form (.frm), nell'ordine: le prime due
#  lettere della label di ogni riga "input" della form 1 (PosTag di
#  OlForm_regol.c cerca con lunghezza 2).
proc ::lgconn::leggi_form {file} {
    set f [open $file r]
    fconfigure $f -encoding iso8859-1
    set righe [split [read $f] \n]
    close $f
    set tipo [dict create]
    set label [dict create]
    foreach r $righe {
        if {[regexp {^1\.(\d+)\.type:\s*(\S+)} $r -> n t]} { dict set tipo $n $t }
        if {[regexp {^1\.(\d+)\.label:\s*(.*)$} $r -> n l]} { dict set label $n $l }
    }
    set vars {}
    foreach n [lsort -integer [dict keys $tipo]] {
        if {[dict get $tipo $n] eq "input" && [dict exists $label $n]} {
            lappend vars [string range [string trimleft [dict get $label $n]] 0 1]
        }
    }
    return $vars
}

#  Il valore di una variabile da "inputValue" del blocco: coppie
#  |valore| |descrizione| nell'ordine delle righe input della form.
proc ::lgconn::valore_da_input {input_value vars var} {
    set pos [lsearch -exact $vars [string range $var 0 1]]
    if {$pos < 0 || [string match "*no value*" $input_value]} { return "" }
    set pezzi {}
    foreach p [split $input_value |] {
        if {[string trim $p] ne ""} { lappend pezzi [string trim $p] }
    }
    set v [lindex $pezzi [expr {2 * $pos}]]
    if {![string is double -strict $v]} { return "" }
    return $v
}

#  Connessioni.reg come lista di dict: {uscita {...} ingresso {...} valore V
#  riga N testo T}. Una riga con un numero di campi sbagliato diventa
#  {errore ... riga N testo T}.
proc ::lgconn::leggi_reg {file} {
    variable campi_lato
    set conns {}
    if {![file exists $file]} { return $conns }
    set f [open $file r]
    fconfigure $f -encoding iso8859-1
    set n 0
    foreach riga [split [read $f] \n] {
        incr n
        if {[string trim $riga] eq ""} continue
        set c [split $riga |]
        if {[llength $c] != 22 || [lindex $c 0] ne "uscita" ||
            [lindex $c 10] ne "ingresso" || [lindex $c 20] ne "valore"} {
            lappend conns [dict create errore "riga non nel formato atteso" riga $n testo $riga]
            continue
        }
        set u [dict create]
        set i [dict create]
        foreach nome $campi_lato k {1 2 3 4 5 6 7 8 9} {
            dict set u $nome [lindex $c $k]
            dict set i $nome [lindex $c [expr {$k + 10}]]
        }
        lappend conns [dict create uscita $u ingresso $i \
                           valore [lindex $c 21] riga $n testo $riga]
    }
    close $f
    return $conns
}

#  Una connessione come riga del file, nel formato di WriteConn ("%f").
proc ::lgconn::riga_reg {conn} {
    variable campi_lato
    set pezzi {uscita}
    foreach nome $campi_lato { lappend pezzi [dict get $conn uscita $nome] }
    lappend pezzi ingresso
    foreach nome $campi_lato { lappend pezzi [dict get $conn ingresso $nome] }
    set v [dict get $conn valore]
    if {![string is double -strict $v]} { set v 0 }
    lappend pezzi valore [format %f $v]
    return [join $pezzi |]
}

# ---------------------------------------------------------------------------
# La task
# ---------------------------------------------------------------------------

#  Carica una task: dict con
#    dir, ctx, dir_pagine, file_reg, pagine (elenco), lib (dir delle form),
#    porte: dict "<pagina> <porta_int>" -> dict della porta di interfaccia
#    conns: lista di Connessioni.reg
#  Una porta di interfaccia (vedi porta_interfaccia) e' il "lato" di una
#  connessione possibile, gia' con tutti i campi che config scriverebbe.
proc ::lgconn::carica_task {dir} {
    set ctx_file [file join $dir Context.ctx]
    if {![file exists $ctx_file]} {
        error "in $dir non c'e' Context.ctx: non e' la directory di una task di regolazione"
    }
    set ctx [leggi_risorse $ctx_file]
    set dir_pagine [file normalize [file join $dir [risorsa $ctx "" pages ./]]]
    set lib [risorsa $ctx "" objectLibraries ""]
    if {$lib ne ""} { set lib [file normalize [file join $dir $lib]] }
    set pagine [string trim [risorsa $ctx "" page_list ""] " \\\t"]
    set pagine [regexp -all -inline {\S+} $pagine]

    set task [dict create dir [file normalize $dir] dir_pagine $dir_pagine \
                  file_reg [file join $dir_pagine Connessioni.reg] \
                  pagine $pagine lib $lib porte {} problemi_pagine {}]
    set forms [dict create]
    foreach pag $pagine {
        set f [file join $dir_pagine $pag.pag]
        if {![file exists $f]} {
            dict lappend task problemi_pagine [list $pag "manca $pag.pag"]
            continue
        }
        set db [leggi_risorse $f]
        foreach w [dict keys $db] {
            if {[risorsa $db $w iconRegType 0] in {"" 0}} continue
            foreach p [porta_interfaccia $db $w $pag $lib forms] {
                dict set task porte "$pag [dict get $p porta_int]" $p
            }
        }
    }
    dict set task conns [leggi_reg [dict get $task file_reg]]
    dict set task letto [contenuto_file [dict get $task file_reg]]
    dict set task salvato 0
    return $task
}

#  Le porte di interfaccia di un'icona di interfaccia $w della pagina $pag
#  (di solito una). Per ognuna, i campi del suo lato di connessione:
#    pagina porta_int icona_int  - la porta esterna e la sua icona
#    direzione (uscita/ingresso) analogica (1/0)
#    collegata_dentro (1/0)      - la porta interna e' collegata a un blocco?
#    porta icona var modulo tag descr valore  - il blocco interno (se c'e')
#    porta_connessa              - portNameConnect della porta esterna, la
#                                  copia che config tiene nel .pag
proc ::lgconn::porta_interfaccia {db w pag lib forms_var} {
    upvar $forms_var forms
    set figli {}
    foreach {nome classe} [risorsa $db $w listChildren ""] {
        if {$classe eq "Port"} { lappend figli $nome }
    }
    set esterne {}
    set interna ""
    foreach p $figli {
        if {[risorsa $db $p interfacePort 0] == 1} { lappend esterne $p } else { set interna $p }
    }
    set fuori {}
    foreach p $esterne {
        set tipo [risorsa $db $p tipoPort ""]
        set d [dict create pagina $pag porta_int $p icona_int $w \
                   direzione [expr {$tipo in {1 3} ? "uscita" : "ingresso"}] \
                   analogica [expr {$tipo in {0 1}}] \
                   porta_connessa [risorsa $db $p portNameConnect ""] \
                   collegata_dentro 0 porta "" icona "" var "" modulo "" \
                   tag "" descr "-" valore ""]
        set colleg ""
        if {$interna ne ""} { set colleg [lindex [risorsa $db $interna portNameConnect ""] 0] }
        if {$colleg ne "" && [regexp {^(\d+w)} $colleg -> blocco] && [dict exists $db $blocco]} {
            set mod [risorsa $db $blocco modulName ""]
            set descr [risorsa $db $blocco iconDescr ""]
            if {[string trim $descr] eq ""} { set descr "-" }
            dict set d collegata_dentro 1
            dict set d porta $colleg
            dict set d icona $blocco
            dict set d var [risorsa $db $colleg portName ""]
            dict set d modulo $mod
            dict set d tag [risorsa $db $blocco tagName ""]
            dict set d descr $descr
            #  il valore, dalla form del modulo
            if {$lib ne "" && $mod ne ""} {
                if {![dict exists $forms $mod]} {
                    set ff [file join $lib $mod.frm]
                    dict set forms $mod [expr {[file exists $ff] ? [leggi_form $ff] : {}}]
                }
                dict set d valore [valore_da_input [risorsa $db $blocco inputValue ""] \
                                       [dict get $forms $mod] [dict get $d var]]
            }
        }
        lappend fuori $d
    }
    return $fuori
}

#  La connessione che config scriverebbe fra la porta d'uscita $u e quella
#  d'ingresso $i (dict di porta_interfaccia), compreso lo scambio dei moduli.
proc ::lgconn::nuova_connessione {u i} {
    variable campi_lato
    set lu [dict create]
    set li [dict create]
    foreach nome $campi_lato {
        dict set lu $nome [dict get $u $nome]
        dict set li $nome [dict get $i $nome]
    }
    dict set lu modulo [dict get $i modulo]
    dict set li modulo [dict get $u modulo]
    #  riga 0: non ancora nel file (salva la numera)
    return [dict create uscita $lu ingresso $li valore [dict get $u valore] riga 0]
}

# ---------------------------------------------------------------------------
# Controllo
# ---------------------------------------------------------------------------

#  I problemi della task, come lista di dict {gravita errore|avviso|nota
#  testo T riga N}. riga e' quella di Connessioni.reg, 0 se non c'entra.
proc ::lgconn::verifica {task} {
    variable campi_lato
    set probl {}
    set porte [dict get $task porte]
    set pagine [dict get $task pagine]
    foreach pp [dict get $task problemi_pagine] {
        lassign $pp pag testo
        lappend probl [dict create gravita avviso riga 0 testo "pagina $pag: $testo"]
    }

    set usate [dict create]     ;# "<pagina> <porta>" -> righe
    set assenti [dict create]   ;# pagina non nella task -> righe
    foreach c [dict get $task conns] {
        set n [dict get $c riga]
        if {[dict exists $c errore]} {
            lappend probl [dict create gravita errore riga $n testo [dict get $c errore]]
            continue
        }
        set descr_conn "[dict get $c uscita pagina]/[dict get $c uscita var] -> [dict get $c ingresso pagina]/[dict get $c ingresso var]"
        set rotta 0
        foreach lato {uscita ingresso} {
            set l [dict get $c $lato]
            set pag [dict get $l pagina]
            set chiave "$pag [dict get $l porta_int]"
            dict lappend usate $chiave $n
            if {$pag ni $pagine} {
                #  pagine non piu' nella task: config non le compila, quindi
                #  la connessione non fa danni, ma e' rimasta li'
                dict lappend assenti $pag $n
                set rotta 1
                continue
            }
            if {![dict exists $porte $chiave]} {
                lappend probl [dict create gravita errore riga $n \
                    testo "$descr_conn: in $pag non c'e' piu' la porta di interfaccia [dict get $l porta_int] (icona [dict get $l icona_int])"]
                set rotta 1
                continue
            }
            set p [dict get $porte $chiave]
            if {[dict get $p direzione] ne $lato} {
                lappend probl [dict create gravita errore riga $n \
                    testo "$descr_conn: la porta [dict get $l porta_int] di $pag e' una porta di [dict get $p direzione], non di $lato"]
                set rotta 1
                continue
            }
            if {![dict get $p collegata_dentro]} {
                lappend probl [dict create gravita errore riga $n \
                    testo "$descr_conn: in $pag l'icona di interfaccia [dict get $l icona_int] non e' collegata a nessun blocco"]
                set rotta 1
                continue
            }
            #  i dati che la compilazione usa: variabile e tag del blocco
            foreach campo {var tag} {
                if {[dict get $p $campo] ne [dict get $l $campo]} {
                    lappend probl [dict create gravita errore riga $n \
                        testo "$descr_conn: in $pag il blocco collegato all'icona [dict get $l icona_int] ha $campo '[dict get $p $campo]', nel file c'e' '[dict get $l $campo]' (la compilazione userebbe il valore vecchio)"]
                }
            }
            foreach campo {porta icona descr} {
                if {[dict get $p $campo] ne [dict get $l $campo]} {
                    lappend probl [dict create gravita avviso riga $n \
                        testo "$descr_conn: in $pag l'icona [dict get $l icona_int] e' ora collegata a $campo '[dict get $p $campo]', nel file c'e' '[dict get $l $campo]'"]
                }
            }
        }
        if {!$rotta} {
            set pu [dict get $porte "[dict get $c uscita pagina] [dict get $c uscita porta_int]"]
            set pi [dict get $porte "[dict get $c ingresso pagina] [dict get $c ingresso porta_int]"]
            if {[dict get $pu analogica] != [dict get $pi analogica]} {
                lappend probl [dict create gravita errore riga $n \
                    testo "$descr_conn: collega una porta [expr {[dict get $pu analogica] ? "analogica" : "digitale"}] a una [expr {[dict get $pi analogica] ? "analogica" : "digitale"}]"]
            }
        }
    }

    dict for {pag righe} $assenti {
        set righe [lsort -integer -unique $righe]
        lappend probl [dict create gravita avviso riga [lindex $righe 0] \
            testo "[llength $righe] connessioni con la pagina $pag, che non e' fra le pagine della task (Context.ctx): non servono a niente, si possono togliere (righe [comprimi_righe $righe])"]
    }
    #  una porta di interfaccia in piu' connessioni
    dict for {chiave righe} $usate {
        if {[llength $righe] > 1} {
            lappend probl [dict create gravita errore riga [lindex $righe 0] \
                testo "la porta di interfaccia $chiave compare in [llength $righe] connessioni (righe [join $righe {, }])"]
        }
    }
    #  ingressi di interfaccia scollegati
    dict for {chiave p} $porte {
        if {[dict get $p direzione] eq "ingresso" && ![dict exists $usate $chiave]} {
            set cosa [expr {[dict get $p collegata_dentro] ? "variabile [dict get $p var] del blocco [dict get $p tag]" : "non collegato a nessun blocco"}]
            #  non e' per forza un errore: la variabile resta un ingresso
            #  della task, e puo' arrivare da un'altra task (S01, kConnex)
            lappend probl [dict create gravita nota riga 0 \
                testo "pagina [dict get $p pagina]: l'ingresso di interfaccia [dict get $p icona_int] ($cosa) non e' collegato ad altri schemi"]
        }
    }
    return $probl
}

#  "1 2 3 5 7 8" -> "1-3, 5, 7-8"
proc ::lgconn::comprimi_righe {righe} {
    set pezzi {}
    set inizio ""
    set prec ""
    foreach r [concat $righe [list ""]] {
        if {$inizio ne "" && ($r eq "" || $r != $prec + 1)} {
            lappend pezzi [expr {$inizio == $prec ? $inizio : "$inizio-$prec"}]
            set inizio ""
        }
        if {$r eq ""} break
        if {$inizio eq ""} { set inizio $r }
        set prec $r
    }
    return [join $pezzi {, }]
}

# ---------------------------------------------------------------------------
# Modifiche (le usa l'editor)
#
# Ogni procedura prende la task e restituisce la task modificata, oppure dà
# errore con un messaggio per l'utente. Le connessioni lette dal file tengono
# il loro testo ("testo"): finché non si toccano si riscrivono identiche, byte
# per byte. Quelle nuove o aggiornate non hanno "testo" e si scrivono con
# riga_reg.
# ---------------------------------------------------------------------------

#  La chiave "<pagina> <porta_int>" di un lato di una connessione.
proc ::lgconn::chiave_lato {c lato} {
    return "[dict get $c $lato pagina] [dict get $c $lato porta_int]"
}

#  L'indice della connessione che usa la porta $chiave, -1 se nessuna.
proc ::lgconn::connessione_di {task chiave} {
    set i -1
    foreach c [dict get $task conns] {
        incr i
        if {[dict exists $c errore]} continue
        if {[chiave_lato $c uscita] eq $chiave || [chiave_lato $c ingresso] eq $chiave} { return $i }
    }
    return -1
}

#  Le porte d'uscita a cui si puo' collegare l'ingresso $ki: collegate a un
#  blocco dentro il loro schema, dello stesso tipo (analogico/digitale), libere.
proc ::lgconn::uscite_per {task ki} {
    set porte [dict get $task porte]
    set analogica [dict get $porte $ki analogica]
    set fuori {}
    dict for {k p} $porte {
        if {[dict get $p direzione] ne "uscita" || ![dict get $p collegata_dentro]} continue
        if {[dict get $p analogica] != $analogica} continue
        if {[connessione_di $task $k] >= 0} continue
        lappend fuori $k
    }
    return $fuori
}

#  Collega l'uscita $ku all'ingresso $ki (chiavi "<pagina> <porta_int>").
proc ::lgconn::collega {task ku ki} {
    set porte [dict get $task porte]
    foreach k [list $ku $ki] lato {uscita ingresso} {
        if {![dict exists $porte $k]} { error "la porta $k non esiste" }
        set p [dict get $porte $k]
        if {[dict get $p direzione] ne $lato} {
            error "[dict get $p pagina]/[dict get $p icona_int] e' una porta di [dict get $p direzione], non di $lato"
        }
        if {![dict get $p collegata_dentro]} {
            error "in [dict get $p pagina] l'icona di interfaccia [dict get $p icona_int] non e' collegata a nessun blocco: collegala prima dentro lo schema, con config"
        }
        if {[connessione_di $task $k] >= 0} {
            error "[dict get $p pagina]/[dict get $p icona_int] e' gia' collegata: scollegala prima"
        }
    }
    if {[dict get $porte $ku analogica] != [dict get $porte $ki analogica]} {
        error "non si collega una porta analogica a una digitale"
    }
    dict lappend task conns [nuova_connessione [dict get $porte $ku] [dict get $porte $ki]]
    return $task
}

#  Toglie la connessione di indice $i.
proc ::lgconn::scollega {task i} {
    dict set task conns [lreplace [dict get $task conns] $i $i]
    return $task
}

#  Toglie le connessioni con pagine che non sono nella task. Restituisce
#  {task numero_tolte}.
proc ::lgconn::togli_orfane {task} {
    set pagine [dict get $task pagine]
    set tenute {}
    set tolte 0
    foreach c [dict get $task conns] {
        if {![dict exists $c errore] &&
            ([dict get $c uscita pagina] ni $pagine || [dict get $c ingresso pagina] ni $pagine)} {
            incr tolte
            continue
        }
        lappend tenute $c
    }
    dict set task conns $tenute
    return [list $task $tolte]
}

#  Rifa' dalle pagine i dati copiati (variabile, tag, blocco, descrizione,
#  modulo, valore) delle connessioni le cui due porte esistono e sono
#  collegate dentro lo schema: e' quello che config non fa mai. Restituisce
#  {task numero_aggiornate}.
proc ::lgconn::aggiorna_copie {task} {
    set porte [dict get $task porte]
    set nuove {}
    set n 0
    foreach c [dict get $task conns] {
        if {![dict exists $c errore]} {
            set ku [chiave_lato $c uscita]
            set ki [chiave_lato $c ingresso]
            if {[dict exists $porte $ku] && [dict exists $porte $ki] &&
                [dict get $porte $ku collegata_dentro] && [dict get $porte $ki collegata_dentro]} {
                set c2 [nuova_connessione [dict get $porte $ku] [dict get $porte $ki]]
                if {[riga_reg $c2] ne [testo_di $c]} {
                    dict set c2 riga [dict get $c riga]
                    set c $c2
                    incr n
                }
            }
        }
        lappend nuove $c
    }
    dict set task conns $nuove
    return [list $task $n]
}

#  Il testo con cui si scrive una connessione: quello letto, se non e' stata
#  toccata.
proc ::lgconn::testo_di {c} {
    if {[dict exists $c testo]} { return [dict get $c testo] }
    return [riga_reg $c]
}

#  Il contenuto di Connessioni.reg com'era quando la task e' stata caricata
#  (o salvata l'ultima volta): serve a salva per accorgersi se qualcun altro
#  lo ha cambiato nel frattempo.
proc ::lgconn::contenuto_file {file} {
    if {![file exists $file]} { return "" }
    set f [open $file r]
    fconfigure $f -encoding iso8859-1 -translation binary
    set t [read $f]
    close $f
    return $t
}

#  Salva Connessioni.reg. Se il file su disco non e' piu' quello letto (lo ha
#  cambiato config o un altro lgconn) non lo sovrascrive: da' errore con
#  codice LGCONN_CAMBIATO, e chi chiama decide. La prima volta che salva in
#  una sessione lascia la copia del file com'era in Connessioni.reg.bak.
#  Scrive in un file provvisorio e poi lo rinomina: un'interruzione a meta'
#  non lascia un file troncato.
proc ::lgconn::salva {task} {
    set file [dict get $task file_reg]
    if {[contenuto_file $file] ne [dict get $task letto]} {
        return -code error -errorcode LGCONN_CAMBIATO \
            "$file e' stato cambiato da un altro programma (config?) dopo che lgconn l'ha letto"
    }
    if {![dict get $task salvato] && [file exists $file]} {
        file copy -force $file $file.bak
    }
    set testo ""
    foreach c [dict get $task conns] { append testo [testo_di $c] \n }
    set tmp $file.lgconn_[pid]
    set f [open $tmp w]
    fconfigure $f -encoding iso8859-1 -translation lf
    puts -nonewline $f $testo
    close $f
    file rename -force $tmp $file
    #  da qui in poi le connessioni sono quelle del file
    set conns {}
    set n 0
    foreach c [dict get $task conns] {
        incr n
        dict set c testo [testo_di $c]
        dict set c riga $n
        lappend conns $c
    }
    dict set task conns $conns
    dict set task letto [contenuto_file $file]
    dict set task salvato 1
    return $task
}
