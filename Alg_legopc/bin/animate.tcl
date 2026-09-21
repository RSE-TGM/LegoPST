set ::indicatore_after 0
set ::pipeon 0
## ::anima_sim_path = dir della simulazione usata da viewval/graphics/xaing.
## Se LG_SIM_PATH e' impostata e punta a una dir valida (es. da `lghmi -loc`,
## che vi mette la cwd di lancio) la si usa come default -> evita il
## "Set Sim path" manuale nel menu View.
if {[info exists ::env(LG_SIM_PATH)] && $::env(LG_SIM_PATH) ne "" && [file isdirectory $::env(LG_SIM_PATH)]} {
    set ::anima_sim_path $::env(LG_SIM_PATH)
} else {
    set ::anima_sim_path "locpath - click to change"
}

# Inizializza strutture remap se non già definite (animate.tcl può essere
# sourciato sia da draw2gr.tcl che da legopc.tix — in quest'ultimo caso
# le proc anim_* di draw2gr.tcl non sono disponibili).
if {![info exists ::anim_remap]} { array set ::anim_remap {} }
if {![info exists ::anim_mode]}  { array set ::anim_mode {} }
if {![info exists ::anim_selected_item]} { set ::anim_selected_item -1 }

# Carica il file .remap associato al modello corrente.
# Usa curFileName (globale in legopc.tix e draw2gr.tcl).
# Se il file non esiste o contiene righe invalide, le ignora silenziosamente.
proc anim_load_remap {} {
    global curFileName tipVarMod
    array unset ::anim_remap
    array unset ::anim_mode
    if {![info exists curFileName]} return
    set fname "[file rootname $curFileName].remap"
    if {![file exists $fname]} return

    # Controlla che tipVarMod sia popolato (loadF01 già eseguito).
    # Se è vuoto non possiamo validare le variabili: carichiamo senza
    # validare e NON riscriviamo il file (per non cancellarlo).
    set can_validate [expr [array size tipVarMod] > 0]

    set had_stale 0
    catch {
        set fid [open $fname r]
        while {[gets $fid line] >= 0} {
            set line [string trim $line]
            if {[string equal $line ""] || \
                [string equal [string index $line 0] "#"]} continue
            set eqpos [string first "=" $line]
            if {$eqpos < 1} continue
            set inst [string trim [string range $line 0 [expr $eqpos-1]]]
            set rhs  [string trim [string range $line [expr $eqpos+1] end]]
            # Token opzionale ";L" (solo elementi freeval): modalità tag+valore
            set mode ""
            set semipos [string first ";" $rhs]
            if {$semipos >= 0} {
                set mode [string trim [string range $rhs [expr $semipos+1] end]]
                set var  [string trim [string range $rhs 0 [expr $semipos-1]]]
            } else {
                set var $rhs
            }
            if {[string equal $inst ""] || [string equal $var ""]} continue
            # ";F" (faceplate @stz_0, hmielem.tcl): il valore e' una pagina di
            # r02.dat, non una variabile, e non si valida contro il modello
            if {$can_validate && $mode ne "F" && ![info exists tipVarMod($var)]} {
                # variabile non più nel modello → riga obsoleta, salta
                set had_stale 1
                continue
            }
            set ::anim_remap($inst) $var
            if {$mode ne ""} { set ::anim_mode($inst) $mode }
        }
        close $fid
    }
    # Riscrive il file ripulito SOLO se tipVarMod era disponibile
    # (evita di cancellare il file quando chiamata troppo presto)
    if {$had_stale && $can_validate} { anim_save_remap }
}

# Salva ::anim_remap nel file .remap (accanto al .tom corrente).
# Chiamata sia da draw2gr.tcl (al remap interattivo) che da anim_load_remap
# (pulizia automatica voci obsolete).
proc anim_save_remap {} {
    global curFileName
    if {![info exists curFileName]} return
    set fname "[file rootname $curFileName].remap"
    catch {
        set fid [open $fname w]
        puts $fid "# LegoPC animation remap - generato automaticamente"
        foreach inst [lsort [array names ::anim_remap]] {
            set line "$inst=$::anim_remap($inst)"
            # token modalità tag+valore (solo elementi freeval @val_0)
            if {[info exists ::anim_mode($inst)] && $::anim_mode($inst) ne ""} {
                append line ";$::anim_mode($inst)"
            }
            puts $fid $line
        }
        close $fid
    }
}

# Il .remap su disco, senza validare e senza toccare le strutture in memoria:
# un dict {istanza -> {valore modo}}. Vuoto se il file non c'e'.
proc anim_remap_leggi {{fname ""}} {
    global curFileName
    set d [dict create]
    #  senza argomento il .remap del modello aperto; con un path quel file
    #  (Include Model legge quello del modello incluso, modelli.tcl)
    if {$fname eq ""} {
        if {![info exists curFileName] || $curFileName eq ""} { return $d }
        set fname "[file rootname $curFileName].remap"
    }
    if {[catch {open $fname r} fid]} { return $d }
    while {[gets $fid line] >= 0} {
        set line [string trim $line]
        if {$line eq "" || [string index $line 0] eq "#"} continue
        set eqpos [string first "=" $line]
        if {$eqpos < 1} continue
        set inst [string trim [string range $line 0 [expr {$eqpos-1}]]]
        set rhs  [string trim [string range $line [expr {$eqpos+1}] end]]
        set mode ""
        set semipos [string first ";" $rhs]
        if {$semipos >= 0} {
            set mode [string trim [string range $rhs [expr {$semipos+1}] end]]
            set rhs  [string trim [string range $rhs 0 [expr {$semipos-1}]]]
        }
        if {$inst eq "" || $rhs eq ""} continue
        dict set d $inst [list $rhs $mode]
    }
    close $fid
    return $d
}

