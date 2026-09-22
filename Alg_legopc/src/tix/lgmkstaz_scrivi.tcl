# lgmkstaz_scrivi.tcl - dal modello in memoria (lgmkstaz_leggi.tcl) al testo
# di un r01.dat. Tcl puro, nessun Tk.
#
# E' lo specchio di lgmkstaz_leggi.tcl, campo per campo, guidato dalla stessa
# grammatica di lgmkstaz_dati.tcl: non serve un caso speciale per ogni tipo di
# oggetto, solo un "come si scrive" per ogni FORMA di riga (colore, etichetta,
# riferimento, output, numero, minmax).
#
# Convenzioni di chi scrive (compstaz non le richiede tutte, ma lgmkstaz le
# rispetta sempre, per restare ben dentro i limiti del formato):
#   - fine riga LF, mai CRLF;
#   - nessuna riga comincia con uno spazio;
#   - nessuna riga supera 78 caratteri (oltre, compstaz la leggerebbe
#     troncata in silenzio: qui e' un errore, non un file che si scrive lo
#     stesso e si rompe dopo);
#   - le pagine si scrivono tutte prima, poi tutte le stazioni: l'ORDINE
#     originale nel file (che puo' alternare PAGINA e STAZIONE a piacere, il
#     formato non lo richiede) non sopravvive al giro completo
#     lettura->modello->scrittura, ma il file resta valido e SEMANTICAMENTE
#     identico - lo stesso numero di pagine e stazioni, con gli stessi dati.
#     Un test di fedelta' va quindi fatto sul MODELLO (rileggere il file
#     appena scritto e confrontarlo con quello di partenza), non sul testo.

namespace eval ::lgmkstaz {
    variable sc_righe {}
}

proc ::lgmkstaz::sc_avvia {} {
    variable sc_righe
    set sc_righe {}
}

#  Accoda una riga gia' pronta, controllando che rispetti i limiti del
#  formato (vedi sopra). L'errore qui e' un errore INTERNO se arriva da un
#  modello che leggi_testo avrebbe accettato: vuol dire che un valore e'
#  cambiato a mano dopo la lettura (o da una futura GUI) senza rispettare i
#  limiti, e va segnalato subito, non lasciato scoprire a compstaz.
proc ::lgmkstaz::sc_riga {riga} {
    variable sc_righe
    if {[string index $riga 0] eq " "} {
        error "riga generata che comincia con uno spazio (vietato): '$riga'"
    }
    if {[string length $riga] > 78} {
        error "riga generata lunga [string length $riga] caratteri, oltre i 78\
               ammessi: '$riga'"
    }
    lappend sc_righe $riga
}

# ---------------------------------------------------------------------------
# Un campo generico, nella forma {nome valore} prodotta da leggi_campo.
# ---------------------------------------------------------------------------

proc ::lgmkstaz::scrivi_campo {nome valore} {
    variable kw

    switch -- $nome {
        colore {
            sc_riga "$kw(colore) $valore"
        }
        etichetta {
            if {$valore eq ""} {
                sc_riga $kw(etichetta)
            } else {
                sc_riga "$kw(etichetta) $valore"
            }
        }
        input       { scrivi_riferimento $kw(input) $valore 3 }
        input_neg   { scrivi_riferimento $kw(input) $valore 4 }
        input_blink_neg { scrivi_riferimento $kw(input_blink) $valore 4 }
        input_err   { scrivi_riferimento $kw(input_err) $valore 3 }
        inibizione  { scrivi_riferimento $kw(inibizione) $valore 3 }
        output {
            set var [dict get $valore var]
            if {$var eq ""} {
                sc_riga $kw(output)
            } else {
                set riga "$kw(output) $var [dict get $valore mod] [dict get $valore modo]"
                set v [dict get $valore valore]
                if {$v ne ""} { append riga " $v" }
                sc_riga $riga
            }
        }
        scalamento     { sc_riga "$kw(scalamento) $valore" }
        offset         { sc_riga "$kw(offset) $valore" }
        scalamento_err { sc_riga "$kw(scalamento_err) $valore" }
        minmax         { sc_riga "$kw(minmax) [lindex $valore 0] [lindex $valore 1]" }
        minmax_err     { sc_riga "$kw(minmax_err) [lindex $valore 0] [lindex $valore 1]" }
        default {
            error "campo di grammatica sconosciuto: '$nome' (errore interno di lgmkstaz)"
        }
    }
}

