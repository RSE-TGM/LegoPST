set ::LINUXPLAT 1
set ::modegrafpert "Plot"
set ::comandoAccShM "$env(LG_BIN)/LegoSim/LgSincroAccShM.exe"
set ::lastmod "-"

source $env(LG_TIX)/checkopen.tcl

source $env(LG_TIX)/balloon.tcl

# --- Directory della simulazione per Plot Mode e Command Mode ------------
# "Set Sim path" (menu View) imposta ::anima_sim_path = dir della simulazione
# in corso. animate.tcl fa gia' `cd` li' prima di viewval, cosi' l'animazione
# legge i file/valori giusti (e poi ripristina la cwd). Sia graphics (Plot) sia
# xaing (Command) vanno lanciati nella dir della simulazione:
#   - graphics: legge il file f22; ATTENZIONE f22name e' ASSOLUTO e ancorato
#     alla cwd di avvio (riga ~603), quindi ShowGraf_lin oltre al cd ripunta
#     esplicitamente args(0) a <simdir>/<basename f22>;
#   - xaing (pannello, via costruisci_var): legge "variabili.rtf" dalla CWD,
#     necessario anche per agganciare la SHM ([error shared-memory not attached]
#     se assente -> il pannello muore).
# Senza questo, con "Set Sim path" attivo: grafico sbagliato/crash / pannello
# xaing che non compare. d2g_sim_dir ritorna quella dir se valida, altrimenti ""
# (si resta nella cwd corrente).
proc d2g_sim_dir {} {
    if { [info exists ::anima_sim_path] && $::anima_sim_path ne "" \
         && $::anima_sim_path != 0 && [file isdirectory $::anima_sim_path] } {
        return $::anima_sim_path
    }
    return ""
}

proc ShowGraf { kill evar1 evar2 evar3 evar4} {
	global env argc argv grafpid f22name refr  args

	set command "$env(LG_GRAF)/grafics"

#tk_messageBox -message 	"ShowGraf: file da leggere=$f22name" -type ok
	if { $kill == 1 } {
                  catch {exec taskkill /F /PID $grafpid}
        }


	set i 0
        set args($i) "-FILE=$f22name"; incr i
	if { $argc == 2 || $argc == 3 } { set args($i) "-R=$refr" ; incr i }



#tk_messageBox -message 	$args(0) -type ok
#tk_messageBox -message 	$args(1) -type ok

	if {$evar1 != ""} {
 		set args($i) "-V=$evar1"; incr i
	}
	if {$evar2 != ""} {
 		set args($i) "-V=$evar2"; incr i
	}
	if {$evar3 != ""} {
 		set args($i) "-V=$evar3"; incr i
	}
	if {$evar4 != ""} {
 		set args($i) "-V=$evar4"; incr i
	}

	switch [expr $i - 1] {
 		1 {set grafpid [exec $command $args(0) $args(1) & ] }
 		2 {set grafpid [exec $command $args(0) $args(1) $args(2) & ] } \
 		3 {set grafpid [exec $command $args(0) $args(1) $args(2) $args(3) & ] } \
 		4 {set grafpid [exec $command $args(0) $args(1) $args(2) $args(3) $args(4) & ] } \
 		5 {set grafpid [exec $command $args(0) $args(1) $args(2) $args(3) $args(4) $args(5) & ] } \
 		default {tk_messageBox -message "Choose at least one variable!"}
	}
#tk_messageBox -message 	$grafpid -type ok
}

proc ShowGraf_lin { kill evar1 evar2 evar3 evar4 } {
	global env argc argv grafpid f22name refr  args

#	set command "$env(LEGORT_BIN)/graphics"
        set command $::grafexec


	#creo una lista dei processi attivi
	set result [exec ps]

	#elimino dalla lista i processi di tipo grafics che sono dichiarati defunti
	set index [lsearch $result *defunct*]
	while {$index != -1} {
		set result [lreplace $result [expr $index -1] $index "erased" "erased"]
		set index [lsearch $result *defunct*]
	}

	#cerco l'ultimo grafico fatto
	set index [lsearch $result *grafics*]
	set grafpid -1
	while {$index != -1} {
		set grafpid [lindex $result [expr $index -3]]
		set result [lreplace $result $index $index "erased"]
		set index [lsearch $result *grafics*]
	}

	#chiudo il grafico e lo rifaccio
        if { $kill == 1 && $grafpid != -1} {
                set command2 "kill"
                catch { exec $command2 $grafpid }
        }


	set i 0
        set args($i) "$f22name"; incr i
#		if { $argc == 2 } { set args($i) "-R=$refr" ; incr i }




	if {$evar1 != ""} {
 		set args($i) "$evar1"; incr i
	}
	if {$evar2 != ""} {
 		set args($i) "$evar2"; incr i
	}
	if {$evar3 != ""} {
 		set args($i) "$evar3"; incr i
	}
	if {$evar4 != ""} {
 		set args($i) "$evar4"; incr i
	}
#tk_messageBox -message  "draw2fgr: f22name:$f22name args0-->$args(0) args1-->$args(1)" -type ok

	# f22name arriva ASSOLUTO, ancorato alla cwd di AVVIO di draw2gr (riga ~603):
	# graphics leggerebbe quindi il f22 della dir di avvio, non della simulazione
	# scelta con "Set Sim path". Quando il sim path e' impostato ripuntiamo il
	# file f22 (args(0)) alla sua dir (stesso basename) e ci spostiamo li' con cd
	# (come animate.tcl con viewval), cosi' f22 e cwd restano coerenti; altrimenti
	# graphics legge un f22 sbagliato -> variabile assente / crash.
	set d2g_olddir [pwd]
	set d2g_simdir [d2g_sim_dir]
	if { $d2g_simdir ne "" } {
		set args(0) [file join $d2g_simdir [file tail $f22name]]
		catch { cd $d2g_simdir }
	}

	switch [expr $i - 1] {
 		1 {set grafpid [exec $command $args(0) $args(1) & ] }
 		2 {set grafpid [exec $command $args(0) $args(1) $args(2) & ] } \
 		3 {set grafpid [exec $command $args(0) $args(1) $args(2) $args(3) & ] } \
 		4 {set grafpid [exec $command $args(0) $args(1) $args(2) $args(3) $args(4) & ] } \
 		5 {set grafpid [exec $command $args(0) $args(1) $args(2) $args(3) $args(4) $args(5) & ] } \
 		default {tk_messageBox -message "Choose at least one variable!"}
	}
	catch { cd $d2g_olddir }
#tk_messageBox -message 	$grafpid -type ok
}



