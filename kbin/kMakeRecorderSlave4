#!/bin/ksh
#
# $KGRAF/kRecorder_VAR.txt without empty line !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Variables extraction from kRecorder_VAR.txt
print "\n${star6}\nVariables extraction from kRecorder_VAR.txt\n${star6}\n"
print "\n${star6}\nVariables extraction from kRecorder_VAR.txt\n${star6}\n" >> $KLOG/kMakeRecorder.log
grep "P " $KSIM/al_sim.conf | while read id TaskName TaskDirectory TaskDescription
do
echo "Specific process variables added to task $TaskName :"
grep -wf $KGRAF/kRecorder_VAR.txt $LEGOCAD_USER/legocad/$TaskDirectory/variabili.edf | grep "\-\-" | cut -f2 -d" " | sort | uniq
grep -wf $KGRAF/kRecorder_VAR.txt $LEGOCAD_USER/legocad/$TaskDirectory/variabili.edf | grep "\-\-" | cut -f2 -d" " | sort | uniq >> $KSIM/kRecorderSupportDirectory/${TaskName}
done
