# hmielem.tcl - elementi operatore delle pagine: faceplate e invio di valori.
#
# Sorgiato da animate.tcl, quindi presente ovunque ci sia View -> Show Value:
# legopc (tab Data Assignment & Simulation, e l'inserimento da Model
# Topology) e draw2gr.
#
# Due elementi della libreria remark, testi non topologici come @val_0:
#   @stz_0  tag hmistaz  bottone che apre una pagina di faceplate (xstaz)
#   @set_0  tag hmiset   invio di un valore a una variabile di ingresso
#
# Il loro contenuto - la pagina, la variabile - sta nel file .remap del
# modello, per nome istanza, con un suffisso di tipo:
#   F001=RISCBP;F      pagina di faceplate (non e' una variabile: al
#                      caricamento non si valida contro il modello)
#   S001=WEST;S        variabile a cui inviare valori (deve essere IN)
# Si assegna da Model Topology (subito dopo l'inserimento, e poi con la voce
# del menu che per i testi e' "Modify Text") oppure in Show Value (tasto
# destro sull'elemento). Nel .tom resta solo l'etichetta del segnaposto.
#
# Gesti in Show Value:
#   tasto sinistro  faceplate: apre la pagina; set value: apre il dialogo
#   tasto destro    menu con quello che e' assegnato e "Assign ..."
#
# SENZA SIMULAZIONE NON SI DISTURBA: gli elementi si disegnano spenti, un
# clic mostra un fumetto breve e basta, e ogni comando esterno gira dentro un
# catch con l'output in un log ($::hmi_log). I dialoghi restano solo per le
# scelte dell'utente (assegnazioni) e per il conflitto con un xstaz che gira
# su un'altra simulazione.
#
# Solo ASCII nelle stringhe mostrate: con LANG=POSIX Tcl non decodifica UTF-8.

source [file join [file dirname [file normalize [info script]]] lgstaz.tcl]

#  Dove finisce l'output dei comandi lanciati da qui (viewval -f, xaing).
set ::hmi_log /tmp/legopc_hmi.log

#  Stato "simulazione dal vivo" per canvas, per ridisegnare gli elementi solo
#  quando cambia (vedi hmi_aggiorna).
if {![array exists ::hmi_live_di]} { array set ::hmi_live_di {} }

# --------------------------------------------------------------------------
# Riconoscimento e dati di un elemento
# --------------------------------------------------------------------------

#  Tipo di un item: staz, set, oppure "" se non e' un elemento operatore.
proc hmi_tipo {c item} {
    set tags [$c gettags $item]
    if {[lsearch -exact $tags hmistaz] >= 0} { return staz }
    if {[lsearch -exact $tags hmiset]  >= 0} { return set }
    return ""
}

#  Nome istanza dell'elemento (tag <nome>.name).
proc hmi_inst {c item} {
    set tags [$c gettags $item]
    return [file rootname [lindex $tags [lsearch $tags *.name]]]
}

#  Suffisso del .remap per ciascun tipo.
proc hmi_modo {tipo} {
    return [expr {$tipo eq "staz" ? "F" : "S"}]
}

#  La pagina o la variabile assegnata a <inst>, "" se nessuna.
#  fonte = mem  dalle strutture caricate da anim_load_remap (Show Value);
#  fonte = file dal .remap su disco: fuori da Show Value le strutture in
#               memoria possono essere di un modello aperto prima.
proc hmi_valore {inst tipo {fonte mem}} {
    set modo [hmi_modo $tipo]
    if {$fonte eq "file"} {
        set d [anim_remap_leggi]
        if {![dict exists $d $inst]} { return "" }
        lassign [dict get $d $inst] var m
        return [expr {$m eq $modo ? $var : ""}]
    }
    if {![info exists ::anim_remap($inst)]} { return "" }
    if {![info exists ::anim_mode($inst)] || $::anim_mode($inst) ne $modo} { return "" }
    return $::anim_remap($inst)
}

#  Testo dell'elemento: quello che il .tom conserva e che si vede dove il
#  segnaposto disegnato non c'e' (una versione piu' vecchia, un altro
#  programma). E' lo STESSO testo del segnaposto, cosi' la casella che gli sta
#  sopra lo copre esatto senza allargarsi.
proc hmi_etichetta {tipo valore} {
    if {$valore ne ""} { return $valore }
    return [expr {$tipo eq "staz" ? "xstaz: ?" : "set: ?"}]
}

#  Rimette le etichette dei segnaposto d'accordo con il .remap: la pagina o la
#  variabile possono essere state assegnate da un'altra applicazione (draw2gr)
#  dopo l'ultimo salvataggio del .tom. Da chiamare dopo topRead.
proc hmi_aggiorna_etichette {c} {
    foreach item [concat [$c find withtag hmistaz] [$c find withtag hmiset]] {
        set tipo [hmi_tipo $c $item]
        catch {
            $c itemconfigure $item \
                -text [hmi_etichetta $tipo [hmi_valore [hmi_inst $c $item] $tipo file]]
        }
    }
}

#  Descrizione di una variabile dal F01 caricato (blocVars), o "".
proc hmi_descrizione {var} {
    foreach tipo {IN US UA} {
        if {[info exists ::matrVblo($var,$tipo,bloc)]} {
            set b $::matrVblo($var,$tipo,bloc)
            set k $::matrVblo($var,$tipo,indx)
            if {[info exists ::blocVars($b,$k,desc)]} {
                return [string trim $::blocVars($b,$k,desc)]
            }
        }
    }
    return ""
}

# --------------------------------------------------------------------------
# Stato della simulazione, fumetti, stato, log
# --------------------------------------------------------------------------

#  La simulazione e' dal vivo? Serve la pipe di Show Value aperta e non finita
#  (anima init) e net_sked vivo. net_sked si controlla al massimo ogni 3 s:
#  la proc e' chiamata a ogni ciclo di animazione e a ogni clic.
#  Fuori da Linux gli elementi restano spenti.
proc hmi_live {} {
    if {![info exists ::LINUXPLAT] || $::LINUXPLAT != 1} { return 0 }
    if {![info exists ::pipeon] || !$::pipeon} { return 0 }
    if {[catch {eof $::pipeanim} finita] || $finita} { return 0 }
    set ora [clock seconds]
    if {![info exists ::hmi_sked_quando] || $ora - $::hmi_sked_quando >= 3} {
        set ::hmi_sked_quando $ora
        set ::hmi_sked_vivo [hmi_sked_vivo]
    }
    return $::hmi_sked_vivo
}

