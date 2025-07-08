
proc leggi_font {c  mymodId  modo newfont} {
# modo 1 : lettura da canvas 
# modo 2 : scrittura su canvas
        set myTags [$c gettags $mymodId]
	set mylist [lindex $myTags 7].[string range [lindex $myTags 0] 2 end]
	global $mylist
## set si vuole cambiare: set $mylist {qqqq 111}	
        if { $modo == 2 } { set $mylist $newfont }
	set mvlist [join [set $mylist] " "]
	set ff  [set $mylist] 
# ff �una varibile lista come font.$mymodId e ha lo stesso comtenuto
        set fff [lindex $ff 0]
        set ccc [lindex $ff 1]
#debug    tk_messageBox -message "leggi_font: mod=$modo \nmymodId=$mymodId\n myTags=$myTags=\n mylist=$mylist\n mvlist=$mvlist=\n ff=$ff \nfont=$fff carattere=$ccc\n"
#set fret [list $fff $ccc] 

return $ff
#return $fret
}
proc topWrite {c mode} {
	global envir returnvalue
	global env curFileName modified DIRMODEL f14File
	global wsXsiz wsYsiz
#tk_messageBox -icon error -message "topWrite: $curFileName"
	if {$mode == "saveas"} {
	        if {$curFileName != "untitled" &&  $curFileName != "-" && $curFileName != "" } { 

#	          tk_messageBox -icon error -message "No model to save as" -type ok
#	          return 
		#   Type names		Extension(s)	Mac File Type(s)
		set types {
			{"Model topology"		{.tom}	}
			{"All files"		*}
		}
                set selPath [tk_getSaveFile -title "Save topology file .tom as..." -filetypes $types -parent $c -initialdir $env(LG_MODELS)]
		if {$selPath == ""} {return 1}
		set curFileName [file rootname $selPath].tom
		puts "topWrite: $curFileName"
        	cd [ file dirname $curFileName ]
		if {$curFileName == ".tom"} {return 1}
		if {[writeFiles $c $curFileName $envir]} { return 1}
		return 0
#		set curFileName [file rootname $selPath].tom
#		if {$curFileName == ".tom"} {return}
#		writeFiles $c $curFileName $envir
#	        return 
	       }
	}
	if {$curFileName == "untitled" || $curFileName == "-" || $curFileName == "" } {
#tk_messageBox -icon error -message "topWrite: if "
		catch { destroy  $c.mod }
		toplevel $c.mod
		wm title $c.mod "ModelName Selection"
		grab $c.mod
		label $c.mod.lab -text "Model name:" -font "Courier 12"
		entry $c.mod.ent -textvariable modelName
		frame $c.mod.but
		button $c.mod.but.ok -text OK -command {
                   if [file isdirectory $env(LG_MODELS)/$modelName] {
                      tk_messageBox -icon info -message "model already exists!" \
                                    -type ok -parent .
			 } elseif {[string length $modelName] > 8} {
                     tk_messageBox -icon warning -message "Model name too long (max 8 char.)" \
                                    -type ok -parent .
               
                   } else {
			    file mkdir $env(LG_MODELS)/$modelName
			    set curFileName $env(LG_MODELS)/${modelName}/${modelName}.tom
			set DIRMODEL [ file dirname $curFileName ]
			set f14File $DIRMODEL/f14.dat
			cd $DIRMODEL

                      destroy $c.mod
		      if {[writeFiles $c $curFileName $envir]} {
		        global returnvalue
		      	set returnvalue 1
		      } else {
		        global returnvalue
		      	set returnvalue 0
		      }
#		          writeFiles $c $curFileName $envir
#tk_messageBox -icon error -message "topWrite: $DIRMODEL $f14File $curFileName"	
                   }
		}
		button $c.mod.but.canc -text Canc -command {
			global returnvalue
			destroy $c.mod
			set returnvalue 1
#                       return
		}
		pack $c.mod.but.ok $c.mod.but.canc -side left -padx 2m -fill x
		pack $c.mod.lab $c.mod.ent $c.mod.but -side top -pady 3m -padx 2m

		vwait returnvalue
		return $returnvalue

	} else {
#tk_messageBox -icon error -message "topWrite: else  modified $modified"		
		if {[writeFiles $c $curFileName $envir]} { return 1}
	}	
    return 0
} 