# Cambia UNA voce del .remap (valore "" = la toglie) e la riporta in memoria.
# Rilegge il file prima di riscriverlo: anim_save_remap scrive le strutture in
# memoria, che fuori da Show Value possono mancare o essere di un altro
# modello, e riscrivere quelle cancellerebbe le altre voci. Ritorna 1 se il
# file e' stato scritto.
proc anim_remap_set {inst valore {mode ""}} {
    global curFileName
    if {![info exists curFileName] || $curFileName eq "" \
        || $curFileName eq "untitled" || $curFileName eq "-"} { return 0 }
    set fname "[file rootname $curFileName].remap"
    if {![file isdirectory [file dirname $fname]]} { return 0 }
    set d [anim_remap_leggi]
    if {$valore eq ""} {
        dict unset d $inst
        catch {unset ::anim_remap($inst)}
        catch {unset ::anim_mode($inst)}
    } else {
        dict set d $inst [list $valore $mode]
        set ::anim_remap($inst) $valore
        if {$mode ne ""} {
            set ::anim_mode($inst) $mode
        } else {
            catch {unset ::anim_mode($inst)}
        }
    }
    if {[catch {
        set fid [open $fname w]
        puts $fid "# LegoPC animation remap - generato automaticamente"
        foreach k [lsort [dict keys $d]] {
            lassign [dict get $d $k] v m
            puts $fid [expr {$m ne "" ? "$k=$v;$m" : "$k=$v"}]
        }
        close $fid
    }]} { return 0 }
    return 1
}

# Ritorna la variabile animata effettiva per una data istanza:
# se esiste il remap usa quello, altrimenti usa il default.
# La validazione contro tipVarMod è già fatta da anim_load_remap al caricamento;
# qui ci fidiamo del remap in memoria senza ri-controllare.
proc anim_get_var { pisqu_name pisqu_default } {
    if {[info exists ::anim_remap($pisqu_name)]} {
        return $::anim_remap($pisqu_name)
    }
    return $pisqu_default
}

# ============================================================
# REMAP dal doppio clic sul campo di un blocco (Show Value)
# ============================================================
# Doppio clic sul campo giallo (o azzurro, a simulazione ferma) sotto l'icona
# di un blocco: si sceglie quale variabile del blocco mostrare, e la scelta
# va nel .remap (anim_remap_set). La scelta la fa ciascun programma a modo
# suo, il resto e' comune:
#   draw2gr  anim_field_select (draw2gr.tcl): il campo si evidenzia e si apre
#            l'elenco delle variabili del blocco nel pannello del Plot; il
#            clic su una variabile (setSlot) chiama anim_apply_remap;
#   legopc   non ha quel pannello: anim_field_dialog apre un dialogo con le
#            stesse variabili, nell'elenco filtrabile degli altri dialoghi.
#
# ::anim_selected_item  canvas id del modulo del campo selezionato (-1 = nessuno)
# ::anim_selected_rect  canvas id del rettangolo del campo
# ::anim_selected_mod   nome (4 caratteri) del blocco
# ::anim_selected_fill  colore del campo prima dell'evidenziazione (giallo dal
#                       vivo, azzurro statico), da rimettere dopo
#  una per una: ::anim_selected_item la inizializza gia' la testa del file, e
#  draw2gr imposta le sue dopo aver sorgiato questo file
foreach {_anim_v _anim_i} {item -1 rect -1 mod "" fill yellow} {
    if {![info exists ::anim_selected_$_anim_v]} { set ::anim_selected_$_anim_v $_anim_i }
}
unset _anim_v _anim_i

#  Toglie dai moduli i tag *.visual e *.nome_anim dei campi di prima: Show
#  Value li rimette a ogni avvio, e un modulo che ne porta due - il vecchio
#  punta a una casella gia' cancellata - fa leggere a chi cerca il primo (il
#  ciclo dal vivo, anim_apply_remap) quello sbagliato. Succedeva riaccendendo
#  Show Value a simulazione ferma (il modo 3 non ripuliva niente; il modo 1
#  toglieva solo il primo *.visual) o dopo un remap scritto da un'altra
#  applicazione (il nome_anim vecchio restava davanti al nuovo).
proc anim_togli_campi_vecchi {c} {
    foreach item [$c find withtag module] {
        foreach t [$c gettags $item] {
            if {[string match *.visual $t] || [string match *.nome_anim $t]} {
                $c dtag $item $t
            }
        }
    }
}

proc anim_field_remap {c item rect modtags} {
    if {[info procs anim_field_select] ne ""} {
        anim_field_select $c $item $rect $modtags
    } else {
        anim_field_dialog $c $item $rect $modtags
    }
}

#  Evidenzia il campo <rect> del modulo <item> (bordo verde, sfondo lime) e lo
#  ricorda come selezionato; il campo selezionato prima torna com'era.
proc anim_field_evidenzia {c item rect modtags} {
    anim_field_rilascia $c
    set ::anim_selected_item $item
    set ::anim_selected_rect $rect
    set nome [file rootname [lindex $modtags [lsearch $modtags *.name]]]
    set ::anim_selected_mod [string range $nome 0 3]
    set ::anim_selected_fill [$c itemcget $rect -fill]
    $c itemconfigure $rect -fill "#CCFF99" -outline "#00AA00"
}

#  Il campo selezionato torna del suo colore e nessun campo e' piu' selezionato.
proc anim_field_rilascia {c} {
    if {$::anim_selected_rect != -1} {
        catch {$c itemconfigure $::anim_selected_rect \
                   -fill $::anim_selected_fill -outline yellow}
    }
    set ::anim_selected_item -1
    set ::anim_selected_rect -1
    set ::anim_selected_mod  ""
}

#  Applica il remap al campo selezionato: <name> e' la variabile scelta.
#  Comune a draw2gr (setSlot) e legopc (anim_field_dialog).
proc anim_apply_remap { c name } {
    global tipVarMod

    set item $::anim_selected_item
    if {$item == -1} return

    # Sicurezza: la variabile deve esistere in F01
    if {![info exists tipVarMod($name)]} return

    # Ricava nome istanza dal tag *.name del modulo
    set tags_curr [$c gettags $item]
    set pisqu_name [file rootname [lindex $tags_curr [lsearch $tags_curr *.name]]]

    # Aggiorna il remap in memoria e su disco. anim_remap_set rilegge il file
    # prima di riscriverlo: legopc e draw2gr possono avervi scritto nel
    # frattempo (pagine e variabili degli elementi operatore, altri remap), e
    # la riscrittura della sola memoria cancellerebbe quelle voci.
    catch { anim_remap_set $pisqu_name $name }

    # Aggiorna tag *.nome_anim sull'item (letto dal loop mode 2): via tutti
    # quelli che ha, non solo il primo
    foreach t $tags_curr {
        if {[string match *.nome_anim $t]} { $c dtag $item $t }
    }
    $c addtag ${name}.nome_anim withtag $item

    # Aggiorna subito il testo visibile: dal vivo lo rinfresca il prossimo
    # ciclo (1 s); a simulazione ferma non passa nessuno, e si mette il valore
    # di stazionario, come anima_aggiorna modo 3
    # la casella: il tag *.visual il cui item esiste ancora
    set pisqu_tid ""
    foreach t [$c gettags $item] {
        if {[string match *.visual $t] && [$c find withtag [file rootname $t]] ne ""} {
            set pisqu_tid [file rootname $t]
        }
    }
    set testo "$name ..."
    if {(![info exists ::pipeon] || !$::pipeon) && [info exists ::matrVf14($name,valu)]} {
        catch {set testo "[conv_umis $name $::matrVf14($name,valu)] [ret_umis $name]"}
    }
    catch { $c itemconfigure $pisqu_tid -text $testo }

    anim_field_rilascia $c
}

