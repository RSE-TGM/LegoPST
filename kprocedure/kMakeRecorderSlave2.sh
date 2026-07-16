#!/bin/ksh
#
# $KGRAF/kRecorder_KKS.txt without empty line !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Variables extraction from kRecorder_KKS.txt
print "\n${star6}\nVariables extraction from kRecorder_KKS.txt\n${star6}\n"
print "\n${star6}\nVariables extraction from kRecorder_KKS.txt\n${star6}\n" >> $KLOG/kMakeRecorder.log
#
grep "^P " $KSIM/al_sim.conf | while read id TaskName TaskDirectory TaskDescription
do
echo "Process variables added to lego task ${TaskName} :"
	cat $KGRAF/kRecorder_KKS.txt | while read kks
	do
	grep -w ${kks} $LEGOCAD_USER/legocad/$TaskDirectory/variabili.edf | grep "\-\-" | cut -f2 -d" " | uniq
	grep -w ${kks} $LEGOCAD_USER/legocad/$TaskDirectory/variabili.edf | grep "\-\-" | cut -f2 -d" " | uniq >> $KSIM/kRecorderSupportDirectory/${TaskName}
	done
done
#
grep "^N " $KSIM/al_sim.conf | while read id TaskName TaskDirectory TaskDescription
do
echo "Process variables added to sid task ${TaskName} :"
	cat $KGRAF/kRecorder_KKS.txt | while read kks
	do
	cat $LEGOCAD_USER/legocad/$TaskDirectory/proc/n04.dat | tr -s '\011' ' ' | grep -w ${kks} | cut -f1 -d" " | uniq
	cat $LEGOCAD_USER/legocad/$TaskDirectory/proc/n04.dat | tr -s '\011' ' ' | grep -w ${kks} | cut -f1 -d" " | uniq >> $KSIM/kRecorderSupportDirectory/${TaskName}
	done
done
