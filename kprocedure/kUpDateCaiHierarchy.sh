#!/bin/ksh
#
export PAGMOD=pagmod
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
if [ -f $KLOG/kUpDateCaiHierarchy.log ]
then
mv $KLOG/kUpDateCaiHierarchy.log $KLOG/kUpDateCaiHierarchy.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kUpDateCaiHierarchy.log
kAddScreen kUpDateCaiHierarchy Start
kAddLog kUpDateCaiHierarchy Start
#
###############################################################################
#	Script Elaboration       
############################################################################### 
cd ${KWIN}
echo "Searching CAI in plant display :"
ls M_S_*.pag | grep -v GR.pag | while read page
do
print " ${page}\c"
echo "page=${page}" > ${page}.scr
grep objectTag ${page} | grep _[FX]$ | sed "s/O_//g" | \
    tr -s '\011' ' ' | awk '{ print $1 , $2 }' | while read tag ow_ext
	do
	FieldNumber=`echo ${ow_ext} | awk -v FS="_" '{ print NF }'`
	FieldNumberInf=`expr ${FieldNumber} - 1`
	ow=`echo ${ow_ext} | cut -f1-${FieldNumberInf} -d"_"`
	ext=`echo ${ow_ext} | cut -f${FieldNumber} -d"_"`
	hier=`grep -w ${ow} sosti.dat | cut -f2-20 -d"," | sed  "s/\"//g"`
	if [ "${hier}" = "" ]
	then
	print "\n\nERROR : Hierarchy of OW ${ow} in PD ${page} not found in sosti.dat\n\a"
	echo "ERROR : Hierarchy of OW ${ow} in PD ${page} not found in sosti.dat" >> $KLOG/kUpDateCaiHierarchy.log
	hier="0,7,-1,-1,-1,-1"
	ext="F"
	fi
	NewValue=`echo "${hier} ${ext}"`
	widgetname=`echo ${tag} | cut -f1 -d"." | cut -f2 -d"*"`
	echo "widgetname=${widgetname}" >> ${page}.scr
	echo "attrib=objectTag" >> ${page}.scr
	echo "value=${NewValue}" >> ${page}.scr
#	echo TAG : ${widgetname} @ NEWVALUE : ${NewValue}
	done
LineNumber=`cat ${page}.scr | wc -l `
if [ "${LineNumber}" -eq "1" ]
then
rm ${page}.scr
fi
done
###############################################################################
#	Script Execution       
############################################################################### 
	ScriptNumber=` ls M_S_*scr | wc -w `
	if [ ! "${ScriptNumber}" -eq "0" ]
	then
		print "\n${star5}\n"
		kAddScreen kUpDateCaiHierarchy "Start $PAGMOD"
		print "\n${star5}\n" >> $KLOG/kUpDateCaiHierarchy.log
		kAddLog kUpDateCaiHierarchy "Start $PAGMOD"
		echo Processing ${ScriptNumber} scripts ...
		ls M_S_*scr | while read script
		do
		pagemod=`echo ${script} | sed "s/.scr//g" `
		$PAGMOD -s ${script}
		mv ${pagemod}.mod ${pagemod}
		rm ${script}
		done
		kAddScreen kUpDateCaiHierarchy "End $PAGMOD"
		print "\n${star5}"
		kAddLog kUpDateCaiHierarchy "End $PAGMOD"
		print "\n${star5}" >> $KLOG/kUpDateCaiHierarchy.log
	else
	print "\n${star3}"
	echo Nothing to do
	echo ${star3}
	fi
###############################################################################
$KBINSLAVE/kUpDateCaiHierarchySlave1
###############################################################################
$KBINSLAVE/kUpDateCaiHierarchySlave2
###############################################################################
error=`grep ERROR $KLOG/kUpDateCaiHierarchy.log | wc -l`
echo ${star2}
if [ "${error}" -gt "0" ]
then
banner "NOK"
print "\t${error} ERRORS found during kUpDateCaiHierarchy\a\a\a"
banner "NOK" >> $KLOG/kUpDateCaiHierarchy.log
print "\t${error} ERRORS found during kUpDateCaiHierarchy" >> $KLOG/kUpDateCaiHierarchy.log
else
print "\tkUpDateCaiHierarchy : OK"
fi
echo ${star2}
kAddScreen kUpDateCaiHierarchy End 
kAddLog kUpDateCaiHierarchy End
print "\n${star}\n"
print "\n${star}\n" >> $KLOG/kUpDateCaiHierarchy.log
print "Log file :\t$KLOG/kUpDateCaiHierarchy.log"
