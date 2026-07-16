#!/bin/ksh
#
export PAGMOD=pagmod
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
if [ ! -f ${KSIM}/variabili.edf  ]
then
print "File ${KSIM}/variabili.edf not found !!! \a"
banner "NOK"
exit
fi
kpresentation
if [ -f $KLOG/kUpDateNavigation.log ]
then
mv $KLOG/kUpDateNavigation.log $KLOG/kUpDateNavigation.log.kold
fi
kAddScreen kUpDateNavigation Start
kAddLog kUpDateNavigation Start
#
mode=$1
type=$2
script=$3
#
if [ "$type" = "-Child" ]
then
echo "Please not use the Child configuration"
exit
fi
#
if [ "$mode" = "-i" -o "$mode" = "-b" ]
then
echo "Mode = $mode" >> $KLOG/kUpDateNavigation.log
else
print "\n\tCorrect Use : kUpDateNavigation  -b/-i  -Parent/-Child  [ -script ]\n\a"
exit 
fi
if [ "$type" = "-Parent" -o "$mode" = "-Child" ]
then
echo "Type = $type" >> $KLOG/kUpDateNavigation.log
else
print "\n\tCorrect Use : kUpDateNavigation  -b/-i  -Parent/-Child  [ -script ]\n\a"
exit
fi
if [ ! -f $KSIM/al_sim.conf ]
then
echo File $KSIM/al_sim.conf not found
banner "NOK"
exit
fi
if [ ! -f $KSIM/S01 ]
then
echo File $KSIM/S01 not found
banner "NOK"
exit
fi
NAVIGATION_ALL=`grep ^R $KSIM/al_sim.conf | tr -s '\011' ' ' |  cut -f3 -d" "`
if [ ! -f $KSIM/kUpDateNavigation.list ]
then
grep ^R $KSIM/al_sim.conf | tr -s '\011' ' ' |  cut -f3 -d" " > $KSIM/kUpDateNavigation.list
fi
NAVIGATION_SELECTION=`grep r_ $KSIM/kUpDateNavigation.list`
###################################################################################################
#	Interactive Mode
###################################################################################################
if [ "$mode" = "-i" ]
then
export LUNG_kUpDateNavigationlist="`cat kUpDateNavigation.list | wc -l`"
if [ ${LUNG_kUpDateNavigationlist} -gt 15 ]
then
print "\aWarning Task Number > 15"
exit
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 1 ]
then
export NAVIGATION_TASK1=`head -n 1 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 2 ]
then
export NAVIGATION_TASK2=`head -n 2 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 3 ]
then
export NAVIGATION_TASK3=`head -n 3 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 4 ]
then
export NAVIGATION_TASK4=`head -n 4 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 5 ]
then
export NAVIGATION_TASK5=`head -n 5 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 6 ]
then
export NAVIGATION_TASK6=`head -n 6 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 7 ]
then
export NAVIGATION_TASK7=`head -n 7 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 8 ]
then
export NAVIGATION_TASK8=`head -n 8 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 9 ]
then
export NAVIGATION_TASK9=`head -n 9 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 10 ]
then
export NAVIGATION_TASK10=`head -n 10 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 11 ]
then
export NAVIGATION_TASK11=`head -n 11 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 12 ]
then
export NAVIGATION_TASK12=`head -n 12 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 13 ]
then
export NAVIGATION_TASK13=`head -n 13 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 14 ]
then
export NAVIGATION_TASK14=`head -n 14 $KSIM/kUpDateNavigation.list | tail -n 1`
fi
if [ ${LUNG_kUpDateNavigationlist} -ge 15 ]
then
export NAVIGATION_TASK15=`head -n 15 $KSIM/kUpDateNavigation.list | tail -n 1`
fi

cat <<EOF

		$0
		-------------------------

		1.- Help
		2.- All simulator tasks or Selection task from kUpDateNavigation.list
		3.- ${NAVIGATION_TASK1}
		4.- ${NAVIGATION_TASK2}
		5.- ${NAVIGATION_TASK3}
		6.- ${NAVIGATION_TASK4}
		7.- ${NAVIGATION_TASK5}
		8.- ${NAVIGATION_TASK6}
		9.- ${NAVIGATION_TASK7}
		10.- ${NAVIGATION_TASK8}
		11.- ${NAVIGATION_TASK9}
		12.- ${NAVIGATION_TASK10}
		13.- ${NAVIGATION_TASK11}
		14.- ${NAVIGATION_TASK12}
		15.- ${NAVIGATION_TASK13}
		16.- ${NAVIGATION_TASK14}
		17.- ${NAVIGATION_TASK15}

		0.- Exit


EOF
print "\tSelection ==> \c"
read option
case $option in
     0) exit;
      ;;
     1) if [ -f $KHELP/kUpDateNavigationHELP1${KLANG} ]
        then
		$KHELP/kUpDateNavigationHELP1${KLANG}
        	exit
	else
		echo Help not yet implemented!
		exit
	fi
      ;;
     2) NAVIGATION_TASK_LIST=${NAVIGATION_SELECTION}
      ;;
     3) NAVIGATION_TASK_LIST=${NAVIGATION_TASK1}
      ;;
     4) NAVIGATION_TASK_LIST=${NAVIGATION_TASK2}
      ;;
     5) NAVIGATION_TASK_LIST=${NAVIGATION_TASK3}
      ;;
     6) NAVIGATION_TASK_LIST=${NAVIGATION_TASK4}
      ;;
     7) NAVIGATION_TASK_LIST=${NAVIGATION_TASK5}
      ;;
     8) NAVIGATION_TASK_LIST=${NAVIGATION_TASK6}
      ;;
     9) NAVIGATION_TASK_LIST=${NAVIGATION_TASK7}
      ;;
     10) NAVIGATION_TASK_LIST=${NAVIGATION_TASK8}
      ;;
     11) NAVIGATION_TASK_LIST=${NAVIGATION_TASK9}
      ;;
     12) NAVIGATION_TASK_LIST=${NAVIGATION_TASK10}
      ;;
     13) NAVIGATION_TASK_LIST=${NAVIGATION_TASK11}
      ;;
     14) NAVIGATION_TASK_LIST=${NAVIGATION_TASK12}
      ;;
     15) NAVIGATION_TASK_LIST=${NAVIGATION_TASK13}
      ;;
     16) NAVIGATION_TASK_LIST=${NAVIGATION_TASK14}
      ;;
     17) NAVIGATION_TASK_LIST=${NAVIGATION_TASK15}
      ;;
esac
###################################################################################################
#	Background Mode
###################################################################################################
else
NAVIGATION_TASK_LIST=${NAVIGATION_ALL}
fi
###################################################################################################
print "\n${star5}\nTasks list :\n${NAVIGATION_TASK_LIST}\n${star5}"
print "\n${star5}\nTasks list :\n${NAVIGATION_TASK_LIST}\n${star5}" >> $KLOG/kUpDateNavigation.log
kAddScreen kUpDateNavigation "Database creation : Start"
kAddLog kUpDateNavigation "Database creation : Start"
grep BLOCCO $KSIM/variabili.edf | \
	tr -s '\011' ' ' | \
	awk '{ print $2, $3 }' | \
	sed "s/BL.-//g" | \
	cut -c 5-13  > $KSIM/kUpDateNavigation.DB
kAddScreen kUpDateNavigation "Database creation : End"
kAddLog kUpDateNavigation "Database creation : End"
for task in ${NAVIGATION_TASK_LIST}
do
	if [ -d $LEGOCAD_USER/legocad/${task} ]
	then
	print "\n${star3}\n\t${task}\n${star3}\n"
	print "\n${star3}\n\t${task}\n${star3}\n" >> $KLOG/kUpDateNavigation.log
	cd $LEGOCAD_USER/legocad/${task}
	rm -f *scr
	if [ "${type}" = "-Child" ]
	then
	. kUpDateNavigationSlave1
	else
#	. kUpDateNavigationSlave2
	. kUpDateNavigationSlave2 NOLIMPIAR

	fi
	ScriptNumber=` ls *scr | wc -w `
	if [ ! "${script}" = "-script" ]
	then
	if [ ! "${ScriptNumber}" -eq "0" ]
	then
		print "\n${star5}\n"
		kAddScreen kUpDateNavigation "Start $PAGMOD"
		print "\n${star5}\n" >> $KLOG/kUpDateNavigation.log
		kAddLog kUpDateNavigation "Start $PAGMOD"
		echo Processing ${ScriptNumber} scripts ...
		ls *scr | while read script
		do
		pagemod=`echo ${script} | sed "s/.scr//g" `
		$PAGMOD -s ${script}
		mv ${pagemod}.mod ${pagemod}
		rm ${script}
		done
		kAddScreen kUpDateNavigation "End $PAGMOD"
		print "\n${star5}"
		kAddLog kUpDateNavigation "End $PAGMOD"
		print "\n${star5}" >> $KLOG/kUpDateNavigation.log
	else
	echo Nothing to do
	fi
	else
	print "\n${star5}\n"
	kAddScreen kUpDateNavigation "WARNING : ONLY SCRIPT ELABORATION"
	print "\n${star5}\n" >> $KLOG/kUpDateNavigation.log
	kAddLog kUpDateNavigation "WARNING : ONLY SCRIPT ELABORATION"
	fi
	else
	echo Directory $LEGOCAD_USER/legocad/${task} not found
	fi
done 
kAddScreen kUpDateNavigation End
kAddLog kUpDateNavigation End
echo "Log File : $KLOG/kUpDateNavigation.log"