proc showVars { c x y} {
	set myTags [$c gettags current]
      if {[file rootname [lindex $myTags [lsearch $myTags *.ori]]] == ""} {
	 set modName [string range [lindex $myTags 3] 0 3]
      } else {
       set modName [string range [lindex $myTags 5] 0 3]
	}
	catch	{$c delete modOpt}

	showIt $modName
	$c delete modOpt
}


proc getIndipIng {} {

      set contarighe 0
      set index 0
      set fileid [checkopen "proc/f14.dat" r]
      gets $fileid linea
      while { $linea != "*LG*EOF" } {
       set righef14($contarighe) $linea
	 incr contarighe
	 gets $fileid  linea

       if { $linea == "*LG*CONDIZIONI INIZIALI VARIABILI DI INGRESSO  " } {
        while {$linea != "*LG*DATI FISICI E GEOMETRICI DEL SISTEMA SUDDIVISI A BLOCCHI" } {
            incr index
            set righef14($contarighe) $linea
	      incr contarighe
	      gets $fileid linea

             if { $linea != "*LG*DATI FISICI E GEOMETRICI DEL SISTEMA SUDDIVISI A BLOCCHI" } {
                set nome [string range $linea 4 11]
		    set rig $w3.middle.riga$index
		    frame $rig
	          pack $rig -side top
                set nome1 [ string range $linea 8 11 ]
                if { $mod == $nome1 } {
		       set nomeVars($index) $nome
                   set tipovar $tipVarMod($nome)
				}
				}
				}
				}
	}
}


proc showIt { mod } {
        global curFileName campo
	global numVars nomeVars tipoVars tipVarMod envir f22name
	global numVarsINDIP nomeVarsINDIP


    set ::lastmod $mod
#	if {[loadVariables $mod]} return

	set w1 .varwin

	catch {destroy $w1}
        frame .varwin

#        pack $w1 -side left

grid $w1 -in .frame -row 0 -column 3 -rowspan 1 -columnspan 1

#       set w1 .varwin
#	catch {destroy $w1}
#	toplevel $w1
#	wm title $w1 "Block $mod variables"
#	wm maxsize $w1 620 440

	set but $w1.buttons
	frame $but
	pack $but -side bottom -pady 10

#	button $but.quit -text "Quit" -command "destroy $w1"
#	pack $but.quit -side left -expand yes -ipadx 35 -padx 50

	set w2 $w1.sfondo
	tixScrolledWindow $w2 -scrollbar "auto -x"
	pack $w2

	set w3 [ $w1.sfondo subwidget window]

	frame $w3.middle
	pack $w3.middle  -side top -padx 10 -pady 10

	set tit $w3.middle.titolo
	frame $tit
	pack $tit -side top

	set campo(0) "Var.Name"
	set campo(1) "  "
	set campo(2) $mod
    if {[string compare $::modegrafpert "Plot"]} {
		set VarColor "red"
		} else { set VarColor "green"}

	button $tit.modulo -textvariable campo(2) -width 12 -relief raised \
		-font entryFont -state disabled  \
		-disabledforeground black

	button $tit.nome -textvariable campo(0) -width 8 -relief raised \
		-font titleFont -state disabled -background $VarColor \
		-disabledforeground black
	button $tit.tipo -textvariable campo(1) -width 2 -relief raised \
		-font titleFont -state disabled -background $VarColor
	pack $tit.modulo -side top -anchor w
	pack $tit.nome $tit.tipo -side left -anchor w

	 if { [ string compare $mod "-" ] != 0 } {
	 if {[loadVariables $mod] } return
	 } else { return }

if {$::modegrafpert == "Plot"} {
	for {set index 0} {$index < $numVars} {incr index} {
		set rig $w3.middle.riga$index
		frame $rig
		pack $rig -side top
            set nome $nomeVars($index)
            set tipovar $tipVarMod($nome)
		button $rig.nome -textvariable nomeVars($index) -width 8 -relief groove \
			-font entryFont -background gray80
		bind $rig.nome <1> "setSlot $nomeVars($index)"
		button $rig.tipo -textvariable tipoVars($index) -width 2 -relief groove \
			-font entryFont -state disabled -background yellow \
			-disabledforeground black
		pack $rig.nome $rig.tipo -side left
	}
} else {

	for {set index 0} {$index < $numVarsINDIP} {incr index} {
		set rig $w3.middle.riga$index
		frame $rig
		pack $rig -side top
            set nome $nomeVarsINDIP($index)
            set tipovar "IN"
		button $rig.nome -textvariable nomeVarsINDIP($index) -width 8 -relief groove \
			-font entryFont -background gray80
		bind $rig.nome <1> "setSlot $nomeVarsINDIP($index)"
		button $rig.tipo -text $tipovar -width 2 -relief groove \
			-font entryFont -state disabled -background yellow \
			-disabledforeground black
		pack $rig.nome $rig.tipo -side left
	}


}


}



# ----------------------------------------------------------------------
# Command Mode (Linux): perturbazione real-time tramite xaing
# ----------------------------------------------------------------------
# Su Linux l'analogo del servizio lgsincro di Windows e' la catena
#   xaing (modalita' send) -> msg RIC_AING -> pannello xaing -> g_perturba
# che opera sul runtime real-time net_simula (scheduler net_sked).

