#!/bin/ksh
#
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
if [ -f $KLOG/kFind.log ]
then
mv $KLOG/kFind.log $KLOG/kFind.log.kold
fi
###############################################################################
echo ${star}
echo ${star} >> $KLOG/kFind.log
kAddScreen kFind Start
kAddLog kFind Start
kAddStatus kFind Start
kAddLog kLogger "Start kFind"
kAddStatistic kFind
###############################################################################
cd $KINFOTAG
rm -f KKS.list
rm -f TAG.list
rm -f REV.list
rm -f DESCR.list
rm -f LI.list
rm -f OW.list
rm -f RP.list
rm -f TASK.list
###############################################################################
print "\n\n${star8}\nExtraction tags and descriptions from rego tasks\n${star8}\n"
cd $LEGOCAD_USER/legocad
ls -d r_* | while read task
do
cd $task
print "\n${star2}\n$task\n${star2}\n"
echo 1.- Find KKS
$KBIN/xFindKks
echo 2.- Find tag
$KBIN/xFindTag
echo 3.- Find revision
$KBIN/xFindRev
echo 4.- Find description
$KBIN/xFindDescr
echo 5.- Find task
$KBIN/xFindTask ${task}
cd $LEGOCAD_USER/legocad
done
###############################################################################
print "\n\n${star8}\nExtraction tags and descriptions from plant display\n${star8}\n"
cd ${KDISPLAY}/plant_display
ls -d ./*/pag | while read task
do
cd $task
print "\n${star2}\n$task\n${star2}\n"
echo 1.- Find Link Direct
. $KBIN/xFindLi
echo 2.- Find operating windows
. $KBIN/xFindOw
echo 3.- Find remote parameters
. $KBIN/xFindRp
echo 4.- Find plant display description
. $KBIN/xFindPdDescr
cd ${KDISPLAY}/plant_display
done
###############################################################################
print "\n\n$star"
echo "$star" >> $KLOG/kFind.log
kAddScreen kFind Started
kAddLog kFind Started
kAddStatus kFind Started
kAddLog kLogger "Started kFind"
echo "$star"
echo "$star" >> $KLOG/kFind.log
#
