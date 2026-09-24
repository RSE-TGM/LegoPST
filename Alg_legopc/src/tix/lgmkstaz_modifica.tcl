# lgmkstaz_modifica.tcl - le mutazioni del modello che servono all'editing
# (Fase 2): creare/spostare/cancellare stazioni, creare pagine, la
# conversione celle<->pixel (con il capovolgimento verticale di xstaz, vedi
# sotto) usata sia da chi disegna sia da chi piazza. Tcl puro, nessun Tk:
# testabile con tclsh, come lgmkstaz_leggi.tcl/lgmkstaz_scrivi.tcl.
#
# Non scrive su disco (quello resta lgmkstaz_scrivi.tcl) e non sa niente di
# canvas o finestre (quello resta lgmkstaz.tcl): qui c'e' solo "il modello
# dopo la modifica", una funzione pura del modello prima e degli argomenti
# della modifica.

# ---------------------------------------------------------------------------
# Geometria: celle <-> pixel, con l'origine in BASSO a sinistra come xstaz.
#
# xstaz disegna con Y capovolta rispetto all'ALTEZZA DELLA PAGINA: una
# stazione con POSIZIONE piccola sta vicino al basso, non in cima (verificato
# nel sorgente vero, Alg_rt/grafica/xstaz/cnewstaz.c: "ydraw = altezza_pagina
# - posiy0*DIM_UNITSTAZ - altezza_stazione*DIM_UNITSTAZ" prima di passarlo a
# Motif come XmNy). L'ALTEZZA DELLA PAGINA e' quella che compstaz chiama
# posmy: il massimo (posy+altezza) fra le stazioni della pagina
# (Alg_rt/grafica/compstaz/compstaz.c, fill_pagina). Qui e' un parametro
# esplicito ($altezza_pagina_celle), non una variabile globale: la vista lo
# ricalcola a ogni pagina disegnata (vedi lgmkstaz.tcl, altezza_pagina_celle).
# ---------------------------------------------------------------------------

#  Altezza della pagina in celle: il massimo posy+altezza fra le sue
#  stazioni (posmy di compstaz). Con nessuna stazione, 1 (una pagina vuota
#  ha comunque bisogno di un'altezza per disegnare la griglia).
proc ::lgmkstaz::altezza_pagina {stazioni_qui} {
    set h 1
    foreach s $stazioni_qui {
        set fondo [expr {[dict get $s posy] + [dict get $s altezza]}]
        if {$fondo > $h} { set h $fondo }
    }
    return $h
}

#  Il riquadro in pixel (x0 y0 x1 y1) di una stazione a POSIZIONE
#  ($posx,$posy), grande ($larg,$altezza) celle, sulla pagina alta
#  $altezza_pagina_celle celle.
proc ::lgmkstaz::riquadro_celle {posx posy larg altezza altezza_pagina_celle} {
    variable dim_cella_px
    set x0 [expr {$posx * $dim_cella_px}]
    set y0 [expr {($altezza_pagina_celle - $posy - $altezza) * $dim_cella_px}]
    set x1 [expr {$x0 + $larg * $dim_cella_px}]
    set y1 [expr {$y0 + $altezza * $dim_cella_px}]
    return [list $x0 $y0 $x1 $y1]
}

#  L'inverso: da un punto in pixel del canvas (dove l'utente ha cliccato) alla
#  cella POSIZIONE piu' vicina - lo spigolo in alto a sinistra del riquadro
#  che ci si piazzerebbe, di dimensione ($larg,$altezza) celle, agganciato
#  alla griglia. Non impedisce valori negativi: chi chiama decide se
#  accettarli (POSIZIONE negativa non e' valida - vedi valida_posizione).
proc ::lgmkstaz::cella_da_pixel {x_px y_px larg altezza altezza_pagina_celle} {
    variable dim_cella_px
    set posx [expr {int(round(double($x_px) / $dim_cella_px))}]
    #  L'inverso di riquadro_celle: y0 = (H - posy - altezza)*d  =>
    #  posy = H - altezza - y0/d
    set cella_y [expr {int(round(double($y_px) / $dim_cella_px))}]
    set posy [expr {$altezza_pagina_celle - $altezza - $cella_y}]
    return [list $posx $posy]
}

