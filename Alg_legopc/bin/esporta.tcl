# esporta.tcl - File -> Export as: il canvas in vista come PDF o PNG.
#
# Sorgiato da legopc.tix (tab Model Topology e Data Assignment) e da
# draw2gr.tcl (lo schema della HMI). Prima stava tutto in legopc.tix.
#
# La strada normale: il canvas diventa PostScript con il comando di Tk
# ($c postscript, plotPS_internal), e Ghostscript lo converte in PDF
# (pdfwrite) o in PNG (png16m, alla risoluzione scelta). Il file va accanto al
# .tom, con il nome del modello (<modello>.pdf, <modello>.png); se quella
# directory non e' scrivibile - un bundle FMU installato in sola lettura - lo
# si chiede.
#
# Senza Ghostscript l'esportazione e' IMPOSSIBILE, e il dialogo lo dice per
# prima cosa, con il modo di installarlo. Poi propone un ripiego, che si fa
# solo se l'utente risponde Si' (default No: nessun file che non si e'
# chiesto):
#   PDF  salvare il PostScript (<modello>.ps): vettoriale, stampabile, e si
#        converte dopo con ps2pdf;
#   PNG  fotografare la finestra, con il pacchetto Tcl Img (tkimg) o con
#        "import" di ImageMagick: solo la parte del disegno visibile, alla
#        risoluzione dello schermo. Se manca anche quello, il PostScript come
#        per il PDF.
# Un errore di Ghostscript lascia il PostScript al posto del risultato.

proc _openFile {path} {
    # Apre un file con il viewer disponibile (evita xdg-open su WSL)
    global env
    if {[info exists env(LG_PDFVIEWER)] && $env(LG_PDFVIEWER) != ""} {
        catch {exec $env(LG_PDFVIEWER) $path &}
        return
    }
    foreach viewer {evince okular zathura mupdf xpdf atril} {
        if {![catch {set v [exec which $viewer]}]} {
            catch {exec [string trim $v] $path &}
            return
        }
    }
    tk_messageBox -icon info -type ok \
        -message "File creato:\n$path\n\nNessun PDF/PNG viewer trovato.\nInstallare con: sudo dnf install evince"
}

proc findGhostscript {} {
    # Su Linux cerca 'gs' nel PATH
    if {![catch {set gs [exec which gs]} ]} {
        set gs [string trim $gs]
        if {[file exists $gs]} { return $gs }
    }
    # Fallback: percorsi comuni
    foreach candidate {/usr/bin/gs /usr/local/bin/gs} {
        if {[file exists $candidate]} { return $candidate }
    }
    return ""
}

# Calcola bounding box di tutti gli item (incluse immagini)
proc canvasBboxAll {c} {
    set cx1 1e9; set cy1 1e9; set cx2 -1e9; set cy2 -1e9
    foreach item [$c find all] {
        if {[$c type $item] == "image"} {
            set coords [$c coords $item]
            if {$coords == ""} continue
            set img    [$c itemcget $item -image]
            if {[catch {set iw [image width  $img]}]} continue
            if {[catch {set ih [image height $img]}]} continue
            set anchor [$c itemcget $item -anchor]
            set x [lindex $coords 0]
            set y [lindex $coords 1]
            switch $anchor {
                nw     { set x1 $x;                    set y1 $y                   }
                n      { set x1 [expr {$x - $iw/2}];   set y1 $y                   }
                ne     { set x1 [expr {$x - $iw}];     set y1 $y                   }
                w      { set x1 $x;                    set y1 [expr {$y - $ih/2}]  }
                e      { set x1 [expr {$x - $iw}];     set y1 [expr {$y - $ih/2}]  }
                sw     { set x1 $x;                    set y1 [expr {$y - $ih}]    }
                s      { set x1 [expr {$x - $iw/2}];   set y1 [expr {$y - $ih}]    }
                se     { set x1 [expr {$x - $iw}];     set y1 [expr {$y - $ih}]    }
                default { set x1 [expr {$x - $iw/2}];  set y1 [expr {$y - $ih/2}]  }
            }
            set x2 [expr {$x1 + $iw}]
            set y2 [expr {$y1 + $ih}]
        } else {
            set bb [$c bbox $item]
            if {$bb == ""} continue
            set x1 [lindex $bb 0]; set y1 [lindex $bb 1]
            set x2 [lindex $bb 2]; set y2 [lindex $bb 3]
        }
        if {$x1 < $cx1} { set cx1 $x1 }
        if {$y1 < $cy1} { set cy1 $y1 }
        if {$x2 > $cx2} { set cx2 $x2 }
        if {$y2 > $cy2} { set cy2 $y2 }
    }
    if {$cx1 >= $cx2 || $cy1 >= $cy2} { return "" }
    return [list $cx1 $cy1 $cx2 $cy2]
}

