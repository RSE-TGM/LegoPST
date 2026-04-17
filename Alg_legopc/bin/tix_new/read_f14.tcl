proc readF14new {} {
	global numBlo listBlo
	global blocModu blocBloc blocDesc blocNvar blocVars blocNome
	global tipVarMod numeVblo listVblo matrVblo numeNoto
	global normLine ndatDati vectDati

	set numeNoto(US) 0
	set numeNoto(UA) 0
	set numeNoto(IN) 0


	set NrigF14 0
	if  { $::tcl_platform(os) != "Linux" } {
	set chk [catch {set fileid [open "proc/f14.dat" r]} result]
        } else {
	set chk [catch {set fileid [open "f14.dat" r]} result]
        }



	if {$chk != 0} {
		set messaggio "readF14new: The F14.DAT file of the model doesn't exist"
		tk_messageBox -message $messaggio -type ok
		return 1
	}

	gets $fileid linea
	while {![eof $fileid]} {
		set righeF14($NrigF14) $linea
		incr NrigF14
		gets $fileid linea
	}
	close $fileid

	set i 0
	incr i      ; # skip first line "****"
	set normLine(0) [string trim $righeF14($i)]
	incr i
	set normLine(1) [string trim $righeF14($i)]

	incr i
	set linea [string trimright $righeF14($i)]
	set k 0
	while {$k < 3} {
		if {[string range $linea 0 3] == "*LG*"} {
			incr k
		} else {
			set nome [string range $linea 4 11]
			set valu [string range $linea 14 23]
			set code [string range $linea 39 42]
			set chk [catch {set tipo $tipVarMod($nome)} retval]
			if {$chk == 0} {
				set matrVblo($nome,$tipo,valu) $valu
				set matrVblo($nome,$tipo,code) $code
				if { $code == "NOTO" } { incr numeNoto($tipo) }
			} else {
				set messaggio "Variable $nome not found in new model...\n linea=$linea"
				tk_messageBox -message $messaggio -type ok
				return 1
			}
		}
		incr i
		set linea [string trimright $righeF14($i)]
	}
#set messaggio "NEW NumeNotoUS $numeNoto(US) NumeNotoUA $numeNoto(UA) NumeNotoIN $numeNoto(IN)"
#tk_messageBox -message $messaggio -type ok

	set blocco ""
	set found 0
	while {$linea != "*LG*EOF"} {
		set head [string range $linea 0 3]
		set lun [string length $linea]
		if {$head == "*LG*"} {
			set nomblo [string range $linea 20 27]
			set desblo [string range $linea 44 end]
			set blocco ""
			for {set j 0} {$j < $numBlo} {incr j} {
				set newblo $listBlo($j)
				if {$blocNome($newblo) == $nomblo} {set blocco $newblo}
			}
			if {$blocco != ""} {
				set ndatDati($blocco) 0
				set blocDesc($blocco) $desblo
				incr found
			} else {
				set messaggio "Block $nomblo not found in new model..."
				tk_messageBox -message $messaggio -type ok
				return 1
			}
		} elseif {$head == "*   "} {
			set k $ndatDati($blocco)
			set vectDati($blocco,$k,tipo) comm
			set vectDati($blocco,$k,text) [string range $linea 4 end]
			set ndatDati($blocco) [expr $k + 1]
		} elseif {$head == "    "} {
			set k $ndatDati($blocco)
			if {$lun == 25} {
				set tipo dat1
			} elseif {$lun == 50} {
				set tipo dat2
			} elseif {$lun == 75} {
				set tipo dat3
			} else {
				incr k
				set messaggio "Error in format of data line $k in block $blocco"
				tk_messageBox -message $messaggio -type ok
				return 1
			}
			set vectDati($blocco,$k,tipo) $tipo
			if {$tipo == "dat1" || $tipo == "dat2" || $tipo == "dat3"} {
				set vectDati($blocco,$k,sym1) [string range $linea 4 11]
				set vectDati($blocco,$k,val1) [string range $linea 14 23]
			}
			if {$tipo == "dat2" || $tipo == "dat3"} {
				set vectDati($blocco,$k,sym2) [string range $linea 29 36]
				set vectDati($blocco,$k,val2) [string range $linea 39 48]
			}
			if {$tipo == "dat3"} {
				set vectDati($blocco,$k,sym3) [string range $linea 54 61]
				set vectDati($blocco,$k,val3) [string range $linea 64 73]
			}
			set ndatDati($blocco) [expr $k + 1]
		} else {
			set k [expr $ndatDati($blocco) + 1]
			set messaggio "Wrong line type reading data line $k in block $blocco"
			tk_messageBox -message $messaggio -type ok
			return 1
		}
		incr i
		set linea [string trimright $righeF14($i)]
	}
	if {$found != $numBlo} {
		set messaggio "Found $found blocks data zone while expecting $numBloc"
		tk_messageBox -message $messaggio -type ok
		return 1
	}
	return 0
}


