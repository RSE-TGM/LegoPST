#!/bin/ksh
#
#
kAddStatus kCheckAlarm2 Running
echo ${star5}
echo ${star5} >> $KLOG/kCheckAlarm.log
kAddScreen kCheckAlarm "Verification n.2 : START"
kAddLog kCheckAlarm "Verification n.2 : START"
print "\n\tVerification that all alarms are associated with a Plant Display"
print "\tor an Operating Window of the same Functional Area\n"
print "\n\tVerification that all alarms are associated with a Plant Display" >> $KLOG/kCheckAlarm.log
print "\tor an Operating Window of the same Functional Area\n" >> $KLOG/kCheckAlarm.log
#
rm -f $KSIM/*.pb
ErrorNumber=0
cat $KSTART_TABLES/ALARM.txt | grep -v AL_HIER | awk -v FS=";" ' {print $1, $8, $12} ' | \
while read alarm zone1 zone2
do
if [ ! "$zone1" = "$zone2" ]
then
AlarmTag=`grep -w ^${alarm} $KSTART_TABLES/TAG.txt | cut -f2 -d";" `
echo "The alarm ${AlarmTag} belong to the FA [${zone2}] (Wrong value [${zone1}])"
echo "The alarm ${AlarmTag} belong to the FA [${zone2}] (Wrong value [${zone1}])" >> $KLOG/kCheckAlarm.log
ErrorNumber=`expr ${ErrorNumber} + 1`
#echo $alarm $zone1 $zone2 >> $KSIM/scada.pb
#grep ^"$alarm;" $KSTART_TABLES/ALARM.txt >> $KSIM/alarm.pb
#grep ^"$alarm;" $KSTART_TABLES/TAG.txt >> $KSIM/tag.pb
fi
done
if [ "${ErrorNumber}" = 0 ]
then
kAddScreen kCheckAlarm "Verification n.2 : OK"
kAddLog kCheckAlarm "Verification n.2 : OK"
kAddStatus kCheckAlarm2 OK
else
print "\n${ErrorNumber} error(s) found\a\n"
banner NOK
kAddScreen kCheckAlarm "Verification n.2 : NOK"
kAddLog kCheckAlarm "Verification n.2 : NOK"
kAddStatus kCheckAlarm2 NOK
exit
fi
#
kAddScreen kCheckAlarm "Verification n.2 : END"
kAddLog kCheckAlarm "Verification n.2 : END"
print "${star5}\n"
print "${star5}\n" >> $KLOG/kCheckAlarm.log