#  legopc: la variabile del blocco da mostrare, scelta in un dialogo. Le
#  variabili sono quelle del blocco nel F01 caricato (blocNvar/blocVars, le
#  stesse che loadVariables mostra in draw2gr), lette senza passare da
#  loadVariables, che riscrive le globali del pannello dei dati.
proc anim_field_dialog {c item rect modtags} {
    set inst [file rootname [lindex $modtags [lsearch $modtags *.name]]]
    set blocco [string range $inst 0 3]
    set default "[file rootname [lindex $modtags [lsearch $modtags *.anim]]]$inst"
    if {![info exists ::blocNvar($blocco)]} {
        tk_messageBox -parent [winfo toplevel $c] -icon warning -title "Variable to show" \
            -message "Block $blocco not found in the F01 loaded: build F01/F14 first."
        return
    }
    set nomi {}
    array unset ::anim_scelta_info
    for {set i 0} {$i < $::blocNvar($blocco)} {incr i} {
        set n $::blocVars($blocco,$i,nome)
        lappend nomi $n
        set ::anim_scelta_info($n) [list $::blocVars($blocco,$i,tipo) $::blocVars($blocco,$i,desc)]
    }

    anim_field_evidenzia $c $item $rect $modtags
    set attuale [anim_get_var $inst $default]
    set ::anim_var_nome ""

    set w .anim_var_blocco
    catch {destroy $w}
    toplevel $w
    wm title $w "Variable to show - $inst"
    wm transient $w [winfo toplevel $c]
    wm protocol $w WM_DELETE_WINDOW [list anim_field_dialog_via $c $w]
    label $w.info -anchor w -justify left -text \
        "Variable shown under block $inst ([llength $nomi] variables).\nNow: $attuale    Default: $default"
    frame $w.f
    label $w.f.l -text "Variable:"
    entry $w.f.e -textvariable ::anim_var_nome -width 16
    pack $w.f.l -side left
    pack $w.f.e -side left -fill x -expand 1
    set ok [list anim_field_dialog_ok $c $w]
    #  il filtro parte vuoto, cosi' si vedono tutte le variabili del blocco;
    #  quella di adesso e' selezionata
    set lb [hmi_lista $w.l $w.f.e ::anim_var_nome $nomi $ok anim_riga_blocco \
                [format "%-10s %-3s %s" Name Typ Description]]
    set i [lsearch -exact $nomi $attuale]
    if {$i >= 0} { $lb selection set $i ; $lb see $i }
    frame $w.b
    button $w.b.ok -text OK -width 10 -command $ok
    button $w.b.no -text Cancel -width 10 -command [list anim_field_dialog_via $c $w]
    pack $w.b.ok $w.b.no -side left -padx 6
    pack $w.info -side top -fill x -padx 8 -pady {8 4}
    pack $w.f -side top -fill x -padx 8 -pady 4
    pack $w.b -side bottom -pady 8
    pack $w.l -side top -fill both -expand 1 -padx 8
    bind $w.f.e <Return> $ok
    bind $w <Escape> [list anim_field_dialog_via $c $w]
    focus $w.f.e
}

#  Una riga dell'elenco delle variabili del blocco: nome, tipo, descrizione.
proc anim_riga_blocco {n} {
    lassign $::anim_scelta_info($n) tipo desc
    return [format "%-10s %-3s %s" $n $tipo $desc]
}

#  OK: la variabile scritta nel campo (anche in minuscolo) o, a campo vuoto,
#  la riga selezionata; deve essere del blocco.
proc anim_field_dialog_ok {c w} {
    set t [string trim $::anim_var_nome]
    if {$t eq ""} {
        set lb $w.l.lb
        set sel [$lb curselection]
        if {[llength $sel]} { set t [lindex [hmi_lista_mostrati $lb] [lindex $sel 0]] }
    }
    set nome ""
    foreach n [array names ::anim_scelta_info] {
        if {[string equal -nocase $n $t]} { set nome $n; break }
    }
    if {$nome eq ""} {
        tk_messageBox -parent $w -icon warning -title "Variable to show" -message \
            [expr {$t eq "" ? "Choose a variable from the list." \
                            : "'$t' is not a variable of this block: choose one from the list."}]
        return
    }
    destroy $w
    anim_apply_remap $c $nome
}

#  Cancel, Escape o chiusura: il campo torna com'era.
proc anim_field_dialog_via {c w} {
    catch {destroy $w}
    anim_field_rilascia $c
}

# Mostra balloon con il nome della variabile animata (tasto destro premuto).
proc anim_show_balloon { x y pisqu_name pisqu_default } {
    set varname [anim_get_var $pisqu_name $pisqu_default]
    catch {destroy .anim_balloon}
    toplevel .anim_balloon
    wm overrideredirect .anim_balloon 1
    wm geometry .anim_balloon +[expr {$x+12}]+[expr {$y+12}]
    label .anim_balloon.lbl -text $varname \
          -background "#FFFFE0" -relief solid -bd 1 \
          -font "Helvetica 9" -padx 4 -pady 2
    pack .anim_balloon.lbl
    raise .anim_balloon
}

# Nasconde il balloon (tasto destro rilasciato).
proc anim_hide_balloon {} { catch {destroy .anim_balloon} }

