#!/bin/ksh
#
# kCompStaz - compila i faceplate di comando: r01.dat -> r02.dat (per xstaz).
#
# Esiste per un motivo solo: dare un exit status utilizzabile. compstaz termina
# con exit(puts(...)), cioe' restituisce il NUMERO DI CARATTERI stampati - 24
# quando e' andato bene, 42 quando fallisce - quindi in una catena `&&` blocca
# tutto anche se e' andato tutto liscio, e non distingue il successo dall'errore.
# Qui l'esito si ricava dall'output, che e' l'unico segnale affidabile.
#
# Va lanciato dopo net_compi/kNetCompi: e' quello a rigenerare variabili.rtf, e
# compstaz risolve gli indici delle variabili proprio contro quel file. Se le
# task vengono ricompilate e i faceplate no, gli indici in r02.dat possono non
# corrispondere piu' e le stazioni leggerebbero punti sbagliati.
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
exit 1
fi
kpresentation
###############################################################################
#	VERIFICATION END
###############################################################################
if [ -f $KLOG/kCompStaz.log ]
then
mv $KLOG/kCompStaz.log $KLOG/kCompStaz.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kCompStaz.log
kAddScreen kCompStaz Start
kAddLog kCompStaz Start
#
cd $KSIM
#
#  Nessun r01.dat = questo simulatore non ha faceplate: non e' un errore.
#
if [ ! -f $KSIM/r01.dat ]
then
print "\nNessun r01.dat in $KSIM : nessun faceplate da compilare\n"
print "\nNessun r01.dat in $KSIM : nessun faceplate da compilare\n" >> $KLOG/kCompStaz.log
kAddScreen kCompStaz End
kAddLog kCompStaz End
exit 0
fi
#
#  compstaz risolve le variabili contro variabili.rtf: senza, si ferma subito.
#
if [ ! -f $KSIM/variabili.rtf ]
then
print "\nERROR\t: variabili.rtf non presente in $KSIM"
print "SOLUTION: lanciare prima kNetCompi (o net_compi), che lo genera\n"
print "\nERROR : variabili.rtf non presente in $KSIM" >> $KLOG/kCompStaz.log
banner NOK
print "\a"
kAddScreen kCompStaz End
kAddLog kCompStaz End
exit 1
fi
#
kAddScreen kCompStaz "Compilazione r01.dat -> r02.dat ..."
kAddLog kCompStaz "Compilazione r01.dat -> r02.dat"
USCITA=$KSIM/tmp/kCompStaz.$$
compstaz | tee $USCITA
cat $USCITA >> $KLOG/kCompStaz.log
#
#  L'esito sta nell'output, non in $? : vedi l'intestazione.
#
if grep -q 'Fine corretta' $USCITA
then
    rm -f $USCITA
    echo ${star3}
    print "\nr02.dat rigenerato in $KSIM\n"
    print "\nr02.dat rigenerato in $KSIM\n" >> $KLOG/kCompStaz.log
    kAddScreen kCompStaz End
    kAddLog kCompStaz End
    echo "$star"
    echo "$star" >> $KLOG/kCompStaz.log
    print "Log file\t: $KLOG/kCompStaz.log"
    exit 0
else
    rm -f $USCITA
    echo ${star3}
    print "\nERROR\t: compilazione dei faceplate FALLITA"
    print "SOLUTION: il dettaglio e' in $KSIM/compstaz.log e in $KLOG/kCompStaz.log\n"
    print "\nERROR : compilazione dei faceplate FALLITA" >> $KLOG/kCompStaz.log
    banner NOK
    print "\a"
    kAddScreen kCompStaz End
    kAddLog kCompStaz End
    print "Log file\t: $KLOG/kCompStaz.log"
    exit 1
fi
