set ::LINUXPLAT 0
set ::indicatore_after 0
set ::pipeon 0
#set ::anima_sim_path "D:\legopc\user_Paris_Saclay\models\ParSac10"
set ::anima_sim_path 0

set ::line_anim2 INIZ

proc anima { vai variab } {


switch $vai {
	   init	
        	{
	
puts stdout "anima: START Animazione"
        		set ::pipeon 0 		

				if { $::LINUXPLAT == 0 } {set viewer "sincview.exe -pipe"
				} else {set viewer "viewval -s"}			
# set viewer "viewval -s"
        		set olddir [pwd]
				
				if { $::anima_sim_path != 0 } { catch {cd $::anima_sim_path} } 

puts stdout "anima- path=$::anima_sim_path"

				if {[catch {open "| $viewer" r+} ::pipeanim]} {  
					tk_messageBox -message "Error opening pipe: $::pipeanim"  
					set ::pipeanim 0
					set ::pipeon 0
				} else {
				set ::pipeon 1
# Configure reader for ::pipeanim  #### D:/svn_wa/sincview.exe -pipe
       			fconfigure $::pipeanim -buffering line
#puts stdout "aspetto: $::pipeanim"
#scarto la prima lettura: é un messaggio di avviso che è partita la modalita PIPE
				catch {gets $::pipeanim ::line_anim}

# se la pipe è finita (eof) allora l'apertura non ha avuto successo per mancanza della shared memory
				if [eof $::pipeanim] { set ::pipeon 0 } 
#puts stdout $::line_anim
#tk_messageBox -message "else in init: $::line_anim"
				}
				cd $olddir
				return 
			}
		leggi	
			{
#puts stdout "anima: -pipeon=$::pipeon- leggo $variab pipeanim=$::pipeanim"
    	        if { $::pipeon == 0 } { return } 
#set ::line_anim2 "...."				
				puts $::pipeanim $variab
#				catch {flush $::pipeanim }
### after 10000
### tk_messageBox -message "DEBUG: DOPO AFTER"
				gets $::pipeanim ::line_anim
puts $::outfile "DEBUG: DOPO GET  - la variabile $variab: $::line_anim "

# tk_messageBox -message "DEBUG: DOPO GET  - la variabile $variab: $::line_anim "
				  # gets $::pipeanim ::line_anim2
				
#puts stdout "anima: 2-pipeon=$::pipeon- leggo2 $variab pipeanim=$::pipeanim letto=$::line_anim"

##while {[string match "*#!" $::line_anim]} {
###tk_messageBox -message "DEBUG: la variabile $variab: $::line_anim"
##catch {gets $::pipeanim ::line_anim}
##catch {flush $::pipeanim }
##}
#tk_messageBox -message "DEBUG: la variabile $variab: $::line_anim  line_anim2=$::line_anim2"
				return }
        chiudi	
			{
#if { $pipeon == 0 } { return }  
#fermo sincview...    
# senza anima_aggiorna non serve...
#                catch {after cancel anima_aggiorna}
#                catch {after cancel $::indicatore_after}
    		set variab "%STOP%"
    		catch { puts $::pipeanim $variab }
			catch { flush $::pipeanim }
    		set ::pipeon 0
    		set ::done 1  
#catch {tk_messageBox -message "pipe fermata: ind=$::indicatore_after"}
#                catch {after cancel ::indicatore_after}
			puts stdout "anima: STOP Animazione"
close $::outfile
			return }
	
		}
}


proc chk_exit {} {
    set ans [tk_messageBox -type yesno -icon question -message "Sincview si ferma. Confermi?"]
    if { $ans != "no" } {
		 anima chiudi qq	
         exit
       }
  };# chk_exit


#---main
wm protocol . WM_DELETE_WINDOW chk_exit

set variab_inv "JPSSDHLP\n"
set variab_inv "UA_1DHLP\n"
set variab_inv "WVALFDWI\n"
set ::outfile [open [file join $::env(TEMP) sincview_report.out] w]
set ::textnew "Enter a NewValue --->"

anima init qq
#tk_messageBox -message "dopo init"

	# toplevel .sn
	
	wm title . "Sincview"
	frame .fr
	frame .frValore
	frame .etc
	frame .pushValore
	
	# scroll widget
	frame .tl
	text .tl.t -yscrollcommand {.tl.s set}
    scrollbar .tl.s -command {.tl.t yview}
	grid .tl.t .tl.s -sticky nsew

	grid columnconfigure .tl 0 -weight 1
    grid rowconfigure .tl 0 -weight 1

	
    label  .fr.lab -text         "<--- Enter a Tag" -font "Courier 12"
	entry  .fr.entVal -textvariable variab_inv -width 22	
	
#    label  .pushValore.lab -text "Enter a NewValue --->" -font "Courier 12"
    label  .pushValore.lab -textvariable ::textnew  -font "Courier 12"
	entry  .pushValore.entVal -textvariable variab_push -width 22	

	set valore  -
	label .frValore.valore -textvariable valore -font "Courier 12" -borderwidth 5 -relief sunken -width 60
	
	button .fr.show -text Show -command { 	 
		anima leggi $variab_inv
		# set valore $::line_anim
		set ::textnew "Enter a NewValue for $variab_inv --->"	
		set valore [string range $::line_anim 0 50]
		
		## Split into records on newlines
		set records [split $::line_anim "!"]
		## Iterate over the records
		foreach rec $records {
	#tk_messageBox -message "DEBUG: ---$rec"
			.tl.t insert end $rec\n
		}
	
	
	}

	button .pushValore.push -text Push -command { 	
#    set comando [concat $variab_inv $variab_push]
#	tk_messageBox -message  "il comando è -->$comando"
	anima leggi [concat $variab_inv $variab_push]
	# set valore $::line_anim
	
	set valore [string range $::line_anim 0 50]
	.tl.t insert end $valore\n
	
	}

	button .etc.exit -text Exit -command { 
	chk_exit
	}
	
	
	
	grid .fr -column 0 -row 0
	grid .fr.lab -column 2 -row 0
	grid .fr.entVal -column 1 -row 0
	grid .fr.show -column 0 -row 0
	
	grid .frValore -column 0 -row 4
	grid .frValore.valore -column 0 -row 0

	grid .pushValore -column 0 -row 2
	grid .pushValore.lab -column 0 -row 0
	grid .pushValore.entVal -column 1 -row 0
	grid .pushValore.push -column 2 -row 0
	
	grid .etc -column 0 -row 3
	grid .etc.exit -column 0 -row 0
	

	grid .tl -column 0 -row 1
	
	# .tl.t insert end "A long line of text for testing scrollbar."

	
# anima leggi $variab_inv
# anima chiudi qq
		

