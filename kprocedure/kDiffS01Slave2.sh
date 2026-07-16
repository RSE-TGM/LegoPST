#!/bin/ksh
#
	cd ${KSIM}
	space0=""
	space1=" "
	space2="  "
	space3="   "
	space4="    "
	space5="     "
	space6="      "
	space7="       "
	space8="        "
	space9="         "
	space10="          "
	space11="           "
	space12="            "
	space13="             "
	space14="              "
	space15="               "
	space16="                "
##############################################################################
#	Database Creation for SID tasks
###############################################################################
print "\n${star5}"
print "\n${star5}" >> $KLOG/kDiffS01.log
kAddScreen kDiffS01 "Database Creation for SID tasks ..."
kAddLog kDiffS01 "Database Creation for SID tasks ..."
print "${star5}\n"
print "${star5}\n" >> $KLOG/kDiffS01.log
count=0
cat S01 |
while read var
do
	if [ $count -lt 3 ]
	then
	if [ "${var}" = '****' ]
	then 
	let count=count+1 
	fi
	continue
	fi
	if [ "${var}" = '****' ]
	then 
	break 
	fi
	echo ${var}
done |
	grep ' N ' |
	while read tok1 tok2 tok3 tok4
do
	cd ${tok1}
	print "\t\tDatabase task ${tok1} ..."
	print "\t\tDatabase task ${tok1} ..." >> $KLOG/kDiffS01.log
##############################################################################	
	rm -f kDiffS01.DB
if [ -s proc/task.info ]
then
	DirIni=`grep "Direttorio di inizializzazione -->" proc/task.info | cut -f2 -d">" | tr -s '\011' ' '| tr -d ' '`
	DirDB=`grep "Direttorio di configurazione" proc/task.info | cut -f2 -d">" | tr -s '\011' ' '| tr -d ' '`
	TagTask=`grep "Sigla della task" proc/task.info | cut -f2 -d">" | tr -s '\011' ' '| tr -d ' '`
# nodo.ini
	if [ -f ${DirIni}/nodi.ini ]
	then
	grep -v ^/ ${DirIni}/nodi.ini | while read Node sep1 Pressure sep2 Temperature sep3
	do
	PressureN=`echo "${Pressure}*100000" | bc`
	TemperatureN=`echo "${Temperature}+273.15" | bc`
	Variable=`echo "P${TagTask}N${Node}"`
	Variable2=`echo "PN${Node}${TagTask}"`
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	Variable2Description=`grep -w ${Variable2} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '` 
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	echo "${Variable}${Space}${PressureN}${space10}${VariableDescription}" >> kDiffS01.DB
	echo "${Variable2}${Space}${PressureN}${space10}${Variable2Description}" >> kDiffS01.DB
#
	Variable=`echo "T${TagTask}N${Node}"`
	Variable2=`echo "TN${Node}${TagTask}"`
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	Variable2Description=`grep -w ${Variable2} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '` 
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	echo "${Variable}${Space}${TemperatureN}${space10}${VariableDescription}" >> kDiffS01.DB
	echo "${Variable2}${Space}${TemperatureN}${space10}${Variable2Description}" >> kDiffS01.DB
#
	Variable=`echo "HN${Node}${TagTask}"`
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '` 
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	echo "${Variable}${Space}${TemperatureN}${space10}${VariableDescription}" >> kDiffS01.DB
	done
	fi
