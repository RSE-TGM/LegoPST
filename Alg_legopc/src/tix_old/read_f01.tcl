proc loadF01 {c showres} {
	global env curFileName envir

	if {$curFileName == "untitled" || $curFileName == "" || $curFileName == "-" } {return 0}

	set LG_MODEL   [file dirname $curFileName]
#tk_messageBox -message "3 $curFileName - $LG_MODEL"
#puts "loadF01: ciao"
	set ok 1
	set messaggio "Model topology correctly loaded\n"

	if {$ok && $envir == "PostProc" } {
		cd $LG_MODEL		
		if {[readF01] == 0} {
			append messaggio "File F01.DAT reading phase done\n"
			return 1
		} else {
			append messaggio "File F01.DAT reading phase aborted...\n"
			set ok 0
		}
	}
	
	if {$ok && $envir != "Edit_Simul" } {
		if  { $::tcl_platform(os) == "Linux" } {
         		set comm1 [file join $env(LEGO_BIN) cad_crealg1 ]
	 		set comm2 "P"
		set lg1_error [catch {exec $comm1 $comm2} result]
#puts "loadF01: $curFileName - $LG_MODEL\n $comm1 $comm2 \n lg1_error=$lg1_error"

		} else {
        		if {[file exists [file join $LG_MODEL lg1_done.out]]} {
        			set lg1_error 0
        		} else {
        			set lg1_error 1
         		}
#	 		tk_messageBox -message "Verificare il comportamento di read_f01.tcl in Windows"
		}
		if {!$lg1_error} {
			append messaggio "Files F01 and F14 building phase done\n"
		} else {
			append messaggio "Files F01 and F14 building phase aborted...\n"
			set ok 0
		}
		
#		if {[file exists [file join $LG_MODEL lg1_done.out]]} {
#			append messaggio "Files F01 and F14 building phase done"
#		} else {
#			append messaggio "Files F01 and F14 building phase aborted..."
#			set ok 0
#		}
	}

	if {$ok} {
		cd $LG_MODEL
		if {[readF01] == 0} {
			append messaggio "File F01.DAT reading phase done\n"
		} else {
			append messaggio "File F01.DAT reading phase aborted...\n"
			set ok 0
		}
	}

	if {$ok && $envir != "Edit_Simul" } {
		if {[readF14new] == 0} {
			append messaggio "File F14.DAT \"empty\" reading phase done\n"
		} else {
			append messaggio "File F14.DAT \"empty\" reading phase aborted...\n"
			set ok 0
		}
	}

	if {$ok && $envir != "Edit_Simul" } {
		if {[readF14old] == 0} {
			append messaggio "Old data retrived from former F14.DAT file\n"
		} else {
			append messaggio "(no former F14.DAT file found for this model)\n"
		}
	}

	if {$ok == 0} {
		$c delete all
		if  { $::tcl_platform(os) != "Linux" } {
		wm title . "LegoPc - untitled"
		set curFileName untitled
		}
		append messaggio "\n"
		append messaggio "Topology unloaded.\n"

	} else {
		append messaggio "\nModel topology correctly loaded"
	}
puts "loadf01: 	$messaggio"
	if {$showres == "yes"} {tk_messageBox -message $messaggio -type ok}
	
	return $ok
}


