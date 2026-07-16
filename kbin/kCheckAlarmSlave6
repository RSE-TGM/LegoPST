#!/bin/ksh
#
#
kAddStatus kCheckAlarm6 UNKNOWN
echo ${star5}
echo ${star5} >> $KLOG/kCheckAlarm.log
kAddScreen kCheckAlarm "Verification n.6 : START"
kAddLog kCheckAlarm "Verification n.6 : START"
print "\n\tOperating Windows selection in N_winXXX.list with the criterion"
print "\tFunctional Area presents in al_sim.conf file.\n"
print "\tFunctional Area List: "$MMI_K_FA_LIST""
print "\n\tOperating Windows selection in N_winXXX.list with the criterion" >> $KLOG/kCheckAlarm.log
print "\tFunctional Area presents in al_sim.conf file.\n" >> $KLOG/kCheckAlarm.log
print "\tFunctional Area List: "$MMI_K_FA_LIST"" >> $KLOG/kCheckAlarm.log
#
ls $KWIN/N_win*.list | grep -v DFGK_S | while read file
do
	grep "@#L@HIER" ${file}  > ${file}.tmp
	grep -v "@#L@HIER" ${file} | while read line
	do
	 	FA=` echo "${line}" | cut -f2 -d',' `
	 	for facrit in $MMI_K_FA_LIST
	 	do
	 	if [ "${FA}" = "${facrit}" ]
	 	then
	 	echo "${line}" >> ${file}.tmp
	 	fi
	 	done
	 	if [ "${FA}" = "-1" ]
	 	then
	 	staz=`echo "${line}" | cut -f1  | sed "s/O_//g"`
	 	stazhier=`grep ${staz} ${KWIN}/sosti.dat`
	 	echo "@ Warning (FA=-1) : ${stazhier} @"
	 	echo "@ Warning (FA=-1) : ${stazhier} @" >> $KLOG/kCheckAlarm.log
		echo "${line}" >> ${file}.tmp
	 	fi
	done
	mv ${file} ${file}.kold2
	mv  ${file}.tmp ${file}
done
kAddStatus kCheckAlarm6 RUNED
#
kAddScreen kCheckAlarm "Verification n.6 : END"
kAddLog kCheckAlarm "Verification n.6 : END"
print "${star5}\n"
print "${star5}\n" >> $KLOG/kCheckAlarm.log
