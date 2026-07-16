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
. kSimMove
kpresentation
if [ -f $KLOG/kDirect.log ]
then
mv $KLOG/kDirect.log $KLOG/kDirect.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kDirect.log
kAddScreen kDirect Start
kAddLog kDirect Start
#
cd ${KSIM}
if [ ! -f S01 ]
then
print "-->> Error : File S01 not found \a"
banner NOK
exit
fi
###############################################################################
#	Clean
###############################################################################
cd ${KWIN}
ls | grep ^M_S_ | grep -v _GR | while read OldPd
do
echo "Please remove ${OldPd}"
done
cd ${KSIM}
###############################################################################
#	DirLinksAn.list
###############################################################################
print "\n${star5}"
print "\n${star5}" >> $KLOG/kDirect.log
kAddScreen kDirect "Merge files DirLinksAn.list ..."
kAddLog kDirect "Merge files DirLinksAn.list ..."
print "${star5}\n"
print "${star5}\n" >> $KLOG/kDirect.log
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
	print "\t\tfrom task ${tok1} ..."
	print "\t\tfrom task ${tok1} ..." >> $KLOG/kDirect.log
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
#	Plant Display Elaboration
###############################################################################
print "\n${star5}"
print "\n${star5}" >> $KLOG/kDirect.log
kAddScreen kDirect "Elaboration Plant Displays ... "
kAddLog kDirect "Elaboration Plant Displays ... "
print "${star5}\n"
print "${star5}\n" >> $KLOG/kDirect.log
direct.sh -s ${KSIMNAME}
if [ -f ${KBASIC}/direct.sh.log ]
then
cat ${KBASIC}/direct.sh.log >> ${KLOG}/kDirect.log
rm ${KBASIC}/direct.sh.log
fi
#
kAddScreen kDirect End 
kAddLog kDirect End
echo "$star"
echo "$star" >> $KLOG/kDirect.log
sed "s/Sto esaminando pathnomelist/Reading/g" $KLOG/kDirect.log > $KLOG/kDirect.log.tmp1
sed "s/Operazione conclusa positivamente//g" $KLOG/kDirect.log.tmp1 > $KLOG/kDirect.log.tmp2
sed "s/Errore in /ERROR		: /g" $KLOG/kDirect.log.tmp2 > $KLOG/kDirect.log.tmp3
sed "s/verificare eseguendo/SOLUTION	: 1.- kbasic!		  2.- Execute/g" $KLOG/kDirect.log.tmp3 > $KLOG/kDirect.log.tmp4
sed "s/leggendo il file/		  3.- mye/g" $KLOG/kDirect.log.tmp4 > $KLOG/kDirect.log.tmp5
sed "s/rilevato da chgvar//g" $KLOG/kDirect.log.tmp5 > $KLOG/kDirect.log.tmp6
tr '!' '[\n*]' < $KLOG/kDirect.log.tmp6 > $KLOG/kDirect.log
rm -f $KLOG/kDirect.log.tmp*
echo "Log file : $KLOG/kDirect.log"
