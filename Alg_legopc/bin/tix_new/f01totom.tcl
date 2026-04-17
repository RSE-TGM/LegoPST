#
# interfaccia gui a f01totom
#

proc positionWindow w {
    wm geometry $w +300+300
#    wm maxsize $w 50 30
}



proc textSearch {w string tag} {
    $w tag remove search 0.0 end
#tk_messageBox -message $string
    if {$string == ""} {
	return
    }
    set cur 1.0
    while 1 {
	set cur [$w search -count length $string $cur end]
	if {$cur == ""} {
	    break
	}
	$w tag add $tag $cur "$cur + $length char"
	set cur [$w index "$cur + $length char"]
    }
}

proc textToggle {cmd1 sleep1 cmd2 sleep2} {
    catch {
	eval $cmd1
	after $sleep1 [list textToggle $cmd2 $sleep2 $cmd1 $sleep1]
    }
}




proc fileDialog {w ent operation} {
    #   Type names		Extension(s)	Mac File Type(s)
    #
    #---------------------------------------------------------
    global filef01 filetom
    set typesRead {
	{"f01.dat files"	{.dat}		TEXT}

	{"All files"		*}
    }
    set typesWrite {
	{"f01.dat files"	{.tom}		TEXT}

	{"All files"		*}
    }
    if {$operation == "open"} {
	set filef01 [tk_getOpenFile -filetypes $typesRead -parent $w \
	    -initialfile $filef01 -defaultextension .dat ]
	set curdir [file dirname "$filef01"]
	cd $curdir
    } else {
	set filetom [tk_getSaveFile -filetypes $typesWrite -parent $w \
	    -initialfile $filetom -defaultextension .tom ]
    }
#    if [string compare $filef01 ""] {
#	$ent delete 0 end
#	$ent insert 0 $filef01
#	$ent xview end
#    }
#tk_messageBox -message $filef01
}

proc run_f01totom { w } {
global env
global filef01 filetom
#set mess "$filef01 $filetom"
#tk_messageBox -message $mess

catch {exec $env(LG_TOOLS)/f01totom -a $filef01 $filetom} res
$w.text configure -state normal
$w.text delete 1.0 end
$w.text insert end $res
$w.text yview moveto 1

## Esempio di ricerca di testo, inquesto caso la "e"
##set searchString {e}
##textSearch $w.text $searchString search



#$w.text configure -state disabled

}


proc showfile {w id} {
global filef01 filetom
if [string compare $id "1"] { set file $filef01
} else {
set file $filetom
#tk_messageBox -message [tk appname]
#send -async legopc.tix reload_curr
}
#set mess "showfile: $file $filef01 $filetom"
#tk_messageBox -message $mess

  if { [file exists $file] } {

    set f [checkopen $file]
    $w configure -state normal
    $w delete 1.0 end
    while {![eof $f]} {
	$w insert end [read $f 10000]
    }
    close $f
    $w configure -state disabled
  } else {
    $w configure -state normal
    $w delete 1.0 end
    $w insert end "Il file $file non esiste"
    $w configure -state disabled
  }


}
proc editinp { w file} {
  if { [file exists $file] } {
#	set editor notepad
        set editor $::env(LG_TEXTEDITOR)
	exec $editor  $file &
   } else {
    $w configure -state normal
    $w delete 1.0 end
    $w insert end "Il file $file non esiste"
    $w configure -state disabled
    }
}


#
#  MAIN #####
#
set filef01 "f01.dat"
set filetom "f01totom.tom"
set fileinp "f01totom.inp"
set curdir "."

set font {Courier 10}