# Ritorna 1 se e' disponibile un contesto in cui ha senso perturbare
# (scheduler net_sked attivo). Il path FMU/lgser verra' aggiunto in seguito.
proc d2g_cmdmode_available {} {
    set running 0
    catch {
        set psout [exec ps -A -o comm]
        foreach _ln [split $psout "\n"] {
            if { [string trim $_ln] == "net_sked" } { set running 1; break }
        }
    }
    return $running
}

# Invia a xaing (tipo_aing=3, modalita' send) la richiesta di perturbazione
# per la variabile $name. xaing lancia il proprio pannello se assente e
# mostra il dialogo nativo per valore/tipo perturbazione, applicando la
# perturbazione via g_perturba.
proc d2g_send_aing {name} {
    global env
    set xaing [file join $env(LEGORT_BIN) xaing]
    if { ![file executable $xaing] } {
        tk_messageBox -icon error -type ok \
            -message "xaing non trovato in LEGORT_BIN:\n$xaing"
        return
    }
    # Il pannello xaing (xaing 1, forkato da xaing 3) legge "variabili.rtf" dalla
    # CWD e aggancia la SHM in base a quel modello: va quindi lanciato nella dir
    # della simulazione scelta con "Set Sim path", come animate.tcl fa con
    # viewval. Senza questo cd, con "Set Sim path" attivo xaing non trova
    # variabili.rtf -> "shared-memory not attached" -> il pannello muore (non
    # compare nulla). Il figlio (xaing 3) e poi il pannello ereditano la cwd.
    set d2g_olddir [pwd]
    set d2g_simdir [d2g_sim_dir]
    if { $d2g_simdir ne "" } { catch { cd $d2g_simdir } }
    if { [catch { exec $xaing 3 $name & } err] } {
        tk_messageBox -icon error -type ok \
            -message "Invio perturbazione fallito per $name:\n$err"
    }
    catch { cd $d2g_olddir }
}

proc selVars {} {
     global env
     global rvar evar1 evar2 evar3 evar4

#     toplevel .varch
#     wm title .varch "Selected Set"
#     wm iconname .varch "VARs"
#     wm minsize .varch 15 1

     frame .varch
     frame .varch.frame

     radiobutton .varch.frame.r1 -variable rvar -relief flat -value 1
     radiobutton .varch.frame.r2 -variable rvar -relief flat -value 2
     radiobutton .varch.frame.r3 -variable rvar -relief flat -value 3
     radiobutton .varch.frame.r4 -variable rvar -relief flat -value 4

     entry .varch.frame.e1 -width 15 -textvariable evar1
     entry .varch.frame.e2 -width 15 -textvariable evar2
     entry .varch.frame.e3 -width 15 -textvariable evar3
     entry .varch.frame.e4 -width 15 -textvariable evar4
     .varch.frame.r1 select

     frame .varch.butt
	 frame .varch.buttMode
	 set textbut "$::modegrafpert    Mode"
     if  { $::LINUXPLAT != 1 }  {
     button .varch.butt.ok -text "Show" -command { ShowGraf 1 $evar1 $evar2 $evar3 $evar4 }
     button .varch.butt.oknew -text "Show New" -command { ShowGraf 0 $evar1 $evar2 $evar3 $evar4 }

	button .varch.buttMode.mode   -background green  \
	   -text $textbut -textvariable textbut -font "Courier 12" \
	   -command    {
	          if { $::modegrafpert == "Plot" } {
				if { ([ non_esiste_processo "lgsincro" ] == 0)  } {
					catch {exec $::comandoAccShM -pict }
					catch {exec $::comandoAccShM -pert }
					set ::modegrafpert "Command"
					.varch.buttMode.mode configure -background red
					set textbut "$::modegrafpert Mode"
					showIt $::lastmod
					} else { return }

			  } else { \
				set ::modegrafpert "Plot"
				.varch.buttMode.mode configure -background green
				set textbut "$::modegrafpert    Mode"
				showIt $::lastmod

			}

		 }

	 set_balloon .varch.buttMode.mode "Press LgSincro Pert. to send commands"

     } else {
     button .varch.butt.ok -text "Show" -command { ShowGraf_lin 1 $evar1 $evar2 $evar3 $evar4 }
     button .varch.butt.oknew -text "Show New" -command { ShowGraf_lin 0 $evar1 $evar2 $evar3 $evar4 }

	button .varch.buttMode.mode   -background green  \
	   -text $textbut -textvariable textbut -font "Courier 12" \
	   -command    {
	          if { $::modegrafpert == "Plot" } {
	                if { [d2g_cmdmode_available] } {
	                        set ::modegrafpert "Command"
	                        .varch.buttMode.mode configure -background red
	                        set textbut "$::modegrafpert Mode"
	                        showIt $::lastmod
	                } else {
	                        tk_messageBox -icon info -type ok \
	                          -message "Command Mode richiede una simulazione attiva (net_sked)."
	                        return
	                }
	          } else {
	                set ::modegrafpert "Plot"
	                .varch.buttMode.mode configure -background green
	                set textbut "$::modegrafpert    Mode"
	                showIt $::lastmod
	          }
	    }
	 set_balloon .varch.buttMode.mode "Switch to Command Mode to send perturbations (xaing)"
     }


     button .varch.butt.clear -text "Clear" -command { set evar1 ""; set evar2 "";set evar3 "";set evar4 ""; .varch.frame.r1 select }

     frame .varch.taskname
     label .varch.taskname.lab -text "Task: $::modelname" \
           -font "Courier 12 bold" -foreground "#003399" -anchor center
     pack .varch.taskname.lab -padx 4 -pady 6 -fill x

     pack .varch

     grid .varch.frame -row 0 -column 0 -rowspan 1 -columnspan 1 -sticky news
     grid .varch.butt -row 1 -column 0 -rowspan 1 -columnspan 1 -sticky news
     grid .varch.buttMode -row 1 -column 1 -rowspan 1 -columnspan 1 -sticky news
     grid .varch.taskname -row 0 -column 1 -rowspan 1 -columnspan 1 -sticky news

     #pack .varch.frame .varch.butt -side top -pady 1m
	 #pack .varch.buttMode -side right -anchor e

     pack .varch.frame.r1 .varch.frame.e1 \
          .varch.frame.r2 .varch.frame.e2 \
          .varch.frame.r3 .varch.frame.e3 \
          .varch.frame.r4 .varch.frame.e4 -side left

#     pack  .varch.butt.ok  .varch.butt.okold  .varch.butt.clear  -side left
     if  { $::LINUXPLAT != 1 } {
     pack  .varch.butt.ok .varch.butt.oknew  .varch.butt.clear  -side left -padx 4m -pady 1m -fill x -expand 1
	 pack  .varch.buttMode.mode -side right -padx 0m -expand 1
	 } else {
     pack  .varch.butt.ok .varch.butt.oknew  .varch.butt.clear  -side left -padx 4m -pady 1m -fill x -expand 1
	 pack  .varch.buttMode.mode -side right -padx 0m -expand 1
	 }




	 }

