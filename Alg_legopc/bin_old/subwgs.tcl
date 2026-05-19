#============================================================

proc forceWidth4 {name el op} {
   global $name ${name}_wid
   set old ${name}_wid
   if {[string length [set $name]] > 4} {
      set $name [set $old]
      bell; return
   }
   set $old [set $name]
}

#============================================================

proc forceWidth10 {name el op} {
   global $name ${name}_wid
   set old ${name}_wid
   if {[string length [set $name]] > 10} {
      set $name [set $old]
      bell; return
   }
   set $old [set $name]
}

#============================================================

proc compareFiles {f14 f24} {
    global list2
    set f1 [checkopen $f14 r]
    set list1 {}
    set in_vars 0
    while {[gets $f1 line] >= 0} {
        if {[string first "DATI FISICI E GEOMETRICI" $line] > 0} {
            set in_vars 0
        } elseif {[string first "CONDIZIONI INIZIALI VARIABILI" $line] > 0} {
            set in_vars 1
        }
        if {$in_vars == 1} {
#           lappend list1 [string range $line 0 43]
            lappend list1 [string range $line 0 24]
        }
    }
    close $f1
    set f2 [checkopen $f24 r]
    set list2 {}
    set in_vars 0
    while {[gets $f2 line] >= 0} {
        if {[string first "DATI FISICI E GEOMETRICI" $line] > 0} {
            set in_vars 0
        } elseif {[string first "CONDIZIONI INIZIALI VARIABILI" $line] > 0} {
            set in_vars 1
        }
        if {$in_vars == 1} {
#           lappend list2 [string range $line 0 37]
           lappend list2 [string range $line 0 70]
        }
    }
    close $f2
    catch {destroy .cpr}
    toplevel .cpr
    wm title .cpr "Comparison between f14.dat and f24.dat"
    wm iconname .cpr "diff"

    frame .cpr.a
    
    frame .cpr.a.frm0     
    label .cpr.a.frm0.lab14 -text "F14 - initial conditions" \
                         -font "Courier 12"
    scrollbar .cpr.a.frm0.scrl -command {scrollProc}
    listbox .cpr.a.frm0.f14 -yscroll ".cpr.a.frm0.scrl set" \
                         -setgrid 1 -height 12 -width 0  \
                         -font "Courier 12"

    frame .cpr.a.frm   
    label .cpr.a.frm.lab24 -text "F24 - final conditions" \
                         -font "Courier 12"
                        
    listbox .cpr.a.frm.f24 -yscroll ".cpr.a.frm0.scrl set" \
                         -setgrid 1 -height 12 -width 0 \
                         -font "Courier 12"
                         
    foreach  i $list1 j $list2  {
       .cpr.a.frm0.f14 insert end $i
       .cpr.a.frm.f24 insert end $j
       if { [string compare -length 25 $i $j ] != 0 } { .cpr.a.frm.f24 itemconfigure end -background yellow; \
              .cpr.a.frm0.f14 itemconfigure end -background yellow; }
    }

global icontcerca seastring
set icontcerca 0
set lunlist2 [llength  $list2] 
#set seastring "TCOL"

    frame .cpr.1frm2

    label .cpr.1frm2.lab  -justify left -pady 2m -font "Times 12" \
                                        -text "Find..." -anchor w
    label .cpr.1frm2.labwarn -width 60 -justify left -pady 2m -font "Times 12" \
                                        -text " " -foreground red          
    entry .cpr.1frm2.ent -width 20 -textvariable seastring 
    button .cpr.1frm2.next -text "Next" -command "cercacomp  $lunlist2  [expr 1] " 
    button .cpr.1frm2.prev -text "Previous" -command "cercacomp $lunlist2  [expr -1]" 

    button .cpr.1frm2.ok -text Cancel -command "destroy .cpr" -anchor e
    
    pack .cpr.1frm2.lab .cpr.1frm2.ent .cpr.1frm2.next .cpr.1frm2.prev .cpr.1frm2.labwarn -side left -padx 2m -padx 2m
    
    pack .cpr.1frm2.ok -side right -expand no -padx 0m -pady 2m  
    pack .cpr.1frm2 -side bottom

 
    pack .cpr.a -expand yes -fill y 
    pack .cpr.a.frm0 .cpr.a.frm -side left -expand yes -fill y 

     
    pack .cpr.a.frm0.lab14
    pack .cpr.a.frm0.f14 -side left -fill y -expand yes
    pack .cpr.a.frm0.scrl -side left -padx 2m -fill y

    pack .cpr.a.frm.lab24
    pack .cpr.a.frm.f24 -side left -fill y -expand yes



}

