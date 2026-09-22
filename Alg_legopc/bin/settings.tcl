# ==============================================================================
# settings.tcl  —  Dialog "File -> Settings" (versione Linux)
#
# Lo aprono legopc (File -> Settings...) e lghmi (stessa voce, stesso menu):
# le applicazioni di base sono le stesse per tutto LegoPST, e la scelta fatta
# in un programma deve valere anche nell'altro. Per questo il file delle
# preferenze e' uno solo, legopc_prefs.tcl nell'area utente.
#
# Permette all'utente di modificare:
#   LG_TEXTEDITOR  editor di testo  (default: gedit o quello da .profile_legoroot)
#   LG_BROWSER     browser HTML     (default: xdg-open o quello da .profile_legoroot)
#   LG_ICOEDITOR   editor icone     (default: gimp o quello da .profile_legoroot)
#   LG_PDFVIEWER   viewer PDF/PNG   (default: evince o quello da .profile_legoroot)
#   LG_XTERM       terminale X      (default: xterm o quello da .profile_legoroot;
#                                    menu "Installed" con i terminali che lgterm
#                                    sa usare, fra quelli presenti)
#
# Le modifiche vengono applicate immediatamente all'env del processo corrente
# e salvate in legopc_prefs.tcl (in LG_ENTRY = dir utente, non sovrascritta
# dall'installer): con savePrefs, dove c'e' (legopc), altrimenti riscrivendo
# solo le righe delle applicazioni (settings_salva_prefs).
# ==============================================================================

proc lancia_settings {} {
    global env

    set w .settings_dlg
    if {[winfo exists $w]} { raise $w; return }

    toplevel $w
    wm title $w "LegoPST Settings"
    wm resizable $w 0 0

    # Valori correnti: usa la variabile d'env se definita, altrimenti default Linux
    if {[info exists env(LG_TEXTEDITOR)] && $env(LG_TEXTEDITOR) ne ""} {
        set ::settings_te $env(LG_TEXTEDITOR)
    } else {
        set ::settings_te "gedit"
    }
    if {[info exists env(LG_BROWSER)] && $env(LG_BROWSER) ne ""} {
        set ::settings_br $env(LG_BROWSER)
    } else {
        set ::settings_br "xdg-open"
    }
    if {[info exists env(LG_ICOEDITOR)] && $env(LG_ICOEDITOR) ne ""} {
        set ::settings_ie $env(LG_ICOEDITOR)
    } else {
        set ::settings_ie "gimp"
    }
    if {[info exists env(LG_PDFVIEWER)] && $env(LG_PDFVIEWER) ne ""} {
        set ::settings_pv $env(LG_PDFVIEWER)
    } else {
        set ::settings_pv "evince"
    }
    if {[info exists env(LG_XTERM)] && $env(LG_XTERM) ne ""} {
        set ::settings_xt $env(LG_XTERM)
    } else {
        set ::settings_xt "xterm"
    }


    # ── Frame principale ──
    frame $w.f
    pack $w.f -fill both -expand 1 -padx 10 -pady 8

    set row 0
    foreach {lbl var} [list \
        "Text editor:"  ::settings_te \
        "HTML browser:" ::settings_br \
        "Icon editor:"  ::settings_ie \
        "PDF viewer:"   ::settings_pv \
        "Terminal:"     ::settings_xt] {

        label  $w.f.lbl$row -text $lbl -anchor w -width 14
        entry  $w.f.ent$row -textvariable $var -width 42
        button $w.f.btn$row -text "Browse..." \
            -command [list settings_browse $w.f.ent$row $var]
        grid $w.f.lbl$row -row $row -column 0 -sticky w  -pady 3
        grid $w.f.ent$row -row $row -column 1 -sticky ew -pady 3 -padx 4
        grid $w.f.btn$row -row $row -column 2 -sticky w  -pady 3
        incr row
    }

    # Terminal: accanto al campo, i terminali che lgterm (util97) sa usare,
    # fra quelli installati. Il terminale scelto vale per tutto LegoPST: lo
    # usa lgterm (kStat, kLeeF22, Export as -> FMU, Tools -> Terminal) e il
    # profilo lo legge da legopc_prefs.tcl per le shell aperte dopo.
    set rt [expr {$row - 1}]
    menubutton $w.f.mbt -text "Installed" -relief raised -indicatoron 1 \
        -menu $w.f.mbt.m
    menu $w.f.mbt.m -tearoff 0
    set n 0
    foreach t {xfce4-terminal tilix xterm konsole gnome-terminal lxterminal} {
        if {[auto_execok $t] ne ""} {
            $w.f.mbt.m add radiobutton -label $t -variable ::settings_xt -value $t
            incr n
        }
    }
    if {$n == 0} { $w.f.mbt configure -state disabled }
    grid $w.f.mbt -row $rt -column 3 -sticky w -pady 3 -padx 2

    # ── Separatore + bottoni ──
    frame $w.sep -height 2 -relief groove -bd 1
    pack $w.sep -fill x -padx 8 -pady 4

    frame $w.btns
    pack $w.btns -pady 6

    button $w.btns.ok     -text "OK"     -width 10 -default active \
        -command [list settings_apply $w]
    button $w.btns.cancel -text "Cancel" -width 10 \
        -command [list destroy $w]
    pack $w.btns.ok $w.btns.cancel -side left -padx 8

    bind $w <Return> [list settings_apply $w]
    bind $w <Escape> [list destroy $w]
}

