
proc writeModule {w c way} {
	global env curModName moduleX moduleY mlib
	global connlist extname idconn clines cvardef
if  { $::tcl_platform(os) != "Linux" } {
  set env(TMPDIR) $env(LG_PREINST)
}
	set defname [string trim [string tolower $curModName]].tcl
	if $way {
		#            Type names			Extension(s)
		set types {{"Preinited files"		{.tcl}}
			     {"All files"			*}}
		set rootfname [file rootname [tk_getSaveFile -filetypes $types \
			-parent $c -initialdir $mlib -initialfile $defname]]
		if {$rootfname == ""} {return}
	} else {
            set rootfname [file rootname [file join $mlib $defname]]
	}
	set curFileName $rootfname.tcl
#set mex "w=$w c=$c way=$way curFileName=$curFileName"
#tk_messageBox -icon warning -message $mex 
	set fileid [checkopen $curFileName w]
	puts $fileid "# this file created with PREINST rel. 0.3 "
	puts $fileid ""

	# module section
	set lcurModName [string trimright [string tolower $curModName]]
	puts $fileid "set mymodId \[\$c create image \$x \$y -image $lcurModName\$GIForient\]"
	puts $fileid {$c addtag id$mymodId withtag $mymodId}
	puts $fileid {$c addtag module withtag $mymodId}
	puts $fileid {$c addtag $idclass.cls withtag $mymodId}
        puts $fileid {$c addtag $GIForient.ori withtag $mymodId}
	puts $fileid "if {\$fromfile == \"yes\"} {"
	puts $fileid {	$c addtag $mlpath.lpath withtag $mymodId}
	puts $fileid "} else {"
	puts $fileid {	$c addtag $curLibPath.lpath withtag $mymodId}
	puts $fileid "}"
	puts $fileid "# let the user choose a name (Id)"
	puts $fileid "if {\$fromfile == \"yes\"} then {set progName \$ff_progNumb} else {inputModName \$c \$x \$y}"
	puts $fileid {$c addtag $progName.name withtag $mymodId}
	puts $fileid "incr progNumb"

	# port(s) section
	foreach item [$c find withtag phconn] {
		puts $fileid ""

		# retrieve port infos
		set tmage [$c itemcget $item -image]
            set ori1 [string range $tmage 4 4]
            switch $ori1 {
            "n" {set ori2 "e"; set ori3 "s"; set ori4 "w"}
            "e" {set ori2 "s"; set ori3 "w"; set ori4 "n"}
            "s" {set ori2 "w"; set ori3 "n"; set ori4 "e"}
            "w" {set ori2 "n"; set ori3 "e"; set ori4 "s"}
            }

		set phImage1 [string range $tmage 0 3]_$ori1
 		set phImage2 [string range $tmage 0 3]_$ori2
		set phImage3 [string range $tmage 0 3]_$ori3
            set phImage4 [string range $tmage 0 3]_$ori4

     		set phType [string range [$c itemcget $item -image] 0 3]_ptype
		if {[lsearch -exact [$c gettags $item] input] != -1} {set phDir input} else {set phDir output}
		if {[lsearch -exact [$c gettags $item] input] != -1} {
			set phAnchor [string tolower [string index [$c itemcget $item -image] 4]]
		} else {
			set phAnchor [string tolower [oppdir [string index [$c itemcget $item -image] 4]]]
		}
		set phNumber [string range [lindex [$c gettags $item] 1] 2 end]
		set lcoord [$c coords $item]
		set halfwidth [expr ([lindex [$c bbox $item] 2] - [lindex [$c bbox $item] 0])/4]
		set halfheight [expr ([lindex [$c bbox $item] 3] - [lindex [$c bbox $item] 1])/4]
		switch $phAnchor {
			n {set nudgex 0; set nudgey [expr -$halfheight]; set anc2 "e"; set anc3 "s"; set anc4 "w"}
			s {set nudgex 0; set nudgey $halfheight; set anc2 "w"; set anc3 "n"; set anc4 "e"}
			e {set nudgex $halfwidth; set nudgey 0; set anc2 "s"; set anc3 "w"; set anc4 "n"}
			w {set nudgex [expr -$halfwidth]; set nudgey 0; set anc2 "n"; set anc3 "e"; set anc4 "s"}
		}
		set shiftx1 [expr [lindex $lcoord 0]/2 - $moduleX/2 + $nudgex]
		set shifty1 [expr [lindex $lcoord 1]/2 - $moduleY/2 + $nudgey]
		set shiftx2 [expr -$shifty1]; set shifty2 $shiftx1
            set shiftx3 [expr -$shiftx1]; set shifty3 [expr -$shifty1]
            set shiftx4 $shifty1; set shifty4 [expr -$shiftx1]

		# parte aggiunta per la gestione delle var. matematiche del connettore da legofi
		if {[lsearch -exact [$c gettags $item] input] != -1} {set ct I} else {set ct O}
		set ctype [string range [$c itemcget $item -image] 0 3]${ct}Fvar
		set pcid [string range [lindex [$c gettags $item] 1] 2 end]
		set connarr $ctype$pcid
		global $connarr
		set phcMlist {}

		set tlist {}
		foreach {key value} [array get $connarr {??[a-z]*}] {
			set stlist {}
			lappend stlist $key
			lappend stlist $value
			lappend tlist $stlist
		}
		set tlist [lsort -index 0 $tlist]
		foreach {keyvalue} $tlist {
			set value [lindex $keyvalue 1]
			lappend phcMlist $value
		}


		# "puts" here...
            puts $fileid "switch \$GIForient {"
		puts $fileid "n {set myportId \[\$c create image \[expr \$x + $shiftx1\] \[expr \$y + $shifty1\] -image $phImage1 -anchor $phAnchor\]}"
		puts $fileid "e {set myportId \[\$c create image \[expr \$x + $shiftx2\] \[expr \$y + $shifty2\] -image $phImage2 -anchor $anc2\]}"		
		puts $fileid "s {set myportId \[\$c create image \[expr \$x + $shiftx3\] \[expr \$y + $shifty3\] -image $phImage3 -anchor $anc3\]}"		
		puts $fileid "w {set myportId \[\$c create image \[expr \$x + $shiftx4\] \[expr \$y + $shifty4\] -image $phImage4 -anchor $anc4\]}"
		puts $fileid "}"
		puts $fileid {$c addtag id$myportId withtag $myportId}
		puts $fileid {$c addtag id$mymodId withtag $myportId}
		puts $fileid "\$c addtag port$phNumber withtag \$myportId"
		puts $fileid {$c addtag port withtag $myportId}
		puts $fileid "\$c addtag $phType withtag \$myportId; #port type"
		puts $fileid "\$c addtag $phDir withtag \$myportId; #port direction"
		puts $fileid "global port$phNumber.\$mymodId"
		puts $fileid "set port$phNumber.\$mymodId \{ $phcMlist \}"
	}

puts $fileid ""
puts $fileid "set var1_anim $::var1_anim"
puts $fileid "\$c addtag \$var1_anim.anim withtag \$mymodId"
# guag feb 2008 - aggiungo la tag per i moduli di tipo REGOL.	
	if { $::tiporeg == 1 } { puts $fileid {$c addtag tiporeg withtag $mymodId} }	

	close $fileid

	# .pi4 creation - needed for compatibility with pag2f01 - connectors info "hard-wired"
	set hardwiredInfo {}
	foreach i $connlist {
		lappend hardwiredInfo [list $extname($i) "input"]
		lappend hardwiredInfo [list $extname($i) "output"]
	}
	set curFileName $rootfname.pi4
#puts stdout $curFileName
	file copy -force $rootfname.pi3 $curFileName

	set fileid [checkopen $curFileName a]
	set phclist [$c find withtag phconn]
	puts $fileid [llength $phclist]

	foreach item $phclist {
		set phcType [$c itemcget $item -image]
		if {[lsearch -exact [$c gettags $item] input] != -1} {
			set tycon [string range $phcType 0 3]
			set phcNum $idconn($tycon)
		} else {
			set tycon [string range $phcType 0 3]
			set phcNum [expr $idconn($tycon) + 1]
		}		
		set phDir t[string range [lindex [$c gettags $item] 1] 2 end]	
		if {[string length $phDir] > 2} {set phDir [string range $phDir 1 end]}	
 
		# which connector type?
		if {[lsearch -exact [$c gettags $item] input] != -1} {set ct I} else {set ct O}
		set ctype [string range [$c itemcget $item -image] 0 3]${ct}Fvar
		set pcid [string range [lindex [$c gettags $item] 1] 2 end]
		set connarr $ctype$pcid
		global $connarr

		puts $fileid "$phDir $phcNum"
		puts $fileid [lindex $hardwiredInfo $phcNum]
		set phcMlist {}

		set tlist {}
		foreach {key value} [array get $connarr {??[a-z]*}] {
			set stlist {}
			lappend stlist $key
			lappend stlist $value
			lappend tlist $stlist
		}
		set tlist [lsort -index 0 $tlist]
		foreach {keyvalue} $tlist {
			set value [lindex $keyvalue 1]
			lappend phcMlist $value
		}
		puts $fileid $phcMlist

	}

	close $fileid

# lancio di i32i5 per convertire il .pi4 al formato .I5

#puts stdout [pwd]
	set toolsPath $env(LG_TOOLS)
	set olddir [pwd]
	cd $env(TMPDIR)
#tk_messageBox -message "writeModule: sono nella dir=$env(TMPDIR)\n presave: rootfname=$rootfname" -type ok
	exec $env(LG_TOOLS)/i32i5 $rootfname.pi4
	set curFileName [file tail $rootfname].i5
#	file rename -force $env(LG_TOOLS)/$curFileName $env(LG_FILESI5)/$curFileName
	file rename -force $env(TMPDIR)/$curFileName $env(LG_FILESI5)/$curFileName

# cancellazione del .pi4
	#file delete $rootfname.pi4

# creazione del .tch
        cd $env(LG_FILESI5)
        set curFileName [file tail $rootfname].i5
        set fileid [checkopen $curFileName r]
        gets $fileid line
        gets $fileid line
        gets $fileid line
        close $fileid
	cd $env(LG_HELP)
	set curFileName [file tail $rootfname].tch
	set fileid [checkopen $curFileName w]
	puts $fileid "[file tail $rootfname]"
	puts $fileid $line
	close $fileid

## creazione del .xbm (versione bianco e nero dell'icona per la stampa   
# tolti gli xbm sono inutili
#	cd $env(LG_TOOLS)
#      set puca {n e s w}
#      foreach i $puca {
#	 set curFileName [file tail $rootfname]$i.xbm
#	 set tempGIF [file tail $rootfname]$i.gif
#       if [file exists ${rootfname}${i}.gif] {
#	  file copy -force ${rootfname}${i}.gif $env(LG_TOOLS)
#        exec forcedos alchemy.bat $tempGIF $curFileName
#	  file rename -force $curFileName $env(LG_XBM)/$curFileName
#	  file delete $tempGIF
#       }
#      }
	cd $olddir
}
