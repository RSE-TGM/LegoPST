# lgmkstaz_leggi.tcl - legge un r01.dat e produce il modello in memoria che
# lgmkstaz_scrivi.tcl sa riscrivere e che una futura vista a canvas sa
# disegnare. Tcl puro (nessun Tk): si prova con tclsh, senza display.
#
# Rispecchia il PARSER VERO (Alg_rt/grafica/compstaz/compstaz.c e cnewstaz.c),
# non un'interpretazione della documentazione: l'ordine delle letture, le
# parole chiave e i limiti sono presi da li' (vedi le note in
# lgmkstaz_dati.tcl). Dove compstaz e' silenziosamente permissivo o rischia un
# comportamento indefinito (riga CRLF, riga oltre 78 caratteri, NUMERO di
# pagina fuori 1..500), lgmkstaz e' piu' severo e lo dice: e' il livello 1 di
# validazione del piano (i controlli strutturali immediati, prima ancora di
# chiamare compstaz per il livello 2, quello vero).
#
# Richiede lgmkstaz_dati.tcl gia' sorgiato (le tabelle di grammatica/catalogo).

namespace eval ::lgmkstaz {
    variable rl_righe {}
    variable rl_nriga 0
}

# ---------------------------------------------------------------------------
# Lettore di righe grezze: l'equivalente di legge_riga()+lungh() in
# co_legge.c/co_lungh.c.
# ---------------------------------------------------------------------------

#  Prepara il lettore su un testo gia' in memoria (l'intero contenuto del
#  file). Non tocca il disco: lo fa leggi_file, che chiama questa.
proc ::lgmkstaz::rl_avvia {testo} {
    variable rl_righe
    variable rl_nriga

    set righe [split $testo "\n"]
    #  se il file finisce con \n (il caso normale) lo split lascia un
    #  elemento vuoto in coda, che non e' una riga
    if {[llength $righe] > 0 && [lindex $righe end] eq ""} {
        set righe [lrange $righe 0 end-1]
    }
    set rl_righe $righe
    set rl_nriga 0
}

#  Ritorna il numero dell'ultima riga letta (per i messaggi d'errore di chi
#  chiama).
proc ::lgmkstaz::rl_riga_corrente {} {
    variable rl_nriga
    return $rl_nriga
}

#  Consuma e ritorna la prossima riga. Solleva un errore Tcl (col numero di
#  riga nel messaggio) se il file finisce prima del previsto o se la riga
#  viola una delle regole del formato - le tre gia' note (spazio iniziale,
#  CRLF, oltre 78 caratteri) piu' quella ovvia (file finito).
proc ::lgmkstaz::rl_leggi {} {
    variable rl_righe
    variable rl_nriga

    if {[llength $rl_righe] == 0} {
        error "riga [expr {$rl_nriga + 1}]: fine del file inattesa (manca END_OF_FILE)"
    }
    set riga [lindex $rl_righe 0]
    set rl_righe [lrange $rl_righe 1 end]
    incr rl_nriga

    if {[string index $riga 0] eq " "} {
        error "riga $rl_nriga: comincia con uno spazio - vietato, compstaz la\
               rifiuta come se il file finisse li' (co_legge.c)"
    }
    if {[string index $riga end] eq "\r"} {
        error "riga $rl_nriga: finisce con \\r - il file ha terminatori CRLF,\
               non LF; salvalo in formato Unix (l'ultimo campo della riga ne\
               resterebbe sporcato, in silenzio, in compstaz)"
    }
    if {[string length $riga] > 78} {
        error "riga $rl_nriga: lunga [string length $riga] caratteri, oltre i\
               78 che compstaz legge (fgets(riga,80,...) tronca il resto SENZA\
               errore)"
    }
    return $riga
}

#  I primi $n token separati da spazi/tabulazioni (run consecutivi contano
#  come un separatore solo, come strtok in C). Mancanti in coda = "".
proc ::lgmkstaz::rl_token {riga n} {
    set t [regexp -all -inline {[^ \t]+} $riga]
    while {[llength $t] < $n} { lappend t "" }
    return [lrange $t 0 [expr {$n - 1}]]
}

