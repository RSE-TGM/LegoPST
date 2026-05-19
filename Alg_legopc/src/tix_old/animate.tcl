set ::indicatore_after 0
set ::pipeon 0
set ::anima_sim_path "locpath - click to change"
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
#cd /usr/users/testguag/legocad/CAS_HPS6
puts stdout "anima- path=$::anima_sim_path"
#cd /usr/users/cassano/sked/cassano
			if {[catch {open "| $viewer" r+} ::pipeanim]} {  

    			tk_messageBox -message "Error opening pipe: $::pipeanim"  
  			set ::pipeanim 0
  			set ::pipeon 0
  			} else {
  			set ::pipeon 1
   			# Configure reader for ::pipeanim  
       			fconfigure $::pipeanim -buffering line
#puts stdout "aspetto: $::pipeanim"
#scarto la prima lettura...
##			if { $::LINUXPLAT == 0 } {catch {gets $::pipeanim ::line_anim}}
			catch {gets $::pipeanim ::line_anim}

# se la pipe �finita (eof) allora l'apertura non ha avauto successo per mancanza della shared memory
if [eof $::pipeanim] { set ::pipeon 0 } 
#puts stdout $::line_anim
#tk_messageBox -message "else in init: $::line_anim"

#       		.top.text subwidget listbox insert end $::line_anim
			}
			cd $olddir
		return }
	leggi	
	{
#puts stdout "anima: -pipeon=$::pipeon- leggo $variab pipeanim=$::pipeanim"
    	        if { $::pipeon == 0 } { return } 
    		catch {puts $::pipeanim $variab}
		catch {flush $::pipeanim}
    		catch {gets $::pipeanim ::line_anim}
#puts stdout "anima: 2-pipeon=$::pipeon- leggo2 $variab pipeanim=$::pipeanim letto=$::line_anim"
#tk_messageBox -message "la variabile $variab: $::line_anim"
        	return }
        chiudi	
        {
#if { $pipeon == 0 } { return }  
#fermo sincview...    
                catch {after cancel anima_aggiorna}
                catch {after cancel $::indicatore_after}

    		set variab "%STOP%"
    		catch { puts $::pipeanim $variab }
		catch {flush $::pipeanim }
    		set ::pipeon 0
    		set ::done 1  
#catch {tk_messageBox -message "pipe fermata: ind=$::indicatore_after"}
#                catch {after cancel ::indicatore_after}
puts stdout "anima: STOP Animazione"
		return }
	
	}
}

#
#set variab_inv "TFINRH1_\n"
#anima init qq
#tk_messageBox -message "dopo init"
#anima leggi $variab_inv
#anima chiudi qq
#exit

