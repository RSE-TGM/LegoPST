#!/bin/ksh
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
if [ -f $KLOG/kMakeCurve.log ]
then
mv $KLOG/kMakeCurve.log $KLOG/kMakeCurve.log.kold
fi
if [ -f $KLOG/kMakeCurve.err ]
then
mv $KLOG/kMakeCurve.err $KLOG/kMakeCurve.err.kold
fi
if [ ! -f $KGRAF/grugraf.txt ]
then
print "ERROR\t: File $KGRAF/grugraf.txt not found"
exit
fi
if [ ! -f $KGRAF/kksgrafi.txt ]
then
print "ERROR\t: File $KGRAF/kksgrafi.txt not found"
exit
fi
if [ ! -f $KGRAF/vargraf.txt ]
then
print "ERROR\t: File $KGRAF/vargraf.txt not found"
exit
fi
if [ ! -f $KSIM/variabili.edf ]
then
print "ERROR\t: File $KSIM/variabili.edf not found"
print "Solution\t: kNetCompi"
exit
fi
###############################################################################
#	Clean
###############################################################################
cd ${KWIN}
echo "Cleaning (start)"
ls | grep ^M_S_ | grep _GR | while read OldGr
do
rm ${OldGr}
done
echo "Cleaning (end)"
###############################################################################
$KBINSLAVE/kMakeCurveSlave0
KTEST=`cat $KSTATUS/kMakeCurveSlave0.status`
echo "kMakeCurveSlave0 result : $KTEST"
echo "kMakeCurveSlave0 result : $KTEST" >> $KLOG/kMakeCurve.log
if [ ! "$KTEST" = "OK" ]
then
print "ACCESS database not correctly configurated\a"
echo "ACCESS database not correctly configurated" >> $KLOG/kMakeCurve.log
banner "NOK"
exit
fi
###############################################################################
#	VERIFICATION END
###############################################################################
Check=$1
echo ${star}
echo ${star} >> $KLOG/kMakeCurve.log
kAddScreen kMakeCurve Start
kAddLog kMakeCurve Start
kAddStatus kMakeCurve Start
kAddLog kLogger "Start kMakeCurve"
kAddStatistic kMakeCurve
echo ${star}
echo ${star} >> $KLOG/kMakeCurve.log
#
$KBINSLAVE/kMakeCurveSlave1 $Check
$KBINSLAVE/kMakeCurveSlave2
#
print "\n$star"
print "\n$star" >> $KLOG/kMakeCurve.log
kAddScreen kMakeCurve End
kAddLog kMakeCurve End
kAddStatus kMakeCurve End
kAddLog kLogger "End kMakeCurve"
echo "$star"
echo "$star" >> $KLOG/kMakeCurve.log
#
print "Log File\t: $KLOG/kMakeCurve.log"
print "Error File\t: $KLOG/kMakeCurve.err\a\n"
