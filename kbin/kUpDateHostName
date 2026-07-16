#!/bin/ksh
#
NewHost=`hostname`
#
if [ ! -f $KSIM/S01 ]
then
print "ERROR\t: File S01 not found"
exit
fi
#
echo UpDating al_sim.conf ...
OldHost=`grep MMI_HOSTNAME_SCADA $KSIM/al_sim.conf | awk -v FS="=" ' {print $2} '`
sed "s/$OldHost/$NewHost/g" $KSIM/al_sim.conf > $KSIM/al_sim.conf.NewHost
mv $KSIM/al_sim.conf.NewHost $KSIM/al_sim.conf
#
echo UpDating kMmi.cfg ...
if [ -f $KSIM/kMmi.cfg ]
then
cp $KSIM/kMmi.cfg $KSIM/kMmi.cfg.kold
sed "s/$OldHost/$NewHost/g" $KSIM/kMmi.cfg.kold > $KSIM/kMmi.cfg
else
echo "INFO : File kMmi.cfg not found"
fi
#
echo UpDating S01 ...
OldHost=`grep SCADA $KSIM/S01 | awk ' {print $2} '`
#	rm -f $KSIM/S01.NewHost
#	cat S01 | while read line
#	do
#	identification=`echo $line | cut -f1 -d" "`
#	if [ "${identification}" != "SCADA" ]
#	then
#	echo "$line" >> $KSIM/S01.NewHost
#	else
#	echo "SCADA    $NewHost"  >> $KSIM/S01.NewHost
#	fi
#	done
echo "Start_S01" > $KSIM/S01.tmp
cat $KSIM/S01 >> $KSIM/S01.tmp
awk  '/Start_S01/,/^BM/' $KSIM/S01.tmp > $KSIM/S01.NewHost
echo "SCADA    $NewHost" >> $KSIM/S01.NewHost
echo "BI       " >> $KSIM/S01.NewHost
echo "****     " >> $KSIM/S01.NewHost
grep -v Start_S01 $KSIM/S01.NewHost > $KSIM/S01
rm -f $KSIM/S01.tmp $KSIM/S01.NewHost
#
echo UpDating mmi ...
if [ -f $KPAGES/Context.ctx ]
then
cp $KPAGES/Context.ctx $KPAGES/Context.ctx.kold
OldHost=`grep hostNameS $KPAGES/Context.ctx | awk ' {print $2} '`
sed "s/$OldHost/$NewHost/g" $KPAGES/Context.ctx > $KPAGES/Context.ctx.NewHost
mv $KPAGES/Context.ctx.NewHost $KPAGES/Context.ctx
rm -f $KPAGES/Context.ctx_rtf
else
echo "INFO : General MMI not configurated"
fi
#
echo UpDating $HOME/.dt/sessions/home/dt.session
sed "s/${OldHost}/${NewHost}/g" $HOME/.dt/sessions/home/dt.session > $HOME/.dt/sessions/home/dt.session.NewHost
mv $HOME/.dt/sessions/home/dt.session.NewHost $HOME/.dt/sessions/home/dt.session
#
echo UpDating $HOME/.Xdefaults-`hostname`
OldHost=`ls $HOME/.Xdefaults-* | cut -f2 -d-`
if [ "$OldHost" != $NewHost ]
then
mv $HOME/.Xdefaults-"$OldHost" $HOME/.Xdefaults-`hostname`
fi