#  Tutto cio' che segue la prima parola e il run di spazi/tab dopo di essa,
#  spazi interni compresi - per ETICHETTA e la DESCRIZIONE di una stazione,
#  che in C si leggono con strstr() sulla riga grezza, non parola per parola.
#  Troncato a $lung caratteri, come strncpy(...,LUNG_ETICHETTA) in C.
proc ::lgmkstaz::rl_resto_riga {riga lung} {
    if {![regexp {^[^ \t]+[ \t]+(.*)$} $riga -> resto]} {
        return ""
    }
    if {[string length $resto] > $lung} {
        set resto [string range $resto 0 [expr {$lung - 1}]]
    }
    return $resto
}

#  Un numero (intero o decimale, col punto) dove compstaz si aspetta un
#  float: qui lo si tiene STRINGA, non lo si converte - per riscriverlo tale
#  e quale se non cambia, invece di normalizzarne la forma (1. / 1.0 / 1).
#  Solo la validita' sintattica si controlla.
proc ::lgmkstaz::rl_valida_numero {testo cosa} {
    if {![string is double -strict $testo]} {
        error "$cosa: '$testo' non e' un numero valido"
    }
}

# ---------------------------------------------------------------------------
# Un riferimento a variabile+modello (le righe INPUT, INPUT_ERR, INPUT_BLINK,
# INIBIZIONE): entrambi assenti = scollegato, altrimenti servono entrambi.
# $nstr e' 3 (nessun NOT ammesso) o 4 (il NOT finale e' facoltativo).
# ---------------------------------------------------------------------------

proc ::lgmkstaz::leggi_riferimento {riga parola_chiave nstr} {
    set tok [rl_token $riga $nstr]
    set kw [lindex $tok 0]
    if {$kw ne $parola_chiave} {
        error "attesa la riga '$parola_chiave', trovata '$kw'"
    }
    set var [lindex $tok 1]
    set mod [lindex $tok 2]
    set esito [dict create var "" mod "" not 0]
    if {$var eq "" && $mod eq ""} {
        return $esito
    }
    if {$var eq "" || $mod eq ""} {
        error "'$parola_chiave' incompleta: manca la variabile o il modello"
    }
    dict set esito var $var
    dict set esito mod $mod
    if {$nstr == 4} {
        dict set esito not [expr {[lindex $tok 3] eq $::lgmkstaz::kw(not)}]
    }
    return $esito
}

# ---------------------------------------------------------------------------
# Un campo generico dell'oggetto: legge UNA riga (quasi sempre; "etichetta"
# per SELETTORE ne legge due, vedi il caso a parte) e ritorna il suo valore.
# Il chiamante (leggi_oggetto) sa a quale nome di campo associarlo.
# ---------------------------------------------------------------------------

proc ::lgmkstaz::leggi_campo {campo} {
    variable kw
    variable colori
    variable perturbazioni
    variable lun_etichetta

    switch -- $campo {
        colore {
            set riga [rl_leggi]
            set tok [rl_token $riga 2]
            if {[lindex $tok 0] ne $kw(colore)} {
                error "attesa la riga 'COLORE', trovata '[lindex $tok 0]'"
            }
            set nome [lindex $tok 1]
            if {$nome eq ""} { error "COLORE senza nome" }
            if {$nome ni $colori} { error "colore sconosciuto: '$nome'" }
            return $nome
        }
        etichetta {
            set riga [rl_leggi]
            set primo [lindex [rl_token $riga 1] 0]
            if {$primo ne $kw(etichetta)} {
                error "attesa la riga 'ETICHETTA', trovata '$primo'"
            }
            return [rl_resto_riga $riga $lun_etichetta]
        }
        input {
            return [leggi_riferimento [rl_leggi] $kw(input) 3]
        }
        input_neg {
            return [leggi_riferimento [rl_leggi] $kw(input) 4]
        }
        input_blink_neg {
            return [leggi_riferimento [rl_leggi] $kw(input_blink) 4]
        }
        input_err {
            return [leggi_riferimento [rl_leggi] $kw(input_err) 3]
        }
        inibizione {
            return [leggi_riferimento [rl_leggi] $kw(inibizione) 3]
        }
        output {
            set riga [rl_leggi]
            set tok [rl_token $riga 5]
            if {[lindex $tok 0] ne $kw(output)} {
                error "attesa la riga 'OUTPUT', trovata '[lindex $tok 0]'"
            }
            set var [lindex $tok 1]; set mod [lindex $tok 2]
            set modo [lindex $tok 3]; set valore [lindex $tok 4]
            set esito [dict create var "" mod "" modo "" valore ""]
            if {$var eq "" && $mod eq "" && $modo eq ""} {
                return $esito
            }
            if {$var eq "" || $mod eq "" || $modo eq ""} {
                error "OUTPUT incompleto: variabile, modello e modo vanno dati tutti e tre"
            }
            if {$modo ni $perturbazioni} {
                error "modo di perturbazione sconosciuto: '$modo'"
            }
            if {$valore ne ""} { rl_valida_numero $valore "valore di OUTPUT" }
            dict set esito var $var
            dict set esito mod $mod
            dict set esito modo $modo
            dict set esito valore $valore
            return $esito
        }
        scalamento   { return [leggi_campo_numerico $kw(scalamento)] }
        offset       { return [leggi_campo_numerico $kw(offset)] }
        scalamento_err { return [leggi_campo_numerico $kw(scalamento_err)] }
        minmax       { return [leggi_campo_minmax $kw(minmax)] }
        minmax_err   { return [leggi_campo_minmax $kw(minmax_err)] }
        default {
            error "campo di grammatica sconosciuto: '$campo' (errore interno di lgmkstaz)"
        }
    }
}

