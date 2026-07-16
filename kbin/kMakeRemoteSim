#!/bin/ksh
#
IdKey=`echo $USER | grep oper | wc -l | tr -s '\011' ' ' | tr  -d '[:space:]'`
if [ "$IdKey" = "1" ]
then
cd ${KSIM}
echo "Cleaning ${KSIM}"
rm -rf ${KSIM}/f22_fgraf.edf ${KSIM}/f22circ.dat ${KSIM}/kMmi.cfg ${KSIM}/al_sim.conf ${KSIM}/scada ${KSIM}/log ${KSIM}/status ${KINFO}
FILES_REMOTED="f22_fgraf.edf f22circ.dat kMmi.cfg al_sim.conf scada log status .info"
if [ ! -e /usr/users_mount/RemoteSim/al_sim.conf ]
then
print "\nERROR\t: The simulator is not mounted in /usr/users_mount/RemoteSim !\a\a"
banner "NOK"
exit
fi
for file in $FILES_REMOTED
do
if [ -e /usr/users_mount/RemoteSim/$file ]
then
	echo "Linking /usr/users_mount/RemoteSim/$file"
	ln -fs /usr/users_mount/RemoteSim/$file .
else
	echo "ERROR : File /usr/users_mount/RemoteSim/$file not found !!!"
	exit
fi
done
kMmiConfig
else
echo "This shell can only be used by operartors"
fi