#  net_sked e' vivo? Stesso controllo di ShowNamesfilt.
proc hmi_sked_vivo {} {
    if {[catch {exec ps -A -o comm} out]} { return 0 }
    foreach riga [split $out "\n"] {
        if {[string trim $riga] eq "net_sked"} { return 1 }
    }
    return 0
}

#  Directory della simulazione (Set Sim path), "" se non valida: in quel caso
#  i comandi girano nella directory corrente, come fa anima init.
proc hmi_sim_dir {} {
    if {[info exists ::anima_sim_path] && $::anima_sim_path ne "" \
        && $::anima_sim_path != 0 && [file isdirectory $::anima_sim_path]} {
        return $::anima_sim_path
    }
    return ""
}

#  Fumetto breve vicino al puntatore (coordinate dello schermo). Non blocca e
#  sparisce da solo: e' la risposta a un clic che non puo' fare niente.
proc hmi_fumetto {X Y testo {ms 1800}} {
    catch {after cancel $::hmi_fumetto_after}
    catch {destroy .hmi_fumetto}
    toplevel .hmi_fumetto
    wm overrideredirect .hmi_fumetto 1
    wm geometry .hmi_fumetto +[expr {$X + 12}]+[expr {$Y + 12}]
    label .hmi_fumetto.l -text $testo -justify left \
        -background "#FFFFE0" -relief solid -bd 1 -font "Helvetica 9" -padx 4 -pady 2
    pack .hmi_fumetto.l
    raise .hmi_fumetto
    set ::hmi_fumetto_after [after $ms {catch {destroy .hmi_fumetto}}]
}

#  Riga di stato dell'applicazione, se ne ha una (legopc la crea nel tab Data
#  Assignment e la registra in ::hmi_status_widget); altrimenti stderr.
proc hmi_stato {testo} {
    if {[info exists ::hmi_status_widget] && [winfo exists $::hmi_status_widget]} {
        $::hmi_status_widget configure -text $testo
    } else {
        puts stderr $testo
    }
}

#  Una riga nel log dei comandi.
proc hmi_log_scrivi {testo} {
    catch {
        set fd [open $::hmi_log a]
        puts $fd "[clock format [clock seconds] -format {%Y-%m-%d %H:%M:%S}]  $testo"
        close $fd
    }
}

#  Nome dell'applicazione, per il registro delle scritture.
proc hmi_app {} {
    return [file rootname [file tail $::argv0]]
}

#  Un programma dell'installazione: $LEGORT_BIN/<nome> se c'e', o il nome nudo.
proc hmi_cmd {nome} {
    return [staz_cmd $nome]
}

# --------------------------------------------------------------------------
# Disegno in Show Value
# --------------------------------------------------------------------------

#  Font scalato allo zoom del canvas, e la sua base a 100% (per doZoom).
proc hmi_font {c} {
    set zl [expr {[info exists ::zoomLevelOf($c)] ? $::zoomLevelOf($c) : 1.0}]
    set sz [expr {int(round(8 * $zl))}]
    if {$sz < 1} { set sz 1 }
    return [list [list Helvetica $sz] {Helvetica 8}]
}

#  Un bottone disegnato sul canvas: testo, fondo, due linee per il rilievo.
#  <minbox> (x1 y1 x2 y2) e' il minimo da coprire: il segnaposto sotto.
#  Ritorna gli id creati. Non un widget vero: dentro un canvas non seguirebbe
#  lo zoom e resterebbe sopra a tutto.
proc hmi_bottone {c cx cy testo attivo tags {minbox {}}} {
    lassign [hmi_font $c] font base
    if {$attivo} {
        set fg black;     set bg "#d9d9d9"; set chiaro white;     set scuro "#6e6e6e"
    } else {
        set fg "#9a9a9a"; set bg "#ececec"; set chiaro "#f8f8f8"; set scuro "#c4c4c4"
    }
    set t [$c create text $cx $cy -text $testo -font $font -fill $fg -tags $tags]
    set ::origFontOf($c,$t) $base
    lassign [$c bbox $t] x1 y1 x2 y2
    set x1 [expr {$x1 - 4}]; set y1 [expr {$y1 - 2}]
    set x2 [expr {$x2 + 4}]; set y2 [expr {$y2 + 2}]
    if {[llength $minbox] == 4} {
        lassign $minbox m1 n1 m2 n2
        set x1 [expr {min($x1, $m1 - 1)}]; set y1 [expr {min($y1, $n1 - 1)}]
        set x2 [expr {max($x2, $m2 + 1)}]; set y2 [expr {max($y2, $n2 + 1)}]
    }
    set r  [$c create rectangle $x1 $y1 $x2 $y2 -fill $bg -outline "" -tags $tags]
    set l1 [$c create line $x1 $y2 $x1 $y1 $x2 $y1 -fill $chiaro -tags $tags]
    set l2 [$c create line $x2 $y1 $x2 $y2 $x1 $y2 -fill $scuro -tags $tags]
    $c raise $t
    return [list $t $r $l1 $l2]
}

#  Collega i gesti a tutti gli oggetti disegnati per <item> (tag hmov<item>).
#  Il binding e' sul tag, quindi vale anche per gli oggetti ridisegnati dopo.
#  L'azione parte al RILASCIO del tasto, come per un bottone vero: aprire una
#  finestra mentre il tasto e' ancora premuto (il canvas ha il puntatore
#  catturato) confonde alcuni window manager.
proc hmi_collega {c item} {
    $c bind hmov$item <1>               {}
    $c bind hmov$item <ButtonRelease-1> [list hmi_clic $c $item %X %Y]
    $c bind hmov$item <ButtonRelease-3> [list hmi_menu $c $item %X %Y]
    $c bind hmov$item <Enter>           [list $c configure -cursor hand2]
    $c bind hmov$item <Leave>           [list $c configure -cursor {}]
}

#  Centro e ingombro del segnaposto.
proc hmi_ingombro {c item} {
    return [$c bbox $item]
}

#  Disegna (o ridisegna) un elemento. live = simulazione dal vivo.
proc hmi_campo {c item live} {
    switch -- [hmi_tipo $c $item] {
        staz { hmi_campo_staz $c $item $live }
        set  { hmi_campo_set  $c $item $live }
    }
}

#  Tutti gli elementi di un canvas. Registra lo stato per hmi_aggiorna.
proc hmi_campi {c live} {
    #  Show Value prende il posto dei segnaposti disegnati (tab dei dati).
    hmi_segnaposti_via $c
    foreach item [concat [$c find withtag hmistaz] [$c find withtag hmiset]] {
        catch {hmi_campo $c $item $live}
    }
    set ::hmi_live_di($c) $live
}

