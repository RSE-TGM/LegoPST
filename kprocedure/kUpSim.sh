#!/bin/ksh
#
# kUpSim - riallinea tutta la configurazione del simulatore corrente dopo una
#          modifica a modelli, schemi o faceplate.
#
# Orchestra i comandi che finora si mettevano in fila a mano (l'alias lgupsim
# in Alg_env.sh), in quest'ordine:
#
#   kConnex      topologia e connessioni fra task        -> S01
#   kNetCompi    compilazione delle task                 -> variabili.rtf
#   kCompStaz    faceplate per xstaz                     -> r02.dat
#   kStazPages   gli stessi faceplate come pagine MMI    -> $KWIN/O_*.pag
#   kWinContext  Context.ctx di $KWIN
#   kCompileSim  compila le pagine di $KWIN              -> .rtf
#   kCollect     raccolta in globpages + kMmiConfig
#
# L'ordine non e' arbitrario: kNetCompi riscrive variabili.rtf e kCompStaz vi
# risolve contro gli indici delle variabili, quindi i faceplate vanno ricompilati
# DOPO le task. Ricompilare le une senza gli altri lascia un r02.dat con indici
# che possono non corrispondere piu', senza alcun messaggio d'errore.
#
# Uso:
#   kUpSim            catena completa
#   kUpSim -nommi     salta i tre passi delle pagine MMI dei faceplate
#   kUpSim -n         mostra i passi senza eseguirli
#   kUpSim -h         aiuto
#
# Nota: i comandi vanno invocati in CamelCase. Le versioni minuscole (kconnex,
# kcollect...) sono wrapper che lanciano in background: ritornano subito, quindi
# non si potrebbe sapere ne' quando hanno finito ne' come e' andata.
#
###############################################################################
#	OPZIONI
###############################################################################
CONMMI=1
DRYRUN=0
for arg in $*
do
    case $arg in
        -nommi|--nommi) CONMMI=0 ;;
        -n|--dry-run)   DRYRUN=1 ;;
        -h|--help)
            print "\nkUpSim - riallinea la configurazione del simulatore corrente\n"
            print "\tkUpSim\t\tcatena completa"
            print "\tkUpSim -nommi\tsenza le pagine MMI dei faceplate"
            print "\tkUpSim -n\tmostra i passi senza eseguirli\n"
            exit 0 ;;
        *)  print "\nkUpSim: opzione non riconosciuta [$arg] - vedi kUpSim -h\n"
            exit 1 ;;
    esac
