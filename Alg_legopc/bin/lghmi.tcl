# lghmi.tcl - selettore grafico delle task LegoPST -> lancia la HMI draw2gr.
# Si avvia con l'helper "lghmi" (Alg_rt/bin/lghmi), che sorgia il profilo e fa
# `wish $LG_TIX/lghmi.tcl`.
#
# Due modalita', scelte automaticamente in base alla cwd di lancio:
#
#  * Modalita' S01 (se nella cwd esiste un file "S01"): la lista delle task
#    viene letta dal file S01, che descrive un simulatore complessivo composto
#    da piu' task lego. Vengono elencate SOLO le task di PROCESSO (tipo P) -
#    le uniche ad avere un file di topologia .tom. Ogni task viene lanciata
#    dalla propria directory modello (path relativo risolto rispetto al S01).
#
#  * Modalita' dir-scan (default, nessun S01): elenca le sottodir di
#    $LG_TASKROOT (default $HOME/legocad) che contengono almeno un *.tom (le
#    dir di libreria come libgraph/libut, prive di .tom, sono quindi escluse).
#
# Con l'opzione -staz il selettore cambia oggetto: invece delle task elenca le
# PAGINE DI FACEPLATE (stazioni di comando) compilate in r02.dat, e apre quella
# scelta con xstaz. Serve a chi gestisce la simulazione con net_startup, che
# monta il banco (new_monit) e non ha il dialogo delle stazioni di net_monit.
#
# La barra in basso ha anche un pulsante "mmi" che lancia l'applicazione MMI
# (Alg_mmi): non dipende dalle liste, sceglie la directory di lavoro fra $KPAGES,
# $LG_SIM_PATH/globpages, $KPAGES, ./globpages e la cwd, perche' mmi legge il
# Context.ctx della dir da cui parte.
#
# Accanto c'e' il pulsante "net_startup", che lancia la simulazione della
# directory corrente. E' abilitato solo dove esiste variabili.rtf, il file che
# net_startup controlla per primo, e chiede sempre conferma perche' comincia con
# killsim (che cancella tutte le SHM dell'utente). La simulazione parte in una
# SESSIONE PROPRIA e il suo output va in una finestra di log del selettore, non
# in un terminale: chiudere quella finestra non ferma piu' la simulazione (in un
# terminale la ammazzava - vedi lancia_net_startup), e da li' la si puo' anche
# fermare per davvero.
#
# File -> Work area cambia l'area di lavoro (i link ~/legocad e ~/sked) con
# lgswitch, dopo aver controllato che niente lavori ancora sull'area corrente;
# l'area corrente sta nel titolo e nella prima riga dell'intestazione.
#
# Tools -> Edit model apre la task selezionata in legopc. I controlli e il
# lancio stanno in lgedit.tcl, condivisi con il menu Edit di draw2gr: le HMI
# lanciate da qui lo hanno (draw2gr -edit) salvo -noedit, -insim o task di
# un'altra area - vedi hmi_con_edit.
#
# La barra dei menu ha File -> Open Simulator path, che cambia a runtime la
# directory di lavoro del selettore: equivale a rilanciare lghmi da quella
# directory, e aggiorna modalita', liste, Set Sim path e stato del pulsante di
# lancio.
#
# In entrambe, selezionando una task la HMI viene lanciata in un processo
# INDIPENDENTE (detached):
#     cd <task> ; wish $LG_TIX/draw2gr.tcl 1 f22circ
# Il selettore resta aperto per altre scelte. "Quit" chiude SOLO il selettore:
# le HMI gia' aperte restano vive (le chiude l'utente), e cosi' la simulazione -
# con Quit se ne va anche la finestra di log, ma il log resta in /tmp.

package require Tk

# openhelp.tcl porta open_hlp (il manuale dei moduli, come nel ? di legopc) e
# browser_disponibile, che sceglie il browser partendo da $LG_BROWSER. Se non
# c'e' si perde solo il menu "?": il selettore funziona comunque.
catch {source [file join $env(LG_TIX) openhelp.tcl]}

# md2html.tcl converte i .md in HTML in Tcl puro, senza dipendere da pacchetti
# esterni: e' cosi' che la documentazione si vede formattata su OGNI
# installazione di LegoPST, non solo dove qualcuno ha installato pandoc.
catch {source [file join $env(LG_TIX) md2html.tcl]}

# balloon.tcl porta set_balloon, l'aiuto a comparsa gia' usato da draw2gr: si
# usa quello invece di rifarne uno qui, cosi' i suggerimenti hanno lo stesso
# aspetto e lo stesso ritardo in tutta l'interfaccia. Se il file non c'e' si
# perdono solo i suggerimenti (vedi aiuto_a_comparsa).
catch {source [file join $env(LG_TIX) balloon.tcl]}

# lgedit.tcl porta la modifica del modello con legopc (Tools -> Edit model) e i
# controlli che il selettore usa anche altrove (sim_attiva, stessa_directory,
# area_della_task). Non e' facoltativo come i precedenti: senza, mezzo
# selettore non funziona. Si cerca accanto a questo script, che e' dove il
# makefile e build.sh (bundle FMU) lo mettono, anche con LG_TIX non definito.
#
# lgstaz.tcl, allo stesso modo, porta i faceplate: parse_s01, le pagine di
# r02.dat e l'apertura con xstaz, condivise con i bottoni faceplate delle
# pagine di legopc e draw2gr.
foreach _lib {lgedit.tcl lgstaz.tcl} {
    if {[catch {source [file join [file dirname [file normalize [info script]]] $_lib]} err]} {
        tk_messageBox -icon error -title "lghmi" -message \
            "Cannot load $_lib, which must sit next to lghmi.tcl:\n\n$err\n\nReinstall the Tcl scripts (make in Alg_legopc/src/tix)."
        exit 1
    }
}
unset -nocomplain _lib

set TASKROOT [expr {[info exists env(LG_TASKROOT)] && $env(LG_TASKROOT) ne "" \
                    ? $env(LG_TASKROOT) : [file join $env(HOME) legocad]}]

#  Tre modalita':
#    -reg    mostra il riquadro delle task di REGOLAZIONE (r_*): acceso di
#            default in modalita' doppia, serve a riaccenderlo con -proc/-staz
#    -noreg  nasconde il riquadro delle task di regolazione
#    -proc   solo le pagine di processo (task -> HMI draw2gr)
#    -staz   solo i faceplate (pagine di r02.dat -> xstaz)
#    nessuna delle due: entrambe, in due liste affiancate
set stazmode [expr {[lsearch -exact $argv "-staz"] >= 0}]
set procmode [expr {[lsearch -exact $argv "-proc"] >= 0}]

# -insim: il selettore e' stato lanciato DA DENTRO una simulazione in corso -
# lo fa il banco (new_monit, attiva_lghmi in cont_rec.c) dal suo menu, e il
# banco gira nella dir del simulatore. In quel caso il selettore appartiene a
# quella simulazione e due comandi non hanno senso, o sono dannosi:
#   * Open Simulator path  porterebbe il selettore su un'ALTRA directory,
#                          scollegandolo dalla simulazione che l'ha aperto;
#   * net_startup          farebbe killsim, cioe' ammazzerebbe proprio la
#                          simulazione da cui e' stato lanciato (e il banco
#                          con lei).
# Entrambi vengono quindi disabilitati.
set insim [expr {[lsearch -exact $argv "-insim"] >= 0}]

# -noedit: le HMI lanciate da qui NON hanno il menu Edit (draw2gr -edit), per
# nessuna task. Senza, il menu c'e' dove ha senso: vedi hmi_con_edit.
set noeditmode [expr {[lsearch -exact $argv "-noedit"] >= 0}]

# Directory usate di recente, per riaprirle dal menu File senza passare dal
# dialogo di selezione. Stanno in un file nella home e non in
# $LG_ENTRY/legopc_prefs.tcl perche' la lista attraversa le installazioni: la
# radice utente cambia proprio quando si cambia directory.
#  Il menu ne mostra MAXRECENTI, e solo quelle dell'area di lavoro corrente
#  (File -> Work area); il file ne tiene di piu', di tutte le aree, cosi'
#  tornando a un'area si ritrovano le sue.
set RECENTIFILE [file join $env(HOME) .lghmi_recent]
set MAXRECENTI  3
set MAXRECENTIFILE 30
set RECENTI     {}

# L'ultimo simulatore usato in ciascuna area di lavoro, per ritrovarlo dopo
# File -> Work area. Una riga per area: <directory fisica dell'area>|<nome>.
# Nella home come ~/.legosim, e non dentro le aree, che si copiano e si
# impacchettano.
set AREEFILE [file join $env(HOME) .lghmi_areas]
set mostra_proc [expr {$procmode || !$stazmode}]
set mostra_staz [expr {$stazmode || !$procmode}]
set doppia      [expr {$mostra_proc && $mostra_staz}]

#  Le task di REGOLAZIONE (r_*) hanno un riquadro loro, sotto agli altri due.
#  Acceso di default: sono parte del simulatore come le altre, e finora erano
#  invisibili qui dentro perche' scan_tasks tiene solo le directory con un .tom
#  - e una regolazione il .tom non ce l'ha. Si spegne con -noreg, e -reg lo
#  riaccende anche nelle modalita' a lista singola (-proc / -staz), dove
#  altrimenti non comparirebbe.
set regmode   [expr {[lsearch -exact $argv "-reg"] >= 0}]
set noregmode [expr {[lsearch -exact $argv "-noreg"] >= 0}]
set mostra_reg [expr {!$noregmode && ($regmode || $doppia)}]
set LGTIX    [expr {[info exists env(LG_TIX)] ? $env(LG_TIX) : ""}]

# --- Modalita' S01: file "S01" nella cwd di lancio ----------------------
# La cwd e' la dir da cui l'helper ha fatto exec di wish (nessun cd), quindi
# tipicamente la dir del simulatore in esecuzione.
set S01FILE [file join [pwd] S01]
set s01mode [expr {[file exists $S01FILE] && ![file isdirectory $S01FILE]}]
set s01_name ""
set s01_desc ""

# Set Sim path (helper -loc): LG_SIM_PATH = dir del simulatore in esecuzione,
# ereditata dal processo draw2gr. In modalita' S01 e' la dir del file S01 (la
# cwd di lancio): li' gira il simulatore COMPOSTO (dati live, SHM, f22circ.dat,
# variabili.rtf). Le task vengono invece lanciate dalla loro dir MODELLO (solo
# per caricare lo schema .tom): animazione/Plot/Command devono comunque puntare
# alla dir del simulatore, non a quella modello. Qui la mostriamo soltanto.
set SIMPATH  [expr {[info exists env(LG_SIM_PATH)] && $env(LG_SIM_PATH) ne "" ? $env(LG_SIM_PATH) : ""}]

# ITEMS = lista parallela alla listbox: {label dir name} per ogni voce.
set ITEMS {}

# --- Task = sottodir di $root con almeno un *.tom (modalita' dir-scan) ---
#  Le task di REGOLAZIONE presenti sotto $root.
#
#  Criterio: il prefisso "r_", che e' la convenzione gia' usata dal resto
#  dell'ambiente - kCompile entra "in ogni r_* sotto legocad" per compilarle.
#  Non si guarda dentro le directory: una task di regolazione non ha .tom (ha
#  i .sed dell'editor), quindi scan_tasks la scarta gia', e cercare i .sed
#  costerebbe una glob per ogni directory a ogni Refresh.
#
#  In modalita' S01 questa funzione NON si usa: li' il tipo della task e'
#  scritto nel file (lettera R), che e' l'informazione autorevole.
proc scan_reg_tasks {root} {
    set out {}
    foreach d [lsort [glob -nocomplain -type d [file join $root r_*]]] {
        lappend out [file tail $d]
    }
    return $out
}

proc scan_tasks {root} {
    set out {}
    foreach d [lsort [glob -nocomplain -type d [file join $root *]]] {
        if {[llength [glob -nocomplain [file join $d *.tom]]] > 0} {
            lappend out [file tail $d]
        }
    }
    return $out
}

#  --- Modalita' -staz: pagine di faceplate --------------------------------

#  Directory in cui cercare r02.dat, in ordine di preferenza:
#   1. la cwd di lancio, se contiene gia' un r02.dat (caso tipico: lghmi -staz
#      lanciato dalla dir della regolazione);
#   2. in modalita' S01, tutte le task del simulatore, REGOLAZIONE COMPRESA;
#   3. altrimenti le sottodir di $TASKROOT.
proc dirs_con_r02 {} {
    global TASKROOT s01mode S01FILE
    set out {}
    if {[file exists [file join [pwd] r02.dat]]} { lappend out [pwd] }
    if {$s01mode} {
        foreach e [parse_s01 $S01FILE {P R}] {
            lassign $e name desc dir
            if {[file exists [file join $dir r02.dat]] && [lsearch -exact $out $dir] < 0} {
                lappend out $dir
            }
        }
    } else {
        foreach d [lsort [glob -nocomplain -type d [file join $TASKROOT *]]] {
            if {[file exists [file join $d r02.dat]] && [lsearch -exact $out $d] < 0} {
                lappend out $d
            }
        }
    }
    return $out
}

#  Apre la pagina selezionata. Avvio di xstaz e richiesta li fa staz_apri
#  (lgstaz.tcl), condivisa con i bottoni faceplate delle pagine draw2gr.
proc apri_faceplate {} {
    global ITEMS_STAZ LB_STAZ
    set sel [$LB_STAZ curselection]
    if {[llength $sel] == 0} {
        .status configure -text "Select a faceplate page from the list."
        return
    }
    lassign [lindex $ITEMS_STAZ [lindex $sel 0]] label dir nome

    lassign [staz_apri $dir $nome] esito msg
    switch -- $esito {
        ok {
            #  msg non vuoto: la simulazione non gira, la pagina si apre ferma
            set t "Page '$nome' requested from xstaz  ($dir)"
            if {$msg ne ""} { append t "  -  $msg" }
            .status configure -text $t
        }
        altrove {
            tk_messageBox -icon warning -title "xstaz" -parent . -message $msg
        }
        default {
            tk_messageBox -icon error -title "Faceplate" -parent . -message $msg
        }
    }
}

#  Riempie la lista delle pagine di PROCESSO (task con .tom). Ritorna il testo
#  di stato da mostrare.
proc riempi_proc {} {
    global TASKROOT s01mode S01FILE ITEMS_PROC LB_PROC
    $LB_PROC delete 0 end
    set ITEMS_PROC {}

    if {$s01mode} {
        foreach e [parse_s01 $S01FILE] {
            lassign $e name desc dir
            set label [expr {$desc ne "" ? "$name  $desc" : $name}]
            $LB_PROC insert end $label
            lappend ITEMS_PROC [list $label $dir $name]
        }
        catch {.hdr.s01 configure -text "Simulator: $::s01_name  $::s01_desc"}
        if {[llength $ITEMS_PROC] == 0} {
            return "No process task (P) in the S01 file"
        }
        $LB_PROC selection clear 0 end
        $LB_PROC selection set 0
        return "[llength $ITEMS_PROC] process tasks (S01: $::s01_name)"
    }

    # fuori dalla modalita' S01 l'intestazione del simulatore non ha senso:
    # va svuotata, altrimenti resta quella della directory precedente
    catch {.hdr.s01 configure -text ""}

    if {![file isdirectory $TASKROOT]} {
        return "Task directory not found: $TASKROOT"
    }
    set tasks [scan_tasks $TASKROOT]
    foreach t $tasks {
        $LB_PROC insert end $t
        lappend ITEMS_PROC [list $t [file join $TASKROOT $t] $t]
    }
    if {[llength $tasks] == 0} {
        return "No task (*.tom) in $TASKROOT"
    }
    $LB_PROC selection clear 0 end
    $LB_PROC selection set 0
    return "[llength $tasks] tasks in $TASKROOT"
}

#  Riempie la lista dei FACEPLATE (pagine dei vari r02.dat).
proc riempi_staz {} {
    global ITEMS_STAZ LB_STAZ
    $LB_STAZ delete 0 end
    set ITEMS_STAZ {}

    set dirs [dirs_con_r02]
    if {[llength $dirs] == 0} {
        return "No r02.dat found (compile r01.dat with compstaz)"
    }
    set piu_dir [expr {[llength $dirs] > 1}]
    foreach d $dirs {
        foreach pg [pagine_di $d] {
            lassign $pg nome descr nstaz
            set label [format "%-10s %-42s %3s stations" $nome $descr $nstaz]
            if {$piu_dir} { append label "   \[[file tail $d]\]" }
            $LB_STAZ insert end $label
            lappend ITEMS_STAZ [list $label $d $nome]
        }
    }
    if {[llength $ITEMS_STAZ] == 0} {
        return "r02.dat found but with no readable pages"
    }
    $LB_STAZ selection clear 0 end
    $LB_STAZ selection set 0
    return "[llength $ITEMS_STAZ] faceplate pages in [llength $dirs] directories"
}

#  Riempie la lista delle task di REGOLAZIONE.
#
#  In modalita' S01 le prende dal file per TIPO (lettera R): e' il dato
#  autorevole, e parse_s01 sa gia' leggerlo perche' lo fa per i faceplate.
#  Fuori da S01 ricade sul prefisso r_ sotto $TASKROOT.
proc riempi_reg {} {
    global TASKROOT s01mode S01FILE ITEMS_REG LB_REG
    $LB_REG delete 0 end
    set ITEMS_REG {}

    if {$s01mode} {
        foreach e [parse_s01 $S01FILE R] {
            lassign $e name desc dir
            set label [expr {$desc ne "" ? "$name  $desc" : $name}]
            $LB_REG insert end $label
            lappend ITEMS_REG [list $label $dir $name]
        }
        if {[llength $ITEMS_REG] == 0} {
            return "no regulation task (R) in the S01"
        }
        $LB_REG selection clear 0 end
        return "[llength $ITEMS_REG] regulation tasks (S01)"
    }

    if {![file isdirectory $TASKROOT]} { return "" }
    set tasks [scan_reg_tasks $TASKROOT]
    foreach t $tasks {
        $LB_REG insert end $t
        lappend ITEMS_REG [list $t [file join $TASKROOT $t] $t]
    }
    if {[llength $tasks] == 0} { return "no r_* task in $TASKROOT" }
    $LB_REG selection clear 0 end
    return "[llength $tasks] regulation tasks"
}

proc refresh_list {} {
    global mostra_proc mostra_staz mostra_reg doppia
    #  L'area prima di tutto: i link possono essere cambiati da un lgswitch in
    #  un terminale, e allora anche i recenti da mostrare sono altri.
    set prima $::AREE_CORRENTI
    aggiorna_area
    if {$::AREE_CORRENTI ne $prima} { catch {aggiorna_menu_file} }
    set msg {}
    if {$mostra_proc} { lappend msg [riempi_proc] }
    if {$mostra_staz} { lappend msg [riempi_staz] }
    if {$mostra_reg}  { set nr [riempi_reg] ; if {$nr ne ""} { lappend msg $nr } }
    set nota [aggiorna_stato_mmi]
    if {$nota ne ""} { lappend msg $nota }
    set nota [aggiorna_stato_startup]
    if {$nota ne ""} { lappend msg $nota }
    aggiorna_etichette_loc
    # stato delle voci di Tools e nome del simulatore nella voce di kUpSim
    catch {aggiorna_menu_tools}
    #  Perche' le voci di Tools sono come sono: senza questa riga un menu tutto
    #  spento non dice niente, e sembra un guasto.
    if {[simulatore_corrente] eq ""} {
        lappend msg "no current simulator: pick one from File -> Current simulator"
    } elseif {[info exists ::RIPIEGO] && $::RIPIEGO ne ""} {
        lappend msg "simulator '$::RIPIEGO' (KSIM was not in the environment)"
        set ::RIPIEGO ""
    }
    .status configure -text [join $msg "   |   "]
    if {$doppia || $mostra_reg} { aggiorna_intestazioni }
}