#  Faceplate: un bottone con il nome della pagina.
proc hmi_campo_staz {c item live} {
    catch {$c delete hmov$item}
    set pagina [hmi_valore [hmi_inst $c $item] staz]
    #  Il faceplate non ha bisogno della simulazione (staz_apri, lgstaz.tcl):
    #  la pagina si apre anche a simulazione ferma, con i valori fermi.
    set attivo [expr {$pagina ne "" && [staz_disponibile]}]
    set testo [expr {$pagina ne "" ? $pagina : "xstaz: ?"}]
    lassign [hmi_ingombro $c $item] x1 y1 x2 y2
    hmi_bottone $c [expr {($x1 + $x2) / 2.0}] [expr {($y1 + $y2) / 2.0}] \
        $testo $attivo [list infoitemname hmov$item] [list $x1 $y1 $x2 $y2]
    hmi_collega $c $item
}

#  Testo del riquadro di un set value: variabile e valore.
proc hmi_testo_set {var live} {
    if {$var eq ""} { return "set: ?" }
    if {$live} {
        anima leggi "$var\n"
        if {$::line_anim eq "" || [regexp -nocase {^ERRORE} $::line_anim]} {
            set v "--"
        } else {
            set v "[lindex $::line_anim 0] [lindex $::line_anim 1]"
        }
    } elseif {[info exists ::matrVf14($var,valu)]} {
        set v "[conv_umis $var $::matrVf14($var,valu)] [ret_umis $var]"
    } else {
        set v "--"
    }
    return "$var $v"
}

#  Set value: riquadro con il valore (giallo dal vivo, azzurro fuori, come i
#  display) e, a destra, il bottone "Set".
proc hmi_campo_set {c item live} {
    catch {$c delete hmov$item}
    set var [hmi_valore [hmi_inst $c $item] set]
    lassign [hmi_font $c] font base
    lassign [hmi_ingombro $c $item] x1 y1 x2 y2
    set cx [expr {($x1 + $x2) / 2.0}]
    set cy [expr {($y1 + $y2) / 2.0}]
    set t [$c create text $cx $cy -text [hmi_testo_set $var $live] -font $font \
               -tags [list infoitemname hmov$item hmiv$item]]
    set ::origFontOf($c,$t) $base
    set fondo [expr {$live ? "yellow" : "cyan"}]
    $c create rectangle 0 0 1 1 -fill $fondo -outline "" \
        -tags [list infoitemname hmov$item hmir$item]
    $c raise $t
    hmi_bottone $c 0 0 "Set" [expr {$live && $var ne ""}] \
        [list infoitemname hmov$item hmib$item]
    hmi_adatta_set $c $item
    hmi_collega $c $item
}

#  Rimette riquadro e bottone intorno al testo del valore, che cambia
#  lunghezza a ogni aggiornamento.
proc hmi_adatta_set {c item} {
    lassign [hmi_ingombro $c $item] p1 q1 p2 q2
    set t [$c find withtag hmiv$item]
    lassign [$c bbox $t] x1 y1 x2 y2
    set x1 [expr {min($x1, $p1) - 1}]; set y1 [expr {min($y1, $q1) - 1}]
    set x2 [expr {max($x2, $p2) + 1}]; set y2 [expr {max($y2, $q2) + 1}]
    $c coords [$c find withtag hmir$item] $x1 $y1 $x2 $y2
    lassign [$c bbox hmib$item] bx1 by1 bx2 by2
    $c move hmib$item [expr {$x2 + 3 - $bx1}] \
        [expr {($y1 + $y2) / 2.0 - ($by1 + $by2) / 2.0}]
}

#  Aggiornamento periodico (anima_aggiorna, modo 2): ridisegna tutto quando
#  la simulazione compare o sparisce, altrimenti aggiorna solo i valori.
proc hmi_aggiorna {c} {
    set live [hmi_live]
    if {![info exists ::hmi_live_di($c)] || $::hmi_live_di($c) != $live} {
        hmi_campi $c $live
        return
    }
    if {!$live} return
    foreach item [$c find withtag hmiset] {
        set var [hmi_valore [hmi_inst $c $item] set]
        if {$var eq ""} continue
        set t [$c find withtag hmiv$item]
        if {$t eq ""} continue
        catch {
            $c itemconfigure $t -text [hmi_testo_set $var 1]
            hmi_adatta_set $c $item
        }
    }
}

# --------------------------------------------------------------------------
# Segnaposti nel canvas di disegno (Model Topology)
# --------------------------------------------------------------------------
#
# Nel tab del disegno gli elementi si vedono come in Show Value a simulazione
# ferma - casella per il display, bottoni grigi per faceplate e set value, con
# dentro la variabile o la pagina assegnata - invece del testo fra parentesi
# quadre che il .tom conserva.
#
# I pezzi disegnati stanno SOPRA l'elemento ma hanno "-state disabled": Tk li
# disegna e basta, non li considera nella scelta dell'oggetto sotto il
# puntatore. Cosi' il clic arriva sempre all'elemento, e trascinamento,
# selezione e menu del tasto destro funzionano come prima. Non portano il tag
# del modulo ne' quello dell'istanza: chi conta i moduli (writeFiles, il .top)
# e chi legge i tag per posizione non li vede.
#
# Non si spostano da soli: si ridisegnano quando qualcosa cambia (caricamento,
# inserimento, assegnazione, fine trascinamento, cancellazione, incolla,
# ordine di sovrapposizione). Durante il trascinamento si tolgono, cosi' non
# restano indietro. Lo zoom invece li scala da se', come le caselle di
# Show Value.

#  E' un canvas di legopc (Model Topology o Data Assignment)? Li' gli elementi
#  si vedono come segnaposti disegnati; quando pero' e' attivo Show Value, nel
#  tab dei dati comandano le caselle vive e i segnaposti si tolgono. In
#  draw2gr ::canv1 e' la pagina, e li' comanda sempre Show Value.
proc hmi_canvas_disegno {c} {
    if {![info exists ::envir] || $::envir eq "Draw2Gr"} { return 0 }
    foreach v {::canv1 ::canv2} {
        if {[info exists $v] && $c eq [set $v]} { return 1 }
    }
    return 0
}

#  La variabile di un display @val_0 dal .remap (le righe senza suffisso, o
#  con ";L" per la modalita' etichetta).
proc hmi_valore_display {inst} {
    set d [anim_remap_leggi]
    if {![dict exists $d $inst]} { return "" }
    lassign [dict get $d $inst] var m
    if {$m eq "F" || $m eq "S"} { return "" }
    return $var
}

