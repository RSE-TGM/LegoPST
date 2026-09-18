# modelli.tcl - i modelli dell'area di lavoro, in lettura e in scrittura:
#   File -> Open Model   (modelli_dialogo)       quale modello aprire;
#   File -> Save Model   (modelli_salva_nuovo)   il nome di un modello NUOVO,
#                                                dal ramo "untitled" di topWrite;
#   File -> Save As...   (modelli_salva_come)    il nome della copia;
#   File -> Include model (modelli_includi)      il modello da includere;
#   File -> Delete Model (modelli_cancella)      il modello da mettere nel
#                                                cestino dell'area.
#
# Sorgiato da legopc.tix. Al posto dei selettori di file e dei campi "Model
# name:" di prima, l'elenco dei modelli che stanno in $LG_MODELS (su Linux
# ~/legocad, cioe' l'area scelta con lgswitch), filtrabile come Variable to
# set: il componente e' lo stesso, hmi_lista in hmielem.tcl. In scrittura
# l'elenco mostra i nomi gia' presi: un nome esistente si rifiuta, come prima.
#
# Un modello e' una directory con il .tom OMONIMO (<dir>/<dir>.tom): e' quello
# che legopc apre per nome (topRead) e che lghmi considera il modello della
# task (tom_della_task, lgedit.tcl). Le directory che hanno solo .tom di altro
# nome - copie di backup come MDC_NI0_bad/MDC_NI0.tom - non compaiono: si
# aprono ancora con Browse..., il selettore di file di prima.
#
# Per ogni modello l'elenco mostra la data dell'ultima modifica del .tom e chi
# lo sta usando: un altro legopc aperto sulla sua directory, o la sua
# simulazione in corso. Aprirlo si puo' lo stesso, dopo un avviso.

#  esegue_legopc, stessa_directory, tom_della_task: gli stessi controlli di
#  lghmi (Edit model), non una copia.
source [file join [file dirname [file normalize [info script]]] lgedit.tcl]

#  La directory fisica di <d>. file normalize risolve i link di tutti i
#  componenti TRANNE L'ULTIMO, e ~/legocad e' proprio un link (lgswitch).
proc modelli_fisica {d} {
    for {set n 0} {$n < 16} {incr n} {
        if {[catch {file type $d} tipo] || $tipo ne "link"} break
        set l [file readlink $d]
        set d [expr {[file pathtype $l] eq "absolute" ? $l : [file join [file dirname $d] $l]}]
    }
    return [file normalize $d]
}

#  Il nome dell'area per il titolo: legopst_<x> quando $LG_MODELS porta a
#  <area>/legocad, altrimenti il path.
proc modelli_nome_area {} {
    set d [modelli_fisica $::env(LG_MODELS)]
    if {[file tail $d] eq "legocad"} { return [file tail [file dirname $d]] }
    return $d
}

#  I modelli dell'area in ordine alfabetico: lista di {nome dir mtime}, con
#  mtime la data dell'ultima modifica del .tom.
proc modelli_area {} {
    set out {}
    if {![info exists ::env(LG_MODELS)]} { return {} }
    foreach d [glob -nocomplain -types d -directory $::env(LG_MODELS) *] {
        set tom [tom_della_task $d]
        if {![file isfile $tom] || [catch {file mtime $tom} quando]} continue
        lappend out [list [file tail $d] $d $quando]
    }
    return [lsort -dictionary -index 0 $out]
}

#  Gli ALTRI legopc aperti, con la loro directory corrente: {pid cwd ...}.
#  Stesso criterio di legopc_aperto_su (lgedit.tcl) - la directory corrente,
#  perche' legopc ci si porta quando apre un modello - ma in un giro solo:
#  l'elenco lo chiede per tutti i modelli insieme.
proc modelli_legopc_aperti {} {
    set out {}
    if {!$::LINUXPLAT || [catch {exec pgrep -f legopc.tix} pids]} { return {} }
    foreach p [split [string trim $pids] "\n"] {
        set p [string trim $p]
        if {$p eq "" || $p == [pid] || ![esegue_legopc $p]} continue
        if {[catch {file readlink /proc/$p/cwd} cwd]} continue
        lappend out $p $cwd
    }
    return $out
}

#  Le directory su cui gira una simulazione dell'utente: {pid dir ...}, con pid
#  quello del net_sked. net_startup fa cd nella directory del simulatore prima
#  di lanciarlo, quindi dalla sua directory corrente si arriva al file S01 e
#  alle task di processo che elenca. Nei bundle FMU la directory del
#  simulatore e' la task stessa, e conta anche lei.
#  Piu' preciso di sim_attiva (lgedit.tcl), che guarda se una simulazione
#  qualsiasi e' in corso: qui interessa quella del modello.
proc modelli_in_simulazione {} {
    set out {}
    if {!$::LINUXPLAT} { return {} }
    if {[catch {exec pgrep -x -u $::tcl_platform(user) net_sked} pids]} { return {} }
    foreach p [split [string trim $pids] "\n"] {
        set p [string trim $p]
        if {$p eq "" || [catch {file readlink /proc/$p/cwd} cwd]} continue
        lappend out $p $cwd
        set s01 [file join $cwd S01]
        if {[file isfile $s01]} {
            foreach t [parse_s01 $s01 P] { lappend out $p [lindex $t 2] }
        }
    }
    return $out
}