proc ::lgmkstaz::leggi_campo_numerico {parola_chiave} {
    set riga [rl_leggi]
    set tok [rl_token $riga 2]
    if {[lindex $tok 0] ne $parola_chiave} {
        error "attesa la riga '$parola_chiave', trovata '[lindex $tok 0]'"
    }
    set valore [lindex $tok 1]
    rl_valida_numero $valore $parola_chiave
    return $valore
}

proc ::lgmkstaz::leggi_campo_minmax {parola_chiave} {
    set riga [rl_leggi]
    set tok [rl_token $riga 3]
    if {[lindex $tok 0] ne $parola_chiave} {
        error "attesa la riga '$parola_chiave', trovata '[lindex $tok 0]'"
    }
    set minimo [lindex $tok 1]
    set massimo [lindex $tok 2]
    rl_valida_numero $minimo "$parola_chiave (minimo)"
    rl_valida_numero $massimo "$parola_chiave (massimo)"
    return [list $minimo $massimo]
}

# ---------------------------------------------------------------------------
# Un oggetto elementare: la riga che lo apre (LED, STRINGA, ...) piu' i suoi
# campi, nell'ordine dato da ::lgmkstaz::grammatica($tipo_grammatica).
# $tipo_grammatica e' gia' risolto da chi chiama (leggi_stazione): per
# INDICATORE puo' essere "INDICATORE" o "INDICATORE_ERR" a seconda del TIPO
# di stazione, ma la riga scritta nel file e' "INDICATORE" in entrambi i casi
# (vedi blocco_kw in lgmkstaz_dati.tcl).
# ---------------------------------------------------------------------------

proc ::lgmkstaz::leggi_oggetto {tipo_grammatica} {
    variable grammatica
    variable blocco_kw

    set intestazione [lindex [rl_token [rl_leggi] 1] 0]
    set atteso $blocco_kw($tipo_grammatica)
    if {$intestazione ne $atteso} {
        error "atteso l'oggetto '$atteso', trovato '$intestazione'\
               (oggetto previsto dal tipo di stazione, nell'ordine esatto)"
    }

    set campi {}
    foreach nome_campo $grammatica($tipo_grammatica) {
        set valore [leggi_campo $nome_campo]
        lappend campi [dict create nome $nome_campo valore $valore]
    }
    return [dict create tipo $tipo_grammatica campi $campi]
}

# ---------------------------------------------------------------------------
# Il record PAGINA.
# ---------------------------------------------------------------------------