#  Casella piatta (il display): testo su fondo pieno, senza bordo.
proc hmi_casella {c cx cy testo tags minbox {fondo "#ececec"} {fg black}} {
    lassign [hmi_font $c] font base
    set t [$c create text $cx $cy -text $testo -font $font -fill $fg -tags $tags]
    set ::origFontOf($c,$t) $base
    lassign [$c bbox $t] x1 y1 x2 y2
    if {[llength $minbox] == 4} {
        lassign $minbox m1 n1 m2 n2
        set x1 [expr {min($x1, $m1)}] ; set y1 [expr {min($y1, $n1)}]
        set x2 [expr {max($x2, $m2)}] ; set y2 [expr {max($y2, $n2)}]
    }
    set r [$c create rectangle [expr {$x1 - 1}] [expr {$y1 - 1}] \
               [expr {$x2 + 1}] [expr {$y2 + 1}] -fill $fondo -outline "" -tags $tags]
    $c raise $t
    return [list $t $r]
}

#  Ridisegna il segnaposto di un elemento.
proc hmi_segnaposto {c item} {
    set tags [$c gettags $item]
    set tg [list hmiedit hmiedit$item]
    $c delete hmiedit$item
    lassign [$c bbox $item] x1 y1 x2 y2
    if {$x1 eq ""} return
    set cx [expr {($x1 + $x2) / 2.0}]
    set cy [expr {($y1 + $y2) / 2.0}]
    set inst [hmi_inst $c $item]
    if {[lsearch -exact $tags freeval] >= 0} {
        #  Senza variabile assegnata la casella c'e' lo stesso, con "--?--"
        #  al posto del nome (come il placeholder di @val_0): si vede subito
        #  che il display e' li' e cosa gli manca, e un solo "?" sarebbe una
        #  casella minuscola.
        set var [hmi_valore_display $inst]
        hmi_casella $c $cx $cy [expr {$var ne "" ? $var : "--?--"}] $tg \
            [list $x1 $y1 $x2 $y2]
    } elseif {[lsearch -exact $tags hmistaz] >= 0} {
        set pag [hmi_valore $inst staz file]
        hmi_bottone $c $cx $cy [expr {$pag ne "" ? $pag : "xstaz: ?"}] 0 $tg \
            [list $x1 $y1 $x2 $y2]
    } elseif {[lsearch -exact $tags hmiset] >= 0} {
        set var [hmi_valore $inst set file]
        lassign [hmi_casella $c $cx $cy [expr {$var ne "" ? $var : "set: ?"}] $tg \
                     [list $x1 $y1 $x2 $y2]] t r
        lassign [$c bbox $r] b1 c1 b2 c2
        set ids [hmi_bottone $c 0 0 "Set" 0 [concat $tg hmieditb$item]]
        lassign [$c bbox hmieditb$item] d1 e1 d2 e2
        $c move hmieditb$item [expr {$b2 + 3 - $d1}] \
            [expr {($c1 + $c2) / 2.0 - ($e1 + $e2) / 2.0}]
    } else {
        return
    }
    #  Disegnati ma non cliccabili: il clic passa all'elemento sotto.
    foreach id [$c find withtag hmiedit$item] { $c itemconfigure $id -state disabled }
}

#  Ridisegna tutti i segnaposti del canvas di disegno.
proc hmi_segnaposti {c} {
    if {![hmi_canvas_disegno $c]} return
    $c delete hmiedit
    foreach item [concat [$c find withtag freeval] [$c find withtag hmistaz] \
                      [$c find withtag hmiset]] {
        catch {hmi_segnaposto $c $item}
    }
}

#  Ridisegno differito: la chiamano le operazioni del canvas (inserimento,
#  incolla) che finiscono di sistemare testo e font DOPO aver creato
#  l'elemento. A operazione conclusa ne resta uno solo.
proc hmi_segnaposti_dopo {c} {
    if {![hmi_canvas_disegno $c]} return
    if {[info exists ::hmi_segnaposti_attesa($c)]} return
    set ::hmi_segnaposti_attesa($c) 1
    after idle [list hmi_segnaposti_ora $c]
}

proc hmi_segnaposti_ora {c} {
    unset -nocomplain ::hmi_segnaposti_attesa($c)
    catch {hmi_segnaposti $c}
}

#  Li toglie (durante un trascinamento resterebbero indietro).
proc hmi_segnaposti_via {c} {
    catch {$c delete hmiedit}
}

# --------------------------------------------------------------------------
# Gesti
# --------------------------------------------------------------------------

#  Tasto sinistro. Un secondo clic entro mezzo secondo (un doppio clic, gesto
#  abituale con i display) si ignora: altrimenti arriverebbe a draw2gr, che
#  il window manager porterebbe in primo piano, sopra il dialogo appena aperto.
proc hmi_clic {c item X Y} {
    set ora [clock milliseconds]
    if {[info exists ::hmi_ultimo_clic($c,$item)] \
        && $ora - $::hmi_ultimo_clic($c,$item) < 500} { return }
    set ::hmi_ultimo_clic($c,$item) $ora
    switch -- [hmi_tipo $c $item] {
        staz { hmi_apri_staz $c $item $X $Y }
        set  {
            set var [hmi_valore [hmi_inst $c $item] set]
            if {$var eq ""} {
                hmi_fumetto $X $Y "No variable assigned: right-click to assign one."
                return
            }
            hmi_dialogo_set $c $item $X $Y
        }
    }
}

#  Tasto destro: quello che e' assegnato, e le azioni.
proc hmi_menu {c item X Y} {
    set m .hmi_menu
    catch {destroy $m}
    menu $m -tearoff 0 -activebackground darkblue -activeforeground white
    set inst [hmi_inst $c $item]
    switch -- [hmi_tipo $c $item] {
        staz {
            set p [hmi_valore $inst staz]
            $m add command -state disabled \
                -label "Page: [expr {$p ne "" ? $p : "(none)"}]"
            $m add separator
            $m add command -label "Open page" \
                -state [expr {$p ne "" ? "normal" : "disabled"}] \
                -command [list hmi_apri_staz $c $item $X $Y]
            $m add command -label "Assign page..." -command [list hmi_assegna $c $item]
        }
        set {
            set v [hmi_valore $inst set]
            $m add command -state disabled \
                -label "Variable: [expr {$v ne "" ? $v : "(none)"}]"
            $m add separator
            $m add command -label "Set value..." \
                -state [expr {$v ne "" ? "normal" : "disabled"}] \
                -command [list hmi_dialogo_set $c $item $X $Y]
            $m add command -label "Assign variable..." -command [list hmi_assegna $c $item]
        }
        default { return }
    }
    tk_popup $m $X $Y
}

