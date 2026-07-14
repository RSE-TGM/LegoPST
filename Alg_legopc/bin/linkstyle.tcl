## linkstyle.tcl - override PER-MODELLO di colore/spessore/tratteggio delle
## connessioni tra icone (canvas topologia).
##
## Due livelli, salvati nel side-file <model>.lstyle accanto al .tom (come .remap):
##   - LINK: eccezione sul SINGOLO tratto, chiave = coppia porte normalizzata
##           "modA.portA|modB.portB" (indipendente dall'ordine);
##   - CAT : override dell'intera CATEGORIA (Hydraulic, Electrical, ...) valido
##           solo per questo modello, chiave = nome categoria (tycon).
## Priorita': default connect.dat  <  override CAT  <  override LINK.
##
## Arrays in memoria:
##   ::linkstyle(key)  -> {color <c> width <w> dash <0|1>}   (solo attr presenti)
##   ::linkcat(tycon)  -> idem

# ---------------------------------------------------------------- side-file
proc linkstyle_file {} {
    global curFileName
    if {![info exists curFileName]} { return "" }
    if {$curFileName eq "" || $curFileName eq "untitled" || $curFileName eq "-"} { return "" }
    return "[file rootname $curFileName].lstyle"
}

# ------------------------------------------------------------- chiave stabile
proc linkstyle_key {modA portA modB portB} {
    set a "$modA.$portA"
    set b "$modB.$portB"
    if {[string compare $a $b] <= 0} { return "$a|$b" } else { return "$b|$a" }
}

# item porta (id canvas) -> {modname porttag}, "" se non risolvibile
proc linkstyle_modport {c portid} {
    set tags [$c gettags $portid]
    set pi [lsearch $tags {port?*}]
    if {$pi < 0} { return "" }
    set porttag [lindex $tags $pi]
    foreach t $tags {
        if {![string match id* $t]} { continue }
        foreach it [$c find withtag $t] {
            set gt [$c gettags $it]
            if {[lsearch $gt module] < 0} { continue }
            set ni [lsearch $gt *.name]
            if {$ni < 0} { continue }
            return [list [file rootname [lindex $gt $ni]] $porttag]
        }
    }
    return ""
}

# item linea di connessione -> chiave normalizzata ("" se non risolvibile)
# Il tag della linea e' "link<sPortId>.<ePortId>" dove sPortId/ePortId sono il
# PRIMO tag delle due porte (di norma "id<itemId>", vedi ffconnect). Quindi il
# tag reale e' tipicamente "linkid42.id58"; le due meta' (dopo "link", spezzate
# sul primo ".") sono passate a linkstyle_modport, che accetta sia un id item
# nudo sia un tag "id<n>" (entrambi risolvibili via gettags/find).
proc linkstyle_key_from_line {c item} {
    set linktag ""
    foreach t [$c gettags $item] {
        if {[string match {link*.*} $t]} { set linktag $t; break }
    }
    if {$linktag eq ""} { return "" }
    set rest [string range $linktag 4 end]
    set dot  [string first "." $rest]
    if {$dot < 1} { return "" }
    set pa [string range $rest 0 [expr {$dot-1}]]
    set pb [string range $rest [expr {$dot+1}] end]
    set a [linkstyle_modport $c $pa]
    set b [linkstyle_modport $c $pb]
    if {$a eq "" || $b eq ""} { return "" }
    return [linkstyle_key [lindex $a 0] [lindex $a 1] [lindex $b 0] [lindex $b 1]]
}

# categoria (tycon) di un item connessione, dal tag <tycon>_ltype
proc linkstyle_tycon {c item} {
    set i [lsearch [$c gettags $item] *_ltype]
    if {$i < 0} { return "" }
    return [string range [lindex [$c gettags $item] $i] 0 end-6]
}

# --------------------------------------------------------- parsing attributi
# "color=#ff0000 width=3 dash=1" -> {color #ff0000 width 3 dash 1}
proc linkstyle_parse_attrs {toks} {
    set out {}
    foreach tk $toks {
        set e [string first "=" $tk]
        if {$e < 1} { continue }
        set k [string range $tk 0 [expr {$e-1}]]
        set v [string range $tk [expr {$e+1}] end]
        if {[lsearch -exact {color width dash} $k] >= 0} { lappend out $k $v }
    }
    return $out
}
# {color #ff0000 width 3} -> "color=#ff0000 width=3"
proc linkstyle_fmt_attrs {attrs} {
    set out {}
    foreach {k v} $attrs { lappend out "$k=$v" }
    return [join $out " "]
}

# ------------------------------------------------------------- load / save
proc linkstyle_load {} {
    array unset ::linkstyle
    array unset ::linkcat
    set f [linkstyle_file]
    if {$f eq "" || ![file exists $f]} { return }
    catch {
        set fid [open $f r]
        while {[gets $fid line] >= 0} {
            set line [string trim $line]
            if {$line eq "" || [string index $line 0] eq "#"} { continue }
            set kind [lindex $line 0]
            set attrs [linkstyle_parse_attrs [lrange $line 2 end]]
            if {$attrs eq ""} { continue }
            if {$kind eq "CAT"} {
                set tycon [lindex $line 1]
                if {$tycon ne ""} { set ::linkcat($tycon) $attrs }
            } elseif {$kind eq "LINK"} {
                set key [lindex $line 1]
                if {$key ne ""} { set ::linkstyle($key) $attrs }
            }
        }
        close $fid
    }
}

