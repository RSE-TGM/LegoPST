#!/bin/ksh
#
#
kAddStatus kCheckAlarm3 UNKNOWN
echo ${star5}
echo ${star5} >> $KLOG/kCheckAlarm.log
kAddScreen kCheckAlarm "Verification n.3 : START"
kAddLog kCheckAlarm "Verification n.3 : START"
print "\n\tAlarm selection in cai_var.dat with the criterion Functional Area"
print "\tpresents in al_sim.conf file.\n"
print "\tFunctional Area List: "$MMI_K_FA_LIST""
print "\n\tAlarm selection in cai_var.dat with the criterion Functional Area" >> $KLOG/kCheckAlarm.log
print "\tpresents in al_sim.conf file.\n" >> $KLOG/kCheckAlarm.log
print "\tFunctional Area List: "$MMI_K_FA_LIST"" >> $KLOG/kCheckAlarm.log
#
rm -f $KSIM/cai_var.tmp
cat $KSIM/cai_var.dat | awk -v FS="," ' {print $1, $2, $3, $4, $5, $6, $7, $8} ' | \
while read field1 FA field3 field4 field5 field6 type kks
do
	for facrit in $MMI_K_FA_LIST
	do
	if [ $FA = $facrit ]
	then
	echo "$field1,$FA,$field3,$field4,$field5,$field6,$type,$kks" >> $KSIM/cai_var.tmp
	fi
	done
done
mv $KSIM/cai_var.dat $KSIM/cai_var.dat.kold
mv $KSIM/cai_var.tmp $KSIM/cai_var.dat
print "\n\tResults are in $KSIM/cai_var.dat file\n"
print "\n\tResults are in $KSIM/cai_var.dat file\n" >> $KLOG/kCheckAlarm.log
kAddStatus kCheckAlarm3 OK
#
kAddScreen kCheckAlarm "Verification n.3 : END"
kAddLog kCheckAlarm "Verification n.3 : END"
print "${star5}\n"
print "${star5}\n" >> $KLOG/kCheckAlarm.log