#  In modalita' doppia il conteggio va anche sulle intestazioni dei due riquadri.
#  L'intestazione di un riquadro, ricavata dalla sua listbox: <riquadro>.f.lb
#  -> <riquadro>.h. Non si cablano i path (.pw.staz.h e simili) perche' cambiano
#  con la disposizione: i faceplate sono passati da .pw.staz a .pv.staz quando
#  le regolazioni hanno preso il posto accanto al processo.
proc intestazione_di {lb} {
    return [winfo parent [winfo parent $lb]].h
}

proc aggiorna_intestazioni {} {
    global ITEMS_PROC ITEMS_STAZ LB_PROC LB_STAZ
    catch {[intestazione_di $LB_PROC] configure \
               -text "Process pages ([llength $ITEMS_PROC])"}
    catch {[intestazione_di $LB_STAZ] configure \
               -text "xstaz faceplates ([llength $ITEMS_STAZ])"}
    catch {[intestazione_di $::LB_REG] configure \
               -text "Regulation tasks ([llength $::ITEMS_REG])"}
}

# --- Lancio della HMI in un processo indipendente ------------------------

#  La HMI di questa task avra' il menu Edit (draw2gr -edit)? draw2gr lo tiene
#  spento di default; qui lo si accende solo dove legopc potrebbe davvero
#  aprire la task:
#    - non con -noedit, che lo spegne per tutte;
#    - non con -insim: il selettore e' del banco, e la simulazione gira;
#    - non se in questa installazione legopc non c'e';
#    - non per le task di un'altra area: modifica_task le rifiuterebbe sempre,
#      e un comando che dice sempre di no e' meglio non offrirlo.
#  Le task dei bundle FMU non passano di qui (le lancia il run_draw2gr.sh del
#  bundle). La simulazione in corso invece NON spegne il menu: puo' fermarsi
#  mentre la HMI e' aperta, e draw2gr rifa' il controllo al momento del clic.
proc hmi_con_edit {dir} {
    global env LGTIX insim noeditmode
    if {$insim || $noeditmode} { return 0 }
    if {$LGTIX eq "" || ![file exists [file join $LGTIX legopc.tix]]} { return 0 }
    if {![info exists env(LG_ENTRY)] || $env(LG_ENTRY) eq ""} { return 0 }
    return [stessa_directory [area_della_task $dir] $env(LG_ENTRY)]
}

proc launch_hmi {} {
    global LGTIX ITEMS_PROC LB_PROC
    set sel [$LB_PROC curselection]
    if {[llength $sel] == 0} {
        .status configure -text "Select a process task from the list."
        return
    }
    lassign [lindex $ITEMS_PROC [lindex $sel 0]] label dir name
    if {![file isdirectory $dir]} {
        tk_messageBox -icon error -title "Task" -parent . -message \
            "Task directory not found:\n$dir"
        return
    }
    # Task dentro un bundle FMU? (<bundle>/task/<nome> -> <bundle>/run_draw2gr.sh)
    # Allora la HMI va lanciata col run_draw2gr.sh di QUEL bundle, non col
    # draw2gr.tcl dell'installazione LegoPST: solo lui conosce l'ambiente del
    # proprio bundle (wish, LG_TIX, runtime Tcl/Tk/Tix, LG_MODELS) e si ricava da
    # solo SHR_USR_KEY/LG_SIM_PATH dal net_sked di QUESTA task. Serve in
    # co-simulazione (lg_cosim), dove ogni FMU e' un simulatore a se' con la
    # propria chiave. Sulla macchina target del bundle, LegoPST non c'e' affatto.
    set launcher [file normalize [file join $dir .. .. run_draw2gr.sh]]
    set d2g [file join $LGTIX draw2gr.tcl]
    if {![file exists $launcher] && ($LGTIX eq "" || ![file exists $d2g])} {
        tk_messageBox -icon error -title "LG_TIX" -parent . -message \
            "draw2gr.tcl not found (LG_TIX='$LGTIX').\nStart lghmi from a LegoPST environment (profile sourced)."
        return
    }
    # LG_SIM_PATH (Set Sim path) e' ereditato invariato dall'helper: la dir del
    # simulatore in esecuzione (in S01 = dir del file S01). NON va reimpostato
    # alla dir modello della task, altrimenti animazione/Plot/Command leggono
    # una dir senza dati live/SHM del simulatore attivo.
    # Processo INDIPENDENTE: cd nella task ed exec della HMI. `setsid` la mette
    # in una nuova sessione -> sopravvive al Quit del selettore. L'output va in
    # un log in /tmp (per non sporcare la task) utile per debug.
    set log [file join /tmp "lghmi_${name}.log"]
    if {[file exists $launcher]} {
        # Bundle: `env -u` toglie LG_SIM_PATH/SHR_USR_KEY ereditate dal selettore.
        # Sono quelle del simulatore da cui e' partito lghmi (per S01/-loc) e qui
        # sarebbero SBAGLIATE: in co-simulazione ogni task ha la sua sim e la sua
        # chiave. Tolte, run_draw2gr.sh le ricava dal net_sked di questa task.
        # Mai -edit qui: sulla macchina target del bundle legopc non c'e'.
        set sh "exec env -u LG_SIM_PATH -u SHR_USR_KEY bash [list $launcher] [list $dir] >[list $log] 2>&1"
    } else {
        set opz [expr {[hmi_con_edit $dir] ? " -edit" : ""}]
        set sh "cd [list $dir] && exec wish [list $d2g] 1 f22circ$opz >[list $log] 2>&1"
    }
    if {[catch {exec setsid sh -c $sh &} err]} {
        # fallback senza setsid: resta comunque orfano (sopravvive) alla chiusura
        if {[catch {exec sh -c $sh &} err2]} {
            tk_messageBox -icon error -title "HMI launch" -parent . \
                -message "Cannot launch the HMI for '$name':\n$err2"
            return
        }
    }
    .status configure -text "HMI started for '$name'  (log: $log)"
}

#  Quante istanze di mmi sono vive adesso. Serve per capire se il lancio e'
#  riuscito: mmi parte in background, quindi l'esito non torna da exec.
proc conta_mmi {} {
    if {[catch {exec pgrep -x mmi} out]} { return 0 }
    return [llength [split [string trim $out] "\n"]]
}

#  Lancia l'applicazione MMI (LegoMMI, Alg_mmi/run_time).
#  La directory di lavoro e' tutto: mmi legge ./Context.ctx e da li' ricava dove
#  stanno le pagine, senza cercare altrove (vedi Alg_mmi/README.md). Ordine:
#     1) $LG_SIM_PATH/globpages  il Set Sim path (-loc) vince: e' il simulatore
#                                che l'utente ha indicato a QUESTO selettore, e
#                                puo' non essere quello scelto con ksetsim
#     2) $KPAGES                 (di norma $KSIM/globpages, dal profilo)
#     3) ./globpages             (simulatore sotto la cwd, profilo non sorgiato)
#     4) nessuna                 (si lancia dalla cwd, sara' mmi a lamentarsi
#                                 del Context mancante)
proc dir_mmi {} {
    global env SIMPATH
    if {$SIMPATH ne "" && [file isdirectory [file join $SIMPATH globpages]]} {
        return [list [file join $SIMPATH globpages] "Set Sim path"]
    }
    if {[info exists env(KPAGES)] && $env(KPAGES) ne "" \
        && [file isdirectory $env(KPAGES)]} {
        return [list $env(KPAGES) "KPAGES"]
    }
    if {[file isdirectory globpages]} {
        return [list [file normalize globpages] "./globpages"]
    }
    return [list "" ""]
}

#  Quante pagine mmi potrebbe davvero aprire partendo da $dir, applicando le sue
#  stesse regole: il Context.ctx della dir di lancio dichiara in *pages DOVE
#  stanno le pagine compilate e in *page_list QUALI sono; mmi apre poi
#  <pages>/<NOME>.rtf (vedi Alg_mmi/README.md). Non basta contare i *.rtf della
#  directory: la dir di un simulatore ne contiene altri che pagine non sono
#  (variabili.rtf, recorder.rtf...), e il Context puo' puntare le pagine altrove.
#  Ritorna -1 se manca il Context (mmi uscirebbe subito), altrimenti il numero di
#  pagine elencate che hanno il .rtf al suo posto.
proc pagine_mmi {dir} {
    set ctx [file join $dir Context.ctx]
    if {![file exists $ctx]} { return -1 }
    if {[catch {open $ctx r} fp]} { return -1 }
    set testo [read $fp] ; close $fp
    set pagdir $dir
    set elenco {}
    foreach riga [split $testo "\n"] {
        if {[regexp {^\*pages:[ \t]*(.*)$} $riga -> v]} {
            set v [string trim $v]
            if {$v ne ""} {
                set pagdir [expr {[string index $v 0] eq "/" ? $v : [file join $dir $v]}]
            }
        } elseif {[regexp {^\*page_list:[ \t]*(.*)$} $riga -> v]} {
            # Il valore inizia con "\ " (continuazione delle risorse X): via il
            # backslash, poi i nomi separati da spazi.
            set elenco [regexp -all -inline {\S+} [string map {"\\" " "} $v]]
        }
    }
    set n 0
    foreach nome $elenco {
        if {[file exists [file join $pagdir $nome.rtf]]} { incr n }
    }
    return $n
}

#  Abilita/disabilita il pulsante mmi: senza pagine apribili non ha senso.
#  Ritorna il motivo (stringa vuota se tutto a posto) da mostrare nella riga di
#  stato insieme ai conteggi delle liste.
proc aggiorna_stato_mmi {} {
    lassign [dir_mmi] dir via
    set d [expr {$dir eq "" ? [pwd] : $dir}]
    set n [pagine_mmi $d]
    if {$n > 0} {
        .btn.mmi configure -state normal -background "#50a050"
        return ""
    }
    .btn.mmi configure -state disabled -background "#9ab89a"
    if {$n < 0} { return "mmi: no Context.ctx in $d" }
    return "mmi: no compiled page (.rtf) in $d"
}

proc launch_mmi {} {
    lassign [dir_mmi] dir via
    # Senza profilo LegoPST sorgiato l'eseguibile non e' raggiungibile.
    if {[auto_execok mmi] eq ""} {
        tk_messageBox -icon error -title "mmi" -parent . -message \
            "Executable 'mmi' not found in PATH.\nStart lghmi from a LegoPST environment (profile sourced)."
        .status configure -text "mmi not found in PATH."
        return
    }
    set log [file join /tmp "lghmi_mmi.log"]
    if {$dir ne ""} {
        set sh "cd [list $dir] && exec mmi >[list $log] 2>&1"
    } else {
        set sh "exec mmi >[list $log] 2>&1"
    }
    set prima [conta_mmi]
    # Processo INDIPENDENTE come per la HMI: setsid lo mette in una nuova
    # sessione, cosi' sopravvive al Quit del selettore.
    if {[catch {exec setsid sh -c $sh &} err]} {
        if {[catch {exec sh -c $sh &} err2]} {
            tk_messageBox -icon error -title "mmi launch" -parent . \
                -message "Cannot launch mmi:\n$err2"
            .status configure -text "mmi NOT started."
            return
        }
    }
    .status configure -text [expr {$dir eq "" ? "mmi starting from the cwd..." \
                                              : "mmi starting from $dir ($via)..."}]
    after 3000 [list verifica_mmi $prima $dir $log]
}

#  Controllo differito dell'esito: se non e' comparsa una nuova istanza, mmi e'
#  morto subito e il motivo sta nelle ultime righe del log.
proc verifica_mmi {prima dir log} {
    if {[conta_mmi] > $prima} {
        .status configure -text [expr {$dir eq "" ? "mmi started (log: $log)" \
                                                  : "mmi started from $dir (log: $log)"}]
        return
    }
    set coda ""
    if {[file exists $log]} {
        catch {
            set fp [open $log r] ; set testo [read $fp] ; close $fp
            set righe [split [string trimright $testo "\n"] "\n"]
            if {[llength $righe] > 12} { set righe [lrange $righe end-11 end] }
            set coda [join $righe "\n"]
        }
    }
    tk_messageBox -icon error -title "mmi launch" -parent . -message \
        "mmi did not start.\n\nDirectory: [expr {$dir eq "" ? "(cwd)" : $dir}]\nLog: $log\n\n$coda"
    .status configure -text "mmi NOT started - see $log"
}

#  Popup minimo del tasto destro: una finestrella senza decorazioni con il solo
#  pulsante "Open page". Il tasto destro prima SELEZIONA la voce sotto il
#  cursore, cosi' il popup agisce su quella puntata e non sulla selezione
#  precedente; poi la apre con la stessa azione del doppio click.
proc chiudi_popup {} {
    if {[winfo exists .popup_open]} {
        catch {grab release .popup_open}
        destroy .popup_open
    }
}

#  Un click fuori dal pulsante chiude il popup. Il grab e' LOCALE: i figli di
#  .popup_open ricevono i loro eventi normalmente (il pulsante funziona), tutto
#  il resto arriva qui.
proc popup_fuori {X Y} {
    if {[winfo containing $X $Y] ne ".popup_open.b"} { chiudi_popup }
}

proc esegui_popup {azione} {
    chiudi_popup
    uplevel #0 $azione
}

#  $etichetta e' il testo del pulsante: sulle due liste di pagine e' "Open
#  page", ma sul riquadro delle regolazioni quell'azione apre config, non una
#  pagina, e dirlo "Open page" sarebbe falso.
proc popup_open_page {lb azione y X Y {etichetta "Open page"}} {
    chiudi_popup
    if {[$lb size] == 0} { return }
    set i [$lb nearest $y]
    if {$i < 0} { return }
    $lb selection clear 0 end
    $lb selection set $i
    $lb activate $i
    toplevel .popup_open -bd 1 -relief solid
    wm overrideredirect .popup_open 1
    wm geometry .popup_open +[expr {$X + 2}]+[expr {$Y + 2}]
    button .popup_open.b -text $etichetta -padx 6 -pady 2 \
                         -command [list esegui_popup $azione]
    pack .popup_open.b
    bind .popup_open <Escape>      { chiudi_popup }
    bind .popup_open <ButtonPress> { popup_fuori %X %Y }
    update idletasks
    raise .popup_open
    focus .popup_open
    grab set .popup_open
}

# --- Directory corrente: "File -> Open Simulator path" -------------------
#
# Cambiare directory a runtime equivale a rilanciare lghmi da quella dir: da
# essa dipendono la modalita' (l'S01 si cerca nella cwd), la lista dei
# faceplate (dirs_con_r02 guarda [pwd]), la dir di lavoro dell'MMI e il Set Sim
# path che le HMI ereditano.
#
# Nota: NON coincide con l'opzione "-loc DIR" dell'helper, che imposta soltanto
# LG_SIM_PATH e lascia la cwd dov'era. Qui si fa entrambe le cose, cioe' quello
# che si otterrebbe lanciando lghmi da quella directory.

#  Mostra o nasconde le due intestazioni che dipendono dalla directory: il
#  simulatore S01 e il Set Sim path. Il testo dell'S01 lo scrive riempi_proc,
#  che ha i valori del parsing; qui si decide solo cosa si vede.
proc aggiorna_etichette_loc {} {
    global SIMPATH
    if {[.hdr.s01 cget -text] ne ""} {
        pack .hdr.s01 -side top -fill x
    } else {
        pack forget .hdr.s01
    }
    if {$SIMPATH ne ""} {
        .hdr.loc configure -text "Set Sim path: $SIMPATH"
        pack .hdr.loc -side top -fill x
    } else {
        .hdr.loc configure -text ""
        pack forget .hdr.loc
    }
}

#  Porta il selettore nella directory <dir>: cwd, Set Sim path (anche
#  nell'ambiente, perche' le HMI lanciate lo ereditano), rilevamento dell'S01 e
#  aggiornamento delle liste. Ritorna un messaggio d'errore, o "" se e' andata.
proc imposta_loc {dir} {
    global S01FILE s01mode SIMPATH s01_name s01_desc
    if {![file isdirectory $dir]} { return "Directory not found: $dir" }
    if {[catch {cd $dir} err]}    { return "Cannot enter $dir: $err" }

    set SIMPATH [pwd]
    set ::env(LG_SIM_PATH) $SIMPATH
    set S01FILE [file join $SIMPATH S01]
    set s01mode [expr {[file exists $S01FILE] && ![file isdirectory $S01FILE]}]
    if {!$s01mode} { set s01_name ""; set s01_desc "" }
    refresh_list
    return ""
}

#  Legge le directory recenti, scartando quelle che non esistono piu' (una sim
#  cancellata, un disco smontato): restano nel file ma non nel menu.
proc carica_recenti {} {
    global RECENTI RECENTIFILE MAXRECENTIFILE
    set RECENTI {}
    if {[catch {open $RECENTIFILE r} fd]} return
    while {[gets $fd riga] >= 0} {
        set riga [string trim $riga]
        if {$riga eq "" || ![file isdirectory $riga]} continue
        if {[lsearch -exact $RECENTI $riga] < 0} { lappend RECENTI $riga }
        if {[llength $RECENTI] >= $MAXRECENTIFILE} break
    }
    close $fd
}

proc salva_recenti {} {
    global RECENTI RECENTIFILE
    # Se la home non e' scrivibile si perde solo la memoria dei recenti: non e'
    # un motivo per fermare il selettore.
    catch {
        set fd [open $RECENTIFILE w]
        foreach d $RECENTI { puts $fd $d }
        close $fd
    }
}

#  Mette <dir> in testa ai recenti (senza doppioni), tronca e salva.
proc ricorda_recente {dir} {
    global RECENTI MAXRECENTIFILE
    set dir [file normalize $dir]
    set pos [lsearch -exact $RECENTI $dir]
    if {$pos >= 0} { set RECENTI [lreplace $RECENTI $pos $pos] }
    set RECENTI [linsert $RECENTI 0 $dir]
    if {[llength $RECENTI] > $MAXRECENTIFILE} {
        set RECENTI [lrange $RECENTI 0 [expr {$MAXRECENTIFILE-1}]]
    }
    salva_recenti
    aggiorna_menu_file
}

#  File -> Current simulator: i simulatori di $KSKED (lista_simulatori), letti
#  all'apertura del sottomenu. Il radiobutton acceso e' quello corrente
#  (::KSIMSCELTO); sceglierne uno lo imposta (scegli_simulatore).
proc riempi_menu_simulatori {} {
    set m .mb.file.sim
    $m delete 0 end
    set sims [lista_simulatori]
    if {[llength $sims] == 0} {
        $m add command -state disabled -label "(no simulator in \$KSKED)"
        return
    }
    foreach sim $sims {
        $m add radiobutton -label $sim -value $sim \
            -variable ::KSIMSCELTO -command [list scegli_simulatore $sim]
    }
}

