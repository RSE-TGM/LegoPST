# lgmkstaz.tcl - builder di pagine faceplate xstaz (r01.dat).
#
# FASE 3 del piano (vedi memoria di progetto project_lgmkstaz.md):
# collegamento al modello. Sull'editing della Fase 2 (libreria, trascinamento,
# Canc, pannello proprieta', pagine nuove, Salva) i campi INPUT/INPUT_ERR/
# INPUT_BLINK/INIBIZIONE/OUTPUT del pannello proprieta' hanno ora un
# selettore ("...") che elenca le variabili vere del simulatore - lette da
# variabili.edf (lgmkstaz_topologia.tcl), lo stesso dump ASCII che scrive
# compstaz - e la scelta a mano viene VERIFICATA al salvataggio: una
# variabile che non esiste, o non e' del tipo giusto (uscita per un INPUT,
# ingresso libero per un OUTPUT), e' rifiutata li', non solo da compstaz
# dopo. Senza un variabili.edf accanto al file (non tutte le directory ce
# l'hanno: il catalogo di consultazione non ne ha bisogno, per esempio) i
# campi restano testo libero come nella Fase 2, senza errori.
#
# Il disegno di ogni oggetto elementare resta una FORMA SEMPLICE
# riconoscibile, non una riproduzione esatta di xstaz (vedi il commento
# storico piu' sotto, invariato dalla Fase 1). Le stazioni "grezze" (tipo
# storico o sconosciuto, vedi lgmkstaz_leggi.tcl) si possono spostare e
# cancellare ma non modificare nei campi: lgmkstaz non ne conosce la
# grammatica.
#
# Non ancora fatto (fase successiva): "Compila e verifica"/"Anteprima con
# xstaz" con compstaz vero.
#
# Uso:
#   wish lgmkstaz.tcl [<r01.dat> | <directory>]
#   (senza argomenti, cerca r01.dat nella directory corrente - come compstaz)
#
# Lanciato normalmente da Alg_rt/bin/lgmkstaz (sorgia il profilo se serve,
# come Alg_rt/bin/lghmi).

package require Tk

# creata subito: serve gia' per le variabili di appoggio del caricamento qui
# sotto, prima ancora che lgmkstaz_dati.tcl (sorgiato fra un attimo) la
# popoli con le sue tabelle.
namespace eval ::lgmkstaz {}

set ::lgmkstaz::_qui [file dirname [file normalize [info script]]]
foreach ::lgmkstaz::_lib {lgmkstaz_dati.tcl lgmkstaz_leggi.tcl lgmkstaz_scrivi.tcl \
                          lgmkstaz_modifica.tcl lgmkstaz_topologia.tcl lgmkstaz_verifica.tcl} {
    if {[catch {source [file join $::lgmkstaz::_qui $::lgmkstaz::_lib]} _err]} {
        tk_messageBox -icon error -title lgmkstaz \
            -message "Impossibile caricare $::lgmkstaz::_lib:\n$_err"
        exit 1
    }
}
# lgstaz.tcl porta staz_apri (e xstaz_attivo/staz_stessa_dir): serve solo ad
# "Anteprima con xstaz" (menu Verifica, dopo una compilazione riuscita) - il
# resto di lgmkstaz funziona anche senza, quindi si sourcia con un catch
# invece che con l'uscita delle librerie sopra: se manca, il pulsante
# "Anteprima" semplicemente lo dice invece di comparire e fallire.
catch {source [file join $::lgmkstaz::_qui lgstaz.tcl]}
# balloon.tcl (il tooltip gia' usato da draw2gr/lghmi): facoltativo, non
# ancora usato qui - sourciato per coerenza con gli altri strumenti.
catch {source [file join $::lgmkstaz::_qui balloon.tcl]}
# openhelp.tcl (browser + conversione .md -> HTML) e md2html.tcl: servono al
# menu "?", le stesse di lghmi. Facoltativi: senza, le voci del menu restano
# spente invece di far fallire l'avvio.
catch {source [file join $::lgmkstaz::_qui openhelp.tcl]}
catch {source [file join $::lgmkstaz::_qui md2html.tcl]}

namespace eval ::lgmkstaz {
    variable modello ""          ;# il file letto (dict pagine/stazioni); "" = niente aperto
    variable percorso_corrente ""
    variable pagina_disegnata ""
    variable altezza_pagina_celle 1  ;# posmy della pagina disegnata ora (vedi disegna_pagina)
    variable modificato 0
    variable stazione_selezionata ""  ;# id, o "" se niente selezionato
    variable tipo_armato ""           ;# tipo scelto in libreria (modo "piazza"), o ""
    variable drag_id ""               ;# id della stazione in trascinamento, o ""
    variable drag_dx 0
    variable drag_dy 0
    variable drag_partito 0           ;# 1 se durante il trascinamento c'e' stato un vero movimento
    variable prossimo_id 0            ;# per le stazioni create durante l'editing
    variable appunti ""               ;# la stazione copiata con Ctrl+C, o ""

    # Disegno delle stazioni con le immagini vere di xstaz (sprite) invece che
    # con lo schema a forme semplici. Vedi sprite_per_tipo.
    variable usa_sprite 1
    variable sprite         ;# tipo -> nome dell'immagine Tk, o "" se non c'e'
    array set sprite {}
    variable miniatura      ;# tipo -> sprite rimpicciolito per la riga di stato
    variable spaziatore     ;# immagine trasparente: tiene ferma l'altezza della riga di stato
    array set miniatura {}
    variable dir_sprite ""  ;# "" = non ancora cercata, "-" = cercata e assente

    # Colori LEGO -> colori Tk. Non i colori X11 puri (yellow/green/red/blue):
    # su uno sfondo bianco sono poco leggibili o troppo accesi; queste
    # tonalita' restano riconoscibili come "lo stesso colore" ma si vedono
    # meglio. E' una scelta della VISTA, non del formato: il file continua a
    # contenere il nome LEGO (VERDE, ROSSO, ...).
    array set colore_tk {
        NERO   black
        BIANCO white
        GIALLO  #cc9900
        VERDE   #1a7a1a
        ROSSO   #c21807
        GRIGIO  #808080
        BLU     #1450a3
    }
}

# ---------------------------------------------------------------------------
# Accesso ai campi di un oggetto: ::lgmkstaz::leggi_oggetto costruisce
# "campi" come lista ordinata di {nome valore} (vedi il commento in
# lgmkstaz_dati.tcl sul perche' non e' un dict). Qui serve solo il PRIMO
# campo con quel nome (colore, etichetta): per il disegno basta.
# ---------------------------------------------------------------------------

proc ::lgmkstaz::oggetto_campo {oggetto nome} {
    foreach campo [dict get $oggetto campi] {
        if {[dict get $campo nome] eq $nome} { return [dict get $campo valore] }
    }
    return ""
}

# ---------------------------------------------------------------------------
# Disegno di una pagina.
# ---------------------------------------------------------------------------

#  Disegna il numero di pagina dato sul canvas principale. Richiamata ogni
#  volta che l'utente sceglie una pagina diversa, e dopo ogni modifica.
proc ::lgmkstaz::disegna_pagina {numero} {
    variable modello
    variable dim_cella_px
    variable pagina_disegnata
    variable altezza_pagina_celle

    set c .corpo.destra.canvas
    $c delete all
    set pagina_disegnata $numero
    if {$modello eq ""} { aggiorna_titolo; return }

    set stazioni_qui {}
    foreach s [dict get $modello stazioni] {
        if {[dict get $s pagina] == $numero} { lappend stazioni_qui $s }
    }
    #  altezza_pagina_celle E' posmy (Alg_rt/grafica/compstaz/compstaz.c,
    #  fill_pagina): il riferimento ESATTO che riquadro_celle/cella_da_pixel
    #  usano per il capovolgimento della Y, lo stesso che xstaz userebbe.
    #  NON va gonfiato per fare posto al margine: la Y e' capovolta, quindi
    #  ingrandire questo valore sposta la zona in PIU' verso il basso (sotto
    #  l'origine, territorio negativo) invece che sopra il contenuto - e'
    #  l'errore fatto nella prima versione di questa vista, segnalato
    #  dall'utente il 2026-09-22. Il margine di crescita si ottiene invece
    #  lasciando che il canvas parta da una Y NEGATIVA (vedi sotto).
    set altezza_pagina_celle [altezza_pagina $stazioni_qui]

    #  estensione del canvas:
    #    - a destra e in alto (X maggiori, Y-file maggiori = Y-canvas MINORI
    #      di zero): margine di crescita vero, spazio bianco con la griglia,
    #      almeno un minimo anche su una pagina vuota;
    #    - sotto l'origine e a sinistra di essa: ESATTAMENTE una riga e una
    #      colonna, territorio proibito (POSIZIONE non scende mai sotto
    #      0,0), disegnate come quadratini grigio chiaro - un confine
    #      visibile, non uno spazio bianco indistinguibile dalle celle vere.
    set contenuto_x 0
    foreach s $stazioni_qui {
        set x1 [expr {([dict get $s posx] + [dict get $s larg]) * $dim_cella_px}]
        if {$x1 > $contenuto_x} { set contenuto_x $x1 }
    }
    set margine_dx  [expr {3 * $dim_cella_px}]
    set margine_su  [expr {3 * $dim_cella_px}]
    set larg_valida [expr {max(10 * $dim_cella_px, $contenuto_x + $margine_dx)}]
    set alt_valida  [expr {max(8 * $dim_cella_px, \
                                $altezza_pagina_celle * $dim_cella_px + $margine_su)}]

    set y_origine [expr {$altezza_pagina_celle * $dim_cella_px}]   ;# bordo inferiore di posy=0
    set y0 [expr {$y_origine - $alt_valida}]                       ;# puo' essere negativo: e' sopra il canvas "logico"
    set y1 [expr {$y_origine + $dim_cella_px}]                     ;# +1 cella: la riga proibita
    set x0 [expr {-1 * $dim_cella_px}]                             ;# 1 cella: la colonna proibita
    set x1 $larg_valida
    $c configure -scrollregion [list $x0 $y0 $x1 $y1]

    #  la striscia proibita: un quadratino grigio chiaro per cella, non un
    #  unico rettangolo - "composta da quadrati", come si conta a vista una
    #  griglia. La riga sotto e la colonna a sinistra si intersecano
    #  nell'angolo (-1,-1 del file): disegnata due volte, stesso aspetto,
    #  non fa differenza.
    for {set gx $x0} {$gx < $x1} {incr gx $dim_cella_px} {
        $c create rectangle $gx $y_origine [expr {$gx + $dim_cella_px}] $y1 \
            -fill #e2e2e2 -outline #cfcfcf -tags proibito
    }
    for {set gy $y0} {$gy < $y1} {incr gy $dim_cella_px} {
        $c create rectangle $x0 $gy [expr {$x0 + $dim_cella_px}] [expr {$gy + $dim_cella_px}] \
            -fill #e2e2e2 -outline #cfcfcf -tags proibito
    }

    #  griglia leggera della zona VALIDA soltanto (0..larg_valida in X,
    #  y0..y_origine in Y): una linea per cella, aiuta a leggere le celle di
    #  POSIZIONE senza doverle contare a mano.
    for {set gx 0} {$gx <= $x1} {incr gx $dim_cella_px} {
        $c create line $gx $y0 $gx $y_origine -fill #eeeeee -tags griglia
    }
    for {set gy $y0} {$gy <= $y_origine} {incr gy $dim_cella_px} {
        $c create line 0 $gy $x1 $gy -fill #eeeeee -tags griglia
    }

    foreach s $stazioni_qui {
        disegna_stazione $c $s
    }
    disegna_selezione
    aggiorna_titolo

    #  la vista si apre con l'ORIGINE (0,0 del file) all'estremo BASSO A
    #  SINISTRA del riquadro visibile, come un piano cartesiano - non nel
    #  punto di default del canvas (l'inizio del suo spazio di scorrimento,
    #  che con la Y capovolta e' la CIMA della pagina: su una pagina alta,
    #  l'origine e le stazioni vicine ad essa sarebbero rimaste fuori vista
    #  finche' non si scorreva in basso a mano). Cosi' si vede anche subito
    #  la striscia proibita, il confine della zona di lavoro.
    $c xview moveto 0
    $c yview moveto 1
}

