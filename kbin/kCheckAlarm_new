#!/bin/ksh
#
clear
${KBIN}/kTest
KTEST=`cat $KSTATUS/kTest.status`
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environment test not succesful\a"
banner "NOK"
exit
fi
. kSimMove
kpresentation
if [ -f $KLOG/kCheckAlarm.log ]
then
mv $KLOG/kCheckAlarm.log $KLOG/kCheckAlarm.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kCheckAlarm.log
kAddScreen kCheckAlarm Start
kAddLog kCheckAlarm Start
kAddStatus kCheckAlarm RUNNING
#
if [ ! -f $KSTART_TABLES/ALARM.txt ]
then
echo File $KSTART_TABLES/ALARM.txt not found !
exit
fi
if [ ! -f $KSTART_TABLES/TAG.txt ]
then
echo File $KSTART_TABLES/TAG.txt not found !
exit
fi
if [ ! -f $KSTART_TABLES/TAGS.txt ]
then
echo File $KSTART_TABLES/TAGS.txt not found !
#gp 10/10/2003
#exit
fi
if [ ! -f $KEXPORT/TAG.txt ]
then
echo File $KEXPORT/TAG.txt not found !
exit
fi
if [ ! -f $KSIM/al_sim.conf ]
then
echo File $KSIM/al_sim.conf not found !
exit
fi
if [ ! -f $KWIN/sosti.dat ]
then
echo File $KWIN/sosti.dat not found !
exit
fi
if [ `ls  $KWIN/*.list | wc -l` -le "0" ]
then
echo Files $KWIN/*.list not found !
exit
fi
if [ ! -f $KSIM/variabili.edf ]
then
echo Files $KSIM/variabili.edf not found !
exit
fi
#
#grep ^MMI_K_FA_LIST $KSIM/al_sim.conf  | sed "s/=/ /g" | \
#awk ' {print $1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16} '  | \
#while read field1 field2 field3 field4 field5 field6 field7 field8 field9 field10 field11  field12 field13 field14 field15 field16
#do
#export MMI_K_FA_LIST="$field1 $field2 $field3 $field4 $field5 $field6 $field7 $field8 $field9 $field10 $field11 $field12 $field13 $field14 $field15 $field16"
#done
MMI_K_FA_LIST=`grep ^MMI_K_FA_LIST $KSIM/al_sim.conf | sed "s/=/ /g"`
if [ "$MMI_K_FA_LIST" = "" ]
then
print "\aRessource MMI_K_FA_LIST not found in $KSIM/al_sim.conf"
banner NOK
exit
fi
#
. $KBINSLAVE/kCheckAlarmSlave0
#
. $KBINSLAVE/kCheckAlarmSlave1
#
#. $KBINSLAVE/kCheckAlarmSlave7
#
. $KBINSLAVE/kCheckAlarmSlave2
#
. $KBINSLAVE/kCheckAlarmSlave3
#
. $KBINSLAVE/kCheckAlarmSlave4
#
. $KBINSLAVE/kCheckAlarmSlave5
#
. $KBINSLAVE/kCheckAlarmSlave6
#
kAddScreen kCheckAlarm End 
kAddLog kCheckAlarm End
kAddStatus kCheckAlarm END
echo "$star"
echo "$star" >> $KLOG/kCheckAlarm.log
print "Log file :\t $KLOG/kCheckAlarm.log"