#  Chi usa il modello in <dir>: {pid_legopc pid_net_sked}, vuoti se nessuno.
#  Il confronto e' per identita' (stessa_directory): ~/legocad e ~/sked sono
#  link, e la stessa directory arriva con grafie diverse.
proc modelli_uso {dir aperti insim} {
    set lp ""
    set sp ""
    foreach {p cwd} $aperti {
        if {[stessa_directory $cwd $dir]} { set lp $p; break }
    }
    foreach {p d} $insim {
        if {[stessa_directory $d $dir]} { set sp $p; break }
    }
    return [list $lp $sp]
}

#  Il testo "In use" di un modello.
proc modelli_testo_uso {lp sp} {
    set uso {}
    if {$lp ne ""} { lappend uso "open in legopc (pid $lp)" }
    if {$sp ne ""} { lappend uso "simulation running" }
    return [join $uso ", "]
}

#  Una riga dell'elenco: nome, data del .tom, chi lo usa.
proc modelli_riga {nome} {
    lassign $::modelli_info($nome) dir quando lp sp
    return [format "%-*s  %-16s  %s" $::modelli_larg $nome \
                [clock format $quando -format "%Y-%m-%d %H:%M"] [modelli_testo_uso $lp $sp]]
}

#  C'e' un modello su disco, nel tab del disegno: Include model si puo' usare.
#  Lo accendeva solo il cambio di tab (raisetopol), quindi dopo Open Model o
#  Save la voce restava spenta finche' non si cambiava tab e si tornava.
proc modelli_menu_modello {} {
    catch {.menu.file entryconfigure "Include model..." -state normal}
}

#  Prepara l'elenco: riempie ::modelli_info (nome -> {dir mtime pid_legopc
#  pid_net_sked}) e ::modelli_larg, e ritorna i nomi in ordine.
proc modelli_prepara {} {
    set aperti [modelli_legopc_aperti]
    set insim [modelli_in_simulazione]
    array unset ::modelli_info
    set nomi {}
    set larg 10
    foreach m [modelli_area] {
        lassign $m nome dir quando
        lassign [modelli_uso $dir $aperti $insim] lp sp
        set ::modelli_info($nome) [list $dir $quando $lp $sp]
        lappend nomi $nome
        set larg [expr {max($larg, [string length $nome])}]
    }
    set ::modelli_larg $larg
    return $nomi
}

#  L'intestazione delle colonne, allineata a modelli_riga.
proc modelli_testata {} {
    return [format "%-*s  %-16s  %s" $::modelli_larg Model Modified "In use"]
}

#  File -> Open Model... Prima le modifiche non salvate del modello aperto,
#  con la stessa domanda del cambio di tab (avverti): rispondere No annulla
#  l'apertura, come li'. Poi l'elenco.
proc modelli_dialogo {c c2 f4} {
    if {[avverti $c]} { return }

    set w .apri_modello
    catch {destroy $w}

    set nomi [modelli_prepara]
    set larg $::modelli_larg
    set ::modelli_nome ""

    toplevel $w
    wm title $w "Open Model - [modelli_nome_area]"
    wm transient $w .
    if {[llength $nomi]} {
        set info "[llength $nomi] models in $::env(LG_MODELS)"
    } else {
        set info "No model in $::env(LG_MODELS)\n(a model is a directory <name> with <name>.tom inside):\nuse Browse... to open a .tom from elsewhere."
    }
    label $w.info -text $info -anchor w -justify left
    frame $w.f
    label $w.f.l -text "Model:"
    entry $w.f.e -textvariable ::modelli_nome -width 32
    pack $w.f.l -side left
    pack $w.f.e -side left -fill x -expand 1

    set ok [list modelli_ok $c $c2 $f4 $w]
    set lb [hmi_lista $w.l $w.f.e ::modelli_nome $nomi $ok modelli_riga \
                [modelli_testata] [expr {$larg + 50}]]

    frame $w.b
    button $w.b.ok -text Open -width 10 -command $ok
    button $w.b.sf -text "Browse..." -width 10 -command [list modelli_sfoglia $c $c2 $f4 $w]
    button $w.b.no -text Cancel -width 10 -command [list destroy $w]
    pack $w.b.ok $w.b.sf $w.b.no -side left -padx 6

    pack $w.info -side top -fill x -padx 8 -pady {8 4}
    pack $w.f -side top -fill x -padx 8 -pady 4
    pack $w.b -side bottom -pady 8
    pack $w.l -side top -fill both -expand 1 -padx 8

    bind $w.f.e <Return> $ok
    bind $w <Escape> [list destroy $w]
    focus $w.f.e
}

#  Il modello che il campo indica: il nome scritto se e' nell'elenco (anche
#  con maiuscole diverse), oppure l'unica riga rimasta dopo il filtro. ""
#  se non se ne puo' dedurre uno.
proc modelli_scelto {w} {
    set nome [string trim $::modelli_nome]
    if {[info exists ::modelli_info($nome)]} { return $nome }
    set uguali [lsearch -all -inline -nocase -exact [array names ::modelli_info] $nome]
    if {[llength $uguali] == 1} { return [lindex $uguali 0] }
    set mostrati [hmi_lista_mostrati $w.l.lb]
    if {$nome ne "" && [llength $mostrati] == 1} { return [lindex $mostrati 0] }
    return ""
}

