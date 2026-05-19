

set monList {}

button $fs.monitor -text "Configure Monitor variables..." -state normal -command p_monitor_var_conf
button $fs.util_monitor -text "Monitor" -command { p_monitor_var }


proc p_monitor_var_conf { } {
	global env activeModel  DIRMODEL f14File typev f14List   monList   
#      if {$activeModel != "(none)"} {
#       set DIRMODEL $env(LG_MODELS)/$activeModel
#       }
     if ![info exists DIRMODEL] {
         tk_messageBox -icon info \
                       -message "First select a model!" \
                       -type ok -parent .
     } else {
	catch {destroy .monvar}
        toplevel .monvar
        wm minsize .monvar 20 1
        wm title .monvar "Monitoring Variables Choice"
        wm iconname .monvar "MVCh"
        frame .monvar.qc
        frame .monvar.qc.frm1
        frame .monvar.qc.frm2
        frame .monvar.qc.frm3
        label .monvar.qc.frm1.lab1 -font "Times 14" -text "Non-monitored variables"
        pack .monvar.qc.frm1.lab1 -side top -pady 3m

        frame .monvar.qc.frm1.list1
        listbox .monvar.qc.frm1.list1.lbox -yscroll ".monvar.qc.frm1.list1.scrl set" -setgrid 1 \
                                  -background white -height 12 -width 10 -font "Courier 12"
        scrollbar .monvar.qc.frm1.list1.scrl -command ".monvar.qc.frm1.list1.lbox yview"
        frame .monvar.qc.frm3.list2
        listbox .monvar.qc.frm3.list2.lbox -yscroll ".monvar.qc.frm3.list2.scrl set" -setgrid 1 \
                                  -background white -height 12 -width 10 -font "Courier 12"
        scrollbar .monvar.qc.frm3.list2.scrl -command ".monvar.qc.frm3.list2.lbox yview"
        if ![file exists lg3.sel] {
           set f [checkopen $DIRMODEL/f14.dat r]
           for {set i 1} {$i < 6} {incr i} {
               gets $f line
           }
           while {[string index $line 24] == "*"} {
               set idf [string range $line 4 11] 
               .monvar.qc.frm1.list1.lbox insert end $idf
               gets $f line
               set typev($idf) "O"
           }
           gets $f line
           while {[string index $line 24] == "*"} {
               set idf [string range $line 4 11] 
               .monvar.qc.frm1.list1.lbox insert end $idf
               set typev($idf) "I"
               gets $f line
           }
           close $f
           set f14List [.monvar.qc.frm1.list1.lbox get 0 end]
        } else {
           set f [checkopen lg3.sel r]
           gets $f line
           while {[string compare $line "********"] != 0} {
                 set idf [string range $line 0 7]
                 .monvar.qc.frm1.list1.lbox insert end $idf
                 set typev($idf) [string range $line 9 9]
                 gets $f line
           }
           while {[gets $f line] >= 0} {
                 set idf [string range $line 0 7]
                 .monvar.qc.frm3.list2.lbox insert end $idf
		     set typev($idf) [string range $line 9 9]	
           }
           close $f
           set f14List {}
           set f [checkopen $DIRMODEL/f14.dat r]
           for {set i 1} {$i < 6} {incr i} {
               gets $f line
           }
           while {[string index $line 24] == "*"} {
               lappend f14List [string range $line 4 11]
               gets $f line
           }
           gets $f line
           while {[string index $line 24] == "*"} {
               lappend f14List [string range $line 4 11]
               gets $f line
           }
           close $f
        }
        pack .monvar.qc.frm1.list1.lbox .monvar.qc.frm1.list1.scrl \
             -anchor center -side left -fill y
        pack .monvar.qc.frm1.list1 -side top -pady 3m
        frame .monvar.qc.frm1.src -relief ridge -bd 1 -height 2
        radiobutton .monvar.qc.frm1.src.but
        entry .monvar.qc.frm1.src.entry -textvariable findVar
        label .monvar.qc.frm1.src.lab -text "Find" -font "Courier 12"
        pack .monvar.qc.frm1.src.but .monvar.qc.frm1.src.entry .monvar.qc.frm1.src.lab \
             -side left -padx 3m -pady 3m
        bind .monvar.qc.frm1.src.but <Button-1> {
             if ![info exists srcIndex] {set srcIndex ""}
             findItem .monvar.qc.frm1.list1.lbox srcIndex $findVar
        }
        pack .monvar.qc.frm1.src -side top -pady 3m
        button .monvar.qc.frm2.add -text "Add" -command {
                global typev f14List
               foreach i [.monvar.qc.frm1.list1.lbox curselection] {
                  set listEntry [.monvar.qc.frm1.list1.lbox get $i]
                  .monvar.qc.frm1.list1.lbox delete $i
                  set lista2 [.monvar.qc.frm3.list2.lbox get 0 end]
                  if {[llength $lista2] == 0} {
                     .monvar.qc.frm3.list2.lbox insert end $listEntry
                  } else {
                     set nn [lsearch $f14List $listEntry]
                     set found 0
                     while {! $found && $nn < [llength $f14List]} {
                           set indexList [lsearch $lista2 [lindex $f14List [incr nn]]]
                           if {$indexList >= 0} {
                              set found 1
                           }
                     }
                     if {! $found} {
                           .monvar.qc.frm3.list2.lbox insert end $listEntry
                     } else {
                           .monvar.qc.frm3.list2.lbox insert $indexList $listEntry
                     }
                  }
               }
        }

	button .monvar.qc.frm2.all -text "Add All" -command {
	        global typev f14List
               set lenglist [.monvar.qc.frm1.list1.lbox size] 
               for {set i 0} {$i < $lenglist} {incr i} {
                  set listEntry [.monvar.qc.frm1.list1.lbox get 0]
                  .monvar.qc.frm1.list1.lbox delete 0
                  set lista2 [.monvar.qc.frm3.list2.lbox get 0 end]
                  if {[llength $lista2] == 0} {
                     .monvar.qc.frm3.list2.lbox insert end $listEntry
                  } else {
                     set nn [lsearch $f14List $listEntry]
                     set found 0
                     while {! $found && $nn < [llength $f14List]} {
                           set indexList [lsearch $lista2 [lindex $f14List [incr nn]]]
                           if {$indexList >= 0} {
                              set found 1
                           }
                     }
                     if {! $found} {
                           .monvar.qc.frm3.list2.lbox insert end $listEntry
                     } else {
                           .monvar.qc.frm3.list2.lbox insert $indexList $listEntry
                     }
                  }
               }
        }


	button .monvar.qc.frm2.delall -text "Del All" -command {
	        global typev f14List
               set lenglist [.monvar.qc.frm3.list2.lbox size] 
               for {set i 0} {$i < $lenglist} {incr i} {
                  set listEntry [.monvar.qc.frm3.list2.lbox get 0]
                  .monvar.qc.frm3.list2.lbox delete 0
                  set lista2 [.monvar.qc.frm1.list1.lbox get 0 end]
                  if {[llength $lista2] == 0} {
                     .monvar.qc.frm1.list1.lbox insert end $listEntry
                  } else {
                     set nn [lsearch $f14List $listEntry]
                     set found 0
                     while {! $found && $nn < [llength $f14List]} {
                           set indexList [lsearch $lista2 [lindex $f14List [incr nn]]]
                           if {$indexList >= 0} {
                              set found 1
                           }
                     }
                     if {! $found} {
                           .monvar.qc.frm1.list1.lbox insert end $listEntry
                     } else {
                           .monvar.qc.frm1.list1.lbox insert $indexList $listEntry
                     }
                  }
               }
        }



        button .monvar.qc.frm2.del -text "Del" -command {
                global typev f14List
               foreach i [.monvar.qc.frm3.list2.lbox curselection] {
                  set listEntry [.monvar.qc.frm3.list2.lbox get $i]
                  .monvar.qc.frm3.list2.lbox delete $i
                  set lista1 [.monvar.qc.frm1.list1.lbox get 0 end]
                  if {[llength $lista1] == 0} {
                     .monvar.qc.frm1.list1.lbox insert end $listEntry
                  } else {
                     set nn [lsearch $f14List $listEntry]
                     set found 0
                     while {! $found && $nn < [llength $f14List]} {
                           set indexList [lsearch $lista1 [lindex $f14List [incr nn]]]
                           if {$indexList >= 0} {
                              set found 1
                           } 
                     }
                     if {! $found} {
                           .monvar.qc.frm1.list1.lbox insert end $listEntry
                     } else {
                           .monvar.qc.frm1.list1.lbox insert $indexList $listEntry
                     }
                  }
               }
        }

        pack .monvar.qc.frm2.add -side top -pady 2m
        pack .monvar.qc.frm2.del -side top -pady 2m
        pack .monvar.qc.frm2.all -side top -pady 2m
        pack .monvar.qc.frm2.delall -side top -pady 2m
        label .monvar.qc.frm3.lab2 -font "Times 14" -text "Monitored variables:"
        pack .monvar.qc.frm3.lab2 -side top -pady 3m
        pack .monvar.qc.frm3.list2.lbox .monvar.qc.frm3.list2.scrl -anchor center -side left -fill y
        pack .monvar.qc.frm3.list2 -side top -pady 3m
        frame .monvar.qc.frm3.src2 -relief ridge -bd 1 -height 2
        radiobutton .monvar.qc.frm3.src2.but
        entry .monvar.qc.frm3.src2.entry -textvariable findVar2
        label .monvar.qc.frm3.src2.lab -text "Find" -font "Courier 12"
        pack .monvar.qc.frm3.src2.but .monvar.qc.frm3.src2.entry .monvar.qc.frm3.src2.lab \
             -side left -padx 3m -pady 3m
        bind .monvar.qc.frm3.src2.but <Button-1> {
             if ![info exists srcIndex2] {set srcIndex2 ""}
             findItem .monvar.qc.frm3.list2.lbox srcIndex2 $findVar2
        }
        pack .monvar.qc.frm3.src2 -side top -pady 3m
        pack .monvar.qc.frm1 .monvar.qc.frm2 .monvar.qc.frm3 -side left
        pack .monvar.qc -side top
        frame .monvar.sepx -relief ridge -bd 1 -height 2
        pack .monvar.sepx -side top -pady 5m -fill x
        frame .monvar.butt
        button .monvar.butt.ok -text OK -command {
        global typev f14List
               set f [checkopen lg3.sel w]
               foreach i [.monvar.qc.frm1.list1.lbox get 0 end] {
                   puts $f [format "%s %s" $i $typev($i)]
               }
               puts $f "********"    
               foreach i [.monvar.qc.frm3.list2.lbox get 0 end] {
                   puts $f [format "%s %s" $i $typev($i)]
               }
               close $f
               set monList [.monvar.qc.frm3.list2.lbox get 0 end]
               destroy .monvar
               if [winfo exists .monitor] {
                  destroy .monitor
#                  .buttons.monit invoke
		  p_monitor_var
              }
        }
        pack .monvar.butt.ok -expand 1
        pack .monvar.butt -side bottom -fill x -pady 3m
     }
}



