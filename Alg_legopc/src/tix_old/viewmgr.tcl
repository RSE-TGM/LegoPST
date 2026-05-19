set linkDlg "empty"
set infoDlg "empty"
set infoMode "none"

proc optLinks {c} {
	global linkDlg wsBackg convConst
	global connlist extname idconn clines cvardef
	if {$linkDlg == "empty"} {
		toplevel .linkdlg
		set linkDlg .linkdlg
		set d $linkDlg
		wm title $d "link Options"
		frame $d.buttons
		pack $d.buttons
		checkbutton $d.buttons.smooth -text "smooth" -variable linecurve -indicatoron true -command "toggleCurve $c"
		pack $d.buttons.smooth -side top -anchor w
		foreach tycon $connlist {
			checkbutton $d.buttons.$tycon -text $extname($tycon) \
				-variable clines($tycon,active) -onvalue $clines($tycon,color) \
				-offvalue $wsBackg -command "showLinks $c $tycon 0" \
				-indicatoron true
			$d.buttons.$tycon select
			pack $d.buttons.$tycon -side top -anchor w
		}
                checkbutton $d.buttons.styledrag -text "style drag" \
				-variable ::styledrag -onvalue 1 \
				-offvalue 0  \
				-indicatoron true	
		pack $d.buttons.styledrag -side top -anchor w	
		scale $d.curs -from 50 -to 100 -orient horizontal -variable convConst
		pack $d.curs -side right -anchor ne
	} else {raise $linkDlg; wm deiconify $linkDlg}
	bind $linkDlg <Destroy> "set linkDlg empty"
}

proc showLinks {c mode nascondi} {
	global connlist extname idconn clines cvardef
	global wsBackg
	set urk [$c find withtag ${mode}_ltype]
      if {$nascondi == 0} {
	foreach it $urk {
		$c itemconfigure $it -fill $clines($mode,active)
	}
      } else {
	foreach it $urk {
		$c itemconfigure $it -fill $wsBackg
	}      	
      }
      	     	
	if {$clines($mode,active) == "$wsBackg"} {
		foreach it $urk {$c lower $it}
	} else {
		foreach it $urk {$c raise $it}
	}
}


proc nascondi_links { c modo } {
	global connlist extname idconn clines cvardef
	global wsBackg
# modo=1 nasconde modo=0 visualizza i collegamenti	

foreach tycon $connlist {
	showLinks $c $tycon $modo
	}


}

proc optInfo {c} {
	global infoDlg infoMode
	if {$infoDlg == "empty"} {
		toplevel .infodlg
		set infoDlg .infodlg
		set d $infoDlg
		wm title $d "info Options"
		frame $d.buttons
		pack $d.buttons
		
#		radiobutton $d.buttons.none -text "None" -variable infoMode -value "none"  -indicatoron true
#		$d.buttons.none select
#		radiobutton $d.buttons.name -text "Name" -variable infoMode -value "name"  -indicatoron true
#		radiobutton $d.buttons.class -text "Class" -variable infoMode -value "class" -indicatoron true
#		radiobutton $d.buttons.help -text "Help" -variable infoMode -value "help" -indicatoron true

		radiobutton $d.buttons.none -text "None" -variable infoMode -value "none" -command { $c bind module <Any-Enter> "" } -indicatoron true   
		$d.buttons.none select                                                                                                                  
		radiobutton $d.buttons.name -text "Name" -variable infoMode -value "name" -command { $c bind module <Any-Enter> "itemEnter $c" } -indicatoron true
		radiobutton $d.buttons.class -text "Class" -variable infoMode -value "class" -command { $c bind module <Any-Enter> "itemEnter $c" } -indicatoron true                                          		
		radiobutton $d.buttons.help -text "Help" -variable infoMode -value "help" -command { $c bind module <Any-Enter> "itemEnter $c" } -indicatoron true                                             	

		pack $d.buttons.none $d.buttons.name $d.buttons.class $d.buttons.help -side top -anchor w
		} else {raise $infoDlg; wm deiconify $infoDlg}
	bind $infoDlg <Destroy> "set infoDlg empty; set infoMode none "
}

proc toggleCurve {c} {
	global linecurve

	foreach item [$c find withtag connection] {
		if {$linecurve == 1} {
			$c itemconfigure $item -smooth true
		} else {
			$c itemconfigure $item -smooth false
		}
	}
}

proc ShowNames { c tipo } {
         set nelem 0
#         set ms 1000
         if { $tipo == 1} {
                # cancello le scritte
		catch [$c delete infoitemname]
	 return
	 }       
         foreach item [$c find withtag module] {
#tk_messageBox  -message "ITEM: $item\n [$c gettags $item]\n [lsearch [$c gettags $item] *.name]\n [lindex [$c gettags $item] [lsearch [$c gettags $item] *.name]]]"
#tk_messageBox  -message "ShowNames: ITEM: $item\n 0: [lindex [$c gettags $item] 0 ]\n 1: [lindex [$c gettags $item] 1 ]\n 2: [lindex [$c gettags $item] 2 ]\n 3: [lindex [$c gettags $item] 3 ]\n 4: [lindex [$c gettags $item] 4 ]\n 5: [lindex [$c gettags $item] 5 ]\n"

		set lc [$c bbox $item] 
#		set x [lindex $lc 2]
#tk_messageBox  -message "ShowNames: ITEM: $item\n lc=$lc"
		set x [expr [expr [lindex $lc 0]+[lindex $lc 2]]/2]
#in alto a destra
#		set y [lindex $lc 1]
#in basso a destra
		set y [expr [lindex $lc 3] + 6]
#tk_messageBox -icon warning -message "$lc x=$x y=$y" -type ok
        	if { $tipo == 2 } { set pisqu [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.name]]] }
		if { $tipo == 3 } { set pisqu [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.cls]]] }
		if { $tipo == 4 } { set pisqu " " }