#  Faceplate: apre la pagina, anche a simulazione ferma (serve a costruire e
#  configurare le stazioni). Gli impedimenti sono un fumetto; solo xstaz su
#  un'altra simulazione merita un dialogo.
proc hmi_apri_staz {c item X Y} {
    set pagina [hmi_valore [hmi_inst $c $item] staz]
    if {$pagina eq ""} {
        hmi_fumetto $X $Y "No page assigned: right-click to assign one."
        return
    }
    if {![staz_disponibile]} {
        hmi_fumetto $X $Y "xstaz is not installed."
        return
    }
    set dirs [hmi_staz_dirs]
    set dir [staz_dir_di_pagina $dirs $pagina]
    if {$dir eq ""} {
        hmi_log_scrivi "xstaz: page $pagina not found in: $dirs"
        hmi_fumetto $X $Y "Page $pagina not found in any r02.dat." 2500
        return
    }
    lassign [staz_apri $dir $pagina] esito msg
    switch -- $esito {
        ok      {
            if {$msg eq ""} {
                hmi_fumetto $X $Y "Page $pagina requested." 1200
            } else {
                hmi_fumetto $X $Y "Page $pagina requested.\n$msg" 2500
            }
        }
        altrove { tk_messageBox -icon warning -title "xstaz" \
                      -parent [winfo toplevel $c] -message $msg }
        default {
            hmi_log_scrivi "xstaz: $msg"
            hmi_fumetto $X $Y [lindex [split $msg "\n"] 0] 2500
        }
    }
}

#  Directory da cui le pagine possono venire: la simulazione in corso, il
#  simulatore corrente, e le task dell'area del modello.
proc hmi_staz_dirs {} {
    set radici [list [hmi_sim_dir]]
    if {[info exists ::env(KSIM)]} { lappend radici $::env(KSIM) }
    #  curFileName e' <area>/<task>/<task>.tom in legopc, e lo stesso senza
    #  estensione in draw2gr: la directory della task e' comunque la sua.
    set area ""
    if {[info exists ::curFileName] && $::curFileName ne ""} {
        set d [file dirname [file normalize $::curFileName]]
        if {[file isdirectory $d]} { set area [file dirname $d] }
    }
    return [staz_dirs $radici $area]
}

# --------------------------------------------------------------------------
# Assegnazione di pagina e variabile
# --------------------------------------------------------------------------

#  Apre il dialogo giusto per l'elemento.
proc hmi_assegna {c item} {
    switch -- [hmi_tipo $c $item] {
        staz { hmi_dialogo_pagina $c $item }
        set  { hmi_dialogo_variabile $c $item }
    }
}

#  Dopo un'assegnazione: etichetta del segnaposto, modello modificato (solo
#  nel canvas di Model Topology di legopc, l'unico che salva il .tom) e, se
#  l'elemento e' disegnato in Show Value, il suo ridisegno.
proc hmi_dopo_assegnazione {c item} {
    set tipo [hmi_tipo $c $item]
    catch {$c itemconfigure $item \
               -text [hmi_etichetta $tipo [hmi_valore [hmi_inst $c $item] $tipo]]}
    if {[info exists ::envir] && $::envir ne "Draw2Gr" \
        && [info exists ::canv1] && $c eq $::canv1} {
        # solo Model Topology salva il .tom
        set ::modified 1
    }
    if {[llength [$c find withtag hmov$item]]} {
        hmi_campo $c $item [hmi_live]
    } elseif {[hmi_canvas_disegno $c]} {
        catch {hmi_segnaposto $c $item}
    }
}

#  Scrive l'assegnazione nel .remap; se non si puo' (modello mai salvato) lo
#  dice. Ritorna 1 se e' andata.
proc hmi_salva_assegnazione {w inst valore modo} {
    if {![anim_remap_set $inst $valore $modo]} {
        tk_messageBox -parent $w -icon error -title "Assign" -message \
            "Cannot write the .remap file of the model.\nSave the model first, then assign again."
        return 0
    }
    return 1
}

#  Dialogo: pagina di faceplate. Elenca le pagine trovate negli r02.dat; il
#  nome si puo' anche scrivere a mano (una pagina che non c'e' ancora).
proc hmi_dialogo_pagina {c item} {
    set inst [hmi_inst $c $item]
    set w .hmi_pagina
    catch {destroy $w}
    toplevel $w
    wm title $w "Faceplate page"
    catch {wm transient $w [winfo toplevel $c]}
    set ::hmi_pag_nome [hmi_valore $inst staz file]

    frame $w.f
    pack $w.f -padx 10 -pady 8 -fill both -expand 1
    label $w.f.i -text "Element $inst - page of r02.dat to open with xstaz:" -anchor w
    entry $w.f.e -textvariable ::hmi_pag_nome -width 20
    pack $w.f.i -side top -fill x
    pack $w.f.e -side top -fill x -pady 4

    set pagine {}
    set dirs [hmi_staz_dirs]
    foreach d $dirs {
        foreach pg [pagine_di $d] {
            lappend pagine [list [lindex $pg 0] [lindex $pg 1] $d]
        }
    }
    frame $w.f.l
    listbox $w.f.l.lb -height 10 -width 64 -font {Courier 10} \
        -yscrollcommand [list $w.f.l.sb set]
    scrollbar $w.f.l.sb -command [list $w.f.l.lb yview]
    pack $w.f.l.sb -side right -fill y
    pack $w.f.l.lb -side left -fill both -expand 1
    pack $w.f.l -side top -fill both -expand 1
    foreach pg $pagine {
        lassign $pg nome descr d
        set riga [format "%-10s %s" $nome $descr]
        if {[llength $dirs] > 1} { append riga "  \[[file tail $d]\]" }
        $w.f.l.lb insert end $riga
    }
    if {[llength $pagine] == 0} {
        set nota "No r02.dat found (simulation, current simulator, tasks of the area):\nthe page name cannot be checked."
    } else {
        set nota "[llength $pagine] pages in [llength $dirs] r02.dat. Click to pick, double-click to confirm."
    }
    label $w.f.n -text $nota -anchor w -justify left -foreground "#555555"
    pack $w.f.n -side top -fill x -pady 4
    bind $w.f.l.lb <<ListboxSelect>> [list hmi_pagina_scelta $w.f.l.lb]
    bind $w.f.l.lb <Double-1> [list hmi_pagina_ok $c $item $w]

    set ::hmi_pag_elenco {}
    foreach pg $pagine { lappend ::hmi_pag_elenco [lindex $pg 0] }

    frame $w.b
    pack $w.b -pady 6
    button $w.b.ok -text OK -width 8 -default active -command [list hmi_pagina_ok $c $item $w]
    button $w.b.rm -text Remove -width 8 -command [list hmi_togli $c $item $w staz]
    button $w.b.no -text Cancel -width 8 -command [list destroy $w]
    pack $w.b.ok $w.b.rm $w.b.no -side left -padx 5
    bind $w <Return> [list hmi_pagina_ok $c $item $w]
    bind $w <Escape> [list destroy $w]
    focus $w.f.e
    $w.f.e selection range 0 end
    catch {grab $w}
}

