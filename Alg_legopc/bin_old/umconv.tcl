
###########################################
#
# tabella per la definizione delle UM
# di default in base all'iniziale della variabile
#
###########################################
proc initDefUM { } {
	global arrDefUM
	global taskpath
	global txtsubdir
	global fnvarass

	set fname [file join $txtsubdir $fnvarass]

	set chk [catch {set fp [open $fname r]} result]

puts "file open result =$chk"

	if {$chk != 0} {
		set messaggio "The file $fname doesn't exist. Assuming default"
		tk_messageBox -message $messaggio -type ok
	

		# associa le iniziali della variabile con una unità di defualt
		# la iniziale della variabile costituisce l'indice dell'array

		set arrDefUM(P) "P"
		set arrDefUM(T) "°K"
		set arrDefUM(W) "Kg/s"
		set arrDefUM(H) "J/K"
		set arrDefUM(Q) "W"
		set arrDefUM(A) "p.u."
		set arrDefUM(X) "p.u."
		set arrDefUM(M) "Kg"
		set arrDefUM(V) "V"
		set arrDefUM(U) "V"
		set arrDefUM(I) "A"
		set arrDefUM(C) "A"
		set arrDefUM(O) "Rad/s"
		set arrDefUM(F) "Hz"

		set wfp [open $fname w]
		set listDefUm [array get arrDefUM]
		set nDefUm [llength $listDefUm]

		for { set i 0 } {$i < $nDefUm } { incr i 2 } {
			set v1 [lindex $listDefUm $i]
			set v2 [lindex $listDefUm [expr $i+1]]
			set item "$v1 $v2"
			puts $wfp $item
		}

		flush $wfp
		close $wfp	


	} else {

puts "DBG: step 1 ... "

		# leggo il contenuto del file
		# ed lo utilizzo per definire e inizializzare
		# l'array 

		set pass 0
		while {![eof $fp]} {

			set item ""
			gets $fp item

			if { [string length $item] < 1} {
				continue
			}

			scan $item "%s %s" nome valore
			set arrDefUM($nome) $valore
puts "pass=$pass"
incr pass
		}

puts "DBG: step 1 end ..."

		close $fp	

puts "DBG: step 1 passed... "	

	}	
}

proc initConvRule { } {
	global convUM
	global arrConvUM
	global taskpath
	global txtsubdir
	global fnconvrule

	set convUM [list]


	set fname [file join $txtsubdir $fnconvrule]

	set chk [catch {set fp [open $fname r]} result]

puts "file open result =$chk"

	if {$chk != 0} {
		set messaggio "The file $fname doesn't exist. Assuming default"
		tk_messageBox -message $messaggio -type ok

		lappend convUM "NN to NN"

		lappend convUM "°K to °C"
		lappend convUM "°C to °K"

		lappend convUM "P to bar"
		lappend convUM "bar to P"

		lappend convUM "p.u. to %"
		lappend convUM "% to p.u."

		lappend convUM "Kg/s to t/h"
		lappend convUM "t/h to Kg/s"


		# nessuna conversione
		set arrConvUM(0,a) 1
		set arrConvUM(0,b) 0
		set arrConvUM(0,newUM) ""

		# da °K a °C
		set arrConvUM(1,a) 1
		set arrConvUM(1,b) -273.15
		set arrConvUM(1,newUM) "°C"

		# da °C a °K
		set arrConvUM(2,a) 1
		set arrConvUM(2,b) 273.15
		set arrConvUM(2,newUM) "°K"

		# da Pascal a bar
		set arrConvUM(3,a) 10E-05
		set arrConvUM(3,b) 0
		set arrConvUM(3,newUM) "bar"

		# da bar a Pascal
		set arrConvUM(4,a) 10E+05
		set arrConvUM(4,b) 0
		set arrConvUM(4,newUM) "P"

		# da p.u. a %
		set arrConvUM(5,a) 100
		set arrConvUM(5,b) 0
		set arrConvUM(5,newUM) "%"

		# da % a p.u.
		set arrConvUM(6,a) 0.01
		set arrConvUM(6,b) 0
		set arrConvUM(6,newUM) "p.u."

		# da Kg/s a t/h
		set arrConvUM(7,a) [expr 1/3.6]
		set arrConvUM(7,b) 0
		set arrConvUM(7,newUM) "t/h"

		# da t/h a Kg/s
		set arrConvUM(8,a) 3.6
		set arrConvUM(8,b) 0
		set arrConvUM(8,newUM) "Kg/s"


		set wfp [open $fname w]
		set lsize [llength $convUM]
		
		for { set i 0 } {$i < $lsize } { incr i } {
			set v1 [lindex $convUM $i]
			set v2 $arrConvUM($i,a)
			set v3 $arrConvUM($i,b)
			set v4 $arrConvUM($i,newUM)

			set item "$v1 $v2 $v3 $v4"
			puts $wfp $item
		}

		flush $wfp
		close $wfp	


	} else {

puts "DBG: step 2 ---"

		# leggo il contenuto del file
		# ed lo utilizzo per definire e inizializzare
		#  
set pass 0
		set i 0
		while {![eof $fp]} {
			set item ""
			gets $fp item
			if { [string length $item] < 1} {
				continue
			}
			set ctype1 ""
			set ctype2 ""
			set ctype3 ""
			set cA ""
			set cB ""
			set symbol ""

			scan $item "%s %s %s %s %s %s" ctype1 ctype2 ctype3 cA cB symbol
			puts "read: $ctype1 $ctype2 $ctype3 $cA $cB $symbol"
			set convtype "$ctype1 $ctype2 $ctype3"

			lappend convUM $convtype

			set arrConvUM($i,a) $cA
			set arrConvUM($i,b) $cB
			set arrConvUM($i,newUM) $symbol

			incr i

puts "pass: $pass"
incr pass

		}


		close $fp	

puts "DBG: step 2 passed ..."

	}
} 


