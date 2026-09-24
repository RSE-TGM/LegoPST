# lgmkstaz_topologia.tcl - legge variabili.edf per dare a lgmkstaz
# l'autocomplete e la verifica di INPUT/OUTPUT sulle variabili vere del
# simulatore, invece del testo libero della Fase 2. Tcl puro, nessun Tk:
# testabile con tclsh.
#
# variabili.edf e' il dump ASCII che compstaz stesso scrive
# (AlgLib/libsim/var_sh.c::costruisci_var, "crea il file ASCII
# variabili.edf") accanto a variabili.rtf (il dump BINARIO che compstaz
# legge per davvero): non e' un formato inventato per lgmkstaz, e' gia'
# quello che produce la catena di compilazione del simulatore, nella
# directory dove giri compstaz (dove sta anche il r01.dat). Qui si legge
# SOLO l'ASCII: variabili.rtf e' un dump binario di struct C
# (VARIABILI/NOMI_MODELLI/NOMI_BLOCCHI), non c'e' modo pulito di leggerlo da
# Tcl senza rifare il layout esatto delle struct - e non ce n'e' bisogno,
# l'ASCII ha la stessa informazione.
#
# Formato di variabili.edf (righe rilevanti, i BLOCCO/indici non servono a
# lgmkstaz - solo nome variabile + tipo + descrizione, per modello):
#   NOME MODELLO <n> <nome> NUMERO BLOCCHI <n>
#   BLOCCO  <nome>  ... (si ignora, e' solo raggruppamento)
#   <tipo> <NOME_VAR> <indice>  --XX--BL.(<blocco>) <descrizione>
# dove <tipo> e' 0 (USCITA), 1 (INGRESSO LIBERO) o 2 (INGRESSO CONNESSO) -
# la riga di intestazione del file lo dice ("USCITA = 0  INGRESSO LIBERO = 1
# INGRESSO CONNESSO = 2").
#
# Il riferimento al blocco NON e' scritto allo stesso modo in ogni modello:
# le task di PROCESSO (NPS, SSS, ...) lo scrivono incollato, fra parentesi -
# "--UA--BL.(TEMPPSLT) FLUID TEMPERATURE [K]"; le task di REGOLAZIONE
# (r_*, es. R_PCS) lo scrivono staccato da uno spazio, senza "BL." ne'
# parentesi - "--UA-- swmc03 USCITA SWITCH COLLAUDO". Trovato dall'utente
# (2026-09-22): con R_PCS nel simulatore SLaurent_0, la prima versione del
# parser (che riconosceva solo la forma con "BL.(...)") non trovava NESSUNA
# riga valida in quella task - 0 variabili, non un limite della lista di
# scelta come si pensava all'inizio. La regex qui sotto riconosce entrambe
# le forme (il nome del blocco non serve comunque a lgmkstaz, si scarta in
# tutti e due i casi: conta solo dove comincia la descrizione).
#
# Chi legge cosa (Alg_rt/grafica/compstaz/checkvar.c):
#   check_output (usato per le righe INPUT/INPUT_ERR/INPUT_BLINK/
#   INIBIZIONE): la variabile deve essere una USCITA (tipo 0) del modello.
#   check_input  (usato per le righe OUTPUT): la variabile deve essere un
#   INGRESSO LIBERO (tipo 1, "INGRESSO_NC" in sim_types.h) - un ingresso gia'
#   connesso internamente (tipo 2) non si puo' perturbare da fuori,
#   checkvar.c lo rifiuta.
#
# I due modi di "scollegare di proposito" (gia' documentati in
# Alg_rt/grafica/xstaz/HOWTO_faceplate.md, capitolo sulle righe INPUT/OUTPUT):
#   - riga vuota (nessun var/mod): scollegato, indice -1;
#   - un nome che comincia per '#' (var O mod): scollegato ma il nome resta
#     leggibile nel file, per prepararlo prima che la variabile esista
#     (checkvar.c, macro COMMENTATO);
#   - i segnaposto storici var="variabil"/mod="modello": qui si tratta come
#     valido solo var=="variabil" (salta la verifica della variabile,
#     qualunque sia il modello) - mod=="modello" da solo, senza
#     var=="variabil", in compstaz e' quasi sempre un errore vero (il
#     modello si risolve a un indice 0 che nessuna variabile ha), quindi qui
#     NON si tratta come sempre valido.

