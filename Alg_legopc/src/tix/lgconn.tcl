# lgconn.tcl - le connessioni fra gli schemi di una task di regolazione.
#
#   lgconn [DIR]      l'editor: collegare, scollegare, aggiornare i dati
#                     copiati, togliere le connessioni orfane, salvare (vedi
#                     "L'editor" piu' sotto)
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
#  openhelp.tcl apre i documenti (.md -> HTML) nel browser, per il menu "?";
#  senza, la guida rapida funziona lo stesso e le altre voci lo dicono.
catch {source [file join $::lgconn::_qui openhelp.tcl]}

#  Il rapporto del controllo come testo, e il numero di errori.
proc ::lgconn::testo_rapporto {task {note 0}} {
    set probl [verifica $task]
    set n [dict create errore 0 avviso 0 nota 0]
    foreach p $probl { dict incr n [dict get $p gravita] }
    set n_conn 0
    foreach c [dict get $task conns] { if {![dict exists $c errore]} { incr n_conn } }
    set t ""
    append t "task:          [dict get $task dir]\n"
    append t "connessioni:   $n_conn in [dict get $task file_reg]\n"
    append t "pagine:        [llength [dict get $task pagine]], porte di interfaccia [dict size [dict get $task porte]]\n\n"
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
        append t "$titolo ([llength $righe])\n"
        foreach r $righe { append t "  $r\n" }
        append t "\n"
    }
    append t "esito: [dict get $n errore] errori, [dict get $n avviso] avvisi, [dict get $n nota] note[expr {$note || ![dict get $n nota] ? "" : " (con -v le note sono elencate)"}]\n"
    return [list $t [dict get $n errore]]
}

