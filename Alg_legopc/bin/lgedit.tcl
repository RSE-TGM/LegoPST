# lgedit.tcl - modifica del modello di una task con legopc (il CAD).
#
# Sorgiato da:
#   lghmi.tcl    sempre: Tools -> Edit model (legopc), e i controlli che lghmi
#                usa anche altrove (sim_attiva, stessa_directory,
#                area_della_task);
#   draw2gr.tcl  SOLO se lanciato con -edit (menu Edit). Senza -edit draw2gr
#                non lo legge nemmeno, quindi gli altri usi di draw2gr
#                (legopc, FMU, lg_cosim, Windows) restano come prima.
#
# Una sola implementazione: il selettore e la HMI rifiutano negli stessi casi
# e con le stesse parole.
#
# legopc e' il CAD: apre il .tom della task e ne riscrive schema, .i5 e
# configurazione. NON si lancia "lgpc": quello e' un ALIAS di Alg_env.sh
# (export LG_TIX=$LG_BIN; wish $LG_TIX/legopc.tix) e gli alias non esistono
# nelle shell non interattive - stessa ragione per cui lghmi chiama kUpSim e
# non lgupsim. Si lancia direttamente wish su legopc.tix EREDITANDO LG_TIX,
# cosi' CAD e HMI vengono dalla stessa installazione: quella da cui e' partita
# l'applicazione che lo chiede.
#
# Le proc di questo file non scrivono direttamente all'utente fuori dai
# dialoghi: passano da legopc_evento, che ogni applicazione ridefinisce DOPO il
# source. lghmi scrive nella riga di stato; draw2gr, che non ce l'ha, scrive
# nel log e avvisa con un dialogo quando legopc si chiude.

#  Notizie su legopc per l'applicazione. evento:
#    stato   testo = una riga di stato (avvio, rifiuti)
#    chiuso  testo = nome della task su cui legopc si e' chiuso
#  Questa e' solo la versione di ripiego: lghmi e draw2gr la ridefiniscono.
proc legopc_evento {evento testo} {
    if {$evento eq "chiuso"} {
        set testo "legopc closed on '$testo'"
    }
    puts $testo
}

#  Processi di simulazione vivi adesso. Ritorna la lista dei nomi trovati.
proc sim_attiva {} {
    set vivi {}
    foreach p {dispatcher net_sked banco} {
        if {![catch {exec pgrep -x $p}]} { lappend vivi $p }
    }
    return $vivi
}

#  L'area di lavoro (LG_ENTRY) a cui appartiene una task: la directory che la
#  contiene. Su Linux NON c'e' il livello "models" - quello esiste solo nella
#  versione Windows - e infatti il profilo pone LG_MODELS=LG_ENTRY. Quindi il
#  modello di una task sta in <area>/<task>/<task>.tom.
proc area_della_task {dir} {
    return [file normalize [file dirname $dir]]
}

#  Due path indicano la STESSA directory? Si confrontano device e inode, non le
#  stringhe.
#
#  Confrontare i nomi non funziona, e non e' un dettaglio: $HOME/legocad e'
#  un SYMLINK (lo gestisce lgswitch) e `file normalize` di Tcl non risolve i
#  symlink - rende assoluto e toglie "." e "..", nient'altro. LG_ENTRY arriva
#  dal profilo nella grafia col link ($HOME/legocad), mentre in modalita' S01 il
#  path della task nasce da [file normalize [file join $s01dir $relpath]]: li'
#  il ".." costringe a risolvere il link, e viene fuori il path REALE
#  (.../legopst_<area>/legocad/<task>). Stessa directory, due grafie: il
#  confronto testuale rifiutava sistematicamente le task del simulatore su cui
#  si sta lavorando, che e' il caso normale.
#
#  device+inode e' l'identita' vera: regge symlink, mount e grafie diverse, e
#  continua a distinguere le aree DAVVERO diverse.
proc stessa_directory {a b} {
    if {[catch {file stat $a sa}]} { return 0 }
    if {[catch {file stat $b sb}]} { return 0 }
    return [expr {$sa(dev) == $sb(dev) && $sa(ino) == $sb(ino)}]
}

