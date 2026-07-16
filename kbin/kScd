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
kClean
kAddStatus kScd Reset
kAddStatus kStart Reset
#
kpresentation
if [ -f $KLOG/kScd.log ]
then
mv $KLOG/kScd.log $KLOG/kScd.log.kold
fi
#
kCleanStatus=`cat $KSTATUS/kClean.status`
if [ ! "$kCleanStatus" = "OK" ]
then
banner "NOK"
print "ERROR : kClean not executed\n\a"
exit
fi
kScdStatus=`cat $KSTATUS/kScd.status`
if [  "$kScdStatus" = "Start" ]
then
banner "NOK"
print "ERROR : kScd starting ...\n\a"
exit
fi
kScdStatus=`cat $KSTATUS/kScd.status`
if [  "$kScdStatus" = "OK" ]
then
banner "NOK"
print "ERROR : kScd already started\n\a"
exit
fi
###############################################################################
#	VERIFICATION END
###############################################################################
echo ${star}
echo ${star} >> $KLOG/kScd.log
kAddScreen kScd Start
kAddLog kScd Start
kAddStatus kScd Start
kAddLog kLogger "Start kScd"
kAddStatistic kScd
echo "$star"
echo "$star" >> $KLOG/kScd.log
#
rm -f $KFILEOP/arcfile.rtf
#
kAddStatus kClean NOK
cd $KFILEOP ; pwd ; scada /FAT /AFRV /M4 &
sleep 3
#
while [ ! -f $KFILEOP/arcfile.rtf ]
do
#echo "..."
sleep 1
done
#
echo "$star"
echo "$star" >> $KLOG/kScd.log
kAddScreen kScd Started
kAddLog kScd Started
kAddStatus kScd Started
kAddLog kLogger "Started kScd"
echo "$star"
echo "$star" >> $KLOG/kScd.log
#