#  Vero se il riquadro (posx,posy,larg,altezza) si sovrappone a quello di
#  un'altra stazione della STESSA pagina ($escludi_id la si esclude dal
#  confronto: serve muovendo una stazione, che altrimenti si troverebbe
#  sovrapposta a se stessa). Solo un avviso in lgmkstaz (vedi
#  ::lgmkstaz::piazza_stazione): compstaz non sembra controllarlo, quindi
#  non e' detto sia davvero vietato - ma due stazioni nello stesso posto
#  sulla pagina vera si sovrapporrebbero anche visivamente, quindi vale la
#  pena dirlo.
proc ::lgmkstaz::stazioni_sovrapposte {stazioni_pagina posx posy larg altezza escludi_id} {
    set x1 [expr {$posx + $larg}]
    set y1 [expr {$posy + $altezza}]
    foreach s $stazioni_pagina {
        if {[dict get $s id] == $escludi_id} { continue }
        set ax0 [dict get $s posx]
        set ay0 [dict get $s posy]
        set ax1 [expr {$ax0 + [dict get $s larg]}]
        set ay1 [expr {$ay0 + [dict get $s altezza]}]
        #  due rettangoli NON si sovrappongono se uno sta tutto a sinistra,
        #  a destra, sopra o sotto l'altro; sovrapposti altrimenti.
        if {$posx >= $ax1 || $ax0 >= $x1 || $posy >= $ay1 || $ay0 >= $y1} { continue }
        return 1
    }
    return 0
}

# ---------------------------------------------------------------------------
# Costruzione di una stazione/oggetto vuoti, dal catalogo - la stessa
# grammatica che leggi_oggetto/scrivi_oggetto usano per un file, qui usata al
# contrario: non leggere un file esistente, ma inventare i valori di
# default di un oggetto nuovo.
# ---------------------------------------------------------------------------

#  Il valore "vuoto" di un campo, secondo il suo nome di grammatica (vedi
#  ::lgmkstaz::grammatica in lgmkstaz_dati.tcl). COLORE non puo' restare
#  vuoto (compstaz lo rifiuta sempre): il default e' NERO, il primo della
#  lista dei colori ammessi.
proc ::lgmkstaz::valore_vuoto_campo {nome_campo} {
    variable colori
    switch -- $nome_campo {
        colore          { return [lindex $colori 0] }
        etichetta       { return "" }
        input - input_err - inibizione - input_neg - input_blink_neg {
            return [dict create var "" mod "" not 0]
        }
        output          { return [dict create var "" mod "" modo "" valore ""] }
        scalamento - offset - scalamento_err { return "0" }
        minmax - minmax_err { return [list 0 100] }
        default {
            error "valore_vuoto_campo: campo di grammatica sconosciuto '$nome_campo'\
                   (errore interno di lgmkstaz)"
        }
    }
}

#  Un oggetto vuoto del tipo dato (LED, PULSANTE, INDICATORE_ERR, ...).
proc ::lgmkstaz::crea_oggetto_vuoto {tipo_grammatica} {
    variable grammatica
    if {![info exists grammatica($tipo_grammatica)]} {
        error "crea_oggetto_vuoto: tipo di oggetto sconosciuto '$tipo_grammatica'"
    }
    set campi {}
    foreach nome_campo $grammatica($tipo_grammatica) {
        lappend campi [dict create nome $nome_campo valore [valore_vuoto_campo $nome_campo]]
    }
    return [dict create tipo $tipo_grammatica campi $campi]
}