#  Il modello di una task: UN SOLO .tom, omonimo della sua directory. Altri
#  .tom eventualmente presenti nella stessa dir non sono alternative - sono
#  un'anomalia dei dati - e vengono ignorati.
proc tom_della_task {dir} {
    return [file join $dir "[file tail $dir].tom"]
}

#  legopc.tix dell'installazione corrente (LG_TIX). Se non c'e' lo dice e
#  ritorna "".
proc legopc_tix {} {
    set lgtix [expr {[info exists ::env(LG_TIX)] ? $::env(LG_TIX) : ""}]
    set lpc [file join $lgtix legopc.tix]
    if {$lgtix eq "" || ![file exists $lpc]} {
        set app [file rootname [file tail $::argv0]]
        tk_messageBox -icon error -title "legopc" -parent . -message \
            "legopc.tix not found (LG_TIX='$lgtix').\nStart $app from a LegoPST environment (profile sourced)."
        return ""
    }
    return $lpc
}

#  Il processo $pid e' un legopc, cioe' un wish che esegue legopc.tix?
#  pgrep -f guarda tutta la riga di comando, e anche un editor aperto su
#  legopc.tix la contiene: si tengono solo gli interpreti.
proc esegue_legopc {pid} {
    if {[catch {open /proc/$pid/cmdline r} fp]} { return 0 }
    fconfigure $fp -translation binary
    set argomenti [split [read $fp] "\0"]
    close $fp
    if {![string match *wish* [file tail [lindex $argomenti 0]]]} { return 0 }
    foreach a [lrange $argomenti 1 end] {
        if {[file tail $a] eq "legopc.tix"} { return 1 }
    }
    return 0
}

#  Questo processo discende da un legopc? Si risale la catena dei padri in
#  /proc fino a init.
#
#  Serve a draw2gr: la HMI che legopc apre dal suo "HMI & Plot" (watchtrends,
#  subwgs.tcl) non deve poter aprire un secondo legopc. watchtrends non passa
#  -edit, quindi il menu non c'e' comunque; questo controllo copre chi
#  aggiungesse -edit a quel lancio.
proc lanciato_da_legopc {} {
    set p [pid]
    for {set n 0} {$n < 64} {incr n} {
        if {[catch {open /proc/$p/stat r} fp]} { return 0 }
        set stat [read $fp]
        close $fp
        # "pid (comm) stato ppid ...": comm puo' contenere spazi e parentesi,
        # quindi si riparte dall'ULTIMA ')'
        set resto [string range $stat [expr {[string last ")" $stat] + 2}] end]
        set p [lindex $resto 1]
        if {![string is integer -strict $p] || $p <= 1} { return 0 }
        if {[esegue_legopc $p]} { return 1 }
    }
    return 0
}

#  Lanci partiti da questo processo e non ancora visti girare, indicizzati con
#  la dir della task normalizzata. Fra l'exec e la comparsa di legopc c'e' un
#  intervallo in cui legopc_aperto_su non lo trova ancora: senza questo, un
#  secondo clic aprirebbe un secondo CAD. Lo svuota sorveglia_legopc.
array set LEGOPC_IN_AVVIO {}