#  Una stazione: il riquadro nella sua cella, poi i suoi oggetti dentro. Le
#  stazioni "grezze" si disegnano come un riquadro tratteggiato col nome del
#  tipo: si vede che c'e' qualcosa e dove, non cosa contiene davvero.
#  L'identificativo sul canvas e' "staz<id>" (l'id stabile del modello, non
#  la posizione nella lista): stazione_sotto lo rilegge per ritrovare la
#  stazione vera a ogni clic, anche dopo che altre sono state aggiunte o
#  tolte.
#  La directory degli sprite: le immagini vere delle stazioni, ritagliate
#  dalle catture di xstaz (Alg_rt/grafica/xstaz/catalogo/staz, generate da
#  ritaglia_sprite.tcl - vedi il README li' accanto). Si cerca come compstaz:
#  prima LEGOROOT, poi risalendo da questo script, cosi' vale sia per il
#  sorgente sia per la copia distribuita in Alg_legopc/bin. Non si duplicano
#  i file: sono gia' nel repo accanto a xstaz, che e' chi li produce.
#  Lo sprite rimpicciolito per la riga di stato. Tk sa ridurre una photo solo
#  per fattori INTERI (-subsample), ma qui va benissimo: le stazioni sono alte
#  un numero intero di celle da 62 px, quindi un fattore pari a altezza/31
#  rende tutte le miniature alte 31 px esatti, con le proporzioni giuste
#  (subsample agisce uguale sui due assi) e larghezza che segue la forma della
#  stazione - una 2x1 viene 62x31, una 12x4 viene 93x31.
proc ::lgmkstaz::miniatura_per_tipo {tipo} {
    variable miniatura
    if {[info exists miniatura($tipo)]} { return $miniatura($tipo) }
    set miniatura($tipo) ""
    set img [sprite_per_tipo $tipo]
    if {$img eq ""} { return "" }
    set fattore [expr {max(1, int(round([image height $img] / 31.0)))}]
    if {[catch {image create photo} mini]} { return "" }
    $mini copy $img -subsample $fattore
    set miniatura($tipo) $mini
    return $mini
}

#  Tutto quello che finisce nella riga di stato passa di qui, cosi' la
#  miniatura del tipo armato compare e sparisce da sola insieme all'armamento,
#  qualunque sia il messaggio mostrato in quel momento (dopo aver piazzato una
#  stazione il tipo resta armato: la miniatura deve restare).
proc ::lgmkstaz::stato {testo} {
    variable tipo_armato
    set img ""
    if {$tipo_armato ne ""} { set img [miniatura_per_tipo $tipo_armato] }
    #  Senza miniatura si mette al suo posto un'immagine trasparente della
    #  stessa altezza: la riga di stato resta alta uguale, invece di saltare
    #  ogni volta che si arma o si disarma un tipo.
    if {$img eq ""} { set img [::lgmkstaz::spaziatore_stato] }
    .status configure -text $testo -image $img -compound left
}

proc ::lgmkstaz::spaziatore_stato {} {
    variable spaziatore
    if {![info exists spaziatore]} {
        set spaziatore [image create photo -width 1 -height 31]
    }
    return $spaziatore
}

# ---------------------------------------------------------------------------
# Menu "?" - la documentazione.
#
# Stesso meccanismo del "?" di lghmi (openhelp.tcl: converte il .md in HTML e
# lo apre nel browser), ma puntato a questo strumento: in cima la guida di
# lgmkstaz, poi i documenti che servono mentre si costruisce una pagina.
# ---------------------------------------------------------------------------

proc ::lgmkstaz::documenti_aiuto {} {
    return {
        {"lgmkstaz: how to use it"                 Alg_legopc/LGMKSTAZ.md  rilievo}
        --
        {"Command faceplates: the r01.dat format"  Alg_rt/grafica/xstaz/HOWTO_faceplate.md}
        {"Station catalogue and sprites"           Alg_rt/grafica/xstaz/catalogo/README.md}
        {"xstaz and compstaz"                      Alg_rt/grafica/xstaz/README.md}
        --
        {"LegoPST - project overview (README)"     README.md}
        {"Annotated documentation index"           DOCUMENTATION_INDEX.html}
    }
}

proc ::lgmkstaz::apri_documento {relativo} {
    if {[llength [info procs aiuto_apri_documento]] == 0} {
        tk_messageBox -icon error -title lgmkstaz -parent . -message \
            "openhelp.tcl non caricato: non posso aprire la documentazione."
        return
    }
    set messaggio [aiuto_apri_documento $relativo]
    if {$messaggio ne ""} { stato $messaggio }
}

#  Una voce che punta a un file assente nasce SPENTA invece di sparire: si
#  vede che il documento e' previsto e che manca (stessa scelta di lghmi).
proc ::lgmkstaz::costruisci_menu_aiuto {m} {
    foreach voce [documenti_aiuto] {
        if {$voce eq "--"} { $m add separator; continue }
        lassign $voce etichetta relativo rilievo
        set c_e [expr {[info exists ::env(LEGOROOT)] && $::env(LEGOROOT) ne "" \
                       && [file exists [file join $::env(LEGOROOT) $relativo]]}]
        $m add command -label $etichetta -state [expr {$c_e ? "normal" : "disabled"}] \
            -command [list ::lgmkstaz::apri_documento $relativo]
        if {$rilievo ne ""} { $m entryconfigure end -font fontMenuRilievo }
    }
}

#  Ridisegna la pagina che si sta guardando, senza toccare il modello: serve
#  a chi cambia solo il MODO di disegnare (menu Visualizza).
proc ::lgmkstaz::ridisegna {} {
    variable pagina_disegnata
    if {$pagina_disegnata eq ""} { return }
    disegna_pagina $pagina_disegnata
    disegna_selezione
}

proc ::lgmkstaz::trova_dir_sprite {} {
    variable dir_sprite
    if {$dir_sprite ne ""} { return [expr {$dir_sprite eq "-" ? "" : $dir_sprite}] }
    set sotto [list Alg_rt grafica xstaz catalogo staz]
    if {[info exists ::env(LEGOROOT)] && $::env(LEGOROOT) ne ""} {
        set c [file join $::env(LEGOROOT) {*}$sotto]
        if {[file isdirectory $c]} { set dir_sprite $c; return $c }
    }
    set dir $::lgmkstaz::_qui
    for {set i 0} {$i < 6} {incr i} {
        set c [file join $dir {*}$sotto]
        if {[file isdirectory $c]} { set dir_sprite $c; return $c }
        set su [file dirname $dir]
        if {$su eq $dir} break
        set dir $su
    }
    set dir_sprite "-"
    return ""
}

#  L'immagine di un tipo di stazione, caricata la prima volta che serve e poi
#  tenuta (una pagina ripete gli stessi tipi, e cambiare pagina non deve
#  rileggere i PNG). Ritorna "" se lo sprite non c'e': le 13 stazioni storiche
#  non ne hanno uno, e nemmeno un tipo che fosse stato aggiunto a newstaz.h
#  senza rigenerare il catalogo. Chi disegna ripiega sullo schema.
proc ::lgmkstaz::sprite_per_tipo {tipo} {
    variable sprite
    if {[info exists sprite($tipo)]} { return $sprite($tipo) }
    set sprite($tipo) ""
    set dir [trova_dir_sprite]
    if {$dir eq ""} { return "" }
    set f [file join $dir "$tipo.png"]
    if {![file readable $f]} { return "" }
    if {[catch {image create photo -file $f} img]} { return "" }
    set sprite($tipo) $img
    return $img
}

proc ::lgmkstaz::disegna_stazione {c stazione} {
    variable altezza_pagina_celle

    set id [dict get $stazione id]
    set larg [dict get $stazione larg]
    set altezza [dict get $stazione altezza]
    lassign [riquadro_celle [dict get $stazione posx] [dict get $stazione posy] \
                 $larg $altezza $altezza_pagina_celle] x0 y0 x1 y1
    set tag "staz$id"

    if {[dict get $stazione grezzo]} {
        $c create rectangle $x0 $y0 $x1 $y1 -fill #f2f2f2 -outline #999999 \
            -stipple gray25 -tags [list stazione $tag]
        $c create text [expr {($x0 + $x1) / 2}] [expr {($y0 + $y1) / 2}] \
            -text [dict get $stazione tipo] -fill #555555 \
            -font {Helvetica 8} -tags [list stazione $tag]
    } else {
        #  Con lo sprite si vede la stazione com'e' davvero in xstaz; le parti
        #  che dipendono dall'istanza (etichette, colori, scale) restano quelle
        #  generiche del catalogo, percio' lo schema a forme semplici resta
        #  disponibile dal menu Visualizza: e' l'unico che mostra le ETICHETTA
        #  vere di questa stazione.
        variable usa_sprite
        set img ""
        if {$usa_sprite} { set img [sprite_per_tipo [dict get $stazione tipo]] }
        if {$img ne ""} {
            $c create image $x0 $y0 -anchor nw -image $img \
                -tags [list stazione $tag]
        } else {
            $c create rectangle $x0 $y0 $x1 $y1 -fill white -outline #bbbbbb \
                -tags [list stazione $tag]
            disegna_oggetti $c $x0 $y0 $x1 $y1 [dict get $stazione oggetti] $tag
        }
    }
}

