#!/bin/ksh
#
#clear
${KBIN}/kTest
KTEST=`cat $KSTATUS/kTest.status`
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit
fi
#kpresentation
if [ -f $KLOG/kCheckDirect.log ]
then
mv $KLOG/kCheckDirect.log $KLOG/kCheckDirect.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kCheckDirect.log
kAddScreen kCheckDirect Start
kAddLog kCheckDirect Start
#
cd ${KSIM}
if [ ! -f S01 ]
then
print "-->> Error : File S01 not found \a"
banner NOK
exit
fi
###############################################################################
#	DirLinksAn.list
###############################################################################
rm -f ${KWIN}/DirLinksAn.list
count=0
cat S01 |
while read var
do
	if [ $count -lt 3 ]
	then
	if [ "${var}" = '****' ]
	then 
	let count=count+1 
	fi
	continue
	fi
	if [ "${var}" = '****' ]
	then 
	break 
	fi
	echo ${var}
done |
	grep ' R ' |
	while read tok1 tok2 tok3 tok4
do
	cd ${tok1}
	if [ -f DirLinksAn.list ]
	then
	ls DirLinksAn.list |
	while read var
	do
        	if [ ! -f ${KWIN}/${var} ]
        	then
		cat ${var} > ${KWIN}/${var}
        	else
		cat ${var} | grep -v 'Offset' >> ${KWIN}/${var}
		fi
      	done
	fi
done
###############################################################################
cd $KWIN
cat DirLinksAn.list | cut -f1 -d" "  | sort | uniq -d > $KLOG/kCheckDirect.err
if [ -s $KLOG/kCheckDirect.err ]
then
print "\nThe following tags are duplicated in DirLinksAn.list :"
print "\nThe following tags are duplicated in DirLinksAn.list :" >> $KLOG/kCheckDirect.log
cat $KLOG/kCheckDirect.err
cat $KLOG/kCheckDirect.err >> $KLOG/kCheckDirect.log
print "\a"
banner NOK
echo "Log file : $KLOG/kCheckDirect.log"
exit
fi
#
kAddScreen kCheckDirect End 
kAddLog kCheckDirect End
echo "$star"
echo "$star" >> $KLOG/kCheckDirect.log
echo "Log file : $KLOG/kCheckDirect.log"