#  Open (o doppio clic, o Invio). Chi usa il modello si ricontrolla adesso,
#  non quando si e' aperto l'elenco: nel frattempo puo' essere cambiato.
proc modelli_ok {c c2 f4 w} {
    set nome [modelli_scelto $w]
    if {$nome eq ""} {
        set t [string trim $::modelli_nome]
        tk_messageBox -parent $w -icon warning -title "Open Model" -message \
            [expr {$t eq "" ? "Choose a model from the list." \
                            : "No model named '$t': choose one from the list."}]
        return
    }
    set dir [lindex $::modelli_info($nome) 0]
    lassign [modelli_uso $dir [modelli_legopc_aperti] [modelli_in_simulazione]] lp sp
    if {$lp ne "" || $sp ne ""} {
        set msg ""
        if {$lp ne ""} {
            append msg "'$nome' is already open in another legopc (pid $lp).\n"
            append msg "Two CADs on the same model overwrite each other's saves,\n"
            append msg "and neither of them knows.\n\n"
        }
        if {$sp ne ""} {
            append msg "The simulation of '$nome' is running (net_sked pid $sp).\n"
            append msg "Saving would rewrite the model and its .i5 underneath the\n"
            append msg "running task: the executable in proc/ and the shared memory\n"
            append msg "would no longer match what is drawn.\n\n"
        }
        append msg "Open it anyway?"
        if {[tk_messageBox -parent $w -icon warning -type yesno -default no \
                 -title "Open Model" -message $msg] ne "yes"} {
            return
        }
    }
    destroy $w
    set ::curFileName "$nome.tom"
    aprimodello $c $c2 $f4 1
}

#  Browse...: il selettore di file di prima, per un .tom fuori dall'area o
#  con un nome diverso da quello della sua directory.
proc modelli_sfoglia {c c2 f4 w} {
    destroy $w
    aprimodello $c $c2 $f4 0
}

# ---------------------------------------------------------------------------
# In scrittura: il nome di un modello nuovo (Save Model, Save As...)
# ---------------------------------------------------------------------------

#  Il nome <nome> va bene per un modello NUOVO? Ritorna "" se si', altrimenti
#  il motivo. Le regole di prima (non esistente, al massimo 8 caratteri) piu':
#  solo lettere, cifre, '_' e '-' (un '/' creava sottodirectory), e nessun
#  omonimo anche con maiuscole diverse - SLB1_NI2 e slb1_ni2 sarebbero due
#  directory diverse solo su Linux. Contano tutte le voci dell'area, non solo
#  i modelli: libgraph, le task r_*, i backup.
proc modelli_nome_nuovo {nome} {
    if {$nome eq ""} { return "Type the name of the new model." }
    if {![regexp {^[A-Za-z0-9_-]+$} $nome]} {
        return "Use only letters, digits, '_' and '-'."
    }
    if {[string length $nome] > 8} { return "Name too long (max 8 characters)." }
    foreach v [glob -nocomplain -directory $::env(LG_MODELS) -tails *] {
        if {[string equal -nocase $v $nome]} {
            return "'$v' already exists in the area: choose another name."
        }
    }
    return ""
}

#  Riga di stato sotto il campo, in tempo reale, e bottone Save acceso solo con
#  un nome valido.
proc modelli_valida {w} {
    set nome [string trim $::modelli_nome]
    set err [modelli_nome_nuovo $nome]
    if {$err eq ""} {
        $w.stato configure -fg darkgreen \
            -text "New model: [file join $::env(LG_MODELS) $nome $nome.tom]"
        $w.b.ok configure -state normal
    } else {
        $w.stato configure -fg [expr {$nome eq "" ? "gray40" : "darkred"}] -text $err
        $w.b.ok configure -state disabled
    }
}

#  Save (o Invio, o doppio clic su una riga): accetta solo un nome valido; la
#  riga di stato dice gia' perche' no.
proc modelli_nome_ok {w} {
    set nome [string trim $::modelli_nome]
    if {[modelli_nome_nuovo $nome] ne ""} { bell; return }
    set ::modelli_esito $nome
}

#  Il dialogo in scrittura: lo stesso elenco di Open Model, per scegliere il
#  NOME di un modello nuovo. Sincrono e modale, come i campi che sostituisce:
#  ritorna il nome scelto, "" se annullato.
proc modelli_chiedi_nome {titolo intro} {
    set w .nome_modello
    catch {destroy $w}
    set nomi [modelli_prepara]
    set larg $::modelli_larg
    set ::modelli_nome ""
    set ::modelli_esito ""

    toplevel $w
    wm title $w "$titolo - [modelli_nome_area]"
    wm transient $w .
    wm protocol $w WM_DELETE_WINDOW {set ::modelli_esito ""; set ::modelli_annulla 1}
    label $w.info -text $intro -anchor w -justify left
    frame $w.f
    label $w.f.l -text "Model:"
    entry $w.f.e -textvariable ::modelli_nome -width 32
    pack $w.f.l -side left
    pack $w.f.e -side left -fill x -expand 1
    label $w.stato -anchor w -justify left
    set ok [list modelli_nome_ok $w]
    set lb [hmi_lista $w.l $w.f.e ::modelli_nome $nomi $ok modelli_riga \
                [modelli_testata] [expr {$larg + 50}]]
    label $w.l2 -anchor w -fg gray40 \
        -text "Models already in $::env(LG_MODELS) ([llength $nomi]):"

    frame $w.b
    button $w.b.ok -text Save -width 10 -command $ok
    button $w.b.no -text Cancel -width 10 \
        -command {set ::modelli_esito ""; set ::modelli_annulla 1}
    pack $w.b.ok $w.b.no -side left -padx 6

    pack $w.info -side top -fill x -padx 8 -pady {8 4}
    pack $w.f -side top -fill x -padx 8 -pady 2
    pack $w.stato -side top -fill x -padx 8 -pady {0 6}
    pack $w.b -side bottom -pady 8
    pack $w.l2 -side top -fill x -padx 8
    pack $w.l -side top -fill both -expand 1 -padx 8

    #  la validazione si aggiunge (+) al filtro che hmi_lista ha gia' legato
    bind $w.f.e <KeyRelease> +[list modelli_valida $w]
    bind $lb <<ListboxSelect>> +[list modelli_valida $w]
    bind $w.f.e <Return> $ok
    bind $w <Escape> {set ::modelli_esito ""; set ::modelli_annulla 1}
    modelli_valida $w
    focus $w.f.e
    catch {grab $w}

    #  si esce con un nome valido (::modelli_esito) o con Cancel/Escape/chiusura
    set ::modelli_annulla 0
    while {$::modelli_esito eq "" && !$::modelli_annulla} {
        vwait ::modelli_esito
    }
    catch {grab release $w}
    destroy $w
    return $::modelli_esito
}

