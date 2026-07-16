#!/bin/ksh
#
#
kAddStatus kCheckAlarm7 UNKNOWN
echo ${star5}
echo ${star5} >> $KLOG/kCheckAlarm.log
kAddScreen kCheckAlarm "Verification n.7 : START"
kAddLog kCheckAlarm "Verification n.7 : START"
print "\n\tVerification that all Plant Display belong to"
print "\tFunctional Area presents in al_sim.conf file.\n"
print "\tFunctional Area List: "$MMI_K_FA_LIST"\n"
print "\n\tVerification that all Plant Display belong to" >> $KLOG/kCheckAlarm.log
print "\tFunctional Area presents in al_sim.conf file.\n" >> $KLOG/kCheckAlarm.log
print "\tFunctional Area List: "$MMI_K_FA_LIST"\n" >> $KLOG/kCheckAlarm.log
#
cd $KBASIC
rm -f $KBASIC/kCheckAlarm7.ok
ls M_S_*.pag | grep -v GR.pag | while read file
do
	 	FA=` grep gerarchia ${file} | cut -f2 -d, `
	 	for facrit in $MMI_K_FA_LIST
	 	do
	 	if [ "${FA}" = "${facrit}" ]
	 	then
	 	echo "${file}" >> $KBASIC/kCheckAlarm7.ok
	 	fi
	 	done
done
ls M_S_*.pag | grep -v GR.pag > $KBASIC/kCheckAlarm7.all
sort -o kCheckAlarm7.all kCheckAlarm7.all
sort -o kCheckAlarm7.ok kCheckAlarm7.ok
diff kCheckAlarm7.all kCheckAlarm7.ok | grep "^<" | tr -d '<' > $KBASIC/kCheckAlarm7.pb
cat $KBASIC/kCheckAlarm7.pb | while read pd
do
FA=` grep gerarchia ${pd} | cut -f2 -d, `
localger=` grep gerarchia ${pd} | cut -f2 `
if [ "${FA}" = "-1" ]
then
print "\tWARNING\t: ${pd} root plant display : ${localger}"
print "\tWARNING\t: ${pd} root plant display : ${localger}" >> $KLOG/kCheckAlarm.log
else
print "\tERROR\t: The Plant Display ${pd} has a hierarchy which\n\t\t  not belong to the Simulator"
print "\tERROR\t: The Plant Display ${pd} has a hierarchy which\n\t\t  not belong to the Simulator" >> $KLOG/kCheckAlarm.log
kAddStatus kCheckAlarm7 ERROR
fi
done
if [ "`cat ${KSTATUS}/kCheckAlarm7.status`" = "ERROR" ]
then
banner NOK
exit
fi
#
print "\n"
kAddScreen kCheckAlarm "Verification n.7 : END"
kAddLog kCheckAlarm "Verification n.7 : END"
print "${star5}\n"
print "${star5}\n" >> $KLOG/kCheckAlarm.log