proc anima_aggiorna { c mod } {
#        global pipeon
	update
	set refr_anim_ms 1000
#	tk_messageBox -message "ciao"
#puts stdout "anima_ggiorn- modo=$mod - indicatore_after=$::indicatore_after"

if { $mod == 1 } {
set modoagg 2
# inizializzazione dei campi delle variabili da visulizzare
        set nelem 0

#         if { $tipo == 1} {
#                # cancello le scritte
#		catch [$c delete infoitemname]	
#	 return
#	 }
#set lista_moduli   [$c find withtag module]     
         foreach item  [$c find withtag module] {
#tk_messageBox  -message "ITEM: $item\n [$c gettags $item]\n [lsearch [$c gettags $item] *.name]\n [lindex [$c gettags $item] [lsearch [$c gettags $item] *.name]]]"
#tk_messageBox  -message "ITEM: $item\n 0: [lindex [$c gettags $item] 0 ]\n 1: [lindex [$c gettags $item] 1 ]\n 2: [lindex [$c gettags $item] 2 ]\n 3: [lindex [$c gettags $item] 3 ]\n 4: [lindex [$c gettags $item] 4 ]\n 5: [lindex [$c gettags $item] 5 ]\n"
set esiste 0
		set lc [$c bbox $item] 
		set x [expr [expr [lindex $lc 0]+[lindex $lc 2]]/2]
#in alto a destra
#		set y [lindex $lc 1]
#in basso a destra
		set y [expr [lindex $lc 3] + 6]
#tk_messageBox -icon warning -message "$lc x=$x y=$y" -type ok
#        	if { $tipo == 2 } { set pisqu [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.name]]] }
#		if { $tipo == 3 } { set pisqu [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.cls]]] }
#GUAG - nov 2007 - continuare qui per variabile di anim nuove,,,
      set tags_curr [$c gettags $item]
#puts "anima_aggiorna:  elemento attuale  ---------> $item ---------> $tags_curr"
      if {[lsearch -exact $tags_curr  remarkdescr] != -1} { 
# �un remark...
#puts "anima_aggiorna:  ho trovato un remark: $tags_curr"
        if {[lsearch -exact $tags_curr  da_animare] != -1} {  
# �un remark da animare...
      set pisqu [file rootname [lindex $tags_curr [lsearch $tags_curr *.anim]]]   
#puts "anima_aggiorna:  ho trovato un remark da animare: $tags_curr e pisqu: $pisqu"      
      set esiste 1
      }	
      } else {
set pisqu_name [file rootname [lindex $tags_curr [lsearch $tags_curr *.name]]]
set pisqu [file rootname [lindex $tags_curr [lsearch $tags_curr *.anim]]]$pisqu_name	 
      set esiste 1
      }

# set pisqu_name [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.name]]]
# set pisqu [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.anim]]]$pisqu_name
#tk_messageBox  -message "anima_aggiorna: $pisqu"
if { $esiste == 1 } {
$c addtag $pisqu.nome_anim withtag $item
$c addtag da_animare withtag $item

	set variab_inv $pisqu\n
        anima leggi $variab_inv
#puts "anima_aggiorna: invio variabile $variab_inv"
        set pisqu $::line_anim
#puts "anima_aggiorna: INIT ottengo  $::line_anim"
if { [regexp -nocase {^ERRORE} $::line_anim] == 1} {
#puts "anima_aggiorna: ERRORE  ignoro "
$c dtag $item da_animare
} else {
#       catch {$c delete $tid}
        
	if { $::stileanim || $::LINUXPLAT == 0 } {
	set testo "[lindex $pisqu 0] [lindex $pisqu 1]"
        } else { 
	set testo "[lindex $pisqu 2]\n[lindex $pisqu 0] [lindex $pisqu 1]"
	}
#puts "-->$testo"
#catch { $c delete $tid }
#catch { $c dtag $item $tid.visual }	
        set tid [$c create text $x $y -text $testo -tags {infoitemname}]
        $c addtag $tid.visual withtag $item        
#tk_messageBox  -message "anima_aggiorna: $item \n $pisqu\n $tid\n $tid.visual"		
		set lc [$c bbox $tid] 
		set x1 [lindex $lc 0]
		set y1 [lindex $lc 1]
		set x2 [lindex $lc 2]
		set y2 [lindex $lc 3]

#catch { $c delete $rect}
		set rect [$c create rectangle $x1 $y1 $x2 $y2 -fill yellow  -outline yellow -tags {infoitemname}]

		$c raise $tid
		incr nelem
         }
	 }    
#puts "-------------------------------------" 
 }    

##############      	 
	update
#	set ii [after info]
#	tk_messageBox -message "after info: $ii"

}

if { $mod == 2 } {
set modoagg 2
#puts "Modalit�2 - aggiornamento" 
#visulizzazione periodica sui campi creati con la modalita 1
       foreach item [$c find withtag module] {
      set tags_curr [$c gettags $item]
#nome della istanza...
##					set pisqu_name [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.name]]]
#nome della variabile da visulizzare ( composta da 4 cratteri+nome della istanza)...
##					set pisqu [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.anim]]]$pisqu_name

if {[lsearch -exact $tags_curr  da_animare] != -1} {  
set pisqu [file rootname [lindex $tags_curr [lsearch $tags_curr *.nome_anim]]]

if { $::LINUXPLAT } {
if { $::stileanim != $::stileanim_prec } { 
#catch { $c delete $pisqu_tid}
#catch { $c delete $rect}
set ::stileanim_prec $::stileanim
#set modoagg 1
}
}

#identificatore del campo su cui si scriver�il valore...
					set pisqu_tid [file rootname [lindex $tags_curr [lsearch $tags_curr *.visual]]]
#puts "anima_aggiorna: Modalit�2 -pipeon=$::pipeon- $item \n  $pisqu\n $pisqu_tid"		

#chiedo al server lego il valore con il nome ( pisqu )
					set variab_inv $pisqu\n
          anima leggi $variab_inv
#carico in pisqu il valore
          set pisqu $::line_anim
#puts "anima_aggiorna: ottengo  $::line_anim"
#aggiorno il campo...
####          set testo "[lindex $pisqu 0] [lindex $pisqu 1]"
	if { $::stileanim || $::LINUXPLAT == 0 } {
	set testo "[lindex $pisqu 0] [lindex $pisqu 1]"
        } else {set testo "[lindex $pisqu 2]\n[lindex $pisqu 0] [lindex $pisqu 1]"}

					$c itemconfigure $pisqu_tid -text $testo
					$c raise $pisqu_tid
	update
					}
					}

}

#puts "rilancio dopo sec=$refr_anim_ms millisecondi la fase di aggiornamento dei valori indicatore_after=$::indicatore_after"
# normalmente modoagg è 2 ... tranne quando, linux , cambia il modo di visualizzazzione allora diventa 1
set ::indicatore_after [after $refr_anim_ms anima_aggiorna $c $modoagg]		
}
   # Wait for completion of output from pipe
#   vwait ::done
#   destroy .top