###########################################
#
# tabella per la definizione delle conversioni
# di default in base all'iniziale della variabile
#
###########################################
proc initConvVar { } {
	global arrConvVar
	global taskpath
	global txtsubdir
	global fnvarconv


	set fname [file join $txtsubdir $fnvarconv]

	set chk [catch {set fp [open $fname r]} result]

puts "file open result =$chk"

	if {$chk != 0} {

		set messaggio "The file $fname doesn't exist. Assuming default"
		tk_messageBox -message $messaggio -type ok
	

		# associa le iniziali della variabile con una unità di defualt
		# la iniziale della variabile costituisce l'indice dell'array

		set arrConvVar(P) 	0
		set arrConvVar(T)   	0
		set arrConvVar(W) 	0
		set arrConvVar(H) 	0
		set arrConvVar(Q) 	0
		set arrConvVar(A) 	0
		set arrConvVar(X) 	0
		set arrConvVar(M) 	0
		set arrConvVar(V)   	0
		set arrConvVar(U)   	0
		set arrConvVar(I)		0
		set arrConvVar(C) 	0
		set arrConvVar(O) 	0
		set arrConvVar(F) 	0


		set wfp [open $fname w]




		set listarr [array get arrConvVar]
		set lsize [llength $listarr]
puts "DBG: lsize=$lsize"
puts "listarr $listarr"

		for { set i 0 } {$i < $lsize } { incr i 2} {
			set v1 [lindex $listarr $i]
			set v2 [lindex $listarr [expr $i+1]]
	
			set item "$v1 $v2"
			puts $wfp $item

			puts "item=$item"


		}

		flush $wfp
		close $wfp	


	} else {

		# leggo il contenuto del file
		# ed lo utilizzo per definire e inizializzare
		# l'array 
		while {![eof $fp]} {
			set item ""
			gets $fp item
			scan $item "%s %s" nome valore
			set arrConvVar($nome) $valore
		}

		close $fp	
	}	
}



proc convVal { Vin a b } {
	
	# formula di vonversione
	set Vout [expr $a * $Vin + $b]

	puts "convVal vin=$Vin a=$a b=$b"

	return $Vout
}

proc searchConvInd { whichConv } {
	global convUM
	global arrConvUM

	#set nConv [llength $convUM]

puts "searchConvInd whichConv=$whichConv"
puts "convUM=$convUM"

	set idx [lsearch $convUM $whichConv]

	if { $idx < 0 } {
		# conv not found
		puts "Conversion not defined"
	} else {
		# conv found
		puts "Conversion defined. Found at index $idx"
	}
	
	return $idx
}

proc getConvCoeff { idx pa pb} {
	upvar 1 $pa ca 
	upvar 1 $pb cb

	global arrConvUM

	set ca 1
	set cb 0


	set ca $arrConvUM($idx,a)		
	set cb $arrConvUM($idx,b)		


	puts "ca=$ca cb=$cb"

}