#  Gli oggetti di una stazione dentro il suo riquadro (x0,y0)-(x1,y1). Le
#  STRINGA in testa alla sequenza sono didascalie e si scrivono come testo,
#  una per riga, in alto; il resto va in un flusso di icone sotto, a
#  griglia, nell'ordine in cui compare nel file.
proc ::lgmkstaz::disegna_oggetti {c x0 y0 x1 y1 oggetti tag} {
    set larg_box [expr {$x1 - $x0}]

    set y_corrente [expr {$y0 + 2}]
    set indice_inizio 0
    foreach oggetto $oggetti {
        if {[dict get $oggetto tipo] ne "STRINGA"} { break }
        set testo [oggetto_campo $oggetto etichetta]
        if {$testo ne ""} {
            $c create text [expr {$x0 + 3}] $y_corrente -anchor nw -text $testo \
                -font {Helvetica 7} -fill #333333 -tags [list stazione $tag]
        }
        incr y_corrente 10
        incr indice_inizio
    }

    set resto [lrange $oggetti $indice_inizio end]
    if {[llength $resto] == 0} { return }

    set lato 12
    set passo [expr {$lato + 3}]
    set per_riga [expr {max(1, int(($larg_box - 4) / $passo))}]
    set riga 0
    set colonna 0
    foreach oggetto $resto {
        set cx [expr {$x0 + 3 + $colonna * $passo}]
        set cy [expr {$y_corrente + $riga * $passo}]
        disegna_icona_oggetto $c $cx $cy $lato $oggetto $tag
        incr colonna
        if {$colonna >= $per_riga} { set colonna 0; incr riga }
    }
}

#  Una forma semplice per un oggetto elementare, dentro il quadrato
#  (cx,cy)-(cx+lato,cy+lato). Riconoscibile, non fedele al pixel (vedi la
#  nota in testa al file).
proc ::lgmkstaz::disegna_icona_oggetto {c cx cy lato oggetto tag} {
    variable colore_tk

    set tipo [dict get $oggetto tipo]
    set nome_colore [oggetto_campo $oggetto colore]
    set colore [expr {
        [info exists colore_tk($nome_colore)] ? $colore_tk($nome_colore) : "#cccccc"
    }]
    set x1 [expr {$cx + $lato}]
    set y1 [expr {$cy + $lato}]
    set t [list stazione $tag]

    switch -- $tipo {
        LED {
            $c create oval $cx $cy $x1 $y1 -fill $colore -outline black -tags $t
        }
        LUCE {
            $c create rectangle $cx $cy $x1 $y1 -fill $colore -outline black -tags $t
        }
        LAMPADA {
            $c create oval [expr {$cx - 2}] [expr {$cy - 2}] [expr {$x1 + 2}] [expr {$y1 + 2}] \
                -fill $colore -outline black -width 2 -tags $t
        }
        PULSANTE - TASTO {
            $c create rectangle $cx $cy $x1 $y1 -fill $colore -outline #333333 -width 2 -tags $t
        }
        PULS_LUCE {
            $c create rectangle $cx $cy $x1 $y1 -fill white -outline #333333 -width 2 -tags $t
            $c create oval [expr {$cx + 3}] [expr {$cy + 3}] [expr {$x1 - 3}] [expr {$y1 - 3}] \
                -fill $colore -outline black -tags $t
        }
        SELETTORE {
            $c create rectangle $cx $cy $x1 $y1 -fill #dddddd -outline black -tags $t
            $c create line $cx [expr {($cy + $y1) / 2}] $x1 [expr {($cy + $y1) / 2}] \
                -fill black -tags $t
        }
        INDICATORE {
            $c create rectangle $cx $cy $x1 $y1 -fill #f5f5f5 -outline black -tags $t
            $c create line [expr {($cx + $x1) / 2}] $cy [expr {($cx + $x1) / 2}] $y1 \
                -fill #888888 -tags $t
        }
        INDICATORE_SINCRO {
            $c create oval $cx $cy $x1 $y1 -fill #f5f5f5 -outline black -tags $t
        }
        DISPLAY - DISPLAY_SCALATO {
            $c create rectangle $cx $cy $x1 $y1 -fill #10241a -outline black -tags $t
            $c create text [expr {($cx + $x1) / 2}] [expr {($cy + $y1) / 2}] \
                -text "8" -fill #34d834 -font {Courier 7} -tags $t
        }
        SET_VALORE {
            $c create rectangle $cx $cy $x1 $y1 -fill white -outline #333333 -tags $t
            $c create text [expr {($cx + $x1) / 2}] [expr {($cy + $y1) / 2}] \
                -text "#" -font {Helvetica 7} -tags $t
        }
        STRINGA {
            set testo [oggetto_campo $oggetto etichetta]
            $c create text $cx $cy -anchor nw -text $testo -font {Helvetica 7} -tags $t
        }
        default {
            $c create rectangle $cx $cy $x1 $y1 -fill #eeeeee -outline black -tags $t
        }
    }
}

#  Il riquadro tratteggiato attorno alla stazione selezionata, se c'e'.
proc ::lgmkstaz::disegna_selezione {} {
    variable stazione_selezionata
    variable modello
    variable altezza_pagina_celle

    set c .corpo.destra.canvas
    $c delete selezione
    if {$stazione_selezionata eq ""} { return }
    set i [modello_indice_stazione $modello $stazione_selezionata]
    if {$i < 0} { set stazione_selezionata ""; return }
    set s [lindex [dict get $modello stazioni] $i]
    lassign [riquadro_celle [dict get $s posx] [dict get $s posy] \
                 [dict get $s larg] [dict get $s altezza] $altezza_pagina_celle] x0 y0 x1 y1
    $c create rectangle [expr {$x0 - 2}] [expr {$y0 - 2}] [expr {$x1 + 2}] [expr {$y1 + 2}] \
        -outline #1450a3 -width 2 -dash {4 2} -tags selezione
}

# ---------------------------------------------------------------------------
# La stazione (dict) sotto un punto del canvas, o "" se sfondo/griglia.
# Tutte le interazioni del mouse passano da qui, invece che da un binding per
# oggetto: un solo posto dove capire "su cosa ho cliccato", niente
# ambiguita' fra i binding a livello di widget e quelli a livello di item.
# ---------------------------------------------------------------------------

proc ::lgmkstaz::stazione_sotto {x_widget y_widget} {
    variable modello
    set c .corpo.destra.canvas
    set cx [$c canvasx $x_widget]
    set cy [$c canvasy $y_widget]
    foreach item [$c find overlapping $cx $cy $cx $cy] {
        foreach tag [$c gettags $item] {
            if {[regexp {^staz(\d+)$} $tag -> id]} {
                set i [modello_indice_stazione $modello $id]
                if {$i >= 0} { return [lindex [dict get $modello stazioni] $i] }
            }
        }
    }
    return ""
}

# ---------------------------------------------------------------------------
# Libreria: piazzare una stazione nuova.
# ---------------------------------------------------------------------------

#  L'ingombro di un tipo in celle, "largXaltezza" (es. 2x1): e' quanto spazio
#  occupera' la stazione nella pagina, l'unita' e' la cella da 62 px.
proc ::lgmkstaz::misura_tipo {tipo} {
    variable catalogo
    if {![info exists catalogo($tipo)]} { return "" }
    lassign $catalogo($tipo) larg altezza sequenza
    return "${larg}x${altezza}"
}

proc ::lgmkstaz::libreria_seleziona {} {
    set sel [.corpo.sinistra.libreria.lista curselection]
    if {[llength $sel] == 0} {
        set ::lgmkstaz::tipo_armato ""
    } else {
        set ::lgmkstaz::tipo_armato [.corpo.sinistra.libreria.lista get [lindex $sel 0]]
    }
    aggiorna_cursore
}

proc ::lgmkstaz::aggiorna_cursore {} {
    variable tipo_armato
    .corpo.destra.canvas configure -cursor [expr {$tipo_armato ne "" ? "crosshair" : ""}]
    if {$tipo_armato ne ""} {
        stato "Libreria: $tipo_armato [misura_tipo $tipo_armato] -\
               clic sul canvas per piazzare, Esc per smettere"
    } else {
        stato ""
    }
}

proc ::lgmkstaz::annulla_armato {} {
    set ::lgmkstaz::tipo_armato ""
    .corpo.sinistra.libreria.lista selection clear 0 end
    aggiorna_cursore
}

#  Filtra la libreria mentre si scrive nel campo di ricerca: un elenco di 54
#  nomi si scorre in fretta, ma cercare "P1L" per i pulsanti a una spia e'
#  piu' rapido che scorrerli tutti a occhio.
proc ::lgmkstaz::libreria_filtra {} {
    variable catalogo
    set filtro [string toupper [string trim $::lgmkstaz::_cerca_libreria]]
    .corpo.sinistra.libreria.lista delete 0 end
    foreach tipo [lsort [array names catalogo]] {
        if {$filtro eq "" || [string first $filtro $tipo] >= 0} {
            .corpo.sinistra.libreria.lista insert end $tipo
        }
    }
}

proc ::lgmkstaz::piazza_stazione {tipo x_widget y_widget} {
    variable modello
    variable pagina_disegnata
    variable altezza_pagina_celle
    variable prossimo_id
    variable catalogo
    variable dim_cella_px

    if {$pagina_disegnata eq ""} {
        tk_messageBox -icon warning -title lgmkstaz \
            -message "Apri o crea prima una pagina (File -> Nuova pagina...)."
        return
    }
    set c .corpo.destra.canvas
    lassign $catalogo($tipo) larg altezza sequenza

    #  il riquadro nasce CENTRATO sul clic (non con lo spigolo li'): si
    #  agisce sull'angolo alto-sinistra spostato di meta' dimensione.
    set cx [expr {[$c canvasx $x_widget] - ($larg * $dim_cella_px) / 2.0}]
    set cy [expr {[$c canvasy $y_widget] - ($altezza * $dim_cella_px) / 2.0}]
    lassign [cella_da_pixel $cx $cy $larg $altezza $altezza_pagina_celle] posx posy
    if {$posx < 0} { set posx 0 }
    if {$posy < 0} { set posy 0 }

    set stazioni_pagina {}
    foreach st [dict get $modello stazioni] {
        if {[dict get $st pagina] == $pagina_disegnata} { lappend stazioni_pagina $st }
    }
    set avviso ""
    if {[stazioni_sovrapposte $stazioni_pagina $posx $posy $larg $altezza -1]} {
        set avviso "  (si sovrappone a un'altra stazione)"
    }

    set nuova [crea_stazione_vuota $prossimo_id $tipo $pagina_disegnata $posx $posy]
    incr prossimo_id
    set modello [modello_aggiungi_stazione $modello $nuova]
    imposta_modificato 1
    set stazione_selezionata [dict get $nuova id]
    set ::lgmkstaz::stazione_selezionata $stazione_selezionata
    disegna_pagina $pagina_disegnata
    stato "Piazzata $tipo in $posx,$posy$avviso"
}

# ---------------------------------------------------------------------------
# Selezione, trascinamento, cancellazione - le interazioni del mouse sulle
# stazioni gia' esistenti (quando la libreria non e' armata).
# ---------------------------------------------------------------------------

