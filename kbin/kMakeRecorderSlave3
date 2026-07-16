#!/bin/ksh
#
grep Info $KWIN/*GR.pag | tr -s '\011' ' '| cut -f2 -d" " | cut -d"|" -f1 | grep -v LEGOname | sort | uniq > $KSIM/kRecorderSupportDirectory/kMakeRecorderSlave3.list
print "\n${star6}\nVariables extraction from Curve Displays\n${star6}\n"
print "\n${star6}\nVariables extraction from Curve Displays\n${star6}\n" >> $KLOG/kMakeRecorder.log
grep "R " $KSIM/al_sim.conf | while read id TaskName TaskDirectory TaskDescription
do
echo "Automation variables added to task $TaskName :"
TagId=`echo ${TaskName} | cut -c1-2 `
grep ......${TagId} $KSIM/kRecorderSupportDirectory/kMakeRecorderSlave3.list
grep ......${TagId} $KSIM/kRecorderSupportDirectory/kMakeRecorderSlave3.list >> $KSIM/kRecorderSupportDirectory/${TaskName}
done
rm -f $KSIM/kRecorderSupportDirectory/kMakeRecorderSlave3.list
