#!/bin/ksh
#
clear
$KBIN/kTest
KTEST=`cat $KSTATUS/kTest.status `
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit
fi
kpresentation
if [ -f $KLOG/kMakeConnDB.log ]
then
mv $KLOG/kMakeConnDB.log $KLOG/kMakeConnDB.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kMakeConnDB.log
kAddScreen kMakeConnDB Start
kAddLog kMakeConnDB Start
kAddStatus kMakeConnDB Start
#
if [ ! -r $KWIN ]
then
mkdir $KWIN
fi
cd $KSIM
rm -f $KSIM/REG_INT_CONN_DB*
rm -f $KWIN/REG_INT_CONN_DB*
kMakeConnDBSlave1
mv $KSIM/REG_INT_CONN_DB.pag $KWIN/REG_INT_CONN_DB.pag
mv $KSIM/REG_INT_CONN_DB.dir $KWIN/REG_INT_CONN_DB.dir
print "\nDatabase REG_INT_CONN_DB created in $KWIN\n"
print "\nDatabase REG_INT_CONN_DB created in $KWIN\n" >> $KLOG/kMakeConnDB.log
#
kAddScreen kMakeConnDB End 
kAddLog kMakeConnDB End
kAddStatus kMakeConnDB End
echo "$star"
echo "$star" >> $KLOG/kMakeConnDB.log
echo "Log File : $KLOG/kMakeConnDB.log"
