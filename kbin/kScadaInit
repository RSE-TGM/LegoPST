#!/bin/ksh
#
clear
kclean
$KBIN/kTest
KTEST=`cat $KSTATUS/kTest.status `
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
echo "Environement test not succesfull"
exit
fi
kpresentation
if [ -f $KLOG/kScadaInit.log ]
then
mv $KLOG/kScadaInit.log $KLOG/kScadaInit.log.kold
fi
kAddScreen kScadaInit Start
kAddLog kScadaInit Start
kclean WithoutKillsim
echo "Alterlego database used : & $ALDB_USED &"
cd $KSIM
if [ $KSCADA = "" ]
then
echo "Ambient variable KSCADA not defined"
exit
fi
if [ ! -r $KSCADA ]
then
mkdir $KSCADA
echo "Creating directory $KSCADA"
fi
if [ ! -r $KSTART_TABLES ]
then
mkdir $KSTART_TABLES
fi
rm -rf $KEDIT_TABLES
rm -rf $KOPER_TABLES
rm -rf $KFILEOP
mkdir $KEDIT_TABLES
mkdir $KOPER_TABLES
mkdir $KFILEOP
chmod u+w $KSTART_TABLES/*
ls $KSOURCE/kdefaults/scada/start_txt/*txt
ls $KSOURCE/kdefaults/scada/start_txt/*tcr
cp $KSOURCE/kdefaults/scada/start_txt/*txt $KSTART_TABLES
cp $KSOURCE/kdefaults/scada/start_txt/*tcr $KSTART_TABLES
if [ ! -f $KSTART_TABLES/ALARM.txt ]
then
echo $KSTART_TABLES/ALARM.txt NOT FOUND !!!!
exit
fi
if [ ! -f $KSTART_TABLES/TAG.txt ]
then
echo $KSTART_TABLES/TAG.txt NOT FOUND !!!!
exit
fi
cd ${KSCADA}
echo 'kPointInst'
PointInst $KSTART_TABLES $KEDIT_TABLES $KOPER_TABLES $KSIM -noinformix $1
if [ ! -f $KEDIT_TABLES/editConfDB.txt ]
then
banner "NOK"
print "\a\a\a"
kAddLog kScadaInit "ERROR: Memory fault(coredump)"
echo "ERROR $0 : `date` : Memory fault(coredump) " >> $KLOG/kScadaInit.log
exit
fi
echo 'kExpRtf'
ExpRtf $KEDIT_TABLES $KOPER_TABLES -noinformix
echo 'kToRtf'
ToRtf $KOPER_TABLES $KFILEOP -noinformix
ln -fs $KFILEOP/fnomi.rtf  $KSIM/fnomi.rtf
mv $KFILEOP/conf.cfg $KFILEOP/conf.sav
echo "$KFILEOP" > $KFILEOP/conf.cfg
if [ ! -f $KFILEOP/fnomi.rtf ]
then
banner "NOK"
print "\a\a\a"
fi
kAddScreen kScadaInit End
kAddLog kScadaInit End
echo "Log File : $KLOG/kScadaInit.log"