#  $nstr e' 3 (nessun NOT: input/input_err/inibizione) o 4 (input_neg/
#  input_blink_neg, dove il NOT si scrive solo se presente).
proc ::lgmkstaz::scrivi_riferimento {parola_chiave valore nstr} {
    set var [dict get $valore var]
    if {$var eq ""} {
        sc_riga $parola_chiave
        return
    }
    set riga "$parola_chiave $var [dict get $valore mod]"
    if {$nstr == 4 && [dict get $valore not]} {
        append riga " $::lgmkstaz::kw(not)"
    }
    sc_riga $riga
}

# ---------------------------------------------------------------------------
# Un oggetto: la riga che lo apre, poi un campo alla volta nell'ordine in cui
# leggi_oggetto li ha letti (che E' l'ordine della grammatica: un oggetto
# letto da un file valido ha sempre tutti e nell'ordine giusto i suoi campi).
# ---------------------------------------------------------------------------

proc ::lgmkstaz::scrivi_oggetto {oggetto} {
    variable blocco_kw

    set tipo [dict get $oggetto tipo]
    sc_riga $blocco_kw($tipo)
    foreach campo [dict get $oggetto campi] {
        scrivi_campo [dict get $campo nome] [dict get $campo valore]
    }
}

# ---------------------------------------------------------------------------
# I record.
# ---------------------------------------------------------------------------

proc ::lgmkstaz::scrivi_pagina {pagina} {
    sc_riga "****"
    sc_riga "PAGINA"
    sc_riga "NUMERO [dict get $pagina numero]"
    sc_riga "NOME [dict get $pagina nome]"
    sc_riga "DESCRIZIONE [dict get $pagina descrizione]"
}

proc ::lgmkstaz::scrivi_stazione {stazione} {
    sc_riga "****"
    sc_riga "STAZIONE"
    set numero [dict get $stazione numero]
    if {$numero eq ""} {
        sc_riga "NUMERO"
    } else {
        sc_riga "NUMERO $numero"
    }
    sc_riga "TIPO [dict get $stazione tipo]"
    set descrizione [dict get $stazione descrizione]
    if {$descrizione eq ""} {
        sc_riga "DESCRIZIONE"
    } else {
        sc_riga "DESCRIZIONE $descrizione"
    }

    #  PAGINA e POSIZIONE: si scrivono sempre qui, anche per un blocco opaco -
    #  leggi_stazione le legge in modo strutturato per OGNI stazione, tipo
    #  storico o sconosciuto compreso (vedi il commento li').
    sc_riga "PAGINA [dict get $stazione pagina]"
    sc_riga "POSIZIONE [dict get $stazione posx] [dict get $stazione posy]"

    if {[dict get $stazione grezzo]} {
        #  blocco opaco (tipo storico o non in catalogo) da qui in poi: le
        #  righe si riscrivono tali e quali, senza passare da sc_riga (sono
        #  gia' state validate quando lette - e potrebbero non rispettare le
        #  convenzioni di lgmkstaz, essendo di un formato che non conosce).
        variable sc_righe
        foreach r [dict get $stazione righe_grezze] {
            lappend sc_righe $r
        }
        return
    }

    foreach oggetto [dict get $stazione oggetti] {
        scrivi_oggetto $oggetto
    }
}

# ---------------------------------------------------------------------------
# Il file intero.
# ---------------------------------------------------------------------------

#  Dal modello (lo stesso dict che ritorna leggi_testo/leggi_file) al testo
#  completo del r01.dat, pronto per essere scritto su disco. Le pagine tutte
#  prima, poi tutte le stazioni (vedi la nota in testa al file).
proc ::lgmkstaz::scrivi_testo {modello} {
    sc_avvia
    foreach p [dict get $modello pagine] { scrivi_pagina $p }
    foreach s [dict get $modello stazioni] { scrivi_stazione $s }
    sc_riga "****"
    sc_riga "END_OF_FILE"
    variable sc_righe
    return "[join $sc_righe \n]\n"
}

#  Scrive il modello su file, LF puro (nessuna traduzione CRLF della
#  piattaforma: sul file di destinazione conta il byte, non il sistema che lo
#  scrive).
proc ::lgmkstaz::scrivi_file {percorso modello} {
    set testo [scrivi_testo $modello]
    set ch [open $percorso w]
    fconfigure $ch -translation lf -encoding binary
    puts -nonewline $ch $testo
    close $ch
}