proc getNewUM { whichConv } {
	global arrConvUM

#	set idx [searchConvInd $whichConv]

	set idx $whichConv

	set newum $arrConvUM($idx,newUM)

	return $newum
}	

proc getDefUM { ch } {
	global arrDefUM

	if { [info exist arrDefUM($ch) ] } {
		set um $arrDefUM($ch)
	} else {
		set um "?"
	}

	return $um
}

proc converti { val convToApply } {
	global convUM
	global arrConvUM

puts "DBG 1 convToApply=$convToApply"
	#set cnv [searchConvInd $convToApply]

	set cnv $convToApply 

	set a 0
	set b 0

puts "DBG 2"

	if { $cnv >= 0 } {
puts "DBG 3"

		getConvCoeff $cnv a b
puts "DBG 4  coeff a=$a b=$b"

		set vconv [convVal $val $a $b]
puts "DBG 5"

	} else {

		set vconv $val
	}

puts "VALUE IN=$val CON VALUE=$vconv"

	return $vconv
}

###################################################################
########## assiciazione variabili unità di misura ################# 
###################################################################


proc um_default { } {
	global arrDefUM
	global convUM
	global arrConvUM

#	set sizeArrDefUM [array size arrDefUM]
#	set sizeConvUM [llength $convUM]
	# la sizeArrConvUm deve essere pari a sizeConvUm*3 in quanto 
	# l'array è strutturato
	# come record con tre campi coeff a, coeff b, e newUM
#	set sizeArrConvUM [array size arrConvUM]

#	puts "sizeArrDefUM = $sizeArrDefUM "
#	puts "sizeConvUM = $sizeConvUM "
#	puts "sizeArrConvUM = $sizeArrConvUM "

	build_um_win

}


proc build_um_win { } {
	
	global arrDefUM
	global nDefUm

    	set w .wmainconv
	set convMainW $w
     	toplevel $w
	set titolo "Variable and Measure Unit"
     	wm title $w $titolo
     	wm iconname $w $titolo
     	wm minsize $w 45 1

	# menù per aggiungere nuove iniziali di variabili
	# e associargli l'unità di misura di default
    	$w configure -menu $w.menu
	menu $w.menu -tearoff 0
	menu $w.menu.new -tearoff 0
	$w.menu add cascade -label "Add variable" -menu $w.menu.new -underline 0
	$w.menu.new add command -label "Add" -command "add_new_variable"


    	set top [frame $w.f -bd 1 -relief raised]
    	set box [tixButtonBox $w.b -bd 1 -relief raised]


	set sizeArrDefUM [array size arrDefUM]
	set listDefUm [array get arrDefUM]
	set nDefUm [llength $listDefUm]

#	puts "sizeArrDefUM: $sizeArrDefUM "
#	puts "listDefUm: $listDefUm"
#	puts "length=[llength $listDefUm]"

	label $top.tit1 -text "Variable"   
	label $top.tit2 -text "Default U.M."   

	grid config $top.tit1  -column 0 -row 0 -sticky w
	grid config $top.tit2  -column 1 -row 0 

	# definisco una Label+Entry per ogni UM di default
	# recuperando la label ed il valore dalla lista
	# ricavata dalla composizione dell'array
	# gli elementi pari sono i valori
	# quelli dispari le label
	# (La label è una entry non editabile)
	for { set i 0 } {$i < $nDefUm } { incr i 2} {

		set row [expr $i + 1]

		set lname [lindex $listDefUm $i]
		set lval  [lindex $listDefUm [expr $i +1]]

		#label $top.lbl$i  -text $lname: -width 2 
		entry $top.lbl$i  -width 2 
		entry $top.ent$i  -width 15

		$top.lbl$i  delete 0 3
		$top.lbl$i  insert 0 $lname

		
		$top.ent$i  delete 0 20
		$top.ent$i  insert 0 $lval

#		puts "entry-> $top.ent$i"

		$top.lbl$i  configure -state disabled -relief ridge

		grid config $top.lbl$i -column 0 -row $row -sticky w
		grid config $top.ent$i -column 1 -row $row 

	}

	grid columnconfigure $top 0 -pad 2

    	# Create the buttons
    	#
    	$box add ok     	-text "Save & Exit"   	-command "ok_vum $w $top" -width 6
    	$box add salva	-text "Save"     -command "apply_vum $top" -width 6
    	$box add cancel 	-text "Quit" 	-command "close_vum $w" -width 6


    	pack $box -side bottom -fill both
    	pack $top -side top -fill both -expand yes


}

