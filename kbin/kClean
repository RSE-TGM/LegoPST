#!/bin/ksh
#
kAddStatistic kClean
export DEBUG="NO"
if [ "$1" = "" ]
then
killsim
fi
export DEBUG="YES"
for var in `ps -u $USER | grep client_mmi | grep -v grep |  awk ' {print $1} '`
do
        kill -9 $var
        echo Kill client_mmi $var
done
for var in `ps -u $USER | grep client_scada | grep -v grep | awk ' {print $1} '`
do
        kill -9 $var
        echo Kill client_scada $var
done
for var in ` ipcs  | grep $USER | grep -v grep | awk ' /q/ { print $2 } ' `
do
        ipcrm -q $var
        echo Clean Message Queues $var
done
for var1 in ` ipcs | grep $USER | grep -v $SHR_TAV_KEY | \
              grep -v 0x3e7  | grep -v grep | awk ' /m/ { print $2 } ' `
do
        ipcrm -m $var1
        echo Clean Shared Memory $var1
done
for var2 in ` ipcs | grep $USER | grep -v $SHR_TAV_KEY | \
              grep -v 0x3e7  | grep -v grep | awk ' /s/ { print $2 } ' `
do
        ipcrm -s $var2
        echo Clean Semaphores $var2
done
for var in `ps | grep scada | grep -v grep | awk ' {print $1} '`
do
        kill -9 $var
        echo Kill Scada $var
done
for var in `ps | grep mmi  | grep -v grep | awk ' {print $1} '`
do
        kill -9 $var
        echo Kill mmi $var
done
for var in `ps | grep Xlego | grep -v grep | awk ' {print $1} '`
do
        kill -9 $var
        echo Kill Xlego $var
done
for var in `ps -ef | grep UtilPrint | grep -v grep | awk ' {print $2} '`
do
        kill -9 $var
        echo Kill UtilPrint $var
done
for var in `ps | grep kStat | grep -v grep | awk ' {print $1} '`
do
        kill -9 $var
        echo Kill kStat $var
done
for var in `ps | grep kLeeF22 | grep -v grep | awk ' {print $1} '`
do
        kill -9 $var
        echo Kill kLeeF22 $var
done
#
if [ -f killsim.out ]
then 
rm killsim.out
fi
# STATUS
kAddStatus kClean OK
#kAddStatus kScd Reset
#kAddStatus kStart Reset
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