proc readF01 {} {
	global numBlo listBlo
	global blocModu blocBloc blocDesc blocNvar blocVars blocNome
	global tipVarMod numeVblo listVblo matrVblo nomeModello
#tk_messageBox -message "sono readF01" -type ok
	set NrigF01 0
	set chk [catch {set fileid [open "f01.dat" r]} result]
	if {$chk != 0} {
		set messaggio "The F01.DAT file of the model doesn't exist"
		tk_messageBox -message $messaggio -type ok
		return 1
	}

	gets $fileid linea
	while {![eof $fileid]} {
		set righeF01($NrigF01) $linea
		incr NrigF01
		gets $fileid linea
	}
	close $fileid

	set i 0
	incr i      ; # skip first line "****"
	set numBlo 0
	set linea [string trim $righeF01($i)]
	while {$linea != "****"} {
		set blocco [string range $linea 18 21]
		set listBlo($numBlo) $blocco
		set blocModu($blocco) [string range $linea 0 3]
		set blocBloc($blocco) [string range $linea 18 21]
		set blocDesc($blocco) [string range $linea 26 end]
		incr numBlo
		incr i
		set linea [string trim $righeF01($i)]
	}
      incr i
	set nomeModello [string trim $righeF01($i)]
	set numeVblo(US) 0
	set numeVblo(UA) 0
	set numeVblo(IN) 0
	set numeVblo(CO) 0
	for {set j 0} {$j < $numBlo} {incr j} {
		incr i      ; # skip "****" at module beginning
		set blocco $listBlo($j)
		incr i      ; # go to module definition line
		set linea [string trim $righeF01($i)]
		set newbloc [string range $linea 14 17]
		if {$blocco != $newbloc} {
			set messaggio [format "Block name read from F01 (%s) different from expected (%s)!" $newbloc $blocco]
			tk_messageBox -message $messaggio -type ok
			return 1
		}
		set blocNome($blocco) [string range $linea 0 7]
		set k 0
		incr i
		set linea [string trim $righeF01($i)]
		while {$linea != "****"} {
			set nome [string range $linea 0 7]
			set tipo [string range $linea 12 13]
			set desc [string range $linea 17 end]
			set blocVars($blocco,$k,nome) $nome
			set blocVars($blocco,$k,tipo) $tipo
			set blocVars($blocco,$k,desc) $desc
			if {$tipo == "US"} {
				set tipVarMod($nome) "US"
				set matrVblo($nome,US,bloc) $blocco
				set matrVblo($nome,US,indx) $k
				set matrVblo($nome,US,valu) "          "
				set matrVblo($nome,US,code) "    "
				set listVblo(US,$numeVblo(US)) $nome
				incr numeVblo(US)
			} elseif {$tipo == "UA"} {
				set tipVarMod($nome) "UA"
				set matrVblo($nome,UA,bloc) $blocco
				set matrVblo($nome,UA,indx) $k
				set matrVblo($nome,UA,valu) "          "
				set matrVblo($nome,UA,code) "    "
				set listVblo(UA,$numeVblo(UA)) $nome
				incr numeVblo(UA)
			} elseif {$tipo == "IN" && [string range $desc 0 0] != "#"} {
				set tipVarMod($nome) "IN"
				set matrVblo($nome,IN,bloc) $blocco
				set matrVblo($nome,IN,indx) $k
				set matrVblo($nome,IN,valu) "          "
				set matrVblo($nome,IN,code) "    "
				set listVblo(IN,$numeVblo(IN)) $nome
				incr numeVblo(IN)
			}
			incr k
			incr i
			set linea [string trim $righeF01($i)]
		}
		set blocNvar($blocco) $k
	}
	incr i      ; # skip "input variables" section
	set linea [string trim $righeF01($i)]
	while {$linea != "****"} {
		incr i
		set linea [string trim $righeF01($i)]
	}
	for {incr i} {$i < $NrigF01} {incr i} {
		set linea [string trim $righeF01($i)]
		set nome [string range $linea 0 7]
		set desc [string range $linea 17 end]
		set matrVblo($nome,CO,nome) $nome
		set matrVblo($nome,CO,desc) $desc
		set listVblo(CO,$numeVblo(CO)) $nome
		incr numeVblo(CO)
	}
	return 0
}