# rami.ini
	if [ -f ${DirIni}/rami.ini ]
	then	
	grep -v ^/ ${DirIni}/rami.ini | while read Ramo sep1 Flow sep2 Temperature sep3 Table sep4 Composition sep5 Valve1 sep6 Valve2 sep7 Valve3 sep8 Valve4 sep9 pipo1 sep10 pipo2 sep11 pipo3 sep12 pipo4
	do
	TemperatureN=`echo "${Temperature}+273.15" | bc`
	Variable=`echo "W${TagTask}R${Ramo}"`
	Variable2=`echo "WR${Ramo}${TagTask}"`
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	Variable2Description=`grep -w ${Variable2} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	echo "${Variable}${Space}${Flow}${space10}${VariableDescription}" >> kDiffS01.DB
	echo "${Variable2}${Space}${Flow}${space10}${Variable2Description}" >> kDiffS01.DB
#	
	Variable=`echo "T${TagTask}R${Ramo}"`
	Variable2=`echo "TR${Ramo}${TagTask}"`
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	Variable2Description=`grep -w ${Variable2} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '` 
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	echo "${Variable}${Space}${TemperatureN}${space10}${VariableDescription}" >> kDiffS01.DB
	echo "${Variable2}${Space}${TemperatureN}${space10}${Variable2Description}" >> kDiffS01.DB
	#	
	Variable=`echo "HR${Ramo}${TagTask}"`
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '` 
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	echo "${Variable}${Space}${TemperatureN}${space10}${VariableDescription}" >> kDiffS01.DB
	if [ "${Valve1}" != "," ] && [ "${Valve2}" != "," ]
	then
		if [ "${Valve3}" != "," ] && [ "${Valve4}" != "," ]
		then
#		echo "4 valves : ${Valve1} ${Valve2} ${Valve3} ${Valve4}"
		Variable=`echo "A1${TagTask}${Ramo}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "A1${Ramo}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "A1L${TagTask}${Ramo}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "A1L${Ramo}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
#		
		Variable=`echo "A2${TagTask}${Ramo}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "A2${Ramo}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "A2L${TagTask}${Ramo}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "A2L${Ramo}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		else
#		echo "2 valves : ${Valve1} ${Valve2} ${Valve3} ${Valve4}"
		Variable=`echo "AV${TagTask}${Ramo}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "AV${Ramo}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "AVL${TagTask}${Ramo}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "AVL${Ramo}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
#		
		Variable=`echo "A${TagTask}${Ramo}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "A${Ramo}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "AL${TagTask}${Ramo}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "AL${Ramo}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		fi
	fi
	done
	fi
# uscite.ini
	if [ -f ${DirIni}/uscite.ini ]
	then
	grep -v ^/ ${DirIni}/uscite.ini | while read Variable Value Unit
	do
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	echo "${Variable}${Space}${Value}${space10}${VariableDescription}" >> kDiffS01.DB
	done
	fi
# ingressi.ini
	if [ -f ${DirIni}/ingressi.ini ]
	then
	grep -v ^/ ${DirIni}/ingressi.ini | while read Variable Value Unit
	do
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
	ValueX=`echo ${Value} | tr -s '\011' ' '| tr -d ' '`
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	VariableType=`echo ${Variable} | cut -c1`
	if [ "${VariableType}" = "T" ]
	then
#	echo "start pipo @${ValueX}@"
	ValueN=`echo "${ValueX}+273.15" | bc`
#	echo "end pipo @${ValueN}@"
	else
	ValueN=${ValueX}
	fi
	echo "${Variable}${Space}${ValueN}${space10}${VariableDescription}" >> kDiffS01.DB
	done
	fi
else
	if [ -f rest.fil ]
	then
	print "\t\t\tGasifier Task"
	print "\t\t\tNot configured"
	echo pipo > kDiffS01.DB
	else
		if [ -f proc/n04.dat ]
		then
		print "\t\t\tTreatment Task"
		grep -v ^/ proc/n04.dat | while read Variable Value Description
		do
		if [ ! "${Value}" = ""  ]
		then
		VariableDescription=`grep -w ^${Variable} proc/n04.dat | tr -s '\011' ' ' | cut -f3- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Value}${space10}${VariableDescription}" >> kDiffS01.DB
		fi
		done
		else
		pwd
		echo "proc/task.info not found"
		fi
	fi
fi
# Check
	if [ ! -f kDiffS01.DB  ]
	then
	echo pipo > kDiffS01.DB
	echo "ERROR : File kDiffS01.DB not Created"
	fi
##############################################################################
done