proc setSlot {name} {
	global rvar evar1 evar2 evar3 evar4
	global env

	# Se un campo giallo è selezionato per il remap, intercetta il click
	# sulla lista e applica il remap invece di aggiungere una variabile al plot.
	# Controllo aggiuntivo: il modulo della lista aperta deve corrispondere
	# al modulo del campo selezionato (::anim_selected_mod == ::lastmod).
	if { $::anim_selected_item != -1 && $::modegrafpert == "Plot" } {
		if { [string equal $::anim_selected_mod $::lastmod] } {
			anim_apply_remap $::draw2gr_c $name
		} else {
			# Lista appartenente a un modulo diverso → non consentito
			tk_messageBox -icon warning -type ok \
				-message "Select a variable from the icon whose yellow field is highlighted."
		}
		return
	}

	if { $::modegrafpert == "Plot" }	{
		set evar${rvar} $name
		incr rvar
		if { $rvar == 5 } {set rvar 1 }
	} else {
	# invio la variabile da perturbare
		if { $::LINUXPLAT == 1 } {
			d2g_send_aing $name
		} else {
			set evar1{rvar} $name
			catch {exec $::comandoAccShM $name }
		}
	}

}


proc savef22 { c } {
	global f22name

		#   Type names		Extension(s)	Mac File Type(s)
		set types {
			{"Grafics file f22.f22"		{.f22}	}
			{"All files"		*}
		}
                set selPath [tk_getSaveFile -filetypes $types -parent $c -initialdir . -title "Save $f22name to a new file"]
		if {$selPath == ""} return
#tk_messageBox -message 	[ file tail  $selPath ] -type ok
		if { [ file tail  $f22name ] != [ file tail  $selPath ] } {

                     file copy -force $f22name $selPath
                     set nometargetg22 [ file rootname $selPath ].g22
                     set nomesourceg22 [ file rootname $f22name ].g22
                     file copy -force $nomesourceg22 $nometargetg22
                } else { tk_messageBox -message "Copy fault! $f22name is the source file! " -type ok   }

		 return

}

proc openf22 { c } {
global f22name envir curFileName
		#   Type names		Extension(s)	Mac File Type(s)
		set types {
			{"Grafics file f22.f22"		{.f22}	}
			{"All files"		*}
		}
                set selPath [tk_getOpenFile -filetypes $types -parent $c -initialdir . -title "Open graphic file"]

		if {$selPath == ""} return
		set f22name $selPath
		set f22nn [file tail $f22name ]
wm title . "$envir - $f22nn - $curFileName.tom"
		return

}

proc chk_exit {} {
# fermosincview
#		 anima chiudi qq
# imposto modalità pict per HMI interno in lgsincro
	set argom "-nopict"
	if { ([ non_esiste_processo "lgsincro" ] == 0)  } {
		catch { exec $::comandoAccShM $argom }
		}

    exit

  };# chk_exit




######################################################################
#
# Inizio main draw2gr.tcl
#
######################################################################


package require Tix

global env envir f22name
global wsBackg wsWidth wsBackg wsWidth wsHeight
global wsScrWidth wsScrHeight wsXsiz wsYsiz wsXmin wsYmin
global progNumb portw curTool progName palId jshift porth
global numBlo

#---main
wm protocol . WM_DELETE_WINDOW chk_exit

set envir "Draw2Gr"

set grafpid -1
set f22name0 "-"
set refr 1
#tk_messageBox -message "draw2gr: argc=$argc argv0=[lindex $argv 0] argv1=[lindex $argv 1]"
set i 0



if  { $::LINUXPLAT == 1 } {
if { [lindex $::argv 0] == 0 } {
  set ::grafexec $env(LEGORT_BIN)/grafics
  set seqfile 1
  } else {
  set ::grafexec $env(LEGORT_BIN)/graphics
  set seqfile 2
  }
  set curdir [pwd]
  #set f22name [file join $curdir f22circ ]
  set f22name [file join $curdir [lindex $::argv 1] ]
  #puts "draw2gr: f22name=$f22name"
  switch $argc  {
   	0 { set args($i) "$f22name"; incr i }
   	1 { set args($i) "$f22name"; incr i }
  	2 { set args($i) "$f22name" ; incr i}
    }
  # lgser_clientnum not used on Linux but must exist for ShowNamesfilt
  set ::lgser_clientnum -1

} else {
  # ::lgser_clientnum: numero SHM per sincview in modalità lgser-FMU
  # -1 = non impostato (usa lgsincro o f14)
  set ::lgser_clientnum -1

  switch $argc  {
 	0 { set f22name "f22.f22"; set args($i) "-FILE=f22.f22"; incr i }
 	1 { set f22name "f22.f22"; set args($i) "-FILE=f22.f22"; incr i }
 	2 { set f22name [lindex $argv 1 ] ; set args($i) "-FILE=$f22name"; incr i }
    3 { # FMU lgser mode: argv[2] = clientNum (es. 11,12,13)
        set f22name [lindex $argv 1]; set args($i) "-FILE=$f22name"; incr i
        set ::lgser_clientnum [lindex $argv 2] }
    default { set f22name [lindex $argv 1 ] ; set args($i) "-FILE=$f22name" ; incr i;  \
       	    set refr [lindex $argv 2 ] ; set args($i) "-R=$refr" ; incr i }
  }
}