proc ::lgmkstaz::canvas_press {x y} {
    variable tipo_armato
    variable stazione_selezionata
    variable altezza_pagina_celle
    variable drag_id
    variable drag_dx
    variable drag_dy
    variable drag_partito

    focus .corpo.destra.canvas
    set drag_partito 0

    if {$tipo_armato ne ""} {
        piazza_stazione $tipo_armato $x $y
        return
    }

    set s [stazione_sotto $x $y]
    if {$s eq ""} {
        set stazione_selezionata ""
        set drag_id ""
        disegna_selezione
        return
    }

    set stazione_selezionata [dict get $s id]
    disegna_selezione

    set c .corpo.destra.canvas
    lassign [riquadro_celle [dict get $s posx] [dict get $s posy] \
                 [dict get $s larg] [dict get $s altezza] $altezza_pagina_celle] x0 y0 x1 y1
    set drag_id [dict get $s id]
    set drag_dx [expr {[$c canvasx $x] - $x0}]
    set drag_dy [expr {[$c canvasy $y] - $y0}]
}

#  Durante il trascinamento sposta solo gli ITEM sul canvas (feedback
#  visivo): il modello non cambia finche' non si rilascia il pulsante, dove
#  la posizione si aggancia alla griglia (canvas_release).
proc ::lgmkstaz::canvas_motion {x y} {
    variable drag_id
    variable drag_dx
    variable drag_dy
    variable drag_partito

    if {$drag_id eq ""} { return }
    set c .corpo.destra.canvas
    set tag "staz$drag_id"
    set bbox [$c bbox $tag]
    if {$bbox eq ""} { return }
    set nuovo_x0 [expr {[$c canvasx $x] - $drag_dx}]
    set nuovo_y0 [expr {[$c canvasy $y] - $drag_dy}]
    set dx [expr {$nuovo_x0 - [lindex $bbox 0]}]
    set dy [expr {$nuovo_y0 - [lindex $bbox 1]}]
    $c move $tag $dx $dy
    $c move selezione $dx $dy
    set drag_partito 1
}

proc ::lgmkstaz::canvas_release {x y} {
    variable drag_id
    variable drag_dx
    variable drag_dy
    variable drag_partito
    variable modello
    variable altezza_pagina_celle
    variable pagina_disegnata

    if {$drag_id eq "" || !$drag_partito} { set drag_id ""; return }
    set id $drag_id
    set drag_id ""

    set i [modello_indice_stazione $modello $id]
    if {$i < 0} { return }
    set s [lindex [dict get $modello stazioni] $i]
    set larg [dict get $s larg]
    set altezza [dict get $s altezza]

    set c .corpo.destra.canvas
    set nuovo_x0 [expr {[$c canvasx $x] - $drag_dx}]
    set nuovo_y0 [expr {[$c canvasy $y] - $drag_dy}]
    lassign [cella_da_pixel $nuovo_x0 $nuovo_y0 $larg $altezza $altezza_pagina_celle] posx posy
    if {$posx < 0} { set posx 0 }
    if {$posy < 0} { set posy 0 }

    set stazioni_pagina {}
    foreach st [dict get $modello stazioni] {
        if {[dict get $st pagina] == [dict get $s pagina]} { lappend stazioni_pagina $st }
    }
    set avviso ""
    if {[stazioni_sovrapposte $stazioni_pagina $posx $posy $larg $altezza $id]} {
        set avviso "  (si sovrappone a un'altra stazione)"
    }

    set modello [modello_sposta_stazione $modello $id $posx $posy]
    imposta_modificato 1
    set ::lgmkstaz::stazione_selezionata $id
    disegna_pagina $pagina_disegnata
    stato "Spostata in $posx,$posy$avviso"
}

proc ::lgmkstaz::canvas_hover {x y} {
    variable tipo_armato
    if {$tipo_armato ne ""} { return }
    set s [stazione_sotto $x $y]
    if {$s eq ""} { stato ""; return }
    mostra_info $s
}

proc ::lgmkstaz::canvas_double {x y} {
    set s [stazione_sotto $x $y]
    if {$s eq ""} { return }
    if {[dict get $s grezzo]} {
        tk_messageBox -icon info -title lgmkstaz \
            -message "[dict get $s tipo]: tipo storico o sconosciuto,\
                      lgmkstaz non lo modifica (solo spostare o cancellare)."
        return
    }
    apri_proprieta [dict get $s id]
}

# ---------------------------------------------------------------------------
# Copia e incolla di una stazione.
#
# Si copia la stazione INTERA, com'e' nel modello: tipo, descrizione e tutti i
# valori degli oggetti (colori, riferimenti alle variabili, scalamenti). E'
# questo che serve davvero - rifare a mano le stesse connessioni su una
# stazione gemella e' il lavoro che si vuole evitare.
#
# Quello che NON si copia e' cio' che identifica la stazione nella pagina:
# l'id (ne serve uno nuovo), la pagina e la posizione (le decide l'incolla) e
# il NUMERO, che si azzera. Un numero duplicato nel file sarebbe un errore
# vero per compstaz; lasciandolo vuoto la stazione incollata si comporta come
# una appena piazzata (scrivi_stazione scrive "NUMERO" nudo).
#
# Gli appunti sono interni a lgmkstaz, non la selezione X11: si incolla in
# questa finestra, anche fra pagine diverse dello stesso file.
# ---------------------------------------------------------------------------

proc ::lgmkstaz::copia_selezionata {} {
    variable stazione_selezionata
    variable modello
    variable appunti

    if {$stazione_selezionata eq ""} {
        stato "Niente da copiare: nessuna stazione selezionata."
        return
    }
    set i [modello_indice_stazione $modello $stazione_selezionata]
    if {$i < 0} { return }
    set appunti [lindex [dict get $modello stazioni] $i]
    stato "Copiata [dict get $appunti tipo] - Ctrl+V per incollarla"
}

#  Dove finisce la stazione incollata: sotto il puntatore, se e' sul canvas
#  (come il piazzamento dalla libreria, riquadro CENTRATO sul punto); se il
#  puntatore e' altrove - per esempio si e' appena usato un menu - una cella
#  in diagonale rispetto all'originale, cosi' la copia non finisce esattamente
#  sopra di esso e si vede che ce ne sono due.
proc ::lgmkstaz::posizione_incolla {larg altezza} {
    variable appunti
    variable altezza_pagina_celle
    variable dim_cella_px

    set c .corpo.destra.canvas
    lassign [winfo pointerxy $c] sx sy
    set wx [expr {$sx - [winfo rootx $c]}]
    set wy [expr {$sy - [winfo rooty $c]}]
    if {$wx >= 0 && $wy >= 0 && $wx < [winfo width $c] && $wy < [winfo height $c]} {
        set cx [expr {[$c canvasx $wx] - ($larg * $dim_cella_px) / 2.0}]
        set cy [expr {[$c canvasy $wy] - ($altezza * $dim_cella_px) / 2.0}]
        lassign [cella_da_pixel $cx $cy $larg $altezza $altezza_pagina_celle] posx posy
    } else {
        set posx [expr {[dict get $appunti posx] + 1}]
        set posy [expr {[dict get $appunti posy] + 1}]
    }
    if {$posx < 0} { set posx 0 }
    if {$posy < 0} { set posy 0 }
    return [list $posx $posy]
}

proc ::lgmkstaz::incolla {} {
    variable appunti
    variable modello
    variable pagina_disegnata
    variable prossimo_id
    variable stazione_selezionata

    if {$appunti eq ""} {
        stato "Niente da incollare: copia prima una stazione con Ctrl+C."
        return
    }
    if {$pagina_disegnata eq ""} {
        tk_messageBox -icon warning -title lgmkstaz \
            -message "Apri o crea prima una pagina (File -> Nuova pagina...)."
        return
    }

    set larg [dict get $appunti larg]
    set altezza [dict get $appunti altezza]
    lassign [posizione_incolla $larg $altezza] posx posy

    set stazioni_pagina {}
    foreach st [dict get $modello stazioni] {
        if {[dict get $st pagina] == $pagina_disegnata} { lappend stazioni_pagina $st }
    }
    set avviso ""
    if {[stazioni_sovrapposte $stazioni_pagina $posx $posy $larg $altezza -1]} {
        set avviso "  (si sovrappone a un'altra stazione)"
    }

    set nuova $appunti
    dict set nuova id $prossimo_id
    dict set nuova pagina $pagina_disegnata
    dict set nuova posx $posx
    dict set nuova posy $posy
    dict set nuova numero ""
    incr prossimo_id

    set modello [modello_aggiungi_stazione $modello $nuova]
    imposta_modificato 1
    set stazione_selezionata [dict get $nuova id]
    disegna_pagina $pagina_disegnata
    disegna_selezione
    stato "Incollata [dict get $nuova tipo] in $posx,$posy$avviso"
}

proc ::lgmkstaz::elimina_selezionata {} {
    variable stazione_selezionata
    variable modello
    variable pagina_disegnata

    if {$stazione_selezionata eq ""} { return }
    set modello [modello_elimina_stazione $modello $stazione_selezionata]
    set stazione_selezionata ""
    imposta_modificato 1
    disegna_pagina $pagina_disegnata
}

proc ::lgmkstaz::mostra_info {stazione} {
    set testo [dict get $stazione tipo]
    set d [dict get $stazione descrizione]
    if {$d ne ""} { append testo " - $d" }
    append testo "  (pagina [dict get $stazione pagina], cella\
                  [dict get $stazione posx],[dict get $stazione posy],\
                  [dict get $stazione larg]x[dict get $stazione altezza] celle)"
    if {[dict get $stazione grezzo]} {
        append testo "  -  [dict get $stazione nota]"
    }
    stato $testo
}

# ---------------------------------------------------------------------------
# Pannello proprieta': i campi di una stazione, generati dalla grammatica
# (lgmkstaz_dati.tcl) invece che scritti a mano per ognuno dei 54 tipi. Ogni
# campo diventa una riga con i widget adatti alla sua forma (colore -> menu,
# riferimento -> due entry, numero -> un'entry, minmax -> due entry). I
# valori dei widget si tengono in ::lgmkstaz::_campo(<indice_oggetto>,<indice
# campo>,<sotto-nome>), ripuliti a ogni apertura del pannello.
# ---------------------------------------------------------------------------

array set ::lgmkstaz::_etichetta_campo {
    input          "Input:"
    input_neg      "Input:"
    input_blink_neg "Input blink:"
    input_err      "Input err.:"
    inibizione     "Inibizione:"
    scalamento     "Scalamento:"
    offset         "Offset:"
    scalamento_err "Scalamento err.:"
    minmax         "Min/Max:"
    minmax_err     "Min/Max err.:"
}

