#!/bin/ksh
#
# kStazPages - porta in $KWIN (o_win) i faceplate descritti nei file r01.dat,
#              come pagine MMI pronte per la catena di raccolta.
#
# Per ogni r01.dat trovato lancia convstaz, che produce una <NOME>.pag e una
# <NOME>.bkg per ogni blocco PAGINA. Le pagine vengono depositate in $KWIN con
# il prefisso O_ : e' quello che kMakeGlobpages cerca (insieme a F_ e M_) per
# collegarle in globpages. Un nome gia' prefissato O_/F_/M_ viene lasciato com'e'.
#
# Dove cerca i r01.dat:
#   - $KSIM                        (il caso normale: r01.dat accanto a variabili.rtf)
#   - le directory delle task R dichiarate in al_sim.conf, risolte via BASEPATH
#
# Dopo questa procedura, nell'ordine:
#   kWinContext           rigenera $KWIN/Context.ctx con le pagine presenti
#   kconfig -c compall    compila i .pag in .rtf
#   kCollect              kMakeGlobpages collega gli O_*.rtf in globpages
#
# NOTA: convstaz esce con il numero di caratteri stampati (exit(puts(...))),
# quindi $? NON dice se e' andata bene: l'esito si controlla guardando se ha
# prodotto delle pagine.
#
###############################################################################
#	VERIFICATION START
###############################################################################
clear
${KBIN}/kTest
KTEST=`cat $KSTATUS/kTest.status`
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit
fi
kpresentation
if [ ! -d "$KWIN" ]
then
print "ERROR\t: Directory $KWIN not found"
print "SOLUTION: creare la directory delle operating window (o_win) del simulatore"
banner NOK
print "\a"
exit
fi
###############################################################################
#	VERIFICATION END
###############################################################################
if [ -f $KLOG/kStazPages.log ]
then
mv $KLOG/kStazPages.log $KLOG/kStazPages.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kStazPages.log
kAddScreen kStazPages Start
kAddLog kStazPages Start
#
# elenco delle directory in cui cercare un r01.dat
#
BASEPATH=`grep '^BASEPATH=' $KSIM/al_sim.conf 2>/dev/null | cut -f2 -d=`
if [ "$BASEPATH" = "" ]
then
BASEPATH=../../legocad
fi
DIRLIST=$KSIM
for task in `grep '^R' $KSIM/al_sim.conf 2>/dev/null | tr -s '\011' ' ' | cut -f3 -d" "`
do
    DIRLIST="$DIRLIST $KSIM/$BASEPATH/$task"
done
#
TMPDIR=$KSIM/tmp/kStazPages.$$
mkdir -p $TMPDIR
npag=0
#
for dir in $DIRLIST
do
    if [ ! -f $dir/r01.dat ]
    then
        continue
    fi
    kAddScreen kStazPages "Conversione `basename $dir`/r01.dat ..."
    kAddLog kStazPages "Conversione $dir/r01.dat"
    rm -f $TMPDIR/*.pag $TMPDIR/*.bkg
    ( cd $dir ; convstaz -d $TMPDIR >> $KLOG/kStazPages.log 2>&1 )
    #  convstaz non restituisce un exit status utilizzabile: si guarda se ha
    #  davvero prodotto delle pagine. Il test sul glob va fatto con ls, non con
    #  [ -f ... ]: senza corrispondenze il pattern resterebbe letterale.
    if ls $TMPDIR/*.pag > /dev/null 2>&1
    then
        :
    else
        print "WARNING : nessuna pagina prodotta da $dir/r01.dat"
        print "WARNING : nessuna pagina prodotta da $dir/r01.dat" >> $KLOG/kStazPages.log
        print "          (dettagli in $dir/convstaz.log)"
        continue
    fi
    for pag in $TMPDIR/*.pag
    do
        nome=`basename $pag .pag`
        case $nome in
            O_*|F_*|M_*) nuovo=$nome ;;
            *)           nuovo=O_$nome ;;
        esac
        cp $pag $KWIN/$nuovo.pag
        if [ -f $TMPDIR/$nome.bkg ]
        then
            cp $TMPDIR/$nome.bkg $KWIN/$nuovo.bkg
        fi
        echo "   $nome  ->  $KWIN/$nuovo.pag"
        echo "   $nome  ->  $KWIN/$nuovo.pag" >> $KLOG/kStazPages.log
        let npag=npag+1
    done
done
#
rm -rf $TMPDIR
echo ${star3}
print "\n${npag} pagine di faceplate depositate in $KWIN\n"
print "\n${npag} pagine di faceplate depositate in $KWIN\n" >> $KLOG/kStazPages.log
if [ $npag -gt 0 ]
then
print "Per portarle nell'MMI, nell'ordine:"
print "\tkWinContext\t\trigenera $KWIN/Context.ctx"
print "\tkconfig -c compall\tcompila i .pag in .rtf"
print "\tkCollect\t\tcollega gli O_*.rtf in globpages\n"
fi
#
kAddScreen kStazPages End
kAddLog kStazPages End
echo "$star"
echo "$star" >> $KLOG/kStazPages.log
print "Log file\t: $KLOG/kStazPages.log"
