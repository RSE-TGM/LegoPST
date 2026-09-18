# lgstaz.tcl - faceplate di comando (xstaz): pagine di r02.dat e apertura.
#
# Sorgiato da:
#   lghmi.tcl     sempre: la lista dei faceplate e il suo doppio clic;
#   hmielem.tcl   i bottoni faceplate (@stz_0) delle pagine di legopc e
#                 draw2gr.
#
# Una sola implementazione di "come si apre una pagina": xstaz si avvia
# ICONIFICATO nella directory di r02.dat e aspetta le richieste sulla coda
# SHR_USR_KEY + ID_MSG_STAZ; stazpag <pagina> gliene manda una. Vedi
# Alg_rt/grafica/xstaz/README.md.
#
# Nessuna proc di questo file apre dialoghi: ritornano un esito, e il
# chiamante decide come mostrarlo (lghmi con dialoghi, le pagine con un
# fumetto, perche' senza simulazione non devono disturbare).

#  Il programma <nome> dell'installazione corrente: $LEGORT_BIN/<nome> se c'e',
#  altrimenti il nome nudo, cercato nel PATH.
proc staz_cmd {nome} {
    if {[info exists ::env(LEGORT_BIN)]} {
        set f [file join $::env(LEGORT_BIN) $nome]
        if {[file executable $f]} { return $f }
    }
    return $nome
}

#  I programmi dei faceplate ci sono? xstaz e stazpag vanno insieme.
proc staz_disponibile {} {
    foreach p {xstaz stazpag} {
        set c [staz_cmd $p]
        if {![file executable $c] && [auto_execok $c] eq ""} { return 0 }
    }
    return 1
}

#  La simulazione gira? Basta net_sked. Si usa ps -A, non ps -a: la
#  simulazione un terminale puo' non averlo (vedi
#  Alg_rt/net_simula/viewval/README.md).
proc staz_sim_viva {} {
    if {[catch {exec ps -A -o comm} out]} { return 0 }
    foreach riga [split $out "\n"] {
        if {[string trim $riga] eq "net_sked"} { return 1 }
    }
    return 0
}

#  Due path indicano la stessa directory? Per identita' (device e inode), non
#  per nome: i link ~/legocad e ~/sked danno due grafie della stessa directory.
proc staz_stessa_dir {a b} {
    if {[catch {file stat $a sa}]} { return 0 }
    if {[catch {file stat $b sb}]} { return 0 }
    return [expr {$sa(dev) == $sb(dev) && $sa(ino) == $sb(ino)}]
}

# --- Parsing S01 --------------------------------------------------------
# Il file S01 e' diviso in sezioni separate da righe che iniziano con '****'
# (quattro asterischi a partire dalla prima colonna):
#   sez.1  nome simulatore + descrizione (una riga)
#   sez.2  una riga per task: nome + descrizione
#   sez.3  una riga per task (associazione posizionale con la sez.2):
#          path relativo <tab/spazi> lettera tipo (P=processo, R=regolazione)
# Ritorna la lista {name desc dir} delle sole task di PROCESSO (P), con il
# path risolto in assoluto rispetto alla directory del file S01.
proc parse_s01 {path {tipi P}} {
    global s01_name s01_desc
    set s01dir [file dirname $path]
    if {[catch {open $path r} fh]} { return {} }
    set data [read $fh]
    close $fh

    # Suddividi in sezioni sui separatori '****' in colonna 1.
    set sec 0
    array set S {}
    foreach line [split $data "\n"] {
        if {[string range $line 0 3] eq "****"} { incr sec; continue }
        lappend S($sec) $line
    }

    # sez.1: prima riga non vuota = nome + descrizione simulatore.
    if {[info exists S(1)]} {
        foreach l $S(1) {
            if {[string trim $l] eq ""} continue
            if {[regexp {^(\S+)\s*(.*)$} $l -> nm ds]} {
                set s01_name $nm
                set s01_desc [string trim $ds]
            }
            break
        }
    }

    # sez.2: nome + descrizione di ogni task.
    set names {}
    if {[info exists S(2)]} {
        foreach l $S(2) {
            if {[string trim $l] eq ""} continue
            if {[regexp {^(\S+)\s*(.*)$} $l -> nm ds]} {
                lappend names [list $nm [string trim $ds]]
            }
        }
    }

    # sez.3: path relativo + tipo (P/R) di ogni task.
    set paths {}
    if {[info exists S(3)]} {
        foreach l $S(3) {
            if {[string trim $l] eq ""} continue
            if {[regexp {^(\S+)\s+(\S+)} $l -> rp tp]} {
                lappend paths [list $rp $tp]
            }
        }
    }

    # Associazione posizionale sez.2 <-> sez.3; tieni solo il tipo P.
    set out {}
    set n [expr {min([llength $names], [llength $paths])}]
    for {set i 0} {$i < $n} {incr i} {
        lassign [lindex $names $i] name desc
        lassign [lindex $paths $i] relpath tipo
        #  In modalita' faceplate servono anche le task di REGOLAZIONE (R):
        #  r01.dat/r02.dat vivono li', non nelle task di processo.
        set tenere 0
        foreach t $tipi { if {[string equal -nocase $tipo $t]} { set tenere 1 } }
        if {!$tenere} continue
        set dir [file normalize [file join $s01dir $relpath]]
        lappend out [list $name $desc $dir]
    }
    return $out
}