# Produce il PostScript ottimizzato (A4, portrait/landscape automatico)
proc plotPS_internal {c psfile} {
    update
    set bb [canvasBboxAll $c]
    if {$bb == ""} {
        tk_messageBox -icon warning -type ok -message "Canvas vuoto, nessun file prodotto."
        return ""
    }
    set cx1 [lindex $bb 0]; set cy1 [lindex $bb 1]
    set cx2 [lindex $bb 2]; set cy2 [lindex $bb 3]

    set margin 20
    set cx1 [expr {$cx1 - $margin}]
    set cy1 [expr {$cy1 - $margin}]
    set cw  [expr {$cx2 - $cx1 + 2*$margin}]
    set ch  [expr {$cy2 - $cy1 + 2*$margin}]

    # A4 con margini 20mm: area utile portrait 481x728 pt, landscape 728x481 pt
    set sx_p [expr {double(481) / $cw}]; set sy_p [expr {double(728) / $ch}]
    if {$sx_p < $sy_p} { set s_port $sx_p } else { set s_port $sy_p }
    set sx_l [expr {double(728) / $cw}]; set sy_l [expr {double(481) / $ch}]
    if {$sx_l < $sy_l} { set s_land $sx_l } else { set s_land $sy_l }

    if {$s_land > $s_port} { set rotate 1; set scale $s_land } \
    else                   { set rotate 0; set scale $s_port }

    set out_w [expr {int($cw * $scale)}]

    # Includi il colore di sfondo della canvas nel PostScript
    set _bgcol [$c cget -background]
    set _bgrect [$c create rectangle $cx1 $cy1 \
        [expr {$cx1 + $cw}] [expr {$cy1 + $ch}] \
        -fill $_bgcol -outline {} -tags _ps_background]
    $c lower _ps_background

    $c postscript \
        -x $cx1 -y $cy1 -width $cw -height $ch \
        -pagewidth  ${out_w}p \
        -pagex      297p \
        -pagey      421p \
        -pageanchor center \
        -rotate     $rotate \
        -colormode  color \
        -file       $psfile

    $c delete _ps_background
    return $psfile
}

proc export_show_busy {msg} {
    set w .busywin
    catch {destroy $w}
    toplevel $w
    wm title $w "Please wait"
    wm resizable $w 0 0
    wm transient $w .
    wm protocol $w WM_DELETE_WINDOW {}
    label $w.l -text $msg -padx 20 -pady 10 -font "Helvetica 10 bold"
    pack $w.l
    catch {
        ttk::progressbar $w.pb -mode indeterminate -length 260
        pack $w.pb -padx 20 -pady {0 15}
        $w.pb start 30
    }
    update idletasks
    set px [expr {[winfo rootx .] + ([winfo width .] - [winfo width $w]) / 2}]
    set py [expr {[winfo rooty .] + ([winfo height .] - [winfo height $w]) / 2}]
    wm geometry $w "+${px}+${py}"
    . configure -cursor watch
    update
    return $w
}

proc export_hide_busy {w} {
    catch { $w.pb stop }
    catch { destroy $w }
    . configure -cursor ""
}

#  Il file da scrivere: <modello>.<est> accanto al .tom. Se non c'e' un
#  modello o la directory non e' scrivibile, lo si chiede ("" = annullato).
proc esporta_file {est} {
    set f ""
    if {[info exists ::curFileName] && [lsearch -exact {"" untitled -} $::curFileName] < 0} {
        set f "[file rootname $::curFileName].$est"
    }
    if {$f ne "" && [file writable [file dirname $f]]} { return $f }
    set ini [expr {$f ne "" ? [file tail $f] : "schema.$est"}]
    return [tk_getSaveFile -parent . -title "Export as [string toupper $est]" \
                -initialdir $::env(HOME) -initialfile $ini -defaultextension .$est]
}

#  PostScript temporaneo, uno per processo: legopc e draw2gr possono
#  esportare nello stesso momento.
proc esporta_tmp_ps {} {
    set tmp /tmp
    if {[info exists ::env(TMPDIR)] && [file isdirectory $::env(TMPDIR)]} { set tmp $::env(TMPDIR) }
    return [file join $tmp _legopc_export_[pid].ps]
}

#  Come installare Ghostscript, per i messaggi.
proc esporta_come_installare_gs {} {
    return "sudo dnf install ghostscript   (Fedora)\nsudo apt install ghostscript   (Ubuntu)"
}

