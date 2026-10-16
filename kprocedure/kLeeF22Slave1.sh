#!/bin/ksh
#
###############################################################################
#	VERIFICATION START
###############################################################################
${KBIN}/kTest
KTEST=`cat $KSTATUS/kTest.status `
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit
fi
#
#kpresentation
if [ -f $KLOG/kLeeF22.log ]
then
mv $KLOG/kLeeF22.log $KLOG/kLeeF22.log.kold
fi
#
###############################################################################
#	VERIFICATION END
###############################################################################
echo ${star}
echo ${star} >> $KLOG/kLeeF22.log
kAddScreen kLeeF22 Start
kAddLog kLeeF22 Start
kAddStatus kLeeF22 Start
kAddLog kLogger "Start kLeeF22"
kAddStatistic kLeeF22
print "$star\n"
print "$star\n" >> $KLOG/kLeeF22.log
###############################################################################
#
#  Prima qui si lanciava $CBIN/LeeF22, che leggeva il vecchio f22.dat con i
#  parametri di LeeF22.in e non si compilava piu'. Il file delle registrazioni
#  e' ora f22circ.dat (circolare): lo esporta f22exp, in CSV.
cd $KSIM
if [ ! -f f22circ.dat ]
then
print "ERROR\t: in $KSIM non c'e' f22circ.dat (la simulazione non ha ancora registrato)\a"
print "ERROR : manca $KSIM/f22circ.dat" >> $KLOG/kLeeF22.log
kAddStatus kLeeF22 NOK
sleep 10
exit 1
fi
f22exp -l f22circ.dat | head -20
print ""
if f22exp -o f22circ.csv f22circ.dat >> $KLOG/kLeeF22.log 2>&1
then
print "\a"
else
print "ERROR\t: f22exp non e' riuscito a esportare f22circ.dat: vedi $KLOG/kLeeF22.log\a"
kAddStatus kLeeF22 NOK
sleep 10
exit 1
fi
#
###############################################################################
echo "$star"
echo "$star" >> $KLOG/kLeeF22.log
kAddScreen kLeeF22 End
kAddLog kLeeF22 End
kAddStatus kLeeF22 End
echo "$star"
echo "$star" >> $KLOG/kLeeF22.log
print "Log File\t: $KLOG/kLeeF22.log"
print "Output File\t: $KSIM/f22circ.csv\n"
sleep 10
#