proc hmi_pagina_scelta {lb} {
    set sel [$lb curselection]
    if {[llength $sel]} {
        set ::hmi_pag_nome [lindex $::hmi_pag_elenco [lindex $sel 0]]
    }
}

proc hmi_pagina_ok {c item w} {
    set nome [string toupper [string trim $::hmi_pag_nome]]
    if {$nome eq ""} {
        tk_messageBox -parent $w -icon warning -title "Faceplate page" \
            -message "Insert a page name, or use Remove."
        return
    }
    if {[llength $::hmi_pag_elenco] > 0 \
        && [lsearch -exact [string toupper $::hmi_pag_elenco] $nome] < 0} {
        if {[tk_messageBox -parent $w -icon warning -type yesno -default no \
                 -title "Faceplate page" -message \
                 "Page $nome is not in the r02.dat files found.\n\nUse it anyway?"] ne "yes"} {
            return
        }
    }
    if {![hmi_salva_assegnazione $w [hmi_inst $c $item] $nome F]} return
    catch {grab release $w}
    destroy $w
    hmi_dopo_assegnazione $c $item
}

#  Remove: toglie l'assegnazione.
proc hmi_togli {c item w tipo} {
    if {![hmi_salva_assegnazione $w [hmi_inst $c $item] "" ""]} return
    catch {grab release $w}
    destroy $w
    hmi_dopo_assegnazione $c $item
}

#  Gli elenchi dei dialoghi hanno bisogno delle variabili del modello. Nel tab
#  del disegno (Model Topology) il F01 non e' caricato - lo carica il tab dei
#  dati - ma se accanto al modello c'e' gia' un f01.dat lo si legge qui, senza
#  ricostruire niente: cad_crealg1 riscrive i file della task e ci mette
#  secondi. Se il file non c'e', gli elenchi restano vuoti e i dialoghi lo
#  dicono.
proc hmi_assicura_variabili {} {
    if {[array size ::tipVarMod] > 0} return
    if {![info exists ::curFileName]} return
    if {$::curFileName eq "" || $::curFileName eq "untitled" || $::curFileName eq "-"} return
    set dir [file dirname $::curFileName]
    if {![file isfile [file join $dir f01.dat]]} return
    set old [pwd]
    if {[catch {cd $dir}]} return
    catch {readF01}
    catch {cd $old}
}

#  Si possono elencare le variabili del modello? Se no (niente f01.dat accanto
#  al modello, per esempio una task mai compilata) all'inserimento non si
#  chiede niente: l'elemento resta con "?" e la variabile si assegna piu'
#  tardi, dal tasto destro o in Show Value.
proc hmi_variabili_disponibili {} {
    hmi_assicura_variabili
    return [expr {[array size ::tipVarMod] > 0}]
}

#  Le variabili di ingresso del modello caricato, ordinate. Vuota se il F01
#  non e' caricato e non si e' potuto leggere.
proc hmi_variabili_in {} {
    hmi_assicura_variabili
    set out {}
    foreach n [array names ::tipVarMod] {
        if {$::tipVarMod($n) eq "IN"} { lappend out $n }
    }
    return [lsort $out]
}

#  Dialogo: variabile a cui inviare valori. Deve essere un ingresso (IN): su
#  una variabile calcolata il modello riscrive il valore al passo dopo.
proc hmi_dialogo_variabile {c item} {
    set inst [hmi_inst $c $item]
    set w .hmi_variabile
    catch {destroy $w}
    toplevel $w
    wm title $w "Variable to set"
    catch {wm transient $w [winfo toplevel $c]}
    set ::hmi_var_nome [hmi_valore $inst set file]
    set ::hmi_var_tutte [hmi_variabili_in]

    frame $w.f
    pack $w.f -padx 10 -pady 8 -fill both -expand 1
    label $w.f.i -text "Element $inst - input variable (IN) to send values to:" -anchor w
    entry $w.f.e -textvariable ::hmi_var_nome -width 20
    pack $w.f.i -side top -fill x
    pack $w.f.e -side top -fill x -pady 4
    hmi_lista_variabili $w.f.l $w.f.e ::hmi_var_nome $::hmi_var_tutte \
        [list hmi_variabile_ok $c $item $w]
    pack $w.f.l -side top -fill both -expand 1
    if {[llength $::hmi_var_tutte] == 0} {
        set nota "The model variables are not loaded: the name cannot be checked."
    } else {
        set nota "[llength $::hmi_var_tutte] input variables. Type to filter, click to pick, double-click to confirm."
    }
    label $w.f.n -text $nota -anchor w -justify left -foreground "#555555"
    pack $w.f.n -side top -fill x -pady 4


    frame $w.b
    pack $w.b -pady 6
    button $w.b.ok -text OK -width 8 -default active -command [list hmi_variabile_ok $c $item $w]
    button $w.b.rm -text Remove -width 8 -command [list hmi_togli $c $item $w set]
    button $w.b.no -text Cancel -width 8 -command [list destroy $w]
    pack $w.b.ok $w.b.rm $w.b.no -side left -padx 5
    bind $w <Return> [list hmi_variabile_ok $c $item $w]
    bind $w <Escape> [list destroy $w]
    focus $w.f.e
    $w.f.e selection range 0 end
    catch {grab $w}
}

#  Tutte le variabili del modello caricato, ordinate. Vuota se il F01 non e'
#  caricato.
proc hmi_variabili_tutte {} {
    hmi_assicura_variabili
    return [lsort [array names ::tipVarMod]]
}