proc readF14old {} {
	global numBlo listBlo
	global blocModu blocBloc blocDesc blocNvar blocVars blocNome
	global tipVarMod numeVblo listVblo matrVblo numeNoto
	global normLine ndatDati vectDati

	set numeNoto(US) 0
	set numeNoto(UA) 0
	set numeNoto(IN) 0


	set NrigF14 0
	set chk [catch {set fileid [open "f14.dat" r]} result]
	if {$chk != 0} {return 1}

	gets $fileid linea
	while {![eof $fileid]} {
		set righeF14($NrigF14) $linea
		incr NrigF14
		gets $fileid linea
	}
	close $fileid

	set i 0
	incr i      ; # skip first line "****"
	set normLine(0) [string trim $righeF14($i)]
	incr i
	set normLine(1) [string trim $righeF14($i)]

	incr i
	set linea [string trimright $righeF14($i)]
	set k 0
	while {$k < 3} {
		if {[string range $linea 0 3] == "*LG*"} {
			incr k
		} else {
			set nome [string range $linea 4 11]
			set valu [string range $linea 14 23]
			set code [string range $linea 39 42]
			set desc [string range $linea 45 end]
			set chk [catch {set tipo $tipVarMod($nome)} retval]
			if {$chk == 0} {
				set matrVblo($nome,$tipo,valu) $valu
				set matrVblo($nome,$tipo,code) $code
				if { $code == "NOTO" } { incr numeNoto($tipo) }
				set blocco $matrVblo($nome,$tipo,bloc)
				set indice $matrVblo($nome,$tipo,indx)
				set blocVars($blocco,$indice,desc) $desc
			}
		}
		incr i
		set linea [string trimright $righeF14($i)]
	}
#set messaggio "OLD NumeNotoUS $numeNoto(US) NumeNotoUA $numeNoto(UA) NumeNotoIN $numeNoto(IN)"
#tk_messageBox -message $messaggio -type ok
	set blocco ""
	while {$linea != "*LG*EOF"} {
		set head [string range $linea 0 3]
		set lun [string length $linea]
		if {$head == "*LG*"} {
			set nomblo [string range $linea 20 27]
			set desblo [string range $linea 44 end]
			set blocco ""
			for {set j 0} {$j < $numBlo} {incr j} {
				set newblo $listBlo($j)
				if {$blocNome($newblo) == $nomblo} {set blocco $newblo}
			}
			if {$blocco != ""} {set blocDesc($blocco) $desblo}
		} elseif {$head == "    " && $blocco != ""} {
			if {$lun == 25} {
				set tipo dat1
			} elseif {$lun == 50} {
				set tipo dat2
			} elseif {$lun == 75} {
				set tipo dat3
			}
			if {$tipo == "dat1" || $tipo == "dat2" || $tipo == "dat3"} {
				set simbolo [string range $linea 4 11]
				set valore [string range $linea 14 23]
				cercaDato $blocco $simbolo $valore
			}
			if {$tipo == "dat2" || $tipo == "dat3"} {
				set simbolo [string range $linea 29 36]
				set valore [string range $linea 39 48]
				cercaDato $blocco $simbolo $valore
			}
			if {$tipo == "dat3"} {
				set simbolo [string range $linea 54 61]
				set valore [string range $linea 64 73]
				cercaDato $blocco $simbolo $valore
			}
		}
		incr i
		set linea [string trimright $righeF14($i)]
	}
	return 0
}


