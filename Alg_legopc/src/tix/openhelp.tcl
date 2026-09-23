#####################################################
#
# apertura file di help tramite browser HTML
#
#####################################################

# Primo browser realmente eseguibile fra quello configurato e i fallback.
# Serve perche' LG_BROWSER ha avuto a lungo come default /usr/bin/mozilla,
# che su Linux moderno non esiste piu': senza fallback il menu ?->Help si
# fermava sul messaggio "HTML browser not found".
proc browser_disponibile { preferito } {

	set candidati [list $preferito firefox falkon chromium \
	                    chromium-browser google-chrome epiphany xdg-open]

	foreach b $candidati {
		if { $b ne "" && [auto_execok $b] ne "" } {
			return $b
		}
	}
	return ""
}

proc open_hlp { helpfile  } {

	global env

	set preferito ""
	if { [info exists env(LG_BROWSER)] } {
		set preferito $env(LG_BROWSER)
	}

	if { ![info exists env(LG_HTML)] } {
		tk_messageBox -icon error -type ok -title "Help" \
			-message "LG_HTML not set: cannot locate the documentation"
		return
	}

	set hf [file join $env(LG_HTML) $helpfile.htm]

	set browser [browser_disponibile $preferito]
	if { $browser eq "" } {
		tk_messageBox -icon error -type ok -title "Help" \
			-message "HTML browser not found\nVerify your LG_BROWSER enviroment variable\nLG_BROWSER=$preferito"
		return
	}

	if { ![file exists $hf]} {
		tk_messageBox -icon error -type ok -title "Help" \
			-message "Help file $hf  not found"
		return
	}

	#  exec puo' fallire per motivi che il check su auto_execok non vede
	#  (browser installato ma non avviabile): meglio dirlo che restare muti.
	if { [catch { exec $browser $hf & } err] } {
		tk_messageBox -icon error -type ok -title "Help" \
			-message "Cannot start the browser:\n$browser $hf\n\n$err"
	}
}


# --- Documentazione: aprire un .md o un .html nel browser -------------------
#
# Usate sia da lghmi sia da lgmkstaz (menu "?"), per questo stanno qui e non
# dentro uno dei due. aiuto_apri_documento NON scrive nella riga di stato -
# ritorna il messaggio da mostrare, perche' i due programmi la gestiscono in
# modo diverso; "" vuol dire che qualcosa e' andato storto e il dialogo di
# errore e' gia' stato mostrato.