proc ok_vum { win topw } {
	puts "a_ok"
	# save on file and exit
	save_vum $topw
	close_vum $win
}

proc apply_vum { topw } {
	puts "a_apply"
	# save on file without exit
	save_vum $topw
}

proc close_vum { win } {
	destroy $win
}

proc save_vum { topw } {
	global nDefUm
	global taskpath
	global txtsubdir
	global fnvarass

	set fname [file join $txtsubdir $fnvarass]

	set chk [catch {set fp [open $fname w]} result]
	if {$chk == 0} {
		set messaggio "The file $fname already exist. Overwrite"
		tk_messageBox -message $messaggio -type ok
	}


	set vars [list]
	set val [list]

	for {set i 0} {$i < $nDefUm } { incr i 2 } {
		set wlname $topw.lbl$i
		set wlval  $topw.ent$i

#		puts "label wid $wlname"
#		puts "entry wid $wlval"

		set varinit [$wlname get]
		set newval [$wlval get]

		lappend vars $varinit
		lappend val $newval

#		puts "$varinit -> $newval"

	}


	foreach vname $vars vval $val {
#		puts "$vname: $vval"
		puts $fp "$vname $vval"
	}

	flush $fp
	close $fp

	# reinizializzo l'array dopo lo modifiche
	initDefUM
}

proc add_new_variable { } {
	puts "aggiunge una nuova iniziale di variabile"

    	set w .wdefvars
	set newdefvars $w
     	toplevel $w
	set titolo "New default variable"
     	wm title $w $titolo
     	wm iconname $w $titolo
     	wm minsize $w 45 1

    	set top [frame $w.f -bd 1 -relief raised]

    	set box [tixButtonBox $w.b -bd 2 -relief raised]

    	# Create the buttons
    	#
    	$box add ok     -text Ok     -command { CBadd_new_vars $varinit  $vardefum } -width 3 -height 1
    	$box add cancel -text Cancel -command "destroy $w" -width 3 -height 1

	# creo un frame per contenere le entry iniziale e unità di misura
	set fr $w.frame
	frame $w.frame

    	tixLabelEntry $fr.lblVarInit  -label "Variable Init: " \
		-options {
	   	 entry.width 3
	   	 label.width 20
	   	 label.anchor e
	   	 entry.textVariable varinit
		}

   	tixLabelEntry $fr.lblDefUM  -label "Default U.M.: " \
		-options {
	   	 entry.width 10
	   	 label.width 20
	   	 label.anchor e
	   	 entry.textVariable vardefum
		}

  	pack $box -side bottom -fill both -pady 3
    	pack $top -side top -fill both -expand yes

	pack $fr.lblVarInit   -anchor w -padx 10 -pady 10
	pack $fr.lblDefUM    -anchor w -padx 10 -pady 10

	pack $w.frame -side top -fill both -expand yes
}

proc CBadd_new_vars { vinit vdefum } {
	global arrConvVar
	global arrDefUM
	global taskpath
	global txtsubdir
	global fnvarass

	puts "CBadd_new_vars vinit:$vinit vdefum:$vdefum"

	# assegno nell'array la nuova unità di misura
	set arrDefUm($vinit) $vdefum


	# assegno anche la conversione di default
	set arrConvVar($vinit) 	0

	set fname [file join $txtsubdir $fnvarass]

	set chk [catch {set fp [open $fname a+]} result]
	if {$chk != 0} {
		set messaggio "The file $fname not exist. Writing Default"
		tk_messageBox -message $messaggio -type ok
		#initConvRule
	}

	while {![eof $fp]} {
			set appo ""
			gets $fp appo
			puts "$appo"
	}

	puts $fp "$vinit $vdefum"

	flush $fp
	close $fp


	close_vum  .wmainconv

	destroy .wdefvars


	# reinizializzo l'array dopo lo modifiche
	initDefUM

	build_um_win 
}

###################################################################
########## definizione conversione unità di misura ################ 
###################################################################