done
###############################################################################
#	VERIFICATION START
###############################################################################
if [ $DRYRUN -eq 0 ]
then
clear
${KBIN}/kTest
KTEST=`cat $KSTATUS/kTest.status`
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit 1
fi
kpresentation
fi
###############################################################################
#	VERIFICATION END
###############################################################################
if [ -f $KLOG/kUpSim.log ]
then
mv $KLOG/kUpSim.log $KLOG/kUpSim.log.kold
fi
echo ${star} >> $KLOG/kUpSim.log
kAddScreen kUpSim Start
kAddLog kUpSim Start
#
cd $KSIM
PASSO=0
FALLITO=""
#
#  Esegue un passo e si ferma al primo che fallisce, dicendo QUALE.
#  Attenzione: quasi tutte le kprocedure terminano con una print e restituiscono
#  0 comunque; l'esito vero lo danno kCompStaz e kStazPages, scritte apposta. Per
#  gli altri passi si controlla che il file atteso sia stato prodotto.
#
Passo()
{
    nome=$1
    shift
    let PASSO=PASSO+1
    print "\n${star3}"
    print "  [$PASSO] $nome"
    print "${star3}\n"
    print "[$PASSO] $nome" >> $KLOG/kUpSim.log
    if [ $DRYRUN -eq 1 ]
    then
        print "        (dry run: $*)"
        return 0
    fi
    "$@"
    esito=$?
    if [ $esito -ne 0 ]
    then
        FALLITO="$nome"
        print "\nERROR\t: il passo $nome e' fallito (exit $esito)"
        print "\nERROR : il passo $nome e' fallito (exit $esito)" >> $KLOG/kUpSim.log
        return 1
    fi
    return 0
}
#
#  Verifica che il passo abbia davvero prodotto il suo file, e che sia fresco.
#
Prodotto()
{
    nome=$1
    file=$2
    [ $DRYRUN -eq 1 ] && return 0
    if [ ! -f "$file" ]
    then
        print "WARNING : $nome non ha prodotto $file"
        print "WARNING : $nome non ha prodotto $file" >> $KLOG/kUpSim.log
        return 1
    fi
    if [ "$file" -ot $KSIM/tmp/kUpSim.avvio.$$ ]
    then
        print "WARNING : $file non e' stato riscritto da $nome"
        print "WARNING : $file non e' stato riscritto da $nome" >> $KLOG/kUpSim.log
    fi
    return 0
}
#
[ $DRYRUN -eq 0 ] && mkdir -p $KSIM/tmp && touch $KSIM/tmp/kUpSim.avvio.$$
#
Passo "kConnex     - topologia e connessioni"   kConnex     || FALLITO="kConnex"
[ -z "$FALLITO" ] && Prodotto kConnex $KSIM/S01
#
if [ -z "$FALLITO" ]
then
Passo "kNetCompi   - compilazione delle task"   kNetCompi   || FALLITO="kNetCompi"
[ -z "$FALLITO" ] && Prodotto kNetCompi $KSIM/variabili.rtf
fi
#
if [ -z "$FALLITO" ]
then
Passo "kCompStaz   - faceplate per xstaz"       kCompStaz   || FALLITO="kCompStaz"
fi
#
if [ -z "$FALLITO" -a $CONMMI -eq 1 ]
then
Passo "kStazPages  - faceplate come pagine MMI" kStazPages  || FALLITO="kStazPages"
fi
if [ -z "$FALLITO" -a $CONMMI -eq 1 ]
then
Passo "kWinContext - Context.ctx di o_win"      kWinContext || FALLITO="kWinContext"
fi
if [ -z "$FALLITO" -a $CONMMI -eq 1 ]
then
Passo "kCompileSim - compila le pagine di o_win" kCompileSim || FALLITO="kCompileSim"
fi
#
if [ -z "$FALLITO" ]
then
Passo "kCollect    - raccolta in globpages"     kCollect    || FALLITO="kCollect"
fi
#
[ $DRYRUN -eq 0 ] && rm -f $KSIM/tmp/kUpSim.avvio.$$
#
###############################################################################
#	ESITO
###############################################################################
print "\n${star}"
print "${star}" >> $KLOG/kUpSim.log
if [ -n "$FALLITO" ]
then
    print "\nCONFIGURAZIONE NON AGGIORNATA : fermo al passo $FALLITO\n"
    print "\nCONFIGURAZIONE NON AGGIORNATA : fermo al passo $FALLITO" >> $KLOG/kUpSim.log
    banner NOK
    print "\a"
    kAddScreen kUpSim End
    kAddLog kUpSim End
    print "Log file\t: $KLOG/kUpSim.log"
    exit 1
fi
if [ $CONMMI -eq 1 ]
then
    print "\nConfigurazione aggiornata: S01, task, faceplate (xstaz e MMI), globpages\n"
    print "\nConfigurazione aggiornata: S01, task, faceplate (xstaz e MMI), globpages" >> $KLOG/kUpSim.log
else
    print "\nConfigurazione aggiornata: S01, task, faceplate (xstaz), globpages\n"
    print "\nConfigurazione aggiornata: S01, task, faceplate (xstaz), globpages" >> $KLOG/kUpSim.log
fi
kAddScreen kUpSim End
kAddLog kUpSim End
print "Log file\t: $KLOG/kUpSim.log"
exit 0
