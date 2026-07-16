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
if [ -f $KLOG/kOw.log ]
then
mv $KLOG/kOw.log $KLOG/kOw.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kOw.log
kAddScreen kOw Start
kAddLog kOw Start
#
echo ${star3}
echo ${star3} >> $KLOG/kOw.log
cd ${KWIN}
###############################################################################
#	Clean
###############################################################################
ls | grep ^O_ | while read OldOw
do
echo "Please remove ${OldOw}"
done
#
SlashNumber=`grep / N_win*list | wc -l | tr -s '\011' ' ' | tr -d ' '`
if [ ! "$SlashNumber" = "0" ]
then
print "\nERROR\t\t: The following files contain a slash(/) symbol."
echo
grep -l / N_win*list
print "\nSOLUTION\t: 1.- Substitue / with : or & in the database"
print "\t\t  2.- Export database on workstation\n"
banner NOK
print "\nERROR\t\t: The following files contain a slash(/) symbol." >> $KLOG/kOw.log
echo >> $KLOG/kOw.log
grep -l / N_win*list >> $KLOG/kOw.log
print "\nSOLUTION\t: 1.- Substitue / with : or & in the database" >> $KLOG/kOw.log
print "\t\t  2.- Export database on workstation\n" >> $KLOG/kOw.log
banner NOK >> $KLOG/kOw.log
print "\a\a\a"
exit
fi
#
ls N_win*.list |
cut -f1 -d. |
while read file
do
    kAddScreen kOw "Creation Operating Window : "${file}" ..."
    kAddLog kOw "Creation Operating Window : "${file}" ..."
    mkstaz ${file}
done
#
echo ${star3}
#
if [ "$1" != -nodetail ]
then
   ls N_win*.list |
   grep -v DFGK_S |
   cut -f1 -d. |
   while read file
   do
       kAddScreen kOw "Creation Detail Window : "${file}" ..."
       kAddLog kOw  "Creation Detail Window : "${file}" ..."
       mkdtw  ${file}
   done
fi
#
echo ${star3}
#
if [ -f $KWIN/mgw.list ]
then
    kAddScreen kOw "Creation Malfunction Group Window : mgw ..."
    kAddLog kOw "Creation Malfunction Group Window : mgw ..."
    mkstaz2 mgw
else
    echo "Warning : File $KWIN/mgw.list not found"
fi
#
echo ${star3}
echo ${star3} >> $KLOG/kOw.log
#
kAddScreen kOw End 
kAddLog kOw End
echo "$star"
echo "$star" >> $KLOG/kOw.log
echo "Log file : $KLOG/kOw.log"