proc define_conv_rule { } { 
	global arrDefUM
	global convUM
	global arrConvUM

#	set sizeArrDefUM [array size arrDefUM]
#	set sizeConvUM [llength $convUM]
	# la sizeArrConvUm deve essere pari a sizeConvUm*3 in quanto 
	# l'array è strutturato
	# come record con tre campi coeff a, coeff b, e newUM
#	set sizeArrConvUM [array size arrConvUM]

#	puts "sizeArrDefUM = $sizeArrDefUM "
#	puts "sizeConvUM = $sizeConvUM "
#	puts "sizeArrConvUM = $sizeArrConvUM "

	build_crule_win

}

proc build_crule_win { } {
	global convUM
	global arrConvUM
	global nConv

	set nConv [llength $convUM]
puts "NCONV =$nConv"

    	set w .wmaincrule
	set cruleMainW $w
     	toplevel $w
	set titolo "Measure Unit Conversion Rules"
     	wm title $w $titolo
     	wm iconname $w $titolo
     	wm minsize $w 45 1

    	$w configure -menu $w.menu
	menu $w.menu -tearoff 0
	menu $w.menu.new -tearoff 0
	$w.menu add cascade -label "Add conversion" -menu $w.menu.new -underline 0
	$w.menu.new add command -label "Add" -command "add_new_conv "



    	set top [frame $w.f -bd 1 -relief raised]
    	set box [tixButtonBox $w.b -bd 1 -relief raised]

	label $top.tit1 -text "Conversion type"   
	label $top.tit2 -text "Coeff. A"   
	label $top.tit3 -text "Coeff. B"   
	label $top.tit4 -text "Symbol"

	grid config $top.tit1  -column 0 -row 0 -sticky w
	grid config $top.tit2  -column 1 -row 0 
	grid config $top.tit3  -column 2 -row 0 
	grid config $top.tit4  -column 3 -row 0 


	# definisco una Label+Entry per ogni tipo di conversione
	# recuperando la label ed il valore dalla lista


	for { set i 0 } {$i < $nConv } { incr i } {

		set row [expr $i + 1]

		set lconvtype [lindex $convUM $i]
		set coeffA $arrConvUM($i,a)
		set coeffB $arrConvUM($i,b)
		set newUM $arrConvUM($i,newUM)

		#label $top.lbl$i  -text $lconvtype -width 10
		entry $top.lbl$i   -width 10
		entry $top.entA$i  -width 10 
		entry $top.entB$i  -width 10 
		entry $top.entSym$i  -width 10 

		$top.lbl$i  delete 0 end
		$top.lbl$i  insert 0 $lconvtype

		$top.entA$i  delete 0 end
		$top.entA$i  insert 0 $coeffA

		$top.entB$i  delete 0 end
		$top.entB$i  insert 0 $coeffB

		$top.entSym$i  delete 0 end
		$top.entSym$i  insert 0 $newUM

		$top.lbl$i  configure -state disabled -relief ridge

		puts "lconvtype=$lconvtype"
		
		grid config $top.lbl$i -column 0 -row $row -sticky w
		grid config $top.entA$i -column 1 -row $row -sticky w
		grid config $top.entB$i -column 2 -row $row -sticky w
		grid config $top.entSym$i -column 3 -row $row -sticky w

	}

	grid columnconfigure $top 0 -pad 2

    	# Create the buttons
    	#
    	$box add ok     	-text "Save & Exit"   	-command "ok_crule $w $top" -width 6
    	$box add salva	-text "Save"     		-command "apply_crule $top" -width 6
    	$box add cancel 	-text "Quit" 		-command "close_crule $w" -width 6

    	pack $box -side bottom -fill both
    	pack $top -side top -padx 5 -pady 5 -fill both -expand yes

}

