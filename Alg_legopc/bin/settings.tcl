# ==============================================================================
# settings.tcl  —  Dialog "File -> Settings" di LegoPC (versione Linux)
#
# Permette all'utente di modificare:
#   LG_TEXTEDITOR  editor di testo  (default: gedit o quello da .profile_legoroot)
#   LG_BROWSER     browser HTML     (default: xdg-open o quello da .profile_legoroot)
#   LG_ICOEDITOR   editor icone     (default: gimp o quello da .profile_legoroot)
#   LG_PDFVIEWER   viewer PDF/PNG   (default: evince o quello da .profile_legoroot)
#   LG_XTERM       terminale X      (default: xterm o quello da .profile_legoroot)
#
# Le modifiche vengono applicate immediatamente all'env del processo corrente
# e salvate in legopc_prefs.tcl (in LG_ENTRY = dir utente, non sovrascritta
# dall'installer) tramite savePrefs/loadPrefs di legopc.tix.
# ==============================================================================

proc lancia_settings {} {
    global env

    set w .settings_dlg
    if {[winfo exists $w]} { raise $w; return }

    toplevel $w
    wm title $w "LegoPC Settings"
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
    savePrefs
    destroy $w
}
