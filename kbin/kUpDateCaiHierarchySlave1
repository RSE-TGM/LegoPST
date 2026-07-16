#!/bin/ksh
#
echo $star8
echo $star8 >> $KLOG/kUpDateCaiHierarchy.log
kAddLog kUpDateCaiHierarchy "Start Cai Navigation"
kAddScreen kUpDateCaiHierarchy "Start Cai Navigation"
if [ ! -f $KEXPORT/kpd.txt ]
then
echo "ERROR : File kpd.txt not found in $KEXPORT"
banner NOK
exit
fi
#
cd $KWIN
ls M_S_*.pag | while read page
	do
	rm -f ${page}.script
	grep _C$ ${page} | sed "s/.objectTag://g" | sed "s/_C//g" | tr -s '\011' ' ' | tr -d '*' | \
	while read WIDGET PLANTDISPLAY
	do
	echo widgetname=${WIDGET} >> ${page}.script
	echo hasattrib=objectTag >> ${page}.script
	echo value=${PLANTDISPLAY}_C >> ${page}.script
        echo attrib=alarmsHierarchy >> ${page}.script
        ALARMSHIERARCHY=`grep -F "${PLANTDISPLAY}" $KEXPORT/kpd.txt | cut -f2 -d" "`
        if [ "${ALARMSHIERARCHY}" = "" ]
	then
	ALARMSHIERARCHY="[-1][-1][-1][-1][-1][-1]"
	print "\a\nERROR : Hierarchy of plant display [${PLANTDISPLAY}] (present in ${page}) not found in kpd.txt"
	echo "ERROR : Hierarchy of plant display [${PLANTDISPLAY}] (present in ${page}) not found in kpd.txt" >> $KLOG/kUpDateCaiHierarchy.log
	else
	print ". \c"
	fi
        echo value=${ALARMSHIERARCHY} >> ${page}.script
	done
	if [ -s ${page}.script ]
	then
	pagmod.1.4 -p ${page} -s ${page}.script
	mv ${page}.mod ${page}
	rm ${page}.script
	fi
	done
	print "\n"
kAddLog kUpDateCaiHierarchy "End Cai Navigation"
kAddScreen kUpDateCaiHierarchy "End Cai Navigation"
echo "$star8"
echo "$star8" >> $KLOG/kUpDateCaiHierarchy.log