#  legopc e' gia' aperto sulla task in $dir? Ritorna {pid cwd} del primo
#  trovato, oppure {}.
#
#  Si guarda la DIRECTORY CORRENTE dei processi legopc, non la loro riga di
#  comando: chi lancia legopc su una task ci fa cd prima (lghmi, draw2gr, lgpc
#  da un terminale), e legopc stesso ci si porta quando apre un modello
#  (apri_modello in legopc.tix). Cosi' si trova anche il legopc partito vuoto
#  che ha poi fatto Open Model sulla task, e che nella riga di comando non ne
#  ha traccia. Il confronto e' per identita' (stessa_directory).
proc legopc_aperto_su {dir} {
    if {[catch {exec pgrep -f legopc.tix} out]} { return {} }
    foreach pid [split [string trim $out] "\n"] {
        set pid [string trim $pid]
        if {$pid eq "" || $pid == [pid] || ![esegue_legopc $pid]} continue
        if {[catch {file readlink /proc/$pid/cwd} cwd]} continue
        if {[stessa_directory $cwd $dir]} { return [list $pid $cwd] }
    }
    return {}
}

#  Apre in legopc il modello della task in $dir, dopo i controlli.
#
#  vuoto = 1 (lghmi): con la simulazione in corso propone di aprire legopc
#            vuoto, che non sta editando la simulazione;
#  vuoto = 0 (draw2gr): rifiuta e basta - dalla HMI si chiede proprio questa
#            task, e un CAD vuoto non e' quel che si voleva.
#
#  Quattro rifiuti, in quest'ordine, e ognuno dice perche':
#
#  1. SIMULAZIONE IN CORSO. Salvare da legopc riscrive .tom e .i5 mentre la
#     task gira: il binario in proc/ e il layout della SHM non corrisponderebbero
#     piu' a quel che e' disegnato, e le HMI draw2gr gia' aperte leggerebbero
#     file che cambiano sotto.
#
#     Attenzione: questo non IMPEDISCE di modificare, impedisce di porgere la
#     task gia' aperta. Da legopc vuoto ci si arriva lo stesso con Open Model,
#     e fuori di qui non c'e' modo di sorvegliarlo.
#
#  2. TASK DI UN'ALTRA AREA. lghmi elenca anche task che non stanno sotto
#     $LG_ENTRY: in modalita' S01 i path del file sono arbitrari, e i bundle FMU
#     hanno le task in <bundle>/task/<nome>. Su Linux il contesto lo fissa solo
#     il profilo (LG_ENTRY e le derivate LG_LIBGRAPH/LG_LIBUT/LG_LIBRARIES):
#     applyUserFromTom di legopc.tix si aspetta <LG_ENTRY>/models/<n>/<n>.tom,
#     che e' il layout Windows, quindi qui non scatta mai e non corregge
#     niente. Aprire la task di un'altra area significherebbe risolverne i
#     blocchi contro il libgraph SBAGLIATO, senza che niente lo dica: si
#     rifiuta, indicando lgswitch.
#
#  3. MODELLO ASSENTE. Nessun ripiego su un altro .tom della directory.
#
#  4. LEGOPC GIA' APERTO SULLA TASK. Due CAD sullo stesso modello si
#     sovrascriverebbero i salvataggi a vicenda, senza che nessuno dei due lo
#     sappia.
proc modifica_task {dir vuoto} {
    global env
    set task [file tail $dir]

    if {![file isdirectory $dir]} {
        tk_messageBox -icon error -title "legopc" -parent . -message \
            "Task directory not found:\n$dir"
        return
    }

    # 1. simulazione in corso
    set vivi [sim_attiva]
    if {[llength $vivi] > 0} {
        set msg "Cannot open '$task' in legopc while the simulation is running.\n\n"
        append msg "Running: [join $vivi ", "].\n\n"
        append msg "Saving would rewrite the model and its .i5 underneath the task\n"
        append msg "that is running: the executable in proc/ and the shared memory\n"
        append msg "would no longer match what is drawn, and the HMIs already open\n"
        append msg "would read files changing under them.\n\n"
        append msg "Stop the simulation first (Simulator Shutdown in the Master Menu\n"
        append msg "of the desk), then edit."
        if {!$vuoto} {
            tk_messageBox -icon warning -title "legopc" -parent . -message $msg
            legopc_evento stato "legopc: not opened - a simulation is running."
            return
        }
        append msg "\n\nOpen legopc without a model instead?"
        if {[tk_messageBox -icon warning -type yesno -default no -parent . \
                 -title "legopc" -message $msg] eq "yes"} {
            avvia_legopc "" ""
        } else {
            legopc_evento stato "legopc: not opened - a simulation is running."
        }
        return
    }

    # 2. la task deve appartenere all'area corrente
    set area [area_della_task $dir]
    set entry [expr {[info exists env(LG_ENTRY)] ? $env(LG_ENTRY) : ""}]
    if {$entry eq "" || ![file isdirectory $entry]} {
        set app [file rootname [file tail $::argv0]]
        tk_messageBox -icon error -title "legopc" -parent . -message \
            "LG_ENTRY is not set, or is not a directory:\n\n    [expr {$entry eq "" ? "(not set)" : $entry}]\n\nStart $app from a LegoPST environment (profile sourced)."
        return
    }
    if {![stessa_directory $area $entry]} {
        set msg "'$task' does not belong to the current work area.\n\n"
        append msg "    task area:    $area\n"
        append msg "    LG_ENTRY:     [file normalize $entry]\n\n"
        append msg "These are different directories, not two spellings of the same one.\n\n"
        append msg "legopc would open the model but resolve its blocks against the\n"
        append msg "module library of LG_ENTRY, which is a different area: the model\n"
        append msg "would open and be wrong, with nothing saying so.\n\n"
        append msg "Switch work area first (lgswitch), then reopen lghmi."
        tk_messageBox -icon error -title "legopc" -parent . -message $msg
        legopc_evento stato "legopc: '$task' belongs to another work area."
        return
    }
    if {![file exists [file join $area libgraph connect.dat]]} {
        tk_messageBox -icon error -title "legopc" -parent . -message \
            "The work area has no libgraph/connect.dat:\n\n    $area\n\nlegopc cannot resolve the module library from here."
        return
    }

    # 3. il modello, che e' uno solo e omonimo della directory.
    #    Due assenze diverse, che non vanno confuse: una task di REGOLAZIONE non
    #    ha alcun .tom - si costruisce dai .sed/.dxf e non si apre in legopc -
    #    mentre una task di processo senza il .tom omonimo e' un dato anomalo.
    set tom [tom_della_task $dir]
    if {![file isfile $tom]} {
        set altri [glob -nocomplain -directory $dir *.tom]
        if {[llength $altri] == 0} {
            tk_messageBox -icon info -title "legopc" -parent . -message \
                "'$task' has no legopc model.\n\nThere is no .tom in\n    $dir\n\nRegulation tasks are built from their .sed/.dxf files, not from a\nmodel: they are not edited with legopc."
            legopc_evento stato "legopc: '$task' has no .tom model."
        } else {
            set elenco ""
            foreach f [lsort $altri] { append elenco "    [file tail $f]\n" }
            tk_messageBox -icon error -title "legopc" -parent . -message \
                "The model of '$task' is missing.\n\nExpected:\n    [file tail $tom]\n\nFound instead:\n$elenco\nThe model of a task is the .tom named after its directory; the\nothers are not alternatives. Fix the naming in\n    $dir"
            legopc_evento stato "legopc: '$task' has no .tom named after the directory."
        }
        return
    }

    # 4. legopc gia' aperto sulla task, o appena lanciato da qui
    set aperto [legopc_aperto_su $dir]
    if {[llength $aperto] || [info exists ::LEGOPC_IN_AVVIO([file normalize $dir])]} {
        set msg "legopc is already open on '$task'.\n\n"
        if {[llength $aperto]} {
            lassign $aperto pid cwd
            append msg "    pid:          $pid\n"
            append msg "    directory:    $cwd\n\n"
        } else {
            append msg "It was started from here a moment ago and is still coming up.\n\n"
        }
        append msg "Two editors on the same model would overwrite each other's saves.\n"
        append msg "Use the legopc window that is already open, or close it first."
        tk_messageBox -icon warning -title "legopc" -parent . -message $msg
        legopc_evento stato "legopc: already open on '$task'."
        return
    }

    avvia_legopc $dir [file tail $tom]
}

