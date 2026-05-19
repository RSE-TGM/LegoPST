
if {![file exists $env(LG_LIBGRAPH)/connect.dat]} {
	switchuser
    } else {
    set f [checkopen $env(LG_LIBGRAPH)/connect.dat r]
}

set count 0
set idcount 0
set connlist {}
set cvardef {}
set vars {}
while {[gets $f line] >= 0} {
    if {[string range $line 0 3] == "****"} {
	 if {[llength vars] > 0} {
		lappend cvardef $vars
	 }
	 set count 0
    } else {
	 incr count
 # connector name
	 if {$count == 1} {
		scan $line "%s %s" name ext_nam
		lappend connlist $name
		set extname($name) $ext_nam
		set vars {}
		set idconn($name) $idcount
		incr idcount 2
 # line characteristics
	 } elseif {$count > 1 && $count < 5} {
		scan $line "%s %s" caract value
		if {$caract == "LineColor"} {
			set clines($name,color) $value
			set clines($name,active) $value
		} elseif {$caract == "LineWidth"} {
			set clines($name,width) $value
		} elseif {$caract == "LineBitmap"} {
			set clines($name,bitmap) $value
		}
 # variables
	 } else {
 		 scan $line "%s" var
	 	 lappend vars $var
 	 }
    }
}
close $f