set  ::dpimon [winfo fpixels . 1i ]

set modelname [file tail [pwd]]
# Usa [pwd] come directory del modello: funziona sia in modalità FMU bundle
# (cwd = bundle/<modelname>/ impostato da CreateProcessA) sia in modalità
# normale (cwd = LG_MODELS/<modelname>/ dopo apertura modello).
# LG_MODELS è process-global: con più FMU nello stesso processo (es. Simulink)
# viene sovrascritto dall'ultima istanza inizializzata → non affidabile.
set curFileName [file join [pwd] $modelname]
wm title . "Graphics Vars Selection - $curFileName.tom"
#wm title . "HMI & Plot Plot - $f22nn - $curFileName.tom"

set c .frame.c
set ::draw2gr_c $c

menu .menu -tearoff 0
set m .menu.file
menu $m -tearoff 0 -activebackground darkblue -activeforeground white
.menu add cascade -label "File" -menu $m -underline 0
$m add command -label "Open f22..." -command "openf22 $c "
$m add command -label "Save Current f22 to..." -command "savef22 $c "
#$m add command -label "Select Vars" -command "destroy .varch;selVars"
$m add command -label "Quit" -command "chk_exit"

set m .menu.vmgr
menu $m -tearoff 0 -activebackground darkblue -activeforeground white
.menu add cascade -label "View" -menu $m -underline 0

set showon 2

# $m add radio -label "Show OFF" -variable showon -value 1 -command "ShowNames $c 1"
# $m add radio -label "Show Names" -variable showon -value 2 -command "ShowNames $c 1; ShowNames $c 2"
# $m add radio -label "Show Classes" -variable showon -value 3 -command "ShowNames $c 1; ShowNames $c 3"

    $m add radio -label "Show OFF" -variable showon -value 1 -command "ShowNamesfilt $c $c 1"
    $m add radio -label "Show Names" -variable showon -value 2 -command "ShowNamesfilt $c $c 1; ShowNamesfilt $c $c 2"
    $m add radio -label "Show Classes" -variable showon -value 3 -command "ShowNamesfilt $c $c 1; ShowNamesfilt $c $c 3"
    $m add command -label "Show Connections..." -command "viewConn_dlg $c $c $c"
    $m add radio -label "Show Value" -variable showon -value 4 -command "ShowNamesfilt $c $c 1; ShowNamesfilt $c $c 4"


if  { $::LINUXPLAT == 1 } {
$m add radio -label "Graf sequential" -variable seqfile -value 1 -command "set ::grafexec $env(LEGORT_BIN)/grafics"
$m add radio -label "Graf circular" -variable seqfile -value 2 -command "set ::grafexec $env(LEGORT_BIN)/graphics"
}

# aggiungo menu per la ricerca dei blocchi
set ::canv1 $c
set ::modalita 1
$m add command -label "Find name" -command trova_win

# Zoom submenu
set mzoom .menu.vmgr.zoom
menu $mzoom -tearoff 0 -activebackground darkblue -activeforeground white
$m add cascade -label "Zoom" -menu $mzoom
$mzoom add command -label "400%"  -command {doZoom $::canv1 4.0}
$mzoom add command -label "300%"  -command {doZoom $::canv1 3.0}
$mzoom add command -label "200%"  -command {doZoom $::canv1 2.0}
$mzoom add command -label "150%"  -command {doZoom $::canv1 1.5}
$mzoom add command -label "100%"  -command {doZoom $::canv1 1.0}
$mzoom add command -label " 75%"  -command {doZoom $::canv1 0.75}
$mzoom add command -label " 50%"  -command {doZoom $::canv1 0.5}
$mzoom add command -label " 25%"  -command {doZoom $::canv1 0.25}
##############

# Set Sim path: sceglie la directory del simulatore verso cui viewval fa `cd`
# (animate.tcl:133) prima di leggere le variabili dalla Shared Memory per
# animare lo schema. Replica la voce omonima del tab "Data Assignment &
# Simulation" di legopc.tix: ::anima_sim_path e' condivisa via animate.tcl,
# la label dell'unica voce mostra il path corrente e il click apre
# tk_chooseDirectory aggiornandola.
# NB: animate.tcl (che definisce ::anima_sim_path) e' sorgiato piu' sotto,
# mentre questo menu si costruisce prima: inizializziamo la variabile qui col
# medesimo default se assente, per evitare "no such variable".
if {![info exists ::anima_sim_path]} { set ::anima_sim_path "locpath - click to change" }
if { $::LINUXPLAT == 1 } {
    set d2g_simpath .menu.vmgr.simpath
    menu $d2g_simpath -tearoff 0 -activebackground darkblue -activeforeground white
    $m add cascade -label "Set Sim path" -menu $d2g_simpath -underline 0
    $d2g_simpath add command -label [set ::anima_sim_path] -command {
        set ::anima_sim_path [tk_chooseDirectory -initialdir $::anima_sim_path -title "Choose simulator path"]
        .menu.vmgr.simpath entryconfigure 0 -label [set ::anima_sim_path]
    }
}


#
#$m add separator
#set showbutt 1
#$m add check -label "Show buttons" -variable showbutt -command {
#         if { $showbutt == 1 } { selVars; showIt "-" } else { destroy .varch; destroy .varwin }
#}
#

. configure -menu .menu

selVars;

frame .frame
frame .varwin

#grid .varwin -in . -row 0 -column 0 -rowspan 1 -columnspan 1 -sticky ns
#grid .frame -in . -row 0 -column 1 -rowspan 1 -columnspan 1 -sticky news