#  Save Model di un modello mai salvato (topWrite, fileio.tcl, ramo
#  "untitled"): il nome nel dialogo, poi la directory e il salvataggio come
#  faceva il campo di prima. Ritorna 0 se ha salvato, 1 se annullato o
#  fallito: e' il valore che topWrite restituisce ai suoi chiamanti.
proc modelli_salva_nuovo {c envir} {
    set nome [modelli_chiedi_nome "Save Model" \
        "Name of the new model. It must be a new name: the models\nalready in the area are listed below."]
    if {$nome eq ""} { return 1 }
    set dir [file join $::env(LG_MODELS) $nome]
    if {[catch {file mkdir $dir} err]} {
        tk_messageBox -parent . -icon error -title "Save Model" \
            -message "Cannot create the model directory:\n$dir\n\n$err"
        return 1
    }
    set ::curFileName [file join $dir $nome.tom]
    set ::DIRMODEL $dir
    set ::f14File [file join $dir f14.dat]
    cd $dir
    set esito [expr {[writeFiles $c $::curFileName $envir] ? 1 : 0}]
    if {$esito == 0} { modelli_menu_modello }
    return $esito
}

#  I file che Save As copia, oltre al .tom: i dati del modello (come prima) e i
#  file laterali che portano il nome del modello - .remap (display, faceplate,
#  set value) e .lstyle (stili delle connessioni) - rinominati. proc/ e i file
#  di build no: la copia si ricompila quando la si apre.
set ::modelli_dati_copia {f14.dat f01.dat foraus.for tasks.dat simul.dat}
set ::modelli_laterali_copia {remap lstyle}

#  File -> Save As...: copia il modello aperto con un nome nuovo e passa alla
#  copia. Prima le modifiche non salvate (avverti, come Open Model: No
#  annulla), cosi' la copia - che si fa dai file su disco - parte dall'ultima
#  versione del disegno.
proc modelli_salva_come {c c2 c4 f4} {
    global curFileName showon
    if {$curFileName eq "untitled" || $curFileName eq "" || $curFileName eq "-"} {
        tk_messageBox -parent . -icon error -title "Save As" \
            -message "No Model to 'Save As...'"
        return
    }
    if {[avverti $c]} { return }

    set src [file dirname $curFileName]
    set vecchio [file rootname [file tail $curFileName]]
    set nome [modelli_chiedi_nome "Save As" \
        "Copy of '$vecchio' as a new model. The copy takes the .tom, f01/f14,\nforaus.for, tasks.dat, simul.dat, .remap and .lstyle, and is\nrebuilt when opened. The models already in the area are listed below."]
    if {$nome eq ""} { return }

    set dst [file join $::env(LG_MODELS) $nome]
    . configure -cursor watch
    update idletasks
    set err ""
    if {[catch {
        file mkdir $dst
        file copy $curFileName [file join $dst $nome.tom]
        foreach f $::modelli_dati_copia {
            if {[file exists [file join $src $f]]} {
                file copy -force [file join $src $f] $dst
            }
        }
        foreach ext $::modelli_laterali_copia {
            set f [file join $src $vecchio.$ext]
            if {[file exists $f]} {
                file copy -force $f [file join $dst $nome.$ext]
            }
        }
    } err]} {
        #  la directory l'ha creata questo Save As (il nome era nuovo): via
        catch {file delete -force $dst}
        . configure -cursor arrow
        tk_messageBox -parent . -icon error -title "Save As" \
            -message "Cannot copy '$vecchio' to:\n$dst\n\n$err"
        return
    }

    #  si passa alla copia, come faceva DupModel
    set ::modified 1
    set curFileName [file join $dst $nome.tom]
    if {![apri_modello $c $c2 1]} {
        modelli_menu_modello
        ShowNamesfilt $c $c2 $showon
        if {$::LINUXNOMATLAB != 1} {
            open_simul $f4 $::DIRMODEL/tasks.dat
            load_task $::DIRMODEL $c4
        }
    }
    . configure -cursor arrow
}

# ---------------------------------------------------------------------------
# Include model: un altro modello dell'area dentro quello aperto
# ---------------------------------------------------------------------------
#
# La fusione la fa inhoud (Alg_legopc/src/inhoud), come prima: nella directory
# del modello corrente, "inhoud <corrente> <incluso> <N|S|W|E>" legge i due
# .tom DAL DISCO e scrive inhoud.tom, che prende il posto di <corrente>.tom.
# I blocchi dell'incluso con un nome gia' usato vengono rinominati (quarto
# carattere) e le coppie vecchio/nuovo vanno in changed.out. Poi il modello si
# ricarica e si ricompila. Rispetto a prima:
#   - prima la domanda sulle modifiche non salvate (avverti), perche' inhoud
#     lavora sul file;
#   - una copia <corrente>.tom.bak prima di sostituirlo;
#   - gli errori di inhoud si mostrano, e il .tom corrente resta intatto;
#   - .remap e .lstyle dell'incluso si fondono nel corrente, con i nomi
#     rinominati;
#   - i blocchi rinominati si mostrano in un dialogo.
# I dati (f14) del modello incluso restano fuori, come prima: la
# ricompilazione parte dall'f14 del modello corrente.