proc anima { vai variab } {
switch $vai {

	init
        	{
#tk_messageBox -message "anima: START Animazione"
        		set ::pipeon 0

			if { $::LINUXPLAT == 0 } {
                # In modalità lgser-FMU passa il numero SHM a sincview
                # sincview -pipe {n} → SharedLego{n}  (lgser diretto)
                # sincview -pipe    → SharedLego1      (lgsincro, default)
                if { [info exists ::lgser_clientnum] && $::lgser_clientnum >= 0 } {
                    set viewer "sincview.exe -pipe $::lgser_clientnum"
                } else {
                    set viewer "sincview.exe -pipe"
                }
			} else {set viewer "viewval -s"}
            set olddir [pwd]
			if { $::anima_sim_path != 0 } { catch {cd $::anima_sim_path} }

puts stdout "anima- path=$::anima_sim_path"

			if {[catch {open "| $viewer" r+} ::pipeanim]} {
tk_messageBox -message "anima: 1 Error opening pipe: $::pipeanim"
                catch {hmi_stato "Show Value: cannot start $viewer ($::pipeanim)"}
  			set ::pipeanim 0
  			set ::pipeon 0
  			} else {
#tk_messageBox -message "anima: 2 pipe attiva ::pipeanim=$::pipeanim"
  			set ::pipeon 1
# Configure reader for ::pipeanim
       		fconfigure $::pipeanim -buffering line
			catch {gets $::pipeanim ::line_anim}
# se la pipe è finita (eof) allora l'apertura non ha avuto successo per mancanza della shared memory
            if [eof $::pipeanim] { set ::pipeon 0 }
#puts stdout $::line_anim
#tk_messageBox -message "anima: 3 else in init, successo nell'apertura della pipe : $::line_anim"
			}
			cd $olddir
		return }

	leggi
	       {
#puts stdout "anima: -pipeon=$::pipeon- leggo $variab pipeanim=$::pipeanim"
    	    if { $::pipeon == 0 } { return }
    		catch {puts $::pipeanim $variab}
		    catch {flush $::pipeanim }
    		catch {gets $::pipeanim ::line_anim}
#puts stdout "anima leggi: 2-pipeon=$::pipeon- leggo2 $variab pipeanim=$::pipeanim letto=$::line_anim"
#tk_messageBox -message "anima:  la variabile $variab: $::line_anim"
        	return }

     chiudi
           {
#fermo sincview...
            catch {after cancel anima_aggiorna}
            catch {after cancel $::indicatore_after}
    		set variab "%STOP%"
    		catch { puts $::pipeanim $variab }
		    catch {flush $::pipeanim }
			catch {close $::pipeanim }
    		set ::pipeon 0
			set ::pipeanim 0
    		set ::done 1
#tk_messageBox -message "anima: STOP Animazione"
		    return }

	}
}


