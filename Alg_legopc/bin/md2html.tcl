# md2html.tcl - convertitore Markdown -> HTML in Tcl puro.
#
# PERCHE' ESISTE
#   I browser non rendono il Markdown (il sistema classifica i .md come
#   text/plain e ne mostra il sorgente), e la documentazione di LegoPST e' in
#   .md. Appoggiarsi a un convertitore esterno - pandoc, python3-markdown -
#   vorrebbe dire che la documentazione si vede bene solo dove quel pacchetto e'
#   installato, e su un'installazione pulita si torna al testo grezzo. Tcl
#   invece c'e' per definizione: tutta l'interfaccia di LegoPST e' Tcl/Tk.
#   Cosi' la resa e' la stessa su ogni installazione, senza aggiungere requisiti.
#
# PERIMETRO
#   Il sottoinsieme che la documentazione di LegoPST usa davvero, misurato su
#   34 documenti e 10.073 righe: 635 titoli, 260 blocchi di codice recintati,
#   1004 righe di tabella, 132 citazioni, 591 elenchi puntati, 114 numerati,
#   52 righe orizzontali, 236 link, 16 immagini, 5302 codici inline, 1140
#   grassetti, 176 corsivi. Niente note a pie' di pagina, niente tabelle
#   annidate. Non e' un parser Markdown generico e non prova a esserlo.
#
#   Di HTML si riconoscono SOLO <details> e <summary>, che nel README fanno una
#   sezione richiudibile. Tutto il resto degli angolari resta escapato, ed e' la
#   scelta giusta: nella nostra documentazione <nome>, <task>, <modello>, <dir>
#   e simili sono segnaposto in prosa, a centinaia, e passandoli come HTML il
#   browser li cancellerebbe dalla pagina.
#
# USO
#   source md2html.tcl
#   set html [md2html::documento $percorso_md]      ;# pagina completa
#   set corpo [md2html::converti $testo]            ;# solo il body

package require Tcl 8.5

namespace eval md2html {
    variable codici          ;# segnaposto -> codice inline, per ogni conversione
    variable ncodici 0
}

#  & < > diventano entita'. Va fatto PRIMA di inserire tag, altrimenti si
#  escaperebbero anche quelli.
proc md2html::esc {s} {
    string map {& &amp; < &lt; > &gt;} $s
}

