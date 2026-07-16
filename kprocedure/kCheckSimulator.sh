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
if [ -f $KLOG/kCheckSimulator.log ]
then
mv $KLOG/kCheckSimulator.log $KLOG/kCheckSimulator.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kCheckSimulator.log
kAddScreen kCheckSimulator Start
kAddLog kCheckSimulator Start
###############################################################################
print "\n${star5}"
echo "Malfunctions not found"
echo ${star5}
print "\n${star5}" >> $KLOG/kCheckSimulator.log
echo "Malfunctions not found" >> $KLOG/kCheckSimulator.log
echo ${star5} >> $KLOG/kCheckSimulator.log
cd ${KWIN}
ls | grep O_ | grep .pag | grep -v DTW | \
	grep -v MGW | grep -v DTS | grep -v DFRS | \
	grep -v DVW | grep -v DFGK_S | \
	grep -v DAU | grep -v DBR | grep -v DFRC | \
	grep -v DFGK | grep -v DGS | while read ow
		do
		kks=`echo ${ow} | cut -f2 -d"_"`
		if [ ! "${kks}" = "" ]
		then
			fault=`echo F_${kks}.pag`
			if [ ! -f ${fault} ]
			then
			print "ERROR : Malfunction of OW [${ow}] \tnot found"
			print "ERROR : Malfunction of OW [${ow}] \tnot found" >> $KLOG/kCheckSimulator.log
			fi
		fi
		done
###############################################################################
cd ${KWIN}
rm -f $KLOG/kCheckSimulator.tmp
ls M_S_*pag | while read pd
do
grep pageName ${pd} | grep -v M_S_NS | cut -f2 | while read page
	do
	if [ ! -f "${page}.pag" ]
	then
	print "ERROR : The hyperlink [${page}] \tof the PD ${pd} \tnot found" >> $KLOG/kCheckSimulator.tmp
	fi
	done
done
sort -o $KLOG/kCheckSimulator.tmp $KLOG/kCheckSimulator.tmp 
print "\n${star5}"
echo "Remote parameters not found"
echo ${star5}
print "\n${star5}" >> $KLOG/kCheckSimulator.log
echo "Remote parameters not found" >> $KLOG/kCheckSimulator.log
echo ${star5} >> $KLOG/kCheckSimulator.log
grep F_ $KLOG/kCheckSimulator.tmp
grep F_ $KLOG/kCheckSimulator.tmp >> $KLOG/kCheckSimulator.log
print "\n${star5}"
echo "Plant displaies not found"
echo ${star5}
print "\n${star5}" >> $KLOG/kCheckSimulator.log
echo "Plant displaies not found" >> $KLOG/kCheckSimulator.log
echo ${star5} >> $KLOG/kCheckSimulator.log
grep -v F_ $KLOG/kCheckSimulator.tmp
grep -v F_ $KLOG/kCheckSimulator.tmp >> $KLOG/kCheckSimulator.log
rm -f $KLOG/kCheckSimulator.tmp
###############################################################################
$KBINSLAVE/kCheckSimulatorSlave1
###############################################################################
echo ${star5}
echo ${star5} >> $KLOG/kCheckSimulator.log
error=`grep ERROR $KLOG/kCheckSimulator.log | wc -l`
echo ${star2}
if [ "${error}" -gt "0" ]
then
banner "NOK"
print "\t${error} ERRORS found during kCheckSimulator\a\a\a"
banner "NOK" >> $KLOG/kCheckSimulator.log
print "\t${error} ERRORS found during kCheckSimulator" >> $KLOG/kCheckSimulator.log
else
print "\tkCheckSimulator : OK"
fi
echo ${star2}
kAddScreen kCheckSimulator End 
kAddLog kCheckSimulator End
print "\n${star}"
print "\n${star}" >> $KLOG/kCheckSimulator.log
print "Log file :\t$KLOG/kCheckSimulator.log"
