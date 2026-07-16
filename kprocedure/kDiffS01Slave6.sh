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
#	Database Creation for GIPS tasks
###############################################################################
print "\n${star5}"
print "\n${star5}" >> $KLOG/kDiffS01.log
kAddScreen kDiffS01 "Database Creation for GIPS tasks ..."
kAddLog kDiffS01 "Database Creation for GIPS tasks ..."
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
	DIRINI=`grep "Initialisation directory" proc/par_sim.info | cut -f2 -d":" | tr -s '\011' ' '| tr -d ' ' | tr -d '\015'`
	DirDB=`grep "Configuration directory" proc/task.info | cut -f2 -d":" | tr -s '\011' ' '| tr -d ' ' | tr -d '\015'`
	TagTask=`grep "Task tag" proc/task.info | cut -f2 -d":" | tr -s '\011' ' '| tr -d ' ' | tr -d '\015'`
#
#	non chiamateci pazzi !!!1
#
	rm gino
	echo "cd " $DIRINI  > gino
	chmod a+x gino
	. gino
	DIRINI=`pwd`
	cd - 	
	rm gino
	echo "cd " $DirDB  > gino
	chmod a+x gino
	. gino
	DirDB=`pwd`
	echo $DIRINI $DirDB
	cd - 	
# nodi.ini
	if [ -f $DIRINI/nodi.ini ]
	then
	grep -v ^/ ${DIRINI}/nodi.ini | while read Node sep1 Pressure sep2 TType sep3 TVar sep4 Tmetallo sep5 Corrente sep6
	do

	PressureN=`echo "${Pressure}*100000" | bc`
	TTypeN=$TType
	if [ "$TType" = "T" ]
	then
	TVarN=`echo "${TVar}+273.15" | bc`
	else
		if [ "$TType" = "H" ]
		then
		TVarN=$TVar
		fi
	fi
	TmetalloN=`echo "${Tmetallo}+273.15" | bc`
	CorrenteN=$Corrente
		
	NodeName=`grep -w ${Node} ${DirDB}/modules_names.dat | grep ^N | cut -f3 | tr -d '\015'` 
	Variable=`echo "P${TagTask}${NodeName}"`
	Variable2=`echo "P${NodeName}${TagTask}"`
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	Variable2Description=`grep -w ${Variable2} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '` 
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	echo "${Variable}${Space}${PressureN}${space10}${VariableDescription}" >> kDiffS01.DB
	echo "${Variable2}${Space}${PressureN}${space10}${Variable2Description}" >> kDiffS01.DB
#
	if [ "$TType" = "T" ]
	then
	Variable=`echo "T${TagTask}${NodeName}"`
	Variable2=`echo "T${NodeName}${TagTask}"`
	else
		if [ "$TType" = "H" ]
		then
		Variable=`echo "H${TagTask}${NodeName}"`
		Variable2=`echo "H${NodeName}${TagTask}"`
		fi
	fi
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	Variable2Description=`grep -w ${Variable2} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '` 
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	echo "${Variable}${Space}${TVarN}${space10}${VariableDescription}" >> kDiffS01.DB
	echo "${Variable2}${Space}${TVarN}${space10}${Variable2Description}" >> kDiffS01.DB
#
	Variable=`echo "TM${TagTask}${NodeName}"`
	Variable2=`echo "TM${NodeName}${TagTask}"`
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	Variable2Description=`grep -w ${Variable2} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '` 
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	echo "${Variable}${Space}${TmetalloN}${space10}${VariableDescription}" >> kDiffS01.DB
	echo "${Variable2}${Space}${TmetalloN}${space10}${Variable2Description}" >> kDiffS01.DB
#
	Variable=`echo "CORRENTE${NodeName}"`
	echo "${Variable}${Space}${CorrenteN}${space10}" >> kDiffS01.DB	
#		
	done
	fi
	echo fine nodi