if {$argc == 1} {
   set filef01 "f01totom.dat"
   set filetom $argv
} else {




#eval destroy [winfo child .]
#wm title . "f01 to tom import"
#set widgetDemo 1

set font {Helvetica 14}

#set w .filebox
#set w .
#catch {destroy $w}
#toplevel $w

frame .frmSopra
set wS .frmSopra

pack $wS

label $wS.msg -font $font -wraplength 4i -justify left -text "Enter \"f01\" file name to convert into \"tom\" file "
pack $wS.msg -side top -expand yes -fill x



wm title . "Import f01 to tom"
wm iconname . "filebox"
positionWindow .


set font {Courier 10}

#foreach i {open save} {
##global file_open file_save
#    set f [frame $w.$i]
#    label $f.lab -text "Select a file to $i: " -anchor e
#    entry $f.ent -width 20 -textvariable file_$i
#    button $f.but -text "Browse ..." -command "fileDialog $w $f.ent $i"
#    pack $f.lab -side left
#    pack $f.ent -side left -expand yes -fill x
#    pack $f.but -side left
#    pack $f -fill x -padx 1c -pady 3
#}

    set i "open"
    set f_open [frame $wS.open]
    label $f_open.lab -text "Select a file f01 to open: " -anchor e
    entry $f_open.ent -width 20 -textvariable filef01
    button $f_open.but -text "Browse ..." -command "fileDialog $wS $f_open.ent $i"
    pack $f_open.lab -side left -expand yes
    pack $f_open.ent -side left -expand yes -fill x
    pack $f_open.but -side left -expand yes
    pack $f_open -fill x -padx 1c -pady 3 -expand yes


    set i "save"
    set f_save [frame $wS.save]
    label $f_save.lab -text "Select a file tom to save: " -anchor e
    entry $f_save.ent -width 20 -textvariable filetom
    button $f_save.but -text "Browse ..." -command "fileDialog $wS $f_save.ent $i"
    pack $f_save.lab -side left -expand yes
    pack $f_save.ent -side left -expand yes -fill x
    pack $f_save.but -side left -expand yes
    pack $f_save -fill x -padx 1c -pady 3 -expand yes
}

frame .frmMezz
set wM .frmMezz
pack $wM -expand yes -fill both


text $wM.text -yscrollcommand "$wM.yscroll set"  -setgrid true \
	 -font $font -wrap word
#text $w.text -yscrollcommand "$w.yscroll set"  -setgrid true \
#	-width 80 -height 24 -font $font -wrap word
scrollbar $wM.yscroll -command "$wM.text yview" -orient vertical
#scrollbar $w.xscroll -command "$w.text xview" -orient horizontal

#	scrollbar .code.xscroll -command ".code.text xview" \
#	    -highlightthickness 0 -orient horizontal
#	scrollbar .code.yscroll -command ".code.text yview" \
#	    -highlightthickness 0 -orient vertical

pack $wM.yscroll -side right -fill y -expand yes -anchor w
#pack $w.xscroll -side bottom -fill x
pack $wM.text -expand yes -fill both
#$w.text configure -state disabled

#if ![string compare $tcl_platform(platform) unix] {
#    checkbutton $w.strict -text "Use Motif Style Dialog" \
#	-variable tk_strictMotif -onvalue 1 -offvalue 0
#    pack $w.strict -anchor c
#}



frame .frmGiu
set wG .frmGiu

pack $wG


frame $wG.buttons
pack $wG.buttons -side bottom -fill x -pady 2m
button $wG.buttons.exit -text Exit -command "destroy ."
button $wG.buttons.editinp -text "Conversion Init" -command "editinp $wM.text $fileinp"
button $wG.buttons.conv -text "Convert to tom" -command "run_f01totom $wM"
button $wG.buttons.shf01 -text "Show f01" -command "showfile $wM.text 2"
button $wG.buttons.shtom -text "Show tom" -command "showfile $wM.text 1"
pack $wG.buttons.editinp $wG.buttons.conv $wG.buttons.shf01 $wG.buttons.shtom  $wG.buttons.exit -side left -expand yes






#textToggle "$wM.text tag configure search -background \
#	    #ce5555 -foreground white" 800 "$wM.text tag configure \
#	    search -background {} -foreground {}" 200