proc saveF01 {} {
	global env curFileName numBlo modified1

	set LG_MODEL [file dirname $curFileName]
	set FILE_RNAME [file rootname $curFileName]

	set lg1_error 0

	if {$curFileName == "untitled" || $curFileName == ""} {
		set messaggio "No topology loaded yet... make it first, please!"
		set ok 0
	} elseif {$numBlo <= 0} {
		set messaggio "F01 file not loaded... make it first, please!"
		set ok 0
	} else {
		set ok 1
	}

	if {$ok} {
		cd $LG_MODEL
		if {[writF01 $LG_MODEL] == 0} {
			set messaggio "File F01.DAT correctly saved"
		} else {
			set messaggio "Errors in saving F01.DAT file..."
			set ok 0
		}
	}

	if {$ok} {
		append messaggio "\n"
		if {[writF14 $LG_MODEL] == 0} {
			append messaggio "File F14.DAT correctly saved"
		} else {
			append messaggio "Errors in saving F14.DAT file..."
			set ok 0
		}
	}

	if {$ok} {
#		append messaggio "\n"
#		set env(LEGO_PATH) [join [split $env(LG_LEGO) / ] \\ ]
#		set comm1 [file join $env(LG_LEGO) rerunlg2.bat]
#		set comm2 [file tail [file rootname $curFileName]]
#		set chk [catch {exec $comm1 >> $comm2.out} result]
#		if {[file exists lg2_done.out]} {
#			append messaggio "Files F02 and F03 updated"
#		} else {
#			append messaggio "Files F02 and F03 update aborted..."
#			set ok 0
#		}
#####
		append messaggio "\n"
		if  { $::tcl_platform(os) == "Linux" } {
			set comm1 [file join $env(LEGO_BIN) cad_crealg1 ]
	 		set comm2 "p"
	 		set comm3 ${FILE_RNAME}
			set comm4 ${FILE_RNAME}.out
			set lg1_error [catch {exec $comm1 $comm2 $comm3 > $comm4 2>>$comm4} result]
		} else {
			set env(LEGO_PATH) [join [split $env(LG_LEGO) / ] \\ ]   
			set comm1 [file join $env(LG_LEGO) rerunlg2.bat]
			set comm2 [file tail [file rootname $curFileName]]
			if {![catch {exec $comm1 > $comm2.out} result] && [file exists lg2_done.out]} {
				set lg1_error 0
			} else {
				set lg1_error 1
			}
		}
		if {!$lg1_error} {
			append messaggio "Files F02 and F03 updated"
		} else {
			append messaggio "Files F02 and F03 update aborted:\n"
			append messaggio "$result"
			set ok 0
		}
#####
	}

	tk_messageBox -message $messaggio -type ok
	set modified1 0
	return $ok
}


proc writF01 {LG_MODEL} {
	global numBlo listBlo
	global blocModu blocBloc blocDesc blocNvar blocVars blocNome
	global tipVarMod numeVblo listVblo matrVblo nomeModello
	
	if  { $::tcl_platform(os) == "Linux" } {
        	if { [catch {open "f01.dat" w} fileid]} {
        		set messaggio "New F01.DAT file cannot be recorded!"
        		tk_messageBox -message $messaggio -type ok
        		return 1
        	}
		
	} else {       
        	set chk [catch {set fileid [open "f01.dat" w]} result]
        	if {$chk != 0} {
        		set messaggio "New F01.DAT file cannot be recorded!"
        		tk_messageBox -message $messaggio -type ok
        		return 1
        	   }
	}

	puts $fileid "****"
	for {set i 0} {$i < $numBlo} {incr i} {
		set blocco $listBlo($i)
		set riga [format "%4.4s      BLOCCO (%4.4s) - %s" \
			$blocModu($blocco) $blocBloc($blocco) $blocDesc($blocco)]
		puts $fileid $riga
	}

	puts $fileid "****"
      puts $nomeModello
	puts $fileid $nomeModello

	for {set i 0} {$i < $numBlo} {incr i} {
		set blocco $listBlo($i)
		puts $fileid "****"
		set riga [format "%8.8s  BL.-%4.4s- **** MODULO %4.4s - %s" \
			$blocNome($blocco) $blocco $blocModu($blocco) $blocDesc($blocco)]
		puts $fileid $riga
		for {set j 0} {$j < $blocNvar($blocco)} {incr j} {
			set riga [format "%8.8s  --%2.2s-- %s" $blocVars($blocco,$j,nome) \
				$blocVars($blocco,$j,tipo) $blocVars($blocco,$j,desc)]
			puts $fileid $riga
		}
		puts $fileid "****"
	}

	puts $fileid "****      >>>>>>INGRESSI DI TUTTI I MODULI<<<<<<"
	for {set i 0} {$i < $numeVblo(IN)} {incr i} {
		set nome $listVblo(IN,$i)
		set blocco $matrVblo($nome,IN,bloc)
		set indice $matrVblo($nome,IN,indx)
		set riga [format "%8.8s  --%2.2s-- %s" $blocVars($blocco,$indice,nome) \
			$blocVars($blocco,$indice,tipo) $blocVars($blocco,$indice,desc)]
		puts $fileid $riga
	}

	puts $fileid "****"
	for {set i 0} {$i < $numeVblo(CO)} {incr i} {
		set nome $listVblo(CO,$i)
		set indice 
		set riga [format "%8.8s  --%2.2s-- %s" $nome CO $matrVblo($nome,CO,desc)]
		puts $fileid $riga
	}

	close $fileid
	return 0
}


