#!/bin/ksh
#
kAddStatistic Kks
option=${1}
question=${2}
if [ "${option}" = "-h" ]
then
print "\n${star8}\nThe Kks command helps the user to found a tag in the simulator.\n"
echo "Options :"
print "\t-h\t: this help"
print "\t-r\t: found kks in regopages [default]"
print "\t-ow\t: found operating windows in plant displaies"
print "\t-rp\t: found remote parameters in plant displaies"
print "\t-li\t: found direct links in plant displaies"
print "\t-r20\t: found kks in R20 database"
print "\t-a\t: found ALL"
print "\n\tExemple\t: kks -a 0HAd10Cp0"
print "\n${star8}\n${star2}"
exit
fi
if [ "${question}" = "" ]
then
option="-r"
question=${1}
fi
###############################################################################
cd $KINFOTAG
if [ "${option}" = "-r" ]
then
	print "\nRegolation pages:"
	grep -i ${question} ./KKS.list | while read page kks
	do
	task=` grep -w ${page} ./TASK.list | cut -f2 -d' ' `
	tag=` grep -w ${page} ./TAG.list | cut -f2 -d' ' `
	rev=` grep -w ${page} ./REV.list | cut -f2 -d' ' `
	descr=` grep -w ${page} ./DESCR.list | cut -f2- -d' ' `
	print "|" ${task}"\t" "|" ${page} "|" ${tag} "|" ${rev} "|" ${kks} "|" ${descr} "|"
	done
elif [ "${option}" = "-ow" ]
then
	print "\nOperating windows:"
	grep -i ${question} ./OW.list | while read pd ow
	do
	pddescr=` grep -w ${pd} ./PDDESCR.list | cut -f2- -d' ' `
		if [ -f ${KEXPORT}/kpd.txt ]
		then
		pdwithoutext=` echo ${pd} | sed "s/M_S_//g" `
		pdhier=` grep -w ${pdwithoutext} ${KEXPORT}/kpd.txt | cut -f3- -d' ' `
		else
		pdhier=""
		fi
	echo "|" ${ow} "|" ${pd} "|" ${pddescr} "|" ${pdhier} "|" 
	done
elif [ "${option}" = "-rp" ]
then
	print "\nRemote parameters:"
	grep -i ${question} ./RP.list | while read pd rp
	do
	pddescr=` grep -w ${pd} ./PDDESCR.list | cut -f2- -d' ' `
	echo "|" ${rp} "|" ${pd} "|" ${pddescr} "|"
	done
elif [ "${option}" = "-li" ]
then
	print "\nDirect links:"
	grep -i ${question} ./LI.list | while read pd li
	do
	echo "|" ${pd} "|" ${li} "|"
	done

elif [ "${option}" = "-r20" ]
then
	print "\nR20 database:"
	grep -i ${question} ./R20.txt | while read pd description fa date status
	do
	if [ "${status}" = "1" ]
	then
	status="OK"
	else
	status="NOK"
	fi
	if [ "${date}" = "&" ]
	then
	date="\t"
	fi
	description=` echo ${description} | sed "s/\&/ /g" `
	echo "|" ${pd} "|" ${fa} "|" ${status} "|" ${date} "|" ${description} "|"
	done
elif [ "${option}" = "-a" ]
then
Kks -r ${question}
Kks -r20 ${question}
Kks -ow ${question}
Kks -rp ${question}
Kks -li ${question}
else
banner NOK
print "Option non configurated.\a\a\a"
fi
#echo ${star2}