proc ::lgmkstaz::leggi_pagina {} {
    variable max_pagine
    variable lun_nome_pagina
    variable lun_descrizione_pagina

    #  NUMERO: obbligatorio e numerico (a differenza del NUMERO di STAZIONE,
    #  che compstaz ignora del tutto). compstaz non controlla che stia in
    #  1..MAX_PAG (indicizza fuori tabella se non ci sta): lo facciamo qui.
    set riga [rl_leggi]
    set tok [rl_token $riga 2]
    if {[lindex $tok 0] ne "NUMERO"} {
        error "riga [rl_riga_corrente]: attesa 'NUMERO', trovata '[lindex $tok 0]'"
    }
    set numero [lindex $tok 1]
    if {![string is integer -strict $numero]} {
        error "riga [rl_riga_corrente]: NUMERO della pagina non e' un intero: '$numero'"
    }
    if {$numero < 1 || $numero > $max_pagine} {
        error "riga [rl_riga_corrente]: NUMERO della pagina fuori 1..$max_pagine: $numero"
    }

    set riga [rl_leggi]
    set tok [rl_token $riga 2]
    if {[lindex $tok 0] ne "NOME"} {
        error "riga [rl_riga_corrente]: attesa 'NOME', trovata '[lindex $tok 0]'"
    }
    set nome [lindex $tok 1]
    if {$nome eq ""} {
        error "riga [rl_riga_corrente]: NOME della pagina mancante"
    }
    if {[string length $nome] > $lun_nome_pagina} {
        error "riga [rl_riga_corrente]: NOME '$nome' supera $lun_nome_pagina caratteri"
    }

    #  DESCRIZIONE: compstaz la ricostruisce da al piu' 9 parole unite da UNO
    #  spazio (perde spaziature multiple originali) entro LUN_DES_PAG byte;
    #  qui si tiene il testo verbatim (per non perdere nulla nell'editor) e
    #  si segnala solo se supera il limite byte per byte, non parola per
    #  parola - la differenza conta poco in pratica (nessuna descrizione
    #  reale del parco macchine usa piu' di 9 parole).
    set riga [rl_leggi]
    set primo [lindex [rl_token $riga 1] 0]
    if {$primo ne "DESCRIZIONE"} {
        error "riga [rl_riga_corrente]: attesa 'DESCRIZIONE', trovata '$primo'"
    }
    set descrizione [rl_resto_riga $riga 200]
    if {$descrizione eq ""} {
        error "riga [rl_riga_corrente]: DESCRIZIONE della pagina mancante (obbligatoria)"
    }
    if {[string length $descrizione] > $lun_descrizione_pagina} {
        error "riga [rl_riga_corrente]: DESCRIZIONE supera $lun_descrizione_pagina caratteri"
    }

    return [dict create numero $numero nome $nome descrizione $descrizione]
}

# ---------------------------------------------------------------------------
# Il record STAZIONE. Le 13 stazioni "storiche" (tipi_old_staz) e i tipi non
# riconosciuti restano BLOCCHI OPACHI: si leggono in modo grezzo (righe di
# testo, senza interpretarle) cosi' che il file torni fuori identico a com'era
# per quella stazione, e lgmkstaz possa comunque spostarla o cancellarla senza
# doverla capire. Decisione dell'utente: v1 non le edita (vedi lgmkstaz_dati.tcl).
# ---------------------------------------------------------------------------

