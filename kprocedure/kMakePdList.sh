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
if [ -f $KLOG/kMakePdList.log ]
then
mv $KLOG/kMakePdList.log $KLOG/kMakePdList.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kMakePdList.log
kAddScreen kMakePdList Start
kAddLog kMakePdList Start
#
if [ -f $KSIM/pd.list ]
then
mv $KSIM/pd.list $KSIM/pd.list.kold
fi
for fa in $KFUNCTIONALAREA
do
	for directory in $KDIRPD $KDIRGR
	do
	echo "Adding $directory (${fa}) in $KSIM/pd.list"
	echo "Adding $directory (${fa}) in $KSIM/pd.list" >> $KLOG/kMakePdList.log
	cd $directory/${fa}/pag
	ls M_S_*.pag | sed "s/M_S_//g" | sed "s/.pag//g" >> $KSIM/pd.list
	done
done	
###############################################################################
mv $KSIM/pd.list $KSIM/pd.list.tmp
sort $KSIM/pd.list.tmp | uniq > $KSIM/pd.list
rm -f $KSIM/pd.list.tmp
kAddScreen kMakePdList End 
kAddLog kMakePdList End
echo "$star"
echo "$star" >> $KLOG/kMakePdList.log
print "Log file\t: $KLOG/kMakePdList.log"
