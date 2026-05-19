proc golg3 { modo parent1 } {
   global env curFileName
   global modified1


   if {$curFileName == "untitled" || $curFileName == "" || $curFileName == "-"} {
		set messaggio "No Model is selected!"
		tk_messageBox -message $messaggio -type ok -icon error
		return
   }
   if { $modified1 == 1} {
         set mex [tk_messageBox -icon warning -type yesno \
                 -title Warning -parent . -message "Model Data modified. Save f14...?"]
	        if {$mex == "yes"} {  saveF01 }
#	tk_messageBox -icon warning -type ok \
#         -title Warning -parent . -message "Data modified. Save F01/F14 is required."
#    return
    }

set SLV $env(LG_LEGO)
set tk_strictMotif  0

     destroy .conv
     destroy .nonconv
     set f14File [file dirname $curFileName]/f14.dat
     set filelist [split $f14File /]
     if ![string compare [lindex $filelist [expr [llength $filelist]-1]] f14.dat] {
        if [file exists proc/f24.da2] {
                 file rename -force proc/f24.da2 proc/f24.da3
        }
        if [file exists proc/f24.da1] {
                 file rename -force proc/f24.da1 proc/f24.da2
        }
        if [file exists proc/f24.dat] {
                 file rename -force proc/f24.dat proc/f24.da1
      }


if  { $::LINUXPLAT == 0 } {

     if { ![file exists lg3.inp]}  { file copy -force $SLV/lg3.def lg3.inp }

#tk_messageBox -icon info -message [file rootname [file tail $curFileName]] -type ok
#tk_messageBox -icon info -message [file rootname $curFileName] -type ok
#tk_messageBox -icon info -message [file dirname $curFileName] -type ok
#tk_messageBox -icon info -message $f14File -type ok

     catch {exec $SLV/nssls.bat}
     catch {exec $SLV/lgser.bat}
     }

     catch {exec proc/nssls <lg3.inp >lg3.out}
}

if  { $::LINUXPLAT == 1 } {

     set comm1 [file join $env(LEGO_BIN) cad_crealg3 ]
     set comm2 "p"
     set comm3 "lg3comp.out"
     set chk [catch {exec $comm1 $comm2 > $comm3} result]
     if { $chk != 0 } {
        tk_messageBox -icon info -message "Buld Steady state Error: $comm1 $comm2 > $comm3 chk:$chk - result:$result"
     }
     if { $chk == 1 } { tk_messageBox -icon error -message "LG3 Compilation failed: Details in $comm3"; return }

# lettura del file delle opzioni lg3.inp

     if [file exists lg3.inp] {
        set f [checkopen lg3.inp r]
     } else {
        set f [checkopen $::env(LEGO_BIN)/lg3.inp r]
     }
        gets $f ::stampe
        gets $f ::tolerance
        close $f

puts "golg3: $::tolerance"
      set comm1 "proc/lg3"
      set comm2 $::tolerance
      set comm3 "lg3.out"
#      tk_messageBox -icon info -message "golg3 2: $comm1 $comm2 > $comm3"
      set chk [catch {exec $comm1 $comm2 > $comm3} result]
}


                 set cond1 [catch { exec grep -c "CONDIZIONI DI REGIME CALCOLATE" lg3.out} ]
                 set cond2 [ catch { exec grep -c "EQUAZIONI NON SONO VERIFICATE" lg3.out} ]
                 if {$cond1 == 0} {
                  set cond1 1
                 } elseif {$cond1 == 1} {
                  set cond1 0
                 }
                 if {$cond2 == 0} {
                  set cond2 1
                 } elseif {$cond2 == 1} {
                  set cond2 0
                 }
		  catch {destroy .conv}
                  toplevel .conv
                  wm title .conv  "Steady state results"
		  
                  set conver 0
              if {($cond1 == 0) || (($cond1 > 0) && ($cond2 > 0))} {  label .conv.lab -width 50 -text "Steady state does not converges" -font "Times 14 bold"
               } else { set conver 1 ; label .conv.lab -width 50 -text "Steady state converges" -font "Times 14 bold"
               file copy -force proc/f24.dat proc/f24.last
               }

                  frame .conv.button1
                  frame .conv.button2
                  label .conv.button1.lab -text "view file lg3.out" -justify left
                  button .conv.button1.lg3 -background red -command "edit lg3.out $tk_strictMotif"
                  label .conv.button2.lab -text "view differences between f14 and f24"
                  button .conv.button2.df  -background red \
                                           -command "compareFiles f14.dat proc/f24.dat" \
                                           -justify left
                  pack .conv.lab -side top
                  pack .conv.button1.lg3  -side left -padx 1m -pady 1m
                  pack .conv.button1.lab  -side left -padx 1m -pady 1m
                  pack .conv.button1 -fill x -expand 1
                  pack .conv.button2.df -side left -padx 1m -pady 1m
                  pack .conv.button2.lab  -side left -padx 1m -pady 1m
                  pack .conv.button2 -fill x -expand 1
                  frame .conv.button3
                  label .conv.button3.lab -text "Copy f24 in f14"
                  button .conv.button3.but -background blue -justify left -command "copiaf24 $modo $parent1"
frame .conv.sep -relief ridge -bd 1 -height 2
                  frame .conv.button4
                  label .conv.button4.lab -text "Execute drift"
                  button .conv.button4.but -background green -justify left -command {
			set SLV         $env(LG_LEGO)
			set LIBUT       $env(LG_LIBUT)
                        execLg5_v2 $SLV $LIBUT "drift" $tk_strictMotif
                  }
                  pack .conv.button3.but -side left -padx 1m -pady 1m
                  pack .conv.button3.lab -side left -padx 1m -pady 1m
                  pack .conv.button3 -fill x -expand 1
pack .conv.sep  -fill x -expand 1
                  pack .conv.button4.but -side left -padx 1m -pady 1m
                  pack .conv.button4.lab -side left -padx 1m -pady 1m
                  pack .conv.button4 -fill x -expand 1
                  frame .conv.buttonok
                  button .conv.buttonok.ok -text Cancel -command {destroy .conv}
                  pack .conv.buttonok.ok -expand 1
                  pack .conv.buttonok -side bottom -fill x -expand 1

if { $conver == 0 } {
.conv.button4.but configure -background gray
.conv.button4.but configure -state disable }

}