proc ::lgmkstaz::leggi_stazione {} {
    variable catalogo
    variable dimensioni_storiche
    variable max_pagine

    #  La riga che apre il record puo' avere un secondo token (un nome):
    #  compstaz lo legge ma non lo usa mai (ne' per il round-trip serve
    #  scriverlo di nuovo - lgmkstaz_scrivi.tcl emette sempre "STAZIONE" bare).

    #  NUMERO: ignorato da compstaz (l'indice vero e' la posizione nel file);
    #  la riga deve solo esistere ed essere "NUMERO", il valore si tiene per
    #  leggibilita' ma non si valida.
    set riga [rl_leggi]
    set primo [lindex [rl_token $riga 1] 0]
    if {$primo ne "NUMERO"} {
        error "riga [rl_riga_corrente]: attesa 'NUMERO', trovata '$primo'"
    }
    set numero_dichiarato [lindex [rl_token $riga 2] 1]

    set riga [rl_leggi]
    set tok [rl_token $riga 2]
    if {[lindex $tok 0] ne "TIPO"} {
        error "riga [rl_riga_corrente]: attesa 'TIPO', trovata '[lindex $tok 0]'"
    }
    set nome_tipo [lindex $tok 1]
    if {$nome_tipo eq ""} {
        error "riga [rl_riga_corrente]: TIPO mancante"
    }

    #  DESCRIZIONE: per le stazioni "nuove" compstaz la legge ma la BUTTA VIA
    #  (compila_new_staz: "legge la riga di descrizione ma non la memorizza,
    #  e se non c'e' non da errore" - controlla pero' che la riga cominci per
    #  DESCRIZIONE). Qui si tiene comunque, per non far sparire una nota
    #  scritta dall'utente solo perche' xstaz non la mostra.
    set riga [rl_leggi]
    set primo [lindex [rl_token $riga 1] 0]
    if {$primo ne "DESCRIZIONE"} {
        error "riga [rl_riga_corrente]: attesa 'DESCRIZIONE', trovata '$primo'"
    }
    set descrizione [rl_resto_riga $riga 200]

    #  PAGINA e POSIZIONE stanno SEMPRE qui, identiche per un tipo nuovo o
    #  storico (stesso ordine in cnewstaz.c e in ognuno dei 13 lettori
    #  storici, es. Alg_rt/grafica/compstaz/amd_c.c): servono anche a una
    #  stazione che lgmkstaz non sa interpretare, solo per sapere DOVE sta
    #  sulla griglia - un blocco opaco senza posizione non si potrebbe
    #  disegnare insieme alle altre.
    set riga [rl_leggi]
    set tok [rl_token $riga 2]
    if {[lindex $tok 0] ne "PAGINA"} {
        error "riga [rl_riga_corrente]: attesa 'PAGINA', trovata '[lindex $tok 0]'"
    }
    set pagina [lindex $tok 1]
    if {![string is integer -strict $pagina] || $pagina < 1 || $pagina > $max_pagine} {
        error "riga [rl_riga_corrente]: PAGINA non valida: '$pagina'"
    }

    set riga [rl_leggi]
    set tok [rl_token $riga 3]
    if {[lindex $tok 0] ne "POSIZIONE"} {
        error "riga [rl_riga_corrente]: attesa 'POSIZIONE', trovata '[lindex $tok 0]'"
    }
    set posx [lindex $tok 1]
    set posy [lindex $tok 2]
    if {![string is integer -strict $posx] || ![string is integer -strict $posy]} {
        error "riga [rl_riga_corrente]: POSIZIONE non valida: '$posx' '$posy'"
    }

    #  Se il tipo e' storico o sconosciuto, da qui in poi il record e' un
    #  blocco opaco: si leggono le righe grezze fino al prossimo "****" (che
    #  NON si consuma: lo legge il chiamante, leggi_file, come per ogni altro
    #  record) e si tengono cosi' come sono. La dimensione (celle) si sa per
    #  le 13 storiche (dimensioni_storiche, sempre 2 celle larghe - solo
    #  l'altezza cambia; tutti i lettori storici fanno posix1=ipx+2), per un
    #  tipo davvero sconosciuto si stima 2x1 e lo si segnala.
    if {![info exists catalogo($nome_tipo)]} {
        set righe_grezze [leggi_blocco_grezzo]
        if {[info exists dimensioni_storiche($nome_tipo)]} {
            lassign $dimensioni_storiche($nome_tipo) larg altezza
            set nota "tipo storico: lgmkstaz lo preserva ma non lo modifica"
        } else {
            set larg 2
            set altezza 1
            set nota "tipo sconosciuto (non in catalogo): dimensione stimata 2x1,\
                      preservato senza interpretarlo"
        }
        return [dict create numero $numero_dichiarato tipo $nome_tipo \
                    descrizione $descrizione pagina $pagina posx $posx posy $posy \
                    larg $larg altezza $altezza grezzo 1 righe_grezze $righe_grezze \
                    nota $nota]
    }

    #  Gli oggetti, nell'ordine esatto dato dal catalogo per questo tipo.
    lassign $catalogo($nome_tipo) larg altezza sequenza
    set oggetti {}
    foreach tipo_oggetto $sequenza {
        lappend oggetti [leggi_oggetto $tipo_oggetto]
    }

    return [dict create numero $numero_dichiarato tipo $nome_tipo \
                descrizione $descrizione pagina $pagina posx $posx posy $posy \
                larg $larg altezza $altezza grezzo 0 oggetti $oggetti]
}

#  Legge righe grezze fino a (ma senza consumare) la prossima riga "****".
#  Serve alle stazioni "opache" (leggi_stazione) per non dover capire un
#  formato che non conoscono.
proc ::lgmkstaz::leggi_blocco_grezzo {} {
    variable rl_righe

    set righe {}
    while {[llength $rl_righe] > 0} {
        set prossima [lindex $rl_righe 0]
        if {$prossima eq "****"} { break }
        lappend righe [rl_leggi]
    }
    return $righe
}

# ---------------------------------------------------------------------------
# Il file intero.
# ---------------------------------------------------------------------------

#  Legge un "****" e si lamenta se non lo trova - lo stesso controllo che fa
#  compstaz prima di ogni record (compstaz.c, il ciclo "for(;;)").
proc ::lgmkstaz::leggi_separatore {} {
    set riga [rl_leggi]
    if {$riga ne "****"} {
        error "riga [rl_riga_corrente]: attesi quattro asterischi ('****'),\
               trovato '$riga'"
    }
}

