# lgmkstaz_verifica.tcl - "Compila e verifica": lancia compstaz PER DAVVERO
# su una copia scratch del modello corrente, per la validazione vera che le
# regole di lgmkstaz (grammatica, limiti, topologia) non possono sostituire
# del tutto. Fase 4 del piano (vedi memoria di progetto project_lgmkstaz.md).
#
# ATTENZIONE ALLA SHARED MEMORY. costruisci_var() (AlgLib/libsim/var_sh.c),
# chiamata da compstaz appena parte, TOCCA LA SHM ANCHE quando trova
# variabili.rtf gia' pronto (non solo nel ramo "file assente" che si temeva
# all'inizio): prende SHR_USR_KEY dall'ambiente, verifica se esiste gia' una
# shared memory a quella chiave e, se no, la CREA della dimensione di
# variabili.rtf e ce lo carica dentro. Se la chiave scelta fosse quella del
# banco operatore vero (o di una FMU in corso), compstaz si aggancerebbe a
# QUELLA invece di crearne una sua - un rischio vero, non teorico.
#
# Per questo ogni esecuzione:
#   - lavora SEMPRE su una copia scratch (mai il file salvato su disco, mai
#     l'originale): scrive il modello ATTUALE (anche non salvato) in una
#     directory temporanea nuova, con una copia di variabili.rtf preso
#     accanto al variabili.edf gia' caricato (../lgmkstaz_topologia.tcl);
#   - usa una chiave SHR_USR_KEY ISOLATA (verifica_chiave_isolata): un
#     offset largo (50 milioni) fuori da qualunque range che il profilo
#     LegoPST assegnerebbe mai (il banco operatore e' uid*10000, per
#     l'utente di questa macchina 10000000 - vedi Alg_env.sh) sommato al pid
#     di QUESTO processo wish, cosi' anche due lgmkstaz aperti insieme non
#     si pestano i piedi;
#   - all'uscita di compstaz rimuove SOLO quella shared memory
#     (ipcrm -M <chiave>+ID_SHM_VAR), MAI killsim: killsim su Linux cancella
#     TUTTE le SHM dell'utente, senza filtro per chiave (vedi CLAUDE.md,
#     tranelli) - qui sarebbe una scure per un problema di ago;
#   - chiede conferma esplicita prima di ogni esecuzione (lo showbox lo fa
#     lgmkstaz.tcl, non questo file): decisione dell'utente, 2026-09-22.
#
# L'exit status di compstaz NON e' affidabile (Alg_rt/grafica/xstaz/README.md,
# sezione Trappole: "restituisce 24 anche quando va bene" - puts() ritorna i
# caratteri stampati da "Fine corretta COMPSTAZ", non un booleano): l'esito
# si legge cercando quella frase nell'output vero, come fa kCompStaz.
#
# "ANTEPRIMA CON XSTAZ" (dopo una compilazione riuscita): apre per davvero la
# pagina compilata, con la stessa chiave isolata. Non reinventa il lancio di
# xstaz: chiama staz_apri (lgstaz.tcl), la stessa proc che gia' usano lghmi e
# i bottoni faceplate delle pagine, gia' collaudata per aprire pagine ANCHE
# senza una simulazione viva (il caso di qui: xstaz non trova il DB punti,
# mostra la pagina lo stesso, senza valori). staz_apri legge SHR_USR_KEY
# dall'AMBIENTE DEL PROCESSO (non da un parametro): per non usare quella vera
# del profilo (ereditata da questo stesso wish), la si sovrascrive nel
# processo SOLO per la durata della chiamata, poi si ripristina - vedi
# verifica_apri_anteprima.
#
# xstaz, oltre alla shared memory della topologia (ID_SHM_VAR, gia' vista per
# compstaz), crea un'INTERA FAMIGLIA di code di messaggi (msg_create_fam,
# AlgLib/libipc/msg_create_fam.c: banco, monit, sked, pert, prep, snap, leg,
# mandb, aing, staz, ret_aing, buffer) piu' un semaforo (ID_SEM_MSG) - tutte
# alla stessa chiave isolata. verifica_pulisci_chiave le toglie tutte, non
# solo quella di staz: mai lasciare in giro pezzi di IPC di una prova.

