#!/bin/ksh
#
key=$1
if [ "$key" != "7777" ]
then
echo "Sorry : Not authorised ..."
exit
fi
cd $LEGOCAD_USER/legocad
ls -d r_* | while read task
do
cd $task
echo ${star5}
echo "$task : kDangerPipo"
echo ${star5}
#
ls *.pag | while read page
		do
		grep -v nextnum ${page} > ${page}.WithoutNextNum
		print "*nextnum:\t6000" >> ${page}.WithoutNextNum
		mv ${page}.WithoutNextNum ${page}
 		done
#
cd $LEGOCAD_USER/legocad
done
print "\n\a"
