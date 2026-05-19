
source $env(LG_TIX)/checkopen.tcl   

proc leggi_file { file_da_leggere } {
global posx posy connett porta variabili progId
global connarr cmodMvarloc kporte porta
#tk_messageBox -icon info -type ok -message "leggi_file: eccomi"
	set kporte 0
	if {![file exists $file_da_leggere]} {
		tk_messageBox -message "file inesistente"
		return
	}
	set stream_to_file_to_read [checkopen $file_da_leggere r]
	while { [gets $stream_to_file_to_read line] != -1} {
#tk_messageBox -icon info -type ok -message "leggi_file: $line"
		set char_orientation [string index $line 0]
# cerco la linea che inzia per "n" ...			
		if {$char_orientation == "n" } {
			set coord($kporte) {}
			set tempcoord {}
			set m 7
			set B "J"
			set A [lrange $line 1 1]
			set i 2
			while { $B != {} } {				
				set B [string range $A $i [expr $m + $i]]
				incr i
				if {$B == "myportId"} {
					set m 0
					set B "V"
				# sono sicuro di essere sulla riga giusta
				}
				if { $m == 0 } {
					set val2 [ regexp {[0-9]} $B ma2]
					
					if { $B == "+" } {
						set tempcoord $tempcoord$B
					} elseif { $B == "-"} {
						set tempcoord $tempcoord$B
					} elseif { $B == "."} {
						set tempcoord $tempcoord$B
					} elseif { $val2 == 1} {
						set tempcoord $tempcoord$B
					} else { 
					}
					
				}
				set coord($kporte) $tempcoord
			}
			set m 5
			set i 60
			set B "J"
			while { $B != {} } {				
				set B [string range $A $i [expr $m + $i]]
				incr i
				if {$B == "-image"} {
					set connett($kporte) [string range $A  [expr 6 + $i] [expr 6 + $m + $i]]
				}
			}
		incr kporte
		}
		set AXCV [string range $line 0 2]
# cerco la definizione di una porta ...			
		if {$AXCV == "set"} {
			set AXCV1 [string range $line 4 17]
			set unaporta [regexp {port} $AXCV1]
			set valore [regexp {[0-9]+} $AXCV1 numr]
			if { $valore && $unaporta } {
#tk_messageBox -icon info -type ok -message "leggi_file: VALORE OK\n $line\n $AXCV\n $AXCV1\n $numr\n $valore"
				set porta([expr $kporte - 1]) $numr
				set variabili([expr $kporte - 1]) [lindex $line 2]
				set cmodMvarloc([expr $kporte - 1]) $variabili([expr $kporte - 1])
			}
# cerco la defnizione della variabile di animazione ...
			set animazione [regexp {var1_anim} $AXCV1]
			if { $animazione } { set ::var1_anim [string range $line 14 18] 
#tk_messageBox -icon info -type ok -message "leggi_file: -$::var1_anim- [string range $line 14 18]" 			
			}
		}
		
	}
	close $stream_to_file_to_read
	for {set k 0} {$k<$kporte} {incr k} {
		set x2 [string range $coord($k) 1 1]
		set x1 [string range $coord($k) 0 0]
		set i 1
		set num {}
		while { $x2 != {} } {
			set val2 [ regexp {([0-9])} $x2 ma2]
			if { $val2 == 1 || $x2 == "." } {
				set num $num$x1
			} else {
				set val1 [ regexp {([0-9])} $x1 ma1]	
				if { $val2 == 0 && $val1 == 1} {
					set num $num$x1
				}
			}
			set x1 $x2
			incr i
			set x2 [string range $coord($k) $i $i]
		}
		set xx [string range $num 0 0]
		set i 1
		set token 0
		set X $xx
		set Y {}
		while { $xx != {} } {
			set xx [string range $num $i $i]
			if { $xx == "+" || $xx == "-" } {
				set token 1
			}
			if { $token == 1 } {
				set Y $Y$xx
			} else {
				set X $X$xx
			}
			incr i
		}
		set posx($k) $X
		set posy($k) $Y
	
	}
	set progId $porta(0)
	for {set k 1} {$k<$kporte} {incr k} {
		if {$progId < $porta($k) } { set progId $porta($k) }
	}
	set progId [expr $progId + 1]
}

