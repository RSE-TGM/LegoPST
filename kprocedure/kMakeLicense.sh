#!/bin/ksh
#
Silent=$1
${KBIN}/kTest
KTEST=`cat $KSTATUS/kTest.status`
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit
fi
if [ "$Silent" = "" ]
then
	clear
	kpresentation
	if [ -f $KLOG/kMakeLicense.log ]
	then
	mv $KLOG/kMakeLicense.log $KLOG/kMakeLicense.log.kold
	fi
	echo ${star}
	echo ${star} >> $KLOG/kMakeLicense.log
	kAddScreen kMakeLicense Start
	kAddLog kMakeLicense Start
	kAddStatus kMakeLicense Start
	kAddLog kLogger "Start kMakeLicense"
	echo ${star}
	echo ${star} >> $KLOG/kMakeLicense.log
fi
kAddStatistic kMakeLicense
TCP_DEFAULT="128.1.0"
if [ ! -d $ALTERLEGO_CAP_DIR ]
then
mkdir $ALTERLEGO_CAP_DIR
else
	if [ ! -w $ALTERLEGO_CAP_DIR ]
	then
	echo "Sorry, the program can not access $ALTERLEGO_CAP_DIR directory !!"
	print "Solution : Assign ALTERLEGO_CAP_DIR environement variable to HOME/.license.\a"
	banner NOK
	exit
	fi
fi
BeginLine1="LAYVXFFFZBUYRYGYVXZYHQLYLWUWTQPYMPBUBALYWNPUUW"
BeginLine2="LAYVXFFFZBUYRYGYVXZYHGBYLWUWTQFYMFLULKBYWNFUUW"
BeginLine3="LKYVXPPFZBUYRYQYVXZYHQBYLWUWJGPYCPBUBKLYWDPUUW"
BeginLine4="BKYVXPFFZLUYHYQYVXZYHQBYBWUWTQPYMPLUBKLYWNPUUW"
BeginLine5="LAYVXPFFZLUYHYGYVXZYRGBYBWUWJQFYMPBUBALYWDPUUW"
BeginLine6="BAYVXFFPZBUYHYQYVXZYRGLYLWUWTQPYCFLULKLYWNPUUW"
EndLine1="ZYNUEAAOZUGJZNCDUDWPJAOVZTWTTV"
EndLine2="ZYDUOKZZZNGTZDMDUDWFTKEVZTWTJV"
EndLine3="ZYNUEKZFYWGJZDCDUNVIHZDUYJVTSV"
EndLine4="ZYDUEAKPYJGJZNMNUNWFJKOVZJWTTV"
EndLine5="UHWYXJJCIQGJZNCDUDWPTKEVZTWTJV"
EndLine6="ZYNUOKZYYJGTZDCNUNVSHZDUYTVJSV"
cd /etc
TCP_ACTUAL=`grep ^IFCONFIG_0= rc.config | cut -f2 -d"\"" | cut -f1-3 -d"."`
NetId=`grep ^IFCONFIG_0= rc.config | cut -f4 -d"." | \
       cut -f1 -d" " | tr -s '\011' ' ' | tr -d '[:space:]'`
if [ ! "$TCP_ACTUAL" = "$TCP_DEFAULT" ]
then
echo "Sorry, this program can only calculate license for ["$TCP_DEFAULT".X] TCP address type."
echo "(The value X must be included between 1 and 19.)"
print "The TCP address can not be ["$TCP_ACTUAL.$NetId"] .\a"
banner NOK
exit
fi
cd $KBIN
TagLine1=`grep ^"$NetId " kMakeLicenseList1.txt | cut -f2 -d" " | tr -s '\011' ' ' | tr -d '[:space:]'`
if [ "${TagLine1}" = "" ]
then
echo "Sorry, this program can not calulate license for [$TCP_ACTUAL.$NetId] address."
echo "It can only calculate license for ["$TCP_DEFAULT".X] TCP address type."
print "(The value X must be included between 1 and 19.)\a"
banner NOK
exit
fi
TagLine2=`grep ^"$NetId " kMakeLicenseList2.txt | cut -f2 -d" " | tr -s '\011' ' ' | tr -d '[:space:]'`
TagLine3=`grep ^"$NetId " kMakeLicenseList3.txt | cut -f2 -d" " | tr -s '\011' ' ' | tr -d '[:space:]'`
TagLine4=${TagLine3}
TagLine5=${TagLine1}
TagLine6=${TagLine3}
echo "${BeginLine1}${TagLine1}${EndLine1}" > $ALTERLEGO_CAP_DIR/CAP.conf
echo "${BeginLine2}${TagLine2}${EndLine2}" >> $ALTERLEGO_CAP_DIR/CAP.conf
echo "${BeginLine3}${TagLine3}${EndLine3}" >> $ALTERLEGO_CAP_DIR/CAP.conf
echo "${BeginLine4}${TagLine4}${EndLine4}" >> $ALTERLEGO_CAP_DIR/CAP.conf
echo "${BeginLine5}${TagLine5}${EndLine5}" >> $ALTERLEGO_CAP_DIR/CAP.conf
echo "${BeginLine6}${TagLine6}${EndLine6}" >> $ALTERLEGO_CAP_DIR/CAP.conf
print "TCP\t: [$TCP_ACTUAL.$NetId]"
print "TCP\t: [$TCP_ACTUAL.$NetId]" >> $KLOG/kMakeLicense.log
print "Output File\t: $ALTERLEGO_CAP_DIR/CAP.conf" >> $KLOG/kMakeLicense.log
if [ "$Silent" = "" ]
then
	echo "$star"
	echo "$star" >> $KLOG/kMakeLicense.log
	kAddScreen kMakeLicense End
	kAddLog kMakeLicense End
	kAddStatus kMakeLicense End
	kAddLog kLogger "End kMakeLicense"
	echo "$star"
	echo "$star" >> $KLOG/kMakeLicense.log
	print "Log File\t: $KLOG/kMakeLicense.log"
	print "Output File\t: $ALTERLEGO_CAP_DIR/CAP.conf"
fi
