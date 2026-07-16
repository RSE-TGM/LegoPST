#!/bin/ksh
#
#
kAddStatus kCheckAlarm1 UNKNOWN
echo ${star5}
echo ${star5} >> $KLOG/kCheckAlarm.log
kAddScreen kCheckAlarm "Verification n.1 : START"
kAddLog kCheckAlarm "Verification n.1 : START"
print "\n\tAlarm selection in ALARM.txt with the criterion Functional Area"
print "\tpresents in al_sim.conf file.\n"
print "\tFunctional Area List: "$MMI_K_FA_LIST""
print "\n\tAlarm selection in ALARM.txt with the criterion Functional Area" >> $KLOG/kCheckAlarm.log
print "\tpresents in al_sim.conf file.\n" >> $KLOG/kCheckAlarm.log
print "\tFunctional Area List: "$MMI_K_FA_LIST"" >> $KLOG/kCheckAlarm.log
#
echo "Waiting ..."
grep "AL_ID" $KSTART_TABLES/ALARM.txt > $KSTART_TABLES/ALARM.tmp
grep -v "AL_ID" $KSTART_TABLES/ALARM.txt | while read line
do
 FA=` echo "${line}" | cut -f12 -d';' `
 for facrit in $MMI_K_FA_LIST
 do
 if [ "${FA}" = "${facrit}" ]
 then
 echo "${line}" >> $KSTART_TABLES/ALARM.tmp
 fi
 done
done
mv $KSTART_TABLES/ALARM.txt $KSTART_TABLES/ALARM.txt.kold
mv $KSTART_TABLES/ALARM.tmp $KSTART_TABLES/ALARM.txt
print "\n\tResults are in $KSTART_TABLES/ALARM.txt file\n"
print "\n\tResults are in $KSTART_TABLES/ALARM.txt file\n" >> $KLOG/kCheckAlarm.log
kAddStatus kCheckAlarm1 OK
#
kAddScreen kCheckAlarm "Verification n.1 : END"
kAddLog kCheckAlarm "Verification n.1 : END"
print "${star5}\n"
print "${star5}\n" >> $KLOG/kCheckAlarm.log