proc writeFiles {c curFileName envir} {

	   global wsXsiz wsYsiz
	   global modified
	   global connlist extname idconn clines cvardef

 	   wm title . "$envir - $curFileName"
#tk_messageBox -icon error -message "writeFiles: modified $modified"	
if {$modified == 0 } { return 0}   
#tk_messageBox -icon error -message "writeFiles: dopo modified"	
	   if { [catch {open $curFileName w} fileid]} {
		tk_messageBox -icon error -message "Can not open file $curFileName for writing"
		return 1
	   }
	   puts $fileid "# this file created with LEGOPHI rel. 0.2"

   	   # write canvas size
	   set attdim [$c cget -scrollregion]
	   set wsXsiz [lindex $attdim 2]
	   set wsYsiz [lindex $attdim 3]
# puts "writeFiles: $wsXsiz $wsYsiz"
	   puts $fileid "$wsXsiz $wsYsiz"

         set nelem 0
# trovo i moduli di tipo regol e li metto in cima alla lista ...
	   foreach item [$c find withtag tiporeg] {
            set blo_ite($nelem) $item
            set blo_nam($nelem) [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.name]]]
#puts "writeFiles: $item è un REGOL. nelem=$nelem  blo_nam=$blo_nam($nelem)"
            
	    incr nelem
        }	    
	   foreach item [$c find withtag module] { 
	   if {[lsearch [$c gettags $item] tiporeg] == -1} {
            set blo_ite($nelem) $item
            set blo_nam($nelem) [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.name]]]
#puts "writeFiles: $item è un MODULO nelem=$nelem  blo_nam=$blo_nam($nelem)"
            
	    incr nelem
	   }
         }

#         for {set i 0} {$i < $nelem} {incr i} {
#            for {set j [expr $nelem-1]} {$j > $i} {set j [expr $j-1]} {
#               set k [expr $j-1]
#               if {$blo_nam($k) > $blo_nam($j)} {
#                  set tmp_ite $blo_ite($k)
#                  set blo_ite($k) $blo_ite($j)
#                  set blo_ite($j) $tmp_ite
#                  set tmp_nam $blo_nam($k)
#                  set blo_nam($k) $blo_nam($j)
#                  set blo_nam($j) $tmp_nam
#               }
#            }
#         }

	   # first module pass
	   for {set i 0} {$i < $nelem} {incr i} {
            set item $blo_ite($i)
            # mantiene la compatibilit�con i moduli preistanziati senza orientamento
            if {[file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.ori]]] == ""} {
             puts $fileid [$c itemcget $item -image]
             puts $fileid "n"
            } else {    
		 puts $fileid [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.cls]]]
             puts $fileid [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.ori]]]
		}
		puts $fileid [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.name]]]
		puts $fileid [$c coords $item]
		puts $fileid [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.lpath]]]
	   }
	   puts $fileid "****"

	   # second module pass
	   for {set i 0} {$i < $nelem} {incr i} {
            set item $blo_ite($i)
		puts $fileid [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.cls]]]
		puts $fileid [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.name]]]
		set myid [lindex [$c gettags $item] [lsearch [$c gettags $item] id*]]
		foreach port [$c find withtag $myid] {
			if {[lsearch [$c gettags $port] module] != "-1"} {continue}
			puts $fileid [lindex [$c gettags $port] [lsearch [$c gettags $port] port?*]]
			if {[lsearch [$c gettags $port] busy] != "-1"} {
				set cptag [lindex [split [lindex [$c gettags $port] [lsearch [$c gettags $port] connwith.*]] . ] end ]
				set portx [lindex [$c gettags $cptag] [lsearch [$c gettags $cptag] port?*]]
				foreach modorport [$c find withtag [lindex [$c gettags $cptag] 1]] {
					if {[lsearch -exact [$c gettags $modorport] module] != "-1"} {
						set cpmod [file rootname [lindex [$c gettags $modorport] [lsearch [$c gettags $modorport] *.name]]]
					}
				}
				puts $fileid "busy $portx $cpmod"
			} else {
				puts $fileid free
			}
		}
#Guag 16/05/2007 
                if {[lsearch [$c gettags $item] remarkdescr] != -1} {
#                	set qqqq [lsearch -inline [$c gettags $item] remarkdescr]
                	set testo_attuale [$c itemcget $item -text ]
##debug tk_messageBox -message "writeFiles:item=$item\n testo_attuale=$testo_attuale=\n font=$::xfont"
#
## font ...
#			puts $fileid $::xfont
                        puts $fileid [leggi_font $c $item 1 "qq"] 
#			puts $fileid $::xfont
                	puts $fileid $testo_attuale
                }

