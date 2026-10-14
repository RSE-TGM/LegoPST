#!/bin/ksh
#
###############################################################################
#	VERIFICATION START
###############################################################################
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
echo ${star}
if [ ! -f $KSIM/kMmi.cfg ]
then
print "ERROR : $KSIM/kMmi.cfg not found\a\a"
sleep 2
echo "Creating $KSIM/kMmi.cfg default file"
echo "a;1;I;$HOST;$USER" > $KSIM/kMmi.cfg
echo "b;2;I;$HOST;$USER" >> $KSIM/kMmi.cfg
echo "c;3;O;$HOST;$USER" >> $KSIM/kMmi.cfg
echo "d;4;O;$HOST;$USER" >> $KSIM/kMmi.cfg
fi
if [ ! -r ${KPAGES} ]
then
print "ERROR\t: Directory ${KPAGES} not found"
banner NOK
print "\a\a\a"
exit
fi
###############################################################################
#	VERIFICATION END
###############################################################################
rm -rf ${KPAGES}_*
cat $KSIM/kMmi.cfg | tr -s '\011' ' ' | tr -s ';' ' ' | while read IdMmi IdScada IdType MmiHost MmiUser
do
# Conta solo l'utente. Il test era scritto [ host ] & [ utente ], con una "&"
# sola: il primo test finiva in background e decideva l'utente da solo.
# Non si passa a "&&": l'host di kMmi.cfg e' quello di chi l'ha scritto, e nel
# container (hostname nuovo a ogni avvio) o in una demo non coinciderebbe mai.
if [ "$MmiUser" = "$USER" ]
then
mkdir ${KPAGES}_${IdMmi}
echo Configuring MMI for user ${IdMmi}
cp $KPAGES/Context.ctx ${KPAGES}_${IdMmi}/Context.ctx.tmp
MAIN_HOST_SCADA_OLD=` grep hostNameS $KPAGES/Context.ctx `
MAIN_HOST_SCADA_NEW=` grep hostNameS $KPAGES/Context.ctx | cut -d" "  -f1 `
sed "s/$MAIN_HOST_SCADA_OLD/$MAIN_HOST_SCADA_NEW ${IdScada}/g" < ${KPAGES}_${IdMmi}/Context.ctx.tmp > ${KPAGES}_${IdMmi}/Context.ctx
rm -f ${KPAGES}_${IdMmi}/Context.ctx.tmp
fi
done