proc chooseModule {c icona} {
	global env cmodMvar cmodMvarRem pw curModName moduleX moduleY mmod mlib
	global tbox progId

	#   Type names		Extension(s)
	set types {{"Icons"		{*n.gif}}
		     {"All files"		*}}
	if {$icona == {} } {    
		set file [tk_getOpenFile -filetypes $types -parent $c -initialdir $env(LG_LIBRARIES)]
		if {$file == ""} {tk_messageBox -message "non ci sono files"
					return}
	      set Xfile [file rootname $file]
	} else {
		set Xfile [file rootname $icona]
		set file $icona
	}

      set lastlet [string range $Xfile end end]
      if {$lastlet == "e" || $lastlet == "s" || $lastlet == "w"} {
       tk_messageBox -icon warning -message "You can preinst only n-oriented GIF" -type ok
       return
      }

	catch {$c delete iconModule}
	catch {$c delete iconLittle}
	catch {image delete iconImage}
	catch {$c delete all}
	image create photo iconImage
	image create photo littleIcon -file $file
	iconImage copy littleIcon -zoom 2

	set littleX [expr [image width littleIcon] / 2 + 30]
	set littleY [expr [image height littleIcon] / 2 + 30]
	$c create image $littleX $littleY -image littleIcon -tags iconLittle

	$c create image $moduleX $moduleY -image iconImage -tags iconModule
	$c lower iconModule

	set mlib [file dirname $file]

	# here you must read the .pi3 file of the module
      # se l'ultima lettera indica un orientamento la tolgo
     
      if {$lastlet == "n"} {
       set Yfile [string trimright $Xfile $lastlet]
       set mvFileName $Yfile.pi3
      } else {
	 set mvFileName [file rootname $file].pi3
      }
	set fileid [checkopen $mvFileName r]
	gets $fileid aline
	set curModName $aline
	wm title . "Preinst - $curModName"

	gets $fileid aline; #math module name
	gets $fileid aline; #comment
# pre moduli di tipo REGOL.
	set ::tiporeg 0
	set ::tiporeg [regexp {#} $aline]
	
	gets $fileid aline; #separator line
	gets $fileid aline
	scan $aline "NSTAT = %d" numStati
	gets $fileid aline
	scan $aline "NUSCI = %d" numUscite
	gets $fileid aline
	scan $aline "NINGR = %d" numIngressi
	gets $fileid aline; #separator line
	gets $fileid aline; #NCONF line
	gets $fileid aline; #separator line

	catch {unset cmodMvar cmodMvarRem}
	for {set mv 0} {$mv < [expr $numStati + $numIngressi + $numUscite]} {incr mv} {
		gets $fileid aline
		scan $aline "%s" mvar
		lappend cmodMvar $mvar
	}
	close $fileid
	lappend cmodMvar "____"
	lappend cmodMvarRem "vuoto"
	
	if { [file exists $Yfile.tcl] } {
		global connIFOFvars posx posy connett porta kporte cmodMvarloc
		foreach i $connIFOFvars {
			global $i
		}
		set posx(0) 0
		set posy(0) 0
		set connett(0) 0 
		leggi_file $Yfile.tcl
		
		for {set k 0} {$k<$kporte} {incr k} {
		
			set conn [string range $connett($k) 0 3]
			set direction [string range $connett($k) 5 5]
#la linea di codice sottostante ha un piccolo baco nel caso in cui si mettano dei connettori al centro del 
#disegno del modulo			
			set nsoe [whichDir $conn$direction [expr $moduleX + 2 * $posx($k)] [expr $moduleY + 2 * $posy($k)] ]
			set halfwidth [expr ([image width $conn$direction] / 4)]
			set halfheight [expr ([image height $conn$direction] / 4)]
 			if { $nsoe == "input"} {
				set phAnchor $direction
			} else {
				set phAnchor [oppdir $direction]
			}
			
			switch $phAnchor {
				n {set posy($k) [expr ($posy($k) + $halfheight)] }
				s {set posy($k) [expr ($posy($k) - $halfheight)] }
				e {set posx($k) [expr ($posx($k) - $halfwidth)] }
				w {set posx($k) [expr ($posx($k) + $halfwidth)] }
			}
			set newcn [$c create image [expr $moduleX + 2 * $posx($k)] [expr $moduleY + 2 * $posy($k)] -image $conn$direction -tags [list phconn "id$porta($k)" $nsoe]]
			if { $nsoe == "input"} {
				set ktype [string range [$c itemcget $newcn -image] 0 3]IFvar
				set connarr ${conn}IFvar$porta($k)
			} else {
				set ktype [string range [$c itemcget $newcn -image] 0 3]OFvar
				set connarr ${conn}OFvar$porta($k)
			}
			global $connarr
			set i 0
			if {![info exists ${connarr}(0)]} {
				set ${connarr}(0) 1
				foreach {key value} [array get $ktype] {
					set ${connarr}($key) $value
					incr i
				}
			}
			set tlist {}
			foreach {key value} [array get $connarr {??[a-z]*}] {
				set stlist {}
				lappend stlist $key
				lappend stlist $value
				lappend tlist $stlist
			}
			set tlist [lsort -index 0 $tlist]

			set i 0
			foreach keyvalue $tlist {
				set key [lindex $keyvalue 0]
				set ${connarr}($key) [lindex $cmodMvarloc($k) $i]
				incr i
			}
		}
	}
}
proc phconnStartDrag {c x y} {
	$c raise current
	global lastX lastY startX startY
	set lastX [$c canvasx $x]
	set lastY [$c canvasy $y]
	set startX [$c canvasx $x]
	set startY [$c canvasy $y]
}

proc phconnDrag {c x y} {
	global lastX lastY snapVal

	set x [$c canvasx $x]
	set y [$c canvasy $y]
	if {[expr abs($x-$lastX)] > $snapVal || [expr abs($y-$lastY)] > $snapVal} {
		set dx [expr int([expr $x-$lastX]/$snapVal) * $snapVal]
		set dy [expr int([expr $y-$lastY]/$snapVal) * $snapVal]
		$c move current $dx $dy
		set lastX [expr $dx + $lastX]
		set lastY [expr $dy + $lastY]
	}
}

proc phconnFinishDrag {c x y} {
	$c dtag current input
	$c dtag current output
	$c addtag [whichDir [$c itemcget current -image] $x $y] withtag current
}

proc nextdir {dir clockw} {
	if {$clockw} {
		switch $dir {n {return e} e {return s} s {return w} w {return n}}
	} else {
		switch $dir {n {return w} e {return n} s {return e} w {return s}}
	}
}

proc oppdir {dir} { switch $dir {n {return s} e {return w} s {return n} w {return e}} }

proc toolAction {c x y} {
	global curCTool progId cmodMvar snapVal
	global connIFOFvars
	foreach i $connIFOFvars {
		global $i
	}
	# check if a connector is edited
	if {[$c find withtag hiliteConn] != {}} {bell; return}
	
	if {![info exists cmodMvar]} {tk_messageBox -icon warning -message "You must first select a module" -type ok; return}

	if {[string match *delete* $curCTool]} {
		if {[$c find withtag iconModule] != [$c find withtag current]} {$c delete current}
	} elseif {[string match *rotatel* $curCTool]} {
		if {[$c find withtag iconModule] != [$c find withtag current]} {
			# rotate left (counterclockwise)
			set currim [$c itemcget current -image]
			$c itemconfigure current -image \
				[string range $currim 0 3][nextdir [string index $currim 4] 0]
				$c dtag current input
				$c dtag current output
				$c addtag [whichDir [$c itemcget current -image] $x $y] withtag current
		}
	} elseif {[string match *rotater* $curCTool]} {
		if {[$c find withtag iconModule] != [$c find withtag current]} {
			# rotate right (clockwise)
			set currim [$c itemcget current -image]
			$c itemconfigure current -image \
				[string range $currim 0 3][nextdir [string index $currim 4] 1]
				$c dtag current input
				$c dtag current output
				$c addtag [whichDir [$c itemcget current -image] $x $y] withtag current
		}
	} else {
		set x [expr int($x/$snapVal) * $snapVal]
		set y [expr int($y/$snapVal) * $snapVal]
		set newconn [$c create image $x $y -image $curCTool -tags [list phconn "id$progId"]]
#puts stdout [$c gettags current] 		
		$c addtag [whichDir $curCTool $x $y] withtag $newconn
#puts stdout [whichDir $curCTool $x $y]
		incr progId

		# mathvar list management (to avoid empty list)
		if {[lsearch -exact [$c gettags $newconn] input] != -1} {set ct I} else {set ct O}
		set ctype [string range [$c itemcget $newconn -image] 0 3]${ct}Fvar
		set pcid [string range [lindex [$c gettags $newconn] 1] 2 end]

		set connarr $ctype$pcid
#puts stdout $ctype
		global $connarr
		# set the array
		if {![info exists ${connarr}(0)]} {
			set ${connarr}(0) 1
			foreach {key value} [array get $ctype] {
				set ${connarr}($key) $value
			}
		}

	}
}

proc whichDir {tconn x y} {
	global moduleX moduleY

	switch [string index $tconn 4] {
		n {if {$y > $moduleY} {return input} else {return output}}
		s {if {$y < $moduleY} {return input} else {return output}}
		w {if {$x > $moduleX} {return input} else {return output}}
		e {if {$x < $moduleX} {return input} else {return output}}
	}
}

proc phconnLinkMath {c x y} {
	global wcm mnamedone cmodMvar
	global connIFOFvars curPhysVar connarr
	
	foreach i $connIFOFvars {
		global $i
	}

	# puts active conn image on current item
	$c delete hiliteConn
	set phcX [lindex [$c coords current] 0]
	set phcY [lindex [$c coords current] 1]
	$c create image $phcX $phcY -image actconn -tags hiliteConn

	set wcm .linkmath
	# check linkMath existence
	catch {destroy $wcm}
	toplevel $wcm
	bind $wcm <Unmap> "$c delete hiliteConn"
if  { $::tcl_platform(os) != "Linux" } {
  grab $wcm
}
	# which connector type?
	if {[lsearch -exact [$c gettags current] input] != -1} {set ct I} else {set ct O}
#puts stdout "Connnector type: $ct"
	set ctype [string range [$c itemcget current -image] 0 3]${ct}Fvar
	set pcid [string range [lindex [$c gettags current] 1] 2 end]
	set connarr $ctype$pcid
	global $connarr

	set mnamedone 0
	set mndlg $wcm.fismathvars
	frame $mndlg -relief raised 

	pack $mndlg -side top -fill both -expand yes
		
	frame $mndlg.chk
	frame $wcm.lista

# if first time, set the array
	if {![info exists ${connarr}(0)]} {
		set ${connarr}(0) 1
		foreach {key value} [array get $ctype] {
			set ${connarr}($key) $value
		}
	}
	set tlist {}
#puts stdout [array get $connarr {??[a-z]*}]
	foreach {key value} [array get $connarr {??[a-z]*}] {
		set stlist {}
		lappend stlist $key
		lappend stlist $value
		lappend tlist $stlist
	}
	set tlist [lsort -index 0 $tlist]
set nvar 1
#puts stdout "Variables list: $tlist"
	foreach {keyvalue} $tlist {
		set key [lindex $keyvalue 0]
		set value [lindex $keyvalue 1]
		radiobutton $mndlg.chk.l$key -text [string range $key 2 end] -value $key -variable curPhysVar -width 10 -anchor w
		entry $mndlg.chk.e$key -relief sunken -width 4 -textvariable ${connarr}($key) -font entryFont
		grid $mndlg.chk.l$key $mndlg.chk.e$key -in $mndlg.chk 
		grid $mndlg.chk.l$key -sticky w -padx 1 -padx 2
		grid $mndlg.chk.e$key -pady 2 -padx 2
		incr nvar
	}
	
	$mndlg.chk.l$key select
	if {$nvar<=5} {
		set altezza [expr (0.2 * $nvar) + $nvar - 1]
	} else {
		set altezza [expr (0.25 * $nvar) + $nvar - 1]
	}
	set altezza [expr ceil ( $altezza ) ]
	set altezza [expr int ( $altezza ) ]
	scrollbar $wcm.lista.scroll -command "$wcm.lista.list yview"
	listbox $wcm.lista.list -yscroll "$wcm.lista.scroll set" -width 6 -height $altezza -font {Courier 10} -setgrid 1 -bg white
	pack $wcm.lista.list $wcm.lista.scroll -side left -fill y 
	
	foreach {mvar} $cmodMvar {
		$wcm.lista.list insert anchor $mvar
	}
	

	button $mndlg.dismiss -text Dismiss -command "destroy $wcm"
	grid $mndlg.chk -column 0 -row 0 -padx 12 -sticky n
	grid $wcm.lista -column 1 -row 0 -in $mndlg -ipadx 8 
	grid $mndlg.dismiss -column 0 -row 1 -pady 8 -padx 12 -sticky w
	bind $wcm.lista.list <Double-1> {
		set ${connarr}($curPhysVar) [selection get]
	}
}

proc Scroll_Set {scrollbar geoCmd offset size} {
	if {$offset != 0.0 || $size !=1.0} {
		eval $geoCmd
		$scrollbar set $offset $size
	} else {
		set manager [lindex $geoCmd 0]
		$manager forget $scrollbar
	}
}



##########
proc scegli_anim {} {
global cmodMvar
	set wanim .wanimaz
	# check linkMath existence
	catch {destroy $wanim}
	toplevel $wanim
	grab $wanim


	set mnamedone 0
	set mndlg $wanim.fismathvars
	frame $mndlg -relief raised 

	pack $mndlg -side top -fill both -expand yes
		
	frame $mndlg.chk
	frame $wanim.lista

## if first time, set the array
#	if {![info exists ${connarr}(0)]} {
#		set ${connarr}(0) 1
#		foreach {key value} [array get $ctype] {
#			set ${connarr}($key) $value
#		}
#	}
#	set tlist {}
##puts stdout [array get $connarr {??[a-z]*}]
#	foreach {key value} [array get $connarr {??[a-z]*}] {
#		set stlist {}
#		lappend stlist $key
#		lappend stlist $value
#		lappend tlist $stlist
#	}
#	set tlist [lsort -index 0 $tlist]
#set nvar 1
##puts stdout "Variables list: $tlist"
#	foreach {keyvalue} $tlist {
#		set key [lindex $keyvalue 0]
#		set value [lindex $keyvalue 1]
#		radiobutton $mndlg.chk.l$key -text [string range $key 2 end] -value $key -variable curPhysVar -width 10 -anchor w
#		entry $mndlg.chk.e$key -relief sunken -width 4 -textvariable ${connarr}($key) -font entryFont
#		grid $mndlg.chk.l$key $mndlg.chk.e$key -in $mndlg.chk 
#		grid $mndlg.chk.l$key -sticky w -padx 1 -padx 2
#		grid $mndlg.chk.e$key -pady 2 -padx 2
#		incr nvar
#	}
#	
#	$mndlg.chk.l$key select
set key 1
set animvar $::var1_anim
#tk_messageBox -message "scegli_anim: $::var1_anim"
		label $mndlg.chk.e$key  -relief sunken -text $animvar -textvariable animvar
#		entry $mndlg.chk.e$key -relief sunken -width 4 -text $animavar -textvariable animvar -font entryFont
#		grid $mndlg.chk.l$key $mndlg.chk.e$key -in $mndlg.chk 
#		grid $mndlg.chk.l$key -sticky w -padx 1 -padx 2
		grid $mndlg.chk.e$key 
	grid $mndlg.chk -column 0 -row 0 -padx 12 -sticky n

set nvar 7
	if {$nvar<=5} {
		set altezza [expr (0.2 * $nvar) + $nvar - 1]
	} else {
		set altezza [expr (0.25 * $nvar) + $nvar - 1]
	}
	set altezza [expr ceil ( $altezza ) ]
	set altezza [expr int ( $altezza ) ]
	scrollbar $wanim.lista.scroll -command "$wanim.lista.list yview"
	listbox $wanim.lista.list -yscroll "$wanim.lista.scroll set" -width 6 -height $altezza -font {Courier 10} -setgrid 1 -bg white
	pack $wanim.lista.list $wanim.lista.scroll -side left -fill y 
	
	foreach {mvar} $cmodMvar {
		$wanim.lista.list insert anchor $mvar
	}


	
$mndlg.chk.e$key configure -text $animvar
$wanim.lista.scroll activate 1

	button $mndlg.dismiss -text Dismiss -command "destroy $wanim"
	grid $wanim.lista -column 1 -row 0 -in $mndlg -ipadx 8 
	grid $mndlg.dismiss -column 0 -row 1 -pady 8 -padx 12 -sticky w
	bind $wanim.lista.list <Double-1> {
#		set ${connarr}($curPhysVar) [selection get]
		set animvar [selection get]

		set ::var1_anim $animvar

	}
#set ::var1_anim $animvar
#		tk_messageBox -message $::var1_anim
# fine scegli_anim
}

#####################
# Main
#####################


set curModName "untitled"
wm title . "Preinst�- $curModName"

frame .frame
pack .frame -side top -fill both -expand yes

set pc .frame.c

global num_arg arg_pass
set num_arg $argc
set arg_pass $argv

# per moduli di tipo REGOL.
set ::tiporeg 0

menu .menu -tearoff 0 -activebackground darkblue -activeforeground white
set pm .menu.file
menu $pm -tearoff 0 -activebackground darkblue -activeforeground white
.menu add cascade -label "File" -menu $pm -underline 0
$pm add command -label "Save" -command "writeModule . $pc 0"
$pm add command -label "Save As..." -command "writeModule . $pc 1"
$pm add separator
$pm add command -label "Quit" -command "destroy .toolbox; exit 0"

set pm .menu.module
menu $pm -tearoff 0 -activebackground darkblue -activeforeground white
.menu add cascade -label "Module" -menu $pm -underline 0
$pm add command -label "Choose..." -command "chooseModule $pc {}"

set pm .menu.window
menu $pm -tearoff 0 -activebackground darkblue -activeforeground white
.menu add cascade -label "Window" -menu $pm -underline 0
$pm add command -label "Animazione" -command "scegli_anim"; 
$pm add command -label "Toolbox" -command "source $env(LG_TIX)/preinit.tcl"; #temporary solution
. configure -menu .menu

set pwsBackg gray70
#set pwsWidth 512
#set pwsHeight 384

set pwsWidth 1024
set pwsHeight 768

canvas $pc -bg $pwsBackg -width $pwsWidth -height $pwsHeight
grid $pc -in .frame -row 0 -column 0 -rowspan 1 -columnspan 1 -sticky news

set moduleX [expr $pwsWidth/2]
set moduleY [expr $pwsHeight/2]

# canvas bindings

$pc bind phconn <1> "phconnStartDrag $pc %x %y"
$pc bind phconn <B1-Motion> "phconnDrag $pc %x %y"
$pc bind phconn <B1-ButtonRelease> "phconnFinishDrag $pc %x %y"
$pc bind phconn <Double-Button-1> "phconnLinkMath $pc %x %y"
bind $pc <Control-Button-1> "toolAction $pc %x %y"


catch {font create entryFont -family Courier -size 9}
set progId 0

#era cos� set snapVal [expr 4 * 2]; # it must be even
set snapVal [expr 1 * 2]; # it must be even 

source $env(LG_TIX)/read_con.tcl
source $env(LG_TIX)/preinit.tcl
source $env(LG_TIX)/presave.tcl
set mmod ____
set mlib .

set ::var1_anim ""

if {[expr ($argc > 0)]} {chooseModule $pc $argv}