#  Lancio vero e proprio. Processo INDIPENDENTE come per la HMI: `setsid` lo
#  mette in una nuova sessione, cosi' chiudere chi l'ha lanciato non porta via
#  il CAD con il lavoro non salvato dentro. L'output va in un log in /tmp per
#  non sporcare la directory della task.
#
#  legopc.tix tiene del suo argomento SOLO il basename (file tail), e lo risolve
#  sulla directory corrente: per questo si fa cd nella task e si passa il nome
#  nudo, esattamente come lghmi fa per draw2gr.
proc avvia_legopc {dir tom} {
    set lpc [file join $::env(LG_TIX) legopc.tix]
    if {$dir eq ""} {
        set etichetta "legopc"
        set log [file join /tmp "lghmi_legopc.log"]
        set sh "exec wish [list $lpc] >[list $log] 2>&1"
    } else {
        set etichetta "legopc on '[file tail $dir]'"
        set log [file join /tmp "lghmi_legopc_[file tail $dir].log"]
        set sh "cd [list $dir] && exec wish [list $lpc] [list $tom] >[list $log] 2>&1"
    }
    if {[catch {exec setsid sh -c $sh &} err]} {
        if {[catch {exec sh -c $sh &} err2]} {
            tk_messageBox -icon error -title "legopc" -parent . \
                -message "Cannot launch legopc:\n$err2"
            return
        }
    }
    legopc_evento stato "$etichetta started  (log: $log)"
    if {$dir ne ""} {
        set ::LEGOPC_IN_AVVIO([file normalize $dir]) 1
        sorveglia_legopc $dir $tom 0 0
    }
}