#  Ricostruisce il menu File PER INTERO a ogni cambiamento dei recenti.
#  Non si toccano le singole voci: gli indici cambierebbero a ogni path in piu'
#  o in meno, ed e' proprio il tipo di indirizzamento da evitare in un menu Tk.
proc aggiorna_menu_file {} {
    global RECENTI MAXRECENTI insim env
    if {![winfo exists .mb.file]} return
    set stato [expr {$insim ? "disabled" : "normal"}]

    .mb.file delete 0 end
    #  Le aree si elencano all'apertura del sottomenu (riempi_menu_aree), non
    #  qui: possono cambiare anche fuori da lghmi.
    if {![winfo exists .mb.file.aree]} {
        menu .mb.file.aree -tearoff 0 -postcommand riempi_menu_aree
    }
    .mb.file add cascade -label "Work area" -menu .mb.file.aree -state $stato
    #  Il simulatore corrente: prima l'area, poi il simulatore di quell'area.
    #  Anche questo si elenca all'apertura (riempi_menu_simulatori), cosi' un
    #  simulatore creato fuori da lghmi compare senza Refresh. Scegliere un
    #  simulatore porta lghmi nella sua directory: con -insim la directory e'
    #  quella della simulazione in corso, fissa, e la voce e' spenta come Work
    #  area e Open Simulator path.
    if {![winfo exists .mb.file.sim]} {
        menu .mb.file.sim -tearoff 0 -postcommand riempi_menu_simulatori
    }
    .mb.file add cascade -label "Current simulator" -menu .mb.file.sim -state $stato
    .mb.file add command -label "Open Simulator path..." -command apri_loc_path -state $stato
    set visibili {}
    foreach d $RECENTI {
        if {[llength $visibili] >= $MAXRECENTI} break
        if {[recente_visibile $d]} { lappend visibili $d }
    }
    if {[llength $visibili] > 0} {
        .mb.file add separator
        foreach d $visibili {
            # ~ al posto della home: i path delle simulazioni sono lunghi e la
            # parte utile e' la coda
            set etichetta $d
            if {[string first $env(HOME)/ $d] == 0} {
                set etichetta "~[string range $d [string length $env(HOME)] end]"
            }
            .mb.file add command -label $etichetta -state $stato \
                                 -command [list vai_a_loc $d]
        }
    }
    .mb.file add separator
    #  Riapre la finestra di log di net_startup, che finora si poteva solo
    #  chiudere. Lo stato non si decide qui ma in aggiorna_voci_log, appesa al
    #  -postcommand del menu: vedi li' il perche'.
    .mb.file add command -label "Simulation log" -command riapri_log_sim
    #  Gli altri log che lghmi scrive in /tmp: uno per HMI lanciata, piu' mmi e
    #  xstaz. Stanno in un sottomenu e non qui perche' sono un numero variabile
    #  e di importanza minore; la simulazione resta la voce di primo livello,
    #  perche' la sua finestra non e' solo un visore - da li' si FERMA.
    if {![winfo exists .mb.file.logs]} { menu .mb.file.logs -tearoff 0 }
    .mb.file add cascade -label "Logs" -menu .mb.file.logs
    .mb.file add separator
    .mb.file add command -label "Refresh" -command refresh_list
    .mb.file add separator
    .mb.file add command -label "Quit" -command exit
    aggiorna_voci_log
}

#  Voci di log del menu File, ricalcolate ogni volta che il menu viene aperto
#  (-postcommand) e non alla sua costruzione: il menu File si rifa' di rado -
#  solo quando cambiano i path recenti - mentre i log compaiono e la simulazione
#  parte e si ferma in qualsiasi momento.
#
#  "Simulation log" e' acceso se c'e' un log da rileggere OPPURE una simulazione
#  viva: nel secondo caso la finestra serve anche senza log, perche' e' l'unico
#  posto da cui si puo' FERMARE la simulazione.
#
#  Con -insim resta spento, come il resto del menu: il selettore appartiene a una
#  simulazione che non ha lanciato lui, quindi il log in /tmp non e' il suo - e'
#  di un'altra sessione o e' vecchio - e il pulsante "Kill simulation" della
#  finestra ammazzerebbe proprio la simulazione da cui lghmi e' stato aperto.
#  Il sottomenu "Logs" invece resta acceso anche li': leggere il log di una HMI
#  non tocca niente, e quelle finestre non hanno nessun pulsante che ferma nulla.
#
#  Le voci si indirizzano PER ETICHETTA: gli indici cambiano con i recenti.
proc aggiorna_voci_log {} {
    global SIMLOG insim
    if {![winfo exists .mb.file]} return
    set acceso [expr {!$insim && ([file exists $SIMLOG] || [llength [sim_attiva]] > 0)}]
    catch {.mb.file entryconfigure "Simulation log" \
               -state [expr {$acceso ? "normal" : "disabled"}]}

    if {![winfo exists .mb.file.logs]} return
    .mb.file.logs delete 0 end
    set adesso [clock seconds]
    set n 0
    foreach f [elenco_log] {
        #  Dimensione ed eta' distinguono a colpo d'occhio il log di adesso da
        #  quello di ieri, e un log vuoto da uno che ha qualcosa da dire.
        set nota [format "%s, %s" [dimensione_leggibile [file size $f]] \
                                  [eta_leggibile [expr {$adesso - [file mtime $f]}]]]
        .mb.file.logs add command -label "[etichetta_log $f]   ($nota)" \
                                  -command [list apri_log $f]
        incr n
    }
    if {$n == 0} {
        .mb.file.logs add command -label "(no logs in /tmp)" -state disabled
    }
    catch {.mb.file entryconfigure "Logs" \
               -state [expr {$n > 0 ? "normal" : "disabled"}]}
}

#  Va nella directory <dir> e la promuove in testa ai recenti. E' la strada sia
#  del dialogo di selezione sia delle voci di menu dei path recenti.
proc vai_a_loc {dir} {
    global insim
    if {$insim} {
        .status configure -text \
            "Fixed directory: this launcher was started from the desk of the running simulation."
        return
    }
    set err [imposta_loc $dir]
    if {$err ne ""} {
        tk_messageBox -icon error -title "Open Simulator path" -parent . -message $err
        return
    }
    ricorda_recente [pwd]
    #  una directory di simulatore dell'area ne fa il simulatore corrente,
    #  come se lo si fosse scelto da File -> Current simulator
    set s [allinea_simulatore 1]
    # refresh_list ha gia' scritto i conteggi: la directory si aggiunge davanti,
    # non li sostituisce
    set testa "Directory: [pwd]"
    if {$s ne ""} { append testa "   |   current simulator: $s" }
    .status configure -text "$testa   |   [.status cget -text]"
}

#  Voce di menu: scegli la directory e vacci.
proc apri_loc_path {} {
    global insim
    if {$insim} {
        # la voce di menu e' disabilitata, ma la proc resta raggiungibile
        .status configure -text \
            "Fixed directory: this launcher was started from the desk of the running simulation."
        return
    }
    set dir [tk_chooseDirectory -parent . -mustexist 1 -initialdir [pwd] \
                 -title "Simulator path"]
    if {$dir eq ""} return
    vai_a_loc $dir
}

# --- File -> Work area: cambiare area di lavoro (lgswitch) ----------------
#
# Un'area di lavoro e' una directory legopst_<nome> con dentro legocad e sked.
# Quella corrente la scelgono due link, <base>/legocad e <base>/sked, e tutto
# il profilo la raggiunge ATTRAVERSO quei link: LG_ENTRY=$HOME/legocad,
# LG_LIBGRAPH, LG_MODELS, KSKED=$HOME/sked, KSIM=$KSKED/<nome>, KPAGES, e il
# PATH con $HOME/legocad/libut_bin. Cambiare i link cambia quindi l'area anche
# ai processi GIA' APERTI, che pero' restano con la directory corrente nella
# vecchia: un legopc continuerebbe a editare un modello della vecchia area
# risolvendone i blocchi contro il libgraph della nuova, senza errori. Per
# questo lo switch si rifiuta finche' qualcosa lavora sull'area corrente.
#
# I link li cambia lgswitch, non questo file: qui si chiede a lgswitch --list
# cosa c'e' e si lancia lgswitch -f <area>. La regola su cosa e' un'area, le
# copie .prelink-* delle directory vere e il controllo sulle aree incomplete
# restano scritti in un posto solo.
#
# Dopo lo switch il selettore riparte in dir-scan, dalla directory dei link e
# senza Set Sim path: la directory da cui lavorava, il suo S01 e il suo sim
# path erano della vecchia area.

#  Stato dell'area, ricalcolato da aggiorna_area:
#    AREA_CORRENTE  nome dell'area (il radiobutton del menu), "" se non ce
#                   n'e' una sola
#    AREE_BASE      directory FISICA dei link, "" se lo switch non e' gestito
#    AREE_CORRENTI  directory FISICHE a cui puntano legocad e sked
#    AREA_SKED      l'area di sked: e' li' che stanno i simulatori
set AREA_CORRENTE ""
set AREE_BASE     ""
set AREE_CORRENTI {}
set AREA_SKED     ""

#  Il path FISICO di <path>, con tutti i symlink risolti. `file normalize` li
#  risolve in tutti i componenti tranne l'ultimo: aggiungendone uno fittizio,
#  anche l'ultimo diventa intermedio.
proc fisico {path} {
    return [file dirname [file normalize [file join $path _]]]
}

#  ~ al posto della home, per i path mostrati.
proc con_tilde {path} {
    global env
    if {$path eq $env(HOME)} { return "~" }
    if {[string first $env(HOME)/ $path] == 0} {
        return "~[string range $path [string length $env(HOME)] end]"
    }
    return $path
}

#  La directory che contiene i link legocad e sked, cioe' quella in cui va
#  eseguito lgswitch. Ritorna {base ""}, oppure {"" motivo} se lo switch da qui
#  non e' possibile: i due link devono essere quelli che il profilo usa
#  (LG_ENTRY e KSKED) e stare nella STESSA directory, perche' lgswitch li crea
#  entrambi nella directory corrente.
proc base_aree {} {
    global env
    set entry [expr {[info exists env(LG_ENTRY)] ? $env(LG_ENTRY) : ""}]
    set sked  [expr {[info exists env(KSKED)] ? $env(KSKED) : ""}]
    if {$entry eq "" || $sked eq ""} {
        return [list "" "LG_ENTRY or KSKED is not set"]
    }
    if {[file tail $entry] ne "legocad" || [file tail $sked] ne "sked"} {
        return [list "" "LG_ENTRY and KSKED do not end in legocad and sked"]
    }
    set base [file dirname $entry]
    if {![stessa_directory $base [file dirname $sked]]} {
        return [list "" "legocad and sked are not in the same directory"]
    }
    if {[comando_lgswitch] eq ""} {
        return [list "" "lgswitch not found"]
    }
    return [list $base ""]
}

#  Lo script lgswitch dell'installazione corrente (util97/bin), o "" se non
#  c'e'. Il PATH non basta: l'helper sorgia il profilo solo se manca LG_TIX,
#  quindi util97/bin puo' non esserci.
proc comando_lgswitch {} {
    global env
    set cand {}
    if {[info exists env(UTIL97)]}   { lappend cand [file join $env(UTIL97) bin lgswitch] }
    if {[info exists env(LEGOROOT)]} { lappend cand [file join $env(LEGOROOT) util97 bin lgswitch] }
    foreach c $cand {
        if {[file executable $c]} { return $c }
    }
    set c [auto_execok lgswitch]
    return [expr {$c ne "" ? [lindex $c 0] : ""}]
}

#  Il censimento di `lgswitch --list`, eseguito in <base>. Ritorna un dict:
#    link    {legocad {stato destinazione} sked {stato destinazione}}
#    aree    lista di {nome ha_legocad ha_sked}
#    errore  "" oppure il motivo per cui non si e' potuto leggere
proc elenco_aree {base} {
    set ris [dict create link {} aree {} errore ""]
    set old [pwd]
    if {[catch {cd $base} err]} {
        dict set ris errore $err
        return $ris
    }
    set rc [catch {exec [comando_lgswitch] --list 2>@1} out]
    cd $old
    if {$rc} {
        dict set ris errore "lgswitch --list failed: [lindex [split $out \n] 0]"
        return $ris
    }
    foreach riga [split $out "\n"] {
        set c [split $riga "|"]
        switch -exact -- [lindex $c 0] {
            link { dict set ris link [lindex $c 1] [list [lindex $c 2] [lindex $c 3]] }
            area { dict lappend ris aree [lrange $c 1 3] }
        }
    }
    return $ris
}

#  Com'e' messa l'area di lavoro, per il titolo, l'intestazione e i controlli.
#  Ritorna un dict:
#    base    directory dei link, "" se lo switch non e' gestito (vedi motivo)
#    motivo  perche' non e' gestito, o perche' non si e' potuto leggere
#    nome    area corrente, "" se non ce n'e' UNA (link misti, directory vere)
#    aree    directory FISICHE su cui si lavora adesso: le destinazioni dei
#            link, o legocad/sked stesse se sono directory vere
#    sked    area FISICA di sked, "" se sked non e' un link
#    testo   descrizione per l'intestazione
#    grave   1 se lo stato merita attenzione (tutto tranne il caso normale)
#    elenco  il dict di elenco_aree
proc stato_area {} {
    set st [dict create base "" motivo "" nome "" aree {} sked "" \
                        testo "" grave 0 elenco {}]
    lassign [base_aree] base motivo
    if {$base eq ""} {
        dict set st motivo $motivo
        dict set st testo "Work area: not managed from here ($motivo)"
        return $st
    }
    dict set st base $base
    set el [elenco_aree $base]
    dict set st elenco $el
    if {[dict get $el errore] ne ""} {
        dict set st motivo [dict get $el errore]
        dict set st testo "Work area: unknown ([dict get $el errore])"
        dict set st grave 1
        return $st
    }
    set parti {}
    set aree {}
    set nlink 0
    foreach n {legocad sked} {
        set stato assente
        set dest ""
        if {[dict exists $el link $n]} { lassign [dict get $el link $n] stato dest }
        switch -exact -- $stato {
            link {
                incr nlink
                set a [file dirname [fisico [file join $base $dest]]]
                lappend aree $a
                lappend parti "$n -> [file tail $a]"
                if {$n eq "sked"} { dict set st sked $a }
            }
            dir {
                lappend aree [fisico [file join $base $n]]
                lappend parti "$n is a real directory"
            }
            altro   { lappend parti "$n is neither a link nor a directory" }
            default { lappend parti "$n is missing" }
        }
    }
    set aree [lsort -unique $aree]
    dict set st aree $aree
    if {$nlink == 2 && [llength $aree] == 1} {
        dict set st nome [file tail [lindex $aree 0]]
        dict set st testo "Work area: [file tail [lindex $aree 0]]"
    } elseif {$nlink == 2} {
        dict set st testo "Work area: MIXED - [join $parti {, }]"
        dict set st grave 1
    } else {
        dict set st testo "Work area: none - [join $parti {, }]"
        dict set st grave 1
    }
    return $st
}

#  Ricalcola l'area e la mostra nel titolo e nell'intestazione. Ritorna lo
#  stato, che il chiamante puo' riusare.
proc aggiorna_area {} {
    global insim
    set st [stato_area]
    set ::AREA_CORRENTE [dict get $st nome]
    set ::AREE_BASE     [expr {[dict get $st base] ne "" ? [fisico [dict get $st base]] : ""}]
    set ::AREE_CORRENTI [dict get $st aree]
    set ::AREA_SKED     [dict get $st sked]

    set t $::TITOLO_BASE
    if {[dict get $st nome] ne ""} {
        append t "  \[[dict get $st nome]\]"
    } elseif {[dict get $st base] ne ""} {
        append t "  \[no single work area\]"
    }
    if {$insim} { append t "  (from the desk)" }
    wm title . $t
    catch {.area configure -text [dict get $st testo] \
               -foreground [expr {[dict get $st grave] ? "red" : "black"}]}
    return $st
}

#  Un path recente va nel menu? No se sta in un'ALTRA area della stessa base
#  (<base>/legopst_*, diversa da quelle su cui si lavora); si' in tutti gli
#  altri casi, compresi i path che non stanno in nessuna area. Il file dei
#  recenti non si tocca: tornando a un'area si ritrovano i suoi.
proc recente_visibile {dir} {
    if {$::AREE_BASE eq ""} { return 1 }
    if {[string first "$::AREE_BASE/legopst_" $dir] != 0} { return 1 }
    foreach a $::AREE_CORRENTI {
        if {$dir eq $a || [string first "$a/" $dir] == 0} { return 1 }
    }
    return 0
}

#  Memoria dell'ultimo simulatore per area (AREEFILE): un dict
#  {area fisica -> nome del simulatore}.
proc leggi_aree_file {} {
    global AREEFILE
    set d [dict create]
    if {[catch {open $AREEFILE r} fd]} { return $d }
    while {[gets $fd riga] >= 0} {
        set i [string last "|" $riga]
        if {$i <= 0} continue
        dict set d [string range $riga 0 [expr {$i - 1}]] \
                   [string range $riga [expr {$i + 1}] end]
    }
    close $fd
    return $d
}

#  L'ultimo simulatore usato nell'area <area>, o "".
proc sim_ricordato {area} {
    set d [leggi_aree_file]
    if {$area eq "" || ![dict exists $d $area]} { return "" }
    return [dict get $d $area]
}

#  Ricorda <nome> come ultimo simulatore dell'area di sked corrente. Se la home
#  non e' scrivibile si perde solo la memoria, come per i recenti.
proc ricorda_sim_area {nome} {
    global AREEFILE
    if {$::AREA_SKED eq "" || $nome eq ""} return
    set d [leggi_aree_file]
    if {[dict exists $d $::AREA_SKED] && [dict get $d $::AREA_SKED] eq $nome} return
    dict set d $::AREA_SKED $nome
    catch {
        set fd [open $AREEFILE w]
        dict for {a s} $d { puts $fd "$a|$s" }
        close $fd
    }
}

#  Nome del processo (/proc/<pid>/comm).
proc nome_processo {pid} {
    if {[catch {open /proc/$pid/comm r} fd]} { return "?" }
    set n [string trim [read $fd]]
    close $fd
    return $n
}

#  Una shell che aspetta comandi: bash, sh, ksh... con soli argomenti che sono
#  opzioni, e senza -c. Con -c, o con uno script come argomento, sta eseguendo
#  qualcosa - i lanci di lghmi passano da sh -c e bash -c - e conta come
#  processo al lavoro. Uno script lanciato col suo #! ha il suo nome in comm, e
#  quindi non arriva nemmeno qui.
proc shell_interattiva {pid nome} {
    if {[lsearch -exact {bash sh dash ksh ksh93 mksh zsh tcsh csh fish} $nome] < 0} {
        return 0
    }
    if {[catch {open /proc/$pid/cmdline r} fd]} { return 0 }
    fconfigure $fd -translation binary
    set argomenti [split [read $fd] "\0"]
    close $fd
    foreach a [lrange $argomenti 1 end] {
        if {$a eq ""} continue
        if {$a eq "-c" || ![string match -* $a]} { return 0 }
    }
    return 1
}

#  I processi dell'utente con la directory corrente dentro <aree>, escluso
#  questo selettore. Ritorna {bloccanti shell}, due liste di {pid nome dove}:
#  le shell interattive sono solo un avviso, tutto il resto blocca. I processi
#  di altri utenti non si vedono (readlink di cwd e' negato) e non contano.
proc processi_nell_area {aree} {
    set io [pid]
    set bloccanti {}
    set shell {}
    foreach d [glob -nocomplain -types d -directory /proc {[0-9]*}] {
        set p [file tail $d]
        if {![string is integer -strict $p] || $p == $io} continue
        if {[catch {file readlink [file join $d cwd]} cwd]} continue
        set dove ""
        foreach a $aree {
            if {$cwd eq $a || [string first "$a/" $cwd] == 0} {
                set dove "[file tail $a][string range $cwd [string length $a] end]"
                break
            }
        }
        if {$dove eq ""} continue
        set nome [nome_processo $p]
        if {[shell_interattiva $p $nome]} {
            lappend shell [list $p $nome $dove]
        } else {
            lappend bloccanti [list $p $nome $dove]
        }
    }
    return [list $bloccanti $shell]
}

