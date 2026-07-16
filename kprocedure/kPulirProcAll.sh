#!/bin/ksh
#
cd $LEGOCAD_USER/legocad
ls ./*/macroblocks.dat | cut -f2 -d"/" | while read task
do
cd $task
echo ${star5}
echo $task : Cleaning process task $1
echo ${star5}
. $KBIN/kPulirProc
cd $LEGOCAD_USER/legocad
done
print "\n\a"