#  Identificatore per i titoli, nello stile di GitHub: minuscolo, i caratteri
#  non alfanumerici diventano trattini. Serve ai rimandi interni (#sezione),
#  che la nostra documentazione usa.
proc md2html::ancora {testo} {
    set t [string tolower $testo]
    regsub -all {`|\*|\(|\)|\[|\]|,|\.|:|;|\?|!|'|"|/|\\} $t "" t
    regsub -all {[^a-z0-9àèéìòù-]+} $t "-" t
    return [string trim $t "-"]
}

#  Formattazione dentro una riga: codice, immagini, link, grassetto, corsivo.
#
#  I codici inline vanno estratti PER PRIMI e messi da parte: dentro i backtick
#  gli asterischi e le parentesi quadre non sono formattazione, e senza questa
#  precauzione `**` dentro un codice diventerebbe grassetto.
proc md2html::inline {s} {
    variable codici
    variable ncodici

    set fuori ""
    while {[regexp -indices {`([^`]+)`} $s -> pezzo]} {
        set tutto [lindex [regexp -inline -indices {`([^`]+)`} $s] 0]
        set seg [string range $s 0 [expr {[lindex $tutto 0]-1}]]
        set cod [string range $s [lindex $pezzo 0] [lindex $pezzo 1]]
        set key "\x01[incr ncodici]\x02"
        set codici($key) $cod
        append fuori $seg $key
        set s [string range $s [expr {[lindex $tutto 1]+1}] end]
    }
    append fuori $s
    set s [esc $fuori]

    # immagini prima dei link: la sintassi differisce solo per il ! iniziale
    regsub -all {!\[([^\]]*)\]\(([^)]+)\)} $s {<img src="\2" alt="\1">} s
    regsub -all {\[([^\]]+)\]\(([^)]+)\)} $s {<a href="\2">\1</a>} s
    # grassetto prima del corsivo, altrimenti ** verrebbe letto come due *
    regsub -all {\*\*([^*]+)\*\*} $s {<strong>\1</strong>} s
    regsub -all {(^|[^*])\*([^*]+)\*($|[^*])} $s {\1<em>\2</em>\3} s

    foreach {key cod} [array get codici] {
        set s [string map [list $key "<code>[esc $cod]</code>"] $s]
    }
    return $s
}

#  Una tabella: righe che cominciano per "|". La seconda e' il separatore
#  |---|---| e distingue l'intestazione dal corpo.
proc md2html::tabella {righe} {
    set out "<table>\n"
    set sep [expr {[llength $righe] > 1 && \
                   [regexp {^\s*\|[\s:|-]+\|?\s*$} [lindex $righe 1]]}]
    set i 0
    foreach r $righe {
        if {$sep && $i == 1} { incr i; continue }
        set celle {}
        set r [string trim $r]
        set r [string trim $r "|"]
        foreach c [split $r "|"] { lappend celle [inline [string trim $c]] }
        set tag [expr {($sep && $i == 0) ? "th" : "td"}]
        if {$sep && $i == 0} { append out "<thead>\n" }
        if {$sep && $i == 2} { append out "<tbody>\n" }
        append out "<tr>"
        foreach c $celle { append out "<$tag>$c</$tag>" }
        append out "</tr>\n"
        if {$sep && $i == 0} { append out "</thead>\n" }
        incr i
    }
    if {$sep && $i > 2} { append out "</tbody>\n" }
    append out "</table>\n"
    return $out
}

#  Il convertitore: una macchina a stati sulle righe. Gli stati sono i blocchi
#  che si estendono su piu' righe - recinto di codice, tabella, citazione,
#  elenco, paragrafo - e ognuno si chiude quando comincia qualcos'altro.
proc md2html::converti {testo} {
    variable codici
    variable ncodici
    array unset codici
    set ncodici 0

    set out ""
    set para {}          ;# righe del paragrafo in corso
    set tab  {}          ;# righe della tabella in corso
    set cita {}          ;# righe della citazione in corso
    set liste {}         ;# pila degli elenchi aperti: ul/ol con la loro rientranza
    set recinto 0

    foreach riga [split $testo "\n"] {

        # --- recinto di codice: dentro, tutto e' verbatim ---
        if {[regexp {^\s*```} $riga]} {
            if {$recinto} {
                append out "</code></pre>\n"
                set recinto 0
            } else {
                # prima di aprire, chiudi il resto
                if {[llength $para]} { append out "<p>[inline [join $para " "]]</p>\n"; set para {} }
                if {[llength $tab]}  { append out [tabella $tab]; set tab {} }
                if {[llength $cita]} { append out "<blockquote>[inline [join $cita " "]]</blockquote>\n"; set cita {} }
                while {[llength $liste]} {
                    append out "</[lindex [lindex $liste end] 0]>\n"
                    set liste [lrange $liste 0 end-1]
                }
                append out "<pre><code>"
                set recinto 1
            }
            continue
        }
        if {$recinto} { append out "[esc $riga]\n"; continue }

        # --- tabella ---
        if {[string match "|*" [string trimleft $riga]]} {
            if {[llength $para]} { append out "<p>[inline [join $para " "]]</p>\n"; set para {} }
            lappend tab $riga
            continue
        }
        if {[llength $tab]} { append out [tabella $tab]; set tab {} }

        # --- citazione ---
        if {[regexp {^>\s?(.*)$} $riga -> resto]} {
            if {[llength $para]} { append out "<p>[inline [join $para " "]]</p>\n"; set para {} }
            lappend cita $resto
            continue
        }
        if {[llength $cita]} {
            append out "<blockquote>[inline [join $cita " "]]</blockquote>\n"
            set cita {}
        }

        # --- titolo ---
        if {[regexp {^(#{1,6})\s+(.*)$} $riga -> cancelletti testo_t]} {
            if {[llength $para]} { append out "<p>[inline [join $para " "]]</p>\n"; set para {} }
            while {[llength $liste]} {
                append out "</[lindex [lindex $liste end] 0]>\n"
                set liste [lrange $liste 0 end-1]
            }
            set n [string length $cancelletti]
            set testo_t [string trimright $testo_t " #"]
            append out "<h$n id=\"[ancora $testo_t]\">[inline $testo_t]</h$n>\n"
            continue
        }

        # --- <details>/<summary> ---
        #
        # Sono le uniche righe di HTML VERO nella documentazione di LegoPST: il
        # README ne fa una sezione richiudibile (l'installazione Docker). Tutti
        # gli altri <...> - <nome>, <task>, <modello>, <dir>, a centinaia -
        # sono segnaposto in prosa e devono restare escapati: passandoli come
        # HTML il browser li tratterebbe da tag sconosciuti e li cancellerebbe
        # dalla pagina, cambiando il senso di quello che c'e' scritto.
        if {[regexp {^\s*<summary>(.*)</summary>\s*$} $riga -> sommario]} {
            if {[llength $para]} { append out "<p>[inline [join $para " "]]</p>\n"; set para {} }
            append out "<summary>[inline $sommario]</summary>\n"
            continue
        }
        if {[regexp {^\s*(</?(?:details|summary)>)\s*$} $riga -> tag]} {
            if {[llength $para]} { append out "<p>[inline [join $para " "]]</p>\n"; set para {} }
            append out "$tag\n"
            continue
        }

        # --- riga orizzontale ---
        if {[regexp {^\s*([-*_])\1{2,}\s*$} $riga]} {
            if {[llength $para]} { append out "<p>[inline [join $para " "]]</p>\n"; set para {} }
            append out "<hr>\n"
            continue
        }

        # --- elenchi, con un minimo di annidamento per rientranza ---
        if {[regexp {^(\s*)([-*]|[0-9]+\.)\s+(.*)$} $riga -> spazi segno voce]} {
            if {[llength $para]} { append out "<p>[inline [join $para " "]]</p>\n"; set para {} }
            set liv [string length $spazi]
            set tipo [expr {[string match {[0-9]*} $segno] ? "ol" : "ul"}]
            # chiudi gli elenchi piu' rientrati di questo
            while {[llength $liste] && [lindex [lindex $liste end] 1] > $liv} {
                append out "</[lindex [lindex $liste end] 0]>\n"
                set liste [lrange $liste 0 end-1]
            }
            if {![llength $liste] || [lindex [lindex $liste end] 1] < $liv} {
                append out "<$tipo>\n"
                lappend liste [list $tipo $liv]
            }
            append out "<li>[inline $voce]</li>\n"
            continue
        }

        # --- riga vuota: chiude paragrafo ed elenchi ---
        if {[string trim $riga] eq ""} {
            if {[llength $para]} { append out "<p>[inline [join $para " "]]</p>\n"; set para {} }
            while {[llength $liste]} {
                append out "</[lindex [lindex $liste end] 0]>\n"
                set liste [lrange $liste 0 end-1]
            }
            continue
        }

        # --- riga di testo: paragrafo, oppure continuazione di una voce di
        #     elenco (rientrata sotto di essa) ---
        if {[llength $liste] && [regexp {^\s+\S} $riga]} {
            append out "[inline [string trim $riga]]\n"
            continue
        }
        lappend para [string trim $riga]
    }

    # chiusura di quello che resta aperto
    if {$recinto}        { append out "</code></pre>\n" }
    if {[llength $tab]}  { append out [tabella $tab] }
    if {[llength $cita]} { append out "<blockquote>[inline [join $cita " "]]</blockquote>\n" }
    if {[llength $para]} { append out "<p>[inline [join $para " "]]</p>\n" }
    while {[llength $liste]} {
        append out "</[lindex [lindex $liste end] 0]>\n"
        set liste [lrange $liste 0 end-1]
    }
    return $out
}

#  Foglio di stile: incorporato, perche' la pagina finisce in una directory
#  temporanea e non puo' dipendere da niente di esterno.
proc md2html::stile {} {
    return {
        body   { font-family: sans-serif; line-height: 1.5; max-width: 62em;
                 margin: 2em auto; padding: 0 1.2em; color: #222; }
        h1,h2,h3,h4 { color: #003070; margin-top: 1.6em; line-height: 1.25; }
        h1     { border-bottom: 2px solid #003070; padding-bottom: .3em; }
        h2     { border-bottom: 1px solid #ccd; padding-bottom: .2em; }
        table  { border-collapse: collapse; margin: 1em 0; }
        th, td { border: 1px solid #bbb; padding: .35em .7em; text-align: left;
                 vertical-align: top; }
        th     { background: #eef; }
        tr:nth-child(even) td { background: #fafafa; }
        pre    { background: #f4f4f4; border: 1px solid #ddd; border-radius: 3px;
                 padding: .7em; overflow-x: auto; line-height: 1.35; }
        code   { background: #f4f4f4; padding: 0 .25em; border-radius: 2px;
                 font-size: .95em; }
        pre code { background: none; padding: 0; }
        blockquote { border-left: 4px solid #a0b0d0; margin-left: 0;
                     padding: .3em 0 .3em 1em; color: #444; background: #f8f9fc; }
        a      { color: #0050a0; }
        hr     { border: 0; border-top: 1px solid #ccd; margin: 2em 0; }
        img    { max-width: 100%; }
    }
}

#  Pagina completa a partire da un file .md.
#
#  Il <base href> punta alla directory del documento ORIGINALE: senza quello i
#  rimandi relativi agli altri documenti - che la nostra documentazione usa
#  molto - si risolverebbero rispetto alla directory della pagina generata.
proc md2html::documento {percorso} {
    set fd [open $percorso r]
    set testo [read $fd]
    close $fd

    set titolo [file tail $percorso]
    set out "<!DOCTYPE html>\n<html><head><meta charset=\"utf-8\">\n"
    append out "<title>$titolo</title>\n"
    append out "<base href=\"file://[file dirname [file normalize $percorso]]/\">\n"
    append out "<style>[stile]</style></head><body>\n"
    append out [converti $testo]
    append out "</body></html>\n"
    return $out
}