proc linkstyle_save {} {
    set f [linkstyle_file]
    if {$f eq ""} { return }
    set hascat [expr {[array exists ::linkcat]  && [array size ::linkcat]  > 0}]
    set haslnk [expr {[array exists ::linkstyle] && [array size ::linkstyle] > 0}]
    # se non c'e' piu' nulla, rimuovi il file per non lasciare residui
    if {!$hascat && !$haslnk} { catch {file delete $f}; return }
    catch {
        set fid [open $f w]
        puts $fid "# LegoPC per-connection/per-category line style (per-model, auto-generato)"
        if {$hascat} {
            foreach t [lsort [array names ::linkcat]] {
                puts $fid "CAT $t [linkstyle_fmt_attrs $::linkcat($t)]"
            }
        }
        if {$haslnk} {
            foreach k [lsort [array names ::linkstyle]] {
                puts $fid "LINK $k [linkstyle_fmt_attrs $::linkstyle($k)]"
            }
        }
        close $fid
    }
}

# ------------------------------------------------------------- applicazione
proc linkstyle_apply_attrs {c item attrs} {
    array set a $attrs
    if {[info exists a(color)] && $a(color) ne ""} { catch {$c itemconfigure $item -fill  $a(color)} }
    if {[info exists a(width)] && $a(width) ne ""} { catch {$c itemconfigure $item -width $a(width)} }
    if {[info exists a(dash)]} {
        if {$a(dash) == 1} { catch {$c itemconfigure $item -dash "-"} } \
                      else { catch {$c itemconfigure $item -dash {}} }
    }
}

# riporta un tratto al livello sottostante (default categoria + eventuale CAT)
proc linkstyle_revert_item {c item} {
    global clines
    set tycon [linkstyle_tycon $c $item]
    if {$tycon eq ""} { return }
    catch {$c itemconfigure $item -fill $clines($tycon,color) -width $clines($tycon,width) -dash {}}
    if {[info exists ::linkcat($tycon)]} { linkstyle_apply_attrs $c $item $::linkcat($tycon) }
}

# applica gli override (CAT poi LINK) a una singola categoria
proc linkstyle_apply_category {c tycon} {
    if {[info exists ::linkcat($tycon)]} {
        foreach it [$c find withtag ${tycon}_ltype] { linkstyle_apply_attrs $c $it $::linkcat($tycon) }
    }
    foreach it [$c find withtag ${tycon}_ltype] {
        set key [linkstyle_key_from_line $c $it]
        if {$key ne "" && [info exists ::linkstyle($key)]} { linkstyle_apply_attrs $c $it $::linkstyle($key) }
    }
}

# applica TUTTI gli override presenti (usato al post-load): CAT poi LINK
proc linkstyle_apply_all {c} {
    if {[array exists ::linkcat]} {
        foreach tycon [array names ::linkcat] {
            foreach it [$c find withtag ${tycon}_ltype] { linkstyle_apply_attrs $c $it $::linkcat($tycon) }
        }
    }
    if {[array exists ::linkstyle]} {
        foreach it [$c find withtag connection] {
            set key [linkstyle_key_from_line $c $it]
            if {$key ne "" && [info exists ::linkstyle($key)]} { linkstyle_apply_attrs $c $it $::linkstyle($key) }
        }
    }
}

# stile effettivo corrente di un tratto: {color .. width .. dash ..}
proc linkstyle_effective {c item} {
    global clines
    set tycon [linkstyle_tycon $c $item]
    array set eff {color "" width "" dash 0}
    if {$tycon ne ""} {
        catch { set eff(color) $clines($tycon,color) }
        catch { set eff(width) $clines($tycon,width) }
    }
    if {$tycon ne "" && [info exists ::linkcat($tycon)]} { array set eff $::linkcat($tycon) }
    set key [linkstyle_key_from_line $c $item]
    if {$key ne "" && [info exists ::linkstyle($key)]} { array set eff $::linkstyle($key) }
    return [array get eff]
}

# ------------------------------------------------------------- caricamento modello
# Chiamata dopo topRead sul canvas topologia: (ri)legge il file e riapplica.
proc linkstyle_reload {c} {
    linkstyle_load
    linkstyle_apply_all $c
}

# ------------------------------------------------------------- dialogo
proc linkstyle_dialog_current {c} {
    set item [lindex [$c find withtag $::currTid] 0]
    if {$item eq ""} { return }
    linkstyle_dialog $c $item
}