proc p_monitor_var { } {

package require Tix 

	global env activeModel  DIRMODEL f14File typev f14List   monList 

       if { ([llength $monList] == 0) && [file exists lg3.sel] } {
          set f [checkopen lg3.sel r]
          gets $f line
          while {[string compare $line "********"] != 0} {
                gets $f line
          }
          while {[gets $f line] >= 0} {
		    set idf [string range $line 0 7]		
                lappend monList $idf
                set typev($idf) [string range $line 9 9]
          }
          close $f
       }
       catch {destroy .monitor}
       toplevel .monitor
       wm title .monitor "Variables Monitor"
       wm iconname .monitor "vMON"
       label .monitor.title -bd 1 -height 2 -font "Courier 12 bold" \
       -text " VARNAME    I/O   KNOWN   WEIGHT       f14           f24       f24(t-1)    f24(t-2)    f24(t-3)"
       pack .monitor.title -side top -anchor w
       tixScrolledWindow .monitor.sw -scrollbar auto -height 250
       set subw [.monitor.sw subwidget window] 
       foreach i $monList {         
          set varname [string tolower $i]
          frame $subw.frm${varname}
          label $subw.frm${varname}.name -text $i -font "Courier 12"
          label $subw.frm${varname}.type -text $typev($i) -font "Courier 14"
          pack $subw.frm${varname}.name -side left -padx 2m
          pack $subw.frm${varname}.type -side left -padx 8m
          checkbutton $subw.frm${varname}.chbut -variable noto($i) -command {
           foreach i $monList {
            set varname [string tolower $i]
            if {$noto($i) == 1} {
             set ${varname}wght "NOTO"
             set notopr($i) 1
            } elseif {$noto($i) == 0 && $notopr($i) == 1} {
             set ${varname}wght ""
             set notopr($i) 0
            }
           }
          }

          pack $subw.frm${varname}.chbut -side left -padx 4m 
       }
       set f [checkopen f14.dat r]
       foreach i $monList {
          gets $f line
          while {[string range $line 4 11] != $i} {
                gets $f line
          }
          set varname [string tolower $i]
          set wghtVarName ${varname}wght
          set f14vVarName ${varname}f14v
          entry $subw.frm${varname}.wght -textvariable $wghtVarName \
                                         -font "Courier 12" -width 4
          puts $subw.frm${varname}.wght 
          puts $subw.frm${varname}.wght 
          entry $subw.frm${varname}.f14v -textvariable $f14vVarName \
                                         -font "Courier 12" -width 10
          trace variable $wghtVarName w forceWidth4
	    trace variable $f14vVarName w forceWidth10
          set $wghtVarName [string range $line 39 42]
          if {[set $wghtVarName] == "NOTO"} {
           set noto($i) 1
           set notopr($i) 1
          } else {
           set noto($i) 0
           set notopr($i) 0
          }
          set $f14vVarName [string range $line 14 23]
          pack $subw.frm${varname}.wght $subw.frm${varname}.f14v \
               -side left -padx 6m
       }
       close $f
       if [file exists proc/f24.dat] {
          set f [checkopen proc/f24.dat r]
          foreach i $monList {
             gets $f line
             while {[string range $line 4 11] != $i} {
                   gets $f line
             }
             set varname [string tolower $i]
             label $subw.frm${varname}.f24v -text [string range $line 14 23] \
                   -font "Courier 12" -width 10 -relief ridge -bd 1 -height 1
             pack $subw.frm${varname}.f24v -side left -padx 2m
          }
          close $f
       }
       if [file exists proc/f24.da1] {
          set f [checkopen proc/f24.da1 r]
          foreach i $monList {
             gets $f line
             while {[string range $line 4 11] != $i} {
                   gets $f line
             }
             set varname [string tolower $i]
             label $subw.frm${varname}.min1v -text [string range $line 14 23] \
                   -font "Courier 12" -width 10 -relief ridge -bd 1 -height 1
             pack $subw.frm${varname}.min1v -side left -padx 2m
          }
          close $f
       }
       if [file exists proc/f24.da2] {
          set f [checkopen proc/f24.da2 r]
          foreach i $monList {
             gets $f line
             while {[string range $line 4 11] != $i} {
                   gets $f line
             }
             set varname [string tolower $i]
             label $subw.frm${varname}.min2v -text [string range $line 14 23] \
                   -font "Courier 12" -width 10 -relief ridge -bd 1 -height 1
             pack $subw.frm${varname}.min2v -side left -padx 2m
          }
          close $f
       }
       if [file exists proc/f24.da3] {
          set f [checkopen proc/f24.da3 r]
          foreach i $monList {
             gets $f line
             while {[string range $line 4 11] != $i} {
                   gets $f line
             }
             set varname [string tolower $i]
             label $subw.frm${varname}.min3v -text [string range $line 14 23] \
                   -font "Courier 12" -width 10 -relief ridge -bd 1 -height 1
             pack $subw.frm${varname}.min3v -side left -padx 2m
          }
          close $f
       }
       foreach i $monList {
          set varname [string tolower $i]
          pack $subw.frm${varname} -side top -pady 3m -anchor w
       }
       pack .monitor.sw -side top -expand yes -fill x
       frame .monitor.sep -relief ridge -bd 1 -height 2
       pack .monitor.sep -fill x -pady 3m
       frame .monitor.butt
       button .monitor.butt.save -text "Save" -command {
              set f [checkopen f14.dat r]
              set f2 [checkopen f14.da2 w]
              set varsect 1
              while {[gets $f line] >= 0} {
                 if {[set index [lsearch $monList [string range $line 4 11]]] >= 0 && \
                    $varsect == 1} {
                    set varname [string tolower [lindex $monList $index]]
                    set ik [string toupper $varname]
                    set wghtVarName ${varname}wght
                    set f14vVarName ${varname}f14v
                    puts $f2 [format "%14s%-10s%15s%-4s%s" [string range $line 0 13] \
                                                           [set $f14vVarName] \
                                                           [string range $line 24 38] \
                                                           [set $wghtVarName] \
                                                           [string range $line 43 end]]
                 } else {
                    puts $f2 $line
                 }
                 if {[string range $line 4 27] == "DATI FISICI E GEOMETRICI"} {
                    set varsect 0
                 }
              }
              close $f
              close $f2
              file rename -force f14.da2 f14.dat
       }
       button .monitor.butt.config -text "Config" -command {
#              .menu.config invoke 0
		p_monitor_var_conf
       }
       button .monitor.butt.known -text "Inp. var. known" -command {
	     foreach i $monList {
            set varname [string tolower $i]
            if {$typev($i) == "I"} {
             set ${varname}wght "NOTO"
             set noto($i) 1
             set notopr($i) 1
            } elseif {$typev($i) == "O" && $noto($i) == 1} {
             set ${varname}wght ""
		 set noto($i) 0
             set notopr($i) 0                
            }
           }
          }               
       button .monitor.butt.canc -text "Cancel" -command {
              destroy .monitor
       }
       pack .monitor.butt.known .monitor.butt.save .monitor.butt.config \
            .monitor.butt.canc -side left -expand 1
       pack .monitor.butt -side bottom -fill x -pady 3m
}

