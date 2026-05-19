proc itemRotate {c} {

# CREA LA TOP LEVEL WINDOW PER EFFETTUARE LA ROTAZIONE

global dirlib nclass itemcur

set mylist_curr [$c gettags current]

      if {$mylist_curr == ""} {
      	       set mylist_curr $::currTags
       	       set mytag [lindex $::currTags [lsearch $::currTags id* ]]     	       
      	       set itemcur [string trimleft $mytag id]
      } else { 
      	       set mytag [lindex $mylist_curr [lsearch $mylist_curr id* ]]
               set itemcur [string trimleft $mytag id]
              }

      if {[lsearch -exact $mylist_curr  remarkdescr] != -1} { 
# è un remark, non si può ruotare
		tk_messageBox -title Warning -icon warning -message "This is a Text. It can not be rotated" -type ok
      	       	return }
              
#tk_messageBox -message "itemRotate: mylist_curr=$mylist_curr\n mytag=$mytag\n itemcur=$itemcur"
	set mylist [$c find withtag $mytag]
#tk_messageBox -message "itemRotate:itemcur=$itemcur\n mylist per foreach=$mylist"

	foreach item $mylist {
#tk_messageBox -message "itemRotate:itemc=$item|n [$c gettags $item]"

		if {[lsearch -exact [$c gettags $item] busy] != -1} {
			tk_messageBox -title Warning -icon warning -message "This module is linked.\nBreak connection first,\nthen rotate it." -type ok
			return
		}
      }
#set dirlib [file rootname [lindex [$c gettags current] [lsearch [$c gettags current] *.lpath]]]
#set nclass [file rootname [lindex [$c gettags current] [lsearch [$c gettags current] *.cls]]]
set dirlib [file rootname [lindex $mylist_curr [lsearch $mylist_curr *.lpath]]]
set nclass [file rootname [lindex $mylist_curr [lsearch $mylist_curr *.cls]]]
set icoe [file join $dirlib ${nclass}e.gif]
set icos [file join $dirlib ${nclass}s.gif]
set icow [file join $dirlib ${nclass}w.gif]

if {![file exists $icoe] || ![file exists $icos] || ![file exists $icow]} {
  tk_messageBox -title Warning -icon warning -message "This module may not be rotated" -type ok
			return
}

toplevel .rot
wm title .rot "ROTATE"
frame .rot.rot1
pack .rot.rot1 -side top -pady 1m

label .rot.rot1.ruota -text "Rotation angle"
pack .rot.rot1.ruota -side top

frame .rot.rot2
pack .rot.rot2 -side top -anchor w -padx 1m -pady 1m

button .rot.rot2.bu90 -text 90° -width 4 -command {doRotation $c 90}
button .rot.rot2.bu180 -text 180° -width 4 -command {doRotation $c 180}
button .rot.rot2.bu270 -text 270° -width 4 -command {doRotation $c 270}

pack .rot.rot2.bu90 .rot.rot2.bu180 .rot.rot2.bu270 -side left -anchor w -padx 1m

}

proc doRotation {c a} {

# CREA LA TOP LEVEL WINDOW PER EFFETTUARE LA ROTAZIONE

global dirlib nclass itemcur fromfile progNumb modified

set GIForientold [file rootname [lindex [$c gettags $itemcur] [lsearch [$c gettags $itemcur] *.ori]]]

# calcola la nuova gif


switch $GIForientold {
 "n" {set newor(0) "n"; set newor(90) "e"; set newor(180) "s"; set newor(270) "w"}
 "e" {set newor(0) "e"; set newor(90) "s"; set newor(180) "w"; set newor(270) "n"}
 "s" {set newor(0) "s"; set newor(90) "w"; set newor(180) "n"; set newor(270) "e"}
 "w" {set newor(0) "w"; set newor(90) "n"; set newor(180) "e"; set newor(270) "s"}
}

# salva le coordinate attuali e cancella la vecchia istanza

set coords [$c coords $itemcur]
set blname [file rootname [lindex [$c gettags $itemcur] [lsearch [$c gettags $itemcur] *.name]]]

set xist [lindex $coords 0]
set yist [lindex $coords 1]

set fromfile "yes"

set mylist [$c gettags $itemcur]
set mytag [lindex $mylist [lsearch $mylist id* ]]
set mylist [$c find withtag $mytag]

foreach item $mylist {$c delete $item}
set modified 1


set x $xist
set y $yist

set mlpath $dirlib
set idclass $nclass
set GIForient $newor($a)
set ff_progNumb $blname


source [file join $dirlib $nclass.tcl]

destroy .rot

}