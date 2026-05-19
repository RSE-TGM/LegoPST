
source $env(LG_TIX)/checkopen.tcl

proc ShowGraf { kill evar1 evar2 evar3 evar4 } {
	global env argc argv grafpid f22name refr  args

	set command "$env(LG_GRAF)/grafics"

#tk_messageBox -message 	"ShowGraf: file da leggere=$f22name" -type ok
	if { $kill == 1 } {
                  set command2 "pskill"
                  catch { exec $command2 $grafpid }
        }


	set i 0
        set args($i) "-FILE=$f22name"; incr i
	if { $argc == 2 } { set args($i) "-R=$refr" ; incr i }



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

proc showIt { mod } {
        global curFileName campo
	global numVars nomeVars tipoVars tipVarMod envir f22name

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

	button $tit.modulo -textvariable campo(2) -width 12 -relief raised \
		-font entryFont -state disabled  \
		-disabledforeground black

	button $tit.nome -textvariable campo(0) -width 8 -relief raised \
		-font titleFont -state disabled -background green \
		-disabledforeground black
	button $tit.tipo -textvariable campo(1) -width 2 -relief raised \
		-font titleFont -state disabled -background green
	pack $tit.modulo -side top -anchor w
	pack $tit.nome $tit.tipo -side left -anchor w

	 if { [ string compare $mod "-" ] != 0 } { if {[loadVariables $mod] } return
	 } else { return }

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
     if  { $::tcl_platform(os) != "Linux" } {
     button .varch.butt.ok -text "Show" -command { ShowGraf 1 $evar1 $evar2 $evar3 $evar4 }
     button .varch.butt.oknew -text "Show New" -command { ShowGraf 0 $evar1 $evar2 $evar3 $evar4 }
     } else {
     button .varch.butt.ok -text "Show" -command { ShowGraf_lin 1 $evar1 $evar2 $evar3 $evar4 }
     button .varch.butt.oknew -text "Show New" -command { ShowGraf_lin 0 $evar1 $evar2 $evar3 $evar4 }
     }


     button .varch.butt.clear -text "Clear" -command { set evar1 ""; set evar2 "";set evar3 "";set evar4 ""; .varch.frame.r1 select}

     pack .varch

     pack .varch.frame .varch.butt -side top -pady 1m

     pack .varch.frame.r1 .varch.frame.e1 \
          .varch.frame.r2 .varch.frame.e2 \
          .varch.frame.r3 .varch.frame.e3 \
          .varch.frame.r4 .varch.frame.e4 -side left

     pack .varch.butt.ok .varch.butt.oknew  .varch.butt.clear -side left -padx 3m -pady 1m -fill x -expand 1
}

proc setSlot {name} {
	global rvar evar1 evar2 evar3 evar4

	set evar${rvar} $name
	incr rvar
	if { $rvar == 5 } {set rvar 1 }

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

set envir "Draw2Gr"

set grafpid -1
set f22name0 "-"
set refr 1
#tk_messageBox -message "draw2gr: argc=$argc argv0=[lindex $argv 0] argv1=[lindex $argv 1]"
set i 0



if  { $::tcl_platform(os) == "Linux" } {
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
  
} else {
  switch $argc  {
 	0 { set f22name "f22.f22"; set args($i) "-FILE=f22.f22"; incr i }
 	1 { set f22name "f22.f22"; set args($i) "-FILE=f22.f22"; incr i }
 	2 { set f22name [lindex $argv 1 ] ; set args($i) "-FILE=$f22name"; incr i }
	3 { set f22name [lindex $argv 1 ] ; set args($i) "-FILE=$f22name" ; incr i;  \
       	    set refr [lindex $argv 2 ] ; set args($i) "-R=$refr" ; incr i }
  }
}
set  ::dpimon [winfo fpixels . 1i ]

set modelname [file tail [pwd]]
set curFileName "$env(LG_MODELS)/$modelname/$modelname"
#wm title . "Graphics Vars Selection - $curFileName.tom"
set c .frame.c

menu .menu -tearoff 0
set m .menu.file
menu $m -tearoff 0 -activebackground darkblue -activeforeground white
.menu add cascade -label "File" -menu $m -underline 0
$m add command -label "Open f22..." -command "openf22 $c "
$m add command -label "Save Current f22 to..." -command "savef22 $c "
#$m add command -label "Select Vars" -command "destroy .varch;selVars"
$m add command -label "Quit" -command "destroy ."

set m .menu.vmgr
menu $m -tearoff 0 -activebackground darkblue -activeforeground white
.menu add cascade -label "View" -menu $m -underline 0

set showon 2

$m add radio -label "Show OFF" -variable showon -value 1 -command "ShowNames $c 1"
$m add radio -label "Show Names" -variable showon -value 2 -command "ShowNames $c 1; ShowNames $c 2"
$m add radio -label "Show Classes" -variable showon -value 3 -command "ShowNames $c 1; ShowNames $c 3"
if  { $::tcl_platform(os) == "Linux" } {
$m add radio -label "Graf sequential" -variable seqfile -value 1 -command "set ::grafexec $env(LEGORT_BIN)/grafics"
$m add radio -label "Graf circular" -variable seqfile -value 2 -command "set ::grafexec $env(LEGORT_BIN)/graphics"
}

# aggiungo menu per la ricerca dei blocchi
set ::canv1 $c
set ::modalita 1
$m add command -label "Find name" -command trova_win 
##############



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
source $env(LG_TIX)/read_con.tcl
source $env(LG_TIX)/read_f01.tcl
source $env(LG_TIX)/fileio.tcl
source $env(LG_TIX)/itemjoin.tcl
source $env(LG_TIX)/read_f14.tcl
source $env(LG_TIX)/viewmgr.tcl



topRead $c $curFileName
loadF01 $c no
#selVars

showIt "-"

catch { ShowNames $c 2}
wm title . "$envir - $f22name - $curFileName"

#tk_messageBox -message "drwa2grf: argc=$argc\n argv=$argv f22name=$f22name"