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
if [ -f $KLOG/kMakeRecorder.log ]
then
mv $KLOG/kMakeRecorder.log $KLOG/kMakeRecorder.log.kold
fi
cd ${KSIM}
if [ ! -f al_sim.conf ]
then
print "-->> Error : File al_sim.conf not found \a"
banner NOK
exit
fi
if [ ! -f $KGRAF/kksgrafi.rtf ]
then
print "-->> Error : File $KGRAF/kksgrafi.rtf not found \a"
banner NOK
exit
fi
cp $KGRAF/kksgrafi.rtf $KGRAF/recorder.txt
echo ${star}
echo ${star} >> $KLOG/kMakeRecorder.log
kAddScreen kMakeRecorder Start
kAddLog kMakeRecorder Start
kAddStatus kMakeRecorder Start
kAddStatistic kMakeRecorder
#
if [ -f recorder.edf ]
then
echo "Copy recorder.edf in recorder.edf_old"
cp recorder.edf recorder.edf_old
fi
rm -rf $KSIM/kRecorderSupportDirectory
mkdir $KSIM/kRecorderSupportDirectory
cp $KGRAF/recorder.txt $KGRAF/recorder.txt.kold
cut -f1 -d. $KGRAF/recorder.txt.kold > $KGRAF/recorder.txt
grep '[012BMX][A-Z]..[0-9]......' $KGRAF/recorder.txt > $KGRAF/kRecorder_KKS.txt
grep -v '[012BMX][A-Z]..[0-9]......' $KGRAF/recorder.txt > $KGRAF/kRecorder_VAR.txt
###############################################################################
${KBINSLAVE}/kMakeRecorderSlave1
${KBINSLAVE}/kMakeRecorderSlave2
${KBINSLAVE}/kMakeRecorderSlave3
${KBINSLAVE}/kMakeRecorderSlave4
${KBINSLAVE}/kMakeRecorderSlave6
${KBINSLAVE}/kMakeRecorderSlave5
###############################################################################
cd $KSIM
mv recorder.edf.new recorder.edf
echo "New recorder file : recorder.edf"
rm -f recorder.rtf
rm -f f22circ.dat
rm -rf $KSIM/kRecorderSupportDirectory
echo ${star5} >> $KLOG/kMakeRecorder.log
kAddScreen kMakeRecorder End 
kAddLog kMakeRecorder End
kAddStatus kMakeRecorder End
echo "$star"
echo "$star" >> $KLOG/kMakeRecorder.log
echo "Log file : $KLOG/kMakeRecorder.log"