pack .varwin .frame -fill both -expand yes
#pack .frame -side left -fill both -expand yes


set wsBackg gray90
set wsWidth 512
set wsHeight 384
set wsScrWidth 800
set wsScrHeight 600
set wsXsiz $wsScrWidth
set wsYsiz $wsScrHeight
set wsXmin $wsWidth
set wsYmin $wsHeight

set progNumb 0
set portw 12
set curTool none
set progName "name"
set palId 0
set jshift .5
set porth 12
set numBlo 0


canvas $c -bg $wsBackg -scrollregion [list 0 0 $wsScrWidth $wsScrHeight] \
	-width $wsWidth -height $wsHeight -xscrollcommand ".frame.hscroll set" \
	-yscrollcommand ".frame.vscroll set"
scrollbar .frame.vscroll -command "$c yview"
scrollbar .frame.hscroll -orient horiz -command "$c xview"


grid $c -in .frame -row 0 -column 0 -rowspan 1 -columnspan 1 -sticky news
grid .frame.vscroll -row 0 -column 1 -rowspan 1 -columnspan 1 -sticky news
grid .frame.hscroll -row 1 -column 0 -rowspan 1 -columnspan 1 -sticky news
catch {
	ttk::sizegrip .frame.sizegrip
	grid .frame.sizegrip -row 1 -column 1 -sticky se
}

grid rowconfig .frame 0 -weight 1 -minsize 0
grid columnconfig .frame 0 -weight 1 -minsize 0


set pixmapPath "$env(LG_PIXMAPS)"
set olddir [pwd]
cd $pixmapPath
foreach singleconn [glob -nocomplain {????_[news].ppm}] {
	image create photo [file rootname $singleconn] -file [file join $pixmapPath $singleconn]
}
image create photo portActive -file [file join $pixmapPath actconn.ppm]
cd $olddir

#help files
set helpPath "$env(LG_HELP)"
catch {font create helpFont -family Helvetica -size 9}
catch {font create entryFont -family Courier -size 9}
catch {font create titleFont -family Courier -size 9 -weight bold -slant italic}
catch {font create entryBig -family Courier -size 14}
catch {font create titleBig -family Courier -size 14 -weight bold}

$c bind module <1> "showVars $c %x %y"

# ============================================================
# Zoom canvas: scaling runtime delle immagini con image copy
# Livelli fissi: 25%, 50%, 75%, 100%, 150%, 200%, 300%, 400%
# ============================================================
array set ::zoomRatios {
    025 {1 4}
    050 {1 2}
    075 {3 4}
    100 {1 1}
    150 {3 2}
    200 {2 1}
    300 {3 1}
    400 {4 1}
}
array set ::zoomLevelOf {}

proc scaleImage {basename key} {
    if {$key == "100"} { return $basename }
    set dest "z${key}_${basename}"
    if {[lsearch -exact [image names] $dest] < 0} {
        set ratio $::zoomRatios($key)
        set zf [lindex $ratio 0]
        set sf [lindex $ratio 1]
        image create photo $dest
        $dest copy $basename -zoom $zf $zf -subsample $sf $sf
    }
    return $dest
}

proc doZoom {c newLevel} {
    set newKey [format "%03d" [expr {round($newLevel * 100)}]]
    if {![info exists ::zoomRatios($newKey)]} return
    set curLevel [expr {[info exists ::zoomLevelOf($c)] ? $::zoomLevelOf($c) : 1.0}]
    if {$newLevel == $curLevel} return
    set delta [expr {$newLevel / $curLevel}]
    $c scale all 0 0 $delta $delta
    foreach item [$c find withtag module] {
        if {[$c type $item] != "image"} continue
        set curImg [$c itemcget $item -image]
        regsub {^z[0-9]+_} $curImg {} base
        $c itemconfigure $item -image [scaleImage $base $newKey]
    }
    foreach item [$c find withtag port] {
        if {[$c type $item] != "image"} continue
        set curImg [$c itemcget $item -image]
        regsub {^z[0-9]+_} $curImg {} base
        $c itemconfigure $item -image [scaleImage $base $newKey]
    }
    foreach item [$c find all] {
        if {[$c type $item] != "text"} continue
        set fontspec [$c itemcget $item -font]
        if {$fontspec == ""} continue
        if {![info exists ::origFontOf($c,$item)]} {
            set ::origFontOf($c,$item) $fontspec
        }
        if {[catch {set fa [font actual $::origFontOf($c,$item)]}]} continue
        set origSize [lindex $fa 3]
        if {$origSize == 0} continue
        set newSize [expr {round(abs($origSize) * $newLevel)}]
        if {$newSize < 1} { set newSize 1 }
        if {$origSize < 0} { set newSize [expr {-$newSize}] }
        set family [lindex $fa 1]
        set weight [lindex $fa 5]
        set slant  [lindex $fa 7]
        $c itemconfigure $item -font [list $family $newSize $weight $slant]
    }
    set ::zoomLevelOf($c) $newLevel
    $c configure -scrollregion [list 0 0 \
        [expr {$::wsXsiz * $newLevel}] \
        [expr {$::wsYsiz * $newLevel}]]
}

proc resetZoom {c} {
    doZoom $c 1.0
}

# Zoom con Ctrl+MouseWheel
bind $c <Enter> {focus %W}
bind $c <Control-MouseWheel> {
    set _zlevels {0.25 0.5 0.75 1.0 1.5 2.0 3.0 4.0}
    set _cur [expr {[info exists ::zoomLevelOf(%W)] ? $::zoomLevelOf(%W) : 1.0}]
    set _idx [lsearch $_zlevels $_cur]
    if {%D > 0} { incr _idx } else { incr _idx -1 }
    if {$_idx >= 0 && $_idx < [llength $_zlevels]} {
        doZoom %W [lindex $_zlevels $_idx]
    }
}
# Dopo topRead il canvas è ridisegnato a 100%: sincronizza stato zoom
proc draw2gr_syncZoom {c} {
    set ::zoomLevelOf($c) 1.0
    array unset ::origFontOf ${c},*
}

