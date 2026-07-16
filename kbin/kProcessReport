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
if [ -f $KLOG/kProcessReport.log ]
then
mv $KLOG/kProcessReport.log $KLOG/kProcessReport.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kProcessReport.log
kAddScreen kProcessReport Start
kAddLog kProcessReport Start
#
rm -f $KINFO/kProcessReport.info
#
cd $LEGOCAD_USER/legocad
ls ./*/macroblocks.dat | cut -f2 -d"/" | while read task
do
cd $task
if [ ! -f f14.dat ]
then
print "ERROR\t: File $PWD/f14.dat not found"
print "ERROR\t: File $PWD/f14.dat not found" >> $KLOG/kProcessReport.log
banner "NOK"
banner "NOK" >> $KLOG/kProcessReport.log
print "\a\a\a"
exit
fi
echo ${star5}
echo $task : Process task $1
echo ${star5}
#
awk  '/DATI FISICI E GEOMETRICI/,/EOF/' f14.dat | grep -v GEOMETRICI | grep -v EOF | while read line
do
IdBlockLine=`echo $line | grep BLOCCO | wc -l | tr -s '\011' ' ' | tr  -d '[:space:]'`
if [ "$IdBlockLine" = "1" ]
then
echo "$SUPERLINE" >> $KINFO/kProcessReport.info
BLOCK=`echo "$line" | cut -c25-28`
MODULE=`echo "$line" | cut -c38-41`
KKS=`echo "$line" | cut -c45-200 | sed "s/@#K@//g" | sed "s/-/ /g" | tr -s '\011' ' ' | sed "s/ /-/g"`
SUPERLINE=`echo "$BLOCK;$MODULE;$KKS"`
else
IdEqual=`echo "$line" | cut -c10`
	if [ "$IdEqual" = "=" ]
	then
	DATO1=`echo "$line" | cut -c11-20`
	DATO2=`echo "$line" | cut -c36-45`
	DATO3=`echo "$line" | cut -c61-70`
	SUPERLINE=`echo "$SUPERLINE;$DATO1;$DATO2;$DATO3;"`
	fi
fi
SUPERLINE=`echo "$SUPERLINE" | tr -s '\011' ' ' | tr -d '[:space:]'`
done
#
cd $LEGOCAD_USER/legocad
done
print "\n\a"
#
sort -o $KINFO/kProcessReport.info $KINFO/kProcessReport.info
#
kAddScreen kProcessReport End
kAddLog kProcessReport End
print "Log File :\t$KLOG/kProcessReport.log"
echo "$star"
echo "$star" >> $KLOG/kProcessReport.log
echo "Log file :" $KLOG/kProcessReport.log
echo "Output file :" $KINFO/kProcessReport.info