#  Una stazione vuota del tipo dato (deve essere nel catalogo: le storiche
#  non si possono CREARE con lgmkstaz, solo spostare/cancellare se gia' nel
#  file - vedi il commento in lgmkstaz_leggi.tcl su leggi_stazione). $id va
#  dato da chi chiama (::lgmkstaz::prossimo_id, nella GUI).
proc ::lgmkstaz::crea_stazione_vuota {id tipo pagina posx posy} {
    variable catalogo
    if {![info exists catalogo($tipo)]} {
        error "crea_stazione_vuota: '$tipo' non e' nel catalogo (solo i tipi\
               nuovi si possono piazzare, non quelli storici)"
    }
    lassign $catalogo($tipo) larg altezza sequenza
    set oggetti {}
    foreach tipo_oggetto $sequenza {
        lappend oggetti [crea_oggetto_vuoto $tipo_oggetto]
    }
    return [dict create id $id numero "" tipo $tipo descrizione "" pagina $pagina \
                posx $posx posy $posy larg $larg altezza $altezza \
                grezzo 0 oggetti $oggetti]
}

# ---------------------------------------------------------------------------
# Mutazioni del modello: ognuna prende il modello e ritorna quello nuovo (il
# modello e' un dict, in Tcl un valore come un altro - non c'e' bisogno di
# passarlo per riferimento). Chi chiama (la GUI) fa
# "set ::lgmkstaz::modello [aggiungi_stazione $::lgmkstaz::modello ...]".
# ---------------------------------------------------------------------------

proc ::lgmkstaz::modello_aggiungi_stazione {modello stazione} {
    dict lappend modello stazioni
    #  dict lappend crea la chiave se manca ma lascia un elemento vuoto in
    #  piu' alla primissima stazione di un file nuovo: si ripulisce cosi'
    #  invece di complicare la lettura con un caso speciale.
    set lista [dict get $modello stazioni]
    if {[llength $lista] > 0 && [lindex $lista end] eq ""} {
        set lista [lreplace $lista end end]
    }
    lappend lista $stazione
    dict set modello stazioni $lista
    return $modello
}

#  L'indice (in "stazioni") della stazione con quell'id, o -1.
proc ::lgmkstaz::modello_indice_stazione {modello id} {
    set stazioni [dict get $modello stazioni]
    for {set i 0} {$i < [llength $stazioni]} {incr i} {
        if {[dict get [lindex $stazioni $i] id] == $id} { return $i }
    }
    return -1
}

proc ::lgmkstaz::modello_stazione_per_id {modello id} {
    set i [modello_indice_stazione $modello $id]
    if {$i < 0} { error "nessuna stazione con id $id" }
    return [lindex [dict get $modello stazioni] $i]
}

proc ::lgmkstaz::modello_elimina_stazione {modello id} {
    set i [modello_indice_stazione $modello $id]
    if {$i < 0} { error "nessuna stazione con id $id" }
    dict set modello stazioni [lreplace [dict get $modello stazioni] $i $i]
    return $modello
}

#  Sposta la stazione $id alla nuova cella (posx,posy) - non tocca nient'altro.
proc ::lgmkstaz::modello_sposta_stazione {modello id posx posy} {
    set i [modello_indice_stazione $modello $id]
    if {$i < 0} { error "nessuna stazione con id $id" }
    set stazioni [dict get $modello stazioni]
    set s [lindex $stazioni $i]
    dict set s posx $posx
    dict set s posy $posy
    dict set modello stazioni [lreplace $stazioni $i $i $s]
    return $modello
}

#  Sostituisce per intero la stazione $id (usato dal pannello proprieta',
#  che modifica descrizione/oggetti insieme e li scrive tutti in una volta).
proc ::lgmkstaz::modello_sostituisci_stazione {modello id nuova} {
    set i [modello_indice_stazione $modello $id]
    if {$i < 0} { error "nessuna stazione con id $id" }
    dict set modello stazioni [lreplace [dict get $modello stazioni] $i $i $nuova]
    return $modello
}

proc ::lgmkstaz::modello_indice_pagina {modello numero} {
    set pagine [dict get $modello pagine]
    for {set i 0} {$i < [llength $pagine]} {incr i} {
        if {[dict get [lindex $pagine $i] numero] == $numero} { return $i }
    }
    return -1
}