source $env(LG_TIX)/read_con.tcl
source $env(LG_TIX)/read_f01.tcl
source $env(LG_TIX)/fileio.tcl
source $env(LG_TIX)/itemjoin.tcl
source $env(LG_TIX)/read_f14.tcl
source $env(LG_TIX)/viewmgr.tcl

#------------------------- nuovo guag 2020
source $env(LG_TIX)/animate.tcl

# ── Backg Color con colori recenti (condivisi con legopc.tix via stesso file prefs) ──
# Popup tasto destro canvas: mostra "Backg Color" solo se il click NON è
# su un item infoitemname (casella animazione / Show Names).
# In draw2gr.tcl il menu bgcolor è .menu.vmgr.bgcolor (separato da legopc.tix).

if {![info exists ::recentColors]} { set ::recentColors {} }

proc d2g_prefsFile {} {
    global env
    return [file join $env(LG_ENTRY) legopc_prefs.tcl]
}
proc d2g_loadPrefs {} {
    set pf [d2g_prefsFile]
    if {[file exists $pf]} { catch {source $pf} }
}
proc d2g_savePrefs {} {
    # Aggiorna solo la riga recentColors senza toccare le voci di legopc.tix
    set pf [d2g_prefsFile]
    set lines {}
    if {[file exists $pf]} {
        set fh [open $pf r]
        set lines [split [read $fh] \n]
        close $fh
    }
    set out {}
    foreach ln $lines {
        if {![string match "set ::recentColors*" $ln]} { lappend out $ln }
    }
    lappend out "set ::recentColors [list $::recentColors]"
    set fh [open $pf w]
    puts $fh [join $out \n]
    close $fh
}
proc d2g_addRecentColor {color} {
    set idx [lsearch -exact $::recentColors $color]
    if {$idx >= 0} { set ::recentColors [lreplace $::recentColors $idx $idx] }
    set ::recentColors [linsert $::recentColors 0 $color]
    if {[llength $::recentColors] > 8} {
        set ::recentColors [lrange $::recentColors 0 7]
    }
}
proc d2g_rebuildBgMenu {} {
    set menu .menu.vmgr.bgcolor
    set stop 0
    while {!$stop} {
        if {[catch {set last [$menu index end]}]} {
            set stop 1
        } elseif {$last < 2} {
            set stop 1
        } else {
            $menu delete $last
        }
    }
    if {[llength $::recentColors] > 0} {
        $menu add separator
        foreach color $::recentColors {
            $menu add command -label "  $color" \
                -background $color \
                -command [list d2g_applyBgColor $color]
        }
    }
}
proc d2g_applyBgColor {color} {
    global wsBackg
    set wsBackg $color
    $::canv1 configure -bg $color
}
proc d2g_setBgColor {mode} {
    global wsBackg
    if {$mode == "default"} {
        set color gray90
    } else {
        set color [tk_chooseColor -initialcolor $wsBackg -title "Background color"]
        if {$color == ""} return
        d2g_addRecentColor $color
        d2g_rebuildBgMenu
        d2g_savePrefs
    }
    d2g_applyBgColor $color
}

d2g_loadPrefs

menu .menu.vmgr.bgcolor -tearoff 0
set popc $c.pop
menu $popc -activebackground darkblue -activeforeground white -tearoff 0
$popc add cascade -label "Backg Color" -menu .menu.vmgr.bgcolor
.menu.vmgr.bgcolor add command -label "Default"   -command {d2g_setBgColor default}
.menu.vmgr.bgcolor add command -label "Custom..." -command {d2g_setBgColor custom}
d2g_rebuildBgMenu

proc d2g_popup_if_background {pop W X Y x y} {
    set cx [$W canvasx $x]
    set cy [$W canvasy $y]
    set hits [$W find overlapping $cx $cy $cx $cy]
    foreach h $hits {
        if {[lsearch [$W gettags $h] infoitemname] >= 0} { return }
    }
    tk_popup $pop $X $Y
}
bind $c <ButtonRelease-3> "d2g_popup_if_background $popc %W %X %Y %x %y"
set lgsincropid 1



proc non_esiste_processo { prid } {

# ritorna 1 se NON esiste il processo con il process id "prid"
# Usa tasklist (standard Windows, no Sysinternals richiesto)

switch $::LINUXPLAT {
			0 {
				if {[regexp {^[0-9]+$} $prid] && $prid > 0} {
					set filter "PID eq $prid"
				} else {
					set nome $prid
					if {![regexp -nocase {\.exe$} $nome]} { append nome ".exe" }
					set filter "IMAGENAME eq $nome"
				}
				catch {exec tasklist /FI $filter /FO CSV /NH} output
				# found: output inizia con '"' (CSV); not found: msg INFO
				if {[string first "\"" $output] == 0} {
					return 0
				}
				return 1
			}

			1 {
				return 1
			}
		}
}
proc ShowNamesfilt { c c2 mod } {
  global modalita lgsincropid matrVf14

  switch $mod  {
#modo debug per fare alcune stampe
  5       { set qq [$c2 gettags infoitemname ]
#tk_messageBox  -message "ITEM: $item\n 0: [lindex [$c gettags $item] 0 ]\n 1: [lindex [$c gettags $item] 1 ]\n 2: [lindex [$c gettags $item] 2 ]\n 3: [lindex [$c gettags $item] 3 ]\n 4: [lindex [$c gettags $item] 4 ]\n 5: [lindex [$c gettags $item] 5 ]\n"
#tk_messageBox  -message "ShowNamesfilt: $c2: $qq\n [ lindex $qq 0]\n [ lindex $qq 1]\n [ lindex $qq 2]\n [ lindex $qq 3]\n"
            set mod 4 }

  4       {
            if  { $::LINUXPLAT == 1 } {
                anima chiudi qq
                set sked_running 0
                catch {
                    set psout [exec ps -A -o comm]
                    foreach _ln [split $psout "\n"] {
                        if { [string trim $_ln] == "net_sked" } { set sked_running 1; break }
                    }
                }
                if { $sked_running == 1 } {
                    anima init qq
                    anima_aggiorna $c2 1
                } else {
                    anima_aggiorna $c2 3
                }
            } else {
                # metto in animazione lo schema con i valori del transitorio
                if { [ non_esiste_processo "lgsincro" ] == 0 } {
                  # LgSincro in esecuzione: usa sincview con SharedLego1
                  anima chiudi qq
                  anima init qq
                  anima_aggiorna $c2 1
                } elseif { $::lgser_clientnum >= 0 } {
                  # FMU lgser diretto: usa sincview con SharedLego{clientNum}
                  anima chiudi qq
                  anima init qq
                  anima_aggiorna $c2 1
                } else {
                  # Nessuna simulazione attiva: mostra valori statici da f14
                  anima chiudi qq
                  anima_aggiorna $c2 3
                }
            }

            }

	default {
                   anima chiudi qq
				   ShowNames $c $mod
	               ShowNames $c2 $mod
            }
        }
}

