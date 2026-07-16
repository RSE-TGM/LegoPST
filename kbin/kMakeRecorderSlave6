#!/bin/ksh
#
print "\n${star6}\nVariables extraction from f22fgraf.edf\n${star6}\n"
print "\n${star6}\nVariables extraction from f22fgraf.edf\n${star6}\n" >> $KLOG/kMakeRecorder.log
cd $KSIM
strings f22_fgraf.edf | grep "BL." | cut -f1 -d" " | sort -r | uniq | tr -s '[:space:]' > f22fgraf.txt
grep "P " $KSIM/al_sim.conf | while read id TaskName TaskDirectory TaskDescription
do
echo "Process variables added to task $TaskName :"
grep -wf $KSIM/f22fgraf.txt $LEGOCAD_USER/legocad/$TaskDirectory/variabili.edf | grep "\-\-" | cut -f2 -d" " | sort | uniq
grep -wf $KSIM/f22fgraf.txt $LEGOCAD_USER/legocad/$TaskDirectory/variabili.edf | grep "\-\-" | cut -f2 -d" " | sort | uniq >> $KSIM/kRecorderSupportDirectory/${TaskName}
done
rm -f f22fgraf.txt