#  inhoud accetta solo i nomi e cerca l'incluso in ../<nome>, cioe' accanto al
#  modello corrente: i due devono stare nella stessa area, quella dell'elenco.
proc modelli_includi {c} {
    global curFileName
    if {$curFileName eq "untitled" || $curFileName eq "" || $curFileName eq "-"} {
        tk_messageBox -parent . -icon error -title "Include Model" \
            -message "No model is open: open the model to include into first."
        return
    }
    set dir [file dirname $curFileName]
    if {![stessa_directory [file dirname $dir] $::env(LG_MODELS)]} {
        tk_messageBox -parent . -icon warning -title "Include Model" -message \
            "The open model is not in the work area:\n$dir\n\nInclude works between models of the same area ($::env(LG_MODELS))."
        return
    }
    if {[avverti $c]} { return }

    set corrente [file rootname [file tail $curFileName]]
    set w .includi_modello
    catch {destroy $w}

    #  l'elenco di Open Model, senza il modello corrente
    set nomi [modelli_prepara]
    set i [lsearch -exact $nomi $corrente]
    if {$i >= 0} { set nomi [lreplace $nomi $i $i] }
    unset -nocomplain ::modelli_info($corrente)
    set larg $::modelli_larg
    set ::modelli_nome ""
    if {![info exists ::modelli_direzione]} { set ::modelli_direzione N }

    toplevel $w
    wm title $w "Include Model - [modelli_nome_area]"
    wm transient $w .
    label $w.info -anchor w -justify left -text \
        "Model to include into '$corrente'. [llength $nomi] models in $::env(LG_MODELS)"
    frame $w.f
    label $w.f.l -text "Model:"
    entry $w.f.e -textvariable ::modelli_nome -width 32
    pack $w.f.l -side left
    pack $w.f.e -side left -fill x -expand 1

    set ok [list modelli_includi_ok $c $w]
    set lb [hmi_lista $w.l $w.f.e ::modelli_nome $nomi $ok modelli_riga \
                [modelli_testata] [expr {$larg + 50}]]

    frame $w.d
    label $w.d.l -text "Place it:"
    pack $w.d.l -side left
    foreach {v t} {N "above" S "below" W "on the left" E "on the right"} {
        radiobutton $w.d.[string tolower $v] -text $t -variable ::modelli_direzione -value $v
        pack $w.d.[string tolower $v] -side left -padx 4
    }

    frame $w.b
    button $w.b.ok -text Include -width 10 -command $ok
    button $w.b.no -text Cancel -width 10 -command [list destroy $w]
    pack $w.b.ok $w.b.no -side left -padx 6

    pack $w.info -side top -fill x -padx 8 -pady {8 4}
    pack $w.f -side top -fill x -padx 8 -pady 4
    pack $w.b -side bottom -pady 8
    pack $w.d -side bottom -fill x -padx 8 -pady {6 0}
    pack $w.l -side top -fill both -expand 1 -padx 8

    bind $w.f.e <Return> $ok
    bind $w <Escape> [list destroy $w]
    focus $w.f.e
}

#  Include (o doppio clic, o Invio). Conta chi usa il modello CORRENTE: e'
#  quello che si riscrive e si ricompila. L'incluso si legge soltanto.
proc modelli_includi_ok {c w} {
    set nome [modelli_scelto $w]
    if {$nome eq ""} {
        set t [string trim $::modelli_nome]
        tk_messageBox -parent $w -icon warning -title "Include Model" -message \
            [expr {$t eq "" ? "Choose the model to include from the list." \
                            : "No model named '$t': choose one from the list."}]
        return
    }
    set dir [file dirname $::curFileName]
    set corrente [file rootname [file tail $::curFileName]]
    lassign [modelli_uso $dir [modelli_legopc_aperti] [modelli_in_simulazione]] lp sp
    if {$lp ne "" || $sp ne ""} {
        set msg ""
        if {$lp ne ""} {
            append msg "'$corrente' is also open in another legopc (pid $lp).\n"
            append msg "Its next save would overwrite the model with the inclusion.\n\n"
        }
        if {$sp ne ""} {
            append msg "The simulation of '$corrente' is running (net_sked pid $sp).\n"
            append msg "Including rewrites the model and rebuilds f01/f14 underneath\n"
            append msg "the running task.\n\n"
        }
        append msg "Include anyway?"
        if {[tk_messageBox -parent $w -icon warning -type yesno -default no \
                 -title "Include Model" -message $msg] ne "yes"} {
            return
        }
    }
    destroy $w
    modelli_esegui_inclusione $c $corrente $dir $nome $::modelli_direzione
}

#  Il nome <n> dopo le rinomine di inhoud (dict vecchio -> nuovo).
proc modelli_rinomina {n mappa} {
    if {[dict exists $mappa $n]} { return [dict get $mappa $n] }
    return $n
}

#  Una variabile LEGO porta negli ultimi 4 caratteri il nome del suo blocco
#  (WALIPGCD e' WALI del blocco PGCD): se il blocco e' stato rinominato, la
#  ricompilazione le da' il nome nuovo, e cosi' deve fare il .remap.
proc modelli_rinomina_var {v mappa} {
    if {[string length $v] != 8} { return $v }
    set blocco [string range $v 4 7]
    if {![dict exists $mappa $blocco]} { return $v }
    return "[string range $v 0 3][dict get $mappa $blocco]"
}

