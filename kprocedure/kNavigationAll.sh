#!/bin/ksh
#
key=$1
cd $LEGOCAD_USER/legocad
ls -d r_* | while read task
do
cd $task
echo ${star5}
echo $task : Creating Navigation capability
echo ${star5}
. $KBIN/kNavigation -Parent ${key}
cd $LEGOCAD_USER/legocad
done
print "\n\a"