#  Ripiego senza conversione: il PostScript accanto al file che si voleva
#  (<nome>.ps). <tmpps> e' un PostScript gia' prodotto da spostare li', oppure
#  "" per produrlo adesso. Ritorna il path del .ps, "" se non e' riuscito.
proc esporta_ripiego_ps {c voluto tmpps} {
    set psfile "[file rootname $voluto].ps"
    if {$tmpps ne "" && [file exists $tmpps]} {
        if {[catch {file rename -force $tmpps $psfile}]} { return "" }
        return $psfile
    }
    if {[plotPS_internal $c $psfile] eq ""} { return "" }
    return $psfile
}

#  Apre un PNG: con l'editor di icone dell'installazione (LG_ICOEDITOR), come
#  ha sempre fatto legopc, altrimenti con il viewer.
proc esporta_apri_png {pngfile} {
    if {[info exists ::env(LG_ICOEDITOR)] && $::env(LG_ICOEDITOR) ne "" \
            && [auto_execok $::env(LG_ICOEDITOR)] ne ""} {
        catch {exec $::env(LG_ICOEDITOR) $pngfile &}
        return
    }
    _openFile $pngfile
}

proc plotPDF {c} {
    set pdffile [esporta_file pdf]
    if {$pdffile eq ""} return
    set gs [findGhostscript]

    if {$gs eq ""} {
        esporta_impossibile_ps $c $pdffile PDF "ps2pdf [file rootname $pdffile].ps"
        return
    }
    set bw [export_show_busy "Esportazione PDF in corso..."]

    set psfile [esporta_tmp_ps]
    if {[plotPS_internal $c $psfile] == ""} { export_hide_busy $bw; return }
    set rc [catch {exec $gs -dNOPAUSE -dBATCH -sDEVICE=pdfwrite \
        "-sOutputFile=$pdffile" $psfile} err]
    if {$rc != 0} {
        set ps [esporta_ripiego_ps $c $pdffile $psfile]
        export_hide_busy $bw
        tk_messageBox -icon error -type ok -title "Export PDF" -message \
            "Esportazione non riuscita, errore di Ghostscript:\n$err[expr {$ps ne "" ? "\n\nResta il PostScript:\n    $ps" : ""}]"
        return
    }
    catch {file delete $psfile}
    export_hide_busy $bw

    _openFile $pdffile
    tk_messageBox -icon info -type ok -message "PDF creato: $pdffile"
}

#  Risoluzione del PNG (solo con Ghostscript: la fotografia della finestra ha
#  quella dello schermo). Ritorna i dpi, "" se annullato.
proc esporta_chiedi_dpi {} {
    set w .png_dlg
    if {[winfo exists $w]} { destroy $w }
    toplevel $w
    wm title $w "Export PNG"
    wm resizable $w 0 0

    set ::_png_dpi 150
    frame $w.f
    pack $w.f -padx 12 -pady 8
    label $w.f.lbl -text "Risoluzione:" -anchor w
    pack $w.f.lbl -anchor w
    foreach {txt val} {"96 dpi  (schermo)" 96 "150 dpi (stampa normale)" 150 "300 dpi (alta qualita')" 300} {
        radiobutton $w.f.r$val -text $txt -variable ::_png_dpi -value $val
        pack $w.f.r$val -anchor w
    }

    frame $w.sep -height 2 -relief groove -bd 1
    pack $w.sep -fill x -padx 8 -pady 4
    frame $w.btns
    pack $w.btns -pady 6

    set ::_png_ok 0
    button $w.btns.ok -text "OK" -width 10 -default active \
        -command {set ::_png_ok 1; destroy .png_dlg}
    button $w.btns.cancel -text "Cancel" -width 10 \
        -command {destroy .png_dlg}
    pack $w.btns.ok $w.btns.cancel -side left -padx 8
    bind $w <Return> {set ::_png_ok 1; destroy .png_dlg}
    bind $w <Escape> {destroy .png_dlg}

    tkwait window $w
    if {!$::_png_ok} { return "" }
    return $::_png_dpi
}

#  Con che cosa si puo' fotografare la finestra, senza Ghostscript: "img"
#  (pacchetto Tcl Img, formato "window"), "import" o "magick" (ImageMagick),
#  "" se niente.
proc esporta_strumento_schermo {} {
    if {![catch {package require Img}]} { return img }
    if {[auto_execok import] ne ""} { return import }
    if {[auto_execok magick] ne ""} { return magick }
    return ""
}

proc esporta_nome_strumento {s} {
    return [dict get {img "il pacchetto Tcl Img" import "ImageMagick import" magick "ImageMagick"} $s]
}