proc ::lgmkstaz::apri_proprieta {id} {
    variable modello
    variable _etichetta_campo

    set i [modello_indice_stazione $modello $id]
    if {$i < 0} { return }
    set s [lindex [dict get $modello stazioni] $i]

    array unset ::lgmkstaz::_campo

    set w .lgmkstaz_proprieta
    catch {destroy $w}
    toplevel $w
    wm title $w "Stazione: [dict get $s tipo]"
    wm transient $w .

    frame $w.testa
    pack $w.testa -fill x -padx 8 -pady 6
    label $w.testa.l1 -text "Tipo:" -anchor w
    label $w.testa.v1 -text [dict get $s tipo] -anchor w -font {Helvetica 9 bold}
    label $w.testa.l2 -text "Descrizione:" -anchor w
    set ::lgmkstaz::_prop_descrizione [dict get $s descrizione]
    entry $w.testa.v2 -width 40 -textvariable ::lgmkstaz::_prop_descrizione
    label $w.testa.l3 -text "Posizione:" -anchor w
    label $w.testa.v3 -anchor w -text \
        "pagina [dict get $s pagina], cella [dict get $s posx],[dict get $s posy]\
         ([dict get $s larg]x[dict get $s altezza] celle - si sposta trascinando)"
    grid $w.testa.l1 $w.testa.v1 -sticky w -pady 1
    grid $w.testa.l2 $w.testa.v2 -sticky w -pady 1
    grid $w.testa.l3 $w.testa.v3 -sticky w -pady 1
    grid columnconfigure $w.testa 1 -weight 1

    frame $w.sep -height 2 -relief groove -bd 1
    pack $w.sep -fill x -padx 8 -pady 4

    set corpo [frame $w.corpo]
    pack $corpo -fill both -expand 1 -padx 8

    set indice_oggetto 0
    foreach oggetto [dict get $s oggetti] {
        costruisci_editor_oggetto $corpo $indice_oggetto $oggetto
        incr indice_oggetto
    }

    frame $w.btns
    pack $w.btns -pady 8
    button $w.btns.ok -text OK -width 10 -default active \
        -command [list ::lgmkstaz::salva_proprieta $w $id]
    button $w.btns.cancel -text Annulla -width 10 -command [list destroy $w]
    pack $w.btns.ok $w.btns.cancel -side left -padx 6

    bind $w <Return> [list ::lgmkstaz::salva_proprieta $w $id]
    bind $w <Escape> [list destroy $w]
}

# ---------------------------------------------------------------------------
# Il selettore di variabile ("..." accanto a un campo INPUT/OUTPUT): un
# elenco filtrabile delle variabili vere del simulatore, dallo stesso
# genere del campo (uscita per INPUT-simili, ingresso libero per OUTPUT) -
# vedi lgmkstaz_topologia.tcl. Stesso interaction design del resto del
# progetto (scrivere filtra, doppio clic conferma - come il remap dei
# blocchi in animate.tcl/hmielem.tcl), riscritto qui invece di sourciare
# hmielem.tcl: quel file porta con se' lo stato di draw2gr/legopc (il
# canvas del modello, blocNvar/blocVars) che lgmkstaz non ha.
# ---------------------------------------------------------------------------

#  Richiamata dal pulsante "..." di un campo: apre il selettore e, se
#  l'utente sceglie qualcosa, riempie var/mod di quel campo.
proc ::lgmkstaz::scegli_e_riempi {chiave genere} {
    set risultato [scegli_variabile .lgmkstaz_proprieta $genere]
    if {[llength $risultato] != 2} { return }
    lassign $risultato var mod
    set ::lgmkstaz::_campo($chiave,var) $var
    set ::lgmkstaz::_campo($chiave,mod) $mod
}

#  L'elenco combinato di tutti i modelli: ogni "nome" e' "VAR@MODELLO", cosi'
#  il filtro (sottostringa, senza distinguere maiuscole/minuscole) trova sia
#  scrivendo il nome della variabile sia quello del modello.
proc ::lgmkstaz::_scelta_filtra {} {
    variable _scelta_nomi
    variable _scelta_filtro
    variable _scelta_mostrati
    set lb .lgmkstaz_scegli.l.lb
    set filtro [string toupper [string trim $_scelta_filtro]]
    $lb delete 0 end
    set _scelta_mostrati {}
    foreach n $_scelta_nomi {
        if {$filtro ne "" && [string first $filtro [string toupper $n]] < 0} continue
        lappend _scelta_mostrati $n
        lassign [split $n @] var mod
        $lb insert end [format "%-10s %-8s %s" $var $mod [topo_descrizione $mod $var]]
    }
}

proc ::lgmkstaz::_scelta_conferma {} {
    variable _scelta_mostrati
    variable _scelta_risultato
    variable _scelta_fatta
    set sel [.lgmkstaz_scegli.l.lb curselection]
    if {[llength $sel] == 0} { return }
    set _scelta_risultato [split [lindex $_scelta_mostrati [lindex $sel 0]] @]
    set _scelta_fatta 1
}

proc ::lgmkstaz::_scelta_annulla {} {
    set ::lgmkstaz::_scelta_risultato {}
    set ::lgmkstaz::_scelta_fatta 1
}

#  Il selettore vero e proprio: modale (vwait), ritorna {var mod} scelti o
#  {} se annullato/chiuso. $genere e' "uscita" o "ingresso" (vedi
#  lgmkstaz_topologia.tcl). Senza una topologia caricata avvisa e basta: il
#  campo resta da scrivere a mano, come prima di questa fase.
proc ::lgmkstaz::scegli_variabile {parent genere} {
    variable topo_modelli
    variable _scelta_nomi
    variable _scelta_filtro
    variable _scelta_fatta
    variable _scelta_risultato

    if {[llength $topo_modelli] == 0} {
        tk_messageBox -icon info -title lgmkstaz -parent $parent -message \
            "Nessuna topologia caricata (manca variabili.edf accanto al file):\
             il nome si scrive a mano."
        return {}
    }

    set _scelta_nomi {}
    foreach mod $topo_modelli {
        set lista [expr {$genere eq "uscita" ? [topo_variabili_uscita $mod] \
                                              : [topo_variabili_ingresso $mod]}]
        foreach var $lista { lappend _scelta_nomi "$var@$mod" }
    }
    set _scelta_nomi [lsort $_scelta_nomi]
    set _scelta_filtro ""
    set _scelta_fatta 0
    set _scelta_risultato {}

    set d .lgmkstaz_scegli
    catch {destroy $d}
    toplevel $d
    wm title $d [expr {$genere eq "uscita" ? "Scegli una variabile (uscita di un modello)" \
                                            : "Scegli una variabile (ingresso libero di un modello)"}]
    wm transient $d $parent

    frame $d.f
    pack $d.f -fill x -padx 8 -pady 6
    label $d.f.l -text "Filtro:"
    entry $d.f.e -textvariable ::lgmkstaz::_scelta_filtro -width 30
    pack $d.f.l -side left
    pack $d.f.e -side left -fill x -expand 1 -padx 4

    frame $d.l
    pack $d.l -fill both -expand 1 -padx 8
    listbox $d.l.lb -height 14 -width 55 -font {Courier 10} \
        -yscrollcommand [list $d.l.sb set]
    scrollbar $d.l.sb -command [list $d.l.lb yview]
    pack $d.l.sb -side right -fill y
    pack $d.l.lb -side left -fill both -expand 1

    bind $d.f.e <KeyRelease> ::lgmkstaz::_scelta_filtra
    bind $d.l.lb <Double-1> ::lgmkstaz::_scelta_conferma
    ::lgmkstaz::_scelta_filtra

    frame $d.b
    pack $d.b -pady 6
    button $d.b.ok -text OK -width 10 -default active -command ::lgmkstaz::_scelta_conferma
    button $d.b.no -text Annulla -width 10 -command ::lgmkstaz::_scelta_annulla
    pack $d.b.ok $d.b.no -side left -padx 6

    bind $d <Escape> ::lgmkstaz::_scelta_annulla
    wm protocol $d WM_DELETE_WINDOW ::lgmkstaz::_scelta_annulla
    focus $d.f.e

    vwait ::lgmkstaz::_scelta_fatta
    destroy $d
    return $_scelta_risultato
}

proc ::lgmkstaz::costruisci_editor_oggetto {padre indice_oggetto oggetto} {
    variable colori
    variable perturbazioni
    variable _etichetta_campo

    set tipo [dict get $oggetto tipo]
    set f [frame $padre.o$indice_oggetto -relief groove -bd 1]
    pack $f -fill x -pady 3

    label $f.titolo -text $tipo -font {Helvetica 9 bold} -anchor w
    grid $f.titolo -row 0 -column 0 -sticky w -padx 4 -pady 2

    set riga 1
    set indice_campo 0
    foreach campo [dict get $oggetto campi] {
        set nome [dict get $campo nome]
        set valore [dict get $campo valore]
        set chiave "$indice_oggetto,$indice_campo"

        switch -- $nome {
            colore {
                label $f.l$riga -text "Colore:" -anchor w
                set ::lgmkstaz::_campo($chiave,colore) $valore
                tk_optionMenu $f.v$riga ::lgmkstaz::_campo($chiave,colore) {*}$colori
                grid $f.l$riga -row $riga -column 0 -sticky w -padx 4
                grid $f.v$riga -row $riga -column 1 -sticky w
            }
            etichetta {
                label $f.l$riga -text "Etichetta:" -anchor w
                set ::lgmkstaz::_campo($chiave,etichetta) $valore
                entry $f.v$riga -width 32 -textvariable ::lgmkstaz::_campo($chiave,etichetta)
                grid $f.l$riga -row $riga -column 0 -sticky w -padx 4
                grid $f.v$riga -row $riga -column 1 -sticky w
            }
            input - input_neg - input_blink_neg - input_err - inibizione {
                label $f.l$riga -text $_etichetta_campo($nome) -anchor w
                set ::lgmkstaz::_campo($chiave,var) [dict get $valore var]
                set ::lgmkstaz::_campo($chiave,mod) [dict get $valore mod]
                set ::lgmkstaz::_campo($chiave,not) [dict get $valore not]
                set rv [frame $f.v$riga]
                entry $rv.var -width 12 -textvariable ::lgmkstaz::_campo($chiave,var)
                label $rv.a -text " di "
                entry $rv.mod -width 8 -textvariable ::lgmkstaz::_campo($chiave,mod)
                button $rv.scegli -text "..." -width 2 \
                    -command [list ::lgmkstaz::scegli_e_riempi $chiave uscita]
                pack $rv.var $rv.a $rv.mod $rv.scegli -side left
                if {$nome in {input_neg input_blink_neg}} {
                    checkbutton $rv.not -text NOT -variable ::lgmkstaz::_campo($chiave,not)
                    pack $rv.not -side left -padx 6
                }
                grid $f.l$riga -row $riga -column 0 -sticky w -padx 4
                grid $rv -row $riga -column 1 -sticky w
            }
            output {
                label $f.l$riga -text "Output:" -anchor w
                set ::lgmkstaz::_campo($chiave,var) [dict get $valore var]
                set ::lgmkstaz::_campo($chiave,mod) [dict get $valore mod]
                set modo_letto [dict get $valore modo]
                set ::lgmkstaz::_campo($chiave,modo) \
                    [expr {$modo_letto eq "" ? [lindex $perturbazioni 0] : $modo_letto}]
                set ::lgmkstaz::_campo($chiave,valore) [dict get $valore valore]
                set rv [frame $f.v$riga]
                entry $rv.var -width 12 -textvariable ::lgmkstaz::_campo($chiave,var)
                label $rv.a -text " di "
                entry $rv.mod -width 8 -textvariable ::lgmkstaz::_campo($chiave,mod)
                button $rv.scegli -text "..." -width 2 \
                    -command [list ::lgmkstaz::scegli_e_riempi $chiave ingresso]
                tk_optionMenu $rv.modo ::lgmkstaz::_campo($chiave,modo) {*}$perturbazioni
                label $rv.b -text " val."
                entry $rv.valore -width 6 -textvariable ::lgmkstaz::_campo($chiave,valore)
                pack $rv.var $rv.a $rv.mod $rv.scegli $rv.modo $rv.b $rv.valore -side left
                grid $f.l$riga -row $riga -column 0 -sticky w -padx 4
                grid $rv -row $riga -column 1 -sticky w
            }
            scalamento - offset - scalamento_err {
                label $f.l$riga -text $_etichetta_campo($nome) -anchor w
                set ::lgmkstaz::_campo($chiave,numero) $valore
                entry $f.v$riga -width 10 -textvariable ::lgmkstaz::_campo($chiave,numero)
                grid $f.l$riga -row $riga -column 0 -sticky w -padx 4
                grid $f.v$riga -row $riga -column 1 -sticky w
            }
            minmax - minmax_err {
                label $f.l$riga -text $_etichetta_campo($nome) -anchor w
                set ::lgmkstaz::_campo($chiave,min) [lindex $valore 0]
                set ::lgmkstaz::_campo($chiave,max) [lindex $valore 1]
                set rv [frame $f.v$riga]
                entry $rv.min -width 8 -textvariable ::lgmkstaz::_campo($chiave,min)
                label $rv.a -text " - "
                entry $rv.max -width 8 -textvariable ::lgmkstaz::_campo($chiave,max)
                pack $rv.min $rv.a $rv.max -side left
                grid $f.l$riga -row $riga -column 0 -sticky w -padx 4
                grid $rv -row $riga -column 1 -sticky w
            }
        }
        incr riga
        incr indice_campo
    }
}

