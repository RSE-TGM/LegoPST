proc connettori {} {
global env help_figlio
set connector_asiajm {}
set short_name_alsiajm {}
set token 1
set file_to_connect [checkopen $env(LG_LIBGRAPH)/connect.dat r]
while {[gets $file_to_connect line] != -1} {
	if {$token == 1 & $line != "****"} { 
		lappend connector_asiajm [lindex $line 1]
		lappend short_name_alsiajm [string range $line 0 3] }
	if {$line == "****"} {
		set token 1
	} else {
		set token 0
	}
}
close $file_to_connect
foreach j $connector_asiajm k $short_name_alsiajm {
	set help_figlio($k) $j
}

}

set tbox .tbox
catch {destroy $tbox}
toplevel $tbox
wm title $tbox "toolbox"

global help_figlio
cd $env(LG_PIXMAPS)
set items [lsort [glob actconn.gif ????e.gif ????w.gif ????s.gif ????n.gif zrotate?.gif zxdelete.gif]]
connettori
set help_figlio(zrotatel) "rotate left"
set help_figlio(zrotater) "rotate right"
set help_figlio(zxde) "cancella"
set row 0
set column 0
foreach item $items {
	set connoext [file rootname $item]
	image create photo $connoext -file $connoext.gif
	if {$connoext != "actconn"} {
		radiobutton $tbox.$connoext -image $connoext -indicatoron 0 -variable curCTool -value $connoext
		grid $tbox.$connoext -row $row -column $column
                bind $tbox.$connoext <Button-3> {
				set window_name [lindex [split %W .] end]
				toplevel .tch
				wm maxsize .tch 10 2
				wm title .tch "$window_name"
				text .tch.txt -relief sunken -bd 2 -setgrid 1 -background yellow -font {Courier 14}
				set tmp [string range $window_name 0 3]
				if {$tmp == "zrot"} {set tmp $window_name}
				.tch.txt insert end $help_figlio($tmp)
				pack .tch.txt -expand yes -fill both
		 	}
		 	bind $tbox.$connoext <ButtonRelease-3> "destroy .tch"
		incr column
		if {$column == "4"} {set column 0; incr row}

		set curCTool $connoext
	}
}

cd $env(LG_TIX)

# connector info setting
set count 0
set connIFOFvars {}

foreach conn $connlist {
 foreach fvar [lindex $cvardef $count] {
	set ${conn}IFvar($fvar) ____
	set ${conn}OFvar($fvar) ____
 }
 lappend connIFOFvars ${conn}IFvar
 lappend connIFOFvars ${conn}OFvar
 incr count
}