#  Il binario compstaz: $LEGOROOT/Alg_rt/bin/compstaz se LEGOROOT e'
#  definito (il caso normale: Alg_rt/bin/lgmkstaz, il lanciatore, lo esporta
#  sempre prima di sorgiare il profilo). Senza LEGOROOT - uno che lancia
#  "wish .../lgmkstaz.tcl" a mano, saltando il lanciatore - si risale da
#  questo script cercando una directory che contenga Alg_rt/bin/compstaz:
#  funziona sia da Alg_legopc/src/tix (tre livelli sotto la radice) sia da
#  Alg_legopc/bin, dove finisce la copia distribuita (due livelli) - senza
#  dover indovinare quanti livelli servono in un caso o nell'altro.
proc ::lgmkstaz::verifica_trova_compstaz {} {
    if {[info exists ::env(LEGOROOT)] && $::env(LEGOROOT) ne ""} {
        set candidato [file join $::env(LEGOROOT) Alg_rt bin compstaz]
        if {[file executable $candidato]} { return $candidato }
    }
    set dir $::lgmkstaz::_qui
    for {set i 0} {$i < 6} {incr i} {
        set candidato [file join $dir Alg_rt bin compstaz]
        if {[file executable $candidato]} { return $candidato }
        set su [file dirname $dir]
        if {$su eq $dir} break
        set dir $su
    }
    return ""
}

#  Una chiave SHR_USR_KEY isolata per questa esecuzione (vedi la nota in
#  testa al file per il perche' di questo intervallo).
proc ::lgmkstaz::verifica_chiave_isolata {} {
    return [expr {50000000 + [pid]}]
}

#  Toglie OGNI IPC che compstaz e xstaz possono aver creato a quella chiave:
#  la shared memory della topologia (ID_SHM_VAR), le code di messaggi di
#  msg_create_fam e il suo semaforo contatore (ID_SEM_MSG) - vedi
#  AlgLib/libinclude/sim_ipc.h per questi numeri. Ognuna si toglie a se',
#  perche' non tutte esistono sempre (senza "Anteprima con xstaz" ci sono
#  solo la SHM di compstaz, le altre non sono mai state create: ipcrm su una
#  chiave inesistente fallisce in silenzio, va bene cosi').
proc ::lgmkstaz::verifica_pulisci_chiave {chiave} {
    catch { exec ipcrm -M [expr {$chiave + 5}] }   ;# ID_SHM_VAR
    foreach offset {3 4 5 6 7 8 9 10 11 12 13 60} { ;# msg_create_fam (ID_MSG_*)
        catch { exec ipcrm -Q [expr {$chiave + $offset}] }
    }
    catch { exec ipcrm -S [expr {$chiave + 10}] }  ;# ID_SEM_MSG
}

#  Prepara una directory scratch nuova con r01.dat (dal modello ATTUALE, non
#  necessariamente salvato su disco) e una copia di variabili.rtf, trovato
#  accanto al variabili.edf gia' caricato da topo_carica. Ritorna il
#  percorso della directory. Solleva un errore chiaro (senza aver creato o
#  toccato nulla di permanente) se manca la topologia o variabili.rtf.
proc ::lgmkstaz::verifica_prepara_scratch {} {
    variable modello
    variable topo_percorso

    if {$topo_percorso eq ""} {
        error "Nessuna topologia caricata (manca variabili.edf accanto al file\
               aperto): senza di essa non si trova nemmeno variabili.rtf, che\
               compstaz vuole per davvero. Apri un file la cui directory ce\
               l'abbia, o rigeneralo (dolgfmu/net_compi)."
    }
    set rtf [file join [file dirname $topo_percorso] variabili.rtf]
    if {![file exists $rtf]} {
        error "variabili.rtf non trovato accanto a\n$topo_percorso\n\
               (compstaz legge quello, non il .edf - sono scritti insieme\
               dalla stessa compilazione del simulatore)."
    }

    set base [expr {
        [info exists ::env(TMPDIR)] && $::env(TMPDIR) ne "" ? $::env(TMPDIR) : "/tmp"
    }]
    set scratch [file join $base "lgmkstaz_verifica_[pid]_[clock clicks]"]
    if {[file exists $scratch]} { set scratch "${scratch}_2" }
    file mkdir $scratch

    if {[catch {file copy -force $rtf [file join $scratch variabili.rtf]} err]} {
        catch { file delete -force $scratch }
        error "impossibile copiare variabili.rtf nella directory scratch: $err"
    }
    if {[catch {scrivi_file [file join $scratch r01.dat] $modello} err]} {
        catch { file delete -force $scratch }
        error "impossibile scrivere r01.dat nella directory scratch: $err"
    }
    return $scratch
}