proc ::lgmkstaz::modello_aggiungi_pagina {modello pagina} {
    dict lappend modello pagine
    set lista [dict get $modello pagine]
    if {[llength $lista] > 0 && [lindex $lista end] eq ""} {
        set lista [lreplace $lista end end]
    }
    lappend lista $pagina
    dict set modello pagine $lista
    return $modello
}

proc ::lgmkstaz::modello_sostituisci_pagina {modello numero nuova} {
    set i [modello_indice_pagina $modello $numero]
    if {$i < 0} { error "nessuna pagina numero $numero" }
    dict set modello pagine [lreplace [dict get $modello pagine] $i $i $nuova]
    return $modello
}

# ---------------------------------------------------------------------------
# Validazione di una pagina nuova o modificata - le stesse regole di
# leggi_pagina (lgmkstaz_leggi.tcl), qui applicate a un NUMERO/NOME/
# DESCRIZIONE dati dall'utente invece che letti da un file. $numero_da_escludere
# e' il numero della pagina che si sta modificando (per non rifiutarla perche'
# "gia' usata" da se stessa); "" per una pagina nuova.
#  Toglie una pagina E le stazioni che ci stanno sopra. Le due cose vanno
#  insieme: una stazione che cita una pagina inesistente e' un file che il
#  parser rifiuta ("la stazione N cita la pagina P, mai dichiarata"), quindi
#  lasciarle orfane produrrebbe un r01.dat che lgmkstaz stesso non rilegge.
proc ::lgmkstaz::modello_elimina_pagina {modello numero} {
    set pagine {}
    foreach p [dict get $modello pagine] {
        if {[dict get $p numero] != $numero} { lappend pagine $p }
    }
    set stazioni {}
    foreach s [dict get $modello stazioni] {
        if {[dict get $s pagina] != $numero} { lappend stazioni $s }
    }
    dict set modello pagine $pagine
    dict set modello stazioni $stazioni
    return $modello
}

#  Quante stazioni stanno su una pagina: serve a dire all'utente cosa sta per
#  perdere prima di cancellarla.
proc ::lgmkstaz::modello_stazioni_pagina {modello numero} {
    set n 0
    foreach s [dict get $modello stazioni] {
        if {[dict get $s pagina] == $numero} { incr n }
    }
    return $n
}

proc ::lgmkstaz::valida_pagina {modello numero nome descrizione numero_da_escludere} {
    variable max_pagine
    variable lun_nome_pagina
    variable lun_descrizione_pagina

    if {![string is integer -strict $numero] || $numero < 1 || $numero > $max_pagine} {
        error "il numero pagina deve essere un intero fra 1 e $max_pagine"
    }
    if {$numero != $numero_da_escludere} {
        foreach p [dict get $modello pagine] {
            if {[dict get $p numero] == $numero} {
                error "il numero pagina $numero e' gia' usato"
            }
        }
    }
    if {$nome eq ""} { error "il nome della pagina e' obbligatorio" }
    if {[string length $nome] > $lun_nome_pagina} {
        error "il nome '$nome' supera $lun_nome_pagina caratteri"
    }
    if {[regexp {[ \t]} $nome]} {
        error "il nome della pagina non puo' contenere spazi (e' un token del file)"
    }
    if {$descrizione eq ""} { error "la descrizione della pagina e' obbligatoria" }
    if {[string length $descrizione] > $lun_descrizione_pagina} {
        error "la descrizione supera $lun_descrizione_pagina caratteri"
    }
}

#  Validazione di un COLORE dato dall'utente (pannello proprieta').
proc ::lgmkstaz::valida_colore {nome} {
    variable colori
    if {$nome ni $colori} {
        error "colore non ammesso: '$nome' (validi: [join $colori {, }])"
    }
}

#  Validazione di un MODO di perturbazione (OUTPUT) dato dall'utente.
proc ::lgmkstaz::valida_perturbazione {modo} {
    variable perturbazioni
    if {$modo ni $perturbazioni} {
        error "modo di perturbazione non ammesso: '$modo' (validi: [join $perturbazioni {, }])"
    }
}