# rami.ini
	if [ -f ${DIRINI}/rami.ini ]
	then	
	grep -v ^/ ${DIRINI}/rami.ini | while read Ramo sep1 Flow sep2 TType sep2plus TVar sep3 Table sep4 Composition sep5 Valve1 sep6 Valve2 sep7 Valve3 sep8 Valve4 sep9 pipo1 sep10 pipo2 sep11 pipo3 sep12 pipo4
	do
	TTypeN=$TType
	if [ "$TType" = "T" ]
	then
	TVarN=`echo "${TVar}+273.15" | bc`
	else
		if [ "$TType" = "H" ]
		then
		TVarN= $TVar
		fi
	fi
	
	RamoName=`grep -w ${Ramo} ${DirDB}/modules_names.dat | grep ^B | cut -f3 | tr -d '\015'`	
	
	Variable=`echo "W${TagTask}${RamoName}"`
	Variable2=`echo "W${RamoName}${TagTask}"`
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	Variable2Description=`grep -w ${Variable2} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	echo "${Variable}${Space}${Flow}${space10}${VariableDescription}" >> kDiffS01.DB
	echo "${Variable2}${Space}${Flow}${space10}${Variable2Description}" >> kDiffS01.DB
#	
	if [ "$TType" = "T" ]
	then
		Variable=`echo "T${TagTask}${RamoName}"`
		Variable2=`echo "T${RamoName}${TagTask}"`
	else
		if [ "$TType" = "H" ]
		then
		Variable=`echo "H${TagTask}${RamoName}"`
		Variable2=`echo "H${RamoName}${TagTask}"`
		fi
	fi
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	Variable2Description=`grep -w ${Variable2} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '` 
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	echo "${Variable}${Space}${TVarN}${space10}${VariableDescription}" >> kDiffS01.DB
	echo "${Variable2}${Space}${TVarN}${space10}${Variable2Description}" >> kDiffS01.DB
#	
	Variable=`echo "FLUIDO${RamoName}"`
	echo "${Variable}${Space}${Table}${space10}" >> kDiffS01.DB	
	Variable=`echo "CORRENTE${RamoName}"`
	echo "${Variable}${Space}${Composition}${space10}" >> kDiffS01.DB	
#
	if [ "${Valve1}" != "," ] 
	then
		Variable=`echo "A1${TagTask}${RamoName}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "A1${RamoName}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "B1${TagTask}${RamoName}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "B1${RamoName}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve1}${space10}${VariableDescription}" >> kDiffS01.DB				
	fi
#	
	if [ "${Valve2}" != "," ] 
	then
		Variable=`echo "A2${TagTask}${RamoName}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve2}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "A2${RamoName}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve2}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "B2${TagTask}${RamoName}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve2}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "B2${RamoName}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve2}${space10}${VariableDescription}" >> kDiffS01.DB				
	fi	
#	
	if [ "${Valve3}" != "," ] 
	then
		Variable=`echo "A3${TagTask}${RamoName}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve3}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "A3${RamoName}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve3}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "B3${TagTask}${RamoName}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve3}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "B3${RamoName}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve3}${space10}${VariableDescription}" >> kDiffS01.DB				
	fi	
#	
	if [ "${Valve4}" != "," ] 
	then
		Variable=`echo "A4${TagTask}${RamoName}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve4}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "A4${RamoName}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve4}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "B4${TagTask}${RamoName}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve4}${space10}${VariableDescription}" >> kDiffS01.DB
		Variable=`echo "B4${RamoName}${TagTask}"`
		VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
		VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
		SpaceNumber=`expr 16 - ${VariableLenght} + 1`
		. kSpace ${SpaceNumber}
		echo "${Variable}${Space}${Valve4}${space10}${VariableDescription}" >> kDiffS01.DB				
	fi	
	done
	fi
	echo fine rami
# uscite.ini
	if [ -f ${DIRINI}/uscite.ini ]
	then
	grep -v ^/ ${DIRINI}/uscite.ini | while read Variable Value Unit
	do
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	echo "${Variable}${Space}${Value}${space10}${VariableDescription}" >> kDiffS01.DB
	done
	fi
# ingressi.ini
	if [ -f ${DIRINI}/ingressi.ini ]
	then
	grep -v ^/ ${DIRINI}/ingressi.ini | while read Variable Value Unit
	do
	VariableDescription=`grep -w ${Variable} ${DirDB}/descr.dat | tr -s '\011' ' ' | cut -f2- -d" "`
	VariableLenght=`echo ${Variable} | wc -m | tr -s '\011' ' '| tr -d ' '`
	ValueX=`echo ${Value} | tr -s '\011' ' '| tr -d ' '`
	SpaceNumber=`expr 16 - ${VariableLenght} + 1`
	. kSpace ${SpaceNumber}
	VariableType=`echo ${Variable} | cut -c1`
	if [ "${VariableType}" = "T" ] && [ "${Unit}" = "C" ]
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
fi
# Check
	if [ ! -f kDiffS01.DB  ]
	then
	echo pipo > kDiffS01.DB
	echo "ERROR : File kDiffS01.DB not Created"
	fi
##############################################################################
done