#  Lancia compstaz nella directory scratch data, con una chiave SHM isolata,
#  e rimuove SOLO quella shared memory alla fine (mai killsim). Ritorna un
#  dict {esito ok|errore output <testo catturato> log <compstaz.log>}.
#  Solleva un errore Tcl solo per un problema di lgmkstaz stesso (compstaz
#  non trovato): un fallimento di COMPILAZIONE e' un esito normale, non
#  un'eccezione - lo dice "esito errore" nel dict, non un throw.
proc ::lgmkstaz::verifica_esegui_compstaz {scratch} {
    set bin [verifica_trova_compstaz]
    if {$bin eq ""} {
        error "compstaz non trovato (ne' in \$LEGOROOT/Alg_rt/bin ne' vicino a lgmkstaz.tcl)."
    }
    set chiave [verifica_chiave_isolata]

    set cwd_precedente [pwd]
    cd $scratch
    if {[catch {
        exec env -i \
            HOME=$::env(HOME) \
            PATH=/usr/bin:/bin \
            LANG=POSIX \
            SHR_USR_KEY=$chiave \
            $bin 2>@1
    } catturato]} {
        #  compstaz esce quasi sempre con un codice diverso da zero (24 in
        #  successo, come detto sopra): per Tcl e' comunque un errore di
        #  exec, e il messaggio DI QUELL'ERRORE e' l'output vero del
        #  comando (stdout+stderr, uniti da 2>@1) - e' la via normale con
        #  cui questo script legge cosa ha detto compstaz, non un problema.
        set output $catturato
    } else {
        set output $catturato
    }
    catch { cd $cwd_precedente }

    #  qualunque sia stato l'esito della compilazione, quello che compstaz
    #  ha creato a questa chiave si toglie subito - "Anteprima con xstaz",
    #  se la si usa dopo, ne crea altra (la famiglia di code, non la SHM
    #  della topologia: compstaz e' gia' finito) e la sua pulizia e' un
    #  passo a parte (verifica_apri_anteprima/verifica_pulisci_chiave).
    verifica_pulisci_chiave $chiave

    set log ""
    set log_path [file join $scratch compstaz.log]
    if {[file exists $log_path]} {
        set ch [open $log_path r]
        fconfigure $ch -encoding binary
        set log [read $ch]
        close $ch
    }

    set esito [expr {
        [string first "Fine corretta COMPSTAZ" "$output\n$log"] >= 0 ? "ok" : "errore"
    }]
    return [dict create esito $esito output $output log $log]
}

# ---------------------------------------------------------------------------
# "Anteprima con xstaz". Richiede lgstaz.tcl gia' sorgiato (staz_apri,
# xstaz_attivo, staz_stessa_dir - lo fa lgmkstaz.tcl).
# ---------------------------------------------------------------------------

#  Apre per davvero $nome_pagina dell'r02.dat in $scratch, con xstaz.
#  staz_apri (lgstaz.tcl) legge SHR_USR_KEY dall'ambiente DEL PROCESSO, non
#  da un parametro: qui lo si sovrascrive con la chiave isolata SOLO per
#  questa chiamata (che lancia xstaz e ritorna subito - xstaz resta acceso
#  per conto suo, in background) e lo si ripristina subito dopo, cosi' il
#  resto di lgmkstaz continua a vedere l'ambiente di sempre.
#  Ritorna {esito messaggio} come staz_apri: "ok" (aperta, il messaggio dice
#  se la simulazione non gira), "altrove" (un altro xstaz e' gia' aperto su
#  un'altra directory: non se ne lancia un secondo), "errore".
proc ::lgmkstaz::verifica_apri_anteprima {scratch nome_pagina chiave} {
    set c_era [info exists ::env(SHR_USR_KEY)]
    if {$c_era} { set precedente $::env(SHR_USR_KEY) }
    set ::env(SHR_USR_KEY) $chiave

    set risultato [staz_apri $scratch $nome_pagina]

    if {$c_era} {
        set ::env(SHR_USR_KEY) $precedente
    } else {
        unset ::env(SHR_USR_KEY)
    }
    return $risultato
}

#  Vero se l'xstaz aperto da verifica_apri_anteprima e' ANCORA su questa
#  directory scratch - serve a non cancellarla (e a non togliere l'IPC che
#  sta ancora usando) mentre la sta mostrando davvero.
proc ::lgmkstaz::verifica_anteprima_attiva {scratch} {
    set attivo [xstaz_attivo]
    if {![llength $attivo]} { return 0 }
    lassign $attivo pid cwd
    return [expr {$cwd ne "" && [staz_stessa_dir $cwd $scratch]}]
}