namespace eval ::lgmkstaz {
    variable topo_modelli {}    ;# nomi modello, nell'ordine del file
    variable topo_percorso ""   ;# variabili.edf caricato adesso, o ""
    array set topo_var {}       ;# topo_var($modello,$nome) = {tipo descrizione}
}

proc ::lgmkstaz::topo_reset {} {
    variable topo_modelli
    variable topo_percorso
    array unset ::lgmkstaz::topo_var
    set topo_modelli {}
    set topo_percorso ""
}

#  Cerca variabili.edf accanto al r01.dat dato (stessa directory - e' li'
#  che lo cercherebbe compstaz stesso, girando in quella directory). Ritorna
#  il percorso o "" se non c'e'.
proc ::lgmkstaz::topo_trova {percorso_r01} {
    set candidato [file join [file dirname $percorso_r01] variabili.edf]
    if {[file exists $candidato]} { return $candidato }
    return ""
}

#  Legge un variabili.edf. Non solleva errore su righe che non riconosce
#  (il file ha anche intestazioni, righe vuote, "BLOCCO ..."): le ignora. Un
#  errore vero (file illeggibile) si propaga a chi chiama.
#  Ritorna il numero di modelli caricati.
proc ::lgmkstaz::topo_carica {percorso} {
    topo_reset

    set ch [open $percorso r]
    fconfigure $ch -encoding binary -translation lf
    set testo [read $ch]
    close $ch

    set modello_corrente ""
    foreach riga [split $testo "\n"] {
        if {[regexp {^NOME MODELLO\s+[0-9]+\s+(\S+)\s+NUMERO BLOCCHI} $riga -> nome]} {
            set modello_corrente $nome
            lappend ::lgmkstaz::topo_modelli $nome
            continue
        }
        if {$modello_corrente eq ""} { continue }
        #  "<tipo> <NOME_VAR> <indice>  --XX--BL.(<blocco>) <descrizione>" (task
        #  di processo) oppure "<tipo> <NOME_VAR> <indice>  --XX-- <blocco>
        #  <descrizione>" (task di regolazione, r_*: senza "BL." ne' parentesi,
        #  col nome del blocco staccato da uno spazio). Il nome del blocco non
        #  serve comunque a lgmkstaz in nessuna delle due forme: si scarta,
        #  conta solo dove comincia la descrizione dopo di esso.
        if {[regexp {^([0-9])\s+(\S+)\s+[0-9]+\s+--\S\S--\s*(?:BL\.\([^)]*\)|\S+)?\s*(.*?)\s*$} \
                 $riga -> tipo nome descrizione]} {
            set ::lgmkstaz::topo_var($modello_corrente,$nome) [list $tipo $descrizione]
        }
    }

    set ::lgmkstaz::topo_percorso $percorso
    return [llength $::lgmkstaz::topo_modelli]
}

#  Le variabili di un modello con quel tipo (0 uscita, 1 ingresso libero, 2
#  ingresso connesso), in ordine alfabetico.
proc ::lgmkstaz::topo_variabili_tipo {modello tipo_voluto} {
    variable topo_var
    set prefisso "$modello,"
    set lp [string length $prefisso]
    set esito {}
    foreach chiave [array names topo_var "$prefisso*"] {
        if {[lindex $topo_var($chiave) 0] == $tipo_voluto} {
            lappend esito [string range $chiave $lp end]
        }
    }
    return [lsort $esito]
}

proc ::lgmkstaz::topo_variabili_uscita {modello} {
    return [topo_variabili_tipo $modello 0]
}

