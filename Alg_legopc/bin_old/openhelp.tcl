#####################################################
#
# apertura file di help tramite browser HTML
#
#####################################################
proc open_hlp { helpfile  } {
	
	global env

	set browser {$env(LG_BROWSER)}
	set hp $env(LG_HTML)
	
	set hf  [file join $hp $helpfile.htm]

	if { ![file exists $env(LG_BROWSER)] } {
		tk_messageBox -type ok -message "HTML browser not found\nVerify your LG_BROWSER enviroment variable\nLG_BROWSER=$browser"
		return 
	}

	if { ![file exists $hf]} {
		tk_messageBox -type ok -message "Help file $hf  not found"
		#set hf  [file join $hp index.htm]
		#if { ![file exists $hf]} {
		#	return 
		#} 

		return
	} 
	
	eval exec $browser $hf &
}