#		set mdescr [lindex [$c gettags $item] [lsearch [$c gettags $item] remarkdescr]]
#		foreach descr [$c find withtag $mdescr] {
#tk_messageBox -message "writeFiles:item=$item\n descr=$descr=\n mdescr=$mdescr="
#	             set vecchio_testo [$c itemcget $descr -text ]
#		     if { $descr != "" } {puts $fileid $vecchio_testo }
#		}
##
		puts $fileid "++++"
	   }
	   puts $fileid "****"

	   close $fileid

	   #save also ".top" file, used to build F01

	   set topFileName [file rootname $curFileName].top
	   if { [catch {open $topFileName w} fileid]} {
		tk_messageBox -icon error -message "Can not open file $topFileName for writing"
		return 1
	   }
	   for {set i 0} {$i < $nelem} {incr i} {
            set item $blo_ite($i)
# guag 4/2007- ignoro i moduli con il nome che inizia per @
            set strstr [string toupper [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.cls]]]]
	    set nonmodulo [regexp {@} $strstr]
#tk_messageBox -message "writeFiles: item=$item strsstr=$strstr nonmodulo=$nonmodulo" -type ok
	    if { $nonmodulo == 0 } { 
            if {[file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.ori]]] == ""} {
             puts $fileid [string toupper [$c itemcget $item -image]]
            } else {
		 puts $fileid $strstr
		}
		puts $fileid [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.name]]]
		puts $fileid ""
	    }
	   }
	   puts $fileid "****"

	   # second module pass
	   for {set i 0} {$i < $nelem} {incr i} {
            set item $blo_ite($i)
# guag 4/2007- ignoro i moduli con il nome che inizia per @
            set strstr [string toupper [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.cls]]]]
	    set nonmodulo [regexp {@} $strstr]
#tk_messageBox -message "writeFiles 2: item=$item strsstr=$strstr nonmodulo=$nonmodulo" -type ok
	    if { $nonmodulo == 0 } {
            if {[file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.ori]]] == ""} {
             puts $fileid [string toupper [$c itemcget $item -image]]
            } else {
		 puts $fileid $strstr
		}
		puts $fileid [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.name]]]
		set myid [lindex [$c gettags $item] [lsearch [$c gettags $item] id*]]
		foreach port [$c find withtag $myid] {
			if {[lsearch [$c gettags $port] module] != "-1"} {continue}

			set port1 [lindex [$c gettags $port] [lsearch [$c gettags $port] port?*]]
			set t1 [string range $port1 [expr [string length $port1]-2] end]
			set ptype1 [lindex [$c gettags $port] [lsearch [$c gettags $port] *_ptype]]

			if {[lsearch [$c gettags $port] output] != "-1"} {
				set tycon [string range $ptype1 0 3]
				set phcNum [expr $idconn($tycon) + 1]
				puts $fileid "$t1 $phcNum"
			} elseif {[lsearch [$c gettags $port] input] != "-1"} {
				set tycon [string range $ptype1 0 3]
				set phcNum $idconn($tycon)

				if {[lsearch [$c gettags $port] busy] != "-1"} {
					set cptag [lindex [split [lindex [$c gettags $port] [lsearch [$c gettags $port] connwith.*]] . ] end ]
					set port2 [lindex [$c gettags $cptag] [lsearch [$c gettags $cptag] port?*]]
					set t2 [string range $port2 [expr [string length $port2]-2] end]
					foreach modorport [$c find withtag [lindex [$c gettags $cptag] 1]] {
						if {[lsearch -exact [$c gettags $modorport] module] != "-1"} {
							set cpmod [file rootname [lindex [$c gettags $modorport] [lsearch [$c gettags $modorport] *.name]]]
						}
					}
					puts $fileid "$t1 $phcNum $t2 $cpmod"
				} else {
					puts $fileid "$t1 $phcNum __"
				}
			}
		   }
		   puts $fileid "****"
	   }
	   }

	   close $fileid
	   set modified 0
return 0
}


proc topRead {c model} {
	global envir
	global env curFileName ff_progNumb fromfile progNumb modified numBloc
	global wsXsiz wsYsiz wsXmin wsYmin wsWidth wsHeight GIForient
	set progNumb 0
#set dovesono [pwd]  
#tk_messageBox -message "topRead 1: dovesono1:$dovesono curFileName$curFileName model:$model"  
#tk_messageBox -message "qq" -type ok
	if {$curFileName == "-"} {return 1}

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

	if {$curFileName == "untitled"} {return 1}


	if {[catch {open $curFileName r} fileid]} {
		tk_messageBox -type ok -icon error  -message "can not open file $curFileName for reading"
		return 1
	}

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
#		tk_messageBox -message "fileio.tcl: prima di include tcl"
		set result [source [file join $mlpath $mclass.tcl]]
#		tk_messageBox -message "fileio.tcl: dopo di include tcl"
		gets $fileid mclass
	}
