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