#  Il rapporto del controllo, sullo standard output. Ritorna il codice di
#  uscita.
proc ::lgconn::rapporto {dir {note 0}} {
    if {[catch {carica_task $dir} task]} {
        puts stderr "lgconn: $task"
        return 2
    }
    lassign [testo_rapporto $task $note] t n_err
    puts -nonewline $t
    return [expr {$n_err > 0 ? 1 : 0}]
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

# ===========================================================================
# L'editor
#
#   lgconn [DIR]
#
# In alto gli ingressi di interfaccia di tutti gli schemi della task, ciascuno
# con l'uscita a cui e' collegato e lo stato; in basso le uscite che si
# possono collegare all'ingresso scelto (libere, collegate dentro il loro
# schema, dello stesso tipo). Le connessioni verso pagine o porte che non ci
# sono piu' compaiono anch'esse in alto, in rosso, per poterle togliere.
# Niente si scrive finche' non si salva.
# ===========================================================================

package require Tk

namespace eval ::lgconn::gui {
    variable task {}
    variable modificato 0
    variable filtro ""
    variable filtro_uscite ""
    variable solo_liberi 0
    variable solo_problemi 0
    variable stato ""
    variable righe {}        ;# id riga della tabella -> dict
    variable righe_uscite {} ;# id riga della tabella delle uscite -> chiave della porta
}

#  Le righe della tabella degli ingressi: una per porta d'ingresso di
#  interfaccia, piu' una per ogni connessione il cui ingresso non e' una porta
#  della task (pagina o porta sparite). Ogni riga: chiave (porta d'ingresso,
#  vuota per le orfane), conn (indice in conns, -1 se libera), i campi da
#  mostrare e lo stato.
proc ::lgconn::gui::calcola_righe {} {
    variable task
    set porte [dict get $task porte]
    #  i problemi, per riga del file
    set probl_riga [dict create]
    foreach p [::lgconn::verifica $task] {
        set r [dict get $p riga]
        #  l'avviso sulle pagine assenti e' un riassunto, con il numero della
        #  sola prima riga: lo stato di quelle connessioni si fa qui sotto
        if {$r > 0 && [dict get $p gravita] ne "nota" &&
            ![string match "*si possono togliere*" [dict get $p testo]]} {
            dict lappend probl_riga $r [list [dict get $p gravita] [dict get $p testo]]
        }
    }
    set fuori {}
    set usati [dict create]
    set i -1
    foreach c [dict get $task conns] {
        incr i
        if {[dict exists $c errore]} {
            lappend fuori [dict create chiave "" conn $i ingresso {} uscita {} orfana 1 \
                gravita errore stato "riga [dict get $c riga]: [dict get $c errore]"]
            continue
        }
        set ki [::lgconn::chiave_lato $c ingresso]
        set gravita ok
        set stato "collegato"
        set orfana 0
        set r [dict get $c riga]
        set pagine [dict get $task pagine]
        foreach lato {uscita ingresso} {
            set pag [dict get $c $lato pagina]
            if {$pag ni $pagine} {
                set orfana 1
                set gravita avviso
                set stato "la pagina $pag non e' fra le pagine della task: connessione inutile (Modifica -> Togli le connessioni verso pagine assenti)"
            }
        }
        if {!$orfana && $r > 0 && [dict exists $probl_riga $r]} {
            set primo [lindex [dict get $probl_riga $r] 0]
            set gravita [lindex $primo 0]
            foreach q [dict get $probl_riga $r] { if {[lindex $q 0] eq "errore"} { set primo $q; set gravita errore; break } }
            set stato [lindex $primo 1]
        }
        if {[dict exists $porte $ki] && [dict get $porte $ki direzione] eq "ingresso" &&
            ![dict exists $usati $ki]} {
            dict set usati $ki 1
            set ing [dict get $porte $ki]
        } else {
            set ki ""
            set ing [dict get $c ingresso]
        }
        lappend fuori [dict create chiave $ki conn $i ingresso $ing \
            uscita [dict get $c uscita] gravita $gravita stato $stato orfana $orfana]
    }
    dict for {k p} $porte {
        if {[dict get $p direzione] ne "ingresso" || [dict exists $usati $k]} continue
        set st [expr {[dict get $p collegata_dentro] ? "libero" : "libero, e non collegato a nessun blocco dentro lo schema"}]
        lappend fuori [dict create chiave $k conn -1 ingresso $p uscita {} gravita libero stato $st orfana 0]
    }
    return [lsort -command ::lgconn::gui::ordine $fuori]
}

#  Prima le pagine della task, poi le connessioni verso pagine che non ci
#  sono piu'; dentro, per pagina, blocco e icona.
proc ::lgconn::gui::ordine {a b} {
    set oa [dict get $a orfana]
    set ob [dict get $b orfana]
    if {$oa != $ob} { return [expr {$oa - $ob}] }
    set ka [list [campo [dict get $a ingresso] pagina] [campo [dict get $a ingresso] tag] [campo [dict get $a ingresso] icona_int]]
    set kb [list [campo [dict get $b ingresso] pagina] [campo [dict get $b ingresso] tag] [campo [dict get $b ingresso] icona_int]]
    return [string compare $ka $kb]
}

proc ::lgconn::gui::campo {d nome} {
    if {$d eq {} || ![dict exists $d $nome]} { return "" }
    return [dict get $d $nome]
}

#  Ridisegna la tabella degli ingressi, mantenendo la selezione.
proc ::lgconn::gui::riempi {} {
    variable righe
    variable filtro
    variable solo_liberi
    variable solo_problemi
    set t .corpo.ingressi.t
    set sel [chiave_selezionata]
    $t delete [$t children {}]
    set righe [dict create]
    set n_tot 0
    set n_coll 0
    set n_err 0
    set n_avv 0
    set da_selezionare ""
    foreach r [calcola_righe] {
        set g [dict get $r gravita]
        incr n_tot
        if {[dict get $r conn] >= 0} { incr n_coll }
        if {$g eq "errore"} { incr n_err }
        if {$g eq "avviso"} { incr n_avv }
        if {$solo_liberi && $g ne "libero"} continue
        if {$solo_problemi && $g ni {errore avviso}} continue
        set i [dict get $r ingresso]
        set u [dict get $r uscita]
        set valori [list [campo $i pagina] [campo $i icona_int] [campo $i var] [campo $i tag] \
            [campo $i descr] [campo $u pagina] [campo $u var] [campo $u tag] [dict get $r stato]]
        if {$filtro ne "" && ![string match -nocase "*$filtro*" [join $valori " "]]} continue
        set id [$t insert {} end -values $valori -tags [list $g]]
        dict set righe $id $r
        if {[identita $r] eq $sel} { set da_selezionare $id }
    }
    if {$da_selezionare ne ""} {
        $t selection set [list $da_selezionare]
        #  dopo il ridisegno, o "see" la lascia a meta' sul bordo
        after idle [list $t see $da_selezionare]
    }
    variable stato
    set stato "$n_tot ingressi, $n_coll collegati, $n_err con errori, $n_avv con avvisi[expr {[set ::lgconn::gui::modificato] ? "  -  MODIFICATO, non salvato" : ""}]"
    riempi_uscite
}

#  Un'identita' stabile di una riga, per ritrovarla dopo un ridisegno.
proc ::lgconn::gui::identita {r} {
    if {[dict get $r chiave] ne ""} { return "p [dict get $r chiave]" }
    return "c [dict get $r conn]"
}

proc ::lgconn::gui::riga_selezionata {} {
    variable righe
    set id [lindex [.corpo.ingressi.t selection] 0]
    if {$id eq "" || ![dict exists $righe $id]} { return {} }
    return [dict get $righe $id]
}

proc ::lgconn::gui::chiave_selezionata {} {
    set r [riga_selezionata]
    if {$r eq {}} { return "" }
    return [identita $r]
}

#  La tabella in basso: le uscite collegabili all'ingresso selezionato.
proc ::lgconn::gui::riempi_uscite {} {
    variable task
    variable filtro_uscite
    variable righe_uscite
    set t .corpo.uscite.t
    $t delete [$t children {}]
    set righe_uscite [dict create]
    set r [riga_selezionata]
    set titolo "Uscite collegabili: seleziona un ingresso libero nella tabella in alto"
    if {$r ne {}} {
        set i [dict get $r ingresso]
        if {[dict get $r conn] >= 0} {
            set titolo "[campo $i pagina]/[campo $i var] ([campo $i tag]) e' gia' collegato: Scollega per cambiarlo"
        } elseif {[dict get $r chiave] ne "" && [dict get $i collegata_dentro]} {
            set titolo "Uscite collegabili a [campo $i pagina]/[campo $i var] ([campo $i tag]) - doppio clic o Collega"
            set porte [dict get $task porte]
            set ordinate {}
            foreach k [::lgconn::uscite_per $task [dict get $r chiave]] {
                lappend ordinate [list [dict get $porte $k pagina] [dict get $porte $k tag] $k]
            }
            foreach o [lsort -index 0 [lsort -index 1 $ordinate]] {
                set k [lindex $o 2]
                set p [dict get $porte $k]
                set valori [list [dict get $p pagina] [dict get $p icona_int] [dict get $p var] \
                    [dict get $p tag] [dict get $p descr] [expr {[dict get $p analogica] ? "analogica" : "digitale"}]]
                if {$filtro_uscite ne "" && ![string match -nocase "*$filtro_uscite*" [join $valori " "]]} continue
                dict set righe_uscite [$t insert {} end -values $valori] $k
            }
        } elseif {[dict get $r chiave] ne ""} {
            set titolo "L'icona [campo $i icona_int] di [campo $i pagina] non e' collegata a nessun blocco: collegala prima dentro lo schema, con config"
        }
    }
    .corpo.uscite.titolo configure -text $titolo
    aggiorna_bottoni
}

#  La chiave della porta d'uscita selezionata in basso, "" se nessuna.
proc ::lgconn::gui::uscita_selezionata {} {
    variable righe_uscite
    set id [lindex [.corpo.uscite.t selection] 0]
    if {$id eq "" || ![dict exists $righe_uscite $id]} { return "" }
    return [dict get $righe_uscite $id]
}

proc ::lgconn::gui::aggiorna_bottoni {} {
    set r [riga_selezionata]
    set puo_scollegare [expr {$r ne {} && [dict get $r conn] >= 0}]
    set puo_collegare [expr {$r ne {} && [dict get $r conn] < 0 &&
        [uscita_selezionata] ne ""}]
    .corpo.bottoni.collega configure -state [expr {$puo_collegare ? "normal" : "disabled"}]
    .corpo.bottoni.scollega configure -state [expr {$puo_scollegare ? "normal" : "disabled"}]
    .mb.modifica entryconfigure "Collega" -state [expr {$puo_collegare ? "normal" : "disabled"}]
    .mb.modifica entryconfigure "Scollega" -state [expr {$puo_scollegare ? "normal" : "disabled"}]
}

proc ::lgconn::gui::messaggio {testo} {
    variable stato
    set stato $testo
}

proc ::lgconn::gui::cambiato {testo} {
    variable modificato
    set modificato 1
    riempi
    messaggio "$testo  -  MODIFICATO, non salvato"
}

proc ::lgconn::gui::collega {} {
    variable task
    set r [riga_selezionata]
    set ku [uscita_selezionata]
    if {$r eq {} || $ku eq "" || [dict get $r conn] >= 0} return
    if {[catch {::lgconn::collega $task $ku [dict get $r chiave]} nuovo]} {
        tk_messageBox -parent . -icon warning -title "Collega" -message $nuovo
        return
    }
    set task $nuovo
    set u [dict get $task porte $ku]
    cambiato "Collegato [dict get $u pagina]/[dict get $u var] -> [campo [dict get $r ingresso] pagina]/[campo [dict get $r ingresso] var]"
}

proc ::lgconn::gui::scollega {} {
    variable task
    set r [riga_selezionata]
    if {$r eq {} || [dict get $r conn] < 0} return
    set task [::lgconn::scollega $task [dict get $r conn]]
    cambiato "Scollegato [campo [dict get $r uscita] pagina]/[campo [dict get $r uscita] var] -> [campo [dict get $r ingresso] pagina]/[campo [dict get $r ingresso] var]"
}

proc ::lgconn::gui::togli_orfane {} {
    variable task
    lassign [::lgconn::togli_orfane $task] task n
    if {$n == 0} { messaggio "Nessuna connessione verso pagine che non sono nella task."; return }
    cambiato "Tolte $n connessioni verso pagine che non sono nella task"
}

proc ::lgconn::gui::aggiorna_copie {} {
    variable task
    lassign [::lgconn::aggiorna_copie $task] task n
    if {$n == 0} { messaggio "I dati copiati in Connessioni.reg corrispondono gia' alle pagine."; return }
    cambiato "Aggiornate $n connessioni con i dati attuali delle pagine (variabile, tag, descrizione, valore)"
}

proc ::lgconn::gui::salva {} {
    variable task
    variable modificato
    if {[catch {::lgconn::salva $task} nuovo]} {
        if {$::errorCode ne "LGCONN_CAMBIATO"} {
            tk_messageBox -parent . -icon error -title "Salva" -message "Non riesco a salvare: $nuovo"
            return 0
        }
        set alt "[dict get $task file_reg].lgconn"
        set risp [tk_messageBox -parent . -icon warning -type yesno -title "Salva" -message \
            "Connessioni.reg e' stato cambiato da un altro programma (config?) dopo che lgconn l'ha aperto.\n\nPer non cancellare quelle modifiche non lo sovrascrivo.\n\nSalvo le tue connessioni a parte, in [file tail $alt]? Poi potrai ricaricare (File -> Ricarica) e rifare le tue modifiche, confrontando i due file."]
        if {$risp eq "yes"} {
            set f [open $alt w]
            fconfigure $f -encoding iso8859-1 -translation lf
            foreach c [dict get $task conns] { puts $f [::lgconn::testo_di $c] }
            close $f
            messaggio "Connessioni salvate in $alt (Connessioni.reg non toccato)"
        }
        return 0
    }
    set task $nuovo
    set modificato 0
    riempi
    messaggio "Salvato [dict get $task file_reg] (la versione precedente e' in Connessioni.reg.bak)"
    return 1
}

#  Prima di buttare via le modifiche (Ricarica, Apri, Esci) chiede: Si'
#  salva, No le butta, Annulla torna indietro. Ritorna 1 se si puo' procedere.
proc ::lgconn::gui::puo_lasciare {} {
    variable modificato
    if {!$modificato} { return 1 }
    set risp [tk_messageBox -parent . -icon question -type yesnocancel -title "lgconn" \
        -message "Le connessioni sono state modificate e non salvate. Le salvo?"]
    switch -- $risp {
        yes     { return [salva] }
        no      { return 1 }
        default { return 0 }
    }
}

proc ::lgconn::gui::carica {dir} {
    variable task
    variable modificato
    if {[catch {::lgconn::carica_task $dir} t]} {
        tk_messageBox -parent . -icon error -title "lgconn" -message $t
        return 0
    }
    set task $t
    set modificato 0
    wm title . "lgconn - [dict get $task dir]"
    riempi
    return 1
}

proc ::lgconn::gui::ricarica {} {
    variable task
    if {![puo_lasciare]} return
    carica [dict get $task dir]
    messaggio "Ricaricato da disco"
}

proc ::lgconn::gui::apri {} {
    if {![puo_lasciare]} return
    set d [tk_chooseDirectory -parent . -mustexist 1 -title "Directory della task di regolazione (con Context.ctx)"]
    if {$d ne ""} { carica $d }
}

proc ::lgconn::gui::esci {} {
    if {[puo_lasciare]} { destroy . ; exit 0 }
}

proc ::lgconn::gui::mostra_rapporto {} {
    variable task
    lassign [::lgconn::testo_rapporto $task 1] testo
    set w .rapporto
    destroy $w
    toplevel $w
    wm title $w "lgconn - rapporto del controllo"
    text $w.t -wrap word -width 110 -height 35 -yscrollcommand [list $w.s set]
    scrollbar $w.s -command [list $w.t yview]
    pack $w.s -side right -fill y
    pack $w.t -fill both -expand 1
    $w.t insert end $testo
    if {[set ::lgconn::gui::modificato]} {
        $w.t insert 1.0 "(il rapporto riguarda le connessioni come sono ora nell'editor, non ancora salvate)\n\n"
    }
    $w.t configure -state disabled
}

# ---------------------------------------------------------------------------
# Menu "?": in cima la guida rapida, in una finestra di lgconn (cos'e' e come
# si usa, in breve); sotto i documenti completi, aperti nel browser come nel
# "?" di lghmi e lgmkstaz. Solo ASCII: con LANG=POSIX Tcl non decodifica
# l'UTF-8.
# ---------------------------------------------------------------------------

proc ::lgconn::gui::testo_guida {} {
    return {lgconn - le connessioni fra gli schemi di una task di regolazione

COS'E'
In una task di regolazione (directory r_...) le variabili passano da uno
schema (una pagina) all'altro attraverso le icone di interfaccia: l'uscita
di uno schema collegata all'ingresso di un altro. Queste connessioni non
stanno nelle pagine ma nel file Connessioni.reg della task. lgconn le mostra
tutte in una tabella e permette di farle, toglierle e controllarle, senza
aprire config e senza aprire le pagine.

LA FINESTRA
- In alto, tutti gli ingressi di interfaccia della task, con l'uscita a cui
  sono collegati e lo stato: collegato; libero (in grigio); avvisi (in
  giallo); errori (in rosso).
- In basso, le uscite che si possono collegare all'ingresso scelto: libere,
  dello stesso tipo (analogico o digitale), collegate a un blocco dentro il
  loro schema.
- Cerca: filtra le due tabelle per pagina, variabile, blocco o descrizione.

COME SI USA
- Collegare: selezionare l'ingresso in alto, poi doppio clic sull'uscita in
  basso (oppure Modifica -> Collega).
- Scollegare: selezionare l'ingresso collegato, Modifica -> Scollega.
- Salvare: File -> Salva (Ctrl+S). Finche' non si salva non si scrive
  niente; il primo salvataggio lascia la copia Connessioni.reg.bak.
- Controllare: Verifica -> Rapporto completo (gli stessi controlli di
  "lgconn -c"). Gli errori sono connessioni che la compilazione userebbe
  sbagliate; gli avvisi, connessioni verso pagine che non ci sono piu'.
- Pulire: Modifica -> Aggiorna i dati copiati dalle pagine (corregge
  variabili e tag invecchiati); Modifica -> Togli le connessioni verso pagine
  assenti.
- Dopo il salvataggio la task di regolazione va ricompilata, perche' le
  connessioni nuove entrino nel simulatore.

DA SAPERE
- Le icone di interfaccia, e il loro filo verso il blocco dentro lo schema,
  si disegnano in config: lgconn collega fra loro icone che esistono gia'.
  Un'icona non collegata a un blocco nel suo schema non viene proposta.
- Con config aperto insieme: nessuno dei due sovrascrive Connessioni.reg se
  l'altro lo ha cambiato; le pagine aperte in config vedono le modifiche
  solo dopo averle riaperte.
- Si apre anche da config (menu Scheme Conn.) e da lghmi (menu Tools).
  Da terminale: "lgconn DIR" apre questo editor, "lgconn -c DIR" controlla
  senza grafica.

La guida completa e' nel menu "?": Alg_mmi/README.md, sezione "Collegare
gli schemi di una task di regolazione".
}
}

proc ::lgconn::gui::mostra_guida {} {
    set w .guida
    if {[winfo exists $w]} { raise $w; return }
    toplevel $w
    wm title $w "lgconn - guida rapida"
    text $w.t -wrap word -width 80 -height 40 -yscrollcommand [list $w.s set]
    scrollbar $w.s -command [list $w.t yview]
    button $w.chiudi -text Chiudi -command [list destroy $w]
    pack $w.chiudi -side bottom -pady 4
    pack $w.s -side right -fill y
    pack $w.t -fill both -expand 1
    $w.t insert end [testo_guida]
    #  i titoli delle sezioni (righe tutte maiuscole) in grassetto
    $w.t tag configure titolo -font [concat [font actual TkFixedFont] -weight bold]
    set n [lindex [split [$w.t index end] .] 0]
    for {set i 1} {$i <= $n} {incr i} {
        set riga [$w.t get $i.0 "$i.0 lineend"]
        if {$i == 1 || ($riga ne "" && [regexp {^[A-Z' ]+$} $riga])} {
            $w.t tag add titolo $i.0 "$i.0 lineend"
        }
    }
    $w.t configure -state disabled
    bind $w <Escape> [list destroy $w]
}

#  Un documento nel browser (openhelp.tcl); il messaggio va nella barra di
#  stato.
proc ::lgconn::gui::apri_documento {relativo} {
    variable stato
    if {[llength [info procs aiuto_apri_documento]] == 0} {
        tk_messageBox -icon error -title lgconn -parent . -message \
            "openhelp.tcl non caricato: non posso aprire la documentazione."
        return
    }
    set messaggio [aiuto_apri_documento $relativo]
    if {$messaggio ne ""} { set stato $messaggio }
}

proc ::lgconn::gui::costruisci_menu_aiuto {m} {
    if {[lsearch -exact [font names] fontMenuRilievo] < 0} {
        font create fontMenuRilievo {*}[font actual TkMenuFont]
        font configure fontMenuRilievo -weight bold
    }
    $m add command -label "Guida rapida: cos'e' lgconn e come si usa" \
        -font fontMenuRilievo -command ::lgconn::gui::mostra_guida
    $m add separator
    #  Una voce che punta a un file assente nasce SPENTA invece di sparire
    #  (stessa scelta di lghmi e lgmkstaz).
    foreach {etichetta relativo} {
        "Guida completa: collegare gli schemi (Alg_mmi/README.md)"  Alg_mmi/README.md
        "I comandi lg* (docs/COMANDI_LG.md)"                        docs/COMANDI_LG.md
        "Indice della documentazione"                                DOCUMENTATION_INDEX.html
    } {
        set c_e [expr {[info exists ::env(LEGOROOT)] && $::env(LEGOROOT) ne "" \
                       && [file exists [file join $::env(LEGOROOT) $relativo]]}]
        $m add command -label $etichetta -state [expr {$c_e ? "normal" : "disabled"}] \
            -command [list ::lgconn::gui::apri_documento $relativo]
    }
}

proc ::lgconn::gui::costruisci {} {
    wm geometry . 1250x760
    wm protocol . WM_DELETE_WINDOW ::lgconn::gui::esci

    menu .mb
    . configure -menu .mb
    menu .mb.file -tearoff 0
    .mb add cascade -label "File" -menu .mb.file
    .mb.file add command -label "Apri task..." -command ::lgconn::gui::apri
    .mb.file add command -label "Ricarica" -command ::lgconn::gui::ricarica
    .mb.file add command -label "Salva" -accelerator Ctrl+S -command ::lgconn::gui::salva
    .mb.file add separator
    .mb.file add command -label "Esci" -command ::lgconn::gui::esci
    menu .mb.modifica -tearoff 0
    .mb add cascade -label "Modifica" -menu .mb.modifica
    .mb.modifica add command -label "Collega" -command ::lgconn::gui::collega
    .mb.modifica add command -label "Scollega" -command ::lgconn::gui::scollega
    .mb.modifica add separator
    .mb.modifica add command -label "Aggiorna i dati copiati dalle pagine" -command ::lgconn::gui::aggiorna_copie
    .mb.modifica add command -label "Togli le connessioni verso pagine assenti" -command ::lgconn::gui::togli_orfane
    menu .mb.vista -tearoff 0
    .mb add cascade -label "Visualizza" -menu .mb.vista
    .mb.vista add checkbutton -label "Solo ingressi liberi" -variable ::lgconn::gui::solo_liberi \
        -command {set ::lgconn::gui::solo_problemi 0; ::lgconn::gui::riempi}
    .mb.vista add checkbutton -label "Solo righe con problemi" -variable ::lgconn::gui::solo_problemi \
        -command {set ::lgconn::gui::solo_liberi 0; ::lgconn::gui::riempi}
    menu .mb.verifica -tearoff 0
    .mb add cascade -label "Verifica" -menu .mb.verifica
    .mb.verifica add command -label "Rapporto completo" -command ::lgconn::gui::mostra_rapporto
    menu .mb.aiuto -tearoff 0
    .mb add cascade -label "?" -menu .mb.aiuto
    costruisci_menu_aiuto .mb.aiuto

    frame .cerca
    label .cerca.l -text "Cerca:"
    entry .cerca.e -textvariable ::lgconn::gui::filtro -width 40
    label .cerca.n -text "  (pagina, variabile, blocco, descrizione; in alto gli ingressi, in basso le uscite)" -foreground #555555
    pack .cerca.l .cerca.e .cerca.n -side left -padx 3
    pack .cerca -side top -fill x -pady 4
    bind .cerca.e <KeyRelease> ::lgconn::gui::riempi

    label .stato -textvariable ::lgconn::gui::stato -anchor w -relief sunken -padx 4
    pack .stato -side bottom -fill x

    ttk::panedwindow .corpo -orient vertical
    pack .corpo -fill both -expand 1 -padx 4

    #  in alto: gli ingressi
    ttk::labelframe .corpo.ingressi -text "Ingressi di interfaccia di tutti gli schemi"
    set t [ttk::treeview .corpo.ingressi.t -columns {pi ii vi ti di pu vu tu st} -show headings \
               -selectmode browse -yscrollcommand {.corpo.ingressi.s set}]
    ttk::scrollbar .corpo.ingressi.s -command [list $t yview]
    foreach col {pi ii vi ti di pu vu tu st} tit {Pagina Icona Variabile Blocco Descrizione \
            "<- da pagina" Variabile Blocco Stato} larg {60 55 65 65 230 85 65 65 500} {
        $t heading $col -text $tit -anchor w
        $t column $col -width $larg -stretch [expr {$col eq "st"}] -anchor w
    }
    $t tag configure errore -background #ffd6d6
    $t tag configure avviso -background #fff1c2
    $t tag configure libero -foreground #777777
    pack .corpo.ingressi.s -side right -fill y
    pack $t -fill both -expand 1
    bind $t <<TreeviewSelect>> ::lgconn::gui::riempi_uscite
    .corpo add .corpo.ingressi -weight 3

    #  in basso: le uscite collegabili, e i bottoni
    ttk::labelframe .corpo.uscite -text "Collega"
    label .corpo.uscite.titolo -anchor w -font {TkDefaultFont 10 bold}
    frame .corpo.uscite.cerca
    label .corpo.uscite.cerca.l -text "Cerca fra le uscite:"
    entry .corpo.uscite.cerca.e -textvariable ::lgconn::gui::filtro_uscite -width 30
    pack .corpo.uscite.cerca.l .corpo.uscite.cerca.e -side left -padx 3
    bind .corpo.uscite.cerca.e <KeyRelease> ::lgconn::gui::riempi_uscite
    set u [ttk::treeview .corpo.uscite.t -columns {pu iu vu tu du tipo} -show headings \
               -selectmode browse -height 8 -yscrollcommand {.corpo.uscite.s set}]
    ttk::scrollbar .corpo.uscite.s -command [list $u yview]
    foreach col {pu iu vu tu du tipo} tit {Pagina Icona Variabile Blocco Descrizione Tipo} \
            larg {70 60 70 70 380 80} {
        $u heading $col -text $tit -anchor w
        $u column $col -width $larg -stretch [expr {$col eq "du"}] -anchor w
    }
    pack .corpo.uscite.titolo -side top -fill x -padx 4 -pady 2
    pack .corpo.uscite.cerca -side top -fill x
    pack .corpo.uscite.s -side right -fill y
    pack $u -fill both -expand 1
    bind $u <<TreeviewSelect>> ::lgconn::gui::aggiorna_bottoni
    bind $u <Double-1> ::lgconn::gui::collega
    .corpo add .corpo.uscite -weight 2

    frame .corpo.bottoni
    button .corpo.bottoni.collega -text "Collega" -width 12 -command ::lgconn::gui::collega
    button .corpo.bottoni.scollega -text "Scollega" -width 12 -command ::lgconn::gui::scollega
    button .corpo.bottoni.salva -text "Salva" -width 12 -command ::lgconn::gui::salva
    pack .corpo.bottoni.collega .corpo.bottoni.scollega -side left -padx 4 -pady 4
    pack .corpo.bottoni.salva -side right -padx 4 -pady 4
    pack .corpo.bottoni -in .corpo.uscite -side bottom -fill x -before .corpo.uscite.s

    bind . <Control-s> ::lgconn::gui::salva
}

::lgconn::gui::costruisci
::lgconn::gui::carica [expr {[llength $::argv] > 0 ? [lindex $::argv 0] : "."}]
