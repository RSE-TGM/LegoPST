#!/bin/ksh
#
# Recorder.edf.new elaboration
print "\n${star6}\nRecorder.edf.new elaboration\n${star6}\n"
print "\n${star6}\nRecorder.edf.new elaboration\n${star6}\n" >> $KLOG/kMakeRecorder.log
cd $KSIM/kRecorderSupportDirectory
ls | while read file
do
echo "Sort ${file}"
sort ${file} | uniq > ${file}.new
mv ${file}.new ${file}
done
#
echo "****" > $KSIM/recorder.edf.new
echo "NOME $KDESCR" >> $KSIM/recorder.edf.new
echo "****" >> $KSIM/recorder.edf.new
echo "NOME $KREV" >> $KSIM/recorder.edf.new
echo "AUTORE $USER" >> $KSIM/recorder.edf.new
echo "VERSIONE $KREVDATE" >> $KSIM/recorder.edf.new
echo "****" >> $KSIM/recorder.edf.new
grep ^". " $KSIM/al_sim.conf | while read Id Modello Description
do
echo "MODELLO $Modello" >> $KSIM/recorder.edf.new
cat $KSIM/kRecorderSupportDirectory/$Modello >> $KSIM/recorder.edf.new
echo "****" >> $KSIM/recorder.edf.new
done
if [ -f $KSIM/recorder.edf_old ]
then
OldVariablesNumberInRecorder=`cat $KSIM/recorder.edf_old | wc -l | tr -d '[:space:]'`
else
OldVariablesNumberInRecorder="0"
fi
VariablesNumberInRecorder=`cat $KSIM/recorder.edf.new | wc -l | tr -d '[:space:]'` 
VariablesNumberInRecorderMax=`grep "Simulator.NUM_VAR" $KSIM/Simulator | cut -f2 -d: | tr -d '[:space:]'`
if [ "$VariablesNumberInRecorder" -gt "$VariablesNumberInRecorderMax" ]
then
banner NOK
banner NOK >> $KLOG/kMakeRecorder.log
print "\a"
fi
print "\nVariables number in recorder.edf :\t\t [$VariablesNumberInRecorder]\t($OldVariablesNumberInRecorder)"
print "Maximum variables number in recorder.edf :\t [$VariablesNumberInRecorderMax]\n"
print "\nVariables number in recorder.edf :\t\t [$VariablesNumberInRecorder]\t($OldVariablesNumberInRecorder)" >> $KLOG/kMakeRecorder.log
print "Maximum variables number in recorder.edf :\t [$VariablesNumberInRecorderMax]\n" >> $KLOG/kMakeRecorder.log