#  changed.out di inhoud -> dict vecchio -> nuovo. Due righe di intestazione,
#  poi coppie di nomi; i nomi possono contenere $ ? ! (i simboli delle
#  rinomine), quindi si spezzano come testo, non come lista Tcl.
proc modelli_leggi_changed {f} {
    set m [dict create]
    if {[catch {open $f r} fid]} { return $m }
    set righe [split [read $fid] "\n"]
    close $fid
    set tok [regexp -all -inline {\S+} [join [lrange $righe 2 end] " "]]
    foreach {vecchio nuovo} $tok {
        if {$nuovo ne ""} { dict set m $vecchio $nuovo }
    }
    return $m
}

#  Aggiunge al .remap del modello corrente le voci del modello incluso, con i
#  nomi rinominati: la chiave (l'istanza) e il valore quando e' una variabile.
#  Il valore di un faceplate (;F) e' una pagina di r02.dat e resta com'e'.
#  Ritorna quante voci ha aggiunto.
proc modelli_fondi_remap {remap_cor remap_inc mappa} {
    if {![file isfile $remap_inc]} { return 0 }
    set d [anim_remap_leggi $remap_cor]
    set n 0
    dict for {inst vm} [anim_remap_leggi $remap_inc] {
        lassign $vm v m
        if {$m ne "F"} { set v [modelli_rinomina_var $v $mappa] }
        dict set d [modelli_rinomina $inst $mappa] [list $v $m]
        incr n
    }
    if {$n == 0} { return 0 }
    set fid [open $remap_cor w]
    puts $fid "# LegoPC animation remap - generato automaticamente"
    foreach k [lsort [dict keys $d]] {
        lassign [dict get $d $k] v m
        puts $fid [expr {$m ne "" ? "$k=$v;$m" : "$k=$v"}]
    }
    close $fid
    return $n
}

#  Aggiunge al .lstyle del modello corrente gli stili dei SINGOLI tratti
#  (LINK) del modello incluso, con i nomi dei blocchi rinominati. Gli
#  override di CATEGORIA (CAT) dell'incluso no: valgono per tutto il modello e
#  ricolorerebbero anche i tratti del corrente. Ritorna quanti tratti ha
#  aggiunto.
proc modelli_fondi_lstyle {ls_cor ls_inc mappa} {
    if {![file isfile $ls_inc]} { return 0 }
    set nuovi [dict create]
    set fid [open $ls_inc r]
    while {[gets $fid riga] >= 0} {
        set riga [string trim $riga]
        if {$riga eq "" || [string index $riga 0] eq "#"} continue
        if {[lindex $riga 0] ne "LINK"} continue
        set capi [split [lindex $riga 1] "|"]
        if {[llength $capi] != 2} continue
        set mp {}
        foreach capo $capi {
            set punto [string first "." $capo]
            if {$punto < 1} { set mp {}; break }
            lappend mp [modelli_rinomina [string range $capo 0 [expr {$punto-1}]] $mappa] \
                       [string range $capo [expr {$punto+1}] end]
        }
        if {[llength $mp] != 4} continue
        dict set nuovi [linkstyle_key {*}$mp] [lrange $riga 2 end]
    }
    close $fid
    if {[dict size $nuovi] == 0} { return 0 }

    #  le righe del corrente restano tali e quali, tranne un LINK con la stessa
    #  chiave (una voce rimasta indietro: vince quella dell'incluso)
    set righe {}
    if {![catch {open $ls_cor r} fid]} {
        while {[gets $fid riga] >= 0} {
            set t [string trim $riga]
            if {[lindex $t 0] eq "LINK" && [dict exists $nuovi [lindex $t 1]]} continue
            lappend righe $riga
        }
        close $fid
    }
    if {[llength $righe] == 0} {
        lappend righe "# LegoPC per-connection/per-category line style (per-model, auto-generato)"
    }
    dict for {k attrs} $nuovi { lappend righe "LINK $k $attrs" }
    set fid [open $ls_cor w]
    puts $fid [join $righe "\n"]
    close $fid
    return [dict size $nuovi]
}

#  L'inclusione vera e propria. Il .tom corrente si tocca solo se inhoud ha
#  finito bene; prima se ne fa una copia <corrente>.tom.bak.
proc modelli_esegui_inclusione {c corrente dir nome direzione} {
    global env
    set tom [file join $dir $corrente.tom]
    set inh [file join $env(LG_TOOLS) inhoud]
    set dove [dict get {N above S below W "on the left" E "on the right"} $direzione]
    . configure -cursor watch
    update idletasks
    set old [pwd]
    cd $dir
    catch {file delete -force inhoud.tom changed.out}

    set err ""
    if {[catch {file copy -force $tom $tom.bak} e]} {
        set err "Cannot make the backup copy $corrente.tom.bak:\n$e"
    } elseif {[catch {exec $inh $corrente $nome $direzione} e]} {
        #  inhoud scrive i suoi messaggi senza "a capo" finale, e Tcl ci
        #  attacca il proprio: si tiene solo il messaggio di inhoud
        regsub {\s*child process exited abnormally\s*$} $e "" e
        set err "inhoud failed:\n\n$e"
    } elseif {![file isfile inhoud.tom] || [file size inhoud.tom] == 0} {
        set err "inhoud produced no model (inhoud.tom is missing or empty)."
    }
    if {$err ne ""} {
        catch {file delete -force inhoud.tom}
        cd $old
        . configure -cursor arrow
        tk_messageBox -parent . -icon error -title "Include Model" -message \
            "Cannot include '$nome' into '$corrente'.\n\n$err\n\nThe model was not changed."
        return
    }
    file rename -force inhoud.tom $corrente.tom
    set mappa [modelli_leggi_changed changed.out]

    #  assegnazioni e stili dell'incluso, con i nomi nuovi
    set mdir [file join $env(LG_MODELS) $nome]
    set nremap 0
    set nlstyle 0
    catch {set nremap [modelli_fondi_remap [file join $dir $corrente.remap] \
                           [file join $mdir $nome.remap] $mappa]}
    catch {set nlstyle [modelli_fondi_lstyle [file join $dir $corrente.lstyle] \
                            [file join $mdir $nome.lstyle] $mappa]}

    #  come prima: via .top e f01.dat, si ricarica e si ricompila
    catch {file delete -force [file join $dir $corrente.top] [file join $dir f01.dat]}
    cd $old
    if {[topRead $c $corrente]} {
        . configure -cursor arrow
        return
    }
    catch {linkstyle_reload $c}
    catch {hmi_aggiorna_etichette $c}
    catch {hmi_segnaposti $c}
    set ::modified 1
    set ::needbuild 1
    if {$::LINUXPLAT == 0} {
        set compilato [buildLegoFiles $c]
    } else {
        set compilato [buildLegoFiles_linux $c]
    }
    . configure -cursor arrow
    modelli_esito_inclusione $nome $corrente $dove $mappa $nremap $nlstyle $dir
}