#  Elenco filtrabile di variabili, lo stesso in tutti i dialoghi che ne
#  scelgono una (Variable to set, e Variabile da animare dei display @val_0):
#  scrivendo nel campo di testo l'elenco si restringe ai nomi che contengono
#  quel testo, un clic su una riga ne mette il nome nel campo, il doppio clic
#  conferma.
#    f      frame da creare, con l'elenco e la sua barra (lo impacchetta il
#           chiamante)
#    entry  il campo di testo del dialogo
#    testo  la variabile globale legata al campo (es. ::hmi_var_nome)
#    nomi   le variabili da elencare
#    ok     il comando del doppio clic
#    tipo   1 per mostrare anche il tipo (IN, US, UA)
#  Ritorna la listbox.
proc hmi_lista_variabili {f entry testo nomi ok {tipo 0}} {
    frame $f
    listbox $f.lb -height 12 -width 64 -font {Courier 10} \
        -yscrollcommand [list $f.sb set]
    scrollbar $f.sb -command [list $f.lb yview]
    pack $f.sb -side right -fill y
    pack $f.lb -side left -fill both -expand 1
    set ::hmi_lista_nomi($f.lb) $nomi
    set ::hmi_lista_opz($f.lb) [list $testo $tipo]
    bind $f.lb <<ListboxSelect>> [list hmi_lista_scelta $f.lb]
    bind $f.lb <Double-1> $ok
    bind $f.lb <Destroy> [list hmi_lista_via $f.lb]
    bind $entry <KeyRelease> [list hmi_lista_filtra $f.lb]
    hmi_lista_filtra $f.lb
    return $f.lb
}

#  Riempie l'elenco con le variabili il cui nome contiene il testo scritto.
proc hmi_lista_filtra {lb} {
    lassign $::hmi_lista_opz($lb) testo tipo
    upvar #0 $testo valore
    set filtro [string toupper [string trim $valore]]
    $lb delete 0 end
    set mostrati {}
    foreach n $::hmi_lista_nomi($lb) {
        if {$filtro ne "" && [string first $filtro $n] < 0} continue
        lappend mostrati $n
        if {$tipo} {
            set t [expr {[info exists ::tipVarMod($n)] ? $::tipVarMod($n) : ""}]
            $lb insert end [format "%-10s %-3s %s" $n $t [hmi_descrizione $n]]
        } else {
            $lb insert end [format "%-10s %s" $n [hmi_descrizione $n]]
        }
    }
    set ::hmi_lista_mostrati($lb) $mostrati
}

#  Clic su una riga: il suo nome va nel campo di testo.
proc hmi_lista_scelta {lb} {
    lassign $::hmi_lista_opz($lb) testo tipo
    upvar #0 $testo valore
    set sel [$lb curselection]
    if {[llength $sel]} {
        set valore [lindex $::hmi_lista_mostrati($lb) [lindex $sel 0]]
    }
}

proc hmi_lista_via {lb} {
    unset -nocomplain ::hmi_lista_nomi($lb) ::hmi_lista_opz($lb) ::hmi_lista_mostrati($lb)
}

proc hmi_variabile_ok {c item w} {
    set var [string toupper [string trim $::hmi_var_nome]]
    if {$var eq ""} {
        tk_messageBox -parent $w -icon warning -title "Variable to set" \
            -message "Insert a variable name, or use Remove."
        return
    }
    if {[array size ::tipVarMod] > 0} {
        if {![info exists ::tipVarMod($var)]} {
            tk_messageBox -parent $w -icon error -title "Variable to set" \
                -message "Variable $var is not in the model."
            return
        }
        if {$::tipVarMod($var) ne "IN"} {
            tk_messageBox -parent $w -icon error -title "Variable to set" -message \
                "Variable $var is not an input (it is $::tipVarMod($var)).\n\nThe model computes it: a value sent to it would be overwritten at the next step."
            return
        }
    }
    if {![hmi_salva_assegnazione $w [hmi_inst $c $item] $var S]} return
    catch {grab release $w}
    destroy $w
    hmi_dopo_assegnazione $c $item
}

# --------------------------------------------------------------------------
# Invio dei valori
# --------------------------------------------------------------------------

#  Dialogo di invio, non bloccante e uno per elemento: resta aperto per
#  mandare piu' valori, e mostra il valore attuale aggiornato.
#    Send            scrittura diretta nel DB punti (viewval VAR -f valore),
#                    registrata nel registro delle scritture
#    Perturbation... pannello xaing (gradino, rampa...), applicato dallo
#                    scheduler
#  Il valore si scrive nelle unita' mostrate e si converte in quelle interne.
#
#  Finestra e window manager (WSLg/XWayland in particolare):
#   - il dialogo e' TRANSIENT della finestra dello schema: resta sopra di lei
#     anche quando si clicca lo schema, e non ha un'icona sua da cui non si
#     riesca a ripristinarlo;
#   - si mostra solo quando e' pronto, vicino al puntatore;
#   - se e' gia' aperto (magari nascosto o iconizzato) lo si ritira e lo si
#     rimostra: raise e deiconify da soli su quei window manager spesso non
#     fanno niente, e il dialogo sembrava non aprirsi piu';
#   - il suo aggiornamento periodico e' UNO solo e si ferma con la finestra.
proc hmi_dialogo_set {c item {X ""} {Y ""}} {
    set inst [hmi_inst $c $item]
    set var [hmi_valore $inst set]
    if {$var eq ""} { set var [hmi_valore $inst set file] }
    if {$var eq ""} return
    set w .hmi_set_[string map {. _} $inst]
    if {[winfo exists $w]} {
        hmi_log_scrivi "set value $var: dialog already open (state [wm state $w]), shown again"
        hmi_in_primo_piano $w $X $Y
        return
    }
    toplevel $w
    wm withdraw $w
    wm title $w "Set value - $var"
    wm resizable $w 0 0
    catch {wm transient $w [winfo toplevel $c]}
    wm protocol $w WM_DELETE_WINDOW [list destroy $w]
    bind $w <Destroy> [list hmi_set_chiuso $w %W]
    set ::hmi_set_val($w) ""
    set ::hmi_set_msg($w) ""

    frame $w.f
    pack $w.f -padx 10 -pady 8
    label $w.f.l1 -text "Variable:" -anchor w
    label $w.f.v1 -text "$var  [hmi_descrizione $var]" -anchor w
    label $w.f.l2 -text "Current:" -anchor w
    label $w.f.v2 -text "--" -anchor w -width 24
    label $w.f.l3 -text "New value:" -anchor w
    entry $w.f.e -textvariable ::hmi_set_val($w) -width 14
    label $w.f.u -text [ret_umis $var] -anchor w
    grid $w.f.l1 $w.f.v1 - -sticky w -pady 2
    grid $w.f.l2 $w.f.v2 - -sticky w -pady 2
    grid $w.f.l3 $w.f.e $w.f.u -sticky w -pady 2
    label $w.m -textvariable ::hmi_set_msg($w) -anchor w -foreground "#555555"
    pack $w.m -fill x -padx 10

    frame $w.b
    pack $w.b -pady 6
    button $w.b.send -text Send -width 8 -default active \
        -command [list hmi_invia $w $var]
    button $w.b.pert -text "Perturbation..." -command [list hmi_perturba $w $var]
    button $w.b.close -text Close -width 8 -command [list destroy $w]
    pack $w.b.send $w.b.pert $w.b.close -side left -padx 5
    bind $w <Return> [list hmi_invia $w $var]
    bind $w <Escape> [list destroy $w]
    hmi_log_scrivi "set value $var: dialog opened"
    hmi_in_primo_piano $w $X $Y
    hmi_set_tick $w $var
}