#  Pagine definite in <dir>/r02.dat. La lettura la fa 'stazpag -m', che conosce
#  il formato binario: qui non si reimplementa il layout delle strutture.
#  Ritorna una lista di {nome descrizione num_stazioni}.
proc pagine_di {dir} {
    if {![file exists [file join $dir r02.dat]]} { return {} }
    set old [pwd]
    if {[catch {cd $dir}]} { return {} }
    set rc [catch {exec [staz_cmd stazpag] -m 2>@1} out]
    cd $old
    if {$rc} { return {} }
    set res {}
    foreach riga [split $out "\n"] {
        if {[string trim $riga] eq ""} continue
        set campi [split $riga "|"]
        if {[llength $campi] < 3} continue
        lappend res [list [lindex $campi 0] [lindex $campi 1] [lindex $campi 2]]
    }
    return $res
}

#  xstaz dell'utente gia' in esecuzione? Ritorna {pid cwd}, oppure {}.
#  Quelli di altri utenti non contano: la coda e' legata a SHR_USR_KEY.
proc xstaz_attivo {} {
    if {[catch {exec pgrep -x -u $::tcl_platform(user) xstaz} out]} { return {} }
    set pid [lindex [split [string trim $out]] 0]
    if {$pid eq ""} { return {} }
    set cwd ""
    catch {set cwd [file readlink /proc/$pid/cwd]}
    return [list $pid $cwd]
}

#  Le directory con un r02.dat da cui una pagina puo' venire, in ordine:
#   1. le <radici>, se hanno r02.dat (la simulazione in corso, il simulatore
#      corrente...);
#   2. per le radici con un file S01, le task del simulatore, REGOLAZIONE
#      COMPRESA (r01.dat/r02.dat vivono li');
#   3. le sottodirectory di <area> (le task dell'area di lavoro).
#  E' lo stesso ordine di lghmi (dirs_con_r02), generalizzato alle pagine,
#  che non hanno una "cwd di lancio" ma una simulazione e un modello.
proc staz_dirs {radici {area ""}} {
    set out {}
    set viste {}
    set aggiungi {d {
        upvar out out viste viste
        if {![file exists [file join $d r02.dat]]} return
        foreach v $viste { if {[staz_stessa_dir $v $d]} return }
        lappend viste $d
        lappend out $d
    }}
    foreach r $radici {
        if {$r eq "" || ![file isdirectory $r]} continue
        apply $aggiungi $r
        set s01 [file join $r S01]
        if {[file isfile $s01]} {
            foreach e [parse_s01 $s01 {P R}] {
                apply $aggiungi [lindex $e 2]
            }
        }
    }
    if {$area ne "" && [file isdirectory $area]} {
        foreach d [lsort [glob -nocomplain -type d [file join $area *]]] {
            apply $aggiungi $d
        }
    }
    return $out
}