proc loadVariables {mod} {
	global curFileName numBlo listBlo
	global blocModu blocBloc blocDesc blocNvar blocVars
	global numVars nomeVars tipoVars descVars valuVars weightvars
	global tipVarMod numeVblo listVblo matrVblo

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

	set numVars $blocNvar($mod)
	for {set i 0} {$i < $numVars} {incr i} {
		set nomeVars($i) $blocVars($mod,$i,nome)
		set tipoVars($i) $blocVars($mod,$i,tipo)
		set descVars($i) $blocVars($mod,$i,desc)
		set nom $nomeVars($i)
		set tip $tipVarMod($nom)
		set valuVars($i) [string trim $matrVblo($nom,$tip,valu)]
            set weightvars($i) $matrVblo($nom,$tip,code)
	}

	return 0
}


proc saveVariables {mod win} {

#tk_messageBox -message "$mod - $win" -type ok
	global curFileName numBlo listBlo modified1
	global blocModu blocBloc blocDesc blocNvar blocVars
	global numVars nomeVars tipoVars descVars valuVars weightvars
	global tipVarMod numeVblo listVblo matrVblo numeNoto

	for {set i 0} {$i < $numVars} {incr i} {
		set blocVars($mod,$i,nome) $nomeVars($i)
		set blocVars($mod,$i,tipo) $tipoVars($i)
		set blocVars($mod,$i,desc) [string range [string trim $descVars($i)] 0 54]
		set nom $nomeVars($i)
		set tip $tipVarMod($nom)
		set val [string trim $valuVars($i)]
		set matrVblo($nom,$tip,valu) [formattaValore $val $matrVblo($nom,$tip,valu) $nom]
		contaNoto $nom $tip $i
            set matrVblo($nom,$tip,code) $weightvars($i) 
	}
	
	agg_vis_noti

#set messaggio "saveVariables: 
#NumeNotoUS $numeNoto(US) NumeNotoUA $numeNoto(UA) NumeNotoIN $numeNoto(IN)
#numeVbloIN $numeVblo(IN) nemevbloUS $numeVblo(US)nemevbloUA $numeVblo(UA)"
#tk_messageBox -message $messaggio -type ok

###?	destroy $win
        set modified1 1
}




proc contaNoto {nom  tip  i} {
	global weightvars
	global matrVblo numeNoto 
# il vecchio valore �in $matrVblo il nuovo valore �in $weightvars
        if { $matrVblo($nom,$tip,code) != $weightvars($i)} {
               if {  $weightvars($i) == "NOTO" &&  $matrVblo($nom,$tip,code) != "NOTO" } { incr numeNoto($tip)}
               if {  $weightvars($i) != "NOTO" &&  $matrVblo($nom,$tip,code) == "NOTO" } { set numeNoto($tip) [expr $numeNoto($tip)-1]} 
        }
}

#
#               if { $matrVblo($nom,$tip,code) != "NOTO"} { incr numeNoto($tip)}
#               if { $matrVblo($nom,$tip,code) == "NOTO"} { 
#                set numeNoto($tip) [expr $numeNoto($tip)-1]
#               } 
#        }
#}

proc formattaValore {valore valprec nom} {
	global tcl_precision
	if {$valore == ""} {return "          "}
	set save_prec $tcl_precision
	set tcl_precision 9
	set chk [catch {set valstr [expr double($valore)]} retval]
	set tcl_precision $save_prec
	if {$chk != 0} {
		set messaggio "Error converting variable $nom value ($valore) - previous value kept!"
		tk_messageBox -message $messaggio -type ok
		return $valprec
	}
	set lun [string length $valstr]
	if {[string range $valstr [expr $lun-2] end] == ".0"} {
		set valstr [string range $valstr 0 [expr $lun-2]]
		set lun [expr $lun-1]
	}
	if {$lun <= 10 && ![string match {*[eEdD]*} $valstr]} {
		set fmt [string range "$valstr          " 0 9]
	} elseif {$valstr < 0} {
		set fmt [format "%10.2E" $valstr]
	} else {
		set fmt [format "%10.3E" $valstr]
	}
	return $fmt
}
