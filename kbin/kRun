#!/bin/ksh
#
clear
. ${HOME}/.profile
${KBIN}/kTest
KTEST=`cat $KSTATUS/kTest.status `
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit
fi
kpresentation
if [ -f $KLOG/kRun.log ]
then
mv $KLOG/kRun.log $KLOG/kRun.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kRun.log
kAddScreen kRun Start
kAddLog kRun Start
kAddLog kLogger "Start kRun"
#
kRunStatus=`cat $KSTATUS/kRun.status`
if [ "$kRunStatus" = "Blocked" ]
then
echo "kRun is blocked"
exit
fi
kAddStatus kRun Blocked
#
kClean
sleep 5
#
kScd
sleep 5
#
kScdStatus=`cat $KSTATUS/kScd.status`
while [ ! "$kScdStatus" = "Started" ]
do
print "kRun :\tWaiting for SCADA started"
sleep 20
kScdStatus=`cat $KSTATUS/kScd.status`
done
sleep 5
kStart
sleep 5
#
kStartStatus=`cat $KSTATUS/kStart.status`
while [ ! "$kStartStatus" = "Started" ]
do
print "kRun :\tWaiting for SCHEDULATOR started"
sleep 20
kStartStatus=`cat $KSTATUS/kStart.status`
done
sleep 5
kMmi 0
sleep 30
#
kAddStatus kRun Free
#

