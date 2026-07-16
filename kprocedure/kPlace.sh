#!/bin/ksh
#
clear
${KBIN}/kTest
KTEST=`cat $KSTATUS/kTest.status `
echo "kTest result : $KTEST"
if [ ! "${KTEST}" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit
fi
kpresentation
if [ -f $KLOG/kPlace.log ]
then
mv $KLOG/kPlace.log $KLOG/kPlace.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kPlace.log
kAddScreen kPlace Start
kAddLog kPlace Start
#
###############################################################################
#	Clean
###############################################################################
cd ${KWIN}
ls | grep ^F_ | while read OldMf
do
echo "Please remove ${OldMf}"
done
#
print "\n\tWindows Generation :\n\t\t(*)Generic Malfunctions\n\t\t(*)Specific Malfunctions\n\t\t(*)Remote Parameters\n"
print "\n\tWindows Generation :\n\t\t(*)Generic Malfunctions\n\t\t(*)Specific Malfunctions\n\t\t(*)Remote Parameters\n" >> $KLOG/kPlace.log
echo "..."
sleep 3
cd ${KWIN}
MF -p N AP
#
kAddScreen kPlace End 
kAddLog kPlace End
echo "${star}"
echo "${star}" >> ${KLOG}/kPlace.log
echo "Log file : ${KLOG}/kPlace.log"
#