#  L'inverso di costruisci_editor_oggetto: dai widget al valore del campo,
#  con la stessa validazione che leggi_campo applica leggendo un file (colore
#  ammesso, modo di perturbazione ammesso, numeri veri) - cosi' il pannello
#  non puo' salvare qualcosa che poi lgmkstaz stesso non saprebbe rileggere.
proc ::lgmkstaz::leggi_valore_campo {nome chiave} {
    switch -- $nome {
        colore {
            set v $::lgmkstaz::_campo($chiave,colore)
            valida_colore $v
            return $v
        }
        etichetta {
            set v [string trim $::lgmkstaz::_campo($chiave,etichetta)]
            variable lun_etichetta
            if {[string length $v] > $lun_etichetta} { set v [string range $v 0 [expr {$lun_etichetta-1}]] }
            return $v
        }
        input - input_err - inibizione {
            set var [string trim $::lgmkstaz::_campo($chiave,var)]
            set mod [string trim $::lgmkstaz::_campo($chiave,mod)]
            if {($var eq "") != ($mod eq "")} {
                error "servono sia la variabile sia il modello, o nessuno dei due"
            }
            topo_verifica_riferimento $var $mod uscita
            return [dict create var $var mod $mod not 0]
        }
        input_neg - input_blink_neg {
            set var [string trim $::lgmkstaz::_campo($chiave,var)]
            set mod [string trim $::lgmkstaz::_campo($chiave,mod)]
            if {($var eq "") != ($mod eq "")} {
                error "servono sia la variabile sia il modello, o nessuno dei due"
            }
            topo_verifica_riferimento $var $mod uscita
            return [dict create var $var mod $mod not $::lgmkstaz::_campo($chiave,not)]
        }
        output {
            set var [string trim $::lgmkstaz::_campo($chiave,var)]
            set mod [string trim $::lgmkstaz::_campo($chiave,mod)]
            if {$var eq "" && $mod eq ""} {
                return [dict create var "" mod "" modo "" valore ""]
            }
            if {$var eq "" || $mod eq ""} {
                error "servono sia la variabile sia il modello, o nessuno dei due"
            }
            topo_verifica_riferimento $var $mod ingresso
            set modo $::lgmkstaz::_campo($chiave,modo)
            valida_perturbazione $modo
            set valore [string trim $::lgmkstaz::_campo($chiave,valore)]
            if {$valore ne ""} { rl_valida_numero $valore "valore di OUTPUT" }
            return [dict create var $var mod $mod modo $modo valore $valore]
        }
        scalamento - offset - scalamento_err {
            set v [string trim $::lgmkstaz::_campo($chiave,numero)]
            rl_valida_numero $v $nome
            return $v
        }
        minmax - minmax_err {
            set mn [string trim $::lgmkstaz::_campo($chiave,min)]
            set mx [string trim $::lgmkstaz::_campo($chiave,max)]
            rl_valida_numero $mn "$nome (minimo)"
            rl_valida_numero $mx "$nome (massimo)"
            return [list $mn $mx]
        }
        default { error "leggi_valore_campo: campo sconosciuto '$nome' (errore interno)" }
    }
}

proc ::lgmkstaz::salva_proprieta {w id} {
    variable modello
    variable pagina_disegnata

    set i [modello_indice_stazione $modello $id]
    if {$i < 0} { destroy $w; return }
    set s [lindex [dict get $modello stazioni] $i]

    set nuovi_oggetti {}
    set indice_oggetto 0
    foreach oggetto [dict get $s oggetti] {
        set nuovi_campi {}
        set indice_campo 0
        foreach campo [dict get $oggetto campi] {
            set nome [dict get $campo nome]
            set chiave "$indice_oggetto,$indice_campo"
            if {[catch {leggi_valore_campo $nome $chiave} nuovo_valore err]} {
                tk_messageBox -icon error -title lgmkstaz -parent $w \
                    -message "[dict get $oggetto tipo], campo $nome:\n$err"
                return
            }
            lappend nuovi_campi [dict create nome $nome valore $nuovo_valore]
            incr indice_campo
        }
        lappend nuovi_oggetti [dict create tipo [dict get $oggetto tipo] campi $nuovi_campi]
        incr indice_oggetto
    }

    set descrizione [string trim $::lgmkstaz::_prop_descrizione]
    if {[string length $descrizione] > 200} { set descrizione [string range $descrizione 0 199] }
    dict set s descrizione $descrizione
    dict set s oggetti $nuovi_oggetti

    set modello [modello_sostituisci_stazione $modello $id $s]
    imposta_modificato 1
    destroy $w
    disegna_pagina $pagina_disegnata
}

# ---------------------------------------------------------------------------
# Nuova pagina.
# ---------------------------------------------------------------------------

proc ::lgmkstaz::nuova_pagina_dialogo {} {
    variable modello
    if {$modello eq ""} {
        tk_messageBox -icon warning -title lgmkstaz -message "Apri prima un r01.dat."
        return
    }

    set numeri {}
    foreach p [dict get $modello pagine] { lappend numeri [dict get $p numero] }
    set n 1
    while {$n in $numeri} { incr n }
    set ::lgmkstaz::_pag_numero $n
    set ::lgmkstaz::_pag_nome ""
    set ::lgmkstaz::_pag_descrizione ""

    set w .lgmkstaz_nuovapag
    catch {destroy $w}
    toplevel $w
    wm title $w "Nuova pagina"
    wm transient $w .
    wm resizable $w 0 0

    frame $w.f
    pack $w.f -padx 10 -pady 8
    label $w.f.l1 -text "Numero:" -anchor w
    entry $w.f.v1 -textvariable ::lgmkstaz::_pag_numero -width 8
    label $w.f.l2 -text "Nome (max 8, senza spazi):" -anchor w
    entry $w.f.v2 -textvariable ::lgmkstaz::_pag_nome -width 12
    label $w.f.l3 -text "Descrizione:" -anchor w
    entry $w.f.v3 -textvariable ::lgmkstaz::_pag_descrizione -width 40
    grid $w.f.l1 $w.f.v1 -sticky w -pady 2
    grid $w.f.l2 $w.f.v2 -sticky w -pady 2
    grid $w.f.l3 $w.f.v3 -sticky w -pady 2

    frame $w.btns
    pack $w.btns -pady 8
    button $w.btns.ok -text Crea -width 10 -default active \
        -command [list ::lgmkstaz::crea_pagina_da_dialogo $w]
    button $w.btns.cancel -text Annulla -width 10 -command [list destroy $w]
    pack $w.btns.ok $w.btns.cancel -side left -padx 6

    bind $w <Return> [list ::lgmkstaz::crea_pagina_da_dialogo $w]
    bind $w <Escape> [list destroy $w]
}

proc ::lgmkstaz::crea_pagina_da_dialogo {w} {
    variable modello

    set numero $::lgmkstaz::_pag_numero
    set nome $::lgmkstaz::_pag_nome
    set descrizione $::lgmkstaz::_pag_descrizione
    if {[catch {valida_pagina $modello $numero $nome $descrizione ""} err]} {
        tk_messageBox -icon error -title lgmkstaz -parent $w -message $err
        return
    }
    set modello [modello_aggiungi_pagina $modello \
                     [dict create numero $numero nome $nome descrizione $descrizione]]
    imposta_modificato 1
    destroy $w
    popola_lista_pagine
    seleziona_pagina_in_lista $numero
}

proc ::lgmkstaz::seleziona_pagina_in_lista {numero} {
    variable modello
    set pagine [dict get $modello pagine]
    for {set i 0} {$i < [llength $pagine]} {incr i} {
        if {[dict get [lindex $pagine $i] numero] == $numero} {
            .corpo.sinistra.pagine.lista selection clear 0 end
            .corpo.sinistra.pagine.lista selection set $i
            .corpo.sinistra.pagine.lista see $i
            disegna_pagina $numero
            return
        }
    }
}

# ---------------------------------------------------------------------------
# Apertura, salvataggio, stato "modificato".
# ---------------------------------------------------------------------------

proc ::lgmkstaz::imposta_modificato {v} {
    variable modificato
    set modificato $v
    aggiorna_titolo
}

#  Vero se si puo' procedere (niente da salvare, o l'utente ha scelto cosa
#  fare): usato prima di aprire un altro file, ricaricare, o uscire, cosi'
#  una modifica non si perde per un clic distratto.
proc ::lgmkstaz::conferma_scarto_modifiche {} {
    variable modificato
    if {!$modificato} { return 1 }
    set risposta [tk_messageBox -icon warning -type yesnocancel -title lgmkstaz \
                      -message "Ci sono modifiche non salvate. Salvarle prima di continuare?"]
    switch -- $risposta {
        yes    { salva; return [expr {!$::lgmkstaz::modificato}] }
        no     { return 1 }
        cancel { return 0 }
    }
}