#  Mostra il dialogo in cima, vicino al puntatore se si sa dov'e'. Ritirarlo e
#  rimostrarlo produce una nuova richiesta di mappa, che il window manager
#  esegue sempre sopra le altre finestre; raise da solo non basta.
proc hmi_in_primo_piano {w {X ""} {Y ""}} {
    wm withdraw $w
    if {[string is integer -strict $X] && [string is integer -strict $Y]} {
        wm geometry $w +[expr {max(0, $X + 12)}]+[expr {max(0, $Y + 12)}]
    }
    wm deiconify $w
    raise $w
    catch {focus -force $w.f.e}
}

#  Chiusura del dialogo: si ferma il suo aggiornamento. <Destroy> arriva anche
#  per ogni figlio della finestra, per questo il confronto con %W.
proc hmi_set_chiuso {w W} {
    if {$W ne $w} return
    if {[info exists ::hmi_set_after($w)]} {
        after cancel $::hmi_set_after($w)
        unset ::hmi_set_after($w)
    }
}

#  Ogni secondo: valore attuale e stato dei bottoni.
proc hmi_set_tick {w var} {
    if {![winfo exists $w]} return
    set live [hmi_live]
    set stato [expr {$live ? "normal" : "disabled"}]
    $w.b.send configure -state $stato
    $w.b.pert configure -state $stato
    if {$live} {
        anima leggi "$var\n"
        if {$::line_anim eq "" || [regexp -nocase {^ERRORE} $::line_anim]} {
            $w.f.v2 configure -text "--"
        } else {
            $w.f.v2 configure -text "[lindex $::line_anim 0] [lindex $::line_anim 1]"
        }
        if {$::hmi_set_msg($w) eq "Simulation not running."} { set ::hmi_set_msg($w) "" }
    } else {
        $w.f.v2 configure -text "--"
        set ::hmi_set_msg($w) "Simulation not running."
    }
    set ::hmi_set_after($w) [after 1000 [list hmi_set_tick $w $var]]
}

#  Dove si registrano le scritture: nella directory della simulazione, cosi'
#  il registro resta con lei; se non e' scrivibile, in /tmp.
proc hmi_registro {} {
    set d [hmi_sim_dir]
    if {$d eq ""} { set d [pwd] }
    if {[file writable $d]} { return [file join $d hmi_setvalue.log] }
    return /tmp/legopc_hmi_setvalue.log
}

#  Send: converte e scrive con viewval -f. viewval esce con 0 anche quando la
#  simulazione non c'e', quindi lo stato si controlla PRIMA; l'effetto si vede
#  nel valore attuale del dialogo.
proc hmi_invia {w var} {
    if {![winfo exists $w]} return
    if {[$w.b.send cget -state] eq "disabled" || ![hmi_live]} {
        set ::hmi_set_msg($w) "Simulation not running."
        hmi_log_scrivi "set value $var: not sent, simulation not running"
        return
    }
    set vis [string trim $::hmi_set_val($w)]
    if {![string is double -strict $vis]} {
        set ::hmi_set_msg($w) "Not a number: '$vis' (use a point for decimals)"
        hmi_log_scrivi "set value $var: not sent, not a number '$vis'"
        return
    }
    #  unita' mostrate -> interne: visuale = A*interno + B
    set int $vis
    set umis [ret_umis $var]
    set let [string index [string toupper $var] 0]
    if {[info exists ::umis_tab($let)]} {
        lassign $::umis_tab($let) um A B
        if {[string is double -strict $A] && $A != 0} {
            set int [format %.9g [expr {($vis - $B) / $A}]]
        }
    }
    set reg [hmi_registro]
    catch {
        set fd [open $reg a]
        puts $fd "# [clock format [clock seconds] -format {%Y-%m-%d %H:%M:%S}]  [hmi_app]: $var <- $vis $umis (internal $int)"
        close $fd
    }
    set old [pwd]
    set d [hmi_sim_dir]
    if {$d ne ""} { catch {cd $d} }
    set rc [catch {exec [hmi_cmd viewval] $var -f $int -l $reg >>$::hmi_log 2>@1 &} err]
    catch {cd $old}
    if {$rc} {
        hmi_log_scrivi "viewval -f $var: $err"
        set ::hmi_set_msg($w) "Send failed (see $::hmi_log)."
        return
    }
    hmi_log_scrivi "set value $var <- $vis $umis (internal $int): sent, register $reg"
    set ::hmi_set_msg($w) "Sent $vis $umis at [clock format [clock seconds] -format %H:%M:%S]."
}

#  Perturbation...: il pannello xaing, come il Command Mode di draw2gr. Va
#  lanciato nella directory della simulazione: legge variabili.rtf dalla cwd.
proc hmi_perturba {w var} {
    if {![hmi_live]} {
        set ::hmi_set_msg($w) "Simulation not running."
        return
    }
    set xaing [hmi_cmd xaing]
    set reg [hmi_registro]
    catch {
        set fd [open $reg a]
        puts $fd "# [clock format [clock seconds] -format {%Y-%m-%d %H:%M:%S}]  [hmi_app]: $var perturbation panel (xaing)"
        close $fd
    }
    set old [pwd]
    set d [hmi_sim_dir]
    if {$d ne ""} { catch {cd $d} }
    set rc [catch {exec $xaing 3 $var >>$::hmi_log 2>@1 &} err]
    catch {cd $old}
    if {$rc} {
        hmi_log_scrivi "xaing 3 $var: $err"
        set ::hmi_set_msg($w) "Perturbation panel not available (see $::hmi_log)."
        return
    }
    set ::hmi_set_msg($w) "Perturbation panel requested for $var."
}