proc cercacomp {  lunlist2  vincr} {
#------------ prove di search
    global icontcerca list2 seastring
    

#tk_messageBox -message "cercacomp: $sstring ----  icontcerca $icontcerca" -type ok  
    .cpr.1frm2.labwarn configure -text "" 
     set trovato 0
     if { $icontcerca <= 0 } { 
     	set icontcerca 1 
     } else {
     	.cpr.a.frm.f24 itemconfigure $icontcerca -foreground black 
     	.cpr.a.frm0.f14 itemconfigure $icontcerca -foreground black 
        incr icontcerca $vincr
     }
#tk_messageBox -message "cercacomp: $sstring ----  icontcerca $icontcerca" -type ok          
     for { set i $icontcerca } { $i < $lunlist2 && $i > 0  } { incr i $vincr} {
       if { [ string first  $seastring [ lindex $list2 $i ] ] > 0 } { 
        set icontcerca $i
##tk_messageBox -message "trovato $sstring ---- $i --- in $icontcerca" -type ok  

        eval .cpr.a.frm0.f14 yview moveto 0
        eval .cpr.a.frm.f24 yview moveto 0
        eval .cpr.a.frm0.f14 yview scroll [expr $icontcerca-1] units
        eval .cpr.a.frm.f24 yview scroll [expr $icontcerca-1] units  
        .cpr.a.frm.f24 itemconfigure $i -foreground red;
        .cpr.a.frm0.f14 itemconfigure $i -foreground red;
        
        set trovato 1;
        break;     	
       	}
    } 
    if { $trovato == 0 } { 
    bell;
    .cpr.1frm2.labwarn configure -text "$seastring --- Not Found!"  
#    tk_messageBox -message "STRINGA non trovata: $sstring! --- in $icontcerca" -type ok 
    set icontcerca 0
    }

}

#============================================================

proc scrollProc args {
     eval .cpr.a.frm0.f14 yview $args
     eval .cpr.a.frm.f24 yview $args
}

#============================================================

proc execLg5 {SLV LIBUT MODE tk_strictMotif} {

global env

     if {$::tcl_platform(os) != "Linux"} {
     set lg4drift $SLV/lg4.inp
     set lg5drift $SLV/lg5.inp
     } else {
     set lg4drift $env(LG_TIX)/lg4_drift.inp
     set lg5drift $env(LG_TIX)/lg5_drift.inp
     }

     if {$::tcl_platform(os) != "Linux"} {
        if {$MODE == "drift"} {
           exec $SLV/lg4_exe <$lg4drift >lg4.out
        } else {
           exec $SLV/lg4_exe <lg4.inp >lg4.out
        }
     } else {
        if {$MODE == "drift"} {
           exec lg4_exe <$lg4drift >lg4.out
        } else {
           exec lg4_exe <lg4.inp >lg4.out
        }
     }

     if { $::tcl_platform(os) == "Linux" } {
#        exec make -f $SLV/crea_solver lg5
        set comm1 "crealg5"
	set comm2 "lg5.out"
	set chk [catch {exec $comm1 > $comm2} result]
        if { $chk != 0 } {
         tk_messageBox -message "Build lg5 Error: $comm1 >> $comm2  chk:$chk result:$result"
        }

#	exec crealg5
     } else {
        catch {exec $SLV/lg5.bat}
        catch {exec $SLV/lgser.bat}
     }
     if {$MODE == "drift"} {
        exec proc/lg5 <$lg5drift >lg5.out
     } else {
        exec proc/lg5 <lg5.inp >lg5.out
     }
    
    catch {destroy .end5} 
    toplevel .end5
    wm title .end5 "End of transient"
    wm iconname .end5 "info"
    frame .end5.frm
    label .end5.frm.lab -text "********\n\nTRANSIENT CALCULATION ENDED CORRECTLY\n\n********" \
                        -font "Courier 16 bold"
    pack .end5.frm.lab -padx 3m -pady 3m -side top
    frame .end5.frm2
    button .end5.frm2.b1 -text "OutFile"  -command "edit lg5.out $tk_strictMotif"
     if { $::tcl_platform(os) == "Linux" } {
    button .end5.frm2.b2 -text "Graphics" -command " watchtrends 0 proc/f22.dat"
   } else {
    button .end5.frm2.b2 -text "Graphics" -command " watchtrends_old $tk_strictMotif"
   }
    button .end5.frm2.b3 -text "Cancel"   -command "destroy .end5"
    pack .end5.frm2.b1 .end5.frm2.b2 .end5.frm2.b3 -side left -padx 9m -pady 3m -expand 1
    pack .end5.frm .end5.frm2 -pady 3m -side top
}