#  Quelle che un campo INPUT puo' citare: tutte quelle del modello, perche'
#  tanto accetta compstaz (vedi topo_esiste_nel_modello). Le uscite vengono
#  per prime: sono il caso normale, e chi cerca trova subito quelle.
proc ::lgmkstaz::topo_variabili_citabili {modello} {
    set uscite [topo_variabili_tipo $modello 0]
    set altre {}
    foreach t {1 2} {
        foreach v [topo_variabili_tipo $modello $t] { lappend altre $v }
    }
    return [concat $uscite [lsort $altre]]
}

proc ::lgmkstaz::topo_variabili_ingresso {modello} {
    return [topo_variabili_tipo $modello 1]
}

proc ::lgmkstaz::topo_descrizione {modello nome} {
    variable topo_var
    if {[info exists topo_var($modello,$nome)]} {
        return [lindex $topo_var($modello,$nome) 1]
    }
    return ""
}

#  ATTENZIONE al nome: compstaz chiama check_output il controllo dei campi
#  INPUT/INPUT_BLINK/INPUT_ERR/INIBIZIONE, ma quella funzione NON guarda il
#  tipo della variabile - scorre le variabili del modello e si ferma alla
#  prima col nome giusto, qualunque tipo abbia (checkvar.c, check_output:
#  l'unico filtro e' "variabili[i].mod != imu"). Un INPUT che cita un
#  INGRESSO del modello quindi compila, e in esecuzione funziona: xstaz legge
#  quell'indirizzo come qualunque altro.
#
#  Serve saperlo perche' il contrario - rifiutarlo qui - vorrebbe dire dare
#  errore su pagine che compilano e girano da anni. Il tipo si usa solo per
#  ORDINARE le proposte del selettore (prima le uscite, che sono il caso
#  normale), non per vietare.
proc ::lgmkstaz::topo_esiste_nel_modello {modello nome} {
    variable topo_var
    return [info exists topo_var($modello,$nome)]
}

proc ::lgmkstaz::topo_e_uscita {modello nome} {
    variable topo_var
    return [expr {[info exists topo_var($modello,$nome)] \
                  && [lindex $topo_var($modello,$nome) 0] == 0}]
}

proc ::lgmkstaz::topo_e_ingresso_libero {modello nome} {
    variable topo_var
    return [expr {[info exists topo_var($modello,$nome)] \
                  && [lindex $topo_var($modello,$nome) 0] == 1}]
}

proc ::lgmkstaz::topo_commentato {nome} {
    return [expr {[string index $nome 0] eq "#"}]
}

#  Verifica un riferimento (var,mod) di una riga: $genere e' "uscita" (per
#  INPUT/INPUT_ERR/INPUT_BLINK/INIBIZIONE) o "ingresso" (per OUTPUT). Torna
#  silenziosamente se e' valido (scollegato, commentato, il segnaposto
#  "variabil", o trovato davvero nella topologia); solleva un errore con un
#  messaggio pronto per l'utente altrimenti. Senza una topologia caricata,
#  non c'e' modo di verificare: si accetta (lgmkstaz si comporta come prima
#  di questa fase, testo libero).
proc ::lgmkstaz::topo_verifica_riferimento {var mod genere} {
    variable topo_percorso
    variable topo_modelli

    if {$var eq "" && $mod eq ""} { return }
    if {[topo_commentato $var] || [topo_commentato $mod]} { return }
    if {$var eq "variabil"} { return }
    if {$topo_percorso eq ""} { return }

    if {$mod eq "" || $mod ni $topo_modelli} {
        error "il modello '$mod' non e' nella topologia caricata\
               ([llength $topo_modelli] modelli: [join [lsort $topo_modelli] {, }])"
    }
    if {$genere eq "uscita"} {
        #  come check_output: basta che la variabile esista nel modello
        if {![topo_esiste_nel_modello $mod $var]} {
            error "'$var' non esiste nel modello '$mod'\
                   (per scriverla prima che esista: #$var)"
        }
    } else {
        if {![topo_e_ingresso_libero $mod $var]} {
            error "'$var' non e' un ingresso libero del modello '$mod'\
                   (assente, o gia' connesso internamente - un ingresso\
                   connesso non si puo' perturbare da fuori; per scriverla\
                   prima che esista: #$var)"
        }
    }
}
