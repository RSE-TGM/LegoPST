#!/bin/ksh
#
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
. kSimMove
kpresentation
if [ -f $KLOG/kConnex.log ]
then
mv $KLOG/kConnex.log $KLOG/kConnex.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kConnex.log
kAddScreen kConnex Start
kAddLog kConnex Start
#
mode=$1
if [ "$mode" = "" ]
then
mode="-b"
fi
echo "Mode = $mode" >> $KLOG/kConnex.log
if [ "$mode" = "-i" ]
then
kAddScreen kConnex  "Interface mode not implemented"
kAddLog kConnex "Interface mode not  implemented"
else
	if [ "$mode" = "-b" ]
	then
	. $KBIN/kConnexSlave1
	else
	echo ERROR not defined
	fi
fi
#
kAddScreen kConnex End 
kAddLog kConnex End
echo "$star"
echo "$star" >> $KLOG/kConnex.log
print "Log file\t: $KLOG/kConnex.log"
print "Error file\t: $KLOG/kConnex.err"

