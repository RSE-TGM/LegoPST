# this file created with PREINST rel. 0.3 


	
	# era cosi: set mymodId [$c create image $x $y -image @com_0$GIForient]

#	set mymodId [$c create text $x $y -text "Double click to modify" -tags remark_testo]
	set mymodId [$c create text $x $y -text "Double click to modify!"]

#        set mvlist [join [set $mylist] "\n"]
##        upvar $qq font.$mymodId
#        lappend font.$mymodId "WWW"
#        set qq   font.$mymodId
##debug tk_messageBox -message "@COM_0.TCL: ECCOMI mymodId=$mymodId\n font.$mymodId=$qq" -type ok

	$c addtag id$mymodId withtag $mymodId
	$c addtag module withtag $mymodId
	$c addtag $idclass.cls withtag $mymodId
	$c addtag $GIForient.ori withtag $mymodId
	$c addtag remarkdescr withtag $mymodId
	if {$fromfile == "yes"} {
		$c addtag $mlpath.lpath withtag $mymodId
	} else {
		$c addtag $curLibPath.lpath withtag $mymodId
	}
	# let the user choose a name (Id)
	if {$fromfile == "yes"} then {set progName $ff_progNumb} else {inputModName $c $x $y}
	$c addtag $progName.name withtag $mymodId
	$c addtag font withtag $mymodId
	incr progNumb
#crezione tag per il font	
        global font.$mymodId
        set font.$mymodId [list helvetica 12]
        $c itemconfigure $mymodId  -font "helvetica 12"            
#debug        leggi_font $c $mymodId 1 "qq"           
      
#switch $GIForient {
#n {set myportId [$c create image [expr $x + -26.0] [expr $y + 0.0] -image hydr_e -anchor e]}
#e {set myportId [$c create image [expr $x + 0.0] [expr $y + -26.0] -image hydr_s -anchor s]}
#s {set myportId [$c create image [expr $x + 26.0] [expr $y + 0.0] -image hydr_w -anchor w]}
#w {set myportId [$c create image [expr $x + 0.0] [expr $y + 26.0] -image hydr_n -anchor n]}
#}
#$c addtag id$myportId withtag $myportId
#$c addtag id$mymodId withtag $myportId
#$c addtag port0 withtag $myportId
#$c addtag port withtag $myportId
#$c addtag hydr_ptype withtag $myportId; #port type
#$c addtag input withtag $myportId; #port direction
#global port0.$mymodId
#set port0.$mymodId { WE_1 PCOL ____ ____ ____ HE_1 ____ HCOL }
#
#switch $GIForient {
#n {set myportId [$c create image [expr $x + 26.0] [expr $y + 0.0] -image hydr_e -anchor w]}
#e {set myportId [$c create image [expr $x + 0.0] [expr $y + 26.0] -image hydr_s -anchor n]}
#s {set myportId [$c create image [expr $x + -26.0] [expr $y + 0.0] -image hydr_w -anchor e]}
#w {set myportId [$c create image [expr $x + 0.0] [expr $y + -26.0] -image hydr_n -anchor s]}
#}
#$c addtag id$myportId withtag $myportId
#$c addtag id$mymodId withtag $myportId
#$c addtag port1 withtag $myportId
#$c addtag port withtag $myportId
#$c addtag hydr_ptype withtag $myportId; #port type
#$c addtag output withtag $myportId; #port direction
#global port1.$mymodId
#set port1.$mymodId { WU_1 PCOL ____ ____ ____ HCOL ____ HU_1 }
