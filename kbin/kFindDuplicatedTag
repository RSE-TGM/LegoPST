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
if [ -f $KLOG/kFindDuplicatedTag.log ]
then
mv $KLOG/kFindDuplicatedTag.log $KLOG/kFindDuplicatedTag.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kFindDuplicatedTag.log
kAddScreen kFindDuplicatedTag Start
kAddLog kFindDuplicatedTag Start
#
cd ${KSIM}
if [ ! -f ${KSIM}/variabili.edf ]
then
echo "${KSIM}/variabili.edf not found"
exit
fi
cat variabili.edf | grep "\-\-U" | tr -s '\011' ' ' | sort | uniq | tr ' ' '[\n*]' | grep '@#K@' | sort > kFindDuplicatedTag.DB
uniq -d kFindDuplicatedTag.DB > kFindDuplicatedTag.PB
if [ -s ${KSIM}/kFindDuplicatedTag.PB ]
then
cat kFindDuplicatedTag.PB | while read tag
do
print "ERROR\t: ${tag}\tduplicated"
print "ERROR\t: ${tag}\tduplicated" >> $KLOG/kFindDuplicatedTag.log
done
print "\n$star8"
print "SOLUTION \t: 1.- ksim"
print "\t\t: 2.- grep @#K@DuplicatedTag variabili.edf | grep "\""\-\-U"\"" | sort | uniq"
print "\t\t: 3.- Delete with config or legocad the wrong tag"
print "$star8\n"
print "\n$star8" >> $KLOG/kFindDuplicatedTag.log
print "SOLUTION \t: 1.- ksim" >> $KLOG/kFindDuplicatedTag.log
print "\t\t: 2.- grep @#K@DuplicatedTag variabili.edf | grep "\""\-\-U"\"" | sort | uniq" >> $KLOG/kFindDuplicatedTag.log
print "\t\t: 3.- Delete with config or legocad the wrong tag" >> $KLOG/kFindDuplicatedTag.log
print "$star8\n" >> $KLOG/kFindDuplicatedTag.log
banner "NOK"
print "\a\a"
else
echo "No error found"
fi
rm -f kFindDuplicatedTag.DB kFindDuplicatedTag.PB
#
kAddScreen kFindDuplicatedTag End 
kAddLog kFindDuplicatedTag End
echo "$star"
echo "$star" >> $KLOG/kFindDuplicatedTag.log
print "Log file\t: $KLOG/kFindDuplicatedTag.log"
kCheckDirect
