#!/bin/ksh
#
limpiar=$1
#########################################################################################
#	Not in use pages
#########################################################################################
grep -l "schemeInUse:	0" ????.pag > NotInUse.list
#########################################################################################
#	Input Navigation UpDating
#########################################################################################
print "$star\n\tInput Navigation UpDating\n$star"
print "$star\n\tInput Navigation UpDating\n$star" >> $KLOG/kUpDateNavigation.log
grep kNavigationInput ????.pag | \
	sed "s/:/ /g" | \
	sed "s/.objectTag//g" | \
	sed "s/kNavigationInput//g" | \
	tr  -d '*' | \
	tr -s '\011' ' ' | \
	grep -v "c $" |  grep -v -f NotInUse.list > kUpDateNavigation.in
cat kUpDateNavigation.in | while read line
do
OldPage=${Page}
Page=`echo ${line} | cut -f1 -d" "`
NavigationTag=`echo ${line} | cut -f2 -d" "`
yYellow=`grep ^*${NavigationTag}.y0 ${Page} | tr -s '\011' ' ' | cut -f2 -d" "`
grep r_saio_03 ${Page} | \
	cut -f1 -d":" | \
	sed "s/.nomeFile//g" > ${Page}.Intaglist
grep r_slio_03 ${Page} | \
	cut -f1 -d":" | \
	sed "s/.nomeFile//g" >> ${Page}.Intaglist
grep r_saio_02 ${Page} | \
	cut -f1 -d":" | \
	sed "s/.nomeFile//g" >> ${Page}.Intaglist
grep r_saio_06 ${Page} | \
	cut -f1 -d":" | \
	sed "s/.nomeFile//g" >> ${Page}.Intaglist
grep r_saio_07 ${Page} | \
	cut -f1 -d":" | \
	sed "s/.nomeFile//g" >> ${Page}.Intaglist
grep r_slio_06 ${Page} | \
	cut -f1 -d":" | \
	sed "s/.nomeFile//g" >> ${Page}.Intaglist
TagYellow=`grep -f ${Page}.Intaglist ${Page} | \
	grep .y0 | \
	tr -s '\011' ' ' | \
	grep ": ${yYellow}$" | \
	grep -v ^*${NavigationTag}.y0 | \
	cut -f1 -d"." | \
	tr  -d '*' `