#  Com'e' andata: i blocchi rinominati (se ce ne sono), in una finestra con
#  l'elenco scorrevole - includere un modello in un altro con molti nomi
#  comuni ne rinomina decine.
proc modelli_esito_inclusione {nome corrente dove mappa nremap nlstyle dir} {
    if {[dict size $mappa] == 0} return
    set w .inclusione_esito
    catch {destroy $w}
    toplevel $w
    wm title $w "Include Model - renamed blocks"
    wm transient $w .
    set t "'$nome' included into '$corrente' ($dove).\n\n"
    append t "[dict size $mappa] blocks of '$nome' were renamed, because the name was\n"
    append t "already used:"
    label $w.t -text $t -anchor w -justify left
    frame $w.l
    listbox $w.l.lb -height [expr {min(15, [dict size $mappa])}] -width 28 \
        -font {Courier 10} -yscrollcommand [list $w.l.sb set]
    scrollbar $w.l.sb -command [list $w.l.lb yview]
    pack $w.l.sb -side right -fill y
    pack $w.l.lb -side left -fill both -expand 1
    dict for {vecchio nuovo} $mappa { $w.l.lb insert end "  $vecchio  ->  $nuovo" }
    set p "Display, faceplate and set value assignments ($nremap) and line\n"
    append p "styles ($nlstyle) of '$nome' follow the new names.\n"
    append p "The list is also in [file join $dir changed.out]."
    label $w.p -text $p -anchor w -justify left
    button $w.ok -text OK -width 10 -command [list destroy $w]
    pack $w.t -side top -fill x -padx 8 -pady {8 4}
    pack $w.l -side top -fill both -expand 1 -padx 8
    pack $w.p -side top -fill x -padx 8 -pady 4
    pack $w.ok -side top -pady 8
    bind $w <Return> [list destroy $w]
    bind $w <Escape> [list destroy $w]
    focus $w.ok
}

# ---------------------------------------------------------------------------
# Delete Model: il modello va nel cestino dell'area
# ---------------------------------------------------------------------------
#
# Nessuna cancellazione vera: la directory si SPOSTA in
# $LG_MODELS/.deleted/<nome>_<data>. Il punto la nasconde a tutti gli elenchi
# (glob * non vede le directory nascoste: quelli di legopc e il dir-scan di
# lghmi), segue l'area quando lgswitch la sposta, e si recupera con un mv. Il
# cestino si svuota a mano.
#
# Si rifiuta se il modello e' in uso: aperto in questo legopc o in un altro,
# o con la sua simulazione in corso. Si avvisa, con la possibilita' di
# proseguire, se lo usa un simulatore dell'area (il suo S01 lo elenca) o se
# altri processi lavorano nella sua directory.

proc modelli_cestino {} {
    return [file join $::env(LG_MODELS) .deleted]
}

#  I simulatori dell'area che usano il modello in <dir>: quelli il cui S01,
#  in $KSKED (~/sked, il link dell'area come ~/legocad), lo elenca tra le task.
proc modelli_simulatori_di {dir} {
    set sked [expr {[info exists ::env(KSKED)] ? $::env(KSKED) : [file join $::env(HOME) sked]}]
    set out {}
    foreach s01 [lsort [glob -nocomplain -directory $sked */S01]] {
        foreach t [parse_s01 $s01 {P R}] {
            if {[stessa_directory [lindex $t 2] $dir]} {
                lappend out [file tail [file dirname $s01]]
                break
            }
        }
    }
    return $out
}

#  Gli altri processi dell'utente al lavoro nella directory <dir> (una HMI
#  draw2gr, una shell, un editor): {pid nome ...}. Non bloccano - spostare la
#  directory non toglie loro niente - ma quello che salvano finisce nel
#  cestino. Stesso criterio di processi_nell_area di lghmi: la directory
#  corrente in /proc.
proc modelli_processi_in {dir} {
    set out {}
    if {!$::LINUXPLAT} { return {} }
    set d [modelli_fisica $dir]
    foreach p [glob -nocomplain -types d -directory /proc {[0-9]*}] {
        set pid [file tail $p]
        if {$pid == [pid] || [catch {file readlink [file join $p cwd]} cwd]} continue
        if {$cwd ne $d && [string first "$d/" $cwd] != 0} continue
        set nome "?"
        catch {
            set f [open [file join $p comm]]
            set nome [string trim [read $f]]
            close $f
        }
        lappend out $pid $nome
    }
    return $out
}