#------------------------- FINE nuovo guag 2020

# ============================================================
# REMAP: variabile animata per icona (rimappatura persistente)
# ============================================================
# ::anim_remap(pisqu_name) = nome_variabile_completo  (es. "T02TURBO1")
# ::anim_selected_item     = canvas item id icona selezionata (-1 = nessuna)
# ::anim_selected_rect     = canvas item id del rettangolo giallo selezionato
# ::anim_selected_mod      = codice modulo (4 chars) dell'icona selezionata

array set ::anim_remap {}
set ::anim_selected_item -1
set ::anim_selected_rect -1
set ::anim_selected_mod  ""

# Gestisce il doppio-click sul campo giallo sotto un'icona.
# item  = canvas id del modulo
# rect  = canvas id del rettangolo giallo
# modtags = lista dei tag del modulo (passata da animate.tcl)
proc anim_field_select { c item rect modtags } {
    # Deseleziona il campo precedentemente selezionato
    if {$::anim_selected_rect != -1} {
        catch { $c itemconfigure $::anim_selected_rect \
                    -fill yellow -outline yellow }
    }

    # Toggle: doppio-click sullo stesso campo → deseleziona
    if {$::anim_selected_item == $item} {
        set ::anim_selected_item -1
        set ::anim_selected_rect -1
        set ::anim_selected_mod  ""
        return
    }

    # Selezione nuovo campo
    set ::anim_selected_item $item
    set ::anim_selected_rect $rect

    # Ricava codice modulo (4 chars) dai tag per filtrare la lista variabili
    # Usa la stessa logica di showVars: tag index 5 (NOME.name) → 4 chars
    # ma più robusto: cerca *.name e usa i primi 4 char del nome istanza
    # (corrispondono al tipo di blocco F01)
    set name_tag [file rootname [lindex $modtags [lsearch $modtags *.name]]]
    set ::anim_selected_mod [string range $name_tag 0 3]

    # Evidenzia il campo selezionato (bordo verde, sfondo lime)
    $c itemconfigure $rect -fill "#CCFF99" -outline "#00AA00"

    # Apre automaticamente la lista variabili per questo modulo
    # (stessa chiamata di showVars ma senza bisogno di current)
    showIt $::anim_selected_mod
}

# Applica il remap: chiamato da setSlot quando un campo è selezionato.
# name = nome variabile completo (da nomeVars, es. "T02TURBO1")
proc anim_apply_remap { c name } {
    global tipVarMod

    set item $::anim_selected_item
    set rect $::anim_selected_rect
    if {$item == -1} return

    # Sicurezza: la variabile deve esistere in F01
    if {![info exists tipVarMod($name)]} return

    # Ricava nome istanza dal tag *.name del modulo
    set tags_curr [$c gettags $item]
    set pisqu_name [file rootname [lindex $tags_curr [lsearch $tags_curr *.name]]]

    # Aggiorna remap in memoria
    set ::anim_remap($pisqu_name) $name

    # Aggiorna tag *.nome_anim sull'item (letto dal loop mode 2)
    set old_nome_anim [lsearch $tags_curr *.nome_anim]
    if {$old_nome_anim >= 0} {
        set old_tag [lindex $tags_curr $old_nome_anim]
        $c dtag $item $old_tag
    }
    $c addtag ${name}.nome_anim withtag $item

    # Aggiorna subito il testo visibile (senza aspettare il prossimo tick)
    set pisqu_tid [file rootname [lindex [$c gettags $item] \
                       [lsearch [$c gettags $item] *.visual]]]
    catch { $c itemconfigure $pisqu_tid -text "$name ..." }

    # Ripristina colore giallo normale e deseleziona
    catch { $c itemconfigure $rect -fill yellow -outline yellow }
    set ::anim_selected_item -1
    set ::anim_selected_rect -1
    set ::anim_selected_mod  ""

    # Salva su disco
    catch { anim_save_remap }
}

topRead $c $curFileName
viewConn_reapply $c
draw2gr_syncZoom $c
loadF01 $c no
#selVars

showIt "-"

catch { ShowNames $c 2}
#wm title . "$envir - $f22name - $curFileName"
wm title . "HMI & Plot  - $f22name - $curFileName.tom"



# imposto modalità pict esterno per lgsincro
if { ([ non_esiste_processo "lgsincro" ] == 0)  } {
catch { exec $::comandoAccShM -pict }
# Se avviato in Command Mode (da tasto Pert di LgSincro: argv[3] == "command")
if { $argc >= 4 && [string equal [lindex $argv 3] "command"] } {
    catch { exec $::comandoAccShM -pert }
    set ::modegrafpert "Command"
    .varch.buttMode.mode configure -background red
    set textbut "$::modegrafpert Mode"
}
}

#tk_messageBox -message "drwa2grf: argc=$argc\n argv=$argv f22name=$f22name"