SamePositionNumber=`echo ${TagYellow} | wc -w`
if [ ! "${SamePositionNumber}" -eq "1" ]
then
print "\n${star5}\nFatal error on page ${Page} : Two interface objets (${TagYellow}) have the same position ${yYellow} !\a\a\a\a\n${star5}\n" >> $KLOG/kUpDateNavigation.log
print "\n${star5}\nFatal error on page ${Page} : Two interface objets (${TagYellow}) have the same position ${yYellow} !\a\a\a\a\n${star5}\n"
banner "NOK"
exit
fi
LocalPageTagTmp=`grep ^*${TagYellow}.tagName ${Page} | cut -f2 -d":" | tr -d '[:space:]' `
LocalPageTag=`grep "[IJ]I${LocalPageTagTmp}" variabili.edf | cut -f2 -d" "`
if [ ! "${LocalPageTag}" = "" ]
then
LinkedPageTagLong=`grep ${LocalPageTag} $KSIM/S01 | tr -s '\011' ' ' | cut -f3 -d" "`
LinkedTaskId=`grep ${LocalPageTag} $KSIM/S01 | tr -s '\011' ' ' | cut -f2 -d" " | cut -c1 | sort | uniq`
else
banner "NOK"
print "\a\a\aTask $PWD not compilated"
echo "Problem with page [${Page}]"
echo "Detail : [${Page}] [${NavigationTag}] [${yYellow}] [${TagYellow}] [${LocalPageTagTmp}] [${LocalPageTag}]"
exit
fi
LinkedPageTagShort=`echo ${LinkedPageTagLong} | cut -c 5-8 `
if [ ! "${LinkedPageTagShort}" = "" ] && [ ! "${LinkedTaskId}" = "$" ]
then
LinkedPageName=`grep ${LinkedPageTagShort} $KSIM/kUpDateNavigation.DB | cut -f2 -d" " | sort | uniq`
else
LinkedPageName=NotFoundInS01
fi
# Script Elaboration
	if [ "${Page}" != "${OldPage}" ]
	then
	echo page=${Page} >> ${Page}.scr
	print "${star}\n#\tInput\n${star}" >> ${Page}.scr
	print "${star2}\n${Page}\t${NavigationTag}\t${LinkedPageName}"
	print "${star2}\n${Page}\t${NavigationTag}\t${LinkedPageName}" >> $KLOG/kUpDateNavigation.log
	else
	print "\t\t${NavigationTag}\t${LinkedPageName}"
	print "\t\t${NavigationTag}\t${LinkedPageName}" >> $KLOG/kUpDateNavigation.log
	fi
	echo widgetname=${NavigationTag} >> ${Page}.scr
	echo attrib=pageName >> ${Page}.scr
	echo value=${LinkedPageName} >> ${Page}.scr
	TaskLetter1='echo "${LinkedPageName}" | cut -c1'
	TaskLetter2='echo "${LinkedPageName}" | cut -c1-2'
	if [ "${TaskLetter1}" = "X" ]
	then
	echo attrib=background >> ${Page}.scr
	echo value=#0000ffff0000 >> ${Page}.scr
	echo attrib=userLevel >> ${Page}.scr
	echo value=1 >> ${Page}.scr
	elif [ "${TaskLetter2}" = "EP" -o "${TaskLetter2}" = "GI" -o "${TaskLetter2}" = "DP" ]
	then
	echo attrib=background >> ${Page}.scr
	echo value=#0000ffff0000 >> ${Page}.scr
	echo attrib=userLevel >> ${Page}.scr
	echo value=1 >> ${Page}.scr
	elif [ "${LinkedPageName}" = "NotFoundInS01" ]
	then
	echo attrib=background >> ${Page}.scr
	echo value=#ffff00000000 >> ${Page}.scr
	echo attrib=userLevel >> ${Page}.scr
	echo value=1 >> ${Page}.scr
	else
	echo attrib=background >> ${Page}.scr
	echo value=#ffffffff0000 >> ${Page}.scr
	echo attrib=userLevel >> ${Page}.scr
	echo value=0 >> ${Page}.scr
	fi
if [ ! "${limpiar}" = "NOLIMPIAR" ]
then
rm ${Page}.Intaglist
fi
done
if [ ! "${limpiar}" = "NOLIMPIAR" ]
then
rm kUpDateNavigation.in
fi
#########################################################################################
#########################################################################################
#	Output Navigation UpDating
#########################################################################################
print "$star\n\tOutput Navigation UpDating\n$star"
print "$star\n\tOutput Navigation UpDating\n$star" >> $KLOG/kUpDateNavigation.log
grep kNavigationOutput ????.pag | \
	sed "s/:/ /g" | \
	sed "s/.objectTag//g" | \
	sed "s/kNavigationOutput//g" | \
	tr  -d '*' | \
	tr -s '\011' ' ' | \
	grep -v "c $"  | grep -v -f NotInUse.list > kUpDateNavigation.out
cat kUpDateNavigation.out | while read line
do
OldPage=${Page}
Page=`echo ${line} | cut -f1 -d" "`
NavigationTag=`echo ${line} | cut -f2 -d" "`
yYellow=`grep ^*${NavigationTag}.y0 ${Page} | tr -s '\011' ' ' | cut -f2 -d" "`
#
grep r_saio_04 ${Page} | cut -f1 -d":" | sed "s/.nomeFile//g" > ${Page}.Outtaglist
grep r_slio_02 ${Page} | cut -f1 -d":" | sed "s/.nomeFile//g" >> ${Page}.Outtaglist
#
# SPS Start
grep r_saio_08 ${Page} | \
	cut -f1 -d":" | \
	sed "s/.nomeFile//g" >> ${Page}.Outtaglist
grep r_slio_08 ${Page} | \
	cut -f1 -d":" | \
	sed "s/.nomeFile//g" >> ${Page}.Outtaglist