#  Quanto pesa la directory, per la conferma.
proc modelli_dimensione {dir} {
    if {![catch {exec du -sk $dir} out]} {
        set kb [lindex $out 0]
        if {[string is integer -strict $kb]} {
            if {$kb >= 1024} { return [format "%.0f MB" [expr {$kb / 1024.0}]] }
            return "$kb KB"
        }
    }
    return "size unknown"
}

#  File -> Delete Model...
proc modelli_cancella {c} {
    set w .cancella_modello
    catch {destroy $w}
    set nomi [modelli_prepara]
    set larg $::modelli_larg
    set ::modelli_nome ""

    toplevel $w
    wm title $w "Delete Model - [modelli_nome_area]"
    wm transient $w .
    label $w.info -anchor w -justify left -text \
        "Model to delete. It is moved to the trash of the area,\n[modelli_cestino],\nand can be restored from there."
    frame $w.f
    label $w.f.l -text "Model:"
    entry $w.f.e -textvariable ::modelli_nome -width 32
    pack $w.f.l -side left
    pack $w.f.e -side left -fill x -expand 1

    set ok [list modelli_cancella_ok $c $w]
    set lb [hmi_lista $w.l $w.f.e ::modelli_nome $nomi $ok modelli_riga \
                [modelli_testata] [expr {$larg + 50}]]

    frame $w.b
    button $w.b.ok -text Delete -width 10 -command $ok \
        -background #b22222 -foreground white \
        -activebackground #d03030 -activeforeground white
    button $w.b.no -text Cancel -width 10 -command [list destroy $w]
    pack $w.b.ok $w.b.no -side left -padx 6

    pack $w.info -side top -fill x -padx 8 -pady {8 4}
    pack $w.f -side top -fill x -padx 8 -pady 4
    pack $w.b -side bottom -pady 8
    pack $w.l -side top -fill both -expand 1 -padx 8

    #  Invio NON cancella: con un'azione distruttiva ci vuole il bottone (o
    #  il doppio clic), e comunque segue la conferma
    bind $w <Escape> [list destroy $w]
    focus $w.f.e
}

#  Delete (o doppio clic): i rifiuti, poi la conferma, poi lo spostamento.
proc modelli_cancella_ok {c w} {
    set nome [modelli_scelto $w]
    if {$nome eq ""} {
        set t [string trim $::modelli_nome]
        tk_messageBox -parent $w -icon warning -title "Delete Model" -message \
            [expr {$t eq "" ? "Choose the model to delete from the list." \
                            : "No model named '$t': choose one from the list."}]
        return
    }
    set dir [lindex $::modelli_info($nome) 0]

    #  in uso: rifiuto, non avviso - si cancellerebbe sotto chi lo usa
    set perche ""
    if {$::curFileName ne "untitled" && $::curFileName ne "" && $::curFileName ne "-" \
            && [stessa_directory [file dirname $::curFileName] $dir]} {
        set perche "'$nome' is the model open in this legopc.\nOpen another model first, or start a new one (New Model)."
    } else {
        lassign [modelli_uso $dir [modelli_legopc_aperti] [modelli_in_simulazione]] lp sp
        if {$sp ne ""} {
            set perche "The simulation of '$nome' is running (net_sked pid $sp).\nStop the simulation first."
        } elseif {$lp ne ""} {
            set perche "'$nome' is open in another legopc (pid $lp).\nClose it there first."
        }
    }
    if {$perche ne ""} {
        tk_messageBox -parent $w -icon error -title "Delete Model" \
            -message "Cannot delete '$nome'.\n\n$perche"
        return
    }

    #  destinazione: <cestino>/<nome>_<data>, con un suffisso se esiste gia'
    set base [file join [modelli_cestino] ${nome}_[clock format [clock seconds] -format %Y%m%d_%H%M%S]]
    set dest $base
    for {set i 1} {[file exists $dest]} {incr i} { set dest ${base}_$i }

    set msg "Delete model '$nome'?\n\n    $dir   ([modelli_dimensione $dir])\n\n"
    append msg "It is moved to the trash of the area:\n    $dest\n"
    append msg "and can be restored from there."
    set sim [modelli_simulatori_di $dir]
    if {[llength $sim]} {
        append msg "\n\nWARNING: it is a task of the simulator(s) [join $sim {, }]:\n"
        append msg "their S01 lists it. They will not update (kUpSim) or start\n"
        append msg "until the model is restored or removed from their S01."
    }
    set proc [modelli_processi_in $dir]
    if {[llength $proc]} {
        set el {}
        foreach {p n} $proc { lappend el "$n (pid $p)" }
        append msg "\n\nThese processes are working in its directory:\n    [join $el {, }]\n"
        append msg "Whatever they save from now on would end up in the trash."
    }
    append msg "\n\nDelete it?"
    if {[tk_messageBox -parent $w -icon warning -type yesno -default no \
             -title "Delete Model" -message $msg] ne "yes"} {
        return
    }

    if {[catch {
        file mkdir [modelli_cestino]
        file rename $dir $dest
    } err]} {
        tk_messageBox -parent $w -icon error -title "Delete Model" \
            -message "Cannot move '$nome' to the trash:\n\n$err\n\nThe model was not changed."
        return
    }
    destroy $w
    tk_messageBox -parent . -icon info -title "Delete Model" -message \
        "'$nome' moved to the trash:\n    $dest\n\nTo restore it:\n    mv $dest $dir"
}
