set ::indicatore_after 0
set ::pipeon 0
set ::anima_sim_path "locpath - click to change"

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
            if {$can_validate && ![info exists tipVarMod($var)]} {
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

#tk_messageBox  -message "anima_ggiorn- modo=$mod - indicatore_after=$::indicatore_after"

if { $mod == 1 } {

# Carica (o ricarica) il file .remap per il modello corrente.
# Funziona sia da draw2gr.tcl che da legopc.tix grazie a curFileName globale.
anim_load_remap

# reset: eventualmente cancello i precedenti campi già creati
foreach item [$c find withtag module] {
        set tags_curr [$c gettags $item]
        set pisqu_tid [file rootname [lindex $tags_curr [lsearch $tags_curr *.visual]]]
#puts "anima_aggiorna 1: cancello pisqu_tid=$pisqu_tid"
		$c dtag $pisqu_tid.visual
		}

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
                # Binding doppio-click per remap: solo blocchi normali (non remark)
                # e solo se anim_field_select è disponibile (draw2gr.tcl, non legopc.tix)
                if { $is_remark == 0 } {
                    $c bind $rect <Double-1> \
                        "if {\[info procs anim_field_select\] != {}} { anim_field_select $c $item $rect {$tags_curr} }"
                    $c bind $tid  <Double-1> \
                        "if {\[info procs anim_field_select\] != {}} { anim_field_select $c $item $rect {$tags_curr} }"
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
set ::indicatore_after [after $refr_anim_ms anima_aggiorna $c 2]
}

if { $mod == 3 } {
# Carica il file .remap anche per la modalità f14 statica (legopc.tix)
anim_load_remap
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
           set testo "$valore14 [ret_umis $variab_inv]"
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
           # Binding doppio-click per remap: solo blocchi normali (non remark)
           # e solo se anim_field_select è disponibile (draw2gr.tcl, non legopc.tix)
           if { $is_remark == 0 } {
               $c bind $rect <Double-1> \
                   "if {\[info procs anim_field_select\] != {}} { anim_field_select $c $item $rect {$tags_curr} }"
               $c bind $tid  <Double-1> \
                   "if {\[info procs anim_field_select\] != {}} { anim_field_select $c $item $rect {$tags_curr} }"
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
            set valtxt "$valore14 [ret_umis $var]"
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
    set ::freeval_mode [expr {[info exists ::anim_mode($inst_name)] && \
                              $::anim_mode($inst_name) eq "L" ? 1 : 0}]

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
    if {![info exists tipVarMod($var)]} {
        tk_messageBox -parent $w -icon error -type ok \
            -message "Variabile '$var' non presente nel modello."
        return
    }

    set tags_curr [$c gettags $item]
    set inst_name [file rootname [lindex $tags_curr [lsearch $tags_curr *.name]]]
    set ::anim_remap($inst_name) $var
    if {$::freeval_mode} {
        set ::anim_mode($inst_name) "L"
    } else {
        catch {unset ::anim_mode($inst_name)}
    }
    catch { anim_save_remap }
    catch { grab release $w }
    catch { destroy $w }

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

proc ret_umis { nome } {
  set nome1 [string toupper $nome]
  set vai [string index $nome1 0]
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