# SPS End	
TagYellow=`grep -f ${Page}.Outtaglist ${Page} | \
	grep .y0 | \
	tr -s '\011' ' ' | \
	grep ": ${yYellow}$" | \
	grep -v ^*${NavigationTag}.y0 | \
	cut -f1 -d"." | \
	tr  -d '*' `
SamePositionNumber=`echo ${TagYellow} | wc -w`
if [ ! "${SamePositionNumber}" -eq "1" ]
then
print "\n${star5}\nFatal error on page ${Page} : Two interface objets (${TagYellow}) have the same position ${yYellow} !\a\a\a\a\n${star5}\n" >> $KLOG/kUpDateNavigation.log
print "\n${star5}\nFatal error on page ${Page} : Two interface objets (${TagYellow}) have the same position ${yYellow} !\a\a\a\a\n${star5}\n"
banner "NOK"
exit
fi
LocalPageTagTmp=`grep ^*${TagYellow}.tagName ${Page} | cut -f2 -d":" | tr -d '[:space:]' `
LocalPageTag=`grep "[UV]U${LocalPageTagTmp}" variabili.edf | cut -f2 -d" "`
if [ ! "${LocalPageTag}" = "" ]
then
LinkedPageTagLong=`grep ${LocalPageTag} $KSIM/S01 | tr -s '\011' ' ' | cut -f1 -d" "`
LinkedTaskId=`grep ${LocalPageTag} $KSIM/S01 | tr -s '\011' ' ' | cut -f2 -d" " | cut -c1 | sort | uniq`
else
banner "NOK"
print "Task $PWD (page $Page) not compilated\a\a\a"
fi
LinkedPageTagShort=`echo ${LinkedPageTagLong} | cut -c 5-8 `
if [ ! "${LinkedPageTagShort}" = "" ] && [ ! "${LinkedTaskId}" = "$" ]
then
LinkedPageName=`grep ${LinkedPageTagShort} $KSIM/kUpDateNavigation.DB | cut -f2 -d" " | sort | uniq`
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
	print "${star2}\n${Page}\t${NavigationTag}\t${LinkedPageName}"
	print "${star2}\n${Page}\t${NavigationTag}\t${LinkedPageName}" >> $KLOG/kUpDateNavigation.log
	else
	print "\t\t${NavigationTag}\t${LinkedPageName}"
	print "\t\t${NavigationTag}\t${LinkedPageName}" >> $KLOG/kUpDateNavigation.log
	fi
	echo widgetname=${NavigationTag} >> ${Page}.scr
	echo attrib=pageName >> ${Page}.scr
	echo value=${LinkedPageName} >> ${Page}.scr
	TaskLetter1='echo "${LinkedPageName}" | cut -c1'
	TaskLetter2='echo "${LinkedPageName}" | cut -c1-2'
	if [ "${TaskLetter1}" = "X" ]
	then
	echo attrib=background >> ${Page}.scr
	echo value=#0000ffff0000 >> ${Page}.scr
	echo attrib=userLevel >> ${Page}.scr
	echo value=1 >> ${Page}.scr
	elif [ "${TaskLetter2}" = "EP" -o "${TaskLetter2}" = "GI" -o "${TaskLetter2}" = "DP" ]
	then
	echo attrib=background >> ${Page}.scr
	echo value=#0000ffff0000 >> ${Page}.scr
	echo attrib=userLevel >> ${Page}.scr
	echo value=1 >> ${Page}.scr
	elif [ "${LinkedPageName}" = "NotFoundInS01" ]
	then
	echo attrib=background >> ${Page}.scr
	echo value=#ffff00000000 >> ${Page}.scr
	echo attrib=userLevel >> ${Page}.scr
	echo value=1 >> ${Page}.scr
	else
	echo attrib=background >> ${Page}.scr
	echo value=#ffffffff0000 >> ${Page}.scr
	echo attrib=userLevel >> ${Page}.scr
	echo value=0 >> ${Page}.scr
	fi
if [ ! "${limpiar}" = "NOLIMPIAR" ]
then
rm ${Page}.Outtaglist
fi
done
if [ ! "${limpiar}" = "NOLIMPIAR" ]
then
rm kUpDateNavigation.out
fi