proc anima_aggiorna { c mod } {
#        global pipeon
global  matrVf14
update
set refr_anim_ms 1000

# Font scalato al livello di zoom corrente del canvas (stesso meccanismo di ShowNames)
set baseFont {Helvetica 8}
set zl [expr {[info exists ::zoomLevelOf($c)] ? $::zoomLevelOf($c) : 1.0}]
set scaledSize [expr {int(round(8 * $zl))}]
if {$scaledSize < 1} { set scaledSize 1 }
set scaledFont [list Helvetica $scaledSize]

# (Ri)carica la tabella delle unita' di misura ad ogni attivazione (modi
# init): serve al path statico F14 (conversione lato Tcl); il path live e'
# gia' convertito da viewval con lo stesso file unita'.
if { $mod == 1 || $mod == 3 } { catch {umis_load} }

#tk_messageBox  -message "anima_ggiorn- modo=$mod - indicatore_after=$::indicatore_after"

if { $mod == 1 } {

# Carica (o ricarica) il file .remap per il modello corrente.
# Funziona sia da draw2gr.tcl che da legopc.tix grazie a curFileName globale.
anim_load_remap

# reset: via i tag dei campi creati prima (anim_togli_campi_vecchi)
anim_togli_campi_vecchi $c

        set nelem 0
        foreach item  [$c find withtag module] {
        set esiste 0
		set lc [$c bbox $item]
		set x [expr [expr [lindex $lc 0]+[lindex $lc 2]]/2]
#in alto a destra
#		set y [lindex $lc 1]
#in basso a destra
		set y [expr [lindex $lc 3] + 6]
#GUAG - nov 2007 - continuare qui per variabile di anim nuove,,,
        set tags_curr [$c gettags $item]
#puts "anima_aggiorna:  elemento attuale  ---------> $item ---------> $tags_curr"
        # --- Elemento valore libero (@val_0): gestione dedicata ---
        if {[lsearch -exact $tags_curr  freeval] != -1} {
            anima_freeval_field $c $item $x $y $scaledFont $baseFont 1
            incr nelem
            continue
        }
        # --- Elementi operatore (@stz_0/@set_0): hmi_campi, dopo il ciclo ---
        if {[lsearch -exact $tags_curr hmistaz] != -1 || \
            [lsearch -exact $tags_curr hmiset] != -1} { continue }
        set is_remark 0
        if {[lsearch -exact $tags_curr  remarkdescr] != -1} {
# è un remark...
             if {[lsearch -exact $tags_curr  da_animare] != -1} {
# è un remark da animare...
             set pisqu [file rootname [lindex $tags_curr [lsearch $tags_curr *.anim]]]
             # per il balloon: la variabile è direttamente pisqu (nessun remap per remark)
             set pisqu_name $pisqu
             set pisqu_default $pisqu
             set is_remark 1
             set esiste 1
             }
        } else {
               set pisqu_name [file rootname [lindex $tags_curr [lsearch $tags_curr *.name]]]
               set pisqu_default [file rootname [lindex $tags_curr [lsearch $tags_curr *.anim]]]$pisqu_name
               # Applica remap persistente (se presente e valido)
               set pisqu [anim_get_var $pisqu_name $pisqu_default]
               set esiste 1
               }
       if { $esiste == 1 } {
           $c addtag $pisqu.nome_anim withtag $item
           $c addtag da_animare withtag $item
	       set variab_inv $pisqu\n
           anima leggi $variab_inv
           set pisqu $::line_anim

           if { [regexp -nocase {^ERRORE} $::line_anim] == 1} {
           $c dtag $item da_animare
           } else {
                  set testo "[lindex $pisqu 0] [lindex $pisqu 1]"
                  set tid [$c create text $x $y -text $testo -font $scaledFont -tags {infoitemname}]
                  set ::origFontOf($c,$tid) $baseFont
                $c addtag $tid.visual withtag $item
		        set lc [$c bbox $tid]
	  	        set x1 [lindex $lc 0]
		        set y1 [lindex $lc 1]
		        set x2 [lindex $lc 2]
		        set y2 [lindex $lc 3]

		        set rect [$c create rectangle $x1 $y1 $x2 $y2 \
                              -fill yellow -outline yellow -tags {infoitemname}]
                # Doppio clic = scelta della variabile da mostrare (remap nel
                # .remap), solo per i blocchi normali (non i remark): vedi
                # anim_field_remap
                if { $is_remark == 0 } {
                    $c bind $rect <Double-1> [list anim_field_remap $c $item $rect $tags_curr]
                    $c bind $tid  <Double-1> [list anim_field_remap $c $item $rect $tags_curr]
                }
                # Balloon help (tasto destro): mostra il nome della variabile
                $c bind $rect <ButtonPress-3> \
                    "anim_show_balloon %X %Y {$pisqu_name} {$pisqu_default}"
                $c bind $rect <ButtonRelease-3> "anim_hide_balloon"
                $c bind $tid  <ButtonPress-3> \
                    "anim_show_balloon %X %Y {$pisqu_name} {$pisqu_default}"
                $c bind $tid  <ButtonRelease-3> "anim_hide_balloon"
		        $c raise $tid
		        incr nelem
                 }

          }

        }

       # elementi operatore: faceplate e set value (hmielem.tcl)
       catch {hmi_campi $c [hmi_live]}
##############
       update
       set ::indicatore_after [after $refr_anim_ms anima_aggiorna $c 2]
}



if { $mod == 2 } {
#visualizzazione periodica sui campi creati con la modalita 1
   foreach item [$c find withtag module] {
        set tags_curr [$c gettags $item]

        if {[lsearch -exact $tags_curr  da_animare] != -1} {
           # Il tag *.nome_anim viene aggiornato da anim_apply_remap al volo:
           # leggiamo sempre il valore corrente (include eventuali remap).
           set pisqu [file rootname [lindex $tags_curr [lsearch $tags_curr *.nome_anim]]]
           set varname $pisqu
#identificatore del campo su cui si scriverà il valore...
		   set pisqu_tid [file rootname [lindex $tags_curr [lsearch $tags_curr *.visual]]]
#chiedo al server lego il valore con il nome ( pisqu )
		   set variab_inv $pisqu\n
           anima leggi $variab_inv
#carico in pisqu il valore
           set pisqu $::line_anim

#aggiorno il campo...
           set testo "[lindex $pisqu 0] [lindex $pisqu 1]"

           # freeval in modalità etichetta: anteponi il nome variabile
           if {[lsearch -exact $tags_curr  freeval] != -1} {
               set inst_name [file rootname [lindex $tags_curr [lsearch $tags_curr *.name]]]
               if {[info exists ::anim_mode($inst_name)] && $::anim_mode($inst_name) eq "L"} {
                   set testo "$varname $testo"
               }
           }

		   $c itemconfigure $pisqu_tid -text $testo
		   $c raise $pisqu_tid
	       update
		 }
	   }
# elementi operatore: valori e stato dal vivo/spento (hmielem.tcl)
catch {hmi_aggiorna $c}
set ::indicatore_after [after $refr_anim_ms anima_aggiorna $c 2]
}

if { $mod == 3 } {
# Carica il file .remap anche per la modalità f14 statica (legopc.tix)
anim_load_remap
# reset: via i tag dei campi creati prima, come nel modo 1
anim_togli_campi_vecchi $c
# inizializzazione dei campi delle variabili da visualizzare per il modo F14
        set nelem 0
        foreach item  [$c find withtag module] {
        set esiste 0
		set lc [$c bbox $item]
		set x [expr [expr [lindex $lc 0]+[lindex $lc 2]]/2]
#in alto a destra
#		set y [lindex $lc 1]
#in basso a destra
		set y [expr [lindex $lc 3] + 6]
        set tags_curr [$c gettags $item]
        # --- Elemento valore libero (@val_0): gestione dedicata (statico F14) ---
        if {[lsearch -exact $tags_curr  freeval] != -1} {
            anima_freeval_field $c $item $x $y $scaledFont $baseFont 0
            incr nelem
            continue
        }
        # --- Elementi operatore (@stz_0/@set_0): hmi_campi, dopo il ciclo ---
        if {[lsearch -exact $tags_curr hmistaz] != -1 || \
            [lsearch -exact $tags_curr hmiset] != -1} { continue }
        set is_remark 0
        if {[lsearch -exact $tags_curr  remarkdescr] != -1} {
# è un remark...
             if {[lsearch -exact $tags_curr  da_animare] != -1} {
# è un remark da animare...
             set pisqu [file rootname [lindex $tags_curr [lsearch $tags_curr *.anim]]]
             # per il balloon: la variabile è direttamente pisqu (nessun remap per remark)
             set pisqu_name $pisqu
             set pisqu_default $pisqu
             set is_remark 1
             set esiste 1
             }
        } else {
               set pisqu_name [file rootname [lindex $tags_curr [lsearch $tags_curr *.name]]]
               set pisqu_default [file rootname [lindex $tags_curr [lsearch $tags_curr *.anim]]]$pisqu_name
               # Applica remap persistente anche per valori statici f14
               set pisqu [anim_get_var $pisqu_name $pisqu_default]
               set esiste 1
               }
       if { $esiste == 1 } {
           $c addtag $pisqu.nome_anim withtag $item
           $c addtag da_animare withtag $item
	       set variab_inv $pisqu

           if { [catch { set valore14 $matrVf14($variab_inv,valu)} errmsg ]} {
              set valore14 $errmsg
              }
           # valore f14 in MKS: conversione nell'unita' selezionata (come live)
           set testo "[conv_umis $variab_inv $valore14] [ret_umis $variab_inv]"
           set tid [$c create text $x $y -text $testo -font $scaledFont -tags {infoitemname}]
           set ::origFontOf($c,$tid) $baseFont
           $c addtag $tid.visual withtag $item
		   set lc [$c bbox $tid]
  	   set x1 [lindex $lc 0]
		   set y1 [lindex $lc 1]
		   set x2 [lindex $lc 2]
		   set y2 [lindex $lc 3]
		   set rect [$c create rectangle $x1 $y1 $x2 $y2 \
                         -fill cyan -outline yellow -tags {infoitemname}]
           # Doppio clic = scelta della variabile da mostrare (anim_field_remap)
           if { $is_remark == 0 } {
               $c bind $rect <Double-1> [list anim_field_remap $c $item $rect $tags_curr]
               $c bind $tid  <Double-1> [list anim_field_remap $c $item $rect $tags_curr]
           }
           # Balloon help (tasto destro): mostra il nome della variabile
           $c bind $rect <ButtonPress-3> \
               "anim_show_balloon %X %Y {$pisqu_name} {$pisqu_default}"
           $c bind $rect <ButtonRelease-3> "anim_hide_balloon"
           $c bind $tid  <ButtonPress-3> \
               "anim_show_balloon %X %Y {$pisqu_name} {$pisqu_default}"
           $c bind $tid  <ButtonRelease-3> "anim_hide_balloon"
		   $c raise $tid
		   incr nelem

          }

        }

       # elementi operatore, spenti: senza simulazione non si invia niente
       catch {hmi_campi $c 0}
##############
       update
}


}

