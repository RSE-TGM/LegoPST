###########################################
#
# readAllF14 
#
# lettura del file f14 su array arrRigheF14
#
###########################################
proc readAllF14 { } {
	global righeF14 arrRigheF14
	set chk [catch {set fileid [open "f14.dat" r]} result]
	if {$chk != 0} {
		set messaggio "The F14.DAT file of the model doesn't exist"
		tk_messageBox -message $messaggio -type ok
		return 1
	}
	gets $fileid linea
	while {![eof $fileid]} {
		set arrRigheF14($righeF14) $linea
		incr righeF14
		gets $fileid linea
	}
	close $fileid
}

###########################################
#
# searchVarValue 
#
# ricerca la variabile su arrRigheF14
# e restituisce il valore 
# se la variabile non è stata trovata 
# restituisce comunque 0
#
###########################################
proc searchVarValue { varname } {
	global righeF14 arrRigheF14

	#inizializzo variabile
	set varvalue 0

	for { set i 0 } { $i < $righeF14 } { incr i} {
		#puts "search row=$i"
		set found [string first $varname $arrRigheF14($i)]
		if { $found > 0 } {
			set  varvalue [string range $arrRigheF14($i) 14 23]
			puts "variable found " 
			break
		}
	}
	
	if { $found >= 0 } {
#		puts "VARIABLE FOUND: $varname VALUE: $varvalue"
	} else {
#		puts "VARIABLE NOT FOUND: $varname"
		set varvalue "-0"
	}

	return $varvalue
}
 

###########################################
#
# tabella per la definizione delle UM
# di default in base all'iniziale della variabile
#
###########################################
proc initDefUM { } {
	global arrDefUM

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
}

proc intConvRule { } {
	global arrConvUM

	# nessuna conversione
	set arConvUM(NN,a) 1
	set arConvUM(NN,b) 0

	# da °K a °C
	set arConvUM(KtoC,a) 0
	set arConvUM(KtoC,b) -273.15

	# da °C a °K
	set arConvUM(CtoK%,a) 0
	set arConvUM(CtoK%,b) 273.15

	# da Pascal a bar
	set arConvUM(PtoB,a) 10E-05
	set arConvUM(PtoB,b) 0

	# da bar a Pascal
	set arConvUM(BtoP,a) 10E+05
	set arConvUM(BtoP,b) 0

	# da p.u. a %
	set arConvUM(PUto%,a) 100
	set arConvUM(PUto%,b) 0

	# da % a p.u.
	set arConvUM(%toPU,a) 0.01
	set arConvUM(%toPU,b) 0


	# da Kg/s a t/h
	set arConvUM(KgstoTh%,a) [expr 1/3.6]
	set arConvUM(KgstoTh,b) 0

	# da t/h a Kg/s
	set arConvUM(ThtoKgs%,a) 3.6
	set arConvUM(ThtoKgs,b) 0


} 


proc convVal { Vin Vout a b } {
	
	# formula di vonversione
	set Vout [expr a*Vin + b]
}


###########################################
#
# readAllF01 
#
# lettura del file f01 su array arrRigheF01
#
###########################################
proc readAllF01 { } {
	global righeF01 arrRigheF01
	set chk [catch {set fileid [open "f01.dat" r]} result]
	if {$chk != 0} {
		set messaggio "The F01.DAT file of the model doesn't exist"
		tk_messageBox -message $messaggio -type ok
		return 1
	}
	gets $fileid linea
	while {![eof $fileid]} {
		set arrRigheF01($righeF01) $linea
		incr righeF01
		gets $fileid linea
	}
	close $fileid
}

###########################################
#
# searchVarValue 
#
# ricerca la variabile su arrRigheF14
# e restituisce il valore 
# se la variabile non è stata trovata 
# restituisce comunque 0
#
###########################################
proc searchF01VarDescr { varname } {
	global righeF01 arrRigheF01

	#inizializzo variabile
	set vardesc ""
	for { set i 0 } { $i < $righeF01 } { incr i} {
		#puts "search row=$i"

		set found [string first $varname $arrRigheF01($i)]

		if { $found >= 0 } {
			set  descr [string range $arrRigheF01($i) 17 53]

			#se il primo carattere descrizione 
			#  è il # devo continuare la ricerca
			if { [string range $descr 0 0 ] == "#" } { 
				puts "caso 2"
				set descr ""
			} else {
				puts "caso 1 $descr"
				set vardesc $descr
				break
			}
		}
	}
	
	if { $found >= 0 } {
		puts "VARIABLE FOUND: $varname DESCR: $vardesc"
	} else {
		puts "VARIABLE NOT FOUND: $varname"
	}

	return $vardesc
}

