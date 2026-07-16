#!/bin/ksh
#
clear
${KBIN}/kTest
KTEST=`cat $KSTATUS/kTest.status`
echo "kTest result : $KTEST"
if [ ! $KTEST = "OK" ]
then
echo "Environement test not succesfull"
banner NOK
exit
fi
kpresentation
if [ -f $KLOG/kExport.log ]
then
mv $KLOG/kExport.log $KLOG/kExport.log.kold
fi
kAddScreen kExport Start
kAddLog kExport Start
###############################################################################
if [ ! -r $KEXPORT ]
then
echo "Creating $KEXPORT directory ... "
mkdir $KEXPORT
fi
if [ ! -r $KGRAF ]
then
echo "Creating $KGRAF directory ... "
mkdir $KGRAF
fi
if [ ! -f $KEXPORT/ALARM.txt ]
then
print "${star3}\n"
echo "$KEXPORT/ALARM.txt not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
if [ ! -f $KEXPORT/TAG.txt ]
then
print "${star3}\n"
echo "$KEXPORT/TAG.txt not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
if [ ! -f $KEXPORT/TAGS.txt ]
then
print "${star3}\n"
echo "$KEXPORT/TAGS.txt not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
if [ ! -f $KEXPORT/component.mf ]
then
print "${star3}\n"
echo "$KEXPORT/component.mf not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
if [ ! -f $KEXPORT/malf_set.mf ]
then
print "${star3}\n"
echo "$KEXPORT/malf_set.mf not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
if [ ! -f $KEXPORT/tipo_comp_malf.mf ]
then
print "${star3}\n"
echo "$KEXPORT/tipo_comp_malf.mf not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
if [ ! -f $KEXPORT/sosti.dat ]
then
print "${star3}\n"
echo "$KEXPORT/sosti.dat not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
if [ ! -f $KEXPORT/mgw.con ]
then
print "${star3}\n"
echo "$KEXPORT/mgw.con not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
if [ ! -f $KEXPORT/mgw.des ]
then
print "${star3}\n"
echo "$KEXPORT/mgw.des not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
if [ ! -f $KEXPORT/cai_var.dat ]
then
print "${star3}\n"
echo "$KEXPORT/cai_var.dat not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
if [[ ! -f $KEXPORT/N_winDGWM.tab && ! -f $KEXPORT/N_winDGWI.tab ]]
then
echo "$KEXPORT/N_win*.tab not found ... "
banner NOK
exit
fi
if [ ! -f $KEXPORT/vargraf.txt ]
then
print "${star3}\n"
echo "$KEXPORT/vargraf.txt not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
if [ ! -f $KEXPORT/kksgrafi.txt ]
then
print "${star3}\n"
echo "$KEXPORT/kksgrafi.txt not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
if [ ! -f $KEXPORT/grugraf.txt ]
then
print "${star3}\n"
echo "$KEXPORT/grugraf.txt not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
if [ ! -f $KEXPORT/R20.txt ]
then
print "${star3}\n"
echo "$KEXPORT/R20.txt not found ... "
print "\n${star3}\n"
#banner NOK
#exit
fi
ls -C1 $KEXPORT >> $KLOG/kExport.log
cd $KEXPORT
kControlM
###############################################################################
if [ ! -r $KSIM/scada ]
then
echo "Creating $KSIM/scada directory ... "
mkdir $KSIM/scada
fi
if [ ! -r $KSTART_TABLES ]
then
echo "Creating $KSTART_TABLES directory ... "
mkdir $KSTART_TABLES
fi
print "${star5}\n\tALARM.txt\n${star5}\n"
if [ -f $KSTART_TABLES/ALARM.txt ]
then
echo Rename $KSTART_TABLES/ALARM.txt in ALARM.txt.kold
mv $KSTART_TABLES/ALARM.txt $KSTART_TABLES/ALARM.txt.kold
fi
echo Copy export/ALARM.txt in $KSTART_TABLES
cp $KEXPORT/ALARM.txt $KSTART_TABLES/ALARM.txt
###############################################################################
print "${star5}\n\tTAG.txt\n${star5}\n"
if [ -f $KSTART_TABLES/TAG.txt ]
then
echo Rename $KSTART_TABLES/TAG.txt in TAG.txt.kold
mv $KSTART_TABLES/TAG.txt $KSTART_TABLES/TAG.txt.kold
fi
echo Copy export/TAG.txt in $KSTART_TABLES
cp $KEXPORT/TAG.txt $KSTART_TABLES/TAG.txt
###############################################################################
print "${star5}\n\tTAGS.txt\n${star5}\n"
if [ -f $KSTART_TABLES/TAGS.txt ]
then
echo Rename $KSTART_TABLES/TAGS.txt in TAGS.txt.kold
mv $KSTART_TABLES/TAGS.txt $KSTART_TABLES/TAGS.txt.kold
fi
echo Copy export/TAGS.txt in $KSTART_TABLES
cp $KEXPORT/TAGS.txt $KSTART_TABLES/TAGS.txt
###############################################################################
print "${star5}\n\tcomponent.mf\n${star5}\n"
if [ -f $KWIN/component.mf ]
then
echo Rename $KWIN/component.mf in component.mf.kold
mv $KWIN/component.mf $KWIN/component.mf.kold
fi
echo Copy export/component.mf in $KWIN
cp $KEXPORT/component.mf $KWIN/component.mf
###############################################################################
print "${star5}\n\tmalf_set.mf\n${star5}\n"
if [ -f $KWIN/malf_set.mf ]
then
echo Rename $KWIN/malf_set.mf in malf_set.mf.kold
mv $KWIN/malf_set.mf $KWIN/malf_set.mf.kold
fi
echo Copy export/malf_set.mf in $KWIN
cp $KEXPORT/malf_set.mf $KWIN/malf_set.mf
###############################################################################
print "${star5}\n\ttipo_comp_malf.mf\n${star5}\n"
if [ -f $KWIN/tipo_comp_malf.mf ]
then
echo Rename $KWIN/tipo_comp_malf.mf in tipo_comp_malf.mf.kold
mv $KWIN/tipo_comp_malf.mf $KWIN/tipo_comp_malf.mf.kold
fi
echo Copy export/tipo_comp_malf.mf in $KWIN
cp $KEXPORT/tipo_comp_malf.mf $KWIN/tipo_comp_malf.mf
###############################################################################
print "${star5}\n\tsosti.dat\n${star5}\n"
if [ -f $KWIN/sosti.dat ]
then
echo Rename $KWIN/sosti.dat in sosti.dat.kold
mv $KWIN/sosti.dat $KWIN/sosti.dat.kold
fi
echo Copy export/sosti.dat in $KWIN
cp $KEXPORT/sosti.dat $KWIN/sosti.dat
###############################################################################
print "${star5}\n\tcai_var.dat\n${star5}\n"
if [ -f $KSIM/cai_var.dat ]
then
echo Rename $KSIM/cai_var.dat in cai_var.dat.kold
mv $KSIM/cai_var.dat $KSIM/cai_var.dat.kold
fi
echo Copy export/cai_var.dat in $KSIM
cp $KEXPORT/cai_var.dat $KSIM/cai_var.dat
###############################################################################
print "${star5}\n\tmgw.conf\n${star5}\n"
if [ -f $KWIN/mgw.conf ]
then
echo Rename $KWIN/mgw.conf in mgw.conf.kold
mv $KWIN/mgw.conf $KWIN/mgw.conf.kold
fi
echo Copy export/mgw.conf in $KWIN
cp $KEXPORT/mgw.con $KWIN/mgw.conf
###############################################################################
print "${star5}\n\tmgw.descr\n${star5}\n"
if [ -f $KWIN/mgw.descr ]
then
echo Rename $KWIN/mgw.descr in mgw.descr.kold
mv $KWIN/mgw.descr $KWIN/mgw.descr.kold
fi
echo Copy export/mgw.descr in $KWIN
cp $KEXPORT/mgw.des $KWIN/mgw.descr
###############################################################################
print "${star5}\n\tgrugraf.txt\n${star5}\n"
if [ -f $KGRAF/grugraf.txt ]
then
echo Rename $KGRAF/grugraf.txt in grugraf.txt.kold
mv $KGRAF/grugraf.txt $KGRAF/grugraf.txt.kold
fi
echo Copy export/grugraf.txt in $KGRAF
cp $KEXPORT/grugraf.txt $KGRAF/grugraf.txt
###############################################################################
print "${star5}\n\tkksgrafi.txt\n${star5}\n"
if [ -f $KGRAF/kksgrafi.txt ]
then
echo Rename $KGRAF/kksgrafi.txt in kksgrafi.txt.kold
mv $KGRAF/kksgrafi.txt $KGRAF/kksgrafi.txt.kold
fi
echo Copy export/kksgrafi.txt in $KGRAF
cp $KEXPORT/kksgrafi.txt $KGRAF/kksgrafi.txt
###############################################################################
print "${star5}\n\tvargraf.txt\n${star5}\n"
if [ -f $KGRAF/vargraf.txt ]
then
echo Rename $KGRAF/vargraf.txt in vargraf.txt.kold
mv $KGRAF/vargraf.txt $KGRAF/vargraf.txt.kold
fi
echo Copy export/vargraf.txt in $KGRAF
cp $KEXPORT/vargraf.txt $KGRAF/vargraf.txt
###############################################################################
print "${star5}\n\tR20.txt\n${star5}\n"
if [ -f $KINFOTAG/R20.txt ]
then
echo Rename $KINFOTAG/R20.txt in R20.txt.kold
mv $KINFOTAG/R20.txt $KINFOTAG/R20.txt.kold
fi
echo Copy export/R20.txt in $KINFOTAG
cat $KEXPORT/R20.txt | tr -s '\011' ' ' | sed "s/ 0:00:00//g" | sed "s/;;/;\&;/g" | \
    sed "s/ /\&/g" | sed "s/\&\&/\&/g" | sed "s/;/ /g" | tr -s '\011' ' ' > $KINFOTAG/R20.txt
###############################################################################
print "${star5}\n\tDatabases\n${star5}\n"
print "Copy databases in $KDATABASES \n"
if [ -f $KEXPORT/*.mdb ]
then
	ls -C1 $KEXPORT/*.mdb
	cp $KEXPORT/*mdb $KDATABASES
fi
###############################################################################
print "${star5}\n\tN_win*.tab\n${star5}\n"
print "Copy N_win*.tab in $KWIN \n"
ls -C1 $KEXPORT/N_win*.tab
cp $KEXPORT/N_win*.tab $KWIN
cd $KWIN
punt2canc.sh
rm $KWIN/*.tab
###############################################################################
. ${KBIN}/kMalfunctionGroupWindow -b
cat $KLOG/kMalfunctionGroupWindow.log >> $KLOG/kExport.log
rm -f $KLOG/kMalfunctionGroupWindow.log
kAddScreen kExport End
kAddLog kExport End
print "Log File :\t$KLOG/kExport.log\a"
