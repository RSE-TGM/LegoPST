#!/bin/ksh
#
rm -f *scr
#########################################################################################
#	Input Navigation UpDating
#########################################################################################
print "$star\n\tInput Navigation UpDating\n$star"
print "$star\n\tInput Navigation UpDating\n$star" >> $KLOG/kUpDateNavigation.log
OldPage="PIPO"
grep kNavigationInput ????.pag | \
	sed "s/:/ /g" | \
	sed "s/.objectTag//g" | \
	sed "s/kNavigationInput//g" | \
	tr  -d '*' > kUpDateNavigation.in
cat kUpDateNavigation.in | while read line
do
OldPage=${Page}
Page=`echo ${line} | cut -f1 -d" "`
ChildrenTag=`echo ${line} | cut -f2 -d" "`
ParentTagTmp=`echo ${ChildrenTag} | cut -f1 -d"w"`
ParentTag="${ParentTagTmp}"w
LocalPageTagTmp=`grep ${ParentTag}.tagName ${Page} | cut -f2 -d":" | tr -d '[:space:]' `
LocalPageTag=`grep "I${LocalPageTagTmp}" variabili.edf | cut -f2 -d" "`
LinkedPageTagLong=`grep ${LocalPageTag} $KSIM/S01 | tr -s '\011' ' ' | cut -f3 -d" "`
LinkedPageTagShort=`echo ${LinkedPageTagLong} | cut -c 5-8 `
if [ ! "${LinkedPageTagShort}" = "" ]
then
LinkedPageName=`grep ${LinkedPageTagShort} $KSIM/kUpDateNavigation.DB | cut -f2 -d" "`
else
LinkedPageName=NotFoundInS01
fi
# Script Elaboration
	if [ "${Page}" != "${OldPage}" ]
	then
	echo page=${Page} >> ${Page}.scr
	print "${star}\n#\tInput\n${star}" >> ${Page}.scr
	print "${star2}\n${Page}\t${ChildrenTag}\t${LinkedPageName}"
	print "${star2}\n${Page}\t${ChildrenTag}\t${LinkedPageName}" >> $KLOG/kUpDateNavigation.log
	else
	print "\t\t${ChildrenTag}\t${LinkedPageName}"
	print "\t\t${ChildrenTag}\t${LinkedPageName}" >> $KLOG/kUpDateNavigation.log
	fi
	echo widgetname=${ChildrenTag} >> ${Page}.scr
	echo attrib=pageName >> ${Page}.scr
	echo value=${LinkedPageName} >> ${Page}.scr
done
#########################################################################################
#########################################################################################
#	Output Navigation UpDating
#########################################################################################
print "$star\n\tOutput Navigation UpDating\n$star"
print "$star\n\tOutput Navigation UpDating\n$star" >> $KLOG/kUpDateNavigation.log
OldPage="PIPO"
grep kNavigationOutput ????.pag | \
	sed "s/:/ /g" | \
	sed "s/.objectTag//g" | \
	sed "s/kNavigationOutput//g" | \
	tr  -d '*' > kUpDateNavigation.out
cat kUpDateNavigation.out | while read line
do
OldPage=${Page}
Page=`echo ${line} | cut -f1 -d" "`
ChildrenTag=`echo ${line} | cut -f2 -d" "`
ParentTagTmp=`echo ${ChildrenTag} | cut -f1 -d"w"`
ParentTag="${ParentTagTmp}"w
LocalPageTagTmp=`grep ${ParentTag}.tagName ${Page} | cut -f2 -d":" | tr -d '[:space:]' `
LocalPageTag=`grep "U${LocalPageTagTmp}" variabili.edf | cut -f2 -d" "`
LinkedPageTagLong=`grep ${LocalPageTag} $KSIM/S01 | tr -s '\011' ' ' | cut -f1 -d" "`
LinkedPageTagShort=`echo ${LinkedPageTagLong} | cut -c 5-8 `
if [ ! "${LinkedPageTagShort}" = "" ]
then
LinkedPageName=`grep ${LinkedPageTagShort} $KSIM/kUpDateNavigation.DB | cut -f2 -d" "`
else
LinkedPageName=NotFoundInS01
fi
# Script Elaboration
	if [ "${Page}" != "${OldPage}" ]
	then
		if [ ! -f ${Page}.scr ]
		then
		echo page=${Page} >> ${Page}.scr
		fi
	print "${star}\n#\tOutput\n${star}" >> ${Page}.scr
	print "${star2}\n${Page}\t${ChildrenTag}\t${LinkedPageName}"
	print "${star2}\n${Page}\t${ChildrenTag}\t${LinkedPageName}" >> $KLOG/kUpDateNavigation.log
	else
	print "\t\t${ChildrenTag}\t${LinkedPageName}"
	print "\t\t${ChildrenTag}\t${LinkedPageName}" >> $KLOG/kUpDateNavigation.log
	fi
	echo widgetname=${ChildrenTag} >> ${Page}.scr
	echo attrib=pageName >> ${Page}.scr
	echo value=${LinkedPageName} >> ${Page}.scr
done
#########################################################################################
