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
kpresentation
if [ -f $KLOG/kDiffReport.log ]
then
mv $KLOG/kDiffReport.log $KLOG/kDiffReport.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kDiffReport.log
kAddScreen kDiffReport Start
kAddLog kDiffReport Start
print "\n$star8"
print "Available simulators :\n"
$KBIN/kArchiveDisponibility
print "\n$star8\n"
OldRevision=$1
NewRevision=$2
if [ "$NewRevision" = "" ]
then
	NewRevision=`cat $KINFO/Simulator_Revision.info`
	if [ -f $KINFO/Original_Revision.info ]
	then
	OldRevision=`cat $KINFO/Original_Revision.info`
	else
	print "\nSorry ..."
	print "The simulator $NewRevision is a version without father.\n\a"
	exit
	fi
fi
	CONFIRM=""
	echo "Please confirm :"
	print "\tOld version : [$OldRevision]"
	print "\tNew version : [$NewRevision]"	
	print "\nPress y to confirm"
	read CONFIRM
	if [ ! "${CONFIRM}" = "y" ]
	then
	print "\nSorry ...\a\n"
	echo "$star8"
	exit
	fi
#
if [ ! -f $KARCHIVE/$OldRevision/.info/kProcessReport.info ]
then
print "\aSorry : File $KARCHIVE/$OldRevision/.info/kProcessReport.info not found"
exit
fi
if [ ! -f $KARCHIVE/$NewRevision/.info/kProcessReport.info ]
then
print "\aSorry : File $KARCHIVE/$NewRevision/.info/kProcessReport.info not found"
exit
fi
print "\nFrom $OldRevision to $NewRevision simulator,"
print "the following legocad blocks have been modified :\n"
diff $KARCHIVE/$OldRevision/.info/kProcessReport.info $KARCHIVE/$NewRevision/.info/kProcessReport.info | \
     cut -f 1-3 -d ";" | grep ";" | tr -d '>' | tr -d '<' | uniq | sed  "s/;/ \| /g"
echo
#
print "\nFrom $OldRevision to $NewRevision simulator," >> $KLOG/kDiffReport.log
print "the following legocad blocks have been modified :\n" >> $KLOG/kDiffReport.log
diff $KARCHIVE/$OldRevision/.info/kProcessReport.info $KARCHIVE/$NewRevision/.info/kProcessReport.info | \
     cut -f 1-3 -d ";" | grep ";" | tr -d '>' | tr -d '<' | uniq | sed  "s/;/ \| /g" >> $KLOG/kDiffReport.log
echo >> $KLOG/kDiffReport.log
#
kAddScreen kDiffReport End
kAddLog kDiffReport End
print "Log File :\t$KLOG/kDiffReport.log\a"
echo "$star"
echo "$star" >> $KLOG/kDiffReport.log

