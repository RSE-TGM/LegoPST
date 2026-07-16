#!/bin/ksh
#
#clear
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
if [ -f $KLOG/kNetCompi.log ]
then
mv $KLOG/kNetCompi.log $KLOG/kNetCompi.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kNetCompi.log
kAddScreen kNetCompi Start
kAddLog kNetCompi Start
#
if [ ! -f $KSIM/S01 ]
then
print "ERROR\t: File $KSIM/S01 not found"
print "ERROR\t: File $KSIM/S01 not found" >> $KLOG/kNetCompi.log
banner "NOK"
banner "NOK" >> $KLOG/kNetCompi.log
print "\a\a\a"
exit
fi
#
cd $KSIM
export DEBUG=YES
rm -f $KSIM/net_compi.out
net_compi >> $KLOG/kNetCompi.log
export DEBUG=YES
if [ ! -f $KSIM/variabili.rtf ]
then
banner "NOK"
print "\a\a\a"
echo "ERROR : variabili.rtf not generated" >> $KLOG/kNetCompi.log
fi
kAddScreen kNetCompi End
kAddLog kNetCompi End
print "Log File :\t$KLOG/kNetCompi.log"
echo "$star"
echo "$star" >> $KLOG/kNetCompi.log