#  Cio' che impedisce lo switch anche lavorando FUORI dall'area: la
#  simulazione, i legopc (anche aperti vuoti leggono libgraph attraverso
#  $HOME/legocad) e gli altri lghmi (resterebbero con l'area vecchia in
#  memoria). Stesso formato di processi_nell_area; la simulazione non ha pid.
proc bloccanti_ovunque {} {
    set out {}
    foreach p [sim_attiva] { lappend out [list - $p "simulation running"] }
    foreach {script cosa} {legopc.tix "legopc (CAD)" lghmi.tcl "another lghmi"} {
        if {[catch {exec pgrep -f $script} pids]} continue
        foreach p [split [string trim $pids] "\n"] {
            set p [string trim $p]
            if {$p eq "" || $p == [pid] || ![esegue_script $p $script]} continue
            lappend out [list $p [nome_processo $p] $cosa]
        }
    }
    return $out
}

#  Le ultime <n> righe di un file, per i dialoghi d'errore.
proc coda_file {file n} {
    if {[catch {open $file r} fd]} { return "" }
    set righe [split [string trimright [read $fd]] "\n"]
    close $fd
    return [join [lrange $righe end-[expr {$n - 1}] end] "\n"]
}

#  Il sottomenu File -> Work area, ricostruito a ogni apertura: le aree si
#  possono creare, e i link cambiare, anche fuori da lghmi. Le aree incomplete
#  si vedono ma sono spente, con il motivo nell'etichetta.
proc riempi_menu_aree {} {
    set m .mb.file.aree
    $m delete 0 end
    set st [aggiorna_area]
    if {[dict get $st base] eq ""} {
        $m add command -state disabled -label "Not available: [dict get $st motivo]"
        return
    }
    set el [dict get $st elenco]
    if {[dict get $el errore] ne ""} {
        $m add command -state disabled -label "Not available: [dict get $el errore]"
        return
    }
    set aree [dict get $el aree]
    if {[llength $aree] == 0} {
        $m add command -state disabled \
            -label "(no legopst_* directory in [con_tilde [dict get $st base]])"
        return
    }
    foreach a $aree {
        lassign $a nome lc sk
        set manca {}
        if {!$lc} { lappend manca legocad }
        if {!$sk} { lappend manca sked }
        if {[llength $manca]} {
            $m add radiobutton -state disabled -variable ::AREA_CORRENTE -value $nome \
                -label "$nome   (no [join $manca { and }])"
        } else {
            $m add radiobutton -variable ::AREA_CORRENTE -value $nome \
                -label $nome -command [list cambia_area $nome]
        }
    }
    $m add separator
    set b [con_tilde [dict get $st base]]
    $m add command -state disabled -label "Links: $b/legocad, $b/sked"
}

#  Voce del sottomenu: passa all'area <nome>. Il radiobutton ha gia' spostato
#  la spunta: ogni rinuncia la rimette dov'era (aggiorna_area).
proc cambia_area {nome} {
    global insim
    set st [aggiorna_area]
    if {$insim} return
    set base [dict get $st base]
    if {$base eq ""} {
        tk_messageBox -icon error -title "Work area" -parent . -message \
            "The work area cannot be switched from here:\n[dict get $st motivo]"
        return
    }
    set el [dict get $st elenco]
    set voce [lsearch -inline -exact -index 0 [dict get $el aree] $nome]
    if {$voce eq ""} {
        tk_messageBox -icon error -title "Work area" -parent . -message \
            "'$nome' is no longer in [con_tilde $base]."
        return
    }
    lassign $voce - lc sk
    if {!$lc || !$sk} {
        tk_messageBox -icon error -title "Work area" -parent . -message \
            "'$nome' does not have both legocad and sked.\n\nThe missing link would keep pointing to the current area, and you would\nwork with legocad from one area and sked from another."
        return
    }
    set vecchia [dict get $st nome]
    if {$vecchia eq $nome} {
        .status configure -text "Work area: already on $nome."
        return
    }
    set da [expr {$vecchia ne "" ? $vecchia \
                  : [string map {"Work area: " ""} [dict get $st testo]]}]

    # 1. chi lavora ancora sull'area corrente
    lassign [processi_nell_area [dict get $st aree]] bloccanti shell
    set visti {}
    foreach b $bloccanti { lappend visti [lindex $b 0] }
    foreach b [bloccanti_ovunque] {
        if {[lsearch -exact $visti [lindex $b 0]] >= 0} continue
        lappend bloccanti $b
    }
    if {[llength $bloccanti] > 0} {
        set msg "Cannot switch the work area now: these are still working.\n\n"
        foreach b [lrange $bloccanti 0 14] {
            lassign $b p n dove
            append msg [format "    %-7s %-16s %s\n" $p $n $dove]
        }
        if {[llength $bloccanti] > 15} {
            append msg "    ... and [expr {[llength $bloccanti] - 15}] more\n"
        }
        append msg "\nAfter the switch they would reach $nome through ~/legocad\n"
        append msg "and ~/sked while still working on $da, with nothing saying so.\n\n"
        append msg "Close them first - the simulation with Simulator Shutdown in the\n"
        append msg "Master Menu of the desk - then retry."
        tk_messageBox -icon warning -title "Work area" -parent . -message $msg
        .status configure -text \
            "Work area: not switched - [llength $bloccanti] processes still at work."
        return
    }

    # 2. cosa c'e' al posto dei link
    set rinomina {}
    foreach n {legocad sked} {
        set stato assente
        if {[dict exists $el link $n]} { set stato [lindex [dict get $el link $n] 0] }
        if {$stato eq "altro"} {
            tk_messageBox -icon error -title "Work area" -parent . -message \
                "[con_tilde [file join $base $n]] exists and is neither a link nor a directory.\n\nIt is not touched: move it away by hand, then retry."
            return
        }
        if {$stato eq "dir"} { lappend rinomina $n }
    }

    # 3. conferma
    set b [con_tilde $base]
    set msg "Switch the work area?\n\n"
    append msg "    from:   $da\n"
    append msg "    to:     $nome\n\n"
    append msg "$b/legocad and $b/sked will point to $nome.\n"
    foreach n $rinomina {
        append msg "\n$b/$n is a real directory, not a link: it will be RENAMED to\n"
        append msg "$n.prelink-<date>-<time> in the same place. Nothing is deleted.\n"
    }
    if {[llength $shell] > 0} {
        append msg "\nShells working inside the current area:\n"
        foreach s [lrange $shell 0 9] {
            lassign $s p n dove
            append msg [format "    %-7s %-16s %s\n" $p $n $dove]
        }
        append msg "They are not stopped, but from now on their paths lead to $nome.\n"
    }
    append msg "\nShells already open keep the simulator they started with (KSIM),\n"
    append msg "which now names a directory of $nome: open new ones after the switch.\n"
    append msg "\nThis launcher will then list the tasks of $nome, with no Set Sim path."
    if {[tk_messageBox -icon question -type yesno -default no -parent . \
             -title "Work area" -message $msg] ne "yes"} {
        .status configure -text "Work area: not switched."
        return
    }

    # 4. il simulatore di adesso resta ricordato per l'area che si lascia
    ricorda_sim_area [simulatore_corrente]

    # 5. lgswitch, dalla directory dei link: e' li' che crea legocad e sked.
    #    Il nome del log segue lghmi_*.log, cosi' compare in File -> Logs.
    set log [file join /tmp lghmi_lgswitch.log]
    if {[catch {cd $base} err]} {
        tk_messageBox -icon error -title "Work area" -parent . -message \
            "Cannot enter $base:\n$err"
        return
    }
    set fallito [catch {exec [comando_lgswitch] -f $nome >$log 2>@1}]

    # 6. riallineamento, anche se lgswitch e' fallito: i link possono essere
    #    cambiati a meta', e il selettore deve mostrare com'e' adesso
    set sim [riallinea_dopo_switch]
    set dopo [.status cget -text]
    set simtesto [expr {$sim ne "" ? "simulator: $sim" : "no simulator in the new area"}]
    #  Conta il risultato, non solo il codice di uscita: e' quello che si
    #  mostra, ed e' quello su cui si lavorera'.
    if {$::AREA_CORRENTE ne $nome} { set fallito 1 }
    if {$fallito} {
        tk_messageBox -icon error -title "Work area" -parent . -message \
            "lgswitch did not complete the switch:\n\n[coda_file $log 12]\n\n[.area cget -text]\n\nFull output: $log (File -> Logs)."
        .status configure -text "Work area: lgswitch FAILED - see File -> Logs   |   $dopo"
        return
    }
    .status configure -text "Work area: $nome   |   $simtesto   |   $dopo"
}

#  Riporta il selettore in una situazione coerente con l'area appena scelta:
#  directory dei link, dir-scan, niente Set Sim path, e come simulatore quello
#  ricordato per la nuova area o, in mancanza, la cascata del profilo
#  (~/.legosim, cassano0, il primo disponibile). La scelta si scrive in
#  ~/.legosim: e' l'utente che ha cambiato area, e le shell future devono
#  trovare un simulatore che esiste. Ritorna il nome del simulatore, o "".
proc riallinea_dopo_switch {} {
    global env SIMPATH S01FILE s01mode s01_name s01_desc
    set st [aggiorna_area]
    if {[dict get $st base] ne ""} { catch {cd [dict get $st base]} }
    set SIMPATH ""
    unset -nocomplain env(LG_SIM_PATH)
    set S01FILE [file join [pwd] S01]
    set s01mode 0
    set s01_name ""
    set s01_desc ""

    set candidati {}
    set r [sim_ricordato $::AREA_SKED]
    if {$r ne ""} { lappend candidati $r }
    if {![catch {open [file join $env(HOME) .legosim] r} fd]} {
        set voluto [string trim [read $fd]]
        close $fd
        if {$voluto ne ""} { lappend candidati $voluto }
    }
    lappend candidati cassano0
    foreach s [lista_simulatori] { lappend candidati $s }
    set scelto ""
    if {[info exists env(KSKED)]} {
        foreach c $candidati {
            if {[file isdirectory [file join $env(KSKED) $c]]} { set scelto $c ; break }
        }
    }
    if {$scelto ne ""} {
        imposta_simulatore $scelto 1
    } else {
        unset -nocomplain env(KSIM) env(KSIMNAME) env(KPAGES)
        set ::KSIMSCELTO ""
        aggiorna_menu_tools
    }
    set ::RIPIEGO ""
    aggiorna_menu_file
    refresh_list
    return $scelto
}

# --- Lancio della simulazione (net_startup) ------------------------------
#
# sim_attiva (i processi di simulazione vivi) sta in lgedit.tcl: la usa anche
# il controllo che impedisce di editare una task mentre gira.

#  Il pulsante di lancio si abilita solo dove net_startup puo' funzionare.
#  Il file che lo script controlla e' variabili.rtf: senza quello si fermano
#  tutti i suoi passi successivi. Sta anche nelle dir delle task singole, che
#  sono lanciabili come i simulatori composti.
proc aggiorna_stato_startup {} {
    global insim
    if {$insim} {
        # lanciato dal banco: la simulazione gira gia', e net_startup la
        # fermerebbe con killsim
        .btn.start configure -state disabled
        return "started from the desk: fixed directory, simulation already running"
    }
    if {[file exists [file join [pwd] variabili.rtf]]} {
        .btn.start configure -state normal
        return "simulation can be started from here"
    }
    .btn.start configure -state disabled
    return ""
}


#  Dove finisce l'output di net_startup: un file, non un terminale. La finestra
#  di log lo segue da li' (mostra_log_sim) e resta leggibile anche dopo che la
#  finestra e' stata chiusa. In /tmp, come gli altri lanci di questo file, per
#  non sporcare la directory della simulazione.
set SIMLOG [file join /tmp "lghmi_net_startup.log"]

#  Directory dell'ultimo net_startup lanciato da qui. Serve solo alla finestra
#  di log, che la mette nel titolo: riaprendola va ridetta, e il file di log non
#  la contiene (net_startup comincia direttamente con i suoi controlli). Resta
#  vuota finche' non si lancia niente - per esempio quando lghmi e' appena
#  partito e il log in /tmp e' di una sessione precedente.
set SIMLOG_DIR ""

#  Lancia net_startup nella directory corrente e ne mostra l'output.
#
#  La simulazione parte in una SESSIONE PROPRIA (setsid), NON dentro la finestra
#  che la mostra, e non e' un dettaglio: net_startup lancia dispatcher, net_sked
#  e banco con `&` da una ksh non interattiva, e senza job control restano tutti
#  nel process group di chi li ha lanciati. Dentro un terminale quel process
#  group prende un SIGHUP ogni volta che il terminale se ne va - chiudendo la
#  finestra con la X, ma anche premendo Invio al prompt finale, perche' uscendo
#  il session leader il kernel manda SIGHUP al foreground process group - e
#  nessuno dei tre binari ignora SIGHUP: morivano tutti e tre, e con loro le HMI
#  aperte. Con setsid la finestra e' soltanto un visore del log: chiuderla non
#  tocca la simulazione, e la conferma della X lo dice.
#
#  Chiede comunque conferma PRIMA di partire, perche' la prima cosa che
#  net_startup fa e' `killsim`, che su Linux cancella TUTTE le SHM, le code e i
#  semafori dell'utente senza filtrare per chiave: se c'e' una simulazione in
#  corso la ferma, e con essa le HMI che le stanno sopra. Se dei processi di
#  simulazione sono vivi lo si dice esplicitamente nel testo della conferma.
proc lancia_net_startup {} {
    global SIMLOG SIMLOG_DIR
    set dir [pwd]
    if {![file exists [file join $dir variabili.rtf]]} {
        tk_messageBox -icon error -title "Start simulation" -parent . -message \
            "There is no variabili.rtf in this directory: net_startup cannot start.\n\n$dir"
        return
    }

    set vivi [sim_attiva]
    set avviso "Start the simulation in\n$dir\n\n"
    append avviso "net_startup runs killsim, which deletes every SHM segment, queue\n"
    append avviso "and semaphore of this user (no filtering by key)."
    if {[llength $vivi] > 0} {
        append avviso "\n\nWARNING: a simulation is already running\n"
        append avviso "([join $vivi ", "]): it will be stopped, and with it the open HMIs."
    }
    append avviso "\n\nProceed?"
    if {[tk_messageBox -icon warning -type yesno -default no -parent . \
             -title "Start simulation" -message $avviso] ne "yes"} {
        .status configure -text "Simulation launch cancelled."
        return
    }

    # Il log e' quello della simulazione corrente: riparte da zero a ogni lancio.
    # Aprirlo qui non e' solo per troncarlo: e' anche la prova che si puo'
    # scrivere. Il nome in /tmp e' fisso, quindi il file puo' essere di un altro
    # utente della stessa macchina; senza questo controllo la redirezione di
    # net_startup fallirebbe e la finestra di log resterebbe vuota, senza dire
    # perche'.
    if {[catch {open $SIMLOG w} fd]} {
        tk_messageBox -icon error -title "Start simulation" -parent . -message \
            "Cannot write the simulation log:\n$SIMLOG\n\n$fd"
        return
    }
    close $fd

    # Processo indipendente, come per le HMI: la simulazione sopravvive al Quit
    # del selettore. La dir si passa a net_startup, che fa il cd da se'.
    set sh "exec net_startup [list $dir] > [list $SIMLOG] 2>&1"
    if {[catch {exec setsid sh -c $sh &} err]} {
        # Senza setsid la simulazione resta nella sessione del selettore:
        # sopravvive comunque alla finestra di log, ma non alla fine della
        # sessione da cui lghmi e' partito.
        if {[catch {exec sh -c $sh &} err2]} {
            tk_messageBox -icon error -title "Start simulation" -parent . -message \
                "Cannot launch net_startup:\n$err2"
            return
        }
    }
    set SIMLOG_DIR $dir
    .status configure -text "net_startup started in $dir  (log: $SIMLOG)"
    mostra_log_sim $dir
}


# --- Finestra di log della simulazione -----------------------------------
#
# Prende il posto del terminale, e la differenza che conta non e' estetica: la
# finestra e' di lghmi, quindi la X passa da WM_DELETE_WINDOW e prima di
# chiudere si puo' dire all'utente cosa succede - e cosa non succede - alla
# simulazione. Un xterm non lo consente: non ha nessun hook sulla richiesta di
# chiusura del window manager.
#
# Il testo si aggiorna leggendo la CODA del file di log: si tiene la posizione
# gia' mostrata e a ogni giro si legge soltanto quello che e' arrivato dopo.

#  Stato del visore, UNO PER FINESTRA: lghmi scrive quattro tipi di log in /tmp
#  (net_startup, xstaz, mmi e uno per ogni HMI lanciata) e possono essere aperti
#  insieme. Prima erano due globali sole, perche' la finestra era una sola.
array set LOGPOS    {}   ;# byte del log gia' mostrati
array set LOGGEN    {}   ;# generazione: i cicli `after` di una finestra chiusa
                         ;# non devono lavorare per la successiva
array set LOGFILE   {}   ;# quale file segue ogni finestra
array set LOGCONSIM {}   ;# 1 = finestra della simulazione (stato + Stop)
array set LOGWIN    {}   ;# file -> finestra, per riusarla invece di aprirne due
set LOGSEQ 0             ;# contatore dei path dei toplevel: il nome di una task
                         ;# non puo' finirci dentro (Tk non accetta punti e
                         ;# spazi nei path dei widget)

#  Tetto alla PRIMA lettura di un log. Riaprendo si rilegge da capo, e un log
#  sfuggito di mano riempirebbe il visore: oltre questa soglia si parte dalla
#  coda, dicendolo. Largo apposta - i log veri stanno sotto i 100 KB - serve
#  come protezione, non come politica.
set LOGMAX [expr {512 * 1024}]

#  Aggiunge testo al visore di $w. Autoscroll SOLO se si sta guardando il fondo:
#  chi e' risalito a rileggere un errore non se lo vede scappare via.
proc log_scrivi {w testo {tag ""}} {
    set t $w.f.t
    if {![winfo exists $t] || $testo eq ""} return
    set infondo [expr {[lindex [$t yview] 1] >= 0.999}]
    $t configure -state normal
    if {$tag eq ""} { $t insert end $testo } else { $t insert end $testo $tag }
    $t configure -state disabled
    if {$infondo} { $t see end }
}

#  Un giro di lettura del log di $w. Si richiama da solo finche' la finestra
#  esiste ed e' quella per cui il ciclo era partito.
proc segui_log {w gen} {
    global LOGPOS LOGGEN LOGFILE
    if {![winfo exists $w] || $gen != $LOGGEN($w)} return
    set file $LOGFILE($w)
    if {[file exists $file]} {
        set dim [file size $file]
        if {$dim < $LOGPOS($w)} { set LOGPOS($w) 0 }   ;# log rifatto da capo
        if {$dim > $LOGPOS($w)} {
            if {![catch {open $file r} fd]} {
                seek $fd $LOGPOS($w)
                set nuovo [read $fd]
                set LOGPOS($w) [tell $fd]
                close $fd
                log_scrivi $w $nuovo
            }
        }
    }
    after 500 [list segui_log $w $gen]
}

