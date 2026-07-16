#!/bin/ksh
#
###############################################################################
#	Database Creation for LEGO & REGO tasks
###############################################################################
print "\n${star5}"
print "\n${star5}" >> $KLOG/kDiffS01.log
kAddScreen kDiffS01 "Database Creation for LEGO & REGO tasks ..."
kAddLog kDiffS01 "Database Creation for LEGO & REGO tasks ..."
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
	grep -v ' N ' |
	while read tok1 tok2 tok3 tok4
do
	cd ${tok1}
	print "\t\tDatabase task ${tok1} ..."
	print "\t\tDatabase task ${tok1} ..." >> $KLOG/kDiffS01.log
awk  '/CONDIZIONI INIZIALI VARIABILI DEL SISTEMA/,/CONDIZIONI INIZIALI VARIABILI DI INGRESSO/'  \
     ${KSIM}/${tok1}/proc/f24.dat | grep -v "CONDIZIONI INIZIALI" | \
     cut -c5-13,14-100 | sed  "s/=/=======/" > kDiffS01.DB
awk  '/CONDIZIONI INIZIALI VARIABILI DI INGRESSO/,/DATI FISICI E GEOMETRICI DEL SISTEMA SUDDIVISI A BLOCCHI/'  \
     ${KSIM}/${tok1}/proc/f24.dat | grep -v "CONDIZIONI INIZIALI" | \
     grep -v "DATI FISICI E GEOMETRICI" | \
     cut -c5-13,14-100 | sed  "s/=/=======/" >> kDiffS01.DB
done