#  La prima directory, fra <dirs>, il cui r02.dat definisce la pagina <nome>.
#  "" se nessuna.
proc staz_dir_di_pagina {dirs nome} {
    foreach d $dirs {
        foreach pg [pagine_di $d] {
            if {[string equal -nocase [lindex $pg 0] $nome]} { return $d }
        }
    }
    return ""
}

#  Apre la pagina <nome> dell'r02.dat in <dir>: avvia xstaz se serve, poi gli
#  manda la richiesta. Ritorna {esito messaggio}:
#    ok       richiesta spedita; il messaggio non e' vuoto se la simulazione
#             non gira (la pagina si apre, ma con i valori fermi)
#    altrove  xstaz gira gia' su un'altra directory: non se ne lancia un
#             secondo (la coda e' UNA per simulazione, due xstaz si
#             ruberebbero i messaggi)
#    errore   directory inaccessibile, lancio o richiesta falliti
#  L'output di xstaz va in <log>, lo stesso file che lghmi mostra in
#  File -> Logs.
#
#  La simulazione NON serve: si aprono le pagine anche a simulazione ferma,
#  per costruire e configurare le stazioni. Senza simulazione xstaz non trova
#  il DB punti (RtCreateDbPunti ritorna NULL: la chiave dell'header non c'e')
#  e va avanti lo stesso, e la coda delle richieste la crea lui
#  (msg_create_fam), cosi' stazpag la trova. Un xstaz avviato cosi' non si
#  aggancia piu' alla simulazione, ma non ci arriva: net_startup, net_simula e
#  simula cominciano con killsim, che lo chiude.
proc staz_apri {dir nome {log /tmp/lghmi_xstaz.log}} {
    #  xstaz legge SHR_USR_KEY con atoi(getenv(...)) senza controllarla: senza
    #  la variabile andrebbe in crash.
    if {![info exists ::env(SHR_USR_KEY)] || $::env(SHR_USR_KEY) eq ""} {
        return [list errore "SHR_USR_KEY is not defined: source the LegoPST profile."]
    }
    set nota ""
    if {![staz_sim_viva]} {
        set nota "No simulation running: the values are not live."
    }
    set avviato 0
    set attivo [xstaz_attivo]
    if {[llength $attivo]} {
        lassign $attivo pid cwd
        if {$cwd ne "" && ![staz_stessa_dir $cwd $dir]} {
            return [list altrove "xstaz is already running (pid $pid) in the directory:\n$cwd\n\nThe request queue is unique per simulation: close that xstaz before opening pages of:\n$dir"]
        }
    } else {
        set old [pwd]
        if {[catch {cd $dir}]} {
            return [list errore "Directory not accessible:\n$dir"]
        }
        #  xstaz parte ICONIFICATO (una finestrella con il solo tasto Quit) e
        #  apre le pagine su richiesta. setsid lo stacca da chi lo lancia.
        set xs [staz_cmd xstaz]
        if {[catch {exec setsid $xs 1 > $log 2>@1 &}]} {
            catch {exec $xs 1 > $log 2>@1 &}
        }
        cd $old
        set avviato 1
        if {![llength [xstaz_attivo]]} {
            after 700
        }
    }

    #  La richiesta resta in coda finche' xstaz non la scoda: nessuna corsa.
    #  Fa eccezione la coda stessa: a simulazione ferma la crea il xstaz appena
    #  lanciato, qualche istante dopo essere partito, e stazpag che arriva prima
    #  non la trova (esce con 5). In quel caso si riprova per al massimo 3 s.
    set old [pwd]
    if {[catch {cd $dir}]} {
        return [list errore "Directory not accessible:\n$dir"]
    }
    set tentativi [expr {$avviato ? 15 : 1}]
    for {set i 0} {$i < $tentativi} {incr i} {
        set rc [catch {exec [staz_cmd stazpag] $nome 2>@1} out opt]
        if {!$rc} break
        set ec [dict get $opt -errorcode]
        if {[lindex $ec 0] ne "CHILDSTATUS" || [lindex $ec 2] != 5} break
        after 200
    }
    cd $old
    if {$rc} {
        return [list errore "Cannot request page '$nome':\n$out"]
    }
    return [list ok $nota]
}