#  Stato dei processi di simulazione. Ciclo separato e piu' lento di quello del
#  log: sim_attiva costa tre `pgrep`, e lo stato cambia raramente. Gira SOLO
#  sulla finestra della simulazione: sugli altri log non ci sono ne' la riga di
#  stato ne' il pulsante di stop, e tre pgrep ogni 3 secondi per finestra
#  sarebbero sprecati.
proc stato_sim_loop {gen} {
    global LOGGEN
    if {![winfo exists .simlog] || $gen != $LOGGEN(.simlog)} return
    aggiorna_stato_sim
    after 3000 [list stato_sim_loop $gen]
}

proc aggiorna_stato_sim {} {
    if {![winfo exists .simlog]} return
    set vivi [sim_attiva]
    if {[llength $vivi] > 0} {
        .simlog.b.stato configure -foreground "#006400" \
            -text "Simulation running: [join $vivi ", "]"
        .simlog.b.stop configure -state normal
    } else {
        .simlog.b.stato configure -foreground "#707070" \
            -text "No simulation process running"
        .simlog.b.stop configure -state disabled
    }
}

#  Chiusura di un visore: e' qui che finisce la X della finestra.
#
#  La conferma la merita SOLO la finestra della simulazione. Con la simulazione
#  in sessione propria chiudere non la ferma piu', ma la domanda resta - la
#  finestra e' l'unica cosa che dice che una simulazione sta girando, e chi la
#  chiude deve sapere che cosa lascia acceso e come spegnerlo. Il log di una HMI
#  o dell'mmi si chiude e basta: non c'e' niente che resti acceso per colpa sua.
proc chiudi_log {w} {
    global SIMLOG LOGCONSIM
    set consim [expr {[info exists LOGCONSIM($w)] && $LOGCONSIM($w)}]
    set vivi [expr {$consim ? [sim_attiva] : {}}]
    if {[llength $vivi] > 0} {
        set msg "Closing this window does NOT stop the simulation.\n\n"
        append msg "Still running: [join $vivi ", "].\n"
        append msg "They run in a session of their own, and with them stay alive the\n"
        append msg "HMIs and the faceplates already open.\n\n"
        append msg "To stop it the normal way: \"Simulator Shutdown ...\" in the\n"
        append msg "Master Menu of the desk, the window opened by net_startup. That\n"
        append msg "shuts the simulation down in an orderly way.\n\n"
        append msg "In an emergency, when that is not possible: the \"Kill simulation\"\n"
        append msg "button of this window, or the killsim command.\n\n"
        append msg "The log stays readable anyway in\n$SIMLOG\n\n"
        append msg "You can reopen this window from File -> Simulation log.\n\n"
        append msg "Close the log window?"
        if {[tk_messageBox -icon question -type yesno -default yes -parent $w \
                 -title "Simulation log" -message $msg] ne "yes"} return
    }
    destroy $w
}

#  Ammazza la simulazione di forza: e' l'uscita di EMERGENZA, non lo stop
#  normale. Lo stop normale si da' dalla finestra aperta da net_startup - il
#  banco - con la voce "Simulator Shutdown ..." del suo Master Menu, che
#  chiude la simulazione in modo ordinato (new_monit/messaggi.h, ShutdownLabel).
#  Questo pulsante serve quando quella strada non c'e' piu': banco morto o
#  piantato, finestra persa, processi rimasti appesi.
#
#  Lo fa killsim, cioe' lo stesso comando con cui net_startup comincia: e' il
#  modo previsto di ripulire l'ambiente (vedi CLAUDE.md / docs), non un kill a
#  mano dei processi. Brutale per definizione: su Linux killsim non filtra per
#  chiave e cancella TUTTE le SHM, le code e i semafori dell'utente.
proc ferma_simulazione {} {
    set vivi [sim_attiva]
    if {[llength $vivi] == 0} {
        aggiorna_stato_sim
        return
    }
    if {[auto_execok killsim] eq ""} {
        tk_messageBox -icon error -title "Kill simulation" -parent .simlog \
            -message "Executable 'killsim' not found in PATH.\nStart lghmi from a LegoPST environment (profile sourced)."
        return
    }
    set msg "Forcibly kill the running simulation?\n\n"
    append msg "This is the EMERGENCY stop. The normal way is \"Simulator Shutdown\n"
    append msg "...\" in the Master Menu of the desk, the window opened by\n"
    append msg "net_startup, which shuts the simulation down in an orderly way.\n"
    append msg "Use this button only when that is not possible any more.\n\n"
    append msg "The processes ([join $vivi ", "]) will be terminated, and with them\n"
    append msg "the HMIs and the faceplates sitting on top of them.\n\n"
    append msg "killsim does it, and on Linux it deletes EVERY SHM segment, queue\n"
    append msg "and semaphore of this user, without filtering by key.\n\n"
    append msg "Proceed?"
    if {[tk_messageBox -icon warning -type yesno -default no -parent .simlog \
             -title "Kill simulation" -message $msg] ne "yes"} return

    log_scrivi .simlog "\n--- killsim ---\n" lghmi
    .simlog configure -cursor watch
    update idletasks
    set rc [catch {exec killsim} out]
    catch {.simlog configure -cursor ""}
    log_scrivi .simlog "[string trim $out]\n" [expr {$rc ? "errore" : ""}]
    set vivi [sim_attiva]
    if {[llength $vivi] > 0} {
        log_scrivi .simlog "Still running: [join $vivi ", "]\n" errore
    } else {
        log_scrivi .simlog "Simulation stopped.\n" lghmi
    }
    aggiorna_stato_sim
    catch {.status configure -text "Simulation stopped with killsim."}
}

#  Costruisce il visore di $w sul file $file, se non c'e' gia'. Separata da
#  mostra_log_sim perche' la finestra si apre per motivi diversi - un
#  net_startup appena lanciato, una riapertura dal menu, il log di una HMI - e
#  solo il primo ha un log da far ripartire da capo.
#
#  $consim distingue LA finestra della simulazione da tutte le altre: solo lei
#  ha la riga di stato dei processi e il pulsante che chiama killsim. Un visore
#  del log dell'mmi o di una HMI non deve offrire un pulsante che ferma la
#  simulazione - non e' la sua - e non deve nemmeno chiedere conferma alla
#  chiusura: non lascia acceso niente.
proc crea_finestra_log {w titolo file consim} {
    global LOGFILE LOGCONSIM
    set LOGFILE($w)   $file
    set LOGCONSIM($w) $consim
    if {![winfo exists $w]} {
        toplevel $w
        #  Un filo piu' alta della dimensione naturale del contenuto (~437 px):
        #  all'apertura si vede tutto senza che nulla parta compresso.
        wm geometry $w 720x460
        wm minsize  $w 480 240
        #  La X della finestra: il motivo per cui il log sta qui e non in un
        #  terminale.
        wm protocol $w WM_DELETE_WINDOW [list chiudi_log $w]

        #  Le barre in basso si impacchettano PRIMA del visore, anche se stanno
        #  sotto: pack assegna lo spazio nell'ordine di impacchettamento, e chi
        #  arriva dopo si prende quel che resta. Mettendo per primo il testo, che
        #  ha -expand 1, pulsanti e scritte finivano fuori dalla finestra.
        #  Stessa scelta della finestra principale, dove .btn e .status sono
        #  impacchettati prima delle liste.
        frame  $w.b
        button $w.b.chiudi -text "Close" -width 10 -command [list chiudi_log $w]
        pack   $w.b.chiudi -side right -padx 4 -pady 6
        if {$consim} {
            label  $w.b.stato -anchor w -text ""
            button $w.b.stop -text "Kill simulation" -state disabled \
                   -command ferma_simulazione
            pack $w.b.stop  -side right -padx 4 -pady 6
            pack $w.b.stato -side left  -padx 6
        }
        pack $w.b -side bottom -fill x

        label $w.log -anchor w -padx 6 -foreground "#505050" -text "Log: $file"
        pack  $w.log -side bottom -fill x

        #  -width/-height del testo tengono la dimensione NATURALE della finestra
        #  sotto quella imposta da wm geometry: cosi' quello che si vede
        #  all'apertura e' tutto, non solo il visore.
        frame $w.f
        text  $w.f.t -wrap none -font TkFixedFont -state disabled -bd 1 \
              -width 80 -height 18 -relief sunken -background white \
              -yscrollcommand "$w.f.sy set" -xscrollcommand "$w.f.sx set"
        scrollbar $w.f.sy -orient vertical   -command "$w.f.t yview"
        scrollbar $w.f.sx -orient horizontal -command "$w.f.t xview"
        grid $w.f.t  $w.f.sy -sticky nsew
        grid $w.f.sx -sticky ew
        grid rowconfigure    $w.f 0 -weight 1
        grid columnconfigure $w.f 0 -weight 1
        pack $w.f -side top -fill both -expand 1 -padx 4 -pady 4
        #  Le righe scritte da lghmi si distinguono da quelle di net_startup.
        $w.f.t tag configure lghmi  -foreground "#000080"
        $w.f.t tag configure errore -foreground "#a00000"

        bind $w <Escape> [list chiudi_log $w]
    }
    #  Il titolo e il path del log si riscrivono SEMPRE, anche riusando una
    #  finestra gia' aperta: un secondo net_startup puo' partire da un'altra
    #  directory.
    wm title $w $titolo
    catch {$w.log configure -text "Log: $file"}
    return $w
}

#  Svuota il visore di $w e fa ripartire i suoi cicli `after` in una NUOVA
#  generazione: quelli precedenti si spengono da soli al primo giro (LOGGEN).
#  Da qui in poi segui_log rilegge il file da byte 0, quindi il log ricompare
#  per intero - ed e' il motivo per cui riaprire una finestra non costa nulla
#  piu' di questo.
#
#  L'eccezione e' un log piu' grande di LOGMAX: li' si parte dalla coda, e lo si
#  dice invece di far credere che quello sia tutto il log. La prima riga puo'
#  risultare tagliata a meta', ed e' il prezzo di non dover leggere il file due
#  volte per trovare un a capo.
proc riparti_visore_log {w} {
    global LOGPOS LOGGEN LOGFILE LOGMAX
    incr LOGGEN($w)
    set LOGPOS($w) 0
    $w.f.t configure -state normal
    $w.f.t delete 1.0 end
    $w.f.t configure -state disabled
    set file $LOGFILE($w)
    if {[file exists $file]} {
        set dim [file size $file]
        if {$dim > $LOGMAX} {
            set LOGPOS($w) [expr {$dim - $LOGMAX}]
            log_scrivi $w "--- showing the last [dimensione_leggibile $LOGMAX] of [dimensione_leggibile $dim] ---\n" lghmi
        }
    }
}

#  Porta il visore davanti e riavvia i suoi cicli di aggiornamento. Quello dello
#  stato della simulazione parte solo dove ha senso (vedi crea_finestra_log).
proc avvia_visore_log {w} {
    global LOGGEN LOGCONSIM
    wm deiconify $w
    raise $w
    segui_log $w $LOGGEN($w)
    if {$LOGCONSIM($w)} { stato_sim_loop $LOGGEN($w) }
}

#  Apre (o riusa) il visore del log per un net_startup APPENA LANCIATO. Il
#  visore della simulazione e' UNO: un secondo net_startup riparte da capo nella
#  stessa finestra, come il log.
proc mostra_log_sim {dir} {
    global SIMLOG
    set w [crea_finestra_log .simlog "net_startup - $dir" $SIMLOG 1]
    riparti_visore_log $w
    log_scrivi $w "Simulation started in $dir\n" lghmi
    log_scrivi $w "It runs in a session of its own: closing this window does NOT stop it.\n\n" lghmi
    avvia_visore_log $w
}

#  Riapre il visore su un log GIA' ESISTENTE: e' la voce File -> Simulation log.
#
#  Chiudere la finestra non ferma la simulazione - e' il punto di tutto il
#  meccanismo - ma finora la chiudeva anche per sempre: il log restava solo nel
#  file in /tmp, e con la finestra se ne andava l'unico modo grafico di FERMARE
#  la simulazione in emergenza, cioe' il pulsante "Kill simulation".
#
#  Non si riusa mostra_log_sim perche' li' il banner dice "Simulation started",
#  che su una riapertura sarebbe falso: qui non e' partito niente adesso. Il
#  titolo porta la dir dell'ultimo lancio se la sappiamo; se lghmi e' stato
#  riavviato nel frattempo il log in /tmp c'e' ancora ma la dir no, e il titolo
#  lo dice invece di inventarsela.
proc riapri_log_sim {} {
    global SIMLOG SIMLOG_DIR
    set titolo [expr {$SIMLOG_DIR ne "" ? "net_startup - $SIMLOG_DIR" \
                                        : "Simulation log - $SIMLOG"}]
    set w [crea_finestra_log .simlog $titolo $SIMLOG 1]
    riparti_visore_log $w
    if {[file exists $SIMLOG]} {
        log_scrivi $w "Log reopened: $SIMLOG\n" lghmi
        if {$SIMLOG_DIR ne ""} {
            log_scrivi $w "Simulation launched in $SIMLOG_DIR\n" lghmi
        }
        log_scrivi $w "\n" lghmi
    } else {
        #  Nessun file: o non si e' mai lanciato niente da qui, o il log e' stato
        #  cancellato. La finestra si apre lo stesso - serve il pulsante di stop.
        log_scrivi $w "No log file: $SIMLOG\n" lghmi
        log_scrivi $w "Nothing was launched from this window, or the log was removed.\n\n" lghmi
    }
    avvia_visore_log $w
}

# --- Gli altri log di lghmi ----------------------------------------------
#
# Oltre a net_startup, lghmi scrive in /tmp il log di ogni HMI che lancia
# (lghmi_<task>.log), quello dell'mmi e quello di xstaz. Sono l'unico posto dove
# finisce l'output di quei processi - partono tutti in background, staccati - e
# finora non c'era modo di leggerli dalla GUI: bisognava sapere che esistevano e
# andarseli a cercare a mano.
#
# L'elenco si fa con una `glob`, non con un registro popolato nei quattro punti
# di lancio: cosi' si vedono anche i log di una sessione PRECEDENTE di lghmi
# (lanci una HMI, esci, riapri), non c'e' stato da tenere sincronizzato, e
# l'ordine lo da' il mtime.

#  Dimensione in forma leggibile. Solo ASCII: con LANG=POSIX Tcl non decodifica
#  i file come UTF-8.
proc dimensione_leggibile {byte} {
    if {$byte < 1024}          { return "$byte B" }
    if {$byte < 1024*1024}     { return "[expr {$byte / 1024}] KB" }
    return [format "%.1f MB" [expr {$byte / 1048576.0}]]
}

#  Da quanto tempo e' stato scritto, in forma leggibile.
proc eta_leggibile {sec} {
    if {$sec < 60}    { return "just now" }
    if {$sec < 3600}  { return "[expr {$sec / 60}] min ago" }
    if {$sec < 86400} { return "[expr {$sec / 3600}] h ago" }
    return "[expr {$sec / 86400}] d ago"
}

#  Etichetta di un log, dedotta dal nome del file: e' una funzione pura del
#  nome, quindi non serve ricordarsi chi ha lanciato cosa.
proc etichetta_log {file} {
    set n [string range [file rootname [file tail $file]] 6 end]   ;# via "lghmi_"
    switch -exact -- $n {
        net_startup { return "net_startup (simulation)" }
        mmi         { return "mmi" }
        xstaz       { return "xstaz (faceplates)" }
        legopc      { return "legopc (CAD)" }
        lgswitch    { return "lgswitch (work area)" }
    }
    #  I log che portano il nome di cio' su cui hanno lavorato. Senza questi
    #  finirebbero tutti nel ramo "HMI:", che per una compilazione e' falso.
    foreach {prefisso etichetta} {
        kcompile_Regolation_ "kCompile Regolation:"
        kcompile_Task_       "kCompile Task:"
        kcompile_Page_       "kCompile Page:"
        config_              "config (regulation):"
        legopc_          "legopc:"
        kupsim_          "kUpSim:"
    } {
        if {[string match "$prefisso*" $n]} {
            return "$etichetta [string range $n [string length $prefisso] end]"
        }
    }
    return "HMI: $n"
}

#  I log di lghmi in /tmp, dal piu' recente. Esclude quello di net_startup, che
#  ha una voce sua: la sua finestra non e' un visore come gli altri, ha la riga
#  di stato e il pulsante che ferma la simulazione.
#
#  Il filtro sul proprietario non e' pedanteria: i nomi in /tmp sono FISSI,
#  quindi su una macchina con piu' utenti un lghmi_mmi.log puo' essere di un
#  altro - e' la stessa ragione per cui lancia_net_startup apre il suo log in
#  scrittura prima di partire, invece di darlo per suo.
proc elenco_log {} {
    global SIMLOG
    set out {}
    foreach f [glob -nocomplain [file join /tmp "lghmi_*.log"]] {
        if {$f eq $SIMLOG} continue
        if {[catch {file owned $f} mio] || !$mio} continue
        if {[catch {file mtime $f} quando]} continue
        lappend out [list $quando $f]
    }
    set res {}
    foreach e [lsort -integer -decreasing -index 0 $out] {
        lappend res [lindex $e 1]
    }
    return $res
}

#  Apre (o riporta davanti) il visore di un log qualsiasi. Una finestra per
#  file: riaprire lo stesso log riusa la sua, invece di accumularne due sullo
#  stesso contenuto.
#
#  Il path del toplevel e' un progressivo e non il nome della task: quello puo'
#  contenere punti e spazi, che Tk non accetta nei path dei widget.
proc apri_log {file} {
    global LOGWIN LOGSEQ
    if {![info exists LOGWIN($file)] || ![winfo exists $LOGWIN($file)]} {
        set LOGWIN($file) ".log[incr LOGSEQ]"
    }
    set w [crea_finestra_log $LOGWIN($file) [etichetta_log $file] $file 0]
    riparti_visore_log $w
    if {![file exists $file]} {
        log_scrivi $w "No log file: $file\n" lghmi
    }
    avvia_visore_log $w
}


# --- Tools: configurazione del simulatore (kUpSim) -----------------------
#
# kUpSim riallinea tutta la configurazione del simulatore CORRENTE, quello
# puntato da $KSIM: kConnex, kNetCompi, kCompStaz, kStazPages, kWinContext,
# kCompileSim, kCollect. L'ordine non e' arbitrario (i faceplate si risolvono
# contro gli indici di variabili.rtf, quindi vanno dopo le task).
#
# Nota: "lgupsim" e' un alias di kUpSim in Alg_env.sh, e gli alias non esistono
# nelle shell non interattive: qui si chiama kUpSim.