proc writF14 {LG_MODEL} {
	global numBlo listBlo
	global blocModu blocBloc blocDesc blocNvar blocVars blocNome
	global tipVarMod numeVblo listVblo matrVblo
	global normLine ndatDati vectDati

	set chk [catch {set fileid [open "f14.dat" w]} result]
	if {$chk != 0} {
		set messaggio "New F14.DAT file cannot be recorded!"
		tk_messageBox -message $messaggio -type ok
		return 1
	}

	set modelName [string toupper [file tail $LG_MODEL]]
	puts $fileid "*LG*NOME IMPIANTO =$modelName"
	puts $fileid $normLine(0)
	puts $fileid $normLine(1)

	puts $fileid "*LG*CONDIZIONI INIZIALI VARIABILI DEL SISTEMA (MKS)"
	set j 1
	for {set i 0} {$i < $numeVblo(UA)} {incr i} {
		set num [expr $j % 100]
		set nome $listVblo(UA,$i)
		set blocco $matrVblo($nome,UA,bloc)
		set indice $matrVblo($nome,UA,indx)
		set valore $matrVblo($nome,UA,valu)
		set codice $matrVblo($nome,UA,code)
		set blonam $blocNome($blocco)
		set descri $blocVars($blocco,$indice,desc)
		puts $fileid [format "%3.3s %8.8s =%10.10s*BL.(%8.8s)=%4.4s* %s" \
			$num $nome $valore $blonam $codice $descri]
		incr j
	}
	for {set i 0} {$i < $numeVblo(US)} {incr i} {
		set num [expr $j % 100]
		set nome $listVblo(US,$i)
		set blocco $matrVblo($nome,US,bloc)
		set indice $matrVblo($nome,US,indx)
		set valore $matrVblo($nome,US,valu)
		set codice $matrVblo($nome,US,code)
		set blonam $blocNome($blocco)
		set descri $blocVars($blocco,$indice,desc)
		puts $fileid [format "%3.3s %8.8s =%10.10s*BL.(%8.8s)=%4.4s* %s" \
			$num $nome $valore $blonam $codice $descri]
		incr j
	}

	puts $fileid "*LG*CONDIZIONI INIZIALI VARIABILI DI INGRESSO"
	set j 1
	for {set i 0} {$i < $numeVblo(IN)} {incr i} {
		set num [expr $j % 100]
		set nome $listVblo(IN,$i)
		set blocco $matrVblo($nome,IN,bloc)
		set indice $matrVblo($nome,IN,indx)
		set valore $matrVblo($nome,IN,valu)
		set codice $matrVblo($nome,IN,code)
		set blonam $blocNome($blocco)
		set descri $blocVars($blocco,$indice,desc)
		set linea [format "%3.3s %8.8s =%10.10s*BL.(%8.8s)=%4.4s* %s" \
			$num $nome $valore $blonam $codice $descri]
		puts $fileid $linea
		incr j
	}

	puts $fileid "*LG*DATI FISICI E GEOMETRICI DEL SISTEMA SUDDIVISI A BLOCCHI"
	for {set i 0} {$i < $numBlo} {incr i} {
		set blocco $listBlo($i)
		set blonam $blocNome($blocco)
		set modulo $blocModu($blocco)
		set descri $blocDesc($blocco)
		set linea [format "*LG*DATI DEL BLOCCO %8.8s  MODULO %4.4s - %s" \
			$blonam $modulo $descri]
		puts $fileid $linea
		for {set j 0} {$j < $ndatDati($blocco)} {incr j} {
			set tipo $vectDati($blocco,$j,tipo)
			if {$tipo == "comm"} {
				set linea [format "*   %s" $vectDati($blocco,$j,text)]
			} else {
				set linea ""
				if {$tipo == "dat1" || $tipo == "dat2" || $tipo == "dat3"} {
					append linea [format "    %8.8s =%-10.10s*" \
						$vectDati($blocco,$j,sym1) $vectDati($blocco,$j,val1)]
				}
				if {$tipo == "dat2" || $tipo == "dat3"} {
					append linea [format "    %8.8s =%-10.10s*" \
						$vectDati($blocco,$j,sym2) $vectDati($blocco,$j,val2)]
				}
				if {$tipo == "dat3"} {
					append linea [format "    %8.8s =%-10.10s*" \
						$vectDati($blocco,$j,sym3) $vectDati($blocco,$j,val3)]
				}
			}
			puts $fileid $linea
		}
	}

	puts $fileid "*LG*EOF"
	close $fileid
	return 0
}


