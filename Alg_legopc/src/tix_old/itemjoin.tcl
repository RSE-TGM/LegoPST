#####################################################
#
# pix2c
#
#####################################################
proc pix2c {v} {
#attenzione convConst non è più utiizzato!
#	global convConst
#		return [expr ($v/$convConst)*1.9]
#converto pixel a inch e trasformo in centimetri
	return [expr ($v/$::dpimon)*2.54]
}



proc ptsJoin {dirfrom dirto fx fy tx ty} {
	global jshift

	switch $dirto {
		n {set shifttx $tx; set shiftty [expr $ty-$jshift]}
		s {set shifttx $tx; set shiftty [expr $ty+$jshift]}
		e {set shifttx [expr $tx+$jshift]; set shiftty $ty}
		w {set shifttx [expr $tx-$jshift]; set shiftty $ty}
	}

	switch $dirfrom {
		n {set shiftfx $fx; set shiftfy [expr $fy-$jshift]}
		s {set shiftfx $fx; set shiftfy [expr $fy+$jshift]}
		e {set shiftfx [expr $fx+$jshift]; set shiftfy $fy}
		w {set shiftfx [expr $fx-$jshift]; set shiftfy $fy}
	}

	set midx [expr ($tx+$fx)/2.]
	set midy [expr ($ty+$fy)/2.]

	if {($dirfrom == "n") && ($dirto == "s")} {
	# follow the midpoint S rule
		if {([expr $ty-$fy+$jshift*2] < 0.)} {
			return "$shiftfx\c $shiftfy\c $shiftfx\c $midy\c $shifttx\c $midy\c $shifttx\c $shiftty\c"
		} else {
			return "$shiftfx\c $shiftfy\c $midx\c $shiftfy\c $midx\c $shiftty\c $shifttx\c $shiftty\c"
		}
	} elseif {($dirfrom == "s") && ($dirto == "n")} {
	# follow the midpoint S rule
		if {([expr $ty-$fy-$jshift*2] < 0.)} {
			return "$shiftfx\c $shiftfy\c $midx\c $shiftfy\c $midx\c $shiftty\c $shifttx\c $shiftty\c"
		} else {
			return "$shiftfx\c $shiftfy\c $shiftfx\c $midy\c $shifttx\c $midy\c $shifttx\c $shiftty\c"
		}
	} elseif {($dirfrom == "w") && ($dirto == "e")} {
	# follow the midpoint S rule
		if {([expr $tx-$fx+$jshift*2] < 0.)} {
			return "$shiftfx\c $shiftfy\c $midx\c $shiftfy\c $midx\c $shiftty\c $shifttx\c $shiftty\c"
		} else {
			return "$shiftfx\c $shiftfy\c $shiftfx\c $midy\c $shifttx\c $midy\c $shifttx\c $shiftty\c"
		}
	} elseif {($dirfrom == "e") && ($dirto == "w")} {
	# follow the midpoint S rule
		if {([expr $tx-$fx-$jshift*2] < 0.)} {
			return "$shiftfx\c $shiftfy\c $shiftfx\c $midy\c $shifttx\c $midy\c $shifttx\c $shiftty\c"
		} else {
			return "$shiftfx\c $shiftfy\c $midx\c $shiftfy\c $midx\c $shiftty\c $shifttx\c $shiftty\c"
		}
	} elseif {($dirfrom == "n") && ($dirto =="n") && ([expr $ty-$fy] < 0.)} {
	# follow the inverted-diagonal point rule
		return "$shiftfx\c $shiftfy\c $shiftfx\c $shiftty\c $shifttx\c $shiftty\c"
	} elseif {($dirfrom == "s") && ($dirto =="s") && ([expr $ty-$fy] > 0.)} {
	# follow the inverted-diagonal point rule
		return "$shiftfx\c $shiftfy\c $shiftfx\c $shiftty\c $shifttx\c $shiftty\c"
	} elseif {($dirfrom == "e") && ($dirto =="e") && ([expr $tx-$fx] > 0.)} {
	# follow the inverted-diagonal point rule
		return "$shiftfx\c $shiftfy\c $shifttx\c $shiftfy\c $shifttx\c $shiftty\c"
	} elseif {($dirfrom == "w") && ($dirto =="w") && ([expr $tx-$fx] < 0.)} {
	# follow the inverted-diagonal point rule
		return "$shiftfx\c $shiftfy\c $shifttx\c $shiftfy\c $shifttx\c $shiftty\c"
	} elseif {($dirfrom == "n") || ($dirfrom =="s")} {
	# follow the diagonal point rule
		return "$shiftfx\c $shiftfy\c $shifttx\c $shiftfy\c $shifttx\c $shiftty\c"
	} elseif {($dirfrom == "e") || ($dirfrom =="w")} {
	# follow the diagonal point rule
		return "$shiftfx\c $shiftfy\c $shiftfx\c $shiftty\c $shifttx\c $shiftty\c"
	}
}