proc add_new_conv {  } {
	
    	set w .wdefcrule
	set newcrule $w
     	toplevel $w
	set titolo "Measure Unit Conversion Rules"
     	wm title $w $titolo
     	wm iconname $w $titolo
     	wm minsize $w 45 1

    	set top [frame $w.f -bd 1 -relief raised]

    	set box [tixButtonBox $w.b -bd 2 -relief raised]
   
    	# Create the buttons
    	#
    	$box add ok     -text Ok     -command { CBadd_new_crule $vfrom  $vto $vCA $vCB $vSymb} -width 3 -height 1
    	$box add cancel -text Cancel -command "destroy $w" -width 3 -height 1

	# creo un frame per contenere le entry nome,descrizione)
	set fr $w.frame
	frame $w.frame

    	tixLabelEntry $fr.lblFrom  -label "From: " \
		-options {
	   	 entry.width 20
	   	 label.width 20
	   	 label.anchor e
	   	 entry.textVariable vfrom
		}

    	tixLabelEntry $fr.lblTo  -label "To: " \
		-options {
	   	 entry.width 20
	   	 label.width 20
	   	 label.anchor e
	   	 entry.textVariable vto
		}

    	tixLabelEntry $fr.lblCA  -label "Coeff. A: " \
		-options {
	   	 entry.width 20
	   	 label.width 20
	   	 label.anchor e
	   	 entry.textVariable vCA
		}

    	tixLabelEntry $fr.lblCB  -label "Coeff. B: " \
		-options {
	   	 entry.width 20
	   	 label.width 20
	   	 label.anchor e
	   	 entry.textVariable vCB
		}

    	tixLabelEntry $fr.lblSymb  -label "Symbol: " \
		-options {
	   	 entry.width 20
	   	 label.width 20
	   	 label.anchor e
	   	 entry.textVariable vSymb
		}


   	pack $box -side bottom -fill both -pady 3
    	pack $top -side top -fill both -expand yes

	pack $fr.lblFrom   -anchor w -padx 10 -pady 10
	pack $fr.lblTo   -anchor w -padx 10 -pady 10
	pack $fr.lblCA   -anchor w -padx 10 -pady 10
	pack $fr.lblCB   -anchor w -padx 10 -pady 10
	pack $fr.lblSymb   -anchor w -padx 10 -pady 10


	pack $w.frame -side top -fill both -expand yes

}

proc CBadd_new_crule { vfrom vto vCA vCB vSymb } {
	global convUM
	global arrConvUM
	global nConv
	global taskpath
	global txtsubdir
	global fnconvrule

	puts "add_new_crule called... $vfrom $vto $vCA $vCB $vSymb "
	puts "BEFORE NCONV =$nConv"

	set item "$vfrom to $vto"
	lappend convUM $item
	set i [llength $convUM]
	set nConv [llength $convUM]

	puts "AFTER NCONV =$nConv"

	#set lconvtype [lindex $convUM $i]
	set arrConvUM($i,a) $vCA
	set arrConvUM($i,b) $vCB
	set arrConvUM($i,newUM) $vSymb

	set fname [file join $txtsubdir $fnconvrule]

	set chk [catch {set fp [open $fname a+]} result]
	if {$chk != 0} {
		set messaggio "The file $fname not exist. Writing Default"
		tk_messageBox -message $messaggio -type ok
		#initConvRule
	}

	while {![eof $fp]} {
			set appo ""
			gets $fp appo
			puts "$appo"
	}

	puts $fp "$item $vCA $vCB $vSymb"

	flush $fp
	close $fp


	close_crule  .wmaincrule

	destroy .wdefcrule

	# reinizializzo l'array dopo lo modifiche
	initConvRule

	build_crule_win

}

proc ok_crule { win topw } {
	puts "ok_crule  called ..."
	save_crule $topw
	close_crule $win

}

proc apply_crule { topw } {
	puts "apply_crule called ..."
	save_crule $topw
}

proc close_crule  { win } {
	puts "close_crule called ..."
	destroy $win
}

proc save_crule { topw } { 
	global nConv
	global taskpath
	global txtsubdir
	global fnconvrule

	set fname [file join $txtsubdir $fnconvrule]

	set chk [catch {set fp [open $fname w]} result]
	if {$chk == 0} {
		set messaggio "The file $fname already exist. Overwrite"
		tk_messageBox -message $messaggio -type ok
	}


	set lconvType [list]
	set lcoeffA [list]
	set lcoeffB [list]
	set lsymbol [list]

	for {set i 0} {$i < $nConv } { incr i } {

		set wctype 	$topw.lbl$i
		set wA  	$topw.entA$i
		set wB  	$topw.entB$i
		set wsymb  	$topw.entSym$i



#		puts "label wid $wlname"
#		puts "entry wid $wlval"

		set vctype 	[$wctype get]
		set vcoeffA [$wA get]
		set vcoeffB [$wB get]
		set vsymb 	[$wsymb get]


		lappend lconvtype	$vctype 
		lappend lcoeffA  	$vcoeffA
		lappend lcoeffB  	$vcoeffB 
		lappend lsymbol  	$vsymb 

#		puts "$varinit -> $newval"

	}


	for { set i 0 } { $i < $nConv } { incr i} {
	
		set v1 [lindex $lconvtype $i]
		set v2 [lindex $lcoeffA $i]
		set v3 [lindex $lcoeffB $i]
		set v4 [lindex $lsymbol $i]

		puts "convtype: $v1 A: $v2 B: $v3 symbol: $v4 "
		puts $fp "$v1 $v2 $v3 $v4"
	}

	flush $fp
	close $fp

	# reinizializzo l'array dopo lo modifiche
	initConvRule

}



