# bgelement.tcl - script CONDIVISO per gli elementi grafici di background.
#
# Vive tra i file della UI (LG_TIX), NON nelle directory delle risorse grafiche:
# e' unico per QUALSIASI immagine di background di QUALSIASI libreria. E' generico
# perche' usa $idclass (la classe dell'elemento, es. @alb_0) impostato dal
# chiamante. Viene sorgiato da legopc (itemAdd / itemAddFromfile / topRead) via
# l'helper elementScript quando l'elemento e' '@...' e non ha un <nome>.tcl
# dedicato: cosi' per aggiungere un decoro basta mettere la sua GIF @<nome>_0n.gif.
#
# Nota: e' sorgiato NELLO SCOPE del chiamante (come gli script dei moduli), quindi
# usa i suoi locali ($c $x $y $idclass $GIForient $fromfile $mlpath/$curLibPath
# $ff_progNumb) e imposta $mymodId per lui.
#
# Crea un item "image" NON topologico (nome con '@' -> escluso da F01), niente
# porte, non animabile. Tag 0..6 IDENTICI a @com_0 (0=id,1=module,2=cls,3=ori,
# 4=remarkdescr,5=lpath,6=name): codice a indici posizionali fissi. Il tag
# distintivo "bgimage" va aggiunto PER ULTIMO.

	set mymodId [$c create image $x $y -image $idclass$GIForient]

	$c addtag id$mymodId     withtag $mymodId
	$c addtag module         withtag $mymodId
	$c addtag $idclass.cls   withtag $mymodId
	$c addtag $GIForient.ori withtag $mymodId
	$c addtag remarkdescr    withtag $mymodId
	if {$fromfile == "yes"} {
		$c addtag $mlpath.lpath   withtag $mymodId
	} else {
		$c addtag $curLibPath.lpath withtag $mymodId
	}
	if {$fromfile == "yes"} then {set progName $ff_progNumb} else {inputModName $c $x $y}
	$c addtag $progName.name withtag $mymodId
	incr progNumb
	# tag distintivo dell'immagine di background: AGGIUNTO PER ULTIMO
	$c addtag bgimage withtag $mymodId
