#!/bin/ksh
#
clear
${KBIN}/ktest
KTEST=`cat $KSTATUS/kTest.status`
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit
fi
kpresentation
if [ -f $KLOG/kDiffS01.log ]
then
mv $KLOG/kDiffS01.log $KLOG/kDiffS01.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kDiffS01.log
kAddScreen kDiffS01 Start
kAddLog kDiffS01 Start
#
cd ${KSIM}
if [ ! -f S01 ]
then
print "-->> Error : File S01 not found \a"
banner NOK
exit
fi
###############################################################################
${KBINSLAVE}/kDiffS01Slave1
if [ $SID_ENV = "SID" ]
then
	${KBINSLAVE}/kDiffS01Slave2
else
	if [ $SID_ENV = "GIPS" ]
	then
		${FBIN}/kDiffS01Slave6
	else
		echo "Warning: Variabile SID_ENV non inizializzata! (valori previsti SID/GIPS)"
	fi
fi	
${FBIN}/kDiffS01Slave4
###############################################################################
#
echo ${star5} >> $KLOG/kDiffS01.log
cat ${KSIM}/diffs01.out >> $KLOG/kDiffS01.log
rm ${KSIM}/diffs01.out
echo ${star5} >> $KLOG/kDiffS01.log
echo ${star5} >> $KLOG/kDiffS01.log
cat ${KSIM}/diffs01.err >> $KLOG/kDiffS01.log
rm ${KSIM}/diffs01.err
echo ${star5} >> $KLOG/kDiffS01.log
kAddScreen kDiffS01 End 
kAddLog kDiffS01 End
echo "$star"
echo "$star" >> $KLOG/kDiffS01.log
echo "Log file : $KLOG/kDiffS01.log"









