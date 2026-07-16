#!/bin/ksh
#
WidgetType=$1
ParentTag=$2
PageName=$3
if [ ! -f "$PageName" ]
then
print "\n\tCorrect Use\t: kAddChildrenWidget WidgeType ParentTag PageName"
print "\n\tExemple\t: kAddChildrenWidget ChangePage 100w A001.pag\n\a"
exit
fi
grep ^*${ParentTag}.listChildren: ${PageName} | \
	sed "s/*${ParentTag}.listChildren:/ /g" | \
	tr '\\' ' ' | \
	tr -s '\011' ' ' | \
	tr '[:space:]' '[\n*]' > ${PageName}.kAddChildrenWidgetTmp
#
grep c$ ${PageName}.kAddChildrenWidgetTmp | \
	tr 'c' ' ' | \
	cut -f2 -d"w" | \
	sort > ${PageName}.kAddChildrenWidgetTmp2
#
TagNumberMax=0
cat ${PageName}.kAddChildrenWidgetTmp2 | while read TagNumber
do
if [ "${TagNumber}" -gt "${TagNumberMax}" ]
then
TagNumberMax=${TagNumber}
fi
done
SuperTagLoc=`expr $TagNumberMax + 1`
export SuperTag=${SuperTagLoc}c
LIST_CHILDREN=`grep ^*${ParentTag}.listChildren: ${PageName}`
NEW_LIST_CHILDREN="$LIST_CHILDREN ${ParentTag}${SuperTag} $WidgetType"
OldNumFigli=`grep ^*${ParentTag}.numFigli: ${PageName} | tr -s '\011' ' ' | cut -f2 -d" " `
NewNumFigli=`expr $OldNumFigli + 1`
grep -v ^*${ParentTag}.listChildren: ${PageName} > ${PageName}.kAddChildrenWidgetTmp3
echo $NEW_LIST_CHILDREN >> ${PageName}.kAddChildrenWidgetTmp3
sed "s/${ParentTag}.numFigli:	$OldNumFigli/${ParentTag}.numFigli:	$NewNumFigli/g" ${PageName}.kAddChildrenWidgetTmp3 > ${PageName}
sort -o ${PageName} ${PageName}
# Clean
grep ^*${ParentTag}.listChildren: ${PageName} | \
	sed "s/*${ParentTag}.listChildren:/ /g" | \
	tr '\\' ' ' | \
	tr -s '\011' ' ' | \
	tr '[:space:]' '[\n*]' > ${PageName}.kAddChildrenWidgetTmp
#
grep c$ ${PageName}.kAddChildrenWidgetTmp | \
	tr 'c' ' ' | \
	cut -f2 -d"w" | \
	sort > ${PageName}.kAddChildrenWidgetTmp2

#
NewNumFigliTest=`cat ${PageName}.kAddChildrenWidgetTmp2 | wc -l`
if [ ! "${NewNumFigliTest}" -eq "${NewNumFigli}" ]
then
banner ERROR
echo  "${NewNumFigliTest}" "${NewNumFigli}"
exit
fi
rm *kAddChildrenWidgetTmp*
