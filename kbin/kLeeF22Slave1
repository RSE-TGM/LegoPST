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
cd $KSIM
rm -f LeeF22.out
if [ ! -f LeeF22.in ]
then
cp $CBIN/LeeF22.in .
print "Please update LeeF22.in in simulator directory\a"
exit
fi
$CBIN/LeeF22
print "\a"
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
print "Output File\t: $KSIM/LeeF22.out\n"
sleep 10
#
