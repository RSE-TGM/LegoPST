#!/bin/ksh
#
cd $LEGOCAD_USER/legocad
ls -d r_* | while read task
do
cd $task
echo ${star5}
echo $task : Cleaning rego task $1
echo ${star5}
. $KBIN/kPulirRego
cd $LEGOCAD_USER/legocad
done
print "\n\a"
