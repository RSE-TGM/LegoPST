#!/bin/ksh
#
$KBIN/kTest
KTEST=`cat $KSTATUS/kTest.status `
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit
fi
mode=$1
if [ "$mode" = "-i" ]
then
clear
kpresentation
fi
if [ ! -f $KWIN/mgw.conf ]
then
echo "File $KWIN/mgw.conf not found"
banner NOK
print "\a\a\a"
exit
fi
if [ ! -f $KWIN/mgw.descr ]
then
echo "File $KWIN/mgw.descr not found"
banner NOK
print "\a\a\a"
exit
fi
if [ -f $KLOG/kMalfunctionGroupWindow.log ]
then
mv $KLOG/kMalfunctionGroupWindow.log $KLOG/kMalfunctionGroupWindow.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kMalfunctionGroupWindow.log
kAddScreen kMalfunctionGroupWindow Start
kAddLog kMalfunctionGroupWindow Start
kAddStatus kMalfunctionGroupWindow Start
#
if [ "$mode" = "" ]
then
mode="-b"
fi
echo "Mode = $mode" >> $KLOG/kMalfunctionGroupWindow.log
if [ "$mode" = "-i" ]
then
kAddScreen kMalfunctionGroupWindow  "Interface mode not implemented"
kAddLog kMalfunctionGroupWindow "Interface mode not  implemented"
else
	if [ "$mode" = "-b" ]
	then
	kAddScreen kMalfunctionGroupWindow  "Creating kMalfunctionGroupWindow.in "
	kAddLog kMalfunctionGroupWindow  "Creating kMalfunctionGroupWindow.in "
	. $KBINSLAVE/kMalfunctionGroupWindowSlave4
	kAddScreen kMalfunctionGroupWindow  "Creating *.ListVert "
	kAddLog kMalfunctionGroupWindow  "Creating *.ListVert "
	. $KBINSLAVE/kMalfunctionGroupWindowSlave2
	kAddScreen kMalfunctionGroupWindow  "Creating mgw.list "
	kAddLog kMalfunctionGroupWindow  "Creating mgw.list "
	. $KBINSLAVE/kMalfunctionGroupWindowSlave3
	else
	echo ERROR not defined
	fi
fi
#
# Cleaning
#rm -f ${KWIN}/*.ListVert ${KWIN}/kMalfunctionGroupWindow.in
#
kAddScreen kMalfunctionGroupWindow End 
kAddLog kMalfunctionGroupWindow End
kAddStatus kMalfunctionGroupWindow End
echo "$star"
echo "$star" >> $KLOG/kMalfunctionGroupWindow.log