###################################################################
###### definizione conversione di default su variabili ############
###################################################################


proc define_var_conv { } {
	global arrDefUM
	global convUM
	global arrConvUM

	build_convar_win
}


proc build_convar_win { } {
	global convUM
	global arrDefUM
	global nDefUm
	global arrConvVar

    	set w .wmaincv
	set cvMainW $w
     	toplevel $w
	set titolo "Default Variable Conversion"
     	wm title $w $titolo
     	wm iconname $w $titolo
     	wm minsize $w 45 1

    	set top [frame $w.f -bd 1 -relief raised]
    	set box [tixButtonBox $w.b -bd 1 -relief raised]


	set sizeArrDefUM [array size arrDefUM]
	set listDefUm [array get arrDefUM]
	set nDefUm [llength $listDefUm]


	label $top.tit1 -text "Variable"   
	label $top.tit2 -text "Default Conversion"

	grid config $top.tit1  -column 0 -row 0 -sticky w
	grid config $top.tit2  -column 1 -row 0 

	# definisco una Label+Entry per ogni variabile
	# recuperando la label ed il valore dalla lista
	for { set i 0 } {$i < $nDefUm } { incr i 2 } {

		set row [expr $i + 1]

		set lname [lindex $listDefUm $i]	
		entry $top.lbl$i  -width 2 
		$top.lbl$i  delete 0 3
		$top.lbl$i  insert 0 $lname		
		$top.lbl$i  configure -state disabled -relief ridge

	    	# Create and configure comboBox 1
    		#
    		tixComboBox $top.cbx$i  \
			-dropdown true \
			-options {
	    			entry.width    15
	    			listbox.height 3
			}

		foreach cv $convUM {
	      	$top.cbx$i insert end $cv
    		}

		# devo settare l'indice definito precedentemente
		set idx $arrConvVar($lname)
    		$top.cbx$i pick $idx

		grid config $top.lbl$i -column 0 -row $row -sticky w
		grid config $top.cbx$i  -column 1 -row $row -sticky w
		

	}

	grid columnconfigure $top 0 -pad 2

    	# Create the buttons
    	#
    	$box add ok     	-text "Save & Exit"   	-command "ok_defcv $w $top" -width 6
    	$box add salva	-text "Save"     -command "apply_defcv $top" -width 6
    	$box add cancel 	-text "Quit" 	-command "close_defcv $w" -width 6


    	pack $box -side bottom -fill both
    	pack $top -side top -fill both -expand yes

}



proc ok_defcv { win topw } {
	puts "a_ok"
	# save on file and exit
	save_defcv $topw
	close_defcv $win
}

proc apply_defcv { topw } {
	puts "a_apply"
	# save on file without exit
	save_defcv $topw
}

proc close_defcv { win } {
	destroy $win
}

proc save_defcv { topw } {
	global nDefUm
	global taskpath
	global txtsubdir
	global fnvarconv

	set fname [file join $txtsubdir $fnvarconv]

	set chk [catch {set fp [open $fname w]} result]
	if {$chk == 0} {
		set messaggio "The file $fname already exist. Overwrite"
		tk_messageBox -message $messaggio -type ok
	}


	set vars [list]
	set convvar [list]

	for {set i 0} {$i < $nDefUm } { incr i 2 } {

		set wlname $topw.lbl$i		
		set varinit [$wlname get]

		set convstr [$topw.cbx$i subwidget entry get]

		#puts "variable: $varinit conversion: $convstr"

		lappend vars $varinit
		lappend convvar $convstr

	}

	# scrivo sul file di destinazione
	foreach vname $vars cvstr $convvar {

		set idx [searchConvInd $cvstr]
		puts $fp "$vname $idx"
	}

	flush $fp
	close $fp

	# reinizializzo l'array dopo lo modifiche
	initConvVar
}


proc findDefaultConv { initvar } {
	global arrConvVar

	set conv $arrConvVar($initvar)

	return $conv
} 