#  KPAGES del simulatore in <dir>, calcolata dal ksetsim VERO: di norma e'
#  $KSIM/globpages, ma $KSIM/ksim.conf la puo' ridefinire, e quella regola non
#  si ricopia qui. Si sorgia solo Alg_env.sh (che definisce ksetsim senza
#  chiamarlo) con HOME spostata in una directory che non esiste: ksetsim
#  scriverebbe ~/.legosim, e il chiamante decide da se' se scriverlo. Resta la
#  mkdir di status/ e log/ nella directory del simulatore, la stessa che
#  ksetsim fa in ogni shell. Costa circa 0,2 s. Se qualcosa non va si ripiega
#  sulla regola normale.
proc kpages_di {dir} {
    global env
    set ripiego [file join $dir globpages]
    set radice [expr {[info exists env(LEGOROOT)] ? $env(LEGOROOT) : ""}]
    if {$radice eq "" || ![file exists [file join $radice Alg_env.sh]]} {
        return $ripiego
    }
    set script {. "$LEGOROOT/Alg_env.sh" >/dev/null 2>&1; ksetsim "$1" >/dev/null 2>&1 && printf %s "$KPAGES"}
    if {[catch {exec env HOME=/nonexistent LEGOROOT=$radice bash -c $script lghmi $dir} out]
        || $out eq ""} {
        return $ripiego
    }
    return $out
}

#  Rende corrente il simulatore <nome> di $KSKED: nell'ambiente di lghmi
#  (KSIM, KSIMNAME, KPAGES), nel radiobutton di Tools e nella memoria per area
#  (ricorda_sim_area). Con scrivi = 1 lo registra anche in ~/.legosim, che e' la
#  scelta delle shell future. Ritorna 1 se ~/.legosim e' stato scritto.
#
#  KPAGES si aggiorna qui perche' la usa il pulsante mmi: prima restava quella
#  del simulatore con cui lghmi era partito, e mmi apriva le pagine sbagliate.
#  <scrivi> = 1: una scelta dell'utente, che va anche in ~/.legosim (le shell
#  future) e in ~/.lghmi_areas (l'ultimo simulatore dell'area). 0: solo in
#  memoria, per questa sessione - l'avvio di lghmi, che non e' una scelta.
proc imposta_simulatore {nome scrivi} {
    global env
    set dir [file join $env(KSKED) $nome]
    set scritto 0
    if {$scrivi && ![catch {
        set fd [open [file join $env(HOME) .legosim] w]
        puts $fd $nome
        close $fd
    }]} { set scritto 1 }

    set env(KSIM)     $dir
    set env(KSIMNAME) $nome
    set env(KPAGES)   [kpages_di $dir]
    set ::KSIMSCELTO  $nome
    if {$scrivi} { catch {ricorda_sim_area $nome} }
    catch {aggiorna_menu_tools}
    return $scritto
}

#  Il simulatore dell'area che sta nella directory <dir>: il nome, se <dir> e'
#  una delle directory di $KSKED, altrimenti "". Confronto per identita'
#  (stessa_directory, lgedit.tcl): ~/sked e' un link, e la stessa directory
#  arriva con grafie diverse (recenti, dialogo, directory di lancio).
proc simulatore_della_dir {dir} {
    global env
    if {![info exists env(KSKED)] || $env(KSKED) eq ""} { return "" }
    foreach s [lista_simulatori] {
        if {[stessa_directory [file join $env(KSKED) $s] $dir]} { return $s }
    }
    return ""
}

#  Il simulatore che si guarda e' quello su cui si lavora: se la directory
#  corrente e' un simulatore dell'area diverso da quello corrente, diventa
#  quello corrente. Senza questo le due scelte andavano ognuna per conto suo,
#  e si potevano guardare le task di un simulatore mentre kUpSim ne
#  riallineava un altro. Una directory che non e' un simulatore dell'area
#  (un modello, la home) lascia il simulatore com'e'. <scrivi> come in
#  imposta_simulatore. Ritorna il nome se l'ha cambiato, "" altrimenti.
proc allinea_simulatore {scrivi} {
    set s [simulatore_della_dir [pwd]]
    if {$s eq "" || $s eq [simulatore_corrente]} { return "" }
    imposta_simulatore $s $scrivi
    return $s
}

#  I simulatori disponibili: le sottodirectory di $KSKED, come la funzione
#  ksims del profilo.
proc lista_simulatori {} {
    global env
    if {![info exists env(KSKED)] || $env(KSKED) eq ""} { return {} }
    set out {}
    foreach d [lsort [glob -nocomplain -type d [file join $env(KSKED) *]]] {
        lappend out [file tail $d]
    }
    return $out
}

#  Nome del simulatore corrente, o "" se non ce n'e' uno valido.
proc simulatore_corrente {} {
    global env
    if {![info exists env(KSIM)] || ![file isdirectory $env(KSIM)]} { return "" }
    if {[info exists env(KSIMNAME)] && $env(KSIMNAME) ne ""} { return $env(KSIMNAME) }
    return [file tail $env(KSIM)]
}

#  Cambia il simulatore corrente.
#
#  Una GUI non puo' cambiare l'ambiente della shell che l'ha lanciata, ma il
#  profilo LegoPST ha gia' il posto giusto dove registrare la scelta:
#  ksetsim_default legge ~/.legosim a ogni avvio di shell (poi cassano0, poi il
#  primo di ksims). Scrivendo il nome la', la scelta vale per lghmi, per i
#  comandi che lancia e per le shell future.
#
#  KSIM da sola non basterebbe: ksetsim ne deriva una ventina di variabili
#  (KWIN, KPAGES, KSTATUS, KCASSAFORTE, KGRAF...) e sorgia $KSIM/ksim.conf.
#  Quella logica NON si riscrive qui: i comandi si lanciano in una shell che
#  sorgia il profilo e chiama ksetsim, cosi' a derivare e' il codice che esiste
#  gia'. Di quelle variabili, qui dentro se ne usano tre: KSIM e KSIMNAME per
#  menu e dialoghi, KPAGES per il pulsante mmi (dir_mmi).
proc scegli_simulatore {nome} {
    global env
    set dir [file join $env(KSKED) $nome]
    if {![file isdirectory $dir]} {
        tk_messageBox -icon error -title "Simulator" -parent . -message \
            "Simulator directory not found:\n$dir"
        return
    }
    set scritto [imposta_simulatore $nome 1]
    #  e lghmi si sposta nella sua directory, come con Open Simulator path: il
    #  simulatore scelto e' anche quello che si guarda (con -insim la
    #  directory e' fissa, ma allora la voce e' spenta)
    if {![stessa_directory [pwd] $dir]} { vai_a_loc $dir }
    set dove [expr {$scritto ? "written to ~/.legosim: applies to future shells too" \
                             : "~/.legosim not writable: applies to this session only"}]
    .status configure -text "Current simulator: $nome   |   directory: [pwd]   |   $dove"
}

# --- Tools: modifica del modello con legopc ------------------------------
#
# I controlli e il lancio di legopc stanno in lgedit.tcl (modifica_task),
# condivisi con il menu Edit di draw2gr: qui resta solo la scelta della task.

#  Tools -> Edit model: legopc sulla task selezionata, o vuoto se nessuna lo
#  e'. Con la simulazione in corso modifica_task rifiuta la task e propone
#  legopc vuoto (vuoto = 1), che non sta editando la simulazione.
proc lancia_legopc {} {
    global ITEMS_PROC LB_PROC

    if {[legopc_tix] eq ""} return

    set sel {}
    catch {set sel [$LB_PROC curselection]}
    if {[llength $sel] == 0} {
        avvia_legopc "" ""
        return
    }
    lassign [lindex $ITEMS_PROC [lindex $sel 0]] label dir name
    modifica_task $dir 1
}

#  Le notizie di lgedit.tcl vanno nella riga di stato. Quando legopc si chiude
#  si ricorda che la configurazione va riallineata: il modello puo' essere
#  cambiato, e finche' non si rifa' kUpSim la simulazione userebbe la vecchia.
proc legopc_evento {evento testo} {
    if {$evento eq "chiuso"} {
        set testo "legopc closed on '$testo' - if you changed the model, realign with Tools -> kUpSim."
    }
    catch {.status configure -text $testo}
}

# --- Tools/riquadro: le task di regolazione con config --------------------
#
# config e' l'editor delle pagine di regolazione (Motif). Come legopc non prende
# argomenti: lavora sulla DIRECTORY CORRENTE, quindi si fa cd nella task. Il
# precedente e' kc, che fa esattamente questo.
#
# Attenzione a non imitare kc fino in fondo: kc verifica l'esistenza della task
# cercando f01.dat, ma f01.dat ce l'hanno TUTTE le task, anche quelle di
# processo. Qui la selezione viene dal riquadro delle regolazioni, che e'
# popolato per tipo (R nell'S01) o per prefisso r_.
#
# config risolve le sue librerie (libut_reg/libreg, libut_mmi) a partire da
# LEGOCAD_USER, che il profilo pone a ~ : quindi $LEGOCAD_USER/legocad e' lo
# stesso $HOME/legocad di LG_ENTRY, e vale lo stesso controllo sull'area fatto
# per legopc - con il confronto per identita', non per nome.

#  La task di regolazione selezionata: {dir nome}, o {} se non ce n'e' una
#  utilizzabile (e in quel caso ha gia' spiegato perche').
#
#  $azione compare nei messaggi: "Edit", "compreg", "creatask".
proc regolazione_scelta {azione} {
    global env ITEMS_REG LB_REG
    set sel {}
    catch {set sel [$LB_REG curselection]}
    if {[llength $sel] == 0} {
        .status configure -text "Select a regulation task from the list."
        return {}
    }
    lassign [lindex $ITEMS_REG [lindex $sel 0]] label dir name
    set task [file tail $dir]
    if {![file isdirectory $dir]} {
        tk_messageBox -icon error -title $azione -parent . -message \
            "Regulation task directory not found:\n$dir"
        return {}
    }
    set entry [expr {[info exists env(LG_ENTRY)] ? $env(LG_ENTRY) : ""}]
    if {$entry eq "" || ![file isdirectory $entry]} {
        tk_messageBox -icon error -title $azione -parent . -message \
            "LG_ENTRY is not set, or is not a directory.\nStart lghmi from a LegoPST environment (profile sourced)."
        return {}
    }
    if {![stessa_directory [area_della_task $dir] $entry]} {
        set msg "'$task' does not belong to the current work area.\n\n"
        append msg "    task area:    [area_della_task $dir]\n"
        append msg "    LG_ENTRY:     [file normalize $entry]\n\n"
        append msg "These are different directories, not two spellings of the same one.\n\n"
        append msg "config resolves libut_reg/libreg and libut_mmi from the current\n"
        append msg "area: working on this task from here would use the WRONG\n"
        append msg "regulation library, with nothing saying so.\n\n"
        append msg "Switch work area first: File -> Work area in lghmi\n"
        append msg "(or lgswitch in a terminal, then reopen lghmi)."
        tk_messageBox -icon error -title $azione -parent . -message $msg
        .status configure -text "$azione: '$task' belongs to another work area."
        return {}
    }
    return [list $dir $task]
}

#  Avvisa se una simulazione e' in corso, e lascia decidere. A differenza di
#  legopc, che invece BLOCCA: un modello salvato cambia la topologia sotto la
#  task che gira, mentre sulla regolazione si lavora anche a simulazione viva -
#  ed e' una scelta deliberata, non una dimenticanza.
#
#  $extra e' l'avvertimento specifico dell'azione: creatask rigenera la task, e
#  va detto in modo piu' esplicito di quanto serva per l'editor.
proc conferma_con_simulazione {titolo task extra} {
    set vivi [sim_attiva]
    if {[llength $vivi] == 0} { return 1 }
    set msg "A simulation is running ([join $vivi ", "]).\n\n"
    append msg $extra
    append msg "\n\nProceed on '$task'?"
    return [expr {[tk_messageBox -icon warning -type yesno -default no -parent . \
                      -title $titolo -message $msg] eq "yes"}]
}

#  Apre config sulla task di regolazione selezionata. Processo indipendente e
#  log in /tmp, come legopc e le HMI: e' una GUI Motif, non un batch, quindi
#  niente visore di log che la segue.
proc lancia_config {} {
    set scelta [regolazione_scelta "Edit regulation"]
    if {[llength $scelta] == 0} return
    lassign $scelta dir task
    if {![conferma_con_simulazione "Edit regulation" $task \
             "config edits the regulation pages of this task. Saving and\ncompiling while it runs changes files the simulation is using."]} {
        .status configure -text "config: not opened on '$task'."
        return
    }
    set log [file join /tmp "lghmi_config_${task}.log"]
    set sh "cd [list $dir] && exec config >[list $log] 2>&1"
    if {[catch {exec setsid sh -c $sh &} err]} {
        if {[catch {exec sh -c $sh &} err2]} {
            tk_messageBox -icon error -title "Edit regulation" -parent . \
                -message "Cannot launch config:\n$err2"
            return
        }
    }
    .status configure -text "config started on '$task'  (log: $log)"
}

#  Le tre compilazioni di una task di regolazione, sulla sola task selezionata.
#  Sono batch con output da leggere, quindi vanno nel VISORE DI LOG - non in un
#  terminale, dove l'output morirebbe con la finestra.
#
#  L'ORDINE CONTA, ed e' questo:
#
#    1. Regolation  compila tutti gli schemi di regolazione   (config -c compreg)
#    2. Task        produce la task come eseguibile           (config -c creatask)
#    3. Page        compila le pagine che mmi animera'        (config -c compall)
#
#  Il secondo passo NON fa il terzo: sono tre tipi distinti. Le etichette del
#  menu portano il numero apposta - eseguirli in disordine non da' errore, da'
#  una task incoerente.
#
#  Si passa da kCompile e non da "config -c" nudo, che pure sarebbe piu' corto.
#  kCompile prima di compilare CANCELLA i vecchi *err* e net_compi.out, e senza
#  quella pulizia i conteggi dopo la compilazione sono falsi: un .reg_err
#  rimasto dalla corsa precedente fa leggere errori che non ci sono piu'. In
#  piu' fa kTest sull'ambiente, tiene un log suo in $KLOG e conta gli errori.
#
#  Il prezzo e' che kCompile vuole un SIMULATORE CORRENTE: kTest esce NOK senza
#  KSIMNAME, e kCompile si ferma. Per questo le tre voci sono spente quando non
#  c'e' un simulatore, come gia' le tre di kUpSim.
#
#  Attenzione alla grafia: il primo tipo si chiama "Regolation", non
#  "Regulation". Scritto in inglese corretto kCompile cade nell'else e stampa
#  solo la riga d'uso, senza che nulla spieghi perche'.
proc lancia_kcompile {tipo} {
    global env
    switch -exact -- $tipo {
        Regolation {
            set n 1
            set che "kCompile Regolation compiles the regulation schemes of this task\n(config -c compreg)."
        }
        Task {
            set n 2
            set che "kCompile Task REGENERATES the task (config -c creatask). The\nexecutable in proc/ will be rebuilt underneath the running\nsimulation, which would keep using the old one until it is restarted."
        }
        Page {
            set n 3
            set che "kCompile Page recompiles the pages that mmi animates\n(config -c compall, .pag -> .rtf). The running mmi would find them\nchanged underneath."
        }
        default { return }
    }
    set titolo "kCompile $tipo"

    set sim [simulatore_corrente]
    if {$sim eq ""} {
        tk_messageBox -icon error -title $titolo -parent . -message \
            "No current simulator: kCompile runs kTest first, which fails without\nKSIMNAME.\n\nPick one from Tools -> Current simulator, or with 'ksetsim <name>'."
        return
    }
    set scelta [regolazione_scelta $titolo]
    if {[llength $scelta] == 0} return
    lassign $scelta dir task
    if {![conferma_con_simulazione $titolo $task $che]} {
        .status configure -text "$titolo: not run on '$task'."
        return
    }

    set radice [expr {[info exists env(LEGOROOT)] ? $env(LEGOROOT) : ""}]
    if {$radice eq "" || ![file exists [file join $radice .profile_legoroot]]} {
        tk_messageBox -icon error -title $titolo -parent . -message \
            "LEGOROOT not defined, or profile not found: cannot prepare the kCompile environment."
        return
    }
    set prof [file join $radice .profile_legoroot]

    #  Frammento di shell, non una riga passata per [list]: le graffe di Tcl per
    #  sh non sono virgolette. Il cd viene DOPO ksetsim, perche' kCompile in
    #  modalita' Local prende la task da `pwd`.
    #  ksetsim con "||": se il simulatore non si puo' selezionare torna 1, e
    #  senza questa guardia la catena proseguirebbe su QUELLO DI PRIMA,
    #  compilando contro il simulatore sbagliato senza che niente lo dica.
    set riga ". [list $prof] [list $radice] >/dev/null 2>&1 ; ksetsim [list $sim] >/dev/null || { echo \"ksetsim [list $sim] failed: cannot select that simulator.\" ; exit 1 ; } ; cd [list $dir] && exec kCompile $tipo Local"
    set log [file join /tmp "lghmi_kcompile_${tipo}_${task}.log"]
    if {[esegui_con_log $log "$titolo - $task" $riga \
             [list "$n. kCompile $tipo on regulation task '$task'" \
                   "    $dir" \
                   "Simulator: $sim" \
                   "kCompile removes the stale *err* files first, so the error counts" \
                   "below are about THIS run. It keeps its own log in \$KLOG too," \
                   "and single-page errors stay in the task directory as" \
                   "[expr {$tipo eq "Page" ? "<page>.rtf_err" : "<page>.reg_err"}]."] bash]} {
        .status configure -text "$titolo started on '$task'  (log: $log)"
    }
}

#  Ricostruisce il menu Tools per intero, come per il menu File: le etichette
#  portano il nome del simulatore corrente, che cambia.
proc aggiorna_menu_tools {} {
    global env
    if {![winfo exists .mb.tools]} return

    set nome  [simulatore_corrente]
    set stato [expr {$nome ne "" ? "normal" : "disabled"}]
    set quale [expr {$nome ne "" ? $nome : "no simulator"}]

    #  Ordine: Edit model, kUpSim, kCompile, Terminal. Le varianti di kUpSim e
    #  di kCompile stanno in un sottomenu ciascuno: il menu resta corto e le
    #  varianti restano vicine. Il simulatore corrente si sceglie dal menu File
    #  (Current simulator, riempi_menu_simulatori): qui si ricostruisce il menu
    #  perche' la prima voce di kUpSim porta il suo nome.
    .mb.tools delete 0 end

    #  Sempre attiva, anche senza simulatore corrente e con una simulazione in
    #  corso: senza task selezionata apre legopc vuoto, che non tocca niente.
    #  I rifiuti li fa modifica_task (lgedit.tcl), che puo' spiegarli - una voce
    #  spenta no.
    .mb.tools add command -command lancia_legopc \
        -label "Edit model (legopc) - on the selected task, or empty"
    .mb.tools add separator

    #  kUpSim sul simulatore corrente: il nome sta nella prima voce, cosi' si sa
    #  su cosa si sta per agire. Spento, con le sue voci, senza simulatore.
    if {![winfo exists .mb.tools.kupsim]} { menu .mb.tools.kupsim -tearoff 0 }
    .mb.tools.kupsim delete 0 end
    .mb.tools.kupsim add command -state $stato -command [list lancia_kupsim {}] \
        -label "kUpSim - realign the configuration of $quale"
    .mb.tools.kupsim add command -state $stato -command [list lancia_kupsim -nommi] \
        -label "kUpSim -nommi - without the MMI faceplate pages"
    .mb.tools.kupsim add command -state $stato -command [list lancia_kupsim -n] \
        -label "kUpSim -n - preview: show the steps without running them"
    .mb.tools add cascade -label "kUpSim" -menu .mb.tools.kupsim -state $stato

    #  Le tre compilazioni della sola task di regolazione selezionata: accanto a
    #  kUpSim perche' sono compilazioni (l'editor invece sta sul pulsante del
    #  suo riquadro, dove c'e' la lista su cui agisce). Spente senza il riquadro
    #  delle regolazioni (-noreg), perche' agiscono sulla voce selezionata li'
    #  dentro, e senza simulatore corrente: kCompile comincia con kTest, che
    #  senza KSIMNAME esce NOK e ferma tutto.
    set sreg [expr {($::mostra_reg && $nome ne "") ? "normal" : "disabled"}]
    if {![winfo exists .mb.tools.kcompile]} { menu .mb.tools.kcompile -tearoff 0 }
    .mb.tools.kcompile delete 0 end
    .mb.tools.kcompile add command -state $sreg -command [list lancia_kcompile Regolation] \
        -label "1. kCompile Regolation - compile the regulation schemes"
    .mb.tools.kcompile add command -state $sreg -command [list lancia_kcompile Task] \
        -label "2. kCompile Task - build the task executable"
    .mb.tools.kcompile add command -state $sreg -command [list lancia_kcompile Page] \
        -label "3. kCompile Page - compile the pages mmi animates"
    .mb.tools add cascade -label "kCompile" -menu .mb.tools.kcompile -state $sreg
    .mb.tools add separator

    #  Sempre attiva: un terminale serve anche senza simulatore corrente.
    .mb.tools add command -command apri_terminale \
        -label "Terminal - shell in the current directory"
}