#  Converte un .md in HTML dentro una directory temporanea e ne ritorna il
#  path, oppure "" se non c'e' un convertitore.
#
#  Serve perche' i browser NON sanno rendere il Markdown: il sistema classifica
#  i .md come text/plain, quindi Firefox mostra il sorgente. Con un
#  convertitore, tabelle, blocchi di codice e citazioni si vedono per quello che
#  sono.
#
#  Il convertitore e' quello di LegoPST (md2html.tcl, Tcl puro): niente da
#  installare, e la resa e' la stessa su ogni installazione. Se quel file
#  mancasse si prova un convertitore esterno, e solo se non c'e' nemmeno quello
#  si apre il .md grezzo.
#
#  Il <base href> punta alla directory del documento ORIGINALE: senza quello i
#  rimandi relativi agli altri documenti - che i nostri .md usano molto - si
#  romperebbero, risolvendosi dentro la directory temporanea.
proc aiuto_md_in_html {doc} {
    global env

    set tmp [expr {[info exists env(TMPDIR)] && $env(TMPDIR) ne "" ? $env(TMPDIR) : "/tmp"}]
    if {![file isdirectory $tmp] && [catch {file mkdir $tmp}]} { set tmp "/tmp" }
    set out [file join $tmp "lgdoc_[file rootname [file tail $doc]].html"]

    #  1. Il convertitore di LegoPST (md2html.tcl): in Tcl puro, quindi
    #     disponibile su ogni installazione senza installare niente. E' il
    #     primario, non il ripiego, per due ragioni: la resa e' identica
    #     dappertutto, e sui nostri documenti e' piu' fedele di markdown_py, che
    #     sbaglia i recinti di codice rientrati dentro una voce di elenco (li
    #     trasforma in un <code> malformato).
    if {[llength [info procs ::md2html::documento]] > 0} {
        if {![catch {md2html::documento $doc} pagina] && $pagina ne ""} {
            if {![catch {open $out w} fd]} {
                #  documento restituisce testo gia' decodificato da UTF-8
                #  (vedi md2html.tcl, CODIFICA): con la codifica di sistema,
                #  sotto LANG=POSIX, frecce e lineette diventerebbero "?"
                fconfigure $fd -encoding utf-8
                puts $fd $pagina
                close $fd
                return $out
            }
        }
    }

    #  2. Ripiego, se md2html.tcl non c'e' (deploy parziale, una bin vecchia):
    #     un convertitore esterno, se c'e'.
    set conv ""
    foreach c {markdown_py pandoc cmark-gfm cmark} {
        if {[auto_execok $c] ne ""} { set conv $c; break }
    }
    if {$conv eq ""} { return "" }
    set corpo ""
    switch -- $conv {
        markdown_py { set rc [catch {exec markdown_py -x tables -x fenced_code -x toc $doc} corpo] }
        pandoc      { set rc [catch {exec pandoc -f gfm -t html $doc} corpo] }
        default     { set rc [catch {exec $conv $doc} corpo] }
    }
    if {$rc || $corpo eq ""} { return "" }
    if {[catch {open $out w} fd]} { return "" }
    puts $fd "<!DOCTYPE html>\n<html><head><meta charset=\"utf-8\">"
    puts $fd "<title>[file tail $doc]</title>"
    puts $fd "<base href=\"file://[file dirname $doc]/\">"
    if {[llength [info procs ::md2html::stile]] > 0} {
        puts $fd "<style>[md2html::stile]</style>"
    }
    puts $fd "</head><body>"
    puts $fd $corpo
    puts $fd "</body></html>"
    close $fd
    return $out
}

#  Apre un documento del repository nel browser. Il browser lo sceglie
#  browser_disponibile di openhelp.tcl, la stessa di legopc, che scorre una
#  lista di candidati partendo da $LG_BROWSER.
proc aiuto_apri_documento {relativo} {
    global env
    if {![info exists env(LEGOROOT)] || $env(LEGOROOT) eq ""} {
        tk_messageBox -icon error -title "Documentation" -parent . -message \
            "LEGOROOT not defined: cannot find the documentation."
        return ""
    }
    set doc [file join $env(LEGOROOT) $relativo]
    if {![file exists $doc]} {
        tk_messageBox -icon error -title "Documentation" -parent . -message \
            "Document not found:\n$doc"
        return ""
    }
    set preferito [expr {[info exists env(LG_BROWSER)] ? $env(LG_BROWSER) : ""}]
    set browser ""
    catch {set browser [browser_disponibile $preferito]}
    if {$browser eq ""} {
        tk_messageBox -icon error -title "Documentation" -parent . -message \
            "No browser available.\nCheck LG_BROWSER (currently: '$preferito')."
        return ""
    }
    # I .md passano per il convertitore, se c'e' uno.
    set nota ""
    if {[string tolower [file extension $doc]] eq ".md"} {
        set html [aiuto_md_in_html $doc]
        if {$html ne ""} {
            set doc $html
        } else {
            set nota "  (md2html.tcl not found: unformatted text)"
        }
    }
    if {[catch {exec $browser $doc &} err]} {
        tk_messageBox -icon error -title "Documentation" -parent . -message \
            "Cannot start the browser:\n$browser $doc\n\n$err"
        return ""
    }
    return "Opened in the browser: $relativo$nota"
}