#============================================================

proc to_ascii {} {
     set list {}
     global curFileName
     	if {$curFileName == "untitled" || $curFileName == "" || $curFileName == "-"} {
		set messaggio "No f22 to convert"
		tk_messageBox -message $messaggio -type ok -icon error
		return
	}
     set f [checkopen f14.dat r]
     for {set i 1} {$i < 5} {incr i} {
         gets $f line
     }
     gets $f line
     while {[string index $line 24] == "*"} {
           lappend list [string range $line 4 11]
           gets $f line
     }
     gets $f line
     while {[string index $line 24] == "*"} {
           lappend list [string range $line 4 11]
           gets $f line
     }
     close $f
     catch {destroy .varch}
     toplevel .varch
     wm title .varch "Trends of selected variables"
     wm iconname .varch "VARs"
     wm minsize .varch 15 1
     frame .varch.list
     listbox .varch.list.lbox -yscroll ".varch.list.scr set" \
                              -setgrid 1 -height 8 -width 0 \
                              -font "Courier 12"
     foreach i $list {
             .varch.list.lbox insert end $i
     }
     scrollbar .varch.list.scr -command ".varch.list.lbox yview"
     frame .varch.src -relief ridge -bd 1 -height 2
#     radiobutton .varch.src.but
     button .varch.src.but -text "Find"
     entry .varch.src.entry -textvariable findVar2
#     label .varch.src.lab -text "Find" -font "Courier 12"
#     pack .varch.src.but .varch.src.entry .varch.src.lab -side left -padx 3m -pady 3m
     pack .varch.src.entry .varch.src.but -side left -padx 3m -pady 3m
     bind .varch.src.but <Button-1> {
          if ![info exists srcIndex2] {set srcIndex2 ""}
          findItem .varch.list.lbox srcIndex2 $findVar2
     }
     frame .varch.frame
     frame .varch.frame.radio
     radiobutton .varch.frame.radio.r1 -variable rvar -relief flat -value 1
     radiobutton .varch.frame.radio.r2 -variable rvar -relief flat -value 2
     radiobutton .varch.frame.radio.r3 -variable rvar -relief flat -value 3
     radiobutton .varch.frame.radio.r4 -variable rvar -relief flat -value 4
     frame .varch.frame.entry
     entry .varch.frame.entry.e1 -width 25 -textvariable evar1
     entry .varch.frame.entry.e2 -width 25 -textvariable evar2
     entry .varch.frame.entry.e3 -width 25 -textvariable evar3
     entry .varch.frame.entry.e4 -width 25 -textvariable evar4
     bind .varch.frame.radio.r1 <Button-1> {
          set evar1 [selection get]
     }
     bind .varch.frame.radio.r2 <Button-1> {
          set evar2 [selection get]
     }
     bind .varch.frame.radio.r3 <Button-1> {
          set evar3 [selection get]
     }
     bind .varch.frame.radio.r4 <Button-1> {
          set evar4 [selection get]
     }

#
#     frame .varch.dless -relief ridge -bd 1 -height 2
#     checkbutton .varch.dless.check -variable dless -anchor w \
#		     -text "Dimensionless " -font "Courier 12" -padx 5m
     frame .varch.butt
     button .varch.butt.ok -text "OK" -command {
        set f [checkopen tradf22.inp w]

#        if {$dless} {
#           puts $f "p.u."
#        } else {
#           puts $f "dim."
#        }

        foreach i [list $evar1 $evar2 $evar3 $evar4] {
            if {$i != ""} {
               puts $f $i
            }
        }
#        puts $f "EOF"
        close $f
        exec $SLV/tradf22
        if ![info exists ncount] {
           set ncount 1
        } else {
           incr ncount
        }
        toplevel .values${ncount}
        wm title .values${ncount} "Transient values"
        wm iconname .values${ncount} "tVAL"
        frame .values${ncount}.frm
        listbox .values${ncount}.frm.lbox -yscroll ".values${ncount}.frm.scr set" \
                                          -setgrid 1 -height 15 -width 0 -font "Courier 12"
        scrollbar .values${ncount}.frm.scr -command ".values${ncount}.frm.lbox yview"
        pack .values${ncount}.frm.lbox .values${ncount}.frm.scr -side left -fill y -padx 2m -pady 3m
        .values${ncount}.frm.lbox insert end \
                     [format "      time %8s   %8s   %8s   %8s" $evar1 $evar2 $evar3 $evar4]
        .values${ncount}.frm.lbox insert end "      ---- ---------- ---------- ---------- ----------  "
        set f [checkopen "f22.asc" r]
        while {[gets $f line] >=0} {
              .values${ncount}.frm.lbox insert end $line
        }
        close $f
        frame .values${ncount}.butt
        button .values${ncount}.butt.ok -text OK -command "destroy .values${ncount}"
        pack .values${ncount}.butt.ok -expand 1
        pack .values${ncount}.frm .values${ncount}.butt -side top -fill x -pady 3m
     }
     button .varch.butt.cancel -text "Cancel" -command {
        destroy .varch
     }
     pack .varch.frame.entry.e1 .varch.frame.entry.e2 \
          .varch.frame.entry.e3 .varch.frame.entry.e4 \
          -side top -pady 2.8m -anchor n
     pack .varch.frame.radio.r1 .varch.frame.radio.r2 \
          .varch.frame.radio.r3 .varch.frame.radio.r4 \
          -side top -pady 1m -anchor n
     pack .varch.frame.radio .varch.frame.entry -side left \
          -padx 2m -pady 1m
     pack .varch.list.lbox .varch.list.scr -side left \
          -fill both -padx 2m -expand 1
#     pack .varch.dless.check -side left -fill both -padx 16m -expand 1
     pack .varch.butt.ok .varch.butt.cancel -side left -padx 3m -pady 1m -fill x -expand 1
#     pack .varch.list .varch.src .varch.frame .varch.dless .varch.butt -side top -pady 1m
     pack .varch.list .varch.src .varch.frame  .varch.butt -side top -pady 1m
}