#  Legge un testo r01.dat gia' in memoria e ritorna il modello: un dict con
#  "pagine" (lista di record pagina, nell'ordine del file) e "stazioni"
#  (lista di record stazione, nell'ordine del file - quell'ordine E' l'indice
#  che compstaz usa davvero, il NUMERO scritto nel file non conta).
#
#  Solleva un errore Tcl con un messaggio che comincia sempre per "riga N: "
#  se il file non rispetta la grammatica: chi chiama lo intercetta con
#  [catch] e lo mostra cosi' com'e', non serve altra elaborazione.
proc ::lgmkstaz::leggi_testo {testo} {
    variable max_stazioni

    rl_avvia $testo

    set pagine {}
    set stazioni {}
    set numeri_pagina_visti {}
    set prossimo_id 0

    while 1 {
        leggi_separatore
        set intestazione [rl_leggi]
        if {$intestazione eq "END_OF_FILE"} { break }
        if {$intestazione eq "PAGINA"} {
            #  la riga "PAGINA" gia' letta e' l'intestazione del record: il
            #  resto lo legge leggi_pagina come se ripartisse da li'. Per
            #  tenere leggi_pagina semplice (comincia sempre da NUMERO), le
            #  si passa il controllo SENZA aver gia' consumato altro.
            set pagina [leggi_pagina]
            set n [dict get $pagina numero]
            if {$n in $numeri_pagina_visti} {
                error "riga [rl_riga_corrente]: numero di pagina $n gia' usato"
            }
            lappend numeri_pagina_visti $n
            lappend pagine $pagina
        } elseif {$intestazione eq "STAZIONE"} {
            if {[llength $stazioni] >= $max_stazioni} {
                error "riga [rl_riga_corrente]: superate $max_stazioni stazioni (MAX_STAZ)"
            }
            #  "id": un identificativo interno a lgmkstaz, mai scritto nel
            #  file (lgmkstaz_scrivi.tcl non lo guarda nemmeno), assegnato in
            #  ordine di lettura a partire da 0. Serve solo a chi modifica il
            #  modello dopo averlo letto (lgmkstaz_modifica.tcl): la
            #  POSIZIONE di una stazione nella lista cambia se se ne
            #  cancella o se ne aggiunge un'altra prima, l'id no. E'
            #  assegnato qui, deterministico (riparte da 0 a ogni lettura),
            #  cosi' rileggere un file scritto da lgmkstaz_scrivi.tcl da'
            #  sempre la STESSA sequenza di id di partenza: il test di
            #  round-trip (lgmkstaz_test.tcl) confronta modelli con gli id
            #  gia' dentro, e deve continuare a vederli identici.
            set s [leggi_stazione]
            dict set s id $prossimo_id
            incr prossimo_id
            lappend stazioni $s
        } else {
            error "riga [rl_riga_corrente]: attesa 'PAGINA', 'STAZIONE' o\
                   'END_OF_FILE', trovata '$intestazione'"
        }
    }

    #  Ogni stazione con tipo NOTO deve citare una pagina dichiarata: non lo
    #  controlla compstaz mentre legge (lo risolve dopo, in fill_pagina), ma
    #  e' un errore che vale la pena dare subito, col numero di stazione.
    set i 0
    foreach s $stazioni {
        incr i
        if {[dict get $s grezzo]} { continue }
        set p [dict get $s pagina]
        if {$p ni $numeri_pagina_visti} {
            error "la stazione $i (tipo [dict get $s tipo]) cita la pagina $p,\
                   mai dichiarata"
        }
    }

    return [dict create pagine $pagine stazioni $stazioni]
}

#  Come leggi_testo, ma legge il file dal disco. I byte si leggono senza
#  traduzione (-translation lf sia in lettura sia in scrittura vuol dire "non
#  toccare niente"): un eventuale \r di un file CRLF deve arrivare intatto
#  fino a rl_leggi, che lo segnala - se Tcl lo togliesse da solo, l'errore
#  sparirebbe insieme al sintomo che lo fa notare.
proc ::lgmkstaz::leggi_file {percorso} {
    set ch [open $percorso r]
    fconfigure $ch -translation lf -encoding binary
    set testo [read $ch]
    close $ch
    return [leggi_testo $testo]
}
