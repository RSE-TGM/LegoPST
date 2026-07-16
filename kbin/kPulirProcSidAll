#!/bin/ksh
#
cd $LEGOCAD_USER/legocad
ls ./*/task.info | cut -f2 -d"/" | while read task
do
cd $task
echo ${star5}
echo $task : Cleaning SID process task $1
echo ${star5}
. $KBIN/kPulirProcSid
cd $LEGOCAD_USER/legocad
done
print "\n\a"