proc cercaDato {blocco simbolo valore} {
	global ndatDati vectDati
	for {set j 0} {$j < $ndatDati($blocco)} {incr j} {
		set tipo $vectDati($blocco,$j,tipo)
		if {$tipo == "dat1" || $tipo == "dat2" || $tipo == "dat3"} {
			if {$vectDati($blocco,$j,sym1) == $simbolo} {
				set vectDati($blocco,$j,val1) $valore
				return
			}
		}
		if {$tipo == "dat2" || $tipo == "dat3"} {
			if {$vectDati($blocco,$j,sym2) == $simbolo} {
				set vectDati($blocco,$j,val2) $valore
				return
			}
		}
		if {$tipo == "dat3"} {
			if {$vectDati($blocco,$j,sym3) == $simbolo} {
				set vectDati($blocco,$j,val3) $valore
				return
			}
		}
	}
}


proc loadDataZone {mod} {
	global curFileName numBlo listBlo
	global ndatDati vectDati
	global numData vecData

	if {$numBlo <= 0} {
		set messaggio "F01 file not loaded... make it first, please!"
		tk_messageBox -message $messaggio -type ok
		return 1
	}

	set k -1
	for {set i 0} {$i < $numBlo} {incr i} {
		if {$mod == $listBlo($i)} {set k $i}
	}

	if {$k == -1} {
		set messaggio [format "Block name (%s) not found in F01 list..." $mod]
		tk_messageBox -message $messaggio -type ok
		return 1
	}

	set numData $ndatDati($mod)
	for {set i 0} {$i < $numData} {incr i} {
		set tipo $vectDati($mod,$i,tipo)
		set vecData($i,tipo) $tipo
		if {$tipo == "comm"} {
			set vecData($i,text) $vectDati($mod,$i,text)
		}
		if {$tipo == "dat1" || $tipo == "dat2" || $tipo == "dat3"} {
			set vecData($i,sym1) $vectDati($mod,$i,sym1)
			set vecData($i,val1) [string trimright $vectDati($mod,$i,val1)]
		}
		if {$tipo == "dat2" || $tipo == "dat3"} {
			set vecData($i,sym2) $vectDati($mod,$i,sym2)
			set vecData($i,val2) [string trimright $vectDati($mod,$i,val2)]
		}
		if {$tipo == "dat3"} {
			set vecData($i,sym3) $vectDati($mod,$i,sym3)
			set vecData($i,val3) [string trimright $vectDati($mod,$i,val3)]
		}
	}

	return 0
}



proc saveDataZone {mod win} {
	global curFileName numBlo listBlo modified1
	global ndatDati vectDati
	global numData vecData

	set male 0
	set messaggio "The max length of \"value\" fields is 10 characters:"
	for {set i 0} {$i < $numData} {incr i} {
		set tipo $vecData($i,tipo)
		if {$tipo == "dat1" || $tipo == "dat2" || $tipo == "dat3"} {
			set valu [string trimright $vecData($i,val1)]
			set lung [string length $valu]
			if {$lung > 10} {
				append messaggio "\n- datum \"$vecData($i,sym1)\" is $lung characters long"
				set male 1
			}
		}
		if {$tipo == "dat2" || $tipo == "dat3"} {
			set valu [string trimright $vecData($i,val2)]
			set lung [string length $valu]
			if {$lung > 10} {
				append messaggio \n- datum \"$vecData($i,sym2)\" is $lung characters long"
				set male 1
			}
		}
		if {$tipo == "dat3"} {
			set valu [string trimright $vecData($i,val3)]
			set lung [string length $valu]
			if {$lung > 10} {
				append messaggio \n- datum \"$vecData($i,sym3)\" is $lung characters long"
				set male 1
			}
		}
	}
	if {$male != 0} {
		append messaggio "\n\nPlease correct these data to allow saving!"
		tk_messageBox -message $messaggio -type ok
		return
	}

	for {set i 0} {$i < $numData} {incr i} {
		set tipo $vecData($i,tipo)
		if {$tipo == "dat1" || $tipo == "dat2" || $tipo == "dat3"} {
			set vectDati($mod,$i,val1) [format "%-10.10s" $vecData($i,val1)]
		}
		if {$tipo == "dat2" || $tipo == "dat3"} {
			set vectDati($mod,$i,val2) [format "%-10.10s" $vecData($i,val2)]
		}
		if {$tipo == "dat3"} {
			set vectDati($mod,$i,val3) [format "%-10.10s" $vecData($i,val3)]
		}
	}
	set modified1 1
###?	destroy $win
}
