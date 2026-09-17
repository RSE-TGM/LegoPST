# @set_0 - elemento operatore: invio di un valore a una variabile di ingresso durante la simulazione.
# Clone di @val_0: e' un testo non topologico ('@' nel nome) il cui
# significato - la variabile (di tipo IN) - sta nel file .remap del modello, per nome istanza, e
# si assegna sia da Model Topology sia in View -> Show Value. Il
# comportamento (bottone disegnato, menu del tasto destro, azioni) e' in
# $LG_TIX/hmielem.tcl.
# IMPORTANTE: l'ordine dei tag 0..7 deve restare IDENTICO a @com_0/@val_0
# perche' diverso codice (leggi_font, legopc.tix) usa indici posizionali
# fissi (0=id, 2=cls, 3=ori, 5=lpath, 7=font). Il tag distintivo "hmiset"
# va quindi aggiunto PER ULTIMO (indice 8), senza spostare gli altri.
# Solo ASCII nel testo: con LANG=POSIX Tcl non decodifica UTF-8.

	set mymodId [$c create text $x $y -text {[ set: ? ]}]

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
#crezione tag per il font: il colore (terzo campo) va nel .tom con il font,
#altrimenti topRead lo riporterebbe a nero
        global font.$mymodId
        set font.$mymodId [list helvetica 12 #8b0000]
        $c itemconfigure $mymodId  -font "helvetica 12" -fill "#8b0000"
	# tag distintivo dell'elemento: AGGIUNTO PER ULTIMO
	$c addtag hmiset withtag $mymodId
