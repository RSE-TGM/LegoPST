#!/bin/ksh
#
debug=$1
###############################################################################
#	VERIFICATION START
###############################################################################
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
if [ -f $KLOG/kStart.log ]
then
mv $KLOG/kStart.log $KLOG/kStart.log.kold
fi
#
kScdStatus=`cat $KSTATUS/kScd.status`
if [ ! "$kScdStatus" = "Started" ]
then
banner "NOK"
print "ERROR : kScd not started\n\a"
exit
fi
kStartStatus=`cat $KSTATUS/kStart.status`
if [ "$kStartStatus" = "Start" ]
then
banner "NOK"
print "ERROR : kStart starting ...\n\a"
exit
fi
kStartStatus=`cat $KSTATUS/kStart.status`
if [ "$kStartStatus" = "Started" ]
then
banner "NOK"
print "ERROR : kStart already started\n\a"
exit
fi
###############################################################################
#	VERIFICATION END
###############################################################################
echo ${star}
echo ${star} >> $KLOG/kStart.log
kAddScreen kStart Start
kAddLog kStart Start
kAddStatus kStart Start
kAddLog kLogger "Start kStart"
kAddStatistic kStart
#
#
rm -f ${KSIM}/.message_list
touch ${KSIM}/.message_list
rm -f ${KSIM}/banco.log
touch ${KSIM}/banco.log
kStat
kAddStatus kClean NOK
cd $KSIM ; net_startup &
sleep 30
#
IdSkedDescr=""
while [ ! "${IdSkedDescr}" = "OK" ]
do
sleep 5
strings ${KSIM}/banco.log ${KSIM}/.message_list | grep 3 | while read LogLine
do
	freeze=`echo ${LogLine} | grep "Reading last recorded B.T." | wc -l | tr -s '\011' ' ' | tr -d '[:space:]' `
	if [ "${debug}" = "DEBUG" ]
	then
	echo $star
	echo "freeze= [${freeze}]"
	echo $star
	fi
	if [ "${freeze}" = "1" ]
	then
	IdSkedDescr="OK"
	kAddStatus kStart Started
	fi
done
done
#
echo "$star"
echo "$star" >> $KLOG/kStart.log
kAddScreen kStart Started
kAddLog kStart Started
kAddStatus kStart Started
kAddLog kLogger "Started kStart"
echo "$star"
echo "$star" >> $KLOG/kStart.log
#
kLeeF22
#