# ==========================================================================
# Elementi "valore libero" @val_0 (tag freeval)
# ==========================================================================
#
# Crea/aggiorna il campo animato di un elemento freeval.
#  live = 1 : valore letto dalla pipe (simulazione attiva, modi 1/2)
#  live = 0 : valore statico letto da matrVf14 (modo F14, modo 3)
# La variabile è presa esclusivamente dal file .remap (per nome istanza);
# se non assegnata mostra il placeholder "--?--" e resta comunque cliccabile.
proc anima_freeval_field { c item x y scaledFont baseFont live } {
    global matrVf14

    # rimuovi un eventuale overlay precedente di questo item (idempotente)
    catch { $c delete fvov$item }

    set tags_curr [$c gettags $item]
    set inst_name [file rootname [lindex $tags_curr [lsearch $tags_curr *.name]]]
    set var [anim_get_var $inst_name ""]

    # ripulisci tag di animazione precedenti
    set old_na [lsearch $tags_curr *.nome_anim]
    if {$old_na >= 0} { $c dtag $item [lindex $tags_curr $old_na] }
    set old_vi [lsearch $tags_curr *.visual]
    if {$old_vi >= 0} { $c dtag $item [lindex $tags_curr $old_vi] }
    $c dtag $item da_animare

    if {$var eq ""} {
        set testo "--?--"
    } else {
        $c addtag $var.nome_anim withtag $item
        $c addtag da_animare withtag $item
        if {$live} {
            set variab_inv $var\n
            anima leggi $variab_inv
            if {[regexp -nocase {^ERRORE} $::line_anim] == 1} {
                set valtxt "--?--"
            } else {
                set valtxt "[lindex $::line_anim 0] [lindex $::line_anim 1]"
            }
        } else {
            if {[catch { set valore14 $matrVf14($var,valu) } errmsg]} {
                set valore14 $errmsg
            }
            # valore f14 in MKS: conversione nell'unita' selezionata (come live)
            set valtxt "[conv_umis $var $valore14] [ret_umis $var]"
        }
        if {[info exists ::anim_mode($inst_name)] && $::anim_mode($inst_name) eq "L"} {
            set testo "$var $valtxt"
        } else {
            set testo $valtxt
        }
    }

    # Posiziona il campo valore SOPRA il placeholder dell'elemento, così da
    # nasconderlo. Stile come i blocchi: niente bordo, giallo in simulazione
    # (live), azzurro nella visualizzazione dei valori di stazionario (statico).
    set bb_base [$c bbox $item]
    set cx [expr {([lindex $bb_base 0]+[lindex $bb_base 2])/2.0}]
    set cy [expr {([lindex $bb_base 1]+[lindex $bb_base 3])/2.0}]

    set tid [$c create text $cx $cy -text $testo -font $scaledFont \
                 -tags [list infoitemname fvov$item]]
    set ::origFontOf($c,$tid) $baseFont
    $c addtag $tid.visual withtag $item

    # rettangolo opaco senza bordo, dimensionato sull'unione (placeholder + valore)
    set tb [$c bbox $tid]
    set x1 [expr {min([lindex $bb_base 0],[lindex $tb 0]) - 1}]
    set y1 [expr {min([lindex $bb_base 1],[lindex $tb 1]) - 1}]
    set x2 [expr {max([lindex $bb_base 2],[lindex $tb 2]) + 1}]
    set y2 [expr {max([lindex $bb_base 3],[lindex $tb 3]) + 1}]
    set boxfill [expr {$live ? "yellow" : "cyan"}]
    set rect [$c create rectangle $x1 $y1 $x2 $y2 \
                  -fill $boxfill -outline "" \
                  -tags [list infoitemname fvov$item]]

    # doppio-click: (ri)definisce la variabile da animare - sempre attivo
    $c bind $rect <Double-1> "anim_freeval_dialog $c $item"
    $c bind $tid  <Double-1> "anim_freeval_dialog $c $item"

    # balloon (tasto destro): nome variabile o avviso se non assegnata
    set ball [expr {$var eq "" ? "(doppio-click per assegnare)" : $var}]
    $c bind $rect <ButtonPress-3>   "anim_show_balloon %X %Y {$ball} {$ball}"
    $c bind $rect <ButtonRelease-3> "anim_hide_balloon"
    $c bind $tid  <ButtonPress-3>   "anim_show_balloon %X %Y {$ball} {$ball}"
    $c bind $tid  <ButtonRelease-3> "anim_hide_balloon"

    # layering: placeholder < rettangolo opaco < testo valore
    $c raise $rect
    $c raise $tid
}

