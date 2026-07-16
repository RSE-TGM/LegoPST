#!/bin/ksh
#
echo "Please wait ..."
rm -f $KWIN/kCheckCaiVarPD.all
star="*********************************************************************************"
print "$star\n$KWIN/kCheckCaiVarPD.all\n$star" > $KWIN/kCheckCaiVarPD.all
print "$star\n$KWIN/kCheckCaiVarPD.pb\n$star" > $KWIN/kCheckCaiVarPD.pb
text="CaiVar not animated by alarm"
text2="CaiVar Problem"
ls $KWIN/M_S_*.pag | grep -v GR | while read page
do
	print "$star\n$text\nPlant Display : $page\n$star" >> $KWIN/kCheckCaiVarPD.all
	print "$star\n$text2\nPlant Display : $page\n$star" >> $KWIN/kCheckCaiVarPD.pb
	grep input1 $page | grep -v SUPERVIS > $page.kCheckCaiVarPD
	cat $page.kCheckCaiVarPD | while read line
	do
	 	internalTag=`echo $line | cut -d. -f1 `
	 	hier=`grep "$internalTag".objectTag: $page  | cut -d" "  -f1 | cut  -f2 `
	 	type=`grep "$internalTag".objectTag: $page  | cut -d" "  -f2 `
	 	ow=`grep $hier $KWIN/sosti.dat | cut -d","  -f1 `
	 	echo $hier $type $internalTag $ow >> $KWIN/kCheckCaiVarPD.all
	 	if [ "$ow" = "" ]
	 	then
	 	echo $hier $type $internalTag OW_NOT_FOUND >> $KWIN/kCheckCaiVarPD.pb
	 	else
	 		if [ "$type" = "F" ]
	 		then
	 		echo $hier $type $internalTag $ow WARNING_TYPE_FAULT >> $KWIN/kCheckCaiVarPD.pb
	 		fi
	 	fi
	done
	rm -f $page.kCheckCaiVarPD
done
print "The results are in files :\n\t$KWIN/kCheckCaiVarPD.pb\n\t$KWIN/kCheckCaiVarPD.all\n\a"