#tk_messageBox -message "topRead 3: prima secindo passaggio"
	# second module pass with link connection
	gets $fileid mclass
	while {$mclass != "****"} {
## Guag 17/5/2007 da modifi�are per i remark, verificare che �un remark ( cio�se c'�@ in mclass) 
##e comportarsi di conseguenza, senza cercare connessioni se no d�errore
		gets $fileid mname
		if { [regexp {@} $mclass] == 1} {
# �un elemento di tipo remark			
			set item [$c find withtag $mname.name ] 
			gets $fileid fontbuff			
			gets $fileid testo_linea
			if { $testo_linea != "++++"} { 
			   set testo_dafile $testo_linea
#GUAG - nov 2007 - test su presenza di nome di variabile da animare che inizia per #
			   $c dtag $item da_animare
			   if { [regexp {^#} $testo_dafile] == 1} { 
			   	set var_animaz [string range $testo_dafile 1 8]
#			   	tk_messageBox -message "topRead 4: var_animaz=$var_animaz" 
#puts "topRead 4: variabile che inizia per cancelletto: var_animaz=$var_animaz item=$item"  
				$c addtag $var_animaz.anim withtag $item  
                                $c addtag da_animare withtag $item
			   	}
			 }
			
			gets $fileid testo_linea
			while {$testo_linea != "++++"} {
				append testo_dafile "\n" $testo_linea
				gets $fileid testo_linea
			}
#			gets $fileid dummy
#tk_messageBox -message "topRead 3: testo_dafile=$testo_dafile"
####			set item [$c find withtag $mname.name ] 
#tk_messageBox -message "topRead 3: item=$item   mname=$mname"
                        leggi_font $c $item 2 $fontbuff
#tk_messageBox -message "topRead 3: item=$item fontbuff=$fontbuff"
                        set ff [lindex $fontbuff 0]
                        set cc [lindex $fontbuff 1]
                        
                        catch [set colo [lindex $fontbuff 2]]
                        if { $colo == "" } { set colo "black"}
			$c itemconfigure $item -text $testo_dafile -font [list $ff $cc] -fill $colo
#tk_messageBox -message "topRead 4: item=$item fontbuff=$fontbuff\n $ff $cc $colo"

#			$c itemconfigure $item -text $testo_dafile -font $::xfont
		} else { 
			gets $fileid portx
			while {$portx != "++++"} {
				gets $fileid pstatus
				if {$pstatus != "free"} {
					scan $pstatus "%s%s%s" dummy cport cmod
#debug tk_messageBox -message "topread. mname=$mname portx=$portx cmod=$cmod cport=$cport"
					ffconnect $c $mname $portx $cmod $cport
				}
				gets $fileid portx
			}
		}
		gets $fileid mclass
	}

	close $fileid
	set modified 0
        if {$curFileName != "untitled" && $envir != "Draw2Gr" && $envir != "Edit_Simul" && $envir != "PostProc" } {
#GUAG: non so perch�c'�questo comando.. lo tolgo
#           .menu.file entryconfigure 4 -state normal
        }
        
        return 0
        
#tk_messageBox -message "topRead 4: fine"
}


proc checkImage {name path} {
      global GIForient
#     check per l'icona senza orientazioni
      set filen [file join $path ${name}n.gif]
      set filee [file join $path ${name}e.gif]
      set files [file join $path ${name}s.gif]
      set filew [file join $path ${name}n.gif]      
      set fileori [file join $path $name$GIForient.gif]      
      if [file exists $fileori] {
	 set chk [catch {set result [image type $name$GIForient]} retval]
	 if {$chk != 0} {
        image create photo ${name}n -file [file join $path ${name}n.gif]
        if {[file exists $filen] && [file exists $filee] && [file exists $files] && [file exists $filew]} {
         image create photo ${name}e -file [file join $path ${name}e.gif]
         image create photo ${name}s -file [file join $path ${name}s.gif]
         image create photo ${name}w -file [file join $path ${name}w.gif]
        }
       }
	} else {
	 set chk [catch {set result [image type $name]} retval]
	 if {$chk != 0} {image create photo $name -file [file join $path $name.gif]}	               
	}
}


proc ffconnect {c modfrom portfrom modto portto} {
	global connlist extname idconn clines cvardef ffcurrent ff2current
	# retrieve real portfrom id
	set modid [lindex [$c gettags [$c find withtag $modfrom.name]] 0]
	foreach item [$c find withtag $modid] {
		if {[lsearch [$c gettags $item] $portfrom] != "-1"} {set ffcurrent $item}
	}

	# retrieve real portto id
	set modid [lindex [$c gettags [$c find withtag $modto.name]] 0]
	foreach item [$c find withtag $modid] {
		if {[lsearch [$c gettags $item] $portto] != "-1"} {set ff2current $item}
	}

	# if connection is "inverted", swap ports
	if {[lsearch -exact [$c gettags $ffcurrent] output] == "-1"} {
		set temp $ffcurrent
		set ffcurrent $ff2current
		set ff2current $temp
	}

	# if portto is busy, forget it (it has already been connected
	if {[lsearch -exact [$c gettags $ff2current] busy] != "-1"} {return}

	# this portion "copied" from portSelect phase 1
	set sPortId [lindex [$c gettags $ffcurrent] 0]
	set sPortType [lindex [$c gettags $ffcurrent] [lsearch [$c gettags $ffcurrent] *ptype]]
	set anchor [$c itemcget $sPortId -anchor]
	switch $anchor {
		n {set sPortDir s}
		e {set sPortDir w}
		s {set sPortDir n}
		w {set sPortDir e}
	}

	# this portion "copied" from portSelect phase 2
	set ePortId [lindex [$c gettags $ff2current] 0]
	set linktag link$sPortId.$ePortId
	set anchor [$c itemcget $ePortId -anchor]
	switch $anchor {
		n {set ePortDir s}
		e {set ePortDir w}
		s {set ePortDir n}
		w {set ePortDir e}
	}
	set tycon [string range $sPortType 0 3]
	portJoin $linktag $c $sPortId $sPortDir $ePortId $ePortDir $tycon
	set revlinktag link$ePortId.$sPortId
	$c addtag $revlinktag withtag $linktag
	$c itemconfigure $linktag -fill $clines($tycon,color)
	$c addtag ${tycon}_ltype withtag $linktag
	$c addtag busy withtag $sPortId
	$c addtag busy withtag $ePortId
	$c addtag connwith.$ePortId withtag $sPortId
	$c addtag connwith.$sPortId withtag $ePortId
}


proc sizeCanv {c what} {
	global wsWidth wsHeight wsXsiz wsYsiz wsXmin wsYmin
	set attdim [$c cget -scrollregion]
	set wsXsiz [lindex $attdim 2]
	set wsYsiz [lindex $attdim 3]
	$c addtag tutto all
	set mindim [$c bbox tutto]
	$c dtag tutto
	if {$mindim == {}} {
		set wsXmin 0
            set wsYmin 0
	} else {
		set wsXmin [lindex $mindim 2]
		set wsYmin [lindex $mindim 3]
	}
	if {$wsXmin < $wsWidth} {set wsXmin $wsWidth}
	if {$wsYmin < $wsHeight} {set wsYmin $wsHeight}

	toplevel .cansize
	wm title .cansize "Canvas size"
	grab .cansize

	label .cansize.title -font titleFont -text "Set canvas size"
	pack .cansize.title -side top -padx 20 -pady 10
	if {$what == "X" || $what == "both"} {
		frame .cansize.x
		pack .cansize.x -side top -padx 20 -pady 10
		label .cansize.x.tit -font helpFont -text "Width (min = $wsXmin)"	
		entry .cansize.x.ent -font entryFont -textvariable wsXsiz -width 6
		pack .cansize.x.tit .cansize.x.ent -side left -padx 5

	}
	if {$what == "Y" || $what == "both"} {
		frame .cansize.y
		pack .cansize.y -side top -padx 10 -pady 10
		label .cansize.y.tit -font helpFont -text "Height (min = $wsYmin)"	
		entry .cansize.y.ent -font entryFont -textvariable wsYsiz -width 6
		pack .cansize.y.tit .cansize.y.ent -side left -padx 5
	}
	frame .cansize.b
	pack .cansize.b -side bottom -padx 20 -pady 10
	button .cansize.b.ok -text "Ok" -command "setCanSiz $c; destroy .cansize"
	button .cansize.b.can -text "Cancel" -command "destroy .cansize"
	pack .cansize.b.ok .cansize.b.can -side left -padx 20
}


proc setCanSiz {c} {
	global wsXsiz wsYsiz wsXmin wsYmin
	global modified
	if {$wsXsiz < $wsXmin} {set wsXsiz $wsXmin}
	if {$wsYsiz < $wsYmin} {set wsYsiz $wsYmin}
	$c configure -scrollregion [list 0 0 $wsXsiz $wsYsiz]
	set modified 1
}

