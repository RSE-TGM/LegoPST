#!/bin/ksh
###############################################################
#
# creasim - crea un nuovo simulatore vuoto, pronto da configurare.
#
# Uso:  creasim <nome-simulatore>
#
# Colma il buco fra i due strumenti esistenti:
#   - ksetsim <nome>        pretende che $KSKED/<nome> esista GIA'
#   - k_crea_simulatore     riempie la sottostruttura, ma lavora su $KSIM
# creasim crea la directory di primo livello, delega a k_crea_simulatore per
# le sottodirectory (unica fonte della lista, niente duplicati) e installa i
# due file di configurazione partendo dai template del repository.
#
# Cosa NON fa: non compila e non genera l'S01. Dopo creasim vanno adattati
# al_sim.conf (elenco delle task) e, se serve, Simulator (dimensionamenti).
#
###############################################################

PROG=$(basename $0)

if [ -z "$1" ]
then
	print "uso: $PROG <nome-simulatore>"
	print ""
	print "Crea \$KSKED/<nome-simulatore> con la struttura standard e i file"
	print "di configurazione iniziali (al_sim.conf, Simulator)."
	exit 1
fi

#  KSKED (root dei simulatori) e LEGOROOT arrivano dal profilo. Senza profilo
#  sorgiato non possiamo indovinare i percorsi: meglio fermarsi subito.
if [ -z "$KSKED" ]
then
	print "$PROG: KSKED non definita: sorgiare .profile_legoroot" >&2
	exit 2
fi
if [ -z "$LEGOROOT" ]
then
	print "$PROG: LEGOROOT non definita: sorgiare .profile_legoroot" >&2
	exit 2
fi

SIMNAME="$1"
SIMDIR="$KSKED/$SIMNAME"

if [ -d "$SIMDIR" ]
then
	print "$PROG: il simulatore '$SIMNAME' esiste gia': $SIMDIR" >&2
	exit 3
fi

#  Template: stanno nel repository, non in percorsi utente (la vecchia versione
#  cercava $HOME/legocad/libut_bin/al_sim.conf, che non esiste piu').
TPL_CONF="$LEGOROOT/util97/bin/al_sim.conf.example"
TPL_SIMU="$LEGO/procedure/Simulator.tpl"

if [ ! -f "$TPL_CONF" ]
then
	print "$PROG: template non trovato: $TPL_CONF" >&2
	exit 4
fi

mkdir -p "$SIMDIR" || exit 5
print "Creata directory $SIMDIR"

#  Sottostruttura: la conosce k_crea_simulatore (18 directory). Gliela facciamo
#  creare passandogli KSIM, cosi' la lista resta in un posto solo.
KSIM="$SIMDIR" k_crea_simulatore

#  al_sim.conf dal template, senza le righe di README ("rename it ... and delete
#  these rows"), che nel file finale sarebbero solo rumore.
sed '/^# READ ME/,/^#$/d' "$TPL_CONF" > "$SIMDIR/al_sim.conf"
print "Creato   $SIMDIR/al_sim.conf   (da al_sim.conf.example)"

#  Simulator: parametri di dimensionamento. Gli script di avvio lo copierebbero
#  comunque da soli, ma averlo subito lo rende visibile e modificabile.
if [ -f "$TPL_SIMU" ]
then
	cp "$TPL_SIMU" "$SIMDIR/Simulator"
	print "Creato   $SIMDIR/Simulator      (da Simulator.tpl)"
else
	print "$PROG: attenzione, Simulator.tpl non trovato ($TPL_SIMU):"
	print "        il file Simulator sara' creato al primo avvio."
fi

print ""
print "Simulatore '$SIMNAME' creato. Passi successivi:"
print "  1. ksetsim $SIMNAME"
print "  2. adattare $SIMDIR/al_sim.conf"
print "     (TITLE, BASEPATH, MMI_HOSTNAME e l'elenco delle task P/R)"
print "  3. verificare i dimensionamenti in $SIMDIR/Simulator"
exit 0