# Dialogo doppio-click: chiede la variabile da animare + modalità etichetta.
proc anim_freeval_dialog { c item } {
    set tags_curr [$c gettags $item]
    set inst_name [file rootname [lindex $tags_curr [lsearch $tags_curr *.name]]]
    set ::freeval_var  [anim_get_var $inst_name ""]
    set modo [expr {[info exists ::anim_mode($inst_name)] ? $::anim_mode($inst_name) : ""}]
    # Nel tab del disegno le strutture del remap non sono caricate (le carica
    # Show Value): si legge il file. I suffissi ";F" e ";S" sono di altri
    # elementi (faceplate, set value) e qui non contano.
    if {$::freeval_var eq ""} {
        set d [anim_remap_leggi]
        if {[dict exists $d $inst_name]} {
            lassign [dict get $d $inst_name] v m
            if {$m ne "F" && $m ne "S"} {
                set ::freeval_var $v
                set modo $m
            }
        }
    }
    set ::freeval_mode [expr {$modo eq "L" ? 1 : 0}]

    set w .freeval_dlg
    catch {destroy $w}
    toplevel $w
    wm title $w "Variabile da animare"
    catch { wm transient $w [winfo toplevel $c] }
    wm resizable $w 0 0

    frame $w.f
    pack $w.f -padx 12 -pady 10 -fill x
    label $w.f.l -text "Nome variabile:" -anchor w
    entry $w.f.e -textvariable ::freeval_var -width 16
    grid $w.f.l -row 0 -column 0 -sticky w -pady 3
    grid $w.f.e -row 0 -column 1 -sticky ew -pady 3 -padx 4
    checkbutton $w.f.cb -text "Mostra nome variabile (etichetta)" \
        -variable ::freeval_mode -anchor w
    grid $w.f.cb -row 1 -column 0 -columnspan 2 -sticky w -pady 4

    # Elenco filtrabile delle variabili del modello, come nel dialogo
    # "Variable to set" (hmi_lista_variabili, hmielem.tcl): si scrive per
    # restringerlo, un clic sceglie, il doppio clic conferma. Qui ci sono
    # tutte le variabili, con il loro tipo: un display puo' mostrare anche
    # uscite e variabili calcolate.
    set tutte [hmi_variabili_tutte]
    hmi_lista_variabili $w.f.v $w.f.e ::freeval_var $tutte \
        [list anim_freeval_apply $c $item $w] 1
    grid $w.f.v -row 2 -column 0 -columnspan 2 -sticky nsew -pady 4
    if {[llength $tutte] == 0} {
        set nota "Variabili del modello non caricate."
    } else {
        set nota "[llength $tutte] variabili del modello. Scrivi per filtrare,\nclic per scegliere, doppio clic per confermare."
    }
    label $w.f.n -text $nota -anchor w -justify left -foreground "#555555"
    grid $w.f.n -row 3 -column 0 -columnspan 2 -sticky w

    frame $w.btn
    pack $w.btn -pady 8
    button $w.btn.ok  -text OK      -width 8 -default active \
        -command "anim_freeval_apply $c $item $w"
    button $w.btn.can -text Annulla -width 8 -command "destroy $w"
    pack $w.btn.ok $w.btn.can -side left -padx 6

    bind $w <Return> "anim_freeval_apply $c $item $w"
    bind $w <Escape> "destroy $w"
    focus $w.f.e
    $w.f.e selection range 0 end
    catch { grab $w }
}

# Applica la variabile scelta: valida contro il modello, salva nel .remap,
# aggiorna immediatamente il campo.
proc anim_freeval_apply { c item w } {
    global tipVarMod
    set var [string toupper [string trim $::freeval_var]]
    if {$var eq ""} {
        tk_messageBox -parent $w -icon warning -type ok \
            -message "Inserire un nome di variabile."
        return
    }
    # Fuori dal tab dei dati (Model Topology) il F01 puo' non essere caricato:
    # li' il nome non si puo' verificare, e si accetta com'e' scritto.
    if {[array size tipVarMod] > 0 && ![info exists tipVarMod($var)]} {
        tk_messageBox -parent $w -icon error -type ok \
            -message "Variabile '$var' non presente nel modello."
        return
    }

    set tags_curr [$c gettags $item]
    set inst_name [file rootname [lindex $tags_curr [lsearch $tags_curr *.name]]]
    # anim_remap_set aggiorna memoria e file, rileggendo il file prima: altre
    # applicazioni possono avervi scritto nel frattempo.
    catch { anim_remap_set $inst_name $var [expr {$::freeval_mode ? "L" : ""}] }
    catch { grab release $w }
    catch { destroy $w }

    # Fuori da Show Value (Model Topology, o il tab dei dati con un'altra
    # modalita') non ci sono campi animati: si rifa' il segnaposto disegnato.
    if {[info procs hmi_canvas_disegno] ne "" && [hmi_canvas_disegno $c] \
        && (![info exists ::showon] || $::showon != 4)} {
        catch {hmi_segnaposto $c $item}
        return
    }

    # refresh immediato del campo
    set lc [$c bbox $item]
    set x [expr ([lindex $lc 0]+[lindex $lc 2])/2]
    set y [expr [lindex $lc 3] + 6]
    set baseFont {Helvetica 8}
    set zl [expr {[info exists ::zoomLevelOf($c)] ? $::zoomLevelOf($c) : 1.0}]
    set ssz [expr {int(round(8 * $zl))}]
    if {$ssz < 1} { set ssz 1 }
    set live [expr {[info exists ::pipeon] && $::pipeon ? 1 : 0}]
    anima_freeval_field $c $item $x $y [list Helvetica $ssz] $baseFont $live
}

# ==========================================================================
# Unita' di misura (tool umis, Alg_rt/bin)
# ==========================================================================
# La conversione dei valori LIVE (pipe viewval -s) avviene gia' in viewval,
# che legge il file unita' (uni_misc.cfg per-simulazione, fallback
# uni_misc.dat) dalla dir della sim. Qui carichiamo la STESSA tabella via
# `umis -l` per: 1) i valori statici F14 (conversione lato Tcl), 2) il
# dialogo di scelta unita' (umis_dialog).
#   ::umis_tab(<lettera>) = {unita A B} dell'unita' SELEZIONATA del tipo
#   ::umis_rows           = righe {codice lettera sel {unita...}} (dialogo)

# Directory della simulazione: la stessa usata da viewval/graphics/xaing.
proc umis_sim_dir {} {
    if {[info exists ::anima_sim_path] && $::anima_sim_path ne "" && \
        [file isdirectory $::anima_sim_path]} { return $::anima_sim_path }
    return [pwd]
}