proc linkstyle_dialog {c item} {
    set tycon [linkstyle_tycon $c $item]
    set key   [linkstyle_key_from_line $c $item]
    if {$key eq ""} {
        tk_messageBox -icon warning -type ok \
            -message "Impossibile identificare la connessione (porte/moduli non risolti)."
        return
    }
    array set eff [linkstyle_effective $c $item]

    catch {destroy .lstyledlg}
    toplevel .lstyledlg
    set d .lstyledlg
    wm title $d "Connection style"
    wm resizable $d 0 0

    set kdisp [string map {| " <-> "} $key]
    label $d.info -justify left -text "$kdisp\nCategory: $tycon"
    pack $d.info -side top -anchor w -padx 10 -pady 6

    labelframe $d.scope -text "Apply to"
    pack $d.scope -side top -fill x -padx 10 -pady 4
    set ::lstyle_scope link
    radiobutton $d.scope.link -variable ::lstyle_scope -value link \
        -text "This connection only"
    radiobutton $d.scope.cat  -variable ::lstyle_scope -value cat \
        -text "All \"$tycon\" connections (this model)"
    pack $d.scope.link $d.scope.cat -side top -anchor w

    frame $d.at
    pack $d.at -side top -fill x -padx 10 -pady 4
    set ::lstyle_color $eff(color)
    label  $d.at.cl -text "Color:"
    button $d.at.cb -width 6 -command {
        set nc [tk_chooseColor -initialcolor $::lstyle_color -title "Line color"]
        if {$nc ne ""} { set ::lstyle_color $nc; catch {.lstyledlg.at.cb configure -background $nc} }
    }
    catch {$d.at.cb configure -background $eff(color)}
    grid $d.at.cl $d.at.cb -row 0 -sticky w -padx 4 -pady 2

    set ::lstyle_width [expr {$eff(width) eq "" ? 1 : $eff(width)}]
    label   $d.at.wl -text "Width:"
    spinbox $d.at.wb -from 1 -to 10 -width 4 -textvariable ::lstyle_width
    grid $d.at.wl $d.at.wb -row 1 -sticky w -padx 4 -pady 2

    set ::lstyle_dash [expr {[info exists eff(dash)] && $eff(dash) == 1 ? 1 : 0}]
    checkbutton $d.at.db -text "Dashed" -variable ::lstyle_dash -onvalue 1 -offvalue 0
    grid $d.at.db -row 2 -sticky w -padx 4 -pady 2

    frame $d.bt
    pack $d.bt -side bottom -pady 6
    button $d.bt.apply -text "Apply"            -command [list linkstyle_apply_dialog $c $item]
    button $d.bt.reset -text "Reset to default" -command [list linkstyle_reset_dialog $c $item]
    button $d.bt.close -text "Close"            -command "destroy $d"
    pack $d.bt.apply $d.bt.reset $d.bt.close -side left -padx 6
}

proc linkstyle_apply_dialog {c item} {
    set attrs {}
    if {$::lstyle_color ne ""} { lappend attrs color $::lstyle_color }
    if {$::lstyle_width ne ""} { lappend attrs width $::lstyle_width }
    lappend attrs dash $::lstyle_dash
    if {$::lstyle_scope eq "cat"} {
        set tycon [linkstyle_tycon $c $item]
        if {$tycon eq ""} { return }
        set ::linkcat($tycon) $attrs
        # applica a tutta la categoria; i tratti con override LINK restano prioritari
        foreach it [$c find withtag ${tycon}_ltype] {
            linkstyle_apply_attrs $c $it $attrs
            set key [linkstyle_key_from_line $c $it]
            if {$key ne "" && [info exists ::linkstyle($key)]} { linkstyle_apply_attrs $c $it $::linkstyle($key) }
        }
    } else {
        set key [linkstyle_key_from_line $c $item]
        if {$key eq ""} { return }
        set ::linkstyle($key) $attrs
        linkstyle_apply_attrs $c $item $attrs
    }
    linkstyle_save
    destroy .lstyledlg
}

proc linkstyle_reset_dialog {c item} {
    global clines
    if {$::lstyle_scope eq "cat"} {
        set tycon [linkstyle_tycon $c $item]
        catch {unset ::linkcat($tycon)}
        foreach it [$c find withtag ${tycon}_ltype] {
            catch {$c itemconfigure $it -fill $clines($tycon,color) -width $clines($tycon,width) -dash {}}
            set key [linkstyle_key_from_line $c $it]
            if {$key ne "" && [info exists ::linkstyle($key)]} { linkstyle_apply_attrs $c $it $::linkstyle($key) }
        }
    } else {
        set key [linkstyle_key_from_line $c $item]
        catch {unset ::linkstyle($key)}
        linkstyle_revert_item $c $item
    }
    linkstyle_save
    destroy .lstyledlg
}

# rimuove l'eventuale override LINK di un tratto in via di cancellazione.
# sport/eport = ID item delle due porte (come in linkDelete).
proc linkstyle_forget_link {c sport eport} {
    if {![array exists ::linkstyle]} { return }
    set a [linkstyle_modport $c $sport]
    set b [linkstyle_modport $c $eport]
    if {$a eq "" || $b eq ""} { return }
    set key [linkstyle_key [lindex $a 0] [lindex $a 1] [lindex $b 0] [lindex $b 1]]
    if {[info exists ::linkstyle($key)]} { unset ::linkstyle($key); linkstyle_save }
}
