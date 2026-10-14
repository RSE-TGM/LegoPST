#!/bin/ksh
#
kAddStatistic kCleanLoc
${KSOURCE}/Alg_global_product-2.2_osf4.0_patch/Alg_global_product/Alg_mmi_product/bin/cleanloc
#
for var in `ps | grep kRun | grep -v grep | awk ' {print $1} '`
do
        kill -9 $var
        echo Kill kRun $var
done
for var in `ps | grep sleep | grep -v grep | awk ' {print $1} '`
do
        kill -9 $var
        echo Kill sleep $var
done
# STATUS
if [ -f $KSIM/kMmi.cfg ]
then
	cat $KSIM/kMmi.cfg | tr -s '\011' ' ' | tr -s ';' ' ' | while read IdMmi IdScada IdType MmiHost MmiUser
	do
	# Conta solo l'utente. Il test era scritto [ host ] & [ utente ], con una "&"
	# sola: il primo test finiva in background e decideva l'utente da solo.
	# Non si passa a "&&": l'host di kMmi.cfg e' quello di chi l'ha scritto, e nel
	# container (hostname nuovo a ogni avvio) o in una demo non coinciderebbe mai.
	if [ "$MmiUser" = "$USER" ]
	then
	echo "Reset kMmi_${IdMmi}"
	kAddStatus kMmi_${IdMmi} Reset
	fi
	done
fi
kAddStatus kRun Reset
echo "${star5}"
ipcs
echo "${star5}"
