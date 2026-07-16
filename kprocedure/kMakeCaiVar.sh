#!/bin/ksh
#
clear
$KBIN/kTest
KTEST=`cat $KSTATUS/kTest.status `
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit
fi
kpresentation
if [ -f $KLOG/kMakeCaiVar.log ]
then
mv $KLOG/kMakeCaiVar.log $KLOG/kMakeCaiVar.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kMakeCaiVar.log
kAddScreen kMakeCaiVar Start
kAddLog kMakeCaiVar Start
#
clean
cd $KSIM
rm -f $KSIM/CAI_VAR_DB.*
kAddScreen kMakeCaiVar "CaiVar Database Elaboration"
kAddLog kMakeCaiVar "CaiVar Database Elaboration"
leggi_cai_var.sh
kAddScreen kMakeCaiVar "Plant Display CaiVar Elaboration"
kAddLog kMakeCaiVar "Plant Display CaiVar Elaboration"
elab_cai_var.sh
rm -f CaiVar.scr
rm -f leggi_cai_var.log
rm -f $KWIN/*.elencocai
rm -f $KWIN/M_S_*.scr
#
kAddScreen kMakeCaiVar End 
kAddLog kMakeCaiVar End
echo "$star"
echo "$star" >> $KLOG/kMakeCaiVar.log
echo "Log file : $KLOG/kMakeCaiVar.log"
