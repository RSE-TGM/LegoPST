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
#  Niente piu' kScd: SCADA e' stato rimosso (ottobre 2026). kStart ora
#  aspetta da se' che la simulazione sia partita, con un limite di tempo, ed
#  esce con 1 se non ci riesce: qui non si gira piu' in tondo ad aspettare.
if ! kStart
then
print "kRun :\tla simulazione non e' partita (vedi $KLOG/kStart.log)"
kAddStatus kRun Free
exit 1
fi
sleep 5
kMmi 0
sleep 30
#
kAddStatus kRun Free
#