#  Il file delle preferenze: quello di legopc se la sua proc c'e', altrimenti
#  lo stesso path calcolato a mano. E' uno solo apposta - una scelta fatta in
#  legopc deve valere in lghmi e viceversa.
proc settings_prefs_file {} {
    global env
    if {[llength [info procs prefsFile]] > 0} { return [prefsFile] }
    set dir [expr {[info exists env(LG_ENTRY)] && $env(LG_ENTRY) ne "" \
                   ? $env(LG_ENTRY) : [file join $env(HOME) legocad]}]
    return [file join $dir legopc_prefs.tcl]
}

#  Scrive le cinque preferenze LASCIANDO STARE tutto il resto del file.
#  Serve perche' il file e' condiviso: legopc ci tiene anche i colori dei
#  canvas e i colori recenti, che tiene in memoria e riscrive interi con
#  savePrefs. Chi quei valori non li ha (lghmi) non puo' riscrivere il file da
#  zero senza cancellarli, quindi qui si sostituiscono riga per riga solo le
#  "set ::pref_<nome>" e le altre si ricopiano come stanno.
proc settings_salva_prefs {} {
    global env
    set f [settings_prefs_file]

    set tenute {}
    if {[file exists $f]} {
        set ch [open $f r]
        fconfigure $ch -translation binary
        set testo [read $ch]
        close $ch
        foreach r [split [string trimright $testo "\n"] "\n"] {
            #  le righe che stiamo per riscrivere: si saltano
            if {[regexp {^\s*set\s+::(pref_texteditor|pref_browser|pref_icoeditor|pref_pdfviewer|pref_xterm)\s} $r]} {
                continue
            }
            lappend tenute $r
        }
    } else {
        lappend tenute "# LegoPST user preferences - saved automatically"
    }

    foreach {nome var} {pref_texteditor LG_TEXTEDITOR \
                        pref_browser    LG_BROWSER    \
                        pref_icoeditor  LG_ICOEDITOR  \
                        pref_pdfviewer  LG_PDFVIEWER  \
                        pref_xterm      LG_XTERM} {
        if {[info exists env($var)] && $env($var) ne ""} {
            lappend tenute "set ::$nome {$env($var)}"
        }
    }

    set ch [open $f w]
    fconfigure $ch -translation binary
    puts -nonewline $ch "[join $tenute \n]\n"
    close $ch
}

#  Porta nell'ambiente le preferenze salvate. La usa chi non ha la loadPrefs
#  di legopc (lghmi), all'avvio: senza, il dialogo mostrerebbe i default del
#  profilo invece di quello che l'utente ha scelto l'ultima volta.
#
#  Il file si LEGGE, non si sorgia: sorgiarlo eseguirebbe Tcl qualunque e
#  definirebbe qui dentro i globali dei colori di legopc, che qui non
#  servono a niente.
proc settings_carica_prefs {} {
    global env
    set f [settings_prefs_file]
    if {![file exists $f]} return
    set ch [open $f r]
    fconfigure $ch -translation binary
    set testo [read $ch]
    close $ch
    foreach {nome var} {pref_texteditor LG_TEXTEDITOR \
                        pref_browser    LG_BROWSER    \
                        pref_icoeditor  LG_ICOEDITOR  \
                        pref_pdfviewer  LG_PDFVIEWER  \
                        pref_xterm      LG_XTERM} {
        if {[regexp -line "^\\s*set\\s+::$nome\\s+\\{(\[^\}\]*)\\}" $testo -> valore]} {
            set valore [string trim $valore]
            if {$valore ne ""} { set env($var) $valore }
        }
    }
}

proc settings_browse {entry var} {
    # Su Linux naviga nella directory dei programmi comuni
    set initdir "/usr/bin"
    set f [tk_getOpenFile \
        -title "Select executable" \
        -initialdir $initdir \
        -filetypes {{"All files" *}}]
    if {[string length $f] > 0} {
        set $var $f
    }
}

proc settings_apply {w} {
    global env
    set env(LG_TEXTEDITOR) $::settings_te
    set env(LG_BROWSER)    $::settings_br
    set env(LG_ICOEDITOR)  $::settings_ie
    set env(LG_PDFVIEWER)  $::settings_pv
    set env(LG_XTERM)      $::settings_xt

    #  In legopc si usa la sua savePrefs, che salva anche i colori dei canvas.
    #  Altrove si riscrivono le sole righe delle applicazioni.
    if {[llength [info procs savePrefs]] > 0} {
        set esito [catch {savePrefs} err]
    } else {
        set esito [catch {settings_salva_prefs} err]
    }
    #  Se il file non si e' potuto scrivere la finestra resta aperta: la scelta
    #  vale per questo processo ma non sopravvive, e l'utente deve saperlo.
    if {$esito} {
        tk_messageBox -icon error -title "Settings" -parent $w \
            -message "Settings not saved:\n$err"
        return
    }
    destroy $w
}