proc ::lgmkstaz::apri_file {percorso} {
    variable modello
    variable percorso_corrente
    variable prossimo_id
    variable stazione_selezionata
    variable tipo_armato

    if {![conferma_scarto_modifiche]} { return 0 }

    if {[catch {::lgmkstaz::leggi_file $percorso} risultato]} {
        tk_messageBox -icon error -title "lgmkstaz - errore di lettura" \
            -message "$percorso:\n\n$risultato"
        return 0
    }

    #  una copia di sicurezza dell'originale, presa la PRIMA volta che si
    #  apre questo file in questa sessione (non a ogni salvataggio: quello
    #  che conta e' com'era PRIMA di qualunque modifica). Non si sovrascrive
    #  un .bak gia' presente - non e' compito di lgmkstaz deciderne il turnover.
    if {![file exists "$percorso.bak"]} {
        catch { file copy $percorso "$percorso.bak" }
    }

    set modello $risultato
    set percorso_corrente $percorso
    set stazione_selezionata ""
    set tipo_armato ""
    .corpo.sinistra.libreria.lista selection clear 0 end

    set massimo -1
    foreach s [dict get $modello stazioni] {
        if {[dict get $s id] > $massimo} { set massimo [dict get $s id] }
    }
    set prossimo_id [expr {$massimo + 1}]

    #  variabili.edf accanto al file, se c'e': da' l'autocomplete e la
    #  verifica delle variabili nel pannello proprieta'. Non e' un errore se
    #  manca (non tutte le directory ce l'hanno) - i campi restano testo
    #  libero, come prima di questa fase.
    set edf [topo_trova $percorso]
    if {$edf ne ""} {
        if {[catch {topo_carica $edf} n]} {
            topo_reset
            stato "variabili.edf trovato ma non leggibile: $n"
        } else {
            stato "Topologia caricata: $n modelli ($edf)"
        }
    } else {
        topo_reset
    }

    imposta_modificato 0
    popola_lista_pagine
    return 1
}

proc ::lgmkstaz::apri_dialogo {} {
    if {![conferma_scarto_modifiche]} { return }
    set iniziale [pwd]
    variable percorso_corrente
    if {$percorso_corrente ne ""} { set iniziale [file dirname $percorso_corrente] }
    set f [tk_getOpenFile -title "Apri r01.dat" -initialdir $iniziale \
               -filetypes {{{r01.dat} {r01.dat}} {{Tutti i file} {*}}}]
    if {$f ne ""} { apri_file $f }
}

proc ::lgmkstaz::ricarica {} {
    variable percorso_corrente
    if {$percorso_corrente ne ""} { apri_file $percorso_corrente }
}

proc ::lgmkstaz::salva {} {
    variable percorso_corrente
    if {$percorso_corrente eq ""} { salva_come; return }
    salva_su_file $percorso_corrente
}

proc ::lgmkstaz::salva_come {} {
    variable percorso_corrente
    set iniziale [expr {$percorso_corrente ne "" ? [file dirname $percorso_corrente] : [pwd]}]
    set f [tk_getSaveFile -title "Salva r01.dat come" -initialdir $iniziale \
               -initialfile r01.dat \
               -filetypes {{{r01.dat} {r01.dat}} {{Tutti i file} {*}}}]
    if {$f ne ""} { salva_su_file $f }
}

proc ::lgmkstaz::salva_su_file {percorso} {
    variable modello
    variable percorso_corrente

    if {[catch {::lgmkstaz::scrivi_file $percorso $modello} err]} {
        tk_messageBox -icon error -title "lgmkstaz - errore di scrittura" \
            -message "$percorso:\n\n$err"
        return
    }
    set percorso_corrente $percorso
    imposta_modificato 0
    stato "Salvato: $percorso"
}

proc ::lgmkstaz::esci {} {
    if {![conferma_scarto_modifiche]} { return }
    exit
}

# ---------------------------------------------------------------------------
# Elenco delle pagine.
# ---------------------------------------------------------------------------

proc ::lgmkstaz::popola_lista_pagine {} {
    variable modello

    .corpo.sinistra.pagine.lista delete 0 end
    if {$modello eq ""} { return }
    foreach p [dict get $modello pagine] {
        .corpo.sinistra.pagine.lista insert end \
            "[dict get $p numero] - [dict get $p nome]"
    }
    set pagine [dict get $modello pagine]
    if {[llength $pagine] > 0} {
        .corpo.sinistra.pagine.lista selection set 0
        disegna_pagina [dict get [lindex $pagine 0] numero]
    } else {
        disegna_pagina ""
    }
}

proc ::lgmkstaz::cambia_pagina_da_lista {} {
    variable modello

    set sel [.corpo.sinistra.pagine.lista curselection]
    if {[llength $sel] == 0} { return }
    set p [lindex [dict get $modello pagine] [lindex $sel 0]]
    annulla_armato
    disegna_pagina [dict get $p numero]
}

proc ::lgmkstaz::aggiorna_titolo {} {
    variable modello
    variable percorso_corrente
    variable pagina_disegnata
    variable modificato

    if {$modello eq ""} {
        wm title . "lgmkstaz"
        .corpo.destra.titolo configure -text "Nessun file aperto - File -> Apri..."
        return
    }
    set stella [expr {$modificato ? "*" : ""}]
    wm title . "lgmkstaz - $percorso_corrente$stella"
    if {$pagina_disegnata eq ""} {
        .corpo.destra.titolo configure -text "Nessuna pagina - File -> Nuova pagina..."
        return
    }
    set nstaz 0
    set ngrezze 0
    foreach s [dict get $modello stazioni] {
        if {[dict get $s pagina] == $pagina_disegnata} {
            incr nstaz
            if {[dict get $s grezzo]} { incr ngrezze }
        }
    }
    set testo "Pagina $pagina_disegnata - $nstaz stazioni"
    if {$ngrezze > 0} { append testo " ($ngrezze di tipo storico o sconosciuto)" }
    .corpo.destra.titolo configure -text $testo
}

# ---------------------------------------------------------------------------
# "Compila e verifica" (Fase 4): lgmkstaz_verifica.tcl fa il lavoro vero
# (scratch, chiave SHM isolata, exec di compstaz, pulizia); qui c'e' solo
# l'interazione con l'utente - la conferma prima (decisione dell'utente,
# 2026-09-22: sempre, non solo la prima volta) e la finestra col risultato.
# ---------------------------------------------------------------------------

proc ::lgmkstaz::compila_e_verifica {} {
    variable modello

    if {$modello eq ""} {
        tk_messageBox -icon warning -title lgmkstaz -message "Apri prima un file."
        return
    }

    set chiave [verifica_chiave_isolata]
    set risposta [tk_messageBox -icon question -type okcancel -default ok \
        -title "Compila e verifica" -message \
        "Lancia compstaz per davvero, su una COPIA del modello ATTUALE (anche\
se non salvato) in una directory temporanea nuova - mai il file salvato su\
disco, mai l'originale.\n\n\
Usa una shared memory ISOLATA (chiave $chiave), mai quella di un banco\
operatore vero, e la rimuove appena finito.\n\n\
Procedere?"]
    if {$risposta ne "ok"} { return }

    if {[catch {verifica_prepara_scratch} scratch]} {
        tk_messageBox -icon error -title "Compila e verifica" -message $scratch
        return
    }
    if {[catch {verifica_esegui_compstaz $scratch} esito]} {
        tk_messageBox -icon error -title "Compila e verifica" \
            -message "Errore preparando l'esecuzione:\n$esito"
        catch { file delete -force $scratch }
        return
    }

    mostra_esito_verifica $scratch $esito $chiave
}

#  La finestra con l'esito: OK/ERRORE in testa, poi l'output vero di
#  compstaz (catturato + compstaz.log). Su un successo aggiunge la scelta
#  della pagina e "Anteprima con xstaz" (vedi avvia_anteprima). Su un
#  fallimento la directory scratch NON viene cancellata alla chiusura, per
#  poterla ispezionare - lo si dice in finestra, col percorso.
proc ::lgmkstaz::mostra_esito_verifica {scratch esito chiave} {
    variable modello
    variable pagina_disegnata

    set ok [expr {[dict get $esito esito] eq "ok"}]

    set w .lgmkstaz_esito
    catch {destroy $w}
    toplevel $w
    wm title $w [expr {$ok ? "Compila e verifica - OK" : "Compila e verifica - ERRORE"}]
    wm transient $w .

    label $w.t -anchor w -font {Helvetica 10 bold} \
        -text [expr {$ok ? "Compilazione riuscita." : "Compilazione fallita."}] \
        -fg [expr {$ok ? "#1a7a1a" : "#c21807"}]
    pack $w.t -fill x -padx 8 -pady 6

    frame $w.f
    pack $w.f -fill both -expand 1 -padx 8
    text $w.f.txt -width 90 -height 20 -wrap none -font {Courier 9} \
        -xscrollcommand [list $w.f.hs set] -yscrollcommand [list $w.f.vs set]
    scrollbar $w.f.vs -command [list $w.f.txt yview]
    scrollbar $w.f.hs -orient horizontal -command [list $w.f.txt xview]
    grid $w.f.txt -row 0 -column 0 -sticky nsew
    grid $w.f.vs -row 0 -column 1 -sticky ns
    grid $w.f.hs -row 1 -column 0 -sticky ew
    grid rowconfigure $w.f 0 -weight 1
    grid columnconfigure $w.f 0 -weight 1

    $w.f.txt insert end \
        "[dict get $esito output]\n\n--- compstaz.log ---\n[dict get $esito log]"
    $w.f.txt configure -state disabled

    if {!$ok} {
        label $w.dir -anchor w -justify left -wraplength 640 -text \
            "Directory scratch NON cancellata, per ispezionarla:\n$scratch"
        pack $w.dir -fill x -padx 8 -pady {4 0}
    } else {
        frame $w.anteprima -relief groove -bd 1
        pack $w.anteprima -fill x -padx 8 -pady {6 0}
        label $w.anteprima.l -text "Anteprima con xstaz, pagina:" -anchor w
        set pagine [dict get $modello pagine]
        set nomi {}
        foreach p $pagine { lappend nomi [dict get $p nome] }
        set default_nome [lindex $nomi 0]
        foreach p $pagine {
            if {[dict get $p numero] == $pagina_disegnata} { set default_nome [dict get $p nome] }
        }
        set ::lgmkstaz::_esito_pagina_scelta $default_nome
        if {[llength $nomi] > 0} {
            tk_optionMenu $w.anteprima.scelta ::lgmkstaz::_esito_pagina_scelta {*}$nomi
        } else {
            label $w.anteprima.scelta -text "(nessuna pagina)"
        }
        button $w.anteprima.vai -text "Anteprima con xstaz" \
            -state [expr {[llength [info procs ::staz_apri]] > 0 ? "normal" : "disabled"}] \
            -command [list ::lgmkstaz::avvia_anteprima $w $scratch $chiave]
        grid $w.anteprima.l -row 0 -column 0 -sticky w -padx 4 -pady 4
        grid $w.anteprima.scelta -row 0 -column 1 -sticky w -pady 4
        grid $w.anteprima.vai -row 0 -column 2 -sticky w -padx 4 -pady 4
        label $w.anteprima.stato -anchor w -justify left -wraplength 640 -text ""
        grid $w.anteprima.stato -row 1 -column 0 -columnspan 3 -sticky w -padx 4 -pady {0 4}
        if {[llength [info procs ::staz_apri]] == 0} {
            $w.anteprima.stato configure -text \
                "lgstaz.tcl non trovato: l'anteprima non e' disponibile in questa\
                 installazione (il resto di lgmkstaz funziona lo stesso)." -fg "#c21807"
        }
    }

    button $w.chiudi -text Chiudi -width 10 -default active \
        -command [list ::lgmkstaz::chiudi_esito_verifica $w $scratch $chiave $ok]
    pack $w.chiudi -pady 8

    bind $w <Return> [list ::lgmkstaz::chiudi_esito_verifica $w $scratch $chiave $ok]
    bind $w <Escape> [list ::lgmkstaz::chiudi_esito_verifica $w $scratch $chiave $ok]
    wm protocol $w WM_DELETE_WINDOW [list ::lgmkstaz::chiudi_esito_verifica $w $scratch $chiave $ok]
}

