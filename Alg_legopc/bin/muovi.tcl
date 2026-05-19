proc topRead {c model} {
	global envir
	global env curFileName ff_progNumb fromfile progNumb modified numBloc
	global wsXsiz wsYsiz wsXmin wsYmin wsWidth wsHeight GIForient
	set progNumb 0
#set dovesono [pwd]  
#tk_messageBox -message "topRead 1: dovesono1:$dovesono curFileName$curFileName model:$model"  
#tk_messageBox -message "qq" -type ok
	if {$curFileName == "-"} {return}

	#   Type names		Extension(s)	Mac File Type(s)
	set types {
		{"Topology files"		{.tom}	}
		{"All files"		*}
	}
	if {$model == ""} {
		set curFileNametemp ""	        
		set curFileNametemp [tk_getOpenFile -filetypes $types -parent $c -initialdir $env(LG_MODELS)]
		if { $curFileNametemp == "" } { return 2 }
		set curFileName $curFileNametemp
	} else {
		set curFileName [file join $env(LG_MODELS) $model $model.tom]
		if {![file exists $curFileName]} {
			tk_messageBox -message "TopRead: 1 - File $curFileName not found...curFileName=$curFileName" -type ok
			set curFileName ""
		}
	}

	if {$curFileName == ""} {set curFileName untitled}

	$c delete all
	wm title . "$envir - $curFileName"
	set numBloc 0

	if {$curFileName == "untitled"} {return}

	set fileid [open $curFileName r]
	gets $fileid aline ; # first line (comment)

	# read and sets canvas size
	gets $fileid mcoords
	scan $mcoords "%s%s" wsXsiz wsYsiz
	set wsXmin $wsWidth
	set wsYmin $wsHeight
	setCanSiz $c	

	# first module pass with creation
	global mclass
	gets $fileid mclass
#tk_messageBox -message "topRead 2: prima di while"
	while {$mclass != "****"} {
            gets $fileid GIForient
		gets $fileid mname
		gets $fileid mcoords
		scan $mcoords "%s%s" x y
		gets $fileid mlpath_lungo ;    # GUAG - Lettura del path delle librerie grafiche dei moduli
		
		set nome_file [ file tail  $mlpath_lungo]
		set mlpath [ file join $env(LG_LIBRARIES) $nome_file]
#		tk_messageBox -message "fileio.tcl: File $mlpath giusto? dovrebbe $env(LG_LIBRARIES)" -type ok

		checkImage $mclass $mlpath  ; # loads image if not yet done...

		set ff_progNumb $mname
		set fromfile "yes"
            set idclass $mclass
		set result [source [file join $mlpath $mclass.tcl]]
		gets $fileid mclass
	}
#tk_messageBox -message "topRead 3: prima secindo passaggio"
	# second module pass with link connection
	gets $fileid mclass
	while {$mclass != "****"} {
		gets $fileid mname
		gets $fileid portx
		while {$portx != "++++"} {
			gets $fileid pstatus
			if {$pstatus != "free"} {
				scan $pstatus "%s%s%s" dummy cport cmod
				ffconnect $c $mname $portx $cmod $cport
			}
			gets $fileid portx
		}
		gets $fileid mclass
	}

	close $fileid
	set modified 0
        if {$curFileName != "untitled" && $envir != "Draw2Gr" && $envir != "Edit_Simul" && $envir != "PostProc" } {
           .menu.file entryconfigure 4 -state normal
        }
#tk_messageBox -message "topRead 4: fine"
}