#  Quando legopc si chiude, ricordare che la configurazione va riallineata.
#  Il modello puo' essere cambiato, e finche' non si rifa' kUpSim la
#  simulazione userebbe la vecchia configurazione.
#
#  Non c'e' un PID da attendere: il lancio passa per setsid, che puo' forkare, e
#  l'eventuale figlio non e' raggiungibile da qui. Si guarda percio' il processo
#  con pgrep, filtrando sul nome del .tom: e' univoco, perche' il modello di
#  una task e' uno solo e porta il nome della directory. Dei processi trovati
#  si tengono solo i legopc veri (esegue_legopc): pgrep -f trova anche una
#  shell la cui riga di comando nomina legopc.tix e la task, e la scambiava
#  per un legopc che non si chiudeva mai.
#  "visto" evita il falso allarme fra il lancio e la comparsa del processo;
#  dopo 20 tentativi a vuoto (10 s) si rinuncia in silenzio, senza restare
#  appesi per sempre se il lancio e' fallito. In entrambi i casi il lancio non
#  e' piu' "in avvio": da li' in poi la doppia apertura la vede
#  legopc_aperto_su.
proc sorveglia_legopc {dir tom visto tentativi} {
    set chiave [file normalize $dir]
    set vivo 0
    if {![catch {exec pgrep -f "legopc.tix.*[file rootname $tom]"} out]} {
        foreach pid [split [string trim $out] "\n"] {
            if {[esegue_legopc [string trim $pid]]} { set vivo 1 ; break }
        }
    }
    if {$vivo} {
        unset -nocomplain ::LEGOPC_IN_AVVIO($chiave)
        after 2000 [list sorveglia_legopc $dir $tom 1 0]
        return
    }
    if {!$visto} {
        if {$tentativi >= 20} {
            unset -nocomplain ::LEGOPC_IN_AVVIO($chiave)
            return
        }
        after 500 [list sorveglia_legopc $dir $tom 0 [expr {$tentativi + 1}]]
        return
    }
    legopc_evento chiuso [file tail $dir]
}