#  "Anteprima con xstaz": apre per davvero la pagina scelta, con la stessa
#  chiave isolata della compilazione appena fatta (verifica_apri_anteprima,
#  lgmkstaz_verifica.tcl). Da questo momento la copia scratch NON si cancella
#  piu' chiudendo la finestra finche' xstaz non e' chiuso a sua volta - vedi
#  chiudi_esito_verifica.
proc ::lgmkstaz::avvia_anteprima {w scratch chiave} {
    set nome_pagina $::lgmkstaz::_esito_pagina_scelta
    if {$nome_pagina eq ""} { return }

    lassign [verifica_apri_anteprima $scratch $nome_pagina $chiave] esito messaggio
    switch -- $esito {
        ok {
            set ::lgmkstaz::_anteprima_lanciata($w) 1
            set testo "xstaz aperto sulla pagina '$nome_pagina'."
            if {$messaggio ne ""} { append testo "  $messaggio" }
            append testo "\nChiudi la finestra di xstaz quando hai finito, POI premi\
                          Chiudi qui sotto: solo cosi' si pulisce la copia scratch."
            $w.anteprima.stato configure -text $testo -fg "#1a7a1a"
        }
        altrove {
            $w.anteprima.stato configure -text $messaggio -fg "#c21807"
        }
        default {
            $w.anteprima.stato configure -text "Impossibile aprire l'anteprima:\n$messaggio" \
                -fg "#c21807"
        }
    }
}

proc ::lgmkstaz::chiudi_esito_verifica {w scratch chiave ok} {
    if {[info exists ::lgmkstaz::_anteprima_lanciata($w)]} {
        #  e' stata aperta un'anteprima da questa finestra: se xstaz e'
        #  ancora su quella copia scratch, non si tocca ne' la directory
        #  (r02.dat, che sta ancora leggendo) ne' l'IPC che sta ancora
        #  usando - si chiede di chiudere xstaz prima.
        if {[verifica_anteprima_attiva $scratch]} {
            tk_messageBox -icon warning -title lgmkstaz -parent $w -message \
                "xstaz e' ancora aperto su questa anteprima: chiudilo prima,\
                 poi premi di nuovo Chiudi."
            return
        }
        verifica_pulisci_chiave $chiave
        catch { file delete -force $scratch }
        unset ::lgmkstaz::_anteprima_lanciata($w)
    } elseif {$ok} {
        #  nessuna anteprima aperta: su un esito positivo la copia scratch
        #  ha gia' fatto il suo lavoro (verifica_esegui_compstaz ha gia'
        #  tolto la sua SHM); su un fallimento resta, l'ha appena detto la
        #  finestra - la si tocca solo qui, non altrove, cosi' e' sempre
        #  chiaro chi la cancella e quando.
        catch { file delete -force $scratch }
    }
    destroy $w
}

# ---------------------------------------------------------------------------
# Finestra principale.
# ---------------------------------------------------------------------------

wm title . lgmkstaz

menu .mb -tearoff 0
. configure -menu .mb
menu .mb.file -tearoff 0
.mb add cascade -label File -menu .mb.file
.mb.file add command -label "Apri..." -command ::lgmkstaz::apri_dialogo
.mb.file add command -label "Aggiorna" -command ::lgmkstaz::ricarica
.mb.file add separator
.mb.file add command -label "Nuova pagina..." -command ::lgmkstaz::nuova_pagina_dialogo
.mb.file add separator
.mb.file add command -label Salva -command ::lgmkstaz::salva
.mb.file add command -label "Salva con nome..." -command ::lgmkstaz::salva_come
.mb.file add separator
.mb.file add command -label Esci -command ::lgmkstaz::esci

menu .mb.visualizza -tearoff 0
.mb add cascade -label Visualizza -menu .mb.visualizza
.mb.visualizza add checkbutton -label "Immagini reali di xstaz" \
    -variable ::lgmkstaz::usa_sprite -command ::lgmkstaz::ridisegna

menu .mb.verifica -tearoff 0
.mb add cascade -label Verifica -menu .mb.verifica
.mb.verifica add command -label "Compila e verifica..." -command ::lgmkstaz::compila_e_verifica

#  Il font in grassetto della voce principale del menu "?", come in lghmi.
if {[lsearch -exact [font names] fontMenuRilievo] < 0} {
    font create fontMenuRilievo {*}[font actual TkMenuFont]
    font configure fontMenuRilievo -weight bold
}
menu .mb.aiuto -tearoff 0
.mb add cascade -label "?" -menu .mb.aiuto
::lgmkstaz::costruisci_menu_aiuto .mb.aiuto

frame .corpo
pack .corpo -fill both -expand 1

frame .corpo.sinistra -width 190
pack .corpo.sinistra -side left -fill y
pack propagate .corpo.sinistra 0

label .corpo.sinistra.etic1 -text Pagine -anchor w
pack .corpo.sinistra.etic1 -fill x -padx 4 -pady {4 0}
frame .corpo.sinistra.pagine
pack .corpo.sinistra.pagine -fill both -expand 1 -padx 4 -pady {0 4}
listbox .corpo.sinistra.pagine.lista -exportselection 0 -height 8
pack .corpo.sinistra.pagine.lista -fill both -expand 1
bind .corpo.sinistra.pagine.lista <<ListboxSelect>> ::lgmkstaz::cambia_pagina_da_lista

label .corpo.sinistra.etic2 -text "Libreria (clic per piazzare)" -anchor w
pack .corpo.sinistra.etic2 -fill x -padx 4 -pady {4 0}
frame .corpo.sinistra.libreria
pack .corpo.sinistra.libreria -fill both -expand 1 -padx 4 -pady {0 4}
set ::lgmkstaz::_cerca_libreria ""
entry .corpo.sinistra.libreria.cerca -textvariable ::lgmkstaz::_cerca_libreria
pack .corpo.sinistra.libreria.cerca -fill x -pady {0 2}
bind .corpo.sinistra.libreria.cerca <KeyRelease> ::lgmkstaz::libreria_filtra
listbox .corpo.sinistra.libreria.lista -exportselection 0
pack .corpo.sinistra.libreria.lista -fill both -expand 1
bind .corpo.sinistra.libreria.lista <<ListboxSelect>> ::lgmkstaz::libreria_seleziona
::lgmkstaz::libreria_filtra

frame .corpo.destra
pack .corpo.destra -side left -fill both -expand 1
label .corpo.destra.titolo -anchor w -text "Nessun file aperto - File -> Apri..."

canvas .corpo.destra.canvas -bg white -highlightthickness 0 \
    -xscrollcommand {.corpo.destra.hs set} -yscrollcommand {.corpo.destra.vs set}
scrollbar .corpo.destra.hs -orient horizontal -command {.corpo.destra.canvas xview}
scrollbar .corpo.destra.vs -orient vertical -command {.corpo.destra.canvas yview}
grid .corpo.destra.titolo -row 0 -column 0 -columnspan 2 -sticky ew
grid .corpo.destra.canvas -row 1 -column 0 -sticky nsew
grid .corpo.destra.vs -row 1 -column 1 -sticky ns
grid .corpo.destra.hs -row 2 -column 0 -sticky ew
grid rowconfigure .corpo.destra 1 -weight 1
grid columnconfigure .corpo.destra 0 -weight 1

bind .corpo.destra.canvas <ButtonPress-1>   {::lgmkstaz::canvas_press %x %y}
bind .corpo.destra.canvas <B1-Motion>       {::lgmkstaz::canvas_motion %x %y}
bind .corpo.destra.canvas <ButtonRelease-1> {::lgmkstaz::canvas_release %x %y}
bind .corpo.destra.canvas <Motion>          {::lgmkstaz::canvas_hover %x %y}
bind .corpo.destra.canvas <Double-1>        {::lgmkstaz::canvas_double %x %y}
bind .corpo.destra.canvas <Delete>          ::lgmkstaz::elimina_selezionata
bind .corpo.destra.canvas <BackSpace>       ::lgmkstaz::elimina_selezionata
#  Copia/incolla sul CANVAS, non sulla finestra: sulla finestra scatterebbero
#  anche mentre si scrive in una casella (il pannello proprieta', la ricerca
#  della libreria), dove Ctrl+C e Ctrl+V devono restare copia e incolla del
#  TESTO. Il canvas prende il fuoco al primo clic (canvas_press).
bind .corpo.destra.canvas <Control-c>       ::lgmkstaz::copia_selezionata
bind .corpo.destra.canvas <Control-v>       ::lgmkstaz::incolla
bind . <Escape>                             ::lgmkstaz::annulla_armato
wm protocol . WM_DELETE_WINDOW              ::lgmkstaz::esci

label .status -anchor w -relief sunken -bd 1
pack .status -side bottom -fill x

wm geometry . 1000x620

# ---------------------------------------------------------------------------
# Apertura iniziale: l'argomento sulla riga di comando (file o directory),
# altrimenti r01.dat nella directory corrente se c'e' - come compstaz, che lo
# cerca li' senza che si debba dirglielo.
# ---------------------------------------------------------------------------

set ::lgmkstaz::_dato [lindex $::argv 0]
if {$::lgmkstaz::_dato eq "" && [file exists r01.dat]} {
    set ::lgmkstaz::_dato r01.dat
}
if {$::lgmkstaz::_dato ne ""} {
    if {[file isdirectory $::lgmkstaz::_dato]} {
        set ::lgmkstaz::_dato [file join $::lgmkstaz::_dato r01.dat]
    }
    ::lgmkstaz::apri_file $::lgmkstaz::_dato
}
