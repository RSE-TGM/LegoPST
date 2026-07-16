#!/bin/ksh
#
#
kAddStatus kCheckAlarm0 UNKNOWN
echo ${star5}
echo ${star5} >> $KLOG/kCheckAlarm.log
kAddScreen kCheckAlarm "Verification n.0 : START"
kAddLog kCheckAlarm "Verification n.0 : START"
print "\n\tVerification that all alarms presents in simulator are configurated in database.\n"
print "\n\tVerification that all alarms presents in simulator are configurated in database.\n" >> $KLOG/kCheckAlarm.log
print "\n\tWait ..."
#
cd $KEXPORT
cp TAG.txt TAG_ALL.txt
cat TAGS.txt >> TAG_ALL.txt
rm -f $KLOG/kCheckAlarmSlave0.log
cd $KSIM
grep -F @#K@ variabili.edf | grep -Fv @#K@X | grep -F _X | grep -v ALWAYS |  tr -s '\011' ' ' | awk '{ print $6, $2 }' | \
        sed "s/@#K@//g" | sort | uniq > $KLOG/AlarmsInSimulator.txt
cd $KLOG
cat AlarmsInSimulator.txt | while read kks tag
do
AlarmPresentInDataBase=`grep $kks $KEXPORT/TAG_ALL.txt | wc -l | tr -s '\011' ' ' | tr -d '[:space:]'`
if [ "$AlarmPresentInDataBase" = "0" ]
then
print "ERROR : Alarm $kks\t($tag) presents in simulator but not found in database" >> $KLOG/kCheckAlarmSlave0.log
fi
done
ErrorNumber=`grep ERROR $KLOG/kCheckAlarmSlave0.log | wc -l | tr -s '\011' ' ' | tr -d '[:space:]'`
if [ ! "$ErrorNumber" = "0" ]
then
print "\n\t$ErrorNumber errors\n\a"
banner NOK
fi
cat $KLOG/kCheckAlarmSlave0.log >> $KLOG/kCheckAlarm.log
rm $KLOG/kCheckAlarmSlave0.log
rm -f TAG_ALL.txt
rm -f $KLOG/AlarmsInSimulator.txt
#
kAddStatus kCheckAlarm0 OK
#
kAddScreen kCheckAlarm "Verification n.0 : END"
kAddLog kCheckAlarm "Verification n.0 : END"
print "${star5}\n"
print "${star5}\n" >> $KLOG/kCheckAlarm.log