#  Tools -> Terminal: un terminale nella directory corrente del selettore,
#  cioe' quella del simulatore di solito (di lancio, o scelta con Open
#  Simulator path / le recenti: imposta_loc ci fa cd). L'ambiente e' quello di
#  lghmi: KSIM e le altre del simulatore corrente, LG_SIM_PATH.
#  Il terminale e' quello scelto dall'utente, aperto da lgterm (util97), che
#  lancia xfce4-terminal e tilix come processo nuovo: aperti nudi, mettono la
#  finestra in un'istanza gia' attiva, con l'ambiente di quella. Senza lgterm
#  - nel bundle FMU non c'e' - LG_XTERM o xterm.
proc apri_terminale {} {
    global env
    set dir [pwd]
    if {[auto_execok lgterm] ne ""} {
        set cmd [list lgterm]
    } else {
        set t [expr {[info exists env(LG_XTERM)] && $env(LG_XTERM) ne "" ? $env(LG_XTERM) : "xterm"}]
        if {[auto_execok $t] eq ""} {
            tk_messageBox -icon error -title "Terminal" -parent . -message \
                "No terminal found ('$t' is not installed).\nInstall one (sudo dnf install xfce4-terminal) and choose it in legopc,\nFile -> Settings, or set LG_XTERM."
            return
        }
        set cmd [list $t]
    }
    if {[catch {exec {*}$cmd &} err]} {
        tk_messageBox -icon error -title "Terminal" -parent . \
            -message "Cannot open the terminal:\n$err"
        return
    }
    #  quale si e' aperto davvero: lgterm sceglie da se' (preferenza di legopc
    #  prima di LG_XTERM), l'ambiente di lghmi puo' non saperlo
    if {[lindex $cmd 0] eq "lgterm"} {
        if {[catch {exec lgterm --which} quale] || $quale eq ""} { set quale "terminal" }
    } else {
        set quale [lindex $cmd 0]
    }
    .status configure -text "$quale opened in $dir"
}

#  Esegue un comando LegoPST seguendone l'output nel VISORE DI LOG, invece che
#  in un terminale.
#
#  Il terminale sembrava la scelta ovvia per un comando batch, ma perde
#  l'output: chiusa la finestra non resta niente, e di una compilazione si vuole
#  poter rileggere gli errori. Il visore invece tiene il file in /tmp, lo segue
#  dal vivo, si riapre da File -> Logs e non dipende da $LG_XTERM - che su una
#  macchina senza xterm non c'e'.
#
#  Il nome del file DEVE stare nella forma lghmi_*.log: elenco_log fa la glob
#  su quel modello, e cosi' la voce compare da sola nel sottomenu Logs.
#
#  $riga e' un FRAMMENTO DI SHELL, non una riga gia' quotata: ci si arriva
#  componendo le singole parole con [list], come fanno launch_hmi e
#  lancia_net_startup. NON si passa l'intero comando dentro [list]: quello e'
#  quoting TCL, che mette le graffe, e per sh le graffe non sono virgolette -
#  riceverebbe "{cd" come comando e il resto come parametri, fallendo con
#  "{cd: command not found" e lasciando il log vuoto.
#
#  Il frammento viene racchiuso in un gruppo { ...; } cosi' la redirezione vale
#  per TUTTO, non solo per l'ultimo comando della sequenza.
#
#  $shell e' "sh" o "bash": serve bash dove si sorgia il profilo LegoPST.
#  $intestazione sono le righe da scrivere nel visore prima di partire.
#  Ritorna 1 se il lancio e' riuscito.
proc esegui_con_log {file titolo riga intestazione {shell sh}} {
    global LOGWIN LOGSEQ

    #  Aprire il file qui non serve solo a troncarlo: e' la prova che si puo'
    #  scrivere. I nomi in /tmp sono fissi, quindi il file puo' essere di un
    #  altro utente della stessa macchina; senza questo controllo la redirezione
    #  fallirebbe e il visore resterebbe vuoto senza dire perche'.
    if {[catch {open $file w} fd]} {
        tk_messageBox -icon error -title $titolo -parent . -message \
            "Cannot write the log:\n$file\n\n$fd"
        return 0
    }
    close $fd

    set sh "{ $riga ; } > [list $file] 2>&1"
    if {[catch {exec setsid $shell -c $sh &} err]} {
        if {[catch {exec $shell -c $sh &} err2]} {
            tk_messageBox -icon error -title $titolo -parent . -message \
                "Cannot launch:\n$err2"
            return 0
        }
    }

    if {![info exists LOGWIN($file)] || ![winfo exists $LOGWIN($file)]} {
        set LOGWIN($file) ".log[incr LOGSEQ]"
    }
    set w [crea_finestra_log $LOGWIN($file) $titolo $file 0]
    riparti_visore_log $w
    foreach r $intestazione { log_scrivi $w "$r\n" lghmi }
    log_scrivi $w "\n" lghmi
    avvia_visore_log $w
    return 1
}

#  Lancia kUpSim sul simulatore corrente, seguendolo nel visore di log.
#
#  La shell sorgia il profilo e chiama ksetsim: e' la sola strada
#  per avere KSIM E tutte le sue derivate coerenti, ed e' anche il motivo per
#  cui serve bash (il profilo e' pensato per quella) e non la sh usata per
#  net_startup.
#
#  Chiede conferma dicendo cosa succede e su quale simulatore, tranne per
#  l'anteprima -n, che non esegue niente. Se una simulazione e' in corso lo
#  segnala: kUpSim riscrive variabili.rtf, r02.dat e le pagine, che quella
#  simulazione sta usando.
proc lancia_kupsim {opzioni} {
    global env
    set nome [simulatore_corrente]
    if {$nome eq ""} {
        tk_messageBox -icon error -title "kUpSim" -parent . -message \
            "No current simulator: KSIM is not defined, or is not a directory.\nPick one from Tools -> Current simulator, or with 'ksetsim <name>'."
        return
    }
    set anteprima [expr {[lsearch -exact $opzioni "-n"] >= 0}]

    if {!$anteprima} {
        set msg "Update the configuration of simulator\n\n"
        append msg "    $nome\n    $env(KSIM)\n\n"
        append msg "kUpSim redoes in sequence:\n"
        append msg "    kConnex       topology between tasks    -> S01\n"
        append msg "    kNetCompi     task compilation          -> variabili.rtf\n"
        append msg "    kCompStaz     faceplates for xstaz      -> r02.dat\n"
        if {[lsearch -exact $opzioni "-nommi"] < 0} {
            append msg "    kStazPages    faceplates as MMI pages\n"
            append msg "    kWinContext   Context.ctx of \$KWIN\n"
            append msg "    kCompileSim   compile the pages         -> .rtf\n"
        } else {
            append msg "    (the three MMI page steps are skipped)\n"
        }
        append msg "    kCollect      gathering into globpages + kMmiConfig\n"
        set vivi [sim_attiva]
        if {[llength $vivi] > 0} {
            append msg "\nWARNING: a simulation is running ([join $vivi ", "]).\n"
            append msg "It is using variabili.rtf, r02.dat and the pages, and would find\n"
            append msg "them changed underneath: better stop it first."
        }
        append msg "\n\nProceed?"
        if {[tk_messageBox -icon warning -type yesno -default no -parent . \
                 -title "kUpSim" -message $msg] ne "yes"} {
            .status configure -text "kUpSim cancelled."
            return
        }
    }

    set radice [expr {[info exists env(LEGOROOT)] ? $env(LEGOROOT) : ""}]
    if {$radice eq "" || ![file exists [file join $radice .profile_legoroot]]} {
        tk_messageBox -icon error -title "kUpSim" -parent . -message \
            "LEGOROOT not defined, or profile not found: cannot prepare the kUpSim environment."
        return
    }
    set prof [file join $radice .profile_legoroot]
    set kup [string trim "kUpSim $opzioni"]
    #  Vedi lancia_kcompile: senza il "||" un ksetsim fallito lascerebbe
    #  kUpSim a lavorare sul simulatore precedente, in silenzio.
    set riga ". [list $prof] [list $radice] >/dev/null 2>&1 ; ksetsim [list $nome] >/dev/null || { echo \"ksetsim [list $nome] failed: cannot select that simulator.\" ; exit 1 ; } ; exec $kup"
    set log [file join /tmp "lghmi_kupsim_${nome}.log"]
    if {![esegui_con_log $log "kUpSim - $nome" $riga \
             [list "$kup on simulator '$nome'" "    $env(KSIM)" \
                   "Output follows below. It stays readable in $log," \
                   "and this window reopens from File -> Logs."] bash]} {
        return
    }
    if {$anteprima} {
        .status configure -text "kUpSim -n: preview of the steps on $nome  (log: $log)"
    } else {
        .status configure -text "kUpSim started on $nome  (log: $log)"
    }
}

# --- Menu "?" : documentazione e versione --------------------------------
#
# La documentazione di LegoPST e' molta - una trentina di .md, due HTML e il
# manuale storico dei moduli in 218 pagine .htm - e il menu non deve diventarne
# il catalogo. Qui ci sono l'INDICE RAGIONATO, che e' l'hub di tutto il resto,
# il riferimento dei comandi kbin, il manuale dei moduli, e i tre documenti che
# rispondono alle domande di chi sta usando PROPRIO questa finestra: lghmi, la
# configurazione del simulatore che il menu Tools riallinea, e i faceplate che
# la lista di destra apre.
#
# I .md si aprono nel browser, dove si vedono come testo: e' quello che succede
# comunque ai 36 rimandi ai .md dentro l'indice ragionato, quindi il
# meccanismo resta uno solo.

#  {etichetta  path relativo a $LEGOROOT ?rilievo?}, con "--" per un separatore
#  e "MODULI" per il manuale storico dei moduli, che non e' un file del
#  repository ma una collezione sotto $LG_HTML e si apre con open_hlp. Il terzo
#  campo, se c'e', mette la voce in grassetto. Cosi' l'ordine e il risalto delle
#  voci stanno tutti qui.
#
#  Il README e' il primo, e in grassetto: e' il documento che dice CHE COS'E'
#  LegoPST - quello che si legge su GitHub - e viene prima di sapere dove sta
#  tutto il resto.
proc documenti_aiuto {} {
    return {
        {"LegoPST - project overview (README)"     README.md  rilievo}
        --
        {"Annotated documentation index"           INDICE_DOCUMENTAZIONE.html}
        {"kbin commands (the 192 kprocedure)"      kbin/kbin-riferimento-comandi-LegoPST.html}
        MODULI
        --
        {"This window: lghmi"                      Alg_legopc/LGHMI.md}
        {"Configuring a simulator: al_sim.conf"    docs/AL_SIM_CONF.md}
        {"Command faceplates (xstaz)"              Alg_rt/grafica/xstaz/HOWTO_faceplate.md}
    }
}

#  Converte un .md in HTML dentro una directory temporanea e ne ritorna il
#  path, oppure "" se non c'e' un convertitore.
#
#  Serve perche' i browser NON sanno rendere il Markdown: il sistema classifica
#  i .md come text/plain, quindi Firefox mostra il sorgente. Con un
#  convertitore, tabelle, blocchi di codice e citazioni si vedono per quello che
#  sono.
#
#  Il convertitore e' quello di LegoPST (md2html.tcl, Tcl puro): niente da
#  installare, e la resa e' la stessa su ogni installazione. Se quel file
#  mancasse si prova un convertitore esterno, e solo se non c'e' nemmeno quello
#  si apre il .md grezzo.
#
#  Il <base href> punta alla directory del documento ORIGINALE: senza quello i
#  rimandi relativi agli altri documenti - che i nostri .md usano molto - si
#  romperebbero, risolvendosi dentro la directory temporanea.
proc md_in_html {doc} {
    global env

    set tmp [expr {[info exists env(TMPDIR)] && $env(TMPDIR) ne "" ? $env(TMPDIR) : "/tmp"}]
    if {![file isdirectory $tmp] && [catch {file mkdir $tmp}]} { set tmp "/tmp" }
    set out [file join $tmp "lghmi_doc_[file rootname [file tail $doc]].html"]

    #  1. Il convertitore di LegoPST (md2html.tcl): in Tcl puro, quindi
    #     disponibile su ogni installazione senza installare niente. E' il
    #     primario, non il ripiego, per due ragioni: la resa e' identica
    #     dappertutto, e sui nostri documenti e' piu' fedele di markdown_py, che
    #     sbaglia i recinti di codice rientrati dentro una voce di elenco (li
    #     trasforma in un <code> malformato).
    if {[llength [info procs ::md2html::documento]] > 0} {
        if {![catch {md2html::documento $doc} pagina] && $pagina ne ""} {
            if {![catch {open $out w} fd]} {
                #  documento restituisce testo gia' decodificato da UTF-8
                #  (vedi md2html.tcl, CODIFICA): con la codifica di sistema,
                #  sotto LANG=POSIX, frecce e lineette diventerebbero "?"
                fconfigure $fd -encoding utf-8
                puts $fd $pagina
                close $fd
                return $out
            }
        }
    }

    #  2. Ripiego, se md2html.tcl non c'e' (deploy parziale, una bin vecchia):
    #     un convertitore esterno, se c'e'.
    set conv ""
    foreach c {markdown_py pandoc cmark-gfm cmark} {
        if {[auto_execok $c] ne ""} { set conv $c; break }
    }
    if {$conv eq ""} { return "" }
    set corpo ""
    switch -- $conv {
        markdown_py { set rc [catch {exec markdown_py -x tables -x fenced_code -x toc $doc} corpo] }
        pandoc      { set rc [catch {exec pandoc -f gfm -t html $doc} corpo] }
        default     { set rc [catch {exec $conv $doc} corpo] }
    }
    if {$rc || $corpo eq ""} { return "" }
    if {[catch {open $out w} fd]} { return "" }
    puts $fd "<!DOCTYPE html>\n<html><head><meta charset=\"utf-8\">"
    puts $fd "<title>[file tail $doc]</title>"
    puts $fd "<base href=\"file://[file dirname $doc]/\">"
    if {[llength [info procs ::md2html::stile]] > 0} {
        puts $fd "<style>[md2html::stile]</style>"
    }
    puts $fd "</head><body>"
    puts $fd $corpo
    puts $fd "</body></html>"
    close $fd
    return $out
}

#  Apre un documento del repository nel browser. Il browser lo sceglie
#  browser_disponibile di openhelp.tcl, la stessa di legopc, che scorre una
#  lista di candidati partendo da $LG_BROWSER.
proc apri_documento {relativo} {
    global env
    if {![info exists env(LEGOROOT)] || $env(LEGOROOT) eq ""} {
        tk_messageBox -icon error -title "Documentation" -parent . -message \
            "LEGOROOT not defined: cannot find the documentation."
        return
    }
    set doc [file join $env(LEGOROOT) $relativo]
    if {![file exists $doc]} {
        tk_messageBox -icon error -title "Documentation" -parent . -message \
            "Document not found:\n$doc"
        return
    }
    set preferito [expr {[info exists env(LG_BROWSER)] ? $env(LG_BROWSER) : ""}]
    set browser ""
    catch {set browser [browser_disponibile $preferito]}
    if {$browser eq ""} {
        tk_messageBox -icon error -title "Documentation" -parent . -message \
            "No browser available.\nCheck LG_BROWSER (currently: '$preferito')."
        return
    }
    # I .md passano per il convertitore, se c'e' uno.
    set nota ""
    if {[string tolower [file extension $doc]] eq ".md"} {
        set html [md_in_html $doc]
        if {$html ne ""} {
            set doc $html
        } else {
            set nota "  (md2html.tcl not found: unformatted text)"
        }
    }
    if {[catch {exec $browser $doc &} err]} {
        tk_messageBox -icon error -title "Documentation" -parent . -message \
            "Cannot start the browser:\n$browser $doc\n\n$err"
        return
    }
    .status configure -text "Opened in the browser: $relativo$nota"
}

# --- Menu "?" : versione dell'ambiente -----------------------------------

