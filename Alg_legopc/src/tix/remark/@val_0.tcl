# @val_0 - elemento "valore libero" animabile
# Clone di @com_0 ma senza testo statico: e' una casella valore la cui
# variabile da animare e' definibile a run-time (doppio-click in Show Value).
# IMPORTANTE: l'ordine dei tag 0..7 deve restare IDENTICO a @com_0 perche'
# diverso codice (leggi_font, legopc.tix) usa indici posizionali fissi
# (0=id, 2=cls, 3=ori, 5=lpath, 7=font). Il tag distintivo "freeval" va
# quindi aggiunto PER ULTIMO (indice 8), senza spostare gli altri.

	set mymodId [$c create text $x $y -text "--?--"]

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
	# tag distintivo dell'elemento valore-libero: AGGIUNTO PER ULTIMO
	$c addtag freeval withtag $mymodId
