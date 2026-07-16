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
#
type=$1
if [ "${type}" = "Regolation" ]
then
typeconfig=compreg
elif [ "${type}" = "Task" ]
then
typeconfig=creatask
elif [ "${type}" = "Page" ]
then
typeconfig=compall
else
print "Correct Use :\n\tkCompile  Regolation/Task/Page  All/Sim/Local\a"
exit
fi
#
donde=$2
if [ "${donde}" = "All" ]
then
cd $LEGOCAD_USER/legocad
REGO_TASK_LIST=`ls -d r_*`
elif [ "${donde}" = "Sim" ]
then
REGO_TASK_LIST=`grep ^R $KSIM/al_sim.conf | tr -s '\011' ' ' |  cut -f3 -d" "`
elif [ "${donde}" = "Local" ]
then
REGO_TASK_LIST=`pwd`
else
print "Correct Use :\n\tkCompile  Regolation/Task/Page  All/Sim/Local\a"
exit
fi
#
if [ -f $KLOG/kCompile${type}.log ]
then
mv $KLOG/kCompile${type}.log $KLOG/kCompile${type}.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kCompile${type}.log
kAddScreen kCompile${type} Start
kAddLog kCompile${type} Start
echo ${star}
echo ${star} >> $KLOG/kCompile${type}.log
#
echo ${star5}
echo "Task list :"
echo ${REGO_TASK_LIST}
echo ${star5}
cd $LEGOCAD_USER/legocad
for task in ${REGO_TASK_LIST}
do
clean
banner "$task"
echo "$task"
cd $task
ls | grep err | while read poubelle
do
rm  ${poubelle}
done
rm -f net_compi.out
print "\n${star5}\n$task : Compile $type\n${star5}"
print "\n${star5}\n$task : Compile $type\n${star5}" >> $KLOG/kCompile${type}.log
export DEBUG=NO
kconfig -c ${typeconfig}
export DEBUG=YES
###############################################################################
if [ "${type}" = "Regolation" ]
then
if [ -f *.reg_err ]
then
regopagenumber=`ls *.reg_err | wc -w`
else
regopagenumber=0
fi
print "\n\t * Rego pages number compilied : "${regopagenumber}"\n"
print "\n\t * Rego pages number compilied : "${regopagenumber}"\n" >> $KLOG/kCompile${type}.log
	if [ -f *.reg_err ]
	then
	ls *.reg_err | while read errorpage
	do
	errlinenumber=`cat ${errorpage} | wc -l`
	if [ "${errlinenumber}" -gt "5" ]
	then
	print "\t * ERROR in Rego page "${errorpage}"\n\a"
	print "\t * ERROR in Rego page "${errorpage}"\n\a" >> $KLOG/kCompile${type}.log
	echo "$star2" >> $KLOG/kCompile${type}.log
#	cat ${errorpage} >> $KLOG/kCompile${type}.log
	echo "$star2" >> $KLOG/kCompile${type}.log
	fi
	done
	fi
fi
###############################################################################
if [ "${type}" = "Task" ]
then
	if [ -f *.reg_err ]
	then
	errlinenumber=`cat *.reg_err | wc -l`
	if [ "${errlinenumber}" -gt "5" ]
	then
	print "\t * ERROR during f01.dat & f14.dat generation"\n\a"
	print "\t * ERROR during f01.dat & f14.dat generation"\n\a" >> $KLOG/kCompile${type}.log
	fi
	fi
if [ -f net_compi.out ]
then
status=`grep Fine net_compi.out | awk '{ print $2 }'`
if [ ${status}="regolare" ]
then
time=`grep Fine net_compi.out | awk '{ print $5, $6 }'`
print "\n\t * Succesful end of the program in ${time}\n"
print "\n\t * Succesful end of the program in ${time}\n" >> $KLOG/kCompile${type}.log
else
print "\t * ERROR in net_compi"\n\a"
print "\t * ERROR in net_compi"\n\a" >> $KLOG/kCompile${type}.log
fi
else
print "\t * ERROR : File net_compi.out not found\n\a"
print "\t * ERROR : File net_compi.out not found\n\a" >> $KLOG/kCompile${type}.log
fi
fi
###############################################################################
if [ "${type}" = "Page" ]
then
export pagepagenumber=`ls *.rtf_err | wc -l`
print "\n\t * Animated pages number compilied : "${pagepagenumber}"\n"
print "\n\t * Animated pages number compilied : "${pagepagenumber}"\n" >> $KLOG/kCompile${type}.log
if [ `ls *.rtf_err | wc -l` -ge "1" ]
then
   ls *.rtf_err | while read errorpage
   do
      errlinenumber=`cat ${errorpage} | wc -l`
      if [ "${errlinenumber}" -gt "5" ]
      then
         print "\t * ERROR in Animated page "${errorpage}"\n\a"
         print "\t * ERROR in Animated page "${errorpage}"\n\a" >> $KLOG/kCompile${type}.log
      fi
   done
fi
fi
###############################################################################
cd $LEGOCAD_USER/legocad
done
#
echo "$star"
echo "$star" >> $KLOG/kCompile${type}.log
kAddScreen kCompile${type} End 
kAddLog kCompile${type} End
echo "$star"
echo "$star" >> $KLOG/kCompile${type}.log
#
errtot=`grep -w ERROR $KLOG/kCompile${type}.log | wc -l`
print "\n${star5}"
echo "${star5}" >> $KLOG/kCompile${type}.log
if [ "${errtot}" -gt "0" ]
then
banner "NOK"
print "\t${errtot} ERRORS found during compilation"
banner "NOK" >> $KLOG/kCompile${type}.log
print "\t${errtot} ERRORS found during compilation" >> $KLOG/kCompile${type}.log
else
print "\tCompile ${type} : OK"
fi
echo "${star5}" >> $KLOG/kCompile${type}.log
print "\tLog File : ${KLOG}/kCompile${type}.log\n${star5}\a"