#  Versione di LegoPST, dalla stessa fonte che usa legopc: $LEGOROOT/version.h,
#  generato dal Makefile (bersaglio "version.h") a partire da git.
#
#  A differenza di legopc, qui il file NON viene generato se manca: lghmi puo'
#  girare dove il repository non e' scrivibile - per esempio dentro un bundle
#  FMU - e un selettore non deve mettersi a invocare make. Se manca si dice come
#  ottenerlo.
proc versione_legopst {} {
    global env
    set fuori [dict create versione "" build "" data "" nota ""]
    if {![info exists env(LEGOROOT)] || $env(LEGOROOT) eq ""} {
        dict set fuori nota "LEGOROOT not defined: LegoPST profile not sourced."
        return $fuori
    }
    set vf [file join $env(LEGOROOT) version.h]
    if {![file exists $vf]} {
        dict set fuori nota "version.h missing. Generate it with:\n    make -C $env(LEGOROOT) -f Makefile.mk version.h"
        return $fuori
    }
    if {[catch {open $vf r} fd]} {
        dict set fuori nota "version.h not readable: $vf"
        return $fuori
    }
    while {[gets $fd riga] >= 0} {
        if {[regexp {^#define\s+GIT_VERSION_STRING\s+"([^"]+)"} $riga -> v]} {
            dict set fuori versione $v
        } elseif {[regexp {^#define\s+BUILD_NUMBER\s+(\d+)} $riga -> v]} {
            dict set fuori build $v
        } elseif {[regexp {^#define\s+BUILD_DATE_STRING\s+"([^"]+)"} $riga -> v]} {
            dict set fuori data $v
        }
    }
    close $fd
    return $fuori
}

proc about_legopst {} {
    global env

    if {[winfo exists .about]} { raise .about; focus .about; return }

    set v [versione_legopst]
    set testo "LegoPST\n"
    append testo "LegoPowerSystemTechnology\n\n"
    if {[dict get $v versione] ne ""} {
        append testo "Version:\t[dict get $v versione]\n"
        append testo "Build:\t\t[dict get $v build]\n"
        append testo "Date:\t\t[dict get $v data]\n"
    } else {
        append testo "Version:\tnot available\n"
    }
    # Dove si sta lavorando: in questo ambiente e' la domanda che viene subito
    # dopo "che versione e'".
    append testo "\nLEGOROOT:\t[expr {[info exists env(LEGOROOT)] ? $env(LEGOROOT) : "-"}]\n"
    append testo "Simulator:\t[expr {[simulatore_corrente] ne "" ? [simulatore_corrente] : "-"}]\n"
    append testo "User root:\t[expr {[info exists env(LG_ENTRY)] ? $env(LG_ENTRY) : "-"}]\n"
    append testo "Directory:\t[pwd]"
    if {[dict get $v nota] ne ""} { append testo "\n\n[dict get $v nota]" }

    toplevel .about
    wm title .about "About LegoPST"
    wm resizable .about 0 0

    frame .about.f
    # Lo stesso logo di legopc, se c'e'.
    set logo [file join $env(LG_TIX) img lego.gif]
    if {[file exists $logo] && ![catch {image create photo imgabout -file $logo}]} {
        label .about.f.logo -image imgabout -bd 1 -relief sunken
        pack  .about.f.logo -side left -padx 3m -pady 2m
    }
    label .about.f.txt -justify left -padx 3m -pady 3m -text $testo
    pack  .about.f.txt -side left
    pack  .about.f -side top

    frame .about.b
    # Le note di rilascio stanno in $LG_INSTALL/relnotes.txt, come per legopc:
    # il pulsante compare solo se il file c'e' davvero.
    set rn ""
    if {[info exists env(LG_INSTALL)]} { set rn [file join $env(LG_INSTALL) relnotes.txt] }
    if {$rn ne "" && [file exists $rn]} {
        set ed [expr {[info exists env(LG_TEXTEDITOR)] && $env(LG_TEXTEDITOR) ne "" \
                      ? $env(LG_TEXTEDITOR) : "xdg-open"}]
        button .about.b.rn -text "Release Notes" -width 14 \
               -command [list catch [list exec $ed $rn &]]
        pack   .about.b.rn -side left -padx 3 -pady 6
    }
    button .about.b.ok -text "OK" -width 10 -command {destroy .about}
    pack   .about.b.ok -side left -padx 3 -pady 6
    pack   .about.b -side bottom
    bind .about <Escape> {destroy .about}
    focus .about.b.ok
}

# --- Interfaccia ---------------------------------------------------------
if {$doppia} {
    wm title . "LegoPST - HMI and faceplates"
    # Stessa larghezza del banco (new_monit, 680 px): le due finestre si usano
    # insieme, una sopra l'altra, e allineate stanno meglio. L'altezza e' quella
    # che serve a 12 righe di lista.
    #  Con le regolazioni l'altezza si calcola DOPO aver costruito i riquadri
    #  (vedi in fondo): cablarla qui darebbe alle tre liste altezze diverse.
    wm geometry . 680x328
    wm minsize . 560 [expr {$mostra_reg ? 380 : 300}]
} elseif {$stazmode} {
    wm title . "LegoPST - Faceplate launcher (xstaz)"
    wm minsize . 520 280
} else {
    wm title . "LegoPST - HMI launcher"
    wm minsize . 340 280
}

# Barra dei menu. Finora non c'era: nasce per "Open Simulator path", che cambia
# la directory di lavoro del selettore e non e' un'azione sulle liste come i
# pulsanti in basso. Refresh e Quit ci stanno per comodita', ma restano anche
# in basso, dove si usano.
menu .mb -tearoff 0
. configure -menu .mb
menu .mb.file -tearoff 0 -postcommand aggiorna_voci_log
.mb add cascade -label "File" -menu .mb.file
# Le voci del menu File le costruisce aggiorna_menu_file, che lo rifa' da capo
# ogni volta che i path recenti cambiano. Con -insim nascono tutte disabilitate,
# recenti compresi: cambiare directory scollegherebbe il selettore dalla
# simulazione che l'ha lanciato.
menu .mb.tools -tearoff 0
.mb add cascade -label "Tools" -menu .mb.tools

# Menu "?" con lo stesso nome che usa legopc, per coerenza fra le due finestre.
# Font per le voci in rilievo: quello dei menu, in grassetto. Derivato invece
# che scritto a mano, cosi' segue il tema e la dimensione del sistema.
catch {
    font create fontMenuRilievo {*}[font actual TkMenuFont]
    font configure fontMenuRilievo -weight bold
}

menu .mb.aiuto -tearoff 0
.mb add cascade -label "?" -menu .mb.aiuto
.mb.aiuto add command -label "About LegoPST" -command about_legopst
.mb.aiuto add separator

# Le voci che puntano a un file assente nascono disabilitate invece di sparire:
# si vede che il documento e' previsto e che manca.
foreach _voce [documenti_aiuto] {
    if {$_voce eq "--"} { .mb.aiuto add separator; continue }
    if {$_voce eq "MODULI"} {
        # il manuale storico dei moduli: lo apre open_hlp di openhelp.tcl, la
        # stessa proc della voce Help di legopc
        if {[info exists env(LG_HTML)] \
            && [file exists [file join $env(LG_HTML) index.htm]] \
            && [llength [info procs open_hlp]] > 0} {
            .mb.aiuto add command -label "Modules help (legacy manual)" \
                                  -command {open_hlp index}
        }
        continue
    }
    lassign $_voce _etichetta _rel _rilievo
    set _ok [expr {[info exists env(LEGOROOT)] \
                   && [file exists [file join $env(LEGOROOT) $_rel]]}]
    if {$_rilievo ne ""} {
        .mb.aiuto add command -label $_etichetta -command [list apri_documento $_rel] \
                              -state [expr {$_ok ? "normal" : "disabled"}] \
                              -font fontMenuRilievo
    } else {
        .mb.aiuto add command -label $_etichetta -command [list apri_documento $_rel] \
                              -state [expr {$_ok ? "normal" : "disabled"}]
    }
}
unset -nocomplain _voce _etichetta _rel _rilievo _ok

#  Se KSIM non e' arrivata nell'ambiente, sceglie il simulatore da se'.
#
#  Il simulatore corrente lo fissa normalmente il profilo (ksetsim_default:
#  ~/.legosim, poi cassano0, poi il primo di ksims) e lghmi lo eredita. Ma
#  l'eredita' non e' garantita: il wrapper risorgia il profilo solo se LG_TIX e'
#  vuota, quindi un lancio da un ambiente che ha LG_TIX ma non KSIM arrivava qui
#  senza simulatore - e tutte le voci di Tools restavano SPENTE, senza che nulla
#  dicesse perche'. Bastava poi passare da Tools -> Current simulator per
#  vederle accendersi, il che faceva sembrare un capriccio dell'interfaccia.
#
#  Qui si rifa' la stessa cascata del profilo, ma SOLO in memoria: non si scrive
#  ~/.legosim, perche' aprire il selettore non e' una scelta dell'utente e non
#  deve cambiare il default delle shell future. Delle variabili derivate si
#  aggiorna solo KPAGES, che usa il pulsante mmi; le altre (KWIN, KLOG...) no:
#  i comandi girano in una shell che chiama ksetsim per conto suo, ed e' quella
#  a derivarle.
proc simulatore_di_ripiego {} {
    global env
    if {[simulatore_corrente] ne ""} { return "" }
    if {![info exists env(KSKED)] || $env(KSKED) eq ""} { return "" }
    set candidati {}
    if {![catch {open [file join $env(HOME) .legosim] r} fd]} {
        set voluto [string trim [read $fd]]
        close $fd
        if {$voluto ne ""} { lappend candidati $voluto }
    }
    lappend candidati cassano0
    foreach s [lista_simulatori] { lappend candidati $s }
    foreach nome $candidati {
        set dir [file join $env(KSKED) $nome]
        if {[file isdirectory $dir]} {
            set env(KSIM)     $dir
            set env(KSIMNAME) $nome
            set env(KPAGES)   [kpages_di $dir]
            return $nome
        }
    }
    return ""
}

# Il simulatore corrente e' quello che il profilo ha scelto all'avvio
# (ksetsim_default: ~/.legosim, poi cassano0, poi il primo di ksims). La
# variabile tiene il radiobutton del sottomenu allineato.
set ::RIPIEGO [simulatore_di_ripiego]
# Lanciato dalla directory di un simulatore dell'area, lghmi lavora su quello:
# diventa il simulatore corrente, solo in memoria (l'avvio non e' una scelta
# e non cambia ~/.legosim). Il ripiego, se c'era, non conta piu'.
if {[allinea_simulatore 0] ne ""} { set ::RIPIEGO "" }
set ::KSIMSCELTO [simulatore_corrente]

# Il titolo impostato sopra e' la base: aggiorna_area ci aggiunge l'area di
# lavoro e, con -insim, "(from the desk)". L'area va calcolata prima dei
# recenti, perche' il menu File mostra solo quelli dell'area corrente.
set ::TITOLO_BASE [wm title .]
aggiorna_area

carica_recenti
aggiorna_menu_file
aggiorna_menu_tools

# La directory di lancio entra fra i recenti solo se e' una directory di
# simulazione (c'e' un S01 o variabili.rtf): lanciando lghmi da casa, in
# dir-scan, non ha senso ricordarsela. Cosi' il menu serve dalla prima volta,
# senza dover passare almeno una volta dal dialogo di selezione.
if {[file exists [file join [pwd] S01]] || \
    [file exists [file join [pwd] variabili.rtf]]} {
    ricorda_recente [pwd]
}


# Le due righe di intestazione che stavano qui - quella che spiegava come erano
# disposte le liste e quella che diceva come si apre una voce - sono sparite: la
# prima la dicono gia' i titoli dei riquadri, la seconda e' diventata il balloon
# help sui titoli stessi (BALLOON_APRI, piu' sotto). Cosi' la finestra comincia
# direttamente con le liste, che sono quello per cui la si apre.

# Intestazioni che dipendono dalla directory corrente: il simulatore S01 e il
# Set Sim path. Vengono create SEMPRE, anche quando non servono, perche'
# File -> Open Simulator path puo' cambiare directory a runtime e con essa la
# modalita': creandole solo all'avvio, aprendo una dir con S01 da una sessione
# partita in dir-scan non ci sarebbe nessun widget da riempire. Stanno in un
# frame perche' l'ordine fra le due resti stabile quando si mostrano e si
# nascondono (pack/pack forget dentro il frame, non sulla toplevel).
frame .hdr
pack  .hdr -side top -fill x
# L'area di lavoro (File -> Work area) sta sopra le altre due intestazioni ed
# e' sempre visibile: e' il contesto di tutto il resto. In rosso quando i link
# non indicano un'area sola.
label .area -text "" -anchor w -padx 6
pack  .area -side top -fill x -before .hdr
label .hdr.s01 -text "" -anchor w -padx 6 -foreground "#006400"
label .hdr.loc -text "" -anchor w -padx 6 -foreground blue

# Barra in basso: Refresh, mmi e Quit. Le pagine si aprono dalla lista (doppio
# click o tasto destro), non da un pulsante.
frame .btn
button .btn.refresh -text "Refresh" -command refresh_list
button .btn.quit    -text "Quit"    -command exit
# Lancia la simulazione nella directory corrente. Nome del comando come
# etichetta, come per il pulsante "mmi": si riconosce cosa fa. Parte disabilitato
# ed e' aggiorna_stato_startup a deciderne lo stato, in base alla presenza di
# variabili.rtf nella dir corrente.
button .btn.start -text "net_startup" -width 11 -state disabled \
                  -command lancia_net_startup
# Lancio di un'altra applicazione, non un'azione sulla lista: sta al centro
# della barra, largo il doppio e con il verde della finestra dell'MMI, cosi' si
# riconosce a colpo d'occhio. Centratura con `place` (non pack -expand): il
# centro e' quello della finestra, non della porzione lasciata libera da
# Refresh e Quit, che hanno larghezze diverse.
button .btn.mmi -text "mmi" -width 11 -command launch_mmi \
                -background "#50a050" -activebackground "#60c060" \
                -foreground black -activeforeground black
pack  .btn.refresh -side left  -padx 4 -pady 6
pack  .btn.start   -side left  -padx 4 -pady 6
pack  .btn.quit    -side right -padx 4 -pady 6
place .btn.mmi -relx 0.5 -rely 0.5 -anchor center
pack .btn -side bottom -fill x

label .status -text "" -anchor w -relief sunken -bd 1 -padx 4
pack .status -side bottom -fill x

#  Testo del balloon sui titoli delle liste: prende il posto della riga fissa
#  "Per aprire: ..." che stava sotto l'intestazione. E' il modo di aprire una
#  voce, ed e' lo stesso per le due liste.
set BALLOON_APRI "Double-click an entry to open it, or right-click -> Open page"

#  Mette il suggerimento a comparsa su un widget, se balloon.tcl e' stato
#  caricato. Senza il controllo, un lghmi lanciato dove $LG_TIX/balloon.tcl
#  manca - un bundle FMU incompleto, una bin vecchia - morirebbe qui invece di
#  partire senza suggerimenti.
proc aiuto_a_comparsa {widget testo} {
    if {[llength [info procs set_balloon]] > 0} {
        catch {set_balloon $widget $testo}
    }
}

#  Costruisce un riquadro "intestazione + lista + pulsante". Ritorna il path
#  della listbox.
proc crea_riquadro {parent titolo larghezza {balloon ""}} {
    global BALLOON_APRI
    frame $parent
    if {$titolo ne ""} {
        label $parent.h -text $titolo -anchor w -padx 4 -pady 2 -foreground "#000080"
        pack  $parent.h -side top -fill x
        aiuto_a_comparsa $parent.h \
            [expr {$balloon ne "" ? $balloon : $BALLOON_APRI}]
    }
    frame $parent.f
    listbox $parent.f.lb -yscrollcommand "$parent.f.sb set" -height 12 \
                         -width $larghezza -activestyle dotbox -exportselection 0
    scrollbar $parent.f.sb -orient vertical -command "$parent.f.lb yview"
    pack $parent.f.sb -side right -fill y
    pack $parent.f.lb -side left -fill both -expand 1
    pack $parent.f -side top -fill both -expand 1
    return $parent.f.lb
}

#  Contenitore delle liste. Con le regolazioni accese il layout e' 2+1:
#  PROCESSO e REGOLAZIONE affiancate in alto - sono le due liste su cui si
#  lavora di piu' e si confrontano fra loro - e i faceplate xstaz sotto, a tutta
#  larghezza. La finestra NON si allarga: resta 680 px, la larghezza del banco,
#  che e' il motivo per cui quel numero e' quello, e cresce solo in altezza.
#  Tre liste affiancate avrebbero sfondato la larghezza o ridotto ogni colonna a
#  una ventina di caratteri.
#
#  $sopra e' un FRATELLO di .pv, non un suo figlio: Tk lo accetta come pannello,
#  ma .pv - creata dopo - gli finirebbe DAVANTI nell'ordine di sovrapposizione,
#  lasciando un rettangolo grigio al posto delle liste. Da qui la raise.
proc impila_sotto {sopra sotto} {
    .pv add $sopra -minsize 150
    .pv add $sotto -minsize 150
    raise $sopra .pv
}

set BALLOON_REG "Double-click a task to edit its regulation with config"

if {$doppia} {
    # Due liste affiancate a meta' schermo ciascuna (39 caratteri -> ~328 px):
    # e' la ripartizione che sta in 680 px, la larghezza del banco. I faceplate
    # hanno etichette piu' lunghe, ma il divisorio si trascina.
    #
    # .pv va creata PRIMA di .pv.staz, che e' un suo figlio.
    if {$mostra_reg} {
        panedwindow .pv -orient vertical -sashrelief raised -sashwidth 6
        pack .pv -side top -fill both -expand 1 -padx 6 -pady 2
    }
    panedwindow .pw -orient horizontal -sashrelief raised -sashwidth 6
    if {!$mostra_reg} { pack .pw -side top -fill both -expand 1 -padx 6 -pady 2 }
    set LB_PROC [crea_riquadro .pw.proc "Process pages" 39]
    if {$mostra_reg} {
        set LB_REG  [crea_riquadro .pw.reg  "Regulation tasks (r_*)" 39 $BALLOON_REG]
        set LB_STAZ [crea_riquadro .pv.staz "xstaz faceplates" 39]
        .pw add .pw.proc -minsize 180
        .pw add .pw.reg  -minsize 180
        impila_sotto .pw .pv.staz
    } else {
        set LB_STAZ [crea_riquadro .pw.staz "xstaz faceplates" 39]
        .pw add .pw.proc -minsize 180
        .pw add .pw.staz -minsize 180
    }
    bind $LB_PROC <Double-1> { launch_hmi }
    bind $LB_PROC <Return>   { launch_hmi }
    bind $LB_STAZ <Double-1> { apri_faceplate }
    bind $LB_STAZ <Return>   { apri_faceplate }
    bind $LB_PROC <Button-3> [list popup_open_page $LB_PROC launch_hmi     %y %X %Y]
    bind $LB_STAZ <Button-3> [list popup_open_page $LB_STAZ apri_faceplate %y %X %Y]
} elseif {$stazmode} {
    if {$mostra_reg} {
        panedwindow .pv -orient vertical -sashrelief raised -sashwidth 6
        pack .pv -side top -fill both -expand 1 -padx 6 -pady 2
    }
    set LB_STAZ [crea_riquadro .f "xstaz faceplates" 68]
    if {!$mostra_reg} { pack .f -side top -fill both -expand 1 -padx 6 -pady 2 }
    bind $LB_STAZ <Double-1> { apri_faceplate }
    bind . <Return>          { apri_faceplate }
    bind $LB_STAZ <Button-3> [list popup_open_page $LB_STAZ apri_faceplate %y %X %Y]
    if {$mostra_reg} {
        set LB_REG [crea_riquadro .pv.reg "Regulation tasks (r_*)" 68 $BALLOON_REG]
        impila_sotto .f .pv.reg
    }
} else {
    if {$mostra_reg} {
        panedwindow .pv -orient vertical -sashrelief raised -sashwidth 6
        pack .pv -side top -fill both -expand 1 -padx 6 -pady 2
    }
    set LB_PROC [crea_riquadro .f "Process pages" 34]
    if {!$mostra_reg} { pack .f -side top -fill both -expand 1 -padx 6 -pady 2 }
    bind $LB_PROC <Double-1> { launch_hmi }
    bind . <Return>          { launch_hmi }
    bind $LB_PROC <Button-3> [list popup_open_page $LB_PROC launch_hmi %y %X %Y]
    if {$mostra_reg} {
        set LB_REG [crea_riquadro .pv.reg "Regulation tasks (r_*)" 34 $BALLOON_REG]
        impila_sotto .f .pv.reg
    }
}

#  Doppio clic e tasto destro sulle regolazioni, come sulle altre due liste.
if {$mostra_reg} {
    bind $LB_REG <Double-1> { lancia_config }
    bind $LB_REG <Return>   { lancia_config }
    bind $LB_REG <Button-3> [list popup_open_page $LB_REG lancia_config %y %X %Y \
                                 "Edit regulation"]
}

bind . <Escape> { exit }

#  Altezza della finestra: quella RICHIESTA dal contenuto, non un numero
#  cablato. Le tre liste chiedono tutte 12 righe (crea_riquadro), e una
#  panedwindow alla prima apertura da' a ogni pannello la sua dimensione
#  naturale: chiedendo alla finestra esattamente quel che le serve, le tre liste
#  partono alla STESSA altezza. Con un'altezza fissa il pannello di sotto si
#  prendeva quel che avanzava, e si apriva schiacciato.
#  La larghezza resta 680, che non dipende dal contenuto.
if {$mostra_reg} {
    update idletasks
    wm geometry . 680x[winfo reqheight .]
}

refresh_list