proc copiaf24 { modo parent1 } {
   global env curFileName
	file delete f14.dat
	file copy proc/f24.dat f14.dat
#	set comm1 [info nameofexecutable]
#	set comm2 "$env(LG_TIX)/legodat.tix"
##tk_messageBox -icon info -message "$comm1 $comm2" -type ok
#	destroy $parent1
#	exec $comm1 -f $comm2 [file rootname [file tail $curFileName]]

if { $modo == 1 } { loadF01 $parent1 no }

}



proc execLg5_v2 {SLV LIBUT MODE tk_strictMotif} {

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


    catch {destroy .conv.frm}
    catch {destroy .conv.frm2}

    frame .conv.frm
    label .conv.frm.lab -text "Transient calculation ended correctly" \
                        -font "Courier 14 bold"
    pack .conv.frm.lab -padx 1m -pady 1m -side top
    frame .conv.frm2
    button .conv.frm2.b1 -text "OutFile" -background green -command "edit lg5.out $tk_strictMotif"
   if { $::tcl_platform(os) == "Linux" } {
    button .conv.frm2.b2 -text "Plot trends" -background green -command " watchtrends 0 proc/f22.dat"
   } else {
    button .conv.frm2.b2 -text "Plot trends" -background green -command " watchtrends_old $tk_strictMotif"
 }

    pack .conv.frm2.b1 .conv.frm2.b2  -side left -padx 9m -pady 3m -expand 1
    pack .conv.frm .conv.frm2 -pady 3m -side top
}



source $env(LG_TIX)/subwgs.tcl