# guag 4/2007 ignoro gli item con nome della classe che contiene @
            set strstr [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.cls]]]
	    set nonmodulo [regexp {@} $strstr]
#tk_messageBox -message "writeFiles: item=$item strsstr=$strstr nonmodulo=$nonmodulo" -type ok
	    if { $nonmodulo == 0 || $tipo == 3 } {
         	set tid [$c create text $x $y -text $pisqu -tags {infoitemname}]
		set lc [$c bbox $tid] 
		set x1 [lindex $lc 0]
		set y1 [lindex $lc 1]
		set x2 [lindex $lc 2]
		set y2 [lindex $lc 3]
		$c create rectangle $x1 $y1 $x2 $y2 -fill yellow -tags {infoitemname}
		$c raise $tid
	    }
		incr nelem
         } 
#after $ms $c delete infoitemname
}

proc trova_win {}  {
	global env SerialNumber modalita canv
	catch { destroy .trovaW }
	toplevel .trovaW
	wm title .trovaW "Find item"

	label .trovaW.lab -text "Serach for:" -font "Courier 12"
	label .trovaW.result -text "  " -font "Courier 12"
	entry .trovaW.ent -textvariable NameToSearch -width 22 -state normal

    case $modalita {
	1 {
	    set canv $::canv1
	}
	2 {
	    set canv $::canv2
	}
	3 {
	    set canv $::canv3
	}
	default {
	    set canv $::canv1
	}
    }



#	set lounlo "Locked"
#	checkbutton .sn.unlock -indicatoron 0 -background red -activebackground green \
#	  -text $lounlo -textvariable lounlo -font "Courier 12" -command { 
#	    if { $lounlo == "Locked" } {
#	       set lounlo "Unlocked"
#	       .sn.unlock configure -background green
#	       .sn.ent configure -state normal 
#	    } else { set lounlo "Locked" 
#	       .sn.unlock configure -background red
#	       .sn.ent configure -state disabled }
#	    }

	wm protocol .trovaW WM_DELETE_WINDOW "$canv delete trovato_item; destroy .trovaW"

	frame .trovaW.but

	button .trovaW.but.find -text Find -command {
		.trovaW.result  configure -text "  "  
		$canv delete trovato_item
		set ret [trova_item $canv $NameToSearch 0]
		if {$ret == 0 } {.trovaW.result  configure -text "Not Found!"}  
		return
        }
	button .trovaW.but.next -text Next -command {
		.trovaW.result  configure -text "  "  
		$canv delete trovato_item
		set ret [trova_item $canv $NameToSearch 1]
		if {$ret == 0 } {.trovaW.result  configure -text "Not Found!"}  
		return
        }
	button .trovaW.but.canc -text Quit -command {
		$canv delete trovato_item
		destroy .trovaW
		return
	}
pack  .trovaW.but -side bottom -pady 3m -padx 2m
	pack .trovaW.lab -side top -pady 3m -padx 2m
        pack .trovaW.ent
        pack .trovaW.result
	pack .trovaW.but.find .trovaW.but.next .trovaW.but.canc -side left -padx 6m -fill x


}

set blo_name_punt -1
proc trova_item { canv nome next } {
# se next 1 trova il successivo
	global blo_name_punt 
	if { $next == 0 } { set blo_name_punt -1 }
	   set ii -1
	   set trovato 0
	   foreach item [$canv find withtag module] {
	    incr ii
            set blo_ite $item
            set blo_nam [file rootname [lindex [$canv gettags $item] [lsearch [$canv gettags $item] *.name]]]
#	    puts "ii=$ii nome=$nome ---- blo_name_punt=$blo_name_punt"
#            puts "$item - $blo_nam [$canv coords $item] ->test=[string first $nome $blo_nam]"

            if {[string first $nome $blo_nam] > -1} {
            	if { $ii >  $blo_name_punt } {
            	set blo_name_punt $ii
            	set trovato 1
#            	puts "------->[$canv coords $item]"
            	set lc [$canv bbox $item] 
            	set x1 [lindex $lc 0]
		set y1 [lindex $lc 1]
		set x2 [lindex $lc 2]
		set y2 [lindex $lc 3]
#		$canv create rectangle $x1 $y1 $x2 $y2 -fill yellow -tags { trovato_item }
		$canv create rectangle $x1 $y1 $x2 $y2 -width 5 -outline red -dash . -tags { trovato_item }
		
                set xwidth [$canv cget -width ]
                set yheight  [$canv cget -height ]
                
                set xmax [lindex [$canv cget -scrollregion ] 2]
                set ymax [lindex [$canv cget -scrollregion ] 3]
                set xnew [expr {[ expr $x1*1.0 - 0.5*$xwidth]/$xmax}]
                set ynew [expr {[ expr $y1*1.0 - 0.5*$yheight]/$ymax}]
                
                if { $xnew < 0 } { set xnew 0}
                if { $ynew < 0 } { set ynew 0}
                
#puts "x1=$x1 y1=$y1  \n xmax=$xmax ymax=$ymax \n xnew=$xnew ynew=$ynew"
                
                $canv xview moveto $xnew
                $canv yview moveto $ynew
                
                break
                }
            	
            }
         }
return $trovato       
}