#  La fotografia della finestra del canvas con lo strumento <s>. La finestra
#  deve essere davanti e scoperta, quindi prima la si porta su. Ritorna 1 se
#  il file e' stato scritto.
proc esporta_png_schermo {c pngfile s} {
    catch {raise [winfo toplevel $c]}
    update
    after 300
    update
    set id [winfo id $c]
    switch -- $s {
        img {
            catch {
                set img [image create photo -format window -data $c]
                $img write $pngfile -format png
                image delete $img
            }
        }
        import { catch {exec {*}[auto_execok import] -window $id $pngfile 2>@1} }
        magick { catch {exec {*}[auto_execok magick] import -window $id $pngfile 2>@1} }
    }
    return [file exists $pngfile]
}

#  Senza Ghostscript: l'esportazione e' impossibile, e lo si dice; poi, se
#  l'utente vuole, il PostScript al posto di <voluto>.
proc esporta_impossibile_ps {c voluto tipo comando} {
    set psfile "[file rootname $voluto].ps"
    set msg "Esportazione in $tipo impossibile: manca Ghostscript, che serve alla\n"
    append msg "conversione.\n\nPer installarlo:\n[esporta_come_installare_gs]\n\n"
    append msg "Salvare intanto il disegno in PostScript (vettoriale, stampabile,\n"
    append msg "da convertire poi in $tipo)?\n    $psfile"
    if {[tk_messageBox -icon warning -type yesno -default no -title "Export $tipo" \
             -message $msg] ne "yes"} { return }
    set bw [export_show_busy "Salvataggio PostScript in corso..."]
    set ps [esporta_ripiego_ps $c $voluto ""]
    export_hide_busy $bw
    if {$ps eq ""} return
    tk_messageBox -icon info -type ok -title "Export $tipo" -message \
        "PostScript salvato:\n    $ps\n\nPer il $tipo, installato Ghostscript:\n    $comando"
}

proc plotPNG {c} {
    set gs [findGhostscript]

    if {$gs eq ""} {
        set strumento [esporta_strumento_schermo]
        if {$strumento eq ""} {
            #  niente per fotografare: si propone il PostScript
            set pngfile [esporta_file png]
            if {$pngfile eq ""} return
            set ps "[file rootname $pngfile].ps"
            esporta_impossibile_ps $c $pngfile PNG \
                "gs -sDEVICE=png16m -r150 -o [file rootname $pngfile].png $ps"
            return
        }
        #  si propone la fotografia della finestra
        set msg "Esportazione in PNG impossibile: manca Ghostscript, che serve alla\n"
        append msg "conversione.\n\nPer installarlo:\n[esporta_come_installare_gs]\n\n"
        append msg "Fotografare invece la finestra, con [esporta_nome_strumento $strumento]?\n"
        append msg "Solo la parte del disegno visibile, alla risoluzione dello schermo."
        if {[tk_messageBox -icon warning -type yesno -default no -title "Export PNG" \
                 -message $msg] ne "yes"} { return }
        set pngfile [esporta_file png]
        if {$pngfile eq ""} return
        catch {file delete $pngfile}
        if {![esporta_png_schermo $c $pngfile $strumento]} {
            tk_messageBox -icon error -type ok -title "Export PNG" -message \
                "Esportazione in PNG impossibile: la fotografia della finestra\n([esporta_nome_strumento $strumento]) non e' riuscita."
            return
        }
        esporta_apri_png $pngfile
        tk_messageBox -icon info -type ok -title "Export PNG" -message \
            "PNG creato (fotografia della finestra):\n    $pngfile"
        return
    }

    set dpi [esporta_chiedi_dpi]
    if {$dpi eq ""} return
    set pngfile [esporta_file png]
    if {$pngfile eq ""} return

    set psfile [esporta_tmp_ps]
    set bw [export_show_busy "Esportazione PNG (${dpi} dpi) in corso..."]
    if {[plotPS_internal $c $psfile] == ""} { export_hide_busy $bw; return }
    set rc [catch {exec $gs -dNOPAUSE -dBATCH -sDEVICE=png16m \
        -r$dpi "-sOutputFile=$pngfile" $psfile} err]
    if {$rc != 0} {
        set ps [esporta_ripiego_ps $c $pngfile $psfile]
        export_hide_busy $bw
        tk_messageBox -icon error -type ok -title "Export PNG" -message \
            "Esportazione non riuscita, errore di Ghostscript:\n$err[expr {$ps ne "" ? "\n\nResta il PostScript:\n    $ps" : ""}]"
        return
    }
    catch {file delete $psfile}
    export_hide_busy $bw

    esporta_apri_png $pngfile
    tk_messageBox -icon info -type ok -message "PNG creato (${dpi} dpi): $pngfile"
}
