#!/bin/ksh
#
WidgetType=$1
PageName=$2
if [ ! -f "$PageName" ]
then
print "\n\tCorrect Use\t: kAddWidget widgetype page"
print "\n\tExemple\t: kAddWidget ChangePage A001.pag\n\a"
exit
fi
grep ^*elenco_wid0 ${PageName} | \
sed "s/*elenco_wid0:/ /g" | \
tr '\\' ' ' | \
tr -s '\011' ' ' | \
tr '[:space:]' '[\n*]' > ${PageName}.kAddWidgetTmp
#
grep w$ ${PageName}.kAddWidgetTmp | \
tr 'w' ' ' | sort > ${PageName}.kAddWidgetTmp2
#
TagNumberMax=0
cat ${PageName}.kAddWidgetTmp2 | while read TagNumber
do
if [ "${TagNumber}" -gt "${TagNumberMax}" ]
then
TagNumberMax=${TagNumber}
fi
done
if [ "${TagNumberMax}" -eq "0" ]
then
print "Error on kAddWidget with page ${PageName}\a\a"
banner "NOK"
exit
fi
SuperTagLoc=`expr $TagNumberMax + 10`
export SuperTag=${SuperTagLoc}w
ELENCO_WID0=`grep ^*elenco_wid0 ${PageName}`
NEW_ELENCO_WID0="$ELENCO_WID0 $SuperTag $WidgetType"
OldNumWidget=`grep ^*num_widget ${PageName} | tr -s '\011' ' ' | cut -f2 -d" " `
NewNumWidget=`expr $OldNumWidget + 1`
grep -v ^*elenco_wid0 ${PageName} > ${PageName}.kAddWidgetTmp3
echo $NEW_ELENCO_WID0 >> ${PageName}.kAddWidgetTmp3
sed "s/num_widget:	$OldNumWidget/num_widget:	$NewNumWidget/g" ${PageName}.kAddWidgetTmp3 > ${PageName}
sort -o ${PageName} ${PageName}
# Clean
grep ^*elenco_wid0 ${PageName} | \
sed "s/*elenco_wid0:/ /g" | \
tr '\\' ' ' | \
tr -s '\011' ' ' | \
tr '[:space:]' '[\n*]' > ${PageName}.kAddWidgetTmp
#
grep w$ ${PageName}.kAddWidgetTmp | \
tr 'w' ' ' | sort > ${PageName}.kAddWidgetTmp2
#
NewNumWidgetTest=`cat ${PageName}.kAddWidgetTmp2 | wc -l`
if [ ! "${NewNumWidgetTest}" -eq "${NewNumWidget}" ]
then
echo  "${NewNumWidgetTest}" "${NewNumWidget}"
echo Error in kAddWidget with page ${PageName}
banner NOK
exit
fi
rm *kAddWidgetTmp*