#============================================================

proc findItem {lbox srcInd findVar} {
     upvar $srcInd srcIndex
     $lbox selection clear 0 [$lbox size]
     if {$srcIndex == ""} {
        set elem 0
     } else {
        set elem [expr $srcIndex+1]
        if {$elem > [$lbox size]} {
           set elem 0
        }
     }
     set listSize [$lbox size]
     set i $elem; set n 1
     set found 0
     while {$n <= $listSize} {
        if [string match *${findVar}* [$lbox get $i]] {
           set srcIndex $i
           set found 1
           break
        } else {
           incr n
           if {$i == $listSize} {
              set i 0
           } else {
              incr i
           }
        }
     }
     if {! $found} {
        set srcIndex 0
     }
     $lbox selection set $srcIndex $srcIndex
     $lbox yview $srcIndex
}


#============================================================

proc edit {file tk_strictMotif} {
global env
#     if {$tk_strictMotif == 1}
#        catch [exec $::env(LG_TEXTEDITOR) $file &]
if   { $::LINUXPLAT == 1 } {
        catch {exec $::env(LG_TEXTEDITOR) $file &}
     } else {
        catch {exec $env(LG_LEGO)/edita.bat [join [split $file / ] \\ ] &}
     }
}

#============================================================

#proc watchtrends { tk_strictMotif } {
#global env
#    if ![file exists proc/f22.dat] {
#       tk_messageBox -title "Warning" -message "NO FILE proc/f22.dat TO SHOW!" \
#                     -type ok -icon warning
#    } else {
#          	exec [info nameofexecutable] -f $env(LG_TIX)/draw2gr.tcl &
#    }
#}
#
proc watchtrends { modo filetoview } {

# modo 0  file sequenziale
# modo 1  file circolare ( solo per linux)

global env

if { ![string compare $filetoview "f22circ"] } { set filetest  $filetoview.dat
} else { set filetest $filetoview }
puts "filetest: $filetest"
puts "$filetoview $filetest"
    if ![file exists $filetest ] {
       tk_messageBox -title "Warning" -message "NO FILE $filetest TO SHOW!" \
                     -type ok -icon warning
    } else {         	
          	exec [info nameofexecutable] -f $env(LG_TIX)/draw2gr.tcl $modo $filetoview &
    }
}



proc watchtrends_old { tk_strictMotif } {
global env
    if ![file exists proc/f22.dat] {
       tk_messageBox -title "Warning" -message "NO FILE proc/f22.dat TO SHOW!" \
                     -type ok -icon warning
    } else {
       if {$tk_strictMotif == 1} {
	    catch {exec "echo proc/f22.dat | traduci_f22"}
          catch {exec grafics ./f22 &}
       } else {
		set translate 0
       		if ![file exists trad.f22] {
#			set translate 1
      		} else {
            		set timeTrad [file mtime trad.f22]
			set timeF22  [file mtime proc/f22.dat]
			if {$timeF22 > $timeTrad} {
#				set translate 1
			}
       		}
		if {$translate == 1} {
  	 	        exec forcedos $env(LG_GRAF)/traduci.bat
		}
          	exec [info nameofexecutable] -f $env(LG_TIX)/draw2gr.tcl &
       }
    }
}