# Path del tool umis ("" se non disponibile: la feature degrada senza errori).
proc umis_exec_path {} {
    if {[info exists ::env(LEGORT_BIN)]} {
        set p [file join $::env(LEGORT_BIN) umis]
        if {[file executable $p]} { return $p }
    }
    set p [auto_execok umis]
    if {$p ne ""} { return [lindex $p 0] }
    return ""
}

# Carica la tabella unita' correnti dalla dir della simulazione.
# Ritorna 1 se la tabella e' stata caricata.
proc umis_load {} {
    array unset ::umis_tab
    set ::umis_rows {}
    set exe [umis_exec_path]
    if {$exe eq ""} { return 0 }
    set olddir [pwd]
    catch {cd [umis_sim_dir]}
    set out ""
    catch {set out [exec $exe -l]}
    cd $olddir
    foreach line [split $out "\n"] {
        # salta commenti/intestazioni (#...) e righe di servizio di chdefaults
        if {[string index [string trim $line] 0] eq "#"} continue
        # riga tabella: CODICE lettera sel u0|[u1]|... A B  (6 campi;
        # l'unita' selezionata e' tra parentesi quadre, che qui rimuoviamo)
        if {[catch {llength $line} n] || $n != 6} continue
        lassign $line cod let sel units A B
        if {![string is integer -strict $sel]} continue
        set ulist [split [string map {[ {} ] {}} $units] |]
        set ::umis_tab($let) [list [lindex $ulist $sel] $A $B]
        lappend ::umis_rows [list $cod $let $sel $ulist]
    }
    return [expr {[llength $::umis_rows] > 0}]
}

# Unita' di misura della variabile (dalla lettera iniziale del nome).
# Usa la tabella umis se caricata, altrimenti il fallback storico MKS.
proc ret_umis { nome } {
  set nome1 [string toupper $nome]
  set vai [string index $nome1 0]
  if {[info exists ::umis_tab($vai)]} {
      return [lindex $::umis_tab($vai) 0]
  }
  set umis " "
  switch $vai {

            W {set umis "kg/s"}
			P {set umis "Pa"}
			H {set umis "j/kg"}
			T {set umis "K"}
			Q {set umis "W"}
			L {set umis "m"}
			A {set umis "p.u."}
			R {set umis "RPM"}
      default {set umis "-"}

			  }
  return $umis
}

# Converte un valore MKS nell'unita' selezionata: val_vis = A*val + B
# (stessa formula di viewval/graphics). Ritorna il valore invariato se la
# tabella non e' caricata o il valore non e' numerico (es. messaggi errore).
proc conv_umis { nome val } {
    set vai [string index [string toupper $nome] 0]
    if {![info exists ::umis_tab($vai)]} { return $val }
    if {![string is double -strict $val]} { return $val }
    lassign $::umis_tab($vai) um A B
    return [format %.6g [expr {$A*$val + $B}]]
}

# --------------------------------------------------------------------------
# Dialogo di scelta unita' (per tipo di grandezza). Le scelte vengono salvate
# nel file TESTO per-simulazione uni_misc.cfg della dir della sim (creato se
# assente) dal tool umis. refresh_cmd: script eseguito dopo il salvataggio
# (tipicamente il re-Show Value, per rilanciare viewval con le nuove unita').
proc umis_dialog { {refresh_cmd ""} } {
    set exe [umis_exec_path]
    if {$exe eq ""} {
        tk_messageBox -icon error -title "Units" -message \
            "Tool 'umis' non trovato (LEGORT_BIN)."
        return
    }
    if {![umis_load]} {
        tk_messageBox -icon error -title "Units" -message \
            "Impossibile leggere la tabella delle unita' di misura."
        return
    }
    set w .umis_dlg
    catch {destroy $w}
    toplevel $w
    wm title $w "Unita' di misura"
    label $w.head -justify left -anchor w -text \
        "Unita' per la simulazione:\n[umis_sim_dir]"
    pack $w.head -fill x -padx 10 -pady 6

    frame $w.g
    pack $w.g -padx 12 -pady 4
    array unset ::umis_choice
    set r 0
    foreach row $::umis_rows {
        lassign $row cod let sel units
        # tipi con una sola unita' (o placeholder): niente da scegliere
        if {[llength $units] < 2} continue
        label $w.g.l$r -text "$cod ($let)" -anchor w -width 12
        grid $w.g.l$r -row $r -column 0 -sticky w -padx 2 -pady 1
        set ::umis_choice($cod) [lindex $units $sel]
        set col 1
        foreach u $units {
            radiobutton $w.g.r${r}c$col -text $u -value $u \
                -variable ::umis_choice($cod)
            grid $w.g.r${r}c$col -row $r -column $col -sticky w
            incr col
        }
        incr r
    }

    frame $w.btn
    pack $w.btn -pady 8
    button $w.btn.ok  -text OK      -width 8 -default active \
        -command [list umis_apply $w $exe $refresh_cmd]
    button $w.btn.can -text Annulla -width 8 -command [list destroy $w]
    pack $w.btn.ok $w.btn.can -side left -padx 6
    bind $w <Return> [list umis_apply $w $exe $refresh_cmd]
    bind $w <Escape> [list destroy $w]
}

# Applica le scelte del dialogo: una exec di umis per ogni tipo cambiato.
proc umis_apply { w exe refresh_cmd } {
    set olddir [pwd]
    catch {cd [umis_sim_dir]}
    set errs ""
    foreach row $::umis_rows {
        lassign $row cod let sel units
        if {![info exists ::umis_choice($cod)]} continue
        if {$::umis_choice($cod) eq [lindex $units $sel]} continue
        if {[catch {exec $exe $cod $::umis_choice($cod)} msg]} {
            append errs "$cod: $msg\n"
        }
    }
    cd $olddir
    umis_load
    catch {destroy $w}
    if {$errs ne ""} {
        tk_messageBox -icon error -title "Units" -message $errs
    }
    if {$refresh_cmd ne ""} { uplevel #0 $refresh_cmd }
}

# Elementi operatore delle pagine (faceplate @stz_0, set value @set_0): usano
# le proc di questo file (anima, conv_umis, anim_remap_*), quindi si caricano
# per ultimi. Stanno accanto a questo script, come lgstaz.tcl che sorgiano.
source [file join [file dirname [file normalize [info script]]] hmielem.tcl]