proc portJoin {itemId c bstart dirstart bend dirend tycon} {
	global portw porth env
	global connlist extname idconn clines cvardef

	#get bstart bbox

	set lc [$c coords $bstart] 
	set bsx [lindex $lc 0]
	set bsy [lindex $lc 1]
	#shift dir-dependent (this parameter depend from the -anchor option of the port)
	switch $dirstart {
		n {set bsx [pix2c [expr $bsx]]; set bsy [pix2c [expr $bsy-$porth]]}
		e {set bsx [pix2c [expr $bsx+$portw]]; set bsy [pix2c [expr $bsy]]}
		s {set bsx [pix2c [expr $bsx]]; set bsy [pix2c [expr $bsy+$porth]]}
		w {set bsx [pix2c [expr $bsx-$portw]]; set bsy [pix2c [expr $bsy]]}
	}

	#get bend bbox
	set lc [$c coords $bend] 
	set bex [lindex $lc 0]
	set bey [lindex $lc 1]
	#shift dir-dependent
	switch $dirend {
		n {set bex [pix2c [expr $bex]]; set bey [pix2c [expr $bey-$porth]]}
		e {set bex [pix2c [expr $bex+$portw]]; set bey [pix2c [expr $bey]]}
		s {set bex [pix2c [expr $bex]]; set bey [pix2c [expr $bey+$porth]]}
		w {set bex [pix2c [expr $bex-$portw]]; set bey [pix2c [expr $bey]]}
	}

	set cucu [ptsJoin $dirstart $dirend $bsx $bsy $bex $bey]
	catch {$c delete $itemId}
	if {$clines($tycon,bitmap) == "None"} {
		eval "$c create line $bsx\c $bsy\c $cucu $bex\c $bey\c -width $clines($tycon,width) -tags {$itemId connection}"
	} else {
		eval "$c create line $bsx\c $bsy\c $cucu $bex\c $bey\c -width $clines($tycon,width) -stipple @[file join $env(LG_PIXMAPS) broken.bmp]  -tags {$itemId connection}"
	}
}

proc portFollow {itemId c bstart dirstart bend dirend} {
	global portw porth

	#get bstart bbox
	set lc [$c coords $bstart] 
	set bsx [lindex $lc 0]
	set bsy [lindex $lc 1]
	#shift dir-dependent
	switch $dirstart {
		n {set bsx [pix2c [expr $bsx]]; set bsy [pix2c [expr $bsy-$porth]]}
		e {set bsx [pix2c [expr $bsx+$portw]]; set bsy [pix2c [expr $bsy]]}
		s {set bsx [pix2c [expr $bsx]]; set bsy [pix2c [expr $bsy+$porth]]}
		w {set bsx [pix2c [expr $bsx-$portw]]; set bsy [pix2c [expr $bsy]]}
	}

	#get bend bbox
	set lc [$c coords $bend] 
	set bex [lindex $lc 0]
	set bey [lindex $lc 1]
	#shift dir-dependent
	switch $dirend {
		n {set bex [pix2c [expr $bex]]; set bey [pix2c [expr $bey-$porth]]}
		e {set bex [pix2c [expr $bex+$portw]]; set bey [pix2c [expr $bey]]}
		s {set bex [pix2c [expr $bex]]; set bey [pix2c [expr $bey+$porth]]}
		w {set bex [pix2c [expr $bex-$portw]]; set bey [pix2c [expr $bey]]}
	}

	set cucu [ptsJoin $dirstart $dirend $bsx $bsy $bex $bey]
	eval "$c coords $itemId $bsx\c $bsy\c $cucu $bex\c $bey\c"
}
