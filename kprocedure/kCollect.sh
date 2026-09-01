#!/bin/ksh
#
clear
OPTION=$1
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
if [ -f $KLOG/kCollect.log ]
then
mv $KLOG/kCollect.log $KLOG/kCollect.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kCollect.log
kAddScreen kCollect Start
kAddLog kCollect Start
#
kclean
cd $KSIM
if [ "$OPTION" = "NoPage" ]
then
banner "NO PAGE"
kMakeGlobpages -g globpages -p NO -w o_win 
else
echo "WARNING : globpages will be removed !!!"
sleep 10
kMakeGlobpages -g globpages -w o_win
fi
cat kMakeGlobpages.log >> $KLOG/kCollect.log
rm -f kMakeGlobpages.log
kAddScreen kCollect "Multi MMI Configuration"
kAddLog kCollect "Multi MMI Configuration"
kMmiConfig
kAddScreen kCollect "Malfunction Interface Configuration"
kAddLog kCollect "Malfunction Interface Configuration"
#  Il glob va protetto: se in $KWIN non c'e' nessun file *mf (interfaccia
#  malfunzioni non configurata) la shell passa il pattern LETTERALE e
#  `ln -s` crea in $KSIM un link penzolante chiamato "*mf".
if ls $KWIN/*mf > /dev/null 2>&1
then
ln -fs $KWIN/*mf $KSIM
else
print "WARNING : nessun file *mf in $KWIN - interfaccia malfunzioni non configurata"
print "WARNING : nessun file *mf in $KWIN" >> $KLOG/kCollect.log
fi
###############################################################################
cd ${KSIM}
#cp al_sim.conf al_sim.conf.tmp
#grep "TITLE=" al_sim.conf.tmp | sed "s/TITLE=//g" | \
#                          awk  ' {print $1, $2} ' | while read name descr
#do
#if [ "${name}" = "" ]
#then
#name="Default_Name"
#fi
#if [ "${descr}" = "" ]
#then
#descr="Default_Description"
#fi
#echo "TITLE=${name} ${descr} `date +"%d/%m/%y"`" > al_sim.conf
#grep -v "TITLE=" al_sim.conf.tmp >> al_sim.conf
#rm al_sim.conf.tmp
#done
kAddInfo Simulator_Date `date +"%d/%m/%y"`
###############################################################################
kAddScreen kCollect End 
kAddLog kCollect End
echo "$star"
echo "$star" >> $KLOG/kCollect.log
echo "Log file : $KLOG/kCollect.log"
