#!/bin/ksh
#
###############################################################################
#	VERIFICATION START
###############################################################################
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
echo ${star}
if [ ! -f $KSIM/kMmi.cfg ]
then
echo "ERROR : $KSIM/kMmi.cfg not found"
exit
fi
#kStartStatus=`cat $KSTATUS/kStart.status`
#if [ ! "$kStartStatus" = "Started" ]
#then
#echo "ERROR : kStart not started"
#exit
#fi
###############################################################################
#	VERIFICATION END
###############################################################################
IdMmiSelectedDirect=$1
if [ "$IdMmiSelectedDirect"  = "" ]
then
echo "Mmi available on $HOST :"
cat $KSIM/kMmi.cfg | tr -s '\011' ' ' | tr -s ';' ' ' | while read IdMmi IdScada IdType MmiHost MmiUser
do
if [ "$MmiHost" = "$HOST" ] & [ "$MmiUser" = "$USER" ]
then
	if [ "$IdScada" -gt "4" ]
	then
	print "\n${star8}\nERROR\t\t: The scada identifier must be lower than 5"
	print "SOLUTION\t: 1.- mye $KSIM/kMmi.cfg\n\t\t  2.- Modify second column\n\t\t  3.- kMmiConfig\n${star8}\n"
	banner NOK
	print "\a\a\a"
	exit
	fi
	if [ "$IdType" = "I" ]
	then
	print "\t${IdMmi}-\tInstructor"
	elif [ "$IdType" = "O" ]
	then
	print "\t${IdMmi}-\tOperator"
	elif [ "$IdType" = "S" ]
	then
	print "\t${IdMmi}-\tOnly Scada"
	elif [ "$IdType" = "X" ]
	then
	print "\t${IdMmi}-\tSuper instructor"
	else
	echo "[${IdType}] not defined"
	banner NOK
	exit
	fi
fi
done
echo "Selection :"
read IdMmiSelected
else
IdMmiSelected=$IdMmiSelectedDirect
fi
if [ "$IdMmiSelected" = "" ]
then
print "SORRY : You have to answer with an available mmi identifier [not null].\a\a\n"
exit
fi
cat $KSIM/kMmi.cfg | tr -s '\011' ' ' | tr -s ';' ' ' | while read IdMmi IdScada IdType MmiHost MmiUser
do
if [ "$IdMmiSelected" = "$IdMmi" ]
then
IdTypeSelected=$IdType
IdScadaSelected=$IdScada
fi
done
touch $KSTATUS/kMmi_${IdMmiSelected}.status
chmod -f 777 $KSTATUS/kMmi_${IdMmiSelected}.status
kMmiStatus=`cat $KSTATUS/kMmi_${IdMmiSelected}.status`
if [ "$kMmiStatus" = "Start" ]
then
echo "kMmi ${IdMmiSelected} starting ..."
exit
fi
if [ "$kMmiStatus" = "Started" ]
then
echo "kMmi ${IdMmiSelected} already started"
exit
fi
if [ -r ${KPAGES}_${IdMmiSelected} ]
then
cd ${KPAGES}_${IdMmiSelected}
else
print "\nSORRY : You have to answer with an available mmi identifier [not ${IdMmiSelected}].\a\a\n"
exit
fi
kAddScreen kMmi_${IdMmiSelected} Start
kAddLog kMmi_${IdMmiSelected} Start
kAddStatus kMmi_${IdMmiSelected} Start
kAddLog kLogger "Start kMmi_${IdMmiSelected}"
kAddStatistic kMmi_${IdMmiSelected}
kMmiStatus=`cat $KSTATUS/kMmi_${IdMmiSelected}.status`
. kuser 77${IdScadaSelected}
	if [ "$IdTypeSelected" = "I" ]
	then
	print "\nConfiguration MMI INSTRUCTOR [${IdMmiSelected}]"
	export MMI_ULEVEL=1
	elif [ "$IdTypeSelected" = "O" ]
	then
	print "\nConfiguration MMI OPERATOR [${IdMmiSelected}]"
	export MMI_ULEVEL=0
	elif [ "$IdTypeSelected" = "S" ]
	then
	print "\nConfiguration MMI SCADA [${IdMmiSelected}]"
	export MMI_ULEVEL=0
	elif [ "$IdTypeSelected" = "X" ]
	then
	print "\nConfiguration MMI SUPER INSTRUCTOR [${IdMmiSelected}]"
	export MMI_ULEVEL=2
	else
	echo "[${IdTypeSelected}] not defined"
	banner NOK
	exit
	fi
#
rm -f ${KPAGES}_${IdMmiSelected}/LegoMMI.log
mmi &
#
sleep 10
while [ ! -f ${KPAGES}_${IdMmiSelected}/LegoMMI.log ]
do
#echo "..."
sleep 2
done
#
echo "$star"
echo "$star" >> $KLOG/kMmi_${IdMmiSelected}.log
kAddLog kMmi_${IdMmiSelected} Started
kAddStatus kMmi_${IdMmiSelected} Started
kAddLog kLogger "Started kMmi_${IdMmiSelected}"
sleep 2
kAddScreen kMmi_${IdMmiSelected} Started
echo "$star"
echo "$star" >> $KLOG/kMmi_${IdMmiSelected}.log
